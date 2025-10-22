﻿#pragma once

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

namespace CSC {
struct Point2F {
	FLT32 mX ;
	FLT32 mY ;
} ;

struct Point3F {
	FLT32 mX ;
	FLT32 mY ;
	FLT32 mZ ;
} ;

struct Line2F {
	Point2F mMin ;
	Point2F mMax ;
} ;

struct Line3F {
	Point3F mMin ;
	Point3F mMax ;
} ;

template <class A>
class XYZProxy {
protected:
	XR<A> mThat ;

public:
	implicit XYZProxy () = delete ;

	explicit XYZProxy (XR<A> that) :mThat (that) {}

	forceinline operator Point2F () rightvalue {
		Point2F ret ;
		ret.mX = FLT32 (mThat.at (0)) ;
		ret.mY = FLT32 (mThat.at (1)) ;
		return move (ret) ;
	}

	forceinline operator Point3F () rightvalue {
		Point3F ret ;
		ret.mX = FLT32 (mThat.at (0)) ;
		ret.mY = FLT32 (mThat.at (1)) ;
		ret.mZ = FLT32 (mThat.at (2)) ;
		return move (ret) ;
	}
} ;

struct VectorLayout ;
struct MatrixLayout ;
struct QuaternionLayout ;

struct VectorLayout {
	Buffer<FLT64 ,RANK4> mVector ;
} ;

struct VectorHolder implement Interface {
	imports VFat<VectorHolder> hold (VR<VectorLayout> that) ;
	imports CFat<VectorHolder> hold (CR<VectorLayout> that) ;

	virtual void initialize (CR<Buffer<FLT64 ,RANK4>> that) = 0 ;
	virtual void initialize (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z ,CR<FLT64> w) = 0 ;
	virtual void initialize (CR<Pixel> point) = 0 ;
	virtual void initialize (CR<Point2F> point) = 0 ;
	virtual void initialize (CR<Point3F> point) = 0 ;
	virtual VR<FLT64> at (CR<INDEX> y) leftvalue = 0 ;
	virtual CR<FLT64> at (CR<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CR<VectorLayout> that) const = 0 ;
	virtual FLAG compr (CR<VectorLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual VectorLayout sadd (CR<VectorLayout> that) const = 0 ;
	virtual VectorLayout ssub (CR<VectorLayout> that) const = 0 ;
	virtual VectorLayout smul (CR<FLT64> that) const = 0 ;
	virtual VectorLayout sdiv (CR<FLT64> that) const = 0 ;
	virtual FLT64 dot (CR<VectorLayout> that) const = 0 ;
	virtual VectorLayout smul (CR<MatrixLayout> that) const = 0 ;
	virtual VectorLayout cross (CR<VectorLayout> that) const = 0 ;
	virtual VectorLayout sabs () const = 0 ;
	virtual VectorLayout minus () const = 0 ;
	virtual FLT64 magnitude () const = 0 ;
	virtual VectorLayout normalize () const = 0 ;
	virtual VectorLayout projection () const = 0 ;
	virtual VectorLayout homogenize () const = 0 ;
} ;

class Vector implement VectorLayout {
protected:
	using VectorLayout::mVector ;

public:
	implicit Vector () = default ;

	explicit Vector (CR<Buffer<FLT64 ,RANK4>> that) {
		VectorHolder::hold (thiz)->initialize (that) ;
	}

	explicit Vector (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z ,CR<FLT64> w) {
		VectorHolder::hold (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Vector (CR<Pixel> point) {
		VectorHolder::hold (thiz)->initialize (point) ;
	}

	explicit Vector (CR<Point2F> point) {
		VectorHolder::hold (thiz)->initialize (point) ;
	}

	explicit Vector (CR<Point3F> point) {
		VectorHolder::hold (thiz)->initialize (point) ;
	}

	static CR<Vector> zero () {
		return memorize ([&] () {
			return Vector (0 ,0 ,0 ,0) ;
		}) ;
	}

	static CR<Vector> axis_x () {
		return memorize ([&] () {
			return Vector (1 ,0 ,0 ,0) ;
		}) ;
	}

	static CR<Vector> axis_y () {
		return memorize ([&] () {
			return Vector (0 ,1 ,0 ,0) ;
		}) ;
	}

	static CR<Vector> axis_z () {
		return memorize ([&] () {
			return Vector (0 ,0 ,1 ,0) ;
		}) ;
	}

	static CR<Vector> axis_w () {
		return memorize ([&] () {
			return Vector (0 ,0 ,0 ,1) ;
		}) ;
	}

	XYZProxy<CR<Vector>> xyz () const leftvalue {
		return XYZProxy<CR<Vector>> (thiz) ;
	}

	VR<FLT64> at (CR<INDEX> y) leftvalue {
		return VectorHolder::hold (thiz)->at (y) ;
	}

	forceinline VR<FLT64> operator[] (CR<INDEX> y) leftvalue {
		return at (y) ;
	}

	CR<FLT64> at (CR<INDEX> y) const leftvalue {
		return VectorHolder::hold (thiz)->at (y) ;
	}

	forceinline CR<FLT64> operator[] (CR<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CR<Vector> that) const {
		return VectorHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Vector> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Vector> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Vector> that) const {
		return VectorHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Vector> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Vector> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Vector> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Vector> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return VectorHolder::hold (thiz)->visit (visitor) ;
	}

	Vector sadd (CR<Vector> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator+ (CR<Vector> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<Vector> that) {
		thiz = sadd (that) ;
	}

	Vector ssub (CR<Vector> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator- (CR<Vector> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<Vector> that) {
		thiz = ssub (that) ;
	}

	Vector smul (CR<FLT64> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CR<FLT64> that) const {
		return smul (that) ;
	}

	forceinline friend Vector operator* (CR<FLT64> thiz_ ,CR<Vector> that) {
		return that.smul (thiz_) ;
	}

	forceinline void operator*= (CR<FLT64> that) {
		thiz = smul (that) ;
	}

	Vector sdiv (CR<FLT64> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator/ (CR<FLT64> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CR<FLT64> that) {
		thiz = sdiv (that) ;
	}

	FLT64 dot (CR<Vector> that) const {
		return VectorHolder::hold (thiz)->dot (that) ;
	}

	forceinline FLT64 operator* (CR<Vector> that) const {
		return dot (that) ;
	}

	Vector smul (CR<MatrixLayout> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CR<MatrixLayout> that) const {
		return smul (that) ;
	}

	Vector cross (CR<Vector> that) const {
		VectorLayout ret = VectorHolder::hold (thiz)->cross (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator^ (CR<Vector> that) const {
		return cross (that) ;
	}

	forceinline void operator^= (CR<Vector> that) {
		thiz = cross (that) ;
	}

	Vector sabs () const {
		VectorLayout ret = VectorHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector minus () const {
		VectorLayout ret = VectorHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator- () const {
		return minus () ;
	}

	FLT64 magnitude () const {
		return VectorHolder::hold (thiz)->magnitude () ;
	}

	Vector normalize () const {
		VectorLayout ret = VectorHolder::hold (thiz)->normalize () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector projection () const {
		VectorLayout ret = VectorHolder::hold (thiz)->projection () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector homogenize () const {
		VectorLayout ret = VectorHolder::hold (thiz)->homogenize () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}
} ;

struct MatrixLayout {
	Buffer<FLT64 ,ENUM<16>> mMatrix ;
} ;

struct MatrixHolder implement Interface {
	imports VFat<MatrixHolder> hold (VR<MatrixLayout> that) ;
	imports CFat<MatrixHolder> hold (CR<MatrixLayout> that) ;

	virtual void initialize (CR<Buffer<FLT64 ,ENUM<16>>> that) = 0 ;
	virtual void initialize (CR<VectorLayout> x ,CR<VectorLayout> y ,CR<VectorLayout> z ,CR<VectorLayout> w) = 0 ;
	virtual VR<FLT64> at (CR<INDEX> x ,CR<INDEX> y) leftvalue = 0 ;
	virtual CR<FLT64> at (CR<INDEX> x ,CR<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CR<MatrixLayout> that) const = 0 ;
	virtual FLAG compr (CR<MatrixLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual MatrixLayout sadd (CR<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout ssub (CR<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout smul (CR<FLT64> that) const = 0 ;
	virtual MatrixLayout sdiv (CR<FLT64> that) const = 0 ;
	virtual VectorLayout smul (CR<VectorLayout> that) const = 0 ;
	virtual MatrixLayout smul (CR<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout sabs () const = 0 ;
	virtual MatrixLayout minus () const = 0 ;
	virtual MatrixLayout transpose () const = 0 ;
	virtual MatrixLayout triangular () const = 0 ;
	virtual MatrixLayout homogenize () const = 0 ;
	virtual FLT64 determinant () const = 0 ;
	virtual MatrixLayout adjoint () const = 0 ;
	virtual MatrixLayout inverse () const = 0 ;
	virtual FLT64 trace () const = 0 ;
} ;

class Matrix implement MatrixLayout {
protected:
	using MatrixLayout::mMatrix ;

public:
	implicit Matrix () = default ;

	explicit Matrix (CR<Buffer<FLT64 ,ENUM<16>>> that) {
		MatrixHolder::hold (thiz)->initialize (that) ;
	}

	explicit Matrix (CR<Vector> x ,CR<Vector> y ,CR<Vector> z ,CR<Vector> w) {
		MatrixHolder::hold (thiz)->initialize (x ,y ,z ,w) ;
	}

	static CR<Matrix> zero () {
		return memorize ([&] () {
			const auto r1x = Vector::zero () ;
			return Matrix (r1x ,r1x ,r1x ,r1x) ;
		}) ;
	}

	static CR<Matrix> iden () {
		return memorize ([&] () {
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}) ;
	}

	static CR<Matrix> axis_w () {
		return memorize ([&] () {
			const auto r1x = Vector::zero () ;
			const auto r2x = Vector::axis_w () ;
			return Matrix (r1x ,r1x ,r1x ,r2x) ;
		}) ;
	}

	VR<FLT64> at (CR<INDEX> x ,CR<INDEX> y) leftvalue {
		return MatrixHolder::hold (thiz)->at (x ,y) ;
	}

	VR<FLT64> at (CR<Pixel> index) leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline VR<FLT64> operator[] (CR<Pixel> index) leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<VR<Matrix>> operator[] (CR<INDEX> y) leftvalue {
		return RowProxy<VR<Matrix>> (thiz ,y) ;
	}

	CR<FLT64> at (CR<INDEX> x ,CR<INDEX> y) const leftvalue {
		return MatrixHolder::hold (thiz)->at (x ,y) ;
	}

	CR<FLT64> at (CR<Pixel> index) const leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline CR<FLT64> operator[] (CR<Pixel> index) const leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<CR<Matrix>> operator[] (CR<INDEX> y) const leftvalue {
		return RowProxy<CR<Matrix>> (thiz ,y) ;
	}

	BOOL equal (CR<Matrix> that) const {
		return MatrixHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Matrix> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Matrix> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Matrix> that) const {
		return MatrixHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Matrix> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Matrix> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Matrix> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Matrix> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return MatrixHolder::hold (thiz)->visit (visitor) ;
	}

	Matrix sadd (CR<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator+ (CR<Matrix> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<Matrix> that) {
		thiz = sadd (that) ;
	}

	Matrix ssub (CR<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator- (CR<Matrix> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<Matrix> that) {
		thiz = ssub (that) ;
	}

	Matrix smul (CR<FLT64> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator* (CR<FLT64> that) const {
		return smul (that) ;
	}

	forceinline friend Matrix operator* (CR<FLT64> thiz_ ,CR<Matrix> that) {
		return that.smul (thiz_) ;
	}

	forceinline void operator*= (CR<FLT64> that) {
		thiz = smul (that) ;
	}

	Matrix sdiv (CR<FLT64> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator/ (CR<FLT64> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CR<FLT64> that) {
		thiz = sdiv (that) ;
	}

	Vector smul (CR<Vector> that) const {
		VectorLayout ret = MatrixHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CR<Vector> that) const {
		return smul (that) ;
	}

	Matrix smul (CR<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator* (CR<Matrix> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CR<Matrix> that) {
		thiz = smul (that) ;
	}

	Matrix sabs () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix minus () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator- () const {
		return minus () ;
	}

	Matrix transpose () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->transpose () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix triangular () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->triangular () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix homogenize () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->homogenize () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 determinant () const {
		return MatrixHolder::hold (thiz)->determinant () ;
	}

	Matrix adjoint () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->adjoint () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix inverse () const {
		MatrixLayout ret = MatrixHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 trace () const {
		return MatrixHolder::hold (thiz)->trace () ;
	}
} ;

struct ViewMatrixOption {
	enum {
		XYZ ,
		XZY ,
		YXZ ,
		YZX ,
		ZXY ,
		ZYX ,
		ETC
	} ;
} ;

struct MakeMatrixHolder implement Interface {
	imports VFat<MakeMatrixHolder> hold (VR<MatrixLayout> that) ;
	imports CFat<MakeMatrixHolder> hold (CR<MatrixLayout> that) ;

	virtual void make_DiagMatrix (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z ,CR<FLT64> w) = 0 ;
	virtual void make_ShearMatrix (CR<Vector> x ,CR<Vector> y ,CR<Vector> z) = 0 ;
	virtual void make_RotationMatrix (CR<Vector> normal ,CR<FLT64> angle) = 0 ;
	virtual void make_RotationMatrix (CR<Vector> from ,CR<Vector> into) = 0 ;
	virtual void make_TranslationMatrix (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z) = 0 ;
	virtual void make_PerspectiveMatrix (CR<FLT64> fovx ,CR<ImageShape> shape) = 0 ;
	virtual void make_PerspectiveMatrix (CR<FLT64> fx ,CR<FLT64> fy ,CR<FLT64> cx ,CR<FLT64> cy) = 0 ;
	virtual void make_ProjectionMatrix (CR<Vector> normal ,CR<Vector> center ,CR<Vector> light) = 0 ;
	virtual void make_ViewMatrix (CR<Vector> vx ,CR<Vector> vy) = 0 ;
	virtual void make_ViewMatrix (CR<Vector> vx ,CR<Vector> vy ,CR<Just<ViewMatrixOption>> option) = 0 ;
	virtual void make_CrossProductMatrix (CR<Vector> xyz) = 0 ;
	virtual void make_OuterProductMatrix (CR<Vector> x ,CR<Vector> y) = 0 ;
} ;

inline Matrix DiagMatrix (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_DiagMatrix (x ,y ,z ,1) ;
	return move (ret) ;
}

inline Matrix DiagMatrix (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z ,CR<FLT64> w) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_DiagMatrix (x ,y ,z ,w) ;
	return move (ret) ;
}

inline Matrix ShearMatrix (CR<Vector> x ,CR<Vector> y ,CR<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ShearMatrix (x ,y ,z) ;
	return move (ret) ;
}

inline Matrix RotationMatrix (CR<Vector> normal ,CR<FLT64> angle) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_RotationMatrix (normal ,angle) ;
	return move (ret) ;
}

inline Matrix RotationMatrix (CR<Vector> from ,CR<Vector> into) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_RotationMatrix (from ,into) ;
	return move (ret) ;
}

inline Matrix TranslationMatrix (CR<Vector> xyz) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_TranslationMatrix (xyz[0] ,xyz[1] ,xyz[2]) ;
	return move (ret) ;
}

inline Matrix TranslationMatrix (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_TranslationMatrix (x ,y ,z) ;
	return move (ret) ;
}

inline Matrix PerspectiveMatrix (CR<FLT64> fovx ,CR<ImageShape> shape) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_PerspectiveMatrix (fovx ,shape) ;
	return move (ret) ;
}

inline Matrix PerspectiveMatrix (CR<FLT64> fx ,CR<FLT64> fy ,CR<FLT64> cx ,CR<FLT64> cy) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_PerspectiveMatrix (fx ,fy ,cx ,cy) ;
	return move (ret) ;
}

inline Matrix ProjectionMatrix (CR<Vector> normal ,CR<Vector> center ,CR<Vector> light) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ProjectionMatrix (normal ,center ,light) ;
	return move (ret) ;
}

inline Matrix ViewMatrixXYZ (CR<Vector> x ,CR<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (x ,y ,ViewMatrixOption::XYZ) ;
	return move (ret) ;
}

inline Matrix ViewMatrixXZY (CR<Vector> x ,CR<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (x ,z ,ViewMatrixOption::XZY) ;
	return move (ret) ;
}

inline Matrix ViewMatrixYXZ (CR<Vector> y ,CR<Vector> x) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (y ,x ,ViewMatrixOption::YXZ) ;
	return move (ret) ;
}

inline Matrix ViewMatrixYZX (CR<Vector> y ,CR<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (y ,z ,ViewMatrixOption::YZX) ;
	return move (ret) ;
}

inline Matrix ViewMatrixZXY (CR<Vector> z ,CR<Vector> x) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (z ,x ,ViewMatrixOption::ZXY) ;
	return move (ret) ;
}

inline Matrix ViewMatrixZYX (CR<Vector> z ,CR<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_ViewMatrix (z ,y ,ViewMatrixOption::ZYX) ;
	return move (ret) ;
}

inline Matrix CrossProductMatrix (CR<Vector> xyz) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_CrossProductMatrix (xyz) ;
	return move (ret) ;
}

inline Matrix OuterProductMatrix (CR<Vector> x ,CR<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_OuterProductMatrix (x ,y) ;
	return move (ret) ;
}

inline Matrix SymmetryMatrix (CR<Vector> x ,CR<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::hold (ret)->make_OuterProductMatrix (x ,y) ;
	return move (ret) ;
}

struct TRSResult {
	Matrix mT ;
	Matrix mR ;
	Matrix mS ;
} ;

struct KRTResult {
	Matrix mK ;
	Matrix mR ;
	Matrix mT ;
	Vector mN ;
	Vector mC ;
} ;

struct SVDResult {
	Matrix mU ;
	Matrix mS ;
	Matrix mV ;
} ;

struct MatrixProcLayout ;

struct MatrixProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<MatrixProcLayout>>> expr_m () ;
	imports VFat<MatrixProcHolder> hold (VR<MatrixProcLayout> that) ;
	imports CFat<MatrixProcHolder> hold (CR<MatrixProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual TRSResult solve_trs (CR<Matrix> a) const = 0 ;
	virtual KRTResult solve_krt (CR<Matrix> a) const = 0 ;
	virtual SVDResult solve_svd (CR<Matrix> a) const = 0 ;
} ;

class MatrixProc implement OfThis<UniqueRef<MatrixProcLayout>> {
public:
	static CR<MatrixProc> expr_m () {
		return keep[TYPE<MatrixProc>::expr] (MatrixProcHolder::expr) ;
	}

	static TRSResult solve_trs (CR<Matrix> a) {
		return MatrixProcHolder::hold (expr)->solve_trs (a) ;
	}

	static KRTResult solve_krt (CR<Matrix> a) {
		return MatrixProcHolder::hold (expr)->solve_krt (a) ;
	}

	static SVDResult solve_svd (CR<Matrix> a) {
		return MatrixProcHolder::hold (expr)->solve_svd (a) ;
	}
} ;

struct DuplexMatrixLayout {
	Buffer2<Matrix> mDuplexMatrix ;
} ;

struct DuplexMatrixHolder implement Interface {
	imports VFat<DuplexMatrixHolder> hold (VR<DuplexMatrixLayout> that) ;
	imports CFat<DuplexMatrixHolder> hold (CR<DuplexMatrixLayout> that) ;

	virtual void initialize (CR<Matrix> that) = 0 ;
	virtual DuplexMatrixLayout inverse () const = 0 ;
} ;

class DuplexMatrix implement DuplexMatrixLayout {
protected:
	using DuplexMatrixLayout::mDuplexMatrix ;

public:
	implicit DuplexMatrix () = default ;

	implicit DuplexMatrix (CR<Matrix> that) {
		DuplexMatrixHolder::hold (thiz)->initialize (that) ;
	}

	CR<Matrix> at (CR<INDEX> index) const leftvalue {
		return mDuplexMatrix[index] ;
	}

	forceinline CR<Matrix> operator[] (CR<INDEX> index) const leftvalue {
		return at (index) ;
	}

	DuplexMatrix inverse () const {
		DuplexMatrixLayout ret = DuplexMatrixHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<DuplexMatrix>::expr] (ret)) ;
	}
} ;

struct EulerAngle {
	Just<ViewMatrixOption> mType ;
	FLT64 mPitch ;
	FLT64 mYaw ;
	FLT64 mRoll ;
} ;

struct QuaternionLayout {
	Buffer<FLT64 ,RANK4> mQuaternion ;
} ;

struct QuaternionHolder implement Interface {
	imports VFat<QuaternionHolder> hold (VR<QuaternionLayout> that) ;
	imports CFat<QuaternionHolder> hold (CR<QuaternionLayout> that) ;

	virtual void initialize (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z ,CR<FLT64> w) = 0 ;
	virtual void initialize (CR<Vector> that) = 0 ;
	virtual void initialize (CR<Matrix> that) = 0 ;
	virtual void initialize (CR<EulerAngle> that) = 0 ;
	virtual CR<FLT64> at (CR<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CR<QuaternionLayout> that) const = 0 ;
	virtual FLAG compr (CR<QuaternionLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual QuaternionLayout smul (CR<QuaternionLayout> that) const = 0 ;
	virtual Vector vector () const = 0 ;
	virtual Matrix matrix () const = 0 ;
	virtual EulerAngle euler (CR<Just<ViewMatrixOption>> type) const = 0 ;
} ;

class Quaternion implement QuaternionLayout {
protected:
	using QuaternionLayout::mQuaternion ;

public:
	implicit Quaternion () = default ;

	explicit Quaternion (CR<FLT64> x ,CR<FLT64> y ,CR<FLT64> z ,CR<FLT64> w) {
		QuaternionHolder::hold (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Quaternion (CR<Vector> that) {
		QuaternionHolder::hold (thiz)->initialize (that) ;
	}

	explicit Quaternion (CR<Matrix> that) {
		QuaternionHolder::hold (thiz)->initialize (that) ;
	}

	explicit Quaternion (CR<EulerAngle> that) {
		QuaternionHolder::hold (thiz)->initialize (that) ;
	}

	static CR<Quaternion> iden () {
		return memorize ([&] () {
			return Quaternion (0 ,0 ,0 ,1) ;
		}) ;
	}

	CR<FLT64> at (CR<INDEX> y) const leftvalue {
		return QuaternionHolder::hold (thiz)->at (y) ;
	}

	forceinline CR<FLT64> operator[] (CR<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CR<Quaternion> that) const {
		return QuaternionHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Quaternion> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Quaternion> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Quaternion> that) const {
		return QuaternionHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Quaternion> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Quaternion> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Quaternion> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Quaternion> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return QuaternionHolder::hold (thiz)->visit (visitor) ;
	}

	Quaternion smul (CR<Quaternion> that) const {
		QuaternionLayout ret = QuaternionHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Quaternion>::expr] (ret)) ;
	}

	forceinline Quaternion operator* (CR<Quaternion> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CR<Quaternion> that) {
		thiz = smul (that) ;
	}

	Vector vector () const {
		return QuaternionHolder::hold (thiz)->vector () ;
	}

	Matrix matrix () const {
		return QuaternionHolder::hold (thiz)->matrix () ;
	}

	EulerAngle euler (CR<Just<ViewMatrixOption>> type) const {
		return QuaternionHolder::hold (thiz)->euler (type) ;
	}
} ;

struct LinearProcLayout ;

struct LinearProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<LinearProcLayout>>> expr_m () ;
	imports VFat<LinearProcHolder> hold (VR<LinearProcLayout> that) ;
	imports CFat<LinearProcHolder> hold (CR<LinearProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Image<FLT64> solve_lsm (CR<Image<FLT64>> a) const = 0 ;
	virtual Image<FLT64> solve_lsm (CR<Image<FLT64>> a ,CR<Image<FLT64>> b) const = 0 ;
	virtual Image<FLT64> solve_inv (CR<Image<FLT64>> a) const = 0 ;
} ;

class LinearProc implement OfThis<UniqueRef<LinearProcLayout>> {
public:
	static CR<LinearProc> expr_m () {
		return keep[TYPE<LinearProc>::expr] (LinearProcHolder::expr) ;
	}

	static Image<FLT64> solve_lsm (CR<Image<FLT64>> a) {
		return LinearProcHolder::hold (expr)->solve_lsm (a) ;
	}

	static Image<FLT64> solve_lsm (CR<Image<FLT64>> a ,CR<Image<FLT64>> b) {
		return LinearProcHolder::hold (expr)->solve_lsm (a ,b) ;
	}

	static Image<FLT64> solve_inv (CR<Image<FLT64>> a) {
		return LinearProcHolder::hold (expr)->solve_inv (a) ;
	}
} ;

struct PointCloudKDTreeLayout ;

struct PointCloudKDTreeHolder implement Interface {
	imports AutoRef<PointCloudKDTreeLayout> create () ;
	imports VFat<PointCloudKDTreeHolder> hold (VR<PointCloudKDTreeLayout> that) ;
	imports CFat<PointCloudKDTreeHolder> hold (CR<PointCloudKDTreeLayout> that) ;

	virtual void initialize (CR<Array<Pointer>> pointcloud) = 0 ;
	virtual Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor) const = 0 ;
	virtual Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor ,CR<FLT64> radius) const = 0 ;
} ;

class PointCloudKDTree implement OfThis<AutoRef<PointCloudKDTreeLayout>> {} ;

struct PointCloudLayout {
	LENGTH mRank ;
	Ref<Array<Pointer>> mPointCloud ;
	Matrix mWorld ;
	PointCloudKDTree mKDTree ;
} ;

struct PointCloudHolder implement Interface {
	imports VFat<PointCloudHolder> hold (VR<PointCloudLayout> that) ;
	imports CFat<PointCloudHolder> hold (CR<PointCloudLayout> that) ;

	virtual void initialize (RR<Ref<Array<Point2F>>> pointcloud) = 0 ;
	virtual void initialize (RR<Ref<Array<Point3F>>> pointcloud) = 0 ;
	virtual void initialize (RR<Ref<Array<Vector>>> pointcloud) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void get (CR<INDEX> index ,VR<Vector> item) const = 0 ;
	virtual Vector center () const = 0 ;
	virtual Matrix pca_matrix () const = 0 ;
	virtual Matrix box_matrix (CR<FLT64> ax ,CR<FLT64> ay ,CR<FLT64> az) const = 0 ;
	virtual Line3F bound () const = 0 ;
	virtual PointCloudLayout smul (CR<Matrix> that) const = 0 ;
	virtual Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor) const = 0 ;
	virtual Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor ,CR<FLT64> radius) const = 0 ;
} ;

class PointCloud implement PointCloudLayout {
protected:
	using PointCloudLayout::mRank ;
	using PointCloudLayout::mPointCloud ;
	using PointCloudLayout::mWorld ;
	using PointCloudLayout::mKDTree ;

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

	LENGTH size () const {
		return PointCloudHolder::hold (thiz)->size () ;
	}

	void get (CR<INDEX> index ,VR<Vector> item) const {
		return PointCloudHolder::hold (thiz)->get (index ,item) ;
	}

	forceinline Vector operator[] (CR<INDEX> index) const {
		Vector ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	Vector center () const {
		return PointCloudHolder::hold (thiz)->center () ;
	}

	Matrix pca_matrix () const {
		return PointCloudHolder::hold (thiz)->pca_matrix () ;
	}

	Matrix box_matrix (CR<FLT64> ax ,CR<FLT64> ay ,CR<FLT64> az) const {
		return PointCloudHolder::hold (thiz)->box_matrix (ax ,ay ,az) ;
	}

	Line3F bound () const {
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

	Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor) const {
		return PointCloudHolder::hold (thiz)->search (center ,neighbor) ;
	}

	Array<INDEX> search (CR<Vector> center ,CR<LENGTH> neighbor ,CR<FLT64> radius) const {
		return PointCloudHolder::hold (thiz)->search (center ,neighbor ,radius) ;
	}
} ;

struct TPSFitLayout {
	DuplexMatrix mNSrc ;
	DuplexMatrix mNDst ;
	Array<Vector> mPSrc ;
	Image<FLT64> mQA ;
	Image<FLT64> mQB ;
	Image<FLT64> mQC ;
} ;

struct TPSFitHolder implement Interface {
	imports VFat<TPSFitHolder> hold (VR<TPSFitLayout> that) ;
	imports CFat<TPSFitHolder> hold (CR<TPSFitLayout> that) ;

	virtual void compute (CR<Array<Vector>> dst ,CR<Array<Vector>> src) = 0 ;
	virtual Vector smul (CR<Vector> that) const = 0 ;
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

	void compute (CR<Array<Vector>> dst ,CR<Array<Vector>> src) {
		TPSFitHolder::hold (thiz)->compute (dst ,src) ;
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
} ;
} ;