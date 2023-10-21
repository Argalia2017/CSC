#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
trait VECTOR_HELP ;

template <class...>
trait QUATERNION_HELP ;

template <class...>
trait MATRIX_HELP ;

template <class ITEM>
trait VECTOR_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	class Vector ;

	using Matrix = typename MATRIX_HELP<ITEM ,ALWAYS>::Matrix ;
	using RANK = RANK4 ;

	class Vector {
	protected:
		BoxBuffer<ITEM ,RANK> mVector ;

	public:
		implicit Vector () = default ;

		explicit Vector (CREF<ARRAY3<ITEM>> xyz_ ,CREF<ITEM> w_) {
			mVector[0] = xyz_[0] ;
			mVector[1] = xyz_[1] ;
			mVector[2] = xyz_[2] ;
			mVector[3] = w_ ;
		}

		explicit Vector (CREF<ITEM> x_ ,CREF<ITEM> y_ ,CREF<ITEM> z_ ,CREF<ITEM> w_) {
			mVector[0] = x_ ;
			mVector[1] = y_ ;
			mVector[2] = z_ ;
			mVector[3] = w_ ;
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

		VREF<ITEM> at (CREF<INDEX> y_) leftvalue {
			assert (vbetween (y_ ,0 ,4)) ;
			return mVector[y_] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> y_) leftvalue {
			return at (y_) ;
		}

		CREF<ITEM> at (CREF<INDEX> y_) const leftvalue {
			assert (vbetween (y_ ,0 ,4)) ;
			return mVector[y_] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> y_) const leftvalue {
			return at (y_) ;
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
			for (auto &&i : iter (0 ,RANK::expr))
				ret.mVector[i] = mVector[i] + that.mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator+ (CREF<Vector> that) const {
			return add (that) ;
		}

		void add_with (CREF<Vector> that) {
			for (auto &&i : iter (0 ,RANK::expr))
				mVector[i] += that.mVector[i] ;
		}

		inline void operator+= (CREF<Vector> that) {
			add_with (that) ;
		}

		Vector sub (CREF<Vector> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,RANK::expr))
				ret.mVector[i] = mVector[i] - that.mVector[i] ;
			return move (ret) ;
		}

		inline Vector operator- (CREF<Vector> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Vector> that) {
			for (auto &&i : iter (0 ,RANK::expr))
				mVector[i] -= that.mVector[i] ;
		}

		inline void operator-= (CREF<Vector> that) {
			sub_with (that) ;
		}

		Vector mul (CREF<ITEM> scale) const {
			Vector ret ;
			for (auto &&i : iter (0 ,RANK::expr))
				ret.mVector[i] = mVector[i] * scale ;
			return move (ret) ;
		}

		inline Vector operator* (CREF<ITEM> scale) const {
			return mul (scale) ;
		}

		inline friend Vector operator* (CREF<ITEM> scale ,CREF<Vector> that) {
			return that.mul (scale) ;
		}

		void mul_with (CREF<ITEM> scale) {
			for (auto &&i : iter (0 ,RANK::expr))
				mVector[i] *= scale ;
		}

		inline void operator*= (CREF<ITEM> scale) {
			mul_with (scale) ;
		}

		ITEM dot (CREF<Vector> that) const {
			ITEM ret = ITEM (0) ;
			for (auto &&i : iter (0 ,RANK::expr))
				ret += mVector[i] * that.mVector[i] ;
			return move (ret) ;
		}

		inline ITEM operator* (CREF<Vector> that) const {
			return dot (that) ;
		}

		Vector mul (CREF<Matrix> that) const {
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

		inline Vector operator* (CREF<Matrix> that) const {
			return mul (that) ;
		}

		Vector cross (CREF<Vector> that) const {
			assert (mVector[3] == ITEM (0)) ;
			assert (that.mVector[3] == ITEM (0)) ;
			Vector ret ;
			ret.mVector[0] = mVector[1] * that.mVector[2] - mVector[2] * that.mVector[1] ;
			ret.mVector[1] = mVector[2] * that.mVector[0] - mVector[0] * that.mVector[2] ;
			ret.mVector[2] = mVector[0] * that.mVector[1] - mVector[1] * that.mVector[0] ;
			ret.mVector[3] = 0 ;
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
			for (auto &&i : iter (0 ,RANK::expr))
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
trait QUATERNION_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	class Quaternion ;

	using Vector = typename VECTOR_HELP<ITEM ,ALWAYS>::Vector ;
	using Matrix = typename MATRIX_HELP<ITEM ,ALWAYS>::Matrix ;
	using RANK = RANK4 ;

	class Quaternion {
	protected:
		BoxBuffer<ITEM ,RANK> mQuaternion ;

	public:
		implicit Quaternion () = default ;

		explicit Quaternion (CREF<ITEM> x_ ,CREF<ITEM> y_ ,CREF<ITEM> z_ ,CREF<ITEM> w_) {
			mQuaternion[0] = x_ ;
			mQuaternion[1] = y_ ;
			mQuaternion[2] = z_ ;
			mQuaternion[3] = w_ ;
			update_normalize () ;
		}

		explicit Quaternion (CREF<Matrix> that) {
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
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (r3x == 0)
					discard ;
				mQuaternion[0] = (r1x.at (1 ,2) - r1x.at (2 ,1)) * r4x ;
				mQuaternion[1] = (r1x.at (2 ,0) - r1x.at (0 ,2)) * r4x ;
				mQuaternion[2] = (r1x.at (0 ,1) - r1x.at (1 ,0)) * r4x ;
				mQuaternion[3] = r2x[0] * r4x ;
			}
			if ifswitch (act) {
				if ifnot (r3x == 1)
					discard ;
				mQuaternion[0] = r2x[1] * r4x ;
				mQuaternion[1] = (r1x.at (0 ,1) + r1x.at (1 ,0)) * r4x ;
				mQuaternion[2] = (r1x.at (2 ,0) + r1x.at (0 ,2)) * r4x ;
				mQuaternion[3] = (r1x.at (1 ,2) - r1x.at (2 ,1)) * r4x ;
			}
			if ifswitch (act) {
				if ifnot (r3x == 2)
					discard ;
				mQuaternion[0] = (r1x.at (0 ,1) + r1x.at (1 ,0)) * r4x ;
				mQuaternion[1] = r2x[2] * r4x ;
				mQuaternion[2] = (r1x.at (1 ,2) + r1x.at (2 ,1)) * r4x ;
				mQuaternion[3] = (r1x.at (2 ,0) - r1x.at (0 ,2)) * r4x ;
			}
			if ifswitch (act) {
				if ifnot (r3x == 3)
					discard ;
				mQuaternion[0] = (r1x.at (2 ,0) + r1x.at (0 ,2)) * r4x ;
				mQuaternion[1] = (r1x.at (1 ,2) + r1x.at (2 ,1)) * r4x ;
				mQuaternion[2] = r2x[3] * r4x ;
				mQuaternion[3] = (r1x.at (0 ,1) - r1x.at (1 ,0)) * r4x ;
			}
			update_normalize () ;
		}

		Vector axis () const {
			const auto r1x = Vector (mQuaternion[0] ,mQuaternion[1] ,mQuaternion[2] ,ITEM (0)) * MathProc::sign (mQuaternion[3]) ;
			return r1x.normalize () ;
		}

		ITEM angle () const {
			const auto r1x = Vector (mQuaternion[0] ,mQuaternion[1] ,mQuaternion[2] ,ITEM (0)) ;
			const auto r2x = r1x.magnitude () * MathProc::sign (mQuaternion[3]) ;
			const auto r3x = MathProc::arctan (r2x ,MathProc::abs (mQuaternion[3])) ;
			return r3x * ITEM (2) ;
		}

		Vector angle_axis () const {
			const auto r1x = angle () ;
			if (MathProc::inverse (r1x) == ITEM (0))
				return Vector::zero () ;
			return axis () * r1x ;
		}

		Matrix matrix () const {
			Matrix ret ;
			const auto r1x = at (0) ;
			const auto r2x = at (1) ;
			const auto r3x = at (2) ;
			const auto r4x = at (3) ;
			const auto r5x = r1x * ITEM (2) ;
			const auto r6x = r2x * ITEM (2) ;
			const auto r7x = r3x * ITEM (2) ;
			const auto r8x = r4x * ITEM (2) ;
			ret.at (0 ,0) = ITEM (1) - r2x * r6x - r3x * r7x ;
			ret.at (1 ,0) = r1x * r6x - r3x * r8x ;
			ret.at (2 ,0) = r1x * r7x + r2x * r8x ;
			ret.at (3 ,0) = ITEM (0) ;
			ret.at (0 ,1) = r1x * r6x + r3x * r8x ;
			ret.at (1 ,1) = ITEM (1) - r1x * r5x - r3x * r7x ;
			ret.at (2 ,1) = r2x * r7x - r1x * r8x ;
			ret.at (3 ,1) = ITEM (0) ;
			ret.at (0 ,2) = r1x * r7x - r2x * r8x ;
			ret.at (1 ,2) = r2x * r7x + r1x * r8x ;
			ret.at (2 ,2) = ITEM (1) - r1x * r5x - r2x * r6x ;
			ret.at (3 ,2) = ITEM (0) ;
			ret.at (0 ,3) = ITEM (0) ;
			ret.at (1 ,3) = ITEM (0) ;
			ret.at (2 ,3) = ITEM (0) ;
			ret.at (3 ,3) = ITEM (1) ;
			return move (ret) ;
		}

		CREF<ITEM> at (CREF<INDEX> y_) const leftvalue {
			assert (vbetween (y_ ,0 ,4)) ;
			return mQuaternion[y_] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> y_) const leftvalue {
			return at (y_) ;
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
trait MATRIX_HELP<ITEM ,REQUIRE<IS_FLOAT<ITEM>>> {
	class Matrix ;
	struct DECOMPOSE ;
	struct SINGULAR ;

	using Vector = typename VECTOR_HELP<ITEM ,ALWAYS>::Vector ;
	using Quaternion = typename QUATERNION_HELP<ITEM ,ALWAYS>::Quaternion ;
	using RANK = ENUMAS<VAL ,16> ;

	class Matrix {
	protected:
		BoxBuffer<ITEM ,RANK> mMatrix ;

	public:
		implicit Matrix () = default ;

		explicit Matrix (CREF<Vector> vx ,CREF<Vector> vy ,CREF<Vector> vz ,CREF<Vector> vw) {
			for (auto &&i : iter (0 ,4)) {
				at (0 ,i) = vx[i] ;
				at (1 ,i) = vy[i] ;
				at (2 ,i) = vz[i] ;
				at (3 ,i) = vw[i] ;
			}
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

		VREF<ITEM> at (CREF<PIXEL> xy) leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline VREF<ITEM> operator[] (CREF<PIXEL> xy) leftvalue {
			return at (xy) ;
		}

		VREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) leftvalue {
			assert (vbetween (x_ ,0 ,4)) ;
			assert (vbetween (y_ ,0 ,4)) ;
			return mMatrix[y_ * 4 + x_] ;
		}

		inline RowProxy<VREF<Matrix> ,ITEM> operator[] (CREF<INDEX> y_) leftvalue {
			return RowProxy<VREF<Matrix> ,ITEM> (VRef<Matrix>::reference (thiz) ,y_) ;
		}

		CREF<ITEM> at (CREF<PIXEL> xy) const leftvalue {
			return at (xy.x ,xy.y) ;
		}

		inline CREF<ITEM> operator[] (CREF<PIXEL> xy) const leftvalue {
			return at (xy) ;
		}

		CREF<ITEM> at (CREF<INDEX> x_ ,CREF<INDEX> y_) const leftvalue {
			assert (vbetween (x_ ,0 ,4)) ;
			assert (vbetween (y_ ,0 ,4)) ;
			return mMatrix[y_ * 4 + x_] ;
		}

		inline RowProxy<CREF<Matrix> ,ITEM> operator[] (CREF<INDEX> y_) const leftvalue {
			return RowProxy<CREF<Matrix> ,ITEM> (CRef<Matrix>::reference (thiz) ,y_) ;
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
			for (auto &&i : iter (0 ,RANK::expr))
				ret.mMatrix[i] = mMatrix[i] + that.mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator+ (CREF<Matrix> that) const {
			return add (that) ;
		}

		void add_with (CREF<Matrix> that) {
			for (auto &&i : iter (0 ,RANK::expr))
				mMatrix[i] += that.mMatrix[i] ;
		}

		inline void operator+= (CREF<Matrix> that) {
			add_with (that) ;
		}

		Matrix sub (CREF<Matrix> that) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,RANK::expr))
				ret.mMatrix[i] = mMatrix[i] - that.mMatrix[i] ;
			return move (ret) ;
		}

		inline Matrix operator- (CREF<Matrix> that) const {
			return sub (that) ;
		}

		void sub_with (CREF<Matrix> that) {
			for (auto &&i : iter (0 ,RANK::expr))
				mMatrix[i] -= that.mMatrix[i] ;
		}

		inline void operator-= (CREF<Matrix> that) {
			sub_with (that) ;
		}

		Matrix mul (CREF<ITEM> scale) const {
			Matrix ret ;
			for (auto &&i : iter (0 ,RANK::expr))
				ret.mMatrix[i] = mMatrix[i] * scale ;
			return move (ret) ;
		}

		inline Matrix operator* (CREF<ITEM> scale) const {
			return mul (scale) ;
		}

		inline friend Matrix operator* (CREF<ITEM> scale ,CREF<Matrix> that) {
			return that.mul (scale) ;
		}

		void mul_with (CREF<ITEM> scale) {
			for (auto &&i : iter (0 ,RANK::expr))
				mMatrix[i] *= scale ;
		}

		inline void operator*= (CREF<ITEM> scale) {
			mul_with (scale) ;
		}

		Vector mul (CREF<Vector> that) const {
			Vector ret ;
			for (auto &&i : iter (0 ,4)) {
				const auto r1x = at (0 ,i) * that.at (0) ;
				const auto r2x = at (1 ,i) * that.at (1) ;
				const auto r3x = at (2 ,i) * that.at (2) ;
				const auto r4x = at (3 ,i) * that.at (3) ;
				ret.at (i) = r1x + r2x + r3x + r4x ;
			}
			return move (ret) ;
		}

		inline Vector operator* (CREF<Vector> that) const {
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
			for (auto &&i : iter (0 ,RANK::expr))
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

		DECOMPOSE decompose () const {
			assert (is_affine_matrix ()) ;
			DECOMPOSE ret ;
			const auto r1x = thiz * Vector::axis_x () ;
			const auto r2x = thiz * Vector::axis_y () ;
			const auto r3x = thiz * Vector::axis_z () ;
			ret.mShear = ShearMatrix::make (r1x ,r2x ,r3x) ;
			const auto r4x = thiz * ret.mShear.inverse () ;
			const auto r5x = r4x * Vector::axis_x () ;
			const auto r6x = r4x * Vector::axis_y () ;
			const auto r7x = r4x * Vector::axis_z () ;
			const auto r8x = r4x * Vector::axis_w () ;
			const auto r9x = (r5x ^ r6x) * r7x ;
			const auto r10x = MathProc::sign (r9x) * MathProc::inverse (r8x[3]) ;
			const auto r11x = r5x.magnitude () * r10x ;
			const auto r12x = r6x.magnitude () * r10x ;
			const auto r13x = r7x.magnitude () * r10x ;
			ret.mScale = DiagMatrix::make (r11x ,r12x ,r13x ,ITEM (1)) ;
			ret.mRotation = ViewMatrix::make_xy (r5x ,r6x) ;
			const auto r14x = r8x.projection () ;
			ret.mTranslation = TranslationMatrix::make (r14x) ;
			return move (ret) ;
		}

		SINGULAR singular () const {
			unimplemented () ;
			return bad (TYPEAS<SINGULAR>::expr) ;
		}

		Matrix pseudo_inverse () const {
			Matrix ret ;
			for (auto &&i : iter (0 ,RANK::expr))
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

		INDEX find_abs_max_row (CREF<INDEX> x_) const {
			INDEX ret = NONE ;
			auto rax = ITEM () ;
			for (auto &&i : iter (x_ ,4)) {
				const auto r1x = MathProc::abs (at (x_ ,i)) ;
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

	struct SINGULAR {
		Matrix mU ;
		Matrix mS ;
		Matrix mV ;
	} ;

	class DiagMatrix implement Proxy {
	public:
		imports Matrix make (CREF<ITEM> x_ ,CREF<ITEM> y_ ,CREF<ITEM> z_ ,CREF<ITEM> w_) {
			Matrix ret = Matrix::zero () ;
			ret.at (0 ,0) = x_ ;
			ret.at (1 ,1) = y_ ;
			ret.at (2 ,2) = z_ ;
			ret.at (3 ,3) = w_ ;
			return move (ret) ;
		}
	} ;

	class ShearMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> vx ,CREF<Vector> vy ,CREF<Vector> vz) {
			const auto r1x = vx.normalize () ;
			const auto r2x = vy.normalize () ;
			const auto r3x = vz.normalize () ;
			const auto r4x = r1x * r2x ;
			const auto r5x = r1x * r3x ;
			const auto r6x = r2x * r3x ;
			const auto r7x = MathProc::sqrt (ITEM (1) - MathProc::square (r4x)) ;
			const auto r8x = (r6x - r4x * r5x) * MathProc::inverse (r7x) ;
			const auto r9x = MathProc::sqrt (ITEM (1) - MathProc::square (r5x) - MathProc::square (r8x)) ;
			const auto r10x = Vector::axis_x () ;
			const auto r11x = Vector (r4x ,r7x ,ITEM (0) ,ITEM (0)) ;
			const auto r12x = Vector (r5x ,r8x ,r9x ,ITEM (0)) ;
			const auto r13x = Vector::axis_w () ;
			return Matrix (r10x ,r11x ,r12x ,r13x) ;
		}
	} ;

	class RotationMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> normal ,CREF<ITEM> angle) {
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
	} ;

	class TranslationMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> t) {
			return make (t[0] ,t[1] ,t[2]) ;
		}

		imports Matrix make (CREF<ITEM> tx ,CREF<ITEM> ty ,CREF<ITEM> tz) {
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector (tx ,ty ,tz ,ITEM (1)) ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}
	} ;

	class PerspectiveMatrix implement Proxy {
	public:
		imports Matrix make (CREF<ITEM> fx ,CREF<ITEM> fy ,CREF<ITEM> wx ,CREF<ITEM> wy) {
			assert (fx > ITEM (0)) ;
			assert (fy > ITEM (0)) ;
			const auto r1x = Vector (fx ,ITEM (0) ,ITEM (0) ,ITEM (0)) ;
			const auto r2x = Vector (ITEM (0) ,fy ,ITEM (0) ,ITEM (0)) ;
			const auto r3x = Vector (wx ,wy ,ITEM (0) ,ITEM (1)) ;
			const auto r4x = Vector::axis_z () ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}
	} ;

	class ProjectionMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> normal ,CREF<ITEM> center ,CREF<Vector> light) {
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
	} ;

	class ViewMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> vx ,CREF<Vector> vy) {
			const auto r1x = vx.normalize () ;
			const auto r2x = vy.normalize () ;
			const auto r3x = (r1x ^ r2x).normalize () ;
			const auto r4x = (r3x ^ r1x).normalize () ;
			const auto r5x = Vector::axis_w () ;
			return Matrix (r1x ,r4x ,r3x ,r5x) ;
		}

		imports Matrix make_xy (CREF<Vector> vx ,CREF<Vector> vy) {
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make (vx ,vy) * r5x ;
		}

		imports Matrix make_zx (CREF<Vector> vz ,CREF<Vector> vx) {
			const auto r1x = Vector::axis_y () ;
			const auto r2x = Vector::axis_z () ;
			const auto r3x = Vector::axis_x () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make (vz ,vx) * r5x ;
		}

		imports Matrix make_yz (CREF<Vector> vy ,CREF<Vector> vz) {
			const auto r1x = Vector::axis_z () ;
			const auto r2x = Vector::axis_x () ;
			const auto r3x = Vector::axis_y () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make (vy ,vz) * r5x ;
		}

		imports Matrix make_yx (CREF<Vector> vy ,CREF<Vector> vx) {
			const auto r1x = Vector::axis_y () ;
			const auto r2x = Vector::axis_x () ;
			const auto r3x = -Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make (vy ,vx) * r5x ;
		}

		imports Matrix make_xz (CREF<Vector> vx ,CREF<Vector> vz) {
			const auto r1x = Vector::axis_x () ;
			const auto r2x = -Vector::axis_z () ;
			const auto r3x = Vector::axis_y () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make (vx ,vz) * r5x ;
		}

		imports Matrix make_zy (CREF<Vector> vz ,CREF<Vector> vy) {
			const auto r1x = -Vector::axis_z () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_x () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			return make (vz ,vy) * r5x ;
		}
	} ;

	class CrossProductMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> vx) {
			assert (vx[3] == ITEM (0)) ;
			const auto r1x = Vector (ITEM (0) ,vx[2] ,-vx[1] ,ITEM (0)) ;
			const auto r2x = Vector (-vx[2] ,ITEM (0) ,vx[0] ,ITEM (0)) ;
			const auto r3x = Vector (vx[1] ,-vx[0] ,ITEM (0) ,ITEM (1)) ;
			const auto r4x = Vector (ITEM (0) ,ITEM (0) ,ITEM (0) ,ITEM (1)) ;
			return Matrix (r1x ,r2x ,r3x ,r4x) ;
		}
	} ;

	class SymmetryMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> vx ,CREF<Vector> vy) {
			Matrix ret ;
			for (auto &&i : iter (0 ,4)) {
				for (auto &&j : iter (0 ,4))
					ret.at (j ,i) = vx[i] * vy[j] ;
			}
			return move (ret) ;
		}
	} ;

	class ReflectionMatrix implement Proxy {
	public:
		imports Matrix make (CREF<Vector> normal) {
			const auto r1x = normal.normalize () ;
			return Matrix::identity () - SymmetryMatrix::make (r1x ,r1x) * ITEM (2) ;
		}
	} ;
} ;

template <class ITEM>
using Vector = typename VECTOR_HELP<ITEM ,ALWAYS>::Vector ;

template <class ITEM>
using Quaternion = typename QUATERNION_HELP<ITEM ,ALWAYS>::Quaternion ;

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
using ViewMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::ViewMatrix ;
template <class ITEM>
using CrossProductMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::CrossProductMatrix ;
template <class ITEM>
using SymmetryMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::SymmetryMatrix ;
template <class ITEM>
using ReflectionMatrix = typename MATRIX_HELP<ITEM ,ALWAYS>::ReflectionMatrix ;
} ;