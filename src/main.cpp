#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

int test () ;

int main () {
	Singleton<Console>::expr.open (slice (".")) ;
	Singleton<Console>::expr.show () ;
	Singleton<Console>::expr.debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;

	test () ;

	return 0 ;
}