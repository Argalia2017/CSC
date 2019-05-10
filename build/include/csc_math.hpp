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
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	if (x == _ARG1 (0))
		return _ARG1 (0) ;
	return _ARG1 (1) / x ;
}

template <class _ARG1>
inline _ARG1 _PINV_ (const _ARG1 &x ,const _ARG1 &y) {
	_STATIC_ASSERT_ (stl::is_arithmetic<_ARG1>::value) ;
	_DEBUG_ASSERT_ (y > VALX (0)) ;
	if (_ABS_ (x) < y)
		return _ARG1 (0) ;
	return _ARG1 (1) / x ;
}

inline VALX _FLOOR_ (const VALX &x ,const VALX &y) {
	_DEBUG_ASSERT_ (y > VALX (0)) ;
	const auto r1x = VAR64 (x / y) ;
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
	const auto r1x = VAR64 (x / y) ;
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

inline VAL64 _IEEE754_ENCODE_ (const ARRAY2<VAR64> &sne2) {
	DATA ret = 0 ;
	const auto r1x = _CALL_ ([&] () {
		ARRAY3<DATA> ret ;
		ret[0] = DATA (sne2[0]) ;
		ret[1] = DATA (sne2[1]) ;
		ret[2] = ret[0] ;
		if ((ret[0] & DATA (0X8000000000000000)) != 0)
			ret[0] = ~ret[0] + 1 ;
		while (ret[0] != 0 && (ret[0] & ~0X000FFFFFFFFFFFFF) != 0X0010000000000000) {
			ret[0] <<= 1 ;
			ret[1]-- ;
		}
		const auto r3x = VAR64 (ret[1]) ;
		while (VAR64 (ret[1]) <= -1075) {
			ret[0] >>= 1 ;
			ret[1]++ ;
		}
		ret[1] = ret[1] + 1075 - EFLAG (r3x <= -1075) ;
		ret[1] <<= 52 ;
		if (ret[0] == 0)
			ret[1] = 0 ;
		return std::move (ret) ;
	}) ;
	_DYNAMIC_ASSERT_ ((r1x[0] & ~0X001FFFFFFFFFFFFF) == 0) ;
	_DYNAMIC_ASSERT_ ((r1x[1] & ~0X7FF0000000000000) == 0) ;
	ret |= r1x[0] & DATA (0X000FFFFFFFFFFFFF) ;
	ret |= r1x[1] & DATA (0X7FF0000000000000) ;
	ret |= r1x[2] & DATA (0X8000000000000000) ;
	return std::move (_CAST_<VAL64> (ret)) ;
}

inline ARRAY2<VAR64> _IEEE754_DECODE_ (const VAL64 &ieee754) {
	ARRAY2<DATA> ret ;
	const auto r1x = _CAST_<DATA> (ieee754) ;
	ret[0] = r1x & DATA (0X000FFFFFFFFFFFFF) ;
	const auto r2x = r1x & DATA (0X7FF0000000000000) ;
	if (r2x != 0)
		ret[0] |= DATA (0X0010000000000000) ;
	ret[1] = r2x >> 52 ;
	ret[1] -= DATA (1074 + EFLAG (r2x != 0)) ;
	while (ret[0] != 0 && (ret[0] & DATA (0X0000000000000001)) == 0) {
		ret[0] >>= 1 ;
		ret[1]++ ;
	}
	if ((r1x & DATA (0X8000000000000000)) != 0)
		ret[0] = ~ret[0] + 1 ;
	if (r1x == 0) {
		ret[0] = 0 ;
		ret[1] = 0 ;
	}
	return std::move (_CAST_<ARRAY2<VAR64>> (ret)) ;
}

inline ARRAY2<VAR64> _IEEE754_E2TOE10_ (const ARRAY2<VAR64> &sne2) {
	_STATIC_WARNING_ ("note") ;
	/*
	X^(b/a) => { Y[0]=X^b ; Y[K+1]=Y[k]+((X^b)/(Y[k]^(a-1))-Y[k])/a ; }
	X^(b/a) = Y[inf]
	*/
	const auto r1x = _CALL_ ([&] () {
		ARRAY2<DATA> ret = _CAST_<ARRAY2<DATA>> (sne2) ;
		if (sne2[0] < 0)
			ret[0] = ~ret[0] + 1 ;
		_DEBUG_ASSERT_ ((ret[0] & DATA (0X8000000000000000)) == 0) ;
		while (ret[0] != 0 && (ret[0] & ~0X000FFFFFFFFFFFFF) != 0X0010000000000000) {
			ret[0] <<= 1 ;
			ret[1]-- ;
		}
		return std::move (_CAST_<ARRAY2<VAR64>> (ret)) ;
	}) ;
	const auto r2x = (VALX_LOGE2 / VALX_LOGE10) * r1x[1] ;
	ARRAY2<DATA> ret ;
	ret[0] = DATA (VAR64 (r1x[0] * _POW_ (10 ,(r2x - VAR64 (r2x))) + VAL64 (0.5))) ;
	ret[1] = DATA (VAR64 (r2x)) ;
	while (ret[0] != 0 && ret[0] % 10 == 0) {
		ret[0] /= 10 ;
		ret[1]++ ;
	}
	if (sne2[0] < 0)
		ret[0] = ~ret[0] + 1 ;
	return std::move (_CAST_<ARRAY2<VAR64>> (ret)) ;
}

inline ARRAY2<VAR64> _IEEE754_E10TOE2_ (const ARRAY2<VAR64> &sne10) {
	_STATIC_WARNING_ ("note") ;
	/*
	X^(b/a) => { Y[0]=X^b ; Y[K+1]=Y[k]+((X^b)/(Y[k]^(a-1))-Y[k])/a ; }
	X^(b/a) = Y[inf]
	*/
	const auto r1x = _CALL_ ([&] () {
		ARRAY2<DATA> ret = _CAST_<ARRAY2<DATA>> (sne10) ;
		if (sne10[0] < 0)
			ret[0] = ~ret[0] + 1 ;
		_DEBUG_ASSERT_ ((ret[0] & DATA (0X8000000000000000)) == 0) ;
		while (ret[0] != 0 && (ret[0] & ~0X000FFFFFFFFFFFFF) == 0) {
			ret[0] = (ret[0] << 3) + (ret[0] << 1) ;
			ret[1]-- ;
		}
		return std::move (_CAST_<ARRAY2<VAR64>> (ret)) ;
	}) ;
	const auto r2x = (VALX_LOGE10 / VALX_LOGE2) * r1x[1] ;
	ARRAY2<DATA> ret ;
	ret[0] = DATA (VAR64 (r1x[0] * _POW_ (2 ,(r2x - VAR64 (r2x))) + VAL64 (0.5))) ;
	ret[1] = DATA (VAR64 (r2x)) ;
	while (ret[0] != 0 && (ret[0] & DATA (~0X001FFFFFFFFFFFFF)) != 0) {
		ret[0] >>= 1 ;
		ret[1]++ ;
	}
	while (ret[0] != 0 && (ret[0] & DATA (0X0000000000000001)) == 0) {
		ret[0] >>= 1 ;
		ret[1]++ ;
	}
	if (sne10[0] < 0)
		ret[0] = ~ret[0] + 1 ;
	return std::move (_CAST_<ARRAY2<VAR64>> (ret)) ;
}
} ;

template <class TYPE ,class SUBJECT = VAR>
struct Classified {
	TYPE mData ;
	SUBJECT mClassi ;
} ;

template <class TYPE ,class SUBJECT = VAL>
struct Weighted {
	TYPE mData ;
	SUBJECT mWeight ;
} ;

#ifdef __CSC_DEPRECATED__
inline namespace S {
template <class _ARG1 ,class _ARG2>
inline Array<_ARG2> _MAP_ (const Array<_ARG1> &array1 ,const Function<_ARG2 (const _ARG1 &)> &func) {
	Array<_ARG2> ret = Array<_ARG2> (array1.size ()) ;
	for (INDEX i = 0 ; i < array1.length () ; i++)
		ret[i] = func (array1[i]) ;
	return std::move (ret) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline Array<_ARG3> _MAP_ (const Array<_ARG1> &array1 ,const Array<_ARG2> &array2 ,const Function<_ARG3 (const _ARG1 & ,const _ARG2 &)> &func) {
	_DEBUG_ASSERT_ (array1.size () == array2.size ()) ;
	Array<_ARG3> ret = Array<_ARG3> (array1.size ()) ;
	for (INDEX i = 0 ; i < array1.length () ; i++)
		ret[i] = func (array1[i] ,array2[i]) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline Array<_ARG1> _FILTER_ (const Array<_ARG1> &array1 ,const Array<INDEX> &elem) {
	Array<_ARG1> ret = Array<_ARG1> (elem.length ()) ;
	INDEX iw = 0 ;
	for (auto &&i : elem)
		ret[iw++] = array1[i] ;
	return std::move (ret) ;
}

inline Array<INDEX> _ELEMENT_ (const Array<BOOL> &elem) {
	const auto r1x = _CALL_ ([&] () {
		LENGTH ret = 0 ;
		for (auto &&i : elem)
			ret += EFLAG (i) ;
		return std::move (ret) ;
	}) ;
	Array<INDEX> ret = Array<INDEX> (r1x) ;
	INDEX iw = 0 ;
	for (INDEX i = 0 ; i < elem.length () ; i++) {
		if (!elem[i])
			continue ;
		ret[iw++] = i ;
	}
	return std::move (ret) ;
}

template <class _ARG1>
inline Array<_ARG1> _FILTER_ (const Array<_ARG1> &array1 ,const Array<BOOL> &elem) {
	_DEBUG_ASSERT_ (array1.size () == elem.size ()) ;
	return _FILTER_ (array1 ,_ELEMENT_ (elem)) ;
}

template <class _ARG1>
inline Array<_ARG1> _REDUCE_ (const _ARG1 &first ,LENGTH count ,const Function<_ARG1 (const _ARG1 &)> &func) {
	Array<_ARG1> ret = Array<_ARG1> (count) ;
	for (INDEX i = 0 ,ie = _MIN_ (LENGTH (1) ,ret.length ()) ; i < ie ; i++)
		ret[i] = first ;
	for (INDEX i = 1 ; i < ret.length () ; i++)
		ret[i] = func (ret[i - 1]) ;
	return std::move (ret) ;
}

template <class _ARG1 ,class _ARG2>
inline _ARG1 _REDUCE_ (const _ARG1 &first ,const Array<_ARG2> &array1 ,const Function<_ARG1 (const _ARG1 & ,const _ARG2 &)> &func) {
	_ARG1 ret = first ;
	for (auto &&i : array1)
		ret = func (ret ,i) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline Array<_ARG1> _CONCAT_ (const Array<_ARG1> &array1 ,const _ARG1 &array2) {
	Array<_ARG1> ret = Array<_ARG1> (array1.length () + 1) ;
	for (INDEX i = 0 ; i < array1.length () ; i++)
		ret[i] = array1[i] ;
	INDEX ix = array1.length () ;
	ret[ix] = array2 ;
	return std::move (ret) ;
}

template <class _ARG1>
inline Array<_ARG1> _CONCAT_ (const Array<_ARG1> &array1 ,const Array<_ARG1> &array2) {
	Array<_ARG1> ret = Array<_ARG1> (array1.length () + array2.length ()) ;
	for (INDEX i = 0 ; i < array1.length () ; i++)
		ret[i] = array1[i] ;
	INDEX ix = array1.length () ;
	for (INDEX i = 0 ; i < array2.length () ; i++)
		ret[ix + i] = array2[i] ;
	return std::move (ret) ;
}

template <class _ARG1 ,class... _ARGS>
inline Function<_ARG1 (const _ARGS &...)> _BIND_ (const PTR<_ARG1 (const _ARGS &...)> &func) {
	return Function<_ARG1 (const _ARGS &...)> (func) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class _ARG4>
inline Function<_ARG1 (const _ARG2 & ,const _ARG3 &)> _BIND_ (const decltype (ARGVP1) & ,const decltype (ARGVP1) & ,const decltype (ARGVP2) & ,const Function<_ARG4 (const _ARG2 & ,const _ARG3 &)> &func1 ,const Function<_ARG1 (const _ARG2 & ,const _ARG4 &)> &func2) {
	return Function<_ARG1 (const _ARG2 & ,const _ARG3 &)> ([&] (const _ARG2 &op1 ,const _ARG3 &op2) {
		return func2 (op1 ,func1 (op1 ,op2)) ;
	}) ;
}

template <class _ARG1>
inline Array<Array<_ARG1>> _GROUP_ (const Array<_ARG1> &array1 ,const Array<Function<BOOL (const _ARG1 &)>> &func) {
	Array<Array<_ARG1>> ret ;
	const auto r1x = _BIND_<Array<_ARG1> ,Array<_ARG1>> (&_COPY_<Array<_ARG1>>) ;
	const auto r2x = _REDUCE_ (array1 ,func.length () ,r1x) ;
	const auto r3x = _BIND_<Array<BOOL> ,Array<_ARG1> ,Function<BOOL (const _ARG1 &)>> (&_MAP_<_ARG1 ,BOOL>) ;
	const auto r4x = _BIND_<Array<_ARG1> ,Array<_ARG1> ,Array<BOOL>> (&_FILTER_< _ARG1>) ;
	const auto r5x = _BIND_<Array<_ARG1> ,Array<_ARG1> ,Function<BOOL (const _ARG1 &)>> (ARGVP1 ,ARGVP1 ,ARGVP2 ,r3x ,r4x) ;
	const auto r6x = _MAP_ (r2x ,func ,r5x) ;
	const auto r7x = _BIND_<Array<Array<_ARG1>> ,Array<Array<_ARG1>> ,Array<_ARG1>> (&_CONCAT_<Array<_ARG1>>) ;
	ret = _REDUCE_ (ret ,r6x ,r7x) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline Array<_ARG1> _FLATTEN_ (const Array<Array<_ARG1>> &array1) {
	const auto r1x = _CALL_ ([&] () {
		LENGTH ret = 0 ;
		for (auto &&i : array1)
			ret += i.length () ;
		return std::move (ret) ;
	}) ;
	Array<_ARG1> ret = Array<_ARG1> (r1x) ;
	INDEX iw = 0 ;
	for (auto &&i : array1) {
		for (auto &&j : i)
			ret[iw++] = j ;
	}
	return std::move (ret) ;
}
} ;
#endif

#ifdef __CSC_DEPRECATED__
class Number {
public:
	Number () {
		_STATIC_WARNING_ ("unimplemented") ;
	}
} ;
#endif

#ifdef __CSC_DEPRECATED__
class Operator {
public:
	Operator () {
		_STATIC_WARNING_ ("unimplemented") ;
	}
} ;
#endif

#ifdef __CSC_DEPRECATED__
class Expression {
public:
	Expression () {
		_STATIC_WARNING_ ("unimplemented") ;
	}
} ;
#endif
} ;