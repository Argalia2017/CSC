#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

static const auto anonymous = invoke ([] () {
	Singleton<Console>::expr.open (slice (".")) ;
	Singleton<Console>::expr.show () ;
	Singleton<Console>::expr.debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	return TRUE ;
}) ;

int test () ;

int main () {
	auto rax = Deque<int> (0) ;
	auto &&rbx = keep[TYPE<DequeLayout>::expr] (rax) ;
	assume (!rbx.mDeque.exist ()) ;

	return 0 ;
}