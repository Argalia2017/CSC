#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require 'csc_core.hpp'"
#endif

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require 'csc_basic.hpp'"
#endif

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_runtime.hpp"

#include "csc_end.h"
#include <cstddef>
#include <cstring>
#include <new>
#include <exception>
#include <atomic>
#include <mutex>
#include <thread>
#include <stdarg.h>
#include <malloc.h>
#include "csc_begin.h"

#ifdef __CSC_COMPILER_GNUC__
namespace std {
using ::max_align_t ;
} ;
#endif

namespace CSC {
template <class...>
trait FUNCTION_current_usage_size_HELP ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_WINDOWS<DEPEND>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr) const {
			if (addr == ZERO)
				return 0 ;
			return LENGTH (_msize (csc_pointer_t (addr))) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_LINUX<DEPEND>>> {
#ifdef __CSC_SYSTEM_LINUX__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr) const {
			if (addr == ZERO)
				return 0 ;
			return LENGTH (malloc_usable_size (csc_pointer_t (addr))) ;
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
		void initialize () override {
			mPointer = address (unique ().self) ;
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
			const auto r1x = current_usage_size (ret) ;
			fake.mUsageSize.fetch_add (r1x) ;
			return move (ret) ;
		}

		void free (CREF<FLAG> addr) const override {
			if (addr == ZERO)
				return ;
			const auto r1x = current_usage_size (addr) ;
			fake.mUsageSize.fetch_sub (r1x) ;
			operator delete (csc_pointer_t (addr) ,std::nothrow) ;
		}

		imports CREF<Box<HEAP>> unique () {
			return memorize ([&] () {
				return Box<HEAP>::make () ;
			}) ;
		}

		VREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_pointer (mPointer))) ;
		}

		LENGTH current_usage_size (CREF<FLAG> addr) const {
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
trait STATICPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename STATICPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	struct NODE {
		FLAG mCabi ;
		CRef<Proxy> mAddr ;
		BOOL mGood ;
		INDEX mNext ;
	} ;

	struct HEAP {
		Mutex mMutex ;
		BOOL mFinalizing ;
		VarBuffer<INDEX> mRange ;
		LENGTH mSize ;
		INDEX mRead ;
		FLAG mMinCabi ;
		FLAG mMaxCabi ;
		Allocator<NODE ,VARIABLE> mHeap ;
		INDEX mFirst ;
		LENGTH mLength ;
	} ;

	using HEADER_SIZE = ENUMAS<VAL ,65536> ;

	class ImplHolder implement Holder {
	protected:
		FLAG mPointer ;

	public:
		void initialize () {
			mPointer = address (unique ().self) ;
			fake.mMutex = RecursiveMutex::make () ;
			fake.mFinalizing = FALSE ;
			fake.mSize = 0 ;
			fake.mRead = 0 ;
			fake.mMinCabi = ZERO ;
			fake.mMaxCabi = ZERO ;
			fake.mFirst = NONE ;
			fake.mLength = 0 ;
		}

		void finalize () override {
			auto rax = SharedLock (fake.mMutex) ;
			fake.mFinalizing = TRUE ;
			auto rbx = CRef<Proxy> () ;
			while (TRUE) {
				if (fake.mFirst == NONE)
					break ;
				if ifswitch (TRUE) {
					Scope<SharedLock> anonymous (rax) ;
					if (fake.mFirst == NONE)
						discard ;
					INDEX ix = fake.mFirst ;
					rbx = move (fake.mHeap[ix].mAddr) ;
					fake.mHeap[ix].mGood = FALSE ;
					fake.mFirst = fake.mHeap[ix].mNext ;
					fake.mLength-- ;
				}
				rbx = NULL ;
			}
			fake.mHeap.clear () ;
			fake.mHeap = Allocator<NODE ,VARIABLE> () ;
			fake.mRange = VarBuffer<INDEX> () ;
		}

		void regi (CREF<FLAG> cabi ,VREF<CRef<Proxy>> addr) const {
			assert (addr != NULL) ;
			auto rax = SharedLock (fake.mMutex) ;
			if (fake.mFinalizing)
				return ;
			const auto r1x = cabi ;
			INDEX ix = NONE ;
			if ifswitch (TRUE) {
				if (r1x < fake.mMinCabi)
					discard ;
				if (r1x > fake.mMaxCabi)
					discard ;
				if (fake.mSize == 0)
					discard ;
				const auto r2x = (fake.mRead + r1x - fake.mMinCabi) % fake.mSize ;
				ix = fake.mRange[r2x] ;
			}
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				Scope<SharedLock> anonymous (rax) ;
				update_resize (r1x) ;
				ix = fake.mHeap.alloc () ;
				const auto r3x = (fake.mRead + r1x - fake.mMinCabi) % fake.mSize ;
				fake.mRange[r3x] = ix ;
				fake.mHeap[ix].mCabi = r1x ;
				fake.mHeap[ix].mAddr = NULL ;
				fake.mHeap[ix].mGood = FALSE ;
				fake.mHeap[ix].mNext = NONE ;
			}
			if ifswitch (TRUE) {
				if (fake.mHeap[ix].mGood)
					discard ;
				Scope<SharedLock> anonymous (rax) ;
				fake.mHeap[ix].mAddr = move (addr) ;
				fake.mHeap[ix].mGood = TRUE ;
				fake.mHeap[ix].mNext = fake.mFirst ;
				fake.mFirst = ix ;
				fake.mLength++ ;
			}
			addr = CRef<Proxy>::reference (fake.mHeap[ix].mAddr.self) ;
		}

		CRef<Proxy> link (CREF<FLAG> cabi) const {
			auto rax = SharedLock (fake.mMutex) ;
			const auto r1x = cabi ;
			INDEX ix = NONE ;
			if ifswitch (TRUE) {
				if (r1x < fake.mMinCabi)
					discard ;
				if (r1x > fake.mMaxCabi)
					discard ;
				if (fake.mSize == 0)
					discard ;
				const auto r2x = (fake.mRead + r1x - fake.mMinCabi) % fake.mSize ;
				ix = fake.mRange[r2x] ;
				if (ix == NONE)
					discard ;
				return CRef<Proxy>::reference (fake.mHeap[ix].mAddr.self) ;
			}
			return NULL ;
		}

		void update_resize (CREF<FLAG> cabi) const {
			auto act = TRUE ;
			if ifswitch (act) {
				if (fake.mRange.size () > 0)
					discard ;
				const auto r1x = HEADER_SIZE::expr ;
				auto rax = VarBuffer<INDEX> (r1x) ;
				if (rax.size () == 0)
					discard ;
				BufferProc<INDEX>::buf_fill (rax ,NONE ,0 ,rax.size ()) ;
				fake.mRange = move (rax) ;
				fake.mSize = r1x ;
				fake.mRead = 0 ;
				fake.mMinCabi = cabi ;
				fake.mMaxCabi = cabi ;
			}
			if ifswitch (act) {
				if (cabi >= fake.mMinCabi)
					discard ;
				const auto r2x = fake.mMinCabi - cabi ;
				const auto r3x = fake.mSize - (fake.mMaxCabi + 1 - fake.mMinCabi) ;
				if (r2x > r3x)
					discard ;
				fake.mRead = (fake.mRead - r2x + fake.mSize) % fake.mSize ;
				fake.mMinCabi = cabi ;
			}
			if ifswitch (act) {
				if (cabi <= fake.mMaxCabi)
					discard ;
				const auto r4x = cabi - fake.mMaxCabi ;
				const auto r5x = fake.mSize - (fake.mMaxCabi + 1 - fake.mMinCabi) ;
				if (r4x > r5x)
					discard ;
				fake.mMaxCabi = cabi ;
			}
			if ifswitch (act) {
				if (vbetween (cabi ,fake.mMinCabi ,fake.mMaxCabi + 1))
					discard ;
				const auto r6x = fake.mSize + vmax (cabi - fake.mMaxCabi ,fake.mMinCabi - cabi) ;
				auto rax = VarBuffer<INDEX> (r6x) ;
				if (rax.size () == 0)
					discard ;
				const auto r7x = fake.mRead + (fake.mMaxCabi + 1 - fake.mMinCabi) ;
				const auto r8x = vmin (r7x ,fake.mSize) - fake.mRead ;
				BufferProc<INDEX>::buf_copy (rax ,unsafe_array (fake.mRange[fake.mRead]) ,0 ,r8x) ;
				const auto r9x = vmax (r7x - fake.mSize ,0) ;
				BufferProc<INDEX>::buf_copy (unsafe_array (rax[r8x]) ,fake.mRange ,0 ,r9x) ;
				const auto r10x = vmax (fake.mMinCabi - cabi ,0) ;
				fake.mRange = move (rax) ;
				fake.mSize = r6x ;
				fake.mRead = (fake.mRead - r10x + fake.mSize) % fake.mSize ;
				fake.mMinCabi = vmin (fake.mMinCabi ,cabi) ;
				fake.mMaxCabi = vmax (fake.mMaxCabi ,cabi) ;
			}
		}

		imports CREF<Box<HEAP>> unique () {
			return memorize ([&] () {
				return Box<HEAP>::make () ;
			}) ;
		}

		VREF<HEAP> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <>
exports auto STATICPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->Box<FakeHolder> {
	using R1X = typename STATICPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait AUTO_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename AUTO_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		BOOL mGood ;
		FLAG mCabi ;
		Unknown mUnknown ;

	public:
		void initialize (CREF<FLAG> cabi ,RREF<Unknown> unknown) override {
			mGood = FALSE ;
			mCabi = cabi ;
			mUnknown = move (unknown) ;
		}

		void finalize () override {
			if ifnot (mGood)
				return ;
			mUnknown.recycle (0) ;
			mGood = FALSE ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			assert (ifnot (mGood)) ;
			mUnknown.acquire (0 ,obj) ;
			mGood = TRUE ;
		}

		void release () override {
			mGood = FALSE ;
			mCabi = ZERO ;
		}

		FLAG pointer () const override {
			return mUnknown.pointer () ;
		}

		FLAG type_cabi () const override {
			return mCabi ;
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

	class ImplHolder implement Holder {
	protected:
		CRef<Holder> mPrefix ;
		csc_text_t mText ;
		LENGTH mSize ;

	public:
		void initialize (CREF<csc_text_t> text) override {
			mText = text ;
			mSize = text_size () ;
		}

		void initialize (CREF<CRef<Holder>> prefix ,CREF<csc_text_t> text) override {
			mPrefix = prefix ;
			mText = text ;
			mSize = prefix_size () + text_size () ;
		}

		LENGTH prefix_size () const {
			if (mPrefix == NULL)
				return 0 ;
			return mPrefix->size () ;
		}

		LENGTH text_size () const {
			if (mText.mStep == 0)
				return 0 ;
			INDEX ix = mText.mBegin ;
			while (TRUE) {
				if (ix >= mText.mEnd)
					break ;
				const auto r1x = at_load (mText.mStep ,ix) ;
				if (r1x == ITEM (0))
					break ;
				ix += mText.mStep ;
			}
			return (ix - mText.mBegin) / mText.mStep ;
		}

		LENGTH size () const override {
			return mSize ;
		}

		ITEM at (CREF<INDEX> index) const override {
			assert (vbetween (index ,0 ,size ())) ;
			const auto r1x = prefix_size () ;
			if ifswitch (TRUE) {
				if (index >= r1x)
					discard ;
				return mPrefix->at (index) ;
			}
			const auto r2x = mText.mBegin + (index - r1x) * mText.mStep ;
			return at_load (mText.mStep ,r2x) ;
		}

		ITEM at_load (CREF<LENGTH> align_ ,CREF<FLAG> pointer) const {
			if ifswitch (TRUE) {
				if (align_ != ALIGN_OF<STRU8>::expr)
					discard ;
				auto rax = STRU8 () ;
				unsafe_sync (unsafe_deptr (rax) ,unsafe_pointer (pointer)) ;
				unsafe_launder (rax) ;
				return ITEM (rax) ;
			}
			if ifswitch (TRUE) {
				if (align_ != ALIGN_OF<STRU16>::expr)
					discard ;
				auto rax = STRU16 () ;
				unsafe_sync (unsafe_deptr (rax) ,unsafe_pointer (pointer)) ;
				unsafe_launder (rax) ;
				return ITEM (rax) ;
			}
			if ifswitch (TRUE) {
				if (align_ != ALIGN_OF<STRU32>::expr)
					discard ;
				auto rax = STRU32 () ;
				unsafe_sync (unsafe_deptr (rax) ,unsafe_pointer (pointer)) ;
				unsafe_launder (rax) ;
				return ITEM (rax) ;
			}
			return bad (TYPEAS<ITEM>::expr) ;
		}

		BOOL equal (CREF<Holder> that) const override {
			return equal (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		BOOL equal (CREF<ImplHolder> that) const {
			if (size () != that.size ())
				return FALSE ;
			for (auto &&i : iter (0 ,size ())) {
				if ifnot (operator_equal (at (i) ,that.at (i)))
					return FALSE ;
			}
			return TRUE ;
		}

		FLAG compr (CREF<Holder> that) const override {
			return compr (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		FLAG compr (CREF<ImplHolder> that) const {
			const auto r1x = vmin (size () ,that.size ()) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = operator_compr (at (i) ,that.at (i)) ;
				if (r2x != ZERO)
					return r2x ;
			}
			return ZERO ;
		}

		FLAG hash () const override {
			FLAG ret = hashcode () ;
			for (auto &&i : iter (0 ,size ())) {
				const auto r1x = operator_hash (at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
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
		LENGTH mSize ;
		LENGTH mAlign ;
		FLAG mCabi ;
		Slice<STR> mName ;

	public:
		void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> align_ ,CREF<FLAG> cabi ,CREF<Slice<STR>> name) override {
			mSize = size_ ;
			mAlign = align_ ;
			mCabi = cabi ;
			mName = name ;
		}

		LENGTH type_size () const override {
			return mSize ;
		}

		LENGTH type_align () const override {
			return mAlign ;
		}

		FLAG type_cabi () const override {
			return mCabi ;
		}

		Slice<STR> type_name () const override {
			return mName ;
		}

		BOOL equal (CREF<Holder> that) const override {
			return equal (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		BOOL equal (CREF<ImplHolder> that) const {
			return mCabi == that.mCabi ;
		}

		FLAG compr (CREF<Holder> that) const override {
			return compr (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		FLAG compr (CREF<ImplHolder> that) const {
			const auto r1x = operator_compr (mCabi ,that.mCabi) ;
			if (r1x == ZERO)
				return r1x ;
			const auto r2x = operator_compr (mName ,that.mName) ;
			if (r2x != ZERO)
				return r2x ;
			return r1x ;
		}

		FLAG hash () const override {
			return mName.hash () ;
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