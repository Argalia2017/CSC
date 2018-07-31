#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : required csc_stream.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#include <cmath>
#endif

#if defined (_CMATH_) || defined (_GLIBCXX_CMATH)
namespace CSC {
inline namespace STREAM {
inline export ARRAY2<VAR64> _DOUBLEE2TOE10_ (const ARRAY2<VAR64> &arg) {
	const auto r1x = _CALL_ ([&] () {
		ARRAY2<VAR64> ret = arg ;
		auto &r1 = _CAST_<ARRAY2<DATA>> (ret) ;
		if ((r1[0] & DATA (0X8000000000000000)) != 0)
			r1[0] = 0 - r1[0] ;
		while (r1[0] != 0 && (r1[0] & ~0X000FFFFFFFFFFFFF) != 0X0010000000000000) {
			r1[0] <<= 1 ;
			r1[1]-- ;
		}
		return std::move (ret) ;
	}) ;
	ARRAY2<VAR64> ret ;
	auto &r2 = _CAST_<ARRAY2<DATA>> (ret) ;
	const auto r2x = VALX_LOGE2 / VALX_LOGE10 * r1x[1] ;
	ret[0] = VAR64 (r1x[0] * std::pow (10 ,r2x - VAR64 (r2x)) + VAL64 (0.5)) ;
	ret[1] = VAR64 (r2x) ;
	while (r2[0] != 0 && r2[0] % 10 == 0) {
		r2[0] /= 10 ;
		r2[1]++ ;
	}
	if (arg[0] < 0)
		ret[0] = -ret[0] ;
	return std::move (ret) ;
}

inline export ARRAY2<VAR64> _DOUBLEE10TOE2_ (const ARRAY2<VAR64> &arg) {
	const auto r1x = _CALL_ ([&] () {
		ARRAY2<VAR64> ret = arg ;
		if (arg[0] < 0)
			ret[0] = -ret[0] ;
		auto &r1 = _CAST_<ARRAY2<DATA>> (ret) ;
		while (r1[0] != 0 && (r1[0] & ~0X000FFFFFFFFFFFFF) == 0) {
			r1[0] = (r1[0] << 3) + (r1[0] << 1) ;
			r1[1]-- ;
		}
		return std::move (ret) ;
	}) ;
	ARRAY2<VAR64> ret ;
	auto &r2 = _CAST_<ARRAY2<DATA>> (ret) ;
	const auto r2x = VALX_LOGE10 / VALX_LOGE2 * r1x[1] ;
	ret[0] = VAR64 (r1x[0] * std::pow (2 ,r2x - VAR64 (r2x)) + VAL64 (0.5)) ;
	ret[1] = VAR64 (r2x) ;
	while (r2[0] != 0 && (r2[0] & DATA (~0X001FFFFFFFFFFFFF)) != 0) {
		r2[0] >>= 1 ;
		r2[1]++ ;
	}
	while (r2[0] != 0 && (r2[0] & DATA (0X0000000000000001)) == 0) {
		r2[0] >>= 1 ;
		r2[1]++ ;
	}
	if (arg[0] < 0)
		ret[0] = -ret[0] ;
	return std::move (ret) ;
}
} ;
} ;
#endif