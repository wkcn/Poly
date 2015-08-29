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
			newPoly.push_back(make_pair(-b.poly[j].first,b.poly[j].second));
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

Poly& PolyDivide(Poly &a, const Poly &b, bool mod) {
	if (b.poly.empty()) throw "不能除以0";
	Vector<term> newPoly;
	int offset = 0;
	while (!a.poly.empty() && offset < a.poly.size()) {
		//下标0为最高次
		term &at = a.poly[offset];
		term &bt = b.poly[0];
		BigInt co = at.first / bt.first;
		if (co.isZero()) {
			++offset;
			continue;
		}
		BigInt ex = at.second - bt.second;
		if (ex.isMinus())break;
		Poly s(co, ex);
		newPoly.push_back(make_pair(co, ex));

		BigInt lastExp = at.second;//保证被除数对应数的次数降低

		a -= s*b;
		offset = 0;
		for (;offset < a.poly.size();++offset) {
			if (a.poly[offset].second < lastExp)break;
		}

		if (offset >= a.poly.size())break;
	}
	if (!mod)a.poly = newPoly;
	return a;
}

Poly& operator/=(Poly&a, const Poly&b) {
	return PolyDivide(a,b,false);
}

Poly& operator%=(Poly&a, const Poly&b) {
	return PolyDivide(a, b, true);
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

BigInt Poly::GetBigInt() const{
	if (!isNum())return 0;
	if (poly.empty())return 0;
	return poly[0].first;
}

Poly pow(const Poly &a,const Poly &b){
	if (!b.isNum())throw "指数必须为数字！";
	BigInt u = b.GetBigInt();
	if (u.isMinus())throw "目前暂不支持负数次方运算";
	Poly p = a;
	Poly result = 1;
	/*
	int j = u.GetInt();
	
	for (; j > 0; j >>= 1){
		if (j & 1){
			result *= p;
		}
		p *= p;
	}
	return result;
	*/
	
	for (;!u.isZero();u /= 2) {
		if (!(u % BigInt(2)).isZero()) {
			result *= p;
		}
		p *= p;
	}
	return result;
}

//求导
Poly Poly::Derivative() {
	Poly newPoly;
	for (int i = 0;i < poly.size();++i) {
		if (!poly[i].second.isZero()) {
			newPoly.poly.push_back(make_pair(poly[i].first*poly[i].second, poly[i].second - BigInt(1)));
		}
	}
	return newPoly;
}

//代入
Poly Poly::Substitution(const Poly&p) {

	if (p == Poly(1, 1))return *this;//当以x代换时

	Poly newPoly;
	for (int i = 0;i < poly.size();++i) {
		term &t = poly[i];
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

			BigInt coe = t.first;
			const BigInt one(1);

			//符号

			if (!minus && !first)os << " + ";

			if (t.first.isMinus()) {
				if (first)os << "-";
				else os << " - ";
				coe = -t.first;
			}

			//显示非1或非-1的x^i（i!=0）项或常数项
			if (t.first != one || secondZero){
				os << coe;
			}

			if (!secondZero){
				os << 'x';
				if (t.second != one){
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
	if (a.poly.size() == 0 && b.poly.size() == 0)return false;
	if (a.poly.size() == 0)return BigInt(0) < b.poly[0].first;
	if (b.poly.size() == 0)return a.poly[0].first < BigInt(0);
	return a.poly[0].first < b.poly[0].first;
}

bool operator<=(const Poly&a, const Poly&b){
	if (!a.isNum() || !b.isNum())throw "无法比较";
	if (a.poly.size() == 0 && b.poly.size() == 0)return false;
	if (a.poly.size() == 0)return BigInt(0) <= b.poly[0].first;
	if (b.poly.size() == 0)return a.poly[0].first <= BigInt(0);
	return a.poly[0].first <= b.poly[0].first;
}

bool operator>(const Poly&a, const Poly&b){
	if (!a.isNum() || !b.isNum())throw "无法比较";
	if (a.poly.size() == 0 && b.poly.size() == 0)return false;
	if (a.poly.size() == 0)return BigInt(0) > b.poly[0].first;
	if (b.poly.size() == 0)return a.poly[0].first > BigInt(0);
	return a.poly[0].first > b.poly[0].first;
}

bool operator>=(const Poly&a, const Poly&b){
	if(!a.isNum()||!b.isNum())throw "无法比较";
	if (a.poly.size() == 0 && b.poly.size() == 0)return false;
	if (a.poly.size() == 0)return BigInt(0) >= b.poly[0].first;
	if (b.poly.size() == 0)return a.poly[0].first >= BigInt(0);
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
	int u = poly.size() - 1;//最低项在尾部
	if (poly[u].second.isZero())return poly[u].first.GetInt();
	return 0;
}

BigInt Poly::GetExp() const{
	if (poly.empty())return 0;
	return poly[0].second;
}

istream& operator>>(istream &is, Poly &a){
	throw "暂时不实现";
	return is;
}