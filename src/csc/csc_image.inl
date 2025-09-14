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
	void prepare (CR<Unknown> holder) override {
		if (self.mImage.exist ())
			return ;
		RefBufferHolder::hold (self.mImage)->prepare (holder) ;
		self.mWidth = 0 ;
		self.mStride = 0 ;
		self.mBX = 0 ;
		self.mBY = 0 ;
		self.mCX = 0 ;
		self.mCY = 0 ;
	}

	void initialize (RR<ImageLayout> that) override {
		const auto r1x = RFat<ReflectSize> (self.mImage.unknown ()) ;
		const auto r2x = r1x->type_size () ;
		noop (r2x) ;
		assume (r2x == that.mImage.step ()) ;
		self = move (that) ;
	}

	void initialize (CR<LENGTH> cx_ ,CR<LENGTH> cy_ ,CR<LENGTH> step_) override {
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		const auto r1x = RFat<ReflectSize> (self.mImage.unknown ()) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = cx_ * cy_ * step_ ;
		const auto r4x = inline_alignas (r3x ,r2x) / r2x ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mImage) ;
		RefBufferHolder::hold (rax)->initialize (r4x) ;
		rax.mSize = cx_ * cy_ ;
		rax.mStep = step_ ;
		self.mWidth = cx_ ;
		self.mStride = cx_ ;
		reset () ;
	}

	void initialize (CR<ImageLayout> that) override {
		const auto r1x = ImageHolder::hold (that)->shape () ;
		if (r1x.size () == 0)
			return ;
		initialize (r1x.mCX ,r1x.mCY ,r1x.mStep) ;
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

	void reset (CR<LENGTH> bx_ ,CR<LENGTH> by_ ,CR<LENGTH> cx_ ,CR<LENGTH> cy_) override {
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

	VR<BoxLayout> raw () leftvalue override {
		return RefBufferHolder::hold (self.mImage)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return RefBufferHolder::hold (self.mImage)->raw () ;
	}

	VR<Pointer> at (CR<INDEX> x ,CR<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + self.mBX) + (y + self.mBY) * self.mStride ;
		return self.mImage.at (ix) ;
	}

	CR<Pointer> at (CR<INDEX> x ,CR<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + self.mBX) + (y + self.mBY) * self.mStride ;
		return self.mImage.at (ix) ;
	}

	void fill (CR<Pointer> item) override {
		const auto r1x = step () ;
		if ifdo (TRUE) {
			if (cy () == 0)
				discard ;
			for (auto &&i : range (0 ,cx ())) {
				inline_memcpy (at (i ,0) ,item ,r1x) ;
			}
		}
		const auto r2x = cx () * r1x ;
		for (auto &&i : range (1 ,cy ())) {
			inline_memcpy (at (0 ,i) ,at (0 ,0) ,r2x) ;
		}
	}

	void splice (CR<INDEX> x ,CR<INDEX> y ,CR<ImageLayout> item) override {
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
		for (auto &&i : range (0 ,r2x)) {
			INDEX ix = x + 0 ;
			INDEX iy = y + i ;
			inline_memcpy (at (ix ,iy) ,ImageHolder::hold (item)->at (0 ,i) ,r4x) ;
		}
	}
} ;

exports VFat<ImageHolder> ImageHolder::hold (VR<ImageLayout> that) {
	return VFat<ImageHolder> (ImageImplHolder () ,that) ;
}

exports CFat<ImageHolder> ImageHolder::hold (CR<ImageLayout> that) {
	return CFat<ImageHolder> (ImageImplHolder () ,that) ;
}

template class External<ImageProcHolder ,ImageProcLayout> ;

struct ImageProcLayout {
	UniqueRef<BOOL> mContext ;
} ;

exports CR<OfThis<UniqueRef<ImageProcLayout>>> ImageProcHolder::expr_m () {
	return memorize ([&] () {
		OfThis<UniqueRef<ImageProcLayout>> ret ;
		ret.mThis = UniqueRef<ImageProcLayout>::make () ;
		ImageProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ImageProcHolder> ImageProcHolder::hold (VR<ImageProcLayout> that) {
	return VFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::expr ,that) ;
}

exports CFat<ImageProcHolder> ImageProcHolder::hold (CR<ImageProcLayout> that) {
	return CFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::expr ,that) ;
}

struct ReflectTensorCopy implement Interface {
	virtual void xcopy (VR<Pointer> dst ,CR<Pointer> src) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 501 ;
	}
} ;

template <class A ,class B>
class ReflectTensorCopyBinder final implement Fat<ReflectTensorCopy ,Proxy> {
public:
	void xcopy (VR<Pointer> dst ,CR<Pointer> src) const override {
		auto &&rax = keep[TYPE<A>::expr] (dst) ;
		auto &&rbx = keep[TYPE<B>::expr] (src) ;
		rax = A (rbx) ;
	}
} ;

class TensorImplHolder final implement Fat<TensorHolder ,TensorLayout> {
public:
	void initialize (CR<RefBufferLayout> that) override {
		auto &&rax = keep[TYPE<RefBuffer<Pointer>>::expr] (that) ;
		const auto r1x = tensor_type_from_step (rax.step ()) ;
		initialize (rax.size () ,r1x) ;
		const auto r2x = choose_tensor_copy (r1x ,type ()) ;
		const auto r3x = RFat<ReflectTensorCopy> (r2x) ;
		const auto r4x = self.mStride[self.mRank] ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r5x = self.mBuffer + i * r4x ;
			const auto r6x = address (rax[i]) ;
			r3x->xcopy (Pointer::make (r5x) ,Pointer::make (r6x)) ;
		}
	}

	void initialize (VR<FarBufferLayout> that) override {
		auto &&rax = keep[TYPE<FarBuffer<Pointer>>::expr] (that) ;
		const auto r1x = tensor_type_from_step (rax.step ()) ;
		initialize (rax.size () ,r1x) ;
		const auto r2x = choose_tensor_copy (r1x ,type ()) ;
		const auto r3x = RFat<ReflectTensorCopy> (r2x) ;
		const auto r4x = self.mStride[self.mRank] ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r5x = self.mBuffer + i * r4x ;
			const auto r6x = address (rax[i]) ;
			r3x->xcopy (Pointer::make (r5x) ,Pointer::make (r6x)) ;
		}
	}

	void initialize (CR<LENGTH> size_ ,CR<Just<TensorType>> type_) override {
		const auto r1x = step_from_tensor_type (type_) ;
		const auto r2x = size_ * r1x + 16 ;
		self.mTensor = Ref<RefBuffer<BYTE>>::make (r2x) ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mTensor.ref) ;
		rax.mBuffer = inline_alignas (rax.mBuffer ,16) ;
		rax.mSize = size_ ;
		rax.mStep = r1x ;
		self.mBuffer = rax.mBuffer ;
		self.mRank = 1 ;
		self.mStride[0] = size_ * r1x ;
		for (auto &&i : range (1 ,self.mStride.size ()))
			self.mStride[i] = r1x ;
	}

	LENGTH step_from_tensor_type (CR<Just<TensorType>> type_) const {
		if (type_ == TensorType::Flt16)
			return 2 ;
		if (type_ == TensorType::Flt32)
			return 4 ;
		if (type_ == TensorType::Flt64)
			return 8 ;
		if (type_ == TensorType::Flt128)
			return 16 ;
		return 0 ;
	}

	Just<TensorType> tensor_type_from_step (CR<LENGTH> step_) const {
		if (step_ == 2)
			return TensorType::Flt16 ;
		if (step_ == 4)
			return TensorType::Flt32 ;
		if (step_ == 8)
			return TensorType::Flt64 ;
		if (step_ == 16)
			return TensorType::Flt128 ;
		return 0 ;
	}

	LENGTH size () const override {
		if (self.mTensor == NULL)
			return 0 ;
		return self.mStride[0] / self.mStride[self.mRank] ;
	}

	Just<TensorType> type () const override {
		if (self.mTensor == NULL)
			return TensorType::ETC ;
		return tensor_type_from_step (self.mStride[self.mRank]) ;
	}

	LENGTH rank () const override {
		if (self.mTensor == NULL)
			return 0 ;
		return self.mRank ;
	}

	LENGTH shape (CR<INDEX> index) const override {
		assert (index >= 0) ;
		if (self.mTensor == NULL)
			return 0 ;
		const auto r1x = self.mRank ;
		INDEX ix = inline_min (index ,r1x) ;
		INDEX iy = inline_min (index + 1 ,r1x) ;
		return self.mStride[ix] / self.mStride[iy] ;
	}

	TensorLayout share () const {
		TensorLayout ret ;
		ret.mTensor = self.mTensor.share () ;
		ret.mBuffer = self.mBuffer ;
		ret.mRank = self.mRank ;
		ret.mStride = self.mStride ;
		return move (ret) ;
	}

	TensorLayout recast (CR<Just<TensorType>> type_) const override {
		if (type () == type_)
			return share () ;
		TensorLayout ret ;
		const auto r1x = size () ;
		TensorHolder::hold (ret)->initialize (r1x ,type_) ;
		const auto r2x = choose_tensor_copy (type_ ,type ()) ;
		const auto r3x = RFat<ReflectTensorCopy> (r2x) ;
		const auto r4x = self.mStride[self.mRank] ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r5x = ret.mBuffer + i * r4x ;
			const auto r6x = self.mBuffer + i * r4x ;
			r3x->xcopy (Pointer::make (r5x) ,Pointer::make (r6x)) ;
		}
		ret.mRank = self.mRank ;
		ret.mStride = self.mStride ;
		return move (ret) ;
	}

	Unknown choose_tensor_copy (CR<Just<TensorType>> dst ,CR<Just<TensorType>> src) const {
		if (dst == TensorType::Flt32)
			if (src == TensorType::Flt32)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT32 ,FLT32>> () ;
		if (dst == TensorType::Flt32)
			if (src == TensorType::Flt64)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT32 ,FLT64>> () ;
		if (dst == TensorType::Flt64)
			if (src == TensorType::Flt32)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT64 ,FLT32>> () ;
		if (dst == TensorType::Flt64)
			if (src == TensorType::Flt64)
				return SimpleUnknownBinder<ReflectTensorCopyBinder<FLT64 ,FLT64>> () ;
		assume (FALSE) ;
		return Unknown (ZERO) ;
	}

	TensorLayout reshape () const override {
		if (self.mTensor == NULL)
			return share () ;
		return reshape (MakeWrapper (size ())) ;
	}

	TensorLayout reshape (CR<WrapperLayout> shape_) const override {
		assert (self.mTensor != NULL) ;
		auto &&rax = keep[TYPE<Wrapper<LENGTH>>::expr] (shape_) ;
		assert (rax.rank () > 0) ;
		const auto r1x = self.mStride.size () - 1 ;
		assert (rax.rank () <= r1x) ;
		TensorLayout ret = share () ;
		const auto r2x = inline_min (rax.rank () ,r1x) ;
		ret.mRank = r2x ;
		ret.mStride[r1x] = self.mStride[self.mRank] ;
		for (auto &&i : range (0 ,r1x - r2x)) {
			INDEX ix = r1x - i ;
			ret.mStride[ix - 1] = ret.mStride[ix] ;
		}
		for (auto &&i : range (r1x - r2x ,r1x)) {
			INDEX ix = r1x - i ;
			ret.mStride[ix - 1] = ret.mStride[ix] * rax[ix - 1] ;
		}
		assume (TensorHolder::hold (ret)->size () == size ()) ;
		return move (ret) ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue override {
		return self.mTensor.share () ;
	}

	FltProxy at (CR<INDEX> i1) const override {
		const auto r1x = i1 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[0])) ;
		const auto r2x = self.mBuffer + r1x ;
		return FltProxy (r2x ,self.mStride[self.mRank]) ;
	}

	FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2) const override {
		const auto r1x = i1 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[0])) ;
		const auto r2x = i2 * self.mStride[2] ;
		assert (inline_between (r1x ,0 ,self.mStride[1])) ;
		const auto r3x = self.mBuffer + r1x + r2x ;
		return FltProxy (r3x ,self.mStride[self.mRank]) ;
	}

	FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2 ,CR<INDEX> i3) const override {
		const auto r1x = i1 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[0])) ;
		const auto r2x = i2 * self.mStride[2] ;
		assert (inline_between (r1x ,0 ,self.mStride[1])) ;
		const auto r3x = i3 * self.mStride[3] ;
		assert (inline_between (r1x ,0 ,self.mStride[2])) ;
		const auto r4x = self.mBuffer + r1x + r2x + r3x ;
		return FltProxy (r4x ,self.mStride[self.mRank]) ;
	}

	FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2 ,CR<INDEX> i3 ,CR<INDEX> i4) const override {
		const auto r1x = i1 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[0])) ;
		const auto r2x = i2 * self.mStride[2] ;
		assert (inline_between (r1x ,0 ,self.mStride[1])) ;
		const auto r3x = i3 * self.mStride[3] ;
		assert (inline_between (r1x ,0 ,self.mStride[2])) ;
		const auto r4x = i4 * self.mStride[4] ;
		assert (inline_between (r1x ,0 ,self.mStride[3])) ;
		const auto r5x = self.mBuffer + r1x + r2x + r3x + r4x ;
		return FltProxy (r5x ,self.mStride[self.mRank]) ;
	}
} ;

exports VFat<TensorHolder> TensorHolder::hold (VR<TensorLayout> that) {
	return VFat<TensorHolder> (TensorImplHolder () ,that) ;
}

exports CFat<TensorHolder> TensorHolder::hold (CR<TensorLayout> that) {
	return CFat<TensorHolder> (TensorImplHolder () ,that) ;
}

class DisjointImplHolder final implement Fat<DisjointHolder ,DisjointLayout> {
public:
	void initialize (CR<LENGTH> size_) override {
		self.mTable = Array<INDEX> (size_) ;
		self.mTable.fill (NONE) ;
	}

	LENGTH size () const override {
		return self.mTable.size () ;
	}

	INDEX lead (CR<INDEX> from_) override {
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

	INDEX parent (CR<INDEX> curr) const {
		if (curr == self.mTable[curr])
			return NONE ;
		return self.mTable[curr] ;
	}

	void joint (CR<INDEX> from_ ,CR<INDEX> to_) override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		self.mTable[ix] = ix ;
		self.mTable[iy] = ix ;
	}

	BOOL edge (CR<INDEX> from_ ,CR<INDEX> to_) override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		return ix == iy ;
	}

	LENGTH depth (CR<INDEX> from_) override {
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

	Deque<INDEX> cluster (CR<INDEX> from_) override {
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

	Array<INDEX> jump (CR<INDEX> from_) override {
		Array<INDEX> ret = Array<INDEX> (self.mTable.size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : range (0 ,self.mTable.size ())) {
			INDEX ix = lead (i) ;
			if (ix == NONE)
				continue ;
			ret[ix] = ret[i] ;
			ret[i] = ix ;
		}
		return move (ret) ;
	}
} ;

exports VFat<DisjointHolder> DisjointHolder::hold (VR<DisjointLayout> that) {
	return VFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

exports CFat<DisjointHolder> DisjointHolder::hold (CR<DisjointLayout> that) {
	return CFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

class KMMatchImplHolder final implement Fat<KMMatchHolder ,KMMatchLayout> {
public:
	void initialize (CR<LENGTH> size_) override {
		self.mSize = size_ ;
		self.mThreshold = FLT32 (0.1) ;
		self.mUser = Array<FLT32> (self.mSize) ;
		self.mWork = Array<FLT32> (self.mSize) ;
		self.mUserVisit = BitSet (self.mSize) ;
		self.mWorkVisit = BitSet (self.mSize) ;
		self.mMatch = Array<INDEX> (self.mSize) ;
		self.mLack = Array<FLT32> (self.mSize) ;
	}

	void set_threshold (CR<FLT64> threshold) override {
		self.mThreshold = FLT32 (threshold) ;
	}

	LENGTH size () const override {
		return self.mSize ;
	}

	Array<INDEX> sort (CR<Image<FLT32>> love) override {
		assert (self.mMatch.size () > 0) ;
		assert (love.size () == MathProc::square (self.mSize)) ;
		self.mLove = Ref<Image<FLT32>>::reference (love) ;
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
		for (auto &&i : range (0 ,self.mSize)) {
			self.mUser[i] = -infinity ;
			for (auto &&j : range (0 ,self.mSize)) {
				const auto r1x = self.mLove.ref[i][j] ;
				self.mUser[i] = MathProc::max_of (self.mUser[i] ,r1x) ;
			}
		}
		for (auto &&i : range (0 ,self.mSize)) {
			self.mLack.fill (infinity) ;
			while (TRUE) {
				self.mUserVisit.clear () ;
				self.mWorkVisit.clear () ;
				if (dfs (i))
					break ;
				const auto r2x = invoke ([&] () {
					FLT32 ret = infinity ;
					for (auto &&j : range (0 ,self.mSize)) {
						if (self.mWorkVisit[j])
							continue ;
						ret = MathProc::min_of (ret ,self.mLack[j]) ;
					}
					return move (ret) ;
				}) ;
				for (auto &&j : range (0 ,self.mSize)) {
					if ifdo (TRUE) {
						if (!self.mUserVisit[j])
							discard ;
						self.mUser[j] -= r2x ;
					}
					if ifdo (TRUE) {
						if (!self.mWorkVisit[j])
							discard ;
						self.mWork[j] += r2x ;
					}
					if ifdo (TRUE) {
						if (self.mWorkVisit[j])
							discard ;
						self.mLack[j] -= r2x ;
					}
				}
			}
		}
	}

	BOOL dfs (CR<INDEX> user) {
		self.mUserVisit[user] = TRUE ;
		for (auto &&i : range (0 ,self.mSize)) {
			if (self.mWorkVisit[i])
				continue ;
			const auto r1x = self.mLove.ref[user][i] ;
			const auto r2x = self.mUser[user] + self.mWork[i] - r1x ;
			if ifdo (TRUE) {
				if (r2x < self.mThreshold)
					discard ;
				self.mLack[i] = MathProc::min_of (self.mLack[i] ,r2x) ;
			}
			if (r2x >= self.mThreshold)
				continue ;
			self.mWorkVisit[i] = TRUE ;
			const auto r3x = self.mMatch[i] ;
			if ifdo (TRUE) {
				if (r3x != NONE)
					discard ;
				self.mMatch[i] = user ;
				return TRUE ;
			}
			if ifdo (TRUE) {
				if (!dfs (r3x))
					discard ;
				self.mMatch[i] = user ;
				return TRUE ;
			}
		}
		return FALSE ;
	}
} ;

exports VFat<KMMatchHolder> KMMatchHolder::hold (VR<KMMatchLayout> that) {
	return VFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}

exports CFat<KMMatchHolder> KMMatchHolder::hold (CR<KMMatchLayout> that) {
	return CFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}
} ;
