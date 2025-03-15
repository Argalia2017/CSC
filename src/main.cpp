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

	Singleton<Console>::instance ().info (slice ("r1x = {")) ;
	for (auto &&i : r1x) {
		Singleton<Console>::instance ().info (Format (slice ("\t\"$1\"")) (i)) ;
	}
	Singleton<Console>::instance ().info (slice ("}")) ;
	Singleton<Console>::instance ().info (slice ("r2x = {")) ;
	for (auto &&i : r2x) {
		Singleton<Console>::instance ().info (Format (slice ("\t\"$1\"")) (i)) ;
	}
	Singleton<Console>::instance ().info (slice ("}")) ;
	Singleton<Console>::instance ().info (slice ("r3x = {")) ;
	for (auto &&i : r3x) {
		Singleton<Console>::instance ().info (Format (slice ("\t\"$1\"")) (i)) ;
	}
	Singleton<Console>::instance ().info (slice ("}")) ;
	Singleton<Console>::instance ().info (slice ("r4x = {")) ;
	for (auto &&i : r4x) {
		Singleton<Console>::instance ().info (Format (slice ("\t\"$1\"")) (i)) ;
	}
	Singleton<Console>::instance ().info (slice ("}")) ;

	GlobalProc::shutdown () ;
	return 0 ;
}