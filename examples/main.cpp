#include <iostream>
#include "../lib/BBDocument.h"

int main()
{
    auto doc = BBDocument::create();
    doc->load("This is [b]an example[/b]!!");
    
    std::cout << "Hello world!" << std::endl;
    std::cout << ">" << doc->getNodeName() << std::endl;
    
    return 0;
}