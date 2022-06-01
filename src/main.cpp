#include "util.h"

using namespace UNITTEST ;

exports void test_main () ;

#ifdef __CSC_TARGET_EXE__
exports int main () {
	Singleton<Reporter>::instance ().detect_memory_leaks () ;
	Singleton<Reporter>::instance ().detect_crash_signal () ;
	Singleton<Console>::instance ().open () ;
	Singleton<Console>::instance ().link (slice (".")) ;
	
	test_main () ;

	return 0 ;
}
#endif