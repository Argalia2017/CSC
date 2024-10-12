#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"

namespace CSC {
class SizeProxy {
protected:
	LENGTH mSize ;

public:
	implicit SizeProxy () = delete ;

	implicit SizeProxy (CREF<LENGTH> size_) {
		mSize = size_ ;
	}

	LENGTH fetch () const {
		return mSize ;
	}

	forceinline operator LENGTH () const {
		return fetch () ;
	}
} ;

template <class A>
class ArrayIterator {
private:
	using ITEM = REF<typeof (nullof (A).at (0)) ,REFLECT_REF<A>> ;

protected:
	XREF<A> mThat ;
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (XREF<A> that) :mThat (that) {
		mBegin = mThat.ibegin () ;
		mEnd = mThat.iend () ;
		mPeek = mBegin ;
	}

	LENGTH length () const {
		return mThat.length () ;
	}

	ArrayIterator begin () const {
		return thiz ;
	}

	ArrayIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CREF<ArrayIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CREF<ArrayIterator>) const {
		return good () ;
	}

	XREF<ITEM> peek () const leftvalue {
		return mThat.at (mPeek) ;
	}

	forceinline XREF<ITEM> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek = mThat.inext (mPeek) ;
	}

	forceinline void operator++ () {
		next () ;
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

	LENGTH length () const {
		return mThat.length () ;
	}

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

	ArrayIterator<CREF<ArrayRange>> begin () const leftvalue {
		return ArrayIterator<CREF<ArrayRange>> (thiz) ;
	}

	ArrayIterator<CREF<ArrayRange>> end () const leftvalue {
		return ArrayIterator<CREF<ArrayRange>> (thiz) ;
	}
} ;

struct ArrayLayout {
	RefBuffer<Pointer> mArray ;
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> create (VREF<ArrayLayout> that) ;
	imports CFat<ArrayHolder> create (CREF<ArrayLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
	virtual void visit (VREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<Pointer> item) = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<ArrayLayout> item) = 0 ;
} ;

template <class A>
class ArrayUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectCloneBinder<A>::expr)
			return inline_hold (ReflectCloneBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_hold (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_hold (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class ArrayRealLayout implement ArrayLayout {
public:
	implicit ArrayRealLayout () noexcept {
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (thiz.mArray) ;
		rax = RefBuffer<A> () ;
	}
} ;

template <class A>
class Array implement ArrayRealLayout<A> {
protected:
	using ArrayRealLayout<A>::mArray ;

public:
	implicit Array () = default ;

	explicit Array (CREF<SizeProxy> size_) {
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
	}

	explicit Array (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	template <class ARG1>
	imports Array make (CREF<ARG1> iterator) {
		Array ret = Array (iterator.length ()) ;
		INDEX ix = 0 ;
		for (auto &&i : iterator) {
			ret[ix] = A (i) ;
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

	implicit Array (RREF<Array> that) noexcept {
		swap (thiz ,that) ;
	}

	forceinline VREF<Array> operator= (RREF<Array> that) noexcept {
		return assign (thiz ,that) ;
	}

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

	ArrayIterator<CREF<Array>> begin () const leftvalue {
		return ArrayIterator<CREF<Array>> (thiz) ;
	}

	ArrayIterator<CREF<Array>> end () const leftvalue {
		return ArrayIterator<CREF<Array>> (thiz) ;
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
		return (!equal (that)) ;
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

	void visit (VREF<Visitor> visitor) const {
		return ArrayHolder::create (thiz)->visit (visitor) ;
	}

	void fill (CREF<A> item) {
		return ArrayHolder::create (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CREF<INDEX> index ,CREF<Array> item) {
		return ArrayHolder::create (thiz)->splice (index ,item) ;
	}
} ;

template <class A>
class StringParse ;

template <class A>
class StringBuild ;

struct StringLayout {
	RefBuffer<Pointer> mString ;
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> create (VREF<StringLayout> that) ;
	imports CFat<StringHolder> create (CREF<StringLayout> that) ;

	virtual void initialize (CREF<Slice> that ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<StringLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual void set (CREF<INDEX> index ,CREF<STRU32> item) = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<Slice> that) const = 0 ;
	virtual BOOL equal (CREF<StringLayout> that) const = 0 ;
	virtual FLAG compr (CREF<Slice> that) const = 0 ;
	virtual FLAG compr (CREF<StringLayout> that) const = 0 ;
	virtual void visit (VREF<Visitor> visitor) const = 0 ;
	virtual void trunc (CREF<INDEX> index) = 0 ;
	virtual void fill (CREF<STRU32> item) = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<Slice> item) = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<StringLayout> item) = 0 ;
} ;

template <class A>
class StringRealLayout implement StringLayout {
public:
	implicit StringRealLayout () noexcept {
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (thiz.mString) ;
		rax = RefBuffer<A> () ;
	}
} ;

template <class A>
class String implement StringRealLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	using StringRealLayout<A>::mString ;

public:
	implicit String () = default ;

	implicit String (CREF<Slice> that) {
		StringHolder::create (thiz)->initialize (that ,SIZE_OF<A>::expr) ;
	}

	explicit String (CREF<SizeProxy> size_) {
		StringHolder::create (thiz)->initialize (size_ ,SIZE_OF<A>::expr) ;
	}

	template <class...ARG1>
	imports String make (CREF<ARG1>...params) {
		using R1X = KILL<StringBuild<A> ,TYPE<ARG1...>> ;
		return R1X::make (params...) ;
	}

	imports CREF<String> zero () {
		return memorize ([&] () {
			return String (slice ("\0")) ;
		}) ;
	}

	implicit String (CREF<String> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<String> operator= (CREF<String> that) {
		return assign (thiz ,that) ;
	}

	implicit String (RREF<String> that) noexcept {
		swap (thiz ,that) ;
	}

	forceinline VREF<String> operator= (RREF<String> that) noexcept {
		return assign (thiz ,that) ;
	}

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

	VREF<ARR<A>> self_m () leftvalue {
		return StringHolder::create (thiz)->self ;
	}

	forceinline operator VREF<ARR<A>> () leftvalue {
		return self ;
	}

	CREF<ARR<A>> self_m () const leftvalue {
		return StringHolder::create (thiz)->self ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return self ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue {
		return StringHolder::create (thiz)->borrow () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return StringHolder::create (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return StringHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
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

	ArrayIterator<CREF<String>> begin () const leftvalue {
		return ArrayIterator<CREF<String>> (thiz) ;
	}

	ArrayIterator<CREF<String>> end () const leftvalue {
		return ArrayIterator<CREF<String>> (thiz) ;
	}

	CREF<ArrayRange<String>> range () const leftvalue {
		return ArrayRange<String>::from (thiz) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return StringHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Slice> that) const {
		return (!equal (that)) ;
	}

	BOOL equal (CREF<String> that) const {
		return StringHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<String> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<String> that) const {
		return (!equal (that)) ;
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

	void visit (VREF<Visitor> visitor) const {
		return StringHolder::create (thiz)->visit (visitor) ;
	}

	void trunc (CREF<INDEX> index) {
		return StringHolder::create (thiz)->trunc (index) ;
	}

	void fill (CREF<STRU32> item) {
		return StringHolder::create (thiz)->fill (item) ;
	}

	void splice (CREF<INDEX> index ,CREF<Slice> item) {
		return StringHolder::create (thiz)->splice (index ,item) ;
	}

	void splice (CREF<INDEX> index ,CREF<String> item) {
		return StringHolder::create (thiz)->splice (index ,item) ;
	}
} ;

struct DequeLayout {
	RefBuffer<Pointer> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct DequeHolder implement Interface {
	imports VFat<DequeHolder> create (VREF<DequeLayout> that) ;
	imports CFat<DequeHolder> create (CREF<DequeLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
	virtual void ring (CREF<LENGTH> size_) = 0 ;
} ;

template <class A>
class DequeUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_hold (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class DequeRealLayout implement DequeLayout {
public:
	implicit DequeRealLayout () noexcept {
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (thiz.mDeque) ;
		rax = RefBuffer<A> () ;
	}
} ;

template <class A>
class Deque implement DequeRealLayout<A> {
protected:
	using DequeRealLayout<A>::mDeque ;
	using DequeRealLayout<A>::mRead ;
	using DequeRealLayout<A>::mWrite ;

public:
	implicit Deque () = default ;

	explicit Deque (CREF<SizeProxy> size_) {
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> () ,size_) ;
	}

	explicit Deque (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
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

	ArrayIterator<CREF<Deque>> begin () const leftvalue {
		return ArrayIterator<CREF<Deque>> (thiz) ;
	}

	ArrayIterator<CREF<Deque>> end () const leftvalue {
		return ArrayIterator<CREF<Deque>> (thiz) ;
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

	void ring (CREF<LENGTH> size_) {
		return DequeHolder::create (thiz)->ring (size_) ;
	}
} ;

struct PriorityNode {
	INDEX mMap ;
} ;

struct PriorityLayout {
	RefBuffer<Pointer> mPriority ;
	FLAG mOffset ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct PriorityHolder implement Interface {
	imports VFat<PriorityHolder> create (VREF<PriorityLayout> that) ;
	imports CFat<PriorityHolder> create (CREF<PriorityLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
class PriorityUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = Tuple<A ,PriorityNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_hold (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class PriorityRealLayout implement PriorityLayout {
public:
	implicit PriorityRealLayout () noexcept {
		using R1X = Tuple<A ,PriorityNode> ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (thiz.mPriority) ;
		rax = RefBuffer<R1X> () ;
	}
} ;

template <class A>
class Priority implement PriorityRealLayout<A> {
protected:
	using PriorityRealLayout<A>::mPriority ;
	using PriorityRealLayout<A>::mRead ;
	using PriorityRealLayout<A>::mWrite ;

public:
	implicit Priority () = default ;

	explicit Priority (CREF<SizeProxy> size_) {
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> () ,size_) ;
	}

	explicit Priority (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
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

	ArrayIterator<CREF<Priority>> beign () const leftvalue {
		return ArrayIterator<CREF<Priority>> (thiz) ;
	}

	ArrayIterator<CREF<Priority>> end () const leftvalue {
		return ArrayIterator<CREF<Priority>> (thiz) ;
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
} ;

struct ListHolder implement Interface {
	imports VFat<ListHolder> create (VREF<ListLayout> that) ;
	imports CFat<ListHolder> create (CREF<ListLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
	virtual INDEX insert (RREF<BoxLayout> item) = 0 ;
	virtual INDEX insert (CREF<INDEX> index ,RREF<BoxLayout> item) = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void order (CREF<Array<INDEX>> range_) = 0 ;
} ;

template <class A>
class ListUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = TupleNode<A ,ListNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_hold (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class ListRealLayout implement ListLayout {
public:
	implicit ListRealLayout () noexcept {
		auto &&rax = keep[TYPE<AllocatorLayout>::expr] (thiz.mList) ;
		rax = Allocator<A ,ListNode> () ;
	}
} ;

template <class A>
class List implement ListRealLayout<A> {
protected:
	using ListRealLayout<A>::mList ;
	using ListRealLayout<A>::mFirst ;
	using ListRealLayout<A>::mLast ;

public:
	implicit List () = default ;

	explicit List (CREF<SizeProxy> size_) {
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> () ,size_) ;
	}

	explicit List (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
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

	ArrayIterator<CREF<List>> begin () const leftvalue {
		return ArrayIterator<CREF<List>> (thiz) ;
	}

	ArrayIterator<CREF<List>> end () const leftvalue {
		return ArrayIterator<CREF<List>> (thiz) ;
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
		auto rax = Box<A>::make () ;
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> ()) ;
		return ListHolder::create (thiz)->insert (move (rax)) ;
	}

	INDEX insert (CREF<INDEX> index) {
		auto rax = Box<A>::make () ;
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> ()) ;
		return ListHolder::create (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CREF<INDEX> index) {
		return ListHolder::create (thiz)->remove (index) ;
	}

	void order (CREF<Array<INDEX>> range_) {
		return ListHolder::create (thiz)->order (range_) ;
	}
} ;

struct ArrayListNode implement AllocatorNode {} ;

struct ArrayListLayout {
	Allocator<Pointer ,ArrayListNode> mList ;
	RefBuffer<INDEX> mRange ;
	INDEX mTop ;
	BOOL mSorted ;
} ;

struct ArrayListHolder implement Interface {
	imports VFat<ArrayListHolder> create (VREF<ArrayListLayout> that) ;
	imports CFat<ArrayListHolder> create (CREF<ArrayListLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
	virtual INDEX insert (RREF<BoxLayout> item) = 0 ;
	virtual INDEX insert (CREF<INDEX> index ,RREF<BoxLayout> item) = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void order (CREF<Array<INDEX>> range_) = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class ArrayListUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = TupleNode<A ,ArrayListNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_hold (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class ArrayListRealLayout implement ArrayListLayout {
public:
	implicit ArrayListRealLayout () noexcept {
		auto &&rax = keep[TYPE<AllocatorLayout>::expr] (thiz.mList) ;
		rax = Allocator<A ,ArrayListNode> () ;
	}
} ;

template <class A>
class ArrayList implement ArrayListRealLayout<A> {
protected:
	using ArrayListRealLayout<A>::mList ;
	using ArrayListRealLayout<A>::mRange ;
	using ArrayListRealLayout<A>::mTop ;

public:
	implicit ArrayList () = default ;

	explicit ArrayList (CREF<SizeProxy> size_) {
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> () ,size_) ;
	}

	explicit ArrayList (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
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

	ArrayIterator<CREF<ArrayList>> begin () const leftvalue {
		return ArrayIterator<CREF<ArrayList>> (thiz) ;
	}

	ArrayIterator<CREF<ArrayList>> end () const leftvalue {
		return ArrayIterator<CREF<ArrayList>> (thiz) ;
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
		auto rax = Box<A>::make () ;
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::create (thiz)->insert (move (rax)) ;
	}

	INDEX insert (CREF<INDEX> index) {
		auto rax = Box<A>::make () ;
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::create (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CREF<INDEX> index) {
		return ArrayListHolder::create (thiz)->remove (index) ;
	}

	void order (CREF<Array<INDEX>> range_) {
		return ArrayListHolder::create (thiz)->order (range_) ;
	}

	void remap () {
		return ArrayListHolder::create (thiz)->remap () ;
	}
} ;

struct SortedMapNode implement AllocatorNode {
	INDEX mMap ;
} ;

struct SortedMapImplLayout {
	Allocator<Pointer ,SortedMapNode> mList ;
	INDEX mCheck ;
} ;

struct SortedMapLayout {
	SharedRef<SortedMapImplLayout> mThis ;
	RefBuffer<INDEX> mRange ;
	INDEX mWrite ;
	BOOL mSorted ;
} ;

struct SortedMapHolder implement Interface {
	imports VFat<SortedMapHolder> create (VREF<SortedMapLayout> that) ;
	imports CFat<SortedMapHolder> create (CREF<SortedMapLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual SortedMapLayout share () const = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual INDEX find (CREF<Pointer> item) const = 0 ;
	virtual BOOL contain (CREF<Pointer> item) const = 0 ;
	virtual INDEX map (CREF<Pointer> item) const = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class SortedMapUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = TupleNode<A ,SortedMapNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_hold (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_hold (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class SortedMapRealLayout implement SortedMapLayout {
public:
	implicit SortedMapRealLayout () noexcept {
		if (!thiz.mThis.exist ())
			return ;
		auto &&rax = keep[TYPE<AllocatorLayout>::expr] (thiz.mThis->mList) ;
		rax = Allocator<A ,SortedMapNode> () ;
	}
} ;

template <class A>
class SortedMap implement SortedMapRealLayout<A> {
protected:
	using SortedMapRealLayout<A>::mThis ;
	using SortedMapRealLayout<A>::mRange ;
	using SortedMapRealLayout<A>::mWrite ;
	using SortedMapRealLayout<A>::mSorted ;

public:
	implicit SortedMap () = default ;

	explicit SortedMap (CREF<SizeProxy> size_) {
		SortedMapHolder::create (thiz)->initialize (SortedMapUnknownBinder<A> () ,size_) ;
	}

	explicit SortedMap (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		SortedMapHolder::create (thiz)->initialize (SortedMapUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	SortedMap share () const {
		SortedMapLayout ret = SortedMapHolder::create (thiz)->share () ;
		return move (keep[TYPE<SortedMap>::expr] (ret)) ;
	}

	void clear () {
		return SortedMapHolder::create (thiz)->clear () ;
	}

	LENGTH size () const {
		return SortedMapHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return SortedMapHolder::create (thiz)->step () ;
	}

	LENGTH length () const {
		return SortedMapHolder::create (thiz)->length () ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return SortedMapHolder::create (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return SortedMapHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SortedMapHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return SortedMapHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<SortedMap>> begin () const leftvalue {
		return ArrayIterator<CREF<SortedMap>> (thiz) ;
	}

	ArrayIterator<CREF<SortedMap>> end () const leftvalue {
		return ArrayIterator<CREF<SortedMap>> (thiz) ;
	}

	CREF<ArrayRange<SortedMap>> range () const leftvalue {
		return ArrayRange<SortedMap>::from (thiz) ;
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
		SortedMapHolder::create (thiz)->initialize (SortedMapUnknownBinder<A> ()) ;
		return SortedMapHolder::create (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return SortedMapHolder::create (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return SortedMapHolder::create (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
		return SortedMapHolder::create (thiz)->map (Pointer::from (item)) ;
	}

	void remap () {
		return SortedMapHolder::create (thiz)->remap () ;
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
} ;

struct SetHolder implement Interface {
	imports VFat<SetHolder> create (VREF<SetLayout> that) ;
	imports CFat<SetHolder> create (CREF<SetLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
class SetUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = TupleNode<A ,SetNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_hold (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_hold (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class SetRealLayout implement SetLayout {
public:
	implicit SetRealLayout () noexcept {
		auto &&rax = keep[TYPE<AllocatorLayout>::expr] (thiz.mSet) ;
		rax = Allocator<A ,SetNode> () ;
	}
} ;

template <class A>
class Set implement SetRealLayout<A> {
protected:
	using SetRealLayout<A>::mSet ;
	using SetRealLayout<A>::mRoot ;
	using SetRealLayout<A>::mTop ;

public:
	implicit Set () = default ;

	explicit Set (CREF<SizeProxy> size_) {
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> () ,size_) ;
	}

	explicit Set (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
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

	ArrayIterator<CREF<Set>> begin () const leftvalue {
		return ArrayIterator<CREF<Set>> (thiz) ;
	}

	ArrayIterator<CREF<Set>> end () const leftvalue {
		return ArrayIterator<CREF<Set>> (thiz) ;
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

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
class HashSetUnknownBinder implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		using R1X = TupleNode<A ,HashSetNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_hold (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_hold (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_hold (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_hold (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class HashSetRealLayout implement HashSetLayout {
public:
	implicit HashSetRealLayout () noexcept {
		auto &&rax = keep[TYPE<AllocatorLayout>::expr] (thiz.mSet) ;
		rax = Allocator<A ,HashSetNode> () ;
	}
} ;

template <class A>
class HashSet implement HashSetRealLayout<A> {
protected:
	using HashSetRealLayout<A>::mSet ;
	using HashSetRealLayout<A>::mRange ;
	using HashSetRealLayout<A>::mVisitor ;

public:
	implicit HashSet () = default ;

	explicit HashSet (CREF<SizeProxy> size_) {
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> () ,size_) ;
	}

	explicit HashSet (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
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

	ArrayIterator<CREF<HashSet>> begin () const leftvalue {
		return ArrayIterator<CREF<HashSet>> (thiz) ;
	}

	ArrayIterator<CREF<HashSet>> end () const leftvalue {
		return ArrayIterator<CREF<HashSet>> (thiz) ;
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

template <class A>
class BitProxy {
protected:
	XREF<A> mThat ;
	INDEX mIndex ;

public:
	implicit BitProxy () = delete ;

	explicit BitProxy (XREF<A> that ,CREF<INDEX> index) :mThat (that) {
		mIndex = index ;
	}

	forceinline operator BOOL () rightvalue {
		BOOL ret ;
		mThat.get (mIndex ,ret) ;
		return move (ret) ;
	}

	forceinline void operator= (CREF<BOOL> that) rightvalue {
		mThat.set (mIndex ,that) ;
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
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<BitSetLayout> that) = 0 ;
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
	virtual void visit (VREF<Visitor> visitor) const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual BOOL contain (CREF<Pointer> item) const = 0 ;
	virtual void erase (CREF<Pointer> item) = 0 ;
	virtual void fill (CREF<BYTE> item) = 0 ;
	virtual BitSetLayout sand (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout sor (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout sxor (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout ssub (CREF<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout snot () const = 0 ;
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

	explicit BitSet (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A>::make () ;
		BitSetHolder::create (thiz)->initialize (BufferUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	implicit BitSet (CREF<BitSet> that) {
		BitSetHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<BitSet> operator= (CREF<BitSet> that) {
		return assign (thiz ,that) ;
	}

	implicit BitSet (RREF<BitSet> that) noexcept {
		swap (thiz ,that) ;
	}

	forceinline VREF<BitSet> operator= (RREF<BitSet> that) noexcept {
		return assign (thiz ,that) ;
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

	forceinline BitProxy<VREF<BitSet>> operator[] (CREF<INDEX> index) leftvalue {
		return BitProxy<VREF<BitSet>> (thiz ,index) ;
	}

	forceinline BitProxy<CREF<BitSet>> operator[] (CREF<INDEX> index) const leftvalue {
		return BitProxy<CREF<BitSet>> (thiz ,index) ;
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

	ArrayIterator<CREF<BitSet>> begin () const leftvalue {
		return ArrayIterator<CREF<BitSet>> (thiz) ;
	}

	ArrayIterator<CREF<BitSet>> end () const leftvalue {
		return ArrayIterator<CREF<BitSet>> (thiz) ;
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
		return (!equal (that)) ;
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

	void visit (VREF<Visitor> visitor) const {
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

	BitSet sand (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->sand (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator& (CREF<BitSet> that) const {
		return sand (that) ;
	}

	forceinline void operator&= (CREF<BitSet> that) {
		thiz = sand (that) ;
	}

	BitSet sor (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->sor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator| (CREF<BitSet> that) const {
		return sor (that) ;
	}

	forceinline void operator|= (CREF<BitSet> that) {
		thiz = sor (that) ;
	}

	BitSet sxor (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->sxor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator^ (CREF<BitSet> that) const {
		return sxor (that) ;
	}

	forceinline void operator^= (CREF<BitSet> that) {
		thiz = sxor (that) ;
	}

	BitSet ssub (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::create (thiz)->ssub (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator- (CREF<BitSet> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<BitSet> that) {
		thiz = ssub (that) ;
	}

	BitSet snot () const {
		BitSetLayout ret = BitSetHolder::create (thiz)->snot () ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator~ () const {
		return snot () ;
	}
} ;
} ;