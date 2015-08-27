#include "Poly.h"

Poly::Poly(){}
Poly::Poly(BigInt co,BigInt exp){
	poly.push_back(make_pair(co, exp));
}
Poly::Poly(int num){
	if (num != 0)poly.push_back(make_pair(num, 0));
}
Poly::Poly(const char *s){
	BigInt b = BigInt(s);
	if (!b.isZero())poly.push_back(make_pair(b, 0));
}
Poly& operator+=(Poly&a, const Poly&b) {
	Vector<term> newPoly;
	int i = 0, j = 0;
	while (i < a.poly.size() || j < b.poly.size()) {
		while (i<a.poly.size() && (j >= b.poly.size() || a.poly[i].second > b.poly[j].second)) {
			newPoly.push_back(a.poly[i++]);
		}
		while (j<b.poly.size() && (i >= a.poly.size() || a.poly[i].second < b.poly[j].second)) {
			newPoly.push_back(b.poly[j++]);
		}
		while (i<a.poly.size() && j < b.poly.size() && a.poly[i].second == b.poly[j].second) {
			BigInt u = a.poly[i].first + b.poly[j].first;
			if(!u.isZero())newPoly.push_back(make_pair(u,a.poly[i].second));
			++i;
			++j;
		}
	}
	a.poly = newPoly;
	return a;
}

Poly& operator-=(Poly&a, const Poly&b) {
	Vector<term> newPoly;
	int i = 0, j = 0;
	while (i < a.poly.size() || j < b.poly.size()) {
		while (i<a.poly.size() && (j >= b.poly.size() || a.poly[i].second > b.poly[j].second)) {
			newPoly.push_back(make_pair(a.poly[i].first,a.poly[i].second));
			++i;
		}
		while (j<b.poly.size() && (i >= a.poly.size() || a.poly[i].second < b.poly[j].second)) {
			newPoly.push_back(make_pair(-b.poly[i].first,b.poly[i].second));
			++j;
		}
		while (i<a.poly.size() && j < b.poly.size() && a.poly[i].second == b.poly[j].second) {
			BigInt u = a.poly[i].first - b.poly[j].first;
			if (!u.isZero())newPoly.push_back(make_pair(u, a.poly[i].second));
			++i;
			++j;
		}
	}
	a.poly = newPoly;
	return a;
}

Poly& operator*=(Poly&a, const Poly&b) {
	Poly newPoly;
	for (int i = 0;i < a.poly.size();++i) {
		for (int j = 0;j < b.poly.size();++j) {
			newPoly += Poly(a.poly[i].first*b.poly[j].first, a.poly[i].second+b.poly[j].second);
		}
	}
	a = newPoly;
	return a;
}

Poly& operator/=(Poly&a, const Poly&b) {
	/*
	if (!a.isNum() || !b.isNum())throw "无法进行除法计算";
	a.poly[0].first /= b.poly[0].first;
	*/

	if (b.poly.empty()) throw "不能除以0";
	Vector<term> newPoly;
	int offset = 0;
	while (a.GetExp() >= b.GetExp() && !a.poly.empty() && offset < a.poly.size()) {
		//下标0为最高次
		term &at = a.poly[offset];
		term &bt = b.poly[0];
		BigInt co = at.first / bt.first;
		if (co.isZero()) {
			++offset;
			continue;
		}
		BigInt ex = at.second - bt.second;
		Poly s(co, ex);
		newPoly.push_back(make_pair(co, ex));
		a -= s*b;
	}

	a.poly = newPoly;

	return a;
}

Poly& operator%=(Poly&a, const Poly&b) {
	if (!a.isNum() || !b.isNum())throw "无法进行取模计算";
	a.poly[0].first %= b.poly[0].first;
	return a;
}

Poly operator+(const Poly &a, const Poly &b){
	Poly temp = a;
	temp += b;
	return temp;
}

Poly operator-(const Poly &a, const Poly &b){
	Poly temp = a;
	temp -= b;
	return temp;
}

Poly operator*(const Poly &a, const Poly &b){
	Poly temp = a;
	temp *= b;
	return temp;
}

Poly operator/(const Poly &a, const Poly &b){
	Poly temp = a;
	temp /= b;
	return temp;
}

Poly operator%(const Poly &a, const Poly &b){
	Poly temp = a;
	temp %= b;
	return temp;
}

Poly pow(const Poly &a,BigInt u){
	Poly p = a;
	Poly result = 1;
	/*
	for (; u > 0; u >>= 1){
		if (u & 1){
			result *= p;
		}
		p *= p;
	}
	*/
	for (;!u.isZero();u /= 2) {
		if (!(u % BigInt(2)).isZero()) {
			result *= p;
		}
		p *= p;
	}
	return result;
}

Poly Poly::Derivative() {
	Poly newPoly;
	for (int i = 0;i < poly.size();++i) {
		if (!poly[i].second.isZero()) {
			newPoly.poly.push_back(make_pair(poly[i].first*poly[i].second, poly[i].second - BigInt(1)));
		}
	}
	return newPoly;
}

Poly Poly::Substitution(const Poly&p) {
	Poly newPoly;
	for (int i = 0;i < poly.size();++i) {
		term &t = poly[i];
		//cout << Poly(p) << "---" << t.second << endl;
		newPoly += pow(p, t.second) * Poly(t.first);
	}
	return newPoly;
}

ostream& operator<<(ostream &os,const Poly &u){

	if (u.poly.size() == 0)os << 0;
	else{
		bool first = true;
		for (int i = 0; i < u.poly.size(); ++i){
			term &t = u.poly[i];

			bool minus = t.first.isMinus();
			bool secondZero = t.second.isZero();
			if (!minus && !first)os << '+';


			if (t.first != BigInt(1)){
				if (t.first == BigInt(-1))os << '-';
				else{
					os << t.first;
				}
			}
			else {
				if (secondZero) {
					os << t.first;
				}
			}

			if (!secondZero){
				os << 'x';
				if (t.second != BigInt(1)){
					os << '^';
					os << t.second;
				}
			}

			first = false;
		}
	}
	return os;
}



bool operator==(const Poly&a, const Poly&b){
	if (a.poly.size() != b.poly.size())return false;
	for (size_t i = 0; i < a.poly.size(); ++i){
		if (a.poly[i] != b.poly[i]) return false;
	}
	return true;
}

bool operator!=(const Poly&a, const Poly&b){
	return !(a == b);
}

bool operator<(const Poly&a, const Poly&b){
	if (!a.isNum() || !b.isNum())throw "无法比较";
	return a.poly[0].first < b.poly[0].first;
}

bool operator<=(const Poly&a, const Poly&b){
	if (!a.isNum() || !b.isNum())throw "无法比较";
	return a.poly[0].first <= b.poly[0].first;
}

bool operator>(const Poly&a, const Poly&b){
	if (!a.isNum() || !b.isNum())throw "无法比较";
	return a.poly[0].first > b.poly[0].first;
}

bool operator>=(const Poly&a, const Poly&b){
	if(!a.isNum()||!b.isNum())throw "无法比较";
	return a.poly[0].first >= b.poly[0].first;
}

bool Poly::isNum() const{
	if (poly.size() == 0)return true;
	if (poly[0].second.isZero())return true;
	return false;
}

bool Poly::isZero() const{
	return poly.empty();
}

int Poly::GetInt(){
	if (poly.empty())return 0;
	for (int i = 0; i < poly.size(); ++i){
		if (poly[i].second.isZero()){
			return poly[i].first.GetInt();
		}
	}
	return 0;
}

BigInt Poly::GetExp() const{
	if (poly.empty())return 0;
	return poly[poly.size() - 1].second;
}

istream& operator>>(istream &is, Poly &a){
	throw "暂时不实现";
	return is;
}