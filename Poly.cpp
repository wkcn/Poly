#include "Poly.h"

Poly::Poly(){}
Poly::Poly(BigInt co,BigInt exp){
	poly.push_back(make_pair(co, exp));
}
Poly::Poly(int num){
	poly.push_back(make_pair(num, 0));
}
Poly::Poly(const char *s){
	poly.push_back(make_pair(BigInt(s), 0));
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
	throw "Don't Divide";
}

Poly& operator%=(Poly&a, const Poly&b) {
	throw "Don't Mod";
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

ostream& operator<<(ostream &os, Poly &u){

	if (u.poly.size() == 0)os << 0;
	else{
		bool first = true;
		for (int i = 0; i < u.poly.size(); ++i){
			term &t = u.poly[i];

			bool minus = t.first < BigInt(0);
			if (!minus && !first)os << '+';

			if (t.first != BigInt(1)){
				if (t.first == BigInt(-1))os << '-';
				else{
					os << t.first;
				}
			}

			if (!t.second.isZero()){
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
	throw "无法比较";
}

bool operator<=(const Poly&a, const Poly&b){
	throw "无法比较";
}

bool operator>(const Poly&a, const Poly&b){
	throw "无法比较";
}

bool operator>=(const Poly&a, const Poly&b){
	throw "无法比较";
}

bool Poly::isZero(){
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

istream& operator>>(istream &is, Poly &a){
	throw "暂时不实现";
	return is;
}