#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require 'csc_string.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_once")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_once
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#include <cstdlib>
#include <clocale>
#include <string>
#include <regex>
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_once")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using std::smatch ;
using std::regex ;
using std::string ;

using std::regex_match ;
using std::regex_search ;
using std::regex_replace ;
using std::localtime ;
using std::setlocale ;
using std::mbstowcs ;
using std::wcstombs ;

using ::tm ;
using ::time_t ;

using ::mktime ;

#ifdef __CSC_COMPILER_MSVC__
using ::localtime_s ;
using ::_create_locale ;
using ::_free_locale ;
using ::_mbstowcs_s_l ;
using ::_wcstombs_s_l ;
#endif
} ;

class LocaleStaticProc
	:private Wrapped<> {
public:
#ifdef __CSC_COMPILER_MSVC__
	imports const UniqueRef<_locale_t> &static_locale_page () ;
#endif

	imports String<STRW> static_locale_cvt_lastows (const String<STRA> &val) ;

	imports String<STRA> static_locale_cvt_wstolas (const String<STRW> &val) ;
} ;

#ifdef __CSC_COMPILER_MSVC__
inline exports const UniqueRef<_locale_t> &LocaleStaticProc::static_locale_page () {
	return _CACHE_ ([&] () {
		return UniqueRef<_locale_t> ([&] (_locale_t &me) {
			me = api::_create_locale (LC_CTYPE ,_PCSTRA_ ("")) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (_locale_t &me) {
			api::_free_locale (me) ;
		}) ;
	}) ;
}
#endif

#ifdef __CSC_COMPILER_MSVC__
inline exports String<STRW> LocaleStaticProc::static_locale_cvt_lastows (const String<STRA> &val) {
	auto &r1x = LocaleStaticProc::static_locale_page () ;
	String<STRW> ret = String<STRW> (val.length () + 1) ;
	_DEBUG_ASSERT_ (ret.size () < VAR32_MAX) ;
	if switch_once (TRUE) {
		const auto r2x = api::_mbstowcs_s_l (NULL ,ret.raw ().self ,VAR32 (ret.size ()) ,val.raw ().self ,_TRUNCATE ,r1x) ;
		if (r2x == 0)
			discard ;
		ret = String<STRW> () ;
	}
	return _MOVE_ (ret) ;
}
#endif

#ifndef __CSC_COMPILER_MSVC__
inline exports String<STRW> LocaleStaticProc::static_locale_cvt_lastows (const String<STRA> &val) {
	String<STRW> ret = String<STRW> (val.length () + 1) ;
	_DEBUG_ASSERT_ (ret.size () < VAR32_MAX) ;
	if switch_once (TRUE) {
		const auto r1x = api::mbstowcs (ret.raw ().self ,val.raw ().self ,VAR32 (ret.size ())) ;
		if (r1x == 0)
			discard ;
		ret = String<STRW> () ;
	}
	return _MOVE_ (ret) ;
}
#endif

#ifdef __CSC_COMPILER_MSVC__
inline exports String<STRA> LocaleStaticProc::static_locale_cvt_wstolas (const String<STRW> &val) {
	auto &r1x = LocaleStaticProc::static_locale_page () ;
	String<STRA> ret = String<STRA> ((val.length () + 1) * _SIZEOF_ (STRW)) ;
	_DEBUG_ASSERT_ (ret.size () < VAR32_MAX) ;
	if switch_once (TRUE) {
		const auto r2x = api::_wcstombs_s_l (NULL ,ret.raw ().self ,VAR32 (ret.size ()) ,val.raw ().self ,_TRUNCATE ,r1x) ;
		if (r2x == 0)
			discard ;
		ret = String<STRA> () ;
	}
	return _MOVE_ (ret) ;
}
#endif

#ifndef __CSC_COMPILER_MSVC__
inline exports String<STRA> LocaleStaticProc::static_locale_cvt_wstolas (const String<STRW> &val) {
	String<STRA> ret = String<STRA> ((val.length () + 1) * _SIZEOF_ (STRW)) ;
	_DEBUG_ASSERT_ (ret.size () < VAR32_MAX) ;
	if switch_once (TRUE) {
		const auto r1x = api::wcstombs (ret.raw ().self ,val.raw ().self ,VAR32 (ret.size ())) ;
		if (r1x == 0)
			discard ;
		ret = String<STRA> () ;
	}
	return _MOVE_ (ret) ;
}
#endif

inline exports String<STRW> StringProc::cvt_as_ws (const String<STRA> &val) {
	//@warn: not thread-safe due to internel storage
	const auto r1x = api::setlocale (LC_CTYPE ,NULL) ;
	_DEBUG_ASSERT_ (r1x != NULL) ;
	const auto r2x = BasicProc::mem_chr (PTRTOARR[r1x] ,VAR32_MAX ,STRA (0)) ;
	if (r2x == 1)
		if (BasicProc::mem_equal (PTRTOARR[r1x] ,_PCSTRA_ ("C").self ,1))
			return StringProc::cvt_u8s_ws (StringProc::cvt_uas_u8s (val)) ;
	if (r2x >= 4)
		if (BasicProc::mem_equal (PTRTOARR[DEPTR[r1x[r2x - 4]]] ,_PCSTRA_ (".936").self ,4))
			return StringProc::cvt_gbks_ws (val) ;
	if (r2x >= 5)
		if (BasicProc::mem_equal (PTRTOARR[r1x] ,_PCSTRA_ ("zh_CN").self ,5))
			return StringProc::cvt_gbks_ws (val) ;
	return LocaleStaticProc::static_locale_cvt_lastows (val) ;
}

inline exports String<STRA> StringProc::cvt_ws_as (const String<STRW> &val) {
	//@warn: not thread-safe due to internel storage
	const auto r1x = api::setlocale (LC_CTYPE ,NULL) ;
	_DEBUG_ASSERT_ (r1x != NULL) ;
	const auto r2x = BasicProc::mem_chr (PTRTOARR[r1x] ,VAR32_MAX ,STRA (0)) ;
	if (r2x == 1)
		if (BasicProc::mem_equal (PTRTOARR[r1x] ,_PCSTRA_ ("C").self ,1))
			return StringProc::cvt_u8s_uas (StringProc::cvt_ws_u8s (val)) ;
	if (r2x >= 4)
		if (BasicProc::mem_equal (PTRTOARR[DEPTR[r1x[r2x - 4]]] ,_PCSTRA_ (".936").self ,4))
			return StringProc::cvt_ws_gbks (val) ;
	if (r2x >= 5)
		if (BasicProc::mem_equal (PTRTOARR[r1x] ,_PCSTRA_ ("zh_CN").self ,5))
			return StringProc::cvt_ws_gbks (val) ;
	return LocaleStaticProc::static_locale_cvt_wstolas (val) ;
}

#ifdef __CSC_EXTEND__
class RegexMatcher::Private::Implement
	:public Abstract {
private:
	AutoRef<api::regex> mRegex ;

public:
	implicit Implement () = delete ;

	explicit Implement (const String<STRU8> &reg) {
		const auto r1x = StringProc::cvt_u8s_uas (reg) ;
		mRegex = AutoRef<api::regex>::make (r1x.raw ().self) ;
	}

	BOOL match (const String<STRU8> &expr) const override {
		if (expr.empty ())
			return FALSE ;
		const auto r1x = StringProc::cvt_u8s_uas (expr) ;
		if (!api::regex_match (r1x.raw ().self ,mRegex.self))
			return FALSE ;
		return TRUE ;
	}

	Deque<ARRAY2<INDEX>> search (const String<STRU8> &expr) const override {
		Deque<ARRAY2<INDEX>> ret = Deque<ARRAY2<INDEX>> (expr.length ()) ;
		if switch_once (TRUE) {
			if (expr.empty ())
				discard ;
			auto rax = AutoRef<api::smatch>::make () ;
			const auto r1x = StringProc::cvt_u8s_uas (expr) ;
			const auto r2x = api::string (r1x.raw ().self) ;
			auto rbx = r2x.begin () ;
			const auto r3x = r2x.end () ;
			while (TRUE) {
				const auto r4x = api::regex_search (rbx ,r3x ,rax.self ,mRegex.self) ;
				if (!r4x)
					break ;
				INDEX ix = ret.insert () ;
				auto &r5x = (*rax.self[0].first) ;
				auto &r6x = (*rax.self[0].second) ;
				ret[ix][0] = INDEX (DEPTR[r5x] - DEPTR[r2x[0]]) ;
				ret[ix][1] = INDEX (DEPTR[r6x] - DEPTR[r2x[0]]) ;
				rbx = rax.self[0].second ;
			}
		}
		return _MOVE_ (ret) ;
	}

	String<STRU8> replace (const String<STRU8> &expr ,const String<STRU8> &rep) const override {
		if (expr.empty ())
			return String<STRU8> () ;
		const auto r1x = StringProc::cvt_u8s_uas (expr) ;
		const auto r2x = StringProc::cvt_u8s_uas (rep) ;
		const auto r3x = api::string (r1x.raw ().self) ;
		const auto r4x = api::string (r2x.raw ().self) ;
		const auto r5x = api::regex_replace (r3x ,mRegex.self ,r4x) ;
		if (r5x.empty ())
			return String<STRU8> () ;
		return StringProc::cvt_uas_u8s (PTRTOARR[r5x.c_str ()]) ;
	}
} ;

inline exports RegexMatcher::RegexMatcher (const String<STRU8> &reg) {
	using R1X = typename Private::Implement ;
	mThis = StrongRef<R1X>::make (reg) ;
}
#endif
} ;