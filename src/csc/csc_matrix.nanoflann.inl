#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_matrix.hpp"

#include "csc_end.h"
#include <nanoflann.hpp>
#include "csc_begin.h"

namespace CSC {
inline namespace {
using KDTreeDistance = nanoflann::L2_Simple_Adaptor<FLT32 ,KDTreePointCloudAdaptor> ;
using KDTree = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistance ,KDTreePointCloudAdaptor ,-1 ,VAL32> ;
} ;

struct KDTreePointCloudAdaptor {
	RefBuffer<FLT32> mPointCloud ;
	LENGTH mDimension ;
	LENGTH mSize ;

public:
	inline size_t kdtree_get_point_count () const {
		return mSize ;
	}

	inline FLT32 kdtree_distance (const FLT32 *p1 ,const size_t idx_p2 ,size_t size) const {
		FLT32 result = 0.0;
		for (size_t i = 0; i < size; ++i) {
			const FLT32 d = p1[i] - mPointCloud[idx_p2 * mDimension + i];
			result += d * d;
		}
		return result;
	}

	inline FLT32 kdtree_get_pt (const size_t idx ,int dim) const {
		return mPointCloud[idx * mDimension + dim];
	}

	template <class BBOX>
	bool kdtree_get_bbox (BBOX &) const {
		return false ;
	}
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
		fake->mKDTree = Box<KDTree>::make (r2x ,fake->mAdaptor ,r4x) ;
		fake->mKDTree->buildIndex () ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const override {
		const auto r1x = keep[TYPE<Point3F>::expr] (center.xyz ()) ;
		auto rax = std::vector<VAL32> (neighbor) ;
		const auto r2x = fake->mKDTree->knnSearch ((&r1x.mX) ,csc_size_t (neighbor) ,(&rax[0]) ,NULL) ;
		Array<INDEX> ret = Array<INDEX> (r2x) ;
		for (auto &&i : iter (0 ,r2x))
			ret[i] = rax[i] ;
		return move (ret) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const override {
		const auto r1x = keep[TYPE<Point3F>::expr] (center.xyz ()) ;
		auto rax = std::vector<nanoflann::ResultItem<VAL32 ,FLT32>> () ;
		const auto r2x = fake->mKDTree->radiusSearch ((&r1x.mX) ,csc_size_t (neighbor) ,rax) ;
		Array<INDEX> ret = Array<INDEX> (r2x) ;
		for (auto &&i : iter (0 ,r2x))
			ret[i] = rax[i].first ;
		return move (ret) ;
	}
} ;

static const auto mPointCloudKDTreecExternal = External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> (PointCloudKDTreeImplHolder ()) ;
} ;