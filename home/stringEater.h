#include <list>
using namespace std;
#pragma once
class stringEater
{
public:
	stringEater(const char* target, char(*getter)());
	~stringEater();
	bool jump();
	void clearCache();
private :
	char(*getter)();
	char* target;
	list<char> lis;
	int len;
	bool cmp();
};

