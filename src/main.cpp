#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

exports int main () {
	const auto r1x = Box<Box<FLT64>>::make () ;
	const auto r2x = Ref<Ref<FLT64>>::make (Ref<FLT64>::make (32.0f)) ;
	const auto r3x = RefBuffer<int> (200) ;
	const auto r4x = Clazz (TYPE<float>::expr) ;
	const auto r6x = Exception (slice ("XSSS")) ;
	const auto r10x = Auto (100) ;
	const auto r11x = Auto (200.0f) ;
	const auto r12x = Auto (slice ("30")) ;
	return 0 ;
}