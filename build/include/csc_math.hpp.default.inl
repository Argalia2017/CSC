#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require 'csc_math.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef import
#undef export
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("import")
#pragma pop_macro ("export")
#endif

#ifdef __CSC_DEPRECATED__
#include <cmath>
#include <random>
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("import")
#pragma push_macro ("export")
#define self to ()
#define implicit
#define popping
#define import extern
#define export
#endif

namespace CSC {
#if defined (_CMATH_) || defined (_GLIBCXX_CMATH)
inline namespace S {
inline export VALX _SQRT_ (const VALX &arg) {
	return std::sqrt (arg) ;
}

inline export VALX _POW_ (const VALX &arg1 ,const VALX &arg2) {
	return std::pow (arg1 ,arg2) ;
}

inline export VALX _EXP_ (const VALX &arg) {
	return std::exp (arg) ;
}

inline export VALX _LOG_ (const VALX &arg) {
	return std::log (arg) ;
}

inline export VALX _SIN_ (const VALX &arg) {
	return std::sin (arg) ;
}

inline export VALX _COS_ (const VALX &arg) {
	return std::cos (arg) ;
}

inline export VALX _TAN_ (const VALX &arg) {
	return std::tan (arg) ;
}

inline export VALX _ASIN_ (const VALX &arg) {
	return std::asin (arg) ;
}

inline export VALX _ACOS_ (const VALX &arg) {
	return std::acos (arg) ;
}

inline export VALX _ATAN_ (const VALX &arg) {
	return std::atan (arg) ;
}

inline export VALX _ATAN_ (const VALX &arg1 ,const VALX &arg2) {
	return std::atan2 (arg1 ,arg2) ;
}
} ;
#endif
} ;