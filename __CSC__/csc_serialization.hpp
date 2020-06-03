#pragma once

#ifndef __CSC_SERIALIZATION__
#define __CSC_SERIALIZATION__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
class SerializationStaticProc
	:private Wrapped<void> {
public:
	imports const String<STRU8> &static_empty_string () ;
} ;

inline exports const String<STRU8> &SerializationStaticProc::static_empty_string () {
	return _CACHE_ ([&] () {
		return String<STRU8> () ;
	}) ;
}

class XmlParser {
private:
	static constexpr auto NODE_CLAZZ_TABLE = EFLAG (1) ;
	static constexpr auto NODE_CLAZZ_OBJECT = EFLAG (2) ;
	static constexpr auto NODE_CLAZZ_ARRAY = EFLAG (3) ;
	static constexpr auto NODE_CLAZZ_FINAL = EFLAG (4) ;

	struct NODE {
		String<STRU8> mName ;
		Deque<String<STRU8>> mAttributeList ;
		SoftSet<String<STRU8>> mAttributeMappingSet ;
		SoftSet<INDEX> mMemberSet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

	struct Private {
		class RecursiveCounter ;

		class InitializeLambda ;

		class InitializeLambda2 ;
	} ;

private:
	SharedRef<FixedBuffer<NODE>> mHeap ;
	INDEX mIndex ;

public:
 	implicit XmlParser () {
		mIndex = VAR_NONE ;
	}

	BOOL exist () const {
		if (!mHeap.exist ())
			return FALSE ;
		if (mIndex == VAR_NONE)
			return FALSE ;
		return TRUE ;
	}

	XmlParser root () const {
		if (!exist ())
			return XmlParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return XmlParser (_COPY_ (mHeap) ,0) ;
	}

	XmlParser parent () const {
		if (!exist ())
			return XmlParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return XmlParser (_COPY_ (mHeap) ,mHeap.self[mIndex].mParent) ;
	}

	XmlParser brother () const {
		if (!exist ())
			return XmlParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return XmlParser (_COPY_ (mHeap) ,mHeap.self[mIndex].mBrother) ;
	}

	XmlParser child () const {
		if (!exist ())
			return XmlParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return XmlParser (_COPY_ (mHeap) ,mHeap.self[mIndex].mChild) ;
	}

	XmlParser child (const String<STRU8> &name) const {
		if (!exist ())
			return XmlParser (_COPY_ (mHeap) ,VAR_NONE) ;
		INDEX ix = mHeap.self[mIndex].mObjectSet.map (name) ;
		return XmlParser (_COPY_ (mHeap) ,ix) ;
	}

	Array<XmlParser> child_array () const {
		Array<XmlParser> ret ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			ret = Array<XmlParser> (mHeap.self[mIndex].mMemberSet.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet)
				ret[iw++] = XmlParser (_COPY_ (mHeap) ,i.sid) ;
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
		}
		return _MOVE_ (ret) ;
	}

	Array<XmlParser> child_array (const LENGTH &fixed_len) const {
		Array<XmlParser> ret = Array<XmlParser> (fixed_len) ;
		INDEX iw = 0 ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = XmlParser (_COPY_ (mHeap) ,i.sid) ;
			}
		}
		return _MOVE_ (ret) ;
	}

	BOOL equal (const XmlParser &that) const {
		if (!exist ())
			if (!that.exist ())
				return TRUE ;
		if (!exist ())
			return FALSE ;
		if (!that.exist ())
			return FALSE ;
		if (DEPTR[mHeap.self] != DEPTR[that.mHeap.self])
			return FALSE ;
		if (mIndex != that.mIndex)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const XmlParser &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const XmlParser &that) const {
		return !equal (that) ;
	}

	const String<STRU8> &name () const leftvalue {
		_DYNAMIC_ASSERT_ (exist ()) ;
		return mHeap.self[mIndex].mName ;
	}

	const String<STRU8> &attribute (const String<STRU8> &tag) const leftvalue {
		if (!exist ())
			return SerializationStaticProc::static_empty_string () ;
		INDEX ix = mHeap.self[mIndex].mAttributeMappingSet.map (tag) ;
		if (ix == VAR_NONE)
			return SerializationStaticProc::static_empty_string () ;
		return mHeap.self[mIndex].mAttributeList[ix] ;
	}

	template <class _ARG1>
	REMOVE_CVR_TYPE<_ARG1> attribute (const String<STRU8> &tag ,const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		REMOVE_CVR_TYPE<_ARG1> ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (attribute (tag)) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return _MOVE_ (ret) ;
	}

	BOOL attribute (const String<STRU8> &tag ,const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (StringProc::parse_bools<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAR32 attribute (const String<STRU8> &tag ,const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (StringProc::parse_var32s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAR64 attribute (const String<STRU8> &tag ,const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (StringProc::parse_var64s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL32 attribute (const String<STRU8> &tag ,const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (StringProc::parse_val32s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL64 attribute (const String<STRU8> &tag ,const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (StringProc::parse_val64s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU8> attribute (const String<STRU8> &tag ,const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (_COPY_<String<STRU8>>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU16> attribute (const String<STRU8> &tag ,const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (StringProc::cvt_u8s_u16s) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU32> attribute (const String<STRU8> &tag ,const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (StringProc::cvt_u8s_u32s) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRA> attribute (const String<STRU8> &tag ,const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (StringProc::cvt_u8s_as) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRW> attribute (const String<STRU8> &tag ,const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (StringProc::cvt_u8s_ws) ;
		return attribute (tag ,def ,r1x) ;
	}

	PTR<const VOID> attribute (const String<STRU8> & ,const PTR<const VOID> &) const = delete ;

	const String<STRU8> &value () const leftvalue {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mMemberSet.size () == 0) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mAttributeMappingSet.length () == 1) ;
		INDEX ix = mHeap.self[mIndex].mAttributeList.head () ;
		return mHeap.self[mIndex].mAttributeList[ix] ;
	}

	template <class _ARG1>
	REMOVE_CVR_TYPE<_ARG1> value (const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		REMOVE_CVR_TYPE<_ARG1> ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (value ()) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return _MOVE_ (ret) ;
	}

	BOOL value (const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (StringProc::parse_bools<STRU8>) ;
		return value (def ,r1x) ;
	}

	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (StringProc::parse_var32s<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAR64 value (const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (StringProc::parse_var64s<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL32 value (const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (StringProc::parse_val32s<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL64 value (const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (StringProc::parse_val64s<STRU8>) ;
		return value (def ,r1x) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (_COPY_<String<STRU8>>) ;
		return value (def ,r1x) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (StringProc::cvt_u8s_u16s) ;
		return value (def ,r1x) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (StringProc::cvt_u8s_u32s) ;
		return value (def ,r1x) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (StringProc::cvt_u8s_as) ;
		return value (def ,r1x) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (StringProc::cvt_u8s_ws) ;
		return value (def ,r1x) ;
	}

	void friend_write (TextWriter<STRU8> &writer) const ;

public:
	imports XmlParser make (const PhanBuffer<const STRU8> &data) {
		XmlParser ret ;
		ret.initialize (data) ;
		return _MOVE_ (ret) ;
	}

	imports XmlParser make (const Array<XmlParser> &sequence) {
		XmlParser ret ;
		ret.initialize (sequence) ;
		return _MOVE_ (ret) ;
	}

private:
	explicit XmlParser (SharedRef<FixedBuffer<NODE>> &&heap ,const INDEX &index) {
		mHeap = _MOVE_ (heap) ;
		mIndex = index ;
	}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;

	void initialize (const Array<XmlParser> &sequence) ;
} ;

class XmlParser::Private::RecursiveCounter
	:private Wrapped<LENGTH> {
public:
	void lock () {
		_DYNAMIC_ASSERT_ (RecursiveCounter::mSelf <= DEFAULT_RECURSIVE_SIZE::value) ;
		RecursiveCounter::mSelf++ ;
	}

	void unlock () {
		RecursiveCounter::mSelf-- ;
	}
} ;

class XmlParser::Private::InitializeLambda
	:private Proxy {
private:
	XmlParser &mContext ;

	TextReader<STRU8> mTextReader ;
	RegularReader mRis ;
	LENGTH mRecursiveCounter ;
	INDEX mLatestIndex ;
	String<STRU8> mLatestString ;

	SoftSet<String<STRU8>> mAttributeMappingSoftSet ;
	SoftSet<INDEX> mMemberSoftSet ;
	SoftSet<String<STRU8>> mObjectSoftSet ;
	Allocator<NODE ,SAUTO> mNodeHeap ;
	SharedRef<FixedBuffer<NODE>> mHeap ;
	INDEX mRoot ;

public:
	explicit InitializeLambda (XmlParser &context_ ,const PhanBuffer<const STRU8> &data)
		: mContext (context_) ,mTextReader (data) {}

	inline void operator() () {
		prepare () ;
		generate () ;
		refresh () ;
	}

private:
	void prepare () {
		mRis = RegularReader (PhanRef<TextReader<STRU8>>::make (mTextReader) ,2) ;
		mRecursiveCounter = 0 ;
		mAttributeMappingSoftSet = SoftSet<String<STRU8>> (0) ;
		mMemberSoftSet = SoftSet<INDEX> (0) ;
		mObjectSoftSet = SoftSet<String<STRU8>> (0) ;
		mNodeHeap = Allocator<NODE ,SAUTO> () ;
		mHeap = SharedRef<FixedBuffer<NODE>> () ;
		mRoot = VAR_NONE ;
	}

	void generate () {
		/*
		*	$0->$8 $7 $9
		*	$1->${identity}
		*	$2->"${string}"
		*	$3->$1 = $2
		*	$4->${eps}|$3 $4
		*	$5-><$1 $4 />|<$1 $4 > $7 </$1 >
		*	$6-><!--comment-->
		*	$7->${eps}|$5 $7|$6 $7
		*	$8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		*	$9->${end}
		*/
		update_shift_e0 () ;
		_DEBUG_ASSERT_ (mLatestIndex == 0) ;
		mRoot = mLatestIndex ;
		update_heap () ;
	}

	//@info: $0->$8 $7 $9
	void update_shift_e0 () {
		update_shift_e8 () ;
		mRis >> RegularReader::SKIP_GAP ;
		INDEX ix = mNodeHeap.alloc () ;
		mNodeHeap[ix].mMemberSet = mMemberSoftSet.share () ;
		mNodeHeap[ix].mObjectSet = mObjectSoftSet.share () ;
		mNodeHeap[ix].mParent = VAR_NONE ;
		update_shift_e7 (ix) ;
		mNodeHeap[ix].mChild = mLatestIndex ;
		mNodeHeap[ix].mBrother = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e9 () ;
		mLatestIndex = ix ;
	}

	//@info: $1->${identity}
	void update_shift_e1 () {
		mRis >> RegularReader::HINT_IDENTIFIER >> mLatestString ;
	}

	//@info: $2->"${string}"
	void update_shift_e2 () {
		mRis >> RegularReader::HINT_STRING >> mLatestString ;
	}

	//@info: $3->$1 = $2
	void update_shift_e3 (const INDEX &curr) {
		update_shift_e1 () ;
		INDEX ix = mNodeHeap[curr].mAttributeMappingSet.map (mLatestString) ;
		_DYNAMIC_ASSERT_ (ix == VAR_NONE) ;
		ix = mNodeHeap[curr].mAttributeList.insert () ;
		mNodeHeap[curr].mAttributeMappingSet.add (_MOVE_ (mLatestString) ,ix) ;
		mRis >> RegularReader::SKIP_GAP ;
		mRis >> _PCSTRU8_ ("=") ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e2 () ;
		mNodeHeap[curr].mAttributeList[ix] = _MOVE_ (mLatestString) ;
	}

	//@info: $4->${eps}|$3 $4
	void update_shift_e4 (const INDEX &curr) {
		while (TRUE) {
			if (!(mRis[0] >= STRU8 ('A') && mRis[0] <= STRU8 ('Z')))
				if (!(mRis[0] >= STRU8 ('a') && mRis[0] <= STRU8 ('z')))
					if (!(mRis[0] == STRU8 ('_')))
						break ;
			update_shift_e3 (curr) ;
			mRis >> RegularReader::SKIP_GAP ;
		}
	}

	//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
	void update_shift_e5 (const INDEX &curr) {
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::null ,mRecursiveCounter)) ;
		mRis >> _PCSTRU8_ ("<") ;
		INDEX ix = mNodeHeap.alloc () ;
		update_shift_e1 () ;
		mNodeHeap[ix].mName = _MOVE_ (mLatestString) ;
		mNodeHeap[ix].mAttributeList = Deque<String<STRU8>> () ;
		mNodeHeap[ix].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
		mNodeHeap[ix].mParent = curr ;
		mNodeHeap[ix].mBrother = VAR_NONE ;
		mNodeHeap[ix].mChild = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e4 (ix) ;
		mRis >> RegularReader::SKIP_GAP ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('>')))
				discard ;
			mRis++ ;
			mRis >> RegularReader::SKIP_GAP ;
			mNodeHeap[ix].mMemberSet = mMemberSoftSet.share () ;
			mNodeHeap[ix].mObjectSet = mObjectSoftSet.share () ;
			update_shift_e7 (ix) ;
			mNodeHeap[ix].mChild = mLatestIndex ;
			mRis >> RegularReader::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("</") ;
			update_shift_e1 () ;
			_DYNAMIC_ASSERT_ (mNodeHeap[ix].mName == mLatestString) ;
			mRis >> RegularReader::SKIP_GAP ;
			mRis >> _PCSTRU8_ (">") ;
		}
		if switch_once (fax) {
			mRis >> _PCSTRU8_ ("/>") ;
		}
		mLatestIndex = ix ;
	}

	//@info: $6-><!--${comment}-->
	void update_shift_e6 () {
		mRis >> _PCSTRU8_ ("<!--") ;
		while (TRUE) {
			if (mRis[0] == STRU8 ('\0'))
				break ;
			if (mRis[0] == STRU8 ('-'))
				if (mRis[1] == STRU8 ('-'))
					break ;
			mRis++ ;
		}
		mRis >> _PCSTRU8_ ("-->") ;
	}

	//@info: $7->${eps}|$5 $7|$6 $7
	void update_shift_e7 (const INDEX &curr) {
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::null ,mRecursiveCounter)) ;
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		while (TRUE) {
			const auto r1x = BOOL (mRis[0] == STRU8 ('<') && mRis[1] == STRU8 ('!')) ;
			const auto r2x = BOOL (mRis[0] == STRU8 ('<') && mRis[1] != STRU8 ('/')) ;
			if (!r1x && !r2x)
				break ;
			auto fax = TRUE ;
			if switch_once (fax) {
				if (!r1x)
					discard ;
				update_shift_e6 () ;
			}
			if switch_once (fax) {
				if (!r2x)
					discard ;
				update_shift_e5 (curr) ;
				const auto r3x = mNodeHeap[curr].mMemberSet.length () ;
				mNodeHeap[curr].mMemberSet.add (r3x ,mLatestIndex) ;
				mNodeHeap[curr].mObjectSet.add (mNodeHeap[mLatestIndex].mName ,mLatestIndex) ;
				auto &r4x = _SWITCH_ (
					(ix == VAR_NONE) ? ix :
					mNodeHeap[iy].mBrother) ;
				r4x = mLatestIndex ;
				iy = mLatestIndex ;
			}
			mRis >> RegularReader::SKIP_GAP ;
		}
		mLatestIndex = ix ;
	}

	//@info: $8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
	void update_shift_e8 () {
		if (mRis[0] != STRU8 ('<'))
			return ;
		if (mRis[1] != STRU8 ('?'))
			return ;
		mRis++ ;
		mRis++ ;
		mRis >> _PCSTRU8_ ("xml") ;
		mRis >> RegularReader::SKIP_GAP ;
		mRis >> _PCSTRU8_ ("version") ;
		mRis >> RegularReader::SKIP_GAP ;
		mRis >> _PCSTRU8_ ("=") ;
		mRis >> RegularReader::SKIP_GAP ;
		mRis >> _PCSTRU8_ ("\"1.0\"") ;
		mRis >> RegularReader::SKIP_GAP ;
		if switch_once (TRUE) {
			if (mRis[0] == STRU8 ('?'))
				discard ;
			mRis >> _PCSTRU8_ ("encoding") ;
			mRis >> RegularReader::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("=") ;
			mRis >> RegularReader::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("\"utf-8\"") ;
			mRis >> RegularReader::SKIP_GAP ;
		}
		mRis >> _PCSTRU8_ ("?>") ;
	}

	//@info: $9->${end}
	void update_shift_e9 () {
		_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
	}

	void update_heap () {
		mAttributeMappingSoftSet.clean () ;
		mObjectSoftSet.clean () ;
		mHeap = SharedRef<FixedBuffer<NODE>>::make (mNodeHeap.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : _RANGE_ (0 ,mNodeHeap.size ())) {
			if (!mNodeHeap.used (i))
				continue ;
			mHeap.self[iw++] = _MOVE_ (mNodeHeap[i]) ;
		}
		_DEBUG_ASSERT_ (iw == mHeap->size ()) ;
	}

	void refresh () {
		mContext.mHeap = _MOVE_ (mHeap) ;
		mContext.mIndex = mRoot ;
	}
} ;

class XmlParser::Private::InitializeLambda2
	:private Proxy {
private:
	struct FOUND_NODE {
		String<STRU8> mName ;
		Deque<String<STRU8>> mAttributeList ;
		SoftSet<String<STRU8>> mAttributeMappingSet ;
		EFLAG mClazz ;
		Deque<XmlParser> mBaseNodeList ;
	} ;

	struct STACK_NODE {
		INDEX mParent ;
		EFLAG mClazz ;
		Deque<XmlParser> mBaseNodeList ;
	} ;

private:
	XmlParser &mContext ;
	const Array<XmlParser> &mSequence ;
	const String<STRU8> mClazzString ;
	const String<STRU8> mTableClazzString ;
	const String<STRU8> mObjectClazzString ;
	const String<STRU8> mArrayClazzString ;
	const String<STRU8> mFinalClazzString ;

	Deque<STACK_NODE> mNodeStack ;
	Array<Function<MEMPTR<void (const XmlParser &)>>> mFoundNodeProc ;
	Set<EFLAG> mFoundNodeProcMappingSet ;
	SoftSet<String<STRU8>> mAttributeMappingSoftSet ;
	SoftSet<INDEX> mMemberSoftSet ;
	SoftSet<String<STRU8>> mObjectSoftSet ;
	Allocator<NODE ,SAUTO> mNodeHeap ;
	Deque<FOUND_NODE> mFoundNodeList ;
	Set<String<STRU8>> mFoundNodeMappingSet ;
	SharedRef<FixedBuffer<NODE>> mHeap ;
	INDEX mRoot ;

	Deque<Deque<XmlParser>> mFoundNodeBaseNodeQueue ;
	STACK_NODE mTempNode ;

public:
	explicit InitializeLambda2 (XmlParser &context_ ,const Array<XmlParser> &sequence)
		: mContext (context_) ,mSequence (sequence) ,mClazzString (_PCSTRU8_ ("type")) ,mTableClazzString (_PCSTRU8_ ("table")) ,mObjectClazzString (_PCSTRU8_ ("object")) ,mArrayClazzString (_PCSTRU8_ ("array")) ,mFinalClazzString (_PCSTRU8_ ("final")) {}

	inline void operator() () {
		prepare () ;
		generate () ;
		refresh () ;
	}

private:
	void prepare () {
		mNodeStack = Deque<STACK_NODE> () ;
		//@error: fuck g++4.8
		mFoundNodeProc = Array<Function<MEMPTR<void (const XmlParser &)>>> (3) ;
		mFoundNodeProcMappingSet.add (EFLAG (NODE_CLAZZ_TABLE) ,0) ;
		mFoundNodeProcMappingSet.add (EFLAG (NODE_CLAZZ_OBJECT) ,1) ;
		mFoundNodeProcMappingSet.add (EFLAG (NODE_CLAZZ_ARRAY) ,2) ;
		mFoundNodeProcMappingSet.add (EFLAG (NODE_CLAZZ_FINAL) ,0) ;
		mFoundNodeProc[0] = Function<MEMPTR<void (const XmlParser &)>> (PhanRef<InitializeLambda2>::make (DEREF[this]) ,&InitializeLambda2::update_found_table_node) ;
		mFoundNodeProc[1] = Function<MEMPTR<void (const XmlParser &)>> (PhanRef<InitializeLambda2>::make (DEREF[this]) ,&InitializeLambda2::update_found_object_node) ;
		mFoundNodeProc[2] = Function<MEMPTR<void (const XmlParser &)>> (PhanRef<InitializeLambda2>::make (DEREF[this]) ,&InitializeLambda2::update_found_array_node) ;
		mAttributeMappingSoftSet = SoftSet<String<STRU8>> (0) ;
		mMemberSoftSet = SoftSet<INDEX> (0) ;
		mObjectSoftSet = SoftSet<String<STRU8>> (0) ;
		mNodeHeap = Allocator<NODE ,SAUTO> () ;
		mRoot = mNodeHeap.alloc () ;
		mNodeHeap[mRoot].mMemberSet = mMemberSoftSet.share () ;
		mNodeHeap[mRoot].mObjectSet = mObjectSoftSet.share () ;
		mNodeHeap[mRoot].mParent = VAR_NONE ;
		mNodeHeap[mRoot].mChild = VAR_NONE ;
		mNodeHeap[mRoot].mBrother = VAR_NONE ;
		mFoundNodeList = Deque<FOUND_NODE> () ;
	}

	EFLAG node_type (const XmlParser &node) const {
		auto &r1x = node.attribute (mClazzString) ;
		if (r1x == mTableClazzString)
			return NODE_CLAZZ_TABLE ;
		if (r1x == mObjectClazzString)
			return NODE_CLAZZ_OBJECT ;
		if (r1x == mArrayClazzString)
			return NODE_CLAZZ_ARRAY ;
		if (r1x == mFinalClazzString)
			return NODE_CLAZZ_FINAL ;
		_DYNAMIC_ASSERT_ (r1x.empty ()) ;
		return NODE_CLAZZ_TABLE ;
	}

	void generate () {
		update_root_node () ;
		while (TRUE) {
			if (mNodeStack.empty ())
				break ;
			mTempNode = _MOVE_ (mNodeStack[mNodeStack.tail ()]) ;
			mNodeStack.pop () ;
			for (auto &&i : mTempNode.mBaseNodeList) {
				INDEX ix = mFoundNodeProcMappingSet.map (mTempNode.mClazz) ;
				mFoundNodeProc[ix] (i) ;
			}
			update_merge_found_node (mTempNode.mParent) ;
			mFoundNodeBaseNodeQueue.add (_MOVE_ (mTempNode.mBaseNodeList)) ;
		}
		update_heap () ;
	}

	void update_root_node () {
		INDEX ix = find_normal_node () ;
		if (ix == VAR_NONE)
			return ;
		INDEX jx = mNodeStack.insert () ;
		mNodeStack[jx].mBaseNodeList = Deque<XmlParser> (mSequence.length ()) ;
		for (auto &&i : mSequence) {
			if (!i.exist ())
				continue ;
			mNodeStack[jx].mBaseNodeList.add (i) ;
		}
		mNodeStack[jx].mClazz = NODE_CLAZZ_OBJECT ;
		mNodeStack[jx].mParent = mRoot ;
	}

	INDEX find_normal_node () const {
		for (auto &&i : mSequence)
			if (i.exist ())
				return mSequence.at (i) ;
		return VAR_NONE ;
	}

	void update_found_table_node (const XmlParser &node) {
		for (XmlParser i = node ,it ; i.exist () ; i = it) {
			it = i.brother () ;
			INDEX ix = mFoundNodeList.insert () ;
			mFoundNodeList[ix].mName = i.name () ;
			mFoundNodeList[ix].mClazz = node_type (i) ;
			mFoundNodeList[ix].mAttributeList = Deque<String<STRU8>> () ;
			mFoundNodeList[ix].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
			mFoundNodeList[ix].mAttributeList.appand (i.mHeap.self[i.mIndex].mAttributeList) ;
			mFoundNodeList[ix].mAttributeMappingSet.appand (i.mHeap.self[i.mIndex].mAttributeMappingSet) ;
			if (mFoundNodeBaseNodeQueue.empty ())
				mFoundNodeBaseNodeQueue.add (Deque<XmlParser> ()) ;
			mFoundNodeBaseNodeQueue.take (mFoundNodeList[ix].mBaseNodeList) ;
			mFoundNodeList[ix].mBaseNodeList.clear () ;
			mFoundNodeList[ix].mBaseNodeList.add (i.child ()) ;
		}
	}

	void update_found_object_node (const XmlParser &node) {
		for (XmlParser i = node ,it ; i.exist () ; i = it) {
			it = i.brother () ;
			const auto r1x = i.name () ;
			const auto r2x = node_type (i) ;
			INDEX ix = mFoundNodeMappingSet.map (r1x) ;
			if switch_once (TRUE) {
				if (ix == VAR_NONE)
					discard ;
				_DYNAMIC_ASSERT_ (mFoundNodeList[ix].mClazz == r2x) ;
				_DYNAMIC_ASSERT_ (r2x != NODE_CLAZZ_FINAL) ;
			}
			INDEX iy = ix ;
			if switch_once (TRUE) {
				if (ix != VAR_NONE)
					discard ;
				iy = mFoundNodeList.insert () ;
				mFoundNodeMappingSet.add (r1x ,iy) ;
				mFoundNodeList[iy].mName = r1x ;
				mFoundNodeList[iy].mClazz = r2x ;
				mFoundNodeList[iy].mAttributeList = Deque<String<STRU8>> () ;
				mFoundNodeList[iy].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
				if (mFoundNodeBaseNodeQueue.empty ())
					mFoundNodeBaseNodeQueue.add (Deque<XmlParser> ()) ;
				mFoundNodeBaseNodeQueue.take (mFoundNodeList[iy].mBaseNodeList) ;
				mFoundNodeList[iy].mBaseNodeList.clear () ;
			}
			for (auto &&j : i.mHeap.self[i.mIndex].mAttributeMappingSet) {
				INDEX jx = mFoundNodeList[iy].mAttributeMappingSet.map (j.key) ;
				if switch_once (TRUE) {
					if (jx != VAR_NONE)
						discard ;
					jx = mFoundNodeList[iy].mAttributeList.insert () ;
					mFoundNodeList[iy].mAttributeMappingSet.add (j.key ,jx) ;
					mFoundNodeList[iy].mAttributeList[jx] = i.mHeap.self[i.mIndex].mAttributeList[j.sid] ;
				}
			}
			mFoundNodeList[iy].mBaseNodeList.add (i.child ()) ;
		}
	}

	void update_found_array_node (const XmlParser &node) {
		for (XmlParser i = node ,it ; i.exist () ; i = it) {
			it = i.brother () ;
			const auto r1x = i.name () ;
			const auto r2x = node_type (i) ;
			INDEX ix = mFoundNodeList.head () ;
			if switch_once (TRUE) {
				if (ix == VAR_NONE)
					discard ;
				_DYNAMIC_ASSERT_ (mFoundNodeList[ix].mName == r1x) ;
				_DYNAMIC_ASSERT_ (r2x != NODE_CLAZZ_FINAL) ;
			}
			INDEX iy = mFoundNodeList.insert () ;
			mFoundNodeMappingSet.add (r1x ,iy) ;
			mFoundNodeList[iy].mName = r1x ;
			mFoundNodeList[iy].mClazz = r2x ;
			mFoundNodeList[iy].mAttributeList = Deque<String<STRU8>> () ;
			mFoundNodeList[iy].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
			mFoundNodeList[iy].mAttributeList.appand (i.mHeap.self[i.mIndex].mAttributeList) ;
			mFoundNodeList[iy].mAttributeMappingSet.appand (i.mHeap.self[i.mIndex].mAttributeMappingSet) ;
			if (mFoundNodeBaseNodeQueue.empty ())
				mFoundNodeBaseNodeQueue.add (Deque<XmlParser> ()) ;
			mFoundNodeBaseNodeQueue.take (mFoundNodeList[iy].mBaseNodeList) ;
			mFoundNodeList[iy].mBaseNodeList.clear () ;
			mFoundNodeList[iy].mBaseNodeList.add (i.child ()) ;
		}
	}

	void update_merge_found_node (const INDEX &curr) {
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		for (auto &&i : mFoundNodeList) {
			iy = ix ;
			ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mName = _MOVE_ (i.mName) ;
			mNodeHeap[ix].mAttributeList = _MOVE_ (i.mAttributeList) ;
			mNodeHeap[ix].mAttributeMappingSet = _MOVE_ (i.mAttributeMappingSet) ;
			mNodeHeap[ix].mParent = curr ;
			if switch_once (TRUE) {
				INDEX jx = mNodeHeap[ix].mParent ;
				if (jx == VAR_NONE)
					discard ;
				if (mNodeHeap[jx].mChild == VAR_NONE)
					mNodeHeap[jx].mChild = ix ;
				if switch_once (TRUE) {
					if (mNodeHeap[jx].mMemberSet.size () > 0)
						discard ;
					mNodeHeap[jx].mMemberSet = mMemberSoftSet.share () ;
					mNodeHeap[jx].mObjectSet = mObjectSoftSet.share () ;
				}
				const auto r1x = mNodeHeap[jx].mMemberSet.length () ;
				mNodeHeap[jx].mMemberSet.add (r1x ,ix) ;
				mNodeHeap[jx].mObjectSet.add (mNodeHeap[ix].mName ,ix) ;
			}
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			if (iy != VAR_NONE)
				mNodeHeap[iy].mBrother = ix ;
			if (mRoot == VAR_NONE)
				mRoot = ix ;
			INDEX jy = mNodeStack.insert () ;
			mNodeStack[jy].mBaseNodeList = _MOVE_ (i.mBaseNodeList) ;
			mNodeStack[jy].mClazz = i.mClazz ;
			mNodeStack[jy].mParent = ix ;
		}
		mFoundNodeMappingSet.clear () ;
		mFoundNodeList.clear () ;
	}

	void update_heap () {
		mAttributeMappingSoftSet.clean () ;
		mObjectSoftSet.clean () ;
		mHeap = SharedRef<FixedBuffer<NODE>>::make (mNodeHeap.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : _RANGE_ (0 ,mNodeHeap.size ())) {
			if (!mNodeHeap.used (i))
				continue ;
			mHeap.self[iw++] = _MOVE_ (mNodeHeap[i]) ;
		}
		_DEBUG_ASSERT_ (iw == mHeap->size ()) ;
	}

	void refresh () {
		mContext.mHeap = _MOVE_ (mHeap) ;
		mContext.mIndex = mRoot ;
	}
} ;

inline exports void XmlParser::friend_write (TextWriter<STRU8> &writer) const {
	static constexpr auto M_NODE_X1 = EFLAG (1) ;
	static constexpr auto M_NODE_X2 = EFLAG (2) ;
	auto rax = Deque<PACK<INDEX ,EFLAG>> () ;
	auto rbx = Deque<PACK<INDEX ,EFLAG>> () ;
	rax.add (PACK<INDEX ,EFLAG> {mIndex ,M_NODE_X1}) ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		const auto r1x = rax[rax.tail ()] ;
		rax.pop () ;
		auto fax = TRUE ;
		if switch_once (fax) {
			//@info: case '<?xml ...>'
			if (!(r1x.mP1 != VAR_NONE))
				discard ;
			if (!mHeap.self[r1x.mP1].mName.empty ())
				discard ;
			if (!(r1x.mP2 == M_NODE_X1))
				discard ;
			auto &r2x = mHeap.self[r1x.mP1] ;
			writer << _PCSTRU8_ ("<?xml version=\"1.0\" encoding=\"utf-8\" ?>") ;
			writer << TextWriter<STRU8>::GAP ;
			rbx.clear () ;
			for (INDEX i = r2x.mChild ,it ; i != VAR_NONE ; i = it) {
				it = mHeap.self[i].mBrother ;
				rbx.add (PACK<INDEX ,EFLAG> {i ,M_NODE_X1}) ;
			}
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_once (fax) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?/>'
			if (!(r1x.mP1 != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x.mP1].mChild == VAR_NONE))
				discard ;
			if (!(r1x.mP2 == M_NODE_X1))
				discard ;
			auto &r3x = mHeap.self[r1x.mP1] ;
			writer << _PCSTRU8_ ("<") << r3x.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r3x.mAttributeMappingSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << r3x.mAttributeList[i.sid] << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ ("/>") ;
		}
		if switch_once (fax) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?>'
			if (!(r1x.mP1 != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x.mP1].mChild != VAR_NONE))
				discard ;
			if (!(r1x.mP2 == M_NODE_X1))
				discard ;
			auto &r4x = mHeap.self[r1x.mP1] ;
			writer << _PCSTRU8_ ("<") << r4x.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r4x.mAttributeMappingSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << r4x.mAttributeList[i.sid] << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ (">") ;
			rbx.clear () ;
			for (INDEX i = r4x.mChild ,it ; i != VAR_NONE ; i = it) {
				it = mHeap.self[i].mBrother ;
				rbx.add (PACK<INDEX ,EFLAG> {i ,M_NODE_X1}) ;
			}
			rbx.add (PACK<INDEX ,EFLAG> {r1x.mP1 ,M_NODE_X2}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_once (fax) {
			//@info: case '</xxx>'
			if (!(r1x.mP1 != VAR_NONE))
				discard ;
			if (!(r1x.mP2 == M_NODE_X2))
				discard ;
			writer << _PCSTRU8_ ("</") << mHeap.self[r1x.mP1].mName << _PCSTRU8_ (">") ;
		}
	}
}

inline exports void XmlParser::initialize (const PhanBuffer<const STRU8> &data) {
	struct Dependent ;
	using InitializeLambda = typename DEPENDENT_TYPE<Private ,Dependent>::InitializeLambda ;
	_CALL_ (InitializeLambda (DEREF[this] ,data)) ;
}

inline exports void XmlParser::initialize (const Array<XmlParser> &sequence) {
	struct Dependent ;
	using InitializeLambda2 = typename DEPENDENT_TYPE<Private ,Dependent>::InitializeLambda2 ;
	_CALL_ (InitializeLambda2 (DEREF[this] ,sequence)) ;
}

class JsonParser {
private:
	static constexpr auto NODE_CLAZZ_NULL = EFLAG (1) ;
	static constexpr auto NODE_CLAZZ_STRING = EFLAG (2) ;
	static constexpr auto NODE_CLAZZ_ARRAY = EFLAG (3) ;
	static constexpr auto NODE_CLAZZ_OBJECT = EFLAG (4) ;

	struct NODE {
		AnyRef<void> mValue ;
		EFLAG mClazz ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

	struct Private {
		class RecursiveCounter ;

		class InitializeLambda ;
	} ;

private:
	SharedRef<FixedBuffer<NODE>> mHeap ;
	INDEX mIndex ;

public:
 	implicit JsonParser () {
		mIndex = VAR_NONE ;
	}

	BOOL exist () const {
		if (!mHeap.exist ())
			return FALSE ;
		if (mIndex == VAR_NONE)
			return FALSE ;
		if (mHeap.self[mIndex].mClazz == NODE_CLAZZ_NULL)
			return FALSE ;
		return TRUE ;
	}

	BOOL string_type () const {
		if (mHeap.self[mIndex].mClazz != NODE_CLAZZ_STRING)
			return FALSE ;
		return TRUE ;
	}

	BOOL array_type () const {
		if (mHeap.self[mIndex].mClazz != NODE_CLAZZ_ARRAY)
			return FALSE ;
		return TRUE ;
	}

	BOOL object_type () const {
		if (mHeap.self[mIndex].mClazz != NODE_CLAZZ_OBJECT)
			return FALSE ;
		return TRUE ;
	}

	JsonParser root () const {
		if (!exist ())
			return JsonParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return JsonParser (_COPY_ (mHeap) ,0) ;
	}

	JsonParser parent () const {
		if (!exist ())
			return JsonParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return JsonParser (_COPY_ (mHeap) ,mHeap.self[mIndex].mParent) ;
	}

	JsonParser brother () const {
		if (!exist ())
			return JsonParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return JsonParser (_COPY_ (mHeap) ,mHeap.self[mIndex].mBrother) ;
	}

	JsonParser child () const {
		if (!exist ())
			return JsonParser (_COPY_ (mHeap) ,VAR_NONE) ;
		return JsonParser (_COPY_ (mHeap) ,mHeap.self[mIndex].mChild) ;
	}

	JsonParser child (const String<STRU8> &key) const {
		if (!exist ())
			return JsonParser (_COPY_ (mHeap) ,VAR_NONE) ;
		if (!object_type ())
			return JsonParser (_COPY_ (mHeap) ,VAR_NONE) ;
		auto &r1x = mHeap.self[mIndex].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::null).self ;
		INDEX ix = r1x.map (key) ;
		return JsonParser (_COPY_ (mHeap) ,ix) ;
	}

	Array<JsonParser> child_array () const {
		Array<JsonParser> ret ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1x = mHeap.self[mIndex].mValue.rebind (ARGV<SoftSet<INDEX>>::null).self ;
			ret = Array<JsonParser> (r1x.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : r1x)
				ret[iw++] = JsonParser (_COPY_ (mHeap) ,i.sid) ;
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
		}
		return _MOVE_ (ret) ;
	}

	Array<JsonParser> child_array (const LENGTH &fixed_len) const {
		Array<JsonParser> ret = Array<JsonParser> (fixed_len) ;
		INDEX iw = 0 ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1x = mHeap.self[mIndex].mValue.rebind (ARGV<SoftSet<INDEX>>::null).self ;
			for (auto &&i : r1x) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = JsonParser (_COPY_ (mHeap) ,i.sid) ;
			}
		}
		return _MOVE_ (ret) ;
	}

	BOOL equal (const JsonParser &that) const {
		if (!exist ())
			if (!that.exist ())
				return TRUE ;
		if (!exist ())
			return FALSE ;
		if (!that.exist ())
			return FALSE ;
		if (DEPTR[mHeap.self] != DEPTR[that.mHeap.self])
			return FALSE ;
		if (mIndex != that.mIndex)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const JsonParser &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const JsonParser &that) const {
		return !equal (that) ;
	}

	const String<STRU8> &value () const leftvalue {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (string_type ()) ;
		return mHeap.self[mIndex].mValue.rebind (ARGV<String<STRU8>>::null).self ;
	}

	template <class _ARG1>
	REMOVE_CVR_TYPE<_ARG1> value (const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		REMOVE_CVR_TYPE<_ARG1> ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (value ()) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return _MOVE_ (ret) ;
	}

	BOOL value (const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (StringProc::parse_bools<STRU8>) ;
		return value (def ,r1x) ;
	}

	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (StringProc::parse_var32s<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAR64 value (const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (StringProc::parse_var64s<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL32 value (const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (StringProc::parse_val32s<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL64 value (const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (StringProc::parse_val64s<STRU8>) ;
		return value (def ,r1x) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (_COPY_<String<STRU8>>) ;
		return value (def ,r1x) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (StringProc::cvt_u8s_u16s) ;
		return value (def ,r1x) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (StringProc::cvt_u8s_u32s) ;
		return value (def ,r1x) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (StringProc::cvt_u8s_as) ;
		return value (def ,r1x) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (StringProc::cvt_u8s_ws) ;
		return value (def ,r1x) ;
	}

	void friend_write (TextWriter<STRU8> &writer) const ;

public:
	imports JsonParser make (const PhanBuffer<const STRU8> &data) {
		JsonParser ret ;
		ret.initialize (data) ;
		return _MOVE_ (ret) ;
	}

private:
	explicit JsonParser (SharedRef<FixedBuffer<NODE>> &&heap ,const INDEX &index) {
		mHeap = _MOVE_ (heap) ;
		mIndex = index ;
	}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;

	Set<PTR<const String<STRU8>>> object_key_adress_set () const {
		Set<PTR<const String<STRU8>>> ret = Set<PTR<const String<STRU8>>> (mHeap->size ()) ;
		for (auto &&i : _RANGE_ (0 ,mHeap->size ())) {
			if (mHeap.self[i].mClazz != NODE_CLAZZ_OBJECT)
				continue ;
			auto &r1x = mHeap.self[i].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::null).self ;
			for (auto &&j : r1x)
				ret.add (DEPTR[j.key]) ;
		}
		return _MOVE_ (ret) ;
	}
} ;

class JsonParser::Private::RecursiveCounter
	:private Wrapped<LENGTH> {
public:
	void lock () {
		_DYNAMIC_ASSERT_ (RecursiveCounter::mSelf <= DEFAULT_RECURSIVE_SIZE::value) ;
		RecursiveCounter::mSelf++ ;
	}

	void unlock () {
		RecursiveCounter::mSelf-- ;
	}
} ;

class JsonParser::Private::InitializeLambda
	:private Proxy {
private:
	JsonParser &mContext ;

	TextReader<STRU8> mTextReader ;
	RegularReader mRis ;
	LENGTH mRecursiveCounter ;
	INDEX mLatestIndex ;
	String<STRU8> mLatestString ;

	SoftSet<INDEX> mArraySoftSet ;
	SoftSet<String<STRU8>> mObjectSoftSet ;
	Allocator<NODE ,SAUTO> mNodeHeap ;
	SharedRef<FixedBuffer<NODE>> mHeap ;
	INDEX mRoot ;

public:
	explicit InitializeLambda (JsonParser &context_ ,const PhanBuffer<const STRU8> &data)
		: mContext (context_) ,mTextReader (data) {}

	inline void operator() () {
		prepare () ;
		generate () ;
		refresh () ;
	}

private:
	void prepare () {
		mRis = RegularReader (PhanRef<TextReader<STRU8>>::make (mTextReader) ,2) ;
		mRecursiveCounter = 0 ;
		mArraySoftSet = SoftSet<INDEX> (0) ;
		mObjectSoftSet = SoftSet<String<STRU8>> (0) ;
		mNodeHeap = Allocator<NODE ,SAUTO> () ;
		mHeap = SharedRef<FixedBuffer<NODE>> () ;
		mRoot = VAR_NONE ;
	}

	void generate () {
		/*
		*	$0->$11 $10 $12
		*	$1->${value}
		*	$2->true|TRUE|false|FALSE
		*	$2x->null
		*	$3->"${string}"
		*	$4->$1|$2|$2x|$3|$6|$9
		*	$5->$4|$4 , $5
		*	$6->[ ]|[ $5 ]
		*	$7->$3 : $4
		*	$8->$7|$7 , $8
		*	$9->{ }|{ $8 }
		*	$10->${eps}|$4
		*	$11->${eps}
		*	$12->${end}
		*/
		update_shift_e0 () ;
		_DEBUG_ASSERT_ (mLatestIndex == 0) ;
		mRoot = mLatestIndex ;
		update_heap () ;
	}

	//@info: $0->$11 $10 $12
	void update_shift_e0 () {
		update_shift_e11 () ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e10 () ;
		INDEX ix = mLatestIndex ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e12 () ;
		mLatestIndex = ix ;
	}

	//@info: $1->${value}
	void update_shift_e1 () {
		mRis >> RegularReader::HINT_VALUE >> mLatestString ;
	}

	//@info: $2->true|TRUE|false|FALSE
	void update_shift_e2 () {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('t')))
				discard ;
			mRis >> _PCSTRU8_ ("true") ;
			mLatestString = _PCSTRU8_ ("true") ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('T')))
				discard ;
			mRis >> _PCSTRU8_ ("TRUE") ;
			mLatestString = _PCSTRU8_ ("TRUE") ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('f')))
				discard ;
			mRis >> _PCSTRU8_ ("false") ;
			mLatestString = _PCSTRU8_ ("false") ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('F')))
				discard ;
			mRis >> _PCSTRU8_ ("FALSE") ;
			mLatestString = _PCSTRU8_ ("FALSE") ;
		}
		if switch_once (fax) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}
	}

	//@info: $2x->null
	void update_shift_e2x () {
		mRis >> _PCSTRU8_ ("null") ;
		mLatestString = String<STRU8> () ;
	}

	//@info: $3->"${string}"
	void update_shift_e3 () {
		mRis >> RegularReader::HINT_STRING >> mLatestString ;
	}

	//@info: $4->$1|$2|$2x|$3|$6|$9
	void update_shift_e4 (const INDEX &curr) {
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::null ,mRecursiveCounter)) ;
		INDEX ix = VAR_NONE ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('+') || mRis[0] == STRU8 ('-')))
				if (!(mRis[0] >= STRU8 ('0') && mRis[0] <= STRU8 ('9')))
					discard ;
			ix = mNodeHeap.alloc () ;
			update_shift_e1 () ;
			mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (_MOVE_ (mLatestString)) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('t') || mRis[0] == STRU8 ('T')))
				if (!(mRis[0] == STRU8 ('f') || mRis[0] == STRU8 ('F')))
					discard ;
			ix = mNodeHeap.alloc () ;
			update_shift_e2 () ;
			mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (_MOVE_ (mLatestString)) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('n')))
				discard ;
			ix = mNodeHeap.alloc () ;
			update_shift_e2x () ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_NULL ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('\"')))
				discard ;
			ix = mNodeHeap.alloc () ;
			update_shift_e3 () ;
			mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (_MOVE_ (mLatestString)) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('[')))
				discard ;
			update_shift_e6 (curr) ;
			ix = mLatestIndex ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('{')))
				discard ;
			update_shift_e9 (curr) ;
			ix = mLatestIndex ;
		}
		if switch_once (fax) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}
		mLatestIndex = ix ;
	}

	//@info: $5->$4|$4 , $5
	void update_shift_e5 (const INDEX &curr) {
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		while (TRUE) {
			update_shift_e4 (curr) ;
			auto &r1x = mNodeHeap[curr].mValue.rebind (ARGV<SoftSet<INDEX>>::null).self ;
			const auto r2x = r1x.length () ;
			r1x.add (r2x ,mLatestIndex) ;
			auto &r3x = _SWITCH_ (
				(ix == VAR_NONE) ? ix :
				mNodeHeap[iy].mBrother) ;
			r3x = mLatestIndex ;
			iy = mLatestIndex ;
			mRis >> RegularReader::SKIP_GAP ;
			if (mRis[0] != STRU8 (','))
				break ;
			mRis++ ;
			mRis >> RegularReader::SKIP_GAP ;
		}
		mLatestIndex = ix ;
	}

	//@info: $6->[ ]|[ $5 ]
	void update_shift_e6 (const INDEX &curr) {
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::null ,mRecursiveCounter)) ;
		mRis >> _PCSTRU8_ ("[") ;
		INDEX ix = mNodeHeap.alloc () ;
		mNodeHeap[ix].mValue = AnyRef<SoftSet<INDEX>>::make (mArraySoftSet.share ()) ;
		mNodeHeap[ix].mClazz = NODE_CLAZZ_ARRAY ;
		mNodeHeap[ix].mParent = curr ;
		mNodeHeap[ix].mBrother = VAR_NONE ;
		mNodeHeap[ix].mChild = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		if switch_once (TRUE) {
			if (mRis[0] == STRU8 (']'))
				discard ;
			update_shift_e5 (ix) ;
			mNodeHeap[ix].mChild = mLatestIndex ;
			mRis >> RegularReader::SKIP_GAP ;
		}
		mRis >> _PCSTRU8_ ("]") ;
		mLatestIndex = ix ;
	}

	//@info: $7->$3 : $4
	void update_shift_e7 (const INDEX &curr) {
		update_shift_e3 () ;
		const auto r1x = _MOVE_ (mLatestString) ;
		mRis >> RegularReader::SKIP_GAP ;
		mRis >> _PCSTRU8_ (":") ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e4 (curr) ;
		auto &r2x = mNodeHeap[curr].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::null).self ;
		r2x.add (r1x ,mLatestIndex) ;
	}

	//@info: $8->$7|$7 , $8
	void update_shift_e8 (const INDEX &curr) {
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		while (TRUE) {
			update_shift_e7 (curr) ;
			auto &r1x = _SWITCH_ (
				(ix == VAR_NONE) ? ix :
				mNodeHeap[iy].mBrother) ;
			r1x = mLatestIndex ;
			iy = mLatestIndex ;
			mRis >> RegularReader::SKIP_GAP ;
			if (mRis[0] != STRU8 (','))
				break ;
			mRis++ ;
			mRis >> RegularReader::SKIP_GAP ;
		}
		mLatestIndex = ix ;
	}

	//@info: $9->{ }|{ $8 }
	void update_shift_e9 (const INDEX &curr) {
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::null ,mRecursiveCounter)) ;
		mRis >> _PCSTRU8_ ("{") ;
		INDEX ix = mNodeHeap.alloc () ;
		mNodeHeap[ix].mValue = AnyRef<SoftSet<String<STRU8>>>::make (mObjectSoftSet.share ()) ;
		mNodeHeap[ix].mClazz = NODE_CLAZZ_OBJECT ;
		mNodeHeap[ix].mParent = curr ;
		mNodeHeap[ix].mBrother = VAR_NONE ;
		mNodeHeap[ix].mChild = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		if switch_once (TRUE) {
			if (mRis[0] == STRU8 ('}'))
				discard ;
			update_shift_e8 (ix) ;
			mNodeHeap[ix].mChild = mLatestIndex ;
			mRis >> RegularReader::SKIP_GAP ;
		}
		mRis >> _PCSTRU8_ ("}") ;
		mLatestIndex = ix ;
	}

	//@info: $10->${eps}|$4
	void update_shift_e10 () {
		INDEX ix = VAR_NONE ;
		if switch_once (TRUE) {
			if (mRis[0] == STRU8 ('\0'))
				discard ;
			update_shift_e4 (VAR_NONE) ;
			ix = mLatestIndex ;
		}
		mLatestIndex = ix ;
	}

	//@info: $11->${eps}
	void update_shift_e11 () {
		_STATIC_WARNING_ ("noop") ;
	}

	//@info: $12->${end}
	void update_shift_e12 () {
		_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
	}

	void update_heap () {
		mArraySoftSet.clean () ;
		mObjectSoftSet.clean () ;
		mHeap = SharedRef<FixedBuffer<NODE>>::make (mNodeHeap.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : _RANGE_ (0 ,mNodeHeap.size ())) {
			if (!mNodeHeap.used (i))
				continue ;
			mHeap.self[iw++] = _MOVE_ (mNodeHeap[i]) ;
		}
		_DEBUG_ASSERT_ (iw == mHeap->size ()) ;
	}

	void refresh () {
		mContext.mHeap = _MOVE_ (mHeap) ;
		mContext.mIndex = mRoot ;
	}
} ;

inline exports void JsonParser::friend_write (TextWriter<STRU8> &writer) const {
	static constexpr auto M_NODE_X1 = EFLAG (1) ;
	static constexpr auto M_NODE_X2 = EFLAG (2) ;
	static constexpr auto M_NODE_X3 = EFLAG (3) ;
	static constexpr auto M_NODE_X4 = EFLAG (4) ;
	static constexpr auto M_NODE_X5 = EFLAG (5) ;
	static constexpr auto M_NODE_X6 = EFLAG (6) ;
	static constexpr auto M_NODE_X7 = EFLAG (7) ;
	static constexpr auto M_NODE_X8 = EFLAG (8) ;
	static constexpr auto M_NODE_X9 = EFLAG (9) ;
	auto rax = Deque<PACK<INDEX ,EFLAG>> () ;
	auto rbx = Deque<PACK<INDEX ,EFLAG>> () ;
	rax.add (PACK<INDEX ,EFLAG> {mIndex ,M_NODE_X1}) ;
	const auto r1x = object_key_adress_set () ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		const auto r2x = rax[rax.tail ()] ;
		rax.pop () ;
		auto fax = TRUE ;
		if switch_once (fax) {
			//@info: case 'null'
			if (!(r2x.mP1 != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x.mP1].mClazz == NODE_CLAZZ_NULL))
				discard ;
			if (!(r2x.mP2 == M_NODE_X1))
				discard ;
			writer << _PCSTRU8_ ("null") ;
		}
		if switch_once (fax) {
			//@info: case '"xxx"'
			if (!(r2x.mP1 != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x.mP1].mClazz == NODE_CLAZZ_STRING))
				discard ;
			if (!(r2x.mP2 == M_NODE_X1))
				discard ;
			auto &r3x = mHeap.self[r2x.mP1].mValue.rebind (ARGV<String<STRU8>>::null).self ;
			writer << _PCSTRU8_ ("\"") ;
			writer << r3x ;
			writer << _PCSTRU8_ ("\"") ;
		}
		if switch_once (fax) {
			//@info: case '[(yyy(,yyy)*)?]'
			if (!(r2x.mP1 != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x.mP1].mClazz == NODE_CLAZZ_ARRAY))
				discard ;
			if (!(r2x.mP2 == M_NODE_X1))
				discard ;
			auto &r4x = mHeap.self[r2x.mP1].mValue.rebind (ARGV<SoftSet<INDEX>>::null).self ;
			rbx.clear () ;
			rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X3}) ;
			INDEX ir = 0 ;
			for (auto &&i : r4x) {
				if (ir > 0)
					rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X4}) ;
				ir++ ;
				rbx.add (PACK<INDEX ,EFLAG> {i.sid ,M_NODE_X1}) ;
			}
			rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X5}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_once (fax) {
			//@info: case '{("xxx":yyy(,"xxx":yyy)*)?}'
			if (!(r2x.mP1 != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x.mP1].mClazz == NODE_CLAZZ_OBJECT))
				discard ;
			if (!(r2x.mP2 == M_NODE_X1))
				discard ;
			auto &r5x = mHeap.self[r2x.mP1].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::null).self ;
			rbx.clear () ;
			rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X6}) ;
			INDEX ir = 0 ;
			for (auto &&i : r5x) {
				if (ir > 0)
					rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X7}) ;
				ir++ ;
				INDEX ix = r1x.find (DEPTR[i.key]) ;
				rbx.add (PACK<INDEX ,EFLAG> {ix ,M_NODE_X2}) ;
				rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X8}) ;
				rbx.add (PACK<INDEX ,EFLAG> {i.sid ,M_NODE_X1}) ;
			}
			rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X9}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_once (fax) {
			if (!(r2x.mP1 != VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X2))
				discard ;
			writer << _PCSTRU8_ ("\"") ;
			writer << DEREF[r1x[r2x.mP1].key] ;
			writer << _PCSTRU8_ ("\"") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X3))
				discard ;
			writer << _PCSTRU8_ ("[") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X4))
				discard ;
			writer << _PCSTRU8_ (",") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X5))
				discard ;
			writer << _PCSTRU8_ ("]") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X6))
				discard ;
			writer << _PCSTRU8_ ("{") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X7))
				discard ;
			writer << _PCSTRU8_ (",") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X8))
				discard ;
			writer << _PCSTRU8_ (":") ;
		}
		if switch_once (fax) {
			if (!(r2x.mP1 == VAR_NONE))
				discard ;
			if (!(r2x.mP2 == M_NODE_X9))
				discard ;
			writer << _PCSTRU8_ ("}") ;
		}
	}
}

inline exports void JsonParser::initialize (const PhanBuffer<const STRU8> &data) {
	struct Dependent ;
	using InitializeLambda = typename DEPENDENT_TYPE<Private ,Dependent>::InitializeLambda ;
	_CALL_ (InitializeLambda (DEREF[this] ,data)) ;
}

class CommandParser {
private:
	struct Private {
		class InitializeLambda ;
	} ;

private:
	Set<String<STRU8>> mOptionSet ;
	Deque<String<STRU8>> mAttributeList ;
	Set<String<STRU8>> mAttributeMappingSet ;
	Array<String<STRU8>> mCommand ;

public:
	implicit CommandParser () = delete ;

	explicit CommandParser (const PhanBuffer<const STRU8> &data) {
		initialize (data) ;
	}

	explicit CommandParser (const VAR32 &argc ,const PTR<const PTR<STRA>> &argv) {
		const auto r1x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8>::make () ;
			auto rax = TextWriter<STRU8> (ret.raw ()) ;
			for (auto &&i : _RANGE_ (1 ,LENGTH (argc))) {
				rax << StringProc::cvt_as_u8s (PTRTOARR[argv[i]]) ;
				rax << _PCSTRU8_ (" ") ;
			}
			rax << TextWriter<STRU8>::EOS ;
			return _MOVE_ (ret) ;
		}) ;
		initialize (r1x.raw ()) ;
	}

	BOOL option (const String<STRU8> &tag) const {
		INDEX ix = mOptionSet.find (tag) ;
		if (ix == VAR_NONE)
			return FALSE ;
		return TRUE ;
	}

	const String<STRU8> &attribute (const String<STRU8> &tag) const leftvalue {
		INDEX ix = mAttributeMappingSet.map (tag) ;
		if (ix == VAR_NONE)
			return SerializationStaticProc::static_empty_string () ;
		return mAttributeList[ix] ;
	}

	template <class _ARG1>
	REMOVE_CVR_TYPE<_ARG1> attribute (const String<STRU8> &tag ,const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		REMOVE_CVR_TYPE<_ARG1> ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (attribute (tag)) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return _MOVE_ (ret) ;
	}

	BOOL attribute (const String<STRU8> &tag ,const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (StringProc::parse_bools<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	PTR<const VOID> attribute (const String<STRU8> & ,const PTR<const VOID> &) const = delete ;

	VAR32 attribute (const String<STRU8> &tag ,const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (StringProc::parse_var32s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAR64 attribute (const String<STRU8> &tag ,const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (StringProc::parse_var64s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL32 attribute (const String<STRU8> &tag ,const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (StringProc::parse_val32s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL64 attribute (const String<STRU8> &tag ,const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (StringProc::parse_val64s<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU8> attribute (const String<STRU8> &tag ,const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (_COPY_<String<STRU8>>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU16> attribute (const String<STRU8> &tag ,const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (StringProc::cvt_u8s_u16s) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU32> attribute (const String<STRU8> &tag ,const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (StringProc::cvt_u8s_u32s) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRA> attribute (const String<STRU8> &tag ,const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (StringProc::cvt_u8s_as) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRW> attribute (const String<STRU8> &tag ,const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (StringProc::cvt_u8s_ws) ;
		return attribute (tag ,def ,r1x) ;
	}

	const Array<String<STRU8>> &command () const leftvalue {
		return mCommand ;
	}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;
} ;

class CommandParser::Private::InitializeLambda
	:private Proxy {
private:
	CommandParser &mContext ;

	TextReader<STRU8> mTextReader ;
	RegularReader mRis ;
	String<STRU8> mLatestString ;

	Set<String<STRU8>> mOptionSet ;
	Deque<String<STRU8>> mAttributeList ;
	Set<String<STRU8>> mAttributeMappingSet ;
	SoftList<String<STRU8>> mCommandList ;
	Array<String<STRU8>> mCommand ;

public:
	explicit InitializeLambda (CommandParser &context_ ,const PhanBuffer<const STRU8> &data)
		: mContext (context_) ,mTextReader (data) {}

	inline void operator() () {
		prepare () ;
		generate () ;
		refresh () ;
	}

private:
	void prepare () {
		mRis = RegularReader (PhanRef<TextReader<STRU8>>::make (mTextReader) ,2) ;
		mOptionSet = Set<String<STRU8>> () ;
		mAttributeList = Deque<String<STRU8>> () ;
		mAttributeMappingSet = Set<String<STRU8>> () ;
		mCommandList = SoftList<String<STRU8>> () ;
	}

	void generate () {
		/*
		*	$0->$8 $7 $9
		*	$1->${identity}
		*	$2->"${string}"
		*	$3->${newgap}
		*	$4->/$1
		*	$5->-$1|-$1=$2|-$1=$3
		*	$6->$2|$3
		*	$7->${eps}|$4 $7|$5 $7|$6 $7
		*	$8->${eps}
		*	$9->${end}
		*/
		update_shift_e0 () ;
		update_command () ;
	}

	//@info: $0->$8 $7 $9
	void update_shift_e0 () {
		update_shift_e8 () ;
		mRis >> RegularReader::SKIP_GAP_SPACE ;
		update_shift_e7 () ;
		mRis >> RegularReader::SKIP_GAP_SPACE ;
		update_shift_e9 () ;
	}

	//@info: $1->${identity}
	void update_shift_e1 () {
		mRis >> RegularReader::HINT_IDENTIFIER >> mLatestString ;
	}

	//@info: $2->"${string}"
	void update_shift_e2 () {
		mRis >> RegularReader::HINT_STRING >> mLatestString ;
	}

	//@info: $3->${newgap}
	void update_shift_e3 () {
		mRis >> RegularReader::HINT_NEWGAP >> mLatestString ;
	}

	//@info: $4->/$1
	void update_shift_e4 () {
		mRis >> _PCSTRU8_ ("/") ;
		update_shift_e1 () ;
		mOptionSet.add (_MOVE_ (mLatestString)) ;
	}

	//@info: $5->-$1|-$1=$2|-$1=$3
	void update_shift_e5 () {
		mRis >> _PCSTRU8_ ("-") ;
		update_shift_e1 () ;
		INDEX ix = mAttributeMappingSet.map (mLatestString) ;
		_DYNAMIC_ASSERT_ (ix == VAR_NONE) ;
		ix = mAttributeList.insert () ;
		mAttributeMappingSet.add (_MOVE_ (mLatestString) ,ix) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('=')))
				discard ;
			if (!(mRis[1] == STRU8 ('\"')))
				discard ;
			mRis >> _PCSTRU8_ ("=") ;
			update_shift_e2 () ;
			mAttributeList[ix] = _MOVE_ (mLatestString) ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('=')))
				discard ;
			mRis >> _PCSTRU8_ ("=") ;
			update_shift_e3 () ;
			mAttributeList[ix] = _MOVE_ (mLatestString) ;
		}
		if switch_once (fax) {
			mAttributeList[ix] = _PCSTRU8_ ("TRUE") ;
		}
	}

	//@info: $6->$2|$3
	void update_shift_e6 () {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('\"')))
				discard ;
			update_shift_e2 () ;
		}
		if switch_once (fax) {
			update_shift_e3 () ;
		}
		mCommandList.add (_MOVE_ (mLatestString)) ;
	}

	//@info: $7->${eps}|$4 $7|$5 $7|$6 $7
	void update_shift_e7 () {
		while (TRUE) {
			if (mRis[0] == STRU8 ('\0'))
				break ;
			auto fax = TRUE ;
			if switch_once (fax) {
				if (!(mRis[0] == STRU8 ('/')))
					discard ;
				update_shift_e4 () ;
			}
			if switch_once (fax) {
				if (!(mRis[0] == STRU8 ('-')))
					discard ;
				update_shift_e5 () ;
			}
			if switch_once (fax) {
				update_shift_e6 () ;
			}
			mRis >> RegularReader::SKIP_GAP_SPACE ;
		}
	}

	//@info: $8->${eps}
	void update_shift_e8 () {
		_STATIC_WARNING_ ("noop") ;
	}

	//@info: $9->${end}
	void update_shift_e9 () {
		_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
	}

	void update_command () {
		mCommand = Array<String<STRU8>> (mCommandList.length ()) ;
		for (auto &&i : _RANGE_ (0 ,mCommandList.length ()))
			mCommand[i] = _MOVE_ (mCommandList[mCommandList.access (i)]) ;
	}

	void refresh () {
		mContext.mOptionSet = _MOVE_ (mOptionSet) ;
		mContext.mAttributeList = _MOVE_ (mAttributeList) ;
		mContext.mAttributeMappingSet = _MOVE_ (mAttributeMappingSet) ;
		mContext.mCommand = _MOVE_ (mCommand) ;
	}
} ;

inline exports void CommandParser::initialize (const PhanBuffer<const STRU8> &data) {
	struct Dependent ;
	using InitializeLambda = typename DEPENDENT_TYPE<Private ,Dependent>::InitializeLambda ;
	_CALL_ (InitializeLambda (DEREF[this] ,data)) ;
}
} ;