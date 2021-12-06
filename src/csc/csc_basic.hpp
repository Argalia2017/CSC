#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
namespace BASIC {
template <class...>
trait OPTIONAL_HELP ;

template <class...>
trait OPTIONAL_SUPER_HELP ;

template <class UNIT1>
trait OPTIONAL_SUPER_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mCode = ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait OPTIONAL_SUPER_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	ENUM_NOT<IS_VOID<UNIT1>> ,
	IS_CLONEABLE<UNIT1>>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		FLAG mCode ;
		Box<UNIT1> mSome ;

	public:
		implicit Optional () noexcept {
			mCode = ZERO ;
		}

		implicit Optional (CREF<Optional> that) :Optional () {
			mSome = Box<UNIT1>::make (that.mSome.self) ;
		}

		inline VREF<Optional> operator= (CREF<Optional> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Optional (RREF<Optional> that) noexcept :Optional () {
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
trait OPTIONAL_SUPER_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
	ENUM_NOT<IS_VOID<UNIT1>> ,
	ENUM_NOT<IS_CLONEABLE<UNIT1>>>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		FLAG mCode ;
		Box<UNIT1> mSome ;

	public:
		implicit Optional () noexcept {
			mCode = ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait OPTIONAL_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using SUPER = typename OPTIONAL_SUPER_HELP<UNIT1 ,ALWAYS>::Optional ;

	class Optional extend SUPER {
	private:
		using SUPER::mCode ;

	public:
		implicit Optional () = default ;

		imports CREF<Optional> none () {
			return memorize ([&] () {
				return Optional () ;
			}) ;
		}

		imports Optional make_code (CREF<FLAG> code_) {
			Optional ret ;
			ret.mCode = code_ ;
			return move (ret) ;
		}

		BOOL exist () const {
			return FALSE ;
		}

		FLAG code () const {
			if (exist ())
				return ZERO ;
			return mCode ;
		}

		Optional maybe (RREF<Optional> that) rightvalue {
			return move (that) ;
		}

		BOOL equal (CREF<Optional> that) const {
			return TRUE ;
		}

		inline BOOL operator== (CREF<Optional> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Optional> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Optional> that) const {
			return ZERO ;
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
			return hashcode () ;
		}
	} ;
} ;

template <class UNIT1>
trait OPTIONAL_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
	using SUPER = typename OPTIONAL_SUPER_HELP<UNIT1 ,ALWAYS>::Optional ;

	class Optional extend SUPER {
	private:
		using SUPER::mCode ;
		using SUPER::mSome ;

	public:
		implicit Optional () = default ;

		imports CREF<Optional> none () {
			return memorize ([&] () {
				return Optional () ;
			}) ;
		}

		imports Optional make_code (CREF<FLAG> code_) {
			Optional ret ;
			ret.mCode = code_ ;
			return move (ret) ;
		}

		template <class...ARG1>
		imports Optional make_some (XREF<ARG1>...objs) {
			Optional ret ;
			ret.mSome = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mSome != NULL ;
		}

		FLAG code () const {
			if (exist ())
				return ZERO ;
			return mCode ;
		}

		UNIT1 poll () leftvalue {
			dynamic_assert (exist ()) ;
			UNIT1 ret = move (mSome.self) ;
			mSome = NULL ;
			return move (ret) ;
		}

		UNIT1 poll (CREF<UNIT1> def) leftvalue {
			return poll (move (def)) ;
		}

		UNIT1 poll (RREF<UNIT1> def) leftvalue {
			UNIT1 ret = move (def) ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				ret = move (mSome.self) ;
				mSome = NULL ;
			}
			return move (ret) ;
		}

		Optional maybe (RREF<Optional> that) rightvalue {
			if ifnot (exist ())
				return move (that) ;
			return move (thiz) ;
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

template <class UNIT1 = void>
using Optional = typename OPTIONAL_HELP<UNIT1 ,ALWAYS>::Optional ;

template <class...>
trait INTEGER_HELP ;

template <class...>
trait INTEGER_SUPER_HELP ;

template <class SIZE>
trait INTEGER_SUPER_HELP<SIZE ,ALWAYS> {
	class Integer {
	private:
		template <class...>
		friend trait INTEGER_HELP ;

	private:
		Box<ARR<BYTE ,SIZE>> mInteger ;

	public:
		implicit Integer () noexcept {
			mInteger = Box<ARR<BYTE ,SIZE>>::make () ;
		}
	} ;
} ;

template <class SIZE>
trait INTEGER_HELP<SIZE ,ALWAYS> {
	using SUPER = typename INTEGER_SUPER_HELP<SIZE ,ALWAYS>::Integer ;

	using HIGHEST = ENUM_DEC<SIZE> ;

	class Integer extend SUPER {
	private:
		using SUPER::mInteger ;

	public:
		implicit Integer () = default ;

		implicit Integer (CREF<VAL32> that) {
			const auto r1x = vmin (SIZE::value ,SIZE_OF<VAL32>::value) ;
			const auto r2x = CHAR (that) ;
			for (auto &&i : iter (0 ,r1x))
				mInteger.self[i] = BYTE ((r2x >> (i * 8)) & CHAR (0XFF)) ;
			const auto r3x = invoke ([&] () {
				if (that >= 0)
					return BYTE (0X00) ;
				return BYTE (0XFF) ;
			}) ;
			for (auto &&i : iter (r1x ,SIZE::value))
				mInteger.self[i] = r3x ;
		}

		implicit Integer (CREF<VAL64> that) {
			const auto r1x = vmin (SIZE::value ,SIZE_OF<VAL64>::value) ;
			const auto r2x = DATA (that) ;
			for (auto &&i : iter (0 ,r1x))
				mInteger.self[i] = BYTE ((r2x >> (i * 8)) & DATA (0XFF)) ;
			const auto r3x = invoke ([&] () {
				if (that >= 0)
					return BYTE (0X00) ;
				return BYTE (0XFF) ;
			}) ;
			for (auto &&i : iter (r1x ,SIZE::value))
				mInteger.self[i] = r3x ;
		}

		BOOL equal (CREF<Integer> that) const {
			for (auto &&i : iter (0 ,SIZE::value)) {
				if ifnot (operator_equal (mInteger.self[i] ,that.mInteger.self[i]))
					return FALSE ;
			}
			return TRUE ;
		}

		inline BOOL operator== (CREF<Integer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Integer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Integer> that) const {
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = operator_compr (mInteger.self[i] ,that.mInteger.self[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
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
			FLAG ret = hashcode () ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = operator_hash (mInteger.self[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		Integer add (CREF<Integer> that) const {
			Integer ret ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = VAL64 (mInteger.self[i]) + VAL64 (that.mInteger.self[i]) + rax ;
				const auto r2x = DATA (r1x) ;
				ret.mInteger.self[i] = BYTE (r2x & DATA (0XFF)) ;
				rax = VAL64 (r2x >> 8) ;
			}
			return move (ret) ;
		}

		inline Integer operator+ (CREF<Integer> that) const {
			return add (that) ;
		}

		inline void operator+= (CREF<Integer> that) {
			thiz = add (that) ;
		}

		Integer sub (CREF<Integer> that) const {
			Integer ret ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = VAL64 (mInteger.self[i]) - VAL64 (that.mInteger.self[i]) - rax ;
				const auto r2x = VAL64 (r1x < 0) ;
				const auto r3x = r1x + VAL64 (256) * r2x ;
				const auto r4x = DATA (r3x) ;
				ret.mInteger.self[i] = BYTE (r4x & DATA (0XFF)) ;
				rax = r2x ;
			}
			return move (ret) ;
		}

		inline Integer operator- (CREF<LENGTH> that) const {
			return sub (that) ;
		}

		inline void operator-= (CREF<Integer> that) {
			thiz = sub (that) ;
		}

		Integer mul (CREF<LENGTH> scale) const {
			assert (scale >= VAL32_MIN) ;
			assert (scale <= VAL32_MAX) ;
			Integer ret ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = VAL64 (mInteger.self[i]) * VAL64 (scale) + rax ;
				const auto r2x = DATA (r1x) ;
				ret.mInteger.self[i] = BYTE (r2x & DATA (0XFF)) ;
				rax = VAL64 (r2x >> 8) ;
			}
			return move (ret) ;
		}

		inline Integer operator* (CREF<LENGTH> scale) const {
			return mul (scale) ;
		}

		inline void operator*= (CREF<LENGTH> scale) {
			thiz = mul (scale) ;
		}

		Integer div (CREF<LENGTH> scale) const {
			assert (scale >= VAL32_MIN) ;
			assert (scale <= VAL32_MAX) ;
			assert (scale != ZERO) ;
			Integer ret ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = VAL64 (mInteger.self[i]) + rax ;
				const auto r2x = r1x / VAL64 (scale) ;
				const auto r3x = DATA (r2x) ;
				ret.mInteger.self[i] = BYTE (r3x & DATA (0XFF)) ;
				const auto r4x = r1x - r2x * VAL64 (scale) ;
				rax = r4x * VAL64 (256) ;
			}
			return move (ret) ;
		}

		inline Integer operator/ (CREF<LENGTH> scale) const {
			return div (scale) ;
		}

		inline void operator/= (CREF<LENGTH> scale) {
			thiz = div (scale) ;
		}

		Integer mod (CREF<Integer> scale) const {
			return thiz - thiz / scale * scale ;
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
			Integer ret ;
			for (auto &&i : iter (0 ,SIZE::value))
				ret.mInteger.self[i] = ~mInteger.self[i] ;
			ret++ ;
			return move (ret) ;
		}

		inline Integer operator- () const {
			return minus () ;
		}

		void increase () {
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= SIZE::value)
					break ;
				mInteger.self[ix]++ ;
				if (mInteger.self[ix] != BYTE (0X00))
					break ;
				ix++ ;
			}
		}

		inline void operator++ (int) {
			increase () ;
		}

		void decrease () {
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= SIZE::value)
					break ;
				mInteger.self[ix]-- ;
				if (mInteger.self[ix] != BYTE (0XFF))
					break ;
				ix++ ;
			}
		}

		inline void operator-- (int) {
			decrease () ;
		}
	} ;
} ;

template <class SIZE>
using Integer = typename INTEGER_HELP<SIZE ,ALWAYS>::Integer ;

template <class...>
trait TUPLE_HELP ;

template <class UNIT1>
trait TUPLE_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	class Tuple {
	public:
		implicit Tuple () = default ;

		void initialize () {
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

template <class UNIT1>
trait TUPLE_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<UNIT1>>>> {
	using FIRST_ONE = TYPE_FIRST_ONE<UNIT1> ;
	using FIRST_REST = TYPE_FIRST_REST<UNIT1> ;
	using LAST_ONE = TYPE_LAST_ONE<UNIT1> ;
	using LAST_REST = TYPE_LAST_REST<UNIT1> ;
	using FIRST_REST_TUPLE = typename TUPLE_HELP<FIRST_REST ,ALWAYS>::Tuple ;
	using LAST_REST_TUPLE = typename TUPLE_HELP<LAST_REST ,ALWAYS>::Tuple ;
	require (IS_DEFAULT<FIRST_ONE>) ;
	require (IS_DEFAULT<LAST_ONE>) ;

	struct FOREWARD {
		FIRST_ONE mOne ;
		FIRST_REST_TUPLE mRest ;
	} ;

	struct BACKWARD {
		LAST_REST_TUPLE mRest ;
		LAST_ONE mOne ;
	} ;

	class Tuple {
	private:
		FOREWARD mTuple ;

	public:
		implicit Tuple () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Tuple>>>>
		explicit Tuple (XREF<ARG1> one_ ,XREF<ARG2>...rest_) {
			initialize (forward[TYPEAS<ARG1>::id] (one_) ,forward[TYPEAS<ARG2>::id] (rest_)...) ;
		}

		template <class...ARG1>
		void initialize (CREF<FIRST_ONE> one_ ,XREF<ARG1>...rest_) {
			mTuple.mOne = move (one_) ;
			mTuple.mRest.initialize (forward[TYPEAS<ARG1>::id] (rest_)...) ;
		}

		template <class...ARG1>
		void initialize (RREF<FIRST_ONE> one_ ,XREF<ARG1>...rest_) {
			mTuple.mOne = move (one_) ;
			mTuple.mRest.initialize (forward[TYPEAS<ARG1>::id] (rest_)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<UNIT1>::value ;
		}

		VREF<FIRST_ONE> first_one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> first_one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<FIRST_REST_TUPLE> first_rest () leftvalue {
			return mTuple.mRest ;
		}

		CREF<FIRST_REST_TUPLE> first_rest () const leftvalue {
			return mTuple.mRest ;
		}

		VREF<LAST_ONE> last_one () leftvalue {
			unimplemented () ;
			return bad (TYPEAS<LAST_ONE>::id) ;
		}

		CREF<LAST_ONE> last_one () const leftvalue {
			unimplemented () ;
			return bad (TYPEAS<LAST_ONE>::id) ;
		}

		VREF<LAST_REST_TUPLE> last_rest () leftvalue {
			unimplemented () ;
			return bad (TYPEAS<LAST_REST_TUPLE>::id) ;
		}

		CREF<LAST_REST_TUPLE> last_rest () const leftvalue {
			unimplemented () ;
			return bad (TYPEAS<LAST_REST_TUPLE>::id) ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<UNIT1 ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<UNIT1>>) ;
			return template_pick (PHX ,TYPEAS<R1X>::id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<UNIT1 ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) const leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<UNIT1>>) ;
			return template_pick (PHX ,TYPEAS<R1X>::id) ;
		}

		BOOL equal (CREF<Tuple> that) const {
			if ifnot (operator_equal (first_one () ,that.first_one ()))
				return FALSE ;
			if ifnot (operator_equal (first_rest () ,that.first_rest ()))
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
			const auto r1x = operator_compr (first_one () ,that.first_one ()) ;
			if (r1x != ZERO)
				return r1x ;
			const auto r2x = operator_compr (first_rest () ,that.first_rest ()) ;
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
			const auto r1x = operator_hash (first_one ()) ;
			const auto r2x = operator_hash (first_rest ()) ;
			return hashcode (r1x ,r2x) ;
		}

	private:
		template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_ALL<ARG1>>>>
		VREF<TYPE_PICK<UNIT1 ,REMOVE_ALL<ARG1>>> template_pick (CREF<typeof (PH2)> ,XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			return first_rest ().pick (TYPEAS<ENUM_DEC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_ALL<ARG1>>>>
		VREF<FIRST_ONE> template_pick (CREF<typeof (PH1)> ,XREF<ARG1> id) leftvalue {
			return first_one () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_ALL<ARG1>>>>
		CREF<TYPE_PICK<UNIT1 ,REMOVE_ALL<ARG1>>> template_pick (CREF<typeof (PH2)> ,XREF<ARG1> id) const leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			return first_rest ().pick (TYPEAS<ENUM_DEC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_ALL<ARG1>>>>
		CREF<FIRST_ONE> template_pick (CREF<typeof (PH1)> ,XREF<ARG1> id) const leftvalue {
			return first_one () ;
		}
	} ;
} ;

template <class...PARAMS>
using Tuple = typename TUPLE_HELP<TYPEAS<PARAMS...> ,ALWAYS>::Tuple ;

template <class...>
trait IS_EFFECTIVE_HELP ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait IS_EFFECTIVE_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,ALWAYS> {
	using R1X = UNIT1 ;
	using R2X = DEF<RETURN (*) (PARAMS...)> ;
	using R3X = IS_CONVERTIBLE<CREF<R1X> ,RREF<R2X>> ;
	using RET = ENUM_NOT<R3X> ;
} ;

template <class UNIT1>
using IS_EFFECTIVE = typename IS_EFFECTIVE_HELP<FUNCTION_RETURN<UNIT1> ,FUNCTION_PARAMS<UNIT1> ,UNIT1 ,ALWAYS>::RET ;

template <class...>
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_IMPLHOLDER_HELP ;

template <class...>
trait FUNCTION_WRAPHOLDER_HELP ;

template <class RETURN ,class...PARAMS>
trait FUNCTION_HELP<RETURN ,TYPEAS<PARAMS...> ,ALWAYS> {
	struct Holder implement Interface {
		virtual BOOL effective () const = 0 ;
		virtual BOOL exception () const = 0 ;
		virtual RETURN invoke (XREF<PARAMS>...params) const = 0 ;
	} ;

	class Function {
	private:
		CRef<Holder> mThis ;

	public:
		implicit Function () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Function>>>>
		explicit Function (XREF<ARG1> that) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_SAME<RETURN ,FUNCTION_RETURN<R1X>>) ;
			using R2X = typename FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<PARAMS...> ,R1X ,ALWAYS>::ImplHolder ;
			auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (that)) ;
			mThis = CRef<R2X>::make (move (rax)) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		LENGTH rank () const {
			return COUNT_OF<TYPEAS<PARAMS...>>::value ;
		}

		BOOL effective () const {
			if (mThis == NULL)
				return FALSE ;
			return mThis->effective () ;
		}

		BOOL exception () const {
			if (mThis == NULL)
				return FALSE ;
			return mThis->exception () ;
		}

		RETURN invoke (XREF<PARAMS>...params) const {
			return mThis->invoke (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}

		inline RETURN operator() (XREF<PARAMS>...params) const {
			return invoke (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,ALWAYS> {
	using Holder = typename FUNCTION_HELP<RETURN ,TYPEAS<PARAMS...> ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mFunctor ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Box<UNIT1>> functor) {
			mFunctor = move (functor) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<UNIT1>::value ;
		}

		BOOL exception () const override {
			unimplemented () ;
			return TRUE ;
		}

		RETURN invoke (XREF<PARAMS>...params) const override {
			return mFunctor.self (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class PARAMS = TYPEAS<>>
using Function = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Function ;

template <class...>
trait FINALLY_HELP ;

template <>
trait FINALLY_HELP<ALWAYS> {
	class Finally extend Proxy {
	private:
		Function<void> mBase ;

	public:
		imports CREF<Finally> from (CREF<Function<void>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Finally>>::id ,unsafe_deptr (that))) ;
		}

		void enter () const {
			assert (mBase.exist ()) ;
		}

		void leave () const {
			mBase.invoke () ;
		}
	} ;
} ;

using Finally = typename FINALLY_HELP<ALWAYS>::Finally ;

template <class...>
trait ANYREF_HELP ;

template <class...>
trait ANYREF_HOLDER_HELP ;

template <class...>
trait ANYREF_SUPER_HELP ;

template <class...>
trait ANYREF_IMPLHOLDER_HELP ;

template <>
trait ANYREF_HOLDER_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG addr () const = 0 ;
		virtual Clazz clazz () const = 0 ;
	} ;
} ;

template <class UNIT1>
trait ANYREF_SUPER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::Holder ;

	class AnyRef {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		VRef<Holder> mThis ;
		FLAG mPointer ;

	public:
		implicit AnyRef () noexcept {
			mPointer = ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::Holder ;
	using SUPER = typename ANYREF_SUPER_HELP<UNIT1 ,ALWAYS>::AnyRef ;

	template <class ARG1 ,class ARG2>
	using MACRO_AnyRef = typename DEPENDENT<ANYREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS> ,ARG2>::AnyRef ;

	class AnyRef extend SUPER {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mPointer ;

	public:
		implicit AnyRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,AnyRef>> ,HAS_REBIND<REMOVE_ALL<ARG1> ,UNIT1 ,AnyRef>>>>
		implicit AnyRef (XREF<ARG1> that) : AnyRef (move (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		Clazz clazz () const {
			if (mThis == NULL)
				return Clazz () ;
			return mThis->clazz () ;
		}

		template <class ARG1>
		MACRO_AnyRef<REMOVE_ALL<ARG1> ,ARG1> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			MACRO_AnyRef<R1X ,ARG1> ret ;
			ret.mThis = move (mThis) ;
			ret.mPointer = mPointer ;
			mPointer = ZERO ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::Holder ;
	using SUPER = typename ANYREF_SUPER_HELP<UNIT1 ,ALWAYS>::AnyRef ;

	template <class ARG1 ,class ARG2>
	using MACRO_AnyRef = typename DEPENDENT<ANYREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS> ,ARG2>::AnyRef ;

	class AnyRef extend SUPER {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mPointer ;

	public:
		implicit AnyRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,AnyRef>> ,HAS_REBIND<REMOVE_ALL<ARG1> ,UNIT1 ,AnyRef>>>>
		implicit AnyRef (XREF<ARG1> that) : AnyRef (move (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports AnyRef make (XREF<ARG1>...objs) {
			using R1X = typename ANYREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			AnyRef ret ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			ret.mThis = VRef<R1X>::make (move (rax)) ;
			ret.mPointer = ret.mThis->addr () ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		Clazz clazz () const {
			if (mThis == NULL)
				return Clazz () ;
			return mThis->clazz () ;
		}

		VREF<UNIT1> m_self () leftvalue {
			assert (clazz () == Clazz (TYPEAS<UNIT1>::id)) ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			assert (clazz () == Clazz (TYPEAS<UNIT1>::id)) ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		MACRO_AnyRef<REMOVE_ALL<ARG1> ,ARG1> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			MACRO_AnyRef<R1X ,ARG1> ret ;
			ret.mThis = move (mThis) ;
			ret.mPointer = mPointer ;
			mPointer = ZERO ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait ANYREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename ANYREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Box<UNIT1>> value_) {
			mValue = move (value_) ;
		}

		FLAG addr () const override {
			return address (mValue.self) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPEAS<UNIT1>::id) ;
		}
	} ;
} ;

template <class UNIT1 = void>
using AnyRef = typename ANYREF_HELP<UNIT1 ,ALWAYS>::AnyRef ;

template <class...>
trait SHAREDREF_HELP ;

template <class...>
trait SHAREDREF_HOLDER_HELP ;

template <class...>
trait SHAREDREF_SUPER_HELP ;

template <class...>
trait SHAREDREF_IMPLHOLDER_HELP ;

template <>
trait SHAREDREF_HOLDER_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG addr () const = 0 ;
		virtual void enter () const = 0 ;
		virtual void leave () const = 0 ;
	} ;
} ;

template <class UNIT1>
trait SHAREDREF_SUPER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename SHAREDREF_HOLDER_HELP<ALWAYS>::Holder ;

	struct SUPER {
		CRef<Holder> mThis ;
		FLAG mPointer ;
	} ;

	class SharedRef extend SUPER {
	private:
		template <class...>
		friend trait SHAREDREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mPointer ;
		Scope<CREF<Holder>> mHandle ;

	public:
		implicit SharedRef () noexcept {
			mPointer = ZERO ;
		}

		implicit SharedRef (CREF<SharedRef> that) :SharedRef () {
			if (that.mThis == NULL)
				return ;
			mHandle = Scope<CREF<Holder>> (that.mThis.self) ;
			mThis = that.mThis ;
			mPointer = that.mPointer ;
		}

		inline VREF<SharedRef> operator= (CREF<SharedRef> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit SharedRef (RREF<SharedRef> that) noexcept :SharedRef () {
			swap (thiz ,that) ;
		}

		inline VREF<SharedRef> operator= (RREF<SharedRef> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class UNIT1>
trait SHAREDREF_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename SHAREDREF_HOLDER_HELP<ALWAYS>::Holder ;
	using SUPER = typename SHAREDREF_SUPER_HELP<UNIT1 ,ALWAYS>::SharedRef ;

	template <class ARG1 ,class ARG2>
	using MACRO_SharedRef = typename DEPENDENT<SHAREDREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS> ,ARG2>::SharedRef ;

	class SharedRef extend SUPER {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit SharedRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,SharedRef>> ,HAS_REBIND<REMOVE_ALL<ARG1> ,UNIT1 ,SharedRef>>>>
		implicit SharedRef (XREF<ARG1> that) : SharedRef (move (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports SharedRef make (XREF<ARG1>...objs) {
			using R1X = typename SHAREDREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			SharedRef ret ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			ret.mThis = CRef<R1X>::make (move (rax)) ;
			ret.mPointer = ret.mThis->addr () ;
			ret.mHandle = Scope<CREF<Holder>> (ret.mThis.self) ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		VREF<UNIT1> m_self () const leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		MACRO_SharedRef<REMOVE_ALL<ARG1> ,ARG1> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			MACRO_SharedRef<R1X ,ARG1> ret ;
			ret.mThis = move (mThis) ;
			ret.mHandle = move (mHandle) ;
			ret.mPointer = template_recast (PHX ,TYPEAS<R1X>::id) ;
			mPointer = ZERO ;
			return move (ret) ;
		}

	private:
		template <class ARG1 ,class = ENABLE<IS_EXTEND<REMOVE_ALL<ARG1> ,UNIT1>>>
		FLAG template_recast (CREF<typeof (PH3)> ,XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			if (mPointer == ZERO)
				return ZERO ;
			return address (keep[TYPEAS<VREF<R1X>>::id] (self)) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_ALL<IS_EXTEND<UNIT1 ,Interface> ,IS_EXTEND<REMOVE_ALL<ARG1> ,Interface>>>>
		FLAG template_recast (CREF<typeof (PH2)> ,XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			if (mPointer == ZERO)
				return ZERO ;
			return address (dynamic_cast<VREF<R1X>> (self)) ;
		}

		template <class ARG1>
		FLAG template_recast (CREF<typeof (PH1)> ,XREF<ARG1> id) {
			return ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait SHAREDREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename SHAREDREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mValue ;
		LENGTH mCounter ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Box<UNIT1>> value_) {
			mValue = move (value_) ;
			mCounter = 0 ;
		}

		FLAG addr () const override {
			return address (mValue.self) ;
		}

		void enter () const {
			fake.mCounter++ ;
		}

		void leave () const {
			fake.mCounter-- ;
			if (fake.mCounter > 0)
				return ;
			fake.mValue = NULL ;
		}

	private:
		VREF<ImplHolder> m_fake () const leftvalue {
			const auto r1x = address (thiz) ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<ImplHolder>>::id ,unsafe_pointer (r1x))) ;
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
trait UNIQUEREF_SUPER_HELP ;

template <class...>
trait UNIQUEREF_IMPLHOLDER_HELP ;

template <>
trait UNIQUEREF_HOLDER_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG addr () const = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_SUPER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<ALWAYS>::Holder ;

	struct SUPER {
		VRef<Holder> mThis ;
		FLAG mPointer ;
	} ;

	class UniqueRef extend SUPER {
	private:
		template <class...>
		friend trait UNIQUEREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mPointer ;
		Scope<VREF<Holder>> mHandle ;

	public:
		implicit UniqueRef () noexcept {
			mPointer = ZERO ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<ALWAYS>::Holder ;
	using SUPER = typename UNIQUEREF_SUPER_HELP<UNIT1 ,ALWAYS>::UniqueRef ;

	class UniqueRef extend SUPER {
	private:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (XREF<ARG1> constructor ,XREF<ARG2> destructor) {
			using R1X = Function<void> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			auto rax = R1X (forward[TYPEAS<ARG2>::id] (destructor)) ;
			mThis = VRef<R2X>::make (move (rax)) ;
			mPointer = mThis->addr () ;
			constructor () ;
			mHandle = Scope<VREF<Holder>> (mThis.self) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<ALWAYS>::Holder ;
	using SUPER = typename UNIQUEREF_SUPER_HELP<UNIT1 ,ALWAYS>::UniqueRef ;

	class UniqueRef extend SUPER {
	private:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (XREF<ARG1> constructor ,XREF<ARG2> destructor) {
			using R1X = Function<void ,TYPEAS<VREF<UNIT1>>> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			auto rax = R1X (forward[TYPEAS<ARG2>::id] (destructor)) ;
			mThis = VRef<R2X>::make (move (rax)) ;
			mPointer = mThis->addr () ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (mPointer))) ;
			constructor (tmp) ;
			mHandle = Scope<VREF<Holder>> (mThis.self) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<UNIT1>>::id ,unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		Function<void> mDestructor ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Function<void>> destructor) {
			assert (ifnot (destructor.effective ())) ;
			mDestructor = move (destructor) ;
		}

		FLAG addr () const override {
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
	private:
		UNIT1 mValue ;
		Function<void ,TYPEAS<VREF<UNIT1>>> mDestructor ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Function<void ,TYPEAS<VREF<UNIT1>>>> destructor) {
			assert (ifnot (destructor.effective ())) ;
			mDestructor = move (destructor) ;
		}

		FLAG addr () const override {
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

template <class UNIT1 = void>
using UniqueRef = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::UniqueRef ;

template <class...>
trait BUFFER_HELP ;

template <class...>
trait BOXBUFFER_SUPER_HELP ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_SUPER_HELP<ITEM ,SIZE ,REQUIRE<IS_CLONEABLE<ITEM>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) :Buffer () {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (that.mBuffer.self) ;
		}

		inline VREF<Buffer> operator= (CREF<Buffer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept :Buffer () {
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
trait BOXBUFFER_SUPER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_NOT<IS_CLONEABLE<ITEM>>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

	public:
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	using SUPER = typename BOXBUFFER_SUPER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

	class Buffer extend SUPER {
	private:
		using SUPER::mBuffer ;

	public:
		implicit Buffer () = default ;

		implicit Buffer (CREF<ARR<ITEM ,SIZE>> that) {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (forward[TYPEAS<CREF<ARR<ITEM ,SIZE>>>::id] (that)) ;
		}

		implicit Buffer (RREF<ARR<ITEM ,SIZE>> that) {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (forward[TYPEAS<RREF<ARR<ITEM ,SIZE>>>::id] (that)) ;
		}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			assert (size_ <= SIZE::value) ;
			mBuffer = Box<ARR<ITEM ,SIZE>>::make () ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return SIZE::value ;
		}

		VREF<ARR<ITEM>> m_self () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM>>>::id ,unsafe_deptr (mBuffer.self)) ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<ARR<ITEM>>>::id ,tmp)) ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
			auto &&tmp = unsafe_cast (TYPEAS<ARR<TEMP<ITEM>>>::id ,unsafe_deptr (mBuffer.self)) ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<ARR<ITEM>>>::id ,tmp)) ;
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
			for (auto &&i : iter (0 ,size ())) {
				if ifnot (operator_equal (mBuffer.self[i] ,that.mBuffer.self[i]))
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
				const auto r2x = operator_compr (mBuffer.self[i] ,that.mBuffer.self[i]) ;
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
				const auto r1x = operator_hash (mBuffer.self[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
using Buffer = typename BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
using BoxBuffer = typename BUFFER_HELP<ITEM ,ENABLE<ENUM_GT_ZERO<SIZE> ,SIZE> ,ALWAYS>::Buffer ;

template <class...>
trait VARBUFFER_SUPER_HELP ;

template <class ITEM>
trait VARBUFFER_SUPER_HELP<ITEM ,REQUIRE<IS_CLONEABLE<ITEM>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}

		implicit Buffer (CREF<Buffer> that) :Buffer () {
			if (that.mSize == 0)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
			for (auto &&i : iter (0 ,that.mSize))
				mBuffer.self[i] = that.mBuffer.self[i] ;
			mSize = that.mSize ;
		}

		inline VREF<Buffer> operator= (CREF<Buffer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept :Buffer () {
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

template <class ITEM>
trait VARBUFFER_SUPER_HELP<ITEM ,REQUIRE<ENUM_NOT<IS_CLONEABLE<ITEM>>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,VARIABLE>>> {
	using SUPER = typename VARBUFFER_SUPER_HELP<ITEM ,ALWAYS>::Buffer ;

	template <class ARG1>
	using MACRO_ConBuffer = typename DEPENDENT<BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS> ,ARG1>::Buffer ;

	class Buffer extend SUPER {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		using SUPER::mBuffer ;
		using SUPER::mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}

		template <class ARG1 = void>
		implicit Buffer (RREF<MACRO_ConBuffer<ARG1>> that) {
			mBuffer = that.mBuffer.lock () ;
			dynamic_assert (that.mBuffer == NULL) ;
			mSize = that.mSize ;
		}

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

		VREF<ARR<ITEM>> m_self () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
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
			assert (size_ > 0) ;
			auto rax = VRef<ARR<ITEM>>::make (size_) ;
			const auto r1x = vmin (size_ ,mSize) ;
			for (auto &&i : iter (0 ,r1x))
				rax.self[i] = move (mBuffer.self[i]) ;
			mBuffer = move (rax) ;
			mSize = size_ ;
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			for (auto &&i : iter (0 ,size ())) {
				if ifnot (operator_equal (mBuffer.self[i] ,that.mBuffer.self[i]))
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
				const auto r2x = operator_compr (mBuffer.self[i] ,that.mBuffer.self[i]) ;
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
				const auto r1x = operator_hash (mBuffer.self[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using VarBuffer = typename BUFFER_HELP<ITEM ,VARIABLE ,ALWAYS>::Buffer ;

template <class...>
trait CONBUFFER_SUPER_HELP ;

template <class ITEM>
trait CONBUFFER_SUPER_HELP<ITEM ,REQUIRE<IS_CLONEABLE<ITEM>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		CRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}

		implicit Buffer (CREF<Buffer> that) :Buffer () {
			if (that.mSize == 0)
				return ;
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
			for (auto &&i : iter (0 ,that.mSize))
				mBuffer.self[i] = that.mBuffer.self[i] ;
			mSize = that.mSize ;
		}

		inline VREF<Buffer> operator= (CREF<Buffer> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept :Buffer () {
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

template <class ITEM>
trait CONBUFFER_SUPER_HELP<ITEM ,REQUIRE<ENUM_NOT<IS_CLONEABLE<ITEM>>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
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
	using SUPER = typename CONBUFFER_SUPER_HELP<ITEM ,ALWAYS>::Buffer ;

	template <class ARG1>
	using MACRO_VarBuffer = typename DEPENDENT<BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS> ,ARG1>::Buffer ;

	class Buffer extend SUPER {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		using SUPER::mBuffer ;
		using SUPER::mSize ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = void>
		implicit Buffer (RREF<MACRO_VarBuffer<ARG1>> that) {
			mBuffer = that.mBuffer.unlock () ;
			dynamic_assert (that.mBuffer == NULL) ;
			mSize = that.mSize ;
		}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			if (size_ == 0)
				return ;
			mBuffer = CRef<ARR<ITEM>>::make (size_) ;
			mSize = size_ ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
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
			dynamic_assert (FALSE) ;
		}

		BOOL equal (CREF<Buffer> that) const {
			if (size () != that.size ())
				return FALSE ;
			for (auto &&i : iter (0 ,size ())) {
				if ifnot (operator_equal (mBuffer.self[i] ,that.mBuffer.self[i]))
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
				const auto r2x = operator_compr (mBuffer.self[i] ,that.mBuffer.self[i]) ;
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
				const auto r1x = operator_hash (mBuffer.self[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using ConBuffer = typename BUFFER_HELP<ITEM ,CONSTANT ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,REGISTER>>> {
	class Buffer extend Proxy {
	private:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		imports VREF<Buffer> from (VREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (CREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (CREF<ConBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		imports VREF<Buffer> from (VREF<VarBuffer<ITEM>> that ,RREF<VRef<ARR<ITEM>>> buffer ,CREF<LENGTH> size_) {
			that.mBuffer = move (buffer) ;
			that.mSize = size_ ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (VREF<ConBuffer<ITEM>> that ,RREF<CRef<ARR<ITEM>>> buffer ,CREF<LENGTH> size_) {
			that.mBuffer = move (buffer) ;
			that.mSize = size_ ;
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		VREF<ARR<ITEM>> m_self () leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			if (mBuffer == NULL)
				return unsafe_array (TYPEAS<ITEM>::id) ;
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
			for (auto &&i : iter (0 ,size ())) {
				if ifnot (operator_equal (mBuffer.self[i] ,that.mBuffer.self[i]))
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
				const auto r2x = operator_compr (mBuffer.self[i] ,that.mBuffer.self[i]) ;
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
				const auto r1x = operator_hash (mBuffer.self[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using RegBuffer = typename BUFFER_HELP<ITEM ,REGISTER ,ALWAYS>::Buffer ;

template <class...>
trait ALLOCATOR_HELP ;

template <class...>
trait ALLOCATOR_SUPER_HELP ;

template <class ITEM ,class SIZE>
trait ALLOCATOR_SUPER_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	struct NODE {
		ITEM mItem ;
		INDEX mNext ;
	} ;

	class Allocator {
	private:
		template <class...>
		friend trait ALLOCATOR_HELP ;

	private:
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

template <class ITEM ,class SIZE>
trait ALLOCATOR_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	using NODE = typename ALLOCATOR_SUPER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename ALLOCATOR_SUPER_HELP<ITEM ,SIZE ,ALWAYS>::Allocator ;

	using ALLOCATOR_POW = RANK2 ;
	using ALLOCATOR_MIN_SIZE = ENUMAS<VAL ,ENUMID<256>> ;

	class Allocator extend SUPER {
	private:
		using SUPER::mAllocator ;
		using SUPER::mSize ;
		using SUPER::mLength ;
		using SUPER::mFree ;

	public:
		implicit Allocator () = default ;

		explicit Allocator (CREF<LENGTH> size_) {
			mAllocator = Buffer<NODE ,SIZE> (size_) ;
			mSize = mAllocator.size () ;
			mLength = 0 ;
			update_reserve (NONE ,0 ,mSize) ;
		}

		void clear () {
			for (auto &&i : iter (0 ,mAllocator.size ()))
				mAllocator[i].mItem = ITEM () ;
			mSize = mAllocator.size () ;
			mLength = 0 ;
			update_reserve (NONE ,0 ,mSize) ;
		}

		LENGTH size () const {
			if (mAllocator.size () == 0)
				return ZERO ;
			return mSize ;
		}

		LENGTH length () const {
			if (mAllocator.size () == 0)
				return ZERO ;
			return mLength ;
		}

		BOOL used (CREF<INDEX> index) const {
			assert (vbetween (index ,0 ,mSize)) ;
			return mAllocator[index].mNext == USED ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mAllocator[index].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,size ())) ;
			return mAllocator[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX alloc () leftvalue {
			update_resize () ;
			INDEX ret = mFree ;
			mFree = mAllocator[ret].mNext ;
			mAllocator[ret].mNext = USED ;
			mLength++ ;
			return move (ret) ;
		}

		void free (CREF<INDEX> index) {
			assert (used (index)) ;
			mAllocator[index].mItem = ITEM () ;
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
			mSize = mAllocator.size () ;
			update_reserve (ix ,r1x ,mSize) ;
		}

	private:
		void update_reserve (CREF<INDEX> free_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			INDEX ix = free_ ;
			for (auto &&i : iter (begin_ ,end_)) {
				INDEX iy = end_ - 1 - i ;
				mAllocator[iy].mNext = ix ;
				ix = iy ;
			}
			mFree = ix ;
		}

		void update_resize () {
			if (mAllocator.size () > 0)
				if (mFree != NONE)
					return ;
			const auto r1x = vmax (mSize * ALLOCATOR_POW::value ,ALLOCATOR_MIN_SIZE::value) ;
			resize (r1x) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
using Allocator = typename ALLOCATOR_HELP<ITEM ,SIZE ,ALWAYS>::Allocator ;

template <class...>
trait LATER_HELP ;

template <class...>
trait LATER_BINDER_HELP ;

template <class UNIT1>
trait LATER_HELP<UNIT1 ,ALWAYS> {
	using LATER_HEAP_SIZE = ENUMAS<VAL ,ENUMID<256>> ;

	template <class ARG1>
	using MACRO_NODE = typename DEPENDENT<LATER_BINDER_HELP<UNIT1 ,ALWAYS> ,ARG1>::NODE ;

	template <class ARG1>
	using MACRO_HEAP = typename DEPENDENT<LATER_BINDER_HELP<UNIT1 ,ALWAYS> ,ARG1>::HEAP ;

	struct WRAP {
		VRef<MACRO_HEAP<void>> mHeap ;
		INDEX mIndex ;
	} ;

	using NODE = MACRO_NODE<void> ;
	using HEAP = MACRO_HEAP<void> ;

	class Later {
	private:
		SharedRef<HEAP> mHeap ;
		CRef<UniqueRef<WRAP>> mThis ;

	public:
		implicit Later () = default ;

		explicit Later (CREF<FLAG> tag) {
			link_heap () ;
			INDEX ix = mHeap->mLast ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				if (mHeap->mList[ix].mTag == tag)
					break ;
				ix = mHeap->mList[ix].mPrev ;
			}
			if (ix == NONE)
				return ;
			assert (mHeap->mList[ix].mPrev != USED) ;
			mThis = CRef<UniqueRef<WRAP>>::intrusive (mHeap->mList[ix].mNode) ;
		}

		explicit Later (CREF<FLAG> tag ,RREF<Function<UNIT1>> expr) {
			assert (expr.exist ()) ;
			link_heap () ;
			auto rax = UniqueRef<WRAP> ([&] (VREF<WRAP> me) {
				me.mHeap = VRef<HEAP>::reference (mHeap.self) ;
				INDEX ix = me.mHeap->mFree ;
				dynamic_assert (ix != NONE) ;
				me.mHeap->mList[ix].mTag = tag ;
				me.mHeap->mList[ix].mExpr = move (expr) ;
				zeroize (me.mHeap->mList[ix].mNode) ;
				me.mHeap->mFree = me.mHeap->mList[ix].mNext ;
				me.mHeap->mList[ix].mPrev = me.mHeap->mLast ;
				me.mHeap->mList[ix].mNext = NONE ;
				curr_next (me.mHeap ,me.mHeap->mLast ,ix) ;
				me.mHeap->mLast = ix ;
				me.mIndex = ix ;
			} ,[] (VREF<WRAP> me) {
				INDEX ix = me.mIndex ;
				if (me.mHeap->mList[ix].mPrev == USED)
					return ;
				curr_next (me.mHeap ,me.mHeap->mList[ix].mPrev ,me.mHeap->mList[ix].mNext) ;
				curr_prev (me.mHeap ,me.mHeap->mList[ix].mNext ,me.mHeap->mList[ix].mPrev) ;
				me.mHeap->mList[ix].mPrev = USED ;
				me.mHeap->mList[ix].mNext = me.mHeap->mFree ;
				me.mHeap->mFree = ix ;
				me.mHeap->mList[ix].mTag = 0 ;
				me.mHeap->mList[ix].mExpr = Function<UNIT1> () ;
			}) ;
			INDEX ix = rax->mIndex ;
			mThis = CRef<UniqueRef<WRAP>>::intrusive (mHeap->mList[ix].mNode ,move (rax)) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		UNIT1 invoke () const {
			return mHeap->mList[mThis->self.mIndex].mExpr () ;
		}

		inline UNIT1 operator() () const {
			return invoke () ;
		}

	private:
		void link_heap () {
			mHeap = memorize ([&] () {
				SharedRef<HEAP> ret = SharedRef<HEAP>::make () ;
				ret->mList = BoxBuffer<NODE ,LATER_HEAP_SIZE> (0) ;
				update_reserve (ret.self) ;
				return move (ret) ;
			}) ;
		}

		imports void update_reserve (VREF<HEAP> heap) {
			INDEX ix = NONE ;
			for (auto &&i : iter (0 ,heap.mList.size ())) {
				INDEX iy = heap.mList.size () - 1 - i ;
				heap.mList[iy].mPrev = USED ;
				heap.mList[iy].mNext = ix ;
				ix = iy ;
			}
			heap.mFirst = NONE ;
			heap.mLast = NONE ;
			heap.mFree = ix ;
		}

		imports void curr_next (VREF<HEAP> heap ,CREF<INDEX> curr ,CREF<INDEX> next) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (curr != NONE)
					discard ;
				heap.mFirst = next ;
			}
			if ifswitch (eax) {
				heap.mList[curr].mNext = next ;
			}
		}

		imports void curr_prev (VREF<HEAP> heap ,CREF<INDEX> curr ,CREF<INDEX> prev) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (curr != NONE)
					discard ;
				heap.mLast = prev ;
			}
			if ifswitch (eax) {
				heap.mList[curr].mPrev = prev ;
			}
		}
	} ;
} ;

template <class UNIT1>
trait LATER_BINDER_HELP<UNIT1 ,ALWAYS> {
	using WRAP = typename LATER_HELP<UNIT1 ,ALWAYS>::WRAP ;
	using LATER_HEAP_SIZE = typename LATER_HELP<UNIT1 ,ALWAYS>::LATER_HEAP_SIZE ;

	using OWNERSHIP = typename CREF_PUREHOLDER_HELP<UniqueRef<WRAP> ,ALWAYS>::PureHolder ;

	struct NODE {
		FLAG mTag ;
		Function<UNIT1> mExpr ;
		TEMP<OWNERSHIP> mNode ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	struct HEAP {
		BoxBuffer<NODE ,LATER_HEAP_SIZE> mList ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mFree ;
	} ;
} ;

template <class UNIT1>
using Later = typename LATER_HELP<UNIT1 ,ALWAYS>::Later ;
} ;
} ;

namespace CSC {
using namespace BASIC ;
} ;
