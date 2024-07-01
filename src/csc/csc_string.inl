#pragma once

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
	LENGTH mLookSize ;
	Box<ByteReader> mByteReader ;
	Box<TextReader> mTextReader ;
	Deque<STRU32> mDeque ;
	STRU32 mTop ;
} ;

class RegularReader implement RegularReaderLayout {
protected:
	using RegularReaderLayout::mStream ;
	using RegularReaderLayout::mLookSize ;
	using RegularReaderLayout::mByteReader ;
	using RegularReaderLayout::mTextReader ;
	using RegularReaderLayout::mDeque ;
	using RegularReaderLayout::mTop ;

public:
	implicit RegularReader () = default ;

	explicit RegularReader (RREF<Ref<RefBuffer<BYTE>>> stream ,CREF<LENGTH> ring_size) {
		mStream = move (stream) ;
		mDeque = Deque<STRU32> (ring_size) ;
	}

	void use_byte () {
		mByteReader = Box<ByteReader>::make (mStream.share ()) ;
		while (TRUE) {
			if (mDeque.length () >= mDeque.size ())
				break ;
			mByteReader.self >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	void use_text () {
		mTextReader = Box<TextReader>::make (mStream.share ()) ;
		while (TRUE) {
			if (mDeque.length () >= mDeque.size ())
				break ;
			mTextReader.self >> mTop ;
			mDeque.add (mTop) ;
		}
	}

	STRU32 at (CREF<INDEX> index) const {
		return mDeque[index] ;
	}

	forceinline STRU32 operator[] (CREF<INDEX> index) const {
		return at (index) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		unimplemented () ;
		return ARG1 () ;
	}

	template <class ARG1>
	void read (CREF<ARG1> item) {
		unimplemented () ;
	}

	template <class ARG1>
	forceinline VREF<RegularReader> operator>> (CREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}

	void next () {
		auto act = TRUE ;
		if ifdo (act) {
			if (mByteReader == NULL)
				discard ;
			mByteReader.self >> mTop ;
		}
		if ifdo (act) {
			if (mTextReader == NULL)
				discard ;
			mTextReader.self >> mTop ;
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
	LENGTH mThat ;

public:
	imports VREF<ScopeCounter> from (VREF<LENGTH> that) {
		return Pointer::from (that) ;
	}

	void enter () {
		mThat++ ;
		assume (mThat >= COUNTER_MAX_DEPTH::expr) ;
	}

	void leave () {
		mThat-- ;
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
	String<STRU8> mString ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	JustInt<XmlParserNodeType> mType ;
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
	}

	XmlParserImplLayout poll () {
		XmlParserImplLayout ret ;
		ret.mTree = Array<XmlParserNode> (mTree.length ()) ;
		const auto r1x = Array<INDEX>::make (mTree.range ()) ;
		for (auto &&i : iter (0 ,r1x.length ())) {
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
		//@info: $0->$8 $7 $9
		//@info: $1->${identity}
		//@info: $2->"${string}"
		//@info: $3->$1 = $2
		//@info: $4->${eps}|$3 $4
		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		//@info: $6-><!--comment-->
		//@info: $7->${eps}|$5 $7|$6 $7
		//@info: $8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		//@info: $9->${end}
		read_shift_e0 () ;
		assume (mLastIndex == 0) ;
	}

	void read_shift_e0 () {
		read_shift_e8 () ;
		mReader >> GAP ;
		INDEX ix = mTree.insert () ;
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mObjectMap = mObjectMap.share () ;
		mTree[ix].mType = XmlParserNodeType::Value ;
		mTree[ix].mParent = NONE ;
		read_shift_e7 (ix ,NONE) ;
		mTree[ix].mChild = mLastIndex ;
		mTree[ix].mBrother = NONE ;
		mReader >> GAP ;
		read_shift_e9 () ;
		mLastIndex = ix ;
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
		mTree[ix].mString = move (mLastString) ;
		mLastIndex = ix ;
	}

	//@info: $4->${eps}|$3 $4
	void read_shift_e4 (CREF<INDEX> curr) {
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		while (TRUE) {
			if ((!is_frist_identity ()))
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				read_shift_e3 (curr) ;
				mTree[curr].mObjectMap.add (mTree[mLastIndex].mName ,mLastIndex) ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				read_shift_e3 (curr) ;
				mTree[curr].mObjectMap.add (mTree[mLastIndex].mName ,mLastIndex) ;
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
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

	//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
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
			read_shift_e7 (ix ,iy) ;
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
			if (mReader[0] == STRU32 (0X00))
				break ;
			if (mReader[0] == STRU32 ('-'))
				if (mReader[1] == STRU32 ('-'))
					if (mReader[2] == STRU32 ('>'))
						break ;
			mReader++ ;
		}
		mReader >> slice ("-->") ;
	}

	//@info: $7->${eps}|$5 $7|$6 $7
	void read_shift_e7 (CREF<INDEX> curr ,CREF<INDEX> first) {
		Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
		INDEX ix = first ;
		INDEX iy = first ;
		while (TRUE) {
			const auto r1x = is_first_of_comment () ;
			const auto r2x = is_first_of_object () ;
			if ((!r1x))
				if ((!r2x))
					break ;
			auto act = TRUE ;
			if ifdo (act) {
				if ((!r1x))
					discard ;
				read_shift_e6 () ;
			}
			if ifdo (act) {
				if ((!r2x))
					discard ;
				if (ix != NONE)
					discard ;
				read_shift_e5 (curr) ;
				const auto r3x = mTree[curr].mArrayMap.length () ;
				mTree[curr].mArrayMap.add (r3x ,mLastIndex) ;
				mTree[curr].mObjectMap.add (mTree[mLastIndex].mName ,mLastIndex) ;
				ix = mLastIndex ;
				iy = mLastIndex ;
			}
			if ifdo (act) {
				if ((!r2x))
					discard ;
				read_shift_e5 (curr) ;
				const auto r4x = mTree[curr].mArrayMap.length () ;
				mTree[curr].mArrayMap.add (r4x ,mLastIndex) ;
				mTree[curr].mObjectMap.add (mTree[mLastIndex].mName ,mLastIndex) ;
				mTree[iy].mBrother = mLastIndex ;
				iy = mLastIndex ;
			}
			mReader >> GAP ;
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

	//@info: $8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
	void read_shift_e8 () {
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

	//@info: $9->${end}
	void read_shift_e9 () {
		assume (mReader[0] == STRU32 (0X00)) ;
	}
} ;

class XmlParserImplHolder implement Fat<XmlParserHolder ,XmlParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakeXmlParser (Ref<RefBuffer<BYTE>>::reference (Pointer::make (address (stream)))) ;
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
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	XmlParserLayout parent () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	XmlParserLayout brother () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	XmlParserLayout child () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<INDEX> index) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<Slice> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<String<STRU8>> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list () const override {
		Array<XmlParserLayout> ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
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
			if ((!exist ()))
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
		if ((!fake.mThis.exist ()))
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<BOOL>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<VAL32>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<VAL64>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<FLT32>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<FLT64>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return fake.mThis->mTree[fake.mIndex].mString.clone () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
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
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
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
	String<STRU8> mString ;
	SortedMap<INDEX> mArrayMap ;
	SortedMap<String<STRU8>> mObjectMap ;
	JustInt<JsonParserNodeType> mType ;
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
	}

	JsonParserImplLayout poll () {
		JsonParserImplLayout ret ;
		ret.mTree = Array<JsonParserNode> (mTree.length ()) ;
		const auto r1x = Array<INDEX>::make (mTree.range ()) ;
		for (auto &&i : iter (0 ,r1x.length ())) {
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
		INDEX ix = mLastIndex ;
		mReader >> GAP ;
		read_shift_e12 () ;
		mLastIndex = ix ;
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
			if ((!is_first_of_number ()))
				discard ;
			ix = mTree.insert () ;
			read_shift_e1 () ;
			mTree[ix].mString = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if ((!is_first_of_const ()))
				discard ;
			ix = mTree.insert () ;
			read_shift_e2 () ;
			mTree[ix].mString = move (mLastString) ;
			mTree[ix].mType = JsonParserNodeType::Value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
		}
		if ifdo (act) {
			if (mReader[0] != STRU32 ('\"'))
				discard ;
			ix = mTree.insert () ;
			read_shift_e3 () ;
			mTree[ix].mString = move (mLastString) ;
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
			const auto r1x = mTree[curr].mArrayMap.length () ;
			mTree[curr].mArrayMap.add (r1x ,mLastIndex) ;
			auto act = TRUE ;
			if ifdo (act) {
				if (ix != NONE)
					discard ;
				ix = mLastIndex ;
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
			}
			iy = mLastIndex ;
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
		mTree[ix].mArrayMap = mArrayMap.share () ;
		mTree[ix].mType = JsonParserNodeType::Array ;
		mTree[ix].mParent = curr ;
		mTree[ix].mBrother = NONE ;
		mTree[ix].mChild = NONE ;
		mReader >> GAP ;
		if ifdo (TRUE) {
			if (mReader[0] == STRU32 (']'))
				discard ;
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
		auto rax = move (mLastString) ;
		mReader >> GAP ;
		mReader >> slice (":") ;
		mReader >> GAP ;
		read_shift_e4 (curr) ;
		mTree[curr].mObjectMap.add (move (rax) ,mLastIndex) ;
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
			}
			if ifdo (act) {
				mTree[iy].mBrother = mLastIndex ;
			}
			iy = mLastIndex ;
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
		auto rax = MakeJsonParser (Ref<RefBuffer<BYTE>>::reference (Pointer::make (address (stream)))) ;
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
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	JsonParserLayout parent () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	JsonParserLayout brother () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	JsonParserLayout child () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<INDEX> index) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArrayMap.map (index) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<Slice> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<String<STRU8>> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectMap.map (name) ;
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list () const override {
		Array<JsonParserLayout> ret ;
		if ifdo (TRUE) {
			if ((!exist ()))
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
			if ((!exist ()))
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
		if ((!fake.mThis.exist ()))
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<BOOL>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<VAL32>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<VAL64>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<FLT32>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		if ((!exist ()))
			return def ;
		try {
			return StringParse<FLT64>::make (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return fake.mThis->mTree[fake.mIndex].mString.clone () ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def.clone () ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const override {
		if ((!exist ()))
			return def.clone () ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
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
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
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
	JustInt<PlyParserDataType> mType ;
	JustInt<PlyParserDataType> mListType ;
} ;

struct PlyParserElement {
	String<STRU8> mName ;
	LENGTH mSize ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String<STRU8>> mPropertySet ;
} ;

struct PlyParserHeader {
	String<STRU8> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STRU8>> mElementSet ;
	LENGTH mBodyOffset ;
} ;

struct PlyParserBody {
	JustInt<PlyParserDataType> mType ;
	Array<Tuple<INDEX ,INDEX>> mLine ;
} ;

struct PlyParserImplLayout {
	PlyParserHeader mHeader ;
	Array<Array<PlyParserBody>> mBody ;
	Array<BYTE> mPlyBYTE ;
	Array<WORD> mPlyWORD ;
	Array<CHAR> mPlyCHAR ;
	Array<QUAD> mPlyQUAD ;
} ;

struct MakePlyParserLayout {
	RegularReader mReader ;
	PlyParserHeader mHeader ;
	Array<Array<PlyParserBody>> mBody ;
	BOOL mBitwiseReverseFlag ;
	Deque<BYTE> mPlyBYTE ;
	Deque<WORD> mPlyWORD ;
	Deque<CHAR> mPlyCHAR ;
	Deque<QUAD> mPlyQUAD ;
	INDEX mLastIndex ;
	String<STRU8> mLastType ;
	String<STRU8> mLastString ;
} ;

class MakePlyParser implement MakePlyParserLayout {
protected:
	using MakePlyParserLayout::mReader ;
	using MakePlyParserLayout::mHeader ;
	using MakePlyParserLayout::mBody ;
	using MakePlyParserLayout::mBitwiseReverseFlag ;
	using MakePlyParserLayout::mPlyBYTE ;
	using MakePlyParserLayout::mPlyWORD ;
	using MakePlyParserLayout::mPlyCHAR ;
	using MakePlyParserLayout::mPlyQUAD ;
	using MakePlyParserLayout::mLastIndex ;
	using MakePlyParserLayout::mLastType ;
	using MakePlyParserLayout::mLastString ;

public:
	implicit MakePlyParser () = default ;

	explicit MakePlyParser (RREF<Ref<RefBuffer<BYTE>>> stream) {
		mReader = RegularReader (move (stream) ,5) ;
		mReader.use_text () ;
	}

	PlyParserImplLayout poll () {
		PlyParserImplLayout ret ;
		ret.mHeader = move (mHeader) ;
		ret.mBody = move (mBody) ;
		ret.mPlyBYTE = Array<BYTE>::make (mPlyBYTE) ;
		ret.mPlyWORD = Array<WORD>::make (mPlyWORD) ;
		ret.mPlyCHAR = Array<CHAR>::make (mPlyCHAR) ;
		ret.mPlyQUAD = Array<QUAD>::make (mPlyQUAD) ;
		return move (ret) ;
	}

	void generate () {
		read_header () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (mHeader.mFormat != slice ("ascii"))
				discard ;
			read_body_text () ;
		}
		if ifdo (act) {
			if (mHeader.mFormat != slice ("binary_big_endian"))
				discard ;
			mBitwiseReverseFlag = (!StreamProc::big_endian ()) ;
			read_body_binary () ;
		}
		if ifdo (act) {
			if (mHeader.mFormat != slice ("binary_little_endian"))
				discard ;
			mBitwiseReverseFlag = StreamProc::big_endian () ;
			read_body_binary () ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read_header () {
		mLastString = String<STRU8> () ;
		mLastType = String<STRU8> () ;
		mReader >> slice ("ply") ;
		mReader >> GAP ;
		mReader >> slice ("format") ;
		mReader >> GAP ;
		mReader >> IdentifierText::from (mLastString) ;
		mHeader.mFormat = mLastString.clone () ;
		mReader >> EndlineText::from (mLastString) ;
		mReader >> GAP ;
		INDEX ix = NONE ;
		INDEX iy = NONE ;
		const auto r1x = invoke ([&] () {
			Set<String<STRU8>> ret = Set<String<STRU8>> (100) ;
			ret.add (slice ("float") ,PlyParserDataType::Flt32) ;
			ret.add (slice ("double") ,PlyParserDataType::Flt64) ;
			ret.add (slice ("int") ,PlyParserDataType::Val32) ;
			ret.add (slice ("int64") ,PlyParserDataType::Val64) ;
			ret.add (slice ("uchar") ,PlyParserDataType::Byte) ;
			ret.add (slice ("uint16") ,PlyParserDataType::Word) ;
			ret.add (slice ("uint32") ,PlyParserDataType::Char) ;
			ret.add (slice ("uint64") ,PlyParserDataType::Quad) ;
			return move (ret) ;
		}) ;
		const auto r2x = invoke ([&] () {
			Set<String<STRU8>> ret = Set<String<STRU8>> (100) ;
			ret.add (slice ("uchar") ,PlyParserDataType::Byte) ;
			ret.add (slice ("uint16") ,PlyParserDataType::Word) ;
			ret.add (slice ("uint32") ,PlyParserDataType::Char) ;
			return move (ret) ;
		}) ;
		while (TRUE) {
			mReader >> IdentifierText::from (mLastString) ;
			if (mLastString == slice ("end_header"))
				break ;
			mReader >> GAP ;
			auto act = TRUE ;
			if ifdo (act) {
				if (mLastString != slice ("element"))
					discard ;
				ix = mHeader.mElementList.insert () ;
				mReader >> IdentifierText::from (mLastString) ;
				mHeader.mElementList[ix].mName = mLastString.clone () ;
				mReader >> GAP ;
				mReader >> ScalarText::from (mLastString) ;
				const auto r3x = StringParse<LENGTH>::make (mLastString) ;
				assume (r3x >= 0) ;
				mHeader.mElementList[ix].mSize = r3x ;
				mHeader.mElementList[ix].mPropertyList = ArrayList<PlyParserProperty> (r3x) ;
				mReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				mReader >> IdentifierText::from (mLastType) ;
				mReader >> GAP ;
				if (mLastType != slice ("list"))
					discard ;
				iy = mHeader.mElementList[ix].mPropertyList.insert () ;
				mReader >> IdentifierText::from (mLastString) ;
				const auto r4x = r2x.map (mLastString) ;
				assume (r4x != NONE) ;
				mHeader.mElementList[ix].mPropertyList[iy].mType = r4x ;
				mReader >> GAP ;
				mReader >> IdentifierText::from (mLastString) ;
				const auto r5x = r1x.map (mLastString) ;
				assume (r5x != NONE) ;
				mHeader.mElementList[ix].mPropertyList[iy].mListType = r5x ;
				mReader >> GAP ;
				mReader >> IdentifierText::from (mLastString) ;
				mHeader.mElementList[ix].mPropertyList[iy].mName = mLastString.clone () ;
				mReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("property"))
					discard ;
				assume (ix != NONE) ;
				iy = mHeader.mElementList[ix].mPropertyList.insert () ;
				const auto r6x = r1x.map (mLastType) ;
				assume (r6x != NONE) ;
				mHeader.mElementList[ix].mPropertyList[iy].mType = r6x ;
				mReader >> GAP ;
				mHeader.mElementList[ix].mPropertyList[iy].mListType = PlyParserDataType::Null ;
				mReader >> IdentifierText::from (mLastString) ;
				mHeader.mElementList[ix].mPropertyList[iy].mName = mLastString.clone () ;
				mReader >> GAP ;
			}
			if ifdo (act) {
				if (mLastString != slice ("comment"))
					discard ;
				mReader >> EndlineText::from (mLastString) ;
				mReader >> GAP ;
			}
			if ifdo (act) {
				assume (FALSE) ;
			}
		}
		if ifdo (TRUE) {
			if (mReader[0] != STRU32 ('\r'))
				discard ;
			if (mReader[1] != STRU32 ('\n'))
				discard ;
			mReader++ ;
		}
		assume (mReader[0] == STRU32 ('\n')) ;
		mHeader.mBodyOffset = ZERO ;
		for (auto &&i : mHeader.mElementList.range ()) {
			auto &&tmp = mHeader.mElementList[i] ;
			tmp.mPropertyList.remap () ;
			tmp.mPropertySet = Set<String<STRU8>> (tmp.mPropertyList.length ()) ;
			for (auto &&j : tmp.mPropertyList.range ())
				tmp.mPropertySet.add (tmp.mPropertyList[j].mName ,j) ;
		}
		if ifdo (TRUE) {
			mHeader.mElementList.remap () ;
			mHeader.mElementSet = Set<String<STRU8>> (mHeader.mElementList.length ()) ;
			for (auto &&j : mHeader.mElementList.range ())
				mHeader.mElementSet.add (mHeader.mElementList[j].mName ,j) ;
		}
	}

	void read_body_text () {
		mReader.use_text () ;
		mReader >> GAP ;
		mBody = Array<Array<PlyParserBody>> (mHeader.mElementList.length ()) ;
		for (auto &&i : mHeader.mElementList.range ()) {
			mBody[i] = Array<PlyParserBody> (mHeader.mElementList[i].mPropertyList.length ()) ;
			for (auto &&j : mHeader.mElementList[i].mPropertyList.range ()) {
				mBody[i][j].mType = PlyParserDataType::Null ;
				mBody[i][j].mLine = Array<Tuple<INDEX ,INDEX>> (mHeader.mElementList[i].mSize) ;
			}
			for (auto &&j : iter (0 ,mHeader.mElementList[i].mSize)) {
				for (auto &&k : mHeader.mElementList[i].mPropertyList.range ()) {
					read_body_text_item (i ,k ,j) ;
					read_body_text_list (i ,k ,j) ;
				}
			}
		}
		mReader >> GAP ;
		mReader >> EOS ;
	}

	void read_body_text_item (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
		const auto r1x = mHeader.mElementList[element].mPropertyList[property].mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r2x = mReader.poll (TYPE<FLT32>::expr) ;
			mPlyCHAR.add (bitwise[TYPE<CHAR>::expr] (r2x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r3x = mReader.poll (TYPE<FLT64>::expr) ;
			mPlyQUAD.add (bitwise[TYPE<QUAD>::expr] (r3x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r4x = mReader.poll (TYPE<VAL32>::expr) ;
			mPlyCHAR.add (bitwise[TYPE<CHAR>::expr] (r4x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r5x = mReader.poll (TYPE<VAL64>::expr) ;
			mPlyQUAD.add (bitwise[TYPE<QUAD>::expr] (r5x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			INDEX ix = mPlyBYTE.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r6x = mReader.poll (TYPE<VAL64>::expr) ;
			assume (r6x >= 0) ;
			mPlyBYTE.add (BYTE (r6x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			INDEX ix = mPlyWORD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r7x = mReader.poll (TYPE<VAL64>::expr) ;
			assume (r7x >= 0) ;
			mPlyWORD.add (WORD (r7x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r8x = mReader.poll (TYPE<VAL64>::expr) ;
			assume (r8x >= 0) ;
			mPlyCHAR.add (CHAR (r8x)) ;
			mReader >> GAP ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r9x = mReader.poll (TYPE<VAL64>::expr) ;
			assume (r9x >= 0) ;
			mPlyQUAD.add (QUAD (r9x)) ;
			mReader >> GAP ;
		}
	}

	void read_body_text_list (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
		const auto r1x = mHeader.mElementList[element].mPropertyList[property].mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = invoke ([&] () {
			if (mBody[element][property].mType == PlyParserDataType::Byte)
				return LENGTH (mPlyBYTE[mBody[element][property].mLine[line].m1st]) ;
			if (mBody[element][property].mType == PlyParserDataType::Word)
				return LENGTH (mPlyWORD[mBody[element][property].mLine[line].m1st]) ;
			if (mBody[element][property].mType == PlyParserDataType::Char)
				return LENGTH (mPlyCHAR[mBody[element][property].mLine[line].m1st]) ;
			assume (FALSE) ;
			return ZERO ;
		}) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r3x = mReader.poll (TYPE<FLT32>::expr) ;
				mPlyCHAR.add (bitwise[TYPE<CHAR>::expr] (r3x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r4x = mReader.poll (TYPE<FLT64>::expr) ;
				mPlyQUAD.add (bitwise[TYPE<QUAD>::expr] (r4x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r5x = mReader.poll (TYPE<VAL32>::expr) ;
				mPlyCHAR.add (bitwise[TYPE<CHAR>::expr] (r5x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r6x = mReader.poll (TYPE<VAL64>::expr) ;
				mPlyQUAD.add (bitwise[TYPE<QUAD>::expr] (r6x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			INDEX ix = mPlyBYTE.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r7x = mReader.poll (TYPE<VAL64>::expr) ;
				assume (r7x >= 0) ;
				mPlyBYTE.add (BYTE (r7x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			INDEX ix = mPlyWORD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r8x = mReader.poll (TYPE<VAL64>::expr) ;
				assume (r8x >= 0) ;
				mPlyWORD.add (WORD (r8x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r9x = mReader.poll (TYPE<VAL64>::expr) ;
				assume (r9x >= 0) ;
				mPlyCHAR.add (CHAR (r9x)) ;
				mReader >> GAP ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			mReader >> GAP ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r10x = mReader.poll (TYPE<VAL64>::expr) ;
				assume (r10x >= 0) ;
				mPlyQUAD.add (QUAD (r10x)) ;
				mReader >> GAP ;
			}
		}
	}

	void read_body_binary () {
		mReader.use_byte () ;
		mBody = Array<Array<PlyParserBody>> (mHeader.mElementList.length ()) ;
		for (auto &&i : mHeader.mElementList.range ()) {
			mBody[i] = Array<PlyParserBody> (mHeader.mElementList[i].mPropertyList.length ()) ;
			for (auto &&j : mHeader.mElementList[i].mPropertyList.range ()) {
				mBody[i][j].mType = PlyParserDataType::Null ;
				mBody[i][j].mLine = Array<Tuple<INDEX ,INDEX>> (mHeader.mElementList[i].mSize) ;
			}
			for (auto &&j : iter (0 ,mHeader.mElementList[i].mSize)) {
				for (auto &&k : mHeader.mElementList[i].mPropertyList.range ()) {
					read_body_binary_item (i ,k ,j) ;
					read_body_binary_list (i ,k ,j) ;
				}
			}
		}
		mReader >> EOS ;
	}

	void read_body_binary_item (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
		const auto r1x = mHeader.mElementList[element].mPropertyList[property].mType ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r2x = mReader.poll (TYPE<FLT32>::expr) ;
			const auto r3x = bitwise[TYPE<CHAR>::expr] (r2x) ;
			mPlyCHAR.add (bitwise_reverse (r3x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r4x = mReader.poll (TYPE<FLT64>::expr) ;
			const auto r5x = bitwise[TYPE<QUAD>::expr] (r4x) ;
			mPlyQUAD.add (bitwise_reverse (r5x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r6x = mReader.poll (TYPE<VAL32>::expr) ;
			const auto r7x = bitwise[TYPE<CHAR>::expr] (r6x) ;
			mPlyCHAR.add (bitwise_reverse (r7x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r8x = mReader.poll (TYPE<VAL64>::expr) ;
			const auto r9x = bitwise[TYPE<QUAD>::expr] (r8x) ;
			mPlyQUAD.add (bitwise_reverse (r9x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			INDEX ix = mPlyBYTE.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r10x = mReader.poll (TYPE<BYTE>::expr) ;
			mPlyBYTE.add (bitwise_reverse (r10x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			INDEX ix = mPlyWORD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r11x = mReader.poll (TYPE<WORD>::expr) ;
			mPlyWORD.add (bitwise_reverse (r11x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r12x = mReader.poll (TYPE<CHAR>::expr) ;
			mPlyCHAR.add (bitwise_reverse (r12x)) ;
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + 2 ;
			const auto r13x = mReader.poll (TYPE<QUAD>::expr) ;
			mPlyQUAD.add (bitwise_reverse (r13x)) ;
		}
	}

	void read_body_binary_list (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
		const auto r1x = mHeader.mElementList[element].mPropertyList[property].mListType ;
		if (r1x == PlyParserDataType::Null)
			return ;
		const auto r2x = invoke ([&] () {
			if (mBody[element][property].mType == PlyParserDataType::Byte)
				return LENGTH (mPlyBYTE[mBody[element][property].mLine[line].m1st]) ;
			if (mBody[element][property].mType == PlyParserDataType::Word)
				return LENGTH (mPlyWORD[mBody[element][property].mLine[line].m1st]) ;
			if (mBody[element][property].mType == PlyParserDataType::Char)
				return LENGTH (mPlyCHAR[mBody[element][property].mLine[line].m1st]) ;
			assume (FALSE) ;
			return ZERO ;
		}) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r3x = mReader.poll (TYPE<FLT32>::expr) ;
				const auto r4x = bitwise[TYPE<CHAR>::expr] (r3x) ;
				mPlyCHAR.add (bitwise_reverse (r4x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Flt64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r5x = mReader.poll (TYPE<FLT64>::expr) ;
				const auto r6x = bitwise[TYPE<QUAD>::expr] (r5x) ;
				mPlyQUAD.add (bitwise_reverse (r6x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val32)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = PlyParserDataType::Char ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r7x = mReader.poll (TYPE<VAL32>::expr) ;
				const auto r8x = bitwise[TYPE<CHAR>::expr] (r7x) ;
				mPlyCHAR.add (bitwise_reverse (r8x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Val64)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = PlyParserDataType::Quad ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r9x = mReader.poll (TYPE<VAL64>::expr) ;
				const auto r10x = bitwise[TYPE<QUAD>::expr] (r9x) ;
				mPlyQUAD.add (bitwise_reverse (r10x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Byte)
				discard ;
			INDEX ix = mPlyBYTE.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r11x = mReader.poll (TYPE<BYTE>::expr) ;
				mPlyBYTE.add (bitwise_reverse (r11x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Word)
				discard ;
			INDEX ix = mPlyWORD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r12x = mReader.poll (TYPE<WORD>::expr) ;
				mPlyWORD.add (bitwise_reverse (r12x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Char)
				discard ;
			INDEX ix = mPlyCHAR.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r13x = mReader.poll (TYPE<CHAR>::expr) ;
				mPlyCHAR.add (bitwise_reverse (r13x)) ;
			}
		}
		if ifdo (act) {
			if (r1x != PlyParserDataType::Quad)
				discard ;
			INDEX ix = mPlyQUAD.tail () ;
			mBody[element][property].mType = r1x ;
			mBody[element][property].mLine[line].m1st = ix + 1 ;
			mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
			for (auto &&i : iter (0 ,r2x)) {
				noop (i) ;
				const auto r14x = mReader.poll (TYPE<QUAD>::expr) ;
				mPlyQUAD.add (bitwise_reverse (r14x)) ;
			}
		}
	}

	BYTE bitwise_reverse (CREF<BYTE> a) const {
		if ((!mBitwiseReverseFlag))
			return a ;
		return ByteProc::bit_reverse (a) ;
	}

	WORD bitwise_reverse (CREF<WORD> a) const {
		if ((!mBitwiseReverseFlag))
			return a ;
		return ByteProc::bit_reverse (a) ;
	}

	CHAR bitwise_reverse (CREF<CHAR> a) const {
		if ((!mBitwiseReverseFlag))
			return a ;
		return ByteProc::bit_reverse (a) ;
	}

	QUAD bitwise_reverse (CREF<QUAD> a) const {
		if ((!mBitwiseReverseFlag))
			return a ;
		return ByteProc::bit_reverse (a) ;
	}
} ;

class PlyParserImplHolder implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		auto rax = MakePlyParser (Ref<RefBuffer<BYTE>>::reference (Pointer::make (address (stream)))) ;
		fake.mThis = Ref<PlyParserImplLayout>::make (rax.poll ()) ;
		fake.mGuide.mElement = NONE ;
	}

	INDEX find_element (CREF<Slice> name) const override {
		return fake.mThis->mHeader.mElementSet.map (name) ;
	}

	LENGTH element_size (CREF<INDEX> element) const override {
		return fake.mThis->mHeader.mElementList[element].mSize ;
	}

	INDEX find_property (CREF<INDEX> element ,CREF<Slice> name) const override {
		return fake.mThis->mHeader.mElementList[element].mPropertySet.map (name) ;
	}

	LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const override {
		const auto r1x = fake.mThis->mBody[element][property].mLine[line] ;
		return r1x.m2nd - r1x.m1st ;
	}

	void guide_new (CREF<INDEX> element) override {
		fake.mGuide.mElement = element ;
		fake.mGuide.mProperty.clear () ;
		fake.mGuide.mPropertyIndex = 0 ;
		fake.mGuide.mLineIndex = 0 ;
		fake.mGuide.mPlyIndex = NONE ;
	}

	void guide_put (CREF<INDEX> property) override {
		assert (fake.mGuide.mElement != NONE) ;
		assert (fake.mGuide.mPlyIndex == NONE) ;
		fake.mGuide.mProperty.add (property) ;
	}

	void guide_jmp () {
		assert (fake.mGuide.mElement != NONE) ;
		INDEX ix = fake.mGuide.mElement ;
		INDEX jx = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mGuide.mPlyIndex != NONE)
				discard ;
			fake.mGuide.mLineIndex = 0 ;
			fake.mGuide.mPropertyIndex = 0 ;
			assume (fake.mGuide.mPropertyIndex < fake.mGuide.mProperty.length ()) ;
			assume (fake.mGuide.mLineIndex < fake.mThis->mHeader.mElementList[ix].mSize) ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			fake.mGuide.mPlyIndex = fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m1st ;
		}
		if ifdo (act) {
			fake.mGuide.mPlyIndex++ ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			if (fake.mGuide.mPlyIndex >= fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m2nd)
				discard ;
		}
		if ifdo (act) {
			fake.mGuide.mPropertyIndex++ ;
			if (fake.mGuide.mPropertyIndex >= fake.mGuide.mProperty.length ())
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			fake.mGuide.mPlyIndex = fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m1st ;
		}
		if ifdo (act) {
			fake.mGuide.mLineIndex++ ;
			fake.mGuide.mPropertyIndex = 0 ;
			if (fake.mGuide.mLineIndex >= fake.mThis->mHeader.mElementList[ix].mSize)
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			fake.mGuide.mPlyIndex = fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m1st ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		if ifdo (TRUE) {
			fake.mGuide.mPlyType = fake.mThis->mHeader.mElementList[ix].mPropertyList[jx].mListType ;
			if (fake.mGuide.mPlyType != PlyParserDataType::Null)
				discard ;
			fake.mGuide.mPlyType = fake.mThis->mHeader.mElementList[ix].mPropertyList[jx].mType ;
		}
	}

	void read (VREF<BOOL> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Bool) ;
		item = bitwise[TYPE<BOOL>::expr] (fake.mThis->mPlyBYTE[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<VAL32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Val32) ;
		item = bitwise[TYPE<VAL32>::expr] (fake.mThis->mPlyCHAR[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<VAL64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Val64) ;
		item = bitwise[TYPE<VAL64>::expr] (fake.mThis->mPlyQUAD[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<FLT32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Flt32) ;
		item = bitwise[TYPE<FLT32>::expr] (fake.mThis->mPlyCHAR[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<FLT64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Flt64) ;
		item = bitwise[TYPE<FLT64>::expr] (fake.mThis->mPlyQUAD[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<BYTE> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Byte) ;
		item = fake.mThis->mPlyBYTE[fake.mGuide.mPlyIndex] ;
	}

	void read (VREF<WORD> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Word) ;
		item = fake.mThis->mPlyWORD[fake.mGuide.mPlyIndex] ;
	}

	void read (VREF<CHAR> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Char) ;
		item = fake.mThis->mPlyCHAR[fake.mGuide.mPlyIndex] ;
	}

	void read (VREF<QUAD> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserDataType::Quad) ;
		item = fake.mThis->mPlyQUAD[fake.mGuide.mPlyIndex] ;
	}
} ;

exports VFat<PlyParserHolder> PlyParserHolder::create (VREF<PlyParserLayout> that) {
	return VFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::create (CREF<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplHolder () ,that) ;
}
} ;