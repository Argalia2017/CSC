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
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
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

template <class UNIT>
trait OPTIONAL_HOLDER_HELP<UNIT ,ALWAYS> {
	class Optional {
	protected:
		Box<UNIT> mValue ;
		BOOL mGood ;
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mGood = FALSE ;
			mCode = ZERO ;
		}
	} ;
} ;

template <class UNIT>
trait OPTIONAL_HELP<UNIT ,ALWAYS> {
	using Super = typename OPTIONAL_HOLDER_HELP<UNIT ,ALWAYS>::Optional ;

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
		imports Optional make (XREF<ARG1>...obj) {
			Optional ret ;
			ret.mValue = Box<UNIT>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
			ret.mGood = TRUE ;
			return move (ret) ;
		}

		BOOL good () const {
			return mGood ;
		}

		FLAG code () const {
			return mCode ;
		}

		UNIT poll () {
			assume (good ()) ;
			UNIT ret = move (mValue.self) ;
			mValue = NULL ;
			mGood = FALSE ;
			return move (ret) ;
		}

		UNIT fetch () const {
			require (IS_CLONEABLE<UNIT>) ;
			return mValue.self ;
		}

		void store (CREF<UNIT> obj) {
			require (IS_CLONEABLE<UNIT>) ;
			mValue.self = obj ;
		}

		UNIT exchange (CREF<UNIT> obj) {
			require (IS_CLONEABLE<UNIT>) ;
			UNIT ret = mValue.self ;
			mValue.self = obj ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT>
using Optional = typename OPTIONAL_HELP<UNIT ,ALWAYS>::Optional ;

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

template <class UNIT>
trait BUFFERPROC_HELP<UNIT ,ALWAYS> {
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

		imports BOOL buf_equal (CREF<ARR<UNIT>> src1 ,CREF<ARR<UNIT>> src2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_)) {
				if ifnot (operator_equal (src1[i] ,src2[i]))
					return FALSE ;
			}
			return TRUE ;
		}

		imports FLAG buf_compr (CREF<ARR<UNIT>> src1 ,CREF<ARR<UNIT>> src2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_)) {
				const auto r1x = operator_compr (src1[i] ,src2[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
		}

		imports FLAG buf_hash (CREF<ARR<UNIT>> src ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			FLAG ret = hashcode () ;
			for (auto &&i : iter (begin_ ,end_)) {
				const auto r1x = operator_hash (src[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		imports void buf_copy (VREF<ARR<UNIT>> dst ,CREF<ARR<UNIT>> src ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_))
				dst[i] = src[i] ;
		}

		imports void buf_swap (VREF<ARR<UNIT>> dst1 ,VREF<ARR<UNIT>> dst2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_))
				swap (dst1[i] ,dst2[i]) ;
		}

		imports void buf_fill (VREF<ARR<UNIT>> dst ,CREF<UNIT> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_))
				dst[i] = item ;
		}

		imports INDEX buf_find (CREF<ARR<UNIT>> src ,CREF<UNIT> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_)) {
				if (operator_equal (src[i] ,item))
					return i ;
			}
			return NONE ;
		}

		imports INDEX buf_find_r (CREF<ARR<UNIT>> src ,CREF<UNIT> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (0 ,end_ - begin_)) {
				INDEX ix = end_ - 1 - i ;
				if (operator_equal (src[ix] ,item))
					return ix ;
			}
			return NONE ;
		}

		template <class ARG1 = UNIT ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRA ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPEAS<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = UNIT ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRW ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPEAS<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = UNIT ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRU8 ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPEAS<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = UNIT ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRU16 ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPEAS<ARG1>::expr ,src ,size_) ;
		}

		template <class ARG1 = UNIT ,class = REQUIRE<IS_TEXT<ARG1>>>
		imports Slice<ARG1> buf_slice (CREF<ARR<KILL<STRU32 ,ARG1>>> src ,CREF<LENGTH> size_) {
			return template_buf_slice (TYPEAS<ARG1>::expr ,src ,size_) ;
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

template <class UNIT>
using BufferProc = typename BUFFERPROC_HELP<UNIT ,ALWAYS>::BufferProc ;

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
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPEAS<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		imports inline VREF<TYPE_FIRST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,VREF<ARG1> tuple ,TYPEID<ARG2> id) {
			return tuple.one () ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG2>>>
		imports inline CREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,CREF<ARG1> tuple ,TYPEID<ARG2> id) {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPEAS<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		imports inline CREF<TYPE_FIRST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,CREF<ARG1> tuple ,TYPEID<ARG2> id) {
			return tuple.one () ;
		}
	} ;
} ;

template <class...>
trait TUPLE_HELP ;

template <class UNIT>
trait TUPLE_HELP<UNIT ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT>>>> {
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
	using FIRST_ONE = TYPE_FIRST_ONE<PARAMS> ;
	require (IS_DEFAULT<FIRST_ONE>) ;
	using Super = typename TUPLE_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::Tuple ;

	struct PACK {
		FIRST_ONE mOne ;
	} ;

	class Tuple {
	protected:
		PACK mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Tuple ,ARG1>>...>>>
		explicit Tuple (XREF<ARG1>...obj) {
			assign (forward[TYPEAS<ARG1>::expr] (obj)...) ;
		}

		void assign () {
			noop () ;
		}

		void assign (CREF<FIRST_ONE> obj) {
			one () = move (obj) ;
		}

		void assign (RREF<FIRST_ONE> obj) {
			one () = move (obj) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		VREF<FIRST_ONE> one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<Super> rest () leftvalue {
			return Super::zero () ;
		}

		CREF<Super> rest () const leftvalue {
			return Super::zero () ;
		}

		VREF<FIRST_ONE> m1st_m () leftvalue {
			return one () ;
		}

		CREF<FIRST_ONE> m1st_m () const leftvalue {
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
	using FIRST_ONE = TYPE_FIRST_ONE<PARAMS> ;
	require (IS_DEFAULT<FIRST_ONE>) ;
	using SECOND_ONE = TYPE_SECOND_ONE<PARAMS> ;
	using FIRST_REST = typename TUPLE_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::PACK ;
	using Super = typename TUPLE_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::Tuple ;

	struct PACK {
		FIRST_ONE mOne ;
		FIRST_REST mRest ;
	} ;

	class Tuple {
	protected:
		PACK mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Tuple ,ARG1>>...>>>
		explicit Tuple (XREF<ARG1>...obj) {
			assign (forward[TYPEAS<ARG1>::expr] (obj)...) ;
		}

		void assign () {
			noop () ;
		}

		template <class...ARG1>
		void assign (CREF<FIRST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (forward[TYPEAS<ARG1>::expr] (obj2)...) ;
		}

		template <class...ARG1>
		void assign (RREF<FIRST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (forward[TYPEAS<ARG1>::expr] (obj2)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		VREF<FIRST_ONE> one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<Super> rest () leftvalue {
			return unsafe_cast[TYPEAS<Super>::expr] (mTuple.mRest) ;
		}

		CREF<Super> rest () const leftvalue {
			return unsafe_cast[TYPEAS<Super>::expr] (mTuple.mRest) ;
		}

		VREF<FIRST_ONE> m1st_m () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> m1st_m () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<SECOND_ONE> m2nd_m () leftvalue {
			return mTuple.mRest.mOne ;
		}

		CREF<SECOND_ONE> m2nd_m () const leftvalue {
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
	using FIRST_ONE = TYPE_FIRST_ONE<PARAMS> ;
	require (IS_DEFAULT<FIRST_ONE>) ;
	using SECOND_ONE = TYPE_SECOND_ONE<PARAMS> ;
	using THIRD_ONE = TYPE_THIRD_ONE<PARAMS> ;
	using FIRST_REST = typename TUPLE_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::PACK ;
	using Super = typename TUPLE_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::Tuple ;

	struct PACK {
		FIRST_ONE mOne ;
		FIRST_REST mRest ;
	} ;

	class Tuple {
	protected:
		PACK mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Tuple ,ARG1>>...>>>
		explicit Tuple (XREF<ARG1>...obj) {
			assign (forward[TYPEAS<ARG1>::expr] (obj)...) ;
		}

		void assign () {
			noop () ;
		}

		template <class...ARG1>
		void assign (CREF<FIRST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (forward[TYPEAS<ARG1>::expr] (obj2)...) ;
		}

		template <class...ARG1>
		void assign (RREF<FIRST_ONE> obj1 ,XREF<ARG1>...obj2) {
			one () = move (obj1) ;
			rest ().assign (forward[TYPEAS<ARG1>::expr] (obj2)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		VREF<FIRST_ONE> one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<Super> rest () leftvalue {
			return unsafe_cast[TYPEAS<Super>::expr] (mTuple.mRest) ;
		}

		CREF<Super> rest () const leftvalue {
			return unsafe_cast[TYPEAS<Super>::expr] (mTuple.mRest) ;
		}

		VREF<FIRST_ONE> m1st_m () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> m1st_m () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<SECOND_ONE> m2nd_m () leftvalue {
			return mTuple.mRest.mOne ;
		}

		CREF<SECOND_ONE> m2nd_m () const leftvalue {
			return mTuple.mRest.mOne ;
		}

		VREF<THIRD_ONE> m3rd_m () leftvalue {
			return mTuple.mRest.mRest.mOne ;
		}

		CREF<THIRD_ONE> m3rd_m () const leftvalue {
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

template <class...UNIT>
using Tuple = typename TUPLE_HELP<TYPEAS<UNIT...> ,ALWAYS>::Tuple ;

template <class...>
trait FUNCTION_FORWARD_EACH_HELP ;

template <class UNIT>
trait FUNCTION_FORWARD_EACH_HELP<UNIT ,REQUIRE<IS_VARIABLE<UNIT>>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait FUNCTION_FORWARD_EACH_HELP<UNIT ,REQUIRE<IS_CONSTANT<UNIT>>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait FUNCTION_FORWARD_EACH_HELP<UNIT ,REQUIRE<IS_REGISTER<UNIT>>> {
	using RET = UNIT ;
} ;

template <class UNIT>
trait FUNCTION_FORWARD_EACH_HELP<UNIT ,REQUIRE<IS_ORDINARY<UNIT>>> {
	using RET = CREF<REMOVE_REF<UNIT>> ;
} ;

template <class UNIT>
using FUNCTION_FORWARD_EACH = typename FUNCTION_FORWARD_EACH_HELP<UNIT ,ALWAYS>::RET ;

template <class...>
trait FUNCTION_FORWARD_HELP ;

template <class...PARAM>
trait FUNCTION_FORWARD_HELP<TYPEAS<PARAM...> ,ALWAYS> {
	using RET = TYPEAS<FUNCTION_FORWARD_EACH<PARAM>...> ;
} ;

template <class PARAMS>
using FUNCTION_FORWARD = typename FUNCTION_FORWARD_HELP<PARAMS ,ALWAYS>::RET ;

template <class...>
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_WRAPPER_HELP ;

template <class...>
trait FUNCTION_PUREHOLDER_HELP ;

template <class RETURN ,class...PARAM>
trait FUNCTION_HELP<RETURN ,TYPEAS<PARAM...> ,ALWAYS> {
	using PARAMS = TYPEAS<PARAM...> ;

	struct Holder implement Interface {
		virtual void initialize (RREF<XRef> value_) = 0 ;
		virtual BOOL effective () const = 0 ;
		virtual RETURN invoke (XREF<PARAM>...params) const = 0 ;
	} ;

	class Function {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Function () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<Function ,ARG1>>>>>
		implicit Function (RREF<ARG1> that) {
			require (IS_FUNCTION<ARG1>) ;
			require (IS_SAME<RETURN ,FUNCTION_RETURN<ARG1>>) ;
			using R2X = typename FUNCTION_PUREHOLDER_HELP<RETURN ,PARAMS ,ARG1 ,ALWAYS>::PureHolder ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (IS_EFFECTIVE<ARG1>::expr)
					discard ;
				auto rax = VRef<Holder> (VRef<R2X>::make ()) ;
				rax->initialize (VRef<ARG1>::reference (that)) ;
				mThis = move (rax) ;
			}
			if ifswitch (act) {
				auto rax = VRef<Holder> (VRef<R2X>::make ()) ;
				rax->initialize (VRef<ARG1>::reference (that)) ;
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

		RETURN invoke (XREF<PARAM>...params) const {
			return mThis->invoke (forward[TYPEAS<PARAM>::expr] (params)...) ;
		}

		inline RETURN operator() (XREF<PARAM>...params) const {
			return invoke (forward[TYPEAS<PARAM>::expr] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class...PARAM ,class UNIT>
trait FUNCTION_PUREHOLDER_HELP<RETURN ,TYPEAS<PARAM...> ,UNIT ,ALWAYS> {
	using PARAMS = TYPEAS<PARAM...> ;
	using Holder = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		Box<UNIT> mValue ;

	public:
		void initialize (RREF<XRef> value_) override {
			auto rax = VRef<UNIT> (move (value_)) ;
			mValue = Box<UNIT>::make (move (rax.self)) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<UNIT>::expr ;
		}

		RETURN invoke (XREF<PARAM>...params) const override {
			return mValue.self (forward[TYPEAS<PARAM>::expr] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class PARAMS = TYPEAS<>>
using Function = typename FUNCTION_HELP<RETURN ,FUNCTION_FORWARD<PARAMS> ,ALWAYS>::Function ;

template <class...>
trait SCOPEFINALLY_HELP ;

template <class DEPEND>
trait SCOPEFINALLY_HELP<DEPEND ,ALWAYS> {
	class ScopeFinally implement Proxy {
	protected:
		Function<void> mThat ;

	public:
		imports CREF<ScopeFinally> from (CREF<Function<void>> that) {
			return unsafe_cast[TYPEAS<ScopeFinally>::expr] (that) ;
		}

		imports CREF<ScopeFinally> from (RREF<Function<void>>) = delete ;

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
			return unsafe_cast[TYPEAS<ScopeCounter>::expr] (that) ;
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

template <class UNIT>
trait AUTOREF_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Layout ;
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
			auto &&tmp = keep[TYPEAS<VREF<AutoRef>>::expr] (that) ;
			assume (tmp.good ()) ;
			return tmp ;
		}

		imports CREF<AutoRef> from (CREF<Layout> that) {
			auto &&tmp = keep[TYPEAS<CREF<AutoRef>>::expr] (that) ;
			assume (tmp.good ()) ;
			return tmp ;
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

template <class UNIT>
trait AUTOREF_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_VOID<UNIT>>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Layout ;
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
			auto &&tmp = keep[TYPEAS<VREF<AutoRef>>::expr] (that) ;
			assume (tmp.good ()) ;
			return tmp ;
		}

		imports CREF<AutoRef> from (CREF<Layout> that) {
			auto &&tmp = keep[TYPEAS<CREF<AutoRef>>::expr] (that) ;
			assume (tmp.good ()) ;
			return tmp ;
		}

		imports CREF<AutoRef> from (RREF<Layout>) = delete ;

		template <class...ARG1>
		imports AutoRef make (XREF<ARG1>...obj) {
			using R1X = KILL<UNIT ,TYPEAS<ARG1...>> ;
			using R2X = typename KILL<AUTOREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			AutoRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			ret.mThis->initialize () ;
			ret.mPointer = ret.mThis->pointer () ;
			auto &&tmp = unsafe_cast[TYPEAS<Box<R1X>>::expr] (unsafe_deref (ret.mPointer)) ;
			tmp.acquire (TYPEAS<R1X>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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
			if (mThis->clazz () != Clazz (TYPEAS<UNIT>::expr))
				return FALSE ;
			return TRUE ;
		}

		Clazz clazz () const {
			if ifnot (exist ())
				return Clazz () ;
			return mThis->clazz () ;
		}

		VREF<UNIT> self_m () leftvalue {
			return unsafe_cast[TYPEAS<UNIT>::expr] (unsafe_deref (mPointer)) ;
		}

		inline implicit operator VREF<UNIT> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return unsafe_cast[TYPEAS<UNIT>::expr] (unsafe_deref (mPointer)) ;
		}

		inline implicit operator CREF<UNIT> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}
	} ;
} ;

template <class UNIT>
trait AUTOREF_PUREHOLDER_HELP<UNIT ,ALWAYS> {
	using Holder = typename AUTOREF_HELP<UNIT ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		Box<UNIT> mValue ;

	public:
		void initialize () override {
			noop () ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPEAS<UNIT>::expr) ;
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

template <class UNIT = void>
using AutoRef = typename AUTOREF_HELP<UNIT ,ALWAYS>::AutoRef ;

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

	using FAKE_MAX_SIZE = ENUMAS<VAL ,128> ;
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

template <class UNIT>
trait SHAREDREF_HELP<UNIT ,ALWAYS> {
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
		imports SharedRef make (XREF<ARG1>...obj) {
			using R1X = KILL<UNIT ,TYPEAS<ARG1...>> ;
			using R2X = typename KILL<SHAREDREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			SharedRef ret ;
			auto rax = VRef<Holder> (VRef<R2X>::make ()) ;
			rax->initialize () ;
			ret.mThis = move (rax) ;
			ret.mPointer = ret.mThis->pointer () ;
			auto &&tmp = unsafe_cast[TYPEAS<Box<R1X>>::expr] (unsafe_deref (ret.mPointer)) ;
			tmp.acquire (TYPEAS<R1X>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		template <class ARG1 = CRef<Holder>>
		imports SharedRef intrusive (RREF<ARG1> obj) {
			using R1X = KILL<UNIT ,ARG1> ;
			assert (obj != NULL) ;
			SharedRef ret ;
			Scope<PinMutex> anonymous (obj->pin_mutex ()) ;
			ret.mThis = move (obj) ;
			ret.mPointer = ret.mThis->pointer () ;
			if ifswitch (TRUE) {
				if (ret.mThis->counter () > 0)
					discard ;
				auto &&tmp = unsafe_cast[TYPEAS<Box<R1X>>::expr] (unsafe_deref (ret.mPointer)) ;
				tmp.acquire (TYPEAS<R1X>::expr) ;
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

		VREF<UNIT> self_m () const leftvalue {
			return unsafe_cast[TYPEAS<UNIT>::expr] (unsafe_deref (mPointer)) ;
		}

		inline implicit operator VREF<UNIT> () const leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT>> operator-> () const leftvalue {
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

template <class UNIT>
trait SHAREDREF_PUREHOLDER_HELP<UNIT ,ALWAYS> {
	using Holder = typename SHAREDREF_HELP<UNIT ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		PinMutex mMutex ;
		LENGTH mCounter ;
		Box<UNIT> mValue ;

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

template <class UNIT>
using SharedRef = typename SHAREDREF_HELP<UNIT ,ALWAYS>::SharedRef ;

template <class...>
trait UNIQUEREF_HELP ;

template <class...>
trait UNIQUEREF_HOLDER_HELP ;

template <class...>
trait UNIQUEREF_PUREHOLDER_HELP ;

template <class DEPEND>
trait UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<XRef> destructor) = 0 ;
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

template <class UNIT>
trait UNIQUEREF_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
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
			using R1X = KILL<UNIT ,TYPEAS<ARG1 ,ARG2>> ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<void> ;
			mThis = VRef<R2X>::make () ;
			auto rax = R3X (move (destructor)) ;
			assert (ifnot (rax.effective ())) ;
			mThis->initialize (VRef<R3X>::reference (rax)) ;
			mPointer = mThis->pointer () ;
			constructor () ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		imports UniqueRef make () {
			using R1X = UNIT ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<void> ;
			UniqueRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			auto rax = R3X () ;
			ret.mThis->initialize (VRef<R3X>::reference (rax)) ;
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

template <class UNIT>
trait UNIQUEREF_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_VOID<UNIT>>>> {
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
			using R1X = KILL<UNIT ,TYPEAS<ARG1 ,ARG2>> ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<void ,TYPEAS<VREF<R1X>>> ;
			mThis = VRef<R2X>::make () ;
			auto rax = R3X (move (destructor)) ;
			assert (ifnot (rax.effective ())) ;
			mThis->initialize (VRef<R3X>::reference (rax)) ;
			mPointer = mThis->pointer () ;
			auto &&tmp = unsafe_cast[TYPEAS<Box<R1X>>::expr] (unsafe_deref (mPointer)) ;
			tmp.acquire (TYPEAS<R1X>::expr) ;
			constructor (tmp.self) ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		template <class...ARG1>
		imports UniqueRef make (XREF<ARG1>...obj) {
			using R1X = KILL<UNIT ,TYPEAS<ARG1...>> ;
			using R2X = typename KILL<UNIQUEREF_PUREHOLDER_HELP<R1X ,ALWAYS> ,DEPEND>::PureHolder ;
			using R3X = Function<void ,TYPEAS<VREF<R1X>>> ;
			UniqueRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			auto rax = R3X () ;
			ret.mThis->initialize (VRef<R3X>::reference (rax)) ;
			ret.mPointer = ret.mThis->pointer () ;
			auto &&tmp = unsafe_cast[TYPEAS<Box<R1X>>::expr] (unsafe_deref (ret.mPointer)) ;
			tmp.acquire (TYPEAS<R1X>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		CREF<UNIT> self_m () const leftvalue {
			return unsafe_cast[TYPEAS<UNIT>::expr] (unsafe_deref (mPointer)) ;
		}

		inline implicit operator CREF<UNIT> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}
	} ;
} ;

template <class UNIT>
trait UNIQUEREF_PUREHOLDER_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT ,ALWAYS>::Holder ;

	using Destructor = Function<void> ;

	class PureHolder implement Holder {
	protected:
		Destructor mDestructor ;

	public:
		void initialize (RREF<XRef> destructor) override {
			auto rax = VRef<Destructor> (move (destructor)) ;
			mDestructor = move (rax.self) ;
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
				mDestructor () ;
				mDestructor = Destructor () ;
			}
		}
	} ;
} ;

template <class UNIT>
trait UNIQUEREF_PUREHOLDER_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_VOID<UNIT>>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT ,ALWAYS>::Holder ;

	using Destructor = Function<void ,TYPEAS<VREF<UNIT>>> ;

	class PureHolder implement Holder {
	protected:
		Destructor mDestructor ;
		Box<UNIT> mValue ;

	public:
		void initialize (RREF<XRef> destructor) override {
			auto rax = VRef<Destructor> (move (destructor)) ;
			mDestructor = move (rax.self) ;
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

template <class UNIT = void>
using UniqueRef = typename UNIQUEREF_HELP<UNIT ,ALWAYS>::UniqueRef ;

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
	class Buffer {
	protected:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_ALL<ENUM_NOT<IS_TRIVIAL<ITEM>> ,IS_DEFAULT<ITEM>>>> {
	class Buffer {
	protected:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () noexcept {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make () ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_HOLDER_HOLDER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_NOT<IS_DEFAULT<ITEM>>>> {
	class Buffer {
	protected:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE ,class COND>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,COND ,REQUIRE<ENUM_NOT<COND>>> {
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
	using Super = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mBuffer == NULL)
				return ;
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (that.mBuffer.self) ;
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
	using Super = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<ARR<ITEM ,SIZE>> that) {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (that) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			assert (size_ <= SIZE::expr) ;
			mBuffer = Box<ARR<ITEM ,SIZE>>::make () ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return SIZE::expr ;
		}

		VREF<ARR<ITEM>> self_m () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return unsafe_array (mBuffer.self[0]) ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return unsafe_array (mBuffer.self[0]) ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
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

template <class ITEM ,class SIZE>
using Buffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE ,class = REQUIRE<ENUM_GT_ZERO<SIZE>>>
using BoxBuffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class...>
trait REGBUFFER_HELP ;

template <class ITEM>
trait REGBUFFER_HELP<ITEM ,ALWAYS> {
	struct Layout {} ;
} ;

template <class...>
trait VARBUFFER_HELP ;

template <class ITEM ,class COND>
trait VARBUFFER_HELP<ITEM ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	using Layout = typename REGBUFFER_HELP<ITEM ,ALWAYS>::Layout ;

	class Buffer implement Layout {
	protected:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class COND>
trait VARBUFFER_HELP<ITEM ,COND ,REQUIRE<COND>> {
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
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
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
	using Layout = typename REGBUFFER_HELP<ITEM ,ALWAYS>::Layout ;
	using Super = typename VARBUFFER_HELP<ITEM ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;
		using Super::mSize ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (RREF<Layout> that) {
			thiz = keep[TYPEAS<RREF<Buffer>>::expr] (that) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return mSize ;
		}

		VREF<ARR<ITEM>> self_m () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
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
				auto rax = VRef<ARR<ITEM>>::make (size_) ;
				const auto r1x = vmin (size_ ,mSize) ;
				BufferProc<ITEM>::buf_swap (rax.self ,mBuffer.self ,0 ,r1x) ;
				mBuffer = move (rax) ;
				mSize = size_ ;
			}
			if ifswitch (act) {
				if (size_ == 0)
					discard ;
				mBuffer = VRef<ARR<ITEM>>::make (size_) ;
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

	class Buffer implement Layout {
	protected:
		CRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,CONSTANT>>> {
	using Layout = typename REGBUFFER_HELP<ITEM ,ALWAYS>::Layout ;
	using Super = typename CONBUFFER_HELP<ITEM ,ALWAYS>::Buffer ;

	class Buffer implement Super {
	protected:
		using Super::mBuffer ;
		using Super::mSize ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (RREF<Layout> that) {
			thiz = keep[TYPEAS<RREF<Buffer>>::expr] (that) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = CRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return 0 ;
			return mSize ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
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

	struct RegCaches ;

	class Buffer implement Proxy ,Layout {
	protected:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		imports VREF<Buffer> from (VREF<Layout> that) {
			auto &&tmp = keep[TYPEAS<VREF<Buffer>>::expr] (that) ;
			return unsafe_cast[TYPEAS<Buffer>::expr] (tmp) ;
		}

		imports CREF<Buffer> from (CREF<Layout> that) {
			auto &&tmp = keep[TYPEAS<CREF<Buffer>>::expr] (that) ;
			return unsafe_cast[TYPEAS<Buffer>::expr] (tmp) ;
		}

		imports CREF<Buffer> from (RREF<Layout>) = delete ;

		imports VREF<Buffer> from (VREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			return from (unsafe_cast[TYPEAS<ARR<ITEM>>::expr] (buffer) ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports VREF<Buffer> from (VREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			auto &&tmp = unsafe_cast[TYPEAS<Buffer>::expr] (unnamed) ;
			if ifswitch (TRUE) {
				const auto r1x = vmax (end_ - begin_ ,0) ;
				if (r1x == ZERO)
					discard ;
				tmp.mBuffer = VRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
				tmp.mSize = r1x ;
			}
			return tmp ;
		}

		imports CREF<Buffer> from (CREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			return from (unsafe_cast[TYPEAS<ARR<ITEM>>::expr] (buffer) ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports CREF<Buffer> from (CREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<RegCaches> unnamed = RegCaches ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			auto &&tmp = unsafe_cast[TYPEAS<Buffer>::expr] (unnamed) ;
			if ifswitch (TRUE) {
				const auto r1x = vmax (end_ - begin_ ,0) ;
				if (r1x == ZERO)
					discard ;
				tmp.mBuffer = CRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
				tmp.mSize = r1x ;
			}
			return tmp ;
		}

		VRef<Buffer> borrow () leftvalue {
			VRef<Buffer> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<VRef<RegCaches>>::expr] (ret) ;
			tmp = VRef<RegCaches>::make () ;
			unsafe_launder (ret) ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				ret->mBuffer = VRef<ARR<ITEM>>::reference (mBuffer.self) ;
				ret->mSize = mSize ;
			}
			return move (ret) ;
		}

		CRef<Buffer> borrow () const leftvalue {
			VRef<Buffer>  ret ;
			auto &&tmp = unsafe_cast[TYPEAS<VRef<RegCaches>>::expr] (ret) ;
			tmp = VRef<RegCaches>::make () ;
			unsafe_launder (ret) ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				ret->mBuffer = CRef<ARR<ITEM>>::reference (mBuffer.self) ;
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
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
		}

		inline BOOL operator== (CREF<Buffer>) = delete ;

		inline BOOL operator!= (CREF<Buffer>) = delete ;

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
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
trait ORDBUFFER_PUREHOLDER_HELP ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,ORDINARY>>> {
	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<Holder>> prefix) = 0 ;
		virtual XRef native () const leftvalue = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual VREF<ITEM> at (CREF<INDEX> index) leftvalue = 0 ;
		virtual CREF<ITEM> at (CREF<INDEX> index) const leftvalue = 0 ;
	} ;

	class Buffer {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (RREF<VRef<Holder>> that) {
			using R1X = typename KILL<ORDBUFFER_PUREHOLDER_HELP<ITEM ,ALWAYS> ,DEPEND>::PureHolder ;
			mThis = VRef<R1X>::make () ;
			mThis->initialize (move (that)) ;
		}

		explicit Buffer (CREF<SizeProxy> size_) {
			assert (size_ == 0) ;
		}

		XRef native () const leftvalue {
			return mThis->native () ;
		}

		LENGTH size () const {
			if (mThis == NULL)
				return 0 ;
			return mThis->size () ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mThis->at (index) ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mThis->at (index) ;
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
trait ORDBUFFER_PUREHOLDER_HELP<ITEM ,ALWAYS> {
	using Holder = typename BUFFER_HELP<ITEM ,ORDINARY ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		VRef<Holder> mPrefix ;

	public:
		void initialize (RREF<VRef<Holder>> prefix) override {
			mPrefix = move (prefix) ;
		}

		XRef native () const leftvalue override {
			return mPrefix->native () ;
		}

		LENGTH size () const override {
			return mPrefix->size () ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue override {
			return mPrefix->at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue override {
			return mPrefix->at (index) ;
		}
	} ;
} ;

template <class ITEM>
using OrdBuffer = typename BUFFER_HELP<ITEM ,ORDINARY ,ALWAYS>::Buffer ;

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
			for (auto &&i : iter (0 ,that.mSize)) {
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
	using RESIZE_MIN_SIZE = ENUMAS<VAL ,256> ;

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
			mSize = vmin (mSize ,mAllocator.size ()) ;
			for (auto &&i : iter (0 ,mSize)) {
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
			if ifnot (vbetween (index ,0 ,mSize))
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
			const auto r1x = vmin (size_ ,mSize) ;
			mLength = r1x ;
			for (auto &&i : iter (0 ,r1x)) {
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
				const auto r1x = vmax (mSize * RESIZE_POW::expr ,RESIZE_MIN_SIZE::expr) ;
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

template <class UNIT>
trait LATER_HELP<UNIT ,ALWAYS> {
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

		explicit Later (CREF<FLAG> tag ,CREF<UNIT> value_) {
			mThis = Holder::create () ;
			mThis->initialize (tag ,AutoRef<UNIT>::make (move (value_))) ;
		}

		UNIT invoke () const {
			return AutoRef<UNIT>::from (mThis->invoke ()).self ;
		}

		inline UNIT operator() () const {
			return invoke () ;
		}
	} ;
} ;

template <class UNIT>
using Later = typename LATER_HELP<UNIT ,ALWAYS>::Later ;
} ;
