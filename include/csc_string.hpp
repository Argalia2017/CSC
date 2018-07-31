#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"

namespace CSC {
namespace U {
template <class ,class>
struct OPERATOR_CONVERT ;

template <class TYPE>
struct OPERATOR_CONVERT<TYPE ,void> {
	template <class _ARG>
	inline static TYPE invoke (const _ARG &arg) {
		return OPERATOR_CONVERT<TYPE ,_ARG>::invoke (arg) ;
	}
} ;
} ;

template <class TYPE>
using OPERATOR_CONVERT_TYPE = U::OPERATOR_CONVERT<TYPE ,void> ;

inline namespace STRING {
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

inline import DEF<String<STRW> (const String<STRA> &src)> _ASTOWS_ ;
inline import DEF<String<STRA> (const String<STRW> &src)> _WSTOAS_ ;
} ;

namespace U {
template <>
struct OPERATOR_CONVERT<String<STRU8> ,String<STRU8>> {
	inline static String<STRU8> invoke (const String<STRU8> &arg) {
		return arg ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU16> ,String<STRU16>> {
	inline static String<STRU16> invoke (const String<STRU16> &arg) {
		return arg ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU32> ,String<STRU32>> {
	inline static String<STRU32> invoke (const String<STRU32> &arg) {
		return arg ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRA> ,String<STRA>> {
	inline static String<STRA> invoke (const String<STRA> &arg) {
		return arg ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRW> ,String<STRW>> {
	inline static String<STRW> invoke (const String<STRW> &arg) {
		return arg ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU8> ,String<STRU16>> {
	inline static String<STRU8> invoke (const String<STRU16> &arg) {
		return _U16STOU8S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU16> ,String<STRU8>> {
	inline static String<STRU16> invoke (const String<STRU8> &arg) {
		return _U8STOU16S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU8> ,String<STRU32>> {
	inline static String<STRU8> invoke (const String<STRU32> &arg) {
		return _U32STOU8S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU32> ,String<STRU8>> {
	inline static String<STRU32> invoke (const String<STRU8> &arg) {
		return _U8STOU32S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU16> ,String<STRU32>> {
	inline static String<STRU16> invoke (const String<STRU32> &arg) {
		return _U32STOU16S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU32> ,String<STRU16>> {
	inline static String<STRU32> invoke (const String<STRU16> &arg) {
		return _U16STOU32S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU8> ,String<STRW>> {
	inline static String<STRU8> invoke (const String<STRW> &arg) {
		return _WSTOU8S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRW> ,String<STRU8>> {
	inline static String<STRW> invoke (const String<STRU8> &arg) {
		return _U8STOWS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU16> ,String<STRW>> {
	inline static String<STRU16> invoke (const String<STRW> &arg) {
		return _WSTOU16S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRW> ,String<STRU16>> {
	inline static String<STRW> invoke (const String<STRU16> &arg) {
		return _U16STOWS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU32> ,String<STRW>> {
	inline static String<STRU32> invoke (const String<STRW> &arg) {
		return _WSTOU32S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRW> ,String<STRU32>> {
	inline static String<STRW> invoke (const String<STRU32> &arg) {
		return _U32STOWS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRW> ,String<STRA>> {
	inline static String<STRW> invoke (const String<STRA> &arg) {
		return _ASTOWS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRA> ,String<STRW>> {
	inline static String<STRA> invoke (const String<STRW> &arg) {
		return _WSTOAS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU8> ,String<STRA>> {
	inline static String<STRU8> invoke (const String<STRA> &arg) {
		return _ASTOU8S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRA> ,String<STRU8>> {
	inline static String<STRA> invoke (const String<STRU8> &arg) {
		return _U8STOAS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU16> ,String<STRA>> {
	inline static String<STRU16> invoke (const String<STRA> &arg) {
		return _ASTOU16S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRA> ,String<STRU16>> {
	inline static String<STRA> invoke (const String<STRU16> &arg) {
		return _U16STOAS_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRU32> ,String<STRA>> {
	inline static String<STRU32> invoke (const String<STRA> &arg) {
		return _ASTOU32S_ (arg) ;
	}
} ;

template <>
struct OPERATOR_CONVERT<String<STRA> ,String<STRU32>> {
	inline static String<STRA> invoke (const String<STRU32> &arg) {
		return _U32STOAS_ (arg) ;
	}
} ;
} ;

inline namespace STRING {
inline String<STRU16> _U8STOU16S_ (const String<STRU8> &src) {
	String<STRU16> ret = String<STRU16> (src.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &i : src) {
		if (rax == 0 && i <= STRU8 (0X7F)) {
			ret[iw++] = i ;
		} else if (rax > 1 && i <= STRU8 (0XBF)) {
			rbx = (rbx << 6) | (i & STRU8 (0X3F)) ;
			rax-- ;
		} else if (rax == 1 && i <= STRU8 (0XBF)) {
			rbx = (rbx << 6) | (i & STRU8 (0X3F)) ;
			rax = 0 ;
			if (rbx <= STRU32 (0X0000FFFF)) {
				ret[iw++] = STRU16 (rbx) ;
			} else if (rbx <= STRU32 (0X0010FFFF)) {
				rbx -= STRU32 (0X00010000) ;
				ret[iw++] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
				ret[iw++] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
			} else if (i <= STRU32 (0X7FFFFFFF)) {
				//@warn:undefined
				ret[iw++] = STRU16 ('?') ;
			} else {
				ret = String<STRU16> () ;
				break ;
			}
		} else if (rax == 0 && i <= STRU8 (0XDF)) {
			rbx = i & STRU8 (0X1F) ;
			rax = 1 ;
		} else if (rax == 0 && i <= STRU8 (0XEF)) {
			rbx = i & STRU8 (0X0F) ;
			rax = 2 ;
		} else if (rax == 0 && i <= STRU8 (0XF7)) {
			rbx = i & STRU8 (0X07) ;
			rax = 3 ;
		} else if (rax == 0 && i <= STRU8 (0XFB)) {
			rbx = i & STRU8 (0X03) ;
			rax = 4 ;
		} else if (rax == 0 && i <= STRU8 (0XFD)) {
			rbx = i & STRU8 (0X01) ;
			rax = 5 ;
		} else {
			ret = String<STRU16> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> _U16STOU8S_ (const String<STRU16> &src) {
	String<STRU8> ret = String<STRU8> (src.length () * 3) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &i : src) {
		if (rax == 0 && i <= STRU16 (0X007F)) {
			ret[iw++] = STRU8 (i) ;
		} else if (rax == 0 && i <= STRU16 (0X07FF)) {
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else if (rax == 0 && i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)) {
			rbx = i & STRU16 (0X03FF) ;
			rax = 1 ;
		} else if (rax == 1 && i >= STRU16 (0XDC00) && i <= STRU16 (0XDFFF)) {
			rbx = ((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000) ;
			ret[iw++] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
			ret[iw++] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
			rax = 0 ;
		} else if (rax == 0) {
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else {
			ret = String<STRU8> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU32> _U8STOU32S_ (const String<STRU8> &src) {
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
	for (auto &i : src) {
		if (rax == 0 && i <= STRU8 (0X7F)) {
			ret[iw++] = i ;
		} else if (rax > 0 && i <= STRU8 (0XBF)) {
			rbx = (rbx << 6) | (i & STRU8 (0X3F)) ;
			rax-- ;
			if (rax == 0)
				ret[iw++] = rbx ;
		} else if (rax == 0 && i <= STRU8 (0XDF)) {
			rbx = i & STRU8 (0X1F) ;
			rax = 1 ;
		} else if (rax == 0 && i <= STRU8 (0XEF)) {
			rbx = i & STRU8 (0X0F) ;
			rax = 2 ;
		} else if (rax == 0 && i <= STRU8 (0XF7)) {
			rbx = i & STRU8 (0X07) ;
			rax = 3 ;
		} else if (rax == 0 && i <= STRU8 (0XFB)) {
			rbx = i & STRU8 (0X03) ;
			rax = 4 ;
		} else if (rax == 0 && i <= STRU8 (0XFD)) {
			rbx = i & STRU8 (0X01) ;
			rax = 5 ;
		} else {
			ret = String<STRU32> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> _U32STOU8S_ (const String<STRU32> &src) {
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
	for (auto &i : src) {
		if (i <= STRU32 (0X0000007F)) {
			ret[iw++] = STRU8 (i) ;
		} else if (i <= STRU32 (0X000007FF)) {
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else if (i <= STRU32 (0X0000FFFF)) {
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else if (i <= STRU32 (0X001FFFFF)) {
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else if (i <= STRU32 (0X03FFFFFF)) {
			ret[iw++] = (STRU8 (i >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else if (i <= STRU32 (0X7FFFFFFF)) {
			ret[iw++] = (STRU8 (i >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
			ret[iw++] = (STRU8 (i >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		} else {
			ret = String<STRU8> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU32> _U16STOU32S_ (const String<STRU16> &src) {
	/*
	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
	*/
	String<STRU32> ret = String<STRU32> (src.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &i : src) {
		if (rax == 0 && i <= STRU16 (0X07FF)) {
			ret[iw++] = STRU32 (i) ;
		} else if (rax == 0 && i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)) {
			rbx = i & STRU16 (0X03FF) ;
			rax = 1 ;
		} else if (rax == 1 && i >= STRU16 (0XDC00) && i <= STRU16 (0XDFFF)) {
			rbx = ((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		} else if (rax == 0) {
			ret[iw++] = STRU32 (i) ;
		} else {
			ret = String<STRU32> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU16> _U32STOU16S_ (const String<STRU32> &src) {
	/*
	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
	*/
	String<STRU16> ret = String<STRU16> (src.length () * 2) ;
	INDEX iw = 0 ;
	for (auto &i : src) {
		if (i <= STRU32 (0X0000FFFF)) {
			ret[iw++] = STRU16 (i) ;
		} else if (i <= STRU32 (0X0010FFFF)) {
			ret[iw++] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
		} else if (i <= STRU32 (0X7FFFFFFF)) {
			//@warn:undefined
			ret[iw++] = STRU16 ('?') ;
		} else {
			ret = String<STRU16> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRUA> &_ASTOUAS_ (String<STRA> &src) {
	return _CAST_<String<STRUA>> (src) ;
}

inline const String<STRUA> &_ASTOUAS_ (const String<STRA> &src) {
	return _CAST_<String<STRUA>> (src) ;
}

inline String<STRA> &_UASTOAS_ (String<STRUA> &src) {
	return _CAST_<String<STRA>> (src) ;
}

inline const String<STRA> &_UASTOAS_ (const String<STRUA> &src) {
	return _CAST_<String<STRA>> (src) ;
}

inline String<STRUW> &_WSTOUWS_ (String<STRW> &src) {
	return _CAST_<String<STRUW>> (src) ;
}

inline const String<STRUW> &_WSTOUWS_ (const String<STRW> &src) {
	return _CAST_<String<STRUW>> (src) ;
}

inline String<STRW> &_UWSTOWS_ (String<STRUW> &src) {
	return _CAST_<String<STRW>> (src) ;
}

inline const String<STRW> &_UWSTOWS_ (const String<STRUW> &src) {
	return _CAST_<String<STRW>> (src) ;
}

inline String<STRU8> _WSTOU8S_ (const String<STRW> &src) {
	return OPERATOR_CONVERT_TYPE<String<STRU8>>::invoke (_WSTOUWS_ (src)) ;
}

inline String<STRW> _U8STOWS_ (const String<STRU8> &src) {
	return _UWSTOWS_ (OPERATOR_CONVERT_TYPE<String<STRUW>>::invoke (src)) ;
}

inline String<STRU16> _WSTOU16S_ (const String<STRW> &src) {
	return OPERATOR_CONVERT_TYPE<String<STRU16>>::invoke (_WSTOUWS_ (src)) ;
}

inline String<STRW> _U16STOWS_ (const String<STRU16> &src) {
	return _UWSTOWS_ (OPERATOR_CONVERT_TYPE<String<STRUW>>::invoke (src)) ;
}

inline String<STRU32> _WSTOU32S_ (const String<STRW> &src) {
	return OPERATOR_CONVERT_TYPE<String<STRU32>>::invoke (_WSTOUWS_ (src)) ;
}

inline String<STRW> _U32STOWS_ (const String<STRU32> &src) {
	return _UWSTOWS_ (OPERATOR_CONVERT_TYPE<String<STRUW>>::invoke (src)) ;
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

inline import DEF<PhanBuffer<const PACK<STRW ,STRW>> ()> _LOADUWSTOGBKSTABLE_ ;

inline String<STRW> _GBKSTOWS_ (const String<STRA> &src) {
	static const HashSet<STRW ,STRW> mCVTSet = _CALL_ ([&] () {
		const auto r1x = _LOADUWSTOGBKSTABLE_ () ;
		HashSet<STRW ,STRW> ret = HashSet<STRW ,STRW> (r1x.size ()) ;
		for (INDEX i = 0 ; i < r1x.size () ; i++)
			ret.add (r1x[i].P2 ,r1x[i].P1) ;
		return std::move (ret) ;
	}) ;
	String<STRW> ret = String<STRW> (src.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRW () ;
	for (auto &i : _ASTOUAS_ (src)) {
		if (rax == 0) {
			rbx = STRW (i) ;
			INDEX ix = mCVTSet.find (rbx) ;
			if (ix != VAR_NONE) {
				ret[iw++] = mCVTSet[ix].item ;
			} else {
				rax = 1 ;
			}
		} else if (rax == 1) {
			rbx = (rbx << 8) | STRW (i) ;
			INDEX ix = mCVTSet.find (rbx) ;
			//@warn:undefined
			ret[iw++] = ix != VAR_NONE ? mCVTSet[ix].item : STRW ('?') ;
			rax = 0 ;
		} else {
			ret = String<STRW> () ;
			break ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRA> _WSTOGBKS_ (const String<STRW> &src) {
	static const HashSet<STRW ,STRW> mCVTSet = _CALL_ ([&] () {
		const auto r1x = _LOADUWSTOGBKSTABLE_ () ;
		HashSet<STRW ,STRW> ret = HashSet<STRW ,STRW> (r1x.size ()) ;
		for (INDEX i = 0 ; i < r1x.size () ; i++)
			ret.add (r1x[i].P1 ,r1x[i].P2) ;
		return std::move (ret) ;
	}) ;
	String<STRA> ret = String<STRA> (src.length () * 2) ;
	auto &r1 = _ASTOUAS_ (ret) ;
	INDEX iw = 0 ;
	for (auto &i : src) {
		INDEX ix = mCVTSet.find (i) ;
		if (ix == VAR_NONE) {
			//@warn:undefined
			r1[iw++] = STRUA ('?') ;
		} else if (mCVTSet[ix].item <= STRW (0X00FF)) {
			r1[iw++] = STRUA (mCVTSet[ix].item) ;
		} else {
			r1[iw++] = STRUA (mCVTSet[ix].item >> 8) ;
			r1[iw++] = STRUA (mCVTSet[ix].item) ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return std::move (ret) ;
}

template <class _ARG>
inline String<STR> _PARSESTRS_ (const String<_ARG> &arg) {
	return OPERATOR_CONVERT_TYPE<String<STR>>::invoke (arg) ;
}

template <class _RET>
inline String<_RET> _BUILDSTRS_ (const String<STR> &arg) {
	return OPERATOR_CONVERT_TYPE<String<_RET>>::invoke (arg) ;
}
} ;

#ifdef __CSC_DEPRECATED__
class RegexMatcher {
private:
	RegexMatcher () = default ;

	explicit RegexMatcher (const String<STRU8> &expr) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;
#endif

inline namespace STRING {
template <class _ARG>
inline BOOL _PARSEBOOLS_ (const String<_ARG> &arg) {
	BOOL ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDBOOLS_ (const BOOL &arg) {
	String<_RET> ret = String<_RET> (7) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline VAR32 _PARSEVAR32S_ (const String<_ARG> &arg) {
	VAR32 ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDVAR32S_ (const VAR32 &arg) {
	String<_RET> ret = String<_RET> (63) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline VAR64 _PARSEVAR64S_ (const String<_ARG> &arg) {
	VAR64 ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDVAR64S_ (const VAR64 &arg) {
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline VAR _PARSEVARS_ (const String<_ARG> &arg) {
	VAR ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDVARS_ (const VAR &arg) {
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline VAL32 _PARSEVAL32S_ (const String<_ARG> &arg) {
	VAL32 ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDVAL32S_ (const VAL32 &arg) {
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline VAL64 _PARSEVAL64S_ (const String<_ARG> &arg) {
	VAL64 ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDVAL64S_ (const VAL64 &arg) {
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline VAL _PARSEVALS_ (const String<_ARG> &arg) {
	VAL ret ;
	TextReader<_ARG> (arg.raw ()) >> ret >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDVALS_ (const VAL &arg) {
	String<_RET> ret = String<_RET> (127) ;
	TextWriter<_RET> (ret.raw ()) << arg << _EOS_ ;
	return std::move (ret) ;
}
} ;

namespace U {
template <class _ARG>
struct OPERATOR_CONVERT<BOOL ,String<_ARG>> {
	inline static BOOL invoke (const String<_ARG> &arg) {
		return _PARSEBOOLS_ (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<String<_ARG> ,BOOL> {
	inline static String<_ARG> invoke (const BOOL &arg) {
		return _BUILDBOOLS_<_ARG> (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<VAR32 ,String<_ARG>> {
	inline static VAR32 invoke (const String<_ARG> &arg) {
		return _PARSEVAR32S_ (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<String<_ARG> ,VAR32> {
	inline static String<_ARG> invoke (const VAR32 &arg) {
		return _BUILDVAR32S_<_ARG> (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<VAR64 ,String<_ARG>> {
	inline static VAR64 invoke (const String<_ARG> &arg) {
		return _PARSEVAR64S_ (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<String<_ARG> ,VAR64> {
	inline static String<_ARG> invoke (const VAR64 &arg) {
		return _BUILDVAR64S_<_ARG> (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<VAL32 ,String<_ARG>> {
	inline static VAL32 invoke (const String<_ARG> &arg) {
		return _PARSEVAL32S_ (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<String<_ARG> ,VAL32> {
	inline static String<_ARG> invoke (const VAL32 &arg) {
		return _BUILDVAL32S_<_ARG> (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<VAL64 ,String<_ARG>> {
	inline static VAL64 invoke (const String<_ARG> &arg) {
		return _PARSEVAL64S_ (arg) ;
	}
} ;

template <class _ARG>
struct OPERATOR_CONVERT<String<_ARG> ,VAL64> {
	inline static String<_ARG> invoke (const VAL64 &arg) {
		return _BUILDVAL64S_<_ARG> (arg) ;
	}
} ;
} ;

inline namespace STRING {
template <class _RET ,class... _ARGS>
inline String<_RET> _BUILDARGSS_ (const _ARGS &...args) {
	String<_RET> ret = String<_RET> (LONGSTRING_SIZE::value) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << StreamBinder<const _ARGS...> (args...) ;
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline CHAR _PARSEHEX8S_ (const String<_ARG> &arg) {
	CHAR ret = 0 ;
	auto ris = TextReader<_ARG> (arg.raw ()) ;
	auto rax = _ARG () ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('&')) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('H')) ;
	for (INDEX i = 0 ; i < 8 ; i++) {
		ris >> rax ;
		_DYNAMIC_ASSERT_ ((rax >= _ARG ('0') && rax <= _ARG ('9')) || (rax >= _ARG ('A') && rax <= _ARG ('F'))) ;
		const auto r1x = rax >= _ARG ('0') && rax <= _ARG ('9') ? _ARG ('0') : _ARG ('A' - 10) ;
		ret = (ret << 4) | DATA (rax - r1x) ;
	}
	ris >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDHEX8S_ (const CHAR &arg) {
	String<_RET> ret = String<_RET> (15) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << _RET ('&') ;
	wos << _RET ('H') ;
	for (INDEX i = 0 ; i < 8 ; i++) {
		const auto r1x = CHAR (arg >> (28 - i * 4)) & CHAR (0X0F) ;
		wos << _RET ((r1x < CHAR (10) ? _RET ('0') : _RET ('A') - 10) + r1x) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline DATA _PARSEHEX16S_ (const String<_ARG> &arg) {
	DATA ret = 0 ;
	auto ris = TextReader<_ARG> (arg.raw ()) ;
	auto rax = _ARG () ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('&')) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('H')) ;
	for (INDEX i = 0 ; i < 16 ; i++) {
		ris >> rax ;
		_DYNAMIC_ASSERT_ ((rax >= _ARG ('0') && rax <= _ARG ('9')) || (rax >= _ARG ('A') && rax <= _ARG ('F'))) ;
		const auto r1x = rax >= _ARG ('0') && rax <= _ARG ('9') ? _ARG ('0') : _ARG ('A' - 10) ;
		ret = (ret << 4) | DATA (rax - r1x) ;
	}
	ris >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDHEX16S_ (const DATA &arg) {
	String<_RET> ret = String<_RET> (31) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << _RET ('&') ;
	wos << _RET ('H') ;
	for (INDEX i = 0 ; i < 16 ; i++) {
		const auto r1x = DATA (arg >> (60 - i * 4)) & DATA (0X0F) ;
		wos << _RET ((r1x < DATA (10) ? _RET ('0') : _RET ('A') - 10) + r1x) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

template <class _ARG>
inline PACK<WORD ,CHAR> _PARSEIPV4S_ (const String<_ARG> &arg) {
	PACK<WORD ,CHAR> ret = {0} ;
	auto &r1 = _CAST_<EndianBytes<CHAR>> (ret.P2) ;
	auto ris = TextReader<_ARG> (arg.raw ()) ;
	auto rax = _ARG () ;
	auto rbx = VAR () ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	r1[0] = BYTE (rbx) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('.')) ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	r1[1] = BYTE (rbx) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('.')) ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	r1[2] = BYTE (rbx) ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == _ARG ('.')) ;
	ris >> rbx ;
	_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 256) ;
	r1[3] = BYTE (rbx) ;
	ris.clone () >> rax ;
	if (rax == _ARG (':')) {
		ris >> rax ;
		ris >> rbx ;
		_DYNAMIC_ASSERT_ (rbx >= 0 && rbx < 65536) ;
		ret.P1 = WORD (rbx) ;
	}
	ris >> _EOS_ ;
	return std::move (ret) ;
}

template <class _RET>
inline String<_RET> _BUILDIPV4S_ (const PACK<WORD ,CHAR> &arg) {
	String<_RET> ret = String<_RET> (63) ;
	auto &r1 = _CAST_<EndianBytes<CHAR>> (arg.P2) ;
	auto wos = TextWriter<_RET> (ret.raw ()) ;
	wos << VAR (r1[0]) ;
	wos << _RET ('.') ;
	wos << VAR (r1[1]) ;
	wos << _RET ('.') ;
	wos << VAR (r1[2]) ;
	wos << _RET ('.') ;
	wos << VAR (r1[3]) ;
	if (arg.P1 != 0) {
		wos << _RET (':') ;
		wos << VAR (arg.P1) ;
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

#ifdef __CSC_EXT__
inline import DEF<std::chrono::system_clock::time_point (const String<STR> &arg)> _PARSEDATES_ ;

template <class _ARG>
inline std::chrono::system_clock::time_point _PARSEDATES_ (const String<_ARG> &arg) {
	return _PARSEDATES_ (_PARSESTRS_ (arg)) ;
}

inline import DEF<String<STR> (const std::chrono::system_clock::time_point &arg)> _BUILDDATES_ ;

template <class _RET>
inline String<_RET> _BUILDDATES_ (const std::chrono::system_clock::time_point &arg) {
	return _BUILDSTRS_<_RET> (_BUILDDATES_ (arg)) ;
}

inline import DEF<std::chrono::system_clock::time_point (const String<STR> &arg)> _PARSEHOURS_ ;

template <class _ARG>
inline std::chrono::system_clock::time_point _PARSEHOURS_ (const String<_ARG> &arg) {
	return _PARSEDATES_ (_PARSEHOURS_ (arg)) ;
}

inline import DEF<String<STR> (const std::chrono::system_clock::time_point &arg)> _BUILDHOURS_ ;

template <class _RET>
inline String<_RET> _BUILDHOURS_ (const std::chrono::system_clock::time_point &arg) {
	return _BUILDSTRS_<_RET> (_BUILDHOURS_ (arg)) ;
}

inline import DEF<std::chrono::system_clock::time_point (const String<STR> &arg)> _PARSETIMES_ ;

template <class _ARG>
inline std::chrono::system_clock::time_point _PARSETIMES_ (const String<_ARG> &arg) {
	return _PARSEDATES_ (_PARSETIMES_ (arg)) ;
}

inline import DEF<String<STR> (const std::chrono::system_clock::time_point &arg)> _BUILDTIMES_ ;

template <class _RET>
inline String<_RET> _BUILDTIMES_ (const std::chrono::system_clock::time_point &arg) {
	return _BUILDSTRS_<_RET> (_BUILDTIMES_ (arg)) ;
}
#endif
} ;
} ;

#ifndef __CSC_STRICTHPPINL__
#include "csc_string.hpp.inl"
#endif