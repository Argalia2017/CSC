#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"

namespace CSC {
namespace U {
template <class ,class>
struct OPERATOR_CVT_STRING ;
} ;

inline namespace S {
inline String<STRU16> _U8STOU16S_ (const String<STRU8> &src) ;
inline String<STRU8> _U16STOU8S_ (const String<STRU16> &src) ;
inline String<STRU32> _U8STOU32S_ (const String<STRU8> &src) ;
inline String<STRU8> _U32STOU8S_ (const String<STRU32> &src) ;
inline String<STRU32> _U16STOU32S_ (const String<STRU16> &src) ;
inline String<STRU16> _U32STOU16S_ (const String<STRU32> &src) ;
inline String<STRU8> _WSTOU8S_ (const String<STRW> &src) ;
inline String<STRW> _U8STOWS_ (const String<STRU8> &src) ;
inline String<STRU16> _WSTOU16S_ (const String<STRW> &src) ;
inline String<STRW> _U16STOWS_ (const String<STRU16> &src) ;
inline String<STRU32> _WSTOU32S_ (const String<STRW> &src) ;
inline String<STRW> _U32STOWS_ (const String<STRU32> &src) ;
inline String<STRW> _GBKSTOWS_ (const String<STRA> &src) ;
inline String<STRA> _WSTOGBKS_ (const String<STRW> &src) ;
inline String<STRU8> _ASTOU8S_ (const String<STRA> &src) ;
inline String<STRA> _U8STOAS_ (const String<STRU8> &src) ;
inline String<STRU16> _ASTOU16S_ (const String<STRA> &src) ;
inline String<STRA> _U16STOAS_ (const String<STRU16> &src) ;
inline String<STRU32> _ASTOU32S_ (const String<STRA> &src) ;
inline String<STRA> _U32STOAS_ (const String<STRU32> &src) ;

inline imports DEF<String<STRW> (const String<STRA> &src)> _ASTOWS_ ;
inline imports DEF<String<STRA> (const String<STRW> &src)> _WSTOAS_ ;
} ;

namespace U {
template <>
struct OPERATOR_CVT_STRING<String<STRU8> ,String<STRU8>> {
	inline static String<STRU8> invoke (const String<STRU8> &src) {
		return _COPY_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU16> ,String<STRU16>> {
	inline static String<STRU16> invoke (const String<STRU16> &src) {
		return _COPY_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU32> ,String<STRU32>> {
	inline static String<STRU32> invoke (const String<STRU32> &src) {
		return _COPY_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRA> ,String<STRA>> {
	inline static String<STRA> invoke (const String<STRA> &src) {
		return _COPY_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRW> ,String<STRW>> {
	inline static String<STRW> invoke (const String<STRW> &src) {
		return _COPY_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU8> ,String<STRU16>> {
	inline static String<STRU8> invoke (const String<STRU16> &src) {
		return _U16STOU8S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU16> ,String<STRU8>> {
	inline static String<STRU16> invoke (const String<STRU8> &src) {
		return _U8STOU16S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU8> ,String<STRU32>> {
	inline static String<STRU8> invoke (const String<STRU32> &src) {
		return _U32STOU8S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU32> ,String<STRU8>> {
	inline static String<STRU32> invoke (const String<STRU8> &src) {
		return _U8STOU32S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU16> ,String<STRU32>> {
	inline static String<STRU16> invoke (const String<STRU32> &src) {
		return _U32STOU16S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU32> ,String<STRU16>> {
	inline static String<STRU32> invoke (const String<STRU16> &src) {
		return _U16STOU32S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU8> ,String<STRW>> {
	inline static String<STRU8> invoke (const String<STRW> &src) {
		return _WSTOU8S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRW> ,String<STRU8>> {
	inline static String<STRW> invoke (const String<STRU8> &src) {
		return _U8STOWS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU16> ,String<STRW>> {
	inline static String<STRU16> invoke (const String<STRW> &src) {
		return _WSTOU16S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRW> ,String<STRU16>> {
	inline static String<STRW> invoke (const String<STRU16> &src) {
		return _U16STOWS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU32> ,String<STRW>> {
	inline static String<STRU32> invoke (const String<STRW> &src) {
		return _WSTOU32S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRW> ,String<STRU32>> {
	inline static String<STRW> invoke (const String<STRU32> &src) {
		return _U32STOWS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRW> ,String<STRA>> {
	inline static String<STRW> invoke (const String<STRA> &src) {
		return _ASTOWS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRA> ,String<STRW>> {
	inline static String<STRA> invoke (const String<STRW> &src) {
		return _WSTOAS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU8> ,String<STRA>> {
	inline static String<STRU8> invoke (const String<STRA> &src) {
		return _ASTOU8S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRA> ,String<STRU8>> {
	inline static String<STRA> invoke (const String<STRU8> &src) {
		return _U8STOAS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU16> ,String<STRA>> {
	inline static String<STRU16> invoke (const String<STRA> &src) {
		return _ASTOU16S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRA> ,String<STRU16>> {
	inline static String<STRA> invoke (const String<STRU16> &src) {
		return _U16STOAS_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRU32> ,String<STRA>> {
	inline static String<STRU32> invoke (const String<STRA> &src) {
		return _ASTOU32S_ (src) ;
	}
} ;

template <>
struct OPERATOR_CVT_STRING<String<STRA> ,String<STRU32>> {
	inline static String<STRA> invoke (const String<STRU32> &src) {
		return _U32STOAS_ (src) ;
	}
} ;
} ;

inline namespace S {
inline String<STRU16> _U8STOU16S_ (const String<STRU8> &src) {
	String<STRU16> ret = String<STRU16> (src.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0X7F)))
				discard ;
			ret[iw++] = STRU16 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XDF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X1F)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XEF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X0F)) ;
			rax = 2 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XF7)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X07)) ;
			rax = 3 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFB)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X03)) ;
			rax = 4 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFD)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X01)) ;
			rax = 5 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax = 10 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax >= 2 && rax <= 5))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax-- ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
		if (rax < 10)
			continue ;
		auto ifb = FALSE ;
		if SWITCH_CASE (ifb) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = STRU16 (rbx) ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifb) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X0010FFFF)))
				discard ;
			rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
			ret[iw++] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifb) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = STRU16 ('?') ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifb) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU16 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> _U16STOU8S_ (const String<STRU16> &src) {
	String<STRU8> ret = String<STRU8> (src.length () * 3) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X007F)))
				discard ;
			ret[iw++] = STRU8 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X07FF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)))
				discard ;
			rbx = STRU32 (i & STRU16 (0X03FF)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			if (!(i >= STRU16 (0XDC00) && i <= STRU16 (0XDFFF)))
				discard ;
			rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
			ret[iw++] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
			ret[iw++] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU8 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU32> _U8STOU32S_ (const String<STRU8> &src) {
	_STATIC_WARNING_ ("note") ;
	/*
	1 bytes [0,0X7F] 0xxxxxxx
	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	String<STRU32> ret = String<STRU32> (src.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0X7F)))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XDF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X1F)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XEF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X0F)) ;
			rax = 2 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XF7)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X07)) ;
			rax = 3 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFB)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X03)) ;
			rax = 4 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFD)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X01)) ;
			rax = 5 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax >= 2 && rax <= 5))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax-- ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU32 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> _U32STOU8S_ (const String<STRU32> &src) {
	_STATIC_WARNING_ ("note") ;
	/*
	1 bytes [0,0X7F] 0xxxxxxx
	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	String<STRU8> ret = String<STRU8> (src.length () * 6) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000007F)))
				discard ;
			ret[iw++] = STRU8 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X000007FF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X001FFFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X03FFFFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
			ret[iw++] = (STRU8 (i >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU32> _U16STOU32S_ (const String<STRU16> &src) {
	_STATIC_WARNING_ ("note") ;
	/*
	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
	*/
	String<STRU32> ret = String<STRU32> (src.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X07FF)))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)))
				discard ;
			rbx = STRU32 (i & STRU16 (0X03FF)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			if (!(i >= STRU16 (0XDC00) && i <= STRU16 (0XDFFF)))
				discard ;
			rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU32 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU16> _U32STOU16S_ (const String<STRU32> &src) {
	_STATIC_WARNING_ ("note") ;
	/*
	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
	*/
	String<STRU16> ret = String<STRU16> (src.length () * 2) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = STRU16 (i) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0010FFFF)))
				discard ;
			ret[iw++] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = STRU16 ('?') ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> _WSTOU8S_ (const String<STRW> &src) {
	auto &r1 = _CAST_<String<STRUW>> (src) ;
	return U::OPERATOR_CVT_STRING<String<STRU8> ,String<STRUW>>::invoke (r1) ;
}

inline String<STRW> _U8STOWS_ (const String<STRU8> &src) {
	String<STRUW> ret = U::OPERATOR_CVT_STRING<String<STRUW> ,String<STRU8>>::invoke (src) ;
	return std::move (_CAST_<String<STRW>> (ret)) ;
}

inline String<STRU16> _WSTOU16S_ (const String<STRW> &src) {
	auto &r1 = _CAST_<String<STRUW>> (src) ;
	return U::OPERATOR_CVT_STRING<String<STRU16> ,String<STRUW>>::invoke (r1) ;
}

inline String<STRW> _U16STOWS_ (const String<STRU16> &src) {
	String<STRUW> ret = U::OPERATOR_CVT_STRING<String<STRUW> ,String<STRU16>>::invoke (src) ;
	return std::move (_CAST_<String<STRW>> (ret)) ;
}

inline String<STRU32> _WSTOU32S_ (const String<STRW> &src) {
	auto &r1 = _CAST_<String<STRUW>> (src) ;
	return U::OPERATOR_CVT_STRING<String<STRU32> ,String<STRUW>>::invoke (r1) ;
}

inline String<STRW> _U32STOWS_ (const String<STRU32> &src) {
	String<STRUW> ret = U::OPERATOR_CVT_STRING<String<STRUW> ,String<STRU32>>::invoke (src) ;
	return std::move (_CAST_<String<STRW>> (ret)) ;
}

inline String<STRU8> _ASTOU8S_ (const String<STRA> &src) {
	return _WSTOU8S_ (_ASTOWS_ (src)) ;
}

inline String<STRA> _U8STOAS_ (const String<STRU8> &src) {
	return _WSTOAS_ (_U8STOWS_ (src)) ;
}

inline String<STRU16> _ASTOU16S_ (const String<STRA> &src) {
	return _WSTOU16S_ (_ASTOWS_ (src)) ;
}

inline String<STRA> _U16STOAS_ (const String<STRU16> &src) {
	return _WSTOAS_ (_U16STOWS_ (src)) ;
}

inline String<STRU32> _ASTOU32S_ (const String<STRA> &src) {
	return _WSTOU32S_ (_ASTOWS_ (src)) ;
}

inline String<STRA> _U32STOAS_ (const String<STRU32> &src) {
	return _WSTOAS_ (_U32STOWS_ (src)) ;
}

inline String<STRU8> _UASTOU8S_ (const String<STRA> &src) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRU8> ret = std::move (_CAST_<String<STRUA>> (src)) ;
	for (auto &&i : ret) {
		const auto r1x = i ;
		_DEBUG_ASSERT_ (r1x <= STRUA (0X7F)) ;
		(void) r1x ;
	}
	return std::move (ret) ;
}

inline String<STRU8> _UASTOU8S_ (String<STRA> &&src) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRU8> ret = std::move (_CAST_<String<STRUA>> (src)) ;
	for (auto &&i : ret) {
		const auto r1x = i ;
		_DEBUG_ASSERT_ (r1x <= STRUA (0X7F)) ;
		(void) r1x ;
	}
	return std::move (ret) ;
}

inline String<STRA> _U8STOUAS_ (const String<STRU8> &src) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRUA> ret = std::move (src) ;
	for (auto &&i : ret) {
		const auto r1x = i ;
		_DEBUG_ASSERT_ (r1x <= STRUA (0X7F)) ;
		(void) r1x ;
	}
	return std::move (_CAST_<String<STRA>> (ret)) ;
}

inline String<STRA> _U8STOUAS_ (String<STRU8> &&src) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRUA> ret = std::move (src) ;
	for (auto &&i : ret) {
		const auto r1x = i ;
		_DEBUG_ASSERT_ (r1x <= STRUA (0X7F)) ;
		(void) r1x ;
	}
	return std::move (_CAST_<String<STRA>> (ret)) ;
}

inline imports DEF<PhanBuffer<const DEF<STRUW[2]>> ()> _LOADUWSTOUGBKSTABLE_ ;

inline String<STRW> _GBKSTOWS_ (const String<STRA> &src) {
	auto &r1 = _CACHE_ ([] () {
		const auto r1x = _LOADUWSTOUGBKSTABLE_ () ;
		HashSet<STRUW ,STRUW> ret = HashSet<STRUW ,STRUW> (r1x.size ()) ;
		for (INDEX i = 0 ,ie = r1x.size () ; i < ie ; i++)
			ret.add (r1x[i][1] ,r1x[i][0]) ;
		return std::move (ret) ;
	}) ;
	String<STRW> ret = String<STRW> (src.length ()) ;
	INDEX iw = 0 ;
	auto &r2 = _CAST_<String<STRUA>> (src) ;
	auto rax = VAR_ZERO ;
	auto rbx = STRUW () ;
	for (auto &&i : r2) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			rbx = STRUW (i) ;
			rax = 10 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			rbx = STRUW ((rbx << 8) | STRUW (i)) ;
			rax = 11 ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
		if (rax < 10)
			continue ;
		INDEX ix = r1.find (rbx) ;
		auto ifb = FALSE ;
		if SWITCH_CASE (ifb) {
			if (!(rax == 10))
				discard ;
			if (!(ix != VAR_NONE))
				discard ;
			ret[iw++] = STRW (r1[ix].item) ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifb) {
			if (!(rax == 10))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifb) {
			if (!(rax == 11))
				discard ;
			if (!(ix != VAR_NONE))
				discard ;
			ret[iw++] = STRW (r1[ix].item) ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifb) {
			if (!(rax == 11))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			ret[iw++] = STRW ('?') ;
			rax = 0 ;
		}
		if SWITCH_CASE (ifb) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRW ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRA> _WSTOGBKS_ (const String<STRW> &src) {
	auto &r1 = _CACHE_ ([] () {
		const auto r1x = _LOADUWSTOUGBKSTABLE_ () ;
		HashSet<STRUW ,STRUW> ret = HashSet<STRUW ,STRUW> (r1x.size ()) ;
		for (INDEX i = 0 ,ie = r1x.size () ; i < ie ; i++)
			ret.add (r1x[i][0] ,r1x[i][1]) ;
		return std::move (ret) ;
	}) ;
	String<STRUA> ret = String<STRUA> (src.length () * 2) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		INDEX ix = r1.find (STRUW (i)) ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			ret[iw++] = STRUA ('?') ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(r1[ix].item <= STRUW (0X00FF)))
				discard ;
			ret[iw++] = STRUA (r1[ix].item) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(r1[ix].item <= STRUW (0XFFFF)))
				discard ;
			ret[iw++] = STRUA (r1[ix].item >> 8) ;
			ret[iw++] = STRUA (r1[ix].item) ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (_CAST_<String<STRA>> (ret)) ;
}
} ;

inline namespace S {
template <class _ARG1>
inline BOOL _PARSEBOOLS_ (const String<_ARG1> &stri) {
	BOOL ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDBOOLS_ (const BOOL &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (7) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAR32 _PARSEVAR32S_ (const String<_ARG1> &stri) {
	VAR32 ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAR32S_ (const VAR32 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (63) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAR64 _PARSEVAR64S_ (const String<_ARG1> &stri) {
	VAR64 ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAR64S_ (const VAR64 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAR _PARSEVARS_ (const String<_ARG1> &stri) {
	VAR ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVARS_ (const VAR &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAL32 _PARSEVAL32S_ (const String<_ARG1> &stri) {
	VAL32 ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAL32S_ (const VAL32 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAL64 _PARSEVAL64S_ (const String<_ARG1> &stri) {
	VAL64 ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAL64S_ (const VAL64 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAL _PARSEVALS_ (const String<_ARG1> &stri) {
	VAL ret ;
	TextReader<_ARG1> (stri.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVALS_ (const VAL &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline String<STR> _PARSESTRS_ (const String<_ARG1> &stri) {
	return U::OPERATOR_CVT_STRING<String<STR> ,String<_ARG1>>::invoke (stri) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDSTRS_ (const String<STR> &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return U::OPERATOR_CVT_STRING<String<_RET> ,String<STR>>::invoke (stru) ;
}
} ;

template <class ITEM ,class SIZE>
template <class... _ARGS>
inline String<ITEM ,SIZE> String<ITEM ,SIZE>::make (const _ARGS &...args) {
	using DEFAULT_LONGSTRING_SIZE = ARGC<8195> ;
	_STATIC_ASSERT_ (std::is_same<SIZE ,SAUTO>::value) ;
	String<ITEM> ret = String<ITEM> (DEFAULT_LONGSTRING_SIZE::value) ;
	auto wos = TextWriter<ITEM> (ret.raw ()) ;
	_PRINTS_ (wos ,args...) ;
	wos << _EOS_ ;
	return std::move (ret) ;
}

class RegexMatcher {
private:
	class Implement ;
	AnyRef<void> mThis ;

public:
	RegexMatcher () = delete ;

	explicit RegexMatcher (const String<STRU8> &reg) ;

	BOOL match (const String<STRU8> &expr) const ;

	Deque<ARRAY2<INDEX>> search (const String<STRU8> &expr) const ;

	String<STRU8> replace (const String<STRU8> &expr ,const String<STRU8> &rep) const ;
} ;

inline namespace S {
template <class _ARG1>
inline CHAR _PARSEHEX8S_ (const String<_ARG1> &stri) {
	CHAR ret = 0 ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	auto rax = _ARG1 () ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('&')) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('H')) ;
	for (INDEX i = 0 ,ie = 8 ; i < ie ; i++) {
		ris >> rax ;
		const auto r1x = BOOL (rax >= _ARG1 ('0') && rax <= _ARG1 ('9')) ;
		const auto r2x = BOOL (rax >= _ARG1 ('A') && rax <= _ARG1 ('F')) ;
		_DYNAMIC_ASSERT_ (r1x || r2x) ;
		const auto r3x = _SWITCH_ (
			r1x ? (_ARG1 ('0')) :
			(_ARG1 ('A' - 10))) ;
		ret = (ret << 4) | CHAR (rax - r3x) ;
	}
	ris >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDHEX8S_ (const CHAR &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (15) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << _RET ('&') ;
	wos << _RET ('H') ;
	for (INDEX i = 0 ,ie = 8 ; i < ie ; i++) {
		const auto r1x = CHAR (stru >> (28 - i * 4)) & CHAR (0X0F) ;
		const auto r2x = _SWITCH_ (
			(r1x < DATA (10)) ? (_RET ('0')) :
			(_RET ('A') - 10)) ;
		wos << _RET (r2x + r1x) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline DATA _PARSEHEX16S_ (const String<_ARG1> &stri) {
	DATA ret = 0 ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	auto rax = _ARG1 () ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('&')) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('H')) ;
	for (INDEX i = 0 ,ie = 16 ; i < ie ; i++) {
		ris >> rax ;
		const auto r1x = BOOL (rax >= _ARG1 ('0') && rax <= _ARG1 ('9')) ;
		const auto r2x = BOOL (rax >= _ARG1 ('A') && rax <= _ARG1 ('F')) ;
		_DYNAMIC_ASSERT_ (r1x || r2x) ;
		const auto r3x = _SWITCH_ (
			r1x ? (_ARG1 ('0')) :
			(_ARG1 ('A' - 10))) ;
		ret = (ret << 4) | DATA (rax - r3x) ;
	}
	ris >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDHEX16S_ (const DATA &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (31) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << _RET ('&') ;
	wos << _RET ('H') ;
	for (INDEX i = 0 ,ie = 16 ; i < ie ; i++) {
		const auto r1x = DATA (stru >> (60 - i * 4)) & DATA (0X0F) ;
		const auto r2x = _SWITCH_ (
			(r1x < DATA (10)) ? (_RET ('0')) :
			(_RET ('A') - 10)) ;
		wos << _RET (r2x + r1x) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDBASE64U8S_ (const String<STRU8> &src) {
	static constexpr auto M_BASE64 = PACK<STRU8[66]> ({
		STRU8 ('-') ,STRU8 ('A') ,STRU8 ('B') ,STRU8 ('C') ,STRU8 ('D') ,STRU8 ('E') ,STRU8 ('F') ,STRU8 ('G') ,
		STRU8 ('H') ,STRU8 ('I') ,STRU8 ('J') ,STRU8 ('K') ,STRU8 ('L') ,STRU8 ('M') ,STRU8 ('N') ,STRU8 ('O') ,
		STRU8 ('P') ,STRU8 ('Q') ,STRU8 ('R') ,STRU8 ('S') ,STRU8 ('T') ,STRU8 ('U') ,STRU8 ('V') ,STRU8 ('W') ,
		STRU8 ('X') ,STRU8 ('Y') ,STRU8 ('Z') ,STRU8 ('a') ,STRU8 ('b') ,STRU8 ('c') ,STRU8 ('d') ,STRU8 ('e') ,
		STRU8 ('f') ,STRU8 ('g') ,STRU8 ('h') ,STRU8 ('i') ,STRU8 ('j') ,STRU8 ('k') ,STRU8 ('l') ,STRU8 ('m') ,
		STRU8 ('n') ,STRU8 ('o') ,STRU8 ('p') ,STRU8 ('q') ,STRU8 ('r') ,STRU8 ('s') ,STRU8 ('t') ,STRU8 ('u') ,
		STRU8 ('v') ,STRU8 ('w') ,STRU8 ('x') ,STRU8 ('y') ,STRU8 ('z') ,STRU8 ('_') ,STRU8 ('0') ,STRU8 ('1') ,
		STRU8 ('2') ,STRU8 ('3') ,STRU8 ('4') ,STRU8 ('5') ,STRU8 ('6') ,STRU8 ('7') ,STRU8 ('8') ,STRU8 ('9') ,
		STRU8 ('.') ,STRU8 (':')}) ;
	const auto r1x = (src.length () + 2) / 3 * 4 ;
	String<_RET> ret = String<_RET> (r1x) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = CHAR () ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			rbx = CHAR (i) ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR ((rbx << 8) | CHAR (i & STRU8 (0XFF))) ;
			rax = 2 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR ((rbx << 8) | CHAR (i & STRU8 (0XFF))) ;
			rax = 0 ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 18) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 12) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 6) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX (rbx & CHAR (0X3F))]) ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	for (FOR_ONCE_DO) {
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR (rbx << 16) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 18) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 12) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[64]) ;
			ret[iw++] = _RET (M_BASE64.P1[64]) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR (rbx << 8) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 18) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 12) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 6) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[64]) ;
		}
		if SWITCH_CASE (ifa) {
			_DEBUG_ASSERT_ (rax == 0) ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

template <class _ARG1>
inline String<STRU8> _PARSEBASE64U8S_ (const String<_ARG1> &src) {
	static constexpr auto M_BASE64 = PACK<INDEX[96]> ({
		-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,+0 ,64 ,-1 ,
		54 ,55 ,56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,64 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		-1 ,+1 ,+2 ,+3 ,+4 ,+5 ,+6 ,+7 ,+8 ,+9 ,10 ,11 ,12 ,13 ,14 ,15 ,
		16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24 ,25 ,26 ,-1 ,-1 ,-1 ,-1 ,53 ,
		-1 ,27 ,28 ,29 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,40 ,41 ,
		42 ,43 ,44 ,45 ,46 ,47 ,48 ,49 ,50 ,51 ,52 ,-1 ,-1 ,-1 ,-1 ,-1}) ;
	const auto r1x = (src.length () + 3) / 4 * 3 ;
	String<STRU8> ret = String<STRU8> (r1x) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = CHAR () ;
	for (auto &&i : src) {
		if (rax == VAR_NONE)
			discard ;
		const auto r2x = _SWITCH_ (
			((i & STRU8 (0X80)) == 0) ? (M_BASE64.P1[LENGTH (i) - 32]) :
			VAR_NONE) ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 0))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR (r2x & 63) ;
			rax = 1 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR ((rbx << 6) | CHAR (r2x & 63)) ;
			rax = 2 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 2))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR ((rbx << 6) | CHAR (r2x & 63)) ;
			rax = 3 ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 3))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR ((rbx << 6) | CHAR (r2x & 63)) ;
			rax = 0 ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 (rbx & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (ifa) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	for (FOR_ONCE_DO) {
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR (rbx << 18) ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR (rbx << 12) ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (ifa) {
			if (!(rax == 3))
				discard ;
			rbx = CHAR (rbx << 6) ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 (rbx & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (ifa) {
			_DEBUG_ASSERT_ (rax == 0) ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

template <class _ARG1>
inline PACK<WORD ,CHAR> _PARSEIPV4S_ (const String<_ARG1> &stri) {
	PACK<WORD ,CHAR> ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	auto rax = _ARG1 () ;
	auto rbx = VAR () ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	const auto r1x = BYTE (rbx) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('.')) ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	const auto r2x = BYTE (rbx) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('.')) ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	const auto r3x = BYTE (rbx) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('.')) ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	const auto r4x = BYTE (rbx) ;
	const auto r5x = PACK<BYTE[4]> {r1x ,r2x ,r3x ,r4x} ;
	ret.P2 = _CAST_<EndianBytes<CHAR>> (r5x) ;
	ret.P1 = 0 ;
	ris.copy () >> rax ;
	for (FOR_ONCE_DO) {
		if (rax != _ARG1 (':'))
			discard ;
		ris >> rax ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 65536) ;
		ret.P1 = WORD (rbx) ;
	}
	ris >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDIPV4S_ (const PACK<WORD ,CHAR> &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (63) ;
	auto &r1 = _CAST_<EndianBytes<CHAR>> (stru.P2) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << VAR (r1[0]) ;
	wos << _RET ('.') ;
	wos << VAR (r1[1]) ;
	wos << _RET ('.') ;
	wos << VAR (r1[2]) ;
	wos << _RET ('.') ;
	wos << VAR (r1[3]) ;
	for (FOR_ONCE_DO) {
		if (stru.P1 == 0)
			discard ;
		wos << _RET (':') ;
		wos << VAR (stru.P1) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

#ifdef __CSC_EXT__
inline imports DEF<ARRAY8<VAR32> (const std::chrono::system_clock::time_point &src)> _LOCALE_MAKE_TIMEMETRIC_ ;
inline imports DEF<std::chrono::system_clock::time_point (const ARRAY8<VAR32> &src)> _LOCALE_MAKE_TIMEPOINT_ ;

template <class _ARG1>
inline std::chrono::system_clock::time_point _PARSEDATES_ (const String<_ARG1> &stri) {
	const auto r1x = _CALL_ ([&] () {
		ARRAY8<VAR32> ret ;
		ret.fill (0) ;
		auto ris = TextReader<STR> (stri.raw ()) ;
		auto rax = STR () ;
		auto rbx = VAR () ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 1900) ;
		ret[0] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR ('-')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 1 && rbx <= 12) ;
		ret[1] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR ('-')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 1 && rbx <= 31) ;
		ret[2] = VAR32 (rbx) ;
		ris >> _EOS_ ;
		return std::move (ret) ;
	}) ;
	return _LOCALE_MAKE_TIMEPOINT_ (r1x) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDDATES_ (const std::chrono::system_clock::time_point &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<STR> ret = String<STR> (31) ;
	auto wos = TextWriter<STR> (ret.raw ()) ;
	const auto r1x = _LOCALE_MAKE_TIMEMETRIC_ (stru) ;
	wos << r1x[0] ;
	wos << STR ('-') ;
	if (r1x[1] < 10)
		wos << STR ('0') ;
	wos << r1x[1] ;
	wos << STR ('-') ;
	if (r1x[2] < 10)
		wos << STR ('0') ;
	wos << r1x[2] ;
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline std::chrono::system_clock::time_point _PARSEHOURS_ (const String<_ARG1> &stri) {
	const auto r1x = _CALL_ ([&] () {
		ARRAY8<VAR32> ret ;
		ret.fill (0) ;
		auto ris = TextReader<STR> (stri.raw ()) ;
		auto rax = STR () ;
		auto rbx = VAR () ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx <= 23) ;
		ret[5] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR (':')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx <= 59) ;
		ret[6] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR (':')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx <= 60) ;
		ret[7] = VAR32 (rbx) ;
		ris >> _EOS_ ;
		return std::move (ret) ;
	}) ;
	return _LOCALE_MAKE_TIMEPOINT_ (r1x) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDHOURS_ (const std::chrono::system_clock::time_point &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<STR> ret = String<STR> (31) ;
	auto wos = TextWriter<STR> (ret.raw ()) ;
	const auto r1x = _LOCALE_MAKE_TIMEMETRIC_ (stru) ;
	if (r1x[5] < 10)
		wos << STR ('0') ;
	wos << r1x[5] ;
	wos << STR (':') ;
	if (r1x[6] < 10)
		wos << STR ('0') ;
	wos << r1x[6] ;
	wos << STR (':') ;
	if (r1x[7] < 10)
		wos << STR ('0') ;
	wos << r1x[7] ;
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline std::chrono::system_clock::time_point _PARSETIMES_ (const String<_ARG1> &stri) {
	const auto r1x = _CALL_ ([&] () {
		ARRAY8<VAR32> ret ;
		ret.fill (0) ;
		auto ris = TextReader<STR> (stri.raw ()) ;
		auto rax = STR () ;
		auto rbx = VAR () ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 1900) ;
		ret[0] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR ('-')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 1 && rbx <= 12) ;
		ret[1] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR ('-')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 1 && rbx <= 31) ;
		ret[2] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR (' ')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx <= 23) ;
		ret[5] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR (':')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx <= 59) ;
		ret[6] = VAR32 (rbx) ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == STR (':')) ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx <= 60) ;
		ret[7] = VAR32 (rbx) ;
		ris >> _EOS_ ;
		return std::move (ret) ;
	}) ;
	return _LOCALE_MAKE_TIMEPOINT_ (r1x) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDTIMES_ (const std::chrono::system_clock::time_point &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<STR> ret = String<STR> (63) ;
	auto wos = TextWriter<STR> (ret.raw ()) ;
	const auto r1x = _LOCALE_MAKE_TIMEMETRIC_ (stru) ;
	wos << r1x[0] ;
	wos << STR ('-') ;
	if (r1x[1] < 10)
		wos << STR ('0') ;
	wos << r1x[1] ;
	wos << STR ('-') ;
	if (r1x[2] < 10)
		wos << STR ('0') ;
	wos << r1x[2] ;
	wos << STR (' ') ;
	if (r1x[5] < 10)
		wos << STR ('0') ;
	wos << r1x[5] ;
	wos << STR (':') ;
	if (r1x[6] < 10)
		wos << STR ('0') ;
	wos << r1x[6] ;
	wos << STR (':') ;
	if (r1x[7] < 10)
		wos << STR ('0') ;
	wos << r1x[7] ;
	wos << _EOS_ ;
	return std::move (ret) ;
}
#endif
} ;
} ;