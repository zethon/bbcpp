#include "gtest/gtest.h"

#include "../lib/BBDocument.h"
#include "../lib/bbcpputils.h"

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

TEST(simpletests, emptyElement)
{
   const auto doc = createDoc("This is [ ] xx [b] ok! [] ok?");
   BBNodeList children = doc->getChildren();
   EXPECT_EQ(children.size(), 2);
   EXPECT_EQ(children.at(1)->getNodeName(), "b");
   EXPECT_EQ(children.at(1)->getChildren().size(), 1);
}

TEST(simpletests, brokenElements)
{
   std::string string1 = "This is [b]broken bbcode";

   BBDocumentPtr doc = BBDocument::create();
   doc->load(string1);

   BBNodeList children = doc->getChildren();
   EXPECT_EQ(children.size(), 2);

   doc = createDoc("This is[text[[[[[[");
   children = doc->getChildren();
   EXPECT_EQ(children.size(), 1);
   BBTextPtr textnode = children.at(0)->downCast<BBTextPtr>(false);
   EXPECT_TRUE(textnode);
   EXPECT_EQ(textnode->getText(), "This is[text[[[[[[");
   EXPECT_FALSE(children.at(0)->downCast<BBElementPtr>(false));

   doc  = createDoc("This is[b[b]");
   children = doc->getChildren();
   EXPECT_EQ(children.size(), 2);
   textnode = children.at(0)->downCast<BBTextPtr>(false);
   EXPECT_TRUE(textnode);
   EXPECT_EQ(textnode->getText(), "This is[b");
   EXPECT_EQ(children.at(1)->getNodeName(), "b");

   doc  = createDoc("This [] is[bbb");
   children = doc->getChildren();
   EXPECT_EQ(children.size(), 1);
   textnode = children.at(0)->downCast<BBTextPtr>(false);
   EXPECT_TRUE(textnode);
   EXPECT_EQ(textnode->getText(), "This [] is[bbb");
}

TEST(simpletests, singleparameter)
{
    const std::vector<std::string> strings =
    {
        "This is [style color=red]WARNING[/style]",
//        "This is [style color =red]red[/color]",
//        "This is [style color= red]red[/color]",
//        "This is [style color = red]red[/color]",
//        "This is [style color=red ]red[/color]"
    };

    for (auto& text : strings)
    {
        SCOPED_TRACE("Test Text: " + text);
        BBDocumentPtr doc  = createDoc(text);
        BBNodeList children = doc->getChildren();
        EXPECT_EQ(children.size(), 2);

        BBElementPtr el = children.at(1)->downCast<BBElementPtr>(false);
        EXPECT_NE(el, nullptr);

        const ParameterMap& pairs = el->getParameters();
        EXPECT_EQ(pairs.size(), 1);
        EXPECT_NE(pairs.find("color"), pairs.end());
        EXPECT_EQ(pairs.at("color"), "red");
    }
}

TEST(simpletests, nestedElements)
{
    const std::string test1 = "This is [b][i][u]OK![/u][/i][/b]!!";
    auto doc  = createDoc(test1);

    BBNodeList children = doc->getChildren();
    EXPECT_EQ(children.size(), 3);

    BBNodePtr bNode = children.at(1);
    EXPECT_EQ(bNode->getChildren().size(),2);
    EXPECT_EQ(bNode->getNodeName(), "b");

    BBNodePtr iNode = bNode->getChildren().at(0);
    EXPECT_EQ(iNode->getChildren().size(), 2);
    EXPECT_EQ(iNode->getNodeName(), "i");

    BBNodePtr uNode = iNode->getChildren().at(0);
    EXPECT_EQ(uNode->getChildren().size(), 2);
    EXPECT_EQ(uNode->getNodeName(), "u");

    BBNodePtr tNode = uNode->getChildren().at(0);
    EXPECT_EQ(tNode->getNodeType(), BBNode::NodeType::TEXT);

    BBTextPtr textNode = tNode->downCast<BBTextPtr>(false);
    ASSERT_NE(textNode, nullptr);
    EXPECT_EQ(textNode->getText(), "OK!");
}


