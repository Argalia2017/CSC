#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require 'csc_math.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <cmath>
#include <random>
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("discard")
#endif

namespace CSC {
#if defined (_CMATH_) || defined (_GLIBCXX_CMATH)
inline namespace S {
inline exports VALX _SQRT_ (const VALX &x) {
	return std::sqrt (x) ;
}

inline exports VALX _POW_ (const VALX &x ,const VALX &y) {
	return std::pow (x ,y) ;
}

inline exports VALX _EXP_ (const VALX &x) {
	return std::exp (x) ;
}

inline exports VALX _LOG_ (const VALX &x) {
	return std::log (x) ;
}

inline exports VALX _SIN_ (const VALX &x) {
	return std::sin (x) ;
}

inline exports VALX _COS_ (const VALX &x) {
	return std::cos (x) ;
}

inline exports VALX _TAN_ (const VALX &x) {
	return std::tan (x) ;
}

inline exports VALX _ASIN_ (const VALX &y) {
	return std::asin (y) ;
}

inline exports VALX _ACOS_ (const VALX &x) {
	return std::acos (x) ;
}

inline exports VALX _ATAN_ (const VALX &x) {
	return std::atan (x) ;
}

inline exports VALX _ATAN_ (const VALX &y ,const VALX &x) {
	return std::atan2 (y ,x) ;
}
} ;
#endif
} ;