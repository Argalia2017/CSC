#include "util.h"

#include <csc_math.hpp.default.inl>

#ifdef __CSC_ARRAY__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_ARRAY) {
public:
	TEST_METHOD (TEST_CSC_ARRAY_STRING) {
		const auto r1x = String<STR> () ;
		_UNITTEST_ASSERT_ (r1x.size () == 0) ;
		const auto r2x = String<STR> (PTRTOARR[&_PCSTR_ ("")[0]]) ;
		_UNITTEST_ASSERT_ (r2x.size () == 0) ;
		const auto r3x = String<STR> (_PCSTR_ ("")) ;
		_UNITTEST_ASSERT_ (r3x.size () == 0) ;
	}

	TEST_METHOD (TEST_CSC_ARRAY_STACK) {
		auto rax = Stack<int> {1 ,2 ,4 ,5} ;
		rax.insert_sort (3) ;
		auto rbx = Array<INDEX> (rax.size ()) ;
		const auto r1x = rax.esort () ;
		for (INDEX i = 0 ; i < r1x.length () ; i++)
			rbx[i] = r1x[i] ;
		_UNITTEST_ASSERT_ (rbx[0] == 0) ;
		_UNITTEST_ASSERT_ (rbx[1] == 1) ;
		_UNITTEST_ASSERT_ (rbx[2] == 4) ;
		_UNITTEST_ASSERT_ (rbx[3] == 2) ;
		_UNITTEST_ASSERT_ (rbx[4] == 3) ;
	}

	TEST_METHOD (TEST_CSC_ARRAY_QUEUE) {
		auto rax = Queue<int> {1 ,2 ,3 ,4} ;
		rax.take () ;
		rax.add (5) ;
		rax.add (6) ;
		_UNITTEST_ASSERT_ (rax.length () == 5) ;
		_UNITTEST_ASSERT_ (rax[rax.access (0)] == 2) ;
		_UNITTEST_ASSERT_ (rax[rax.access (1)] == 3) ;
		_UNITTEST_ASSERT_ (rax[rax.access (2)] == 4) ;
		_UNITTEST_ASSERT_ (rax[rax.access (3)] == 5) ;
		_UNITTEST_ASSERT_ (rax[rax.access (4)] == 6) ;
	}

	TEST_METHOD (TEST_CSC_ARRAY_PRIORITY) {
		auto rax = Array<int> {1 ,2 ,3 ,1 ,2 ,5 ,7 ,5 ,3 ,2 ,4 ,5 ,6} ;
		auto rbx = Priority<int> (rax.length ()) ;
		for (auto &&i : rax)
			rbx.add (i) ;
		const auto r2x = rbx.esort () ;
		rax.sort () ;
		for (INDEX i = 0 ; i < rax.length () ; i++)
			_UNITTEST_ASSERT_ (rbx[r2x[i]] == rax[i]) ;
		INDEX ir = 0 ;
		while (TRUE) {
			if (rbx.empty ())
				break ;
			const auto r3x = rbx[rbx.peek ()].key ;
			rbx.take () ;
			_UNITTEST_ASSERT_ (r3x == rax[ir]) ;
			ir++ ;
		}
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

	TEST_METHOD (TEST_CSC_ARRAY_SET) {
		auto rax = Set<INDEX> (233) ;
		auto rbx = AutoRef<std::random_device>::make () ;
		while (TRUE) {
			if (rax.length () >= rax.size ())
				break ;
			const auto r1x = rbx.self () ;
			rax.add (r1x) ;
			for (auto &&i : rax) {
				INDEX ix = rax.find (i.key) ;
				_UNITTEST_ASSERT_ (ix == rax.at (i)) ;
			}
		}
		for (INDEX i = 0 ; i < rax.size () ; i++) {
			const auto r2x = rbx.self () ;
			INDEX ix = rax.find (r2x) ;
			if (ix == VAR_NONE)
				continue ;
			rax.remove (ix) ;
			for (auto &&j : rax) {
				INDEX jx = rax.find (j.key) ;
				_UNITTEST_ASSERT_ (jx == rax.at (j)) ;
			}
		}
	}

	TEST_METHOD (TEST_CSC_ARRAY_SOFTSET) {
		auto rax = SoftSet<INDEX> (233) ;
		auto rbx = AutoRef<std::random_device>::make () ;
		while (TRUE) {
			if (rax.length () >= rax.size ())
				break ;
			const auto r1x = rbx.self () ;
			rax.add (r1x) ;
			for (auto &&j : rax) {
				INDEX ix = rax.find (j.key) ;
				_UNITTEST_ASSERT_ (ix == rax.at (j)) ;
			}
		}
	}
} ;
} ;
#endif