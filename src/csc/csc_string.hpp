#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"

namespace CSC {
template <class...>
trait STRINGPROC_HELP ;

template <class...>
trait STRINGPROC_IMPLHOLDER_HELP ;

template <class DEPEND>
trait STRINGPROC_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual String<STRA> ansi_string_cvt (CREF<String<STRW>> obj) const = 0 ;
		virtual String<STRW> ansi_string_cvt (CREF<String<STRA>> obj) const = 0 ;
		virtual String<STRA> gbks_string_cvt (CREF<String<STRW>> obj) const = 0 ;
		virtual String<STRW> gbks_string_cvt (CREF<String<STRA>> obj) const = 0 ;
		virtual String<STRA> locale_string_cvt (CREF<String<STRW>> obj) const = 0 ;
		virtual String<STRW> locale_string_cvt (CREF<String<STRA>> obj) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class StringProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<StringProc> instance () {
			return memorize ([&] () {
				StringProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports String<STRW> ansi_string_cvt (CREF<String<STRA>> obj) {
			return instance ().mThis->ansi_string_cvt (obj) ;
		}

		imports String<STRA> ansi_string_cvt (CREF<String<STRW>> obj) {
			return instance ().mThis->ansi_string_cvt (obj) ;
		}

		imports String<STRW> gbks_string_cvt (CREF<String<STRA>> obj) {
			return instance ().mThis->gbks_string_cvt (obj) ;
		}

		imports String<STRA> gbks_string_cvt (CREF<String<STRW>> obj) {
			return instance ().mThis->gbks_string_cvt (obj) ;
		}

		imports String<STRW> locale_string_cvt (CREF<String<STRA>> obj) {
			return instance ().mThis->locale_string_cvt (obj) ;
		}

		imports String<STRA> locale_string_cvt (CREF<String<STRW>> obj) {
			return instance ().mThis->locale_string_cvt (obj) ;
		}
	} ;
} ;

using StringProc = typename STRINGPROC_HELP<DEPEND ,ALWAYS>::StringProc ;

template <class...>
trait TEMPLATE_string_parse_HELP ;

template <class...>
trait TEMPLATE_string_build_HELP ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,BOOL>>> {
	struct TEMPLATE_string_parse {
		inline BOOL operator() (CREF<String<UNIT2>> obj) const {
			BOOL ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,BOOL>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<BOOL> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VAL32>>> {
	struct TEMPLATE_string_parse {
		inline VAL32 operator() (CREF<String<UNIT2>> obj) const {
			VAL32 ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VAL32>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<VAL32> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VAL64>>> {
	struct TEMPLATE_string_parse {
		inline VAL64 operator() (CREF<String<UNIT2>> obj) const {
			VAL64 ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VAL64>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<VAL64> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,SINGLE>>> {
	struct TEMPLATE_string_parse {
		inline SINGLE operator() (CREF<String<UNIT2>> obj) const {
			SINGLE ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,SINGLE>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<SINGLE> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,DOUBLE>>> {
	struct TEMPLATE_string_parse {
		inline DOUBLE operator() (CREF<String<UNIT2>> obj) const {
			DOUBLE ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,DOUBLE>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<DOUBLE> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,BYTE>>> {
	struct TEMPLATE_string_parse {
		inline BYTE operator() (CREF<String<UNIT2>> obj) const {
			BYTE ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,BYTE>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<BYTE> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,WORD>>> {
	struct TEMPLATE_string_parse {
		inline WORD operator() (CREF<String<UNIT2>> obj) const {
			WORD ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,WORD>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<WORD> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,CHAR>>> {
	struct TEMPLATE_string_parse {
		inline CHAR operator() (CREF<String<UNIT2>> obj) const {
			CHAR ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,CHAR>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<CHAR> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,DATA>>> {
	struct TEMPLATE_string_parse {
		inline DATA operator() (CREF<String<UNIT2>> obj) const {
			DATA ret ;
			auto rax = TextReader<UNIT2> (obj.raw ()) ;
			rax >> ret ;
			rax >> TextReader<UNIT2>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,DATA>>> {
	struct TEMPLATE_string_build {
		inline String<UNIT1> operator() (CREF<DATA> obj) const {
			String<UNIT1> ret = String<UNIT1> (64) ;
			auto rax = TextWriter<UNIT1> (ret.raw ()) ;
			rax << obj ;
			rax << TextWriter<UNIT1>::EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using TEMPLATE_string_parse = typename TEMPLATE_string_parse_HELP<UNIT1 ,UNIT2 ,ALWAYS>::TEMPLATE_string_parse ;

template <class...>
trait FUNCTION_string_parse_HELP ;

template <class DEPEND>
trait FUNCTION_string_parse_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_parse {
		template <class ARG1>
		inline constexpr TEMPLATE_string_parse<TYPE_FIRST_ONE<ARG1> ,TYPE_SECOND_ONE<ARG1>> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
			using R1X = TYPE_FIRST_ONE<ARG1> ;
			using R2X = TYPE_SECOND_ONE<ARG1> ;
			using R3X = TEMPLATE_string_parse<R1X ,R2X> ;
			return R3X () ;
		}
	} ;
} ;

using FUNCTION_string_parse = typename FUNCTION_string_parse_HELP<DEPEND ,ALWAYS>::FUNCTION_string_parse ;

static constexpr auto string_parse = FUNCTION_string_parse () ;

template <class UNIT1 ,class UNIT2>
using TEMPLATE_string_build = typename TEMPLATE_string_build_HELP<UNIT1 ,UNIT2 ,ALWAYS>::TEMPLATE_string_build ;

template <class...>
trait FUNCTION_string_build_HELP ;

template <class DEPEND>
trait FUNCTION_string_build_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_build {
		template <class ARG1>
		inline constexpr TEMPLATE_string_build<TYPE_FIRST_ONE<ARG1> ,TYPE_SECOND_ONE<ARG1>> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
			using R1X = TYPE_FIRST_ONE<ARG1> ;
			using R2X = TYPE_SECOND_ONE<ARG1> ;
			using R3X = TEMPLATE_string_build<R1X ,R2X> ;
			return R3X () ;
		}
	} ;
} ;

using FUNCTION_string_build = typename FUNCTION_string_build_HELP<DEPEND ,ALWAYS>::FUNCTION_string_build ;

static constexpr auto string_build = FUNCTION_string_build () ;

template <class...>
trait TEMPLATE_string_cvt_HELP ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRA>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			return StringProc::locale_string_cvt (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			return StringProc::locale_string_cvt (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRW>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto rax = r1x (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto rax = r1x (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto rax = r1x (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU8>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU16>> obj) const {
			String<STRU8> ret = String<STRU8> (obj.length () * 3) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X007F))
						discard ;
					ret[ix] = STRU8 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X07FF))
						discard ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i >= STRU16 (0XD800))
						discard ;
					if ifnot (i <= STRU16 (0XDBFF))
						discard ;
					rbx = STRU32 (i & STRU16 (0X03FF)) ;
					rax = 1 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					ret[ix] = (STRU8 (i >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
					ix++ ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (rxx) {
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
				if ifswitch (rxx) {
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
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
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
			INDEX ix = 0 ;
			auto rax = ZERO ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000007F))
						discard ;
					ret[ix] = STRU8 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X000007FF))
						discard ;
					ret[ix] = (STRU8 (i >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
					ix++ ;
					ret[ix] = (STRU8 (i) & STRU8 (0X3F)) | STRU8 (0X80) ;
					ix++ ;
				}
				if ifswitch (rxx) {
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
				if ifswitch (rxx) {
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
				if ifswitch (rxx) {
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
				if ifswitch (rxx) {
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
				if ifswitch (rxx) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU8>> obj) const {
			String<STRU16> ret = String<STRU16> (obj.length ()) ;
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0X7F))
						discard ;
					ret[ix] = STRU16 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XDF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X1F)) ;
					rax = 1 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XEF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X0F)) ;
					rax = 2 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XF7))
						discard ;
					rbx = STRU32 (i & STRU8 (0X07)) ;
					rax = 3 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFB))
						discard ;
					rbx = STRU32 (i & STRU8 (0X03)) ;
					rax = 4 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFD))
						discard ;
					rbx = STRU32 (i & STRU8 (0X01)) ;
					rax = 5 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax = 10 ;
				}
				if ifswitch (rxx) {
					if ifnot (vbetween (rax ,2 ,6))
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifswitch (rxx) {
					ret.clear () ;
					rax = NONE ;
				}
				if (rax < 10)
					continue ;
				auto ryx = TRUE ;
				if ifswitch (ryx) {
					if ifnot (rax == 10)
						discard ;
					if ifnot (rbx <= STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (rbx) ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (ryx) {
					if ifnot (rax == 10)
						discard ;
					if ifnot (rbx <= STRU32 (0X0010FFFF))
						discard ;
					rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
					ret[ix] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (ryx) {
					if ifnot (rax == 10)
						discard ;
					if ifnot (rbx <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 ('?') ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (ryx) {
					ret.clear () ;
					rax = NONE ;
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
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU16>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU32>> obj) const {
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
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X0010FFFF))
						discard ;
					ret[ix] = (STRU16 ((i - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (i - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 ('?') ;
					ix++ ;
				}
				if ifswitch (rxx) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<UNIT1 ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
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
			INDEX ix = 0 ;
			auto rax = ZERO ;
			auto rbx = STRU32 () ;
			for (auto &&i : obj) {
				if (rax == NONE)
					continue ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0X7F))
						discard ;
					ret[ix] = STRU32 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XDF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X1F)) ;
					rax = 1 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XEF))
						discard ;
					rbx = STRU32 (i & STRU8 (0X0F)) ;
					rax = 2 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XF7))
						discard ;
					rbx = STRU32 (i & STRU8 (0X07)) ;
					rax = 3 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFB))
						discard ;
					rbx = STRU32 (i & STRU8 (0X03)) ;
					rax = 4 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU8 (0XFD))
						discard ;
					rbx = STRU32 (i & STRU8 (0X01)) ;
					rax = 5 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					ret[ix] = rbx ;
					ix++ ;
					rax = 0 ;
				}
				if ifswitch (rxx) {
					if ifnot (vbetween (rax ,2 ,6))
						discard ;
					if ifnot (i <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (i & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifswitch (rxx) {
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

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU16>> obj) const {
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
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i <= STRU16 (0X07FF))
						discard ;
					ret[ix] = STRU32 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (i >= STRU16 (0XD800))
						discard ;
					if ifnot (i <= STRU16 (0XDBFF))
						discard ;
					rbx = STRU32 (i & STRU16 (0X03FF)) ;
					rax = 1 ;
				}
				if ifswitch (rxx) {
					if ifnot (rax == 0)
						discard ;
					ret[ix] = STRU32 (i) ;
					ix++ ;
				}
				if ifswitch (rxx) {
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
				if ifswitch (rxx) {
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

template <class UNIT1 ,class UNIT2>
trait TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU32>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using TEMPLATE_string_cvt = typename TEMPLATE_string_cvt_HELP<UNIT1 ,UNIT2 ,ALWAYS>::TEMPLATE_string_cvt ;

template <class...>
trait FUNCTION_string_cvt_HELP ;

template <class DEPEND>
trait FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_cvt {
		template <class ARG1>
		inline constexpr TEMPLATE_string_cvt<TYPE_FIRST_ONE<ARG1> ,TYPE_SECOND_ONE<ARG1>> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
			using R1X = TYPE_FIRST_ONE<ARG1> ;
			using R2X = TYPE_SECOND_ONE<ARG1> ;
			using R3X = TEMPLATE_string_cvt<R1X ,R2X> ;
			return R3X () ;
		}
	} ;
} ;

using FUNCTION_string_cvt = typename FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt ;

static constexpr auto string_cvt = FUNCTION_string_cvt () ;

template <class...>
trait ANYSTRING_HELP ;

template <class...>
trait ANYSTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ANYSTRING_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<Slice<STR>> text) = 0 ;
		virtual void initialize (CREF<String<STRA>> text) = 0 ;
		virtual void initialize (CREF<String<STRW>> text) = 0 ;
		virtual void initialize (CREF<String<STRU8>> text) = 0 ;
		virtual void initialize (CREF<String<STRU16>> text) = 0 ;
		virtual void initialize (CREF<String<STRU32>> text) = 0 ;
		virtual CREF<String<STRA>> pick (CREF<TYPEID<STRA>> id) const leftvalue = 0 ;
		virtual CREF<String<STRW>> pick (CREF<TYPEID<STRW>> id) const leftvalue = 0 ;
		virtual CREF<String<STRU8>> pick (CREF<TYPEID<STRU8>> id) const leftvalue = 0 ;
		virtual CREF<String<STRU16>> pick (CREF<TYPEID<STRU16>> id) const leftvalue = 0 ;
		virtual CREF<String<STRU32>> pick (CREF<TYPEID<STRU32>> id) const leftvalue = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		Cell<CRef<String<STR>>> mText ;
		Cell<CRef<String<STRA>>> mTextA ;
		Cell<CRef<String<STRW>>> mTextW ;
		Cell<CRef<String<STRU8>>> mTextU8 ;
		Cell<CRef<String<STRU16>>> mTextU16 ;
		Cell<CRef<String<STRU32>>> mTextU32 ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class AnyString {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit AnyString () = default ;

		explicit AnyString (CREF<Slice<STR>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (text) ;
		}

		explicit AnyString (CREF<String<STRA>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (text) ;
		}

		explicit AnyString (CREF<String<STRW>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (text) ;
		}

		explicit AnyString (CREF<String<STRU8>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (text) ;
		}

		explicit AnyString (CREF<String<STRU16>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (text) ;
		}

		explicit AnyString (CREF<String<STRU32>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (text) ;
		}

		CREF<String<STRA>> pick (CREF<TYPEID<STRA>> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRW>> pick (CREF<TYPEID<STRW>> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRU8>> pick (CREF<TYPEID<STRU8>> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRU16>> pick (CREF<TYPEID<STRU16>> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRU32>> pick (CREF<TYPEID<STRU32>> id) const leftvalue {
			return mThis->pick (id) ;
		}
	} ;
} ;

using AnyString = typename ANYSTRING_HELP<DEPEND ,ALWAYS>::AnyString ;

template <class...>
trait ESCAPESTRING_HELP ;

template <class...>
trait ESCAPESTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ESCAPESTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	struct Holder implement Interface {
		virtual void initialize (RREF<AnyString> text) = 0 ;
		virtual void write_text (VREF<TextWriter<STRA>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRW>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU8>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU16>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU32>> writer) const = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		AnyString mText ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class EscapeString implement Binder {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit EscapeString () = default ;

		explicit EscapeString (CREF<Slice<STR>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (text)) ;
		}

		explicit EscapeString (CREF<String<STRA>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (text)) ;
		}

		explicit EscapeString (CREF<String<STRW>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (text)) ;
		}

		explicit EscapeString (CREF<String<STRU8>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (text)) ;
		}

		explicit EscapeString (CREF<String<STRU16>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (text)) ;
		}

		explicit EscapeString (CREF<String<STRU32>> text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (text)) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			return mThis->write_text (writer) ;
		}
	} ;
} ;

using EscapeString = typename ESCAPESTRING_HELP<DEPEND ,ALWAYS>::EscapeString ;

template <class...>
trait REPEATSTRING_HELP ;

template <class...>
trait REPEATSTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait REPEATSTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	struct Holder implement Interface {
		virtual void initialize (RREF<AnyString> gap_text ,RREF<AnyString> comma_text) = 0 ;
		virtual void write_text (VREF<TextWriter<STRA>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRW>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU8>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU16>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU32>> writer) const = 0 ;
		virtual void tight () = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	class FakeHolder implement Interface {
	protected:
		AnyString mGapText ;
		AnyString mCommaText ;
		LENGTH mCounter ;
		Cell<LENGTH> mTightCounter ;
		Deque<Cell<BOOL>> mFirst ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class RepeatString implement Binder {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit RepeatString () = default ;

		explicit RepeatString (CREF<Slice<STR>> gap_text ,CREF<Slice<STR>> comma_text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (gap_text) ,AnyString (comma_text)) ;
		}

		explicit RepeatString (CREF<String<STRA>> gap_text ,CREF<String<STRA>> comma_text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (gap_text) ,AnyString (comma_text)) ;
		}

		explicit RepeatString (CREF<String<STRW>> gap_text ,CREF<String<STRW>> comma_text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (gap_text) ,AnyString (comma_text)) ;
		}

		explicit RepeatString (CREF<String<STRU8>> gap_text ,CREF<String<STRU8>> comma_text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (gap_text) ,AnyString (comma_text)) ;
		}

		explicit RepeatString (CREF<String<STRU16>> gap_text ,CREF<String<STRU16>> comma_text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (gap_text) ,AnyString (comma_text)) ;
		}

		explicit RepeatString (CREF<String<STRU32>> gap_text ,CREF<String<STRU32>> comma_text) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (AnyString (gap_text) ,AnyString (comma_text)) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void tight () {
			return mThis->tight () ;
		}

		void enter () {
			return mThis->enter () ;
		}

		void leave () {
			return mThis->leave () ;
		}
	} ;
} ;

using RepeatString = typename REPEATSTRING_HELP<DEPEND ,ALWAYS>::RepeatString ;

template <class...>
trait ALIGNEDSTRING_HELP ;

template <class...>
trait ALIGNEDSTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ALIGNEDSTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	struct Holder implement Interface {
		virtual void initialize (CREF<VAL64> value_ ,CREF<LENGTH> align_) = 0 ;
		virtual void write_text (VREF<TextWriter<STRA>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRW>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU8>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU16>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU32>> writer) const = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		VAL64 mValue ;
		LENGTH mAlign ;
		LENGTH mSpace ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class AlignedString implement Binder {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit AlignedString () = default ;

		explicit AlignedString (CREF<VAL64> value_ ,CREF<LENGTH> align_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (value_ ,align_) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			return mThis->write_text (writer) ;
		}
	} ;
} ;

using AlignedString = typename ALIGNEDSTRING_HELP<DEPEND ,ALWAYS>::AlignedString ;
} ;