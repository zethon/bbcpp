#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#include <iostream>

class BBNode;
class BBDocument;

using BBNodePtr = std::shared_ptr<BBNode>;
using BBNodeWeakPtr = std::weak_ptr<BBNode>;
using BBNodeList = std::vector<BBNodePtr>;
using BBDocumentPtr = std::shared_ptr<BBDocument>;

class BBNode : public std::enable_shared_from_this<BBNode>
{
public:
    enum NodeType
    {
        DOCUMENT,   
        SIMPLE,     // [b]bold[/b], [code]print("hello")[/code]
        VALUE,      // [QUOTE=Username;12345]This is a quote[/QUOTE] (mostly used by vBulletin)
        PARAMETER,  // [QUOTE user=Bob userid=1234]This is a quote[/QUOTE] 
        TEXT        // plain text
    };

    BBNode(NodeType nodeType, const std::string& name);
    virtual ~BBNode() = default;

    const std::string& getNodeName() const { return _name; }
    NodeType getNodeType() const { return _nodeType; }
    BBNodePtr getParent() const { return BBNodePtr(_parent); }

    virtual void appendChild(BBNodePtr node)
    {
        throw std::logic_error("Cannot append child to type BBNode");
    }
  
protected:
    std::string     _name;
    NodeType        _nodeType;
    BBNodeWeakPtr   _parent;
    BBNodeList      _children;

    friend class BBText;
    friend class BBDocument;
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

        auto textNode = std::make_shared<BBText>(std::string(begin, endingChar));

        std::string name(begin, endingChar);
        std::cout << "name: [" << name << "]" << std::endl;
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
        auto bUnknownNodeType = true;
        auto current = begin;
        auto nodeType = BBNode::TEXT;

        while (current != end)
        {
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
                    default:
                        throw std::runtime_error("Unknown node type in BBDocument::load()");
                    break;
                }
            }
        }
    }

    virtual void appendChild(BBNodePtr node) override
    {
        _children.push_back(node);
        node->_parent = shared_from_this();
    }

private:
    BBNodeList  _stack;    
};