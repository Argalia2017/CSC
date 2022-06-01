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
template <class DEPEND>
trait LINKAGE_PUREHOLDER_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		FLAG mOrigin ;
		FLAG mItem ;
		Linkage mClazz ;
		BOOL mGood ;
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
		INDEX mNext ;
	} ;

	class PureHolder {
	protected:
		std::atomic<LENGTH> mSpin ;
		std::recursive_mutex mRecursive ;
		INDEX mRoot ;
		INDEX mFirst ;

	public:
		implicit PureHolder () {
			mSpin.store (0) ;
			mRoot = NONE ;
			mFirst = NONE ;
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

		FLAG linkage (CREF<TEMP<void>> func ,RREF<Linkage> clazz) {
			using R1X = typename LINKAGE_PUREHOLDER_HELP<DEPEND ,ALWAYS>::SharedLock ;
			using R2X = typename LINKAGE_PUREHOLDER_HELP<DEPEND ,ALWAYS>::UniqueLock ;
			INDEX ix = NONE ;
			const auto r1x = clazz.type_cabi () ;
			if ifswitch (TRUE) {
				Scope<R1X> anonymous (R1X::from (thiz)) ;
				ix = find (r1x) ;
				if (ix == NONE)
					discard ;
				if ifnot (fake[ix].mGood)
					discard ;
				const auto r2x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
				const auto r3x = valign (r2x ,clazz.type_align ()) ;
				return r3x ;
			}
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				Scope<R2X> anonymous (R2X::from (thiz)) ;
				const auto r4x = vmax (clazz.type_align () - ALIGN_OF<NODE>::expr ,ZERO) ;
				const auto r5x = vmax (ALIGN_OF<NODE>::expr - 1 ,ZERO) ;
				const auto r6x = r5x + SIZE_OF<NODE>::expr + r4x + clazz.type_size () ;
				const auto r7x = FLAG (operator new (r6x ,std::nothrow)) ;
				assume (r7x != ZERO) ;
				ix = r7x / SIZE_OF<NODE>::expr ;
				fake[ix].mOrigin = r7x ;
				fake[ix].mNext = mFirst ;
				fake[ix].mGood = FALSE ;
				mFirst = ix ;
				fake[ix].mItem = r1x ;
				auto rax = Box<Linkage>::make (move (clazz)) ;
				swap (fake[ix].mClazz ,rax.self) ;
				rax.release () ;
				fake[ix].mWeight = 1 ;
				fake[ix].mLeft = NONE ;
				fake[ix].mRight = NONE ;
				update_insert (mRoot) ;
			}
			std::lock_guard<std::recursive_mutex> anonymous (mRecursive) ;
			const auto r8x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
			const auto r9x = valign (r8x ,fake[ix].mClazz.type_align ()) ;
			if ifswitch (TRUE) {
				if (fake[ix].mGood)
					discard ;
				fake[ix].mClazz.create (unsafe_pointer (r9x) ,func) ;
				fake[ix].mGood = TRUE ;
			}
			return r9x ;
		}

		VREF<ARR<NODE>> fake_m () const {
			return unsafe_array (TYPEAS<NODE>::expr) ;
		}

		FLAG find (CREF<FLAG> item) const {
			FLAG ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = operator_compr (item ,fake[ret].mItem) ;
				if (r1x == ZERO)
					break ;
				const auto r2x = invoke ([&] () {
					if (r1x < ZERO)
						return fake[ret].mLeft ;
					return fake[ret].mRight ;
				}) ;
				ret = r2x ;
			}
			return move (ret) ;
		}

		void update_insert (VREF<INDEX> curr) {
			INDEX ix = mFirst ;
			if ifswitch (TRUE) {
				if (curr == NONE)
					discard ;
				ix = curr ;
				fake[ix].mWeight++ ;
				const auto r1x = operator_compr (fake[mFirst].mItem ,fake[ix].mItem) ;
				assert (r1x != ZERO) ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if (r1x > ZERO)
						discard ;
					update_insert (fake[ix].mLeft) ;
					update_insert_left (ix) ;
				}
				if ifswitch (rxx) {
					update_insert (fake[ix].mRight) ;
					update_insert_right (ix) ;
				}
			}
			curr = ix ;
		}

		void update_insert_left (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (fake[ix].mLeft == NONE)
				return ;
			const auto r1x = node_weight (fake[ix].mRight) ;
			const auto r2x = node_weight (fake[fake[ix].mLeft].mLeft) ;
			const auto r3x = node_weight (fake[fake[ix].mLeft].mRight) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_left (fake[ix].mLeft) ;
			}
			rotate_right (ix) ;
			update_insert_left (fake[ix].mLeft) ;
			update_insert_right (fake[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void update_insert_right (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (fake[ix].mRight == NONE)
				return ;
			const auto r1x = node_weight (fake[ix].mLeft) ;
			const auto r2x = node_weight (fake[fake[ix].mRight].mRight) ;
			const auto r3x = node_weight (fake[fake[ix].mRight].mLeft) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_right (fake[ix].mRight) ;
			}
			rotate_left (ix) ;
			update_insert_left (fake[ix].mLeft) ;
			update_insert_right (fake[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void rotate_left (VREF<INDEX> curr) {
			INDEX ix = fake[curr].mRight ;
			fake[curr].mRight = fake[ix].mLeft ;
			fake[ix].mLeft = curr ;
			fake[ix].mWeight = fake[curr].mWeight ;
			const auto r1x = node_weight (fake[curr].mLeft) ;
			const auto r2x = node_weight (fake[curr].mRight) ;
			fake[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		void rotate_right (VREF<INDEX> curr) {
			INDEX ix = fake[curr].mLeft ;
			fake[curr].mLeft = fake[ix].mRight ;
			fake[ix].mRight = curr ;
			fake[ix].mWeight = fake[curr].mWeight ;
			const auto r1x = node_weight (fake[curr].mLeft) ;
			const auto r2x = node_weight (fake[curr].mRight) ;
			fake[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		LENGTH node_weight (CREF<INDEX> curr) const {
			if (curr == NONE)
				return ZERO ;
			return fake[curr].mWeight ;
		}
	} ;

	class SharedLock {
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

	class UniqueLock {
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
exports FLAG LINKAGE_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage::invoke (CREF<TEMP<void>> func ,RREF<Linkage> clazz) {
	using R1X = typename LINKAGE_PUREHOLDER_HELP<DEPEND ,ALWAYS>::PureHolder ;
	static R1X mInstance ;
	return mInstance.linkage (func ,move (clazz)) ;
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

		void free (CREF<FLAG> addr_) const override {
			if (addr_ == ZERO)
				return ;
			const auto r1x = csc_pointer_t (addr_) ;
			const auto r2x = current_usage_size (r1x) ;
			fake.mUsageSize.fetch_sub (r2x) ;
			operator delete (r1x ,std::nothrow) ;
		}

		VREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_pointer (mPointer))) ;
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
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}
} ;