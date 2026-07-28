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
template <class A>
struct KDTreeDataset {
	RefBuffer<A> mPointCloud ;
	csc_size_t mSize ;
	csc_size_t mChannel ;

public:
	csc_size_t kdtree_get_point_count () const {
		return mSize ;
	}

	A kdtree_distance (PTR<CR<A>> pt ,csc_size_t idx ,csc_size_t size_) const {
		auto rax = Flt64 (0) ;
		for (auto &&i : range (0 ,Length (size_))) {
			const auto r1x = pt[i] - mPointCloud[idx * mChannel + i] ;
			rax += MathProc::square (r1x) ;
		}
		return A (rax) ;
	}

	A kdtree_get_pt (csc_size_t idx ,csc_size_t dim) const {
		if (dim >= mChannel)
			return 0 ;
		Index ix = Index (idx * mChannel + dim) ;
		return mPointCloud[ix] ;
	}

	template <class BBOX>
	Bool kdtree_get_bbox (DEF<BBOX &> bb) const {
		return false ;
	}
} ;

template <class A>
struct KDTreeResult {
	A mL2Dist ;
	Priority<IndexPair<A>> mResult ;

public:
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

	Bool addPoint (A dist ,Index index) {
		if (dist >= mL2Dist)
			return true ;
		mResult.add ({-dist ,index}) ;
		if ifdo (TRUE) {
			if (!mResult.full ())
				discard ;
			mResult.take () ;
			mL2Dist = MathProc::min_of (mL2Dist ,-mResult[0].m1st) ;
		}
		return true ;
	}

	A worstDist () const {
		return mL2Dist ;
	}

	nanoflann::ResultItem<Index ,A> worst_item () const {
		assume (mResult.length () > 0) ;
		nanoflann::ResultItem<Index ,A> ret ;
		ret.first = mResult[0].m2nd ;
		ret.second = -mResult[0].m1st ;
		return move (ret) ;
	}

	using DistanceType = A ;
	using IndexType = Val32 ;

	void sort () {
		noop () ;
	}
} ;

using KDTreeDistanceF32 = nanoflann::L2_Simple_Adaptor<Flt32 ,KDTreeDataset<Flt32> ,Flt32 ,Index> ;
using KDTreeDistanceF64 = nanoflann::L2_Simple_Adaptor<Flt64 ,KDTreeDataset<Flt64> ,Flt64 ,Index> ;
using KDTreeKNNAdaptorF32C3 = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistanceF32 ,KDTreeDataset<Flt32> ,3 ,Index> ;
using KDTreeKNNAdaptorF64C3 = nanoflann::KDTreeSingleIndexAdaptor<KDTreeDistanceF64 ,KDTreeDataset<Flt64> ,3 ,Index> ;

struct KDTreeF32 {
	Box<KDTreeDataset<Flt32>> mDataset ;
	Box<KDTreeKNNAdaptorF32C3> mKNNSearch ;
	Box<KDTreeResult<Flt32>> mResult ;
} ;

struct KDTreeF64 {
	Box<KDTreeDataset<Flt64>> mDataset ;
	Box<KDTreeKNNAdaptorF64C3> mKNNSearch ;
	Box<KDTreeResult<Flt64>> mResult ;
} ;

class PointCloudKDTreeImplHolder final implement Fat<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> {
public:
	void initialize (RR<RefBuffer<Flt32>> pointcloud ,CR<Length> channel) override {
		assert (channel > 0) ;
		const auto r1x = pointcloud.size () ;
		const auto r2x = pointcloud.step () ;
		const auto r3x = r1x % channel ;
		assert (r1x > 0) ;
		assert (r3x == 0) ;
		assert (r2x == SIZE_OF<Flt32>::expr) ;
		self.mSize = r1x / channel ;
		self.mStep = r2x ;
		self.mChannel = channel ;
		self.mF32 = Ref<KDTreeF32>::make () ;
		self.mF32->mDataset = Box<KDTreeDataset<Flt32>>::make () ;
		self.mF32->mDataset->mPointCloud = move (pointcloud) ;
		self.mF32->mDataset->mSize = self.mSize ;
		self.mF32->mDataset->mChannel = self.mChannel ;
		const auto r5x = nanoflann::KDTreeSingleIndexAdaptorParams () ;
		self.mF32->mKNNSearch = Box<KDTreeKNNAdaptorF32C3>::make (Val32 (3) ,self.mF32->mDataset.ref ,r5x) ;
		self.mF32->mKNNSearch->buildIndex () ;
		self.mF32->mResult = Box<KDTreeResult<Flt32>>::make () ;
	}

	void initialize (RR<RefBuffer<Flt64>> pointcloud ,CR<Length> channel) override {
		assert (channel > 0) ;
		const auto r1x = pointcloud.size () ;
		const auto r2x = pointcloud.step () ;
		const auto r3x = r1x % channel ;
		assert (r1x > 0) ;
		assert (r3x == 0) ;
		assert (r2x == SIZE_OF<Flt64>::expr) ;
		self.mSize = r1x / channel ;
		self.mStep = r2x ;
		self.mChannel = channel ;
		self.mF64 = Ref<KDTreeF64>::make () ;
		self.mF64->mDataset = Box<KDTreeDataset<Flt64>>::make () ;
		self.mF64->mDataset->mPointCloud = move (pointcloud) ;
		self.mF64->mDataset->mSize = self.mSize ;
		self.mF64->mDataset->mChannel = self.mChannel ;
		const auto r5x = nanoflann::KDTreeSingleIndexAdaptorParams () ;
		self.mF64->mKNNSearch = Box<KDTreeKNNAdaptorF64C3>::make (Val32 (3) ,self.mF64->mDataset.ref ,r5x) ;
		self.mF64->mKNNSearch->buildIndex () ;
		self.mF64->mResult = Box<KDTreeResult<Flt64>>::make () ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) override {
		assert (neighbor > 0) ;
		Array<Index> ret ;
		const auto r1x = nanoflann::SearchParameters (0 ,false) ;
		if ifdo (TRUE) {
			if (self.mStep != SIZE_OF<Flt32>::expr)
				discard ;
			self.mF32->mResult->mL2Dist = Flt32 (MathProc::square (radius)) ;
			self.mF32->mResult->mResult = Priority<IndexPair<Flt32>> (neighbor + 1) ;
			auto rbx = Buffer3<Flt32> () ;
			rbx[0] = Flt32 (center[0]) ;
			rbx[1] = Flt32 (center[1]) ;
			rbx[2] = Flt32 (center[2]) ;
			self.mF32->mKNNSearch->findNeighbors (self.mF32->mResult.ref ,(&rbx[0]) ,r1x) ;
			const auto r2x = inline_min (self.mF32->mResult->mResult.length () ,neighbor) ;
			ret = Array<Index> (r2x) ;
			for (auto &&i : range (0 ,r2x)) {
				Index ix = r2x - 1 - i ;
				ret[ix] = self.mF32->mResult->mResult[0].m2nd ;
				self.mF32->mResult->mResult.take () ;
			}
		}
		if ifdo (TRUE) {
			if (self.mStep != SIZE_OF<Flt64>::expr)
				discard ;
			self.mF64->mResult->mL2Dist = Flt64 (MathProc::square (radius)) ;
			self.mF64->mResult->mResult = Priority<IndexPair<Flt64>> (neighbor + 1) ;
			auto rbx = Buffer3<Flt64> () ;
			rbx[0] = Flt64 (center[0]) ;
			rbx[1] = Flt64 (center[1]) ;
			rbx[2] = Flt64 (center[2]) ;
			self.mF64->mKNNSearch->findNeighbors (self.mF64->mResult.ref ,(&rbx[0]) ,r1x) ;
			const auto r3x = inline_min (self.mF64->mResult->mResult.length () ,neighbor) ;
			ret = Array<Index> (r3x) ;
			for (auto &&i : range (0 ,r3x)) {
				Index ix = r3x - 1 - i ;
				ret[ix] = self.mF64->mResult->mResult[0].m2nd ;
				self.mF64->mResult->mResult.take () ;
			}
		}
		return move (ret) ;
	}
} ;

static const auto mPointCloudKDTreecExternal = External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> (PointCloudKDTreeImplHolder ()) ;
} ;