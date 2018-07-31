#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_array.hpp"

namespace CSC {
inline namespace MATH {
inline import DEF<VALX (const VALX &arg)> _SQRT_ ;

template <class _ARG>
inline _ARG _SQRT_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_SQRT_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg1 ,const VALX &arg2)> _POW_ ;

template <class _ARG>
inline _ARG _POW_ (const _ARG &arg1 ,const _ARG &arg2) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_POW_ (VALX (arg1) ,VALX (arg2))) ;
}

inline import DEF<VALX (const VALX &arg)> _EXP_ ;

template <class _ARG>
inline _ARG _EXP_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_EXP_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _LOG_ ;

template <class _ARG>
inline _ARG _LOG_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_LOG_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _SIN_ ;

template <class _ARG>
inline _ARG _SIN_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_SIN_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _COS_ ;

template <class _ARG>
inline _ARG _COS_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_COS_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _TAN_ ;

template <class _ARG>
inline _ARG _TAN_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_TAN_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _ASIN_ ;

template <class _ARG>
inline _ARG _ASIN_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_ASIN_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _ACOS_ ;

template <class _ARG>
inline _ARG _ACOS_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_ACOS_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg)> _ATAN_ ;

template <class _ARG>
inline _ARG _ATAN_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_ATAN_ (VALX (arg))) ;
}

inline import DEF<VALX (const VALX &arg1 ,const VALX &arg2)> _ATAN_ ;

template <class _ARG>
inline _ARG _ATAN_ (const _ARG &arg1 ,const _ARG &arg2) {
	_STATIC_ASSERT_ (std::is_arithmetic<_ARG>::value) ;
	return _ARG (_ATAN_ (VALX (arg1) ,VALX (arg2))) ;
}

template <class _ARG>
inline _ARG _SIGN_ (const _ARG &arg) {
	return arg < _ARG (0) ? _ARG (-1) : arg == _ARG (0) ? _ARG (0) : _ARG (1) ;
}

template <class _ARG>
inline _ARG _ROUND_ (const _ARG &arg1 ,const _ARG &arg2) {
	_DEBUG_ASSERT_ (arg2 != _ARG (0)) ;
	const auto r1x = arg2 < _ARG (0) ? -arg1 : arg1 ;
	const auto r2x = _ABS_ (arg2) ;
	_ARG ret = _ARG (r2x * VAR64 (r1x / r2x)) ;
	if (r1x < 0 && ret > r1x)
		ret -= r2x ;
	if (arg2 < 0)
		ret = -ret ;
	return std::move (ret) ;
}

template <class _ARG1 ,class _ARG2>
inline _ARG2 _CLAMP_ (const _ARG1 &arg1 ,const _ARG2 &arg2 ,const _ARG2 &arg3) {
	return arg1 < arg2 ? arg2 : !(arg1 < arg3) ? arg3 : _ARG2 (arg1) ;
}

template <class _ARG>
inline ARRAY2<_ARG> _SORT_ (const _ARG &arg1 ,const _ARG &arg2) {
	return {_MIN_ (arg1 ,arg2) ,_MAX_ (arg1 ,arg2)} ;
}
} ;

#ifdef __CSC_DEPRECATED__
class Number {
public:
	Number () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;

class Operator {
public:
	Operator () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;

class Expression {
public:
	Expression () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;
#endif
} ;

#ifndef __CSC_STRICTHPPINL__
#include "csc_math.hpp.inl"
#endif