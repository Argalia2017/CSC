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
		const auto r5x = _PARSEVALS_ (String<STR> (_PCSTR_ ("-0.0122"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r5x - VAL (-0.0122)) < VAL (1E-4)) ;
		const auto r6x = _PARSEVALS_ (String<STR> (_PCSTR_ ("1.42E38"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r6x - VAL (1.42E38)) < VAL (1E34)) ;
		const auto r7x = _PARSEVALS_ (String<STR> (_PCSTR_ ("0.99287"))) ;
		_UNITTEST_ASSERT_ (_ABS_ (r7x - VAL (0.99287)) < VAL (1E-4)) ;
		const auto r8x = _BUILDDATES_<STR> (std::chrono::system_clock::now ()) ;
		_UNITTEST_ASSERT_ (r8x.size () > 0) ;
		const auto r9x = _BUILDHOURS_<STR> (std::chrono::system_clock::now ()) ;
		_UNITTEST_ASSERT_ (r9x.size () > 0) ;
		const auto r10x = _BUILDTIMES_<STR> (std::chrono::system_clock::now ()) ;
		_UNITTEST_ASSERT_ (r10x.size () > 0) ;
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
		const auto r3x = r1x.search (rax) ;
		_UNITTEST_ASSERT_ (r3x.length () == 1) ;
		_UNITTEST_ASSERT_ (r3x[r3x.peek ()][0] == 10 && r3x[r3x.peek ()][1] == 14) ;
		rax = r1x.replace (rax ,_PCSTRU8_ ("unittest")) ;
		_UNITTEST_ASSERT_ (rax == String<STRU8> (_PCSTRU8_ ("this is a unittest!"))) ;
	}
} ;
} ;
#endif

#ifdef __CSC_STRING__
#include "csc_string.hpp.default.inl"
#endif