#pragma once
#include <cstring>
#include <cctype>

namespace bbcpp
{

inline bool IsDigit(char c)
{
    return ('0' <= c && c <= '9');
}

inline bool IsAlpha(char c)
{
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return (std::strchr(alpha, c) != nullptr);
}

inline bool IsSpace(char c)
{
    return std::isspace(static_cast<unsigned char>(c)) != 0;
}

}