#pragma once

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
namespace U {
#ifdef __CSC_COMPILER_MSVC__
template <>
exports void FUNCTION_unsafe_break_HELP<void ,ALWAYS>::FUNCTION_unsafe_break::extern_invoke () {
	__debugbreak () ;
}
#endif

#ifdef __CSC_COMPILER_GNUC__
template <>
exports void FUNCTION_unsafe_break_HELP<void ,ALWAYS>::FUNCTION_unsafe_break::extern_invoke () {
	__builtin_trap () ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
template <>
exports void FUNCTION_unsafe_break_HELP<void ,ALWAYS>::FUNCTION_unsafe_break::extern_invoke () {
	std::abort () ;
}
#endif
} ;

namespace U {
#ifdef __CSC_COMPILER_MSVC__
template <>
exports void FUNCTION_barrier_HELP<void ,ALWAYS>::FUNCTION_barrier::extern_invoke () {
	noop () ;
}
#endif

#ifdef __CSC_COMPILER_GNUC__
template <>
exports void FUNCTION_barrier_HELP<void ,ALWAYS>::FUNCTION_barrier::extern_invoke () {
	asm volatile ("" ::: "memory") ;
}
#endif

#ifdef __CSC_COMPILER_CLANG__
template <>
exports void FUNCTION_barrier_HELP<void ,ALWAYS>::FUNCTION_barrier::extern_invoke () {
	asm volatile ("" ::: "memory") ;
}
#endif
} ;
} ;