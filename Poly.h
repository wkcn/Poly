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
	Poly Substitution(const Poly&);

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
	friend Poly& PolyDivide(Poly &a, const Poly &b, bool mod);
	friend Poly pow(const Poly&, BigInt u);

	//friend int PolyCmp(const Poly&, const Poly&);	//�Ƚ��ã����ظ���ʱa<b;	0ʱa=b;	����ʱa>b
	friend bool operator==(const Poly&, const Poly&);
	friend bool operator!=(const Poly&, const Poly&);
	friend bool operator<(const Poly&, const Poly&);
	friend bool operator<=(const Poly&, const Poly&);
	friend bool operator>(const Poly&, const Poly&);
	friend bool operator>=(const Poly&, const Poly&);

	bool isZero() const;//�ж��Ƿ�Ϊ0�����ڿ����ж�
	bool isNum() const;
	int GetInt(); 
	BigInt GetExp() const;
public:
	friend ostream& operator<<(ostream &,const Poly &);
	friend istream& operator>>(istream &, Poly &);
};

#endif