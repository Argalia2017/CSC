#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
		virtual String<STRA> string_cvt_ansi_from_w (CREF<String<STRW>> obj) const = 0 ;
		virtual String<STRW> string_cvt_w_from_ansi (CREF<String<STRA>> obj) const = 0 ;
		virtual String<STRA> string_cvt_gbks_from_w (CREF<String<STRW>> obj) const = 0 ;
		virtual String<STRW> string_cvt_w_from_gbks (CREF<String<STRA>> obj) const = 0 ;
		virtual String<STRA> string_cvt_a_from_w (CREF<String<STRW>> obj) const = 0 ;
		virtual String<STRW> string_cvt_w_from_a (CREF<String<STRA>> obj) const = 0 ;
		virtual String<STRU8> string_cvt_u8_from_u16 (CREF<String<STRU16>> obj) const = 0 ;
		virtual String<STRU8> string_cvt_u8_from_u32 (CREF<String<STRU32>> obj) const = 0 ;
		virtual String<STRU16> string_cvt_u16_from_u8 (CREF<String<STRU8>> obj) const = 0 ;
		virtual String<STRU16> string_cvt_u16_from_u32 (CREF<String<STRU32>> obj) const = 0 ;
		virtual String<STRU32> string_cvt_u32_from_u8 (CREF<String<STRU8>> obj) const = 0 ;
		virtual String<STRU32> string_cvt_u32_from_u16 (CREF<String<STRU16>> obj) const = 0 ;
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

		imports String<STRA> string_cvt_ansi_from_w (CREF<String<STRW>> obj) {
			return instance ().mThis->string_cvt_ansi_from_w (obj) ;
		}

		imports String<STRW> string_cvt_w_from_ansi (CREF<String<STRA>> obj) {
			return instance ().mThis->string_cvt_w_from_ansi (obj) ;
		}

		imports String<STRA> string_cvt_gbks_from_w (CREF<String<STRW>> obj) {
			return instance ().mThis->string_cvt_gbks_from_w (obj) ;
		}

		imports String<STRW> string_cvt_w_from_gbks (CREF<String<STRA>> obj) {
			return instance ().mThis->string_cvt_w_from_gbks (obj) ;
		}

		imports String<STRA> string_cvt_a_from_w (CREF<String<STRW>> obj) {
			return instance ().mThis->string_cvt_a_from_w (obj) ;
		}

		imports String<STRW> string_cvt_w_from_a (CREF<String<STRA>> obj) {
			return instance ().mThis->string_cvt_w_from_a (obj) ;
		}

		imports String<STRU8> string_cvt_u8_from_u16 (CREF<String<STRU16>> obj) {
			return instance ().mThis->string_cvt_u8_from_u16 (obj) ;
		}

		imports String<STRU8> string_cvt_u8_from_u32 (CREF<String<STRU32>> obj) {
			return instance ().mThis->string_cvt_u8_from_u32 (obj) ;
		}

		imports String<STRU16> string_cvt_u16_from_u8 (CREF<String<STRU8>> obj) {
			return instance ().mThis->string_cvt_u16_from_u8 (obj) ;
		}

		imports String<STRU16> string_cvt_u16_from_u32 (CREF<String<STRU32>> obj) {
			return instance ().mThis->string_cvt_u16_from_u32 (obj) ;
		}

		imports String<STRU32> string_cvt_u32_from_u8 (CREF<String<STRU8>> obj) {
			return instance ().mThis->string_cvt_u32_from_u8 (obj) ;
		}

		imports String<STRU32> string_cvt_u32_from_u16 (CREF<String<STRU16>> obj) {
			return instance ().mThis->string_cvt_u32_from_u16 (obj) ;
		}
	} ;
} ;

using StringProc = typename STRINGPROC_HELP<DEPEND ,ALWAYS>::StringProc ;

template <class...>
trait TEMPLATE_string_parse_HELP ;

template <class...>
trait TEMPLATE_string_build_HELP ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,BOOL>>> {
	struct TEMPLATE_string_parse {
		forceinline BOOL operator() (CREF<String<SIDE>> obj) const {
			BOOL ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,BOOL>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<BOOL> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,VAL32>>> {
	struct TEMPLATE_string_parse {
		forceinline VAL32 operator() (CREF<String<SIDE>> obj) const {
			VAL32 ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,VAL32>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<VAL32> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,VAL64>>> {
	struct TEMPLATE_string_parse {
		forceinline VAL64 operator() (CREF<String<SIDE>> obj) const {
			VAL64 ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,VAL64>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<VAL64> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,SINGLE>>> {
	struct TEMPLATE_string_parse {
		forceinline SINGLE operator() (CREF<String<SIDE>> obj) const {
			SINGLE ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,SINGLE>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<SINGLE> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,DOUBLE>>> {
	struct TEMPLATE_string_parse {
		forceinline DOUBLE operator() (CREF<String<SIDE>> obj) const {
			DOUBLE ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,DOUBLE>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<DOUBLE> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,BYTE>>> {
	struct TEMPLATE_string_parse {
		forceinline BYTE operator() (CREF<String<SIDE>> obj) const {
			BYTE ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,BYTE>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<BYTE> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,WORD>>> {
	struct TEMPLATE_string_parse {
		forceinline WORD operator() (CREF<String<SIDE>> obj) const {
			WORD ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,WORD>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<WORD> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,CHAR>>> {
	struct TEMPLATE_string_parse {
		forceinline CHAR operator() (CREF<String<SIDE>> obj) const {
			CHAR ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,CHAR>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<CHAR> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_parse_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<UNIT ,DATA>>> {
	struct TEMPLATE_string_parse {
		forceinline DATA operator() (CREF<String<SIDE>> obj) const {
			DATA ret ;
			auto rax = TextReader<SIDE> (obj.raw ().borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_build_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,DATA>>> {
	struct TEMPLATE_string_build {
		forceinline String<UNIT> operator() (CREF<DATA> obj) const {
			String<UNIT> ret = String<UNIT> (64) ;
			auto rax = TextWriter<UNIT> (ret.raw ().borrow ()) ;
			rax << obj ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
using TEMPLATE_string_parse = typename TEMPLATE_string_parse_HELP<UNIT ,SIDE ,ALWAYS>::TEMPLATE_string_parse ;

template <class...>
trait FUNCTION_string_parse_HELP ;

template <class DEPEND>
trait FUNCTION_string_parse_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_parse {
		template <class ARG1 ,class ARG2>
		forceinline consteval TEMPLATE_string_parse<ARG1 ,ARG2> operator[] (CREF<TYPEID<ARG1 ,ARG2>> id) const noexcept {
			return TEMPLATE_string_parse<ARG1 ,ARG2> () ;
		}
	} ;
} ;

using FUNCTION_string_parse = typename FUNCTION_string_parse_HELP<DEPEND ,ALWAYS>::FUNCTION_string_parse ;

static constexpr auto string_parse = FUNCTION_string_parse () ;

template <class UNIT ,class SIDE>
using TEMPLATE_string_build = typename TEMPLATE_string_build_HELP<UNIT ,SIDE ,ALWAYS>::TEMPLATE_string_build ;

template <class...>
trait FUNCTION_string_build_HELP ;

template <class DEPEND>
trait FUNCTION_string_build_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_build {
		template <class ARG1 ,class ARG2>
		forceinline consteval TEMPLATE_string_build<ARG1 ,ARG2> operator[] (CREF<TYPEID<ARG1 ,ARG2>> id) const noexcept {
			return TEMPLATE_string_build<ARG1 ,ARG2> () ;
		}
	} ;
} ;

using FUNCTION_string_build = typename FUNCTION_string_build_HELP<DEPEND ,ALWAYS>::FUNCTION_string_build ;

static constexpr auto string_build = FUNCTION_string_build () ;

template <class...>
trait TEMPLATE_string_cvt_HELP ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRA> ,IS_SAME<SIDE ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRA> operator() (CREF<String<STRA>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRA> ,IS_SAME<SIDE ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRA> operator() (CREF<String<STRW>> obj) const {
			return StringProc::string_cvt_a_from_w (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRA> ,IS_SAME<SIDE ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRA> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRA> ,IS_SAME<SIDE ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRA> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRA> ,IS_SAME<SIDE ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRA> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRW> ,IS_SAME<SIDE ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRW> operator() (CREF<String<STRA>> obj) const {
			return StringProc::string_cvt_w_from_a (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRW> ,IS_SAME<SIDE ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRW> operator() (CREF<String<STRW>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRW> ,IS_SAME<SIDE ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRW> operator() (CREF<String<STRU8>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			String<STRW> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<String<STRUW>>::expr] (ret) ;
			const auto r1x = R1X () ;
			tmp = r1x (obj) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRW> ,IS_SAME<SIDE ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRW> operator() (CREF<String<STRU16>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			String<STRW> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<String<STRUW>>::expr] (ret) ;
			const auto r1x = R1X () ;
			tmp = r1x (obj) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRW> ,IS_SAME<SIDE ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRW> operator() (CREF<String<STRU32>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRUW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			String<STRW> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<String<STRUW>>::expr] (ret) ;
			const auto r1x = R1X () ;
			tmp = r1x (obj) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU8> ,IS_SAME<SIDE ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU8> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU8> ,IS_SAME<SIDE ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU8> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_cast[TYPEAS<String<STRUW>>::expr] (obj) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU8> ,IS_SAME<SIDE ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU8> operator() (CREF<String<STRU8>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU8> ,IS_SAME<SIDE ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU8> operator() (CREF<String<STRU16>> obj) const {
			return StringProc::string_cvt_u8_from_u16 (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU8> ,IS_SAME<SIDE ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU8> operator() (CREF<String<STRU32>> obj) const {
			return StringProc::string_cvt_u8_from_u32 (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU16> ,IS_SAME<SIDE ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU16> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU16> ,IS_SAME<SIDE ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU16> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_cast[TYPEAS<String<STRUW>>::expr] (obj) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU16> ,IS_SAME<SIDE ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU16> operator() (CREF<String<STRU8>> obj) const {
			return StringProc::string_cvt_u16_from_u8 (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU16> ,IS_SAME<SIDE ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU16> operator() (CREF<String<STRU16>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU16> ,IS_SAME<SIDE ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU16> operator() (CREF<String<STRU32>> obj) const {
			return StringProc::string_cvt_u16_from_u32 (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU32> ,IS_SAME<SIDE ,STRA>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU32> operator() (CREF<String<STRA>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<STRW ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;
			using R2X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			const auto r2x = R2X () ;
			return r2x (r1x (obj)) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU32> ,IS_SAME<SIDE ,STRW>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU32> operator() (CREF<String<STRW>> obj) const {
			using R1X = typename TEMPLATE_string_cvt_HELP<UNIT ,STRUW ,ALWAYS>::TEMPLATE_string_cvt ;
			const auto r1x = R1X () ;
			auto &&tmp = unsafe_cast[TYPEAS<String<STRUW>>::expr] (obj) ;
			return r1x (tmp) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU32> ,IS_SAME<SIDE ,STRU8>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU32> operator() (CREF<String<STRU8>> obj) const {
			return StringProc::string_cvt_u32_from_u8 (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU32> ,IS_SAME<SIDE ,STRU16>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU32> operator() (CREF<String<STRU16>> obj) const {
			return StringProc::string_cvt_u32_from_u16 (obj) ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,REQUIRE<ENUM_ALL<IS_SAME<UNIT ,STRU32> ,IS_SAME<SIDE ,STRU32>>>> {
	struct TEMPLATE_string_cvt {
		forceinline String<STRU32> operator() (CREF<String<STRU32>> obj) const {
			return obj ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
using TEMPLATE_string_cvt = typename TEMPLATE_string_cvt_HELP<UNIT ,SIDE ,ALWAYS>::TEMPLATE_string_cvt ;

template <class...>
trait FUNCTION_string_cvt_HELP ;

template <class DEPEND>
trait FUNCTION_string_cvt_HELP<DEPEND ,ALWAYS> {
	struct FUNCTION_string_cvt {
		template <class ARG1 ,class ARG2>
		forceinline consteval TEMPLATE_string_cvt<ARG1 ,ARG2> operator[] (CREF<TYPEID<ARG1 ,ARG2>> id) const noexcept {
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
		virtual CREF<String<STRA>> pick (CREF<TYPEID<STRA>> id) const leftvalue = 0 ;
		virtual CREF<String<STRW>> pick (CREF<TYPEID<STRW>> id) const leftvalue = 0 ;
		virtual CREF<String<STRU8>> pick (CREF<TYPEID<STRU8>> id) const leftvalue = 0 ;
		virtual CREF<String<STRU16>> pick (CREF<TYPEID<STRU16>> id) const leftvalue = 0 ;
		virtual CREF<String<STRU32>> pick (CREF<TYPEID<STRU32>> id) const leftvalue = 0 ;
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

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,256> ;

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