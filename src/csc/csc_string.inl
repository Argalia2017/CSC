﻿#pragma once

#ifndef __CSC_STRING__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_string.hpp"

namespace CSC {
struct CommaImplLayout {
	Slice mIndent ;
	Slice mComma ;
	Slice mEndline ;
	LENGTH mDepth ;
	Deque<BOOL> mFirst ;
	LENGTH mTight ;
	LENGTH mLastTight ;
} ;

class CommaImplHolder implement Fat<CommaHolder ,CommaLayout> {
public:
	void initialize (CREF<Slice> indent ,CREF<Slice> comma ,CREF<Slice> endline) override {
		fake.mThis = SharedRef<CommaImplLayout>::make () ;
		fake.mThis->mIndent = indent ;
		fake.mThis->mComma = comma ;
		fake.mThis->mEndline = endline ;
		fake.mThis->mDepth = 0 ;
		fake.mThis->mTight = 255 ;
		fake.mThis->mLastTight = 0 ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		if ifdo (TRUE) {
			if (fake.mThis->mDepth >= fake.mThis->mTight + fake.mThis->mLastTight)
				discard ;
			writer.write (fake.mThis->mEndline) ;
		}
		if ifdo (TRUE) {
			if (fake.mThis->mFirst.empty ())
				discard ;
			INDEX ix = fake.mThis->mFirst.tail () ;
			if ifdo (TRUE) {
				if (fake.mThis->mFirst[ix])
					discard ;
				writer.write (fake.mThis->mComma) ;
			}
			fake.mThis->mFirst[ix] = FALSE ;
		}
		if ifdo (TRUE) {
			if (fake.mThis->mDepth >= fake.mThis->mTight + fake.mThis->mLastTight)
				discard ;
			for (auto &&i : iter (0 ,fake.mThis->mDepth)) {
				noop (i) ;
				writer.write (fake.mThis->mIndent) ;
			}
		}
		fake.mThis->mLastTight = 0 ;
	}

	void increase () const override {
		fake.mThis->mDepth++ ;
		if ifdo (TRUE) {
			if (fake.mThis->mFirst.empty ())
				discard ;
			fake.mThis->mFirst[fake.mThis->mFirst.tail ()] = TRUE ;
		}
		fake.mThis->mFirst.add (TRUE) ;
	}

	void decrease () const override {
		fake.mThis->mFirst.pop () ;
		fake.mThis->mDepth-- ;
		fake.mThis->mLastTight = fake.mThis->mTight - 256 ;
		fake.mThis->mTight = 255 ;
	}

	void tight () const override {
		fake.mThis->mTight = inline_min (fake.mThis->mTight ,fake.mThis->mDepth) ;
	}
} ;

exports VFat<CommaHolder> CommaHolder::create (VREF<CommaLayout> that) {
	return VFat<CommaHolder> (CommaImplHolder () ,that) ;
}

exports CFat<CommaHolder> CommaHolder::create (CREF<CommaLayout> that) {
	return CFat<CommaHolder> (CommaImplHolder () ,that) ;
}

struct RegularReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	INDEX mRead ;
	INDEX mWrite ;
	Box<ByteReader> mByteReader ;
	Box<TextReader> mTextReader ;
	Deque<STRU32> mDeque ;
	STRU32 mTop ;
} ;

class RegularReader implement RegularReaderLayout {
protected:
	using RegularReaderLayout::mStream ;
	using RegularReaderLayout::mRead ;
	using RegularReaderLayout::mWrite ;
	using RegularReaderLayout::mByteReader ;
	using RegularReaderLayout::mTextReader ;
	using RegularReaderLayout::mDeque ;
	using RegularReaderLayout::mTop ;

public:
	implicit RegularReader () = default ;

	explicit RegularReader (RREF<Ref<RefBuffer<BYTE>>> stream ,CREF<LENGTH> ring_size) {
		mStream = move (stream) ;
		mRead = 0 ;
		mWrite = mStream->size () ;
		mDeque = Deque<STRU32> (ring_size) ;
	}

	void use_byte () {
		mTextReader = NULL ;
		mByteReader = Box<ByteReader>::make (mStream.share ()) ;
		mByteReader->reset (mRead ,mWrite) ;
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.length () >= mDeque.size ())
				break ;
			mByteReader.self >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	void use_text () {
		mByteReader = NULL ;
		mTextReader = Box<TextReader>::make (mStream.share ()) ;
		mTextReader->reset (mRead ,mWrite) ;
		mDeque.clear () ;
		while (TRUE) {
			if (mDeque.length () >= mDeque.size ())
				break ;
			mTextReader.self >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	STRU32 get (CREF<INDEX> index) const {
		return mDeque[index] ;
	}

	forceinline STRU32 operator[] (CREF<INDEX> index) const {
		return get (index) ;
	}

	template <class ARG1>
	void read (XREF<ARG1> item) {
		auto act = TRUE ;
		if ifdo (act) {
			if (mByteReader == NULL)
				discard ;
			mByteReader->reset (mRead ,mWrite) ;
			mByteReader.self >> item ;
			mRead = mByteReader->length () ;
			mDeque.clear () ;
			while (TRUE) {
				if (mDeque.length () >= mDeque.size ())
					break ;
				mByteReader.self >> mTop ;
				mDeque.add (mTop) ;
			}
		}
		if ifdo (act) {
			if (mTextReader == NULL)
				discard ;
			mTextReader->reset (mRead ,mWrite) ;
			mTextReader.self >> item ;
			mRead = mTextReader->length () ;
			mDeque.clear () ;
			while (TRUE) {
				if (mDeque.length () >= mDeque.size ())
					break ;
				mTextReader.self >> mTop ;
				mDeque.add (mTop) ;
			}
		}
	}

	template <class ARG1>
	forceinline VREF<RegularReader> operator>> (XREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}

	void next () {
		auto act = TRUE ;
		if ifdo (act) {
			if (mByteReader == NULL)
				discard ;
			mByteReader.self >> mTop ;
			mRead++ ;
			assume (mRead <= mByteReader->size ()) ;
		}
		if ifdo (act) {
			if (mTextReader == NULL)
				discard ;
			mTextReader.self >> mTop ;
			mRead++ ;
			assume (mRead <= mTextReader->size ()) ;
		}
		mDeque.take () ;
		mDeque.add (mTop) ;
	}

	forceinline void operator++ (int) {
		next () ;
	}
} ;

class ScopeCounter implement Proxy {
private:
	using COUNTER_MAX_DEPTH = ENUM<256> ;

protected:
	Pin<LENGTH> mThat ;

public:
	imports CREF<ScopeCounter> from (VREF<LENGTH> that) {
		return Pointer::from (that) ;
	}

	void enter () const {
		mThat.self++ ;
		assume (mThat.self < COUNTER_MAX_DEPTH::expr) ;
	}

	void leave () const {
		mThat.self-- ;
	}
} ;

struct XmlParserNodeType {
	enum {
		Value ,
		Array ,
		Object ,
		ETC
	} ;
} ;

struct XmlParserNode {
	String<STRU8> mName ;
	String<STRU8> mValue ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	Just<XmlParserNodeType> mType ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct XmlParserImplLayout {
	Array<XmlParserNode> mTree ;
	INDEX mRoot ;
} ;

struct MakeXmlParserLayout {
	RegularReader mReader ;
	LENGTH mRecursiveCounter ;
	List<XmlParserNode> mTree ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	INDEX mLastIndex ;
	String<STRU8> mLastString ;
} ;

class MakeXmlParser implement MakeXmlParserLayout {
protected:
	using MakeXmlParserLayout::mReader ;
	using MakeXmlParserLayout::mRecursiveCounter ;
	using MakeXmlParserLayout::mTree ;
	using MakeXmlParserLayout::mArrayMap ;
	using MakeXmlParserLayout::mObjectMap ;
	using MakeXmlParserLayout::mLastIndex ;
	using MakeXmlParserLayout::mLastString ;

public:
	implicit MakeXmlParser () = default ;

	explicit MakeXmlParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
		mRecursiveCounter = 0 ;
		mArrayMap = SortedMap<INDEX> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<STRU8>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	XmlParserImplLayout poll () {
		XmlParserImplLayout ret ;
		ret.mTree = Array<XmlParserNode> (mTree.length ()) ;
		const auto r1x = Array<INDEX>::make (mTree.range ()) ;
		for (auto &&i : ret.mTree.range ()) {
			ret.mTree[i] = move (mTree[r1x[i]]) ;
			ret.mTree[i].mArrayMap.remap () ;
			ret.mTree[i].mObjectMap.remap () ;
		}
		ret.mRoot = NONE ;
		if ifdo (TRUE) {
			if (r1x.length () == 0)
				discard ;
			ret.mRoot = 0 ;
		}
		return move (ret) ;
	}

	void generate () {
		//@info: $0->$10 $9 $11
		//@info: $1->${identity}
		//@info: $2->"${string}"
		//@info: $3->$1 = $2
		//@info: $4->${eps}|$3 $4
		//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
		//@info: $6-><!--comment-->
		//@info: $7->${[^<>]+}
		//@info: $8->$5 $8|$6 $8|$7 $8
		//@info: $9->${eps}|$5
		//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		//@info: $11->${end}
		read_shift_e0 () ;
		assume (mLastIndex == 0) ;
	}

	void read_shift_e0 () {
		read_shift_e10 () ;
		mReader >> GAP ;
		read_shift_e9 () ;
		mReader >> GAP ;
		read_shift_e11 () ;
	}

	//@info: $1->${identity}
	void read_shift_e1 () {
		mReader >> IdentifierText::from (mLastString) ;
	}

	//@info: $2->"${string}"
	void read_shift_e2 () {
		mReader >> EscapeText::from (mLastString) ;
	}

	//@info: $3->$1 = $2
	void read_shift_e3 (CREF<INDEX> curr) {
		INDEX ix = mTree.insert () ;
		read_shift_e1 () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mType = XmlParserNodeType::Value ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		read_shift_e2 () ;
		mTree[ix].mValue = move (mLastString) ;
		mLastIndex = ix ;
	}

	//@info: $4->${eps}|$3 $4
	void read_shift_e4 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			if (!is_frist_identity ())
				break ;
			read_shift_e3 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	BOOL is_frist_identity () const {
		if (mReader[0] >= STRU32 ('A'))
			if (mReader[0] <= STRU32 ('Z'))
				return TRUE ;
		if (mReader[0] >= STRU32 ('a'))
			if (mReader[0] <= STRU32 ('z'))
				return TRUE ;
		if (mReader[0] == STRU32 ('_'))
			return TRUE ;
		return FALSE ;
	}

	//@info: $5-><$1 $4 />|<$1 $4 > $8 </$1 >
	void read_shift_e5 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		mReader >> slice ("<") ;
		INDEX ix = mTree.insert () ;
		read_shift_e1 () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mType = XmlParserNodeType::Value ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		read_shift_e4 (ix) ;
		INDEX iy = mLastIndex ;
		mReader >> GAP ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != STRU32 ('>'))
				discard ;
			mReader++ ;
			mReader >> GAP ;
			mTree[ix].mArrayMap = mArrayMap.share () ;
			mTree[ix].mObjectMap = mObjectMap.share () ;
			read_shift_e8 (ix ,iy) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
			mReader >> slice ("</") ;
			read_shift_e1 () ;
			assume (mTree[ix].mName == mLastString) ;
			mReader >> GAP ;
			mReader >> slice (">") ;
		}
		if ifdo (act) {
			mReader >> slice ("/>") ;
		}
		mLastIndex = ix ;
	}

	//@info: $6-><!--${comment}-->
	void read_shift_e6 () {
		mReader >> slice ("<!--") ;
		while (TRUE) {
			if (mReader[0] == STRU32 ('-'))
				if (mReader[1] == STRU32 ('-'))
					if (mReader[2] == STRU32 ('>'))
						break ;
			mReader++ ;
		}
		mReader >> slice ("-->") ;
	}

	//@info: $7->${[^<>]+}
	void read_shift_e7 (CREF<INDEX> curr) {
		auto rax = String<STRU8>::make () ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (mReader[0] == STRU32 ('<'))
				break ;
			if (mReader[0] == STRU32 ('>'))
				break ;
			assume (ix < rax.size ()) ;
			rax[ix] = STRU8 (mReader[0]) ;
			ix++ ;
			mReader++ ;
		}
		while (TRUE) {
			ix-- ;
			if (ix < 0)
				break ;
			if (!StreamProc::is_blank (rax[ix]))
				break ;
		}
		ix++ ;
		rax.trunc (ix) ;
		if ifdo (TRUE) {
			if (mTree[curr].mValue.size () > 0)
				discard ;
			mTree[curr].mValue = move (rax) ;
		}
		mTree[curr].mValue.splice (mTree[curr].mValue.length () ,rax) ;
	}

	//@info: $8->$5 $8|$6 $8|$7 $8
	void read_shift_e8 (CREF<INDEX> curr ,CREF<INDEX> first) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		INDEX ix = first ;
		INDEX iy = first ;
		while (TRUE) {
			auto act = TRUE ;
			if ifdo (TRUE) {
				if (!is_first_of_object ())
					discard ;
				read_shift_e5 (curr) ;
				if ifdo (act) {
					if (ix != NONE)
						discard ;
					ix = mLastIndex ;
					iy = mLastIndex ;
				}
				if ifdo (act) {
					mTree[iy].mBrother = mLastIndex ;
					iy = mLastIndex ;
				}
				const auto r1x = mTree[curr].mArrayMap.length () ;
				mTree[curr].mArrayMap.add (r1x ,iy) ;
				mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			}
			if ifdo (act) {
				if (!is_first_of_comment ())
					discard ;
				read_shift_e6 () ;
			}
			mReader >> GAP ;
			if (mReader[0] == STRU32 ('<'))
				if (mReader[1] == STRU32 ('/'))
					break ;
			if ifdo (act) {
				read_shift_e7 (curr) ;
			}
		}
		mLastIndex = ix ;
	}

	BOOL is_first_of_comment () const {
		if (mReader[0] == STRU32 ('<'))
			if (mReader[1] == STRU32 ('!'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_first_of_object () const {
		if (mReader[0] == STRU32 ('<'))
			if (mReader[1] != STRU32 ('/'))
				return TRUE ;
		return FALSE ;
	}

	//@info: $9->${eps}|$5
	void read_shift_e9 () {
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (0X00))
				discard ;
			read_shift_e5 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
	void read_shift_e10 () {
		mReader >> BOM ;
		if (mReader[0] != STRU32 ('<'))
			return ;
		if (mReader[1] != STRU32 ('?'))
			return ;
		mReader++ ;
		mReader++ ;
		mReader >> slice ("xml") ;
		mReader >> GAP ;
		mReader >> slice ("version") ;
		mReader >> GAP ;
		mReader >> slice ("=") ;
		mReader >> GAP ;
		mReader >> slice ("\"1.0\"") ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 ('?'))
				discard ;
			mReader >> slice ("encoding") ;
			mReader >> GAP ;
			mReader >> slice ("=") ;
			mReader >> GAP ;
			mReader >> slice ("\"utf-8\"") ;
			mReader >> GAP ;
		}
		mReader >> slice ("?>") ;
	}

	//@info: $11->${end}
	void read_shift_e11 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class XmlParserImplHolder implement Fat<XmlParserHolder ,XmlParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeXmlParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		fake.mThis = Ref<XmlParserImplLayout>::make (rax.poll ()) ;
		fake.mIndex = fake.mThis->mRoot ;
	}

	void initialize (CREF<XmlParserLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mIndex = that.mIndex ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	XmlParserLayout root () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	XmlParserLayout parent () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	XmlParserLayout brother () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	XmlParserLayout child () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<INDEX> index) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<Slice> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<String<STRU8>> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list () const override {
		Array<XmlParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			ret = Array<XmlParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list (CREF<LENGTH> size_) const override {
		Array<XmlParserLayout> ret = Array<XmlParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<XmlParserLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!fake.mThis.exist ())
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String<STRU8>> name () const leftvalue override {
		assert (exist ()) ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<BOOL>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return fake.mThis->mTree[fake.mIndex].mValue.clone () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	template <class ARG1>
	Array<ARG1> fetch_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.range ()) {
			const auto r2x = XmlParserHolder::create (fake)->child (i) ;
			ret[i] = XmlParserHolder::create (r2x)->fetch (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<XmlParserHolder> XmlParserHolder::create (VREF<XmlParserLayout> that) {
	return VFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

exports CFat<XmlParserHolder> XmlParserHolder::create (CREF<XmlParserLayout> that) {
	return CFat<XmlParserHolder> (XmlParserImplHolder () ,that) ;
}

struct JsonParserNodeType {
	enum {
		Value ,
		Array ,
		Object ,
		ETC
	} ;
} ;

struct JsonParserNode {
	String<STRU8> mName ;
	String<STRU8> mValue ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	Just<JsonParserNodeType> mType ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct JsonParserImplLayout {
	Array<JsonParserNode> mTree ;
	INDEX mRoot ;
} ;

struct MakeJsonParserLayout {
	RegularReader mReader ;
	LENGTH mRecursiveCounter ;
	List<JsonParserNode> mTree ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	INDEX mLastIndex ;
	String<STRU8> mLastString ;
} ;

class MakeJsonParser implement MakeJsonParserLayout {
protected:
	using MakeJsonParserLayout::mReader ;
	using MakeJsonParserLayout::mRecursiveCounter ;
	using MakeJsonParserLayout::mTree ;
	using MakeJsonParserLayout::mArrayMap ;
	using MakeJsonParserLayout::mObjectMap ;
	using MakeJsonParserLayout::mLastIndex ;
	using MakeJsonParserLayout::mLastString ;

public:
	implicit MakeJsonParser () = default ;

	explicit MakeJsonParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
		mRecursiveCounter = 0 ;
		mArrayMap = SortedMap<INDEX> (ALLOCATOR_MIN_SIZE::expr) ;
		mObjectMap = SortedMap<String<STRU8>> (ALLOCATOR_MIN_SIZE::expr) ;
	}

	JsonParserImplLayout poll () {
		JsonParserImplLayout ret ;
		ret.mTree = Array<JsonParserNode> (mTree.length ()) ;
		const auto r1x = Array<INDEX>::make (mTree.range ()) ;
		for (auto &&i : ret.mTree.range ()) {
			ret.mTree[i] = move (mTree[r1x[i]]) ;
			ret.mTree[i].mArrayMap.remap () ;
			ret.mTree[i].mObjectMap.remap () ;
		}
		ret.mRoot = NONE ;
		if ifdo (TRUE) {
			if (r1x.length () == 0)
				discard ;
			ret.mRoot = 0 ;
		}
		return move (ret) ;
	}

	void generate () {
		//@info: $0->$11 $10 $12
		//@info: $1->${scalar}
		//@info: $2->true|false|null
		//@info: $3->"${string}"
		//@info: $4->$1|$2|$3|$6|$9
		//@info: $5->$4|$4 , $5
		//@info: $6->[ ]|[ $5 ]
		//@info: $7->$3 : $4
		//@info: $8->$7|$7 , $8
		//@info: $9->{ }|{ $8 }
		//@info: $10->${eps}|$4
		//@info: $11->${eps}
		//@info: $12->${end}
		read_shift_e0 () ;
		assert (mLastIndex == 0) ;
	}

	//@info: $0->$11 $10 $12
	void read_shift_e0 () {
		read_shift_e11 () ;
		mReader >> GAP ;
		read_shift_e10 () ;
		mReader >> GAP ;
		read_shift_e12 () ;
	}

	//@info: $1->${scalar}
	void read_shift_e1 () {
		mReader >> ScalarText::from (mLastString) ;
	}

	//@info: $2->true|false|null
	void read_shift_e2 () {
		auto act = TRUE ;
		if ifdo (act) {
			if (mReader[0] != STRU32 ('t'))
				discard ;
			mReader >> slice ("true") ;
			mLastString = String<STRU8>::make (slice ("true")) ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('f'))
				discard ;
			mReader >> slice ("false") ;
			mLastString = String<STRU8>::make (slice ("false")) ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('n'))
				discard ;
			mReader >> slice ("null") ;
			mLastString = String<STRU8> () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	//@info: $3->"${string}"
	void read_shift_e3 () {
		mReader >> EscapeText::from (mLastString) ;
	}

	//@info: $4->$1|$2|$3|$6|$9
	void read_shift_e4 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		INDEX ix = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!is_first_of_number ())
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e1 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (!is_first_of_const ())
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e2 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('\"'))
				discard ;
			ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			read_shift_e3 () ;
			mTree[ix].mValue = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('['))
				discard ;
			read_shift_e6 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('{'))
				discard ;
			read_shift_e9 (curr) ;
			ix = mLastIndex ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
		mLastIndex = ix ;
	}

	BOOL is_first_of_number () const {
		if (mReader[0] == STRU32 ('+'))
			return TRUE ;
		if (mReader[0] == STRU32 ('-'))
			return TRUE ;
		if (mReader[0] >= STRU32 ('0'))
			if (mReader[0] <= STRU32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_first_of_const () const {
		if (mReader[0] == STRU32 ('t'))
			return TRUE ;
		if (mReader[0] == STRU32 ('f'))
			return TRUE ;
		if (mReader[0] == STRU32 ('n'))
			return TRUE ;
		return FALSE ;
	}

	//@info: $5->$4|$4 , $5
	void read_shift_e5 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			read_shift_e4 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != STRU32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $6->[ ]|[ $5 ]
	void read_shift_e6 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		mReader >> slice ("[") ;
		INDEX ix = mTree.insert () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Array ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (']'))
				break ;
			read_shift_e5 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("]") ;
		mLastIndex = ix ;
	}

	//@info: $7->$3 : $4
	void read_shift_e7 (CREF<INDEX> curr) {
		read_shift_e3 () ;
		mReader >> GAP ;
		mReader >> slice (":") ;
		mReader >> GAP ;
		read_shift_e4 (curr) ;
	}

	//@info: $8->$7|$7 , $8
	void read_shift_e8 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			read_shift_e7 (curr) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,iy) ;
			mTree[curr].mObjectMap.add (mTree[iy].mName ,iy) ;
			mReader >> GAP ;
			if (mReader[0] != STRU32 (','))
				break ;
			mReader++ ;
			mReader >> GAP ;
		}
		mLastIndex = ix ;
	}

	//@info: $9->{ }|{ $8 }
	void read_shift_e9 (CREF<INDEX> curr) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		mReader >> slice ("{") ;
		INDEX ix = mTree.insert () ;
		mTree[ix].mName = move (mLastString) ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mObjectMap = mObjectMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Object ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 ('}'))
				discard ;
			read_shift_e8 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mReader >> GAP ;
		}
		mReader >> slice ("}") ;
		mLastIndex = ix ;
	}

	//@info: $10->${eps}|$4
	void read_shift_e10 () {
		INDEX ix = NONE ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (0X00))
				discard ;
			read_shift_e4 (NONE) ;
			ix = mLastIndex ;
		}
		mLastIndex = ix ;
	}

	//@info: $11->${eps}
	void read_shift_e11 () {
		mReader >> BOM ;
	}

	//@info: $12->${end}
	void read_shift_e12 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class JsonParserImplHolder implement Fat<JsonParserHolder ,JsonParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeJsonParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		fake.mThis = Ref<JsonParserImplLayout>::make (rax.poll ()) ;
		fake.mIndex = fake.mThis->mRoot ;
	}

	void initialize (CREF<JsonParserLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mIndex = that.mIndex ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	JsonParserLayout root () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	JsonParserLayout parent () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	JsonParserLayout brother () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	JsonParserLayout child () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<INDEX> index) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<Slice> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<String<STRU8>> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list () const override {
		Array<JsonParserLayout> ret ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			ret = Array<JsonParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list (CREF<LENGTH> size_) const override {
		Array<JsonParserLayout> ret = Array<JsonParserLayout> (size_) ;
		if ifdo (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArrayMap.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (i) ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<JsonParserLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!fake.mThis.exist ())
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String<STRU8>> name () const leftvalue override {
		assert (exist ()) ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<BOOL>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<VAL64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT32>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		if (!exist ())
			return def ;
		try {
			return StringParse<FLT64>::make (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return fake.mThis->mTree[fake.mIndex].mValue.clone () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const override {
		if (!exist ())
			return def.clone () ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mValue) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	template <class ARG1>
	Array<ARG1> fetch_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list () ;
		assume (r1x.size () == size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : ret.range ()) {
			const auto r2x = JsonParserHolder::create (fake)->child (i) ;
			ret[i] = JsonParserHolder::create (r2x)->fetch (def) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<JsonParserHolder> JsonParserHolder::create (VREF<JsonParserLayout> that) {
	return VFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

exports CFat<JsonParserHolder> JsonParserHolder::create (CREF<JsonParserLayout> that) {
	return CFat<JsonParserHolder> (JsonParserImplHolder () ,that) ;
}

struct PlyParserDataType {
	enum {
		Bool ,
		Val32 ,
		Val64 ,
		Flt32 ,
		Flt64 ,
		Byte ,
		Word ,
		Char ,
		Quad ,
		Null ,
		ETC
	} ;
} ;

struct PlyParserProperty {
	String<STRU8> mName ;
	Just<PlyParserDataType> mType ;
	Just<PlyParserDataType> mListType ;
	LENGTH mListSize ;
	INDEX mPlyBegin ;
	INDEX mPlyEnd ;
} ;

struct PlyParserElement {
	String<STRU8> mName ;
	LENGTH mLineSize ;
	LENGTH mLineStep ;
	LENGTH mLineLength ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String<STRU8>> mPropertySet ;
	RefBuffer<BYTE> mPlyBuffer ;
	INDEX mPlyIndex ;
} ;

struct PlyParserImplLayout {
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
} ;

struct MakePlyParserLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	TextReader mTextReader ;
	ByteReader mByteReader ;
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
	BOOL mBitwiseReverseFlag ;
	LENGTH mBodyOffset ;
	INDEX mLastIndex ;
	String<STRU8> mLastType ;
	String<STRU8> mLastString ;
	Set<String<STRU8>> mPropertyType ;
	Set<String<STRU8>> mPropertyListType ;
} ;

class MakePlyParser implement MakePlyParserLayout {
protected:
	using MakePlyParserLayout::mStream ;
	using MakePlyParserLayout::mTextReader ;
	using MakePlyParserLayout::mByteReader ;
	using MakePlyParserLayout::mFormat ;
	using MakePlyParserLayout::mElementList ;
	using MakePlyParserLayout::mElementSet ;
	using MakePlyParserLayout::mBitwiseReverseFlag ;
	using MakePlyParserLayout::mLastIndex ;
	using MakePlyParserLayout::mLastType ;
	using MakePlyParserLayout::mLastString ;
	using MakePlyParserLayout::mPropertyType ;
	using MakePlyParserLayout::mPropertyListType ;

public:
	implicit MakePlyParser () = default ;

	explicit MakePlyParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mStream = move (stream) ;
		mPropertyType.add (slice ("bool") ,PlyParserDataType::Bool) ;
		mPropertyType.add (slice ("int") ,PlyParserDataType::Val32) ;
		mPropertyType.add (slice ("int64") ,PlyParserDataType::Val64) ;
		mPropertyType.add (slice ("float") ,PlyParserDataType::Flt32) ;
		mPropertyType.add (slice ("double") ,PlyParserDataType::Flt64) ;
		mPropertyType.add (slice ("uchar") ,PlyParserDataType::Byte) ;
		mPropertyType.add (slice ("ushort") ,PlyParserDataType::Word) ;
		mPropertyType.add (slice ("uint") ,PlyParserDataType::Char) ;
		mPropertyType.add (slice ("uint64") ,PlyParserDataType::Quad) ;
		mPropertyListType.add (slice ("uchar") ,PlyParserDataType::Byte) ;
		mPropertyListType.add (slice ("ushort") ,PlyParserDataType::Word) ;
		mPropertyListType.add (slice ("uint") ,PlyParserDataType::Char) ;
	}

	PlyParserImplLayout poll () {
		PlyParserImplLayout ret ;
		ret.mFormat = move (mFormat) ;
		ret.mElementList = move (mElementList) ;
		ret.mElementSet = move (mElementSet) ;
		return move (ret) ;
	}

	void generate () {
		read_header () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mFormat != slice ("ascii"))
				discard ;
			read_body_text () ;
		}
		if ifdo (act) {
			if (mFormat != slice ("binary_big_endian"))
				discard ;
			mBitwiseReverseFlag = FALSE ;
			read_body_byte () ;
		}
		if ifdo (act) {
			if (mFormat != slice ("binary_little_endian"))
				discard ;
			mBitwiseReverseFlag = TRUE ;
			read_body_byte () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read_header () {
		mTextReader = TextReader (mStream.share ()) ;
		mTextReader >> BOM ;
		mTextReader >> slice ("ply") ;
		mTextReader >> GAP ;
		mTextReader >> slice ("format") ;
		mTextReader >> GAP ;
		mTextReader >> IdentifierText::from (mLastString) ;
		mFormat = mLastString.clone () ;
		mTextReader >> EndlineText::from (mLastString) ;
		mTextReader >> GAP ;
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			mTextReader >> IdentifierText::from (mLastString) ;
			if (mLastString == slice ("end_header"))
				break ;
			mTextReader >> GAP ;
			auto act = TRUE ;
			if ifdo (act) {
				if (mLastString != slice ("element"))
					discard ;
				ix = mElementList.insert () ;
				mTextReader >> IdentifierText::from (mLastString) ;
				mElementList[ix].mName = mLastString.clone () ;
				mTextReader >> GAP ;
				mTextReader >> ScalarText::from (mLastString) ;
				const auto r1x = StringParse<LENGTH>::make (mLastString) ;
				assume (r1x >= 0) ;
				mElementList[ix].mLineSize = r1x ;
				mElementList[ix].mLineStep = 0 ;
				mElementList[ix].mLineLength = 0 ;
				mElementList[ix].mPropertyList = ArrayList<PlyParserProperty> (r1x) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				mTextReader >> IdentifierText::from (mLastType) ;
				mTextReader >> GAP ;
				if (mLastType != slice ("list"))
					discard ;
				iy = mElementList[ix].mPropertyList.insert () ;
				mTextReader >> IdentifierText::from (mLastString) ;
				const auto r2x = mPropertyListType.map (mLastString) ;
				assume (r2x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r2x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r3x = ply_parser_data_type_size (r2x) ;
				mElementList[ix].mLineStep += r3x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mTextReader >> IdentifierText::from (mLastString) ;
				const auto r4x = mPropertyType.map (mLastString) ;
				assume (r4x != NONE) ;
				mElementList[ix].mPropertyList[iy].mListType = r4x ;
				mElementList[ix].mPropertyList[iy].mListSize = 0 ;
				mTextReader >> GAP ;
				mTextReader >> IdentifierText::from (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = mLastString.clone () ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				iy = mElementList[ix].mPropertyList.insert () ;
				const auto r5x = mPropertyType.map (mLastType) ;
				assume (r5x != NONE) ;
				mElementList[ix].mPropertyList[iy].mType = r5x ;
				mElementList[ix].mPropertyList[iy].mPlyBegin = mElementList[ix].mLineStep ;
				const auto r6x = ply_parser_data_type_size (r5x) ;
				mElementList[ix].mLineStep += r6x ;
				mElementList[ix].mPropertyList[iy].mPlyEnd = mElementList[ix].mLineStep ;
				mTextReader >> GAP ;
				mElementList[ix].mPropertyList[iy].mListType = PlyParserDataType::Null ;
				mElementList[ix].mPropertyList[iy].mListSize = 0 ;
				mTextReader >> GAP ;
				mTextReader >> IdentifierText::from (mLastString) ;
				mElementList[ix].mPropertyList[iy].mName = mLastString.clone () ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("comment"))
					discard ;
				mTextReader >> EndlineText::from (mLastString) ;
				mTextReader >> GAP ;
			}
			if ifdo (act) {
				assume (FALSE) ;
			}
		}
		if ifdo (TRUE) {
			auto rax = STRU32 () ;
			mTextReader.read (rax) ;
			if (rax == STRU32 ('\r'))
				discard ;
			mTextReader.reset (mTextReader.length () - 1 ,mTextReader.size ()) ;
		}
		if ifdo (TRUE) {
			auto rax = STRU32 () ;
			mTextReader.read (rax) ;
			assume (rax == STRU32 ('\n')) ;
			mBodyOffset = mTextReader.length () ;
		}
		for (auto &&i : mElementList.range ()) {
			auto &&rax = mElementList[i] ;
			rax.mPropertyList.remap () ;
			rax.mPropertySet = Set<String<STRU8>> (rax.mPropertyList.length ()) ;
			for (auto &&j : rax.mPropertyList.range ())
				rax.mPropertySet.add (rax.mPropertyList[j].mName ,j) ;
		}
		if ifdo (TRUE) {
			mElementList.remap () ;
			mElementSet = Set<String<STRU8>> (mElementList.length ()) ;
			for (auto &&j : mElementList.range ())
				mElementSet.add (mElementList[j].mName ,j) ;
		}
		for (auto &&i : mElementList.range ()) {
			const auto r7x = mElementList[i].mLineSize * mElementList[i].mLineStep ;
			mElementList[i].mPlyBuffer = RefBuffer<BYTE> (r7x) ;
			mElementList[i].mPlyIndex = address (mElementList[i].mPlyBuffer[0]) ;
		}
	}

	LENGTH ply_parser_data_type_size (CREF<Just<PlyParserDataType>> type) const {
		if (type == PlyParserDataType::Bool)
			return 1 ;
		if (type == PlyParserDataType::Val32)
			return 4 ;
		if (type == PlyParserDataType::Val64)
			return 8 ;
		if (type == PlyParserDataType::Flt32)
			return 4 ;
		if (type == PlyParserDataType::Flt64)
			return 8 ;
		if (type == PlyParserDataType::Byte)
			return 1 ;
		if (type == PlyParserDataType::Word)
			return 2 ;
		if (type == PlyParserDataType::Char)
			return 4 ;
		if (type == PlyParserDataType::Quad)
			return 8 ;
		return 0 ;
	}

	void read_body_text () {
		mTextReader = TextReader (mStream.share ()) ;
		mTextReader.reset (mBodyOffset ,mStream->size ()) ;
		mTextReader >> GAP ;
		for (auto &&i : iter (0 ,mElementList.length ())) {
			for (auto &&j : iter (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : iter (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_text_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_text_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mTextReader >> GAP ;
		mTextReader >> EOS ;
	}

	void read_body_text_item (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mTextReader.poll (TYPE<FLT32>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r2x) ,SIZE_OF<FLT32>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r3x = mTextReader.poll (TYPE<FLT64>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r3x) ,SIZE_OF<FLT64>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r4x = mTextReader.poll (TYPE<BOOL>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r4x) ,SIZE_OF<BOOL>::expr) ;
			element.mPlyIndex += SIZE_OF<BOOL>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r5x = mTextReader.poll (TYPE<VAL32>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r5x) ,SIZE_OF<VAL32>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL32>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r6x = mTextReader.poll (TYPE<VAL64>::expr) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r6x) ,SIZE_OF<VAL64>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL64>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r7x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r7x >= 0) ;
			const auto r8x = BYTE (r7x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r8x) ,SIZE_OF<BYTE>::expr) ;
			element.mPlyIndex += SIZE_OF<BYTE>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r9x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r9x >= 0) ;
			const auto r10x = BYTE (r9x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r10x) ,SIZE_OF<WORD>::expr) ;
			element.mPlyIndex += SIZE_OF<WORD>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r11x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r11x >= 0) ;
			const auto r12x = BYTE (r11x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r12x) ,SIZE_OF<CHAR>::expr) ;
			element.mPlyIndex += SIZE_OF<CHAR>::expr ;
			mTextReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r13x = mTextReader.poll (TYPE<VAL64>::expr) ;
			assume (r13x >= 0) ;
			const auto r14x = BYTE (r13x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r14x) ,SIZE_OF<QUAD>::expr) ;
			element.mPlyIndex += SIZE_OF<QUAD>::expr ;
			mTextReader >> GAP ;
		}
	}

	void read_body_text_list (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = address (element.mPlyBuffer[property.mPlyBegin]) ;
		const auto r3x = invoke ([&] () {
			if (property.mType == PlyParserDataType::Byte)
				return LENGTH (bitwise[TYPE<BYTE>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Word)
				return LENGTH (bitwise[TYPE<WORD>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Char)
				return LENGTH (bitwise[TYPE<CHAR>::expr] (Pointer::make (r2x))) ;
			assume (FALSE) ;
			return ZERO ;
		}) ;
		if ifdo (TRUE) {
			if (property.mListSize != 0)
				discard ;
			property.mListSize = r3x ;
			const auto r4x = ply_parser_data_type_size (property.mListType) ;
			const auto r5x = property.mPlyEnd ;
			for (auto &&i : iter (0 ,element.mPropertyList.length ())) {
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyBegin < r5x)
						discard ;
					element.mPropertyList[i].mPlyBegin += r4x ;
				}
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyEnd < r5x)
						discard ;
					element.mPropertyList[i].mPlyEnd += r4x ;
				}
			}
			element.mLineStep += r4x ;
			const auto r6x = element.mLineSize * element.mLineStep ;
			element.mPlyBuffer.resize (r6x) ;
		}
		assume (property.mListSize == r3x) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r7x = mTextReader.poll (TYPE<VAL32>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r7x) ,SIZE_OF<VAL32>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL32>::expr ;
				mTextReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			mTextReader >> GAP ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r8x = mTextReader.poll (TYPE<VAL64>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r8x) ,SIZE_OF<VAL64>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL64>::expr ;
				mTextReader >> GAP ;
			}
		}
	}

	void read_body_byte () {
		mByteReader = ByteReader (mStream.share ()) ;
		mByteReader.reset (mBodyOffset ,mStream->size ()) ;
		for (auto &&i : iter (0 ,mElementList.length ())) {
			for (auto &&j : iter (0 ,mElementList[i].mLineSize)) {
				for (auto &&k : iter (0 ,mElementList[i].mPropertyList.length ())) {
					read_body_byte_item (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					read_body_byte_list (mElementList[i] ,mElementList[i].mPropertyList[k] ,j) ;
					mElementList[i].mLineLength++ ;
				}
			}
		}
		mByteReader >> EOS ;
	}

	void read_body_byte_item (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			const auto r2x = mByteReader.poll (TYPE<FLT32>::expr) ;
			const auto r3x = bitwise_reverse (r2x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r3x) ,SIZE_OF<FLT32>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			const auto r4x = mByteReader.poll (TYPE<FLT64>::expr) ;
			const auto r5x = bitwise_reverse (r4x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r5x) ,SIZE_OF<FLT64>::expr) ;
			element.mPlyIndex += SIZE_OF<FLT64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Bool)
				discard ;
			const auto r6x = mByteReader.poll (TYPE<BOOL>::expr) ;
			const auto r7x = bitwise_reverse (r6x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r7x) ,SIZE_OF<BOOL>::expr) ;
			element.mPlyIndex += SIZE_OF<BOOL>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			const auto r8x = mByteReader.poll (TYPE<VAL32>::expr) ;
			const auto r9x = bitwise_reverse (r8x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r9x) ,SIZE_OF<VAL32>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL32>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			const auto r10x = mByteReader.poll (TYPE<VAL64>::expr) ;
			const auto r11x = bitwise_reverse (r10x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r11x) ,SIZE_OF<VAL64>::expr) ;
			element.mPlyIndex += SIZE_OF<VAL64>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			const auto r12x = mByteReader.poll (TYPE<BYTE>::expr) ;
			const auto r13x = bitwise_reverse (r12x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r13x) ,SIZE_OF<BYTE>::expr) ;
			element.mPlyIndex += SIZE_OF<BYTE>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			const auto r14x = mByteReader.poll (TYPE<WORD>::expr) ;
			const auto r15x = bitwise_reverse (r14x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r15x) ,SIZE_OF<WORD>::expr) ;
			element.mPlyIndex += SIZE_OF<WORD>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			const auto r16x = mByteReader.poll (TYPE<CHAR>::expr) ;
			const auto r17x = bitwise_reverse (r16x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r17x) ,SIZE_OF<CHAR>::expr) ;
			element.mPlyIndex += SIZE_OF<CHAR>::expr ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			const auto r18x = mByteReader.poll (TYPE<QUAD>::expr) ;
			const auto r19x = bitwise_reverse (r18x) ;
			inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r19x) ,SIZE_OF<QUAD>::expr) ;
			element.mPlyIndex += SIZE_OF<QUAD>::expr ;
		}
	}

	void read_body_byte_list (VREF<PlyParserElement> element ,VREF<PlyParserProperty> property ,CREF<INDEX> line) {
		const auto r1x = property.mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = address (element.mPlyBuffer[property.mPlyBegin]) ;
		const auto r3x = invoke ([&] () {
			if (property.mType == PlyParserDataType::Byte)
				return LENGTH (bitwise[TYPE<BYTE>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Word)
				return LENGTH (bitwise[TYPE<WORD>::expr] (Pointer::make (r2x))) ;
			if (property.mType == PlyParserDataType::Char)
				return LENGTH (bitwise[TYPE<CHAR>::expr] (Pointer::make (r2x))) ;
			assume (FALSE) ;
			return ZERO ;
		}) ;
		if ifdo (TRUE) {
			if (property.mListSize != 0)
				discard ;
			property.mListSize = r3x ;
			const auto r4x = ply_parser_data_type_size (property.mListType) ;
			const auto r5x = property.mPlyEnd ;
			for (auto &&i : iter (0 ,element.mPropertyList.length ())) {
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyBegin < r5x)
						discard ;
					element.mPropertyList[i].mPlyBegin += r4x ;
				}
				if ifdo (TRUE) {
					if (element.mPropertyList[i].mPlyEnd < r5x)
						discard ;
					element.mPropertyList[i].mPlyEnd += r4x ;
				}
			}
			element.mLineStep += r4x ;
			const auto r6x = element.mLineSize * element.mLineStep ;
			element.mPlyBuffer.resize (r6x) ;
		}
		assume (property.mListSize == r3x) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r7x = mByteReader.poll (TYPE<VAL32>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r7x) ,SIZE_OF<VAL32>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL32>::expr ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			for (auto &&i : iter (0 ,r3x)) {
				noop (i) ;
				const auto r8x = mByteReader.poll (TYPE<VAL64>::expr) ;
				inline_memcpy (Pointer::make (element.mPlyIndex) ,Pointer::from (r8x) ,SIZE_OF<VAL64>::expr) ;
				element.mPlyIndex += SIZE_OF<VAL64>::expr ;
			}
		}
	}

	template <class ARG1>
	BYTE_BASE<ARG1> bitwise_reverse (CREF<ARG1> a) const {
		if (!mBitwiseReverseFlag)
			return bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a) ;
		return ByteProc::bit_reverse (bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a)) ;
	}
} ;

class PlyParserImplHolder implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakePlyParser (Ref<RefBuffer<BYTE>>::reference (stream)) ;
		rax.generate () ;
		fake.mThis = Ref<PlyParserImplLayout>::make (rax.poll ()) ;
		fake.mGuide.mElementIndex = NONE ;
	}

	LENGTH element_size (CREF<Slice> element) const override {
		INDEX ix = fake.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		return fake.mThis->mElementList[ix].mLineSize ;
	}

	LENGTH property_size (CREF<Slice> element ,CREF<Slice> property) const override {
		INDEX ix = fake.mThis->mElementSet.map (element) ;
		if (ix == NONE)
			return 0 ;
		INDEX jx = fake.mThis->mElementList[ix].mPropertySet.map (property) ;
		if (jx == NONE)
			return 0 ;
		return fake.mThis->mElementList[ix].mPropertyList[jx].mListSize ;
	}

	void guide_new (CREF<Slice> element) override {
		INDEX ix = fake.mThis->mElementSet.map (element) ;
		assume (ix != NONE) ;
		fake.mGuide.mElementIndex = ix ;
		fake.mGuide.mProperty.clear () ;
		fake.mGuide.mPropertyIndex = 0 ;
		fake.mGuide.mLineIndex = 0 ;
		fake.mGuide.mPlyIndex = NONE ;
	}

	void guide_put (CREF<Slice> property) override {
		INDEX ix = fake.mGuide.mElementIndex ;
		assume (ix != NONE) ;
		INDEX jx = fake.mThis->mElementList[ix].mPropertySet.map (property) ;
		assume (jx != NONE) ;
		assert (fake.mGuide.mPlyIndex == NONE) ;
		fake.mGuide.mProperty.add (jx) ;
	}

	void guide_jmp () {
		assert (fake.mGuide.mElementIndex != NONE) ;
		INDEX ix = fake.mGuide.mElementIndex ;
		INDEX jx = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mGuide.mPlyIndex != NONE)
				discard ;
			fake.mGuide.mLineIndex = 0 ;
			fake.mGuide.mPropertyIndex = 0 ;
			assume (fake.mGuide.mPropertyIndex < fake.mGuide.mProperty.length ()) ;
			assume (fake.mGuide.mLineIndex < fake.mThis->mElementList[ix].mLineSize) ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			const auto r1x = fake.mGuide.mLineIndex * fake.mThis->mElementList[ix].mLineStep ;
			const auto r2x = r1x + fake.mThis->mElementList[ix].mPropertyList[jx].mPlyBegin ;
			fake.mGuide.mPlyIndex = address (fake.mThis->mElementList[ix].mPlyBuffer[r2x]) ;
		}
		if ifdo (act) {
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			if (fake.mGuide.mPlyIndex >= fake.mThis->mElementList[ix].mPropertyList[jx].mPlyEnd)
				discard ;
		}
		if ifdo (act) {
			fake.mGuide.mPropertyIndex++ ;
			if (fake.mGuide.mPropertyIndex >= fake.mGuide.mProperty.length ())
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			const auto r3x = fake.mGuide.mLineIndex * fake.mThis->mElementList[ix].mLineStep ;
			const auto r4x = r3x + fake.mThis->mElementList[ix].mPropertyList[jx].mPlyBegin ;
			fake.mGuide.mPlyIndex = address (fake.mThis->mElementList[ix].mPlyBuffer[r4x]) ;
		}
		if ifdo (act) {
			fake.mGuide.mLineIndex++ ;
			fake.mGuide.mPropertyIndex = 0 ;
			if (fake.mGuide.mLineIndex >= fake.mThis->mElementList[ix].mLineSize)
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			const auto r5x = fake.mGuide.mLineIndex * fake.mThis->mElementList[ix].mLineStep ;
			const auto r6x = r5x + fake.mThis->mElementList[ix].mPropertyList[jx].mPlyBegin ;
			fake.mGuide.mPlyIndex = address (fake.mThis->mElementList[ix].mPlyBuffer[r6x]) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		fake.mGuide.mPlyType = fake.mThis->mElementList[ix].mPropertyList[jx].mType ;
	}

	void read (VREF<BOOL> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Bool) ;
		item = bitwise[TYPE<BOOL>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<BOOL>::expr ;
	}

	void read (VREF<VAL32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Val32) ;
		item = bitwise[TYPE<VAL32>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<VAL32>::expr ;
	}

	void read (VREF<VAL64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Val64) ;
		item = bitwise[TYPE<VAL64>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<VAL64>::expr ;
	}

	void read (VREF<FLT32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Flt32) ;
		item = bitwise[TYPE<FLT32>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<FLT32>::expr ;
	}

	void read (VREF<FLT64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Flt64) ;
		item = bitwise[TYPE<FLT64>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<FLT64>::expr ;
	}

	void read (VREF<BYTE> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Byte) ;
		item = bitwise[TYPE<BYTE>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<BYTE>::expr ;
	}

	void read (VREF<WORD> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Word) ;
		item = bitwise[TYPE<WORD>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<WORD>::expr ;
	}

	void read (VREF<CHAR> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Char) ;
		item = bitwise[TYPE<CHAR>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<CHAR>::expr ;
	}

	void read (VREF<QUAD> item) override {
		guide_jmp () ;
		item = bitwise[TYPE<QUAD>::expr] (Pointer::make (fake.mGuide.mPlyIndex)) ;
		fake.mGuide.mPlyIndex += SIZE_OF<QUAD>::expr ;
	}
} ;

exports VFat<PlyParserHolder> PlyParserHolder::create (VREF<PlyParserLayout> that) {
	return VFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::create (CREF<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}
} ;