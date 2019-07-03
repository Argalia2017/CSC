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
		for (INDEX j = _SQE_ (i) ; j < mPrimeSet.size () ; j += r1x)
			mPrimeSet[j] = FALSE ;
	}
}

template <class UNIT>
class KMPAlgorithm {
private:
	Array<INDEX> mNext ;
	Array<UNIT> mPattern ;

public:
	KMPAlgorithm () = delete ;

	explicit KMPAlgorithm (const PhanBuffer<const UNIT> &pattern) {
		initialize (pattern) ;
	}

	INDEX query (const PhanBuffer<const UNIT> &target ,INDEX seg) const {
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
	void initialize (const PhanBuffer<const UNIT> &pattern) ;

	INDEX find_next (INDEX slow ,INDEX fast) const {
		for (INDEX i = fast ; i != VAR_NONE ; i = mNext[i])
			if (mPattern[i] != mPattern[slow])
				return i ;
		return VAR_NONE ;
	}
} ;

template <class UNIT>
inline void KMPAlgorithm<UNIT>::initialize (const PhanBuffer<const UNIT> &pattern) {
	mNext = Array<INDEX> (pattern.size ()) ;
	mPattern = Array<UNIT> (pattern.size ()) ;
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

template <class UNIT>
class DijstraAlgorithm {
private:
	Array<INDEX> mPrev ;
	Array<UNIT> mDistance ;
	INDEX mRoot ;

public:
	DijstraAlgorithm () = delete ;

	explicit DijstraAlgorithm (const Bitmap<UNIT> &adjacency ,INDEX root) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		initialize (adjacency ,root) ;
	}

	UNIT query (INDEX index) const {
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
	void initialize (const Bitmap<UNIT> &adjacency ,INDEX root) ;

	LENGTH query_path_depth (INDEX index) const {
		LENGTH ret = 0 ;
		for (INDEX i = index ; i != VAR_NONE ; i = mPrev[i])
			ret++ ;
		return std::move (ret) ;
	}
} ;

template <class UNIT>
inline void DijstraAlgorithm<UNIT>::initialize (const Bitmap<UNIT> &adjacency ,INDEX root) {
	class Lambda {
	private:
		DijstraAlgorithm &mContext ;
		const Bitmap<UNIT> &mAdjacency ;

		INDEX mRoot ;
		Array<INDEX> mPrev ;
		Array<UNIT> mDistance ;
		Priority<UNIT ,INDEX> mPriority ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;

	public:
		inline explicit Lambda (DijstraAlgorithm &context ,const Bitmap<UNIT> &adjancency ,INDEX root) popping : mContext (context) ,mAdjacency (adjancency) ,mRoot (root) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mPrev = Array<INDEX> (mAdjacency.cx ()) ;
			mPrev.fill (VAR_NONE) ;
			mDistance = Array<UNIT> (mAdjacency.cx ()) ;
			mDistance[mRoot] = UNIT (0) ;
			mPriority = Priority<UNIT ,INDEX> (mAdjacency.cy ()) ;
			mPriority.add (mDistance[mRoot] ,mRoot) ;
			mXVisit = BitSet<> (mAdjacency.cx ()) ;
			mXVisit[mRoot] = TRUE ;
			mYVisit = BitSet<> (mAdjacency.cy ()) ;
		}

		inline void generate () {
			while (TRUE) {
				if (mPriority.empty ())
					break ;
				const auto r1x = mPriority[mPriority.peek ()].item ;
				mPriority.take () ;
				update_distance (r1x) ;
			}
		}

		inline void update_distance (INDEX y) {
			if (mYVisit[y])
				return ;
			mYVisit[y] = TRUE ;
			for (INDEX i = 0 ; i < mPrev.length () ; i++) {
				if (i == y)
					continue ;
				if (mAdjacency[y][i] < UNIT (0))
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
	_CALL_ (Lambda (*this ,adjacency ,root)) ;
}

template <class UNIT>
class KMeansAlgorithm {
private:
	Set<BitSet<>> mClusterSet ;

public:
	KMeansAlgorithm () = delete ;

	explicit KMeansAlgorithm (const Set<UNIT> &dataset ,const Function<UNIT (const UNIT & ,const UNIT &)> &distance ,const Array<UNIT> &center) {
		_DEBUG_ASSERT_ (dataset.length () >= 2 && center.length () >= 2) ;
		initialize (dataset ,distance ,center) ;
	}

	const Set<BitSet<>> &query () const & {
		return mClusterSet ;
	}

	Set<BitSet<>> query () && {
		return std::move (mClusterSet) ;
	}

private:
	void initialize (const Set<UNIT> &dataset ,const Function<UNIT (const UNIT & ,const UNIT &)> &distance ,const Array<UNIT> &center) ;
} ;

template <class UNIT>
inline void KMeansAlgorithm<UNIT>::initialize (const Set<UNIT> &dataset ,const Function<UNIT (const UNIT & ,const UNIT &)> &distance ,const Array<UNIT> &center) {
	class Lambda {
	private:
		KMeansAlgorithm &mContext ;
		const Set<UNIT> &mDataSet ;
		const Function<UNIT (const UNIT & ,const UNIT &)> &mDistanceFunc ;
		const Array<UNIT> &mCenter ;
		const UNIT mTolerance = UNIT (1E-6) ;

		Queue<UNIT> mCurrCenterList ;
		Queue<UNIT> mNextCenterList ;
		Array<ARRAY2<INDEX>> mCenterIndex ;
		Set<INDEX ,BitSet<>> mClusterSet ;
		ARRAY3<UNIT> mConvergence ;

	public:
		inline explicit Lambda (KMeansAlgorithm &context ,const Set<UNIT> &dataset ,const Function<UNIT (const UNIT & ,const UNIT &)> &distance ,const Array<UNIT> &center) popping : mContext (context) ,mDistanceFunc (distance) ,mDataSet (dataset) ,mCenter (center) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mCurrCenterList = Queue<UNIT> (mCenterIndex.length ()) ;
			mNextCenterList = Queue<UNIT> (mCenterIndex.length ()) ;
			mCurrCenterList.appand (mCenter) ;
			mCenterIndex = Array<ARRAY2<INDEX>> (mCurrCenterList.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mCurrCenterList)
				mCenterIndex[iw++][0] = mCurrCenterList.at (i) ;
			_DEBUG_ASSERT_ (iw == mCenterIndex.length ()) ;
			mClusterSet = Set<INDEX ,BitSet<>> () ;
			mConvergence.fill (UNIT (-1)) ;
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
			for (INDEX i = 0 ; i < mCenter.length () ; i++)
				mClusterSet.add (i ,BitSet<> ()) ;
		}

		inline INDEX closest_center_of_point (const UNIT &point) const {
			INDEX ret = VAR_NONE ;
			auto rax = UNIT () ;
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
				const auto r1x = (i.item.length () > 0) ? (average_center (i.item)) : (mCurrCenterList[i.key]) ;
				mNextCenterList.add (r1x) ;
			}
			_DEBUG_ASSERT_ (mCenterIndex.size () == mNextCenterList.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mNextCenterList)
				mCenterIndex[iw++][0] = mNextCenterList.at (i) ;
			_DEBUG_ASSERT_ (iw == mCenterIndex.length ()) ;
		}

		inline UNIT average_center (const BitSet<> &cluster) const {
			UNIT ret = UNIT (0) ;
			const auto r1x = cluster.length () ;
			_DEBUG_ASSERT_ (r1x != 0) ;
			for (auto &&i : cluster)
				ret += mDataSet[i] ;
			ret *= _PINV_ (UNIT (r1x)) ;
			return std::move (ret) ;
		}

		inline void update_convergence () {
			mConvergence[0] = mConvergence[1] ;
			mConvergence[1] = mConvergence[2] ;
			mConvergence[2] = UNIT (-1) ;
			if (mCurrCenterList.length () != mNextCenterList.length ())
				return ;
			mConvergence[2] = UNIT (0) ;
			_DEBUG_ASSERT_ (mCurrCenterList.length () == mNextCenterList.length ()) ;
			for (auto &&i : mCenterIndex)
				mConvergence[2] = _MAX_ (mConvergence[2] ,mDistanceFunc (mCurrCenterList[i[0]] ,mNextCenterList[i[1]])) ;
		}

		inline BOOL reach_convergence () const {
			if (mConvergence[0] < UNIT (0))
				return FALSE ;
			if (mConvergence[1] < UNIT (0))
				return FALSE ;
			if (mConvergence[2] < UNIT (0))
				return FALSE ;
			if (mConvergence[0] > mConvergence[1] || mConvergence[1] > mConvergence[2])
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
	_CALL_ (Lambda (*this ,dataset ,distance ,center)) ;
}

template <class UNIT>
class KMHungarianAlgorithm {
private:
	UNIT mWeight ;
	Array<ARRAY2<INDEX>> mMatch ;

public:
	KMHungarianAlgorithm () = delete ;

	explicit KMHungarianAlgorithm (const Bitmap<UNIT> &adjacency) {
		initialize (adjacency) ;
	}

	UNIT query () const {
		return mWeight ;
	}

	const Array<ARRAY2<INDEX>> &query_match () const & {
		return mMatch ;
	}

	Array<ARRAY2<INDEX>> query_match () && {
		return std::move (mMatch) ;
	}

private:
	void initialize (const Bitmap<UNIT> &adjacency) ;
} ;

template <class UNIT>
inline void KMHungarianAlgorithm<UNIT>::initialize (const Bitmap<UNIT> &adjacency) {
	class Lambda {
	private:
		KMHungarianAlgorithm &mContext ;
		const Bitmap<UNIT> &mAdjacency ;
		const UNIT mTolerance = UNIT (1E-6) ;

		Array<INDEX> mXYLink ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;
		Array<UNIT> mXWeight ;
		Array<UNIT> mYWeight ;
		ARRAY2<UNIT> mLackWeight ;

		Stack<ARRAY2<INDEX>> mTempStack ;
		BOOL mTempRet ;
		FLAG mTempState ;

	public:
		inline explicit Lambda (KMHungarianAlgorithm &context ,const Bitmap<UNIT> &adjacency) popping : mContext (context) ,mAdjacency (adjacency) {}

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
			mXWeight = Array<UNIT> (mAdjacency.cx ()) ;
			mXWeight.fill (UNIT (0)) ;
			mYWeight = Array<UNIT> (mAdjacency.cy ()) ;
			mYWeight.fill (UNIT (0)) ;
			for (auto &&i : mAdjacency.range ()) {
				_DYNAMIC_ASSERT_ (mAdjacency[i] >= UNIT (0)) ;
				mYWeight[i[0]] = _MAX_ (mYWeight[i[0]] ,mAdjacency[i]) ;
			}
		}

		inline void generate () {
			for (INDEX i = 0 ; i < mAdjacency.cy () ; i++) {
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
			mTempState = 1 ;
			mTempRet = FALSE ;
			INDEX ix = VAR_NONE ;
			while (TRUE) {
				if (mTempState == 0)
					break ;
				if (mTempState == 1) {
					mLackWeight[0] = 0 ;
					mLackWeight[1] = 0 ;
					mTempState = 7 ;
				} else if (mTempState == 2) {
					mTempRet = TRUE ;
					mTempState = 17 ;
				} else if (mTempState == 3) {
					mYVisit[mTempStack[ix][1]] = TRUE ;
					mTempStack[ix][0] = 0 ;
					mTempState = 4 ;
				} else if (mTempState == 4) {
					const auto r1x = (mTempStack[ix][0] < mAdjacency.cx ()) ? 5 : 16 ;
					mTempState = r1x ;
				} else if (mTempState == 5) {
					const auto r2x = (mXVisit[mTempStack[ix][0]]) ? 15 : 6 ;
					mTempState = r2x ;
				} else if (mTempState == 6) {
					mLackWeight[0] = mYWeight[mTempStack[ix][1]] + mXWeight[mTempStack[ix][0]] - mAdjacency[mTempStack[ix][1]][mTempStack[ix][0]] ;
					const auto r3x = (mLackWeight[0] < mTolerance) ? 8 : 12 ;
					mTempState = r3x ;
				} else if (mTempState == 7) {
					ix = mTempStack.peek () ;
					const auto r4x = (mTempStack[ix][1] == VAR_NONE) ? 2 : 3 ;
					mTempState = r4x ;
				} else if (mTempState == 8) {
					mXVisit[mTempStack[ix][0]] = TRUE ;
					mTempStack.add (ARRAY2<INDEX> {0 ,mXYLink[mTempStack[ix][0]]}) ;
					mTempState = 7 ;
				} else if (mTempState == 9) {
					ix = mTempStack.peek () ;
					mTempState = 10 ;
				} else if (mTempState == 10) {
					const auto r5x = mTempRet ? 11 : 15 ;
					mTempState = r5x ;
				} else if (mTempState == 11) {
					mXYLink[mTempStack[ix][0]] = mTempStack[ix][1] ;
					mTempRet = TRUE ;
					mTempState = 17 ;
				} else if (mTempState == 12) {
					const auto r6x = (mLackWeight[1] < mTolerance) ? 13 : 14 ;
					mTempState = r6x ;
				} else if (mTempState == 13) {
					mLackWeight[1] = mLackWeight[0] ;
					mTempState = 15 ;
				} else if (mTempState == 14) {
					mLackWeight[1] = _MIN_ (mLackWeight[1] ,mLackWeight[0]) ;
					mTempState = 15 ;
				} else if (mTempState == 15) {
					mTempStack[ix][0]++ ;
					mTempState = 4 ;
				} else if (mTempState == 16) {
					mTempRet = FALSE ;
					mTempState = 17 ;
				} else if (mTempState = 17) {
					mTempStack.take () ;
					const auto r7x = (mTempStack.length () > 0) ? 9 : 18 ;
					mTempState = r7x ;
				} else if (mTempState == 18) {
					const auto r8x = mTempRet ? 19 : 20 ;
					mTempState = r8x ;
				} else if (mTempState == 19) {
					mLackWeight[0] = 0 ;
					mLackWeight[1] = 0 ;
					mTempState = 20 ;
				} else if (mTempState == 20) {
					mTempState = 0 ;
				}
			}
		}

		inline void refresh () {
			mContext.mWeight = best_weight () ;
			mContext.mMatch = best_match () ;
		}

		inline UNIT best_weight () const {
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < mXYLink.length () ; i++)
				ret += mAdjacency[mXYLink[i]][i] ;
			return std::move (ret) ;
		}

		inline Array<ARRAY2<INDEX>> best_match () const {
			Array<ARRAY2<INDEX>> ret = Array<ARRAY2<INDEX>> (best_match_depth ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ; i < mXYLink.length () ; i++) {
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
	_CALL_ (Lambda (*this ,adjacency)) ;
}

template <class UNIT>
class TriangulateAlgorithm {
private:
	Array<ARRAY3<INDEX>> mTriangle ;

public:
	TriangulateAlgorithm () = delete ;

	explicit TriangulateAlgorithm (const Array<ARRAY2<UNIT>> &vertex) {
		initialize (vertex) ;
	}

	const Array<ARRAY3<INDEX>> &query () const & {
		return mTriangle ;
	}

	Array<ARRAY3<INDEX>> query () && {
		return std::move (mTriangle) ;
	}

private:
	void initialize (const Array<ARRAY2<UNIT>> &vertex) ;
} ;

template <class UNIT>
inline void TriangulateAlgorithm<UNIT>::initialize (const Array<ARRAY2<UNIT>> &vertex) {
	class Lambda {
	private:
		TriangulateAlgorithm &mContext ;
		const Array<ARRAY2<UNIT>> &mVertex ;
		const UNIT mTolerance = UNIT (1E-6) ;

		Deque<INDEX> mPloygonVertexList ;
		BOOL mClockwiseFlag ;
		Queue<ARRAY3<INDEX>> mTriangleList ;
		Array<ARRAY3<INDEX>> mTriangle ;

	public:
		inline explicit Lambda (TriangulateAlgorithm &context ,const Array<ARRAY2<UNIT>> &vertex) popping : mContext (context) ,mVertex (vertex) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mPloygonVertexList = ploygon_vertex_list () ;
			const auto r1x = BOOL (ploygon_vertex_clockwise () > UNIT (0)) ;
			mClockwiseFlag = r1x ;
			for (FOR_ONCE_DO_WHILE) {
				if (!mClockwiseFlag)
					continue ;
				for (auto &&i : mPloygonVertexList)
					i = mVertex.length () + ~i ;
			}
			mTriangleList = Queue<ARRAY3<INDEX>> () ;
			mTriangle = Array<ARRAY3<INDEX>> () ;
		}

		inline Deque<INDEX> ploygon_vertex_list () const {
			Deque<INDEX> ret = Deque<INDEX> (mVertex.length ()) ;
			for (INDEX i = 0 ; i < mVertex.length () ; i++) {
				INDEX ix = i ;
				INDEX iy = (i + 1) % mVertex.length () ;
				if (_ABS_ (mVertex[iy][0] - mVertex[ix][0]) < mTolerance && _ABS_ (mVertex[iy][1] - mVertex[ix][1]) < mTolerance)
					continue ;
				ret.add (i) ;
			}
			return std::move (ret) ;
		}

		inline UNIT ploygon_vertex_clockwise () const {
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < mPloygonVertexList.length () ; i++) {
				INDEX ix = mPloygonVertexList.access ((i - 1 + mPloygonVertexList.length ()) % mPloygonVertexList.length ()) ;
				INDEX iy = mPloygonVertexList.access (i) ;
				INDEX iz = mPloygonVertexList.access ((i + 1) % mPloygonVertexList.length ()) ;
				const auto r1x = math_cross_product_z (mVertex ,mPloygonVertexList[iy] ,mPloygonVertexList[ix] ,mPloygonVertexList[iz]) ;
				ret -= _SIGN_ (r1x) ;
			}
			return std::move (ret) ;
		}

		inline void generate () {
			while (TRUE) {
				if (mPloygonVertexList.length () <= 2)
					break ;
				INDEX ix = sharp_vertex_one () ;
				if (ix == VAR_NONE)
					break ;
				INDEX jx = mTriangleList.insert () ;
				const auto r1x = ARRAY3<INDEX> ({
					mPloygonVertexList.access ((ix + 1) % mPloygonVertexList.length ()) ,
					mPloygonVertexList.access (ix) ,
					mPloygonVertexList.access ((ix - 1 + mPloygonVertexList.length ()) % mPloygonVertexList.length ())}) ;
				mTriangleList[jx][0] = mPloygonVertexList[r1x[0]] ;
				mTriangleList[jx][1] = mPloygonVertexList[r1x[1]] ;
				mTriangleList[jx][2] = mPloygonVertexList[r1x[2]] ;
				mPloygonVertexList.remove (mPloygonVertexList.access (ix)) ;
			}
			update_triangle () ;
		}

		inline INDEX sharp_vertex_one () const {
			for (INDEX i = 0 ; i < mPloygonVertexList.length () ; i++)
				if (sharp_vertex_each (i))
					return i ;
			return VAR_NONE ;
		}

		inline BOOL sharp_vertex_each (INDEX it) const {
			INDEX ix = mPloygonVertexList.access ((it - 1 + mPloygonVertexList.length ()) % mPloygonVertexList.length ()) ;
			INDEX iy = mPloygonVertexList.access (it) ;
			INDEX iz = mPloygonVertexList.access ((it + 1) % mPloygonVertexList.length ()) ;
			const auto r1x = math_cross_product_z (mVertex ,mPloygonVertexList[ix] ,mPloygonVertexList[iy] ,mPloygonVertexList[iz]) ;
			if (r1x > UNIT (0))
				return FALSE ;
			if (r1x == UNIT (0))
				return FALSE ;
			if (!edge_triangle (mPloygonVertexList[ix] ,mPloygonVertexList[iy] ,mPloygonVertexList[iz]))
				return FALSE ;
			return TRUE ;
		}

		inline BOOL edge_triangle (INDEX v1 ,INDEX v2 ,INDEX v3) const {
			for (auto &&i : mPloygonVertexList)
				if (!edge_triangle_each (v1 ,v2 ,v3 ,i))
					return FALSE ;
			return TRUE ;
		}

		inline BOOL edge_triangle_each (INDEX v1 ,INDEX v2 ,INDEX v3 ,INDEX v4) const {
			if (v4 == v1 || v4 == v2 || v4 == v3)
				return TRUE ;
			const auto r1x = math_cross_product_z (mVertex ,v1 ,v2 ,v4) ;
			const auto r2x = math_cross_product_z (mVertex ,v2 ,v3 ,v4) ;
			const auto r3x = math_cross_product_z (mVertex ,v3 ,v1 ,v4) ;
			if (r1x < UNIT (0) && r2x < UNIT (0) && r3x <= UNIT (0))
				return FALSE ;
			if (r1x < UNIT (0) && r2x <= UNIT (0) && r3x < UNIT (0))
				return FALSE ;
			if (r1x <= UNIT (0) && r2x < UNIT (0) && r3x < UNIT (0))
				return FALSE ;
			return TRUE ;
		}

		inline UNIT math_cross_product_z (const Array<ARRAY2<UNIT>> &vertex ,INDEX v1 ,INDEX v2 ,INDEX v3) const {
			const auto r1x = (vertex[v2][0] - vertex[v1][0]) * (vertex[v3][1] - vertex[v1][1]) ;
			const auto r2x = (vertex[v2][1] - vertex[v1][1]) * (vertex[v3][0] - vertex[v1][0]) ;
			return r1x - r2x ;
		}

		inline void update_triangle () {
			mTriangle = Array<ARRAY3<INDEX>> (mTriangleList.length ()) ;
			for (auto &&i : mTriangle)
				mTriangleList.take (i) ;
			if (!mClockwiseFlag)
				return ;
			for (auto &&i : mTriangle)
				_SWAP_ (i[1] ,i[2]) ;
		}

		inline void refresh () {
			mContext.mTriangle = std::move (mTriangle) ;
		}
	} ;
	_CALL_ (Lambda (*this ,vertex)) ;
}

template <class UNIT>
class BFGSAlgorithm {
private:
	Array<UNIT> mDX ;
	UNIT mDXLoss ;

public:
	BFGSAlgorithm () = delete ;

	explicit BFGSAlgorithm (const Function<UNIT (const Array<UNIT> &)> &loss ,const Array<UNIT> &fdx) {
		initialize (loss ,fdx) ;
	}

	const Array<UNIT> &query () const & {
		return mDX ;
	}

	Array<UNIT> query () && {
		return std::move (mDX) ;
	}

	UNIT query_loss () const {
		return mDXLoss ;
	}

private:
	void initialize (const Function<UNIT (const Array<UNIT> &)> &loss ,const Array<UNIT> &fdx) ;
} ;

template <class UNIT>
inline void BFGSAlgorithm<UNIT>::initialize (const Function<UNIT (const Array<UNIT> &)> &loss ,const Array<UNIT> &fdx) {
	class Lambda {
	private:
		BFGSAlgorithm &mContext ;
		const Function<UNIT (const Array<UNIT> &)> &mLossFunc ;
		const Array<UNIT> &mFDX ;
		const UNIT mTolerance = UNIT (1E-6) ;
		const UNIT mDXLambdaFirst = UNIT (1000) ;
		const UNIT mDXLambdaPower = UNIT (0.618) ;
		const ARRAY2<UNIT> mDXC1C2 = ARRAY2<UNIT> {UNIT (1E-4) ,UNIT (0.9)} ;

		Array<UNIT> mDX ;
		Bitmap<UNIT> mDM ;
		Array<UNIT> mDG ;
		ARRAY3<UNIT> mDXLoss ;
		ARRAY3<UNIT> mDXLambda ;
		Array<UNIT> mIX ;
		Bitmap<UNIT> mIM ;
		Array<UNIT> mIG ;
		Array<UNIT> mIS ;
		Array<UNIT> mIY ;
		Array<UNIT> mSX ;

	public:
		inline explicit Lambda (BFGSAlgorithm &context ,const Function<UNIT (const Array<UNIT> &)> &loss ,const Array<UNIT> &fdx) popping : mContext (context) ,mLossFunc (loss) ,mFDX (fdx) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mDX = mFDX ;
			mDM = Bitmap<UNIT> (mDX.size () ,mDX.size ()) ;
			mDM.fill (UNIT (0)) ;
			for (INDEX i = 0 ; i < mDM.cy () ; i++)
				mDM[i][i] = UNIT (1) ;
			mDG = Array<UNIT> (mDX.size ()) ;
			mIX = Array<UNIT> (mDX.size ()) ;
			mIM = Bitmap<UNIT> (mDX.size () ,mDX.size ()) ;
			mIG = Array<UNIT> (mDX.size ()) ;
			mIS = Array<UNIT> (mDX.size ()) ;
			mIY = Array<UNIT> (mDX.size ()) ;
			mSX = Array<UNIT> (mDX.size ()) ;
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

		inline void compute_gradient_of_loss (const Array<UNIT> &dx ,Array<UNIT> &dg ,Array<UNIT> &sx) const {
			for (INDEX i = 0 ; i < dx.length () ; i++)
				sx[i] = dx[i] ;
			_STATIC_WARNING_ ("mark") ;
			const auto r1x = _PINV_ (mTolerance) ;
			for (INDEX i = 0 ; i < dg.length () ; i++) {
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
			for (INDEX i = 0 ; i < mDG.length () ; i++)
				mIS[i] = -math_matrix_mul (mDM ,i ,mDG) ;
			const auto r1x = math_vector_dot (mDG ,mIS) ;
			mDXLoss[0] = mLossFunc (mDX) ;
			mDXLoss[2] = mDXLoss[0] ;
			mDXLambda[0] = UNIT (0) ;
			mDXLambda[1] = mDXLambdaFirst ;
			mDXLambda[2] = mDXLambda[0] ;
			while (TRUE) {
				if (mDXLambda[1] < mTolerance)
					break ;
				if (mDXLoss[2] <= UNIT (0))
					break ;
				for (INDEX i = 0 ; i < mIX.length () ; i++)
					mIX[i] = mDX[i] + mIS[i] * mDXLambda[1] ;
				mDXLoss[1] = mLossFunc (mIX) ;
				for (FOR_ONCE_DO_WHILE) {
					if (mDXLoss[1] - mDXLoss[0] > mDXLambda[1] * mDXC1C2[0] * r1x)
						continue ;
					compute_gradient_of_loss (mIX ,mIG ,mSX) ;
					if (_ABS_ (math_vector_dot (mIG ,mIS)) > -mDXC1C2[1] * r1x)
						continue ;
					mDXLoss[2] = UNIT (0) ;
				}
				for (FOR_ONCE_DO_WHILE) {
					if (mDXLoss[1] >= mDXLoss[2])
						continue ;
					mDXLoss[2] = mDXLoss[1] ;
					mDXLambda[2] = mDXLambda[1] ;
				}
				mDXLambda[1] *= mDXLambdaPower ;
			}
			_CALL_IF_ ([&] (BOOL &_case_req) {
				_CASE_REQUIRE_ (mDXLoss[0] >= mDXLoss[2]) ;
				const auto r2x = (mDXLoss[2] > UNIT (0)) ? (mDXLoss[2]) : (mDXLoss[1]) ;
				mDXLoss[0] = r2x ;
				_SWAP_ (mDX ,mIX) ;
				compute_gradient_of_loss (mDX ,mIG ,mSX) ;
			} ,[&] (BOOL &_case_req) {
				mIG.fill (UNIT (0)) ;
			}) ;
		}

		inline UNIT math_matrix_mul (const Bitmap<UNIT> &mat ,INDEX y ,const Array<UNIT> &v) const {
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < v.length () ; i++)
				ret += mat[y][i] * v[i] ;
			return std::move (ret) ;
		}

		inline UNIT math_vector_dot (const Array<UNIT> &v1 ,const Array<UNIT> &v2) const {
			_DEBUG_ASSERT_ (v1.length () == v2.length ()) ;
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < v1.length () ; i++)
				ret += v1[i] * v2[i] ;
			return std::move (ret) ;
		}

		inline UNIT current_convergence () const {
			UNIT ret = UNIT (0) ;
			for (auto &&i : mIG)
				ret += _SQE_ (i) ;
			ret = _SQRT_ (ret) ;
			return std::move (ret) ;
		}

		inline void update_iy_and_dg () {
			for (INDEX i = 0 ; i < mIG.length () ; i++) {
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

		inline UNIT hessian_matrix_each (INDEX y ,INDEX x ,const UNIT &ys) const {
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < mDM.cy () ; i++) {
				const auto r1x = hessian_matrix_each_factor (x ,i ,ys) ;
				ret += r1x * (-mIS[y] * mIY[i] * ys) ;
				if (i == y)
					continue ;
				ret += r1x ;
			}
			return std::move (ret) ;
		}

		inline UNIT hessian_matrix_each_factor (INDEX x ,INDEX z ,const UNIT &ys) const {
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < mDM.cx () ; i++) {
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
	_CALL_ (Lambda (*this ,loss ,fdx)) ;
}

template <class UNIT>
class KDimensionTreeAlgorithm {
private:
	class Node {
	private:
		friend KDimensionTreeAlgorithm ;
		UNIT mKey ;
		INDEX mLeaf ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		inline Node () = delete ;

		inline explicit Node (const UNIT &key ,INDEX leaf ,INDEX left ,INDEX right) :mKey (std::move (key)) ,mLeaf (leaf) ,mLeft (left) ,mRight (right) {}
	} ;

private:
	Array<ARRAY3<UNIT>> mVertex ;
	ARRAY3<INDEX> mNextRot ;
	ARRAY3<ARRAY2<UNIT>> mBound ;
	Allocator<Node ,SAUTO> mHeap ;
	INDEX mRoot ;

public:
	KDimensionTreeAlgorithm () = delete ;

	explicit KDimensionTreeAlgorithm (const Array<ARRAY3<UNIT>> &vertex) {
		_DEBUG_ASSERT_ (vertex.length () > 0) ;
		initialize (vertex) ;
	}

	Queue<INDEX> query (const ARRAY3<UNIT> &point ,const UNIT &width) const {
		_DEBUG_ASSERT_ (width > UNIT (0)) ;
		Queue<INDEX> ret ;
		auto rax = ARRAY3<ARRAY2<UNIT>> () ;
		rax[0][0] = _MAX_ ((point[0] - width) ,mBound[0][0]) ;
		rax[0][1] = _MIN_ ((point[0] + width) ,mBound[0][1]) ;
		rax[1][0] = _MAX_ ((point[1] - width) ,mBound[1][0]) ;
		rax[1][1] = _MIN_ ((point[1] + width) ,mBound[1][1]) ;
		rax[2][0] = _MAX_ ((point[2] - width) ,mBound[2][0]) ;
		rax[2][1] = _MIN_ ((point[2] + width) ,mBound[2][1]) ;
		compute_search_range (point ,_SQE_ (width) ,mRoot ,0 ,rax ,ret) ;
		return std::move (ret) ;
	}

	Array<PACK<INDEX ,UNIT>> query_nearst (const ARRAY3<UNIT> &point ,LENGTH count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		const auto r1x = first_count_vertex (point ,count) ;
		const auto r2x = r1x.esort () ;
		Array<PACK<INDEX ,UNIT>> ret = Array<PACK<INDEX ,UNIT>> (count) ;
		for (INDEX i = 0 ; i < ret.length () ; i++) {
			ret[i].P1 = r2x[i] ;
			ret[i].P2 = r1x[r2x[i]] ;
		}
		compute_search_range (point ,mRoot ,0 ,ret) ;
		return std::move (ret) ;
	}

private:
	void initialize (const Array<ARRAY3<UNIT>> &vertex) ;

	void compute_search_range (const ARRAY3<UNIT> &point ,const UNIT &sqe_range ,INDEX it ,INDEX rot ,ARRAY3<ARRAY2<UNIT>> &bound ,Queue<INDEX> &out) const {
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mHeap[it].mLeaf != VAR_NONE) ;
			for (FOR_ONCE_DO_WHILE) {
				INDEX ix = mHeap[it].mLeaf ;
				const auto r2x = _SQE_ (mVertex[ix][0] - point[0]) + _SQE_ (mVertex[ix][1] - point[1]) + _SQE_ (mVertex[ix][2] - point[2]) ;
				if (r2x > sqe_range)
					continue ;
				out.add (ix) ;
			}
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mHeap[it].mLeaf == VAR_NONE) ;
			const auto r3x = mHeap[it].mKey ;
			for (FOR_ONCE_DO_WHILE) {
				if (r3x < bound[rot][0])
					continue ;
				const auto r4x = bound[rot][1] ;
				bound[rot][1] = _MIN_ (bound[rot][1] ,r3x) ;
				compute_search_range (point ,sqe_range ,mHeap[it].mLeft ,mNextRot[rot] ,bound ,out) ;
				bound[rot][1] = r4x ;
			}
			for (FOR_ONCE_DO_WHILE) {
				if (r3x > bound[rot][1])
					continue ;
				const auto r5x = bound[rot][0] ;
				bound[rot][0] = _MAX_ (bound[rot][0] ,r3x) ;
				compute_search_range (point ,sqe_range ,mHeap[it].mRight ,mNextRot[rot] ,bound ,out) ;
				bound[rot][0] = r5x ;
			}
		}) ;
	}

	Stack<UNIT> first_count_vertex (const ARRAY3<UNIT> &point ,LENGTH count) const {
		_DEBUG_ASSERT_ (count >= 1 && count <= mVertex.length ()) ;
		Stack<UNIT> ret = Stack<UNIT> (count) ;
		for (INDEX i = 0 ; i < count ; i++) {
			const auto r1x = _SQE_ (mVertex[i][0] - point[0]) + _SQE_ (mVertex[i][1] - point[1]) + _SQE_ (mVertex[i][2] - point[2]) ;
			ret.add (r1x) ;
		}
		return std::move (ret) ;
	}

	void compute_search_range (const ARRAY3<UNIT> &point ,INDEX it ,INDEX rot ,Array<PACK<INDEX ,UNIT>> &out) const {
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mHeap[it].mLeaf != VAR_NONE) ;
			for (FOR_ONCE_DO_WHILE) {
				INDEX ix = mHeap[it].mLeaf ;
				const auto r2x = (Vector<UNIT> {mVertex[ix] ,UNIT (0)} -Vector<UNIT> {point ,UNIT (0)}).magnitude () ;
				INDEX jx = out.length () ;
				while (TRUE) {
					if (jx - 1 < 0)
						break ;
					if (r2x >= out[jx - 1].P2)
						break ;
					if (jx < out.length ())
						out[jx] = out[jx - 1] ;
					jx-- ;
				}
				if (jx >= out.length ())
					continue ;
				out[jx].P1 = ix ;
				out[jx].P2 = r2x ;
			}
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mHeap[it].mLeaf == VAR_NONE) ;
			const auto r3x = mHeap[it].mKey ;
			if (r3x >= point[rot] - out[out.length () - 1].P2)
				compute_search_range (point ,mHeap[it].mLeft ,mNextRot[rot] ,out) ;
			if (r3x <= point[rot] + out[out.length () - 1].P2)
				compute_search_range (point ,mHeap[it].mRight ,mNextRot[rot] ,out) ;
		}) ;
	}
} ;

template <class UNIT>
inline void KDimensionTreeAlgorithm<UNIT>::initialize (const Array<ARRAY3<UNIT>> &vertex) {
	class Lambda {
	private:
		KDimensionTreeAlgorithm &mContext ;
		const Array<ARRAY3<UNIT>> &mVertex ;

		ARRAY3<INDEX> mNextRot ;
		ARRAY3<Array<INDEX>> mOrder ;
		ARRAY3<ARRAY2<UNIT>> mBound ;
		Allocator<Node ,SAUTO> mHeap ;
		INDEX mRoot ;
		INDEX mLatestIndex ;

		Array<INDEX> mTempOrder ;

	public:
		inline explicit Lambda (KDimensionTreeAlgorithm &context ,const Array<ARRAY3<UNIT>> &vertex) popping : mContext (context) ,mVertex (vertex) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mNextRot = ARRAY3<INDEX> {1 ,2 ,0} ;
			for (INDEX i = 0 ; i < mOrder.length () ; i++)
				mOrder[i] = stack_of_order (i).esort () ;
			mHeap = Allocator<Node ,SAUTO> (mVertex.length ()) ;
			mRoot = VAR_NONE ;
		}

		Stack<INDEX> stack_of_order (INDEX rot) const {
			Stack<INDEX> ret = Stack<INDEX> (mVertex.length ()) ;
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
			for (FOR_ONCE_DO_WHILE) {
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

		void update_build_tree (INDEX it ,INDEX rot ,INDEX seg ,INDEX seg_len) {
			_DEBUG_ASSERT_ (seg_len > 0) ;
			_DEBUG_ASSERT_ (seg >= 0 && seg + seg_len <= mVertex.size ()) ;
			_CALL_IF_ ([&] (BOOL &_case_req) {
				_CASE_REQUIRE_ (seg_len == 1) ;
				INDEX jx = mHeap.alloc (UNIT (0) ,mOrder[rot][seg] ,VAR_NONE ,VAR_NONE) ;
				mLatestIndex = jx ;
			} ,[&] (BOOL &_case_req) {
				_CASE_REQUIRE_ (seg_len > 1) ;
				INDEX ix = seg + seg_len / 2 ;
				for (INDEX i = seg ,ie = seg + seg_len - 1 ; i < ie ; i++)
					_DEBUG_ASSERT_ (mVertex[mOrder[rot][i]][rot] <= mVertex[mOrder[rot][i + 1]][rot]) ;
				compute_order (mTempOrder ,mOrder ,rot ,mNextRot[rot] ,seg ,ix ,seg_len) ;
				compute_order (mTempOrder ,mOrder ,rot ,mNextRot[mNextRot[rot]] ,seg ,ix ,seg_len) ;
				INDEX jx = mHeap.alloc (mVertex[mOrder[rot][ix]][rot] ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
				update_build_tree (mHeap[jx].mLeft ,mNextRot[rot] ,seg ,(ix - seg)) ;
				mHeap[jx].mLeft = mLatestIndex ;
				update_build_tree (mHeap[jx].mRight ,mNextRot[rot] ,ix ,(seg_len - (ix - seg))) ;
				mHeap[jx].mRight = mLatestIndex ;
				mLatestIndex = it ;
			}) ;
		}

		void compute_order (Array<INDEX> &tmp_order ,ARRAY3<Array<INDEX>> &order ,INDEX rot ,INDEX n_rot ,INDEX seg_a ,INDEX seg_b ,LENGTH seg_len) const {
			if (tmp_order.size () != mVertex.size ())
				tmp_order = Array<INDEX> (mVertex.size ()) ;
			INDEX iw = 0 ;
			for (INDEX i = seg_a ; i < seg_b ; i++)
				tmp_order[iw++] = mOrder[n_rot][i] ;
			for (INDEX i = seg_b ,ie = seg_a + seg_len ; i < ie ; i++)
				tmp_order[iw++] = mOrder[n_rot][i] ;
			const auto r1x = ARRAY2<INDEX> {0 ,iw} ;
			for (INDEX i = r1x[0] ; i < r1x[1] ; i++)
				order[n_rot][seg_a + i] = tmp_order[i] ;
			_DEBUG_ASSERT_ (iw == seg_len) ;
		}

		inline void refresh () {
			mContext.mVertex = mVertex ;
			mContext.mNextRot = mNextRot ;
			mContext.mBound = std::move (mBound) ;
			mContext.mHeap = std::move (mHeap) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	_CALL_ (Lambda (*this ,vertex)) ;
}

template <class UNIT>
class MaxFlowAlgorithm {
private:
	Bitmap<UNIT> mCurrentFlow ;
	UNIT mMaxFlow ;

public:
	MaxFlowAlgorithm () = delete ;

	explicit MaxFlowAlgorithm (const Bitmap<UNIT> &adjacency ,INDEX source ,INDEX sink) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		_DEBUG_ASSERT_ (source != sink) ;
		initialize (adjacency ,source ,sink) ;
	}

	UNIT query () const {
		return mMaxFlow ;
	}

	const Bitmap<UNIT> &query_flow () const & {
		return mCurrentFlow ;
	}

	Bitmap<UNIT> query_flow () && {
		return std::move (mCurrentFlow) ;
	}

private:
	void initialize (const Bitmap<UNIT> &adjacency ,INDEX source ,INDEX sink) ;
} ;

template <class UNIT>
inline void MaxFlowAlgorithm<UNIT>::initialize (const Bitmap<UNIT> &adjacency ,INDEX source ,INDEX sink) {
	class Lambda {
	private:
		MaxFlowAlgorithm &mContext ;
		const Bitmap<UNIT> &mAdjacency ;

		INDEX mSource ;
		INDEX mSink ;
		UNIT mSingleFlow ;
		Bitmap<UNIT> mCurrentFlow ;
		Array<INDEX> mBFSPath ;

		Queue<INDEX> mTempQueue ;

	public:
		inline explicit Lambda (MaxFlowAlgorithm &context ,const Bitmap<UNIT> &adjacency ,INDEX source ,INDEX sink) popping : mContext (context) ,mAdjacency (adjacency) ,mSource (source) ,mSink (sink) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mSingleFlow = single_flow () ;
			mCurrentFlow = Bitmap<UNIT> (mAdjacency.cx () ,mAdjacency.cy ()) ;
			mCurrentFlow.fill (UNIT (0)) ;
			mBFSPath = Array<INDEX> (mAdjacency.cx ()) ;
		}

		inline UNIT single_flow () const {
			UNIT ret = UNIT (0) ;
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
				mTempQueue = Queue<INDEX> (mAdjacency.cx ()) ;
			mTempQueue.clear () ;
			mBFSPath.fill (VAR_NONE) ;
			mTempQueue.add (mSink) ;
			while (TRUE) {
				if (mTempQueue.empty ())
					break ;
				INDEX ix = mTempQueue[mTempQueue.peek ()] ;
				mTempQueue.take () ;
				for (INDEX i = 0 ; i < mAdjacency.cy () ; i++) {
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

		inline UNIT augument_max_flow () const {
			UNIT ret = mSingleFlow ;
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

		inline UNIT max_flow () const {
			UNIT ret = UNIT (0) ;
			for (INDEX i = 0 ; i < mCurrentFlow.cy () ; i++)
				ret += mCurrentFlow[i][mSink] ;
			return std::move (ret) ;
		}
	} ;
	_CALL_ (Lambda (*this ,adjacency ,source ,sink)) ;
}
} ;