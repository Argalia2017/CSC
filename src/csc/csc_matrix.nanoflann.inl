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
struct KDTreeDataset {
public:
	RefBuffer<Flt32> mPointCloud ;
	csc_size_t mDimension ;
	csc_size_t mSize ;

public:
	csc_size_t kdtree_get_point_count () const {
		return mSize ;
	}

	Flt32 kdtree_distance (PTR<CR<Flt32>> pt ,csc_size_t idx ,csc_size_t size_) const {
		auto rax = Flt64 (0) ;
		for (auto &&i : range (0 ,Length (size_))) {
			const auto r1x = pt[i] - mPointCloud[idx * mDimension + i] ;
			rax += MathProc::square (r1x) ;
		}
		return Flt32 (rax) ;
	}

	Flt32 kdtree_get_pt (csc_size_t idx ,csc_size_t dim) const {
		if (dim >= mDimension)
			return 0 ;
		Index ix = Index (idx * mDimension + dim) ;
		return mPointCloud[ix] ;
	}

	template <class BBOX>
	Bool kdtree_get_bbox (BBOX &) const {
		return false ;
	}
} ;

struct KDTreeResult {
	Flt32 mRadius ;
	Priority<IndexPair<Flt32>> mResult ;

public:
	implicit KDTreeResult () = delete ;

	explicit KDTreeResult (CR<Length> capacity ,CR<Flt32> radius_) {
		mRadius = radius_ ;
		mResult = Priority<IndexPair<Flt32>> (capacity + 1) ;
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

	Bool full () const {
		return true ;
	}

	Bool addPoint (Flt32 dist ,Index index) {
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

	Flt32 worstDist () const {
		return mRadius ;
	}

	nanoflann::ResultItem<Index ,Flt32> worst_item () const {
		assume (mResult.length () > 0) ;
		nanoflann::ResultItem<Index ,Flt32> ret ;
		ret.first = mResult[0].m2nd ;
		ret.second = -mResult[0].m1st ;
		return move (ret) ;
	}
} ;

using KDTreeDistance = nanoflann::L2_Simple_Adaptor<Flt32 ,KDTreeDataset ,Flt32 ,Index> ;
using KDTreeKNNAdaptor = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistance ,KDTreeDataset ,3 ,Index> ;

struct KDTreeKNNSearch implement KDTreeKNNAdaptor {
public:
	implicit KDTreeKNNSearch () = delete ;

	using KDTreeKNNAdaptor::KDTreeKNNAdaptor ;
} ;

class PointCloudKDTreeImplHolder final implement Fat<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> {
public:
	void initialize (CR<Array<Pointer>> pointcloud) override {
		const auto r1x = address (pointcloud[0]) ;
		const auto r2x = pointcloud.step () / SIZE_OF<Flt32>::expr ;
		assume (inline_between (r2x ,1 ,4)) ;
		const auto r3x = pointcloud.size () * r2x ;
		self.mDataset = Ref<KDTreeDataset>::make () ;
		self.mDataset->mPointCloud = RefBuffer<Flt32>::reference (r1x ,r3x) ;
		self.mDataset->mDimension = r2x ;
		self.mDataset->mSize = pointcloud.length () ;
		const auto r4x = nanoflann::KDTreeSingleIndexAdaptorParams () ;
		self.mKNNSearch = Ref<KDTreeKNNSearch>::make (Val32 (r2x) ,self.mDataset.ref ,r4x) ;
		self.mKNNSearch->buildIndex () ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor) const override {
		assert (neighbor > 0) ;
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		auto rax = KDTreeResult (neighbor ,infinity) ;
		self.mKNNSearch->findNeighbors (rax ,(&r1x.mX) ,r2x) ;
		const auto r3x = inline_min (rax.mResult.length () ,neighbor) ;
		Array<Index> ret = Array<Index> (r3x) ;
		for (auto &&i : range (0 ,r3x)) {
			Index ix = r3x - 1 - i ;
			ret[ix] = rax.mResult[0].m2nd ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const override {
		assert (neighbor > 0) ;
		const auto r1x = Point3F (center.xyz ()) ;
		const auto r2x = nanoflann::SearchParameters (0 ,false) ;
		const auto r3x = Flt32 (MathProc::square (radius)) ;
		auto rax = KDTreeResult (neighbor ,r3x) ;
		self.mKNNSearch->radiusSearchCustomCallback ((&r1x.mX) ,rax ,r2x) ;
		const auto r4x = inline_min (rax.mResult.length () ,neighbor) ;
		Array<Index> ret = Array<Index> (r4x) ;
		for (auto &&i : range (0 ,r4x)) {
			Index ix = r4x - 1 - i ;
			ret[ix] = rax.mResult[0].m2nd ;
			rax.mResult.take () ;
		}
		return move (ret) ;
	}
} ;

static const auto mPointCloudKDTreecExternal = External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> (PointCloudKDTreeImplHolder ()) ;
} ;