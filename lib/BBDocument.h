#pragma once
#include <memory>
#include <string>
#include <vector>

class BBNode;
class BBNodeDocument;

using BBNodePtr = std::shared_ptr<BBNode>;
using BBNodeWeakPtr = std::weak_ptr<BBNode>;
using BBNodeList = std::vector<BBNode>;
using BBNodeDocumentPtr = std::shared_ptr<BBNodeDocument>;

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
  
private:
    std::string     _name;
    NodeType        _nodeType;
    BBNodeWeakPtr   _parent;
};

class BBNodeDocument : public BBNode
{
    BBNodeDocument() 
        : BBNode(DOCUMENT, "#document")
    {
        // nothing to do        
    }

public: 
    static BBNodeDocumentPtr create()
    {
        BBNodeDocumentPtr doc = BBNodeDocumentPtr(new BBNodeDocument());
        return doc;
    }

    template<class Iterator>
    void load(Iterator begin, Iterator end)
    {
        auto current = begin;

        while (current != end)
        {
            current++;
        }
    }
};