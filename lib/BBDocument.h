#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

#include <iostream>

class BBNode;
class BBText;
class BBElement;
class BBDocument;

using BBNodePtr = std::shared_ptr<BBNode>;
using BBTextPtr = std::shared_ptr<BBText>;
using BBElementPtr = std::shared_ptr<BBElement>;

using BBNodeWeakPtr = std::weak_ptr<BBNode>;
using BBNodeList = std::vector<BBNodePtr>;
using BBNodeStack = std::stack<BBNodePtr>;
using BBDocumentPtr = std::shared_ptr<BBDocument>;

class BBNode : public std::enable_shared_from_this<BBNode>
{
    template<typename NewTypePtrT>
    NewTypePtrT cast(BBNodePtr node, bool bThrowOnFail)
    {
        if (node == nullptr && !bThrowOnFail)
        {
            return NewTypePtrT();
        }
        else if (node == nullptr)
        {
            throw std::invalid_argument("Cannot downcast BBNode, object is null");
        }

        NewTypePtrT newobj = std::dynamic_pointer_cast<typename NewTypePtrT::element_type, BBNode>(node);

        if (newobj == nullptr && bThrowOnFail)
        {
            throw std::invalid_argument("Cannot downcast, object is not correct type");
        }

        return newobj;
    }   

    template<typename NewTypePtrT>
    NewTypePtrT cast(BBNodePtr node, bool bThrowOnFail) const
    {
        if (node == nullptr && !bThrowOnFail)
        {
            return NewTypePtrT();
        }
        else if (node == nullptr)
        {
            throw std::invalid_argument("Cannot downcast, BBNode object is null");
        }

        NewTypePtrT newobj = std::dynamic_pointer_cast<typename NewTypePtrT::element_type, BBNode>(node);

        if (newobj == nullptr && bThrowOnFail)
        {
            throw std::invalid_argument("Cannot downcast, object is not correct type");
        }

        return newobj;
    }      
        
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

    template<typename NewTypePtrT>
	NewTypePtrT downCast(bool bThrowOnFail = true)
	{
		return cast<NewTypePtrT>(shared_from_this(), bThrowOnFail);
	}

    template<typename NewTypePtrT>
	NewTypePtrT downCast(bool bThrowOnFail = true) const
	{
		return cast<NewTypePtrT>(shared_from_this(), bThrowOnFail);
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

    virtual std::string getText() const { return _name; }
};

class BBElement : public BBNode
{
public:
    enum ElementType
    {
        SIMPLE,     // [b]bold[/b], [code]print("hello")[/code]
        VALUE,      // [QUOTE=Username;12345]This is a quote[/QUOTE] (mostly used by vBulletin)
        PARAMETER,  // [QUOTE user=Bob userid=1234]This is a quote[/QUOTE]
        CLOSING     // [/b], [/code]
    };

    BBElement(const std::string& name, ElementType et = BBElement::SIMPLE)
        : BBNode(BBNode::ELEMENT, name),
          _elementType(et)
    {
        // nothing to do
    }

    virtual ~BBElement() = default;

    const ElementType getElementType() const { return _elementType; }

private:
    ElementType     _elementType = BBElement::SIMPLE;    
};

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

    // template <typename citerator>
    // citerator parseValue(citerator begin, citerator end)
    // {

    // }

    template <typename citerator>
    citerator parseElement(citerator begin, citerator end)
    {
        bool closingTag = false;
        auto nameStart = std::next(begin);
        auto nameEnd = end;

        if (*nameStart == '/')
        {
            closingTag = true;
            nameStart = std::next(nameStart);
        }

        for (auto it = nameStart; it != end; it++)
        {
            if (*it == ']' || it == end)
            {
                nameEnd = it;
                break;
            }
            else if (*it == '=')
            {
                // it = parseValue(it, end);
            }
        }

        // looks like we have a valid ELEMENT
        if (nameEnd != end 
            && nameStart != nameEnd 
            && *nameEnd == ']'
            && nameEnd != std::next(begin))
        {
            if (closingTag)
            {
                newClosingElement(std::string(nameStart, nameEnd));
            }
            else
            {
                newElement(std::string(nameStart, nameEnd));
            }

            return std::next(nameEnd);
        }

        return begin;
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

        Iterator temp;

        while (current != end)
        {
            if (bUnknownNodeType)
            {
                if (*current == '[')
                {
                    nodeType = BBNode::ELEMENT;
                    bUnknownNodeType = false;
                }
                else
                {
                    nodeType = BBNode::TEXT;
                    bUnknownNodeType = false;
                }
            }
            
            if (!bUnknownNodeType)
            {
                switch (nodeType)
                {
                    default:
                        throw std::runtime_error("Unknown node type in BBDocument::load()");
                    break;

                    case BBNode::TEXT:
                    {
                        current = parseText(current, end);
                        bUnknownNodeType = true;
                    }
                    break;

                    case BBNode::ELEMENT:
                    {
                        temp  = parseElement(current, end);
                        if (temp == current)
                        {
                            // nothing was parsed, treat as text
                            nodeType = BBNode::TEXT;
                            bUnknownNodeType = false;
                        }
                        else
                        {
                            current = temp;
                            bUnknownNodeType = true;
                        }
                    }
                    break;                    
                }
            }
        }
    }

private:
    BBNodeStack     _stack;

    BBText& newText(const std::string& text = std::string());
    BBElement& newElement(const std::string& name);
    BBElement& newClosingElement(const std::string& name);
};

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
