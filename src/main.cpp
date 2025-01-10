#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <csignal>
#include <csc_begin.h>

using namespace SOLUTION ;

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	for (auto &&t : iter (0 ,100)) {
		const auto r1x = CurrentRandom () ;
		const auto r2x = MathProc::inverse (FLT64 (10000)) ;
		const auto r3x = invoke ([&] () {
			Array<Point2F> ret = Array<Point2F> (10000) ;
			for (auto &&i : ret.range ()) {
				const auto r4x = r1x.random_value (0 ,20000) - 10000 ;
				const auto r5x = r1x.random_value (0 ,20000) - 10000 ;
				ret[i].mX = FLT32 (FLT64 (r4x) * r2x) ;
				ret[i].mY = FLT32 (FLT64 (r5x) * r2x) ;
			}
			return move (ret) ;
		}) ;
		const auto r6x = PointCloud (Ref<Array<Point2F>>::reference (r3x)) ;
		const auto r7x = invoke ([&] () {
			Vector ret = Vector::axis_w () ;
			const auto r8x = r1x.random_value (0 ,20000) - 10000 ;
			const auto r9x = r1x.random_value (0 ,20000) - 10000 ;
			ret[0] = FLT64 (r8x) * r2x ;
			ret[1] = FLT64 (r9x) * r2x ;
			return move (ret) ;
		}) ;
		const auto r10x = r6x.search (r7x ,10) ;
		Singleton<Console>::instance ().info (slice ("KNN-search.length = ") ,r10x.length ()) ;
		for (auto &&i : r10x.range ()) {
			const auto r11x = (r6x[r10x[i]] - r7x).magnitude () ;
			Singleton<Console>::instance ().debug (Format (slice ("[$1].distance = $2")) (i ,r11x)) ;
		}
		const auto r12x = (r6x[r10x[r10x.length () - 1]] - r7x).magnitude () + FLT32_EPS ;
		const auto r13x = r6x.search (r7x ,100 ,r12x) ;
		Singleton<Console>::instance ().info (slice ("RAN-search.length = ") ,r13x.length ()) ;
		for (auto &&i : r13x.range ()) {
			const auto r14x = (r6x[r13x[i]] - r7x).magnitude () ;
			Singleton<Console>::instance ().debug (Format (slice ("[$1].distance = $2")) (i ,r14x)) ;
		}
		assert (r13x.length () == r10x.length ()) ;
	}

	GlobalRoot::shutdown () ;
	return 0 ;
}