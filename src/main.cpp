#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

exports int main () {
	const auto r1x = Box<Box<FLT64>>::make () ;
	const auto r2x = Ref<Ref<FLT64>>::make (Ref<FLT64>::make (32.0f)) ;
	const auto r3x = RefBuffer<int> (200) ;
	const auto r20x = BoxBuffer<float ,RANK5> () ;
	const auto r4x = Clazz (TYPE<float>::expr) ;
	const auto r5x = Exception (slice ("XSSS")) ;
	const auto r6x = Auto (100) ;
	const auto r7x = Auto (200.0f) ;
	const auto r8x = Auto (slice ("30")) ;
	const auto r9x = Vector<FLT64>::axis_w () ;
	const auto r10x = Matrix<FLT64>::identity () ;
	return 0 ;
}