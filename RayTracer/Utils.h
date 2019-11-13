#pragma once

#include <string>

using namespace std;

bool startsWith(string src, string prefix) {
	return strncmp(src.c_str(), prefix.c_str(), prefix.size()) == 0;
}