/*
	注意Vector引用&
*/

#include<iostream>
#include<iomanip>

#include "BigInt.h"
#include "SBuild.h"
#include "SVM.h"
#include <sstream>
using namespace std;

int main(){

	/*
	Poly a = -2;
	Poly t(1, 1);
	a += t;
	cout << a << endl;
	Poly b = pow(a, 2);
	cout << b << endl;
	cout << pow(a, 3) << endl;
	*/

	//Poly a(1, 1);
	//cout << a << endl;

	SVM vm;
	streamx ss;
	srand(size_t(time(0)));

	//帮助文件
	SBuild readme;
	//streamx rs;
	//rs << "(readme)";
	//readme.SetStream(rs);
	//vm.Eval(readme.Build());

	vm.SetVar("x", Poly(1, 1));

	while (true){

		vm.SetVar("x", Poly(1, 1));//Poly Project 防止被修改！

		SBuild bu;
		bu.SetStream(ss);
		string sen;
		getline(cin, sen);
		ss.clear();

		/*
			对sen进行解析
		*/

		if (sen.empty())continue;

		//简单分割测试
		vector<string> sp;
		string buf;
		for (int i = 0;i < sen.size();++i) {
			if (sen[i] == ' ') {
				if (!buf.empty())sp.push_back(buf);
				buf = "";
			}else{
				buf += sen[i];
			}
		}
		if (!buf.empty())sp.push_back(buf);

		if(sp.size() >= 1){
			if (sp[0] == "restart" || sp[0] == "clear" || sp[0] == "cls") {
				vm.Restart();
				vm.SetVar("x", Poly(1, 1));
				cout << "已经重启虚拟机" << endl;
				continue;
			}
		}

		if (sp.size() >= 2) {
			if (sp[0] == "print" || sp[0] == "show") {
				if (sp[1] == "all") {
					vm.PrintAllVars();
					continue;
				}
			}
		}

		string postsen;
		string condition;
		string *psen = &postsen;
		bool prenum = false;
		postsen += '#';//SLang展开宏
		condition += '#';
		bool sem = false;//分号
		bool hascon = false;//附加语句，优先级最高

		//psen为string指针，可能会被交换
		for (int i = 0;i < sen.size();++i) {
			if (sen[i] == ' ')continue;//ignore blank
			if (sen[i] == '|') {
				hascon = true;
				prenum = false;
				psen = &condition;
				continue;
			}
			if (sen[i] == ';') {
				sem = true;
				break;
			}
			if ((sen[i] >= '0' && sen[i] <= '9') || sen[i] == ')') {
				prenum = true;
				*psen += sen[i];
				continue;
			}
			if (sen[i] == 'x' && prenum) {
				*psen += "&x";//&符代表系数运算
				prenum = false;
				continue;
			}
			prenum = false;
			*psen += sen[i];
		}

		Poly conx;

		if (hascon) {
			//执行附加条件语句
			SBuild cBuild;
			streamx cons;
			cBuild.SetStream(cons);
			cons << condition;
			SExp *c = cBuild.Build();
			conx = vm.Eval(c);
		}

		ss << postsen;//针对Poly Project 优化~

		//ss << sen;

		SExp *e = bu.Build();
		Poly value = vm.Eval(e);
		vm.SetVar("ans", value);
		//if(!sem)cout << value << endl;

		if (!sem) {
			//value belongs to class Poly
			//if (hascon)value = value.Substitution(conx);
			cout << value << endl;
		}

		DelSExp(e);

	}

	return 0;
}
