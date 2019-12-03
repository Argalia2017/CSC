#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"

namespace CSC {
inline namespace STRING {
inline String<STRU16> _U8STOU16S_ (const String<STRU8> &val) ;
inline String<STRU8> _U16STOU8S_ (const String<STRU16> &val) ;
inline String<STRU32> _U8STOU32S_ (const String<STRU8> &val) ;
inline String<STRU8> _U32STOU8S_ (const String<STRU32> &val) ;
inline String<STRU32> _U16STOU32S_ (const String<STRU16> &val) ;
inline String<STRU16> _U32STOU16S_ (const String<STRU32> &val) ;
inline String<STRU8> _WSTOU8S_ (const String<STRW> &val) ;
inline String<STRW> _U8STOWS_ (const String<STRU8> &val) ;
inline String<STRU16> _WSTOU16S_ (const String<STRW> &val) ;
inline String<STRW> _U16STOWS_ (const String<STRU16> &val) ;
inline String<STRU32> _WSTOU32S_ (const String<STRW> &val) ;
inline String<STRW> _U32STOWS_ (const String<STRU32> &val) ;
inline String<STRW> _GBKSTOWS_ (const String<STRA> &val) ;
inline String<STRA> _WSTOGBKS_ (const String<STRW> &val) ;
inline String<STRU8> _ASTOU8S_ (const String<STRA> &val) ;
inline String<STRA> _U8STOAS_ (const String<STRU8> &val) ;
inline String<STRU16> _ASTOU16S_ (const String<STRA> &val) ;
inline String<STRA> _U16STOAS_ (const String<STRU16> &val) ;
inline String<STRU32> _ASTOU32S_ (const String<STRA> &val) ;
inline String<STRA> _U32STOAS_ (const String<STRU32> &val) ;

inline imports DEF<String<STRW> (const String<STRA> &val)> _ASTOWS_ ;
inline imports DEF<String<STRA> (const String<STRW> &val)> _WSTOAS_ ;
} ;

namespace U {
template <class ,class>
struct STRING_TRAITS ;

template <>
struct STRING_TRAITS<STRU8 ,STRU8> {
	inline static String<STRU8> invoke (const String<STRU8> &val) {
		return _COPY_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU16 ,STRU16> {
	inline static String<STRU16> invoke (const String<STRU16> &val) {
		return _COPY_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU32 ,STRU32> {
	inline static String<STRU32> invoke (const String<STRU32> &val) {
		return _COPY_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRA ,STRA> {
	inline static String<STRA> invoke (const String<STRA> &val) {
		return _COPY_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRW ,STRW> {
	inline static String<STRW> invoke (const String<STRW> &val) {
		return _COPY_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU8 ,STRU16> {
	inline static String<STRU8> invoke (const String<STRU16> &val) {
		return _U16STOU8S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU16 ,STRU8> {
	inline static String<STRU16> invoke (const String<STRU8> &val) {
		return _U8STOU16S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU8 ,STRU32> {
	inline static String<STRU8> invoke (const String<STRU32> &val) {
		return _U32STOU8S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU32 ,STRU8> {
	inline static String<STRU32> invoke (const String<STRU8> &val) {
		return _U8STOU32S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU16 ,STRU32> {
	inline static String<STRU16> invoke (const String<STRU32> &val) {
		return _U32STOU16S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU32 ,STRU16> {
	inline static String<STRU32> invoke (const String<STRU16> &val) {
		return _U16STOU32S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU8 ,STRW> {
	inline static String<STRU8> invoke (const String<STRW> &val) {
		return _WSTOU8S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRW ,STRU8> {
	inline static String<STRW> invoke (const String<STRU8> &val) {
		return _U8STOWS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU16 ,STRW> {
	inline static String<STRU16> invoke (const String<STRW> &val) {
		return _WSTOU16S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRW ,STRU16> {
	inline static String<STRW> invoke (const String<STRU16> &val) {
		return _U16STOWS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU32 ,STRW> {
	inline static String<STRU32> invoke (const String<STRW> &val) {
		return _WSTOU32S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRW ,STRU32> {
	inline static String<STRW> invoke (const String<STRU32> &val) {
		return _U32STOWS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRW ,STRA> {
	inline static String<STRW> invoke (const String<STRA> &val) {
		return _ASTOWS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRA ,STRW> {
	inline static String<STRA> invoke (const String<STRW> &val) {
		return _WSTOAS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU8 ,STRA> {
	inline static String<STRU8> invoke (const String<STRA> &val) {
		return _ASTOU8S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRA ,STRU8> {
	inline static String<STRA> invoke (const String<STRU8> &val) {
		return _U8STOAS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU16 ,STRA> {
	inline static String<STRU16> invoke (const String<STRA> &val) {
		return _ASTOU16S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRA ,STRU16> {
	inline static String<STRA> invoke (const String<STRU16> &val) {
		return _U16STOAS_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRU32 ,STRA> {
	inline static String<STRU32> invoke (const String<STRA> &val) {
		return _ASTOU32S_ (val) ;
	}
} ;

template <>
struct STRING_TRAITS<STRA ,STRU32> {
	inline static String<STRA> invoke (const String<STRU32> &val) {
		return _U32STOAS_ (val) ;
	}
} ;

template <class _ARG1 ,class _ARG2>
using STRING_TRAITS_HELP = STRING_TRAITS<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2>> ;
} ;

inline namespace STRING {
inline String<STRU16> _U8STOU16S_ (const String<STRU8> &val) {
	String<STRU16> ret = String<STRU16> (val.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0X7F)))
				discard ;
			ret[iw++] = STRU16 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XDF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X1F)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XEF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X0F)) ;
			rax = 2 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XF7)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X07)) ;
			rax = 3 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFB)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X03)) ;
			rax = 4 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFD)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X01)) ;
			rax = 5 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax = 10 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax >= 2 && rax <= 5))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax-- ;
		}
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
		if (rax < 10)
			continue ;
		auto fbx = TRUE ;
		if SWITCH_CASE (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = STRU16 (rbx) ;
			rax = 0 ;
		}
		if SWITCH_CASE (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X0010FFFF)))
				discard ;
			rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
			ret[iw++] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
			rax = 0 ;
		}
		if SWITCH_CASE (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = STRU16 ('?') ;
			rax = 0 ;
		}
		if SWITCH_CASE (fbx) {
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

inline String<STRU8> _U16STOU8S_ (const String<STRU16> &val) {
	String<STRU8> ret = String<STRU8> (val.length () * 3) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X007F)))
				discard ;
			ret[iw++] = STRU8 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X07FF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)))
				discard ;
			rbx = STRU32 (i & STRU16 (0X03FF)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (fax) {
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
		if SWITCH_CASE (fax) {
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

inline String<STRU32> _U8STOU32S_ (const String<STRU8> &val) {
	_STATIC_WARNING_ ("note") ;
	/*
	*	1 bytes [0,0X7F] 0xxxxxxx
	*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
	*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
	*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	String<STRU32> ret = String<STRU32> (val.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0X7F)))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XDF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X1F)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XEF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X0F)) ;
			rax = 2 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XF7)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X07)) ;
			rax = 3 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFB)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X03)) ;
			rax = 4 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFD)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X01)) ;
			rax = 5 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax >= 2 && rax <= 5))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax-- ;
		}
		if SWITCH_CASE (fax) {
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

inline String<STRU8> _U32STOU8S_ (const String<STRU32> &val) {
	_STATIC_WARNING_ ("note") ;
	/*
	*	1 bytes [0,0X7F] 0xxxxxxx
	*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
	*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
	*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	String<STRU8> ret = String<STRU8> (val.length () * 6) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000007F)))
				discard ;
			ret[iw++] = STRU8 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X000007FF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X001FFFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if SWITCH_CASE (fax) {
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
		if SWITCH_CASE (fax) {
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
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU32> _U16STOU32S_ (const String<STRU16> &val) {
	_STATIC_WARNING_ ("note") ;
	/*
	*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
	*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
	*/
	String<STRU32> ret = String<STRU32> (val.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X07FF)))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)))
				discard ;
			rbx = STRU32 (i & STRU16 (0X03FF)) ;
			rax = 1 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			if (!(i >= STRU16 (0XDC00) && i <= STRU16 (0XDFFF)))
				discard ;
			rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		}
		if SWITCH_CASE (fax) {
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

inline String<STRU16> _U32STOU16S_ (const String<STRU32> &val) {
	_STATIC_WARNING_ ("note") ;
	/*
	*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
	*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
	*/
	String<STRU16> ret = String<STRU16> (val.length () * 2) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = STRU16 (i) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0010FFFF)))
				discard ;
			ret[iw++] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = STRU16 ('?') ;
		}
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> _WSTOU8S_ (const String<STRW> &val) {
	return U::STRING_TRAITS_HELP<STRU8 ,STRUW>::invoke (_CAST_<String<STRUW>> (val)) ;
}

inline String<STRW> _U8STOWS_ (const String<STRU8> &val) {
	String<STRUW> ret = U::STRING_TRAITS_HELP<STRUW ,STRU8>::invoke (val) ;
	return std::move (_CAST_<String<STRW>> (ret)) ;
}

inline String<STRU16> _WSTOU16S_ (const String<STRW> &val) {
	return U::STRING_TRAITS_HELP<STRU16 ,STRUW>::invoke (_CAST_<String<STRUW>> (val)) ;
}

inline String<STRW> _U16STOWS_ (const String<STRU16> &val) {
	String<STRUW> ret = U::STRING_TRAITS_HELP<STRUW ,STRU16>::invoke (val) ;
	return std::move (_CAST_<String<STRW>> (ret)) ;
}

inline String<STRU32> _WSTOU32S_ (const String<STRW> &val) {
	return U::STRING_TRAITS_HELP<STRU32 ,STRUW>::invoke (_CAST_<String<STRUW>> (val)) ;
}

inline String<STRW> _U32STOWS_ (const String<STRU32> &val) {
	String<STRUW> ret = U::STRING_TRAITS_HELP<STRUW ,STRU32>::invoke (val) ;
	return std::move (_CAST_<String<STRW>> (ret)) ;
}

inline String<STRU8> _ASTOU8S_ (const String<STRA> &val) {
	return _WSTOU8S_ (_ASTOWS_ (val)) ;
}

inline String<STRA> _U8STOAS_ (const String<STRU8> &val) {
	return _WSTOAS_ (_U8STOWS_ (val)) ;
}

inline String<STRU16> _ASTOU16S_ (const String<STRA> &val) {
	return _WSTOU16S_ (_ASTOWS_ (val)) ;
}

inline String<STRA> _U16STOAS_ (const String<STRU16> &val) {
	return _WSTOAS_ (_U16STOWS_ (val)) ;
}

inline String<STRU32> _ASTOU32S_ (const String<STRA> &val) {
	return _WSTOU32S_ (_ASTOWS_ (val)) ;
}

inline String<STRA> _U32STOAS_ (const String<STRU32> &val) {
	return _WSTOAS_ (_U32STOWS_ (val)) ;
}

inline String<STRU8> _UASTOU8S_ (const String<STRA> &val) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRU8> ret = std::move (_CAST_<String<STRUA>> (val)) ;
	for (auto &&i : ret) {
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
		(void) i ;
	}
	return std::move (ret) ;
}

inline String<STRU8> _UASTOU8S_ (String<STRA> &&val) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRU8> ret = std::move (_CAST_<String<STRUA>> (val)) ;
	for (auto &&i : ret) {
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
		(void) i ;
	}
	return std::move (ret) ;
}

inline String<STRA> _U8STOUAS_ (const String<STRU8> &val) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRUA> ret = std::move (val) ;
	for (auto &&i : ret) {
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
		(void) i ;
	}
	return std::move (_CAST_<String<STRA>> (ret)) ;
}

inline String<STRA> _U8STOUAS_ (String<STRU8> &&val) {
	_STATIC_ASSERT_ (std::is_same<STRUA ,STRU8>::value) ;
	String<STRUA> ret = std::move (val) ;
	for (auto &&i : ret) {
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
		(void) i ;
	}
	return std::move (_CAST_<String<STRA>> (ret)) ;
}

inline imports DEF<PhanBuffer<const DEF<STRUW[2]>> ()> _LOADUWSTOUGBKSTABLE_ ;

inline const HashSet<STRUW ,STRUW> &_inline_GBKSTOWS_TABLE_ () {
	return _CACHE_ ([] () {
		const auto r1x = _LOADUWSTOUGBKSTABLE_ () ;
		HashSet<STRUW ,STRUW> ret = HashSet<STRUW ,STRUW> (r1x.size ()) ;
		for (auto &&i : _RANGE_ (0 ,r1x.size ()))
			ret.add (r1x[i][1] ,r1x[i][0]) ;
		return std::move (ret) ;
	}) ;
}

inline String<STRW> _GBKSTOWS_ (const String<STRA> &val) {
	auto &r1y = _inline_GBKSTOWS_TABLE_ () ;
	auto &r2y = _CAST_<String<STRUA>> (val) ;
	String<STRW> ret = String<STRW> (r2y.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRUW () ;
	for (auto &&i : r2y) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			rbx = STRUW (i) ;
			rax = 10 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			rbx = STRUW ((rbx << 8) | STRUW (i)) ;
			rax = 11 ;
		}
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
		if (rax < 10)
			continue ;
		INDEX ix = r1y.find (rbx) ;
		auto fbx = TRUE ;
		if SWITCH_CASE (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(ix != VAR_NONE))
				discard ;
			ret[iw++] = STRW (r1y[ix].item) ;
			rax = 0 ;
		}
		if SWITCH_CASE (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			rax = 1 ;
		}
		if SWITCH_CASE (fbx) {
			if (!(rax == 11))
				discard ;
			if (!(ix != VAR_NONE))
				discard ;
			ret[iw++] = STRW (r1y[ix].item) ;
			rax = 0 ;
		}
		if SWITCH_CASE (fbx) {
			if (!(rax == 11))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			ret[iw++] = STRW ('?') ;
			rax = 0 ;
		}
		if SWITCH_CASE (fbx) {
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

inline const HashSet<STRUW ,STRUW> &_inline_WSTOGBKS_TABLE_ () {
	return _CACHE_ ([] () {
		const auto r1x = _LOADUWSTOUGBKSTABLE_ () ;
		HashSet<STRUW ,STRUW> ret = HashSet<STRUW ,STRUW> (r1x.size ()) ;
		for (auto &&i : _RANGE_ (0 ,r1x.size ()))
			ret.add (r1x[i][0] ,r1x[i][1]) ;
		return std::move (ret) ;
	}) ;
}

inline String<STRA> _WSTOGBKS_ (const String<STRW> &val) {
	auto &r1y = _inline_WSTOGBKS_TABLE_ () ;
	String<STRUA> ret = String<STRUA> (val.length () * 2) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		INDEX ix = r1y.find (STRUW (i)) ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			ret[iw++] = STRUA ('?') ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(r1y[ix].item <= STRUW (0X00FF)))
				discard ;
			ret[iw++] = STRUA (r1y[ix].item) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(r1y[ix].item <= STRUW (0XFFFF)))
				discard ;
			ret[iw++] = STRUA (r1y[ix].item >> 8) ;
			ret[iw++] = STRUA (r1y[ix].item) ;
		}
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (_CAST_<String<STRA>> (ret)) ;
}
} ;

inline namespace STRING {
template <class _ARG1>
inline BOOL _PARSEBOOLS_ (const String<_ARG1> &stri) {
	BOOL ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDBOOLS_ (const BOOL &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (7) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAR32 _PARSEVAR32S_ (const String<_ARG1> &stri) {
	VAR32 ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAR32S_ (const VAR32 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (63) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAR64 _PARSEVAR64S_ (const String<_ARG1> &stri) {
	VAR64 ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAR64S_ (const VAR64 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAR _PARSEVARS_ (const String<_ARG1> &stri) {
	VAR ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVARS_ (const VAR &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAL32 _PARSEVAL32S_ (const String<_ARG1> &stri) {
	VAL32 ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAL32S_ (const VAL32 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAL64 _PARSEVAL64S_ (const String<_ARG1> &stri) {
	VAL64 ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVAL64S_ (const VAL64 &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline VAL _PARSEVALS_ (const String<_ARG1> &stri) {
	VAL ret ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	ris >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDVALS_ (const VAL &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	String<_RET> ret = String<_RET> (127) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << stru << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline String<STR> _PARSESTRS_ (const String<_ARG1> &stri) {
	return U::STRING_TRAITS_HELP<STR ,_ARG1>::invoke (stri) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDSTRS_ (const String<STR> &stru) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return U::STRING_TRAITS_HELP<_RET ,STR>::invoke (stru) ;
}
} ;

template <class ITEM ,class SIZE>
template <class... _ARGS>
inline String<ITEM ,SIZE> String<ITEM ,SIZE>::make (const _ARGS &...initval) {
	using DEFAULT_LONGSTRING_SIZE = ARGC<8195> ;
	_STATIC_ASSERT_ (std::is_same<SIZE ,SAUTO>::value) ;
	String<ITEM> ret = String<ITEM> (DEFAULT_LONGSTRING_SIZE::value) ;
	auto wos = TextWriter<ITEM> (ret.raw ()) ;
	_PRINTS_ (wos ,initval...) ;
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

inline namespace STRING {
template <class _ARG1>
inline CHAR _PARSEHEX8S_ (const String<_ARG1> &stri) {
	CHAR ret = 0 ;
	auto ris = TextReader<_ARG1> (stri.raw ()) ;
	auto rax = _ARG1 () ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('&')) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG1 ('H')) ;
	for (auto &&i : _RANGE_ (0 ,8)) {
		ris >> rax ;
		const auto r1x = BOOL (rax >= _ARG1 ('0') && rax <= _ARG1 ('9')) ;
		const auto r2x = BOOL (rax >= _ARG1 ('A') && rax <= _ARG1 ('F')) ;
		_DYNAMIC_ASSERT_ (r1x || r2x) ;
		const auto r3x = _SWITCH_ (
			r1x ? _ARG1 ('0') :
			_ARG1 ('A' - 10)) ;
		ret = (ret << 4) | CHAR (rax - r3x) ;
		(void) i ;
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
	for (auto &&i : _RANGE_ (0 ,8)) {
		const auto r1x = CHAR (stru >> (28 - i * 4)) & CHAR (0X0F) ;
		const auto r2x = _SWITCH_ (
			(r1x < DATA (10)) ? _RET ('0') :
			_RET ('A') - 10) ;
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
	for (auto &&i : _RANGE_ (0 ,16)) {
		ris >> rax ;
		const auto r1x = BOOL (rax >= _ARG1 ('0') && rax <= _ARG1 ('9')) ;
		const auto r2x = BOOL (rax >= _ARG1 ('A') && rax <= _ARG1 ('F')) ;
		_DYNAMIC_ASSERT_ (r1x || r2x) ;
		const auto r3x = _SWITCH_ (
			r1x ? _ARG1 ('0') :
			_ARG1 ('A' - 10)) ;
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
	for (auto &&i : _RANGE_ (0 ,16)) {
		const auto r1x = DATA (stru >> (60 - i * 4)) & DATA (0X0F) ;
		const auto r2x = _SWITCH_ (
			(r1x < DATA (10)) ? _RET ('0') :
			_RET ('A') - 10) ;
		wos << _RET (r2x + r1x) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _RET = STR>
inline String<_RET> _BUILDBASE64U8S_ (const String<STRU8> &stru) {
	static constexpr auto M_BASE64 = PACK<STRU8[66]> ({
		'-' ,'A' ,'B' ,'C' ,'D' ,'E' ,'F' ,'G' ,
		'H' ,'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,
		'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,'W' ,
		'X' ,'Y' ,'Z' ,'a' ,'b' ,'c' ,'d' ,'e' ,
		'f' ,'g' ,'h' ,'i' ,'j' ,'k' ,'l' ,'m' ,
		'n' ,'o' ,'p' ,'q' ,'r' ,'s' ,'t' ,'u' ,
		'v' ,'w' ,'x' ,'y' ,'z' ,'_' ,'0' ,'1' ,
		'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,
		'.' ,':'}) ;
	const auto r1x = (stru.length () + 2) / 3 * 4 ;
	String<_RET> ret = String<_RET> (r1x) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = CHAR () ;
	for (auto &&i : stru) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			rbx = CHAR (i) ;
			rax = 1 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR ((rbx << 8) | CHAR (i & STRU8 (0XFF))) ;
			rax = 2 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR ((rbx << 8) | CHAR (i & STRU8 (0XFF))) ;
			rax = 0 ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 18) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 12) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 6) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX (rbx & CHAR (0X3F))]) ;
		}
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if SWITCH_CASE (TRUE) {
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR (rbx << 16) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 18) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 12) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[64]) ;
			ret[iw++] = _RET (M_BASE64.P1[64]) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR (rbx << 8) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 18) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 12) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[INDEX ((rbx >> 6) & CHAR (0X3F))]) ;
			ret[iw++] = _RET (M_BASE64.P1[64]) ;
		}
		if SWITCH_CASE (fax) {
			_DYNAMIC_ASSERT_ (rax == 0) ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

template <class _ARG1>
inline String<STRU8> _PARSEBASE64U8S_ (const String<_ARG1> &stri) {
	static constexpr auto M_BASE64 = PACK<INDEX[96]> ({
		-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,+0 ,64 ,-1 ,
		54 ,55 ,56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,64 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		-1 ,+1 ,+2 ,+3 ,+4 ,+5 ,+6 ,+7 ,+8 ,+9 ,10 ,11 ,12 ,13 ,14 ,15 ,
		16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24 ,25 ,26 ,-1 ,-1 ,-1 ,-1 ,53 ,
		-1 ,27 ,28 ,29 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,40 ,41 ,
		42 ,43 ,44 ,45 ,46 ,47 ,48 ,49 ,50 ,51 ,52 ,-1 ,-1 ,-1 ,-1 ,-1}) ;
	const auto r1x = (stri.length () + 3) / 4 * 3 ;
	String<STRU8> ret = String<STRU8> (r1x) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = CHAR () ;
	for (auto &&i : stri) {
		if (rax == VAR_NONE)
			continue ;
		const auto r2x = _SWITCH_ (
			((i & STRU8 (0X80)) == 0) ? M_BASE64.P1[LENGTH (i) - 32] :
			VAR_NONE) ;
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 0))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR (r2x & 63) ;
			rax = 1 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR ((rbx << 6) | CHAR (r2x & 63)) ;
			rax = 2 ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 2))
				discard ;
			if (!(r2x >= 0))
				discard ;
			rbx = CHAR ((rbx << 6) | CHAR (r2x & 63)) ;
			rax = 3 ;
		}
		if SWITCH_CASE (fax) {
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
		if SWITCH_CASE (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if SWITCH_CASE (TRUE) {
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR (rbx << 18) ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR (rbx << 12) ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (fax) {
			if (!(rax == 3))
				discard ;
			rbx = CHAR (rbx << 6) ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 (rbx & CHAR (0XFF)) ;
		}
		if SWITCH_CASE (fax) {
			_DYNAMIC_ASSERT_ (rax == 0) ;
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
	const auto r5x = PACK<BYTE[_SIZEOF_ (CHAR)]> {r1x ,r2x ,r3x ,r4x} ;
	ByteReader (PhanBuffer<const BYTE>::make (r5x.P1)) >> ret.P2 ;
	ret.P1 = 0 ;
	ris.copy () >> rax ;
	if SWITCH_CASE (TRUE) {
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
	auto rax = PACK<BYTE[_SIZEOF_ (CHAR)]> () ;
	ByteWriter (PhanBuffer<BYTE>::make (rax.P1)) << stru.P2 ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << VAR (rax.P1[0]) ;
	wos << _RET ('.') ;
	wos << VAR (rax.P1[1]) ;
	wos << _RET ('.') ;
	wos << VAR (rax.P1[2]) ;
	wos << _RET ('.') ;
	wos << VAR (rax.P1[3]) ;
	if SWITCH_CASE (TRUE) {
		if (stru.P1 == 0)
			discard ;
		wos << _RET (':') ;
		wos << VAR (stru.P1) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

#ifdef __CSC_EXTEND__
inline imports DEF<ARRAY8<VAR32> (const std::chrono::system_clock::time_point &val)> _LOCALE_MAKE_TIMEMETRIC_ ;
inline imports DEF<std::chrono::system_clock::time_point (const ARRAY8<VAR32> &val)> _LOCALE_MAKE_TIMEPOINT_ ;

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