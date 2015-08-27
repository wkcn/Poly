#include "UBigInt.h"

UBigInt::UBigInt(){
	_len = 1;
	datas[0] = 0;
}

UBigInt::UBigInt(unsigned int u){
	_len = 0;
	if (u == 0){
		_len = 1;
		datas[0] = 0;
		return;
	}
	while(u > 0){
		datas[_len] = u % 10000;
		u /= 10000;
		++_len;
	}
}

UBigInt::UBigInt(const char * s){
	int slen = 0;
	while (s[slen] != '\0')++slen;
	int base = 1;	//基数，这里使用10000进制
	int buf = 0;	//缓冲4位数字
	_len = 0;

	for (int i = slen - 1; i >= 0; i--){
		if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t' || s[i] == '\r' || s[i] == '\b')continue;	//跳过空格
		if (!(s[i] >= '0' && s[i] <= '9')){
			throw "您的输入包含了非数字字符。";
		}
		int k = s[i] - '0';
		buf += k * base;
		if (base == 1000){
			base = 1;
			datas[_len++] = buf;
			buf = 0;
		}
		else{
			base *= 10;
		}
	}
	if (buf > 0){
		datas[_len++] = buf;
	}
	//如果数字长度仍是0
	if (_len == 0){
		_len = 1;
		datas[0] = 0;
	}
}

//比较类
int UBigIntCmp(const UBigInt &a,const UBigInt &b){
	if (a._len != b._len)return a._len - b._len;
	for (int i = a._len - 1; i >= 0; --i){
		if (a.datas[i] != b.datas[i])return a.datas[i] - b.datas[i];
	}
	return 0;
}

bool operator==(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) == 0;
}

bool operator!=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) != 0;
}

bool operator<(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) < 0;
}

bool operator<=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) <= 0;
}

bool operator>(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) > 0;
}

bool operator>=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) >= 0;
}
bool UBigInt::isZero(){
	return (_len == 1 && datas[0] == 0);
}
int UBigInt::GetInt(){
	if (_len == 1)return datas[0];
	return datas[0] + datas[1] * 10000;
}
//完美实现！

//四则运算

UBigInt& UBigInt::operator++(){
	//++a
	return ((*this) += 1);
}
UBigInt UBigInt::operator++(int dummy){
	//a++
	UBigInt old = (*this);
	(*this) += 1;
	return old;
}
UBigInt& UBigInt::operator--(){
	//--a
	return ((*this) -= 1);
}
UBigInt UBigInt::operator--(int dummy){
	//a--
	UBigInt old = (*this);
	(*this) -= 1;
	return old;
}

UBigInt& operator+=(UBigInt &a, const UBigInt &b){
	//不确定的优化
	if (b._len == 1){
		if (b.datas[0] == 0){
			return a;
		}
	}
	int carry = 0;
	//这里采用一种较简单的写法
	//性能有所牺牲，但是容易理解
	//其实还可以进一步优化，比如不存在进位且和的长度已超过其中一个加数时
	int maxLen = a._len > b._len ? a._len : b._len;
	int anum, bnum;
	for (int i = 0; i < maxLen; ++i){
		anum = i < a._len ? a.datas[i] : 0;
		bnum = i < b._len ? b.datas[i] : 0;
		a.datas[i] = anum + bnum + carry;
		carry = a.datas[i] / 10000;
		a.datas[i] %= 10000;
	}
	a._len = maxLen;
	if (carry > 0){
		a.datas[a._len++] = carry;	//由加法性质，carry不会大于10000
	}
	return a;
}

UBigInt& operator-=(UBigInt &a, const UBigInt &b){
	//不确定的优化
	if (b._len == 1){
		if (b.datas[0] == 0){
			return a;
		}
	}
	//按竖式减法，从低位开始减
	//考虑到效率，可以到最后再比较a,b大小
	//简单的长度比较,a<b时
	if (a._len < b._len){
		throw "无符号整数减法a-b中,a不能比b小";
	}
	for (int i = 0; i < b._len; ++i){
		a.datas[i] -= b.datas[i];
		if (a.datas[i] < 0){
			//向高位借位
			//assert(i + 1 < a._len);	//断言存在高位，否则说明a<b
			if (i + 1 >= a._len){
				throw "无符号整数a-b中，a不能比b小";
			}
			--a.datas[i + 1];
			a.datas[i] += 10000;
		}
	}
	while (a.datas[a._len - 1] == 0 && a._len > 1)--a._len;//更新长度
	//这里处理完毕
	//因为大数的高位不会等于0，如果在高位上b>a，则会在之前的借位断言中报错
	return a;
}

UBigInt& operator*=(UBigInt &a, const UBigInt &b){
	//不确定的优化
	if (b._len == 1){
		if (b.datas[0] == 0){
			a.datas.clear();
			a.datas[0] = 0;
			a._len = 1;
			return a;
		}else if (b.datas[0] == 1){
			return a;
		}
	}
	Vector<int> temp;//临时累加器
	for (int i = 0; i < b._len; ++i){
		for (int j = 0; j < a._len; ++j){
			temp[i + j] += b.datas[i] * a.datas[j];
			temp[i + j + 1] += temp[i + j] / 10000;
			temp[i + j] %= 10000;
			//这里简单地做了第一次进位处理，防止溢出
		}
	}
	//w为从左到右数第一个非零位，最小值为0
	int w;
	for (w = temp.size() - 1; w > 0; --w){
		if (temp[w] != 0)break;
	}
	a._len = w + 1;
	a.datas.resize(a._len);
	int carry = 0;
	for (int i = 0; i <= w; ++i){
		temp[i] += carry;
		a.datas[i] = temp[i] % 10000;
		carry = temp[i] / 10000;
	}
	if (carry > 0){
		a.datas[a._len++] = carry;
	}
	return a;
}

//为了更方便地写div和mod,将其封装为一个函数
UBigInt& UBigIntDivide(UBigInt &a, const UBigInt &b, bool mod){
	//使用二分法判断除数某位
	//a的位数小于b的位数的情况（必有a<b）
	if (a._len < b._len){
		if (mod){
			return a;
		}
		else{
			a.datas.clear();
			a.datas[0] = 0;
			a._len = 1;
			return a;
		}
	}
	//特殊情况优化，不是很确定命中频率
	if (b._len == 1){
		if (b.datas[0] == 0){
			throw "除数不能为0";
		}
		if (b.datas[0] == 1){
			//除以1
			if (!mod){
				return a;
			}
			else{
				a.datas.clear();
				a.datas[0] = 0;
				a._len = 1;
				return a;
			}
		}
	}
	//a的位数大于等于b的位数
	Vector<int> res;	//结果缓存
	int notZero = a._len - 1;//a被减后的非零位
	int qlen = a._len - b._len;//for中的继续条件中的变量是时刻更新的
	for (int q = 0; q <= qlen; ++q){
		//q代表将除数靠左后向右移动的格数
		int delta = a._len - b._len - q;//相对位置偏差
		//用二分取商的某位的最大值(由于是万进制，所以hi取10000)
		int j = b._len - 1 + delta;	//当前a被对齐的最高位

		int big = 0;
		int base = 1;
		for (int u = j; u <= notZero; ++u){
			big += a.datas[u] * base;	//曾经写错写为了=
			base *= 10000;
		}

		int lo = big / (b.datas[b._len -1] + 1), hi = big / (b.datas[b._len - 1]) + 1;//[lo,hi)
		//int lo = 0, hi = 10000;
		UBigInt temp;	//将要减去的偏移数
		while (lo < hi){
			int mi = (lo + hi) >> 1;
			temp = b * mi;
			bool isLarge = false;//过大
			//cout << temp << "-" << a << endl;
			if (temp._len - 1 + delta < notZero){
				isLarge = false;
				//对齐后a还是比temp多一位
			}
			else{
				for (int i = temp._len - 1; i >= 0; --i){
					//cout << "cmp" << temp << "_--" << temp.datas[i] << " " << a.datas[i + delta] << endl;
					if (temp.datas[i] > a.datas[i + delta]){
						isLarge = true;
						break;
					}
					else{
						if (temp.datas[i] < a.datas[i + delta])break;
					}
				}
			}
			isLarge ? hi = mi : lo = mi + 1;
		}
		//cout << "--" << temp << endl;
		//--lo为所求
		res[delta] = --lo;
		temp = b * res[delta];	//上面二分过程中的temp不一定是最终结果 = = 因此最后还要算一次
		//对齐位减去temp
		//cout << a << "===" << temp << "[][]" << delta << endl;
		for (int i = 0; i < temp._len; ++i){
			a.datas[i + delta] -= temp.datas[i];
			//检查是否需要向前借位
			for (int w = i + delta; w <= notZero; ++w){
				if (a.datas[w] < 0){
					a.datas[w] += 10000;
					--a.datas[w + 1];
				}
				else{
					break;
				}
			}
		}
		//更新a的最高非零位
		for (; notZero > 0; --notZero){
			if (a.datas[notZero])break;
		}
		//cout <<"=="<< a << endl;
	}
	if (mod){
		//取模
		int w;
		//去除a多余的前缀0,由模的性质，a%b的位数 <= b的位数
		for (w = b._len - 1; w > 0; --w){
			if (a.datas[w] != 0)break;
		}
		a._len = w + 1;
		a.datas.resize(a._len);
	}
	else{
		//w为从左到右数第一个非零位，最小值为0
		int w;
		for (w = res.size() - 1; w > 0; --w){
			if (res[w] != 0)break;
		}
		a._len = w + 1;
		a.datas.resize(a._len);
		for (int i = 0; i <= w; ++i){
			a.datas[i] = res[i];
		}
	}
	return a;
}

UBigInt& operator/=(UBigInt &a, const UBigInt &b){
	return UBigIntDivide(a, b, false);
}
UBigInt& operator%=(UBigInt &a, const UBigInt &b){
	return UBigIntDivide(a, b, true);
}

UBigInt operator+(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp += b;
	return temp;
}
UBigInt operator-(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp -= b;
	return temp;
}
UBigInt operator*(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp *= b;
	return temp;
}
UBigInt operator/(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp /= b;
	return temp;
}
UBigInt operator%(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp %= b;
	return temp;
}


//IO流
ostream& operator<<(ostream &os, UBigInt &&u){
	os.fill('0');
	os << u.datas[u._len - 1];
	for (int i = u._len - 2; i >= 0; i--){
		os << setw(4) << u.datas[i];
	}
	os.fill(' ');
	return os;
}

ostream& operator<<(ostream &os, UBigInt &u){
	//我记得C++ IO流还有一种很简单的补零方法，现在找不到了
	os.fill('0');
	os << u.datas[u._len - 1];
	for (int i = u._len - 2; i >= 0; i--){
		os << setw(4) << u.datas[i];
	}
	os.fill(' ');
	return os;
}

istream& operator>>(istream &is, UBigInt &u){
	//这里以字符串的形式读入，读到无效字符时停止
	//遇到空格也会停止
	char c;
	Vector<char> buf;
	bool read = false;//至少读一次
	while (is.peek() == ' ' || is.peek() == '\n' || is.peek() == '\t'){
		is.get();
	}
	while ((c = is.peek()) && c >= '0' && c <= '9'){
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
	u = UBigInt(cstr);
	delete[] cstr;
	return is;
}
