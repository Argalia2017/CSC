#pragma once

#ifndef __CSC_GEOMETRY__
#define __CSC_GEOMETRY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_image.hpp"

namespace CSC {
struct POINT2F {
	FLT32 mX ;
	FLT32 mY ;
} ;

struct POINT2D {
	FLT64 mX ;
	FLT64 mY ;
} ;

struct POINT3F {
	FLT32 mX ;
	FLT32 mY ;
	FLT32 mZ ;
} ;

struct POINT3D {
	FLT64 mX ;
	FLT64 mY ;
	FLT64 mZ ;
} ;

struct BBOX3F {
	POINT3F mMin ;
	POINT3F mMax ;
} ;

struct BBOX3D {
	POINT3D mMin ;
	POINT3D mMax ;
} ;

class VectorLayout {} ;

class MatrixLayout {} ;

class QuaternionLayout {} ;

template <class A>
struct VectorHolder implement Interface {
	imports VFat<VectorHolder> create (VREF<VectorLayout> that) ;
	imports CFat<VectorHolder> create (CREF<VectorLayout> that) ;

	virtual void initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) = 0 ;
	virtual void initialize (CREF<PIXEL> xy) = 0 ;
	virtual void initialize (CREF<POINT2F> xy) = 0 ;
	virtual void initialize (CREF<POINT3F> xyz) = 0 ;
	virtual CREF<VectorLayout> zero () const = 0 ;
	virtual CREF<VectorLayout> axis_x () const = 0 ;
	virtual CREF<VectorLayout> axis_y () const = 0 ;
	virtual CREF<VectorLayout> axis_z () const = 0 ;
	virtual CREF<VectorLayout> axis_w () const = 0 ;
	virtual POINT2F xy () const = 0 ;
	virtual POINT3F xyz () const = 0 ;
	virtual VREF<A> at (CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<A> at (CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<VectorLayout> that) const = 0 ;
	virtual FLAG compr (CREF<VectorLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual void add_with (CREF<VectorLayout> that) = 0 ;
	virtual VectorLayout sub (CREF<VectorLayout> that) const = 0 ;
	virtual void sub_with (CREF<VectorLayout> that) = 0 ;
	virtual VectorLayout mul (CREF<A> scale) const = 0 ;
	virtual void mul_with (CREF<A> scale) = 0 ;
	virtual A dot (CREF<VectorLayout> that) const = 0 ;
	virtual VectorLayout mul (CREF<MatrixLayout> that) const = 0 ;
	virtual VectorLayout cross (CREF<VectorLayout> that) const = 0 ;
	virtual void cross_with (CREF<VectorLayout> that) = 0 ;
	virtual VectorLayout minus () const = 0 ;
	virtual A magnitude () const = 0 ;
	virtual VectorLayout normalize () const = 0 ;
	virtual VectorLayout projection () const = 0 ;
	virtual VectorLayout homogenize () const = 0 ;
} ;

template <class>
class Matrix ;

template <class A>
class Vector implement VectorLayout {
protected:
	BoxBuffer<A ,RANK4> mVector ;

public:
	implicit Vector () = default ;

	implicit Vector (RREF<VectorLayout> that) :Vector (keep[TYPE<RREF<Vector>>::expr] (that)) {}

	explicit Vector (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) {
		VectorHolder<A>::create (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Vector (CREF<POINT2F> xy) {
		VectorHolder<A>::create (thiz)->initialize (xy) ;
	}

	explicit Vector (CREF<POINT3F> xyz) {
		VectorHolder<A>::create (thiz)->initialize (xyz) ;
	}

	imports CREF<Vector> zero () {
		return VectorHolder<A>::create (Pointer::make (0))->zero () ;
	}

	imports CREF<Vector> axis_x () {
		return VectorHolder<A>::create (Pointer::make (0))->axis_x () ;
	}

	imports CREF<Vector> axis_y () {
		return VectorHolder<A>::create (Pointer::make (0))->axis_y () ;
	}

	imports CREF<Vector> axis_z () {
		return VectorHolder<A>::create (Pointer::make (0))->axis_z () ;
	}

	imports CREF<Vector> axis_w () {
		return VectorHolder<A>::create (Pointer::make (0))->axis_w () ;
	}

	POINT2F xy () const {
		return VectorHolder<A>::create (thiz)->xy () ;
	}

	POINT3F xyz () const {
		return VectorHolder<A>::create (thiz)->xyz () ;
	}

	inline VREF<A> at (CREF<INDEX> y) leftvalue {
		return VectorHolder<A>::create (thiz)->at (y) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> y) leftvalue {
		return at (y) ;
	}

	inline CREF<A> at (CREF<INDEX> y) const leftvalue {
		return VectorHolder<A>::create (thiz)->at (y) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CREF<Vector> that) const {
		return VectorHolder<A>::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<Vector> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Vector> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Vector> that) const {
		return VectorHolder<A>::create (thiz)->compr (that) ;
	}

	inline BOOL operator< (CREF<Vector> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Vector> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Vector> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Vector> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return VectorHolder<A>::create (thiz)->visit (visitor) ;
	}

	Vector add (CREF<Vector> that) const {
		return VectorHolder<A>::create (thiz)->add (that) ;
	}

	inline Vector operator+ (CREF<Vector> that) const {
		return add (that) ;
	}

	void add_with (CREF<Vector> that) {
		return VectorHolder<A>::create (thiz)->add_with (that) ;
	}

	inline void operator+= (CREF<Vector> that) {
		add_with (that) ;
	}

	Vector sub (CREF<Vector> that) const {
		return VectorHolder<A>::create (thiz)->sub (that) ;
	}

	inline Vector operator- (CREF<Vector> that) const {
		return sub (that) ;
	}

	void sub_with (CREF<Vector> that) {
		return VectorHolder<A>::create (thiz)->sub_with (that) ;
	}

	inline void operator-= (CREF<Vector> that) {
		sub_with (that) ;
	}

	Vector mul (CREF<A> scale) const {
		return VectorHolder<A>::create (thiz)->mul (scale) ;
	}

	inline Vector operator* (CREF<A> scale) const {
		return mul (scale) ;
	}

	inline friend Vector operator* (CREF<A> scale ,CREF<Vector> that) {
		return that.mul (scale) ;
	}

	void mul_with (CREF<A> scale) {
		return VectorHolder<A>::create (thiz)->mul_with (scale) ;
	}

	inline void operator*= (CREF<A> scale) {
		mul_with (scale) ;
	}

	A dot (CREF<Vector> that) const {
		return VectorHolder<A>::create (thiz)->dot (that) ;
	}

	inline A operator* (CREF<Vector> that) const {
		return dot (that) ;
	}

	Vector mul (CREF<Matrix<A>> that) const {
		return VectorHolder<A>::create (thiz)->mul (that) ;
	}

	inline Vector operator* (CREF<Matrix<A>> that) const {
		return mul (that) ;
	}

	Vector cross (CREF<Vector> that) const {
		return VectorHolder<A>::create (thiz)->cross (that) ;
	}

	inline Vector operator^ (CREF<Vector> that) const {
		return cross (that) ;
	}

	inline void operator^= (CREF<Vector> that) {
		return VectorHolder<A>::create (thiz)->cross_with (that) ;
	}

	inline Vector operator+ () const {
		return thiz ;
	}

	Vector minus () const {
		return VectorHolder<A>::create (thiz)->minus () ;
	}

	inline Vector operator- () const {
		return minus () ;
	}

	A magnitude () const {
		return VectorHolder<A>::create (thiz)->magnitude () ;
	}

	Vector normalize () const {
		return VectorHolder<A>::create (thiz)->normalize () ;
	}

	Vector projection () const {
		return VectorHolder<A>::create (thiz)->projection () ;
	}

	Vector homogenize () const {
		return VectorHolder<A>::create (thiz)->homogenize () ;
	}
} ;

template <class A>
struct MatrixHolder implement Interface {
	imports VFat<MatrixHolder> create (VREF<MatrixLayout> that) ;
	imports CFat<MatrixHolder> create (CREF<MatrixLayout> that) ;

	virtual void initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z ,CREF<VectorLayout> w) = 0 ;
	virtual VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<MatrixLayout> that) const = 0 ;
	virtual FLAG compr (CREF<MatrixLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual MatrixLayout add (CREF<MatrixLayout> that) const = 0 ;
	virtual void add_with (CREF<MatrixLayout> that) = 0 ;
	virtual MatrixLayout sub (CREF<MatrixLayout> that) const = 0 ;
	virtual void sub_with (CREF<MatrixLayout> that) = 0 ;
	virtual MatrixLayout mul (CREF<A> scale) const = 0 ;
	virtual void mul_with (CREF<A> scale) = 0 ;
	virtual MatrixLayout mul (CREF<MatrixLayout> that) const = 0 ;
	virtual void mul_with (CREF<MatrixLayout> scale) = 0 ;
	virtual MatrixLayout minus () const = 0 ;
	virtual MatrixLayout transpose () const = 0 ;
	virtual MatrixLayout triangular () const = 0 ;
	virtual MatrixLayout homogensize () const = 0 ;
	virtual A determinant () const = 0 ;
	virtual MatrixLayout inverse () const = 0 ;
	virtual A trace () const = 0 ;
	virtual RREF<Pointer> decompose () const = 0 ;
	virtual RREF<Pointer> singular () const = 0 ;
	virtual MatrixLayout pseudo_inverse () const = 0 ;

	virtual void DiagMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) = 0 ;
	virtual void ShearMatrix_initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z) = 0 ;
	virtual void RotationMatrix_initialize (CREF<VectorLayout> normal ,CREF<A> angle) = 0 ;
	virtual void TranslationMatrix_initialize (CREF<VectorLayout> xyz) = 0 ;
	virtual void TranslationMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z) = 0 ;
	virtual void PerspectiveMatrix_initialize (CREF<A> fx ,CREF<A> fy ,CREF<A> wx ,CREF<A> wy) = 0 ;
	virtual void ProjectionMatrix_initialize (CREF<VectorLayout> normal ,CREF<VectorLayout> center ,CREF<VectorLayout> light) = 0 ;
	virtual void ViewMatrix_initialize (CREF<VectorLayout> vx ,CREF<VectorLayout> vy) = 0 ;
	virtual void ViewMatrix_initialize (CREF<VectorLayout> vx ,CREF<VectorLayout> vy ,CREF<FLAG> flag) = 0 ;
} ;

template <class A>
class Matrix implement MatrixLayout {
private:
	struct DECOMPOSE {
		Matrix mT ;
		Matrix mR ;
		Matrix mS ;
		Matrix mH ;
	} ;

	struct SINGULAR {
		Matrix mU ;
		Matrix mS ;
		Matrix mV ;
	} ;

protected:
	BoxBuffer<A ,ENUM<16>> mMatrix ;

public:
	implicit Matrix () = default ;

	implicit Matrix (RREF<MatrixLayout> that) :Matrix (keep[TYPE<RREF<Matrix>>::expr] (that)) {}

	explicit Matrix (CREF<Vector<A>> x ,CREF<Vector<A>> y ,CREF<Vector<A>> z ,CREF<Vector<A>> w) {
		MatrixHolder<A>::create (thiz)->initialize (x ,y ,z ,w) ;
	}

	imports CREF<Matrix> zero () {
		return MatrixHolder<A>::create (Pointer::make (0))->zero () ;
	}

	imports CREF<Matrix> identity () {
		return MatrixHolder<A>::create (Pointer::make (0))->identity () ;
	}

	VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
		return MatrixHolder<A>::create (thiz)->at (x ,y) ;
	}

	VREF<A> at (CREF<PIXEL> xy) leftvalue {
		return at (xy.mX ,xy.mY) ;
	}

	inline VREF<A> operator[] (CREF<PIXEL> xy) leftvalue {
		return at (xy) ;
	}

	inline RowProxy<VPTR<Matrix>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VPTR<Matrix>> ((&thiz) ,y) ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return MatrixHolder<A>::create (thiz)->at (x ,y) ;
	}

	CREF<A> at (CREF<PIXEL> xy) const leftvalue {
		return at (xy.mX ,xy.mY) ;
	}

	inline CREF<A> operator[] (CREF<PIXEL> xy) const leftvalue {
		return at (xy) ;
	}

	inline RowProxy<CPTR<Matrix>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CPTR<Matrix>> ((&thiz) ,y) ;
	}

	BOOL equal (CREF<Matrix> that) const {
		return MatrixHolder<A>::create (thiz)->at (that) ;
	}

	inline BOOL operator== (CREF<Matrix> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Matrix> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Matrix> that) const {
		return MatrixHolder<A>::create (thiz)->at (that) ;
	}

	inline BOOL operator< (CREF<Matrix> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Matrix> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Matrix> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Matrix> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return MatrixHolder<A>::create (thiz)->visit (visitor) ;
	}

	Matrix add (CREF<Matrix> that) const {
		return MatrixHolder<A>::create (thiz)->add (that) ;
	}

	inline Matrix operator+ (CREF<Matrix> that) const {
		return add (that) ;
	}

	void add_with (CREF<Matrix> that) {
		return MatrixHolder<A>::create (thiz)->add_with (that) ;
	}

	inline void operator+= (CREF<Matrix> that) {
		add_with (that) ;
	}

	Matrix sub (CREF<Matrix> that) const {
		return MatrixHolder<A>::create (thiz)->sub (that) ;
	}

	inline Matrix operator- (CREF<Matrix> that) const {
		return sub (that) ;
	}

	void sub_with (CREF<Matrix> that) {
		return MatrixHolder<A>::create (thiz)->sub_with (that) ;
	}

	inline void operator-= (CREF<Matrix> that) {
		sub_with (that) ;
	}

	Matrix mul (CREF<A> scale) const {
		return MatrixHolder<A>::create (thiz)->mul (scale) ;
	}

	inline Matrix operator* (CREF<A> scale) const {
		return mul (scale) ;
	}

	inline friend Matrix operator* (CREF<A> scale ,CREF<Matrix> that) {
		return that.mul (scale) ;
	}

	void mul_with (CREF<A> scale) {
		return MatrixHolder<A>::create (thiz)->mul_with (scale) ;
	}

	inline void operator*= (CREF<A> scale) {
		mul_with (scale) ;
	}

	Vector<A> mul (CREF<Vector<A>> that) const {
		return MatrixHolder<A>::create (thiz)->mul (that) ;
	}

	inline Vector<A> operator* (CREF<Vector<A>> that) const {
		return mul (that) ;
	}

	Matrix mul (CREF<Matrix> that) const {
		return MatrixHolder<A>::create (thiz)->mul (that) ;
	}

	inline Matrix operator* (CREF<Matrix> that) const {
		return mul (that) ;
	}

	void mul_with (CREF<Matrix> that) {
		return MatrixHolder<A>::create (thiz)->mul_with (that) ;
	}

	inline void operator*= (CREF<Matrix> that) {
		return mul_with (that) ;
	}

	inline Matrix operator+ () const {
		return thiz ;
	}

	Matrix minus () const {
		return MatrixHolder<A>::create (thiz)->minus () ;
	}

	inline Matrix operator- () const {
		return minus () ;
	}

	Matrix transpose () const {
		return MatrixHolder<A>::create (thiz)->transpose () ;
	}

	Matrix triangular () const {
		return MatrixHolder<A>::create (thiz)->triangular () ;
	}

	Matrix homogensize () const {
		return MatrixHolder<A>::create (thiz)->homogensize () ;
	}

	A determinant () const {
		return MatrixHolder<A>::create (thiz)->determinant () ;
	}

	Matrix inverse () const {
		return MatrixHolder<A>::create (thiz)->inverse () ;
	}

	A trace () const {
		return MatrixHolder<A>::create (thiz)->trace () ;
	}

	DECOMPOSE decompose () const {
		return MatrixHolder<A>::create (thiz)->decompose () ;
	}

	SINGULAR singular () const {
		return MatrixHolder<A>::create (thiz)->singular () ;
	}

	Matrix pseudo_inverse () const {
		return MatrixHolder<A>::create (thiz)->pseudo_inverse () ;
	}
} ;

template <class A>
class DiagMatrix implement Matrix<A> {
public:
	explicit DiagMatrix (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) {
		MatrixHolder<A>::create (thiz)->DiagMatrix_initialize (x ,y ,z ,w) ;
	}
} ;

template <class A>
class ShearMatrix implement Matrix<A> {
public:
	explicit ShearMatrix (CREF<Vector<A>> x ,CREF<Vector<A>> y ,CREF<Vector<A>> z) {
		MatrixHolder<A>::create (thiz)->ShearMatrix_initialize (x ,y ,z) ;
	}
} ;

template <class A>
class RotationMatrix implement Matrix<A> {
public:
	explicit RotationMatrix (CREF<Vector<A>> normal ,CREF<A> angle) {
		MatrixHolder<A>::create (thiz)->RotationMatrix_initialize (normal ,angle) ;
	}
} ;

template <class A>
class TranslationMatrix implement Matrix<A> {
public:
	explicit TranslationMatrix (CREF<Vector<A>> xyz) {
		MatrixHolder<A>::create (thiz)->TranslationMatrix_initialize (xyz) ;
	}

	explicit TranslationMatrix (CREF<A> x ,CREF<A> y ,CREF<A> z) {
		MatrixHolder<A>::create (thiz)->TranslationMatrix_initialize (x ,y ,z) ;
	}
} ;

template <class A>
class PerspectiveMatrix implement Matrix<A> {
public:
	explicit PerspectiveMatrix (CREF<A> fx ,CREF<A> fy ,CREF<A> wx ,CREF<A> wy) {
		MatrixHolder<A>::create (thiz)->PerspectiveMatrix_initialize (fx ,fy ,wx ,wy) ;
	}
} ;

template <class A>
class ProjectionMatrix implement Matrix<A> {
public:
	explicit ProjectionMatrix (CREF<Vector<A>> normal) {
		MatrixHolder<A>::create (thiz)->ProjectionMatrix_initialize (normal) ;
	}

	explicit ProjectionMatrix (CREF<Vector<A>> normal ,CREF<Vector<A>> center ,CREF<Vector<A>> light) {
		MatrixHolder<A>::create (thiz)->ProjectionMatrix_initialize (normal ,center ,light) ;
	}
} ;

struct ViewMatrixFlag {
	enum {
		XY ,
		XZ ,
		YX ,
		YZ ,
		ZX ,
		ZY ,
		MAX_SIZE
	} ;
} ;

template <class A>
class ViewMatrix implement Matrix<A> {
public:
	explicit ViewMatrix (CREF<Vector<A>> vx ,CREF<Vector<A>> vy) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vx ,vy) ;
	}

	imports Matrix<A> make_xy (CREF<Vector<A>> vx ,CREF<Vector<A>> vy) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vx ,vy ,ViewMatrixFlag::XY) ;
	}

	imports Matrix<A> make_xz (CREF<Vector<A>> vx ,CREF<Vector<A>> vz) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vx ,vz ,ViewMatrixFlag::XZ) ;
	}

	imports Matrix<A> make_yx (CREF<Vector<A>> vy ,CREF<Vector<A>> vx) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vy ,vx ,ViewMatrixFlag::YX) ;
	}

	imports Matrix<A> make_yz (CREF<Vector<A>> vy ,CREF<Vector<A>> vz) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vy ,vz ,ViewMatrixFlag::YZ) ;
	}

	imports Matrix<A> make_zx (CREF<Vector<A>> vz ,CREF<Vector<A>> vx) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vz ,vx ,ViewMatrixFlag::ZX) ;
	}

	imports Matrix<A> make_zy (CREF<Vector<A>> vz ,CREF<Vector<A>> vy) {
		MatrixHolder<A>::create (thiz)->ViewMatrix_initialize (vz ,vy ,ViewMatrixFlag::ZY) ;
	}
} ;

template <class A>
struct QuaternionHolder implement Interface {
	imports VFat<QuaternionHolder> create (VREF<QuaternionLayout> that) ;
	imports CFat<QuaternionHolder> create (CREF<QuaternionLayout> that) ;

	virtual void initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) = 0 ;
	virtual void initialize (CREF<MatrixLayout> that) = 0 ;
	virtual VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual BOOL equal (CREF<QuaternionLayout> that) const = 0 ;
	virtual FLAG compr (CREF<QuaternionLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual VectorLayout axis () const = 0 ;
	virtual A angle () const = 0 ;
	virtual VectorLayout angle_axis () const = 0 ;
	virtual MatrixLayout matrix () const = 0 ;
	virtual QuaternionLayout normalize () const = 0 ;
} ;

template <class A>
class Quaternion implement QuaternionLayout {
protected:
	BoxBuffer<A ,RANK4> mQuaternion ;

public:
	implicit Quaternion () = default ;

	implicit Quaternion (RREF<QuaternionLayout> that) :Quaternion (keep[TYPE<RREF<Quaternion>>::expr] (that)) {}

	explicit Quaternion (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) {
		QuaternionHolder<A>::create (thiz)->initialize (x ,y ,z ,w) ;
	}

	explicit Quaternion (CREF<Matrix<A>> that) {
		QuaternionHolder<A>::create (thiz)->initialize (that) ;
	}

	VREF<A> at (CREF<INDEX> y) leftvalue {
		return QuaternionHolder<A>::create (thiz)->at (y) ;
	}

	inline VREF<A> operator[] (CREF<INDEX> y) leftvalue {
		return at (y) ;
	}

	CREF<A> at (CREF<INDEX> y) const leftvalue {
		return QuaternionHolder<A>::create (thiz)->at (y) ;
	}

	inline CREF<A> operator[] (CREF<INDEX> y) const leftvalue {
		return at (y) ;
	}

	BOOL equal (CREF<Quaternion> that) const {
		return QuaternionHolder<A>::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<Quaternion> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Quaternion> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Quaternion> that) const {
		return QuaternionHolder<A>::create (thiz)->compr (that) ;
	}

	inline BOOL operator< (CREF<Quaternion> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Quaternion> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Quaternion> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Quaternion> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return QuaternionHolder<A>::create (thiz)->visit (visitor) ;
	}

	Vector<A> axis () const {
		return QuaternionHolder<A>::create (thiz)->axis () ;
	}

	A angle () const {
		return QuaternionHolder<A>::create (thiz)->angle () ;
	}

	Vector<A> angle_axis () const {
		return QuaternionHolder<A>::create (thiz)->angle_axis () ;
	}

	Matrix<A> matrix () const {
		return QuaternionHolder<A>::create (thiz)->matrix () ;
	}

	Quaternion normalize () const {
		return QuaternionHolder<A>::create (thiz)->normalize () ;
	}
} ;
} ;