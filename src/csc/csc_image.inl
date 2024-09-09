#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

namespace CSC {
class ImageImplHolder implement Fat<ImageHolder ,ImageLayout> {
public:
	void initialize (CREF<Unknown> holder ,RREF<ImageLayout> that) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		noop (r2x) ;
		assert (r2x == that.mWidth.mStep) ;
		assert (r2x == that.mImage.step ()) ;
		fake = move (that) ;
	}

	void initialize (CREF<Unknown> holder ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> step_) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = cx_ * cy_ * step_ ;
		const auto r4x = inline_alignas (r3x ,r2x) / r2x ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (fake.mImage) ;
		RefBufferHolder::create (rax)->initialize (holder ,r4x) ;
		rax.mSize = cx_ * cy_ ;
		rax.mStep = step_ ;
		fake.mWidth.mCX = cx_ ;
		fake.mWidth.mCY = cy_ ;
		fake.mWidth.mStep = step_ ;
		reset () ;
	}

	ImageLayout clone () const override {
		ImageLayout ret ;
		if ifdo (TRUE) {
			const auto r1x = width () ;
			if (r1x.size () == 0)
				discard ;
			ImageHolder::create (ret)->initialize (fake.mImage.unknown () ,r1x.mCX ,r1x.mCY ,r1x.mStep) ;
			ImageHolder::create (ret)->splice (0 ,0 ,fake) ;
		}
		return move (ret) ;
	}

	BOOL exist () const override {
		return fake.mImage.exist () ;
	}

	LENGTH size () const override {
		return fake.mImage.size () ;
	}

	LENGTH step () const override {
		return fake.mImage.step () ;
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

	LENGTH tx () const override {
		if (!fake.mImage.exist ())
			return 0 ;
		return fake.mTX ;
	}

	LENGTH ty () const override {
		if (!fake.mImage.exist ())
			return 0 ;
		return fake.mTY ;
	}

	ImageWidth width () const override {
		ImageWidth ret ;
		ret.mCX = cx () ;
		ret.mCY = cy () ;
		ret.mStep = step () ;
		return move (ret) ;
	}

	void reset () override {
		fake.mCX = fake.mWidth.mCX ;
		fake.mCY = fake.mWidth.mCY ;
		fake.mTX = 0 ;
		fake.mTY = 0 ;
	}

	void reset (CREF<INDEX> cx_ ,CREF<INDEX> cy_ ,CREF<INDEX> tx_ ,CREF<INDEX> ty_) override {
		fake.mCX = cx_ ;
		fake.mCY = cy_ ;
		fake.mTX = tx_ ;
		fake.mTY = ty_ ;
	}

	VREF<BoxLayout> raw () leftvalue override {
		return RefBufferHolder::create (fake.mImage)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue override {
		return RefBufferHolder::create (fake.mImage)->raw () ;
	}

	VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + fake.mTX) + (y + fake.mTY) * fake.mWidth.mCX ;
		return fake.mImage.at (ix) ;
	}

	CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = (x + fake.mTX) + (y + fake.mTY) * fake.mWidth.mCX ;
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
		const auto r1x = ImageHolder::create (item)->cx () ;
		const auto r2x = ImageHolder::create (item)->cy () ;
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
			inline_memcpy (at (ix ,iy) ,ImageHolder::create (item)->at (0 ,i) ,r3x) ;
		}
	}
} ;

exports VFat<ImageHolder> ImageHolder::create (VREF<ImageLayout> that) {
	return VFat<ImageHolder> (ImageImplHolder () ,that) ;
}

exports CFat<ImageHolder> ImageHolder::create (CREF<ImageLayout> that) {
	return CFat<ImageHolder> (ImageImplHolder () ,that) ;
}

template class External<ImageProcHolder ,ImageProcLayout> ;

exports VFat<ImageProcHolder> ImageProcHolder::create (VREF<ImageProcLayout> that) {
	return VFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::instance () ,that) ;
}

exports CFat<ImageProcHolder> ImageProcHolder::create (CREF<ImageProcLayout> that) {
	return CFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::instance () ,that) ;
}

class SparseImplHolder implement Fat<SparseHolder ,SparseLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		fake.mTable = Array<INDEX> (size_) ;
		fake.mTable.fill (NONE) ;
	}

	LENGTH size () const override {
		return fake.mTable.size () ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
		INDEX ix = fake.mEdge.insert () ;
		fake.mEdge[ix].mSrc = from_ ;
		fake.mEdge[ix].mSrcNext = fake.mTable[from_] ;
		fake.mTable[from_] = ix ;
		fake.mEdge[ix].mDst = to_ ;
		fake.mEdge[ix].mDstNext = fake.mTable[to_] ;
		fake.mTable[to_] = ix ;
	}

	BOOL edge (CREF<INDEX> from_ ,CREF<INDEX> to_) const override {
		INDEX ix = fake.mTable[from_] ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (fake.mEdge[ix].mSrc != from_)
					discard ;
				if (fake.mEdge[ix].mDst == to_)
					return TRUE ;
				ix = fake.mEdge[ix].mSrcNext ;
			}
			if ifdo (act) {
				if (fake.mEdge[ix].mDst != from_)
					discard ;
				if (fake.mEdge[ix].mSrc == to_)
					return TRUE ;
				ix = fake.mEdge[ix].mDstNext ;
			}
		}
		return FALSE ;
	}

	LENGTH depth (CREF<INDEX> from_) const override {
		LENGTH ret = 0 ;
		INDEX ix = fake.mTable[from_] ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (fake.mEdge[ix].mSrc != from_)
					discard ;
				ret++ ;
				ix = fake.mEdge[ix].mSrcNext ;
			}
			if ifdo (act) {
				if (fake.mEdge[ix].mDst != from_)
					discard ;
				ret++ ;
				ix = fake.mEdge[ix].mDstNext ;
			}
		}
		return move (ret) ;
	}

	Deque<INDEX> cluster (CREF<INDEX> from_) const override {
		Deque<INDEX> ret ;
		INDEX ix = fake.mTable[from_] ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (fake.mEdge[ix].mSrc != from_)
					discard ;
				ret.add (fake.mEdge[ix].mDst) ;
				ix = fake.mEdge[ix].mSrcNext ;
			}
			if ifdo (act) {
				if (fake.mEdge[ix].mDst != from_)
					discard ;
				ret.add (fake.mEdge[ix].mSrc) ;
				ix = fake.mEdge[ix].mDstNext ;
			}
		}
		return move (ret) ;
	}
} ;

exports VFat<SparseHolder> SparseHolder::create (VREF<SparseLayout> that) {
	return VFat<SparseHolder> (SparseImplHolder () ,that) ;
}

exports CFat<SparseHolder> SparseHolder::create (CREF<SparseLayout> that) {
	return CFat<SparseHolder> (SparseImplHolder () ,that) ;
}

class DisjointImplHolder implement Fat<DisjointHolder ,DisjointLayout> {
public:
	void initialize (CREF<LENGTH> size_) override {
		fake.mTable.self = Array<INDEX> (size_) ;
		fake.mTable->fill (NONE) ;
	}

	LENGTH size () const override {
		return fake.mTable->size () ;
	}

	INDEX lead (CREF<INDEX> from_) const override {
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
				fake.mTable.self[ix] = ret ;
				ix = iy ;
			}
		}
		return move (ret) ;
	}

	INDEX parent (CREF<INDEX> curr) const {
		if (curr == fake.mTable.self[curr])
			return NONE ;
		return fake.mTable.self[curr] ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		fake.mTable.self[ix] = ix ;
		fake.mTable.self[iy] = ix ;
	}

	BOOL edge (CREF<INDEX> from_ ,CREF<INDEX> to_) const override {
		INDEX ix = lead (from_) ;
		INDEX iy = lead (to_) ;
		return ix == iy ;
	}

	LENGTH depth (CREF<INDEX> from_) const override {
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

	Deque<INDEX> cluster (CREF<INDEX> from_) const override {
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

	Array<INDEX> jump (CREF<INDEX> from_) const override {
		Array<INDEX> ret = Array<INDEX> (fake.mTable->size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : iter (0 ,fake.mTable->size ())) {
			INDEX ix = lead (i) ;
			if (ix == NONE)
				continue ;
			ret[ix] = ret[i] ;
			ret[i] = ix ;
		}
		return move (ret) ;
	}
} ;

exports VFat<DisjointHolder> DisjointHolder::create (VREF<DisjointLayout> that) {
	return VFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

exports CFat<DisjointHolder> DisjointHolder::create (CREF<DisjointLayout> that) {
	return CFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

class KMMatchImplHolder implement Fat<KMMatchHolder ,KMMatchLayout> {
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
		return fake.mMatch.clone () ;
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

exports VFat<KMMatchHolder> KMMatchHolder::create (VREF<KMMatchLayout> that) {
	return VFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}

exports CFat<KMMatchHolder> KMMatchHolder::create (CREF<KMMatchLayout> that) {
	return CFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}
} ;
