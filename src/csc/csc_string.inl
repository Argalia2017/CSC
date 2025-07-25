﻿#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_string.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <clocale>
#include "csc_begin.h"

namespace CSC {
template <class A>
struct FUNCTION_string_from_impl {
	template <class ARG1>
	forceinline CREF<String<A>> operator() (CREF<String<ARG1>> that) {
		assert (IS_SAME<A ,ARG1>::expr) ;
		return keep[TYPE<String<A>>::expr] (Pointer::from (that)) ;
	}

	template <class ARG1>
	forceinline RREF<String<A>> operator() (RREF<String<ARG1>> that) {
		assert (IS_SAME<A ,ARG1>::expr) ;
		return move (keep[TYPE<String<A>>::expr] (Pointer::from (that))) ;
	}
} ;

struct FUNCTION_string_from {
	template <class ARG1>
	forceinline consteval FUNCTION_string_from_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_string_from_impl<ARG1> () ;
	}
} ;

static constexpr auto string_from = FUNCTION_string_from () ;

#ifdef __CSC_SYSTEM_WINDOWS__
using csc_locale_t = _locale_t ;

struct FUNCTION_string_locale {
	forceinline UniqueRef<csc_locale_t> operator() () const {
		return UniqueRef<csc_locale_t> ([&] (VREF<csc_locale_t> me) {
			me = _create_locale (LC_CTYPE ,String<STRA>::zero ()) ;
		} ,[&] (VREF<csc_locale_t> me) {
			_free_locale (me) ;
		}) ;
	}
} ;

struct FUNCTION_string_stra_from_strw {
	forceinline void operator() (VREF<String<STRA>> dst ,CREF<String<STRW>> src ,CREF<csc_locale_t> loc) const {
		auto rax = csc_size_t (0) ;
		const auto r1x = _wcstombs_s_l ((&rax) ,dst ,dst.size () ,src ,_TRUNCATE ,loc) ;
		assume (r1x == 0) ;
		dst.trunc (rax) ;
	}
} ;

struct FUNCTION_string_strw_from_stra {
	forceinline void operator() (VREF<String<STRW>> dst ,CREF<String<STRA>> src ,CREF<csc_locale_t> loc) const {
		auto rax = csc_size_t (0) ;
		const auto r1x = _mbstowcs_s_l ((&rax) ,dst ,dst.size () ,src ,_TRUNCATE ,loc) ;
		assume (r1x == 0) ;
		dst.trunc (rax) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
using csc_locale_t = locale_t ;

struct FUNCTION_string_locale {
	forceinline UniqueRef<csc_locale_t> operator() () const {
		return UniqueRef<csc_locale_t> ([&] (VREF<csc_locale_t> me) {
			me = newlocale (LC_CTYPE_MASK ,String<STRA>::zero () ,NULL) ;
		} ,[&] (VREF<csc_locale_t> me) {
			freelocale (me) ;
		}) ;
	}
} ;

struct FUNCTION_string_stra_from_strw {
	forceinline void operator() (VREF<String<STRA>> dst ,CREF<String<STRW>> src ,CREF<csc_locale_t> loc) const {
		auto rax = mbstate_t () ;
		inline_memset (rax) ;
		uselocale (loc) ;
		auto rbx = src.ref ;
		const auto r1x = LENGTH (wcsrtombs (dst ,(&rbx) ,dst.size () ,(&rax))) ;
		assume (r1x >= 0) ;
		dst.trunc (r1x) ;
	}
} ;

struct FUNCTION_string_strw_from_stra {
	forceinline void operator() (VREF<String<STRW>> dst ,CREF<String<STRA>> src ,CREF<csc_locale_t> loc) const {
		auto rax = mbstate_t () ;
		inline_memset (rax) ;
		uselocale (loc) ;
		auto rbx = src.ref ;
		const auto r1x = LENGTH (mbsrtowcs (dst ,(&rbx) ,dst.size () ,(&rax))) ;
		assume (r1x >= 0) ;
		dst.trunc (r1x) ;
	}
} ;
#endif

static constexpr auto string_locale = FUNCTION_string_locale () ;
static constexpr auto string_stra_from_strw = FUNCTION_string_stra_from_strw () ;
static constexpr auto string_strw_from_stra = FUNCTION_string_strw_from_stra () ;

struct StringProcLayout {
	UniqueRef<csc_locale_t> mStringLocale ;
} ;

class StringProcImplHolder final implement Fat<StringProcHolder ,StringProcLayout> {
public:
	void initialize () override {
		self.mStringLocale = string_locale () ;
	}

	String<STRA> stra_from_strw (CREF<String<STRW>> a) const override {
		String<STRA> ret = String<STRA> (a.length () * 2 + 1) ;
		string_stra_from_strw (ret ,a ,self.mStringLocale) ;
		return move (ret) ;
	}

	String<STRA> stra_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STRA>::expr] (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return stra_from_strw (string_from[TYPE<STRW>::expr] (a)) ;
		assert (FALSE) ;
		return String<STRA> () ;
	}

	String<STRW> strw_from_stra (CREF<String<STRA>> a) const override {
		String<STRW> ret = String<STRW> (a.length () + 1) ;
		string_strw_from_stra (ret ,a ,self.mStringLocale) ;
		return move (ret) ;
	}

	String<STRW> strw_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return strw_from_stra (string_from[TYPE<STRA>::expr] (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STRW>::expr] (a) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stra (CREF<String<STRA>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stra (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_strw (CREF<String<STRW>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_strw (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (a) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) const override {
		String<STRU8> ret = String<STRU8> (a.length () * 3) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU16 (0X007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU16 (0X07FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] < STRU16 (0XD800))
					discard ;
				if (a[i] > STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (a[i] < STRU16 (0XDC00))
					discard ;
				if (a[i] > STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU8 (0X3F) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<STRU8> ret = String<STRU8> (a.length () * 6) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0000007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X000007FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X001FFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X03FFFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) const override {
		String<STRU16> ret = String<STRU16> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			if ifdo (TRUE) {
				auto act = TRUE ;
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0X7F))
						discard ;
					ret[ix] = STRU16 (a[i]) ;
					ix++ ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XDF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
					rax = FLAG (1) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XEF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
					rax = FLAG (2) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XF7))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X07)) ;
					rax = FLAG (3) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XFB))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X03)) ;
					rax = FLAG (4) ;
				}
				if ifdo (act) {
					if (rax != FLAG (0))
						discard ;
					if (a[i] > STRU8 (0XFD))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X01)) ;
					rax = FLAG (5) ;
				}
				if ifdo (act) {
					if (rax != FLAG (1))
						discard ;
					if (a[i] > STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax = FLAG (10) ;
				}
				if ifdo (act) {
					if (!inline_between (rax ,2 ,6))
						discard ;
					if (a[i] > STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = FLAG (99) ;
				}
			}
			if ifdo (TRUE) {
				if (rax != FLAG (10))
					discard ;
				auto act = TRUE ;
				if ifdo (act) {
					if (rbx > STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (rbx) ;
					ix++ ;
					rax = FLAG (0) ;
				}
				if ifdo (act) {
					if (rbx > STRU32 (0X0010FFFF))
						discard ;
					rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
					ret[ix] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
					rax = FLAG (0) ;
				}
				if ifdo (act) {
					if (rbx > STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 (0X3F) ;
					ix++ ;
					rax = FLAG (0) ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = FLAG (99) ;
				}
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU16 (0X3F) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<STRU16> ret = String<STRU16> (a.length () * 2) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = STRU16 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X0010FFFF))
					discard ;
				ret[ix] = (STRU16 ((a[i] - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
				ix++ ;
				ret[ix] = (STRU16 (a[i] - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = STRU16 (0X3F) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<STRU32> ret = String<STRU32> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0X7F))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XDF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XEF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
				rax = FLAG (2) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XF7))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X07)) ;
				rax = FLAG (3) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XFB))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X03)) ;
				rax = FLAG (4) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU8 (0XFD))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X01)) ;
				rax = FLAG (5) ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (a[i] > STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				if (!inline_between (rax ,2 ,6))
					discard ;
				if (a[i] > STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				rax-- ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<STRU32> ret = String<STRU32> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = FLAG (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == FLAG (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] > STRU16 (0X07FF))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				if (a[i] < STRU16 (0XD800))
					discard ;
				if (a[i] > STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = FLAG (1) ;
			}
			if ifdo (act) {
				if (rax != FLAG (0))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != FLAG (1))
					discard ;
				if (a[i] < STRU16 (0XDC00))
					discard ;
				if (a[i] > STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = FLAG (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = FLAG (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == FLAG (0))
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRUA> strua_from_stra (RREF<String<STRA>> a) const override {
		return move (keep[TYPE<String<STRUA>>::expr] (Pointer::from (a))) ;
	}

	String<STRA> stra_from_strua (RREF<String<STRUA>> a) const override {
		return move (keep[TYPE<String<STRA>>::expr] (Pointer::from (a))) ;
	}

	String<STRUW> struw_from_strw (RREF<String<STRW>> a) const override {
		return move (keep[TYPE<String<STRUW>>::expr] (Pointer::from (a))) ;
	}

	String<STRW> strw_from_struw (RREF<String<STRUW>> a) const override {
		return move (keep[TYPE<String<STRW>>::expr] (Pointer::from (a))) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU8>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU16>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU32>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru8 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru8 (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru16 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru16 (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STRU8> stru8_from_struw (CREF<String<STRUW>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return string_from[TYPE<STRU8>::expr] (a) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return stru8_from_stru16 (string_from[TYPE<STRU16>::expr] (a)) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return stru8_from_stru32 (string_from[TYPE<STRU32>::expr] (a)) ;
		assert (FALSE) ;
		return String<STRU8> () ;
	}

	String<STRU16> stru16_from_struw (CREF<String<STRUW>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return stru16_from_stru8 (string_from[TYPE<STRU8>::expr] (a)) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return string_from[TYPE<STRU16>::expr] (a) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return stru16_from_stru32 (string_from[TYPE<STRU32>::expr] (a)) ;
		assert (FALSE) ;
		return String<STRU16> () ;
	}

	String<STRU32> stru32_from_struw (CREF<String<STRUW>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return stru32_from_stru8 (string_from[TYPE<STRU8>::expr] (a)) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return stru32_from_stru16 (string_from[TYPE<STRU16>::expr] (a)) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return string_from[TYPE<STRU32>::expr] (a) ;
		assert (FALSE) ;
		return String<STRU32> () ;
	}

	String<STRU8> stru8_from_strs (CREF<String<STRA>> a) const override {
		return stru8_from_struw (struw_from_strw (strw_from_stra (a))) ;
	}

	String<STRU8> stru8_from_strs (CREF<String<STRW>> a) const override {
		return stru8_from_struw (struw_from_strw (move (a))) ;
	}

	String<STRU16> stru16_from_strs (CREF<String<STRA>> a) const override {
		return stru16_from_struw (struw_from_strw (strw_from_stra (a))) ;
	}

	String<STRU16> stru16_from_strs (CREF<String<STRW>> a) const override {
		return stru16_from_struw (struw_from_strw (move (a))) ;
	}

	String<STRU32> stru32_from_strs (CREF<String<STRA>> a) const override {
		return stru32_from_struw (struw_from_strw (strw_from_stra (a))) ;
	}

	String<STRU32> stru32_from_strs (CREF<String<STRW>> a) const override {
		return stru32_from_struw (struw_from_strw (move (a))) ;
	}
} ;

exports CREF<OfThis<UniqueRef<StringProcLayout>>> StringProcHolder::expr_m () {
	return memorize ([&] () {
		OfThis<UniqueRef<StringProcLayout>> ret ;
		ret.mThis = UniqueRef<StringProcLayout>::make () ;
		StringProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StringProcHolder> StringProcHolder::hold (VREF<StringProcLayout> that) {
	return VFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

exports CFat<StringProcHolder> StringProcHolder::hold (CREF<StringProcLayout> that) {
	return CFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

struct RegularReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	StreamShape mBackup ;
	Box<TextReader> mTextReader ;
	Deque<STRU32> mDeque ;
	STRU32 mTop ;
} ;

class RegularReader implement RegularReaderLayout {
protected:
	using RegularReaderLayout::mStream ;
	using RegularReaderLayout::mBackup ;
	using RegularReaderLayout::mTextReader ;
	using RegularReaderLayout::mDeque ;
	using RegularReaderLayout::mTop ;

public:
	implicit RegularReader () = default ;

	explicit RegularReader (RREF<Ref<RefBuffer<BYTE>>> stream ,CREF<LENGTH> ring_size) {
		mStream = move (stream) ;
		mBackup.mRead = 0 ;
		mBackup.mWrite = mStream->size () ;
		mDeque = Deque<STRU32> (ring_size) ;
	}

	void use_text () {
		mTextReader = Box<TextReader>::make (mStream.share ()) ;
		mTextReader->reset (mBackup) ;
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.full ())
				break ;
			mTextReader.ref >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	void get (CREF<INDEX> index ,VREF<STRU32> item) const {
		item = mDeque[index] ;
	}

	forceinline STRU32 operator[] (CREF<INDEX> index) const {
		STRU32 ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	template <class ARG1>
	void read (XREF<ARG1> item) {
		mTextReader->reset (mBackup) ;
		mTextReader.ref >> item ;
		mBackup = mTextReader->backup () ;
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.full ())
				break ;
			mTextReader.ref >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	template <class ARG1>
	forceinline VREF<RegularReader> operator>> (XREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}

	void next () {
		mTextReader.ref >> mTop ;
		if ifdo (TRUE) {
			if (mTop == STRU32 (0X00))
				discard ;
			mBackup.mRead++ ;
		}
		mDeque.take () ;
		mDeque.add (mTop) ;
	}

	forceinline void operator++ (int) {
		next () ;
	}
} ;

struct PinnedCounter {
	Pin<PinnedCounter> mPin ;
	LENGTH mCounter ;
} ;

class ScopeCounter implement Proxy {
private:
	using SCOPECOUNTER_MAX_DEPTH = ENUM<256> ;

protected:
	PinnedCounter mThat ;

public:
	static CREF<ScopeCounter> from (CREF<PinnedCounter> that) {
		return Pointer::from (that) ;
	}

	static CREF<ScopeCounter> from (RREF<PinnedCounter> that) = delete ;

	void enter () const {
		mThat.mPin.ref.mCounter++ ;
		assume (mThat.mCounter < SCOPECOUNTER_MAX_DEPTH::expr) ;
	}

	void leave () const {
		mThat.mPin.ref.mCounter-- ;
		assume (mThat.mCounter >= ZERO) ;
	}
} ;

struct XmlParserNodeType {
	enum {
		Value ,
		Array ,
		Object ,
		ETC
	} ;
} ;

struct XmlParserNode {
	String<STRU8> mName ;
	String<STRU8> mValue ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	INDEX mMember ;
	Just<XmlParserNodeType> mType ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct XmlParserTree {
	Array<XmlParserNode> mList ;
	INDEX mRoot ;
} ;

struct MakeXmlParserLayout {
	RegularReader mReader ;
	PinnedCounter mPinnedCounter ;
	List<XmlParserNode> mList ;
	SortedMap<INDEX> mArrayMap ;
	List<INDEX> mArrayMemberList ;
	SortedMap<String<STRU8>> mObjectMap ;
	List<Set<String<STRU8>>> mObjectMemberList ;
	INDEX mLastIndex ;
	String<STRU8> mLastString ;
} ;

class MakeXmlParser implement MakeXmlParserLayout {
protected:
	using MakeXmlParserLayout::mReader ;
	using MakeXmlParserLayout::mPinnedCounter ;
	using MakeXmlParserLayout::mList ;
	using MakeXmlParserLayout::mArrayMap ;
	using MakeXmlParserLayout::mObjectMap ;
	using MakeXmlParserLayout::mLastIndex ;
	using MakeXmlParserLayout::mLastString ;

public:
	implicit MakeXmlParser () = default ;

	explicit MakeXmlParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
		mPinnedCounter.mCounter = 0 ;
		mPinnedCounter.mPin.pin (mPinnedCounter) ;
		mArrayMap = SortedMap<INDEX> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<STRU8>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	XmlParserTree poll () {
		XmlParserTree ret ;
		ret.mList = Array<XmlParserNode> (mList.length ()) ;
		const auto r1x = Array<INDEX>::make (mList.range ()) ;
		for (auto &&i : ret.mList.range ()) {
			ret.mList[i] = move (mList[r1x[i]]) ;
			const auto r2x = ret.mList[i].mArrayMap.length () ;
			ret.mList[i].mArrayMap.remap () ;
			assume (ret.mList[i].mArrayMap.length () == r2x) ;
			ret.mList[i].mObjectMap.remap () ;
		}
		ret.mRoot = NONE ;
		if ifdo (TRUE) {
			if (r1x.length () == 0)
				discard ;
			ret.mRoot = 0 ;
		}
		return move (ret) ;
	}

	void generate () {
		//@info: $0->$10 $9 $11
		//@info: $1->${keyword}
		//@info: $2->"${escape}"
		//@info: $3->$1 = $2
		//@info: $4->${eps}|$3 $4
		//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
		//@info: $6-><!--comment-->
		//@info: $7->${[^<>]+}
		//@info: $8->$5 $8|$6 $8|$7 $8
		//@info: $9->${eps}|$5
		//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		//@info: $11->${end}
		read_shift_e0 () ;
		assume (mLastIndex == 0) ;
	}

	void read_shift_e0 () {
		read_shift_e10 () ;
		mReader >> GAP ;
		read_shift_e9 () ;
		mReader >> GAP ;
		read_shift_e11 () ;
	}

	//@info: $1->${keyword}
	void read_shift_e1 () {
		mReader >> KeywordText::from (mLastString) ;
	}

	//@info: $2->"${escape}"
	void read_shift_e2 () {
		mReader >> EscapeText::from (mLastString) ;
	}

	//@info: $3->$1 = $2
	void read_shift_e3 (CREF<INDEX> curr) {
		INDEX ix = mList.insert () ;
		read_shift_e1 () ;
		mList[ix].mName = move (mLastString) ;
		mList[ix].mMember = NONE ;
		mList[ix].mType = XmlParserNodeType::Value ;
		mList[ix].mParent = curr ;
		mList[ix].mBrother = NONE ;
		mList[ix].mChild = NONE ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		read_shift_e2 () ;
		mList[ix].mValue = move (mLastString) ;
		mLastIndex = ix ;
	}

	//@info: $4->${eps}|$3 $4
	void read_shift_e4 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			if (!StreamProc::is_word (mReader[0]))
				break ;
			read_shift_e3 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mList[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			mList[curr].mObjectMap.add (mList[iy].mName ,iy) ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
	void read_shift_e5 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mPinnedCounter)) ;
		mReader >> slice ("<") ;
		INDEX ix = mList.insert () ;
		read_shift_e1 () ;
		mList[ix].mName = move (mLastString) ;
		mList[ix].mObjectMap = mObjectMap.share () ;
		mList[ix].mMember = mObjectMemberList.insert () ;
		mList[ix].mType = XmlParserNodeType::Object ;
		mList[ix].mParent = curr ;
		mList[ix].mBrother = NONE ;
		mList[ix].mChild = NONE ;
		mReader >> GAP ;
		read_shift_e4 (ix) ;
		INDEX iy = mLastIndex ;
		mReader >> GAP ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != STRU32 ('>'))
				discard ;
			mReader++ ;
			mReader >> GAP ;
			read_shift_e8 (ix ,iy) ;
			mList[ix].mChild = mLastIndex ;
			mReader >> GAP ;
			mReader >> slice ("</") ;
			read_shift_e1 () ;
			assume (mLastString == mList[ix].mName) ;
			mReader >> GAP ;
			mReader >> slice (">") ;
		}
		if ifdo (act) {
			mReader >> slice ("/>") ;
		}
		mObjectMemberList.remove (mList[ix].mMember) ;
		mList[ix].mMember = NONE ;
		mLastIndex = ix ;
	}

	//@info: $6-><!--${comment}-->
	void read_shift_e6 () {
		mReader >> slice ("<!--") ;
		while (TRUE) {
			if (mReader[0] == STRU32 ('-'))
				if (mReader[1] == STRU32 ('-'))
					if (mReader[2] == STRU32 ('>'))
						break ;
			mReader++ ;
		}
		mReader >> slice ("-->") ;
	}

	//@info: $7->${[^<>]+}
	void read_shift_e7 (CREF<INDEX> curr) {
		auto rax = String<STRU8>::make () ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (mReader[0] == STRU32 ('<'))
				break ;
			if (mReader[0] == STRU32 ('>'))
				break ;
			assume (ix < rax.size ()) ;
			rax[ix] = STRU8 (mReader[0]) ;
			ix++ ;
			mReader++ ;
		}
		while (TRUE) {
			ix-- ;
			if (ix < 0)
				break ;
			if (!StreamProc::is_blank (rax[ix]))
				break ;
		}
		ix++ ;
		rax.trunc (ix) ;
		if ifdo (TRUE) {
			if (mList[curr].mValue.size () > 0)
				discard ;
			mList[curr].mValue = move (rax) ;
		}
		mList[curr].mValue.splice (mList[curr].mValue.length () ,rax) ;
	}

	//@info: $8->$5 $8|$6 $8|$7 $8
	void read_shift_e8 (CREF<INDEX> curr ,CREF<INDEX> first) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mPinnedCounter)) ;
		INDEX ix = first ;
		INDEX iy = first ;
		INDEX kx = mList[curr].mMember ;
		while (TRUE) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!is_first_of_object ())
					discard ;
				read_shift_e5 (curr) ;
				auto rbx = move (mList[mLastIndex].mName) ;
				INDEX jx = mObjectMemberList[kx].map (rbx) ;
				if ifdo (TRUE) {
					if (jx != NONE)
						discard ;
					jx = mList.insert () ;
					mList[jx].mName = move (rbx) ;
					mObjectMemberList[kx].add (mList[jx].mName ,jx) ;
					mList[jx].mArrayMap = mArrayMap.share () ;
					mList[jx].mMember = NONE ;
					mList[jx].mType = XmlParserNodeType::Array ;
					mList[jx].mParent = curr ;
					mList[jx].mBrother = NONE ;
					mList[jx].mChild = NONE ;
					brother_prev (ix ,iy) = jx ;
					iy = jx ;
					mList[curr].mObjectMap.add (mList[jx].mName ,iy) ;
				}
				mList[mLastIndex].mParent = jx ;
				brother_prev (mList[jx].mChild ,mList[jx].mMember) = mLastIndex ;
				mList[jx].mMember = mLastIndex ;
				const auto r1x = mList[jx].mArrayMap.length () ;
				mList[jx].mArrayMap.add (r1x ,mLastIndex) ;
			}
			if ifdo (act) {
				if (!is_first_of_comment ())
					discard ;
				read_shift_e6 () ;
			}
			mReader >> GAP ;
			if (mReader[0] == STRU32 ('<'))
				if (mReader[1] == STRU32 ('/'))
					break ;
			if ifdo (act) {
				read_shift_e7 (curr) ;
			}
		}
		if ifdo (TRUE) {
			INDEX iz = mList[curr].mChild ;
			while (TRUE) {
				if (iz == NONE)
					break ;
				mList[iz].mMember = NONE ;
				iz = mList[iz].mBrother ;
			}
		}
		mLastIndex = ix ;
	}

	VREF<INDEX> brother_prev (VREF<INDEX> prev ,CREF<INDEX> curr) leftvalue {
		if (prev == NONE)
			return prev ;
		return mList[curr].mBrother ;
	}

	BOOL is_first_of_comment () const {
		if (mReader[0] == STRU32 ('<'))
			if (mReader[1] == STRU32 ('!'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_first_of_object () const {
		if (mReader[0] == STRU32 ('<'))
			if (mReader[1] != STRU32 ('/'))
				return TRUE ;
		return FALSE ;
	}

	//@info: $9->${eps}|$5
	void read_shift_e9 () {
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (0X00))
				discard ;
			read_shift_e5 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
	void read_shift_e10 () {
		mReader >> BOM ;
		if (mReader[0] != STRU32 ('<'))
			return ;
		if (mReader[1] != STRU32 ('?'))
			return ;
		mReader++ ;
		mReader++ ;
		mReader >> slice ("xml") ;
		mReader >> GAP ;
		mReader >> slice ("version") ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		mReader >> slice ("\"1.0\"") ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 ('?'))
				discard ;
			mReader >> slice ("encoding") ;
			mReader >> GAP ;
			mReader >> slice ("=") ;
			mReader >> GAP ;
			mReader >> slice ("\"utf-8\"") ;
			mReader >> GAP ;
		}
		mReader >> slice ("?>") ;
	}

	//@info: $11->${end}
	void read_shift_e11 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class XmlParserImplHolder final implement Fat<XmlParserHolder ,XmlParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeXmlParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		self.mThis = UniqueRef<XmlParserTree>::make (rax.poll ()) ;
		self.mIndex = self.mThis->mRoot ;
	}

	BOOL exist () const override {
		if (!self.mThis.exist ())
			return FALSE ;
		if (self.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	XmlParserLayout root () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mRoot ;
		}
		return move (ret) ;
	}

	XmlParserLayout parent () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mParent ;
		}
		return move (ret) ;
	}

	XmlParserLayout brother () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	XmlParserLayout child () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mChild ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<INDEX> index) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<Slice> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<String<STRU8>> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list () const override {
		Array<XmlParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mList[self.mIndex].mArrayMap.length () ;
			ret = Array<XmlParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mList[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list (CREF<LENGTH> size_) const override {
		Array<XmlParserLayout> ret = Array<XmlParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mList[self.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mList[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<XmlParserLayout> that) const override {
		const auto r1x = inline_compr (self.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!self.mThis.exist ())
			return TRUE ;
		if (address (self.mThis->mList) != address (that.mThis->mList))
			return FALSE ;
		if (self.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String<STRU8>> name () const leftvalue override {
		assert (exist ()) ;
		return self.mThis->mList[self.mIndex].mName ;
	}

	BOOL parse (CREF<BOOL> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<BOOL>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 parse (CREF<VAL32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL32>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 parse (CREF<VAL64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL64>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 parse (CREF<FLT32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT32>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 parse (CREF<FLT64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT64>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> parse (CREF<String<STRA>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stra_from_stru (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRW> parse (CREF<String<STRW>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::strw_from_stru (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU8> parse (CREF<String<STRU8>> def) const override {
		if (!exist ())
			return def ;
		try {
			return self.mThis->mList[self.mIndex].mValue ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU16> parse (CREF<String<STRU16>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru16_from_stru8 (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU32> parse (CREF<String<STRU32>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru32_from_stru8 (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	template <class ARG1>
	forceinline Array<ARG1> parse_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.range ()) {
			const auto r2x = XmlParserHolder::hold (self)->child (i) ;
			ret[i] = XmlParserHolder::hold (r2x)->parse (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<XmlParserHolder> XmlParserHolder::hold (VREF<XmlParserLayout> that) {
	return VFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

exports CFat<XmlParserHolder> XmlParserHolder::hold (CREF<XmlParserLayout> that) {
	return CFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

struct JsonParserNodeType {
	enum {
		Value ,
		Array ,
		Object ,
		ETC
	} ;
} ;

struct JsonParserNode {
	String<STRU8> mName ;
	String<STRU8> mValue ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	Just<JsonParserNodeType> mType ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct JsonParserTree {
	Array<JsonParserNode> mList ;
	INDEX mRoot ;
} ;

struct MakeJsonParserLayout {
	RegularReader mReader ;
	PinnedCounter mPinnedCounter ;
	List<JsonParserNode> mList ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	INDEX mLastIndex ;
	String<STRU8> mLastString ;
} ;

class MakeJsonParser implement MakeJsonParserLayout {
protected:
	using MakeJsonParserLayout::mReader ;
	using MakeJsonParserLayout::mPinnedCounter ;
	using MakeJsonParserLayout::mList ;
	using MakeJsonParserLayout::mArrayMap ;
	using MakeJsonParserLayout::mObjectMap ;
	using MakeJsonParserLayout::mLastIndex ;
	using MakeJsonParserLayout::mLastString ;

public:
	implicit MakeJsonParser () = default ;

	explicit MakeJsonParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
		mPinnedCounter.mCounter = 0 ;
		mPinnedCounter.mPin.pin (mPinnedCounter) ;
		mArrayMap = SortedMap<INDEX> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<STRU8>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	JsonParserTree poll () {
		JsonParserTree ret ;
		ret.mList = Array<JsonParserNode> (mList.length ()) ;
		const auto r1x = Array<INDEX>::make (mList.range ()) ;
		for (auto &&i : ret.mList.range ()) {
			ret.mList[i] = move (mList[r1x[i]]) ;
			const auto r2x = ret.mList[i].mArrayMap.length () ;
			ret.mList[i].mArrayMap.remap () ;
			assume (ret.mList[i].mArrayMap.length () == r2x) ;
			const auto r3x = ret.mList[i].mObjectMap.length () ;
			ret.mList[i].mObjectMap.remap () ;
			assume (ret.mList[i].mObjectMap.length () == r3x) ;
		}
		ret.mRoot = NONE ;
		if ifdo (TRUE) {
			if (r1x.length () == 0)
				discard ;
			ret.mRoot = 0 ;
		}
		return move (ret) ;
	}

	void generate () {
		//@info: $0->$11 $10 $12
		//@info: $1->${scalar}
		//@info: $2->true|false|null
		//@info: $3->"${escape}"
		//@info: $4->$1|$2|$3|$6|$9
		//@info: $5->$4|$4 , $5
		//@info: $6->[ ]|[ $5 ]
		//@info: $7->$3 : $4
		//@info: $8->$7|$7 , $8
		//@info: $9->{ }|{ $8 }
		//@info: $10->${eps}|$4
		//@info: $11->${eps}
		//@info: $12->${end}
		read_shift_e0 () ;
		assert (mLastIndex == 0) ;
	}

	//@info: $0->$11 $10 $12
	void read_shift_e0 () {
		read_shift_e11 () ;
		mReader >> GAP ;
		read_shift_e10 () ;
		mReader >> GAP ;
		read_shift_e12 () ;
	}

	//@info: $1->${scalar}
	void read_shift_e1 () {
		mReader >> ScalarText::from (mLastString) ;
	}

	//@info: $2->true|false|null
	void read_shift_e2 () {
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != STRU32 ('t'))
				discard ;
			mReader >> slice ("true") ;
			mLastString = String<STRU8>::make (slice ("true")) ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('f'))
				discard ;
			mReader >> slice ("false") ;
			mLastString = String<STRU8>::make (slice ("false")) ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('n'))
				discard ;
			mReader >> slice ("null") ;
			mLastString = String<STRU8> () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	//@info: $3->"${escape}"
	void read_shift_e3 () {
		mReader >> EscapeText::from (mLastString) ;
	}

	//@info: $4->$1|$2|$3|$6|$9
	void read_shift_e4 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mPinnedCounter)) ;
		INDEX ix = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!is_first_of_number ())
				discard ;
			ix = mList.insert () ;
			mList[ix].mName = move (mLastString) ;
			read_shift_e1 () ;
			mList[ix].mValue = move (mLastString) ;
			mList[ix].mType = JsonParserNodeType::Value ;
			mList[ix].mParent = curr ;
			mList[ix].mBrother = NONE ;
			mList[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (!is_first_of_const ())
				discard ;
			ix = mList.insert () ;
			mList[ix].mName = move (mLastString) ;
			read_shift_e2 () ;
			mList[ix].mValue = move (mLastString) ;
			mList[ix].mType = JsonParserNodeType::Value ;
			mList[ix].mParent = curr ;
			mList[ix].mBrother = NONE ;
			mList[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('\"'))
				discard ;
			ix = mList.insert () ;
			mList[ix].mName = move (mLastString) ;
			read_shift_e3 () ;
			mList[ix].mValue = move (mLastString) ;
			mList[ix].mType = JsonParserNodeType::Value ;
			mList[ix].mParent = curr ;
			mList[ix].mBrother = NONE ;
			mList[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('['))
				discard ;
			read_shift_e6 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('{'))
				discard ;
			read_shift_e9 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
		mLastIndex = ix ;
	}

	BOOL is_first_of_number () const {
		if (mReader[0] == STRU32 ('+'))
			return TRUE ;
		if (mReader[0] == STRU32 ('-'))
			return TRUE ;
		if (mReader[0] >= STRU32 ('0'))
			if (mReader[0] <= STRU32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_first_of_const () const {
		if (mReader[0] == STRU32 ('t'))
			return TRUE ;
		if (mReader[0] == STRU32 ('f'))
			return TRUE ;
		if (mReader[0] == STRU32 ('n'))
			return TRUE ;
		return FALSE ;
	}

	//@info: $5->$4|$4 , $5
	void read_shift_e5 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			read_shift_e4 (curr) ;
			brother_prev (ix ,iy) = mLastIndex ;
			iy = mLastIndex ;
			const auto r1x = mList[curr].mArrayMap.length () ;
			mList[curr].mArrayMap.add (r1x ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != STRU32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $6->[ ]|[ $5 ]
	void read_shift_e6 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mPinnedCounter)) ;
		mReader >> slice ("[") ;
		INDEX ix = mList.insert () ;
		mList[ix].mName = move (mLastString) ;
		mList[ix].mArrayMap = mArrayMap.share () ;
		mList[ix].mType = JsonParserNodeType::Array ;
		mList[ix].mParent = curr ;
		mList[ix].mBrother = NONE ;
		mList[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (']'))
				break ;
			read_shift_e5 (ix) ;
			mList[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("]") ;
		mLastIndex = ix ;
	}

	//@info: $7->$3 : $4
	void read_shift_e7 (CREF<INDEX> curr) {
		read_shift_e3 () ;
		mReader >> GAP ;
		mReader >> slice (":") ;
		mReader >> GAP ;
		read_shift_e4 (curr) ;
	}

	//@info: $8->$7|$7 , $8
	void read_shift_e8 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			read_shift_e7 (curr) ;
			brother_prev (ix ,iy) = mLastIndex ;
			iy = mLastIndex ;
			const auto r1x = mList[curr].mArrayMap.length () ;
			mList[curr].mArrayMap.add (r1x ,iy) ;
			mList[curr].mObjectMap.add (mList[iy].mName ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != STRU32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	VREF<INDEX> brother_prev (VREF<INDEX> prev ,CREF<INDEX> curr) leftvalue {
		if (prev == NONE)
			return prev ;
		return mList[curr].mBrother ;
	}

	//@info: $9->{ }|{ $8 }
	void read_shift_e9 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mPinnedCounter)) ;
		mReader >> slice ("{") ;
		INDEX ix = mList.insert () ;
		mList[ix].mName = move (mLastString) ;
		mList[ix].mArrayMap = mArrayMap.share () ;
		mList[ix].mObjectMap = mObjectMap.share () ;
		mList[ix].mType = JsonParserNodeType::Object ;
		mList[ix].mParent = curr ;
		mList[ix].mBrother = NONE ;
		mList[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 ('}'))
				discard ;
			read_shift_e8 (ix) ;
			mList[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("}") ;
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|$4
	void read_shift_e10 () {
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (0X00))
				discard ;
			read_shift_e4 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $11->${eps}
	void read_shift_e11 () {
		mReader >> BOM ;
	}

	//@info: $12->${end}
	void read_shift_e12 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class JsonParserImplHolder final implement Fat<JsonParserHolder ,JsonParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeJsonParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		self.mThis = UniqueRef<JsonParserTree>::make (rax.poll ()) ;
		self.mIndex = self.mThis->mRoot ;
	}

	BOOL exist () const override {
		if (!self.mThis.exist ())
			return FALSE ;
		if (self.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	JsonParserLayout root () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mRoot ;
		}
		return move (ret) ;
	}

	JsonParserLayout parent () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mParent ;
		}
		return move (ret) ;
	}

	JsonParserLayout brother () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	JsonParserLayout child () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mChild ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<INDEX> index) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<Slice> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<String<STRU8>> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mList[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list () const override {
		Array<JsonParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mList[self.mIndex].mArrayMap.length () ;
			ret = Array<JsonParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mList[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list (CREF<LENGTH> size_) const override {
		Array<JsonParserLayout> ret = Array<JsonParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mList[self.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mList[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<JsonParserLayout> that) const override {
		const auto r1x = inline_compr (self.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!self.mThis.exist ())
			return TRUE ;
		if (address (self.mThis->mList) != address (that.mThis->mList))
			return FALSE ;
		if (self.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String<STRU8>> name () const leftvalue override {
		assert (exist ()) ;
		return self.mThis->mList[self.mIndex].mName ;
	}

	BOOL parse (CREF<BOOL> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<BOOL>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 parse (CREF<VAL32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL32>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 parse (CREF<VAL64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL64>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 parse (CREF<FLT32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT32>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 parse (CREF<FLT64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT64>::make (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> parse (CREF<String<STRA>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stra_from_stru (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRW> parse (CREF<String<STRW>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::strw_from_stru (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU8> parse (CREF<String<STRU8>> def) const override {
		if (!exist ())
			return def ;
		try {
			return self.mThis->mList[self.mIndex].mValue ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU16> parse (CREF<String<STRU16>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru16_from_stru8 (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU32> parse (CREF<String<STRU32>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru32_from_stru8 (self.mThis->mList[self.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> parse (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL32> parse (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<VAL64> parse (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT32> parse (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<FLT64> parse (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRA>> parse (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRW>> parse (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU8>> parse (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU16>> parse (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<STRU32>> parse (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return parse_impl (def ,size_) ;
	}

	template <class ARG1>
	forceinline Array<ARG1> parse_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.range ()) {
			const auto r2x = JsonParserHolder::hold (self)->child (i) ;
			ret[i] = JsonParserHolder::hold (r2x)->parse (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<JsonParserHolder> JsonParserHolder::hold (VREF<JsonParserLayout> that) {
	return VFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

exports CFat<JsonParserHolder> JsonParserHolder::hold (CREF<JsonParserLayout> that) {
	return CFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

struct PlyParserDataType {
	enum {
		Bool ,
		Val32 ,
		Val64 ,
		Flt32 ,
		Flt64 ,
		Byte ,
		Word ,
		Char ,
		Quad ,
		Null ,
		ETC
	} ;
} ;

struct PlyParserProperty {
	String<STRU8> mName ;
	Just<PlyParserDataType> mType ;
	Just<PlyParserDataType> mListType ;
	LENGTH mListSize ;
	INDEX mPlyBegin ;
	INDEX mPlyEnd ;
} ;

struct PlyParserElement {
	String<STRU8> mName ;
	LENGTH mLineSize ;
	LENGTH mLineStep ;
	LENGTH mLineLength ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String<STRU8>> mPropertySet ;
	LENGTH mLastSize ;
	RefBuffer<BYTE> mPlyBuffer ;
	INDEX mPlyIndex ;
	RefBuffer<BYTE> mExtBuffer ;
	INDEX mExtIndex ;
} ;

struct PlyParserTree {
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
} ;

struct MakePlyParserLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	TextReader mTextReader ;
	ByteReader mByteReader ;
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
	BOOL mDiffEndianFlag ;
	StreamShape mBodyBackup ;
	INDEX mLastIndex ;
	String<STRU8> mLastType ;
	String<STRU8> mLastString ;
	Set<String<STRU8>> mPropertyType ;
	Set<String<STRU8>> mPropertyListType ;
} ;

class MakePlyParser implement MakePlyParserLayout {
protected:
	using MakePlyParserLayout::mStream ;
	using MakePlyParserLayout::mTextReader ;
	using MakePlyParserLayout::mByteReader ;
	using MakePlyParserLayout::mFormat ;
	using MakePlyParserLayout::mElementList ;
	using MakePlyParserLayout::mElementSet ;
	using MakePlyParserLayout::mDiffEndianFlag ;
	using MakePlyParserLayout::mLastIndex ;
	using MakePlyParserLayout::mLastType ;
	using MakePlyParserLayout::mLastString ;
	using MakePlyParserLayout::mPropertyType ;
	using MakePlyParserLayout::mPropertyListType ;

public:
	implicit MakePlyParser () = default ;

	explicit MakePlyParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mStream = move (stream) ;
		mPropertyType.add (slice ("bool") ,PlyParserDataType::Bool) ;
		mPropertyType.add (slice ("int") ,PlyParserDataType::Val32) ;
		mPropertyType.add (slice ("int64") ,PlyParserDataType::Val64) ;
		mPropertyType.add (slice ("float") ,PlyParserDataType::Flt32) ;
		mPropertyType.add (slice ("double") ,PlyParserDataType::Flt64) ;
		mPropertyType.add (slice ("uchar") ,PlyParserDataType::Byte) ;
		mPropertyType.add (slice ("ushort") ,PlyParserDataType::Word) ;
		mPropertyType.add (slice ("uint") ,PlyParserDataType::Char) ;
		mPropertyType.add (slice ("uint64") ,PlyParserDataType::Quad) ;
		mPropertyListType.add (slice ("uchar") ,PlyParserDataType::Byte) ;
		mPropertyListType.add (slice ("ushort") ,PlyParserDataType::Word) ;
		mPropertyListType.add (slice ("uint") ,PlyParserDataType::Char) ;
	}

	PlyParserTree poll () {
		PlyParserTree ret ;
		ret.mFormat = move (mFormat) ;
		ret.mElementList = move (mElementList) ;
		ret.mElementSet = move (mElementSet) ;
		return move (ret) ;
	}

	void generate () {
		read_header () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mFormat != slice ("ascii"))
				discard ;
			read_body_text () ;
		}
		if ifdo (act) {
			if (mFormat != slice ("binary_big_endian"))
				discard ;
			mDiffEndianFlag = !StreamProc::big_endian () ;
			read_body_byte () ;
		}
		if ifdo (act) {
			if (mFormat != slice ("binary_little_endian"))
				discard ;
			mDiffEndianFlag = StreamProc::big_endian () ;
			read_body_byte () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read_header () {
		mTextReader = TextReader (mStream.share ()) ;
		mTextReader >> BOM ;
		mTextReader >> slice ("ply") ;
		mTextReader >> GAP ;
		mTextReader >> slice ("format") ;
		mTextReader >> GAP ;
		mTextReader >> KeywordText::from (mLastString) ;
		mFormat = move (mLastString) ;
		mTextReader >> EndlineText::from (mLastString) ;
		mTextReader >> GAP ;
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			mTextReader >> KeywordText::from (mLastString) ;
			if (mLastString == slice ("end_header"))
				break ;
			mTextReader >> GAP ;
			auto act = TRUE ;
			if ifdo (act) {
				if (mLastString != slice ("element"))
					discard ;
				ix = mElementList.insert () ;
				mTextReader >> KeywordText::from (mLastString) ;
				mElementList[ix].mName = move (mLastString) ;
				mTextReader >> GAP ;
				mTextReader >> ScalarText::from (mLastString) ;
				const auto r1x = StringParse<LENGTH>::make (mLastString) ;
				assume (r1x >= 0) ;
				mElementList[ix].mLineSize = r1x ;
				mElementList[ix].mLineStep = 0 ;
				mElementList[ix].mLineLength = 0 ;
				mElementList[ix].mPropertyList = ArrayList<PlyParserProperty> (r1x) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				mTextReader >> KeywordText::from (mLastType) ;
				mTextReader >> GAP ;
				if (mLastType != slice ("list"))
					discard ;
				iy = mElementList[ix].mPropertyList.insert () ;
				mTextReader >> KeywordText::from (mLastString) ;
				const auto r2x = mPropertyListType.map (mLastString) ;
				assume (r2x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r2x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r3x = ply_parser_data_type_size (r2x) ;
				mElementList[ix].mLineStep += r3x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mTextReader >> KeywordText::from (mLastString) ;
				const auto r4x = mPropertyType.map (mLastString) ;
				if ifdo (TRUE) {
					if (r4x == PlyParserDataType::Val32)
						discard ;
					if (r4x == PlyParserDataType::Val64)
						discard ;
					assume (FALSE) ;
				}
				mElementList[ix].mPropertyList[iy].mListType = r4x ;
				mElementList[ix].mPropertyList[iy].mListSize = 0 ;
				mElementList[ix].mLineStep += SIZE_OF<INDEX>::expr ;
				mElementList[ix].mLineStep += SIZE_OF<INDEX>::expr ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mTextReader >> KeywordText::from (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = move (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				iy = mElementList[ix].mPropertyList.insert () ;
				const auto r5x = mPropertyType.map (mLastType) ;
				assume (r5x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r5x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r6x = ply_parser_data_type_size (r5x) ;
				mElementList[ix].mLineStep += r6x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mElementList[ix].mPropertyList[iy].mListType = PlyParserDataType::Null ;
				mElementList[ix].mPropertyList[iy].mListSize = mElementList[ix].mLineSize ;
				mTextReader >> GAP ;
				mTextReader >> KeywordText::from (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = move (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("comment"))
					discard ;
				mTextReader >> EndlineText::from (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				assume (FALSE) ;
			}
		}
		if ifdo (TRUE) {
			auto rax = STRU32 () ;
			const auto r7x = mTextReader.backup () ;
			mTextReader.read (rax) ;
			if (rax == STRU32 ('\r'))
				discard ;
			mTextReader.reset (r7x) ;
		}
		if ifdo (TRUE) {
			auto rax = STRU32 () ;
			mTextReader.read (rax) ;
			assume (rax == STRU32 ('\n')) ;
			mBodyBackup = mTextReader.backup () ;
		}
		for (auto &&i : mElementList.range ()) {
			auto &&rax = mElementList[i] ;
			rax.mPropertyList.remap () ;
			rax.mPropertySet = Set<String<STRU8>> (rax.mPropertyList.length ()) ;
			for (auto &&j : rax.mPropertyList.range ())
				rax.mPropertySet.add (rax.mPropertyList[j].mName ,j) ;
		}
		if ifdo (TRUE) {
			mElementList.remap () ;
			mElementSet = Set<String<STRU8>> (mElementList.length ()) ;
			for (auto &&j : mElementList.range ())
				mElementSet.add (mElementList[j].mName ,j) ;
		}
		for (auto &&i : mElementList.range ()) {
			auto &&rax = mElementList[i] ;
			const auto r8x = rax.mLineSize * rax.mLineStep ;
			rax.mPlyBuffer = RefBuffer<BYTE> (r8x) ;
			rax.mPlyIndex = 0 ;
			rax.mExtBuffer = RefBuffer<BYTE> () ;
			rax.mExtIndex = 0 ;
		}
	}

	LENGTH ply_parser_data_type_size (CREF<Just<PlyParserDataType>> type_) const {
		if (type_ == PlyParserDataType::Bool)
			return 1 ;
		if (type_ == PlyParserDataType::Val32)
			return 4 ;
		if (type_ == PlyParserDataType::Val64)
			return 8 ;
		if (type_ == PlyParserDataType::Flt32)
			return 4 ;
		if (type_ == PlyParserDataType::Flt64)
			return 8 ;
		if (type_ == PlyParserDataType::Byte)
			return 1 ;
		if (type_ == PlyParserDataType::Word)
			return 2 ;
		if (type_ == PlyParserDataType::Char)
			return 4 ;
		if (type_ == PlyParserDataType::Quad)
			return 8 ;
		return 0 ;
	}

	void read_body_text () {
		mTextReader = TextReader (mStream.share ()) ;
		mTextReader.reset (mBodyBackup) ;
		mTextReader >> GAP ;
		for (auto &&i : iter (0 ,mElementList.length ())) {
			for (auto &&j : iter (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : iter (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_text_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_text_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mTextReader >> GAP ;
		mTextReader >> EOS ;
	}

	void read_body_text_item (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mTextReader.poll (TYPE<FLT32>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r2x) ,SIZE_OF<FLT32>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r3x = mTextReader.poll (TYPE<FLT64>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r3x) ,SIZE_OF<FLT64>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r4x = mTextReader.poll (TYPE<BOOL>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r4x) ,SIZE_OF<BOOL>::expr) ;
			element.mPlyIndex += SIZE_OF<BOOL>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r5x = mTextReader.poll (TYPE<VAL32>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r5x) ,SIZE_OF<VAL32>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r6x = mTextReader.poll (TYPE<VAL64>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r6x) ,SIZE_OF<VAL64>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r7x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r7x >= 0) ;
			const auto r8x = BYTE (r7x) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r8x) ,SIZE_OF<BYTE>::expr) ;
			element.mPlyIndex += SIZE_OF<BYTE>::expr ;
			mTextReader >> GAP ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = LENGTH (r8x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r9x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r9x >= 0) ;
			const auto r10x = WORD (r9x) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r10x) ,SIZE_OF<WORD>::expr) ;
			element.mPlyIndex += SIZE_OF<WORD>::expr ;
			mTextReader >> GAP ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = LENGTH (r9x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r11x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r11x >= 0) ;
			const auto r12x = CHAR (r11x) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r12x) ,SIZE_OF<CHAR>::expr) ;
			element.mPlyIndex += SIZE_OF<CHAR>::expr ;
			mTextReader >> GAP ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = LENGTH (r12x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r13x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r13x >= 0) ;
			const auto r14x = QUAD (r13x) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r14x) ,SIZE_OF<QUAD>::expr) ;
			element.mPlyIndex += SIZE_OF<QUAD>::expr ;
			mTextReader >> GAP ;
		}
	}

	void read_body_text_list (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = element.mLastSize ;
		assume (r2x >= 0) ;
		property.mListSize += r2x ;
		if ifdo (TRUE) {
			const auto r3x = element.mExtIndex + r2x * ply_parser_data_type_size (r1x) ;
			if (element.mExtBuffer.size () >= r3x)
				discard ;
			const auto r4x = inline_max (element.mExtBuffer.size () * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
			element.mExtBuffer.resize (r4x) ;
		}
		const auto r5x = LENGTH (element.mExtIndex) ;
		inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r5x) ,SIZE_OF<LENGTH>::expr) ;
		element.mPlyIndex += SIZE_OF<LENGTH>::expr ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r6x = mTextReader.poll (TYPE<VAL32>::expr) ;
				inline_memcpy (Pointer::from (element.mExtBuffer[element.mExtIndex]) ,Pointer::from (r6x) ,SIZE_OF<VAL32>::expr) ;
				element.mExtIndex += SIZE_OF<VAL32>::expr ;
				mTextReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r7x = mTextReader.poll (TYPE<VAL64>::expr) ;
				inline_memcpy (Pointer::from (element.mExtBuffer[element.mExtIndex]) ,Pointer::from (r7x) ,SIZE_OF<VAL64>::expr) ;
				element.mExtIndex += SIZE_OF<VAL64>::expr ;
				mTextReader >> GAP ;
			}
		}
		const auto r8x = LENGTH (element.mExtIndex) ;
		inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r8x) ,SIZE_OF<LENGTH>::expr) ;
		element.mPlyIndex += SIZE_OF<LENGTH>::expr ;
	}

	void read_body_byte () {
		mByteReader = ByteReader (mStream.share ()) ;
		mByteReader.reset (mBodyBackup) ;
		if ifdo (TRUE) {
			if (!mDiffEndianFlag)
				discard ;
			mByteReader >> BOM ;
		}
		for (auto &&i : iter (0 ,mElementList.length ())) {
			for (auto &&j : iter (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : iter (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_byte_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_byte_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mByteReader >> EOS ;
	}

	void read_body_byte_item (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mByteReader.poll (TYPE<FLT32>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r2x) ,SIZE_OF<FLT32>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r3x = mByteReader.poll (TYPE<FLT64>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r3x) ,SIZE_OF<FLT64>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r4x = mByteReader.poll (TYPE<BOOL>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r4x) ,SIZE_OF<BOOL>::expr) ;
			element.mPlyIndex += SIZE_OF<BOOL>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r5x = mByteReader.poll (TYPE<VAL32>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r5x) ,SIZE_OF<VAL32>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r6x = mByteReader.poll (TYPE<VAL64>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r6x) ,SIZE_OF<VAL64>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r7x = mByteReader.poll (TYPE<BYTE>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r7x) ,SIZE_OF<BYTE>::expr) ;
			element.mPlyIndex += SIZE_OF<BYTE>::expr ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = LENGTH (r7x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r8x = mByteReader.poll (TYPE<WORD>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r8x) ,SIZE_OF<WORD>::expr) ;
			element.mPlyIndex += SIZE_OF<WORD>::expr ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = LENGTH (r8x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r9x = mByteReader.poll (TYPE<CHAR>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r9x) ,SIZE_OF<CHAR>::expr) ;
			element.mPlyIndex += SIZE_OF<CHAR>::expr ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = LENGTH (r9x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r10x = mByteReader.poll (TYPE<QUAD>::expr) ;
			inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r10x) ,SIZE_OF<QUAD>::expr) ;
			element.mPlyIndex += SIZE_OF<QUAD>::expr ;
		}
	}

	void read_body_byte_list (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = element.mLastSize ;
		assume (r2x >= 0) ;
		property.mListSize += r2x ;
		if ifdo (TRUE) {
			const auto r3x = element.mExtIndex + r2x * ply_parser_data_type_size (r1x) ;
			if (element.mExtBuffer.size () >= r3x)
				discard ;
			const auto r4x = inline_max (element.mExtBuffer.size () * 2 ,ALLOCATOR_MIN_SIZE::expr) ;
			element.mExtBuffer.resize (r4x) ;
		}
		const auto r5x = LENGTH (element.mExtIndex) ;
		inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r5x) ,SIZE_OF<LENGTH>::expr) ;
		element.mPlyIndex += SIZE_OF<LENGTH>::expr ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r6x = mByteReader.poll (TYPE<VAL32>::expr) ;
				inline_memcpy (Pointer::from (element.mExtBuffer[element.mExtIndex]) ,Pointer::from (r6x) ,SIZE_OF<VAL32>::expr) ;
				element.mExtIndex += SIZE_OF<VAL32>::expr ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r7x = mByteReader.poll (TYPE<VAL64>::expr) ;
				inline_memcpy (Pointer::from (element.mExtBuffer[element.mExtIndex]) ,Pointer::from (r7x) ,SIZE_OF<VAL64>::expr) ;
				element.mExtIndex += SIZE_OF<VAL64>::expr ;
			}
		}
		const auto r8x = LENGTH (element.mExtIndex) ;
		inline_memcpy (Pointer::from (element.mPlyBuffer[element.mPlyIndex]) ,Pointer::from (r8x) ,SIZE_OF<LENGTH>::expr) ;
		element.mPlyIndex += SIZE_OF<LENGTH>::expr ;
	}
} ;

class PlyParserImplHolder final implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakePlyParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		self.mThis = UniqueRef<PlyParserTree>::make (rax.poll ()) ;
		self.mGuide.mElement = NONE ;
	}

	LENGTH element_size (CREF<Slice> element) const override {
		INDEX ix = self.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		return self.mThis->mElementList[ix].mLineSize ;
	}

	LENGTH property_size (CREF<Slice> element ,CREF<Slice> property) const override {
		INDEX ix = self.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		INDEX jx = self.mThis->mElementList[ix].mPropertySet.map (property) ;
		if (jx == NONE)
			return 0 ;
		return self.mThis->mElementList[ix].mPropertyList[jx].mListSize ;
	}

	void guide_new (CREF<Slice> element) override {
		INDEX ix = self.mThis->mElementSet.map (element) ;
		assume (ix != NONE) ;
		self.mGuide.mElement = ix ;
		self.mGuide.mProperty.clear () ;
		self.mGuide.mCol = 0 ;
		self.mGuide.mRow = NONE ;
		self.mGuide.mPlyBegin = 0 ;
		self.mGuide.mPlyEnd = 0 ;
		self.mGuide.mPlyIndex = 0 ;
		self.mGuide.mPlyListMode = FALSE ;
	}

	void guide_put (CREF<Slice> property) override {
		INDEX ix = self.mGuide.mElement ;
		assume (ix != NONE) ;
		INDEX jx = self.mThis->mElementList[ix].mPropertySet.map (property) ;
		assume (jx != NONE) ;
		assert (self.mGuide.mRow == NONE) ;
		self.mGuide.mProperty.add (jx) ;
	}

	void guide_jmp () {
		assert (self.mGuide.mElement != NONE) ;
		INDEX ix = self.mGuide.mElement ;
		auto &&rax = self.mThis->mElementList[ix] ;
		INDEX jx = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mGuide.mRow != NONE)
				discard ;
			self.mGuide.mRow = 0 ;
			self.mGuide.mCol = 0 ;
			assume (self.mGuide.mCol < self.mGuide.mProperty.length ()) ;
			assume (self.mGuide.mRow < rax.mLineSize) ;
			jx = self.mGuide.mProperty[self.mGuide.mCol] ;
			const auto r1x = self.mGuide.mRow * rax.mLineStep ;
			self.mGuide.mPlyBegin = r1x + rax.mPropertyList[jx].mPlyBegin ;
			self.mGuide.mPlyEnd = r1x + rax.mPropertyList[jx].mPlyEnd ;
			self.mGuide.mPlyIndex = self.mGuide.mPlyBegin ;
			self.mGuide.mPlyListMode = FALSE ;
		}
		if ifdo (act) {
			jx = self.mGuide.mProperty[self.mGuide.mCol] ;
			if ifdo (TRUE) {
				if (self.mGuide.mPlyListMode)
					discard ;
				if (rax.mPropertyList[jx].mListType == PlyParserDataType::Null)
					discard ;
				self.mGuide.mPlyBegin = bitwise[TYPE<LENGTH>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
				self.mGuide.mPlyIndex += SIZE_OF<LENGTH>::expr ;
				self.mGuide.mPlyEnd = bitwise[TYPE<LENGTH>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
				self.mGuide.mPlyIndex = self.mGuide.mPlyBegin ;
				self.mGuide.mPlyListMode = TRUE ;
			}
			if (self.mGuide.mPlyIndex >= self.mGuide.mPlyEnd)
				discard ;
		}
		if ifdo (act) {
			self.mGuide.mCol++ ;
			if (self.mGuide.mCol >= self.mGuide.mProperty.length ())
				discard ;
			jx = self.mGuide.mProperty[self.mGuide.mCol] ;
			const auto r2x = self.mGuide.mRow * rax.mLineStep ;
			self.mGuide.mPlyBegin = r2x + rax.mPropertyList[jx].mPlyBegin ;
			self.mGuide.mPlyEnd = r2x + rax.mPropertyList[jx].mPlyEnd ;
			self.mGuide.mPlyIndex = self.mGuide.mPlyBegin ;
			self.mGuide.mPlyListMode = FALSE ;
		}
		if ifdo (act) {
			self.mGuide.mRow++ ;
			self.mGuide.mCol = 0 ;
			if (self.mGuide.mRow >= rax.mLineSize)
				discard ;
			jx = self.mGuide.mProperty[self.mGuide.mCol] ;
			const auto r3x = self.mGuide.mRow * rax.mLineStep ;
			self.mGuide.mPlyBegin = r3x + rax.mPropertyList[jx].mPlyBegin ;
			self.mGuide.mPlyEnd = r3x + rax.mPropertyList[jx].mPlyEnd ;
			self.mGuide.mPlyIndex = self.mGuide.mPlyBegin ;
			self.mGuide.mPlyListMode = FALSE ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void read (VREF<BOOL> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Bool) ;
		item = bitwise[TYPE<BOOL>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<BOOL>::expr ;
	}

	void read (VREF<VAL32> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mGuide.mPlyListMode)
				discard ;
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mType == PlyParserDataType::Val32) ;
			item = bitwise[TYPE<VAL32>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
			self.mGuide.mPlyIndex += SIZE_OF<VAL32>::expr ;
		}
		if ifdo (act) {
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mListType == PlyParserDataType::Val32) ;
			item = bitwise[TYPE<VAL32>::expr] (Pointer::from (rax.mExtBuffer[self.mGuide.mPlyIndex])) ;
			self.mGuide.mPlyIndex += SIZE_OF<VAL32>::expr ;
		}
	}

	void read (VREF<VAL64> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mGuide.mPlyListMode)
				discard ;
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mType == PlyParserDataType::Val64) ;
			item = bitwise[TYPE<VAL64>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
			self.mGuide.mPlyIndex += SIZE_OF<VAL64>::expr ;
		}
		if ifdo (act) {
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mListType == PlyParserDataType::Val64) ;
			item = bitwise[TYPE<VAL64>::expr] (Pointer::from (rax.mExtBuffer[self.mGuide.mPlyIndex])) ;
			self.mGuide.mPlyIndex += SIZE_OF<VAL64>::expr ;
		}
	}

	void read (VREF<FLT32> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Flt32) ;
		item = bitwise[TYPE<FLT32>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<FLT32>::expr ;
	}

	void read (VREF<FLT64> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Flt64) ;
		item = bitwise[TYPE<FLT64>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<FLT64>::expr ;
	}

	void read (VREF<BYTE> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Byte) ;
		item = bitwise[TYPE<BYTE>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<BYTE>::expr ;
	}

	void read (VREF<WORD> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Word) ;
		item = bitwise[TYPE<WORD>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<WORD>::expr ;
	}

	void read (VREF<CHAR> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Char) ;
		item = bitwise[TYPE<CHAR>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<CHAR>::expr ;
	}

	void read (VREF<QUAD> item) override {
		guide_jmp () ;
		INDEX ix = self.mGuide.mElement ;
		INDEX jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Quad) ;
		item = bitwise[TYPE<QUAD>::expr] (Pointer::from (rax.mPlyBuffer[self.mGuide.mPlyIndex])) ;
		self.mGuide.mPlyIndex += SIZE_OF<QUAD>::expr ;
	}
} ;

exports VFat<PlyParserHolder> PlyParserHolder::hold (VREF<PlyParserLayout> that) {
	return VFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::hold (CREF<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}
} ;