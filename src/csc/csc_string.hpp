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
		inline constexpr FUNCTION_string_parse_impl<TYPE_FIRST_ONE<REMOVE_ALL<ARG1>> ,TYPE_SECOND_ONE<REMOVE_ALL<ARG1>>> operator[] (CREF<ARG1> id) const noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = TYPE_FIRST_ONE<R1X> ;
			using R3X = TYPE_SECOND_ONE<R1X> ;
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
		inline constexpr FUNCTION_string_build_impl<TYPE_FIRST_ONE<REMOVE_ALL<ARG1>> ,TYPE_SECOND_ONE<REMOVE_ALL<ARG1>>> operator[] (CREF<ARG1> id) const noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = TYPE_FIRST_ONE<R1X> ;
			using R3X = TYPE_SECOND_ONE<R1X> ;
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
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::id] (unsafe_deptr (rax))) ;
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
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::id] (unsafe_deptr (rax))) ;
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
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRW>>>::id] (unsafe_deptr (rax))) ;
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
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::id] (unsafe_deptr (obj))) ;
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
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::id] (unsafe_deptr (obj))) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU16> ,IS_SAME<UNIT2 ,STRU8>>>> {
	struct FUNCTION_string_cvt_impl {
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
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<String<STRUW>>>::id] (unsafe_deptr (obj))) ;
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
trait FUNCTION_string_cvt_impl_HELP<UNIT1 ,UNIT2 ,REQUIRE<ENUM_ALL<IS_SAME<UNIT1 ,STRU32> ,IS_SAME<UNIT2 ,STRU16>>>> {
	struct FUNCTION_string_cvt_impl {
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
		inline constexpr FUNCTION_string_cvt_impl<TYPE_FIRST_ONE<REMOVE_ALL<ARG1>> ,TYPE_SECOND_ONE<REMOVE_ALL<ARG1>>> operator[] (CREF<ARG1> id) const noexcept {
			using R1X = REMOVE_ALL<ARG1> ;
			using R2X = TYPE_FIRST_ONE<R1X> ;
			using R3X = TYPE_SECOND_ONE<R1X> ;
			using R4X = FUNCTION_string_cvt_impl<R2X ,R3X> ;
			return R4X () ;
		}
	} ;
} ;

using FUNCTION_string_cvt = typename FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt ;

static constexpr auto string_cvt = FUNCTION_string_cvt () ;

template <class...>
trait REGULARSTRING_HELP ;

template <class ITEM>
trait REGULARSTRING_HELP<ITEM ,ALWAYS> {
	class EscapeString extend Proxy {
	protected:
		String<ITEM> mBase ;

	public:
		imports CREF<EscapeString> from (CREF<String<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<EscapeString>>::id] (unsafe_deptr (that))) ;
		}

		imports CREF<EscapeString> from (RREF<String<ITEM>>) = delete ;

		inline friend VREF<TextWriter<ITEM>> operator<< (VREF<TextWriter<ITEM>> wos ,CREF<EscapeString> thiz_) {
			const auto r1x = wos.get_attr () ;
			wos << ITEM ('\"') ;
			for (auto &&i : thiz_.mBase) {
				auto eax = TRUE ;
				if ifswitch (eax) {
					const auto r2x = r1x->escape_cast (i) ;
					if ifnot (r2x.exist ())
						discard ;
					wos << ITEM ('\\') ;
					wos << r2x.self ;
				}
				if ifswitch (eax) {
					wos << i ;
				}
			}
			wos << ITEM ('\"') ;
			return wos ;
		}
	} ;
} ;

template <class ITEM>
using EscapeString = typename REGULARSTRING_HELP<ITEM ,ALWAYS>::EscapeString ;

template <class...>
trait REPEATSTRING_HELP ;

template <class ITEM>
trait REPEATSTRING_HELP<ITEM ,ALWAYS> {
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class RepeatString implement Binder {
	protected:
		String<ITEM> mGapText ;
		String<ITEM> mCommaText ;
		LENGTH mCounter ;
		Cell<LENGTH> mTightCounter ;
		Deque<Cell<BOOL>> mFirst ;

	public:
		implicit RepeatString () = delete ;

		explicit RepeatString (CREF<Slice<STRA>> gap_text ,CREF<Slice<STRA>> comma_text) {
			mGapText = String<ITEM> (gap_text.size ()) ;
			for (auto &&i : iter (0 ,gap_text.size ())) {
				vbetween (INDEX (gap_text[i]) ,0 ,128) ;
				mGapText[i] = ITEM (gap_text[i]) ;
			}
			mCommaText = String<ITEM> (comma_text.size ()) ;
			for (auto &&i : iter (0 ,comma_text.size ())) {
				vbetween (INDEX (comma_text[i]) ,0 ,128) ;
				mCommaText[i] = ITEM (comma_text[i]) ;
			}
			mCounter = 0 ;
			mTightCounter = Cell<LENGTH>::make (COUNTER_MAX_DEPTH::value) ;
			mFirst = Deque<Cell<BOOL>> (COUNTER_MAX_DEPTH::value * 2) ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		explicit RepeatString (CREF<Slice<STRW>> gap_text ,CREF<Slice<STRW>> comma_text) {
			mGapText = String<ITEM> (gap_text.size ()) ;
			for (auto &&i : iter (0 ,gap_text.size ())) {
				vbetween (INDEX (gap_text[i]) ,0 ,128) ;
				mGapText[i] = ITEM (gap_text[i]) ;
			}
			mCommaText = String<ITEM> (comma_text.size ()) ;
			for (auto &&i : iter (0 ,comma_text.size ())) {
				vbetween (INDEX (comma_text[i]) ,0 ,128) ;
				mCommaText[i] = ITEM (comma_text[i]) ;
			}
			mCounter = 0 ;
			mTightCounter = Cell<LENGTH>::make (COUNTER_MAX_DEPTH::value) ;
			mFirst = Deque<Cell<BOOL>> (COUNTER_MAX_DEPTH::value * 2) ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		void friend_write (VREF<TextWriter<ITEM>> writer) const override {
			INDEX ix = mFirst.tail () ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mCounter > mTightCounter.fetch ())
					discard ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					writer << mCommaText ;
				}
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (eax) {
				if (mCounter != mTightCounter.fetch ())
					discard ;
				if ifnot (mFirst[ix].fetch ())
					discard ;
				mTightCounter.store (COUNTER_MAX_DEPTH::value) ;
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (eax) {
				writer << TextWriter<ITEM>::GAP ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					writer << mCommaText ;
				}
				mFirst[ix].store (FALSE) ;
				for (auto &&i : iter (0 ,mCounter)) {
					noop (i) ;
					writer << mGapText ;
				}
			}
		}

		void tight () {
			mTightCounter.store (mCounter) ;
		}

		void enter () {
			assume (mCounter < COUNTER_MAX_DEPTH::value) ;
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

template <class ITEM>
using RepeatString = typename REPEATSTRING_HELP<ITEM ,ALWAYS>::RepeatString ;

template <class...>
trait ALIGNEDSTRING_HELP ;

template <class ITEM>
trait ALIGNEDSTRING_HELP<ITEM ,ALWAYS> {
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;

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

		void friend_write (VREF<TextWriter<ITEM>> writer) const override {
			for (auto &&i : iter (0 ,mSpace)) {
				noop (i) ;
				writer << slice ("0") ;
			}
			writer << mValue ;
		}
	} ;
} ;

template <class ITEM>
using AlignedString = typename ALIGNEDSTRING_HELP<ITEM ,ALWAYS>::AlignedString ;
} ;