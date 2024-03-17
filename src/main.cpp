#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

#pragma warning (disable :4189)

exports int main () {
	_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF) ;
	cerr << "AA" << "\n" ;
	auto rax = RefBuffer<int> (100) ;
	rax[0] = 0 ;
	rax[1] = 1 ;
	rax[2] = 2 ;
	rax[3] = 3 ;
	cerr << "BB" << "\n" ;
	return 0 ;
}