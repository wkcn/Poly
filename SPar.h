#ifndef SPAR_H
#define SPAR_H

#include "Defines.h"
//#include "SVM.h"

class SVM;

//参数类，负责C++与Slang的交互^-^
class SPar{
public:
	SVM *vm;
	SExp *exp;	//当前表达式
public:
	SPar(SVM *, SExp *);
	int size();
	Poly& operator[](int i);	//返回对应BigInt引用(必须是实例化),第一个参数下标为0，引用~（不能做下标0代表参数大小》）
	string GetStr(int i);
	Poly GetNum(int i);
	SExp::SEXP_TYPE GetType(int i);
};


#endif
