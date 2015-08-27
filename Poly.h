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
	friend Poly pow(const Poly&, int u);//����Ч�ʣ�ʹ��int

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
public:
	friend ostream& operator<<(ostream &, Poly &);
	friend istream& operator>>(istream &, Poly &);
};

#endif