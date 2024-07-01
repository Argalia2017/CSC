#pragma once

#ifndef __CSC_ARRAY__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_array.hpp"

#include "csc_end.h"
#include <algorithm>
#include "csc_begin.h"

namespace CSC {
struct FUNCTION_buffer_from_initializer_list {
	forceinline RefBuffer<Pointer> operator() (CREF<WrapperLayout> params ,CREF<Unknown> element) const {
		RefBuffer<Pointer> ret ;
		auto &&tmp = keep[TYPE<Wrapper<CREF<Pointer>>>::expr] (Pointer::from (params)) ;
		tmp ([&] (CREF<Pointer> a) {
			const auto r1x = RFat<ReflectElement> (element)->unknown () ;
			const auto r2x = RFat<ReflectSize> (r1x) ;
			const auto r3x = r2x->type_size () ;
			const auto r4x = inline_list_begin (a ,r3x) ;
			const auto r5x = inline_list_end (a ,r3x) ;
			const auto r6x = (r5x - r4x) / r3x ;
			ret = RefBuffer<Pointer>::reference (r4x ,r6x ,r1x) ;
		}) ;
		return move (keep[TYPE<RefBuffer<Pointer>>::expr] (ret)) ;
	}
} ;

static constexpr auto buffer_from_initializer_list = FUNCTION_buffer_from_initializer_list () ;

class ArrayImplHolder implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mArray)->initialize (element ,size_) ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->xmove (at (i) ,BoxHolder::create (item)->self) ;
		}
	}

	ArrayLayout clone () const override {
		ArrayLayout ret ;
		if ifdo (TRUE) {
			const auto r1x = length () ;
			if (r1x == 0)
				discard ;
			ArrayHolder::create (ret)->initialize (fake.mArray.unknown () ,r1x) ;
			ArrayHolder::create (ret)->splice (0 ,fake) ;
		}
		return move (ret) ;
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
		const auto r1x = size () ;
		const auto r2x = ArrayHolder::create (that)->size () ;
		if (r1x != r2x)
			return FALSE ;
		const auto r3x = RFat<ReflectEqual> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r4x = r3x->equal (at (i) ,ArrayHolder::create (that)->at (i)) ;
			if ((!r4x))
				return r4x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<ArrayLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = ArrayHolder::create (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		const auto r4x = RFat<ReflectCompr> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r3x)) {
			const auto r5x = r4x->compr (at (i) ,ArrayHolder::create (that)->at (i)) ;
			if (r5x != ZERO)
				return r5x ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (VREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = size () ;
		const auto r2x = RFat<ReflectVisit> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			r2x->visit (visitor ,at (i)) ;
		}
		visitor.end () ;
	}

	void fill (CREF<Pointer> item) override {
		const auto r1x = size () ;
		const auto r2x = RFat<ReflectClone> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			r2x->clone (at (i) ,item) ;
		}
	}

	void splice (CREF<INDEX> index ,CREF<ArrayLayout> item) override {
		const auto r1x = ArrayHolder::create (item)->size () ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		const auto r2x = RFat<ReflectClone> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = index + i ;
			r2x->clone (at (ix) ,ArrayHolder::create (item)->at (i)) ;
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
	void initialize (CREF<Slice> that ,CREF<LENGTH> step_) override {
		const auto r1x = SliceHolder::create (that)->size () ;
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
			auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (fake.mString) ;
			tmp = RefBuffer<STRU8> (r1x) ;
		}
		if ifdo (act) {
			if (step_ != SIZE_OF<STRU16>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (fake.mString) ;
			tmp = RefBuffer<STRU16> (r1x) ;
			clear () ;
		}
		if ifdo (act) {
			if (step_ != SIZE_OF<STRU32>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (fake.mString) ;
			tmp = RefBuffer<STRU32> (r1x) ;
			clear () ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	StringLayout clone () const override {
		StringLayout ret ;
		if ifdo (TRUE) {
			const auto r1x = length () ;
			const auto r2x = step () ;
			if (r1x == 0)
				discard ;
			StringHolder::create (ret)->initialize (r1x ,r2x) ;
			StringHolder::create (ret)->splice (0 ,fake) ;
		}
		return move (ret) ;
	}

	void clear () override {
		trunc (0) ;
		trunc (size ()) ;
	}

	LENGTH size () const override {
		if ((!fake.mString.exist ()))
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
		return RefBufferHolder::create (fake.mString)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return RefBufferHolder::create (fake.mString)->self ;
	}

	Ref<RefBuffer<BYTE>> borrow () const override {
		//@warn: ignore cv-qualifier
		Ref<RefBuffer<BYTE>> ret = Ref<RefBuffer<BYTE>>::make () ;
		const auto r1x = fake.mString.size () * fake.mString.step () ;
		ret.self = RefBuffer<BYTE>::reference (FLAG (fake.mString.self) ,r1x ,fake.mString.unknown ()) ;
		return move (ret) ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != SIZE_OF<STRU8>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU8>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mString)) ;
			item = tmp[index] ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU16>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU8>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mString)) ;
			item = tmp[index] ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU32>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU8>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mString)) ;
			item = tmp[index] ;
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
			auto &&tmp = keep[TYPE<RefBuffer<STRU8>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mString)) ;
			tmp[index] = STRU8 (item) ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU16>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU16>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mString)) ;
			tmp[index] = STRU16 (item) ;
		}
		if ifdo (act) {
			if (step () != SIZE_OF<STRU32>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU32>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mString)) ;
			tmp[index] = STRU32 (item) ;
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
		const auto r1x = size () ;
		const auto r2x = that.size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			const auto r4x = inline_equal (rax ,that[i]) ;
			if ((!r4x))
				return r4x ;
			if (rax == 0)
				return TRUE ;
		}
		return inline_equal (r1x ,r2x) ;
	}

	BOOL equal (CREF<StringLayout> that) const override {
		const auto r1x = size () ;
		const auto r2x = StringHolder::create (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			StringHolder::create (that)->get (i ,rbx) ;
			const auto r4x = inline_equal (rax ,rbx) ;
			if ((!r4x))
				return r4x ;
			if (rax == 0)
				return TRUE ;
		}
		return inline_equal (r1x ,r2x) ;
	}

	FLAG compr (CREF<Slice> that) const override {
		const auto r1x = size () ;
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
		const auto r1x = size () ;
		const auto r2x = StringHolder::create (that)->size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			StringHolder::create (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
			if (rax == 0)
				return ZERO ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (VREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = size () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			if (rax == 0)
				break ;
			const auto r2x = bitwise[TYPE<CHAR>::expr] (rax) ;
			visitor.push (r2x) ;
		}
		visitor.end () ;
	}

	void trunc (CREF<INDEX> index) override {
		if ((!inline_between (index ,0 ,fake.mString.size ())))
			return ;
		set (index ,0) ;
	}

	void fill (CREF<STRU32> item) override {
		for (auto &&i : iter (0 ,size ())) {
			set (i ,item) ;
		}
	}

	void splice (CREF<INDEX> index ,CREF<Slice> item) override {
		const auto r1x = SliceHolder::create (item)->size () ;
		assert (inline_between (index ,0 ,size ())) ;
		assert (index + r1x <= size ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = index + i ;
			set (ix ,item[i]) ;
		}
	}

	void splice (CREF<INDEX> index ,CREF<StringLayout> item) override {
		const auto r1x = StringHolder::create (item)->length () ;
		assert (inline_between (index ,0 ,size ())) ;
		const auto r2x = index + r1x ;
		assert (index + r1x <= size ()) ;
		const auto r3x = step () ;
		inline_memcpy (fake.mString[index] ,item.mString[0] ,r1x * r3x) ;
		trunc (r2x) ;
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
	void initialize (CREF<Unknown> element) override {
		if (fake.mDeque.exist ())
			return ;
		RefBufferHolder::create (fake.mDeque)->initialize (element) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mDeque)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
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
		if ((!fake.mDeque.exist ()))
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
		if ((!fake.mDeque.exist ()))
			return TRUE ;
		return fake.mRead == fake.mWrite ;
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
		r2x->xmove (fake.mDeque.at (ix) ,BoxHolder::create (item)->self) ;
		fake.mWrite++ ;
	}

	void take () override {
		const auto r1x = fake.mDeque.size () ;
		assert (r1x > 0) ;
		INDEX ix = fake.mRead % r1x ;
		INDEX iy = fake.mWrite % r1x ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->xmove (fake.mDeque.at (ix) ,fake.mDeque.at (iy)) ;
		fake.mRead++ ;
		check_bound () ;
	}

	void push (RREF<BoxLayout> item) override {
		check_resize () ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = (fake.mRead - 1 + r1x) % r1x ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->xmove (fake.mDeque.at (ix) ,BoxHolder::create (item)->self) ;
		fake.mRead-- ;
		check_bound () ;
	}

	void pop () override {
		const auto r1x = fake.mDeque.size () ;
		assert (r1x > 0) ;
		INDEX ix = (fake.mWrite - 1 + r1x) % r1x ;
		INDEX iy = fake.mWrite % r1x ;
		const auto r2x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
		r2x->xmove (fake.mDeque.at (ix) ,fake.mDeque.at (iy)) ;
		fake.mWrite-- ;
	}

	void check_bound () {
		const auto r1x = fake.mDeque.size () ;
		if ifdo (TRUE) {
			if (fake.mRead >= 0)
				discard ;
			fake.mRead += r1x ;
			fake.mWrite += r1x ;
		}
		if (fake.mRead < r1x)
			return ;
		fake.mRead -= r1x ;
		fake.mWrite -= r1x ;
	}

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::create (fake.mDeque)->resize (r2x) ;
		if ifdo (TRUE) {
			if (fake.mWrite <= r1x)
				discard ;
			const auto r3x = RFat<ReflectAssign> (fake.mDeque.unknown ()) ;
			for (auto &&i : iter (0 ,r1x - fake.mRead)) {
				INDEX ix = r1x - 1 - i ;
				INDEX iy = r2x - 1 - i ;
				r3x->xmove (fake.mDeque.at (iy) ,fake.mDeque.at (ix)) ;
			}
			fake.mRead += r2x - r1x ;
			check_bound () ;
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
	void initialize (CREF<Unknown> element) override {
		if (fake.mPriority.exist ())
			return ;
		RefBufferHolder::create (fake.mPriority)->initialize (element) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		if (size_ <= 0)
			return ;
		RefBufferHolder::create (fake.mPriority)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
			add (move (item) ,NONE) ;
		}
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		if ((!fake.mPriority.exist ()))
			return 0 ;
		return fake.mPriority.size () ;
	}

	LENGTH step () const override {
		return fake.mPriority.step () ;
	}

	LENGTH length () const override {
		if ((!fake.mPriority.exist ()))
			return 0 ;
		return fake.mWrite ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mPriority.at (index) ;
	}

	void get (CREF<INDEX> index ,VREF<INDEX> map_) const override {
		map_ = ptr (fake ,index).mMap ;
	}

	void set (CREF<INDEX> index ,CREF<INDEX> map_) override {
		ptr (fake ,index).mMap = map_ ;
	}

	imports VREF<PriorityNode> ptr (VREF<PriorityLayout> layout ,CREF<INDEX> index) {
		return layout.mPriority.bt (index) ;
	}

	imports CREF<PriorityNode> ptr (CREF<PriorityLayout> layout ,CREF<INDEX> index) {
		return layout.mPriority.bt (index) ;
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
		if ((!fake.mPriority.exist ()))
			return TRUE ;
		return fake.mWrite == 0 ;
	}

	INDEX head () const override {
		return 0 ;
	}

	void add (RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		check_resize () ;
		INDEX ix = fake.mWrite ;
		const auto r1x = RFat<ReflectAssign> (fake.mPriority.unknown ()) ;
		r1x->xmove (fake.mPriority.at (ix) ,BoxHolder::create (item)->self) ;
		ptr (fake ,ix).mMap = map_ ;
		fake.mWrite++ ;
		update_insert (ix) ;
	}

	void take () override {
		assert ((!empty ())) ;
		INDEX ix = fake.mWrite - 1 ;
		const auto r1x = RFat<ReflectAssign> (fake.mPriority.unknown ()) ;
		r1x->xmove (fake.mPriority.at (0) ,fake.mPriority.at (ix)) ;
		ptr (fake ,0).mMap = ptr (fake ,ix).mMap ;
		fake.mWrite = ix ;
		update_insert (0) ;
	}

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::create (fake.mPriority)->resize (r2x) ;
	}

	void update_insert (CREF<INDEX> curr) {
		INDEX ix = curr ;
		INDEX jy = fake.mWrite ;
		const auto r1x = RFat<ReflectAssign> (fake.mPriority.unknown ()) ;
		const auto r2x = RFat<ReflectCompr> (fake.mPriority.unknown ()) ;
		r1x->xmove (fake.mPriority.at (jy) ,fake.mPriority.at (ix)) ;
		ptr (fake ,jy) = ptr (fake ,ix) ;
		while (TRUE) {
			INDEX iy = parent (ix) ;
			if (iy < 0)
				break ;
			const auto r3x = r2x->compr (fake.mPriority.at (jy) ,fake.mPriority.at (iy)) ;
			if (r3x >= 0)
				break ;
			r1x->xmove (fake.mPriority.at (ix) ,fake.mPriority.at (iy)) ;
			ptr (fake ,ix) = ptr (fake ,iy) ;
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
			r1x->xmove (fake.mPriority.at (ix) ,fake.mPriority.at (jx)) ;
			ptr (fake ,ix) = ptr (fake ,jx) ;
			ix = jx ;
		}
		r1x->xmove (fake.mPriority.at (ix) ,fake.mPriority.at (jy)) ;
		ptr (fake ,ix) = ptr (fake ,jy) ;
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

exports VFat<PriorityHolder> PriorityHolder::create (VREF<PriorityLayout> that) {
	return VFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

exports CFat<PriorityHolder> PriorityHolder::create (CREF<PriorityLayout> that) {
	return CFat<PriorityHolder> (PriorityImplHolder () ,that) ;
}

class ListImplHolder implement Fat<ListHolder ,ListLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mList.exist ())
			return ;
		AllocatorHolder::create (fake.mList)->initialize (element) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mList)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
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
		if ((!fake.mList.exist ()))
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
		if ((!fake.mList.exist ()))
			return TRUE ;
		return fake.mFirst != NONE ;
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
		assert ((!empty ())) ;
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
		assert ((!empty ())) ;
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
		assert ((!empty ())) ;
		INDEX ix = fake.mList.bt (index).mLeft ;
		INDEX iy = fake.mList.bt (index).mRight ;
		curr_next (ix) = iy ;
		curr_prev (iy) = ix ;
		fake.mList.free (index) ;
	}

	void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
		if (index1 == index2)
			return ;
		const auto r1x = fake.mList.bt (index1).mLeft ;
		const auto r2x = fake.mList.bt (index1).mRight ;
		const auto r3x = fake.mList.bt (index2).mLeft ;
		const auto r4x = fake.mList.bt (index2).mRight ;
		curr_next (r1x) = index2 ;
		curr_prev (r2x) = index2 ;
		curr_next (r3x) = index1 ;
		curr_prev (r4x) = index1 ;
		swap (fake.mList.bt (index1).mLeft ,fake.mList.bt (index2).mLeft) ;
		swap (fake.mList.bt (index1).mRight ,fake.mList.bt (index2).mRight) ;
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

exports VFat<ListHolder> ListHolder::create (VREF<ListLayout> that) {
	return VFat<ListHolder> (ListImplHolder () ,that) ;
}

exports CFat<ListHolder> ListHolder::create (CREF<ListLayout> that) {
	return CFat<ListHolder> (ListImplHolder () ,that) ;
}

class ArrayListImplHolder implement Fat<ArrayListHolder ,ArrayListLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mList.exist ())
			return ;
		AllocatorHolder::create (fake.mList)->initialize (element) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mList)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
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
	}

	INDEX insert (RREF<BoxLayout> item) override {
		INDEX ix = fake.mList.alloc (move (item)) ;
		check_resize () ;
		INDEX ret = find_free () ;
		assert (fake.mRange[ret] == NONE) ;
		fake.mRange[ret] = ix ;
		return move (ret) ;
	}

	INDEX insert (CREF<INDEX> index ,RREF<BoxLayout> item) override {
		assert (inline_between (index ,0 ,fake.mRange.size ())) ;
		assert ((!fake.mList.used (fake.mRange[index]))) ;
		INDEX ix = fake.mList.alloc (move (item)) ;
		check_resize () ;
		INDEX ret = index ;
		assert (fake.mRange[ret] == NONE) ;
		fake.mRange[ret] = ix ;
		return move (ret) ;
	}

	void remove (CREF<INDEX> index) override {
		INDEX ix = fake.mRange[index] ;
		fake.mRange[index] = NONE ;
		fake.mList.free (ix) ;
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
	}

	INDEX find_free () {
		INDEX ix = fake.mTop ;
		while (TRUE) {
			if (ix >= fake.mRange.size ())
				break ;
			if (fake.mRange[ix] == NONE)
				break ;
			ix++ ;
		}
		if ifdo (TRUE) {
			if (ix >= fake.mRange.size ())
				discard ;
			fake.mTop = ix ;
			return ix ;
		}
		ix = 0 ;
		while (TRUE) {
			if (ix >= fake.mTop)
				break ;
			if (fake.mRange[ix] == NONE)
				break ;
			ix++ ;
		}
		if ifdo (TRUE) {
			if (ix >= fake.mTop)
				discard ;
			fake.mTop = ix ;
			return ix ;
		}
		assert (FALSE) ;
		return INDEX () ;
	}

	void check_resize () {
		const auto r1x = fake.mList.size () ;
		const auto r2x = fake.mRange.size () ;
		if (r2x == r1x)
			return ;
		assert (r2x <= r1x) ;
		RefBufferHolder::create (fake.mRange)->initialize (BufferUnknownBinder<INDEX> ()) ;
		fake.mRange.resize (r1x) ;
		for (auto &&i : iter (r2x ,r1x)) {
			fake.mRange[i] = NONE ;
		}
	}
} ;

exports VFat<ArrayListHolder> ArrayListHolder::create (VREF<ArrayListLayout> that) {
	return VFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

exports CFat<ArrayListHolder> ArrayListHolder::create (CREF<ArrayListLayout> that) {
	return CFat<ArrayListHolder> (ArrayListImplHolder () ,that) ;
}

class SortedMapImplHolder implement Fat<SortedMapHolder ,SortedMapLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mThis.exist ())
			return ;
		fake.mThis = Ref<SortedMapImplLayout<Pointer>>::make () ;
		RefBufferHolder::create (fake.mThis->mList)->initialize (element) ;
		fake.mThis->mWrite = 0 ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		fake.mThis = Ref<SortedMapImplLayout<Pointer>>::make () ;
		RefBufferHolder::create (fake.mThis->mList)->initialize (element ,size_) ;
		fake.mThis->mWrite = 0 ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
			add (move (item) ,NONE) ;
		}
	}

	SortedMapLayout share () const override {
		SortedMapLayout ret ;
		ret.mThis = fake.mThis.share () ;
		SortedMapHolder::create (ret)->clear () ;
		return move (ret) ;
	}

	void clear () override {
		fake.mWrite = 0 ;
		fake.mSorted = FALSE ;
	}

	LENGTH size () const override {
		return fake.mRange.size () ;
	}

	LENGTH step () const override {
		return fake.mRange.step () ;
	}

	LENGTH length () const override {
		if ((!fake.mThis.exist ()))
			return 0 ;
		return fake.mWrite ;
	}

	CREF<Pointer> at (CREF<INDEX> index) const leftvalue override {
		return fake.mThis->mList.at (fake.mRange[index]) ;
	}

	imports VREF<SortedMapNode> ptr (VREF<SortedMapLayout> layout ,CREF<INDEX> index) {
		return layout.mThis->mList.bt (index) ;
	}

	imports CREF<SortedMapNode> ptr (CREF<SortedMapLayout> layout ,CREF<INDEX> index) {
		return layout.mThis->mList.bt (index) ;
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

	void add (RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		check_resize () ;
		INDEX ix = fake.mThis->mWrite ;
		const auto r1x = RFat<ReflectAssign> (fake.mThis->mList.unknown ()) ;
		r1x->xmove (fake.mThis->mList.at (ix) ,BoxHolder::create (item)->self) ;
		fake.mThis->mWrite++ ;
		ptr (fake ,ix).mMap = map_ ;
		fake.mRange[fake.mWrite] = ix ;
		fake.mWrite++ ;
		fake.mSorted = FALSE ;
	}

	INDEX find (CREF<Pointer> item) const override {
		if ((!fake.mThis.exist ()))
			return NONE ;
		assert (fake.mSorted) ;
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
		return ptr (fake ,ix).mMap ;
	}

	void remap () override {
		if ((!fake.mRange.exist ()))
			return ;
		const auto r1x = RFat<ReflectCompr> (fake.mThis->mList.unknown ()) ;
		const auto r2x = RFat<ReflectEqual> (fake.mThis->mList.unknown ()) ;
		const auto r3x = (&fake.mRange[0]) ;
		std::sort (r3x ,r3x + fake.mWrite ,[&] (CREF<INDEX> a ,CREF<INDEX> b) {
			return r1x->compr (fake.mThis->mList[a] ,fake.mThis->mList[b]) < ZERO ;
		}) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (1 ,fake.mWrite)) {
			const auto r4x = r2x->equal (fake.mThis->mList[fake.mRange[ix]] ,fake.mThis->mList[fake.mRange[i]]) ;
			if (r4x)
				continue ;
			ix++ ;
			fake.mRange[ix] = fake.mRange[i] ;
		}
		ix++ ;
		fake.mWrite = ix ;
		fake.mSorted = TRUE ;
	}

	void check_resize () {
		const auto r1x = fake.mThis->mList.size () ;
		const auto r2x = inline_max (r1x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::create (fake.mThis->mList)->resize (r2x) ;
		if (length () < size ())
			return ;
		const auto r3x = length () ;
		const auto r4x = inline_max (r3x * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
		RefBufferHolder::create (fake.mRange)->initialize (BufferUnknownBinder<INDEX> ()) ;
		fake.mRange.resize (r4x) ;
	}
} ;

exports VFat<SortedMapHolder> SortedMapHolder::create (VREF<SortedMapLayout> that) {
	return VFat<SortedMapHolder> (SortedMapImplHolder () ,that) ;
}

exports CFat<SortedMapHolder> SortedMapHolder::create (CREF<SortedMapLayout> that) {
	return CFat<SortedMapHolder> (SortedMapImplHolder () ,that) ;
}

struct SetChild {
	INDEX mUp ;
	BOOL mBin ;
} ;

class SetImplHolder implement Fat<SetHolder ,SetLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mSet.exist ())
			return ;
		AllocatorHolder::create (fake.mSet)->initialize (element) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mSet)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
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
			INDEX ix = find (BoxHolder::create (item)->self) ;
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
			if ((!red_node (iy)))
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
			if ((!red_node (jx)))
				discard ;
			fake.mSet.bt (jx).mRed = FALSE ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			fake.mTop = iy ;
		}
		if ifdo (act) {
			if ((!fake.mSet.bt (curr).mBin))
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
			if ((!red_node (jx)))
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
		if ((!fake.mSet.exist ()))
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
		if (ix == NONE)
			return ;
		fake.mSet.bt (ix).mRed = FALSE ;
	}

	void update_remove_left (CREF<INDEX> curr) {
		INDEX ix = fake.mSet.bt (curr).mRight ;
		if ifdo (TRUE) {
			if ((!red_node (ix)))
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
			if ((!red_node (ix)))
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
			if ((!binary))
				discard ;
			fake.mSet.bt (index).mBin = binary ;
			return fake.mSet.bt (index).mUp ;
		}
		assert (FALSE) ;
		return fake.mTop ;
	}
} ;

exports VFat<SetHolder> SetHolder::create (VREF<SetLayout> that) {
	return VFat<SetHolder> (SetImplHolder () ,that) ;
}

exports CFat<SetHolder> SetHolder::create (CREF<SetLayout> that) {
	return CFat<SetHolder> (SetImplHolder () ,that) ;
}

#ifdef __CSC_CONFIG_VAL32__
struct FUNCTION_fnvhash {
	forceinline CHAR operator() () const {
		return HashProc::fnvhash32 (Pointer::make (0) ,0) ;
	}

	template <class ARG1>
	forceinline CHAR operator() (CREF<ARG1> src ,CREF<CHAR> curr) const {
		return HashProc::fnvhash32 (Pointer::from (src) ,SIZE_OF<ARG1>::expr ,curr) ;
	}
} ;
#endif

#ifdef __CSC_CONFIG_VAL64__
struct FUNCTION_fnvhash {
	forceinline QUAD operator() () const {
		return HashProc::fnvhash64 (Pointer::make (0) ,0) ;
	}

	template <class ARG1>
	forceinline QUAD operator() (CREF<ARG1> src ,CREF<QUAD> curr) const {
		return HashProc::fnvhash64 (Pointer::from (src) ,SIZE_OF<ARG1>::expr ,curr) ;
	}
	} ;
#endif

static constexpr auto fnvhash = FUNCTION_fnvhash () ;

class HashcodeVisitor implement Visitor {
protected:
	BYTE_BASE<VAL> mCode ;
	LENGTH mDepth ;

public:
	void begin () override {
		if ifdo (TRUE) {
			if (mDepth != 0)
				discard ;
			mCode = fnvhash () ;
		}
		mDepth++ ;
	}

	void end () override {
		mDepth-- ;
	}

	void push (CREF<BYTE> a) override {
		mCode = fnvhash (a ,mCode) ;
	}

	void push (CREF<WORD> a) override {
		mCode = fnvhash (a ,mCode) ;
	}

	void push (CREF<CHAR> a) override {
		mCode = fnvhash (a ,mCode) ;
	}

	void push (CREF<QUAD> a) override {
		mCode = fnvhash (a ,mCode) ;
	}

	FLAG peek () const override {
		const auto r1x = mCode ;
		const auto r2x = r1x & BYTE_BASE<VAL> (VAL_MAX) ;
		return FLAG (r2x) ;
	}
} ;

class HashSetImplHolder implement Fat<HashSetHolder ,HashSetLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		if (fake.mSet.exist ())
			return ;
		AllocatorHolder::create (fake.mSet)->initialize (element) ;
		fake.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mSet)->initialize (element ,size_) ;
		fake.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
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
		return find_next (index) ;
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
			INDEX ix = find (BoxHolder::create (item)->self) ;
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
		fake.mVisitor->begin () ;
		const auto r1x = RFat<ReflectVisit> (fake.mSet.unknown ()) ;
		r1x->visit (fake.mVisitor.self ,item) ;
		fake.mVisitor->end () ;
		return fake.mVisitor->peek () ;
	}

	void update_emplace (CREF<INDEX> curr) {
		assert (fake.mRange.size () > 0) ;
		INDEX ix = fake.mSet.bt (curr).mHash % fake.mRange.size () ;
		auto act = TRUE ;
		if ifdo (act) {
			INDEX jx = fake.mRange[ix] ;
			if (jx == NONE)
				discard ;
			fake.mSet.bt (curr).mDown = fake.mSet.bt (jx).mDown ;
			fake.mSet.bt (jx).mDown = curr ;
			fake.mRange[ix] = curr ;
		}
		if ifdo (act) {
			fake.mSet.bt (curr).mDown = curr ;
			fake.mRange[ix] = curr ;
		}
	}

	INDEX find (CREF<Pointer> item) const override {
		if ((!fake.mSet.exist ()))
			return NONE ;
		if (fake.mRange.size () == 0)
			return NONE ;
		INDEX ret = NONE ;
		const auto r1x = hashcode (item) ;
		INDEX jx = r1x % fake.mRange.size () ;
		INDEX ix = fake.mRange[jx] ;
		const auto r2x = RFat<ReflectEqual> (fake.mSet.unknown ()) ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			if ifdo (TRUE) {
				if (fake.mSet.bt (ix).mHash != r1x)
					discard ;
				const auto r3x = r2x->equal (item ,fake.mSet.at (ix)) ;
				if ((!r3x))
					discard ;
				ret = ix ;
			}
			if (ret != NONE)
				break ;
			ix = fake.mSet.bt (ix).mDown ;
			if (ix == fake.mRange[jx])
				break ;
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
		INDEX ix = find_successor (index) ;
		INDEX jx = fake.mSet.bt (ix).mHash % fake.mRange.size () ;
		fake.mSet.bt (ix).mDown = fake.mSet.bt (index).mDown ;
		if ifdo (TRUE) {
			if (ix != index)
				discard ;
			ix = NONE ;
		}
		fake.mRange[jx] = NONE ;
		fake.mSet.free (index) ;
	}

	INDEX find_successor (CREF<INDEX> index) const {
		INDEX ret = index ;
		while (TRUE) {
			if (fake.mSet.bt (ret).mDown == index)
				break ;
			ret = fake.mSet.bt (ret).mDown ;
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
			if ((!fake.mSet.used (i)))
				continue ;
			update_emplace (i) ;
		}
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
		if (size_ <= 0)
			return ;
		const auto r1x = (size_ + 8 - 1) / 8 ;
		fake.mSet = RefBuffer<BYTE> (r1x) ;
		fake.mWidth = size_ ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<WrapperLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (rax.size ()) ;
		const auto r1x = RFat<ReflectAssign> (element) ;
		const auto r2x = RFat<ReflectElement> (element)->unknown () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			r1x->xmove (BoxHolder::create (item)->self ,rax[i]) ;
			add (move (item)) ;
		}
	}

	BitSetLayout clone () const override {
		BitSetLayout ret ;
		if ifdo (TRUE) {
			const auto r1x = size () ;
			if (r1x == 0)
				discard ;
			BitSetHolder::create (ret)->initialize (r1x) ;
			for (auto &&i : iter (0 ,fake.mSet.size ()))
				ret.mSet[i] = fake.mSet[i] ;
		}
		return move (ret) ;
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
		const auto r1x = ByteProc::bit_pow (index % 8) ;
		const auto r2x = fake.mSet[index / 8] & BYTE (r1x) ;
		item = BOOL (r2x != BYTE (0X00)) ;
	}

	void set (CREF<INDEX> index ,CREF<BOOL> item) override {
		const auto r1x = ByteProc::bit_pow (index % 8) ;
		auto act = TRUE ;
		if ifdo (act) {
			if ((!item))
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
			const auto r2x = ByteProc::bit_pow (r1x) - 1 ;
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
			if ((!r3x))
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
		for (auto &&i : iter (0 ,r3x)) {
			const auto r4x = inline_compr (fake.mSet[i] ,that.mSet[i]) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return ZERO ;
	}

	void visit (VREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = fake.mSet.size () ;
		for (auto &&i : iter (0 ,r1x)) {
			visitor.push (fake.mSet[i]) ;
		}
		visitor.end () ;
	}

	void add (RREF<BoxLayout> item) override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (BoxHolder::create (item)->self) ;
		assume (inline_between (r1x ,0 ,size ())) ;
		set (r1x ,TRUE) ;
	}

	BOOL contain (CREF<Pointer> item) const override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (item) ;
		if ((!inline_between (r1x ,0 ,size ())))
			return FALSE ;
		auto rax = FALSE ;
		get (r1x ,rax) ;
		return move (rax) ;
	}

	void erase (CREF<Pointer> item) override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (item) ;
		if ((!inline_between (r1x ,0 ,size ())))
			return ;
		set (r1x ,FALSE) ;
	}

	void fill (CREF<BYTE> item) override {
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			fake.mSet[i] = BYTE (0X00) ;
		}
		check_mask (fake) ;
	}

	BitSetLayout band (CREF<BitSetLayout> that) const override {
		assert (fake.mWidth == that.mWidth) ;
		BitSetLayout ret ;
		BitSetHolder::create (ret)->initialize (fake.mWidth) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] & that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout bor (CREF<BitSetLayout> that) const override {
		assert (fake.mWidth == that.mWidth) ;
		BitSetLayout ret ;
		BitSetHolder::create (ret)->initialize (fake.mWidth) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] | that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout bxor (CREF<BitSetLayout> that) const override {
		assert (fake.mWidth == that.mWidth) ;
		BitSetLayout ret ;
		BitSetHolder::create (ret)->initialize (fake.mWidth) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] ^ that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout bsub (CREF<BitSetLayout> that) const override {
		assert (fake.mWidth == that.mWidth) ;
		BitSetLayout ret ;
		BitSetHolder::create (ret)->initialize (fake.mWidth) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = fake.mSet[i] & ~that.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	BitSetLayout bnot () const override {
		BitSetLayout ret ;
		BitSetHolder::create (ret)->initialize (fake.mWidth) ;
		for (auto &&i : iter (0 ,fake.mSet.size ())) {
			ret.mSet[i] = ~fake.mSet[i] ;
		}
		check_mask (ret) ;
		return move (ret) ;
	}

	void check_mask (VREF<BitSetLayout> layout) const {
		INDEX ix = layout.mSet.size () - 1 ;
		if (ix <= 0)
			return ;
		const auto r1x = layout.mWidth % 8 + 1 ;
		const auto r2x = ByteProc::bit_pow (r1x) - 1 ;
		layout.mSet[ix] &= BYTE (r2x) ;
	}
} ;

exports VFat<BitSetHolder> BitSetHolder::create (VREF<BitSetLayout> that) {
	return VFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}

exports CFat<BitSetHolder> BitSetHolder::create (CREF<BitSetLayout> that) {
	return CFat<BitSetHolder> (BitSetImplHolder () ,that) ;
}
} ;