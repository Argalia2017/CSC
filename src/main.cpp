#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

exports int main () {
	auto rax = Box<FLT64>::make (32.0f) ;
	auto rbx = Ref<FLT64>::make (32.0f) ;
	auto rdx = RefBuffer<int> (2) ;
	return 0 ;
}