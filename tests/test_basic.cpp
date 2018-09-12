#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../lib/BBDocument.h"

BOOST_AUTO_TEST_SUITE(basic)

BOOST_AUTO_TEST_CASE(constructionTest)
{
    using namespace bbcpp;

    BBNode node { BBNode::NodeType::ELEMENT, "QUOTE" };
    BOOST_CHECK(node.getNodeName() == "QUOTE");
}

BOOST_AUTO_TEST_SUITE_END()
