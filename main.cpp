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
		SBuild bu;
		bu.SetStream(ss);
		string sen;
		getline(cin, sen);
		ss.clear();

		/*
			对sen进行解析
		*/

		string postsen;
		bool prenum = false;
		postsen += '#';//SLang展开宏
		for (int i = 0;i < sen.size();++i) {
			if (sen[i] == ' ')continue;//ignore blank
			if (sen[i] >= '0' && sen[i] <= '9') {
				prenum = true;
				postsen += sen[i];
				continue;
			}
			if (sen[i] == 'x' && prenum) {
				postsen += "*x";
				prenum = false;
				continue;
			}
			prenum = false;
			postsen += sen[i];
		}

		ss << postsen;//针对Poly Project 优化~

		//ss << sen;

		SExp *e = bu.Build();
		Poly value = vm.Eval(e);
		vm.SetVar("ans", value);
		cout << value << endl;
		DelSExp(e);

	}

	return 0;
}
