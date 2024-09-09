#include "util.h"

#include "csc/csc_end.h"
#include <initializer_list>
#include <type_traits>
#include <mutex>
#include "csc/csc_begin.h"

using namespace SOLUTION ;

/*
implicit ~\w+(?<!Layout) \(\)
(?<!CREF<\w+Proc> )instance \(\) \{
Pointer::make \(address
*/

exports int main () {
	Singleton<Console>::instance ().start () ;
	Singleton<Console>::instance ().open (slice (".")) ;

	return 0 ;
}