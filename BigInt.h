#ifndef BIGINT_H
#define BIGINT_H
#include "UBigInt.h"
//不知道是否能用继承UBigInt的方式实现？
class BigInt{
protected:
	UBigInt num;
	bool minus;	//负号
public:
	//构造
	BigInt();
	BigInt(int);
	BigInt(const char *);

	operator string();
	//operator int();重大Bug！！！

	//运算符(部分使用友元，更适合强制转换）
	//一元运算
	BigInt operator-();

	BigInt& operator++();//++a
	BigInt operator++(int dummy);//a++
	BigInt& operator--();//--a
	BigInt operator--(int dummy);//a--


	friend BigInt& operator+=(BigInt&, const BigInt&);
	friend BigInt& operator-=(BigInt&, const BigInt&);
	friend BigInt& operator*=(BigInt&, const BigInt&);
	friend BigInt& operator/=(BigInt&, const BigInt&);
	friend BigInt& operator%=(BigInt&, const BigInt&);

	friend BigInt& BigIntDivide(BigInt &a, const BigInt &b, bool mod);//除法和取模用

	//二元运算
	friend BigInt operator+(const BigInt&, const BigInt &);
	friend BigInt operator-(const BigInt&, const BigInt &);
	friend BigInt operator*(const BigInt&, const BigInt &);
	friend BigInt operator/(const BigInt&, const BigInt &);
	friend BigInt operator%(const BigInt&, const BigInt &);

	//比较,使用友元能实现int < UBigInt
	friend int BigIntCmp(const BigInt&, const BigInt&);	//比较用，返回负数时a<b;	0时a=b;	正数时a>b
	friend bool operator==(const BigInt&, const BigInt&);
	friend bool operator!=(const BigInt&, const BigInt&);
	friend bool operator<(const BigInt&, const BigInt&);
	friend bool operator<=(const BigInt&, const BigInt&);
	friend bool operator>(const BigInt&, const BigInt&);
	friend bool operator>=(const BigInt&, const BigInt&);
	bool isZero();//判断是否为0，用于快速判断
	bool isMinus();
	int GetInt();	//获取int值,会取模1 0000 0000
public:
	friend ostream& operator<<(ostream &, BigInt &&);
	friend ostream& operator<<(ostream &, BigInt &);
	friend istream& operator>>(istream &, BigInt &);
};

#endif
