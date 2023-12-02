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

template <class A>
class ArrayIterator {
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

	ArrayIterator begin () const {
		return thiz ;
	}

	ArrayIterator end () const {
		return thiz ;
	}

	LENGTH rank () const {
		return mThat->length () ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CREF<ArrayIterator>) const {
		return ifnot (good ()) ;
	}

	forceinline BOOL operator!= (CREF<ArrayIterator>) const {
		return good () ;
	}

	CREF<INDEX> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CREF<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek = mThat->inext (mPeek) ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct ArrayLayout {
	RefBufferLayout mArray ;
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> create (VREF<ArrayLayout> that) ;
	imports CFat<ArrayHolder> create (CREF<ArrayLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
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
} ;

template <class A>
class ArrayUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return unsafe_hold (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return unsafe_hold (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return unsafe_hold (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectCloneBinder<A>::expr)
			return unsafe_hold (ReflectCloneBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return unsafe_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return unsafe_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return unsafe_hold (ReflectVisitBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class Array implement ArrayLayout {
protected:
	require (CHECK<RefBuffer<A>>) ;
	using ArrayLayout::mArray ;

public:
	implicit Array () = default ;

	explicit Array (CREF<SizeProxy> size_) {
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
	}

	explicit Array (CREF<csc_initializer_t<A>> that) {
		ArrayHolder::create (thiz)->initialize (ArrayUnknownBinder<A> () ,capture (that)) ;
	}

	implicit Array (CREF<Array> that) {
		ArrayHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Array> operator= (CREF<Array> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit Array (RREF<Array>) = default ;

	forceinline VREF<Array> operator= (RREF<Array>) = default ;

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

	ArrayIterator<CPTR<Array>> iter () const leftvalue {
		return ArrayIterator<CPTR<Array>> ((&thiz)) ;
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
} ;

struct MakeArrayHolder implement Interface {
	imports VFat<MakeArrayHolder> create (VREF<ArrayLayout> that) ;
	imports CFat<MakeArrayHolder> create (CREF<ArrayLayout> that) ;

	virtual void MakeArray_initialize () = 0 ;
} ;

template <class A>
class MakeArray implement Proxy {
public:
	imports Array<A> make () {
		Array<A> ret ;
		MakeArrayHolder::create (ret)->MakeArray_initialize () ;
		return move (ret) ;
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

struct StringLayout {
	RefBufferLayout mString ;
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> create (VREF<StringLayout> that) ;
	imports CFat<StringHolder> create (CREF<StringLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<SliceLayout> that) = 0 ;
	virtual void initialize (CREF<StringLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
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
} ;

class String implement StringLayout {
protected:
	using StringLayout::mString ;

public:
	implicit String () = default ;

	explicit String (CREF<SizeProxy> size_ ,CREF<LENGTH> step_) {
		StringHolder::create (thiz)->initialize (size_ ,step_) ;
	}

	explicit String (CREF<SliceLayout> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	implicit String (CREF<String> that) {
		StringHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<String> operator= (CREF<String> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit String (RREF<String>) = default ;

	forceinline VREF<String> operator= (RREF<String>) = default ;

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

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		return StringHolder::create (thiz)->get (index ,item) ;
	}

	void set (CREF<INDEX> index ,CREF<STRU32> item) {
		return StringHolder::create (thiz)->set (index ,item) ;
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

	ArrayIterator<CPTR<String>> iter () const leftvalue {
		return ArrayIterator<CPTR<String>> ((&thiz)) ;
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
} ;

struct MakeStringHolder implement Interface {
	imports VFat<MakeStringHolder> create (VREF<StringLayout> that) ;
	imports CFat<MakeStringHolder> create (CREF<StringLayout> that) ;

	virtual void MakeString_initialize () = 0 ;
} ;

class MakeString implement Proxy {
public:
	imports String make () {
		String ret ;
		MakeStringHolder::create (ret)->MakeString_initialize () ;
		return move (ret) ;
	}
} ;

struct DequeLayout {
	RefBufferLayout mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit DequeLayout () noexcept {
		mRead = 0 ;
		mWrite = 0 ;
	}
} ;

struct DequeNodeLayout {} ;

template <class A>
struct DequeNode {
	TEMP<A> mItem ;
	DequeNodeLayout mNode ;
} ;

struct DequeHolder implement Interface {
	imports VFat<DequeHolder> create (VREF<DequeLayout> that) ;
	imports CFat<DequeHolder> create (CREF<DequeLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
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
	virtual void add (CREF<Unknown> holder ,RREF<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (CREF<Unknown> holder ,RREF<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
} ;

template <class A>
class DequeUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<DequeNode<A>>::expr)
			return unsafe_hold (ReflectSizeBinder<DequeNode<A>> ()) ;
		if (uuid == ReflectCreateBinder<DequeNode<A>>::expr)
			return unsafe_hold (ReflectCreateBinder<DequeNode<A>> ()) ;
		if (uuid == ReflectDestroyBinder<DequeNode<A>>::expr)
			return unsafe_hold (ReflectDestroyBinder<DequeNode<A>> ()) ;
		if (uuid == ReflectCloneBinder<DequeNode<A>>::expr)
			return unsafe_hold (ReflectCloneBinder<DequeNode<A>> ()) ;
		if (uuid == ReflectNodeBinder<DequeNode<A>>::expr)
			return unsafe_hold (ReflectNodeBinder<DequeNode<A>> ()) ;
		return 0 ;
	}
} ;

template <class A>
class Deque implement DequeLayout {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	require (CHECK<RefBuffer<DequeNode<A>>>) ;
	using DequeLayout::mDeque ;
	using DequeLayout::mRead ;
	using DequeLayout::mWrite ;

public:
	implicit Deque () = default ;

	explicit Deque (CREF<SizeProxy> size_) {
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> () ,size_) ;
	}

	explicit Deque (CREF<csc_initializer_t<A>> that) {
		DequeHolder::create (thiz)->initialize (DequeUnknownBinder<A> () ,capture (that)) ;
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

	ArrayIterator<CPTR<Deque>> iter () const leftvalue {
		return ArrayIterator<CPTR<Deque>> ((&thiz)) ;
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
		return DequeHolder::create (thiz)->add (DequeUnknownBinder<A> () ,move (rax)) ;
	}

	void take () {
		return DequeHolder::create (thiz)->take () ;
	}

	void push (CREF<A> item) {
		move (move (item)) ;
	}

	void push (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return DequeHolder::create (thiz)->push (DequeUnknownBinder<A> () ,move (rax)) ;
	}

	void pop () {
		return DequeHolder::create (thiz)->pop () ;
	}
} ;

struct PriorityLayout {
	RefBufferLayout mPriority ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit PriorityLayout () noexcept {
		mRead = 0 ;
		mWrite = 0 ;
	}
} ;

struct PriorityNodeLayout {
	INDEX mMap ;
} ;

template <class A>
struct PriorityNode {
	TEMP<A> mItem ;
	PriorityNodeLayout mNode ;
} ;

struct PriorityHolder implement Interface {
	imports VFat<PriorityHolder> create (VREF<PriorityLayout> that) ;
	imports CFat<PriorityHolder> create (CREF<PriorityLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual void add (CREF<Unknown> holder ,RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual void take () = 0 ;
} ;

template <class A>
class PriorityUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<PriorityNode<A>>::expr)
			return unsafe_hold (ReflectSizeBinder<PriorityNode<A>> ()) ;
		if (uuid == ReflectCreateBinder<PriorityNode<A>>::expr)
			return unsafe_hold (ReflectCreateBinder<PriorityNode<A>> ()) ;
		if (uuid == ReflectDestroyBinder<PriorityNode<A>>::expr)
			return unsafe_hold (ReflectDestroyBinder<PriorityNode<A>> ()) ;
		if (uuid == ReflectCloneBinder<PriorityNode<A>>::expr)
			return unsafe_hold (ReflectCloneBinder<PriorityNode<A>> ()) ;
		if (uuid == ReflectNodeBinder<PriorityNode<A>>::expr)
			return unsafe_hold (ReflectNodeBinder<PriorityNode<A>> ()) ;
		return 0 ;
	}
} ;

template <class A>
class Priority implement PriorityLayout {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	require (CHECK<RefBuffer<PriorityNode<A>>>) ;
	using PriorityLayout::mPriority ;
	using PriorityLayout::mRead ;
	using PriorityLayout::mWrite ;

public:
	implicit Priority () = default ;

	explicit Priority (CREF<SizeProxy> size_) {
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> () ,size_) ;
	}

	explicit Priority (CREF<csc_initializer_t<A>> that) {
		PriorityHolder::create (thiz)->initialize (PriorityUnknownBinder<A> () ,capture (that)) ;
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

	INDEX ibegin () const {
		return PriorityHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return PriorityHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return PriorityHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Priority>> iter () const leftvalue {
		return ArrayIterator<CPTR<Priority>> ((&thiz)) ;
	}

	BOOL empty () const {
		return PriorityHolder::create (thiz)->empty () ;
	}

	INDEX head () const {
		return PriorityHolder::create (thiz)->head () ;
	}

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return PriorityHolder::create (thiz)->add (PriorityUnknownBinder<A> () ,move (rax)) ;
	}

	void take () {
		return PriorityHolder::create (thiz)->take () ;
	}
} ;

struct ListLayout {
	AllocatorLayout mList ;
	INDEX mFirst ;
	INDEX mLast ;

public:
	implicit ListLayout () noexcept {
		mFirst = NONE ;
		mLast = NONE ;
	}
} ;

struct ListNodeLayout {
	INDEX mLeft ;
	INDEX mRight ;
} ;

template <class A>
struct ListNode {
	TEMP<A> mItem ;
	ListNodeLayout mNode ;
} ;

struct ListHolder implement Interface {
	imports VFat<ListHolder> create (VREF<ListLayout> that) ;
	imports CFat<ListHolder> create (CREF<ListLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
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
	virtual void add (CREF<Unknown> holder ,RREF<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (CREF<Unknown> holder ,RREF<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual INDEX insert () = 0 ;
	virtual INDEX insert (CREF<INDEX> index) = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
} ;

template <class A>
class ListUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<AllocatorNode<ListNode<A>>>::expr)
			return unsafe_hold (ReflectSizeBinder<AllocatorNode<ListNode<A>>> ()) ;
		if (uuid == ReflectCreateBinder<AllocatorNode<ListNode<A>>>::expr)
			return unsafe_hold (ReflectCreateBinder<AllocatorNode<ListNode<A>>> ()) ;
		if (uuid == ReflectDestroyBinder<AllocatorNode<ListNode<A>>>::expr)
			return unsafe_hold (ReflectDestroyBinder<AllocatorNode<ListNode<A>>> ()) ;
		if (uuid == ReflectCloneBinder<AllocatorNode<ListNode<A>>>::expr)
			return unsafe_hold (ReflectCloneBinder<AllocatorNode<ListNode<A>>> ()) ;
		if (uuid == ReflectNodeBinder<AllocatorNode<ListNode<A>>>::expr)
			return unsafe_hold (ReflectNodeBinder<AllocatorNode<ListNode<A>>> ()) ;
		if (uuid == ReflectRemakeBinder<A>::expr)
			return unsafe_hold (ReflectRemakeBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class List implement ListLayout {
protected:
	require (CHECK<Allocator<ListNode<A>>>) ;
	using ListLayout::mList ;
	using ListLayout::mFirst ;
	using ListLayout::mLast ;

public:
	implicit List () = default ;

	explicit List (CREF<SizeProxy> size_) {
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> () ,size_) ;
	}

	explicit List (CREF<csc_initializer_t<A>> that) {
		ListHolder::create (thiz)->initialize (ListUnknownBinder<A> () ,capture (that)) ;
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

	ArrayIterator<CPTR<List>> iter () const leftvalue {
		return ArrayIterator<CPTR<List>> ((&thiz)) ;
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
		return ListHolder::create (thiz)->add (ListUnknownBinder<A> () ,move (rax)) ;
	}

	void take () {
		return ListHolder::create (thiz)->take () ;
	}

	void push (CREF<A> item) {
		move (move (item)) ;
	}

	void push (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return ListHolder::create (thiz)->push (ListUnknownBinder<A> () ,move (rax)) ;
	}

	void pop () {
		return ListHolder::create (thiz)->pop () ;
	}

	INDEX insert () {
		return ListHolder::create (thiz)->insert () ;
	}

	INDEX insert (CREF<INDEX> index) {
		return ListHolder::create (thiz)->insert (index) ;
	}

	void remove (CREF<INDEX> index) {
		return ListHolder::create (thiz)->remove (index) ;
	}
} ;

struct ArrayListLayout {
	AllocatorLayout mList ;
	RefBuffer<INDEX> mRange ;
} ;

struct ArrayListHolder implement Interface {
	imports VFat<ArrayListHolder> create (VREF<ArrayListLayout> that) ;
	imports CFat<ArrayListHolder> create (CREF<ArrayListLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual INDEX insert () = 0 ;
	virtual INDEX insert (CREF<INDEX> index) = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class ArrayListUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectSizeBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectCreateBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectCreateBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectDestroyBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectDestroyBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectCloneBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectCloneBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectNodeBinder<AllocatorNode<A>>::expr)
			return unsafe_hold (ReflectNodeBinder<AllocatorNode<A>> ()) ;
		if (uuid == ReflectRemakeBinder<A>::expr)
			return unsafe_hold (ReflectRemakeBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class ArrayList implement ArrayListLayout {
protected:
	require (CHECK<Allocator<A>>) ;
	using ArrayListLayout::mList ;
	using ArrayListLayout::mRange ;

public:
	implicit ArrayList () = default ;

	explicit ArrayList (CREF<SizeProxy> size_) {
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> () ,size_) ;
	}

	explicit ArrayList (CREF<csc_initializer_t<A>> that) {
		ArrayListHolder::create (thiz)->initialize (ArrayListUnknownBinder<A> () ,capture (that)) ;
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

	ArrayIterator<CPTR<ArrayList>> iter () const leftvalue {
		return ArrayIterator<CPTR<ArrayList>> ((&thiz)) ;
	}

	INDEX insert () {
		return ArrayListHolder::create (thiz)->insert () ;
	}

	INDEX insert (CREF<INDEX> index) {
		return ArrayListHolder::create (thiz)->insert (index) ;
	}

	void remove (CREF<INDEX> index) {
		return ArrayListHolder::create (thiz)->remove (index) ;
	}

	void remap () {
		return ArrayListHolder::create (thiz)->remap () ;
	}
} ;

struct SetLayout {
	AllocatorLayout mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	implicit SetLayout () noexcept {
		mRoot = NONE ;
		mTop = NONE ;
	}
} ;

struct SetNodeLayout {
	INDEX mMap ;
	BOOL mRed ;
	INDEX mUp ;
	INDEX mLeft ;
	INDEX mRight ;
} ;

template <class A>
struct SetNode {
	TEMP<A> mItem ;
	SetNodeLayout mNode ;
} ;

struct SetHolder implement Interface {
	imports VFat<SetHolder> create (VREF<SetLayout> that) ;
	imports CFat<SetHolder> create (CREF<SetLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (CREF<Unknown> holder ,RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual INDEX find (CREF<BoxLayout> item) const = 0 ;
	virtual BOOL contain (CREF<BoxLayout> item) const = 0 ;
	virtual INDEX insert (CREF<BoxLayout> item) const = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void erase (CREF<BoxLayout> item) = 0 ;
} ;

template <class A>
class SetUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<AllocatorNode<SetNode<A>>>::expr)
			return unsafe_hold (ReflectSizeBinder<AllocatorNode<SetNode<A>>> ()) ;
		if (uuid == ReflectCreateBinder<AllocatorNode<SetNode<A>>>::expr)
			return unsafe_hold (ReflectCreateBinder<AllocatorNode<SetNode<A>>> ()) ;
		if (uuid == ReflectDestroyBinder<AllocatorNode<SetNode<A>>>::expr)
			return unsafe_hold (ReflectDestroyBinder<AllocatorNode<SetNode<A>>> ()) ;
		if (uuid == ReflectCloneBinder<AllocatorNode<SetNode<A>>>::expr)
			return unsafe_hold (ReflectCloneBinder<AllocatorNode<SetNode<A>>> ()) ;
		if (uuid == ReflectNodeBinder<AllocatorNode<SetNode<A>>>::expr)
			return unsafe_hold (ReflectNodeBinder<AllocatorNode<SetNode<A>>> ()) ;
		if (uuid == ReflectRemakeBinder<A>::expr)
			return unsafe_hold (ReflectRemakeBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return unsafe_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return unsafe_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return unsafe_hold (ReflectVisitBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class Set implement SetLayout {
protected:
	require (CHECK<Allocator<SetNode<A>>>) ;
	using SetLayout::mSet ;
	using SetLayout::mRoot ;
	using SetLayout::mTop ;

public:
	implicit Set () = default ;

	explicit Set (CREF<SizeProxy> size_) {
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> () ,size_) ;
	}

	explicit Set (CREF<csc_initializer_t<A>> that) {
		SetHolder::create (thiz)->initialize (SetUnknownBinder<A> () ,capture (that)) ;
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

	INDEX ibegin () const {
		return SetHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SetHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return SetHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<Set>> iter () const leftvalue {
		return ArrayIterator<CPTR<Set>> ((&thiz)) ;
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
		return SetHolder::create (thiz)->add (SetUnknownBinder<A> () ,move (rax) ,map_) ;
	}

	INDEX find (CREF<BoxLayout> item) const {
		return SetHolder::create (thiz)->find (item) ;
	}

	BOOL contain (CREF<BoxLayout> item) const {
		return SetHolder::create (thiz)->contain (item) ;
	}

	INDEX insert (CREF<BoxLayout> item) const {
		return SetHolder::create (thiz)->insert (item) ;
	}

	void remove (CREF<INDEX> index) {
		return SetHolder::create (thiz)->remove (> index) ;
	}
} ;

struct HashSetLayout {
	AllocatorLayout mSet ;
	RefBuffer<INDEX> mRange ;
	INDEX mTop ;

public:
	implicit HashSetLayout () noexcept {
		mTop = NONE ;
	}
} ;

struct HashSetNodeLayout {
	FLAG mHash ;
	INDEX mDown ;
} ;

template <class A>
struct HashSetNode {
	TEMP<A> mItem ;
	HashSetNodeLayout mNode ;
} ;

struct HashSetHolder implement Interface {
	imports VFat<HashSetHolder> create (VREF<HashSetLayout> that) ;
	imports CFat<HashSetHolder> create (CREF<HashSetLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual void add (CREF<Unknown> holder ,RREF<BoxLayout> item ,CREF<INDEX> map_) = 0 ;
	virtual INDEX find (CREF<BoxLayout> item) const = 0 ;
	virtual BOOL contain (CREF<BoxLayout> item) const = 0 ;
	virtual INDEX insert (CREF<BoxLayout> item) const = 0 ;
	virtual void remove (CREF<INDEX> index) = 0 ;
	virtual void erase (CREF<BoxLayout> item) = 0 ;
} ;

template <class A>
class HashSetUnknownBinder final implement Unknown {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<AllocatorNode<HashSetNode<A>>>::expr)
			return unsafe_hold (ReflectSizeBinder<AllocatorNode<HashSetNode<A>>> ()) ;
		if (uuid == ReflectCreateBinder<AllocatorNode<HashSetNode<A>>>::expr)
			return unsafe_hold (ReflectCreateBinder<AllocatorNode<HashSetNode<A>>> ()) ;
		if (uuid == ReflectDestroyBinder<AllocatorNode<HashSetNode<A>>>::expr)
			return unsafe_hold (ReflectDestroyBinder<AllocatorNode<HashSetNode<A>>> ()) ;
		if (uuid == ReflectCloneBinder<AllocatorNode<HashSetNode<A>>>::expr)
			return unsafe_hold (ReflectCloneBinder<AllocatorNode<HashSetNode<A>>> ()) ;
		if (uuid == ReflectNodeBinder<AllocatorNode<HashSetNode<A>>>::expr)
			return unsafe_hold (ReflectNodeBinder<AllocatorNode<HashSetNode<A>>> ()) ;
		if (uuid == ReflectRemakeBinder<A>::expr)
			return unsafe_hold (ReflectRemakeBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return unsafe_hold (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return unsafe_hold (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return unsafe_hold (ReflectVisitBinder<A> ()) ;
		return 0 ;
	}
} ;

template <class A>
class HashSet implement HashSetLayout {
protected:
	require (CHECK<Allocator<HashSetNode<A>>>) ;
	using HashSetLayout::mSet ;
	using HashSetLayout::mRange ;
	using HashSetLayout::mTop ;

public:
	implicit HashSet () = default ;

	explicit HashSet (CREF<SizeProxy> size_) {
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> () ,size_) ;
	}

	explicit HashSet (CREF<csc_initializer_t<A>> that) {
		HashSetHolder::create (thiz)->initialize (HashSetUnknownBinder<A> () ,capture (that)) ;
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

	INDEX ibegin () const {
		return HashSetHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return HashSetHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return HashSetHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<HashSet>> iter () const leftvalue {
		return ArrayIterator<CPTR<HashSet>> ((&thiz)) ;
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
		return HashSetHolder::create (thiz)->add (HashSetUnknownBinder<A> () ,move (rax) ,map_) ;
	}

	INDEX find (CREF<BoxLayout> item) const {
		return HashSetHolder::create (thiz)->find (item) ;
	}

	BOOL contain (CREF<BoxLayout> item) const {
		return HashSetHolder::create (thiz)->contain (item) ;
	}

	INDEX insert (CREF<BoxLayout> item) const {
		return HashSetHolder::create (thiz)->insert (item) ;
	}

	void remove (CREF<INDEX> index) {
		return HashSetHolder::create (thiz)->remove (> index) ;
	}
} ;

struct BitSetLayout {
	RefBuffer<BYTE> mSet ;
	BYTE mMask ;
	LENGTH mSize ;

public:
	implicit BitSetLayout () noexcept {
		mMask = BYTE (0X00) ;
		mSize = 0 ;
	}
} ;

struct BitSetHolder implement Interface {
	imports VFat<BitSetHolder> create (VREF<BitSetLayout> that) ;
	imports CFat<BitSetHolder> create (CREF<BitSetLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<CaptureLayout> that) = 0 ;
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
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void fill (CREF<BYTE> item) = 0 ;
	virtual void add (CREF<INDEX> item) = 0 ;
	virtual BOOL contain (CREF<INDEX> item) const = 0 ;
	virtual void erase (CREF<INDEX> item) = 0 ;
} ;

class BitSet implement BitSetLayout {
protected:
	using BitSetLayout::mSet ;
	using BitSetLayout::mSize ;

public:
	implicit BitSet () = default ;

	explicit BitSet (CREF<SizeProxy> size_) {
		BitSetHolder::create (thiz)->initialize (size_) ;
	}

	explicit BitSet (CREF<csc_initializer_t<INDEX>> that) {
		BitSetHolder::create (thiz)->initialize (capture (that)) ;
	}

	implicit BitSet (CREF<BitSet> that) {
		BitSetHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<BitSet> operator= (CREF<BitSet> that) {
		if (address (thiz) == address (that))
			return thiz ;
		swap (thiz ,move (that)) ;
		return thiz ;
	}

	implicit BitSet (RREF<BitSet>) = default ;

	forceinline VREF<BitSet> operator= (RREF<BitSet>) = default ;

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

	INDEX ibegin () const {
		return BitSetHolder::create (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return BitSetHolder::create (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return BitSetHolder::create (thiz)->inext (index) ;
	}

	ArrayIterator<CPTR<BitSet>> iter () const leftvalue {
		return ArrayIterator<CPTR<BitSet>> ((&thiz)) ;
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

	void fill (CREF<BYTE> item) {
		return BitSetHolder::create (thiz)->fill (item) ;
	}

	void add (CREF<INDEX> item) {
		return BitSetHolder::create (thiz)->add (item) ;
	}

	BOOL contain (CREF<INDEX> item) {
		return BitSetHolder::create (thiz)->contain (item) ;
	}

	void erase (CREF<INDEX> item) {
		return BitSetHolder::create (thiz)->erase (item) ;
	}
} ;
} ;