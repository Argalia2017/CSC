#pragma once

#ifndef __CSC_ALGORITHM__
#define __CSC_ALGORITHM__
#endif

#include "csc.hpp"
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

	explicit PrimeSieveAlgorithm (LENGTH len) {
		initialize (len) ;
	}

	const BitSet<> &query () const & {
		return mPrimeSet ;
	}

	BitSet<> query () && {
		return std::move (mPrimeSet) ;
	}

private:
	void initialize (LENGTH len) ;
} ;

inline void PrimeSieveAlgorithm::initialize (LENGTH len) {
	mPrimeSet = BitSet<> (len) ;
	mPrimeSet.fill (BYTE (0XAA)) ;
	mPrimeSet[1] = FALSE ;
	mPrimeSet[2] = TRUE ;
	for (INDEX i = 3 ,ie = _SQRT_ (mPrimeSet.size ()) + 1 ; i < ie ; i += 2) {
		const auto r1x = i * 2 ;
		for (INDEX j = _SQE_ (i) ,je = mPrimeSet.size () ; j < je ; j += r1x)
			mPrimeSet[j] = FALSE ;
	}
}

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

	INDEX query (const PhanBuffer<const REAL> &target ,INDEX seg) const {
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
			while (iy != VAR_NONE && target[ix] != mPattern[iy])
				iy = mNext[iy] ;
			ix++ ;
			iy++ ;
		}
		if (iy < mNext.length ())
			return VAR_NONE ;
		return ix - mNext.length () ;
	}

private:
	void initialize (const PhanBuffer<const REAL> &pattern) ;

	INDEX find_next (INDEX slow ,INDEX fast) const {
		for (INDEX i = fast ; i != VAR_NONE ; i = mNext[i])
			if (mPattern[i] != mPattern[slow])
				return i ;
		return VAR_NONE ;
	}
} ;

template <class REAL>
inline void KMPAlgorithm<REAL>::initialize (const PhanBuffer<const REAL> &pattern) {
	mNext = Array<INDEX> (pattern.size ()) ;
	mPattern = Array<REAL> (pattern.size ()) ;
	INDEX ix = 0 ;
	INDEX iy = VAR_NONE ;
	mNext[ix] = VAR_NONE ;
	mPattern[ix] = pattern[ix] ;
	while (TRUE) {
		if (ix >= pattern.size () - 1)
			break ;
		while (iy != VAR_NONE && pattern[ix] != pattern[iy])
			iy = mNext[iy] ;
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

	explicit DijstraAlgorithm (const Bitmap<REAL> &adjacency ,INDEX root) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		initialize (adjacency ,root) ;
	}

	REAL query (INDEX index) const {
		return mDistance[index] ;
	}

	Array<INDEX> query_path (INDEX index) const {
		Array<INDEX> ret = Array<INDEX> (query_path_depth (index)) ;
		INDEX iw = ret.length () ;
		for (INDEX i = index ; i != VAR_NONE ; i = mPrev[i])
			ret[--iw] = i ;
		_DEBUG_ASSERT_ (iw == 0) ;
		return std::move (ret) ;
	}

private:
	void initialize (const Bitmap<REAL> &adjacency ,INDEX root) ;

	LENGTH query_path_depth (INDEX index) const {
		LENGTH ret = 0 ;
		for (INDEX i = index ; i != VAR_NONE ; i = mPrev[i])
			ret++ ;
		return std::move (ret) ;
	}
} ;

template <class REAL>
inline void DijstraAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency ,INDEX root) {
	class Lambda {
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
		inline explicit Lambda (DijstraAlgorithm &context ,const Bitmap<REAL> &adjancency ,INDEX root) popping : mContext (context) ,mAdjacency (adjancency) ,mRoot (root) {}

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
				const auto r1x = mPriority[mPriority.head ()].item ;
				mPriority.take () ;
				update_distance (r1x) ;
			}
		}

		inline void update_distance (INDEX y) {
			if (mYVisit[y])
				return ;
			mYVisit[y] = TRUE ;
			for (INDEX i = 0 ,ie = mPrev.length () ; i < ie ; i++) {
				if (i == y)
					continue ;
				if (mAdjacency[y][i] < REAL (0))
					continue ;
				const auto r1x = mDistance[y] + mAdjacency[y][i] ;
				if (mXVisit[i] && mDistance[i] <= r1x)
					continue ;
				mPrev[i] = y ;
				mDistance[i] = r1x ;
				mXVisit[i] = TRUE ;
				mPriority.add (mDistance[i] ,i) ;
			}
		}

		inline void refresh () {
			mContext.mPrev = std::move (mPrev) ;
			mContext.mDistance = std::move (mDistance) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,adjacency ,root)) ;
}

template <class REAL>
class KMeansAlgorithm {
private:
	Set<BitSet<>> mClusterSet ;

public:
	KMeansAlgorithm () = delete ;

	explicit KMeansAlgorithm (const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) {
		_DEBUG_ASSERT_ (dataset.length () >= 2) ;
		_DEBUG_ASSERT_ (center.length () >= 2) ;
		initialize (dataset ,distance ,center) ;
	}

	const Set<BitSet<>> &query () const & {
		return mClusterSet ;
	}

	Set<BitSet<>> query () && {
		return std::move (mClusterSet) ;
	}

private:
	void initialize (const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) ;
} ;

template <class REAL>
inline void KMeansAlgorithm<REAL>::initialize (const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) {
	class Lambda {
	private:
		KMeansAlgorithm &mContext ;
		const Set<REAL> &mDataSet ;
		const Function<REAL (const REAL & ,const REAL &)> &mDistanceFunc ;
		const Array<REAL> &mCenter ;
		const REAL mTolerance = REAL (1E-6) ;

		Deque<REAL> mCurrCenterList ;
		Deque<REAL> mNextCenterList ;
		Array<ARRAY2<INDEX>> mCenterIndex ;
		Set<INDEX ,BitSet<>> mClusterSet ;
		ARRAY3<REAL> mConvergence ;

	public:
		inline explicit Lambda (KMeansAlgorithm &context ,const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) popping : mContext (context) ,mDistanceFunc (distance) ,mDataSet (dataset) ,mCenter (center) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mCurrCenterList = Deque<REAL> (mCenterIndex.length ()) ;
			mNextCenterList = Deque<REAL> (mCenterIndex.length ()) ;
			mCurrCenterList.appand (mCenter) ;
			mCenterIndex = Array<ARRAY2<INDEX>> (mCurrCenterList.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mCurrCenterList)
				mCenterIndex[iw++][0] = mCurrCenterList.at (i) ;
			_DEBUG_ASSERT_ (iw == mCenterIndex.length ()) ;
			mClusterSet = Set<INDEX ,BitSet<>> () ;
			mConvergence.fill (REAL (-1)) ;
		}

		inline void generate () {
			while (TRUE) {
				update_cluster_set () ;
				update_next_center_list () ;
				update_convergence () ;
				if (reach_convergence ())
					break ;
				update_curr_center_list () ;
			}
		}

		inline void update_cluster_set () {
			mClusterSet.clear () ;
			for (auto &&i : mDataSet) {
				INDEX ix = closest_center_of_point (i) ;
				INDEX iy = mClusterSet.insert (ix) ;
				if (mClusterSet[iy].item.size () == 0)
					mClusterSet[iy].item = BitSet<> (mDataSet.size ()) ;
				mClusterSet[iy].item[mDataSet.at (i)] = TRUE ;
			}
			for (INDEX i = 0 ,ie = mCenter.length () ; i < ie ; i++)
				mClusterSet.add (i ,BitSet<> ()) ;
		}

		inline INDEX closest_center_of_point (const REAL &point) const {
			INDEX ret = VAR_NONE ;
			auto rax = REAL () ;
			for (auto &&i : mCurrCenterList) {
				const auto r1x = mDistanceFunc (point ,i) ;
				if (ret != VAR_NONE && rax <= r1x)
					continue ;
				ret = mCurrCenterList.at (i) ;
				rax = r1x ;
			}
			return std::move (ret) ;
		}

		inline void update_next_center_list () {
			mNextCenterList.clear () ;
			for (auto &&i : mClusterSet) {
				const auto r1x = _SWITCH_ (
					(i.item.length () > 0) ? (average_center (i.item)) :
					(mCurrCenterList[i.key])) ;
				mNextCenterList.add (r1x) ;
			}
			_DEBUG_ASSERT_ (mCenterIndex.size () == mNextCenterList.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mNextCenterList)
				mCenterIndex[iw++][0] = mNextCenterList.at (i) ;
			_DEBUG_ASSERT_ (iw == mCenterIndex.length ()) ;
		}

		inline REAL average_center (const BitSet<> &cluster) const {
			REAL ret = REAL (0) ;
			const auto r1x = cluster.length () ;
			_DEBUG_ASSERT_ (r1x != 0) ;
			for (auto &&i : cluster)
				ret += mDataSet[i] ;
			ret *= _PINV_ (REAL (r1x)) ;
			return std::move (ret) ;
		}

		inline void update_convergence () {
			mConvergence[0] = mConvergence[1] ;
			mConvergence[1] = mConvergence[2] ;
			mConvergence[2] = REAL (-1) ;
			if (mCurrCenterList.length () != mNextCenterList.length ())
				return ;
			mConvergence[2] = REAL (0) ;
			_DEBUG_ASSERT_ (mCurrCenterList.length () == mNextCenterList.length ()) ;
			for (auto &&i : mCenterIndex) {
				const auto r1x = mDistanceFunc (mCurrCenterList[i[0]] ,mNextCenterList[i[1]]) ;
				mConvergence[2] = _MAX_ (mConvergence[2] ,r1x) ;
			}
		}

		inline BOOL reach_convergence () const {
			if (mConvergence[0] < REAL (0))
				return FALSE ;
			if (mConvergence[1] < REAL (0))
				return FALSE ;
			if (mConvergence[2] < REAL (0))
				return FALSE ;
			if (mConvergence[0] > mConvergence[1])
				return FALSE ;
			if (mConvergence[1] > mConvergence[2])
				return FALSE ;
			if (mConvergence[1] - mConvergence[2] >= mTolerance)
				return FALSE ;
			return TRUE ;
		}

		inline void update_curr_center_list () {
			mCurrCenterList.clear () ;
			mCurrCenterList.appand (mNextCenterList) ;
			_DEBUG_ASSERT_ (mCenterIndex.size () == mCurrCenterList.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mCurrCenterList)
				mCenterIndex[iw++][0] = mCurrCenterList.at (i) ;
			_DEBUG_ASSERT_ (iw == mCenterIndex.length ()) ;
		}

		inline void refresh () {
			mContext.mClusterSet = Set<BitSet<>> () ;
			mContext.mClusterSet.appand (mClusterSet) ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,dataset ,distance ,center)) ;
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

	const Array<ARRAY2<INDEX>> &query_match () const & {
		return mMatch ;
	}

	Array<ARRAY2<INDEX>> query_match () && {
		return std::move (mMatch) ;
	}

private:
	void initialize (const Bitmap<REAL> &adjacency) ;
} ;

template <class REAL>
inline void KMHungarianAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency) {
	class Lambda {
	private:
		KMHungarianAlgorithm &mContext ;
		const Bitmap<REAL> &mAdjacency ;
		const REAL mTolerance = REAL (1E-6) ;

		Array<INDEX> mXYLink ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;
		Array<REAL> mXWeight ;
		Array<REAL> mYWeight ;
		ARRAY2<REAL> mLackWeight ;

		Deque<ARRAY2<INDEX>> mTempStack ;
		BOOL mTempRet ;
		FLAG mTempState ;

	public:
		inline explicit Lambda (KMHungarianAlgorithm &context ,const Bitmap<REAL> &adjacency) popping : mContext (context) ,mAdjacency (adjacency) {}

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
				mYWeight[i[0]] = _MAX_ (mYWeight[i[0]] ,mAdjacency[i]) ;
			}
		}

		inline void generate () {
			for (INDEX i = 0 ,ie = mAdjacency.cy () ; i < ie ; i++) {
				while (TRUE) {
					mXVisit.clear () ;
					mYVisit.clear () ;
					update_lack_weight (i) ;
					if (mLackWeight[1] < mTolerance)
						break ;
					for (auto &&j : mXVisit)
						mXWeight[j] += mLackWeight[1] ;
					for (auto &&j : mYVisit)
						mYWeight[j] -= mLackWeight[1] ;
				}
			}
		}

		inline void update_lack_weight (INDEX y) {
			mTempStack.clear () ;
			mTempStack.add (ARRAY2<INDEX> {0 ,y}) ;
			mTempState = VAR_ZERO ;
			mTempRet = FALSE ;
			INDEX ix = VAR_NONE ;
			while (TRUE) {
				if (mTempState == VAR_NONE)
					break ;
				auto ifa = FALSE ;
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 0))
						discard ;
					mLackWeight[0] = 0 ;
					mLackWeight[1] = 0 ;
					mTempState = 7 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 2))
						discard ;
					mTempRet = TRUE ;
					mTempState = 17 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 3))
						discard ;
					mYVisit[mTempStack[ix][1]] = TRUE ;
					mTempStack[ix][0] = 0 ;
					mTempState = 4 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 4))
						discard ;
					const auto r1x = _SWITCH_ (
						(mTempStack[ix][0] < mAdjacency.cx ()) ? 5 :
						16) ;
					mTempState = r1x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 5))
						discard ;
					const auto r2x = _SWITCH_ (
						(mXVisit[mTempStack[ix][0]]) ? 15 :
						6) ;
					mTempState = r2x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 6))
						discard ;
					mLackWeight[0] = mYWeight[mTempStack[ix][1]] + mXWeight[mTempStack[ix][0]] - mAdjacency[mTempStack[ix][1]][mTempStack[ix][0]] ;
					const auto r3x = _SWITCH_ (
						(mLackWeight[0] < mTolerance) ? 8 :
						12) ;
					mTempState = r3x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 7))
						discard ;
					ix = mTempStack.tail () ;
					const auto r4x = _SWITCH_ (
						(mTempStack[ix][1] == VAR_NONE) ? 2 :
						3) ;
					mTempState = r4x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 8))
						discard ;
					mXVisit[mTempStack[ix][0]] = TRUE ;
					mTempStack.add (ARRAY2<INDEX> {0 ,mXYLink[mTempStack[ix][0]]}) ;
					mTempState = 7 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 9))
						discard ;
					ix = mTempStack.tail () ;
					mTempState = 10 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 10))
						discard ;
					const auto r5x = _SWITCH_ (
						mTempRet ? 11 :
						15) ;
					mTempState = r5x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 11))
						discard ;
					mXYLink[mTempStack[ix][0]] = mTempStack[ix][1] ;
					mTempRet = TRUE ;
					mTempState = 17 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 12))
						discard ;
					const auto r6x = _SWITCH_ (
						(mLackWeight[1] < mTolerance) ? 13 :
						14) ;
					mTempState = r6x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 13))
						discard ;
					mLackWeight[1] = mLackWeight[0] ;
					mTempState = 15 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 14))
						discard ;
					mLackWeight[1] = _MIN_ (mLackWeight[1] ,mLackWeight[0]) ;
					mTempState = 15 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 15))
						discard ;
					mTempStack[ix][0]++ ;
					mTempState = 4 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 16))
						discard ;
					mTempRet = FALSE ;
					mTempState = 17 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 17))
						discard ;
					mTempStack.pop () ;
					const auto r7x = _SWITCH_ (
						(mTempStack.length () > 0) ? 9 :
						18) ;
					mTempState = r7x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 18))
						discard ;
					const auto r8x = _SWITCH_ (
						mTempRet ? 19 :
						20) ;
					mTempState = r8x ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 19))
						discard ;
					mLackWeight[0] = 0 ;
					mLackWeight[1] = 0 ;
					mTempState = 20 ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mTempState == 20))
						discard ;
					mTempState = VAR_NONE ;
				}
				if SWITCH_CASE (ifa) {
					_STATIC_WARNING_ ("unexpected") ;
					_DYNAMIC_ASSERT_ (FALSE) ;
				}
			}
		}

		inline void refresh () {
			mContext.mWeight = best_weight () ;
			mContext.mMatch = best_match () ;
		}

		inline REAL best_weight () const {
			REAL ret = REAL (0) ;
			for (INDEX i = 0 ,ie = mXYLink.length () ; i < ie ; i++)
				ret += mAdjacency[mXYLink[i]][i] ;
			return std::move (ret) ;
		}

		inline Array<ARRAY2<INDEX>> best_match () const {
			Array<ARRAY2<INDEX>> ret = Array<ARRAY2<INDEX>> (best_match_depth ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ,ie = mXYLink.length () ; i < ie ; i++) {
				if (mXYLink[i] == VAR_NONE)
					continue ;
				ret[iw++] = ARRAY2<INDEX> {mXYLink[i] ,i} ;
			}
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
			return std::move (ret) ;
		}

		inline LENGTH best_match_depth () const {
			LENGTH ret = 0 ;
			for (auto &&i : mXYLink)
				ret += EFLAG (i != VAR_NONE) ;
			return std::move (ret) ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,adjacency)) ;
}

template <class REAL>
class BFGSAlgorithm {
private:
	Array<REAL> mDX ;
	REAL mDXLoss ;

public:
	BFGSAlgorithm () = delete ;

	explicit BFGSAlgorithm (const Function<REAL (const Array<REAL> &)> &loss ,const Array<REAL> &fdx) {
		initialize (loss ,fdx) ;
	}

	const Array<REAL> &query () const & {
		return mDX ;
	}

	Array<REAL> query () && {
		return std::move (mDX) ;
	}

	REAL query_loss () const {
		return mDXLoss ;
	}

private:
	void initialize (const Function<REAL (const Array<REAL> &)> &loss ,const Array<REAL> &fdx) ;
} ;

template <class REAL>
inline void BFGSAlgorithm<REAL>::initialize (const Function<REAL (const Array<REAL> &)> &loss ,const Array<REAL> &fdx) {
	class Lambda {
	private:
		BFGSAlgorithm &mContext ;
		const Function<REAL (const Array<REAL> &)> &mLossFunc ;
		const Array<REAL> &mFDX ;
		const REAL mTolerance = REAL (1E-6) ;
		const REAL mDXLambdaFirst = REAL (1000) ;
		const REAL mDXLambdaPower = REAL (0.618) ;
		const ARRAY2<REAL> mDXC1C2 = ARRAY2<REAL> {REAL (1E-4) ,REAL (0.9)} ;

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
		inline explicit Lambda (BFGSAlgorithm &context ,const Function<REAL (const Array<REAL> &)> &loss ,const Array<REAL> &fdx) popping : mContext (context) ,mLossFunc (loss) ,mFDX (fdx) {}

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
			for (INDEX i = 0 ,ie = mDM.cy () ; i < ie ; i++)
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
			for (INDEX i = 0 ,ie = dx.length () ; i < ie ; i++)
				sx[i] = dx[i] ;
			_STATIC_WARNING_ ("mark") ;
			const auto r1x = _PINV_ (mTolerance) ;
			for (INDEX i = 0 ,ie = dg.length () ; i < ie ; i++) {
				const auto r3x = sx[i] ;
				sx[i] = r3x + mTolerance ;
				dg[i] = (mLossFunc (sx) - mLossFunc (dx)) * r1x ;
				sx[i] = r3x ;
			}
		}

		inline void update_is_and_ig () {
			_STATIC_WARNING_ ("note") ;
			/*
			Strong Wolfe-Powell Conditions
			find _lambda match E1 and E2
			(0 < _lambda)
			E1 := mLossFunc (mDX + _lambda * mIS) - mLossFunc (mDX) <= _lambda * c1 * (mDG * mIS)
			(0 < c1 < 0.5)
			E2 := _ABS_ (gradient_of_loss (mDX + _lambda * mIS) * mIS) <= -c2 * (mDG * mIS)
			(c1 < c2 < 1)
			*/
			for (INDEX i = 0 ,ie = mDG.length () ; i < ie ; i++)
				mIS[i] = -math_matrix_mul (mDM ,i ,mDG) ;
			const auto r1x = math_vector_dot (mDG ,mIS) ;
			mDXLoss[0] = mLossFunc (mDX) ;
			mDXLoss[2] = mDXLoss[0] ;
			mDXLambda[0] = REAL (0) ;
			mDXLambda[1] = mDXLambdaFirst ;
			mDXLambda[2] = mDXLambda[0] ;
			while (TRUE) {
				if (mDXLambda[1] < mTolerance)
					break ;
				if (mDXLoss[2] <= REAL (0))
					break ;
				for (INDEX i = 0 ,ie = mIX.length () ; i < ie ; i++)
					mIX[i] = mDX[i] + mIS[i] * mDXLambda[1] ;
				mDXLoss[1] = mLossFunc (mIX) ;
				for (FOR_ONCE_DO) {
					if (mDXLoss[1] - mDXLoss[0] > mDXLambda[1] * mDXC1C2[0] * r1x)
						discard ;
					compute_gradient_of_loss (mIX ,mIG ,mSX) ;
					if (_ABS_ (math_vector_dot (mIG ,mIS)) > -mDXC1C2[1] * r1x)
						discard ;
					mDXLoss[2] = REAL (0) ;
				}
				for (FOR_ONCE_DO) {
					if (mDXLoss[1] >= mDXLoss[2])
						discard ;
					mDXLoss[2] = mDXLoss[1] ;
					mDXLambda[2] = mDXLambda[1] ;
				}
				mDXLambda[1] *= mDXLambdaPower ;
			}
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
				if (!(mDXLoss[0] >= mDXLoss[2]))
					discard ;
				const auto r2x = _SWITCH_ (
					(mDXLoss[2] > REAL (0)) ? (mDXLoss[2]) :
					(mDXLoss[1])) ;
				mDXLoss[0] = r2x ;
				_SWAP_ (mDX ,mIX) ;
				compute_gradient_of_loss (mDX ,mIG ,mSX) ;
			}
			if SWITCH_CASE (ifa) {
				mIG.fill (REAL (0)) ;
			}
		}

		inline REAL math_matrix_mul (const Bitmap<REAL> &mat ,INDEX y ,const Array<REAL> &v) const {
			REAL ret = REAL (0) ;
			for (INDEX i = 0 ,ie = v.length () ; i < ie ; i++)
				ret += mat[y][i] * v[i] ;
			return std::move (ret) ;
		}

		inline REAL math_vector_dot (const Array<REAL> &v1 ,const Array<REAL> &v2) const {
			_DEBUG_ASSERT_ (v1.length () == v2.length ()) ;
			REAL ret = REAL (0) ;
			for (INDEX i = 0 ,ie = v1.length () ; i < ie ; i++)
				ret += v1[i] * v2[i] ;
			return std::move (ret) ;
		}

		inline REAL current_convergence () const {
			REAL ret = REAL (0) ;
			for (auto &&i : mIG)
				ret += _SQE_ (i) ;
			ret = _SQRT_ (ret) ;
			return std::move (ret) ;
		}

		inline void update_iy_and_dg () {
			for (INDEX i = 0 ,ie = mIG.length () ; i < ie ; i++) {
				mIY[i] = mIG[i] - mDG[i] ;
				mDG[i] = mIG[i] ;
			}
		}

		inline void update_dm () {
			const auto r1x = _PINV_ (math_vector_dot (mIY ,mIS)) ;
			for (auto &&i : mDM.range ())
				mIM[i] = hessian_matrix_each (i[0] ,i[1] ,r1x) + mIS[i[0]] * mIS[i[1]] * r1x ;
			_SWAP_ (mDM ,mIM) ;
		}

		inline REAL hessian_matrix_each (INDEX y ,INDEX x ,const REAL &ys) const {
			REAL ret = REAL (0) ;
			for (INDEX i = 0 ,ie = mDM.cy () ; i < ie ; i++) {
				const auto r1x = hessian_matrix_each_factor (x ,i ,ys) ;
				ret += r1x * (-mIS[y] * mIY[i] * ys) ;
				if (i == y)
					continue ;
				ret += r1x ;
			}
			return std::move (ret) ;
		}

		inline REAL hessian_matrix_each_factor (INDEX x ,INDEX z ,const REAL &ys) const {
			REAL ret = REAL (0) ;
			for (INDEX i = 0 ,ie = mDM.cx () ; i < ie ; i++) {
				ret += mDM[z][i] * (-mIY[i] * mIS[x] * ys) ;
				if (i == x)
					continue ;
				ret += mDM[z][i] ;
			}
			return std::move (ret) ;
		}

		inline void refresh () {
			mContext.mDX = std::move (mDX) ;
			mContext.mDXLoss = mDXLoss[0] ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,loss ,fdx)) ;
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

		inline explicit Node (const REAL &key ,INDEX leaf ,INDEX left ,INDEX right) :mKey (std::move (key)) ,mLeaf (leaf) ,mLeft (left) ,mRight (right) {}
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
		rax[0][0] = _MAX_ ((point[0] - width) ,mBound[0][0]) ;
		rax[0][1] = _MIN_ ((point[0] + width) ,mBound[0][1]) ;
		rax[1][0] = _MAX_ ((point[1] - width) ,mBound[1][0]) ;
		rax[1][1] = _MIN_ ((point[1] + width) ,mBound[1][1]) ;
		rax[2][0] = _MAX_ ((point[2] - width) ,mBound[2][0]) ;
		rax[2][1] = _MIN_ ((point[2] + width) ,mBound[2][1]) ;
		compute_search_range (point ,_SQE_ (width) ,mRoot ,0 ,rax ,ret) ;
		return std::move (ret) ;
	}

	Array<PACK<INDEX ,REAL>> query_nearst (const ARRAY3<REAL> &point ,LENGTH count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		const auto r1x = first_count_vertex (point ,count) ;
		const auto r2x = r1x.esort () ;
		Array<PACK<INDEX ,REAL>> ret = Array<PACK<INDEX ,REAL>> (count) ;
		for (INDEX i = 0 ,ie = ret.length () ; i < ie ; i++) {
			ret[i].P1 = r2x[i] ;
			ret[i].P2 = r1x[r2x[i]] ;
		}
		compute_search_range (point ,mRoot ,0 ,ret) ;
		return std::move (ret) ;
	}

private:
	void initialize (const Array<ARRAY3<REAL>> &vertex) ;

	void compute_search_range (const ARRAY3<REAL> &point ,const REAL &sqe_range ,INDEX curr ,INDEX rot ,ARRAY3<ARRAY2<REAL>> &bound ,Deque<INDEX> &out) const {
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(mKDTree[curr].mLeaf != VAR_NONE))
				discard ;
			for (FOR_ONCE_DO) {
				INDEX ix = mKDTree[curr].mLeaf ;
				const auto r2x = _SQE_ (mVertex[ix][0] - point[0]) + _SQE_ (mVertex[ix][1] - point[1]) + _SQE_ (mVertex[ix][2] - point[2]) ;
				if (r2x > sqe_range)
					discard ;
				out.add (ix) ;
			}
		}
		if SWITCH_CASE (ifa) {
			if (!(mKDTree[curr].mLeaf == VAR_NONE))
				discard ;
			const auto r3x = mKDTree[curr].mKey ;
			for (FOR_ONCE_DO) {
				if (r3x < bound[rot][0])
					discard ;
				const auto r4x = bound[rot][1] ;
				bound[rot][1] = _MIN_ (bound[rot][1] ,r3x) ;
				compute_search_range (point ,sqe_range ,mKDTree[curr].mLeft ,mNextRot[rot] ,bound ,out) ;
				bound[rot][1] = r4x ;
			}
			for (FOR_ONCE_DO) {
				if (r3x > bound[rot][1])
					discard ;
				const auto r5x = bound[rot][0] ;
				bound[rot][0] = _MAX_ (bound[rot][0] ,r3x) ;
				compute_search_range (point ,sqe_range ,mKDTree[curr].mRight ,mNextRot[rot] ,bound ,out) ;
				bound[rot][0] = r5x ;
			}
		}
	}

	Deque<REAL> first_count_vertex (const ARRAY3<REAL> &point ,LENGTH count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		Deque<REAL> ret = Deque<REAL> (count) ;
		for (INDEX i = 0 ,ie = count ; i < ie ; i++) {
			const auto r1x = _SQE_ (mVertex[i][0] - point[0]) + _SQE_ (mVertex[i][1] - point[1]) + _SQE_ (mVertex[i][2] - point[2]) ;
			ret.add (r1x) ;
		}
		return std::move (ret) ;
	}

	void compute_search_range (const ARRAY3<REAL> &point ,INDEX curr ,INDEX rot ,Array<PACK<INDEX ,REAL>> &out) const {
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(mKDTree[curr].mLeaf != VAR_NONE))
				discard ;
			for (FOR_ONCE_DO) {
				INDEX ix = mKDTree[curr].mLeaf ;
				const auto r1x = Vector<REAL> {mVertex[ix] ,REAL (0)} ;
				const auto r2x = Vector<REAL> {point ,REAL (0)} ;
				const auto r3x = (r1x - r2x).magnitude () ;
				INDEX jx = out.length () ;
				while (TRUE) {
					if (jx - 1 < 0)
						break ;
					if (r3x >= out[jx - 1].P2)
						break ;
					if (jx < out.length ())
						out[jx] = out[jx - 1] ;
					jx-- ;
				}
				if (jx >= out.length ())
					discard ;
				out[jx].P1 = ix ;
				out[jx].P2 = r3x ;
			}
		}
		if SWITCH_CASE (ifa) {
			if (!(mKDTree[curr].mLeaf == VAR_NONE))
				discard ;
			const auto r4x = mKDTree[curr].mKey ;
			if (r4x >= point[rot] - out[out.length () - 1].P2)
				compute_search_range (point ,mKDTree[curr].mLeft ,mNextRot[rot] ,out) ;
			if (r4x <= point[rot] + out[out.length () - 1].P2)
				compute_search_range (point ,mKDTree[curr].mRight ,mNextRot[rot] ,out) ;
		}
	}
} ;

template <class REAL>
inline void KDTreeAlgorithm<REAL>::initialize (const Array<ARRAY3<REAL>> &vertex) {
	class Lambda {
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
		inline explicit Lambda (KDTreeAlgorithm &context ,const Array<ARRAY3<REAL>> &vertex) popping : mContext (context) ,mVertex (vertex) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mNextRot = ARRAY3<INDEX> {1 ,2 ,0} ;
			for (INDEX i = 0 ,ie = mOrder.length () ; i < ie ; i++)
				mOrder[i] = stack_of_order (i).esort () ;
			mKDTree = Allocator<Node ,SAUTO> (mVertex.length ()) ;
			mRoot = VAR_NONE ;
		}

		Deque<INDEX> stack_of_order (INDEX rot) const {
			Deque<INDEX> ret = Deque<INDEX> (mVertex.length ()) ;
			for (auto &&i : mVertex)
				ret.add (i[rot]) ;
			return std::move (ret) ;
		}

		inline void generate () {
			update_bound () ;
			update_build_tree (mRoot ,0 ,0 ,mVertex.length ()) ;
			mRoot = mLatestIndex ;
		}

		inline void update_bound () {
			_DEBUG_ASSERT_ (mVertex.length () > 0) ;
			for (FOR_ONCE_DO) {
				mBound[0][0] = mVertex[0][0] ;
				mBound[0][1] = mVertex[0][0] ;
				mBound[1][0] = mVertex[0][1] ;
				mBound[1][1] = mVertex[0][1] ;
				mBound[2][0] = mVertex[0][2] ;
				mBound[2][1] = mVertex[0][2] ;
			}
			for (auto &&i : mVertex) {
				mBound[0][0] = _MIN_ (mBound[0][0] ,i[0]) ;
				mBound[0][1] = _MAX_ (mBound[0][1] ,i[0]) ;
				mBound[1][0] = _MIN_ (mBound[1][0] ,i[1]) ;
				mBound[1][1] = _MAX_ (mBound[1][1] ,i[1]) ;
				mBound[2][0] = _MIN_ (mBound[2][0] ,i[2]) ;
				mBound[2][1] = _MAX_ (mBound[2][1] ,i[2]) ;
			}
		}

		void update_build_tree (INDEX curr ,INDEX rot ,INDEX seg ,INDEX seg_len) {
			_DEBUG_ASSERT_ (seg_len > 0) ;
			_DEBUG_ASSERT_ (seg >= 0 && seg <= mVertex.size () - seg_len) ;
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
				if (!(seg_len == 1))
					discard ;
				INDEX jx = mKDTree.alloc (REAL (0) ,mOrder[rot][seg] ,VAR_NONE ,VAR_NONE) ;
				mLatestIndex = jx ;
			}
			if SWITCH_CASE (ifa) {
				if (!(seg_len > 1))
					discard ;
				INDEX ix = seg + seg_len / 2 ;
				for (INDEX i = seg ,ie = seg + seg_len - 1 ; i < ie ; i++)
					_DEBUG_ASSERT_ (mVertex[mOrder[rot][i]][rot] <= mVertex[mOrder[rot][i + 1]][rot]) ;
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

		void compute_order (Array<INDEX> &tmp_order ,ARRAY3<Array<INDEX>> &order ,INDEX rot ,INDEX n_rot ,INDEX seg_a ,INDEX seg_b ,LENGTH seg_len) const {
			if (tmp_order.size () != mVertex.size ())
				tmp_order = Array<INDEX> (mVertex.size ()) ;
			INDEX iw = 0 ;
			for (INDEX i = seg_a ,ie = seg_b ; i < ie ; i++)
				tmp_order[iw++] = mOrder[n_rot][i] ;
			for (INDEX i = seg_b ,ie = seg_a + seg_len ; i < ie ; i++)
				tmp_order[iw++] = mOrder[n_rot][i] ;
			const auto r1x = ARRAY2<INDEX> {0 ,iw} ;
			for (INDEX i = r1x[0] ,ie = r1x[1] ; i < ie ; i++)
				order[n_rot][seg_a + i] = tmp_order[i] ;
			_DEBUG_ASSERT_ (iw == seg_len) ;
		}

		inline void refresh () {
			mContext.mVertex = mVertex ;
			mContext.mNextRot = mNextRot ;
			mContext.mBound = std::move (mBound) ;
			mContext.mKDTree = std::move (mKDTree) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,vertex)) ;
}

template <class REAL>
class MaxFlowAlgorithm {
private:
	Bitmap<REAL> mCurrentFlow ;
	REAL mMaxFlow ;

public:
	MaxFlowAlgorithm () = delete ;

	explicit MaxFlowAlgorithm (const Bitmap<REAL> &adjacency ,INDEX source ,INDEX sink) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		_DEBUG_ASSERT_ (source != sink) ;
		initialize (adjacency ,source ,sink) ;
	}

	REAL query () const {
		return mMaxFlow ;
	}

	const Bitmap<REAL> &query_flow () const & {
		return mCurrentFlow ;
	}

	Bitmap<REAL> query_flow () && {
		return std::move (mCurrentFlow) ;
	}

private:
	void initialize (const Bitmap<REAL> &adjacency ,INDEX source ,INDEX sink) ;
} ;

template <class REAL>
inline void MaxFlowAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency ,INDEX source ,INDEX sink) {
	class Lambda {
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
		inline explicit Lambda (MaxFlowAlgorithm &context ,const Bitmap<REAL> &adjacency ,INDEX source ,INDEX sink) popping : mContext (context) ,mAdjacency (adjacency) ,mSource (source) ,mSink (sink) {}

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
				ret = _MAX_ (ret ,mAdjacency[i]) ;
			}
			return std::move (ret) ;
		}

		inline void generate () {
			while (TRUE) {
				update_augument_bfs () ;
				if (mBFSPath[mSource] == VAR_NONE)
					break ;
				const auto r1x = augument_max_flow () ;
				for (INDEX i = mSource ; i != mSink ; i = mBFSPath[i]) {
					INDEX ix = mBFSPath[i] ;
					const auto r2x = _MIN_ (mCurrentFlow[ix][i] ,r1x) ;
					mCurrentFlow[ix][i] -= r2x ;
					mCurrentFlow[i][ix] += r1x - r2x ;
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
				for (INDEX i = 0 ,ie = mAdjacency.cy () ; i < ie ; i++) {
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
			for (INDEX i = mSource ; i != mSink ; i = mBFSPath[i]) {
				INDEX ix = mBFSPath[i] ;
				_DEBUG_ASSERT_ (i != ix) ;
				const auto r1x = mAdjacency[i][ix] + mCurrentFlow[ix][i] - mCurrentFlow[i][ix] ;
				ret = _MIN_ (ret ,r1x) ;
			}
			return std::move (ret) ;
		}

		inline void refresh () {
			mContext.mMaxFlow = max_flow () ;
			mContext.mCurrentFlow = std::move (mCurrentFlow) ;
		}

		inline REAL max_flow () const {
			REAL ret = REAL (0) ;
			for (INDEX i = 0 ,ie = mCurrentFlow.cy () ; i < ie ; i++)
				ret += mCurrentFlow[i][mSink] ;
			return std::move (ret) ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,adjacency ,source ,sink)) ;
}
} ;