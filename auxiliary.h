#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <type_traits>

#define abortMessage(msg) __AbortMessage(msg,__FILE__,__LINE__)
namespace aux
{
    int __AbortMessage(std::string error, const char* file, int line);
}
namespace math
{
    template<class T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix
    {
    public:
        Matrix(int size_x,int size_y);
        Matrix(const Matrix& mat);
        T& operator=(const Matrix& mat);
        T& operator()(int i, int j);
        ~Matrix();
    private:
        T* data;
        size_t cols,rows;
    };
	
}
