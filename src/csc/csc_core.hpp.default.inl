#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require 'csc_core.hpp'"
#endif

#include "begin.h"
#include <cstddef>
#include <new>
#include <exception>
#include <atomic>
#include <stdarg.h>
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

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
	struct FUNCTION_unsafe_barrier {
		inline forceinline void operator() () const noexcept {
			noop () ;
		}
	} ;
} ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_barrier {
		inline forceinline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_barrier_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_barrier {
		inline forceinline void operator() () const noexcept {
			asm volatile ("" ::: "memory") ;
		}
	} ;
#endif
} ;

exports void FUNCTION_unsafe_barrier::invoke () {
	using R1X = typename FUNCTION_unsafe_barrier_HELP<DEPEND ,ALWAYS>::FUNCTION_unsafe_barrier ;
	static constexpr auto M_INVOKE = R1X () ;
	return M_INVOKE () ;
} ;

template <class...>
trait FUNCTION_unsafe_break_HELP ;

template <class DEPEND>
trait FUNCTION_unsafe_break_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
#ifdef __CSC_COMPILER_MSVC__
	struct FUNCTION_unsafe_break {
		inline forceinline void operator() () const noexcept {
			__debugbreak () ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_break_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_unsafe_break {
		inline forceinline void operator() () const noexcept {
			__builtin_trap () ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_unsafe_break_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_unsafe_break {
		inline forceinline void operator() () const noexcept {
			__builtin_trap () ;
		}
	} ;
#endif
} ;

exports void FUNCTION_unsafe_break::invoke () {
	using R1X = typename FUNCTION_unsafe_break_HELP<DEPEND ,ALWAYS>::FUNCTION_unsafe_break ;
	static constexpr auto M_INVOKE = R1X () ;
	return M_INVOKE () ;
} ;

exports void FUNCTION_unsafe_abort::invoke () {
	std::terminate () ;
}

template <class...>
trait FUNCTION_current_usage_size_HELP ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_WINDOWS<DEPEND>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr_) const {
			if (addr_ == ZERO)
				return ZERO ;
			return LENGTH (_msize ((&unsafe_pointer (addr_)))) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_LINUX<DEPEND>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr_) const {
			if (addr_ == ZERO)
				return ZERO ;
			return LENGTH (malloc_usable_size ((&unsafe_pointer (addr_)))) ;
		}
	} ;
#endif
} ;

struct FUNCTION_current_usage_size {
	inline LENGTH operator() (CREF<FLAG> addr_) const {
		using R1X = typename FUNCTION_current_usage_size_HELP<DEPEND ,ALWAYS>::FUNCTION_current_usage_size ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE (addr_) ;
	}
} ;

static constexpr auto current_usage_size = FUNCTION_current_usage_size () ;

template <class...>
trait HEAPPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
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
				assume (FALSE) ;
			}
			const auto r1x = current_usage_size (ret) ;
			mUsageSize->fetch_add (r1x) ;
			return move (ret) ;
		}

		void free (CREF<LENGTH> addr_) {
			const auto r1x = current_usage_size (addr_) ;
			mUsageSize->fetch_sub (r1x) ;
			operator delete ((&unsafe_pointer (addr_)) ,std::nothrow) ;
		}
	} ;
} ;

exports LENGTH FUNCTION_unsafe_align::invoke () {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return R1X::instance ().align () ;
}

exports LENGTH FUNCTION_unsafe_usage::invoke () {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return R1X::instance ().usage () ;
}

exports FLAG FUNCTION_unsafe_alloc::invoke[[nodiscard]] (CREF<LENGTH> size_) {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return R1X::instance ().alloc (size_) ;
}

exports void FUNCTION_unsafe_free::invoke (CREF<LENGTH> addr_) {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return R1X::instance ().free (addr_) ;
}
} ;
} ;