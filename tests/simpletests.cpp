#include "gtest/gtest.h"
#include "../lib/BBDocument.h"

using namespace bbcpp;

BBDocumentPtr createDoc(const std::string& text)
{
    auto doc = BBDocument::create();
    doc->load(text);
    return doc;
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

TEST(simpletests, brokenElements)
{
    std::string string1 = "This is [b]broken bbcode";

    auto doc = BBDocument::create();
    doc->load(string1);

    const auto children = doc->getChildren();
    EXPECT_EQ(children.size(), 2);

    const auto doc2 = createDoc("This is[text[[[[[[");
    printDocument(*doc2);
}

TEST(simpletests, emptyElement)
{
    const auto doc = createDoc("This is [ ] xx [b] ok! [] ok?");
    printDocument(*doc);
}
