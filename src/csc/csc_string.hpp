#pragma once

#ifndef __CSC_STRING__
#define __CSC_STRING__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"

namespace CSC {
class IdentifierText implement StreamReaderFriend {
protected:
	PTR<VREF<String<STRU8>>> mText ;

public:
	implicit IdentifierText () = delete ;

	explicit IdentifierText (VREF<String<STRU8>> text) :mText ((&text)) {}

	void friend_read (VREF<StreamReader> reader) override {
		unimplemented () ;
	}
} ;

class ScalarText implement StreamReaderFriend {
protected:
	PTR<VREF<String<STRU8>>> mText ;

public:
	implicit ScalarText () = delete ;

	explicit ScalarText (VREF<String<STRU8>> text) :mText ((&text)) {}

	void friend_read (VREF<StreamReader> reader) override {
		unimplemented () ;
	}
} ;

class EscapeText implement StreamReaderFriend ,public StreamWriterFriend {
protected:
	PTR<VREF<String<STRU8>>> mText ;

public:
	implicit EscapeText () = delete ;

	explicit EscapeText (VREF<String<STRU8>> text) :mText ((&text)) {}

	void friend_read (VREF<StreamReader> reader) override {
		unimplemented () ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		unimplemented () ;
	}
} ;

class NotEndlineText implement StreamReaderFriend {
protected:
	PTR<VREF<String<STRU8>>> mText ;

public:
	implicit NotEndlineText () = delete ;

	explicit NotEndlineText (VREF<String<STRU8>> text) :mText ((&text)) {}

	void friend_read (VREF<StreamReader> reader) override {
		unimplemented () ;
	}
} ;

class AlignedText implement StreamWriterFriend {
protected:
	VAL64 mText ;
	VAL64 mAlign ;

public:
	implicit AlignedText () = delete ;

	explicit AlignedText (CREF<VAL64> text ,CREF<VAL64> align) {
		assert (text >= 0) ;
		assert (align >= 1) ;
		mText = text ;
		mAlign = align ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		auto rax = LENGTH (1) ;
		for (auto &&i : iter (0 ,mAlign - 1)) {
			noop (i) ;
			rax *= 10 ;
			if (mText >= rax)
				continue ;
			writer.write (STRU32 ('0')) ;
		}
		rax *= 10 ;
		const auto r1x = mText % rax ;
		writer.write (r1x) ;
	}
} ;

struct XmlParserPureLayout ;

struct XmlParserLayout {
	Ref<XmlParserPureLayout> mThis ;
	INDEX mIndex ;
} ;

struct XmlParserHolder implement Interface {
	imports VFat<XmlParserHolder> create (VREF<XmlParserLayout> that) ;
	imports CFat<XmlParserHolder> create (CREF<XmlParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual void initialize (CREF<XmlParserLayout> that) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual XmlParserLayout root () const = 0 ;
	virtual XmlParserLayout parent () const = 0 ;
	virtual XmlParserLayout brother () const = 0 ;
	virtual XmlParserLayout child () const = 0 ;
	virtual XmlParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual XmlParserLayout child (CREF<Slice> name) const = 0 ;
	virtual XmlParserLayout child (CREF<String<STRU8>> name) const = 0 ;
	virtual Array<XmlParserLayout> list () const = 0 ;
	virtual Array<XmlParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<XmlParserLayout> that) const = 0 ;
	virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
	virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
	virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
	virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
	virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
	virtual String<STRA> fetch (CREF<String<STRA>> def) const = 0 ;
	virtual String<STRW> fetch (CREF<String<STRW>> def) const = 0 ;
	virtual String<STRU8> fetch (CREF<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> fetch (CREF<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> fetch (CREF<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class XmlParser implement XmlParserLayout {
protected:
	using XmlParserLayout::mThis ;
	using XmlParserLayout::mIndex ;

public:
	implicit XmlParser () = default ;

	explicit XmlParser (CREF<RefBuffer<BYTE>> stream) {
		XmlParserHolder::create (thiz)->initialize (stream) ;
	}

	implicit XmlParser (CREF<XmlParser> that) {
		XmlParserHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<XmlParser> operator= (CREF<XmlParser> that) {
		return assign (thiz ,that) ;
	}

	implicit XmlParser (RREF<XmlParser> that) = default ;

	forceinline VREF<XmlParser> operator= (RREF<XmlParser> that) = default ;

	BOOL exist () const {
		return XmlParserHolder::create (thiz)->exist () ;
	}

	XmlParser root () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->root () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser parent () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->parent () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser brother () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->brother () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<INDEX> index) const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child (index) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<Slice> name) const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<String<STRU8>> name) const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	Array<XmlParser> list () const {
		Array<XmlParserLayout> ret = XmlParserHolder::create (thiz)->list () ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	Array<XmlParser> list (CREF<LENGTH> size_) const {
		Array<XmlParserLayout> ret = XmlParserHolder::create (thiz)->list (size_) ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	BOOL equal (CREF<XmlParser> that) const {
		return XmlParserHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<XmlParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<XmlParser> that) const {
		return !(equal (that)) ;
	}

	BOOL fetch (CREF<BOOL> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	VAL32 fetch (CREF<VAL32> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	VAL64 fetch (CREF<VAL64> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	FLT32 fetch (CREF<FLT32> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	FLT64 fetch (CREF<FLT64> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}
} ;

struct JsonParserPureLayout ;

struct JsonParserLayout {
	Ref<JsonParserPureLayout> mThis ;
	INDEX mIndex ;
} ;

struct JsonParserHolder implement Interface {
	imports VFat<JsonParserHolder> create (VREF<JsonParserLayout> that) ;
	imports CFat<JsonParserHolder> create (CREF<JsonParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual void initialize (CREF<JsonParserLayout> that) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual JsonParserLayout root () const = 0 ;
	virtual JsonParserLayout parent () const = 0 ;
	virtual JsonParserLayout brother () const = 0 ;
	virtual JsonParserLayout child () const = 0 ;
	virtual JsonParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual JsonParserLayout child (CREF<Slice> name) const = 0 ;
	virtual JsonParserLayout child (CREF<String<STRU8>> name) const = 0 ;
	virtual Array<JsonParserLayout> list () const = 0 ;
	virtual Array<JsonParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<JsonParserLayout> that) const = 0 ;
	virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
	virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
	virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
	virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
	virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
	virtual String<STRA> fetch (CREF<String<STRA>> def) const = 0 ;
	virtual String<STRW> fetch (CREF<String<STRW>> def) const = 0 ;
	virtual String<STRU8> fetch (CREF<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> fetch (CREF<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> fetch (CREF<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class JsonParser implement JsonParserLayout {
protected:
	using JsonParserLayout::mThis ;
	using JsonParserLayout::mIndex ;

public:
	implicit JsonParser () = default ;

	explicit JsonParser (CREF<RefBuffer<BYTE>> stream) {
		JsonParserHolder::create (thiz)->initialize (stream) ;
	}

	implicit JsonParser (CREF<JsonParser> that) {
		JsonParserHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<JsonParser> operator= (CREF<JsonParser> that) {
		return assign (thiz ,that) ;
	}

	implicit JsonParser (RREF<JsonParser> that) = default ;

	forceinline VREF<JsonParser> operator= (RREF<JsonParser> that) = default ;

	BOOL exist () const {
		return JsonParserHolder::create (thiz)->exist () ;
	}

	JsonParser root () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->root () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser parent () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->parent () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser brother () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->brother () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<INDEX> index) const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child (index) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<Slice> name) const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<String<STRU8>> name) const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	Array<JsonParser> list () const {
		Array<JsonParserLayout> ret = JsonParserHolder::create (thiz)->list () ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	Array<JsonParser> list (CREF<LENGTH> size_) const {
		Array<JsonParserLayout> ret = JsonParserHolder::create (thiz)->list (size_) ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	BOOL equal (CREF<JsonParser> that) const {
		return JsonParserHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<JsonParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<JsonParser> that) const {
		return !(equal (that)) ;
	}

	BOOL fetch (CREF<BOOL> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	VAL32 fetch (CREF<VAL32> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	VAL64 fetch (CREF<VAL64> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	FLT32 fetch (CREF<FLT32> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	FLT64 fetch (CREF<FLT64> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}
} ;

struct PlyParserGuide {
	INDEX mElement ;
	ArrayList<INDEX> mProperty ;
	INDEX mPropertyIndex ;
	INDEX mLineIndex ;
	INDEX mPlyIndex ;
	FLAG mPlyType ;
} ;

struct PlyParserPureLayout ;

struct PlyParserLayout {
	Ref<PlyParserPureLayout> mThis ;
	PlyParserGuide mGuide ;
} ;

struct PlyParserHolder implement Interface {
	imports VFat<PlyParserHolder> create (VREF<PlyParserLayout> that) ;
	imports CFat<PlyParserHolder> create (CREF<PlyParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual INDEX find_element (CREF<Slice> name) const = 0 ;
	virtual LENGTH element_size (CREF<INDEX> element) const = 0 ;
	virtual INDEX find_property (CREF<INDEX> element ,CREF<Slice> name) const = 0 ;
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
	virtual void read (VREF<QUAD> item) = 0 ;
} ;

class PlyParser implement PlyParserLayout {
protected:
	using PlyParserLayout::mThis ;

public:
	implicit PlyParser () = default ;

	explicit PlyParser (CREF<RefBuffer<BYTE>> stream) {
		PlyParserHolder::create (thiz)->initialize (stream) ;
	}

	INDEX find_element (CREF<Slice> name) const {
		return PlyParserHolder::create (thiz)->find_element (name) ;
	}

	LENGTH element_size (CREF<INDEX> element) const {
		return PlyParserHolder::create (thiz)->element_size (element) ;
	}

	INDEX find_property (CREF<INDEX> element ,CREF<Slice> name) const {
		return PlyParserHolder::create (thiz)->find_property (element ,name) ;
	}

	LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const {
		return PlyParserHolder::create (thiz)->property_size (element ,line ,property) ;
	}

	void guide_new (CREF<INDEX> element) {
		return PlyParserHolder::create (thiz)->guide_new (element) ;
	}

	void guide_put (CREF<INDEX> property) {
		return PlyParserHolder::create (thiz)->guide_put (property) ;
	}

	void read (VREF<BOOL> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<VAL32> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<VAL64> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<FLT32> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<FLT64> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<BYTE> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<WORD> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<CHAR> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<QUAD> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}
} ;
} ;