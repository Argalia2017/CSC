#pragma once

#ifndef __CSC_GEOMETRY__
#define __CSC_GEOMETRY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"

namespace CSC {
template <class...>
trait MATRIX_HELP ;

template <class...>
trait VECTOR_HELP ;

template <class ITEM>
trait VECTOR_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = RANK4 ;

	template <class ARG1>
	using MACRO_Matrix = typename DEPENDENT<MATRIX_HELP<ITEM ,ALWAYS> ,ARG1>::Matrix ;

	class Vector {
	protected:
		BoxBuffer<ITEM ,SIZE> mVector ;

	public:
		implicit Vector () = default ;

		explicit Vector (CREF<ARRAY3<ITEM>> xyz ,CREF<ITEM> w) {
			mVector[0] = xyz[0] ;
			mVector[1] = xyz[1] ;
			mVector[2] = xyz[2] ;
			mVector[3] = w ;
		}

		explicit Vector (CREF<ITEM> x ,CREF<ITEM> y ,CREF<ITEM> z ,CREF<ITEM> w) {
			mVector[0] = x ;
			mVector[1] = y ;
			mVector[2] = z ;
			mVector[3] = w ;
		}

		imports CREF<Vector> zero () {
			return memorize ([&] () {
				return Vector (ITEM (0) ,ITEM (0) ,ITEM (0) ,ITEM (0)) ;
			}) ;
		}

		imports CREF<Vector> axis_x () {
			return memorize ([&] () {
				return Vector (ITEM (1) ,ITEM (0) ,ITEM (0) ,ITEM (0)) ;
			}) ;
		}

		imports CREF<Vector> axis_y () {
			return memorize ([&] () {
				return Vector (ITEM (0) ,ITEM (1) ,ITEM (0) ,ITEM (0)) ;
			}) ;
		}

		imports CREF<Vector> axis_z () {
			return memorize ([&] () {
				return Vector (ITEM (0) ,ITEM (0) ,ITEM (1) ,ITEM (0)) ;
			}) ;
		}

		imports CREF<Vector> axis_w () {
			return memorize ([&] () {
				return Vector (ITEM (0) ,ITEM (0) ,ITEM (0) ,ITEM (1)) ;
			}) ;
		}

		ARRAY3<ITEM> xyz () const {
			ARRAY3<ITEM> ret ;
			ret[0] = mVector[0] ;
			ret[1] = mVector[1] ;
			ret[2] = mVector[2] ;
			return move (ret) ;
		}

		VREF<ITEM> at (CREF<INDEX> y) leftvalue {
			assert (vbetween (y ,0 ,4)) ;
			return mVector[y] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> y) leftvalue {
			return at (y) ;
		}

		CREF<ITEM> at (CREF<INDEX> y) const leftvalue {
			assert (vbetween (y ,0 ,4)) ;
			return mVector[y] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> y) const leftvalue {
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

		FLAG hash () const {
			return operator_hash (mVector) ;
		}

		Vector add (CREF<Vector> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mVector[i] = mVector[i] + that.mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator+ (CREF<Vector> that) const {
			return add (that) ;
		}

		void addto (CREF<Vector> that) {
			for (auto &&i : iter (0 ,SIZE::expr))
				mVector[i] += that.mVector[i] ;
		}

		inline void operator+= (CREF<Vector> that) {
			addto (that) ;
		}

		Vector sub (CREF<Vector> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mVector[i] = mVector[i] - that.mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator- (CREF<Vector> that) const {
			return sub (that) ;
		}

		void subto (CREF<Vector> that) {
			for (auto &&i : iter (0 ,SIZE::expr))
				mVector[i] -= that.mVector[i] ;
		}

		inline void operator-= (CREF<Vector> that) {
			subto (that) ;
		}

		Vector mul (CREF<ITEM> scale) const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mVector[i] = mVector[i] * scale ;
			return move (ret) ;
		}

		inline Vector operator* (CREF<ITEM> scale) const {
			return mul (scale) ;
		}

		inline friend Vector operator* (CREF<ITEM> scale ,CREF<Vector> that) {
			return that.mul (scale) ;
		}

		void multo (CREF<ITEM> scale) {
			for (auto &&i : iter (0 ,SIZE::expr))
				mVector[i] *= scale ;
		}

		inline void operator*= (CREF<ITEM> scale) {
			multo (scale) ;
		}

		ITEM dot (CREF<Vector> that) const {
			ITEM ret = ITEM (0) ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret += mVector[i] * that.mVector[i] ;
			return move (ret) ;
		}

		inline ITEM operator* (CREF<Vector> that) const {
			return dot (that) ;
		}

		template <class ARG1 = DEPEND>
		Vector mul (CREF<MACRO_Matrix<ARG1>> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,4)) {
				const auto r1x = at (0) * that.at (i ,0) ;
				const auto r2x = at (1) * that.at (i ,1) ;
				const auto r3x = at (2) * that.at (i ,2) ;
				const auto r4x = at (3) * that.at (i ,3) ;
				ret.at (i) = r1x + r2x + r3x + r4x ;
			}
			return move (ret) ;
		}

		template <class ARG1 = DEPEND>
		inline Vector operator* (CREF<MACRO_Matrix<ARG1>> that) const {
			return mul (that) ;
		}

		Vector cross (CREF<Vector> that) const {
			assert (mVector[3] == ITEM (0)) ;
			assert (that.mVector[3] == ITEM (0)) ;
			Vector ret ;
			ret.mVector[0] = mVector[1] * that.mVector[2] - mVector[2] * that.mVector[1] ;
			ret.mVector[1] = mVector[2] * that.mVector[0] - mVector[0] * that.mVector[2] ;
			ret.mVector[2] = mVector[0] * that.mVector[1] - mVector[1] * that.mVector[0] ;
			return move (ret) ;
		}

		inline Vector operator^ (CREF<Vector> that) const {
			return cross (that) ;
		}

		inline void operator^= (CREF<Vector> that) {
			thiz = cross (that) ;
		}

		inline Vector operator+ () const {
			return thiz ;
		}

		Vector minus () const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mVector[i] = -mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator- () const {
			return minus () ;
		}

		INDEX find_abs_max () const {
			INDEX ret = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,4)) {
				const auto r1x = MathProc::abs (mVector[i]) ;
				if (ret != NONE)
					if (rax >= r1x)
						continue ;
				ret = i ;
				rax = r1x ;
			}
			return move (ret) ;
		}

		INDEX find_abs_min () const {
			INDEX ret = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,4)) {
				const auto r1x = MathProc::abs (mVector[i]) ;
				if (ret != NONE)
					if (rax <= r1x)
						continue ;
				ret = i ;
				rax = r1x ;
			}
			return move (ret) ;
		}

		ITEM magnitude () const {
			assert (mVector[3] == ITEM (0)) ;
			const auto r1x = MathProc::square (mVector[0]) ;
			const auto r2x = MathProc::square (mVector[1]) ;
			const auto r3x = MathProc::square (mVector[2]) ;
			return MathProc::sqrt (r1x + r2x + r3x) ;
		}

		Vector normalize () const {
			Vector ret ;
			const auto r1x = MathProc::inverse (magnitude ()) ;
			ret.mVector[0] = mVector[0] * r1x ;
			ret.mVector[1] = mVector[1] * r1x ;
			ret.mVector[2] = mVector[2] * r1x ;
			ret.mVector[3] = ITEM (0) ;
			return move (ret) ;
		}

		Vector projection () const {
			Vector ret ;
			const auto r1x = MathProc::inverse (mVector[3]) ;
			ret.mVector[0] = mVector[0] * r1x ;
			ret.mVector[1] = mVector[1] * r1x ;
			ret.mVector[2] = mVector[2] * r1x ;
			ret.mVector[3] = ITEM (1) ;
			return move (ret) ;
		}

		Vector homogenize () const {
			Vector ret ;
			ret.mVector[0] = mVector[0] ;
			ret.mVector[1] = mVector[1] ;
			ret.mVector[2] = mVector[2] ;
			ret.mVector[3] = ITEM (0) ;
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using Vector = typename VECTOR_HELP<ITEM ,ALWAYS>::Vector ;

template <class...>
trait QUATERNION_HELP ;

template <class ITEM>
trait QUATERNION_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = RANK4 ;

	template <class ARG1>
	using MACRO_Matrix = typename DEPENDENT<MATRIX_HELP<ITEM ,ALWAYS> ,ARG1>::Matrix ;

	class Quaternion {
	protected:
		BoxBuffer<ITEM ,SIZE> mQuaternion ;

	public:
		implicit Quaternion () = default ;

		explicit Quaternion (CREF<ITEM> x ,CREF<ITEM> y ,CREF<ITEM> z ,CREF<ITEM> w) {
			mQuaternion[0] = x ;
			mQuaternion[1] = y ;
			mQuaternion[2] = z ;
			mQuaternion[3] = w ;
			update_normalize () ;
		}

		template <class ARG1 = DEPEND>
		explicit Quaternion (CREF<MACRO_Matrix<ARG1>> that) {
			const auto r1x = that.decompose ().mRotation ;
			const auto r2x = invoke ([&] () {
				ARRAY4<ITEM> ret ;
				ret[0] = ITEM (1) + r1x.at (0 ,0) + r1x.at (1 ,1) + r1x.at (2 ,2) ;
				ret[1] = ITEM (1) + r1x.at (0 ,0) - r1x.at (1 ,1) - r1x.at (2 ,2) ;
				ret[2] = ITEM (1) - r1x.at (0 ,0) + r1x.at (1 ,1) - r1x.at (2 ,2) ;
				ret[3] = ITEM (1) - r1x.at (0 ,0) - r1x.at (1 ,1) + r1x.at (2 ,2) ;
				return move (ret) ;
			}) ;
			const auto r3x = invoke ([&] () {
				INDEX ret = NONE ;
				auto rax = ITEM () ;
				for (auto &&i : iter (0 ,4)) {
					if (ret != NONE)
						if (rax >= r2x[i])
							continue ;
					ret = i ;
					rax = r2x[i] ;
				}
				return move (ret) ;
			}) ;
			const auto r4x = MathProc::inverse (ITEM (2) * MathProc::sqrt (r2x[r3x])) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (r3x == 0)
					discard ;
				mQuaternion[0] = (r1x.at (1 ,2) - r1x.at (2 ,1)) * r4x ;
				mQuaternion[1] = (r1x.at (2 ,0) - r1x.at (0 ,2)) * r4x ;
				mQuaternion[2] = (r1x.at (0 ,1) - r1x.at (1 ,0)) * r4x ;
				mQuaternion[3] = r2x[0] * r4x ;
			}
			if ifswitch (rxx) {
				if ifnot (r3x == 1)
					discard ;
				mQuaternion[0] = r2x[1] * r4x ;
				mQuaternion[1] = (r1x.at (0 ,1) + r1x.at (1 ,0)) * r4x ;
				mQuaternion[2] = (r1x.at (2 ,0) + r1x.at (0 ,2)) * r4x ;
				mQuaternion[3] = (r1x.at (1 ,2) - r1x.at (2 ,1)) * r4x ;
			}
			if ifswitch (rxx) {
				if ifnot (r3x == 2)
					discard ;
				mQuaternion[0] = (r1x.at (0 ,1) + r1x.at (1 ,0)) * r4x ;
				mQuaternion[1] = r2x[2] * r4x ;
				mQuaternion[2] = (r1x.at (1 ,2) + r1x.at (2 ,1)) * r4x ;
				mQuaternion[3] = (r1x.at (2 ,0) - r1x.at (0 ,2)) * r4x ;
			}
			if ifswitch (rxx) {
				if ifnot (r3x == 3)
					discard ;
				mQuaternion[0] = (r1x.at (2 ,0) + r1x.at (0 ,2)) * r4x ;
				mQuaternion[1] = (r1x.at (1 ,2) + r1x.at (2 ,1)) * r4x ;
				mQuaternion[2] = r2x[3] * r4x ;
				mQuaternion[3] = (r1x.at (0 ,1) - r1x.at (1 ,0)) * r4x ;
			}
			update_normalize () ;
		}

		Vector<ITEM> axis () const {
			const auto r1x = Vector<ITEM> (mQuaternion[0] ,mQuaternion[1] ,mQuaternion[2] ,ITEM (0)) ;
			return r1x.normalize () * MathProc::sign (mQuaternion[3]) ;
		}

		ITEM angle () const {
			const auto r1x = Vector<ITEM> (mQuaternion[0] ,mQuaternion[1] ,mQuaternion[2] ,ITEM (0)) ;
			const auto r2x = r1x.magnitude () * MathProc::sign (mQuaternion[3]) ;
			const auto r3x = MathProc::arctan (MathProc::abs (mQuaternion[3]) ,r2x) ;
			return r3x * ITEM (2) ;
		}

		Vector<ITEM> angle_axis () const {
			const auto r1x = angle () ;
			if (MathProc::inverse (r1x) == ITEM (0))
				return Vector<ITEM>::zero () ;
			return axis () * r1x ;
		}

		CREF<ITEM> at (CREF<INDEX> y) const leftvalue {
			assert (vbetween (y ,0 ,4)) ;
			return mQuaternion[y] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> y) const leftvalue {
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

		FLAG hash () const {
			return operator_hash (mQuaternion) ;
		}

	private:
		void update_normalize () {
			const auto r1x = MathProc::square (mQuaternion[0]) + MathProc::square (mQuaternion[1]) + MathProc::square (mQuaternion[2]) + MathProc::square (mQuaternion[3]) ;
			const auto r2x = MathProc::sqrt (r1x) ;
			const auto r3x = MathProc::inverse (r2x) ;
			mQuaternion[0] *= r3x ;
			mQuaternion[1] *= r3x ;
			mQuaternion[2] *= r3x ;
			mQuaternion[3] *= r3x ;
		}
	} ;
} ;

template <class ITEM>
using Quaternion = typename QUATERNION_HELP<ITEM ,ALWAYS>::Quaternion ;

template <class...>
trait MATRIX_HELP ;

template <class...>
trait MATRIX_SINGULAR_HELP ;

template <class ITEM>
trait MATRIX_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = ENUMAS<VAL ,ENUMID<16>> ;

	template <class ARG1>
	using MACRO_DECOMPOSE = typename DEPENDENT<MATRIX_HELP<ITEM ,ALWAYS> ,ARG1>::DECOMPOSE ;

	template <class ARG1>
	using MACRO_SVD_DECOMPOSE = typename DEPENDENT<MATRIX_HELP<ITEM ,ALWAYS> ,ARG1>::SVD_DECOMPOSE ;

	class Matrix {
	protected:
		BoxBuffer<ITEM ,SIZE> mMatrix ;

	public:
		implicit Matrix () = default ;

		explicit Matrix (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vz ,CREF<Vector<ITEM>> vw) {
			for (auto &&i : iter (0 ,4)) {
				at (0 ,i) = vx[i] ;
				at (1 ,i) = vy[i] ;
				at (2 ,i) = vz[i] ;
				at (3 ,i) = vw[i] ;
			}
		}

		imports CREF<Matrix> zero () {
			return memorize ([&] () {
				const auto r1x = Vector<ITEM>::zero () ;
				return Matrix (r1x ,r1x ,r1x ,r1x) ;
			}) ;
		}

		imports CREF<Matrix> identity () {
			return memorize ([&] () {
				const auto r1x = Vector<ITEM>::axis_x () ;
				const auto r2x = Vector<ITEM>::axis_y () ;
				const auto r3x = Vector<ITEM>::axis_z () ;
				const auto r4x = Vector<ITEM>::axis_w () ;
				return Matrix (r1x ,r2x ,r3x ,r4x) ;
			}) ;
		}

		imports Matrix make_diag (CREF<ITEM> x ,CREF<ITEM> y ,CREF<ITEM> z ,CREF<ITEM> w) {
			Matrix ret = zero () ;
			ret.at (0 ,0) = x ;
			ret.at (1 ,1) = y ;
			ret.at (2 ,2) = z ;
			ret.at (3 ,3) = w ;
			return move (ret) ;
		}

		imports Matrix make_shear (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vz) {
			const auto r1x = vx.normalize () ;
			const auto r2x = vy.normalize () ;
			const auto r3x = vz.normalize () ;
			const auto r4x = r1x * r2x ;
			const auto r5x = r1x * r3x ;
			const auto r6x = r2x * r3x ;
			const auto r7x = MathProc::sqrt (ITEM (1) - MathProc::square (r4x)) ;
			const auto r8x = (r6x - r4x * r5x) * MathProc::inverse (r7x) ;
			const auto r9x = MathProc::sqrt (ITEM (1) - MathProc::square (r5x) - MathProc::square (r8x)) ;
			const auto r10x = Vector<ITEM>::axis_x () ;
			const auto r11x = Vector<ITEM> (r4x ,r7x ,ITEM (0) ,ITEM (0)) ;
			const auto r12x = Vector<ITEM> (r5x ,r8x ,r9x ,ITEM (0)) ;
			const auto r13x = Vector<ITEM>::axis_w () ;
			return Matrix (r10x ,r11x ,r12x ,r13x) ;
		}

		imports Matrix make_rotation (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy) {
			const auto r1x = vx.normalize () ;
			const auto r2x = vy.normalize () ;
			const auto r3x = (r1x ^ r2x).normalize () ;
			const auto r4x = (r3x ^ r1x).normalize () ;
			const auto r5x = Vector<ITEM>::axis_w () ;
			return Matrix (r1x ,r4x ,r3x ,r5x) ;
		}

		imports Matrix make_rotation (CREF<Vector<ITEM>> normal ,CREF<ITEM> angle) {
			Matrix ret ;
			const auto r1x = normal.normalize () ;
			const auto r2x = MathProc::cos (angle) ;
			const auto r3x = r1x * MathProc::sin (angle) ;
			const auto r4x = r1x * (ITEM (1) - r2x) ;
			ret.at (0 ,0) = r1x[0] * r4x[0] + r2x ;
			ret.at (1 ,0) = r1x[0] * r4x[1] - r3x[2] ;
			ret.at (2 ,0) = r1x[0] * r4x[2] + r3x[1] ;
			ret.at (3 ,0) = ITEM (0) ;
			ret.at (0 ,1) = r1x[1] * r4x[0] + r3x[2] ;
			ret.at (1 ,1) = r1x[1] * r4x[1] + r2x ;
			ret.at (2 ,1) = r1x[1] * r4x[2] - r3x[0] ;
			ret.at (3 ,1) = ITEM (0) ;
			ret.at (0 ,2) = r1x[2] * r4x[0] - r3x[1] ;
			ret.at (1 ,2) = r1x[2] * r4x[1] + r3x[0] ;
			ret.at (2 ,2) = r1x[2] * r4x[2] + r2x ;
			ret.at (3 ,2) = ITEM (0) ;
			ret.at (0 ,3) = ITEM (0) ;
			ret.at (1 ,3) = ITEM (0) ;
			ret.at (2 ,3) = ITEM (0) ;
			ret.at (3 ,3) = ITEM (1) ;
			return move (ret) ;
		}

		imports Matrix make_rotation (CREF<Quaternion<ITEM>> quat) {
			Matrix ret ;
			const auto r1x = quat[0] * ITEM (2) ;
			const auto r2x = quat[1] * ITEM (2) ;
			const auto r3x = quat[2] * ITEM (2) ;
			const auto r4x = quat[3] * ITEM (2) ;
			ret.at (0 ,0) = ITEM (1) - quat[1] * r2x - quat[2] * r3x ;
			ret.at (1 ,0) = quat[0] * r2x - quat[2] * r4x ;
			ret.at (2 ,0) = quat[0] * r3x + quat[1] * r4x ;
			ret.at (3 ,0) = ITEM (0) ;
			ret.at (0 ,1) = quat[0] * r2x + quat[2] * r4x ;
			ret.at (1 ,1) = ITEM (1) - quat[0] * r1x - quat[2] * r3x ;
			ret.at (2 ,1) = quat[1] * r3x - quat[0] * r4x ;
			ret.at (3 ,1) = ITEM (0) ;
			ret.at (0 ,2) = quat[0] * r3x - quat[1] * r4x ;
			ret.at (1 ,2) = quat[1] * r3x + quat[0] * r4x ;
			ret.at (2 ,2) = ITEM (1) - quat[0] * r1x - quat[1] * r2x ;
			ret.at (3 ,2) = ITEM (0) ;
			ret.at (0 ,3) = ITEM (0) ;
			ret.at (1 ,3) = ITEM (0) ;
			ret.at (2 ,3) = ITEM (0) ;
			ret.at (3 ,3) = ITEM (1) ;
			return move (ret) ;
		}

		imports Matrix make_translation (CREF<ITEM> tx ,CREF<ITEM> ty ,CREF<ITEM> tz) {
			const auto r1x = Vector<ITEM>::axis_x () ;
			const auto r2x = Vector<ITEM>::axis_y () ;
			const auto r3x = Vector<ITEM>::axis_z () ;
			const auto r4x = Vector<ITEM> (tx ,ty ,tz ,ITEM (1)) ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}

		imports Matrix make_view_xy (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy) {
			const auto r1x = Vector<ITEM>::axis_x () ;
			const auto r2x = Vector<ITEM>::axis_y () ;
			const auto r3x = Vector<ITEM>::axis_z () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_rotation (vx ,vy) * r5x ;
		}

		imports Matrix make_view_zx (CREF<Vector<ITEM>> vz ,CREF<Vector<ITEM>> vx) {
			const auto r1x = Vector<ITEM>::axis_y () ;
			const auto r2x = Vector<ITEM>::axis_z () ;
			const auto r3x = Vector<ITEM>::axis_x () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_rotation (vz ,vx) * r5x ;
		}

		imports Matrix make_view_yz (CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vz) {
			const auto r1x = Vector<ITEM>::axis_z () ;
			const auto r2x = Vector<ITEM>::axis_x () ;
			const auto r3x = Vector<ITEM>::axis_y () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_rotation (vy ,vz) * r5x ;
		}

		imports Matrix make_view_yx (CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vx) {
			const auto r1x = Vector<ITEM>::axis_y () ;
			const auto r2x = Vector<ITEM>::axis_x () ;
			const auto r3x = -Vector<ITEM>::axis_z () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_rotation (vy ,vx) * r5x ;
		}

		imports Matrix make_view_xz (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vz) {
			const auto r1x = Vector<ITEM>::axis_x () ;
			const auto r2x = -Vector<ITEM>::axis_z () ;
			const auto r3x = Vector<ITEM>::axis_y () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_rotation (vx ,vz) * r5x ;
		}

		imports Matrix make_view_zy (CREF<Vector<ITEM>> vz ,CREF<Vector<ITEM>> vy) {
			const auto r1x = -Vector<ITEM>::axis_z () ;
			const auto r2x = Vector<ITEM>::axis_y () ;
			const auto r3x = Vector<ITEM>::axis_x () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_rotation (vz ,vy) * r5x ;
		}

		imports Matrix make_perspective (CREF<ITEM> fx ,CREF<ITEM> fy ,CREF<ITEM> wx ,CREF<ITEM> wy) {
			assert (fx > ITEM (0)) ;
			assert (fy > ITEM (0)) ;
			const auto r1x = Vector<ITEM> (fx ,ITEM (0) ,ITEM (0) ,ITEM (0)) ;
			const auto r2x = Vector<ITEM> (ITEM (0) ,fy ,ITEM (0) ,ITEM (0)) ;
			const auto r3x = Vector<ITEM> (wx ,wy ,ITEM (0) ,ITEM (1)) ;
			const auto r4x = Vector<ITEM>::axis_z () ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}

		imports Matrix make_projection (CREF<Vector<ITEM>> normal ,CREF<ITEM> center ,CREF<Vector<ITEM>> light) {
			Matrix ret ;
			const auto r1x = normal.normalize () ;
			const auto r2x = light.normalize () ;
			const auto r3x = r1x * r2x ;
			ret.at (0 ,0) = r3x - r1x[0] * r2x[0] ;
			ret.at (1 ,0) = -r1x[1] * r2x[0] ;
			ret.at (2 ,0) = -r1x[2] * r2x[0] ;
			ret.at (3 ,0) = center * r2x[0] ;
			ret.at (0 ,1) = -r1x[0] * r2x[1] ;
			ret.at (1 ,1) = r3x - r1x[1] * r2x[1] ;
			ret.at (2 ,1) = -r1x[2] * r2x[1] ;
			ret.at (3 ,1) = center * r2x[1] ;
			ret.at (0 ,2) = -r1x[0] * r2x[2] ;
			ret.at (1 ,2) = -r1x[1] * r2x[2] ;
			ret.at (2 ,2) = r3x - r1x[2] * r2x[2] ;
			ret.at (3 ,2) = center * r2x[2] ;
			ret.at (0 ,3) = ITEM (0) ;
			ret.at (1 ,3) = ITEM (0) ;
			ret.at (2 ,3) = ITEM (0) ;
			ret.at (3 ,3) = r3x ;
			return move (ret) ;
		}

		imports Matrix make_cross_product (CREF<Vector<ITEM>> vx) {
			assert (vx[3] == ITEM (0)) ;
			const auto r1x = Vector<ITEM> (ITEM (0) ,vx[2] ,-vx[1] ,ITEM (0)) ;
			const auto r2x = Vector<ITEM> (-vx[2] ,ITEM (0) ,vx[0] ,ITEM (0)) ;
			const auto r3x = Vector<ITEM> (vx[1] ,-vx[0] ,ITEM (0) ,ITEM (1)) ;
			const auto r4x = Vector<ITEM> (ITEM (0) ,ITEM (0) ,ITEM (0) ,ITEM (1)) ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}

		imports Matrix make_symmetry (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy) {
			Matrix ret ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4))
					ret.at (j ,i) = vx[i] * vy[j] ;
			}
			return move (ret) ;
		}

		imports Matrix make_reflection (CREF<Vector<ITEM>> normal) {
			const auto r1x = normal.normalize () ;
			return identity () - make_symmetry (r1x ,r1x) * ITEM (2) ;
		}

		VREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
			assert (vbetween (x ,0 ,4)) ;
			assert (vbetween (y ,0 ,4)) ;
			return mMatrix[y * 4 + x] ;
		}

		inline RowProxy<VREF<Matrix> ,ITEM> operator[] (CREF<INDEX> y) leftvalue {
			return RowProxy<VREF<Matrix> ,ITEM> (VRef<Matrix>::reference (thiz) ,y) ;
		}

		CREF<ITEM> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
			assert (vbetween (x ,0 ,4)) ;
			assert (vbetween (y ,0 ,4)) ;
			return mMatrix[y * 4 + x] ;
		}

		inline RowProxy<CREF<Matrix> ,ITEM> operator[] (CREF<INDEX> y) const leftvalue {
			return RowProxy<CREF<Matrix> ,ITEM> (CRef<Matrix>::reference (thiz) ,y) ;
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

		FLAG hash () const {
			return operator_hash (mMatrix) ;
		}

		Matrix add (CREF<Matrix> that) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mMatrix[i] = mMatrix[i] + that.mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator+ (CREF<Matrix> that) const {
			return add (that) ;
		}

		void addto (CREF<Matrix> that) {
			for (auto &&i : iter (0 ,SIZE::expr))
				mMatrix[i] += that.mMatrix[i] ;
		}

		inline void operator+= (CREF<Matrix> that) {
			addto (that) ;
		}

		Matrix sub (CREF<Matrix> that) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mMatrix[i] = mMatrix[i] - that.mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator- (CREF<Matrix> that) const {
			return sub (that) ;
		}

		void subto (CREF<Matrix> that) {
			for (auto &&i : iter (0 ,SIZE::expr))
				mMatrix[i] -= that.mMatrix[i] ;
		}

		inline void operator-= (CREF<Matrix> that) {
			subto (that) ;
		}

		Matrix mul (CREF<ITEM> scale) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mMatrix[i] = mMatrix[i] * scale ;
			return move (ret) ;
		}

		inline Matrix operator* (CREF<ITEM> scale) const {
			return mul (scale) ;
		}

		inline friend Matrix operator* (CREF<ITEM> scale ,CREF<Matrix> that) {
			return that.mul (scale) ;
		}

		void multo (CREF<ITEM> scale) {
			for (auto &&i : iter (0 ,SIZE::expr))
				mMatrix[i] *= scale ;
		}

		inline void operator*= (CREF<ITEM> scale) {
			multo (scale) ;
		}

		Vector<ITEM> mul (CREF<Vector<ITEM>> that) const {
			Vector<ITEM> ret ;
			for (auto &&i : iter (0 ,4)) {
				const auto r1x = at (0 ,i) * that.at (0) ;
				const auto r2x = at (1 ,i) * that.at (1) ;
				const auto r3x = at (2 ,i) * that.at (2) ;
				const auto r4x = at (3 ,i) * that.at (3) ;
				ret.at (i) = r1x + r2x + r3x + r4x ;
			}
			return move (ret) ;
		}

		inline Vector<ITEM> operator* (CREF<Vector<ITEM>> that) const {
			return mul (that) ;
		}

		Matrix mul (CREF<Matrix> that) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4)) {
					const auto r1x = at (0 ,i) * that.at (j ,0) ;
					const auto r2x = at (1 ,i) * that.at (j ,1) ;
					const auto r3x = at (2 ,i) * that.at (j ,2) ;
					const auto r4x = at (3 ,i) * that.at (j ,3) ;
					ret.at (j ,i) = r1x + r2x + r3x + r4x ;
				}
			}
			return move (ret) ;
		}

		inline Matrix operator* (CREF<Matrix> that) const {
			return mul (that) ;
		}

		inline Matrix operator+ () const {
			return thiz ;
		}

		Matrix minus () const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mMatrix[i] = -mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator- () const {
			return minus () ;
		}

		Matrix transpose () const {
			Matrix ret ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4))
					ret.at (j ,i) = at (i ,j) ;
			}
			return move (ret) ;
		}

		Matrix triangular () const {
			Matrix ret = thiz ;
			for (auto &&i : iter (0 ,4)) {
				if ifswitch (TRUE) {
					INDEX ix = ret.find_abs_max_row (i) ;
					if (ix == i)
						discard ;
					for (auto &&j : iter (i ,4)) {
						const auto r1x = -ret.at (j ,i) ;
						ret.at (j ,i) = ret.at (j ,ix) ;
						ret.at (j ,ix) = r1x ;
					}
				}
				const auto r2x = MathProc::inverse (ret.at (i ,i)) ;
				if (r2x == ITEM (0))
					continue ;
				for (auto &&j : iter (i + 1 ,4)) {
					const auto r3x = ret.at (i ,j) * r2x ;
					for (auto &&k : iter (i + 1 ,4))
						ret.at (k ,j) -= r3x * ret.at (k ,i) ;
					ret.at (i ,j) = ITEM (0) ;
				}
			}
			return move (ret) ;
		}

		LENGTH rank () const {
			LENGTH ret = 0 ;
			const auto r1x = triangular () ;
			for (auto &&i : iter (0 ,4)) {
				const auto r2x = MathProc::inverse (r1x.at (i ,i)) ;
				ret += LENGTH (r2x == ITEM (0)) ;
			}
			ret = 4 - ret ;
			return move (ret) ;
		}

		ITEM det () const {
			const auto r1x = triangular () ;
			ITEM ret = ITEM (1) ;
			for (auto &&i : iter (0 ,4))
				ret *= r1x.at (i ,i) ;
			return move (ret) ;
		}

		Matrix inverse () const {
			Matrix ret ;
			const auto r1x = MathProc::inverse (det ()) ;
			assume (r1x != ITEM (0)) ;
			for (auto &&i : iter (0 ,4)) {
				INDEX ix = LENGTH (i == 0) ;
				INDEX iy = ix + 1 + LENGTH (i == 1) ;
				INDEX iz = iy + 1 + LENGTH (i == 2) ;
				for (auto &&j : iter (0 ,4)) {
					INDEX jx = LENGTH (j == 0) ;
					INDEX jy = jx + 1 + LENGTH (j == 1) ;
					INDEX jz = jy + 1 + LENGTH (j == 2) ;
					const auto r2x = at (jx ,ix) * (at (jy ,iy) * at (jz ,iz) - at (jz ,iy) * at (jy ,iz)) ;
					const auto r3x = at (jx ,iy) * (at (jy ,ix) * at (jz ,iz) - at (jz ,ix) * at (jy ,iz)) ;
					const auto r4x = at (jx ,iz) * (at (jy ,ix) * at (jz ,iy) - at (jz ,ix) * at (jy ,iy)) ;
					const auto r5x = r2x - r3x + r4x ;
					const auto r6x = invoke ([&] () {
						if ((i + j) % 2 == 0)
							return r5x ;
						return -r5x ;
					}) ;
					ret.at (i ,j) = r6x ;
				}
			}
			ret *= r1x ;
			if ifswitch (TRUE) {
				if ifnot (is_affine_matrix ())
					discard ;
				if (at (3 ,3) != ITEM (1))
					discard ;
				if ifnot (ret.is_affine_matrix ())
					discard ;
				const auto r7x = MathProc::inverse (ret.at (3 ,3)) ;
				ret *= r7x ;
				ret.at (3 ,3) = ITEM (1) ;
			}
			return move (ret) ;
		}

		ITEM trace () const {
			ITEM ret = ITEM (0) ;
			for (auto &&i : iter (0 ,4))
				ret += at (i ,i) ;
			return move (ret) ;
		}

		template <class ARG1 = DEPEND>
		MACRO_DECOMPOSE<ARG1> decompose () const {
			assert (is_affine_matrix ()) ;
			MACRO_DECOMPOSE<ARG1> ret ;
			const auto r1x = thiz * Vector<ITEM>::axis_x () ;
			const auto r2x = thiz * Vector<ITEM>::axis_y () ;
			const auto r3x = thiz * Vector<ITEM>::axis_z () ;
			ret.mShear = make_shear (r1x ,r2x ,r3x) ;
			const auto r4x = thiz * ret.mShear.inverse () ;
			const auto r5x = r4x * Vector<ITEM>::axis_x () ;
			const auto r6x = r4x * Vector<ITEM>::axis_y () ;
			const auto r7x = r4x * Vector<ITEM>::axis_z () ;
			const auto r8x = r4x * Vector<ITEM>::axis_w () ;
			const auto r9x = (r5x ^ r6x) * r7x ;
			const auto r10x = MathProc::sign (r9x) * MathProc::inverse (r8x[3]) ;
			const auto r11x = r5x.magnitude () * r10x ;
			const auto r12x = r6x.magnitude () * r10x ;
			const auto r13x = r7x.magnitude () * r10x ;
			ret.mScale = make_diag (r11x ,r12x ,r13x ,ITEM (1)) ;
			ret.mRotation = make_rotation (r5x ,r6x) ;
			const auto r14x = r8x.projection () ;
			ret.mTranslation = make_translation (r14x[0] ,r14x[1] ,r14x[2]) ;
			return move (ret) ;
		}

		template <class ARG1 = DEPEND>
		MACRO_SVD_DECOMPOSE<ARG1> singular () const {
			using R1X = typename MATRIX_SINGULAR_HELP<ITEM ,ALWAYS>::Singular ;
			MACRO_SVD_DECOMPOSE<ARG1> ret ;
			auto rax = R1X (thiz) ;
			rax.generate () ;
			ret.mU = rax.matrix_u () ;
			ret.mS = rax.matrix_s () ;
			ret.mV = rax.matrix_v () ;
			return move (ret) ;
		}

		Matrix pseudo_inverse () const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::expr))
				ret.mMatrix[i] = MathProc::inverse (mMatrix[i]) ;
			return move (ret) ;
		}

	private:
		BOOL is_affine_matrix () const {
			if (at (0 ,3) != ITEM (0))
				return FALSE ;
			if (at (1 ,3) != ITEM (0))
				return FALSE ;
			if (at (2 ,3) != ITEM (0))
				return FALSE ;
			if (MathProc::inverse (at (3 ,3)) == ITEM (0))
				return FALSE ;
			return TRUE ;
		}

		INDEX find_abs_max_row (CREF<INDEX> x) const {
			INDEX ret = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (x ,4)) {
				const auto r1x = MathProc::abs (at (x ,i)) ;
				if (ret != NONE)
					if (rax >= r1x)
						continue ;
				ret = i ;
				rax = r1x ;
			}
			return move (ret) ;
		}
	} ;

	struct DECOMPOSE {
		Matrix mTranslation ;
		Matrix mRotation ;
		Matrix mScale ;
		Matrix mShear ;
	} ;

	struct SVD_DECOMPOSE {
		Matrix mU ;
		Matrix mS ;
		Matrix mV ;
	} ;

	class DiagMatrix implement Matrix {
	public:
		implicit DiagMatrix () = delete ;

		explicit DiagMatrix (CREF<ITEM> x ,CREF<ITEM> y ,CREF<ITEM> z ,CREF<ITEM> w)
			:DiagMatrix (Matrix::make_diag (x ,y ,z ,w)) {}
	} ;

	class ShearMatrix implement Matrix {
	public:
		implicit ShearMatrix () = delete ;

		explicit ShearMatrix (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vz)
			:ShearMatrix (Matrix::make_shear (vx ,vy ,vz)) {}
	} ;

	class RotationMatrix implement Matrix {
	public:
		implicit RotationMatrix () = delete ;

		explicit RotationMatrix (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy)
			:RotationMatrix (Matrix::make_rotation (vx ,vy)) {}

		explicit RotationMatrix (CREF<Vector<ITEM>> normal ,CREF<ITEM> angle)
			:RotationMatrix (Matrix::make_rotation (normal ,angle)) {}

		explicit RotationMatrix (CREF<Quaternion<ITEM>> quat)
			:RotationMatrix (Matrix::make_rotation (quat)) {}
	} ;

	class TranslationMatrix implement Matrix {
	public:
		implicit TranslationMatrix () = delete ;

		explicit TranslationMatrix (CREF<ITEM> tx ,CREF<ITEM> ty ,CREF<ITEM> tz)
			:TranslationMatrix (Matrix::make_translation (tx ,ty ,tz)) {}
	} ;

	class PerspectiveMatrix implement Matrix {
	public:
		implicit PerspectiveMatrix () = delete ;

		explicit PerspectiveMatrix (CREF<ITEM> fx ,CREF<ITEM> fy ,CREF<ITEM> wx ,CREF<ITEM> wy)
			:PerspectiveMatrix (Matrix::make_perspective (fx ,fy ,wx ,wy)) {}
	} ;

	class ProjectionMatrix implement Matrix {
	public:
		implicit ProjectionMatrix () = delete ;

		explicit ProjectionMatrix (CREF<Vector<ITEM>> normal ,CREF<ITEM> center ,CREF<Vector<ITEM>> light)
			:ProjectionMatrix (Matrix::make_projection (normal ,center ,light)) {}
	} ;

	class CrossProductMatrix implement Matrix {
	public:
		implicit CrossProductMatrix () = delete ;

		explicit CrossProductMatrix (CREF<Vector<ITEM>> vx)
			:CrossProductMatrix (Matrix::make_cross_product (vx)) {}
	} ;

	class SymmetryMatrix implement Matrix {
	public:
		implicit SymmetryMatrix () = delete ;

		explicit SymmetryMatrix (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy)
			:SymmetryMatrix (Matrix::make_symmetry (vx ,vy)) {}
	} ;

	class ReflectionMatrix implement Matrix {
	public:
		implicit ReflectionMatrix () = delete ;

		explicit ReflectionMatrix (CREF<Vector<ITEM>> normal)
			:ReflectionMatrix (Matrix::make_reflection (normal)) {}
	} ;
} ;

template <class ITEM>
trait MATRIX_SINGULAR_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = typename MATRIX_HELP<ITEM ,ALWAYS>::SIZE ;
	using Vector = typename VECTOR_HELP<ITEM ,ALWAYS>::Vector ;
	using Matrix = typename MATRIX_HELP<ITEM ,ALWAYS>::Matrix ;

	using MAX_ITERATION = ENUMAS<VAL ,ENUMID<1024>> ;

	class Singular {
	protected:
		Matrix mA ;
		Matrix mB ;
		Matrix mC ;
		ARRAY4<Vector> mVV ;
		Matrix mQ ;
		Matrix mR ;
		Matrix mU ;
		Matrix mV ;
		Matrix mS ;

	public:
		implicit Singular () = delete ;

		explicit Singular (CREF<Matrix> a) {
			mA = a ;
		}

		Matrix matrix_u () const {
			return mU ;
		}

		Matrix matrix_s () const {
			return mS ;
		}

		Matrix matrix_v () const {
			return mV ;
		}

		void generate () {
			mU = Matrix::zero () ;
			mV = Matrix::zero () ;
			mS = Matrix::zero () ;
			mQ = mA * mA.transpose () ;
			mR = Matrix::identity () ;
			update_QR () ;
			for (auto &&i : iter (0 ,4))
				mS.at (i ,i) = MathProc::sqrt (mQ.at (i ,i)) ;
			INDEX ix = 3 ;
			for (auto &&i : iter (0 ,4)) {
				INDEX iy = 3 - i ;
				const auto r1x = MathProc::inverse (mS.at (iy ,iy)) ;
				if (r1x != ITEM (0))
					continue ;
				assume (ix >= 0) ;
				for (auto &&j : iter (0 ,4))
					mVV[ix][j] = mR.at (iy ,j) ;
				ix-- ;
			}
			while (TRUE) {
				if (ix < 0)
					break ;
				const auto r2x = ix ;
				mB = mA - Matrix::identity () * mS.at (ix ,ix) ;
				mC = Matrix::zero () ;
				for (auto &&j : iter (ix + 1 ,4))
					mC = mC + Matrix::make_symmetry (mVV[j] ,mVV[j]) ;
				mQ = mB * mB.transpose () + mC ;
				mR = Matrix::identity () ;
				update_QR () ;
				for (auto &&j : iter (0 ,4)) {
					INDEX iy = 3 - j ;
					const auto r3x = MathProc::inverse (mQ.at (iy ,iy)) ;
					if (r3x != ITEM (0))
						continue ;
					assume (ix >= 0) ;
					for (auto &&k : iter (0 ,4))
						mVV[ix][k] = mR.at (iy ,k) ;
					ix-- ;
				}
				if (ix == r2x)
					break ;
			}
			while (TRUE) {
				if (ix < 0)
					break ;
				mC = Matrix::zero () ;
				for (auto &&j : iter (ix + 1 ,4))
					mC = mC + Matrix::make_symmetry (mVV[j] ,mVV[j]) ;
				mQ = mC ;
				mR = Matrix::identity () ;
				update_QR () ;
				for (auto &&j : iter (0 ,4)) {
					INDEX iy = 3 - j ;
					const auto r4x = MathProc::inverse (mQ.at (iy ,iy)) ;
					if (r4x != ITEM (0))
						continue ;
					assume (ix >= 0) ;
					for (auto &&k : iter (0 ,4))
						mVV[ix][k] = mR.at (iy ,k) ;
					ix-- ;
				}
			}
			mV = Matrix (mVV[0] ,mVV[1] ,mVV[2] ,mVV[3]) ;
			mB = mA * mV * mS.pseudo_inverse () ;
			INDEX jx = 0 ;
			for (auto &&i : iter (0 ,4)) {
				const auto r5x = MathProc::inverse (mS.at (i ,i)) ;
				if (r5x == ITEM (0))
					continue ;
				assume (jx < 4) ;
				for (auto &&j : iter (0 ,4))
					mVV[jx][j] = mB.at (i ,j) ;
				jx++ ;
			}
			while (TRUE) {
				if (jx >= 4)
					break ;
				mC = Matrix::zero () ;
				for (auto &&j : iter (0 ,jx))
					mC = mC + Matrix::make_symmetry (mVV[j] ,mVV[j]) ;
				mQ = mC ;
				mR = Matrix::identity () ;
				update_QR () ;
				for (auto &&j : iter (0 ,4)) {
					INDEX iy = 3 - j ;
					const auto r6x = MathProc::inverse (mQ.at (iy ,iy)) ;
					if (r6x != ITEM (0))
						continue ;
					assume (jx < 4) ;
					for (auto &&k : iter (0 ,4))
						mVV[jx][k] = mR.at (iy ,k) ;
					jx++ ;
				}
			}
			mU = Matrix (mVV[0] ,mVV[1] ,mVV[2] ,mVV[3]) ;
		}

		void update_QR () {
			INDEX jx = 0 ;
			while (TRUE) {
				if (jx >= MAX_ITERATION::expr)
					break ;
				jx++ ;
				const auto r1x = find_abs_rot (mQ) ;
				INDEX ix = r1x[0] ;
				INDEX iy = r1x[1] ;
				const auto r2x = mQ.at (ix ,iy) ;
				const auto r3x = mQ.at (iy ,ix) ;
				const auto r4x = mQ.at (ix ,ix) ;
				const auto r5x = mQ.at (iy ,iy) ;
				if (MathProc::inverse (r2x) == ITEM (0))
					break ;
				const auto r6x = r4x - r5x ;
				const auto r7x = MathProc::square (r6x) + ITEM (4) * r2x * r3x ;
				assert (r7x >= ITEM (0)) ;
				const auto r8x = -MathProc::inverse (ITEM (2) * r2x) ;
				const auto r9x = MathProc::sign (r8x) ;
				const auto r10x = (-r6x + r9x * MathProc::sqrt (r7x)) * r8x ;
				assert (r10x >= ITEM (0)) ;
				const auto r11x = MathProc::inverse (ITEM (1) + MathProc::square (r10x)) ;
				const auto r12x = MathProc::sqrt (r11x) ;
				const auto r13x = r12x * r10x ;
				const auto r14x = matrix_r (ix ,iy ,r13x ,r12x) ;
				mQ = r14x.transpose () * mQ * r14x ;
				mR = mR * r14x ;
				mQ.at (ix ,iy) = ITEM (0) ;
			}
			for (auto &&i : iter (0 ,4)) {
				if ifswitch (TRUE) {
					const auto r15x = MathProc::inverse (mQ.at (i ,i)) ;
					if (r15x != ITEM (0))
						discard ;
					mQ.at (i ,i) = ITEM (0) ;
				}
				assume (mQ.at (i ,i) >= ITEM (0)) ;
				for (auto &&j : iter (0 ,4)) {
					if (i == j)
						continue ;
					mQ.at (i ,j) = ITEM (0) ;
				}
			}
			for (auto &&i : iter (0 ,4)) {
				const auto r16x = invoke ([&] () {
					INDEX ret = NONE ;
					auto rax = ITEM () ;
					for (auto &&j : iter (i ,4)) {
						if (ret != NONE)
							if (rax >= mQ.at (j ,j))
								continue ;
						ret = j ;
						rax = mQ.at (j ,j) ;
					}
					return move (ret) ;
				}) ;
				if (r16x == i)
					continue ;
				const auto r17x = matrix_sr (i ,r16x) ;
				mQ = r17x.transpose () * mQ * r17x ;
				mR = mR * r17x ;
			}
		}

		ARRAY2<INDEX> find_abs_rot (CREF<Matrix> b) const {
			ARRAY2<INDEX> ret ;
			ret[0] = NONE ;
			ret[1] = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4)) {
					if (i == j)
						continue ;
					const auto r1x = MathProc::abs (b.at (j ,i)) ;
					if (ret[0] != NONE)
						if (rax >= r1x)
							continue ;
					ret[0] = i ;
					ret[1] = j ;
					rax = r1x ;
				}
			}
			return move (ret) ;
		}

		Matrix matrix_r (CREF<INDEX> x ,CREF<INDEX> y ,CREF<ITEM> sin_ ,CREF<ITEM> cos_) const {
			Matrix ret = Matrix::identity () ;
			ret.at (x ,y) = -sin_ ;
			ret.at (y ,x) = sin_ ;
			ret.at (x ,x) = cos_ ;
			ret.at (y ,y) = cos_ ;
			return move (ret) ;
		}

		Matrix matrix_sr (CREF<INDEX> x ,CREF<INDEX> y) const {
			Matrix ret = Matrix::identity () ;
			ret.at (x ,y) = ITEM (1) ;
			ret.at (y ,x) = ITEM (1) ;
			ret.at (x ,x) = ITEM (0) ;
			ret.at (y ,y) = ITEM (0) ;
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
using Matrix = typename MATRIX_HELP<ITEM ,ALWAYS>::Matrix ;
template <class ITEM>
using DiagMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::DiagMatrix ;
template <class ITEM>
using ShearMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::ShearMatrix ;
template <class ITEM>
using RotationMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::RotationMatrix ;
template <class ITEM>
using TranslationMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::TranslationMatrix ;
template <class ITEM>
using PerspectiveMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::PerspectiveMatrix ;
template <class ITEM>
using ProjectionMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::ProjectionMatrix ;
template <class ITEM>
using CrossProductMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::CrossProductMatrix ;
template <class ITEM>
using SymmetryMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::SymmetryMatrix ;
template <class ITEM>
using ReflectionMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::ReflectionMatrix ;
} ;