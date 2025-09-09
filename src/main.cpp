#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

int main () {
	Singleton<Console>::expr.open (slice (".")) ;
	Singleton<Console>::expr.show () ;
	Singleton<Console>::expr.debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	Scope<ConfigProc> anonymous (ConfigProc::expr) ;

	const auto r1x = CurrentRandom () ;
	for (auto &&i : range (0 ,10000)) {
		const auto r2x = r1x.random_float (10000) ;
		const auto r3x = r1x.random_float (10000) ;
		const auto r4x = r1x.random_float (10000) ;
		const auto r5x = r1x.random_float (10000) ;
		const auto r6x = r1x.random_float (10000) ;
		const auto r7x = r1x.random_float (10000) ;
		const auto r8x = Vector (0 ,0 ,0 ,0).normalize () ;
		const auto r9x = Vector (0 ,0 ,0 ,0).normalize () ;
		const auto r10x = RotationMatrix (r8x ,r9x) ;
		const auto r11x = r10x * r8x ;
		const auto r12x = (r11x - r9x).magnitude () ;
		assume (r12x < 1E-10) ;
	}

	return 0 ;
}