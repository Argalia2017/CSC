#pragma once

#ifndef __CSC_ALGORITHM__
#define __CSC_ALGORITHM__
#endif

#include "csc.hpp"
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

	explicit DijstraAlgorithm (const Bitmap<REAL> &adjacency ,INDEX root_) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		initialize (adjacency ,root_) ;
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
	void initialize (const Bitmap<REAL> &adjacency ,INDEX root_) ;

	LENGTH query_path_depth (INDEX index) const {
		LENGTH ret = 0 ;
		for (INDEX i = index ; i != VAR_NONE ; i = mPrev[i])
			ret++ ;
		return std::move (ret) ;
	}
} ;

template <class REAL>
inline void DijstraAlgorithm<REAL>::initialize (const Bitmap<REAL> &adjacency ,INDEX root_) {
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
		inline explicit Lambda (DijstraAlgorithm &context_ ,const Bitmap<REAL> &adjancency ,INDEX root_) popping : mContext (context_) ,mAdjacency (adjancency) ,mRoot (root_) {}

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
			mContext.mPrev = std::move (mPrev) ;
			mContext.mDistance = std::move (mDistance) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,adjacency ,root_)) ;
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
		const REAL mTolerance ;
		const REAL mInfinity ;

		Deque<REAL> mCurrCenterList ;
		Deque<REAL> mNextCenterList ;
		Array<ARRAY2<INDEX>> mCenterIndex ;
		Set<INDEX ,BitSet<>> mClusterSet ;
		ARRAY3<REAL> mConvergence ;

	public:
		inline explicit Lambda (KMeansAlgorithm &context_ ,const Set<REAL> &dataset ,const Function<REAL (const REAL & ,const REAL &)> &distance ,const Array<REAL> &center) popping : mContext (context_) ,mDistanceFunc (distance) ,mDataSet (dataset) ,mCenter (center) ,mTolerance (1E-6) ,mInfinity (VAL_INF) {}

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
			mConvergence.fill (mInfinity) ;
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
			for (auto &&i : _RANGE_ (0 ,mCenter.length ()))
				mClusterSet.add (i ,BitSet<> ()) ;
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
			return std::move (ret) ;
		}

		inline void update_next_center_list () {
			mNextCenterList.clear () ;
			for (auto &&i : mClusterSet) {
				const auto r1x = _SWITCH_ (
					(i.item.length () > 0) ? average_center (i.item) :
					mCurrCenterList[i.key]) ;
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
			INDEX ix = mConvergence.length () - 1 ;
			for (auto &&i : _RANGE_ (0 ,ix))
				mConvergence[i] = mConvergence[i + 1] ;
			mConvergence[ix] = +mInfinity ;
			if (mCurrCenterList.length () != mNextCenterList.length ())
				return ;
			mConvergence[ix] = REAL (0) ;
			_DEBUG_ASSERT_ (mCurrCenterList.length () == mNextCenterList.length ()) ;
			for (auto &&i : mCenterIndex) {
				const auto r1x = mDistanceFunc (mCurrCenterList[i[0]] ,mNextCenterList[i[1]]) ;
				mConvergence[ix] = _MAX_ (mConvergence[ix] ,r1x) ;
			}
		}

		inline BOOL reach_convergence () const {
			INDEX ix = mConvergence.length () - 1 ;
			for (auto &&i : _RANGE_ (0 ,ix))
				if (mConvergence[i] > mConvergence[i + 1])
					return FALSE ;
			if (_ABS_ (mConvergence[0] - mConvergence[ix]) >= mTolerance)
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
		FLAG mTempState ;

	public:
		inline explicit Lambda (KMHungarianAlgorithm &context_ ,const Bitmap<REAL> &adjacency) popping : mContext (context_) ,mAdjacency (adjacency) ,mTolerance (1E-6) ,mInfinity (VAL_INF) {}

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

		inline void update_lack_weight (INDEX y) {
			_STATIC_WARNING_ ("note") ;
			/*
			*	inline void update_lack_weight_e0 (INDEX y) {
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
			*	inline void update_lack_weight_e7 (INDEX stack_x ,INDEX stack_y ,BOOL &stack_ret) popping {
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
			*					mLackWeight[1] = _MIN_ (mLackWeight[1] ,mLackWeight[0]) ;
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
			mTempStack.clear () ;
			mTempState = VAR_ZERO ;
			INDEX ix = VAR_NONE ;
			while (TRUE) {
				if (mTempState == VAR_NONE)
					break ;
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 0))
						discard ;
					mLackWeight[0] = 0 ;
					mLackWeight[1] = +mInfinity ;
					mTempState = 1 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 1))
						discard ;
					mTempRet = FALSE ;
					mTempStack.add (ARRAY2<INDEX> {0 ,y}) ;
					mTempState = 7 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 2))
						discard ;
					mTempRet = TRUE ;
					mTempState = 17 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 3))
						discard ;
					mYVisit[mTempStack[ix][1]] = TRUE ;
					mTempStack[ix][0] = 0 ;
					mTempState = 4 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 4))
						discard ;
					const auto r1x = _SWITCH_ (
						(mTempStack[ix][0] < mAdjacency.cx ()) ? 5 :
						16) ;
					mTempState = r1x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 5))
						discard ;
					const auto r2x = _SWITCH_ (
						(mXVisit[mTempStack[ix][0]]) ? 15 :
						6) ;
					mTempState = r2x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 6))
						discard ;
					mLackWeight[0] = mYWeight[mTempStack[ix][1]] + mXWeight[mTempStack[ix][0]] - mAdjacency[mTempStack[ix][1]][mTempStack[ix][0]] ;
					mTempState = 9 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 7))
						discard ;
					ix = mTempStack.tail () ;
					const auto r3x = _SWITCH_ (
						(mTempStack[ix][1] == VAR_NONE) ? 2 :
						3) ;
					mTempState = r3x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 8))
						discard ;
					mXVisit[mTempStack[ix][0]] = TRUE ;
					mTempStack.add (ARRAY2<INDEX> {0 ,mXYLink[mTempStack[ix][0]]}) ;
					mTempState = 7 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 9))
						discard ;
					const auto r4x = _SWITCH_ (
						(mLackWeight[0] < mTolerance) ? 8 :
						14) ;
					mTempState = r4x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 10))
						discard ;
					ix = mTempStack.tail () ;
					const auto r5x = _SWITCH_ (
						mTempRet ? 11 :
						15) ;
					mTempState = r5x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 11))
						discard ;
					mXYLink[mTempStack[ix][0]] = mTempStack[ix][1] ;
					mTempRet = TRUE ;
					mTempState = 17 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 14))
						discard ;
					mLackWeight[1] = _MIN_ (mLackWeight[1] ,mLackWeight[0]) ;
					mTempState = 15 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 15))
						discard ;
					mTempStack[ix][0]++ ;
					mTempState = 4 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 16))
						discard ;
					mTempRet = FALSE ;
					mTempState = 17 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 17))
						discard ;
					mTempStack.pop () ;
					const auto r6x = _SWITCH_ (
						(mTempStack.length () > 0) ? 10 :
						18) ;
					mTempState = r6x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 18))
						discard ;
					const auto r7x = _SWITCH_ (
						mTempRet ? 19 :
						20) ;
					mTempState = r7x ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 19))
						discard ;
					mLackWeight[0] = 0 ;
					mLackWeight[1] = +mInfinity ;
					mTempState = 20 ;
				}
				if SWITCH_CASE (TRUE) {
					if (!(mTempState == 20))
						discard ;
					mTempState = VAR_NONE ;
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
			return std::move (ret) ;
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

	explicit BFGSAlgorithm (const Function<REAL (const Array<REAL> &)> &loss ,const Function<void (const Array<REAL> & ,Array<REAL> &)> &gradient ,const Array<REAL> &fdx) {
		initialize (loss ,gradient ,fdx) ;
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
		inline explicit Lambda (BFGSAlgorithm &context_ ,const Function<REAL (const Array<REAL> &)> &loss ,const Function<void (const Array<REAL> & ,Array<REAL> &)> &gradient ,const Array<REAL> &fdx) popping : mContext (context_) ,mLossFunc (loss) ,mGradientProc (gradient) ,mFDX (fdx) ,mTolerance (1E-6) ,mDXLambdaFirst (1000) ,mDXLambdaPower (0.618) ,mDXLambdaC1 (1E-4) ,mDXLambdaC2 (0.9) {}

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
			mDXLoss[0] = _ABS_ (mLossFunc (mDX)) ;
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
				mDXLoss[1] = _ABS_ (mLossFunc (mIX)) ;
				if SWITCH_CASE (TRUE) {
					if (mDXLoss[1] - mDXLoss[0] > mDXLambda[1] * mDXLambdaC1 * r1x)
						discard ;
					compute_gradient_of_loss (mIX ,mIG ,mSX) ;
					if (_ABS_ (math_vector_dot (mIG ,mIS)) > -mDXLambdaC2 * r1x)
						discard ;
					mDXLoss[2] = REAL (0) ;
				}
				if SWITCH_CASE (TRUE) {
					if (mDXLoss[1] >= mDXLoss[2])
						discard ;
					mDXLoss[2] = mDXLoss[1] ;
					mDXLambda[2] = mDXLambda[1] ;
				}
				mDXLambda[1] *= mDXLambdaPower ;
			}
			auto fax = TRUE ;
			if SWITCH_CASE (fax) {
				if (!(mDXLoss[0] >= mDXLoss[2]))
					discard ;
				auto &r2y = _SWITCH_ (
					(mDXLoss[2] > REAL (0)) ? mDXLoss[2] :
					mDXLoss[1]) ;
				mDXLoss[0] = r2y ;
				_SWAP_ (mDX ,mIX) ;
				compute_gradient_of_loss (mDX ,mIG ,mSX) ;
			}
			if SWITCH_CASE (fax) {
				mIG.fill (REAL (0)) ;
			}
		}

		inline REAL math_matrix_mul (const Bitmap<REAL> &mat ,INDEX y ,const Array<REAL> &v) const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,v.length ()))
				ret += mat[y][i] * v[i] ;
			return std::move (ret) ;
		}

		inline REAL math_vector_dot (const Array<REAL> &v1 ,const Array<REAL> &v2) const {
			_DEBUG_ASSERT_ (v1.length () == v2.length ()) ;
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,v1.length ()))
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
			for (auto &&i : _RANGE_ (0 ,mIG.length ())) {
				mIY[i] = mIG[i] - mDG[i] ;
				mDG[i] = mIG[i] ;
			}
		}

		inline void update_dm () {
			const auto r1x = _PINV_ (math_vector_dot (mIY ,mIS)) ;
			for (auto &&i : mDM.range ()) {
				const auto r2x = hessian_matrix_each (i[0] ,i[1] ,r1x) ;
				mIM[i] = r2x + mIS[i[0]] * mIS[i[1]] * r1x ;
			}
			_SWAP_ (mDM ,mIM) ;
		}

		inline REAL hessian_matrix_each (INDEX y ,INDEX x ,const REAL &ys) const {
			REAL ret = REAL (0) ;
			for (auto &&i : _RANGE_ (0 ,mDM.cy ())) {
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
			for (auto &&i : _RANGE_ (0 ,mDM.cx ())) {
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
		const auto r2x = r1x.range_sort () ;
		Array<PACK<INDEX ,REAL>> ret = Array<PACK<INDEX ,REAL>> (count) ;
		for (auto &&i : _RANGE_ (0 ,ret.length ())) {
			ret[i].P1 = r2x[i] ;
			ret[i].P2 = r1x[r2x[i]] ;
		}
		compute_search_range (point ,mRoot ,0 ,ret) ;
		return std::move (ret) ;
	}

private:
	void initialize (const Array<ARRAY3<REAL>> &vertex) ;

	void compute_search_range (const ARRAY3<REAL> &point ,const REAL &sqe_range ,INDEX curr ,INDEX rot ,ARRAY3<ARRAY2<REAL>> &bound ,Deque<INDEX> &out) const {
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(mKDTree[curr].mLeaf != VAR_NONE))
				discard ;
			if SWITCH_CASE (TRUE) {
				INDEX ix = mKDTree[curr].mLeaf ;
				const auto r1x = _SQE_ (mVertex[ix][0] - point[0]) + _SQE_ (mVertex[ix][1] - point[1]) + _SQE_ (mVertex[ix][2] - point[2]) ;
				if (r1x > sqe_range)
					discard ;
				out.add (ix) ;
			}
		}
		if SWITCH_CASE (fax) {
			if (!(mKDTree[curr].mLeaf == VAR_NONE))
				discard ;
			const auto r2x = mKDTree[curr].mKey ;
			if SWITCH_CASE (TRUE) {
				if (r2x < bound[rot][0])
					discard ;
				const auto r3x = bound[rot][1] ;
				bound[rot][1] = _MIN_ (bound[rot][1] ,r2x) ;
				compute_search_range (point ,sqe_range ,mKDTree[curr].mLeft ,mNextRot[rot] ,bound ,out) ;
				bound[rot][1] = r3x ;
			}
			if SWITCH_CASE (TRUE) {
				if (r2x > bound[rot][1])
					discard ;
				const auto r4x = bound[rot][0] ;
				bound[rot][0] = _MAX_ (bound[rot][0] ,r2x) ;
				compute_search_range (point ,sqe_range ,mKDTree[curr].mRight ,mNextRot[rot] ,bound ,out) ;
				bound[rot][0] = r4x ;
			}
		}
	}

	Deque<REAL> first_count_vertex (const ARRAY3<REAL> &point ,LENGTH count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		Deque<REAL> ret = Deque<REAL> (count) ;
		for (auto &&i : _RANGE_ (0 ,count)) {
			const auto r1x = _SQE_ (mVertex[i][0] - point[0]) + _SQE_ (mVertex[i][1] - point[1]) + _SQE_ (mVertex[i][2] - point[2]) ;
			ret.add (r1x) ;
		}
		return std::move (ret) ;
	}

	void compute_search_range (const ARRAY3<REAL> &point ,INDEX curr ,INDEX rot ,Array<PACK<INDEX ,REAL>> &out) const {
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(mKDTree[curr].mLeaf != VAR_NONE))
				discard ;
			if SWITCH_CASE (TRUE) {
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
		if SWITCH_CASE (fax) {
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
		inline explicit Lambda (KDTreeAlgorithm &context_ ,const Array<ARRAY3<REAL>> &vertex) popping : mContext (context_) ,mVertex (vertex) {}

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
			mBound[0][0] = mVertex[0][0] ;
			mBound[0][1] = mVertex[0][0] ;
			mBound[1][0] = mVertex[0][1] ;
			mBound[1][1] = mVertex[0][1] ;
			mBound[2][0] = mVertex[0][2] ;
			mBound[2][1] = mVertex[0][2] ;
			for (auto &&i : mVertex) {
				mBound[0][0] = _MIN_ (mBound[0][0] ,i[0]) ;
				mBound[0][1] = _MAX_ (mBound[0][1] ,i[0]) ;
				mBound[1][0] = _MIN_ (mBound[1][0] ,i[1]) ;
				mBound[1][1] = _MAX_ (mBound[1][1] ,i[1]) ;
				mBound[2][0] = _MIN_ (mBound[2][0] ,i[2]) ;
				mBound[2][1] = _MAX_ (mBound[2][1] ,i[2]) ;
			}
		}

		void update_build_tree (INDEX curr ,INDEX rot ,INDEX seg ,LENGTH seg_len) {
			_DEBUG_ASSERT_ (seg_len > 0) ;
			_DEBUG_ASSERT_ (seg >= 0 && seg <= mVertex.size () - seg_len) ;
			auto fax = TRUE ;
			if SWITCH_CASE (fax) {
				if (!(seg_len == 1))
					discard ;
				INDEX jx = mKDTree.alloc (REAL (0) ,mOrder[rot][seg] ,VAR_NONE ,VAR_NONE) ;
				mLatestIndex = jx ;
			}
			if SWITCH_CASE (fax) {
				if (!(seg_len > 1))
					discard ;
				INDEX ix = seg + seg_len / 2 ;
				for (auto &&i : _RANGE_ (seg ,seg + seg_len - 1)) {
					_DEBUG_ASSERT_ (mVertex[mOrder[rot][i]][rot] <= mVertex[mOrder[rot][i + 1]][rot]) ;
					(void) i ;
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

		void compute_order (Array<INDEX> &tmp_order ,ARRAY3<Array<INDEX>> &order ,INDEX rot ,INDEX n_rot ,INDEX seg_a ,INDEX seg_b ,LENGTH seg_len) const {
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
		inline explicit Lambda (MaxFlowAlgorithm &context_ ,const Bitmap<REAL> &adjacency ,INDEX source ,INDEX sink) popping : mContext (context_) ,mAdjacency (adjacency) ,mSource (source) ,mSink (sink) {}

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
				for (INDEX i = mSource ,ie = mSink ; i != ie ; i = mBFSPath[i]) {
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
			for (INDEX i = mSource ,ie = mSink ; i != ie ; i = mBFSPath[i]) {
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
			for (auto &&i : _RANGE_ (0 ,mCurrentFlow.cy ()))
				ret += mCurrentFlow[i][mSink] ;
			return std::move (ret) ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,adjacency ,source ,sink)) ;
}
} ;