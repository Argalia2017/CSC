﻿#pragma once

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
struct KDTreeDatasetLayout {
	RefBuffer<FLT32> mPointCloud ;
	csc_size_t mDimension ;
	csc_size_t mSize ;
} ;

class KDTreeDataset implement KDTreeDatasetLayout {
public:
	csc_size_t kdtree_get_point_count () const {
		return mSize ;
	}

	FLT32 kdtree_distance (const FLT32 *pt ,csc_size_t idx ,csc_size_t size_) const {
		auto rax = FLT64 (0) ;
		for (auto &&i : range (0 ,LENGTH (size_))) {
			const auto r1x = pt[i] - mPointCloud[idx * mDimension + i] ;
			rax += MathProc::square (r1x) ;
		}
		return FLT32 (rax) ;
	}

	FLT32 kdtree_get_pt (csc_size_t idx ,csc_size_t dim) const {
		if (dim >= mDimension)
			return 0 ;
		INDEX ix = INDEX (idx * mDimension + dim) ;
		return mPointCloud[ix] ;
	}

	template <class BBOX>
	BOOL kdtree_get_bbox (BBOX &) const {
		return false ;
	}
} ;

struct KDTreeResultLayout {
	FLT32 mRadius ;
	Priority<IndexPair<FLT32>> mResult ;
} ;

class KDTreeResult implement KDTreeResultLayout {
public:
	using DistanceType = FLT32 ;
	using IndexType = INDEX ;

public:
	implicit KDTreeResult () = delete ;

	explicit KDTreeResult (CR<LENGTH> capacity ,CR<FLT32> radius_) {
		mRadius = radius_ ;
		mResult = Priority<IndexPair<FLT32>> (capacity + 1) ;
		clear () ;
	}

	void clear () {
		mResult.clear () ;
	}

	csc_size_t size () const {
		return mResult.length () ;
	}

	csc_size_t empty () const {
		return mResult.empty () ;
	}

	BOOL full () const {
		return true ;
	}

	BOOL addPoint (FLT32 dist ,INDEX index) {
		if (dist >= mRadius)
			return true ;
		mResult.add ({-dist ,index}) ;
		if ifdo (TRUE) {
			if (!mResult.full ())
				discard ;
			mResult.take () ;
			mRadius = MathProc::min_of (mRadius ,-mResult[0].m1st) ;
		}
		return true ;
	}

	FLT32 worstDist () const {
		return mRadius ;
	}

	nanoflann::ResultItem<INDEX ,FLT32> worst_item () const {
		assume (mResult.length () > 0) ;
		nanoflann::ResultItem<INDEX ,FLT32> ret ;
		ret.first = mResult[0].m2nd ;
		ret.second = -mResult[0].m1st ;
		return move (ret) ;
	}
} ;

using KDTreeDistance = nanoflann::L2_Simple_Adaptor<FLT32 ,KDTreeDataset ,FLT32 ,INDEX> ;
using KDTreeKNNAdaptor = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistance ,KDTreeDataset ,3 ,INDEX> ;

class KDTreeKNNSearch implement KDTreeKNNAdaptor {
public:
	implicit KDTreeKNNSearch () = delete ;

	using KDTreeKNNAdaptor::KDTreeKNNAdaptor ;
} ;

class PointCloudKDTreeImplHolder final implement Fat<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> {
public:
	void initialize (CR<Array<Pointer>> pointcloud) override {
		const auto r1x = address (pointcloud[0]) ;
		const auto r2x = pointcloud.step () / SIZE_OF<FLT32>::expr ;
		assume (inline_mid (r2x ,1 ,4)) ;
		const auto r3x = pointcloud.size () * r2x ;
		self.mDataset = Ref<KDTreeDataset>::make () ;
		self.mDataset->mPointCloud = RefBuffer<FLT32>::reference (r1x ,r3x) ;
		self.mDataset->mDimension = r2x ;
		self.mDataset->mSize = pointcloud.length () ;
		const auto r4x = nanoflann::KDTreeSingleIndexAdaptorParams () ;
		self.mKNNSearch = Ref<KDTreeKNNSearch>::make (VAL32 (r2x) ,self.mDataset.ref ,r4x) ;
		self.mKNNSearch->buildIndex () ;
	}

	Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor) const override {
		assert (neighbor > 0) ;
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResult (neighbor ,infinity) ;
		self.mKNNSearch->findNeighbors (rax ,(&r1x.mX) ,r2x) ;
		const auto r3x = inline_min (rax.mResult.length () ,neighbor) ;
		Array<INDEX> ret = Array<INDEX> (r3x) ;
		for (auto &&i : range (0 ,r3x)) {
			INDEX ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].m2nd ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}

	Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor ,CR<FLT64> radius) const override {
		assert (neighbor > 0) ;
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResult (neighbor ,FLT32 (MathProc::square (radius))) ;
		self.mKNNSearch->radiusSearchCustomCallback ((&r1x.mX) ,rax ,r2x) ;
		const auto r3x = inline_min (rax.mResult.length () ,neighbor) ;
		Array<INDEX> ret = Array<INDEX> (r3x) ;
		for (auto &&i : range (0 ,r3x)) {
			INDEX ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].m2nd ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}
} ;

static const auto mPointCloudKDTreecExternal = External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> (PointCloudKDTreeImplHolder ()) ;
} ;