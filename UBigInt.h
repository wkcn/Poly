#ifndef UBIGINT_H
#define UBIGINT_H

#include "Vector.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

/*
	无符号大整数类
	这里使用10000进制
	有个问题，不可以使用UBigInt a = 0,原因是编译器判断不出0属于unsigned int还是const char * 
*/

class UBigInt{
protected:
	Vector<int> datas;	//以10000进制记录大数
	int _len;	//储存大数所用datas的元素数,是否可以用datas.size()代替这一问题值得考虑
public:
	//构造
	UBigInt();
	UBigInt(unsigned int);
	//UBigInt(const UBigInt&);	//因为内部变量已实现深拷贝
	UBigInt(const char *);
	//UBigInt(const string&);	//不想依赖太多其它类

	//运算符(部分使用友元，更适合强制转换）
	//一元运算
	
	UBigInt& operator++();//++a
	UBigInt operator++(int dummy);//a++
	UBigInt& operator--();//--a
	UBigInt operator--(int dummy);//a--

	friend UBigInt& operator+=(UBigInt&,const UBigInt&);
	friend UBigInt& operator-=(UBigInt&, const UBigInt&);
	friend UBigInt& operator*=(UBigInt&, const UBigInt&);
	friend UBigInt& operator/=(UBigInt&, const UBigInt&);
	friend UBigInt& operator%=(UBigInt&, const UBigInt&);

	friend UBigInt& UBigIntDivide(UBigInt &a, const UBigInt &b, bool mod);//除法和取模用

	//二元运算
	friend UBigInt operator+(const UBigInt&, const UBigInt &);
	friend UBigInt operator-(const UBigInt&, const UBigInt &);
	friend UBigInt operator*(const UBigInt&, const UBigInt &);
	friend UBigInt operator/(const UBigInt&, const UBigInt &);
	friend UBigInt operator%(const UBigInt&, const UBigInt &);

	//比较,使用友元能实现int < UBigInt
	friend int UBigIntCmp(const UBigInt&,const UBigInt&);	//比较用，返回负数时a<b;	0时a=b;	正数时a>b
	friend bool operator==(const UBigInt&,const UBigInt&);
	friend bool operator!=(const UBigInt&,const UBigInt&);
	friend bool operator<(const UBigInt&, const UBigInt&);
	friend bool operator<=(const UBigInt&, const UBigInt&);
	friend bool operator>(const UBigInt&, const UBigInt&);
	friend bool operator>=(const UBigInt&, const UBigInt&);
	bool isZero();//判断是否为0，用于快速判断
	int GetInt();	//取模1 0000 0000
public:
	friend ostream& operator<<(ostream &,UBigInt &);
	friend ostream& operator<<(ostream &,UBigInt &&);
	friend istream& operator>>(istream &, UBigInt &);
};

#endif
