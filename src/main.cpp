#include "util.h"

#include <csc_end.h>
#define _CRT_SECURE_NO_WARNINGS
#include <initializer_list>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <csc_begin.h>

using namespace ROUTINE ;

/*
1. Proxy
2. root_ptr
3. keep
5. PointCloud
*/

int main (int argc ,char **argv) {
	CLOG.show () ;
	const auto r1x = Path ().absolute ().fetch () ;
	const auto r2x = Path (slice ("aa")).absolute ().fetch () ;
	const auto r3x = Path (slice ("/aa")).absolute ().fetch () ;
	const auto r4x = Path (slice ("./aa")).absolute ().fetch () ;
	notice (r1x) ;
	notice (r2x) ;
	notice (r3x) ;
	notice (r4x) ;

	return 0 ;
}