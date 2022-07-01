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
#include <thread>
#include <stdarg.h>
#include <malloc.h>
#include "end.h"

#ifdef __CSC_COMPILER_GNUC__
namespace std {
using ::max_align_t ;
} ;
#endif

namespace CSC {
template <class DEPEND>
trait ABSTRACT_PUREHOLDER_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		FLAG mOrigin ;
		Abstract mAbstract ;
		BOOL mGood ;
		INDEX mNext ;
	} ;

	using HEADER_SIZE = ENUMAS<VAL ,ENUMID<65536>> ;

	class PureHolder {
	protected:
		std::atomic<LENGTH> mSpin ;
		std::recursive_mutex mMutex ;
		INDEX mFirst ;
		INDEX mTempFirst ;
		FLAG mOrigin ;
		FLAG mTempOrigin ;
		LENGTH mSize ;
		INDEX mRead ;
		FLAG mMinCABI ;
		FLAG mMaxCABI ;
		Scope<PureHolder> mHandle ;

	public:
		implicit PureHolder () {
			mSpin.store (0) ;
			mHandle = Scope<PureHolder> (thiz) ;
		}

		void enter () {
			mFirst = NONE ;
			mTempFirst = NONE ;
			mOrigin = ZERO ;
			mTempOrigin = ZERO ;
			mSize = 0 ;
			mRead = 0 ;
			mMinCABI = ZERO ;
			mMaxCABI = ZERO ;
		}

		void leave () {
			using R1X = typename ABSTRACT_PUREHOLDER_HELP<DEPEND ,ALWAYS>::UniqueLock ;
			if ifswitch (TRUE) {
				std::lock_guard<std::recursive_mutex> anonymous (mMutex) ;
				while (TRUE) {
					if (mFirst == NONE)
						break ;
					INDEX ix = mFirst ;
					mFirst = fake[ix].mNext ;
					fake[ix].mNext = mTempFirst ;
					mTempFirst = ix ;
					if ifswitch (TRUE) {
						if (fake[ix].mOrigin == ZERO)
							discard ;
						if ifnot (fake[ix].mGood)
							discard ;
						const auto r2x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
						const auto r3x = valign (r2x ,fake[ix].mAbstract.type_align ()) ;
						fake[ix].mGood = FALSE ;
						fake[ix].mAbstract.destroy (unsafe_pointer (r3x)) ;
						unsafe_destroy (unsafe_deptr (fake[ix].mAbstract)) ;
					}
				}
			}
			if ifswitch (TRUE) {
				Scope<R1X> anonymous (R1X::from (thiz)) ;
				mTempOrigin = mOrigin ;
				mOrigin = ZERO ;
				while (TRUE) {
					if (mTempFirst == NONE)
						break ;
					INDEX ix = mTempFirst ;
					mTempFirst = fake[ix].mNext ;
					operator delete (csc_pointer_t (fake[ix].mOrigin)) ;
				}
				mSize = 0 ;
				mRead = 0 ;
				mMinCABI = ZERO ;
				mMaxCABI = ZERO ;
				operator delete (csc_pointer_t (mTempOrigin)) ;
				mTempOrigin = ZERO ;
			}
		}

		void shared_enter () {
			auto rax = ZERO ;
			while (TRUE) {
				rax = vmax (rax ,0) ;
				const auto r1x = mSpin.compare_exchange_weak (rax ,rax + 1 ,std::memory_order::memory_order_relaxed) ;
				if (r1x)
					break ;
				std::this_thread::yield () ;
			}
			std::atomic_thread_fence (std::memory_order::memory_order_acquire) ;
		}

		void shared_leave () {
			mSpin.fetch_sub (1 ,std::memory_order::memory_order_relaxed) ;
		}

		void unique_enter () {
			auto rax = ZERO ;
			while (TRUE) {
				rax = 0 ;
				const auto r1x = mSpin.compare_exchange_weak (rax ,-1 ,std::memory_order::memory_order_relaxed) ;
				if (r1x)
					break ;
				std::this_thread::yield () ;
			}
			std::atomic_thread_fence (std::memory_order::memory_order_acquire) ;
		}

		void unique_leave () {
			mSpin.store (0 ,std::memory_order::memory_order_relaxed) ;
			std::atomic_thread_fence (std::memory_order::memory_order_release) ;
		}

		FLAG linkage (CREF<TEMP<void>> func ,RREF<Abstract> abst) {
			using R1X = typename ABSTRACT_PUREHOLDER_HELP<DEPEND ,ALWAYS>::SharedLock ;
			using R2X = typename ABSTRACT_PUREHOLDER_HELP<DEPEND ,ALWAYS>::UniqueLock ;
			INDEX ix = NONE ;
			const auto r1x = abst.type_cabi () ;
			if ifswitch (TRUE) {
				Scope<R1X> anonymous (R1X::from (thiz)) ;
				if (r1x < mMinCABI)
					discard ;
				if (r1x > mMaxCABI)
					discard ;
				const auto r2x = mOrigin + (mRead + r1x - mMinCABI) % mSize ;
				ix = bitwise[TYPEAS<INDEX>::expr] (unsafe_pointer (r2x)) ;
				if (ix == NONE)
					discard ;
				assert (fake[ix].mGood) ;
				const auto r3x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
				const auto r4x = valign (r3x ,fake[ix].mAbstract.type_align ()) ;
				return r4x ;
			}
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				Scope<R2X> anonymous (R2X::from (thiz)) ;
				update_resize (r1x) ;
				assert (mOrigin != ZERO) ;
				const auto r5x = vmax (abst.type_align () - SIZE_OF<NODE>::expr ,ZERO) ;
				const auto r6x = vmax (SIZE_OF<NODE>::expr - 1 ,ZERO) ;
				const auto r7x = r6x + SIZE_OF<NODE>::expr + r5x + abst.type_size () ;
				const auto r8x = FLAG (operator new (r7x ,std::nothrow)) ;
				assume (r8x != ZERO) ;
				ix = valign (r8x ,SIZE_OF<NODE>::expr) / SIZE_OF<NODE>::expr ;
				fake[ix].mOrigin = r8x ;
				fake[ix].mGood = FALSE ;
				fake[ix].mNext = NONE ;
				auto rax = Box<Abstract>::make (move (abst)) ;
				swap (fake[ix].mAbstract ,rax.self) ;
				rax.release () ;
				const auto r9x = mOrigin + (mRead + r1x - mMinCABI) % mSize ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<INDEX>>::expr] (unsafe_pointer (r9x))) ;
				tmp = ix ;
			}
			std::lock_guard<std::recursive_mutex> anonymous (mMutex) ;
			const auto r10x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
			const auto r11x = valign (r10x ,fake[ix].mAbstract.type_align ()) ;
			if ifswitch (TRUE) {
				if (fake[ix].mGood)
					discard ;
				fake[ix].mAbstract.create (unsafe_pointer (r11x) ,func) ;
				fake[ix].mGood = TRUE ;
				fake[ix].mNext = mFirst ;
				mFirst = ix ;
			}
			return r11x ;
		}

		void update_resize (CREF<FLAG> cabi) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mOrigin != ZERO)
					discard ;
				const auto r2x = HEADER_SIZE::expr ;
				mTempOrigin = FLAG (operator new (r2x ,std::nothrow)) ;
				if (mTempOrigin == ZERO)
					discard ;
				const auto r3x = valign (mTempOrigin ,ALIGN_OF<INDEX>::expr) ;
				std::memset (csc_pointer_t (r3x) ,int (0XFF) ,r2x) ;
				mMinCABI = cabi ;
				mMaxCABI = cabi ;
				mRead = r3x - mTempOrigin ;
				mSize = r2x ;
				mOrigin = mTempOrigin ;
				mTempOrigin = ZERO ;
			}
			if ifswitch (rxx) {
				if (cabi >= mMinCABI)
					discard ;
				const auto r4x = mMinCABI - cabi ;
				const auto r5x = mSize - (mMaxCABI + ALIGN_OF<INDEX>::expr - mMinCABI) ;
				if (r4x > r5x)
					discard ;
				mRead = (mRead - r4x + mSize) % mSize ;
				mMinCABI = cabi ;
			}
			if ifswitch (rxx) {
				if (cabi <= mMaxCABI)
					discard ;
				const auto r6x = cabi - mMaxCABI ;
				const auto r7x = mSize - (mMaxCABI + ALIGN_OF<INDEX>::expr - mMinCABI) ;
				if (r6x > r7x)
					discard ;
				mMaxCABI = cabi ;
			}
			if ifswitch (rxx) {
				if (vbetween (cabi ,mMinCABI ,mMaxCABI + 1))
					discard ;
				const auto r8x = mSize + vmax (cabi - mMaxCABI ,mMinCABI - cabi) ;
				const auto r9x = invoke ([&] () {
					LENGTH ret = mSize ;
					while (TRUE) {
						if (ret >= r8x)
							break ;
						ret *= 2 ;
					}
					return move (ret) ;
				}) ;
				mTempOrigin = FLAG (operator new (r9x ,std::nothrow)) ;
				if (mTempOrigin == ZERO)
					discard ;
				const auto r10x = valign (mTempOrigin ,ALIGN_OF<INDEX>::expr) ;
				const auto r11x = mOrigin + mRead ;
				const auto r12x = mRead + (mMaxCABI + ALIGN_OF<INDEX>::expr - mMinCABI) ;
				auto ryx = TRUE ;
				if ifswitch (ryx) {
					if (r12x >= mSize)
						discard ;
					const auto r13x = r12x - mRead ;
					std::memcpy (csc_pointer_t (r10x) ,csc_pointer_t (r11x) ,r13x) ;
					std::memset (csc_pointer_t (r10x + r13x) ,int (0XFF) ,r9x - r13x) ;
				}
				if ifswitch (ryx) {
					const auto r14x = mSize - mRead ;
					const auto r15x = mRead % ALIGN_OF<INDEX>::expr ;
					const auto r16x = r12x % mSize ;
					std::memcpy (csc_pointer_t (r10x) ,csc_pointer_t (r11x) ,r14x) ;
					std::memcpy (csc_pointer_t (r10x + r14x) ,csc_pointer_t (mOrigin + r15x) ,r16x) ;
					std::memset (csc_pointer_t (r10x + r14x + r16x) ,int (0XFF) ,r9x - r14x - r16x) ;
				}
				const auto r17x = MathProc::max_of (mMinCABI - cabi ,ZERO) ;
				mRead = r10x - mTempOrigin ;
				mSize = r9x ;
				operator delete (csc_pointer_t (mOrigin)) ;
				mOrigin = mTempOrigin ;
				mTempOrigin = ZERO ;
				mMinCABI = vmin (mMinCABI ,cabi) ;
				mMaxCABI = vmax (mMaxCABI ,cabi) ;
				mRead = (mRead - r17x + mSize) % mSize ;
			}
		}

		VREF<ARR<NODE>> fake_m () const leftvalue {
			return unsafe_array (TYPEAS<NODE>::expr) ;
		}
	} ;

	class SharedLock extend Proxy {
	protected:
		PureHolder mBase ;

	public:
		imports VREF<SharedLock> from (VREF<PureHolder> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<SharedLock>>::expr] (unsafe_deptr (that))) ;
		}

		void enter () {
			mBase.shared_enter () ;
		}

		void leave () {
			mBase.shared_leave () ;
		}
	} ;

	class UniqueLock extend Proxy {
	protected:
		PureHolder mBase ;

	public:
		imports VREF<UniqueLock> from (VREF<PureHolder> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UniqueLock>>::expr] (unsafe_deptr (that))) ;
		}

		void enter () {
			mBase.unique_enter () ;
		}

		void leave () {
			mBase.unique_leave () ;
		}
	} ;
} ;

template <>
exports FLAG ABSTRACT_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage::invoke (CREF<TEMP<void>> func ,RREF<Abstract> abst) {
	using R1X = typename ABSTRACT_PUREHOLDER_HELP<DEPEND ,ALWAYS>::PureHolder ;
	static R1X mInstance ;
	return mInstance.linkage (func ,move (abst)) ;
}

template <class...>
trait FUNCTION_current_usage_size_HELP ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_WINDOWS<DEPEND>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<csc_pointer_t> addr) const {
			if (addr == NULL)
				return ZERO ;
			return LENGTH (_msize (addr)) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_LINUX<DEPEND>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<csc_pointer_t> addr) const {
			if (addr == ZERO)
				return ZERO ;
			return LENGTH (malloc_usable_size (addr)) ;
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
			auto &&tmp = memorize ([&] () {
				return Box<HEAP>::make () ;
			}) ;
			mPointer = address (tmp.self) ;
			fake.mUsageSize.store (0) ;
		}

		LENGTH usage_size () const override {
			LENGTH ret = fake.mUsageSize.load () ;
			return move (ret) ;
		}

		LENGTH basic_align () const override {
			return ALIGN_OF<std::max_align_t>::expr ;
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

		void free (CREF<FLAG> addr) const override {
			if (addr == ZERO)
				return ;
			const auto r1x = csc_pointer_t (addr) ;
			const auto r2x = current_usage_size (r1x) ;
			fake.mUsageSize.fetch_sub (r2x) ;
			operator delete (r1x ,std::nothrow) ;
		}

		VREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_pointer (mPointer))) ;
		}

		LENGTH current_usage_size (CREF<csc_pointer_t> addr) const {
			using R1X = typename FUNCTION_current_usage_size_HELP<DEPEND ,ALWAYS>::FUNCTION_current_usage_size ;
			const auto r1x = R1X () ;
			return r1x (addr) ;
		}
	} ;
} ;

template <>
exports auto HEAPPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> Box<FakeHolder> {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait CLAZZ_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mTypeSize ;
		LENGTH mTypeAlign ;
		FLAG mTypeCABI ;
		Slice<STR> mTypeName ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> align_ ,CREF<FLAG> cabi ,CREF<Slice<STR>> name) override {
			mTypeSize = size_ ;
			mTypeAlign = align_ ;
			mTypeCABI = cabi ;
			mTypeName = name ;
		}

		LENGTH type_size () const override {
			return mTypeSize ;
		}

		LENGTH type_align () const override {
			return mTypeAlign ;
		}

		FLAG type_cabi () const override {
			return mTypeCABI ;
		}

		Slice<STR> type_name () const override {
			return mTypeName ;
		}
	} ;
} ;

template <>
exports auto CLAZZ_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename CLAZZ_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait EXCEPTION_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename EXCEPTION_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Slice<STR> mWhat ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<Slice<STR>> what_) override {
			mWhat = move (what_) ;
		}

		Slice<STR> what () const override {
			return mWhat ;
		}
	} ;
} ;

template <>
exports auto EXCEPTION_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename EXCEPTION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;