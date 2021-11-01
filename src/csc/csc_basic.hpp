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
trait OPTIONAL_DEFAULT_HELP ;

template <class UNIT1>
trait OPTIONAL_DEFAULT_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
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
trait OPTIONAL_DEFAULT_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
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

		implicit Optional (CREF<Optional> that) {
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
trait OPTIONAL_DEFAULT_HELP<UNIT1 ,REQUIRE<ENUM_ALL<
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
	using SUPER = typename OPTIONAL_DEFAULT_HELP<UNIT1 ,ALWAYS>::Optional ;

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
	using SUPER = typename OPTIONAL_DEFAULT_HELP<UNIT1 ,ALWAYS>::Optional ;

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

		UNIT1 poll () {
			dynamic_assert (exist ()) ;
			UNIT1 ret = move (mSome.self) ;
			mSome = NULL ;
			return move (ret) ;
		}

		UNIT1 poll (CREF<UNIT1> obj) {
			return poll (move (obj)) ;
		}

		UNIT1 poll (RREF<UNIT1> obj) {
			UNIT1 ret = move (obj) ;
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
trait INTEGER_DEFAULT_HELP ;

template <class SIZE>
trait INTEGER_DEFAULT_HELP<SIZE ,ALWAYS> {
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
	using SUPER = typename INTEGER_DEFAULT_HELP<SIZE ,ALWAYS>::Integer ;

	using HIGHEST = ENUM_DEC<SIZE> ;

	class Integer extend SUPER {
	private:
		using SUPER::mInteger ;

	public:
		implicit Integer () = default ;

		implicit Integer (CREF<VAL32> that) {
			const auto r1x = vmin (SIZE::value ,SIZE_OF<VAL32>::value) ;
			const auto r2x = CHAR (that) ;
			for (auto &&i : iter (0 ,r1x)) {
				mInteger.self[i] = BYTE ((r2x >> (i * 8)) & CHAR (0XFF)) ;
			}
			const auto r3x = invoke ([&] () {
				if (that >= 0)
					return BYTE (0X00) ;
				return BYTE (0XFF) ;
			}) ;
			for (auto &&i : iter (r1x ,SIZE::value)) {
				mInteger.self[i] = r3x ;
			}
		}

		implicit Integer (CREF<VAL64> that) {
			const auto r1x = vmin (SIZE::value ,SIZE_OF<VAL64>::value) ;
			const auto r2x = DATA (that) ;
			for (auto &&i : iter (0 ,r1x)) {
				mInteger.self[i] = BYTE ((r2x >> (i * 8)) & DATA (0XFF)) ;
			}
			const auto r3x = invoke ([&] () {
				if (that >= 0)
					return BYTE (0X00) ;
				return BYTE (0XFF) ;
			}) ;
			for (auto &&i : iter (r1x ,SIZE::value)) {
				mInteger.self[i] = r3x ;
			}
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
				rax = (r1x - r2x * VAL64 (scale)) * VAL64 (256) ;
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

	using FOREWARD_NODE = PACK<FIRST_ONE ,FIRST_REST_TUPLE> ;
	using BACKWARD_NODE = PACK<LAST_REST_TUPLE ,LAST_ONE> ;

	class Tuple {
	private:
		FOREWARD_NODE mTuple ;

	public:
		implicit Tuple () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Tuple>>>>
		explicit Tuple (XREF<ARG1> one_ ,XREF<ARG2>...rest_) {
			initialize (forward[TYPEAS<ARG1>::id] (one_) ,forward[TYPEAS<ARG2>::id] (rest_)...) ;
		}

		template <class...ARG1>
		void initialize (CREF<FIRST_ONE> one_ ,XREF<ARG1>...rest_) {
			initialize (move (one_) ,forward[TYPEAS<ARG1>::id] (rest_)...) ;
		}

		template <class...ARG1>
		void initialize (RREF<FIRST_ONE> one_ ,XREF<ARG1>...rest_) {
			mTuple.mP1 = move (one_) ;
			mTuple.mP2.initialize (forward[TYPEAS<ARG1>::id] (rest_)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<UNIT1>::value ;
		}

		VREF<FIRST_ONE> first_one () leftvalue {
			return mTuple.mP1 ;
		}

		CREF<FIRST_ONE> first_one () const leftvalue {
			return mTuple.mP1 ;
		}

		VREF<FIRST_REST_TUPLE> first_rest () leftvalue {
			return mTuple.mP2 ;
		}

		CREF<FIRST_REST_TUPLE> first_rest () const leftvalue {
			return mTuple.mP2 ;
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
	using RET = IS_CONVERTIBLE<CREF<R1X> ,RREF<R2X>> ;
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
			return FALSE ;
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
		implicit Finally () = delete ;

		imports CREF<Finally> from (CREF<Function<void>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Finally>>::id ,unsafe_deptr (that))) ;
		}

		void enter () const {
			assert (mBase.exist ()) ;
		}

		void leave () const {
			try_invoke ([&] () {
				mBase () ;
			} ,[&] () {
				noop () ;
			}) ;
		}
	} ;
} ;

using Finally = typename FINALLY_HELP<ALWAYS>::Finally ;

template <class...>
trait ANYREF_HELP ;

template <class...>
trait ANYREF_HOLDER_HELP ;

template <class...>
trait ANYREF_IMPLHOLDER_HELP ;

template <>
trait ANYREF_HOLDER_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual Clazz clazz () const = 0 ;
		virtual Auto get () leftvalue = 0 ;
		virtual Auto get () const leftvalue = 0 ;
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::Holder ;

	class AnyRef {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		VRef<Holder> mThis ;

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
		DEF<typename ANYREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS>::AnyRef> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename ANYREF_HELP<R1X ,ALWAYS>::AnyRef ;
			R2X ret ;
			ret.mThis = move (mThis) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::Holder ;

	class AnyRef {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		VRef<Holder> mThis ;

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
			const auto r1x = mThis->get ().as (TYPEAS<VRef<UNIT1>>::id) ;
			return r1x.self ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			const auto r1x = mThis->get ().as (TYPEAS<CRef<UNIT1>>::id) ;
			return r1x.self ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		template <class ARG1>
		DEF<typename ANYREF_HELP<REMOVE_ALL<ARG1> ,ALWAYS>::AnyRef> rebind (XREF<ARG1> id) rightvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = typename ANYREF_HELP<R1X ,ALWAYS>::AnyRef ;
			R2X ret ;
			ret.mThis = move (mThis) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1>
trait ANYREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Box<UNIT1>> value_) {
			mValue = move (value_) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPEAS<UNIT1>::id) ;
		}

		Auto get () leftvalue override {
			return VRef<UNIT1>::reference (mValue.self) ;
		}

		Auto get () const leftvalue override {
			return CRef<UNIT1>::reference (mValue.self) ;
		}
	} ;
} ;

template <class UNIT1 = void>
using AnyRef = typename ANYREF_HELP<UNIT1 ,ALWAYS>::AnyRef ;

template <class...>
trait SHAREDREF_HELP ;

template <class UNIT1>
trait SHAREDREF_HELP<UNIT1 ,ALWAYS> {
	class SharedRef {
	private:
		template <class...>
		friend trait SHAREDREF_HELP ;

	private:
		Cell<CRef<UNIT1>> mCRef ;

	public:
		implicit SharedRef () = default ;

		template <class...ARG1>
		imports SharedRef make (XREF<ARG1>...objs) {
			SharedRef ret ;
			auto rax = CRef<UNIT1>::make (forward[TYPEAS<ARG1>::id] (objs)...) ;
			ret.mCRef.initialize (move (rax)) ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mCRef == NULL)
				return FALSE ;
			if (mCRef.fetch () == NULL)
				return FALSE ;
			return TRUE ;
		}

		CRef<UNIT1> at () const {
			return mCRef.fetch () ;
		}

		VRef<UNIT1> borrow () const {
			auto rax = mCRef.exchange (NULL) ;
			VRef<UNIT1> ret = rax.lock () ;
			if ifswitch (TRUE) {
				if (ret != NULL)
					discard ;
				mCRef.store (rax) ;
				dynamic_assert (FALSE) ;
			}
			return move (ret) ;
		}

		void restore (RREF<VRef<UNIT1>> obj) const {
			//@mark
			auto rax = obj.unlock () ;
			dynamic_assert (rax != NULL) ;
			mCRef.store (move (rax)) ;
		}
	} ;
} ;

template <class UNIT1>
using SharedRef = typename SHAREDREF_HELP<UNIT1 ,ALWAYS>::SharedRef ;

template <class...>
trait UNIQUEREF_HELP ;

template <class...>
trait UNIQUEREF_IMPLHOLDER_HELP ;

template <class UNIT1>
trait UNIQUEREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	struct Holder implement Interface {
		virtual void acquire () = 0 ;
		virtual void release () = 0 ;
	} ;

	class UniqueRef {
	private:
		VRef<Holder> mThis ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (XREF<ARG1> constructor ,XREF<ARG2> destructor) {
			using R1X = Function<void> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			mThis = VRef<R2X>::make (R1X (forward[TYPEAS<ARG2>::id] (destructor))) ;
			constructor () ;
			mThis->acquire () ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		void release () {
			if (mThis == NULL)
				return ;
			mThis->release () ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_HELP<UNIT1 ,REQUIRE<IS_DEFAULT<UNIT1>>> {
	struct Holder implement Interface {
		virtual VREF<UNIT1> at () leftvalue = 0 ;
		virtual CREF<UNIT1> at () const leftvalue = 0 ;
		virtual void acquire () = 0 ;
		virtual void release () = 0 ;
	} ;

	class UniqueRef {
	private:
		VRef<Holder> mThis ;

	public:
		implicit UniqueRef () = default ;

		template <class ARG1 ,class ARG2>
		explicit UniqueRef (XREF<ARG1> constructor ,XREF<ARG2> destructor) {
			using R1X = Function<void ,TYPEAS<VREF<UNIT1>>> ;
			using R2X = typename UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			mThis = VRef<R2X>::make (R1X (forward[TYPEAS<ARG2>::id] (destructor))) ;
			constructor (mThis->at ()) ;
			mThis->acquire () ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		CREF<UNIT1> m_self () const leftvalue {
			return mThis->at () ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

		void release () {
			if (mThis == NULL)
				return ;
			mThis->release () ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class BaseHolder implement Holder {
	private:
		template <class...>
		friend trait UNIQUEREF_IMPLHOLDER_HELP ;

	private:
		Function<void> mDestructor ;

	public:
		implicit BaseHolder () = default ;

		void enter () {
			noop () ;
		}

		void leave () {
			try_invoke ([&] () {
				if ifnot (mDestructor.exist ())
					return ;
				mDestructor () ;
			} ,[&] () {
				noop () ;
			}) ;
		}
	} ;

	class ImplHolder extend BaseHolder {
	private:
		using BaseHolder::mDestructor ;
		Scope<VREF<BaseHolder>> mHandle ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Function<void>> destructor) {
			assert (ifnot (destructor.effective ())) ;
			mDestructor = move (destructor) ;
		}

		void acquire () override {
			mHandle.acquire (thiz) ;
		}

		void release () override {
			mHandle.release () ;
		}
	} ;
} ;

template <class UNIT1>
trait UNIQUEREF_IMPLHOLDER_HELP<UNIT1 ,REQUIRE<IS_DEFAULT<UNIT1>>> {
	using Holder = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::Holder ;

	class BaseHolder implement Holder {
	private:
		template <class...>
		friend trait UNIQUEREF_IMPLHOLDER_HELP ;

	private:
		UNIT1 mValue ;
		Function<void ,TYPEAS<VREF<UNIT1>>> mDestructor ;

	public:
		implicit BaseHolder () = default ;

		void enter () {
			noop () ;
		}

		void leave () {
			try_invoke ([&] () {
				if ifnot (mDestructor.exist ())
					return ;
				mDestructor (mValue) ;
			} ,[&] () {
				noop () ;
			}) ;
		}
	} ;

	class ImplHolder extend BaseHolder {
	private:
		using BaseHolder::mValue ;
		using BaseHolder::mDestructor ;
		Scope<VREF<BaseHolder>> mHandle ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Function<void ,TYPEAS<VREF<UNIT1>>>> destructor) {
			assert (ifnot (destructor.effective ())) ;
			mDestructor = move (destructor) ;
		}

		VREF<UNIT1> at () leftvalue override {
			return mValue ;
		}

		CREF<UNIT1> at () const leftvalue override {
			return mValue ;
		}

		void acquire () override {
			mHandle.acquire (thiz) ;
		}

		void release () override {
			mHandle.release () ;
		}
	} ;
} ;

template <class UNIT1 = void>
using UniqueRef = typename UNIQUEREF_HELP<UNIT1 ,ALWAYS>::UniqueRef ;

template <class...>
trait BUFFER_HELP ;

using VARBUFFER = ENUMAS<VAL ,ENUMID<(-1)>> ;
using CONBUFFER = ENUMAS<VAL ,ENUMID<(-2)>> ;
using REGBUFFER = ENUMAS<VAL ,ENUMID<(-3)>> ;

template <class...>
trait BOXBUFFER_DEFAULT_HELP ;

template <class ITEM ,class SIZE>
trait BOXBUFFER_DEFAULT_HELP<ITEM ,SIZE ,REQUIRE<IS_CLONEABLE<ITEM>>> {
	class Buffer {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		Box<ARR<ITEM ,SIZE>> mBuffer ;

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
trait BOXBUFFER_DEFAULT_HELP<ITEM ,SIZE ,REQUIRE<ENUM_NOT<IS_CLONEABLE<ITEM>>>> {
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
	using SUPER = typename BOXBUFFER_DEFAULT_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

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

		FLAG addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
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
trait VARBUFFER_DEFAULT_HELP ;

template <class ITEM>
trait VARBUFFER_DEFAULT_HELP<ITEM ,REQUIRE<IS_CLONEABLE<ITEM>>> {
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
trait VARBUFFER_DEFAULT_HELP<ITEM ,REQUIRE<ENUM_NOT<IS_CLONEABLE<ITEM>>>> {
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
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,VARBUFFER>>> {
	using SUPER = typename VARBUFFER_DEFAULT_HELP<ITEM ,ALWAYS>::Buffer ;

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
		implicit Buffer (RREF<typename DEPENDENT<BUFFER_HELP<ITEM ,CONBUFFER ,ALWAYS> ,ARG1>::Buffer> that) {
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

		FLAG addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
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
using VarBuffer = typename BUFFER_HELP<ITEM ,VARBUFFER ,ALWAYS>::Buffer ;

template <class...>
trait CONBUFFER_DEFAULT_HELP ;

template <class ITEM>
trait CONBUFFER_DEFAULT_HELP<ITEM ,REQUIRE<IS_CLONEABLE<ITEM>>> {
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
trait CONBUFFER_DEFAULT_HELP<ITEM ,REQUIRE<ENUM_NOT<IS_CLONEABLE<ITEM>>>> {
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
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,CONBUFFER>>> {
	using SUPER = typename CONBUFFER_DEFAULT_HELP<ITEM ,ALWAYS>::Buffer ;

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
		implicit Buffer (RREF<typename DEPENDENT<BUFFER_HELP<ITEM ,VARBUFFER ,ALWAYS> ,ARG1>::Buffer> that) {
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

		FLAG addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
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
using ConBuffer = typename BUFFER_HELP<ITEM ,CONBUFFER ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,REGBUFFER>>> {
	class Buffer extend Proxy {
	private:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () = delete ;

		imports VREF<Buffer> from (VREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (CREF<VarBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		imports CREF<Buffer> from (CREF<ConBuffer<ITEM>> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<Buffer>>::id ,unsafe_deptr (that))) ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		FLAG addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
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
using RegBuffer = typename BUFFER_HELP<ITEM ,REGBUFFER ,ALWAYS>::Buffer ;

template <class...>
trait ALLOCATOR_HELP ;

template <class...>
trait ALLOCATOR_DEFAULT_HELP ;

template <class ITEM ,class SIZE>
trait ALLOCATOR_DEFAULT_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
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
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait ALLOCATOR_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	using NODE = typename ALLOCATOR_DEFAULT_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename ALLOCATOR_DEFAULT_HELP<ITEM ,SIZE ,ALWAYS>::Allocator ;

	using ALLOCATOR_POW = RANK2 ;
	using ALLOCATOR_MIN_SIZE = ENUMAS<VAL ,ENUMID<(256)>> ;

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
} ;
} ;

namespace CSC {
using namespace BASIC ;
} ;
