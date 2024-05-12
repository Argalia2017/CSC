#include "util.h"

#include "csc/csc_end.h"
#include <iostream>
#include "csc/csc_begin.h"

using namespace std ;
using namespace SOLUTION ;

struct TestInt {
	int mInt ;

public:
	implicit TestInt () noexcept {
		static auto mStaticInt = 0 ;
		mInt = mStaticInt++ ;
		cerr << "[" << mInt << "]create" << "\n" ;
	}

	implicit ~TestInt () noexcept {
		cerr << "[" << mInt << "]destroy" << "\n" ;
	}

	implicit TestInt (CREF<TestInt> that) = delete ;

	forceinline VREF<TestInt> operator= (CREF<TestInt> that) = delete ;

	implicit TestInt (RREF<TestInt> that) noexcept {
		mInt = that.mInt ;
		that.mInt = -1 ;
	}

	forceinline VREF<TestInt> operator= (RREF<TestInt> that) noexcept {
		return CSC::assign (thiz ,that) ;
	}
} ;

exports int main () {
	auto rax = Priority<int> () ;
	INDEX ix = 0 ;
	INDEX iy = 0 ;
	for (auto &&i : iter (0 ,1000)) {
		const auto r1x = rand () % 2 ;
		if (r1x == 0)
			continue ;
		rax.add (int (i) ,ix) ;
		ix++ ;
	}
	ix = 0 ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		rax.get (rax.head () ,iy) ;
		assume (ix == iy) ;
		rax.take () ;
		ix++ ;
	}
	return 0 ;
}