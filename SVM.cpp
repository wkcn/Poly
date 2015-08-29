#include "SVM.h"
#include "SBuild.h"

//一些内置函数
Poly Sprint(SPar par){
	bool first = true;
	for (int i = 0; i < par.size(); ++i){
		if (!first)cout << " ";
		first = false;
		if (par.GetType(i) == SExp::STR){
			cout << par.GetStr(i);
		}
		else{
			cout << par.GetNum(i);
		}
	}
	cout << endl;
	return Poly();
}

Poly Sprintf(SPar par){
	for (int i = 0; i < par.size(); ++i){
		if (par.GetType(i) == SExp::STR){
			cout << par.GetStr(i);
		}
		else{
			cout << par.GetNum(i);
		}
	}
	return Poly();
}

Poly SRestart(SPar par){
	par.vm->Restart();
	cout << "已经成功重启虚拟机" << endl;
	return Poly();
}

Poly SClear(SPar par){
	for (int i = 0; i < par.size(); ++i){
		par.vm->ClearVar(par.GetStr(i));
	}
	return Poly();
}

Poly SInput(SPar par){
	/*
	Vector<BigInt> b;
	for (int i = 0; i < par.size(); ++i){
		cin >> b[i];//par[i];
	}
	for (int i = 0;i < par.size();++i) {
		par[i] = Poly(b[i]);
	}
	*/
	//Poly Project
	for (int i = 0;i < par.size();++i) {
		string sen;
		getline(cin, sen);

		string postsen;
		string *psen = &postsen;
		bool prenum = false;
		postsen += '#';//SLang展开宏
		bool sem = false;//分号

		//psen为string指针，可能会被交换
		for (int i = 0;i < sen.size();++i) {
			if (sen[i] == ' ')continue;//ignore blank
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

		SVM vm;
		vm.SetVar("x", Poly(1, 1));//重要！
		streamx ss;
		SBuild bu;
		bu.SetStream(ss);
		ss << postsen;
		SExp *e = bu.Build();
		par[i] = vm.Eval(e);
	}
	return Poly();
}

Poly SRand(SPar par){
	return rand();
}

Poly STime(SPar par){
	return int(time(0));
}

Poly SClock(SPar par){
	return clock() * 1000 / CLOCKS_PER_SEC;
}

Poly SAddPath(SPar par){
	for (int i = 0; i < par.size(); ++i){
		par.vm->AddPath(par.GetStr(i));
	}
	return Poly();
}

Poly SArrayGet(SPar par){
	//(array_get "a" 0)	获取a[0]
	return par.vm->GetArray(par.GetStr(0), par.GetNum(1).GetInt());
}

Poly SArraySet(SPar par){
	//(array_set "a" 0 11)
	par.vm->SetArray(par.GetStr(0), par.GetNum(1).GetInt(), par.GetNum(2));
	return par.GetNum(2);
}

Poly SArraySize(SPar par){
	return par.vm->GetArraySize(par.GetStr(0));
}

Poly SArrayClear(SPar par){
	for (int i = 0; i < par.size(); ++i){
		par.vm->ClearArray(par.GetStr(i));
	}
	return Poly();
}

Poly SDebug(SPar par){
	par.vm->debug = !(par.GetNum(0).isZero());
	string msg[2] = { "关闭", "开启" };
	cout << msg[par.vm->debug] << "Debug模式";
	return Poly();
}

SVM::SVM(){
	debug = false;
	parent = 0;

	AddPath("./");	//添加根目录
	AddPath("./data/");
	AddPath("./Data/");
	AddPath("./DATA/");

	AddFunc("print", Sprint);
	AddFunc("printf", Sprintf);
	AddFunc("restart", SRestart);
	AddFunc("re", SRestart);
	AddFunc("clear", SClear);
	AddFunc("input", SInput);
	AddFunc("rand", SRand);
	AddFunc("time", STime);
	AddFunc("clock", SClock);
	AddFunc("addpath", SAddPath);
	AddFunc("array_set", SArraySet);
	AddFunc("array_get", SArrayGet);
	AddFunc("array_size", SArraySize);
	AddFunc("array_clear", SArrayClear);
	AddFunc("debug", SDebug);
}

SVM::~SVM(){
	for (map<string, SExp*>::iterator iter = sfuncs.begin(); iter != sfuncs.end(); ++iter){
		DelSExp(iter->second);
	}
}

void SVM::AddFunc(const string &name, SFunc f){
	funcs[name] = f;
}

void SVM::AddPath(const string &name){
	path.push_back(name);
}

void SVM::PrintAllVars() {
	map<string, Poly>::iterator iter;
	for (iter = vars.begin();iter != vars.end();++iter) {
		//不打印数字
		if(iter->first[0] < '0' || iter->first[0] > '9')cout << iter->first << " = " << iter->second << endl;
	}
}

void SVM::Restart(){
	vars.clear();
	polyVars.clear();
	arrays.clear();
	sfuncs.clear();
}

Vector<Poly>& SVM::FindArray(const string &name){

	return arrays[name];

	//以下暂不考虑，可以实现全局变量
	if (name[0] == '_')return arrays[name];
	SVM *node = this;
	while (node){
		if (node->arrays.count(name))return node->arrays[name];
		node = node->parent;
	}
	return arrays[name];
}

void SVM::SetArray(const string &name, int id, const Poly &value){
	FindArray(name)[id] = value;
}

Poly SVM::GetArray(const string &name, int id){
	return FindArray(name)[id];
}

int SVM::GetArraySize(const string &name){
	return FindArray(name).size();
}

void SVM::ClearArray(const string &name){
	arrays.erase(name);
	return;


	SVM *node = this;
	while (node){
		if (node->arrays.count(name)){ node->arrays.erase(name); break; }
		node = node->parent;
	}
	//arrays.erase(name);
}

void SVM::ClearVar(const string &v){
	vars.erase(v);	//直接删除好了
}

Poly& SVM::GetVar(const string &name){
	//cout << "find" << name << endl;
	if (name == "x")return vars["x"];//递归基，防止死循环

	if (name[0]=='_'){
		int id;
		sscanf(name.c_str(), "_%d", &id);
		return arrays["_"][id];
	}
	if (!vars.count(name)){
		if ((name[0] >= '0' && name[0] <= '9')||name[0] == '-'){
			vars[name] = Poly(name.c_str());
			//数字，这里我故意留一个'Bug'，你可以自己定义某个数字的数值，为了好玩
		}
		else{
			vars[name] = Poly();	//一般变量(好像没必要再次初始化？)
		}
	}
	//return (vars[name]);
	//多项式系统修改
	//注意，引用必须要有实例，必须生成实例！！！
	if (!vars[name].GetExp().isZero() && vars["x"] != Poly(1,1)) {
		stringstream ss;
		Poly &pu = vars[name];
		ss << pu;
		string pname = ss.str();
		polyVars[pname] = pu.Substitution(vars["x"]);
		return polyVars[pname];
	}
	return vars[name];
}

Poly SVM::Eval(SExp *e){
	return GetValue(e);
}

void SVM::SetParent(SVM *p){
	parent = p;
}

SExp* SVM::SearchFile(const string &filename,SExp *exp,bool *ismacro){
	string name;
	ifstream fin;
	bool can = false;
	for(int i=0;i<path.size();++i){
		name = path[i] + filename + ".txt";
		//cout << name << endl;
		fin.open(name);
		if (!fin.fail()){
			can = true;
			break;
		}
	}
	if (!can)return 0;
	SBuild bu;	//build器是一次性的，所以不用担心一些影响未来的问题
	streamx ss;
	if(exp)bu.SetMacro(exp,ismacro);	//宏
	bu.SetStream(ss);
	string buf;
	while (!fin.eof()){
		getline(fin, buf);	//eof好烦
		ss << buf;
	}
	fin.close();
	return bu.Build();
}

void SVM::SetVar(const string &name, const Poly &value){
	vars[name] = value;
}

Poly SVM::GetValue(SExp *s){
	if (s == 0)return Poly();
	if (debug){
		string msg[] = { "VAR", "FUNC", "COMMAND", "IF", "WHILE", "STR" };
		if(s->type == SExp::FUNC || s->type == SExp::IF || s->type == SExp::WHILE)cout << msg[s->type];
		cout << s->name << " ";
		//if (s->type == SExp::FUNC || s->type == SExp::IF || s->type == SExp::WHILE)cout << endl;
	}
	try{
		if (s->type == SExp::COMMAND || s->name[0] == ',' || s->name[0] == ';'){
			if (s->elems.size() == 0)return Poly();	//空语句
			for (int i = 0; i < s->elems.size() - 1; ++i){
				GetValue(s->elems[i]);
			}
			return GetValue(s->elems[s->elems.size() - 1]);
		}
		else if (s->type == SExp::FUNC){
			if (s->name == "+"){
				Poly count;//累加器
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count += GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "-"){
				Poly count;//累加器
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count -= GetValue(s->elems[i]);
				}
				return count;
			}
			//多项式系数增加&运算
			else if (s->name == "*" || s->name == "&"){
				Poly count;//累加器
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count *= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "/"){
				Poly count;//累加器
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count /= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "%"){
				Poly count;//累加器
				if (s->elems.size() > 0)count = GetValue(s->elems[0]);
				for (int i = 1; i < s->elems.size(); ++i){
					count %= GetValue(s->elems[i]);
				}
				return count;
			}
			else if (s->name == "="){
				//赋值
				return (GetVar(s->elems[0]->name) = GetValue(s->elems[1]));
			}
			else if (s->name == "+="){
				return (GetVar(s->elems[0]->name) += GetValue(s->elems[1]));
			}
			else if (s->name == "-="){
				return (GetVar(s->elems[0]->name) -= GetValue(s->elems[1]));
			}
			else if (s->name == "*="){
				return (GetVar(s->elems[0]->name) *= GetValue(s->elems[1]));
			}
			else if (s->name == "/="){
				return (GetVar(s->elems[0]->name) /= GetValue(s->elems[1]));
			}
			else if (s->name == "%="){
				return (GetVar(s->elems[0]->name) %= GetValue(s->elems[1]));
			}
			else if (s->name == "=="){
				return (GetValue(s->elems[0]) == GetValue(s->elems[1]));
			}
			else if (s->name == "!="){
				return (GetValue(s->elems[0]) != GetValue(s->elems[1]));
			}
			else if (s->name == "<"){
				return (GetValue(s->elems[0]) < GetValue(s->elems[1]));
			}
			else if (s->name == "<="){
				return (GetValue(s->elems[0]) <= GetValue(s->elems[1]));
			}
			else if (s->name == ">"){
				return (GetValue(s->elems[0]) > GetValue(s->elems[1]));
			}
			else if (s->name == ">="){
				return (GetValue(s->elems[0]) >= GetValue(s->elems[1]));
			}
			else if (s->name == "^"){
				return pow(GetValue(s->elems[0]), GetValue(s->elems[1]));
			}
			else if (s->name == "\'") {
				return GetValue(s->elems[0]).Derivative();
			}
			else{
				//其他函数
				if (funcs.count(s->name) == 0){
					SExp *e = 0;
					if (sfuncs.count(s->name) == 0){
						/*
						if (s->name[0] == '`'){
							//宏函数，很强大！
							e = SearchFile(s->name.substr(1), s);
							if(s->parent)s->parent->elems[s->id] = e;	//宏替换
							//sfuncs[s->name] = e;
							DelSExp(s);	//回收资源，宏替换使用的是深拷贝
							return GetValue(e);
						}
						else{
							//普通函数
							e = SearchFile(s->name,s);
							sfuncs[s->name] = e;	//记录
						}*/
						//将普通函数和宏合在一起吧.
						//现在的宏展开效率不高，因为有很多重复项，而且有些不项目其实不用新建或移动
						bool ismacro = false;
						e = SearchFile(s->name, s,&ismacro);
						if (ismacro){
							if (s->parent)s->parent->elems[s->id] = e;	//宏替换
							DelSExp(s);	//回收资源，宏替换使用的是深拷贝
						}
						else{
							sfuncs[s->name] = e;
						}
						if(ismacro)return GetValue(e);
					}
					else{
						e = sfuncs[s->name];
					}
					if (e){
						SVM fvm;
						//传递参数到新的虚拟层
						char temp[64];
						fvm.debug = debug;
						fvm.SetParent(this);

						//Poly Project
						fvm.SetVar("x", GetVar("x"));

						//fvm.vars["_0"] = Poly(s->elems.size());	//_0变量为参数个数，第一个参数叫_1
						fvm.SetArray("_", 0, Poly(s->elems.size()));
						for (int i = 0; i < s->elems.size(); ++i){
							sprintf(temp, "_%d", i + 1);
							//fvm.vars[temp] = GetValue(s->elems[i]);
							fvm.SetArray("_", i + 1, GetValue(s->elems[i]));
						}
						return fvm.Eval(e);
					}
					cout << "错误： 不存在函数：" << s->name << "\n";
					return Poly();
				}
				return funcs[s->name](SPar(this, s));
			}
		}
		else if (s->type == SExp::VAR){
			return GetVar(s->name);
		}
		else if (s->type == SExp::IF){
			if ((GetValue(s->elems[0])).isZero()){
				if (s->elems.size() == 3)return GetValue(s->elems[2]);
				return Poly();
			}
			else{
				return GetValue(s->elems[1]);
			}
		}
		else if (s->type == SExp::WHILE){
			while (!(GetValue(s->elems[0])).isZero()){
				for (int i = 1; i < s->elems.size(); ++i){
					GetValue(s->elems[i]);
				}
			}
			return Poly();//循环结束返回的是0
		}
	}
	catch (const char *msg){
		cout << msg << endl;
		cout << "计算出错！请检查您输入的语句是否正确"<<endl;
	}
	catch (...){
		cout << "出现了异常，请检查您输入的语句是否正确" << endl;
	}
	return Poly();	//无效命令
}
