#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

exports int main () {
	auto rax = Box<Box<FLT64>>::make () ;
	auto rbx = Ref<Ref<FLT64>>::make (Ref<FLT64>::make (32.0f)) ;
	auto rdx = RefBuffer<int> (200) ;
	const auto r1x = Clazz (TYPE<float>::expr) ;
	const auto r2x = Auto (100) ;
	const auto r3x = Exception (slice ("XSSS")) ;
	return 0 ;
}