#pragma once

#ifndef __CSC_CORE__
#error "∑(っ°Д° ;)っ : require 'csc_core.hpp'"
#endif

#include "csc_core.hpp"

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
template <class...>
trait FUNCTION_current_usage_size_HELP ;

template <class DEPEND>
trait FUNCTION_current_usage_size_HELP<DEPEND ,REQUIRE<MACRO_SYSTEM_WINDOWS<DEPEND>>> {
#ifdef __CSC_SYSTEM_WINDOWS__
	struct FUNCTION_current_usage_size {
		inline LENGTH operator() (CREF<FLAG> addr) const {
			if (addr == ZERO)
				return ZERO ;
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
				return ZERO ;
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
trait AUTO_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename AUTO_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Unknown mUnknown ;

	public:
		void initialize (RREF<Unknown> unknown) override {
			mUnknown = move (unknown) ;
		}

		void acquire (CREF<TEMP<void>> obj) override {
			mUnknown.acquire (0 ,obj) ;
		}

		void destroy () override {
			mUnknown.destroy (0) ;
		}

		FLAG pointer () const override {
			return mUnknown.pointer () ;
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
	using RANK = RANK3 ;

	struct NODE {
		FLAG mPointer ;
		LENGTH mSize ;
		LENGTH mAlign ;
	} ;

	class ImplHolder implement Holder {
	protected:
		Box<ARR<NODE ,RANK>> mSlice ;
		LENGTH mCount ;

	public:
		void initialize (CREF<Variadic<csc_text_t>> text) override {
			mSlice = Box<ARR<NODE ,RANK>>::make () ;
			mCount = 0 ;
			write_text (0 ,text) ;
		}

		void write_text (CREF<INDEX> curr ,CREF<Variadic<csc_text_t>> text) {
			if (text.empty ())
				return ;
			assert (curr < RANK::expr) ;
			if ifswitch (TRUE) {
				if (text.one ().mStep == 1)
					discard ;
				assert (text.one ().mStep == ALIGN_OF<ITEM>::expr) ;
			}
			INDEX jx = text.one ().mBegin ;
			while (TRUE) {
				if (jx >= text.one ().mEnd)
					break ;
				const auto r2x = at_impl (text.one ().mStep ,jx) ;
				if (r2x == ITEM (0))
					break ;
				jx += text.one ().mStep ;
			}
			mSlice.self[curr].mPointer = text.one ().mBegin ;
			mSlice.self[curr].mSize = (jx - text.one ().mBegin) / text.one ().mStep ;
			mSlice.self[curr].mAlign = text.one ().mStep ;
			mCount += mSlice.self[curr].mSize ;
			write_text (curr + 1 ,text.rest ()) ;
		}

		LENGTH size () const override {
			return mCount ;
		}

		ITEM at (CREF<INDEX> index) const override {
			assert (vbetween (index ,0 ,size ())) ;
			INDEX ix = index ;
			for (auto &&i : iter (0 ,RANK::expr)) {
				if ifswitch (TRUE) {
					if ifnot (vbetween (ix ,0 ,mSlice.self[i].mSize))
						discard ;
					const auto r1x = mSlice.self[i].mPointer + ix * mSlice.self[i].mAlign ;
					return at_impl (mSlice.self[i].mAlign ,r1x) ;
				}
				ix -= mSlice.self[i].mSize ;
			}
			return bad (TYPEAS<ITEM>::expr) ;
		}

		ITEM at_impl (CREF<LENGTH> align_ ,CREF<FLAG> pointer) const {
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
		LENGTH mTypeSize ;
		LENGTH mTypeAlign ;
		FLAG mTypeCabi ;
		Slice<STR> mTypeName ;

	public:
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

		BOOL equal (CREF<Holder> that) const override {
			return equal (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		BOOL equal (CREF<ImplHolder> that) const {
			return mTypeCabi == that.mTypeCabi ;
		}

		FLAG compr (CREF<Holder> that) const override {
			return compr (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		FLAG compr (CREF<ImplHolder> that) const {
			const auto r1x = operator_compr (mTypeCabi ,that.mTypeCabi) ;
			if (r1x == ZERO)
				return r1x ;
			const auto r2x = operator_compr (mTypeName ,that.mTypeName) ;
			if (r2x != ZERO)
				return r2x ;
			return r1x ;
		}

		FLAG hash () const override {
			return mTypeName.hash () ;
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