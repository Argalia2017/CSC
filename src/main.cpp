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

	const auto r1x = 1024 * 1024 * 1024 ;
	const auto r2x = RuntimeProc::thread_concurrency () ;
	const auto r3x = invoke ([&] () {
		Array<Random> ret = Array<Random> (r2x) ;
		for (auto &&i : ret.range ()) {
			ret[i] = CurrentRandom () ;
		}
		return move (ret) ;
	}) ;
	auto rax = Array<Point3F> (r1x) ;
	auto rbx = WorkThread (NULL) ;
	auto rcx = Atomic (NULL) ;
	const auto r10x = CurrentTime () ;
	const auto r5x = rax.size () ;
	rcx.store (0) ;
#pragma omp parallel for
	for (int i = 0 ; i < r5x ; i++) {
		thread_local auto r4x = invoke ([&] () {
			LENGTH ret = 0 ;
			while (TRUE) {
				const auto r1x = rcx.change (ret ,ret + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
			return move (ret) ;
		}) ;
		rax[i].mX = r3x[r4x].random_float (10000) ;
		rax[i].mY = r3x[r4x].random_float (10000) ;
		rax[i].mZ = r3x[r4x].random_float (10000) ;
	}
	const auto r11x = CurrentTime () ;
	rbx.set_thread_size (r2x) ;
	rcx.store (0) ;
	rbx.start ([&] (CREF<INDEX> index) {
		thread_local auto r4x = invoke ([&] () {
			LENGTH ret = 0 ;
			while (TRUE) {
				const auto r1x = rcx.change (ret ,ret + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
			return move (ret) ;
		}) ;
		rax[index].mX = r3x[r4x].random_float (10000) ;
		rax[index].mY = r3x[r4x].random_float (10000) ;
		rax[index].mZ = r3x[r4x].random_float (10000) ;
	}) ;
	rbx.post (0 ,rax.size ()) ;
	rbx.join () ;
	const auto r12x = CurrentTime () ;
	const auto omp_time = (r11x - r10x).milliseconds () ;
	const auto mwk_time = (r12x - r11x).milliseconds () ;
	watch (rax) ;
	watch (rbx) ;
	watch (omp_time) ;
	watch (mwk_time) ;

	GlobalProc::shutdown () ;
	return 0 ;
}