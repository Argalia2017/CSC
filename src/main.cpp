#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <csignal>
#include <csc_begin.h>

using namespace SOLUTION ;

inline Vector random_vector (CREF<Random> r) {
	Vector ret ;
	ret[0] = FLT64 (r.random_value (0 ,10000)) ;
	ret[1] = FLT64 (r.random_value (0 ,10000)) ;
	ret[2] = FLT64 (r.random_value (0 ,10000)) ;
	ret[3] = 0 ;
	ret = ret.normalize () ;
	return move (ret) ;
}

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	const auto r1x = CurrentRandom () ;
	for (auto &&t : iter (0 ,6)) {
		for (auto &&i : iter (0 ,100000)) {
			const auto r2x = random_vector (r1x) ;
			const auto r3x = FLT64 (r1x.random_value (-180 ,180)) * MATH_R ;
			const auto r4x = RotationMatrix (r2x ,r3x) ;
			const auto r5x = Quaternion (r4x).euler (t) ;
			assert (MathProc::abs (r5x.mPitch) <= MATH_PI / 2) ;
			const auto r6x = Quaternion (r5x).matrix () ;
			const auto r7x = r4x.transpose () * r6x ;
			for (auto &&j : iter (0 ,4 ,0 ,4)) {
				const auto r10x = LENGTH (j.mX == j.mY) ;
				assert (MathProc::abs (r7x[j] - r10x) < FLT32_EPS) ;
			}
		}
	}

	GlobalRoot::shutdown () ;
	return 0 ;
}