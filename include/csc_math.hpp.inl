#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : required csc_math.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#include <cmath>
#include <random>
#endif

#if defined (_CMATH_) || defined (_GLIBCXX_CMATH)
namespace CSC {
inline namespace MATH {
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
} ;
#endif