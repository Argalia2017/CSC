#pragma once

#ifndef __CSC_ALGORITHM__
#define __CSC_ALGORITHM__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_images.hpp"
#include "csc_graphics.hpp"

namespace CSC {
class PrimeSieveAlgorithm {
private:
	BitSet<> mPrimeSet ;

public:
	PrimeSieveAlgorithm () = delete ;

	explicit PrimeSieveAlgorithm (LENGTH len) {
		initialize (len) ;
	}

	const BitSet<> &query () const {
		return mPrimeSet ;
	}

private:
	void initialize (LENGTH len) {
		mPrimeSet = BitSet<> (len) ;
		mPrimeSet.fill (BYTE (0XAA)) ;
		mPrimeSet[1] = FALSE ;
		mPrimeSet[2] = TRUE ;
		for (INDEX i = 3 ,ie = _SQRT_ (mPrimeSet.size ()) + 2 ; i < ie ; i += 2)
			for (INDEX j = _SQE_ (i) ; j < mPrimeSet.size () ; j += i * 2)
				mPrimeSet[j] = FALSE ;
	}
} ;

template <class ITEM>
class KMPAlgorithm {
private:
	Array<INDEX> mNext ;
	Array<ITEM> mPattern ;

public:
	KMPAlgorithm () = delete ;

	explicit KMPAlgorithm (const PhanBuffer<const ITEM> &pattern) {
		initialize (pattern) ;
	}

	INDEX query (const PhanBuffer<const ITEM> &target ,INDEX offset) const {
		_DEBUG_ASSERT_ (offset >= 0 && offset < target.size ()) ;
		INDEX ret = VAR_NONE ;
		INDEX ir = target.size () - offset >= mNext.length () ? offset : target.size () ;
		INDEX jr = 0 ;
		while (ir < target.size () && jr < mNext.length ()) {
			while (jr != VAR_NONE && target[ir] != mPattern[jr])
				jr = mNext[jr] ;
			ir++ ;
			jr++ ;
		}
		if (jr >= mNext.length ())
			ret = ir - mNext.length () ;
		return std::move (ret) ;
	}

private:
	void initialize (const PhanBuffer<const ITEM> &pattern) {
		mNext = Array<INDEX> (pattern.size ()) ;
		mPattern = Array<ITEM> (pattern.size ()) ;
		INDEX iw = 0 ;
		INDEX jw = VAR_NONE ;
		mNext[iw] = VAR_NONE ;
		mPattern[iw] = pattern[iw] ;
		while (iw < pattern.size () - 1) {
			while (jw != VAR_NONE && pattern[iw] != pattern[jw])
				jw = mNext[jw] ;
			iw++ ;
			jw++ ;
			mNext[iw] = find_next (iw ,jw) ;
			mPattern[iw] = pattern[iw] ;
		}
	}

	inline INDEX find_next (INDEX slow ,INDEX fast) const {
		for (INDEX i = fast ; i != VAR_NONE ; i = mNext[i])
			if (mPattern[i] != mPattern[slow])
				return i ;
		return VAR_NONE ;
	}
} ;

template <class ITEM>
class DijistraAlgorithm {
private:
	Array<INDEX> mPrev ;
	Array<ITEM> mDistance ;
	INDEX mRoot ;

public:
	DijistraAlgorithm () = delete ;

	explicit DijistraAlgorithm (const SoftImage<ITEM> &adjacency ,INDEX root) {
		_DEBUG_ASSERT_ (adjacency.cx () == adjacency.cy ()) ;
		initialize (adjacency ,root) ;
	}

	const ITEM &query (INDEX index) const {
		return mDistance[index] ;
	}

	Array<INDEX> query_path (INDEX index) const {
		Array<INDEX> ret = Array<INDEX> (query_path_depth (index)) ;
		INDEX iw = ret.length () ;
		for (INDEX i = index ; i != VAR_NONE ; i = mPrev[i])
			ret[--iw] = i ;
		return std::move (ret) ;
	}

private:
	void initialize (const SoftImage<ITEM> &adjacency ,INDEX root) ;

	inline LENGTH query_path_depth (INDEX index) const {
		LENGTH ret = 0 ;
		for (INDEX i = index ; i != VAR_NONE ; i = mPrev[i])
			ret++ ;
		return std::move (ret) ;
	}
} ;

template <class ITEM>
inline void DijistraAlgorithm<ITEM>::initialize (const SoftImage<ITEM> &adjacency ,INDEX root) {
	class Lambda {
	private:
		class Element {
		private:
			PTR<Array<ITEM>> mDistance ;
			INDEX mIndex ;

		public:
			inline Element () = default ;

			inline explicit Element (PTR<Array<ITEM>> distance ,INDEX index) :mDistance (distance) ,mIndex (index) {}

			inline BOOL operator== (const Element &right) {
				return (*mDistance)[mIndex] == (*right.mDistance)[mIndex] ;
			}

			inline BOOL operator!= (const Element &right) {
				return (*mDistance)[mIndex] != (*right.mDistance)[mIndex] ;
			}

			inline BOOL operator< (const Element &right) {
				return (*mDistance)[mIndex] < (*right.mDistance)[mIndex] ;
			}

			inline BOOL operator>= (const Element &right) {
				return (*mDistance)[mIndex] >= (*right.mDistance)[mIndex] ;
			}

			inline BOOL operator> (const Element &right) {
				return (*mDistance)[mIndex] > (*right.mDistance)[mIndex] ;
			}

			inline BOOL operator<= (const Element &right) {
				return (*mDistance)[mIndex] <= (*right.mDistance)[mIndex] ;
			}
		} ;

	private:
		DijistraAlgorithm & mContext ;
		const SoftImage<ITEM> &mAdjacency ;
		INDEX mRoot ;
		Array<INDEX> mPrev ;
		Array<ITEM> mDistance ;
		Priority<Element> mPriority ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;

	public:
		inline explicit Lambda (DijistraAlgorithm &context ,const SoftImage<ITEM> &adjancency ,INDEX root) :mContext (context) ,mAdjacency (adjancency) ,mRoot (root) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mPrev = Array<INDEX> (mAdjacency.cx ()) ;
			mPrev.fill (VAR_NONE) ;
			mDistance = Array<ITEM> (mAdjacency.cx ()) ;
			mDistance[mRoot] = ITEM (0) ;
			mXVisit = BitSet<> (mAdjacency.cx ()) ;
			mXVisit[mRoot] = TRUE ;
			mYVisit = BitSet<> (mAdjacency.cy ()) ;
			mPriority = Priority<Element> () ;
			mPriority.add (Element (&mDistance ,mRoot)) ;
		}

		inline void generate () {
			while (!mPriority.empty ())
				update_distance (mPriority.poll ()) ;
		}

		inline void update_distance (INDEX y) {
			if (mYVisit[y])
				return ;
			mYVisit[y] = TRUE ;
			for (INDEX i = 0 ; i < mPrev.length () ; i++) {
				if (i == y)
					continue ;
				if (mAdjacency[y][i] < ITEM (0))
					continue ;
				const auto r1x = mDistance[y] + mAdjacency[y][i] ;
				if (mXVisit[i] && mDistance[i] <= r1x)
					continue ;
				mPrev[i] = y ;
				mDistance[i] = r1x ;
				mXVisit[i] = TRUE ;
				mPriority.add (Element (&mDistance ,i)) ;
			}
		}

		inline void refresh () {
			mContext.mPrev = std::move (mPrev) ;
			mContext.mDistance = std::move (mDistance) ;
			mContext.mRoot = mRoot ;
		}
	} ;
	Lambda (*this ,adjacency ,root) () ;
}

template <class ITEM>
class KMeansAlgorithm {
private:
	Set<BitSet<>> mClusterSet ;

public:
	KMeansAlgorithm () = delete ;

	explicit KMeansAlgorithm (const Set<ITEM> &dataset ,const Function<ITEM (const ITEM & ,const ITEM &)> &distance ,const Array<ITEM> &center) {
		_DEBUG_ASSERT_ (dataset.length () >= 2 && center.length () >= 2) ;
		initialize (dataset ,distance ,center) ;
	}

	const Set<BitSet<>> &query () const {
		return mClusterSet ;
	}

private:
	void initialize (const Set<ITEM> &dataset ,const Function<ITEM (const ITEM & ,const ITEM &)> &distance ,const Array<ITEM> &center) ;
} ;

template <class ITEM>
inline void KMeansAlgorithm<ITEM>::initialize (const Set<ITEM> &dataset ,const Function<ITEM (const ITEM & ,const ITEM &)> &distance ,const Array<ITEM> &center) {
	class Lambda {
	private:
		KMeansAlgorithm & mContext ;
		const Set<ITEM> &mDataSet ;
		const Function<ITEM (const ITEM & ,const ITEM &)> &mDistanceProc ;
		const Array<ITEM> &mCenter ;
		ITEM mConvergenceEPS ;
		Queue<ITEM> mCurrCenterList ;
		Queue<ITEM> mNextCenterList ;
		Set<INDEX ,BitSet<>> mClusterSet ;
		ARRAY3<ITEM> mConvergence ;

	public:
		inline explicit Lambda (KMeansAlgorithm &context ,const Set<ITEM> &dataset ,const Function<ITEM (const ITEM & ,const ITEM &)> &distance ,const Array<ITEM> &center) :mContext (context) ,mDistanceProc (distance) ,mDataSet (dataset) ,mCenter (center) {
			mConvergenceEPS = ITEM (1E-6) ;
		}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mCurrCenterList = Queue<ITEM> (mCenter.length ()) ;
			mCurrCenterList.appand (mCenter) ;
			mNextCenterList = Queue<ITEM> (mCenter.length ()) ;
			mClusterSet = Set<INDEX ,BitSet<>> () ;
			mConvergence.fill (ITEM (VAL32_INF)) ;
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
			for (INDEX i = mDataSet.ibegin () ,ie = mDataSet.iend () ; i != ie ; i = mDataSet.inext (i)) {
				INDEX ix = closest_center_of_point (mDataSet[i]) ;
				INDEX iy = mClusterSet.insert (ix) ;
				if (mClusterSet[iy].item.size () == 0)
					mClusterSet[iy].item = BitSet<> (mDataSet.size ()) ;
				mClusterSet[iy].item[i] = TRUE ;
			}
			for (INDEX i = 0 ; i < mCenter.length () ; i++)
				mClusterSet.insert (i) ;
		}

		inline INDEX closest_center_of_point (const ITEM &point) const {
			INDEX ret = VAR_NONE ;
			auto rax = ITEM () ;
			for (INDEX i = mCurrCenterList.ibegin () ,ie = mCurrCenterList.iend () ; i != ie ; i = mCurrCenterList.inext (i)) {
				const auto r1x = mDistanceProc (point ,mCurrCenterList[i]) ;
				if (ret != VAR_NONE && rax <= r1x)
					continue ;
				ret = i ;
				rax = r1x ;
			}
			return std::move (ret) ;
		}

		inline void update_next_center_list () {
			for (INDEX i = mClusterSet.ibegin () ,ie = mClusterSet.iend () ; i != ie ; i = mClusterSet.inext (i)) {
				INDEX ix = mNextCenterList.emplace () ;
				mNextCenterList[ix] = mClusterSet[i].item.length () > 0 ? average_center (mClusterSet[i].item) : mCurrCenterList[mClusterSet[i].key] ;
			}
		}

		inline ITEM average_center (const BitSet<> &cluster) const {
			ITEM ret = ITEM (0) ;
			const auto r1x = cluster.length () ;
			_DEBUG_ASSERT_ (r1x != 0) ;
			for (const auto &i : cluster)
				ret += mDataSet[i] ;
			ret /= ITEM (r1x) ;
			return std::move (ret) ;
		}

		inline void update_convergence () {
			mConvergence[0] = mConvergence[1] ;
			mConvergence[1] = mConvergence[2] ;
			mConvergence[2] = ITEM (VAL32_INF) ;
			if (mCurrCenterList.length () != mNextCenterList.length ())
				return ;
			mConvergence[2] = ITEM (0) ;
			_DEBUG_ASSERT_ (mCurrCenterList.length () == mNextCenterList.length ()) ;
			for (INDEX i = mCurrCenterList.ibegin () ,ie = mCurrCenterList.iend () ,j = mNextCenterList.ibegin () ,je = mNextCenterList.iend () ; i != ie && j != je ; i = mCurrCenterList.inext (i) ,j = mNextCenterList.inext (j))
				mConvergence[2] = _MAX_ (mConvergence[2] ,mDistanceProc (mCurrCenterList[i] ,mNextCenterList[j])) ;
		}

		inline BOOL reach_convergence () const {
			if (mConvergence[0] < 0 || mConvergence[1] < 0 || mConvergence[2] < 0)
				return FALSE ;
			if (mConvergence[0] > mConvergence[1] || mConvergence[1] > mConvergence[2])
				return FALSE ;
			if (mConvergence[1] - mConvergence[2] >= mConvergenceEPS)
				return FALSE ;
			return TRUE ;
		}

		inline void update_curr_center_list () {
			mCurrCenterList.clear () ;
			for (auto &i : mNextCenterList)
				mCurrCenterList.add (i) ;
			mNextCenterList.clear () ;
		}

		inline void refresh () {
			mContext.mClusterSet = Set<BitSet<>> () ;
			mContext.mClusterSet.appand (mClusterSet) ;
		}
	} ;
	Lambda (*this ,dataset ,distance ,center) () ;
}

template <class ITEM>
class KMHungarianAlgorithm {
private:
	ITEM mWeight ;
	Array<ARRAY2<INDEX>> mMatch ;

public:
	KMHungarianAlgorithm () = delete ;

	explicit KMHungarianAlgorithm (const SoftImage<ITEM> &adjacency) {
		initialize (adjacency) ;
	}

	const ITEM &query () const {
		return mWeight ;
	}

	const Array<ARRAY2<INDEX>> &query_match () const {
		return mMatch ;
	}

private:
	void initialize (const SoftImage<ITEM> &adjacency) ;
} ;

template <class ITEM>
inline void KMHungarianAlgorithm<ITEM>::initialize (const SoftImage<ITEM> &adjacency) {
	class Lambda {
	private:
		KMHungarianAlgorithm & mContext ;
		const SoftImage<ITEM> &mAdjacency ;
		Stack<ARRAY2<INDEX>> mGoStack ;
		ARRAY2<INDEX> mGoXY ;
		BOOL mGoRet ;
		Array<INDEX> mXYLink ;
		BitSet<> mXVisit ;
		BitSet<> mYVisit ;
		BitSet<> mLackVisit ;
		Array<ITEM> mXWeight ;
		Array<ITEM> mYWeight ;
		Array<ITEM> mLackWeight ;

	public:
		inline explicit Lambda (KMHungarianAlgorithm &context ,const SoftImage<ITEM> &adjacency) :mContext (context) ,mAdjacency (adjacency) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mGoStack = Stack<ARRAY2<INDEX>> () ;
			mXYLink = Array<INDEX> (mAdjacency.cx ()) ;
			mXYLink.fill (VAR_NONE) ;
			mXVisit = BitSet<> (mAdjacency.cx ()) ;
			mYVisit = BitSet<> (mAdjacency.cy ()) ;
			mLackVisit = BitSet<> (mAdjacency.cx ()) ;
			mXWeight = Array<ITEM> (mAdjacency.cx ()) ;
			mXWeight.fill (ITEM (0)) ;
			mYWeight = Array<ITEM> (mAdjacency.cy ()) ;
			mYWeight.fill (ITEM (0)) ;
			for (INDEX i = 0 ; i < mAdjacency.cy () ; i++)
				for (INDEX j = 0 ; j < mAdjacency.cx () ; j++)
					mYWeight[i] = _MAX_ (mYWeight[i] ,mAdjacency[i][j]) ;
			mLackWeight = Array<ITEM> (mAdjacency.cx ()) ;
		}

		inline void generate () {
			for (INDEX i = 0 ; i < mAdjacency.cy () ; i++) {
				mLackVisit.clear () ;
				while (TRUE) {
					mXVisit.clear () ;
					mYVisit.clear () ;
					update_lack_weight (i) ;
					INDEX ix = min_lack_weight_one () ;
					if (ix == VAR_NONE)
						break ;
					for (const auto &j : mXVisit)
						mXWeight[j] += mLackWeight[ix] ;
					for (const auto &j : mYVisit)
						mYWeight[j] -= mLackWeight[ix] ;
				}
			}
		}

		inline void update_lack_weight (INDEX y) {
			mGoRet = FALSE ;
			mGoStack.add ({0 ,y}) ;
LABEL_1: //@info:enter recursive call
			mGoStack.take (mGoXY) ;
			if (mGoXY[1] == VAR_NONE) {
				mGoRet = TRUE ;
				goto LABEL_3 ;
			}
			mYVisit[mGoXY[1]] = TRUE ;
			mGoRet = FALSE ;
			for (mGoXY[0] = 0 ; mGoXY[0] < mAdjacency.cx () ; mGoXY[0]++) {
				if (mXVisit[mGoXY[0]])
					continue ;
				if (mYWeight[mGoXY[1]] + mXWeight[mGoXY[0]] - mAdjacency[mGoXY[1]][mGoXY[0]] != 0)
					continue ;
				if (TRUE) {
					const auto r1x = mYWeight[mGoXY[1]] + mXWeight[mGoXY[0]] - mAdjacency[mGoXY[1]][mGoXY[0]] ;
					mLackWeight[mGoXY[0]] = mLackVisit[mGoXY[0]] ? _MIN_ (mLackWeight[mGoXY[0]] ,r1x) : r1x ;
					mLackVisit[mGoXY[0]] = TRUE ;
				}
				mGoStack.add (mGoXY) ;
				mGoStack.add ({0 ,mXYLink[mGoXY[0]]}) ;
				goto LABEL_1 ;
LABEL_2: //@info:return from the previous call
				if (!mGoRet)
					continue ;
				mXYLink[mGoXY[0]] = mGoXY[1] ;
				goto LABEL_3 ;
			}
LABEL_3: //@info:return to the previous call
			if (!mGoStack.empty ()) {
				mGoStack.take (mGoXY) ;
				goto LABEL_2 ;
			}
			if (mGoRet)
				mLackVisit.clear () ;
		}

		inline INDEX min_lack_weight_one () const {
			INDEX ret = VAR_NONE ;
			auto rax = ITEM () ;
			for (const auto &i : mLackVisit) {
				if (ret != VAR_NONE && rax <= mLackWeight[i])
					continue ;
				ret = i ;
				rax = mLackWeight[i] ;
			}
			return std::move (ret) ;
		}

		inline ITEM best_weight () const {
			ITEM ret = ITEM (0) ;
			for (INDEX i = 0 ; i < mXYLink.length () ; i++)
				ret += mAdjacency[mXYLink[i]][i] ;
			return std::move (ret) ;
		}

		inline Array<ARRAY2<INDEX>> best_match () const {
			Array<ARRAY2<INDEX>> ret = Array<ARRAY2<INDEX>> (best_match_depth ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ; i < mXYLink.length () ; i++)
				if (mXYLink[i] != VAR_NONE)
					ret[iw++] = {mXYLink[i] ,i} ;
			return std::move (ret) ;
		}

		inline LENGTH best_match_depth () const {
			LENGTH ret = 0 ;
			for (auto &i : mXYLink)
				ret += EFLAG (i != VAR_NONE) ;
			return std::move (ret) ;
		}

		inline void refresh () {
			mContext.mWeight = best_weight () ;
			mContext.mMatch = best_match () ;
		}
	} ;
	Lambda (*this ,adjacency) () ;
}

template <class ITEM>
class TrianglateAlgorithm {
private:
	Array<ARRAY3<INDEX>> mTriangle ;

public:
	TrianglateAlgorithm () = delete ;

	explicit TrianglateAlgorithm (const Array<ARRAY2<ITEM>> &vertex) {
		_DEBUG_ASSERT_ (vertex.length () >= 3) ;
		initialize (vertex) ;
	}

	const Array<ARRAY3<INDEX>> &query () const {
		return mTriangle ;
	}

private:
	void initialize (const Array<ARRAY2<ITEM>> &vertex) ;
} ;

template <class ITEM>
inline void TrianglateAlgorithm<ITEM>::initialize (const Array<ARRAY2<ITEM>> &vertex) {
	class Lambda {
	private:
		TrianglateAlgorithm & mContext ;
		const Array<ARRAY2<ITEM>> &mVertex ;
		Deque<INDEX> mPloygonVertexList ;
		Queue<ARRAY3<INDEX>> mTriangleList ;
		BOOL mClockwiseFlag ;

	public:
		inline explicit Lambda (TrianglateAlgorithm &context ,const Array<ARRAY2<ITEM>> &vertex) :mContext (context) ,mVertex (vertex) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mPloygonVertexList = Deque<INDEX> (mVertex.length ()) ;
			for (INDEX i = 0 ; i < mVertex.length () ; i++) {
				INDEX ix = (i + 1) % mVertex.length () ;
				if (_ABS_ (mVertex[ix][0] - mVertex[i][0]) < VAL32_EPS && _ABS_ (mVertex[ix][1] - mVertex[i][1]) < VAL32_EPS)
					continue ;
				mPloygonVertexList.add (i) ;
			}
			mClockwiseFlag = ploygon_vertex_clockwise () > 0 ;
			if (mClockwiseFlag) {
				for (auto &i : mPloygonVertexList)
					i = mVertex.length () + ~i ;
			}
			mTriangleList = Queue<ARRAY3<INDEX>> () ;
		}

		inline ITEM ploygon_vertex_clockwise () const {
			ITEM ret = 0 ;
			for (INDEX i = 0 ; i < mPloygonVertexList.length () ; i++) {
				INDEX ix = mPloygonVertexList.access ((i - 1 + mPloygonVertexList.length ()) % mPloygonVertexList.length ()) ;
				INDEX iy = mPloygonVertexList.access (i) ;
				INDEX iz = mPloygonVertexList.access ((i + 1) % mPloygonVertexList.length ()) ;
				const auto r1x = (mVertex[mPloygonVertexList[iy]][0] - mVertex[mPloygonVertexList[ix]][0]) * (mVertex[mPloygonVertexList[iz]][1] - mVertex[mPloygonVertexList[iy]][1]) ;
				const auto r2x = (mVertex[mPloygonVertexList[iy]][1] - mVertex[mPloygonVertexList[ix]][1]) * (mVertex[mPloygonVertexList[iz]][0] - mVertex[mPloygonVertexList[iy]][0]) ;
				ret += _SIGN_ (r1x - r2x) ;
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
				INDEX jx = mTriangleList.emplace () ;
				mTriangleList[jx][2] = mPloygonVertexList[mPloygonVertexList.access ((ix - 1 + mPloygonVertexList.length ()) % mPloygonVertexList.length ())] ;
				mTriangleList[jx][1] = mPloygonVertexList[mPloygonVertexList.access (ix)] ;
				mTriangleList[jx][0] = mPloygonVertexList[mPloygonVertexList.access ((ix + 1) % mPloygonVertexList.length ())] ;
				mPloygonVertexList.remove (mPloygonVertexList.access (ix)) ;
			}
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
			if (math_cross_product_z (mPloygonVertexList[ix] ,mPloygonVertexList[iy] ,mPloygonVertexList[iz]) >= 0)
				return FALSE ;
			if (!edge_triangle (mPloygonVertexList[ix] ,mPloygonVertexList[iy] ,mPloygonVertexList[iz]))
				return FALSE ;
			return TRUE ;
		}

		inline BOOL edge_triangle (INDEX v1 ,INDEX v2 ,INDEX v3) const {
			for (auto &i : mPloygonVertexList)
				if (!edge_triangle_each (v1 ,v2 ,v3 ,i))
					return FALSE ;
			return TRUE ;
		}

		inline BOOL edge_triangle_each (INDEX v1 ,INDEX v2 ,INDEX v3 ,INDEX v4) const {
			if (v4 == v1 || v4 == v2 || v4 == v3)
				return TRUE ;
			const auto r1x = math_cross_product_z (v1 ,v2 ,v4) ;
			const auto r2x = math_cross_product_z (v2 ,v3 ,v4) ;
			const auto r3x = math_cross_product_z (v3 ,v1 ,v4) ;
			if (r1x < 0 && r2x < 0 && r3x <= 0)
				return FALSE ;
			if (r1x < 0 && r2x <= 0 && r3x < 0)
				return FALSE ;
			if (r1x <= 0 && r2x < 0 && r3x < 0)
				return FALSE ;
			return TRUE ;
		}

		inline ITEM math_cross_product_z (INDEX v1 ,INDEX v2 ,INDEX v3) const {
			const auto r1x = (mVertex[v2][0] - mVertex[v1][0]) * (mVertex[v3][1] - mVertex[v1][1]) ;
			const auto r2x = (mVertex[v2][1] - mVertex[v1][1]) * (mVertex[v3][0] - mVertex[v1][0]) ;
			return r1x - r2x ;
		}

		inline void refresh () {
			if (mClockwiseFlag) {
				for (auto &i : mTriangleList) {
					i[0] = mVertex.length () + ~i[0] ;
					i[1] = mVertex.length () + ~i[1] ;
					i[2] = mVertex.length () + ~i[2] ;
				}
			}
			mContext.mTriangle = mTriangleList.poll (mTriangleList.length ()) ;
		}
	} ;
	Lambda (*this ,vertex) () ;
}

template <class ITEM>
class BGFSAlgorithm {
private:
	Array<ITEM> mDX ;
	ITEM mDXLoss ;

public:
	BGFSAlgorithm () = delete ;

	explicit BGFSAlgorithm (const Function<ITEM (const Array<ITEM> &)> &loss ,const Array<ITEM> &dx) {
		initialize (loss ,dx) ;
	}

	const Array<ITEM> &query () const {
		return mDX ;
	}

	const ITEM &query_loss () const {
		return mDXLoss ;
	}

private:
	void initialize (const Function<ITEM (const Array<ITEM> &)> &loss ,const Array<ITEM> &dx) ;
} ;

template <class ITEM>
inline void BGFSAlgorithm<ITEM>::initialize (const Function<ITEM (const Array<ITEM> &)> &loss ,const Array<ITEM> &dx) {
	class Lambda {
	private:
		BGFSAlgorithm & mContext ;
		const Function<ITEM (const Array<ITEM> &)> &mLoss ;
		ITEM mConvergenceEPS ;
		ITEM mDXLambdaXMax ;
		ITEM mDXLambdaPower ;
		Array<ITEM> mDX ;
		SoftImage<ITEM> mDM ;
		Array<ITEM> mDG ;
		ARRAY3<ITEM> mDXLoss ;
		ARRAY3<ITEM> mDXLambda ;
		ARRAY2<ITEM> mDXC1C2 ;
		Array<ITEM> mIX ;
		SoftImage<ITEM> mIM ;
		Array<ITEM> mIG ;
		Array<ITEM> mIS ;
		Array<ITEM> mIY ;
		Array<ITEM> mSX ;

	public:
		inline explicit Lambda (BGFSAlgorithm &context ,const Function<ITEM (const Array<ITEM> &)> &loss ,const Array<ITEM> &dx) :mContext (context) ,mLoss (loss) ,mDX (dx) {
			mConvergenceEPS = ITEM (1E-6) ;
			mDXLambdaXMax = ITEM (1000) ;
			mDXLambdaPower = ITEM (0.618) ;
			mDXC1C2[0] = ITEM (1E-4) ;
			mDXC1C2[1] = ITEM (0.9) ;
		}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mDM = SoftImage<ITEM> (mDX.length () ,mDX.length ()) ;
			mDM.fill (ITEM (0)) ;
			for (INDEX i = 0 ; i < mDM.cy () ; i++)
				mDM[i][i] = ITEM (1) ;
			mIX = Array<ITEM> (mDX.length ()) ;
			mIM = SoftImage<ITEM> (mDM.cx () ,mDM.cy ()) ;
			mIG = Array<ITEM> (mDX.length ()) ;
			mIS = Array<ITEM> (mDX.length ()) ;
			mIY = Array<ITEM> (mDX.length ()) ;
			mSX = Array<ITEM> (mDX.length ()) ;
			mDG = gradient_of_loss (mDX ,Array<ITEM> (mDX.length ()) ,mSX) ;
		}

		inline void generate () {
			while (TRUE) {
				update_is_and_ig () ;
				if (current_convergence () < mConvergenceEPS)
					break ;
				update_iy_and_dg () ;
				update_dm () ;
			}
		}

		inline Array<ITEM> gradient_of_loss (const Array<ITEM> &dx ,Array<ITEM> &&dg ,Array<ITEM> &sx) const {
			Array<ITEM> ret = std::move (dg) ;
			for (INDEX i = 0 ; i < dx.length () ; i++)
				sx[i] = dx[i] ;
			for (INDEX i = 0 ; i < ret.length () ; i++) {
				const auto r1x = sx[i] ;
				sx[i] += mConvergenceEPS ;
				ret[i] = (mLoss (sx) - mLoss (dx)) / mConvergenceEPS ;
				sx[i] = r1x ;
			}
			return std::move (ret) ;
		}

		inline void update_is_and_ig () {
			/*
			Strong Wolfe-Powell Conditions
			find lamda match E1 and E2
			(0 < _lambda)
			E1 := mLose (mDX + _lambda * mIS) - mLose (mDX) <= _lambda * c1 * (mDG * mIS)
			(0 < c1 < 0.5)
			E2 := _ABS_ (gradient_of_loss (mDX + _lambda * mIS) * mIS) <= -c2 * (mDG * mIS)
			(c1 < c2 < 1)
			*/
			for (INDEX i = 0 ; i < mDG.length () ; i++)
				mIS[i] = -math_matrix_mul (mDM ,i ,mDG) ;
			const auto r1x = math_vector_dot (mDG ,mIS) ;
			mDXLoss[0] = mLoss (mDX) ;
			mDXLoss[2] = mDXLoss[0] ;
			mDXLambda[0] = 0 ;
			mDXLambda[2] = mDXLambda[0] ;
			mDXLambda[1] = mDXLambdaXMax ;
			while (mDXLambda[1] >= mConvergenceEPS && mDXLoss[2] > 0) {
				for (INDEX i = 0 ; i < mIX.length () ; i++)
					mIX[i] = mDX[i] + mIS[i] * mDXLambda[1] ;
				mDXLoss[1] = mLoss (mIX) ;
				if (mDXLoss[1] - mDXLoss[0] <= mDXLambda[1] * mDXC1C2[0] * r1x) {
					mIG = gradient_of_loss (mIX ,std::move (mIG) ,mSX) ;
					if (_ABS_ (math_vector_dot (mIG ,mIS)) <= -mDXC1C2[1] * r1x)
						mDXLoss[2] = ITEM (0) ;
				}
				if (mDXLoss[1] < mDXLoss[2]) {
					mDXLoss[2] = mDXLoss[1] ;
					mDXLambda[2] = mDXLambda[1] ;
				}
				mDXLambda[1] *= mDXLambdaPower ;
			}
			if (mDXLoss[2] < mDXLoss[0]) {
				mDXLoss[0] = mDXLoss[2] > 0 ? mDXLoss[2] : mDXLoss[1] ;
				_SWAP_ (mDX ,mIX) ;
				mIG = gradient_of_loss (mDX ,std::move (mIG) ,mSX) ;
			} else {
				mIG.fill (ITEM (0)) ;
			}
		}

		inline ITEM math_matrix_mul (const SoftImage<ITEM> &mat ,INDEX y ,const Array<ITEM> &v) const {
			ITEM ret = ITEM (0) ;
			for (INDEX i = 0 ; i < v.length () ; i++)
				ret += mat[y][i] * v[i] ;
			return std::move (ret) ;
		}

		inline ITEM math_vector_dot (const Array<ITEM> &v1 ,const Array<ITEM> &v2) const {
			ITEM ret = ITEM (0) ;
			for (INDEX i = 0 ; i < mDG.length () ; i++)
				ret += v1[i] * v2[i] ;
			return std::move (ret) ;
		}

		inline ITEM current_convergence () const {
			ITEM ret = ITEM (0) ;
			for (auto &i : mIG)
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
			const auto r1x = 1 / math_vector_dot (mIY ,mIS) ;
			for (INDEX i = 0 ; i < mDM.cy () ; i++)
				for (INDEX j = 0 ; j < mDM.cx () ; j++)
					mIM[i][j] = hessian_matrix_each (i ,j ,r1x) + mIS[i] * mIS[j] * r1x ;
			_SWAP_ (mDM ,mIM) ;
		}

		inline ITEM hessian_matrix_each (INDEX y ,INDEX x ,const ITEM &ys) const {
			ITEM ret = ITEM (0) ;
			for (INDEX i = 0 ; i < mDM.cy () ; i++) {
				const auto r1x = hessian_matrix_each_part (x ,i ,ys) ;
				ret += r1x * (-mIS[y] * mIY[i] * ys) ;
				ret += i != y ? r1x : ITEM (0) ;
			}
			return std::move (ret) ;
		}

		inline ITEM hessian_matrix_each_part (INDEX x ,INDEX z ,const ITEM &ys) const {
			ITEM ret = ITEM (0) ;
			for (INDEX i = 0 ; i < mDM.cx () ; i++) {
				ret += mDM[z][i] * (-mIY[i] * mIS[x] * ys) ;
				ret += i != x ? mDM[z][i] : ITEM (0) ;
			}
			return std::move (ret) ;
		}

		inline void refresh () {
			mContext.mDX = std::move (mDX) ;
			mContext.mDXLoss = mDXLoss[0] ;
		}
	} ;
	Lambda (*this ,loss ,dx) () ;
}

#ifdef __CSC_DEPRECATED__
template <class ITEM>
class KDimensionTreeAlgorithm {
private:
	struct NODE {
		INDEX mItem ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;
	} ;

private:
	Allocator<NODE ,SAUTO> mTree ;
	INDEX mRoot ;

public:
	KDimensionTreeAlgorithm () = delete ;

	explicit KDimensionTreeAlgorithm (const SoftImage<ITEM> &adjacency) {
		initialize (adjacency) ;
	}

	Array<INDEX> query (const INDEX &item ,LENGTH count) const {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Array<INDEX>> () ;
	}

	Array<INDEX> query_range (const INDEX &item ,const ITEM &range) const {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Array<INDEX>> () ;
	}

private:
	void initialize (const SoftImage<ITEM> &adjacency) ;
} ;

template <class ITEM>
inline void KDimensionTreeAlgorithm<ITEM>::initialize (const SoftImage<ITEM> &adjacency) {
	class Lambda {
	private:
		KDimensionTreeAlgorithm & mContext ;
		const SoftImage<ITEM> &mAdjacency ;

	public:
		inline explicit Lambda (KDimensionTreeAlgorithm &context ,const SoftImage<ITEM> &adjacency) :mContext (context) ,mAdjacency (adjacency) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {

		}

		inline void generate () {

		}

		inline void refresh () {

		}
	} ;
	Lambda (*this ,adjacency) () ;
}

template <class ITEM>
class SegmentTreeAlgorithm {
public:
	SegmentTreeAlgorithm () = delete ;

	explicit SegmentTreeAlgorithm (const Array<ARRAY2<ITEM>> &segment) {
		initialize (segment) ;
	}

	BitSet<> query (const ITEM &point) const {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<BitSet<>> () ;
	}

private:
	void initialize (const Array<ARRAY2<ITEM>> &segment) ;
} ;

template <class ITEM>
inline void SegmentTreeAlgorithm<ITEM>::initialize (const Array<ARRAY2<ITEM>> &segment) {
	class Lambda {
	private:
		SegmentTreeAlgorithm & mContext ;
		const Array<ARRAY2<ITEM>> &mSegment ;

	public:
		inline explicit Lambda (SegmentTreeAlgorithm &context ,const Array<ARRAY2<ITEM>> &segment) :mContext (context) ,mSegment (segment) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {

		}

		inline void generate () {

		}

		inline void refresh () {

		}
	} ;
	Lambda (*this ,segment) () ;
}
#endif
} ;