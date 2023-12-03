#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

#include "csc_end.h"
#include <atomic>
#include "csc_begin.h"

namespace CSC {
class BoxImplHolder implement BoxHolder {
protected:
	XPTR<BoxLayout> mThis ;

public:
	void initialize (CREF<Unknown> that) override {
		mThis->mHolder = Pointer::from (that) ;
	}

	void destroy () override {
		fake.destroy (1) ;
	}

	BOOL exist () const override {
		return mThis->mHolder != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.type_align () ;
		const auto r2x = operator_alignas (address (mThis->mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		const auto r1x = fake.type_align () ;
		const auto r2x = operator_alignas (address (mThis->mHolder) + SIZE_OF<FLAG>::expr ,r1x) ;
		return Pointer::make (r2x) ;
	}

	void acquire (CREF<BoxLayout> that) override {
		mThis->mHolder = that.mHolder ;
		const auto r1x = fake.type_size () ;
		const auto r2x = fake.type_align () ;
		const auto r3x = operator_alignas (address (mThis->mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		const auto r4x = operator_alignas (address (that.mHolder) + SIZE_OF<FLAG>::expr ,r2x) ;
		memcpy (csc_pointer_t (r3x) ,csc_pointer_t (r4x) ,r1x) ;
	}

	void release () override {
		mThis->mHolder = ZERO ;
	}

	VREF<Unknown> fake_m () const leftvalue {
		return Pointer::from (mThis->mHolder) ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return VFat<BoxHolder> (BoxImplHolder () ,that) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return CFat<BoxHolder> (BoxImplHolder () ,that) ;
}

struct HeapProcData {
	mutable std::atomic<LENGTH> mValue ;
} ;

class HeapProcImplHolder implement HeapProcHolder {
protected:
	XPTR<HeapProcLayout> mThis ;

public:
	void initialize () override {
		static HeapProcData mInstance ;
		mThis->mPointer = address (mInstance) ;
	}

	LENGTH length () const override {
		return fake.mValue ;
	}

	FLAG alloc (CREF<LENGTH> size_) const override {
		FLAG ret = FLAG (operator new (size_)) ;
		const auto r1x = csc_pointer_t (ret) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake.mValue += r2x ;
		return move (ret) ;
	}

	void free (CREF<FLAG> addr) const override {
		const auto r1x = csc_pointer_t (addr) ;
		const auto r2x = LENGTH (_msize (r1x)) ;
		fake.mValue -= r2x ;
		operator delete (r1x) ;
	}

	VREF<HeapProcData> fake_m () const leftvalue {
		return Pointer::make (mThis->mPointer) ;
	}
} ;

exports VFat<HeapProcHolder> HeapProcHolder::create (VREF<HeapProcLayout> that) {
	return VFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

exports CFat<HeapProcHolder> HeapProcHolder::create (CREF<HeapProcLayout> that) {
	return CFat<HeapProcHolder> (HeapProcImplHolder () ,that) ;
}

struct RefUnknownBinderLayout {
	LENGTH mCounter ;
	LENGTH mSize ;
	FLAG mHolder ;
} ;

class RefImplHolder implement RefHolder {
protected:
	XPTR<RefLayout> mThis ;

public:
	void initialize (CREF<BoxLayout> value ,CREF<LENGTH> size_) override {
		auto &&tmp1 = reinterpret_cast<CREF<Unknown>> (value.mHolder) ;
		const auto r1x = operator_max (tmp1.type_align () - ALIGN_OF<FLAG>::expr ,0) ;
		const auto r2x = SIZE_OF<RefUnknownBinderLayout>::expr + r1x + tmp1.type_size () ;
		mThis->mHolder = HeapProc::instance ().alloc (r2x) ;
		fake.mCounter = 0 ;
		fake.mSize = size_ ;
		auto &&tmp2 = reinterpret_cast<VREF<BoxLayout>> (fake.mHolder) ;
		BoxHolder::create (tmp2)->acquire (value) ;
		const auto r3x = mThis->mHolder + SIZE_OF<RefUnknownBinderLayout>::expr ;
		mThis->mPointer = operator_alignas (r3x ,tmp1.type_align ()) ;
	}

	void destroy () override {
		const auto r1x = --fake.mCounter ;
		if (r1x > 0)
			return ;
		auto &&tmp1 = reinterpret_cast<VREF<Unknown>> (fake.mHolder) ;
		tmp1.destroy (fake.mSize) ;
	}

	RefLayout clone () const override {
		RefLayout ret ;
		ret.mHolder = mThis->mHolder ;
		ret.mPointer = mThis->mPointer ;
		fake.mCounter++ ;
		return move (ret) ;
	}

	BOOL exist () const override {
		return mThis->mPointer != ZERO ;
	}

	VREF<Pointer> self_m () leftvalue override {
		assert (exist ()) ;
		return Pointer::make (mThis->mPointer) ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		assert (exist ()) ;
		return Pointer::make (mThis->mPointer) ;
	}

	VREF<RefUnknownBinderLayout> fake_m () const leftvalue {
		return Pointer::make (mThis->mHolder) ;
	}
} ;

exports VFat<RefHolder> RefHolder::create (VREF<RefLayout> that) {
	return VFat<RefHolder> (RefImplHolder () ,that) ;
}

exports CFat<RefHolder> RefHolder::create (CREF<RefLayout> that) {
	return CFat<RefHolder> (RefImplHolder () ,that) ;
}
} ;