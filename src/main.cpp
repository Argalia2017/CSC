#include "util.h"

using namespace SOLUTION ;

class TestInt {
protected:
	int mInt ;

public:
	implicit TestInt () noexcept {
		static auto mStaticInt = 0 ;
		mInt = mStaticInt++ ;
		auto mFormat = Format (slice ("[$0] create in $1")) ;
		Singleton<Console>::instance ().info (mFormat (mInt ,QUAD (address (thiz)))) ;
	}

	implicit ~TestInt () noexcept {
		auto mFormat = Format (slice ("[$0] destroy in $1")) ;
		Singleton<Console>::instance ().info (mFormat (mInt ,QUAD (address (thiz)))) ;
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
	Singleton<Console>::instance ().start () ;
	
	return 0 ;
}