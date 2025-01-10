#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_matrix.hpp"

#include "csc_end.h"
#ifdef __CSC_COMPILER_MSVC__
#endif

#include <nanoflann.hpp>
#include "csc_begin.h"

namespace CSC {
struct KDTreePointCloudAdaptor {
	RefBuffer<FLT32> mPointCloud ;
	LENGTH mDimension ;
	LENGTH mSize ;

public:
	inline size_t kdtree_get_point_count () const {
		return mSize ;
	}

	inline FLT32 kdtree_distance (const FLT32 *pt ,size_t idx_p2 ,size_t size_) const {
		FLT32 ret = 0 ;
		for (auto &&i : iter (0 ,LENGTH (size_))) {
			const auto r1x = pt[i] - mPointCloud[idx_p2 * mDimension + i] ;
			ret += MathProc::square (r1x) ;
		}
		return ret ;
	}

	inline FLT32 kdtree_get_pt (size_t idx ,size_t dim) const {
		INDEX ix = INDEX (idx) * mDimension + INDEX (dim) ;
		return mPointCloud[ix] ;
	}

	template <class BBOX>
	bool kdtree_get_bbox (BBOX &) const {
		return false ;
	}
} ;

class KDTreeResultAdaptor {
public:
	using DistanceType = FLT32 ;
	using IndexType = INDEX ;

public:
	DistanceType mRadius ;
	Priority<IndexPair<FLT32>> mResult ;

	explicit KDTreeResultAdaptor (CREF<LENGTH> capacity ,CREF<FLT32> radius_) {
		mRadius = radius_ ;
		mResult = Priority<IndexPair<FLT32>> (capacity) ;
		clear () ;
	}

	void clear () {
		mResult.clear () ;
	}

	size_t size () const {
		return mResult.length () ;
	}

	size_t empty () const {
		return mResult.empty () ;
	}

	bool full () const {
		return true ;
	}

	bool addPoint (DistanceType dist ,IndexType index) {
		if (dist >= mRadius)
			return true ;
		if ifdo (TRUE) {
			if (mResult.full ())
				discard ;
			mResult.add ({-dist ,index}) ;
			return true ;
		}
		mResult.take () ;
		mResult.add ({-dist ,index}) ;
		mRadius = MathProc::min_of (mRadius ,-mResult[0].mItem) ;
		return true ;
	}

	DistanceType worstDist () const {
		return mRadius ;
	}

	nanoflann::ResultItem<IndexType ,DistanceType> worst_item () const {
		assume (mResult.length () > 0) ;
		nanoflann::ResultItem<IndexType ,DistanceType> ret ;
		ret.first = mResult[0].mIndex ;
		ret.second = -mResult[0].mItem ;
		return move (ret) ;
	}
} ;

inline namespace {
using KDTreeDistance = nanoflann::L2_Simple_Adaptor<FLT32 ,KDTreePointCloudAdaptor ,FLT32 ,INDEX> ;
using KDTree = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistance ,KDTreePointCloudAdaptor ,-1 ,INDEX> ;
} ;

struct PointCloudKDTreeImplLayout {
	KDTreePointCloudAdaptor mAdaptor ;
	Box<KDTree> mKDTree ;
} ;

class PointCloudKDTreeImplHolder implement Fat<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> {
public:
	void initialize (CREF<Array<Pointer>> pointcloud) override {
		fake.mThis = AutoRef<PointCloudKDTreeImplLayout>::make () ;
		const auto r1x = address (pointcloud[0]) ;
		const auto r2x = pointcloud.step () / SIZE_OF<FLT32>::expr ;
		assume (inline_between (r2x ,1 ,4)) ;
		const auto r3x = pointcloud.size () * r2x ;
		fake->mAdaptor.mPointCloud = RefBuffer<FLT32>::reference (r1x ,r3x) ;
		fake->mAdaptor.mDimension = r2x ;
		fake->mAdaptor.mSize = pointcloud.length () ;
		const auto r4x = nanoflann::KDTreeSingleIndexAdaptorParams () ;
		fake->mKDTree = Box<KDTree>::make (VAL32 (r2x) ,fake->mAdaptor ,r4x) ;
		fake->mKDTree->buildIndex () ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const override {
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResultAdaptor (neighbor ,infinity) ;
		fake->mKDTree->findNeighbors (rax ,(&r1x.mX) ,r2x) ;
		const auto r3x = rax.mResult.length () ;
		assert (r3x <= neighbor) ;
		Array<INDEX> ret = Array<INDEX> (r3x) ;
		for (auto &&i : iter (0 ,r3x)) {
			INDEX ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].mIndex ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const override {
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResultAdaptor (neighbor ,FLT32 (MathProc::square (radius))) ;
		fake->mKDTree->radiusSearchCustomCallback ((&r1x.mX) ,rax ,r2x) ;
		const auto r3x = rax.mResult.length () ;
		assert (r3x <= neighbor) ;
		Array<INDEX> ret = Array<INDEX> (r3x) ;
		for (auto &&i : iter (0 ,r3x)) {
			INDEX ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].mIndex ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}
} ;

static const auto mPointCloudKDTreecExternal = External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> (PointCloudKDTreeImplHolder ()) ;
} ;