#pragma once

#ifndef __CSC_GEOMETRY__
#define __CSC_GEOMETRY__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class>
class Matrix ;

template <class REAL>
class Vector {
	_STATIC_ASSERT_ (IS_VAL_XYZ_HELP<REAL>::value) ;

private:
	Buffer<REAL ,ARGC<4>> mVector ;

public:
	implicit Vector () = default ;

	implicit Vector (const ARRAY3<REAL> &xyz_ ,const REAL &w)
		:Vector (xyz_[0] ,xyz_[1] ,xyz_[2] ,w) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit Vector (const REAL &x ,const REAL &y ,const REAL &z ,const REAL &w) {
		mVector[0] = x ;
		mVector[1] = y ;
		mVector[2] = z ;
		mVector[3] = w ;
	}

	ARRAY3<REAL> xyz () const {
		ARRAY3<REAL> ret ;
		ret[0] = mVector[0] ;
		ret[1] = mVector[1] ;
		ret[2] = mVector[2] ;
		return _MOVE_ (ret) ;
	}

	REAL &get (const INDEX &y) leftvalue {
		return mVector[y] ;
	}

	inline REAL &operator[] (const INDEX &y) leftvalue {
		return get (y) ;
	}

	const REAL &get (const INDEX &y) const leftvalue {
		return mVector[y] ;
	}

	inline const REAL &operator[] (const INDEX &y) const leftvalue {
		return get (y) ;
	}

	BOOL equal (const Vector &that) const {
		return BOOL (mVector == that.mVector) ;
	}

	inline BOOL operator== (const Vector &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Vector &that) const {
		return !equal (that) ;
	}

	FLAG compr (const Vector &that) const {
		return mVector.compr (that.mVector) ;
	}

	inline BOOL operator< (const Vector &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Vector &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Vector &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Vector &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	Vector mul (const REAL &scale) const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = mVector[i] * scale ;
		return _MOVE_ (ret) ;
	}

	inline Vector operator* (const REAL &scale) const {
		return mul (scale) ;
	}

	inline friend Vector operator* (const REAL &scale ,const Vector &self_) {
		return self_.mul (scale) ;
	}

	void multo (const REAL &scale) {
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			mVector[i] *= scale ;
	}

	inline Vector &operator*= (const REAL &scale) {
		multo (scale) ;
		return DEREF[this] ;
	}

	Vector div (const REAL &scale) const {
		return mul (MathProc::inverse (scale)) ;
	}

	inline Vector operator/ (const REAL &scale) const {
		return div (scale) ;
	}

	inline friend Vector operator/ (const REAL &scale ,const Vector &self_) {
		return self_.div (scale) ;
	}

	void divto (const REAL &scale) {
		multo (MathProc::inverse (scale)) ;
	}

	inline Vector &operator/= (const REAL &scale) {
		divto (scale) ;
		return DEREF[this] ;
	}

	Vector add (const Vector &that) const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = mVector[i] + that.mVector[i] ;
		return _MOVE_ (ret) ;
	}

	inline Vector operator+ (const Vector &that) const {
		return add (that) ;
	}

	void addto (const Vector &that) {
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			mVector[i] += that.mVector[i] ;
	}

	inline Vector &operator+= (const Vector &that) {
		addto (that) ;
		return DEREF[this] ;
	}

	Vector sub (const Vector &that) const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = mVector[i] - that.mVector[i] ;
		return _MOVE_ (ret) ;
	}

	inline Vector operator- (const Vector &that) const {
		return sub (that) ;
	}

	void subto (const Vector &that) {
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			mVector[i] -= that.mVector[i] ;
	}

	inline Vector &operator-= (const Vector &that) {
		subto (that) ;
		return DEREF[this] ;
	}

	Vector plus () const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = +mVector[i] ;
		return _MOVE_ (ret) ;
	}

	inline Vector operator+ () const {
		return plus () ;
	}

	Vector minus () const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = -mVector[i] ;
		return _MOVE_ (ret) ;
	}

	inline Vector operator- () const {
		return minus () ;
	}

	REAL mul (const Vector &that) const {
		const auto r1x = mVector[0] * that.mVector[0] ;
		const auto r2x = mVector[1] * that.mVector[1] ;
		const auto r3x = mVector[2] * that.mVector[2] ;
		const auto r4x = mVector[3] * that.mVector[3] ;
		return r1x + r2x + r3x + r4x ;
	}

	inline REAL operator* (const Vector &that) const {
		return mul (that) ;
	}

	Vector mul (const Matrix<REAL> &that) const {
		struct Dependent ;
		Vector<REAL> ret ;
		auto &r1x = _FORWARD_ (ARGV<DEPENDENT_TYPE<Matrix<REAL> ,Dependent>>::null ,that) ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			const auto r2x = get (0) * r1x.get (0 ,i) ;
			const auto r3x = get (1) * r1x.get (1 ,i) ;
			const auto r4x = get (2) * r1x.get (2 ,i) ;
			const auto r5x = get (3) * r1x.get (3 ,i) ;
			ret.get (i) = r2x + r3x + r4x + r5x ;
		}
		return _MOVE_ (ret) ;
	}

	inline Vector operator* (const Matrix<REAL> &that) const {
		return mul (that) ;
	}

	void multo (const Matrix<REAL> &that) {
		DEREF[this] = mul (that) ;
	}

	inline Vector &operator*= (const Matrix<REAL> &that) {
		multo (that) ;
		return DEREF[this] ;
	}

	Vector cross (const Vector &that) const {
		_DEBUG_ASSERT_ (mVector[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (that.mVector[3] == REAL (0)) ;
		Vector ret ;
		ret.mVector[0] = mVector[1] * that.mVector[2] - mVector[2] * that.mVector[1] ;
		ret.mVector[1] = mVector[2] * that.mVector[0] - mVector[0] * that.mVector[2] ;
		ret.mVector[2] = mVector[0] * that.mVector[1] - mVector[1] * that.mVector[0] ;
		ret.mVector[3] = REAL (0) ;
		return _MOVE_ (ret) ;
	}

	inline Vector operator^ (const Vector &that) const {
		return cross (that) ;
	}

	void crossto (const Vector &that) {
		DEREF[this] = cross (that) ;
	}

	inline Vector &operator^= (const Vector &that) {
		crossto (that) ;
		return DEREF[this] ;
	}

	REAL magnitude () const {
		_DEBUG_ASSERT_ (mVector[3] == REAL (0)) ;
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
		ret.mVector[3] = REAL (0) ;
		return _MOVE_ (ret) ;
	}

	Vector projection () const {
		Vector ret ;
		const auto r1x = MathProc::inverse (mVector[3]) ;
		ret.mVector[0] = mVector[0] * r1x ;
		ret.mVector[1] = mVector[1] * r1x ;
		ret.mVector[2] = mVector[2] * r1x ;
		ret.mVector[3] = REAL (1) ;
		return _MOVE_ (ret) ;
	}

	Vector homogenize () const {
		Vector ret ;
		ret.mVector[0] = mVector[0] ;
		ret.mVector[1] = mVector[1] ;
		ret.mVector[2] = mVector[2] ;
		ret.mVector[3] = REAL (0) ;
		return _MOVE_ (ret) ;
	}

	imports const Vector &axis_x () {
		return _CACHE_ ([&] () {
			return Vector {REAL (1) ,REAL (0) ,REAL (0) ,REAL (0)} ;
		}) ;
	}

	imports const Vector &axis_y () {
		return _CACHE_ ([&] () {
			return Vector {REAL (0) ,REAL (1) ,REAL (0) ,REAL (0)} ;
		}) ;
	}

	imports const Vector &axis_z () {
		return _CACHE_ ([&] () {
			return Vector {REAL (0) ,REAL (0) ,REAL (1) ,REAL (0)} ;
		}) ;
	}

	imports const Vector &axis_w () {
		return _CACHE_ ([&] () {
			return Vector {REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)} ;
		}) ;
	}
} ;

template <class REAL>
class Matrix {
	_STATIC_ASSERT_ (IS_VAL_XYZ_HELP<REAL>::value) ;

private:
	struct Private {
		template <class>
		class Row ;
	} ;

private:
	Buffer<REAL ,ARGC<16>> mMatrix ;

public:
	implicit Matrix () = default ;

	implicit Matrix (const ARRAY4<REAL> &hx ,const ARRAY4<REAL> &hy ,const ARRAY4<REAL> &hz ,const ARRAY4<REAL> &hw) {
		for (auto &&i : _RANGE_ (0 ,4)) {
			get (0 ,i) = hx[i] ;
			get (1 ,i) = hy[i] ;
			get (2 ,i) = hz[i] ;
			get (3 ,i) = hw[i] ;
		}
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<_ARG1 ,REAL>::value)>>
	implicit Matrix (const Vector<_ARG1> &vx ,const Vector<_ARG1> &vy ,const Vector<_ARG1> &vz ,const Vector<_ARG1> &vw) {
		for (auto &&i : _RANGE_ (0 ,4)) {
			get (i ,0) = vx[i] ;
			get (i ,1) = vy[i] ;
			get (i ,2) = vz[i] ;
			get (i ,3) = vw[i] ;
		}
	}

	REAL &get (const INDEX &y ,const INDEX &x) leftvalue {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y * 4 + x] ;
	}

	const REAL &get (const INDEX &y ,const INDEX &x) const leftvalue {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y * 4 + x] ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Matrix>>>
	_RET get (const INDEX &y) leftvalue {
		struct Dependent ;
		using Row = typename DEPENDENT_TYPE<Private ,Dependent>::template Row<Matrix> ;
		return Row (PhanRef<Matrix>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Matrix>>>
	inline _RET operator[] (const INDEX &y) leftvalue {
		return get (y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Matrix>>>
	_RET get (const INDEX &y) const leftvalue {
		struct Dependent ;
		using Row = typename DEPENDENT_TYPE<Private ,Dependent>::template Row<const Matrix> ;
		return Row (PhanRef<const Matrix>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Matrix>>>
	inline _RET operator[] (const INDEX &y) const leftvalue {
		return get (y) ;
	}

	BOOL equal (const Matrix &that) const {
		return BOOL (mMatrix == that.mMatrix) ;
	}

	inline BOOL operator== (const Matrix &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Matrix &that) const {
		return !equal (that) ;
	}

	FLAG compr (const Matrix &that) const {
		return mMatrix.compr (that.mMatrix) ;
	}

	inline BOOL operator< (const Matrix &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Matrix &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Matrix &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Matrix &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	Matrix mul (const REAL &scale) const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = mMatrix[i] * scale ;
		return _MOVE_ (ret) ;
	}

	inline Matrix operator* (const REAL &scale) const {
		return mul (scale) ;
	}

	inline friend Matrix operator* (const REAL &scale ,const Matrix &self_) {
		return self_.mul (scale) ;
	}

	void multo (const REAL &scale) {
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			mMatrix[i] *= scale ;
	}

	inline Matrix &operator*= (const REAL &scale) {
		multo (scale) ;
		return DEREF[this] ;
	}

	Matrix div (const REAL &scale) const {
		return mul (MathProc::inverse (scale)) ;
	}

	inline Matrix operator/ (const REAL &scale) const {
		return div (scale) ;
	}

	inline friend Matrix operator/ (const REAL &scale ,const Matrix &self_) {
		return self_.div (scale) ;
	}

	void divto (const REAL &scale) {
		multo (MathProc::inverse (scale)) ;
	}

	inline Matrix &operator/= (const REAL &scale) {
		divto (scale) ;
		return DEREF[this] ;
	}

	Matrix add (const Matrix &that) const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = mMatrix[i] + that.mMatrix[i] ;
		return _MOVE_ (ret) ;
	}

	inline Matrix operator+ (const Matrix &that) const {
		return add (that) ;
	}

	void addto (const Matrix &that) {
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			mMatrix[i] += that.mMatrix[i] ;
	}

	inline Matrix &operator+= (const Matrix &that) {
		addto (that) ;
		return DEREF[this] ;
	}

	Matrix sub (const Matrix &that) const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = mMatrix[i] - that.mMatrix[i] ;
		return _MOVE_ (ret) ;
	}

	inline Matrix operator- (const Matrix &that) const {
		return sub (that) ;
	}

	void subto (const Matrix &that) {
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			mMatrix[i] -= that.mMatrix[i] ;
	}

	inline Matrix &operator-= (const Matrix &that) {
		subto (that) ;
		return DEREF[this] ;
	}

	Matrix plus () const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = +mMatrix[i] ;
		return _MOVE_ (ret) ;
	}

	inline Matrix operator+ () const {
		return plus () ;
	}

	Matrix minus () const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = -mMatrix[i] ;
		return _MOVE_ (ret) ;
	}

	inline Matrix operator- () const {
		return minus () ;
	}

	Matrix mul (const Matrix &that) const {
		Matrix ret ;
		const auto r1x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : _RANGE_ (r1x)) {
			const auto r2x = get (i[0] ,0) * that.get (0 ,i[1]) ;
			const auto r3x = get (i[0] ,1) * that.get (1 ,i[1]) ;
			const auto r4x = get (i[0] ,2) * that.get (2 ,i[1]) ;
			const auto r5x = get (i[0] ,3) * that.get (3 ,i[1]) ;
			ret.get (i[0] ,i[1]) = r2x + r3x + r4x + r5x ;
		}
		return _MOVE_ (ret) ;
	}

	inline Matrix operator* (const Matrix &that) const {
		return mul (that) ;
	}

	void multo (const Matrix &that) {
		DEREF[this] = mul (that) ;
	}

	inline Matrix &operator*= (const Matrix &that) {
		multo (that) ;
		return DEREF[this] ;
	}

	Vector<REAL> mul (const Vector<REAL> &that) const {
		Vector<REAL> ret ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			const auto r1x = get (i ,0) * that.get (0) ;
			const auto r2x = get (i ,1) * that.get (1) ;
			const auto r3x = get (i ,2) * that.get (2) ;
			const auto r4x = get (i ,3) * that.get (3) ;
			ret.get (i) = r1x + r2x + r3x + r4x ;
		}
		return _MOVE_ (ret) ;
	}

	inline Vector<REAL> operator* (const Vector<REAL> &that) const {
		return mul (that) ;
	}

	Matrix transpose () const {
		Matrix ret ;
		const auto r1x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : _RANGE_ (r1x))
			ret.get (i[1] ,i[0]) = get (i[0] ,i[1]) ;
		return _MOVE_ (ret) ;
	}

	Matrix triangular () const {
		Matrix ret = DEREF[this] ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			if switch_once (TRUE) {
				INDEX ix = ret.find_max_row (i) ;
				if (ix == i)
					discard ;
				for (auto &&j : _RANGE_ (i ,4)) {
					const auto r1x = -ret.get (i ,j) ;
					ret.get (i ,j) = ret.get (ix ,j) ;
					ret.get (ix ,j) = r1x ;
				}
			}
			const auto r2x = MathProc::inverse (ret.get (i ,i)) ;
			if (r2x == REAL (0))
				continue ;
			for (auto &&j : _RANGE_ (i + 1 ,4)) {
				const auto r3x = ret.get (j ,i) * r2x ;
				for (auto &&k : _RANGE_ (i + 1 ,4))
					ret.get (j ,k) -= r3x * ret.get (i ,k) ;
				ret.get (j ,i) = REAL (0) ;
			}
		}
		return _MOVE_ (ret) ;
	}

	LENGTH rank () const {
		LENGTH ret = 0 ;
		const auto r1x = triangular () ;
		for (auto &&i : _RANGE_ (0 ,4))
			ret += _EBOOL_ (MathProc::inverse (r1x[i][i]) == REAL (0)) ;
		ret = 4 - ret ;
		return _MOVE_ (ret) ;
	}

	REAL det () const {
		const auto r1x = triangular () ;
		REAL ret = REAL (1) ;
		for (auto &&i : _RANGE_ (0 ,4))
			ret *= r1x.get (i ,i) ;
		return _MOVE_ (ret) ;
	}

	Matrix inverse () const {
		Matrix ret ;
		const auto r1x = MathProc::inverse (det ()) ;
		_DYNAMIC_ASSERT_ (r1x != REAL (0)) ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = _EBOOL_ (i == 0) ;
			INDEX iy = ix + 1 + _EBOOL_ (i == 1) ;
			INDEX iz = iy + 1 + _EBOOL_ (i == 2) ;
			for (auto &&j : _RANGE_ (0 ,4)) {
				INDEX jx = _EBOOL_ (j == 0) ;
				INDEX jy = jx + 1 + _EBOOL_ (j == 1) ;
				INDEX jz = jy + 1 + _EBOOL_ (j == 2) ;
				const auto r2x = get (ix ,jx) * (get (iy ,jy) * get (iz ,jz) - get (iz ,jy) * get (iy ,jz)) ;
				const auto r3x = get (iy ,jx) * (get (ix ,jy) * get (iz ,jz) - get (iz ,jy) * get (ix ,jz)) ;
				const auto r4x = get (iz ,jx) * (get (ix ,jy) * get (iy ,jz) - get (iy ,jy) * get (ix ,jz)) ;
				const auto r5x = r2x - r3x + r4x ;
				const auto r6x = -r5x ;
				auto &r7x = _SWITCH_ (
					((i + j) % 2 == 0) ? r5x :
					r6x) ;
				ret.get (j ,i) = r7x ;
			}
		}
		ret *= r1x ;
		if switch_once (TRUE) {
			if (!affine_matrix_like ())
				discard ;
			if (get (3 ,3) != REAL (1))
				discard ;
			if (!ret.affine_matrix_like ())
				discard ;
			const auto r8x = MathProc::inverse (ret.get (3 ,3)) ;
			ret *= r8x ;
			ret.get (3 ,3) = REAL (1) ;
		}
		return _MOVE_ (ret) ;
	}

	REAL trace () const {
		REAL ret = REAL (0) ;
		for (auto &&i : _RANGE_ (0 ,4))
			ret += get (i ,i) ;
		return _MOVE_ (ret) ;
	}

	//@info: 3-translation * 2-rotation * 1-scale * 0-shear
	ARRAY4<Matrix> decompose () const {
		_DYNAMIC_ASSERT_ (affine_matrix_like ()) ;
		ARRAY4<Matrix> ret ;
		const auto r1x = mul (Vector<REAL>::axis_x ()) ;
		const auto r2x = mul (Vector<REAL>::axis_y ()) ;
		const auto r3x = mul (Vector<REAL>::axis_z ()) ;
		ret[0] = Matrix::make_shear (r1x ,r2x ,r3x) ;
		const auto r4x = mul (ret[0].inverse ()) ;
		const auto r5x = r4x.mul (Vector<REAL>::axis_x ()) ;
		const auto r6x = r4x.mul (Vector<REAL>::axis_y ()) ;
		const auto r7x = r4x.mul (Vector<REAL>::axis_z ()) ;
		const auto r8x = r4x.mul (Vector<REAL>::axis_w ()) ;
		const auto r9x = MathProc::sign ((r5x ^ r6x) * r7x) * MathProc::inverse (r8x[3]) ;
		const auto r10x = r5x.magnitude () * r9x ;
		const auto r11x = r6x.magnitude () * r9x ;
		const auto r12x = r7x.magnitude () * r9x ;
		ret[1] = Matrix::make_diag (r10x ,r11x ,r12x ,REAL (1)) ;
		ret[2] = Matrix::make_view (r5x ,r6x) ;
		const auto r13x = r8x.projection () - Vector<REAL>::axis_w () ;
		ret[3] = Matrix::make_translation (r13x) ;
		return _MOVE_ (ret) ;
	}

	imports const Matrix &identity () {
		return _CACHE_ ([&] () {
			return Matrix ({
				{REAL (1) ,REAL (0) ,REAL (0) ,REAL (0)} ,
				{REAL (0) ,REAL (1) ,REAL (0) ,REAL (0)} ,
				{REAL (0) ,REAL (0) ,REAL (1) ,REAL (0)} ,
				{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		}) ;
	}

	imports Matrix make_diag (const REAL &x ,const REAL &y ,const REAL &z ,const REAL &w) {
		Matrix ret = Matrix ({
			{x ,REAL (0) ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,y ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,z ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,w}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_shear (const Vector<REAL> &vx ,const Vector<REAL> &vy ,const Vector<REAL> &vz) {
		_DEBUG_ASSERT_ (vx[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (vy[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (vz[3] == REAL (0)) ;
		const auto r1x = vx.normalize () ;
		const auto r2x = vy.normalize () ;
		const auto r3x = vz.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = r2x * r3x ;
		const auto r7x = MathProc::sqrt (REAL (1) - MathProc::square (r4x)) ;
		const auto r8x = (r6x - r4x * r5x) * MathProc::inverse (r7x) ;
		const auto r9x = MathProc::sqrt (REAL (1) - MathProc::square (r5x) - MathProc::square (r8x)) ;
		Matrix ret = Matrix ({
			{REAL (1) ,r4x ,r5x ,REAL (0)} ,
			{REAL (0) ,r7x ,r8x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,r9x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_rotation (const Vector<REAL> &normal ,const REAL &angle) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		Matrix ret ;
		const auto r1x = normal.normalize () ;
		const auto r2x = MathProc::cos (angle) ;
		const auto r3x = r1x * MathProc::sin (angle) ;
		const auto r4x = r1x * (REAL (1) - r2x) ;
		ret[0][0] = r1x[0] * r4x[0] + r2x ;
		ret[0][1] = r1x[0] * r4x[1] - r3x[2] ;
		ret[0][2] = r1x[0] * r4x[2] + r3x[1] ;
		ret[0][3] = REAL (0) ;
		ret[1][0] = r1x[1] * r4x[0] + r3x[2] ;
		ret[1][1] = r1x[1] * r4x[1] + r2x ;
		ret[1][2] = r1x[1] * r4x[2] - r3x[0] ;
		ret[1][3] = REAL (0) ;
		ret[2][0] = r1x[2] * r4x[0] - r3x[1] ;
		ret[2][1] = r1x[2] * r4x[1] + r3x[0] ;
		ret[2][2] = r1x[2] * r4x[2] + r2x ;
		ret[2][3] = REAL (0) ;
		ret[3][0] = REAL (0) ;
		ret[3][1] = REAL (0) ;
		ret[3][2] = REAL (0) ;
		ret[3][3] = REAL (1) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_rotation (const REAL &qx ,const REAL &qy ,const REAL &qz ,const REAL &qw) {
		Matrix ret ;
		const auto r1x = MathProc::square (qx) + MathProc::square (qy) + MathProc::square (qz) + MathProc::square (qw) ;
		const auto r2x = REAL (2) * MathProc::inverse (r1x) ;
		const auto r3x = qx * r2x ;
		const auto r4x = qy * r2x ;
		const auto r5x = qz * r2x ;
		const auto r6x = qw * r2x ;
		ret[0][0] = REAL (1) - qy * r4x - qz * r5x ;
		ret[0][1] = qx * r4x - qz * r6x ;
		ret[0][2] = qx * r5x + qy * r6x ;
		ret[0][3] = REAL (0) ;
		ret[1][0] = qx * r4x + qz * r6x ;
		ret[1][1] = REAL (1) - qx * r3x - qz * r5x ;
		ret[1][2] = qy * r5x - qx * r6x ;
		ret[1][3] = REAL (0) ;
		ret[2][0] = qx * r5x - qy * r6x ;
		ret[2][1] = qy * r5x + qx * r6x ;
		ret[2][2] = REAL (1) - qx * r3x - qy * r4x ;
		ret[2][3] = REAL (0) ;
		ret[3][0] = REAL (0) ;
		ret[3][1] = REAL (0) ;
		ret[3][2] = REAL (0) ;
		ret[3][3] = REAL (1) ;
		return _MOVE_ (ret) ;
	}

	imports ARRAY4<REAL> make_rotation_quat (const Matrix &rot_mat) {
		ARRAY4<REAL> ret ;
		const auto r1x = rot_mat.decompose () ;
		const auto r2x = r1x[2] ;
		const auto r3x = ARRAY4<REAL> ({
			REAL (1) + r2x[0][0] + r2x[1][1] + r2x[2][2] ,
			REAL (1) + r2x[0][0] - r2x[1][1] - r2x[2][2] ,
			REAL (1) - r2x[0][0] + r2x[1][1] - r2x[2][2] ,
			REAL (1) - r2x[0][0] - r2x[1][1] + r2x[2][2]}) ;
		const auto r4x = _CALL_ ([&] () {
			INDEX ret = VAR_NONE ;
			auto rax = REAL () ;
			for (auto &&i : _RANGE_ (0 ,4)) {
				if (ret != VAR_NONE)
					if (rax >= r3x[i])
						continue ;
				ret = i ;
				rax = r3x[i] ;
			}
			return _MOVE_ (ret) ;
		}) ;
		const auto r5x = MathProc::inverse (REAL (2) * MathProc::sqrt (r3x[r4x])) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(r4x == 0))
				discard ;
			ret[0] = (r2x[2][1] - r2x[1][2]) * r5x ;
			ret[1] = (r2x[0][2] - r2x[2][0]) * r5x ;
			ret[2] = (r2x[1][0] - r2x[0][1]) * r5x ;
			ret[3] = r3x[0] * r5x ;
		}
		if switch_once (fax) {
			if (!(r4x == 1))
				discard ;
			ret[0] = r3x[1] * r5x ;
			ret[1] = (r2x[1][0] + r2x[0][1]) * r5x ;
			ret[2] = (r2x[0][2] + r2x[2][0]) * r5x ;
			ret[3] = (r2x[2][1] - r2x[1][2]) * r5x ;
		}
		if switch_once (fax) {
			if (!(r4x == 2))
				discard ;
			ret[0] = (r2x[1][0] + r2x[0][1]) * r5x ;
			ret[1] = r3x[2] * r5x ;
			ret[2] = (r2x[2][1] + r2x[1][2]) * r5x ;
			ret[3] = (r2x[0][2] - r2x[2][0]) * r5x ;
		}
		if switch_once (fax) {
			if (!(r4x == 3))
				discard ;
			ret[0] = (r2x[0][2] + r2x[2][0]) * r5x ;
			ret[1] = (r2x[2][1] + r2x[1][2]) * r5x ;
			ret[2] = r3x[3] * r5x ;
			ret[3] = (r2x[1][0] - r2x[0][1]) * r5x ;
		}
		return _MOVE_ (ret) ;
	}

	imports ARRAY3<REAL> make_rotation_axis (const Matrix &rot_mat) {
		ARRAY3<REAL> ret ;
		const auto r1x = make_rotation_quat (rot_mat) ;
		const auto r2x = Vector<REAL> {r1x[0] ,r1x[1] ,r1x[2] ,REAL (0)}.magnitude () ;
		const auto r3x = r2x * MathProc::sign (r1x[3]) ;
		const auto r4x = MathProc::arctan (r3x ,MathProc::abs (r1x[3])) ;
		const auto r5x = (REAL (1) - (r2x - r4x) * MathProc::inverse (r2x)) * REAL (2) ;
		ret[0] = r1x[0] * r5x ;
		ret[1] = r1x[1] * r5x ;
		ret[2] = r1x[2] * r5x ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_translation (const Vector<REAL> &direction) {
		_DEBUG_ASSERT_ (direction[3] == REAL (0)) ;
		Matrix ret = Matrix ({
			{REAL (1) ,REAL (0) ,REAL (0) ,direction[0]} ,
			{REAL (0) ,REAL (1) ,REAL (0) ,direction[1]} ,
			{REAL (0) ,REAL (0) ,REAL (1) ,direction[2]} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_view (const Vector<REAL> &vx ,const Vector<REAL> &vy) {
		_DEBUG_ASSERT_ (vx[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (vy[3] == REAL (0)) ;
		const auto r1x = vx.normalize () ;
		const auto r2x = vy.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		Matrix ret = Matrix ({
			{r1x[0] ,r4x[0] ,r3x[0] ,REAL (0)} ,
			{r1x[1] ,r4x[1] ,r3x[1] ,REAL (0)} ,
			{r1x[2] ,r4x[2] ,r3x[2] ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_view_xy (const Vector<REAL> &vx ,const Vector<REAL> &vy) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vx ,vy) * r1x ;
	}

	imports Matrix make_view_zx (const Vector<REAL> &vz ,const Vector<REAL> &vx) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vz ,vx) * r1x ;
	}

	imports Matrix make_view_yz (const Vector<REAL> &vy ,const Vector<REAL> &vz) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vy ,vz) * r1x ;
	}

	imports Matrix make_view_yx (const Vector<REAL> &vy ,const Vector<REAL> &vx) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_x () ,
			-Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vy ,vx) * r1x ;
	}

	imports Matrix make_view_xz (const Vector<REAL> &vx ,const Vector<REAL> &vz) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_x () ,
			-Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vx ,vz) * r1x ;
	}

	imports Matrix make_view_zy (const Vector<REAL> &vz ,const Vector<REAL> &vy) {
		const auto r1x = Matrix ({
			-Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vz ,vy) * r1x ;
	}

	imports Matrix make_view (const Vector<REAL> &px ,const Vector<REAL> &py ,const Vector<REAL> &pw) {
		_DEBUG_ASSERT_ (px[3] == REAL (1)) ;
		_DEBUG_ASSERT_ (py[3] == REAL (1)) ;
		_DEBUG_ASSERT_ (pw[3] == REAL (1)) ;
		const auto r1x = (px - pw).normalize () ;
		const auto r2x = (py - pw).normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		Matrix ret = Matrix ({
			{r1x[0] ,r4x[0] ,r3x[0] ,pw[0]} ,
			{r1x[1] ,r4x[1] ,r3x[1] ,pw[1]} ,
			{r1x[2] ,r4x[2] ,r3x[2] ,pw[2]} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_perspective (const REAL &fx ,const REAL &fy ,const REAL &wx ,const REAL &wy) {
		_DEBUG_ASSERT_ (fx > REAL (0)) ;
		_DEBUG_ASSERT_ (fy > REAL (0)) ;
		Matrix ret = Matrix ({
			{fx ,REAL (0) ,wx ,REAL (0)} ,
			{REAL (0) ,fy ,wy ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)} ,
			{REAL (0) ,REAL (0) ,REAL (1) ,REAL (0)}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_projection (const Vector<REAL> &normal ,const REAL &center ,const Vector<REAL> &light) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (light[3] == REAL (0)) ;
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
		ret[3][0] = REAL (0) ;
		ret[3][1] = REAL (0) ;
		ret[3][2] = REAL (0) ;
		ret[3][3] = r3x ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_cross_product (const Vector<REAL> &first) {
		_DEBUG_ASSERT_ (first[3] == REAL (0)) ;
		Matrix ret = Matrix ({
			{REAL (0) ,-first[2] ,first[1] ,REAL (0)} ,
			{first[2] ,REAL (0) ,-first[0] ,REAL (0)} ,
			{-first[1] ,first[0] ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (0)}}) ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_symmetry (const Vector<REAL> &first ,const Vector<REAL> &second) {
		Matrix ret ;
		const auto r1x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : _RANGE_ (r1x))
			ret[i[0]][i[1]] = first[i[0]] * second[i[1]] ;
		return _MOVE_ (ret) ;
	}

	imports Matrix make_reflection (const Vector<REAL> &normal) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		const auto r1x = normal.normalize () ;
		return identity () - make_symmetry (r1x ,r1x) * REAL (2) ;
	}

private:
	BOOL affine_matrix_like () const {
		if (get (3 ,0) != REAL (0))
			return FALSE ;
		if (get (3 ,1) != REAL (0))
			return FALSE ;
		if (get (3 ,2) != REAL (0))
			return FALSE ;
		if (MathProc::inverse (get (3 ,3)) == REAL (0))
			return FALSE ;
		return TRUE ;
	}

	INDEX find_max_row (const INDEX &yx) const {
		INDEX ret = VAR_NONE ;
		auto rax = REAL () ;
		for (auto &&i : _RANGE_ (yx ,4)) {
			const auto r1x = MathProc::abs (get (i ,yx)) ;
			if (ret != VAR_NONE)
				if (rax >= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return _MOVE_ (ret) ;
	}
} ;

template <class REAL>
template <class BASE>
class Matrix<REAL>::Private::Row
	:private Proxy {
private:
	PhanRef<BASE> mBase ;
	INDEX mY ;

public:
	implicit Row () = delete ;

	explicit Row (PhanRef<BASE> &&base ,const INDEX &y) {
		mBase = _MOVE_ (base) ;
		mY = y ;
	}

	inline CAST_TRAITS_TYPE<REAL ,BASE> &operator[] (const INDEX &x) rightvalue {
		return mBase->get (mY ,x) ;
	}
} ;
} ;