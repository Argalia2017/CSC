#pragma once

#ifndef __CSC_ALGORITHM__
#define __CSC_ALGORITHM__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_images.hpp"
#include "csc_geometry.hpp"

namespace CSC {
class PrimeSieveAlgorithm {
private:
	BitSet<> mPrimeSet ;

public:
	PrimeSieveAlgorithm () = delete ;

	explicit PrimeSieveAlgorithm (const LENGTH &len) {
		initialize (len) ;
	}

	const BitSet<> &query () const leftvalue {
		return mPrimeSet ;
	}

private:
	void initialize (const LENGTH &len) ;
} ;

inline exports void PrimeSieveAlgorithm::initialize (const LENGTH &len) {
	mPrimeSet = BitSet<> (len) ;
	mPrimeSet.fill (BYTE (0XAA)) ;
	mPrimeSet[1] = FALSE ;
	mPrimeSet[2] = TRUE ;
	const auto r1x = (MathProc::sqrt (mPrimeSet.size ()) - 2) / 2 + 1 ;
	for (auto &&i : _RANGE_ (0 ,r1x)) {
		INDEX ix = i * 2 + 3 ;
		const auto r2x = ix * 2 ;
		_DEBUG_ASSERT_ (r2x > 0) ;
		const auto r3x = MathProc::square (ix) ;
		const auto r4x = (mPrimeSet.size () - r3x) / r2x + 1 ;
		for (auto &&j : _RANGE_ (0 ,r4x)) {
			INDEX jx = j * r2x + r3x ;
			mPrimeSet[jx] = FALSE ;
		}
	}
}

class DisjointTable {
private:
	class Node {
	private:
		friend DisjointTable ;
		INDEX mUp ;
		LENGTH mWidth ;

	public:
		inline Node () {
			mUp = VAR_NONE ;
			mWidth = 0 ;
		}
	} ;

private:
	Array<Node> mTable ;

public:
	DisjointTable () = delete ;

	explicit DisjointTable (const LENGTH &len) {
		mTable = Array<Node> (len) ;
	}

	INDEX lead (const INDEX &index) popping {
		INDEX ret = index ;
		if switch_case (TRUE) {
			if (mTable[ret].mUp != VAR_NONE)
				discard ;
			mTable[ret].mUp = ret ;
			mTable[ret].mWidth = 1 ;
		}
		while (TRUE) {
			if (mTable[ret].mUp == ret)
				break ;
			ret = mTable[ret].mUp ;
		}
		for (INDEX i = index ,it ,ie = ret ; i != ie ; i = it) {
			it = mTable[i].mUp ;
			mTable[i].mUp = ret ;
			if (it == ret)
				continue ;
			mTable[it].mWidth -= mTable[i].mWidth ;
		}
		return _MOVE_ (ret) ;
	}

	void joint (const INDEX &index1 ,const INDEX &index2) {
		INDEX ix = lead (index1) ;
		INDEX iy = lead (index2) ;
		if (ix == iy)
			return ;
		mTable[iy].mUp = ix ;
		mTable[ix].mWidth += mTable[iy].mWidth ;
	}

	Array<BitSet<>> closure () popping {
		const auto r1x = map_of_closure () ;
		Array<BitSet<>> ret = Array<BitSet<>> (r1x.length ()) ;
		for (auto &&i : _RANGE_ (0 ,ret.length ()))
			ret[i] = BitSet<> (mTable.size ()) ;
		for (auto &&i : _RANGE_ (0 ,mTable.length ())) {
			if (mTable[i].mUp == VAR_NONE)
				continue ;
			INDEX ix = lead (i) ;
			INDEX iy = r1x.map (ix) ;
			ret[iy][i] = TRUE ;
		}
		return _MOVE_ (ret) ;
	}

private:
	Set<INDEX> map_of_closure () const {
		Set<INDEX> ret = Set<INDEX> (mTable.length ()) ;
		for (auto &&i : _RANGE_ (0 ,mTable.length ())) {
			if (mTable[i].mUp != i)
				continue ;
			const auto r1x = ret.length () ;
			ret.add (i ,r1x) ;
		}
		return _MOVE_ (ret) ;
	}
} ;

template <class REAL>
class KMPAlgorithm {
private:
	Array<INDEX> mNext ;
	Array<REAL> mPattern ;

public:
	KMPAlgorithm () = delete ;

	explicit KMPAlgorithm (const PhanBuffer<const REAL> &pattern) {
		initialize (pattern) ;
	}

	INDEX query (const PhanBuffer<const REAL> &target ,const INDEX &seg) const {
		_DEBUG_ASSERT_ (seg >= 0 && seg < target.size ()) ;
		INDEX ix = seg ;
		INDEX iy = 0 ;
		if (target.size () - seg < mNext.length ())
			return VAR_NONE ;
		while (TRUE) {
			if (ix >= target.size ())
				break ;
			if (iy >= mNext.length ())
				break ;
			while (TRUE) {
				if (iy == VAR_NONE)
					break ;
				if (target[ix] == mPattern[iy])
					break ;
				iy = mNext[iy] ;
			}
			ix++ ;
			iy++ ;
		}
		if (iy < mNext.length ())
			return VAR_NONE ;
		return ix - mNext.length () ;
	}

private:
	void initialize (const PhanBuffer<const REAL> &pattern) ;

	INDEX find_next (const INDEX &slow ,const INDEX &fast) const {
		for (INDEX i = fast ,it ; i != VAR_NONE ; i = it) {
			it = mNext[i] ;
			if (mPattern[i] != mPattern[slow])
				return i ;
		}
		return VAR_NONE ;
	}
} ;

template <class REAL>
inline exports void KMPAlgorithm<REAL>::initialize (const PhanBuffer<const REAL> &pattern) {
	mNext = Array<INDEX> (pattern.size ()) ;
	mPattern = Array<REAL> (pattern.size ()) ;
	INDEX ix = 0 ;
	INDEX iy = VAR_NONE ;
	mNext[ix] = VAR_NONE ;
	mPattern[ix] = pattern[ix] ;
	while (TRUE) {
		if (ix >= pattern.size () - 1)
			break ;
		while (TRUE) {
			if (iy == VAR_NONE)
				break ;
			if (pattern[ix] == pattern[iy])
				break ;
			iy = mNext[iy] ;
		}
		ix++ ;
		iy++ ;
		mNext[ix] = find_next (ix ,iy) ;
		mPattern[ix] = pattern[ix] ;
	}
}

template <class REAL>
class DijstraAlgorithm {
private:
	Array<INDEX> mPrev ;
	Array<REAL> mDistance ;
	INDEX mRoot ;

public:
	DijstraAlgorithm () = delete ;

	explicit DijstraAlgorithm (const Bitmap<REAL> &adjacency ,const INDEX &root_) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		initialize (adjacency ,root_) ;
	}

	REAL query (const INDEX &index) const {
		return mDistance[index] ;
	}

	Array<INDEX> query_path (const INDEX &index) const {
		Array<INDEX> ret = Array<INDEX> (query_path_depth (index)) ;
		INDEX iw = ret.length () ;
		for (INDEX i = index ,it ; i != VAR_NONE ; i = it) {
			it = mPrev[i] ;
			ret[--iw] = i ;
		}
		_DEBUG_ASSERT_ (iw == 0) ;
		return _MOVE_ (ret) ;
	}

private:
	void initialize (const Bitmap<REAL> &adjacency ,const INDEX &root_) ;

	LENGTH query_path_depth (const INDEX &index) const {
		LENGTH ret = 0 ;
		for (INDEX i = index ,it ; i != VAR_NONE ; i = it) {
			it = mPrev[i] ;
			ret++ ;
		}
		return _MOVE_ (ret) ;
	}
} ;

template <class REAL>
inline exports void DijstraAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency ,const INDEX &root_) {
	class Lambda final
		:private Proxy {
	private:
		DijstraAlgorithm &mContext ;
		const Bitmap<REAL> &mAdjacency ;

		INDEX mRoot ;
		Array<INDEX> mPrev ;
		Array<REAL> mDistance ;
		Priority<REAL ,INDEX> mPriority ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;

	public:
		inline explicit Lambda (DijstraAlgorithm &context_ ,const Bitmap<REAL> &adjancency ,const INDEX &root_)
			: mContext (context_) ,mAdjacency (adjancency) ,mRoot (root_) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mPrev = Array<INDEX> (mAdjacency.cx ()) ;
			mPrev.fill (VAR_NONE) ;
			mDistance = Array<REAL> (mAdjacency.cx ()) ;
			mDistance[mRoot] = REAL (0) ;
			mPriority = Priority<REAL ,INDEX> (mAdjacency.cy ()) ;
			mPriority.add (mDistance[mRoot] ,mRoot) ;
			mXVisit = BitSet<> (mAdjacency.cx ()) ;
			mXVisit[mRoot] = TRUE ;
			mYVisit = BitSet<> (mAdjacency.cy ()) ;
		}

		inline void generate () {
			while (TRUE) {
				if (mPriority.empty ())
					break ;
				const auto r1x = mPriority[mPriority.head ()].sid ;
				mPriority.take () ;
				update_distance (r1x) ;
			}
		}

		inline void update_distance (const INDEX &y) {
			if (mYVisit[y])
				return ;
			mYVisit[y] = TRUE ;
			for (auto &&i : _RANGE_ (0 ,mPrev.length ())) {
				if (i == y)
					continue ;
				if (mAdjacency[y][i] < REAL (0))
					continue ;
				const auto r1x = mDistance[y] + mAdjacency[y][i] ;
				if (mXVisit[i])
					if (mDistance[i] <= r1x)
						continue ;
				mPrev[i] = y ;
				mDistance[i] = r1x ;
				mXVisit[i] = TRUE ;
				mPriority.add (mDistance[i] ,i) ;
			}
		}

		inline void refresh () {
			mContext.mPrev = _MOVE_ (mPrev) ;
			mContext.mDistance = _MOVE_ (mDistance) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	_CALL_ (Lambda (DEREF[this] ,adjacency ,root_)) ;
}

template <class REAL>
class KMeansAlgorithm {
private:
	Deque<BitSet<>> mClusterList ;

public:
	KMeansAlgorithm () = delete ;

	explicit KMeansAlgorithm (const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) {
		_DEBUG_ASSERT_ (dataset.length () >= 2) ;
		_DEBUG_ASSERT_ (center.length () >= 2) ;
		initialize (dataset ,distance ,center) ;
	}

	const Deque<BitSet<>> &query () const leftvalue {
		return mClusterList ;
	}

private:
	void initialize (const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) ;
} ;

template <class REAL>
inline exports void KMeansAlgorithm<REAL>::initialize (const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) {
	class Lambda final
		:private Proxy {
	private:
		KMeansAlgorithm &mContext ;
		const Set<REAL> &mDataSet ;
		const Function<REAL (const REAL & ,const REAL &)> &mDistanceFunc ;
		const Array<REAL> &mCenter ;
		const REAL mTolerance ;
		const REAL mInfinity ;

		SoftList<REAL> mCurrCenterList ;
		SoftList<REAL> mNextCenterList ;
		Set<INDEX> mCenterMoveSet ;
		Deque<BitSet<>> mClusterList ;
		Set<INDEX> mClusterMappingSet ;
		ARRAY3<REAL> mConvergence ;

	public:
		inline explicit Lambda (KMeansAlgorithm &context_ ,const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center)
			: mContext (context_) ,mDistanceFunc (distance) ,mDataSet (dataset) ,mCenter (center) ,mTolerance (1E-6) ,mInfinity (VAL_INF) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mCurrCenterList = SoftList<REAL> (mCenter.length ()) ;
			mNextCenterList = SoftList<REAL> (mCenter.length ()) ;
			mCurrCenterList.appand (mCenter) ;
			mCenterMoveSet = Set<INDEX> (mCenter.length ()) ;
			mClusterList = Deque<BitSet<>> () ;
			mClusterMappingSet = Set<INDEX> () ;
			mConvergence.fill (mInfinity) ;
		}

		inline void generate () {
			while (TRUE) {
				update_cluster_set () ;
				update_convergence () ;
				if (reach_convergence ())
					break ;
				update_center_list () ;
			}
		}

		inline void update_cluster_set () {
			mClusterList.clear () ;
			mClusterMappingSet.clear () ;
			for (auto &&i : mDataSet) {
				INDEX ix = closest_center_of_point (i) ;
				INDEX iy = mClusterMappingSet.map (ix) ;
				if switch_case (TRUE) {
					if (iy != VAR_NONE)
						discard ;
					iy = mClusterList.insert () ;
					mClusterMappingSet.add (ix ,iy) ;
					mClusterList[iy] = BitSet<> (mDataSet.size ()) ;
				}
				INDEX jx = mDataSet.at (i) ;
				mClusterList[iy][jx] = TRUE ;
			}
			for (auto &&i : mClusterMappingSet) {
				INDEX ix = mNextCenterList.insert () ;
				mNextCenterList[ix] = average_center (mClusterList[i.sid]) ;
				mCenterMoveSet.add (i.key ,ix) ;
			}
		}

		inline INDEX closest_center_of_point (const REAL &point) const {
			INDEX ret = VAR_NONE ;
			auto rax = REAL () ;
			for (auto &&i : mCurrCenterList) {
				const auto r1x = mDistanceFunc (point ,i) ;
				if (ret != VAR_NONE)
					if (rax <= r1x)
						continue ;
				ret = mCurrCenterList.at (i) ;
				rax = r1x ;
			}
			return _MOVE_ (ret) ;
		}

		inline REAL average_center (const BitSet<> &cluster) const {
			REAL ret = REAL (0) ;
			const auto r1x = cluster.length () ;
			_DEBUG_ASSERT_ (r1x != 0) ;
			for (auto &&i : cluster)
				ret += mDataSet[i] ;
			ret *= MathProc::inverse (REAL (r1x)) ;
			return _MOVE_ (ret) ;
		}

		inline void update_convergence () {
			INDEX ix = mConvergence.length () - 1 ;
			for (auto &&i : _RANGE_ (0 ,ix))
				mConvergence[i] = mConvergence[i + 1] ;
			mConvergence[ix] = +mInfinity ;
			if (mCurrCenterList.length () != mNextCenterList.length ())
				return ;
			mConvergence[ix] = REAL (0) ;
			for (auto &&i : mCenterMoveSet) {
				const auto r1x = mDistanceFunc (mCurrCenterList[i.key] ,mNextCenterList[i.sid]) ;
				mConvergence[ix] = MathProc::maxof (mConvergence[ix] ,r1x) ;
			}
		}

		inline BOOL reach_convergence () const {
			INDEX ix = mConvergence.length () - 1 ;
			for (auto &&i : _RANGE_ (0 ,ix))
				if (mConvergence[i] > mConvergence[i + 1])
					return FALSE ;
			if (MathProc::abs (mConvergence[0] - mConvergence[ix]) >= mTolerance)
				return FALSE ;
			return TRUE ;
		}

		inline void update_center_list () {
			mCurrCenterList.clear () ;
			mCurrCenterList.appand (mNextCenterList) ;
			mNextCenterList.clear () ;
			mCenterMoveSet.clear () ;
		}

		inline void refresh () {
			mContext.mClusterList = _MOVE_ (mClusterList) ;
		}
	} ;
	_CALL_ (Lambda (DEREF[this] ,dataset ,distance ,center)) ;
}

template <class REAL>
class KMHungarianAlgorithm {
private:
	REAL mWeight ;
	Array<ARRAY2<INDEX>> mMatch ;

public:
	KMHungarianAlgorithm () = delete ;

	explicit KMHungarianAlgorithm (const Bitmap<REAL> &adjacency) {
		initialize (adjacency) ;
	}

	REAL query () const {
		return mWeight ;
	}

	const Array<ARRAY2<INDEX>> &query_match () const leftvalue {
		return mMatch ;
	}

private:
	void initialize (const Bitmap<REAL> &adjacency) ;
} ;

template <class REAL>
inline exports void KMHungarianAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency) {
	class Lambda final
		:private Proxy {
	private:
		KMHungarianAlgorithm &mContext ;
		const Bitmap<REAL> &mAdjacency ;
		const REAL mTolerance ;
		const REAL mInfinity ;

		Array<INDEX> mXYLink ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;
		Array<REAL> mXWeight ;
		Array<REAL> mYWeight ;
		ARRAY2<REAL> mLackWeight ;

		Deque<ARRAY2<INDEX>> mTempStack ;
		BOOL mTempRet ;
		EFLAG mTempState ;

	public:
		inline explicit Lambda (KMHungarianAlgorithm &context_ ,const Bitmap<REAL> &adjacency)
			: mContext (context_) ,mAdjacency (adjacency) ,mTolerance (1E-6) ,mInfinity (VAL_INF) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mXYLink = Array<INDEX> (mAdjacency.cx ()) ;
			mXYLink.fill (VAR_NONE) ;
			mXVisit = BitSet<> (mAdjacency.cx ()) ;
			mYVisit = BitSet<> (mAdjacency.cy ()) ;
			mXWeight = Array<REAL> (mAdjacency.cx ()) ;
			mXWeight.fill (REAL (0)) ;
			mYWeight = Array<REAL> (mAdjacency.cy ()) ;
			mYWeight.fill (REAL (0)) ;
			for (auto &&i : mAdjacency.range ()) {
				_DYNAMIC_ASSERT_ (mAdjacency[i] >= REAL (0)) ;
				mYWeight[i[0]] = MathProc::maxof (mYWeight[i[0]] ,mAdjacency[i]) ;
			}
		}

		inline void generate () {
			for (auto &&i : _RANGE_ (0 ,mAdjacency.cy ())) {
				while (TRUE) {
					mXVisit.clear () ;
					mYVisit.clear () ;
					update_lack_weight (i) ;
					if (mLackWeight[1] >= +mInfinity)
						break ;
					for (auto &&j : mXVisit)
						mXWeight[j] += mLackWeight[1] ;
					for (auto &&j : mYVisit)
						mYWeight[j] -= mLackWeight[1] ;
				}
			}
		}

		inline void update_lack_weight (const INDEX &y) {
			/*
			*	inline void update_lack_weight_e0 (const INDEX &y) {
			*		//@info: $0
			*		mLackWeight[0] = 0 ;
			*		mLackWeight[1] = +mInfinity ;
			*		//@info: $1
			*		auto rax = FALSE ;
			*		update_lack_weight_e7 (0 ,y ,rax) ;
			*		//@info: $18
			*		if (rax) {
			*			//@info: $19
			*			mLackWeight[0] = 0 ;
			*			mLackWeight[1] = +mInfinity ;
			*		}
			*		//@info: $20
			*	}
			*
			*	inline void update_lack_weight_e7 (const INDEX &stack_x ,const INDEX &stack_y ,BOOL &stack_ret) {
			*		//@info: $7
			*		if (stack_y == VAR_NONE) {
			*			//@info: $2
			*			stack_ret = TRUE ;
			*			//@info: $17
			*			return ;
			*		}
			*		//@info: $3
			*		mYVisit[stack_y] = TRUE ;
			*		stack_x = 0 ;
			*		//@info: $4
			*		while (stack_x < mAdjacency.cx ()) {
			*			//@info: $5
			*			if (!mXVisit[stack_x]) {
			*				//@info: $6
			*				mLackWeight[0] = mYWeight[stack_y] + mXWeight[stack_x] - mAdjacency[stack_y][stack_x] ;
			*				//@info: $9
			*				if (mLackWeight[0] < mTolerance) {
			*					//@info: $8
			*					mXVisit[stack_x] = TRUE ;
			*					update_lack_weight_e7 (0 ,mXYLink[stack_x] ,stack_ret) ;
			*					//@info: $10
			*					if (stack_ret) {
			*						//@info: $11
			*						mXYLink[stack_x] = stack_y ;
			*						stack_ret = TRUE ;
			*						//@info: $17
			*						return ;
			*					}
			*				} else {
			*					//@info: $14
			*					mLackWeight[1] = MathProc::minof (mLackWeight[1] ,mLackWeight[0]) ;
			*				}
			*			}
			*			//@info: $15
			*			stack_x++ ;
			*		}
			*		//@info: $16
			*		stack_ret = FALSE ;
			*		//@info: $17
			*		return ;
			*	}
			*/
			static const auto M_STATE = PACK<EFLAG[22]> ({
				EFLAG (0) ,EFLAG (1) ,EFLAG (2) ,EFLAG (3) ,EFLAG (4) ,
				EFLAG (5) ,EFLAG (6) ,EFLAG (7) ,EFLAG (8) ,EFLAG (9) ,
				EFLAG (10) ,EFLAG (11) ,EFLAG (12) ,EFLAG (13) ,EFLAG (14) ,
				EFLAG (15) ,EFLAG (16) ,EFLAG (17) ,EFLAG (18) ,EFLAG (19) ,
				EFLAG (20) ,EFLAG (21)}) ;
			mTempStack.clear () ;
			mTempState = M_STATE.P1[21] ;
			INDEX ix = VAR_NONE ;
			while (TRUE) {
				if (mTempState == M_STATE.P1[0])
					break ;
				auto fax = TRUE ;
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[21]))
						discard ;
					mLackWeight[0] = 0 ;
					mLackWeight[1] = +mInfinity ;
					mTempState = M_STATE.P1[1] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[1]))
						discard ;
					mTempRet = FALSE ;
					mTempStack.add (ARRAY2<INDEX> {0 ,y}) ;
					mTempState = M_STATE.P1[7] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[2]))
						discard ;
					mTempRet = TRUE ;
					mTempState = M_STATE.P1[17] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[3]))
						discard ;
					mYVisit[mTempStack[ix][1]] = TRUE ;
					mTempStack[ix][0] = 0 ;
					mTempState = M_STATE.P1[4] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[4]))
						discard ;
					auto &r1x = _SWITCH_ (
						(mTempStack[ix][0] < mAdjacency.cx ()) ? M_STATE.P1[5] :
						M_STATE.P1[16]) ;
					mTempState = r1x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[5]))
						discard ;
					auto &r2x = _SWITCH_ (
						(mXVisit[mTempStack[ix][0]]) ? M_STATE.P1[15] :
						M_STATE.P1[6]) ;
					mTempState = r2x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[6]))
						discard ;
					mLackWeight[0] = mYWeight[mTempStack[ix][1]] + mXWeight[mTempStack[ix][0]] - mAdjacency[mTempStack[ix][1]][mTempStack[ix][0]] ;
					mTempState = M_STATE.P1[9] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[7]))
						discard ;
					ix = mTempStack.tail () ;
					auto &r3x = _SWITCH_ (
						(mTempStack[ix][1] == VAR_NONE) ? M_STATE.P1[2] :
						M_STATE.P1[3]) ;
					mTempState = r3x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[8]))
						discard ;
					mXVisit[mTempStack[ix][0]] = TRUE ;
					mTempStack.add (ARRAY2<INDEX> {0 ,mXYLink[mTempStack[ix][0]]}) ;
					mTempState = M_STATE.P1[7] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[9]))
						discard ;
					auto &r4x = _SWITCH_ (
						(mLackWeight[0] < mTolerance) ? M_STATE.P1[8] :
						M_STATE.P1[14]) ;
					mTempState = r4x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[10]))
						discard ;
					ix = mTempStack.tail () ;
					auto &r5x = _SWITCH_ (
						mTempRet ? M_STATE.P1[11] :
						M_STATE.P1[15]) ;
					mTempState = r5x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[11]))
						discard ;
					mXYLink[mTempStack[ix][0]] = mTempStack[ix][1] ;
					mTempRet = TRUE ;
					mTempState = M_STATE.P1[17] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[14]))
						discard ;
					mLackWeight[1] = MathProc::minof (mLackWeight[1] ,mLackWeight[0]) ;
					mTempState = M_STATE.P1[15] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[15]))
						discard ;
					mTempStack[ix][0]++ ;
					mTempState = M_STATE.P1[4] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[16]))
						discard ;
					mTempRet = FALSE ;
					mTempState = M_STATE.P1[17] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[17]))
						discard ;
					mTempStack.pop () ;
					auto &r6x = _SWITCH_ (
						(mTempStack.length () > 0) ? M_STATE.P1[10] :
						M_STATE.P1[18]) ;
					mTempState = r6x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[18]))
						discard ;
					auto &r7x = _SWITCH_ (
						mTempRet ? M_STATE.P1[19] :
						M_STATE.P1[20]) ;
					mTempState = r7x ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[19]))
						discard ;
					mLackWeight[0] = 0 ;
					mLackWeight[1] = +mInfinity ;
					mTempState = M_STATE.P1[20] ;
				}
				if switch_case (fax) {
					if (!(mTempState == M_STATE.P1[20]))
						discard ;
					mTempState = M_STATE.P1[0] ;
				}
			}
		}

		inline void refresh () {
			mContext.mWeight = best_weight () ;
			mContext.mMatch = best_match () ;
		}

		inline REAL best_weight () const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,mXYLink.length ()))
				ret += mAdjacency[mXYLink[i]][i] ;
			return _MOVE_ (ret) ;
		}

		inline Array<ARRAY2<INDEX>> best_match () const {
			Array<ARRAY2<INDEX>> ret = Array<ARRAY2<INDEX>> (best_match_depth ()) ;
			INDEX iw = 0 ;
			for (auto &&i : _RANGE_ (0 ,mXYLink.length ())) {
				if (mXYLink[i] == VAR_NONE)
					continue ;
				INDEX ix = iw++ ;
				ret[ix][0] = mXYLink[i] ;
				ret[ix][1] = i ;
			}
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
			return _MOVE_ (ret) ;
		}

		inline LENGTH best_match_depth () const {
			LENGTH ret = 0 ;
			for (auto &&i : mXYLink)
				ret += _EBOOL_ (i != VAR_NONE) ;
			return _MOVE_ (ret) ;
		}
	} ;
	_CALL_ (Lambda (DEREF[this] ,adjacency)) ;
}

template <class REAL>
class BFGSAlgorithm {
private:
	Array<REAL> mDX ;
	REAL mDXLoss ;

public:
	BFGSAlgorithm () = delete ;

	explicit BFGSAlgorithm (const Function<REAL (const Array<REAL> &)> &loss ,const Function<void (const Array<REAL> & ,Array<REAL> &)> &gradient ,const Array<REAL> &fdx) {
		initialize (loss ,gradient ,fdx) ;
	}

	const Array<REAL> &query () const leftvalue {
		return mDX ;
	}

	REAL query_loss () const {
		return mDXLoss ;
	}

private:
	void initialize (const Function<REAL (const Array<REAL> &)> &loss ,const Array<REAL> &fdx) ;
} ;

template <class REAL>
inline exports void BFGSAlgorithm<REAL>::initialize (const Function<REAL (const Array<REAL> &)> &loss ,const Array<REAL> &fdx) {
	class Lambda final
		:private Proxy {
	private:
		BFGSAlgorithm &mContext ;
		const Function<REAL (const Array<REAL> &)> &mLossFunc ;
		const Function<void (const Array<REAL> & ,Array<REAL> &)> &mGradientProc ;
		const Array<REAL> &mFDX ;
		const REAL mTolerance ;
		const REAL mDXLambdaFirst ;
		const REAL mDXLambdaPower ;
		const REAL mDXLambdaC1 ;
		const REAL mDXLambdaC2 ;

		Array<REAL> mDX ;
		Bitmap<REAL> mDM ;
		Array<REAL> mDG ;
		ARRAY3<REAL> mDXLoss ;
		ARRAY3<REAL> mDXLambda ;
		Array<REAL> mIX ;
		Bitmap<REAL> mIM ;
		Array<REAL> mIG ;
		Array<REAL> mIS ;
		Array<REAL> mIY ;
		Array<REAL> mSX ;

	public:
		inline explicit Lambda (BFGSAlgorithm &context_ ,const Function<REAL (const Array<REAL> &)> &loss ,const Function<void (const Array<REAL> & ,Array<REAL> &)> &gradient ,const Array<REAL> &fdx)
			: mContext (context_) ,mLossFunc (loss) ,mGradientProc (gradient) ,mFDX (fdx) ,mTolerance (1E-6) ,mDXLambdaFirst (1000) ,mDXLambdaPower (0.618) ,mDXLambdaC1 (1E-4) ,mDXLambdaC2 (0.9) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mDX = mFDX ;
			mDM = Bitmap<REAL> (mDX.size () ,mDX.size ()) ;
			mDM.fill (REAL (0)) ;
			for (auto &&i : _RANGE_ (0 ,mDM.cy ()))
				mDM[i][i] = REAL (1) ;
			mDG = Array<REAL> (mDX.size ()) ;
			mIX = Array<REAL> (mDX.size ()) ;
			mIM = Bitmap<REAL> (mDX.size () ,mDX.size ()) ;
			mIG = Array<REAL> (mDX.size ()) ;
			mIS = Array<REAL> (mDX.size ()) ;
			mIY = Array<REAL> (mDX.size ()) ;
			mSX = Array<REAL> (mDX.size ()) ;
			compute_gradient_of_loss (mDX ,mDG ,mSX) ;
		}

		inline void generate () {
			while (TRUE) {
				update_is_and_ig () ;
				if (current_convergence () < mTolerance)
					break ;
				update_iy_and_dg () ;
				update_dm () ;
			}
		}

		inline void compute_gradient_of_loss (const Array<REAL> &dx ,Array<REAL> &dg ,Array<REAL> &sx) const {
			for (auto &&i : _RANGE_ (0 ,dx.length ()))
				sx[i] = dx[i] ;
			mGradientProc (sx ,dg) ;
		}

		inline void update_is_and_ig () {
			for (auto &&i : _RANGE_ (0 ,mDG.length ()))
				mIS[i] = -math_matrix_mul (mDM ,i ,mDG) ;
			const auto r1x = math_vector_dot (mDG ,mIS) ;
			mDXLoss[0] = MathProc::abs (mLossFunc (mDX)) ;
			mDXLoss[2] = mDXLoss[0] ;
			mDXLambda[0] = REAL (0) ;
			mDXLambda[1] = mDXLambdaFirst ;
			mDXLambda[2] = mDXLambda[0] ;
			while (TRUE) {
				if (mDXLambda[1] < mTolerance)
					break ;
				if (mDXLoss[2] < mTolerance)
					break ;
				for (auto &&i : _RANGE_ (0 ,mIX.length ()))
					mIX[i] = mDX[i] + mIS[i] * mDXLambda[1] ;
				mDXLoss[1] = MathProc::abs (mLossFunc (mIX)) ;
				if switch_case (TRUE) {
					if (mDXLoss[1] - mDXLoss[0] > mDXLambda[1] * mDXLambdaC1 * r1x)
						discard ;
					compute_gradient_of_loss (mIX ,mIG ,mSX) ;
					if (MathProc::abs (math_vector_dot (mIG ,mIS)) > -mDXLambdaC2 * r1x)
						discard ;
					mDXLoss[2] = REAL (0) ;
				}
				if switch_case (TRUE) {
					if (mDXLoss[1] >= mDXLoss[2])
						discard ;
					mDXLoss[2] = mDXLoss[1] ;
					mDXLambda[2] = mDXLambda[1] ;
				}
				mDXLambda[1] *= mDXLambdaPower ;
			}
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(mDXLoss[0] >= mDXLoss[2]))
					discard ;
				auto &r2x = _SWITCH_ (
					(mDXLoss[2] > REAL (0)) ? mDXLoss[2] :
					mDXLoss[1]) ;
				mDXLoss[0] = r2x ;
				_SWAP_ (mDX ,mIX) ;
				compute_gradient_of_loss (mDX ,mIG ,mSX) ;
			}
			if switch_case (fax) {
				mIG.fill (REAL (0)) ;
			}
		}

		inline REAL math_matrix_mul (const Bitmap<REAL> &mat ,const INDEX &y ,const Array<REAL> &v) const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,v.length ()))
				ret += mat[y][i] * v[i] ;
			return _MOVE_ (ret) ;
		}

		inline REAL math_vector_dot (const Array<REAL> &v1 ,const Array<REAL> &v2) const {
			_DEBUG_ASSERT_ (v1.length () == v2.length ()) ;
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,v1.length ()))
				ret += v1[i] * v2[i] ;
			return _MOVE_ (ret) ;
		}

		inline REAL current_convergence () const {
			REAL ret = REAL (0) ;
			for (auto &&i : mIG)
				ret += MathProc::square (i) ;
			ret = MathProc::sqrt (ret) ;
			return _MOVE_ (ret) ;
		}

		inline void update_iy_and_dg () {
			for (auto &&i : _RANGE_ (0 ,mIG.length ())) {
				mIY[i] = mIG[i] - mDG[i] ;
				mDG[i] = mIG[i] ;
			}
		}

		inline void update_dm () {
			const auto r1x = MathProc::inverse (math_vector_dot (mIY ,mIS)) ;
			for (auto &&i : mDM.range ()) {
				const auto r2x = hessian_matrix_each (i[0] ,i[1] ,r1x) ;
				mIM[i] = r2x + mIS[i[0]] * mIS[i[1]] * r1x ;
			}
			_SWAP_ (mDM ,mIM) ;
		}

		inline REAL hessian_matrix_each (const INDEX &y ,const INDEX &x ,const REAL &ys) const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,mDM.cy ())) {
				const auto r1x = hessian_matrix_each_factor (x ,i ,ys) ;
				ret += r1x * (-mIS[y] * mIY[i] * ys) ;
				if (i == y)
					continue ;
				ret += r1x ;
			}
			return _MOVE_ (ret) ;
		}

		inline REAL hessian_matrix_each_factor (const INDEX &x ,const INDEX &z ,const REAL &ys) const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,mDM.cx ())) {
				ret += mDM[z][i] * (-mIY[i] * mIS[x] * ys) ;
				if (i == x)
					continue ;
				ret += mDM[z][i] ;
			}
			return _MOVE_ (ret) ;
		}

		inline void refresh () {
			mContext.mDX = _MOVE_ (mDX) ;
			mContext.mDXLoss = mDXLoss[0] ;
		}
	} ;
	_CALL_ (Lambda (DEREF[this] ,loss ,fdx)) ;
}

template <class REAL>
class KDTreeAlgorithm {
private:
	class Node {
	private:
		friend KDTreeAlgorithm ;
		REAL mKey ;
		INDEX mLeaf ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		inline Node () = delete ;

		inline implicit Node (const REAL &key ,const INDEX &leaf ,const INDEX &left ,const INDEX &right)
			:mKey (_MOVE_ (key)) ,mLeaf (leaf) ,mLeft (left) ,mRight (right) {}
	} ;

private:
	Array<ARRAY3<REAL>> mVertex ;
	ARRAY3<INDEX> mNextRot ;
	ARRAY3<ARRAY2<REAL>> mBound ;
	Allocator<Node ,SAUTO> mKDTree ;
	INDEX mRoot ;

public:
	KDTreeAlgorithm () = delete ;

	explicit KDTreeAlgorithm (const Array<ARRAY3<REAL>> &vertex) {
		_DEBUG_ASSERT_ (vertex.length () > 0) ;
		initialize (vertex) ;
	}

	Deque<INDEX> query (const ARRAY3<REAL> &point ,const REAL &width) const {
		_DEBUG_ASSERT_ (width > REAL (0)) ;
		Deque<INDEX> ret ;
		auto rax = ARRAY3<ARRAY2<REAL>> () ;
		rax[0][0] = MathProc::maxof ((point[0] - width) ,mBound[0][0]) ;
		rax[0][1] = MathProc::minof ((point[0] + width) ,mBound[0][1]) ;
		rax[1][0] = MathProc::maxof ((point[1] - width) ,mBound[1][0]) ;
		rax[1][1] = MathProc::minof ((point[1] + width) ,mBound[1][1]) ;
		rax[2][0] = MathProc::maxof ((point[2] - width) ,mBound[2][0]) ;
		rax[2][1] = MathProc::minof ((point[2] + width) ,mBound[2][1]) ;
		compute_search_range (point ,width ,mRoot ,0 ,rax ,ret) ;
		return _MOVE_ (ret) ;
	}

	Array<PACK<INDEX ,REAL>> query_nearst (const ARRAY3<REAL> &point ,const LENGTH &count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		const auto r1x = first_count_vertex (point ,count) ;
		const auto r2x = r1x.range_sort () ;
		Array<PACK<INDEX ,REAL>> ret = Array<PACK<INDEX ,REAL>> (count) ;
		for (auto &&i : _RANGE_ (0 ,ret.length ())) {
			ret[i].P1 = r2x[i] ;
			ret[i].P2 = r1x[r2x[i]] ;
		}
		compute_search_range (point ,mRoot ,0 ,ret) ;
		return _MOVE_ (ret) ;
	}

private:
	void initialize (const Array<ARRAY3<REAL>> &vertex) ;

	REAL distance_of_point (const ARRAY3<REAL> &a ,const ARRAY3<REAL> &b) const {
		const auto r1x = Vector<REAL> {a ,REAL (0)} ;
		const auto r2x = Vector<REAL> {b ,REAL (0)} ;
		return (r1x - r2x).magnitude () ;
	}

	void compute_search_range (const ARRAY3<REAL> &point ,const REAL &width ,const INDEX &curr ,const INDEX &rot ,ARRAY3<ARRAY2<REAL>> &bound ,Deque<INDEX> &out) const {
		auto fax = TRUE ;
		if switch_case (fax) {
			if (!(mKDTree[curr].mLeaf != VAR_NONE))
				discard ;
			if switch_case (TRUE) {
				INDEX ix = mKDTree[curr].mLeaf ;
				const auto r1x = distance_of_point (mVertex[ix] ,point) ;
				if (r1x > width)
					discard ;
				out.add (ix) ;
			}
		}
		if switch_case (fax) {
			if (!(mKDTree[curr].mLeaf == VAR_NONE))
				discard ;
			const auto r2x = mKDTree[curr].mKey ;
			if switch_case (TRUE) {
				if (r2x < bound[rot][0])
					discard ;
				const auto r3x = bound[rot][1] ;
				bound[rot][1] = MathProc::minof (bound[rot][1] ,r2x) ;
				compute_search_range (point ,width ,mKDTree[curr].mLeft ,mNextRot[rot] ,bound ,out) ;
				bound[rot][1] = r3x ;
			}
			if switch_case (TRUE) {
				if (r2x > bound[rot][1])
					discard ;
				const auto r4x = bound[rot][0] ;
				bound[rot][0] = MathProc::maxof (bound[rot][0] ,r2x) ;
				compute_search_range (point ,width ,mKDTree[curr].mRight ,mNextRot[rot] ,bound ,out) ;
				bound[rot][0] = r4x ;
			}
		}
	}

	Deque<REAL> first_count_vertex (const ARRAY3<REAL> &point ,const LENGTH &count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		Deque<REAL> ret = Deque<REAL> (count) ;
		for (auto &&i : _RANGE_ (0 ,count)) {
			const auto r1x = distance_of_point (mVertex[i] ,point) ;
			ret.add (r1x) ;
		}
		return _MOVE_ (ret) ;
	}

	void compute_search_range (const ARRAY3<REAL> &point ,const INDEX &curr ,const INDEX &rot ,Array<PACK<INDEX ,REAL>> &out) const {
		auto fax = TRUE ;
		if switch_case (fax) {
			if (!(mKDTree[curr].mLeaf != VAR_NONE))
				discard ;
			if switch_case (TRUE) {
				INDEX ix = mKDTree[curr].mLeaf ;
				const auto r1x = distance_of_point (mVertex[ix] ,point) ;
				INDEX jx = out.length () ;
				INDEX jy = jx - 1 ;
				while (TRUE) {
					if (jy < 0)
						break ;
					if (r1x >= out[jy].P2)
						break ;
					if (jx < out.length ())
						out[jx] = out[jy] ;
					jx = jy ;
					jy-- ;
				}
				if (jx >= out.length ())
					discard ;
				out[jx].P1 = ix ;
				out[jx].P2 = r1x ;
			}
		}
		if switch_case (fax) {
			if (!(mKDTree[curr].mLeaf == VAR_NONE))
				discard ;
			const auto r2x = mKDTree[curr].mKey ;
			if (r2x >= point[rot] - out[out.length () - 1].P2)
				compute_search_range (point ,mKDTree[curr].mLeft ,mNextRot[rot] ,out) ;
			if (r2x <= point[rot] + out[out.length () - 1].P2)
				compute_search_range (point ,mKDTree[curr].mRight ,mNextRot[rot] ,out) ;
		}
	}
} ;

template <class REAL>
inline exports void KDTreeAlgorithm<REAL>::initialize (const Array<ARRAY3<REAL>> &vertex) {
	class Lambda final
		:private Proxy {
	private:
		KDTreeAlgorithm &mContext ;
		const Array<ARRAY3<REAL>> &mVertex ;

		ARRAY3<INDEX> mNextRot ;
		ARRAY3<Array<INDEX>> mOrder ;
		ARRAY3<ARRAY2<REAL>> mBound ;
		Allocator<Node ,SAUTO> mKDTree ;
		INDEX mRoot ;
		INDEX mLatestIndex ;

		Array<INDEX> mTempOrder ;

	public:
		inline explicit Lambda (KDTreeAlgorithm &context_ ,const Array<ARRAY3<REAL>> &vertex)
			: mContext (context_) ,mVertex (vertex) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mNextRot = ARRAY3<INDEX> {1 ,2 ,0} ;
			for (auto &&i : _RANGE_ (0 ,mOrder.length ())) {
				const auto r1x = stack_of_order (i) ;
				mOrder[i] = r1x.range_sort () ;
			}
			mKDTree = Allocator<Node ,SAUTO> (mVertex.length ()) ;
			mRoot = VAR_NONE ;
		}

		Deque<INDEX> stack_of_order (const INDEX &rot) const {
			Deque<INDEX> ret = Deque<INDEX> (mVertex.length ()) ;
			for (auto &&i : mVertex)
				ret.add (i[rot]) ;
			return _MOVE_ (ret) ;
		}

		inline void generate () {
			update_bound () ;
			update_build_tree (mRoot ,0 ,0 ,mVertex.length ()) ;
			mRoot = mLatestIndex ;
		}

		inline void update_bound () {
			_DEBUG_ASSERT_ (mVertex.length () > 0) ;
			mBound[0][0] = mVertex[0][0] ;
			mBound[0][1] = mVertex[0][0] ;
			mBound[1][0] = mVertex[0][1] ;
			mBound[1][1] = mVertex[0][1] ;
			mBound[2][0] = mVertex[0][2] ;
			mBound[2][1] = mVertex[0][2] ;
			for (auto &&i : mVertex) {
				mBound[0][0] = MathProc::minof (mBound[0][0] ,i[0]) ;
				mBound[0][1] = MathProc::maxof (mBound[0][1] ,i[0]) ;
				mBound[1][0] = MathProc::minof (mBound[1][0] ,i[1]) ;
				mBound[1][1] = MathProc::maxof (mBound[1][1] ,i[1]) ;
				mBound[2][0] = MathProc::minof (mBound[2][0] ,i[2]) ;
				mBound[2][1] = MathProc::maxof (mBound[2][1] ,i[2]) ;
			}
		}

		void update_build_tree (const INDEX &curr ,const INDEX &rot ,const INDEX &seg ,const LENGTH &seg_len) {
			_DEBUG_ASSERT_ (seg_len > 0) ;
			_DEBUG_ASSERT_ (seg >= 0 && seg <= mVertex.size () - seg_len) ;
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(seg_len == 1))
					discard ;
				INDEX jx = mKDTree.alloc (REAL (0) ,mOrder[rot][seg] ,VAR_NONE ,VAR_NONE) ;
				mLatestIndex = jx ;
			}
			if switch_case (fax) {
				if (!(seg_len > 1))
					discard ;
				INDEX ix = seg + seg_len / 2 ;
				for (auto &&i : _RANGE_ (seg ,seg + seg_len - 1)) {
					_DEBUG_ASSERT_ (mVertex[mOrder[rot][i]][rot] <= mVertex[mOrder[rot][i + 1]][rot]) ;
					_STATIC_UNUSED_ (i) ;
				}
				compute_order (mTempOrder ,mOrder ,rot ,mNextRot[rot] ,seg ,ix ,seg_len) ;
				compute_order (mTempOrder ,mOrder ,rot ,mNextRot[mNextRot[rot]] ,seg ,ix ,seg_len) ;
				INDEX jx = mKDTree.alloc (mVertex[mOrder[rot][ix]][rot] ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
				update_build_tree (mKDTree[jx].mLeft ,mNextRot[rot] ,seg ,(ix - seg)) ;
				mKDTree[jx].mLeft = mLatestIndex ;
				update_build_tree (mKDTree[jx].mRight ,mNextRot[rot] ,ix ,(seg_len - (ix - seg))) ;
				mKDTree[jx].mRight = mLatestIndex ;
				mLatestIndex = curr ;
			}
		}

		void compute_order (Array<INDEX> &tmp_order ,ARRAY3<Array<INDEX>> &order ,const INDEX &rot ,const INDEX &n_rot ,const INDEX &seg_a ,const INDEX &seg_b ,const LENGTH &seg_len) const {
			if (tmp_order.size () != mVertex.size ())
				tmp_order = Array<INDEX> (mVertex.size ()) ;
			INDEX iw = 0 ;
			for (auto &&i : _RANGE_ (seg_a ,seg_b))
				tmp_order[iw++] = mOrder[n_rot][i] ;
			for (auto &&i : _RANGE_ (seg_b ,seg_a + seg_len))
				tmp_order[iw++] = mOrder[n_rot][i] ;
			const auto r1x = ARRAY2<INDEX> {0 ,iw} ;
			for (auto &&i : _RANGE_ (r1x[0] ,r1x[1]))
				order[n_rot][seg_a + i] = tmp_order[i] ;
			_DEBUG_ASSERT_ (iw == seg_len) ;
		}

		inline void refresh () {
			mContext.mVertex = mVertex ;
			mContext.mNextRot = mNextRot ;
			mContext.mBound = _MOVE_ (mBound) ;
			mContext.mKDTree = _MOVE_ (mKDTree) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	_CALL_ (Lambda (DEREF[this] ,vertex)) ;
}

template <class REAL>
class MaxFlowAlgorithm {
private:
	Bitmap<REAL> mCurrentFlow ;
	REAL mMaxFlow ;

public:
	MaxFlowAlgorithm () = delete ;

	explicit MaxFlowAlgorithm (const Bitmap<REAL> &adjacency ,const INDEX &source ,const INDEX &sink) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		_DEBUG_ASSERT_ (source != sink) ;
		initialize (adjacency ,source ,sink) ;
	}

	REAL query () const {
		return mMaxFlow ;
	}

	const Bitmap<REAL> &query_flow () const leftvalue {
		return mCurrentFlow ;
	}

private:
	void initialize (const Bitmap<REAL> &adjacency ,const INDEX &source ,const INDEX &sink) ;
} ;

template <class REAL>
inline exports void MaxFlowAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency ,const INDEX &source ,const INDEX &sink) {
	class Lambda final
		:private Proxy {
	private:
		MaxFlowAlgorithm &mContext ;
		const Bitmap<REAL> &mAdjacency ;

		INDEX mSource ;
		INDEX mSink ;
		REAL mSingleFlow ;
		Bitmap<REAL> mCurrentFlow ;
		Array<INDEX> mBFSPath ;

		Deque<INDEX> mTempQueue ;

	public:
		inline explicit Lambda (MaxFlowAlgorithm &context_ ,const Bitmap<REAL> &adjacency ,const INDEX &source ,const INDEX &sink)
			: mContext (context_) ,mAdjacency (adjacency) ,mSource (source) ,mSink (sink) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mSingleFlow = single_flow () ;
			mCurrentFlow = Bitmap<REAL> (mAdjacency.cx () ,mAdjacency.cy ()) ;
			mCurrentFlow.fill (REAL (0)) ;
			mBFSPath = Array<INDEX> (mAdjacency.cx ()) ;
		}

		inline REAL single_flow () const {
			REAL ret = REAL (0) ;
			for (auto &&i : mAdjacency.range ()) {
				if (i[0] == i[1])
					continue ;
				ret = MathProc::maxof (ret ,mAdjacency[i]) ;
			}
			return _MOVE_ (ret) ;
		}

		inline void generate () {
			while (TRUE) {
				update_augument_bfs () ;
				if (mBFSPath[mSource] == VAR_NONE)
					break ;
				const auto r1x = augument_max_flow () ;
				for (INDEX i = mSource ,it ,ie = mSink ; i != ie ; i = it) {
					it = mBFSPath[i] ;
					const auto r2x = MathProc::minof (mCurrentFlow[it][i] ,r1x) ;
					mCurrentFlow[it][i] -= r2x ;
					mCurrentFlow[i][it] += r1x - r2x ;
				}
			}
		}

		inline void update_augument_bfs () {
			if (mTempQueue.size () != mAdjacency.cx ())
				mTempQueue = Deque<INDEX> (mAdjacency.cx ()) ;
			mTempQueue.clear () ;
			mBFSPath.fill (VAR_NONE) ;
			mTempQueue.add (mSink) ;
			while (TRUE) {
				if (mTempQueue.empty ())
					break ;
				INDEX ix = mTempQueue[mTempQueue.head ()] ;
				mTempQueue.take () ;
				for (auto &&i : _RANGE_ (0 ,mAdjacency.cy ())) {
					if (i == ix)
						continue ;
					if (mBFSPath[i] != VAR_NONE)
						continue ;
					if (mCurrentFlow[i][ix] - mCurrentFlow[ix][i] >= mAdjacency[i][ix])
						continue ;
					mBFSPath[i] = ix ;
					mTempQueue.add (i) ;
				}
			}
		}

		inline REAL augument_max_flow () const {
			REAL ret = mSingleFlow ;
			for (INDEX i = mSource ,it ,ie = mSink ; i != ie ; i = it) {
				it = mBFSPath[i] ;
				_DEBUG_ASSERT_ (i != it) ;
				const auto r1x = mAdjacency[i][it] + mCurrentFlow[it][i] - mCurrentFlow[i][it] ;
				ret = MathProc::minof (ret ,r1x) ;
			}
			return _MOVE_ (ret) ;
		}

		inline void refresh () {
			mContext.mMaxFlow = max_flow () ;
			mContext.mCurrentFlow = _MOVE_ (mCurrentFlow) ;
		}

		inline REAL max_flow () const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,mCurrentFlow.cy ()))
				ret += mCurrentFlow[i][mSink] ;
			return _MOVE_ (ret) ;
		}
	} ;
	_CALL_ (Lambda (DEREF[this] ,adjacency ,source ,sink)) ;
}
} ;