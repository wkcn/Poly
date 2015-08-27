#ifndef SBUILD_H
#define SBUILD_H

#include "Defines.h"

//处理流的
class streamx{
private:
	queue<string> buf;
	unsigned int pointer;
	bool eofed;
	void Update();
public:
	streamx();
	char get();
	char peek();
	bool eof();
	void clear();
	streamx& operator<<(const string&);
};

//Sugar酱，语法糖好好吃
//语法糖#(3+1)*5	-> (* (+ 3 1) 5)
//		!help\n		-> (help)

class SSugar{
private:
	streamx *is;	//用流处理吧
	queue<char> buf;
	bool updated;
	map<string,int> level;
	char nowchar;
	string word;
private:
	bool isOP(char c);
	bool isBlank(char c);
	string NewSExp(string name, string left, string right);
	char GetC(bool &can,bool del = true);
	void PushS(stack<string> &res, stack<string> &op);
	inline int ISP(string &op);	//栈内优先级
	inline int ICP(string &op); //输入优先级
	void Update();
public:
	SSugar();
	string Formula2Sexp(string);	//中缀表达式转S表达式
	string peek();
	string next();
	bool eof();
	void SetStream(streamx *is);
};

//S表达式创建
class SBuild{
private:
	SSugar sugar;	//本来有Reader的，后来让Sugar酱代替了
	SExp* macro;
	bool* ismacro;
private:
	void MarkParent(SExp *);
public:
	SBuild();
	void SetStream(streamx &is);
	void SetMacro(SExp *,bool *ismacro);
	SExp* Build(bool first = true);
};

#endif
