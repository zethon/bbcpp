#include <iostream>
#include <sstream>
#include <fstream>

#include "cxxopts.hpp"
#include "../lib/BBSpirit.h"
#include "../lib/BBDocument.h"
#include "../lib/bbcpputils.h"

using namespace bbcpp;

std::string readFile(const std::string& filename)
{
    std::ifstream t(filename);

    if (t.good())
    {
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        return str;
    }

    return std::string();
}

void doSpiritParse(const std::string& text)
{
    std::cout << "spirit parse" << std::endl;
}

int main(int argc, char* argv[])
{
    cxxopts::Options options(argv[0], " - example command line options");
    options
        .positional_help("[optional args]")
        .show_positional_help();

    options.add_options()
        ("f, file", "File", cxxopts::value<std::string>())
        ("s", "Spirit Parsing")
    ;

    std::string inputfile;
    bool spiritParse = false;

    try
    {
        cxxopts::ParseResult result = options.parse(argc, argv);
        if (result.count("file") > 0)
        {
            inputfile = result["file"].as<std::string>();
        }

        spiritParse = result["s"].as<bool>();
    }
    catch (const cxxopts::OptionException& e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
    }

    if (inputfile.size() > 0 && std::ifstream(inputfile).good())
    {
        std::string filetext = readFile(inputfile);

        std::cout << "file: " << inputfile << std::endl;
        std::cout << "bbcode: " << filetext << std::endl;

        if (spiritParse)
        {
            doSpiritParse(filetext);
        }
        else
        {
            auto doc = BBDocument::create();
            doc->load(filetext);
            printDocument(*doc);
        }
    }
    else
    {
        std::cout << "invalid file: " << inputfile << std::endl;
    }

    return 0;
}
