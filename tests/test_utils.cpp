#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../lib/bbcpputils.h"

BOOST_AUTO_TEST_SUITE(Utils)

BOOST_AUTO_TEST_CASE(nodeTypeStringTest)
{
    using namespace bbcpp;

    BOOST_CHECK(nodeTypeToString(BBNode::NodeType::DOCUMENT) == "Document");
    BOOST_CHECK(nodeTypeToString(BBNode::NodeType::ELEMENT) == "Element");
    BOOST_CHECK(nodeTypeToString(BBNode::NodeType::TEXT) == "Text");
}

BOOST_AUTO_TEST_SUITE_END()
