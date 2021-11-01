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
#include <mutex>
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
trait FUNCTION_current_usage_size_HELP ;

template <class UNIT1>
trait FUNCTION_current_usage_size_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_WINDOWS<UNIT1>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr_) const {
			return LENGTH (_msize (&unsafe_pointer (addr_))) ;
		}
	} ;
#endif
} ;

template <class UNIT1>
trait FUNCTION_current_usage_size_HELP<UNIT1 ,REQUIRE<MACRO_SYSTEM_LINUX<UNIT1>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr_) const {
			return LENGTH (malloc_usable_size (&unsafe_pointer (addr_))) ;
		}
	} ;
#endif
} ;

struct FUNCTION_current_usage_size {
	inline LENGTH operator() (CREF<FLAG> addr_) const {
		using R1X = typename FUNCTION_current_usage_size_HELP<void ,ALWAYS>::FUNCTION_current_usage_size ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE (addr_) ;
	}
} ;

static constexpr auto current_usage_size = FUNCTION_current_usage_size () ;

template <class...>
trait HEAPPROC_IMPLHOLDER_HELP ;

template <>
trait HEAPPROC_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename HEAPPROC_HELP<ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		mutable std::mutex mUsageMutex ;
		Cell<LENGTH> mUsageSize ;

	public:
		LENGTH align () const override {
			return ALIGN_OF<std::max_align_t>::value ;
		}

		LENGTH usage () const override {
			std::lock_guard<std::mutex> anonymous (mUsageMutex) ;
			return mUsageSize.fetch () ;
		}

		FLAG alloc (CREF<LENGTH> size_) const override {
			FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				dynamic_assert (FALSE) ;
				return ZERO ;
			}
			usage_add (usage_size (ret)) ;
			return move (ret) ;
		}

		void free (CREF<LENGTH> addr_) const override {
			usage_sub (usage_size (addr_)) ;
			operator delete (&unsafe_pointer (addr_) ,std::nothrow) ;
		}

	private:
		LENGTH usage_size (CREF<LENGTH> addr_) const {
			if (addr_ == ZERO)
				return ZERO ;
			return current_usage_size (addr_) ;
		}

		void usage_add (CREF<LENGTH> size_) const {
			std::lock_guard<std::mutex> anonymous (mUsageMutex) ;
			mUsageSize.fetch_add (size_) ;
		}

		void usage_sub (CREF<LENGTH> size_) const {
			std::lock_guard<std::mutex> anonymous (mUsageMutex) ;
			mUsageSize.fetch_sub (size_) ;
		}
	} ;
} ;

exports auto HEAPPROC_HELP<ALWAYS>::FUNCTION_make::extern_invoke () -> FLAG {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	auto &&tmp = memorize ([&] () {
		return R1X () ;
	}) ;
	return address (tmp) ;
} ;

exports void FUNCTION_unsafe_abort::extern_invoke () {
	std::terminate () ;
}
} ;
} ;