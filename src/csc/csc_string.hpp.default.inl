#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require 'csc_string.hpp'"
#endif

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#include "begin.h"
#include <cstdlib>
#include <clocale>
#include <string>
#include <regex>
#include "end.h"

namespace CSC {
template <class...>
trait FUNCTION_system_string_cvt_HELP ;

template <class DEPEND>
trait FUNCTION_system_string_cvt_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_MSVC<DEPEND>>> {
#ifdef __CSC_COMPILER_MSVC__
	struct FUNCTION_system_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			const auto r1x = system_page () ;
			const auto r2x = _wcstombs_s_l (NULL ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE ,r1x.self) ;
			assume (r2x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			const auto r1x = system_page () ;
			String<STRW> ret = String<STRW> (obj.length ()) ;
			const auto r2x = _mbstowcs_s_l (NULL ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE ,r1x.self) ;
			assume (r2x == 0) ;
			return move (ret) ;
		}

		CRef<UniqueRef<_locale_t>> system_page () const {
			return memorize ([&] () {
				auto rax = UniqueRef<_locale_t> ([&] (VREF<_locale_t> me) {
					me = _create_locale (LC_CTYPE ,"") ;
					assume (me != NULL) ;
				} ,[] (VREF<_locale_t> me) {
					_free_locale (me) ;
				}) ;
				return CRef<UniqueRef<_locale_t>>::make (move (rax)) ;
			}) ;
		}
	} ;
#endif
} ;

template <class DEPEND>
trait FUNCTION_system_string_cvt_HELP<DEPEND ,REQUIRE<MACRO_COMPILER_GNUC<DEPEND>>> {
#ifdef __CSC_COMPILER_GNUC__
	struct FUNCTION_system_string_cvt {
		inline String<STRA> operator() (CREF<String<STRW>> obj) const {
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			std::setlocale (LC_CTYPE ,"") ;
			const auto r1x = std::wcstombs ((&ret[0]) ,(&obj[0]) ,VAL32 (ret.size ())) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			String<STRW> ret = String<STRW> (obj.length ()) ;
			std::setlocale (LC_CTYPE ,"") ;
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
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			std::setlocale (LC_CTYPE ,"") ;
			auto rax = std::size_t (0) ;
			const auto r1x = wcstombs_s ((&rax) ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			String<STRW> ret = String<STRW> (obj.length ()) ;
			std::setlocale (LC_CTYPE ,"") ;
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
		implicit ImplHolder () = default ;

		String<STRW> ansi_string_cvt (CREF<String<STRA>> obj) const override {
			String<STRW> ret = String<STRW> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				assert (vbetween (INDEX (i) ,0 ,128)) ;
				ret[ix] = STRW (i) ;
				ix++ ;
			}
			return move (ret) ;
		}

		String<STRA> ansi_string_cvt (CREF<String<STRW>> obj) const override {
			String<STRA> ret = String<STRA> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				assert (vbetween (INDEX (i) ,0 ,128)) ;
				ret[ix] = STRA (i) ;
				ix++ ;
			}
			return move (ret) ;
		}

		String<STRA> gbks_string_cvt (CREF<String<STRW>> obj) const override {
			using R1X = typename STRINGPROC_GBKSCACHE_HELP<DEPEND ,ALWAYS>::GBKSCache ;
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				auto eax = TRUE ;
				if ifswitch (eax) {
					const auto r1x = R1X::instance ().find_utfs (STRUW (i)) ;
					if (r1x == NONE)
						discard ;
					ret[ix] = STRA (R1X::instance ()[r1x].pick (TYPEAS<RANK0>::id)) ;
					ix++ ;
				}
				if ifswitch (eax) {
					ret[ix] = STRA ('?') ;
					ix++ ;
				}
			}
			return move (ret) ;
		}

		String<STRW> gbks_string_cvt (CREF<String<STRA>> obj) const override {
			using R1X = typename STRINGPROC_GBKSCACHE_HELP<DEPEND ,ALWAYS>::GBKSCache ;
			String<STRW> ret = String<STRW> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				auto eax = TRUE ;
				if ifswitch (eax) {
					const auto r1x = R1X::instance ().find_utfs (STRUW (i)) ;
					if (r1x == NONE)
						discard ;
					ret[ix] = STRW (R1X::instance ()[r1x].pick (TYPEAS<RANK1>::id)) ;
					ix++ ;
				}
				if ifswitch (eax) {
					ret[ix] = STRW ('?') ;
					ix++ ;
				}
			}
			return move (ret) ;
		}

		String<STRA> locale_string_cvt (CREF<String<STRW>> obj) const override {
			const auto r1x = System::instance ().get_locale () ;
			if (r1x == slice ("C"))
				return ansi_string_cvt (obj) ;
			const auto r2x = r1x.length () ;
			if (r2x >= 6)
				if (r1x.segment (0 ,6) == slice ("zh_CN."))
					return gbks_string_cvt (obj) ;
			if (r2x >= 4)
				if (r1x.segment (r2x - 4 ,r2x) == slice (".936"))
					return gbks_string_cvt (obj) ;
			return system_string_cvt (obj) ;
		}

		String<STRW> locale_string_cvt (CREF<String<STRA>> obj) const override {
			const auto r1x = System::instance ().get_locale () ;
			if (r1x == slice ("C"))
				return ansi_string_cvt (obj) ;
			const auto r2x = r1x.length () ;
			if (r2x >= 6)
				if (r1x.segment (0 ,6) == slice ("zh_CN."))
					return gbks_string_cvt (obj) ;
			if (r2x >= 4)
				if (r1x.segment (r2x - 4 ,r2x) == slice (".936"))
					return gbks_string_cvt (obj) ;
			return system_string_cvt (obj) ;
		}

		String<STRA> system_string_cvt (CREF<String<STRW>> obj) const {
			using R1X = typename FUNCTION_system_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_system_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			return M_INVOKE (obj) ;
		}

		String<STRW> system_string_cvt (CREF<String<STRA>> obj) const {
			using R1X = typename FUNCTION_system_string_cvt_HELP<DEPEND ,ALWAYS>::FUNCTION_system_string_cvt ;
			static constexpr auto M_INVOKE = R1X () ;
			return M_INVOKE (obj) ;
		}
	} ;
} ;

template <>
exports auto STRINGPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename STRINGPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;