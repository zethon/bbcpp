#include "gtest/gtest.h"
#include "../lib/BBDocument.h"

BBDocumentPtr createDoc(const std::string& text)
{
    auto doc = BBDocument::create();
    doc->load(text);
    return doc;
}

std::string getIndent(const uint indent)
{
    std::stringstream output;

    for (auto i = 0; i < indent; i++)
    {
        output << "|   ";
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

TEST(simpletests, noElement)
{
    const std::string string1 = "Hello world!";

    auto doc = BBDocument::create();
    doc->load(string1);

    EXPECT_EQ(doc->getChildren().size(), 1);

    const BBTextPtr text = doc->getChildren().at(0)->downCast<BBTextPtr>(false);
    EXPECT_NE(text, nullptr);
    EXPECT_EQ(text->getText(), string1);
}

TEST(simpletests, oneElement)
{
    const std::string string1 = "This is a [b]simple[/b] test";

    auto doc = BBDocument::create();
    doc->load(string1);

    const auto children = doc->getChildren();
    EXPECT_EQ(children.size(), 3);

    const auto secnode = children.at(1);
    EXPECT_EQ(secnode->getChildren().size(), 2);

    BBTextPtr textnode = children.at(0)->downCast<BBTextPtr>(false);
    EXPECT_NE(textnode, nullptr);
    EXPECT_EQ(textnode->getText(), "This is a ");

    textnode = children.at(2)->downCast<BBTextPtr>(false);
    EXPECT_EQ(textnode->getText(), " test");

    textnode = children.at(1)->downCast<BBTextPtr>(false);
    EXPECT_EQ(textnode, nullptr);
}

TEST(simpletests, brokenElement)
{
    const std::string string1 = "This is [b]broken bbcode";

    auto doc = BBDocument::create();
    doc->load(string1);

    const auto children = doc->getChildren();
    EXPECT_EQ(children.size(), 2);
}

TEST(simpletests, emptyElement)
{
    const auto doc = createDoc("This is [ ] xx [b] ok! [] ok?");
    printDocument(*doc);
}
