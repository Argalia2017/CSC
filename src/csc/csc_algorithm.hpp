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

namespace CSC {
struct DisjointLayout {
	Array<Index> mTable ;
} ;

struct DisjointHolder implement Interface {
	imports VFat<DisjointHolder> hold (VR<DisjointLayout> that) ;
	imports CFat<DisjointHolder> hold (CR<DisjointLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual Length size () const = 0 ;
	virtual Index lead (CR<Index> from_) = 0 ;
	virtual void joint (CR<Index> from_ ,CR<Index> to_) = 0 ;
	virtual Bool edge (CR<Index> from_ ,CR<Index> to_) = 0 ;
	virtual Length depth (CR<Index> from_) = 0 ;
	virtual Deque<Index> cluster (CR<Index> from_) = 0 ;
	virtual Array<Index> jump (CR<Index> from_) = 0 ;
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

	void joint (CR<Index> from_ ,CR<Index> to_) {
		return DisjointHolder::hold (thiz)->joint (from_ ,to_) ;
	}

	Bool edge (CR<Index> from_ ,CR<Index> to_) {
		return DisjointHolder::hold (thiz)->edge (from_ ,to_) ;
	}

	Length depth (CR<Index> from_) {
		return DisjointHolder::hold (thiz)->depth (from_) ;
	}

	Deque<Index> cluster (CR<Index> from_) {
		return DisjointHolder::hold (thiz)->cluster (from_) ;
	}

	Array<Index> jump (CR<Index> from_) {
		return DisjointHolder::hold (thiz)->jump (from_) ;
	}
} ;

struct KMMatchLayout {
	Length mSize ;
	Flt32 mThreshold ;
	Ref<Image<Flt32>> mLove ;
	Array<Flt32> mUser ;
	Array<Flt32> mWork ;
	BitSet mUserVisit ;
	BitSet mWorkVisit ;
	Array<Index> mMatch ;
	Array<Flt32> mLack ;
} ;

struct KMMatchHolder implement Interface {
	imports VFat<KMMatchHolder> hold (VR<KMMatchLayout> that) ;
	imports CFat<KMMatchHolder> hold (CR<KMMatchLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void set_threshold (CR<Flt64> threshold) = 0 ;
	virtual Length size () const = 0 ;
	virtual Array<Index> sort (CR<Image<Flt32>> love) = 0 ;
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

	Array<Index> sort (CR<Image<Flt32>> love) {
		return KMMatchHolder::hold (thiz)->sort (love) ;
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

	forceinline Vector operator() (CR<Vector> that) const {
		return smul (that) ;
	}

	forceinline friend Vector operator* (CR<TPSFit> thiz_ ,CR<Vector> that) {
		return thiz_.smul (that) ;
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
	virtual CR<Array<Vector>> ref_m () const leftvalue = 0 ;
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

	CR<Array<Vector>> ref_m () const leftvalue {
		return BCSFitHolder::hold (thiz)->ref_m () ;
	}

	Vector smul (CR<Vector> that) const {
		return BCSFitHolder::hold (thiz)->smul (that) ;
	}

	forceinline Vector operator() (CR<Vector> that) const {
		return smul (that) ;
	}

	forceinline friend Vector operator* (CR<BCSFit> thiz_ ,CR<Vector> that) {
		return thiz_.smul (that) ;
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

	forceinline Array<Vector> operator() (CR<Array<Vector>> that) const {
		return smul (that) ;
	}

	forceinline friend Array<Vector> operator* (CR<FFTransform> thiz_ ,CR<Array<Vector>> that) {
		return thiz_.smul (that) ;
	}

	FFTransform inverse () const {
		FFTransformLayout ret = FFTransformHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<FFTransform>::expr] (ret)) ;
	}
} ;
} ;