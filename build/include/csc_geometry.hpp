#pragma once

#ifndef __CSC_GEOMETRY__
#define __CSC_GEOMETRY__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class>
class Matrix ;

template <class UNIT>
class Vector {
private:
	_STATIC_ASSERT_ (std::is_same<UNIT ,VAL32>::value || std::is_same<UNIT ,VAL64>::value || std::is_same<UNIT ,VALX>::value) ;
	Buffer<UNIT ,ARGC<4>> mVector ;

public:
	Vector () = default ;

	implicit Vector (const ARRAY3<UNIT> &xyz ,const UNIT &w) : Vector (xyz[0] ,xyz[1] ,xyz[2] ,w) {}

	implicit Vector (const UNIT &x ,const UNIT &y ,const UNIT &z ,const UNIT &w) {
		mVector[0] = x ;
		mVector[1] = y ;
		mVector[2] = z ;
		mVector[3] = w ;
	}

	PhanBuffer<UNIT> raw () {
		return PhanBuffer<UNIT>::make (mVector) ;
	}

	PhanBuffer<const UNIT> raw () const {
		return PhanBuffer<const UNIT>::make (mVector) ;
	}

	const ARRAY3<UNIT> &xyz () const {
		return _CAST_<const PACK<ARRAY3<UNIT> ,UNIT>> (mVector).P1 ;
	}

	UNIT &get (INDEX x) {
		return mVector[x] ;
	}

	inline UNIT &operator[] (INDEX x) {
		return get (x) ;
	}

	const UNIT &get (INDEX x) const {
		return mVector[x] ;
	}

	inline const UNIT &operator[] (INDEX x) const {
		return get (x) ;
	}

	BOOL equal (const Vector &right) const {
		return mVector == right.mVector ;
	}

	inline BOOL operator== (const Vector &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Vector &right) const {
		return !equal (right) ;
	}

	BOOL less (const Vector &right) const {
		return mVector < right.mVector ;
	}

	inline BOOL operator< (const Vector &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Vector &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Vector &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Vector &right) const {
		return !right.less (*this) ;
	}

	Vector mul (const UNIT &scale) const {
		Vector ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret.mVector[i] = mVector[i] * scale ;
		return std::move (ret) ;
	}

	inline Vector operator* (const UNIT &scale) const {
		return mul (scale) ;
	}

	void multo (const UNIT &scale) {
		for (INDEX i = 0 ; i < 4 ; i++)
			mVector[i] *= scale ;
	}

	inline Vector &operator*= (const UNIT &scale) {
		multo (scale) ;
		return *this ;
	}

	Vector div (const UNIT &scale) const {
		return mul (_PINV_ (scale)) ;
	}

	inline Vector operator/ (const UNIT &scale) const {
		return div (scale) ;
	}

	void divto (const UNIT &scale) {
		multo (_PINV_ (scale)) ;
	}

	inline Vector &operator/= (const UNIT &scale) {
		divto (scale) ;
		return *this ;
	}

	Vector add (const Vector &right) const {
		Vector ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret.mVector[i] = mVector[i] + right.mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator+ (const Vector &right) const {
		return add (right) ;
	}

	void addto (const Vector &right) {
		for (INDEX i = 0 ; i < 4 ; i++)
			mVector[i] += right.mVector[i] ;
	}

	inline Vector &operator+= (const Vector &right) {
		addto (right) ;
		return *this ;
	}

	Vector sub (const Vector &right) const {
		Vector ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret.mVector[i] = mVector[i] - right.mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator- (const Vector &right) const {
		return sub (right) ;
	}

	void subto (const Vector &right) {
		for (INDEX i = 0 ; i < 4 ; i++)
			mVector[i] -= right.mVector[i] ;
	}

	inline Vector &operator-= (const Vector &right) {
		subto (right) ;
		return *this ;
	}

	Vector plus () const {
		Vector ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret.mVector[i] = +mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator+ () const {
		return plus () ;
	}

	Vector minus () const {
		Vector ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret.mVector[i] = -mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator- () const {
		return minus () ;
	}

	UNIT mul (const Vector &right) const {
		_DEBUG_ASSERT_ (mVector[3] == UNIT (0) && right.mVector[3] == UNIT (0)) ;
		return mVector[0] * right.mVector[0] + mVector[1] * right.mVector[1] + mVector[2] * right.mVector[2] ;
	}

	inline UNIT operator* (const Vector &right) const {
		return mul (right) ;
	}

	Vector mul (const Matrix<UNIT> &right) const ;

	inline Vector operator* (const Matrix<UNIT> &right) const {
		return mul (right) ;
	}

	void multo (const Matrix<UNIT> &right) {
		*this = mul (right) ;
	}

	inline Vector &operator*= (const Matrix<UNIT> &right) {
		multo (right) ;
		return *this ;
	}

	Vector cross (const Vector &right) const {
		_DEBUG_ASSERT_ (mVector[3] == UNIT (0) && right.mVector[3] == UNIT (0)) ;
		Vector ret ;
		ret.mVector[0] = mVector[1] * right.mVector[2] - mVector[2] * right.mVector[1] ;
		ret.mVector[1] = mVector[2] * right.mVector[0] - mVector[0] * right.mVector[2] ;
		ret.mVector[2] = mVector[0] * right.mVector[1] - mVector[1] * right.mVector[0] ;
		ret.mVector[3] = UNIT (0) ;
		return std::move (ret) ;
	}

	inline Vector operator^ (const Vector &right) const {
		return cross (right) ;
	}

	void crossto (const Vector &right) const {
		*this = cross (right) ;
	}

	inline Vector &operator^= (const Vector &right) {
		crossto (right) ;
		return *this ;
	}

	UNIT magnitude () const {
		_DEBUG_ASSERT_ (mVector[3] == UNIT (0)) ;
		return _SQRT_ (_SQE_ (mVector[0]) + _SQE_ (mVector[1]) + _SQE_ (mVector[2])) ;
	}

	Vector normalize () const {
		Vector ret ;
		const auto r1x = BOOL (mVector[3] == UNIT (0)) ;
		const auto r2x = r1x ? (magnitude ()) : (mVector[3]) ;
		const auto r3x = _PINV_ (r2x) ;
		ret.mVector[0] = mVector[0] * r3x ;
		ret.mVector[1] = mVector[1] * r3x ;
		ret.mVector[2] = mVector[2] * r3x ;
		ret.mVector[3] = r1x ? (UNIT (0)) : (UNIT (1)) ;
		return std::move (ret) ;
	}

	Vector projection () const {
		_DEBUG_ASSERT_ (mVector[3] != UNIT (0)) ;
		Vector ret ;
		const auto r1x = _PINV_ (mVector[3]) ;
		ret.mVector[0] = mVector[0] * r1x ;
		ret.mVector[1] = mVector[1] * r1x ;
		ret.mVector[2] = UNIT (0) ;
		ret.mVector[3] = UNIT (1) ;
		return std::move (ret) ;
	}

	Vector reflect (const Vector<UNIT> &normal) const {
		const auto r1x = normal * (2 * mul (normal)) ;
		return sub (r1x) ;
	}
} ;

template <class UNIT>
class Matrix {
private:
	template <class BASE>
	class Row {
	private:
		friend Matrix ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline Row (const Row &) = delete ;

		inline Row (Row &&) noexcept = default ;

		inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

		inline void operator= (const ARRAY4<UNIT> &row) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.get (mY ,i) = row[i] ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) popping :mBase (base) ,mY (y) {}
	} ;

private:
	_STATIC_ASSERT_ (std::is_same<UNIT ,VAL32>::value || std::is_same<UNIT ,VAL64>::value || std::is_same<UNIT ,VALX>::value) ;
	Buffer<UNIT ,ARGC<16>> mMatrix ;

public:
	Matrix () = default ;

	implicit Matrix (const ARRAY4<UNIT> &hx ,const ARRAY4<UNIT> &hy ,const ARRAY4<UNIT> &hz ,const ARRAY4<UNIT> &hw) {
		for (INDEX i = 0 ; i < 4 ; i++) {
			get (0 ,i) = hx[i] ;
			get (1 ,i) = hy[i] ;
			get (2 ,i) = hz[i] ;
			get (3 ,i) = hw[i] ;
		}
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<_ARG1 ,UNIT>::value>>
	implicit Matrix (const Vector<_ARG1> &hx ,const Vector<_ARG1> &hy ,const Vector<_ARG1> &hz ,const Vector<_ARG1> &hw) {
		for (INDEX i = 0 ; i < 4 ; i++) {
			get (i ,0) = hx[i] ;
			get (i ,1) = hy[i] ;
			get (i ,2) = hz[i] ;
			get (i ,3) = hw[i] ;
		}
	}

	PhanBuffer<UNIT> raw () {
		return PhanBuffer<UNIT>::make (mMatrix) ;
	}

	PhanBuffer<const UNIT> raw () const {
		return PhanBuffer<const UNIT>::make (mMatrix) ;
	}

	UNIT &get (INDEX y ,INDEX x) {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y * 4 + x] ;
	}

	const UNIT &get (INDEX y ,INDEX x) const {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y * 4 + x] ;
	}

	Row<Matrix> get (INDEX y) {
		return Row<Matrix> (*this ,y) ;
	}

	inline Row<Matrix> operator[] (INDEX y) {
		return get (y) ;
	}

	Row<const Matrix> get (INDEX y) const {
		return Row<const Matrix> (*this ,y) ;
	}

	inline Row<const Matrix> operator[] (INDEX y) const {
		return get (y) ;
	}

	BOOL equal (const Matrix &right) const {
		return mMatrix == right.mMatrix ;
	}

	inline BOOL operator== (const Matrix &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Matrix &right) const {
		return !equal (right) ;
	}

	BOOL less (const Matrix &right) const {
		return mMatrix < right.mMatrix ;
	}

	inline BOOL operator< (const Matrix &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Matrix &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Matrix &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Matrix &right) const {
		return !right.less (*this) ;
	}

	Matrix mul (const UNIT &scale) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			ret.mMatrix[i] = mMatrix[i] * scale ;
		return std::move (ret) ;
	}

	inline Matrix operator* (const UNIT &scale) const {
		return mul (scale) ;
	}

	void multo (const UNIT &scale) {
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			mMatrix[i] *= scale ;
	}

	inline Matrix &operator*= (const UNIT &scale) {
		multo (scale) ;
		return *this ;
	}

	Matrix div (const UNIT &scale) const {
		return mul (_PINV_ (scale)) ;
	}

	inline Matrix operator/ (const UNIT &scale) const {
		return div (scale) ;
	}

	void divto (const UNIT &scale) {
		multo (_PINV_ (scale)) ;
	}

	inline Matrix &operator/= (const UNIT &scale) {
		divto (scale) ;
		return *this ;
	}

	Matrix add (const Matrix &right) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			ret.mMatrix[i] = mMatrix[i] + right.mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ (const Matrix &right) const {
		return add (right) ;
	}

	void addto (const Matrix &right) {
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			mMatrix[i] += right.mMatrix[i] ;
	}

	inline Matrix &operator+= (const Matrix &right) {
		addto (right) ;
		return *this ;
	}

	Matrix sub (const Matrix &right) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			ret.mMatrix[i] = mMatrix[i] - right.mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator- (const Matrix &right) const {
		return sub (right) ;
	}

	void subto (const Matrix &right) {
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			mMatrix[i] -= right.mMatrix[i] ;
	}

	inline Matrix &operator-= (const Matrix &right) {
		subto (right) ;
		return *this ;
	}

	Matrix plus () const {
		Matrix ret ;
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			ret.mMatrix[i] = +mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ () const {
		return plus () ;
	}

	Matrix minus () const {
		Matrix ret ;
		for (INDEX i = 0 ; i < mMatrix.size () ; i++)
			ret.mMatrix[i] = -mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator- () const {
		return minus () ;
	}

	Matrix mul (const Matrix &right) const {
		Matrix ret ;
		for (auto &&i : RangeFolder<ARGC<2>> {4 ,4}) {
			const auto r1x = get (i[0] ,0) * right.get (0 ,i[1]) ;
			const auto r2x = get (i[0] ,1) * right.get (1 ,i[1]) ;
			const auto r3x = get (i[0] ,2) * right.get (2 ,i[1]) ;
			const auto r4x = get (i[0] ,3) * right.get (3 ,i[1]) ;
			ret.get (i[0] ,i[1]) = r1x + r2x + r3x + r4x ;
		}
		return std::move (ret) ;
	}

	inline Matrix operator* (const Matrix &right) const {
		return mul (right) ;
	}

	void multo (const Matrix &right) {
		*this = mul (right) ;
	}

	inline Matrix &operator*= (const Matrix &right) {
		multo (right) ;
		return *this ;
	}

	Vector<UNIT> mul (const Vector<UNIT> &right) const ;

	inline Vector<UNIT> operator* (const Vector<UNIT> &right) const {
		return mul (right) ;
	}

	Matrix transpose () const {
		Matrix ret ;
		for (auto &&i : RangeFolder<ARGC<2>> {4 ,4})
			ret.get (i[1] ,i[0]) = get (i[0] ,i[1]) ;
		return std::move (ret) ;
	}

	Matrix triangular () const {
		Matrix ret = *this ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = ret.max_row_one (i) ;
			if (ix != i) {
				for (INDEX j = i ; j < 4 ; j++) {
					const auto r1x = -ret.get (i ,j) ;
					ret.get (i ,j) = ret.get (ix ,j) ;
					ret.get (ix ,j) = r1x ;
				}
			}
			if (ret.get (i ,i) == 0)
				continue ;
			for (INDEX j = i + 1 ; j < 4 ; j++) {
				const auto r2x = ret.get (j ,i) / ret.get (i ,i) ;
				for (INDEX k = i + 1 ; k < 4 ; k++)
					ret.get (j ,k) -= r2x * ret.get (i ,k) ;
				ret.get (j ,i) = UNIT (0) ;
			}
		}
		return std::move (ret) ;
	}

	LENGTH rank () const {
		LENGTH ret = 0 ;
		const auto r1x = triangular () ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret += EFLAG (r1x[i][i] == UNIT (0)) ;
		ret = 4 - ret ;
		return std::move (ret) ;
	}

	UNIT det () const {
		const auto r1x = triangular () ;
		UNIT ret = UNIT (1) ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret *= r1x.get (i ,i) ;
		return std::move (ret) ;
	}

	Matrix inverse () const {
		Matrix ret ;
		const auto r1x = det () ;
		_DYNAMIC_ASSERT_ (r1x != UNIT (0)) ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = 0 ;
			ix += EFLAG (ix == i) ;
			INDEX iy = ix + 1 ;
			iy += EFLAG (iy == i) ;
			INDEX iz = iy + 1 ;
			iz += EFLAG (iz == i) ;
			for (INDEX j = 0 ; j < 4 ; j++) {
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
				ret.get (j ,i) = ((i + j) % 2 != 0) ? (-r5x) : r5x ;
			}
		}
		ret *= _PINV_ (r1x) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (get (3 ,3) != UNIT (1))
				break ;
			if (!affine_matrix_like ())
				break ;
			if (!ret.affine_matrix_like ())
				break ;
			const auto r6x = _PINV_ (ret.get (3 ,3)) ;
			ret *= r6x ;
			ret.get (3 ,3) = UNIT (1) ;
		}
		return std::move (ret) ;
	}

	UNIT trace () const {
		UNIT ret = UNIT (0) ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret += get (i ,i) ;
		return std::move (ret) ;
	}

	//@info: 4-projection * 3-translation * 2-rotation * 1-shear * 0-scale
	ARRAY5<Matrix> decompose () const {
		_DEBUG_ASSERT_ (affine_matrix_like ()) ;
		ARRAY5<Matrix> ret ;
		const auto r1x = mul ({UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)}) ;
		const auto r2x = mul ({UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)}) ;
		const auto r3x = mul ({UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) ;
		ret[0] = Matrix::make_diag (r1x.magnitude () ,r2x.magnitude () ,r3x.magnitude () ,UNIT (1)) ;
		const auto r4x = r1x.normalize () ;
		const auto r5x = r2x.normalize () ;
		const auto r6x = r3x.normalize () ;
		ret[1] = Matrix::make_shear (r4x ,r5x ,r6x) ;
		const auto r7x = Matrix {r4x ,r5x ,r6x ,Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}} ;
		ret[2] = r7x * ret[1].inverse () ;
		const auto r8x = mul ({UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}) ;
		ret[3] = Matrix::make_translation ({r8x[0] ,r8x[1] ,r8x[2] ,UNIT (0)}) ;
		ret[4] = Matrix::make_diag (UNIT (1) ,UNIT (1) ,UNIT (1) ,r8x[3]) ;
		return std::move (ret) ;
	}

private:
	BOOL affine_matrix_like () const {
		if (get (3 ,0) != UNIT (0))
			return FALSE ;
		if (get (3 ,1) != UNIT (0))
			return FALSE ;
		if (get (3 ,2) != UNIT (0))
			return FALSE ;
		if (get (3 ,3) == UNIT (0))
			return FALSE ;
		return TRUE ;
	}

	INDEX max_row_one (INDEX yx) const {
		INDEX ret = VAR_NONE ;
		auto rax = UNIT () ;
		for (INDEX i = yx ; i < 4 ; i++) {
			const auto r1x = _ABS_ (get (i ,yx)) ;
			if (ret != VAR_NONE && rax >= r1x)
				continue ;
			ret = i ;
			rax = r1x ;
		}
		return std::move (ret) ;
	}

public:
	static Matrix make_identity () {
		return Matrix ({
			{UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_diag (const UNIT &x ,const UNIT &y ,const UNIT &z ,const UNIT &w) {
		return Matrix ({
			{x ,UNIT (0) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,y ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,z ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,w}}) ;
	}

	static Matrix make_shear (const UNIT &angle_xy ,const UNIT &angle_xz ,const UNIT &angle_yz) {
		const auto r1x = _PINV_ (_SIN_ (angle_xy)) ;
		const auto r2x = (_COS_ (angle_yz) - _COS_ (angle_xy) * _COS_ (angle_xz)) * r1x ;
		const auto r3x = _SQRT_ (_SQE_ (_SIN_ (angle_xz)) - _SQE_ (r2x)) ;
		return Matrix ({
			{UNIT (1) ,_COS_ (angle_xy) ,_COS_ (angle_xz) ,UNIT (0)} ,
			{UNIT (0) ,_SIN_ (angle_xy) ,r2x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,r3x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_shear (const Vector<UNIT> &x ,const Vector<UNIT> &y ,const Vector<UNIT> &z) {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = z.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = _SQRT_ (1 - _SQE_ (r4x)) ;
		const auto r7x = (r2x * r3x - r4x * r5x) * _PINV_ (r6x) ;
		const auto r8x = _SQRT_ (1 - _SQE_ (r5x) - _SQE_ (r7x)) ;
		return Matrix ({
			{UNIT (1) ,r4x ,r5x ,UNIT (0)} ,
			{UNIT (0) ,r6x ,r7x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,r8x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_rotation (const Vector<UNIT> &normal ,const UNIT &angle) {
		_DEBUG_ASSERT_ (normal[3] == UNIT (0)) ;
		const auto r1x = normal.normalize () ;
		const auto r2x = _COS_ (angle) ;
		const auto r3x = r1x * _SIN_ (angle) ;
		const auto r4x = r1x * (UNIT (1) - r2x) ;
		return Matrix ({
			{(r1x[0] * r4x[0] + r2x) ,(r1x[0] * r4x[1] - r3x[2]) ,(r1x[0] * r4x[2] + r3x[1]) ,UNIT (0)} ,
			{(r1x[1] * r4x[0] + r3x[2]) ,(r1x[1] * r4x[1] + r2x) ,(r1x[1] * r4x[2] - r3x[0]) ,UNIT (0)} ,
			{(r1x[2] * r4x[0] - r3x[1]) ,(r1x[2] * r4x[1] + r3x[0]) ,(r1x[2] * r4x[2] + r2x) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_rotation (const UNIT &qx ,const UNIT &qy ,const UNIT &qz ,const UNIT &qw) {
		const auto r1x = _SQE_ (qx) + _SQE_ (qy) + _SQE_ (qz) + _SQE_ (qw) ;
		const auto r2x = UNIT (2) * _PINV_ (r1x) ;
		const auto r3x = qx * r2x ;
		const auto r4x = qy * r2x ;
		const auto r5x = qz * r2x ;
		const auto r6x = qw * r2x ;
		return Matrix ({
			{(UNIT (1) - qy * r4x - qz * r5x) ,(qx * r4x - qz * r6x) ,(qx * r5x + qy * r6x) ,UNIT (0)} ,
			{(qx * r4x + qz * r6x) ,(UNIT (1) - qx * r3x - qz * r5x) ,(qy * r5x - qx * r6x) ,UNIT (0)} ,
			{(qx * r5x - qy * r6x) ,(qy * r5x + qx * r6x) ,(UNIT (1) - qx * r3x - qy * r4x) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static ARRAY4<UNIT> make_rotation_quat (const Matrix &rotation) {
		ARRAY4<UNIT> ret ;
		const auto r1x = rotation.decompose ()[2] ;
		_DEBUG_ASSERT_ (UNIT (1) + r1x[0][0] + r1x[1][1] + r1x[2][2] >= UNIT (0)) ;
		const auto r2x = UNIT (2) * _SQRT_ (UNIT (1) + r1x[0][0] + r1x[1][1] + r1x[2][2]) ;
		const auto r3x = _PINV_ (r2x) ;
		ret[0] = (r1x[2][1] - r1x[1][2]) * r3x ;
		ret[1] = (r1x[0][2] - r1x[2][0]) * r3x ;
		ret[2] = (r1x[1][0] - r1x[0][1]) * r3x ;
		ret[3] = r2x / UNIT (4) ;
		return std::move (ret) ;
	}

	static ARRAY3<UNIT> make_rotation_axis (const Matrix &rotation) {
		ARRAY3<UNIT> ret ;
		const auto r1x = make_rotation_quat (rotation) ;
		const auto r2x = Vector<UNIT> {r1x[0] ,r1x[1] ,r1x[2] ,0}.magnitude () ;
		const auto r3x = (r2x != UNIT (0)) ? (UNIT (2) * _ATAN_ (r2x * _SIGN_ (r1x[3]) ,_ABS_ (r1x[3])) / r2x) : (UNIT (2)) ;
		ret[0] = r1x[0] * r3x ;
		ret[1] = r1x[1] * r3x ;
		ret[2] = r1x[2] * r3x ;
		return std::move (ret) ;
	}

	static Matrix make_translation (const Vector<UNIT> &direction) {
		const auto r1x = (direction[3] != UNIT (0)) ? (Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)} -direction.normalize ()) : direction ;
		return Matrix ({
			{UNIT (1) ,UNIT (0) ,UNIT (0) ,r1x[0]} ,
			{UNIT (0) ,UNIT (1) ,UNIT (0) ,r1x[1]} ,
			{UNIT (0) ,UNIT (0) ,UNIT (1) ,r1x[2]} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_view (const Vector<UNIT> &normal ,const Vector<UNIT> &center) {
		_DEBUG_ASSERT_ (normal[3] == UNIT (0) && center[3] == UNIT (1)) ;
		const auto r1x = normal.normalize () ;
		_DEBUG_ASSERT_ (r1x[0] != UNIT (0) || r1x[1] != UNIT (0) || r1x[2] != UNIT (0)) ;
		const auto r2x = Vector<UNIT> {_ABS_ (normal[0]) ,_ABS_ (normal[1]) ,_ABS_ (normal[2]) ,UNIT (0)} ;
		const auto r3x = (r2x[0] < r2x[2]) ? (Vector<UNIT> {UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)}) : (Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) ;
		const auto r4x = (r2x[1] < r2x[2]) ? (Vector<UNIT> {UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)}) : (Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) ;
		const auto r5x = (r2x[0] < r2x[1]) ? r3x : r4x ;
		const auto r6x = (r1x ^ r5x).normalize () ;
		const auto r7x = (r1x ^ r6x).normalize () ;
		return Matrix {r6x ,r7x ,r1x ,center} ;
	}

	static Matrix make_perspective (const UNIT &fx ,const UNIT &fy ,const UNIT &wx ,const UNIT &wy) {
		_DEBUG_ASSERT_ (fx > UNIT (0) && fy > UNIT (0)) ;
		return Matrix ({
			{fx ,UNIT (0) ,wx ,UNIT (0)} ,
			{UNIT (0) ,fy ,wy ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}}) ;
	}

	static Matrix make_projection (const Vector<UNIT> &normal ,const Vector<UNIT> &center ,const Vector<UNIT> &light) {
		_DEBUG_ASSERT_ (normal[3] == UNIT (0) && center[3] == UNIT (1)) ;
		const auto r1x = normal.normalize () ;
		_DEBUG_ASSERT_ (r1x[0] != UNIT (0) || r1x[1] != UNIT (0) || r1x[2] != UNIT (0)) ;
		const auto r2x = light.normalize () ;
		const auto r3x = Vector<UNIT> {center[0] ,center[1] ,center[2] ,UNIT (0)} *r1x ;
		const auto r4x = Vector<UNIT> {r2x[0] ,r2x[1] ,r2x[2] ,UNIT (0)} *r1x ;
		const auto r5x = (r2x[3] != UNIT (0)) ? r3x : (UNIT (0)) ;
		const auto r6x = (r2x[3] != UNIT (0)) ? (r1x.xyz ()) : (ARRAY3<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0)}) ;
		return Matrix ({
			{(r1x[0] * r2x[0] - r4x + r5x) ,(r1x[1] * r2x[0]) ,(r1x[2] * r2x[0]) ,(-r3x * r2x[0])} ,
			{(r1x[0] * r2x[1]) ,(r1x[1] * r2x[1] - r4x + r5x) ,(r1x[2] * r2x[1]) ,(-r3x * r2x[1])} ,
			{(r1x[0] * r2x[2]) ,(r1x[1] * r2x[2]) ,(r1x[2] * r2x[2] - r4x + r5x) ,(-r3x * r2x[2])} ,
			{r6x[0] ,r6x[1] ,r6x[2] ,-r4x}}) ;
	}

	static Matrix make_cross_product (const Vector<UNIT> &first) {
		_DEBUG_ASSERT_ (first[3] == UNIT (0)) ;
		return Matrix ({
			{UNIT (0) ,-first[2] ,first[1] ,UNIT (0)} ,
			{first[2] ,UNIT (0) ,-first[0] ,UNIT (0)} ,
			{-first[1] ,first[0] ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (0)}}) ;
	}

	static Matrix make_symmetry (const Vector<UNIT> &first ,const Vector<UNIT> &second) {
		return Matrix ({
			{(first[0] * second[0]) ,(first[0] * second[1]) ,(first[0] * second[2]) ,(first[0] * second[3])} ,
			{(first[1] * second[0]) ,(first[1] * second[1]) ,(first[1] * second[2]) ,(first[1] * second[3])} ,
			{(first[2] * second[0]) ,(first[2] * second[1]) ,(first[2] * second[2]) ,(first[2] * second[3])} ,
			{(first[3] * second[0]) ,(first[3] * second[1]) ,(first[3] * second[2]) ,(first[3] * second[3])}}) ;
	}
} ;

template <class UNIT>
inline Vector<UNIT> Vector<UNIT>::mul (const Matrix<UNIT> &right) const {
	Vector<UNIT> ret ;
	for (INDEX i = 0 ; i < 4 ; i++)
		ret.get (i) = get (0) * right.get (0 ,i) + get (1) * right.get (1 ,i) + get (2) * right.get (2 ,i) + get (3) * right.get (3 ,i) ;
	return std::move (ret) ;
}

template <class UNIT>
inline Vector<UNIT> Matrix<UNIT>::mul (const Vector<UNIT> &right) const {
	Vector<UNIT> ret ;
	for (INDEX i = 0 ; i < 4 ; i++)
		ret.get (i) = get (i ,0) * right.get (0) + get (i ,1) * right.get (1) + get (i ,2) * right.get (2) + get (i ,3) * right.get (3) ;
	return std::move (ret) ;
}
} ;