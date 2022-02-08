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
trait OPTIONAL_SUPER_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		Box<UNIT1> mSome ;
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mCode = NONE ;
		}

		implicit Optional (CREF<Optional> that) :Optional () {
			mSome = Box<UNIT1>::make (that.mSome.self) ;
			mCode = that.mCode ;
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
trait OPTIONAL_SUPER_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_CLONEABLE<UNIT1>>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		Box<UNIT1> mSome ;
		FLAG mCode ;

	public:
		implicit Optional () noexcept {
			mCode = NONE ;
		}
	} ;
} ;

template <class UNIT1>
trait OPTIONAL_HELP<UNIT1 ,ALWAYS> {
	using SUPER = typename OPTIONAL_SUPER_HELP<UNIT1 ,ALWAYS>::Optional ;

	class Optional extend SUPER {
	private:
		using SUPER::mSome ;
		using SUPER::mCode ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<FLAG> that) {
			assert (that >= 0) ;
			mCode = that ;
		}

		imports CREF<Optional> none () {
			return memorize ([&] () {
				return Optional () ;
			}) ;
		}

		template <class...ARG1>
		imports Optional make (XREF<ARG1>...objs) {
			Optional ret ;
			ret.mSome = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			ret.mCode = USED ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mSome != NULL ;
		}

		FLAG code () const {
			return mCode ;
		}

		UNIT1 poll () {
			assume (exist ()) ;
			UNIT1 ret = move (mSome.self) ;
			mSome = NULL ;
			return move (ret) ;
		}

		UNIT1 poll (CREF<UNIT1> def) {
			return poll (move (def)) ;
		}

		UNIT1 poll (RREF<UNIT1> def) {
			UNIT1 ret = move (def) ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				ret = move (mSome.self) ;
				mSome = NULL ;
			}
			return move (ret) ;
		}

		template <class ARG1>
		Optional and_then (XREF<ARG1> func) rightvalue {
			if ifnot (exist ())
				return none () ;
			auto rax = func (move (mSome.self)) ;
			using R1X = typeof (rax) ;
			require (IS_SAME<R1X ,UNIT1>) ;
			mSome = NULL ;
			return make (move (rax)) ;
		}

		template <class ARG1>
		Optional or_else (XREF<ARG1> func) rightvalue {
			if (exist ())
				return move (thiz) ;
			auto rax = func () ;
			using R1X = typeof (rax) ;
			require (IS_SAME<R1X ,UNIT1>) ;
			return make (move (rax)) ;
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
	using LAST_ONE = TYPE_LAST_ONE<PARAMS> ;
	using LAST_REST = TYPE_LAST_REST<PARAMS> ;
	using LAST_REST_TUPLE = typename TUPLE_HELP<LAST_REST ,ALWAYS>::Tuple ;
	require (IS_DEFAULT<FIRST_ONE>) ;
	require (IS_DEFAULT<LAST_ONE>) ;

	struct FOWARD {
		FIRST_ONE mOne ;
	} ;

	struct BACKWARD {
		LAST_ONE mOne ;
	} ;

	class Tuple {
	private:
		FOWARD mTuple ;

	public:
		implicit Tuple () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Tuple>>>>
		explicit Tuple (XREF<ARG1> one_) {
			assign (forward[TYPEAS<ARG1>::id] (one_)) ;
		}

		void assign (CREF<FIRST_ONE> one_) {
			first_one () = move (one_) ;
		}

		void assign (RREF<FIRST_ONE> one_) {
			first_one () = move (one_) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::value ;
		}

		VREF<FIRST_ONE> first_one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<FIRST_ONE> first_one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<FIRST_REST_TUPLE> first_rest () leftvalue {
			return FIRST_REST_TUPLE::zero () ;
		}

		CREF<FIRST_REST_TUPLE> first_rest () const leftvalue {
			return FIRST_REST_TUPLE::zero () ;
		}

		VREF<LAST_ONE> last_one () leftvalue {
			return mTuple.mOne ;
		}

		CREF<LAST_ONE> last_one () const leftvalue {
			return mTuple.mOne ;
		}

		VREF<LAST_REST_TUPLE> last_rest () leftvalue {
			return LAST_REST_TUPLE::zero () ;
		}

		CREF<LAST_REST_TUPLE> last_rest () const leftvalue {
			return LAST_REST_TUPLE::zero () ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			return template_pick (PHX ,TYPEAS<R1X>::id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) const leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
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
		VREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> template_pick (CREF<typeof (PH2)> ,XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			return first_rest ().pick (TYPEAS<ENUM_DEC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_ALL<ARG1>>>>
		VREF<FIRST_ONE> template_pick (CREF<typeof (PH1)> ,XREF<ARG1> id) leftvalue {
			return first_one () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_ALL<ARG1>>>>
		CREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> template_pick (CREF<typeof (PH2)> ,XREF<ARG1> id) const leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			return first_rest ().pick (TYPEAS<ENUM_DEC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_ALL<ARG1>>>>
		CREF<FIRST_ONE> template_pick (CREF<typeof (PH1)> ,XREF<ARG1> id) const leftvalue {
			return first_one () ;
		}
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_GT_IDEN<COUNT_OF<PARAMS>>>> {
	using FIRST_ONE = TYPE_FIRST_ONE<PARAMS> ;
	using FIRST_REST = TYPE_FIRST_REST<PARAMS> ;
	using FIRST_REST_TUPLE = typename TUPLE_HELP<FIRST_REST ,ALWAYS>::Tuple ;
	using LAST_ONE = TYPE_LAST_ONE<PARAMS> ;
	using LAST_REST = TYPE_LAST_REST<PARAMS> ;
	using LAST_REST_TUPLE = typename TUPLE_HELP<LAST_REST ,ALWAYS>::Tuple ;
	require (IS_DEFAULT<FIRST_ONE>) ;
	require (IS_DEFAULT<LAST_ONE>) ;

	struct FOWARD {
		FIRST_ONE mOne ;
		FIRST_REST_TUPLE mRest ;
	} ;

	struct BACKWARD {
		LAST_REST_TUPLE mRest ;
		LAST_ONE mOne ;
	} ;

	class Tuple {
	private:
		FOWARD mTuple ;

	public:
		implicit Tuple () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Tuple>>>>
		explicit Tuple (XREF<ARG1> one_ ,XREF<ARG2>...rest_) {
			assign (forward[TYPEAS<ARG1>::id] (one_) ,forward[TYPEAS<ARG2>::id] (rest_)...) ;
		}

		template <class...ARG1>
		void assign (CREF<FIRST_ONE> one_ ,XREF<ARG1>...rest_) {
			first_one () = move (one_) ;
			first_rest ().assign (forward[TYPEAS<ARG1>::id] (rest_)...) ;
		}

		template <class...ARG1>
		void assign (RREF<FIRST_ONE> one_ ,XREF<ARG1>...rest_) {
			first_one () = move (one_) ;
			first_rest ().assign (forward[TYPEAS<ARG1>::id] (rest_)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::value ;
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
			require (ENUM_EQUAL<ALIGN_OF<LAST_REST_TUPLE> ,ALIGN_OF<LAST_ONE>>) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BACKWARD>>::id] (unsafe_deptr (mTuple))) ;
			return tmp.mOne ;
		}

		CREF<LAST_ONE> last_one () const leftvalue {
			require (ENUM_EQUAL<ALIGN_OF<LAST_REST_TUPLE> ,ALIGN_OF<LAST_ONE>>) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BACKWARD>>::id] (unsafe_deptr (mTuple))) ;
			return tmp.mOne ;
		}

		VREF<LAST_REST_TUPLE> last_rest () leftvalue {
			require (ENUM_EQUAL<ALIGN_OF<LAST_REST_TUPLE> ,ALIGN_OF<LAST_ONE>>) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BACKWARD>>::id] (unsafe_deptr (mTuple))) ;
			return tmp.mRest ;
		}

		CREF<LAST_REST_TUPLE> last_rest () const leftvalue {
			require (ENUM_EQUAL<ALIGN_OF<LAST_REST_TUPLE> ,ALIGN_OF<LAST_ONE>>) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BACKWARD>>::id] (unsafe_deptr (mTuple))) ;
			return tmp.mRest ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
			return template_pick (PHX ,TYPEAS<R1X>::id) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) const leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<PARAMS>>) ;
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
		VREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> template_pick (CREF<typeof (PH2)> ,XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			return first_rest ().pick (TYPEAS<ENUM_DEC<R1X>>::id) ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_ALL<ARG1>>>>
		VREF<FIRST_ONE> template_pick (CREF<typeof (PH1)> ,XREF<ARG1> id) leftvalue {
			return first_one () ;
		}

		template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_ALL<ARG1>>>>
		CREF<TYPE_PICK<PARAMS ,REMOVE_ALL<ARG1>>> template_pick (CREF<typeof (PH2)> ,XREF<ARG1> id) const leftvalue {
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
trait TUPLEBINDER_HELP ;

template <class...>
trait TUPLEBINDER_BASE_HELP ;

template <class UNIT1>
trait TUPLEBINDER_BASE_HELP<UNIT1 ,REQUIRE<IS_VARIABLE<UNIT1>>> {
	using RET = VRef<REMOVE_REF<UNIT1>> ;
} ;

template <class UNIT1>
trait TUPLEBINDER_BASE_HELP<UNIT1 ,REQUIRE<IS_CONSTANT<UNIT1>>> {
	using RET = CRef<REMOVE_REF<UNIT1>> ;
} ;

template <class UNIT1>
using TUPLEBINDER_BASE = typename TUPLEBINDER_BASE_HELP<XREF<UNIT1> ,ALWAYS>::RET ;

template <class UNIT1>
trait TUPLEBINDER_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<UNIT1>>>> {
	using BASE = Tuple<> ;

	class TupleBinder extend Proxy {
	private:
		BASE mBase ;

	public:
		imports VRef<TupleBinder> from (VREF<BASE> that) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<TupleBinder>>::id] (unsafe_deptr (that))) ;
			return VRef<TupleBinder>::reference (tmp) ;
		}

		imports VRef<TupleBinder> from () {
			auto rax = BASE () ;
			rax.assign () ;
			auto rbx = VRef<BASE>::make (move (rax)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VRef<TupleBinder>>>::id] (unsafe_deptr (rbx))) ;
			return move (tmp) ;
		}

		LENGTH rank () const {
			return ZERO ;
		}
	} ;
} ;

template <class...UNIT1>
trait TUPLEBINDER_HELP<TYPEAS<UNIT1...> ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<TYPEAS<UNIT1...>>>>> {
	using PARAMS = TYPEAS<UNIT1...> ;

	using BASE = Tuple<TUPLEBINDER_BASE<UNIT1>...> ;
	using FIRST_ONE = TYPE_FIRST_ONE<PARAMS> ;
	using FIRST_REST = TYPE_FIRST_REST<PARAMS> ;
	using FIRST_REST_TUPLE = typename TUPLEBINDER_HELP<FIRST_REST ,ALWAYS>::TupleBinder ;
	using LAST_ONE = TYPE_LAST_ONE<PARAMS> ;
	using LAST_REST = TYPE_LAST_REST<PARAMS> ;
	using LAST_REST_TUPLE = typename TUPLEBINDER_HELP<LAST_REST ,ALWAYS>::TupleBinder ;

	class TupleBinder extend Proxy {
	private:
		BASE mBase ;

	public:
		imports VRef<TupleBinder> from (VREF<BASE> that) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<TupleBinder>>::id] (unsafe_deptr (that))) ;
			return VRef<TupleBinder>::reference (tmp) ;
		}

		template <class...ARG1>
		imports VRef<TupleBinder> from (XREF<ARG1>...objs) {
			auto rax = BASE () ;
			rax.assign (TUPLEBINDER_BASE<XREF<UNIT1>>::reference (objs)...) ;
			auto rbx = VRef<BASE>::make (move (rax)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VRef<TupleBinder>>>::id] (unsafe_deptr (rbx))) ;
			return move (tmp) ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::value ;
		}

		XREF<FIRST_ONE> first_one () leftvalue {
			return mBase.first_one ().self ;
		}

		VREF<FIRST_REST_TUPLE> first_rest () leftvalue {
			return FIRST_REST_TUPLE::from (mBase.first_rest ()) ;
		}

		XREF<LAST_ONE> last_one () leftvalue {
			return mBase.last_one ().self ;
		}

		VREF<LAST_REST_TUPLE> last_rest () leftvalue {
			return LAST_REST_TUPLE::from (mBase.last_rest ()) ;
		}
	} ;
} ;

template <class...PARAMS>
using TupleBinder = typename TUPLEBINDER_HELP<TYPEAS<XREF<PARAMS>...> ,ALWAYS>::TupleBinder ;

template <class...>
trait IS_EFFECTIVE_HELP ;

template <class RETURN ,class...UNIT1 ,class UNIT2>
trait IS_EFFECTIVE_HELP<RETURN ,TYPEAS<UNIT1...> ,UNIT2 ,ALWAYS> {
	using R1X = UNIT2 ;
	using R2X = DEF<RETURN (*) (UNIT1...)> ;
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

template <class RETURN ,class...UNIT1>
trait FUNCTION_HELP<RETURN ,TYPEAS<UNIT1...> ,ALWAYS> {
	using PARAMS = TYPEAS<UNIT1...> ;

	struct Holder implement Interface {
		virtual BOOL effective () const = 0 ;
		virtual BOOL exception () const = 0 ;
		virtual RETURN invoke (XREF<UNIT1>...params) const = 0 ;
	} ;

	class Function {
	private:
		CRef<Holder> mThis ;

	public:
		implicit Function () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Function>>>>
		explicit Function (XREF<ARG1> that) {
			using R1X = REMOVE_ALL<ARG1> ;
			require (IS_FUNCTION<R1X>) ;
			require (IS_SAME<RETURN ,FUNCTION_RETURN<R1X>>) ;
			using R2X = typename FUNCTION_IMPLHOLDER_HELP<RETURN ,PARAMS ,R1X ,ALWAYS>::ImplHolder ;
			using R3X = typename CREF_PUREHOLDER_HELP<R2X ,ALWAYS>::PureHolder ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (IS_EFFECTIVE<R1X>::value)
					discard ;
				static auto mInstance = CORE::invoke ([&] () {
					TEMP<R3X> ret ;
					zeroize (ret) ;
					return move (ret) ;
				}) ;
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (that)) ;
				mThis = CRef<R2X>::intrusive (mInstance ,move (rax)) ;
			}
			if ifswitch (eax) {
				auto rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (that)) ;
				mThis = CRef<R2X>::make (move (rax)) ;
			}
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		LENGTH rank () const {
			return COUNT_OF<PARAMS>::value ;
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

		RETURN invoke (XREF<UNIT1>...params) const {
			return mThis->invoke (forward[TYPEAS<UNIT1>::id] (params)...) ;
		}

		inline RETURN operator() (XREF<UNIT1>...params) const {
			return invoke (forward[TYPEAS<UNIT1>::id] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class...UNIT1 ,class UNIT2>
trait FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<UNIT1...> ,UNIT2 ,ALWAYS> {
	using PARAMS = TYPEAS<UNIT1...> ;

	using Holder = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT2> mFunctor ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Box<UNIT2>> functor) {
			mFunctor = move (functor) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<UNIT2>::value ;
		}

		BOOL exception () const override {
			unimplemented () ;
			return TRUE ;
		}

		RETURN invoke (XREF<UNIT1>...params) const override {
			return mFunctor.self (forward[TYPEAS<UNIT1>::id] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class PARAMS = TYPEAS<>>
using Function = typename FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Function ;

template <class...>
trait FINALLY_HELP ;

template <class DEPEND>
trait FINALLY_HELP<DEPEND ,ALWAYS> {
	class Finally extend Proxy {
	private:
		Function<void> mBase ;

	public:
		imports CREF<Finally> from (CREF<Function<void>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<Finally>>::id] (unsafe_deptr (that))) ;
		}

		void enter () const {
			assert (mBase.exist ()) ;
		}

		void leave () const {
			mBase.invoke () ;
		}
	} ;
} ;

using Finally = typename FINALLY_HELP<DEPEND ,ALWAYS>::Finally ;

template <class...>
trait AUTOREF_HELP ;

template <class...>
trait AUTOREF_HOLDER_HELP ;

template <class...>
trait AUTOREF_SUPER_HELP ;

template <class...>
trait AUTOREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG addr () const = 0 ;
		virtual Clazz clazz () const = 0 ;
		virtual VRef<Holder> clone () const = 0 ;
	} ;
} ;

template <class UNIT1>
trait AUTOREF_SUPER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class AutoRef {
	private:
		template <class...>
		friend trait AUTOREF_HELP ;

	private:
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
	using SUPER = typename AUTOREF_SUPER_HELP<UNIT1 ,ALWAYS>::AutoRef ;

	template <class ARG1>
	using MACRO_AnyRef = typename DEPENDENT<AUTOREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS> ,ARG1>::AutoRef ;

	class AutoRef extend SUPER {
	private:
		template <class...>
		friend trait AUTOREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mPointer ;

	public:
		implicit AutoRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<IS_REGISTER<ARG1> ,ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,AutoRef>> ,HAS_REBIND<REMOVE_ALL<ARG1> ,UNIT1 ,AutoRef>>>>
		implicit AutoRef (XREF<ARG1> that) : AutoRef (forward[TYPEAS<ARG1>::id] (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		BOOL available () const {
			if ifnot (exist ())
				return FALSE ;
			return mPointer != ZERO ;
		}

		Clazz clazz () const {
			if (mThis == NULL)
				return Clazz () ;
			return mThis->clazz () ;
		}

		AutoRef clone () const {
			AutoRef ret ;
			if ifswitch (TRUE) {
				if (mThis != NULL)
					discard ;
				ret.mThis = mThis->clone () ;
				if (ret.mThis->clazz () != Clazz (TYPEAS<UNIT1>::id))
					discard ;
				ret.mPointer = ret.mThis->addr () ;
			}
			return move (ret) ;
		}

		template <class ARG1>
		MACRO_AnyRef<ARG1> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			MACRO_AnyRef<ARG1> ret ;
			if ifswitch (TRUE) {
				if (mThis != NULL)
					discard ;
				ret.mThis = move (mThis) ;
				mPointer = ZERO ;
				if (ret.mThis->clazz () != Clazz (TYPEAS<R1X>::id))
					discard ;
				ret.mPointer = ret.mThis->addr () ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait AUTOREF_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_VOID<UNIT1>>>> {
	using Holder = typename AUTOREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename AUTOREF_SUPER_HELP<UNIT1 ,ALWAYS>::AutoRef ;

	template <class ARG1>
	using MACRO_AnyRef = typename DEPENDENT<AUTOREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS> ,ARG1>::AutoRef ;

	class AutoRef extend SUPER {
	private:
		template <class...>
		friend trait AUTOREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mPointer ;

	public:
		implicit AutoRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<IS_REGISTER<ARG1> ,ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,AutoRef>> ,HAS_REBIND<REMOVE_ALL<ARG1> ,UNIT1 ,AutoRef>>>>
		implicit AutoRef (XREF<ARG1> that) : AutoRef (forward[TYPEAS<ARG1>::id] (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports AutoRef make (XREF<ARG1>...objs) {
			using R1X = typename AUTOREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			AutoRef ret ;
			auto rax = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			ret.mThis = VRef<R1X>::make (move (rax)) ;
			ret.mPointer = ret.mThis->addr () ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		BOOL available () const {
			if ifnot (exist ())
				return FALSE ;
			return mPointer != ZERO ;
		}

		Clazz clazz () const {
			if (mThis == NULL)
				return Clazz () ;
			return mThis->clazz () ;
		}

		VREF<UNIT1> m_self () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		AutoRef clone () const {
			AutoRef ret ;
			if ifswitch (TRUE) {
				if (mThis != NULL)
					discard ;
				ret.mThis = mThis->clone () ;
				if (ret.mThis->clazz () != Clazz (TYPEAS<UNIT1>::id))
					discard ;
				ret.mPointer = ret.mThis->addr () ;
			}
			return move (ret) ;
		}

		template <class ARG1>
		MACRO_AnyRef<ARG1> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			MACRO_AnyRef<ARG1> ret ;
			if ifswitch (TRUE) {
				if (mThis != NULL)
					discard ;
				ret.mThis = move (mThis) ;
				mPointer = ZERO ;
				if (ret.mThis->clazz () != Clazz (TYPEAS<R1X>::id))
					discard ;
				ret.mPointer = ret.mThis->addr () ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait AUTOREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename AUTOREF_HELP<UNIT1 ,ALWAYS>::Holder ;

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

		VRef<Holder> clone () const override {
			auto rax = template_clone (PHX ,TYPEAS<UNIT1>::id) ;
			assume (rax != NULL) ;
			return VRef<ImplHolder>::make (ImplHolder (move (rax))) ;
		}

	private:
		template <class ARG1 ,class = ENABLE<IS_CLONEABLE<ARG1>>>
		Box<UNIT1> template_clone (CREF<typeof (PH2)> ,XREF<ARG1> id) const {
			return Box<UNIT1>::make (mValue.self) ;
		}

		template <class ARG1>
		Box<UNIT1> template_clone (CREF<typeof (PH1)> ,XREF<ARG1> id) const {
			return NULL ;
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
trait SHAREDREF_SUPER_HELP ;

template <class...>
trait SHAREDREF_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG addr () const = 0 ;
		virtual BOOL good () const = 0 ;
		virtual void enter () const = 0 ;
		virtual void leave () const = 0 ;
	} ;
} ;

template <class UNIT1>
trait SHAREDREF_SUPER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class SharedRef {
	private:
		template <class...>
		friend trait SHAREDREF_HELP ;

	private:
		CRef<Holder> mThis ;
		Scope<CREF<Holder>> mHandle ;
		FLAG mPointer ;

	public:
		implicit SharedRef () noexcept {
			mPointer = ZERO ;
		}

		implicit SharedRef (CREF<SharedRef> that) :SharedRef () {
			if (that.mThis == NULL)
				return ;
			mThis = that.mThis ;
			if ifnot (mThis->good ())
				return ;
			mPointer = mThis->addr () ;
			mHandle = Scope<CREF<Holder>> (that.mThis.self) ;
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
	using Holder = typename SHAREDREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using SUPER = typename SHAREDREF_SUPER_HELP<UNIT1 ,ALWAYS>::SharedRef ;

	class SharedRef extend SUPER {
	private:
		template <class...>
		friend trait SHAREDREF_HELP ;

	private:
		using SUPER::mThis ;
		using SUPER::mHandle ;
		using SUPER::mPointer ;

	public:
		implicit SharedRef () = default ;

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

		BOOL available () const {
			if ifnot (exist ())
				return FALSE ;
			return mPointer != ZERO ;
		}

		VREF<UNIT1> m_self () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
		}

		inline implicit operator VREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
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

		BOOL good () const override {
			return mValue != NULL ;
		}

		void enter () const override {
			fake.mCounter++ ;
		}

		void leave () const override {
			fake.mCounter-- ;
			if (fake.mCounter > 0)
				return ;
			fake.mValue = NULL ;
		}

	private:
		VREF<ImplHolder> m_fake () const leftvalue {
			const auto r1x = address (thiz) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<ImplHolder>>::id] (unsafe_pointer (r1x))) ;
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

template <class DEPEND>
trait UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual FLAG addr () const = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_SUPER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	class UniqueRef {
	private:
		template <class...>
		friend trait UNIQUEREF_HELP ;

	private:
		VRef<Holder> mThis ;
		Scope<VREF<Holder>> mHandle ;
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
			assert (ifnot (rax.effective ())) ;
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
	using Holder = typename UNIQUEREF_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
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
			assert (ifnot (rax.effective ())) ;
			mThis = VRef<R2X>::make (move (rax)) ;
			mPointer = mThis->addr () ;
			constructor (fake) ;
			mHandle = Scope<VREF<Holder>> (mThis.self) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return fake ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

	private:
		VREF<UNIT1> m_fake () const leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT1>>::id] (unsafe_pointer (mPointer))) ;
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
			assert (size () > 0) ;
			return unsafe_array (mBuffer.self[0]) ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			assert (size () > 0) ;
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
			assume (that.mBuffer == NULL) ;
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
			assert (size () > 0) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			assert (size () > 0) ;
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
			mBuffer = that.mBuffer ;
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
			assume (that.mBuffer == NULL) ;
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
			assert (size () > 0) ;
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
		imports VRef<Buffer> from (VREF<VarBuffer<ITEM>> that) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::id] (unsafe_deptr (that))) ;
			return VRef<Buffer>::reference (tmp) ;
		}

		imports CRef<Buffer> from (CREF<VarBuffer<ITEM>> that) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::id] (unsafe_deptr (that))) ;
			return CRef<Buffer>::reference (tmp) ;
		}

		imports CRef<Buffer> from (CREF<ConBuffer<ITEM>> that) {
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<Buffer>>::id] (unsafe_deptr (that))) ;
			return CRef<Buffer>::reference (tmp) ;
		}

		imports VRef<Buffer> from (VREF<ARR<ITEM>> buffer ,CREF<LENGTH> begin_ ,CREF<LENGTH> end_) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			const auto r1x = vmax (end_ - begin_ ,ZERO) ;
			auto rax = VarBuffer<ITEM> () ;
			if ifswitch (TRUE) {
				if (r1x == ZERO)
					discard ;
				rax.mBuffer = VRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
			}
			rax.mSize = r1x ;
			auto rbx = VRef<VarBuffer<ITEM>>::make (move (rax)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<VRef<Buffer>>>::id] (unsafe_deptr (rbx))) ;
			return move (tmp) ;
		}

		imports CRef<Buffer> from (CREF<ARR<ITEM>> buffer ,CREF<LENGTH> begin_ ,CREF<LENGTH> end_) {
			assert (begin_ >= 0) ;
			assert (end_ >= 0) ;
			const auto r1x = vmax (end_ - begin_ ,ZERO) ;
			auto rax = ConBuffer<ITEM> () ;
			if ifswitch (TRUE) {
				if (r1x == ZERO)
					discard ;
				rax.mBuffer = CRef<ARR<ITEM>>::reference (unsafe_array (buffer[begin_])) ;
			}
			rax.mSize = r1x ;
			auto rbx = CRef<ConBuffer<ITEM>>::make (move (rax)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<CRef<Buffer>>>::id] (unsafe_deptr (rbx))) ;
			return move (tmp) ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		VREF<ARR<ITEM>> m_self () leftvalue {
			assert (size () > 0) ;
			return mBuffer.self ;
		}

		inline implicit operator VREF<ARR<ITEM>> () leftvalue {
			return self ;
		}

		CREF<ARR<ITEM>> m_self () const leftvalue {
			assert (size () > 0) ;
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
		}

		void clear () {
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
			assert (vbetween (index ,0 ,mSize)) ;
			return mAllocator[index].mNext == USED ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,mSize)) ;
			return mAllocator[index].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,mSize)) ;
			return mAllocator[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX alloc () {
			update_resize () ;
			INDEX ret = mFree ;
			mFree = mAllocator[ret].mNext ;
			mAllocator[ret].mNext = USED ;
			mLength++ ;
			return move (ret) ;
		}

		void free (CREF<INDEX> index) {
			assert (used (index)) ;
			drop (mAllocator[index].mItem) ;
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
				if (mSize >= mAllocator.size ())
					discard ;
				if (mFree != NONE)
					discard ;
				mAllocator[mSize].mNext = mFree ;
				mFree = mSize ;
				mSize++ ;
			}
			if ifswitch (TRUE) {
				if (mFree != NONE)
					discard ;
				const auto r1x = vmax (mSize * ALLOCATOR_POW::value ,ALLOCATOR_MIN_SIZE::value) ;
				resize (r1x) ;
			}
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
trait LATER_BINDER_HELP<UNIT1 ,ALWAYS> {
	using OWNERSHIP = typename LATER_HELP<UNIT1 ,ALWAYS>::OWNERSHIP ;
	using LATER_HEAP_SIZE = typename LATER_HELP<UNIT1 ,ALWAYS>::LATER_HEAP_SIZE ;

	using PureHolder = typename CREF_PUREHOLDER_HELP<UniqueRef<OWNERSHIP> ,ALWAYS>::PureHolder ;

	struct NODE {
		FLAG mTag ;
		Function<UNIT1> mExpr ;
		TEMP<PureHolder> mOwnership ;
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
trait LATER_HELP<UNIT1 ,ALWAYS> {
	using LATER_HEAP_SIZE = ENUMAS<VAL ,ENUMID<256>> ;

	template <class ARG1>
	using MACRO_NODE = typename DEPENDENT<LATER_BINDER_HELP<UNIT1 ,ALWAYS> ,ARG1>::NODE ;

	template <class ARG1>
	using MACRO_HEAP = typename DEPENDENT<LATER_BINDER_HELP<UNIT1 ,ALWAYS> ,ARG1>::HEAP ;

	struct OWNERSHIP {
		VRef<MACRO_HEAP<void>> mHeap ;
		INDEX mIndex ;
	} ;

	using NODE = MACRO_NODE<void> ;
	using HEAP = MACRO_HEAP<void> ;

	class Later {
	private:
		SharedRef<HEAP> mHeap ;
		CRef<UniqueRef<OWNERSHIP>> mThis ;

	public:
		implicit Later () = default ;

		explicit Later (CREF<FLAG> tag) {
			mHeap = unique_heap () ;
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
			mThis = CRef<UniqueRef<OWNERSHIP>>::intrusive (mHeap->mList[ix].mOwnership) ;
		}

		explicit Later (CREF<FLAG> tag ,RREF<Function<UNIT1>> expr) {
			assert (expr.exist ()) ;
			mHeap = unique_heap () ;
			auto rax = UniqueRef<OWNERSHIP> ([&] (VREF<OWNERSHIP> me) {
				me.mHeap = VRef<HEAP>::reference (mHeap) ;
				INDEX ix = me.mHeap->mFree ;
				assume (ix != NONE) ;
				me.mHeap->mList[ix].mTag = tag ;
				me.mHeap->mList[ix].mExpr = move (expr) ;
				zeroize (me.mHeap->mList[ix].mOwnership) ;
				me.mHeap->mFree = me.mHeap->mList[ix].mNext ;
				me.mHeap->mList[ix].mPrev = me.mHeap->mLast ;
				me.mHeap->mList[ix].mNext = NONE ;
				curr_next (me.mHeap ,me.mHeap->mLast ,ix) ;
				me.mHeap->mLast = ix ;
				me.mIndex = ix ;
			} ,[] (VREF<OWNERSHIP> me) {
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
			mThis = CRef<UniqueRef<OWNERSHIP>>::intrusive (mHeap->mList[ix].mOwnership ,move (rax)) ;
		}

		BOOL exist () const {
			if (mThis == NULL)
				return FALSE ;
			if (mThis.self == NULL)
				return FALSE ;
			return TRUE ;
		}

		UNIT1 invoke () const {
			return mHeap->mList[mThis->self.mIndex].mExpr () ;
		}

		inline UNIT1 operator() () const {
			return invoke () ;
		}

	private:
		imports CREF<SharedRef<HEAP>> unique_heap () {
			return memorize ([&] () {
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
using Later = typename LATER_HELP<UNIT1 ,ALWAYS>::Later ;
} ;
} ;

namespace CSC {
using namespace BASIC ;
} ;
