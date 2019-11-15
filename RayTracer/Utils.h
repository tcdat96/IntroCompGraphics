#pragma once

#include <string>

#ifdef DEBUG
#  define DEBUG_PRINT(x) cerr << x
#else
#  define DEBUG_PRINT(x)
#endif

using namespace std;

bool startsWith(string src, string prefix) {
	return strncmp(src.c_str(), prefix.c_str(), prefix.size()) == 0;
}