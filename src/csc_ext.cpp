#include "util.h"

#ifdef __CSC_EXT__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_EXT) {
public:
	TEST_METHOD (TEST_CSC_EXT) {
		_UNITTEST_ASSERT_ (_ANYOF_ (1 ,2 ,3) == 1) ;
		_UNITTEST_ASSERT_ (_ANYOF_ (1 ,2 ,3) != 1) ;
		_UNITTEST_ASSERT_ (!(_ALLOF_ (1 ,2 ,3) == 1)) ;
		_UNITTEST_ASSERT_ (!(_ALLOF_ (1 ,2 ,3) != 1)) ;
	}

	TEST_METHOD (TEST_CSC_EXT_VAR128) {
		const auto r1x = VAR128 (VAR64_MAX) * 2 - VAR64_MAX ;
		_UNITTEST_ASSERT_ (r1x == VAR64_MAX) ;
		const auto r2x = VAR128 (VAR64_MAX) / (VAR64_MAX / 2 - 1) ;
		_UNITTEST_ASSERT_ (r2x == 2) ;
		_UNITTEST_ASSERT_ (VAR128 (22) < VAR128 (33)) ;
		_UNITTEST_ASSERT_ (VAR128 (-22) >= VAR128 (-33)) ;
	}

	TEST_METHOD (TEST_CSC_EXT_VARIANT) {
		auto rax = Optional<AutoBuffer<int>> () ;
		rax = AutoBuffer<int> (8) ;
		const auto r1x = rax.self.size () ;
		_UNITTEST_ASSERT_ (r1x == 8) ;
	}

	TEST_METHOD (TEST_CSC_EXT_FUNCTION) {
		struct A :public Interface {
			void test1 () {}
			void test2 () const {}
			virtual void test3 () const {}
			static void test4 () {}
		} ;
		auto rax = AutoRef<A>::make () ;
		_CALL_ (Function<DEF<void ()> NONE::*> (PhanRef<A>::make (rax) ,&A::test1)) ;
		_CALL_ (Function<DEF<void ()> NONE::*> (PhanRef<const A>::make (rax) ,&A::test2)) ;
		_CALL_ (Function<DEF<void ()> NONE::*> (PhanRef<const A>::make (rax) ,&A::test3)) ;
		_CALL_ (Function<void ()> (&A::test4)) ;
		const auto r1x = _XVALUE_<PTR<int (const int & ,const int &)>> ([] (const int &arg1 ,const int &arg2) {
			if (arg2 == 1)
				return arg1 ;
			if (arg2 == 2)
				return _SQE_ (arg1) ;
			return 0 ;
		}) ;
		const auto r2x = Function<int (const int &)>::make (r1x ,_XVALUE_<int> (2)) ;
		_UNITTEST_ASSERT_ (r2x (2) == 4) ;
		_UNITTEST_ASSERT_ (r2x (3) == 9) ;
		const auto r3x = Function<int (const int & ,const int &)>::make (r1x) ;
		_UNITTEST_ASSERT_ (r3x (1 ,1) == 1) ;
	}

	TEST_METHOD (TEST_CSC_EXT_STRONGREF) {
		struct A :public Interface {
			virtual int func1 () const = 0 ;
			virtual int func2 () const = 0 ;
		} ;
		struct B :public A {
			int func1 () const override {
				return 1 ;
			}
			int func2 () const override {
				return 1 ;
			}
		} ;
		struct C :public A {
			int func1 () const override {
				return 2 ;
			}
			int func2 () const override {
				return 2 ;
			}
		} ;
		struct D :public B ,public C {
			int func1 () const override {
				return 3 ;
			}
		} ;
		const auto r1x = StrongRef<D>::make () ;
		const auto r2x = r1x.recast<B> () ;
		const auto r3x = r1x.recast<C> () ;
		const auto r4x = r2x.recast<A> () ;
		const auto r5x = r3x.recast<A> () ;
		_UNITTEST_ASSERT_ (_ADDRESS_ (&r4x.self) != _ADDRESS_ (&r5x.self)) ;
		_UNITTEST_ASSERT_ (r4x == r5x) ;
		_UNITTEST_ASSERT_ (r4x->func1 () == 3) ;
		_UNITTEST_ASSERT_ (r5x->func1 () == 3) ;
		_UNITTEST_ASSERT_ (r4x->func2 () == 1) ;
		_UNITTEST_ASSERT_ (r5x->func2 () == 2) ;
		const auto r6x = r2x.recast<C> () ;
		_UNITTEST_ASSERT_ (_ADDRESS_ (&r3x.self) == _ADDRESS_ (&r6x.self)) ;
	}

	TEST_METHOD (TEST_CSC_EXT_MEMORYPOOL) {
		auto rax = AutoRef<MemoryPool>::make () ;
		const auto r2x = rax->alloc<int> () ;
		const auto r10x = _XVALUE_<PTR<VAR (const VAR & ,const VAR &)>> ([] (const VAR &arg1 ,const VAR &arg2) {
			_DEBUG_ASSERT_ (arg2 != VAR_ZERO) ;
			const auto r4x = _CALL_ ([&] () {
				if (arg2 < VAR_ZERO)
					return -arg1 ;
				return arg1 ;
			}) ;
			const auto r5x = _ABS_ (arg2) ;
			VAR ret = VAR (r5x * VAR (r4x / r5x)) ;
			for (FOR_ONCE_DO) {
				if (r4x >= 0)
					discard ;
				if (r4x >= ret)
					discard ;
				ret -= r5x ;
			}
			if (arg2 < 0)
				ret = -ret ;
			return std::move (ret) ;
		}) ;
		const auto r11x = r10x (_SIZEOF_ (int) ,LENGTH (-8)) + _MAX_ (_ALIGNOF_ (int) - 8 ,VAR_ZERO) ;
		const auto r12x = r10x (_SIZEOF_ (TEMP<UniqueRef<void>>) ,LENGTH (-8)) + _MAX_ (_ALIGNOF_ (TEMP<UniqueRef<void>>) - 8 ,VAR_ZERO) ;
		const auto r13x = r10x (_SIZEOF_ (DEF<double[44]>) ,LENGTH (-8)) + _MAX_ (_ALIGNOF_ (DEF<double[44]>) - 8 ,VAR_ZERO) ;
		_UNITTEST_ASSERT_ (rax->length () == r11x) ;
		rax->alloc<TEMP<UniqueRef<void>>> () ;
		_UNITTEST_ASSERT_ (rax->length () == r11x + r12x) ;
		rax->alloc<double> (44) ;
		_UNITTEST_ASSERT_ (rax->length () == r11x + r12x + r13x) ;
		rax->clean () ;
		_UNITTEST_ASSERT_ (rax->length () == r11x + r12x + r13x) ;
		rax->free (r2x) ;
		_UNITTEST_ASSERT_ (rax->length () == r12x + r13x) ;
		const auto r3x = rax->alloc<PACK<VAR128[3]>> (1) ;
		_UNITTEST_ASSERT_ (_ADDRESS_ (r3x) % _ALIGNOF_ (PACK<VAR128[3]>) == 0) ;
		rax->free (r3x) ;
	}

#ifdef __CSC_TARGET_EXE__
	TEST_METHOD (TEST_CSC_EXT_GLOBALSTATIC) {
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

	TEST_METHOD (TEST_CSC_EXT_SERIALIZER) {
		struct wrapped_String_STRU8 :private Wrapped<String<STRU8>> {
			inline void visit (const int &stru) {
				wrapped_String_STRU8::mSelf += _BUILDVAR32S_<STRU8> (stru) ;
			}
			inline void visit (const float &stru) {
				wrapped_String_STRU8::mSelf += _BUILDVAL32S_<STRU8> (stru) ;
			}
		} ;
		const auto r1x = PACK<int ,float> {1 ,2.1f} ;
		const auto r2x = Serializer<wrapped_String_STRU8 ,const PACK<int ,float>> (&PACK<int ,float>::P1 ,&PACK<int ,float>::P2) ;
		auto rax = String<STRU8> () ;
		for (FOR_ONCE_DO) {
			auto &r1 = _CAST_<wrapped_String_STRU8> (rax) ;
			r2x (r1x).friend_visit (r1) ;
			//@info: see also std::launder
			rax = std::move (_CAST_<String<STRU8>> (r1)) ;
		}
		_UNITTEST_ASSERT_ (rax == String<STRU8> (_PCSTRU8_ ("12.1"))) ;
	}
} ;
} ;
#endif