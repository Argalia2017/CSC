#include "util.h"

#include <csc_math.hpp.default.inl>

#ifdef __CSC_ARRAY__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_ARRAY) {
public:
	TEST_METHOD (TEST_CSC_ARRAY_STRING) {
		const auto r1x = String<STR> () ;
		_UNITTEST_ASSERT_ (r1x.size () == 0) ;
		const auto r2x = String<STR> (_PCSTR_ ("").self) ;
		_UNITTEST_ASSERT_ (r2x.size () == 0) ;
		const auto r3x = String<STR> (_PCSTR_ ("")) ;
		_UNITTEST_ASSERT_ (r3x.size () == 0) ;
	}

	TEST_METHOD (TEST_CSC_ARRAY_DEQUE) {
		auto rax = Deque<int> {1 ,2 ,3 ,4} ;
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
		auto rax = Deque<int> {1 ,2 ,3 ,1 ,2 ,5 ,7 ,5 ,3 ,2 ,4 ,5 ,6} ;
		auto rbx = Priority<int> (rax.length ()) ;
		for (auto &&i : rax)
			rbx.add (i) ;
		const auto r1x = rbx.range_sort () ;
		const auto r2x = rax.range_sort () ;
		for (auto &&i : _RANGE_ (0 ,r2x.length ()))
			_UNITTEST_ASSERT_ (rbx[r1x[i]] == rax[r2x[i]]) ;
		INDEX ir = 0 ;
		while (TRUE) {
			if (rbx.empty ())
				break ;
			const auto r3x = rbx[rbx.head ()].key ;
			rbx.take () ;
			const auto r4x = ir++ ;
			_UNITTEST_ASSERT_ (r3x == rax[r2x[r4x]]) ;
			(void) r4x ;
		}
	}

	TEST_METHOD (TEST_CSC_ARRAY_SLIST) {
		auto rax = SList<INDEX> (100000) ;
		for (auto &&i : _RANGE_ (0 ,rax.size ()))
			rax.add (i) ;
		rax.remove (1) ;
		rax.remove (10) ;
		rax.remove (98) ;
		rax.remove (100) ;
		rax.remove (101) ;
		rax.remove (1000) ;
		for (auto &&i : _RANGE_ (0 ,rax.length ())) {
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
				INDEX iy = rax.at (i) ;
				_UNITTEST_ASSERT_ (ix == iy) ;
			}
		}
		for (auto &&i : _RANGE_ (0 ,rax.size ())) {
			const auto r2x = rbx.self () ;
			INDEX ix = rax.find (r2x) ;
			if (ix == VAR_NONE)
				continue ;
			rax.remove (ix) ;
			for (auto &&j : rax) {
				INDEX jx = rax.find (j.key) ;
				INDEX jy = rax.at (j) ;
				_UNITTEST_ASSERT_ (jx == jy) ;
			}
			(void) i ;
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
				INDEX iy = rax.at (j) ;
				_UNITTEST_ASSERT_ (ix == iy) ;
			}
		}
	}
} ;
} ;
#endif