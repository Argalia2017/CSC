#pragma once

#ifndef __CSC_SERIALIZATION__
#define __CSC_SERIALIZATION__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
template <class...>
trait XMLPARSER_HELP ;

template <class...>
trait XMLPARSER_IMPLHOLDER_HELP ;

template <class...>
trait XMLPARSER_SERIALIZATION_HELP ;

template <class...>
trait XMLPARSER_COMBINATION_HELP ;

template <class DEPEND>
trait XMLPARSER_HELP<DEPEND ,ALWAYS> {
	template <class ARG1>
	using CRTP_XmlParser = typename DEPENDENT<XMLPARSER_HELP<DEPEND ,ALWAYS> ,ARG1>::XmlParser ;

	struct Holder implement Interface {
		virtual void initialize (CREF<TEMP<void>> heap ,CREF<INDEX> index) = 0 ;
		virtual BOOL exist () const = 0 ;
		virtual CRTP_XmlParser<DEPEND> clone () const = 0 ;
		virtual CRTP_XmlParser<DEPEND> root () const = 0 ;
		virtual CRTP_XmlParser<DEPEND> parent () const = 0 ;
		virtual CRTP_XmlParser<DEPEND> brother () const = 0 ;
		virtual CRTP_XmlParser<DEPEND> child () const = 0 ;
		virtual CRTP_XmlParser<DEPEND> child (CREF<String<STRU8>> name) const = 0 ;
		virtual Array<CRTP_XmlParser<DEPEND>> child_array () const = 0 ;
		virtual Array<CRTP_XmlParser<DEPEND>> child_array (CREF<LENGTH> size_) const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual CREF<String<STRU8>> name () const leftvalue = 0 ;
		virtual CREF<String<STRU8>> attribute (CREF<String<STRU8>> tag) const leftvalue = 0 ;
		virtual CREF<String<STRU8>> fetch () const leftvalue = 0 ;
		virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
		virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
		virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
		virtual SINGLE fetch (CREF<SINGLE> def) const = 0 ;
		virtual DOUBLE fetch (CREF<DOUBLE> def) const = 0 ;
		virtual BYTE fetch (CREF<BYTE> def) const = 0 ;
		virtual WORD fetch (CREF<WORD> def) const = 0 ;
		virtual CHAR fetch (CREF<CHAR> def) const = 0 ;
		virtual DATA fetch (CREF<DATA> def) const = 0 ;
		virtual String<STRA> fetch (CREF<String<STRA>> def) const = 0 ;
		virtual String<STRW> fetch (CREF<String<STRW>> def) const = 0 ;
		virtual String<STRU8> fetch (CREF<String<STRU8>> def) const = 0 ;
		virtual String<STRU16> fetch (CREF<String<STRU16>> def) const = 0 ;
		virtual String<STRU32> fetch (CREF<String<STRU32>> def) const = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		CRef<TEMP<void>> mHeap ;
		INDEX mIndex ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class XmlParser {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit XmlParser () {
			mThis = FUNCTION_extern::invoke () ;
		}

		explicit XmlParser (CREF<TEMP<void>> heap ,CREF<INDEX> index) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (heap ,index) ;
		}

		BOOL exist () const {
			return mThis->exist () ;
		}

		XmlParser clone () const {
			return mThis->clone () ;
		}

		XmlParser root () const {
			return mThis->root () ;
		}

		XmlParser parent () const {
			return mThis->parent () ;
		}

		XmlParser brother () const {
			return mThis->brother () ;
		}

		XmlParser child () const {
			return mThis->child () ;
		}

		XmlParser child (CREF<Slice<STR>> name) const {
			return child (String<STRU8>::make (name)) ;
		}

		XmlParser child (CREF<String<STRU8>> name) const {
			return mThis->child (name) ;
		}

		Array<XmlParser> child_array () const {
			return mThis->child_array () ;
		}

		Array<XmlParser> child_array (CREF<LENGTH> size_) const {
			return mThis->child_array () ;
		}

		BOOL equal (CREF<XmlParser> that) const {
			return mThis->equal (that.mThis) ;
		}

		inline BOOL operator== (CREF<XmlParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<XmlParser> that) const {
			return ifnot (equal (that)) ;
		}

		CREF<String<STRU8>> name () const leftvalue {
			return mThis->name () ;
		}

		CREF<String<STRU8>> attribute (CREF<Slice<STR>> tag) const leftvalue {
			return attribute (String<STRU8>::make (tag)) ;
		}

		CREF<String<STRU8>> attribute (CREF<String<STRU8>> tag) const leftvalue {
			return mThis->attribute (tag) ;
		}

		CREF<String<STRU8>> fetch () const leftvalue {
			return mThis->fetch () ;
		}

		BOOL fetch (CREF<BOOL> def) const {
			return mThis->fetch (def) ;
		}

		VAL32 fetch (CREF<VAL32> def) const {
			return mThis->fetch (def) ;
		}

		VAL64 fetch (CREF<VAL64> def) const {
			return mThis->fetch (def) ;
		}

		SINGLE fetch (CREF<SINGLE> def) const {
			return mThis->fetch (def) ;
		}

		DOUBLE fetch (CREF<DOUBLE> def) const {
			return mThis->fetch (def) ;
		}

		BYTE fetch (CREF<BYTE> def) const {
			return mThis->fetch (def) ;
		}

		WORD fetch (CREF<WORD> def) const {
			return mThis->fetch (def) ;
		}

		CHAR fetch (CREF<CHAR> def) const {
			return mThis->fetch (def) ;
		}

		DATA fetch (CREF<DATA> def) const {
			return mThis->fetch (def) ;
		}

		String<STRA> fetch (CREF<String<STRA>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRW> fetch (CREF<String<STRW>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRU8> fetch (CREF<String<STRU8>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRU16> fetch (CREF<String<STRU16>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRU32> fetch (CREF<String<STRU32>> def) const {
			return mThis->fetch (def) ;
		}
	} ;
} ;

using XmlParser = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::XmlParser ;

template <class...>
trait JSONPARSER_HELP ;

template <class...>
trait JSONPARSER_IMPLHOLDER_HELP ;

template <class...>
trait JSONPARSER_SERIALIZATION_HELP ;

template <class DEPEND>
trait JSONPARSER_HELP<DEPEND ,ALWAYS> {
	template <class ARG1>
	using CRTP_JsonParser = typename DEPENDENT<JSONPARSER_HELP<DEPEND ,ALWAYS> ,ARG1>::JsonParser ;

	struct Holder implement Interface {
		virtual void initialize (CREF<TEMP<void>> heap ,CREF<INDEX> index) = 0 ;
		virtual BOOL exist () const = 0 ;
		virtual BOOL string_type () const = 0 ;
		virtual BOOL array_type () const = 0 ;
		virtual BOOL object_type () const = 0 ;
		virtual CRTP_JsonParser<DEPEND> clone () const = 0 ;
		virtual CRTP_JsonParser<DEPEND> root () const = 0 ;
		virtual CRTP_JsonParser<DEPEND> parent () const = 0 ;
		virtual CRTP_JsonParser<DEPEND> brother () const = 0 ;
		virtual CRTP_JsonParser<DEPEND> child () const = 0 ;
		virtual CRTP_JsonParser<DEPEND> child (CREF<Slice<STR>> name) const = 0 ;
		virtual CRTP_JsonParser<DEPEND> child (CREF<String<STRU8>> name) const = 0 ;
		virtual Array<CRTP_JsonParser<DEPEND>> child_array () const = 0 ;
		virtual Array<CRTP_JsonParser<DEPEND>> child_array (CREF<LENGTH> size_) const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual CREF<String<STRU8>> fetch () const leftvalue = 0 ;
		virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
		virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
		virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
		virtual SINGLE fetch (CREF<SINGLE> def) const = 0 ;
		virtual DOUBLE fetch (CREF<DOUBLE> def) const = 0 ;
		virtual BYTE fetch (CREF<BYTE> def) const = 0 ;
		virtual WORD fetch (CREF<WORD> def) const = 0 ;
		virtual CHAR fetch (CREF<CHAR> def) const = 0 ;
		virtual DATA fetch (CREF<DATA> def) const = 0 ;
		virtual String<STRA> fetch (CREF<String<STRA>> def) const = 0 ;
		virtual String<STRW> fetch (CREF<String<STRW>> def) const = 0 ;
		virtual String<STRU8> fetch (CREF<String<STRU8>> def) const = 0 ;
		virtual String<STRU16> fetch (CREF<String<STRU16>> def) const = 0 ;
		virtual String<STRU32> fetch (CREF<String<STRU32>> def) const = 0 ;
	} ;

	class FakeHolder implement Holder {
	protected:
		CRef<TEMP<void>> mHeap ;
		INDEX mIndex ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class JsonParser {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit JsonParser () {
			mThis = FUNCTION_extern::invoke () ;
		}

		explicit JsonParser (CREF<TEMP<void>> heap ,CREF<INDEX> index) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (heap ,index) ;
		}

		BOOL exist () const {
			return mThis->exist () ;
		}

		BOOL string_type () const {
			return mThis->string_type () ;
		}

		BOOL array_type () const {
			return mThis->array_type () ;
		}

		BOOL object_type () const {
			return mThis->object_type () ;
		}

		JsonParser clone () const {
			return mThis->clone () ;
		}

		JsonParser root () const {
			return mThis->root () ;
		}

		JsonParser parent () const {
			return mThis->parent () ;
		}

		JsonParser brother () const {
			return mThis->brother () ;
		}

		JsonParser child () const {
			return mThis->child () ;
		}

		JsonParser child (CREF<Slice<STR>> name) const {
			return child (String<STRU8>::make (name)) ;
		}

		JsonParser child (CREF<String<STRU8>> name) const {
			return mThis->child (name) ;
		}

		Array<JsonParser> child_array () const {
			return mThis->child_array () ;
		}

		Array<JsonParser> child_array (CREF<LENGTH> size_) const {
			return mThis->child_array (size_) ;
		}

		BOOL equal (CREF<JsonParser> that) const {
			return mThis->equal (that.mThis) ;
		}

		inline BOOL operator== (CREF<JsonParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<JsonParser> that) const {
			return ifnot (equal (that)) ;
		}

		CREF<String<STRU8>> fetch () const leftvalue {
			return mThis->fetch () ;
		}

		BOOL fetch (CREF<BOOL> def) const {
			return mThis->fetch (def) ;
		}

		VAL32 fetch (CREF<VAL32> def) const {
			return mThis->fetch (def) ;
		}

		VAL64 fetch (CREF<VAL64> def) const {
			return mThis->fetch (def) ;
		}

		SINGLE fetch (CREF<SINGLE> def) const {
			return mThis->fetch (def) ;
		}

		DOUBLE fetch (CREF<DOUBLE> def) const {
			return mThis->fetch (def) ;
		}

		BYTE fetch (CREF<BYTE> def) const {
			return mThis->fetch (def) ;
		}

		WORD fetch (CREF<WORD> def) const {
			return mThis->fetch (def) ;
		}

		CHAR fetch (CREF<CHAR> def) const {
			return mThis->fetch (def) ;
		}

		DATA fetch (CREF<DATA> def) const {
			return mThis->fetch (def) ;
		}

		String<STRA> fetch (CREF<String<STRA>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRW> fetch (CREF<String<STRW>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRU8> fetch (CREF<String<STRU8>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRU16> fetch (CREF<String<STRU16>> def) const {
			return mThis->fetch (def) ;
		}

		String<STRU32> fetch (CREF<String<STRU32>> def) const {
			return mThis->fetch (def) ;
		}
	} ;
} ;

using JsonParser = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::JsonParser ;
} ;