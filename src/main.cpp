#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

/*
1. Proxy
2. 
*/

int main () {
	Singleton<Console>::expr.show () ;

	return 0 ;
}