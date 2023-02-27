#include "util.h"

#include "csc/csc_end.h"
#include <random>
#include "csc/csc_begin.h"

using namespace CSC ;

/*
^(\t+)if ifswitch \([^\r]+\) \{(\r\n(?!(\1)\})[^\r]+)*\r\n(\1)\treturn
^(\t+)if ifswitch \([^\r]+\) \{(\r\n(?!(\1)\})[^\r]+)*\r\n(\1)\}\r\n\t+(return [^\r]*;\r\n\t+)?\}(\r\n\r\n|\r\n\t+\} ;)
[CVRXL]REF<ARG\d>[^\r]+\{\r\n(?!(\t+require \([^\r]+\) ;\r\n)*\t+return (instance \(\)\.)?mThis->\w+ \((\w+( ,\w+)*)?\) ;)
*/

exports void test_main () {

}