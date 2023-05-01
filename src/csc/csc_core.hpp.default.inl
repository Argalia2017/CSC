#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require 'csc_core.hpp'"
#endif

#include "csc_core.hpp"

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
		Box<std::atomic<VAL>> mUsageSize ;
	} ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			fake.mUsageSize = Box<std::atomic<VAL>>::make () ;
			fake.mUsageSize->store (0 ,std::memory_order::memory_order_relaxed) ;
		}

		void finalize () override {
			drop (fake) ;
		}

		LENGTH usage_size () const override {
			LENGTH ret = fake.mUsageSize->load (std::memory_order::memory_order_relaxed) ;
			return move (ret) ;
		}

		LENGTH basic_align () const override {
			return ALIGN_OF<DATA>::expr ;
		}

		FLAG alloc (CREF<LENGTH> size_) const override {
			FLAG ret = FLAG (operator new (size_ ,std::nothrow)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				assume (FALSE) ;
			}
			const auto r1x = current_usage_size (ret) ;
			fake.mUsageSize->fetch_add (r1x ,std::memory_order::memory_order_relaxed) ;
			return move (ret) ;
		}

		void free (CREF<FLAG> addr) const override {
			if (addr == ZERO)
				return ;
			const auto r1x = current_usage_size (addr) ;
			fake.mUsageSize->fetch_sub (r1x ,std::memory_order::memory_order_relaxed) ;
			operator delete (csc_pointer_t (addr) ,std::nothrow) ;
		}

		LENGTH current_usage_size (CREF<FLAG> addr) const {
			using R1X = typename FUNCTION_current_usage_size_HELP<DEPEND ,ALWAYS>::FUNCTION_current_usage_size ;
			const auto r1x = R1X () ;
			return r1x (addr) ;
		}

		VREF<HEAP> fake_m () const leftvalue {
			const auto r1x = address (heap_root ()) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<HEAP>>::expr] (unsafe_pointer (r1x))) ;
		}

		imports CREF<Box<HEAP>> heap_root () {
			return memorize ([&] () {
				return Box<HEAP>::make () ;
			}) ;
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
trait AUTO_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename AUTO_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		FLAG mCabi ;
		Unknown mUnknown ;

	public:
		void initialize (RREF<Unknown> unknown) override {
			mCabi = ZERO ;
			mUnknown = move (unknown) ;
		}

		void finalize () override {
			if (mCabi == ZERO)
				return ;
			mUnknown.recycle (0) ;
			mCabi = ZERO ;
		}

		void acquire (CREF<FLAG> cabi ,CREF<TEMP<void>> obj) override {
			assert (mCabi == ZERO) ;
			mUnknown.acquire (0 ,obj) ;
			mCabi = cabi ;
		}

		void release () override {
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
		csc_span_t mText ;
		LENGTH mSize ;

	public:
		void initialize (CREF<csc_span_t> text) override {
			mText = text ;
			mSize = text_size () ;
		}

		void initialize (RREF<CRef<Holder>> prefix ,CREF<csc_span_t> text) override {
			mPrefix = move (prefix) ;
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
			if (index < r1x)
				return mPrefix->at (index) ;
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
			return operator_compr (size () ,that.size ()) ;
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
			if (mCabi == that.mCabi)
				return TRUE ;
			if (mName != that.mName)
				return FALSE ;
			if (mSize != that.mSize)
				return FALSE ;
			if (mAlign != that.mAlign)
				return FALSE ;
			return TRUE ;
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