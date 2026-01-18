#pragma once

#ifndef __CSC_ARRAY__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_array.hpp"

#include "csc_end.h"
#include <algorithm>
#include "csc_begin.h"

namespace CSC {
struct FUNCTION_from_initializer_list {
	forceinline RefBuffer<Pointer> operator() (CR<Pointer> params ,CR<Unknown> holder) const {
		RefBuffer<Pointer> ret ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = inline_list_pair (params ,r2x) ;
		const auto r4x = (r3x.m2nd - r3x.m1st) / r2x ;
		const auto r5x = Slice (r3x.m1st ,r4x ,r2x) ;
		RefBufferHolder::hold (ret)->initialize (holder ,r5x ,Box<int>::make ()) ;
		return move (ret) ;
	}
} ;

static constexpr auto from_initializer_list = FUNCTION_from_initializer_list () ;

class ArrayImplHolder final implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		RefBufferHolder::hold (self.mArray)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		RefBufferHolder::hold (self.mArray)->initialize (holder ,size_) ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		BoxHolder::hold (item)->initialize (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			r1x->assign (at (i) ,BoxHolder::hold (item)->ref) ;
		}
	}

	void initialize (CR<ArrayLayout> that) override {
		const auto r1x = ArrayHolder::hold (that)->length () ;
		if (r1x == 0)
			return ;
		initialize (that.mArray.unknown () ,r1x) ;
		splice (0 ,that) ;
	}

	Length size () const override {
		return self.mArray.size () ;
	}

	Length step () const override {
		return self.mArray.step () ;
	}

	Length length () const override {
		return self.mArray.size () ;
	}

	VR<Pointer> ref_m () leftvalue override {
		return RefBufferHolder::hold (self.mArray)->ref ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		return RefBufferHolder::hold (self.mArray)->ref ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		return self.mArray.at (index) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mArray.at (index) ;
	}

	Index ibegin () const override {
		return 0 ;
	}

	Index iend () const override {
		return length () ;
	}

	Index inext (CR<Index> index) const override {
		return index + 1 ;
	}

	Bool equal (CR<ArrayLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = ArrayHolder::hold (that)->length () ;
		if (r1x != r2x)
			return FALSE ;
		const auto r3x = RFat<ReflectEqual> (self.mArray.unknown ()) ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r4x = r3x->equal (at (i) ,ArrayHolder::hold (that)->at (i)) ;
			if (!r4x)
				return r4x ;
		}
		return TRUE ;
	}

	Flag compr (CR<ArrayLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = ArrayHolder::hold (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		const auto r4x = RFat<ReflectCompr> (self.mArray.unknown ()) ;
		for (auto &&i : range (0 ,r3x)) {
			const auto r5x = r4x->compr (at (i) ,ArrayHolder::hold (that)->at (i)) ;
			if (r5x != ZERO)
				return r5x ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		const auto r2x = RFat<ReflectVisit> (self.mArray.unknown ()) ;
		for (auto &&i : range (0 ,r1x)) {
			r2x->visit (visitor ,at (i)) ;
		}
		visitor.leave () ;
	}

	void fill (CR<Pointer> item) override {
		const auto r1x = size () ;
		const auto r2x = RFat<ReflectClone> (self.mArray.unknown ()) ;
		for (auto &&i : range (0 ,r1x)) {
			r2x->clone (at (i) ,item) ;
		}
	}

	void splice (CR<Index> index ,CR<ArrayLayout> item) override {
		const auto r1x = ArrayHolder::hold (item)->size () ;
		if (r1x == 0)
			return ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		const auto r2x = RFat<ReflectClone> (self.mArray.unknown ()) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = index + i ;
			r2x->clone (at (ix) ,ArrayHolder::hold (item)->at (i)) ;
		}
	}
} ;

exports VFat<ArrayHolder> ArrayHolder::hold (VR<ArrayLayout> that) {
	return VFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

exports CFat<ArrayHolder> ArrayHolder::hold (CR<ArrayLayout> that) {
	return CFat<ArrayHolder> (ArrayImplHolder () ,that) ;
}

class StringImplHolder final implement Fat<StringHolder ,StringLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		RefBufferHolder::hold (self.mString)->prepare (holder) ;
	}

	void initialize (CR<Slice> that ,CR<Length> step_) override {
		const auto r1x = SliceHolder::hold (that)->size () ;
		initialize (r1x ,step_) ;
		for (auto &&i : range (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CR<Length> size_ ,CR<Length> step_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = size_ + 1 ;
		auto act = TRUE ;
		if ifdo (act) {
			if (step_ != SIZE_OF<Stru8>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru8>>::expr] (Pointer::from (self.mString)) ;
			rax = RefBuffer<Stru8> (r1x) ;
		}
		if ifdo (act) {
			if (step_ != SIZE_OF<Stru16>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru16>>::expr] (Pointer::from (self.mString)) ;
			rax = RefBuffer<Stru16> (r1x) ;
		}
		if ifdo (act) {
			if (step_ != SIZE_OF<Stru32>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru32>>::expr] (Pointer::from (self.mString)) ;
			rax = RefBuffer<Stru32> (r1x) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		clear () ;
	}

	void initialize (CR<StringLayout> that) override {
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

	Flag encode () const override {
		if (!self.mString.exist ())
			return 0 ;
		return self.mEncode ;
	}

	Length size () const override {
		if (!self.mString.exist ())
			return 0 ;
		return self.mString.size () - 1 ;
	}

	Length step () const override {
		return self.mString.step () ;
	}

	Length length () const override {
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,size ())) {
			get (i ,rax) ;
			if (rax == 0)
				return i ;
		}
		return size () ;
	}

	VR<Pointer> ref_m () leftvalue override {
		return RefBufferHolder::hold (self.mString)->ref ;
	}

	CR<Pointer> ref_m () const leftvalue override {
		return RefBufferHolder::hold (self.mString)->ref ;
	}

	Ref<RefBuffer<Byte>> borrow () leftvalue override {
		assert (self.mString.exist ()) ;
		auto &&rax = keep[TYPE<RefBuffer<Byte>>::expr] (Pointer::from (self.mString)) ;
		return Ref<RefBuffer<Byte>>::reference (rax) ;
	}

	Ref<RefBuffer<Byte>> borrow () const leftvalue override {
		assert (self.mString.exist ()) ;
		auto &&rax = keep[TYPE<RefBuffer<Byte>>::expr] (Pointer::from (self.mString)) ;
		return Ref<RefBuffer<Byte>>::reference (rax) ;
	}

	void get (CR<Index> index ,VR<Stru32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != SIZE_OF<Stru8>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru8>>::expr] (Pointer::from (self.mString)) ;
			item = rax[index] ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<Stru16>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru16>>::expr] (Pointer::from (self.mString)) ;
			item = rax[index] ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<Stru32>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru32>>::expr] (Pointer::from (self.mString)) ;
			item = rax[index] ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void set (CR<Index> index ,CR<Stru32> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != SIZE_OF<Stru8>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru8>>::expr] (Pointer::from (self.mString)) ;
			rax[index] = Stru8 (item) ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<Stru16>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru16>>::expr] (Pointer::from (self.mString)) ;
			rax[index] = Stru16 (item) ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<Stru32>::expr)
				discard ;
			auto &&rax = keep[TYPE<RefBuffer<Stru32>>::expr] (Pointer::from (self.mString)) ;
			rax[index] = Stru32 (item) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		return self.mString.at (index) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mString.at (index) ;
	}

	Index ibegin () const override {
		return 0 ;
	}

	Index iend () const override {
		return length () ;
	}

	Index inext (CR<Index> index) const override {
		return index + 1 ;
	}

	Bool equal (CR<Slice> that) const override {
		const auto r1x = length () ;
		const auto r2x = that.size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,r3x)) {
			get (i ,rax) ;
			const auto r4x = inline_equal (rax ,that[i]) ;
			if (!r4x)
				return r4x ;
			if (rax == 0)
				return TRUE ;
		}
		return inline_equal (r1x ,r2x) ;
	}

	Bool equal (CR<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::hold (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = Stru32 () ;
		auto rbx = Stru32 () ;
		for (auto &&i : range (0 ,r3x)) {
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

	Flag compr (CR<Slice> that) const override {
		const auto r1x = length () ;
		const auto r2x = that.size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,r3x)) {
			get (i ,rax) ;
			const auto r4x = inline_compr (rax ,that[i]) ;
			if (r4x != ZERO)
				return r4x ;
			if (rax == 0)
				return ZERO ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	Flag compr (CR<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::hold (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = Stru32 () ;
		auto rbx = Stru32 () ;
		for (auto &&i : range (0 ,r3x)) {
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

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		const auto r1x = size () ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,r1x)) {
			get (i ,rax) ;
			if (rax == 0)
				break ;
			const auto r2x = bitwise[TYPE<Char>::expr] (rax) ;
			visitor.push (r2x) ;
		}
		visitor.leave () ;
	}

	void trunc (CR<Index> index) override {
		if (!inline_between (index ,0 ,self.mString.size ()))
			return ;
		set (index ,Stru32 (0X00)) ;
	}

	void fill (CR<Stru32> item) override {
		for (auto &&i : range (0 ,size ())) {
			set (i ,item) ;
		}
	}

	void splice (CR<Index> index ,CR<Slice> item) override {
		const auto r1x = SliceHolder::hold (item)->size () ;
		if (r1x == 0)
			return ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = index + i ;
			set (ix ,item[i]) ;
		}
	}

	void splice (CR<Index> index ,CR<StringLayout> item) override {
		const auto r1x = StringHolder::hold (item)->length () ;
		if (r1x == 0)
			return ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = index + i ;
			StringHolder::hold (item)->get (i ,rax) ;
			set (ix ,rax) ;
		}
	}

	Slice segment (CR<Index> begin_ ,CR<Index> end_) const override {
		if (begin_ >= end_)
			return Slice () ;
		return Slice (address (self.mString[begin_]) ,end_ - begin_ ,step ()) ;
	}

	Array<Slice> split (CR<Stru32> delim) const override {
		if (size () == 0)
			return Array<Slice> () ;
		const auto r1x = split_count (delim) ;
		Array<Slice> ret = Array<Slice> (r1x.m2nd / 2) ;
		Index ix = 0 ;
		for (auto &&i : range (1 ,r1x.m2nd)) {
			if (i % 2 == 0)
				continue ;	
			ret[ix] = segment (r1x.m1st[i - 1] ,r1x.m1st[i]) ;
			ix++ ;
		}
		return move (ret) ;
	}

	Tuple<RefBuffer<Index> ,Length> split_count (CR<Stru32> delim) const {
		Tuple<RefBuffer<Index> ,Length> ret ;
		ret.m1st = RefBuffer<Index> (size ()) ;
		Index ix = 0 ;
		Index iy = 0 ;
		auto rax = Stru32 () ;
		auto rbx = FALSE ;
		while (TRUE) {
			get (iy ,rax) ;
			if (rax == 0)
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rbx)
					discard ;
				if (rax == delim)
					discard ;
				ret.m1st[ix] = iy ;
				ix++ ;
				rbx = TRUE ;
			}
			if ifdo (act) {
				if (!rbx)
					discard ;
				if (rax != delim)
					discard ;
				ret.m1st[ix] = iy ;
				ix++ ;
				rbx = FALSE ;
			}
			iy++ ;
		}
		if ifdo (TRUE) {
			if (!rbx)
				discard ;
			ret.m1st[ix] = iy ;
			ix++ ;
		}
		ret.m2nd = ix ;
		return move (ret) ;
	}

	void replace (CR<Stru32> from ,CR<Stru32> into) override {
		const auto r1x = size () ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,r1x)) {
			get (i ,rax) ;
			if (rax == 0)
				break ;
			if (rax != from)
				continue ;
			set (i ,into) ;
		}
	}

	void trim (CR<Slice> list) override {
		const auto r1x = length () ;
		Index ix = 0 ;
		Index iy = r1x - 1 ;
		auto rax = Stru32 () ;
		while (TRUE) {
			if (ix >= r1x)
				break ;
			get (ix ,rax) ;
			if (!trim_contain (list ,rax))
				break ;
			ix++ ;
		}
		while (TRUE) {
			if (iy < ix)
				break ;
			get (iy ,rax) ;
			if (!trim_contain (list ,rax))
				break ;
			iy-- ;
		}
		const auto r2x = iy - ix + 1 ;
		const auto r3x = r2x * step () ;
		inline_memcpy (self.mString[0] ,self.mString[ix] ,r3x) ;
	}

	Bool trim_contain (CR<Slice> list ,CR<Stru32> item) const {
		for (auto &&i : range (0 ,list.size ())) {
			if (list[i] == item)
				return TRUE ;
		}
		return FALSE ;
	}
} ;

exports VFat<StringHolder> StringHolder::hold (VR<StringLayout> that) {
	return VFat<StringHolder> (StringImplHolder () ,that) ;
}

exports CFat<StringHolder> StringHolder::hold (CR<StringLayout> that) {
	return CFat<StringHolder> (StringImplHolder () ,that) ;
}

class DequeImplHolder final implement Fat<DequeHolder ,DequeLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		RefBufferHolder::hold (self.mDeque)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		RefBufferHolder::hold (self.mDeque)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item)) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void clear () override {
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	Length size () const override {
		return self.mDeque.size () ;
	}

	Length step () const override {
		return self.mDeque.step () ;
	}

	Length length () const override {
		if (!self.mDeque.exist ())
			return 0 ;
		return self.mWrite - self.mRead ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		assert (inline_between (index ,0 ,length ())) ;
		const auto r1x = self.mDeque.size () ;
		Index ix = (index + self.mRead) % r1x ;
		return self.mDeque.at (ix) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		assert (inline_between (index ,0 ,length ())) ;
		const auto r1x = self.mDeque.size () ;
		Index ix = (index + self.mRead) % r1x ;
		return self.mDeque.at (ix) ;
	}

	Index ibegin () const override {
		return 0 ;
	}

	Index iend () const override {
		return length () ;
	}

	Index inext (CR<Index> index) const override {
		return index + 1 ;
	}

	Bool empty () const override {
		if (!self.mDeque.exist ())
			return TRUE ;
		return self.mRead == self.mWrite ;
	}

	Bool full () const override {
		const auto r1x = size () ;
		if (r1x == 0)
			return FALSE ;
		return length () >= r1x ;
	}

	Index head () const override {
		return 0 ;
	}

	Index tail () const override {
		return length () - 1 ;
	}

	void add (RR<BoxLayout> item) override {
		check_exist () ;
		check_resize () ;
		const auto r1x = self.mDeque.size () ;
		Index ix = self.mWrite % r1x ;
		const auto r2x = RFat<ReflectAssign> (self.mDeque.unknown ()) ;
		r2x->assign (self.mDeque.at (ix) ,BoxHolder::hold (item)->ref) ;
		self.mWrite++ ;
	}

	void take () override {
		const auto r1x = self.mDeque.size () ;
		noop (r1x) ;
		assert (r1x > 0) ;
		Index ix = self.mRead ;
		const auto r2x = RFat<ReflectCreate> (self.mDeque.unknown ()) ;
		const auto r3x = RFat<ReflectDestroy> (self.mDeque.unknown ()) ;
		r3x->destroy (self.mDeque[ix] ,1) ;
		r2x->create (self.mDeque[ix] ,1) ;
		self.mRead++ ;
		check_bound () ;
	}

	void push (RR<BoxLayout> item) override {
		check_exist () ;
		check_resize () ;
		const auto r1x = self.mDeque.size () ;
		Index ix = (self.mRead - 1 + r1x) % r1x ;
		const auto r2x = RFat<ReflectAssign> (self.mDeque.unknown ()) ;
		r2x->assign (self.mDeque.at (ix) ,BoxHolder::hold (item)->ref) ;
		self.mRead-- ;
		check_bound () ;
	}

	void pop () override {
		const auto r1x = self.mDeque.size () ;
		noop (r1x) ;
		assert (r1x > 0) ;
		Index ix = self.mWrite - 1 ;
		const auto r2x = RFat<ReflectCreate> (self.mDeque.unknown ()) ;
		const auto r3x = RFat<ReflectDestroy> (self.mDeque.unknown ()) ;
		r3x->destroy (self.mDeque[ix] ,1) ;
		r2x->create (self.mDeque[ix] ,1) ;
		self.mWrite-- ;
	}

	void ring (CR<Length> count) override {
		if (size () <= 0)
			return ;
		self.mRead += count ;
		self.mWrite += count ;
		check_bound () ;
	}

	void check_exist () {
		if (self.mDeque.exist ())
			return ;
		initialize (self.mDeque.unknown () ,0) ;
	}

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::hold (self.mDeque)->resize (r2x) ;
		if ifdo (TRUE) {
			if (self.mWrite <= r1x)
				discard ;
			const auto r3x = RFat<ReflectAssign> (self.mDeque.unknown ()) ;
			for (auto &&i : range (0 ,r1x - self.mRead)) {
				Index ix = r1x - 1 - i ;
				Index iy = r2x - 1 - i ;
				r3x->assign (self.mDeque.at (iy) ,self.mDeque.at (ix)) ;
			}
			self.mRead += r2x - r1x ;
			check_bound () ;
		}
	}

	void check_bound () {
		const auto r1x = self.mDeque.size () ;
		if ifdo (TRUE) {
			if (self.mRead >= 0)
				discard ;
			self.mRead += r1x ;
			self.mWrite += r1x ;
		}
		if ifdo (TRUE) {
			if (self.mRead < r1x)
				discard ;
			self.mRead -= r1x ;
			self.mWrite -= r1x ;
		}
	}
} ;

exports VFat<DequeHolder> DequeHolder::hold (VR<DequeLayout> that) {
	return VFat<DequeHolder> (DequeImplHolder () ,that) ;
}

exports CFat<DequeHolder> DequeHolder::hold (CR<DequeLayout> that) {
	return CFat<DequeHolder> (DequeImplHolder () ,that) ;
}

class PriorityImplHolder final implement Fat<PriorityHolder ,PriorityLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		RefBufferHolder::hold (self.mPriority)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = size_ + 1 ;
		RefBufferHolder::hold (self.mPriority)->initialize (holder ,r1x) ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item)) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void clear () override {
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	Length size () const override {
		if (!self.mPriority.exist ())
			return 0 ;
		return self.mPriority.size () - 1 ;
	}

	Length step () const override {
		return self.mPriority.step () ;
	}

	Length length () const override {
		if (!self.mPriority.exist ())
			return 0 ;
		return self.mWrite ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mPriority.at (index) ;
	}

	Index ibegin () const override {
		return 0 ;
	}

	Index iend () const override {
		return length () ;
	}

	Index inext (CR<Index> index) const override {
		return index + 1 ;
	}

	Bool empty () const override {
		if (!self.mPriority.exist ())
			return TRUE ;
		return self.mRead == self.mWrite ;
	}

	Bool full () const override {
		const auto r1x = size () ;
		if (r1x == 0)
			return FALSE ;
		return length () >= r1x ;
	}

	Index head () const override {
		return 0 ;
	}

	void add (RR<BoxLayout> item) override {
		check_exist () ;
		check_resize () ;
		Index ix = self.mWrite ;
		const auto r1x = RFat<ReflectAssign> (self.mPriority.unknown ()) ;
		r1x->assign (self.mPriority.at (ix) ,BoxHolder::hold (item)->ref) ;
		self.mWrite++ ;
		update_insert (ix) ;
	}

	void take () override {
		assert (!empty ()) ;
		Index ix = self.mWrite - 1 ;
		const auto r1x = RFat<ReflectAssign> (self.mPriority.unknown ()) ;
		r1x->assign (self.mPriority.at (0) ,self.mPriority.at (ix)) ;
		self.mWrite = ix ;
		update_insert (0) ;
	}

	void update_insert (CR<Index> curr) {
		Index ix = curr ;
		Index jy = self.mWrite ;
		const auto r1x = RFat<ReflectAssign> (self.mPriority.unknown ()) ;
		const auto r2x = RFat<ReflectCompr> (self.mPriority.unknown ()) ;
		r1x->assign (self.mPriority.at (jy) ,self.mPriority.at (ix)) ;
		while (TRUE) {
			Index iy = parent (ix) ;
			if (iy < 0)
				break ;
			const auto r3x = r2x->compr (self.mPriority.at (jy) ,self.mPriority.at (iy)) ;
			if (r3x >= 0)
				break ;
			r1x->assign (self.mPriority.at (ix) ,self.mPriority.at (iy)) ;
			ix = iy ;
		}
		while (TRUE) {
			Index iy = left_child (ix) ;
			if (iy >= self.mWrite)
				break ;
			Index jx = jy ;
			if ifdo (TRUE) {
				const auto r4x = r2x->compr (self.mPriority.at (jx) ,self.mPriority.at (iy)) ;
				if (r4x <= 0)
					discard ;
				jx = iy ;
			}
			Index iz = right_child (ix) ;
			if ifdo (TRUE) {
				if (iz >= self.mWrite)
					discard ;
				const auto r5x = r2x->compr (self.mPriority.at (jx) ,self.mPriority.at (iz)) ;
				if (r5x <= 0)
					discard ;
				jx = iz ;
			}
			if (jx == ix)
				break ;
			r1x->assign (self.mPriority.at (ix) ,self.mPriority.at (jx)) ;
			ix = jx ;
		}
		r1x->assign (self.mPriority.at (ix) ,self.mPriority.at (jy)) ;
	}

	Index parent (CR<Index> curr) const {
		if (curr == 0)
			return NONE ;
		return (curr - 1) / 2 ;
	}

	Index left_child (CR<Index> curr) const {
		return curr * 2 + 1 ;
	}

	Index right_child (CR<Index> curr) const {
		return curr * 2 + 2 ;
	}

	void check_exist () {
		if (self.mPriority.exist ())
			return ;
		initialize (self.mPriority.unknown () ,0) ;
	}

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::hold (self.mPriority)->resize (r2x) ;
	}
} ;

exports VFat<PriorityHolder> PriorityHolder::hold (VR<PriorityLayout> that) {
	return VFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

exports CFat<PriorityHolder> PriorityHolder::hold (CR<PriorityLayout> that) {
	return CFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

class ListImplHolder final implement Fat<ListHolder ,ListLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		AllocatorHolder::hold (self.mList)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		AllocatorHolder::hold (self.mList)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item)) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void clear () override {
		self.mList.clear () ;
		self.mFirst = NONE ;
		self.mLast = NONE ;
	}

	Length size () const override {
		return self.mList.size () ;
	}

	Length step () const override {
		return self.mList.step () ;
	}

	Length length () const override {
		return self.mList.length () ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		return self.mList.at (index) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mList.at (index) ;
	}

	Index ibegin () const override {
		if (!self.mList.exist ())
			return NONE ;
		return self.mFirst ;
	}

	Index iend () const override {
		return NONE ;
	}

	Index inext (CR<Index> index) const override {
		return self.mList.bt (index).mRight ;
	}

	Bool empty () const override {
		if (!self.mList.exist ())
			return TRUE ;
		return self.mFirst == NONE ;
	}

	Bool full () const override {
		const auto r1x = size () ;
		if (r1x == 0)
			return FALSE ;
		return length () >= r1x ;
	}

	Index head () const override {
		return self.mFirst ;
	}

	Index tail () const override {
		return self.mLast ;
	}

	void add (RR<BoxLayout> item) override {
		check_exist () ;
		Index ix = self.mList.alloc (move (item)) ;
		self.mList.bt (ix).mLeft = self.mLast ;
		self.mList.bt (ix).mRight = NONE ;
		curr_next (self.mLast) = ix ;
		curr_prev (NONE) = ix ;
	}

	void take () override {
		assert (!empty ()) ;
		Index ix = self.mFirst ;
		Index iy = self.mList.bt (ix).mRight ;
		curr_next (NONE) = iy ;
		curr_prev (iy) = NONE ;
		self.mList.free (ix) ;
	}

	void push (RR<BoxLayout> item) override {
		check_exist () ;
		Index ix = self.mList.alloc (move (item)) ;
		self.mList.bt (ix).mLeft = NONE ;
		self.mList.bt (ix).mRight = self.mFirst ;
		curr_next (NONE) = ix ;
		curr_prev (self.mFirst) = ix ;
	}

	void pop () override {
		assert (!empty ()) ;
		Index ix = self.mLast ;
		Index iy = self.mList.bt (ix).mLeft ;
		curr_next (iy) = NONE ;
		curr_prev (NONE) = iy ;
		self.mList.free (ix) ;
	}

	Index insert (RR<BoxLayout> item) override {
		check_exist () ;
		Index ret = self.mList.alloc (move (item)) ;
		self.mList.bt (ret).mLeft = self.mLast ;
		self.mList.bt (ret).mRight = NONE ;
		curr_next (self.mLast) = ret ;
		curr_prev (NONE) = ret ;
		return move (ret) ;
	}

	Index insert (CR<Index> index ,RR<BoxLayout> item) override {
		check_exist () ;
		assert (self.mList.used (index)) ;
		Index ret = self.mList.alloc (move (item)) ;
		Index ix = self.mList.bt (index).mLeft ;
		self.mList.bt (ret).mLeft = ix ;
		self.mList.bt (ret).mRight = index ;
		curr_next (ix) = ret ;
		curr_prev (index) = ret ;
		return move (ret) ;
	}

	void remove (CR<Index> index) override {
		assert (!empty ()) ;
		Index ix = self.mList.bt (index).mLeft ;
		Index iy = self.mList.bt (index).mRight ;
		curr_next (ix) = iy ;
		curr_prev (iy) = ix ;
		self.mList.free (index) ;
	}

	void order (CR<Array<Index>> range_) override {
		assert (length () == range_.length ()) ;
		if (range_.length () == 0)
			return ;
		if ifdo (TRUE) {
			Index ix = range_[0] ;
			self.mList.bt (ix).mLeft = NONE ;
			self.mFirst = ix ;
		}
		for (auto &&i : range (1 ,range_.length ())) {
			Index ix = range_[i - 1] ;
			Index iy = range_[i] ;
			self.mList.bt (ix).mRight = iy ;
			self.mList.bt (iy).mLeft = ix ;
		}
		if ifdo (TRUE) {
			Index ix = range_[range_.length () - 1] ;
			self.mList.bt (ix).mRight = NONE ;
			self.mLast = ix ;
		}
	}

	VR<Index> curr_next (CR<Index> index) leftvalue {
		if (index == NONE)
			return self.mFirst ;
		return self.mList.bt (index).mRight ;
	}

	VR<Index> curr_prev (CR<Index> index) leftvalue {
		if (index == NONE)
			return self.mLast ;
		return self.mList.bt (index).mLeft ;
	}

	void check_exist () {
		if (self.mList.exist ())
			return ;
		initialize (self.mList.unknown () ,0) ;
	}
} ;

exports VFat<ListHolder> ListHolder::hold (VR<ListLayout> that) {
	return VFat<ListHolder> (ListImplHolder () ,that) ;
}

exports CFat<ListHolder> ListHolder::hold (CR<ListLayout> that) {
	return CFat<ListHolder> (ListImplHolder () ,that) ;
}

class ArrayListImplHolder final implement Fat<ArrayListHolder ,ArrayListLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		AllocatorHolder::hold (self.mList)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		AllocatorHolder::hold (self.mList)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item)) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void clear () override {
		self.mList.clear () ;
		self.mRange = RefBuffer<Index> () ;
		self.mTop = 0 ;
	}

	Length size () const override {
		return self.mList.size () ;
	}

	Length step () const override {
		return self.mList.step () ;
	}

	Length length () const override {
		return self.mList.length () ;
	}

	VR<Pointer> at (CR<Index> index) leftvalue override {
		return self.mList.at (self.mRange[index]) ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mList.at (self.mRange[index]) ;
	}

	Index ibegin () const override {
		return find_next (0) ;
	}

	Index iend () const override {
		return self.mRange.size () ;
	}

	Index inext (CR<Index> index) const override {
		return find_next (index + 1) ;
	}

	Index find_next (CR<Index> index) const {
		Index ret = index ;
		while (TRUE) {
			if (ret >= self.mRange.size ())
				break ;
			if (self.mRange[ret] != NONE)
				break ;
			ret++ ;
		}
		return move (ret) ;
	}

	void add (RR<BoxLayout> item) override {
		check_exist () ;
		Index ix = self.mList.alloc (move (item)) ;
		check_resize () ;
		Index iy = find_free () ;
		assert (self.mRange[iy] == NONE) ;
		self.mRange[iy] = ix ;
		self.mRemap = FALSE ;
	}

	Index insert (RR<BoxLayout> item) override {
		check_exist () ;
		Index ix = self.mList.alloc (move (item)) ;
		check_resize () ;
		Index ret = find_free () ;
		assert (self.mRange[ret] == NONE) ;
		self.mRange[ret] = ix ;
		self.mRemap = FALSE ;
		return move (ret) ;
	}

	Index insert (CR<Index> index ,RR<BoxLayout> item) override {
		check_exist () ;
		assert (inline_between (index ,0 ,self.mRange.size ())) ;
		assert (!self.mList.used (self.mRange[index])) ;
		Index ix = self.mList.alloc (move (item)) ;
		check_resize () ;
		Index ret = index ;
		assert (self.mRange[ret] == NONE) ;
		self.mRange[ret] = ix ;
		self.mRemap = FALSE ;
		return move (ret) ;
	}

	void remove (CR<Index> index) override {
		Index ix = self.mRange[index] ;
		self.mRange[index] = NONE ;
		self.mList.free (ix) ;
		self.mRemap = FALSE ;
	}

	void order (CR<Array<Index>> range_) override {
		assert (length () == range_.length ()) ;
		if (range_.length () == 0)
			return ;
		auto rax = RefBuffer<Index> (self.mRange.size ()) ;
		for (auto &&i : range_.iter ()) {
			Index ix = range_[i] ;
			rax[i] = self.mRange[ix] ;
		}
		for (auto &&i : range (range_.length () ,self.mRange.size ()))
			rax[i] = NONE ;
		self.mRange = move (rax) ;
	}

	void remap () override {
		Index ix = 0 ;
		Index iy = 0 ;
		while (TRUE) {
			iy = find_next (iy) ;
			if (iy >= self.mRange.size ())
				break ;
			self.mRange[ix] = self.mRange[iy] ;
			ix++ ;
			iy++ ;
		}
		self.mTop = ix ;
		while (TRUE) {
			if (ix >= self.mRange.size ())
				break ;
			self.mRange[ix] = NONE ;
			ix++ ;
		}
		self.mRemap = TRUE ;
	}

	Index find_free () {
		const auto r1x = self.mRange.size () ;
		assert (r1x > 0) ;
		const auto r2x = self.mTop % r1x ;
		Index ret = r2x ;
		while (TRUE) {
			if (self.mRange[ret] == NONE)
				break ;
			ret++ ;
			replace (ret ,r1x ,0) ;
			if (ret == r2x)
				break ;
		}
		assert (self.mRange[ret] == NONE) ;
		self.mTop = ret ;
		return move (ret) ;
	}

	void check_exist () {
		if (self.mList.exist ())
			return ;
		initialize (self.mList.unknown () ,0) ;
	}

	void check_resize () {
		const auto r1x = self.mList.size () ;
		const auto r2x = self.mRange.size () ;
		if (r2x == r1x)
			return ;
		assert (r2x <= r1x) ;
		self.mRange.resize (r1x) ;
		for (auto &&i : range (r2x ,r1x)) {
			self.mRange[i] = NONE ;
		}
	}
} ;

exports VFat<ArrayListHolder> ArrayListHolder::hold (VR<ArrayListLayout> that) {
	return VFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

exports CFat<ArrayListHolder> ArrayListHolder::hold (CR<ArrayListLayout> that) {
	return CFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

class SortedMapImplHolder final implement Fat<SortedMapHolder ,SortedMapLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		noop () ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		self.mThis = Ref<SortedMapTree>::make () ;
		AllocatorHolder::hold (self.mThis->mList)->initialize (holder ,size_) ;
		self.mThis->mCheck = 0 ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item) ,NONE) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	SortedMapLayout share () const override {
		assert (self.mRoot == NONE) ;
		SortedMapLayout ret ;
		ret.mThis = self.mThis.share () ;
		ret.mRoot = NONE ;
		ret.mRange = RefBuffer<Index> () ;
		ret.mWrite = 0 ;
		ret.mRemap = FALSE ;
		return move (ret) ;
	}

	void clear () override {
		self.mRoot = NONE ;
		self.mRange = RefBuffer<Index> () ;
		self.mWrite = 0 ;
		self.mRemap = FALSE ;
	}

	Length size () const override {
		return self.mRange.size () ;
	}

	Length step () const override {
		return self.mRange.step () ;
	}

	Length length () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mWrite ;
	}

	VR<Index> at (CR<Index> index) leftvalue override {
		return self.mThis->mList.bt (self.mRange[index]).mMap ;
	}

	CR<Index> at (CR<Index> index) const leftvalue override {
		return self.mThis->mList.bt (self.mRange[index]).mMap ;
	}

	Index ibegin () const override {
		assert (self.mRemap) ;
		return 0 ;
	}

	Index iend () const override {
		assert (self.mRemap) ;
		return length () ;
	}

	Index inext (CR<Index> index) const override {
		return index + 1 ;
	}

	void add (RR<BoxLayout> item ,CR<Index> map_) override {
		check_exist () ;
		Index ix = self.mThis->mList.alloc (move (item)) ;
		self.mThis->mCheck++ ;
		self.mThis->mList.bt (ix).mMap = map_ ;
		self.mThis->mList.bt (ix).mDown = self.mRoot ;
		self.mRoot = ix ;
		self.mWrite++ ;
		self.mRemap = FALSE ;
	}

	Index find (CR<Pointer> item) const override {
		if (!self.mThis.exist ())
			return NONE ;
		assert (self.mRemap) ;
		Index ix = 0 ;
		Index iy = length () - 1 ;
		Index iz = 0 ;
		const auto r1x = RFat<ReflectCompr> (self.mThis->mList.unknown ()) ;
		while (TRUE) {
			if (ix > iy)
				break ;
			iz = ix + (iy - ix) / 2 ;
			const auto r2x = r1x->compr (item ,self.mThis->mList.at (self.mRange[iz])) ;
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

	Bool contain (CR<Pointer> item) const override {
		return find (item) != NONE ;
	}

	Index map (CR<Pointer> item) const override {
		Index ix = find (item) ;
		if (ix == NONE)
			return NONE ;
		return at (ix) ;
	}

	void remap () override {
		if (!self.mThis.exist ())
			return ;
		if (self.mRemap)
			return ;
		if ifdo (TRUE) {
			if (self.mWrite == 0)
				discard ;
			auto &&rax = self.mThis.ref ;
			const auto r1x = RFat<ReflectCompr> (rax.mList.unknown ()) ;
			const auto r2x = RFat<ReflectEqual> (rax.mList.unknown ()) ;
			if ifdo (TRUE) {
				self.mRange = RefBuffer<Index> (self.mWrite) ;
				Index ix = self.mRoot ;
				for (auto &&i : range (0 ,self.mWrite)) {
					assert (ix != NONE) ;
					self.mRange[i] = ix ;
					ix = rax.mList.bt (ix).mDown ;
				}
				assert (ix == NONE) ;
			}
			if ifdo (TRUE) {
				const auto r3x = (&self.mRange[0]) ;
				const auto r4x = r3x + self.mRange.size () ;
				std::sort (r3x ,r4x ,[&] (CR<Index> a ,CR<Index> b) {
					return r1x->compr (rax.mList[a] ,rax.mList[b]) < ZERO ;
				}) ;
			}
			if ifdo (TRUE) {
				Index ix = 0 ;
				for (auto &&i : range (1 ,self.mWrite)) {
					const auto r5x = r2x->equal (rax.mList[self.mRange[ix]] ,rax.mList[self.mRange[i]]) ;
					if (r5x)
						continue ;
					ix++ ;
					self.mRange[ix] = self.mRange[i] ;
				}
				ix++ ;
				//@warn: length would be decresed due to remove the same item
				self.mWrite = ix ;
				for (auto &&i : range (self.mWrite ,self.mRange.size ()))
					self.mRange[i] = NONE ;
			}
			if ifdo (TRUE) {
				self.mRoot = self.mRange[0] ;
				for (auto &&i : range (0 ,self.mWrite - 1)) {
					rax.mList.bt (self.mRange[i]).mDown = self.mRange[i + 1] ;
				}
				Index ix = self.mRange[self.mWrite - 1] ;
				rax.mList.bt (ix).mDown = NONE ;
			}
		}
		self.mRemap = TRUE ;
	}

	void check_exist () {
		if (self.mThis.exist ())
			return ;
		assume (FALSE) ;
	}
} ;

exports VFat<SortedMapHolder> SortedMapHolder::hold (VR<SortedMapLayout> that) {
	return VFat<SortedMapHolder> (SortedMapImplHolder () ,that) ;
}

exports CFat<SortedMapHolder> SortedMapHolder::hold (CR<SortedMapLayout> that) {
	return CFat<SortedMapHolder> (SortedMapImplHolder () ,that) ;
}

struct SetChild {
	Index mUp ;
	Bool mBin ;
} ;

class SetImplHolder final implement Fat<SetHolder ,SetLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		AllocatorHolder::hold (self.mSet)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		AllocatorHolder::hold (self.mSet)->initialize (holder ,size_) ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item) ,NONE) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void clear () override {
		self.mSet.clear () ;
		self.mRoot = NONE ;
		self.mTop = 0 ;
	}

	Length size () const override {
		return self.mSet.size () ;
	}

	Length step () const override {
		return self.mSet.step () ;
	}

	Length length () const override {
		return self.mSet.length () ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mSet.at (index) ;
	}

	void get (CR<Index> index ,VR<Index> map_) const override {
		map_ = self.mSet.bt (index).mMap ;
	}

	void set (CR<Index> index ,CR<Index> map_) override {
		self.mSet.bt (index).mMap = map_ ;
	}

	Index ibegin () const override {
		return find_next (0) ;
	}

	Index iend () const override {
		return self.mSet.size () ;
	}

	Index inext (CR<Index> index) const override {
		return find_next (index + 1) ;
	}

	Index find_next (CR<Index> index) const {
		Index ret = index ;
		while (TRUE) {
			if (ret >= self.mSet.size ())
				break ;
			if (self.mSet.used (ret))
				break ;
			ret++ ;
		}
		return move (ret) ;
	}

	void add (RR<BoxLayout> item ,CR<Index> map_) override {
		check_exist () ;
		if ifdo (TRUE) {
			Index ix = find (BoxHolder::hold (item)->ref) ;
			if (ix != NONE)
				discard ;
			ix = self.mSet.alloc (move (item)) ;
			self.mSet.bt (ix).mMap = map_ ;
			self.mSet.bt (ix).mRed = TRUE ;
			self.mSet.bt (ix).mLeft = NONE ;
			self.mSet.bt (ix).mRight = NONE ;
			update_emplace (ix) ;
			update_insert (ix) ;
		}
	}

	Bool red_node (CR<Index> curr) const {
		if (curr == NONE)
			return FALSE ;
		return self.mSet.bt (curr).mRed ;
	}

	void update_emplace (CR<Index> curr) {
		Index ix = self.mRoot ;
		auto rax = binary_child (NONE ,ZERO) ;
		const auto r1x = RFat<ReflectCompr> (self.mSet.unknown ()) ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			const auto r2x = r1x->compr (self.mSet.at (curr) ,self.mSet.at (ix)) ;
			assert (r2x != ZERO) ;
			rax = binary_child (ix ,r2x) ;
			ix = curr_next (rax) ;
		}
		curr_next (rax) = curr ;
		curr_prev (curr ,rax.mBin) = rax.mUp ;
	}

	void update_insert (CR<Index> curr) {
		Index ix = curr ;
		Index iy = self.mSet.bt (ix).mUp ;
		while (TRUE) {
			if (!red_node (iy))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (self.mSet.bt (iy).mBin)
					discard ;
				update_insert_left (ix) ;
				ix = self.mTop ;
			}
			if ifdo (act) {
				update_insert_right (ix) ;
				ix = self.mTop ;
			}
			iy = self.mSet.bt (ix).mUp ;
		}
		self.mSet.bt (self.mRoot).mRed = FALSE ;
	}

	void update_insert_left (CR<Index> curr) {
		Index ix = self.mSet.bt (curr).mUp ;
		Index iy = self.mSet.bt (ix).mUp ;
		auto act = TRUE ;
		if ifdo (act) {
			Index jx = self.mSet.bt (iy).mRight ;
			if (!red_node (jx))
				discard ;
			self.mSet.bt (jx).mRed = FALSE ;
			self.mSet.bt (ix).mRed = FALSE ;
			self.mSet.bt (iy).mRed = TRUE ;
			self.mTop = iy ;
		}
		if ifdo (act) {
			if (!self.mSet.bt (curr).mBin)
				discard ;
			rotate_left (self.mSet.bt (iy).mLeft) ;
			self.mSet.bt (curr).mRed = FALSE ;
			self.mSet.bt (iy).mRed = TRUE ;
			rotate_right (iy) ;
			self.mTop = ix ;
		}
		if ifdo (act) {
			self.mSet.bt (ix).mRed = FALSE ;
			self.mSet.bt (iy).mRed = TRUE ;
			rotate_right (iy) ;
			self.mTop = curr ;
		}
	}

	void update_insert_right (CR<Index> curr) {
		Index ix = self.mSet.bt (curr).mUp ;
		Index iy = self.mSet.bt (ix).mUp ;
		auto act = TRUE ;
		if ifdo (act) {
			Index jx = self.mSet.bt (iy).mLeft ;
			if (!red_node (jx))
				discard ;
			self.mSet.bt (jx).mRed = FALSE ;
			self.mSet.bt (ix).mRed = FALSE ;
			self.mSet.bt (iy).mRed = TRUE ;
			self.mTop = iy ;
		}
		if ifdo (act) {
			if (self.mSet.bt (curr).mBin)
				discard ;
			rotate_right (self.mSet.bt (iy).mRight) ;
			self.mSet.bt (curr).mRed = FALSE ;
			self.mSet.bt (iy).mRed = TRUE ;
			rotate_left (iy) ;
			self.mTop = ix ;
		}
		if ifdo (act) {
			self.mSet.bt (ix).mRed = FALSE ;
			self.mSet.bt (iy).mRed = TRUE ;
			rotate_left (iy) ;
			self.mTop = curr ;
		}
	}

	Index find (CR<Pointer> item) const override {
		if (!self.mSet.exist ())
			return NONE ;
		Index ret = self.mRoot ;
		const auto r1x = RFat<ReflectCompr> (self.mSet.unknown ()) ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			const auto r2x = r1x->compr (item ,self.mSet.at (ret)) ;
			if (r2x == ZERO)
				break ;
			const auto r3x = binary_child (ret ,r2x) ;
			ret = curr_next (r3x) ;
		}
		return move (ret) ;
	}

	Bool contain (CR<Pointer> item) const override {
		return find (item) != NONE ;
	}

	Index map (CR<Pointer> item) const override {
		Index ix = find (item) ;
		if (ix == NONE)
			return NONE ;
		return self.mSet.bt (ix).mMap ;
	}

	void remove (CR<Index> index) override {
		if ifdo (TRUE) {
			if (self.mSet.bt (index).mLeft == NONE)
				discard ;
			if (self.mSet.bt (index).mRight == NONE)
				discard ;
			eswap (index ,find_successor (index)) ;
		}
		Index ix = NONE ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mSet.bt (index).mLeft ;
		}
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mSet.bt (index).mRight ;
		}
		const auto r1x = parent (index) ;
		curr_next (r1x) = ix ;
		curr_prev (ix ,r1x.mBin) = r1x.mUp ;
		if ifdo (TRUE) {
			if (red_node (index))
				discard ;
			update_remove (r1x) ;
		}
		self.mSet.free (index) ;
	}

	Index find_successor (CR<Index> index) const {
		Index ret = self.mSet.bt (index).mRight ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			const auto r1x = self.mSet.bt (ret).mLeft ;
			if (r1x == NONE)
				break ;
			ret = r1x ;
		}
		return move (ret) ;
	}

	void eswap (CR<Index> index1 ,CR<Index> index2) {
		if (index1 == index2)
			return ;
		const auto r1x = parent (index1) ;
		const auto r2x = self.mSet.bt (index1).mLeft ;
		const auto r3x = self.mSet.bt (index1).mRight ;
		const auto r4x = parent (index2) ;
		const auto r5x = self.mSet.bt (index2).mLeft ;
		const auto r6x = self.mSet.bt (index2).mRight ;
		curr_next (r1x) = index2 ;
		curr_prev (r2x ,FALSE) = index2 ;
		curr_prev (r3x ,TRUE) = index2 ;
		curr_next (r4x) = index1 ;
		curr_prev (r5x ,FALSE) = index1 ;
		curr_prev (r6x ,TRUE) = index1 ;
		swap (self.mSet.bt (index1).mRed ,self.mSet.bt (index2).mRed) ;
		swap (self.mSet.bt (index1).mUp ,self.mSet.bt (index2).mUp) ;
		swap (self.mSet.bt (index1).mBin ,self.mSet.bt (index2).mBin) ;
		swap (self.mSet.bt (index1).mLeft ,self.mSet.bt (index2).mLeft) ;
		swap (self.mSet.bt (index1).mRight ,self.mSet.bt (index2).mRight) ;
	}

	void update_remove (CR<SetChild> curr) {
		Index ix = curr_next (curr) ;
		Index iy = curr.mUp ;
		while (TRUE) {
			if (iy == NONE)
				break ;
			if (red_node (ix))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != self.mSet.bt (iy).mLeft)
					discard ;
				update_remove_left (iy) ;
				ix = self.mTop ;
			}
			if ifdo (act) {
				if (ix != self.mSet.bt (iy).mRight)
					discard ;
				update_remove_right (iy) ;
				ix = self.mTop ;
			}
			iy = self.mSet.bt (ix).mUp ;
		}
		if ifdo (TRUE) {
			if (ix == NONE)
				discard ;
			self.mSet.bt (ix).mRed = FALSE ;
		}
	}

	void update_remove_left (CR<Index> curr) {
		Index ix = self.mSet.bt (curr).mRight ;
		if ifdo (TRUE) {
			if (!red_node (ix))
				discard ;
			self.mSet.bt (ix).mRed = FALSE ;
			self.mSet.bt (curr).mRed = TRUE ;
			rotate_left (curr) ;
			ix = self.mSet.bt (curr).mRight ;
		}
		Index jx = self.mSet.bt (ix).mLeft ;
		Index jy = self.mSet.bt (ix).mRight ;
		auto act = TRUE ;
		if ifdo (act) {
			if (red_node (jx))
				discard ;
			if (red_node (jy))
				discard ;
			self.mSet.bt (ix).mRed = TRUE ;
			self.mTop = curr ;
		}
		if ifdo (act) {
			if (red_node (jy))
				discard ;
			self.mSet.bt (jx).mRed = FALSE ;
			self.mSet.bt (ix).mRed = TRUE ;
			rotate_right (ix) ;
			ix = self.mSet.bt (curr).mRight ;
			jx = self.mSet.bt (ix).mLeft ;
			jy = self.mSet.bt (ix).mRight ;
			self.mSet.bt (ix).mRed = self.mSet.bt (curr).mRed ;
			self.mSet.bt (curr).mRed = FALSE ;
			self.mSet.bt (jy).mRed = FALSE ;
			rotate_left (curr) ;
			self.mTop = self.mRoot ;
		}
		if ifdo (act) {
			self.mSet.bt (ix).mRed = self.mSet.bt (curr).mRed ;
			self.mSet.bt (curr).mRed = FALSE ;
			self.mSet.bt (jy).mRed = FALSE ;
			rotate_left (curr) ;
			self.mTop = self.mRoot ;
		}
	}

	void update_remove_right (CR<Index> curr) {
		Index ix = self.mSet.bt (curr).mLeft ;
		if ifdo (TRUE) {
			if (!red_node (ix))
				discard ;
			self.mSet.bt (ix).mRed = FALSE ;
			self.mSet.bt (curr).mRed = TRUE ;
			rotate_right (curr) ;
			ix = self.mSet.bt (curr).mLeft ;
		}
		Index jx = self.mSet.bt (ix).mRight ;
		Index jy = self.mSet.bt (ix).mLeft ;
		auto act = TRUE ;
		if ifdo (act) {
			if (red_node (jx))
				discard ;
			if (red_node (jy))
				discard ;
			self.mSet.bt (ix).mRed = TRUE ;
			self.mTop = curr ;
		}
		if ifdo (act) {
			if (red_node (jy))
				discard ;
			self.mSet.bt (jx).mRed = FALSE ;
			self.mSet.bt (ix).mRed = TRUE ;
			rotate_left (ix) ;
			ix = self.mSet.bt (curr).mLeft ;
			jx = self.mSet.bt (ix).mRight ;
			jy = self.mSet.bt (ix).mLeft ;
			self.mSet.bt (ix).mRed = self.mSet.bt (curr).mRed ;
			self.mSet.bt (curr).mRed = FALSE ;
			self.mSet.bt (jy).mRed = FALSE ;
			rotate_right (curr) ;
			self.mTop = self.mRoot ;
		}
		if ifdo (act) {
			self.mSet.bt (ix).mRed = self.mSet.bt (curr).mRed ;
			self.mSet.bt (curr).mRed = FALSE ;
			self.mSet.bt (jy).mRed = FALSE ;
			rotate_right (curr) ;
			self.mTop = self.mRoot ;
		}
	}

	void erase (CR<Pointer> item) override {
		Index ix = find (item) ;
		if (ix == NONE)
			return ;
		remove (ix) ;
	}

	void rotate_left (CR<Index> curr) {
		Index ix = curr ;
		Index iy = self.mSet.bt (ix).mRight ;
		Index iz = self.mSet.bt (iy).mLeft ;
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

	void rotate_right (CR<Index> curr) {
		Index ix = curr ;
		Index iy = self.mSet.bt (ix).mLeft ;
		Index iz = self.mSet.bt (iy).mRight ;
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

	SetChild parent (CR<Index> index) const {
		SetChild ret ;
		ret.mUp = self.mSet.bt (index).mUp ;
		ret.mBin = self.mSet.bt (index).mBin ;
		return move (ret) ;
	}

	SetChild left_child (CR<Index> index) const {
		SetChild ret ;
		ret.mUp = index ;
		ret.mBin = FALSE ;
		return move (ret) ;
	}

	SetChild right_child (CR<Index> index) const {
		SetChild ret ;
		ret.mUp = index ;
		ret.mBin = TRUE ;
		return move (ret) ;
	}

	SetChild binary_child (CR<Index> index ,CR<Flag> compr_) const {
		SetChild ret ;
		ret.mUp = index ;
		ret.mBin = Bool (compr_ > 0) ;
		return move (ret) ;
	}

	VR<Index> curr_next (CR<SetChild> index) leftvalue {
		if (index.mUp == NONE)
			return self.mRoot ;
		if (index.mBin)
			return self.mSet.bt (index.mUp).mRight ;
		return self.mSet.bt (index.mUp).mLeft ;
	}

	CR<Index> curr_next (CR<SetChild> index) const leftvalue {
		if (index.mUp == NONE)
			return self.mRoot ;
		if (index.mBin)
			return self.mSet.bt (index.mUp).mRight ;
		return self.mSet.bt (index.mUp).mLeft ;
	}

	VR<Index> curr_prev (CR<Index> index ,CR<Bool> binary) leftvalue {
		if ifdo (TRUE) {
			if (index != NONE)
				discard ;
			return self.mTop ;
		}
		if ifdo (TRUE) {
			if (binary)
				discard ;
			self.mSet.bt (index).mBin = binary ;
			return self.mSet.bt (index).mUp ;
		}
		if ifdo (TRUE) {
			if (!binary)
				discard ;
			self.mSet.bt (index).mBin = binary ;
			return self.mSet.bt (index).mUp ;
		}
		assert (FALSE) ;
		return self.mTop ;
	}

	void check_exist () {
		if (self.mSet.exist ())
			return ;
		initialize (self.mSet.unknown () ,0) ;
	}
} ;

exports VFat<SetHolder> SetHolder::hold (VR<SetLayout> that) {
	return VFat<SetHolder> (SetImplHolder () ,that) ;
}

exports CFat<SetHolder> SetHolder::hold (CR<SetLayout> that) {
	return CFat<SetHolder> (SetImplHolder () ,that) ;
}

#ifdef __CSC_CONFIG_VAL32__
struct FUNCTION_fnvhash {
	forceinline Char operator() () const {
		const auto r1x = ZERO ;
		return HashProc::fnvhash32 (Pointer::from (r1x) ,0) ;
	}

	template <class ARG1>
	forceinline Char operator() (CR<ARG1> src ,CR<Char> val) const {
		return HashProc::fnvhash32 (Pointer::from (src) ,SIZE_OF<ARG1>::expr ,val) ;
	}
} ;
#endif

#ifdef __CSC_CONFIG_VAL64__
struct FUNCTION_fnvhash {
	forceinline Quad operator() () const {
		const auto r1x = ZERO ;
		return HashProc::fnvhash64 (Pointer::from (r1x) ,0) ;
	}

	template <class ARG1>
	forceinline Quad operator() (CR<ARG1> src ,CR<Quad> val) const {
		return HashProc::fnvhash64 (Pointer::from (src) ,SIZE_OF<ARG1>::expr ,val) ;
	}
} ;
#endif

static constexpr auto fnvhash = FUNCTION_fnvhash () ;

class HashcodeVisitorImplHolder final implement Fat<HashcodeVisitorHolder ,HashcodeVisitorLayout> {
public:
	void reset () override {
		self.mCode = fnvhash () ;
		self.mDepth = 0 ;
	}

	void enter () override {
		self.mDepth++ ;
	}

	void leave () override {
		self.mDepth-- ;
	}

	Flag fetch () const override {
		const auto r1x = Flag (self.mCode) ;
		const auto r2x = r1x & VAL_MAX ;
		return r2x ;
	}

	void push (CR<Byte> a) override {
		self.mCode = fnvhash (a ,self.mCode) ;
	}

	void push (CR<Word> a) override {
		self.mCode = fnvhash (a ,self.mCode) ;
	}

	void push (CR<Char> a) override {
		self.mCode = fnvhash (a ,self.mCode) ;
	}

	void push (CR<Quad> a) override {
		self.mCode = fnvhash (a ,self.mCode) ;
	}
} ;

exports VFat<HashcodeVisitorHolder> HashcodeVisitorHolder::hold (VR<HashcodeVisitorLayout> that) {
	return VFat<HashcodeVisitorHolder> (HashcodeVisitorImplHolder () ,that) ;
}

exports CFat<HashcodeVisitorHolder> HashcodeVisitorHolder::hold (CR<HashcodeVisitorLayout> that) {
	return CFat<HashcodeVisitorHolder> (HashcodeVisitorImplHolder () ,that) ;
}

class HashSetImplHolder final implement Fat<HashSetHolder ,HashSetLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		AllocatorHolder::hold (self.mSet)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> size_) override {
		AllocatorHolder::hold (self.mSet)->initialize (holder ,size_) ;
		self.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		clear () ;
	}

	void initialize (CR<Unknown> holder ,CR<Pointer> params ,VR<BoxLayout> item) override {
		auto rax = from_initializer_list (params ,holder) ;
		initialize (holder ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (holder) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (holder) ;
			r1x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item) ,NONE) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void clear () override {
		self.mSet.clear () ;
		self.mRange = RefBuffer<Index> () ;
	}

	Length size () const override {
		return self.mSet.size () ;
	}

	Length step () const override {
		return self.mSet.step () ;
	}

	Length length () const override {
		return self.mSet.length () ;
	}

	CR<Pointer> at (CR<Index> index) const leftvalue override {
		return self.mSet.at (index) ;
	}

	void get (CR<Index> index ,VR<Index> map_) const override {
		map_ = self.mSet.bt (index).mMap ;
	}

	void set (CR<Index> index ,CR<Index> map_) override {
		self.mSet.bt (index).mMap = map_ ;
	}

	Index ibegin () const override {
		return 0 ;
	}

	Index iend () const override {
		return self.mSet.size () ;
	}

	Index inext (CR<Index> index) const override {
		return find_next (index + 1) ;
	}

	Index find_next (CR<Index> index) const {
		Index ret = index ;
		while (TRUE) {
			if (ret >= self.mSet.size ())
				break ;
			if (self.mSet.used (ret))
				break ;
			ret++ ;
		}
		return move (ret) ;
	}

	void add (RR<BoxLayout> item ,CR<Index> map_) override {
		check_exist () ;
		if ifdo (TRUE) {
			Index ix = find (BoxHolder::hold (item)->ref) ;
			if (ix != NONE)
				discard ;
			ix = self.mSet.alloc (move (item)) ;
			check_resize (ix) ;
			self.mSet.bt (ix).mMap = map_ ;
			self.mSet.bt (ix).mHash = hashcode (self.mSet.at (ix)) ;
			update_emplace (ix) ;
		}
	}

	Flag hashcode (CR<Pointer> item) const {
		const auto r1x = Visitor (self.mVisitor.ref) ;
		r1x.reset () ;
		const auto r2x = RFat<ReflectVisit> (self.mSet.unknown ()) ;
		r2x->visit (r1x ,item) ;
		return r1x.fetch () ;
	}

	void update_emplace (CR<Index> curr) {
		assert (self.mRange.size () > 0) ;
		Index ix = self.mSet.bt (curr).mHash % self.mRange.size () ;
		self.mSet.bt (curr).mDown = self.mRange[ix] ;
		self.mRange[ix] = curr ;
	}

	Index find (CR<Pointer> item) const override {
		if (!self.mSet.exist ())
			return NONE ;
		if (self.mRange.size () == 0)
			return NONE ;
		const auto r1x = hashcode (item) ;
		Index ix = r1x % self.mRange.size () ;
		Index ret = self.mRange[ix] ;
		const auto r2x = RFat<ReflectEqual> (self.mSet.unknown ()) ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			if ifdo (TRUE) {
				if (self.mSet.bt (ret).mHash != r1x)
					discard ;
				const auto r3x = r2x->equal (item ,self.mSet.at (ret)) ;
				if (!r3x)
					discard ;
				return move (ret) ;
			}
			ret = self.mSet.bt (ret).mDown ;
		}
		return move (ret) ;
	}

	Bool contain (CR<Pointer> item) const override {
		return find (item) != NONE ;
	}

	Index map (CR<Pointer> item) const override {
		Index ix = find (item) ;
		if (ix == NONE)
			return NONE ;
		return self.mSet.bt (ix).mMap ;
	}

	void remove (CR<Index> index) override {
		Index ix = self.mSet.bt (index).mHash % self.mRange.size () ;
		Index iy = find_successor (ix ,index) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (iy != NONE)
				discard ;
			self.mSet.bt (iy).mDown = self.mSet.bt (index).mDown ;
		}
		if ifdo (act) {
			self.mRange[ix] = iy ;
		}
		self.mSet.free (index) ;
	}

	Index find_successor (CR<Index> first ,CR<Index> index) const {
		if (first == index)
			return NONE ;
		Index ret = first ;
		while (TRUE) {
			Index iy = self.mSet.bt (ret).mDown ;
			if (iy == index)
				break ;
			ret = iy ;
		}
		return move (ret) ;
	}

	void erase (CR<Pointer> item) override {
		Index ix = find (item) ;
		if (ix == NONE)
			return ;
		remove (ix) ;
	}

	void check_exist () {
		if (self.mSet.exist ())
			return ;
		initialize (self.mSet.unknown () ,0) ;
	}

	void check_resize (CR<Index> curr) {
		const auto r1x = self.mSet.size () ;
		const auto r2x = self.mRange.size () ;
		if (r2x == r1x)
			return ;
		self.mRange = RefBuffer<Index> (r1x) ;
		for (auto &&i : range (0 ,r1x))
			self.mRange[i] = NONE ;
		for (auto &&i : range (0 ,self.mSet.size ())) {
			if (i == curr)
				continue ;
			if (!self.mSet.used (i))
				continue ;
			update_emplace (i) ;
		}
	}
} ;

exports VFat<HashSetHolder> HashSetHolder::hold (VR<HashSetLayout> that) {
	return VFat<HashSetHolder> (HashSetImplHolder () ,that) ;
}

exports CFat<HashSetHolder> HashSetHolder::hold (CR<HashSetLayout> that) {
	return CFat<HashSetHolder> (HashSetImplHolder () ,that) ;
}

class BitSetImplHolder final implement Fat<BitSetHolder ,BitSetLayout> {
public:
	void initialize (CR<Length> size_) override {
		if (size_ <= 0)
			return ;
		const auto r1x = (size_ + 8 - 1) / 8 ;
		self.mSet = RefBuffer<Byte> (r1x) ;
		self.mWidth = size_ ;
		clear () ;
	}

	void initialize (CR<Length> size_ ,CR<Pointer> params ,VR<BoxLayout> item) override {
		const auto r1x = Unknown (BufferUnknownBinder<Index> ()) ;
		auto rax = from_initializer_list (params ,r1x) ;
		initialize (size_) ;
		const auto r2x = RFat<ReflectAssign> (r1x) ;
		for (auto &&i : range (0 ,rax.size ())) {
			BoxHolder::hold (item)->initialize (r1x) ;
			r2x->assign (BoxHolder::hold (item)->ref ,rax[i]) ;
			add (move (item)) ;
			BoxHolder::hold (item)->destroy () ;
		}
	}

	void initialize (CR<BitSetLayout> that) override {
		const auto r1x = BitSetHolder::hold (that)->size () ;
		if (r1x == 0)
			return ;
		initialize (r1x) ;
		for (auto &&i : range (0 ,that.mSet.size ()))
			self.mSet[i] = that.mSet[i] ;
	}

	void clear () override {
		fill (Byte (0X00)) ;
	}

	Length size () const override {
		if (self.mSet.size () == 0)
			return 0 ;
		return self.mWidth ;
	}

	Length length () const override {
		Length ret = 0 ;
		for (auto &&i : range (0 ,self.mSet.size ()))
			ret += ByteProc::popcount (self.mSet[i]) ;
		return move (ret) ;
	}

	void get (CR<Index> index ,VR<Bool> item) const override {
		const auto r1x = ByteProc::pow_bit (index % 8) ;
		item = ByteProc::any_bit (self.mSet[index / 8] ,r1x) ;
	}

	void set (CR<Index> index ,CR<Bool> item) override {
		const auto r1x = ByteProc::pow_bit (index % 8) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!item)
				discard ;
			self.mSet[index / 8] |= Byte (r1x) ;
		}
		if ifdo (act) {
			self.mSet[index / 8] &= ~Byte (r1x) ;
		}
	}

	Index ibegin () const override {
		const auto r1x = find_next (0) ;
		return inline_min (r1x ,size ()) ;
	}

	Index iend () const override {
		return size () ;
	}

	Index inext (CR<Index> index) const override {
		Index ix = index / 8 ;
		if ifdo (TRUE) {
			const auto r1x = index % 8 + 1 ;
			if (r1x == 8)
				discard ;
			const auto r2x = ByteProc::pow_bit (r1x) - 1 ;
			const auto r3x = self.mSet[ix] & ~Byte (r2x) ;
			if (r3x == Byte (0X00))
				discard ;
			const auto r4x = ByteProc::lowcount (r3x) ;
			const auto r5x = ix * 8 + r4x ;
			return inline_min (r5x ,size ()) ;
		}
		ix++ ;
		const auto r6x = find_next (ix) ;
		return inline_min (r6x ,size ()) ;
	}

	Index find_next (CR<Index> index) const {
		Index ix = index ;
		while (TRUE) {
			if (ix >= self.mSet.size ())
				break ;
			if ifdo (TRUE) {
				if (self.mSet[ix] == Byte (0X00))
					discard ;
				const auto r1x = ByteProc::lowcount (self.mSet[ix]) ;
				return ix * 8 + r1x ;
			}
			ix++ ;
		}
		return ix * 8 ;
	}

	Bool equal (CR<BitSetLayout> that) const override {
		const auto r1x = self.mSet.size () ;
		const auto r2x = that.mSet.size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r3x = inline_equal (self.mSet[i] ,that.mSet[i]) ;
			if (!r3x)
				return r3x ;
		}
		return TRUE ;
	}

	Flag compr (CR<BitSetLayout> that) const override {
		const auto r1x = self.mSet.size () ;
		const auto r2x = that.mSet.size () ;
		const auto r3x = inline_compr (r1x ,r2x) ;
		if (r3x != ZERO)
			return r3x ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r4x = inline_compr (self.mSet[i] ,that.mSet[i]) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		const auto r1x = self.mSet.size () ;
		for (auto &&i : range (0 ,r1x)) {
			visitor.push (self.mSet[i]) ;
		}
		visitor.leave () ;
	}

	void add (RR<BoxLayout> item) override {
		const auto r1x = bitwise[TYPE<Index>::expr] (BoxHolder::hold (item)->ref) ;
		assume (inline_between (r1x ,0 ,size ())) ;
		set (r1x ,TRUE) ;
	}

	Bool contain (CR<Pointer> item) const override {
		const auto r1x = bitwise[TYPE<Index>::expr] (item) ;
		if (!inline_between (r1x ,0 ,size ()))
			return FALSE ;
		Bool ret = FALSE ;
		get (r1x ,ret) ;
		return move (ret) ;
	}

	void erase (CR<Pointer> item) override {
		const auto r1x = bitwise[TYPE<Index>::expr] (item) ;
		if (!inline_between (r1x ,0 ,size ()))
			return ;
		set (r1x ,FALSE) ;
	}

	void fill (CR<Byte> item) override {
		for (auto &&i : range (0 ,self.mSet.size ())) {
			self.mSet[i] = Byte (0X00) ;
		}
		check_mask (self) ;
	}

	BitSetLayout sand (CR<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : range (0 ,self.mSet.size ())) {
			ret.mSet[i] = self.mSet[i] & that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout sor (CR<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : range (0 ,self.mSet.size ())) {
			ret.mSet[i] = self.mSet[i] | that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout sxor (CR<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : range (0 ,self.mSet.size ())) {
			ret.mSet[i] = self.mSet[i] ^ that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout ssub (CR<BitSetLayout> that) const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		const auto r2x = BitSetHolder::hold (that)->size () ;
		assert (r1x == r2x) ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : range (0 ,self.mSet.size ())) {
			ret.mSet[i] = self.mSet[i] & ~that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout snot () const override {
		BitSetLayout ret ;
		const auto r1x = size () ;
		BitSetHolder::hold (ret)->initialize (r1x) ;
		for (auto &&i : range (0 ,self.mSet.size ())) {
			ret.mSet[i] = ~self.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	static void check_mask (VR<BitSetLayout> that) {
		Index ix = that.mSet.size () - 1 ;
		if (ix <= 0)
			return ;
		const auto r1x = that.mWidth % 8 + 1 ;
		const auto r2x = ByteProc::pow_bit (r1x) - 1 ;
		that.mSet[ix] &= Byte (r2x) ;
	}
} ;

exports VFat<BitSetHolder> BitSetHolder::hold (VR<BitSetLayout> that) {
	return VFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}

exports CFat<BitSetHolder> BitSetHolder::hold (CR<BitSetLayout> that) {
	return CFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}
} ;