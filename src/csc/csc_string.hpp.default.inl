#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require 'csc_string.hpp'"
#endif

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#include "csc_string.hpp"
#include "csc_runtime.hpp"

#include "begin.h"
#include <cstdlib>
#include <cstring>
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
			assert (ifnot (obj.empty ())) ;
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			const auto r1x = system_page () ;
			const auto r2x = _wcstombs_s_l (NULL ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE ,r1x.self) ;
			assume (r2x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			assert (ifnot (obj.empty ())) ;
			const auto r1x = system_page () ;
			String<STRW> ret = String<STRW> (obj.length ()) ;
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
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			std::setlocale (LC_CTYPE ,"") ;
			const auto r1x = std::wcstombs ((&ret[0]) ,(&obj[0]) ,VAL32 (ret.size ())) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			assert (ifnot (obj.empty ())) ;
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
			assert (ifnot (obj.empty ())) ;
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			std::setlocale (LC_CTYPE ,"") ;
			auto rax = std::size_t (0) ;
			const auto r1x = wcstombs_s ((&rax) ,(&ret[0]) ,VAL32 (ret.size ()) ,(&obj[0]) ,_TRUNCATE) ;
			assume (r1x == 0) ;
			return move (ret) ;
		}

		inline String<STRW> operator() (CREF<String<STRA>> obj) const {
			assert (ifnot (obj.empty ())) ;
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

		void initialize () override {
			noop () ;
		}

		String<STRW> ansi_string_cvt (CREF<String<STRA>> obj) const override {
			String<STRW> ret = String<STRW> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				assert (vbetween (INDEX (i) ,0 ,128)) ;
				ret[ix] = STRW (i) ;
				ix++ ;
			}
			ret.trunc (ix) ;
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
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRA> gbks_string_cvt (CREF<String<STRW>> obj) const override {
			using R1X = typename STRINGPROC_GBKSCACHE_HELP<DEPEND ,ALWAYS>::GBKSCache ;
			String<STRA> ret = String<STRA> (obj.length () * 2) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					const auto r1x = R1X::instance ().find_utfs (STRUW (i)) ;
					if (r1x == NONE)
						discard ;
					ret[ix] = STRA (R1X::instance ()[r1x].mP1st) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					ret[ix] = STRA ('?') ;
					ix++ ;
				}
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		String<STRW> gbks_string_cvt (CREF<String<STRA>> obj) const override {
			using R1X = typename STRINGPROC_GBKSCACHE_HELP<DEPEND ,ALWAYS>::GBKSCache ;
			String<STRW> ret = String<STRW> (obj.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : obj) {
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					const auto r1x = R1X::instance ().find_utfs (STRUW (i)) ;
					if (r1x == NONE)
						discard ;
					ret[ix] = STRW (R1X::instance ()[r1x].mP2nd) ;
					ix++ ;
				}
				if ifswitch (rxx) {
					ret[ix] = STRW ('?') ;
					ix++ ;
				}
			}
			ret.trunc (ix) ;
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
	} ;
} ;

template <>
exports auto STRINGPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename STRINGPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait ANYSTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ANYSTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		Cell<CRef<String<STR>>> mText ;
		Cell<CRef<String<STRA>>> mTextA ;
		Cell<CRef<String<STRW>>> mTextW ;
		Cell<CRef<String<STRU8>>> mTextU8 ;
		Cell<CRef<String<STRU16>>> mTextU16 ;
		Cell<CRef<String<STRU32>>> mTextU32 ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<Slice<STR>> text) override {
			mText = Cell<CRef<String<STR>>>::make () ;
			mText.store (CRef<String<STR>>::make (text)) ;
		}

		void initialize (CREF<String<STRA>> text) override {
			mText = Cell<CRef<String<STR>>>::make () ;
			auto rax = string_cvt[TYPEAS<TYPEAS<STR ,STRA>>::expr] (text) ;
			mText.store (CRef<String<STR>>::make (move (rax))) ;
			mTextA = Cell<CRef<String<STRA>>>::make () ;
			mTextA.store (CRef<String<STRA>>::make (text)) ;
			mTextW = Cell<CRef<String<STRW>>>::make () ;
			mTextU8 = Cell<CRef<String<STRU8>>>::make () ;
			mTextU16 = Cell<CRef<String<STRU16>>>::make () ;
			mTextU32 = Cell<CRef<String<STRU32>>>::make () ;
		}

		void initialize (CREF<String<STRW>> text) override {
			mText = Cell<CRef<String<STR>>>::make () ;
			auto rax = string_cvt[TYPEAS<TYPEAS<STR ,STRW>>::expr] (text) ;
			mText.store (CRef<String<STR>>::make (move (rax))) ;
			mTextA = Cell<CRef<String<STRA>>>::make () ;
			mTextW = Cell<CRef<String<STRW>>>::make () ;
			mTextW.store (CRef<String<STRW>>::make (text)) ;
			mTextU8 = Cell<CRef<String<STRU8>>>::make () ;
			mTextU16 = Cell<CRef<String<STRU16>>>::make () ;
			mTextU32 = Cell<CRef<String<STRU32>>>::make () ;
		}

		void initialize (CREF<String<STRU8>> text) override {
			mText = Cell<CRef<String<STR>>>::make () ;
			auto rax = string_cvt[TYPEAS<TYPEAS<STR ,STRU8>>::expr] (text) ;
			mText.store (CRef<String<STR>>::make (move (rax))) ;
			mTextA = Cell<CRef<String<STRA>>>::make () ;
			mTextW = Cell<CRef<String<STRW>>>::make () ;
			mTextU8 = Cell<CRef<String<STRU8>>>::make () ;
			mTextU8.store (CRef<String<STRU8>>::make (text)) ;
			mTextU16 = Cell<CRef<String<STRU16>>>::make () ;
			mTextU32 = Cell<CRef<String<STRU32>>>::make () ;
		}

		void initialize (CREF<String<STRU16>> text) override {
			mText = Cell<CRef<String<STR>>>::make () ;
			auto rax = string_cvt[TYPEAS<TYPEAS<STR ,STRU16>>::expr] (text) ;
			mText.store (CRef<String<STR>>::make (move (rax))) ;
			mTextA = Cell<CRef<String<STRA>>>::make () ;
			mTextW = Cell<CRef<String<STRW>>>::make () ;
			mTextU8 = Cell<CRef<String<STRU8>>>::make () ;
			mTextU16 = Cell<CRef<String<STRU16>>>::make () ;
			mTextU16.store (CRef<String<STRU16>>::make (text)) ;
			mTextU32 = Cell<CRef<String<STRU32>>>::make () ;
		}

		void initialize (CREF<String<STRU32>> text) override {
			mText = Cell<CRef<String<STR>>>::make () ;
			auto rax = string_cvt[TYPEAS<TYPEAS<STR ,STRU32>>::expr] (text) ;
			mText.store (CRef<String<STR>>::make (move (rax))) ;
			mTextA = Cell<CRef<String<STRA>>>::make () ;
			mTextW = Cell<CRef<String<STRW>>>::make () ;
			mTextU8 = Cell<CRef<String<STRU8>>>::make () ;
			mTextU16 = Cell<CRef<String<STRU16>>>::make () ;
			mTextU32 = Cell<CRef<String<STRU32>>>::make () ;
			mTextU32.store (CRef<String<STRU32>>::make (text)) ;
		}

		CREF<String<STRA>> pick (CREF<TYPEID<STRA>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mTextA.fetch () == NULL)
					discard ;
				const auto r1x = mText.fetch () ;
				auto rax = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (r1x.self) ;
				mTextA.store (CRef<String<STRA>>::make (move (rax))) ;
			}
			const auto r2x = mTextA.fetch () ;
			return r2x.self ;
		}

		CREF<String<STRW>> pick (CREF<TYPEID<STRW>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mTextW.fetch () == NULL)
					discard ;
				const auto r1x = mText.fetch () ;
				auto rax = string_cvt[TYPEAS<TYPEAS<STRW ,STR>>::expr] (r1x.self) ;
				mTextW.store (CRef<String<STRW>>::make (move (rax))) ;
			}
			const auto r2x = mTextW.fetch () ;
			return r2x.self ;
		}

		CREF<String<STRU8>> pick (CREF<TYPEID<STRU8>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mTextU8.fetch () == NULL)
					discard ;
				const auto r1x = mText.fetch () ;
				auto rax = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::expr] (r1x.self) ;
				mTextU8.store (CRef<String<STRU8>>::make (move (rax))) ;
			}
			const auto r2x = mTextU8.fetch () ;
			return r2x.self ;
		}

		CREF<String<STRU16>> pick (CREF<TYPEID<STRU16>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mTextU16.fetch () == NULL)
					discard ;
				const auto r1x = mText.fetch () ;
				auto rax = string_cvt[TYPEAS<TYPEAS<STRU16 ,STR>>::expr] (r1x.self) ;
				mTextU16.store (CRef<String<STRU16>>::make (move (rax))) ;
			}
			const auto r2x = mTextU16.fetch () ;
			return r2x.self ;
		}

		CREF<String<STRU32>> pick (CREF<TYPEID<STRU32>> id) const leftvalue override {
			if ifswitch (TRUE) {
				if (mTextU32.fetch () == NULL)
					discard ;
				const auto r1x = mText.fetch () ;
				auto rax = string_cvt[TYPEAS<TYPEAS<STRU32 ,STR>>::expr] (r1x.self) ;
				mTextU32.store (CRef<String<STRU32>>::make (move (rax))) ;
			}
			const auto r2x = mTextU32.fetch () ;
			return r2x.self ;
		}
	} ;
} ;

template <>
exports auto ANYSTRING_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename ANYSTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait ESCAPESTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ESCAPESTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ESCAPESTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		AnyString mText ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<AnyString> text) override {
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
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					const auto r2x = r1x.escape_cast (i) ;
					if ifnot (r2x.exist ())
						discard ;
					writer << slice ("\\") ;
					writer << r2x.fetch () ;
				}
				if ifswitch (rxx) {
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

template <class...>
trait COMMASTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait COMMASTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename COMMASTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class ImplHolder implement Holder {
	protected:
		AnyString mGapText ;
		AnyString mCommaText ;
		LENGTH mCounter ;
		Cell<LENGTH> mTightCounter ;
		Deque<Cell<BOOL>> mFirst ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<AnyString> gap_text ,RREF<AnyString> comma_text) override {
			mGapText = move (gap_text) ;
			mCommaText = move (comma_text) ;
			mCounter = 0 ;
			mTightCounter = Cell<LENGTH>::make (COUNTER_MAX_DEPTH::expr) ;
			mFirst = Deque<Cell<BOOL>> (COUNTER_MAX_DEPTH::expr * 2) ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		void write_text (VREF<TextWriter<STRA>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRA>::expr) ,mCommaText.pick (TYPEAS<STRA>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRW>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRW>::expr) ,mCommaText.pick (TYPEAS<STRW>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU8>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRU8>::expr) ,mCommaText.pick (TYPEAS<STRU8>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU16>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRU16>::expr) ,mCommaText.pick (TYPEAS<STRU16>::expr)) ;
		}

		void write_text (VREF<TextWriter<STRU32>> writer) const override {
			write_text_impl (writer ,mGapText.pick (TYPEAS<STRU32>::expr) ,mCommaText.pick (TYPEAS<STRU32>::expr)) ;
		}

		template <class ARG1 ,class ARG2 ,class ARG3>
		void write_text_impl (VREF<ARG1> writer ,CREF<ARG2> gap_text ,CREF<ARG3> comma_text) const {
			INDEX ix = mFirst.tail () ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mCounter > mTightCounter.fetch ())
					discard ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					for (auto &&j : gap_text)
						writer << j ;
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
				writer << ARG1::GAP ;
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
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
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

template <class...>
trait ALIGNEDSTRING_IMPLHOLDER_HELP ;

template <class DEPEND>
trait ALIGNEDSTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename ALIGNEDSTRING_HELP<DEPEND ,ALWAYS>::Holder ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class ImplHolder implement Holder {
	protected:
		VAL64 mValue ;
		LENGTH mAlign ;
		LENGTH mSpace ;

	public:
		implicit ImplHolder () = default ;

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
exports auto ALIGNEDSTRING_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename ALIGNEDSTRING_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;