#include "util.h"

#include "csc/csc_end.h"
#include <vector>
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
		cerr << "[" << mInt << "]create in " << address (thiz) << "\n" ;
	}

	implicit ~TestInt () noexcept {
		cerr << "[" << mInt << "]destroy in " << address (thiz) << "\n" ;
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
	if (true) {
		auto rbx = std::vector<double> (1000000) ;
		auto rcx = Array<char> (10) ;
		auto rax = FarBuffer<double> (rbx.size ()) ;
		rax.use_setter ([&] (CREF<INDEX> index ,VREF<double> item) {
			rbx[index] = item ;
		}) ;
		rax[0] = 2 ;
		rax[1] = 3 ;
		rax[2] = 4 ;
		assert (FALSE) ;
	}
	return 0 ;
}