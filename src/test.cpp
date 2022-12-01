#include "util.h"

using namespace CSC ;

exports void test_main () {
	/*
	[CVRXL]REF<ARG\d>[^\r]+\{\r\n(?!(\t+require \([^\r]+\) ;\r\n)*\t+return (instance \(\)\.)?mThis->\w+ \((\w+( ,\w+)*)?\) ;)
	*/
}