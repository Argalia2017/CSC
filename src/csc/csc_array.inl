#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
class ArrayImplHolder implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mArray)->initialize (holder ,size_) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void initialize (CREF<ArrayLayout> that) override {
		const auto r1x = ArrayHolder::create (that)->length () ;
		initialize (RefHolder::create (that.mArray.mBuffer)->unknown () ,r1x) ;
		const auto r2x = CFat<ReflectClone> (RefHolder::create (fake.mArray.mBuffer)->unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			auto &&rbx = RefBufferHolder::create (that.mArray)->at (i) ;
			r2x->clone (rax ,rbx) ;
		}
	}

	LENGTH size () const override {
		return RefBufferHolder::create (fake.mArray)->size () ;
	}

	LENGTH step () const override {
		return RefBufferHolder::create (fake.mArray)->step () ;
	}

	LENGTH length () const override {
		return RefBufferHolder::create (fake.mArray)->size () ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return RefBufferHolder::create (fake.mArray)->at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return RefBufferHolder::create (fake.mArray)->at (index) ;
	}

	INDEX ibegin () const override {
		return 0 ;
	}

	INDEX iend () const override {
		return length () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return index + 1 ;
	}

	BOOL equal (CREF<ArrayLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = ArrayHolder::create (that)->length () ;
		if (r1x != r2x)
			return FALSE ;
		const auto r3x = CFat<ReflectEqual> (RefHolder::create (fake.mArray.mBuffer)->unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			auto &&rbx = RefBufferHolder::create (that.mArray)->at (i) ;
			if ifnot (r3x->equal (rax ,rbx))
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<ArrayLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = ArrayHolder::create (that)->length () ;
		const auto r3x = operator_min (r1x ,r2x) ;
		const auto r4x = CFat<ReflectCompr> (RefHolder::create (fake.mArray.mBuffer)->unknown ()) ;
		for (auto &&i : iter (0 ,r3x)) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			auto &&rbx = RefBufferHolder::create (that.mArray)->at (i) ;
			const auto r5x = r4x->compr (rax ,rbx) ;
			if (r5x != ZERO)
				return r5x ;
		}
		return operator_compr (r1x ,r2x) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		const auto r1x = CFat<ReflectVisit> (RefHolder::create (fake.mArray.mBuffer)->unknown ()) ;
		for (auto &&i : iter (0 ,length ())) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			r1x->visit (visitor ,rax) ;
		}
	}

	void fill (CREF<Pointer> item) override {
		const auto r1x = CFat<ReflectClone> (RefHolder::create (fake.mArray.mBuffer)->unknown ()) ;
		for (auto &&i : iter (0 ,size ())) {
			auto &&rax = RefBufferHolder::create (fake.mArray)->at (i) ;
			r1x->clone (rax ,item) ;
		}
	}
} ;

exports VFat<ArrayHolder> ArrayHolder::create (VREF<ArrayLayout> that) {
	return VFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

exports CFat<ArrayHolder> ArrayHolder::create (CREF<ArrayLayout> that) {
	return CFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

class StringImplHolder implement Fat<StringHolder ,StringLayout> {
public:
	void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) override {
		const auto r1x = size_ + 1 ;
		if ifdo (TRUE) {
			if (step_ != 1)
				discard ;
			RefBufferHolder::create (fake.mString)->initialize (RefBufferUnknownBinder<STRU8> () ,r1x) ;
			clear () ;
			return ;
		}
		if ifdo (TRUE) {
			if (step_ != 2)
				discard ;
			RefBufferHolder::create (fake.mString)->initialize (RefBufferUnknownBinder<STRU16> () ,r1x) ;
			clear () ;
			return ;
		}
		if ifdo (TRUE) {
			if (step_ != 4)
				discard ;
			RefBufferHolder::create (fake.mString)->initialize (RefBufferUnknownBinder<STRU32> () ,r1x) ;
			clear () ;
			return ;
		}
		assert (FALSE) ;
	}

	void initialize (CREF<SliceLayout> that) override {
		const auto r1x = SliceHolder::create (that)->size () ;
		initialize (r1x ,that.mStep) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,get (that ,i)) ;
		}
	}

	void initialize (CREF<StringLayout> that) override {
		const auto r1x = StringHolder::create (that)->length () ;
		initialize (r1x ,that.mString.mStep) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,get (that ,i)) ;
		}
	}

	void clear () override {
		set (0 ,0) ;
		set (size () ,0) ;
	}

	LENGTH size () const override {
		return operator_max (RefBufferHolder::create (fake.mString)->size () - 1 ,0) ;
	}

	LENGTH step () const override {
		return RefBufferHolder::create (fake.mString)->step () ;
	}

	LENGTH length () const override {
		for (auto &&i : iter (0 ,size ())) {
			const auto r1x = get (fake ,i) ;
			if (r1x == 0)
				return i ;
		}
		return size () ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		if ifdo (TRUE) {
			if (fake.mString.mStep != 1)
				discard ;
			item = STRU8 (at (index)) ;
			return ;
		}
		if ifdo (TRUE) {
			if (fake.mString.mStep != 2)
				discard ;
			item = STRU16 (at (index)) ;
			return ;
		}
		if ifdo (TRUE) {
			if (fake.mString.mStep != 4)
				discard ;
			item = STRU32 (at (index)) ;
			return ;
		}
		assert (FALSE) ;
	}

	imports STRU32 get (CREF<SliceLayout> layout ,CREF<INDEX> index) {
		STRU32 ret ;
		SliceHolder::create (layout)->get (index ,ret) ;
		return move (ret) ;
	}

	imports STRU32 get (CREF<StringLayout> layout ,CREF<INDEX> index) {
		STRU32 ret ;
		StringHolder::create (layout)->get (index ,ret) ;
		return move (ret) ;
	}

	void set (CREF<INDEX> index ,CREF<STRU32> item) override {
		if ifdo (TRUE) {
			if (fake.mString.mStep != 1)
				discard ;
			keep[TYPE<VREF<STRU8>>::expr] (at (index)) = STRU8 (item) ;
			return ;
		}
		if ifdo (TRUE) {
			if (fake.mString.mStep != 2)
				discard ;
			keep[TYPE<VREF<STRU16>>::expr] (at (index)) = STRU16 (item) ;
			return ;
		}
		if ifdo (TRUE) {
			if (fake.mString.mStep != 4)
				discard ;
			keep[TYPE<VREF<STRU32>>::expr] (at (index)) = STRU32 (item) ;
			return ;
		}
		assert (FALSE) ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return RefBufferHolder::create (fake.mString)->at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return RefBufferHolder::create (fake.mString)->at (index) ;
	}

	INDEX ibegin () const override {
		return 0 ;
	}

	INDEX iend () const override {
		return length () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return index + 1 ;
	}

	BOOL equal (CREF<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::create (that)->length () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = get (fake ,i) ;
			const auto r4x = get (that ,i) ;
			if (r3x != r4x)
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::create (that)->length () ;
		const auto r3x = operator_min (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r3x)) {
			const auto r4x = get (fake ,i) ;
			const auto r5x = get (that ,i) ;
			const auto r6x = operator_compr (r4x ,r5x) ;
			if (r6x != ZERO)
				return r6x ;
		}
		return operator_compr (r1x ,r2x) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		for (auto &&i : iter (0 ,length ())) {
			const auto r1x = get (fake ,i) ;
			operator_visit (visitor ,r1x) ;
		}
	}

	void fill (CREF<STRU32> item) override {
		for (auto &&i : iter (0 ,size ())) {
			set (i ,item) ;
		}
	}
} ;

exports VFat<StringHolder> StringHolder::create (VREF<StringLayout> that) {
	return VFat<StringHolder> (StringImplHolder () ,that) ;
}

exports CFat<StringHolder> StringHolder::create (CREF<StringLayout> that) {
	return CFat<StringHolder> (StringImplHolder () ,that) ;
}

class DequeImplHolder implement Fat<DequeHolder ,DequeLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mDeque)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		return RefBufferHolder::create (fake.mDeque)->size () ;
	}

	LENGTH step () const override {
		return RefBufferHolder::create (fake.mDeque)->step () ;
	}

	LENGTH length () const override {
		if (RefBufferHolder::create (fake.mDeque)->size () == 0)
			return 0 ;
		return fake.mWrite - fake.mRead ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (operator_between (index ,0 ,length ())) ;
		return RefBufferHolder::create (fake.mDeque)->at (fake.mRead + index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (operator_between (index ,0 ,length ())) ;
		return RefBufferHolder::create (fake.mDeque)->at (fake.mRead + index) ;
	}

	INDEX ibegin () const override {
		return 0 ;
	}

	INDEX iend () const override {
		return length () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return index + 1 ;
	}

	BOOL empty () const override {
		if (RefBufferHolder::create (fake.mDeque)->size () == 0)
			return TRUE ;
		return fake.mWrite == fake.mRead ;
	}

	INDEX head () const override {
		if (RefBufferHolder::create (fake.mDeque)->size () == 0)
			return NONE ;
		return fake.mRead % RefBufferHolder::create (fake.mDeque)->size () ;
	}

	INDEX tail () const override {
		if (RefBufferHolder::create (fake.mDeque)->size () == 0)
			return NONE ;
		return fake.mWrite % RefBufferHolder::create (fake.mDeque)->size () ;
	}

	void add (CREF<Unknown> holder ,RREF<BoxLayout> item) override {
		reserve (holder) ;
		INDEX ix = fake.mWrite % RefBufferHolder::create (fake.mDeque)->size () ;
		const auto r1x = CFat<ReflectSize> (BoxHolder::create (item)->unknown ()) ;
		auto &&rax = RefBufferHolder::create (fake.mDeque)->at (ix) ;
		auto &&rbx = BoxHolder::create (item)->self ;
		memcpy ((&rax) ,(&rbx) ,r1x->type_size ()) ;
		BoxHolder::create (item)->release () ;
		fake.mWrite++ ;
		check () ;
	}

	void take () override {
		fake.mRead++ ;
		check () ;
	}

	void push (CREF<Unknown> holder ,RREF<BoxLayout> item) override {
		reserve (holder) ;
		const auto r1x = RefBufferHolder::create (fake.mDeque)->size () ;
		INDEX ix = (fake.mRead - 1 + r1x) % r1x ;
		const auto r2x = CFat<ReflectSize> (BoxHolder::create (item)->unknown ()) ;
		auto &&rax = RefBufferHolder::create (fake.mDeque)->at (ix) ;
		auto &&rbx = BoxHolder::create (item)->self ;
		memcpy ((&rax) ,(&rbx) ,r2x->type_size ()) ;
		BoxHolder::create (item)->release () ;
		fake.mRead-- ;
		check () ;
	}

	void pop () override {
		fake.mWrite-- ;
		check () ;
	}

	void reserve (CREF<Unknown> holder) {
		if (length () < size ())
			return ;
		unimplemented () ;
	}

	void check () {
		const auto r1x = RefBufferHolder::create (fake.mDeque)->size () ;
		if ifdo (TRUE) {
			if (fake.mRead >= 0)
				discard ;
			fake.mRead += r1x ;
			fake.mWrite += r1x ;
		}
		if ifdo (TRUE) {
			if (fake.mRead < r1x)
				discard ;
			fake.mRead -= r1x ;
			fake.mWrite -= r1x ;
		}
	}
} ;

exports VFat<DequeHolder> DequeHolder::create (VREF<DequeLayout> that) {
	return VFat<DequeHolder> (DequeImplHolder () ,that) ;
}

exports CFat<DequeHolder> DequeHolder::create (CREF<DequeLayout> that) {
	return CFat<DequeHolder> (DequeImplHolder () ,that) ;
}

class PriorityImplHolder implement Fat<PriorityHolder ,PriorityLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mPriority)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		return RefBufferHolder::create (fake.mPriority)->size () ;
	}

	LENGTH step () const override {
		return RefBufferHolder::create (fake.mPriority)->step () ;
	}

	LENGTH length () const override {
		if (RefBufferHolder::create (fake.mPriority)->size () == 0)
			return 0 ;
		return fake.mWrite - fake.mRead ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (operator_between (index ,0 ,length ())) ;
		return RefBufferHolder::create (fake.mPriority)->at (index) ;
	}

	INDEX ibegin () const override {
		return 0 ;
	}

	INDEX iend () const override {
		return length () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return index + 1 ;
	}

	BOOL empty () const override {
		if (RefBufferHolder::create (fake.mPriority)->size () == 0)
			return TRUE ;
		return fake.mWrite == fake.mRead ;
	}

	INDEX head () const override {
		if (RefBufferHolder::create (fake.mPriority)->size () == 0)
			return NONE ;
		return fake.mRead ;
	}

	void add (CREF<Unknown> holder ,RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		reserve (holder) ;
		//@mark
	}

	void take () override {
		//@mark
	}

	void reserve (CREF<Unknown> holder) {
		if (length () < size ())
			return ;
		unimplemented () ;
	}
} ;

exports VFat<PriorityHolder> PriorityHolder::create (VREF<PriorityLayout> that) {
	return VFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

exports CFat<PriorityHolder> PriorityHolder::create (CREF<PriorityLayout> that) {
	return CFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

class ListImplHolder implement Fat<ListHolder ,ListLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mList)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void clear () override {
		AllocatorHolder::create (fake.mList)->clear () ;
		fake.mFirst = NONE ;
		fake.mLast = NONE ;
	}

	LENGTH size () const override {
		return AllocatorHolder::create (fake.mList)->size () ;
	}

	LENGTH step () const override {
		return AllocatorHolder::create (fake.mList)->step () ;
	}

	LENGTH length () const override {
		return AllocatorHolder::create (fake.mList)->length () ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return AllocatorHolder::create (fake.mList)->at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return AllocatorHolder::create (fake.mList)->at (index) ;
	}

	imports VREF<ListNodeLayout> at (VREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = CFat<ReflectNode> (RefHolder::create (layout.mAllocator.mBuffer)->unknown ()) ;
		const auto r2x = layout.mAllocator.mBuffer.mPointer + index * layout.mAllocator.mStep + r1x->offset (1) ;
		return Pointer::make (r2x) ;
	}

	imports CREF<ListNodeLayout> at (CREF<AllocatorLayout> layout ,CREF<INDEX> index) {
		const auto r1x = CFat<ReflectNode> (RefHolder::create (layout.mAllocator.mBuffer)->unknown ()) ;
		const auto r2x = layout.mAllocator.mBuffer.mPointer + index * layout.mAllocator.mStep + r1x->offset (1) ;
		return Pointer::make (r2x) ;
	}

	INDEX ibegin () const override {
		if (AllocatorHolder::create (fake.mList)->size () == 0)
			return NONE ;
		return fake.mFirst ;
	}

	INDEX iend () const override {
		return NONE ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return at (fake.mList ,index).mRight ;
	}

	BOOL empty () const override {
		if (AllocatorHolder::create (fake.mList)->size () == 0)
			return TRUE ;
		return fake.mFirst == NONE ;
	}

	INDEX head () const override {
		if (AllocatorHolder::create (fake.mList)->size () == 0)
			return NONE ;
		return fake.mFirst ;
	}

	INDEX tail () const override {
		if (AllocatorHolder::create (fake.mList)->size () == 0)
			return NONE ;
		return fake.mLast ;
	}

	void add (CREF<Unknown> holder ,RREF<BoxLayout> item) override {
		INDEX ix = AllocatorHolder::create (fake.mList)->alloc (holder) ;
		const auto r1x = CFat<ReflectSize> (BoxHolder::create (item)->unknown ()) ;
		auto &&rax = AllocatorHolder::create (fake.mList)->at (ix) ;
		auto &&rbx = BoxHolder::create (item)->self ;
		memcpy ((&rax) ,(&rbx) ,r1x->type_size ()) ;
		BoxHolder::create (item)->release () ;
		left_right (fake.mLast) = ix ;
		at (fake.mList ,ix).mLeft = fake.mLast ;
		at (fake.mList ,ix).mRight = NONE ;
		fake.mLast = ix ;
	}

	void take () override {
		//@mark
	}

	void push (CREF<Unknown> holder ,RREF<BoxLayout> item) override {
		//@mark
	}

	void pop () override {
		//@mark
	}

	INDEX insert () override {
		//@mark
		return NONE ;
	}

	INDEX insert (CREF<INDEX> index) override {
		//@mark
		return NONE ;
	}

	void remove (CREF<INDEX> index) override {
		//@mark
	}

	VREF<INDEX> left_right (CREF<INDEX> index) leftvalue {
		if (index == NONE)
			return fake.mFirst ;
		return at (fake.mList ,index).mRight ;
	}

	VREF<INDEX> right_left (CREF<INDEX> index) leftvalue {
		if (index == NONE)
			return fake.mLast ;
		return at (fake.mList ,index).mLeft ;
	}
} ;

exports VFat<ListHolder> ListHolder::create (VREF<ListLayout> that) {
	return VFat<ListHolder> (ListImplHolder () ,that) ;
}

exports CFat<ListHolder> ListHolder::create (CREF<ListLayout> that) {
	return CFat<ListHolder> (ListImplHolder () ,that) ;
}

class ArrayListImplHolder implement Fat<ArrayListHolder ,ArrayListLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mList)->initialize (holder ,size_) ;
		fake.mRange = RefBuffer<INDEX> (AllocatorHolder::create (fake.mList)->size ()) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void clear () override {
		AllocatorHolder::create (fake.mList)->clear () ;
		for (auto &&i : iter (0 ,fake.mRange.size ())) {
			fake.mRange[i] = NONE ;
		}
	}

	LENGTH size () const override {
		return AllocatorHolder::create (fake.mList)->size () ;
	}

	LENGTH step () const override {
		return AllocatorHolder::create (fake.mList)->step () ;
	}

	LENGTH length () const override {
		return AllocatorHolder::create (fake.mList)->length () ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		INDEX ix = RefBufferHolder::create (fake.mRange)->at (index) ;
		return AllocatorHolder::create (fake.mList)->at (ix) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		INDEX ix = RefBufferHolder::create (fake.mRange)->at (index) ;
		return AllocatorHolder::create (fake.mList)->at (ix) ;
	}

	INDEX ibegin () const override {
		return find_next (0) ;
	}

	INDEX iend () const override {
		return fake.mRange.size () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return find_next (index + 1) ;
	}

	INDEX find_next (CREF<INDEX> index) const {
		INDEX ret = index ;
		while (TRUE) {
			if (ret >= fake.mRange.size ())
				break ;
			if (fake.mRange[ret] != NONE)
				break ;
			ret++ ;
		}
		return move (ret) ;
	}

	INDEX insert () override {
		//@mark
		return NONE ;
	}

	INDEX insert (CREF<INDEX> index) override {
		//@mark
		return NONE ;
	}

	void remove (CREF<INDEX> index) override {
		//@mark
	}

	void remap () override {
		//@mark
	}
} ;

exports VFat<ArrayListHolder> ArrayListHolder::create (VREF<ArrayListLayout> that) {
	return VFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

exports CFat<ArrayListHolder> ArrayListHolder::create (CREF<ArrayListLayout> that) {
	return CFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

class SetImplHolder implement Fat<SetHolder ,SetLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mSet)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void clear () override {
		AllocatorHolder::create (fake.mSet)->clear () ;
		fake.mRoot = NONE ;
		fake.mTop = NONE ;
	}

	LENGTH size () const override {
		return AllocatorHolder::create (fake.mSet)->size () ;
	}

	LENGTH step () const override {
		return AllocatorHolder::create (fake.mSet)->step () ;
	}

	LENGTH length () const override {
		return AllocatorHolder::create (fake.mSet)->length () ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return AllocatorHolder::create (fake.mSet)->at (index) ;
	}

	INDEX ibegin () const override {
		//@mark
		return NONE ;
	}

	INDEX iend () const override {
		//@mark
		return NONE ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		//@mark
		return NONE ;
	}

	void add (CREF<Unknown> holder ,RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		//@mark
	}

	INDEX find (CREF<BoxLayout> item) const override {
		//@mark
		return NONE ;
	}

	BOOL contain (CREF<BoxLayout> item) const override {
		//@mark
		return TRUE ;
	}

	INDEX insert (CREF<BoxLayout> item) const override {
		//@mark
		return NONE ;
	}

	void remove (CREF<INDEX> index) override {
		//@mark
	}

	void erase (CREF<BoxLayout> item) override {
		//@mark
	}
} ;

exports VFat<SetHolder> SetHolder::create (VREF<SetLayout> that) {
	return VFat<SetHolder> (SetImplHolder () ,that) ;
}

exports CFat<SetHolder> SetHolder::create (CREF<SetLayout> that) {
	return CFat<SetHolder> (SetImplHolder () ,that) ;
}

class HashSetImplHolder implement Fat<HashSetHolder ,HashSetLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mSet)->initialize (holder ,size_) ;
		fake.mRange = RefBuffer<INDEX> (AllocatorHolder::create (fake.mSet)->size ()) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void clear () override {
		AllocatorHolder::create (fake.mSet)->clear () ;
		for (auto &&i : iter (0 ,fake.mRange.size ())) {
			fake.mRange[i] = NONE ;
		}
	}

	LENGTH size () const override {
		return AllocatorHolder::create (fake.mSet)->size () ;
	}

	LENGTH step () const override {
		return AllocatorHolder::create (fake.mSet)->step () ;
	}

	LENGTH length () const override {
		return AllocatorHolder::create (fake.mSet)->length () ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return AllocatorHolder::create (fake.mSet)->at (index) ;
	}

	INDEX ibegin () const override {
		//@mark
		return NONE ;
	}

	INDEX iend () const override {
		//@mark
		return NONE ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		//@mark
		return NONE ;
	}

	void add (CREF<Unknown> holder ,RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		//@mark
	}

	INDEX find (CREF<BoxLayout> item) const override {
		//@mark
		return NONE ;
	}

	BOOL contain (CREF<BoxLayout> item) const override {
		//@mark
		return TRUE ;
	}

	INDEX insert (CREF<BoxLayout> item) const override {
		//@mark
		return NONE ;
	}

	void remove (CREF<INDEX> index) override {
		//@mark
	}

	void erase (CREF<BoxLayout> item) override {
		//@mark
	}
} ;

exports VFat<HashSetHolder> HashSetHolder::create (VREF<HashSetLayout> that) {
	return VFat<HashSetHolder> (HashSetImplHolder () ,that) ;
}

exports CFat<HashSetHolder> HashSetHolder::create (CREF<HashSetLayout> that) {
	return CFat<HashSetHolder> (HashSetImplHolder () ,that) ;
}

class BitSetImplHolder implement Fat<BitSetHolder ,BitSetLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		const auto r1x = operator_alignas (size_ ,8) / 8 ;
		fake.mSet = RefBuffer<BYTE> (r1x) ;
		fake.mMask = BYTE (FLAG (WORD (0X01) << (size_ % 8)) - 1) ;
		fake.mSize = size_ ;
		clear () ;
	}

	void initialize (CREF<CaptureLayout> that) override {
		unimplemented () ;
	}

	void initialize (CREF<BitSetLayout> that) override {
		fake.mSet = RefBuffer<BYTE> (that.mSet.size ()) ;
		fake.mSize = that.mSize ;
		for (auto &&i : iter (0 ,that.mSet.size ())) {
			fake.mSet[i] = that.mSet[i] ;
		}
		if ifdo (TRUE) {
			INDEX ix = fake.mSet.size () - 1 ;
			if (ix < 0)
				discard ;
			fake.mSet[ix] &= fake.mMask ;
		}
	}

	void clear () override {
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			fake.mSet[i] = BYTE (0X00) ;
		}
	}

	LENGTH size () const override {
		if (fake.mSet.size () == 0)
			return 0 ;
		return fake.mSize ;
	}

	LENGTH length () const override {
		LENGTH ret = 0 ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			const auto r1x = fake.mSet[i] ;
			ret += count_bit_size (r1x) ;
		}
		return move (ret) ;
	}

	imports LENGTH count_bit_size (CREF<BYTE> item) {
		static const auto &&mInstance = DEF<LENGTH[]> {
			0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8} ;
		return mInstance[INDEX (item)] ;
	}

	imports INDEX find_first_bit (CREF<BYTE> item) {
		static const auto &&mInstance = DEF<INDEX[]> {
			8 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			7 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0} ;
		return mInstance[INDEX (item)] ;
	}

	INDEX find_next_bit (CREF<INDEX> index) const {
		for (auto &&i : iter (index ,fake.mSet.size ())) {
			const auto r2x = fake.mSet[i] ;
			if (r2x == BYTE (0X00))
				continue ;
			return i * 8 + find_first_bit (r2x) ;
		}
		return fake.mSize ;
	}

	void get (CREF<INDEX> index ,VREF<BOOL> item) const override {
		const auto r1x = index / 8 ;
		const auto r2x = index % 8 ;
		const auto r3x = fake.mSet[r1x] & BYTE (1 << r2x) ;
		item = r3x != BYTE (0X00) ;
	}

	void set (CREF<INDEX> index ,CREF<BOOL> item) override {
		const auto r1x = index / 8 ;
		const auto r2x = index % 8 ;
		if ifdo (TRUE) {
			if (!item)
				discard ;
			fake.mSet[r1x] |= BYTE (1 << r2x) ;
			return ;
		}
		if ifdo (TRUE) {
			fake.mSet[r1x] &= ~BYTE (1 << r2x) ;
			return ;
		}
	}

	INDEX ibegin () const override {
		if (fake.mSet.size () == 0)
			return NONE ;
		return find_next_bit (0) ;
	}

	INDEX iend () const override {
		if (fake.mSet.size () == 0)
			return NONE ;
		return fake.mSize ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		const auto r1x = index / 8 ;
		const auto r2x = index % 8 + 1 ;
		const auto r3x = fake.mSet[r1x] ;
		const auto r4x = BYTE (r3x >> r2x) ;
		if (r4x == BYTE (0X00))
			return find_next_bit (r1x + 1) ;
		const auto r5x = r2x + find_first_bit (r4x) ;
		return r1x * 8 + r5x ;
	}

	BOOL equal (CREF<BitSetLayout> that) const override {
		//@mark
		return TRUE ;
	}

	FLAG compr (CREF<BitSetLayout> that) const override {
		//@mark
		return NONE ;
	}

	void visit (CREF<Visitor> visitor) const override {
		//@mark
	}

	void fill (CREF<BYTE> item) override {
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			fake.mSet[i] = item ;
		}
		if ifdo (TRUE) {
			INDEX ix = fake.mSet.size () - 1 ;
			if (ix < 0)
				discard ;
			fake.mSet[ix] &= fake.mMask ;
		}
	}

	void add (CREF<INDEX> item) override {
		assume (operator_between (item ,0 ,size ())) ;
		set (item ,TRUE) ;
	}

	BOOL contain (CREF<INDEX> item) const override {
		BOOL ret = FALSE ;
		if ifdo (TRUE) {
			if (operator_between (item ,0 ,size ()))
				discard ;
			get (item ,ret) ;
		}
		return move (ret) ;
	}

	void erase (CREF<INDEX> item) override {
		assume (operator_between (item ,0 ,size ())) ;
		set (item ,FALSE) ;
	}
} ;

exports VFat<BitSetHolder> BitSetHolder::create (VREF<BitSetLayout> that) {
	return VFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}

exports CFat<BitSetHolder> BitSetHolder::create (CREF<BitSetLayout> that) {
	return CFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}
} ;