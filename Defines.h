#ifndef SLANG_DEFINES_H
#define SLANG_DEFINES_H
#define _CRT_SECURE_NO_WARNINGS
#include "BigInt.h"
#include "Poly.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <utility>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cstdio>
using namespace std;

struct SExp{
	enum SEXP_TYPE{
		VAR, FUNC, COMMAND, IF, WHILE,STR
	};
	SEXP_TYPE type;
	string name;
	SExp *parent;
	int id;
	Vector<SExp*> elems;	//不知道为什么，自己写的Vector不能查看详细元素赋值
	SExp();
	void Copy(SExp *);
};

void DelSExp(SExp *);

#endif
