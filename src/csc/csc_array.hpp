#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
class SizeProxy {
protected:
	LENGTH mSize ;

public:
	implicit SizeProxy () = delete ;

	implicit SizeProxy (CREF<LENGTH> size_) {
		mSize = size_ ;
	}

	forceinline operator LENGTH () const {
		return mSize ;
	}
} ;

template <class A ,class B>
class SetProxy {
protected:
	A mThat ;
	INDEX mIndex ;

public:
	implicit SetProxy () = delete ;

	explicit SetProxy (XREF<A> that ,CREF<INDEX> index) :mThat (that) {
		mIndex = index ;
	}

	forceinline operator B () rightvalue {
		B ret ;
		mThat->get (mIndex ,ret) ;
		return move (ret) ;
	}

	forceinline void operator= (CREF<B> that) rightvalue {
		mThat->set (mIndex ,that) ;
	}
} ;

template <class A>
class ArrayIterator {
private:
	using ITEM = REF<typeof (A (NULL)->at (0)) ,POINTER_CVR<A>> ;

protected:
	A mThat ;
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (XREF<A> that) :mThat (that) {
		mBegin = mThat->ibegin () ;
		mEnd = mThat->iend () ;
		mPeek = mBegin ;
	}

	LENGTH size () const {
		return mThat->length () ;
	}

	ArrayIterator begin () const {
		return thiz ;
	}

	ArrayIterator end () const {
		return thiz ;
	}

	forceinline BOOL operator== (CREF<ArrayIterator>) const {
		return mPeek == mEnd ;
	}

	forceinline BOOL operator!= (CREF<ArrayIterator>) const {
		return mPeek != mEnd ;
	}

	forceinline XREF<ITEM> operator* () const leftvalue {
		return mThat->at (mPeek) ;
	}

	forceinline void operator++ () {
		mPeek = mThat->inext (mPeek) ;
	}
} ;

template <class A>
class ArrayRange implement Proxy {
protected:
	A mThat ;

public:
	imports CREF<ArrayRange> from (CREF<A> that) {
		return Pointer::from (that) ;
	}

	imports CREF<ArrayRange> from (RREF<A> that) = delete ;

	CREF<INDEX> at (CREF<INDEX> index) const leftvalue {
		return index ;
	}

	INDEX ibegin () const {
		return mThat.ibegin () ;
	}

	INDEX iend () const {
		return mThat.iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return mThat.inext (index) ;
	}

	ArrayIterator<CPTR<ArrayRange>> begin () const leftvalue {
		return ArrayIterator<CPTR<ArrayRange>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<ArrayRange>> end () const leftvalue {
		return ArrayIterator<CPTR<ArrayRange>> ((&thiz)) ;
	}
} ;

struct ArrayLayout {
	RefBuffer<Pointer> mArray ;
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> create (VREF<ArrayLayout> that) ;
	imports CFat<ArrayHolder> create (CREF<ArrayLayout> that) ;

	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<ArrayLayout> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<ArrayLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ArrayLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<Pointer> item) = 0 ;
	virtual ArrayLayout segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<ArrayLayout> item) = 0 ;
} ;

template <class A>
struct ArrayUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_code (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		if (uuid == ReflectCloneBinder<A>::expr)
			return inline_code (ReflectCloneBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_code (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_code (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_code (ReflectVisitBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Array implement ArrayLayout {
protected:
	using ArrayLayout::mArray as (RefBuffer<A>) ;

public:
	implicit Array () = default ;

	explicit Array (CREF<SizeProxy> size_) {
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
	}

	explicit Array (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	template <class ARG1>
	imports Array make (XREF<ARG1> initval) {
		Array ret = Array (initval.size ()) ;
		INDEX ix = 0 ;
		for (auto &&i : initval) {
			ret[ix] = i ;
			ix++ ;
		}
		return move (ret) ;
	}

	implicit Array (CREF<Array> that) {
		ArrayHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Array> operator= (CREF<Array> that) {
		return assign (thiz ,that) ;
	}

	implicit Array (RREF<Array> that) = default ;

	forceinline VREF<Array> operator= (RREF<Array> that) = default ;

	LENGTH size () const {
		return ArrayHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return ArrayHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return ArrayHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ArrayHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ArrayHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ArrayHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ArrayHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Array>> begin () const leftvalue {
		return ArrayIterator<CPTR<Array>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<Array>> end () const leftvalue {
		return ArrayIterator<CPTR<Array>> ((&thiz)) ;
	}

	CREF<ArrayRange<Array>> range () const leftvalue {
		return ArrayRange<Array>::from (thiz) ;
	}

	BOOL equal (CREF<Array> that) const {
		return ArrayHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Array> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Array> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Array> that) const {
		return ArrayHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Array> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Array> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Array> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Array> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return ArrayHolder::create (thiz)->visit (visitor) ;
	}

	void fill (CREF<A> item) {
		return ArrayHolder::create (thiz)->fill (Pointer::from (item)) ;
	}

	Array segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		ArrayLayout ret = ArrayHolder::create (thiz)->segment (begin_ ,end_) ;
		return move (keep[TYPE<Array>::expr] (ret)) ;
	}

	void splice (CREF<INDEX> index ,CREF<Array> item) {
		return ArrayHolder::create (thiz)->splice (index ,item) ;
	}
} ;

struct StringLayout {
	RefBuffer<Pointer> mString ;
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> create (VREF<StringLayout> that) ;
	imports CFat<StringHolder> create (CREF<StringLayout> that) ;

	virtual void initialize (CREF<Slice<STRA>> that) = 0 ;
	virtual void initialize (CREF<Slice<STRW>> that) = 0 ;
	virtual void initialize (CREF<Slice<STRU8>> that) = 0 ;
	virtual void initialize (CREF<Slice<STRU16>> that) = 0 ;
	virtual void initialize (CREF<Slice<STRU32>> that) = 0 ;
	virtual void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<StringLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<ARR<STRA>> raw (TYPEID<STRA>) leftvalue = 0 ;
	virtual CREF<ARR<STRA>> raw (TYPEID<STRA>) const leftvalue = 0 ;
	virtual VREF<ARR<STRW>> raw (TYPEID<STRW>) leftvalue = 0 ;
	virtual CREF<ARR<STRW>> raw (TYPEID<STRW>) const leftvalue = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<STRU32> item) = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<StringLayout> that) const = 0 ;
	virtual FLAG compr (CREF<StringLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<STRU32> item) = 0 ;
	virtual void trunc (CREF<INDEX> index) = 0 ;
	virtual StringLayout segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<StringLayout> item) = 0 ;
} ;

class String implement StringLayout {
protected:
	using StringLayout::mString as (RefBuffer<BYTE>) ;

public:
	implicit String () = default ;

	implicit String (CREF<Slice<STRA>> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	implicit String (CREF<Slice<STRW>> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	implicit String (CREF<Slice<STRU8>> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	implicit String (CREF<Slice<STRU16>> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	implicit String (CREF<Slice<STRU32>> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	explicit String (CREF<SizeProxy> size_ ,CREF<SizeProxy> step_) {
		StringHolder::create (thiz)->initialize (size_ ,step_) ;
	}

	template <class...ARG1>
	imports String make (XREF<ARG1>...initval) {
		String ret ;
		unimplemented () ;
		return move (ret) ;
	}

	imports CREF<String> error () {
		return memorize ([&] () {
			return String (0 ,SIZE_OF<STR>::expr) ;
		}) ;
	}

	implicit String (CREF<String> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<String> operator= (CREF<String> that) {
		return assign (thiz ,that) ;
	}

	implicit String (RREF<String> that) = default ;

	forceinline VREF<String> operator= (RREF<String> that) = default ;

	void clear () {
		return StringHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return StringHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return StringHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return StringHolder::create (thiz)->length () ;
	}

	VREF<ARR<STR>> raw () leftvalue {
		return StringHolder::create (thiz)->raw (TYPE<STR>::expr) ;
	}

	CREF<ARR<STR>> raw () const leftvalue {
		return StringHolder::create (thiz)->raw (TYPE<STR>::expr) ;
	}

	VREF<ARR<STRA>> raw (TYPEID<STRA>) leftvalue {
		return StringHolder::create (thiz)->raw (TYPE<STRA>::expr) ;
	}

	CREF<ARR<STRA>> raw (TYPEID<STRA>) const leftvalue {
		return StringHolder::create (thiz)->raw (TYPE<STRA>::expr) ;
	}

	VREF<ARR<STRW>> raw (TYPEID<STRW>) leftvalue {
		return StringHolder::create (thiz)->raw (TYPE<STRW>::expr) ;
	}

	CREF<ARR<STRW>> raw (TYPEID<STRW>) const leftvalue {
		return StringHolder::create (thiz)->raw (TYPE<STRW>::expr) ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		return StringHolder::create (thiz)->get (index ,item) ;
	}

	void set (CREF<INDEX> index ,CREF<STRU32> item) {
		return StringHolder::create (thiz)->set (index ,item) ;
	}

	forceinline SetProxy<VPTR<String> ,STRU32> operator[] (CREF<INDEX> index) leftvalue {
		return SetProxy<VPTR<String> ,STRU32> ((&thiz) ,index) ;
	}

	forceinline SetProxy<CPTR<String> ,STRU32> operator[] (CREF<INDEX> index) const leftvalue {
		return SetProxy<CPTR<String> ,STRU32> ((&thiz) ,index) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue {
		return StringHolder::create (thiz)->at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue {
		return StringHolder::create (thiz)->at (index) ;
	}

	INDEX ibegin () const {
		return StringHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return StringHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return StringHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<String>> begin () const leftvalue {
		return ArrayIterator<CPTR<String>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<String>> end () const leftvalue {
		return ArrayIterator<CPTR<String>> ((&thiz)) ;
	}

	CREF<ArrayRange<String>> range () const leftvalue {
		return ArrayRange<String>::from (thiz) ;
	}

	BOOL equal (CREF<String> that) const {
		return StringHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<String> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<String> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<String> that) const {
		return StringHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<String> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<String> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<String> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<String> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return StringHolder::create (thiz)->visit (visitor) ;
	}

	void fill (CREF<STRU32> item) {
		return StringHolder::create (thiz)->fill (item) ;
	}

	void trunc (CREF<INDEX> index) {
		return StringHolder::create (thiz)->trunc (index) ;
	}

	String segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		StringLayout ret = StringHolder::create (thiz)->segment (begin_ ,end_) ;
		return move (keep[TYPE<String>::expr] (ret)) ;
	}

	void splice (CREF<INDEX> index ,CREF<String> item) {
		return StringHolder::create (thiz)->splice (index ,item) ;
	}
} ;

struct DequeLayout {
	RefBuffer<Pointer> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit DequeLayout () noexcept {
		mRead = 0 ;
		mWrite = 0 ;
	}
} ;

struct DequeHolder implement Interface {
	imports VFat<DequeHolder> create (VREF<DequeLayout> that) ;
	imports CFat<DequeHolder> create (CREF<DequeLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual INDEX tail () const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RREF<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
} ;

template <class A>
struct DequeUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_code (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Deque implement DequeLayout {
protected:
	using DequeLayout::mDeque as (RefBuffer<A>) ;
	using DequeLayout::mRead ;
	using DequeLayout::mWrite ;

public:
	implicit Deque () = default ;

	explicit Deque (CREF<SizeProxy> size_) {
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> () ,size_) ;
	}

	explicit Deque (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return DequeHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return DequeHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return DequeHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return DequeHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return DequeHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return DequeHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return DequeHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return DequeHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return DequeHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Deque>> begin () const leftvalue {
		return ArrayIterator<CPTR<Deque>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<Deque>> end () const leftvalue {
		return ArrayIterator<CPTR<Deque>> ((&thiz)) ;
	}

	CREF<ArrayRange<Deque>> range () const leftvalue {
		return ArrayRange<Deque>::from (thiz) ;
	}

	BOOL empty () const {
		return DequeHolder::create (thiz)->empty () ;
	}

	INDEX head () const {
		return DequeHolder::create (thiz)->head () ;
	}

	INDEX tail () const {
		return DequeHolder::create (thiz)->tail () ;
	}

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> ()) ;
		return DequeHolder::create (thiz)->add (move (rax)) ;
	}

	void take () {
		return DequeHolder::create (thiz)->take () ;
	}

	void push (CREF<A> item) {
		move (move (item)) ;
	}

	void push (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> ()) ;
		return DequeHolder::create (thiz)->push (move (rax)) ;
	}

	void pop () {
		return DequeHolder::create (thiz)->pop () ;
	}
} ;

struct PriorityNode {
	INDEX mMap ;
} ;

struct PriorityLayout {
	RefBuffer<Pointer> mPriority ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit PriorityLayout () noexcept {
		mRead = 0 ;
		mWrite = 0 ;
	}
} ;

struct PriorityHolder implement Interface {
	imports VFat<PriorityHolder> create (VREF<PriorityLayout> that) ;
	imports CFat<PriorityHolder> create (CREF<PriorityLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<INDEX> map_) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<INDEX> map_) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual void add (RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual void take () = 0 ;
} ;

template <class A>
struct PriorityUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = PAIR<A ,PriorityNode> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectPairBinder<R1X>::expr)
			return inline_code (ReflectPairBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_code (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_code (ReflectComprBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Priority implement PriorityLayout {
protected:
	using PriorityLayout::mPriority as (RefBuffer<PAIR<A ,PriorityNode>>) ;
	using PriorityLayout::mRead ;
	using PriorityLayout::mWrite ;

public:
	implicit Priority () = default ;

	explicit Priority (CREF<SizeProxy> size_) {
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> () ,size_) ;
	}

	explicit Priority (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return PriorityHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return PriorityHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return PriorityHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return PriorityHolder::create (thiz)->length () ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return PriorityHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const {
		return PriorityHolder::create (thiz)->get (index ,map_) ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) {
		return PriorityHolder::create (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return PriorityHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return PriorityHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return PriorityHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Priority>> beign () const leftvalue {
		return ArrayIterator<CPTR<Priority>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<Priority>> end () const leftvalue {
		return ArrayIterator<CPTR<Priority>> ((&thiz)) ;
	}

	CREF<ArrayRange<Priority>> range () const leftvalue {
		return ArrayRange<Priority>::from (thiz) ;
	}

	BOOL empty () const {
		return PriorityHolder::create (thiz)->empty () ;
	}

	INDEX head () const {
		return PriorityHolder::create (thiz)->head () ;
	}

	void add (CREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CREF<A> item ,CREF<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RREF<A> item ,CREF<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> ()) ;
		return PriorityHolder::create (thiz)->add (move (rax) ,map_) ;
	}

	void take () {
		return PriorityHolder::create (thiz)->take () ;
	}
} ;

struct ListNode implement AllocatorNode {
	INDEX mLeft ;
	INDEX mRight ;
} ;

struct ListLayout {
	Allocator<Pointer ,ListNode> mList ;
	INDEX mFirst ;
	INDEX mLast ;

public:
	implicit ListLayout () noexcept {
		mFirst = NONE ;
		mLast = NONE ;
	}
} ;

struct ListHolder implement Interface {
	imports VFat<ListHolder> create (VREF<ListLayout> that) ;
	imports CFat<ListHolder> create (CREF<ListLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual INDEX tail () const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RREF<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual INDEX insert () = 0 ;
	virtual INDEX insert (CREF<INDEX> index) = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
} ;

template <class A>
struct ListUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = PAIR<TEMP<A> ,ListNode> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectPairBinder<R1X>::expr)
			return inline_code (ReflectPairBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class List implement ListLayout {
protected:
	using ListLayout::mList as (Allocator<A ,ListNode>) ;
	using ListLayout::mFirst ;
	using ListLayout::mLast ;

public:
	implicit List () = default ;

	explicit List (CREF<SizeProxy> size_) {
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> () ,size_) ;
	}

	explicit List (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return ListHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return ListHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return ListHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return ListHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ListHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ListHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ListHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ListHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ListHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<List>> begin () const leftvalue {
		return ArrayIterator<CPTR<List>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<List>> end () const leftvalue {
		return ArrayIterator<CPTR<List>> ((&thiz)) ;
	}

	CREF<ArrayRange<List>> range () const leftvalue {
		return ArrayRange<List>::from (thiz) ;
	}

	BOOL empty () const {
		return ListHolder::create (thiz)->empty () ;
	}

	INDEX head () const {
		return ListHolder::create (thiz)->head () ;
	}

	INDEX tail () const {
		return ListHolder::create (thiz)->tail () ;
	}

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> ()) ;
		return ListHolder::create (thiz)->add (move (rax)) ;
	}

	void take () {
		return ListHolder::create (thiz)->take () ;
	}

	void push (CREF<A> item) {
		move (move (item)) ;
	}

	void push (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> ()) ;
		return ListHolder::create (thiz)->push (move (rax)) ;
	}

	void pop () {
		return ListHolder::create (thiz)->pop () ;
	}

	INDEX insert () {
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> ()) ;
		return ListHolder::create (thiz)->insert () ;
	}

	INDEX insert (CREF<INDEX> index) {
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> ()) ;
		return ListHolder::create (thiz)->insert (index) ;
	}

	void remove (CREF<INDEX> index) {
		return ListHolder::create (thiz)->remove (index) ;
	}
} ;

struct ArrayListNode implement AllocatorNode {} ;

struct ArrayListLayout {
	Allocator<Pointer ,ArrayListNode> mList ;
	RefBuffer<INDEX> mRange ;
	INDEX mTop ;

public:
	implicit ArrayListLayout () noexcept {
		mTop = 0 ;
	}
} ;

struct ArrayListHolder implement Interface {
	imports VFat<ArrayListHolder> create (VREF<ArrayListLayout> that) ;
	imports CFat<ArrayListHolder> create (CREF<ArrayListLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual INDEX insert () = 0 ;
	virtual INDEX insert (CREF<INDEX> index) = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
struct ArrayListUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = PAIR<TEMP<A> ,ArrayListNode> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectPairBinder<R1X>::expr)
			return inline_code (ReflectPairBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class ArrayList implement ArrayListLayout {
protected:
	using ArrayListLayout::mList as (Allocator<A ,ArrayListNode>) ;
	using ArrayListLayout::mRange ;
	using ArrayListLayout::mTop ;

public:
	implicit ArrayList () = default ;

	explicit ArrayList (CREF<SizeProxy> size_) {
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> () ,size_) ;
	}

	explicit ArrayList (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return ArrayListHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return ArrayListHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return ArrayListHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return ArrayListHolder::create (thiz)->length () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ArrayListHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ArrayListHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ArrayListHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayListHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ArrayListHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<ArrayList>> begin () const leftvalue {
		return ArrayIterator<CPTR<ArrayList>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<ArrayList>> end () const leftvalue {
		return ArrayIterator<CPTR<ArrayList>> ((&thiz)) ;
	}

	CREF<ArrayRange<ArrayList>> range () const leftvalue {
		return ArrayRange<ArrayList>::from (thiz) ;
	}

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::create (thiz)->add (move (rax)) ;
	}

	INDEX insert () {
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::create (thiz)->insert () ;
	}

	INDEX insert (CREF<INDEX> index) {
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::create (thiz)->insert (index) ;
	}

	void remove (CREF<INDEX> index) {
		return ArrayListHolder::create (thiz)->remove (index) ;
	}

	void remap () {
		return ArrayListHolder::create (thiz)->remap () ;
	}
} ;

struct SortedListNode {
	INDEX mMap ;
} ;

struct SortedListLayout {
	RefBuffer<Pointer> mList ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit SortedListLayout () noexcept {
		mRead = 0 ;
		mWrite = 0 ;
	}
} ;

struct SortedListHolder implement Interface {
	imports VFat<SortedListHolder> create (VREF<SortedListLayout> that) ;
	imports CFat<SortedListHolder> create (CREF<SortedListLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<INDEX> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<INDEX> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual INDEX find (CREF<Pointer> item) const = 0 ;
	virtual BOOL contain (CREF<Pointer> item) const = 0 ;
	virtual INDEX map (CREF<Pointer> item) const = 0 ;
} ;

template <class A>
struct SortedListUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = PAIR<A ,SortedListNode> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectPairBinder<R1X>::expr)
			return inline_code (ReflectPairBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_code (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_code (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_code (ReflectVisitBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class SortedList implement SortedListLayout {
protected:
	using SortedListLayout::mList as (RefBuffer<PAIR<A ,SortedListNode>>) ;
	using SortedListLayout::mRead ;
	using SortedListLayout::mWrite ;

public:
	implicit SortedList () = default ;

	explicit SortedList (CREF<SizeProxy> size_) {
		SortedListHolder::create (thiz)->initialize (SortedListUnknownBinder<A> () ,size_) ;
	}

	void clear () {
		return SortedListHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return SortedListHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return SortedListHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return SortedListHolder::create (thiz)->length () ;
	}

	VREF<INDEX> at (CREF<INDEX> index) leftvalue {
		return SortedListHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<INDEX> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<INDEX> at (CREF<INDEX> index) const leftvalue {
		return SortedListHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<INDEX> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return SortedListHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SortedListHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return SortedListHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<SortedList>> begin () const leftvalue {
		return ArrayIterator<CPTR<SortedList>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<SortedList>> end () const leftvalue {
		return ArrayIterator<CPTR<SortedList>> ((&thiz)) ;
	}

	CREF<ArrayRange<SortedList>> range () const leftvalue {
		return ArrayRange<SortedList>::from (thiz) ;
	}

	void add (CREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CREF<A> item ,CREF<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RREF<A> item ,CREF<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		SortedListHolder::create (thiz)->initialize (SortedListUnknownBinder<A> ()) ;
		return SortedListHolder::create (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return SortedListHolder::create (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return SortedListHolder::create (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
		return SortedListHolder::create (thiz)->map (Pointer::from (item)) ;
	}
} ;

struct SetNode implement AllocatorNode {
	INDEX mMap ;
	BOOL mRed ;
	BOOL mBin ;
	INDEX mUp ;
	INDEX mLeft ;
	INDEX mRight ;
} ;

struct SetLayout {
	Allocator<Pointer ,SetNode> mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	implicit SetLayout () noexcept {
		mRoot = NONE ;
		mTop = 0 ;
	}
} ;

struct SetHolder implement Interface {
	imports VFat<SetHolder> create (VREF<SetLayout> that) ;
	imports CFat<SetHolder> create (CREF<SetLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<INDEX> map_) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<INDEX> map_) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual INDEX find (CREF<Pointer> item) const = 0 ;
	virtual BOOL contain (CREF<Pointer> item) const = 0 ;
	virtual INDEX map (CREF<Pointer> item) const = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void erase (CREF<Pointer> item) = 0 ;
} ;

template <class A>
struct SetUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = PAIR<TEMP<A> ,SetNode> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectPairBinder<R1X>::expr)
			return inline_code (ReflectPairBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_code (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_code (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_code (ReflectVisitBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Set implement SetLayout {
protected:
	using SetLayout::mSet as (Allocator<A ,SetNode>) ;
	using SetLayout::mRoot ;
	using SetLayout::mTop ;

public:
	implicit Set () = default ;

	explicit Set (CREF<SizeProxy> size_) {
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> () ,size_) ;
	}

	explicit Set (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return SetHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return SetHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return SetHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return SetHolder::create (thiz)->length () ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return SetHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const {
		return SetHolder::create (thiz)->get (index ,map_) ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) {
		return SetHolder::create (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return SetHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SetHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return SetHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Set>> begin () const leftvalue {
		return ArrayIterator<CPTR<Set>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<Set>> end () const leftvalue {
		return ArrayIterator<CPTR<Set>> ((&thiz)) ;
	}

	CREF<ArrayRange<Set>> range () const leftvalue {
		return ArrayRange<Set>::from (thiz) ;
	}

	void add (CREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CREF<A> item ,CREF<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RREF<A> item ,CREF<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> ()) ;
		return SetHolder::create (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return SetHolder::create (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return SetHolder::create (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
		return SetHolder::create (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CREF<INDEX> index) {
		return SetHolder::create (thiz)->remove (index) ;
	}

	void erase (CREF<A> item) {
		return SetHolder::create (thiz)->erase (Pointer::from (item)) ;
	}
} ;

struct HashSetNode implement AllocatorNode {
	INDEX mMap ;
	FLAG mHash ;
	INDEX mDown ;
} ;

struct HashSetLayout {
	Allocator<Pointer ,HashSetNode> mSet ;
	RefBuffer<INDEX> mRange ;
	SharedRef<Visitor> mVisitor ;
} ;

struct HashSetHolder implement Interface {
	imports VFat<HashSetHolder> create (VREF<HashSetLayout> that) ;
	imports CFat<HashSetHolder> create (CREF<HashSetLayout> that) ;

	virtual void initialize (CREF<Unknown> element) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<INDEX> map_) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<INDEX> map_) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual INDEX find (CREF<Pointer> item) const = 0 ;
	virtual BOOL contain (CREF<Pointer> item) const = 0 ;
	virtual INDEX map (CREF<Pointer> item) const = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void erase (CREF<Pointer> item) = 0 ;
} ;

template <class A>
struct HashSetUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = PAIR<TEMP<A> ,HashSetNode> ;
		if (uuid == ReflectSizeBinder<R1X>::expr)
			return inline_code (ReflectSizeBinder<R1X> ()) ;
		if (uuid == ReflectCreateBinder<R1X>::expr)
			return inline_code (ReflectCreateBinder<R1X> ()) ;
		if (uuid == ReflectDestroyBinder<R1X>::expr)
			return inline_code (ReflectDestroyBinder<R1X> ()) ;
		if (uuid == ReflectPairBinder<R1X>::expr)
			return inline_code (ReflectPairBinder<R1X> ()) ;
		if (uuid == ReflectMoveBinder<A>::expr)
			return inline_code (ReflectMoveBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_code (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_code (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_code (ReflectVisitBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class HashSet implement HashSetLayout {
protected:
	using HashSetLayout::mSet as (Allocator<A ,HashSetNode>) ;
	using HashSetLayout::mRange ;
	using HashSetLayout::mVisitor ;

public:
	implicit HashSet () = default ;

	explicit HashSet (CREF<SizeProxy> size_) {
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> () ,size_) ;
	}

	explicit HashSet (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return HashSetHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return HashSetHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return HashSetHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return HashSetHolder::create (thiz)->length () ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return HashSetHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const {
		return HashSetHolder::create (thiz)->get (index ,map_) ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) {
		return HashSetHolder::create (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return HashSetHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return HashSetHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return HashSetHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<HashSet>> begin () const leftvalue {
		return ArrayIterator<CPTR<HashSet>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<HashSet>> end () const leftvalue {
		return ArrayIterator<CPTR<HashSet>> ((&thiz)) ;
	}

	CREF<ArrayRange<HashSet>> range () const leftvalue {
		return ArrayRange<HashSet>::from (thiz) ;
	}

	void add (CREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RREF<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CREF<A> item ,CREF<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RREF<A> item ,CREF<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> ()) ;
		return HashSetHolder::create (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return HashSetHolder::create (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return HashSetHolder::create (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
		return HashSetHolder::create (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CREF<INDEX> index) {
		return HashSetHolder::create (thiz)->remove (index) ;
	}

	void erase (CREF<A> item) {
		return SetHolder::create (thiz)->erase (Pointer::from (item)) ;
	}
} ;

struct BitSetLayout {
	RefBuffer<BYTE> mSet ;
	LENGTH mWidth ;
} ;

struct BitSetHolder implement Interface {
	imports VFat<BitSetHolder> create (VREF<BitSetLayout> that) ;
	imports CFat<BitSetHolder> create (CREF<BitSetLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<BOOL> item) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<BOOL> item) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<BitSetLayout> that) const = 0 ;
	virtual FLAG compr (CREF<BitSetLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual BOOL contain (CREF<Pointer> item) const = 0 ;
	virtual void erase (CREF<Pointer> item) = 0 ;
	virtual void fill (CREF<BYTE> item) = 0 ;
	virtual BitSetLayout band (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout bor (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout bxor (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout bsub (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout bnot () const = 0 ;
} ;

class BitSet implement BitSetLayout {
private:
	using A = INDEX ;

protected:
	using BitSetLayout::mSet ;
	using BitSetLayout::mWidth ;

public:
	implicit BitSet () = default ;

	explicit BitSet (CREF<SizeProxy> size_) {
		BitSetHolder::create (thiz)->initialize (size_) ;
	}

	explicit BitSet (CREF<csc_initializer_t<A>> that) {
		auto rax = Box<A> () ;
		BitSetHolder::create (thiz)->initialize (RefUnknownBinder<A> () ,Capture<CREF<csc_initializer_t<A>>> (that) ,rax) ;
	}

	void clear () {
		return BitSetHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return BitSetHolder::create (thiz)->size () ;
	}

	LENGTH length () const {
		return BitSetHolder::create (thiz)->length () ;
	}

	void get (CREF<INDEX> index ,VREF<BOOL> item) const {
		return BitSetHolder::create (thiz)->get (index ,item) ;
	}

	void set (CREF<INDEX> index ,CREF<BOOL> item) {
		return BitSetHolder::create (thiz)->set (index ,item) ;
	}

	forceinline SetProxy<VPTR<BitSet> ,BOOL> operator[] (CREF<INDEX> index) leftvalue {
		return SetProxy<VPTR<BitSet> ,BOOL> ((&thiz) ,index) ;
	}

	forceinline SetProxy<CPTR<BitSet> ,BOOL> operator[] (CREF<INDEX> index) const leftvalue {
		return SetProxy<CPTR<BitSet> ,BOOL> ((&thiz) ,index) ;
	}

	CREF<INDEX> at (CREF<INDEX> index) const leftvalue {
		return index ;
	}

	INDEX ibegin () const {
		return BitSetHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return BitSetHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return BitSetHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<BitSet>> begin () const leftvalue {
		return ArrayIterator<CPTR<BitSet>> ((&thiz)) ;
	}

	ArrayIterator<CPTR<BitSet>> end () const leftvalue {
		return ArrayIterator<CPTR<BitSet>> ((&thiz)) ;
	}

	CREF<ArrayRange<BitSet>> range () const leftvalue {
		return ArrayRange<BitSet>::from (thiz) ;
	}

	BOOL equal (CREF<BitSet> that) const {
		return BitSetHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<BitSet> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<BitSet> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<BitSet> that) const {
		return BitSetHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<BitSet> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<BitSet> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<BitSet> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<BitSet> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return BitSetHolder::create (thiz)->visit (visitor) ;
	}

	void add (CREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return BitSetHolder::create (thiz)->add (move (rax)) ;
	}

	BOOL contain (CREF<A> item) const {
		return BitSetHolder::create (thiz)->contain (Pointer::from (item)) ;
	}

	void erase (CREF<A> item) {
		return BitSetHolder::create (thiz)->erase (Pointer::from (item)) ;
	}

	void fill (CREF<BYTE> item) {
		return BitSetHolder::create (thiz)->fill (item) ;
	}

	BitSet band (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->band (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator& (CREF<BitSet> that) const {
		return band (that) ;
	}

	forceinline void operator&= (CREF<BitSet> that) {
		thiz = band (that) ;
	}

	BitSet bor (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->bor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator| (CREF<BitSet> that) const {
		return bor (that) ;
	}

	forceinline void operator|= (CREF<BitSet> that) {
		thiz = bor (that) ;
	}

	BitSet bxor (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->bxor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator^ (CREF<BitSet> that) const {
		return bxor (that) ;
	}

	forceinline void operator^= (CREF<BitSet> that) {
		thiz = bxor (that) ;
	}

	BitSet bsub (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->bsub (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator- (CREF<BitSet> that) const {
		return bsub (that) ;
	}

	forceinline void operator-= (CREF<BitSet> that) {
		thiz = bsub (that) ;
	}

	BitSet bnot () const {
		BitSetLayout ret = BitSetHolder::create (thiz)->bnot () ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator~ () const {
		return bnot () ;
	}
} ;
} ;