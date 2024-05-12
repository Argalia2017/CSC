#pragma once

#ifndef __CSC_ARRAY__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_array.hpp"

#include "csc_end.h"
#include <initializer_list>
#include "csc_begin.h"

namespace CSC {
#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_buffer_from_initializer_list {
	forceinline RefBuffer<Pointer> operator() (CREF<CaptureLayout> params ,CREF<Unknown> element) const {
		RefBufferLayout ret ;
		auto &&tmp = Capture<CREF<csc_initializer_t<Pointer>>>::from (params) ;
		tmp ([&] (CREF<csc_initializer_t<Pointer>> a) {
			const auto r1x = FLAG (a.begin ()) ;
			const auto r2x = FLAG (a.end ()) ;
			const auto r3x = RFat<ReflectSize> (element) ;
			const auto r4x = r3x->type_size () ;
			RefBufferHolder::create (ret)->initialize (element) ;
			ret.mBuffer = r1x ;
			ret.mSize = (r2x - r1x) / r4x ;
			ret.mStep = r4x ;
		}) ;
		return move (keep[TYPE<RefBuffer<Pointer>>::expr] (ret)) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_buffer_from_initializer_list {
	forceinline RefBuffer<Pointer> operator() (CREF<CaptureLayout> params ,CREF<Unknown> element) const {
		RefBufferLayout ret ;
		auto &&tmp = Capture<CREF<csc_initializer_t<Pointer>>>::from (params) ;
		tmp ([&] (CREF<csc_initializer_t<Pointer>> a) {
			const auto r1x = FLAG (a.begin ()) ;
			const auto r2x = LENGTH (a.size ()) ;
			const auto r3x = RFat<ReflectSize> (element) ;
			const auto r4x = r3x->type_size () ;
			RefBufferHolder::create (ret)->initialize (element) ;
			ret.mBuffer = r1x ;
			ret.mSize = r2x ;
			ret.mStep = r4x ;
		}) ;
		return move (keep[TYPE<RefBuffer<Pointer>>::expr] (ret)) ;
	}
} ;
#endif

static constexpr auto buffer_from_initializer_list = FUNCTION_buffer_from_initializer_list () ;

class ArrayImplement implement Fat<ArrayHolder ,ArrayLayout> {
public:
	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mArray)->initialize (element ,size_) ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
			r1x->drop (at (i)) ;
			r1x->copy (at (i) ,BoxHolder::create (item)->self) ;
			BoxHolder::create (item)->release () ;
		}
	}

	void initialize (CREF<ArrayLayout> that) override {
		const auto r1x = ArrayHolder::create (that)->size () ;
		if (r1x == 0)
			return ;
		initialize (that.mArray.unknown () ,r1x) ;
		const auto r2x = RFat<ReflectClone> (that.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			r2x->clone (at (i) ,ArrayHolder::create (that)->at (i)) ;
		}
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
			if ifnot (r4x)
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

	void visit (CREF<Visitor> visitor) const override {
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

	ArrayLayout segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const override {
		ArrayLayout ret ;
		const auto r1x = inline_max (end_ - begin_ ,0) ;
		ArrayHolder::create (ret)->initialize (fake.mArray.unknown () ,r1x) ;
		const auto r2x = RFat<ReflectClone> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = begin_ + i ;
			r2x->clone (ArrayHolder::create (ret)->at (i) ,at (ix)) ;
		}
		return move (ret) ;
	}

	void splice (CREF<INDEX> index ,CREF<ArrayLayout> item) override {
		const auto r1x = ArrayHolder::create (item)->size () ;
		assume (inline_between (index ,0 ,size ())) ;
		assume (index + r1x <= size ()) ;
		const auto r2x = RFat<ReflectClone> (fake.mArray.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = index + i ;
			r2x->clone (at (ix) ,ArrayHolder::create (item)->at (i)) ;
		}
	}
} ;

exports VFat<ArrayHolder> ArrayHolder::create (VREF<ArrayLayout> that) {
	return VFat<ArrayHolder> (ArrayImplement () ,Pointer::from (that)) ;
}

exports CFat<ArrayHolder> ArrayHolder::create (CREF<ArrayLayout> that) {
	return CFat<ArrayHolder> (ArrayImplement () ,Pointer::from (that)) ;
}

class StringImplement implement Fat<StringHolder ,StringLayout> {
public:
	void initialize (CREF<Slice<STRA>> that) override {
		const auto r1x = SliceHolder::create (that)->size () ;
		const auto r2x = SliceHolder::create (that)->step () ;
		initialize (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CREF<Slice<STRW>> that) override {
		const auto r1x = SliceHolder::create (that)->size () ;
		const auto r2x = SliceHolder::create (that)->step () ;
		initialize (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CREF<Slice<STRU8>> that) override {
		const auto r1x = SliceHolder::create (that)->size () ;
		const auto r2x = SliceHolder::create (that)->step () ;
		initialize (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CREF<Slice<STRU16>> that) override {
		const auto r1x = SliceHolder::create (that)->size () ;
		const auto r2x = SliceHolder::create (that)->step () ;
		initialize (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CREF<Slice<STRU32>> that) override {
		const auto r1x = SliceHolder::create (that)->size () ;
		const auto r2x = SliceHolder::create (that)->step () ;
		initialize (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			set (i ,that[i]) ;
		}
		trunc (r1x) ;
	}

	void initialize (CREF<LENGTH> size_ ,CREF<LENGTH> step_) override {
		const auto r1x = size_ + 1 ;
		auto act = TRUE ;
		if ifdo (act) {
			if (step_ != ALIGN_OF<STRU8>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (fake.mString) ;
			tmp = RefBuffer<STRU8> (r1x) ;
		}
		if ifdo (act) {
			if (step_ != ALIGN_OF<STRU16>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (fake.mString) ;
			tmp = RefBuffer<STRU8> (r1x) ;
			clear () ;
		}
		if ifdo (act) {
			if (step_ != ALIGN_OF<STRU32>::expr)
				discard ;
			auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (fake.mString) ;
			tmp = RefBuffer<STRU8> (r1x) ;
			clear () ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void initialize (CREF<StringLayout> that) override {
		const auto r1x = StringHolder::create (that)->length () ;
		const auto r2x = StringHolder::create (that)->step () ;
		if (r1x == 0)
			return ;
		initialize (r1x ,r2x) ;
		const auto r3x = RFat<ReflectClone> (that.mString.unknown ()) ;
		for (auto &&i : iter (0 ,r1x)) {
			r3x->clone (at (i) ,StringHolder::create (that)->at (i)) ;
		}
		trunc (r1x) ;
	}

	void clear () override {
		set (0 ,0) ;
		set (size () ,0) ;
	}

	LENGTH size () const override {
		if ifnot (fake.mString.exist ())
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

	VREF<ARR<STRA>> raw (TYPEID<STRA>) leftvalue override {
		assume (step () == SIZE_OF<STRA>::expr) ;
		return fake.mString.raw () ;
	}

	CREF<ARR<STRA>> raw (TYPEID<STRA>) const leftvalue override {
		assume (step () == SIZE_OF<STRA>::expr) ;
		return fake.mString.raw () ;
	}

	VREF<ARR<STRW>> raw (TYPEID<STRW>) leftvalue override {
		assume (step () == SIZE_OF<STRW>::expr) ;
		return fake.mString.raw () ;
	}

	CREF<ARR<STRW>> raw (TYPEID<STRW>) const leftvalue override {
		assume (step () == SIZE_OF<STRW>::expr) ;
		return fake.mString.raw () ;
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != ALIGN_OF<STRU8>::expr)
				discard ;
			item = bitwise[TYPE<STRU8>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (step () != ALIGN_OF<STRU16>::expr)
				discard ;
			item = bitwise[TYPE<STRU16>::expr] (at (index)) ;
		}
		if ifdo (act) {
			if (step () != ALIGN_OF<STRU32>::expr)
				discard ;
			item = bitwise[TYPE<STRU32>::expr] (at (index)) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void set (CREF<INDEX> index ,CREF<STRU32> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (step () != ALIGN_OF<STRU8>::expr)
				discard ;
			auto &&tmp = keep[TYPE<STRU8>::expr] (at (index)) ;
			tmp = STRU8 (item) ;
		}
		if ifdo (act) {
			if (step () != ALIGN_OF<STRU16>::expr)
				discard ;
			auto &&tmp = keep[TYPE<STRU16>::expr] (at (index)) ;
			tmp = STRU16 (item) ;
		}
		if ifdo (act) {
			if (step () != ALIGN_OF<STRU32>::expr)
				discard ;
			auto &&tmp = keep[TYPE<STRU32>::expr] (at (index)) ;
			tmp = STRU32 (item) ;
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

	BOOL equal (CREF<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::create (that)->length () ;
		if (r1x != r2x)
			return FALSE ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			StringHolder::create (that)->get (i ,rbx) ;
			if (rax != rbx)
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<StringLayout> that) const override {
		const auto r1x = length () ;
		const auto r2x = StringHolder::create (that)->length () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		auto rax = STRU32 () ;
		auto rbx = STRU32 () ;
		for (auto &&i : iter (0 ,r3x)) {
			get (i ,rax) ;
			StringHolder::create (that)->get (i ,rbx) ;
			const auto r4x = inline_compr (rax ,rbx) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = length () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			get (i ,rax) ;
			const auto r2x = bitwise[TYPE<CHAR>::expr] (rax) ;
			visitor.push (r2x) ;
		}
		visitor.end () ;
	}

	void fill (CREF<STRU32> item) override {
		for (auto &&i : iter (0 ,size ())) {
			set (i ,item) ;
		}
	}

	void trunc (CREF<INDEX> index) override {
		if ifnot (inline_between (index ,0 ,fake.mString.size ()))
			return ;
		set (index ,0) ;
	}

	StringLayout segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const override {
		StringLayout ret ;
		const auto r1x = inline_max (end_ - begin_ ,0) ;
		const auto r2x = step () ;
		StringHolder::create (ret)->initialize (r1x ,r2x) ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = begin_ + i ;
			get (i ,rax) ;
			StringHolder::create (ret)->set (ix ,rax) ;
		}
		return move (ret) ;
	}

	void splice (CREF<INDEX> index ,CREF<StringLayout> item) override {
		const auto r1x = StringHolder::create (item)->length () ;
		assume (inline_between (index ,0 ,size ())) ;
		assume (index + r1x <= size ()) ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = index + i ;
			StringHolder::create (item)->get (i ,rax) ;
			set (ix ,rax) ;
		}
	}
} ;

exports VFat<StringHolder> StringHolder::create (VREF<StringLayout> that) {
	return VFat<StringHolder> (StringImplement () ,Pointer::from (that)) ;
}

exports CFat<StringHolder> StringHolder::create (CREF<StringLayout> that) {
	return CFat<StringHolder> (StringImplement () ,Pointer::from (that)) ;
}

class DequeImplement implement Fat<DequeHolder ,DequeLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		RefBufferHolder::create (fake.mDeque)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mDeque)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
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
		if ifnot (fake.mDeque.exist ())
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
		if ifnot (fake.mDeque.exist ())
			return TRUE ;
		return fake.mRead == fake.mWrite ;
	}

	INDEX head () const override {
		return 0 ;
	}

	INDEX tail () const override {
		return length () ;
	}

	void add (RREF<BoxLayout> item) override {
		check_resize () ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = fake.mWrite % r1x ;
		const auto r2x = RFat<ReflectMove> (fake.mDeque.unknown ()) ;
		r2x->drop (fake.mDeque.at (ix)) ;
		r2x->copy (fake.mDeque.at (ix) ,BoxHolder::create (item)->self) ;
		BoxHolder::create (item)->release () ;
		fake.mWrite++ ;
	}

	void take () override {
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = fake.mRead % r1x ;
		const auto r2x = RFat<ReflectMove> (fake.mDeque.unknown ()) ;
		r2x->drop (fake.mDeque.at (ix)) ;
		fake.mRead++ ;
		check_bound () ;
	}

	void push (RREF<BoxLayout> item) override {
		check_resize () ;
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = (fake.mRead - 1 + r1x) % r1x ;
		const auto r2x = RFat<ReflectMove> (fake.mDeque.unknown ()) ;
		r2x->drop (fake.mDeque.at (ix)) ;
		r2x->copy (fake.mDeque.at (ix) ,BoxHolder::create (item)->self) ;
		BoxHolder::create (item)->release () ;
		fake.mRead-- ;
		check_bound () ;
	}

	void pop () override {
		const auto r1x = fake.mDeque.size () ;
		INDEX ix = (fake.mWrite - 1 + r1x) % r1x ;
		const auto r2x = RFat<ReflectMove> (fake.mDeque.unknown ()) ;
		r2x->drop (fake.mDeque.at (ix)) ;
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
		const auto r2x = inline_max (r1x * 2 ,256) ;
		RefBufferHolder::create (fake.mDeque)->resize (r2x) ;
		if ifdo (TRUE) {
			if (fake.mRead <= fake.mWrite)
				discard ;
			const auto r3x = RFat<ReflectMove> (fake.mDeque.unknown ()) ;
			for (auto &&i : iter (0 ,r2x - r1x)) {
				INDEX ix = r2x - 1 - i ;
				INDEX iy = r1x - 1 - i ;
				r3x->drop (fake.mDeque.at (iy)) ;
				r3x->copy (fake.mDeque.at (iy) ,fake.mDeque.at (ix)) ;
				r3x->init (fake.mDeque.at (ix)) ;
			}
			fake.mRead += r2x - r1x ;
			check_bound () ;
		}
	}
} ;

exports VFat<DequeHolder> DequeHolder::create (VREF<DequeLayout> that) {
	return VFat<DequeHolder> (DequeImplement () ,Pointer::from (that)) ;
}

exports CFat<DequeHolder> DequeHolder::create (CREF<DequeLayout> that) {
	return CFat<DequeHolder> (DequeImplement () ,Pointer::from (that)) ;
}

class PriorityImplement implement Fat<PriorityHolder ,PriorityLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		RefBufferHolder::create (fake.mPriority)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		const auto r1x = size_ + 1 ;
		RefBufferHolder::create (fake.mPriority)->initialize (element ,r1x) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
			add (move (item) ,NONE) ;
		}
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		if ifnot (fake.mPriority.exist ())
			return 0 ;
		return fake.mPriority.size () - 1 ;
	}

	LENGTH step () const override {
		return fake.mPriority.step () ;
	}

	LENGTH length () const override {
		if ifnot (fake.mPriority.exist ())
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
		if ifnot (fake.mPriority.exist ())
			return TRUE ;
		return fake.mWrite == 0 ;
	}

	INDEX head () const override {
		return 0 ;
	}

	void add (RREF<BoxLayout> item ,CREF<INDEX> map_) override {
		check_resize () ;
		INDEX ix = fake.mWrite ;
		const auto r1x = RFat<ReflectMove> (fake.mPriority.unknown ()) ;
		r1x->drop (fake.mPriority.at (ix)) ;
		r1x->copy (fake.mPriority.at (ix) ,BoxHolder::create (item)->self) ;
		BoxHolder::create (item)->release () ;
		ptr (fake ,ix).mMap = map_ ;
		fake.mWrite++ ;
		update_insert (ix) ;
	}

	void take () override {
		assert (ifnot (empty ())) ;
		INDEX ix = fake.mWrite - 1 ;
		const auto r1x = RFat<ReflectMove> (fake.mPriority.unknown ()) ;
		r1x->drop (fake.mPriority.at (0)) ;
		r1x->copy (fake.mPriority.at (0) ,fake.mPriority.at (ix)) ;
		r1x->init (fake.mPriority.at (ix)) ;
		ptr (fake ,0).mMap = ptr (fake ,ix).mMap ;
		fake.mWrite = ix ;
		update_insert (0) ;
	}

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,256) ;
		RefBufferHolder::create (fake.mPriority)->resize (r2x) ;
	}

	void update_insert (CREF<INDEX> curr) {
		INDEX ix = curr ;
		INDEX jy = fake.mWrite ;
		const auto r1x = RFat<ReflectMove> (fake.mPriority.unknown ()) ;
		const auto r2x = RFat<ReflectCompr> (fake.mPriority.unknown ()) ;
		r1x->drop (fake.mPriority.at (jy)) ;
		r1x->copy (fake.mPriority.at (jy) ,fake.mPriority.at (ix)) ;
		ptr (fake ,jy) = ptr (fake ,ix) ;
		while (TRUE) {
			INDEX iy = parent (ix) ;
			if (iy < 0)
				break ;
			const auto r3x = r2x->compr (fake.mPriority.at (jy) ,fake.mPriority.at (iy)) ;
			if (r3x >= 0)
				break ;
			r1x->copy (fake.mPriority.at (ix) ,fake.mPriority.at (iy)) ;
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
			r1x->copy (fake.mPriority.at (ix) ,fake.mPriority.at (jx)) ;
			ptr (fake ,ix) = ptr (fake ,jx) ;
			ix = jx ;
		}
		r1x->copy (fake.mPriority.at (ix) ,fake.mPriority.at (jy)) ;
		ptr (fake ,ix) = ptr (fake ,jy) ;
		r1x->init (fake.mPriority.at (jy)) ;
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
	return VFat<PriorityHolder> (PriorityImplement () ,Pointer::from (that)) ;
}

exports CFat<PriorityHolder> PriorityHolder::create (CREF<PriorityLayout> that) {
	return CFat<PriorityHolder> (PriorityImplement () ,Pointer::from (that)) ;
}

class ListImplement implement Fat<ListHolder ,ListLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		AllocatorHolder::create (fake.mList)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mList)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
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
		if ifnot (fake.mList.exist ())
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
		if ifnot (fake.mList.exist ())
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
		INDEX ix = AllocatorHolder::create (fake.mList)->alloc (move (item)) ;
		fake.mList.bt (ix).mLeft = fake.mLast ;
		fake.mList.bt (ix).mRight = NONE ;
		curr_next (fake.mLast) = ix ;
		curr_prev (NONE) = ix ;
	}

	void take () override {
		assume (ifnot (empty ())) ;
		INDEX ix = fake.mFirst ;
		INDEX iy = fake.mList.bt (ix).mRight ;
		curr_next (NONE) = iy ;
		curr_prev (iy) = NONE ;
		fake.mList.free (ix) ;
	}

	void push (RREF<BoxLayout> item) override {
		INDEX ix = AllocatorHolder::create (fake.mList)->alloc (move (item)) ;
		fake.mList.bt (ix).mLeft = NONE ;
		fake.mList.bt (ix).mRight = fake.mFirst ;
		curr_next (NONE) = ix ;
		curr_prev (fake.mFirst) = ix ;
	}

	void pop () override {
		assume (ifnot (empty ())) ;
		INDEX ix = fake.mLast ;
		INDEX iy = fake.mList.bt (ix).mLeft ;
		curr_next (iy) = NONE ;
		curr_prev (NONE) = iy ;
		fake.mList.free (ix) ;
	}

	INDEX insert () override {
		INDEX ret = AllocatorHolder::create (fake.mList)->alloc () ;
		fake.mList.bt (ret).mLeft = fake.mLast ;
		fake.mList.bt (ret).mRight = NONE ;
		curr_next (fake.mLast) = ret ;
		curr_prev (NONE) = ret ;
		return move (ret) ;
	}

	INDEX insert (CREF<INDEX> index) override {
		assume (fake.mList.used (index)) ;
		INDEX ret = AllocatorHolder::create (fake.mList)->alloc () ;
		INDEX ix = fake.mList.bt (index).mLeft ;
		fake.mList.bt (ret).mLeft = ix ;
		fake.mList.bt (ret).mRight = index ;
		curr_next (ix) = ret ;
		curr_prev (index) = ret ;
		return move (ret) ;
	}

	void remove (CREF<INDEX> index) override {
		assume (ifnot (empty ())) ;
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
	return VFat<ListHolder> (ListImplement () ,Pointer::from (that)) ;
}

exports CFat<ListHolder> ListHolder::create (CREF<ListLayout> that) {
	return CFat<ListHolder> (ListImplement () ,Pointer::from (that)) ;
}

class ArrayListImplement implement Fat<ArrayListHolder ,ArrayListLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		AllocatorHolder::create (fake.mList)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mList)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
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
		INDEX ix = AllocatorHolder::create (fake.mList)->alloc (move (item)) ;
		check_resize () ;
		INDEX iy = find_free () ;
		assume (fake.mRange[iy] == NONE) ;
		fake.mRange[iy] = ix ;
	}

	INDEX insert () override {
		INDEX ix = AllocatorHolder::create (fake.mList)->alloc () ;
		check_resize () ;
		INDEX ret = find_free () ;
		assume (fake.mRange[ret] == NONE) ;
		fake.mRange[ret] = ix ;
		return move (ret) ;
	}

	INDEX insert (CREF<INDEX> index) override {
		assume (inline_between (index ,0 ,fake.mRange.size ())) ;
		const auto r1x = fake.mList.used (fake.mRange[index]) ;
		assume (ifnot (r1x)) ;
		INDEX ix = AllocatorHolder::create (fake.mList)->alloc () ;
		check_resize () ;
		INDEX ret = index ;
		assume (fake.mRange[ret] == NONE) ;
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
			if (iy == NONE)
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
		assume (FALSE) ;
		return INDEX () ;
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

exports VFat<ArrayListHolder> ArrayListHolder::create (VREF<ArrayListLayout> that) {
	return VFat<ArrayListHolder> (ArrayListImplement () ,Pointer::from (that)) ;
}

exports CFat<ArrayListHolder> ArrayListHolder::create (CREF<ArrayListLayout> that) {
	return CFat<ArrayListHolder> (ArrayListImplement () ,Pointer::from (that)) ;
}

class SortedListImplement implement Fat<SortedListHolder ,SortedListLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		RefBufferHolder::create (fake.mList)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		RefBufferHolder::create (fake.mList)->initialize (element ,size_) ;
		clear () ;
	}

	void clear () override {
		fake.mRead = 0 ;
		fake.mWrite = 0 ;
	}

	LENGTH size () const override {
		return fake.mList.size () ;
	}

	LENGTH step () const override {
		return fake.mList.step () ;
	}

	LENGTH length () const override {
		if ifnot (fake.mList.exist ())
			return 0 ;
		return fake.mWrite ;
	}

	VREF<INDEX> at (CREF<INDEX> index) leftvalue override {
		return ptr (fake ,index).mMap ;
	}

	CREF<INDEX> at (CREF<INDEX> index) const leftvalue override {
		return ptr (fake ,index).mMap ;
	}

	imports VREF<SortedListNode> ptr (VREF<SortedListLayout> layout ,CREF<INDEX> index) {
		return layout.mList.bt (index) ;
	}

	imports CREF<SortedListNode> ptr (CREF<SortedListLayout> layout ,CREF<INDEX> index) {
		return layout.mList.bt (index) ;
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
		if (duplicate (item))
			return ;
		check_resize () ;
		const auto r1x = RFat<ReflectMove> (fake.mList.unknown ()) ;
		INDEX ix = fake.mWrite ;
		r1x->drop (fake.mList.at (ix)) ;
		r1x->copy (fake.mList.at (ix) ,BoxHolder::create (item)->self) ;
		BoxHolder::create (item)->release () ;
		ptr (fake ,ix).mMap = map_ ;
		fake.mRead = ix ;
		fake.mWrite++ ;
	}

	BOOL duplicate (CREF<BoxLayout> item) const {
		if (length () == 0)
			return FALSE ;
		const auto r1x = RFat<ReflectCompr> (fake.mList.unknown ()) ;
		INDEX ix = fake.mRead ;
		const auto r2x = r1x->compr (fake.mList.at (ix) ,BoxHolder::create (item)->self) ;
		if (r2x == ZERO)
			return TRUE ;
		assume (r2x < 0) ;
		return FALSE ;
	}

	INDEX find (CREF<Pointer> item) const override {
		INDEX ix = 0 ;
		INDEX iy = length () - 1 ;
		INDEX iz = 0 ;
		const auto r1x = RFat<ReflectCompr> (fake.mList.unknown ()) ;
		while (TRUE) {
			if (ix > iy)
				break ;
			iz = ix + (iy - ix) / 2 ;
			const auto r2x = r1x->compr (item ,fake.mList.at (iz)) ;
			if (r2x == ZERO)
				return iz ;
			auto act = TRUE ;
			if ifdo (act) {
				if ifnot (r2x < ZERO)
					discard ;
				iy = iz - 1 ;
			}
			if ifdo (act) {
				if ifnot (r2x > ZERO)
					discard ;
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

	void check_resize () {
		if (length () < size ())
			return ;
		const auto r1x = length () ;
		const auto r2x = inline_max (r1x * 2 ,256) ;
		RefBufferHolder::create (fake.mList)->resize (r2x) ;
	}
} ;

exports VFat<SortedListHolder> SortedListHolder::create (VREF<SortedListLayout> that) {
	return VFat<SortedListHolder> (SortedListImplement () ,Pointer::from (that)) ;
}

exports CFat<SortedListHolder> SortedListHolder::create (CREF<SortedListLayout> that) {
	return CFat<SortedListHolder> (SortedListImplement () ,Pointer::from (that)) ;
}

struct SetChild {
	INDEX mUp ;
	BOOL mBin ;
} ;

class SetImplement implement Fat<SetHolder ,SetLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		AllocatorHolder::create (fake.mSet)->initialize (element) ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mSet)->initialize (element ,size_) ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
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
		INDEX ix = find (BoxHolder::create (item)->self) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = AllocatorHolder::create (fake.mSet)->alloc (move (item)) ;
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
			if ifnot (red_node (iy))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (fake.mSet.bt (iy).mBin)
					discard ;
				update_insert_left (ix) ;
				ix = fake.mTop ;
			}
			if ifdo (act) {
				if ifnot (fake.mSet.bt (iy).mBin)
					discard ;
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
			if ifnot (red_node (jx))
				discard ;
			fake.mSet.bt (jx).mRed = FALSE ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			fake.mTop = iy ;
		}
		if ifdo (act) {
			if ifnot (fake.mSet.bt (curr).mBin)
				discard ;
			rotate_left (fake.mSet.bt (iy).mLeft) ;
			fake.mSet.bt (curr).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			rotate_right (iy) ;
			fake.mTop = ix ;
		}
		if ifdo (act) {
			if (fake.mSet.bt (curr).mBin)
				discard ;
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
			if ifnot (red_node (jx))
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
			if ifnot (fake.mSet.bt (curr).mBin)
				discard ;
			fake.mSet.bt (ix).mRed = FALSE ;
			fake.mSet.bt (iy).mRed = TRUE ;
			rotate_left (iy) ;
			fake.mTop = curr ;
		}
	}

	INDEX find (CREF<Pointer> item) const override {
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
				if ifnot (ix == fake.mSet.bt (iy).mLeft)
					discard ;
				update_remove_left (iy) ;
				ix = fake.mTop ;
			}
			if ifdo (act) {
				if ifnot (ix == fake.mSet.bt (iy).mRight)
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
			if ifnot (red_node (ix))
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
			if ifnot (red_node (ix))
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
			if ifnot (binary)
				discard ;
			fake.mSet.bt (index).mBin = binary ;
			return fake.mSet.bt (index).mUp ;
		}
		assume (FALSE) ;
		return fake.mTop ;
	}
} ;

exports VFat<SetHolder> SetHolder::create (VREF<SetLayout> that) {
	return VFat<SetHolder> (SetImplement () ,Pointer::from (that)) ;
}

exports CFat<SetHolder> SetHolder::create (CREF<SetLayout> that) {
	return CFat<SetHolder> (SetImplement () ,Pointer::from (that)) ;
}

#ifdef __CSC_CONFIG_VAL32__
struct FUNCTION_fnvhash {
	using FNV_BASIS = ENUM<VAL (2166136261)> ;
	using FNV_PRIME = ENUM<VAL (16777619)> ;

	forceinline VAL operator() () const {
		return FNV_BASIS::expr ;
	}

	forceinline VAL operator() (CREF<VAL> curr ,CREF<Pointer> buffer ,CREF<LENGTH> size_) const {
		using R1X = BYTE_BASE<VAL> ;
		VAL ret = curr ;
		auto &&tmp = keep[TYPE<ARR<BYTE>>::expr] (buffer) ;
		for (auto &&i : iter (0 ,size_)) {
			ret = VAL (R1X (ret) ^ R1X (tmp[i])) ;
			ret = VAL (R1X (ret * FNV_PRIME::expr)) ;
		}
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_CONFIG_VAL64__
struct FUNCTION_fnvhash {
	using FNV_BASIS = ENUM<VAL (14695981039346656037)> ;
	using FNV_PRIME = ENUM<VAL (1099511628211)> ;

	forceinline VAL operator() () const {
		return FNV_BASIS::expr ;
	}

	forceinline VAL operator() (CREF<VAL> curr ,CREF<Pointer> buffer ,CREF<LENGTH> size_) const {
		using R1X = BYTE_BASE<VAL> ;
		VAL ret = curr ;
		auto &&tmp = keep[TYPE<ARR<BYTE>>::expr] (buffer) ;
		for (auto &&i : iter (0 ,size_)) {
			ret = VAL (R1X (ret) ^ R1X (tmp[i])) ;
			ret = VAL (R1X (ret * FNV_PRIME::expr)) ;
		}
		return move (ret) ;
	}
} ;
#endif

static constexpr auto fnvhash = FUNCTION_fnvhash () ;

class HashcodeVisitor implement Visitor {
protected:
	Optional<VAL> mCode ;
	Optional<LENGTH> mDepth ;

public:
	void initialize () override {
		mCode = 0 ;
		mDepth = 0 ;
	}

	void begin () const override {
		const auto r1x = mDepth.fetch (0) ;
		if ifdo (TRUE) {
			if (r1x != 0)
				discard ;
			mCode.store (fnvhash ()) ;
		}
		mDepth.store (r1x + 1) ;
	}

	void end () const override {
		const auto r1x = mDepth.fetch (0) ;
		mDepth.store (r1x - 1) ;
	}

	void push (CREF<BYTE> a) const override {
		const auto r1x = mCode.fetch (0) ;
		const auto r2x = fnvhash (r1x ,Pointer::from (a) ,SIZE_OF<BYTE>::expr) ;
		mCode.store (r2x) ;
	}

	void push (CREF<WORD> a) const override {
		const auto r1x = mCode.fetch (0) ;
		const auto r2x = fnvhash (r1x ,Pointer::from (a) ,SIZE_OF<WORD>::expr) ;
		mCode.store (r2x) ;
	}

	void push (CREF<CHAR> a) const override {
		const auto r1x = mCode.fetch (0) ;
		const auto r2x = fnvhash (r1x ,Pointer::from (a) ,SIZE_OF<CHAR>::expr) ;
		mCode.store (r2x) ;
	}

	void push (CREF<QUAD> a) const override {
		const auto r1x = mCode.fetch (0) ;
		const auto r2x = fnvhash (r1x ,Pointer::from (a) ,SIZE_OF<QUAD>::expr) ;
		mCode.store (r2x) ;
	}

	FLAG peek () const override {
		const auto r1x = mCode.fetch (0) ;
		const auto r2x = QUAD (r1x) & QUAD (VAL_MAX) ;
		return FLAG (r2x) ;
	}
} ;

class HashSetImplement implement Fat<HashSetHolder ,HashSetLayout> {
public:
	void initialize (CREF<Unknown> element) override {
		AllocatorHolder::create (fake.mSet)->initialize (element) ;
		fake.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		fake.mVisitor->initialize () ;
	}

	void initialize (CREF<Unknown> element ,CREF<LENGTH> size_) override {
		AllocatorHolder::create (fake.mSet)->initialize (element ,size_) ;
		fake.mVisitor = SharedRef<HashcodeVisitor>::make () ;
		fake.mVisitor->initialize () ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (element ,rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
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
		INDEX ix = find (BoxHolder::create (item)->self) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = AllocatorHolder::create (fake.mSet)->alloc (move (item)) ;
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
		INDEX ret = NONE ;
		if ifdo (TRUE) {
			if (fake.mRange.size () == 0)
				discard ;
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
					if ifnot (r3x)
						discard ;
					ret = ix ;
				}
				if (ret != NONE)
					break ;
				ix = fake.mSet.bt (ix).mDown ;
				if (ix == fake.mRange[jx])
					break ;
			}
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
			if ifnot (fake.mSet.used (i))
				continue ;
			update_emplace (i) ;
		}
	}
} ;

exports VFat<HashSetHolder> HashSetHolder::create (VREF<HashSetLayout> that) {
	return VFat<HashSetHolder> (HashSetImplement () ,Pointer::from (that)) ;
}

exports CFat<HashSetHolder> HashSetHolder::create (CREF<HashSetLayout> that) {
	return CFat<HashSetHolder> (HashSetImplement () ,Pointer::from (that)) ;
}

struct FUNCTION_popcount {
	forceinline LENGTH operator() (CREF<BYTE> bits) const {
		static const DEF<LENGTH[256]> mTable {
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
		return mTable[INDEX (bits)] ;
	}
} ;

static constexpr auto popcount = FUNCTION_popcount () ;

struct FUNCTION_lowcount {
	forceinline LENGTH operator() (CREF<BYTE> bits) const {
		static const DEF<LENGTH[256]> mTable {
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
		return mTable[INDEX (bits)] ;
	}
} ;

static constexpr auto lowcount = FUNCTION_lowcount () ;

class BitSetImplement implement Fat<BitSetHolder ,BitSetLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		const auto r1x = (size_ + 8 - 1) / 8 ;
		fake.mSet = RefBuffer<BYTE> (r1x) ;
		fake.mWidth = size_ ;
		clear () ;
	}

	void initialize (CREF<Unknown> element ,CREF<CaptureLayout> params ,VREF<BoxLayout> item) override {
		auto rax = buffer_from_initializer_list (params ,element) ;
		initialize (rax.size ()) ;
		const auto r1x = RFat<ReflectMove> (element) ;
		for (auto &&i : iter (0 ,rax.size ())) {
			BoxHolder::create (item)->initialize (element) ;
			r1x->copy (BoxHolder::create (item)->self ,rax[i]) ;
			r1x->init (rax[i]) ;
			add (move (item)) ;
		}
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
			ret += popcount (fake.mSet[i]) ;
		return move (ret) ;
	}

	void get (CREF<INDEX> index ,VREF<BOOL> item) const override {
		const auto r1x = BYTE (0X01) << (index % 8) ;
		const auto r2x = fake.mSet[index / 8] & r1x ;
		item = BOOL (r2x != BYTE (0X00)) ;
	}

	void set (CREF<INDEX> index ,CREF<BOOL> item) override {
		const auto r1x = BYTE (0X01) << (index % 8) ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (item)
				discard ;
			fake.mSet[index / 8] |= r1x ;
		}
		if ifdo (act) {
			if (item)
				discard ;
			fake.mSet[index / 8] &= ~r1x ;
		}
		if ifdo (act) {
			assert (FALSE) ;
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
			const auto r2x = BYTE (0X01) << r1x ;
			const auto r3x = fake.mSet[ix] & ~BYTE (INDEX (r2x) - 1) ;
			if (r3x == BYTE (0X00))
				discard ;
			const auto r4x = lowcount (r3x) ;
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
				const auto r1x = lowcount (fake.mSet[ix]) ;
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
			if ifnot (r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<BitSetLayout> that) const override {
		const auto r1x = fake.mSet.size () ;
		const auto r2x = that.mSet.size () ;
		const auto r3x = inline_min (r1x ,r2x) ;
		for (auto &&i : iter (0 ,r3x)) {
			const auto r4x = inline_compr (fake.mSet[i] ,that.mSet[i]) ;
			if (r4x != ZERO)
				return r4x ;
		}
		return inline_compr (r1x ,r2x) ;
	}

	void visit (CREF<Visitor> visitor) const override {
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
		if ifnot (inline_between (r1x ,0 ,size ()))
			return FALSE ;
		auto rax = FALSE ;
		get (r1x ,rax) ;
		return move (rax) ;
	}

	void erase (CREF<Pointer> item) override {
		const auto r1x = bitwise[TYPE<INDEX>::expr] (item) ;
		if ifnot (inline_between (r1x ,0 ,size ()))
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
		const auto r2x = LENGTH (WORD (0X01) << r1x) - 1 ;
		layout.mSet[ix] &= BYTE (r2x) ;
	}
} ;

exports VFat<BitSetHolder> BitSetHolder::create (VREF<BitSetLayout> that) {
	return VFat<BitSetHolder> (BitSetImplement () ,Pointer::from (that)) ;
}

exports CFat<BitSetHolder> BitSetHolder::create (CREF<BitSetLayout> that) {
	return CFat<BitSetHolder> (BitSetImplement () ,Pointer::from (that)) ;
}
} ;