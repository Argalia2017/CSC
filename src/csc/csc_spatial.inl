#pragma once

#ifndef __CSC_SPATIAL__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_spatial.hpp"

namespace CSC {
struct OctTreeChild {
	Index mUp ;
	Index mSide ;
} ;

struct OctTreeResult {
	Priority<IndexPair<Flt64>> mPriority ;
	Vector mCenter ;
	Bound<Vector> mBound ;
	Length mNeighbor ;
	Flt64 mSquareRadius ;
} ;

class OctTreeImplHolder final implement Fat<OctTreeHolder ,OctTreeLayout> {
private:
	using OCTREE_BUCKET_SIZE = ENUM<8> ;

public:
	void initialize (CR<Length> size_ ,CR<Bound<Vector>> bound) override {
		assert (size_ > 0) ;
		self.mTree = Allocator<OctTreeNode ,AllocatorNode> (size_) ;
		self.mTotalBound = bound ;
		self.mEpsilon = Flt64 (1E-6) ;
		self.mDir = Array<Buffer3<Index>> (8) ;
		self.mDir[0] = Buffer3<Index> (0 ,0 ,0) ;
		self.mDir[1] = Buffer3<Index> (1 ,0 ,0) ;
		self.mDir[2] = Buffer3<Index> (0 ,1 ,0) ;
		self.mDir[3] = Buffer3<Index> (1 ,1 ,0) ;
		self.mDir[4] = Buffer3<Index> (0 ,0 ,1) ;
		self.mDir[5] = Buffer3<Index> (1 ,0 ,1) ;
		self.mDir[6] = Buffer3<Index> (0 ,1 ,1) ;
		self.mDir[7] = Buffer3<Index> (1 ,1 ,1) ;
		clear () ;
	}

	void set_epsilon (CR<Flt64> epsilon) override {
		self.mEpsilon = epsilon ;
	}

	Length size () const override {
		return self.mLeaf.size () ;
	}

	Length length () const override {
		return self.mLeaf.length () ;
	}

	void clear () override {
		self.mTree.clear () ;
		self.mLeaf.clear () ;
		Index ix = alloc_node () ;
		for (auto &&i : range (0 ,8)) {
			self.mTree[ix].mChild[i] = ix ;
		}
		self.mNull = ix ;
		self.mRoot = ix ;
		self.mTop = 0 ;
	}

	Index alloc_node () {
		Index ret = self.mTree.alloc () ;
		for (auto &&i : range (0 ,8)) {
			self.mTree[ret].mChild[i] = self.mNull ;
		}
		self.mTree[ret].mFirst = NONE ;
		self.mTree[ret].mWidth = 0 ;
		self.mTree[ret].mBound.mMin = Vector::zero () ;
		self.mTree[ret].mBound.mMax = Vector::zero () ;
		return move (ret) ;
	}

	void add (CR<Vector> item) override {
		if (!is_bound_contain (self.mTotalBound ,item))
			return ;
		const auto r1x = OctTreeChild ({self.mNull ,0}) ;
		update_emplace (r1x ,self.mTotalBound ,item) ;
		Index jx = self.mTop ;
		if (jx == NONE)
			return ;
		Index ix = self.mLeaf.insert () ;
		self.mLeaf[ix].mPoint = item ;
		self.mLeaf[ix].mWidth = 1 ;
		self.mLeaf[ix].mNext = self.mTree[jx].mFirst ;
		self.mTree[jx].mFirst = ix ;
		self.mTree[jx].mWidth++ ;
		update_insert (jx) ;
	}

	void add (CR<Array<Vector>> item) override {
		for (auto &&i : item.iter ()) {
			add (item[i]) ;
		}
	}

	void update_emplace (CR<OctTreeChild> curr ,CR<Bound<Vector>> bound ,CR<Vector> item) {
		auto rax = curr ;
		auto rbx = bound ;
		Index jx = NONE ;
		while (TRUE) {
			jx = curr_next (rax) ;
			if ifdo (TRUE) {
				if (jx != self.mNull)
					discard ;
				jx = alloc_node () ;
				self.mTree[jx].mBound = rbx ;
				curr_next (rax) = jx ;
			}
			if (is_leaf (jx))
				break ;
			self.mTree[jx].mWidth++ ;
			const auto r1x = child_side (self.mTree[jx].mBound ,item) ;
			rax = OctTreeChild ({jx ,r1x}) ;
			rbx = child_bound (self.mTree[jx].mBound ,r1x) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			Index ix = find_item (jx ,item) ;
			if (ix == NONE)
				discard ;
			self.mLeaf[ix].mWidth++ ;
			self.mTop = NONE ;
		}
		if ifdo (act) {
			self.mTop = jx ;
		}
	}

	void update_insert (CR<Index> curr) {
		if (self.mTree[curr].mWidth <= OCTREE_BUCKET_SIZE::expr)
			return ;
		const auto r1x = self.mTree[curr].mBound ;
		Index ix = self.mTree[curr].mFirst ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			const auto r2x = self.mLeaf[ix].mNext ;
			const auto r3x = child_side (r1x ,self.mLeaf[ix].mPoint) ;
			Index jx = self.mTree[curr].mChild[r3x] ;
			if ifdo (TRUE) {
				if (jx != self.mNull)
					discard ;
				jx = alloc_node () ;
				self.mTree[jx].mBound = child_bound (r1x ,r3x) ;
				self.mTree[curr].mChild[r3x] = jx ;
			}
			self.mLeaf[ix].mNext = self.mTree[jx].mFirst ;
			self.mTree[jx].mFirst = ix ;
			self.mTree[jx].mWidth++ ;
			ix = r2x ;
		}
		self.mTree[curr].mFirst = NONE ;
	}

	Index child_side (CR<Bound<Vector>> bound ,CR<Vector> point) const {
		Index ret = 0 ;
		const auto r1x = (bound.mMin + bound.mMax).projection () ;
		ret += 1 * Index (point[0] >= r1x[0]) ;
		ret += 2 * Index (point[1] >= r1x[1]) ;
		ret += 4 * Index (point[2] >= r1x[2]) ;
		return move (ret) ;
	}

	Bound<Vector> child_bound (CR<Bound<Vector>> bound ,CR<Index> side) const {
		Bound<Vector> ret ;
		const auto r1x = (bound.mMin + bound.mMax).projection () ;
		const auto r2x = Buffer3<Vector> (bound.mMin ,r1x ,bound.mMax) ;
		const auto r3x = self.mDir[side] ;
		ret.mMin[0] = r2x[r3x[0] + 0][0] ;
		ret.mMin[1] = r2x[r3x[1] + 0][1] ;
		ret.mMin[2] = r2x[r3x[2] + 0][2] ;
		ret.mMax[0] = r2x[r3x[0] + 1][0] ;
		ret.mMax[1] = r2x[r3x[1] + 1][1] ;
		ret.mMax[2] = r2x[r3x[2] + 1][2] ;
		ret.mMin[3] = 1 ;
		ret.mMax[3] = 1 ;
		return move (ret) ;
	}

	Bool is_bound_contain (CR<Bound<Vector>> bound ,CR<Vector> a) const {
		if (a[0] < bound.mMin[0])
			return FALSE ;
		if (a[0] > bound.mMax[0])
			return FALSE ;
		if (a[1] < bound.mMin[1])
			return FALSE ;
		if (a[1] > bound.mMax[1])
			return FALSE ;
		if (a[2] < bound.mMin[2])
			return FALSE ;
		if (a[2] > bound.mMax[2])
			return FALSE ;
		return TRUE ;
	}

	Index find (CR<Vector> item) const override {
		const auto r1x = find_leaf (self.mNull ,item) ;
		Index jx = curr_next (r1x) ;
		if (jx == self.mNull)
			return NONE ;
		return find_item (jx ,item) ;
	}

	Bool contain (CR<Vector> item) const override {
		return find (item) != NONE ;
	}

	OctTreeChild find_leaf (CR<Index> curr ,CR<Vector> item) const {
		OctTreeChild ret ;
		ret.mUp = curr ;
		ret.mSide = 0 ;
		while (TRUE) {
			Index jx = curr_next (ret) ;
			if (jx == self.mNull)
				break ;
			if (is_leaf (jx))
				break ;
			const auto r1x = child_side (self.mTree[jx].mBound ,item) ;
			ret = OctTreeChild ({jx ,r1x}) ;
		}
		return move (ret) ;
	}

	Index find_item (CR<Index> curr ,CR<Vector> item) const {
		const auto r1x = MathProc::square (self.mEpsilon) ;
		Index ret = self.mTree[curr].mFirst ;
		while (TRUE) {
			if (ret == NONE)
				break ;
			const auto r2x = (self.mLeaf[ret].mPoint - item).norm () ;
			if (r2x < r1x)
				break ;
			ret = self.mLeaf[ret].mNext ;
		}
		return move (ret) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const override {
		assert (neighbor > 0) ;
		assert (radius >= 0) ;
		auto rax = OctTreeResult () ;
		rax.mPriority = Priority<IndexPair<Flt64>> (neighbor) ;
		rax.mCenter = center ;
		rax.mBound.mMin[0] = center[0] - radius ;
		rax.mBound.mMin[1] = center[1] - radius ;
		rax.mBound.mMin[2] = center[2] - radius ;
		rax.mBound.mMax[0] = center[0] + radius ;
		rax.mBound.mMax[1] = center[1] + radius ;
		rax.mBound.mMax[2] = center[2] + radius ;
		rax.mNeighbor = neighbor ;
		rax.mSquareRadius = MathProc::square (radius) ;
		search (self.mRoot ,rax) ;
		const auto r1x = inline_min (rax.mPriority.length () ,neighbor) ;
		Array<Index> ret = Array<Index> (r1x) ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r1x - 1 - i ;
			ret[ix] = rax.mPriority[0].m2nd ;
			rax.mPriority.take () ;
		}
		return move (ret) ;
	}

	void search (CR<Index> curr ,VR<OctTreeResult> result) const {
		if (curr == self.mNull)
			return ;
		const auto r1x = bound_band (self.mTree[curr].mBound ,result.mBound) ;
		if (r1x.mMin[0] >= r1x.mMax[0])
			return ;
		if (r1x.mMin[1] >= r1x.mMax[1])
			return ;
		if (r1x.mMin[2] >= r1x.mMax[2])
			return ;
		if ifdo (TRUE) {
			if (!is_leaf (curr))
				discard ;
			Index ix = self.mTree[curr].mFirst ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				const auto r2x = inline_min (self.mLeaf[ix].mWidth ,result.mNeighbor) ;
				for (auto &&i : range (0 ,r2x)) {
					noop (i) ;
					push_result (ix ,result) ;
				}
				ix = self.mLeaf[ix].mNext ;
			}
			return ;
		}
		for (auto &&i : range (0 ,8)) {
			search (self.mTree[curr].mChild[i] ,result) ;
		}
	}

	Bool is_leaf (CR<Index> curr) const {
		if (self.mTree[curr].mFirst != NONE)
			return TRUE ;
		if (self.mTree[curr].mWidth == 0)
			return TRUE ;
		return FALSE ;
	}

	Bound<Vector> bound_band (CR<Bound<Vector>> a ,CR<Bound<Vector>> b) const {
		Bound<Vector> ret ;
		ret.mMin[0] = MathProc::max_of (a.mMin[0] ,b.mMin[0]) ;
		ret.mMin[1] = MathProc::max_of (a.mMin[1] ,b.mMin[1]) ;
		ret.mMin[2] = MathProc::max_of (a.mMin[2] ,b.mMin[2]) ;
		ret.mMax[0] = MathProc::min_of (a.mMax[0] ,b.mMax[0]) ;
		ret.mMax[1] = MathProc::min_of (a.mMax[1] ,b.mMax[1]) ;
		ret.mMax[2] = MathProc::min_of (a.mMax[2] ,b.mMax[2]) ;
		return move (ret) ;
	}

	void push_result (CR<Index> leaf ,VR<OctTreeResult> result) const {
		const auto r1x = (self.mLeaf[leaf].mPoint - result.mCenter).norm () ;
		if (r1x >= result.mSquareRadius)
			return ;
		if ifdo (TRUE) {
			if (!result.mPriority.full ())
				discard ;
			const auto r2x = -result.mPriority[0].m1st ;
			if (r2x <= r1x)
				discard ;
			result.mPriority.take () ;
		}
		if (result.mPriority.full ())
			return ;
		auto rax = IndexPair<Flt64> () ;
		rax.m1st = -r1x ;
		rax.m2nd = leaf ;
		result.mPriority.add (move (rax)) ;
	}

	VR<Index> curr_next (CR<OctTreeChild> index) leftvalue {
		if (index.mUp == self.mNull)
			return self.mRoot ;
		return self.mTree[index.mUp].mChild[index.mSide] ;
	}

	CR<Index> curr_next (CR<OctTreeChild> index) const leftvalue {
		if (index.mUp == self.mNull)
			return self.mRoot ;
		return self.mTree[index.mUp].mChild[index.mSide] ;
	}
} ;

exports VFat<OctTreeHolder> OctTreeHolder::hold (VR<OctTreeLayout> that) {
	return VFat<OctTreeHolder> (OctTreeImplHolder () ,that) ;
}

exports CFat<OctTreeHolder> OctTreeHolder::hold (CR<OctTreeLayout> that) {
	return CFat<OctTreeHolder> (OctTreeImplHolder () ,that) ;
}

struct KDTreeF32 ;
struct KDTreeF64 ;

struct PointCloudKDTreeLayout {
	Length mSize ;
	Length mAlign ;
	Length mChannel ;
	Ref<KDTreeF32> mF32 ;
	Ref<KDTreeF64> mF64 ;
} ;

exports Ref<PointCloudKDTreeLayout> PointCloudKDTreeHolder::create () {
	return Ref<PointCloudKDTreeLayout>::make () ;
}

template class External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> ;

exports VFat<PointCloudKDTreeHolder> PointCloudKDTreeHolder::hold (VR<PointCloudKDTreeLayout> that) {
	return VFat<PointCloudKDTreeHolder> (External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::expr ,that) ;
}

exports CFat<PointCloudKDTreeHolder> PointCloudKDTreeHolder::hold (CR<PointCloudKDTreeLayout> that) {
	return CFat<PointCloudKDTreeHolder> (External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::expr ,that) ;
}

struct PointCloudTree {
	RefLayout mPointCloud ;
	Length mSize ;
	Length mAlign ;
	Length mChannel ;
	RefBuffer<Byte> mFloatView ;
	PointCloudKDTree mKDTree ;
} ;

class PointCloudImplHolder final implement Fat<PointCloudHolder ,PointCloudLayout> {
public:
	void initialize (RR<Ref<Array<Point2F>>> pointcloud) override {
		self.mThis = SharedRef<PointCloudTree>::make () ;
		self.mThis->mSize = pointcloud->size () ;
		self.mThis->mAlign = SIZE_OF<Flt32>::expr ;
		self.mThis->mChannel = 2 ;
		const auto r1x = address (pointcloud->ref) ;
		const auto r2x = size () * channel () ;
		const auto r3x = Slice (r1x ,r2x ,align ()) ;
		self.mThis->mFloatView = RefBuffer<Byte>::reference (r3x) ;
		self.mThis->mPointCloud = move (pointcloud) ;
		reset_world (self) ;
	}

	void initialize (RR<Ref<Array<Point3F>>> pointcloud) override {
		self.mThis = SharedRef<PointCloudTree>::make () ;
		self.mThis->mSize = pointcloud->size () ;
		self.mThis->mAlign = SIZE_OF<Flt32>::expr ;
		self.mThis->mChannel = 3 ;
		const auto r1x = address (pointcloud->ref) ;
		const auto r2x = size () * channel () ;
		const auto r3x = Slice (r1x ,r2x ,align ()) ;
		self.mThis->mFloatView = RefBuffer<Byte>::reference (r3x) ;
		self.mThis->mPointCloud = move (pointcloud) ;
		reset_world (self) ;
	}

	void initialize (RR<Ref<Array<Vector>>> pointcloud) override {
		self.mThis = SharedRef<PointCloudTree>::make () ;
		self.mThis->mSize = pointcloud->size () ;
		self.mThis->mAlign = SIZE_OF<Flt64>::expr ;
		self.mThis->mChannel = 4 ;
		const auto r1x = address (pointcloud->ref) ;
		const auto r2x = size () * channel () ;
		const auto r3x = Slice (r1x ,r2x ,align ()) ;
		self.mThis->mFloatView = RefBuffer<Byte>::reference (r3x) ;
		self.mThis->mPointCloud = move (pointcloud) ;
		reset_world (self) ;
	}

	void reset_world (VR<PointCloudLayout> that) const {
		that.mWorld = Ref<DuplexMatrix>::make (Matrix::iden ()) ;
		auto &&rax = that.mThis.ref ;
		auto &&rbx = that.mWorld.ref ;
		that.mPointView = FarBuffer<Vector> (rax.mSize) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (rax.mAlign != SIZE_OF<Flt32>::expr)
				discard ;
			if (rax.mChannel != 2)
				discard ;
			that.mPointView.use_getter ([&] (CR<Index> index ,VR<Vector> item) {
				Index ix = index * 2 ;
				const auto r1x = Flt32 (bitwise (rax.mFloatView[ix + 0])) ;
				const auto r2x = Flt32 (bitwise (rax.mFloatView[ix + 1])) ;
				item = Vector (r1x ,r2x ,0 ,1) ;
				item = rbx[0] * item ;
			}) ;
		}
		if ifdo (act) {
			if (rax.mAlign != SIZE_OF<Flt32>::expr)
				discard ;
			if (rax.mChannel != 3)
				discard ;
			that.mPointView.use_getter ([&] (CR<Index> index ,VR<Vector> item) {
				Index ix = index * 3 ;
				const auto r3x = Flt32 (bitwise (rax.mFloatView[ix + 0])) ;
				const auto r4x = Flt32 (bitwise (rax.mFloatView[ix + 1])) ;
				const auto r5x = Flt32 (bitwise (rax.mFloatView[ix + 2])) ;
				item = Vector (r3x ,r4x ,r5x ,1) ;
				item = rbx[0] * item ;
			}) ;
		}
		if ifdo (act) {
			if (rax.mAlign != SIZE_OF<Flt64>::expr)
				discard ;
			if (rax.mChannel != 4)
				discard ;
			that.mPointView.use_getter ([&] (CR<Index> index ,VR<Vector> item) {
				Index ix = index * 4 ;
				const auto r6x = Flt64 (bitwise (rax.mFloatView[ix + 0])) ;
				const auto r7x = Flt64 (bitwise (rax.mFloatView[ix + 1])) ;
				const auto r8x = Flt64 (bitwise (rax.mFloatView[ix + 2])) ;
				item = Vector (r6x ,r7x ,r8x ,1) ;
				item = rbx[0] * item ;
			}) ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	Length size () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mSize ;
	}

	Length align () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mAlign ;
	}

	Length channel () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mChannel ;
	}

	void get (CR<Index> index ,VR<Vector> item) const override {
		item = self.mPointView[index] ;
	}

	Vector pca_center () const {
		Vector ret = Vector::zero () ;
		for (auto &&i : range (0 ,self.mPointView.size ())) {
			ret += self.mPointView[i] ;
		}
		ret = ret.projection () ;
		return move (ret) ;
	}

	Matrix pca_matrix () const override {
		const auto r1x = pca_center () ;
		const auto r2x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			for (auto &&i : range (0 ,self.mPointView.size ())) {
				const auto r3x = self.mPointView[i] - r1x ;
				ret[0][0] += MathProc::square (r3x[0]) ;
				ret[0][1] += r3x[0] * r3x[1] ;
				ret[0][2] += r3x[0] * r3x[2] ;
				ret[1][1] += MathProc::square (r3x[1]) ;
				ret[1][2] += r3x[1] * r3x[2] ;
				ret[2][2] += MathProc::square (r3x[2]) ;
			}
			ret[1][0] = ret[0][1] ;
			ret[2][0] = ret[0][2] ;
			ret[2][1] = ret[1][2] ;
			return move (ret) ;
		}) ;
		const auto r4x = MatrixProc::solve_svd (r2x) ;
		const auto r5x = MathProc::inverse (Flt64 (size ())) ;
		const auto r6x = r4x.mS[0][0] * r5x ;
		const auto r7x = r4x.mS[1][1] * r5x ;
		const auto r8x = r4x.mS[2][2] * r5x ;
		const auto r9x = TranslationMatrix (r1x) ;
		const auto r10x = MatrixProc::solve_trs (r4x.mV) ;
		const auto r11x = DiagMatrix (sqrt_side (r6x) ,sqrt_side (r7x) ,sqrt_side (r8x)) ;
		return r9x * r10x.mR * r11x ;
	}

	Vector box_center (CR<Bound<Vector>> a) const {
		Vector ret = Vector::zero () ;
		ret += a.mMin ;
		ret += a.mMax ;
		ret = ret.projection () ;
		return move (ret) ;
	}

	Matrix box_matrix (CR<Flt64> bx ,CR<Flt64> by ,CR<Flt64> bz) const override {
		const auto r1x = bound () ;
		const auto r2x = box_center (r1x) ;
		const auto r3x = (r1x.mMax - r1x.mMin) / 2 ;
		const auto r4x = MathProc::square (MathProc::max_of (r3x[0] + bx ,Flt64 (0))) ;
		const auto r5x = MathProc::square (MathProc::max_of (r3x[1] + by ,Flt64 (0))) ;
		const auto r6x = MathProc::square (MathProc::max_of (r3x[2] + bz ,Flt64 (0))) ;
		const auto r7x = TranslationMatrix (r2x) ;
		const auto r8x = DiagMatrix (sqrt_side (r4x) ,sqrt_side (r5x) ,sqrt_side (r6x)) ;
		return r7x * r8x ;
	}

	Vector cut_center () const {
		Vector ret ;
		assume (size () % 2 == 1) ;
		Index ix = (size () - 1) / 2 ;
		get (ix ,ret) ;
		return move (ret) ;
	}

	Matrix cut_matrix (CR<Flt64> sx ,CR<Flt64> sy ,CR<Flt64> sz) const override {
		const auto r1x = bound () ;
		const auto r2x = cut_center () ;
		const auto r3x = (r1x.mMin - r2x).sabs () ;
		const auto r4x = (r1x.mMax - r2x).sabs () ;
		const auto r5x = MathProc::min_of (r3x[0] ,r4x[0]) * MathProc::inverse (sx) ;
		const auto r6x = MathProc::min_of (r3x[1] ,r4x[1]) * MathProc::inverse (sy) ;
		const auto r7x = MathProc::min_of (r3x[2] ,r4x[2]) * MathProc::inverse (sz) ;
		const auto r8x = MathProc::delta (r5x) * infinity + r5x ;
		const auto r9x = MathProc::delta (r6x) * infinity + r6x ;
		const auto r10x = MathProc::delta (r7x) * infinity + r7x ;
		const auto r11x = MathProc::min_of (r8x ,r9x ,r10x) ;
		const auto r12x = TranslationMatrix (r2x) ;
		const auto r13x = MathProc::square (sx * r11x) ;
		const auto r14x = MathProc::square (sy * r11x) ;
		const auto r15x = MathProc::square (sz * r11x) ;
		const auto r16x = DiagMatrix (sqrt_side (r13x) ,sqrt_side (r14x) ,sqrt_side (r15x)) ;
		return r12x * r16x ;
	}

	Flt64 sqrt_side (CR<Flt64> a) const {
		return MathProc::sqrt (a + MathProc::delta (a)) ;
	}

	Bound<Vector> bound () const override {
		Bound<Vector> ret ;
		ret.mMin[0] = +infinity ;
		ret.mMin[1] = +infinity ;
		ret.mMin[2] = +infinity ;
		ret.mMax[0] = -infinity ;
		ret.mMax[1] = -infinity ;
		ret.mMax[2] = -infinity ;
		for (auto &&i : range (0 ,self.mPointView.size ())) {
			const auto r1x = self.mPointView[i] ;
			ret.mMin[0] = MathProc::min_of (ret.mMin[0] ,Flt64 (r1x[0])) ;
			ret.mMin[1] = MathProc::min_of (ret.mMin[1] ,Flt64 (r1x[1])) ;
			ret.mMin[2] = MathProc::min_of (ret.mMin[2] ,Flt64 (r1x[2])) ;
			ret.mMax[0] = MathProc::max_of (ret.mMax[0] ,Flt64 (r1x[0])) ;
			ret.mMax[1] = MathProc::max_of (ret.mMax[1] ,Flt64 (r1x[1])) ;
			ret.mMax[2] = MathProc::max_of (ret.mMax[2] ,Flt64 (r1x[2])) ;
		}
		for (auto &&i : range (0 ,3)) {
			if (!MathProc::is_inf (ret.mMin[i]))
				continue ;
			ret.mMin[i] = 0 ;
			ret.mMax[i] = 0 ;
		}
		ret.mMin[3] = 1 ;
		ret.mMax[3] = 1 ;
		return move (ret) ;
	}

	PointCloudLayout smul (CR<Matrix> that) const override {
		PointCloudLayout ret ;
		ret.mThis = self.mThis ;
		reset_world (ret) ;
		ret.mWorld.ref = that.transpose () * self.mWorld.ref[0] ;
		return move (ret) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const override {
		if ifdo (TRUE) {
			if (self.mThis->mKDTree.mThis.exist ())
				discard ;
			const auto r1x = Flag (self.mThis->mFloatView.ref) ;
			const auto r2x = self.mThis->mFloatView.size () ;
			const auto r3x = align () ;
			if ifdo (TRUE) {
				if (r3x != SIZE_OF<Flt32>::expr)
					discard ;
				const auto r4x = Slice (r1x ,r2x ,r3x) ;
				self.mThis->mKDTree = PointCloudKDTree (RefBuffer<Flt32>::reference (r4x) ,channel ()) ;
			}
			if ifdo (TRUE) {
				if (r3x != SIZE_OF<Flt64>::expr)
					discard ;
				const auto r5x = Slice (r1x ,r2x ,r3x) ;
				self.mThis->mKDTree = PointCloudKDTree (RefBuffer<Flt64>::reference (r5x) ,channel ()) ;
			}
		}
		const auto r6x = self.mWorld.ref[1] * center ;
		return self.mThis->mKDTree.search (r6x ,neighbor ,radius) ;
	}
} ;

exports VFat<PointCloudHolder> PointCloudHolder::hold (VR<PointCloudLayout> that) {
	return VFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}

exports CFat<PointCloudHolder> PointCloudHolder::hold (CR<PointCloudLayout> that) {
	return CFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}

class VoxelGridRTreeImplHolder final implement Fat<VoxelGridRTreeHolder ,VoxelGridRTreeLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}
} ;

exports VFat<VoxelGridRTreeHolder> VoxelGridRTreeHolder::hold (VR<VoxelGridRTreeLayout> that) {
	return VFat<VoxelGridRTreeHolder> (VoxelGridRTreeImplHolder () ,that) ;
}

exports CFat<VoxelGridRTreeHolder> VoxelGridRTreeHolder::hold (CR<VoxelGridRTreeLayout> that) {
	return CFat<VoxelGridRTreeHolder> (VoxelGridRTreeImplHolder () ,that) ;
}

class VoxelGridImplHolder final implement Fat<VoxelGridHolder ,VoxelGridLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}
} ;

exports VFat<VoxelGridHolder> VoxelGridHolder::hold (VR<VoxelGridLayout> that) {
	return VFat<VoxelGridHolder> (VoxelGridImplHolder () ,that) ;
}

exports CFat<VoxelGridHolder> VoxelGridHolder::hold (CR<VoxelGridLayout> that) {
	return CFat<VoxelGridHolder> (VoxelGridImplHolder () ,that) ;
}
} ;