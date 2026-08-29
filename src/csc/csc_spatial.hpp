#pragma once

#ifndef __CSC_SPATIAL__
#define __CSC_SPATIAL__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_image.hpp"
#include "csc_matrix.hpp"
#include "csc_numeric.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_file.hpp"
#include "csc_thread.hpp"
#include "csc_property.hpp"
#include "csc_algorithm.hpp"

namespace CSC {
struct OctTreeNode {
	Index mFirst ;
	Length mWidth ;
	Bound<Vector> mBound ;
	Buffer<Index ,RANK8> mChild ;
} ;

struct OctTreeLeaf {
	Vector mPoint ;
	Length mWidth ;
	Index mNext ;
} ;

struct OctTreeLayout {
	Allocator<OctTreeNode ,AllocatorNode> mTree ;
	Bound<Vector> mTotalBound ;
	List<OctTreeLeaf> mLeaf ;
	Index mNull ;
	Index mRoot ;
	Index mTop ;
	Array<Buffer3<Index>> mDir ;
	Flt64 mEpsilon ;
} ;

struct OctTreeHolder implement Interface {
	imports VFat<OctTreeHolder> hold (VR<OctTreeLayout> that) ;
	imports CFat<OctTreeHolder> hold (CR<OctTreeLayout> that) ;

	virtual void initialize (CR<Length> size_ ,CR<Bound<Vector>> bound) = 0 ;
	virtual void set_epsilon (CR<Flt64> epsilon) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length length () const = 0 ;
	virtual void clear () = 0 ;
	virtual void add (CR<Vector> item) = 0 ;
	virtual void add (CR<Array<Vector>> item) = 0 ;
	virtual Index find (CR<Vector> item) const = 0 ;
	virtual Bool contain (CR<Vector> item) const = 0 ;
	virtual Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const = 0 ;
} ;

class OctTree implement OctTreeLayout {
public:
	implicit OctTree () = default ;

	explicit OctTree (CR<Length> size_ ,CR<Bound<Vector>> bound) {
		OctTreeHolder::hold (thiz)->initialize (size_ ,bound) ;
	}

	void set_epsilon (CR<Flt64> epsilon) {
		return OctTreeHolder::hold (thiz)->set_epsilon (epsilon) ;
	}

	Length size () const {
		return OctTreeHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return OctTreeHolder::hold (thiz)->length () ;
	}

	void clear () {
		return OctTreeHolder::hold (thiz)->clear () ;
	}

	void add (CR<Vector> item) {
		return OctTreeHolder::hold (thiz)->add (item) ;
	}

	void add (CR<Array<Vector>> item) {
		return OctTreeHolder::hold (thiz)->add (item) ;
	}

	Index find (CR<Vector> item) const {
		return OctTreeHolder::hold (thiz)->find (item) ;
	}

	Bool contain (CR<Vector> item) const {
		return OctTreeHolder::hold (thiz)->contain (item) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor) const {
		return search (center ,neighbor ,infinity) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const {
		return OctTreeHolder::hold (thiz)->search (center ,neighbor ,radius) ;
	}
} ;

struct PointCloudKDTreeLayout ;

struct PointCloudKDTreeHolder implement Interface {
	imports Ref<PointCloudKDTreeLayout> create () ;
	imports VFat<PointCloudKDTreeHolder> hold (VR<PointCloudKDTreeLayout> that) ;
	imports CFat<PointCloudKDTreeHolder> hold (CR<PointCloudKDTreeLayout> that) ;

	virtual void initialize (RR<RefBuffer<Flt32>> pointcloud ,CR<Length> channel) = 0 ;
	virtual void initialize (RR<RefBuffer<Flt64>> pointcloud ,CR<Length> channel) = 0 ;
	virtual Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) = 0 ;
} ;

class PointCloudKDTree implement Super<Ref<PointCloudKDTreeLayout>> {
public:
	implicit PointCloudKDTree () = default ;

	explicit PointCloudKDTree (RR<RefBuffer<Flt32>> pointcloud ,CR<Length> channel) {
		mThis = PointCloudKDTreeHolder::create () ;
		PointCloudKDTreeHolder::hold (thiz)->initialize (move (pointcloud) ,channel) ;
	}

	explicit PointCloudKDTree (RR<RefBuffer<Flt64>> pointcloud ,CR<Length> channel) {
		mThis = PointCloudKDTreeHolder::create () ;
		PointCloudKDTreeHolder::hold (thiz)->initialize (move (pointcloud) ,channel) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor) {
		return search (center ,neighbor ,infinity) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) {
		return PointCloudKDTreeHolder::hold (thiz)->search (center ,neighbor ,radius) ;
	}
} ;

struct PointCloudTree ;

struct PointCloudLayout {
	SharedRef<PointCloudTree> mThis ;
	Ref<DuplexMatrix> mWorld ;
	FarBuffer<Vector> mPointView ;
} ;

struct PointCloudHolder implement Interface {
	imports VFat<PointCloudHolder> hold (VR<PointCloudLayout> that) ;
	imports CFat<PointCloudHolder> hold (CR<PointCloudLayout> that) ;

	virtual void initialize (RR<Ref<Array<Point2F>>> pointcloud) = 0 ;
	virtual void initialize (RR<Ref<Array<Point3F>>> pointcloud) = 0 ;
	virtual void initialize (RR<Ref<Array<Vector>>> pointcloud) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length align () const = 0 ;
	virtual Length channel () const = 0 ;
	virtual void get (CR<Index> index ,VR<Vector> item) const = 0 ;
	virtual Matrix pca_matrix () const = 0 ;
	virtual Matrix box_matrix (CR<Flt64> bx ,CR<Flt64> by ,CR<Flt64> bz) const = 0 ;
	virtual Matrix cut_matrix (CR<Flt64> sx ,CR<Flt64> sy ,CR<Flt64> sz) const = 0 ;
	virtual Bound<Vector> bound () const = 0 ;
	virtual PointCloudLayout smul (CR<Matrix> that) const = 0 ;
	virtual Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const = 0 ;
} ;

class PointCloud implement PointCloudLayout {
protected:
	using PointCloudLayout::mThis ;
	using PointCloudLayout::mWorld ;
	using PointCloudLayout::mPointView ;

public:
	implicit PointCloud () = default ;

	explicit PointCloud (RR<Ref<Array<Point2F>>> pointcloud) {
		PointCloudHolder::hold (thiz)->initialize (move (pointcloud)) ;
	}

	explicit PointCloud (RR<Ref<Array<Point3F>>> pointcloud) {
		PointCloudHolder::hold (thiz)->initialize (move (pointcloud)) ;
	}

	explicit PointCloud (RR<Ref<Array<Vector>>> pointcloud) {
		PointCloudHolder::hold (thiz)->initialize (move (pointcloud)) ;
	}

	Length size () const {
		return PointCloudHolder::hold (thiz)->size () ;
	}

	Length align () const {
		return PointCloudHolder::hold (thiz)->align () ;
	}

	Length channel () const {
		return PointCloudHolder::hold (thiz)->channel () ;
	}

	void get (CR<Index> index ,VR<Vector> item) const {
		return PointCloudHolder::hold (thiz)->get (index ,item) ;
	}

	forceinline Vector operator[] (CR<Index> index) const {
		Vector ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	Matrix pca_matrix () const {
		return PointCloudHolder::hold (thiz)->pca_matrix () ;
	}

	Matrix box_matrix () const {
		return PointCloudHolder::hold (thiz)->box_matrix (0 ,0 ,0) ;
	}

	Matrix box_matrix (CR<Flt64> bx ,CR<Flt64> by ,CR<Flt64> bz) const {
		return PointCloudHolder::hold (thiz)->box_matrix (bx ,by ,bz) ;
	}

	Matrix cut_matrix (CR<Flt64> sx ,CR<Flt64> sy ,CR<Flt64> sz) const {
		return PointCloudHolder::hold (thiz)->cut_matrix (sx ,sy ,sz) ;
	}

	Bound<Vector> bound () const {
		return PointCloudHolder::hold (thiz)->bound () ;
	}

	PointCloud smul (CR<Matrix> that) const {
		PointCloudLayout ret = PointCloudHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<PointCloud>::expr] (ret)) ;
	}

	forceinline PointCloud operator* (CR<Matrix> that) const {
		return smul (that) ;
	}

	forceinline friend PointCloud operator* (CR<Matrix> thiz_ ,CR<PointCloud> that) {
		return that.smul (thiz_.transpose ()) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor) const {
		return search (center ,neighbor ,infinity) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const {
		return PointCloudHolder::hold (thiz)->search (center ,neighbor ,radius) ;
	}
} ;

struct VoxelGridRTreeLayout {} ;

struct VoxelGridRTreeHolder implement Interface {
	imports VFat<VoxelGridRTreeHolder> hold (VR<VoxelGridRTreeLayout> that) ;
	imports CFat<VoxelGridRTreeHolder> hold (CR<VoxelGridRTreeLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class VoxelGridRTree implement VoxelGridRTreeLayout {
public:
	implicit VoxelGridRTree () = default ;
} ;

struct VoxelGridLayout {} ;

struct VoxelGridHolder implement Interface {
	imports VFat<VoxelGridHolder> hold (VR<VoxelGridLayout> that) ;
	imports CFat<VoxelGridHolder> hold (CR<VoxelGridLayout> that) ;

	virtual void initialize () = 0 ;
} ;

class VoxelGrid implement VoxelGridLayout {
public:
	implicit VoxelGrid () = default ;
} ;
} ;