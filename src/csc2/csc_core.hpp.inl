#pragma once

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
namespace U {
template <class UNIT1>
exports void FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<DEPENDENT<MACRO_COMPILER_MSVC ,UNIT1>>>::
FUNCTION_barrier::extern_invoke () {
	noop () ;
}

template <class UNIT1>
exports void FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<DEPENDENT<MACRO_COMPILER_GNUC ,UNIT1>>>::
FUNCTION_barrier::extern_invoke () {
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" ::: "memory") ;
#endif
}

template <class UNIT1>
exports void FUNCTION_barrier_HELP<UNIT1 ,REQUIRE<DEPENDENT<MACRO_COMPILER_CLANG ,UNIT1>>>::
FUNCTION_barrier::extern_invoke () {
#ifdef __CSC_COMPILER_GNUC__
	asm volatile ("" ::: "memory") ;
#endif
}
} ;

namespace U {
template <class...>
trait LAUNDER_IMPLHOLDER_HELP ;

template <>
trait LAUNDER_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename FUNCTION_launder_HELP<ALWAYS>::FUNCTION_launder::LaunderHolder ;

	class ImplHolder :public Holder {
	public:
		void launder (CREF<LENGTH> addr) const override {
			noop () ;
		}
	} ;
} ;

exports CREF<typename FUNCTION_launder_HELP<ALWAYS>::FUNCTION_launder::LaunderHolder> FUNCTION_launder_HELP<ALWAYS>::
FUNCTION_launder::extern_launder () {
	using R1X = typename LAUNDER_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	static R1X tmp ;
	return tmp ;
}
} ;

namespace U {
template <class UNIT1>
exports void FUNCTION_debug_assert_HELP<UNIT1 ,REQUIRE<DEPENDENT<MACRO_COMPILER_MSVC ,UNIT1>>>::
FUNCTION_debug_break::extern_invoke () {
#ifdef __CSC_COMPILER_MSVC__
	__debugbreak () ;
#endif
}

template <class UNIT1>
exports void FUNCTION_debug_assert_HELP<UNIT1 ,REQUIRE<DEPENDENT<MACRO_COMPILER_GNUC ,UNIT1>>>::
FUNCTION_debug_break::extern_invoke () {
#ifdef __CSC_COMPILER_GUNC__
	__builtin_trap () ;
#endif
}

template <class UNIT1>
exports void FUNCTION_debug_assert_HELP<UNIT1 ,REQUIRE<DEPENDENT<MACRO_COMPILER_CLANG ,UNIT1>>>::
FUNCTION_debug_break::extern_invoke () {
	std::abort () ;
}
} ;
} ;