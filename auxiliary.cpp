#include "auxiliary.h"
int aux::__AbortMessage(std::string error, const char* file, int line)
{
	std::cerr << "Error: " << error << " \nFile " << file << ", line " << line << std::endl;
	abort();
    return -1;
}