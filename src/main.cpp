#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	const auto r1x = Path (slice ("")).decouple () ;
	const auto r2x = Path (slice ("D:")).decouple () ;
	const auto r3x = Path (slice ("\\A")).decouple () ;
	const auto r4x = Path (slice ("B")).decouple () ;
	watch (r1x) ;
	watch (r2x) ;
	watch (r3x) ;
	watch (r4x) ;

	GlobalProc::shutdown () ;
	return 0 ;
}