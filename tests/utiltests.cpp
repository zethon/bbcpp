#include <sstream>

#include "gtest/gtest.h"

#include "../lib/BBDocument.h"
#include "../lib/bbcpputils.h"

using namespace bbcpp;

TEST(utiltests, paramstream)
{
    ParameterMap map1;
    map1.insert(std::make_pair("username", "bob"));

    std::stringstream s1;
    s1 << map1;
    EXPECT_EQ(s1.str(), R"({ {username=bob} })");

    map1.insert(std::make_pair("id", "1234"));
    std::stringstream s2;
    s2 << map1;
    EXPECT_EQ(s2.str(), R"({ {id=1234}, {username=bob} })");

    map1.insert(std::make_pair("date", ""));
    std::stringstream s3;
    s3 << map1;
    EXPECT_EQ(s3.str(), R"({ {date=}, {id=1234}, {username=bob} })");
}
