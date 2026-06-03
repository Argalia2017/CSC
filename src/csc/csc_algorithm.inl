#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_algorithm.hpp"

namespace CSC {
class DisjointImplHolder final implement Fat<DisjointHolder ,DisjointLayout> {
public:
	void initialize (CR<Length> size_) override {
		self.mTable = Array<Index> (size_) ;
		self.mTable.fill (NONE) ;
	}

	Length size () const override {
		return self.mTable.size () ;
	}

	Index lead (CR<Index> from_) override {
		Index ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ix = parent (ix) ;
		}
		Index ret = ix ;
		if ifdo (TRUE) {
			if (ix == NONE)
				discard ;
			ix = from_ ;
			Index iy = NONE ;
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

	Index parent (CR<Index> curr) const {
		if (curr == self.mTable[curr])
			return NONE ;
		return self.mTable[curr] ;
	}

	void joint (CR<Index> from_ ,CR<Index> to_) override {
		Index ix = lead (from_) ;
		Index iy = lead (to_) ;
		self.mTable[ix] = ix ;
		self.mTable[iy] = ix ;
	}

	Bool edge (CR<Index> from_ ,CR<Index> to_) override {
		Index ix = lead (from_) ;
		Index iy = lead (to_) ;
		return ix == iy ;
	}

	Length depth (CR<Index> from_) override {
		Length ret = 0 ;
		Index ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ret++ ;
			ix = parent (ix) ;
		}
		return move (ret) ;
	}

	Deque<Index> cluster (CR<Index> from_) override {
		Deque<Index> ret ;
		Index ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ret.add (ix) ;
			ix = parent (ix) ;
		}
		return move (ret) ;
	}

	Array<Index> jump (CR<Index> from_) override {
		Array<Index> ret = Array<Index> (self.mTable.size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : range (0 ,self.mTable.size ())) {
			Index ix = lead (i) ;
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
	void initialize (CR<Length> size_) override {
		self.mSize = size_ ;
		self.mThreshold = Flt32 (0.1) ;
		self.mUser = Array<Flt32> (self.mSize) ;
		self.mWork = Array<Flt32> (self.mSize) ;
		self.mUserVisit = BitSet (self.mSize) ;
		self.mWorkVisit = BitSet (self.mSize) ;
		self.mMatch = Array<Index> (self.mSize) ;
		self.mLack = Array<Flt32> (self.mSize) ;
	}

	void set_threshold (CR<Flt64> threshold) override {
		self.mThreshold = Flt32 (threshold) ;
	}

	Length size () const override {
		return self.mSize ;
	}

	Array<Index> sort (CR<Image<Flt32>> love) override {
		assert (self.mMatch.size () > 0) ;
		assert (love.size () == MathProc::square (self.mSize)) ;
		self.mLove = Ref<Image<Flt32>>::reference (love) ;
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
					Flt32 ret = infinity ;
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

	Bool dfs (CR<Index> user) {
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

class TPSFitImplHolder final implement Fat<TPSFitHolder ,TPSFitLayout> {
public:
	void initialize (CR<Array<Vector>> dst ,CR<Array<Vector>> src) override {
		assert (dst.size () > 0) ;
		assert (dst.size () == src.size ()) ;
		const auto r1x = src.size () ;
		const auto r2x = PointCloud (Ref<Array<Vector>>::reference (src)) ;
		const auto r3x = PointCloud (Ref<Array<Vector>>::reference (dst)) ;
		self.mNSrc = r2x.pca_matrix () ;
		self.mNDst = r3x.pca_matrix () ;
		const auto r4x = self.mNSrc[1] * r2x ;
		const auto r5x = self.mNDst[1] * r3x ;
		self.mQA = Image<Flt64> (r1x + 4 ,r1x + 4) ;
		self.mQB = Image<Flt64> (3 ,r1x + 4) ;
		for (auto &&i : range (0 ,r1x ,0 ,r1x)) {
			if (i.mY > i.mX)
				continue ;
			const auto r6x = (r4x[i.mY] - r4x[i.mX]).magnitude () ;
			const auto r7x = basic_function (r6x) ;
			self.mQA.at (i.mX ,i.mY) = r7x ;
			self.mQA.at (i.mY ,i.mX) = r7x ;
		}
		for (auto &&i : range (0 ,r1x)) {
			const auto r8x = r4x[i] ;
			const auto r9x = r5x[i] ;
			for (auto &&j : range (0 ,4)) {
				self.mQA.at (r1x + j ,i) = r8x[j] ;
				self.mQA.at (i ,r1x + j) = r8x[j] ;
			}
			for (auto &&j : range (0 ,3)) {
				self.mQB.at (j ,i) = r9x[j] ;
			}
		}
		for (auto &&i : range (0 ,4)) {
			for (auto &&j : range (0 ,4)) {
				self.mQA.at (r1x + j ,r1x + i) = 0 ;
			}
			for (auto &&j : range (0 ,3)) {
				self.mQB.at (j ,r1x + i) = 0 ;
			}
		}
		self.mQC = LinearProc::solve_lsm (self.mQA ,self.mQB) ;
		self.mPSrc = Array<Vector> (r1x) ;
		for (auto &&i : self.mPSrc.iter ()) {
			self.mPSrc[i] = r4x[i] ;
		}
	}

	Vector smul (CR<Vector> that) const override {
		assert (self.mQC.size () > 0) ;
		Vector ret = Vector::axis_w () ;
		const auto r1x = self.mPSrc.length () ;
		const auto r2x = self.mNSrc[1] * that ;
		for (auto &&i : range (0 ,r1x)) {
			for (auto &&j : range (0 ,3)) {
				const auto r3x = r2x - self.mPSrc[i] ;
				const auto r4x = basic_function (r3x.magnitude ()) ;
				ret[j] += self.mQC.at (j ,i) * r4x ;
			}
		}
		for (auto &&i : range (0 ,4)) {
			for (auto &&j : range (0 ,3)) {
				ret[j] += self.mQC.at (j ,r1x + i) * r2x[i] ;
			}
		}
		ret = self.mNDst[0] * ret ;
		return move (ret) ;
	}

	Flt64 basic_function (CR<Flt64> r) const {
		return MathProc::square (r) * MathProc::log (r + FLT64_EPS) ;
	}

	Flt64 basic_function_diff (CR<Flt64> r) const {
		return 2 * MathProc::log (r + FLT64_EPS) + 1 ;
	}

	Matrix jacobian (CR<Vector> that) const override {
		assert (self.mQC.size () > 0) ;
		const auto r1x = self.mPSrc.length () ;
		const auto r2x = self.mNSrc[1] * that ;
		Matrix ret = Matrix::iden () ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r3x = r2x - self.mPSrc[i] ;
			const auto r4x = basic_function_diff (r3x.magnitude ()) ;
			for (auto &&j : range (0 ,3 ,0 ,3)) {
				ret[j] += self.mQC.at (j.mY ,i) * r4x * r3x[j.mX] ;
			}
		}
		for (auto &&i : range (0 ,3 ,0 ,3)) {
			ret[i] += self.mQC.at (i.mY ,r1x + i.mX) ;
		}
		ret = self.mNDst[0] * ret * self.mNSrc[1] ;
		ret = ret.homogenize () + Matrix::axis_w () ;
		return move (ret) ;
	}
} ;

exports VFat<TPSFitHolder> TPSFitHolder::hold (VR<TPSFitLayout> that) {
	return VFat<TPSFitHolder> (TPSFitImplHolder () ,that) ;
}

exports CFat<TPSFitHolder> TPSFitHolder::hold (CR<TPSFitLayout> that) {
	return CFat<TPSFitHolder> (TPSFitImplHolder () ,that) ;
}

class BCSFitImplHolder final implement Fat<BCSFitHolder ,BCSFitLayout> {
public:
	void initialize (CR<Array<Vector>> dst ,CR<Array<Vector>> src) override {
		assert (dst.size () > 0) ;
		assert (dst.size () == src.size ()) ;
		self.mRank = dst.size () / 2 ;
		const auto r1x = self.mRank + 1 ;
		const auto r2x = src.size () ;
		const auto r3x = PointCloud (Ref<Array<Vector>>::reference (src)) ;
		const auto r4x = r3x.box_matrix (1 ,1 ,1) ;
		const auto r5x = TranslationMatrix (-1 ,-1 ,-1) * DiagMatrix (2 ,2 ,2) ;
		self.mNSrc = r4x * r5x ;
		self.mPCtrl = Array<Vector> (r1x) ;
		self.mQA = Image<Flt64> (r1x ,r2x) ;
		self.mQB = Image<Flt64> (3 ,r2x) ;
		for (auto &&i : range (0 ,r2x)) {
			const auto r6x = self.mNSrc[1] * src[i] ;
			Singleton<Console>::expr.info (Format (slice ("[$1] $2 : $3")) (i ,src[i][0] ,r6x[0])) ;
			for (auto &&j : range (0 ,r1x)) {
				self.mQA.at (j ,i) = bernstein (j ,self.mRank ,r6x[0]) ;
			}
			for (auto &&j : range (0 ,3)) {
				self.mQB.at (j ,i) = dst[i][j] ;
			}
		}
		self.mQC = LinearProc::solve_lsm (self.mQA ,self.mQB) ;
		for (auto &&i : range (0 ,r1x)) {
			self.mPCtrl[i] = Vector::axis_w () ;
			for (auto &&j : range (0 ,3)) {
				self.mPCtrl[i][j] = self.mQC.at (j ,i) ;
			}
		}
	}

	Vector smul (CR<Vector> that) const override {
		assert (self.mPCtrl.size () > 0) ;
		const auto r1x = self.mRank + 1 ;
		const auto r2x = self.mNSrc[1] * that ;
		Vector ret = Vector::axis_w () ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r3x = bernstein (i ,self.mRank ,r2x[0]) ;
			for (auto &&j : range (0 ,3)) {
				ret[j] += r3x * self.mPCtrl[i][j] ;
			}
		}
		return move (ret) ;
	}

	Flt64 bernstein (CR<Index> i ,CR<Length> n ,CR<Flt64> t) const {
		Flt64 ret = 1 ;
		for (auto &&j : range (0 ,i)) {
			ret *= (n - j) ;
			ret /= (j + 1) ;
		}
		for (auto &&j : range (0 ,i)) {
			ret *= t ;
		}
		for (auto &&j : range (0 ,n - i)) {
			ret *= (1 - t) ;
		}
		return move (ret) ;
	}

	Flt64 bernstein_diff (CR<Index> i ,CR<Length> n ,CR<Flt64> t) const {
		if (n <= 0)
			return 0 ;
		const auto r1x = invoke ([&] () {
			if (i <= 0)
				return Flt64 (0) ;
			return bernstein (i - 1 ,n - 1 ,t) ;
		}) ;
		const auto r2x = invoke ([&] () {
			if (i >= n)
				return Flt64 (0) ;
			return bernstein (i ,n - 1 ,t) ;
		}) ;
		return n * (r1x - r2x) ;
	}

	Matrix jacobian (CR<Vector> that) const override {
		assert (self.mPCtrl.size () > 0) ;
		const auto r1x = self.mRank + 1 ;
		const auto r2x = self.mNSrc[1] * that ;
		Matrix ret = Matrix::zero () ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r3x = bernstein_diff (i ,self.mRank ,r2x[0]) ;
			for (auto &&j : range (0 ,3)) {
				ret.at (0 ,j) += r3x * self.mPCtrl[i][j] ;
			}
		}
		return move (ret) ;
	}
} ;

exports VFat<BCSFitHolder> BCSFitHolder::hold (VR<BCSFitLayout> that) {
	return VFat<BCSFitHolder> (BCSFitImplHolder () ,that) ;
}

exports CFat<BCSFitHolder> BCSFitHolder::hold (CR<BCSFitLayout> that) {
	return CFat<BCSFitHolder> (BCSFitImplHolder () ,that) ;
}
} ;