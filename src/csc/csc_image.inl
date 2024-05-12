#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

namespace CSC {
class ImageImplement implement Fat<ImageHolder ,ImageLayout> {
public:
	void initialize (CREF<Unknown> element ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<Clazz> clazz) override {
		const auto r1x = cx_ * cy_ ;
		RefBufferHolder::create (fake.mImage)->initialize (element ,r1x) ;
		fake.mWidth.mCX = cx_ ;
		fake.mWidth.mCY = cy_ ;
		fake.mWidth.mClazz = clazz ;
		reset () ;
	}

	void initialize (CREF<Unknown> element ,CREF<ImageWidth> width) override {
		initialize (element ,width.mCX ,width.mCY ,width.mClazz) ;
	}

	LENGTH size () const override {
		return fake.mImage.size () ;
	}

	LENGTH step () const override {
		return fake.mImage.step () ;
	}

	LENGTH cx () const override {
		if ifnot (fake.mImage.exist ())
			return 0 ;
		return fake.mCX ;
	}

	LENGTH cy () const override {
		if ifnot (fake.mImage.exist ())
			return 0 ;
		return fake.mCY ;
	}

	LENGTH sx () const override {
		if ifnot (fake.mImage.exist ())
			return 0 ;
		return fake.mSX ;
	}

	LENGTH sy () const override {
		if ifnot (fake.mImage.exist ())
			return 0 ;
		return fake.mSY ;
	}

	LENGTH offset () const override {
		if ifnot (fake.mImage.exist ())
			return 0 ;
		return fake.mOffset ;
	}

	ImageWidth width () const override {
		ImageWidth ret ;
		ret.mCX = cx () ;
		ret.mCY = cy () ;
		ret.mClazz = fake.mWidth.mClazz ;
		return move (ret) ;
	}

	void reset () override {
		fake.mCX = fake.mWidth.mCX ;
		fake.mCY = fake.mWidth.mCY ;
		fake.mSX = 1 ;
		fake.mSY = fake.mWidth.mCX ;
		fake.mOffset = 0 ;
	}

	void reset (CREF<INDEX> cx_ ,CREF<INDEX> cy_ ,CREF<INDEX> sx_ ,CREF<INDEX> sy_ ,CREF<INDEX> offset_) override {
		fake.mCX = cx_ ;
		fake.mCY = cy_ ;
		fake.mSX = sx_ ;
		fake.mSY = sy_ ;
		fake.mOffset = offset_ ;
	}

	VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = x * fake.mSX + y * fake.mSY + fake.mOffset ;
		return fake.mImage.at (ix) ;
	}

	CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		INDEX ix = x * fake.mSX + y * fake.mSY + fake.mOffset ;
		return fake.mImage.at (ix) ;
	}

	void fill (CREF<Pointer> item) override {
		const auto r1x = RFat<ReflectClone> (fake.mImage.unknown ()) ;
		for (auto &&i : iter (0 ,cx () ,0 ,cy ())) {
			r1x->clone (at (i.mX ,i.mY) ,item) ;
		}
	}

	void splice (CREF<INDEX> x ,CREF<INDEX> y ,CREF<ImageLayout> item) override {
		const auto r1x = ImageHolder::create (item)->cx () ;
		const auto r2x = ImageHolder::create (item)->cy () ;
		assume (inline_between (x ,0 ,cx ())) ;
		assume (inline_between (y ,0 ,cy ())) ;
		assume (x + r1x <= cx ()) ;
		assume (y + r1x <= cy ()) ;
		const auto r3x = RFat<ReflectClone> (fake.mImage.unknown ()) ;
		for (auto &&i : iter (0 ,r1x ,0 ,r2x)) {
			INDEX ix = x + i.mX ;
			INDEX iy = y + i.mY ;
			r3x->clone (at (ix ,iy) ,ImageHolder::create (item)->at (i.mX ,i.mY)) ;
		}
	}
} ;

exports VFat<ImageHolder> ImageHolder::create (VREF<ImageLayout> that) {
	return VFat<ImageHolder> (ImageImplement () ,Pointer::from (that)) ;
}

exports CFat<ImageHolder> ImageHolder::create (CREF<ImageLayout> that) {
	return CFat<ImageHolder> (ImageImplement () ,Pointer::from (that)) ;
}

class SparseImplement implement Fat<SparseHolder ,SparseLayout> {
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

	BOOL contain (CREF<INDEX> from_ ,CREF<INDEX> to_) const override {
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
	return VFat<SparseHolder> (SparseImplement () ,Pointer::from (that)) ;
}

exports CFat<SparseHolder> SparseHolder::create (CREF<SparseLayout> that) {
	return CFat<SparseHolder> (SparseImplement () ,Pointer::from (that)) ;
}

class DisjointImplement implement Fat<DisjointHolder ,DisjointLayout> {
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

	BOOL contain (CREF<INDEX> from_ ,CREF<INDEX> to_) const override {
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
	return VFat<DisjointHolder> (DisjointImplement () ,Pointer::from (that)) ;
}

exports CFat<DisjointHolder> DisjointHolder::create (CREF<DisjointLayout> that) {
	return CFat<DisjointHolder> (DisjointImplement () ,Pointer::from (that)) ;
}

class KMMatchImplement implement Fat<KMMatchHolder ,KMMatchLayout> {
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
		const auto r1x = MathTool::square (fake.mSize) ;
		assume (love.size () == r1x) ;
		solve () ;
		return fake.mMatch ;
	}

	void solve () {
		for (auto &&i : iter (0 ,fake.mSize)) {
			fake.mUser[i] = -fake.mInfinity ;
			for (auto &&j : iter (0 ,fake.mSize)) {
				fake.mUser[i] = MathTool::max_of (fake.mUser[i] ,fake.mLove[i * fake.mSize + j]) ;
			}
		}
		for (auto &&i : iter (0 ,fake.mSize)) {
			fake.mLack.fill (fake.mInfinity) ;
			while (true) {
				fake.mUserVisit.clear () ;
				fake.mWorkVisit.clear () ;
				if (dfs (i))
					break ;
				const auto r1x = invoke ([&] () {
					VAL32 ret = fake.mInfinity ;
					for (auto &&j : iter (0 ,fake.mSize)) {
						if (fake.mWorkVisit[j])
							continue ;
						ret = MathTool::min_of (ret ,fake.mLack[j]) ;
					}
					return move (ret) ;
				}) ;
				for (auto &&j : iter (0 ,fake.mSize)) {
					if (fake.mUserVisit[j]) {
						fake.mUser[j] -= r1x ;
					}
					if (fake.mWorkVisit[j]) {
						fake.mWork[j] += r1x ;
					} else {
						fake.mLack[j] -= r1x ;
					}
				}
			}
		}
	}

	BOOL dfs (CREF<INDEX> user) {
		fake.mUserVisit[user] = TRUE;
		for (auto &&i : iter (0 ,fake.mSize)) {
			if (fake.mWorkVisit[i])
				continue ;
			const auto r1x = fake.mUser[user] + fake.mWork[i] - fake.mLove[user * fake.mSize + i] ;
			const auto r2x = fake.mMatch[i] ;
			if (r1x == 0) {
				fake.mWorkVisit[i] = TRUE ;
				if (r2x == NONE) {
					fake.mMatch[i] = user ;
					return TRUE ;
				} else if (dfs (r2x)) {
					fake.mMatch[i] = user ;
					return TRUE ;
				}
			} else {
				fake.mLack[i] = MathTool::min_of (fake.mLack[i] ,r1x) ;
			}
		}
		return FALSE ;
	}
} ;

exports VFat<KMMatchHolder> KMMatchHolder::create (VREF<KMMatchLayout> that) {
	return VFat<KMMatchHolder> (KMMatchImplement () ,Pointer::from (that)) ;
}

exports CFat<KMMatchHolder> KMMatchHolder::create (CREF<KMMatchLayout> that) {
	return CFat<KMMatchHolder> (KMMatchImplement () ,Pointer::from (that)) ;
}
} ;
