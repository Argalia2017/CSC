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
class XYZProxy implement RefProxy<A> {
protected:
	using RefProxy<A>::mThat ;

public:
	explicit XYZProxy (XREF<A> that) :RefProxy<A> (that) {}

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

struct VectorLayout {
	Buffer<FLT64 ,RANK4> mVector ;
} ;

struct MatrixLayout {
	Buffer<FLT64 ,ENUM<16>> mMatrix ;
} ;

struct QuaternionLayout {
	Buffer<FLT64 ,RANK4> mQuaternion ;
} ;

struct VectorHolder implement Interface {
	imports VFat<VectorHolder> create (VREF<VectorLayout> that) ;
	imports CFat<VectorHolder> create (CREF<VectorLayout> that) ;

	virtual void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void initialize (CREF<Pixel> point) = 0 ;
	virtual void initialize (CREF<Point2F> point) = 0 ;
	virtual void initialize (CREF<Point3F> point) = 0 ;
	virtual VREF<FLT64> at (CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<VectorLayout> that) const = 0 ;
	virtual FLAG compr (CREF<VectorLayout> that) const = 0 ;
	virtual void visit (VREF<Visitor> visitor) const = 0 ;
	virtual VectorLayout add (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout sub (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout mul (CREF<FLT64> scale) const = 0 ;
	virtual VectorLayout div (CREF<FLT64> scale) const = 0 ;
	virtual FLT64 dot (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout mul (CREF<MatrixLayout> that) const = 0 ;
	virtual VectorLayout cross (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout plus () const = 0 ;
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

	explicit Vector (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
		VectorHolder::create (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Vector (CREF<Pixel> point) {
		VectorHolder::create (thiz)->initialize (point) ;
	}

	explicit Vector (CREF<Point2F> point) {
		VectorHolder::create (thiz)->initialize (point) ;
	}

	explicit Vector (CREF<Point3F> point) {
		VectorHolder::create (thiz)->initialize (point) ;
	}

	imports CREF<Vector> zero () {
		return memorize ([&] () {
			return Vector (0 ,0 ,0 ,0) ;
		}) ;
	}

	imports CREF<Vector> axis_x () {
		return memorize ([&] () {
			return Vector (1 ,0 ,0 ,0) ;
		}) ;
	}

	imports CREF<Vector> axis_y () {
		return memorize ([&] () {
			return Vector (0 ,1 ,0 ,0) ;
		}) ;
	}

	imports CREF<Vector> axis_z () {
		return memorize ([&] () {
			return Vector (0 ,0 ,1 ,0) ;
		}) ;
	}

	imports CREF<Vector> axis_w () {
		return memorize ([&] () {
			return Vector (0 ,0 ,0 ,1) ;
		}) ;
	}

	XYZProxy<CREF<Vector>> xyz () const leftvalue {
		return XYZProxy<CREF<Vector>> (thiz) ;
	}

	VREF<FLT64> at (CREF<INDEX> y) leftvalue {
		return VectorHolder::create (thiz)->at (y) ;
	}

	forceinline VREF<FLT64> operator[] (CREF<INDEX> y) leftvalue {
		return at (y) ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue {
		return VectorHolder::create (thiz)->at (y) ;
	}

	forceinline CREF<FLT64> operator[] (CREF<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CREF<Vector> that) const {
		return VectorHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Vector> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Vector> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Vector> that) const {
		return VectorHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Vector> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Vector> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Vector> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Vector> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<Visitor> visitor) const {
		return VectorHolder::create (thiz)->visit (visitor) ;
	}

	Vector add (CREF<Vector> that) const {
		VectorLayout ret = VectorHolder::create (thiz)->add (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator+ (CREF<Vector> that) const {
		return add (that) ;
	}

	forceinline void operator+= (CREF<Vector> that) {
		thiz = add (that) ;
	}

	Vector sub (CREF<Vector> that) const {
		VectorLayout ret = VectorHolder::create (thiz)->sub (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator- (CREF<Vector> that) const {
		return sub (that) ;
	}

	forceinline void operator-= (CREF<Vector> that) {
		thiz = sub (that) ;
	}

	Vector mul (CREF<FLT64> scale) const {
		VectorLayout ret = VectorHolder::create (thiz)->mul (scale) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CREF<FLT64> scale) const {
		return mul (scale) ;
	}

	forceinline friend Vector operator* (CREF<FLT64> scale ,CREF<Vector> that) {
		return that.mul (scale) ;
	}

	forceinline void operator*= (CREF<FLT64> scale) {
		thiz = mul (scale) ;
	}

	Vector div (CREF<FLT64> scale) const {
		VectorLayout ret = VectorHolder::create (thiz)->div (scale) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator/ (CREF<FLT64> scale) const {
		return div (scale) ;
	}

	forceinline void operator/= (CREF<FLT64> scale) {
		thiz = div (scale) ;
	}

	FLT64 dot (CREF<Vector> that) const {
		return VectorHolder::create (thiz)->dot (that) ;
	}

	forceinline FLT64 operator* (CREF<Vector> that) const {
		return dot (that) ;
	}

	Vector mul (CREF<MatrixLayout> that) const {
		VectorLayout ret = VectorHolder::create (thiz)->mul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CREF<MatrixLayout> that) const {
		return mul (that) ;
	}

	Vector cross (CREF<Vector> that) const {
		VectorLayout ret = VectorHolder::create (thiz)->cross (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator^ (CREF<Vector> that) const {
		return cross (that) ;
	}

	forceinline void operator^= (CREF<Vector> that) {
		thiz = cross (that) ;
	}

	Vector plus () const {
		VectorLayout ret = VectorHolder::create (thiz)->plus () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator+ () const {
		return plus () ;
	}

	Vector minus () const {
		VectorLayout ret = VectorHolder::create (thiz)->minus () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator- () const {
		return minus () ;
	}

	FLT64 magnitude () const {
		return VectorHolder::create (thiz)->magnitude () ;
	}

	Vector normalize () const {
		VectorLayout ret = VectorHolder::create (thiz)->normalize () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector projection () const {
		VectorLayout ret = VectorHolder::create (thiz)->projection () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Vector homogenize () const {
		VectorLayout ret = VectorHolder::create (thiz)->homogenize () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}
} ;

struct MatrixHolder implement Interface {
	imports VFat<MatrixHolder> create (VREF<MatrixLayout> that) ;
	imports CFat<MatrixHolder> create (CREF<MatrixLayout> that) ;

	virtual void initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z ,CREF<VectorLayout> w) = 0 ;
	virtual VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<MatrixLayout> that) const = 0 ;
	virtual FLAG compr (CREF<MatrixLayout> that) const = 0 ;
	virtual void visit (VREF<Visitor> visitor) const = 0 ;
	virtual MatrixLayout add (CREF<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout sub (CREF<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout mul (CREF<FLT64> scale) const = 0 ;
	virtual MatrixLayout div (CREF<FLT64> scale) const = 0 ;
	virtual VectorLayout mul (CREF<VectorLayout> that) const = 0 ;
	virtual MatrixLayout mul (CREF<MatrixLayout> that) const = 0 ;
	virtual MatrixLayout plus () const = 0 ;
	virtual MatrixLayout minus () const = 0 ;
	virtual MatrixLayout transpose () const = 0 ;
	virtual MatrixLayout triangular () const = 0 ;
	virtual MatrixLayout homogenize () const = 0 ;
	virtual FLT64 det () const = 0 ;
	virtual MatrixLayout adjoint () const = 0 ;
	virtual MatrixLayout inverse () const = 0 ;
	virtual FLT64 trace () const = 0 ;
} ;

class Matrix implement MatrixLayout {
protected:
	using MatrixLayout::mMatrix ;

public:
	implicit Matrix () = default ;

	explicit Matrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z ,CREF<Vector> w) {
		MatrixHolder::create (thiz)->initialize (x ,y ,z ,w) ;
	}

	imports CREF<Matrix> zero () {
		return memorize ([&] () {
			const auto r1x = Vector::zero () ;
			return Matrix (r1x ,r1x ,r1x ,r1x) ;
		}) ;
	}

	imports CREF<Matrix> identity () {
		return memorize ([&] () {
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}) ;
	}

	imports CREF<Matrix> axis_w () {
		return memorize ([&] () {
			const auto r1x = Vector::zero () ;
			const auto r2x = Vector::axis_w () ;
			return Matrix (r1x ,r1x ,r1x ,r2x) ;
		}) ;
	}

	VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
		return MatrixHolder::create (thiz)->at (x ,y) ;
	}

	VREF<FLT64> at (CREF<Pixel> index) leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline VREF<FLT64> operator[] (CREF<Pixel> index) leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<VREF<Matrix>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VREF<Matrix>> (thiz ,y) ;
	}

	CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return MatrixHolder::create (thiz)->at (x ,y) ;
	}

	CREF<FLT64> at (CREF<Pixel> index) const leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline CREF<FLT64> operator[] (CREF<Pixel> index) const leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<CREF<Matrix>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CREF<Matrix>> (thiz ,y) ;
	}

	BOOL equal (CREF<Matrix> that) const {
		return MatrixHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Matrix> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Matrix> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Matrix> that) const {
		return MatrixHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Matrix> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Matrix> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Matrix> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Matrix> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<Visitor> visitor) const {
		return MatrixHolder::create (thiz)->visit (visitor) ;
	}

	Matrix add (CREF<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::create (thiz)->add (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator+ (CREF<Matrix> that) const {
		return add (that) ;
	}

	forceinline void operator+= (CREF<Matrix> that) {
		thiz = add (that) ;
	}

	Matrix sub (CREF<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::create (thiz)->sub (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator- (CREF<Matrix> that) const {
		return sub (that) ;
	}

	forceinline void operator-= (CREF<Matrix> that) {
		thiz = sub (that) ;
	}

	Matrix mul (CREF<FLT64> scale) const {
		MatrixLayout ret = MatrixHolder::create (thiz)->mul (scale) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator* (CREF<FLT64> scale) const {
		return mul (scale) ;
	}

	forceinline friend Matrix operator* (CREF<FLT64> scale ,CREF<Matrix> that) {
		return that.mul (scale) ;
	}

	forceinline void operator*= (CREF<FLT64> scale) {
		thiz = mul (scale) ;
	}

	Matrix div (CREF<FLT64> scale) const {
		MatrixLayout ret = MatrixHolder::create (thiz)->div (scale) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator/ (CREF<FLT64> scale) const {
		return div (scale) ;
	}

	forceinline void operator/= (CREF<FLT64> scale) {
		thiz = div (scale) ;
	}

	Vector mul (CREF<Vector> that) const {
		VectorLayout ret = MatrixHolder::create (thiz)->mul (that) ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	forceinline Vector operator* (CREF<Vector> that) const {
		return mul (that) ;
	}

	Matrix mul (CREF<Matrix> that) const {
		MatrixLayout ret = MatrixHolder::create (thiz)->mul (that) ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator* (CREF<Matrix> that) const {
		return mul (that) ;
	}

	forceinline void operator*= (CREF<Matrix> that) {
		thiz = mul (that) ;
	}

	Matrix plus () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->plus () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator+ () const {
		return plus () ;
	}

	Matrix minus () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->minus () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	forceinline Matrix operator- () const {
		return minus () ;
	}

	Matrix transpose () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->transpose () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix triangular () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->triangular () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix homogenize () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->homogenize () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 det () const {
		return MatrixHolder::create (thiz)->det () ;
	}

	Matrix adjoint () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->adjoint () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	Matrix inverse () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->inverse () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 trace () const {
		return MatrixHolder::create (thiz)->trace () ;
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
	imports VFat<MakeMatrixHolder> create (VREF<MatrixLayout> that) ;
	imports CFat<MakeMatrixHolder> create (CREF<MatrixLayout> that) ;

	virtual void DiagMatrix_initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void ShearMatrix_initialize (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) = 0 ;
	virtual void RotationMatrix_initialize (CREF<Vector> normal ,CREF<FLT64> angle) = 0 ;
	virtual void TranslationMatrix_initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) = 0 ;
	virtual void PerspectiveMatrix_initialize (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) = 0 ;
	virtual void ProjectionMatrix_initialize (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) = 0 ;
	virtual void ViewMatrix_initialize (CREF<Vector> vx ,CREF<Vector> vy) = 0 ;
	virtual void ViewMatrix_initialize (CREF<Vector> vx ,CREF<Vector> vy ,CREF<JustInt<ViewMatrixOption>> option) = 0 ;
	virtual void CrossProductMatrix_initialize (CREF<Vector> xyz) = 0 ;
	virtual void SymmetryMatrix_initialize (CREF<Vector> x ,CREF<Vector> y) = 0 ;
	virtual void AffineMatrix_initialize (CREF<Buffer<FLT64 ,ENUM<16>>> a) = 0 ;
} ;

inline Matrix DiagMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->DiagMatrix_initialize (x ,y ,z ,1) ;
	return move (ret) ;
}

inline Matrix DiagMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->DiagMatrix_initialize (x ,y ,z ,w) ;
	return move (ret) ;
}

inline Matrix ShearMatrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ShearMatrix_initialize (x ,y ,z) ;
	return move (ret) ;
}

inline Matrix RotationMatrix (CREF<Vector> normal ,CREF<FLT64> angle) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->RotationMatrix_initialize (normal ,angle) ;
	return move (ret) ;
}

inline Matrix TranslationMatrix (CREF<Vector> xyz) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->TranslationMatrix_initialize (xyz[0] ,xyz[1] ,xyz[2]) ;
	return move (ret) ;
}

inline Matrix TranslationMatrix (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->TranslationMatrix_initialize (x ,y ,z) ;
	return move (ret) ;
}

inline Matrix PerspectiveMatrix (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->PerspectiveMatrix_initialize (fx ,fy ,wx ,wy) ;
	return move (ret) ;
}

inline Matrix ProjectionMatrix (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ProjectionMatrix_initialize (normal ,center ,light) ;
	return move (ret) ;
}

inline Matrix ViewMatrixXYZ (CREF<Vector> x ,CREF<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ViewMatrix_initialize (x ,y ,ViewMatrixOption::XYZ) ;
	return move (ret) ;
}

inline Matrix ViewMatrixXZY (CREF<Vector> x ,CREF<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ViewMatrix_initialize (x ,z ,ViewMatrixOption::XZY) ;
	return move (ret) ;
}

inline Matrix ViewMatrixYXZ (CREF<Vector> y ,CREF<Vector> x) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ViewMatrix_initialize (y ,x ,ViewMatrixOption::YXZ) ;
	return move (ret) ;
}

inline Matrix ViewMatrixYZX (CREF<Vector> y ,CREF<Vector> z) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ViewMatrix_initialize (y ,z ,ViewMatrixOption::YZX) ;
	return move (ret) ;
}

inline Matrix ViewMatrixZXY (CREF<Vector> z ,CREF<Vector> x) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ViewMatrix_initialize (z ,x ,ViewMatrixOption::ZXY) ;
	return move (ret) ;
}

inline Matrix ViewMatrixZYX (CREF<Vector> z ,CREF<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->ViewMatrix_initialize (z ,y ,ViewMatrixOption::ZYX) ;
	return move (ret) ;
}

inline Matrix CrossProductMatrix (CREF<Vector> xyz) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->CrossProductMatrix_initialize (xyz) ;
	return move (ret) ;
}

inline Matrix SymmetryMatrix (CREF<Vector> x ,CREF<Vector> y) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->SymmetryMatrix_initialize (x ,y) ;
	return move (ret) ;
}

inline Matrix AffineMatrix (CREF<Buffer<FLT64 ,ENUM<16>>> a) {
	Matrix ret ;
	MakeMatrixHolder::create (ret)->AffineMatrix_initialize (a) ;
	return move (ret) ;
}

struct DuplexMatrixLayout {
	Buffer2<Matrix> mBiMatrix ;
} ;

class DuplexMatrix implement DuplexMatrixLayout {
protected:
	using DuplexMatrixLayout::mBiMatrix ;

public:
	implicit DuplexMatrix () = default ;

	implicit DuplexMatrix (CREF<Matrix> that) {
		mBiMatrix[0] = that ;
		mBiMatrix[1] = mBiMatrix[0].inverse () ;
		if ifdo (TRUE) {
			if (mBiMatrix[0][3][0] != 0)
				discard ;
			if (mBiMatrix[0][3][1] != 0)
				discard ;
			if (mBiMatrix[0][3][2] != 0)
				discard ;
			if (mBiMatrix[0][3][3] != 1)
				discard ;
			mBiMatrix[1][3][3] = 1 ;
		}
	}

	CREF<Matrix> at (CREF<INDEX> index) const leftvalue {
		return mBiMatrix[index] ;
	}

	forceinline CREF<Matrix> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	DuplexMatrix inverse () const {
		DuplexMatrix ret ;
		ret.mBiMatrix[0] = mBiMatrix[1] ;
		ret.mBiMatrix[1] = mBiMatrix[0] ;
		return move (ret) ;
	}
} ;

struct TRSResult {
	Matrix mR ;
	Matrix mT ;
	Matrix mS ;
} ;

struct KRTResult {
	Matrix mK ;
	Matrix mR ;
	Matrix mT ;
} ;

struct SVDResult {
	Matrix mU ;
	Matrix mS ;
	Matrix mV ;
} ;

struct MatrixProcLayout {
	RefLayout mThis ;
} ;

struct MatrixProcHolder implement Interface {
	imports VFat<MatrixProcHolder> create (VREF<MatrixProcLayout> that) ;
	imports CFat<MatrixProcHolder> create (CREF<MatrixProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual TRSResult solve_trs (CREF<Matrix> a) const = 0 ;
	virtual KRTResult solve_krt (CREF<Matrix> a) const = 0 ;
	virtual SVDResult solve_svd (CREF<Matrix> a) const = 0 ;
} ;

class MatrixProc implement MatrixProcLayout {
protected:
	using MatrixProcLayout::mThis ;

public:
	imports CREF<MatrixProc> instance () {
		return memorize ([&] () {
			MatrixProc ret ;
			MatrixProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports TRSResult solve_trs (CREF<Matrix> a) {
		return MatrixProcHolder::create (instance ())->solve_trs (a) ;
	}

	imports KRTResult solve_krt (CREF<Matrix> a) {
		return MatrixProcHolder::create (instance ())->solve_krt (a) ;
	}

	imports SVDResult solve_svd (CREF<Matrix> a) {
		return MatrixProcHolder::create (instance ())->solve_svd (a) ;
	}
} ;

struct LinearProcLayout {
	RefLayout mThis ;
} ;

struct LinearProcHolder implement Interface {
	imports VFat<LinearProcHolder> create (VREF<LinearProcLayout> that) ;
	imports CFat<LinearProcHolder> create (CREF<LinearProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Image<FLT64> solve_lsm (CREF<Image<FLT64>> a) const = 0 ;
	virtual Image<FLT64> solve_lsm (CREF<Image<FLT64>> a ,CREF<Image<FLT64>> b) const = 0 ;
} ;

class LinearProc implement LinearProcLayout {
protected:
	using LinearProcLayout::mThis ;

public:
	imports CREF<LinearProc> instance () {
		return memorize ([&] () {
			LinearProc ret ;
			LinearProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports Image<FLT64> solve_lsm (CREF<Image<FLT64>> a) {
		return LinearProcHolder::create (instance ())->solve_lsm (a) ;
	}

	imports Image<FLT64> solve_lsm (CREF<Image<FLT64>> a ,CREF<Image<FLT64>> b) {
		return LinearProcHolder::create (instance ())->solve_lsm (a ,b) ;
	}
} ;

struct QuaternionHolder implement Interface {
	imports VFat<QuaternionHolder> create (VREF<QuaternionLayout> that) ;
	imports CFat<QuaternionHolder> create (CREF<QuaternionLayout> that) ;

	virtual void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void initialize (CREF<VectorLayout> that) = 0 ;
	virtual void initialize (CREF<MatrixLayout> that) = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<QuaternionLayout> that) const = 0 ;
	virtual FLAG compr (CREF<QuaternionLayout> that) const = 0 ;
	virtual void visit (VREF<Visitor> visitor) const = 0 ;
	virtual QuaternionLayout mul (CREF<QuaternionLayout> that) const = 0 ;
	virtual VectorLayout vector () const = 0 ;
	virtual MatrixLayout matrix () const = 0 ;
} ;

class Quaternion implement QuaternionLayout {
protected:
	using QuaternionLayout::mQuaternion ;

public:
	implicit Quaternion () = default ;

	explicit Quaternion (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
		QuaternionHolder::create (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Quaternion (CREF<Vector> that) {
		QuaternionHolder::create (thiz)->initialize (that) ;
	}

	explicit Quaternion (CREF<Matrix> that) {
		QuaternionHolder::create (thiz)->initialize (that) ;
	}

	imports CREF<Quaternion> identity () {
		return memorize ([&] () {
			return Quaternion (0 ,0 ,0 ,1) ;
		}) ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue {
		return QuaternionHolder::create (thiz)->at (y) ;
	}

	forceinline CREF<FLT64> operator[] (CREF<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CREF<Quaternion> that) const {
		return QuaternionHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Quaternion> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Quaternion> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CREF<Quaternion> that) const {
		return QuaternionHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Quaternion> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Quaternion> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Quaternion> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Quaternion> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VREF<Visitor> visitor) const {
		return QuaternionHolder::create (thiz)->visit (visitor) ;
	}

	Quaternion mul (CREF<Quaternion> that) const {
		QuaternionLayout ret = QuaternionHolder::create (thiz)->mul (that) ;
		return move (keep[TYPE<Quaternion>::expr] (ret)) ;
	}

	forceinline Quaternion operator* (CREF<Quaternion> that) const {
		return mul (that) ;
	}

	forceinline void operator*= (CREF<Quaternion> that) {
		thiz = mul (that) ;
	}

	Vector vector () const {
		VectorLayout ret = QuaternionHolder::create (thiz)->vector () ;
		return move (keep[TYPE<Vector>::expr] (ret)) ;
	}

	Matrix matrix () const {
		MatrixLayout ret = QuaternionHolder::create (thiz)->matrix () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}
} ;

struct PointCloudLayout {
	LENGTH mRank ;
	Array<Pointer> mPointCloud ;
} ;

struct PointCloudHolder implement Interface {
	imports VFat<PointCloudHolder> create (VREF<PointCloudLayout> that) ;
	imports CFat<PointCloudHolder> create (CREF<PointCloudLayout> that) ;

	virtual void initialize (RREF<Array<Point2F>> that) = 0 ;
	virtual void initialize (RREF<Array<Point3F>> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<Vector> item) const = 0 ;
	virtual Vector center () const = 0 ;
	virtual Matrix svd_matrix () const = 0 ;
	virtual Matrix box_matrix () const = 0 ;
	virtual Line3F bound () const = 0 ;
	virtual PointCloudLayout mul (CREF<Matrix> mat) const = 0 ;
	virtual Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const = 0 ;
	virtual Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const = 0 ;
} ;

class PointCloud implement PointCloudLayout {
protected:
	using PointCloudLayout::mRank ;
	using PointCloudLayout::mPointCloud ;

public:
	implicit PointCloud () = default ;

	explicit PointCloud (CREF<Array<Point2F>> that) {
		PointCloudHolder::create (thiz)->initialize (move (that)) ;
	}

	explicit PointCloud (RREF<Array<Point2F>> that) {
		PointCloudHolder::create (thiz)->initialize (move (that)) ;
	}

	explicit PointCloud (CREF<Array<Point3F>> that) {
		PointCloudHolder::create (thiz)->initialize (move (that)) ;
	}

	explicit PointCloud (RREF<Array<Point3F>> that) {
		PointCloudHolder::create (thiz)->initialize (move (that)) ;
	}

	LENGTH size () const {
		return PointCloudHolder::create (thiz)->size () ;
	}

	Vector at (CREF<INDEX> index) const {
		Vector ret ;
		PointCloudHolder::create (thiz)->get (index ,ret) ;
		return move (ret) ;
	}

	forceinline Vector operator[] (CREF<INDEX> index) const {
		return at (index) ;
	}

	Vector center () const {
		return PointCloudHolder::create (thiz)->center () ;
	}

	Matrix svd_matrix () const {
		return PointCloudHolder::create (thiz)->svd_matrix () ;
	}

	Matrix box_matrix () const {
		return PointCloudHolder::create (thiz)->box_matrix () ;
	}

	Line3F bound () const {
		return PointCloudHolder::create (thiz)->bound () ;
	}

	PointCloud mul (CREF<Matrix> mat) const {
		PointCloudLayout ret = PointCloudHolder::create (thiz)->mul (mat) ;
		return move (keep[TYPE<PointCloud>::expr] (ret)) ;
	}

	forceinline PointCloud operator* (CREF<Matrix> mat) const {
		return mul (mat) ;
	}

	forceinline friend PointCloud operator* (CREF<Matrix> mat ,CREF<PointCloud> that) {
		return that.mul (mat.transpose ()) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor) const {
		return PointCloudHolder::create (thiz)->search (center ,neighbor) ;
	}

	Array<INDEX> search (CREF<Vector> center ,CREF<LENGTH> neighbor ,CREF<FLT64> radius) const {
		return PointCloudHolder::create (thiz)->search (center ,neighbor ,radius) ;
	}
} ;
} ;