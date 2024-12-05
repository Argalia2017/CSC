#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

namespace CSC {
class ImageImplHolder final implement Fat<ImageHolder ,ImageLayout> {
public:
	void initialize (CREF<Unknown> holder ,RREF<ImageLayout> that) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		noop (r2x) ;
		assume (r2x == that.mWidth.mStep) ;
		assume (r2x == that.mImage.step ()) ;
		fake = move (that) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> step_) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = cx_ * cy_ * step_ ;
		const auto r4x = inline_alignas (r3x ,r2x) / r2x ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (fake.mImage) ;
		RefBufferHolder::hold (rax)->initialize (holder ,r4x) ;
		rax.mSize = cx_ * cy_ ;
		rax.mStep = step_ ;
		fake.mWidth.mCX = cx_ ;
		fake.mWidth.mCY = cy_ ;
		fake.mWidth.mStep = step_ ;
		reset () ;
	}

	void initialize (CREF<ImageLayout> that) override {
		const auto r1x = ImageHolder::hold (that)->width () ;
		if (r1x.size () == 0)
			return ;
		initialize (that.mImage.unknown () ,r1x.mCX ,r1x.mCY ,r1x.mStep) ;
		splice (0 ,0 ,that) ;
	}

	BOOL exist () const override {
		return fake.mImage.exist () ;
	}

	BOOL fixed () const override {
		return fake.mImage.fixed () ;
	}

	LENGTH size () const override {
		return fake.mImage.size () ;
	}

	LENGTH step () const override {
		return fake.mImage.step () ;
	}

	LENGTH bx () const override {
		if (!fake.mImage.exist ())
			return 0 ;
		return fake.mBX ;
	}

	LENGTH by () const override {
		if (!fake.mImage.exist ())
			return 0 ;
		return fake.mBY ;
	}

	LENGTH cx () const override {
		if (!fake.mImage.exist ())
			return 0 ;
		return fake.mCX ;
	}

	LENGTH cy () const override {
		if (!fake.mImage.exist ())
			return 0 ;
		return fake.mCY ;
	}

	ImageWidth width () const override {
		ImageWidth ret ;
		ret.mCX = cx () ;
		ret.mCY = cy () ;
		ret.mStep = step () ;
		return move (ret) ;
	}

	void reset () override {
		fake.mBX = 0 ;
		fake.mBY = 0 ;
		fake.mCX = fake.mWidth.mCX ;
		fake.mCY = fake.mWidth.mCY ;
	}

	void reset (CREF<INDEX> bx_ ,CREF<INDEX> by_ ,CREF<INDEX> cx_ ,CREF<INDEX> cy_) override {
		const auto r1x = bx_ + cx_ - 1 ;
		const auto r2x = by_ + cy_ - 1 ;
		assert (r1x < fake.mWidth.mCX) ;
		assert (r2x < fake.mWidth.mCY) ;
		const auto r3x = r1x + r2x * fake.mWidth.mCX ;
		noop (r3x) ;
		assert (r3x < fake.mImage.size ()) ;
		fake.mBX = bx_ ;
		fake.mBY = by_ ;
		fake.mCX = cx_ ;
		fake.mCY = cy_ ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return RefBufferHolder::hold (fake.mImage)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return RefBufferHolder::hold (fake.mImage)->raw () ;
	}

	VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + fake.mBX) + (y + fake.mBY) * fake.mWidth.mCX ;
		return fake.mImage.at (ix) ;
	}

	CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + fake.mBX) + (y + fake.mBY) * fake.mWidth.mCX ;
		return fake.mImage.at (ix) ;
	}

	void fill (CREF<Pointer> item) override {
		if ifdo (TRUE) {
			if (cy () == 0)
				discard ;
			for (auto &&i : iter (0 ,cx ())) {
				inline_memcpy (at (i ,0) ,item ,fake.mWidth.mStep) ;
			}
		}
		const auto r1x = cx () * fake.mWidth.mStep ;
		for (auto &&i : iter (1 ,cy ())) {
			inline_memcpy (at (0 ,i) ,at (0 ,0) ,r1x) ;
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
		assert (fake.mWidth.mStep == item.mWidth.mStep) ;
		const auto r3x = r1x * fake.mWidth.mStep ;
		for (auto &&i : iter (0 ,r2x)) {
			INDEX ix = x + 0 ;
			INDEX iy = y + i ;
			inline_memcpy (at (ix ,iy) ,ImageHolder::hold (item)->at (0 ,i) ,r3x) ;
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

exports CREF<ImageProcLayout> ImageProcHolder::instance () {
	return memorize ([&] () {
		ImageProcLayout ret ;
		ImageProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ImageProcHolder> ImageProcHolder::hold (VREF<ImageProcLayout> that) {
	return VFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::linkage () ,that) ;
}

exports CFat<ImageProcHolder> ImageProcHolder::hold (CREF<ImageProcLayout> that) {
	return CFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::linkage () ,that) ;
}

struct TensorCopyFriend implement Interface {
	virtual void xcopy (VREF<Pointer> dst ,CREF<Pointer> src) const = 0 ;
} ;

template <class A ,class B>
class TensorCopyBinder final implement Fat<TensorCopyFriend ,TensorLayout> {
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
		const auto r1x = choose_tensor_unknown (type_) ;
		RefBufferHolder::hold (fake.mTensor)->initialize (r1x ,size_) ;
		fake.mType = type_ ;
		fake.mSize = size_ ;
		shape (size_ ,1 ,1) ;
	}

	Unknown choose_tensor_unknown (Just<TensorDataType> type_) const {
		if (type_ == TensorDataType::Val32)
			return TensorUnknownBinder<CHAR> () ;
		if (type_ == TensorDataType::Val64)
			return TensorUnknownBinder<QUAD> () ;
		if (type_ == TensorDataType::Flt32)
			return TensorUnknownBinder<CHAR> () ;
		if (type_ == TensorDataType::Flt64)
			return TensorUnknownBinder<QUAD> () ;
		assert (FALSE) ;
		return TensorUnknownBinder<BYTE> () ;
	}

	LENGTH size () const override {
		return fake.mSize ;
	}

	Just<TensorDataType> type () const override {
		return fake.mType ;
	}

	LENGTH cx () const override {
		return fake.mCX ;
	}

	LENGTH cy () const override {
		return fake.mCY ;
	}

	LENGTH cz () const override {
		return fake.mCZ ;
	}

	TensorLayout recast (CREF<Just<TensorDataType>> type_) override {
		TensorLayout ret ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!fake.mTensor.exist ())
				discard ;
			if (type () == type_)
				discard ;
			TensorHolder::hold (ret)->initialize (fake.mSize ,type_) ;
			const auto r3x = choose_tensor_copy (type_ ,type ()) ;
			for (auto &&i : iter (0 ,fake.mSize)) {
				r3x->xcopy (Pointer::from (ret.mTensor[i]) ,Pointer::from (fake.mTensor[i])) ;
			}
		}
		if ifdo (act) {
			ret = move (fake) ;
		}
		return move (ret) ;
	}

	CFat<TensorCopyFriend> choose_tensor_copy (CREF<Just<TensorDataType>> dst ,CREF<Just<TensorDataType>> src) const {
		if (dst == TensorDataType::Val32)
			if (src == TensorDataType::Val32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL32 ,VAL32> () ,fake) ;
		if (dst == TensorDataType::Val32)
			if (src == TensorDataType::Val64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL32 ,VAL64> () ,fake) ;
		if (dst == TensorDataType::Val32)
			if (src == TensorDataType::Flt32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL32 ,FLT32> () ,fake) ;
		if (dst == TensorDataType::Val32)
			if (src == TensorDataType::Flt64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL32 ,FLT64> () ,fake) ;
		if (dst == TensorDataType::Val64)
			if (src == TensorDataType::Val32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL64 ,VAL32> () ,fake) ;
		if (dst == TensorDataType::Val64)
			if (src == TensorDataType::Val64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL64 ,VAL64> () ,fake) ;
		if (dst == TensorDataType::Val64)
			if (src == TensorDataType::Flt32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL64 ,FLT32> () ,fake) ;
		if (dst == TensorDataType::Val64)
			if (src == TensorDataType::Flt64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<VAL64 ,FLT64> () ,fake) ;
		if (dst == TensorDataType::Flt32)
			if (src == TensorDataType::Val32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT32 ,VAL32> () ,fake) ;
		if (dst == TensorDataType::Flt32)
			if (src == TensorDataType::Val64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT32 ,VAL64> () ,fake) ;
		if (dst == TensorDataType::Flt32)
			if (src == TensorDataType::Flt32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT32 ,FLT32> () ,fake) ;
		if (dst == TensorDataType::Flt32)
			if (src == TensorDataType::Flt64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT32 ,FLT64> () ,fake) ;
		if (dst == TensorDataType::Flt64)
			if (src == TensorDataType::Val32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT64 ,VAL32> () ,fake) ;
		if (dst == TensorDataType::Flt64)
			if (src == TensorDataType::Val64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT64 ,VAL64> () ,fake) ;
		if (dst == TensorDataType::Flt64)
			if (src == TensorDataType::Flt32)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT64 ,FLT32> () ,fake) ;
		if (dst == TensorDataType::Flt64)
			if (src == TensorDataType::Flt64)
				return CFat<TensorCopyFriend> (TensorCopyBinder<FLT64 ,FLT64> () ,fake) ;
		assert (FALSE) ;
		return CFat<TensorCopyFriend> (TensorCopyBinder<VAL32 ,VAL32> () ,fake) ;
	}

	void shape (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cz_) override {
		const auto r1x = cx_ * cy_ * cz_ ;
		noop (r1x) ;
		assert (r1x == size ()) ;
		fake.mCX = cx_ ;
		fake.mCY = cy_ ;
		fake.mCZ = cz_ ;
		fake.mSX = 1 ;
		fake.mSY = fake.mSX * fake.mCX ;
		fake.mSZ = fake.mSY * fake.mCY ;
	}

	VREF<Pointer> self_m () leftvalue override {
		return RefBufferHolder::hold (fake.mTensor)->self ;
	}

	CREF<Pointer> self_m () const leftvalue override {
		return RefBufferHolder::hold (fake.mTensor)->self ;
	}

	Ref<RefBuffer<BYTE>> borrow () leftvalue override {
		return Ref<RefBuffer<BYTE>>::reference (fake.mTensor) ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue override {
		return Ref<RefBuffer<BYTE>>::reference (fake.mTensor) ;
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
		fake.mTable = Array<INDEX> (size_) ;
		fake.mTable.fill (NONE) ;
	}

	LENGTH size () const override {
		return fake.mTable.size () ;
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
				fake.mTable[ix] = ret ;
				ix = iy ;
			}
		}
		return move (ret) ;
	}

	INDEX parent (CREF<INDEX> curr) const {
		if (curr == fake.mTable[curr])
			return NONE ;
		return fake.mTable[curr] ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		fake.mTable[ix] = ix ;
		fake.mTable[iy] = ix ;
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
		Array<INDEX> ret = Array<INDEX> (fake.mTable.size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : iter (0 ,fake.mTable.size ())) {
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
		fake.mSize = size_ ;
		fake.mInfinity = VAL32_MAX ;
		fake.mUser = Array<VAL32> (fake.mSize) ;
		fake.mUser.fill (0) ;
		fake.mWork = Array<VAL32> (fake.mSize) ;
		fake.mWork.fill (0) ;
		fake.mUserVisit = BitSet (fake.mSize) ;
		fake.mWorkVisit = BitSet (fake.mSize) ;
		fake.mMatch = Array<INDEX> (fake.mSize) ;
		fake.mMatch.fill (NONE) ;
		fake.mLack = Array<VAL32> (fake.mSize) ;
		fake.mLack.fill (0) ;
	}

	Array<INDEX> sort (CREF<Array<VAL32>> love) override {
		assert (fake.mMatch.size () > 0) ;
		assert (love.size () == MathProc::square (fake.mSize)) ;
		solve () ;
		return fake.mMatch ;
	}

	void solve () {
		for (auto &&i : iter (0 ,fake.mSize)) {
			fake.mUser[i] = -fake.mInfinity ;
			for (auto &&j : iter (0 ,fake.mSize)) {
				fake.mUser[i] = MathProc::max_of (fake.mUser[i] ,fake.mLove[i * fake.mSize + j]) ;
			}
		}
		for (auto &&i : iter (0 ,fake.mSize)) {
			fake.mLack.fill (fake.mInfinity) ;
			while (TRUE) {
				fake.mUserVisit.clear () ;
				fake.mWorkVisit.clear () ;
				if (dfs (i))
					break ;
				const auto r1x = invoke ([&] () {
					VAL32 ret = fake.mInfinity ;
					for (auto &&j : iter (0 ,fake.mSize)) {
						if (fake.mWorkVisit[j])
							continue ;
						ret = MathProc::min_of (ret ,fake.mLack[j]) ;
					}
					return move (ret) ;
				}) ;
				for (auto &&j : iter (0 ,fake.mSize)) {
					if ifdo (TRUE) {
						if (!fake.mUserVisit[j])
							discard ;
						fake.mUser[j] -= r1x ;
					}
					if ifdo (TRUE) {
						if (!fake.mWorkVisit[j])
							discard ;
						fake.mWork[j] += r1x ;
					}
					if ifdo (TRUE) {
						if (fake.mWorkVisit[j])
							discard ;
						fake.mLack[j] -= r1x ;
					}
				}
			}
		}
	}

	BOOL dfs (CREF<INDEX> user) {
		fake.mUserVisit[user] = TRUE ;
		for (auto &&i : iter (0 ,fake.mSize)) {
			if (fake.mWorkVisit[i])
				continue ;
			const auto r1x = fake.mUser[user] + fake.mWork[i] - fake.mLove[user * fake.mSize + i] ;
			if ifdo (TRUE) {
				if (r1x == 0)
					discard ;
				fake.mLack[i] = MathProc::min_of (fake.mLack[i] ,r1x) ;
			}
			if (r1x != 0)
				continue ;
			fake.mWorkVisit[i] = TRUE ;
			const auto r2x = fake.mMatch[i] ;
			if ifdo (TRUE) {
				if (r2x != NONE)
					discard ;
				fake.mMatch[i] = user ;
				return TRUE ;
			}
			if ifdo (TRUE) {
				if (!dfs (r2x))
					discard ;
				fake.mMatch[i] = user ;
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
