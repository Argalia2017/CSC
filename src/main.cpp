#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

void test_r (CREF<Capture<int ,float ,int>> initval) {
	initval ([] (int a ,float b ,int c) {
		cout << a << "\n" ;
		cout << b << "\n" ;
		cout << c << "\n" ;
	}) ;
}

void test () {
	test_r (capture (1 ,3.2f ,2)) ;
}

exports int main () {
	test () ;
	return 0 ;
}