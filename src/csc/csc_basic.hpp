#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
template <class...>
trait TUPLE_HELP ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQ_ZERO<COUNT_OF<PARAMS>>>> {
	struct Tuple ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK1>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK2>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
		TYPE_M2ND_ONE<PARAMS> m2nd ;
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<COUNT_OF<PARAMS> ,RANK3>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
		TYPE_M2ND_ONE<PARAMS> m2nd ;
		TYPE_M3RD_ONE<PARAMS> m3rd ;
	} ;
} ;

template <class...A>
using Tuple = typename TUPLE_HELP<TYPE<A...> ,ALWAYS>::Tuple ;

class BoxBufferLayout {} ;

struct BoxBufferHolder implement Interface {
	imports VFat<BoxBufferHolder> create (VREF<BoxBufferLayout> that) ;
	imports CFat<BoxBufferHolder> create (CREF<BoxBufferLayout> that) ;

	virtual void initialize (CREF<Pointer> that) = 0 ;
	virtual void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

template <class A ,class B>
class BoxBuffer implement BoxBufferLayout {
protected:
	TEMP<ARR<A ,B>> mBuffer ;

public:
	implicit BoxBuffer () = default ;

	implicit BoxBuffer (CREF<ARR<A ,B>> that) {
		BoxBufferHolder::create (thiz)->initialize (Pointer::from (that)) ;
	}

	explicit BoxBuffer (CREF<LENGTH> size_) {
		auto rax = Box<A>::make () ;
		BoxBufferHolder::create (thiz)->initialize (rax ,size_) ;
		rax.release () ;
	}

	LENGTH size () const {
		return BoxBufferHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return BoxBufferHolder::create (thiz)->step () ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return BoxBufferHolder::create (thiz)->self ;
	}

	inline implicit operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return BoxBufferHolder::create (thiz)->self ;
	}

	inline implicit operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	inline BOOL operator== (CREF<BoxBuffer>) = delete ;

	inline BOOL operator!= (CREF<BoxBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return BoxBufferHolder::create (thiz)->at (index) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return BoxBufferHolder::create (thiz)->at (index) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return BoxBufferHolder::create (thiz)->resize (size_) ;
	}
} ;

class RefBufferLayout {
public:
	RefLayout mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;
} ;

struct RefBufferHolder implement Interface {
	imports VFat<RefBufferHolder> create (VREF<RefBufferLayout> that) ;
	imports CFat<RefBufferHolder> create (CREF<RefBufferLayout> that) ;

	virtual void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
} ;

template <class A>
class RefBuffer implement RefBufferLayout {
public:
	implicit RefBuffer () = default ;

	explicit RefBuffer (CREF<LENGTH> size_) {
		auto rax = Box<A>::make () ;
		RefBufferHolder::create (thiz)->initialize (rax ,size_) ;
		rax.release () ;
	}

	LENGTH size () const {
		return RefBufferHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return RefBufferHolder::create (thiz)->step () ;
	}

	VREF<ARR<A>> self_m () leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	inline implicit operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return RefBufferHolder::create (thiz)->self ;
	}

	inline implicit operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	inline BOOL operator== (CREF<RefBuffer>) = delete ;

	inline BOOL operator!= (CREF<RefBuffer>) = delete ;

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return RefBufferHolder::create (thiz)->at (index) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void resize (CREF<LENGTH> size_) {
		return RefBufferHolder::create (thiz)->resize (size_) ;
	}
} ;
} ;
