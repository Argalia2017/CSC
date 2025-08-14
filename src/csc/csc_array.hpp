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

	forceinline operator LENGTH () const {
		return mSize ;
	}
} ;

template <class A>
class ArrayIterator {
private:
	using ITEM = decltype (nullof (A).at (0)) ;

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
	static CREF<ArrayRange> from (CREF<A> that) {
		return Pointer::from (that) ;
	}

	static CREF<ArrayRange> from (RREF<A> that) = delete ;

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

template <class A>
struct ArrayPureLayout implement ArrayLayout {
public:
	implicit ArrayPureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
	}
} ;

struct ArrayHolder implement Interface {
	imports VFat<ArrayHolder> hold (VREF<ArrayLayout> that) ;
	imports CFat<ArrayHolder> hold (CREF<ArrayLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<ArrayLayout> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> ref_m () leftvalue = 0 ;
	virtual CREF<Pointer> ref_m () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> index) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL equal (CREF<ArrayLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ArrayLayout> that) const = 0 ;
	virtual void visit (VREF<FriendVisitor> visitor) const = 0 ;
	virtual void fill (CREF<Pointer> item) = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<ArrayLayout> item) = 0 ;
} ;

template <class A>
class ArrayUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class Array implement ArrayPureLayout<A> {
protected:
	using ArrayPureLayout<A>::mArray ;

public:
	implicit Array () = default ;

	explicit Array (CREF<SizeProxy> size_) {
		ArrayHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,size_) ;
	}

	explicit Array (CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		ArrayHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,MakeWrapper (that) ,rax) ;
	}

	template <class ARG1>
	static Array make (CREF<ARG1> iterator) {
		Array ret = Array (iterator.length ()) ;
		INDEX ix = 0 ;
		for (auto &&i : iterator) {
			ret[ix] = A (i) ;
			ix++ ;
		}
		return move (ret) ;
	}

	implicit Array (CREF<Array> that) {
		ArrayHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Array> operator= (CREF<Array> that) {
		return assign (thiz ,that) ;
	}

	implicit Array (RREF<Array> that) = default ;

	forceinline VREF<Array> operator= (RREF<Array> that) = default ;

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

	VREF<ARR<A>> ref_m () leftvalue {
		return ArrayHolder::hold (thiz)->ref ;
	}

	forceinline operator VREF<ARR<A>> () leftvalue {
		return ref ;
	}

	CREF<ARR<A>> ref_m () const leftvalue {
		return ArrayHolder::hold (thiz)->ref ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return ref ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ArrayHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ArrayHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ArrayHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ArrayHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<Array>> begin () const leftvalue {
		return ArrayIterator<CREF<Array>> (thiz) ;
	}

	ArrayIterator<CREF<Array>> end () const leftvalue {
		return ArrayIterator<CREF<Array>> (thiz) ;
	}

	CREF<ArrayRange<Array>> iter () const leftvalue {
		return ArrayRange<Array>::from (thiz) ;
	}

	BOOL equal (CREF<Array> that) const {
		return ArrayHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Array> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Array> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Array> that) const {
		return ArrayHolder::hold (thiz)->compr (that) ;
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

	void visit (VREF<FriendVisitor> visitor) const {
		return ArrayHolder::hold (thiz)->visit (visitor) ;
	}

	void fill (CREF<A> item) {
		return ArrayHolder::hold (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CREF<INDEX> index ,CREF<Array> item) {
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

template <class A>
struct StringPureLayout implement StringLayout {
public:
	implicit StringPureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
	}
} ;

struct StringHolder implement Interface {
	imports VFat<StringHolder> hold (VREF<StringLayout> that) ;
	imports CFat<StringHolder> hold (CREF<StringLayout> that) ;

	virtual void initialize (CREF<Slice> that ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<StringLayout> that) = 0 ;
	virtual void clear () = 0 ;
	virtual FLAG encode () const = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual VREF<Pointer> ref_m () leftvalue = 0 ;
	virtual CREF<Pointer> ref_m () const leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () const leftvalue = 0 ;
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
	virtual void visit (VREF<FriendVisitor> visitor) const = 0 ;
	virtual void trunc (CREF<INDEX> index) = 0 ;
	virtual void fill (CREF<STRU32> item) = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<Slice> item) = 0 ;
	virtual void splice (CREF<INDEX> index ,CREF<StringLayout> item) = 0 ;
	virtual Slice segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const = 0 ;
} ;

template <class A>
class String implement StringPureLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	using StringPureLayout<A>::mString ;
	using StringPureLayout<A>::mEncode ;

public:
	implicit String () = default ;

	implicit String (CREF<Slice> that) {
		StringHolder::hold (thiz)->initialize (that ,SIZE_OF<A>::expr) ;
	}

	explicit String (CREF<SizeProxy> size_) {
		StringHolder::hold (thiz)->initialize (size_ ,SIZE_OF<A>::expr) ;
	}

	template <class...ARG1>
	static String make (CREF<ARG1>...params) {
		using R1X = KILL<StringBuild<A> ,TYPE<ARG1...>> ;
		return R1X::make (params...) ;
	}

	static CREF<String> zero () {
		return memorize ([&] () {
			return String (slice ("\0")) ;
		}) ;
	}

	implicit String (CREF<String> that) {
		StringHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<String> operator= (CREF<String> that) {
		return assign (thiz ,that) ;
	}

	implicit String (RREF<String> that) = default ;

	forceinline VREF<String> operator= (RREF<String> that) = default ;

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

	VREF<ARR<A>> ref_m () leftvalue {
		return StringHolder::hold (thiz)->ref ;
	}

	forceinline operator VREF<ARR<A>> () leftvalue {
		return ref ;
	}

	CREF<ARR<A>> ref_m () const leftvalue {
		return StringHolder::hold (thiz)->ref ;
	}

	forceinline operator CREF<ARR<A>> () const leftvalue {
		return ref ;
	}

	Ref<RefBuffer<BYTE>> borrow () leftvalue {
		return StringHolder::hold (thiz)->borrow () ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue {
		return StringHolder::hold (thiz)->borrow () ;
	}

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return StringHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return StringHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return StringHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return StringHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return StringHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<String>> begin () const leftvalue {
		return ArrayIterator<CREF<String>> (thiz) ;
	}

	ArrayIterator<CREF<String>> end () const leftvalue {
		return ArrayIterator<CREF<String>> (thiz) ;
	}

	CREF<ArrayRange<String>> iter () const leftvalue {
		return ArrayRange<String>::from (thiz) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return StringHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Slice> that) const {
		return (!equal (that)) ;
	}

	BOOL equal (CREF<String> that) const {
		return StringHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<String> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<String> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<String> that) const {
		return StringHolder::hold (thiz)->compr (that) ;
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

	void visit (VREF<FriendVisitor> visitor) const {
		return StringHolder::hold (thiz)->visit (visitor) ;
	}

	void trunc (CREF<INDEX> index) {
		return StringHolder::hold (thiz)->trunc (index) ;
	}

	void fill (CREF<STRU32> item) {
		return StringHolder::hold (thiz)->fill (item) ;
	}

	void splice (CREF<INDEX> index ,CREF<Slice> item) {
		return StringHolder::hold (thiz)->splice (index ,item) ;
	}

	void splice (CREF<INDEX> index ,CREF<String> item) {
		return StringHolder::hold (thiz)->splice (index ,item) ;
	}

	Slice segment () const {
		return segment (0 ,length ()) ;
	}

	Slice segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		return StringHolder::hold (thiz)->segment (begin_ ,end_) ;
	}
} ;

struct DequeLayout {
	RefBuffer<Pointer> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

template <class A>
struct DequePureLayout implement DequeLayout {
public:
	implicit DequePureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
	}
} ;

struct DequeHolder implement Interface {
	imports VFat<DequeHolder> hold (VREF<DequeLayout> that) ;
	imports CFat<DequeHolder> hold (CREF<DequeLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
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
	virtual BOOL full () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual INDEX tail () const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
	virtual void push (RREF<BoxLayout> item) = 0 ;
	virtual void pop () = 0 ;
	virtual void ring (CREF<LENGTH> count) = 0 ;
} ;

template <class A>
class DequeUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class Deque implement DequePureLayout<A> {
protected:
	using DequePureLayout<A>::mDeque ;
	using DequePureLayout<A>::mRead ;
	using DequePureLayout<A>::mWrite ;

public:
	implicit Deque () = default ;

	explicit Deque (CREF<SizeProxy> size_) {
		DequeHolder::hold (thiz)->initialize (DequeUnknownBinder<A> () ,size_) ;
	}

	explicit Deque (CREF<csc_initializer_list_t<A>> that) {
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

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return DequeHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return DequeHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return DequeHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return DequeHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return DequeHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<Deque>> begin () const leftvalue {
		return ArrayIterator<CREF<Deque>> (thiz) ;
	}

	ArrayIterator<CREF<Deque>> end () const leftvalue {
		return ArrayIterator<CREF<Deque>> (thiz) ;
	}

	CREF<ArrayRange<Deque>> iter () const leftvalue {
		return ArrayRange<Deque>::from (thiz) ;
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

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		DequeHolder::hold (thiz)->prepare (DequeUnknownBinder<A> ()) ;
		return DequeHolder::hold (thiz)->add (move (rax)) ;
	}

	void take () {
		return DequeHolder::hold (thiz)->take () ;
	}

	void take (VREF<A> item) {
		item = move (thiz[head ()]) ;
		take () ;
	}

	void push (CREF<A> item) {
		move (move (item)) ;
	}

	void push (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		DequeHolder::hold (thiz)->prepare (DequeUnknownBinder<A> ()) ;
		return DequeHolder::hold (thiz)->push (move (rax)) ;
	}

	void pop () {
		return DequeHolder::hold (thiz)->pop () ;
	}

	void pop (VREF<A> item) {
		item = move (thiz[tail ()]) ;
		take () ;
	}

	void ring (CREF<LENGTH> count) {
		return DequeHolder::hold (thiz)->ring (count) ;
	}
} ;

template <class A>
struct IndexPair implement Tuple<A ,INDEX> {
public:
	BOOL equal (CREF<IndexPair> that) const {
		return inline_equal (thiz.m1st ,that.m1st) ;
	}

	forceinline BOOL operator== (CREF<IndexPair> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<IndexPair> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<IndexPair> that) const {
		return inline_compr (thiz.m1st ,that.m1st) ;
	}

	forceinline BOOL operator< (CREF<IndexPair> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<IndexPair> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<IndexPair> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<IndexPair> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<FriendVisitor> visitor) const {
		visitor.enter () ;
		inline_visit (visitor ,thiz.m1st) ;
		visitor.leave () ;
	}
} ;

struct PriorityLayout {
	RefBuffer<Pointer> mPriority ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

template <class A>
struct PriorityPureLayout implement PriorityLayout {
public:
	implicit PriorityPureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
	}
} ;

struct PriorityHolder implement Interface {
	imports VFat<PriorityHolder> hold (VREF<PriorityLayout> that) ;
	imports CFat<PriorityHolder> hold (CREF<PriorityLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual void clear () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> index) const leftvalue = 0 ;
	virtual INDEX ibegin () const = 0 ;
	virtual INDEX iend () const = 0 ;
	virtual INDEX inext (CREF<INDEX> index) const = 0 ;
	virtual BOOL empty () const = 0 ;
	virtual BOOL full () const = 0 ;
	virtual INDEX head () const = 0 ;
	virtual void add (RREF<BoxLayout> item) = 0 ;
	virtual void take () = 0 ;
} ;

template <class A>
class PriorityUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class Priority implement PriorityPureLayout<A> {
protected:
	using PriorityPureLayout<A>::mPriority ;
	using PriorityPureLayout<A>::mRead ;
	using PriorityPureLayout<A>::mWrite ;

public:
	implicit Priority () = default ;

	explicit Priority (CREF<SizeProxy> size_) {
		PriorityHolder::hold (thiz)->initialize (PriorityUnknownBinder<A> () ,size_) ;
	}

	explicit Priority (CREF<csc_initializer_list_t<A>> that) {
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

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return PriorityHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return PriorityHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return PriorityHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return PriorityHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<Priority>> beign () const leftvalue {
		return ArrayIterator<CREF<Priority>> (thiz) ;
	}

	ArrayIterator<CREF<Priority>> end () const leftvalue {
		return ArrayIterator<CREF<Priority>> (thiz) ;
	}

	CREF<ArrayRange<Priority>> iter () const leftvalue {
		return ArrayRange<Priority>::from (thiz) ;
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

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		PriorityHolder::hold (thiz)->prepare (PriorityUnknownBinder<A> ()) ;
		return PriorityHolder::hold (thiz)->add (move (rax)) ;
	}

	void take () {
		return PriorityHolder::hold (thiz)->take () ;
	}

	void take (VREF<A> item) {
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

template <class A>
struct ListPureLayout implement ListLayout {
public:
	implicit ListPureLayout () noexcept {
		noop (Allocator<A ,ListNode> ()) ;
	}
} ;

struct ListHolder implement Interface {
	imports VFat<ListHolder> hold (VREF<ListLayout> that) ;
	imports CFat<ListHolder> hold (CREF<ListLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
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
	virtual BOOL full () const = 0 ;
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
class ListUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class List implement ListPureLayout<A> {
protected:
	using ListPureLayout<A>::mList ;
	using ListPureLayout<A>::mFirst ;
	using ListPureLayout<A>::mLast ;

public:
	implicit List () = default ;

	explicit List (CREF<SizeProxy> size_) {
		ListHolder::hold (thiz)->initialize (ListUnknownBinder<A> () ,size_) ;
	}

	explicit List (CREF<csc_initializer_list_t<A>> that) {
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

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ListHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ListHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ListHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ListHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ListHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<List>> begin () const leftvalue {
		return ArrayIterator<CREF<List>> (thiz) ;
	}

	ArrayIterator<CREF<List>> end () const leftvalue {
		return ArrayIterator<CREF<List>> (thiz) ;
	}

	CREF<ArrayRange<List>> iter () const leftvalue {
		return ArrayRange<List>::from (thiz) ;
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

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		ListHolder::hold (thiz)->prepare (ListUnknownBinder<A> ()) ;
		return ListHolder::hold (thiz)->add (move (rax)) ;
	}

	void take () {
		return ListHolder::hold (thiz)->take () ;
	}

	void take (VREF<A> item) {
		item = move (thiz[head ()]) ;
		take () ;
	}

	void push (CREF<A> item) {
		push (move (item)) ;
	}

	void push (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		ListHolder::hold (thiz)->prepare (ListUnknownBinder<A> ()) ;
		return ListHolder::hold (thiz)->push (move (rax)) ;
	}

	void pop () {
		return ListHolder::hold (thiz)->pop () ;
	}

	void pop (VREF<A> item) {
		item = move (thiz[tail ()]) ;
		pop () ;
	}

	INDEX insert () {
		auto rax = Box<A>::make () ;
		ListHolder::hold (thiz)->prepare (ListUnknownBinder<A> ()) ;
		return ListHolder::hold (thiz)->insert (move (rax)) ;
	}

	INDEX insert (CREF<INDEX> index) {
		auto rax = Box<A>::make () ;
		ListHolder::hold (thiz)->prepare (ListUnknownBinder<A> ()) ;
		return ListHolder::hold (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CREF<INDEX> index) {
		return ListHolder::hold (thiz)->remove (index) ;
	}

	void order (CREF<Array<INDEX>> range_) {
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

template <class A>
struct ArrayListPureLayout implement ArrayListLayout {
public:
	implicit ArrayListPureLayout () noexcept {
		noop (Allocator<A ,ArrayListNode> ()) ;
	}
} ;

struct ArrayListHolder implement Interface {
	imports VFat<ArrayListHolder> hold (VREF<ArrayListLayout> that) ;
	imports CFat<ArrayListHolder> hold (CREF<ArrayListLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
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
class ArrayListUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class ArrayList implement ArrayListPureLayout<A> {
protected:
	using ArrayListPureLayout<A>::mList ;
	using ArrayListPureLayout<A>::mRange ;
	using ArrayListPureLayout<A>::mTop ;
	using ArrayListPureLayout<A>::mRemap ;

public:
	implicit ArrayList () = default ;

	explicit ArrayList (CREF<SizeProxy> size_) {
		ArrayListHolder::hold (thiz)->initialize (ArrayListUnknownBinder<A> () ,size_) ;
	}

	explicit ArrayList (CREF<csc_initializer_list_t<A>> that) {
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

	VREF<A> at (CREF<INDEX> index) leftvalue {
		return ArrayListHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<A> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return ArrayListHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return ArrayListHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return ArrayListHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return ArrayListHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<ArrayList>> begin () const leftvalue {
		return ArrayIterator<CREF<ArrayList>> (thiz) ;
	}

	ArrayIterator<CREF<ArrayList>> end () const leftvalue {
		return ArrayIterator<CREF<ArrayList>> (thiz) ;
	}

	CREF<ArrayRange<ArrayList>> iter () const leftvalue {
		return ArrayRange<ArrayList>::from (thiz) ;
	}

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		ArrayListHolder::hold (thiz)->prepare (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::hold (thiz)->add (move (rax)) ;
	}

	INDEX insert () {
		auto rax = Box<A>::make () ;
		ArrayListHolder::hold (thiz)->prepare (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::hold (thiz)->insert (move (rax)) ;
	}

	INDEX insert (CREF<INDEX> index) {
		auto rax = Box<A>::make () ;
		ArrayListHolder::hold (thiz)->prepare (ArrayListUnknownBinder<A> ()) ;
		return ArrayListHolder::hold (thiz)->insert (index ,move (rax)) ;
	}

	void remove (CREF<INDEX> index) {
		return ArrayListHolder::hold (thiz)->remove (index) ;
	}

	void order (CREF<Array<INDEX>> range_) {
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

template <class A>
struct SortedMapPureLayout implement SortedMapLayout {
public:
	implicit SortedMapPureLayout () noexcept {
		noop (Allocator<A ,SortedMapNode> ()) ;
	}
} ;

struct SortedMapHolder implement Interface {
	imports VFat<SortedMapHolder> hold (VREF<SortedMapLayout> that) ;
	imports CFat<SortedMapHolder> hold (CREF<SortedMapLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
	virtual SortedMapLayout share () const = 0 ;
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
	virtual void remap () = 0 ;
} ;

template <class A>
class SortedMapUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class SortedMap implement SortedMapPureLayout<A> {
protected:
	using SortedMapPureLayout<A>::mThis ;
	using SortedMapPureLayout<A>::mRoot ;
	using SortedMapPureLayout<A>::mRange ;
	using SortedMapPureLayout<A>::mWrite ;
	using SortedMapPureLayout<A>::mRemap ;

public:
	implicit SortedMap () = default ;

	explicit SortedMap (CREF<SizeProxy> size_) {
		SortedMapHolder::hold (thiz)->initialize (SortedMapUnknownBinder<A> () ,size_) ;
	}

	explicit SortedMap (CREF<csc_initializer_list_t<A>> that) {
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

	VREF<INDEX> at (CREF<INDEX> index) leftvalue {
		return SortedMapHolder::hold (thiz)->at (index) ;
	}

	forceinline VREF<INDEX> operator[] (CREF<INDEX> index) leftvalue {
		return at (index) ;
	}

	CREF<INDEX> at (CREF<INDEX> index) const leftvalue {
		return SortedMapHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<INDEX> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	INDEX ibegin () const {
		return SortedMapHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SortedMapHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return SortedMapHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<SortedMap>> begin () const leftvalue {
		return ArrayIterator<CREF<SortedMap>> (thiz) ;
	}

	ArrayIterator<CREF<SortedMap>> end () const leftvalue {
		return ArrayIterator<CREF<SortedMap>> (thiz) ;
	}

	CREF<ArrayRange<SortedMap>> iter () const leftvalue {
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
		SortedMapHolder::hold (thiz)->prepare (SortedMapUnknownBinder<A> ()) ;
		return SortedMapHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return SortedMapHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return SortedMapHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
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

template <class A>
struct SetPureLayout implement SetLayout {
public:
	implicit SetPureLayout () noexcept {
		noop (Allocator<A ,SetNode> ()) ;
	}
} ;

struct SetHolder implement Interface {
	imports VFat<SetHolder> hold (VREF<SetLayout> that) ;
	imports CFat<SetHolder> hold (CREF<SetLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
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
class SetUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class Set implement SetPureLayout<A> {
protected:
	using SetPureLayout<A>::mSet ;
	using SetPureLayout<A>::mRoot ;
	using SetPureLayout<A>::mTop ;

public:
	implicit Set () = default ;

	explicit Set (CREF<SizeProxy> size_) {
		SetHolder::hold (thiz)->initialize (SetUnknownBinder<A> () ,size_) ;
	}

	explicit Set (CREF<csc_initializer_list_t<A>> that) {
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

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return SetHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const {
		return SetHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) {
		return SetHolder::hold (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return SetHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return SetHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return SetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<Set>> begin () const leftvalue {
		return ArrayIterator<CREF<Set>> (thiz) ;
	}

	ArrayIterator<CREF<Set>> end () const leftvalue {
		return ArrayIterator<CREF<Set>> (thiz) ;
	}

	CREF<ArrayRange<Set>> iter () const leftvalue {
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
		SetHolder::hold (thiz)->prepare (SetUnknownBinder<A> ()) ;
		return SetHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return SetHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return SetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
		return SetHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CREF<INDEX> index) {
		return SetHolder::hold (thiz)->remove (index) ;
	}

	void erase (CREF<A> item) {
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

template <class A>
struct HashSetPureLayout implement HashSetLayout {
public:
	implicit HashSetPureLayout () noexcept {
		noop (Allocator<A ,HashSetNode> ()) ;
	}
} ;

struct HashSetHolder implement Interface {
	imports VFat<HashSetHolder> hold (VREF<HashSetLayout> that) ;
	imports CFat<HashSetHolder> hold (CREF<HashSetLayout> that) ;

	virtual void prepare (CREF<Unknown> holder) = 0 ;
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
class HashSetUnknownBinder final implement Fat<ReflectUnknown ,Proxy> {
public:
	FLAG reflect (CREF<FLAG> uuid) const override {
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
class HashSet implement HashSetPureLayout<A> {
protected:
	using HashSetPureLayout<A>::mSet ;
	using HashSetPureLayout<A>::mRange ;
	using HashSetPureLayout<A>::mVisitor ;

public:
	implicit HashSet () = default ;

	explicit HashSet (CREF<SizeProxy> size_) {
		HashSetHolder::hold (thiz)->initialize (HashSetUnknownBinder<A> () ,size_) ;
	}

	explicit HashSet (CREF<csc_initializer_list_t<A>> that) {
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

	CREF<A> at (CREF<INDEX> index) const leftvalue {
		return HashSetHolder::hold (thiz)->at (index) ;
	}

	forceinline CREF<A> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const {
		return HashSetHolder::hold (thiz)->get (index ,map_) ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) {
		return HashSetHolder::hold (thiz)->set (index ,map_) ;
	}

	INDEX ibegin () const {
		return HashSetHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return HashSetHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return HashSetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<HashSet>> begin () const leftvalue {
		return ArrayIterator<CREF<HashSet>> (thiz) ;
	}

	ArrayIterator<CREF<HashSet>> end () const leftvalue {
		return ArrayIterator<CREF<HashSet>> (thiz) ;
	}

	CREF<ArrayRange<HashSet>> iter () const leftvalue {
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
		HashSetHolder::hold (thiz)->prepare (HashSetUnknownBinder<A> ()) ;
		return HashSetHolder::hold (thiz)->add (move (rax) ,map_) ;
	}

	INDEX find (CREF<A> item) const {
		return HashSetHolder::hold (thiz)->find (Pointer::from (item)) ;
	}

	BOOL contain (CREF<A> item) const {
		return HashSetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	INDEX map (CREF<A> item) const {
		return HashSetHolder::hold (thiz)->map (Pointer::from (item)) ;
	}

	void remove (CREF<INDEX> index) {
		return HashSetHolder::hold (thiz)->remove (index) ;
	}

	void erase (CREF<A> item) {
		return SetHolder::hold (thiz)->erase (Pointer::from (item)) ;
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
	imports VFat<BitSetHolder> hold (VREF<BitSetLayout> that) ;
	imports CFat<BitSetHolder> hold (CREF<BitSetLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual void initialize (CREF<LENGTH> size_ ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) = 0 ;
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
	virtual void visit (VREF<FriendVisitor> visitor) const = 0 ;
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
		BitSetHolder::hold (thiz)->initialize (size_) ;
	}

	explicit BitSet (CREF<SizeProxy> size_ ,CREF<csc_initializer_list_t<A>> that) {
		auto rax = Box<A> () ;
		BitSetHolder::hold (thiz)->initialize (size_ ,MakeWrapper (that) ,rax) ;
	}

	implicit BitSet (CREF<BitSet> that) {
		BitSetHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<BitSet> operator= (CREF<BitSet> that) {
		return assign (thiz ,that) ;
	}

	implicit BitSet (RREF<BitSet> that) = default ;

	forceinline VREF<BitSet> operator= (RREF<BitSet> that) = default ;

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

	void get (CREF<INDEX> index ,VREF<BOOL> item) const {
		return BitSetHolder::hold (thiz)->get (index ,item) ;
	}

	void set (CREF<INDEX> index ,CREF<BOOL> item) {
		return BitSetHolder::hold (thiz)->set (index ,item) ;
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
		return BitSetHolder::hold (thiz)->ibegin () ;
	}

	INDEX iend () const {
		return BitSetHolder::hold (thiz)->iend () ;
	}

	INDEX inext (CREF<INDEX> index) const {
		return BitSetHolder::hold (thiz)->inext (index) ;
	}

	ArrayIterator<CREF<BitSet>> begin () const leftvalue {
		return ArrayIterator<CREF<BitSet>> (thiz) ;
	}

	ArrayIterator<CREF<BitSet>> end () const leftvalue {
		return ArrayIterator<CREF<BitSet>> (thiz) ;
	}

	CREF<ArrayRange<BitSet>> iter () const leftvalue {
		return ArrayRange<BitSet>::from (thiz) ;
	}

	BOOL equal (CREF<BitSet> that) const {
		return BitSetHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<BitSet> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<BitSet> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<BitSet> that) const {
		return BitSetHolder::hold (thiz)->compr (that) ;
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

	void visit (VREF<FriendVisitor> visitor) const {
		return BitSetHolder::hold (thiz)->visit (visitor) ;
	}

	void add (CREF<A> item) {
		add (move (item)) ;
	}

	void add (RREF<A> item) {
		auto rax = Box<A>::make (move (item)) ;
		return BitSetHolder::hold (thiz)->add (move (rax)) ;
	}

	BOOL contain (CREF<A> item) const {
		return BitSetHolder::hold (thiz)->contain (Pointer::from (item)) ;
	}

	void erase (CREF<A> item) {
		return BitSetHolder::hold (thiz)->erase (Pointer::from (item)) ;
	}

	void fill (CREF<BYTE> item) {
		return BitSetHolder::hold (thiz)->fill (item) ;
	}

	BitSet sand (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->sand (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator& (CREF<BitSet> that) const {
		return sand (that) ;
	}

	forceinline void operator&= (CREF<BitSet> that) {
		thiz = sand (that) ;
	}

	BitSet sor (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->sor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator| (CREF<BitSet> that) const {
		return sor (that) ;
	}

	forceinline void operator|= (CREF<BitSet> that) {
		thiz = sor (that) ;
	}

	BitSet sxor (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->sxor (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator^ (CREF<BitSet> that) const {
		return sxor (that) ;
	}

	forceinline void operator^= (CREF<BitSet> that) {
		thiz = sxor (that) ;
	}

	BitSet ssub (CREF<BitSet> that) const {
		BitSetLayout ret = BitSetHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<BitSet>::expr] (ret)) ;
	}

	forceinline BitSet operator- (CREF<BitSet> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<BitSet> that) {
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