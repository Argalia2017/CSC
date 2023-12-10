#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

exports int main () {
	auto rax = AutoRef<int>::make (2223) ;
	auto rbx = AutoRef<float> (CSC::move (rax)) ;
	auto rcx = SharedRef<int>::make (220) ;
	auto rdx = rcx ;
	auto rex = UniqueRef<int> ([] (VREF<int> me) {
		cerr << "me = " << me << "\n" ;
		me = 231 ;
	} ,[] (VREF<int> me) {
		cerr << "me = " << me << "\n" ;
	}) ;
	return 0 ;
}