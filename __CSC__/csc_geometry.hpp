#pragma once

#ifndef __CSC_GEOMETRY__
#define __CSC_GEOMETRY__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class>
class Matrix ;

template <class REAL>
class Vector {
private:
	_STATIC_ASSERT_ (stl::is_val_xyz<REAL>::value) ;
	Buffer<REAL ,ARGC<4>> mVector ;

public:
	Vector () = default ;

	implicit Vector (const ARRAY3<REAL> &xyz ,const REAL &w) :Vector (xyz[0] ,xyz[1] ,xyz[2] ,w) {}

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
		return std::move (ret) ;
	}

	REAL &get (INDEX y) & {
		return mVector[y] ;
	}

	inline REAL &operator[] (INDEX y) & {
		return get (y) ;
	}

	const REAL &get (INDEX y) const & {
		return mVector[y] ;
	}

	inline const REAL &operator[] (INDEX y) const & {
		return get (y) ;
	}

	REAL &get (INDEX) && = delete ;

	inline REAL &operator[] (INDEX) && = delete ;

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
		return std::move (ret) ;
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
		return (*this) ;
	}

	Vector div (const REAL &scale) const {
		return mul (_PINV_ (scale)) ;
	}

	inline Vector operator/ (const REAL &scale) const {
		return div (scale) ;
	}

	inline friend Vector operator/ (const REAL &scale ,const Vector &self_) {
		return self_.div (scale) ;
	}

	void divto (const REAL &scale) {
		multo (_PINV_ (scale)) ;
	}

	inline Vector &operator/= (const REAL &scale) {
		divto (scale) ;
		return (*this) ;
	}

	Vector add (const Vector &that) const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = mVector[i] + that.mVector[i] ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Vector sub (const Vector &that) const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = mVector[i] - that.mVector[i] ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Vector plus () const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = +mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator+ () const {
		return plus () ;
	}

	Vector minus () const {
		Vector ret ;
		for (auto &&i : _RANGE_ (0 ,mVector.size ()))
			ret.mVector[i] = -mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator- () const {
		return minus () ;
	}

	REAL mul (const Vector &that) const {
		_DEBUG_ASSERT_ (mVector[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (that.mVector[3] == REAL (0)) ;
		return mVector[0] * that.mVector[0] + mVector[1] * that.mVector[1] + mVector[2] * that.mVector[2] ;
	}

	inline REAL operator* (const Vector &that) const {
		return mul (that) ;
	}

	Vector mul (const Matrix<REAL> &that) const ;

	inline Vector operator* (const Matrix<REAL> &that) const {
		return mul (that) ;
	}

	void multo (const Matrix<REAL> &that) {
		(*this) = mul (that) ;
	}

	inline Vector &operator*= (const Matrix<REAL> &that) {
		multo (that) ;
		return (*this) ;
	}

	Vector cross (const Vector &that) const {
		_DEBUG_ASSERT_ (mVector[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (that.mVector[3] == REAL (0)) ;
		Vector ret ;
		ret.mVector[0] = mVector[1] * that.mVector[2] - mVector[2] * that.mVector[1] ;
		ret.mVector[1] = mVector[2] * that.mVector[0] - mVector[0] * that.mVector[2] ;
		ret.mVector[2] = mVector[0] * that.mVector[1] - mVector[1] * that.mVector[0] ;
		ret.mVector[3] = REAL (0) ;
		return std::move (ret) ;
	}

	inline Vector operator^ (const Vector &that) const {
		return cross (that) ;
	}

	void crossto (const Vector &that) {
		(*this) = cross (that) ;
	}

	inline Vector &operator^= (const Vector &that) {
		crossto (that) ;
		return (*this) ;
	}

	REAL magnitude () const {
		_DEBUG_ASSERT_ (mVector[3] == REAL (0)) ;
		return _SQRT_ (_SQE_ (mVector[0]) + _SQE_ (mVector[1]) + _SQE_ (mVector[2])) ;
	}

	Vector normalize () const {
		Vector ret ;
		const auto r1x = _PINV_ (magnitude ()) ;
		ret.mVector[0] = mVector[0] * r1x ;
		ret.mVector[1] = mVector[1] * r1x ;
		ret.mVector[2] = mVector[2] * r1x ;
		ret.mVector[3] = REAL (0) ;
		return std::move (ret) ;
	}

	Vector projection () const {
		Vector ret ;
		const auto r1x = _PINV_ (mVector[3]) ;
		ret.mVector[0] = mVector[0] * r1x ;
		ret.mVector[1] = mVector[1] * r1x ;
		ret.mVector[2] = mVector[2] * r1x ;
		ret.mVector[3] = REAL (1) ;
		return std::move (ret) ;
	}

	Vector homogenize () const {
		Vector ret ;
		ret.mVector[0] = mVector[0] ;
		ret.mVector[1] = mVector[1] ;
		ret.mVector[2] = mVector[2] ;
		ret.mVector[3] = REAL (0) ;
		return std::move (ret) ;
	}

public:
	static const Vector &axis_x () {
		return _CACHE_ ([] () {
			return Vector {REAL (1) ,REAL (0) ,REAL (0) ,REAL (0)} ;
		}) ;
	}

	static const Vector &axis_y () {
		return _CACHE_ ([] () {
			return Vector {REAL (0) ,REAL (1) ,REAL (0) ,REAL (0)} ;
		}) ;
	}

	static const Vector &axis_z () {
		return _CACHE_ ([] () {
			return Vector {REAL (0) ,REAL (0) ,REAL (1) ,REAL (0)} ;
		}) ;
	}

	static const Vector &axis_w () {
		return _CACHE_ ([] () {
			return Vector {REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)} ;
		}) ;
	}
} ;

template <class REAL>
class Matrix {
private:
	template <class BASE>
	class Row final {
	private:
		friend Matrix ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline Row (const Row &) = delete ;

		inline Row (Row &&) noexcept = default ;

		inline CAST_TRAITS_TYPE<REAL ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) popping : mBase (base) ,mY (y) {}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_val_xyz<REAL>::value) ;
	Buffer<REAL ,ARGC<16>> mMatrix ;

public:
	Matrix () = default ;

	implicit Matrix (const ARRAY4<REAL> &hx ,const ARRAY4<REAL> &hy ,const ARRAY4<REAL> &hz ,const ARRAY4<REAL> &hw) {
		for (auto &&i : _RANGE_ (0 ,4)) {
			get (0 ,i) = hx[i] ;
			get (1 ,i) = hy[i] ;
			get (2 ,i) = hz[i] ;
			get (3 ,i) = hw[i] ;
		}
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<_ARG1 ,REAL>::value>>
	implicit Matrix (const Vector<_ARG1> &vx ,const Vector<_ARG1> &vy ,const Vector<_ARG1> &vz ,const Vector<_ARG1> &vw) {
		for (auto &&i : _RANGE_ (0 ,4)) {
			get (i ,0) = vx[i] ;
			get (i ,1) = vy[i] ;
			get (i ,2) = vz[i] ;
			get (i ,3) = vw[i] ;
		}
	}

	REAL &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y * 4 + x] ;
	}

	const REAL &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y * 4 + x] ;
	}

	REAL &get (INDEX ,INDEX) && = delete ;

	Row<Matrix> get (INDEX y) & {
		return Row<Matrix> ((*this) ,y) ;
	}

	inline Row<Matrix> operator[] (INDEX y) & {
		return get (y) ;
	}

	Row<const Matrix> get (INDEX y) const & {
		return Row<const Matrix> ((*this) ,y) ;
	}

	inline Row<const Matrix> operator[] (INDEX y) const & {
		return get (y) ;
	}

	Row<Matrix> get (INDEX) && = delete ;

	inline Row<Matrix> operator[] (INDEX) && = delete ;

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
		return std::move (ret) ;
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
		return (*this) ;
	}

	Matrix div (const REAL &scale) const {
		return mul (_PINV_ (scale)) ;
	}

	inline Matrix operator/ (const REAL &scale) const {
		return div (scale) ;
	}

	inline friend Matrix operator/ (const REAL &scale ,const Matrix &self_) {
		return self_.div (scale) ;
	}

	void divto (const REAL &scale) {
		multo (_PINV_ (scale)) ;
	}

	inline Matrix &operator/= (const REAL &scale) {
		divto (scale) ;
		return (*this) ;
	}

	Matrix add (const Matrix &that) const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = mMatrix[i] + that.mMatrix[i] ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Matrix sub (const Matrix &that) const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = mMatrix[i] - that.mMatrix[i] ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Matrix plus () const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = +mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ () const {
		return plus () ;
	}

	Matrix minus () const {
		Matrix ret ;
		for (auto &&i : _RANGE_ (0 ,mMatrix.size ()))
			ret.mMatrix[i] = -mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator- () const {
		return minus () ;
	}

	Matrix mul (const Matrix &that) const {
		Matrix ret ;
		const auto r1x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : ArrayRange<ARGC<2>> (r1x)) {
			const auto r2x = get (i[0] ,0) * that.get (0 ,i[1]) ;
			const auto r3x = get (i[0] ,1) * that.get (1 ,i[1]) ;
			const auto r4x = get (i[0] ,2) * that.get (2 ,i[1]) ;
			const auto r5x = get (i[0] ,3) * that.get (3 ,i[1]) ;
			ret.get (i[0] ,i[1]) = r2x + r3x + r4x + r5x ;
		}
		return std::move (ret) ;
	}

	inline Matrix operator* (const Matrix &that) const {
		return mul (that) ;
	}

	void multo (const Matrix &that) {
		(*this) = mul (that) ;
	}

	inline Matrix &operator*= (const Matrix &that) {
		multo (that) ;
		return (*this) ;
	}

	Vector<REAL> mul (const Vector<REAL> &that) const ;

	inline Vector<REAL> operator* (const Vector<REAL> &that) const {
		return mul (that) ;
	}

	Matrix transpose () const {
		Matrix ret ;
		const auto r1x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : ArrayRange<ARGC<2>> (r1x))
			ret.get (i[1] ,i[0]) = get (i[0] ,i[1]) ;
		return std::move (ret) ;
	}

	Matrix triangular () const {
		Matrix ret = (*this) ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			if SWITCH_CASE (TRUE) {
				INDEX ix = ret.find_max_row (i) ;
				if (ix == i)
					discard ;
				for (auto &&j : _RANGE_ (i ,4)) {
					const auto r1x = -ret.get (i ,j) ;
					ret.get (i ,j) = ret.get (ix ,j) ;
					ret.get (ix ,j) = r1x ;
				}
			}
			const auto r2x = _PINV_ (ret.get (i ,i)) ;
			if (r2x == REAL (0))
				continue ;
			for (auto &&j : _RANGE_ (i + 1 ,4)) {
				const auto r3x = ret.get (j ,i) * r2x ;
				for (auto &&k : _RANGE_ (i + 1 ,4))
					ret.get (j ,k) -= r3x * ret.get (i ,k) ;
				ret.get (j ,i) = REAL (0) ;
			}
		}
		return std::move (ret) ;
	}

	LENGTH rank () const {
		LENGTH ret = 0 ;
		const auto r1x = triangular () ;
		for (auto &&i : _RANGE_ (0 ,4))
			ret += EFLAG (_PINV_ (r1x[i][i]) == REAL (0)) ;
		ret = 4 - ret ;
		return std::move (ret) ;
	}

	REAL det () const {
		const auto r1x = triangular () ;
		REAL ret = REAL (1) ;
		for (auto &&i : _RANGE_ (0 ,4))
			ret *= r1x.get (i ,i) ;
		return std::move (ret) ;
	}

	Matrix inverse () const {
		Matrix ret ;
		const auto r1x = _PINV_ (det ()) ;
		_DYNAMIC_ASSERT_ (r1x != REAL (0)) ;
		for (auto &&i : _RANGE_ (0 ,4)) {
			INDEX ix = 0 ;
			ix += EFLAG (ix == i) ;
			INDEX iy = ix + 1 ;
			iy += EFLAG (iy == i) ;
			INDEX iz = iy + 1 ;
			iz += EFLAG (iz == i) ;
			for (auto &&j : _RANGE_ (0 ,4)) {
				INDEX jx = 0 ;
				jx += EFLAG (jx == j) ;
				INDEX jy = jx + 1 ;
				jy += EFLAG (jy == j) ;
				INDEX jz = jy + 1 ;
				jz += EFLAG (jz == j) ;
				const auto r2x = get (ix ,jx) * (get (iy ,jy) * get (iz ,jz) - get (iz ,jy) * get (iy ,jz)) ;
				const auto r3x = get (iy ,jx) * (get (ix ,jy) * get (iz ,jz) - get (iz ,jy) * get (ix ,jz)) ;
				const auto r4x = get (iz ,jx) * (get (ix ,jy) * get (iy ,jz) - get (iy ,jy) * get (ix ,jz)) ;
				const auto r5x = r2x - r3x + r4x ;
				const auto r6x = _SWITCH_ (
					((i + j) % 2 != 0) ? -r5x :
					r5x) ;
				ret.get (j ,i) = r6x ;
			}
		}
		ret *= r1x ;
		if SWITCH_CASE (TRUE) {
			if (!affine_matrix_like ())
				discard ;
			if (get (3 ,3) != REAL (1))
				discard ;
			if (!ret.affine_matrix_like ())
				discard ;
			const auto r7x = _PINV_ (ret.get (3 ,3)) ;
			ret *= r7x ;
			ret.get (3 ,3) = REAL (1) ;
		}
		return std::move (ret) ;
	}

	REAL trace () const {
		REAL ret = REAL (0) ;
		for (auto &&i : _RANGE_ (0 ,4))
			ret += get (i ,i) ;
		return std::move (ret) ;
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
		const auto r9x = _SIGN_ ((r5x ^ r6x) * r7x) * _PINV_ (r8x[3]) ;
		const auto r11x = r5x.magnitude () * r9x ;
		const auto r12x = r6x.magnitude () * r9x ;
		const auto r13x = r7x.magnitude () * r9x ;
		ret[1] = Matrix::make_diag (r11x ,r12x ,r13x ,REAL (1)) ;
		ret[2] = Matrix::make_view (r5x ,r6x) ;
		const auto r14x = r8x.projection () - Vector<REAL>::axis_w () ;
		ret[3] = Matrix::make_translation (r14x) ;
		return std::move (ret) ;
	}

private:
	BOOL affine_matrix_like () const {
		if (get (3 ,0) != REAL (0))
			return FALSE ;
		if (get (3 ,1) != REAL (0))
			return FALSE ;
		if (get (3 ,2) != REAL (0))
			return FALSE ;
		if (_PINV_ (get (3 ,3)) == REAL (0))
			return FALSE ;
		return TRUE ;
	}

	INDEX find_max_row (INDEX yx) const {
		INDEX ret = VAR_NONE ;
		auto rax = REAL () ;
		for (auto &&i : _RANGE_ (yx ,4)) {
			const auto r1x = _ABS_ (get (i ,yx)) ;
			if (ret != VAR_NONE)
				if (rax >= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return std::move (ret) ;
	}

public:
	static Matrix make_identity () {
		Matrix ret = Matrix ({
			{REAL (1) ,REAL (0) ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,REAL (1) ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (1) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_diag (const REAL &x ,const REAL &y ,const REAL &z ,const REAL &w) {
		Matrix ret = Matrix ({
			{x ,REAL (0) ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,y ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,z ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,w}}) ;
		return std::move (ret) ;
	}

	static Matrix make_shear (const Vector<REAL> &vx ,const Vector<REAL> &vy ,const Vector<REAL> &vz) {
		_DEBUG_ASSERT_ (vx[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (vy[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (vz[3] == REAL (0)) ;
		const auto r1x = vx.normalize () ;
		const auto r2x = vy.normalize () ;
		const auto r3x = vz.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = r2x * r3x ;
		const auto r7x = _SQRT_ (REAL (1) - _SQE_ (r4x)) ;
		const auto r8x = (r6x - r4x * r5x) * _PINV_ (r7x) ;
		const auto r9x = _SQRT_ (REAL (1) - _SQE_ (r5x) - _SQE_ (r8x)) ;
		Matrix ret = Matrix ({
			{REAL (1) ,r4x ,r5x ,REAL (0)} ,
			{REAL (0) ,r7x ,r8x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,r9x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_rotation (const Vector<REAL> &normal ,const REAL &angle) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		Matrix ret ;
		const auto r1x = normal.normalize () ;
		const auto r2x = _COS_ (angle) ;
		const auto r3x = r1x * _SIN_ (angle) ;
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
		return std::move (ret) ;
	}

	static Matrix make_rotation (const REAL &qx ,const REAL &qy ,const REAL &qz ,const REAL &qw) {
		Matrix ret ;
		const auto r1x = _SQE_ (qx) + _SQE_ (qy) + _SQE_ (qz) + _SQE_ (qw) ;
		const auto r2x = REAL (2) * _PINV_ (r1x) ;
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
		return std::move (ret) ;
	}

	static ARRAY4<REAL> make_rotation_quat (const Matrix &rot_mat) {
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
			return std::move (ret) ;
		}) ;
		const auto r5x = _PINV_ (REAL (2) * _SQRT_ (r3x[r4x])) ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(r4x == 0))
				discard ;
			ret[0] = (r2x[2][1] - r2x[1][2]) * r5x ;
			ret[1] = (r2x[0][2] - r2x[2][0]) * r5x ;
			ret[2] = (r2x[1][0] - r2x[0][1]) * r5x ;
			ret[3] = r3x[0] * r5x ;
		}
		if SWITCH_CASE (fax) {
			if (!(r4x == 1))
				discard ;
			ret[0] = r3x[1] * r5x ;
			ret[1] = (r2x[1][0] + r2x[0][1]) * r5x ;
			ret[2] = (r2x[0][2] + r2x[2][0]) * r5x ;
			ret[3] = (r2x[2][1] - r2x[1][2]) * r5x ;
		}
		if SWITCH_CASE (fax) {
			if (!(r4x == 2))
				discard ;
			ret[0] = (r2x[1][0] + r2x[0][1]) * r5x ;
			ret[1] = r3x[2] * r5x ;
			ret[2] = (r2x[2][1] + r2x[1][2]) * r5x ;
			ret[3] = (r2x[0][2] - r2x[2][0]) * r5x ;
		}
		if SWITCH_CASE (fax) {
			if (!(r4x == 3))
				discard ;
			ret[0] = (r2x[0][2] + r2x[2][0]) * r5x ;
			ret[1] = (r2x[2][1] + r2x[1][2]) * r5x ;
			ret[2] = r3x[3] * r5x ;
			ret[3] = (r2x[1][0] - r2x[0][1]) * r5x ;
		}
		return std::move (ret) ;
	}

	static ARRAY3<REAL> make_rotation_axis (const Matrix &rot_mat) {
		ARRAY3<REAL> ret ;
		const auto r1x = make_rotation_quat (rot_mat) ;
		const auto r2x = Vector<REAL> {r1x[0] ,r1x[1] ,r1x[2] ,REAL (0)}.magnitude () ;
		const auto r3x = r2x * _SIGN_ (r1x[3]) ;
		const auto r4x = _ATAN_ (r3x ,_ABS_ (r1x[3])) ;
		const auto r5x = (REAL (1) - (r2x - r4x) * _PINV_ (r2x)) * REAL (2) ;
		ret[0] = r1x[0] * r5x ;
		ret[1] = r1x[1] * r5x ;
		ret[2] = r1x[2] * r5x ;
		return std::move (ret) ;
	}

	static Matrix make_translation (const Vector<REAL> &direction) {
		_DEBUG_ASSERT_ (direction[3] == REAL (0)) ;
		Matrix ret = Matrix ({
			{REAL (1) ,REAL (0) ,REAL (0) ,direction[0]} ,
			{REAL (0) ,REAL (1) ,REAL (0) ,direction[1]} ,
			{REAL (0) ,REAL (0) ,REAL (1) ,direction[2]} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_view (const Vector<REAL> &vx ,const Vector<REAL> &vy) {
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
		return std::move (ret) ;
	}

	static Matrix make_view_xy (const Vector<REAL> &vx ,const Vector<REAL> &vy) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vx ,vy) * r1x ;
	}

	static Matrix make_view_zx (const Vector<REAL> &vz ,const Vector<REAL> &vx) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vz ,vx) * r1x ;
	}

	static Matrix make_view_yz (const Vector<REAL> &vy ,const Vector<REAL> &vz) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vy ,vz) * r1x ;
	}

	static Matrix make_view_yx (const Vector<REAL> &vy ,const Vector<REAL> &vx) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_x () ,
			-Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vy ,vx) * r1x ;
	}

	static Matrix make_view_xz (const Vector<REAL> &vx ,const Vector<REAL> &vz) {
		const auto r1x = Matrix ({
			Vector<REAL>::axis_x () ,
			-Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vx ,vz) * r1x ;
	}

	static Matrix make_view_zy (const Vector<REAL> &vz ,const Vector<REAL> &vy) {
		const auto r1x = Matrix ({
			-Vector<REAL>::axis_z () ,
			Vector<REAL>::axis_y () ,
			Vector<REAL>::axis_x () ,
			Vector<REAL>::axis_w ()}) ;
		return make_view (vz ,vy) * r1x ;
	}

	static Matrix make_view (const Vector<REAL> &px ,const Vector<REAL> &py ,const Vector<REAL> &pw) {
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
		return std::move (ret) ;
	}

	static Matrix make_perspective (const REAL &fx ,const REAL &fy ,const REAL &wx ,const REAL &wy) {
		_DEBUG_ASSERT_ (fx > REAL (0)) ;
		_DEBUG_ASSERT_ (fy > REAL (0)) ;
		Matrix ret = Matrix ({
			{fx ,REAL (0) ,wx ,REAL (0)} ,
			{REAL (0) ,fy ,wy ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)} ,
			{REAL (0) ,REAL (0) ,REAL (1) ,REAL (0)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_projection (const Vector<REAL> &normal ,const REAL &center ,const Vector<REAL> &light) {
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
		return std::move (ret) ;
	}

	static Matrix make_cross_product (const Vector<REAL> &first) {
		_DEBUG_ASSERT_ (first[3] == REAL (0)) ;
		Matrix ret = Matrix ({
			{REAL (0) ,-first[2] ,first[1] ,REAL (0)} ,
			{first[2] ,REAL (0) ,-first[0] ,REAL (0)} ,
			{-first[1] ,first[0] ,REAL (0) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (0)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_symmetry (const Vector<REAL> &first ,const Vector<REAL> &second) {
		Matrix ret ;
		const auto r1x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : ArrayRange<ARGC<2>> (r1x))
			ret[i[0]][i[1]] = first[i[0]] * second[i[1]] ;
		return std::move (ret) ;
	}

	static Matrix make_reflection (const Vector<REAL> &normal) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		const auto r1x = normal.normalize () ;
		return make_identity () - make_symmetry (r1x ,r1x) * REAL (2) ;
	}
} ;

template <class REAL>
inline Vector<REAL> Vector<REAL>::mul (const Matrix<REAL> &that) const {
	Vector<REAL> ret ;
	for (auto &&i : _RANGE_ (0 ,4)) {
		const auto r1x = get (0) * that.get (0 ,i) ;
		const auto r2x = get (1) * that.get (1 ,i) ;
		const auto r3x = get (2) * that.get (2 ,i) ;
		const auto r4x = get (3) * that.get (3 ,i) ;
		ret.get (i) = r1x + r2x + r3x + r4x ;
	}
	return std::move (ret) ;
}

template <class REAL>
inline Vector<REAL> Matrix<REAL>::mul (const Vector<REAL> &that) const {
	Vector<REAL> ret ;
	for (auto &&i : _RANGE_ (0 ,4)) {
		const auto r1x = get (i ,0) * that.get (0) ;
		const auto r2x = get (i ,1) * that.get (1) ;
		const auto r3x = get (i ,2) * that.get (2) ;
		const auto r4x = get (i ,3) * that.get (3) ;
		ret.get (i) = r1x + r2x + r3x + r4x ;
	}
	return std::move (ret) ;
}
} ;