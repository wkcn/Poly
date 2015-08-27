#ifndef VECTOR_H
#define VECTOR_H
#pragma warning (disable:4819)
/*
	Vector类
	这里的Vector类能自动根据下标调整空间大小
	并且会自动初始化各个元素
*/


const int DEFAULT_CAPACITY = 8; //默认初始容量

template<typename T>
class Vector{
protected:
	T *_elem;
	int _size;	//当前使用容量
	int _cap;	//当前申请容量
private:
	void CheckMem();//检查当前容量
	void Copy(T* addr);	//拷贝当前数据

public:
	Vector();
	Vector(const Vector<T>&);
	Vector<T>& operator=(const Vector<T>&);	//克隆	（上面的构造函数只会在变量初始化时调用！带指针的类要注意写等号重载）
	~Vector();

	void push_back(const T& e);
	T pop_back();

	T* c_array();	//返回C类型的数组指针

	void clear();
	void resize(int newSize);

	int size() const;
	bool empty() const;

	T& operator[](int i);
	T& operator[](int i) const;	//常量模式的访问

};

/*
Vector的实现
这一点有点讨厌
部分C++编译器不支持export语句，模板类分离时出现问题。
*/

template <typename T>
Vector<T>::Vector(){
	_size = 0;
	_cap = DEFAULT_CAPACITY;
	_elem = new T[_cap];
}

template <typename T>
Vector<T>::Vector(const Vector<T> &v){
    if (this == &v) return;
	//Vector<T> a = b时调用
	//或者强制转换时调用
	_size = v.size();
	_cap = 0;
	_elem = 0;
	CheckMem();
	for (int i = 0; i < v.size(); i++){
		_elem[i] = v._elem[i];
	}
	//_size = v.size();
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &v){
    if (this == &v)return *this;
	_size = v.size();
	CheckMem();
	for (int i = 0; i < v.size(); i++){
		_elem[i] = v._elem[i];
	}
	return *this;
}

template<typename T>
Vector<T>::~Vector(){
	if (_elem){
		//cout << "dele" << int(_elem) << endl;
		delete[] _elem;
	}
}

template<typename T>
void Vector<T>::CheckMem(){
	//貌似用delete[]删除长度为1的数组时会报错？
	//扩容
	if (_size >= _cap){
		int _newcap = _size << 1;//乘以2，虽然这有点像不成熟的优化
		if (_newcap < DEFAULT_CAPACITY)_newcap = DEFAULT_CAPACITY;
		T *q = new T[_newcap];
		Copy(q);
		if(_elem)delete[] _elem;
		_elem = q;
		_cap = _newcap;
		return;
	}
	//缩小
	if (_size * 3 <= _cap && (_size * 2) >= DEFAULT_CAPACITY){
		int _newcap = _size << 1;
		T *q = new T[_newcap];
		Copy(q);
		if(_elem)delete[] _elem;
		_elem = q;
		_cap = _newcap;
		return;
	}
}

template<typename T>
void Vector<T>::Copy(T* addr){
	int s = _size < _cap ? _size : _cap;	//避免resize扩容问题
	for (int i = 0; i < s; i++){
		addr[i] = _elem[i];
	}
}

//公开操作函数
template<typename T>
T& Vector<T>::operator[](int i){
	if (i < 0){
		throw "使用了负数下标，错误！";
	}
	//这样写影响了效率，稍微降低了性能
	//但这能使设计更方便。
	if (i >= _size){
		int oldsize = _size;
		_size = i + 1;
		CheckMem();
		for (int k = oldsize; k <= i; k++){
			//初始化
			_elem[k] = T();
		}
	}
	return _elem[i];
}
//常量模式
template<typename T>
T& Vector<T>::operator[](int i) const{
	if (i >= _size){
		throw "下标越界！";
	}
	return _elem[i];
}

template<typename T>
void Vector<T>::push_back(const T& e){
	CheckMem();
	_elem[_size++] = e;
}

template<typename T>
T Vector<T>::pop_back(){
	if (_size <= 0){
		throw "POP了空Vector，错误";
	}
	return _elem[--_size];
}

template<typename T>
T* Vector<T>::c_array(){
	T* q = new T[_size];
	for (int i = 0; i < _size; i++){
		q[i] = _elem[i];
	}
	return q;
}

//大小
template<typename T>
int Vector<T>::size() const{
	return _size;
}

template<typename T>
bool Vector<T>::empty() const{
	return _size == 0;
}

//清除类
template<typename T>
void Vector<T>::clear(){
	_size = 0;
	CheckMem();
}

template<typename T>
void Vector<T>::resize(int newSize){
	int oldSize = _size;
	_size = newSize;
	CheckMem();
	//如果是扩容
	for (int i = oldSize; i < newSize; ++i){
		_elem[i] = T();
	}
}

#endif
