#include "auxiliary.h"

#include <string>

int aux::__AbortMessage(std::string error, const char* file, int line)
{
	std::cerr << "DiagCC::error: " << error << " \nFile " << file << ", line " << line << std::endl;
	abort();
	return -1;
};

namespace math
{
	template<class T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
	class Matrix
	{
	public:
		Matrix(int size_x, int size_y);
		Matrix(const Matrix& mat);
		T& operator=(const Matrix& mat);
		T& operator()(int i, int j);
		~Matrix();
	private:
		T* data;
		size_t cols, rows;
	};

}