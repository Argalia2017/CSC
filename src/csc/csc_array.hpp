﻿#pragma once

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

	implicit SizeProxy (CR<LENGTH> size_) {
		mSize = size_ ;
	}

	forceinline operator LENGTH () const {
		return mSize ;
	}
} ;

template <class A>
class ArrayIterator {
private:
	using ITEM = decltype (nullof (A).at (0)) ;

protected:
	XR<A> mThat ;
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (XR<A> that) :mThat (that) {
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

	forceinline BOOL operator== (CR<ArrayIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CR<ArrayIterator>) const {
		return good () ;
	}

	XR<ITEM> peek () const leftvalue {
		return mThat.at (mPeek) ;
	}

	forceinline XR<ITEM> operator* () const leftvalue {
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
class ArrayRange {
protected:
	XR<A> mThat ;

public:
	implicit ArrayRange () = delete ;

	explicit ArrayRange (XR<A> that) :mThat (that) {}

	LENGTH length () const {
		return mThat.length () ;
	}

	CR<INDEX> at (CR<INDEX> index) const leftvalue {
		return index ;
	}

	INDEX ibegin () const {
		return mThat.ibegin () ;
	}

	INDEX iend () const {
		return mThat.iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return mThat.inext (index) ;
	}

	ArrayIterator<CR<ArrayRange>> begin () const leftvalue {
		return ArrayIterator<CR<ArrayRange>> (thiz) ;
	}

	ArrayIterator<CR<ArrayRange>> end () const leftvalue {
		return ArrayIterator<CR<ArrayRange>> (thiz) ;
	}
} ;

struct ArrayLayout {
	RefBuffer<Pointer> mArray ;
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> hold (VR<ArrayLayout> that) ;
	imports CFat<ArrayHolder> hold (CR<ArrayLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void initialize (CR<ArrayLayout> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual VR<Pointer> at (CR<INDEX> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual BOOL equal (CR<ArrayLayout> that) const = 0 ;
	virtual FLAG compr (CR<ArrayLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual void fill (CR<Pointer> item) = 0 ;
	virtual void splice (CR<INDEX> index ,CR<ArrayLayout> item) = 0 ;
} ;

template <class A>
class ArrayUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectCloneBinder<A>::expr)
			return inline_vptr (ReflectCloneBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_vptr (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_vptr (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_vptr (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_vptr (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct ArrayPureLayout implement ArrayLayout {
public:
	implicit ArrayPureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		ArrayHolder::hold (thiz)->prepare (ArrayUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ArrayPureLayout<Pointer> implement ArrayLayout {} ;

template <class A>
class Array implement ArrayPureLayout<A> {
protected:
	using ArrayPureLayout<A>::mArray ;

public:
	implicit Array () = default ;

	explicit Array (CR<SizeProxy> size_) {
		ArrayHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
	}

	explicit Array (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	template <class ARG1>
	static Array make (CR<ARG1> iterator) {
		Array ret = Array (iterator.length ()) ;
		INDEX ix = 0 ;
		for (auto &&i : iterator) {
			ret[ix] = A (i) ;
			ix++ ;
		}
		return move (ret) ;
	}

	implicit Array (CR<Array> that) {
		ArrayHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Array> operator= (CR<Array> that) {
		return assign (thiz ,that) ;
	}

	implicit Array (RR<Array> that) = default ;

	forceinline VR<Array> operator= (RR<Array> that) = default ;

	Array clone () const {
		return move (thiz) ;
	}

	LENGTH size () const {
		return ArrayHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return ArrayHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return ArrayHolder::hold (thiz)->length () ;
	}

	VR<ARR<A>> ref_m () leftvalue {
		return ArrayHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<ARR<A>> () leftvalue {
		return ref ;
	}

	CR<ARR<A>> ref_m () const leftvalue {
		return ArrayHolder::hold (thiz)->ref ;
	}

	forceinline operator CR<ARR<A>> () const leftvalue {
		return ref ;
	}

	VR<A> at (CR<INDEX> index) leftvalue {
		return ArrayHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<INDEX> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return ArrayHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ArrayHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return ArrayHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<Array>> begin () const leftvalue {
		return ArrayIterator<CR<Array>> (thiz) ;
	}

	ArrayIterator<CR<Array>> end () const leftvalue {
		return ArrayIterator<CR<Array>> (thiz) ;
	}

	ArrayRange<CR<Array>> iter () const leftvalue {
		return ArrayRange<CR<Array>> (thiz) ;
	}

	BOOL equal (CR<Array> that) const {
		return ArrayHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Array> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Array> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Array> that) const {
		return ArrayHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Array> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Array> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Array> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Array> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return ArrayHolder::hold (thiz)->visit (visitor) ;
	}

	void fill (CR<A> item) {
		return ArrayHolder::hold (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CR<INDEX> index ,CR<Array> item) {
		return ArrayHolder::hold (thiz)->splice (index ,item) ;
	}
} ;

template <class A>
class StringParse ;

template <class A>
class StringBuild ;

struct StringLayout {
	RefBuffer<Pointer> mString ;
	FLAG mEncode ;
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> hold (VR<StringLayout> that) ;
	imports CFat<StringHolder> hold (CR<StringLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Slice> that ,CR<LENGTH> step_) = 0 ;
	virtual void initialize (CR<LENGTH> size_ ,CR<LENGTH> step_) = 0 ;
	virtual void initialize (CR<StringLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual FLAG encode () const = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () const leftvalue = 0 ;
	virtual void get (CR<INDEX> index ,VR<STRU32> item) const = 0 ;
	virtual void set (CR<INDEX> index ,CR<STRU32> item) = 0 ;
	virtual VR<Pointer> at (CR<INDEX> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual BOOL equal (CR<Slice> that) const = 0 ;
	virtual BOOL equal (CR<StringLayout> that) const = 0 ;
	virtual FLAG compr (CR<Slice> that) const = 0 ;
	virtual FLAG compr (CR<StringLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual void trunc (CR<INDEX> index) = 0 ;
	virtual void fill (CR<STRU32> item) = 0 ;
	virtual void splice (CR<INDEX> index ,CR<Slice> item) = 0 ;
	virtual void splice (CR<INDEX> index ,CR<StringLayout> item) = 0 ;
	virtual Slice segment (CR<INDEX> begin_ ,CR<INDEX> end_) const = 0 ;
} ;

template <class A>
class StringUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_vptr (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct StringPureLayout implement StringLayout {
public:
	implicit StringPureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		StringHolder::hold (thiz)->prepare (StringUnknownBinder<A> ()) ;
	}
} ;

template <>
struct StringPureLayout<Pointer> implement StringLayout {} ;

template <class A>
class String implement StringPureLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	using StringPureLayout<A>::mString ;
	using StringPureLayout<A>::mEncode ;

public:
	implicit String () = default ;

	implicit String (CR<Slice> that) {
		StringHolder::hold (thiz)->initialize (that ,SIZE_OF<A>::expr) ;
	}

	explicit String (CR<SizeProxy> size_) {
		StringHolder::hold (thiz)->initialize (size_ ,SIZE_OF<A>::expr) ;
	}

	template <class...ARG1>
	static String make (CR<ARG1>...params) {
		using R1X = KILL<StringBuild<A> ,TYPE<ARG1...>> ;
		return R1X::make (params...) ;
	}

	static CR<String> zero () {
		return memorize ([&] () {
			return String (slice ("\0")) ;
		}) ;
	}

	implicit String (CR<String> that) {
		StringHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<String> operator= (CR<String> that) {
		return assign (thiz ,that) ;
	}

	implicit String (RR<String> that) = default ;

	forceinline VR<String> operator= (RR<String> that) = default ;

	String clone () const {
		return move (thiz) ;
	}

	void clear () {
		return StringHolder::hold (thiz)->clear () ;
	}

	FLAG encode () const {
		return StringHolder::hold (thiz)->encode () ;
	}

	LENGTH size () const {
		return StringHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return StringHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return StringHolder::hold (thiz)->length () ;
	}

	VR<ARR<A>> ref_m () leftvalue {
		return StringHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<ARR<A>> () leftvalue {
		return ref ;
	}

	CR<ARR<A>> ref_m () const leftvalue {
		return StringHolder::hold (thiz)->ref ;
	}

	forceinline operator CR<ARR<A>> () const leftvalue {
		return ref ;
	}

	Ref<RefBuffer<BYTE>> borrow () leftvalue {
		return StringHolder::hold (thiz)->borrow () ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue {
		return StringHolder::hold (thiz)->borrow () ;
	}

	VR<A> at (CR<INDEX> index) leftvalue {
		return StringHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<INDEX> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return StringHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CR<INDEX> index ,VR<STRU32> map_) const {
		return StringHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CR<INDEX> index ,CR<STRU32> map_) {
		return StringHolder::hold (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return StringHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return StringHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return StringHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<String>> begin () const leftvalue {
		return ArrayIterator<CR<String>> (thiz) ;
	}

	ArrayIterator<CR<String>> end () const leftvalue {
		return ArrayIterator<CR<String>> (thiz) ;
	}

	ArrayRange<CR<String>> iter () const leftvalue {
		return ArrayRange<CR<String>> (thiz) ;
	}

	BOOL equal (CR<Slice> that) const {
		return StringHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Slice> that) const {
		return (!equal (that)) ;
	}

	BOOL equal (CR<String> that) const {
		return StringHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<String> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<String> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<String> that) const {
		return StringHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<String> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<String> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<String> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<String> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return StringHolder::hold (thiz)->visit (visitor) ;
	}

	void trunc (CR<INDEX> index) {
		return StringHolder::hold (thiz)->trunc (index) ;
	}

	void fill (CR<STRU32> item) {
		return StringHolder::hold (thiz)->fill (item) ;
	}

	void splice (CR<INDEX> index ,CR<Slice> item) {
		return StringHolder::hold (thiz)->splice (index ,item) ;
	}

	void splice (CR<INDEX> index ,CR<String> item) {
		return StringHolder::hold (thiz)->splice (index ,item) ;
	}

	Slice segment () const {
		return segment (0 ,length ()) ;
	}

	Slice segment (CR<INDEX> begin_ ,CR<INDEX> end_) const {
		return StringHolder::hold (thiz)->segment (begin_ ,end_) ;
	}
} ;

struct DequeLayout {
	RefBuffer<Pointer> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct DequeHolder implement Interface {
	imports VFat<DequeHolder> hold (VR<DequeLayout> that) ;
	imports CFat<DequeHolder> hold (CR<DequeLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VR<Pointer> at (CR<INDEX> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual BOOL full () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual INDEX tail () const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RR<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual void ring (CR<LENGTH> count) = 0 ;
} ;

template <class A>
class DequeUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_vptr (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct DequePureLayout implement DequeLayout {
public:
	implicit DequePureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		DequeHolder::hold (thiz)->prepare (DequeUnknownBinder<A> ()) ;
	}
} ;

template <>
struct DequePureLayout<Pointer> implement DequeLayout {} ;

template <class A>
class Deque implement DequePureLayout<A> {
protected:
	using DequePureLayout<A>::mDeque ;
	using DequePureLayout<A>::mRead ;
	using DequePureLayout<A>::mWrite ;

public:
	implicit Deque () = default ;

	explicit Deque (CR<SizeProxy> size_) {
		DequeHolder::hold (thiz)->initialize (DequeUnknownBinder<A> () ,size_) ;
	}

	explicit Deque (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		DequeHolder::hold (thiz)->initialize (DequeUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	void clear () {
		return DequeHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return DequeHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return DequeHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return DequeHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<INDEX> index) leftvalue {
		return DequeHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<INDEX> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return DequeHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return DequeHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return DequeHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return DequeHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<Deque>> begin () const leftvalue {
		return ArrayIterator<CR<Deque>> (thiz) ;
	}

	ArrayIterator<CR<Deque>> end () const leftvalue {
		return ArrayIterator<CR<Deque>> (thiz) ;
	}

	ArrayRange<CR<Deque>> iter () const leftvalue {
		return ArrayRange<CR<Deque>> (thiz) ;
	}

	BOOL empty () const {
		return DequeHolder::hold (thiz)->empty () ;
	}

	BOOL full () const {
		return DequeHolder::hold (thiz)->full () ;
	}

	INDEX head () const {
		return DequeHolder::hold (thiz)->head () ;
	}

	INDEX tail () const {
		return DequeHolder::hold (thiz)->tail () ;
	}

	void add (CR<A> item) {
		add (move (item)) ;
	}

	void add (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return DequeHolder::hold (thiz)->add (move (rax)) ;
	}

	void take () {
		return DequeHolder::hold (thiz)->take () ;
	}

	void take (VR<A> item) {
		item = move (thiz[head ()]) ;
		take () ;
	}

	void push (CR<A> item) {
		move (move (item)) ;
	}

	void push (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return DequeHolder::hold (thiz)->push (move (rax)) ;
	}

	void pop () {
		return DequeHolder::hold (thiz)->pop () ;
	}

	void pop (VR<A> item) {
		item = move (thiz[tail ()]) ;
		take () ;
	}

	void ring (CR<LENGTH> count) {
		return DequeHolder::hold (thiz)->ring (count) ;
	}
} ;

struct PriorityLayout {
	RefBuffer<Pointer> mPriority ;
	LENGTH mOffset ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct PriorityHolder implement Interface {
	imports VFat<PriorityHolder> hold (VR<PriorityLayout> that) ;
	imports CFat<PriorityHolder> hold (CR<PriorityLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual BOOL full () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<FLT64> key_) = 0 ;
	virtual void take () = 0 ;
} ;

template <class A>
class PriorityUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		using R1X = Tuple<A ,FLT64> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct PriorityPureLayout implement PriorityLayout {
public:
	implicit PriorityPureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		PriorityHolder::hold (thiz)->prepare (PriorityUnknownBinder<A> ()) ;
	}
} ;

template <>
struct PriorityPureLayout<Pointer> implement PriorityLayout {} ;

template <class A>
class Priority implement PriorityPureLayout<A> {
protected:
	using PriorityPureLayout<A>::mPriority ;
	using PriorityPureLayout<A>::mOffset ;
	using PriorityPureLayout<A>::mRead ;
	using PriorityPureLayout<A>::mWrite ;

public:
	implicit Priority () = default ;

	explicit Priority (CR<SizeProxy> size_) {
		PriorityHolder::hold (thiz)->initialize (PriorityUnknownBinder<A> () ,size_) ;
	}

	explicit Priority (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		PriorityHolder::hold (thiz)->initialize (PriorityUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	void clear () {
		return PriorityHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return PriorityHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return PriorityHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return PriorityHolder::hold (thiz)->length () ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return PriorityHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return PriorityHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return PriorityHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return PriorityHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<Priority>> beign () const leftvalue {
		return ArrayIterator<CR<Priority>> (thiz) ;
	}

	ArrayIterator<CR<Priority>> end () const leftvalue {
		return ArrayIterator<CR<Priority>> (thiz) ;
	}

	ArrayRange<CR<Priority>> iter () const leftvalue {
		return ArrayRange<CR<Priority>> (thiz) ;
	}

	BOOL empty () const {
		return PriorityHolder::hold (thiz)->empty () ;
	}

	BOOL full () const {
		return PriorityHolder::hold (thiz)->full () ;
	}

	INDEX head () const {
		return PriorityHolder::hold (thiz)->head () ;
	}

	void add (CR<A> item ,CR<FLT64> key_) {
		add (move (item) ,key_) ;
	}

	void add (RR<A> item ,CR<FLT64> key_) {
		auto rax = Box<A>::make (move (item)) ;
		return PriorityHolder::hold (thiz)->add (move (rax) ,key_) ;
	}

	void take () {
		return PriorityHolder::hold (thiz)->take () ;
	}

	void take (VR<A> item) {
		item = move (thiz[head ()]) ;
		take () ;
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
	imports VFat<ListHolder> hold (VR<ListLayout> that) ;
	imports CFat<ListHolder> hold (CR<ListLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VR<Pointer> at (CR<INDEX> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual BOOL full () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual INDEX tail () const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RR<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual INDEX insert (RR<BoxLayout> item) = 0 ;
	virtual INDEX insert (CR<INDEX> index ,RR<BoxLayout> item) = 0 ;
	virtual void remove (CR<INDEX> index) = 0 ;
	virtual void order (CR<Array<INDEX>> range_) = 0 ;
} ;

template <class A>
class ListUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		using R1X = UnionPair<A ,ListNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct ListPureLayout implement ListLayout {
public:
	implicit ListPureLayout () noexcept {
		noop (Allocator<A ,ListNode> ()) ;
		ListHolder::hold (thiz)->prepare (ListUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ListPureLayout<Pointer> implement ListLayout {} ;

template <class A>
class List implement ListPureLayout<A> {
protected:
	using ListPureLayout<A>::mList ;
	using ListPureLayout<A>::mFirst ;
	using ListPureLayout<A>::mLast ;

public:
	implicit List () = default ;

	explicit List (CR<SizeProxy> size_) {
		ListHolder::hold (thiz)->initialize (ListUnknownBinder<A> () ,size_) ;
	}

	explicit List (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ListHolder::hold (thiz)->initialize (ListUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	void clear () {
		return ListHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return ListHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return ListHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return ListHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<INDEX> index) leftvalue {
		return ListHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<INDEX> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return ListHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ListHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ListHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return ListHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<List>> begin () const leftvalue {
		return ArrayIterator<CR<List>> (thiz) ;
	}

	ArrayIterator<CR<List>> end () const leftvalue {
		return ArrayIterator<CR<List>> (thiz) ;
	}

	ArrayRange<CR<List>> iter () const leftvalue {
		return ArrayRange<CR<List>> (thiz) ;
	}

	BOOL empty () const {
		return ListHolder::hold (thiz)->empty () ;
	}

	BOOL full () const {
		return ListHolder::hold (thiz)->full () ;
	}

	INDEX head () const {
		return ListHolder::hold (thiz)->head () ;
	}

	INDEX tail () const {
		return ListHolder::hold (thiz)->tail () ;
	}

	void add (CR<A> item) {
		add (move (item)) ;
	}

	void add (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return ListHolder::hold (thiz)->add (move (rax)) ;
	}

	void take () {
		return ListHolder::hold (thiz)->take () ;
	}

	void take (VR<A> item) {
		item = move (thiz[head ()]) ;
		take () ;
	}

	void push (CR<A> item) {
		push (move (item)) ;
	}

	void push (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return ListHolder::hold (thiz)->push (move (rax)) ;
	}

	void pop () {
		return ListHolder::hold (thiz)->pop () ;
	}

	void pop (VR<A> item) {
		item = move (thiz[tail ()]) ;
		pop () ;
	}

	INDEX insert () {
		auto rax = Box<A>::make () ;
		return ListHolder::hold (thiz)->insert (move (rax)) ;
	}

	INDEX insert (CR<INDEX> index) {
		auto rax = Box<A>::make () ;
		return ListHolder::hold (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CR<INDEX> index) {
		return ListHolder::hold (thiz)->remove (index) ;
	}

	void order (CR<Array<INDEX>> range_) {
		return ListHolder::hold (thiz)->order (range_) ;
	}
} ;

struct ArrayListNode implement AllocatorNode {} ;

struct ArrayListLayout {
	Allocator<Pointer ,ArrayListNode> mList ;
	RefBuffer<INDEX> mRange ;
	INDEX mTop ;
	BOOL mRemap ;
} ;

struct ArrayListHolder implement Interface {
	imports VFat<ArrayListHolder> hold (VR<ArrayListLayout> that) ;
	imports CFat<ArrayListHolder> hold (CR<ArrayListLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VR<Pointer> at (CR<INDEX> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual INDEX insert (RR<BoxLayout> item) = 0 ;
	virtual INDEX insert (CR<INDEX> index ,RR<BoxLayout> item) = 0 ;
	virtual void remove (CR<INDEX> index) = 0 ;
	virtual void order (CR<Array<INDEX>> range_) = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class ArrayListUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		using R1X = UnionPair<A ,ArrayListNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct ArrayListPureLayout implement ArrayListLayout {
public:
	implicit ArrayListPureLayout () noexcept {
		noop (Allocator<A ,ArrayListNode> ()) ;
		ArrayListHolder::hold (thiz)->prepare (ArrayListUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ArrayListPureLayout<Pointer> implement ArrayListLayout {} ;

template <class A>
class ArrayList implement ArrayListPureLayout<A> {
protected:
	using ArrayListPureLayout<A>::mList ;
	using ArrayListPureLayout<A>::mRange ;
	using ArrayListPureLayout<A>::mTop ;
	using ArrayListPureLayout<A>::mRemap ;

public:
	implicit ArrayList () = default ;

	explicit ArrayList (CR<SizeProxy> size_) {
		ArrayListHolder::hold (thiz)->initialize (ArrayListUnknownBinder<A> () ,size_) ;
	}

	explicit ArrayList (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayListHolder::hold (thiz)->initialize (ArrayListUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	void clear () {
		return ArrayListHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return ArrayListHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return ArrayListHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return ArrayListHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<INDEX> index) leftvalue {
		return ArrayListHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<INDEX> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return ArrayListHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ArrayListHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayListHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return ArrayListHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<ArrayList>> begin () const leftvalue {
		return ArrayIterator<CR<ArrayList>> (thiz) ;
	}

	ArrayIterator<CR<ArrayList>> end () const leftvalue {
		return ArrayIterator<CR<ArrayList>> (thiz) ;
	}

	ArrayRange<CR<ArrayList>> iter () const leftvalue {
		return ArrayRange<CR<ArrayList>> (thiz) ;
	}

	void add (CR<A> item) {
		add (move (item)) ;
	}

	void add (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return ArrayListHolder::hold (thiz)->add (move (rax)) ;
	}

	INDEX insert () {
		auto rax = Box<A>::make () ;
		return ArrayListHolder::hold (thiz)->insert (move (rax)) ;
	}

	INDEX insert (CR<INDEX> index) {
		auto rax = Box<A>::make () ;
		return ArrayListHolder::hold (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CR<INDEX> index) {
		return ArrayListHolder::hold (thiz)->remove (index) ;
	}

	void order (CR<Array<INDEX>> range_) {
		return ArrayListHolder::hold (thiz)->order (range_) ;
	}

	void remap () {
		return ArrayListHolder::hold (thiz)->remap () ;
	}
} ;

struct SortedMapNode implement AllocatorNode {
	INDEX mMap ;
	INDEX mDown ;
} ;

struct SortedMapTree {
	Allocator<Pointer ,SortedMapNode> mList ;
	INDEX mCheck ;
} ;

struct SortedMapLayout {
	Ref<SortedMapTree> mThis ;
	INDEX mRoot ;
	RefBuffer<INDEX> mRange ;
	INDEX mWrite ;
	BOOL mRemap ;
} ;

struct SortedMapHolder implement Interface {
	imports VFat<SortedMapHolder> hold (VR<SortedMapLayout> that) ;
	imports CFat<SortedMapHolder> hold (CR<SortedMapLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual SortedMapLayout share () const = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VR<INDEX> at (CR<INDEX> index) leftvalue = 0 ;
	virtual CR<INDEX> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<INDEX> map_) = 0 ;
	virtual INDEX find (CR<Pointer> item) const = 0 ;
	virtual BOOL contain (CR<Pointer> item) const = 0 ;
	virtual INDEX map (CR<Pointer> item) const = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class SortedMapUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		using R1X = UnionPair<A ,SortedMapNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_vptr (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_vptr (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_vptr (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct SortedMapPureLayout implement SortedMapLayout {
public:
	implicit SortedMapPureLayout () noexcept {
		noop (Allocator<A ,SortedMapNode> ()) ;
		SortedMapHolder::hold (thiz)->prepare (SortedMapUnknownBinder<A> ()) ;
	}
} ;

template <>
struct SortedMapPureLayout<Pointer> implement SortedMapLayout {} ;

template <class A>
class SortedMap implement SortedMapPureLayout<A> {
protected:
	using SortedMapPureLayout<A>::mThis ;
	using SortedMapPureLayout<A>::mRoot ;
	using SortedMapPureLayout<A>::mRange ;
	using SortedMapPureLayout<A>::mWrite ;
	using SortedMapPureLayout<A>::mRemap ;

public:
	implicit SortedMap () = default ;

	explicit SortedMap (CR<SizeProxy> size_) {
		SortedMapHolder::hold (thiz)->initialize (SortedMapUnknownBinder<A> () ,size_) ;
	}

	explicit SortedMap (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		SortedMapHolder::hold (thiz)->initialize (SortedMapUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	SortedMap share () const {
		SortedMapLayout ret = SortedMapHolder::hold (thiz)->share () ;
		return move (keep[TYPE<SortedMap>::expr] (ret)) ;
	}

	void clear () {
		return SortedMapHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return SortedMapHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return SortedMapHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return SortedMapHolder::hold (thiz)->length () ;
	}

	VR<INDEX> at (CR<INDEX> index) leftvalue {
		return SortedMapHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<INDEX> operator[] (CR<INDEX> index) leftvalue {
		return at (index) ;
	}

	CR<INDEX> at (CR<INDEX> index) const leftvalue {
		return SortedMapHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<INDEX> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return SortedMapHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SortedMapHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return SortedMapHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<SortedMap>> begin () const leftvalue {
		return ArrayIterator<CR<SortedMap>> (thiz) ;
	}

	ArrayIterator<CR<SortedMap>> end () const leftvalue {
		return ArrayIterator<CR<SortedMap>> (thiz) ;
	}

	ArrayRange<CR<SortedMap>> iter () const leftvalue {
		return ArrayRange<CR<SortedMap>> (thiz) ;
	}

	void add (CR<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RR<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CR<A> item ,CR<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RR<A> item ,CR<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		return SortedMapHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CR<A> item) const {
		return SortedMapHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CR<A> item) const {
		return SortedMapHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CR<A> item) const {
		return SortedMapHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remap () {
		return SortedMapHolder::hold (thiz)->remap () ;
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
	imports VFat<SetHolder> hold (VR<SetLayout> that) ;
	imports CFat<SetHolder> hold (CR<SetLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual void get (CR<INDEX> index ,VR<INDEX> map_) const = 0 ;
	virtual void set (CR<INDEX> index ,CR<INDEX> map_) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<INDEX> map_) = 0 ;
	virtual INDEX find (CR<Pointer> item) const = 0 ;
	virtual BOOL contain (CR<Pointer> item) const = 0 ;
	virtual INDEX map (CR<Pointer> item) const = 0 ;
	virtual void remove (CR<INDEX> index) = 0 ;
	virtual void erase (CR<Pointer> item) = 0 ;
} ;

template <class A>
class SetUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		using R1X = UnionPair<A ,SetNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_vptr (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectComprBinder<A>::expr)
			return inline_vptr (ReflectComprBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_vptr (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct SetPureLayout implement SetLayout {
public:
	implicit SetPureLayout () noexcept {
		noop (Allocator<A ,SetNode> ()) ;
		SetHolder::hold (thiz)->prepare (SetUnknownBinder<A> ()) ;
	}
} ;

template <>
struct SetPureLayout<Pointer> implement SetLayout {} ;

template <class A>
class Set implement SetPureLayout<A> {
protected:
	using SetPureLayout<A>::mSet ;
	using SetPureLayout<A>::mRoot ;
	using SetPureLayout<A>::mTop ;

public:
	implicit Set () = default ;

	explicit Set (CR<SizeProxy> size_) {
		SetHolder::hold (thiz)->initialize (SetUnknownBinder<A> () ,size_) ;
	}

	explicit Set (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		SetHolder::hold (thiz)->initialize (SetUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	void clear () {
		return SetHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return SetHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return SetHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return SetHolder::hold (thiz)->length () ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return SetHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CR<INDEX> index ,VR<INDEX> map_) const {
		return SetHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CR<INDEX> index ,CR<INDEX> map_) {
		return SetHolder::hold (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return SetHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SetHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return SetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<Set>> begin () const leftvalue {
		return ArrayIterator<CR<Set>> (thiz) ;
	}

	ArrayIterator<CR<Set>> end () const leftvalue {
		return ArrayIterator<CR<Set>> (thiz) ;
	}

	ArrayRange<CR<Set>> iter () const leftvalue {
		return ArrayRange<CR<Set>> (thiz) ;
	}

	void add (CR<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RR<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CR<A> item ,CR<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RR<A> item ,CR<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		return SetHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CR<A> item) const {
		return SetHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CR<A> item) const {
		return SetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CR<A> item) const {
		return SetHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CR<INDEX> index) {
		return SetHolder::hold (thiz)->remove (index) ;
	}

	void erase (CR<A> item) {
		return SetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}
} ;

struct HashcodeVisitor {
	BYTE_BASE<VAL> mCode ;
	LENGTH mDepth ;
} ;

struct HashSetNode implement AllocatorNode {
	INDEX mMap ;
	FLAG mHash ;
	INDEX mDown ;
} ;

struct HashSetLayout {
	Allocator<Pointer ,HashSetNode> mSet ;
	RefBuffer<INDEX> mRange ;
	SharedRef<HashcodeVisitor> mVisitor ;
} ;

struct HashSetHolder implement Interface {
	imports VFat<HashSetHolder> hold (VR<HashSetLayout> that) ;
	imports CFat<HashSetHolder> hold (CR<HashSetLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CR<Pointer> at (CR<INDEX> index) const leftvalue = 0 ;
	virtual void get (CR<INDEX> index ,VR<INDEX> map_) const = 0 ;
	virtual void set (CR<INDEX> index ,CR<INDEX> map_) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<INDEX> map_) = 0 ;
	virtual INDEX find (CR<Pointer> item) const = 0 ;
	virtual BOOL contain (CR<Pointer> item) const = 0 ;
	virtual INDEX map (CR<Pointer> item) const = 0 ;
	virtual void remove (CR<INDEX> index) = 0 ;
	virtual void erase (CR<Pointer> item) = 0 ;
} ;

template <class A>
class HashSetUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		using R1X = UnionPair<A ,HashSetNode> ;
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectAssignBinder<A>::expr)
			return inline_vptr (ReflectAssignBinder<A> ()) ;
		if (uuid == ReflectEqualBinder<A>::expr)
			return inline_vptr (ReflectEqualBinder<A> ()) ;
		if (uuid == ReflectVisitBinder<A>::expr)
			return inline_vptr (ReflectVisitBinder<A> ()) ;
		if (uuid == ReflectTupleBinder<R1X>::expr)
			return inline_vptr (ReflectTupleBinder<R1X> ()) ;
		if (uuid == ReflectElementBinder<R1X>::expr)
			return inline_vptr (ReflectElementBinder<R1X> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct HashSetPureLayout implement HashSetLayout {
public:
	implicit HashSetPureLayout () noexcept {
		noop (Allocator<A ,HashSetNode> ()) ;
		HashSetHolder::hold (thiz)->prepare (HashSetUnknownBinder<A> ()) ;
	}
} ;

template <>
struct HashSetPureLayout<Pointer> implement HashSetLayout {} ;

template <class A>
class HashSet implement HashSetPureLayout<A> {
protected:
	using HashSetPureLayout<A>::mSet ;
	using HashSetPureLayout<A>::mRange ;
	using HashSetPureLayout<A>::mVisitor ;

public:
	implicit HashSet () = default ;

	explicit HashSet (CR<SizeProxy> size_) {
		HashSetHolder::hold (thiz)->initialize (HashSetUnknownBinder<A> () ,size_) ;
	}

	explicit HashSet (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		HashSetHolder::hold (thiz)->initialize (HashSetUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	void clear () {
		return HashSetHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return HashSetHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return HashSetHolder::hold (thiz)->step () ;
	}

	LENGTH length () const {
		return HashSetHolder::hold (thiz)->length () ;
	}

	CR<A> at (CR<INDEX> index) const leftvalue {
		return HashSetHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CR<INDEX> index ,VR<INDEX> map_) const {
		return HashSetHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CR<INDEX> index ,CR<INDEX> map_) {
		return HashSetHolder::hold (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return HashSetHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return HashSetHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return HashSetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<HashSet>> begin () const leftvalue {
		return ArrayIterator<CR<HashSet>> (thiz) ;
	}

	ArrayIterator<CR<HashSet>> end () const leftvalue {
		return ArrayIterator<CR<HashSet>> (thiz) ;
	}

	ArrayRange<CR<HashSet>> iter () const leftvalue {
		return ArrayRange<CR<HashSet>> (thiz) ;
	}

	void add (CR<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (RR<A> item) {
		add (move (item) ,NONE) ;
	}

	void add (CR<A> item ,CR<INDEX> map_) {
		add (move (item) ,map_) ;
	}

	void add (RR<A> item ,CR<INDEX> map_) {
		auto rax = Box<A>::make (move (item)) ;
		return HashSetHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CR<A> item) const {
		return HashSetHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CR<A> item) const {
		return HashSetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CR<A> item) const {
		return HashSetHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CR<INDEX> index) {
		return HashSetHolder::hold (thiz)->remove (index) ;
	}

	void erase (CR<A> item) {
		return SetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}
} ;

template <class A>
class BitProxy {
protected:
	XR<A> mThat ;
	INDEX mIndex ;

public:
	implicit BitProxy () = delete ;

	explicit BitProxy (XR<A> that ,CR<INDEX> index) :mThat (that) {
		mIndex = index ;
	}

	forceinline operator BOOL () rightvalue {
		BOOL ret ;
		mThat.get (mIndex ,ret) ;
		return move (ret) ;
	}

	forceinline void operator= (CR<BOOL> that) rightvalue {
		mThat.set (mIndex ,that) ;
	}
} ;

struct BitSetLayout {
	RefBuffer<BYTE> mSet ;
	LENGTH mWidth ;
} ;

struct BitSetHolder implement Interface {
	imports VFat<BitSetHolder> hold (VR<BitSetLayout> that) ;
	imports CFat<BitSetHolder> hold (CR<BitSetLayout> that) ;

	virtual void initialize (CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<LENGTH> size_ ,CR<WrapperLayout> params ,VR<BoxLayout> item) = 0 ;
	virtual void initialize (CR<BitSetLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void get (CR<INDEX> index ,VR<BOOL> item) const = 0 ;
	virtual void set (CR<INDEX> index ,CR<BOOL> item) = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CR<INDEX> index) const = 0 ;
	virtual BOOL equal (CR<BitSetLayout> that) const = 0 ;
	virtual FLAG compr (CR<BitSetLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual BOOL contain (CR<Pointer> item) const = 0 ;
	virtual void erase (CR<Pointer> item) = 0 ;
	virtual void fill (CR<BYTE> item) = 0 ;
	virtual BitSetLayout sand (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout sor (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout sxor (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout ssub (CR<BitSetLayout> that) const = 0 ;
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

	explicit BitSet (CR<SizeProxy> size_) {
		BitSetHolder::hold (thiz)->initialize (size_) ;
	}

	explicit BitSet (CR<SizeProxy> size_ ,CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		BitSetHolder::hold (thiz)->initialize (size_ ,MakeWrapper (that) ,rax) ;
	}

	implicit BitSet (CR<BitSet> that) {
		BitSetHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<BitSet> operator= (CR<BitSet> that) {
		return assign (thiz ,that) ;
	}

	implicit BitSet (RR<BitSet> that) = default ;

	forceinline VR<BitSet> operator= (RR<BitSet> that) = default ;

	BitSet clone () const {
		return move (thiz) ;
	}

	void clear () {
		return BitSetHolder::hold (thiz)->clear () ;
	}

	LENGTH size () const {
		return BitSetHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return BitSetHolder::hold (thiz)->length () ;
	}

	void get (CR<INDEX> index ,VR<BOOL> item) const {
		return BitSetHolder::hold (thiz)->get (index ,item) ;
	}

	void set (CR<INDEX> index ,CR<BOOL> item) {
		return BitSetHolder::hold (thiz)->set (index ,item) ;
	}

	forceinline BitProxy<VR<BitSet>> operator[] (CR<INDEX> index) leftvalue {
		return BitProxy<VR<BitSet>> (thiz ,index) ;
	}

	forceinline BitProxy<CR<BitSet>> operator[] (CR<INDEX> index) const leftvalue {
		return BitProxy<CR<BitSet>> (thiz ,index) ;
	}

	CR<INDEX> at (CR<INDEX> index) const leftvalue {
		return index ;
	}

	INDEX ibegin () const {
		return BitSetHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return BitSetHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CR<INDEX> index) const {
		return BitSetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<BitSet>> begin () const leftvalue {
		return ArrayIterator<CR<BitSet>> (thiz) ;
	}

	ArrayIterator<CR<BitSet>> end () const leftvalue {
		return ArrayIterator<CR<BitSet>> (thiz) ;
	}

	ArrayRange<CR<BitSet>> iter () const leftvalue {
		return ArrayRange<CR<BitSet>> (thiz) ;
	}

	BOOL equal (CR<BitSet> that) const {
		return BitSetHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<BitSet> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<BitSet> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<BitSet> that) const {
		return BitSetHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<BitSet> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<BitSet> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<BitSet> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<BitSet> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return BitSetHolder::hold (thiz)->visit (visitor) ;
	}

	void add (CR<A> item) {
		add (move (item)) ;
	}

	void add (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return BitSetHolder::hold (thiz)->add (move (rax)) ;
	}

	BOOL contain (CR<A> item) const {
		return BitSetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	void erase (CR<A> item) {
		return BitSetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}

	void fill (CR<BYTE> item) {
		return BitSetHolder::hold (thiz)->fill (item) ;
	}

	BitSet sand (CR<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->sand (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator& (CR<BitSet> that) const {
		return sand (that) ;
	}

	forceinline void operator&= (CR<BitSet> that) {
		thiz = sand (that) ;
	}

	BitSet sor (CR<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->sor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator| (CR<BitSet> that) const {
		return sor (that) ;
	}

	forceinline void operator|= (CR<BitSet> that) {
		thiz = sor (that) ;
	}

	BitSet sxor (CR<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->sxor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator^ (CR<BitSet> that) const {
		return sxor (that) ;
	}

	forceinline void operator^= (CR<BitSet> that) {
		thiz = sxor (that) ;
	}

	BitSet ssub (CR<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator- (CR<BitSet> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<BitSet> that) {
		thiz = ssub (that) ;
	}

	BitSet snot () const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->snot () ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator~ () const {
		return snot () ;
	}
} ;
} ;