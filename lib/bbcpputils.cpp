#include "BBDocument.h"
#include "bbcpputils.h"

namespace bbcpp
{

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
std::string getIndentString(const unsigned int indent)
{
    std::stringstream output;
    
    for (unsigned int i = 0; i < indent; i++)
    {
        output << "|   ";
    }
    
    output << "|-- ";
    return output.str();
}

void printChildren(const BBNode& parent, unsigned int indent)
{
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
                    std::cout 
                        << getIndentString(indent + 1)
                        << element->getParameters()
                        << std::endl;
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
}

void printDocument(const BBDocument& doc)
{
    std::cout << "#document" << std::endl;
    
    auto indent = 0u;
    printChildren(doc, indent);
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