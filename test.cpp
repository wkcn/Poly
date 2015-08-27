#include "Poly.h"

int TestPoly(){
	Poly a,b;
	Poly t;
	t = Poly(1, 1); a += t;
	t = Poly(2, 0); a += t;

	t = Poly(1, 2); b += t;
	t = Poly(1, 1); b += t;
	t = Poly(7, 0); b += t;

	cout << a << endl;
	cout << b << endl;
	cout << a + b << endl;
	cout << a - b << endl;
	cout << a * b << endl;
	return 0;
}