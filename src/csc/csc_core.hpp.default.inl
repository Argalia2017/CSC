#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require 'csc_core.hpp'"
#endif

#include "begin.h"
#include <cstddef>
#include <new>
#include <exception>
#include <atomic>
#include <mutex>
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
		inline LENGTH operator() (CREF<csc_pointer_t> addr_) const {
			if (addr_ == NULL)
				return ZERO ;
			return LENGTH (_msize (addr_)) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_LINUX<DEPEND>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<csc_pointer_t> addr_) const {
			if (addr_ == ZERO)
				return ZERO ;
			return LENGTH (malloc_usable_size (addr_)) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename HEAPPROC_HELP<DEPEND ,ALWAYS>::Holder ;
	
	struct HEAP {
		std::atomic<LENGTH> mUsageSize ;
	} ;

	class ImplHolder implement Holder {
	protected:
		FLAG mPointer ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			static auto mInstance = Box<HEAP>::make () ;
			mInstance->mUsageSize.store (0) ;
			mPointer = address (mInstance.self) ;
		}

		LENGTH align () const override {
			return ALIGN_OF<std::max_align_t>::value ;
		}

		LENGTH usage_size () const override {
			LENGTH ret = fake.mUsageSize.load () ;
			return move (ret) ;
		}

		FLAG alloc (CREF<LENGTH> size_) const override {
			FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				assume (FALSE) ;
			}
			const auto r1x = csc_pointer_t (ret) ;
			const auto r2x = current_usage_size (r1x) ;
			fake.mUsageSize.fetch_add (r2x) ;
			return move (ret) ;
		}

		void free (CREF<FLAG> addr_) const override {
			if (addr_ == ZERO)
				return ;
			const auto r1x = csc_pointer_t (addr_) ;
			const auto r2x = current_usage_size (r1x) ;
			fake.mUsageSize.fetch_sub (r2x) ;
			operator delete (r1x ,std::nothrow) ;
		}

		VREF<HEAP> m_fake () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_pointer (mPointer))) ;
		}

		LENGTH current_usage_size (CREF<csc_pointer_t> addr_) const {
			using R1X = typename FUNCTION_current_usage_size_HELP<DEPEND ,ALWAYS>::FUNCTION_current_usage_size ;
			const auto r1x = R1X () ;
			return r1x (addr_) ;
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

template <class DEPEND>
trait STATICHEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename STATICHEAPPROC_HELP<DEPEND ,ALWAYS>::Holder ;
	using CHUNK_PAGE_SIZE = ENUMAS<VAL ,ENUMID<65536>> ;
	using CHUNK_SIZE = ENUMAS<VAL ,ENUMID<1024>> ;

	struct NODE {
		FLAG mOrigin ;
		LENGTH mCounter ;
		LENGTH mSize ;
		LENGTH mRest ;
	} ;

	struct HEAP {
		std::mutex mMutex ;
		ARR<NODE ,CHUNK_SIZE> mChunk ;
	} ;

	class ImplHolder implement Holder {
	protected:
		FLAG mPointer ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			static auto mInstance = Box<HEAP>::make () ;
			for (auto &&i : iter (0 ,CHUNK_SIZE::value))
				mInstance->mChunk[i].mOrigin = ZERO ;
			mPointer = address (mInstance.self) ;
		}

		LENGTH align () const override {
			return LENGTH (1) ;
		}

		LENGTH usage_size () const override {
			std::lock_guard<std::mutex> anonymous (fake.mMutex) ;
			LENGTH ret = 0 ;
			for (auto &&i : iter (0 ,CHUNK_SIZE::value)) {
				if (fake.mChunk[i].mOrigin == ZERO)
					continue ;
				ret += fake.mChunk[i].mSize - fake.mChunk[i].mRest ;
			}
			return move (ret) ;
		}

		FLAG alloc (CREF<LENGTH> size_) const override {
			std::lock_guard<std::mutex> anonymous (fake.mMutex) ;
			INDEX ix = find_alloc_chunk (size_) ;
			assume (ix != NONE) ;
			if ifswitch (TRUE) {
				if (fake.mChunk[ix].mOrigin != ZERO)
					discard ;
				fake.mChunk[ix].mOrigin = HeapProc::instance ().alloc (CHUNK_PAGE_SIZE::value) ;
				fake.mChunk[ix].mCounter = 0 ;
				fake.mChunk[ix].mSize = CHUNK_PAGE_SIZE::value ;
				fake.mChunk[ix].mRest = fake.mChunk[ix].mSize ;
			}
			const auto r1x = fake.mChunk[ix].mSize - fake.mChunk[ix].mRest ;
			FLAG ret = fake.mChunk[ix].mOrigin + r1x ;
			fake.mChunk[ix].mCounter++ ;
			fake.mChunk[ix].mRest -= size_ ;
			return move (ret) ;
		}

		INDEX find_alloc_chunk (CREF<LENGTH> size_) const {
			for (auto &&i : iter (0 ,CHUNK_SIZE::value)) {
				if (fake.mChunk[i].mOrigin == ZERO)
					continue ;
				if (fake.mChunk[i].mRest >= size_)
					return i ;
			}
			for (auto &&i : iter (0 ,CHUNK_SIZE::value)) {
				if (fake.mChunk[i].mOrigin == ZERO)
					return i ;
			}
			return NONE ;
		}

		void free (CREF<FLAG> addr_) const override {
			if (addr_ == ZERO)
				return ;
			std::lock_guard<std::mutex> anonymous (fake.mMutex) ;
			INDEX ix = find_free_chunk (addr_) ;
			assert (ix != NONE) ;
			assert (fake.mChunk[ix].mCounter > 0) ;
			if ifswitch (TRUE) {
				fake.mChunk[ix].mCounter-- ;
				if (fake.mChunk[ix].mCounter > 0)
					discard ;
				fake.mChunk[ix].mRest = fake.mChunk[ix].mSize ;
			}
		}

		INDEX find_free_chunk (CREF<FLAG> addr_) const {
			for (auto &&i : iter (0 ,CHUNK_SIZE::value)) {
				if (fake.mChunk[i].mOrigin == ZERO)
					continue ;
				if (vbetween (addr_ ,fake.mChunk[i].mOrigin ,fake.mChunk[i].mSize))
					return i ;
			}
			return NONE ;
		}

		VREF<HEAP> m_fake () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::id] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <>
exports auto STATICHEAPPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> Box<FakeHolder> {
	using R1X = typename STATICHEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::id) ;
	return move (ret) ;
}
} ;