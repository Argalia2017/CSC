#pragma once

#ifndef __CSC_GRAPHICS__
#define __CSC_GRAPHICS__
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
	DEF<UNIT[4]> mVector ;

public:
	Vector () = default ;

	implicit Vector (const ARRAY3<UNIT> &xyz ,const UNIT &w) : Vector (xyz[0] ,xyz[1] ,xyz[2] ,w) {}

	implicit Vector (const UNIT &x ,const UNIT &y ,const UNIT &z ,const UNIT &w) {
		mVector[0] = x ;
		mVector[1] = y ;
		mVector[2] = z ;
		mVector[3] = w ;
	}

	const ARR<UNIT> &to () const {
		return Buffer<UNIT ,ARGC<4>>::from (mVector).self ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	const ARRAY3<UNIT> &xyz () const {
		return ARRAY3<UNIT>::from (Buffer<UNIT ,ARGC<3>>::from (_CAST_<PACK<UNIT[3] ,UNIT>> (mVector).P1)) ;
	}

	UNIT &get (INDEX x) {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		return mVector[x] ;
	}

	inline UNIT &operator[] (INDEX x) {
		return get (x) ;
	}

	const UNIT &get (INDEX x) const {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		return mVector[x] ;
	}

	inline const UNIT &operator[] (INDEX x) const {
		return get (x) ;
	}

	BOOL equal (const Vector &right) const {
		for (INDEX i = 0 ; i < 4 ; i++)
			if (mVector[i] != right.mVector[i])
				return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Vector &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Vector &right) const {
		return !equal (right) ;
	}

	BOOL less (const Vector &right) const {
		for (INDEX i = 0 ; i < 4 ; i++)
			if (mVector[i] != right.mVector[i])
				return mVector[i] < right.mVector[i] ;
		return FALSE ;
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
		_DEBUG_ASSERT_ (scale != UNIT (0)) ;
		Vector ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			ret.mVector[i] = mVector[i] / scale ;
		return std::move (ret) ;
	}

	inline Vector operator/ (const UNIT &scale) const {
		return div (scale) ;
	}

	void divto (const UNIT &scale) {
		_DEBUG_ASSERT_ (scale != UNIT (0)) ;
		for (INDEX i = 0 ; i < 4 ; i++)
			mVector[i] /= scale ;
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

	Matrix<UNIT> mul (const Vector &right) const ;

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

	UNIT dot (const Vector &right) const {
		_DEBUG_ASSERT_ (mVector[3] == UNIT (0) && right.mVector[3] == UNIT (0)) ;
		return mVector[0] * right.mVector[0] + mVector[1] * right.mVector[1] + mVector[2] * right.mVector[2] ;
	}

	inline UNIT operator* (const Vector &right) const {
		return dot (right) ;
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
		const auto r1x = mVector[3] == UNIT (0) ? magnitude () : mVector[3] ;
		const auto r2x = r1x != UNIT (0) ? UNIT (1) / r1x : UNIT (0) ;
		ret.mVector[0] = mVector[0] * r2x ;
		ret.mVector[1] = mVector[1] * r2x ;
		ret.mVector[2] = mVector[2] * r2x ;
		ret.mVector[3] = mVector[3] != UNIT (0) ? UNIT (1) : UNIT (0) ;
		return std::move (ret) ;
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

		inline auto operator[] (INDEX x) const->decltype (mBase.mMatrix[mY][x]) & {
			_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
			return mBase.mMatrix[mY][x] ;
		}

		inline const Row &operator= (const Row<Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] = right.mBase.mMatrix[right.mY][i] ;
			return *this ;
		}

		inline const Row &operator= (const Row<const Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] = right.mBase.mMatrix[right.mY][i] ;
			return *this ;
		}

		inline BOOL operator== (const Row<Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				if (mBase.mMatrix[mY][i] != right.mBase.mMatrix[right.mY][i])
					return FALSE ;
			return TRUE ;
		}

		inline BOOL operator!= (const Row<Matrix> &right) && {
			return !(*this == right) ;
		}

		inline BOOL operator== (const Row<const Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				if (mBase.mMatrix[mY][i] != right.mBase.mMatrix[right.mY][i])
					return FALSE ;
			return TRUE ;
		}

		inline BOOL operator!= (const Row<const Matrix> &right) && {
			return !(*this == right) ;
		}

		inline const Row &operator*= (const UNIT &scale) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] *= scale ;
			return *this ;
		}

		inline const Row &operator/= (const UNIT &scale) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] *= scale ;
			return *this ;
		}

		inline const Row &operator%= (const UNIT &scale) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] *= scale ;
			return *this ;
		}

		inline const Row &operator+= (const Row<Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] += right.mBase.mMatrix[right.mY][i] ;
			return *this ;
		}

		inline const Row &operator+= (const Row<const Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] += right.mBase.mMatrix[right.mY][i] ;
			return *this ;
		}

		inline const Row &operator-= (const Row<Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] += right.mBase.mMatrix[right.mY][i] ;
			return *this ;
		}

		inline const Row &operator-= (const Row<const Matrix> &right) && {
			for (INDEX i = 0 ; i < 4 ; i++)
				mBase.mMatrix[mY][i] += right.mBase.mMatrix[right.mY][i] ;
			return *this ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) :mBase (base) ,mY (y) {}
	} ;

private:
	_STATIC_ASSERT_ (std::is_same<UNIT ,VAL32>::value || std::is_same<UNIT ,VAL64>::value || std::is_same<UNIT ,VALX>::value) ;
	DEF<UNIT[4][4]> mMatrix ;

public:
	Matrix () = default ;

	implicit Matrix (const std::initializer_list<std::initializer_list<UNIT>> &right) {
		_DEBUG_ASSERT_ (LENGTH (right.size ()) == 4) ;
		const auto r1x = right.begin () ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			_DEBUG_ASSERT_ (LENGTH (r1x[i].size ()) == 4) ;
			for (INDEX j = 0 ; j < 4 ; j++)
				mMatrix[i][j] = r1x[i].begin ()[j] ;
		}
	}

	implicit Matrix (const Vector<UNIT> &hx ,const Vector<UNIT> &hy ,const Vector<UNIT> &hz ,const Vector<UNIT> &hw) {
		for (INDEX i = 0 ; i < 4 ; i++) {
			mMatrix[i][0] = hx[i] ;
			mMatrix[i][1] = hy[i] ;
			mMatrix[i][2] = hz[i] ;
			mMatrix[i][3] = hw[i] ;
		}
	}

	const ARR<UNIT> &to () const {
		return Buffer<UNIT ,ARGC<16>>::from (_CAST_<UNIT[16]> (mMatrix)).self ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	UNIT &get (INDEX y ,INDEX x) {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y][x] ;
	}

	const UNIT &get (INDEX y ,INDEX x) const {
		_DEBUG_ASSERT_ (x >= 0 && x < 4) ;
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return mMatrix[y][x] ;
	}

	Row<Matrix> get (INDEX y) {
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return Row<Matrix> (*this ,y) ;
	}

	inline Row<Matrix> operator[] (INDEX y) {
		return get (y) ;
	}

	Row<const Matrix> get (INDEX y) const {
		_DEBUG_ASSERT_ (y >= 0 && y < 4) ;
		return Row<const Matrix> (*this ,y) ;
	}

	inline Row<const Matrix> operator[] (INDEX y) const {
		return get (y) ;
	}

	BOOL equal (const Matrix &right) const {
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				if (mMatrix[i][j] != right.mMatrix[i][j])
					return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Matrix &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Matrix &right) const {
		return !equal (right) ;
	}

	Matrix mul (const UNIT &scale) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[i][j] = mMatrix[i][j] * scale ;
		return std::move (ret) ;
	}

	inline Matrix operator* (const UNIT &scale) const {
		return mul (scale) ;
	}

	void multo (const UNIT &scale) {
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				mMatrix[i][j] *= scale ;
	}

	inline Matrix &operator*= (const UNIT &scale) {
		multo (scale) ;
		return *this ;
	}

	Matrix div (const UNIT &scale) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[i][j] = mMatrix[i][j] / scale ;
		return std::move (ret) ;
	}

	inline Matrix operator/ (const UNIT &scale) const {
		return div (scale) ;
	}

	void divto (const UNIT &scale) {
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				mMatrix[i][j] /= scale ;
	}

	inline Matrix &operator/= (const UNIT &scale) {
		divto (scale) ;
		return *this ;
	}

	Matrix add (const Matrix &right) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[i][j] = mMatrix[i][j] + right.mMatrix[i][j] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ (const Matrix &right) const {
		return add (right) ;
	}

	void addto (const Matrix &right) {
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				mMatrix[i][j] += right.mMatrix[i][j] ;
	}

	inline Matrix &operator+= (const Matrix &right) {
		addto (right) ;
		return *this ;
	}

	Matrix sub (const Matrix &right) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[i][j] = mMatrix[i][j] - right.mMatrix[i][j] ;
		return std::move (ret) ;
	}

	inline Matrix operator- (const Matrix &right) const {
		return sub (right) ;
	}

	void subto (const Matrix &right) {
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				mMatrix[i][j] -= right.mMatrix[i][j] ;
	}

	inline Matrix &operator-= (const Matrix &right) {
		subto (right) ;
		return *this ;
	}

	Matrix plus () const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[i][j] = +mMatrix[i][j] ;
		return std::move (ret) ;
	}

	inline Matrix operator+ () const {
		return plus () ;
	}

	Matrix minus () const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[i][j] = -mMatrix[i][j] ;
		return std::move (ret) ;
	}

	inline Matrix operator- () const {
		return minus () ;
	}

	Matrix mul (const Matrix &right) const {
		Matrix ret ;
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++) {
				ret.mMatrix[i][j] = UNIT (0) ;
				for (INDEX k = 0 ; k < 4 ; k++)
					ret.mMatrix[i][j] += mMatrix[i][k] * right.mMatrix[k][j] ;
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
		for (INDEX i = 0 ; i < 4 ; i++)
			for (INDEX j = 0 ; j < 4 ; j++)
				ret.mMatrix[j][i] = mMatrix[i][j] ;
		return std::move (ret) ;
	}

	Matrix triangular () const {
		Matrix ret = *this ;
		for (INDEX i = 0 ; i < 4 ; i++) {
			INDEX ix = ret.max_row (i) ;
			if (ix != i) {
				for (INDEX j = i ; j < 4 ; j++) {
					const auto r1x = -ret.mMatrix[i][j] ;
					ret.mMatrix[i][j] = ret.mMatrix[ix][j] ;
					ret.mMatrix[ix][j] = r1x ;
				}
			}
			if (ret.mMatrix[i][i] == 0)
				continue ;
			for (INDEX j = i + 1 ; j < 4 ; j++) {
				const auto r2x = ret.mMatrix[j][i] / ret.mMatrix[i][i] ;
				for (INDEX k = i + 1 ; k < 4 ; k++)
					ret.mMatrix[j][k] -= r2x * ret.mMatrix[i][k] ;
				ret.mMatrix[j][i] = UNIT (0) ;
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
			ret *= r1x.mMatrix[i][i] ;
		return std::move (ret) ;
	}

	Matrix inverse () const {
		Matrix ret ;
		const auto r5x = det () ;
		_DYNAMIC_ASSERT_ (r5x != UNIT (0)) ;
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
				const auto r1x = mMatrix[ix][jx] * (mMatrix[iy][jy] * mMatrix[iz][jz] - mMatrix[iz][jy] * mMatrix[iy][jz]) ;
				const auto r2x = mMatrix[iy][jx] * (mMatrix[ix][jy] * mMatrix[iz][jz] - mMatrix[iz][jy] * mMatrix[ix][jz]) ;
				const auto r3x = mMatrix[iz][jx] * (mMatrix[ix][jy] * mMatrix[iy][jz] - mMatrix[iy][jy] * mMatrix[ix][jz]) ;
				const auto r4x = r1x - r2x + r3x ;
				ret.mMatrix[j][i] = (i + j) % 2 != 0 ? -r4x : r4x ;
			}
		}
		ret /= r5x ;
		return std::move (ret) ;
	}

	//@info:4-projection * 3-translation * 2-rotation * 1-shear * 0-scale
	ARRAY5<Matrix> decompose () const {
		_DEBUG_ASSERT_ (mMatrix[3][0] == UNIT (0) && mMatrix[3][1] == UNIT (0) && mMatrix[3][2] == UNIT (0) && mMatrix[3][3] != UNIT (0)) ;
		ARRAY5<Matrix> ret ;
		const auto r1x = mul (Vector<UNIT> {UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)}) ;
		const auto r2x = mul (Vector<UNIT> {UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)}) ;
		const auto r3x = mul (Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) ;
		ret[0] = Matrix ({
			{r1x.magnitude () ,UNIT (0) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,r2x.magnitude () ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,r3x.magnitude () ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
		ret[2] = Matrix {r1x.normalize () ,r2x.normalize () ,r3x.normalize () ,Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}} ;
		const auto r4x = (ret[2] * Vector<UNIT> {UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)}) * (ret[2] * Vector<UNIT> {UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)}) ;
		const auto r5x = _SQRT_ (UNIT (1) - _SQE_ (r4x)) ;
		const auto r6x = (ret[2] * Vector<UNIT> {UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)}) * (ret[2] * Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) ;
		const auto r7x = _SQRT_ (UNIT (1) - _SQE_ (r6x)) * _SIGN_ (((ret[2] * Vector<UNIT> {UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)}) ^ (ret[2] * Vector<UNIT> {UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)})) * (ret[2] * Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)})) ;
		ret[1] = Matrix ({
			{UNIT (1) ,r4x ,r6x ,UNIT (0)} ,
			{UNIT (0) ,r5x ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,r7x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
		ret[2] *= ret[1].inverse () ;
		ret[3] = Matrix ({
			{UNIT (1) ,UNIT (0) ,UNIT (0) ,mMatrix[0][3]} ,
			{UNIT (0) ,UNIT (1) ,UNIT (0) ,mMatrix[1][3]} ,
			{UNIT (0) ,UNIT (0) ,UNIT (1) ,mMatrix[2][3]} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
		ret[4] = Matrix ({
			{UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,mMatrix[3][3]}}) ;
		return std::move (ret) ;
	}

	Matrix eigen () const {
		Matrix ret ;
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return std::move (ret) ;
	}

private:
	INDEX max_row (INDEX yx) const {
		INDEX ret = VAR_NONE ;
		auto rax = UNIT () ;
		for (INDEX i = yx ; i < 4 ; i++) {
			const auto r1x = _ABS_ (mMatrix[i][yx]) ;
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

	static Matrix make_scale (const UNIT &scale_x ,const UNIT &scale_y ,const UNIT &scale_z) {
		_DEBUG_ASSERT_ (scale_x > UNIT (0) && scale_y > UNIT (0) && scale_z > UNIT (0)) ;
		return Matrix ({
			{scale_x ,UNIT (0) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,scale_y ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,scale_z ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_shear (const UNIT &angle_xy ,const UNIT &angle_xz) {
		_DEBUG_ASSERT_ (angle_xy != UNIT (0) && angle_xz != UNIT (0)) ;
		return Matrix ({
			{UNIT (1) ,_COS_ (angle_xy) ,_COS_ (angle_xz) ,UNIT (0)} ,
			{UNIT (0) ,_SIN_ (angle_xy) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,_SIN_ (angle_xz) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_rotation (const Vector<UNIT> &normal ,const UNIT &angle) {
		_DEBUG_ASSERT_ (normal[3] == UNIT (0)) ;
		const auto r1x = normal.normalize () ;
		_DEBUG_ASSERT_ (r1x[0] != UNIT (0) || r1x[1] != UNIT (0) || r1x[2] != UNIT (0)) ;
		const auto r2x = _COS_ (angle) ;
		const auto r3x = r1x * _SIN_ (angle) ;
		const auto r4x = r1x * (UNIT (1) - r2x) ;
		return Matrix ({
			{r1x[0] * r4x[0] + r2x ,r1x[0] * r4x[1] - r3x[2] ,r1x[0] * r4x[2] + r3x[1] ,UNIT (0)} ,
			{r1x[1] * r4x[0] + r3x[2] ,r1x[1] * r4x[1] + r2x ,r1x[1] * r4x[2] - r3x[0] ,UNIT (0)} ,
			{r1x[2] * r4x[0] - r3x[1] ,r1x[2] * r4x[1] + r3x[0] ,r1x[2] * r4x[2] + r2x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static Matrix make_rotation (const UNIT &qx ,const UNIT &qy ,const UNIT &qz ,const UNIT &qw) {
		const auto r1x = _SQE_ (qx) + _SQE_ (qy) + _SQE_ (qz) + _SQE_ (qw) ;
		_DEBUG_ASSERT_ (r1x != UNIT (0)) ;
		const auto r2x = r1x != UNIT (0) ? UNIT (2) / r1x : UNIT (0) ;
		const auto r3x = qx * r2x ;
		const auto r4x = qy * r2x ;
		const auto r5x = qz * r2x ;
		const auto r6x = qw * r2x ;
		return Matrix ({
			{UNIT (1) - qy * r4x - qz * r5x ,qx * r4x - qz * r6x ,qx * r5x + qy * r6x ,UNIT (0)} ,
			{qx * r4x + qz * r6x ,UNIT (1) - qx * r3x - qz * r5x ,qy * r5x - qx * r6x ,UNIT (0)} ,
			{qx * r5x - qy * r6x ,qy * r5x + qx * r6x ,UNIT (1) - qx * r3x - qy * r4x ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)}}) ;
	}

	static ARRAY4<UNIT> make_rotation_quat (const Matrix &rotation) {
		ARRAY4<UNIT> ret ;
		const auto r1x = rotation.decompose ()[2] ;
		_DEBUG_ASSERT_ (UNIT (1) + r1x[0][0] + r1x[1][1] + r1x[2][2] >= 0) ;
		const auto r2x = UNIT (2) * _SQRT_ (UNIT (1) + r1x[0][0] + r1x[1][1] + r1x[2][2]) ;
		ret[0] = (r1x[2][1] - r1x[1][2]) / r2x ;
		ret[1] = (r1x[0][2] - r1x[2][0]) / r2x ;
		ret[2] = (r1x[1][0] - r1x[0][1]) / r2x ;
		ret[3] = r2x / UNIT (4) ;
		return std::move (ret) ;
	}

	static Matrix make_translation (const Vector<UNIT> &direction) {
		const auto r1x = direction[3] != UNIT (0) ? Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)} -direction.normalize () : direction ;
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
		const auto r2x = ARRAY3<UNIT> {_ABS_ (normal[0]) ,_ABS_ (normal[1]) ,_ABS_ (normal[2])} ;
		const auto r3x = r2x[0] < r2x[1] ? (r2x[0] < r2x[2] ? Vector<UNIT> {UNIT (1) ,UNIT (0) ,UNIT (0) ,UNIT (0)} : Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) : (r2x[1] < r2x[2] ? Vector<UNIT> {UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)} : Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}) ;
		const auto r4x = (r1x ^ r3x).normalize () ;
		return Matrix {r4x ,(r1x ^ r4x).normalize () ,r1x ,center} ;
	}

	static Matrix make_perspective (const Vector<UNIT> &normal ,const Vector<UNIT> &center ,const UNIT &far) {
		_DEBUG_ASSERT_ (far != UNIT (0)) ;
		return Matrix ({
			{far ,UNIT (0) ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,far ,UNIT (0) ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,far ,UNIT (0)} ,
			{UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (0)}}) ;
	}

	static Matrix make_projection (const Vector<UNIT> &normal ,const Vector<UNIT> &center ,const Vector<UNIT> &light) {
		_DEBUG_ASSERT_ (normal[3] == UNIT (0) && center[3] == UNIT (1)) ;
		const auto r1x = normal.normalize () ;
		_DEBUG_ASSERT_ (r1x[0] != UNIT (0) || r1x[1] != UNIT (0) || r1x[2] != UNIT (0)) ;
		const auto r2x = light.normalize () ;
		const auto r3x = Vector<UNIT> {center[0] ,center[1] ,center[2] ,UNIT (0)} *r1x ;
		const auto r4x = Vector<UNIT> {r2x[0] ,r2x[1] ,r2x[2] ,UNIT (0)} *r1x ;
		const auto r5x = r2x[3] != UNIT (0) ? r3x : UNIT (0) ;
		const auto r6x = r2x[3] != UNIT (0) ? r1x : Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (0)} ;
		return Matrix ({
			{r1x[0] * r2x[0] - r4x + r5x ,r1x[1] * r2x[0] ,r1x[2] * r2x[0] ,-r3x * r2x[0]} ,
			{r1x[0] * r2x[1] ,r1x[1] * r2x[1] - r4x + r5x ,r1x[2] * r2x[1] ,-r3x * r2x[1]} ,
			{r1x[0] * r2x[2] ,r1x[1] * r2x[2] ,r1x[2] * r2x[2] - r4x + r5x ,-r3x * r2x[2]} ,
			{r6x[0] ,r6x[1] ,r6x[2] ,-r4x}}) ;
	}
} ;

template <class UNIT>
inline Matrix<UNIT> Vector<UNIT>::mul (const Vector &right) const {
	Matrix<UNIT> ret ;
	for (INDEX i = 0 ; i < 4 ; i++)
		for (INDEX j = 0 ; j < 4 ; j++)
			ret.get (i ,j) = get (i) * right.get (j) ;
	return std::move (ret) ;
}

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

template <class UNIT>
class Camera {
private:
	Vector<UNIT> mEyeU ;
	Vector<UNIT> mEyeV ;
	Vector<UNIT> mEyeN ;
	Vector<UNIT> mEyeP ;
#ifdef __CSC_EXT__
	Mutable<Matrix<UNIT>> mViewMatrix ;
#else
	Matrix<UNIT> mViewMatrix ;
#endif
	UNIT mScreenW ;
	UNIT mScreenH ;
	UNIT mScreenD ;
	Matrix<UNIT> mProjectionMatrix ;

public:
	Camera () {
		lookat ({UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (1)} ,{UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)} ,{UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)}) ;
		perspective (UNIT (90) ,UNIT (1) ,UNIT (1) ,UNIT (1000)) ;
	}

	const Vector<UNIT> &eye_u () const {
		return mEyeU ;
	}

	const Vector<UNIT> &eye_v () const {
		return mEyeV ;
	}

	const Vector<UNIT> &eye_n () const {
		return mEyeN ;
	}

	const Vector<UNIT> &eye_p () const {
		return mEyeP ;
	}

	void lookat (const Vector<UNIT> &eye ,const Vector<UNIT> &center ,const Vector<UNIT> &up) {
		_DEBUG_ASSERT_ (eye[3] == UNIT (1) && center[3] == UNIT (1) && up[3] == UNIT (0)) ;
		mEyeN = (center - eye).normalize () ;
		mEyeU = (mEyeN ^ up).normalize () ;
		mEyeV = (mEyeU ^ mEyeN).normalize () ;
		mEyeP = eye ;
#ifdef __CSC_EXT__
		mViewMatrix.signal () ;
#else
		update_view_matrix (mViewMatrix) ;
#endif
	}

	void move (const UNIT &distance_u ,const UNIT &distance_v ,const UNIT &distance_n) {
		mEyeP += mEyeU * distance_u + mEyeV * distance_v + mEyeN * distance_n ;
#ifdef __CSC_EXT__
		mViewMatrix.signal () ;
#else
		update_view_matrix (mViewMatrix) ;
#endif
	}

	//@info:angle_nv-angle_un-angle_vu <<==>> pitch-yaw-roll (anti-clockwise)
	void rotate (const UNIT &angle_nv ,const UNIT &angle_un ,const UNIT &angle_vu) {
		if (angle_nv != UNIT (0)) {
			const auto r1x = mEyeN * _COS_ (angle_nv) + mEyeV * _SIN_ (angle_nv) ;
			const auto r2x = mEyeV * _COS_ (angle_nv) - mEyeN * _SIN_ (angle_nv) ;
			mEyeN = r1x ;
			mEyeV = r2x ;
		}
		if (angle_un != UNIT (0)) {
			const auto r1x = mEyeU * _COS_ (angle_un) + mEyeN * _SIN_ (angle_un) ;
			const auto r2x = mEyeN * _COS_ (angle_un) - mEyeU * _SIN_ (angle_un) ;
			mEyeU = r1x ;
			mEyeN = r2x ;
		}
		if (angle_vu != UNIT (0)) {
			const auto r1x = mEyeV * _COS_ (angle_vu) + mEyeU * _SIN_ (angle_vu) ;
			const auto r2x = mEyeU * _COS_ (angle_vu) - mEyeV * _SIN_ (angle_vu) ;
			mEyeV = r1x ;
			mEyeU = r2x ;
		}
#ifdef __CSC_EXT__
		mViewMatrix.signal () ;
#else
		update_view_matrix (mViewMatrix) ;
#endif
	}

	void circle (const UNIT &near ,const UNIT &angle_un ,const UNIT &angle_nv) {
		mEyeP += mEyeN * near ;
		rotate (angle_nv ,angle_un ,0) ;
		mEyeP -= mEyeN * near ;
	}

	const Matrix<UNIT> &view () const {
#ifdef __CSC_EXT__
		mViewMatrix.apply (_DEDUCE_<Function> (PhanRef<const Camera>::make (this) ,&Camera::update_view_matrix)) ;
#endif
		return mViewMatrix ;
	}

	UNIT screen_w () const {
		return mScreenW ;
	}

	UNIT screen_h () const {
		return mScreenH ;
	}

	UNIT screen_d () const {
		return mScreenD ;
	}

	void perspective (const UNIT &fov ,const UNIT &aspect ,const UNIT &near ,const UNIT &far) {
		_DEBUG_ASSERT_ (fov > 0 && fov < 180) ;
		_DEBUG_ASSERT_ (aspect != 0) ;
		const auto r1x = near * _TAN_ (fov * UNIT (VALX_PI / 360)) ;
		const auto r2x = r1x * aspect ;
		frustum (-r2x ,r2x ,-r1x ,r1x ,near ,far) ;
	}

	void frustum (const UNIT &left ,const UNIT &right ,const UNIT &bottom ,const UNIT &top ,const UNIT &near ,const UNIT &far) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (far > near && near > 0) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = far - near ;
		mProjectionMatrix[0][0] = UNIT (2) * near / mScreenW ;
		mProjectionMatrix[0][1] = UNIT (0) ;
		mProjectionMatrix[0][2] = (right + left) / mScreenW ;
		mProjectionMatrix[0][3] = UNIT (0) ;
		mProjectionMatrix[1][0] = UNIT (0) ;
		mProjectionMatrix[1][1] = UNIT (2) * near / mScreenH ;
		mProjectionMatrix[1][2] = (top + bottom) / mScreenH ;
		mProjectionMatrix[1][3] = UNIT (0) ;
		mProjectionMatrix[2][0] = UNIT (0) ;
		mProjectionMatrix[2][1] = UNIT (0) ;
		mProjectionMatrix[2][2] = -(far + near) / mScreenD ;
		mProjectionMatrix[2][3] = -(UNIT (2) * near * far) / mScreenD ;
		mProjectionMatrix[3][0] = UNIT (0) ;
		mProjectionMatrix[3][1] = UNIT (0) ;
		mProjectionMatrix[3][2] = UNIT (-1) ;
		mProjectionMatrix[3][3] = UNIT (0) ;
	}

	void ortho (const UNIT &left ,const UNIT &right ,const UNIT &bottom ,const UNIT &top ,const UNIT &near ,const UNIT &far) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (far > near) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = far - near ;
		mProjectionMatrix[0][0] = UNIT (2) / mScreenW ;
		mProjectionMatrix[0][1] = UNIT (0) ;
		mProjectionMatrix[0][2] = UNIT (0) ;
		mProjectionMatrix[0][3] = UNIT (0) ;
		mProjectionMatrix[1][0] = UNIT (0) ;
		mProjectionMatrix[1][1] = UNIT (2) / mScreenH ;
		mProjectionMatrix[1][2] = UNIT (0) ;
		mProjectionMatrix[1][3] = UNIT (0) ;
		mProjectionMatrix[2][0] = UNIT (0) ;
		mProjectionMatrix[2][1] = UNIT (0) ;
		mProjectionMatrix[2][2] = UNIT (-2) / mScreenD ;
		mProjectionMatrix[2][3] = UNIT (0) ;
		mProjectionMatrix[3][0] = -(right + left) / mScreenW ;
		mProjectionMatrix[3][1] = -(top + bottom) / mScreenH ;
		mProjectionMatrix[3][2] = -(far + near) / mScreenD ;
		mProjectionMatrix[3][3] = UNIT (1) ;
	}

	const Matrix<UNIT> &projection () const {
		return mProjectionMatrix ;
	}

private:
	void update_view_matrix (Matrix<UNIT> &view) const {
		const auto r1x = Vector<UNIT> {-mEyeP[0] ,-mEyeP[1] ,-mEyeP[2] ,UNIT (0)} ;
		view[0][0] = mEyeU[0] ;
		view[0][1] = mEyeU[1] ;
		view[0][2] = mEyeU[2] ;
		view[0][3] = mEyeU * r1x ;
		view[1][0] = mEyeV[0] ;
		view[1][1] = mEyeV[1] ;
		view[1][2] = mEyeV[2] ;
		view[1][3] = mEyeV * r1x ;
		view[2][0] = -mEyeN[0] ;
		view[2][1] = -mEyeN[1] ;
		view[2][2] = -mEyeN[2] ;
		view[2][3] = -mEyeN * r1x + UNIT (1) ;
		view[3][0] = UNIT (0) ;
		view[3][1] = UNIT (0) ;
		view[3][2] = UNIT (0) ;
		view[3][3] = UNIT (1) ;
	}
} ;

class Mesh {
public:
	Mesh () = default ;
} ;

class AbstractShader {
public:
	class Engine :public Interface {
	public:
		virtual void attach_program (AnyRef<void> &shader ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const = 0 ;
		virtual void active_program (const AnyRef<void> &shader) const = 0 ;
		virtual FLAG uniform_find (const AnyRef<void> &shader ,const String<STRU8> &key) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAR32 &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAR64 &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAL32 &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAL64 &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Vector<VAL32> &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Vector<VAL64> &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Matrix<VAL32> &val) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Matrix<VAL64> &val) const = 0 ;
	} ;

private:
	PhanRef<const Engine> mEngine ;
	AnyRef<void> mHolder ;
	Set<String<STRU8> ,FLAG> mUniformSet ;

public:
	AbstractShader () = default ;

	explicit AbstractShader (const Engine &engine) {
		mEngine = PhanRef<const Engine>::make (&engine) ;
	}

	BOOL exist () const {
		return mEngine.exist () && mHolder.exist () ;
	}

	void attach (const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) {
		_DEBUG_ASSERT_ (exist ()) ;
		mEngine->attach_program (mHolder ,vs ,fs) ;
	}

	void active () {
		_DEBUG_ASSERT_ (exist ()) ;
		mEngine->active_program (mHolder) ;
	}

	void uniform (const String<STRU8> &key ,const VAR32 &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const VAR64 &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const VAL32 &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const VAL64 &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const Vector<VAL32> &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const Vector<VAL64> &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const Matrix<VAL32> &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}

	void uniform (const String<STRU8> &key ,const Matrix<VAL64> &val) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformSet.find (key) ;
		if (ix == VAR_NONE) {
			ix = mUniformSet.insert (key) ;
			mUniformSet[ix].item = mEngine->uniform_find (mHolder ,key) ;
		}
		mEngine->uniform_write (mHolder ,mUniformSet[ix].item ,val) ;
	}
} ;

class AbstractShaderSprite {
public:
	class Engine :public Interface {
	public:
		virtual void attach_mesh (AnyRef<void> &sprite ,const Mesh &mesh) const = 0 ;
		virtual void draw (const AnyRef<void> &sprite) const = 0 ;
	} ;

private:
	PhanRef<const Engine> mEngine ;
	AnyRef<void> mHolder ;

public:
	AbstractShaderSprite () = default ;

	explicit AbstractShaderSprite (const Engine &engine) {
		mEngine = PhanRef<const Engine>::make (&engine) ;
	}

	BOOL exist () const {
		return mEngine.exist () && mHolder.exist () ;
	}

	void attach (const Mesh &mesh) {
		_DEBUG_ASSERT_ (exist ()) ;
		mEngine->attach_mesh (mHolder ,mesh) ;
	}

	void draw () {
		_DEBUG_ASSERT_ (exist ()) ;
		mEngine->draw (mHolder) ;
	}
} ;
} ;