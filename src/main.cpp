#include "util.h"

#include <csc_end.h>
#define _CRT_SECURE_NO_WARNINGS
#include <initializer_list>
#include <cstdio>
#include <csc_begin.h>

using namespace ROUTINE ;

int test () ;

int main () {
	CLOG.show () ;
	CLOG.open (slice (".")) ;
	const auto r1x = CurrentRandom () ;
	auto rax = String<Stra>::make () ;
	for (auto &&i : range (0 ,100)) {
		const auto r2x = r1x.random_uniform (2) ;
		const auto r3x = r2x[0] / r2x[1] ;
		const auto r4x = String<Str>::make (r3x) ;
		std::sprintf (rax.ref ,"%.15g" ,r3x) ;
		const auto r5x = StringProc::strs_from (rax) ;
		const auto r6x = r4x.equal (r5x) ;
		if (r6x)
			continue ;
		CLOG.info (Format (slice ("[$1] eq [$2] : $3")) (r4x ,r5x ,r4x.equal (r5x))) ;
	}
	return 0 ;
}