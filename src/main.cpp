#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

using namespace SOLUTION ;

/*
1. Proxy
2. root_ptr
3. keep
4. FriendBinder
*/

int main () {
	for (auto &&i : range (0 ,1025)) {
		const auto r1x = Flt64 (i) * MathProc::inverse (Flt64 (1024)) ;
		const auto r2x = ColorProc::jet_from_norm (r1x) ;
		const auto r3x = ColorProc::hsv_from_bgr (r2x) ;
		const auto r5x = ColorProc::bgr_from_hsv (r3x) ;
		const auto r7x = ColorProc::norm_from_jet (r5x) ;
		const auto r8x = MathProc::abs (r1x - r7x) ;
        assert (r8x < 0.01) ;
	}
	return 0 ;
}