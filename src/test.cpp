#include "util.h"

using namespace CSC ;

exports void test_main () {
	const auto r1x = Vector<DOUBLE> (1 ,2 ,3 ,0).normalize () ;
	const auto r2x = MathProc::radian_angle (DOUBLE (33)) ;
	const auto r3x = RotationMatrix<DOUBLE>::make (r1x ,r2x) ;
	const auto r4x = Quaternion<DOUBLE> (r3x) ;
	const auto r5x = r4x.axis () ;
	const auto r6x = r4x.angle () ;
	unittest (r5x) ;
	unittest (r6x) ;
}

/*
[CVRXL]REF<ARG\d>[^\r]+\{\r\n(?!(\t+require \([^\r]+\) ;\r\n)*\t+return (instance \(\)\.)?mThis->\w+ \((\w+( ,\w+)*)?\) ;)
*/