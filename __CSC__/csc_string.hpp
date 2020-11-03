#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"

namespace CSC {
class TimePoint ;

using IPV4_ADDRESS = PACK<WORD ,CHAR> ;

class StringProc :
	delegate private Wrapped<> {
public:
	imports String<STRU16> cvt_u8s_u16s (const String<STRU8> &val) ;

	imports String<STRU8> cvt_u16s_u8s (const String<STRU16> &val) ;

	imports String<STRU32> cvt_u8s_u32s (const String<STRU8> &val) ;

	imports String<STRU8> cvt_u32s_u8s (const String<STRU32> &val) ;

	imports String<STRU32> cvt_u16s_u32s (const String<STRU16> &val) ;

	imports String<STRU16> cvt_u32s_u16s (const String<STRU32> &val) ;

	imports String<STRU8> cvt_ws_u8s (const String<STRW> &val) ;

	imports String<STRW> cvt_u8s_ws (const String<STRU8> &val) ;

	imports String<STRU16> cvt_ws_u16s (const String<STRW> &val) ;

	imports String<STRW> cvt_u16s_ws (const String<STRU16> &val) ;

	imports String<STRU32> cvt_ws_u32s (const String<STRW> &val) ;

	imports String<STRW> cvt_u32s_ws (const String<STRU32> &val) ;

	imports String<STRA> cvt_ws_gbks (const String<STRW> &val) ;

	imports String<STRW> cvt_gbks_ws (const String<STRA> &val) ;

	imports String<STRU8> cvt_as_u8s (const String<STRA> &val) ;

	imports String<STRA> cvt_u8s_as (const String<STRU8> &val) ;

	imports String<STRU16> cvt_as_u16s (const String<STRA> &val) ;

	imports String<STRA> cvt_u16s_as (const String<STRU16> &val) ;

	imports String<STRU32> cvt_as_u32s (const String<STRA> &val) ;

	imports String<STRA> cvt_u32s_as (const String<STRU32> &val) ;

	imports String<STRA> cvt_ws_as (const String<STRW> &val) ;

	imports String<STRW> cvt_as_ws (const String<STRA> &val) ;

	imports String<STRU8> cvt_uas_u8s (const String<STRA> &val) ;

	imports String<STRU8> cvt_uas_u8s (String<STRA> &&val) ;

	imports String<STRA> cvt_u8s_uas (const String<STRU8> &val) ;

	imports String<STRA> cvt_u8s_uas (String<STRU8> &&val) ;

	template <class _ARG1>
	imports BOOL parse_bools (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_bools (const ARGVF<_ARG1> & ,const BOOL &stru) ;

	template <class _ARG1>
	imports VAR32 parse_var32s (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_var32s (const ARGVF<_ARG1> & ,const VAR32 &stru) ;

	template <class _ARG1>
	imports VAR64 parse_var64s (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_var64s (const ARGVF<_ARG1> & ,const VAR64 &stru) ;

	template <class _ARG1>
	imports VAR parse_vars (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_vars (const ARGVF<_ARG1> & ,const VAR &stru) ;

	template <class _ARG1>
	imports VAL32 parse_val32s (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_val32s (const ARGVF<_ARG1> & ,const VAL32 &stru) ;

	template <class _ARG1>
	imports VAL64 parse_val64s (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_val64s (const ARGVF<_ARG1> & ,const VAL64 &stru) ;

	template <class _ARG1>
	imports String<STR> parse_strs (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_strs (const ARGVF<_ARG1> & ,const String<STR> &stru) ;

	template <class _ARG1>
	imports DATA parse_hexs (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_hexs (const ARGVF<_ARG1> & ,const BYTE &stru) ;

	template <class _ARG1>
	imports String<_ARG1> build_hexs (const ARGVF<_ARG1> & ,const WORD &stru) ;

	template <class _ARG1>
	imports String<_ARG1> build_hexs (const ARGVF<_ARG1> & ,const CHAR &stru) ;

	template <class _ARG1>
	imports String<_ARG1> build_hexs (const ARGVF<_ARG1> & ,const DATA &stru) ;

	template <class _ARG1>
	imports String<STRU8> parse_base64u8s (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_base64u8s (const ARGVF<_ARG1> & ,const String<STRU8> &stru) ;

	template <class _ARG1>
	imports IPV4_ADDRESS parse_ipv4s (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_ipv4s (const ARGVF<_ARG1> & ,const IPV4_ADDRESS &stru) ;

	template <class _ARG1 ,class _RET = REMOVE_CVR_TYPE<TimePoint>>
	imports _RET parse_dates (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_dates (const ARGVF<_ARG1> & ,const TimePoint &stru) ;

	template <class _ARG1 ,class _RET = REMOVE_CVR_TYPE<TimePoint>>
	imports _RET parse_hours (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_hours (const ARGVF<_ARG1> & ,const TimePoint &stru) ;

	template <class _ARG1 ,class _RET = REMOVE_CVR_TYPE<TimePoint>>
	imports _RET parse_times (const String<_ARG1> &stri) ;

	template <class _ARG1>
	imports String<_ARG1> build_times (const ARGVF<_ARG1> & ,const TimePoint &stru) ;
} ;

class StringConvertInvokeProc :
	delegate private Wrapped<> {
public:
	imports String<STRU8> invoke (const ARGVF<String<STRU8>> & ,const String<STRU8> &val) {
		return _COPY_ (val) ;
	}

	imports String<STRU16> invoke (const ARGVF<String<STRU16>> & ,const String<STRU16> &val) {
		return _COPY_ (val) ;
	}

	imports String<STRU32> invoke (const ARGVF<String<STRU32>> & ,const String<STRU32> &val) {
		return _COPY_ (val) ;
	}

	imports String<STRA> invoke (const ARGVF<String<STRA>> & ,const String<STRA> &val) {
		return _COPY_ (val) ;
	}

	imports String<STRW> invoke (const ARGVF<String<STRW>> & ,const String<STRW> &val) {
		return _COPY_ (val) ;
	}

	imports String<STRU8> invoke (const ARGVF<String<STRU8>> & ,const String<STRU16> &val) {
		return StringProc::cvt_u16s_u8s (val) ;
	}

	imports String<STRU16> invoke (const ARGVF<String<STRU16>> & ,const String<STRU8> &val) {
		return StringProc::cvt_u8s_u16s (val) ;
	}

	imports String<STRU8> invoke (const ARGVF<String<STRU8>> & ,const String<STRU32> &val) {
		return StringProc::cvt_u32s_u8s (val) ;
	}

	imports String<STRU32> invoke (const ARGVF<String<STRU32>> & ,const String<STRU8> &val) {
		return StringProc::cvt_u8s_u32s (val) ;
	}

	imports String<STRU16> invoke (const ARGVF<String<STRU16>> & ,const String<STRU32> &val) {
		return StringProc::cvt_u32s_u16s (val) ;
	}

	imports String<STRU32> invoke (const ARGVF<String<STRU32>> & ,const String<STRU16> &val) {
		return StringProc::cvt_u16s_u32s (val) ;
	}

	imports String<STRU8> invoke (const ARGVF<String<STRU8>> & ,const String<STRW> &val) {
		return StringProc::cvt_ws_u8s (val) ;
	}

	imports String<STRW> invoke (const ARGVF<String<STRW>> & ,const String<STRU8> &val) {
		return StringProc::cvt_u8s_ws (val) ;
	}

	imports String<STRU16> invoke (const ARGVF<String<STRU16>> & ,const String<STRW> &val) {
		return StringProc::cvt_ws_u16s (val) ;
	}

	imports String<STRW> invoke (const ARGVF<String<STRW>> & ,const String<STRU16> &val) {
		return StringProc::cvt_u16s_ws (val) ;
	}

	imports String<STRU32> invoke (const ARGVF<String<STRU32>> & ,const String<STRW> &val) {
		return StringProc::cvt_ws_u32s (val) ;
	}

	imports String<STRW> invoke (const ARGVF<String<STRW>> & ,const String<STRU32> &val) {
		return StringProc::cvt_u32s_ws (val) ;
	}

	imports String<STRW> invoke (const ARGVF<String<STRW>> & ,const String<STRA> &val) {
		return StringProc::cvt_as_ws (val) ;
	}

	imports String<STRA> invoke (const ARGVF<String<STRA>> & ,const String<STRW> &val) {
		return StringProc::cvt_ws_as (val) ;
	}

	imports String<STRU8> invoke (const ARGVF<String<STRU8>> & ,const String<STRA> &val) {
		return StringProc::cvt_as_u8s (val) ;
	}

	imports String<STRA> invoke (const ARGVF<String<STRA>> & ,const String<STRU8> &val) {
		return StringProc::cvt_u8s_as (val) ;
	}

	imports String<STRU16> invoke (const ARGVF<String<STRU16>> & ,const String<STRA> &val) {
		return StringProc::cvt_as_u16s (val) ;
	}

	imports String<STRA> invoke (const ARGVF<String<STRA>> & ,const String<STRU16> &val) {
		return StringProc::cvt_u16s_as (val) ;
	}

	imports String<STRU32> invoke (const ARGVF<String<STRU32>> & ,const String<STRA> &val) {
		return StringProc::cvt_as_u32s (val) ;
	}

	imports String<STRA> invoke (const ARGVF<String<STRA>> & ,const String<STRU32> &val) {
		return StringProc::cvt_u32s_as (val) ;
	}
} ;

inline exports String<STRU16> StringProc::cvt_u8s_u16s (const String<STRU8> &val) {
	String<STRU16> ret = String<STRU16> (val.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0X7F)))
				discard ;
			ret[iw++] = STRU16 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XDF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X1F)) ;
			rax = 1 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XEF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X0F)) ;
			rax = 2 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XF7)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X07)) ;
			rax = 3 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFB)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X03)) ;
			rax = 4 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFD)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X01)) ;
			rax = 5 ;
		}
		if switch_once (fax) {
			if (!(rax == 1))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax = 10 ;
		}
		if switch_once (fax) {
			if (!(rax >= 2 && rax <= 5))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax-- ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
		if (rax < 10)
			continue ;
		auto fbx = TRUE ;
		if switch_once (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = STRU16 (rbx) ;
			rax = 0 ;
		}
		if switch_once (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X0010FFFF)))
				discard ;
			rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
			ret[iw++] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
			rax = 0 ;
		}
		if switch_once (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(rbx <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = STRU16 ('?') ;
			rax = 0 ;
		}
		if switch_once (fbx) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU16 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports String<STRU8> StringProc::cvt_u16s_u8s (const String<STRU16> &val) {
	String<STRU8> ret = String<STRU8> (val.length () * 3) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRU32 () ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X007F)))
				discard ;
			ret[iw++] = STRU8 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X07FF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)))
				discard ;
			rbx = STRU32 (i & STRU16 (0X03FF)) ;
			rax = 1 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if switch_once (fax) {
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
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU8 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports String<STRU32> StringProc::cvt_u8s_u32s (const String<STRU8> &val) {
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
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0X7F)))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XDF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X1F)) ;
			rax = 1 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XEF)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X0F)) ;
			rax = 2 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XF7)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X07)) ;
			rax = 3 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFB)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X03)) ;
			rax = 4 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XFD)))
				discard ;
			rbx = STRU32 (i & STRU8 (0X01)) ;
			rax = 5 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		}
		if switch_once (fax) {
			if (!(rax >= 2 && rax <= 5))
				discard ;
			if (!(i <= STRU8 (0XBF)))
				discard ;
			rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
			rax-- ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU32 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports String<STRU8> StringProc::cvt_u32s_u8s (const String<STRU32> &val) {
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
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000007F)))
				discard ;
			ret[iw++] = STRU8 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X000007FF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X001FFFFF)))
				discard ;
			ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
			ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
			ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
		}
		if switch_once (fax) {
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
		if switch_once (fax) {
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
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports String<STRU32> StringProc::cvt_u16s_u32s (const String<STRU16> &val) {
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
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU16 (0X07FF)))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i >= STRU16 (0XD800) && i <= STRU16 (0XDBFF)))
				discard ;
			rbx = STRU32 (i & STRU16 (0X03FF)) ;
			rax = 1 ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			ret[iw++] = STRU32 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 1))
				discard ;
			if (!(i >= STRU16 (0XDC00) && i <= STRU16 (0XDFFF)))
				discard ;
			rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
			ret[iw++] = rbx ;
			rax = 0 ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRU32 ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports String<STRU16> StringProc::cvt_u32s_u16s (const String<STRU32> &val) {
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
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0000FFFF)))
				discard ;
			ret[iw++] = STRU16 (i) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X0010FFFF)))
				discard ;
			ret[iw++] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
			ret[iw++] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(i <= STRU32 (0X7FFFFFFF)))
				discard ;
			ret[iw++] = STRU16 ('?') ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports String<STRU8> StringProc::cvt_ws_u8s (const String<STRW> &val) {
	return StringConvertInvokeProc::invoke (ARGV<String<STRU8>>::ID ,_CAST_ (ARGV<String<STRUW>>::ID ,val)) ;
}

inline exports String<STRW> StringProc::cvt_u8s_ws (const String<STRU8> &val) {
	String<STRUW> ret = StringConvertInvokeProc::invoke (ARGV<String<STRUW>>::ID ,val) ;
	return _MOVE_ (_CAST_ (ARGV<String<STRW>>::ID ,ret)) ;
}

inline exports String<STRU16> StringProc::cvt_ws_u16s (const String<STRW> &val) {
	return StringConvertInvokeProc::invoke (ARGV<String<STRU16>>::ID ,_CAST_ (ARGV<String<STRUW>>::ID ,val)) ;
}

inline exports String<STRW> StringProc::cvt_u16s_ws (const String<STRU16> &val) {
	String<STRUW> ret = StringConvertInvokeProc::invoke (ARGV<String<STRUW>>::ID ,val) ;
	return _MOVE_ (_CAST_ (ARGV<String<STRW>>::ID ,ret)) ;
}

inline exports String<STRU32> StringProc::cvt_ws_u32s (const String<STRW> &val) {
	return StringConvertInvokeProc::invoke (ARGV<String<STRU32>>::ID ,_CAST_ (ARGV<String<STRUW>>::ID ,val)) ;
}

inline exports String<STRW> StringProc::cvt_u32s_ws (const String<STRU32> &val) {
	String<STRUW> ret = StringConvertInvokeProc::invoke (ARGV<String<STRUW>>::ID ,val) ;
	return _MOVE_ (_CAST_ (ARGV<String<STRW>>::ID ,ret)) ;
}

inline exports String<STRU8> StringProc::cvt_as_u8s (const String<STRA> &val) {
	return StringProc::cvt_ws_u8s (StringProc::cvt_as_ws (val)) ;
}

inline exports String<STRA> StringProc::cvt_u8s_as (const String<STRU8> &val) {
	return StringProc::cvt_ws_as (StringProc::cvt_u8s_ws (val)) ;
}

inline exports String<STRU16> StringProc::cvt_as_u16s (const String<STRA> &val) {
	return StringProc::cvt_ws_u16s (StringProc::cvt_as_ws (val)) ;
}

inline exports String<STRA> StringProc::cvt_u16s_as (const String<STRU16> &val) {
	return StringProc::cvt_ws_as (StringProc::cvt_u16s_ws (val)) ;
}

inline exports String<STRU32> StringProc::cvt_as_u32s (const String<STRA> &val) {
	return StringProc::cvt_ws_u32s (StringProc::cvt_as_ws (val)) ;
}

inline exports String<STRA> StringProc::cvt_u32s_as (const String<STRU32> &val) {
	return StringProc::cvt_ws_as (StringProc::cvt_u32s_ws (val)) ;
}

inline exports String<STRU8> StringProc::cvt_uas_u8s (const String<STRA> &val) {
	_STATIC_ASSERT_ (IS_SAME_HELP<STRUA ,STRU8>::compile ()) ;
	String<STRU8> ret = _MOVE_ (_CAST_ (ARGV<String<STRUA>>::ID ,val)) ;
	for (auto &&i : ret) {
		_NOOP_ (i) ;
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
	}
	return _MOVE_ (ret) ;
}

inline exports String<STRU8> StringProc::cvt_uas_u8s (String<STRA> &&val) {
	_STATIC_ASSERT_ (IS_SAME_HELP<STRUA ,STRU8>::compile ()) ;
	String<STRU8> ret = _MOVE_ (_CAST_ (ARGV<String<STRUA>>::ID ,val)) ;
	for (auto &&i : ret) {
		_NOOP_ (i) ;
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
	}
	return _MOVE_ (ret) ;
}

inline exports String<STRA> StringProc::cvt_u8s_uas (const REMOVE_CONST_TYPE<String<STRU8>> &val) {
	_STATIC_ASSERT_ (IS_SAME_HELP<STRUA ,STRU8>::compile ()) ;
	String<STRUA> ret = _MOVE_ (val) ;
	for (auto &&i : ret) {
		_NOOP_ (i) ;
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
	}
	return _MOVE_ (_CAST_ (ARGV<String<STRA>>::ID ,ret)) ;
}

inline exports String<STRA> StringProc::cvt_u8s_uas (REMOVE_CONST_TYPE<String<STRU8>> &&val) {
	_STATIC_ASSERT_ (IS_SAME_HELP<STRUA ,STRU8>::compile ()) ;
	String<STRUA> ret = _MOVE_ (val) ;
	for (auto &&i : ret) {
		_NOOP_ (i) ;
		_DEBUG_ASSERT_ (i <= STRUA (0X7F)) ;
	}
	return _MOVE_ (_CAST_ (ARGV<String<STRA>>::ID ,ret)) ;
}

class GBKSStaticProc :
	delegate private Wrapped<> {
public:
	imports PhanBuffer<const DEF<STRUW[2]>> static_gbks_ws_table () ;

	imports const PACK<PhanBuffer<const DEF<STRUW[2]>> ,HashSet<STRUW>> &static_cvt_gbks_ws_table () ;

	imports const PACK<PhanBuffer<const DEF<STRUW[2]>> ,HashSet<STRUW>> &static_cvt_ws_gbks_table () ;
} ;

inline exports const PACK<PhanBuffer<const DEF<STRUW[2]>> ,HashSet<STRUW>> &GBKSStaticProc::static_cvt_gbks_ws_table () {
	return _CACHE_ ([&] () {
		PACK<PhanBuffer<const DEF<STRUW[2]>> ,HashSet<STRUW>> ret ;
		ret.mP1 = GBKSStaticProc::static_gbks_ws_table () ;
		ret.mP2 = HashSet<STRUW> (ret.mP1.size ()) ;
		for (auto &&i : _RANGE_ (0 ,ret.mP1.size ()))
			ret.mP2.add (ret.mP1[i][1] ,i) ;
		return _MOVE_ (ret) ;
	}) ;
}

inline exports String<STRW> StringProc::cvt_gbks_ws (const String<STRA> &val) {
	auto &r1x = GBKSStaticProc::static_cvt_gbks_ws_table () ;
	auto &r2x = _CAST_ (ARGV<String<STRUA>>::ID ,val) ;
	String<STRW> ret = String<STRW> (r2x.length ()) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = STRUW () ;
	for (auto &&i : r2x) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			rbx = STRUW (i) ;
			rax = 10 ;
		}
		if switch_once (fax) {
			if (!(rax == 1))
				discard ;
			rbx = STRUW ((rbx << 8) | STRUW (i)) ;
			rax = 11 ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
		if (rax < 10)
			continue ;
		INDEX ix = r1x.mP2.map (rbx) ;
		auto fbx = TRUE ;
		if switch_once (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(ix != VAR_NONE))
				discard ;
			ret[iw++] = STRW (r1x.mP1[ix][0]) ;
			rax = 0 ;
		}
		if switch_once (fbx) {
			if (!(rax == 10))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			rax = 1 ;
		}
		if switch_once (fbx) {
			if (!(rax == 11))
				discard ;
			if (!(ix != VAR_NONE))
				discard ;
			ret[iw++] = STRW (r1x.mP1[ix][0]) ;
			rax = 0 ;
		}
		if switch_once (fbx) {
			if (!(rax == 11))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			ret[iw++] = STRW ('?') ;
			rax = 0 ;
		}
		if switch_once (fbx) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (rax > 0)
		ret[iw++] = STRW ('?') ;
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

inline exports const PACK<PhanBuffer<const DEF<STRUW[2]>> ,HashSet<STRUW>> &GBKSStaticProc::static_cvt_ws_gbks_table () {
	return _CACHE_ ([&] () {
		PACK<PhanBuffer<const DEF<STRUW[2]>> ,HashSet<STRUW>> ret ;
		ret.mP1 = GBKSStaticProc::static_gbks_ws_table () ;
		ret.mP2 = HashSet<STRUW> (ret.mP1.size ()) ;
		for (auto &&i : _RANGE_ (0 ,ret.mP1.size ()))
			ret.mP2.add (ret.mP1[i][0] ,i) ;
		return _MOVE_ (ret) ;
	}) ;
}

inline exports String<STRA> StringProc::cvt_ws_gbks (const String<STRW> &val) {
	auto &r1x = GBKSStaticProc::static_cvt_ws_gbks_table () ;
	String<STRUA> ret = String<STRUA> (val.length () * 2) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	for (auto &&i : val) {
		if (rax == VAR_NONE)
			continue ;
		INDEX ix = r1x.mP2.map (STRUW (i)) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(ix == VAR_NONE))
				discard ;
			ret[iw++] = STRUA ('?') ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(r1x.mP1[ix][1] <= STRUW (0X00FF)))
				discard ;
			ret[iw++] = STRUA (r1x.mP1[ix][1]) ;
		}
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (!(r1x.mP1[ix][1] <= STRUW (0XFFFF)))
				discard ;
			ret[iw++] = STRUA (r1x.mP1[ix][1] >> 8) ;
			ret[iw++] = STRUA (r1x.mP1[ix][1]) ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (_CAST_ (ARGV<String<STRA>>::ID ,ret)) ;
}

template <class _ARG1>
inline exports BOOL StringProc::parse_bools (const String<_ARG1> &stri) {
	BOOL ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	rax >> ret >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_bools (const ARGVF<_ARG1> & ,const BOOL &stru) {
	String<_ARG1> ret = String<_ARG1> (7) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << stru << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports VAR32 StringProc::parse_var32s (const String<_ARG1> &stri) {
	VAR32 ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	rax >> ret >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_var32s (const ARGVF<_ARG1> & ,const VAR32 &stru) {
	String<_ARG1> ret = String<_ARG1> (63) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << stru << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports VAR64 StringProc::parse_var64s (const String<_ARG1> &stri) {
	VAR64 ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	rax >> ret >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_var64s (const ARGVF<_ARG1> & ,const VAR64 &stru) {
	String<_ARG1> ret = String<_ARG1> (127) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << stru << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports VAR StringProc::parse_vars (const String<_ARG1> &stri) {
	VAR ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	rax >> ret >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_vars (const ARGVF<_ARG1> & ,const VAR &stru) {
	String<_ARG1> ret = String<_ARG1> (127) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << stru << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports VAL32 StringProc::parse_val32s (const String<_ARG1> &stri) {
	VAL32 ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	rax >> ret >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_val32s (const ARGVF<_ARG1> & ,const VAL32 &stru) {
	String<_ARG1> ret = String<_ARG1> (127) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << stru << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports VAL64 StringProc::parse_val64s (const String<_ARG1> &stri) {
	VAL64 ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	rax >> ret >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_val64s (const ARGVF<_ARG1> & ,const VAL64 &stru) {
	String<_ARG1> ret = String<_ARG1> (127) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << stru << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<STR> StringProc::parse_strs (const String<_ARG1> &stri) {
	return StringConvertInvokeProc::invoke (ARGV<String<STR>>::ID ,stri) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_strs (const ARGVF<_ARG1> & ,const String<STR> &stru) {
	return StringConvertInvokeProc::invoke (ARGV<String<_ARG1>>::ID ,stru) ;
}

class RegexMatcher {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual BOOL match (const String<STRU8> &expr) const = 0 ;
		virtual Deque<ARRAY2<INDEX>> search (const String<STRU8> &expr) const = 0 ;
		virtual String<STRU8> replace (const String<STRU8> &expr ,const String<STRU8> &rep) const = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit RegexMatcher () = default ;

	explicit RegexMatcher (const String<STRU8> &reg) ;

	BOOL match (const String<STRU8> &expr) const {
		return mThis->match (expr) ;
	}

	Deque<ARRAY2<INDEX>> search (const String<STRU8> &expr) const {
		return mThis->search (expr) ;
	}

	String<STRU8> replace (const String<STRU8> &expr ,const String<STRU8> &rep) const {
		return mThis->replace (expr ,rep) ;
	}
} ;

template <class _ARG1>
inline exports DATA StringProc::parse_hexs (const String<_ARG1> &stri) {
	DATA ret = 0 ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	auto rbx = _ARG1 () ;
	rax >> rbx ;
	_DYNAMIC_ASSERT_ (rbx == _ARG1 ('0')) ;
	rax >> rbx ;
	_DYNAMIC_ASSERT_ (rbx == _ARG1 ('X')) ;
	const auto r1x = ARRAY2<_ARG1> ({_ARG1 ('0') ,(_ARG1 ('A' - 10))}) ;
	auto rcx = VAR_ZERO ;
	while (TRUE) {
		rax >> rbx ;
		if (rbx == 0)
			break ;
		rcx++ ;
		const auto r2x = BOOL (rbx >= _ARG1 ('0') && rbx <= _ARG1 ('9')) ;
		const auto r3x = BOOL (rbx >= _ARG1 ('A') && rbx <= _ARG1 ('F')) ;
		_DYNAMIC_ASSERT_ (r2x || r3x) ;
		auto &r4x = _CALL_ ([&] () {
			if (r2x)
				return _BYREF_ (r1x[0]) ;
			return _BYREF_ (r1x[1]) ;
		}).self ;
		ret = (ret << 4) | DATA (rbx - r4x) ;
	}
	_DYNAMIC_ASSERT_ (rcx == 2 || rcx == 4 || rcx == 8 || rcx == 16) ;
	rax >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_hexs (const ARGVF<_ARG1> & ,const BYTE &stru) {
	String<_ARG1> ret = String<_ARG1> (31) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << _ARG1 ('0') ;
	rax << _ARG1 ('X') ;
	const auto r1x = ARRAY2<_ARG1> ({_ARG1 ('0') ,(_ARG1 ('A' - 10))}) ;
	const auto r2x = _SIZEOF_ (BYTE) * 8 - 4 ;
	for (auto &&i : _RANGE_ (0 ,_SIZEOF_ (BYTE) * 2)) {
		const auto r3x = BYTE (BYTE (stru >> (r2x - i * 4)) & BYTE (0X0F)) ;
		auto &r4x = _CALL_ ([&] () {
			if (r3x < DATA (10))
				return _BYREF_ (r1x[0]) ;
			return _BYREF_ (r1x[1]) ;
		}).self ;
		rax << _ARG1 (r4x + r3x) ;
	}
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_hexs (const ARGVF<_ARG1> & ,const WORD &stru) {
	String<_ARG1> ret = String<_ARG1> (31) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << _ARG1 ('0') ;
	rax << _ARG1 ('X') ;
	const auto r1x = ARRAY2<_ARG1> ({_ARG1 ('0') ,(_ARG1 ('A' - 10))}) ;
	const auto r2x = _SIZEOF_ (WORD) * 8 - 4 ;
	for (auto &&i : _RANGE_ (0 ,_SIZEOF_ (WORD) * 2)) {
		const auto r3x = WORD (WORD (stru >> (r2x - i * 4)) & WORD (0X0F)) ;
		auto &r4x = _CALL_ ([&] () {
			if (r3x < DATA (10))
				return _BYREF_ (r1x[0]) ;
			return _BYREF_ (r1x[1]) ;
		}).self ;
		rax << _ARG1 (r4x + r3x) ;
	}
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_hexs (const ARGVF<_ARG1> & ,const CHAR &stru) {
	String<_ARG1> ret = String<_ARG1> (31) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << _ARG1 ('0') ;
	rax << _ARG1 ('X') ;
	const auto r1x = ARRAY2<_ARG1> ({_ARG1 ('0') ,(_ARG1 ('A' - 10))}) ;
	const auto r2x = _SIZEOF_ (CHAR) * 8 - 4 ;
	for (auto &&i : _RANGE_ (0 ,_SIZEOF_ (CHAR) * 2)) {
		const auto r3x = CHAR (CHAR (stru >> (r2x - i * 4)) & CHAR (0X0F)) ;
		auto &r4x = _CALL_ ([&] () {
			if (r3x < DATA (10))
				return _BYREF_ (r1x[0]) ;
			return _BYREF_ (r1x[1]) ;
		}).self ;
		rax << _ARG1 (r4x + r3x) ;
	}
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_hexs (const ARGVF<_ARG1> & ,const DATA &stru) {
	String<_ARG1> ret = String<_ARG1> (31) ;
	auto rax = TextWriter<_ARG1> (ret.raw ()) ;
	rax << _ARG1 ('0') ;
	rax << _ARG1 ('X') ;
	const auto r1x = ARRAY2<_ARG1> ({_ARG1 ('0') ,(_ARG1 ('A' - 10))}) ;
	const auto r2x = _SIZEOF_ (DATA) * 8 - 4 ;
	for (auto &&i : _RANGE_ (0 ,_SIZEOF_ (DATA) * 2)) {
		const auto r3x = DATA (DATA (stru >> (r2x - i * 4)) & DATA (0X0F)) ;
		auto &r4x = _CALL_ ([&] () {
			if (r3x < DATA (10))
				return _BYREF_ (r1x[0]) ;
			return _BYREF_ (r1x[1]) ;
		}).self ;
		rax << _ARG1 (r4x + r3x) ;
	}
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_base64u8s (const ARGVF<_ARG1> & ,const String<STRU8> &stru) {
	static constexpr auto M_BASE64 = PACK<STRU8[65]> ({
		'A' ,'B' ,'C' ,'D' ,'E' ,'F' ,'G' ,'H' ,
		'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,
		'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,'W' ,'X' ,
		'Y' ,'Z' ,'a' ,'b' ,'c' ,'d' ,'e' ,'f' ,
		'g' ,'h' ,'i' ,'j' ,'k' ,'l' ,'m' ,'n' ,
		'o' ,'p' ,'q' ,'r' ,'s' ,'t' ,'u' ,'v' ,
		'w' ,'x' ,'y' ,'z' ,'0' ,'1' ,'2' ,'3' ,
		'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'+' ,'/' ,
		'='}) ;
	const auto r1x = (stru.length () + 2) / 3 * 4 ;
	String<_ARG1> ret = String<_ARG1> (r1x) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = CHAR () ;
	for (auto &&i : stru) {
		if (rax == VAR_NONE)
			continue ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			rbx = CHAR (i) ;
			rax = 1 ;
		}
		if switch_once (fax) {
			if (!(rax == 1))
				discard ;
			rbx = CHAR ((rbx << 8) | CHAR (i & STRU8 (0XFF))) ;
			rax = 2 ;
		}
		if switch_once (fax) {
			if (!(rax == 2))
				discard ;
			rbx = CHAR ((rbx << 8) | CHAR (i & STRU8 (0XFF))) ;
			rax = 0 ;
			const auto r2x = CHAR ((rbx >> 18) & CHAR (0X3F)) ;
			const auto r3x = CHAR ((rbx >> 12) & CHAR (0X3F)) ;
			const auto r4x = CHAR ((rbx >> 6) & CHAR (0X3F)) ;
			const auto r5x = CHAR (rbx & CHAR (0X3F)) ;
			ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r2x)]) ;
			ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r3x)]) ;
			ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r4x)]) ;
			ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r5x)]) ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	auto fbx = TRUE ;
	if switch_once (fbx) {
		if (!(rax == 1))
			discard ;
		rbx = CHAR (rbx << 16) ;
		const auto r6x = CHAR ((rbx >> 18) & CHAR (0X3F)) ;
		const auto r7x = CHAR ((rbx >> 12) & CHAR (0X3F)) ;
		ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r6x)]) ;
		ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r7x)]) ;
	}
	if switch_once (fbx) {
		if (!(rax == 2))
			discard ;
		rbx = CHAR (rbx << 8) ;
		const auto r8x = CHAR ((rbx >> 18) & CHAR (0X3F)) ;
		const auto r9x = CHAR ((rbx >> 12) & CHAR (0X3F)) ;
		const auto r10x = CHAR ((rbx >> 6) & CHAR (0X3F)) ;
		ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r8x)]) ;
		ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r9x)]) ;
		ret[iw++] = _ARG1 (M_BASE64.mP1[INDEX (r10x)]) ;
	}
	if switch_once (fbx) {
		_DYNAMIC_ASSERT_ (rax == 0) ;
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<STRU8> StringProc::parse_base64u8s (const String<_ARG1> &stri) {
	static constexpr auto M_BASE64 = PACK<INDEX[96]> ({
		-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		-1 ,-1 ,-1 ,62 ,-1 ,-1 ,-1 ,63 ,
		52 ,53 ,54 ,55 ,56 ,57 ,58 ,59 ,
		60 ,61 ,-1 ,-1 ,-1 ,+0 ,-1 ,-1 ,
		-1 ,+0 ,+1 ,+2 ,+3 ,+4 ,+5 ,+6 ,
		+7 ,+8 ,+9 ,10 ,11 ,12 ,13 ,14 ,
		15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,
		23 ,24 ,25 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		-1 ,26 ,27 ,28 ,29 ,30 ,31 ,32 ,
		33 ,34 ,35 ,36 ,37 ,38 ,39 ,40 ,
		41 ,42 ,43 ,44 ,45 ,46 ,47 ,48 ,
		49 ,50 ,51 ,-1 ,-1 ,-1 ,-1 ,-1}) ;
	const auto r1x = (stri.length () + 3) / 4 * 3 ;
	String<STRU8> ret = String<STRU8> (r1x) ;
	INDEX iw = 0 ;
	auto rax = VAR_ZERO ;
	auto rbx = CHAR () ;
	for (auto &&i : stri) {
		if (rax == VAR_NONE)
			continue ;
		const auto r2x = LENGTH (i) - 32 ;
		auto &r3x = _CALL_ ([&] () {
			if (r2x >= 0 && r2x < _COUNTOF_ (DEF<decltype (M_BASE64.mP1)>))
				return _BYREF_ (M_BASE64.mP1[r2x]) ;
			return _BYREF_ (M_BASE64.mP1[0]) ;
		}).self ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(rax == 0))
				discard ;
			if (r3x == VAR_NONE)
				discard ;
			rbx = CHAR (CHAR (r3x) & CHAR (0X3F)) ;
			rax = 1 ;
		}
		if switch_once (fax) {
			if (!(rax == 1))
				discard ;
			if (r3x == VAR_NONE)
				discard ;
			rbx = CHAR ((rbx << 6) | (CHAR (r3x) & CHAR (0X3F))) ;
			rax = 2 ;
		}
		if switch_once (fax) {
			if (!(rax == 2))
				discard ;
			if (r3x == VAR_NONE)
				discard ;
			rbx = CHAR ((rbx << 6) | (CHAR (r3x) & CHAR (0X3F))) ;
			rax = 3 ;
		}
		if switch_once (fax) {
			if (!(rax == 3))
				discard ;
			if (r3x == VAR_NONE)
				discard ;
			rbx = CHAR ((rbx << 6) | (CHAR (r3x) & CHAR (0X3F))) ;
			rax = 0 ;
			ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
			ret[iw++] = STRU8 (rbx & CHAR (0XFF)) ;
		}
		if switch_once (fax) {
			ret.clear () ;
			rax = VAR_NONE ;
		}
	}
	auto fbx = TRUE ;
	if switch_once (fbx) {
		if (!(rax == 1))
			discard ;
		rbx = CHAR (rbx << 18) ;
		ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
	}
	if switch_once (fbx) {
		if (!(rax == 2))
			discard ;
		rbx = CHAR (rbx << 12) ;
		ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
		ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
	}
	if switch_once (fbx) {
		if (!(rax == 3))
			discard ;
		rbx = CHAR (rbx << 6) ;
		ret[iw++] = STRU8 ((rbx >> 16) & CHAR (0XFF)) ;
		ret[iw++] = STRU8 ((rbx >> 8) & CHAR (0XFF)) ;
		ret[iw++] = STRU8 (rbx & CHAR (0XFF)) ;
	}
	if switch_once (fbx) {
		_DYNAMIC_ASSERT_ (rax == 0) ;
	}
	if (iw < ret.size ())
		ret[iw] = 0 ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports IPV4_ADDRESS StringProc::parse_ipv4s (const String<_ARG1> &stri) {
	IPV4_ADDRESS ret ;
	auto rax = TextReader<_ARG1> (stri.raw ()) ;
	auto rbx = _ARG1 () ;
	auto rcx = VAR () ;
	rax >> rcx ;
	_DYNAMIC_ASSERT_ (rcx >= 0 && rcx < 256) ;
	const auto r1x = BYTE (rcx) ;
	rax >> rbx ;
	_DYNAMIC_ASSERT_ (rbx == _ARG1 ('.')) ;
	rax >> rcx ;
	_DYNAMIC_ASSERT_ (rcx >= 0 && rcx < 256) ;
	const auto r2x = BYTE (rcx) ;
	rax >> rbx ;
	_DYNAMIC_ASSERT_ (rbx == _ARG1 ('.')) ;
	rax >> rcx ;
	_DYNAMIC_ASSERT_ (rcx >= 0 && rcx < 256) ;
	const auto r3x = BYTE (rcx) ;
	rax >> rbx ;
	_DYNAMIC_ASSERT_ (rbx == _ARG1 ('.')) ;
	rax >> rcx ;
	_DYNAMIC_ASSERT_ (rcx >= 0 && rcx < 256) ;
	const auto r4x = BYTE (rcx) ;
	const auto r5x = PACK<ARRAY_BIND_TYPE<BYTE ,SIZE_OF_TYPE<CHAR>>> {r1x ,r2x ,r3x ,r4x} ;
	ByteReader<BYTE> (PhanBuffer<const BYTE>::make (r5x.mP1)) >> ret.mP2 ;
	ret.mP1 = 0 ;
	rax.share () >> rbx ;
	if switch_once (TRUE) {
		if (rbx != _ARG1 (':'))
			discard ;
		rax >> rbx ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx < 65536) ;
		ret.mP1 = WORD (rcx) ;
	}
	rax >> TextReader<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_ipv4s (const ARGVF<_ARG1> & ,const IPV4_ADDRESS &stru) {
	String<_ARG1> ret = String<_ARG1> (63) ;
	auto rax = PACK<ARRAY_BIND_TYPE<BYTE ,SIZE_OF_TYPE<CHAR>>> () ;
	ByteWriter<BYTE> (PhanBuffer<BYTE>::make (rax.mP1)) << stru.mP2 ;
	auto rbx = TextWriter<_ARG1> (ret.raw ()) ;
	rbx << VAR (rax.mP1[0]) ;
	rbx << _ARG1 ('.') ;
	rbx << VAR (rax.mP1[1]) ;
	rbx << _ARG1 ('.') ;
	rbx << VAR (rax.mP1[2]) ;
	rbx << _ARG1 ('.') ;
	rbx << VAR (rax.mP1[3]) ;
	if switch_once (TRUE) {
		if (stru.mP1 == 0)
			discard ;
		rbx << _ARG1 (':') ;
		rbx << VAR (stru.mP1) ;
	}
	rbx << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1 ,class _RET>
inline exports _RET StringProc::parse_dates (const String<_ARG1> &stri) {
	using R1X = DEPENDENT_TYPE<TimePoint ,struct ANONYMOUS> ;
	const auto r1x = _CALL_ ([&] () {
		ARRAY8<LENGTH> ret ;
		ret.fill (0) ;
		auto rax = TextReader<STR> (stri.raw ()) ;
		auto rbx = STR () ;
		auto rcx = VAR32 () ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 1900) ;
		ret[0] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR ('-')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 1 && rcx <= 12) ;
		ret[1] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR ('-')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 1 && rcx <= 31) ;
		ret[2] = rcx ;
		rax >> TextReader<_ARG1>::EOS ;
		return _MOVE_ (ret) ;
	}) ;
	return R1X (r1x) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_dates (const ARGVF<_ARG1> & ,const TimePoint &stru) {
	using R1X = DEPENDENT_TYPE<TimePoint ,struct ANONYMOUS> ;
	String<STR> ret = String<STR> (31) ;
	auto rax = TextWriter<STR> (ret.raw ()) ;
	auto &r1x = _FORWARD_ (ARGV<R1X>::ID ,stru) ;
	const auto r2x = r1x.calendar () ;
	rax << r2x[0] ;
	rax << STR ('-') ;
	if (r2x[1] < 10)
		rax << STR ('0') ;
	rax << r2x[1] ;
	rax << STR ('-') ;
	if (r2x[2] < 10)
		rax << STR ('0') ;
	rax << r2x[2] ;
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1 ,class _RET>
inline exports _RET StringProc::parse_hours (const String<_ARG1> &stri) {
	using R1X = DEPENDENT_TYPE<TimePoint ,struct ANONYMOUS> ;
	const auto r1x = _CALL_ ([&] () {
		ARRAY8<LENGTH> ret ;
		ret.fill (0) ;
		auto rax = TextReader<STR> (stri.raw ()) ;
		auto rbx = STR () ;
		auto rcx = VAR32 () ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx <= 23) ;
		ret[5] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR (':')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx <= 59) ;
		ret[6] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR (':')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx <= 60) ;
		ret[7] = rcx ;
		rax >> TextReader<_ARG1>::EOS ;
		return _MOVE_ (ret) ;
	}) ;
	return R1X (r1x) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_hours (const ARGVF<_ARG1> & ,const TimePoint &stru) {
	using R1X = DEPENDENT_TYPE<TimePoint ,struct ANONYMOUS> ;
	String<STR> ret = String<STR> (31) ;
	auto rax = TextWriter<STR> (ret.raw ()) ;
	auto &r1x = _FORWARD_ (ARGV<R1X>::ID ,stru) ;
	const auto r2x = r1x.calendar () ;
	if (r2x[5] < 10)
		rax << STR ('0') ;
	rax << r2x[5] ;
	rax << STR (':') ;
	if (r2x[6] < 10)
		rax << STR ('0') ;
	rax << r2x[6] ;
	rax << STR (':') ;
	if (r2x[7] < 10)
		rax << STR ('0') ;
	rax << r2x[7] ;
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}

template <class _ARG1 ,class _RET>
inline exports _RET StringProc::parse_times (const String<_ARG1> &stri) {
	using R1X = DEPENDENT_TYPE<TimePoint ,struct ANONYMOUS> ;
	const auto r1x = _CALL_ ([&] () {
		ARRAY8<VAR32> ret ;
		ret.fill (0) ;
		auto rax = TextReader<STR> (stri.raw ()) ;
		auto rbx = STR () ;
		auto rcx = VAR32 () ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 1900) ;
		ret[0] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR ('-')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 1 && rcx <= 12) ;
		ret[1] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR ('-')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 1 && rcx <= 31) ;
		ret[2] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR (' ')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx <= 23) ;
		ret[5] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR (':')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx <= 59) ;
		ret[6] = rcx ;
		rax >> rbx ;
		_DYNAMIC_ASSERT_ (rbx == STR (':')) ;
		rax >> rcx ;
		_DYNAMIC_ASSERT_ (rcx >= 0 && rcx <= 60) ;
		ret[7] = rcx ;
		rax >> TextReader<_ARG1>::EOS ;
		return _MOVE_ (ret) ;
	}) ;
	return R1X (r1x) ;
}

template <class _ARG1>
inline exports String<_ARG1> StringProc::build_times (const ARGVF<_ARG1> & ,const TimePoint &stru) {
	using R1X = DEPENDENT_TYPE<TimePoint ,struct ANONYMOUS> ;
	String<STR> ret = String<STR> (63) ;
	auto rax = TextWriter<STR> (ret.raw ()) ;
	auto &r1x = _FORWARD_ (ARGV<R1X>::ID ,stru) ;
	const auto r2x = r1x.calendar () ;
	rax << r2x[0] ;
	rax << STR ('-') ;
	if (r2x[1] < 10)
		rax << STR ('0') ;
	rax << r2x[1] ;
	rax << STR ('-') ;
	if (r2x[2] < 10)
		rax << STR ('0') ;
	rax << r2x[2] ;
	rax << STR (' ') ;
	if (r2x[5] < 10)
		rax << STR ('0') ;
	rax << r2x[5] ;
	rax << STR (':') ;
	if (r2x[6] < 10)
		rax << STR ('0') ;
	rax << r2x[6] ;
	rax << STR (':') ;
	if (r2x[7] < 10)
		rax << STR ('0') ;
	rax << r2x[7] ;
	rax << TextWriter<_ARG1>::EOS ;
	return _MOVE_ (ret) ;
}
} ;