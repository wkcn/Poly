#include "BigInt.h"

BigInt::BigInt(){
	minus = false;
	//num会自动初始化
}

BigInt::BigInt(int u){
	if (u < 0){
		minus = true;
		u = -u;
	}
	else{
		minus = false;
	}
	num = (unsigned int)u;
}

BigInt::BigInt(const char * s){
	int slen = 0;
	while (s[slen] != '\0')++slen;
	//bool found = false;//找到负号
	int i;
	for (i = 0; i < slen; ++i){
		if (s[i] != ' ')break;
	}
	if (i < slen && s[i] == '-'){
		minus = true;
		num = UBigInt(s + i + 1);
	}
	else{
		minus = false;
		num = UBigInt(s + i);
	}
}

BigInt::operator string(){
	stringstream ss;
	ss << (*this);
	return ss.str();
}

/*
BigInt::operator int(){
	return GetInt();
}
*/

//比较类
int BigIntCmp(const BigInt &a, const BigInt &b){
	if (!a.minus && !b.minus){
		return UBigIntCmp(a.num, b.num);
	}
	else{
		if (a.minus && b.minus){
			return -UBigIntCmp(a.num, b.num);
		}
		else{
			return a.minus ? -1 : 1;
		}
	}
	return 0;
}

bool operator==(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) == 0;
}

bool operator!=(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) != 0;
}

bool operator<(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) < 0;
}

bool operator<=(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) <= 0;
}

bool operator>(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) > 0;
}

bool operator>=(const BigInt &a, const BigInt &b){
	return BigIntCmp(a, b) >= 0;
}
bool BigInt::isZero(){
	return num.isZero();
}
bool BigInt::isMinus() {
	return minus;
}
int BigInt::GetInt(){
	int w = num.GetInt();
	if (minus) w = -w;
	return w;
}
//完美实现！

//四则运算

BigInt BigInt::operator-(){
	BigInt temp = *this;
	temp.minus = !temp.minus;
	return temp;
}

BigInt& BigInt::operator++(){
	//++a
	return ((*this) += 1);
}
BigInt BigInt::operator++(int dummy){
	//a++
	BigInt old = (*this);
	(*this) += 1;
	return old;
}
BigInt& BigInt::operator--(){
	//--a
	return ((*this) -= 1);
}
BigInt BigInt::operator--(int dummy){
	//a--
	BigInt old = (*this);
	(*this) -= 1;
	return old;
}

BigInt& operator+=(BigInt &a, const BigInt &b){
	if (a.minus ^ b.minus){
		//异号
		int w = UBigIntCmp(a.num, b.num);
		if (w > 0){
			a.num -= b.num;
		}
		else{
			if (w == 0){
				a.num = UBigInt();//不知道为什么用不了a.num = 0
			}
			else{
				a.num = b.num - a.num;
				a.minus = b.minus;
			}
		}
	}
	else{
		a.num += b.num;
	}
	return a;
}
BigInt& operator-=(BigInt &a, const BigInt &b){
	if (a.minus ^ b.minus){
		//异号
		a.num += b.num;
	}
	else{
		int w = UBigIntCmp(a.num, b.num);
		if (w > 0){
			a.num -= b.num;
		}
		else{
			if (w == 0){
				a.num = UBigInt();
			}
			else{
				a.num = b.num - a.num;
				a.minus = !a.minus;
			}
		}
	}
	return a;
}
BigInt& operator*=(BigInt &a, const BigInt &b){
	a.minus = a.minus ^ b.minus;
	a.num *= b.num;
	if (a.num.isZero())a.minus = false;
	return a;
}
BigInt& operator/=(BigInt &a, const BigInt &b){
	//这里采用向零取整
	a.minus = a.minus ^ b.minus;
	a.num /= b.num;
	if (a.num.isZero())a.minus = false;
	return a;
}
BigInt& operator%=(BigInt &a, const BigInt &b){
	//这里取模的方式
	//为模的绝对值 = |a| % |b|，模的符号等于被除数符号
	//其实这里是有争议的，即使是C++，不同编译器的取模方式都不一样
	a.num %= b.num;
	if (a.num.isZero())a.minus = false;
	return a;
}
BigInt operator+(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp += b;
	return temp;
}
BigInt operator-(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp -= b;
	return temp;
}
BigInt operator*(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp *= b;
	return temp;
}
BigInt operator/(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp /= b;
	return temp;
}
BigInt operator%(const BigInt &a, const BigInt &b){
	BigInt temp = a;
	temp %= b;
	return temp;
}


//IO流
ostream& operator<<(ostream &os, BigInt &&u){
	if (u.minus)os << "-";//不会存在-0情况
	os << u.num;
	return os;
}
ostream& operator<<(ostream &os, BigInt &u){
	if (u.minus)os << "-";//不会存在-0情况
	os << u.num;
	return os;
}

istream& operator>>(istream &is, BigInt &u){
	//这里以字符串的形式读入，读到无效字符时停止
	//遇到空格也会停止
	char c;
	Vector<char> buf;
	bool read = false;//至少读一次
	while (is.peek() == ' ' || is.peek() == '\n' || is.peek() == '\t'){
		is.get();
	}
	while ((c = is.peek()) && ((c >= '0' && c <= '9') || c == '-')){
		is.get();
		read = true;
		buf.push_back(c);
	}
	if (c == '\r'){
		is.get();
		read = true;
		c = is.peek();	//避免文件流中\r\n的情况
	}
	if (c == '\n' || c == ' ' || c == '\t' || c == '\b'){
		is.get();
		read = true;
	}
	if (!read)is.get();
	buf.push_back('\0');
	char *cstr = buf.c_array();
	u = BigInt(cstr);
	delete[] cstr;
	return is;
}
