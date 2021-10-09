#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
namespace U {
template <class...>
trait ATOMIC_HELP ;

template <class UNIT1>
trait ATOMIC_HELP<UNIT1 ,REQUIRE<IS_BASIC<UNIT1>>> {
	class Atomic {
	private:
		std::atomic<UNIT1> mValue ;

	public:
		implicit Atomic () noexcept {
			mValue.store (UNIT1 (0)) ;
		}

		implicit Atomic (CREF<Atomic> that) noexcept {
			mValue.store (that.mValue.load ()) ;
		}

		inline VREF<Atomic> operator= (CREF<Atomic> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		UNIT1 fetch () const {
			return mValue.load () ;
		}

		void store (CREF<UNIT1> obj) {
			mValue.store (obj) ;
		}

		UNIT1 exchange (CREF<UNIT1> obj) {
			return mValue.exchange (obj) ;
		}

		BOOL change (VREF<UNIT1> expect ,CREF<UNIT1> obj) {
			return mValue.compare_exchange_strong (expect ,obj) ;
		}

		UNIT1 increase () {
			return mValue.fetch_add (UNIT1 (1)) + UNIT1 (1) ;
		}

		void operator++ (int) {
			const auto r1x = increase () ;
			noop (r1x) ;
		}

		UNIT1 decrease () {
			return mValue.fetch_add (UNIT1 (1)) - UNIT1 (1) ;
		}

		void operator-- (int) {
			const auto r1x = decrease () ;
			noop (r1x) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Atomic = typename U::ATOMIC_HELP<UNIT1 ,ALWAYS>::Atomic ;

namespace U {
template <class...>
trait OPTIONAL_HELP ;

template <class...>
trait OPTIONAL_DEFAULT_HELP ;

template <class UNIT1>
trait OPTIONAL_DEFAULT_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		Box<UNIT1> mSome ;

	public:
		implicit Optional () = default ;

		implicit Optional (CREF<Optional> that) {
			mSome = Box<UNIT1>::make (that.mSome.self) ;
		}

		implicit VREF<Optional> operator= (CREF<Optional> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Optional (RREF<Optional> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		implicit VREF<Optional> operator= (RREF<Optional> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}
	} ;
} ;

template <class UNIT1>
trait OPTIONAL_DEFAULT_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_CLONEABLE<UNIT1>>>> {
	class Optional {
	private:
		template <class...>
		friend trait OPTIONAL_HELP ;

	private:
		Box<UNIT1> mSome ;

	public:
		implicit Optional () = default ;
	} ;
} ;

template <class UNIT1>
trait OPTIONAL_HELP<UNIT1 ,ALWAYS> {
	using BASE = typename OPTIONAL_DEFAULT_HELP<UNIT1 ,ALWAYS>::Optional ;

	class Optional extend BASE {
	private:
		using BASE::mSome ;

	public:
		implicit Optional () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Optional>>>>
		explicit Optional (XREF<ARG1> obj1 ,XREF<ARG2>...objs) {
			mSome = Box<UNIT1>::make (forward[TYPEAS<ARG1>::id] (obj1) ,forward[TYPEAS<ARG2>::id] (objs)...) ;
		}

		imports CREF<Optional> none () {
			return memorize ([&] () {
				return Optional () ;
			}) ;
		}

		BOOL exist () const {
			return mSome != NULL ;
		}

		UNIT1 poll () {
			dynamic_assert (exist ()) ;
			UNIT1 ret = move (mSome.self) ;
			mSome = NULL ;
			return move (ret) ;
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
			auto &&thiz = *this ;
			if ifnot (exist ())
				return move (that) ;
			return move (thiz) ;
		}

		BOOL equal (CREF<Optional> that) const {
			return operator_equal (mSome.self ,that.mSome.self) ;
		}

		inline BOOL operator== (CREF<Optional> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Optional> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Optional> that) const {
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
			return operator_hash (mSome.self) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Optional = typename U::OPTIONAL_HELP<UNIT1 ,ALWAYS>::Optional ;

namespace U {
template <class...>
trait ANYREF_HELP ;

template <class...>
trait ANYREF_IMPLHOLDER_HELP ;

template <class...>
trait ANYREF_HOLDER_HELP ;

template <>
trait ANYREF_HOLDER_HELP<ALWAYS> {
	struct AnyRefHolder implement Interface {
		virtual Clazz clazz () const = 0 ;
		virtual Auto get () leftvalue = 0 ;
		virtual Auto get () const leftvalue = 0 ;
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,REQUIRE<IS_VOID<UNIT1>>> {
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::AnyRefHolder ;

	class AnyRef {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		VRef<Holder> mThis ;

	public:
		implicit AnyRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<
			ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,AnyRef>> ,
			HAS_REBIND<ARG1 ,UNIT1 ,AnyRef>>>>
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
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::AnyRefHolder ;

	class AnyRef {
	private:
		template <class...>
		friend trait ANYREF_HELP ;

	private:
		VRef<Holder> mThis ;

	public:
		implicit AnyRef () = default ;

		template <class ARG1 ,class = ENABLE<ENUM_ALL<
			ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,AnyRef>> ,
			HAS_REBIND<ARG1 ,UNIT1 ,AnyRef>>>>
			implicit AnyRef (XREF<ARG1> that) : AnyRef (move (that).rebind (TYPEAS<UNIT1>::id)) {
			noop () ;
		}

		template <class...ARG1>
		imports AnyRef make (XREF<ARG1>...objs) {
			using R1X = typename ANYREF_IMPLHOLDER_HELP<UNIT1 ,ALWAYS>::ImplHolder ;
			AnyRef ret ;
			ret.mThis = VRef<R1X>::make (UNIT1 (forward[TYPEAS<ARG1>::id] (objs)...)) ;
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

		VREF<UNIT1> to () leftvalue {
			const auto r1x = mThis->get ().cast (TYPEAS<VRef<UNIT1>>::id) ;
			return r1x.self ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			const auto r1x = mThis->get ().cast (TYPEAS<CRef<UNIT1>>::id) ;
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
	using Holder = typename ANYREF_HOLDER_HELP<ALWAYS>::AnyRefHolder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mValue ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<UNIT1> value_) {
			mValue = Box<UNIT1>::make (move (value_)) ;
		}

		Clazz clazz () const override {
			return Clazz (TYPEAS<UNIT1>::id) ;
		}

		Auto get () leftvalue override {
			return Auto (VRef<UNIT1>::reference (mValue.self)) ;
		}

		Auto get () const leftvalue override {
			return Auto (CRef<UNIT1>::reference (mValue.self)) ;
		}
	} ;
} ;
} ;

template <class UNIT1 = void>
using AnyRef = typename U::ANYREF_HELP<UNIT1 ,ALWAYS>::AnyRef ;

namespace U {
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
	using BASE = typename INTEGER_DEFAULT_HELP<SIZE ,ALWAYS>::Integer ;

	using HIGHEST = ENUM_DEC<SIZE> ;

	class Integer extend BASE {
	private:
		using BASE::mInteger ;

	public:
		implicit Integer () = default ;

		implicit Integer (CREF<VAL32> that) {
			const auto r1x = min (SIZE::value ,SIZE_OF<VAL32>::value) ;
			for (auto &&i : range (0 ,r1x)) {
				mInteger.self[i] = BYTE ((FEAT (that) >> LENGTH (i * 8)) & FEAT (0XFF)) ;
			}
		}

		implicit Integer (CREF<VAL64> that) {
			const auto r1x = min (SIZE::value ,SIZE_OF<VAL64>::value) ;
			for (auto &&i : range (0 ,r1x)) {
				mInteger.self[i] = BYTE ((FEAT (that) >> LENGTH (i * 8)) & FEAT (0XFF)) ;
			}
		}

		inline Integer operator+ (CREF<Integer> that) const {
			Integer ret ;
			auto rax = VAL32 (0) ;
			for (auto &&i : range (0 ,SIZE::value)) {
				const auto r1x = VAL32 (mInteger.self[i]) + VAL32 (that.mInteger.self[i]) + rax ;
				ret.mInteger.self[i] = BYTE (CHAR (r1x) & CHAR (0XFF)) ;
				rax = VAL32 (CHAR (r1x) >> LENGTH (8)) ;
			}
			return move (ret) ;
		}

		inline Integer operator- (CREF<Integer> that) const {
			Integer ret ;
			auto rax = VAL32 (0) ;
			for (auto &&i : range (0 ,SIZE::value)) {
				const auto r1x = VAL32 (mInteger.self[i]) - VAL32 (that.mInteger.self[i]) - rax ;
				const auto r2x = VAL32 (r1x < 0) ;
				const auto r3x = r1x + VAL32 (256) * r2x ;
				ret.mInteger.self[i] = BYTE (CHAR (r3x) & CHAR (0XFF)) ;
				rax = r2x ;
			}
			return move (ret) ;
		}

		inline Integer operator* (CREF<LENGTH> s) const {
			assert (abs (s) <= VAL32_MAX) ;
			Integer ret ;
			auto rax = VAL64 (0) ;
			for (auto &&i : range (0 ,SIZE::value)) {
				const auto r1x = VAL64 (mInteger.self[i]) * VAL64 (s) + rax ;
				ret.mInteger.self[i] = BYTE (FEAT (r1x) & FEAT (0XFF)) ;
				rax = VAL64 (FEAT (r1x) >> LENGTH (8)) ;
			}
			return move (ret) ;
		}

		inline Integer operator/ (CREF<LENGTH> s) const {
			assert (abs (s) <= VAL32_MAX) ;
			assert (s != ZERO) ;
			Integer ret ;
			auto rax = VAL64 (0) ;
			for (auto &&i : range (0 ,SIZE::value)) {
				const auto r1x = VAL64 (mInteger.self[i]) + rax ;
				const auto r2x = VAL64 (abs (r1x) < s) ;
				const auto r3x = r1x / VAL64 (s) ;
				ret.mInteger.self[i] = BYTE (FEAT (r3x) & FEAT (0XFF)) ;
				rax = (r1x - r3x * VAL64 (s)) * VAL64 (256) ;
			}
			return move (ret) ;
		}

		inline Integer operator% (CREF<LENGTH> s) const {
			auto &&thiz = *this ;
			return thiz - thiz / s * s ;
		}

		inline Integer operator+ () const {
			auto &&thiz = *this ;
			return thiz ;
		}

		inline Integer operator- () const {
			Integer ret ;
			for (auto &&i : range (0 ,SIZE::value))
				ret.mInteger.self[i] = ~mInteger.self[i] ;
			ret++ ;
			return move (ret) ;
		}

		inline Integer operator++ () {
			auto &&thiz = *this ;
			Integer ret = thiz ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= SIZE::value)
					break ;
				mInteger.self[ix]++ ;
				if (mInteger.self[ix] != BYTE (0X00))
					break ;
				ix++ ;
			}
			return move (ret) ;
		}

		inline Integer operator-- () {
			auto &&thiz = *this ;
			Integer ret = thiz ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= SIZE::value)
					break ;
				mInteger.self[ix]-- ;
				if (mInteger.self[ix] != BYTE (0XFF))
					break ;
				ix++ ;
			}
			return move (ret) ;
		}
	} ;
} ;
} ;

template <class SIZE>
using Integer = typename U::INTEGER_HELP<SIZE ,ALWAYS>::Integer ;

namespace U {
template <class...>
trait TUPLE_HELP ;

template <>
trait TUPLE_HELP<TYPEAS<> ,ALWAYS> {
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

template <class...PARAMS>
trait TUPLE_HELP<TYPEAS<PARAMS...> ,REQUIRE<ENUM_GT_ZERO<COUNT_OF<TYPEAS<PARAMS...>>>>> {
	using ONE_TYPE = TYPE_FIRST_ONE<TYPEAS<PARAMS...>> ;
	using REST_TYPE = TYPE_FIRST_REST<TYPEAS<PARAMS...>> ;
	using REST_TUPLE = typename TUPLE_HELP<REST_TYPE ,ALWAYS>::Tuple ;
	require (IS_DEFAULT<ONE_TYPE>) ;

	class Tuple {
	private:
		ONE_TYPE mOne ;
		REST_TUPLE mRest ;

	public:
		implicit Tuple () = default ;

		template <class ARG1 ,class...ARG2 ,class = ENABLE<ENUM_NOT<IS_SAME<REMOVE_ALL<ARG1> ,Tuple>>>>
		explicit Tuple (XREF<ARG1> one_ ,XREF<ARG2>...rest_) {
			initialize (forward[TYPEAS<ARG1>::id] (one_) ,forward[TYPEAS<ARG2>::id] (rest_)...) ;
		}

		template <class ARG1 ,class...ARG2>
		void initialize (RREF<ARG1> one_ ,XREF<ARG2>...rest_) {
			mOne = ONE_TYPE (forward[TYPEAS<ARG1>::id] (one_)) ;
			mRest.initialize (forward[TYPEAS<ARG2>::id] (rest_)...) ;
		}

		LENGTH rank () const {
			return COUNT_OF<TYPEAS<PARAMS...>>::value ;
		}

		VREF<ONE_TYPE> one () leftvalue {
			return mOne ;
		}

		CREF<ONE_TYPE> one () const leftvalue {
			return mOne ;
		}

		VREF<REST_TUPLE> rest () leftvalue {
			return mRest ;
		}

		CREF<REST_TUPLE> rest () const leftvalue {
			return mRest ;
		}

		template <class ARG1>
		VREF<TYPE_PICK<TYPEAS<PARAMS...> ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) leftvalue {
			auto &&thiz = *this ;
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<TYPEAS<PARAMS...>>>) ;
			using R2X = TYPE_PICK<TYPEAS<PARAMS...> ,R1X> ;
			return template_pick (id ,TYPEAS<VREF<R2X>>::id ,thiz ,PHX) ;
		}

		template <class ARG1>
		CREF<TYPE_PICK<TYPEAS<PARAMS...> ,REMOVE_ALL<ARG1>>> pick (XREF<ARG1> id) const leftvalue {
			auto &&thiz = *this ;
			using R1X = REMOVE_ALL<ARG1> ;
			require (ENUM_BETWEEN<R1X ,ENUM_ZERO ,COUNT_OF<TYPEAS<PARAMS...>>>) ;
			using R2X = TYPE_PICK<TYPEAS<PARAMS...> ,R1X> ;
			return template_pick (id ,TYPEAS<CREF<R2X>>::id ,thiz ,PHX) ;
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

	private:
		template <class ARG1 ,class ARG2 ,class ARG3 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_ALL<ARG1>>>>
		imports XREF<REMOVE_ALL<ARG2>> template_pick (XREF<ARG1> id1 ,XREF<ARG2> id2 ,XREF<ARG3> curr ,CREF<typeof (PH2)>) {
			return curr.one () ;
		}

		template <class ARG1 ,class ARG2 ,class ARG3 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_ALL<ARG1>>>>
		imports XREF<REMOVE_ALL<ARG2>> template_pick (XREF<ARG1> id1 ,XREF<ARG2> id2 ,XREF<ARG3> curr ,CREF<typeof (PH1)>) {
			using R1X = REMOVE_ALL<ARG1> ;
			return template_pick (TYPEAS<ENUM_DEC<R1X>>::id ,id2 ,curr.rest () ,PHX) ;
		}
	} ;
} ;
} ;

template <class...PARAMS>
using Tuple = typename U::TUPLE_HELP<TYPEAS<PARAMS...> ,ALWAYS>::Tuple ;

namespace U {
template <class...>
trait IS_EFFECTIVE_HELP ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait IS_EFFECTIVE_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,ALWAYS> {
	using R1X = UNIT1 ;
	using R2X = DEF<RETURN (*) (PARAMS...)> ;
	using RET = ENUMAS<BOOL ,U::ENUMID<(std::is_convertible<R1X ,R2X>::value)>> ;
} ;
} ;

template <class UNIT1>
using IS_EFFECTIVE = typename U::IS_EFFECTIVE_HELP<FUNCTION_RETURN<UNIT1> ,FUNCTION_PARAMS<UNIT1> ,UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait IS_EXCEPTION_HELP ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait IS_EXCEPTION_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,REQUIRE<MACRO_STD_BASIC<UNIT1>>> {
	using RET = ENUM_TRUE ;
} ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait IS_EXCEPTION_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,REQUIRE<MACRO_STD_LATEST<UNIT1>>> {
	using RET = ENUMAS<BOOL ,U::ENUMID<(std::is_nothrow_invocable<UNIT1 ,XREF<PARAMS>...>::value)>> ;
} ;
} ;

template <class UNIT1>
using IS_EXCEPTION = typename U::IS_EXCEPTION_HELP<FUNCTION_RETURN<UNIT1> ,FUNCTION_PARAMS<UNIT1> ,UNIT1 ,ALWAYS>::RET ;

namespace U {
template <class...>
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_IMPLHOLDER_HELP ;

template <class RETURN ,class...PARAMS>
trait FUNCTION_HELP<RETURN ,TYPEAS<PARAMS...> ,ALWAYS> {
	struct FunctionHolder implement Interface {
		virtual BOOL effective () const = 0 ;
		virtual BOOL exception () const = 0 ;
		virtual RETURN invoke (XREF<PARAMS>...params) const = 0 ;
	} ;

	using Holder = FunctionHolder ;

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
			mThis = CRef<R2X>::make (R1X (forward[TYPEAS<ARG1>::id] (that))) ;
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

		Function try_else () {
			auto &&thiz = *this ;
			//@mark
			return thiz ;
		}
	} ;
} ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,ALWAYS> {
	using Holder = typename FUNCTION_HELP<RETURN ,TYPEAS<PARAMS...> ,ALWAYS>::FunctionHolder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mFunctor ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<UNIT1> functor) {
			mFunctor = Box<UNIT1>::make (move (functor)) ;
		}

		BOOL effective () const override {
			return IS_EFFECTIVE<UNIT1>::value ;
		}

		BOOL exception () const override {
			return IS_EXCEPTION<UNIT1>::value ;
		}

		RETURN invoke (XREF<PARAMS>...params) const override {
			return mFunctor.self (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}
	} ;
} ;
} ;

template <class RETURN ,class PARAMS = TYPEAS<>>
using Function = typename U::FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Function ;

namespace U {
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

		void enter () {
			assert (mBase.exist ()) ;
		}

		void leave () {
			mBase () ;
		}
	} ;
} ;
} ;

using Finally = typename U::FINALLY_HELP<ALWAYS>::Finally ;

namespace U {
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
			auto rax = obj.unlock () ;
			dynamic_assert (rax != NULL) ;
			mCRef.store (move (rax)) ;
		}

		void apply (CREF<Function<void ,TYPEAS<VREF<UNIT1>>>> proc) const {
			auto rax = borrow () ;
			if (rax == NULL)
				return ;
			const auto r1x = Function<void> ([&] () {
				restore (move (rax)) ;
			}) ;
			Scope<CREF<Finally>> anonymous (Finally::from (r1x)) ;
			proc (rax.self) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using SharedRef = typename U::SHAREDREF_HELP<UNIT1 ,ALWAYS>::SharedRef ;

namespace U {
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

		CREF<UNIT1> to () const leftvalue {
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
			if ifnot (mDestructor.exist ())
				return ;
			mDestructor () ;
		}
	} ;

	class ImplHolder extend BaseHolder {
	private:
		using BaseHolder::mDestructor ;
		Scope<VREF<BaseHolder>> mHandle ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Function<void>> destructor) {
			dynamic_assert (ifnot (destructor.effective ())) ;
			mDestructor = move (destructor) ;
		}

		void acquire () override {
			auto &&thiz = *this ;
			mHandle = Scope<VREF<BaseHolder>> (thiz) ;
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
			if ifnot (mDestructor.exist ())
				return ;
			mDestructor (mValue) ;
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
			dynamic_assert (ifnot (destructor.effective ())) ;
			mDestructor = move (destructor) ;
		}

		VREF<UNIT1> at () leftvalue override {
			return mValue ;
		}

		CREF<UNIT1> at () const leftvalue override {
			return mValue ;
		}

		void acquire () override {
			auto &&thiz = *this ;
			mHandle = Scope<VREF<BaseHolder>> (thiz) ;
		}

		void release () override {
			mHandle.release () ;
		}
	} ;
} ;
} ;

template <class UNIT1 = void>
using UniqueRef = typename U::UNIQUEREF_HELP<UNIT1 ,ALWAYS>::UniqueRef ;

namespace U {
template <class...>
trait BUFFER_HELP ;
} ;

using VARBUFFER_SIZE = ENUMAS<VAL ,U::ENUMID<(-1)>> ;
using CONBUFFER_SIZE = ENUMAS<VAL ,U::ENUMID<(-2)>> ;
using REGBUFFER_SIZE = ENUMAS<VAL ,U::ENUMID<(-3)>> ;

namespace U {
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
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (mBuffer.self) ;
		}

		implicit VREF<Buffer> operator= (CREF<Buffer> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		implicit VREF<Buffer> operator= (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
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
	using BASE = typename BOXBUFFER_DEFAULT_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

	class Buffer extend BASE {
	private:
		using BASE::mBuffer ;

	public:
		implicit Buffer () = default ;

		template <class ARG1 = ALWAYS>
		implicit Buffer (CREF<DEPENDENT<ARR<ITEM ,SIZE> ,ARG1>> that) {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (that) ;
		}

		template <class ARG1 = ALWAYS>
		implicit Buffer (RREF<DEPENDENT<ARR<ITEM ,SIZE> ,ARG1>> that) {
			mBuffer = Box<ARR<ITEM ,SIZE>>::make (move (that)) ;
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

		LENGTH addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
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
			return operator_equal (mBuffer.self ,that.mBuffer.self) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			return operator_compr (mBuffer.self ,that.mBuffer.self) ;
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
			return operator_hash (mBuffer.self) ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE>
using Buffer = typename U::BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class ITEM ,class SIZE>
using BoxBuffer = typename U::BUFFER_HELP<ITEM ,ENABLE<ENUM_GT_ZERO<SIZE> ,SIZE> ,ALWAYS>::Buffer ;

namespace U {
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
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mSize == 0)
				return ;
			mSize = 0 ;
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
			for (auto &&i : range (0 ,that.mSize))
				mBuffer.self[i] = that.mBuffer.self[i] ;
			mSize = that.mSize ;
		}

		implicit VREF<Buffer> operator= (CREF<Buffer> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		implicit VREF<Buffer> operator= (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
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
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,VARBUFFER_SIZE>>> {
	using BASE = typename VARBUFFER_DEFAULT_HELP<ITEM ,ALWAYS>::Buffer ;

	class Buffer extend BASE {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		using BASE::mBuffer ;
		using BASE::mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}

		template <class ARG1 = ALWAYS>
		implicit Buffer (RREF<typename DEPENDENT<BUFFER_HELP<ITEM ,CONBUFFER_SIZE ,ALWAYS> ,ARG1>::Buffer> that) {
			mBuffer = that.mBuffer.lock () ;
			mSize = that.mSize ;
			if (that.mBuffer == NULL)
				return ;
			dynamic_assert (mBuffer != NULL) ;
		}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			mSize = 0 ;
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

		LENGTH addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
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
			const auto r1x = min (size_ ,mSize) ;
			for (auto &&i : range (0 ,r1x))
				rax.self[i] = move (mBuffer.self[i]) ;
			mBuffer = move (rax) ;
			mSize = size_ ;
		}

		BOOL equal (CREF<Buffer> that) const {
			return operator_equal (mBuffer.self ,that.mBuffer.self) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			return operator_compr (mBuffer.self ,that.mBuffer.self) ;
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
			return operator_hash (mBuffer.self) ;
		}
	} ;
} ;
} ;

template <class ITEM>
using VarBuffer = typename U::BUFFER_HELP<ITEM ,VARBUFFER_SIZE ,ALWAYS>::Buffer ;

namespace U {
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
		implicit Buffer () = default ;

		implicit Buffer (CREF<Buffer> that) {
			if (that.mSize == 0)
				return ;
			mSize = 0 ;
			mBuffer = VRef<ARR<ITEM>>::make (that.mSize) ;
			for (auto &&i : range (0 ,that.mSize))
				mBuffer.self[i] = that.mBuffer.self[i] ;
			mSize = that.mSize ;
		}

		implicit VREF<Buffer> operator= (CREF<Buffer> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		implicit VREF<Buffer> operator= (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
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
		implicit Buffer () = default ;
	} ;
} ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,CONBUFFER_SIZE>>> {
	using BASE = typename CONBUFFER_DEFAULT_HELP<ITEM ,ALWAYS>::Buffer ;

	class Buffer extend BASE {
	private:
		template <class...>
		friend trait BUFFER_HELP ;

	private:
		using BASE::mBuffer ;
		using BASE::mSize ;

	public:
		implicit Buffer () noexcept {
			mSize = 0 ;
		}

		template <class ARG1 = ALWAYS>
		implicit Buffer (RREF<typename DEPENDENT<BUFFER_HELP<ITEM ,VARBUFFER_SIZE ,ALWAYS> ,ARG1>::Buffer> that) {
			mBuffer = that.mBuffer.unlock () ;
			mSize = that.mSize ;
			if (that.mBuffer == NULL)
				return ;
			dynamic_assert (mBuffer != NULL) ;
		}

		explicit Buffer (CREF<LENGTH> size_) {
			assert (size_ == 0) ;
			mSize = 0 ;
		}

		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		LENGTH addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
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
			return operator_equal (mBuffer.self ,that.mBuffer.self) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			return operator_compr (mBuffer.self ,that.mBuffer.self) ;
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
			return operator_hash (mBuffer.self) ;
		}
	} ;
} ;
} ;

template <class ITEM>
using ConBuffer = typename U::BUFFER_HELP<ITEM ,CONBUFFER_SIZE ,ALWAYS>::Buffer ;

namespace U {
template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<IS_SAME<SIZE ,REGBUFFER_SIZE>>> {
	class Buffer extend Proxy {
	private:
		VRef<ARR<ITEM>> mBuffer ;
		LENGTH mSize ;

	public:
		LENGTH size () const {
			if (mBuffer == NULL)
				return ZERO ;
			return mSize ;
		}

		LENGTH addr () const {
			if (mBuffer == NULL)
				return ZERO ;
			return address (mBuffer.self) ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mBuffer.self[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
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
			return operator_equal (mBuffer.self ,that.mBuffer.self) ;
		}

		inline BOOL operator== (CREF<Buffer> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Buffer> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Buffer> that) const {
			return operator_compr (mBuffer.self ,that.mBuffer.self) ;
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
			return operator_hash (mBuffer.self) ;
		}
	} ;
} ;
} ;

template <class ITEM>
using RegBuffer = typename U::BUFFER_HELP<ITEM ,REGBUFFER_SIZE ,ALWAYS>::Buffer ;

namespace U {
template <class...>
trait ALLOCATOR_HELP ;

template <class ITEM ,class SIZE>
trait ALLOCATOR_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	struct NODE {
		ITEM mItem ;
		INDEX mNext ;
	} ;

	using ALLOCATOR_POW = RANK2 ;
	using ALLOCATOR_MIN_SIZE = ENUMAS<VAL ,U::ENUMID<256>> ;

	class Allocator {
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

		explicit Allocator (CREF<LENGTH> size_) {
			mAllocator = Buffer<NODE ,SIZE> (size_) ;
			clear () ;
		}

		void clear () {
			mSize = mAllocator.size () ;
			for (auto &&i : range (0 ,mSize))
				mAllocator[i].mItem = ITEM () ;
			mLength = 0 ;
			update_reserve (NONE ,0 ,mSize) ;
		}

		LENGTH size () const {
			if (mAllocator.size () == 0)
				return 0 ;
			return mSize ;
		}

		LENGTH length () const {
			if (mAllocator.size () == 0)
				return 0 ;
			return mLength ;
		}

		BOOL used (CREF<INDEX> index) const {
			assert (between (index ,0 ,mSize)) ;
			return mAllocator[index].mNext == USED ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mAllocator[index].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
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
			const auto r1x = min (size_ ,mSize) ;
			mLength = r1x ;
			for (auto &&i : range (0 ,r1x)) {
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
			for (auto &&i : range (begin_ ,end_)) {
				INDEX iy = end_ - 1 - i ;
				mAllocator[iy].mNext = ix ;
				ix = iy ;
			}
			mFree = ix ;
		}

		void update_resize () {
			if (mFree != NONE)
				return ;
			const auto r1x = max (mSize * ALLOCATOR_POW::value ,ALLOCATOR_MIN_SIZE::value) ;
			resize (r1x) ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE>
using Allocator = typename U::ALLOCATOR_HELP<ITEM ,SIZE ,ALWAYS>::Allocator ;
} ;