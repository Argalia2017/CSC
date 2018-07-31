#include "util.h"

#ifdef __CSC_EXT__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC_EXT) {
public:
	TEST_METHOD (TEST_CSC_EXT) {

	}

	TEST_METHOD (TEST_CSC_EXT_VAR128) {
		_UNITTEST_ASSERT_ (VAR128 (VAR64_MAX) * 2 - VAR64_MAX == VAR64_MAX) ;
	}

	TEST_METHOD (TEST_CSC_EXT_VARIANT) {
		auto rax = Optional<int> () ;
		auto rbx = int () ;
		rax.apply (Function<void (const int &)> ([&] (const int &arg) {
			rbx = arg ;
		})) ;
		Optional<FixedBuffer<STRU8>> a ;
		a.recreate<FixedBuffer<STRU8>> (LENGTH (8)) ;
		auto &r1 = a.self ;
		_UNITTEST_ASSERT_ (r1.size () == 8) ;
		Optional<FixedBuffer<STRU8>> b ;
		b.recreate<FixedBuffer<STRU8>> (LENGTH (4)) ;
		auto &r2 = b.self ;
		_UNITTEST_ASSERT_ (r2.size () == 4) ;
	}

	TEST_METHOD (TEST_CSC_EXT_TUPLE) {
		const auto r1x = _DEDUCE_<Tuple> (1 ,0.2 ,'c') ;
		_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<decltype (r1x.visit<1> ())> ,double>::value) ;
		auto rax = int () ;
		auto rbx = double () ;
		auto rcx = char () ;
		TupleBinder<int ,double ,char> (rax ,rbx ,rcx) = r1x ;
	}

	TEST_METHOD (TEST_CSC_EXT_FUNCTION) {
		struct A {
			void test1 () {}
			void test2 () const {}
			static void test4 () {}
		} ;
		auto rax = A () ;
		_CALL_ (_DEDUCE_<Function> (PhanRef<A>::make (&rax) ,&A::test1)) ;
		_CALL_ (_DEDUCE_<Function> (PhanRef<A>::make (&rax) ,&A::test2)) ;
		_CALL_ (_DEDUCE_<Function> (&A::test4)) ;
		const auto r1x = [] (const int &arg1 ,const int &arg2) {
			if (arg2 == 1)
				return arg1 ;
			if (arg2 == 2)
				return arg1 * arg1 ;
			return 0 ;
		} ;
		const auto r2x = Function<int (const int &)>::make (_XVALUE_<const PTR<int (const int & ,const int &)> &> (r1x) ,_XVALUE_<const int &> (2)) ;
		_UNITTEST_ASSERT_ (r2x (2) == 4) ;
		_UNITTEST_ASSERT_ (r2x (3) == 9) ;
		const auto r3x = Function<int (const int & ,const int &)>::make (_XVALUE_<const PTR<int (const int & ,const int &)> &> (r1x)) ;
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
		_UNITTEST_ASSERT_ (_ADRESS_ (&r4x.self) != _ADRESS_ (&r5x.self) && r4x == r5x) ;
		_UNITTEST_ASSERT_ (r4x->func1 () == 3 && r5x->func1 () == 3) ;
		_UNITTEST_ASSERT_ (r4x->func2 () == 1 && r5x->func2 () == 2) ;
	}

	TEST_METHOD (TEST_CSC_EXT_MEMORYPOOL) {
		const auto r1x = SharedRef<MemoryPool>::make () ;
		const auto r2x = r1x->alloc<int> () ;
		const auto r10x = [] (const VAR &arg1 ,const VAR &arg2) {
			_DEBUG_ASSERT_ (arg2 != VAR (0)) ;
			const auto r4x = arg2 < VAR (0) ? -arg1 : arg1 ;
			const auto r5x = _ABS_ (arg2) ;
			VAR ret = VAR (r5x * VAR64 (r4x / r5x)) ;
			if (r4x < 0 && ret > r4x)
				ret -= r5x ;
			if (arg2 < 0)
				ret = -ret ;
			return std::move (ret) ;
		} ;
		const auto r11x = r10x (_SIZEOF_ (int) ,LENGTH (-8)) + _MAX_ (_ALIGNOF_ (int) - 8 ,VAR_ZERO) ;
		const auto r12x = r10x (_SIZEOF_ (TEMP<UniqueRef<void>>) ,LENGTH (-8)) + _MAX_ (_ALIGNOF_ (TEMP<UniqueRef<void>>) - 8 ,VAR_ZERO) ;
		const auto r13x = r10x (_SIZEOF_ (DEF<double[44]>) ,LENGTH (-8)) + _MAX_ (_ALIGNOF_ (DEF<double[44]>) - 8 ,VAR_ZERO) ;
		_UNITTEST_ASSERT_ (r1x->length () == r11x) ;
		r1x->alloc<TEMP<UniqueRef<void>>> () ;
		_UNITTEST_ASSERT_ (r1x->length () == r11x + r12x) ;
		r1x->alloc<double> (44) ;
		_UNITTEST_ASSERT_ (r1x->length () == r11x + r12x + r13x) ;
		r1x->clean () ;
		_UNITTEST_ASSERT_ (r1x->length () == r11x + r12x + r13x) ;
		r1x->free (r2x) ;
		_UNITTEST_ASSERT_ (r1x->length () == r12x + r13x) ;
		const auto r3x = r1x->alloc<PACK<VAR128[3]>> (1) ;
		_UNITTEST_ASSERT_ (_ADRESS_ (r3x) % _ALIGNOF_ (PACK<VAR128[3]>) == 0) ;
		r1x->free (r3x) ;
	}

	TEST_METHOD (TEST_CSC_EXT_GLOBALSTATIC) {
		using UUID1 = ARGC<1> ;
		using UUID2 = ARGC<2> ;
		using UUID3 = ARGC<3> ;
		using UUID4 = ARGC<4> ;
		GlobalStatic<UUID1>::save (1) ;
		GlobalStatic<UUID2>::save (2) ;
		const auto r1x = GlobalStatic<UUID2>::load () ;
		_UNITTEST_ASSERT_ (r1x == 2) ;
		auto rax = FALSE ;
		_CALL_TRY_ ([&] () {
			GlobalStatic<UUID3>::load () ;
		} ,[&] () {
			rax = TRUE ;
		}) ;
		_UNITTEST_ASSERT_ (rax) ;
		GlobalStatic<UUID4>::save (4) ;
		GlobalStatic<UUID4>::save (-4) ;
		const auto r2x = GlobalStatic<UUID1>::load () ;
		_UNITTEST_ASSERT_ (r2x == 1) ;
	}
} ;
} ;
#endif

#ifdef __CSC_EXT__
namespace CSC {
#pragma region
#pragma warning (push)
#pragma warning (disable :4273)
export LENGTH GlobalStatic<void>::unique_atomic_value (LENGTH exp ,LENGTH val) noexcept popping {
	static std::atomic<LENGTH> mInstance (0) ;
	mInstance.compare_exchange_strong (exp ,val) ;
	return mInstance.load () ;
}
#pragma warning (pop)
#pragma endregion
} ;
#endif