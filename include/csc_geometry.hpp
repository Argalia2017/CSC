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
		return ARRAY3<REAL> {mVector[0] ,mVector[1] ,mVector[2]} ;
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
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
			ret.mVector[i] = mVector[i] * scale ;
		return std::move (ret) ;
	}

	inline Vector operator* (const REAL &scale) const {
		return mul (scale) ;
	}

	void multo (const REAL &scale) {
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
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

	void divto (const REAL &scale) {
		multo (_PINV_ (scale)) ;
	}

	inline Vector &operator/= (const REAL &scale) {
		divto (scale) ;
		return (*this) ;
	}

	Vector add (const Vector &that) const {
		Vector ret ;
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
			ret.mVector[i] = mVector[i] + that.mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator+ (const Vector &that) const {
		return add (that) ;
	}

	void addto (const Vector &that) {
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
			mVector[i] += that.mVector[i] ;
	}

	inline Vector &operator+= (const Vector &that) {
		addto (that) ;
		return (*this) ;
	}

	Vector sub (const Vector &that) const {
		Vector ret ;
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
			ret.mVector[i] = mVector[i] - that.mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator- (const Vector &that) const {
		return sub (that) ;
	}

	void subto (const Vector &that) {
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
			mVector[i] -= that.mVector[i] ;
	}

	inline Vector &operator-= (const Vector &that) {
		subto (that) ;
		return (*this) ;
	}

	Vector plus () const {
		Vector ret ;
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
			ret.mVector[i] = +mVector[i] ;
		return std::move (ret) ;
	}

	inline Vector operator+ () const {
		return plus () ;
	}

	Vector minus () const {
		Vector ret ;
		for (INDEX i = 0 ,ie = mVector.size () ; i < ie ; i++)
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
		_DEBUG_ASSERT_ (mVector[3] == REAL (0)) ;
		Vector ret ;
		const auto r1x = _PINV_ (magnitude ()) ;
		ret.mVector[0] = mVector[0] * r1x ;
		ret.mVector[1] = mVector[1] * r1x ;
		ret.mVector[2] = mVector[2] * r1x ;
		ret.mVector[3] = REAL (0) ;
		return std::move (ret) ;
	}

	Vector projection () const {
		_DEBUG_ASSERT_ (mVector[3] != REAL (0)) ;
		Vector ret ;
		const auto r1x = _PINV_ (mVector[3]) ;
		ret.mVector[0] = mVector[0] * r1x ;
		ret.mVector[1] = mVector[1] * r1x ;
		ret.mVector[2] = REAL (0) ;
		ret.mVector[3] = REAL (1) ;
		return std::move (ret) ;
	}

	Vector homogenize () const {
		Vector ret ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DEBUG_ASSERT_ (FALSE) ;
		return std::move (ret) ;
	}

	Vector reflect (const Vector<REAL> &normal) const {
		const auto r1x = normal * (2 * mul (normal)) ;
		return sub (r1x) ;
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

		inline void operator= (const ARRAY4<REAL> &row) && {
			for (INDEX i = 0 ,ie = 4 ; i < ie ; i++)
				mBase.get (mY ,i) = row[i] ;
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
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++) {
			get (0 ,i) = hx[i] ;
			get (1 ,i) = hy[i] ;
			get (2 ,i) = hz[i] ;
			get (3 ,i) = hw[i] ;
		}
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<_ARG1 ,REAL>::value>>
	implicit Matrix (const Vector<_ARG1> &hx ,const Vector<_ARG1> &hy ,const Vector<_ARG1> &hz ,const Vector<_ARG1> &hw) {
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++) {
			get (i ,0) = hx[i] ;
			get (i ,1) = hy[i] ;
			get (i ,2) = hz[i] ;
			get (i ,3) = hw[i] ;
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
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			ret.mMatrix[i] = mMatrix[i] * scale ;
		return std::move (ret) ;
	}

	inline Matrix operator* (const REAL &scale) const {
		return mul (scale) ;
	}

	void multo (const REAL &scale) {
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
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

	void divto (const REAL &scale) {
		multo (_PINV_ (scale)) ;
	}

	inline Matrix &operator/= (const REAL &scale) {
		divto (scale) ;
		return (*this) ;
	}

	Matrix add (const Matrix &that) const {
		Matrix ret ;
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			ret.mMatrix[i] = mMatrix[i] + that.mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ (const Matrix &that) const {
		return add (that) ;
	}

	void addto (const Matrix &that) {
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			mMatrix[i] += that.mMatrix[i] ;
	}

	inline Matrix &operator+= (const Matrix &that) {
		addto (that) ;
		return (*this) ;
	}

	Matrix sub (const Matrix &that) const {
		Matrix ret ;
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			ret.mMatrix[i] = mMatrix[i] - that.mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator- (const Matrix &that) const {
		return sub (that) ;
	}

	void subto (const Matrix &that) {
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			mMatrix[i] -= that.mMatrix[i] ;
	}

	inline Matrix &operator-= (const Matrix &that) {
		subto (that) ;
		return (*this) ;
	}

	Matrix plus () const {
		Matrix ret ;
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			ret.mMatrix[i] = +mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ () const {
		return plus () ;
	}

	Matrix minus () const {
		Matrix ret ;
		for (INDEX i = 0 ,ie = mMatrix.size () ; i < ie ; i++)
			ret.mMatrix[i] = -mMatrix[i] ;
		return std::move (ret) ;
	}

	inline Matrix operator- () const {
		return minus () ;
	}

	Matrix mul (const Matrix &that) const {
		Matrix ret ;
		const auto r5x = ARRAY2<LENGTH> {4 ,4} ;
		for (auto &&i : ArrayRange<ARGC<2>> (r5x)) {
			const auto r1x = get (i[0] ,0) * that.get (0 ,i[1]) ;
			const auto r2x = get (i[0] ,1) * that.get (1 ,i[1]) ;
			const auto r3x = get (i[0] ,2) * that.get (2 ,i[1]) ;
			const auto r4x = get (i[0] ,3) * that.get (3 ,i[1]) ;
			ret.get (i[0] ,i[1]) = r1x + r2x + r3x + r4x ;
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
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++) {
			for (FOR_ONCE_DO) {
				INDEX ix = ret.max_row_one (i) ;
				if (ix == i)
					discard ;
				for (INDEX j = i ,je = 4 ; j < je ; j++) {
					const auto r1x = -ret.get (i ,j) ;
					ret.get (i ,j) = ret.get (ix ,j) ;
					ret.get (ix ,j) = r1x ;
				}
			}
			const auto r2x = _PINV_ (ret.get (i ,i)) ;
			if (r2x == REAL (0))
				continue ;
			for (INDEX j = i + 1 ,je = 4 ; j < je ; j++) {
				const auto r3x = ret.get (j ,i) * r2x ;
				for (INDEX k = i + 1 ,ke = 4 ; k < ke ; k++)
					ret.get (j ,k) -= r3x * ret.get (i ,k) ;
				ret.get (j ,i) = REAL (0) ;
			}
		}
		return std::move (ret) ;
	}

	LENGTH rank () const {
		LENGTH ret = 0 ;
		const auto r1x = triangular () ;
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++)
			ret += EFLAG (r1x[i][i] == REAL (0)) ;
		ret = 4 - ret ;
		return std::move (ret) ;
	}

	REAL det () const {
		const auto r1x = triangular () ;
		REAL ret = REAL (1) ;
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++)
			ret *= r1x.get (i ,i) ;
		return std::move (ret) ;
	}

	Matrix inverse () const {
		Matrix ret ;
		const auto r1x = det () ;
		_DYNAMIC_ASSERT_ (r1x != REAL (0)) ;
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++) {
			INDEX ix = 0 ;
			ix += EFLAG (ix == i) ;
			INDEX iy = ix + 1 ;
			iy += EFLAG (iy == i) ;
			INDEX iz = iy + 1 ;
			iz += EFLAG (iz == i) ;
			for (INDEX j = 0 ,je = 4 ; j < je ; j++) {
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
					((i + j) % 2 != 0) ? (-r5x) :
					r5x) ;
				ret.get (j ,i) = r6x ;
			}
		}
		ret *= _PINV_ (r1x) ;
		for (FOR_ONCE_DO) {
			if (get (3 ,3) != REAL (1))
				discard ;
			if (!affine_matrix_like ())
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
		for (INDEX i = 0 ,ie = 4 ; i < ie ; i++)
			ret += get (i ,i) ;
		return std::move (ret) ;
	}

	//@info: 4-projection * 3-translation * 2-rotation * 1-shear * 0-scale
	ARRAY5<Matrix> decompose () const {
		_DEBUG_ASSERT_ (affine_matrix_like ()) ;
		ARRAY5<Matrix> ret ;
		const auto r1x = mul (Vector<REAL>::axis_x ()) ;
		const auto r2x = mul (Vector<REAL>::axis_y ()) ;
		const auto r3x = mul (Vector<REAL>::axis_z ()) ;
		ret[0] = Matrix::make_diag (r1x.magnitude () ,r2x.magnitude () ,r3x.magnitude () ,REAL (1)) ;
		const auto r4x = r1x.normalize () ;
		const auto r5x = r2x.normalize () ;
		const auto r6x = r3x.normalize () ;
		ret[1] = Matrix::make_shear (r4x ,r5x ,r6x) ;
		const auto r7x = Matrix {r4x ,r5x ,r6x ,Vector<REAL>::axis_w ()} ;
		ret[2] = r7x * ret[1].inverse () ;
		const auto r8x = mul (Vector<REAL>::axis_w ()) - Vector<REAL>::axis_w () ;
		ret[3] = Matrix::make_translation (r8x) ;
		ret[4] = Matrix::make_diag (REAL (1) ,REAL (1) ,REAL (1) ,r8x[3]) ;
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
		if (get (3 ,3) == REAL (0))
			return FALSE ;
		return TRUE ;
	}

	INDEX max_row_one (INDEX yx) const {
		INDEX ret = VAR_NONE ;
		auto rax = REAL () ;
		for (INDEX i = yx ,ie = 4 ; i < ie ; i++) {
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

	static Matrix make_shear (const REAL &angle_xy ,const REAL &angle_xz ,const REAL &angle_yz) {
		const auto r1x = _PINV_ (_SIN_ (angle_xy)) ;
		const auto r2x = (_COS_ (angle_yz) - _COS_ (angle_xy) * _COS_ (angle_xz)) * r1x ;
		const auto r3x = _SQRT_ (_SQE_ (_SIN_ (angle_xz)) - _SQE_ (r2x)) ;
		Matrix ret = Matrix ({
			{REAL (1) ,_COS_ (angle_xy) ,_COS_ (angle_xz) ,REAL (0)} ,
			{REAL (0) ,_SIN_ (angle_xy) ,r2x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,r3x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_shear (const Vector<REAL> &x ,const Vector<REAL> &y ,const Vector<REAL> &z) {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = z.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = _SQRT_ (1 - _SQE_ (r4x)) ;
		const auto r7x = (r2x * r3x - r4x * r5x) * _PINV_ (r6x) ;
		const auto r8x = _SQRT_ (1 - _SQE_ (r5x) - _SQE_ (r7x)) ;
		Matrix ret = Matrix ({
			{REAL (1) ,r4x ,r5x ,REAL (0)} ,
			{REAL (0) ,r6x ,r7x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,r8x ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_rotation (const Vector<REAL> &normal ,const REAL &angle) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		const auto r1x = normal.normalize () ;
		const auto r2x = _COS_ (angle) ;
		const auto r3x = r1x * _SIN_ (angle) ;
		const auto r4x = r1x * (REAL (1) - r2x) ;
		Matrix ret = Matrix ({
			{(r1x[0] * r4x[0] + r2x) ,(r1x[0] * r4x[1] - r3x[2]) ,(r1x[0] * r4x[2] + r3x[1]) ,REAL (0)} ,
			{(r1x[1] * r4x[0] + r3x[2]) ,(r1x[1] * r4x[1] + r2x) ,(r1x[1] * r4x[2] - r3x[0]) ,REAL (0)} ,
			{(r1x[2] * r4x[0] - r3x[1]) ,(r1x[2] * r4x[1] + r3x[0]) ,(r1x[2] * r4x[2] + r2x) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_rotation (const REAL &qx ,const REAL &qy ,const REAL &qz ,const REAL &qw) {
		const auto r1x = _SQE_ (qx) + _SQE_ (qy) + _SQE_ (qz) + _SQE_ (qw) ;
		const auto r2x = REAL (2) * _PINV_ (r1x) ;
		const auto r3x = qx * r2x ;
		const auto r4x = qy * r2x ;
		const auto r5x = qz * r2x ;
		const auto r6x = qw * r2x ;
		Matrix ret = Matrix ({
			{(REAL (1) - qy * r4x - qz * r5x) ,(qx * r4x - qz * r6x) ,(qx * r5x + qy * r6x) ,REAL (0)} ,
			{(qx * r4x + qz * r6x) ,(REAL (1) - qx * r3x - qz * r5x) ,(qy * r5x - qx * r6x) ,REAL (0)} ,
			{(qx * r5x - qy * r6x) ,(qy * r5x + qx * r6x) ,(REAL (1) - qx * r3x - qy * r4x) ,REAL (0)} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static ARRAY4<REAL> make_rotation_quat (const Matrix &rotation) {
		ARRAY4<REAL> ret ;
		const auto r1x = rotation.decompose () ;
		const auto r2x = r1x[2] ;
		const auto r5x = REAL (1) + r2x[0][0] + r2x[1][1] + r2x[2][2] ;
		_DEBUG_ASSERT_ (r5x >= REAL (0)) ;
		const auto r3x = REAL (2) * _SQRT_ (r5x) ;
		const auto r4x = _PINV_ (r3x) ;
		ret[0] = (r2x[2][1] - r2x[1][2]) * r4x ;
		ret[1] = (r2x[0][2] - r2x[2][0]) * r4x ;
		ret[2] = (r2x[1][0] - r2x[0][1]) * r4x ;
		ret[3] = r3x / REAL (4) ;
		return std::move (ret) ;
	}

	static ARRAY3<REAL> make_rotation_axis (const Matrix &rotation) {
		ARRAY3<REAL> ret ;
		const auto r1x = make_rotation_quat (rotation) ;
		const auto r2x = Vector<REAL> {r1x[0] ,r1x[1] ,r1x[2] ,0}.magnitude () ;
		const auto r3x = _SWITCH_ (
			(r2x != REAL (0)) ? (REAL (2) * _ATAN_ (r2x * _SIGN_ (r1x[3]) ,_ABS_ (r1x[3])) / r2x) :
			(REAL (2))) ;
		ret[0] = r1x[0] * r3x ;
		ret[1] = r1x[1] * r3x ;
		ret[2] = r1x[2] * r3x ;
		return std::move (ret) ;
	}

	static Matrix make_translation (const Vector<REAL> &position) {
		const auto r1x = -position * _PINV_ (position[3]) ;
		const auto r2x = _SWITCH_ (
			(position[3] != REAL (0)) ? r1x :
			position) ;
		Matrix ret = Matrix ({
			{REAL (1) ,REAL (0) ,REAL (0) ,r2x[0]} ,
			{REAL (0) ,REAL (1) ,REAL (0) ,r2x[1]} ,
			{REAL (0) ,REAL (0) ,REAL (1) ,r2x[2]} ,
			{REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)}}) ;
		return std::move (ret) ;
	}

	static Matrix make_view (const Vector<REAL> &normal ,const Vector<REAL> &center) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (center[3] == REAL (1)) ;
		const auto r1x = normal.normalize () ;
		_DEBUG_ASSERT_ (r1x.magnitude () > REAL (0)) ;
		const auto r2x = Vector<REAL> {_ABS_ (normal[0]) ,_ABS_ (normal[1]) ,_ABS_ (normal[2]) ,REAL (0)} ;
		const auto r3x = _SWITCH_ (
			(r2x[0] < r2x[2]) ? (Vector<REAL>::axis_x ()) :
			(Vector<REAL>::axis_z ())) ;
		const auto r4x = _SWITCH_ (
			(r2x[1] < r2x[2]) ? (Vector<REAL>::axis_y ()) :
			(Vector<REAL>::axis_z ())) ;
		const auto r5x = _SWITCH_ (
			(r2x[0] < r2x[1]) ? r3x :
			r4x) ;
		const auto r6x = (r1x ^ r5x).normalize () ;
		const auto r7x = (r1x ^ r6x).normalize () ;
		return Matrix {r6x ,r7x ,r1x ,center} ;
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

	static Matrix make_projection (const Vector<REAL> &normal ,const Vector<REAL> &center ,const Vector<REAL> &light) {
		_DEBUG_ASSERT_ (normal[3] == REAL (0)) ;
		_DEBUG_ASSERT_ (center[3] == REAL (1)) ;
		const auto r1x = normal.normalize () ;
		_DEBUG_ASSERT_ (r1x.magnitude () > REAL (0)) ;
		const auto r8x = light * _PINV_ (light) ;
		const auto r2x = _SWITCH_ (
			(light[3] != REAL (0)) ? r8x :
			(light.normalize ())) ;
		const auto r3x = (center - Vector<REAL>::axis_w ()) * r1x ;
		const auto r9x = Vector<REAL> {r2x[0] ,r2x[1] ,r2x[2] ,REAL (0)} ;
		const auto r4x = r9x * r1x ;
		const auto r7x = Vector<REAL> {REAL (0) ,REAL (0) ,REAL (0) ,REAL (0)} ;
		const auto r5x = _SWITCH_ (
			(r2x[3] != REAL (0)) ? r3x :
			r7x) ;
		const auto r6x = _SWITCH_ (
			(r2x[3] != REAL (0)) ? r1x :
			r7x) ;
		Matrix ret = Matrix ({
			{(r1x[0] * r2x[0] - r4x + r5x) ,(r1x[1] * r2x[0]) ,(r1x[2] * r2x[0]) ,(-r3x * r2x[0])} ,
			{(r1x[0] * r2x[1]) ,(r1x[1] * r2x[1] - r4x + r5x) ,(r1x[2] * r2x[1]) ,(-r3x * r2x[1])} ,
			{(r1x[0] * r2x[2]) ,(r1x[1] * r2x[2]) ,(r1x[2] * r2x[2] - r4x + r5x) ,(-r3x * r2x[2])} ,
			{r6x[0] ,r6x[1] ,r6x[2] ,-r4x}}) ;
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
} ;

template <class REAL>
inline Vector<REAL> Vector<REAL>::mul (const Matrix<REAL> &that) const {
	Vector<REAL> ret ;
	for (INDEX i = 0 ,ie = 4 ; i < ie ; i++)
		ret.get (i) = get (0) * that.get (0 ,i) + get (1) * that.get (1 ,i) + get (2) * that.get (2 ,i) + get (3) * that.get (3 ,i) ;
	return std::move (ret) ;
}

template <class REAL>
inline Vector<REAL> Matrix<REAL>::mul (const Vector<REAL> &that) const {
	Vector<REAL> ret ;
	for (INDEX i = 0 ,ie = 4 ; i < ie ; i++)
		ret.get (i) = get (i ,0) * that.get (0) + get (i ,1) * that.get (1) + get (i ,2) * that.get (2) + get (i ,3) * that.get (3) ;
	return std::move (ret) ;
}
} ;