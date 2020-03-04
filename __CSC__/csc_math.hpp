#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"

namespace CSC {
inline namespace MATH {
inline BOOL _ISNAN_ (const VAL32 &x) {
	const auto r1x = _CAST_<CHAR> (x) ;
	if ((r1x & CHAR (0X7F800000)) != CHAR (0X7F800000))
		return FALSE ;
	if ((r1x & CHAR (0X007FFFFF)) == 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _ISNAN_ (const VAL64 &x) {
	const auto r1x = _CAST_<DATA> (x) ;
	if ((r1x & DATA (0X7FF0000000000000)) != DATA (0X7FF0000000000000))
		return FALSE ;
	if ((r1x & DATA (0X000FFFFFFFFFFFFF)) == 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _ISINF_ (const VAL32 &x) {
	const auto r1x = _CAST_<CHAR> (x) ;
	if ((r1x & CHAR (0X7F800000)) != CHAR (0X7F800000))
		return FALSE ;
	if ((r1x & CHAR (0X007FFFFF)) != 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _ISINF_ (const VAL64 &x) {
	const auto r1x = _CAST_<DATA> (x) ;
	if ((r1x & DATA (0X7FF0000000000000)) != DATA (0X7FF0000000000000))
		return FALSE ;
	if ((r1x & DATA (0X000FFFFFFFFFFFFF)) != 0)
		return FALSE ;
	return TRUE ;
}

template <class _ARG1>
inline _ARG1 _SQE_ (const _ARG1 &val) {
	return val * val ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _SQRT_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _SQRT_ ;

template <class _ARG1>
inline _ARG1 _SQRT_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_SQRT_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &x ,const VAL32 &y)> _POW_ ;
inline imports DEF<VAL64 (const VAL64 &x ,const VAL64 &y)> _POW_ ;

template <class _ARG1>
inline _ARG1 _POW_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_POW_ (VAL64 (x) ,VAL64 (y))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _EXP_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _EXP_ ;

template <class _ARG1>
inline _ARG1 _EXP_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_EXP_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _LOG_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _LOG_ ;

template <class _ARG1>
inline _ARG1 _LOG_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_LOG_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _SIN_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _SIN_ ;

template <class _ARG1>
inline _ARG1 _SIN_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_SIN_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _COS_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _COS_ ;

template <class _ARG1>
inline _ARG1 _COS_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_COS_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _TAN_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _TAN_ ;

template <class _ARG1>
inline _ARG1 _TAN_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_TAN_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &y)> _ASIN_ ;
inline imports DEF<VAL64 (const VAL64 &y)> _ASIN_ ;

template <class _ARG1>
inline _ARG1 _ASIN_ (const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_ASIN_ (VAL64 (y))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _ACOS_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _ACOS_ ;

template <class _ARG1>
inline _ARG1 _ACOS_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_ACOS_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &x)> _ATAN_ ;
inline imports DEF<VAL64 (const VAL64 &x)> _ATAN_ ;

template <class _ARG1>
inline _ARG1 _ATAN_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_ATAN_ (VAL64 (x))) ;
}

inline imports DEF<VAL32 (const VAL32 &y ,const VAL32 &x)> _ATAN_ ;
inline imports DEF<VAL64 (const VAL64 &y ,const VAL64 &x)> _ATAN_ ;

template <class _ARG1>
inline _ARG1 _ATAN_ (const _ARG1 &y ,const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_ATAN_ (VAL64 (y) ,VAL64 (x))) ;
}

template <class _ARG1>
inline _ARG1 _SIGN_ (const _ARG1 &x) {
	if (x < _ARG1 (0))
		return _ARG1 (-1) ;
	if (x > _ARG1 (0))
		return _ARG1 (+1) ;
	return _ARG1 (0) ;
}

template <class _ARG1>
inline _ARG1 _NEGATIVE_ (const _ARG1 &x ,VAR y) {
	_DEBUG_ASSERT_ (y >= 0) ;
	if (y % 2 == 0)
		return x ;
	return -x ;
}

template <class _ARG1>
inline _ARG1 _PINV_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_val_xyz<_ARG1>::value) ;
	_DEBUG_ASSERT_ (y > _ARG1 (0)) ;
	if (_ABS_ (x) < y)
		return _ARG1 (0) ;
	return _ARG1 (1) / x ;
}

inline VAL32 _PINV_ (const VAL32 &x) {
	return _PINV_ (x ,VAL32_EPS) ;
}

inline VAL64 _PINV_ (const VAL64 &x) {
	return _PINV_ (x ,VAL64_EPS) ;
}

inline VAL32 _FLOOR_ (const VAL32 &x ,const VAL32 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * _PINV_ (y)) ;
	const auto r2x = y * VAL32 (r1x) ;
	if (x >= 0)
		return r2x ;
	if (x >= r2x)
		return r2x ;
	return y * VAL32 (r1x - 1) ;
}

inline VAL64 _FLOOR_ (const VAL64 &x ,const VAL64 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * _PINV_ (y)) ;
	const auto r2x = y * VAL64 (r1x) ;
	if (x >= 0)
		return r2x ;
	if (x >= r2x)
		return r2x ;
	return y * VAL64 (r1x - 1) ;
}

template <class _ARG1>
inline _ARG1 _FLOOR_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_FLOOR_ (VAL64 (x) ,VAL64 (y))) ;
}

inline VAL32 _CEIL_ (const VAL32 &x ,const VAL32 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * _PINV_ (y)) ;
	const auto r2x = y * VAL32 (r1x) ;
	if (x <= 0)
		return r2x ;
	if (x <= r2x)
		return r2x ;
	return y * VAL32 (r1x + 1) ;
}

inline VAL64 _CEIL_ (const VAL64 &x ,const VAL64 &y) {
	_DEBUG_ASSERT_ (y > 0) ;
	const auto r1x = VAR64 (x * _PINV_ (y)) ;
	const auto r2x = y * VAL64 (r1x) ;
	if (x <= 0)
		return r2x ;
	if (x <= r2x)
		return r2x ;
	return y * VAL64 (r1x + 1) ;
}

template <class _ARG1>
inline _ARG1 _CEIL_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	return _ARG1 (_CEIL_ (VAL64 (x) ,VAL64 (y))) ;
}

template <class _ARG1>
inline _ARG1 _ROUND_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_var_xyz<_ARG1>::value || stl::is_val_xyz<_ARG1>::value) ;
	_DEBUG_ASSERT_ (y > _ARG1 (0)) ;
	const auto r1x = VAL64 (x) + VAL64 (y) / 2 ;
	return _ARG1 (_FLOOR_ (r1x ,VAL64 (y))) ;
}

template <class _ARG1 ,class _ARG2>
inline _ARG2 _CLAMP_ (const _ARG1 &val ,const _ARG2 &min_ ,const _ARG2 &max_) {
	if (val < _ARG1 (min_))
		return min_ ;
	if (val > _ARG1 (max_))
		return max_ ;
	return _ARG2 (val) ;
}

template <class _ARG1>
inline const _ARG1 &_MINOF_ (const _ARG1 &list_one) {
	return list_one ;
}

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
inline const _ARG1 &_MINOF_ (const _ARG1 &list_one ,const _ARG2 &list_two ,const _ARGS &...list_rest) {
	_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
	return _MINOF_ (_MIN_ (list_one ,list_two) ,list_rest...) ;
}

template <class _ARG1>
inline const _ARG1 &_MAXOF_ (const _ARG1 &list_one) {
	return list_one ;
}

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
inline const _ARG1 &_MAXOF_ (const _ARG1 &list_one ,const _ARG2 &list_two ,const _ARGS &...list_rest) {
	_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
	return _MAXOF_ (_MAX_ (list_one ,list_two) ,list_rest...) ;
}

inline ARRAY3<DATA> _inline_IEEE754_ENCODE_PART_ (const ARRAY3<VAR64> &sne2) {
	const auto r1x = _CAST_<ARRAY3<DATA>> (sne2) ;
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
	if switch_case (TRUE) {
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
	return std::move (ret) ;
}

inline VAL64 _IEEE754_ENCODE_ (const ARRAY3<VAR64> &sne2) {
	const auto r1x = _inline_IEEE754_ENCODE_PART_ (sne2) ;
	_DYNAMIC_ASSERT_ ((r1x[0] & ~DATA (0X001FFFFFFFFFFFFF)) == 0) ;
	_DYNAMIC_ASSERT_ ((r1x[1] & ~DATA (0X7FF0000000000000)) == 0) ;
	DATA ret = 0 ;
	ret |= r1x[0] & DATA (0X000FFFFFFFFFFFFF) ;
	ret |= r1x[1] & DATA (0X7FF0000000000000) ;
	ret |= r1x[2] & DATA (0X8000000000000000) ;
	return std::move (_CAST_<VAL64> (ret)) ;
}

inline ARRAY3<VAR64> _IEEE754_DECODE_ (const VAL64 &ieee754) {
	ARRAY3<DATA> ret ;
	const auto r1x = _CAST_<DATA> (ieee754) ;
	ret[0] = r1x & DATA (0X000FFFFFFFFFFFFF) ;
	const auto r2x = DATA (r1x & DATA (0X7FF0000000000000)) ;
	if (r2x != 0)
		ret[0] |= DATA (0X0010000000000000) ;
	ret[1] = r2x >> 52 ;
	ret[1] -= DATA (1075 - EFLAG (r2x == 0)) ;
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
	const auto r3x = EFLAG ((r1x & DATA (0X8000000000000000)) != 0) * DATA (-1) ;
	ret[2] = r3x ;
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline VAL64 _inline_TAYLOR_EXP_ (VAL64 lnx ,VAL64 y) {
	VAL64 ret = 1 ;
	const auto r1x = lnx * y ;
	_DEBUG_ASSERT_ (r1x >= 0) ;
	auto rax = VAL64 (1) ;
	auto rbx = VAL64 (1) ;
	while (TRUE) {
		rax *= r1x * _PINV_ (rbx) ;
		if (_ABS_ (rax) < VAL64_EPS)
			break ;
		ret += rax ;
		rbx += VAL64 (1) ;
	}
	return std::move (ret) ;
}

inline ARRAY3<VAR64> _inline_IEEE754_E2TOE10_PART_ (const ARRAY3<VAR64> &sne2) {
	const auto r1x = _CAST_<ARRAY3<DATA>> (sne2) ;
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
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline ARRAY3<VAR64> _IEEE754_E2TOE10_ (const ARRAY3<VAR64> &sne2) {
	ARRAY3<DATA> ret ;
	const auto r1x = _inline_IEEE754_E2TOE10_PART_ (sne2) ;
	_STATIC_WARNING_ ("mark") ;
	const auto r2x = VAL64 (MATH_LN2 / MATH_LN10) * VAL64 (r1x[1]) ;
	const auto r3x = _FLOOR_ (r2x ,VAL64 (1)) ;
	const auto r4x = VAL64 (r1x[0]) * _inline_TAYLOR_EXP_ (VAL64 (MATH_LN10) ,(r2x - r3x)) ;
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
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline ARRAY3<VAR64> _inline_IEEE754_E10TOE2_PART_ (const ARRAY3<VAR64> &sne10) {
	const auto r1x = _CAST_<ARRAY3<DATA>> (sne10) ;
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
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline ARRAY3<VAR64> _IEEE754_E10TOE2_ (const ARRAY3<VAR64> &sne10) {
	ARRAY3<DATA> ret ;
	const auto r1x = _inline_IEEE754_E10TOE2_PART_ (sne10) ;
	_STATIC_WARNING_ ("mark") ;
	const auto r2x = VAL64 (MATH_LN10 / MATH_LN2) * VAL64 (r1x[1]) ;
	const auto r3x = _FLOOR_ (r2x ,VAL64 (1)) ;
	const auto r4x = VAL64 (r1x[0]) * _inline_TAYLOR_EXP_ (VAL64 (MATH_LN2) ,(r2x - r3x)) ;
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
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}
} ;
} ;