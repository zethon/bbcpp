#include <iostream>
#include <sstream>
#include "../lib/BBDocument.h"

using namespace bbcpp;

int main()
{
    const std::vector<std::string> tests =
    {
        "This is[text[[[[[[",
    };

    for (const auto s: tests)
    {
        auto doc = BBDocument::create();
        doc->load(s);

        printDocument(*doc);
    }
    
    return 0;
}
