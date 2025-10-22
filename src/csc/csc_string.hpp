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
struct StringProcLayout ;

struct StringProcHolder implement Interface {
	imports CR<Like<UniqueRef<StringProcLayout>>> expr_m () ;
	imports VFat<StringProcHolder> hold (VR<StringProcLayout> that) ;
	imports CFat<StringProcHolder> hold (CR<StringProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual String<STRA> stra_from_strw (CR<String<STRW>> a) const = 0 ;
	virtual String<STRA> stra_from_strs (CR<String<STR>> a) const = 0 ;
	virtual String<STRW> strw_from_stra (CR<String<STRA>> a) const = 0 ;
	virtual String<STRW> strw_from_strs (CR<String<STR>> a) const = 0 ;
	virtual String<STR> strs_from_stra (CR<String<STRA>> a) const = 0 ;
	virtual String<STR> strs_from_strw (CR<String<STRW>> a) const = 0 ;
	virtual String<STRU8> stru8_from_stru16 (CR<String<STRU16>> a) const = 0 ;
	virtual String<STRU8> stru8_from_stru32 (CR<String<STRU32>> a) const = 0 ;
	virtual String<STRU16> stru16_from_stru8 (CR<String<STRU8>> a) const = 0 ;
	virtual String<STRU16> stru16_from_stru32 (CR<String<STRU32>> a) const = 0 ;
	virtual String<STRU32> stru32_from_stru8 (CR<String<STRU8>> a) const = 0 ;
	virtual String<STRU32> stru32_from_stru16 (CR<String<STRU16>> a) const = 0 ;
	virtual String<STRUA> strua_from_stra (RR<String<STRA>> a) const = 0 ;
	virtual String<STRA> stra_from_strua (RR<String<STRUA>> a) const = 0 ;
	virtual String<STRUW> struw_from_strw (RR<String<STRW>> a) const = 0 ;
	virtual String<STRW> strw_from_struw (RR<String<STRUW>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CR<String<STRU8>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CR<String<STRU16>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CR<String<STRU32>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CR<String<STRU8>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CR<String<STRU16>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CR<String<STRU32>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CR<String<STRU8>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CR<String<STRU16>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CR<String<STRU32>> a) const = 0 ;
	virtual String<STRU8> stru8_from_struw (CR<String<STRUW>> a) const = 0 ;
	virtual String<STRU16> stru16_from_struw (CR<String<STRUW>> a) const = 0 ;
	virtual String<STRU32> stru32_from_struw (CR<String<STRUW>> a) const = 0 ;
	virtual String<STRU8> stru8_from_strs (CR<String<STRA>> a) const = 0 ;
	virtual String<STRU8> stru8_from_strs (CR<String<STRW>> a) const = 0 ;
	virtual String<STRU16> stru16_from_strs (CR<String<STRA>> a) const = 0 ;
	virtual String<STRU16> stru16_from_strs (CR<String<STRW>> a) const = 0 ;
	virtual String<STRU32> stru32_from_strs (CR<String<STRA>> a) const = 0 ;
	virtual String<STRU32> stru32_from_strs (CR<String<STRW>> a) const = 0 ;
} ;

class StringProc implement Like<UniqueRef<StringProcLayout>> {
public:
	static CR<StringProc> expr_m () {
		return keep[TYPE<StringProc>::expr] (StringProcHolder::expr) ;
	}

	static String<STRA> stra_from_strw (CR<String<STRW>> a) {
		return StringProcHolder::hold (expr)->stra_from_strw (a) ;
	}

	static String<STRA> stra_from_strs (CR<String<STR>> a) {
		return StringProcHolder::hold (expr)->stra_from_strs (a) ;
	}

	static String<STRW> strw_from_stra (CR<String<STRA>> a) {
		return StringProcHolder::hold (expr)->strw_from_stra (a) ;
	}

	static String<STRW> strw_from_strs (CR<String<STR>> a) {
		return StringProcHolder::hold (expr)->strw_from_strs (a) ;
	}

	static String<STR> strs_from_stra (CR<String<STRA>> a) {
		return StringProcHolder::hold (expr)->strs_from_stra (a) ;
	}

	static String<STR> strs_from_strw (CR<String<STRW>> a) {
		return StringProcHolder::hold (expr)->strs_from_strw (a) ;
	}

	static String<STRU8> stru8_from_stru16 (CR<String<STRU16>> a) {
		return StringProcHolder::hold (expr)->stru8_from_stru16 (a) ;
	}

	static String<STRU8> stru8_from_stru32 (CR<String<STRU32>> a) {
		return StringProcHolder::hold (expr)->stru8_from_stru32 (a) ;
	}

	static String<STRU16> stru16_from_stru8 (CR<String<STRU8>> a) {
		return StringProcHolder::hold (expr)->stru16_from_stru8 (a) ;
	}

	static String<STRU16> stru16_from_stru32 (CR<String<STRU32>> a) {
		return StringProcHolder::hold (expr)->stru16_from_stru32 (a) ;
	}

	static String<STRU32> stru32_from_stru8 (CR<String<STRU8>> a) {
		return StringProcHolder::hold (expr)->stru32_from_stru8 (a) ;
	}

	static String<STRU32> stru32_from_stru16 (CR<String<STRU16>> a) {
		return StringProcHolder::hold (expr)->stru32_from_stru16 (a) ;
	}

	static String<STRUA> strua_from_stra (RR<String<STRA>> a) {
		return StringProcHolder::hold (expr)->strua_from_stra (move (a)) ;
	}

	static String<STRA> stra_from_strua (RR<String<STRUA>> a) {
		return StringProcHolder::hold (expr)->stra_from_strua (move (a)) ;
	}

	static String<STRUW> struw_from_strw (RR<String<STRW>> a) {
		return StringProcHolder::hold (expr)->struw_from_strw (move (a)) ;
	}

	static String<STRW> strw_from_struw (RR<String<STRUW>> a) {
		return StringProcHolder::hold (expr)->strw_from_struw (move (a)) ;
	}

	static String<STRA> stra_from_stru (CR<String<STRU8>> a) {
		return StringProcHolder::hold (expr)->stra_from_stru (a) ;
	}

	static String<STRA> stra_from_stru (CR<String<STRU16>> a) {
		return StringProcHolder::hold (expr)->stra_from_stru (a) ;
	}

	static String<STRA> stra_from_stru (CR<String<STRU32>> a) {
		return StringProcHolder::hold (expr)->stra_from_stru (a) ;
	}

	static String<STRW> strw_from_stru (CR<String<STRU8>> a) {
		return StringProcHolder::hold (expr)->strw_from_stru (a) ;
	}

	static String<STRW> strw_from_stru (CR<String<STRU16>> a) {
		return StringProcHolder::hold (expr)->strw_from_stru (a) ;
	}

	static String<STRW> strw_from_stru (CR<String<STRU32>> a) {
		return StringProcHolder::hold (expr)->strw_from_stru (a) ;
	}

	static String<STR> strs_from_stru (CR<String<STRU8>> a) {
		return StringProcHolder::hold (expr)->strs_from_stru (a) ;
	}

	static String<STR> strs_from_stru (CR<String<STRU16>> a) {
		return StringProcHolder::hold (expr)->strs_from_stru (a) ;
	}

	static String<STR> strs_from_stru (CR<String<STRU32>> a) {
		return StringProcHolder::hold (expr)->strs_from_stru (a) ;
	}

	static String<STRU8> stru8_from_struw (CR<String<STRUW>> a) {
		return StringProcHolder::hold (expr)->stru8_from_struw (a) ;
	}

	static String<STRU16> stru16_from_struw (CR<String<STRUW>> a) {
		return StringProcHolder::hold (expr)->stru16_from_struw (a) ;
	}

	static String<STRU32> stru32_from_struw (CR<String<STRUW>> a) {
		return StringProcHolder::hold (expr)->stru32_from_struw (a) ;
	}

	static String<STRU8> stru8_from_strs (CR<String<STRA>> a) {
		return StringProcHolder::hold (expr)->stru8_from_strs (a) ;
	}

	static String<STRU8> stru8_from_strs (CR<String<STRW>> a) {
		return StringProcHolder::hold (expr)->stru8_from_strs (a) ;
	}

	static String<STRU16> stru16_from_strs (CR<String<STRA>> a) {
		return StringProcHolder::hold (expr)->stru16_from_strs (a) ;
	}

	static String<STRU16> stru16_from_strs (CR<String<STRW>> a) {
		return StringProcHolder::hold (expr)->stru16_from_strs (a) ;
	}

	static String<STRU32> stru32_from_strs (CR<String<STRA>> a) {
		return StringProcHolder::hold (expr)->stru32_from_strs (a) ;
	}

	static String<STRU32> stru32_from_strs (CR<String<STRW>> a) {
		return StringProcHolder::hold (expr)->stru32_from_strs (a) ;
	}
} ;

struct XmlParserTree ;

struct XmlParserLayout {
	UniqueRef<XmlParserTree> mThis ;
	INDEX mIndex ;
} ;

struct XmlParserHolder implement Interface {
	imports VFat<XmlParserHolder> hold (VR<XmlParserLayout> that) ;
	imports CFat<XmlParserHolder> hold (CR<XmlParserLayout> that) ;

	virtual void initialize (CR<RefBuffer<BYTE>> stream) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual XmlParserLayout root () const = 0 ;
	virtual XmlParserLayout parent () const = 0 ;
	virtual XmlParserLayout brother () const = 0 ;
	virtual XmlParserLayout child () const = 0 ;
	virtual XmlParserLayout child (CR<INDEX> index) const = 0 ;
	virtual XmlParserLayout child (CR<Slice> name) const = 0 ;
	virtual XmlParserLayout child (CR<String<STRU8>> name) const = 0 ;
	virtual Array<XmlParserLayout> list () const = 0 ;
	virtual Array<XmlParserLayout> list (CR<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CR<XmlParserLayout> that) const = 0 ;
	virtual CR<String<STRU8>> name () const leftvalue = 0 ;
	virtual BOOL parse (CR<BOOL> def) const = 0 ;
	virtual VAL32 parse (CR<VAL32> def) const = 0 ;
	virtual VAL64 parse (CR<VAL64> def) const = 0 ;
	virtual FLT32 parse (CR<FLT32> def) const = 0 ;
	virtual FLT64 parse (CR<FLT64> def) const = 0 ;
	virtual String<STRA> parse (CR<String<STRA>> def) const = 0 ;
	virtual String<STRW> parse (CR<String<STRW>> def) const = 0 ;
	virtual String<STRU8> parse (CR<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> parse (CR<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> parse (CR<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> parse (CR<BOOL> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> parse (CR<VAL32> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> parse (CR<VAL64> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> parse (CR<FLT32> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> parse (CR<FLT64> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> parse (CR<String<STRA>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> parse (CR<String<STRW>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> parse (CR<String<STRU8>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> parse (CR<String<STRU16>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> parse (CR<String<STRU32>> def ,CR<LENGTH> size_) const = 0 ;
} ;

class XmlParser implement XmlParserLayout {
protected:
	using XmlParserLayout::mThis ;
	using XmlParserLayout::mIndex ;

public:
	implicit XmlParser () = default ;

	explicit XmlParser (CR<RefBuffer<BYTE>> stream) {
		XmlParserHolder::hold (thiz)->initialize (stream) ;
	}

	BOOL exist () const {
		return XmlParserHolder::hold (thiz)->exist () ;
	}

	XmlParser root () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->root () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser parent () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->parent () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser brother () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->brother () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child () const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CR<INDEX> index) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (index) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CR<Slice> name) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CR<String<STRU8>> name) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	Array<XmlParser> list () const {
		ArrayLayout ret = XmlParserHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (Pointer::from (ret))) ;
	}

	Array<XmlParser> list (CR<LENGTH> size_) const {
		ArrayLayout ret = XmlParserHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (Pointer::from (ret))) ;
	}

	BOOL equal (CR<XmlParser> that) const {
		return XmlParserHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<XmlParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<XmlParser> that) const {
		return (!equal (that)) ;
	}

	CR<String<STRU8>> name () const leftvalue {
		return XmlParserHolder::hold (thiz)->name () ;
	}

	BOOL parse (CR<BOOL> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	VAL32 parse (CR<VAL32> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	VAL64 parse (CR<VAL64> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	FLT32 parse (CR<FLT32> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	FLT64 parse (CR<FLT64> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRA> parse (CR<String<STRA>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRW> parse (CR<String<STRW>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU8> parse (CR<String<STRU8>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU16> parse (CR<String<STRU16>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU32> parse (CR<String<STRU32>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Array<BOOL> parse (CR<BOOL> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL32> parse (CR<VAL32> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL64> parse (CR<VAL64> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT32> parse (CR<FLT32> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT64> parse (CR<FLT64> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRA>> parse (CR<String<STRA>> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRW>> parse (CR<String<STRW>> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU8>> parse (CR<String<STRU8>> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU16>> parse (CR<String<STRU16>> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU32>> parse (CR<String<STRU32>> def ,CR<LENGTH> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}
} ;

struct JsonParserTree ;

struct JsonParserLayout {
	UniqueRef<JsonParserTree> mThis ;
	INDEX mIndex ;
} ;

struct JsonParserHolder implement Interface {
	imports VFat<JsonParserHolder> hold (VR<JsonParserLayout> that) ;
	imports CFat<JsonParserHolder> hold (CR<JsonParserLayout> that) ;

	virtual void initialize (CR<RefBuffer<BYTE>> stream) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual JsonParserLayout root () const = 0 ;
	virtual JsonParserLayout parent () const = 0 ;
	virtual JsonParserLayout brother () const = 0 ;
	virtual JsonParserLayout child () const = 0 ;
	virtual JsonParserLayout child (CR<INDEX> index) const = 0 ;
	virtual JsonParserLayout child (CR<Slice> name) const = 0 ;
	virtual JsonParserLayout child (CR<String<STRU8>> name) const = 0 ;
	virtual Array<JsonParserLayout> list () const = 0 ;
	virtual Array<JsonParserLayout> list (CR<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CR<JsonParserLayout> that) const = 0 ;
	virtual CR<String<STRU8>> name () const leftvalue = 0 ;
	virtual BOOL parse (CR<BOOL> def) const = 0 ;
	virtual VAL32 parse (CR<VAL32> def) const = 0 ;
	virtual VAL64 parse (CR<VAL64> def) const = 0 ;
	virtual FLT32 parse (CR<FLT32> def) const = 0 ;
	virtual FLT64 parse (CR<FLT64> def) const = 0 ;
	virtual String<STRA> parse (CR<String<STRA>> def) const = 0 ;
	virtual String<STRW> parse (CR<String<STRW>> def) const = 0 ;
	virtual String<STRU8> parse (CR<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> parse (CR<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> parse (CR<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> parse (CR<BOOL> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> parse (CR<VAL32> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> parse (CR<VAL64> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> parse (CR<FLT32> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> parse (CR<FLT64> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> parse (CR<String<STRA>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> parse (CR<String<STRW>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> parse (CR<String<STRU8>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> parse (CR<String<STRU16>> def ,CR<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> parse (CR<String<STRU32>> def ,CR<LENGTH> size_) const = 0 ;
} ;

class JsonParser implement JsonParserLayout {
protected:
	using JsonParserLayout::mThis ;
	using JsonParserLayout::mIndex ;

public:
	implicit JsonParser () = default ;

	explicit JsonParser (CR<RefBuffer<BYTE>> stream) {
		JsonParserHolder::hold (thiz)->initialize (stream) ;
	}

	BOOL exist () const {
		return JsonParserHolder::hold (thiz)->exist () ;
	}

	JsonParser root () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->root () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser parent () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->parent () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser brother () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->brother () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child () const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CR<INDEX> index) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (index) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CR<Slice> name) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CR<String<STRU8>> name) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	Array<JsonParser> list () const {
		ArrayLayout ret = JsonParserHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (Pointer::from (ret))) ;
	}

	Array<JsonParser> list (CR<LENGTH> size_) const {
		ArrayLayout ret = JsonParserHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (Pointer::from (ret))) ;
	}

	BOOL equal (CR<JsonParser> that) const {
		return JsonParserHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<JsonParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<JsonParser> that) const {
		return (!equal (that)) ;
	}

	CR<String<STRU8>> name () const leftvalue {
		return JsonParserHolder::hold (thiz)->name () ;
	}

	BOOL parse (CR<BOOL> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	VAL32 parse (CR<VAL32> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	VAL64 parse (CR<VAL64> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	FLT32 parse (CR<FLT32> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	FLT64 parse (CR<FLT64> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRA> parse (CR<String<STRA>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRW> parse (CR<String<STRW>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU8> parse (CR<String<STRU8>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU16> parse (CR<String<STRU16>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<STRU32> parse (CR<String<STRU32>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Array<BOOL> parse (CR<BOOL> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL32> parse (CR<VAL32> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<VAL64> parse (CR<VAL64> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT32> parse (CR<FLT32> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<FLT64> parse (CR<FLT64> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRA>> parse (CR<String<STRA>> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRW>> parse (CR<String<STRW>> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU8>> parse (CR<String<STRU8>> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU16>> parse (CR<String<STRU16>> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<STRU32>> parse (CR<String<STRU32>> def ,CR<LENGTH> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}
} ;

struct PlyParserTree ;

struct PlyParserGuide {
	INDEX mElement ;
	Deque<INDEX> mProperty ;
	INDEX mCol ;
	INDEX mRow ;
	INDEX mPlyBegin ;
	INDEX mPlyEnd ;
	INDEX mPlyIndex ;
	BOOL mPlyListMode ;
} ;

struct PlyParserLayout {
	UniqueRef<PlyParserTree> mThis ;
	PlyParserGuide mGuide ;
} ;

struct PlyParserHolder implement Interface {
	imports VFat<PlyParserHolder> hold (VR<PlyParserLayout> that) ;
	imports CFat<PlyParserHolder> hold (CR<PlyParserLayout> that) ;

	virtual void initialize (CR<RefBuffer<BYTE>> stream) = 0 ;
	virtual LENGTH element_size (CR<Slice> element) const = 0 ;
	virtual LENGTH property_size (CR<Slice> element ,CR<Slice> property) const = 0 ;
	virtual void guide_new (CR<Slice> element) = 0 ;
	virtual void guide_put (CR<Slice> property) = 0 ;
	virtual void read (VR<BOOL> item) = 0 ;
	virtual void read (VR<VAL32> item) = 0 ;
	virtual void read (VR<VAL64> item) = 0 ;
	virtual void read (VR<FLT32> item) = 0 ;
	virtual void read (VR<FLT64> item) = 0 ;
	virtual void read (VR<BYTE> item) = 0 ;
	virtual void read (VR<WORD> item) = 0 ;
	virtual void read (VR<CHAR> item) = 0 ;
	virtual void read (VR<QUAD> item) = 0 ;
} ;

class PlyParser implement PlyParserLayout {
protected:
	using PlyParserLayout::mThis ;
	using PlyParserLayout::mGuide ;

public:
	implicit PlyParser () = default ;

	explicit PlyParser (CR<RefBuffer<BYTE>> stream) {
		PlyParserHolder::hold (thiz)->initialize (stream) ;
	}

	LENGTH element_size (CR<Slice> element) const {
		return PlyParserHolder::hold (thiz)->element_size (element) ;
	}

	LENGTH property_size (CR<Slice> element ,CR<Slice> property) const {
		return PlyParserHolder::hold (thiz)->property_size (element ,property) ;
	}

	void guide_new (CR<Slice> element) {
		return PlyParserHolder::hold (thiz)->guide_new (element) ;
	}

	void guide_put (CR<Slice> property) {
		return PlyParserHolder::hold (thiz)->guide_put (property) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<BOOL> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<VAL32> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<VAL64> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<FLT32> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<FLT64> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<BYTE> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<WORD> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<CHAR> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<QUAD> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<QUAD> item) {
		read (item) ;
		return thiz ;
	}
} ;
} ;