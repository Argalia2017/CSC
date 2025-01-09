#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <csc_begin.h>

using namespace SOLUTION ;

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;

	auto rax = Jet<RANK2> (0 ,0).spow (2).ssub (Jet<RANK2> (0 ,1)) ;
	auto rdx = CurrentRandom () ;
	auto rex = Array<Tuple<FLT64 ,FLT64>> (10000) ;
	for (auto &&i : rex.range ()) {
		auto rbx = FLT64 (rdx.random_value (1 * 10000 ,100 * 10000)) / 10000 ;
		auto rcx = rbx ;
		while (TRUE) {
			assert (!MathProc::is_inf (rcx)) ;
			rax (rcx ,rbx) ;
			if (rax.ex () != 0)
				break ;
			if (MathProc::abs (rax.fx ()) < FLT32_EPS)
				break ;
			assert (!MathProc::is_low (rax.dx (0))) ;
			rcx -= rax.fx () / rax.dx (0) ;
		}
		assume (rax.ex () <= 0) ;
		rex[i].m1st = rbx ;
		rex[i].m2nd = rcx ;
	}
	watch (rex) ;
	watch (rex) ;

	GlobalRoot::shutdown () ;
	return 0 ;
}