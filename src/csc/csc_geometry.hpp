#pragma once

#ifndef __CSC_GEOMETRY__
#define __CSC_GEOMETRY__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"

namespace CSC {
namespace GEOMETRY {
template <class...>
trait MATRIX_HELP ;

template <class...>
trait VECTOR_HELP ;

template <class...>
trait VECTOR_SUPER_HELP ;

template <class ITEM>
trait VECTOR_SUPER_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = RANK4 ;

	class Vector {
	private:
		template <class...>
		friend trait VECTOR_HELP ;

	private:
		BoxBuffer<ITEM ,SIZE> mVector ;

	public:
		implicit Vector () noexcept {
			mVector = BoxBuffer<ITEM ,SIZE> (0) ;
		}
	} ;
} ;

template <class ITEM>
trait VECTOR_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = typename VECTOR_SUPER_HELP<ITEM ,ALWAYS>::SIZE ;
	using SUPER = typename VECTOR_SUPER_HELP<ITEM ,ALWAYS>::Vector ;

	template <class ARG1>
	using MACRO_Matrix = typename DEPENDENT<MATRIX_HELP<ITEM> ,ARG1>::Matrix ;

	class Vector extend SUPER {
	private:
		using SUPER::mVector ;

	public:
		implicit Vector () = default ;

		implicit Vector (RREF<BoxBuffer<ITEM ,SIZE>> that) {
			mVector = move (that) ;
		}

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
			for (auto &&i : iter (0 ,SIZE::value)) {
				if ifnot (operator_equal (mVector[i] ,that.mVector[i]))
					return FALSE ;
			}
			return TRUE ;
		}

		inline BOOL operator== (CREF<Vector> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Vector> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Vector> that) const {
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = operator_compr (mVector[i] ,that.mVector[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
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
			FLAG ret = hashcode () ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = operator_hash (mVector[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		Vector add (CREF<Vector> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::value))
				ret.mVector[i] = mVector[i] + that.mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator+ (CREF<Vector> that) const {
			return add (that) ;
		}

		void addto (CREF<Vector> that) {
			for (auto &&i : iter (0 ,SIZE::value))
				mVector[i] += that.mVector[i] ;
		}

		inline void operator+= (CREF<Vector> that) {
			addto (that) ;
		}

		Vector sub (CREF<Vector> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::value))
				ret.mVector[i] = mVector[i] - that.mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator- (CREF<Vector> that) const {
			return sub (that) ;
		}

		void subto (CREF<Vector> that) {
			for (auto &&i : iter (0 ,SIZE::value))
				mVector[i] -= that.mVector[i] ;
		}

		inline void operator-= (CREF<Vector> that) {
			subto (that) ;
		}

		Vector mul (CREF<ITEM> scale) const {
			Vector ret ;
			for (auto &&i : iter (0 ,SIZE::value))
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
			for (auto &&i : iter (0 ,SIZE::value))
				mVector[i] *= scale ;
		}

		inline void operator*= (CREF<ITEM> scale) {
			multo (scale) ;
		}

		ITEM dot (CREF<Vector> that) const {
			ITEM ret = ITEM (0) ;
			for (auto &&i : iter (0 ,SIZE::value))
				ret += mVector[i] * that.mVector[i] ;
			return move (ret) ;
		}

		inline ITEM operator* (CREF<Vector> that) const {
			return dot (that) ;
		}

		template <class ARG1 = void>
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

		template <class ARG1 = void>
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
			for (auto &&i : iter (0 ,SIZE::value))
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

template <class...>
trait QUATERNION_SUPER_HELP ;

template <class ITEM>
trait QUATERNION_SUPER_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = RANK4 ;

	class Quaternion {
	private:
		template <class...>
		friend trait QUATERNION_HELP ;

	private:
		BoxBuffer<ITEM ,SIZE> mQuaternion ;

	public:
		implicit Quaternion () noexcept {
			mQuaternion = BoxBuffer<ITEM ,SIZE>::make (0) ;
		}
	} ;
} ;

template <class ITEM>
trait QUATERNION_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = typename QUATERNION_SUPER_HELP<ITEM ,ALWAYS>::SIZE ;
	using SUPER = typename QUATERNION_SUPER_HELP<ITEM ,ALWAYS>::Quaternion ;

	template <class ARG1>
	using MACRO_Matrix = typename DEPENDENT<MATRIX_HELP<ITEM> ,ARG1>::Matrix ;

	class Quaternion {
	private:
		using SUPER::mQuaternion ;

	public:
		implicit Quaternion () = default ;

		implicit Quaternion (RREF<BoxBuffer<ITEM ,SIZE>> that) {
			mQuaternion = move (that) ;
			update () ;
		}

		explicit Quaternion (CREF<ITEM> x ,CREF<ITEM> y ,CREF<ITEM> z ,CREF<ITEM> w) {
			mQuaternion[0] = x ;
			mQuaternion[1] = y ;
			mQuaternion[2] = z ;
			mQuaternion[3] = w ;
			update () ;
		}

		template <class ARG1 = void>
		explicit Quaternion (CREF<MACRO_Matrix<ARG1>> that) {
			const auto r1x = that.decompose () ;
			const auto r2x = r1x[2] ;
			const auto r3x = invoke ([&] () {
				ARRAY4<ITEM> ret ;
				ret[0] = ITEM (1) + r2x[0][0] + r2x[1][1] + r2x[2][2] ;
				ret[1] = ITEM (1) + r2x[0][0] - r2x[1][1] - r2x[2][2] ;
				ret[2] = ITEM (1) - r2x[0][0] + r2x[1][1] - r2x[2][2] ;
				ret[3] = ITEM (1) - r2x[0][0] - r2x[1][1] + r2x[2][2] ;
				return move (ret) ;
			}) ;
			const auto r4x = invoke ([&] () {
				INDEX ret = NONE ;
				auto rax = ITEM () ;
				for (auto &&i : iter (0 ,4)) {
					if (ret != NONE)
						if (rax >= r3x[i])
							continue ;
					ret = i ;
					rax = r3x[i] ;
				}
				return move (ret) ;
			}) ;
			const auto r5x = MathProc::inverse (ITEM (2) * MathProc::sqrt (r3x[r4x])) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (r4x == 0)
					discard ;
				mQuaternion[0] = (r2x[2][1] - r2x[1][2]) * r5x ;
				mQuaternion[1] = (r2x[0][2] - r2x[2][0]) * r5x ;
				mQuaternion[2] = (r2x[1][0] - r2x[0][1]) * r5x ;
				mQuaternion[3] = r3x[0] * r5x ;
			}
			if ifswitch (eax) {
				if ifnot (r4x == 1)
					discard ;
				mQuaternion[0] = r3x[1] * r5x ;
				mQuaternion[1] = (r2x[1][0] + r2x[0][1]) * r5x ;
				mQuaternion[2] = (r2x[0][2] + r2x[2][0]) * r5x ;
				mQuaternion[3] = (r2x[2][1] - r2x[1][2]) * r5x ;
			}
			if ifswitch (eax) {
				if ifnot (r4x == 2)
					discard ;
				mQuaternion[0] = (r2x[1][0] + r2x[0][1]) * r5x ;
				mQuaternion[1] = r3x[2] * r5x ;
				mQuaternion[2] = (r2x[2][1] + r2x[1][2]) * r5x ;
				mQuaternion[3] = (r2x[0][2] - r2x[2][0]) * r5x ;
			}
			if ifswitch (eax) {
				if ifnot (r4x == 3)
					discard ;
				mQuaternion[0] = (r2x[0][2] + r2x[2][0]) * r5x ;
				mQuaternion[1] = (r2x[2][1] + r2x[1][2]) * r5x ;
				mQuaternion[2] = r3x[3] * r5x ;
				mQuaternion[3] = (r2x[1][0] - r2x[0][1]) * r5x ;
			}
			update () ;
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

	private:
		void update () {
			const auto r1x = MathProc::square (mQuaternion[0]) + MathProc::square (mQuaternion[1]) + MathProc::square (mQuaternion[2]) + MathProc::square (mQuaternion[3]) ;
			const auto r2x = MathProc::sqrt (r2x) ;
			const auto r3x = MathProc::inverse (r3x) ;
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
trait MATRIX_SUPER_HELP ;

template <class ITEM>
trait MATRIX_SUPER_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = ENUMAS<VAL ,ENUMID<16>> ;

	class Matrix {
	private:
		template <class...>
		friend trait MATRIX_HELP ;

	private:
		BoxBuffer<ITEM ,SIZE> mMatrix ;

	public:
		implicit Matrix () noexcept {
			mMatrix = BoxBuffer<ITEM ,SIZE> (0) ;
		}
	} ;
} ;

template <class ITEM>
trait MATRIX_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	using SIZE = typename MATRIX_SUPER_HELP<ITEM ,ALWAYS>::SIZE ;
	using SUPER = typename MATRIX_SUPER_HELP<ITEM ,ALWAYS>::Matrix ;

	using SINGULAR_MAX_ITERATION = ENUMAS<VAL ,ENUMID<1024>> ;

	class Matrix extend SUPER {
	private:
		using SUPER::mMatrix ;

	public:
		implicit Matrix () = default ;

		implicit Matrix (RREF<BoxBuffer<ITEM ,SIZE>> that) {
			mMatrix = move (that) ;
		}

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

		imports Matrix make_rotation (CREF<Vector<ITEM>> normal ,CREF<ITEM> angle) {
			Matrix ret ;
			const auto r1x = normal.normalize () ;
			const auto r2x = MathProc::cos (angle) ;
			const auto r3x = r1x * MathProc::sin (angle) ;
			const auto r4x = r1x * (ITEM (1) - r2x) ;
			ret[0][0] = r1x[0] * r4x[0] + r2x ;
			ret[0][1] = r1x[0] * r4x[1] - r3x[2] ;
			ret[0][2] = r1x[0] * r4x[2] + r3x[1] ;
			ret[0][3] = ITEM (0) ;
			ret[1][0] = r1x[1] * r4x[0] + r3x[2] ;
			ret[1][1] = r1x[1] * r4x[1] + r2x ;
			ret[1][2] = r1x[1] * r4x[2] - r3x[0] ;
			ret[1][3] = ITEM (0) ;
			ret[2][0] = r1x[2] * r4x[0] - r3x[1] ;
			ret[2][1] = r1x[2] * r4x[1] + r3x[0] ;
			ret[2][2] = r1x[2] * r4x[2] + r2x ;
			ret[2][3] = ITEM (0) ;
			ret[3][0] = ITEM (0) ;
			ret[3][1] = ITEM (0) ;
			ret[3][2] = ITEM (0) ;
			ret[3][3] = ITEM (1) ;
			return move (ret) ;
		}

		imports Matrix make_rotation (CREF<Quaternion<ITEM>> q) {
			Matrix ret ;
			const auto r1x = q[0] * ITEM (2) ;
			const auto r2x = q[1] * ITEM (2) ;
			const auto r3x = q[2] * ITEM (2) ;
			const auto r4x = q[3] * ITEM (2) ;
			ret[0][0] = ITEM (1) - q[1] * r2x - q[2] * r3x ;
			ret[0][1] = q[0] * r2x - q[2] * r4x ;
			ret[0][2] = q[0] * r3x + q[1] * r4x ;
			ret[0][3] = ITEM (0) ;
			ret[1][0] = q[0] * r2x + q[2] * r4x ;
			ret[1][1] = ITEM (1) - q[0] * r1x - q[2] * r3x ;
			ret[1][2] = q[1] * r3x - q[0] * r4x ;
			ret[1][3] = ITEM (0) ;
			ret[2][0] = q[0] * r3x - q[1] * r4x ;
			ret[2][1] = q[1] * r3x + q[0] * r4x ;
			ret[2][2] = ITEM (1) - q[0] * r1x - q[1] * r2x ;
			ret[2][3] = ITEM (0) ;
			ret[3][0] = ITEM (0) ;
			ret[3][1] = ITEM (0) ;
			ret[3][2] = ITEM (0) ;
			ret[3][3] = ITEM (1) ;
			return move (ret) ;
		}

		imports Matrix make_translation (CREF<ITEM> tx ,CREF<ITEM> ty ,CREF<ITEM> tz) {
			const auto r1x = Vector<ITEM>::axis_x () ;
			const auto r2x = Vector<ITEM>::axis_y () ;
			const auto r3x = Vector<ITEM>::axis_z () ;
			const auto r4x = Vector<ITEM> (tx ,ty ,tz ,ITEM (1)) ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}

		imports Matrix make_view (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy) {
			const auto r1x = vx.normalize () ;
			const auto r2x = vy.normalize () ;
			const auto r3x = (r1x ^ r2x).normalize () ;
			const auto r4x = (r3x ^ r1x).normalize () ;
			const auto r5x = Vector<ITEM>::axis_w () ;
			return Matrix (r1x ,r4x ,r3x ,r5x) ;
		}

		imports Matrix make_view_xy (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vy) {
			const auto r1x = Vector<ITEM>::axis_x () ;
			const auto r2x = Vector<ITEM>::axis_y () ;
			const auto r3x = Vector<ITEM>::axis_z () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_view (vx ,vy) * r5x ;
		}

		imports Matrix make_view_zx (CREF<Vector<ITEM>> vz ,CREF<Vector<ITEM>> vx) {
			const auto r1x = Vector<ITEM>::axis_y () ;
			const auto r2x = Vector<ITEM>::axis_z () ;
			const auto r3x = Vector<ITEM>::axis_x () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_view (vz ,vx) * r5x ;
		}

		imports Matrix make_view_yz (CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vz) {
			const auto r1x = Vector<ITEM>::axis_z () ;
			const auto r2x = Vector<ITEM>::axis_x () ;
			const auto r3x = Vector<ITEM>::axis_y () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_view (vy ,vz) * r5x ;
		}

		imports Matrix make_view_yx (CREF<Vector<ITEM>> vy ,CREF<Vector<ITEM>> vx) {
			const auto r1x = Vector<ITEM>::axis_y () ;
			const auto r2x = Vector<ITEM>::axis_x () ;
			const auto r3x = -Vector<ITEM>::axis_z () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_view (vy ,vx) * r5x ;
		}

		imports Matrix make_view_xz (CREF<Vector<ITEM>> vx ,CREF<Vector<ITEM>> vz) {
			const auto r1x = Vector<ITEM>::axis_x () ;
			const auto r2x = -Vector<ITEM>::axis_z () ;
			const auto r3x = Vector<ITEM>::axis_y () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_view (vx ,vz) * r5x ;
		}

		imports Matrix make_view_zy (CREF<Vector<ITEM>> vz ,CREF<Vector<ITEM>> vy) {
			const auto r1x = -Vector<ITEM>::axis_z () ;
			const auto r2x = Vector<ITEM>::axis_y () ;
			const auto r3x = Vector<ITEM>::axis_x () ;
			const auto r4x = Vector<ITEM>::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make_view (vz ,vy) * r5x ;
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
			ret[0][0] = r3x - r1x[0] * r2x[0] ;
			ret[0][1] = -r1x[1] * r2x[0] ;
			ret[0][2] = -r1x[2] * r2x[0] ;
			ret[0][3] = center * r2x[0] ;
			ret[1][0] = -r1x[0] * r2x[1] ;
			ret[1][1] = r3x - r1x[1] * r2x[1] ;
			ret[1][2] = -r1x[2] * r2x[1] ;
			ret[1][3] = center * r2x[1] ;
			ret[2][0] = -r1x[0] * r2x[2] ;
			ret[2][1] = -r1x[1] * r2x[2] ;
			ret[2][2] = r3x - r1x[2] * r2x[2] ;
			ret[2][3] = center * r2x[2] ;
			ret[3][0] = ITEM (0) ;
			ret[3][1] = ITEM (0) ;
			ret[3][2] = ITEM (0) ;
			ret[3][3] = r3x ;
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
					ret[i][j] = vx[i] * vy[j] ;
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
			for (auto &&i : iter (0 ,SIZE::value)) {
				if ifnot (operator_equal (mMatrix[i] ,that.mMatrix[i]))
					return FALSE ;
			}
			return TRUE ;
		}

		inline BOOL operator== (CREF<Matrix> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Matrix> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Matrix> that) const {
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = operator_compr (mMatrix[i] ,that.mMatrix[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			return ZERO ;
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
			FLAG ret = hashcode () ;
			for (auto &&i : iter (0 ,SIZE::value)) {
				const auto r1x = operator_hash (mMatrix[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		Matrix add (CREF<Matrix> that) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::value))
				ret.mMatrix[i] = mMatrix[i] + that.mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator+ (CREF<Matrix> that) const {
			return add (that) ;
		}

		void addto (CREF<Matrix> that) {
			for (auto &&i : iter (0 ,SIZE::value))
				mMatrix[i] += that.mMatrix[i] ;
		}

		inline void operator+= (CREF<Matrix> that) {
			addto (that) ;
		}

		Matrix sub (CREF<Matrix> that) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::value))
				ret.mMatrix[i] = mMatrix[i] - that.mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator- (CREF<Matrix> that) const {
			return sub (that) ;
		}

		void subto (CREF<Matrix> that) {
			for (auto &&i : iter (0 ,SIZE::value))
				mMatrix[i] -= that.mMatrix[i] ;
		}

		inline void operator-= (CREF<Matrix> that) {
			subto (that) ;
		}

		Matrix mul (CREF<ITEM> scale) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,SIZE::value))
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
			for (auto &&i : iter (0 ,SIZE::value))
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
			for (auto &&i : iter (0 ,SIZE::value))
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
			dynamic_assert (r1x != ITEM (0)) ;
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

		//@info: 3-translation * 2-rotation * 1-scale * 0-shear
		ARRAY4<Matrix> decompose () const {
			assert (is_affine_matrix ()) ;
			ARRAY4<Matrix> ret ;
			const auto r1x = thiz * Vector<ITEM>::axis_x () ;
			const auto r2x = thiz * Vector<ITEM>::axis_y () ;
			const auto r3x = thiz * Vector<ITEM>::axis_z () ;
			ret[0] = make_shear (r1x ,r2x ,r3x) ;
			const auto r4x = thiz * ret[0].inverse () ;
			const auto r5x = r4x * Vector<ITEM>::axis_x () ;
			const auto r6x = r4x * Vector<ITEM>::axis_y () ;
			const auto r7x = r4x * Vector<ITEM>::axis_z () ;
			const auto r8x = r4x * Vector<ITEM>::axis_w () ;
			const auto r9x = (r5x ^ r6x) * r7x ;
			const auto r10x = MathProc::sign (r9x) * MathProc::inverse (r8x[3]) ;
			const auto r11x = r5x.magnitude () * r10x ;
			const auto r12x = r6x.magnitude () * r10x ;
			const auto r13x = r7x.magnitude () * r10x ;
			ret[1] = make_diag (r11x ,r12x ,r13x ,ITEM (1)) ;
			ret[2] = make_view (r5x ,r6x) ;
			const auto r14x = r8x.projection () ;
			ret[3] = make_translation (r14x[0] ,r14x[1] ,r14x[2]) ;
			return move (ret) ;
		}

		ARRAY3<Matrix> singular () const {
			assert (is_affine_matrix ()) ;
			ARRAY3<Matrix> ret ;
			const auto r1x = transpose () * thiz ;
			const auto r2x = thiz * transpose () ;
			const auto r3x = r1x.singular_decompose () ;
			ret[1] = r3x[1].singular_value () ;
			unimplemented () ;
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

		BOOL is_zero_matrix () const {
			const auto r1x = ITEM (SINGLE_EPS) ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4)) {
					if (MathProc::abs (at (i ,j)) >= r1x)
						return FALSE ;
				}
			}
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

		Matrix singular_value () const {
			const auto r3x = invoke ([&] () {
				ARRAY4<ITEM> ret ;
				INDEX ix = 0 ;
				ret[0] = ITEM (0) ;
				ret[1] = ITEM (0) ;
				ret[2] = ITEM (0) ;
				ret[3] = ITEM (0) ;
				for (auto &&i : iter (0 ,4)) {
					const auto r1x = invoke ([&] () {
						const auto r2x = MathProc::abs (at (i ,i)) ;
						if (MathProc::inverse (r2x) == ITEM (0))
							return ITEM (0) ;
						return MathProc::sqrt (r2x) ;
					}) ;
					const auto r4x = invoke ([&] () {
						for (auto &&j : iter (0 ,ix)) {
							const auto r6x = MathProc::abs (r1x - ret[j]) ;
							if (MathProc::inverse (r6x) == ITEM (0))
								return NONE ;
							if (r1x > ret[j])
								return j ;
						}
						return ix ;
					}) ;
					if (r4x == NONE)
						continue ;
					for (auto &&j : iter (r4x ,ix)) {
						INDEX jx = ix - (j - r4x) - 1 ;
						INDEX jy = jx + 1 ;
						ret[jy] = ret[jx] ;
					}
					ret[r4x] = r1x ;
					ix++ ;
				}
				return move (ret) ;
			}) ;
			return make_diag (r3x[0] ,r3x[1] ,r3x[2] ,r3x[3]) ;
		}

		INDEX find_abs_max_diag (CREF<INDEX> x) const {
			INDEX ret = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (x ,4)) {
				const auto r1x = MathProc::abs (at (i ,i)) ;
				if (ret != NONE)
					if (rax >= r1x)
						continue ;
				ret = i ;
				rax = r1x ;
			}
			return move (ret) ;
		}

		ARRAY2<Matrix> singular_decompose () const {
			ARRAY2<Matrix> ret ;
			ret[0] = identity () ;
			ret[1] = thiz ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (jx >= SINGULAR_MAX_ITERATION::value)
					break ;
				jx++ ;
				const auto r1x = ret[1].find_abs_rot () ;
				INDEX ix = r1x[0] ;
				INDEX iy = r1x[1] ;
				const auto r10x = ret[1].at (ix ,iy) ;
				const auto r11x = ret[1].at (iy ,ix) ;
				const auto r12x = ret[1].at (ix ,ix) ;
				const auto r13x = ret[1].at (iy ,iy) ;
				if (MathProc::inverse (r10x) == ITEM (0))
					break ;
				const auto r2x = r12x - r13x ;
				const auto r3x = MathProc::square (r2x) + ITEM (4) * r10x * r11x ;
				assert (r3x >= ITEM (0)) ;
				const auto r20x = -MathProc::inverse (ITEM (2) * r10x) ;
				const auto r21x = MathProc::sign (r20x) ;
				const auto r4x = (-r2x + r21x * MathProc::sqrt (r3x)) * r20x ;
				assert (r4x >= ITEM (0)) ;
				const auto r8x = MathProc::inverse (ITEM (1) + MathProc::square (r4x)) ;
				const auto r5x = MathProc::sqrt (r8x) ;
				const auto r6x = r5x * r4x ;
				const auto r7x = invoke ([&] () {
					Matrix ret = identity () ;
					ret.at (ix ,iy) = -r6x ;
					ret.at (iy ,ix) = r6x ;
					ret.at (ix ,ix) = r5x ;
					ret.at (iy ,iy) = r5x ;
					return move (ret) ;
				}) ;
				ret[0] = ret[0] * r7x ;
				ret[1] = r7x.transpose () * ret[1] * r7x ;
				ret[1].at (ix ,iy) = ITEM (0) ;
			}
			return move (ret) ;
		}

		ARRAY2<INDEX> find_abs_rot () const {
			ARRAY2<INDEX> ret ;
			ret[0] = NONE ;
			ret[1] = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4)) {
					if (i == j)
						continue ;
					const auto r1x = MathProc::abs (at (j ,i)) ;
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
	} ;
} ;

template <class ITEM>
using Matrix = typename MATRIX_HELP<ITEM ,ALWAYS>::Matrix ;
} ;
} ;

namespace CSC {
using namespace GEOMETRY ;
} ;