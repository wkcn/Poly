#include "Defines.h"

SExp::SExp(){
	parent = 0;
}

void SExp::Copy(SExp *e){
	if (!e)return;
	//主要处理parent、id和elems
	(*this) = *e;//浅拷贝
	parent = 0;
	for (int i = 0; i < e->elems.size(); ++i){
		elems[i] = new SExp();
		elems[i]->Copy(e->elems[i]);
		elems[i]->parent = this;
		elems[i]->id = i;
	}
}

void DelSExp(SExp *e){
	if (e == 0)return;
	for (int i = 0; i < e->elems.size(); ++i){
		DelSExp(e->elems[i]);
	}
	delete e;
}
