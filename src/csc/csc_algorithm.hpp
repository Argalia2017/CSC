#pragma once

#ifndef __CSC_MATRIX__
#define __CSC_MATRIX__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_image.hpp"
#include "csc_matrix.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_file.hpp"
#include "csc_thread.hpp"

namespace CSC {
struct DisjointNode {
	Index mUp ;
	Length mWidth ;
} ;

struct DisjointLayout {
	Array<DisjointNode> mTable ;
} ;

struct DisjointHolder implement Interface {
	imports VFat<DisjointHolder> hold (VR<DisjointLayout> that) ;
	imports CFat<DisjointHolder> hold (CR<DisjointLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual Length size () const = 0 ;
	virtual Index lead (CR<Index> from) = 0 ;
	virtual Length width (CR<Index> from) const = 0 ;
	virtual void joint (CR<Index> from ,CR<Index> into) = 0 ;
	virtual Bool is_edge (CR<Index> from ,CR<Index> into) = 0 ;
	virtual Deque<Index> cluster (CR<Index> from) = 0 ;
	virtual Array<Index> closure () = 0 ;
} ;

class Disjoint implement DisjointLayout {
protected:
	using DisjointLayout::mTable ;

public:
	implicit Disjoint () = default ;

	explicit Disjoint (CR<Length> size_) {
		DisjointHolder::hold (thiz)->initialize (size_) ;
	}

	Length size () const {
		return DisjointHolder::hold (thiz)->size () ;
	}

	Index lead (CR<Index> from) {
		return DisjointHolder::hold (thiz)->lead (from) ;
	}

	Length width (CR<Index> from) const {
		return DisjointHolder::hold (thiz)->width (from) ;
	}

	void joint (CR<Index> from ,CR<Index> into) {
		return DisjointHolder::hold (thiz)->joint (from ,into) ;
	}

	Bool is_edge (CR<Index> from ,CR<Index> into) {
		return DisjointHolder::hold (thiz)->is_edge (from ,into) ;
	}

	Deque<Index> cluster (CR<Index> from) {
		return DisjointHolder::hold (thiz)->cluster (from) ;
	}

	Array<Index> closure () {
		return DisjointHolder::hold (thiz)->closure () ;
	}
} ;
struct RansacLayout {
	Length mRank ;
	Length mSize ;
	Random mRandom ;
	Length mIteration ;
	Length mMaxIteration ;
	Flt64 mProbability ;
	Flt64 mProbFactor ;
	Array<Index> mSample ;
	BitSet mCurrInlier ;
	BitSet mBestInlier ;
	Length mCurrSize ;
	Length mBestSize ;
} ;

struct RansacHolder implement Interface {
	imports VFat<RansacHolder> hold (VR<RansacLayout> that) ;
	imports CFat<RansacHolder> hold (CR<RansacLayout> that) ;

	virtual void initialize (CR<Length> rank_ ,CR<Length> size_) = 0 ;
	virtual void set_seed (CR<Flag> seed_) = 0 ;
	virtual void set_probability (CR<Flt64> probability) = 0 ;
	virtual void set_iteration (CR<Length> iteration) = 0 ;
	virtual Length size () const = 0 ;
	virtual void sample () = 0 ;
	virtual CR<Pointer> peek () const leftvalue = 0 ;
	virtual Bool good () const = 0 ;
	virtual void next () = 0 ;
	virtual void add (CR<Index> index) = 0 ;
	virtual BitSet cluster () const = 0 ;
	virtual Length iteration () const = 0 ;
	virtual Flt64 percent () const = 0 ;
} ;

template <class A>
class Ransac implement RansacLayout {
public:
	implicit Ransac () = default ;

	explicit Ransac (CR<Length> size_) {
		RansacHolder::hold (thiz)->initialize (A::expr ,size_) ;
	}

	void set_seed (CR<Flag> seed_) {
		return RansacHolder::hold (thiz)->set_seed (seed_) ;
	}

	void set_probability (CR<Flt64> probability) {
		return RansacHolder::hold (thiz)->set_probability (probability) ;
	}

	void set_iteration (CR<Length> iteration) {
		return RansacHolder::hold (thiz)->set_iteration (iteration) ;
	}

	Length size () const {
		return RansacHolder::hold (thiz)->size () ;
	}

	Bool good () const {
		return RansacHolder::hold (thiz)->good () ;
	}

	forceinline Bool operator== (CR<Ransac>) const {
		return (!good ()) ;
	}

	forceinline Bool operator!= (CR<Ransac>) const {
		return good () ;
	}

	void sample () {
		return RansacHolder::hold (thiz)->sample () ;
	}

	CR<Buffer<Index ,A>> peek () const leftvalue {
		return RansacHolder::hold (thiz)->peek () ;
	}

	forceinline CR<Buffer<Index ,A>> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		return RansacHolder::hold (thiz)->next () ;
	}

	forceinline void operator++ () {
		next () ;
	}

	void add (CR<Index> index) {
		return RansacHolder::hold (thiz)->add (index) ;
	}

	BitSet cluster () const {
		return RansacHolder::hold (thiz)->cluster () ;
	}

	Length iteration () const {
		return RansacHolder::hold (thiz)->iteration () ;
	}

	Flt64 percent () const {
		return RansacHolder::hold (thiz)->percent () ;
	}
} ;

struct MinCutEdge {
	Index mFrom ;
	Index mInto ;
	Index mNext ;
	Val64 mWeight ;
	Index mJump ;
	Index mInv ;
} ;

struct MinCutLayout {
	Length mSize ;
	Index mRootS ;
	Index mRootT ;
	Array<Index> mFirst ;
	Array<Index> mCurrent ;
	Array<Length> mDepth ;
	List<MinCutEdge> mEdge ;
	Set<Tuple<Index ,Index>> mEdgeKey ;
	Deque<Index> mDeque ;
	Bool mReady ;
} ;

struct MinCutHolder implement Interface {
	imports VFat<MinCutHolder> hold (VR<MinCutLayout> that) ;
	imports CFat<MinCutHolder> hold (CR<MinCutLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual Length size () const = 0 ;
	virtual Index root_s () const = 0 ;
	virtual Index root_t () const = 0 ;
	virtual void joint (CR<Index> from ,CR<Index> into ,CR<Val64> weight) = 0 ;
	virtual Val64 solve () = 0 ;
	virtual BitSet cluster () const = 0 ;
} ;

class MinCut implement MinCutLayout {
protected:
	using MinCutLayout::mSize ;
	using MinCutLayout::mRootS ;
	using MinCutLayout::mRootT ;
	using MinCutLayout::mFirst ;
	using MinCutLayout::mCurrent ;
	using MinCutLayout::mDepth ;
	using MinCutLayout::mEdge ;
	using MinCutLayout::mEdgeKey ;
	using MinCutLayout::mDeque ;
	using MinCutLayout::mReady ;

public:
	implicit MinCut () = default ;

	explicit MinCut (CR<Length> size_) {
		MinCutHolder::hold (thiz)->initialize (size_) ;
	}

	Length size () const {
		return MinCutHolder::hold (thiz)->size () ;
	}

	Index root_s () const {
		return MinCutHolder::hold (thiz)->root_s () ;
	}

	Index root_t () const {
		return MinCutHolder::hold (thiz)->root_t () ;
	}

	void joint (CR<Index> from ,CR<Index> into ,CR<Val64> weight) {
		return MinCutHolder::hold (thiz)->joint (from ,into ,weight) ;
	}

	Val64 solve () {
		return MinCutHolder::hold (thiz)->solve () ;
	}

	BitSet cluster () const {
		return MinCutHolder::hold (thiz)->cluster () ;
	}
} ;

struct KMMatchLayout {
	Length mSize ;
	Flt64 mThreshold ;
	Ref<Image<Flt64>> mLove ;
	Array<Flt64> mUser ;
	Array<Flt64> mWork ;
	BitSet mUserVisit ;
	BitSet mWorkVisit ;
	Array<Index> mMatch ;
	Array<Flt64> mLack ;
} ;

struct KMMatchHolder implement Interface {
	imports VFat<KMMatchHolder> hold (VR<KMMatchLayout> that) ;
	imports CFat<KMMatchHolder> hold (CR<KMMatchLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void set_threshold (CR<Flt64> threshold) = 0 ;
	virtual Length size () const = 0 ;
	virtual Array<Index> solve (CR<Image<Flt64>> love) = 0 ;
} ;

class KMMatch implement KMMatchLayout {
protected:
	using KMMatchLayout::mSize ;
	using KMMatchLayout::mThreshold ;
	using KMMatchLayout::mLove ;
	using KMMatchLayout::mUser ;
	using KMMatchLayout::mWork ;
	using KMMatchLayout::mUserVisit ;
	using KMMatchLayout::mWorkVisit ;
	using KMMatchLayout::mMatch ;
	using KMMatchLayout::mLack ;

public:
	implicit KMMatch () = default ;

	explicit KMMatch (CR<Length> size_) {
		KMMatchHolder::hold (thiz)->initialize (size_) ;
	}

	void set_threshold (CR<Flt64> threshold) {
		return KMMatchHolder::hold (thiz)->set_threshold (threshold) ;
	}

	Length size () const {
		return KMMatchHolder::hold (thiz)->size () ;
	}

	Array<Index> solve (CR<Image<Flt64>> love) {
		return KMMatchHolder::hold (thiz)->solve (love) ;
	}
} ;

struct TPSFitLayout {
	DuplexMatrix mNSrc ;
	DuplexMatrix mNDst ;
	Array<Vector> mPSrc ;
	Image<Flt64> mQA ;
	Image<Flt64> mQB ;
	Image<Flt64> mQC ;
} ;

struct TPSFitHolder implement Interface {
	imports VFat<TPSFitHolder> hold (VR<TPSFitLayout> that) ;
	imports CFat<TPSFitHolder> hold (CR<TPSFitLayout> that) ;

	virtual void initialize (CR<Array<Vector>> dst ,CR<Array<Vector>> src) = 0 ;
	virtual Vector smul (CR<Vector> that) const = 0 ;
	virtual Matrix jacobian (CR<Vector> that) const = 0 ;
} ;

class TPSFit implement TPSFitLayout {
protected:
	using TPSFitLayout::mNSrc ;
	using TPSFitLayout::mNDst ;
	using TPSFitLayout::mPSrc ;
	using TPSFitLayout::mQA ;
	using TPSFitLayout::mQB ;
	using TPSFitLayout::mQC ;

public:
	implicit TPSFit () = default ;

	explicit TPSFit (CR<Array<Vector>> dst ,CR<Array<Vector>> src) {
		TPSFitHolder::hold (thiz)->initialize (dst ,src) ;
	}

	Vector smul (CR<Vector> that) const {
		return TPSFitHolder::hold (thiz)->smul (that) ;
	}

	forceinline Vector operator* (CR<Vector> that) const {
		return smul (that) ;
	}

	Matrix jacobian (CR<Vector> that) const {
		return TPSFitHolder::hold (thiz)->jacobian (that) ;
	}
} ;

struct BCSFitLayout {
	Length mRank ;
	DuplexMatrix mNSrc ;
	Array<Vector> mPCtrl ;
	Image<Flt64> mQA ;
	Image<Flt64> mQB ;
	Image<Flt64> mQC ;
} ;

struct BCSFitHolder implement Interface {
	imports VFat<BCSFitHolder> hold (VR<BCSFitLayout> that) ;
	imports CFat<BCSFitHolder> hold (CR<BCSFitLayout> that) ;

	virtual void initialize (CR<Array<Vector>> dst ,CR<Array<Vector>> src) = 0 ;
	virtual CR<Array<Vector>> control () const leftvalue = 0 ;
	virtual Vector smul (CR<Vector> that) const = 0 ;
	virtual Matrix jacobian (CR<Vector> that) const = 0 ;
} ;

class BCSFit implement BCSFitLayout {
protected:
	using BCSFitLayout::mRank ;
	using BCSFitLayout::mNSrc ;
	using BCSFitLayout::mPCtrl ;
	using BCSFitLayout::mQA ;
	using BCSFitLayout::mQB ;
	using BCSFitLayout::mQC ;

public:
	implicit BCSFit () = default ;

	explicit BCSFit (CR<Array<Vector>> dst ,CR<Array<Vector>> src) {
		BCSFitHolder::hold (thiz)->initialize (dst ,src) ;
	}

	CR<Array<Vector>> control () const leftvalue {
		return BCSFitHolder::hold (thiz)->control () ;
	}

	Vector smul (CR<Vector> that) const {
		return BCSFitHolder::hold (thiz)->smul (that) ;
	}

	forceinline Vector operator* (CR<Vector> that) const {
		return smul (that) ;
	}

	Matrix jacobian (CR<Vector> that) const {
		return BCSFitHolder::hold (thiz)->jacobian (that) ;
	}
} ;

struct FFTransformLayout {
	Length mSize ;
	Length mRank ;
	Array<Array<Vector>> mCosSin ;
	Buffer3<Flt64> mScale ;
} ;

struct FFTransformHolder implement Interface {
	imports VFat<FFTransformHolder> hold (VR<FFTransformLayout> that) ;
	imports CFat<FFTransformHolder> hold (CR<FFTransformLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void set_unitary (CR<Bool> flag) = 0 ;
	virtual Length size () const = 0 ;
	virtual Array<Vector> smul (CR<Array<Vector>> that) const = 0 ;
	virtual FFTransformLayout inverse () const = 0 ;
} ;

class FFTransform implement FFTransformLayout {
protected:
	using FFTransformLayout::mSize ;
	using FFTransformLayout::mRank ;
	using FFTransformLayout::mCosSin ;
	using FFTransformLayout::mScale ;

public:
	implicit FFTransform () = default ;

	explicit FFTransform (CR<Length> size_) {
		FFTransformHolder::hold (thiz)->initialize (size_) ;
	}

	void set_unitary (CR<Bool> flag) {
		return FFTransformHolder::hold (thiz)->set_unitary (flag) ;
	}

	Length size () const {
		return FFTransformHolder::hold (thiz)->size () ;
	}

	Array<Vector> smul (CR<Array<Vector>> that) const {
		return FFTransformHolder::hold (thiz)->smul (that) ;
	}

	forceinline Array<Vector> operator* (CR<Array<Vector>> that) const {
		return smul (that) ;
	}

	FFTransform inverse () const {
		FFTransformLayout ret = FFTransformHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<FFTransform>::expr] (ret)) ;
	}
} ;
} ;