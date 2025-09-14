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
	auto rax = Allocator<int ,AllocatorNode> () ;
	auto rbx = rax.alloc (Box<int>::make (1)) ;

	return 0 ;
}