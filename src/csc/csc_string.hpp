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
	imports CR<Super<Ref<StringProcLayout>>> expr_m () ;
	imports VFat<StringProcHolder> hold (VR<StringProcLayout> that) ;
	imports CFat<StringProcHolder> hold (CR<StringProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual String<Stra> stra_from_strw (CR<String<Strw>> a) const = 0 ;
	virtual String<Strw> strw_from_stra (CR<String<Stra>> a) const = 0 ;
	virtual String<Stru> stru8_from_stru16 (CR<String<Stru16>> a) const = 0 ;
	virtual String<Stru> stru8_from_stru32 (CR<String<Stru32>> a) const = 0 ;
	virtual String<Stru16> stru16_from_stru8 (CR<String<Stru>> a) const = 0 ;
	virtual String<Stru16> stru16_from_stru32 (CR<String<Stru32>> a) const = 0 ;
	virtual String<Stru32> stru32_from_stru8 (CR<String<Stru>> a) const = 0 ;
	virtual String<Stru32> stru32_from_stru16 (CR<String<Stru16>> a) const = 0 ;
	virtual String<Stra> stra_from (CR<String<Stra>> a) const = 0 ;
	virtual String<Stra> stra_from (CR<String<Strw>> a) const = 0 ;
	virtual String<Stra> stra_from (CR<String<Stru>> a) const = 0 ;
	virtual String<Stra> stra_from (CR<String<Stru16>> a) const = 0 ;
	virtual String<Stra> stra_from (CR<String<Stru32>> a) const = 0 ;
	virtual String<Strw> strw_from (CR<String<Stra>> a) const = 0 ;
	virtual String<Strw> strw_from (CR<String<Strw>> a) const = 0 ;
	virtual String<Strw> strw_from (CR<String<Stru>> a) const = 0 ;
	virtual String<Strw> strw_from (CR<String<Stru16>> a) const = 0 ;
	virtual String<Strw> strw_from (CR<String<Stru32>> a) const = 0 ;
	virtual String<Str> strs_from (CR<String<Stra>> a) const = 0 ;
	virtual String<Str> strs_from (CR<String<Strw>> a) const = 0 ;
	virtual String<Str> strs_from (CR<String<Stru>> a) const = 0 ;
	virtual String<Str> strs_from (CR<String<Stru16>> a) const = 0 ;
	virtual String<Str> strs_from (CR<String<Stru32>> a) const = 0 ;
	virtual String<Stru> stru8_from (CR<String<Stra>> a) const = 0 ;
	virtual String<Stru> stru8_from (CR<String<Strw>> a) const = 0 ;
	virtual String<Stru> stru8_from (CR<String<Stru>> a) const = 0 ;
	virtual String<Stru> stru8_from (CR<String<Stru16>> a) const = 0 ;
	virtual String<Stru> stru8_from (CR<String<Stru32>> a) const = 0 ;
	virtual String<Stru16> stru16_from (CR<String<Stra>> a) const = 0 ;
	virtual String<Stru16> stru16_from (CR<String<Strw>> a) const = 0 ;
	virtual String<Stru16> stru16_from (CR<String<Stru>> a) const = 0 ;
	virtual String<Stru16> stru16_from (CR<String<Stru16>> a) const = 0 ;
	virtual String<Stru16> stru16_from (CR<String<Stru32>> a) const = 0 ;
	virtual String<Stru32> stru32_from (CR<String<Stra>> a) const = 0 ;
	virtual String<Stru32> stru32_from (CR<String<Strw>> a) const = 0 ;
	virtual String<Stru32> stru32_from (CR<String<Stru>> a) const = 0 ;
	virtual String<Stru32> stru32_from (CR<String<Stru16>> a) const = 0 ;
	virtual String<Stru32> stru32_from (CR<String<Stru32>> a) const = 0 ;
} ;

class StringProc implement Super<Ref<StringProcLayout>> {
public:
	static CR<StringProc> expr_m () {
		return keep[TYPE<StringProc>::expr] (StringProcHolder::expr) ;
	}

	static String<Stra> stra_from_strw (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->stra_from_strw (a) ;
	}

	static String<Strw> strw_from_stra (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->strw_from_stra (a) ;
	}

	static String<Stru> stru8_from_stru16 (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->stru8_from_stru16 (a) ;
	}

	static String<Stru> stru8_from_stru32 (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->stru8_from_stru32 (a) ;
	}

	static String<Stru16> stru16_from_stru8 (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->stru16_from_stru8 (a) ;
	}

	static String<Stru16> stru16_from_stru32 (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->stru16_from_stru32 (a) ;
	}

	static String<Stru32> stru32_from_stru8 (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->stru32_from_stru8 (a) ;
	}

	static String<Stru32> stru32_from_stru16 (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->stru32_from_stru16 (a) ;
	}

	static String<Stra> stra_from (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->stra_from (a) ;
	}

	static String<Stra> stra_from (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->stra_from (a) ;
	}

	static String<Stra> stra_from (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->stra_from (a) ;
	}

	static String<Stra> stra_from (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->stra_from (a) ;
	}

	static String<Stra> stra_from (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->stra_from (a) ;
	}

	static String<Strw> strw_from (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->strw_from (a) ;
	}

	static String<Strw> strw_from (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->strw_from (a) ;
	}

	static String<Strw> strw_from (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->strw_from (a) ;
	}

	static String<Strw> strw_from (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->strw_from (a) ;
	}

	static String<Strw> strw_from (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->strw_from (a) ;
	}

	static String<Str> strs_from (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->strs_from (a) ;
	}

	static String<Str> strs_from (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->strs_from (a) ;
	}

	static String<Str> strs_from (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->strs_from (a) ;
	}

	static String<Str> strs_from (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->strs_from (a) ;
	}

	static String<Str> strs_from (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->strs_from (a) ;
	}

	static String<Stru> stru8_from (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->stru8_from (a) ;
	}

	static String<Stru> stru8_from (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->stru8_from (a) ;
	}

	static String<Stru> stru8_from (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->stru8_from (a) ;
	}

	static String<Stru> stru8_from (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->stru8_from (a) ;
	}

	static String<Stru> stru8_from (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->stru8_from (a) ;
	}

	static String<Stru16> stru16_from (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->stru16_from (a) ;
	}

	static String<Stru16> stru16_from (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->stru16_from (a) ;
	}

	static String<Stru16> stru16_from (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->stru16_from (a) ;
	}

	static String<Stru16> stru16_from (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->stru16_from (a) ;
	}

	static String<Stru16> stru16_from (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->stru16_from (a) ;
	}

	static String<Stru32> stru32_from (CR<String<Stra>> a) {
		return StringProcHolder::hold (expr)->stru32_from (a) ;
	}

	static String<Stru32> stru32_from (CR<String<Strw>> a) {
		return StringProcHolder::hold (expr)->stru32_from (a) ;
	}

	static String<Stru32> stru32_from (CR<String<Stru>> a) {
		return StringProcHolder::hold (expr)->stru32_from (a) ;
	}

	static String<Stru32> stru32_from (CR<String<Stru16>> a) {
		return StringProcHolder::hold (expr)->stru32_from (a) ;
	}

	static String<Stru32> stru32_from (CR<String<Stru32>> a) {
		return StringProcHolder::hold (expr)->stru32_from (a) ;
	}
} ;

struct XmlParserTree ;

struct XmlParserLayout {
	UniqueRef<XmlParserTree> mThis ;
	Index mIndex ;
} ;

struct XmlParserHolder implement Interface {
	imports VFat<XmlParserHolder> hold (VR<XmlParserLayout> that) ;
	imports CFat<XmlParserHolder> hold (CR<XmlParserLayout> that) ;

	virtual void initialize (CR<RefBuffer<Byte>> stream) = 0 ;
	virtual Bool exist () const = 0 ;
	virtual XmlParserLayout root () const = 0 ;
	virtual XmlParserLayout parent () const = 0 ;
	virtual XmlParserLayout brother () const = 0 ;
	virtual XmlParserLayout child () const = 0 ;
	virtual XmlParserLayout child (CR<Index> index) const = 0 ;
	virtual XmlParserLayout child (CR<Slice> name) const = 0 ;
	virtual XmlParserLayout child (CR<String<Stru>> name) const = 0 ;
	virtual Array<XmlParserLayout> list () const = 0 ;
	virtual Array<XmlParserLayout> list (CR<Length> size_) const = 0 ;
	virtual Bool equal (CR<XmlParserLayout> that) const = 0 ;
	virtual CR<String<Stru>> name () const leftvalue = 0 ;
	virtual Bool parse (CR<Bool> def) const = 0 ;
	virtual Val32 parse (CR<Val32> def) const = 0 ;
	virtual Val64 parse (CR<Val64> def) const = 0 ;
	virtual Flt32 parse (CR<Flt32> def) const = 0 ;
	virtual Flt64 parse (CR<Flt64> def) const = 0 ;
	virtual String<Stra> parse (CR<String<Stra>> def) const = 0 ;
	virtual String<Strw> parse (CR<String<Strw>> def) const = 0 ;
	virtual String<Stru> parse (CR<String<Stru>> def) const = 0 ;
	virtual String<Stru16> parse (CR<String<Stru16>> def) const = 0 ;
	virtual String<Stru32> parse (CR<String<Stru32>> def) const = 0 ;
	virtual Array<Bool> parse (CR<Bool> def ,CR<Length> size_) const = 0 ;
	virtual Array<Val32> parse (CR<Val32> def ,CR<Length> size_) const = 0 ;
	virtual Array<Val64> parse (CR<Val64> def ,CR<Length> size_) const = 0 ;
	virtual Array<Flt32> parse (CR<Flt32> def ,CR<Length> size_) const = 0 ;
	virtual Array<Flt64> parse (CR<Flt64> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stra>> parse (CR<String<Stra>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Strw>> parse (CR<String<Strw>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stru>> parse (CR<String<Stru>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stru16>> parse (CR<String<Stru16>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stru32>> parse (CR<String<Stru32>> def ,CR<Length> size_) const = 0 ;
} ;

class XmlParser implement XmlParserLayout {
protected:
	using XmlParserLayout::mThis ;
	using XmlParserLayout::mIndex ;

public:
	implicit XmlParser () = default ;

	explicit XmlParser (CR<RefBuffer<Byte>> stream) {
		XmlParserHolder::hold (thiz)->initialize (stream) ;
	}

	Bool exist () const {
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

	XmlParser child (CR<Index> index) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (index) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CR<Slice> name) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CR<String<Stru>> name) const {
		XmlParserLayout ret = XmlParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	Array<XmlParser> list () const {
		ArrayLayout ret = XmlParserHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (Pointer::from (ret))) ;
	}

	Array<XmlParser> list (CR<Length> size_) const {
		ArrayLayout ret = XmlParserHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (Pointer::from (ret))) ;
	}

	Bool equal (CR<XmlParser> that) const {
		return XmlParserHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<XmlParser> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<XmlParser> that) const {
		return (!equal (that)) ;
	}

	CR<String<Stru>> name () const leftvalue {
		return XmlParserHolder::hold (thiz)->name () ;
	}

	Bool parse (CR<Bool> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Val32 parse (CR<Val32> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Val64 parse (CR<Val64> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Flt32 parse (CR<Flt32> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Flt64 parse (CR<Flt64> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stra> parse (CR<String<Stra>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<Strw> parse (CR<String<Strw>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stru> parse (CR<String<Stru>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stru16> parse (CR<String<Stru16>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stru32> parse (CR<String<Stru32>> def) const {
		return XmlParserHolder::hold (thiz)->parse (def) ;
	}

	Array<Bool> parse (CR<Bool> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Val32> parse (CR<Val32> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Val64> parse (CR<Val64> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Flt32> parse (CR<Flt32> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Flt64> parse (CR<Flt64> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stra>> parse (CR<String<Stra>> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Strw>> parse (CR<String<Strw>> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stru>> parse (CR<String<Stru>> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stru16>> parse (CR<String<Stru16>> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stru32>> parse (CR<String<Stru32>> def ,CR<Length> size_) const {
		return XmlParserHolder::hold (thiz)->parse (def ,size_) ;
	}
} ;

struct JsonParserTree ;

struct JsonParserLayout {
	UniqueRef<JsonParserTree> mThis ;
	Index mIndex ;
} ;

struct JsonParserHolder implement Interface {
	imports VFat<JsonParserHolder> hold (VR<JsonParserLayout> that) ;
	imports CFat<JsonParserHolder> hold (CR<JsonParserLayout> that) ;

	virtual void initialize (CR<RefBuffer<Byte>> stream) = 0 ;
	virtual Bool exist () const = 0 ;
	virtual JsonParserLayout root () const = 0 ;
	virtual JsonParserLayout parent () const = 0 ;
	virtual JsonParserLayout brother () const = 0 ;
	virtual JsonParserLayout child () const = 0 ;
	virtual JsonParserLayout child (CR<Index> index) const = 0 ;
	virtual JsonParserLayout child (CR<Slice> name) const = 0 ;
	virtual JsonParserLayout child (CR<String<Stru>> name) const = 0 ;
	virtual Array<JsonParserLayout> list () const = 0 ;
	virtual Array<JsonParserLayout> list (CR<Length> size_) const = 0 ;
	virtual Bool equal (CR<JsonParserLayout> that) const = 0 ;
	virtual CR<String<Stru>> name () const leftvalue = 0 ;
	virtual Bool parse (CR<Bool> def) const = 0 ;
	virtual Val32 parse (CR<Val32> def) const = 0 ;
	virtual Val64 parse (CR<Val64> def) const = 0 ;
	virtual Flt32 parse (CR<Flt32> def) const = 0 ;
	virtual Flt64 parse (CR<Flt64> def) const = 0 ;
	virtual String<Stra> parse (CR<String<Stra>> def) const = 0 ;
	virtual String<Strw> parse (CR<String<Strw>> def) const = 0 ;
	virtual String<Stru> parse (CR<String<Stru>> def) const = 0 ;
	virtual String<Stru16> parse (CR<String<Stru16>> def) const = 0 ;
	virtual String<Stru32> parse (CR<String<Stru32>> def) const = 0 ;
	virtual Array<Bool> parse (CR<Bool> def ,CR<Length> size_) const = 0 ;
	virtual Array<Val32> parse (CR<Val32> def ,CR<Length> size_) const = 0 ;
	virtual Array<Val64> parse (CR<Val64> def ,CR<Length> size_) const = 0 ;
	virtual Array<Flt32> parse (CR<Flt32> def ,CR<Length> size_) const = 0 ;
	virtual Array<Flt64> parse (CR<Flt64> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stra>> parse (CR<String<Stra>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Strw>> parse (CR<String<Strw>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stru>> parse (CR<String<Stru>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stru16>> parse (CR<String<Stru16>> def ,CR<Length> size_) const = 0 ;
	virtual Array<String<Stru32>> parse (CR<String<Stru32>> def ,CR<Length> size_) const = 0 ;
} ;

class JsonParser implement JsonParserLayout {
protected:
	using JsonParserLayout::mThis ;
	using JsonParserLayout::mIndex ;

public:
	implicit JsonParser () = default ;

	explicit JsonParser (CR<RefBuffer<Byte>> stream) {
		JsonParserHolder::hold (thiz)->initialize (stream) ;
	}

	Bool exist () const {
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

	JsonParser child (CR<Index> index) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (index) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CR<Slice> name) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CR<String<Stru>> name) const {
		JsonParserLayout ret = JsonParserHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	Array<JsonParser> list () const {
		ArrayLayout ret = JsonParserHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (Pointer::from (ret))) ;
	}

	Array<JsonParser> list (CR<Length> size_) const {
		ArrayLayout ret = JsonParserHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (Pointer::from (ret))) ;
	}

	Bool equal (CR<JsonParser> that) const {
		return JsonParserHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<JsonParser> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<JsonParser> that) const {
		return (!equal (that)) ;
	}

	CR<String<Stru>> name () const leftvalue {
		return JsonParserHolder::hold (thiz)->name () ;
	}

	Bool parse (CR<Bool> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Val32 parse (CR<Val32> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Val64 parse (CR<Val64> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Flt32 parse (CR<Flt32> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Flt64 parse (CR<Flt64> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stra> parse (CR<String<Stra>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<Strw> parse (CR<String<Strw>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stru> parse (CR<String<Stru>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stru16> parse (CR<String<Stru16>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	String<Stru32> parse (CR<String<Stru32>> def) const {
		return JsonParserHolder::hold (thiz)->parse (def) ;
	}

	Array<Bool> parse (CR<Bool> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Val32> parse (CR<Val32> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Val64> parse (CR<Val64> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Flt32> parse (CR<Flt32> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<Flt64> parse (CR<Flt64> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stra>> parse (CR<String<Stra>> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Strw>> parse (CR<String<Strw>> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stru>> parse (CR<String<Stru>> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stru16>> parse (CR<String<Stru16>> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}

	Array<String<Stru32>> parse (CR<String<Stru32>> def ,CR<Length> size_) const {
		return JsonParserHolder::hold (thiz)->parse (def ,size_) ;
	}
} ;

struct PlyParserTree ;

struct PlyParserGuide {
	Index mElement ;
	Deque<Index> mProperty ;
	Index mCol ;
	Index mRow ;
	Index mPlyBegin ;
	Index mPlyEnd ;
	Index mPlyIndex ;
	Bool mPlyListMode ;
} ;

struct PlyParserLayout {
	UniqueRef<PlyParserTree> mThis ;
	PlyParserGuide mGuide ;
} ;

struct PlyParserHolder implement Interface {
	imports VFat<PlyParserHolder> hold (VR<PlyParserLayout> that) ;
	imports CFat<PlyParserHolder> hold (CR<PlyParserLayout> that) ;

	virtual void initialize (CR<RefBuffer<Byte>> stream) = 0 ;
	virtual Length element_size (CR<Slice> element) const = 0 ;
	virtual Length property_size (CR<Slice> element ,CR<Slice> property) const = 0 ;
	virtual void guide_new (CR<Slice> element) = 0 ;
	virtual void guide_put (CR<Slice> property) = 0 ;
	virtual void read (VR<Bool> item) = 0 ;
	virtual void read (VR<Val32> item) = 0 ;
	virtual void read (VR<Val64> item) = 0 ;
	virtual void read (VR<Flt32> item) = 0 ;
	virtual void read (VR<Flt64> item) = 0 ;
	virtual void read (VR<Byte> item) = 0 ;
	virtual void read (VR<Word> item) = 0 ;
	virtual void read (VR<Char> item) = 0 ;
	virtual void read (VR<Quad> item) = 0 ;
} ;

class PlyParser implement PlyParserLayout {
protected:
	using PlyParserLayout::mThis ;
	using PlyParserLayout::mGuide ;

public:
	implicit PlyParser () = default ;

	explicit PlyParser (CR<RefBuffer<Byte>> stream) {
		PlyParserHolder::hold (thiz)->initialize (stream) ;
	}

	Length element_size (CR<Slice> element) const {
		return PlyParserHolder::hold (thiz)->element_size (element) ;
	}

	Length property_size (CR<Slice> element ,CR<Slice> property) const {
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

	void read (VR<Bool> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Bool> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val32> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Val32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val64> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Val64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt32> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Flt32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt64> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Flt64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Byte> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Byte> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Word> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Word> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Char> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Char> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Quad> item) {
		return PlyParserHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<PlyParser> operator>> (VR<Quad> item) {
		read (item) ;
		return thiz ;
	}
} ;
} ;