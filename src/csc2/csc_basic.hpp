#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
namespace U {
template <class...>
trait STATUS_HELP ;

template <class ARG1>
trait STATUS_HELP<ARG1> {
	class Status ;
} ;

template <class UNIT1>
class STATUS_HELP<UNIT1>::Status {
private:
	FLAG mStatus ;

public:
	implicit Status () = delete ;

	template <class ARG1>
	explicit Status (CREF<ARG1> id) {
		using R1X = typeof (id) ;
		mStatus = ENUM_CHECK<R1X>::value ;
	}

	template <class ARG1>
	REMOVE_REF<ARG1> cast (CREF<ARG1> id) const {
		using R1X = typeof (id) ;
		return R1X (mStatus) ;
	}

	inline implicit operator FLAG () const {
		return cast (typeas<FLAG>::id) ;
	}

	BOOL equal (CREF<Status> that) const {
		return mStatus == that.mStatus ;
	}

	inline BOOL operator== (CREF<Status> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Status> that) const {
		return equal (that) ;
	}

	FLAG compr (CREF<Status> that) const {
		return operator_compr (mStatus ,that.mStatus) ;
	}

	inline BOOL operator< (CREF<Status> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Status> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Status> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Status> that) const {
		return compr (that) >= ZERO ;
	}

	FLAG hash () const {
		return mStatus ;
	}
} ;
} ;

namespace U {
template <class...>
trait TUPLE_HELP ;

template <class ARG1>
trait TUPLE_HELP<ARG1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_EQ_ZERO<COUNTOF<ARG1>>) ;

	class Tuple ;
} ;

template <class UNIT1>
class TUPLE_HELP<UNIT1 ,REQUIRE<ENUM_EQ_ZERO<COUNTOF<UNIT1>>>>::Tuple {
public:
	implicit Tuple () = default ;

	LENGTH rank () const {
		return COUNTOF<UNIT1>::value ;
	}

	BOOL equal (CREF<Tuple> that) const {
		return TRUE ;
	}

	inline BOOL operator== (CREF<Tuple> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Tuple> that) const {
		return nnot (equal (that)) ;
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

template <class ARG1>
trait TUPLE_HELP<ARG1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<ARG1>>>> {
	require (ENUM_GT_ZERO<COUNTOF<ARG1>>) ;

	using ALL = TYPE_CHECK<ARG1> ;
	using ONE = TYPE_FIRST_ONE<ALL> ;
	using REST = TYPE_FIRST_REST<ALL> ;
	using BASE = typename TUPLE_HELP<REST ,void>::Tuple ;

	class Tuple ;
} ;

template <class UNIT1>
class TUPLE_HELP<UNIT1 ,REQUIRE<ENUM_GT_ZERO<COUNTOF<UNIT1>>>>::Tuple :
	public BASE {
private:
	ONE mValue ;

public:
	implicit Tuple () = default ;

	template <class ARG1 ,class...ARGS ,class = ENABLE<IS_SAME<ONE ,REMOVE_REF<ARG1>>>>
	explicit Tuple (RREF<ARG1> one_ ,RREF<ARGS>...rest_) :
		delegate BASE (forward (rest_)...) ,
		delegate mValue (forward (one_)) {}

	LENGTH rank () const {
		return COUNTOF<ALL>::value ;
	}

	VREF<ONE> one () leftvalue {
		return mValue ;
	}

	CREF<ONE> one () const leftvalue {
		return mValue ;
	}

	VREF<BASE> rest () leftvalue {
		return m_super () ;
	}

	CREF<BASE> rest () const leftvalue {
		return m_super () ;
	}

	template <class ARG1>
	VREF<TYPE_PICK<UNIT1 ,REMOVE_REF<ARG1>>> pick (CREF<ARG1> nth) leftvalue {
		using R1X = typeof (nth) ;
		require (ENUM_COMPR_GT_EQ<R1X ,ENUM_ZERO>) ;
		require (ENUM_COMPR_LT<R1X ,COUNTOF<ALL>>) ;
		return template_pick (typeas<R1X>::id ,PHX) ;
	}

	template <class ARG1>
	CREF<TYPE_PICK<UNIT1 ,REMOVE_REF<ARG1>>> pick (CREF<ARG1> nth) const leftvalue {
		using R1X = typeof (nth) ;
		require (ENUM_COMPR_GT_EQ<R1X ,ENUM_ZERO>) ;
		require (ENUM_COMPR_LT<R1X ,COUNTOF<ALL>>) ;
		return template_pick (typeas<R1X>::id ,PHX) ;
	}

	BOOL equal (CREF<Tuple> that) const {
		if (mValue != that.mValue)
			return FALSE ;
		if (m_super () != that.m_super ())
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (CREF<Tuple> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Tuple> that) const {
		return nnot (equal (that)) ;
	}

	FLAG compr (CREF<Tuple> that) const {
		const auto r1x = operator_compr (mValue ,that.mValue) ;
		if (r1x != ZERO)
			return r1x ;
		const auto r2x = operator_compr (m_super () ,that.m_super ()) ;
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
		const auto r1x = operator_hash (mValue) ;
		const auto r2x = operator_hash (m_super ()) ;
		return hashcode (r1x ,r2x) ;
	}

private:
	VREF<BASE> m_super () leftvalue {
		auto &&thiz = property[this] ;
		return static_cast<VREF<BASE>> (thiz) ;
	}

	CREF<BASE> m_super () const leftvalue {
		auto &&thiz = property[this] ;
		return static_cast<CREF<BASE>> (thiz) ;
	}

	template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_REF<ARG1>>>>
	VREF<ONE> template_pick (CREF<ARG1> nth ,CREF<typeof (PH1)>) leftvalue {
		return one () ;
	}

	template <class ARG1 ,class = ENABLE<ENUM_EQ_ZERO<REMOVE_REF<ARG1>>>>
	CREF<ONE> template_pick (CREF<ARG1> nth ,CREF<typeof (PH1)>) const leftvalue {
		return one () ;
	}

	template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_REF<ARG1>>>>
	VREF<TYPE_PICK<UNIT1 ,REMOVE_REF<ARG1>>> template_pick (CREF<ARG1> nth ,CREF<typeof (PH2)>) leftvalue {
		using R1X = typeof (nth) ;
		using R2X = ENUM_DEC<REMOVE_REF<R1X>> ;
		const auto r1x = property (rest ()) ;
		return property[r1x].template_pick (typeas<R2X>::id ,PHX) ;
	}

	template <class ARG1 ,class = ENABLE<ENUM_GT_ZERO<REMOVE_REF<ARG1>>>>
	CREF<TYPE_PICK<UNIT1 ,REMOVE_REF<ARG1>>> template_pick (CREF<ARG1> nth ,CREF<typeof (PH2)>) const leftvalue {
		using R1X = typeof (nth) ;
		using R2X = ENUM_DEC<REMOVE_REF<R1X>> ;
		const auto r1x = property (rest ()) ;
		return property[r1x].template_pick (typeas<R2X>::id ,PHX) ;
	}
} ;
} ;

template <class...UNITS>
using Tuple = typename U::TUPLE_HELP<typeas<UNITS...> ,void>::Tuple ;
} ;