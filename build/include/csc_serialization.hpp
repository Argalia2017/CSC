#pragma once

#ifndef __CSC_SERIALIZATION__
#define __CSC_SERIALIZATION__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
class XmlParser {
private:
	class Pack {
	private:
		friend XmlParser ;
		String<STRU8> mName ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSet ;
		SoftSet<INDEX ,INDEX> mMemberSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

private:
	SharedRef<FixedBuffer<Pack>> mHeap ;
	INDEX mIndex ;

public:
	XmlParser () {
		mIndex = VAR_NONE ;
	}

	BOOL exist () const {
		return mHeap.exist () && mIndex != VAR_NONE ;
	}

	XmlParser root () const {
		const auto r1x = (exist ()) ? 0 : VAR_NONE ;
		return XmlParser (mHeap ,r1x) ;
	}

	XmlParser parent () const {
		const auto r1x = (exist ()) ? (mHeap.self[mIndex].mParent) : VAR_NONE ;
		return XmlParser (mHeap ,r1x) ;
	}

	XmlParser brother () const {
		const auto r1x = (exist ()) ? (mHeap.self[mIndex].mBrother) : VAR_NONE ;
		return XmlParser (mHeap ,r1x) ;
	}

	XmlParser child () const {
		const auto r1x = (exist ()) ? (mHeap.self[mIndex].mChild) : VAR_NONE ;
		return XmlParser (mHeap ,r1x) ;
	}

	XmlParser child (const String<STRU8> &name) const {
		INDEX ix = (exist ()) ? (mHeap.self[mIndex].mObjectSet.find (name)) : VAR_NONE ;
		if (ix != VAR_NONE)
			ix = mHeap.self[mIndex].mObjectSet[ix].item ;
		return XmlParser (mHeap ,ix) ;
	}

	Array<XmlParser> child_array () const {
		Array<XmlParser> ret ;
		if (exist ()) {
			ret = Array<XmlParser> (mHeap.self[mIndex].mMemberSet.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet)
				ret[iw++] = XmlParser (mHeap ,i.item) ;
		}
		return std::move (ret) ;
	}

	Array<XmlParser> child_array (LENGTH fixed_len) const {
		Array<XmlParser> ret = Array<XmlParser> (fixed_len) ;
		if (exist ()) {
			INDEX iw = 0 ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = XmlParser (mHeap ,i.item) ;
			}
		}
		return std::move (ret) ;
	}

	BOOL equal (const XmlParser &right) const {
		if (!exist () || !right.exist ())
			return !exist () && !right.exist () ;
		return &mHeap.self == &right.mHeap.self && mIndex == right.mIndex ;
	}

	inline BOOL operator== (const XmlParser &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const XmlParser &right) const {
		return !equal (right) ;
	}

	const String<STRU8> &name () const {
		_DYNAMIC_ASSERT_ (exist ()) ;
		return mHeap.self[mIndex].mName ;
	}

	const String<STRU8> &attribute (const String<STRU8> &tag) const {
		auto &r1 = _CACHE_ ([] () {
			return String<STRU8> () ;
		}) ;
		if (!exist ())
			return r1 ;
		INDEX ix = mHeap.self[mIndex].mAttributeSet.find (tag) ;
		if (ix == VAR_NONE)
			return r1 ;
		return mHeap.self[mIndex].mAttributeSet[ix].item ;
	}

	const String<STRU8> &value () const {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mMemberSet.size () == 0) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mAttributeSet.length () == 1) ;
		INDEX ix = mHeap.self[mIndex].mAttributeSet.min_one () ;
		return mHeap.self[mIndex].mAttributeSet[ix].item ;
	}

	template <class _ARG>
	_ARG value (const _ARG &def ,const Function<_ARG (const String<STRU8> &)> &convert) const {
		_ARG ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (value ()) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL value (const BOOL &def) const {
		return value (def ,Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>)) ;
	}

	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		return value (def ,Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>)) ;
	}

	VAR64 value (const VAR64 &def) const {
		return value (def ,Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>)) ;
	}

	VAL32 value (const VAL32 &def) const {
		return value (def ,Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>)) ;
	}

	VAL64 value (const VAL64 &def) const {
		return value (def ,Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>)) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		return value (def ,Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>)) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		return value (def ,Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_)) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		return value (def ,Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_)) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		return value (def ,Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_)) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		return value (def ,Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_)) ;
	}

	void serialize (TextWriter<STRU8> &writer) const ;

private:
	explicit XmlParser (const SharedRef<FixedBuffer<Pack>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;

	void initialize (const Array<XmlParser> &sequence) ;

public:
	inline static XmlParser make (const PhanBuffer<const STRU8> &data) {
		XmlParser ret ;
		ret.initialize (data) ;
		return std::move (ret) ;
	}

	inline static XmlParser make (const Array<XmlParser> &sequence) {
		XmlParser ret ;
		ret.initialize (sequence) ;
		return std::move (ret) ;
	}
} ;

inline void XmlParser::serialize (TextWriter<STRU8> &writer) const {
	auto rax = ARRAY2<Stack<ARRAY2<INDEX>>> () ;
	rax[0].add ({mIndex ,FLAG (0)}) ;
	while (!rax[0].empty ()) {
		const auto r1x = std::move (rax[0][rax[0].peek ()]) ;
		rax[0].take () ;
		if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mName.empty () && r1x[1] == 0) {
			//@info: case '<?xml ...>'
			auto &r1 = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<?xml version=\"1.0\" encoding=\"utf-8\" ?>") ;
			writer << _GAP_ ;
			rax[1].clear () ;
			for (INDEX i = r1.mChild ; i != VAR_NONE ; i = mHeap.self[i].mBrother)
				rax[1].add ({i ,FLAG (0)}) ;
			rax[0].appand (rax[1]) ;
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mChild == VAR_NONE && r1x[1] == 0) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?/>'
			auto &r1 = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<") << r1.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r1.mAttributeSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << i.item << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ ("/>") ;
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mChild != VAR_NONE && r1x[1] == 0) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?>'
			auto &r1 = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<") << r1.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r1.mAttributeSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << i.item << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ (">") ;
			rax[1].clear () ;
			for (INDEX i = r1.mChild ; i != VAR_NONE ; i = mHeap.self[i].mBrother)
				rax[1].add ({i ,FLAG (0)}) ;
			rax[1].add ({r1x[0] ,FLAG (1)}) ;
			rax[0].appand (rax[1]) ;
		} else if (r1x[0] != VAR_NONE && r1x[1] == 1) {
			//@info: case '</xxx>'
			writer << _PCSTRU8_ ("</") << mHeap.self[r1x[0]].mName << _PCSTRU8_ (">") ;
		}
	}
	writer << _EOS_ ;
}

inline void XmlParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		class Counter :private Wrapped<LENGTH> {
		public:
			inline void lock () {
				_DYNAMIC_ASSERT_ (Counter::mData < DEFAULT_RECURSIVE_SIZE::value) ;
				Counter::mData++ ;
			}

			inline void unlock () {
				Counter::mData-- ;
			}
		} ;

	private:
		XmlParser &mContext ;

		TextReader<STRU8> mTextReader ;
		LLTextReader<ARGC<2>> mRis ;
		LENGTH mRecursiveCounter ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<INDEX ,INDEX> mMemberSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Pack ,SAUTO> mNodeHeap ;
		SharedRef<FixedBuffer<Pack>> mHeap ;
		INDEX mRoot ;
		INDEX mLatestIndex ;
		String<STRU8> mLatestString ;

	public:
		inline explicit Lambda (XmlParser &context ,const PhanBuffer<const STRU8> &data) :mContext (context) ,mTextReader (data) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = LLTextReader<ARGC<2>> (PhanRef<TextReader<STRU8>>::make (mTextReader)) ;
			mRecursiveCounter = 0 ;
			mAttributeSoftSet = SoftSet<String<STRU8> ,String<STRU8>> (0) ;
			mMemberSoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Pack ,SAUTO> () ;
			mHeap = SharedRef<FixedBuffer<Pack>> () ;
			mRoot = VAR_NONE ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
			/*
			$0->$8 $7 $9
			$1->记号串
			$2->"文本串"
			$3->$1 = $2
			$4->ε|$3 $4
			$5-><$1 $4 />|<$1 $4 > $7 </$1 >
			$6-><!--注释-->
			$7->ε|$5 $7|$6 $7
			$8->ε|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
			$9->#
			*/
			update_shift_e0 () ;
			_DEBUG_ASSERT_ (mLatestIndex == 0) ;
			mRoot = mLatestIndex ;
			update_heap () ;
		}

		//@info: $0->$8 $7 $9
		inline void update_shift_e0 () {
			update_shift_e8 () ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mMemberSet = mMemberSoftSet.copy () ;
			mNodeHeap[ix].mMemberSet.reset () ;
			mNodeHeap[ix].mObjectSet = mObjectSoftSet.copy () ;
			mNodeHeap[ix].mObjectSet.reset () ;
			mNodeHeap[ix].mParent = VAR_NONE ;
			update_shift_e7 (ix) ;
			mNodeHeap[ix].mChild = mLatestIndex ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			update_shift_e9 () ;
			mLatestIndex = ix ;
		}

		//@info: $1->记号串
		inline void update_shift_e1 () {
			mRis >> LLTextReader<>::HINT_IDENTIFY_TEXT >> mLatestString ;
		}

		//@info: $2->"文本串"
		inline void update_shift_e2 () {
			mRis >> LLTextReader<>::HINT_STRING_TEXT >> mLatestString ;
		}

		//@info: $3->$1 = $2
		inline void update_shift_e3 (INDEX it) {
			update_shift_e1 () ;
			INDEX ix = mNodeHeap[it].mAttributeSet.insert (std::move (mLatestString)) ;
			_DYNAMIC_ASSERT_ (mNodeHeap[it].mAttributeSet[ix].item.empty ()) ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("=") ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			update_shift_e2 () ;
			mNodeHeap[it].mAttributeSet[ix].item = std::move (mLatestString) ;
		}

		//@info: $4->ε|$3 $4
		inline void update_shift_e4 (INDEX it) {
			while (TRUE) {
				const auto r1x = mRis[0] >= STRU8 ('A') && mRis[0] <= STRU8 ('Z') ;
				const auto r2x = mRis[0] >= STRU8 ('a') && mRis[0] <= STRU8 ('z') ;
				const auto r3x = mRis[0] == STRU8 ('_') ;
				if (!r1x && !r2x && !r3x)
					break ;
				update_shift_e3 (it) ;
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
		}

		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		inline void update_shift_e5 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("<") ;
			INDEX ix = mNodeHeap.alloc () ;
			update_shift_e1 () ;
			mNodeHeap[ix].mName = std::move (mLatestString) ;
			mNodeHeap[ix].mAttributeSet = mAttributeSoftSet.copy () ;
			mNodeHeap[ix].mAttributeSet.reset () ;
			mNodeHeap[ix].mParent = it ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			update_shift_e4 (ix) ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			if (mRis[0] == STRU8 ('>')) {
				mRis++ ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				mNodeHeap[ix].mMemberSet = mMemberSoftSet.copy () ;
				mNodeHeap[ix].mMemberSet.reset () ;
				mNodeHeap[ix].mObjectSet = mObjectSoftSet.copy () ;
				mNodeHeap[ix].mObjectSet.reset () ;
				update_shift_e7 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ ("</") ;
				update_shift_e1 () ;
				_DYNAMIC_ASSERT_ (mNodeHeap[ix].mName == mLatestString) ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ (">") ;
			} else {
				mRis >> _PCSTRU8_ ("/>") ;
			}
			mLatestIndex = ix ;
		}

		//@info: $6-><!--注释-->
		inline void update_shift_e6 () {
			mRis >> _PCSTRU8_ ("<!--") ;
			while (TRUE) {
				if (mRis[0] == STRU8 ('\0'))
					break ;
				if (mRis[0] == STRU8 ('-') && mRis[1] == STRU8 ('-'))
					break ;
				mRis++ ;
			}
			mRis >> _PCSTRU8_ ("-->") ;
		}

		//@info: $7->ε|$5 $7|$6 $7
		inline void update_shift_e7 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				const auto r1x = mRis[0] == STRU8 ('<') && mRis[1] == STRU8 ('!') ;
				const auto r2x = mRis[0] == STRU8 ('<') && mRis[1] != STRU8 ('/') ;
				if (!r1x && !r2x)
					break ;
				if (r1x) {
					update_shift_e6 () ;
				} else if (r2x) {
					update_shift_e5 (it) ;
					mNodeHeap[it].mMemberSet.add (mNodeHeap[it].mMemberSet.length () ,mLatestIndex) ;
					mNodeHeap[it].mObjectSet.add (mNodeHeap[mLatestIndex].mName ,mLatestIndex) ;
					auto &r1 = (ix == VAR_NONE) ? ix : (mNodeHeap[iy].mBrother) ;
					r1 = mLatestIndex ;
					iy = mLatestIndex ;
				}
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
			mLatestIndex = ix ;
		}

		//@info: $8->ε|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		inline void update_shift_e8 () {
			if (mRis[0] != STRU8 ('<'))
				return ;
			if (mRis[1] != STRU8 ('?'))
				return ;
			mRis++ ;
			mRis++ ;
			mRis >> _PCSTRU8_ ("xml") ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("version") ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("=") ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("\"1.0\"") ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			if (mRis[0] != STRU8 ('?')) {
				mRis >> _PCSTRU8_ ("encoding") ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ ("=") ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ ("\"utf-8\"") ;
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("?>") ;
		}

		//@info: $9->#
		inline void update_shift_e9 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_heap () {
			mAttributeSoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Pack>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ; i < mNodeHeap.size () ; i++) {
				if (!mNodeHeap.used (i))
					continue ;
				mHeap.self[iw++] = std::move (mNodeHeap[i]) ;
			}
			_DEBUG_ASSERT_ (iw == mHeap->size ()) ;
		}

		inline void refresh () {
			mContext.mHeap = std::move (mHeap) ;
			mContext.mIndex = mRoot ;
		}
	} ;
	_CALL_ (Lambda (*this ,data)) ;
}

inline void XmlParser::initialize (const Array<XmlParser> &sequence) {
	class Lambda {
	private:
		XmlParser &mContext ;
		const Array<XmlParser> &mSequence ;
		const String<STRU8> mTypeString = String<STRU8> (_PCSTRU8_ ("type")) ;
		const String<STRU8> mTableTypeString = String<STRU8> (_PCSTRU8_ ("table")) ;
		const String<STRU8> mObjectTypeString = String<STRU8> (_PCSTRU8_ ("object")) ;
		const String<STRU8> mArrayTypeString = String<STRU8> (_PCSTRU8_ ("array")) ;
		const String<STRU8> mFinalTypeString = String<STRU8> (_PCSTRU8_ ("final")) ;

		Stack<PACK<Queue<XmlParser> ,INDEX ,ARRAY2<INDEX>>> mNodeStack ;
		ARRAY4<DEF<DEF<void (const XmlParser &)> Lambda::*>> mFoundNodeType ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<INDEX ,INDEX> mMemberSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Pack ,SAUTO> mNodeHeap ;
		String<STRU8> mRootName ;
		INDEX mRootType ;
		Queue<ARRAY4<INDEX>> mFoundNodeList ;
		Set<String<STRU8> ,INDEX> mFoundNodeNameSet ;
		Queue<SoftSet<String<STRU8> ,String<STRU8>>> mFoundNodeAttributeSet ;
		Queue<Queue<XmlParser>> mFoundNodeBase ;
		Queue<Queue<XmlParser>> mFoundNodeBaseRecycle ;
		SharedRef<FixedBuffer<Pack>> mHeap ;
		INDEX mRoot ;

		PACK<Queue<XmlParser> ,INDEX ,ARRAY2<INDEX>> mTempNode ;

	public:
		inline explicit Lambda (XmlParser &context ,const Array<XmlParser> &sequence) :mContext (context) ,mSequence (sequence) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mNodeStack = Stack<PACK<Queue<XmlParser> ,INDEX ,ARRAY2<INDEX>>> () ;
			mFoundNodeType[0] = &Lambda::update_found_table_node ;
			mFoundNodeType[1] = &Lambda::update_found_object_node ;
			mFoundNodeType[2] = &Lambda::update_found_array_node ;
			mFoundNodeType[3] = &Lambda::update_found_table_node ;
			mAttributeSoftSet = SoftSet<String<STRU8> ,String<STRU8>> (0) ;
			mMemberSoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Pack ,SAUTO> () ;
			INDEX ix = normal_node_one () ;
			mRootName = (ix != VAR_NONE) ? (mSequence[ix].name ()) : (String<STRU8> ()) ;
			mRootType = (ix != VAR_NONE) ? (node_type (mSequence[ix])) : VAR_NONE ;
			for (auto &&i : mSequence) {
				if (!i.exist ())
					continue ;
				_DYNAMIC_ASSERT_ (i.name () == mRootName) ;
				_DYNAMIC_ASSERT_ (node_type (i) == mRootType) ;
			}
			mRoot = mNodeHeap.alloc () ;
			mNodeHeap[mRoot].mMemberSet = mMemberSoftSet.copy () ;
			mNodeHeap[mRoot].mMemberSet.reset () ;
			mNodeHeap[mRoot].mObjectSet = mObjectSoftSet.copy () ;
			mNodeHeap[mRoot].mObjectSet.reset () ;
			mNodeHeap[mRoot].mParent = VAR_NONE ;
			mNodeHeap[mRoot].mChild = VAR_NONE ;
			mNodeHeap[mRoot].mBrother = VAR_NONE ;
			mFoundNodeList = Queue<ARRAY4<INDEX>> () ;
			mFoundNodeNameSet = Set<String<STRU8> ,INDEX> () ;
			mFoundNodeAttributeSet = Queue<SoftSet<String<STRU8> ,String<STRU8>>> () ;
			mFoundNodeBase = Queue<Queue<XmlParser>> () ;
			mFoundNodeBaseRecycle = Queue<Queue<XmlParser>> () ;
			mHeap = SharedRef<FixedBuffer<Pack>>::make () ;
		}

		inline INDEX normal_node_one () const {
			for (auto &&i : mSequence)
				if (i.exist ())
					return mSequence.at (i) ;
			return VAR_NONE ;
		}

		inline INDEX node_type (const XmlParser &node) const {
			auto &r1 = node.attribute (mTypeString) ;
			if (r1 == mTableTypeString)
				return 0 ;
			if (r1 == mObjectTypeString)
				return 1 ;
			if (r1 == mArrayTypeString)
				return 2 ;
			if (r1 == mFinalTypeString)
				return 3 ;
			_DYNAMIC_ASSERT_ (r1.empty ()) ;
			return 0 ;
		}

		inline void generate () {
			update_root_node () ;
			while (!mNodeStack.empty ()) {
				mNodeStack.take (mTempNode) ;
				if (mTempNode.P2 == VAR_NONE)
					continue ;
				const auto r1x = mFoundNodeType[mTempNode.P2] ;
				for (auto &&i : mTempNode.P1)
					(this->*r1x) (i) ;
				update_merge_found_node (mTempNode.P3[0]) ;
				mTempNode.P1.clear () ;
				mFoundNodeBaseRecycle.add (std::move (mTempNode.P1)) ;
			}
			update_heap () ;
		}

		inline void update_root_node () {
			INDEX ix = mNodeStack.insert () ;
			mNodeStack[ix].P1 = Queue<XmlParser> (mSequence.length ()) ;
			for (auto &&i : mSequence)
				mNodeStack[ix].P1.add (i.child ()) ;
			mNodeStack[ix].P2 = mRootType ;
			INDEX iy = mRoot ;
			if (!mRootName.empty ()) {
				iy = mNodeHeap.alloc () ;
				mNodeHeap[iy].mName = std::move (mRootName) ;
				mNodeHeap[iy].mAttributeSet = mAttributeSoftSet.copy () ;
				mNodeHeap[iy].mAttributeSet.reset () ;
				for (auto &&i : mSequence) {
					if (!i.exist ())
						continue ;
					mNodeHeap[iy].mAttributeSet.appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				}
				mNodeHeap[iy].mParent = mRoot ;
				mNodeHeap[mRoot].mChild = iy ;
				mNodeHeap[mRoot].mMemberSet.add (mNodeHeap[mRoot].mMemberSet.length () ,mNodeHeap[mRoot].mChild) ;
				mNodeHeap[mRoot].mObjectSet.add (mNodeHeap[iy].mName ,mNodeHeap[mRoot].mChild) ;
				mNodeHeap[iy].mChild = VAR_NONE ;
				mNodeHeap[iy].mBrother = VAR_NONE ;
			}
			mNodeStack[ix].P3[0] = iy ;
			mNodeStack[ix].P3[1] = VAR_NONE ;
		}

		inline void update_found_table_node (const XmlParser &it) {
			for (XmlParser i = it ; i.exist () ; i = i.brother ()) {
				INDEX ix = find_found_node_name (i.name ()) ;
				INDEX iy = mFoundNodeList.insert () ;
				const auto r1x = mFoundNodeNameSet.length () ;
				mFoundNodeList[iy][0] = mFoundNodeNameSet.insert (i.name ()) ;
				if (mFoundNodeNameSet.length () > r1x)
					mFoundNodeNameSet[mFoundNodeList[iy][0]].item = iy ;
				mFoundNodeList[iy][1] = mFoundNodeAttributeSet.insert () ;
				mFoundNodeList[iy][2] = node_type (i) ;
				mFoundNodeList[iy][3] = mFoundNodeBase.insert () ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[ix][2] != 3) ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[iy][2] != 3) ;
				_DYNAMIC_ASSERT_ (mFoundNodeList[iy][2] != 3 || i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]] = mAttributeSoftSet.copy () ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]].reset () ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]].appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				if (mFoundNodeBaseRecycle.empty ())
					mFoundNodeBaseRecycle.add (Queue<XmlParser> ()) ;
				mFoundNodeBaseRecycle.take (mFoundNodeBase[mFoundNodeList[iy][3]]) ;
				mFoundNodeBase[mFoundNodeList[iy][3]].add (i.child ()) ;
			}
		}

		inline void update_found_object_node (const XmlParser &it) {
			for (XmlParser i = it ; i.exist () ; i = i.brother ()) {
				INDEX ix = find_found_node_name (i.name ()) ;
				INDEX iy = ix ;
				for (FOR_ONCE_DO_WHILE_FALSE) {
					if (iy != VAR_NONE)
						continue ;
					iy = mFoundNodeList.insert () ;
					const auto r1x = mFoundNodeNameSet.length () ;
					mFoundNodeList[iy][0] = mFoundNodeNameSet.insert (i.name ()) ;
					if (mFoundNodeNameSet.length () > r1x)
						mFoundNodeNameSet[mFoundNodeList[iy][0]].item = iy ;
					mFoundNodeList[iy][1] = mFoundNodeAttributeSet.insert () ;
					mFoundNodeList[iy][2] = node_type (i) ;
					mFoundNodeList[iy][3] = mFoundNodeBase.insert () ;
					mFoundNodeAttributeSet[mFoundNodeList[iy][1]] = mAttributeSoftSet.copy () ;
					mFoundNodeAttributeSet[mFoundNodeList[iy][1]].reset () ;
					if (mFoundNodeBaseRecycle.empty ())
						mFoundNodeBaseRecycle.add (Queue<XmlParser> ()) ;
					mFoundNodeBaseRecycle.take (mFoundNodeBase[mFoundNodeList[iy][3]]) ;
				}
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[ix][2] == mFoundNodeList[iy][2]) ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[ix][2] != 3) ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[iy][2] != 3) ;
				_DYNAMIC_ASSERT_ (mFoundNodeList[iy][2] != 3 || i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]].appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				mFoundNodeBase[mFoundNodeList[iy][3]].add (i.child ()) ;
			}
		}

		inline void update_found_array_node (const XmlParser &it) {
			if (!mFoundNodeList.empty ())
				return ;
			for (XmlParser i = it ; i.exist () ; i = i.brother ()) {
				INDEX ix = mFoundNodeNameSet.min_one () ;
				INDEX iy = mFoundNodeList.insert () ;
				const auto r1x = mFoundNodeNameSet.length () ;
				mFoundNodeList[iy][0] = mFoundNodeNameSet.insert (i.name ()) ;
				if (mFoundNodeNameSet.length () > r1x)
					mFoundNodeNameSet[mFoundNodeList[iy][0]].item = iy ;
				mFoundNodeList[iy][1] = mFoundNodeAttributeSet.insert () ;
				mFoundNodeList[iy][2] = node_type (i) ;
				mFoundNodeList[iy][3] = mFoundNodeBase.insert () ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[ix][0] == mFoundNodeList[iy][0]) ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[ix][2] != 3) ;
				_DYNAMIC_ASSERT_ (ix == VAR_NONE || mFoundNodeList[iy][2] != 3) ;
				_DYNAMIC_ASSERT_ (mFoundNodeList[iy][2] != 3 || i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]] = mAttributeSoftSet.copy () ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]].reset () ;
				mFoundNodeAttributeSet[mFoundNodeList[iy][1]].appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				if (mFoundNodeBaseRecycle.empty ())
					mFoundNodeBaseRecycle.add (Queue<XmlParser> ()) ;
				mFoundNodeBaseRecycle.take (mFoundNodeBase[mFoundNodeList[iy][3]]) ;
				mFoundNodeBase[mFoundNodeList[iy][3]].add (i.child ()) ;
			}
		}

		inline INDEX find_found_node_name (const String<STRU8> &name) const {
			INDEX ret = mFoundNodeNameSet.find (name) ;
			if (ret != VAR_NONE)
				ret = mFoundNodeNameSet[ret].item ;
			return std::move (ret) ;
		}

		inline void update_merge_found_node (INDEX it) {
			INDEX iz = VAR_NONE ;
			for (auto &&i : mFoundNodeList) {
				INDEX ix = mNodeHeap.alloc () ;
				mNodeHeap[ix].mName = std::move (mFoundNodeNameSet[i[0]]) ;
				mNodeHeap[ix].mAttributeSet = std::move (mFoundNodeAttributeSet[i[1]]) ;
				mNodeHeap[ix].mParent = it ;
				INDEX iy = mNodeHeap[ix].mParent ;
				if (iy != VAR_NONE && mNodeHeap[iy].mChild == VAR_NONE)
					mNodeHeap[iy].mChild = ix ;
				if (iy != VAR_NONE && mNodeHeap[iy].mMemberSet.size () == 0) {
					mNodeHeap[iy].mMemberSet = mMemberSoftSet.copy () ;
					mNodeHeap[iy].mMemberSet.reset () ;
					mNodeHeap[iy].mObjectSet = mObjectSoftSet.copy () ;
					mNodeHeap[iy].mObjectSet.reset () ;
				}
				if (iy != VAR_NONE) {
					mNodeHeap[iy].mMemberSet.add (mNodeHeap[iy].mMemberSet.length () ,ix) ;
					mNodeHeap[iy].mObjectSet.add (mNodeHeap[ix].mName ,ix) ;
				}
				mNodeHeap[ix].mBrother = VAR_NONE ;
				if (iz != VAR_NONE)
					mNodeHeap[iz].mBrother = ix ;
				mNodeHeap[ix].mChild = VAR_NONE ;
				if (mRoot == VAR_NONE)
					mRoot = ix ;
				iz = ix ;
				INDEX jx = mNodeStack.insert () ;
				mNodeStack[jx].P1 = std::move (mFoundNodeBase[i[3]]) ;
				mNodeStack[jx].P2 = i[2] ;
				mNodeStack[jx].P3[0] = ix ;
				mNodeStack[jx].P3[1] = VAR_NONE ;
			}
			mFoundNodeNameSet.clear () ;
			mFoundNodeAttributeSet.clear () ;
			mFoundNodeBase.clear () ;
			mFoundNodeList.clear () ;
		}

		inline void update_heap () {
			mAttributeSoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Pack>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ; i < mNodeHeap.size () ; i++) {
				if (!mNodeHeap.used (i))
					continue ;
				mHeap.self[iw++] = std::move (mNodeHeap[i]) ;
			}
			_DEBUG_ASSERT_ (iw == mHeap->size ()) ;
		}

		inline void refresh () {
			mContext.mHeap = std::move (mHeap) ;
			mContext.mIndex = mRoot ;
		}
	} ;
	_CALL_ (Lambda (*this ,sequence)) ;
}

class JsonParser {
private:
	static constexpr auto TYPE_ID_STRING = FLAG (1) ;
	static constexpr auto TYPE_ID_ARRAY = FLAG (2) ;
	static constexpr auto TYPE_ID_OBJECT = FLAG (3) ;

	class Pack {
	private:
		friend JsonParser ;
		AnyRef<void> mValue ;
		FLAG mTypeID ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

private:
	SharedRef<FixedBuffer<Pack>> mHeap ;
	INDEX mIndex ;

public:
	JsonParser () {
		mIndex = VAR_NONE ;
	}

	BOOL exist () const {
		return mHeap.exist () && mIndex != VAR_NONE ;
	}

	BOOL string_type () const {
		return mHeap.self[mIndex].mTypeID == TYPE_ID_STRING ;
	}

	BOOL array_type () const {
		return mHeap.self[mIndex].mTypeID == TYPE_ID_ARRAY ;
	}

	BOOL object_type () const {
		return mHeap.self[mIndex].mTypeID == TYPE_ID_OBJECT ;
	}

	JsonParser root () const {
		const auto r1x = (exist ()) ? 0 : VAR_NONE ;
		return JsonParser (mHeap ,r1x) ;
	}

	JsonParser parent () const {
		const auto r1x = (exist ()) ? (mHeap.self[mIndex].mParent) : VAR_NONE ;
		return JsonParser (mHeap ,r1x) ;
	}

	JsonParser brother () const {
		const auto r1x = (exist ()) ? (mHeap.self[mIndex].mBrother) : VAR_NONE ;
		return JsonParser (mHeap ,r1x) ;
	}

	JsonParser child () const {
		const auto r1x = (exist ()) ? (mHeap.self[mIndex].mChild) : VAR_NONE ;
		return JsonParser (mHeap ,r1x) ;
	}

	JsonParser child (const String<STRU8> &key) const {
		INDEX ix = VAR_NONE ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!exist ())
				continue ;
			if (!object_type ())
				continue ;
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			ix = r1.find (key) ;
			if (ix == VAR_NONE)
				continue ;
			ix = r1[ix].item ;
		}
		return JsonParser (mHeap ,ix) ;
	}

	Array<JsonParser> child_array () const {
		Array<JsonParser> ret ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!exist ())
				continue ;
			if (!array_type ())
				continue ;
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			ret = Array<JsonParser> (r1.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : r1)
				ret[iw++] = JsonParser (mHeap ,i.item) ;
		}
		return std::move (ret) ;
	}

	Array<JsonParser> child_array (LENGTH fixed_len) const {
		Array<JsonParser> ret = Array<JsonParser> (fixed_len) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!exist ())
				continue ;
			if (!array_type ())
				continue ;
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			INDEX iw = 0 ;
			for (auto &&i : r1) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = JsonParser (mHeap ,i.item) ;
			}
		}
		return std::move (ret) ;
	}

	BOOL equal (const JsonParser &right) const {
		if (!exist () || !right.exist ())
			return !exist () && !right.exist () ;
		return &mHeap.self == &right.mHeap.self && mIndex == right.mIndex ;
	}

	inline BOOL operator== (const JsonParser &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const JsonParser &right) const {
		return !equal (right) ;
	}

	const String<STRU8> &value () const {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (string_type ()) ;
		return mHeap.self[mIndex].mValue.rebind<String<STRU8>> () ;
	}

	template <class _ARG>
	_ARG value (const _ARG &def ,const Function<_ARG (const String<STRU8> &)> &convert) const {
		_ARG ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (value ()) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL value (const BOOL &def) const {
		return value (def ,Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>)) ;
	}

	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		return value (def ,Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>)) ;
	}

	VAR64 value (const VAR64 &def) const {
		return value (def ,Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>)) ;
	}

	VAL32 value (const VAL32 &def) const {
		return value (def ,Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>)) ;
	}

	VAL64 value (const VAL64 &def) const {
		return value (def ,Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>)) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		return value (def ,Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>)) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		return value (def ,Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_)) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		return value (def ,Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_)) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		return value (def ,Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_)) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		return value (def ,Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_)) ;
	}

	void serialize (TextWriter<STRU8> &writer) const ;

private:
	explicit JsonParser (const SharedRef<FixedBuffer<Pack>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;

public:
	inline static JsonParser make (const PhanBuffer<const STRU8> &data) {
		JsonParser ret ;
		ret.initialize (data) ;
		return std::move (ret) ;
	}
} ;

inline void JsonParser::serialize (TextWriter<STRU8> &writer) const {
	auto rax = ARRAY2<Stack<ARRAY2<INDEX>>> () ;
	rax[0].add ({mIndex ,FLAG (0)}) ;
	auto rbx = Queue<PTR<const String<STRU8>>> (mHeap->size ()) ;
	while (!rax[0].empty ()) {
		const auto r1x = std::move (rax[0][rax[0].peek ()]) ;
		rax[0].take () ;
		if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mTypeID == TYPE_ID_STRING && r1x[1] == 0) {
			//@info: case '"xxx"'
			auto &r1 = mHeap.self[r1x[0]].mValue.rebind<String<STRU8>> ().self ;
			writer << _PCSTRU8_ ("\"") ;
			writer << r1 ;
			writer << _PCSTRU8_ ("\"") ;
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mTypeID == TYPE_ID_ARRAY && r1x[1] == 0) {
			//@info: case '[(yyy(,yyy)*)?]'
			auto &r1 = mHeap.self[r1x[0]].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			rax[1].clear () ;
			rax[1].add ({VAR_NONE ,FLAG (2)}) ;
			INDEX ir = 0 ;
			for (auto &&i : r1) {
				if (ir > 0)
					rax[1].add ({VAR_NONE ,FLAG (3)}) ;
				ir++ ;
				rax[1].add ({i.item ,FLAG (0)}) ;
			}
			rax[1].add ({VAR_NONE ,FLAG (4)}) ;
			rax[0].appand (rax[1]) ;
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mTypeID == TYPE_ID_OBJECT && r1x[1] == 0) {
			//@info: case '{("xxx":yyy(,"xxx":yyy)*)?}'
			auto &r1 = mHeap.self[r1x[0]].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			rax[1].clear () ;
			rax[1].add ({VAR_NONE ,FLAG (5)}) ;
			INDEX ir = 0 ;
			for (auto &&i : r1) {
				if (ir > 0)
					rax[1].add ({VAR_NONE ,FLAG (6)}) ;
				ir++ ;
				INDEX ix = rbx.insert () ;
				rbx[ix] = &i.key ;
				rax[1].add ({ix ,FLAG (1)}) ;
				rax[1].add ({VAR_NONE ,FLAG (7)}) ;
				rax[1].add ({i.item ,FLAG (0)}) ;
			}
			rax[1].add ({VAR_NONE ,FLAG (8)}) ;
			rax[0].appand (rax[1]) ;
		} else if (r1x[0] != VAR_NONE && r1x[1] == 1) {
			writer << _PCSTRU8_ ("\"") ;
			writer << *rbx[r1x[0]] ;
			writer << _PCSTRU8_ ("\"") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 2) {
			writer << _PCSTRU8_ ("[") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 3) {
			writer << _PCSTRU8_ (",") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 4) {
			writer << _PCSTRU8_ ("]") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 5) {
			writer << _PCSTRU8_ ("{") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 6) {
			writer << _PCSTRU8_ (",") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 7) {
			writer << _PCSTRU8_ (":") ;
		} else if (r1x[0] == VAR_NONE && r1x[1] == 8) {
			writer << _PCSTRU8_ ("}") ;
		}
	}
	writer << _EOS_ ;
}

inline void JsonParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		class Counter :private Wrapped<LENGTH> {
		public:
			inline void lock () {
				_DYNAMIC_ASSERT_ (Counter::mData < DEFAULT_RECURSIVE_SIZE::value) ;
				Counter::mData++ ;
			}

			inline void unlock () {
				Counter::mData-- ;
			}
		} ;

	private:
		JsonParser &mContext ;

		TextReader<STRU8> mTextReader ;
		LLTextReader<ARGC<2>> mRis ;
		LENGTH mRecursiveCounter ;
		SoftSet<INDEX ,INDEX> mArraySoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Pack ,SAUTO> mNodeHeap ;
		SharedRef<FixedBuffer<Pack>> mHeap ;
		INDEX mRoot ;
		INDEX mLatestIndex ;
		String<STRU8> mLatestString ;

	public:
		inline explicit Lambda (JsonParser &context ,const PhanBuffer<const STRU8> &data) :mContext (context) ,mTextReader (data) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = LLTextReader<ARGC<2>> (PhanRef<TextReader<STRU8>>::make (mTextReader)) ;
			mRecursiveCounter = 0 ;
			mArraySoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Pack ,SAUTO> () ;
			mHeap = SharedRef<FixedBuffer<Pack>> () ;
			mRoot = VAR_NONE ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
			/*
			$0->$11 $10 $12
			$1->数值串
			$2->true|false|null
			$3->"文本串"
			$4->$1|$2|$3|$6|$9
			$5->$4|$4 , $5
			$6->[ ]|[ $5 ]
			$7->$2 : $4
			$8->$7|$7 , $8
			$9->{ }|{ $8 }
			$10->ε|$4
			$11->#
			$12->#
			*/
			update_shift_e0 () ;
			_DEBUG_ASSERT_ (mLatestIndex == 0) ;
			mRoot = mLatestIndex ;
			update_heap () ;
		}

		//@info: $0->$11 $10 $12
		inline void update_shift_e0 () {
			update_shift_e11 () ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			update_shift_e10 () ;
			INDEX ix = mLatestIndex ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			update_shift_e12 () ;
			mLatestIndex = ix ;
		}

		//@info: $1->数值串
		inline void update_shift_e1 () {
			mRis >> LLTextReader<>::HINT_VALUE_TEXT >> mLatestString ;
		}

		//@info: $2->true|false|null
		inline void update_shift_e2 () {
			if (mRis[0] == STRU8 ('t')) {
				mRis >> _PCSTRU8_ ("true") ;
				mLatestString = _PCSTRU8_ ("true") ;
			} else if (mRis[0] == STRU8 ('f')) {
				mRis >> _PCSTRU8_ ("false") ;
				mLatestString = _PCSTRU8_ ("false") ;
			} else if (mRis[0] == STRU8 ('n')) {
				mRis >> _PCSTRU8_ ("null") ;
				mLatestString = String<STRU8> () ;
			}
		}

		//@info: $3->"文本串"
		inline void update_shift_e3 () {
			mRis >> LLTextReader<>::HINT_STRING_TEXT >> mLatestString ;
		}

		//@info: $4->$1|$2|$3|$6|$9
		inline void update_shift_e4 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			INDEX ix = VAR_NONE ;
			const auto r1x = mRis[0] == STRU8 ('+') || mRis[0] == STRU8 ('-') || (mRis[0] >= STRU8 ('0') && mRis[0] <= STRU8 ('9')) ;
			const auto r2x = mRis[0] == STRU8 ('t') || mRis[0] == STRU8 ('f') || mRis[0] == STRU8 ('n') ;
			_DYNAMIC_ASSERT_ (r1x || r2x || mRis[0] == STRU8 ('\"') || mRis[0] == STRU8 ('[') || mRis[0] == STRU8 ('{')) ;
			if (r1x) {
				ix = mNodeHeap.alloc () ;
				update_shift_e1 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mTypeID = TYPE_ID_STRING ;
				mNodeHeap[ix].mParent = it ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			} else if (r2x) {
				ix = mNodeHeap.alloc () ;
				update_shift_e2 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mTypeID = TYPE_ID_STRING ;
				mNodeHeap[ix].mParent = it ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			} else if (mRis[0] == STRU8 ('\"')) {
				ix = mNodeHeap.alloc () ;
				update_shift_e3 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mTypeID = TYPE_ID_STRING ;
				mNodeHeap[ix].mParent = it ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			} else if (mRis[0] == STRU8 ('[')) {
				update_shift_e6 (it) ;
				ix = mLatestIndex ;
			} else if (mRis[0] == STRU8 ('{')) {
				update_shift_e9 (it) ;
				ix = mLatestIndex ;
			}
			mLatestIndex = ix ;
		}

		//@info: $5->$4|$4 , $5
		inline void update_shift_e5 (INDEX it) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e4 (it) ;
				auto &r1 = mNodeHeap[it].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
				r1.add (r1.length () ,mLatestIndex) ;
				auto &r2 = (ix == VAR_NONE) ? ix : (mNodeHeap[iy].mBrother) ;
				r2 = mLatestIndex ;
				iy = mLatestIndex ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				if (mRis[0] != STRU8 (','))
					break ;
				mRis++ ;
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
			mLatestIndex = ix ;
		}

		//@info: $6->[ ]|[ $5 ]
		inline void update_shift_e6 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("[") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<INDEX ,INDEX>>::make (mArraySoftSet.copy ()) ;
			mNodeHeap[ix].mValue.rebind<SoftSet<INDEX ,INDEX>> ()->reset () ;
			mNodeHeap[ix].mTypeID = TYPE_ID_ARRAY ;
			mNodeHeap[ix].mParent = it ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			if (mRis[0] != STRU8 (']')) {
				update_shift_e5 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("]") ;
			mLatestIndex = ix ;
		}

		//@info: $7->$2 : $4
		inline void update_shift_e7 (INDEX it) {
			update_shift_e3 () ;
			auto &r1 = mNodeHeap[it].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			INDEX ix = r1.insert (std::move (mLatestString)) ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ (":") ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			update_shift_e4 (it) ;
			r1[ix].item = mLatestIndex ;
		}

		//@info: $8->$7|$7 , $8
		inline void update_shift_e8 (INDEX it) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e7 (it) ;
				auto &r1 = (ix == VAR_NONE) ? ix : (mNodeHeap[iy].mBrother) ;
				r1 = mLatestIndex ;
				iy = mLatestIndex ;
				mRis >> LLTextReader<>::SKIP_GAP ;
				if (mRis[0] != STRU8 (','))
					break ;
				mRis++ ;
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
			mLatestIndex = ix ;
		}

		//@info: $9->{ }|{ $8 }
		inline void update_shift_e9 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("{") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<String<STRU8> ,INDEX>>::make (mObjectSoftSet.copy ()) ;
			mNodeHeap[ix].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ()->reset () ;
			mNodeHeap[ix].mTypeID = TYPE_ID_OBJECT ;
			mNodeHeap[ix].mParent = it ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> LLTextReader<>::SKIP_GAP ;
			if (mRis[0] != STRU8 ('}')) {
				update_shift_e8 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> LLTextReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("}") ;
			mLatestIndex = ix ;
		}

		//@info: $10->ε|$4
		inline void update_shift_e10 () {
			INDEX ix = VAR_NONE ;
			if (mRis[0] != STRU8 ('\0')) {
				update_shift_e4 (VAR_NONE) ;
				ix = mLatestIndex ;
			}
			mLatestIndex = ix ;
		}

		//@info: $11->#
		inline void update_shift_e11 () {
			_STATIC_WARNING_ ("noop") ;
		}

		//@info: $12->#
		inline void update_shift_e12 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_heap () {
			mArraySoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Pack>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ; i < mNodeHeap.size () ; i++) {
				if (!mNodeHeap.used (i))
					continue ;
				mHeap.self[iw++] = std::move (mNodeHeap[i]) ;
			}
			_DEBUG_ASSERT_ (iw == mHeap->size ()) ;
		}

		inline void refresh () {
			mContext.mHeap = std::move (mHeap) ;
			mContext.mIndex = mRoot ;
		}
	} ;
	_CALL_ (Lambda (*this ,data)) ;
}

class CommandParser {
private:
	Set<String<STRU8>> mOptionSet ;
	Set<String<STRU8> ,String<STRU8>> mAttributeSet ;
	Array<String<STRU8>> mCommand ;

public:
	CommandParser () = delete ;

	explicit CommandParser (const PhanBuffer<const STRU8> &data) {
		initialize (data) ;
	}

#ifdef __CSC_DEPRECATED__
	explicit CommandParser (VAR32 argc ,PTR<const PTR<STRA>> argv) {
		const auto r1x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8> (DEFAULT_LONGSTRING_SIZE::value) ;
			auto wos = TextWriter<STRU8> (ret.raw ()) ;
			for (INDEX i = 1 ,ie = LENGTH (argc) ; i < ie ; i++) {
				wos << _ASTOU8S_ (PTRTOARR[&argv[i][0]]) ;
				wos << _PCSTRU8_ (" ") ;
			}
			wos << _EOS_ ;
			return std::move (ret) ;
		}) ;
		initialize (r1x.raw ()) ;
	}
#endif

	BOOL option (const String<STRU8> &tag) const {
		return mOptionSet.find (tag) != VAR_NONE ;
	}

	const String<STRU8> &attribute (const String<STRU8> &tag) const {
		INDEX ix = mAttributeSet.find (tag) ;
		if (ix == VAR_NONE)
			ix = 0 ;
		return mAttributeSet[ix].item ;
	}

	template <class _ARG>
	_ARG attribute (const String<STRU8> &tag ,const _ARG &def ,const Function<_ARG (const String<STRU8> &)> &convert) const {
		_ARG ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (attribute (tag)) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL attribute (const String<STRU8> &tag ,const BOOL &def) const {
		return attribute (tag ,def ,Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>)) ;
	}

	PTR<const VOID> attribute (const String<STRU8> & ,const PTR<const VOID> &) const = delete ;

	VAR32 attribute (const String<STRU8> &tag ,const VAR32 &def) const {
		return attribute (tag ,def ,Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>)) ;
	}

	VAR64 attribute (const String<STRU8> &tag ,const VAR64 &def) const {
		return attribute (tag ,def ,Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>)) ;
	}

	VAL32 attribute (const String<STRU8> &tag ,const VAL32 &def) const {
		return attribute (tag ,def ,Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>)) ;
	}

	VAL64 attribute (const String<STRU8> &tag ,const VAL64 &def) const {
		return attribute (tag ,def ,Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>)) ;
	}

	String<STRU8> attribute (const String<STRU8> &tag ,const String<STRU8> &def) const {
		return attribute (tag ,def ,Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>)) ;
	}

	String<STRU16> attribute (const String<STRU8> &tag ,const String<STRU16> &def) const {
		return attribute (tag ,def ,Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_)) ;
	}

	String<STRU32> attribute (const String<STRU8> &tag ,const String<STRU32> &def) const {
		return attribute (tag ,def ,Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_)) ;
	}

	String<STRA> attribute (const String<STRU8> &tag ,const String<STRA> &def) const {
		return attribute (tag ,def ,Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_)) ;
	}

	String<STRW> attribute (const String<STRU8> &tag ,const String<STRW> &def) const {
		return attribute (tag ,def ,Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_)) ;
	}

	const Array<String<STRU8>> &command () const {
		return mCommand ;
	}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;
} ;

inline void CommandParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		CommandParser &mContext ;

		TextReader<STRU8> mTextReader ;
		LLTextReader<ARGC<2>> mRis ;
		Set<String<STRU8>> mOptionSet ;
		Set<String<STRU8> ,String<STRU8>> mAttributeSet ;
		Queue<String<STRU8>> mCommandList ;
		Array<String<STRU8>> mCommand ;
		String<STRU8> mLatestString ;

	public:
		inline explicit Lambda (CommandParser &context ,const PhanBuffer<const STRU8> &data) :mContext (context) ,mTextReader (data) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = LLTextReader<ARGC<2>> (PhanRef<TextReader<STRU8>>::make (mTextReader)) ;
			mOptionSet = Set<String<STRU8>> () ;
			mAttributeSet = Set<String<STRU8> ,String<STRU8>> () ;
			mCommandList = Queue<String<STRU8>> () ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
			/*
			$0->$8 $7 $9
			$1->记号串
			$2->"文本串"
			$3->单字符串
			$4->/$1
			$5->-$1|-$1=$2|-$1=$3
			$6->$2|$3
			$7->ε|$4 $7|$5 $7|$6 $7
			$8->#
			$9->#
			*/
			update_shift_e0 () ;
			update_command () ;
		}

		//@info: $0->$8 $7 $9
		inline void update_shift_e0 () {
			update_shift_e8 () ;
			mRis >> LLTextReader<>::SKIP_GAP_SPACE_ONLY ;
			update_shift_e7 () ;
			mRis >> LLTextReader<>::SKIP_GAP_SPACE_ONLY ;
			update_shift_e9 () ;
		}

		//@info: $1->记号串
		inline void update_shift_e1 () {
			mRis >> LLTextReader<>::HINT_IDENTIFY_TEXT >> mLatestString ;
		}

		//@info: $2->"文本串"
		inline void update_shift_e2 () {
			mRis >> LLTextReader<>::HINT_STRING_TEXT >> mLatestString ;
		}

		//@info: $3->单字符串
		inline void update_shift_e3 () {
			mRis >> LLTextReader<>::HINT_GAP_TEXT >> mLatestString ;
		}

		//@info: $4->/$1
		inline void update_shift_e4 () {
			mRis >> _PCSTRU8_ ("/") ;
			update_shift_e1 () ;
			mOptionSet.add (std::move (mLatestString)) ;
		}

		//@info: $5->-$1|-$1=$2|-$1=$3
		inline void update_shift_e5 () {
			mRis >> _PCSTRU8_ ("-") ;
			update_shift_e1 () ;
			INDEX ix = mAttributeSet.find (mLatestString) ;
			_DYNAMIC_ASSERT_ (ix == VAR_NONE) ;
			ix = mAttributeSet.insert (std::move (mLatestString)) ;
			if (mRis[0] == STRU8 ('=') && mRis[1] == STRU8 ('\"')) {
				mRis >> _PCSTRU8_ ("=") ;
				update_shift_e2 () ;
				mAttributeSet[ix].item = std::move (mLatestString) ;
			} else if (mRis[0] == STRU8 ('=')) {
				mRis >> _PCSTRU8_ ("=") ;
				update_shift_e3 () ;
				mAttributeSet[ix].item = std::move (mLatestString) ;
			} else {
				mAttributeSet[ix].item = _PCSTRU8_ ("TRUE") ;
			}
		}

		//@info: $6->$2|$3
		inline void update_shift_e6 () {
			if (mRis[0] == STRU8 ('\"')) {
				update_shift_e2 () ;
			} else {
				update_shift_e3 () ;
			}
			mCommandList.add (std::move (mLatestString)) ;
		}

		//@info: $7->ε|$4 $7|$5 $7|$6 $7
		inline void update_shift_e7 () {
			while (mRis[0] != STRU8 ('\0')) {
				if (mRis[0] == STRU8 ('/')) {
					update_shift_e4 () ;
				} else if (mRis[0] == STRU8 ('-')) {
					update_shift_e5 () ;
				} else {
					update_shift_e6 () ;
				}
				mRis >> LLTextReader<>::SKIP_GAP_SPACE_ONLY ;
			}
		}

		//@info: $8->#
		inline void update_shift_e8 () {
			_STATIC_WARNING_ ("noop") ;
		}

		//@info: $9->#
		inline void update_shift_e9 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_command () {
			mCommand = Array<String<STRU8>> (mCommandList.length ()) ;
			for (auto &&i : mCommand)
				mCommandList.take (i) ;
		}

		inline void refresh () {
			mContext.mOptionSet = std::move (mOptionSet) ;
			mContext.mAttributeSet = std::move (mAttributeSet) ;
			mContext.mCommand = std::move (mCommand) ;
		}
	} ;
	_CALL_ (Lambda (*this ,data)) ;
}
} ;