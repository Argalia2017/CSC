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

template <class DEPEND>
trait XMLPARSER_HELP<DEPEND ,ALWAYS> {
	class XmlParser ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void initialize (CREF<RegBuffer<STRU8>> stream) = 0 ;
		virtual XmlParser clone () const = 0 ;
		virtual BOOL available () const = 0 ;
		virtual XmlParser root () const = 0 ;
		virtual XmlParser parent () const = 0 ;
		virtual XmlParser brother () const = 0 ;
		virtual XmlParser child () const = 0 ;
		virtual XmlParser child (CREF<String<STRU8>> name) const = 0 ;
		virtual Array<XmlParser> child_array () const = 0 ;
		virtual Array<XmlParser> child_array (CREF<LENGTH> size_) const = 0 ;
		virtual XmlParser concat (CREF<XmlParser> that) const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual CREF<String<STRU8>> member () const leftvalue = 0 ;
		virtual CREF<String<STRU8>> member (CREF<String<STRU8>> tag) const leftvalue = 0 ;
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
		CRef<Proxy> mHeap ;
		INDEX mIndex ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class XmlParser {
	private:
		template <class...>
		friend trait XMLPARSER_COMBINATION_HELP ;

	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit XmlParser () = default ;

		explicit XmlParser (RREF<Box<FakeHolder>> that) {
			mThis = move (that) ;
		}

		explicit XmlParser (CREF<BoolProxy> ok) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		explicit XmlParser (CREF<RegBuffer<STRU8>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (stream) ;
		}

		implicit XmlParser (CREF<XmlParser> that) {
			if (that.mThis == NULL)
				return ;
			thiz = that.mThis->clone () ;
		}

		inline VREF<XmlParser> operator= (CREF<XmlParser> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit XmlParser (RREF<XmlParser> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<XmlParser> operator= (RREF<XmlParser> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL available () const {
			return mThis->available () ;
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
			return child (PrintString<STRU8>::make (name)) ;
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
			return mThis->equal (that.mThis.self) ;
		}

		inline BOOL operator== (CREF<XmlParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<XmlParser> that) const {
			return ifnot (equal (that)) ;
		}

		XmlParser concat (CREF<XmlParser> that) {
			return mThis->concat (that) ;
		}

		inline XmlParser operator+ (CREF<XmlParser> that) const {
			return concat (that) ;
		}

		inline void operator+= (CREF<XmlParser> that) {
			thiz = concat (that) ;
		}

		CREF<String<STRU8>> member () const leftvalue {
			return mThis->member () ;
		}

		CREF<String<STRU8>> member (CREF<Slice<STR>> tag) const leftvalue {
			return member (PrintString<STRU8>::make (tag)) ;
		}

		CREF<String<STRU8>> member (CREF<String<STRU8>> tag) const leftvalue {
			return mThis->member (tag) ;
		}

		CREF<String<STRU8>> fetch () const leftvalue {
			return mThis->fetch () ;
		}

		BOOL fetch (CREF<BOOL> def) const {
			return mThis->fetch (def) ;
		}

		csc_pointer_t fetch (CREF<csc_const_pointer_t>) const = delete ;

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

template <class DEPEND>
trait JSONPARSER_HELP<DEPEND ,ALWAYS> {
	class JsonParser ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void initialize (CREF<RegBuffer<STRU8>> stream) = 0 ;
		virtual JsonParser clone () const = 0 ;
		virtual BOOL available () const = 0 ;
		virtual BOOL string_type () const = 0 ;
		virtual BOOL array_type () const = 0 ;
		virtual BOOL object_type () const = 0 ;
		virtual JsonParser root () const = 0 ;
		virtual JsonParser parent () const = 0 ;
		virtual JsonParser brother () const = 0 ;
		virtual JsonParser child () const = 0 ;
		virtual JsonParser child (CREF<String<STRU8>> name) const = 0 ;
		virtual Array<JsonParser> child_array () const = 0 ;
		virtual Array<JsonParser> child_array (CREF<LENGTH> size_) const = 0 ;
		virtual BOOL equal (CREF<Holder> that) const = 0 ;
		virtual CREF<String<STRU8>> member () const leftvalue = 0 ;
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
		CRef<Proxy> mHeap ;
		INDEX mIndex ;
	} ;

	struct FUNCTION_extern {
		imports Box<FakeHolder> invoke () ;
	} ;

	class JsonParser {
	protected:
		Box<FakeHolder> mThis ;

	public:
		implicit JsonParser () = default ;

		explicit JsonParser (RREF<Box<FakeHolder>> that) {
			mThis = move (that) ;
		}

		explicit JsonParser (CREF<BoolProxy> ok) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		explicit JsonParser (CREF<RegBuffer<STRU8>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (stream) ;
		}

		implicit JsonParser (CREF<JsonParser> that) {
			if (that.mThis == NULL)
				return ;
			thiz = that.mThis->clone () ;
		}

		inline VREF<JsonParser> operator= (CREF<JsonParser> that) {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit JsonParser (RREF<JsonParser> that) noexcept {
			swap (thiz ,that) ;
		}

		inline VREF<JsonParser> operator= (RREF<JsonParser> that) noexcept {
			if (address (thiz) == address (that))
				return thiz ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL available () const {
			return mThis->available () ;
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
			return child (PrintString<STRU8>::make (name)) ;
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
			return mThis->equal (that.mThis.self) ;
		}

		inline BOOL operator== (CREF<JsonParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<JsonParser> that) const {
			return ifnot (equal (that)) ;
		}

		CREF<String<STRU8>> member () const leftvalue {
			return mThis->member () ;
		}

		CREF<String<STRU8>> fetch () const leftvalue {
			return mThis->fetch () ;
		}

		BOOL fetch (CREF<BOOL> def) const {
			return mThis->fetch (def) ;
		}

		csc_pointer_t fetch (CREF<csc_const_pointer_t>) const = delete ;

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

template <class...>
trait PLYREADER_HELP ;

template <class...>
trait PLYREADER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait PLYREADER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<RegBuffer<STRU8>> stream) = 0 ;
		virtual INDEX find_element (CREF<String<STRU8>> name) const = 0 ;
		virtual LENGTH element_size (CREF<INDEX> element) const = 0 ;
		virtual INDEX find_property (CREF<INDEX> element ,CREF<String<STRU8>> name) const = 0 ;
		virtual LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const = 0 ;
		virtual void guide_new (CREF<INDEX> element) = 0 ;
		virtual void guide_put (CREF<INDEX> property) = 0 ;
		virtual void read (VREF<BOOL> item) = 0 ;
		virtual void read (VREF<VAL32> item) = 0 ;
		virtual void read (VREF<VAL64> item) = 0 ;
		virtual void read (VREF<SINGLE> item) = 0 ;
		virtual void read (VREF<DOUBLE> item) = 0 ;
		virtual void read (VREF<BYTE> item) = 0 ;
		virtual void read (VREF<WORD> item) = 0 ;
		virtual void read (VREF<CHAR> item) = 0 ;
		virtual void read (VREF<DATA> item) = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class PlyReader {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit PlyReader () = default ;

		explicit PlyReader (CREF<RegBuffer<STRU8>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (stream) ;
		}

		INDEX find_element (CREF<Slice<STR>> name) const {
			return mThis->find_element (PrintString<STRU8>::make (name)) ;
		}

		INDEX find_element (CREF<String<STRU8>> name) const {
			return mThis->find_element (name) ;
		}

		LENGTH element_size (CREF<INDEX> element) const {
			return mThis->element_size (element) ;
		}

		INDEX find_property (CREF<INDEX> element ,CREF<Slice<STR>> name) const {
			return mThis->find_property (element ,PrintString<STRU8>::make (name)) ;
		}

		INDEX find_property (CREF<INDEX> element ,CREF<String<STRU8>> name) const {
			return mThis->find_property (element ,name) ;
		}

		LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const {
			return mThis->property_size (element ,line ,property) ;
		}

		void guide_new (CREF<INDEX> element) {
			return mThis->guide_new (element) ;
		}

		void guide_put (CREF<INDEX> property) {
			return mThis->guide_put (property) ;
		}

		template <class ARG1>
		ARG1 poll (CREF<TYPEID<ARG1>> id) {
			ARG1 ret ;
			read (ret) ;
			return move (ret) ;
		}

		void scans () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void scans (VREF<ARG1> item1 ,VREF<ARG2>...item2) {
			read (item1) ;
			scans (item2...) ;
		}

		void read (VREF<BOOL> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BYTE> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<DATA> item) {
			read (item) ;
			return thiz ;
		}
	} ;
} ;

using PlyReader = typename PLYREADER_HELP<DEPEND ,ALWAYS>::PlyReader ;
} ;