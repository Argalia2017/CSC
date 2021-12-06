#pragma once

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

#include "begin.h"
#include <cstddef>
#include <new>
#include <exception>
#include <atomic>
#include "end.h"

#ifdef __CSC_COMPILER_GNUC__
namespace std {
using ::max_align_t ;
} ;
#endif

namespace CSC {
namespace CORE {
template <class...>
trait FUNCTION_unsafe_barrier_HELP ;

template <class UNIT1>
trait FUNCTION_unsafe_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_MSVC<UNIT1>>> {
	struct FUNCTION_unsafe_barrier {
		inline void operator() () const noexcept {
			noop () ;
		}
	} ;
} ;

template <class UNIT1>
trait FUNCTION_unsafe_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_GNUC<UNIT1>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_barrier {
		inline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_unsafe_barrier_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_CLANG<UNIT1>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_barrier {
		inline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

exports void FUNCTION_unsafe_barrier::invoke () {
	using R1X = typename FUNCTION_unsafe_barrier_HELP<void ,ALWAYS>::FUNCTION_unsafe_barrier ;
	static constexpr auto M_INVOKE = R1X () ;
	return M_INVOKE () ;
} ;

template <class...>
trait FUNCTION_unsafe_break_HELP ;

template <class UNIT1>
trait FUNCTION_unsafe_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_MSVC<UNIT1>>> {
#ifdef __CSC_COMPILER_MSVC__
	struct FUNCTION_unsafe_break {
		forceinline void operator() () const noexcept {
			__debugbreak () ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_unsafe_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_GNUC<UNIT1>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_break {
		forceinline void operator() () const noexcept {
			__builtin_trap () ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_unsafe_break_HELP<UNIT1 ,REQUIRE<MACRO_COMPILER_CLANG<UNIT1>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_break {
		forceinline void operator() () const noexcept {
			__builtin_trap () ;
		}
	} ;
#endif
} ;

exports void FUNCTION_unsafe_break::invoke () {
	using R1X = typename FUNCTION_unsafe_break_HELP<void ,ALWAYS>::FUNCTION_unsafe_break ;
	static constexpr auto M_INVOKE = R1X () ;
	return M_INVOKE () ;
} ;

exports void FUNCTION_unsafe_abort::invoke () {
	std::terminate () ;
}

template <class...>
trait FUNCTION_current_usage_size_HELP ;

template <class UNIT1>
trait FUNCTION_current_usage_size_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_WINDOWS<UNIT1>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr_) const {
			if (addr_ == ZERO)
				return ZERO ;
			const auto r1x = &unsafe_pointer (addr_) ;
			return LENGTH (_msize (r1x)) ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_current_usage_size_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_LINUX<UNIT1>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr_) const {
			if (addr_ == ZERO)
				return ZERO ;
			const auto r1x = &unsafe_pointer (addr_) ;
			return LENGTH (malloc_usable_size (r1x)) ;
		}
	} ;
#endif
} ;

template <class...>
trait HEAPPROC_IMPLHOLDER_HELP ;

template <>
trait HEAPPROC_IMPLHOLDER_HELP<ALWAYS> {
	class ImplHolder {
	private:
		Box<std::atomic<LENGTH>> mUsageSize ;

	public:
		imports VREF<ImplHolder> instance () {
			static auto mInstance = invoke ([&] () {
				ImplHolder ret ;
				ret.mUsageSize = Box<std::atomic<LENGTH>>::make (0) ;
				return move (ret) ;
			}) ;
			return mInstance ;
		}

		LENGTH align () const {
			return ALIGN_OF<std::max_align_t>::value ;
		}

		LENGTH usage () const {
			LENGTH ret = mUsageSize->load () ;
			return move (ret) ;
		}

		FLAG alloc[[nodiscard]] (CREF<LENGTH> size_) {
			FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				dynamic_assert (FALSE) ;
				return ZERO ;
			}
			const auto r1x = current_usage_size (ret) ;
			mUsageSize->fetch_add (r1x) ;
			return move (ret) ;
		}

		void free (CREF<LENGTH> addr_) {
			const auto r1x = current_usage_size (addr_) ;
			mUsageSize->fetch_sub (r1x) ;
			const auto r2x = &unsafe_pointer (addr_) ;
			operator delete (r2x ,std::nothrow) ;
		}

	private:
		LENGTH current_usage_size (CREF<FLAG> addr_) const {
			using R1X = typename FUNCTION_current_usage_size_HELP<void ,ALWAYS>::FUNCTION_current_usage_size ;
			static constexpr auto M_INVOKE = R1X () ;
			return M_INVOKE (addr_) ;
		}
	} ;
} ;

exports LENGTH FUNCTION_unsafe_align::invoke () {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return R1X::instance ().align () ;
}

exports LENGTH FUNCTION_unsafe_usage::invoke () {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return R1X::instance ().usage () ;
}

exports FLAG FUNCTION_unsafe_alloc::invoke (CREF<LENGTH> size_) {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return R1X::instance ().alloc (size_) ;
}

exports void FUNCTION_unsafe_free::invoke (CREF<LENGTH> addr_) {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return R1X::instance ().free (addr_) ;
}
} ;
} ;