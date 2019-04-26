#define _CRT_SECURE_NO_WARNINGS
#include "stringEater.h"
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

stringEater::stringEater(const char* target, char(*getter)())
{
	len = strlen(target);
	this->target = new char[len + 1];
	strcpy(this->target, target);
	this->getter = getter;
	lis.clear();
}

stringEater::~stringEater()
{
	if(target)
		delete target;
}

bool stringEater::jump()
{
	if (len == 0) return true;
	lis.clear();
	while (1) {
		while (lis.size() < len) {
			char c = getter();
			if (c == 0) return false;
			lis.push_back(c);
		}

		if (cmp()) {
			lis.clear();
			return true;
		}

		if (!lis.empty()) lis.pop_front();
		while (!lis.empty() && (*lis.begin() != target[0]))
			lis.pop_front();
	}
}

void stringEater::clearCache()
{
	lis.clear();
}

bool stringEater::cmp() {
	if (lis.size() != len) return false;
	auto li = lis.begin();
	for (int i = 0; li != lis.end(); i++, li++) {
		if (target[i] != *li) return false;
	}
	return true;
}

