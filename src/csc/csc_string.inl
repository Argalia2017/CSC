#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_string.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <clocale>
#include "csc_begin.h"

namespace CSC {
#ifdef __CSC_SYSTEM_WINDOWS__
using csc_locale_t = _locale_t ;

struct FUNCTION_string_locale {
	forceinline UniqueRef<csc_locale_t> operator() () const {
		return UniqueRef<csc_locale_t> ([&] (VR<csc_locale_t> me) {
			me = _create_locale (LC_CTYPE ,String<Stra>::zero ()) ;
		} ,[&] (VR<csc_locale_t> me) {
			_free_locale (me) ;
		}) ;
	}
} ;

struct FUNCTION_string_stra_from_strw {
	forceinline void operator() (VR<String<Stra>> dst ,CR<String<Strw>> src ,CR<csc_locale_t> loc) const {
		auto rax = csc_size_t (0) ;
		const auto r1x = _wcstombs_s_l ((&rax) ,dst ,dst.size () ,src ,_TRUNCATE ,loc) ;
		assume (r1x == 0) ;
		dst.trunc (rax) ;
	}
} ;

struct FUNCTION_string_strw_from_stra {
	forceinline void operator() (VR<String<Strw>> dst ,CR<String<Stra>> src ,CR<csc_locale_t> loc) const {
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
		return UniqueRef<csc_locale_t> ([&] (VR<csc_locale_t> me) {
			me = newlocale (LC_CTYPE_MASK ,String<Stra>::zero () ,NULL) ;
		} ,[&] (VR<csc_locale_t> me) {
			freelocale (me) ;
		}) ;
	}
} ;

struct FUNCTION_string_stra_from_strw {
	forceinline void operator() (VR<String<Stra>> dst ,CR<String<Strw>> src ,CR<csc_locale_t> loc) const {
		auto rax = mbstate_t () ;
		inline_memset (rax) ;
		uselocale (loc) ;
		auto rbx = src.ref ;
		const auto r1x = Length (wcsrtombs (dst ,(&rbx) ,dst.size () ,(&rax))) ;
		assume (r1x >= 0) ;
		dst.trunc (r1x) ;
	}
} ;

struct FUNCTION_string_strw_from_stra {
	forceinline void operator() (VR<String<Strw>> dst ,CR<String<Stra>> src ,CR<csc_locale_t> loc) const {
		auto rax = mbstate_t () ;
		inline_memset (rax) ;
		uselocale (loc) ;
		auto rbx = src.ref ;
		const auto r1x = Length (mbsrtowcs (dst ,(&rbx) ,dst.size () ,(&rax))) ;
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

	String<Stra> stra_from_strw (CR<String<Strw>> a) const override {
		String<Stra> ret = String<Stra> (a.length () * 2 + 1) ;
		string_stra_from_strw (ret ,a ,self.mStringLocale) ;
		return move (ret) ;
	}

	String<Strw> strw_from_stra (CR<String<Stra>> a) const override {
		String<Strw> ret = String<Strw> (a.length () + 1) ;
		string_strw_from_stra (ret ,a ,self.mStringLocale) ;
		return move (ret) ;
	}

	String<Stru> stru8_from_stru16 (CR<String<Stru16>> a) const override {
		String<Stru> ret = String<Stru> (a.length () * 3) ;
		Index ix = 0 ;
		auto rax = Flag (0) ;
		auto rbx = Char () ;
		for (auto &&i : a.iter ()) {
			const auto r1x = Char (a[i]) ;
			if (rax == Flag (99))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X007F))
					discard ;
				ret[ix] = Stru (r1x) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X07FF))
					discard ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X1F)) | Char (0XC0)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x < Char (0XD800))
					discard ;
				if (r1x > Char (0XDBFF))
					discard ;
				rbx = Char (r1x & Char (0X03FF)) ;
				rax = Flag (1) ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				ret[ix] = Stru (((r1x >> 12) & Char (0X0F)) | Char (0XE0)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (1))
					discard ;
				if (r1x < Char (0XDC00))
					discard ;
				if (r1x > Char (0XDFFF))
					discard ;
				rbx = Char ((rbx << 10) | (r1x & Char (0X03FF))) ;
				rbx = Char (Val32 (rbx) + Val32 (0X00010000)) ;
				ret[ix] = Stru (((rbx >> 18) & Char (0X07)) | Char (0XF0)) ;
				ix++ ;
				ret[ix] = Stru (((rbx >> 12) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((rbx >> 6) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru ((rbx & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				rax = Flag (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = Flag (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == Flag (0))
				discard ;
			ret[ix] = Stru (Char (0X3F)) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<Stru> stru8_from_stru32 (CR<String<Stru32>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<Stru> ret = String<Stru> (a.length () * 6) ;
		Index ix = 0 ;
		auto rax = Flag (0) ;
		auto rbx = Char () ;
		for (auto &&i : a.iter ()) {
			const auto r1x = Char (a[i]) ;
			if (rax == Flag (99))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X0000007F))
					discard ;
				ret[ix] = Stru (r1x) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X000007FF))
					discard ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X1F)) | Char (0XC0)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X0000FFFF))
					discard ;
				ret[ix] = Stru (((r1x >> 12) & Char (0X0F)) | Char (0XE0)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X001FFFFF))
					discard ;
				ret[ix] = Stru (((r1x >> 18) & Char (0X07)) | Char (0XF0)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 12) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X03FFFFFF))
					discard ;
				ret[ix] = Stru (((r1x >> 24) & Char (0X03)) | Char (0XF8)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 18) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 12) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X7FFFFFFF))
					discard ;
				ret[ix] = Stru (((r1x >> 30) & Char (0X01)) | Char (0XFC)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 24) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 18) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 12) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru (((r1x >> 6) & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
				ret[ix] = Stru ((r1x & Char (0X3F)) | Char (0X80)) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = Flag (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<Stru16> stru16_from_stru8 (CR<String<Stru>> a) const override {
		String<Stru16> ret = String<Stru16> (a.length ()) ;
		Index ix = 0 ;
		auto rax = Flag (0) ;
		auto rbx = Char () ;
		for (auto &&i : a.iter ()) {
			const auto r1x = Char (a[i]) ;
			if (rax == Flag (99))
				break ;
			if ifdo (TRUE) {
				auto act = TRUE ;
				if ifdo (act) {
					if (rax != Flag (0))
						discard ;
					if (r1x > Char (0X7F))
						discard ;
					ret[ix] = Stru16 (r1x) ;
					ix++ ;
				}
				if ifdo (act) {
					if (rax != Flag (0))
						discard ;
					if (r1x > Char (0XDF))
						discard ;
					rbx = Char (r1x & Char (0X1F)) ;
					rax = Flag (1) ;
				}
				if ifdo (act) {
					if (rax != Flag (0))
						discard ;
					if (r1x > Char (0XEF))
						discard ;
					rbx = Char (r1x & Char (0X0F)) ;
					rax = Flag (2) ;
				}
				if ifdo (act) {
					if (rax != Flag (0))
						discard ;
					if (r1x > Char (0XF7))
						discard ;
					rbx = Char (r1x & Char (0X07)) ;
					rax = Flag (3) ;
				}
				if ifdo (act) {
					if (rax != Flag (0))
						discard ;
					if (r1x > Char (0XFB))
						discard ;
					rbx = Char (r1x & Char (0X03)) ;
					rax = Flag (4) ;
				}
				if ifdo (act) {
					if (rax != Flag (0))
						discard ;
					if (r1x > Char (0XFD))
						discard ;
					rbx = Char (r1x & Char (0X01)) ;
					rax = Flag (5) ;
				}
				if ifdo (act) {
					if (rax != Flag (1))
						discard ;
					if (r1x > Char (0XBF))
						discard ;
					rbx = Char ((rbx << 6) | (r1x & Char (0X3F))) ;
					rax = Flag (10) ;
				}
				if ifdo (act) {
					if (!inline_between (rax ,2 ,6))
						discard ;
					if (r1x > Char (0XBF))
						discard ;
					rbx = Char ((rbx << 6) | (r1x & Char (0X3F))) ;
					rax-- ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = Flag (99) ;
				}
			}
			if ifdo (TRUE) {
				if (rax != Flag (10))
					discard ;
				auto act = TRUE ;
				if ifdo (act) {
					if (rbx > Char (0X0000FFFF))
						discard ;
					ret[ix] = Stru16 (rbx) ;
					ix++ ;
					rax = Flag (0) ;
				}
				if ifdo (act) {
					if (rbx > Char (0X0010FFFF))
						discard ;
					rbx = Char (Val32 (rbx) - Val32 (0X00010000)) ;
					ret[ix] = Stru16 (((rbx >> 10) & Char (0X03FF)) | Char (0XD800)) ;
					ix++ ;
					ret[ix] = Stru16 ((rbx & Char (0X03FF)) | Char (0XDC00)) ;
					ix++ ;
					rax = Flag (0) ;
				}
				if ifdo (act) {
					if (rbx > Char (0X7FFFFFFF))
						discard ;
					ret[ix] = Stru16 (Char (0X3F)) ;
					ix++ ;
					rax = Flag (0) ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = Flag (99) ;
				}
			}
		}
		if ifdo (TRUE) {
			if (rax == Flag (0))
				discard ;
			ret[ix] = Stru16 (Char (0X3F)) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<Stru16> stru16_from_stru32 (CR<String<Stru32>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<Stru16> ret = String<Stru16> (a.length () * 2) ;
		Index ix = 0 ;
		auto rax = Flag (0) ;
		auto rbx = Char () ;
		for (auto &&i : a.iter ()) {
			const auto r1x = Char (a[i]) ;
			if (rax == Flag (99))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X0000FFFF))
					discard ;
				ret[ix] = Stru16 (r1x) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X0010FFFF))
					discard ;
				rbx = Char (Val32 (r1x) - Val32 (0X00010000)) ;
				ret[ix] = Stru16 (((rbx >> 10) & Char (0X03FF)) | Char (0XD800)) ;
				ix++ ;
				ret[ix] = Stru16 ((rbx & Char (0X03FF)) | Char (0XDC00)) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X7FFFFFFF))
					discard ;
				ret[ix] = Stru16 (Char (0X3F)) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = Flag (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<Stru32> stru32_from_stru8 (CR<String<Stru>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<Stru32> ret = String<Stru32> (a.length ()) ;
		Index ix = 0 ;
		auto rax = Flag (0) ;
		auto rbx = Char () ;
		for (auto &&i : a.iter ()) {
			const auto r1x = Char (a[i]) ;
			if (rax == Flag (99))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X7F))
					discard ;
				ret[ix] = Stru32 (r1x) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0XDF))
					discard ;
				rbx = Char (r1x & Char (0X1F)) ;
				rax = Flag (1) ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0XEF))
					discard ;
				rbx = Char (r1x & Char (0X0F)) ;
				rax = Flag (2) ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0XF7))
					discard ;
				rbx = Char (r1x & Char (0X07)) ;
				rax = Flag (3) ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0XFB))
					discard ;
				rbx = Char (r1x & Char (0X03)) ;
				rax = Flag (4) ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0XFD))
					discard ;
				rbx = Char (r1x & Char (0X01)) ;
				rax = Flag (5) ;
			}
			if ifdo (act) {
				if (rax != Flag (1))
					discard ;
				if (r1x > Char (0XBF))
					discard ;
				rbx = Char ((rbx << 6) | (r1x & Char (0X3F))) ;
				ret[ix] = Stru32 (rbx) ;
				ix++ ;
				rax = Flag (0) ;
			}
			if ifdo (act) {
				if (!inline_between (rax ,2 ,6))
					discard ;
				if (r1x > Char (0XBF))
					discard ;
				rbx = Char ((rbx << 6) | (r1x & Char (0X3F))) ;
				rax-- ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = Flag (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == Flag (0))
				discard ;
			ret[ix] = Stru32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<Stru32> stru32_from_stru16 (CR<String<Stru16>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<Stru32> ret = String<Stru32> (a.length ()) ;
		Index ix = 0 ;
		auto rax = Flag (0) ;
		auto rbx = Char () ;
		for (auto &&i : a.iter ()) {
			const auto r1x = Char (a[i]) ;
			if (rax == Flag (99))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x > Char (0X07FF))
					discard ;
				ret[ix] = Stru32 (r1x) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (r1x < Char (0XD800))
					discard ;
				if (r1x > Char (0XDBFF))
					discard ;
				rbx = Char (r1x & Char (0X03FF)) ;
				rax = Flag (1) ;
			}
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				ret[ix] = Stru32 (r1x) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != Flag (1))
					discard ;
				if (r1x < Char (0XDC00))
					discard ;
				if (r1x > Char (0XDFFF))
					discard ;
				rbx = Char ((rbx << 10) | (r1x & Char (0X03FF))) ;
				rbx = Char (Val32 (rbx) + Val32 (0X00010000)) ;
				ret[ix] = Stru32 (rbx) ;
				ix++ ;
				rax = Flag (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = Flag (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == Flag (0))
				discard ;
			ret[ix] = Stru32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<Str> strs_from_straw (RR<String<Stra>> a) const {
		assert (a.step () == SIZE_OF<Str>::expr) ;
		return move (keep[TYPE<String<Str>>::expr] (Pointer::from (a))) ;
	}

	String<Str> strs_from_straw (RR<String<Strw>> a) const {
		assert (a.step () == SIZE_OF<Str>::expr) ;
		return move (keep[TYPE<String<Str>>::expr] (Pointer::from (a))) ;
	}

	String<Strw> strw_from_struw (RR<String<Stru>> a) const {
		assert (a.step () == SIZE_OF<Strw>::expr) ;
		return move (keep[TYPE<String<Strw>>::expr] (Pointer::from (a))) ;
	}

	String<Strw> strw_from_struw (RR<String<Stru16>> a) const {
		assert (a.step () == SIZE_OF<Strw>::expr) ;
		return move (keep[TYPE<String<Strw>>::expr] (Pointer::from (a))) ;
	}

	String<Strw> strw_from_struw (RR<String<Stru32>> a) const {
		assert (a.step () == SIZE_OF<Strw>::expr) ;
		return move (keep[TYPE<String<Strw>>::expr] (Pointer::from (a))) ;
	}

	String<Stra> stra_from (CR<String<Stra>> a) const override {
		return a ;
	}

	String<Stra> stra_from (CR<String<Strw>> a) const override {
		return stra_from_strw (a) ;
	}

	String<Stra> stra_from (CR<String<Stru>> a) const override {
		return stra_from_strw (strw_from (a)) ;
	}

	String<Stra> stra_from (CR<String<Stru16>> a) const override {
		return stra_from_strw (strw_from (a)) ;
	}

	String<Stra> stra_from (CR<String<Stru32>> a) const override {
		return stra_from_strw (strw_from (a)) ;
	}

	String<Strw> strw_from (CR<String<Stra>> a) const override {
		return strw_from_stra (a) ;
	}

	String<Strw> strw_from (CR<String<Strw>> a) const override {
		return a ;
	}

	String<Strw> strw_from (CR<String<Stru>> a) const override {
		if (SIZE_OF<Strw>::expr == 1)
			return strw_from_struw (move (a)) ;
		if (SIZE_OF<Strw>::expr == 2)
			return strw_from_struw (stru16_from_stru8 (a)) ;
		if (SIZE_OF<Strw>::expr == 4)
			return strw_from_struw (stru32_from_stru8 (a)) ;
		assert (FALSE) ;
		return String<Strw> () ;
	}

	String<Strw> strw_from (CR<String<Stru16>> a) const override {
		if (SIZE_OF<Strw>::expr == 1)
			return strw_from_struw (stru8_from_stru16 (a)) ;
		if (SIZE_OF<Strw>::expr == 2)
			return strw_from_struw (move (a)) ;
		if (SIZE_OF<Strw>::expr == 4)
			return strw_from_struw (stru32_from_stru16 (a)) ;
		assert (FALSE) ;
		return String<Strw> () ;
	}

	String<Strw> strw_from (CR<String<Stru32>> a) const override {
		if (SIZE_OF<Strw>::expr == 1)
			return strw_from_struw (stru8_from_stru32 (a)) ;
		if (SIZE_OF<Strw>::expr == 2)
			return strw_from_struw (stru16_from_stru32 (a)) ;
		if (SIZE_OF<Strw>::expr == 4)
			return strw_from_struw (move (a)) ;
		assert (FALSE) ;
		return String<Strw> () ;
	}

	String<Str> strs_from (CR<String<Stra>> a) const override {
		if (IS_SAME<Str ,Stra>::expr)
			return strs_from_straw (move (a)) ;
		if (IS_SAME<Str ,Strw>::expr)
			return strs_from_straw (strw_from_stra (a)) ;
		assert (FALSE) ;
		return String<Str> () ;
	}

	String<Str> strs_from (CR<String<Strw>> a) const override {
		if (IS_SAME<Str ,Stra>::expr)
			return strs_from_straw (stra_from_strw (a)) ;
		if (IS_SAME<Str ,Strw>::expr)
			return strs_from_straw (move (a)) ;
		assert (FALSE) ;
		return String<Str> () ;
	}

	String<Str> strs_from (CR<String<Stru>> a) const override {
		return strs_from_straw (strw_from (a)) ;
	}

	String<Str> strs_from (CR<String<Stru16>> a) const override {
		return strs_from_straw (strw_from (a)) ;
	}

	String<Str> strs_from (CR<String<Stru32>> a) const override {
		return strs_from_straw (strw_from (a)) ;
	}

	String<Stru> stru8_from (CR<String<Stra>> a) const override {
		return stru8_from (strw_from_stra (a)) ;
	}

	String<Stru> stru8_from (CR<String<Strw>> a) const override {
		if (SIZE_OF<Strw>::expr == 1)
			return keep[TYPE<String<Stru>>::expr] (Pointer::from (a)) ;
		if (SIZE_OF<Strw>::expr == 2)
			return stru8_from_stru16 (Pointer::from (a)) ;
		if (SIZE_OF<Strw>::expr == 4)
			return stru8_from_stru32 (Pointer::from (a)) ;
		assert (FALSE) ;
		return String<Stru> () ;
	}

	String<Stru> stru8_from (CR<String<Stru>> a) const override {
		return a ;
	}

	String<Stru> stru8_from (CR<String<Stru16>> a) const override {
		return stru8_from_stru16 (a) ;
	}

	String<Stru> stru8_from (CR<String<Stru32>> a) const override {
		return stru8_from_stru32 (a) ;
	}

	String<Stru16> stru16_from (CR<String<Stra>> a) const override {
		return stru16_from (strw_from_stra (a)) ;
	}

	String<Stru16> stru16_from (CR<String<Strw>> a) const override {
		if (SIZE_OF<Strw>::expr == 1)
			return stru16_from_stru8 (Pointer::from (a)) ;
		if (SIZE_OF<Strw>::expr == 2)
			return keep[TYPE<String<Stru16>>::expr] (Pointer::from (a)) ;
		if (SIZE_OF<Strw>::expr == 4)
			return stru16_from_stru32 (Pointer::from (a)) ;
		assert (FALSE) ;
		return String<Stru16> () ;
	}

	String<Stru16> stru16_from (CR<String<Stru>> a) const override {
		return stru16_from_stru8 (a) ;
	}

	String<Stru16> stru16_from (CR<String<Stru16>> a) const override {
		return a ;
	}

	String<Stru16> stru16_from (CR<String<Stru32>> a) const override {
		return stru16_from_stru32 (a) ;
	}

	String<Stru32> stru32_from (CR<String<Stra>> a) const override {
		return stru32_from (strw_from_stra (a)) ;
	}

	String<Stru32> stru32_from (CR<String<Strw>> a) const override {
		if (SIZE_OF<Strw>::expr == 1)
			return stru32_from_stru8 (Pointer::from (a)) ;
		if (SIZE_OF<Strw>::expr == 2)
			return stru32_from_stru16 (Pointer::from (a)) ;
		if (SIZE_OF<Strw>::expr == 4)
			return keep[TYPE<String<Stru32>>::expr] (Pointer::from (a)) ;
		assert (FALSE) ;
		return String<Stru32> () ;
	}

	String<Stru32> stru32_from (CR<String<Stru>> a) const override {
		return stru32_from_stru8 (a) ;
	}

	String<Stru32> stru32_from (CR<String<Stru16>> a) const override {
		return stru32_from_stru16 (a) ;
	}

	String<Stru32> stru32_from (CR<String<Stru32>> a) const override {
		return a ;
	}
} ;

exports CR<Super<Ref<StringProcLayout>>> StringProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<StringProcLayout>> ret ;
		ret.mThis = Ref<StringProcLayout>::make () ;
		StringProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StringProcHolder> StringProcHolder::hold (VR<StringProcLayout> that) {
	return VFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

exports CFat<StringProcHolder> StringProcHolder::hold (CR<StringProcLayout> that) {
	return CFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

struct RegularReaderLayout {
	Ref<RefBuffer<Byte>> mStream ;
	StreamShape mBackup ;
	Box<TextReader> mTextReader ;
	Deque<Stru32> mDeque ;
	Stru32 mTop ;
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

	explicit RegularReader (RR<Ref<RefBuffer<Byte>>> stream ,CR<Length> ring_size) {
		mStream = move (stream) ;
		mBackup.mRead = 0 ;
		mBackup.mWrite = mStream->size () ;
		mDeque = Deque<Stru32> (ring_size) ;
		cache () ;
	}

	void cache () {
		if ifdo (TRUE) {
			if (mTextReader != NULL)
				discard ;
			mTextReader = Box<TextReader>::make (mStream.share ()) ;
			mTextReader->reset (mBackup) ;
		}
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.full ())
				break ;
			mTextReader.ref >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	void get (CR<Index> index ,VR<Stru32> item) const {
		item = mDeque[index] ;
	}

	forceinline Stru32 operator[] (CR<Index> index) const {
		Stru32 ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	template <class ARG1>
	void read (XR<ARG1> item) {
		mTextReader->reset (mBackup) ;
		mTextReader.ref >> item ;
		mBackup = mTextReader->shape () ;
		cache () ;
	}

	template <class ARG1>
	forceinline VR<RegularReader> operator>> (XR<ARG1> item) {
		read (item) ;
		return thiz ;
	}

	void next () {
		mTextReader.ref >> mTop ;
		if ifdo (TRUE) {
			if (mTop == Stru32 (0X00))
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

struct ScopeCounterLayout {
	Length mCounter ;

public:
	implicit ScopeCounterLayout () noexcept {
		mCounter = 0 ;
	}
} ;

class ScopeCounter implement ScopeCounterLayout {
private:
	using SCOPECOUNTER_MAX_DEPTH = ENUM<256> ;

protected:
	using ScopeCounterLayout::mCounter ;

public:
	implicit ScopeCounter () = default ;

	void enter () {
		mCounter++ ;
		assume (mCounter < SCOPECOUNTER_MAX_DEPTH::expr) ;
	}

	void leave () {
		mCounter-- ;
		assume (mCounter >= ZERO) ;
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
	String<Stru> mName ;
	String<Stru> mValue ;
	SortedMap<Index> mArrayMap ;
	SortedMap<String<Stru>> mObjectMap ;
	Index mMember ;
	Just<XmlParserNodeType> mType ;
	Index mParent ;
	Index mBrother ;
	Index mChild ;
} ;

struct XmlParserTree {
	Array<XmlParserNode> mTree ;
	Index mRoot ;
} ;

struct MakeXmlParserLayout {
	RegularReader mReader ;
	ScopeCounter mScopeCounter ;
	List<XmlParserNode> mTree ;
	SortedMap<Index> mArrayMap ;
	List<Index> mArrayMemberList ;
	SortedMap<String<Stru>> mObjectMap ;
	List<Set<String<Stru>>> mObjectMemberList ;
	Index mLastIndex ;
	String<Stru> mLastString ;
} ;

class MakeXmlParser implement MakeXmlParserLayout {
protected:
	using MakeXmlParserLayout::mReader ;
	using MakeXmlParserLayout::mScopeCounter ;
	using MakeXmlParserLayout::mTree ;
	using MakeXmlParserLayout::mArrayMap ;
	using MakeXmlParserLayout::mObjectMap ;
	using MakeXmlParserLayout::mLastIndex ;
	using MakeXmlParserLayout::mLastString ;

public:
	implicit MakeXmlParser () = default ;

	explicit MakeXmlParser (RR<Ref<RefBuffer<Byte>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mArrayMap = SortedMap<Index> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<Stru>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	XmlParserTree poll () {
		XmlParserTree ret ;
		ret.mTree = Array<XmlParserNode> (mTree.length ()) ;
		const auto r1x = Array<Index>::make (mTree.iter ()) ;
		for (auto &&i : ret.mTree.iter ()) {
			ret.mTree[i] = move (mTree[r1x[i]]) ;
			const auto r2x = ret.mTree[i].mArrayMap.length () ;
			ret.mTree[i].mArrayMap.remap () ;
			assume (ret.mTree[i].mArrayMap.length () == r2x) ;
			ret.mTree[i].mObjectMap.remap () ;
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
		mReader >> ReadKeyword (mLastString) ;
	}

	//@info: $2->"${escape}"
	void read_shift_e2 () {
		mReader >> ReadEscape (mLastString) ;
	}

	//@info: $3->$1 = $2
	void read_shift_e3 (CR<Index> curr) {
		Index ix = mTree.insert () ;
		read_shift_e1 () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mMember = NONE ;
		mTree[ix].mType = XmlParserNodeType::Value ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		read_shift_e2 () ;
		mTree[ix].mValue = move (mLastString) ;
		mLastIndex = ix ;
	}

	//@info: $4->${eps}|$3 $4
	void read_shift_e4 (CR<Index> curr) {
		Index ix = NONE ;
		Index iy = NONE ;
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
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
	void read_shift_e5 (CR<Index> curr) {
		Scope anonymous (mScopeCounter) ;
		mReader >> slice ("<") ;
		Index ix = mTree.insert () ;
		read_shift_e1 () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mObjectMap = mObjectMap.share () ;
		mTree[ix].mMember = mObjectMemberList.insert () ;
		mTree[ix].mType = XmlParserNodeType::Object ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		read_shift_e4 (ix) ;
		Index iy = mLastIndex ;
		mReader >> GAP ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != Stru32 ('>'))
				discard ;
			mReader++ ;
			mReader >> GAP ;
			read_shift_e8 (ix ,iy) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
			mReader >> slice ("</") ;
			read_shift_e1 () ;
			assume (mLastString == mTree[ix].mName) ;
			mReader >> GAP ;
			mReader >> slice (">") ;
		}
		if ifdo (act) {
			mReader >> slice ("/>") ;
		}
		mObjectMemberList.remove (mTree[ix].mMember) ;
		mTree[ix].mMember = NONE ;
		mLastIndex = ix ;
	}

	//@info: $6-><!--${comment}-->
	void read_shift_e6 () {
		mReader >> slice ("<!--") ;
		while (TRUE) {
			if (mReader[0] == Stru32 ('-'))
				if (mReader[1] == Stru32 ('-'))
					if (mReader[2] == Stru32 ('>'))
						break ;
			mReader++ ;
		}
		mReader >> slice ("-->") ;
	}

	//@info: $7->${[^<>]+}
	void read_shift_e7 (CR<Index> curr) {
		auto rax = String<Stru>::make () ;
		Index ix = 0 ;
		while (TRUE) {
			if (mReader[0] == Stru32 ('<'))
				break ;
			if (mReader[0] == Stru32 ('>'))
				break ;
			assume (ix < rax.size ()) ;
			rax[ix] = Stru (mReader[0]) ;
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
			if (mTree[curr].mValue.size () > 0)
				discard ;
			mTree[curr].mValue = move (rax) ;
		}
		mTree[curr].mValue.splice (mTree[curr].mValue.length () ,rax) ;
	}

	//@info: $8->$5 $8|$6 $8|$7 $8
	void read_shift_e8 (CR<Index> curr ,CR<Index> first) {
		Scope anonymous (mScopeCounter) ;
		Index ix = first ;
		Index iy = first ;
		Index kx = mTree[curr].mMember ;
		while (TRUE) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!is_first_of_object ())
					discard ;
				read_shift_e5 (curr) ;
				auto rbx = move (mTree[mLastIndex].mName) ;
				Index jx = mObjectMemberList[kx].map (rbx) ;
				if ifdo (TRUE) {
					if (jx != NONE)
						discard ;
					jx = mTree.insert () ;
					mTree[jx].mName = move (rbx) ;
					mObjectMemberList[kx].add (mTree[jx].mName ,jx) ;
					mTree[jx].mArrayMap = mArrayMap.share () ;
					mTree[jx].mMember = NONE ;
					mTree[jx].mType = XmlParserNodeType::Array ;
					mTree[jx].mParent = curr ;
					mTree[jx].mBrother = NONE ;
					mTree[jx].mChild = NONE ;
					brother_prev (ix ,iy) = jx ;
					iy = jx ;
					mTree[curr].mObjectMap.add (mTree[jx].mName ,iy) ;
				}
				mTree[mLastIndex].mParent = jx ;
				brother_prev (mTree[jx].mChild ,mTree[jx].mMember) = mLastIndex ;
				mTree[jx].mMember = mLastIndex ;
				const auto r1x = mTree[jx].mArrayMap.length () ;
				mTree[jx].mArrayMap.add (r1x ,mLastIndex) ;
			}
			if ifdo (act) {
				if (!is_first_of_comment ())
					discard ;
				read_shift_e6 () ;
			}
			mReader >> GAP ;
			if (mReader[0] == Stru32 ('<'))
				if (mReader[1] == Stru32 ('/'))
					break ;
			if ifdo (act) {
				read_shift_e7 (curr) ;
			}
		}
		if ifdo (TRUE) {
			Index iz = mTree[curr].mChild ;
			while (TRUE) {
				if (iz == NONE)
					break ;
				mTree[iz].mMember = NONE ;
				iz = mTree[iz].mBrother ;
			}
		}
		mLastIndex = ix ;
	}

	VR<Index> brother_prev (VR<Index> prev ,CR<Index> curr) leftvalue {
		if (prev == NONE)
			return prev ;
		return mTree[curr].mBrother ;
	}

	Bool is_first_of_comment () const {
		if (mReader[0] == Stru32 ('<'))
			if (mReader[1] == Stru32 ('!'))
				return TRUE ;
		return FALSE ;
	}

	Bool is_first_of_object () const {
		if (mReader[0] == Stru32 ('<'))
			if (mReader[1] != Stru32 ('/'))
				return TRUE ;
		return FALSE ;
	}

	//@info: $9->${eps}|$5
	void read_shift_e9 () {
		Index ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == Stru32 (0X00))
				discard ;
			read_shift_e5 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
	void read_shift_e10 () {
		mReader >> BOM ;
		if (mReader[0] != Stru32 ('<'))
			return ;
		if (mReader[1] != Stru32 ('?'))
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
			if (mReader[0] == Stru32 ('?'))
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
		assume (mReader[0] == Stru32 (0X00)) ;
	}
} ;

class XmlParserImplHolder final implement Fat<XmlParserHolder ,XmlParserLayout> {
public:
	void initialize (CR<RefBuffer<Byte>> stream) override {
		auto rax = MakeXmlParser (Ref<RefBuffer<Byte>>::reference (stream)) ;
		rax.generate () ;
		self.mThis = UniqueRef<XmlParserTree>::make (rax.poll ()) ;
		self.mIndex = self.mThis->mRoot ;
	}

	Bool exist () const override {
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
			ret.mIndex = self.mThis->mTree[self.mIndex].mParent ;
		}
		return move (ret) ;
	}

	XmlParserLayout brother () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	XmlParserLayout child () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mChild ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CR<Index> index) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CR<Slice> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CR<String<Stru>> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list () const override {
		Array<XmlParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mTree[self.mIndex].mArrayMap.length () ;
			ret = Array<XmlParserLayout> (r1x) ;
			for (auto &&i : range (0 ,r1x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mTree[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list (CR<Length> size_) const override {
		Array<XmlParserLayout> ret = Array<XmlParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mTree[self.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : range (0 ,r2x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mTree[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	Bool equal (CR<XmlParserLayout> that) const override {
		const auto r1x = inline_compr (self.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!self.mThis.exist ())
			return TRUE ;
		if (address (self.mThis->mTree) != address (that.mThis->mTree))
			return FALSE ;
		if (self.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CR<String<Stru>> name () const leftvalue override {
		assert (exist ()) ;
		return self.mThis->mTree[self.mIndex].mName ;
	}

	Bool parse (CR<Bool> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Bool>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Val32 parse (CR<Val32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Val32>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Val64 parse (CR<Val64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Val64>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Flt32 parse (CR<Flt32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Flt32>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Flt64 parse (CR<Flt64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Flt64>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stra> parse (CR<String<Stra>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stra_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Strw> parse (CR<String<Strw>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::strw_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stru> parse (CR<String<Stru>> def) const override {
		if (!exist ())
			return def ;
		try {
			return self.mThis->mTree[self.mIndex].mValue ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stru16> parse (CR<String<Stru16>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru16_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stru32> parse (CR<String<Stru32>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru32_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<Bool> parse (CR<Bool> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Val32> parse (CR<Val32> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Val64> parse (CR<Val64> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Flt32> parse (CR<Flt32> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Flt64> parse (CR<Flt64> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stra>> parse (CR<String<Stra>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Strw>> parse (CR<String<Strw>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stru>> parse (CR<String<Stru>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stru16>> parse (CR<String<Stru16>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stru32>> parse (CR<String<Stru32>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	template <class ARG1>
	forceinline Array<ARG1> parse_impl (CR<ARG1> def ,CR<Length> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.iter ()) {
			const auto r2x = XmlParserHolder::hold (self)->child (i) ;
			ret[i] = XmlParserHolder::hold (r2x)->parse (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<XmlParserHolder> XmlParserHolder::hold (VR<XmlParserLayout> that) {
	return VFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

exports CFat<XmlParserHolder> XmlParserHolder::hold (CR<XmlParserLayout> that) {
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
	String<Stru> mName ;
	String<Stru> mValue ;
	SortedMap<Index> mArrayMap ;
	SortedMap<String<Stru>> mObjectMap ;
	Just<JsonParserNodeType> mType ;
	Index mParent ;
	Index mBrother ;
	Index mChild ;
} ;

struct JsonParserTree {
	Array<JsonParserNode> mTree ;
	Index mRoot ;
} ;

struct MakeJsonParserLayout {
	RegularReader mReader ;
	ScopeCounter mScopeCounter ;
	List<JsonParserNode> mTree ;
	SortedMap<Index> mArrayMap ;
	SortedMap<String<Stru>> mObjectMap ;
	Index mLastIndex ;
	String<Stru> mLastString ;
} ;

class MakeJsonParser implement MakeJsonParserLayout {
protected:
	using MakeJsonParserLayout::mReader ;
	using MakeJsonParserLayout::mScopeCounter ;
	using MakeJsonParserLayout::mTree ;
	using MakeJsonParserLayout::mArrayMap ;
	using MakeJsonParserLayout::mObjectMap ;
	using MakeJsonParserLayout::mLastIndex ;
	using MakeJsonParserLayout::mLastString ;

public:
	implicit MakeJsonParser () = default ;

	explicit MakeJsonParser (RR<Ref<RefBuffer<Byte>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mArrayMap = SortedMap<Index> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<Stru>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	JsonParserTree poll () {
		JsonParserTree ret ;
		ret.mTree = Array<JsonParserNode> (mTree.length ()) ;
		const auto r1x = Array<Index>::make (mTree.iter ()) ;
		for (auto &&i : ret.mTree.iter ()) {
			ret.mTree[i] = move (mTree[r1x[i]]) ;
			const auto r2x = ret.mTree[i].mArrayMap.length () ;
			ret.mTree[i].mArrayMap.remap () ;
			assume (ret.mTree[i].mArrayMap.length () == r2x) ;
			const auto r3x = ret.mTree[i].mObjectMap.length () ;
			ret.mTree[i].mObjectMap.remap () ;
			assume (ret.mTree[i].mObjectMap.length () == r3x) ;
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
		mReader >> ReadScalar (mLastString) ;
	}

	//@info: $2->true|false|null
	void read_shift_e2 () {
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != Stru32 ('t'))
				discard ;
			mReader >> slice ("true") ;
			mLastString = String<Stru>::make (slice ("true")) ;
		}
		if ifdo (act) {
			if (mReader[0] != Stru32 ('f'))
				discard ;
			mReader >> slice ("false") ;
			mLastString = String<Stru>::make (slice ("false")) ;
		}
		if ifdo (act) {
			if (mReader[0] != Stru32 ('n'))
				discard ;
			mReader >> slice ("null") ;
			mLastString = String<Stru> () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	//@info: $3->"${escape}"
	void read_shift_e3 () {
		mReader >> ReadEscape (mLastString) ;
	}

	//@info: $4->$1|$2|$3|$6|$9
	void read_shift_e4 (CR<Index> curr) {
		Scope anonymous (mScopeCounter) ;
		Index ix = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!is_first_of_number ())
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e1 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (!is_first_of_const ())
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e2 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != Stru32 ('\"'))
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e3 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != Stru32 ('['))
				discard ;
			read_shift_e6 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			if (mReader[0] != Stru32 ('{'))
				discard ;
			read_shift_e9 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
		mLastIndex = ix ;
	}

	Bool is_first_of_number () const {
		if (mReader[0] == Stru32 ('+'))
			return TRUE ;
		if (mReader[0] == Stru32 ('-'))
			return TRUE ;
		if (mReader[0] >= Stru32 ('0'))
			if (mReader[0] <= Stru32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	Bool is_first_of_const () const {
		if (mReader[0] == Stru32 ('t'))
			return TRUE ;
		if (mReader[0] == Stru32 ('f'))
			return TRUE ;
		if (mReader[0] == Stru32 ('n'))
			return TRUE ;
		return FALSE ;
	}

	//@info: $5->$4|$4 , $5
	void read_shift_e5 (CR<Index> curr) {
		Index ix = NONE ;
		Index iy = NONE ;
		while (TRUE) {
			read_shift_e4 (curr) ;
			brother_prev (ix ,iy) = mLastIndex ;
			iy = mLastIndex ;
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != Stru32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $6->[ ]|[ $5 ]
	void read_shift_e6 (CR<Index> curr) {
		Scope anonymous (mScopeCounter) ;
		mReader >> slice ("[") ;
		Index ix = mTree.insert () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Array ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == Stru32 (']'))
				break ;
			read_shift_e5 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("]") ;
		mLastIndex = ix ;
	}

	//@info: $7->$3 : $4
	void read_shift_e7 (CR<Index> curr) {
		read_shift_e3 () ;
		mReader >> GAP ;
		mReader >> slice (":") ;
		mReader >> GAP ;
		read_shift_e4 (curr) ;
	}

	//@info: $8->$7|$7 , $8
	void read_shift_e8 (CR<Index> curr) {
		Index ix = NONE ;
		Index iy = NONE ;
		while (TRUE) {
			read_shift_e7 (curr) ;
			brother_prev (ix ,iy) = mLastIndex ;
			iy = mLastIndex ;
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,iy) ;
			mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != Stru32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	VR<Index> brother_prev (VR<Index> prev ,CR<Index> curr) leftvalue {
		if (prev == NONE)
			return prev ;
		return mTree[curr].mBrother ;
	}

	//@info: $9->{ }|{ $8 }
	void read_shift_e9 (CR<Index> curr) {
		Scope anonymous (mScopeCounter) ;
		mReader >> slice ("{") ;
		Index ix = mTree.insert () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mObjectMap = mObjectMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Object ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == Stru32 ('}'))
				discard ;
			read_shift_e8 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("}") ;
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|$4
	void read_shift_e10 () {
		Index ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == Stru32 (0X00))
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
		assume (mReader[0] == Stru32 (0X00)) ;
	}
} ;

class JsonParserImplHolder final implement Fat<JsonParserHolder ,JsonParserLayout> {
public:
	void initialize (CR<RefBuffer<Byte>> stream) override {
		auto rax = MakeJsonParser (Ref<RefBuffer<Byte>>::reference (stream)) ;
		rax.generate () ;
		self.mThis = UniqueRef<JsonParserTree>::make (rax.poll ()) ;
		self.mIndex = self.mThis->mRoot ;
	}

	Bool exist () const override {
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
			ret.mIndex = self.mThis->mTree[self.mIndex].mParent ;
		}
		return move (ret) ;
	}

	JsonParserLayout brother () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	JsonParserLayout child () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mChild ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CR<Index> index) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CR<Slice> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CR<String<Stru>> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = self.mThis ;
			ret.mIndex = self.mThis->mTree[self.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list () const override {
		Array<JsonParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mTree[self.mIndex].mArrayMap.length () ;
			ret = Array<JsonParserLayout> (r1x) ;
			for (auto &&i : range (0 ,r1x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mTree[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list (CR<Length> size_) const override {
		Array<JsonParserLayout> ret = Array<JsonParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = self.mThis->mTree[self.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : range (0 ,r2x)) {
				ret[i].mThis = self.mThis ;
				ret[i].mIndex = self.mThis->mTree[self.mIndex].mArrayMap[i] ;
			}
		}
		return move (ret) ;
	}

	Bool equal (CR<JsonParserLayout> that) const override {
		const auto r1x = inline_compr (self.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!self.mThis.exist ())
			return TRUE ;
		if (address (self.mThis->mTree) != address (that.mThis->mTree))
			return FALSE ;
		if (self.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CR<String<Stru>> name () const leftvalue override {
		assert (exist ()) ;
		return self.mThis->mTree[self.mIndex].mName ;
	}

	Bool parse (CR<Bool> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Bool>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Val32 parse (CR<Val32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Val32>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Val64 parse (CR<Val64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Val64>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Flt32 parse (CR<Flt32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Flt32>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Flt64 parse (CR<Flt64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<Flt64>::make (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stra> parse (CR<String<Stra>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stra_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Strw> parse (CR<String<Strw>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::strw_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stru> parse (CR<String<Stru>> def) const override {
		if (!exist ())
			return def ;
		try {
			return self.mThis->mTree[self.mIndex].mValue ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stru16> parse (CR<String<Stru16>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru16_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<Stru32> parse (CR<String<Stru32>> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringProc::stru32_from (self.mThis->mTree[self.mIndex].mValue) ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<Bool> parse (CR<Bool> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Val32> parse (CR<Val32> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Val64> parse (CR<Val64> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Flt32> parse (CR<Flt32> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<Flt64> parse (CR<Flt64> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stra>> parse (CR<String<Stra>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Strw>> parse (CR<String<Strw>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stru>> parse (CR<String<Stru>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stru16>> parse (CR<String<Stru16>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	Array<String<Stru32>> parse (CR<String<Stru32>> def ,CR<Length> size_) const override {
		return parse_impl (def ,size_) ;
	}

	template <class ARG1>
	forceinline Array<ARG1> parse_impl (CR<ARG1> def ,CR<Length> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.iter ()) {
			const auto r2x = JsonParserHolder::hold (self)->child (i) ;
			ret[i] = JsonParserHolder::hold (r2x)->parse (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<JsonParserHolder> JsonParserHolder::hold (VR<JsonParserLayout> that) {
	return VFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

exports CFat<JsonParserHolder> JsonParserHolder::hold (CR<JsonParserLayout> that) {
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
	String<Stru> mName ;
	Just<PlyParserDataType> mType ;
	Just<PlyParserDataType> mListType ;
	Length mListSize ;
	Index mPlyBegin ;
	Index mPlyEnd ;
} ;

struct PlyParserElement {
	String<Stru> mName ;
	Length mLineSize ;
	Length mLineStep ;
	Length mLineLength ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String<Stru>> mPropertySet ;
	Length mLastSize ;
	RefBuffer<Pointer> mPlyBuffer ;
	Index mPlyIndex ;
	RefBuffer<Pointer> mExtBuffer ;
	Index mExtIndex ;
} ;

struct PlyParserTree {
	String<Stru> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<Stru>> mElementSet ;
} ;

struct MakePlyParserLayout {
	Ref<RefBuffer<Byte>> mStream ;
	TextReader mTextReader ;
	ByteReader mByteReader ;
	String<Stru> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<Stru>> mElementSet ;
	Bool mDiffEndianFlag ;
	StreamShape mBodyBackup ;
	Index mLastIndex ;
	String<Stru> mLastType ;
	String<Stru> mLastString ;
	Set<String<Stru>> mPropertyType ;
	Set<String<Stru>> mPropertyListType ;
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

	explicit MakePlyParser (RR<Ref<RefBuffer<Byte>>> stream) {
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
		mTextReader >> ReadKeyword (mLastString) ;
		mFormat = move (mLastString) ;
		mTextReader >> ReadEndline (mLastString) ;
		mTextReader >> GAP ;
		Index ix = NONE ;
		Index iy = NONE ;
		while (TRUE) {
			mTextReader >> ReadKeyword (mLastString) ;
			if (mLastString == slice ("end_header"))
				break ;
			mTextReader >> GAP ;
			auto act = TRUE ;
			if ifdo (act) {
				if (mLastString != slice ("element"))
					discard ;
				ix = mElementList.insert () ;
				mTextReader >> ReadKeyword (mLastString) ;
				mElementList[ix].mName = move (mLastString) ;
				mTextReader >> GAP ;
				mTextReader >> ReadScalar (mLastString) ;
				const auto r1x = StringParse<Length>::make (mLastString) ;
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
				mTextReader >> ReadKeyword (mLastType) ;
				mTextReader >> GAP ;
				if (mLastType != slice ("list"))
					discard ;
				iy = mElementList[ix].mPropertyList.insert () ;
				mTextReader >> ReadKeyword (mLastString) ;
				const auto r2x = mPropertyListType.map (mLastString) ;
				assume (r2x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r2x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r3x = ply_parser_data_type_size (r2x) ;
				mElementList[ix].mLineStep += r3x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mTextReader >> ReadKeyword (mLastString) ;
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
				mElementList[ix].mLineStep += SIZE_OF<Index>::expr ;
				mElementList[ix].mLineStep += SIZE_OF<Index>::expr ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mTextReader >> ReadKeyword (mLastString) ;
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
				mTextReader >> ReadKeyword (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = move (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("comment"))
					discard ;
				mTextReader >> ReadEndline (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				assume (FALSE) ;
			}
		}
		if ifdo (TRUE) {
			auto rax = Stru32 () ;
			const auto r7x = mTextReader.shape () ;
			mTextReader.read (rax) ;
			if (rax == Stru32 ('\r'))
				discard ;
			mTextReader.reset (r7x) ;
		}
		if ifdo (TRUE) {
			auto rax = Stru32 () ;
			mTextReader.read (rax) ;
			assume (rax == Stru32 ('\n')) ;
			mBodyBackup = mTextReader.shape () ;
		}
		for (auto &&i : mElementList.iter ()) {
			auto &&rax = mElementList[i] ;
			rax.mPropertyList.remap () ;
			rax.mPropertySet = Set<String<Stru>> (rax.mPropertyList.length ()) ;
			for (auto &&j : rax.mPropertyList.iter ())
				rax.mPropertySet.add (rax.mPropertyList[j].mName ,j) ;
		}
		if ifdo (TRUE) {
			mElementList.remap () ;
			mElementSet = Set<String<Stru>> (mElementList.length ()) ;
			for (auto &&j : mElementList.iter ())
				mElementSet.add (mElementList[j].mName ,j) ;
		}
		for (auto &&i : mElementList.iter ()) {
			auto &&rax = mElementList[i] ;
			const auto r8x = rax.mLineSize * rax.mLineStep ;
			auto &&rbx = keep[TYPE<RefBufferLayout>::expr] (rax.mPlyBuffer) ;
			rbx = RefBuffer<Byte> (r8x) ;
			rax.mPlyIndex = 0 ;
			auto &&rcx = keep[TYPE<RefBufferLayout>::expr] (rax.mExtBuffer) ;
			rcx = RefBuffer<Byte> () ;
			rax.mExtIndex = 0 ;
		}
	}

	Length ply_parser_data_type_size (CR<Just<PlyParserDataType>> type_) const {
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
		for (auto &&i : range (0 ,mElementList.length ())) {
			for (auto &&j : range (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : range (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_text_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_text_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mTextReader >> GAP ;
		mTextReader >> EOS ;
	}

	void read_body_text_item (VR<PlyParserElement> element ,VR<PlyParserProperty> property ,CR<Index> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mTextReader.poll (TYPE<Flt32>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r2x ;
			element.mPlyIndex += SIZE_OF<Flt32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r3x = mTextReader.poll (TYPE<Flt64>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r3x ;
			element.mPlyIndex += SIZE_OF<Flt64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r4x = mTextReader.poll (TYPE<Bool>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r4x ;
			element.mPlyIndex += SIZE_OF<Bool>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r5x = mTextReader.poll (TYPE<Val32>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r5x ;
			element.mPlyIndex += SIZE_OF<Val32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r6x = mTextReader.poll (TYPE<Val64>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r6x ;
			element.mPlyIndex += SIZE_OF<Val64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r7x = mTextReader.poll (TYPE<Val64>::expr) ;
			assume (r7x >= 0) ;
			const auto r8x = Byte (r7x) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r8x ;
			element.mPlyIndex += SIZE_OF<Byte>::expr ;
			mTextReader >> GAP ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = Length (r8x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r9x = mTextReader.poll (TYPE<Val64>::expr) ;
			assume (r9x >= 0) ;
			const auto r10x = Word (r9x) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r10x ;
			element.mPlyIndex += SIZE_OF<Word>::expr ;
			mTextReader >> GAP ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = Length (r9x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r11x = mTextReader.poll (TYPE<Val64>::expr) ;
			assume (r11x >= 0) ;
			const auto r12x = Char (r11x) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r12x ;
			element.mPlyIndex += SIZE_OF<Char>::expr ;
			mTextReader >> GAP ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = Length (r12x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r13x = mTextReader.poll (TYPE<Val64>::expr) ;
			assume (r13x >= 0) ;
			const auto r14x = Quad (r13x) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r14x ;
			element.mPlyIndex += SIZE_OF<Quad>::expr ;
			mTextReader >> GAP ;
		}
	}

	void read_body_text_list (VR<PlyParserElement> element ,VR<PlyParserProperty> property ,CR<Index> line) {
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
		const auto r5x = Length (element.mExtIndex) ;
		bitwise (element.mPlyBuffer[element.mPlyIndex]) = r5x ;
		element.mPlyIndex += SIZE_OF<Length>::expr ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : range (0 ,r2x)) {
				noop (i) ;
				const auto r6x = mTextReader.poll (TYPE<Val32>::expr) ;
				bitwise (element.mExtBuffer[element.mExtIndex]) = r6x ;
				element.mExtIndex += SIZE_OF<Val32>::expr ;
				mTextReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : range (0 ,r2x)) {
				noop (i) ;
				const auto r7x = mTextReader.poll (TYPE<Val64>::expr) ;
				bitwise (element.mExtBuffer[element.mExtIndex]) = r7x ;
				element.mExtIndex += SIZE_OF<Val64>::expr ;
				mTextReader >> GAP ;
			}
		}
		const auto r8x = Length (element.mExtIndex) ;
		bitwise (element.mPlyBuffer[element.mPlyIndex]) = r8x ;
		element.mPlyIndex += SIZE_OF<Length>::expr ;
	}

	void read_body_byte () {
		mByteReader = ByteReader (mStream.share ()) ;
		mByteReader.reset (mBodyBackup) ;
		if ifdo (TRUE) {
			if (!mDiffEndianFlag)
				discard ;
			mByteReader >> BOM ;
		}
		for (auto &&i : range (0 ,mElementList.length ())) {
			for (auto &&j : range (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : range (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_byte_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_byte_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mByteReader >> EOS ;
	}

	void read_body_byte_item (VR<PlyParserElement> element ,VR<PlyParserProperty> property ,CR<Index> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mByteReader.poll (TYPE<Flt32>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r2x ;
			element.mPlyIndex += SIZE_OF<Flt32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r3x = mByteReader.poll (TYPE<Flt64>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r3x ;
			element.mPlyIndex += SIZE_OF<Flt64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r4x = mByteReader.poll (TYPE<Bool>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r4x ;
			element.mPlyIndex += SIZE_OF<Bool>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r5x = mByteReader.poll (TYPE<Val32>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r5x ;
			element.mPlyIndex += SIZE_OF<Val32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r6x = mByteReader.poll (TYPE<Val64>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r6x ;
			element.mPlyIndex += SIZE_OF<Val64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r7x = mByteReader.poll (TYPE<Byte>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r7x ;
			element.mPlyIndex += SIZE_OF<Byte>::expr ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = Length (r7x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r8x = mByteReader.poll (TYPE<Word>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r8x ;
			element.mPlyIndex += SIZE_OF<Word>::expr ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = Length (r8x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r9x = mByteReader.poll (TYPE<Char>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r9x ;
			element.mPlyIndex += SIZE_OF<Char>::expr ;
			if ifdo (TRUE) {
				if (property.mListType == PlyParserDataType::Null)
					discard ;
				element.mLastSize = Length (r9x) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r10x = mByteReader.poll (TYPE<Quad>::expr) ;
			bitwise (element.mPlyBuffer[element.mPlyIndex]) = r10x ;
			element.mPlyIndex += SIZE_OF<Quad>::expr ;
		}
	}

	void read_body_byte_list (VR<PlyParserElement> element ,VR<PlyParserProperty> property ,CR<Index> line) {
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
		const auto r5x = Length (element.mExtIndex) ;
		bitwise (element.mPlyBuffer[element.mPlyIndex]) = r5x ;
		element.mPlyIndex += SIZE_OF<Length>::expr ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			for (auto &&i : range (0 ,r2x)) {
				noop (i) ;
				const auto r6x = mByteReader.poll (TYPE<Val32>::expr) ;
				bitwise (element.mExtBuffer[element.mExtIndex]) = r6x ;
				element.mExtIndex += SIZE_OF<Val32>::expr ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			for (auto &&i : range (0 ,r2x)) {
				noop (i) ;
				const auto r7x = mByteReader.poll (TYPE<Val64>::expr) ;
				bitwise (element.mExtBuffer[element.mExtIndex]) = r7x ;
				element.mExtIndex += SIZE_OF<Val64>::expr ;
			}
		}
		const auto r8x = Length (element.mExtIndex) ;
		bitwise (element.mPlyBuffer[element.mPlyIndex]) = r8x ;
		element.mPlyIndex += SIZE_OF<Length>::expr ;
	}
} ;

class PlyParserImplHolder final implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CR<RefBuffer<Byte>> stream) override {
		auto rax = MakePlyParser (Ref<RefBuffer<Byte>>::reference (stream)) ;
		rax.generate () ;
		self.mThis = UniqueRef<PlyParserTree>::make (rax.poll ()) ;
		self.mGuide.mElement = NONE ;
	}

	Length element_size (CR<Slice> element) const override {
		Index ix = self.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		return self.mThis->mElementList[ix].mLineSize ;
	}

	Length property_size (CR<Slice> element ,CR<Slice> property) const override {
		Index ix = self.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		Index jx = self.mThis->mElementList[ix].mPropertySet.map (property) ;
		if (jx == NONE)
			return 0 ;
		return self.mThis->mElementList[ix].mPropertyList[jx].mListSize ;
	}

	void guide_new (CR<Slice> element) override {
		Index ix = self.mThis->mElementSet.map (element) ;
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

	void guide_put (CR<Slice> property) override {
		Index ix = self.mGuide.mElement ;
		assume (ix != NONE) ;
		Index jx = self.mThis->mElementList[ix].mPropertySet.map (property) ;
		assume (jx != NONE) ;
		assert (self.mGuide.mRow == NONE) ;
		self.mGuide.mProperty.add (jx) ;
	}

	void guide_jmp () {
		assert (self.mGuide.mElement != NONE) ;
		Index ix = self.mGuide.mElement ;
		auto &&rax = self.mThis->mElementList[ix] ;
		Index jx = NONE ;
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
				self.mGuide.mPlyBegin = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
				self.mGuide.mPlyIndex += SIZE_OF<Length>::expr ;
				self.mGuide.mPlyEnd = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
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

	void read (VR<Bool> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Bool) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Bool>::expr ;
	}

	void read (VR<Val32> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mGuide.mPlyListMode)
				discard ;
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mType == PlyParserDataType::Val32) ;
			item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
			self.mGuide.mPlyIndex += SIZE_OF<Val32>::expr ;
		}
		if ifdo (act) {
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mListType == PlyParserDataType::Val32) ;
			item = bitwise (rax.mExtBuffer[self.mGuide.mPlyIndex]) ;
			self.mGuide.mPlyIndex += SIZE_OF<Val32>::expr ;
		}
	}

	void read (VR<Val64> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mGuide.mPlyListMode)
				discard ;
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mType == PlyParserDataType::Val64) ;
			item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
			self.mGuide.mPlyIndex += SIZE_OF<Val64>::expr ;
		}
		if ifdo (act) {
			auto &&rax = self.mThis->mElementList[ix] ;
			assume (rax.mPropertyList[jx].mListType == PlyParserDataType::Val64) ;
			item = bitwise (rax.mExtBuffer[self.mGuide.mPlyIndex]) ;
			self.mGuide.mPlyIndex += SIZE_OF<Val64>::expr ;
		}
	}

	void read (VR<Flt32> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Flt32) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Flt32>::expr ;
	}

	void read (VR<Flt64> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Flt64) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Flt64>::expr ;
	}

	void read (VR<Byte> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Byte) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Byte>::expr ;
	}

	void read (VR<Word> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Word) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Word>::expr ;
	}

	void read (VR<Char> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Char) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Char>::expr ;
	}

	void read (VR<Quad> item) override {
		guide_jmp () ;
		Index ix = self.mGuide.mElement ;
		Index jx = self.mGuide.mProperty[self.mGuide.mCol] ;
		auto &&rax = self.mThis->mElementList[ix] ;
		assume (rax.mPropertyList[jx].mType == PlyParserDataType::Quad) ;
		item = bitwise (rax.mPlyBuffer[self.mGuide.mPlyIndex]) ;
		self.mGuide.mPlyIndex += SIZE_OF<Quad>::expr ;
	}
} ;

exports VFat<PlyParserHolder> PlyParserHolder::hold (VR<PlyParserLayout> that) {
	return VFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::hold (CR<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}
} ;