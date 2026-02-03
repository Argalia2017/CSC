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
class SizeProxy implement Proxy {
protected:
	Length mSize ;

public:
	implicit SizeProxy () = delete ;

	implicit SizeProxy (CR<Length> size_) {
		mSize = size_ ;
	}

	forceinline operator Length () const {
		return mSize ;
	}
} ;

template <class A>
class ArrayIterator implement Proxy {
private:
	using ITEM = decltype (nullof (A).at (0)) ;

protected:
	XR<A> mThat ;
	Index mBegin ;
	Index mEnd ;
	Index mPeek ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (XR<A> that) :mThat (that) {
		mBegin = mThat.ibegin () ;
		mEnd = mThat.iend () ;
		mPeek = mBegin ;
	}

	Length length () const {
		return mThat.length () ;
	}

	ArrayIterator begin () const {
		return thiz ;
	}

	ArrayIterator end () const {
		return thiz ;
	}

	Bool good () const {
		return mPeek != mEnd ;
	}

	forceinline Bool operator== (CR<ArrayIterator>) const {
		return (!good ()) ;
	}

	forceinline Bool operator!= (CR<ArrayIterator>) const {
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
class ArrayRange implement Proxy {
protected:
	XR<A> mThat ;

public:
	implicit ArrayRange () = delete ;

	explicit ArrayRange (XR<A> that) :mThat (that) {}

	Length length () const {
		return mThat.length () ;
	}

	CR<Index> at (CR<Index> index) const leftvalue {
		return index ;
	}

	Index ibegin () const {
		return mThat.ibegin () ;
	}

	Index iend () const {
		return mThat.iend () ;
	}

	Index inext (CR<Index> index) const {
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
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void initialize (CR<ArrayLayout> that) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual Bool equal (CR<ArrayLayout> that) const = 0 ;
	virtual Flag compr (CR<ArrayLayout> that) const = 0 ;
	virtual void visit (CR<Visitor> visitor) const = 0 ;
	virtual void fill (CR<Pointer> item) = 0 ;
	virtual void splice (CR<Index> index ,CR<ArrayLayout> item) = 0 ;
} ;

template <class A>
class ArrayUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct ArrayImplLayout implement ArrayLayout {
public:
	implicit ArrayImplLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		ArrayHolder::hold (thiz)->prepare (ArrayUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ArrayImplLayout<Pointer> implement ArrayLayout {} ;

template <class A>
class Array implement ArrayImplLayout<A> {
protected:
	using ArrayImplLayout<A>::mArray ;

public:
	implicit Array () = default ;

	explicit Array (CR<SizeProxy> size_) {
		ArrayHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
	}

	explicit Array (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	template <class ARG1>
	static Array make (CR<ARG1> iterator) {
		Array ret = Array (iterator.length ()) ;
		Index ix = 0 ;
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

	Length size () const {
		return ArrayHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return ArrayHolder::hold (thiz)->step () ;
	}

	Length length () const {
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

	VR<A> at (CR<Index> index) leftvalue {
		return ArrayHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return ArrayHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index ibegin () const {
		return ArrayHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return ArrayHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return ArrayHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<Array>> begin () leftvalue {
		return ArrayIterator<VR<Array>> (thiz) ;
	}

	ArrayIterator<CR<Array>> begin () const leftvalue {
		return ArrayIterator<CR<Array>> (thiz) ;
	}

	ArrayIterator<VR<Array>> end () leftvalue {
		return ArrayIterator<VR<Array>> (thiz) ;
	}

	ArrayIterator<CR<Array>> end () const leftvalue {
		return ArrayIterator<CR<Array>> (thiz) ;
	}

	ArrayRange<CR<Array>> iter () const leftvalue {
		return ArrayRange<CR<Array>> (thiz) ;
	}

	Bool equal (CR<Array> that) const {
		return ArrayHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<Array> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<Array> that) const {
		return (!equal (that)) ;
	}

	Flag compr (CR<Array> that) const {
		return ArrayHolder::hold (thiz)->compr (that) ;
	}

	forceinline Bool operator< (CR<Array> that) const {
		return compr (that) < ZERO ;
	}

	forceinline Bool operator<= (CR<Array> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline Bool operator> (CR<Array> that) const {
		return compr (that) > ZERO ;
	}

	forceinline Bool operator>= (CR<Array> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CR<Visitor> visitor) const {
		return ArrayHolder::hold (thiz)->visit (visitor) ;
	}

	void fill (CR<A> item) {
		return ArrayHolder::hold (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CR<Index> index ,CR<Array> item) {
		return ArrayHolder::hold (thiz)->splice (index ,item) ;
	}
} ;

template <class A>
class StringParse ;

template <class A>
class StringBuild ;

struct StringEncode {
	enum {
		ANSI ,
		UTF8 ,
		UTF16LE ,
		UTF16BE ,
		UTF32LE ,
		UTF32BE ,
		ETC
	} ;
} ;

struct StringLayout {
	RefBuffer<Pointer> mString ;
	Flag mEncode ;
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> hold (VR<StringLayout> that) ;
	imports CFat<StringHolder> hold (CR<StringLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Slice> that ,CR<Length> step_) = 0 ;
	virtual void initialize (CR<Length> size_ ,CR<Length> step_) = 0 ;
	virtual void initialize (CR<StringLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual Flag encode () const = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual Ref<RefBuffer<Byte>> borrow () leftvalue = 0 ;
	virtual Ref<RefBuffer<Byte>> borrow () const leftvalue = 0 ;
	virtual void get (CR<Index> index ,VR<Stru32> item) const = 0 ;
	virtual void set (CR<Index> index ,CR<Stru32> item) = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual Bool equal (CR<Slice> that) const = 0 ;
	virtual Bool equal (CR<StringLayout> that) const = 0 ;
	virtual Flag compr (CR<Slice> that) const = 0 ;
	virtual Flag compr (CR<StringLayout> that) const = 0 ;
	virtual void visit (CR<Visitor> visitor) const = 0 ;
	virtual void trunc (CR<Index> index) = 0 ;
	virtual void fill (CR<Stru32> item) = 0 ;
	virtual void splice (CR<Index> index ,CR<Slice> item) = 0 ;
	virtual void splice (CR<Index> index ,CR<StringLayout> item) = 0 ;
	virtual Slice segment (CR<Index> begin_ ,CR<Index> end_) const = 0 ;
	virtual Array<Slice> split (CR<Stru32> delim) const = 0 ;
	virtual void replace (CR<Stru32> from ,CR<Stru32> into) = 0 ;
	virtual void trim (CR<Slice> list) = 0 ;
} ;

template <class A>
class StringUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct StringImplLayout implement StringLayout {
public:
	implicit StringImplLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		StringHolder::hold (thiz)->prepare (StringUnknownBinder<A> ()) ;
	}
} ;

template <>
struct StringImplLayout<Pointer> implement StringLayout {} ;

template <class A>
class String implement StringImplLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	using StringImplLayout<A>::mString ;
	using StringImplLayout<A>::mEncode ;

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

	Flag encode () const {
		return StringHolder::hold (thiz)->encode () ;
	}

	Length size () const {
		return StringHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return StringHolder::hold (thiz)->step () ;
	}

	Length length () const {
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

	Ref<RefBuffer<Byte>> borrow () leftvalue {
		return StringHolder::hold (thiz)->borrow () ;
	}

	Ref<RefBuffer<Byte>> borrow () const leftvalue {
		return StringHolder::hold (thiz)->borrow () ;
	}

	VR<A> at (CR<Index> index) leftvalue {
		return StringHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return StringHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	void get (CR<Index> index ,VR<Stru32> map_) const {
		return StringHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CR<Index> index ,CR<Stru32> map_) {
		return StringHolder::hold (thiz)->set (index ,map_) ;
	}

	Index ibegin () const {
		return StringHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return StringHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return StringHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<String>> begin () leftvalue {
		return ArrayIterator<VR<String>> (thiz) ;
	}

	ArrayIterator<CR<String>> begin () const leftvalue {
		return ArrayIterator<CR<String>> (thiz) ;
	}

	ArrayIterator<VR<String>> end () leftvalue {
		return ArrayIterator<VR<String>> (thiz) ;
	}

	ArrayIterator<CR<String>> end () const leftvalue {
		return ArrayIterator<CR<String>> (thiz) ;
	}

	ArrayRange<CR<String>> iter () const leftvalue {
		return ArrayRange<CR<String>> (thiz) ;
	}

	Bool equal (CR<Slice> that) const {
		return StringHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<Slice> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<Slice> that) const {
		return (!equal (that)) ;
	}

	Bool equal (CR<String> that) const {
		return StringHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<String> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<String> that) const {
		return (!equal (that)) ;
	}

	Flag compr (CR<String> that) const {
		return StringHolder::hold (thiz)->compr (that) ;
	}

	forceinline Bool operator< (CR<String> that) const {
		return compr (that) < ZERO ;
	}

	forceinline Bool operator<= (CR<String> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline Bool operator> (CR<String> that) const {
		return compr (that) > ZERO ;
	}

	forceinline Bool operator>= (CR<String> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CR<Visitor> visitor) const {
		return StringHolder::hold (thiz)->visit (visitor) ;
	}

	void trunc (CR<Index> index) {
		return StringHolder::hold (thiz)->trunc (index) ;
	}

	void fill (CR<Stru32> item) {
		return StringHolder::hold (thiz)->fill (item) ;
	}

	void splice (CR<Index> index ,CR<Slice> item) {
		return StringHolder::hold (thiz)->splice (index ,item) ;
	}

	void splice (CR<Index> index ,CR<String> item) {
		return StringHolder::hold (thiz)->splice (index ,item) ;
	}

	Slice segment () const {
		return segment (0 ,length ()) ;
	}

	Slice segment (CR<Index> begin_ ,CR<Index> end_) const {
		return StringHolder::hold (thiz)->segment (begin_ ,end_) ;
	}

	Array<Slice> split (CR<Stru32> delim) const {
		return StringHolder::hold (thiz)->split (delim) ;
	}

	void replace (CR<Stru32> from ,CR<Stru32> into) {
		return StringHolder::hold (thiz)->replace (from ,into) ;
	}

	void trim (CR<Slice> list) {
		return StringHolder::hold (thiz)->trim (list) ;
	}
} ;

struct DequeLayout {
	RefBuffer<Pointer> mDeque ;
	Index mRead ;
	Index mWrite ;
} ;

struct DequeHolder implement Interface {
	imports VFat<DequeHolder> hold (VR<DequeLayout> that) ;
	imports CFat<DequeHolder> hold (CR<DequeLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual Bool empty () const = 0 ;
	virtual Bool full () const = 0 ;
	virtual Index head () const = 0 ;
	virtual Index tail () const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RR<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual void ring (CR<Length> count) = 0 ;
} ;

template <class A>
class DequeUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct DequeImplLayout implement DequeLayout {
public:
	implicit DequeImplLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		DequeHolder::hold (thiz)->prepare (DequeUnknownBinder<A> ()) ;
	}
} ;

template <>
struct DequeImplLayout<Pointer> implement DequeLayout {} ;

template <class A>
class Deque implement DequeImplLayout<A> {
protected:
	using DequeImplLayout<A>::mDeque ;
	using DequeImplLayout<A>::mRead ;
	using DequeImplLayout<A>::mWrite ;

public:
	implicit Deque () = default ;

	explicit Deque (CR<SizeProxy> size_) {
		DequeHolder::hold (thiz)->initialize (DequeUnknownBinder<A> () ,size_) ;
	}

	explicit Deque (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		DequeHolder::hold (thiz)->initialize (DequeUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	void clear () {
		return DequeHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return DequeHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return DequeHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return DequeHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<Index> index) leftvalue {
		return DequeHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return DequeHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index ibegin () const {
		return DequeHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return DequeHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return DequeHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<Deque>> begin () leftvalue {
		return ArrayIterator<VR<Deque>> (thiz) ;
	}

	ArrayIterator<CR<Deque>> begin () const leftvalue {
		return ArrayIterator<CR<Deque>> (thiz) ;
	}

	ArrayIterator<VR<Deque>> end () leftvalue {
		return ArrayIterator<VR<Deque>> (thiz) ;
	}

	ArrayIterator<CR<Deque>> end () const leftvalue {
		return ArrayIterator<CR<Deque>> (thiz) ;
	}

	ArrayRange<CR<Deque>> iter () const leftvalue {
		return ArrayRange<CR<Deque>> (thiz) ;
	}

	Bool empty () const {
		return DequeHolder::hold (thiz)->empty () ;
	}

	Bool full () const {
		return DequeHolder::hold (thiz)->full () ;
	}

	Index head () const {
		return DequeHolder::hold (thiz)->head () ;
	}

	Index tail () const {
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

	void ring (CR<Length> count) {
		return DequeHolder::hold (thiz)->ring (count) ;
	}
} ;

template <class A>
struct IndexPair implement Tuple<A ,Index> {
public:
	Bool equal (CR<IndexPair> that) const {
		return inline_equal (thiz.m1st ,that.m1st) ;
	}

	forceinline Bool operator== (CR<IndexPair> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<IndexPair> that) const {
		return (!equal (that)) ;
	}

	Flag compr (CR<IndexPair> that) const {
		return inline_compr (thiz.m1st ,that.m1st) ;
	}

	forceinline Bool operator< (CR<IndexPair> that) const {
		return compr (that) < ZERO ;
	}

	forceinline Bool operator<= (CR<IndexPair> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline Bool operator> (CR<IndexPair> that) const {
		return compr (that) > ZERO ;
	}

	forceinline Bool operator>= (CR<IndexPair> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CR<Visitor> visitor) const {
		visitor.enter () ;
		inline_visit (visitor ,thiz.m1st) ;
		visitor.leave () ;
	}
} ;

struct PriorityLayout {
	RefBuffer<Pointer> mPriority ;
	Index mRead ;
	Index mWrite ;
} ;

struct PriorityHolder implement Interface {
	imports VFat<PriorityHolder> hold (VR<PriorityLayout> that) ;
	imports CFat<PriorityHolder> hold (CR<PriorityLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual Bool empty () const = 0 ;
	virtual Bool full () const = 0 ;
	virtual Index head () const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
} ;

template <class A>
class PriorityUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
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
		if (uuid == ReflectElementBinder<A>::expr)
			return inline_vptr (ReflectElementBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
struct PriorityImplLayout implement PriorityLayout {
public:
	implicit PriorityImplLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		PriorityHolder::hold (thiz)->prepare (PriorityUnknownBinder<A> ()) ;
	}
} ;

template <>
struct PriorityImplLayout<Pointer> implement PriorityLayout {} ;

template <class A>
class Priority implement PriorityImplLayout<A> {
protected:
	using PriorityImplLayout<A>::mPriority ;
	using PriorityImplLayout<A>::mRead ;
	using PriorityImplLayout<A>::mWrite ;

public:
	implicit Priority () = default ;

	explicit Priority (CR<SizeProxy> size_) {
		PriorityHolder::hold (thiz)->initialize (PriorityUnknownBinder<A> () ,size_) ;
	}

	explicit Priority (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		PriorityHolder::hold (thiz)->initialize (PriorityUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	void clear () {
		return PriorityHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return PriorityHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return PriorityHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return PriorityHolder::hold (thiz)->length () ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return PriorityHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index ibegin () const {
		return PriorityHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return PriorityHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return PriorityHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CR<Priority>> beign () const leftvalue {
		return ArrayIterator<CR<Priority>> (thiz) ;
	}

	ArrayIterator<VR<Priority>> end () leftvalue {
		return ArrayIterator<VR<Priority>> (thiz) ;
	}

	ArrayIterator<CR<Priority>> end () const leftvalue {
		return ArrayIterator<CR<Priority>> (thiz) ;
	}

	ArrayRange<CR<Priority>> iter () const leftvalue {
		return ArrayRange<CR<Priority>> (thiz) ;
	}

	Bool empty () const {
		return PriorityHolder::hold (thiz)->empty () ;
	}

	Bool full () const {
		return PriorityHolder::hold (thiz)->full () ;
	}

	Index head () const {
		return PriorityHolder::hold (thiz)->head () ;
	}

	void add (CR<A> item) {
		add (move (item)) ;
	}

	void add (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return PriorityHolder::hold (thiz)->add (move (rax)) ;
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
	Index mLeft ;
	Index mRight ;
} ;

struct ListLayout {
	Allocator<Pointer ,ListNode> mList ;
	Index mFirst ;
	Index mLast ;
} ;

struct ListHolder implement Interface {
	imports VFat<ListHolder> hold (VR<ListLayout> that) ;
	imports CFat<ListHolder> hold (CR<ListLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual Bool empty () const = 0 ;
	virtual Bool full () const = 0 ;
	virtual Index head () const = 0 ;
	virtual Index tail () const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RR<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual Index insert (RR<BoxLayout> item) = 0 ;
	virtual Index insert (CR<Index> index ,RR<BoxLayout> item) = 0 ;
	virtual void remove (CR<Index> index) = 0 ;
	virtual void order (CR<Array<Index>> range_) = 0 ;
} ;

template <class A>
class ListUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct ListImplLayout implement ListLayout {
public:
	implicit ListImplLayout () noexcept {
		noop (Allocator<A ,ListNode> ()) ;
		ListHolder::hold (thiz)->prepare (ListUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ListImplLayout<Pointer> implement ListLayout {} ;

template <class A>
class List implement ListImplLayout<A> {
protected:
	using ListImplLayout<A>::mList ;
	using ListImplLayout<A>::mFirst ;
	using ListImplLayout<A>::mLast ;

public:
	implicit List () = default ;

	explicit List (CR<SizeProxy> size_) {
		ListHolder::hold (thiz)->initialize (ListUnknownBinder<A> () ,size_) ;
	}

	explicit List (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ListHolder::hold (thiz)->initialize (ListUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	void clear () {
		return ListHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return ListHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return ListHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return ListHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<Index> index) leftvalue {
		return ListHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return ListHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index ibegin () const {
		return ListHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return ListHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return ListHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<List>> begin () leftvalue {
		return ArrayIterator<VR<List>> (thiz) ;
	}

	ArrayIterator<CR<List>> begin () const leftvalue {
		return ArrayIterator<CR<List>> (thiz) ;
	}

	ArrayIterator<VR<List>> end () leftvalue {
		return ArrayIterator<VR<List>> (thiz) ;
	}

	ArrayIterator<CR<List>> end () const leftvalue {
		return ArrayIterator<CR<List>> (thiz) ;
	}

	ArrayRange<CR<List>> iter () const leftvalue {
		return ArrayRange<CR<List>> (thiz) ;
	}

	Bool empty () const {
		return ListHolder::hold (thiz)->empty () ;
	}

	Bool full () const {
		return ListHolder::hold (thiz)->full () ;
	}

	Index head () const {
		return ListHolder::hold (thiz)->head () ;
	}

	Index tail () const {
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

	Index insert () {
		auto rax = Box<A>::make () ;
		return ListHolder::hold (thiz)->insert (move (rax)) ;
	}

	Index insert (CR<Index> index) {
		auto rax = Box<A>::make () ;
		return ListHolder::hold (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CR<Index> index) {
		return ListHolder::hold (thiz)->remove (index) ;
	}

	void order (CR<Array<Index>> range_) {
		return ListHolder::hold (thiz)->order (range_) ;
	}
} ;

struct ArrayListNode implement AllocatorNode {} ;

struct ArrayListLayout {
	Allocator<Pointer ,ArrayListNode> mList ;
	RefBuffer<Index> mRange ;
	Index mTop ;
	Bool mRemap ;
} ;

struct ArrayListHolder implement Interface {
	imports VFat<ArrayListHolder> hold (VR<ArrayListLayout> that) ;
	imports CFat<ArrayListHolder> hold (CR<ArrayListLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Pointer> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual Index insert (RR<BoxLayout> item) = 0 ;
	virtual Index insert (CR<Index> index ,RR<BoxLayout> item) = 0 ;
	virtual void remove (CR<Index> index) = 0 ;
	virtual void order (CR<Array<Index>> range_) = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class ArrayListUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct ArrayListImplLayout implement ArrayListLayout {
public:
	implicit ArrayListImplLayout () noexcept {
		noop (Allocator<A ,ArrayListNode> ()) ;
		ArrayListHolder::hold (thiz)->prepare (ArrayListUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ArrayListImplLayout<Pointer> implement ArrayListLayout {} ;

template <class A>
class ArrayList implement ArrayListImplLayout<A> {
protected:
	using ArrayListImplLayout<A>::mList ;
	using ArrayListImplLayout<A>::mRange ;
	using ArrayListImplLayout<A>::mTop ;
	using ArrayListImplLayout<A>::mRemap ;

public:
	implicit ArrayList () = default ;

	explicit ArrayList (CR<SizeProxy> size_) {
		ArrayListHolder::hold (thiz)->initialize (ArrayListUnknownBinder<A> () ,size_) ;
	}

	explicit ArrayList (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayListHolder::hold (thiz)->initialize (ArrayListUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	void clear () {
		return ArrayListHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return ArrayListHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return ArrayListHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return ArrayListHolder::hold (thiz)->length () ;
	}

	VR<A> at (CR<Index> index) leftvalue {
		return ArrayListHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<A> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return ArrayListHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index ibegin () const {
		return ArrayListHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return ArrayListHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return ArrayListHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<ArrayList>> begin () leftvalue {
		return ArrayIterator<VR<ArrayList>> (thiz) ;
	}

	ArrayIterator<CR<ArrayList>> begin () const leftvalue {
		return ArrayIterator<CR<ArrayList>> (thiz) ;
	}

	ArrayIterator<VR<ArrayList>> end () leftvalue {
		return ArrayIterator<VR<ArrayList>> (thiz) ;
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

	Index insert () {
		auto rax = Box<A>::make () ;
		return ArrayListHolder::hold (thiz)->insert (move (rax)) ;
	}

	Index insert (CR<Index> index) {
		auto rax = Box<A>::make () ;
		return ArrayListHolder::hold (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CR<Index> index) {
		return ArrayListHolder::hold (thiz)->remove (index) ;
	}

	void order (CR<Array<Index>> range_) {
		return ArrayListHolder::hold (thiz)->order (range_) ;
	}

	void remap () {
		return ArrayListHolder::hold (thiz)->remap () ;
	}
} ;

struct SortedMapNode implement AllocatorNode {
	Index mMap ;
	Index mDown ;
} ;

struct SortedMapTree {
	Allocator<Pointer ,SortedMapNode> mList ;
	Index mCheck ;
} ;

struct SortedMapLayout {
	Ref<SortedMapTree> mThis ;
	Index mRoot ;
	RefBuffer<Index> mRange ;
	Index mWrite ;
	Bool mRemap ;
} ;

struct SortedMapHolder implement Interface {
	imports VFat<SortedMapHolder> hold (VR<SortedMapLayout> that) ;
	imports CFat<SortedMapHolder> hold (CR<SortedMapLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual SortedMapLayout share () const = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual VR<Index> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Index> at (CR<Index> index) const leftvalue = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<Index> map_) = 0 ;
	virtual Index find (CR<Pointer> item) const = 0 ;
	virtual Bool contain (CR<Pointer> item) const = 0 ;
	virtual Index map (CR<Pointer> item) const = 0 ;
	virtual void remap () = 0 ;
} ;

template <class A>
class SortedMapUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct SortedMapImplLayout implement SortedMapLayout {
public:
	implicit SortedMapImplLayout () noexcept {
		noop (Allocator<A ,SortedMapNode> ()) ;
		SortedMapHolder::hold (thiz)->prepare (SortedMapUnknownBinder<A> ()) ;
	}
} ;

template <>
struct SortedMapImplLayout<Pointer> implement SortedMapLayout {} ;

template <class A>
class SortedMap implement SortedMapImplLayout<A> {
protected:
	using SortedMapImplLayout<A>::mThis ;
	using SortedMapImplLayout<A>::mRoot ;
	using SortedMapImplLayout<A>::mRange ;
	using SortedMapImplLayout<A>::mWrite ;
	using SortedMapImplLayout<A>::mRemap ;

public:
	implicit SortedMap () = default ;

	explicit SortedMap (CR<SizeProxy> size_) {
		SortedMapHolder::hold (thiz)->initialize (SortedMapUnknownBinder<A> () ,size_) ;
	}

	explicit SortedMap (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		SortedMapHolder::hold (thiz)->initialize (SortedMapUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	SortedMap share () const {
		SortedMapLayout ret = SortedMapHolder::hold (thiz)->share () ;
		return move (keep[TYPE<SortedMap>::expr] (ret)) ;
	}

	void clear () {
		return SortedMapHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return SortedMapHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return SortedMapHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return SortedMapHolder::hold (thiz)->length () ;
	}

	VR<Index> at (CR<Index> index) leftvalue {
		return SortedMapHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<Index> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<Index> at (CR<Index> index) const leftvalue {
		return SortedMapHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<Index> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	Index ibegin () const {
		return SortedMapHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return SortedMapHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return SortedMapHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<SortedMap>> begin () leftvalue {
		return ArrayIterator<VR<SortedMap>> (thiz) ;
	}

	ArrayIterator<CR<SortedMap>> begin () const leftvalue {
		return ArrayIterator<CR<SortedMap>> (thiz) ;
	}

	ArrayIterator<VR<SortedMap>> end () leftvalue {
		return ArrayIterator<VR<SortedMap>> (thiz) ;
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

	void add (CR<A> item ,CR<Index> map_) {
		add (move (item) ,map_) ;
	}

	void add (RR<A> item ,CR<Index> map_) {
		auto rax = Box<A>::make (move (item)) ;
		return SortedMapHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	Index find (CR<A> item) const {
		return SortedMapHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	Bool contain (CR<A> item) const {
		return SortedMapHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	Index map (CR<A> item) const {
		return SortedMapHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remap () {
		return SortedMapHolder::hold (thiz)->remap () ;
	}
} ;

struct SetNode implement AllocatorNode {
	Index mMap ;
	Bool mRed ;
	Bool mBin ;
	Index mUp ;
	Index mLeft ;
	Index mRight ;
} ;

struct SetLayout {
	Allocator<Pointer ,SetNode> mSet ;
	Index mRoot ;
	Index mTop ;
} ;

struct SetHolder implement Interface {
	imports VFat<SetHolder> hold (VR<SetLayout> that) ;
	imports CFat<SetHolder> hold (CR<SetLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual void get (CR<Index> index ,VR<Index> map_) const = 0 ;
	virtual void set (CR<Index> index ,CR<Index> map_) = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<Index> map_) = 0 ;
	virtual Index find (CR<Pointer> item) const = 0 ;
	virtual Bool contain (CR<Pointer> item) const = 0 ;
	virtual Index map (CR<Pointer> item) const = 0 ;
	virtual void remove (CR<Index> index) = 0 ;
	virtual void erase (CR<Pointer> item) = 0 ;
} ;

template <class A>
class SetUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct SetImplLayout implement SetLayout {
public:
	implicit SetImplLayout () noexcept {
		noop (Allocator<A ,SetNode> ()) ;
		SetHolder::hold (thiz)->prepare (SetUnknownBinder<A> ()) ;
	}
} ;

template <>
struct SetImplLayout<Pointer> implement SetLayout {} ;

template <class A>
class Set implement SetImplLayout<A> {
protected:
	using SetImplLayout<A>::mSet ;
	using SetImplLayout<A>::mRoot ;
	using SetImplLayout<A>::mTop ;

public:
	implicit Set () = default ;

	explicit Set (CR<SizeProxy> size_) {
		SetHolder::hold (thiz)->initialize (SetUnknownBinder<A> () ,size_) ;
	}

	explicit Set (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		SetHolder::hold (thiz)->initialize (SetUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	void clear () {
		return SetHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return SetHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return SetHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return SetHolder::hold (thiz)->length () ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return SetHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	void get (CR<Index> index ,VR<Index> map_) const {
		return SetHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CR<Index> index ,CR<Index> map_) {
		return SetHolder::hold (thiz)->set (index ,map_) ;
	}

	Index ibegin () const {
		return SetHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return SetHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return SetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<Set>> begin () leftvalue {
		return ArrayIterator<VR<Set>> (thiz) ;
	}

	ArrayIterator<CR<Set>> begin () const leftvalue {
		return ArrayIterator<CR<Set>> (thiz) ;
	}

	ArrayIterator<VR<Set>> end () leftvalue {
		return ArrayIterator<VR<Set>> (thiz) ;
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

	void add (CR<A> item ,CR<Index> map_) {
		add (move (item) ,map_) ;
	}

	void add (RR<A> item ,CR<Index> map_) {
		auto rax = Box<A>::make (move (item)) ;
		return SetHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	Index find (CR<A> item) const {
		return SetHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	Bool contain (CR<A> item) const {
		return SetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	Index map (CR<A> item) const {
		return SetHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CR<Index> index) {
		return SetHolder::hold (thiz)->remove (index) ;
	}

	void erase (CR<A> item) {
		return SetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}
} ;

struct HashcodeVisitorLayout {
	BYTE_BASE<Val> mCode ;
	Length mDepth ;
} ;

struct HashcodeVisitorHolder implement VisitorHolder {
	imports VFat<HashcodeVisitorHolder> hold (VR<HashcodeVisitorLayout> that) ;
	imports CFat<HashcodeVisitorHolder> hold (CR<HashcodeVisitorLayout> that) ;
} ;

class HashcodeVisitor implement HashcodeVisitorLayout {
public:
	implicit HashcodeVisitor () = default ;

	implicit operator Visitor () leftvalue {
		return HashcodeVisitorHolder::hold (thiz) ;
	}
} ;

struct HashSetNode implement AllocatorNode {
	Index mMap ;
	Flag mHash ;
	Index mDown ;
} ;

struct HashSetLayout {
	Allocator<Pointer ,HashSetNode> mSet ;
	RefBuffer<Index> mRange ;
	SharedRef<HashcodeVisitor> mVisitor ;
} ;

struct HashSetHolder implement Interface {
	imports VFat<HashSetHolder> hold (VR<HashSetLayout> that) ;
	imports CFat<HashSetHolder> hold (CR<HashSetLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length length () const = 0 ;
	virtual CR<Pointer> at (CR<Index> index) const leftvalue = 0 ;
	virtual void get (CR<Index> index ,VR<Index> map_) const = 0 ;
	virtual void set (CR<Index> index ,CR<Index> map_) = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual void add (RR<BoxLayout> item ,CR<Index> map_) = 0 ;
	virtual Index find (CR<Pointer> item) const = 0 ;
	virtual Bool contain (CR<Pointer> item) const = 0 ;
	virtual Index map (CR<Pointer> item) const = 0 ;
	virtual void remove (CR<Index> index) = 0 ;
	virtual void erase (CR<Pointer> item) = 0 ;
} ;

template <class A>
class HashSetUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
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
struct HashSetImplLayout implement HashSetLayout {
public:
	implicit HashSetImplLayout () noexcept {
		noop (Allocator<A ,HashSetNode> ()) ;
		HashSetHolder::hold (thiz)->prepare (HashSetUnknownBinder<A> ()) ;
	}
} ;

template <>
struct HashSetImplLayout<Pointer> implement HashSetLayout {} ;

template <class A>
class HashSet implement HashSetImplLayout<A> {
protected:
	using HashSetImplLayout<A>::mSet ;
	using HashSetImplLayout<A>::mRange ;
	using HashSetImplLayout<A>::mVisitor ;

public:
	implicit HashSet () = default ;

	explicit HashSet (CR<SizeProxy> size_) {
		HashSetHolder::hold (thiz)->initialize (HashSetUnknownBinder<A> () ,size_) ;
	}

	explicit HashSet (CR<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		HashSetHolder::hold (thiz)->initialize (HashSetUnknownBinder<A> () ,Pointer::from (that) ,rax) ;
	}

	void clear () {
		return HashSetHolder::hold (thiz)->clear () ;
	}

	Length size () const {
		return HashSetHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return HashSetHolder::hold (thiz)->step () ;
	}

	Length length () const {
		return HashSetHolder::hold (thiz)->length () ;
	}

	CR<A> at (CR<Index> index) const leftvalue {
		return HashSetHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<A> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	void get (CR<Index> index ,VR<Index> map_) const {
		return HashSetHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CR<Index> index ,CR<Index> map_) {
		return HashSetHolder::hold (thiz)->set (index ,map_) ;
	}

	Index ibegin () const {
		return HashSetHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return HashSetHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return HashSetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<HashSet>> begin () leftvalue {
		return ArrayIterator<VR<HashSet>> (thiz) ;
	}

	ArrayIterator<CR<HashSet>> begin () const leftvalue {
		return ArrayIterator<CR<HashSet>> (thiz) ;
	}

	ArrayIterator<VR<HashSet>> end () leftvalue {
		return ArrayIterator<VR<HashSet>> (thiz) ;
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

	void add (CR<A> item ,CR<Index> map_) {
		add (move (item) ,map_) ;
	}

	void add (RR<A> item ,CR<Index> map_) {
		auto rax = Box<A>::make (move (item)) ;
		return HashSetHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	Index find (CR<A> item) const {
		return HashSetHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	Bool contain (CR<A> item) const {
		return HashSetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	Index map (CR<A> item) const {
		return HashSetHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CR<Index> index) {
		return HashSetHolder::hold (thiz)->remove (index) ;
	}

	void erase (CR<A> item) {
		return SetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}
} ;

template <class A>
class BitProxy implement Proxy {
protected:
	XR<A> mThat ;
	Index mIndex ;

public:
	implicit BitProxy () = delete ;

	explicit BitProxy (XR<A> that ,CR<Index> index) :mThat (that) {
		mIndex = index ;
	}

	forceinline operator Bool () rightvalue {
		Bool ret ;
		mThat.get (mIndex ,ret) ;
		return move (ret) ;
	}

	forceinline void operator= (CR<Bool> that) rightvalue {
		mThat.set (mIndex ,that) ;
	}
} ;

struct BitSetLayout {
	RefBuffer<Byte> mSet ;
	Length mWidth ;
} ;

struct BitSetHolder implement Interface {
	imports VFat<BitSetHolder> hold (VR<BitSetLayout> that) ;
	imports CFat<BitSetHolder> hold (CR<BitSetLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void initialize (CR<Length> size_ ,CR<Pointer> params ,VR<BoxLayout> item) = 0 ;
	virtual void initialize (CR<BitSetLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual Length size () const = 0 ;
	virtual Length length () const = 0 ;
	virtual void get (CR<Index> index ,VR<Bool> item) const = 0 ;
	virtual void set (CR<Index> index ,CR<Bool> item) = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual Bool equal (CR<BitSetLayout> that) const = 0 ;
	virtual Flag compr (CR<BitSetLayout> that) const = 0 ;
	virtual void visit (CR<Visitor> visitor) const = 0 ;
	virtual void add (RR<BoxLayout> item) = 0 ;
	virtual Bool contain (CR<Pointer> item) const = 0 ;
	virtual void erase (CR<Pointer> item) = 0 ;
	virtual void fill (CR<Byte> item) = 0 ;
	virtual BitSetLayout sand (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout sor (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout sxor (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout ssub (CR<BitSetLayout> that) const = 0 ;
	virtual BitSetLayout snot () const = 0 ;
} ;

class BitSet implement BitSetLayout {
private:
	using A = Index ;

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
		BitSetHolder::hold (thiz)->initialize (size_ ,Pointer::from (that) ,rax) ;
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

	Length size () const {
		return BitSetHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return BitSetHolder::hold (thiz)->length () ;
	}

	void get (CR<Index> index ,VR<Bool> item) const {
		return BitSetHolder::hold (thiz)->get (index ,item) ;
	}

	void set (CR<Index> index ,CR<Bool> item) {
		return BitSetHolder::hold (thiz)->set (index ,item) ;
	}

	forceinline BitProxy<VR<BitSet>> operator[] (CR<Index> index) leftvalue {
		return BitProxy<VR<BitSet>> (thiz ,index) ;
	}

	forceinline BitProxy<CR<BitSet>> operator[] (CR<Index> index) const leftvalue {
		return BitProxy<CR<BitSet>> (thiz ,index) ;
	}

	CR<Index> at (CR<Index> index) const leftvalue {
		return index ;
	}

	Index ibegin () const {
		return BitSetHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return BitSetHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return BitSetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<VR<BitSet>> begin () leftvalue {
		return ArrayIterator<VR<BitSet>> (thiz) ;
	}

	ArrayIterator<CR<BitSet>> begin () const leftvalue {
		return ArrayIterator<CR<BitSet>> (thiz) ;
	}

	ArrayIterator<VR<BitSet>> end () leftvalue {
		return ArrayIterator<VR<BitSet>> (thiz) ;
	}

	ArrayIterator<CR<BitSet>> end () const leftvalue {
		return ArrayIterator<CR<BitSet>> (thiz) ;
	}

	ArrayRange<CR<BitSet>> iter () const leftvalue {
		return ArrayRange<CR<BitSet>> (thiz) ;
	}

	Bool equal (CR<BitSet> that) const {
		return BitSetHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<BitSet> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<BitSet> that) const {
		return (!equal (that)) ;
	}

	Flag compr (CR<BitSet> that) const {
		return BitSetHolder::hold (thiz)->compr (that) ;
	}

	forceinline Bool operator< (CR<BitSet> that) const {
		return compr (that) < ZERO ;
	}

	forceinline Bool operator<= (CR<BitSet> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline Bool operator> (CR<BitSet> that) const {
		return compr (that) > ZERO ;
	}

	forceinline Bool operator>= (CR<BitSet> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CR<Visitor> visitor) const {
		return BitSetHolder::hold (thiz)->visit (visitor) ;
	}

	void add (CR<A> item) {
		add (move (item)) ;
	}

	void add (RR<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return BitSetHolder::hold (thiz)->add (move (rax)) ;
	}

	Bool contain (CR<A> item) const {
		return BitSetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	void erase (CR<A> item) {
		return BitSetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}

	void fill (CR<Byte> item) {
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