#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

#include <iostream>

class BBNode;
class BBDocument;

using BBNodePtr = std::shared_ptr<BBNode>;
using BBNodeWeakPtr = std::weak_ptr<BBNode>;
using BBNodeList = std::vector<BBNodePtr>;
using BBNodeStack = std::stack<BBNodePtr>;
using BBDocumentPtr = std::shared_ptr<BBDocument>;

class BBNode : public std::enable_shared_from_this<BBNode>
{
public:
    enum NodeType
    {
        DOCUMENT,   
        ELEMENT,    // [b]bold[/b], [QUOTE], [QUOTE=Username;1234], [QUOTE user=Bob] 
        TEXT        // plain text
    };

    BBNode(NodeType nodeType, const std::string& name);
    virtual ~BBNode() = default;

    const std::string& getNodeName() const { return _name; }
    NodeType getNodeType() const { return _nodeType; }
    BBNodePtr getParent() const { return BBNodePtr(_parent); }

    const BBNodeList getChildren() const { return _children; }

    virtual void appendChild(BBNodePtr node)
    {
        _children.push_back(node);
        node->_parent = shared_from_this();
    }
  
protected:
    std::string     _name;
    NodeType        _nodeType;
    BBNodeWeakPtr   _parent;
    BBNodeList      _children;

    friend class BBText;
    friend class BBDocument;
    friend class BBElement;
};

class BBText : public BBNode
{
public:
    BBText(const std::string& value)
        : BBNode(BBNode::TEXT, value)
    {
        // nothing to do
    }

    virtual ~BBText() = default;

    virtual std::string getValue() const { return _name; }
};

class BBElement : public BBNode
{
public:
    enum ElementType
    {
        SIMPLE,     // [b]bold[/b], [code]print("hello")[/code]
        VALUE,      // [QUOTE=Username;12345]This is a quote[/QUOTE] (mostly used by vBulletin)
        PARAMETER,  // [QUOTE user=Bob userid=1234]This is a quote[/QUOTE]
    };

    BBElement(const std::string& name)
        : BBNode(BBNode::ELEMENT, name)
    {
        // nothing to do
    }

    virtual ~BBElement() = default;

private:
    ElementType     _elementType = BBElement::SIMPLE;    
}

class BBDocument : public BBNode
{
    BBDocument() 
        : BBNode(DOCUMENT, "#document")
    {
        // nothing to do        
    }

	template <typename citerator>
	citerator parseText(citerator begin, citerator end)
    {
        auto endingChar = begin;

        for (auto it = begin; it != end; it++)
        {
            if (*it == '[')
            {
                endingChar = it;
                break;
            }
        }

        if (endingChar == begin)
        {
            endingChar = end;
        }

        newText(std::string(begin, endingChar));
        return endingChar;
    }    

public: 
    static BBDocumentPtr create()
    {
        BBDocumentPtr doc = BBDocumentPtr(new BBDocument());
        return doc;
    }

    void load(const std::string& bbcode)
    {
        load(bbcode.begin(), bbcode.end());
    }

    template<class Iterator>
    void load(Iterator begin, Iterator end)
    {
        std::string buffer;
        auto bUnknownNodeType = true;
        auto current = begin;
        auto nodeType = BBNode::TEXT;

        while (current != end)
        {
            buffer.append(*current);
            if (bUnknownNodeType)
            {
                if (*current == '[')
                {
                }
                else
                {
                    nodeType = BBNode::TEXT;
                    bUnknownNodeType = false;
                }
            }
            else
            {
                switch (nodeType)
                {
                    case BBNode::TEXT:
                    {
                        current = parseText(current, end);
                    }
                    break;
                    case BBNode::ELEMENT:
                    {
                        current = parseElement(current, end);   
                    }
                    break;
                    default:
                        throw std::runtime_error("Unknown node type in BBDocument::load()");
                    break;
                }
            }
        }
    }

    BBText& newText(const std::string& text = std::string())
    {
        auto textNode = std::make_shared<BBText>(text);
        if (_stack.size() > 0)
        {
            _stack.top()->appendChild(textNode);
        }
        else
        {
            appendChild(textNode);
        }

        return *textNode;
    }

    BBElement& newElement(const std::string& name)
    {
        auto newNode = std::make_shared<BBElement>(name);
        if (_stack.size() > 0)
        {
            _stack.top()->appendChild(newNode);
        }
        else
        {
            appendChild(newNode);
        }

        return *newNode;
    }

private:
    BBNodeList      _stack2; 
    BBNodeStack     _stack;
};