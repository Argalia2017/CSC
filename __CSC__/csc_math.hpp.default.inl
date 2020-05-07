#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require 'csc_math.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_case
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
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using std::sqrt ;
using std::cbrt ;
using std::pow ;
using std::exp ;
using std::log ;
using std::erf ;
using std::sin ;
using std::cos ;
using std::tan ;
using std::asin ;
using std::acos ;
using std::atan ;
using std::atan2 ;
} ;

inline exports VAL32 MathProc::sqrt (const VAL32 &x) {
	return api::sqrt (x) ;
}

inline exports VAL64 MathProc::sqrt (const VAL64 &x) {
	return api::sqrt (x) ;
}

inline exports VAL32 MathProc::cbrt (const VAL32 &x) {
	return api::cbrt (x) ;
}

inline exports VAL64 MathProc::cbrt (const VAL64 &x) {
	return api::cbrt (x) ;
}

inline exports VAL32 MathProc::pow (const VAL32 &x ,const VAL32 &y) {
	return api::pow (x ,y) ;
}

inline exports VAL64 MathProc::pow (const VAL64 &x ,const VAL64 &y) {
	return api::pow (x ,y) ;
}

inline exports VAL32 MathProc::exp (const VAL32 &x) {
	return api::exp (x) ;
}

inline exports VAL64 MathProc::exp (const VAL64 &x) {
	return api::exp (x) ;
}

inline exports VAL32 MathProc::log (const VAL32 &x) {
	return api::log (x) ;
}

inline exports VAL64 MathProc::log (const VAL64 &x) {
	return api::log (x) ;
}

inline exports VAL32 MathProc::ncdf (const VAL32 &x) {
	return api::erf (x * MathProc::inverse (VAL32 (MATH_SQRT2))) / 2 + VAL32 (0.5) ;
}

inline exports VAL64 MathProc::ncdf (const VAL64 &x) {
	return api::erf (x * MathProc::inverse (VAL64 (MATH_SQRT2))) / 2 + VAL64 (0.5) ;
}

inline exports VAL32 MathProc::sin (const VAL32 &x) {
	return api::sin (x) ;
}

inline exports VAL64 MathProc::sin (const VAL64 &x) {
	return api::sin (x) ;
}

inline exports VAL32 MathProc::cos (const VAL32 &x) {
	return api::cos (x) ;
}

inline exports VAL64 MathProc::cos (const VAL64 &x) {
	return api::cos (x) ;
}

inline exports VAL32 MathProc::tan (const VAL32 &x) {
	return api::tan (x) ;
}

inline exports VAL64 MathProc::tan (const VAL64 &x) {
	return api::tan (x) ;
}

inline exports VAL32 MathProc::arcsin (const VAL32 &y) {
	return api::asin (y) ;
}

inline exports VAL64 MathProc::arcsin (const VAL64 &y) {
	return api::asin (y) ;
}

inline exports VAL32 MathProc::arccos (const VAL32 &x) {
	return api::acos (x) ;
}

inline exports VAL64 MathProc::arccos (const VAL64 &x) {
	return api::acos (x) ;
}

inline exports VAL32 MathProc::arctan (const VAL32 &x) {
	return api::atan (x) ;
}

inline exports VAL64 MathProc::arctan (const VAL64 &x) {
	return api::atan (x) ;
}

inline exports VAL32 MathProc::arctan (const VAL32 &y ,const VAL32 &x) {
	return api::atan2 (y ,x) ;
}

inline exports VAL64 MathProc::arctan (const VAL64 &y ,const VAL64 &x) {
	return api::atan2 (y ,x) ;
}
} ;