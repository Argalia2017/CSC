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
#include <malloc.h>
#include "end.h"

#ifdef __CSC_COMPILER_GNUC__
namespace std {
using ::max_align_t ;
} ;
#endif

namespace CSC {
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

template <class DEPEND>
trait HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename HEAPPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		std::atomic<LENGTH> mUsageSize ;

	public:
		implicit ImplHolder () {
			mUsageSize.store (0) ;
		}

		LENGTH align () const override {
			return ALIGN_OF<std::max_align_t>::value ;
		}

		LENGTH usage () const override {
			LENGTH ret = fake.load () ;
			return move (ret) ;
		}

		FLAG alloc (CREF<LENGTH> size_) const override {
			FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				assume (FALSE) ;
			}
			const auto r1x = current_usage_size (ret) ;
			fake.fetch_add (r1x) ;
			return move (ret) ;
		}

		void free (CREF<FLAG> addr_) const override {
			const auto r1x = current_usage_size (addr_) ;
			fake.fetch_sub (r1x) ;
			operator delete ((&unsafe_pointer (addr_)) ,std::nothrow) ;
		}

	private:
		VREF<std::atomic<LENGTH>> m_fake () const leftvalue {
			const auto r1x = address (mUsageSize) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<std::atomic<LENGTH>>>::id] (unsafe_pointer (r1x))) ;
		}
	} ;
} ;

template <>
exports auto HEAPPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> Box<FakeHolder> {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::id) ;
	return move (ret) ;
}
} ;