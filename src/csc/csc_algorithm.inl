#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_algorithm.hpp"

namespace CSC {
class DisjointImplHolder final implement Fat<DisjointHolder ,DisjointLayout> {
public:
	void initialize (CR<Length> size_) override {
		self.mTable = Array<DisjointNode> (size_) ;
		const auto r1x = invoke ([&] () {
			DisjointNode ret ;
			ret.mUp = NONE ;
			ret.mWidth = 0 ;
			return move (ret) ;
		}) ;
		self.mTable.fill (r1x) ;
	}

	Length size () const override {
		return self.mTable.size () ;
	}

	Index lead (CR<Index> from) override {
		Index ret = from ;
		if ifdo (TRUE) {
			if (self.mTable[ret].mUp == NONE)
				discard ;
			while (TRUE) {
				if (self.mTable[ret].mUp == ret)
					break ;
				ret = self.mTable[ret].mUp ;
			}
			Index ix = from ;
			Index iy = NONE ;
			while (TRUE) {
				iy = ix ;
				ix = self.mTable[iy].mUp ;
				if (ix == ret)
					break ;
				self.mTable[iy].mUp = ret ;
				self.mTable[ix].mWidth -= self.mTable[iy].mWidth ;
			}
		}
		return move (ret) ;
	}

	Length count (CR<Index> from) const override {
		if (self.mTable[from].mUp == NONE)
			return 0 ;
		return self.mTable[from].mWidth ;
	}

	void joint (CR<Index> from ,CR<Index> into) override {
		Index ix = lead (from) ;
		Index iy = lead (into) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = from ;
			self.mTable[ix].mUp = ix ;
			self.mTable[ix].mWidth = 1 ;
		}
		if ifdo (TRUE) {
			if (iy != NONE)
				discard ;
			iy = into ;
			self.mTable[iy].mUp = iy ;
			self.mTable[iy].mWidth = 1 ;
		}
		if (ix == iy)
			return ;
		if ifdo (TRUE) {
			if (self.mTable[ix].mWidth >= self.mTable[iy].mWidth)
				discard ;
			swap (ix ,iy) ;
		}
		self.mTable[iy].mUp = ix ;
		self.mTable[ix].mWidth += self.mTable[iy].mWidth ;
	}

	Bool is_edge (CR<Index> from ,CR<Index> into) override {
		Index ix = lead (from) ;
		Index iy = lead (into) ;
		if (ix == NONE)
			return FALSE ;
		if (iy == NONE)
			return FALSE ;
		return ix == iy ;
	}

	BitSet cluster (CR<Index> from) override {
		BitSet ret = BitSet (self.mTable.size ()) ;
		if ifdo (TRUE) {
			Index ix = lead (from) ;
			if (ix == NONE)
				discard ;
			for (auto &&i : self.mTable.iter ()) {
				Index iy = lead (i) ;
				if (ix != iy)
					continue ;
				ret.add (i) ;
			}
		}
		return move (ret) ;
	}

	Array<Index> closure () override {
		Array<Index> ret = Array<Index> (self.mTable.size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : self.mTable.iter ()) {
			Index ix = lead (i) ;
			if (ix == NONE)
				continue ;
			ret[i] = ret[ix] ;
			ret[ix] = i ;
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

class RansacImplHolder final implement Fat<RansacHolder ,RansacLayout> {
public:
	void initialize (CR<Length> rank_ ,CR<Length> size_) override {
		assert (rank_ > 0) ;
		assert (size_ > 0) ;
		assert (rank_ <= size_) ;
		self.mRank = rank_ ;
		self.mSize = size_ ;
		self.mRandom = CurrentRandom () ;
		set_iteration (10000) ;
		set_probability (0.9973) ;
	}

	void set_seed (CR<Flag> seed_) override {
		self.mRandom = Random (seed_) ;
	}

	void set_probability (CR<Flt64> probability) override {
		//@info: gaussion distribution: [0.6826 ,0.9544 ,0.9973]
		const auto r1x = MathProc::clamp (probability ,Flt64 (0) ,Flt64 (1) - FLT64_EPS) ;
		self.mProbFactor = MathProc::log (r1x + FLT64_EPS) ;
	}

	void set_iteration (CR<Length> iteration) override {
		self.mIteration = 0 ;
		self.mMaxIteration = iteration ;
	}

	Length size () const override {
		return self.mSize ;
	}

	void sample () override {
		if ifdo (TRUE) {
			if (self.mSample.size () > 0)
				discard ;
			self.mSample = Array<Index> (self.mRank) ;
			self.mCurrInlier = BitSet (self.mSize) ;
			self.mBestInlier = BitSet (self.mSize) ;
			self.mCurrSize = 0 ;
			self.mBestSize = 0 ;
		}
		self.mRandom.random_pick (self.mRank ,self.mSize ,self.mCurrInlier) ;
		Index ix = 0 ;
		for (auto &&i : self.mCurrInlier) {
			self.mSample[ix] = i ;
			ix++ ;
		}
		self.mCurrInlier.clear () ;
		self.mCurrSize = 0 ;
	}

	CR<Pointer> peek () const leftvalue override {
		return Pointer::from (self.mSample[0]) ;
	}

	Bool good () const override {
		return self.mIteration < self.mMaxIteration ;
	}

	void next () override {
		self.mCurrSize = self.mCurrInlier.length () ;
		if ifdo (TRUE) {
			if (self.mCurrSize <= self.mBestSize)
				discard ;
			swap (self.mCurrSize ,self.mBestSize) ;
			swap (self.mCurrInlier ,self.mBestInlier) ;
		}
		self.mIteration++ ;
		const auto r1x = Flt64 (self.mBestSize) * MathProc::inverse (Flt64 (self.mSize)) ;
		const auto r2x = 1 - MathProc::pow (r1x ,Val32 (self.mRank)) ;
		const auto r3x = MathProc::clamp (r2x ,Flt64 (0) ,Flt64 (1) - FLT64_EPS) ;
		const auto r4x = MathProc::log (r3x + FLT64_EPS) ;
		const auto r5x = MathProc::delta (r4x) * Flt64 (self.mMaxIteration) ;
		const auto r6x = self.mProbFactor * MathProc::inverse (r4x) + r5x ;
		self.mMaxIteration = MathProc::clamp (Length (r6x) ,Length (1) ,self.mMaxIteration) ;
	}

	void add (CR<Index> index) override {
		self.mCurrInlier.add (index) ;
	}

	BitSet cluster () const override {
		return self.mBestInlier ;
	}

	Length iteration () const override {
		return self.mIteration ;
	}

	Flt64 percent () const override {
		const auto r1x = Flt64 (self.mBestSize) * MathProc::inverse (Flt64 (self.mSize)) ;
		const auto r2x = MathProc::round (r1x * 10000) / 100 ;
		return r2x ;
	}
} ;

exports VFat<RansacHolder> RansacHolder::hold (VR<RansacLayout> that) {
	return VFat<RansacHolder> (RansacImplHolder () ,that) ;
}

exports CFat<RansacHolder> RansacHolder::hold (CR<RansacLayout> that) {
	return CFat<RansacHolder> (RansacImplHolder () ,that) ;
}

class KMeansImplHolder final implement Fat<KMeansHolder ,KMeansLayout> {
public:
	void initialize (CR<Length> rank_ ,CR<DataFrame> pool) override {
		assert (rank_ > 0) ;
		self.mRank = rank_ ;
		self.mPool = pool ;
		const auto r1x = self.mPool.size () ;
		assert (rank_ <= r1x) ;
		self.mCurrCenter = Array<Array<Flt64>> (self.mRank) ;
		self.mCurrCluster = Array<BitSet> (self.mRank) ;
		self.mNextCluster = Array<BitSet> (self.mRank) ;
		for (auto &&i : range (0 ,self.mRank)) {
			self.mCurrCluster[i] = BitSet (r1x) ;
			self.mNextCluster[i] = BitSet (r1x) ;
		}
		self.mCost.mMax = infinity ;
		self.mCost.mAvg = infinity ;
		self.mCost.mStd = 0 ;
	}

	Length rank () const override {
		return self.mRank ;
	}

	Bool good () const override {
		if (self.mCost.mMax > 0)
			return TRUE ;
		return FALSE ;
	}

	void next () override {
		update_curr_center () ;
		update_next_cluster () ;
		update_curr_cluster () ;
	}

	void update_curr_center () {
		auto rax = self.mPool.map (slice ("point") ,0) ;
		for (auto &&i : range (0 ,self.mRank)) {
			const auto r1x = rax.channel () ;
			self.mCurrCenter[i] = Array<Flt64> (r1x) ;
			self.mCurrCenter[i].fill (0) ;
			for (auto &&j : self.mCurrCluster[i]) {
				rax.target (j) ;
				for (auto &&k : range (0 ,r1x)) {
					noop (k) ;
					//self.mCurrCenter[i][k] += rax[k] ;
				}
			}
			const auto r2x = MathProc::inverse (Flt64 (self.mCurrCluster[i].length ())) ;
			for (auto &&k : range (0 ,r1x)) {
				self.mCurrCenter[i][k] *= r2x ;
			}
		}
	}

	void update_next_cluster () {
		for (auto &&i : range (0 ,self.mRank))
			self.mNextCluster[i].clear () ;
		auto rax = self.mPool.map (slice ("point") ,0) ;
		for (auto &&i : self.mPool.iter ()) {
			rax.target (i) ;
			auto rbx = IndexPair<Flt64> () ;
			rbx.m2nd = NONE ;
			for (auto &&j : range (0 ,self.mRank)) {
				const auto r1x = distance (self.mCurrCenter[j] ,rax) ;
				if (rbx.m2nd != NONE)
					if (rbx.m1st <= r1x)
						continue ;
				rbx.m1st = r1x ;
				rbx.m2nd = j ;
			}
			self.mNextCluster[rbx.m2nd].add (i) ;
		}
	}

	void update_curr_cluster () {
		self.mCost = NormalError () ;
		auto rax = self.mPool.map (slice ("point") ,0) ;
		for (auto &&i : range (0 ,self.mRank)) {
			const auto r1x = self.mCurrCluster[i] - self.mNextCluster[i] ;
			for (auto &&j : r1x) {
				rax.target (j) ;
				const auto r2x = distance (self.mCurrCenter[i] ,rax) ;
				const auto r3x = MathProc::pdf (r2x) ;
				self.mCost += r3x ;
			}
		}
		swap (self.mCurrCluster ,self.mNextCluster) ;
	}

	Flt64 distance (CR<Array<Flt64>> a ,CR<Property> b) const {
		Flt64 ret = 0 ;
		for (auto &&i : a.iter ()) {
			noop (i) ;
			//ret += MathProc::square (a[i] - b[i]) ;
		}
		ret = MathProc::sqrt (ret) ;
		return move (ret) ;
	}

	BitSet cluster (CR<Index> from) const override {
		return self.mCurrCluster[from] ;
	}
} ;

exports VFat<KMeansHolder> KMeansHolder::hold (VR<KMeansLayout> that) {
	return VFat<KMeansHolder> (KMeansImplHolder () ,that) ;
}

exports CFat<KMeansHolder> KMeansHolder::hold (CR<KMeansLayout> that) {
	return CFat<KMeansHolder> (KMeansImplHolder () ,that) ;
}

class KMMatchImplHolder final implement Fat<KMMatchHolder ,KMMatchLayout> {
public:
	void initialize (CR<Length> size_) override {
		assert (size_ > 0) ;
		self.mSize = size_ ;
		self.mEpsilon = Flt64 (0.1) ;
		self.mDirection = 1 ;
		self.mUser = Array<Flt64> (self.mSize) ;
		self.mWork = Array<Flt64> (self.mSize) ;
		self.mUserVisit = BitSet (self.mSize) ;
		self.mWorkVisit = BitSet (self.mSize) ;
		self.mMatch = Array<Index> (self.mSize) ;
		self.mLack = Array<Flt64> (self.mSize) ;
	}

	void set_epsilon (CR<Flt64> threshold) override {
		self.mEpsilon = Flt64 (threshold) ;
	}

	Length size () const override {
		return self.mSize ;
	}

	Array<Index> solve_min (CR<Image<Flt64>> cost) override {
		assert (self.mMatch.size () > 0) ;
		assert (cost.size () == MathProc::square (self.mSize)) ;
		self.mDirection = -1 ;
		self.mLove = Ref<Image<Flt64>>::reference (cost) ;
		self.mUser.fill (0) ;
		self.mWork.fill (0) ;
		self.mUserVisit.clear () ;
		self.mWorkVisit.clear () ;
		self.mMatch.fill (NONE) ;
		self.mLack.fill (0) ;
		solve () ;
		return self.mMatch ;
	}

	Array<Index> solve_max (CR<Image<Flt64>> love) override {
		assert (self.mMatch.size () > 0) ;
		assert (love.size () == MathProc::square (self.mSize)) ;
		self.mDirection = +1 ;
		self.mLove = Ref<Image<Flt64>>::reference (love) ;
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
				const auto r1x = self.mLove.ref[i][j] * self.mDirection ;
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
					Flt64 ret = infinity ;
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
			const auto r1x = self.mLove.ref[user][i] * self.mDirection ;
			const auto r2x = self.mUser[user] + self.mWork[i] - r1x ;
			if ifdo (TRUE) {
				if (r2x < self.mEpsilon)
					discard ;
				self.mLack[i] = MathProc::min_of (self.mLack[i] ,r2x) ;
			}
			if (r2x >= self.mEpsilon)
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

class MinCutImplHolder final implement Fat<MinCutHolder ,MinCutLayout> {
public:
	void initialize (CR<Length> size_) override {
		assert (size_ > 0) ;
		self.mSize = size_ + 2 ;
		self.mRootS = self.mSize - 2 ;
		self.mRootT = self.mSize - 1 ;
		self.mFirst = Array<Index> (self.mSize) ;
		self.mFirst.fill (NONE) ;
		self.mCurrent = Array<Index> (self.mSize) ;
		self.mDepth = Array<Length> (self.mSize) ;
		self.mEdge = List<MinCutEdge> (self.mSize * 4) ;
		self.mDeque = Deque<Index> (self.mSize) ;
		self.mReady = FALSE ;
	}

	Length size () const override {
		return self.mSize - 2 ;
	}

	Index root_s () const override {
		return self.mRootS ;
	}

	Index root_t () const override {
		return self.mRootT ;
	}

	void joint (CR<Index> from ,CR<Index> into ,CR<Val64> weight) override {
		assert (weight >= 0) ;
		assert (inline_mid (from ,0 ,self.mSize)) ;
		assert (inline_mid (into ,0 ,self.mSize)) ;
		assert (!self.mReady) ;
		if (from == into)
			return ;
		const auto r1x = invoke ([&] () {
			Tuple<Index ,Index> ret ;
			ret.m1st = from ;
			ret.m2nd = into ;
			if ifdo (TRUE) {
				if (ret.m2nd != self.mRootS)
					discard ;
				swap (ret.m1st ,ret.m2nd) ;
			}
			if ifdo (TRUE) {
				if (ret.m2nd != self.mRootT)
					discard ;
				swap (ret.m1st ,ret.m2nd) ;
			}
			return move (ret) ;
		}) ;
		const auto r2x = r1x.m1st ;
		const auto r3x = r1x.m2nd ;
		assume (r3x != self.mRootS) ;
		assume (r3x != self.mRootT) ;
		Index ix = self.mEdgeKey.map (r1x) ;
		Index iy = NONE ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mEdge.insert () ;
			iy = self.mEdge.insert () ;
			if ifdo (TRUE) {
				self.mEdge[ix].mFrom = r2x ;
				self.mEdge[ix].mInto = r3x ;
				self.mEdge[ix].mNext = self.mFirst[r2x] ;
				self.mFirst[r2x] = ix ;
				self.mEdge[ix].mWeight = 0 ;
				self.mEdge[ix].mInv = iy ;
			}
			if ifdo (TRUE) {
				self.mEdge[iy].mFrom = r3x ;
				self.mEdge[iy].mInto = r2x ;
				self.mEdge[iy].mNext = self.mFirst[r3x] ;
				self.mFirst[r3x] = iy ;
				self.mEdge[iy].mWeight = 0 ;
				self.mEdge[iy].mInv = ix ;
			}
			self.mEdgeKey.add (r1x ,ix) ;
		}
		iy = self.mEdge[ix].mInv ;
		if ifdo (TRUE) {
			if (r2x == self.mRootT)
				discard ;
			self.mEdge[ix].mWeight += weight ;
		}
		if ifdo (TRUE) {
			if (r2x == self.mRootS)
				discard ;
			self.mEdge[iy].mWeight += weight ;
		}
	}

	Val64 solve () override {
		Val64 ret = 0 ;
		while (TRUE) {
			self.mDeque.clear () ;
			self.mDeque.add (self.mRootS) ;
			bfs_level (self.mRootS) ;
			if (self.mDepth[self.mRootT] == 0)
				break ;
			for (auto &&i : self.mCurrent.iter ())
				self.mCurrent[i] = self.mFirst[i] ;
			while (TRUE) {
				const auto r1x = dfs_flow (self.mRootS ,VAL64_MAX) ;
				if (r1x == 0)
					break ;
				ret += r1x ;
			}
		}
		self.mReady = TRUE ;
		return move (ret) ;
	}

	Val64 dfs_flow (CR<Index> u ,CR<Val64> limit) {
		if (u == self.mRootT)
			return limit ;
		Val64 ret = 0 ;
		for (Index jx = self.mCurrent[u] ; jx != NONE ; jx = self.mEdge[jx].mNext) {
			self.mCurrent[u] = jx ;
			Index iy = self.mEdge[jx].mInto ;
			if (self.mEdge[jx].mWeight <= 0)
				continue ;
			if (self.mDepth[iy] != self.mDepth[u] + 1)
				continue ;
			const auto r1x = limit - ret ;
			const auto r2x = MathProc::min_of (r1x ,self.mEdge[jx].mWeight) ;
			const auto r3x = dfs_flow (iy ,r2x) ;
			if (r3x <= 0)
				continue ;
			Index jz = self.mEdge[jx].mInv ;
			self.mEdge[jx].mWeight -= r3x ;
			self.mEdge[jz].mWeight += r3x ;
			ret += r3x ;
			if (ret == limit)
				break ;
		}
		return move (ret) ;
	}

	void bfs_level (CR<Index> s) {
		self.mDepth.fill (0) ;
		self.mDepth[s] = 1 ;
		Index ix = NONE ;
		Index iy = NONE ;
		while (TRUE) {
			if (self.mDeque.empty ())
				break ;
			self.mDeque.take (ix) ;
			for (Index jx = self.mFirst[ix] ; jx != NONE ; jx = self.mEdge[jx].mNext) {
				iy = self.mEdge[jx].mInto ;
				if (self.mEdge[jx].mWeight <= 0)
					continue ;
				if (self.mDepth[iy] > 0)
					continue ;
				self.mDepth[iy] = self.mDepth[ix] + 1 ;
				self.mDeque.add (iy) ;
			}
		}
	}

	BitSet cluster () const override {
		assert (self.mReady) ;
		const auto r1x = size () ;
		BitSet ret = BitSet (r1x) ;
		for (auto &&i : range (0 ,r1x)) {
			if (self.mDepth[i] == 0)
				continue ;
			ret.add (i) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MinCutHolder> MinCutHolder::hold (VR<MinCutLayout> that) {
	return VFat<MinCutHolder> (MinCutImplHolder () ,that) ;
}

exports CFat<MinCutHolder> MinCutHolder::hold (CR<MinCutLayout> that) {
	return CFat<MinCutHolder> (MinCutImplHolder () ,that) ;
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

	CR<Array<Vector>> control () const leftvalue override {
		return self.mPCtrl ;
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

	Flt64 bernstein (CR<Index> x ,CR<Length> n ,CR<Flt64> t) const {
		Flt64 ret = 1 ;
		for (auto &&j : range (0 ,x)) {
			ret *= Flt64 (n - j) ;
			ret /= Flt64 (j + 1) ;
		}
		for (auto &&j : range (0 ,x)) {
			noop (j) ;
			ret *= t ;
		}
		for (auto &&j : range (0 ,n - x)) {
			noop (j) ;
			ret *= (1 - t) ;
		}
		return move (ret) ;
	}

	Flt64 bernstein_diff (CR<Index> x ,CR<Length> n ,CR<Flt64> t) const {
		if (n <= 0)
			return 0 ;
		const auto r1x = x > 0 ? bernstein (x - 1 ,n - 1 ,t) : Flt64 (0) ;
		const auto r2x = x < n ? bernstein (x ,n - 1 ,t) : Flt64 (0) ;
		return Flt64 (n) * (r1x - r2x) ;
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

class FFTransformImplHolder final implement Fat<FFTransformHolder ,FFTransformLayout> {
public:
	void initialize (CR<Length> size_) override {
		assert (size_ > 1) ;
		self.mRank = MathProc::log2_bit (size_ - 1) ;
		self.mSize = MathProc::exp2_bit (self.mRank) ;
		self.mCosSin = Array<Array<Vector>> (self.mRank + 1) ;
		for (auto &&i : range (0 ,self.mRank + 1)) {
			const auto r1x = MathProc::exp2_bit (i) ;
			self.mCosSin[i] = Array<Vector> (r1x) ;
			const auto r2x = MATH_PI * MathProc::inverse (Flt64 (r1x)) ;
			for (auto &&j : range (0 ,r1x)) {
				const auto r3x = Flt64 (j) * r2x ;
				self.mCosSin[i][j][0] = MathProc::cos (r3x) ;
				self.mCosSin[i][j][1] = -MathProc::sin (r3x) ;
				self.mCosSin[i][j][2] = 0 ;
				self.mCosSin[i][j][3] = 0 ;
			}
		}
		self.mScale[0] = 1 ;
		self.mScale[1] = MathProc::inverse (Flt64 (self.mSize)) ;
		self.mScale[2] = MathProc::sqrt (self.mScale[1]) ;
	}

	void set_unitary (CR<Bool> flag) override {
		//@info: use [cos ,-sin] as main direction
		//@info: det(DFT)=sqrt(N) ,normalize IDFT to implement det(IDFT(DFT*DFT))=1
		auto act = TRUE ;
		if ifdo (act) {
			if (!flag)
				discard ;
			swap (self.mScale[0] ,self.mScale[2]) ;
			self.mScale[1] = self.mScale[0] ;
		}
		if ifdo (act) {
			if (self.mScale[2] != 1)
				discard ;
			swap (self.mScale[0] ,self.mScale[2]) ;
			self.mScale[1] = MathProc::inverse (Flt64 (self.mSize)) ;
		}
		if ifdo (act) {
			swap (self.mScale[0] ,self.mScale[2]) ;
			self.mScale[1] = 1 ;
		}
	}

	Length size () const override {
		return self.mSize ;
	}

	Array<Vector> smul (CR<Array<Vector>> that) const override {
		assert (that.size () <= self.mSize) ;
		Array<Vector> ret = Array<Vector> (self.mSize) ;
		ret.fill (Vector::zero ()) ;
		for (auto &&i : that.iter ())
			ret[i] = that[i] ;
		fft (ret ,self.mRank) ;
		for (auto &&i : range (0 ,self.mSize)) {
			ret[i] *= self.mScale[0] ;
		}
		return move (ret) ;
	}

	void fft (VR<Array<Vector>> pt ,CR<Length> n) const {
		auto rax = Array<Vector> (self.mSize) ;
		for (auto &&i : range (0 ,self.mRank)) {
			const auto r1x = self.mRank - 1 - i ;
			const auto r2x = MathProc::exp2_bit (i) ;
			const auto r3x = r2x * 2 ;
			const auto r4x = MathProc::exp2_bit (r1x) ;
			const auto r5x = r4x * 2 ;
			for (auto &&j : range (0 ,r4x)) {
				for (auto &&k : range (0 ,r2x)) {
					Index jx = j + k * r5x ;
					Index jy = j + k * r5x + r4x ;
					const auto r6x = pt[jx] ;
					const auto r7x = complex_mul (self.mCosSin[i][k] ,pt[jy]) ;
					rax[k] = r6x + r7x ;
					rax[k + r2x] = r6x - r7x ;
				}
				for (auto &&k : range (0 ,r3x)) {
					Index ix = j + k * r4x ;
					pt[ix] = rax[k] ;
				}
			}
		}
	}

	Vector complex_mul (CR<Vector> a ,CR<Vector> b) const {
		Vector ret ;
		ret[0] = a[0] * b[0] - a[1] * b[1] ;
		ret[1] = a[1] * b[0] + a[0] * b[1] ;
		ret[2] = 0 ;
		ret[3] = 0 ;
		return move (ret) ;
	}

	FFTransformLayout inverse () const override {
		FFTransformLayout ret ;
		ret.mSize = self.mSize ;
		ret.mRank = self.mRank ;
		ret.mCosSin = Array<Array<Vector>> (self.mRank + 1) ;
		for (auto &&i : range (0 ,self.mRank + 1)) {
			const auto r1x = self.mCosSin[i].length () ;
			ret.mCosSin[i] = Array<Vector> (r1x) ;
			for (auto &&j : range (0 ,r1x)) {
				ret.mCosSin[i][j][0] = self.mCosSin[i][j][0] ;
				ret.mCosSin[i][j][1] = -self.mCosSin[i][j][1] ;
				ret.mCosSin[i][j][2] = 0 ;
				ret.mCosSin[i][j][3] = 0 ;
			}
		}
		ret.mScale[0] = self.mScale[1] ;
		ret.mScale[1] = self.mScale[0] ;
		ret.mScale[2] = self.mScale[2] ;
		return move (ret) ;
	}
} ;

exports VFat<FFTransformHolder> FFTransformHolder::hold (VR<FFTransformLayout> that) {
	return VFat<FFTransformHolder> (FFTransformImplHolder () ,that) ;
}

exports CFat<FFTransformHolder> FFTransformHolder::hold (CR<FFTransformLayout> that) {
	return CFat<FFTransformHolder> (FFTransformImplHolder () ,that) ;
}
} ;
