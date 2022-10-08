#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
template <class...>
trait OPTIONAL_HELP ;

template <class...>
trait OPTIONAL_HOLDER_HELP ;

template <class UNIT ,class ATTR>
trait OPTIONAL_HOLDER_HELP<UNIT ,ATTR ,REQUIRE<ENUM_NOT<ATTR>>> {
	class Optional {
	protected:
		Box<UNIT> mValue ;
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mCode = ZERO ;
		}
	} ;
} ;

template <class UNIT ,class ATTR>
trait OPTIONAL_HOLDER_HELP<UNIT ,ATTR ,REQUIRE<ATTR>> {
	using SUPER = typename OPTIONAL_HOLDER_HELP<UNIT ,ENUM_FALSE ,ALWAYS>::Optional ;

	class Optional extend SUPER {
	protected:
		using SUPER::mValue ;
		using SUPER::mCode ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<Optional> that) {
			mValue = Box<UNIT>::make (that.mValue.self) ;
			mCode = that.mCode ;
		}

		inline VREF<Optional> operator= (CREF<Optional> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Optional (RREF<Optional> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Optional> operator= (RREF<Optional> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class UNIT>
trait OPTIONAL_HELP<UNIT ,ALWAYS> {
	using SUPER = typename OPTIONAL_HOLDER_HELP<UNIT ,IS_CLONEABLE<UNIT> ,ALWAYS>::Optional ;

	class Optional extend SUPER {
	protected:
		using SUPER::mValue ;
		using SUPER::mCode ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<FLAG> that) {
			mCode = that ;
		}

		template <class...ARG1>
		imports Optional make (XREF<ARG1>...obj) {
			Optional ret ;
			ret.mValue = Box<UNIT>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
			ret.mCode = USED ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mValue == NULL)
				return FALSE ;
			if (mCode != USED)
				return FALSE ;
			return TRUE ;
		}

		FLAG code () const {
			return mCode ;
		}

		UNIT poll () {
			assume (exist ()) ;
			UNIT ret = move (mValue.self) ;
			mValue = NULL ;
			mCode = USED ;
			return move (ret) ;
		}

		UNIT fetch () const {
			assert (exist ()) ;
			return mValue.self ;
		}

		UNIT fetch (CREF<UNIT> def) const {
			if ifnot (exist ())
				return move (def) ;
			return mValue.self ;
		}

		UNIT fetch (RREF<UNIT> def) const {
			if ifnot (exist ())
				return move (def) ;
			return mValue.self ;
		}

		BOOL equal (CREF<Optional> that) const {
			const auto r1x = exist () ;
			const auto r2x = that.exist () ;
			if (r1x != r2x)
				return FALSE ;
			if ifnot (r1x)
				return TRUE ;
			return operator_equal (mValue.self ,that.mValue.self) ;
		}

		inline BOOL operator== (CREF<Optional> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Optional> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Optional> that) const {
			const auto r1x = exist () ;
			const auto r2x = that.exist () ;
			if ifnot (r1x)
				return operator_compr (r1x ,r2x) ;
			if ifnot (r2x)
				return operator_compr (r1x ,r2x) ;
			return operator_compr (mValue.self ,that.mValue.self) ;
		}

		inline BOOL operator< (CREF<Optional> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Optional> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Optional> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Optional> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			if ifnot (exist ())
				return hashcode () ;
			return operator_hash (mValue.self) ;
		}
	} ;
} ;

template <class UNIT>
using Optional = typename OPTIONAL_HELP<UNIT ,ALWAYS>::Optional ;

template <class...>
trait BUFFERPROC_HELP ;

template <class...>
trait BUFFERPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BUFFERPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class BufferProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<BufferProc> instance () {
			return memorize ([&] () {
				BufferProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		template <class ARG1 ,class ARG2>
		imports BOOL buf_equal (CREF<ARG1> src1 ,CREF<ARG2> src2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_)) {
				if ifnot (operator_equal (src1[i] ,src2[i]))
					return FALSE ;
			}
			return TRUE ;
		}

		template <class ARG1 ,class ARG2>
		imports FLAG buf_compr (CREF<ARG1> src1 ,CREF<ARG2> src2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_)) {
				const auto r1x = operator_compr (src1[i] ,src2[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
		}

		template <class ARG1>
		imports FLAG buf_hash (CREF<ARG1> src ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			FLAG ret = hashcode () ;
			for (auto &&i : iter (begin_ ,end_)) {
				const auto r1x = operator_hash (src[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		template <class ARG1 ,class ARG2>
		imports void buf_copy (VREF<ARG1> dst ,CREF<ARG2> src ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_))
				dst[i] = src[i] ;
		}

		template <class ARG1>
		imports void buf_move (VREF<ARG1> dst1 ,VREF<ARG1> dst2 ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			using R1X = typeof (dst1[0]) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (IS_TRIVIAL<R1X>::expr)
					discard ;
				for (auto &&i : iter (begin_ ,end_))
					swap (dst1[i] ,dst2[i]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (begin_ ,end_))
					unsafe_deptr (dst1[i]) = unsafe_deptr (dst2[i]) ;
				unsafe_launder (dst1) ;
				unsafe_launder (dst2) ;
			}
		}

		template <class ARG1 ,class ARG2>
		imports void buf_fill (VREF<ARG1> dst ,CREF<ARG2> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			for (auto &&i : iter (begin_ ,end_))
				dst[i] = item ;
		}

		template <class ARG1 ,class ARG2>
		imports INDEX buf_find (CREF<ARG1> src ,CREF<ARG2> item ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,CREF<BOOL> forward_) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (forward_)
					discard ;
				for (auto &&i : iter (begin_ ,end_)) {
					if (operator_equal (src[i] ,item))
						return i ;
				}
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,end_ - begin_)) {
					INDEX ix = end_ - 1 - i ;
					if (operator_equal (src[ix] ,item))
						return ix ;
				}
			}
			return NONE ;
		}

		template <class ARG1 ,class ARG2>
		imports void buf_slice (VREF<ARG1> dst ,CREF<ARG2> src ,CREF<LENGTH> size_) {
			using R1X = typeof (src[0]) ;
			const auto r1x = invoke ([&] () {
				const auto r2x = buf_find (src ,R1X ('\0') ,0 ,size_ ,TRUE) ;
				if (r2x != NONE)
					return r2x ;
				return size_ ;
			}) ;
			buf_copy (dst ,src ,0 ,r1x) ;
			buf_fill (dst ,R1X (0) ,r1x ,size_) ;
		}
	} ;
} ;

using BufferProc = typename BUFFERPROC_HELP<DEPEND ,ALWAYS>::BufferProc ;

template <class...>
trait FUNCTION_tuple_pick_HELP ;

template <class PARAMS>
trait FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS> {
	struct FUNCTION_tuple_pick {
		template <class ARG1 ,class ARG2>
		inline VREF<TYPE_PICK<PARAMS ,ARG2>> operator() (VREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			return template_pick (PHX ,tuple ,id) ;
		}

		template <class ARG1 ,class ARG2>
		inline CREF<TYPE_PICK<PARAMS ,ARG2>> operator() (CREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			return template_pick (PHX ,tuple ,id) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG2>>>
		forceinline VREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,VREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPEAS<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		forceinline VREF<TYPE_FIRST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,VREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			return tuple.one () ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG2>>>
		forceinline CREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,CREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPEAS<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		forceinline CREF<TYPE_FIRST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,CREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
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
			return ZERO ;
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
	using FIRST_REST = TYPE_FIRST_REST<PARAMS> ;
	using FIRST_REST_TUPLE = typename TUPLE_HELP<FIRST_REST ,ALWAYS>::Tuple ;

	struct HEAP {
		FIRST_ONE mOne ;
	} ;

	class Tuple {
	protected:
		HEAP mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_NOT<ENUM_ANY<IS_SAME<ARG1 ,Tuple>...>>>>
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

		VREF<FIRST_REST_TUPLE> rest () leftvalue {
			return FIRST_REST_TUPLE::zero () ;
		}

		CREF<FIRST_REST_TUPLE> rest () const leftvalue {
			return FIRST_REST_TUPLE::zero () ;
		}

		VREF<FIRST_ONE> mP1st_m () leftvalue {
			return one () ;
		}

		CREF<FIRST_ONE> mP1st_m () const leftvalue {
			return one () ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,ARG1>> pick (CREF<TYPEID<ARG1>> id) leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,ARG1>> pick (CREF<TYPEID<ARG1>> id) const leftvalue {
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
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<PARAMS>>>> {
	using FIRST_ONE = TYPE_FIRST_ONE<PARAMS> ;
	using FIRST_REST = TYPE_FIRST_REST<PARAMS> ;
	using FIRST_REST_TUPLE = typename TUPLE_HELP<FIRST_REST ,ALWAYS>::Tuple ;

	struct HEAP {
		FIRST_ONE mOne ;
		FIRST_REST_TUPLE mRest ;
	} ;

	class Tuple {
	protected:
		HEAP mTuple ;

	public:
		implicit Tuple () = default ;

		template <class...ARG1 ,class = REQUIRE<ENUM_NOT<ENUM_ANY<IS_SAME<ARG1 ,Tuple>...>>>>
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

		VREF<FIRST_REST_TUPLE> rest () leftvalue {
			return mTuple.mRest ;
		}

		CREF<FIRST_REST_TUPLE> rest () const leftvalue {
			return mTuple.mRest ;
		}

		VREF<FIRST_ONE> mP1st_m () leftvalue {
			return one () ;
		}

		CREF<FIRST_ONE> mP1st_m () const leftvalue {
			return one () ;
		}

		template <class ARG1 = DEPEND>
		VREF<TYPE_SECOND_ONE<DEPENDENT<PARAMS ,ARG1>>> mP2nd_m () leftvalue {
			return rest ().one () ;
		}

		template <class ARG1 = DEPEND>
		CREF<TYPE_SECOND_ONE<DEPENDENT<PARAMS ,ARG1>>> mP2nd_m () const leftvalue {
			return rest ().one () ;
		}

		template <class ARG1 = DEPEND>
		VREF<TYPE_THIRD_ONE<DEPENDENT<PARAMS ,ARG1>>> mP3rd_m () leftvalue {
			return rest ().rest ().one () ;
		}

		template <class ARG1 = DEPEND>
		CREF<TYPE_THIRD_ONE<DEPENDENT<PARAMS ,ARG1>>> mP3rd_m () const leftvalue {
			return rest ().rest ().one () ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,ARG1>> pick (CREF<TYPEID<ARG1>> id) leftvalue {
			require (ENUM_BETWEEN<ARG1 ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			using R2X = typename FUNCTION_tuple_pick_HELP<PARAMS ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R2X () ;
			return r1x (thiz ,id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,ARG1>> pick (CREF<TYPEID<ARG1>> id) const leftvalue {
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
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_WRAPPER_HELP ;

template <class...>
trait FUNCTION_IMPLHOLDER_HELP ;

template <class RETURN ,class...PARAM>
trait FUNCTION_HELP<RETURN ,TYPEAS<PARAM...> ,ALWAYS> {
	using PARAMS = TYPEAS<PARAM...> ;

	struct Holder implement Interface {
		virtual void initialize (RREF<Auto> value_) = 0 ;
		virtual BOOL effective () const = 0 ;
		virtual BOOL exception () const = 0 ;
		virtual RETURN invoke (XREF<PARAM>...params) const = 0 ;
	} ;

	template <class ARG1>
	using WrapFunction = typename DEPENDENT<FUNCTION_HELP<FUNCTION_RETURN<ARG1> ,TYPE_FIRST_REST<FUNCTION_PARAMS<ARG1>> ,ALWAYS> ,DEPEND>::Function ;

	class Function {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit Function () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Function>>>>
		explicit Function (RREF<ARG1> that) {
			require (IS_FUNCTION<ARG1>) ;
			require (IS_SAME<RETURN ,FUNCTION_RETURN<ARG1>>) ;
			using R2X = typename FUNCTION_IMPLHOLDER_HELP<RETURN ,PARAMS ,ARG1 ,ALWAYS>::ImplHolder ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (IS_EFFECTIVE<ARG1>::expr)
					discard ;
				auto rax = VRef<R2X>::make ().as_cast (TYPEAS<Holder>::expr) ;
				rax->initialize (move (that)) ;
				mThis = rax.as_con () ;
			}
			if ifswitch (rxx) {
				auto rax = VRef<R2X>::make ().as_cast (TYPEAS<Holder>::expr) ;
				rax->initialize (move (that)) ;
				mThis = rax.as_con () ;
			}
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::expr ;
		}

		BOOL effective () const {
			if ifnot (exist ())
				return FALSE ;
			return mThis->effective () ;
		}

		BOOL exception () const {
			if ifnot (exist ())
				return FALSE ;
			return mThis->exception () ;
		}

		RETURN invoke (XREF<PARAM>...params) const {
			return mThis->invoke (forward[TYPEAS<PARAM>::expr] (params)...) ;
		}

		inline RETURN operator() (XREF<PARAM>...params) const {
			return invoke (forward[TYPEAS<PARAM>::expr] (params)...) ;
		}

		template <class ARG1>
		WrapFunction<ARG1> as_wrap (RREF<ARG1> func) {
			require (ENUM_NOT<IS_EFFECTIVE<ARG1>>) ;
			using R1X = FUNCTION_RETURN<ARG1> ;
			using R2X = TYPE_FIRST_REST<FUNCTION_PARAMS<ARG1>> ;
			using R3X = typename FUNCTION_WRAPPER_HELP<R1X ,R2X ,Function ,ARG1 ,ALWAYS>::Wrapper ;
			return WrapFunction<ARG1> (R3X (move (thiz) ,move (func))) ;
		}
	} ;
} ;

template <class RETURN ,class...PARAM ,class UNIT>
trait FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<PARAM...> ,UNIT ,ALWAYS> {
	using PARAMS = TYPEAS<PARAM...> ;
	using Holder = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<UNIT> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<Auto> value_) override {
			mValue = Box<UNIT>::make (value_.poll (TYPEAS<UNIT>::expr)) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<UNIT>::expr ;
		}

		BOOL exception () const override {
			//@warn: always use exception
			return TRUE ;
		}

		RETURN invoke (XREF<PARAM>...params) const override {
			return mValue.self (forward[TYPEAS<PARAM>::expr] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class...PARAM ,class FROM ,class TO>
trait FUNCTION_WRAPPER_HELP<RETURN ,TYPEAS<PARAM...> ,FROM ,TO ,ALWAYS> {
	using PARAMS = TYPEAS<PARAM...> ;
	using Function = typename FUNCTION_HELP<RETURN ,TYPEAS<PARAM...> ,ALWAYS>::Function ;

	class Wrapper {
	protected:
		FROM mFrom ;
		Box<TO> mTo ;

	public:
		implicit Wrapper () = delete ;

		explicit Wrapper (RREF<FROM> from_ ,RREF<TO> to_) {
			mFrom = move (from_) ;
			mTo = Box<TO>::make (move (to_)) ;
		}

		inline RETURN operator() (XREF<PARAM>...params) const {
			return mTo.self (mFrom ,forward[TYPEAS<PARAM>::expr] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class PARAMS = TYPEAS<>>
using Function = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Function ;

template <class...>
trait SCOPEFINALLY_HELP ;

template <class DEPEND>
trait SCOPEFINALLY_HELP<DEPEND ,ALWAYS> {
	class ScopeFinally extend Proxy {
	protected:
		Function<void> mBase ;

	public:
		imports CREF<ScopeFinally> from (CREF<Function<void>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<ScopeFinally>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<ScopeFinally> from (RREF<Function<void>>) = delete ;

		void enter () const {
			assert (mBase.exist ()) ;
		}

		void leave () const {
			mBase.invoke () ;
		}
	} ;
} ;

using ScopeFinally = typename SCOPEFINALLY_HELP<DEPEND ,ALWAYS>::ScopeFinally ;

template <class...>
trait SCOPECOUNTER_HELP ;

template <class DEPEND>
trait SCOPECOUNTER_HELP<DEPEND ,ALWAYS> {
	class ScopeCounter extend Proxy {
	protected:
		LENGTH mBase ;

	public:
		imports VREF<ScopeCounter> from (VREF<LENGTH> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<ScopeCounter>>::expr] (unsafe_deptr (that))) ;
		}

		void enter () {
			mBase++ ;
		}

		void leave () {
			mBase-- ;
		}
	} ;
} ;

using ScopeCounter = typename SCOPECOUNTER_HELP<DEPEND ,ALWAYS>::ScopeCounter ;

template <class...>
trait AUTOREF_HELP ;

template <class...>
trait AUTOREF_HOLDER_HELP ;

template <class...>
trait AUTOREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual Clazz clazz () const = 0 ;
	} ;

	class AutoRef {
	protected:
		VRef<Holder> mThis ;
		FLAG mPointer ;

	public:
		implicit AutoRef () noexcept {
			mPointer = ZERO ;
		}
	} ;
} ;

template <class UNIT>
trait AUTOREF_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::AutoRef ;

	template <class ARG1>
	using CastAutoRef = typename DEPENDENT<AUTOREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::AutoRef ;

	class AutoRef extend SUPER {
	private:
		template <class...>
		friend trait AUTOREF_HELP ;

	protected:
		using SUPER::mThis ;
		using SUPER::mPointer ;

	public:
		implicit AutoRef () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,AutoRef>>>>
		implicit AutoRef (RREF<ARG1> that)
			:AutoRef (forward[TYPEAS<AutoRef>::expr] (that.as_cast (TYPEAS<UNIT>::expr))) {}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;
		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL available () const {
			if (mThis == NULL)
				return FALSE ;
			return TRUE ;
		}

		Clazz clazz () const {
			if ifnot (exist ())
				return Clazz () ;
			return mThis->clazz () ;
		}

		template <class ARG1>
		VREF<CastAutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CastAutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CREF<CastAutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CastAutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CastAutoRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			CastAutoRef<ARG1> ret ;
			if ifswitch (TRUE) {
				ret.mThis = move (mThis) ;
				mPointer = ZERO ;
				if ifnot (ret.available ())
					discard ;
				ret.mPointer = ret.mThis->pointer () ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT>
trait AUTOREF_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_VOID<UNIT>>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::AutoRef ;

	template <class ARG1>
	using CastAutoRef = typename DEPENDENT<AUTOREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::AutoRef ;

	class AutoRef extend SUPER {
	private:
		template <class...>
		friend trait AUTOREF_HELP ;

	protected:
		using SUPER::mThis ;
		using SUPER::mPointer ;

	public:
		implicit AutoRef () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,AutoRef>>>>
		implicit AutoRef (RREF<ARG1> that)
			:AutoRef (forward[TYPEAS<AutoRef>::expr] (that.as_cast (TYPEAS<UNIT>::expr))) {}

		template <class...ARG1>
		imports AutoRef make (XREF<ARG1>...obj) {
			using R1X = typename AUTOREF_IMPLHOLDER_HELP<UNIT ,ALWAYS>::ImplHolder ;
			AutoRef ret ;
			ret.mThis = VRef<R1X>::make () ;
			ret.mThis->initialize () ;
			ret.mPointer = ret.mThis->pointer () ;
			//@warn: depend on it's address
			ret.fake.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;
		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL available () const {
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
			return fake.self ;
		}

		inline implicit operator VREF<UNIT> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return fake.self ;
		}

		inline implicit operator CREF<UNIT> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		VREF<CastAutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CastAutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CREF<CastAutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CastAutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CastAutoRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			CastAutoRef<ARG1> ret ;
			if ifswitch (TRUE) {
				ret.mThis = move (mThis) ;
				mPointer = ZERO ;
				if ifnot (ret.available ())
					discard ;
				ret.mPointer = ret.mThis->pointer () ;
			}
			return move (ret) ;
		}

	private:
		VREF<Box<UNIT>> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Box<UNIT>>>::expr] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <class UNIT>
trait AUTOREF_IMPLHOLDER_HELP<UNIT ,ALWAYS> {
	using Holder = typename AUTOREF_HELP<UNIT ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<UNIT> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			noop () ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPEAS<UNIT>::expr) ;
		}
	} ;
} ;

template <class UNIT = void>
using AutoRef = typename AUTOREF_HELP<UNIT ,ALWAYS>::AutoRef ;

template <class...>
trait SHAREDREF_HELP ;

template <class...>
trait SHAREDREF_HOLDER_HELP ;

template <class...>
trait SHAREDREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
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
	} ;
} ;

template <class UNIT>
trait SHAREDREF_HELP<UNIT ,ALWAYS> {
	using Holder = typename SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS>::SharedRef ;

	class SharedRef extend SUPER {
	protected:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit SharedRef () = default ;

		template <class...ARG1>
		imports SharedRef make (XREF<ARG1>...obj) {
			using R1X = typename SHAREDREF_IMPLHOLDER_HELP<UNIT ,ALWAYS>::ImplHolder ;
			SharedRef ret ;
			auto rax = VRef<R1X>::make ().as_cast (TYPEAS<Holder>::expr) ;
			rax->initialize () ;
			ret.mThis = rax.as_con () ;
			ret.mPointer = ret.mThis->pointer () ;
			//@warn: depend on it's address
			ret.fake.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		implicit SharedRef (CREF<SharedRef> that) {
			mThis = that.mThis ;
			if ifnot (that.exist ())
				return ;
			assert (available ()) ;
			mPointer = mThis->pointer () ;
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

		BOOL available () const {
			if (mThis == NULL)
				return FALSE ;
			if (mThis->counter () == ZERO)
				return FALSE ;
			return TRUE ;
		}

		VREF<UNIT> self_m () const leftvalue {
			return fake.self ;
		}

		inline implicit operator VREF<UNIT> () const leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

		SharedRef weak () const {
			SharedRef ret ;
			ret.mThis = mThis ;
			return move (ret) ;
		}

	private:
		VREF<Box<UNIT>> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Box<UNIT>>>::expr] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <class UNIT>
trait SHAREDREF_IMPLHOLDER_HELP<UNIT ,ALWAYS> {
	using Holder = typename SHAREDREF_HELP<UNIT ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mCounter ;
		Box<UNIT> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mCounter = 0 ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		LENGTH counter () const override {
			return mCounter ;
		}

		void enter () override {
			//@error: not thread-safe
			mCounter++ ;
		}

		void leave () override {
			//@error: not thread-safe
			mCounter-- ;
			if (mCounter > 0)
				return ;
			mValue = NULL ;
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
trait UNIQUEREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<Auto> destructor) = 0 ;
		virtual FLAG pointer () const = 0 ;
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
	using SUPER = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::UniqueRef ;

	class UniqueRef extend SUPER {
	protected:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (RREF<ARG1> constructor ,RREF<ARG2> destructor) {
			using R1X = Function<void> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT ,ALWAYS>::ImplHolder ;
			mThis = VRef<R2X>::make () ;
			auto rax = Box<R1X>::make (move (destructor)) ;
			assert (ifnot (rax->effective ())) ;
			mThis->initialize (move (rax.self)) ;
			mPointer = mThis->pointer () ;
			constructor () ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			return mPointer != ZERO ;
		}

		inline BOOL operator== (CREF<typeof (NULL)>) = delete ;
		inline BOOL operator!= (CREF<typeof (NULL)>) = delete ;

		BOOL available () const {
			if (mThis == NULL)
				return FALSE ;
			return TRUE ;
		}
	} ;
} ;

template <class UNIT>
trait UNIQUEREF_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_VOID<UNIT>>>> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::UniqueRef ;

	class UniqueRef extend SUPER {
	protected:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (RREF<ARG1> constructor ,RREF<ARG2> destructor) {
			using R1X = Function<void ,TYPEAS<VREF<UNIT>>> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT ,ALWAYS>::ImplHolder ;
			mThis = VRef<R2X>::make () ;
			auto rax = Box<R1X>::make (move (destructor)) ;
			assert (ifnot (rax->effective ())) ;
			mThis->initialize (move (rax.self)) ;
			mPointer = mThis->pointer () ;
			//@warn: depend on it's address
			fake.acquire (TYPEAS<UNIT>::expr) ;
			constructor (fake.self) ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		template <class...ARG1>
		imports UniqueRef make (XREF<ARG1>...obj) {
			using R1X = Function<void ,TYPEAS<VREF<UNIT>>> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT ,ALWAYS>::ImplHolder ;
			UniqueRef ret ;
			ret.mThis = VRef<R2X>::make () ;
			auto rax = Box<R1X>::make ([] (VREF<UNIT>) {}) ;
			assert (ifnot (rax->effective ())) ;
			ret.mThis->initialize (move (rax.self)) ;
			ret.mPointer = ret.mThis->pointer () ;
			//@warn: depend on it's address
			ret.fake.acquire (TYPEAS<UNIT>::expr ,forward[TYPEAS<ARG1>::expr] (obj)...) ;
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

		BOOL available () const {
			if (mThis == NULL)
				return FALSE ;
			return TRUE ;
		}

		CREF<UNIT> self_m () const leftvalue {
			return fake.self ;
		}

		inline implicit operator CREF<UNIT> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT>> operator-> () const leftvalue {
			return (&self) ;
		}

	private:
		VREF<Box<UNIT>> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Box<UNIT>>>::expr] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <class UNIT>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT ,REQUIRE<IS_VOID<UNIT>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Function<void> mDestructor ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<Auto> destructor) override {
			mDestructor = destructor.poll (TYPEAS<Function<void>>::expr) ;
		}

		FLAG pointer () const override {
			return ZERO ;
		}

		void enter () override {
			assert (mDestructor.exist ()) ;
		}

		void leave () override {
			mDestructor () ;
		}
	} ;
} ;

template <class UNIT>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT ,REQUIRE<ENUM_NOT<IS_VOID<UNIT>>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Function<void ,TYPEAS<VREF<UNIT>>> mDestructor ;
		Box<UNIT> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<Auto> destructor) override {
			mDestructor = destructor.poll (TYPEAS<Function<void ,TYPEAS<VREF<UNIT>>>>::expr) ;
		}

		FLAG pointer () const override {
			return address (mValue) ;
		}

		void enter () override {
			assert (mDestructor.exist ()) ;
		}

		void leave () override {
			mDestructor (mValue) ;
		}
	} ;
} ;

template <class UNIT = void>
using UniqueRef = typename UNIQUEREF_HELP<UNIT ,ALWAYS>::UniqueRef ;

template <class...>
trait BUFFER_HELP ;

template <class...>
trait BOXBUFFER_HOLDER_HELP ;

template <class ITEM ,class SIZE ,class ATTR>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,ATTR ,REQUIRE<ENUM_NOT<ATTR>>> {
	class Buffer {
	protected:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE ,class ATTR>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,ATTR ,REQUIRE<ATTR>> {
	using SUPER = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::Buffer ;

	class Buffer extend SUPER {
	protected:
		using SUPER::mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
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
	using SUPER = typename BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;

	class Buffer extend SUPER {
	protected:
		using SUPER::mBuffer ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = DEPEND>
		implicit Buffer (CREF<DEPENDENT<ARR<ITEM ,SIZE> ,ARG1>> that) {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (keep[TYPEAS<CREF<ARR<ITEM ,SIZE>>>::expr] (that)) ;
		}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			assert (size_ <= SIZE::expr) ;
			mBuffer = Box<ARR<ITEM ,SIZE>>::make () ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return SIZE::expr ;
		}

		LENGTH align () const {
			if (mBuffer == NULL)
				return ZERO ;
			return SIZE_OF<ITEM>::expr ;
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
			assert (FALSE) ;
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			return BufferProc::buf_equal (mBuffer.self ,that.mBuffer.self ,0 ,size ()) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			const auto r1x = vmin (size () ,that.size ()) ;
			return BufferProc::buf_compr (mBuffer.self ,that.mBuffer.self ,0 ,r1x) ;
		}

		inline BOOL operator< (CREF<Buffer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Buffer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Buffer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Buffer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return BufferProc::buf_hash (mBuffer.self ,0 ,size ()) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
using Buffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE ,class = REQUIRE<ENUM_GT_ZERO<SIZE>>>
using BoxBuffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class...>
trait VARBUFFER_HOLDER_HELP ;

template <class ITEM ,class ATTR>
trait VARBUFFER_HOLDER_HELP<ITEM ,ATTR ,REQUIRE<ENUM_NOT<ATTR>>> {
	class Buffer {
	protected:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;
		LENGTH mAlign ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
			mAlign = 0 ;
		}
	} ;
} ;

template <class ITEM ,class ATTR>
trait VARBUFFER_HOLDER_HELP<ITEM ,ATTR ,REQUIRE<ATTR>> {
	using SUPER = typename VARBUFFER_HOLDER_HELP<ITEM ,ENUM_FALSE ,ALWAYS>::Buffer ;

	class Buffer extend SUPER {
	protected:
		using SUPER::mBuffer ;
		using SUPER::mSize ;
		using SUPER::mAlign ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mBuffer == NULL)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
			BufferProc::buf_copy (mBuffer.self ,that.mBuffer.self ,0 ,that.mSize) ;
			mSize = that.mSize ;
			mAlign = that.mAlign ;
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
	class Buffer ;

	using SUPER = typename VARBUFFER_HOLDER_HELP<ITEM ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;
	using ConBuffer = typename BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS>::Buffer ;

	template <class ARG1>
	using CastVarBuffer = typename DEPENDENT<BUFFER_HELP<ARG1 ,VARIABLE ,ALWAYS> ,DEPEND>::Buffer ;

	class Buffer extend SUPER {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	protected:
		using SUPER::mBuffer ;
		using SUPER::mSize ;
		using SUPER::mAlign ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = DEPEND>
		implicit Buffer (RREF<DEPENDENT<ConBuffer ,ARG1>> that)
			:Buffer (that.as_var ()) {}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
			mAlign = SIZE_OF<ITEM>::expr ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		LENGTH align () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mAlign ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mBuffer == NULL)
					discard ;
				if (size_ == 0)
					discard ;
				auto rax = VRef<ARR<ITEM>>::make (size_) ;
				const auto r1x = vmin (size_ ,mSize) ;
				BufferProc::buf_move (rax.self ,mBuffer.self ,0 ,r1x) ;
				mBuffer = move (rax) ;
				mSize = size_ ;
				mAlign = SIZE_OF<ITEM>::expr ;
			}
			if ifswitch (rxx) {
				if (size_ == 0)
					discard ;
				mBuffer = VRef<ARR<ITEM>>::make (size_) ;
				mSize = size_ ;
				mAlign = SIZE_OF<ITEM>::expr ;
			}
			if ifswitch (rxx) {
				mBuffer = NULL ;
				mSize = 0 ;
				mAlign = 0 ;
			}
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			return BufferProc::buf_equal (mBuffer.self ,that.mBuffer.self ,0 ,size ()) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			const auto r1x = vmin (size () ,that.size ()) ;
			return BufferProc::buf_compr (mBuffer.self ,that.mBuffer.self ,0 ,r1x) ;
		}

		inline BOOL operator< (CREF<Buffer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Buffer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Buffer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Buffer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return BufferProc::buf_hash (mBuffer.self ,0 ,size ()) ;
		}

		ConBuffer as_con () {
			ConBuffer ret ;
			ret.mBuffer = mBuffer.as_con () ;
			ret.mSize = mSize ;
			ret.mAlign = mAlign ;
			return move (ret) ;
		}

		template <class ARG1>
		CastVarBuffer<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			require (ENUM_ANY<IS_SAME<ARG1 ,BYTE_BASE<ITEM>> ,IS_SAME<ARG1 ,TEXT_BASE<ITEM>>>) ;
			CastVarBuffer<ARG1> ret ;
			if ifswitch (TRUE) {
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VRef<ARR<ARG1>>>>::expr] (unsafe_deptr (mBuffer))) ;
				ret.mBuffer = move (tmp) ;
				ret.mSize = mSize ;
				ret.mAlign = mAlign ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using VarBuffer = typename BUFFER_HELP<ITEM ,VARIABLE ,ALWAYS>::Buffer ;

template <class...>
trait CONBUFFER_HOLDER_HELP ;

template <class ITEM>
trait CONBUFFER_HOLDER_HELP<ITEM ,ALWAYS> {
	class Buffer {
	protected:
		CRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;
		LENGTH mAlign ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
			mAlign = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,CONSTANT>>> {
	using SUPER = typename CONBUFFER_HOLDER_HELP<ITEM ,ALWAYS>::Buffer ;
	using VarBuffer = typename BUFFER_HELP<ITEM ,VARIABLE ,ALWAYS>::Buffer ;

	template <class ARG1>
	using CastConBuffer = typename DEPENDENT<BUFFER_HELP<ARG1 ,CONSTANT ,ALWAYS> ,DEPEND>::Buffer ;

	class Buffer extend SUPER {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	protected:
		using SUPER::mBuffer ;
		using SUPER::mSize ;
		using SUPER::mAlign ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = DEPEND>
		implicit Buffer (RREF<DEPENDENT<VarBuffer ,ARG1>> that)
			:Buffer (that.as_con ()) {}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = CRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
			mAlign = SIZE_OF<ITEM>::expr ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		LENGTH align () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mAlign ;
		}

		CREF<ARR<ITEM>> self_m () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::expr) ;
			return mBuffer.self ;
		}

		inline implicit operator CREF<ARR<ITEM>> () const leftvalue {
			return self ;
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
			assert (FALSE) ;
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			return BufferProc::buf_equal (mBuffer.self ,that.mBuffer.self ,0 ,size ()) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			const auto r1x = vmin (size () ,that.size ()) ;
			return BufferProc::buf_compr (mBuffer.self ,that.mBuffer.self ,0 ,r1x) ;
		}

		inline BOOL operator< (CREF<Buffer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Buffer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Buffer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Buffer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return BufferProc::buf_hash (mBuffer.self ,0 ,size ()) ;
		}

		VarBuffer as_var () {
			VarBuffer ret ;
			ret.mBuffer = mBuffer.as_var () ;
			ret.mSize = mSize ;
			ret.mAlign = mAlign ;
			return move (ret) ;
		}

		template <class ARG1>
		CastConBuffer<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			require (ENUM_ANY<IS_SAME<ARG1 ,BYTE_BASE<ITEM>> ,IS_SAME<ARG1 ,TEXT_BASE<ITEM>>>) ;
			CastConBuffer<ARG1> ret ;
			if ifswitch (TRUE) {
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRef<ARR<ARG1>>>>::expr] (unsafe_deptr (mBuffer))) ;
				ret.mBuffer = move (tmp) ;
				ret.mSize = mSize ;
				ret.mAlign = mAlign ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using ConBuffer = typename BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,REGISTER>>> {
	struct UNNAMED ;

	class Buffer extend Proxy {
	protected:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;
		LENGTH mAlign ;

	public:
		imports VREF<Buffer> from (VREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (CREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (RREF<VarBuffer<ITEM>>) = delete ;

		imports CREF<Buffer> from (CREF<ConBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (RREF<ConBuffer<ITEM>>) = delete ;

		imports VREF<Buffer> make (VREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<UNNAMED> unnamed = UNNAMED ()) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<ITEM>>>::expr] (buffer)) ;
			return make (tmp ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports VREF<Buffer> make (VREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<UNNAMED> unnamed = UNNAMED ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VarBuffer<ITEM>>>::expr] (unsafe_deptr (unnamed))) ;
			if ifswitch (TRUE) {
				const auto r1x = vmax (end_ - begin_ ,ZERO) ;
				if (r1x == ZERO)
					discard ;
				tmp.mBuffer = VRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
				tmp.mSize = r1x ;
				tmp.mAlign = SIZE_OF<ITEM>::expr ;
			}
			return from (tmp) ;
		}

		imports CREF<Buffer> make (CREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<UNNAMED> unnamed = UNNAMED ()) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<ITEM>>>::expr] (buffer)) ;
			return make (tmp ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports CREF<Buffer> make (CREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,RREF<UNNAMED> unnamed = UNNAMED ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ConBuffer<ITEM>>>::expr] (unsafe_deptr (unnamed))) ;
			if ifswitch (TRUE) {
				const auto r1x = vmax (end_ - begin_ ,ZERO) ;
				if (r1x == ZERO)
					discard ;
				tmp.mBuffer = CRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
				tmp.mSize = r1x ;
				tmp.mAlign = SIZE_OF<ITEM>::expr ;
			}
			return from (tmp) ;
		}

		VRef<Buffer> ref () leftvalue {
			auto rax = VRef<VarBuffer<ITEM>>::make () ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				rax->mBuffer = VRef<ARR<ITEM>>::reference (mBuffer.self) ;
				rax->mSize = mSize ;
				rax->mAlign = SIZE_OF<ITEM>::expr ;
			}
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VRef<Buffer>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}

		CRef<Buffer> ref () const leftvalue {
			auto rax = VRef<ConBuffer<ITEM>>::make () ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				rax->mBuffer = CRef<ARR<ITEM>>::reference (mBuffer.self) ;
				rax->mSize = mSize ;
				rax->mAlign = mAlign ;
			}
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRef<Buffer>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		LENGTH align () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mAlign ;
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
			assert (FALSE) ;
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			return BufferProc::buf_equal (mBuffer.self ,that.mBuffer.self ,0 ,size ()) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			const auto r1x = vmin (size () ,that.size ()) ;
			return BufferProc::buf_compr (mBuffer.self ,that.mBuffer.self ,0 ,r1x) ;
		}

		inline BOOL operator< (CREF<Buffer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Buffer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Buffer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Buffer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return BufferProc::buf_hash (mBuffer.self ,0 ,size ()) ;
		}
	} ;

	struct UNNAMED {
		VarBuffer<ITEM> mUnnamed ;
	} ;
} ;

template <class ITEM>
using RegBuffer = typename BUFFER_HELP<ITEM ,REGISTER ,ALWAYS>::Buffer ;

template <class...>
trait DYNBUFFER_IMPLHOLDER_HELP ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,DYNAMICS>>> {
	struct Holder implement Interface {
		virtual void initialize (RREF<Auto> value_) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH align () const = 0 ;
		virtual VREF<ITEM> at (CREF<INDEX> index) leftvalue = 0 ;
		virtual CREF<ITEM> at (CREF<INDEX> index) const leftvalue = 0 ;
	} ;

	class Buffer {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Buffer>>>>
		explicit Buffer (RREF<ARG1> that) {
			require (IS_CLASS<ARG1>) ;
			using R1X = typename DYNBUFFER_IMPLHOLDER_HELP<ITEM ,ARG1 ,ALWAYS>::ImplHolder ;
			mThis = VRef<R1X>::make () ;
			mThis->initialize (move (that)) ;
		}

		LENGTH size () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->size () ;
		}

		LENGTH align () const {
			if (mThis == NULL)
				return ZERO ;
			return mThis->align () ;
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
			assert (FALSE) ;
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			for (auto &&i : iter (0 ,size ())) {
				if ifnot (operator_equal (mThis->at (i) ,that.mThis->at (i)))
					return FALSE ;
			}
			return TRUE ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			const auto r1x = vmin (size () ,that.size ()) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = operator_compr (mThis->at (i) ,that.mThis->at (i)) ;
				if (r2x != ZERO)
					return r2x ;
			}
			return ZERO ;
		}

		inline BOOL operator< (CREF<Buffer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Buffer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Buffer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Buffer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			FLAG ret = hashcode () ;
			for (auto &&i : iter (0 ,size ())) {
				const auto r1x = operator_hash (mThis->at (i)) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class UNIT>
trait DYNBUFFER_IMPLHOLDER_HELP<ITEM ,UNIT ,ALWAYS> {
	using Holder = typename BUFFER_HELP<ITEM ,DYNAMICS ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<UNIT> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<Auto> value_) override {
			mValue = Box<UNIT>::make (value_.poll (TYPEAS<UNIT>::expr)) ;
		}

		LENGTH size () const override {
			return LENGTH (mValue->size ()) ;
		}

		LENGTH align () const override {
			return SIZE_OF<ITEM>::expr ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue override {
			return mValue.self[index] ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue override {
			return mValue.self[index] ;
		}
	} ;
} ;

template <class ITEM>
using DynBuffer = typename BUFFER_HELP<ITEM ,DYNAMICS ,ALWAYS>::Buffer ;

template <class...>
trait ALLOCATOR_HELP ;

template <class...>
trait ALLOCATOR_HOLDER_HELP ;

template <class ITEM ,class SIZE ,class ATTR>
trait ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,ATTR ,REQUIRE<ENUM_NOT<ATTR>>> {
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

template <class ITEM ,class SIZE ,class ATTR>
trait ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,ATTR ,REQUIRE<ATTR>> {
	using NODE = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::NODE ;
	using SUPER = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,ENUM_FALSE ,ALWAYS>::Allocator ;

	class Allocator extend SUPER {
	protected:
		using SUPER::mAllocator ;
		using SUPER::mSize ;
		using SUPER::mLength ;
		using SUPER::mFree ;

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
	using SUPER = typename ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,IS_CLONEABLE<ITEM> ,ALWAYS>::Allocator ;

	using RESIZE_POW = RANK2 ;
	using RESIZE_MIN_SIZE = ENUMAS<VAL ,ENUMID<256>> ;

	class Allocator extend SUPER {
	protected:
		using SUPER::mAllocator ;
		using SUPER::mSize ;
		using SUPER::mLength ;
		using SUPER::mFree ;

	public:
		implicit Allocator () = default ;

		explicit Allocator (CREF<LENGTH> size_) {
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

		LENGTH align () const {
			return mAllocator.align () ;
		}

		LENGTH length () const {
			if (mAllocator.size () == 0)
				return ZERO ;
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
			return mAllocator[index].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (used (index)) ;
			return mAllocator[index].mItem ;
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
		virtual void initialize (CREF<FLAG> tag) = 0 ;
		virtual void initialize (CREF<FLAG> tag ,RREF<Function<Auto>> expr_) = 0 ;
		virtual BOOL exist () const = 0 ;
		virtual Auto invoke () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;
} ;

template <class UNIT>
trait LATER_HELP<UNIT ,ALWAYS> {
	using Holder = typename LATER_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename LATER_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	class Later {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Later () = default ;

		explicit Later (CREF<FLAG> tag) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (tag) ;
		}

		explicit Later (CREF<FLAG> tag ,RREF<Function<UNIT>> expr_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (tag ,expr_.as_wrap ([] (CREF<Function<UNIT>> old) {
				return Auto (old ()) ;
			})) ;
		}

		BOOL exist () const {
			return mThis->exist () ;
		}

		UNIT invoke () const {
			return mThis->invoke ().poll (TYPEAS<UNIT>::expr) ;
		}

		inline UNIT operator() () const {
			return invoke () ;
		}
	} ;
} ;

template <class UNIT>
using Later = typename LATER_HELP<UNIT ,ALWAYS>::Later ;

template <class...>
trait INTEGER_HELP ;

template <class...>
trait INTEGER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait INTEGER_HELP<DEPEND ,ALWAYS> {
	class Integer ;

	struct Holder implement Interface {
		virtual void initialize (CREF<VAL64> value_ ,CREF<LENGTH> size_) = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
		virtual Integer add (CREF<Holder> that) const = 0 ;
		virtual Integer sub (CREF<Holder> that) const = 0 ;
		virtual Integer mul (CREF<LENGTH> scale) const = 0 ;
		virtual Integer div (CREF<LENGTH> scale) const = 0 ;
		virtual Integer mod (CREF<LENGTH> scale) const = 0 ;
		virtual void friend_clone (VREF<Integer> that) const = 0 ;
		virtual Integer minus () const = 0 ;
		virtual void increase () = 0 ;
		virtual void decrease () = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		VarBuffer<BYTE> mInteger ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class Integer {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit Integer () = default ;

		explicit Integer (CREF<VAL32> value_ ,CREF<LENGTH> size_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (value_ ,size_) ;
		}

		explicit Integer (CREF<VAL64> value_ ,CREF<LENGTH> size_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (value_ ,size_) ;
		}

		explicit Integer (RREF<Box<FakeHolder>> that) {
			mThis = move (that) ;
		}

		implicit Integer (CREF<Integer> that) {
			that.mThis->friend_clone (thiz) ;
		}

		inline VREF<Integer> operator= (CREF<Integer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Integer (RREF<Integer> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<Integer> operator= (RREF<Integer> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL equal (CREF<Integer> that) const {
			return mThis->equal (that.mThis) ;
		}

		inline BOOL operator== (CREF<Integer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Integer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Integer> that) const {
			return mThis->compr (that.mThis) ;
		}

		inline BOOL operator< (CREF<Integer> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Integer> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Integer> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Integer> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return mThis->hash () ;
		}

		Integer add (CREF<Integer> that) const {
			return mThis->add (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline Integer operator+ (CREF<Integer> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<Integer> that) {
			thiz = add (that) ;
		}

		Integer sub (CREF<Integer> that) const {
			return mThis->sub (keep[TYPEAS<CREF<Holder>>::expr] (that.mThis)) ;
		}

		inline Integer operator- (CREF<Integer> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<Integer> that) {
			thiz = sub (that) ;
		}

		Integer mul (CREF<LENGTH> scale) const {
			return mThis->mul (scale) ;
		}

		inline Integer operator* (CREF<LENGTH> scale) const {
			return mul (scale) ;
		}

		inline void operator*= (CREF<LENGTH> scale) {
			thiz = mul (scale) ;
		}

		Integer div (CREF<LENGTH> scale) const {
			return mThis->div (scale) ;
		}

		inline Integer operator/ (CREF<LENGTH> scale) const {
			return div (scale) ;
		}

		inline void operator/= (CREF<LENGTH> scale) {
			thiz = div (scale) ;
		}

		Integer mod (CREF<LENGTH> scale) const {
			return mThis->mod (scale) ;
		}

		inline Integer operator% (CREF<LENGTH> scale) const {
			return mod (scale) ;
		}

		inline void operator%= (CREF<LENGTH> scale) {
			thiz = mod (scale) ;
		}

		inline Integer operator+ () const {
			return thiz ;
		}

		Integer minus () const {
			return mThis->minus () ;
		}

		inline Integer operator- () const {
			return minus () ;
		}

		void increase () {
			return mThis->increase () ;
		}

		inline void operator++ (VAL32) {
			increase () ;
		}

		void decrease () {
			return mThis->decrease () ;
		}

		inline void operator-- (VAL32) {
			decrease () ;
		}
	} ;
} ;

using Integer = typename INTEGER_HELP<DEPEND ,ALWAYS>::Integer ;
} ;
