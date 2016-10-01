#include <iostream>
#include "../lib/BBDocument.h"

int main()
{
    auto doc = BBDocument::create();
    doc->load("This is [b]an example[/b] of some text.");

    for (const auto node : doc->getChildren())
    {
        std::cout << "name: (" << node->getNodeName() << ")" << std::endl;
    }
    
    return 0;
}