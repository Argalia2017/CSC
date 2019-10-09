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
inline exports VAL32 _SQRT_ (const VAL32 &x) {
	return std::sqrt (x) ;
}

inline exports VAL64 _SQRT_ (const VAL64 &x) {
	return std::sqrt (x) ;
}

inline exports VAL32 _POW_ (const VAL32 &x ,const VAL32 &y) {
	return std::pow (x ,y) ;
}

inline exports VAL64 _POW_ (const VAL64 &x ,const VAL64 &y) {
	return std::pow (x ,y) ;
}

inline exports VAL32 _EXP_ (const VAL32 &x) {
	return std::exp (x) ;
}

inline exports VAL64 _EXP_ (const VAL64 &x) {
	return std::exp (x) ;
}

inline exports VAL32 _LOG_ (const VAL32 &x) {
	return std::log (x) ;
}

inline exports VAL64 _LOG_ (const VAL64 &x) {
	return std::log (x) ;
}

inline exports VAL32 _SIN_ (const VAL32 &x) {
	return std::sin (x) ;
}

inline exports VAL64 _SIN_ (const VAL64 &x) {
	return std::sin (x) ;
}

inline exports VAL32 _COS_ (const VAL32 &x) {
	return std::cos (x) ;
}

inline exports VAL64 _COS_ (const VAL64 &x) {
	return std::cos (x) ;
}

inline exports VAL32 _TAN_ (const VAL32 &x) {
	return std::tan (x) ;
}

inline exports VAL64 _TAN_ (const VAL64 &x) {
	return std::tan (x) ;
}

inline exports VAL32 _ASIN_ (const VAL32 &y) {
	return std::asin (y) ;
}

inline exports VAL64 _ASIN_ (const VAL64 &y) {
	return std::asin (y) ;
}

inline exports VAL32 _ACOS_ (const VAL32 &x) {
	return std::acos (x) ;
}

inline exports VAL64 _ACOS_ (const VAL64 &x) {
	return std::acos (x) ;
}

inline exports VAL32 _ATAN_ (const VAL32 &x) {
	return std::atan (x) ;
}

inline exports VAL64 _ATAN_ (const VAL64 &x) {
	return std::atan (x) ;
}

inline exports VAL32 _ATAN_ (const VAL32 &y ,const VAL32 &x) {
	return std::atan2 (y ,x) ;
}

inline exports VAL64 _ATAN_ (const VAL64 &y ,const VAL64 &x) {
	return std::atan2 (y ,x) ;
}
} ;
#endif
} ;