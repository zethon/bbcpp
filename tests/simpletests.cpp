#include "gtest/gtest.h"
#include "../lib/BBDocument.h"

TEST(simpletests, test1)
{
    const std::string string1 = "This is a [b]simple[/b] test";

    auto doc = BBDocument::create();
    doc->load(string1);

    EXPECT_EQ(doc->getChildren().size(), 4);
}
