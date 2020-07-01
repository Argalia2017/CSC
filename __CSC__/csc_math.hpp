#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
class MathProc
	:private Wrapped<void> {
public:
	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> square (const _ARG1 &val) ;

	imports VAL32 sqrt (const VAL32 &x) ;

	imports VAL64 sqrt (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> sqrt (const _ARG1 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> cube (const _ARG1 &val) ;

	imports VAL32 cbrt (const VAL32 &x) ;

	imports VAL64 cbrt (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> cbrt (const _ARG1 &x) ;

	imports VAL32 pow (const VAL32 &x ,const VAL32 &y) ;

	imports VAL64 pow (const VAL64 &x ,const VAL64 &y) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> pow (const _ARG1 &x ,const _ARG1 &y) ;

	imports VAL32 exp (const VAL32 &x) ;

	imports VAL64 exp (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> exp (const _ARG1 &x) ;

	imports VAL32 log (const VAL32 &x) ;

	imports VAL64 log (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> log (const _ARG1 &x) ;

	imports VAL32 sin (const VAL32 &x) ;

	imports VAL64 sin (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> sin (const _ARG1 &x) ;

	imports VAL32 cos (const VAL32 &x) ;

	imports VAL64 cos (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> cos (const _ARG1 &x) ;

	imports VAL32 tan (const VAL32 &x) ;

	imports VAL64 tan (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> tan (const _ARG1 &x) ;

	imports VAL32 arcsin (const VAL32 &y) ;

	imports VAL64 arcsin (const VAL64 &y) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> arcsin (const _ARG1 &y) ;

	imports VAL32 arccos (const VAL32 &x) ;

	imports VAL64 arccos (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> arccos (const _ARG1 &x) ;

	imports VAL32 arctan (const VAL32 &x) ;

	imports VAL64 arctan (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> arctan (const _ARG1 &x) ;

	imports VAL32 arctan (const VAL32 &y ,const VAL32 &x) ;

	imports VAL64 arctan (const VAL64 &y ,const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> arctan (const _ARG1 &y ,const _ARG1 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> sign (const _ARG1 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> inverse (const _ARG1 &x ,const _ARG1 &y) ;

	imports VAL32 inverse (const VAL32 &x) ;

	imports VAL64 inverse (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> npdf (const _ARG1 &x) ;

	imports VAL32 ncdf (const VAL32 &x) ;

	imports VAL64 ncdf (const VAL64 &x) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> ncdf (const _ARG1 &x) ;

	imports VAL32 floor (const VAL32 &x ,const VAL32 &y) ;

	imports VAL64 floor (const VAL64 &x ,const VAL64 &y) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> floor (const _ARG1 &x ,const _ARG1 &y) ;

	imports VAL32 ceil (const VAL32 &x ,const VAL32 &y) ;

	imports VAL64 ceil (const VAL64 &x ,const VAL64 &y) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> ceil (const _ARG1 &x ,const _ARG1 &y) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> round (const _ARG1 &x ,const _ARG1 &y) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> trunc (const _ARG1 &x ,const _ARG1 &y) ;

	template <class _ARG1 ,class _ARG2>
	imports REMOVE_CVR_TYPE<_ARG2> clamp (const _ARG1 &val ,const _ARG2 &min_ ,const _ARG2 &max_) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> abs (const _ARG1 &val) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> minof (const _ARG1 &list_one) ;

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	imports REMOVE_CVR_TYPE<_ARG1> minof (const _ARG1 &list_one ,const _ARG2 &list_two ,const _ARGS &...list_rest) ;

	template <class _ARG1>
	imports REMOVE_CVR_TYPE<_ARG1> maxof (const _ARG1 &list_one) ;

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	imports REMOVE_CVR_TYPE<_ARG1> maxof (const _ARG1 &list_one ,const _ARG2 &list_two ,const _ARGS &...list_rest) ;

	imports BOOL is_nan (const VAL32 &x) ;

	imports BOOL is_nan (const VAL64 &x) ;

	imports BOOL is_infinite (const VAL32 &x) ;

	imports BOOL is_infinite (const VAL64 &x) ;

	imports VAL64 ieee754_encode (const ARRAY3<VAR64> &sne2) ;

	imports ARRAY3<VAR64> ieee754_decode (const VAL64 &ieee754) ;

	imports ARRAY3<VAR64> ieee754_e2_e10 (const ARRAY3<VAR64> &sne2) ;

	imports ARRAY3<VAR64> ieee754_e10_e2 (const ARRAY3<VAR64> &sne10) ;
} ;

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::square (const _ARG1 &val) {
	return val * val ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::sqrt (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::sqrt (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::cube (const _ARG1 &val) {
	return val * val * val ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::cbrt (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::cbrt (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::pow (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::pow (VAL64 (x) ,VAL64 (y))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::exp (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::exp (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::log (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::log (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::sin (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::sin (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::cos (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::cos (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::tan (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::tan (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::arcsin (const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::arcsin (VAL64 (y))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::arccos (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::arccos (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::arctan (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::arctan (VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::arctan (const _ARG1 &y ,const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::arctan (VAL64 (y) ,VAL64 (x))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::sign (const _ARG1 &x) {
	if (x < _ARG1 (0))
		return _ARG1 (-1) ;
	if (x > _ARG1 (0))
		return _ARG1 (+1) ;
	return _ARG1 (0) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::inverse (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	_DEBUG_ASSERT_ (y > _ARG1 (0)) ;
	if (MathProc::abs (x) < y)
		return _ARG1 (0) ;
	return _ARG1 (1) / x ;
}

inline exports VAL32 MathProc::inverse (const VAL32 &x) {
	return MathProc::inverse (x ,VAL32_EPS) ;
}

inline exports VAL64 MathProc::inverse (const VAL64 &x) {
	return MathProc::inverse (x ,VAL64_EPS) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::npdf (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	const auto r1x = MathProc::inverse (MathProc::sqrt (VAL64 (2 * MATH_PI))) ;
	const auto r2x = -MathProc::square (VAL64 (x)) / 2 ;
	return MathProc::exp (r2x) * r1x ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::ncdf (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::ncdf (VAL64 (x))) ;
}

inline exports VAL32 MathProc::floor (const VAL32 &x ,const VAL32 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * MathProc::inverse (y)) ;
	const auto r2x = y * VAL32 (r1x) ;
	if (x >= 0)
		return r2x ;
	if (x >= r2x)
		return r2x ;
	return y * VAL32 (r1x - 1) ;
}

inline exports VAL64 MathProc::floor (const VAL64 &x ,const VAL64 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * MathProc::inverse (y)) ;
	const auto r2x = y * VAL64 (r1x) ;
	if (x >= 0)
		return r2x ;
	if (x >= r2x)
		return r2x ;
	return y * VAL64 (r1x - 1) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::floor (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::floor (VAL64 (x) ,VAL64 (y))) ;
}

inline exports VAL32 MathProc::ceil (const VAL32 &x ,const VAL32 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * MathProc::inverse (y)) ;
	const auto r2x = y * VAL32 (r1x) ;
	if (x <= 0)
		return r2x ;
	if (x <= r2x)
		return r2x ;
	return y * VAL32 (r1x + 1) ;
}

inline exports VAL64 MathProc::ceil (const VAL64 &x ,const VAL64 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * MathProc::inverse (y)) ;
	const auto r2x = y * VAL64 (r1x) ;
	if (x <= 0)
		return r2x ;
	if (x <= r2x)
		return r2x ;
	return y * VAL64 (r1x + 1) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::ceil (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (MathProc::ceil (VAL64 (x) ,VAL64 (y))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::round (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	_DEBUG_ASSERT_ (y > _ARG1 (0)) ;
	const auto r1x = VAL64 (x) + VAL64 (y) / 2 ;
	return _ARG1 (MathProc::floor (r1x ,VAL64 (y))) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::trunc (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return MathProc::floor (MathProc::abs (x) ,y) * MathProc::sign (x) ;
}

template <class _ARG1 ,class _ARG2>
inline exports REMOVE_CVR_TYPE<_ARG2> MathProc::clamp (const _ARG1 &val ,const _ARG2 &min_ ,const _ARG2 &max_) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	if (val < _ARG1 (min_))
		return min_ ;
	if (val > _ARG1 (max_))
		return max_ ;
	return _ARG2 (val) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::abs (const _ARG1 &val) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	_ARG1 ret = _ABS_ (val) ;
	_DEBUG_ASSERT_ (ret >= _ARG1 (0)) ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::minof (const _ARG1 &list_one) {
	return list_one ;
}

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::minof (const _ARG1 &list_one ,const _ARG2 &list_two ,const _ARGS &...list_rest) {
	_STATIC_ASSERT_ (stl::is_same<_ARG1 ,_ARG2>::value) ;
	auto &r1x = _MIN_ (list_one ,list_two) ;
	return MathProc::minof (r1x ,list_rest...) ;
}

template <class _ARG1>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::maxof (const _ARG1 &list_one) {
	return list_one ;
}

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
inline exports REMOVE_CVR_TYPE<_ARG1> MathProc::maxof (const _ARG1 &list_one ,const _ARG2 &list_two ,const _ARGS &...list_rest) {
	_STATIC_ASSERT_ (stl::is_same<_ARG1 ,_ARG2>::value) ;
	auto &r1x = _MAX_ (list_one ,list_two) ;
	return MathProc::maxof (r1x ,list_rest...) ;
}

class MathStaticProc
	:private Wrapped<void> {
public:
	imports ARRAY3<DATA> static_ieee754_encode_part (const ARRAY3<VAR64> &sne2) ;

	imports VAL64 static_taylor_exp (const VAL64 &lnx ,const VAL64 &y) ;

	imports ARRAY3<VAR64> static_ieee754_e2_e10_part (const ARRAY3<VAR64> &sne2) ;

	imports ARRAY3<VAR64> static_ieee754_e10_e2_part (const ARRAY3<VAR64> &sne10) ;
} ;

inline exports ARRAY3<DATA> MathStaticProc::static_ieee754_encode_part (const ARRAY3<VAR64> &sne2) {
	const auto r1x = _CAST_ (ARGV<ARRAY3<DATA>>::null ,sne2) ;
	ARRAY3<DATA> ret = r1x ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~DATA (0X001FFFFFFFFFFFFF)) == 0)
			break ;
		ret[0] = ret[0] >> 1 ;
		ret[1]++ ;
	}
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~DATA (0X000FFFFFFFFFFFFF)) != 0)
			break ;
		ret[0] = ret[0] << 1 ;
		ret[1]-- ;
	}
	if switch_once (TRUE) {
		const auto r2x = VAR64 (DATA (-1074) - ret[1]) ;
		if (r2x <= 0)
			discard ;
		ret[0] = ret[0] >> r2x ;
		ret[1] = DATA (-1075) ;
	}
	ret[1] += 1075 ;
	ret[1] = ret[1] << 52 ;
	if (ret[0] == 0)
		ret[1] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports BOOL MathProc::is_nan (const VAL32 &x) {
	const auto r1x = _CAST_ (ARGV<CHAR>::null ,x) ;
	if ((r1x & CHAR (0X7F800000)) != CHAR (0X7F800000))
		return FALSE ;
	if ((r1x & CHAR (0X007FFFFF)) == 0)
		return FALSE ;
	return TRUE ;
}

inline exports BOOL MathProc::is_nan (const VAL64 &x) {
	const auto r1x = _CAST_ (ARGV<DATA>::null ,x) ;
	if ((r1x & DATA (0X7FF0000000000000)) != DATA (0X7FF0000000000000))
		return FALSE ;
	if ((r1x & DATA (0X000FFFFFFFFFFFFF)) == 0)
		return FALSE ;
	return TRUE ;
}

inline exports BOOL MathProc::is_infinite (const VAL32 &x) {
	const auto r1x = _CAST_ (ARGV<CHAR>::null ,x) ;
	if ((r1x & CHAR (0X7F800000)) != CHAR (0X7F800000))
		return FALSE ;
	if ((r1x & CHAR (0X007FFFFF)) != 0)
		return FALSE ;
	return TRUE ;
}

inline exports BOOL MathProc::is_infinite (const VAL64 &x) {
	const auto r1x = _CAST_ (ARGV<DATA>::null ,x) ;
	if ((r1x & DATA (0X7FF0000000000000)) != DATA (0X7FF0000000000000))
		return FALSE ;
	if ((r1x & DATA (0X000FFFFFFFFFFFFF)) != 0)
		return FALSE ;
	return TRUE ;
}

inline exports VAL64 MathProc::ieee754_encode (const ARRAY3<VAR64> &sne2) {
	const auto r1x = MathStaticProc::static_ieee754_encode_part (sne2) ;
	_DYNAMIC_ASSERT_ ((r1x[0] & ~DATA (0X001FFFFFFFFFFFFF)) == 0) ;
	_DYNAMIC_ASSERT_ ((r1x[1] & ~DATA (0X7FF0000000000000)) == 0) ;
	DATA ret = 0 ;
	ret |= r1x[0] & DATA (0X000FFFFFFFFFFFFF) ;
	ret |= r1x[1] & DATA (0X7FF0000000000000) ;
	ret |= r1x[2] & DATA (0X8000000000000000) ;
	return _MOVE_ (_CAST_ (ARGV<VAL64>::null ,ret)) ;
}

inline exports ARRAY3<VAR64> MathProc::ieee754_decode (const VAL64 &ieee754) {
	ARRAY3<DATA> ret ;
	const auto r1x = _CAST_ (ARGV<DATA>::null ,ieee754) ;
	ret[0] = r1x & DATA (0X000FFFFFFFFFFFFF) ;
	const auto r2x = DATA (r1x & DATA (0X7FF0000000000000)) ;
	if (r2x != 0)
		ret[0] |= DATA (0X0010000000000000) ;
	ret[1] = r2x >> 52 ;
	ret[1] -= DATA (1075 - _EBOOL_ (r2x == 0)) ;
	if (ret[0] == 0)
		ret[1] = 0 ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & DATA (0X0000000000000001)) != 0)
			break ;
		ret[0] = ret[0] >> 1 ;
		ret[1]++ ;
	}
	ret[2] = DATA (-_EBOOL_ ((r1x & DATA (0X8000000000000000)) != 0)) ;
	return _MOVE_ (_CAST_ (ARGV<ARRAY3<VAR64>>::null ,ret)) ;
}

inline exports VAL64 MathStaticProc::static_taylor_exp (const VAL64 &lnx ,const VAL64 &y) {
	VAL64 ret = 1 ;
	const auto r1x = lnx * y ;
	_DEBUG_ASSERT_ (r1x >= 0) ;
	auto rax = VAL64 (1) ;
	auto rbx = VAL64 (1) ;
	while (TRUE) {
		rax *= r1x * MathProc::inverse (rbx) ;
		if (MathProc::abs (rax) < VAL64_EPS)
			break ;
		ret += rax ;
		rbx += VAL64 (1) ;
	}
	return _MOVE_ (ret) ;
}

inline exports ARRAY3<VAR64> MathStaticProc::static_ieee754_e2_e10_part (const ARRAY3<VAR64> &sne2) {
	const auto r1x = _CAST_ (ARGV<ARRAY3<DATA>>::null ,sne2) ;
	ARRAY3<DATA> ret = r1x ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~DATA (0X000FFFFFFFFFFFFF)) == 0)
			break ;
		ret[0] = ret[0] >> 1 ;
		ret[1]++ ;
	}
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		const auto r2x = DATA (ret[0] << 1) ;
		if ((r2x & ~DATA (0X000FFFFFFFFFFFFF)) != 0)
			break ;
		ret[0] = r2x ;
		ret[1]-- ;
	}
	return _MOVE_ (_CAST_ (ARGV<ARRAY3<VAR64>>::null ,ret)) ;
}

inline exports ARRAY3<VAR64> MathProc::ieee754_e2_e10 (const ARRAY3<VAR64> &sne2) {
	ARRAY3<DATA> ret ;
	const auto r1x = MathStaticProc::static_ieee754_e2_e10_part (sne2) ;
	_STATIC_WARNING_ ("mark") ;
	const auto r2x = VAL64 (MATH_LN2 / MATH_LN10) * VAL64 (r1x[1]) ;
	const auto r3x = MathProc::floor (r2x ,VAL64 (1)) ;
	const auto r4x = VAL64 (r1x[0]) * MathStaticProc::static_taylor_exp (VAL64 (MATH_LN10) ,(r2x - r3x)) ;
	ret[0] = DATA (VAR64 (r4x)) ;
	ret[1] = DATA (VAR64 (r3x)) ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if (ret[0] % 10 != 0)
			break ;
		ret[0] /= 10 ;
		ret[1]++ ;
	}
	ret[2] = r1x[2] ;
	return _MOVE_ (_CAST_ (ARGV<ARRAY3<VAR64>>::null ,ret)) ;
}

inline exports ARRAY3<VAR64> MathStaticProc::static_ieee754_e10_e2_part (const ARRAY3<VAR64> &sne10) {
	const auto r1x = _CAST_ (ARGV<ARRAY3<DATA>>::null ,sne10) ;
	ARRAY3<DATA> ret = r1x ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~DATA (0X000FFFFFFFFFFFFF)) == 0)
			break ;
		ret[0] /= 10 ;
		ret[1]++ ;
	}
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		const auto r2x = DATA ((ret[0] << 3) + (ret[0] << 1)) ;
		if ((r2x & ~DATA (0X000FFFFFFFFFFFFF)) != 0)
			break ;
		ret[0] = r2x ;
		ret[1]-- ;
	}
	return _MOVE_ (_CAST_ (ARGV<ARRAY3<VAR64>>::null ,ret)) ;
}

inline exports ARRAY3<VAR64> MathProc::ieee754_e10_e2 (const ARRAY3<VAR64> &sne10) {
	ARRAY3<DATA> ret ;
	const auto r1x = MathStaticProc::static_ieee754_e10_e2_part (sne10) ;
	_STATIC_WARNING_ ("mark") ;
	const auto r2x = VAL64 (MATH_LN10 / MATH_LN2) * VAL64 (r1x[1]) ;
	const auto r3x = MathProc::floor (r2x ,VAL64 (1)) ;
	const auto r4x = VAL64 (r1x[0]) * MathStaticProc::static_taylor_exp (VAL64 (MATH_LN2) ,(r2x - r3x)) ;
	ret[0] = DATA (VAR64 (r4x)) ;
	ret[1] = DATA (VAR64 (r3x)) ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & DATA (0X0000000000000001)) != 0)
			break ;
		ret[0] = ret[0] >> 1 ;
		ret[1]++ ;
	}
	ret[2] = r1x[2] ;
	return _MOVE_ (_CAST_ (ARGV<ARRAY3<VAR64>>::null ,ret)) ;
}
} ;