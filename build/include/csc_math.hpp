#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_array.hpp"

namespace CSC {
inline namespace S {
inline BOOL _ISNAN_ (const VAL32 &x) {
	const auto r1x = _CAST_<CHAR> (x) & CHAR (0X7F800000) ;
	const auto r2x = _CAST_<CHAR> (x) & CHAR (0X007FFFFF) ;
	if (r1x != CHAR (0X7F800000))
		return FALSE ;
	if (r2x == 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _ISNAN_ (const VAL64 &x) {
	const auto r1x = _CAST_<DATA> (x) & DATA (0X7FF0000000000000) ;
	const auto r2x = _CAST_<DATA> (x) & DATA (0X000FFFFFFFFFFFFF) ;
	if (r1x != DATA (0X7FF0000000000000))
		return FALSE ;
	if (r2x == 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _ISINF_ (const VAL32 &x) {
	const auto r1x = _CAST_<CHAR> (x) & CHAR (0X7F800000) ;
	const auto r2x = _CAST_<CHAR> (x) & CHAR (0X007FFFFF) ;
	if (r1x != CHAR (0X7F800000))
		return FALSE ;
	if (r2x != 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _ISINF_ (const VAL64 &x) {
	const auto r1x = _CAST_<DATA> (x) & DATA (0X7FF0000000000000) ;
	const auto r2x = _CAST_<DATA> (x) & DATA (0X000FFFFFFFFFFFFF) ;
	if (r1x != DATA (0X7FF0000000000000))
		return FALSE ;
	if (r2x != 0)
		return FALSE ;
	return TRUE ;
}

inline imports DEF<VALX (const VALX &x)> _SQRT_ ;

template <class _ARG1>
inline _ARG1 _SQRT_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_SQRT_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &x ,const VALX &y)> _POW_ ;

template <class _ARG1>
inline _ARG1 _POW_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_POW_ (VALX (x) ,VALX (y))) ;
}

inline imports DEF<VALX (const VALX &x)> _EXP_ ;

template <class _ARG1>
inline _ARG1 _EXP_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_EXP_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &x)> _LOG_ ;

template <class _ARG1>
inline _ARG1 _LOG_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_LOG_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &x)> _SIN_ ;

template <class _ARG1>
inline _ARG1 _SIN_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_SIN_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &x)> _COS_ ;

template <class _ARG1>
inline _ARG1 _COS_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_COS_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &x)> _TAN_ ;

template <class _ARG1>
inline _ARG1 _TAN_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_TAN_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &y)> _ASIN_ ;

template <class _ARG1>
inline _ARG1 _ASIN_ (const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_ASIN_ (VALX (y))) ;
}

inline imports DEF<VALX (const VALX &x)> _ACOS_ ;

template <class _ARG1>
inline _ARG1 _ACOS_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_ACOS_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &x)> _ATAN_ ;

template <class _ARG1>
inline _ARG1 _ATAN_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_ATAN_ (VALX (x))) ;
}

inline imports DEF<VALX (const VALX &y ,const VALX &x)> _ATAN_ ;

template <class _ARG1>
inline _ARG1 _ATAN_ (const _ARG1 &y ,const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_ATAN_ (VALX (y) ,VALX (x))) ;
}

template <class _ARG1>
inline _ARG1 _SIGN_ (const _ARG1 &x) {
	if (x < _ARG1 (0))
		return _ARG1 (-1) ;
	if (_ARG1 (0) < x)
		return _ARG1 (+1) ;
	return _ARG1 (0) ;
}

template <class _ARG1>
inline _ARG1 _PINV_ (const _ARG1 &x) {
	_STATIC_ASSERT_ (stl::is_same<_ARG1 ,VAL32>::value || stl::is_same<_ARG1 ,VAL64>::value || stl::is_same<_ARG1 ,VALX>::value) ;
	if (x == _ARG1 (0))
		return _ARG1 (0) ;
	return _ARG1 (1) / x ;
}

template <class _ARG1>
inline _ARG1 _PINV_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_same<_ARG1 ,VAL32>::value || stl::is_same<_ARG1 ,VAL64>::value || stl::is_same<_ARG1 ,VALX>::value) ;
	_DEBUG_ASSERT_ (y > VALX (0)) ;
	if (_ABS_ (x) < y)
		return _ARG1 (0) ;
	return _ARG1 (1) / x ;
}

inline VALX _FLOOR_ (const VALX &x ,const VALX &y) {
	_DEBUG_ASSERT_ (y > VALX (0)) ;
	const auto r1x = VAR64 (x * _PINV_ (y)) ;
	VALX ret = y * VALX (r1x) ;
	if (x < 0 && x < ret)
		ret = y * VALX (r1x - 1) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline _ARG1 _FLOOR_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_FLOOR_ (VALX (x) ,VALX (y))) ;
}

inline VALX _CEIL_ (const VALX &x ,const VALX &y) {
	_DEBUG_ASSERT_ (y > VALX (0)) ;
	const auto r1x = VAR64 (x * _PINV_ (y)) ;
	VALX ret = y * VALX (r1x) ;
	if (0 < x && ret < x)
		ret = y * VALX (r1x + 1) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline _ARG1 _CEIL_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	return _ARG1 (_CEIL_ (VALX (x) ,VALX (y))) ;
}

template <class _ARG1>
inline _ARG1 _ROUND_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	_DEBUG_ASSERT_ (y > _ARG1 (0)) ;
	return _ARG1 (_FLOOR_ (VALX (x) + VALX (y) / 2 ,VALX (y))) ;
}

template <class _ARG1 ,class _ARG2>
inline _ARG2 _CLAMP_ (const _ARG1 &arg1 ,const _ARG2 &arg2 ,const _ARG2 &arg3) {
	if (arg1 < _ARG1 (arg2))
		return arg2 ;
	if (_ARG1 (arg3) < arg1)
		return arg3 ;
	return _ARG2 (arg1) ;
}

template <class _ARG1>
inline const _ARG1 &_MINOF_ (const _ARG1 &arg1) {
	return arg1 ;
}

template <class _ARG1 ,class... _ARGS>
inline const _ARG1 &_MINOF_ (const _ARG1 &arg1 ,const _ARG1 &arg2 ,const _ARGS &...args) {
	return _MIN_ (arg1 ,_MINOF_ (arg2 ,args...)) ;
}

template <class _ARG1>
inline const _ARG1 &_MAXOF_ (const _ARG1 &arg1) {
	return arg1 ;
}

template <class _ARG1 ,class... _ARGS>
inline const _ARG1 &_MAXOF_ (const _ARG1 &arg1 ,const _ARG1 &arg2 ,const _ARGS &...args) {
	return _MAX_ (arg1 ,_MAXOF_ (arg2 ,args...)) ;
}

inline ARRAY3<DATA> _inline_IEEE754_ENCODE_PART_ (const ARRAY3<VAR64> &sne2) {
	ARRAY3<DATA> ret = _CAST_<ARRAY3<DATA>> (sne2) ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & DATA (~0X001FFFFFFFFFFFFF)) == 0)
			break ;
		ret[0] = ret[0] >> 1 ;
		ret[1]++ ;
	}
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~0X000FFFFFFFFFFFFF) == 0X0010000000000000)
			break ;
		ret[0] = ret[0] << 1 ;
		ret[1]-- ;
	}
	for (FOR_ONCE_DO_WHILE) {
		const auto r1x = -1074 - VAR64 (ret[1]) ;
		if (r1x <= 0)
			break ;
		ret[0] = ret[0] >> r1x ;
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
	_DYNAMIC_ASSERT_ ((r1x[0] & ~0X001FFFFFFFFFFFFF) == 0) ;
	_DYNAMIC_ASSERT_ ((r1x[1] & ~0X7FF0000000000000) == 0) ;
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
	const auto r3x = r1x & DATA (0X7FF0000000000000) ;
	if (r3x != 0)
		ret[0] |= DATA (0X0010000000000000) ;
	ret[1] = r3x >> 52 ;
	ret[1] -= DATA (1075 - EFLAG (r3x == 0)) ;
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
	const auto r4x = ((r1x & DATA (0X8000000000000000)) == 0) ? DATA (0) : DATA (-1) ;
	ret[2] = r4x ;
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline VAL64 _inline_TAYLOR_POW_ (VAL64 lnx ,VAL64 y) {
	VAL64 ret = 1 ;
	const auto r1x = lnx * y ;
	auto rax = VAL64 (1) ;
	auto rbx = VAL64 (1) ;
	while (TRUE) {
		rax *= r1x * _PINV_ (rbx) ;
		if (_ABS_ (rax) < VAL64_EPS)
			break ;
		ret += rax ;
		rbx++ ;
	}
	return std::move (ret) ;
}

inline ARRAY3<VAR64> _inline_IEEE754_E2TOE10_PART_ (const ARRAY3<VAR64> &sne2) {
	ARRAY3<DATA> ret = _CAST_<ARRAY3<DATA>> (sne2) ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~0X000FFFFFFFFFFFFF) == 0X0010000000000000)
			break ;
		ret[0] = ret[0] << 1 ;
		ret[1]-- ;
	}
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline ARRAY3<VAR64> _IEEE754_E2TOE10_ (const ARRAY3<VAR64> &sne2) {
	const auto r1x = _inline_IEEE754_E2TOE10_PART_ (sne2) ;
	ARRAY3<DATA> ret ;
	const auto r2x = VAL64 (MATH_LN2 / MATH_LN10) * VAL64 (r1x[1]) ;
	_STATIC_WARNING_ ("mark") ;
	const auto r3x = _inline_TAYLOR_POW_ (VAL64 (MATH_LN10) ,(r2x - VAR64 (r2x))) ;
	ret[0] = DATA (VAR64 (VAL64 (r1x[0]) * r3x)) ;
	ret[1] = DATA (VAR64 (r2x)) ;
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
	ARRAY3<DATA> ret = _CAST_<ARRAY3<DATA>> (sne10) ;
	while (TRUE) {
		if (ret[0] == 0)
			break ;
		if ((ret[0] & ~0X000FFFFFFFFFFFFF) != 0)
			break ;
		ret[0] = (ret[0] << 3) + (ret[0] << 1) ;
		ret[1]-- ;
	}
	return std::move (_CAST_<ARRAY3<VAR64>> (ret)) ;
}

inline ARRAY3<VAR64> _IEEE754_E10TOE2_ (const ARRAY3<VAR64> &sne10) {
	const auto r1x = _inline_IEEE754_E10TOE2_PART_ (sne10) ;
	ARRAY3<DATA> ret ;
	const auto r2x = VAL64 (MATH_LN10 / MATH_LN2) * VAL64 (r1x[1]) ;
	_STATIC_WARNING_ ("mark") ;
	const auto r3x = _inline_TAYLOR_POW_ (VAL64 (MATH_LN2) ,(r2x - VAR64 (r2x))) ;
	ret[0] = DATA (VAR64 (VAL64 (r1x[0]) * r3x)) ;
	ret[1] = DATA (VAR64 (r2x)) ;
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

#ifdef __CSC_DEPRECATED__
class Operand {
public:
	Operand () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;
#endif

#ifdef __CSC_DEPRECATED__
class Operator {
public:
	Operator () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;
#endif

#ifdef __CSC_DEPRECATED__
class Expression {
public:
	Expression () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;
#endif
} ;