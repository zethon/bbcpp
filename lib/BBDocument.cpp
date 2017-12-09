#include "BBDocument.h"

namespace bbcpp
{

BBNode::BBNode(NodeType nodeType, const std::string& name)
    : _name(name), _nodeType(nodeType)
{
    // nothing to do
}

BBText &BBDocument::newText(const std::string &text)
{
#if COCOS2D_DEBUG > 0
#endif
    // first try to append this text to the item on top of the stack
    // if that is a BBText object, if not, then see if the last element
    // pushed to BBDocument is a text item, and if so append this to that
    // text
    if (_stack.size() > 0 && _stack.top()->getChildren().size() > 0)
    {
        int totalChildCnt = _stack.top()->getChildren().size();
        auto textnode = _stack.top()->getChildren().at(totalChildCnt - 1)->downCast<BBTextPtr>(false);
        if (textnode)
        {
            textnode->append(text);
            return *textnode;
        }
    }
    else if (_children.size() > 0)
    {
        auto textnode = _children.back()->downCast<BBTextPtr>(false);
        if (textnode)
        {
            textnode->append(text);
            return *textnode;
        }
    }

    // ok, there was no previous text element so we wil either add this text
    // element as a child of the top item OR we'll add it to the BBDocucment
    // object
    auto textNode = std::make_shared<BBText>(text);
    if (_stack.size() > 0)
    {
        _stack.top()->appendChild(textNode);
    }
    else
    {
        // add this node to the document-node if needed
        appendChild(textNode);
    }

    return *textNode;
}

BBElement& BBDocument::newElement(const std::string &name)
{
#if COCOS2D_DEBUG > 0
#endif
    auto newNode = std::make_shared<BBElement>(name);
    if (_stack.size() > 0)
    {
        _stack.top()->appendChild(newNode);  
    }
    else
    {
        // add this node to the document-node if needed
        appendChild(newNode);
    }
 
    _stack.push(newNode);
    return *newNode;
}

BBElement& BBDocument::newClosingElement(const std::string& name)
{
#if COCOS2D_DEBUG > 0
#endif
    auto newNode = std::make_shared<BBElement>(name, BBElement::CLOSING);
    if (_stack.size() > 0)
    {
        _stack.top()->appendChild(newNode);  
        _stack.pop();
    }
    else
    {
        appendChild(newNode);
    }

    return *newNode;
}

BBElement& BBDocument::newKeyValueElement(const std::string& name, const ParameterMap& pairs)
{
#if COCOS2D_DEBUG > 0
#endif
    auto newNode = std::make_shared<BBElement>(name, BBElement::PARAMETER);
    if (_stack.size() > 0)
    {
        _stack.top()->appendChild(newNode);
    }
    else
    {
        // add this node to the document-node if needed
        appendChild(newNode);
    }

    for (const auto& kv : pairs)
    {
        newNode->setOrAddParameter(kv.first, kv.second);
    }

    _stack.push(newNode);
    return *newNode;
}
  
    std::string nodeTypeToString(BBNode::NodeType type)
    {
        std::string retval = "Unknown";
        
        switch (type)
        {
            case BBNode::NodeType::DOCUMENT:
                retval = "Document";
                break;
                
            case BBNode::NodeType::ELEMENT:
                retval = "Element";
                break;
                
            case BBNode::NodeType::TEXT:
                retval = "Text";
                break;
                
            default:
                break;
        }
        
        return retval;
    }
    
    // Helper Functions
    std::string getIndentString(const uint indent)
    {
        std::stringstream output;
        
        for (auto i = 0; i < indent; i++)
        {
            output << "|   ";
        }
        
        output << "|-- ";
        return output.str();
    }
    
    void printParameters(const ParameterMap& pairs, const uint indent)
    {
#if COCOS2D_DEBUG > 0
        for (const auto& kv : pairs)
        {
            std::cout
            << getIndentString(indent+1)
            << "{" << kv.first << "=" << kv.second << "}"
            << std::endl;
        }
#endif
    }
    
    void printChildren(const BBNode& parent, uint indent)
    {
#if COCOS2D_DEBUG > 0
        for (const auto node : parent.getChildren())
        {
            switch (node->getNodeType())
            {
                default:
                    break;
                    
                case BBNode::ELEMENT:
                {
                    const auto element = node->downCast<BBElementPtr>();
                    std::cout
                    << getIndentString(indent)
                    << "["
                    << (element->getElementType() == BBElement::CLOSING ? "/" : "")
                    << element->getNodeName() << "]"
                    << std::endl;
                    
                    if (element->getElementType() == BBElement::PARAMETER)
                    {
                        printParameters(element->getParameters(), indent);
                    }
                }
                    break;
                    
                case BBNode::TEXT:
                {
                    const auto textnode = node->downCast<BBTextPtr>();
                    std::cout << getIndentString(indent)
                    << "@\"" << textnode->getText() << "\""
                    << std::endl;
                }
                    break;
            }
            
            printChildren(*node, indent+1);
        }
#endif
    }
    
    void printDocument(const BBDocument& doc)
    {
#if COCOS2D_DEBUG > 0
        std::cout << "#document" << std::endl;
        
        auto indent = 0u;
        printChildren(doc, indent);
#endif
    }

    std::string getRawString(const BBNode& parent)
    {
        std::string root = "";
        for (const auto node : parent.getChildren())
        {
            switch (node->getNodeType())
            {
                default:
                    break;
                    
                case BBNode::ELEMENT:
                {
                    const auto element = node->downCast<BBElementPtr>();
                    
                    if (element->getElementType() == BBElement::PARAMETER)
                    {
                    }
                }
                    break;
                    
                case BBNode::TEXT:
                {
                    const auto textnode = node->downCast<BBTextPtr>();
                    root += textnode->getText();
                }
                    break;
            }
            
            root += getRawString(*node);
        }

        return root;
    }

} // namespace
