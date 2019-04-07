#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <type_traits>

#define abortMessage(msg) __AbortMessage(msg,__FILE__,__LINE__)
namespace aux
{
    int __AbortMessage(std::string error, const char* file, int line);
}