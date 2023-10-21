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
	struct Layout ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void initialize (CREF<RegBuffer<STRU8>> stream) = 0 ;
		virtual Layout clone () const = 0 ;
		virtual BOOL good () const = 0 ;
		virtual Layout root () const = 0 ;
		virtual Layout parent () const = 0 ;
		virtual Layout brother () const = 0 ;
		virtual Layout child () const = 0 ;
		virtual Layout child (CREF<String<STRU8>> name) const = 0 ;
		virtual Array<Layout> children () const = 0 ;
		virtual Array<Layout> children (CREF<LENGTH> size_) const = 0 ;
		virtual Layout concat (CREF<Layout> that) const = 0 ;
		virtual BOOL equal (CREF<Layout> that) const = 0 ;
		virtual CREF<String<STRU8>> member () const leftvalue = 0 ;
		virtual CREF<String<STRU8>> member (CREF<String<STRU8>> tag) const leftvalue = 0 ;
		virtual CREF<String<STRU8>> fetch () const leftvalue = 0 ;
		virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
		virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
		virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
		virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
		virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
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

	struct Layout {
		VRef<Holder> mThis ;
	} ;

	class XmlParser implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit XmlParser () = default ;

		implicit XmlParser (RREF<Layout> that) {
			mThis = move (that.mThis) ;
		}

		explicit XmlParser (CREF<BoolProxy> ok) {
			mThis = Holder::create () ;
			mThis->initialize () ;
		}

		explicit XmlParser (CREF<RegBuffer<STRU8>> stream) {
			mThis = Holder::create () ;
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

		BOOL good () const {
			return mThis->good () ;
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

		Array<XmlParser> children () const {
			Array<XmlParser> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<Array<Layout>>::expr] (ret) ;
			tmp = mThis->children () ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		Array<XmlParser> children (CREF<LENGTH> size_) const {
			Array<XmlParser> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<Array<Layout>>::expr] (ret) ;
			tmp = mThis->children (size_) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		BOOL equal (CREF<XmlParser> that) const {
			return mThis->equal (that) ;
		}

		inline BOOL operator== (CREF<XmlParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<XmlParser> that) const {
			return ifnot (equal (that)) ;
		}

		XmlParser concat (CREF<XmlParser> that) const {
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

		FLT32 fetch (CREF<FLT32> def) const {
			return mThis->fetch (def) ;
		}

		FLT64 fetch (CREF<FLT64> def) const {
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
	struct Layout ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void initialize (CREF<RegBuffer<STRU8>> stream) = 0 ;
		virtual Layout clone () const = 0 ;
		virtual BOOL good () const = 0 ;
		virtual BOOL string_type () const = 0 ;
		virtual BOOL array_type () const = 0 ;
		virtual BOOL object_type () const = 0 ;
		virtual Layout root () const = 0 ;
		virtual Layout parent () const = 0 ;
		virtual Layout brother () const = 0 ;
		virtual Layout child () const = 0 ;
		virtual Layout child (CREF<String<STRU8>> name) const = 0 ;
		virtual Array<Layout> children () const = 0 ;
		virtual Array<Layout> children (CREF<LENGTH> size_) const = 0 ;
		virtual BOOL equal (CREF<Layout> that) const = 0 ;
		virtual CREF<String<STRU8>> member () const leftvalue = 0 ;
		virtual CREF<String<STRU8>> fetch () const leftvalue = 0 ;
		virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
		virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
		virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
		virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
		virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
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

	struct Layout {
		VRef<Holder> mThis ;
	} ;

	class JsonParser implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit JsonParser () = default ;

		implicit JsonParser (RREF<Layout> that) {
			mThis = move (that.mThis) ;
		}

		explicit JsonParser (CREF<BoolProxy> ok) {
			mThis = Holder::create () ;
			mThis->initialize () ;
		}

		explicit JsonParser (CREF<RegBuffer<STRU8>> stream) {
			mThis = Holder::create () ;
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

		BOOL good () const {
			return mThis->good () ;
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

		Array<JsonParser> children () const {
			Array<JsonParser> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<Array<Layout>>::expr] (ret) ;
			tmp = mThis->children () ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		Array<JsonParser> children (CREF<LENGTH> size_) const {
			Array<JsonParser> ret ;
			auto &&tmp = unsafe_cast[TYPEAS<Array<Layout>>::expr] (ret) ;
			tmp = mThis->children (size_) ;
			unsafe_launder (ret) ;
			return move (ret) ;
		}

		BOOL equal (CREF<JsonParser> that) const {
			return mThis->equal (that) ;
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

		FLT32 fetch (CREF<FLT32> def) const {
			return mThis->fetch (def) ;
		}

		FLT64 fetch (CREF<FLT64> def) const {
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
		imports VRef<Holder> create () ;

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
		virtual void read (VREF<FLT32> item) = 0 ;
		virtual void read (VREF<FLT64> item) = 0 ;
		virtual void read (VREF<BYTE> item) = 0 ;
		virtual void read (VREF<WORD> item) = 0 ;
		virtual void read (VREF<CHAR> item) = 0 ;
		virtual void read (VREF<DATA> item) = 0 ;
	} ;

	class PlyReader {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit PlyReader () = default ;

		explicit PlyReader (CREF<RegBuffer<STRU8>> stream) {
			mThis = Holder::create () ;
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
		ARG1 poll (TYPEID<ARG1> id) {
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

		void read (VREF<FLT32> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<FLT32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<FLT64> item) {
			return mThis->read (item) ;
		}

		inline VREF<PlyReader> operator>> (VREF<FLT64> item) {
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