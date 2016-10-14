#include <iostream>
#include "../lib/BBDocument.h"

int main()
{
    const std::vector<std::string> tests =
    {
        "This is [b]an example[/b] of some text.",
        "This is [b]an ex[i]amp[/i]le[/b] of some text.",
        "This is [b]an example[/b] of some text."
    };

    for (const auto s: tests)
    {
        auto doc = BBDocument::create();
        doc->load(s);

        std::cout << "Original Text: " << s << std::endl;

        for (const auto node : doc->getChildren())
        {
            std::cout << "\tType  : [" << nodeTypeToString(node->getNodeType()) << "]" << std::endl;
            std::cout << "\tValue : " << node->getNodeName() << std::endl;
        }
    }
    
    return 0;
}
