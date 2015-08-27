#include "SPar.h"
#include "SVM.h"

SPar::SPar(SVM *vm, SExp *exp){
	this->vm = vm;
	this->exp = exp;
}

int SPar::size(){
	return exp->elems.size();
}

SExp::SEXP_TYPE SPar::GetType(int i){
	return exp->elems[i]->type;
}

string SPar::GetStr(int i){
	return exp->elems[i]->name;
}
Poly SPar::GetNum(int i){
	return vm->Eval(exp->elems[i]);
}

Poly& SPar::operator[](int i){
	return vm->GetVar(exp->elems[i]->name);
}
