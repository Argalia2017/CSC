#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING A,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
template <class...>
trait BOOLPROXY_HELP ;

template <class DEPEND>
trait BOOLPROXY_HELP<DEPEND ,ALWAYS> {
	class BoolProxy {
	protected:
		BOOL mBool ;

	public:
		implicit BoolProxy () = delete ;

		implicit BoolProxy (CREF<BOOL> that) {
			mBool = that ;
		}

		template <class ARG1>
		implicit BoolProxy (CREF<csc_initializer_t<ARG1>>) = delete ;

		inline implicit operator BOOL () const {
			return mBool ;
		}
	} ;
} ;

using BoolProxy = typename BOOLPROXY_HELP<DEPEND ,ALWAYS>::BoolProxy ;

template <class...>
trait OPTIONAL_HELP ;

template <class...>
trait OPTIONAL_HOLDER_HELP ;

template <class A>
trait OPTIONAL_HOLDER_HELP<A ,ALWAYS> {
	class Optional {
	protected:
		Box<A> mValue ;
		BOOL mGood ;
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mGood = FALSE ;
			mCode = ZERO ;
		}
	} ;
} ;

template <class A>
trait OPTIONAL_HELP<A ,ALWAYS> {
	using Super = typename OPTIONAL_HOLDER_HELP<A ,ALWAYS>::Optional ;

	class Optional implement Super {
	protected:
		using Super::mValue ;
		using Super::mGood ;
		using Super::mCode ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<FLAG> that) {
			mCode = that ;
		}

		template <class...ARG1>
		imports Optional make (XREF<ARG1>...a) {
			Optional ret ;
			ret.mValue = Box<A>::make (keep[TYPE<ARG1>::expr] (a)...) ;
			ret.mGood = TRUE ;
			return move (ret) ;
		}

		BOOL good () const {
			return mGood ;
		}

		FLAG code () const {
			return mCode ;
		}

		A poll () {
			assume (good ()) ;
			A ret = move (mValue.self) ;
			mValue = NULL ;
			mGood = FALSE ;
			return move (ret) ;
		}

		A fetch () const {
			require (IS_CLONEABLE<A>) ;
			return mValue.self ;
		}

		void store (CREF<A> a) {
			require (IS_CLONEABLE<A>) ;
			mValue.self = a ;
		}

		A exchange (CREF<A> a) {
			require (IS_CLONEABLE<A>) ;
			A ret = mValue.self ;
			mValue.self = a ;
			return move (ret) ;
		}
	} ;
} ;

template <class A>
using Optional = typename OPTIONAL_HELP<A ,ALWAYS>::Optional ;

template <class...>
trait BUFFERPROC_HELP ;

template <class...>
trait BUFFERPROC_HOLDER_HELP ;

template <class...>
trait BUFFERPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BUFFERPROC_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
	} ;
} ;

template <class A>
trait BUFFERPROC_HELP<A ,ALWAYS> {
	using Holder = typename BUFFERPROC_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class BufferProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<BufferProc> instance () {
			return memorize ([&] () {
				BufferProc ret ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports BOOL buf_equal (CREF<ARR<A>> src1 ,CREF<ARR<A>> src2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (begin_ ,end_)) {
				if ifnot (operator_equal (src1[i] ,src2[i]))
					return FALSE ;
			}
			return TRUE ;
		}

		imports FLAG buf_compr (CREF<ARR<A>> src1 ,CREF<ARR<A>> src2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (begin_ ,end_)) {
				const auto r1x = operator_compr (src1[i] ,src2[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
		}

		imports FLAG buf_hash (CREF<ARR<A>> src ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			FLAG ret = hashcode () ;
			for (auto&& i : iter (begin_ ,end_)) {
				const auto r1x = operator_hash (src[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		imports void buf_copy (VREF<ARR<A>> dst ,CREF<ARR<A>> src ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (begin_ ,end_))
				dst[i] = src[i] ;
		}

		imports void buf_swap (VREF<ARR<A>> dst1 ,VREF<ARR<A>> dst2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (begin_ ,end_))
				swap (dst1[i] ,dst2[i]) ;
		}

		imports void buf_fill (VREF<ARR<A>> dst ,CREF<A> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (begin_ ,end_))
				dst[i] = item ;
		}

		imports INDEX buf_find (CREF<ARR<A>> src ,CREF<A> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (begin_ ,end_)) {
				if (operator_equal (src[i] ,item))
					return i ;
			}
			return NONE ;
		}

		imports INDEX buf_find_r (CREF<ARR<A>> src ,CREF<A> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto&& i : iter (0 ,end_ - begin_)) {
				INDEX ix = end_ - 1 - i ;
				if (operator_equal (src[ix] ,item))
					return ix ;
			}
			return NONE ;
		}

		template <class ARG1 = A ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRA ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPE<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = A ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRW ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPE<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = A ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRU8 ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPE<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = A ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRU16 ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPE<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = A ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRU32 ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPE<ARG1>::expr ,src ,size_) ;
		}

	private:
		template <class ARG1 ,class ARG2>
		imports inline Slice<ARG1> template_buf_slice (TYPEID<ARG1> id ,CREF<ARG2> src ,CREF<LENGTH> size_) {
			using R1X = ARRAY_ITEM<ARG2> ;
			using R2X = typename BUFFERPROC_HELP<R1X ,ALWAYS>::BufferProc ;
			const auto r1x = invoke ([&] () {
				const auto r2x = R2X::buf_find (src ,R1X (0) ,0 ,size_) ;
				if (r2x != NONE)
					return r2x ;
				return size_ ;
			}) ;
			auto rax = csc_span_t () ;
			rax.mBegin = address (src) ;
			rax.mEnd = rax.mBegin + r1x * ALIGN_OF<R1X>::expr ;
			rax.mStep = ALIGN_OF<R1X>::expr ;
			return Slice<ARG1> (rax) ;
		}
	} ;
} ;

template <class A>
using BufferProc = typename BUFFERPROC_HELP<A ,ALWAYS>::BufferProc ;

template <class...>
trait FUNCTION_tuple_pick_HELP ;

template <class PARAMS>
trait FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS> {
	struct FUNCTION_tuple_pick {
		template <class ARG1 ,class ARG2>
		inline VREF<TYPE_PICK<PARAMS ,ARG2>> operator() (VREF<ARG1> tuple ,TYPEID<ARG2> id) const noexcept {
			return template_pick (PHX ,tuple ,id) ;
		}

		template <class ARG1 ,class ARG2>
		inline CREF<TYPE_PICK<PARAMS ,ARG2>> operator() (CREF<ARG1> tuple ,TYPEID<ARG2> id) const noexcept {
			return template_pick (PHX ,tuple ,id) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG2>>>
		imports inline VREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,VREF<ARG1> tuple ,TYPEID<ARG2> id) {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPE<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		imports inline VREF<TYPE_M1ST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,VREF<ARG1> tuple ,TYPEID<ARG2> id) {
			return tuple.one () ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG2>>>
		imports inline CREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,CREF<ARG1> tuple ,TYPEID<ARG2> id) {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPE<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		imports inline CREF<TYPE_M1ST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,CREF<ARG1> tuple ,TYPEID<ARG2> id) {
			return tuple.one () ;
		}
	} ;
} ;

template <class...>
trait TUPLE_HELP ;

template <class A>
trait TUPLE_HELP<A ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<A>>>> {
	class Tuple {
	public:
		implicit Tuple () = default ;

		imports CREF<Tuple> zero () {
			return memorize ([&] () {
				return Tuple () ;
			}) ;
		}

		void assign () {
			noop () ;
		}

		LENGTH rank () const {
			return 0 ;
		}

		BOOL equal (CREF<Tuple> that) const {
			return TRUE ;
		}

		inline BOOL operator== (CREF<Tuple> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Tuple> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Tuple> that) const {
			return ZERO ;
		}

		inline BOOL operator< (CREF<Tuple> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Tuple> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Tuple> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Tuple> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return hashcode () ;
		}
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQ_IDEN<COUNT_OF<PARAMS>>>> {
	using M1ST_ONE = TYPE_M1ST_ONE<PARAMS> ;
	require (IS_DEFAULT<M1ST_ONE>) ;
	using Super = typename TUPLE_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::Tuple ;

	struct PACK {
		M1ST_ONE mOne ;
	} ;

	class Tuple {
	protected:
		PACK mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Tuple ,ARG1>>...>>>
		explicit Tuple (XREF<ARG1>...a) {
			assign (keep[TYPE<ARG1>::expr] (a)...) ;
		}

		void assign () {
			noop () ;
		}

		void assign (CREF<M1ST_ONE> a) {
			one () = move (a) ;
		}

		void assign (RREF<M1ST_ONE> a) {
			one () = move (a) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		VREF<M1ST_ONE> one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<M1ST_ONE> one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<Super> rest () leftvalue {
			return Super::zero () ;
		}

		CREF<Super> rest () const leftvalue {
			return Super::zero () ;
		}

		VREF<M1ST_ONE> m1st_m () leftvalue {
			return one () ;
		}

		CREF<M1ST_ONE> m1st_m () const leftvalue {
			return one () ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,ARG1>> pick (TYPEID<ARG1> id) leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,ARG1>> pick (TYPEID<ARG1> id) const leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		BOOL equal (CREF<Tuple> that) const {
			if ifnot (operator_equal (one () ,that.one ()))
				return FALSE ;
			if ifnot (operator_equal (rest () ,that.rest ()))
				return FALSE ;
			return TRUE ;
		}

		inline BOOL operator== (CREF<Tuple> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Tuple> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Tuple> that) const {
			const auto r1x = operator_compr (one () ,that.one ()) ;
			if (r1x != ZERO)
				return r1x ;
			const auto r2x = operator_compr (rest () ,that.rest ()) ;
			if (r2x != ZERO)
				return r2x ;
			return ZERO ;
		}

		inline BOOL operator< (CREF<Tuple> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Tuple> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Tuple> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Tuple> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			const auto r1x = operator_hash (one ()) ;
			const auto r2x = operator_hash (rest ()) ;
			return hashcode (r1x ,r2x) ;
		}
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK2>>> {
	using M1ST_ONE = TYPE_M1ST_ONE<PARAMS> ;
	require (IS_DEFAULT<M1ST_ONE>) ;
	using M2ND_ONE = TYPE_M2ND_ONE<PARAMS> ;
	using M1ST_REST = typename TUPLE_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::PACK ;
	using Super = typename TUPLE_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::Tuple ;

	struct PACK {
		M1ST_ONE mOne ;
		M1ST_REST mRest ;
	} ;

	class Tuple {
	protected:
		PACK mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Tuple ,ARG1>>...>>>
		explicit Tuple (XREF<ARG1>...a) {
			assign (keep[TYPE<ARG1>::expr] (a)...) ;
		}

		void assign () {
			noop () ;
		}

		template <class...ARG1>
		void assign (CREF<M1ST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (keep[TYPE<ARG1>::expr] (obj2)...) ;
		}

		template <class...ARG1>
		void assign (RREF<M1ST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (keep[TYPE<ARG1>::expr] (obj2)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		VREF<M1ST_ONE> one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<M1ST_ONE> one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<Super> rest () leftvalue {
			return unsafe_cast[TYPE<Super>::expr] (mTuple.mRest) ;
		}

		CREF<Super> rest () const leftvalue {
			return unsafe_cast[TYPE<Super>::expr] (mTuple.mRest) ;
		}

		VREF<M1ST_ONE> m1st_m () leftvalue {
			return mTuple.mOne ;
		}

		CREF<M1ST_ONE> m1st_m () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<M2ND_ONE> m2nd_m () leftvalue {
			return mTuple.mRest.mOne ;
		}

		CREF<M2ND_ONE> m2nd_m () const leftvalue {
			return mTuple.mRest.mOne ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,ARG1>> pick (TYPEID<ARG1> id) leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,ARG1>> pick (TYPEID<ARG1> id) const leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		BOOL equal (CREF<Tuple> that) const {
			if ifnot (operator_equal (one () ,that.one ()))
				return FALSE ;
			if ifnot (operator_equal (rest () ,that.rest ()))
				return FALSE ;
			return TRUE ;
		}

		inline BOOL operator== (CREF<Tuple> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Tuple> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Tuple> that) const {
			const auto r1x = operator_compr (one () ,that.one ()) ;
			if (r1x != ZERO)
				return r1x ;
			const auto r2x = operator_compr (rest () ,that.rest ()) ;
			if (r2x != ZERO)
				return r2x ;
			return ZERO ;
		}

		inline BOOL operator< (CREF<Tuple> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Tuple> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Tuple> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Tuple> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			const auto r1x = operator_hash (one ()) ;
			const auto r2x = operator_hash (rest ()) ;
			return hashcode (r1x ,r2x) ;
		}
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_COMPR_GT<COUNT_OF<PARAMS> ,RANK2>>> {
	using M1ST_ONE = TYPE_M1ST_ONE<PARAMS> ;
	require (IS_DEFAULT<M1ST_ONE>) ;
	using M2ND_ONE = TYPE_M2ND_ONE<PARAMS> ;
	using M3RD_ONE = TYPE_M3RD_ONE<PARAMS> ;
	using M1ST_REST = typename TUPLE_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::PACK ;
	using Super = typename TUPLE_HELP<TYPE_M1ST_REST<PARAMS> ,ALWAYS>::Tuple ;

	struct PACK {
		M1ST_ONE mOne ;
		M1ST_REST mRest ;
	} ;

	class Tuple {
	protected:
		PACK mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Tuple ,ARG1>>...>>>
		explicit Tuple (XREF<ARG1>...a) {
			assign (keep[TYPE<ARG1>::expr] (a)...) ;
		}

		void assign () {
			noop () ;
		}

		template <class...ARG1>
		void assign (CREF<M1ST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (keep[TYPE<ARG1>::expr] (obj2)...) ;
		}

		template <class...ARG1>
		void assign (RREF<M1ST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (keep[TYPE<ARG1>::expr] (obj2)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		VREF<M1ST_ONE> one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<M1ST_ONE> one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<Super> rest () leftvalue {
			return unsafe_cast[TYPE<Super>::expr] (mTuple.mRest) ;
		}

		CREF<Super> rest () const leftvalue {
			return unsafe_cast[TYPE<Super>::expr] (mTuple.mRest) ;
		}

		VREF<M1ST_ONE> m1st_m () leftvalue {
			return mTuple.mOne ;
		}

		CREF<M1ST_ONE> m1st_m () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<M2ND_ONE> m2nd_m () leftvalue {
			return mTuple.mRest.mOne ;
		}

		CREF<M2ND_ONE> m2nd_m () const leftvalue {
			return mTuple.mRest.mOne ;
		}

		VREF<M3RD_ONE> m3rd_m () leftvalue {
			return mTuple.mRest.mRest.mOne ;
		}

		CREF<M3RD_ONE> m3rd_m () const leftvalue {
			return mTuple.mRest.mRest.mOne ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,ARG1>> pick (TYPEID<ARG1> id) leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,ARG1>> pick (TYPEID<ARG1> id) const leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		BOOL equal (CREF<Tuple> that) const {
			if ifnot (operator_equal (one () ,that.one ()))
				return FALSE ;
			if ifnot (operator_equal (rest () ,that.rest ()))
				return FALSE ;
			return TRUE ;
		}

		inline BOOL operator== (CREF<Tuple> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Tuple> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Tuple> that) const {
			const auto r1x = operator_compr (one () ,that.one ()) ;
			if (r1x != ZERO)
				return r1x ;
			const auto r2x = operator_compr (rest () ,that.rest ()) ;
			if (r2x != ZERO)
				return r2x ;
			return ZERO ;
		}

		inline BOOL operator< (CREF<Tuple> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Tuple> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Tuple> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Tuple> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			const auto r1x = operator_hash (one ()) ;
			const auto r2x = operator_hash (rest ()) ;
			return hashcode (r1x ,r2x) ;
		}
	} ;
} ;

template <class...A>
using Tuple = typename TUPLE_HELP<TYPE<A...> ,ALWAYS>::Tuple ;

template <class...>
trait FUNCTION_FORWARD_EACH_HELP ;

template <class A>
trait FUNCTION_FORWARD_EACH_HELP<A ,REQUIRE<IS_VARIABLE<A>>> {
	using RET = A ;
} ;

template <class A>
trait FUNCTION_FORWARD_EACH_HELP<A ,REQUIRE<IS_CONSTANT<A>>> {
	using RET = A ;
} ;

template <class A>
trait FUNCTION_FORWARD_EACH_HELP<A ,REQUIRE<IS_REGISTER<A>>> {
	using RET = A ;
} ;

template <class A>
trait FUNCTION_FORWARD_EACH_HELP<A ,REQUIRE<IS_ORDINARY<A>>> {
	using RET = CREF<REMOVE_REF<A>> ;
} ;

template <class A>
using FUNCTION_FORWARD_EACH = typename FUNCTION_FORWARD_EACH_HELP<A ,ALWAYS>::RET ;

template <class...>
trait FUNCTION_FORWARD_HELP ;

template <class...PARAM>
trait FUNCTION_FORWARD_HELP<TYPE<PARAM...> ,ALWAYS> {
	using RET = TYPE<FUNCTION_FORWARD_EACH<PARAM>...> ;
} ;

template <class PARAMS>
using FUNCTION_FORWARD = typename FUNCTION_FORWARD_HELP<PARAMS ,ALWAYS>::RET ;

template <class...>
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_WRAPPER_HELP ;

template <class...>
trait FUNCTION_PUREHOLDER_HELP ;

template <class...PARAM>
trait FUNCTION_HELP<TYPE<PARAM...> ,ALWAYS> {
	using PARAMS = TYPE<PARAM...> ;

	struct Holder implement Interface {
		virtual void initialize (CREF<BoxBase> value_) = 0 ;
		virtual BOOL effective () const = 0 ;
		virtual void invoke (XREF<PARAM>...params) const = 0 ;
	} ;

	class Function {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Function () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Function ,ARG1>>>>>
		implicit Function (RREF<ARG1> that) {
			require (IS_FUNCTION<ARG1>) ;
			require (IS_VOID<FUNCTION_RETURN<ARG1>>) ;
			using R2X = typename FUNCTION_PUREHOLDER_HELP<PARAMS ,ARG1 ,ALWAYS>::PureHolder ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (IS_EFFECTIVE<ARG1>::expr)
					discard ;
				auto rax = VRef<Holder> (VRef<R2X>::make ()) ;
				auto rbx = Box<ARG1>::make (that) ;
				rax->initialize (rbx) ;
				rbx.release () ;
				mThis = move (rax) ;
			}
			if ifswitch (act) {
				auto rax = VRef<Holder> (VRef<R2X>::make ()) ;
				auto rbx = Box<ARG1>::make (that) ;
				rax->initialize (rbx) ;
				rbx.release () ;
				mThis = move (rax) ;
			}
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;

		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		BOOL effective () const {
			if ifnot (exist ())
				return FALSE ;
			return mThis->effective () ;
		}

		void invoke (XREF<PARAM>...params) const {
			if (mThis == NULL)
				return ;
			return mThis->invoke (keep[TYPE<PARAM>::expr] (params)...) ;
		}

		inline void operator() (XREF<PARAM>...params) const {
			return invoke (keep[TYPE<PARAM>::expr] (params)...) ;
		}
	} ;
} ;

template <class...PARAM ,class A>
trait FUNCTION_PUREHOLDER_HELP<TYPE<PARAM...> ,A ,ALWAYS> {
	using PARAMS = TYPE<PARAM...> ;
	using Holder = typename FUNCTION_HELP<PARAMS ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		Box<A> mValue ;

	public:
		void initialize (CREF<BoxBase> value_) override {
			mValue.acquire (value_) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<A>::expr ;
		}

		void invoke (XREF<PARAM>...params) const override {
			return mValue.self (keep[TYPE<PARAM>::expr] (params)...) ;
		}
	} ;
} ;

template <class PARAMS = TYPE<>>
using Function = typename FUNCTION_HELP<FUNCTION_FORWARD<PARAMS> ,ALWAYS>::Function ;

template <class...>
trait SCOPEFINALLY_HELP ;

template <class DEPEND>
trait SCOPEFINALLY_HELP<DEPEND ,ALWAYS> {
	class ScopeFinally implement Proxy {
	protected:
		Function<TYPE<>> mThat ;

	public:
		imports CREF<ScopeFinally> from (CREF<Function<TYPE<>>> that) {
			return unsafe_cast[TYPE<ScopeFinally>::expr] (that) ;
		}

		imports CREF<ScopeFinally> from (RREF<Function<TYPE<>>>) = delete ;

		void enter () {
			assert (mThat.exist ()) ;
		}

		void leave () {
			mThat.invoke () ;
		}
	} ;
} ;

using ScopeFinally = typename SCOPEFINALLY_HELP<DEPEND ,ALWAYS>::ScopeFinally ;

template <class...>
trait SCOPECOUNTER_HELP ;

template <class DEPEND>
trait SCOPECOUNTER_HELP<DEPEND ,ALWAYS> {
	class ScopeCounter implement Proxy {
	protected:
		LENGTH mThat ;

	public:
		imports VREF<ScopeCounter> from (VREF<LENGTH> that) {
			return unsafe_cast[TYPE<ScopeCounter>::expr] (that) ;
		}

		void enter () {
			mThat++ ;
		}

		void leave () {
			mThat-- ;
		}
	} ;
} ;

using ScopeCounter = typename SCOPECOUNTER_HELP<DEPEND ,ALWAYS>::ScopeCounter ;

template <class...>
trait AUTOREF_HELP ;

template <class...>
trait AUTOREF_HOLDER_HELP ;

template <class...>
trait AUTOREF_PUREHOLDER_HELP ;

template <class DEPEND>
trait AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual Clazz clazz () const = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	struct Layout {
		VRef<Holder> mThis ;
		Scope<Holder> mHandle ;
		FLAG mPointer ;
	} ;

	class AutoRef implement Layout {
	protected:
		using Layout::mThis ;
		using Layout::mHandle ;
		using Layout::mPointer ;

	public:
		implicit AutoRef () noexcept {
			mPointer = ZERO ;
		}
	} ;
} ;

template <class A>
trait AUTOREF_HELP<A ,REQUIRE<IS_VOID<A>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Super = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::AutoRef ;

	class AutoRef implement Super {
	protected:
		using Super::mThis ;
		using Super::mHandle ;
		using Super::mPointer ;

	public:
		implicit AutoRef () = default ;

		implicit AutoRef (RREF<Layout> that) {
			mThis = move (that.mThis) ;
			mHandle = move (that.mHandle) ;
			if ifswitch (TRUE) {
				that.mPointer = ZERO ;
				if ifnot (good ())
					discard ;
				mPointer = mThis->pointer () ;
			}
		}

		imports VREF<AutoRef> from (VREF<Layout> that) {
			auto&& tmp1 = keep[TYPE<VREF<AutoRef>>::expr] (that) ;
			assume (tmp1.good ()) ;
			return tmp1 ;
		}

		imports CREF<AutoRef> from (CREF<Layout> that) {
			auto&& tmp1 = keep[TYPE<CREF<AutoRef>>::expr] (that) ;
			assume (tmp1.good ()) ;
			return tmp1 ;
		}

		imports CREF<AutoRef> from (RREF<Layout>) = delete ;

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;

		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL good () const {
			if (mThis == NULL)
				return FALSE ;
			return TRUE ;
		}

		Clazz clazz () const {
			if ifnot (exist ())
				return Clazz () ;
			return mThis->clazz () ;
		}
	} ;
} ;

template <class A>
trait AUTOREF_HELP<A ,REQUIRE<ENUM_NOT<IS_VOID<A>>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Super = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::AutoRef ;

	class AutoRef implement Super {
	protected:
		using Super::mThis ;
		using Super::mHandle ;
		using Super::mPointer ;

	public:
		implicit AutoRef () = default ;

		implicit AutoRef (RREF<Layout> that) {
			mThis = move (that.mThis) ;
			mHandle = move (that.mHandle) ;
			if ifswitch (TRUE) {
				that.mPointer = ZERO ;
				if ifnot (good ())
					discard ;
				mPointer = mThis->pointer () ;
			}
		}

		imports VREF<AutoRef> from (VREF<Layout> that) {
			auto&& tmp1 = keep[TYPE<VREF<AutoRef>>::expr] (that) ;
			assume (tmp1.good ()) ;
			return tmp1 ;
		}

		imports CREF<AutoRef> from (CREF<Layout> that) {
			auto&& tmp1 = keep[TYPE<CREF<AutoRef>>::expr] (that) ;
			assume (tmp1.good ()) ;
			return tmp1 ;
		}

		imports CREF<AutoRef> from (RREF<Layout>) = delete ;

		template <class...ARG1>
		imports AutoRef make (XREF<ARG1>...a) {
			using R1X = KILL<A ,TYPE<ARG1...>> ;
			using R2X = typename KILL<AUTOREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			AutoRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			ret.mThis->initialize () ;
			ret.mPointer = ret.mThis->pointer () ;
			auto&& tmp1 = unsafe_cast[TYPE<Box<R1X>>::expr] (unsafe_pointer (ret.mPointer)) ;
			tmp1.remake (TYPE<R1X>::expr ,keep[TYPE<ARG1>::expr] (a)...) ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;

		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL good () const {
			if (mThis == NULL)
				return FALSE ;
			if (mThis->clazz () != Clazz (TYPE<A>::expr))
				return FALSE ;
			return TRUE ;
		}

		Clazz clazz () const {
			if ifnot (exist ())
				return Clazz () ;
			return mThis->clazz () ;
		}

		VREF<A> self_m () leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline implicit operator VREF<A> () leftvalue {
			return self ;
		}

		inline VPTR<A> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<A> self_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline implicit operator CREF<A> () const leftvalue {
			return self ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}
	} ;
} ;

template <class A>
trait AUTOREF_PUREHOLDER_HELP<A ,ALWAYS> {
	using Holder = typename AUTOREF_HELP<A ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		Box<A> mValue ;

	public:
		void initialize () override {
			noop () ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPE<A>::expr) ;
		}

		void enter () override {
			noop () ;
		}

		void leave () override {
			//@warn: pin it's address
			drop (mValue) ;
		}
	} ;
} ;

template <class A = void>
using AutoRef = typename AUTOREF_HELP<A ,ALWAYS>::AutoRef ;

template <class...>
trait PINMUTEX_HELP ;

template <class...>
trait PINMUTEX_IMPLHOLDER_HELP ;

template <class DEPEND>
trait PINMUTEX_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	using FAKE_MAX_SIZE = ENUM<128> ;
	using FAKE_MAX_ALIGN = RANK8 ;

	class FakeHolder implement Holder {
	protected:
		Storage<FAKE_MAX_SIZE ,FAKE_MAX_ALIGN> mStorage ;
	} ;

	struct FakeImplHolder {
		imports Box<FakeHolder> create () ;
	} ;

	class PinMutex {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit PinMutex () = default ;

		void pin () {
			mThis = FakeImplHolder::create () ;
			mThis->initialize () ;
		}

		void enter () {
			return mThis->enter () ;
		}

		void leave () {
			return mThis->leave () ;
		}
	} ;
} ;

using PinMutex = typename PINMUTEX_HELP<DEPEND ,ALWAYS>::PinMutex ;

template <class...>
trait SHAREDREF_HELP ;

template <class...>
trait SHAREDREF_HOLDER_HELP ;

template <class...>
trait SHAREDREF_PUREHOLDER_HELP ;

template <class DEPEND>
trait SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual CREF<PinMutex> pin_mutex () const = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual LENGTH counter () const = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	class SharedRef {
	protected:
		CRef<Holder> mThis ;
		Scope<Holder> mHandle ;
		FLAG mPointer ;

	public:
		implicit SharedRef () noexcept {
			mPointer = ZERO ;
		}

		implicit ~SharedRef () noexcept {
			if (mThis == NULL)
				return ;
			auto rax = VRef<Holder> () ;
			if ifswitch (TRUE) {
				//@warn: check Singleton
				const auto r1x = interface_vptr (mThis.self) ;
				if (r1x == ZERO)
					discard ;
				Scope<PinMutex> anonymous (mThis->pin_mutex ()) ;
				rax = move (mThis) ;
				mHandle = Scope<Holder> () ;
				mThis = NULL ;
			}
		}
	} ;
} ;

template <class A>
trait SHAREDREF_HELP<A ,ALWAYS> {
	using Holder = typename SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Super = typename SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS>::SharedRef ;

	class SharedRef implement Super {
	protected:
		using Super::mThis ;
		using Super::mHandle ;
		using Super::mPointer ;

	public:
		implicit SharedRef () = default ;

		template <class...ARG1>
		imports SharedRef make (XREF<ARG1>...a) {
			using R1X = KILL<A ,TYPE<ARG1...>> ;
			using R2X = typename KILL<SHAREDREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			SharedRef ret ;
			auto rax = VRef<Holder> (VRef<R2X>::make ()) ;
			rax->initialize () ;
			ret.mThis = move (rax) ;
			ret.mPointer = ret.mThis->pointer () ;
			auto&& tmp1 = unsafe_cast[TYPE<Box<R1X>>::expr] (unsafe_pointer (ret.mPointer)) ;
			tmp1.remake (TYPE<R1X>::expr ,keep[TYPE<ARG1>::expr] (a)...) ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		template <class ARG1 = CRef<Holder>>
		imports SharedRef intrusive (RREF<ARG1> a) {
			using R1X = KILL<A ,ARG1> ;
			assert (a != NULL) ;
			SharedRef ret ;
			Scope<PinMutex> anonymous (a->pin_mutex ()) ;
			ret.mThis = move (a) ;
			ret.mPointer = ret.mThis->pointer () ;
			if ifswitch (TRUE) {
				if (ret.mThis->counter () > 0)
					discard ;
				auto&& tmp1 = unsafe_cast[TYPE<Box<R1X>>::expr] (unsafe_pointer (ret.mPointer)) ;
				tmp1.remake (TYPE<R1X>::expr) ;
			}
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		implicit SharedRef (CREF<SharedRef> that) {
			if (that.mThis == NULL)
				return ;
			Scope<PinMutex> anonymous (that.mThis->pin_mutex ()) ;
			mThis = that.mThis ;
			mPointer = mThis->pointer () ;
			if ifnot (that.good ())
				return ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		inline VREF<SharedRef> operator= (CREF<SharedRef> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit SharedRef (RREF<SharedRef> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<SharedRef> operator= (RREF<SharedRef> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;

		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL good () const {
			if (mThis == NULL)
				return FALSE ;
			if (mThis->counter () == ZERO)
				return FALSE ;
			return TRUE ;
		}

		VREF<A> self_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline implicit operator VREF<A> () const leftvalue {
			return self ;
		}

		inline VPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}

		SharedRef weak () const {
			SharedRef ret ;
			if ifswitch (TRUE) {
				if (mThis == NULL)
					discard ;
				Scope<PinMutex> anonymous (mThis->pin_mutex ()) ;
				ret.mThis = mThis ;
				ret.mPointer = mPointer ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class A>
trait SHAREDREF_PUREHOLDER_HELP<A ,ALWAYS> {
	using Holder = typename SHAREDREF_HELP<A ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		PinMutex mMutex ;
		LENGTH mCounter ;
		Box<A> mValue ;

	public:
		void initialize () override {
			mMutex.pin () ;
			mCounter = 0 ;
		}

		CREF<PinMutex> pin_mutex () const override {
			return mMutex ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		LENGTH counter () const override {
			return mCounter ;
		}

		void enter () override {
			mCounter++ ;
		}

		void leave () override {
			mCounter-- ;
			if (mCounter > 0)
				return ;
			//@warn: pin it's address
			drop (mValue) ;
		}
	} ;
} ;

template <class A>
using SharedRef = typename SHAREDREF_HELP<A ,ALWAYS>::SharedRef ;

template <class...>
trait UNIQUEREF_HELP ;

template <class...>
trait UNIQUEREF_HOLDER_HELP ;

template <class...>
trait UNIQUEREF_PUREHOLDER_HELP ;

template <class DEPEND>
trait UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<BoxBase> destructor) = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual BOOL ownership () const = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	class UniqueRef {
	protected:
		VRef<Holder> mThis ;
		Scope<Holder> mHandle ;
		FLAG mPointer ;

	public:
		implicit UniqueRef () noexcept {
			mPointer = ZERO ;
		}
	} ;
} ;

template <class A>
trait UNIQUEREF_HELP<A ,REQUIRE<IS_VOID<A>>> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Super = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::UniqueRef ;

	class UniqueRef implement Super {
	protected:
		using Super::mThis ;
		using Super::mHandle ;
		using Super::mPointer ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (RREF<ARG1> constructor ,RREF<ARG2> destructor) {
			using R1X = KILL<A ,TYPE<ARG1 ,ARG2>> ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<TYPE<>> ;
			mThis = VRef<R2X>::make () ;
			auto rax = Box<R3X>::make (move (destructor)) ;
			assert (ifnot (rax->effective ())) ;
			mThis->initialize (rax) ;
			rax.release () ;
			mPointer = mThis->pointer () ;
			constructor () ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		imports UniqueRef make () {
			using R1X = A ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<TYPE<>> ;
			UniqueRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			auto rax = Box<R3X>::make () ;
			ret.mThis->initialize (rax) ;
			rax.release () ;
			ret.mPointer = ret.mThis->pointer () ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;

		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL good () const {
			if (mThis == NULL)
				return FALSE ;
			if ifnot (mThis->ownership ())
				return FALSE ;
			return TRUE ;
		}
	} ;
} ;

template <class A>
trait UNIQUEREF_HELP<A ,REQUIRE<ENUM_NOT<IS_VOID<A>>>> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Super = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::UniqueRef ;

	class UniqueRef implement Super {
	protected:
		using Super::mThis ;
		using Super::mHandle ;
		using Super::mPointer ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (RREF<ARG1> constructor ,RREF<ARG2> destructor) {
			using R1X = KILL<A ,TYPE<ARG1 ,ARG2>> ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<TYPE<VREF<R1X>>> ;
			mThis = VRef<R2X>::make () ;
			auto rax = Box<R3X>::make (move (destructor)) ;
			assert (ifnot (rax->effective ())) ;
			mThis->initialize (rax) ;
			rax.release () ;
			mPointer = mThis->pointer () ;
			auto&& tmp1 = unsafe_cast[TYPE<Box<R1X>>::expr] (unsafe_pointer (mPointer)) ;
			tmp1.remake (TYPE<R1X>::expr) ;
			constructor (tmp1.self) ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		template <class...ARG1>
		imports UniqueRef make (XREF<ARG1>...a) {
			using R1X = KILL<A ,TYPE<ARG1...>> ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<TYPE<VREF<R1X>>> ;
			UniqueRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			auto rax = Box<R3X>::make () ;
			ret.mThis->initialize (rax) ;
			rax.release () ;
			ret.mPointer = ret.mThis->pointer () ;
			auto&& tmp1 = unsafe_cast[TYPE<Box<R1X>>::expr] (unsafe_pointer (ret.mPointer)) ;
			tmp1.remake (TYPE<R1X>::expr ,keep[TYPE<ARG1>::expr] (a)...) ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;

		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL good () const {
			if (mThis == NULL)
				return FALSE ;
			if ifnot (mThis->ownership ())
				return FALSE ;
			return TRUE ;
		}

		CREF<A> self_m () const leftvalue {
			return unsafe_cast[TYPE<A>::expr] (unsafe_pointer (mPointer)) ;
		}

		inline implicit operator CREF<A> () const leftvalue {
			return self ;
		}

		inline CPTR<A> operator-> () const leftvalue {
			return (&self) ;
		}
	} ;
} ;

template <class A>
trait UNIQUEREF_PUREHOLDER_HELP<A ,REQUIRE<IS_VOID<A>>> {
	using Holder = typename UNIQUEREF_HELP<A ,ALWAYS>::Holder ;

	using Destructor = Function<TYPE<>> ;

	class PureHolder implement Holder {
	protected:
		Box<Destructor> mDestructor ;

	public:
		void initialize (CREF<BoxBase> destructor) override {
			mDestructor.acquire (destructor) ;
		}

		FLAG pointer () const override {
			return USED ;
		}

		BOOL ownership () const override {
			return mDestructor.exist () ;
		}

		void enter () override {
			noop () ;
		}

		void leave () override {
			if ifswitch (TRUE) {
				if ifnot (mDestructor.exist ())
					discard ;
				mDestructor.self () ;
				mDestructor = NULL ;
			}
		}
	} ;
} ;

template <class A>
trait UNIQUEREF_PUREHOLDER_HELP<A ,REQUIRE<ENUM_NOT<IS_VOID<A>>>> {
	using Holder = typename UNIQUEREF_HELP<A ,ALWAYS>::Holder ;

	using Destructor = Function<TYPE<VREF<A>>> ;

	class PureHolder implement Holder {
	protected:
		Box<Destructor> mDestructor ;
		Box<A> mValue ;

	public:
		void initialize (CREF<BoxBase> destructor) override {
			auto rax = VRef<Destructor> (move (destructor)) ;
			mDestructor.acquire (destructor) ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		BOOL ownership () const override {
			return mDestructor.exist () ;
		}

		void enter () override {
			noop () ;
		}

		void leave () override {
			if ifswitch (TRUE) {
				if ifnot (mDestructor.exist ())
					discard ;
				mDestructor (mValue.self) ;
				mDestructor = Destructor () ;
			}
			//@warn: pin it's address
			drop (mValue) ;
		}
	} ;
} ;

template <class A = void>
using UniqueRef = typename UNIQUEREF_HELP<A ,ALWAYS>::UniqueRef ;

template <class...>
trait SIZEPROXY_HELP ;

template <class DEPEND>
trait SIZEPROXY_HELP<DEPEND ,ALWAYS> {
	class SizeProxy {
	protected:
		LENGTH mSize ;

	public:
		implicit SizeProxy () = delete ;

		implicit SizeProxy (CREF<VAL32> that) {
			mSize = LENGTH (that) ;
		}

		implicit SizeProxy (CREF<VAL64> that) {
			mSize = LENGTH (that) ;
		}

		template <class ARG1>
		implicit SizeProxy (CREF<csc_initializer_t<ARG1>>) = delete ;

		inline implicit operator LENGTH () const {
			return mSize ;
		}
	} ;
} ;

using SizeProxy = typename SIZEPROXY_HELP<DEPEND ,ALWAYS>::SizeProxy ;

template <class...>
trait BUFFER_HELP ;

template <class...>
trait BOXBUFFER_HOLDER_HELP ;

template <class...>
trait BOXBUFFER_HOLDER_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,REQUIRE<IS_TRIVIAL<ITEM>>> {
	struct NODE {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	class Buffer {
	protected:
		Box<NODE> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<ITEM>> ,IS_DEFAULT<ITEM>>>> {
	struct NODE {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	class Buffer {
	protected:
		Box<NODE> mBuffer ;

	public:
		implicit Buffer () noexcept {
			mBuffer = Box<NODE>::make () ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_NOT<IS_DEFAULT<ITEM>>>> {
	struct NODE {
		ARR<ITEM ,SIZE> mArray ;
	} ;

	class Buffer {
	protected:
		Box<NODE> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE ,class COND>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	using NODE = typename BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using Super = typename BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE ,class COND>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,COND ,REQUIRE<COND>> {
	using NODE = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::NODE ;
	using Super = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mBuffer == NULL)
				return ;
			mBuffer = Box<NODE>::make (that.mBuffer.self) ;
		}

		inline VREF<Buffer> operator= (CREF<Buffer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Buffer> operator= (RREF<Buffer> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using NODE = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::NODE ;
	using Super = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<ARR<ITEM ,SIZE>> that) {
			mBuffer = Box<NODE>::make (unsafe_cast[TYPE<NODE>::expr] (that)) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			assert (size_ <= SIZE::expr) ;
			mBuffer = Box<NODE>::make () ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return SIZE::expr ;
		}

		VREF<ARR<ITEM>> self_m () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return unsafe_array (mBuffer->mArray[0]) ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return unsafe_array (mBuffer->mArray[0]) ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer->mArray[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer->mArray[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			if (size_ == size ())
				return ;
			assume (FALSE) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
using Buffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE ,class = REQUIRE<ENUM_GT_ZERO<SIZE>>>
using BoxBuffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class...>
trait REGBUFFER_HELP ;

template <class ITEM>
trait REGBUFFER_HELP<ITEM ,ALWAYS> {
	struct Layout {} ;

	struct NODE {
		ARR<ITEM ,RANK1> mArray ;
	} ;
} ;

template <class...>
trait VARBUFFER_HELP ;

template <class ITEM ,class COND>
trait VARBUFFER_HELP<ITEM ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	using Layout = typename REGBUFFER_HELP<ITEM ,ALWAYS>::Layout ;
	using NODE = typename REGBUFFER_HELP<ITEM ,ALWAYS>::NODE ;

	class Buffer implement Layout {
	protected:
		VRef<NODE> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class COND>
trait VARBUFFER_HELP<ITEM ,COND ,REQUIRE<COND>> {
	using Layout = typename VARBUFFER_HELP<ITEM ,ENUM_FALSE ,ALWAYS>::Layout ;
	using NODE = typename VARBUFFER_HELP<ITEM ,ENUM_FALSE ,ALWAYS>::NODE ;
	using Super = typename VARBUFFER_HELP<ITEM ,ENUM_FALSE ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;
		using Super::mSize ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mBuffer == NULL)
				return ;
			mBuffer = VRef<NODE>::make (that.mSize) ;
			BufferProc<ITEM>::buf_copy (mBuffer.self ,that.mBuffer.self ,0 ,that.mSize) ;
			mSize = that.mSize ;
		}

		inline VREF<Buffer> operator= (CREF<Buffer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Buffer> operator= (RREF<Buffer> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,VARIABLE>>> {
	using Layout = typename VARBUFFER_HELP<ITEM ,IS_CLONEABLE<ITEM> ,ALWAYS>::Layout ;
	using Super = typename VARBUFFER_HELP<ITEM ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;
		using Super::mSize ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (RREF<Layout> that) {
			thiz = keep[TYPE<RREF<Buffer>>::expr] (that) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = VRef<NODE>::make (size_) ;
			mSize = size_ ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return mSize ;
		}

		VREF<ARR<ITEM>> self_m () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return unsafe_array (mBuffer.self) ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return unsafe_array (mBuffer.self) ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			if (size_ == size ())
				return ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (mBuffer == NULL)
					discard ;
				if (size_ == 0)
					discard ;
				auto rax = VRef<NODE>::make (size_) ;
				const auto r1x = operator_min (size_ ,mSize) ;
				BufferProc<ITEM>::buf_swap (rax.self ,mBuffer.self ,0 ,r1x) ;
				mBuffer = move (rax) ;
				mSize = size_ ;
			}
			if ifswitch (act) {
				if (size_ == 0)
					discard ;
				mBuffer = VRef<NODE>::make (size_) ;
				mSize = size_ ;
			}
			if ifswitch (act) {
				mBuffer = NULL ;
				mSize = 0 ;
			}
		}
	} ;
} ;

template <class ITEM>
using VarBuffer = typename BUFFER_HELP<ITEM ,VARIABLE ,ALWAYS>::Buffer ;

template <class...>
trait CONBUFFER_HELP ;

template <class ITEM>
trait CONBUFFER_HELP<ITEM ,ALWAYS> {
	using Layout = typename REGBUFFER_HELP<ITEM ,ALWAYS>::Layout ;
	using NODE = typename REGBUFFER_HELP<ITEM ,ALWAYS>::NODE ;

	class Buffer implement Layout {
	protected:
		CRef<NODE> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,CONSTANT>>> {
	using Layout = typename CONBUFFER_HELP<ITEM ,ALWAYS>::Layout ;
	using Super = typename CONBUFFER_HELP<ITEM ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;
		using Super::mSize ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (RREF<Layout> that) {
			thiz = keep[TYPE<RREF<Buffer>>::expr] (that) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = CRef<NODE>::make (size_) ;
			mSize = size_ ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return mSize ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return unsafe_array (mBuffer.self) ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			if (size_ == size ())
				return ;
			assume (FALSE) ;
		}
	} ;
} ;

template <class ITEM>
using ConBuffer = typename BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,REGISTER>>> {
	using Layout = typename REGBUFFER_HELP<ITEM ,ALWAYS>::Layout ;
	using NODE = typename REGBUFFER_HELP<ITEM ,ALWAYS>::NODE ;

	struct RegCaches ;

	class Buffer implement Proxy ,Layout {
	protected:
		VRef<NODE> mBuffer ;
		LENGTH mSize ;

	public:
		imports VREF<Buffer> from (VREF<Layout> that) {
			auto&& tmp1 = keep[TYPE<VREF<Buffer>>::expr] (that) ;
			return unsafe_cast[TYPE<Buffer>::expr] (tmp1) ;
		}

		imports CREF<Buffer> from (CREF<Layout> that) {
			auto&& tmp1 = keep[TYPE<CREF<Buffer>>::expr] (that) ;
			return unsafe_cast[TYPE<Buffer>::expr] (tmp1) ;
		}

		imports CREF<Buffer> from (RREF<Layout>) = delete ;

		imports VREF<Buffer> from (VREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			return from (unsafe_cast[TYPE<ARR<ITEM>>::expr] (buffer) ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports VREF<Buffer> from (VREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			auto&& tmp1 = unsafe_cast[TYPE<Buffer>::expr] (unnamed) ;
			if ifswitch (TRUE) {
				const auto r1x = operator_max (end_ - begin_ ,0) ;
				if (r1x == ZERO)
					discard ;
				tmp1.mBuffer = VRef<NODE>::reference (unsafe_array (buffer[begin_])) ;
				tmp1.mSize = r1x ;
			}
			return tmp1 ;
		}

		imports CREF<Buffer> from (CREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			return from (unsafe_cast[TYPE<ARR<ITEM>>::expr] (buffer) ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports CREF<Buffer> from (CREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			auto&& tmp1 = unsafe_cast[TYPE<Buffer>::expr] (unnamed) ;
			if ifswitch (TRUE) {
				const auto r1x = operator_max (end_ - begin_ ,0) ;
				if (r1x == ZERO)
					discard ;
				tmp1.mBuffer = CRef<NODE>::reference (unsafe_array (buffer[begin_])) ;
				tmp1.mSize = r1x ;
			}
			return tmp1 ;
		}

		VRef<Buffer> borrow () leftvalue {
			VRef<Buffer> ret ;
			auto&& tmp1 = unsafe_cast[TYPE<VRef<RegCaches>>::expr] (ret) ;
			tmp1 = VRef<RegCaches>::make () ;
			unsafe_launder (ret) ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				ret->mBuffer = VRef<NODE>::reference (mBuffer.self) ;
				ret->mSize = mSize ;
			}
			return move (ret) ;
		}

		CRef<Buffer> borrow () const leftvalue {
			VRef<Buffer>  ret ;
			auto&& tmp1 = unsafe_cast[TYPE<VRef<RegCaches>>::expr] (ret) ;
			tmp1 = VRef<RegCaches>::make () ;
			unsafe_launder (ret) ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				ret->mBuffer = CRef<NODE>::reference (mBuffer.self) ;
				ret->mSize = mSize ;
			}
			return move (ret) ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return mSize ;
		}

		VREF<ARR<ITEM>> self_m () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPE<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (operator_between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			if (size_ == size ())
				return ;
			assume (FALSE) ;
		}
	} ;

	struct RegCaches {
		TEMP<Buffer> mUnused ;
	} ;
} ;

template <class ITEM>
using RegBuffer = typename BUFFER_HELP<ITEM ,REGISTER ,ALWAYS>::Buffer ;
template <class ITEM>
using RegCaches = typename BUFFER_HELP<ITEM ,REGISTER ,ALWAYS>::RegCaches ;

template <class...>
trait ALLOCATOR_HELP ;

template <class...>
trait ALLOCATOR_HOLDER_HELP ;

template <class ITEM ,class SIZE ,class COND>
trait ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mNext ;
	} ;

	class Allocator {
	protected:
		Buffer<NODE ,SIZE> mAllocator ;
		LENGTH mSize ;
		LENGTH mLength ;
		INDEX mFree ;

	public:
		implicit Allocator () noexcept {
			mSize = 0 ;
			mLength = 0 ;
			mFree = NONE ;
		}
	} ;
} ;

template <class ITEM ,class SIZE ,class COND>
trait ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,COND ,REQUIRE<COND>> {
	using NODE = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::NODE ;
	using Super = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::Allocator ;

	class Allocator implement Super {
	protected:
		using Super::mAllocator ;
		using Super::mSize ;
		using Super::mLength ;
		using Super::mFree ;

	public:
		implicit Allocator () = default ;

		implicit Allocator (CREF<Allocator> that) {
			if (that.mAllocator.size () == 0)
				return ;
			mAllocator = Buffer<NODE ,SIZE> (that.mAllocator.size ()) ;
			for (auto&& i : iter (0 ,that.mSize)) {
				mAllocator[i].mNext = that.mAllocator[i].mNext ;
				if (mAllocator[i].mNext != USED)
					continue ;
				mAllocator[i].mItem = Box<ITEM>::make (that.mAllocator[i].mItem.self) ;
			}
			mSize = that.mSize ;
			mLength = that.mLength ;
			mFree = that.mFree ;
		}

		inline VREF<Allocator> operator= (CREF<Allocator> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Allocator (RREF<Allocator> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Allocator> operator= (RREF<Allocator> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait ALLOCATOR_HELP<ITEM ,SIZE ,ALWAYS> {
	using NODE = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::NODE ;
	using Super = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::Allocator ;

	using RESIZE_POW = RANK2 ;
	using RESIZE_MIN_SIZE = ENUM<256> ;

	class Allocator implement Super {
	protected:
		using Super::mAllocator ;
		using Super::mSize ;
		using Super::mLength ;
		using Super::mFree ;

	public:
		implicit Allocator () = default ;

		explicit Allocator (CREF<SizeProxy> size_) {
			mAllocator = Buffer<NODE ,SIZE> (size_) ;
		}

		void clear () {
			mSize = operator_min (mSize ,mAllocator.size ()) ;
			for (auto&& i : iter (0 ,mSize)) {
				if (mAllocator[i].mNext != USED)
					continue ;
				mAllocator[i].mItem = NULL ;
			}
			mSize = 0 ;
			mLength = 0 ;
			mFree = NONE ;
		}

		LENGTH size () const {
			return mAllocator.size () ;
		}

		LENGTH length () const {
			if (mAllocator.size () == 0)
				return 0 ;
			return mLength ;
		}

		BOOL used (CREF<INDEX> index) const {
			if ifnot (operator_between (index ,0 ,mSize))
				return FALSE ;
			if (mAllocator[index].mNext != USED)
				return FALSE ;
			return TRUE ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (used (index)) ;
			return mAllocator[index].mItem.self ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (used (index)) ;
			return mAllocator[index].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX alloc () {
			auto rax = Box<ITEM>::make () ;
			update_resize () ;
			INDEX ret = mFree ;
			mFree = mAllocator[ret].mNext ;
			mAllocator[ret].mItem = move (rax) ;
			mAllocator[ret].mNext = USED ;
			mLength++ ;
			return move (ret) ;
		}

		void free (CREF<INDEX> index) {
			assert (used (index)) ;
			mAllocator[index].mItem = NULL ;
			mAllocator[index].mNext = mFree ;
			mFree = index ;
			mLength-- ;
		}

		void resize (CREF<LENGTH> size_) {
			mAllocator.resize (size_) ;
			INDEX ix = NONE ;
			const auto r1x = operator_min (size_ ,mSize) ;
			mLength = r1x ;
			for (auto&& i : iter (0 ,r1x)) {
				INDEX iy = r1x - 1 - i ;
				if (mAllocator[iy].mNext == USED)
					continue ;
				mAllocator[iy].mNext = ix ;
				ix = iy ;
				mLength-- ;
			}
			mSize = r1x ;
			mFree = ix ;
		}

	private:
		void update_resize () {
			if ifswitch (TRUE) {
				if (mFree != NONE)
					discard ;
				if (mSize < mAllocator.size ())
					discard ;
				const auto r1x = operator_max (mSize * RESIZE_POW::expr ,RESIZE_MIN_SIZE::expr) ;
				resize (r1x) ;
			}
			if ifswitch (TRUE) {
				if (mFree != NONE)
					discard ;
				mAllocator[mSize].mNext = mFree ;
				mFree = mSize ;
				mSize++ ;
			}
		}
	} ;
} ;

template <class ITEM ,class SIZE>
using Allocator = typename ALLOCATOR_HELP<ITEM ,SIZE ,ALWAYS>::Allocator ;

template <class...>
trait LATER_HELP ;

template <class...>
trait LATER_HOLDER_HELP ;

template <class...>
trait LATER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait LATER_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<FLAG> tag) = 0 ;
		virtual void initialize (CREF<FLAG> tag ,RREF<AutoRef<>> value_) = 0 ;
		virtual CREF<AutoRef<>> invoke () const leftvalue = 0 ;
	} ;
} ;

template <class A>
trait LATER_HELP<A ,ALWAYS> {
	using Holder = typename LATER_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class Later {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Later () = default ;

		explicit Later (CREF<FLAG> tag) {
			mThis = Holder::create () ;
			mThis->initialize (tag) ;
		}

		explicit Later (CREF<FLAG> tag ,CREF<A> value_) {
			mThis = Holder::create () ;
			mThis->initialize (tag ,AutoRef<A>::make (move (value_))) ;
		}

		A invoke () const {
			return AutoRef<A>::from (mThis->invoke ()).self ;
		}

		inline A operator() () const {
			return invoke () ;
		}
	} ;
} ;

template <class A>
using Later = typename LATER_HELP<A ,ALWAYS>::Later ;
} ;
