#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"

namespace CSC {
namespace STRING {
template <class...>
trait STRINGPROC_HELP ;

template <class DEPEND>
trait STRINGPROC_HELP<DEPEND ,ALWAYS> {
	class StringProc {
	public:
		imports CREF<StringProc> instance () {
			return memorize ([&] () {
				return StringProc () ;
			}) ;
		}

		imports BOOL is_ansi_string (CREF<String<STRA>> obj) {
			return instance ().template_is_ansi_string (obj) ;
		}

		imports BOOL is_ansi_string (CREF<String<STRW>> obj) {
			return instance ().template_is_ansi_string (obj) ;
		}

	private:
		template <class ARG1>
		BOOL template_is_ansi_string (XREF<ARG1> obj) const {
			for (auto &&i : obj.iter ()) {
				if (STRU32 (i) >= STRU32 (128))
					return FALSE ;
			}
			return TRUE ;
		}
	} ;
} ;

using StringProc = typename STRINGPROC_HELP<DEPEND ,ALWAYS>::StringProc ;

template <class...>
trait FUNCTION_string_parse_HELP ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,BOOL>>> {
	struct FUNCTION_string_parse {
		inline BOOL operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<BOOL>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VAL32>>> {
	struct FUNCTION_string_parse {
		inline VAL32 operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<VAL32>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VAL64>>> {
	struct FUNCTION_string_parse {
		inline VAL64 operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<VAL64>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,SINGLE>>> {
	struct FUNCTION_string_parse {
		inline SINGLE operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<SINGLE>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,DOUBLE>>> {
	struct FUNCTION_string_parse {
		inline DOUBLE operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<DOUBLE>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,BYTE>>> {
	struct FUNCTION_string_parse {
		inline BYTE operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<BYTE>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,WORD>>> {
	struct FUNCTION_string_parse {
		inline WORD operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<WORD>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,CHAR>>> {
	struct FUNCTION_string_parse {
		inline CHAR operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<CHAR>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,DATA>>> {
	struct FUNCTION_string_parse {
		inline DATA operator() (CREF<String<UNIT2>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<DATA>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using FUNCTION_string_parse = typename FUNCTION_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,UNIT1>>>::FUNCTION_string_parse ;

template <class...>
trait FUNCTION_string_build_HELP ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,BOOL>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<BOOL> obj) const {
			unimplemented () ;
			return bad (TYPEAS<BOOL>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VAL32>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<VAL32> obj) const {
			unimplemented () ;
			return bad (TYPEAS<VAL32>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VAL64>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<VAL64> obj) const {
			unimplemented () ;
			return bad (TYPEAS<VAL64>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,SINGLE>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<SINGLE> obj) const {
			unimplemented () ;
			return bad (TYPEAS<SINGLE>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,DOUBLE>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<DOUBLE> obj) const {
			unimplemented () ;
			return bad (TYPEAS<DOUBLE>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,BYTE>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<BYTE> obj) const {
			unimplemented () ;
			return bad (TYPEAS<BYTE>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,WORD>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<WORD> obj) const {
			unimplemented () ;
			return bad (TYPEAS<WORD>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,CHAR>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<CHAR> obj) const {
			unimplemented () ;
			return bad (TYPEAS<CHAR>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,DATA>>> {
	struct FUNCTION_string_parse {
		inline String<UNIT1> operator() (CREF<DATA> obj) const {
			unimplemented () ;
			return bad (TYPEAS<DATA>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using FUNCTION_string_build = typename FUNCTION_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,UNIT2>>>::FUNCTION_string_build ;

template <class...>
trait FUNCTION_string_cvt_HELP ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRA> operator() (CREF<String<STRA>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<String<STRA>>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			using R2X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE1 = R1X () ;
			static constexpr auto M_INVOKE2 = R2X () ;
			return M_INVOKE2 (M_INVOKE1 (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			using R2X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE1 = R1X () ;
			static constexpr auto M_INVOKE2 = R2X () ;
			return M_INVOKE2 (M_INVOKE1 (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			using R2X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE1 = R1X () ;
			static constexpr auto M_INVOKE2 = R2X () ;
			return M_INVOKE2 (M_INVOKE1 (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			unimplemented () ;
			return bad (TYPEAS<String<STRW>>::id) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRW> operator() (CREF<String<STRW>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRUW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			auto rax = M_INVOKE (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<String<STRW>>>::id ,unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRUW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			auto rax = M_INVOKE (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<String<STRW>>>::id ,unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRUW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			auto rax = M_INVOKE (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<String<STRW>>>::id ,unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			using R2X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE1 = R1X () ;
			static constexpr auto M_INVOKE2 = R2X () ;
			return M_INVOKE2 (M_INVOKE1 (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRUW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<String<STRUW>>>::id ,unsafe_deptr (obj))) ;
			return M_INVOKE (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU8>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU16>> obj) const {
			String<STRU8> ret = String<STRU8> (obj.length () * 3) ;
			INDEX iw = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X007F))
						discard ;
					ret[iw++] = STRU8 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X07FF))
						discard ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i >= STRU16 (0XD800))
						discard ;
					if ifnot (i <= STRU16 (0XDBFF))
						discard ;
					rbx = STRU32 (i & STRU16 (0X03FF)) ;
					rax = 1 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i >= STRU16 (0XDC00))
						discard ;
					if ifnot (i <= STRU16 (0XDFFF))
						discard ;
					rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
					ret[iw++] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
					ret[iw++] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
					rax = 0 ;
				}
				if ifswitch (eax) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[iw++] = STRU8 ('?') ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU32>> obj) const {
			/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
			String<STRU8> ret = String<STRU8> (obj.length () * 6) ;
			INDEX iw = 0 ;
			auto rax = ZERO ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000007F))
						discard ;
					ret[iw++] = STRU8 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X000007FF))
						discard ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000FFFF))
						discard ;
					ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X001FFFFF))
						discard ;
					ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
					ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X03FFFFFF))
						discard ;
					ret[iw++] = (STRU8 (i >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
					ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[iw++] = (STRU8 (i >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
					ret[iw++] = (STRU8 (i >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ret[iw++] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
				}
				if ifswitch (eax) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			using R2X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE1 = R1X () ;
			static constexpr auto M_INVOKE2 = R2X () ;
			return M_INVOKE2 (M_INVOKE1 (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRUW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<String<STRUW>>>::id ,unsafe_deptr (obj))) ;
			return M_INVOKE (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU8>> obj) const {
			String<STRU16> ret = String<STRU16> (obj.length ()) ;
			INDEX iw = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0X7F))
						discard ;
					ret[iw++] = STRU16 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XDF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X1F)) ;
					rax = 1 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XEF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X0F)) ;
					rax = 2 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XF7))
						discard ;
					rbx = STRU32 (i & STRU8 (0X07)) ;
					rax = 3 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFB))
						discard ;
					rbx = STRU32 (i & STRU8 (0X03)) ;
					rax = 4 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFD))
						discard ;
					rbx = STRU32 (i & STRU8 (0X01)) ;
					rax = 5 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax = 10 ;
				}
				if ifswitch (eax) {
					if ifnot (vbetween (rax ,2 ,6))
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifswitch (eax) {
					ret.clear () ;
					rax = NONE ;
				}
				if (rax < 10)
					continue ;
				auto ebx = TRUE ;
				if ifswitch (ebx) {
					if ifnot (rax == 10)
						discard ;
					if ifnot (rbx <= STRU32 (0X0000FFFF))
						discard ;
					ret[iw++] = STRU16 (rbx) ;
					rax = 0 ;
				}
				if ifswitch (ebx) {
					if ifnot (rax == 10)
						discard ;
					if ifnot (rbx <= STRU32 (0X0010FFFF))
						discard ;
					rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
					ret[iw++] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ret[iw++] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					rax = 0 ;
				}
				if ifswitch (ebx) {
					if ifnot (rax == 10)
						discard ;
					if ifnot (rbx <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[iw++] = STRU16 ('?') ;
					rax = 0 ;
				}
				if ifswitch (ebx) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[iw++] = STRU16 ('?') ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU16>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU32>> obj) const {
			/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
			String<STRU16> ret = String<STRU16> (obj.length () * 2) ;
			INDEX iw = 0 ;
			auto rax = ZERO ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000FFFF))
						discard ;
					ret[iw++] = STRU16 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0010FFFF))
						discard ;
					ret[iw++] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ret[iw++] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[iw++] = STRU16 ('?') ;
				}
				if ifswitch (eax) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
			using R2X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE1 = R1X () ;
			static constexpr auto M_INVOKE2 = R2X () ;
			return M_INVOKE2 (M_INVOKE1 (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_string_cvt_HELP<UNIT1 ,STRUW ,ALWAYS>::FUNCTION_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast (TYPEAS<TEMP<String<STRUW>>>::id ,unsafe_deptr (obj))) ;
			return M_INVOKE (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU8>> obj) const {
			/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
			String<STRU32> ret = String<STRU32> (obj.length ()) ;
			INDEX iw = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0X7F))
						discard ;
					ret[iw++] = STRU32 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XDF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X1F)) ;
					rax = 1 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XEF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X0F)) ;
					rax = 2 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XF7))
						discard ;
					rbx = STRU32 (i & STRU8 (0X07)) ;
					rax = 3 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFB))
						discard ;
					rbx = STRU32 (i & STRU8 (0X03)) ;
					rax = 4 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFD))
						discard ;
					rbx = STRU32 (i & STRU8 (0X01)) ;
					rax = 5 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					ret[iw++] = rbx ;
					rax = 0 ;
				}
				if ifswitch (eax) {
					if ifnot (vbetween (rax ,2 ,6))
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifswitch (eax) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[iw++] = STRU32 ('?') ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU16>> obj) const {
			/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
			String<STRU32> ret = String<STRU32> (obj.length ()) ;
			INDEX iw = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X07FF))
						discard ;
					ret[iw++] = STRU32 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i >= STRU16 (0XD800))
						discard ;
					if ifnot (i <= STRU16 (0XDBFF))
						discard ;
					rbx = STRU32 (i & STRU16 (0X03FF)) ;
					rax = 1 ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 0)
						discard ;
					ret[iw++] = STRU32 (i) ;
				}
				if ifswitch (eax) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i >= STRU16 (0XDC00))
						discard ;
					if ifnot (i <= STRU16 (0XDFFF))
						discard ;
					rbx = STRU32 (((rbx << 10) | (i & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
					ret[iw++] = rbx ;
					rax = 0 ;
				}
				if ifswitch (eax) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				ret[iw++] = STRU32 ('?') ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU32>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using FUNCTION_string_cvt = typename FUNCTION_string_cvt_HELP<UNIT1 ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt ;
} ;
} ;

namespace CSC {
using namespace STRING ;
} ;