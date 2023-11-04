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
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual String<STRA> string_cvt_ansi_from_w (CREF<String<STRW>> a) const = 0 ;
		virtual String<STRW> string_cvt_w_from_ansi (CREF<String<STRA>> a) const = 0 ;
		virtual String<STRA> string_cvt_gbks_from_w (CREF<String<STRW>> a) const = 0 ;
		virtual String<STRW> string_cvt_w_from_gbks (CREF<String<STRA>> a) const = 0 ;
		virtual String<STRA> string_cvt_a_from_w (CREF<String<STRW>> a) const = 0 ;
		virtual String<STRW> string_cvt_w_from_a (CREF<String<STRA>> a) const = 0 ;
		virtual String<STRU8> string_cvt_u8_from_u16 (CREF<String<STRU16>> a) const = 0 ;
		virtual String<STRU8> string_cvt_u8_from_u32 (CREF<String<STRU32>> a) const = 0 ;
		virtual String<STRU16> string_cvt_u16_from_u8 (CREF<String<STRU8>> a) const = 0 ;
		virtual String<STRU16> string_cvt_u16_from_u32 (CREF<String<STRU32>> a) const = 0 ;
		virtual String<STRU32> string_cvt_u32_from_u8 (CREF<String<STRU8>> a) const = 0 ;
		virtual String<STRU32> string_cvt_u32_from_u16 (CREF<String<STRU16>> a) const = 0 ;
	} ;

	class StringProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<StringProc> instance () {
			return memorize ([&] () {
				StringProc ret ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		imports String<STRA> string_cvt_ansi_from_w (CREF<String<STRW>> a) {
			return instance ().mThis->string_cvt_ansi_from_w (a) ;
		}

		imports String<STRW> string_cvt_w_from_ansi (CREF<String<STRA>> a) {
			return instance ().mThis->string_cvt_w_from_ansi (a) ;
		}

		imports String<STRA> string_cvt_gbks_from_w (CREF<String<STRW>> a) {
			return instance ().mThis->string_cvt_gbks_from_w (a) ;
		}

		imports String<STRW> string_cvt_w_from_gbks (CREF<String<STRA>> a) {
			return instance ().mThis->string_cvt_w_from_gbks (a) ;
		}

		imports String<STRA> string_cvt_a_from_w (CREF<String<STRW>> a) {
			return instance ().mThis->string_cvt_a_from_w (a) ;
		}

		imports String<STRW> string_cvt_w_from_a (CREF<String<STRA>> a) {
			return instance ().mThis->string_cvt_w_from_a (a) ;
		}

		imports String<STRU8> string_cvt_u8_from_u16 (CREF<String<STRU16>> a) {
			return instance ().mThis->string_cvt_u8_from_u16 (a) ;
		}

		imports String<STRU8> string_cvt_u8_from_u32 (CREF<String<STRU32>> a) {
			return instance ().mThis->string_cvt_u8_from_u32 (a) ;
		}

		imports String<STRU16> string_cvt_u16_from_u8 (CREF<String<STRU8>> a) {
			return instance ().mThis->string_cvt_u16_from_u8 (a) ;
		}

		imports String<STRU16> string_cvt_u16_from_u32 (CREF<String<STRU32>> a) {
			return instance ().mThis->string_cvt_u16_from_u32 (a) ;
		}

		imports String<STRU32> string_cvt_u32_from_u8 (CREF<String<STRU8>> a) {
			return instance ().mThis->string_cvt_u32_from_u8 (a) ;
		}

		imports String<STRU32> string_cvt_u32_from_u16 (CREF<String<STRU16>> a) {
			return instance ().mThis->string_cvt_u32_from_u16 (a) ;
		}
	} ;
} ;

using StringProc = typename STRINGPROC_HELP<DEPEND ,ALWAYS>::StringProc ;

template <class...>
trait TEMPLATE_string_parse_HELP ;

template <class...>
trait TEMPLATE_string_build_HELP ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,BOOL>>> {
	struct TEMPLATE_string_parse {
		inline BOOL operator() (CREF<String<B>> a) const {
			BOOL ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,BOOL>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<BOOL> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,VAL32>>> {
	struct TEMPLATE_string_parse {
		inline VAL32 operator() (CREF<String<B>> a) const {
			VAL32 ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,VAL32>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<VAL32> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,VAL64>>> {
	struct TEMPLATE_string_parse {
		inline VAL64 operator() (CREF<String<B>> a) const {
			VAL64 ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,VAL64>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<VAL64> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,FLT32>>> {
	struct TEMPLATE_string_parse {
		inline FLT32 operator() (CREF<String<B>> a) const {
			FLT32 ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,FLT32>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<FLT32> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,FLT64>>> {
	struct TEMPLATE_string_parse {
		inline FLT64 operator() (CREF<String<B>> a) const {
			FLT64 ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,FLT64>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<FLT64> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,BYTE>>> {
	struct TEMPLATE_string_parse {
		inline BYTE operator() (CREF<String<B>> a) const {
			BYTE ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,BYTE>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<BYTE> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,WORD>>> {
	struct TEMPLATE_string_parse {
		inline WORD operator() (CREF<String<B>> a) const {
			WORD ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,WORD>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<WORD> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,CHAR>>> {
	struct TEMPLATE_string_parse {
		inline CHAR operator() (CREF<String<B>> a) const {
			CHAR ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,CHAR>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<CHAR> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_parse_HELP<A ,B ,REQUIRE<IS_SAME<A ,DATA>>> {
	struct TEMPLATE_string_parse {
		inline DATA operator() (CREF<String<B>> a) const {
			DATA ret ;
			auto rax = TextReader<B> (a.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_build_HELP<A ,B ,REQUIRE<IS_SAME<B ,DATA>>> {
	struct TEMPLATE_string_build {
		inline String<A> operator() (CREF<DATA> a) const {
			String<A> ret = String<A> (64) ;
			auto rax = TextWriter<A> (ret.raw ().borrow ()) ;
			rax << a ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
using TEMPLATE_string_parse = typename TEMPLATE_string_parse_HELP<A ,B ,ALWAYS>::TEMPLATE_string_parse ;

template <class...>
trait FUNCTION_string_parse_HELP ;

template <class DEPEND>
trait FUNCTION_string_parse_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_parse {
		template <class ARG1 ,class ARG2>
		inline consteval TEMPLATE_string_parse<ARG1 ,ARG2> operator[] (CREF<TYPEID<ARG1 ,ARG2>> id) const noexcept {
			return TEMPLATE_string_parse<ARG1 ,ARG2> () ;
		}
	} ;
} ;

using FUNCTION_string_parse = typename FUNCTION_string_parse_HELP<DEPEND ,ALWAYS>::FUNCTION_string_parse ;

static constexpr auto string_parse = FUNCTION_string_parse () ;

template <class A ,class B>
using TEMPLATE_string_build = typename TEMPLATE_string_build_HELP<A ,B ,ALWAYS>::TEMPLATE_string_build ;

template <class...>
trait FUNCTION_string_build_HELP ;

template <class DEPEND>
trait FUNCTION_string_build_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_build {
		template <class ARG1 ,class ARG2>
		inline consteval TEMPLATE_string_build<ARG1 ,ARG2> operator[] (CREF<TYPEID<ARG1 ,ARG2>> id) const noexcept {
			return TEMPLATE_string_build<ARG1 ,ARG2> () ;
		}
	} ;
} ;

using FUNCTION_string_build = typename FUNCTION_string_build_HELP<DEPEND ,ALWAYS>::FUNCTION_string_build ;

static constexpr auto string_build = FUNCTION_string_build () ;

template <class...>
trait TEMPLATE_string_cvt_HELP ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRA> ,IS_SAME<B ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRA>> a) const {
			return a ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRA> ,IS_SAME<B ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> a) const {
			return StringProc::string_cvt_a_from_w (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRA> ,IS_SAME<B ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU8>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<A ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (a)) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRA> ,IS_SAME<B ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU16>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<A ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (a)) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRA> ,IS_SAME<B ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRA> operator() (CREF<String<STRU32>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<A ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (a)) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRW> ,IS_SAME<B ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRA>> a) const {
			return StringProc::string_cvt_w_from_a (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRW> ,IS_SAME<B ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRW>> a) const {
			return a ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRW> ,IS_SAME<B ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU8>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			String<STRW> ret ;
			auto &&tmp1 = unsafe_cast[TYPE<String<STRUW>>::expr] (ret) ;
			const auto r1x = R1X () ;
			tmp1 = r1x (a) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRW> ,IS_SAME<B ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU16>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			String<STRW> ret ;
			auto &&tmp1 = unsafe_cast[TYPE<String<STRUW>>::expr] (ret) ;
			const auto r1x = R1X () ;
			tmp1 = r1x (a) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRW> ,IS_SAME<B ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRW> operator() (CREF<String<STRU32>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			String<STRW> ret ;
			auto &&tmp1 = unsafe_cast[TYPE<String<STRUW>>::expr] (ret) ;
			const auto r1x = R1X () ;
			tmp1 = r1x (a) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU8> ,IS_SAME<B ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRA>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<A ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (a)) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU8> ,IS_SAME<B ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRW>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<A ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp1 = unsafe_cast[TYPE<String<STRUW>>::expr] (a) ;
			return r1x (tmp1) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU8> ,IS_SAME<B ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU8>> a) const {
			return a ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU8> ,IS_SAME<B ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU16>> a) const {
			return StringProc::string_cvt_u8_from_u16 (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU8> ,IS_SAME<B ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU8> operator() (CREF<String<STRU32>> a) const {
			return StringProc::string_cvt_u8_from_u32 (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU16> ,IS_SAME<B ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRA>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<A ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (a)) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU16> ,IS_SAME<B ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRW>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<A ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp1 = unsafe_cast[TYPE<String<STRUW>>::expr] (a) ;
			return r1x (tmp1) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU16> ,IS_SAME<B ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU8>> a) const {
			return StringProc::string_cvt_u16_from_u8 (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU16> ,IS_SAME<B ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU16>> a) const {
			return a ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU16> ,IS_SAME<B ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU16> operator() (CREF<String<STRU32>> a) const {
			return StringProc::string_cvt_u16_from_u32 (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU32> ,IS_SAME<B ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRA>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,B ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<A ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (a)) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU32> ,IS_SAME<B ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRW>> a) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<A ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp1 = unsafe_cast[TYPE<String<STRUW>>::expr] (a) ;
			return r1x (tmp1) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU32> ,IS_SAME<B ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU8>> a) const {
			return StringProc::string_cvt_u32_from_u8 (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU32> ,IS_SAME<B ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU16>> a) const {
			return StringProc::string_cvt_u32_from_u16 (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait TEMPLATE_string_cvt_HELP<A ,B ,REQUIRE<ENUM_ALL<IS_SAME<A ,STRU32> ,IS_SAME<B ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		inline String<STRU32> operator() (CREF<String<STRU32>> a) const {
			return a ;
		}
	} ;
} ;

template <class A ,class B>
using TEMPLATE_string_cvt = typename TEMPLATE_string_cvt_HELP<A ,B ,ALWAYS>::TEMPLATE_string_cvt ;

template <class...>
trait FUNCTION_string_cvt_HELP ;

template <class DEPEND>
trait FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_cvt {
		template <class ARG1 ,class ARG2>
		inline consteval TEMPLATE_string_cvt<ARG1 ,ARG2> operator[] (CREF<TYPEID<ARG1 ,ARG2>> id) const noexcept {
			return TEMPLATE_string_cvt<ARG1 ,ARG2> () ;
		}
	} ;
} ;

using FUNCTION_string_cvt = typename FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_string_cvt ;

static constexpr auto string_cvt = FUNCTION_string_cvt () ;

template <class...>
trait TEXTSTRING_HELP ;

template <class...>
trait TEXTSTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait TEXTSTRING_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<Slice<STR>> text) = 0 ;
		virtual void initialize (CREF<String<STRA>> text) = 0 ;
		virtual void initialize (CREF<String<STRW>> text) = 0 ;
		virtual void initialize (CREF<String<STRU8>> text) = 0 ;
		virtual void initialize (CREF<String<STRU16>> text) = 0 ;
		virtual void initialize (CREF<String<STRU32>> text) = 0 ;
		virtual CREF<String<STRA>> pick (TYPEID<STRA> id) const leftvalue = 0 ;
		virtual CREF<String<STRW>> pick (TYPEID<STRW> id) const leftvalue = 0 ;
		virtual CREF<String<STRU8>> pick (TYPEID<STRU8> id) const leftvalue = 0 ;
		virtual CREF<String<STRU16>> pick (TYPEID<STRU16> id) const leftvalue = 0 ;
		virtual CREF<String<STRU32>> pick (TYPEID<STRU32> id) const leftvalue = 0 ;
	} ;

	class TextString {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit TextString () = default ;

		explicit TextString (CREF<Slice<STR>> text) {
			mThis = Holder::create () ;
			mThis->initialize (text) ;
		}

		explicit TextString (CREF<String<STRA>> text) {
			mThis = Holder::create () ;
			mThis->initialize (text) ;
		}

		explicit TextString (CREF<String<STRW>> text) {
			mThis = Holder::create () ;
			mThis->initialize (text) ;
		}

		explicit TextString (CREF<String<STRU8>> text) {
			mThis = Holder::create () ;
			mThis->initialize (text) ;
		}

		explicit TextString (CREF<String<STRU16>> text) {
			mThis = Holder::create () ;
			mThis->initialize (text) ;
		}

		explicit TextString (CREF<String<STRU32>> text) {
			mThis = Holder::create () ;
			mThis->initialize (text) ;
		}

		CREF<String<STRA>> pick (TYPEID<STRA> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRW>> pick (TYPEID<STRW> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRU8>> pick (TYPEID<STRU8> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRU16>> pick (TYPEID<STRU16> id) const leftvalue {
			return mThis->pick (id) ;
		}

		CREF<String<STRU32>> pick (TYPEID<STRU32> id) const leftvalue {
			return mThis->pick (id) ;
		}
	} ;
} ;

using TextString = typename TEXTSTRING_HELP<DEPEND ,ALWAYS>::TextString ;

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
		imports VRef<Holder> create () ;

		virtual void initialize (RREF<TextString> text) = 0 ;
		virtual void write_text (VREF<TextWriter<STRA>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRW>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU8>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU16>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU32>> writer) const = 0 ;
	} ;

	class EscapeString implement Binder {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit EscapeString () = default ;

		explicit EscapeString (CREF<Slice<STR>> text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (text)) ;
		}

		explicit EscapeString (CREF<String<STRA>> text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (text)) ;
		}

		explicit EscapeString (CREF<String<STRW>> text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (text)) ;
		}

		explicit EscapeString (CREF<String<STRU8>> text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (text)) ;
		}

		explicit EscapeString (CREF<String<STRU16>> text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (text)) ;
		}

		explicit EscapeString (CREF<String<STRU32>> text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (text)) ;
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
trait COMMASTRING_HELP ;

template <class...>
trait COMMASTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait COMMASTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (RREF<TextString> gap_text ,RREF<TextString> comma_text ,RREF<TextString> first_text) = 0 ;
		virtual void write_text (VREF<TextWriter<STRA>> writer) = 0 ;
		virtual void write_text (VREF<TextWriter<STRW>> writer) = 0 ;
		virtual void write_text (VREF<TextWriter<STRU8>> writer) = 0 ;
		virtual void write_text (VREF<TextWriter<STRU16>> writer) = 0 ;
		virtual void write_text (VREF<TextWriter<STRU32>> writer) = 0 ;
		virtual void tight () = 0 ;
		virtual void enter () = 0 ;
		virtual void leave () = 0 ;
	} ;

	using COUNTER_MAX_DEPTH = ENUM<256> ;

	class CommaString implement Binder {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit CommaString () = default ;

		explicit CommaString (CREF<Slice<STR>> gap_text ,CREF<Slice<STR>> comma_text ,CREF<String<STR>> first_text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (gap_text) ,TextString (comma_text) ,TextString (first_text)) ;
		}

		explicit CommaString (CREF<String<STRA>> gap_text ,CREF<String<STRA>> comma_text ,CREF<String<STRA>> first_text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (gap_text) ,TextString (comma_text) ,TextString (first_text)) ;
		}

		explicit CommaString (CREF<String<STRW>> gap_text ,CREF<String<STRW>> comma_text ,CREF<String<STRW>> first_text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (gap_text) ,TextString (comma_text) ,TextString (first_text)) ;
		}

		explicit CommaString (CREF<String<STRU8>> gap_text ,CREF<String<STRU8>> comma_text ,CREF<String<STRU8>> first_text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (gap_text) ,TextString (comma_text) ,TextString (first_text)) ;
		}

		explicit CommaString (CREF<String<STRU16>> gap_text ,CREF<String<STRU16>> comma_text ,CREF<String<STRU16>> first_text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (gap_text) ,TextString (comma_text) ,TextString (first_text)) ;
		}

		explicit CommaString (CREF<String<STRU32>> gap_text ,CREF<String<STRU32>> comma_text ,CREF<String<STRU32>> first_text) {
			mThis = Holder::create () ;
			mThis->initialize (TextString (gap_text) ,TextString (comma_text) ,TextString (first_text)) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRA>> writer) const override {
			assert (FALSE) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRW>> writer) const override {
			assert (FALSE) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU8>> writer) const override {
			assert (FALSE) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU16>> writer) const override {
			assert (FALSE) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) override {
			return mThis->write_text (writer) ;
		}

		void friend_write (VREF<TextWriter<STRU32>> writer) const override {
			assert (FALSE) ;
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

using CommaString = typename COMMASTRING_HELP<DEPEND ,ALWAYS>::CommaString ;

template <class...>
trait VALUESTRING_HELP ;

template <class...>
trait VALUESTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait VALUESTRING_HELP<DEPEND ,ALWAYS> {
	using BinderA = typename TEXTWRITER_HELP<STRA ,ALWAYS>::Binder ;
	using BinderW = typename TEXTWRITER_HELP<STRW ,ALWAYS>::Binder ;
	using BinderU8 = typename TEXTWRITER_HELP<STRU8 ,ALWAYS>::Binder ;
	using BinderU16 = typename TEXTWRITER_HELP<STRU16 ,ALWAYS>::Binder ;
	using BinderU32 = typename TEXTWRITER_HELP<STRU32 ,ALWAYS>::Binder ;
	using Binder = Together<BinderA ,BinderW ,BinderU8 ,BinderU16 ,BinderU32> ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize (CREF<VAL64> value_ ,CREF<LENGTH> align_) = 0 ;
		virtual void write_text (VREF<TextWriter<STRA>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRW>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU8>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU16>> writer) const = 0 ;
		virtual void write_text (VREF<TextWriter<STRU32>> writer) const = 0 ;
	} ;

	class ValueString implement Binder {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit ValueString () = default ;

		explicit ValueString (CREF<VAL64> value_ ,CREF<LENGTH> align_) {
			mThis = Holder::create () ;
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

using ValueString = typename VALUESTRING_HELP<DEPEND ,ALWAYS>::ValueString ;
} ;