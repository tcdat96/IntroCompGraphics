#include "utils.h"

char* readFile(const char* fileName) {
	FILE* fp = NULL;
	char* buf = NULL;
	errno_t err = fopen_s(&fp, fileName, "r");
	if (err != 0) {
		std::cout << "There was a problem opening " << fileName << "!" << std::endl;
	}
	else {
		if (fp == NULL) { return NULL; }
		fseek(fp, 0L, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		buf = new char[size + 1];
		fread(buf, 1, size, fp);
		buf[size] = '\0';
		fclose(fp);
	}
	return buf;
}


float sign(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

bool inTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(x, y, x1, y1, x2, y2);
	d2 = sign(x, y, x2, y2, x3, y3);
	d3 = sign(x, y, x3, y3, x1, y1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}