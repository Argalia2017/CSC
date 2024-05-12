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

struct Rect2F {
	Point2F mMin ;
	Point2F mMax ;
} ;

struct Rect3F {
	Point3F mMin ;
	Point3F mMax ;
} ;

struct VectorLayout {
	BoxBuffer<FLT64 ,RANK4> mVector ;
} ;

struct MatrixLayout {
	BoxBuffer<FLT64 ,ENUM<16>> mMatrix ;
} ;

struct QuaternionLayout {
	BoxBuffer<FLT64 ,RANK4> mQuaternion ;
} ;

struct VectorHolder implement Interface {
	imports VFat<VectorHolder> create (VREF<VectorLayout> that) ;
	imports CFat<VectorHolder> create (CREF<VectorLayout> that) ;

	virtual void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void initialize (CREF<Pixel> point) = 0 ;
	virtual void initialize (CREF<Point2F> point) = 0 ;
	virtual void initialize (CREF<Point3F> point) = 0 ;
	virtual Point2F xy () const = 0 ;
	virtual Point3F xyz () const = 0 ;
	virtual VREF<FLT64> at (CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<VectorLayout> that) const = 0 ;
	virtual FLAG compr (CREF<VectorLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
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

	Point2F xy () const {
		return VectorHolder::create (thiz)->xy () ;
	}

	Point3F xyz () const {
		return VectorHolder::create (thiz)->xyz () ;
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
		return ifnot (equal (that)) ;
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

	void visit (CREF<Visitor> visitor) const {
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
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
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
	virtual MatrixLayout inverse () const = 0 ;
	virtual FLT64 trace () const = 0 ;
} ;

class Matrix implement MatrixLayout {
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

	forceinline RowProxy<VPTR<Matrix>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VPTR<Matrix>> (thiz ,y) ;
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

	forceinline RowProxy<CPTR<Matrix>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CPTR<Matrix>> (thiz ,y) ;
	}

	BOOL equal (CREF<Matrix> that) const {
		return MatrixHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Matrix> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Matrix> that) const {
		return ifnot (equal (that)) ;
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

	void visit (CREF<Visitor> visitor) const {
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

	Matrix inverse () const {
		MatrixLayout ret = MatrixHolder::create (thiz)->inverse () ;
		return move (keep[TYPE<Matrix>::expr] (ret)) ;
	}

	FLT64 trace () const {
		return MatrixHolder::create (thiz)->trace () ;
	}
} ;

struct MakeMatrixHolder implement Interface {
	imports VFat<MakeMatrixHolder> create (VREF<MatrixLayout> that) ;
	imports CFat<MakeMatrixHolder> create (CREF<MatrixLayout> that) ;

	virtual void DiagMatrix_initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void ShearMatrix_initialize (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) = 0 ;
	virtual void RotationMatrix_initialize (CREF<Vector> normal ,CREF<FLT64> angle) = 0 ;
	virtual void TranslationMatrix_initialize (CREF<Vector> xyz) = 0 ;
	virtual void TranslationMatrix_initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) = 0 ;
	virtual void PerspectiveMatrix_initialize (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) = 0 ;
	virtual void ProjectionMatrix_initialize (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) = 0 ;
	virtual void ViewMatrix_initialize (CREF<Vector> vx ,CREF<Vector> vy) = 0 ;
	virtual void ViewMatrix_initialize (CREF<Vector> vx ,CREF<Vector> vy ,CREF<FLAG> flag) = 0 ;
	virtual void CrossProductMatrix_initialize (CREF<Vector> xyz) = 0 ;
	virtual void SymmetryMatrix_initialize (CREF<Vector> x ,CREF<Vector> y) = 0 ;
} ;

struct DiagMatrix implement Proxy {
	imports Matrix make (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->DiagMatrix_initialize (x ,y ,z ,w) ;
		return move (ret) ;
	}
} ;

struct ShearMatrix implement Proxy {
	imports Matrix make (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ShearMatrix_initialize (x ,y ,z) ;
		return move (ret) ;
	}
} ;

struct RotationMatrix implement Proxy {
	imports Matrix make (CREF<Vector> normal ,CREF<FLT64> angle) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->RotationMatrix_initialize (normal ,angle) ;
		return move (ret) ;
	}
} ;

struct TranslationMatrix implement Proxy {
	imports Matrix make (CREF<Vector> xyz) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->TranslationMatrix_initialize (xyz) ;
		return move (ret) ;
	}

	imports Matrix make (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->TranslationMatrix_initialize (x ,y ,z) ;
		return move (ret) ;
	}
} ;

struct PerspectiveMatrix implement Proxy {
	imports Matrix make (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->PerspectiveMatrix_initialize (fx ,fy ,wx ,wy) ;
		return move (ret) ;
	}
} ;

struct ProjectionMatrix implement Proxy {
	imports Matrix make (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ProjectionMatrix_initialize (normal ,center ,light) ;
		return move (ret) ;
	}
} ;

struct ViewMatrixOption {
	enum {
		XY ,
		XZ ,
		YX ,
		YZ ,
		ZX ,
		ZY ,
		ETC
	} ;
} ;

struct ViewMatrix implement Proxy {
	imports Matrix make_xy (CREF<Vector> x ,CREF<Vector> y) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ViewMatrix_initialize (x ,y ,ViewMatrixOption::XY) ;
		return move (ret) ;
	}

	imports Matrix make_xz (CREF<Vector> x ,CREF<Vector> z) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ViewMatrix_initialize (x ,z ,ViewMatrixOption::XZ) ;
		return move (ret) ;
	}

	imports Matrix make_yx (CREF<Vector> y ,CREF<Vector> x) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ViewMatrix_initialize (y ,x ,ViewMatrixOption::YX) ;
		return move (ret) ;
	}

	imports Matrix make_yz (CREF<Vector> y ,CREF<Vector> z) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ViewMatrix_initialize (y ,z ,ViewMatrixOption::YZ) ;
		return move (ret) ;
	}

	imports Matrix make_zx (CREF<Vector> z ,CREF<Vector> x) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ViewMatrix_initialize (z ,x ,ViewMatrixOption::ZX) ;
		return move (ret) ;
	}

	imports Matrix make_zy (CREF<Vector> z ,CREF<Vector> y) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->ViewMatrix_initialize (z ,y ,ViewMatrixOption::ZY) ;
		return move (ret) ;
	}
} ;

struct CrossProductMatrix implement Proxy {
	imports Matrix make (CREF<Vector> xyz) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->CrossProductMatrix_initialize (xyz) ;
		return move (ret) ;
	}
} ;

struct SymmetryMatrix implement Proxy {
	imports Matrix make (CREF<Vector> x ,CREF<Vector> y) {
		Matrix ret ;
		MakeMatrixHolder::create (ret)->SymmetryMatrix_initialize (x ,y) ;
		return move (ret) ;
	}
} ;

class BiMatrix {
protected:
	BoxBuffer<Matrix ,RANK2> mBiMatrix ;

public:
	implicit BiMatrix () = default ;

	implicit BiMatrix (CREF<Matrix> that) {
		mBiMatrix[0] = that ;
		mBiMatrix[1] = mBiMatrix[0].inverse () ;
	}

	CREF<Matrix> at (CREF<INDEX> index) const leftvalue {
		return mBiMatrix[index] ;
	}

	forceinline CREF<Matrix> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}
} ;

struct XTRResult {
	Matrix mX ;
	Matrix mT ;
	Matrix mR ;
} ;

struct SVDResult {
	Matrix mU ;
	Matrix mS ;
	Matrix mV ;
} ;

struct MatrixProcPureLayout ;

struct MatrixProcLayout {
	Ref<MatrixProcPureLayout> mThis ;
} ;

struct MatrixProcHolder implement Interface {
	imports VFat<MatrixProcHolder> create (VREF<MatrixProcLayout> that) ;
	imports CFat<MatrixProcHolder> create (CREF<MatrixProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual XTRResult solve_xtr (CREF<Matrix> a) const = 0 ;
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

	imports XTRResult solve_xtr (CREF<Matrix> a) {
		return MatrixProcHolder::create (instance ())->solve_xtr (a) ;
	}

	imports SVDResult solve_svd (CREF<Matrix> a) {
		return MatrixProcHolder::create (instance ())->solve_svd (a) ;
	}
} ;

struct QuaternionHolder implement Interface {
	imports VFat<QuaternionHolder> create (VREF<QuaternionLayout> that) ;
	imports CFat<QuaternionHolder> create (CREF<QuaternionLayout> that) ;

	virtual void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) = 0 ;
	virtual void initialize (CREF<MatrixLayout> that) = 0 ;
	virtual CREF<FLT64> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<QuaternionLayout> that) const = 0 ;
	virtual FLAG compr (CREF<QuaternionLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
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
		return ifnot (equal (that)) ;
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

	void visit (CREF<Visitor> visitor) const {
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
	ArrayLayout mPointCloud ;
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
	virtual Rect3F bound () const = 0 ;
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

	Rect3F bound () const {
		return PointCloudHolder::create (thiz)->bound () ;
	}
} ;
} ;