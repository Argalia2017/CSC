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

template <class UNIT1 ,class CLONEABLE>
trait OPTIONAL_HOLDER_HELP<UNIT1 ,CLONEABLE ,REQUIRE<ENUM_NOT<CLONEABLE>>> {
	class Optional {
	protected:
		Box<UNIT1> mSome ;
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mCode = ZERO ;
		}
	} ;
} ;

template <class UNIT1 ,class CLONEABLE>
trait OPTIONAL_HOLDER_HELP<UNIT1 ,CLONEABLE ,REQUIRE<CLONEABLE>> {
	using SUPER = typename OPTIONAL_HOLDER_HELP<UNIT1 ,ENUM_FALSE ,ALWAYS>::Optional ;

	class Optional extend SUPER {
	protected:
		using SUPER::mSome ;
		using SUPER::mCode ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<Optional> that) {
			mSome = Box<UNIT1>::make (that.mSome.self) ;
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

template <class UNIT1>
trait OPTIONAL_HELP<UNIT1 ,ALWAYS> {
	using SUPER = typename OPTIONAL_HOLDER_HELP<UNIT1 ,IS_CLONEABLE<UNIT1> ,ALWAYS>::Optional ;

	template <class ARG1>
	using CRTP_Optional = typename DEPENDENT<OPTIONAL_HELP<ARG1 ,ALWAYS> ,DEPEND>::Optional ;

	class Optional extend SUPER {
	protected:
		using SUPER::mSome ;
		using SUPER::mCode ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<FLAG> that) {
			mCode = that ;
		}

		template <class...ARG1>
		imports Optional make (XREF<ARG1>...obj) {
			Optional ret ;
			ret.mSome = Box<UNIT1>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
			ret.mCode = USED ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mSome == NULL)
				return FALSE ;
			if (mCode != USED)
				return FALSE ;
			return TRUE ;
		}

		FLAG code () const {
			return mCode ;
		}

		UNIT1 poll () {
			assume (exist ()) ;
			UNIT1 ret = move (mSome.self) ;
			mSome = NULL ;
			mCode = USED ;
			return move (ret) ;
		}

		UNIT1 fetch () const {
			assert (exist ()) ;
			return mSome.self ;
		}

		UNIT1 fetch (CREF<UNIT1> def) const {
			if ifnot (exist ())
				return move (def) ;
			return mSome.self ;
		}

		UNIT1 fetch (RREF<UNIT1> def) const {
			if ifnot (exist ())
				return move (def) ;
			return mSome.self ;
		}

		BOOL equal (CREF<Optional> that) const {
			const auto r1x = exist () ;
			const auto r2x = that.exist () ;
			if (r1x != r2x)
				return FALSE ;
			if ifnot (r1x)
				return TRUE ;
			return operator_equal (mSome.self ,that.mSome.self) ;
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
			return operator_compr (mSome.self ,that.mSome.self) ;
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
			return operator_hash (mSome.self) ;
		}
	} ;
} ;

template <class UNIT1>
using Optional = typename OPTIONAL_HELP<UNIT1 ,ALWAYS>::Optional ;

template <class...>
trait BUFFERPROC_HELP ;

template <class DEPEND>
trait BUFFERPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {} ;

	class BufferProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<BufferProc> instance () {
			return memorize ([&] () {
				BufferProc ret ;
				ret.mThis = NULL ;
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
		imports void buf_move (VREF<ARG1> dst ,CREF<INDEX> offset ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (offset < 0)
					discard ;
				for (auto &&i : iter (begin_ ,end_))
					dst[i] = move (dst[i + offset]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,end_ - begin_)) {
					INDEX ix = end_ - 1 - i ;
					dst[ix] = move (dst[ix + offset]) ;
				}
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
		VREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,VREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPEAS<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		VREF<TYPE_FIRST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,VREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			return tuple.one () ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_GT_ZERO<ARG2>>>
		CREF<TYPE_PICK<PARAMS ,ARG2>> template_pick (CREF<typeof (PH2)> ,CREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			using R1X = typename FUNCTION_tuple_pick_HELP<TYPE_FIRST_REST<PARAMS> ,ALWAYS>::FUNCTION_tuple_pick ;
			const auto r1x = R1X () ;
			return r1x (tuple.rest () ,TYPEAS<ENUM_DEC<ARG2>>::expr) ;
		}

		template <class ARG1 ,class ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<ARG2>>>
		CREF<TYPE_FIRST_ONE<PARAMS>> template_pick (CREF<typeof (PH1)> ,CREF<ARG1> tuple ,CREF<TYPEID<ARG2>> id) const {
			return tuple.one () ;
		}
	} ;
} ;

template <class...>
trait TUPLE_HELP ;

template <class UNIT1>
trait TUPLE_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
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

		void assign (CREF<FIRST_ONE> obj1) {
			one () = move (obj1) ;
		}

		void assign (RREF<FIRST_ONE> obj1) {
			one () = move (obj1) ;
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

template <class...PARAMS>
using Tuple = typename TUPLE_HELP<TYPEAS<PARAMS...> ,ALWAYS>::Tuple ;

template <class...>
trait GENERIC_HELP ;

template <class...>
trait GENERIC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait GENERIC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<FLAG> addr ,CREF<FLAG> qualifier) = 0 ;
		virtual FLAG pointer () const = 0 ;
		virtual BOOL is_variable () const = 0 ;
		virtual BOOL is_constant () const = 0 ;
		virtual BOOL is_register () const = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		FLAG mPointer ;
		FLAG mQualifier ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class Generic {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit Generic () = delete ;

		template <class ARG1>
		explicit Generic (VREF<ARG1> that) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (address (that) ,VARIABLE::expr) ;
		}

		template <class ARG1>
		explicit Generic (CREF<ARG1> that) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (address (that) ,CONSTANT::expr) ;
		}

		template <class ARG1>
		explicit Generic (RREF<ARG1> that) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (address (that) ,REGISTER::expr) ;
		}

		FLAG pointer () const {
			return mThis->pointer () ;
		}

		BOOL is_variable () const {
			return mThis->is_variable () ;
		}

		BOOL is_constant () const {
			return mThis->is_constant () ;
		}

		BOOL is_register () const {
			return mThis->is_register () ;
		}
	} ;
} ;

using Generic = typename GENERIC_HELP<DEPEND ,ALWAYS>::Generic ;

template <class...>
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_WRAPPER_HELP ;

template <class...>
trait FUNCTION_IMPLHOLDER_HELP ;

template <class RETURN ,class...UNIT1>
trait FUNCTION_HELP<RETURN ,TYPEAS<UNIT1...> ,ALWAYS> {
	using PARAMS = TYPEAS<UNIT1...> ;

	struct Holder implement Interface {
		virtual void initialize (CREF<TEMP<void>> functor) = 0 ;
		virtual BOOL effective () const = 0 ;
		virtual BOOL exception () const = 0 ;
		virtual RETURN invoke (XREF<UNIT1>...params) const = 0 ;
	} ;

	using AUTO_RETURN = CONDITIONAL<IS_VOID<RETURN> ,RETURN ,Auto> ;

	template <class ARG1>
	using CRTP_Function = typename DEPENDENT<FUNCTION_HELP<AUTO_RETURN ,TYPEAS<CREF<DEPENDENT<Generic ,UNIT1>>...> ,ALWAYS> ,ARG1>::Function ;

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
				mThis = memorize ([&] () {
					auto rax = VRef<R2X>::make ().as_cast (TYPEAS<Holder>::expr) ;
					auto rbx = Box<ARG1>::make (move (that)) ;
					rax->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rbx.self))) ;
					rbx.release () ;
					return rax.as_cref () ;
				}) ;
			}
			if ifswitch (rxx) {
				auto rax = VRef<R2X>::make ().as_cast (TYPEAS<Holder>::expr) ;
				auto rbx = Box<ARG1>::make (move (that)) ;
				rax->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rbx.self))) ;
				rbx.release () ;
				mThis = rax.as_cref () ;
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

		RETURN invoke (XREF<UNIT1>...params) const {
			return mThis->invoke (forward[TYPEAS<UNIT1>::expr] (params)...) ;
		}

		inline RETURN operator() (XREF<UNIT1>...params) const {
			return invoke (forward[TYPEAS<UNIT1>::expr] (params)...) ;
		}

		template <class ARG1 = DEPEND>
		CRTP_Function<ARG1> as_generic () {
			using R1X = typename FUNCTION_WRAPPER_HELP<RETURN ,PARAMS ,ALWAYS>::Wrapper ;
			return CRTP_Function<ARG1> (R1X (move (thiz))) ;
		}
	} ;

} ;

template <class RETURN ,class...UNIT1 ,class UNIT2>
trait FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<UNIT1...> ,UNIT2 ,ALWAYS> {
	using PARAMS = TYPEAS<UNIT1...> ;
	using Holder = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<UNIT2> mFunctor ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<TEMP<void>> functor) override {
			mFunctor.acquire (functor) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<UNIT2>::expr ;
		}

		BOOL exception () const override {
			//@warn: always use exception
			return TRUE ;
		}

		RETURN invoke (XREF<UNIT1>...params) const override {
			return mFunctor.self (forward[TYPEAS<UNIT1>::expr] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class...UNIT1>
trait FUNCTION_WRAPPER_HELP<RETURN ,TYPEAS<UNIT1...> ,REQUIRE<IS_VOID<RETURN>>> {
	using PARAMS = TYPEAS<UNIT1...> ;
	using Function = typename FUNCTION_HELP<RETURN ,TYPEAS<UNIT1...> ,ALWAYS>::Function ;

	class Wrapper {
	protected:
		Function mThis ;

	public:
		implicit Wrapper () = delete ;

		explicit Wrapper (RREF<Function> that) {
			mThis = move (that) ;
		}

		inline void operator() (CREF<DEPENDENT<Generic ,UNIT1>>...params) const {
			return template_invoke (PHX ,TYPEAS<PARAMS>::expr ,params...) ;
		}

		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_GT_ZERO<COUNT_OF<ARG1>>>>
		void template_invoke (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id ,CREF<Generic> param1 ,XREF<ARG2>...params) const {
			using R1X = TYPE_FIRST_ONE<ARG1> ;
			using R2X = REMOVE_REF<R1X> ;
			assume (IS_VARIABLE<R1X>::expr == param1.is_variable ()) ;
			assume (IS_REGISTER<R1X>::expr == param1.is_register ()) ;
			const auto r1x = param1.pointer () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::expr] (unsafe_pointer (r1x))) ;
			return template_invoke (PHX ,TYPEAS<TYPE_FIRST_REST<ARG1>>::expr ,params... ,keep[TYPEAS<XREF<R1X>>::expr] (tmp)) ;
		}

		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<COUNT_OF<ARG1>>>>
		void template_invoke (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id ,XREF<ARG2>...params) const {
			return mThis (params...) ;
		}
	} ;
} ;

template <class RETURN ,class...UNIT1>
trait FUNCTION_WRAPPER_HELP<RETURN ,TYPEAS<UNIT1...> ,REQUIRE<ENUM_NOT<IS_VOID<RETURN>>>> {
	using PARAMS = TYPEAS<UNIT1...> ;
	using Function = typename FUNCTION_HELP<RETURN ,TYPEAS<UNIT1...> ,ALWAYS>::Function ;

	class Wrapper {
	protected:
		Function mThis ;

	public:
		implicit Wrapper () = delete ;

		explicit Wrapper (RREF<Function> that) {
			mThis = move (that) ;
		}

		inline Auto operator() (CREF<DEPENDENT<Generic ,UNIT1>>...params) const {
			return template_invoke (PHX ,TYPEAS<PARAMS>::expr ,params...) ;
		}

		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_GT_ZERO<COUNT_OF<ARG1>>>>
		Auto template_invoke (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id ,CREF<Generic> param1 ,XREF<ARG2>...params) const {
			using R1X = TYPE_FIRST_ONE<ARG1> ;
			using R2X = REMOVE_REF<R1X> ;
			assume (IS_VARIABLE<R1X>::expr == param1.is_variable ()) ;
			assume (IS_REGISTER<R1X>::expr == param1.is_register ()) ;
			const auto r1x = param1.pointer () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<R2X>>::expr] (unsafe_pointer (r1x))) ;
			return template_invoke (PHX ,TYPEAS<TYPE_FIRST_REST<ARG1>>::expr ,params... ,keep[TYPEAS<XREF<R1X>>::expr] (tmp)) ;
		}

		template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_EQ_ZERO<COUNT_OF<ARG1>>>>
		Auto template_invoke (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id ,XREF<ARG2>...params) const {
			return mThis (params...) ;
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
		virtual void initialize (CREF<TEMP<void>> value_) = 0 ;
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

template <class UNIT1>
trait AUTOREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::AutoRef ;

	template <class ARG1>
	using CRTP_AutoRef = typename DEPENDENT<AUTOREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::AutoRef ;

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
			:AutoRef (forward[TYPEAS<AutoRef>::expr] (that.as_cast (TYPEAS<UNIT1>::expr))) {}

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
		VREF<CRTP_AutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRTP_AutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CREF<CRTP_AutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRTP_AutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CRTP_AutoRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			CRTP_AutoRef<ARG1> ret ;
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

template <class UNIT1>
trait AUTOREF_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::AutoRef ;

	template <class ARG1>
	using CRTP_AutoRef = typename DEPENDENT<AUTOREF_HELP<ARG1 ,ALWAYS> ,DEPEND>::AutoRef ;

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
			:AutoRef (forward[TYPEAS<AutoRef>::expr] (that.as_cast (TYPEAS<UNIT1>::expr))) {}

		template <class...ARG1>
		imports AutoRef make (XREF<ARG1>...obj) {
			using R1X = typename AUTOREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			AutoRef ret ;
			ret.mThis = VRef<R1X>::make () ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
			ret.mThis->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self))) ;
			rax.release () ;
			ret.mPointer = ret.mThis->pointer () ;
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
			if (mThis->clazz () != Clazz (TYPEAS<UNIT1>::expr))
				return FALSE ;
			return TRUE ;
		}

		Clazz clazz () const {
			if ifnot (exist ())
				return Clazz () ;
			return mThis->clazz () ;
		}

		VREF<UNIT1> self_m () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return (&self) ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		template <class ARG1>
		VREF<CRTP_AutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRTP_AutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CREF<CRTP_AutoRef<ARG1>> rebind (CREF<TYPEID<ARG1>> id) const leftvalue {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRTP_AutoRef<ARG1>>>::expr] (unsafe_deptr (thiz))) ;
			assert (tmp.available ()) ;
			return tmp ;
		}

		template <class ARG1>
		CRTP_AutoRef<ARG1> as_cast (CREF<TYPEID<ARG1>> id) {
			CRTP_AutoRef<ARG1> ret ;
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

template <class UNIT1>
trait AUTOREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename AUTOREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<TEMP<void>> value_) override {
			mValue.acquire (value_) ;
		}

		FLAG pointer () const override {
			return address (mValue.self) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPEAS<UNIT1>::expr) ;
		}
	} ;
} ;

template <class UNIT1 = void>
using AutoRef = typename AUTOREF_HELP<UNIT1 ,ALWAYS>::AutoRef ;

template <class...>
trait SHAREDREF_HELP ;

template <class...>
trait SHAREDREF_HOLDER_HELP ;

template <class...>
trait SHAREDREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<TEMP<void>> value_) = 0 ;
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

template <class UNIT1>
trait SHAREDREF_HELP<UNIT1 ,ALWAYS> {
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
			using R1X = typename SHAREDREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			SharedRef ret ;
			auto rax = VRef<R1X>::make ().as_cast (TYPEAS<Holder>::expr) ;
			auto rbx = Box<UNIT1>::make (forward[TYPEAS<ARG1>::expr] (obj)...) ;
			rax->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rbx.self))) ;
			rbx.release () ;
			ret.mThis = rax.as_cref () ;
			ret.mPointer = ret.mThis->pointer () ;
			ret.mHandle = Scope<Holder> (ret.mThis.self) ;
			return move (ret) ;
		}

		implicit SharedRef (CREF<SharedRef> that) {
			mThis = that.mThis ;
			if ifnot (that.exist ())
				return ;
			assume (available ()) ;
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

		VREF<UNIT1> self_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

		SharedRef weak () const {
			SharedRef ret ;
			ret.mThis = mThis ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait SHAREDREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename SHAREDREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		LENGTH mCounter ;
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<TEMP<void>> value_) override {
			mCounter = 0 ;
			mValue.acquire (value_) ;
		}

		FLAG pointer () const override {
			return address (mValue.self) ;
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

template <class UNIT1>
using SharedRef = typename SHAREDREF_HELP<UNIT1 ,ALWAYS>::SharedRef ;

template <class...>
trait UNIQUEREF_HELP ;

template <class...>
trait UNIQUEREF_HOLDER_HELP ;

template <class...>
trait UNIQUEREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<TEMP<void>> destructor) = 0 ;
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

template <class UNIT1>
trait UNIQUEREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
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
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			mThis = VRef<R2X>::make () ;
			auto rax = Box<R1X>::make (move (destructor)) ;
			assert (ifnot (rax->effective ())) ;
			mThis->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self))) ;
			rax.release () ;
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

template <class UNIT1>
trait UNIQUEREF_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
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
			using R1X = Function<void ,TYPEAS<VREF<UNIT1>>> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			mThis = VRef<R2X>::make () ;
			auto rax = Box<R1X>::make (move (destructor)) ;
			assert (ifnot (rax->effective ())) ;
			mThis->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self))) ;
			rax.release () ;
			mPointer = mThis->pointer () ;
			constructor (fake) ;
			mHandle = Scope<Holder> (mThis.self) ;
		}

		template <class...ARG1>
		imports UniqueRef make (XREF<ARG1>...obj) {
			using R1X = Function<void ,TYPEAS<VREF<UNIT1>>> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			UniqueRef ret ;
			auto rax = R1X ([] (VREF<UNIT1>) {}) ;
			assert (ifnot (rax.effective ())) ;
			ret.mThis = VRef<R2X>::make (move (rax)) ;
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

		BOOL available () const {
			if (mThis == NULL)
				return FALSE ;
			return TRUE ;
		}

		CREF<UNIT1> self_m () const leftvalue {
			return fake ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return (&self) ;
		}

	private:
		VREF<UNIT1> fake_m () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::expr] (unsafe_pointer (mPointer))) ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Function<void> mDestructor ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<TEMP<void>> destructor) override {
			auto rax = Box<Function<void>> () ;
			rax.acquire (destructor) ;
			mDestructor = move (rax.self) ;
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

template <class UNIT1>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Function<void ,TYPEAS<VREF<UNIT1>>> mDestructor ;
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<TEMP<void>> destructor) override {
			auto rax = Box<Function<void ,TYPEAS<VREF<UNIT1>>>> () ;
			rax.acquire (destructor) ;
			mDestructor = move (rax.self) ;
			mValue = Box<UNIT1>::make () ;
		}

		FLAG pointer () const override {
			return address (mValue.self) ;
		}

		void enter () override {
			assert (mDestructor.exist ()) ;
		}

		void leave () override {
			mDestructor (mValue) ;
		}
	} ;
} ;

template <class UNIT1 = void>
using UniqueRef = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::UniqueRef ;

template <class...>
trait BUFFER_HELP ;

template <class...>
trait BOXBUFFER_HOLDER_HELP ;

template <class ITEM ,class SIZE ,class CLONEABLE>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,CLONEABLE ,REQUIRE<ENUM_NOT<CLONEABLE>>> {
	class Buffer {
	protected:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE ,class CLONEABLE>
trait BOXBUFFER_HOLDER_HELP<ITEM ,SIZE ,CLONEABLE ,REQUIRE<CLONEABLE>> {
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

	template <class ARG1>
	using CRTP_ARR = DEPENDENT<ARR<ITEM ,SIZE> ,ARG1> ;

	class Buffer extend SUPER {
	protected:
		using SUPER::mBuffer ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = DEPEND>
		implicit Buffer (CREF<CRTP_ARR<ARG1>> that) {
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

template <class ITEM ,class CLONEABLE>
trait VARBUFFER_HOLDER_HELP<ITEM ,CLONEABLE ,REQUIRE<ENUM_NOT<CLONEABLE>>> {
	class Buffer {
	protected:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class CLONEABLE>
trait VARBUFFER_HOLDER_HELP<ITEM ,CLONEABLE ,REQUIRE<CLONEABLE>> {
	using SUPER = typename VARBUFFER_HOLDER_HELP<ITEM ,ENUM_FALSE ,ALWAYS>::Buffer ;

	class Buffer extend SUPER {
	protected:
		using SUPER::mBuffer ;
		using SUPER::mSize ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mBuffer == NULL)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
			BufferProc::buf_copy (mBuffer.self ,that.mBuffer.self ,0 ,that.mSize) ;
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
	using SUPER = typename VARBUFFER_HOLDER_HELP<ITEM ,IS_CLONEABLE<ITEM> ,ALWAYS>::Buffer ;

	template <class ARG1>
	using CRTP_ConBuffer = typename DEPENDENT<BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS> ,ARG1>::Buffer ;

	class Buffer extend SUPER {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	protected:
		using SUPER::mBuffer ;
		using SUPER::mSize ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = DEPEND>
		implicit Buffer (RREF<CRTP_ConBuffer<ARG1>> that)
			:Buffer (that.as_vref ()) {}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
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
				if (size_ == 0)
					discard ;
				auto rax = VRef<ARR<ITEM>>::make (size_) ;
				const auto r1x = vmin (size_ ,mSize) ;
				for (auto &&i : iter (0 ,r1x))
					rax.self[i] = move (mBuffer.self[i]) ;
				mBuffer = move (rax) ;
				mSize = size_ ;
			}
			if ifswitch (rxx) {
				mBuffer = NULL ;
				mSize = 0 ;
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

		template <class ARG1 = DEPEND>
		CRTP_ConBuffer<ARG1> as_cref () {
			CRTP_ConBuffer<ARG1> ret ;
			ret.mBuffer = mBuffer.as_cref () ;
			ret.mSize = mSize ;
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

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,CONSTANT>>> {
	using SUPER = typename CONBUFFER_HOLDER_HELP<ITEM ,ALWAYS>::Buffer ;

	template <class ARG1>
	using CRTP_VarBuffer = typename DEPENDENT<BUFFER_HELP<ITEM ,VARIABLE ,ALWAYS> ,ARG1>::Buffer ;

	class Buffer extend SUPER {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	protected:
		using SUPER::mBuffer ;
		using SUPER::mSize ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = DEPEND>
		implicit Buffer (RREF<CRTP_VarBuffer<ARG1>> that)
			:Buffer (that.as_cref ()) {}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = CRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
		}

		implicit Buffer (CREF<Buffer> that) {
			mBuffer = that.mBuffer ;
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

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
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

		template <class ARG1 = DEPEND>
		CRTP_VarBuffer<ARG1> as_vref () {
			CRTP_VarBuffer<ARG1> ret ;
			ret.mBuffer = mBuffer.as_vref () ;
			ret.mSize = mSize ;
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using ConBuffer = typename BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,REGISTER>>> {
	template <class ARG1 ,class = REQUIRE<IS_TEXT<DEPENDENT<ITEM ,ARG1>>>>
	using CRTP_Byte = BYTE_BASE<DEPENDENT<ITEM ,ARG1>> ;

	template <class ARG1 ,class = REQUIRE<IS_BIT<DEPENDENT<ITEM ,ARG1>>>>
	using CRTP_Text = TEXT_BASE<DEPENDENT<ITEM ,ARG1>> ;

	template <class ARG1>
	using CRTP_RegBuffer = typename DEPENDENT<BUFFER_HELP<ARG1 ,REGISTER ,ALWAYS> ,ARG1>::Buffer ;

	class Buffer extend Proxy {
	protected:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		imports VREF<Buffer> from (VREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (CREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (RREF<VarBuffer<ITEM>>) = delete ;

		template <class ARG1 = DEPEND>
		imports VREF<Buffer> from (VREF<VarBuffer<CRTP_Byte<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (CREF<VarBuffer<CRTP_Byte<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (RREF<VarBuffer<CRTP_Byte<ARG1>>>) = delete ;

		template <class ARG1 = DEPEND>
		imports VREF<Buffer> from (VREF<VarBuffer<CRTP_Text<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (CREF<VarBuffer<CRTP_Text<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (RREF<VarBuffer<CRTP_Text<ARG1>>>) = delete ;

		imports CREF<Buffer> from (CREF<ConBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (RREF<ConBuffer<ITEM>>) = delete ;

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (CREF<ConBuffer<CRTP_Byte<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (RREF<ConBuffer<CRTP_Byte<ARG1>>>) = delete ;

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (CREF<ConBuffer<CRTP_Text<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (RREF<ConBuffer<CRTP_Text<ARG1>>>) = delete ;

		template <class ARG1 = DEPEND>
		imports VREF<Buffer> from (VREF<CRTP_RegBuffer<CRTP_Byte<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (CREF<CRTP_RegBuffer<CRTP_Byte<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (RREF<CRTP_RegBuffer<CRTP_Byte<ARG1>>>) = delete ;

		template <class ARG1 = DEPEND>
		imports VREF<Buffer> from (VREF<CRTP_RegBuffer<CRTP_Text<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (CREF<CRTP_RegBuffer<CRTP_Text<ARG1>>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::expr] (unsafe_deptr (that))) ;
		}

		template <class ARG1 = DEPEND>
		imports CREF<Buffer> from (RREF<CRTP_RegBuffer<CRTP_Text<ARG1>>>) = delete ;

		imports VREF<Buffer> from (VREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,
			RREF<VarBuffer<ITEM>> unnamed = VarBuffer<ITEM> ()) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<ITEM>>>::expr] (buffer)) ;
			return from (tmp ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports VREF<Buffer> from (VREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,
			RREF<VarBuffer<ITEM>> unnamed = VarBuffer<ITEM> ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			if ifswitch (TRUE) {
				const auto r1x = vmax (end_ - begin_ ,ZERO) ;
				if (r1x == ZERO)
					discard ;
				unnamed.mBuffer = VRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
				unnamed.mSize = r1x ;
			}
			return from (unnamed) ;
		}

		imports CREF<Buffer> from (CREF<TEMP<void>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,
			RREF<ConBuffer<ITEM>> unnamed = ConBuffer<ITEM> ()) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<ITEM>>>::expr] (buffer)) ;
			return from (tmp ,begin_ ,end_ ,move (unnamed)) ;
		}

		imports CREF<Buffer> from (CREF<ARR<ITEM>> buffer ,CREF<INDEX> begin_ ,CREF<INDEX> end_ ,
			RREF<ConBuffer<ITEM>> unnamed = ConBuffer<ITEM> ()) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			if ifswitch (TRUE) {
				const auto r1x = vmax (end_ - begin_ ,ZERO) ;
				if (r1x == ZERO)
					discard ;
				unnamed.mBuffer = CRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
				unnamed.mSize = r1x ;
			}
			return from (unnamed) ;
		}

		VRef<Buffer> lift () {
			auto rax = VRef<VarBuffer<ITEM>>::make () ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				rax->mBuffer = VRef<ARR<ITEM>>::reference (mBuffer.self) ;
				rax->mSize = mSize ;
			}
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VRef<Buffer>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}

		CRef<Buffer> lift () const {
			auto rax = VRef<ConBuffer<ITEM>>::make () ;
			if ifswitch (TRUE) {
				if (mBuffer == NULL)
					discard ;
				rax->mBuffer = CRef<ARR<ITEM>>::reference (mBuffer.self) ;
				rax->mSize = mSize ;
			}
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRef<Buffer>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
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

template <class ITEM>
using RegBuffer = typename BUFFER_HELP<ITEM ,REGISTER ,ALWAYS>::Buffer ;

template <class...>
trait DYNBUFFER_IMPLHOLDER_HELP ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,DYNAMICS>>> {
	struct Holder implement Interface {
		virtual void initialize (CREF<TEMP<void>> value_) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual VREF<ITEM> at (CREF<INDEX> index) leftvalue = 0 ;
		virtual CREF<ITEM> at (CREF<INDEX> index) const leftvalue = 0 ;
	} ;

	class Buffer {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Buffer () = default ;

		explicit Buffer (CREF<LENGTH> size_) {
			assume (FALSE) ;
		}

		template <class ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_SAME<ARG1 ,Buffer>> ,IS_CLASS<ARG1>>>>
		explicit Buffer (RREF<ARG1> that) {
			using R1X = typename DYNBUFFER_IMPLHOLDER_HELP<ITEM ,ARG1 ,ALWAYS>::ImplHolder ;
			mThis = VRef<R1X>::make () ;
			auto rax = Box<ARG1>::make (move (that)) ;
			mThis->initialize (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self))) ;
			rax.release () ;
		}

		LENGTH size () const {
			if (mThis == NULL)
				return ZERO ;
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

template <class ITEM ,class UNIT1>
trait DYNBUFFER_IMPLHOLDER_HELP<ITEM ,UNIT1 ,ALWAYS> {
	using Holder = typename BUFFER_HELP<ITEM ,DYNAMICS ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<TEMP<void>> value_) override {
			mValue.acquire (value_) ;
		}

		LENGTH size () const override {
			return LENGTH (mValue->size ()) ;
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

template <class ITEM ,class SIZE ,class CLONEABLE>
trait ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,CLONEABLE ,REQUIRE<ENUM_NOT<CLONEABLE>>> {
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

template <class ITEM ,class SIZE ,class CLONEABLE>
trait ALLOCATOR_HOLDER_HELP<ITEM ,SIZE ,CLONEABLE ,REQUIRE<CLONEABLE>> {
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
			if (mAllocator.size () == 0)
				return ZERO ;
			return mAllocator.size () ;
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

template <class UNIT1>
trait LATER_HELP<UNIT1 ,ALWAYS> {
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

		explicit Later (CREF<FLAG> tag ,RREF<Function<UNIT1>> expr_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (tag ,expr_.as_generic ()) ;
		}

		BOOL exist () const {
			return mThis->exist () ;
		}

		UNIT1 invoke () const {
			return mThis->invoke ().poll (TYPEAS<UNIT1>::expr) ;
		}

		inline UNIT1 operator() () const {
			return invoke () ;
		}
	} ;
} ;

template <class UNIT1>
using Later = typename LATER_HELP<UNIT1 ,ALWAYS>::Later ;

template <class...>
trait INTEGER_HELP ;

template <class...>
trait INTEGER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait INTEGER_HELP<DEPEND ,ALWAYS> {
	template <class ARG1>
	using CRTP_Integer = typename DEPENDENT<INTEGER_HELP<DEPEND ,ALWAYS> ,ARG1>::Integer ;

	struct Holder implement Interface {
		virtual void initialize (CREF<VAL64> value_ ,CREF<LENGTH> size_) = 0 ;
		virtual void initialize (CREF<TEMP<void>> integer_) = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual FLAG compr (CREF<Holder> that) const = 0 ;
		virtual FLAG hash () const = 0 ;
		virtual CRTP_Integer<DEPEND> add (CREF<Holder> that) const = 0 ;
		virtual CRTP_Integer<DEPEND> sub (CREF<Holder> that) const = 0 ;
		virtual CRTP_Integer<DEPEND> mul (CREF<LENGTH> scale) const = 0 ;
		virtual CRTP_Integer<DEPEND> div (CREF<LENGTH> scale) const = 0 ;
		virtual CRTP_Integer<DEPEND> mod (CREF<LENGTH> scale) const = 0 ;
		virtual CRTP_Integer<DEPEND> clone () const = 0 ;
		virtual CRTP_Integer<DEPEND> minus () const = 0 ;
		virtual void increase () = 0 ;
		virtual void decrease () = 0 ;
	} ;

	struct FakeHolder implement Holder {
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

		explicit Integer (CREF<TEMP<void>> integer_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (integer_) ;
		}

		BOOL equal (CREF<Integer> that) const {
			return mThis->equal (that) ;
		}

		inline BOOL operator== (CREF<Integer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Integer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Integer> that) const {
			return mThis->compr (that) ;
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

		Integer clone () const {
			return mThis->clone () ;
		}

		inline Integer operator+ () const {
			return clone () ;
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
