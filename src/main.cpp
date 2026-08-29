#include "util.h"

#include <csc_end.h>
#define _CRT_SECURE_NO_WARNINGS
#include <initializer_list>
#include <csc_begin.h>

using namespace ROUTINE ;

int test () ;

int main () {
	CLOG.show () ;
	CLOG.open (slice (".")) ;
	const auto r1x = ArrayList<int> ({1 ,3 ,5 ,7 ,9}) ;
	noop (r1x) ;
	return 0 ;
}