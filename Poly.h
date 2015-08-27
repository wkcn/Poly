#ifndef _POLY_H
#define _POLY_H

#include "BigInt.h"

typedef pair<BigInt, BigInt> term;

class Poly {
private:
	Vector<term> poly;
public:
	Poly(BigInt co,BigInt exp);
	Poly(int num);
	Poly(const char *);
	Poly();

	Poly Derivative();

	friend Poly& operator+=(Poly&, const Poly&);
	friend Poly& operator-=(Poly&, const Poly&);
	friend Poly& operator*=(Poly&, const Poly&);
	friend Poly& operator/=(Poly&, const Poly&);
	friend Poly& operator%=(Poly&, const Poly&);

	friend Poly operator+(const Poly&, const Poly &);
	friend Poly operator-(const Poly&, const Poly &);
	friend Poly operator*(const Poly&, const Poly &);
	friend Poly operator/(const Poly&, const Poly &);
	friend Poly operator%(const Poly&, const Poly &);
	friend Poly pow(const Poly&, int u);//出于效率，使用int

	//friend int PolyCmp(const Poly&, const Poly&);	//比较用，返回负数时a<b;	0时a=b;	正数时a>b
	friend bool operator==(const Poly&, const Poly&);
	friend bool operator!=(const Poly&, const Poly&);
	friend bool operator<(const Poly&, const Poly&);
	friend bool operator<=(const Poly&, const Poly&);
	friend bool operator>(const Poly&, const Poly&);
	friend bool operator>=(const Poly&, const Poly&);

	bool isZero() const;//判断是否为0，用于快速判断
	bool isNum() const;
	int GetInt(); 
public:
	friend ostream& operator<<(ostream &, Poly &);
	friend istream& operator>>(istream &, Poly &);
};

#endif