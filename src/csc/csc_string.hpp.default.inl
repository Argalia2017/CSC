#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require 'csc_string.hpp'"
#endif

#include "csc_string.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <string>
#include <regex>
#include "csc_begin.h"

namespace CSC {
template <class...>
trait FUNCTION_system_string_cvt_HELP ;

template <class DEPEND>
trait FUNCTION_system_string_cvt_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
#ifdef __CSC_COMPILER_MSVC__
	struct FUNCTION_system_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			assert (ifnot (obj.empty ())) ;
			String<STRA> ret = String<STRA> (obj.length () * 2 + 1) ;
			const auto r1x = system_page () ;
			const auto r2x = _wcstombs_s_l (NULL ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE ,r1x.self) ;
			assume (r2x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			assert (ifnot (obj.empty ())) ;
			const auto r1x = system_page () ;
			String<STRW> ret = String<STRW> (obj.length () + 1) ;
			const auto r2x = _mbstowcs_s_l (NULL ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE ,r1x.self) ;
			assume (r2x == 0) ;
			return move (ret) ;
		}

		CRef<UniqueRef<_locale_t>> system_page () const {
			return CRef<UniqueRef<_locale_t>>::reference (memorize ([&] () {
				return UniqueRef<_locale_t> ([&] (VREF<_locale_t> me) {
					me = _create_locale (LC_CTYPE ,"") ;
					assume (me != NULL) ;
				} ,[] (VREF<_locale_t> me) {
					_free_locale (me) ;
				}) ;
			})) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_system_string_cvt_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_system_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			assert (ifnot (obj.empty ())) ;
			String<STRA> ret = String<STRA> (obj.length () * 2 + 1) ;
			const auto r1x = std::wcstombs ((&ret[0]) ,(&obj[0]) ,VAL32 (ret.size ())) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			assert (ifnot (obj.empty ())) ;
			String<STRW> ret = String<STRW> (obj.length () + 1) ;
			const auto r1x = std::mbstowcs ((&ret[0]) ,(&obj[0]) ,VAL32 (ret.size ())) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_system_string_cvt_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_CLANG<DEPEND>>> {
#ifdef __CSC_COMPILER_CLANG__
	struct FUNCTION_system_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			assert (ifnot (obj.empty ())) ;
			String<STRA> ret = String<STRA> (obj.length () * 2 + 1) ;
			auto rax = std::size_t (0) ;
			const auto r1x = wcstombs_s ((&rax) ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			assert (ifnot (obj.empty ())) ;
			String<STRW> ret = String<STRW> (obj.length () + 1) ;
			auto rax = std::size_t (0) ;
			const auto r1x = mbstowcs_s ((&rax) ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}
	} ;
#endif
} ;

template <class...>
trait STRINGPROC_GBKSCACHE_HELP ;

template <class DEPEND>
trait STRINGPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename STRINGPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		String<STRA> string_cvt_ansi_from_w (CREF<String<STRW>> obj) const override {
			String<STRA> ret ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUA>>>::expr] (unsafe_deptr (ret))) ;
			auto &&tmp_2 = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			tmp = string_cvt_ansi_from_w (tmp_2) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		String<STRUA> string_cvt_ansi_from_w (CREF<String<STRUW>> obj) const {
			String<STRUA> ret = String<STRUA> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				assume (vbetween (INDEX (i) ,0 ,128)) ;
				ret[ix] = STRUA (i) ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRW> string_cvt_w_from_ansi (CREF<String<STRA>> obj) const override {
			String<STRW> ret ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (ret))) ;
			auto &&tmp_2 = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUA>>>::expr] (unsafe_deptr (obj))) ;
			tmp = string_cvt_w_from_ansi (tmp_2) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		String<STRUW> string_cvt_w_from_ansi (CREF<String<STRUA>> obj) const {
			String<STRUW> ret = String<STRUW> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				assume (vbetween (INDEX (i) ,0 ,128)) ;
				ret[ix] = STRUW (i) ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRA> string_cvt_gbks_from_w (CREF<String<STRW>> obj) const override {
			String<STRA> ret ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUA>>>::expr] (unsafe_deptr (ret))) ;
			auto &&tmp_2 = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			tmp = string_cvt_gbks_from_w (tmp_2) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		String<STRUA> string_cvt_gbks_from_w (CREF<String<STRUW>> obj) const {
			using R1X = typename DEPENDENT<STRINGPROC_GBKSCACHE_HELP<DEPEND ,ALWAYS> ,ALWAYS>::GBKSCache ;
			String<STRUA> ret = String<STRUA> (obj.length () * 2) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				const auto r1x = R1X::instance ().find_utfs (STRUW (i)) ;
				auto act = TRUE ;
				if ifswitch (act) {
					if (r1x == NONE)
						discard ;
					const auto r2x = R1X::instance ()[r1x][0] ;
					if (r2x >= STRUW (0X0100))
						discard ;
					ret[ix] = STRUA (r2x) ;
					ix++ ;
				}
				if ifswitch (act) {
					if (r1x == NONE)
						discard ;
					const auto r3x = R1X::instance ()[r1x][0] ;
					ret[ix] = STRUA (r3x >> 8) ;
					ix++ ;
					ret[ix] = STRUA (r3x) ;
					ix++ ;
				}
				if ifswitch (act) {
					ret[ix] = STRUA ('?') ;
					ix++ ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRW> string_cvt_w_from_gbks (CREF<String<STRA>> obj) const override {
			String<STRW> ret ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (ret))) ;
			auto &&tmp_2 = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUA>>>::expr] (unsafe_deptr (obj))) ;
			tmp = string_cvt_w_from_gbks (tmp_2) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		String<STRUW> string_cvt_w_from_gbks (CREF<String<STRUA>> obj) const {
			using R1X = typename DEPENDENT<STRINGPROC_GBKSCACHE_HELP<DEPEND ,ALWAYS> ,DEPEND>::GBKSCache ;
			String<STRUW> ret = String<STRUW> (obj.length ()) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					const auto r1x = R1X::instance ().find_gbks (STRUW (i)) ;
					if (r1x == NONE)
						discard ;
					ret[ix] = R1X::instance ()[r1x][1] ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					rbx = STRU32 (i) ;
					rax = 1 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 1)
						discard ;
					rbx = STRU32 ((rbx << 8) | STRU32 (i)) ;
					const auto r2x = R1X::instance ().find_gbks (STRUW (rbx)) ;
					if (r2x == NONE)
						discard ;
					ret[ix] = R1X::instance ()[r2x][1] ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (act) {
					ret[ix] = STRUW ('?') ;
					ix++ ;
					rax = 0 ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRA> string_cvt_a_from_w (CREF<String<STRW>> obj) const override {
			const auto r1x = system_locale () ;
			if (r1x == slice ("C"))
				return string_cvt_ansi_from_w (obj) ;
			const auto r2x = r1x.length () ;
			if (r2x >= 6)
				if (r1x.segment (0 ,6) == slice ("zh_CN."))
					return string_cvt_gbks_from_w (obj) ;
			if (r2x >= 4)
				if (r1x.segment (r2x - 4 ,r2x) == slice (".936"))
					return string_cvt_gbks_from_w (obj) ;
			return system_string_cvt (obj) ;
		}

		String<STRW> string_cvt_w_from_a (CREF<String<STRA>> obj) const override {
			const auto r1x = system_locale () ;
			if (r1x == slice ("C"))
				return string_cvt_w_from_ansi (obj) ;
			const auto r2x = r1x.length () ;
			if (r2x >= 6)
				if (r1x.segment (0 ,6) == slice ("zh_CN."))
					return string_cvt_w_from_gbks (obj) ;
			if (r2x >= 4)
				if (r1x.segment (r2x - 4 ,r2x) == slice (".936"))
					return string_cvt_w_from_gbks (obj) ;
			return system_string_cvt (obj) ;
		}

		String<STR> system_locale () const {
			const auto r1x = std::setlocale (LC_CTYPE ,NULL) ;
			if (r1x == NULL)
				return Slice<STR> () ;
			return BufferProc<STR>::buf_slice (unsafe_array (r1x[0]) ,64) ;
		}

		String<STRA> system_string_cvt (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_system_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_system_string_cvt ;
			if (obj.empty ())
				return String<STRA> () ;
			const auto r1x = R1X () ;
			return r1x (obj) ;
		}

		String<STRW> system_string_cvt (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_system_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_system_string_cvt ;
			if (obj.empty ())
				return String<STRW> () ;
			const auto r1x = R1X () ;
			return r1x (obj) ;
		}

		String<STRU8> string_cvt_u8_from_u16 (CREF<String<STRU16>> obj) const override {
			String<STRU8> ret = String<STRU8> (obj.length () * 3) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X007F))
						discard ;
					ret[ix] = STRU8 (i) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X07FF))
						discard ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i >= STRU16 (0XD800))
						discard ;
					if ifnot (i <= STRU16 (0XDBFF))
						discard ;
					rbx = STRU32 (i & STRU16 (0X03FF)) ;
					rax = 1 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					ret[ix] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i >= STRU16 (0XDC00))
						discard ;
					if ifnot (i <= STRU16 (0XDFFF))
						discard ;
					rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
					ret[ix] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
					ix++ ;
					ret[ix] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[ix] = STRU8 ('?') ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRU8> string_cvt_u8_from_u32 (CREF<String<STRU32>> obj) const override {
			/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
			String<STRU8> ret = String<STRU8> (obj.length () * 6) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000007F))
						discard ;
					ret[ix] = STRU8 (i) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X000007FF))
						discard ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X001FFFFF))
						discard ;
					ret[ix] = (STRU8 (i >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X03FFFFFF))
						discard ;
					ret[ix] = (STRU8 (i >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = (STRU8 (i >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRU16> string_cvt_u16_from_u8 (CREF<String<STRU8>> obj) const override {
			String<STRU16> ret = String<STRU16> (obj.length ()) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				if ifswitch (TRUE) {
					auto act = TRUE ;
					if ifswitch (act) {
						if ifnot (rax == 0)
							discard ;
						if ifnot (i <= STRU8 (0X7F))
							discard ;
						ret[ix] = STRU16 (i) ;
						ix++ ;
					}
					if ifswitch (act) {
						if ifnot (rax == 0)
							discard ;
						if ifnot (i <= STRU8 (0XDF))
							discard ;
						rbx = STRU32 (i & STRU8 (0X1F)) ;
						rax = 1 ;
					}
					if ifswitch (act) {
						if ifnot (rax == 0)
							discard ;
						if ifnot (i <= STRU8 (0XEF))
							discard ;
						rbx = STRU32 (i & STRU8 (0X0F)) ;
						rax = 2 ;
					}
					if ifswitch (act) {
						if ifnot (rax == 0)
							discard ;
						if ifnot (i <= STRU8 (0XF7))
							discard ;
						rbx = STRU32 (i & STRU8 (0X07)) ;
						rax = 3 ;
					}
					if ifswitch (act) {
						if ifnot (rax == 0)
							discard ;
						if ifnot (i <= STRU8 (0XFB))
							discard ;
						rbx = STRU32 (i & STRU8 (0X03)) ;
						rax = 4 ;
					}
					if ifswitch (act) {
						if ifnot (rax == 0)
							discard ;
						if ifnot (i <= STRU8 (0XFD))
							discard ;
						rbx = STRU32 (i & STRU8 (0X01)) ;
						rax = 5 ;
					}
					if ifswitch (act) {
						if ifnot (rax == 1)
							discard ;
						if ifnot (i <= STRU8 (0XBF))
							discard ;
						rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
						rax = 10 ;
					}
					if ifswitch (act) {
						if ifnot (vbetween (rax ,2 ,6))
							discard ;
						if ifnot (i <= STRU8 (0XBF))
							discard ;
						rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
						rax-- ;
					}
					if ifswitch (act) {
						ret.clear () ;
						rax = NONE ;
					}
				}
				if ifswitch (TRUE) {
					if (rax != 10)
						discard ;
					auto act = TRUE ;
					if ifswitch (act) {
						if ifnot (rbx <= STRU32 (0X0000FFFF))
							discard ;
						ret[ix] = STRU16 (rbx) ;
						ix++ ;
						rax = 0 ;
					}
					if ifswitch (act) {
						if ifnot (rbx <= STRU32 (0X0010FFFF))
							discard ;
						rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
						ret[ix] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
						ix++ ;
						ret[ix] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
						ix++ ;
						rax = 0 ;
					}
					if ifswitch (act) {
						if ifnot (rbx <= STRU32 (0X7FFFFFFF))
							discard ;
						ret[ix] = STRU16 ('?') ;
						ix++ ;
						rax = 0 ;
					}
					if ifswitch (act) {
						ret.clear () ;
						rax = NONE ;
					}
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[ix] = STRU16 ('?') ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRU16> string_cvt_u16_from_u32 (CREF<String<STRU32>> obj) const override {
			/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
			String<STRU16> ret = String<STRU16> (obj.length () * 2) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (i) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0010FFFF))
						discard ;
					ret[ix] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 ('?') ;
					ix++ ;
				}
				if ifswitch (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRU32> string_cvt_u32_from_u8 (CREF<String<STRU8>> obj) const override {
			/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
			String<STRU32> ret = String<STRU32> (obj.length ()) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0X7F))
						discard ;
					ret[ix] = STRU32 (i) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XDF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X1F)) ;
					rax = 1 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XEF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X0F)) ;
					rax = 2 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XF7))
						discard ;
					rbx = STRU32 (i & STRU8 (0X07)) ;
					rax = 3 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFB))
						discard ;
					rbx = STRU32 (i & STRU8 (0X03)) ;
					rax = 4 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFD))
						discard ;
					rbx = STRU32 (i & STRU8 (0X01)) ;
					rax = 5 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					ret[ix] = rbx ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (act) {
					if ifnot (vbetween (rax ,2 ,6))
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifswitch (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[ix] = STRU32 ('?') ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRU32> string_cvt_u32_from_u16 (CREF<String<STRU16>> obj) const override {
			/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
			String<STRU32> ret = String<STRU32> (obj.length ()) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X07FF))
						discard ;
					ret[ix] = STRU32 (i) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i >= STRU16 (0XD800))
						discard ;
					if ifnot (i <= STRU16 (0XDBFF))
						discard ;
					rbx = STRU32 (i & STRU16 (0X03FF)) ;
					rax = 1 ;
				}
				if ifswitch (act) {
					if ifnot (rax == 0)
						discard ;
					ret[ix] = STRU32 (i) ;
					ix++ ;
				}
				if ifswitch (act) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i >= STRU16 (0XDC00))
						discard ;
					if ifnot (i <= STRU16 (0XDFFF))
						discard ;
					rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
					ret[ix] = rbx ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[ix] = STRU32 ('?') ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto STRINGPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename STRINGPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait TEXTSTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TEXTSTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	struct PACK {
		String<STR> mText ;
		String<STRA> mTextA ;
		String<STRW> mTextW ;
		String<STRU8> mTextU8 ;
		String<STRU16> mTextU16 ;
		String<STRU32> mTextU32 ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<PACK> mThis ;

	public:
		void initialize (CREF<Slice<STR>> text) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mText = text ;
		}

		void initialize (CREF<String<STRA>> text) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mTextA = text ;
			mThis->mText = string_cvt[TYPEAS<STR ,STRA>::expr] (text) ;
		}

		void initialize (CREF<String<STRW>> text) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mTextW = text ;
			mThis->mText = string_cvt[TYPEAS<STR ,STRW>::expr] (text) ;
		}

		void initialize (CREF<String<STRU8>> text) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mTextU8 = text ;
			mThis->mText = string_cvt[TYPEAS<STR ,STRU8>::expr] (text) ;
		}

		void initialize (CREF<String<STRU16>> text) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mTextU16 = text ;
			mThis->mText = string_cvt[TYPEAS<STR ,STRU16>::expr] (text) ;
		}

		void initialize (CREF<String<STRU32>> text) override {
			mThis = SharedRef<PACK>::make () ;
			mThis->mTextU32 = text ;
			mThis->mText = string_cvt[TYPEAS<STR ,STRU32>::expr] (text) ;
		}

		CREF<String<STRA>> pick (CREF<TYPEID<STRA>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mThis->mTextA.size () > 0)
					discard ;
				mThis->mTextA = string_cvt[TYPEAS<STRA ,STR>::expr] (mThis->mText) ;
			}
			return mThis->mTextA ;
		}

		CREF<String<STRW>> pick (CREF<TYPEID<STRW>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mThis->mTextW.size () > 0)
					discard ;
				mThis->mTextW = string_cvt[TYPEAS<STRW ,STR>::expr] (mThis->mText) ;
			}
			return mThis->mTextW ;
		}

		CREF<String<STRU8>> pick (CREF<TYPEID<STRU8>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mThis->mTextU8.size () > 0)
					discard ;
				mThis->mTextU8 = string_cvt[TYPEAS<STRU8 ,STR>::expr] (mThis->mText) ;
			}
			return mThis->mTextU8 ;
		}

		CREF<String<STRU16>> pick (CREF<TYPEID<STRU16>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mThis->mTextU16.size () > 0)
					discard ;
				mThis->mTextU16 = string_cvt[TYPEAS<STRU16 ,STR>::expr] (mThis->mText) ;
			}
			return mThis->mTextU16 ;
		}

		CREF<String<STRU32>> pick (CREF<TYPEID<STRU32>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mThis->mTextU32.size () > 0)
					discard ;
				mThis->mTextU32 = string_cvt[TYPEAS<STRU32 ,STR>::expr] (mThis->mText) ;
			}
			return mThis->mTextU32 ;
		}
	} ;
} ;

template <>
exports auto TEXTSTRING_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTSTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait ESCAPESTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ESCAPESTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		TextString mText ;

	public:
		void initialize (RREF<TextString> text) override {
			mText = move (text) ;
		}

		void write_text (VREF<TextWriter<STRA>> writer) const override {
			write_text_impl (writer ,mText.pick (TYPEAS<STRA>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRW>> writer) const override {
			write_text_impl (writer ,mText.pick (TYPEAS<STRW>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU8>> writer) const override {
			write_text_impl (writer ,mText.pick (TYPEAS<STRU8>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU16>> writer) const override {
			write_text_impl (writer ,mText.pick (TYPEAS<STRU16>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU32>> writer) const override {
			write_text_impl (writer ,mText.pick (TYPEAS<STRU32>::expr)) ;
		}

		template <class ARG1 ,class ARG2>
		void write_text_impl (VREF<ARG1> writer ,CREF<ARG2> text) const {
			const auto r1x = writer.attribute () ;
			writer << slice ("\"") ;
			for (auto &&i : text) {
				auto act = TRUE ;
				if ifswitch (act) {
					const auto r2x = r1x.escape_word_cast (i) ;
					if ifnot (r2x.available ())
						discard ;
					writer << slice ("\\") ;
					writer << r2x.fetch () ;
				}
				if ifswitch (act) {
					writer << i ;
				}
			}
			writer << slice ("\"") ;
		}
	} ;
} ;

template <>
exports auto ESCAPESTRING_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename ESCAPESTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait COMMASTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename COMMASTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,256> ;

	class ImplHolder implement Holder {
	protected:
		TextString mGapText ;
		TextString mCommaText ;
		TextString mFirstText ;
		LENGTH mCounter ;
		Optional<LENGTH> mTightCounter ;
		Deque<Optional<BOOL>> mFirst ;

	public:
		void initialize (RREF<TextString> gap_text ,RREF<TextString> comma_text ,RREF<TextString> first_text) override {
			mGapText = move (gap_text) ;
			mCommaText = move (comma_text) ;
			mFirstText = move (first_text) ;
			mCounter = 0 ;
			mTightCounter = Optional<LENGTH>::make (COUNTER_MAX_DEPTH::expr) ;
			mFirst = Deque<Optional<BOOL>> (COUNTER_MAX_DEPTH::expr * 2) ;
			mFirst.add (Optional<BOOL>::make (TRUE)) ;
		}

		void write_text (VREF<TextWriter<STRA>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRA>::expr) ,mCommaText.pick (TYPEAS<STRA>::expr) ,mFirstText.pick (TYPEAS<STRA>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRW>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRW>::expr) ,mCommaText.pick (TYPEAS<STRW>::expr) ,mFirstText.pick (TYPEAS<STRW>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU8>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRU8>::expr) ,mCommaText.pick (TYPEAS<STRU8>::expr) ,mFirstText.pick (TYPEAS<STRU8>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU16>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRU16>::expr) ,mCommaText.pick (TYPEAS<STRU16>::expr) ,mFirstText.pick (TYPEAS<STRU16>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU32>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRU32>::expr) ,mCommaText.pick (TYPEAS<STRU32>::expr) ,mFirstText.pick (TYPEAS<STRU32>::expr)) ;
		}

		template <class ARG1 ,class ARG2>
		void write_text_impl (VREF<ARG1> writer ,CREF<ARG2> gap_text ,CREF<ARG2> comma_text ,CREF<ARG2> first_text) const {
			INDEX ix = mFirst.tail () ;
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mCounter > mTightCounter.fetch ())
					discard ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					for (auto &&j : comma_text)
						writer << j ;
				}
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (act) {
				if (mCounter != mTightCounter.fetch ())
					discard ;
				if ifnot (mFirst[ix].fetch ())
					discard ;
				mTightCounter.store (COUNTER_MAX_DEPTH::expr) ;
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (act) {
				for (auto &&j : first_text)
					writer << j ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					for (auto &&j : comma_text)
						writer << j ;
				}
				mFirst[ix].store (FALSE) ;
				for (auto &&i : iter (0 ,mCounter)) {
					noop (i) ;
					for (auto &&j : gap_text)
						writer << j ;
				}
			}
		}

		void tight () override {
			mTightCounter.store (mCounter) ;
		}

		void enter () override {
			assume (mCounter < COUNTER_MAX_DEPTH::expr) ;
			mCounter++ ;
			mFirst.add (Optional<BOOL>::make (TRUE)) ;
		}

		void leave () override {
			mFirst.pop () ;
			INDEX ix = mFirst.tail () ;
			mFirst[ix].store (TRUE) ;
			mCounter-- ;
		}
	} ;
} ;

template <>
exports auto COMMASTRING_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename COMMASTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait VALUESTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename VALUESTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,256> ;

	class ImplHolder implement Holder {
	protected:
		VAL64 mValue ;
		LENGTH mAlign ;
		LENGTH mSpace ;

	public:
		void initialize (CREF<VAL64> value_ ,CREF<LENGTH> align_) override {
			mValue = value_ ;
			mAlign = align_ ;
			mSpace = mAlign - 1 - MathProc::vlog (mValue ,VAL64 (10)) ;
			assume (mSpace >= 0) ;
		}

		void write_text (VREF<TextWriter<STRA>> writer) const override {
			write_text_impl (writer) ;
		}

		void write_text (VREF<TextWriter<STRW>> writer) const override {
			write_text_impl (writer) ;
		}

		void write_text (VREF<TextWriter<STRU8>> writer) const override {
			write_text_impl (writer) ;
		}

		void write_text (VREF<TextWriter<STRU16>> writer) const override {
			write_text_impl (writer) ;
		}

		void write_text (VREF<TextWriter<STRU32>> writer) const override {
			write_text_impl (writer) ;
		}

		template <class ARG1>
		void write_text_impl (VREF<ARG1> writer) const {
			for (auto &&i : iter (0 ,mSpace)) {
				noop (i) ;
				writer << slice ("0") ;
			}
			writer << mValue ;
		}
	} ;
} ;

template <>
exports auto VALUESTRING_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename VALUESTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;