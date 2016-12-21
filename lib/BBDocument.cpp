#include "BBDocument.h"

BBNode::BBNode(NodeType nodeType, const std::string& name)
    : _name(name), _nodeType(nodeType)
{
    // nothing to do
}

BBText &BBDocument::newText(const std::__1::string &text)
{
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
