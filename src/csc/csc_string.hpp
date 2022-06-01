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
trait FUNCTION_string_parse_impl_HELP ;

template <class...>
trait FUNCTION_string_build_impl_HELP ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,BOOL>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,BOOL>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VAL32>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VAL32>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,VAL64>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,VAL64>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,SINGLE>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,SINGLE>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,DOUBLE>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,DOUBLE>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,BYTE>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,BYTE>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,WORD>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,WORD>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,CHAR>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,CHAR>>> {
	struct FUNCTION_string_build_impl {
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
trait FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT1 ,DATA>>> {
	struct FUNCTION_string_parse_impl {
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
trait FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_SAME<UNIT2 ,DATA>>> {
	struct FUNCTION_string_build_impl {
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
using FUNCTION_string_parse_impl = typename FUNCTION_string_parse_impl_HELP<UNIT1 ,UNIT2 ,ALWAYS>::FUNCTION_string_parse_impl ;

template <class...>
trait FUNCTION_string_parse_HELP ;

template <class DEPEND>
trait FUNCTION_string_parse_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_parse {
		template <class ARG1>
		inline constexpr FUNCTION_string_parse_impl<TYPE_FIRST_ONE<ARG1> ,TYPE_SECOND_ONE<ARG1>> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
			using R2X = TYPE_FIRST_ONE<ARG1> ;
			using R3X = TYPE_SECOND_ONE<ARG1> ;
			using R4X = FUNCTION_string_parse_impl<R2X ,R3X> ;
			return R4X () ;
		}
	} ;
} ;

using FUNCTION_string_parse = typename FUNCTION_string_parse_HELP<DEPEND ,ALWAYS>::FUNCTION_string_parse ;

static constexpr auto string_parse = FUNCTION_string_parse () ;

template <class UNIT1 ,class UNIT2>
using FUNCTION_string_build_impl = typename FUNCTION_string_build_impl_HELP<UNIT1 ,UNIT2 ,ALWAYS>::FUNCTION_string_build_impl ;

template <class...>
trait FUNCTION_string_build_HELP ;

template <class DEPEND>
trait FUNCTION_string_build_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_build {
		template <class ARG1>
		inline constexpr FUNCTION_string_build_impl<TYPE_FIRST_ONE<ARG1> ,TYPE_SECOND_ONE<ARG1>> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
			using R2X = TYPE_FIRST_ONE<ARG1> ;
			using R3X = TYPE_SECOND_ONE<ARG1> ;
			using R4X = FUNCTION_string_build_impl<R2X ,R3X> ;
			return R4X () ;
		}
	} ;
} ;

using FUNCTION_string_build = typename FUNCTION_string_build_HELP<DEPEND ,ALWAYS>::FUNCTION_string_build ;

static constexpr auto string_build = FUNCTION_string_build () ;

template <class...>
trait FUNCTION_string_cvt_impl_HELP ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRA> operator() (CREF<String<STRA>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			return StringProc::locale_string_cvt (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRA> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			using R2X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRA> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			using R2X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRA> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRA> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			using R2X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			return StringProc::locale_string_cvt (obj) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRW> operator() (CREF<String<STRW>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRW> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRUW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			auto rax = r1x (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRW> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRUW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			auto rax = r1x (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRW> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRW> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRUW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			auto rax = r1x (obj) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::expr] (unsafe_deptr (rax))) ;
			return move (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU8> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			using R2X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU8> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRUW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU8> operator() (CREF<String<STRU8>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt_impl {
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
			ret[ix] = 0 ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU8> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt_impl {
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
			ret[ix] = 0 ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU16> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			using R2X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU16> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRUW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt_impl {
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
			ret[ix] = 0 ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU16> operator() (CREF<String<STRU16>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt_impl {
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
			ret[ix] = 0 ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRA>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU32> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<STRW ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;
			using R2X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRW>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU32> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,STRUW ,ALWAYS>::FUNCTION_string_cvt_impl ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::expr] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt_impl {
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
			ret[ix] = 0 ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt_impl {
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
			ret[ix] = 0 ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU32>>>> {
	struct FUNCTION_string_cvt_impl {
		inline String<STRU32> operator() (CREF<String<STRU32>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
using FUNCTION_string_cvt_impl = typename FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,ALWAYS>::FUNCTION_string_cvt_impl ;

template <class...>
trait FUNCTION_string_cvt_HELP ;

template <class DEPEND>
trait FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_cvt {
		template <class ARG1>
		inline constexpr FUNCTION_string_cvt_impl<TYPE_FIRST_ONE<ARG1> ,TYPE_SECOND_ONE<ARG1>> operator[] (CREF<TYPEID<ARG1>> id) const noexcept {
			using R2X = TYPE_FIRST_ONE<ARG1> ;
			using R3X = TYPE_SECOND_ONE<ARG1> ;
			using R4X = FUNCTION_string_cvt_impl<R2X ,R3X> ;
			return R4X () ;
		}
	} ;
} ;

using FUNCTION_string_cvt = typename FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt ;

static constexpr auto string_cvt = FUNCTION_string_cvt () ;

template <class...>
trait REGULARSTRING_HELP ;

template <class DEPEND>
trait REGULARSTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	class EscapeString implement Binder {
	protected:
		String<STRU32> mText ;

	public:
		implicit EscapeString () = delete ;

		explicit EscapeString (CREF<Slice<STR>> text) {
			mText = String<STRU32>::make (text) ;
		}

		explicit EscapeString (CREF<String<STRA>> text) {
			mText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRA>>::expr] (text) ;
		}

		explicit EscapeString (CREF<String<STRW>> text) {
			mText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRW>>::expr] (text) ;
		}

		explicit EscapeString (CREF<String<STRU8>> text) {
			mText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::expr] (text) ;
		}

		explicit EscapeString (CREF<String<STRU16>> text) {
			mText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU16>>::expr] (text) ;
		}

		explicit EscapeString (CREF<String<STRU32>> text) {
			mText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU32>>::expr] (text) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			template_write (TYPEAS<STRA>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			template_write (TYPEAS<STRW>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			template_write (TYPEAS<STRU8>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			template_write (TYPEAS<STRU16>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			template_write (TYPEAS<STRU32>::expr ,writer) ;
		}

		template <class ARG1 ,class ARG2>
		void template_write (CREF<TYPEID<ARG1>> id ,VREF<ARG2> writer) const {
			const auto r1x = writer.get_attr () ;
			writer << ARG1 ('\"') ;
			for (auto &&i : mText) {
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					const auto r2x = r1x->escape_cast (ARG1 (i)) ;
					if ifnot (r2x.exist ())
						discard ;
					writer << ARG1 ('\\') ;
					writer << r2x.fetch () ;
				}
				if ifswitch (rxx) {
					writer << ARG1 (i) ;
				}
			}
			writer << ARG1 ('\"') ;
		}
	} ;
} ;

using EscapeString = typename REGULARSTRING_HELP<DEPEND ,ALWAYS>::EscapeString ;

template <class...>
trait REPEATSTRING_HELP ;

template <class DEPEND>
trait REPEATSTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class RepeatString implement Binder {
	protected:
		String<STRU32> mGapText ;
		String<STRU32> mCommaText ;
		LENGTH mCounter ;
		Cell<LENGTH> mTightCounter ;
		Deque<Cell<BOOL>> mFirst ;

	public:
		implicit RepeatString () = delete ;

		explicit RepeatString (CREF<Slice<STR>> gap_text ,CREF<Slice<STR>> comma_text) {
			mGapText = String<STRU32>::make (gap_text) ;
			mCommaText = String<STRU32>::make (comma_text) ;
			initialize () ;
		}

		explicit RepeatString (CREF<String<STRA>> gap_text ,CREF<String<STRA>> comma_text) {
			mGapText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRA>>::expr] (gap_text) ;
			mCommaText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRA>>::expr] (comma_text) ;
			initialize () ;
		}

		explicit RepeatString (CREF<String<STRW>> gap_text ,CREF<String<STRW>> comma_text) {
			mGapText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRW>>::expr] (gap_text) ;
			mCommaText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRW>>::expr] (comma_text) ;
			initialize () ;
		}

		explicit RepeatString (CREF<String<STRU8>> gap_text ,CREF<String<STRU8>> comma_text) {
			mGapText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::expr] (gap_text) ;
			mCommaText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::expr] (comma_text) ;
			initialize () ;
		}

		explicit RepeatString (CREF<String<STRU16>> gap_text ,CREF<String<STRU16>> comma_text) {
			mGapText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU16>>::expr] (gap_text) ;
			mCommaText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU16>>::expr] (comma_text) ;
			initialize () ;
		}

		explicit RepeatString (CREF<String<STRU32>> gap_text ,CREF<String<STRU32>> comma_text) {
			mGapText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU32>>::expr] (gap_text) ;
			mCommaText = string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU32>>::expr] (comma_text) ;
			initialize () ;
		}

		void initialize () {
			mCounter = 0 ;
			mTightCounter = Cell<LENGTH>::make (COUNTER_MAX_DEPTH::expr) ;
			mFirst = Deque<Cell<BOOL>> (COUNTER_MAX_DEPTH::expr * 2) ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			template_write (TYPEAS<STRA>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			template_write (TYPEAS<STRW>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			template_write (TYPEAS<STRU8>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			template_write (TYPEAS<STRU16>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			template_write (TYPEAS<STRU32>::expr ,writer) ;
		}

		template <class ARG1 ,class ARG2>
		void template_write (CREF<TYPEID<ARG1>> id ,VREF<ARG2> writer) const {
			INDEX ix = mFirst.tail () ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mCounter > mTightCounter.fetch ())
					discard ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					for (auto &&j : mCommaText)
						writer << ARG1 (j) ;
				}
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (rxx) {
				if (mCounter != mTightCounter.fetch ())
					discard ;
				if ifnot (mFirst[ix].fetch ())
					discard ;
				mTightCounter.store (COUNTER_MAX_DEPTH::expr) ;
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (rxx) {
				writer << TextWriter<ARG1>::GAP ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					for (auto &&j : mCommaText)
						writer << ARG1 (j) ;
				}
				mFirst[ix].store (FALSE) ;
				for (auto &&i : iter (0 ,mCounter)) {
					noop (i) ;
					for (auto &&j : mGapText)
						writer << ARG1 (j) ;
				}
			}
		}

		void tight () {
			mTightCounter.store (mCounter) ;
		}

		void enter () {
			assume (mCounter < COUNTER_MAX_DEPTH::expr) ;
			mCounter++ ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		void leave () {
			mFirst.pop () ;
			INDEX ix = mFirst.tail () ;
			mFirst[ix].store (TRUE) ;
			mCounter-- ;
		}
	} ;
} ;

using RepeatString = typename REPEATSTRING_HELP<DEPEND ,ALWAYS>::RepeatString ;

template <class...>
trait ALIGNEDSTRING_HELP ;

template <class DEPEND>
trait ALIGNEDSTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	class AlignedString implement Binder {
	protected:
		VAL64 mValue ;
		LENGTH mAlign ;
		LENGTH mSpace ;

	public:
		implicit AlignedString () = delete ;

		explicit AlignedString (CREF<VAL64> value_ ,CREF<LENGTH> align_) {
			mValue = value_ ;
			mAlign = align_ ;
			mSpace = mAlign - 1 - MathProc::log10v (mValue) ;
			assume (mSpace >= 0) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			template_write (TYPEAS<STRA>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			template_write (TYPEAS<STRW>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			template_write (TYPEAS<STRU8>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			template_write (TYPEAS<STRU16>::expr ,writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			template_write (TYPEAS<STRU32>::expr ,writer) ;
		}

		template <class ARG1 ,class ARG2>
		void template_write (CREF<TYPEID<ARG1>> id ,VREF<ARG2> writer) const {
			for (auto &&i : iter (0 ,mSpace)) {
				noop (i) ;
				writer << slice ("0") ;
			}
			writer << mValue ;
		}
	} ;
} ;

using AlignedString = typename ALIGNEDSTRING_HELP<DEPEND ,ALWAYS>::AlignedString ;
} ;