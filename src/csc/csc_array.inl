﻿#pragma once

#ifndef __CSC_ARRAY__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_array.hpp"

#include "csc_end.h"
#include <algorithm>
#include "csc_begin.h"

namespace CSC {
struct FUNCTION_from_initializer_list {
	forceinline RefBuffer<Pointer> operator() (CREF<WrapperLayout> params ,CREF<Unknown> holder) const {
		RefBuffer<Pointer> ret ;
		auto &&rax = keep[TYPE<Wrapper<CREF<Pointer>>>::expr] (params) ;
		rax ([&] (CREF<Pointer> a) {
			const auto r1x = RFat<ReflectSize> (holder) ;
			const auto r2x = r1x->type_size () ;
			const auto r3x = inline_list_pair (a ,r2x) ;
			const auto r4x = (r3x.m2nd - r3x.m1st) / r2x ;
			const auto r5x = Slice (r3x.m1st ,r4x ,r2x) ;
			RefBufferHolder::hold (ret)->initialize (holder ,r5x) ;
		}) ;
		return move (ret) ;
	}
} ;

static constexpr auto from_initializer_list = FUNCTION_from_initializer_list () ;

class ArrayImplHolder final implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefBufferHolder::hold (fake.mArray)->initialize (holder ,size_) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			r1x->assign (at (i) ,BoxHolder::hold (item)->self) ;
		}
	}

	void initialize (CREF<ArrayLayout> that) override {
		const auto r1x = ArrayHolder::hold (that)->length () ;
		if (r1x == 0)
			return ;
		initialize (that.mArray.unknown () ,r1x) ;
		splice (0 ,that) ;
	}

	LENGTH size () const override {
		return fake.mArray.size () ;
	}

	LENGTH step () const override {
		return fake.mArray.step () ;
	}

	LENGTH length () const override {
		return fake.mArray.size () ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return RefBufferHolder::hold (fake.mArray)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return RefBufferHolder::hold (fake.mArray)->self ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return fake.mArray.at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mArray.at (index) ;
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
		const auto r2x = ArrayHolder::hold (that)->length () ;
		if (r1x != r2x)
			return FALSE ;
		const auto r3x = RFat<ReflectEqual> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r4x = r3x->equal (at (i) ,ArrayHolder::hold (that)->at (i)) ;
			if (!r4x)
				return r4x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<ArrayLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = ArrayHolder::hold (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		const auto r4x = RFat<ReflectCompr> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r3x)) {
			const auto r5x = r4x->compr (at (i) ,ArrayHolder::hold (that)->at (i)) ;
			if (r5x != ZERO)
				return r5x ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		const auto r2x = RFat<ReflectVisit> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			r2x->visit (visitor ,at (i)) ;
		}
		visitor.leave () ;
	}

	void fill (CREF<Pointer> item) override {
		const auto r1x = size () ;
		const auto r2x = RFat<ReflectClone> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			r2x->clone (at (i) ,item) ;
		}
	}

	void splice (CREF<INDEX> index ,CREF<ArrayLayout> item) override {
		const auto r1x = ArrayHolder::hold (item)->size () ;
		if (r1x == 0)
			return ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		const auto r2x = RFat<ReflectClone> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = index + i ;
			r2x->clone (at (ix) ,ArrayHolder::hold (item)->at (i)) ;
		}
	}
} ;

exports VFat<ArrayHolder> ArrayHolder::hold (VREF<ArrayLayout> that) {
	return VFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

exports CFat<ArrayHolder> ArrayHolder::hold (CREF<ArrayLayout> that) {
	return CFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

class StringImplHolder final implement Fat<StringHolder ,StringLayout> {
public:
	void initialize (CREF<Slice> that ,CREF<LENGTH> step_) override {
		const auto r1x = SliceHolder::hold (that)->size () ;
		initialize (r1x ,step_) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = size_ + 1 ;
		auto act = TRUE ;
		if ifdo (act) {
			if (step_ != SIZE_OF<STRU8>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mString)) ;
			rax = RefBuffer<STRU8> (r1x) ;
		}
		if ifdo (act) {
			if (step_ != SIZE_OF<STRU16>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU16>>::expr] (Pointer::from (fake.mString)) ;
			rax = RefBuffer<STRU16> (r1x) ;
		}
		if ifdo (act) {
			if (step_ != SIZE_OF<STRU32>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU32>>::expr] (Pointer::from (fake.mString)) ;
			rax = RefBuffer<STRU32> (r1x) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		clear () ;
	}

	void initialize (CREF<StringLayout> that) override {
		const auto r1x = StringHolder::hold (that)->length () ;
		const auto r2x = StringHolder::hold (that)->step () ;
		if (r1x == 0)
			return ;
		initialize (r1x ,r2x) ;
		splice (0 ,that) ;
	}

	void clear () override {
		trunc (0) ;
		trunc (size ()) ;
	}

	FLAG encode () const override {
		if (!fake.mString.exist ())
			return 0 ;
		return fake.mEncode ;
	}

	LENGTH size () const override {
		if (!fake.mString.exist ())
			return 0 ;
		return fake.mString.size () - 1 ;
	}

	LENGTH step () const override {
		return fake.mString.step () ;
	}

	LENGTH length () const override {
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,size ())) {
			get (i ,rax) ;
			if (rax == 0)
				return i ;
		}
		return size () ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return RefBufferHolder::hold (fake.mString)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return RefBufferHolder::hold (fake.mString)->self ;
	}

	Ref<RefBuffer<BYTE>> borrow () leftvalue override {
		assert (fake.mString.exist ()) ;
		auto &&rax = keep[TYPE<RefBuffer<BYTE>>::expr] (Pointer::from (fake.mString)) ;
		return Ref<RefBuffer<BYTE>>::reference (rax) ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue override {
		assert (fake.mString.exist ()) ;
		auto &&rax = keep[TYPE<RefBuffer<BYTE>>::expr] (Pointer::from (fake.mString)) ;
		return Ref<RefBuffer<BYTE>>::reference (rax) ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != SIZE_OF<STRU8>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mString)) ;
			item = rax[index] ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU16>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mString)) ;
			item = rax[index] ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU32>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mString)) ;
			item = rax[index] ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void set (CREF<INDEX> index ,CREF<STRU32> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != SIZE_OF<STRU8>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU8>>::expr] (Pointer::from (fake.mString)) ;
			rax[index] = STRU8 (item) ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU16>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU16>>::expr] (Pointer::from (fake.mString)) ;
			rax[index] = STRU16 (item) ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU32>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<STRU32>>::expr] (Pointer::from (fake.mString)) ;
			rax[index] = STRU32 (item) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return fake.mString.at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mString.at (index) ;
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

	BOOL equal (CREF<Slice> that) const override {
		const auto r1x = length () ;
		const auto r2x = that.size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			const auto r4x = inline_equal (rax ,that[i]) ;
			if (!r4x)
				return r4x ;
			if (rax == 0)
				return TRUE ;
		}
		return inline_equal (r1x ,r2x) ;
	}

	BOOL equal (CREF<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::hold (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			StringHolder::hold (that)->get (i ,rbx) ;
			const auto r4x = inline_equal (rax ,rbx) ;
			if (!r4x)
				return r4x ;
			if (rax == 0)
				return TRUE ;
		}
		return inline_equal (r1x ,r2x) ;
	}

	FLAG compr (CREF<Slice> that) const override {
		const auto r1x = length () ;
		const auto r2x = that.size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			const auto r4x = inline_compr (rax ,that[i]) ;
			if (r4x != ZERO)
				return r4x ;
			if (rax == 0)
				return ZERO ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	FLAG compr (CREF<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::hold (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			StringHolder::hold (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
			if (rax == 0)
				return ZERO ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			if (rax == 0)
				break ;
			const auto r2x = bitwise[TYPE<CHAR>::expr] (rax) ;
			visitor.push (r2x) ;
		}
		visitor.leave () ;
	}

	void trunc (CREF<INDEX> index) override {
		if (!inline_between (index ,0 ,fake.mString.size ()))
			return ;
		set (index ,STRU32 (0X00)) ;
	}

	void fill (CREF<STRU32> item) override {
		for (auto &&i : iter (0 ,size ())) {
			set (i ,item) ;
		}
	}

	void splice (CREF<INDEX> index ,CREF<Slice> item) override {
		const auto r1x = SliceHolder::hold (item)->size () ;
		if (r1x == 0)
			return ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = index + i ;
			set (ix ,item[i]) ;
		}
	}

	void splice (CREF<INDEX> index ,CREF<StringLayout> item) override {
		const auto r1x = StringHolder::hold (item)->length () ;
		if (r1x == 0)
			return ;
		assert (inline_between (index ,0 ,size ())) ;
		const auto r2x = index + r1x ;
		assert (index + r1x <= size ()) ;
		const auto r3x = step () ;
		inline_memcpy (fake.mString[index] ,item.mString[0] ,r1x * r3x) ;
		trunc (r2x) ;
	}

	Slice segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const override {
		if (begin_ >= end_)
			return Slice () ;
		return Slice (address (fake.mString[begin_]) ,end_ - begin_ ,step ()) ;
	}
} ;

exports VFat<StringHolder> StringHolder::hold (VREF<StringLayout> that) {
	return VFat<StringHolder> (StringImplHolder () ,that) ;
}

exports CFat<StringHolder> StringHolder::hold (CREF<StringLayout> that) {
	return CFat<StringHolder> (StringImplHolder () ,that) ;
}

class DequeImplHolder final implement Fat<DequeHolder ,DequeLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mDeque.exist ())
			return ;
		RefBufferHolder::hold (fake.mDeque)->prepare (holder) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		RefBufferHolder::hold (fake.mDeque)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item)) ;
		}
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		return fake.mDeque.size () ;
	}

	LENGTH step () const override {
		return fake.mDeque.step () ;
	}

	LENGTH length () const override {
		if (!fake.mDeque.exist ())
			return 0 ;
		return fake.mWrite - fake.mRead ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		assert (inline_between (index ,0 ,length ())) ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = (index + fake.mRead) % r1x ;
		return fake.mDeque.at (ix) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		assert (inline_between (index ,0 ,length ())) ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = (index + fake.mRead) % r1x ;
		return fake.mDeque.at (ix) ;
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
		if (!fake.mDeque.exist ())
			return TRUE ;
		return fake.mRead == fake.mWrite ;
	}

	BOOL full () const override {
		const auto r1x = size () ;
		if (r1x == 0)
			return FALSE ;
		return length () >= r1x ;
	}

	INDEX head () const override {
		return 0 ;
	}

	INDEX tail () const override {
		return length () - 1 ;
	}

	void add (RREF<BoxLayout> item) override {
		check_resize () ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = fake.mWrite % r1x ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->assign (fake.mDeque.at (ix) ,BoxHolder::hold (item)->self) ;
		fake.mWrite++ ;
	}

	void take () override {
		const auto r1x = fake.mDeque.size () ;
		noop (r1x) ;
		assert (r1x > 0) ;
		INDEX ix = fake.mRead ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->assign (fake.mDeque[ix]) ;
		fake.mRead++ ;
		check_bound () ;
	}

	void push (RREF<BoxLayout> item) override {
		check_resize () ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = (fake.mRead - 1 + r1x) % r1x ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->assign (fake.mDeque.at (ix) ,BoxHolder::hold (item)->self) ;
		fake.mRead-- ;
		check_bound () ;
	}

	void pop () override {
		const auto r1x = fake.mDeque.size () ;
		noop (r1x) ;
		assert (r1x > 0) ;
		INDEX ix = fake.mWrite - 1 ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->assign (fake.mDeque[ix]) ;
		fake.mWrite-- ;
	}

	void ring (CREF<LENGTH> count) override {
		if (size () <= 0)
			return ;
		fake.mRead += count ;
		fake.mWrite += count ;
		check_bound () ;
	}

	void check_bound () {
		const auto r1x = fake.mDeque.size () ;
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

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::hold (fake.mDeque)->resize (r2x) ;
		if ifdo (TRUE) {
			if (fake.mWrite <= r1x)
				discard ;
			const auto r3x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
			for (auto &&i : iter (0 ,r1x - fake.mRead)) {
				INDEX ix = r1x - 1 - i ;
				INDEX iy = r2x - 1 - i ;
				r3x->assign (fake.mDeque.at (iy) ,fake.mDeque.at (ix)) ;
			}
			fake.mRead += r2x - r1x ;
			check_bound () ;
		}
	}
} ;

exports VFat<DequeHolder> DequeHolder::hold (VREF<DequeLayout> that) {
	return VFat<DequeHolder> (DequeImplHolder () ,that) ;
}

exports CFat<DequeHolder> DequeHolder::hold (CREF<DequeLayout> that) {
	return CFat<DequeHolder> (DequeImplHolder () ,that) ;
}

class PriorityImplHolder final implement Fat<PriorityHolder ,PriorityLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mPriority.exist ())
			return ;
		RefBufferHolder::hold (fake.mPriority)->prepare (holder) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = size_ + 1 ;
		RefBufferHolder::hold (fake.mPriority)->initialize (holder ,r1x) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item)) ;
		}
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		if (!fake.mPriority.exist ())
			return 0 ;
		return fake.mPriority.size () - 1 ;
	}

	LENGTH step () const override {
		return fake.mPriority.step () ;
	}

	LENGTH length () const override {
		if (!fake.mPriority.exist ())
			return 0 ;
		return fake.mWrite ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mPriority.at (index) ;
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
		if (!fake.mPriority.exist ())
			return TRUE ;
		return fake.mRead == fake.mWrite ;
	}

	BOOL full () const override {
		const auto r1x = size () ;
		if (r1x == 0)
			return FALSE ;
		return length () >= r1x ;
	}

	INDEX head () const override {
		return 0 ;
	}

	void add (RREF<BoxLayout> item) override {
		check_resize () ;
		INDEX ix = fake.mWrite ;
		const auto r1x = RFat<ReflectAssign> (fake.mPriority.unknown ()) ;
		r1x->assign (fake.mPriority.at (ix) ,BoxHolder::hold (item)->self) ;
		fake.mWrite++ ;
		update_insert (ix) ;
	}

	void take () override {
		assert (!empty ()) ;
		INDEX ix = fake.mWrite - 1 ;
		const auto r1x = RFat<ReflectAssign> (fake.mPriority.unknown ()) ;
		r1x->assign (fake.mPriority.at (0) ,fake.mPriority.at (ix)) ;
		fake.mWrite = ix ;
		update_insert (0) ;
	}

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::hold (fake.mPriority)->resize (r2x) ;
	}

	void update_insert (CREF<INDEX> curr) {
		INDEX ix = curr ;
		INDEX jy = fake.mWrite ;
		const auto r1x = RFat<ReflectAssign> (fake.mPriority.unknown ()) ;
		const auto r2x = RFat<ReflectCompr> (fake.mPriority.unknown ()) ;
		r1x->assign (fake.mPriority.at (jy) ,fake.mPriority.at (ix)) ;
		while (TRUE) {
			INDEX iy = parent (ix) ;
			if (iy < 0)
				break ;
			const auto r3x = r2x->compr (fake.mPriority.at (jy) ,fake.mPriority.at (iy)) ;
			if (r3x >= 0)
				break ;
			r1x->assign (fake.mPriority.at (ix) ,fake.mPriority.at (iy)) ;
			ix = iy ;
		}
		while (TRUE) {
			INDEX iy = left_child (ix) ;
			if (iy >= fake.mWrite)
				break ;
			INDEX jx = jy ;
			if ifdo (TRUE) {
				const auto r4x = r2x->compr (fake.mPriority.at (jx) ,fake.mPriority.at (iy)) ;
				if (r4x <= 0)
					discard ;
				jx = iy ;
			}
			INDEX iz = right_child (ix) ;
			if ifdo (TRUE) {
				if (iz >= fake.mWrite)
					discard ;
				const auto r5x = r2x->compr (fake.mPriority.at (jx) ,fake.mPriority.at (iz)) ;
				if (r5x <= 0)
					discard ;
				jx = iz ;
			}
			if (jx == ix)
				break ;
			r1x->assign (fake.mPriority.at (ix) ,fake.mPriority.at (jx)) ;
			ix = jx ;
		}
		r1x->assign (fake.mPriority.at (ix) ,fake.mPriority.at (jy)) ;
	}

	INDEX parent (CREF<INDEX> curr) const {
		if (curr == 0)
			return NONE ;
		return (curr - 1) / 2 ;
	}

	INDEX left_child (CREF<INDEX> curr) const {
		return curr * 2 + 1 ;
	}

	INDEX right_child (CREF<INDEX> curr) const {
		return curr * 2 + 2 ;
	}
} ;

exports VFat<PriorityHolder> PriorityHolder::hold (VREF<PriorityLayout> that) {
	return VFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

exports CFat<PriorityHolder> PriorityHolder::hold (CREF<PriorityLayout> that) {
	return CFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

class ListImplHolder final implement Fat<ListHolder ,ListLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mList.exist ())
			return ;
		AllocatorHolder::hold (fake.mList)->prepare (holder) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::hold (fake.mList)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item)) ;
		}
	}

	void clear () override {
		fake.mList.clear () ;
		fake.mFirst = NONE ;
		fake.mLast = NONE ;
	}

	LENGTH size () const override {
		return fake.mList.size () ;
	}

	LENGTH step () const override {
		return fake.mList.step () ;
	}

	LENGTH length () const override {
		return fake.mList.length () ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return fake.mList.at (index) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mList.at (index) ;
	}

	INDEX ibegin () const override {
		if (!fake.mList.exist ())
			return NONE ;
		return fake.mFirst ;
	}

	INDEX iend () const override {
		return NONE ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return fake.mList.bt (index).mRight ;
	}

	BOOL empty () const override {
		if (!fake.mList.exist ())
			return TRUE ;
		return fake.mFirst == NONE ;
	}

	BOOL full () const override {
		const auto r1x = size () ;
		if (r1x == 0)
			return FALSE ;
		return length () >= r1x ;
	}

	INDEX head () const override {
		return fake.mFirst ;
	}

	INDEX tail () const override {
		return fake.mLast ;
	}

	void add (RREF<BoxLayout> item) override {
		INDEX ix = fake.mList.alloc (move (item)) ;
		fake.mList.bt (ix).mLeft = fake.mLast ;
		fake.mList.bt (ix).mRight = NONE ;
		curr_next (fake.mLast) = ix ;
		curr_prev (NONE) = ix ;
	}

	void take () override {
		assert (!empty ()) ;
		INDEX ix = fake.mFirst ;
		INDEX iy = fake.mList.bt (ix).mRight ;
		curr_next (NONE) = iy ;
		curr_prev (iy) = NONE ;
		fake.mList.free (ix) ;
	}

	void push (RREF<BoxLayout> item) override {
		INDEX ix = fake.mList.alloc (move (item)) ;
		fake.mList.bt (ix).mLeft = NONE ;
		fake.mList.bt (ix).mRight = fake.mFirst ;
		curr_next (NONE) = ix ;
		curr_prev (fake.mFirst) = ix ;
	}

	void pop () override {
		assert (!empty ()) ;
		INDEX ix = fake.mLast ;
		INDEX iy = fake.mList.bt (ix).mLeft ;
		curr_next (iy) = NONE ;
		curr_prev (NONE) = iy ;
		fake.mList.free (ix) ;
	}

	INDEX insert (RREF<BoxLayout> item) override {
		INDEX ret = fake.mList.alloc (move (item)) ;
		fake.mList.bt (ret).mLeft = fake.mLast ;
		fake.mList.bt (ret).mRight = NONE ;
		curr_next (fake.mLast) = ret ;
		curr_prev (NONE) = ret ;
		return move (ret) ;
	}

	INDEX insert (CREF<INDEX> index ,RREF<BoxLayout> item) override {
		assert (fake.mList.used (index)) ;
		INDEX ret = fake.mList.alloc (move (item)) ;
		INDEX ix = fake.mList.bt (index).mLeft ;
		fake.mList.bt (ret).mLeft = ix ;
		fake.mList.bt (ret).mRight = index ;
		curr_next (ix) = ret ;
		curr_prev (index) = ret ;
		return move (ret) ;
	}

	void remove (CREF<INDEX> index) override {
		assert (!empty ()) ;
		INDEX ix = fake.mList.bt (index).mLeft ;
		INDEX iy = fake.mList.bt (index).mRight ;
		curr_next (ix) = iy ;
		curr_prev (iy) = ix ;
		fake.mList.free (index) ;
	}

	void order (CREF<Array<INDEX>> range_) override {
		assert (length () == range_.length ()) ;
		if (range_.length () == 0)
			return ;
		if ifdo (TRUE) {
			INDEX ix = range_[0] ;
			fake.mList.bt (ix).mLeft = NONE ;
			fake.mFirst = ix ;
		}
		for (auto &&i : iter (1 ,range_.length ())) {
			INDEX ix = range_[i - 1] ;
			INDEX iy = range_[i] ;
			fake.mList.bt (ix).mRight = iy ;
			fake.mList.bt (iy).mLeft = ix ;
		}
		if ifdo (TRUE) {
			INDEX ix = range_[range_.length () - 1] ;
			fake.mList.bt (ix).mRight = NONE ;
			fake.mLast = ix ;
		}
	}

	VREF<INDEX> curr_next (CREF<INDEX> index) leftvalue {
		if (index == NONE)
			return fake.mFirst ;
		return fake.mList.bt (index).mRight ;
	}

	VREF<INDEX> curr_prev (CREF<INDEX> index) leftvalue {
		if (index == NONE)
			return fake.mLast ;
		return fake.mList.bt (index).mLeft ;
	}
} ;

exports VFat<ListHolder> ListHolder::hold (VREF<ListLayout> that) {
	return VFat<ListHolder> (ListImplHolder () ,that) ;
}

exports CFat<ListHolder> ListHolder::hold (CREF<ListLayout> that) {
	return CFat<ListHolder> (ListImplHolder () ,that) ;
}

class ArrayListImplHolder final implement Fat<ArrayListHolder ,ArrayListLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mList.exist ())
			return ;
		AllocatorHolder::hold (fake.mList)->prepare (holder) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::hold (fake.mList)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item)) ;
		}
	}

	void clear () override {
		fake.mList.clear () ;
		fake.mRange = RefBuffer<INDEX> () ;
		fake.mTop = 0 ;
	}

	LENGTH size () const override {
		return fake.mList.size () ;
	}

	LENGTH step () const override {
		return fake.mList.step () ;
	}

	LENGTH length () const override {
		return fake.mList.length () ;
	}

	VREF<Pointer> at (CREF<INDEX> index) leftvalue override {
		return fake.mList.at (fake.mRange[index]) ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mList.at (fake.mRange[index]) ;
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

	void add (RREF<BoxLayout> item) override {
		INDEX ix = fake.mList.alloc (move (item)) ;
		check_resize () ;
		INDEX iy = find_free () ;
		assert (fake.mRange[iy] == NONE) ;
		fake.mRange[iy] = ix ;
		fake.mRemap = FALSE ;
	}

	INDEX insert (RREF<BoxLayout> item) override {
		INDEX ix = fake.mList.alloc (move (item)) ;
		check_resize () ;
		INDEX ret = find_free () ;
		assert (fake.mRange[ret] == NONE) ;
		fake.mRange[ret] = ix ;
		fake.mRemap = FALSE ;
		return move (ret) ;
	}

	INDEX insert (CREF<INDEX> index ,RREF<BoxLayout> item) override {
		assert (inline_between (index ,0 ,fake.mRange.size ())) ;
		assert (!fake.mList.used (fake.mRange[index])) ;
		INDEX ix = fake.mList.alloc (move (item)) ;
		check_resize () ;
		INDEX ret = index ;
		assert (fake.mRange[ret] == NONE) ;
		fake.mRange[ret] = ix ;
		fake.mRemap = FALSE ;
		return move (ret) ;
	}

	void remove (CREF<INDEX> index) override {
		INDEX ix = fake.mRange[index] ;
		fake.mRange[index] = NONE ;
		fake.mList.free (ix) ;
		fake.mRemap = FALSE ;
	}

	void order (CREF<Array<INDEX>> range_) override {
		assert (length () == range_.length ()) ;
		if (range_.length () == 0)
			return ;
		auto rax = RefBuffer<INDEX> (fake.mRange.size ()) ;
		for (auto &&i : range_.range ()) {
			INDEX ix = range_[i] ;
			rax[i] = fake.mRange[ix] ;
		}
		for (auto &&i : iter (range_.length () ,fake.mRange.size ()))
			rax[i] = NONE ;
		fake.mRange = move (rax) ;
	}

	void remap () override {
		INDEX ix = 0 ;
		INDEX iy = 0 ;
		while (TRUE) {
			iy = find_next (iy) ;
			if (iy >= fake.mRange.size ())
				break ;
			fake.mRange[ix] = fake.mRange[iy] ;
			ix++ ;
			iy++ ;
		}
		fake.mTop = ix ;
		while (TRUE) {
			if (ix >= fake.mRange.size ())
				break ;
			fake.mRange[ix] = NONE ;
			ix++ ;
		}
		fake.mRemap = TRUE ;
	}

	INDEX find_free () {
		const auto r1x = fake.mRange.size () ;
		assert (r1x > 0) ;
		const auto r2x = fake.mTop % r1x ;
		INDEX ret = r2x ;
		while (TRUE) {
			if (fake.mRange[ret] == NONE)
				break ;
			ret++ ;
			replace (ret ,r1x ,0) ;
			if (ret == r2x)
				break ;
		}
		assert (fake.mRange[ret] == NONE) ;
		fake.mTop = ret ;
		return move (ret) ;
	}

	void check_resize () {
		const auto r1x = fake.mList.size () ;
		const auto r2x = fake.mRange.size () ;
		if (r2x == r1x)
			return ;
		assert (r2x <= r1x) ;
		fake.mRange.resize (r1x) ;
		for (auto &&i : iter (r2x ,r1x)) {
			fake.mRange[i] = NONE ;
		}
	}
} ;

exports VFat<ArrayListHolder> ArrayListHolder::hold (VREF<ArrayListLayout> that) {
	return VFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

exports CFat<ArrayListHolder> ArrayListHolder::hold (CREF<ArrayListLayout> that) {
	return CFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

class SortedMapImplHolder final implement Fat<SortedMapHolder ,SortedMapLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mThis.exist ())
			return ;
		assume (FALSE) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		fake.mThis = Ref<SortedMapRoot>::make () ;
		AllocatorHolder::hold (fake.mThis->mList)->initialize (holder ,size_) ;
		fake.mThis->mCheck = 0 ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item) ,NONE) ;
		}
	}

	SortedMapLayout share () const override {
		assert (fake.mRoot == NONE) ;
		SortedMapLayout ret ;
		ret.mThis = fake.mThis ;
		ret.mRoot = NONE ;
		ret.mRange = RefBuffer<INDEX> () ;
		ret.mWrite = 0 ;
		ret.mRemap = FALSE ;
		return move (ret) ;
	}

	void clear () override {
		fake.mRoot = NONE ;
		fake.mRange = RefBuffer<INDEX> () ;
		fake.mWrite = 0 ;
		fake.mRemap = FALSE ;
	}

	LENGTH size () const override {
		return fake.mRange.size () ;
	}

	LENGTH step () const override {
		return fake.mRange.step () ;
	}

	LENGTH length () const override {
		if (!fake.mThis.exist ())
			return 0 ;
		return fake.mWrite ;
	}

	VREF<INDEX> at (CREF<INDEX> index) leftvalue override {
		return fake.mThis->mList.bt (fake.mRange[index]).mMap ;
	}

	CREF<INDEX> at (CREF<INDEX> index) const leftvalue override {
		return fake.mThis->mList.bt (fake.mRange[index]).mMap ;
	}

	INDEX ibegin () const override {
		assert (fake.mRemap) ;
		return 0 ;
	}

	INDEX iend () const override {
		assert (fake.mRemap) ;
		return length () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return index + 1 ;
	}

	void add (RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		INDEX ix = fake.mThis->mList.alloc (move (item)) ;
		fake.mThis->mCheck++ ;
		fake.mThis->mList.bt (ix).mMap = map_ ;
		fake.mThis->mList.bt (ix).mDown = fake.mRoot ;
		fake.mRoot = ix ;
		fake.mWrite++ ;
		fake.mRemap = FALSE ;
	}

	INDEX find (CREF<Pointer> item) const override {
		if (!fake.mThis.exist ())
			return NONE ;
		assert (fake.mRemap) ;
		INDEX ix = 0 ;
		INDEX iy = length () - 1 ;
		INDEX iz = 0 ;
		const auto r1x = RFat<ReflectCompr> (fake.mThis->mList.unknown ()) ;
		while (TRUE) {
			if (ix > iy)
				break ;
			iz = ix + (iy - ix) / 2 ;
			const auto r2x = r1x->compr (item ,fake.mThis->mList.at (fake.mRange[iz])) ;
			if (r2x == ZERO)
				return iz ;
			auto act = TRUE ;
			if ifdo (act) {
				if (r2x >= ZERO)
					discard ;
				iy = iz - 1 ;
			}
			if ifdo (act) {
				ix = iz + 1 ;
			}
		}
		return NONE ;
	}

	BOOL contain (CREF<Pointer> item) const override {
		return find (item) != NONE ;
	}

	INDEX map (CREF<Pointer> item) const override {
		INDEX ix = find (item) ;
		if (ix == NONE)
			return NONE ;
		return at (ix) ;
	}

	void remap () override {
		if (!fake.mThis.exist ())
			return ;
		if (fake.mRemap)
			return ;
		if (fake.mWrite == 0)
			return ;
		auto &&rax = fake.mThis.self ;
		const auto r1x = RFat<ReflectCompr> (rax.mList.unknown ()) ;
		const auto r2x = RFat<ReflectEqual> (rax.mList.unknown ()) ;
		if ifdo (TRUE) {
			fake.mRange = RefBuffer<INDEX> (fake.mWrite) ;
			INDEX ix = fake.mRoot ;
			for (auto &&i : iter (0 ,fake.mWrite)) {
				assert (ix != NONE) ;
				fake.mRange[i] = ix ;
				ix = rax.mList.bt (ix).mDown ;
			}
			assert (ix == NONE) ;
		}
		if ifdo (TRUE) {
			const auto r3x = (&fake.mRange[0]) ;
			const auto r4x = r3x + fake.mRange.size () ;
			std::sort (r3x ,r4x ,[&] (CREF<INDEX> a ,CREF<INDEX> b) {
				return r1x->compr (rax.mList[a] ,rax.mList[b]) < ZERO ;
			}) ;
		}
		if ifdo (TRUE) {
			INDEX ix = 0 ;
			for (auto &&i : iter (1 ,fake.mWrite)) {
				const auto r5x = r2x->equal (rax.mList[fake.mRange[ix]] ,rax.mList[fake.mRange[i]]) ;
				if (r5x)
					continue ;
				ix++ ;
				fake.mRange[ix] = fake.mRange[i] ;
			}
			ix++ ;
			//@warn: length would be decresed due to remove the same item
			fake.mWrite = ix ;
			for (auto &&i : iter (fake.mWrite ,fake.mRange.size ()))
				fake.mRange[i] = NONE ;
		}
		if ifdo (TRUE) {
			fake.mRoot = fake.mRange[0] ;
			for (auto &&i : iter (0 ,fake.mWrite - 1)) {
				rax.mList.bt (fake.mRange[i]).mDown = fake.mRange[i + 1] ;
			}
			INDEX ix = fake.mRange[fake.mWrite - 1] ;
			rax.mList.bt (ix).mDown = NONE ;
		}
		fake.mRemap = TRUE ;
	}
} ;

exports VFat<SortedMapHolder> SortedMapHolder::hold (VREF<SortedMapLayout> that) {
	return VFat<SortedMapHolder> (SortedMapImplHolder () ,that) ;
}

exports CFat<SortedMapHolder> SortedMapHolder::hold (CREF<SortedMapLayout> that) {
	return CFat<SortedMapHolder> (SortedMapImplHolder () ,that) ;
}

struct SetChild {
	INDEX mUp ;
	BOOL mBin ;
} ;

class SetImplHolder final implement Fat<SetHolder ,SetLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mSet.exist ())
			return ;
		AllocatorHolder::hold (fake.mSet)->prepare (holder) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::hold (fake.mSet)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item) ,NONE) ;
		}
	}

	void clear () override {
		fake.mSet.clear () ;
		fake.mRoot = NONE ;
		fake.mTop = 0 ;
	}

	LENGTH size () const override {
		return fake.mSet.size () ;
	}

	LENGTH step () const override {
		return fake.mSet.step () ;
	}

	LENGTH length () const override {
		return fake.mSet.length () ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mSet.at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const override {
		map_ = fake.mSet.bt (index).mMap ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) override {
		fake.mSet.bt (index).mMap = map_ ;
	}

	INDEX ibegin () const override {
		return find_next (0) ;
	}

	INDEX iend () const override {
		return fake.mSet.size () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return find_next (index + 1) ;
	}

	INDEX find_next (CREF<INDEX> index) const {
		INDEX ret = index ;
		while (TRUE) {
			if (ret >= fake.mSet.size ())
				break ;
			if (fake.mSet.used (ret))
				break ;
			ret++ ;
		}
		return move (ret) ;
	}

	void add (RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		if ifdo (TRUE) {
			INDEX ix = find (BoxHolder::hold (item)->self) ;
			if (ix != NONE)
				discard ;
			ix = fake.mSet.alloc (move (item)) ;
			fake.mSet.bt (ix).mMap = map_ ;
			fake.mSet.bt (ix).mRed = TRUE ;
			fake.mSet.bt (ix).mLeft = NONE ;
			fake.mSet.bt (ix).mRight = NONE ;
			update_emplace (ix) ;
			update_insert (ix) ;
		}
	}

	BOOL red_node (CREF<INDEX> curr) const {
		if (curr == NONE)
			return FALSE ;
		return fake.mSet.bt (curr).mRed ;
	}

	void update_emplace (CREF<INDEX> curr) {
		INDEX ix = fake.mRoot ;
		auto rax = binary_child (NONE ,ZERO) ;
		const auto r1x = RFat<ReflectCompr> (fake.mSet.unknown ()) ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			const auto r2x = r1x->compr (fake.mSet.at (curr) ,fake.mSet.at (ix)) ;
			assert (r2x != ZERO) ;
			rax = binary_child (ix ,r2x) ;
			ix = curr_next (rax) ;
		}
		curr_next (rax) = curr ;
		curr_prev (curr ,rax.mBin) = rax.mUp ;
	}

	void update_insert (CREF<INDEX> curr) {
		INDEX ix = curr ;
		INDEX iy = fake.mSet.bt (ix).mUp ;
		while (TRUE) {
			if (!red_node (iy))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (fake.mSet.bt (iy).mBin)
					discard ;
				update_insert_left (ix) ;
				ix = fake.mTop ;
			}
			if ifdo (act) {
				update_insert_right (ix) ;
				ix = fake.mTop ;
			}
			iy = fake.mSet.bt (ix).mUp ;
		}
		fake.mSet.bt (fake.mRoot).mRed = FALSE ;
	}

	void update_insert_left (CREF<INDEX> curr) {
		INDEX ix = fake.mSet.bt (curr).mUp ;
		INDEX iy = fake.mSet.bt (ix).mUp ;
		auto act = TRUE ;
		if ifdo (act) {
			INDEX jx = fake.mSet.bt (iy).mRight ;
			if (!red_node (jx))
				discard ;
			fake.mSet.bt (jx).mRed = FALSE ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			fake.mTop = iy ;
		}
		if ifdo (act) {
			if (!fake.mSet.bt (curr).mBin)
				discard ;
			rotate_left (fake.mSet.bt (iy).mLeft) ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			rotate_right (iy) ;
			fake.mTop = ix ;
		}
		if ifdo (act) {
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			rotate_right (iy) ;
			fake.mTop = curr ;
		}
	}

	void update_insert_right (CREF<INDEX> curr) {
		INDEX ix = fake.mSet.bt (curr).mUp ;
		INDEX iy = fake.mSet.bt (ix).mUp ;
		auto act = TRUE ;
		if ifdo (act) {
			INDEX jx = fake.mSet.bt (iy).mLeft ;
			if (!red_node (jx))
				discard ;
			fake.mSet.bt (jx).mRed = FALSE ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			fake.mTop = iy ;
		}
		if ifdo (act) {
			if (fake.mSet.bt (curr).mBin)
				discard ;
			rotate_right (fake.mSet.bt (iy).mRight) ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			rotate_left (iy) ;
			fake.mTop = ix ;
		}
		if ifdo (act) {
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			rotate_left (iy) ;
			fake.mTop = curr ;
		}
	}

	INDEX find (CREF<Pointer> item) const override {
		if (!fake.mSet.exist ())
			return NONE ;
		INDEX ret = fake.mRoot ;
		const auto r1x = RFat<ReflectCompr> (fake.mSet.unknown ()) ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			const auto r2x = r1x->compr (item ,fake.mSet.at (ret)) ;
			if (r2x == ZERO)
				break ;
			const auto r3x = binary_child (ret ,r2x) ;
			ret = curr_next (r3x) ;
		}
		return move (ret) ;
	}

	BOOL contain (CREF<Pointer> item) const override {
		return find (item) != NONE ;
	}

	INDEX map (CREF<Pointer> item) const override {
		INDEX ix = find (item) ;
		if (ix == NONE)
			return NONE ;
		return fake.mSet.bt (ix).mMap ;
	}

	void remove (CREF<INDEX> index) override {
		if ifdo (TRUE) {
			if (fake.mSet.bt (index).mLeft == NONE)
				discard ;
			if (fake.mSet.bt (index).mRight == NONE)
				discard ;
			eswap (index ,find_successor (index)) ;
		}
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = fake.mSet.bt (index).mLeft ;
		}
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = fake.mSet.bt (index).mRight ;
		}
		const auto r1x = parent (index) ;
		curr_next (r1x) = ix ;
		curr_prev (ix ,r1x.mBin) = r1x.mUp ;
		if ifdo (TRUE) {
			if (red_node (index))
				discard ;
			update_remove (r1x) ;
		}
		fake.mSet.free (index) ;
	}

	INDEX find_successor (CREF<INDEX> index) const {
		INDEX ret = fake.mSet.bt (index).mRight ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			const auto r1x = fake.mSet.bt (ret).mLeft ;
			if (r1x == NONE)
				break ;
			ret = r1x ;
		}
		return move (ret) ;
	}

	void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
		if (index1 == index2)
			return ;
		const auto r1x = parent (index1) ;
		const auto r2x = fake.mSet.bt (index1).mLeft ;
		const auto r3x = fake.mSet.bt (index1).mRight ;
		const auto r4x = parent (index2) ;
		const auto r5x = fake.mSet.bt (index2).mLeft ;
		const auto r6x = fake.mSet.bt (index2).mRight ;
		curr_next (r1x) = index2 ;
		curr_prev (r2x ,FALSE) = index2 ;
		curr_prev (r3x ,TRUE) = index2 ;
		curr_next (r4x) = index1 ;
		curr_prev (r5x ,FALSE) = index1 ;
		curr_prev (r6x ,TRUE) = index1 ;
		swap (fake.mSet.bt (index1).mRed ,fake.mSet.bt (index2).mRed) ;
		swap (fake.mSet.bt (index1).mUp ,fake.mSet.bt (index2).mUp) ;
		swap (fake.mSet.bt (index1).mBin ,fake.mSet.bt (index2).mBin) ;
		swap (fake.mSet.bt (index1).mLeft ,fake.mSet.bt (index2).mLeft) ;
		swap (fake.mSet.bt (index1).mRight ,fake.mSet.bt (index2).mRight) ;
	}

	void update_remove (CREF<SetChild> curr) {
		INDEX ix = curr_next (curr) ;
		INDEX iy = curr.mUp ;
		while (TRUE) {
			if (iy == NONE)
				break ;
			if (red_node (ix))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != fake.mSet.bt (iy).mLeft)
					discard ;
				update_remove_left (iy) ;
				ix = fake.mTop ;
			}
			if ifdo (act) {
				if (ix != fake.mSet.bt (iy).mRight)
					discard ;
				update_remove_right (iy) ;
				ix = fake.mTop ;
			}
			iy = fake.mSet.bt (ix).mUp ;
		}
		if ifdo (TRUE) {
			if (ix == NONE)
				discard ;
			fake.mSet.bt (ix).mRed = FALSE ;
		}
	}

	void update_remove_left (CREF<INDEX> curr) {
		INDEX ix = fake.mSet.bt (curr).mRight ;
		if ifdo (TRUE) {
			if (!red_node (ix))
				discard ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (curr).mRed = TRUE ;
			rotate_left (curr) ;
			ix = fake.mSet.bt (curr).mRight ;
		}
		INDEX jx = fake.mSet.bt (ix).mLeft ;
		INDEX jy = fake.mSet.bt (ix).mRight ;
		auto act = TRUE ;
		if ifdo (act) {
			if (red_node (jx))
				discard ;
			if (red_node (jy))
				discard ;
			fake.mSet.bt (ix).mRed = TRUE ;
			fake.mTop = curr ;
		}
		if ifdo (act) {
			if (red_node (jy))
				discard ;
			fake.mSet.bt (jx).mRed = FALSE ;
			fake.mSet.bt (ix).mRed = TRUE ;
			rotate_right (ix) ;
			ix = fake.mSet.bt (curr).mRight ;
			jx = fake.mSet.bt (ix).mLeft ;
			jy = fake.mSet.bt (ix).mRight ;
			fake.mSet.bt (ix).mRed = fake.mSet.bt (curr).mRed ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (jy).mRed = FALSE ;
			rotate_left (curr) ;
			fake.mTop = fake.mRoot ;
		}
		if ifdo (act) {
			fake.mSet.bt (ix).mRed = fake.mSet.bt (curr).mRed ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (jy).mRed = FALSE ;
			rotate_left (curr) ;
			fake.mTop = fake.mRoot ;
		}
	}

	void update_remove_right (CREF<INDEX> curr) {
		INDEX ix = fake.mSet.bt (curr).mLeft ;
		if ifdo (TRUE) {
			if (!red_node (ix))
				discard ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (curr).mRed = TRUE ;
			rotate_right (curr) ;
			ix = fake.mSet.bt (curr).mLeft ;
		}
		INDEX jx = fake.mSet.bt (ix).mRight ;
		INDEX jy = fake.mSet.bt (ix).mLeft ;
		auto act = TRUE ;
		if ifdo (act) {
			if (red_node (jx))
				discard ;
			if (red_node (jy))
				discard ;
			fake.mSet.bt (ix).mRed = TRUE ;
			fake.mTop = curr ;
		}
		if ifdo (act) {
			if (red_node (jy))
				discard ;
			fake.mSet.bt (jx).mRed = FALSE ;
			fake.mSet.bt (ix).mRed = TRUE ;
			rotate_left (ix) ;
			ix = fake.mSet.bt (curr).mLeft ;
			jx = fake.mSet.bt (ix).mRight ;
			jy = fake.mSet.bt (ix).mLeft ;
			fake.mSet.bt (ix).mRed = fake.mSet.bt (curr).mRed ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (jy).mRed = FALSE ;
			rotate_right (curr) ;
			fake.mTop = fake.mRoot ;
		}
		if ifdo (act) {
			fake.mSet.bt (ix).mRed = fake.mSet.bt (curr).mRed ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (jy).mRed = FALSE ;
			rotate_right (curr) ;
			fake.mTop = fake.mRoot ;
		}
	}

	void erase (CREF<Pointer> item) override {
		INDEX ix = find (item) ;
		if (ix == NONE)
			return ;
		remove (ix) ;
	}

	void rotate_left (CREF<INDEX> curr) {
		INDEX ix = curr ;
		INDEX iy = fake.mSet.bt (ix).mRight ;
		INDEX iz = fake.mSet.bt (iy).mLeft ;
		const auto r1x = right_child (ix) ;
		curr_next (r1x) = iz ;
		curr_prev (iz ,r1x.mBin) = r1x.mUp ;
		const auto r2x = parent (ix) ;
		curr_next (r2x) = iy ;
		curr_prev (iy ,r2x.mBin) = r2x.mUp ;
		const auto r3x = left_child (iy) ;
		curr_next (r3x) = ix ;
		curr_prev (ix ,r3x.mBin) = r3x.mUp ;
	}

	void rotate_right (CREF<INDEX> curr) {
		INDEX ix = curr ;
		INDEX iy = fake.mSet.bt (ix).mLeft ;
		INDEX iz = fake.mSet.bt (iy).mRight ;
		const auto r1x = left_child (ix) ;
		curr_next (r1x) = iz ;
		curr_prev (iz ,r1x.mBin) = r1x.mUp ;
		const auto r2x = parent (ix) ;
		curr_next (r2x) = iy ;
		curr_prev (iy ,r2x.mBin) = r2x.mUp ;
		const auto r3x = right_child (iy) ;
		curr_next (r3x) = ix ;
		curr_prev (ix ,r3x.mBin) = r3x.mUp ;
	}

	SetChild parent (CREF<INDEX> index) const {
		SetChild ret ;
		ret.mUp = fake.mSet.bt (index).mUp ;
		ret.mBin = fake.mSet.bt (index).mBin ;
		return move (ret) ;
	}

	SetChild left_child (CREF<INDEX> index) const {
		SetChild ret ;
		ret.mUp = index ;
		ret.mBin = FALSE ;
		return move (ret) ;
	}

	SetChild right_child (CREF<INDEX> index) const {
		SetChild ret ;
		ret.mUp = index ;
		ret.mBin = TRUE ;
		return move (ret) ;
	}

	SetChild binary_child (CREF<INDEX> index ,CREF<FLAG> compr_) const {
		SetChild ret ;
		ret.mUp = index ;
		ret.mBin = BOOL (compr_ > 0) ;
		return move (ret) ;
	}

	VREF<INDEX> curr_next (CREF<SetChild> index) leftvalue {
		if (index.mUp == NONE)
			return fake.mRoot ;
		if (index.mBin)
			return fake.mSet.bt (index.mUp).mRight ;
		return fake.mSet.bt (index.mUp).mLeft ;
	}

	CREF<INDEX> curr_next (CREF<SetChild> index) const leftvalue {
		if (index.mUp == NONE)
			return fake.mRoot ;
		if (index.mBin)
			return fake.mSet.bt (index.mUp).mRight ;
		return fake.mSet.bt (index.mUp).mLeft ;
	}

	VREF<INDEX> curr_prev (CREF<INDEX> index ,CREF<BOOL> binary) leftvalue {
		if ifdo (TRUE) {
			if (index != NONE)
				discard ;
			return fake.mTop ;
		}
		if ifdo (TRUE) {
			if (binary)
				discard ;
			fake.mSet.bt (index).mBin = binary ;
			return fake.mSet.bt (index).mUp ;
		}
		if ifdo (TRUE) {
			if (!binary)
				discard ;
			fake.mSet.bt (index).mBin = binary ;
			return fake.mSet.bt (index).mUp ;
		}
		assert (FALSE) ;
		return fake.mTop ;
	}
} ;

exports VFat<SetHolder> SetHolder::hold (VREF<SetLayout> that) {
	return VFat<SetHolder> (SetImplHolder () ,that) ;
}

exports CFat<SetHolder> SetHolder::hold (CREF<SetLayout> that) {
	return CFat<SetHolder> (SetImplHolder () ,that) ;
}

#ifdef __CSC_CONFIG_VAL32__
struct FUNCTION_fnvhash {
	forceinline CHAR operator() () const {
		const auto r1x = ZERO ;
		return HashProc::fnvhash32 (Pointer::from (r1x) ,0) ;
	}

	template <class ARG1>
	forceinline CHAR operator() (CREF<ARG1> src ,CREF<CHAR> val) const {
		return HashProc::fnvhash32 (Pointer::from (src) ,SIZE_OF<ARG1>::expr ,val) ;
	}
} ;
#endif

#ifdef __CSC_CONFIG_VAL64__
struct FUNCTION_fnvhash {
	forceinline QUAD operator() () const {
		const auto r1x = ZERO ;
		return HashProc::fnvhash64 (Pointer::from (r1x) ,0) ;
	}

	template <class ARG1>
	forceinline QUAD operator() (CREF<ARG1> src ,CREF<QUAD> val) const {
		return HashProc::fnvhash64 (Pointer::from (src) ,SIZE_OF<ARG1>::expr ,val) ;
	}
} ;
#endif

static constexpr auto fnvhash = FUNCTION_fnvhash () ;

class FriendHashcodeVisitorBinder final implement Fat<VisitorBinder ,HashcodeVisitor> {
public:
	static VFat<VisitorBinder> hold (VREF<HashcodeVisitor> that) {
		return VFat<VisitorBinder> (FriendHashcodeVisitorBinder () ,that) ;
	}

	void reset () override {
		fake.mCode = fnvhash () ;
		fake.mDepth = 0 ;
	}

	void enter () override {
		fake.mDepth++ ;
	}

	void leave () override {
		fake.mDepth-- ;
	}

	FLAG fetch () const override {
		const auto r1x = FLAG (fake.mCode) ;
		const auto r2x = r1x & VAL_MAX ;
		return r2x ;
	}

	void push (CREF<BYTE> a) override {
		fake.mCode = fnvhash (a ,fake.mCode) ;
	}

	void push (CREF<WORD> a) override {
		fake.mCode = fnvhash (a ,fake.mCode) ;
	}

	void push (CREF<CHAR> a) override {
		fake.mCode = fnvhash (a ,fake.mCode) ;
	}

	void push (CREF<QUAD> a) override {
		fake.mCode = fnvhash (a ,fake.mCode) ;
	}
} ;

class HashSetImplHolder final implement Fat<HashSetHolder ,HashSetLayout> {
public:
	void prepare (CREF<Unknown> holder) override {
		if (fake.mSet.exist ())
			return ;
		AllocatorHolder::hold (fake.mSet)->prepare (holder) ;
		fake.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> size_) override {
		AllocatorHolder::hold (fake.mSet)->initialize (holder ,size_) ;
		fake.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		clear () ;
	}

	void initialize (CREF<Unknown> holder ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (holder) ;
			r1x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item) ,NONE) ;
		}
	}

	void clear () override {
		fake.mSet.clear () ;
		fake.mRange = RefBuffer<INDEX> () ;
	}

	LENGTH size () const override {
		return fake.mSet.size () ;
	}

	LENGTH step () const override {
		return fake.mSet.step () ;
	}

	LENGTH length () const override {
		return fake.mSet.length () ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mSet.at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const override {
		map_ = fake.mSet.bt (index).mMap ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) override {
		fake.mSet.bt (index).mMap = map_ ;
	}

	INDEX ibegin () const override {
		return 0 ;
	}

	INDEX iend () const override {
		return fake.mSet.size () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		return find_next (index + 1) ;
	}

	INDEX find_next (CREF<INDEX> index) const {
		INDEX ret = index ;
		while (TRUE) {
			if (ret >= fake.mSet.size ())
				break ;
			if (fake.mSet.used (ret))
				break ;
			ret++ ;
		}
		return move (ret) ;
	}

	void add (RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		if ifdo (TRUE) {
			INDEX ix = find (BoxHolder::hold (item)->self) ;
			if (ix != NONE)
				discard ;
			ix = fake.mSet.alloc (move (item)) ;
			check_resize (ix) ;
			fake.mSet.bt (ix).mMap = map_ ;
			fake.mSet.bt (ix).mHash = hashcode (fake.mSet.at (ix)) ;
			update_emplace (ix) ;
		}
	}

	FLAG hashcode (CREF<Pointer> item) const {
		const auto r1x = FriendHashcodeVisitorBinder::hold (fake.mVisitor) ;
		r1x->reset () ;
		const auto r2x = RFat<ReflectVisit> (fake.mSet.unknown ()) ;
		r2x->visit (r1x.self ,item) ;
		return r1x->fetch () ;
	}

	void update_emplace (CREF<INDEX> curr) {
		assert (fake.mRange.size () > 0) ;
		INDEX ix = fake.mSet.bt (curr).mHash % fake.mRange.size () ;
		fake.mSet.bt (curr).mDown = fake.mRange[ix] ;
		fake.mRange[ix] = curr ;
	}

	INDEX find (CREF<Pointer> item) const override {
		if (!fake.mSet.exist ())
			return NONE ;
		if (fake.mRange.size () == 0)
			return NONE ;
		const auto r1x = hashcode (item) ;
		INDEX ix = r1x % fake.mRange.size () ;
		INDEX ret = fake.mRange[ix] ;
		const auto r2x = RFat<ReflectEqual> (fake.mSet.unknown ()) ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			if ifdo (TRUE) {
				if (fake.mSet.bt (ret).mHash != r1x)
					discard ;
				const auto r3x = r2x->equal (item ,fake.mSet.at (ret)) ;
				if (!r3x)
					discard ;
				return move (ret) ;
			}
			ret = fake.mSet.bt (ret).mDown ;
		}
		return move (ret) ;
	}

	BOOL contain (CREF<Pointer> item) const override {
		return find (item) != NONE ;
	}

	INDEX map (CREF<Pointer> item) const override {
		INDEX ix = find (item) ;
		if (ix == NONE)
			return NONE ;
		return fake.mSet.bt (ix).mMap ;
	}

	void remove (CREF<INDEX> index) override {
		INDEX ix = fake.mSet.bt (index).mHash % fake.mRange.size () ;
		INDEX iy = find_successor (ix ,index) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (iy != NONE)
				discard ;
			fake.mSet.bt (iy).mDown = fake.mSet.bt (index).mDown ;
		}
		if ifdo (act) {
			fake.mRange[ix] = iy ;
		}
		fake.mSet.free (index) ;
	}

	INDEX find_successor (CREF<INDEX> first ,CREF<INDEX> index) const {
		if (first == index)
			return NONE ;
		INDEX ret = first ;
		while (TRUE) {
			INDEX iy = fake.mSet.bt (ret).mDown ;
			if (iy == index)
				break ;
			ret = iy ;
		}
		return move (ret) ;
	}

	void erase (CREF<Pointer> item) override {
		INDEX ix = find (item) ;
		if (ix == NONE)
			return ;
		remove (ix) ;
	}

	void check_resize (CREF<INDEX> curr) {
		const auto r1x = fake.mSet.size () ;
		const auto r2x = fake.mRange.size () ;
		if (r2x == r1x)
			return ;
		fake.mRange = RefBuffer<INDEX> (r1x) ;
		for (auto &&i : iter (0 ,r1x))
			fake.mRange[i] = NONE ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			if (i == curr)
				continue ;
			if (!fake.mSet.used (i))
				continue ;
			update_emplace (i) ;
		}
	}
} ;

exports VFat<HashSetHolder> HashSetHolder::hold (VREF<HashSetLayout> that) {
	return VFat<HashSetHolder> (HashSetImplHolder () ,that) ;
}

exports CFat<HashSetHolder> HashSetHolder::hold (CREF<HashSetLayout> that) {
	return CFat<HashSetHolder> (HashSetImplHolder () ,that) ;
}

class BitSetImplHolder final implement Fat<BitSetHolder ,BitSetLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = (size_ + 8 - 1) / 8 ;
		fake.mSet = RefBuffer<BYTE> (r1x) ;
		fake.mWidth = size_ ;
		clear () ;
	}

	void initialize (CREF<LENGTH> size_ ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		const auto r1x = Unknown (BufferUnknownBinder<INDEX> ()) ;
		auto rax = from_initializer_list (params ,r1x) ;
		initialize (size_) ;
		const auto r2x = RFat<ReflectAssign> (r1x) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::hold (item)->remake (r1x) ;
			r2x->assign (BoxHolder::hold (item)->self ,rax[i]) ;
			add (move (item)) ;
		}
	}

	void initialize (CREF<BitSetLayout> that) override {
		const auto r1x = BitSetHolder::hold (that)->size () ;
		if (r1x == 0)
			return ;
		initialize (r1x) ;
		for (auto &&i : iter (0 ,that.mSet.size ()))
			fake.mSet[i] = that.mSet[i] ;
	}

	void clear () override {
		fill (BYTE (0X00)) ;
	}

	LENGTH size () const override {
		if (fake.mSet.size () == 0)
			return 0 ;
		return fake.mWidth ;
	}

	LENGTH length () const override {
		LENGTH ret = 0 ;
		for (auto &&i : iter (0 ,fake.mSet.size ()))
			ret += ByteProc::popcount (fake.mSet[i]) ;
		return move (ret) ;
	}

	void get (CREF<INDEX> index ,VREF<BOOL> item) const override {
		const auto r1x = ByteProc::pow_bit (index % 8) ;
		item = ByteProc::any_bit (fake.mSet[index / 8] ,r1x) ;
	}

	void set (CREF<INDEX> index ,CREF<BOOL> item) override {
		const auto r1x = ByteProc::pow_bit (index % 8) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!item)
				discard ;
			fake.mSet[index / 8] |= BYTE (r1x) ;
		}
		if ifdo (act) {
			fake.mSet[index / 8] &= ~BYTE (r1x) ;
		}
	}

	INDEX ibegin () const override {
		const auto r1x = find_next (0) ;
		return inline_min (r1x ,size ()) ;
	}

	INDEX iend () const override {
		return size () ;
	}

	INDEX inext (CREF<INDEX> index) const override {
		INDEX ix = index / 8 ;
		if ifdo (TRUE) {
			const auto r1x = index % 8 + 1 ;
			if (r1x == 8)
				discard ;
			const auto r2x = ByteProc::pow_bit (r1x) - 1 ;
			const auto r3x = fake.mSet[ix] & ~BYTE (r2x) ;
			if (r3x == BYTE (0X00))
				discard ;
			const auto r4x = ByteProc::lowcount (r3x) ;
			const auto r5x = ix * 8 + r4x ;
			return inline_min (r5x ,size ()) ;
		}
		ix++ ;
		const auto r6x = find_next (ix) ;
		return inline_min (r6x ,size ()) ;
	}

	INDEX find_next (CREF<INDEX> index) const {
		INDEX ix = index ;
		while (TRUE) {
			if (ix >= fake.mSet.size ())
				break ;
			if ifdo (TRUE) {
				if (fake.mSet[ix] == BYTE (0X00))
					discard ;
				const auto r1x = ByteProc::lowcount (fake.mSet[ix]) ;
				return ix * 8 + r1x ;
			}
			ix++ ;
		}
		return ix * 8 ;
	}

	BOOL equal (CREF<BitSetLayout> that) const override {
		const auto r1x = fake.mSet.size () ;
		const auto r2x = that.mSet.size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = inline_equal (fake.mSet[i] ,that.mSet[i]) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<BitSetLayout> that) const override {
		const auto r1x = fake.mSet.size () ;
		const auto r2x = that.mSet.size () ;
		const auto r3x = inline_compr (r1x ,r2x) ;
		if (r3x != ZERO)
			return r3x ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r4x = inline_compr (fake.mSet[i] ,that.mSet[i]) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (VREF<VisitorBinder> visitor) const override {
		visitor.enter () ;
		const auto r1x = fake.mSet.size () ;
		for (auto &&i : iter (0 ,r1x)) {
			visitor.push (fake.mSet[i]) ;
		}
		visitor.leave () ;
	}

	void add (RREF<BoxLayout> item) override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (BoxHolder::hold (item)->self) ;
		assume (inline_between (r1x ,0 ,size ())) ;
		set (r1x ,TRUE) ;
	}

	BOOL contain (CREF<Pointer> item) const override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (item) ;
		if (!inline_between (r1x ,0 ,size ()))
			return FALSE ;
		BOOL ret = FALSE ;
		get (r1x ,ret) ;
		return move (ret) ;
	}

	void erase (CREF<Pointer> item) override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (item) ;
		if (!inline_between (r1x ,0 ,size ()))
			return ;
		set (r1x ,FALSE) ;
	}

	void fill (CREF<BYTE> item) override {
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			fake.mSet[i] = BYTE (0X00) ;
		}
		check_mask (fake) ;
	}

	BitSetLayout sand (CREF<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] & that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout sor (CREF<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] | that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout sxor (CREF<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] ^ that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout ssub (CREF<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] & ~that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout snot () const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = ~fake.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	static void check_mask (VREF<BitSetLayout> that) {
		INDEX ix = that.mSet.size () - 1 ;
		if (ix <= 0)
			return ;
		const auto r1x = that.mWidth % 8 + 1 ;
		const auto r2x = ByteProc::pow_bit (r1x) - 1 ;
		that.mSet[ix] &= BYTE (r2x) ;
	}
} ;

exports VFat<BitSetHolder> BitSetHolder::hold (VREF<BitSetLayout> that) {
	return VFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}

exports CFat<BitSetHolder> BitSetHolder::hold (CREF<BitSetLayout> that) {
	return CFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}
} ;