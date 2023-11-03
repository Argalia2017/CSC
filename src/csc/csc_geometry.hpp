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
template <class...>
trait VECTOR_HELP ;

template <class...>
trait MATRIX_HELP ;

template <class...>
trait VECTOR_HELP ;

template <class A>
trait VECTOR_HELP<A ,ALWAYS> {
	class Vector ;
	using Matrix = typename MATRIX_HELP<A ,ALWAYS>::Matrix ;

	struct Holder implement Interface {
		imports VFat<Holder> linkage () ;
		imports VFat<Holder> linkage (VREF<Vector> that) ;
		imports CFat<Holder> linkage (CREF<Vector> that) ;

		virtual void initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) = 0 ;
		virtual void initialize (CREF<PIXEL> xy) = 0 ;
		virtual void initialize (CREF<ARRAY2<FLT32>> xy) = 0 ;
		virtual void initialize (CREF<ARRAY3<FLT32>> xyz) = 0 ;
		virtual RFat<Vector> zero () const = 0 ;
		virtual RFat<Vector> axis_x () const = 0 ;
		virtual RFat<Vector> axis_y () const = 0 ;
		virtual RFat<Vector> axis_z () const = 0 ;
		virtual RFat<Vector> axis_w () const = 0 ;
		virtual ARRAY2<FLT32> xy () const = 0 ;
		virtual ARRAY3<FLT32> xyz () const = 0 ;
		virtual void add_with (CREF<Vector> that) = 0 ;
		virtual RFat<Vector> sub (CREF<Vector> that) const = 0 ;
		virtual void sub_with (CREF<Vector> that) = 0 ;
		virtual RFat<Vector> mul (CREF<A> scale) const = 0 ;
		virtual void mul_with (CREF<A> scale) = 0 ;
		virtual A dot (CREF<Vector> that) const = 0 ;
		virtual RFat<Vector> mul (CREF<Matrix> that) const = 0 ;
		virtual RFat<Vector> cross (CREF<Vector> that) const = 0 ;
		virtual void cross_with (CREF<Vector> that) = 0 ;
		virtual RFat<Vector> minus () const = 0 ;
		virtual A magnitude () const = 0 ;
		virtual RFat<Vector> normalize () const = 0 ;
		virtual RFat<Vector> projection () const = 0 ;
		virtual RFat<Vector> homogenize () const = 0 ;
	} ;

	class Vector {
	protected:
		BoxBuffer<A ,RANK4> mVector ;

	public:
		implicit Vector () = default ;

		explicit Vector (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) {
			Holder::linkage (thiz)->initialize (x ,y ,z ,w) ;
		}

		explicit Vector (CREF<ARRAY2<FLT32>> xy) {
			Holder::linkage (thiz)->initialize (xy) ;
		}

		explicit Vector (CREF<ARRAY3<FLT32>> xyz) {
			Holder::linkage (thiz)->initialize (xyz) ;
		}

		imports CREF<Vector> zero () {
			return Holder::linkage ()->zero () ;
		}

		imports CREF<Vector> axis_x () {
			return Holder::linkage ()->axis_x () ;
		}

		imports CREF<Vector> axis_y () {
			return Holder::linkage ()->axis_y () ;
		}

		imports CREF<Vector> axis_z () {
			return Holder::linkage ()->axis_z () ;
		}

		imports CREF<Vector> axis_w () {
			return Holder::linkage ()->axis_w () ;
		}

		ARRAY2<FLT32> xy () const {
			return Holder::linkage (thiz)->xy () ;
		}

		ARRAY3<FLT32> xyz () const {
			return Holder::linkage (thiz)->xyz () ;
		}

		inline VREF<A> at (CREF<INDEX> y) leftvalue {
			assert (operator_between (y ,0 ,4)) ;
			return mVector[y] ;
		}

		inline VREF<A> operator[] (CREF<INDEX> y) leftvalue {
			return at (y) ;
		}

		inline CREF<A> at (CREF<INDEX> y) const leftvalue {
			assert (operator_between (y ,0 ,4)) ;
			return mVector[y] ;
		}

		inline CREF<A> operator[] (CREF<INDEX> y) const leftvalue {
			return at (y) ;
		}

		BOOL equal (CREF<Vector> that) const {
			return operator_equal (mVector ,that.mVector) ;
		}

		inline BOOL operator== (CREF<Vector> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Vector> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Vector> that) const {
			return operator_compr (mVector ,that.mVector) ;
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

		template <class ARG1>
		FLAG hash (CREF<ARG1> visitor) const {
			return visitor (mVector) ;
		}

		Vector add (CREF<Vector> that) const {
			return Holder::linkage (thiz)->add (that) ;
		}

		inline Vector operator+ (CREF<Vector> that) const {
			return add (that) ;
		}

		void add_with (CREF<Vector> that) {
			return Holder::linkage (thiz)->add_with (that) ;
		}

		inline void operator+= (CREF<Vector> that) {
			add_with (that) ;
		}

		Vector sub (CREF<Vector> that) const {
			return Holder::linkage (thiz)->sub (that) ;
		}

		inline Vector operator- (CREF<Vector> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Vector> that) {
			return Holder::linkage (thiz)->sub_with (that) ;
		}

		inline void operator-= (CREF<Vector> that) {
			sub_with (that) ;
		}

		Vector mul (CREF<A> scale) const {
			return Holder::linkage (thiz)->mul (scale) ;
		}

		inline Vector operator* (CREF<A> scale) const {
			return mul (scale) ;
		}

		inline friend Vector operator* (CREF<A> scale ,CREF<Vector> that) {
			return that.mul (scale) ;
		}

		void mul_with (CREF<A> scale) {
			return Holder::linkage (thiz)->mul_with (scale) ;
		}

		inline void operator*= (CREF<A> scale) {
			mul_with (scale) ;
		}

		A dot (CREF<Vector> that) const {
			return Holder::linkage (thiz)->dot (that) ;
		}

		inline A operator* (CREF<Vector> that) const {
			return dot (that) ;
		}

		Vector mul (CREF<Matrix> that) const {
			return Holder::linkage (thiz)->mul (that) ;
		}

		inline Vector operator* (CREF<Matrix> that) const {
			return mul (that) ;
		}

		Vector cross (CREF<Vector> that) const {
			return Holder::linkage (thiz)->cross (that) ;
		}

		inline Vector operator^ (CREF<Vector> that) const {
			return cross (that) ;
		}

		inline void operator^= (CREF<Vector> that) {
			return Holder::linkage (thiz)->cross_with (that) ;
		}

		inline Vector operator+ () const {
			return thiz ;
		}

		Vector minus () const {
			return Holder::linkage (thiz)->minus () ;
		}

		inline Vector operator- () const {
			return minus () ;
		}

		A magnitude () const {
			return Holder::linkage (thiz)->magnitude () ;
		}

		Vector normalize () const {
			return Holder::linkage (thiz)->normalize () ;
		}

		Vector projection () const {
			return Holder::linkage (thiz)->projection () ;
		}

		Vector homogenize () const {
			return Holder::linkage (thiz)->homogenize () ;
		}
	} ;
} ;

template <class...>
trait MATRIX_HELP ;

template <class A>
trait MATRIX_HELP<A ,ALWAYS> {
	using Vector = typename VECTOR_HELP<A ,ALWAYS>::Vector ;
	class Matrix ;

	struct DECOMPOSE ;
	struct SINGULAR ;

	struct Holder implement Interface {
		imports VFat<Holder> linkage () ;
		imports VFat<Holder> linkage (VREF<Matrix> that) ;
		imports CFat<Holder> linkage (CREF<Matrix> that) ;

		virtual void initialize (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z ,CREF<Vector> w) = 0 ;
		virtual RFat<Matrix> add (CREF<Matrix> that) const = 0 ;
		virtual void add_with (CREF<Matrix> that) = 0 ;
		virtual RFat<Matrix> sub (CREF<Matrix> that) const = 0 ;
		virtual void sub_with (CREF<Matrix> that) = 0 ;
		virtual RFat<Matrix> mul (CREF<A> scale) const = 0 ;
		virtual void mul_with (CREF<A> scale) = 0 ;
		virtual RFat<Matrix> mul (CREF<Matrix> that) const = 0 ;
		virtual RFat<Matrix> minus () const = 0 ;
		virtual RFat<Matrix> transpose () const = 0 ;
		virtual RFat<Matrix> triangular () const = 0 ;
		virtual A determinant () const = 0 ;
		virtual RFat<Matrix> inverse () const = 0 ;
		virtual A trace () const = 0 ;
		virtual RFat<DECOMPOSE> decompose () const = 0 ;
		virtual RFat<SINGULAR> singular () const = 0 ;
		virtual RFat<Matrix> pseudo_inverse () const = 0 ;

		virtual void DiagMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) = 0 ;
		virtual void ShearMatrix_initialize (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) = 0 ;
		virtual void RotationMatrix_initialize (CREF<Vector> normal ,CREF<A> angle) = 0 ;
		virtual void TranslationMatrix_initialize (CREF<Vector> xyz) = 0 ;
		virtual void TranslationMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z) = 0 ;
		virtual void PerspectiveMatrix_initialize (CREF<A> fx ,CREF<A> fy ,CREF<A> wx ,CREF<A> wy) = 0 ;
		virtual void ProjectionMatrix_initialize (CREF<Vector> normal ,CREF<A> center ,CREF<Vector> light) = 0 ;
		virtual void ViewMatrix_initialize (CREF<Vector> vx ,CREF<Vector> vy) = 0 ;
		virtual void ViewMatrix_initialize (CREF<Vector> vx ,CREF<Vector> vy ,CREF<FLAG> flag) = 0 ;
	} ;

	class Matrix {
	protected:
		BoxBuffer<A ,ENUM<16>> mMatrix ;

	public:
		implicit Matrix () = default ;

		explicit Matrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z ,CREF<Vector> w) {
			Holder::linkage (thiz)->initialize (x ,y ,z ,w) ;
		}

		imports CREF<Matrix> zero () {
			return Holder::linkage ()->zero () ;
		}

		imports CREF<Matrix> identity () {
			return Holder::linkage ()->identity () ;
		}

		VREF<A> at (CREF<PIXEL> xy) leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline VREF<A> operator[] (CREF<PIXEL> xy) leftvalue {
			return at (xy) ;
		}

		VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
			assert (operator_between (x ,0 ,4)) ;
			assert (operator_between (y ,0 ,4)) ;
			return mMatrix[y * 4 + x] ;
		}

		inline RowProxy<VRef<Matrix>> operator[] (CREF<INDEX> y) leftvalue {
			return RowProxy<VRef<Matrix>> (VRef<Matrix>::reference (thiz) ,y) ;
		}

		CREF<A> at (CREF<PIXEL> xy) const leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline CREF<A> operator[] (CREF<PIXEL> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
			assert (operator_between (x ,0 ,4)) ;
			assert (operator_between (y ,0 ,4)) ;
			return mMatrix[y * 4 + x] ;
		}

		inline RowProxy<CRef<Matrix>> operator[] (CREF<INDEX> y) const leftvalue {
			return RowProxy<CRef<Matrix>> (CRef<Matrix>::reference (thiz) ,y) ;
		}

		BOOL equal (CREF<Matrix> that) const {
			return operator_equal (mMatrix ,that.mMatrix) ;
		}

		inline BOOL operator== (CREF<Matrix> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Matrix> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Matrix> that) const {
			return operator_compr (mMatrix ,that.mMatrix) ;
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

		template <class ARG1>
		FLAG hash (CREF<ARG1> visitor) const {
			return visitor (mMatrix) ;
		}

		Matrix add (CREF<Matrix> that) const {
			return Holder::linkage (thiz)->add (that) ;
		}

		inline Matrix operator+ (CREF<Matrix> that) const {
			return add (that) ;
		}

		void add_with (CREF<Matrix> that) {
			return Holder::linkage (thiz)->add_with (that) ;
		}

		inline void operator+= (CREF<Matrix> that) {
			add_with (that) ;
		}

		Matrix sub (CREF<Matrix> that) const {
			return Holder::linkage (thiz)->sub (that) ;
		}

		inline Matrix operator- (CREF<Matrix> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Matrix> that) {
			return Holder::linkage (thiz)->sub_with (that) ;
		}

		inline void operator-= (CREF<Matrix> that) {
			sub_with (that) ;
		}

		Matrix mul (CREF<A> scale) const {
			return Holder::linkage (thiz)->mul (scale) ;
		}

		inline Matrix operator* (CREF<A> scale) const {
			return mul (scale) ;
		}

		inline friend Matrix operator* (CREF<A> scale ,CREF<Matrix> that) {
			return that.mul (scale) ;
		}

		void mul_with (CREF<A> scale) {
			return Holder::linkage (thiz)->mul_with (scale) ;
		}

		inline void operator*= (CREF<A> scale) {
			mul_with (scale) ;
		}

		Vector mul (CREF<Vector> that) const {
			return Holder::linkage (thiz)->mul (that) ;
		}

		inline Vector operator* (CREF<Vector> that) const {
			return mul (that) ;
		}

		Matrix mul (CREF<Matrix> that) const {
			return Holder::linkage (thiz)->mul (that) ;
		}

		inline Matrix operator* (CREF<Matrix> that) const {
			return mul (that) ;
		}

		inline Matrix operator+ () const {
			return thiz ;
		}

		Matrix minus () const {
			return Holder::linkage (thiz)->minus () ;
		}

		inline Matrix operator- () const {
			return minus () ;
		}

		Matrix transpose () const {
			return Holder::linkage (thiz)->transpose () ;
		}

		Matrix triangular () const {
			return Holder::linkage (thiz)->triangular () ;
		}

		A determinant () const {
			return Holder::linkage (thiz)->determinant () ;
		}

		Matrix inverse () const {
			return Holder::linkage (thiz)->inverse () ;
		}

		A trace () const {
			return Holder::linkage (thiz)->trace () ;
		}

		DECOMPOSE decompose () const {
			return Holder::linkage (thiz)->decompose () ;
		}

		SINGULAR singular () const {
			return Holder::linkage (thiz)->singular () ;
		}

		Matrix pseudo_inverse () const {
			return Holder::linkage (thiz)->pseudo_inverse () ;
		}
	} ;

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

	class DiagMatrix implement Matrix {
	public:
		explicit DiagMatrix (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) {
			Holder::linkage (thiz)->DiagMatrix_initialize (x ,y ,z ,w) ;
		}
	} ;

	class ShearMatrix implement Matrix {
	public:
		explicit ShearMatrix (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) {
			Holder::linkage (thiz)->ShearMatrix_initialize (x ,y ,z) ;
		}
	} ;

	class RotationMatrix implement Matrix {
	public:
		explicit RotationMatrix (CREF<Vector> normal ,CREF<A> angle) {
			Holder::linkage (thiz)->RotationMatrix_initialize (normal ,angle) ;
		}
	} ;

	class TranslationMatrix implement Matrix {
	public:
		explicit TranslationMatrix (CREF<Vector> xyz) {
			Holder::linkage (thiz)->TranslationMatrix_initialize (xyz) ;
		}

		explicit TranslationMatrix (CREF<A> x ,CREF<A> y ,CREF<A> z) {
			Holder::linkage (thiz)->TranslationMatrix_initialize (x ,y ,z) ;
		}
	} ;

	class PerspectiveMatrix implement Matrix {
	public:
		explicit PerspectiveMatrix (CREF<A> fx ,CREF<A> fy ,CREF<A> wx ,CREF<A> wy) {
			Holder::linkage (thiz)->PerspectiveMatrix_initialize (fx ,fy ,wx ,wy) ;
		}
	} ;

	class ProjectionMatrix implement Matrix {
	public:
		explicit ProjectionMatrix (CREF<Vector> normal) {
			Holder::linkage (thiz)->ProjectionMatrix_initialize (normal) ;
		}

		explicit ProjectionMatrix (CREF<Vector> normal ,CREF<A> center ,CREF<Vector> light) {
			Holder::linkage (thiz)->ProjectionMatrix_initialize (normal ,center ,light) ;
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

	class ViewMatrix implement Matrix {
	public:
		explicit ViewMatrix (CREF<Vector> vx ,CREF<Vector> vy) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vx ,vy) ;
		}

		imports Matrix make_xy (CREF<Vector> vx ,CREF<Vector> vy) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vx ,vy ,ViewMatrixFlag::XY) ;
		}

		imports Matrix make_xz (CREF<Vector> vx ,CREF<Vector> vz) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vx ,vz ,ViewMatrixFlag::XZ) ;
		}

		imports Matrix make_yx (CREF<Vector> vy ,CREF<Vector> vx) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vy ,vx ,ViewMatrixFlag::YX) ;
		}

		imports Matrix make_yz (CREF<Vector> vy ,CREF<Vector> vz) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vy ,vz ,ViewMatrixFlag::YZ) ;
		}

		imports Matrix make_zx (CREF<Vector> vz ,CREF<Vector> vx) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vz ,vx ,ViewMatrixFlag::ZX) ;
		}

		imports Matrix make_zy (CREF<Vector> vz ,CREF<Vector> vy) {
			Holder::linkage (thiz)->ViewMatrix_initialize (vz ,vy ,ViewMatrixFlag::ZY) ;
		}
	} ;
} ;

template <class...>
trait QUATERNION_HELP ;

template <class A>
trait QUATERNION_HELP<A ,ALWAYS> {
	using Vector = typename VECTOR_HELP<A ,ALWAYS>::Vector ;
	using Matrix = typename VECTOR_HELP<A ,ALWAYS>::Matrix ;
	class Quaternoin ;

	struct Holder implement Interface {
		imports VFat<Holder> linkage () ;
		imports VFat<Holder> linkage (VREF<Quaternoin> that) ;
		imports CFat<Holder> linkage (CREF<Quaternoin> that) ;

		virtual void initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) = 0 ;
		virtual void initialize (CREF<Matrix> that) = 0 ;
		virtual Vector axis () const = 0 ;
		virtual A angle () const = 0 ;
		virtual Vector angle_axis () const = 0 ;
		virtual Matrix matrix () const = 0 ;
	} ;

	class Quaternion {
	protected:
		BoxBuffer<A ,RANK4> mQuaternion ;

	public:
		implicit Quaternion () = default ;

		explicit Quaternion (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) {
			Holder::linkage (thiz)->initialize (x ,y ,z ,w) ;
		}

		explicit Quaternion (CREF<Matrix> that) {
			Holder::linkage (thiz)->initialize (that) ;
		}

		CREF<A> at (CREF<INDEX> y) const leftvalue {
			return mQuaternion[y] ;
		}

		inline CREF<A> operator[] (CREF<INDEX> y) const leftvalue {
			return at (y) ;
		}

		BOOL equal (CREF<Quaternion> that) const {
			return operator_equal (mQuaternion ,that.mQuaternion) ;
		}

		inline BOOL operator== (CREF<Quaternion> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Quaternion> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Quaternion> that) const {
			return operator_compr (mQuaternion ,that.mQuaternion) ;
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

		template <class ARG1>
		FLAG hash (CREF<ARG1> visitor) const {
			return visitor (mQuaternion) ;
		}

		Vector axis () const {
			return Holder::linkage (thiz)->axis () ;
		}

		A angle () const {
			return Holder::linkage (thiz)->angle () ;
		}

		Vector angle_axis () const {
			return Holder::linkage (thiz)->angle_axis () ;
		}

		Matrix matrix () const {
			return Holder::linkage (thiz)->matrix () ;
		}
	} ;
} ;

template <class A>
using Vector = typename VECTOR_HELP<A ,ALWAYS>::Vector ;

template <class A>
using Matrix = typename MATRIX_HELP<A ,ALWAYS>::Matrix ;

template <class A>
using Quaternion = typename QUATERNION_HELP<A ,ALWAYS>::Quaternion ;
} ;