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
	try {
		assume (argc >= 3) ;
		const auto r1x = String<Str> (Slice (Flag (argv[1]) ,SLICE_MAX_SIZE::expr ,1).eos ()) ;
		const auto r2x = String<Str> (Slice (Flag (argv[2]) ,SLICE_MAX_SIZE::expr ,1).eos ()) ;
		const auto r3x = Image<Color3B> (ImageProc::load_image (r1x)) ;
		ImageProc::save_image (r2x ,r3x) ;
	} catch (CR<Exception> e) {
		CLOG.error (slice ("ERROR:")) ;
		CLOG.error (Format (slice ("assume $1 at $2 in $3 ,$4")) (e.what () ,e.func () ,e.file () ,e.line ())) ;
	}
	return 0 ;
}