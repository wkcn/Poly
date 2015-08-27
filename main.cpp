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

	Poly a(1, 1);
	cout << a << endl;

	SVM vm;
	streamx ss;
	srand(size_t(time(0)));

	//帮助文件
	SBuild readme;
	//streamx rs;
	//rs << "(readme)";
	//readme.SetStream(rs);
	//vm.Eval(readme.Build());

	while (true){
		SBuild bu;
		bu.SetStream(ss);
		string sen;
		getline(cin, sen);
		ss.clear();
		ss << sen;
		SExp *e = bu.Build();
		Poly value = vm.Eval(e);
		vm.SetVar("ans", value);
		cout << value << endl;
		DelSExp(e);
	}

	return 0;
}
