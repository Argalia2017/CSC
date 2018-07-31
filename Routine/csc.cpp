#include "util.h"

#ifdef __CSC__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC) {
public:
	TEST_METHOD (TEST_CSC) {
		_UNITTEST_ASSERT_ (_COPY_ (EXPANDING_SIZE::value) != 0) ;
		_UNITTEST_ASSERT_ (_COPY_ (RECURSIVE_SIZE::value) != 0) ;
		_UNITTEST_ASSERT_ (_COPY_ (SHORTSTRING_SIZE::value) != 0) ;
		_UNITTEST_ASSERT_ (_COPY_ (LONGSTRING_SIZE::value) != 0) ;
		_UNITTEST_ASSERT_ (_COPY_ (HUGESTRING_SIZE::value) != 0) ;
	}

	TEST_METHOD (TEST_CSC_STD) {
		auto rax = Buffer<int ,ARGC<8>> () ;
		for (INDEX i = 0 ; i < rax.size () ; i++)
			rax[i] = int (i) ;
		_MEMCROL_ (rax.self ,rax.size () ,4 ,3) ;
		_UNITTEST_ASSERT_ (rax[0] == 4 && rax[1] == 5 && rax[2] == 6 && rax[3] == 7) ;
		_UNITTEST_ASSERT_ (rax[4] == 0 && rax[5] == 1 && rax[6] == 2 && rax[7] == 3) ;
		_MEMCROL_ (&rax[4] ,4 ,2 ,1) ;
		_UNITTEST_ASSERT_ (rax[4] == 2 && rax[5] == 3 && rax[6] == 0 && rax[7] == 1) ;
	}
} ;
} ;
#endif