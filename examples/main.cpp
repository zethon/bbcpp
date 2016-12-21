#include <iostream>
#include <sstream>
#include "../lib/BBDocument.h"

std::string getIndent(const uint indent)
{
    std::stringstream output;

    for (auto i = 0; i < indent; i++)
    {
        output << "│   ";
    }

    output << "|-- ";
    return output.str();
}

void printChildren(const BBNode& parent, uint indent)
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
                    << getIndent(indent) 
                    << "[" 
                    << (element->getElementType() == BBElement::CLOSING ? "/" : "")
                    << element->getNodeName() << "]"
                    << std::endl;
            }
            break;

             case BBNode::TEXT:
             {
                const auto textnode = node->downCast<BBTextPtr>();
                std::cout << getIndent(indent) 
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

int main()
{
    const std::vector<std::string> tests =
    {
        // "This is [b]an example[/b] of some text  [",
        // "This is [b]an[/b] example of some text",
        // "This is [b]an example[/b] of some text.",
        //"This is [b]an ex[i]amp[/i]le[/b] of some text.",
        "This is [b]an example[/b] of some text.[]"
    };

    for (const auto s: tests)
    {
        auto doc = BBDocument::create();
        doc->load(s);

        printDocument(*doc);
    }
    
    return 0;
}
