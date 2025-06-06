#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

#include "csc_end.h"
#include <complex>
#include "csc_begin.h"

namespace CSC {
class ImageImplHolder final implement Fat<ImageHolder ,ImageLayout> {
public:
	void initialize (CREF<Unknown> holder ,RREF<ImageLayout> that) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		noop (r2x) ;
		assume (r2x == that.mImage.step ()) ;
		self = move (that) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> step_) override {
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = cx_ * cy_ * step_ ;
		const auto r4x = inline_alignas (r3x ,r2x) / r2x ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mImage) ;
		RefBufferHolder::hold (rax)->initialize (holder ,r4x) ;
		rax.mSize = cx_ * cy_ ;
		rax.mStep = step_ ;
		self.mWidth = cx_ ;
		self.mStride = cx_ ;
		reset () ;
	}

	void initialize (CREF<ImageLayout> that) override {
		const auto r1x = ImageHolder::hold (that)->shape () ;
		if (r1x.size () == 0)
			return ;
		initialize (that.mImage.unknown () ,r1x.mCX ,r1x.mCY ,r1x.mStep) ;
		splice (0 ,0 ,that) ;
	}

	BOOL fixed () const override {
		return self.mImage.fixed () ;
	}

	LENGTH size () const override {
		return self.mImage.size () ;
	}

	LENGTH step () const override {
		return self.mImage.step () ;
	}

	LENGTH stride () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mStride ;
	}

	BOOL continous () const override {
		if (bx () != 0)
			return FALSE ;
		if (by () != 0)
			return FALSE ;
		if (cx () != stride ())
			return FALSE ;
		return TRUE ;
	}

	LENGTH bx () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mBX ;
	}

	LENGTH by () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mBY ;
	}

	LENGTH cx () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mCX ;
	}

	LENGTH cy () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mCY ;
	}

	ImageShape shape () const override {
		ImageShape ret ;
		ret.mBX = bx () ;
		ret.mBY = by () ;
		ret.mCX = cx () ;
		ret.mCY = cy () ;
		ret.mStep = step () ;
		return move (ret) ;
	}

	void reset () override {
		self.mBX = 0 ;
		self.mBY = 0 ;
		self.mCX = 0 ;
		self.mCY = 0 ;
		if (self.mImage.size () == 0)
			return ;
		const auto r1x = inline_max (self.mStride ,1) ;
		const auto r2x = self.mImage.size () / r1x ;
		reset (0 ,0 ,self.mWidth ,r2x) ;
	}

	void reset (CREF<LENGTH> bx_ ,CREF<LENGTH> by_ ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) override {
		assert (self.mImage.size () > 0) ;
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		const auto r1x = bx_ + cx_ - 1 ;
		const auto r2x = by_ + cy_ - 1 ;
		assert (r1x < self.mStride) ;
		const auto r3x = r1x + r2x * self.mStride ;
		noop (r3x) ;
		assert (r3x < self.mImage.size ()) ;
		self.mBX = bx_ ;
		self.mBY = by_ ;
		self.mCX = cx_ ;
		self.mCY = cy_ ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return RefBufferHolder::hold (self.mImage)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return RefBufferHolder::hold (self.mImage)->raw () ;
	}

	VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + self.mBX) + (y + self.mBY) * self.mStride ;
		return self.mImage.at (ix) ;
	}

	CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + self.mBX) + (y + self.mBY) * self.mStride ;
		return self.mImage.at (ix) ;
	}

	void fill (CREF<Pointer> item) override {
		const auto r1x = step () ;
		if ifdo (TRUE) {
			if (cy () == 0)
				discard ;
			for (auto &&i : iter (0 ,cx ())) {
				inline_memcpy (at (i ,0) ,item ,r1x) ;
			}
		}
		const auto r2x = cx () * r1x ;
		for (auto &&i : iter (1 ,cy ())) {
			inline_memcpy (at (0 ,i) ,at (0 ,0) ,r2x) ;
		}
	}

	void splice (CREF<INDEX> x ,CREF<INDEX> y ,CREF<ImageLayout> item) override {
		const auto r1x = ImageHolder::hold (item)->cx () ;
		const auto r2x = ImageHolder::hold (item)->cy () ;
		if (r1x == 0)
			return ;
		if (r2x == 0)
			return ;
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		assert (x + r1x <= cx ()) ;
		assert (y + r2x <= cy ()) ;
		const auto r3x = ImageHolder::hold (item)->step () ;
		assert (step () == r3x) ;
		const auto r4x = r1x * r3x ;
		for (auto &&i : iter (0 ,r2x)) {
			INDEX ix = x + 0 ;
			INDEX iy = y + i ;
			inline_memcpy (at (ix ,iy) ,ImageHolder::hold (item)->at (0 ,i) ,r4x) ;
		}
	}
} ;

exports VFat<ImageHolder> ImageHolder::hold (VREF<ImageLayout> that) {
	return VFat<ImageHolder> (ImageImplHolder () ,that) ;
}

exports CFat<ImageHolder> ImageHolder::hold (CREF<ImageLayout> that) {
	return CFat<ImageHolder> (ImageImplHolder () ,that) ;
}

template class External<ImageProcHolder ,ImageProcLayout> ;

struct ImageProcLayout {
	UniqueRef<BOOL> mContext ;
} ;

exports CREF<OfThis<UniqueRef<ImageProcLayout>>> ImageProcHolder::expr_m () {
	return memorize ([&] () {
		OfThis<UniqueRef<ImageProcLayout>> ret ;
		ret.mThis = UniqueRef<ImageProcLayout>::make () ;
		ImageProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ImageProcHolder> ImageProcHolder::hold (VREF<ImageProcLayout> that) {
	return VFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::expr ,that) ;
}

exports CFat<ImageProcHolder> ImageProcHolder::hold (CREF<ImageProcLayout> that) {
	return CFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::expr ,that) ;
}

struct ReflectTensorCopy implement Interface {
	virtual void xcopy (VREF<Pointer> dst ,CREF<Pointer> src) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 501 ;
	}
} ;

template <class A ,class B>
class ReflectTensorCopyBinder final implement Fat<ReflectTensorCopy ,Proxy> {
public:
	void xcopy (VREF<Pointer> dst ,CREF<Pointer> src) const override {
		auto &&rax = keep[TYPE<A>::expr] (dst) ;
		auto &&rbx = keep[TYPE<B>::expr] (src) ;
		rax = A (rbx) ;
	}
} ;

class TensorImplHolder final implement Fat<TensorHolder ,TensorLayout> {
public:
	void initialize (CREF<LENGTH> size_ ,CREF<Just<TensorDataType>> type_) override {
		const auto r1x = size_of_tensor_type (type_) ;
		const auto r2x = size_ * r1x + 16 ;
		self.mTensor = RefBuffer<BYTE> (r2x) ;
		const auto r4x = address (self.mTensor[0]) ;
		self.mOffset = inline_alignas (r4x ,16) - r4x ;
		self.mWidth = size_ ;
		self.mType = type_ ;
		reset () ;
	}

	LENGTH size_of_tensor_type (CREF<Just<TensorDataType>> type_) const {
		if (type_ == TensorDataType::Flt16)
			return 2 ;
		if (type_ == TensorDataType::Flt32)
			return 4 ;
		if (type_ == TensorDataType::Flt64)
			return 8 ;
		if (type_ == TensorDataType::Flt128)
			return 16 ;
		return 0 ;
	}

	LENGTH size () const override {
		if (!self.mTensor.exist ())
			return 0 ;
		return self.mWidth ;
	}

	Just<TensorDataType> type () const override {
		if (!self.mTensor.exist ())
			return TensorDataType::ETC ;
		return self.mType ;
	}

	LENGTH cx () const override {
		if (!self.mTensor.exist ())
			return 0 ;
		return self.mShape[1] / self.mShape[0] ;
	}

	LENGTH cy () const override {
		if (!self.mTensor.exist ())
			return 0 ;
		return self.mShape[2] / self.mShape[1] ;
	}

	LENGTH cz () const override {
		if (!self.mTensor.exist ())
			return 0 ;
		return self.mShape[3] / self.mShape[2] ;
	}

	LENGTH cw () const override {
		if (!self.mTensor.exist ())
			return 0 ;
		return self.mShape[4] / self.mShape[3] ;
	}

	TensorLayout recast (CREF<Just<TensorDataType>> type_) override {
		if (type () == type_)
			return move (self) ;
		TensorLayout ret ;
		const auto r1x = size () ;
		TensorHolder::hold (ret)->initialize (r1x ,type_) ;
		const auto r2x = choose_tensor_copy (type_ ,type ()) ;
		const auto r3x = RFat<ReflectTensorCopy> (r2x) ;
		const auto r4x = address (ret.mTensor[ret.mOffset]) ;
		const auto r5x = address (self.mTensor[self.mOffset]) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r6x = r4x + i * ret.mShape[0] ;
			const auto r7x = r5x + i * self.mShape[0] ;
			r3x->xcopy (Pointer::make (r6x) ,Pointer::make (r7x)) ;
		}
		return move (ret) ;
	}

	Unknown choose_tensor_copy (CREF<Just<TensorDataType>> dst ,CREF<Just<TensorDataType>> src) const {
		if (dst == TensorDataType::Flt32)
			if (src == TensorDataType::Flt32)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT32 ,FLT32>> () ;
		if (dst == TensorDataType::Flt32)
			if (src == TensorDataType::Flt64)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT32 ,FLT64>> () ;
		if (dst == TensorDataType::Flt64)
			if (src == TensorDataType::Flt32)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT64 ,FLT32>> () ;
		if (dst == TensorDataType::Flt64)
			if (src == TensorDataType::Flt64)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT64 ,FLT64>> () ;
		assume (FALSE) ;
		return Unknown (ZERO) ;
	}

	void reset () override {
		for (auto &&i : iter (0 ,self.mShape.size ()))
			self.mShape[i] = 0 ;
		if (self.mTensor.size () == 0)
			return ;
		reset (size () ,1 ,1 ,1) ;
	}

	void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cz_ ,CREF<LENGTH> cw_) override {
		assert (self.mTensor.size () > 0) ;
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		assert (cz_ > 0) ;
		assert (cw_ > 0) ;
		const auto r1x = cx_ * cy_ * cz_ * cw_ ;
		noop (r1x) ;
		assert (r1x == size ()) ;
		self.mShape[0] = size_of_tensor_type (type ()) ;
		self.mShape[1] = self.mShape[0] * cx_ ;
		self.mShape[2] = self.mShape[1] * cy_ ;
		self.mShape[3] = self.mShape[2] * cz_ ;
		self.mShape[4] = self.mShape[3] * cw_ ;
	}

	VREF<Pointer> ref_m () leftvalue override {
		return RefBufferHolder::hold (self.mTensor)->ref ;
	}

	CREF<Pointer> ref_m () const leftvalue override {
		return RefBufferHolder::hold (self.mTensor)->ref ;
	}

	Ref<RefBuffer<BYTE>> borrow () leftvalue override {
		return Ref<RefBuffer<BYTE>>::reference (self.mTensor) ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue override {
		return Ref<RefBuffer<BYTE>>::reference (self.mTensor) ;
	}
} ;

exports VFat<TensorHolder> TensorHolder::hold (VREF<TensorLayout> that) {
	return VFat<TensorHolder> (TensorImplHolder () ,that) ;
}

exports CFat<TensorHolder> TensorHolder::hold (CREF<TensorLayout> that) {
	return CFat<TensorHolder> (TensorImplHolder () ,that) ;
}

class DisjointImplHolder final implement Fat<DisjointHolder ,DisjointLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		self.mTable = Array<INDEX> (size_) ;
		self.mTable.fill (NONE) ;
	}

	LENGTH size () const override {
		return self.mTable.size () ;
	}

	INDEX lead (CREF<INDEX> from_) override {
		INDEX ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ix = parent (ix) ;
		}
		INDEX ret = ix ;
		if ifdo (TRUE) {
			if (ix == NONE)
				discard ;
			ix = from_ ;
			INDEX iy = NONE ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				iy = parent (ix) ;
				self.mTable[ix] = ret ;
				ix = iy ;
			}
		}
		return move (ret) ;
	}

	INDEX parent (CREF<INDEX> curr) const {
		if (curr == self.mTable[curr])
			return NONE ;
		return self.mTable[curr] ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		self.mTable[ix] = ix ;
		self.mTable[iy] = ix ;
	}

	BOOL edge (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		return ix == iy ;
	}

	LENGTH depth (CREF<INDEX> from_) override {
		LENGTH ret = 0 ;
		INDEX ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ret++ ;
			ix = parent (ix) ;
		}
		return move (ret) ;
	}

	Deque<INDEX> cluster (CREF<INDEX> from_) override {
		Deque<INDEX> ret ;
		INDEX ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ret.add (ix) ;
			ix = parent (ix) ;
		}
		return move (ret) ;
	}

	Array<INDEX> jump (CREF<INDEX> from_) override {
		Array<INDEX> ret = Array<INDEX> (self.mTable.size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : iter (0 ,self.mTable.size ())) {
			INDEX ix = lead (i) ;
			if (ix == NONE)
				continue ;
			ret[ix] = ret[i] ;
			ret[i] = ix ;
		}
		return move (ret) ;
	}
} ;

exports VFat<DisjointHolder> DisjointHolder::hold (VREF<DisjointLayout> that) {
	return VFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

exports CFat<DisjointHolder> DisjointHolder::hold (CREF<DisjointLayout> that) {
	return CFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

class KMMatchImplHolder final implement Fat<KMMatchHolder ,KMMatchLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		self.mSize = size_ ;
		self.mThreshold = FLT32 (0.1) ;
		self.mUser = Array<FLT32> (self.mSize) ;
		self.mWork = Array<FLT32> (self.mSize) ;
		self.mUserVisit = BitSet (self.mSize) ;
		self.mWorkVisit = BitSet (self.mSize) ;
		self.mMatch = Array<INDEX> (self.mSize) ;
		self.mLack = Array<FLT32> (self.mSize) ;
	}

	void set_threshold (CREF<FLT64> threshold) override {
		self.mThreshold = FLT32 (threshold) ;
	}

	LENGTH size () const override {
		return self.mSize ;
	}

	Array<INDEX> sort (RREF<Array<FLT32>> love) override {
		assert (self.mMatch.size () > 0) ;
		assert (love.size () == MathProc::square (self.mSize)) ;
		self.mLove = move (love) ;
		self.mUser.fill (0) ;
		self.mWork.fill (0) ;
		self.mUserVisit.clear () ;
		self.mWorkVisit.clear () ;
		self.mMatch.fill (NONE) ;
		self.mLack.fill (0) ;
		solve () ;
		return self.mMatch ;
	}

	void solve () {
		for (auto &&i : iter (0 ,self.mSize)) {
			self.mUser[i] = -infinity ;
			for (auto &&j : iter (0 ,self.mSize)) {
				self.mUser[i] = MathProc::max_of (self.mUser[i] ,self.mLove[i * self.mSize + j]) ;
			}
		}
		for (auto &&i : iter (0 ,self.mSize)) {
			self.mLack.fill (infinity) ;
			while (TRUE) {
				self.mUserVisit.clear () ;
				self.mWorkVisit.clear () ;
				if (dfs (i))
					break ;
				const auto r1x = invoke ([&] () {
					FLT32 ret = infinity ;
					for (auto &&j : iter (0 ,self.mSize)) {
						if (self.mWorkVisit[j])
							continue ;
						ret = MathProc::min_of (ret ,self.mLack[j]) ;
					}
					return move (ret) ;
				}) ;
				for (auto &&j : iter (0 ,self.mSize)) {
					if ifdo (TRUE) {
						if (!self.mUserVisit[j])
							discard ;
						self.mUser[j] -= r1x ;
					}
					if ifdo (TRUE) {
						if (!self.mWorkVisit[j])
							discard ;
						self.mWork[j] += r1x ;
					}
					if ifdo (TRUE) {
						if (self.mWorkVisit[j])
							discard ;
						self.mLack[j] -= r1x ;
					}
				}
			}
		}
	}

	BOOL dfs (CREF<INDEX> user) {
		self.mUserVisit[user] = TRUE ;
		for (auto &&i : iter (0 ,self.mSize)) {
			if (self.mWorkVisit[i])
				continue ;
			const auto r1x = self.mUser[user] + self.mWork[i] - self.mLove[user * self.mSize + i] ;
			if ifdo (TRUE) {
				if (r1x < self.mThreshold)
					discard ;
				self.mLack[i] = MathProc::min_of (self.mLack[i] ,r1x) ;
			}
			if (r1x >= self.mThreshold)
				continue ;
			self.mWorkVisit[i] = TRUE ;
			const auto r2x = self.mMatch[i] ;
			if ifdo (TRUE) {
				if (r2x != NONE)
					discard ;
				self.mMatch[i] = user ;
				return TRUE ;
			}
			if ifdo (TRUE) {
				if (!dfs (r2x))
					discard ;
				self.mMatch[i] = user ;
				return TRUE ;
			}
		}
		return FALSE ;
	}
} ;

exports VFat<KMMatchHolder> KMMatchHolder::hold (VREF<KMMatchLayout> that) {
	return VFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}

exports CFat<KMMatchHolder> KMMatchHolder::hold (CREF<KMMatchLayout> that) {
	return CFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}
} ;
