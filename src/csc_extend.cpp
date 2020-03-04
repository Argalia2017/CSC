#include "util.h"

namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_EXTEND) {
public:
	TEST_METHOD (TEST_CSC_EXTEND) {
		_UNITTEST_ASSERT_ (_ANYOF_ (1 ,2 ,3) == 1) ;
		_UNITTEST_ASSERT_ (_ANYOF_ (1 ,2 ,3) != 1) ;
		_UNITTEST_ASSERT_ (!(_ALLOF_ (1 ,2 ,3) == 1)) ;
		_UNITTEST_ASSERT_ (!(_ALLOF_ (1 ,2 ,3) != 1)) ;
	}

	TEST_METHOD (TEST_CSC_EXTEND_VAR128) {
		const auto r1x = VAR128 (VAR64_MAX) * 2 - VAR64_MAX ;
		_UNITTEST_ASSERT_ (r1x == VAR64_MAX) ;
		const auto r2x = VAR128 (VAR64_MAX) / (VAR64_MAX / 2 - 1) ;
		_UNITTEST_ASSERT_ (r2x == 2) ;
		_UNITTEST_ASSERT_ (VAR128 (22) < VAR128 (33)) ;
		_UNITTEST_ASSERT_ (VAR128 (-22) >= VAR128 (-33)) ;
	}

	TEST_METHOD (TEST_CSC_EXTEND_VARIANT) {
		auto rax = Optional<AutoBuffer<int>> () ;
		rax = AutoBuffer<int> (8) ;
		const auto r1x = rax.self.size () ;
		_UNITTEST_ASSERT_ (r1x == 8) ;
	}

	class CLASS_A :public Interface {
	public:
		void test1 () {}
		void test2 () const {}
		virtual void test3 () const {}
		static void test4 () {}
	} ;

	TEST_METHOD (TEST_CSC_EXTEND_FUNCTION) {
		auto rax = AutoRef<CLASS_A>::make () ;
		_CALL_ (Function<DEF<void ()> NONE::*> (PhanRef<CLASS_A>::make (rax) ,&CLASS_A::test1)) ;
		_CALL_ (Function<DEF<void ()> NONE::*> (PhanRef<const CLASS_A>::make (rax) ,&CLASS_A::test2)) ;
		_CALL_ (Function<DEF<void ()> NONE::*> (PhanRef<const CLASS_A>::make (rax) ,&CLASS_A::test3)) ;
		_CALL_ (Function<void ()> (&CLASS_A::test4)) ;
		const auto r1x = _XVALUE_<PTR<int (const int & ,const int &)>> ([] (const int &x ,const int &y) {
			if (y == 1)
				return x ;
			if (y == 2)
				return _SQE_ (x) ;
			return 0 ;
		}) ;
		const auto r2x = Function<int (const int &)>::make (r1x ,int (2)) ;
		_UNITTEST_ASSERT_ (r2x (2) == 4) ;
		_UNITTEST_ASSERT_ (r2x (3) == 9) ;
		const auto r3x = Function<int (const int & ,const int &)>::make (r1x) ;
		_UNITTEST_ASSERT_ (r3x (1 ,1) == 1) ;
	}

	class CLASS_XA :public Interface {
	public:
		virtual int func1 () const = 0 ;
		virtual int func2 () const = 0 ;
	} ;

	class CLASS_XB :public CLASS_XA {
	public:
		int func1 () const override {
			return 1 ;
		}

		int func2 () const override {
			return 1 ;
		}
	} ;

	class CLASS_XC :public CLASS_XA {
	public:
		int func1 () const override {
			return 2 ;
		}

		int func2 () const override {
			return 2 ;
		}
	} ;

	class CLASS_XD :public CLASS_XB ,public CLASS_XC {
	public:
		int func1 () const override {
			return 3 ;
		}
	} ;

	TEST_METHOD (TEST_CSC_EXTEND_STRONGREF) {
		const auto r1x = StrongRef<CLASS_XD>::make () ;
		const auto r2x = r1x.recast<CLASS_XB> () ;
		const auto r3x = r1x.recast<CLASS_XC> () ;
		const auto r4x = r2x.recast<CLASS_XA> () ;
		const auto r5x = r3x.recast<CLASS_XA> () ;
		_UNITTEST_ASSERT_ (_ADDRESS_ (&r4x.self) != _ADDRESS_ (&r5x.self)) ;
		_UNITTEST_ASSERT_ (r4x == r5x) ;
		_UNITTEST_ASSERT_ (r4x->func1 () == 3) ;
		_UNITTEST_ASSERT_ (r5x->func1 () == 3) ;
		_UNITTEST_ASSERT_ (r4x->func2 () == 1) ;
		_UNITTEST_ASSERT_ (r5x->func2 () == 2) ;
		const auto r6x = r2x.recast<CLASS_XC> () ;
		_UNITTEST_ASSERT_ (_ADDRESS_ (&r3x.self) == _ADDRESS_ (&r6x.self)) ;
	}

	TEST_METHOD (TEST_CSC_EXTEND_MEMORYPOOL) {
		auto rax = AutoRef<MemoryPool>::make () ;
		const auto r1x = rax->alloc<int> () ;
		const auto r2x = LENGTH (8) ;
		const auto r3x = _ALIGNAS_ ((_MAX_ (_ALIGNOF_ (int) - r2x ,VAR_ZERO) + _SIZEOF_ (int)) ,r2x) ;
		_UNITTEST_ASSERT_ (rax->length () == r3x) ;
		rax->alloc<TEMP<UniqueRef<void>>> () ;
		const auto r4x = _ALIGNAS_ ((_MAX_ (_ALIGNOF_ (TEMP<UniqueRef<void>>) - r2x ,VAR_ZERO) - 1 + _SIZEOF_ (TEMP<UniqueRef<void>>)) ,r2x) ;
		_UNITTEST_ASSERT_ (rax->length () == r3x + r4x) ;
		rax->alloc<double> (43) ;
		const auto r5x = _ALIGNAS_ ((_MAX_ (_ALIGNOF_ (double) - r2x ,VAR_ZERO) + 43 * _SIZEOF_ (double)) ,r2x) ;
		_UNITTEST_ASSERT_ (rax->length () == r3x + r4x + r5x) ;
		rax->clean () ;
		_UNITTEST_ASSERT_ (rax->length () == r3x + r4x + r5x) ;
		rax->free (r1x) ;
		_UNITTEST_ASSERT_ (rax->length () == r4x + r5x) ;
		const auto r6x = rax->alloc<PACK<VAR128[3]>> (1) ;
		_UNITTEST_ASSERT_ (_ADDRESS_ (r6x) % _ALIGNOF_ (PACK<VAR128[3]>) == 0) ;
		rax->free (r6x) ;
	}

#ifdef __CSC_TARGET_DLL__
	TEST_METHOD (TEST_CSC_EXTEND_GLOBALSTATIC) {
		using UUID1 = ARGC<+1> ;
		using UUID2 = ARGC<+2> ;
		using UUID3 = ARGC<+3> ;
		using UUID4 = ARGC<+4> ;
		GlobalStatic<UUID1>::save (1) ;
		GlobalStatic<UUID2>::save (2) ;
		const auto r1x = GlobalStatic<UUID2>::load () ;
		_UNITTEST_ASSERT_ (r1x == 2) ;
		auto rax = FALSE ;
		_CALL_TRY_ ([&] () {
			GlobalStatic<UUID3>::load () ;
			rax = TRUE ;
		} ,[&] () {
			rax = FALSE ;
		}) ;
		_UNITTEST_ASSERT_ (!rax) ;
		GlobalStatic<UUID4>::save (4) ;
		GlobalStatic<UUID4>::save (-4) ;
		const auto r2x = GlobalStatic<UUID1>::load () ;
		_UNITTEST_ASSERT_ (r2x == 1) ;
	}
#endif

	class CLASS_WRAPPED_String_STRU8 :private Wrapped<String<STRU8>> {
	public:
		inline void visit (const int &stru) {
			CLASS_WRAPPED_String_STRU8::mSelf += _BUILDVAR32S_<STRU8> (stru) ;
		}

		inline void visit (const float &stru) {
			CLASS_WRAPPED_String_STRU8::mSelf += _BUILDVAL32S_<STRU8> (stru) ;
		}
	} ;

	TEST_METHOD (TEST_CSC_EXTEND_SERIALIZER) {
		const auto r1x = PACK<int ,float> {1 ,2.1f} ;
		const auto r2x = Serializer<CLASS_WRAPPED_String_STRU8 ,const PACK<int ,float>> (&PACK<int ,float>::P1 ,&PACK<int ,float>::P2) ;
		auto rax = String<STRU8> () ;
		r2x (r1x).friend_visit (_CAST_<CLASS_WRAPPED_String_STRU8> (rax)) ;
		_UNITTEST_ASSERT_ (rax == String<STRU8> (_PCSTRU8_ ("12.1"))) ;
	}
} ;
} ;