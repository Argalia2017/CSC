#pragma once

#ifndef __CSC_SERIALIZATION__
#define __CSC_SERIALIZATION__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
class SerializationStaticProc :
	delegate private Wrapped<> {
public:
	imports const String<STRU8> &static_empty_string () ;
} ;

inline exports const String<STRU8> &SerializationStaticProc::static_empty_string () {
	return _CACHE_ ([&] () {
		return String<STRU8> () ;
	}) ;
}

static constexpr auto NODE_CLAZZ_NULL = EFLAG (1) ;
static constexpr auto NODE_CLAZZ_STRING = EFLAG (2) ;
static constexpr auto NODE_CLAZZ_ARRAY = EFLAG (3) ;
static constexpr auto NODE_CLAZZ_OBJECT = EFLAG (4) ;
static constexpr auto NODE_CLAZZ_TABLE = EFLAG (5) ;
static constexpr auto NODE_CLAZZ_FINAL = EFLAG (6) ;

class XmlParser {
private:
	struct Private {
		class RecursiveCounter ;

		class InitializeX1Lambda ;

		class InitializeX2Lambda ;
	} ;

	struct NODE_PACK {
		String<STRU8> mName ;
		Deque<String<STRU8>> mAttribute ;
		SoftSet<String<STRU8>> mAttributeMappingSet ;
		SoftSet<INDEX> mMemberSet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

private:
	SharedRef<FixedBuffer<NODE_PACK>> mHeap ;
	INDEX mIndex ;

public:
	implicit XmlParser () {
		mIndex = VAR_NONE ;
	}

	explicit XmlParser (const SharedRef<FixedBuffer<NODE_PACK>> &heap ,const INDEX &index) {
		mHeap = heap ;
		mIndex = index ;
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
			return XmlParser (mHeap ,VAR_NONE) ;
		return XmlParser (mHeap ,0) ;
	}

	XmlParser parent () const {
		if (!exist ())
			return XmlParser (mHeap ,VAR_NONE) ;
		return XmlParser (mHeap ,mHeap.self[mIndex].mParent) ;
	}

	XmlParser brother () const {
		if (!exist ())
			return XmlParser (mHeap ,VAR_NONE) ;
		return XmlParser (mHeap ,mHeap.self[mIndex].mBrother) ;
	}

	XmlParser child () const {
		if (!exist ())
			return XmlParser (mHeap ,VAR_NONE) ;
		return XmlParser (mHeap ,mHeap.self[mIndex].mChild) ;
	}

	XmlParser child (const String<STRU8> &name) const {
		if (!exist ())
			return XmlParser (mHeap ,VAR_NONE) ;
		INDEX ix = mHeap.self[mIndex].mObjectSet.map (name) ;
		return XmlParser (mHeap ,ix) ;
	}

	Array<XmlParser> child_array () const {
		Array<XmlParser> ret ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			const auto r1x = mHeap.self[mIndex].mMemberSet.range () ;
			ret = Array<XmlParser> (r1x.length ()) ;
			for (auto &&i : _RANGE_ (0 ,r1x.length ()))
				ret[i] = XmlParser (mHeap ,mHeap.self[mIndex].mMemberSet.map_get (r1x[i])) ;
		}
		return _MOVE_ (ret) ;
	}

	Array<XmlParser> child_array (const LENGTH &fixed_len) const {
		auto rax = Deque<XmlParser> (fixed_len) ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet) {
				const auto r1x = mHeap.self[mIndex].mMemberSet.at (i) ;
				if (rax.length () >= fixed_len)
					continue ;
				INDEX ix = rax.insert () ;
				rax[ix] = XmlParser (mHeap ,mHeap.self[mIndex].mMemberSet.map_get (r1x)) ;
			}
		}
		Array<XmlParser> ret = Array<XmlParser> (fixed_len) ;
		for (auto &&i : _RANGE_ (0 ,rax.length ())) {
			INDEX ix = rax.access (i) ;
			ret[i] = _MOVE_ (rax[ix]) ;
		}
		return _MOVE_ (ret) ;
	}

	BOOL equal (const XmlParser &that) const {
		if (_EBOOL_ (exist ()) != _EBOOL_ (that.exist ()))
			return FALSE ;
		if (!exist ())
			return TRUE ;
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
		return mHeap.self[mIndex].mAttribute[ix] ;
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

	PTR<const NONE> attribute (const String<STRU8> & ,const PTR<const NONE> &) const = delete ;

	const String<STRU8> &value () const leftvalue {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mMemberSet.size () == 0) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mAttributeMappingSet.length () == 1) ;
		INDEX ix = mHeap.self[mIndex].mAttribute.head () ;
		return mHeap.self[mIndex].mAttribute[ix] ;
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

	PTR<const NONE> value (const PTR<const NONE> &) const = delete ;

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

	void friend_write (TextWriter<STRU8> &writer) const {
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
				INDEX jx = r2x.mChild ;
				while (TRUE) {
					if (jx == VAR_NONE)
						break ;
					rbx.add (PACK<INDEX ,EFLAG> {jx ,M_NODE_X1}) ;
					jx = mHeap.self[jx].mBrother ;
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
					const auto r4x = r3x.mAttributeMappingSet.at (i) ;
					writer << i ;
					writer << _PCSTRU8_ ("=\"") ;
					INDEX jx = r3x.mAttributeMappingSet.map_get (r4x) ;
					writer << r3x.mAttribute[jx] << _PCSTRU8_ ("\" ") ;
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
				auto &r5x = mHeap.self[r1x.mP1] ;
				writer << _PCSTRU8_ ("<") << r5x.mName << _PCSTRU8_ (" ") ;
				for (auto &&i : r5x.mAttributeMappingSet) {
					const auto r6x = r5x.mAttributeMappingSet.at (i) ;
					writer << i ;
					writer << _PCSTRU8_ ("=\"") ;
					INDEX jx = r5x.mAttributeMappingSet.map_get (r6x) ;
					writer << r5x.mAttribute[jx] << _PCSTRU8_ ("\" ") ;
				}
				writer << _PCSTRU8_ (">") ;
				rbx.clear () ;
				INDEX jx = r5x.mChild ;
				while (TRUE) {
					if (jx == VAR_NONE)
						break ;
					rbx.add (PACK<INDEX ,EFLAG> {jx ,M_NODE_X1}) ;
					jx = mHeap.self[jx].mBrother ;
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

	imports XmlParser make (const PhanBuffer<const STRU8> &data) {
		XmlParser ret ;
		initialize (ret ,data) ;
		return _MOVE_ (ret) ;
	}

	imports XmlParser make (const Array<XmlParser> &sequence) {
		XmlParser ret ;
		initialize (ret ,sequence) ;
		return _MOVE_ (ret) ;
	}

private:
	imports void initialize (XmlParser &this_ ,const PhanBuffer<const STRU8> &data) ;

	imports void initialize (XmlParser &this_ ,const Array<XmlParser> &sequence) ;
} ;

class XmlParser::Private::RecursiveCounter :
	delegate private Wrapped<LENGTH> {
private:
	using Wrapped<LENGTH>::mSelf ;

public:
	void lock () {
		_DYNAMIC_ASSERT_ (mSelf <= DEFAULT_RECURSIVE_SIZE::compile ()) ;
		mSelf++ ;
	}

	void unlock () {
		mSelf-- ;
	}
} ;

class XmlParser::Private::InitializeX1Lambda :
	delegate private Proxy {
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
	List<NODE_PACK> mNodeTree ;
	SharedRef<FixedBuffer<NODE_PACK>> mHeap ;
	INDEX mRoot ;

public:
	implicit InitializeX1Lambda () = delete ;

	explicit InitializeX1Lambda (const DEF<decltype (ARGVP0)> & ,XmlParser &context_ ,REMOVE_CONST_TYPE<PhanBuffer<const STRU8>> &&data) :
		delegate mContext (context_) ,
		delegate mTextReader (_MOVE_ (data)) {}

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
		mNodeTree = List<NODE_PACK> () ;
		mHeap = SharedRef<FixedBuffer<NODE_PACK>> () ;
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
		INDEX ix = mNodeTree.insert () ;
		mNodeTree[ix].mMemberSet = mMemberSoftSet.share () ;
		mNodeTree[ix].mObjectSet = mObjectSoftSet.share () ;
		mNodeTree[ix].mParent = VAR_NONE ;
		update_shift_e7 (ix) ;
		mNodeTree[ix].mChild = mLatestIndex ;
		mNodeTree[ix].mBrother = VAR_NONE ;
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
		INDEX ix = mNodeTree[curr].mAttributeMappingSet.map (mLatestString) ;
		_DYNAMIC_ASSERT_ (ix == VAR_NONE) ;
		ix = mNodeTree[curr].mAttribute.insert () ;
		mNodeTree[curr].mAttributeMappingSet.add (_MOVE_ (mLatestString) ,ix) ;
		mRis >> RegularReader::SKIP_GAP ;
		mRis >> _PCSTRU8_ ("=") ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e2 () ;
		mNodeTree[curr].mAttribute[ix] = _MOVE_ (mLatestString) ;
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
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::ID ,mRecursiveCounter)) ;
		mRis >> _PCSTRU8_ ("<") ;
		INDEX ix = mNodeTree.insert () ;
		update_shift_e1 () ;
		mNodeTree[ix].mName = _MOVE_ (mLatestString) ;
		mNodeTree[ix].mAttribute = Deque<String<STRU8>> () ;
		mNodeTree[ix].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
		mNodeTree[ix].mParent = curr ;
		mNodeTree[ix].mBrother = VAR_NONE ;
		mNodeTree[ix].mChild = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		update_shift_e4 (ix) ;
		mRis >> RegularReader::SKIP_GAP ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('>')))
				discard ;
			mRis++ ;
			mRis >> RegularReader::SKIP_GAP ;
			mNodeTree[ix].mMemberSet = mMemberSoftSet.share () ;
			mNodeTree[ix].mObjectSet = mObjectSoftSet.share () ;
			update_shift_e7 (ix) ;
			mNodeTree[ix].mChild = mLatestIndex ;
			mRis >> RegularReader::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("</") ;
			update_shift_e1 () ;
			_DYNAMIC_ASSERT_ (mNodeTree[ix].mName == mLatestString) ;
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
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::ID ,mRecursiveCounter)) ;
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
				const auto r3x = mNodeTree[curr].mMemberSet.length () ;
				mNodeTree[curr].mMemberSet.add (r3x ,mLatestIndex) ;
				mNodeTree[curr].mObjectSet.add (mNodeTree[mLatestIndex].mName ,mLatestIndex) ;
				auto &r4x = _CALL_ ([&] () {
					if (ix == VAR_NONE)
						return _BYREF_ (ix) ;
					return _BYREF_ (mNodeTree[iy].mBrother) ;
				}).self ;
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
		const auto r1x = mNodeTree.range () ;
		mHeap = SharedRef<FixedBuffer<NODE_PACK>>::make (r1x.length ()) ;
		for (auto &&i : _RANGE_ (0 ,r1x.length ()))
			mHeap.self[i] = _MOVE_ (mNodeTree[r1x[i]]) ;
	}

	void refresh () {
		mContext.mHeap = _MOVE_ (mHeap) ;
		mContext.mIndex = mRoot ;
	}
} ;

class XmlParser::Private::InitializeX2Lambda :
	delegate private Proxy {
private:
	struct FOUND_NODE {
		String<STRU8> mName ;
		Deque<String<STRU8>> mAttribute ;
		SoftSet<String<STRU8>> mAttributeMappingSet ;
		EFLAG mClazz ;
		Deque<XmlParser> mBaseNode ;
	} ;

	struct STACK_NODE {
		INDEX mParent ;
		EFLAG mClazz ;
		Deque<XmlParser> mBaseNode ;
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
	Array<Function<void (const XmlParser &)>> mFoundNodeProc ;
	Set<EFLAG> mFoundNodeProcMappingSet ;
	SoftSet<String<STRU8>> mAttributeMappingSoftSet ;
	SoftSet<INDEX> mMemberSoftSet ;
	SoftSet<String<STRU8>> mObjectSoftSet ;
	List<NODE_PACK> mNodeTree ;
	Deque<FOUND_NODE> mFoundNode ;
	Set<String<STRU8>> mFoundNodeMappingSet ;
	SharedRef<FixedBuffer<NODE_PACK>> mHeap ;
	INDEX mRoot ;

	Deque<Deque<XmlParser>> mFoundNodeBaseNodeQueue ;
	STACK_NODE mTempNode ;

public:
	implicit InitializeX2Lambda () = delete ;

	explicit InitializeX2Lambda (const DEF<decltype (ARGVP0)> & ,XmlParser &context_ ,const Array<XmlParser> &sequence) :
		delegate mContext (context_) ,
		delegate mSequence (sequence) ,
		delegate mClazzString (_PCSTRU8_ ("type")) ,
		delegate mTableClazzString (_PCSTRU8_ ("table")) ,
		delegate mObjectClazzString (_PCSTRU8_ ("object")) ,
		delegate mArrayClazzString (_PCSTRU8_ ("array")) ,
		delegate mFinalClazzString (_PCSTRU8_ ("final")) {}

	inline void operator() () {
		prepare () ;
		generate () ;
		refresh () ;
	}

private:
	void prepare () {
		mNodeStack = Deque<STACK_NODE> () ;
		//@error: fuck g++4.8
		mFoundNodeProc = Array<Function<void (const XmlParser &)>> (3) ;
		mFoundNodeProcMappingSet.add (NODE_CLAZZ_TABLE ,0) ;
		mFoundNodeProcMappingSet.add (NODE_CLAZZ_OBJECT ,1) ;
		mFoundNodeProcMappingSet.add (NODE_CLAZZ_ARRAY ,2) ;
		mFoundNodeProcMappingSet.add (NODE_CLAZZ_FINAL ,0) ;
		mFoundNodeProc[0] = Function<void (const XmlParser &)> ([&] (const XmlParser &node) {
			update_found_table_node (node) ;
		}) ;
		mFoundNodeProc[1] = Function<void (const XmlParser &)> ([&] (const XmlParser &node) {
			update_found_object_node (node) ;
		}) ;
		mFoundNodeProc[2] = Function<void (const XmlParser &)> ([&] (const XmlParser &node) {
			update_found_array_node (node) ;
		}) ;
		mAttributeMappingSoftSet = SoftSet<String<STRU8>> (0) ;
		mMemberSoftSet = SoftSet<INDEX> (0) ;
		mObjectSoftSet = SoftSet<String<STRU8>> (0) ;
		mNodeTree = List<NODE_PACK> () ;
		mRoot = mNodeTree.insert () ;
		mNodeTree[mRoot].mMemberSet = mMemberSoftSet.share () ;
		mNodeTree[mRoot].mObjectSet = mObjectSoftSet.share () ;
		mNodeTree[mRoot].mParent = VAR_NONE ;
		mNodeTree[mRoot].mChild = VAR_NONE ;
		mNodeTree[mRoot].mBrother = VAR_NONE ;
		mFoundNode = Deque<FOUND_NODE> () ;
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
			for (auto &&i : mTempNode.mBaseNode) {
				INDEX ix = mFoundNodeProcMappingSet.map (mTempNode.mClazz) ;
				mFoundNodeProc[ix] (i) ;
			}
			update_merge_found_node (mTempNode.mParent) ;
			mFoundNodeBaseNodeQueue.add (_MOVE_ (mTempNode.mBaseNode)) ;
		}
		update_heap () ;
	}

	void update_root_node () {
		INDEX ix = find_normal_node () ;
		if (ix == VAR_NONE)
			return ;
		INDEX jx = mNodeStack.insert () ;
		mNodeStack[jx].mBaseNode = Deque<XmlParser> (mSequence.length ()) ;
		for (auto &&i : mSequence) {
			if (!i.exist ())
				continue ;
			mNodeStack[jx].mBaseNode.add (i) ;
		}
		mNodeStack[jx].mClazz = NODE_CLAZZ_OBJECT ;
		mNodeStack[jx].mParent = mRoot ;
	}

	INDEX find_normal_node () const {
		for (auto &&i : mSequence) {
			if (i.exist ())
				return mSequence.at (i) ;
		}
		return VAR_NONE ;
	}

	void update_found_table_node (const XmlParser &node) {
		auto rax = node ;
		while (TRUE) {
			if (!rax.exist ())
				break ;
			INDEX ix = mFoundNode.insert () ;
			mFoundNode[ix].mName = rax.name () ;
			mFoundNode[ix].mClazz = node_type (rax) ;
			mFoundNode[ix].mAttribute = Deque<String<STRU8>> () ;
			mFoundNode[ix].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
			mFoundNode[ix].mAttribute.appand (rax.mHeap.self[rax.mIndex].mAttribute) ;
			mFoundNode[ix].mAttributeMappingSet.appand (rax.mHeap.self[rax.mIndex].mAttributeMappingSet) ;
			if (mFoundNodeBaseNodeQueue.empty ())
				mFoundNodeBaseNodeQueue.add (Deque<XmlParser> ()) ;
			mFoundNodeBaseNodeQueue.take (mFoundNode[ix].mBaseNode) ;
			mFoundNode[ix].mBaseNode.clear () ;
			mFoundNode[ix].mBaseNode.add (rax.child ()) ;
			rax = rax.brother () ;
		}
	}

	void update_found_object_node (const XmlParser &node) {
		auto rax = node ;
		while (TRUE) {
			if (!rax.exist ())
				break ;
			const auto r1x = rax.name () ;
			const auto r2x = node_type (rax) ;
			INDEX ix = mFoundNodeMappingSet.map (r1x) ;
			if switch_once (TRUE) {
				if (ix == VAR_NONE)
					discard ;
				_DYNAMIC_ASSERT_ (mFoundNode[ix].mClazz == r2x) ;
				_DYNAMIC_ASSERT_ (r2x != NODE_CLAZZ_FINAL) ;
			}
			INDEX iy = ix ;
			if switch_once (TRUE) {
				if (ix != VAR_NONE)
					discard ;
				iy = mFoundNode.insert () ;
				mFoundNodeMappingSet.add (r1x ,iy) ;
				mFoundNode[iy].mName = r1x ;
				mFoundNode[iy].mClazz = r2x ;
				mFoundNode[iy].mAttribute = Deque<String<STRU8>> () ;
				mFoundNode[iy].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
				if (mFoundNodeBaseNodeQueue.empty ())
					mFoundNodeBaseNodeQueue.add (Deque<XmlParser> ()) ;
				mFoundNodeBaseNodeQueue.take (mFoundNode[iy].mBaseNode) ;
				mFoundNode[iy].mBaseNode.clear () ;
			}
			for (auto &&j : rax.mHeap.self[rax.mIndex].mAttributeMappingSet) {
				const auto r3x = rax.mHeap.self[rax.mIndex].mAttributeMappingSet.at (j) ;
				INDEX jx = mFoundNode[iy].mAttributeMappingSet.map (j) ;
				if switch_once (TRUE) {
					if (jx != VAR_NONE)
						discard ;
					jx = mFoundNode[iy].mAttribute.insert () ;
					mFoundNode[iy].mAttributeMappingSet.add (j ,jx) ;
					INDEX kx = rax.mHeap.self[rax.mIndex].mAttributeMappingSet.map_get (r3x) ;
					mFoundNode[iy].mAttribute[jx] = rax.mHeap.self[rax.mIndex].mAttribute[kx] ;
				}
			}
			mFoundNode[iy].mBaseNode.add (rax.child ()) ;
			rax = rax.brother () ;
		}
	}

	void update_found_array_node (const XmlParser &node) {
		auto rax = node ;
		while (TRUE) {
			if (!rax.exist ())
				break ;
			const auto r1x = rax.name () ;
			const auto r2x = node_type (rax) ;
			INDEX ix = mFoundNode.head () ;
			if switch_once (TRUE) {
				if (ix == VAR_NONE)
					discard ;
				_DYNAMIC_ASSERT_ (mFoundNode[ix].mName == r1x) ;
				_DYNAMIC_ASSERT_ (r2x != NODE_CLAZZ_FINAL) ;
			}
			INDEX iy = mFoundNode.insert () ;
			mFoundNodeMappingSet.add (r1x ,iy) ;
			mFoundNode[iy].mName = r1x ;
			mFoundNode[iy].mClazz = r2x ;
			mFoundNode[iy].mAttribute = Deque<String<STRU8>> () ;
			mFoundNode[iy].mAttributeMappingSet = mAttributeMappingSoftSet.share () ;
			mFoundNode[iy].mAttribute.appand (rax.mHeap.self[rax.mIndex].mAttribute) ;
			mFoundNode[iy].mAttributeMappingSet.appand (rax.mHeap.self[rax.mIndex].mAttributeMappingSet) ;
			if (mFoundNodeBaseNodeQueue.empty ())
				mFoundNodeBaseNodeQueue.add (Deque<XmlParser> ()) ;
			mFoundNodeBaseNodeQueue.take (mFoundNode[iy].mBaseNode) ;
			mFoundNode[iy].mBaseNode.clear () ;
			mFoundNode[iy].mBaseNode.add (rax.child ()) ;
			rax = rax.brother () ;
		}
	}

	void update_merge_found_node (const INDEX &curr) {
		INDEX ix = VAR_NONE ;
		for (auto &&i : mFoundNode) {
			INDEX iy = ix ;
			ix = mNodeTree.insert () ;
			mNodeTree[ix].mName = _MOVE_ (i.mName) ;
			mNodeTree[ix].mAttribute = _MOVE_ (i.mAttribute) ;
			mNodeTree[ix].mAttributeMappingSet = _MOVE_ (i.mAttributeMappingSet) ;
			mNodeTree[ix].mParent = curr ;
			if switch_once (TRUE) {
				INDEX jx = mNodeTree[ix].mParent ;
				if (jx == VAR_NONE)
					discard ;
				if (mNodeTree[jx].mChild == VAR_NONE)
					mNodeTree[jx].mChild = ix ;
				if switch_once (TRUE) {
					if (mNodeTree[jx].mMemberSet.size () > 0)
						discard ;
					mNodeTree[jx].mMemberSet = mMemberSoftSet.share () ;
					mNodeTree[jx].mObjectSet = mObjectSoftSet.share () ;
				}
				const auto r1x = mNodeTree[jx].mMemberSet.length () ;
				mNodeTree[jx].mMemberSet.add (r1x ,ix) ;
				mNodeTree[jx].mObjectSet.add (mNodeTree[ix].mName ,ix) ;
			}
			mNodeTree[ix].mBrother = VAR_NONE ;
			mNodeTree[ix].mChild = VAR_NONE ;
			if (iy != VAR_NONE)
				mNodeTree[iy].mBrother = ix ;
			if (mRoot == VAR_NONE)
				mRoot = ix ;
			INDEX jy = mNodeStack.insert () ;
			mNodeStack[jy].mBaseNode = _MOVE_ (i.mBaseNode) ;
			mNodeStack[jy].mClazz = i.mClazz ;
			mNodeStack[jy].mParent = ix ;
		}
		mFoundNodeMappingSet.clear () ;
		mFoundNode.clear () ;
	}

	void update_heap () {
		mAttributeMappingSoftSet.clean () ;
		mObjectSoftSet.clean () ;
		const auto r1x = mNodeTree.range () ;
		mHeap = SharedRef<FixedBuffer<NODE_PACK>>::make (r1x.length ()) ;
		for (auto &&i : _RANGE_ (0 ,r1x.length ()))
			mHeap.self[i] = _MOVE_ (mNodeTree[r1x[i]]) ;
	}

	void refresh () {
		mContext.mHeap = _MOVE_ (mHeap) ;
		mContext.mIndex = mRoot ;
	}
} ;

inline exports void XmlParser::initialize (XmlParser &this_ ,const PhanBuffer<const STRU8> &data) {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::InitializeX1Lambda ;
	_CALL_TRY_ (R1X (ARGVP0 ,this_ ,PhanBuffer<const STRU8>::make (data))) ;
}

inline exports void XmlParser::initialize (XmlParser &this_ ,const Array<XmlParser> &sequence) {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::InitializeX2Lambda ;
	_CALL_TRY_ (R1X (ARGVP0 ,this_ ,sequence)) ;
}

class JsonParser {
private:
	struct Private {
		class RecursiveCounter ;

		class InitializeLambda ;
	} ;

	struct NODE_PACK {
		AnyRef<> mValue ;
		EFLAG mClazz ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

private:
	SharedRef<FixedBuffer<NODE_PACK>> mHeap ;
	INDEX mIndex ;

public:
	implicit JsonParser () {
		mIndex = VAR_NONE ;
	}

	explicit JsonParser (const SharedRef<FixedBuffer<NODE_PACK>> &heap ,const INDEX &index) {
		mHeap = heap ;
		mIndex = index ;
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
			return JsonParser (mHeap ,VAR_NONE) ;
		return JsonParser (mHeap ,0) ;
	}

	JsonParser parent () const {
		if (!exist ())
			return JsonParser (mHeap ,VAR_NONE) ;
		return JsonParser (mHeap ,mHeap.self[mIndex].mParent) ;
	}

	JsonParser brother () const {
		if (!exist ())
			return JsonParser (mHeap ,VAR_NONE) ;
		return JsonParser (mHeap ,mHeap.self[mIndex].mBrother) ;
	}

	JsonParser child () const {
		if (!exist ())
			return JsonParser (mHeap ,VAR_NONE) ;
		return JsonParser (mHeap ,mHeap.self[mIndex].mChild) ;
	}

	JsonParser child (const String<STRU8> &key) const {
		if (!exist ())
			return JsonParser (mHeap ,VAR_NONE) ;
		if (!object_type ())
			return JsonParser (mHeap ,VAR_NONE) ;
		auto &r1x = mHeap.self[mIndex].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::ID).self ;
		INDEX ix = r1x.map (key) ;
		return JsonParser (mHeap ,ix) ;
	}

	Array<JsonParser> child_array () const {
		Array<JsonParser> ret ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1x = mHeap.self[mIndex].mValue.rebind (ARGV<SoftSet<INDEX>>::ID).self ;
			const auto r2x = r1x.range () ;
			ret = Array<JsonParser> (r2x.length ()) ;
			for (auto &&i : _RANGE_ (0 ,r2x.length ()))
				ret[i] = JsonParser (mHeap ,r1x.map_get (r2x[i])) ;
		}
		return _MOVE_ (ret) ;
	}

	Array<JsonParser> child_array (const LENGTH &fixed_len) const {
		auto rax = Deque<JsonParser> (fixed_len) ;
		if switch_once (TRUE) {
			if (!exist ())
				discard ;
			auto &r1x = mHeap.self[mIndex].mValue.rebind (ARGV<SoftSet<INDEX>>::ID).self ;
			for (auto &&i : r1x) {
				const auto r2x = r1x.at (i) ;
				if (rax.length () >= fixed_len)
					continue ;
				INDEX ix = rax.insert () ;
				rax[ix] = JsonParser (mHeap ,r1x.map_get (r2x)) ;
			}
		}
		Array<JsonParser> ret = Array<JsonParser> (fixed_len) ;
		for (auto &&i : _RANGE_ (0 ,rax.length ())) {
			INDEX ix = rax.access (i) ;
			ret[i] = _MOVE_ (rax[ix]) ;
		}
		return _MOVE_ (ret) ;
	}

	BOOL equal (const JsonParser &that) const {
		if (_EBOOL_ (exist ()) != _EBOOL_ (that.exist ()))
			return FALSE ;
		if (!exist ())
			return TRUE ;
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
		return mHeap.self[mIndex].mValue.rebind (ARGV<String<STRU8>>::ID).self ;
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

	PTR<const NONE> value (const PTR<const NONE> &) const = delete ;

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

	void friend_write (TextWriter<STRU8> &writer) const {
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
				auto &r3x = mHeap.self[r2x.mP1].mValue.rebind (ARGV<String<STRU8>>::ID).self ;
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
				auto &r4x = mHeap.self[r2x.mP1].mValue.rebind (ARGV<SoftSet<INDEX>>::ID).self ;
				rbx.clear () ;
				rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X3}) ;
				INDEX iw = 0 ;
				for (auto &&i : r4x) {
					const auto r5x = r4x.at (i) ;
					if (iw > 0)
						rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X4}) ;
					iw++ ;
					rbx.add (PACK<INDEX ,EFLAG> {r4x.map_get (r5x) ,M_NODE_X1}) ;
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
				auto &r6x = mHeap.self[r2x.mP1].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::ID).self ;
				rbx.clear () ;
				rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X6}) ;
				INDEX iw = 0 ;
				for (auto &&i : r6x) {
					const auto r7x = r6x.at (i) ;
					if (iw > 0)
						rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X7}) ;
					iw++ ;
					INDEX ix = r1x.find (DEPTR[i]) ;
					rbx.add (PACK<INDEX ,EFLAG> {ix ,M_NODE_X2}) ;
					rbx.add (PACK<INDEX ,EFLAG> {VAR_NONE ,M_NODE_X8}) ;
					rbx.add (PACK<INDEX ,EFLAG> {r6x.map_get (r7x) ,M_NODE_X1}) ;
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
				writer << DEREF[r1x[r2x.mP1]] ;
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

	imports JsonParser make (const PhanBuffer<const STRU8> &data) {
		JsonParser ret ;
		initialize (ret ,data) ;
		return _MOVE_ (ret) ;
	}

private:
	Set<PTR<const String<STRU8>>> object_key_adress_set () const {
		Set<PTR<const String<STRU8>>> ret = Set<PTR<const String<STRU8>>> (mHeap->size ()) ;
		for (auto &&i : _RANGE_ (0 ,mHeap->size ())) {
			if (mHeap.self[i].mClazz != NODE_CLAZZ_OBJECT)
				continue ;
			auto &r1x = mHeap.self[i].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::ID).self ;
			for (auto &&j : r1x)
				ret.add (DEPTR[j]) ;
		}
		return _MOVE_ (ret) ;
	}

private:
	imports void initialize (JsonParser &this_ ,const PhanBuffer<const STRU8> &data) ;
} ;

class JsonParser::Private::RecursiveCounter :
	delegate private Wrapped<LENGTH> {
private:
	using Wrapped<LENGTH>::mSelf ;

public:
	void lock () {
		_DYNAMIC_ASSERT_ (mSelf <= DEFAULT_RECURSIVE_SIZE::compile ()) ;
		mSelf++ ;
	}

	void unlock () {
		mSelf-- ;
	}
} ;

class JsonParser::Private::InitializeLambda :
	delegate private Proxy {
private:
	JsonParser &mContext ;

	TextReader<STRU8> mTextReader ;
	RegularReader mRis ;
	LENGTH mRecursiveCounter ;
	INDEX mLatestIndex ;
	String<STRU8> mLatestString ;

	SoftSet<INDEX> mArraySoftSet ;
	SoftSet<String<STRU8>> mObjectSoftSet ;
	List<NODE_PACK> mNodeTree ;
	SharedRef<FixedBuffer<NODE_PACK>> mHeap ;
	INDEX mRoot ;

public:
	implicit InitializeLambda () = delete ;

	explicit InitializeLambda (const DEF<decltype (ARGVP0)> & ,JsonParser &context_ ,REMOVE_CONST_TYPE<PhanBuffer<const STRU8>> &&data) :
		delegate mContext (context_) ,
		delegate mTextReader (_MOVE_ (data)) {}

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
		mNodeTree = List<NODE_PACK> () ;
		mHeap = SharedRef<FixedBuffer<NODE_PACK>> () ;
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
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::ID ,mRecursiveCounter)) ;
		INDEX ix = VAR_NONE ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('+') || mRis[0] == STRU8 ('-')))
				if (!(mRis[0] >= STRU8 ('0') && mRis[0] <= STRU8 ('9')))
					discard ;
			ix = mNodeTree.insert () ;
			update_shift_e1 () ;
			mNodeTree[ix].mValue = AnyRef<String<STRU8>>::make (_MOVE_ (mLatestString)) ;
			mNodeTree[ix].mClazz = NODE_CLAZZ_STRING ;
			mNodeTree[ix].mParent = curr ;
			mNodeTree[ix].mBrother = VAR_NONE ;
			mNodeTree[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('t') || mRis[0] == STRU8 ('T')))
				if (!(mRis[0] == STRU8 ('f') || mRis[0] == STRU8 ('F')))
					discard ;
			ix = mNodeTree.insert () ;
			update_shift_e2 () ;
			mNodeTree[ix].mValue = AnyRef<String<STRU8>>::make (_MOVE_ (mLatestString)) ;
			mNodeTree[ix].mClazz = NODE_CLAZZ_STRING ;
			mNodeTree[ix].mParent = curr ;
			mNodeTree[ix].mBrother = VAR_NONE ;
			mNodeTree[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('n')))
				discard ;
			ix = mNodeTree.insert () ;
			update_shift_e2x () ;
			mNodeTree[ix].mClazz = NODE_CLAZZ_NULL ;
			mNodeTree[ix].mParent = curr ;
			mNodeTree[ix].mBrother = VAR_NONE ;
			mNodeTree[ix].mChild = VAR_NONE ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('\"')))
				discard ;
			ix = mNodeTree.insert () ;
			update_shift_e3 () ;
			mNodeTree[ix].mValue = AnyRef<String<STRU8>>::make (_MOVE_ (mLatestString)) ;
			mNodeTree[ix].mClazz = NODE_CLAZZ_STRING ;
			mNodeTree[ix].mParent = curr ;
			mNodeTree[ix].mBrother = VAR_NONE ;
			mNodeTree[ix].mChild = VAR_NONE ;
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
			auto &r1x = mNodeTree[curr].mValue.rebind (ARGV<SoftSet<INDEX>>::ID).self ;
			const auto r2x = r1x.length () ;
			r1x.add (r2x ,mLatestIndex) ;
			auto &r3x = _CALL_ ([&] () {
				if (ix == VAR_NONE)
					return _BYREF_ (ix) ;
				return _BYREF_ (mNodeTree[iy].mBrother) ;
			}).self ;
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
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::ID ,mRecursiveCounter)) ;
		mRis >> _PCSTRU8_ ("[") ;
		INDEX ix = mNodeTree.insert () ;
		mNodeTree[ix].mValue = AnyRef<SoftSet<INDEX>>::make (mArraySoftSet.share ()) ;
		mNodeTree[ix].mClazz = NODE_CLAZZ_ARRAY ;
		mNodeTree[ix].mParent = curr ;
		mNodeTree[ix].mBrother = VAR_NONE ;
		mNodeTree[ix].mChild = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		if switch_once (TRUE) {
			if (mRis[0] == STRU8 (']'))
				discard ;
			update_shift_e5 (ix) ;
			mNodeTree[ix].mChild = mLatestIndex ;
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
		auto &r2x = mNodeTree[curr].mValue.rebind (ARGV<SoftSet<String<STRU8>>>::ID).self ;
		r2x.add (r1x ,mLatestIndex) ;
	}

	//@info: $8->$7|$7 , $8
	void update_shift_e8 (const INDEX &curr) {
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		while (TRUE) {
			update_shift_e7 (curr) ;
			const auto r1x = _CALL_ ([&] () {
				if (ix == VAR_NONE)
					return _BYREF_ (ix) ;
				return _BYREF_ (mNodeTree[iy].mBrother) ;
			}) ;
			r1x.self = mLatestIndex ;
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
		ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_ (ARGV<RecursiveCounter>::ID ,mRecursiveCounter)) ;
		mRis >> _PCSTRU8_ ("{") ;
		INDEX ix = mNodeTree.insert () ;
		mNodeTree[ix].mValue = AnyRef<SoftSet<String<STRU8>>>::make (mObjectSoftSet.share ()) ;
		mNodeTree[ix].mClazz = NODE_CLAZZ_OBJECT ;
		mNodeTree[ix].mParent = curr ;
		mNodeTree[ix].mBrother = VAR_NONE ;
		mNodeTree[ix].mChild = VAR_NONE ;
		mRis >> RegularReader::SKIP_GAP ;
		if switch_once (TRUE) {
			if (mRis[0] == STRU8 ('}'))
				discard ;
			update_shift_e8 (ix) ;
			mNodeTree[ix].mChild = mLatestIndex ;
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
		_NOOP_ () ;
	}

	//@info: $12->${end}
	void update_shift_e12 () {
		_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
	}

	void update_heap () {
		mArraySoftSet.clean () ;
		mObjectSoftSet.clean () ;
		const auto r1x = mNodeTree.range () ;
		mHeap = SharedRef<FixedBuffer<NODE_PACK>>::make (r1x.length ()) ;
		for (auto &&i : _RANGE_ (0 ,r1x.length ()))
			mHeap.self[i] = _MOVE_ (mNodeTree[r1x[i]]) ;
	}

	void refresh () {
		mContext.mHeap = _MOVE_ (mHeap) ;
		mContext.mIndex = mRoot ;
	}
} ;

inline exports void JsonParser::initialize (JsonParser &this_ ,const PhanBuffer<const STRU8> &data) {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::InitializeLambda ;
	_CALL_TRY_ (R1X (ARGVP0 ,this_ ,PhanBuffer<const STRU8>::make (data))) ;
}

class CommandParser {
private:
	struct Private {
		class InitializeLambda ;
	} ;

private:
	Set<String<STRU8>> mOptionSet ;
	Deque<String<STRU8>> mAttribute ;
	Set<String<STRU8>> mAttributeMappingSet ;
	ArrayList<String<STRU8>> mCommand ;

public:
	implicit CommandParser () = default ;

	explicit CommandParser (const PhanBuffer<const STRU8> &data) {
		initialize (DEREF[this] ,data) ;
	}

	explicit CommandParser (const VAR32 &argc ,const PTR<const PTR<STRA>> &argv) {
		const auto r1x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8>::make () ;
			auto rax = TextWriter<STRU8> (ret.raw ()) ;
			for (auto &&i : _RANGE_ (1 ,LENGTH (argc))) {
				rax << StringProc::cvt_as_u8s (String<STRA> (PTRTOARR[argv[i]])) ;
				rax << _PCSTRU8_ (" ") ;
			}
			rax << TextWriter<STRU8>::EOS ;
			return _MOVE_ (ret) ;
		}) ;
		initialize (DEREF[this] ,r1x.raw ()) ;
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
		return mAttribute[ix] ;
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

	PTR<const NONE> attribute (const String<STRU8> & ,const PTR<const NONE> &) const = delete ;

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

	const ArrayList<String<STRU8>> &command () const leftvalue {
		return mCommand ;
	}

private:
	imports void initialize (CommandParser &this_ ,const PhanBuffer<const STRU8> &data) ;
} ;

class CommandParser::Private::InitializeLambda :
	delegate private Proxy {
private:
	CommandParser &mContext ;

	TextReader<STRU8> mTextReader ;
	RegularReader mRis ;
	String<STRU8> mLatestString ;

	Set<String<STRU8>> mOptionSet ;
	Deque<String<STRU8>> mAttribute ;
	Set<String<STRU8>> mAttributeMappingSet ;
	ArrayList<String<STRU8>> mCommand ;

public:
	implicit InitializeLambda () = delete ;

	explicit InitializeLambda (const DEF<decltype (ARGVP0)> & ,CommandParser &context_ ,PhanBuffer<const STRU8> &&data) :
		delegate mContext (context_) ,
		delegate mTextReader (PhanBuffer<const STRU8>::make (data)) {}

	inline void operator() () {
		prepare () ;
		generate () ;
		refresh () ;
	}

private:
	void prepare () {
		mRis = RegularReader (PhanRef<TextReader<STRU8>>::make (mTextReader) ,2) ;
		mOptionSet = Set<String<STRU8>> () ;
		mAttribute = Deque<String<STRU8>> () ;
		mAttributeMappingSet = Set<String<STRU8>> () ;
		mCommand = ArrayList<String<STRU8>> () ;
	}

	void generate () {
		/*
		*	$0->$8 $7 $9
		*	$1->${identity}
		*	$2->"${string}"
		*	$3->${word_gap}
		*	$4->/$1
		*	$5->-$1|-$1=$2|-$1=$3
		*	$6->$2|$3
		*	$7->${eps}|$4 $7|$5 $7|$6 $7
		*	$8->${eps}
		*	$9->${end}
		*/
		update_shift_e0 () ;
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

	//@info: $3->${word_gap}
	void update_shift_e3 () {
		mRis >> RegularReader::HINT_WORD_GAP >> mLatestString ;
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
		ix = mAttribute.insert () ;
		mAttributeMappingSet.add (_MOVE_ (mLatestString) ,ix) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('=')))
				discard ;
			if (!(mRis[1] == STRU8 ('\"')))
				discard ;
			mRis >> _PCSTRU8_ ("=") ;
			update_shift_e2 () ;
			mAttribute[ix] = _MOVE_ (mLatestString) ;
		}
		if switch_once (fax) {
			if (!(mRis[0] == STRU8 ('=')))
				discard ;
			mRis >> _PCSTRU8_ ("=") ;
			update_shift_e3 () ;
			mAttribute[ix] = _MOVE_ (mLatestString) ;
		}
		if switch_once (fax) {
			mAttribute[ix] = _PCSTRU8_ ("TRUE") ;
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
		mCommand.add (_MOVE_ (mLatestString)) ;
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
		_NOOP_ () ;
	}

	//@info: $9->${end}
	void update_shift_e9 () {
		_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
	}

	void refresh () {
		mContext.mOptionSet = _MOVE_ (mOptionSet) ;
		mContext.mAttribute = _MOVE_ (mAttribute) ;
		mContext.mAttributeMappingSet = _MOVE_ (mAttributeMappingSet) ;
		mContext.mCommand = _MOVE_ (mCommand) ;
		mContext.mCommand.remap () ;
	}
} ;

inline exports void CommandParser::initialize (CommandParser &this_ ,const PhanBuffer<const STRU8> &data) {
	using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::InitializeLambda ;
	_CALL_TRY_ (R1X (ARGVP0 ,this_ ,PhanBuffer<const STRU8>::make (data))) ;
}
} ;