#include "util.h"

#ifdef __CSC_STRING__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_STRING) {
public:
	TEST_METHOD (TEST_CSC_STRING) {
		const auto r1x = _PARSEBOOLS_ (String<STR> (_PCSTR_ ("TRUE"))) ;
		_UNITTEST_ASSERT_ (r1x) ;
		const auto r2x = _PARSEBOOLS_ (String<STR> (_PCSTR_ ("FALSE"))) ;
		_UNITTEST_ASSERT_ (!r2x) ;
		const auto r3x = _PARSEVARS_ (String<STR> (_PCSTR_ ("3332"))) ;
		_UNITTEST_ASSERT_ (r3x == 3332) ;
		const auto r4x = _PARSEVARS_ (String<STR> (_PCSTR_ ("-01"))) ;
		_UNITTEST_ASSERT_ (r4x == -1) ;
		const auto r5x = _PARSEVAL64S_ (String<STR> (_PCSTR_ ("0.0"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r5x) < VAL64 (1E-13)) ;
		const auto r6x = _PARSEVAL64S_ (String<STR> (_PCSTR_ ("1.0"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r6x - VAL64 (1.0)) < VAL64 (1E-13)) ;
		const auto r7x = _BUILDVAL64S_ (VAL64 (15.234999999999975)) ;
		_UNITTEST_ASSERT_ (r7x == _PCSTR_ ("15.235")) ;
		const auto r8x = _BUILDVAL64S_ (VAL64 (1234E20)) ;
		_UNITTEST_ASSERT_ (r8x == _PCSTR_ ("1.234e+23")) ;
		const auto r9x = _BUILDVAL64S_ (VAL64 (0)) ;
		_UNITTEST_ASSERT_ (r9x == _PCSTR_ ("0")) ;
		const auto r10x = _CAST_<VAL64> (_XVALUE_<DATA> (DATA (0X4027FFFFFFFFFFE1))) ;
		const auto r11x = _BUILDVAL64S_ (r10x) ;
		_UNITTEST_ASSERT_ (r11x.length () >= 2 && r11x.segment (0 ,2) == _PCSTR_ ("12")) ;
		const auto r12x = _CAST_<VAL64> (_XVALUE_<DATA> (DATA (0X3F9986338B47C730))) ;
		const auto r13x = _BUILDVAL64S_ (r12x) ;
		_UNITTEST_ASSERT_ (r13x.length () >= 6 && r13x.segment (0 ,6) == _PCSTR_ ("0.0249")) ;
		const auto r14x = _PARSEVAL64S_ (String<STR> (_PCSTR_ ("-0.0122"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r14x - VAL64 (-0.0122)) < VAL64 (1E-4)) ;
		const auto r15x = _PARSEVAL64S_ (String<STR> (_PCSTR_ ("1.42E38"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r15x - VAL64 (1.42E38)) < VAL64 (1E34)) ;
		const auto r16x = _PARSEVAL64S_ (String<STR> (_PCSTR_ ("0.99287"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r16x - VAL64 (0.99287)) < VAL64 (1E-4)) ;
		const auto r17x = _BUILDDATES_ (GlobalRuntime::clock_now ()) ;
		_UNITTEST_ASSERT_ (r17x.size () > 0) ;
		const auto r18x = _BUILDHOURS_ (GlobalRuntime::clock_now ()) ;
		_UNITTEST_ASSERT_ (r18x.size () > 0) ;
		const auto r19x = _BUILDTIMES_ (GlobalRuntime::clock_now ()) ;
		_UNITTEST_ASSERT_ (r19x.size () > 0) ;
	}

#ifdef __CSC_COMPILER_MSVC__
	TEST_METHOD (TEST_CSC_STRING_OPERATOR) {
		const auto r1x = _U8STOU16S_ (String<STRU8> (_PCSTRU8_ ("this 100-\t≤‚ ‘£°x"))) ;
		_UNITTEST_ASSERT_ (r1x == String<STRU16> (_PCSTRU16_ ("this 100-\t≤‚ ‘£°x"))) ;
	}
#endif

	TEST_METHOD (TEST_CSC_STRING_REGEXMATCHER) {
		auto rax = String<STRU8> (_PCSTRU8_ ("this is a test!")) ;
		const auto r1x = RegexMatcher (_PCSTRU8_ ("test")) ;
		const auto r2x = r1x.search (rax) ;
		_UNITTEST_ASSERT_ (r2x.length () == 1) ;
		_UNITTEST_ASSERT_ (r2x[r2x.head ()][0] == 10) ;
		_UNITTEST_ASSERT_ (r2x[r2x.head ()][1] == 14) ;
		rax = r1x.replace (rax ,_PCSTRU8_ ("unittest")) ;
		_UNITTEST_ASSERT_ (rax == String<STRU8> (_PCSTRU8_ ("this is a unittest!"))) ;
	}
} ;
} ;
#endif

#ifdef __CSC_STRING__
#include "csc_string.hpp.default.inl"
#endif