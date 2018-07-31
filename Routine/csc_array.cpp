#include "util.h"

#ifdef __CSC_ARRAY__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_ARRAY) {
public:
	TEST_METHOD (TEST_CSC_ARRAY) {

	}

	TEST_METHOD (TEST_CSC_ARRAY_STACK) {
		auto rax = Stack<int> {1 ,2 ,3 ,4} ;
		auto rbx = Array<INDEX> (rax.size ()) ;
		INDEX iw = 0 ;
		for (INDEX i = rax.ibegin () ; i != rax.iend () ; i = rax.inext (i))
			rbx[iw++] = i ;
		rbx[0] = 4 ;
		rbx[1] = 3 ;
		rbx[2] = 2 ;
		rbx[3] = 1 ;
	}

	TEST_METHOD (TEST_CSC_ARRAY_SLIST) {
		auto rax = SList<int> {1 ,2 ,3 ,4} ;
		auto rbx = Array<INDEX> (rax.size ()) ;
		INDEX iw = 0 ;
		for (INDEX i = rax.ibegin () ; i != rax.iend () ; i = rax.inext (i))
			rbx[iw++] = i ;
		rbx[0] = 4 ;
		rbx[1] = 3 ;
		rbx[2] = 2 ;
		rbx[3] = 1 ;
	}

	TEST_METHOD (TEST_CSC_ARRAY_DEQUE) {
		auto rax = Deque<INDEX> (100000) ;
		for (INDEX i = 0 ; i < rax.size () ; i++)
			rax.add (i) ;
		rax.remove (1) ;
		rax.remove (10) ;
		rax.remove (98) ;
		rax.remove (100) ;
		rax.remove (101) ;
		rax.remove (1000) ;
		for (INDEX i = 0 ; i < rax.length () ; i++) {
			INDEX ix = rax[rax.access (i)] ;
			ix -= EFLAG (ix > 1000) ;
			ix -= EFLAG (ix > 101) ;
			ix -= EFLAG (ix > 100) ;
			ix -= EFLAG (ix > 98) ;
			ix -= EFLAG (ix > 10) ;
			ix -= EFLAG (ix > 1) ;
			_UNITTEST_ASSERT_ (ix == i) ;
		}
	}
} ;
} ;
#endif