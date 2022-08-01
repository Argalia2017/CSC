#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require 'csc_core.hpp'"
#endif

#include "begin.h"
#include <cstddef>
#include <cstring>
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
		FLAG mAlways ;
		INDEX mFirst ;
		INDEX mTempFirst ;
		FLAG mOrigin ;
		FLAG mTempOrigin ;
		LENGTH mSize ;
		INDEX mRead ;
		FLAG mMinCabi ;
		FLAG mMaxCabi ;
		Scope<PureHolder> mHandle ;

	public:
		implicit PureHolder () {
			mAlways = SIZE_OF<NODE>::expr ;
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
			mMinCabi = ZERO ;
			mMaxCabi = ZERO ;
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
						const auto r1x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
						const auto r2x = valign (r1x ,fake[ix].mAbstract.type_align ()) ;
						fake[ix].mGood = FALSE ;
						fake[ix].mAbstract.destroy (unsafe_pointer (r2x)) ;
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
					const auto r3x = fake[ix].mOrigin ;
					unsafe_destroy (unsafe_deptr (fake[ix])) ;
					operator delete (csc_pointer_t (r3x)) ;
				}
				mSize = 0 ;
				mRead = 0 ;
				mMinCabi = ZERO ;
				mMaxCabi = ZERO ;
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
#ifdef __CSC_COMPILER_CLANG__
			printf ("r1x = %d\n" ,r1x) ;
#endif
			if ifswitch (TRUE) {
				Scope<R1X> anonymous (R1X::from (thiz)) ;
				if (r1x < mMinCabi)
					discard ;
				if (r1x > mMaxCabi)
					discard ;
				const auto r2x = mOrigin + (mRead + r1x - mMinCabi) % mSize ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<INDEX>>::expr] (unsafe_pointer (r2x))) ;
				ix = tmp ;
				if (ix == NONE)
					discard ;
				assert (fake[ix].mGood) ;
#ifdef __CSC_COMPILER_CLANG__
				const auto r13x = fake[ix].mAbstract.type_cabi () ;
				printf ("r13x = %d\n" ,r13x) ;
				assert (r13x == r1x) ;
#endif
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
				const auto r9x = valign (r8x ,SIZE_OF<NODE>::expr) ;
				ix = (r9x - mAlways) / SIZE_OF<NODE>::expr ;
				unsafe_create (unsafe_deptr (fake[ix])) ;
				fake[ix].mOrigin = r8x ;
				fake[ix].mGood = FALSE ;
				fake[ix].mNext = NONE ;
				fake[ix].mAbstract = move (abst) ;
				const auto r10x = mOrigin + (mRead + r1x - mMinCabi) % mSize ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<INDEX>>::expr] (unsafe_pointer (r10x))) ;
				tmp = ix ;
			}
			std::lock_guard<std::recursive_mutex> anonymous (mMutex) ;
			const auto r11x = address (fake[ix]) + SIZE_OF<NODE>::expr ;
			const auto r12x = valign (r11x ,fake[ix].mAbstract.type_align ()) ;
			if ifswitch (TRUE) {
				if (fake[ix].mGood)
					discard ;
				fake[ix].mAbstract.create (unsafe_pointer (r12x) ,func) ;
				fake[ix].mGood = TRUE ;
				fake[ix].mNext = mFirst ;
				mFirst = ix ;
			}
			return r12x ;
		}

		void update_resize (CREF<FLAG> cabi) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mOrigin != ZERO)
					discard ;
				const auto r1x = HEADER_SIZE::expr ;
				mTempOrigin = FLAG (operator new (r1x ,std::nothrow)) ;
				if (mTempOrigin == ZERO)
					discard ;
				const auto r2x = valign (mTempOrigin ,ALIGN_OF<INDEX>::expr) ;
				std::memset (csc_pointer_t (r2x) ,int (0XFF) ,r1x) ;
				mMinCabi = cabi ;
				mMaxCabi = cabi ;
				mRead = r2x - mTempOrigin ;
				mSize = r1x ;
				mOrigin = mTempOrigin ;
				mTempOrigin = ZERO ;
			}
			if ifswitch (rxx) {
				if (cabi >= mMinCabi)
					discard ;
				const auto r3x = mMinCabi - cabi ;
				const auto r4x = mSize - (mMaxCabi + ALIGN_OF<INDEX>::expr - mMinCabi) ;
				if (r3x > r4x)
					discard ;
				mRead = (mRead - r3x + mSize) % mSize ;
				mMinCabi = cabi ;
			}
			if ifswitch (rxx) {
				if (cabi <= mMaxCabi)
					discard ;
				const auto r5x = cabi - mMaxCabi ;
				const auto r6x = mSize - (mMaxCabi + ALIGN_OF<INDEX>::expr - mMinCabi) ;
				if (r5x > r6x)
					discard ;
				mMaxCabi = cabi ;
			}
			if ifswitch (rxx) {
				if (vbetween (cabi ,mMinCabi ,mMaxCabi + 1))
					discard ;
				const auto r7x = mSize + vmax (cabi - mMaxCabi ,mMinCabi - cabi) ;
				const auto r8x = invoke ([&] () {
					LENGTH ret = mSize ;
					while (TRUE) {
						if (ret >= r7x)
							break ;
						ret *= 2 ;
					}
					return move (ret) ;
				}) ;
				mTempOrigin = FLAG (operator new (r8x ,std::nothrow)) ;
				if (mTempOrigin == ZERO)
					discard ;
				const auto r9x = valign (mTempOrigin ,ALIGN_OF<INDEX>::expr) ;
				const auto r10x = mOrigin + mRead ;
				const auto r11x = mRead + (mMaxCabi + ALIGN_OF<INDEX>::expr - mMinCabi) ;
				auto ryx = TRUE ;
				if ifswitch (ryx) {
					if (r11x >= mSize)
						discard ;
					const auto r12x = r11x - mRead ;
					std::memcpy (csc_pointer_t (r9x) ,csc_pointer_t (r10x) ,r12x) ;
					std::memset (csc_pointer_t (r9x + r12x) ,int (0XFF) ,r8x - r12x) ;
					unsafe_barrier () ;
				}
				if ifswitch (ryx) {
					const auto r13x = mSize - mRead ;
					const auto r14x = mRead % ALIGN_OF<INDEX>::expr ;
					const auto r15x = r11x % mSize ;
					std::memcpy (csc_pointer_t (r9x) ,csc_pointer_t (r10x) ,r13x) ;
					std::memcpy (csc_pointer_t (r9x + r13x) ,csc_pointer_t (mOrigin + r14x) ,r15x) ;
					std::memset (csc_pointer_t (r9x + r13x + r15x) ,int (0XFF) ,r8x - r13x - r15x) ;
					unsafe_barrier () ;
				}
				const auto r16x = MathProc::max_of (mMinCabi - cabi ,ZERO) ;
				mRead = r9x - mTempOrigin ;
				mSize = r8x ;
				operator delete (csc_pointer_t (mOrigin)) ;
				mOrigin = mTempOrigin ;
				mTempOrigin = ZERO ;
				mMinCabi = vmin (mMinCabi ,cabi) ;
				mMaxCabi = vmax (mMaxCabi ,cabi) ;
				mRead = (mRead - r16x + mSize) % mSize ;
			}
		}

		VREF<ARR<NODE>> fake_m () const leftvalue {
			//@fatal: fuck clang5.0
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<NODE>>::expr] (unsafe_pointer (mAlways))) ;
			return unsafe_array (tmp) ;
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
exports auto HEAPPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename HEAPPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait VREF_PUREHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename VREF_HELP<DEPEND ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		FLAG mOrigin ;
		LENGTH mCounter ;
		Abstract mAbstract ;
		LENGTH mSize ;

	public:
		implicit PureHolder () = default ;

		imports FLAG create (CREF<LENGTH> size_ ,CREF<LENGTH> align_) {
			const auto r1x = dynamic_size (size_ ,align_) ;
			const auto r2x = HeapProc::instance ().alloc (r1x) ;
			const auto r3x = valign (r2x ,ALIGN_OF<PureHolder>::expr) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<PureHolder>>::expr] (unsafe_pointer (r3x))) ;
			unsafe_create (unsafe_deptr (tmp)) ;
			tmp.mOrigin = r2x ;
			tmp.mCounter = 0 ;
			tmp.mSize = 0 ;
			return address (keep[TYPEAS<VREF<Holder>>::expr] (tmp)) ;
		}

		imports LENGTH dynamic_size (CREF<LENGTH> size_ ,CREF<LENGTH> align_) {
			const auto r1x = HeapProc::instance ().basic_align () ;
			const auto r2x = vmax (ALIGN_OF<PureHolder>::expr ,align_) ;
			const auto r3x = vmax (r2x - r1x ,ZERO) ;
			const auto r4x = valign (SIZE_OF<PureHolder>::expr ,align_) ;
			const auto r5x = r3x + r4x + size_ ;
			return r5x ;
		}

		void initialize (RREF<Abstract> abst) override {
			mAbstract = move (abst) ;
		}

		void destroy () override {
			const auto r1x = mOrigin ;
			if (r1x == ZERO)
				return ;
			unsafe_destroy (unsafe_deptr (thiz)) ;
			HeapProc::instance ().free (r1x) ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			const auto r1x = pointer () + mSize * mAbstract.type_size () ;
			std::memcpy (csc_pointer_t (r1x) ,(&obj) ,mAbstract.type_size ()) ;
			unsafe_barrier () ;
			mSize++ ;
		}

		void release () override {
			while (TRUE) {
				if (mSize <= 0)
					break ;
				INDEX ix = mSize - 1 ;
				const auto r1x = pointer () + ix * mAbstract.type_size () ;
				mAbstract.destroy (unsafe_pointer (r1x)) ;
				mSize-- ;
			}
		}

		FLAG pointer () const override {
			const auto r1x = valign (SIZE_OF<PureHolder>::expr ,mAbstract.type_align ()) ;
			const auto r2x = address (thiz) + r1x ;
			return r2x ;
		}

		LENGTH increase () override {
			if (mCounter == NONE)
				return NONE ;
			mCounter++ ;
			return mCounter ;
		}

		LENGTH decrease () override {
			if (mCounter == NONE)
				return NONE ;
			mCounter-- ;
			return mCounter ;
		}

		BOOL lock () override {
			replace (mCounter ,IDEN ,NONE) ;
			return mCounter == NONE ;
		}

		void unlock () override {
			replace (mCounter ,NONE ,IDEN) ;
		}
	} ;
} ;

template <>
exports auto VREF_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_linkage::invoke (CREF<LENGTH> size_ ,CREF<LENGTH> align_) ->FLAG {
	using R1X = typename VREF_PUREHOLDER_HELP<DEPEND ,ALWAYS>::PureHolder ;
	return R1X::create (size_ ,align_) ;
}

template <class DEPEND>
trait AUTO_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename AUTO_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Abstract mAbstract ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<Abstract> abst) override {
			mAbstract = move (abst) ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			const auto r1x = pointer () ;
			std::memcpy (csc_pointer_t (r1x) ,(&obj) ,mAbstract.type_size ()) ;
			unsafe_barrier () ;
		}

		void release () override {
			const auto r1x = pointer () ;
			mAbstract.destroy (unsafe_pointer (r1x)) ;
		}

		FLAG pointer () const override {
			const auto r1x = valign (SIZE_OF<ImplHolder>::expr ,mAbstract.type_align ()) ;
			const auto r2x = address (thiz) + r1x ;
			return r2x ;
		}

		FLAG type_cabi () const override {
			return mAbstract.type_cabi () ;
		}
	} ;
} ;

template <>
exports auto AUTO_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename AUTO_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class ITEM>
trait SLICE_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using Holder = typename SLICE_HELP<ITEM ,ALWAYS>::Holder ;
	using NODE = typename SLICE_HELP<ITEM ,ALWAYS>::NODE ;
	using RANK = RANK3 ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mSize ;
		Box<ARR<NODE ,RANK>> mSlice ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<NODE> text1) override {
			mSize = 0 ;
			mSlice = Box<ARR<NODE ,RANK>>::make () ;
			template_assign (PHX ,TYPEAS<RANK>::expr ,text1) ;
		}

		void initialize (CREF<NODE> text1 ,CREF<NODE> text2) override {
			mSize = 0 ;
			mSlice = Box<ARR<NODE ,RANK>>::make () ;
			template_assign (PHX ,TYPEAS<RANK>::expr ,text1 ,text2) ;
		}

		void initialize (CREF<NODE> text1 ,CREF<NODE> text2 ,CREF<NODE> text3) override {
			mSize = 0 ;
			mSlice = Box<ARR<NODE ,RANK>>::make () ;
			template_assign (PHX ,TYPEAS<RANK>::expr ,text1 ,text2 ,text3) ;
		}

		template <class ARG1 ,class...ARG3 ,class = REQUIRE<ENUM_GT_ZERO<ARG1>>>
		void template_assign (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id ,CREF<NODE> text1 ,CREF<ARG3>...text2) {
			if ifswitch (TRUE) {
				if (text1.mAlign == 1)
					discard ;
				assert (text1.mAlign == ALIGN_OF<ITEM>::expr) ;
			}
			INDEX jx = 0 ;
			while (TRUE) {
				if (jx >= text1.mCount)
					break ;
				const auto r1x = text1.mPointer + jx * text1.mAlign ;
				const auto r2x = at_load (text1.mAlign ,r1x) ;
				if (r2x == ITEM (0))
					break ;
				jx++ ;
			}
			INDEX ix = ENUM_SUB<RANK ,ARG1>::expr ;
			mSlice.self[ix].mPointer = text1.mPointer ;
			mSlice.self[ix].mAlign = text1.mAlign ;
			mSlice.self[ix].mCount = jx ;
			mSize += jx ;
			template_assign (PHX ,TYPEAS<ENUM_DEC<ARG1>>::expr ,text2...) ;
		}

		template <class ARG1>
		void template_assign (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id) {
			noop () ;
		}

		LENGTH size () const override {
			return mSize ;
		}

		ITEM at (CREF<INDEX> index) const override {
			INDEX ix = index ;
			for (auto &&i : iter (0 ,RANK::expr)) {
				if ifswitch (TRUE) {
					if ifnot (vbetween (ix ,0 ,mSlice.self[i].mCount))
						discard ;
					const auto r1x = mSlice.self[i].mPointer + ix * mSlice.self[i].mAlign ;
					return at_load (mSlice.self[i].mAlign ,r1x) ;
				}
				ix -= mSlice.self[i].mCount ;
			}
			return bad (TYPEAS<ITEM>::expr) ;
		}

		ITEM at_load (CREF<LENGTH> align_ ,CREF<FLAG> pointer) const {
			if ifswitch (TRUE) {
				if (align_ != ALIGN_OF<STRU8>::expr)
					discard ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRU8>>::expr] (unsafe_pointer (pointer))) ;
				return ITEM (tmp) ;
			}
			if ifswitch (TRUE) {
				if (align_ != ALIGN_OF<STRU16>::expr)
					discard ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRU16>>::expr] (unsafe_pointer (pointer))) ;
				return ITEM (tmp) ;
			}
			if ifswitch (TRUE) {
				if (align_ != ALIGN_OF<STRU32>::expr)
					discard ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRU32>>::expr] (unsafe_pointer (pointer))) ;
				return ITEM (tmp) ;
			}
			assert (FALSE) ;
			return ITEM (0) ;
		}
	} ;
} ;

template <>
exports auto SLICE_HELP<STRA ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SLICE_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <>
exports auto SLICE_HELP<STRW ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SLICE_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <>
exports auto SLICE_HELP<STRU8 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SLICE_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <>
exports auto SLICE_HELP<STRU16 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SLICE_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <>
exports auto SLICE_HELP<STRU32 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SLICE_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait CLAZZ_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename CLAZZ_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mTypeSize ;
		LENGTH mTypeAlign ;
		FLAG mTypeCabi ;
		Slice<STR> mTypeName ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> align_ ,CREF<FLAG> cabi ,CREF<Slice<STR>> name) override {
			mTypeSize = size_ ;
			mTypeAlign = align_ ;
			mTypeCabi = cabi ;
			mTypeName = name ;
		}

		LENGTH type_size () const override {
			return mTypeSize ;
		}

		LENGTH type_align () const override {
			return mTypeAlign ;
		}

		FLAG type_cabi () const override {
			return mTypeCabi ;
		}

		Slice<STR> type_name () const override {
			return mTypeName ;
		}
	} ;
} ;

template <>
exports auto CLAZZ_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
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
exports auto EXCEPTION_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename EXCEPTION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;