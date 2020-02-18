#pragma once

#ifndef __CSC_SERIALIZATION__
#define __CSC_SERIALIZATION__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
class XmlParser {
private:
	static constexpr auto NODE_CLAZZ_TABLE = FLAG (0) ;
	static constexpr auto NODE_CLAZZ_OBJECT = FLAG (1) ;
	static constexpr auto NODE_CLAZZ_ARRAY = FLAG (2) ;
	static constexpr auto NODE_CLAZZ_FINAL = FLAG (3) ;

	class Node {
	private:
		friend XmlParser ;
		String<STRU8> mName ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSet ;
		SoftSet<INDEX ,INDEX> mMemberSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;

	public:
		inline Node () = default ;
	} ;

private:
	SharedRef<FixedBuffer<Node>> mHeap ;
	INDEX mIndex ;

public:
	XmlParser () {
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
		INDEX ix = mHeap.self[mIndex].mObjectSet.find (name) ;
		if (ix == VAR_NONE)
			return XmlParser (mHeap ,VAR_NONE) ;
		INDEX jx = mHeap.self[mIndex].mObjectSet[ix].item ;
		return XmlParser (mHeap ,jx) ;
	}

	Array<XmlParser> child_array () const {
		Array<XmlParser> ret ;
		if switch_case (TRUE) {
			if (!exist ())
				discard ;
			ret = Array<XmlParser> (mHeap.self[mIndex].mMemberSet.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet)
				ret[iw++] = XmlParser (mHeap ,i.item) ;
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
		}
		return std::move (ret) ;
	}

	Array<XmlParser> child_array (LENGTH fixed_len) const {
		Array<XmlParser> ret = Array<XmlParser> (fixed_len) ;
		INDEX iw = 0 ;
		if switch_case (TRUE) {
			if (!exist ())
				discard ;
			for (auto &&i : mHeap.self[mIndex].mMemberSet) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = XmlParser (mHeap ,i.item) ;
			}
		}
		return std::move (ret) ;
	}

	BOOL equal (const XmlParser &that) const {
		if (!exist ())
			if (!that.exist ())
				return TRUE ;
		if (!exist ())
			return FALSE ;
		if (!that.exist ())
			return FALSE ;
		if (&mHeap.self != &that.mHeap.self)
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

	const String<STRU8> &name () const & {
		_DYNAMIC_ASSERT_ (exist ()) ;
		return mHeap.self[mIndex].mName ;
	}

	const String<STRU8> &name () && = delete ;

	const String<STRU8> &attribute (const String<STRU8> &tag) const & {
		auto &r1y = _CACHE_ ([] () {
			return String<STRU8> () ;
		}) ;
		if (!exist ())
			return r1y ;
		INDEX ix = mHeap.self[mIndex].mAttributeSet.find (tag) ;
		if (ix == VAR_NONE)
			return r1y ;
		return mHeap.self[mIndex].mAttributeSet[ix].item ;
	}

	const String<STRU8> &attribute (const String<STRU8> &) && = delete ;

	template <class _ARG1>
	_ARG1 attribute (const String<STRU8> &tag ,const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		_ARG1 ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (attribute (tag)) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL attribute (const String<STRU8> &tag ,const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	PTR<const VOID> attribute (const String<STRU8> & ,const PTR<const VOID> &) const = delete ;

	VAR32 attribute (const String<STRU8> &tag ,const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAR64 attribute (const String<STRU8> &tag ,const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL32 attribute (const String<STRU8> &tag ,const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL64 attribute (const String<STRU8> &tag ,const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU8> attribute (const String<STRU8> &tag ,const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU16> attribute (const String<STRU8> &tag ,const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU32> attribute (const String<STRU8> &tag ,const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRA> attribute (const String<STRU8> &tag ,const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRW> attribute (const String<STRU8> &tag ,const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_) ;
		return attribute (tag ,def ,r1x) ;
	}

	const String<STRU8> &value () const & {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mMemberSet.size () == 0) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mAttributeSet.length () == 1) ;
		INDEX ix = mHeap.self[mIndex].mAttributeSet.head () ;
		return mHeap.self[mIndex].mAttributeSet[ix].item ;
	}

	const String<STRU8> &value () && = delete ;

	template <class _ARG1>
	_ARG1 value (const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		_ARG1 ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (value ()) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL value (const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>) ;
		return value (def ,r1x) ;
	}

	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAR64 value (const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL32 value (const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL64 value (const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>) ;
		return value (def ,r1x) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_) ;
		return value (def ,r1x) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_) ;
		return value (def ,r1x) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_) ;
		return value (def ,r1x) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_) ;
		return value (def ,r1x) ;
	}

	void friend_write (TextWriter<STRU8> &writer) const ;

private:
	explicit XmlParser (const SharedRef<FixedBuffer<Node>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

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

inline void XmlParser::friend_write (TextWriter<STRU8> &writer) const {
	auto rax = Deque<ARRAY2<INDEX>> () ;
	auto rbx = Deque<ARRAY2<INDEX>> () ;
	rax.add (ARRAY2<INDEX> {mIndex ,FLAG (0)}) ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		const auto r1x = rax[rax.tail ()] ;
		rax.pop () ;
		auto fax = TRUE ;
		if switch_case (fax) {
			//@info: case '<?xml ...>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!mHeap.self[r1x[0]].mName.empty ())
				discard ;
			if (!(r1x[1] == 0))
				discard ;
			auto &r2y = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<?xml version=\"1.0\" encoding=\"utf-8\" ?>") ;
			writer << _GAP_ ;
			rbx.clear () ;
			for (INDEX i = r2y.mChild ,it ; i != VAR_NONE ; i = it) {
				it = mHeap.self[i].mBrother ;
				rbx.add (ARRAY2<INDEX> {i ,FLAG (0)}) ;
			}
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_case (fax) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?/>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x[0]].mChild == VAR_NONE))
				discard ;
			if (!(r1x[1] == 0))
				discard ;
			auto &r3y = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<") << r3y.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r3y.mAttributeSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << i.item << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ ("/>") ;
		}
		if switch_case (fax) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x[0]].mChild != VAR_NONE))
				discard ;
			if (!(r1x[1] == 0))
				discard ;
			auto &r4y = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<") << r4y.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r4y.mAttributeSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << i.item << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ (">") ;
			rbx.clear () ;
			for (INDEX i = r4y.mChild ,it ; i != VAR_NONE ; i = it) {
				it = mHeap.self[i].mBrother ;
				rbx.add (ARRAY2<INDEX> {i ,FLAG (0)}) ;
			}
			rbx.add (ARRAY2<INDEX> {r1x[0] ,FLAG (1)}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_case (fax) {
			//@info: case '</xxx>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(r1x[1] == 1))
				discard ;
			writer << _PCSTRU8_ ("</") << mHeap.self[r1x[0]].mName << _PCSTRU8_ (">") ;
		}
	}
}

inline void XmlParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		class RecursiveCounter :private Wrapped<LENGTH> {
		public:
			inline void lock () {
				using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
				_DYNAMIC_ASSERT_ (RecursiveCounter::mSelf < DEFAULT_RECURSIVE_SIZE::value) ;
				RecursiveCounter::mSelf++ ;
			}

			inline void unlock () {
				RecursiveCounter::mSelf-- ;
			}
		} ;

	private:
		XmlParser &mContext ;

		TextReader<STRU8> mTextReader ;
		RegularReader<ARGC<2>> mRis ;
		LENGTH mRecursiveCounter ;
		INDEX mLatestIndex ;
		String<STRU8> mLatestString ;

		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<INDEX ,INDEX> mMemberSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Node ,SAUTO> mNodeHeap ;
		SharedRef<FixedBuffer<Node>> mHeap ;
		INDEX mRoot ;

	public:
		inline explicit Lambda (XmlParser &context_ ,const PhanBuffer<const STRU8> &data) popping : mContext (context_) ,mTextReader (data) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = RegularReader<ARGC<2>> (PhanRef<TextReader<STRU8>>::make (mTextReader)) ;
			mRecursiveCounter = 0 ;
			mAttributeSoftSet = SoftSet<String<STRU8> ,String<STRU8>> (0) ;
			mMemberSoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Node ,SAUTO> () ;
			mHeap = SharedRef<FixedBuffer<Node>> () ;
			mRoot = VAR_NONE ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
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
		inline void update_shift_e0 () {
			update_shift_e8 () ;
			mRis >> RegularReader<>::SKIP_GAP ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mMemberSet = mMemberSoftSet.share () ;
			mNodeHeap[ix].mObjectSet = mObjectSoftSet.share () ;
			mNodeHeap[ix].mParent = VAR_NONE ;
			update_shift_e7 (ix) ;
			mNodeHeap[ix].mChild = mLatestIndex ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e9 () ;
			mLatestIndex = ix ;
		}

		//@info: $1->${identity}
		inline void update_shift_e1 () {
			mRis >> RegularReader<>::HINT_IDENTIFIER_TEXT >> mLatestString ;
		}

		//@info: $2->"${string}"
		inline void update_shift_e2 () {
			mRis >> RegularReader<>::HINT_STRING_TEXT >> mLatestString ;
		}

		//@info: $3->$1 = $2
		inline void update_shift_e3 (INDEX curr) {
			update_shift_e1 () ;
			INDEX ix = mNodeHeap[curr].mAttributeSet.insert (std::move (mLatestString)) ;
			_DYNAMIC_ASSERT_ (mNodeHeap[curr].mAttributeSet[ix].item.empty ()) ;
			mRis >> RegularReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("=") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e2 () ;
			mNodeHeap[curr].mAttributeSet[ix].item = std::move (mLatestString) ;
		}

		//@info: $4->${eps}|$3 $4
		inline void update_shift_e4 (INDEX curr) {
			while (TRUE) {
				if (!(mRis[0] >= STRU8 ('A') && mRis[0] <= STRU8 ('Z')))
					if (!(mRis[0] >= STRU8 ('a') && mRis[0] <= STRU8 ('z')))
						if (!(mRis[0] == STRU8 ('_')))
							break ;
				update_shift_e3 (curr) ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
		}

		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		inline void update_shift_e5 (INDEX curr) {
			ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_<RecursiveCounter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("<") ;
			INDEX ix = mNodeHeap.alloc () ;
			update_shift_e1 () ;
			mNodeHeap[ix].mName = std::move (mLatestString) ;
			mNodeHeap[ix].mAttributeSet = mAttributeSoftSet.share () ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e4 (ix) ;
			mRis >> RegularReader<>::SKIP_GAP ;
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('>')))
					discard ;
				mRis++ ;
				mRis >> RegularReader<>::SKIP_GAP ;
				mNodeHeap[ix].mMemberSet = mMemberSoftSet.share () ;
				mNodeHeap[ix].mObjectSet = mObjectSoftSet.share () ;
				update_shift_e7 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> RegularReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ ("</") ;
				update_shift_e1 () ;
				_DYNAMIC_ASSERT_ (mNodeHeap[ix].mName == mLatestString) ;
				mRis >> RegularReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ (">") ;
			}
			if switch_case (fax) {
				mRis >> _PCSTRU8_ ("/>") ;
			}
			mLatestIndex = ix ;
		}

		//@info: $6-><!--${comment}-->
		inline void update_shift_e6 () {
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
		inline void update_shift_e7 (INDEX curr) {
			ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_<RecursiveCounter> (mRecursiveCounter)) ;
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				const auto r1x = BOOL (mRis[0] == STRU8 ('<') && mRis[1] == STRU8 ('!')) ;
				const auto r2x = BOOL (mRis[0] == STRU8 ('<') && mRis[1] != STRU8 ('/')) ;
				if (!r1x && !r2x)
					break ;
				auto fax = TRUE ;
				if switch_case (fax) {
					if (!r1x)
						discard ;
					update_shift_e6 () ;
				}
				if switch_case (fax) {
					if (!r2x)
						discard ;
					update_shift_e5 (curr) ;
					mNodeHeap[curr].mMemberSet.add (mNodeHeap[curr].mMemberSet.length () ,mLatestIndex) ;
					mNodeHeap[curr].mObjectSet.add (mNodeHeap[mLatestIndex].mName ,mLatestIndex) ;
					auto &r3y = _SWITCH_ (
						(ix == VAR_NONE) ? ix :
						mNodeHeap[iy].mBrother) ;
					r3y = mLatestIndex ;
					iy = mLatestIndex ;
				}
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mLatestIndex = ix ;
		}

		//@info: $8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		inline void update_shift_e8 () {
			if (mRis[0] != STRU8 ('<'))
				return ;
			if (mRis[1] != STRU8 ('?'))
				return ;
			mRis++ ;
			mRis++ ;
			mRis >> _PCSTRU8_ ("xml") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("version") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("=") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ ("\"1.0\"") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			if switch_case (TRUE) {
				if (mRis[0] == STRU8 ('?'))
					discard ;
				mRis >> _PCSTRU8_ ("encoding") ;
				mRis >> RegularReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ ("=") ;
				mRis >> RegularReader<>::SKIP_GAP ;
				mRis >> _PCSTRU8_ ("\"utf-8\"") ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("?>") ;
		}

		//@info: $9->${end}
		inline void update_shift_e9 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_heap () {
			mAttributeSoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : _RANGE_ (0 ,mNodeHeap.size ())) {
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
	_CALL_ (Lambda ((*this) ,data)) ;
}

inline void XmlParser::initialize (const Array<XmlParser> &sequence) {
	class Lambda {
	private:
		struct FOUND_NODE {
			String<STRU8> mName ;
			SoftSet<String<STRU8> ,String<STRU8>> mAttributeSet ;
			FLAG mType ;
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

		Deque<PACK<Deque<XmlParser> ,FLAG ,INDEX>> mNodeStack ;
		ARRAY4<Function<DEF<void (const XmlParser &)> NONE::*>> mFoundNodeProc ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<INDEX ,INDEX> mMemberSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Node ,SAUTO> mNodeHeap ;
		String<STRU8> mRootName ;
		INDEX mRootType ;
		Deque<FOUND_NODE> mFoundNode ;
		Set<String<STRU8> ,INDEX> mFoundNodeNameSet ;
		SharedRef<FixedBuffer<Node>> mHeap ;
		INDEX mRoot ;

		Deque<Deque<XmlParser>> mFoundNodeBaseNodeHeap ;
		PACK<Deque<XmlParser> ,FLAG ,INDEX> mTempNode ;

	public:
		inline explicit Lambda (XmlParser &context_ ,const Array<XmlParser> &sequence) popping : mContext (context_) ,mSequence (sequence) ,mClazzString (_PCSTRU8_ ("type")) ,mTableClazzString (_PCSTRU8_ ("table")) ,mObjectClazzString (_PCSTRU8_ ("object")) ,mArrayClazzString (_PCSTRU8_ ("array")) ,mFinalClazzString (_PCSTRU8_ ("final")) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mNodeStack = Deque<PACK<Deque<XmlParser> ,FLAG ,INDEX>> () ;
			mFoundNodeProc[0] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_table_node) ;
			mFoundNodeProc[1] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_object_node) ;
			mFoundNodeProc[2] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_array_node) ;
			mFoundNodeProc[3] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_table_node) ;
			mAttributeSoftSet = SoftSet<String<STRU8> ,String<STRU8>> (0) ;
			mMemberSoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Node ,SAUTO> () ;
			if switch_case (TRUE) {
				mRootName.clear () ;
				mRootType = VAR_NONE ;
				INDEX ix = find_normal_node () ;
				if (ix == VAR_NONE)
					discard ;
				mRootName = mSequence[ix].name () ;
				mRootType = node_type (mSequence[ix]) ;
			}
			for (auto &&i : mSequence) {
				if (!i.exist ())
					continue ;
				_DYNAMIC_ASSERT_ (i.name () == mRootName) ;
				_DYNAMIC_ASSERT_ (node_type (i) == mRootType) ;
			}
			mRoot = mNodeHeap.alloc () ;
			mNodeHeap[mRoot].mMemberSet = mMemberSoftSet.share () ;
			mNodeHeap[mRoot].mObjectSet = mObjectSoftSet.share () ;
			mNodeHeap[mRoot].mParent = VAR_NONE ;
			mNodeHeap[mRoot].mChild = VAR_NONE ;
			mNodeHeap[mRoot].mBrother = VAR_NONE ;
			mFoundNode = Deque<FOUND_NODE> () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make () ;
		}

		inline INDEX find_normal_node () const {
			for (auto &&i : mSequence)
				if (i.exist ())
					return mSequence.at (i) ;
			return VAR_NONE ;
		}

		inline INDEX node_type (const XmlParser &node) const {
			auto &r1y = node.attribute (mClazzString) ;
			if (r1y == mTableClazzString)
				return NODE_CLAZZ_TABLE ;
			if (r1y == mObjectClazzString)
				return NODE_CLAZZ_OBJECT ;
			if (r1y == mArrayClazzString)
				return NODE_CLAZZ_ARRAY ;
			if (r1y == mFinalClazzString)
				return NODE_CLAZZ_FINAL ;
			_DYNAMIC_ASSERT_ (r1y.empty ()) ;
			return NODE_CLAZZ_TABLE ;
		}

		inline void generate () {
			update_root_node () ;
			while (TRUE) {
				if (mNodeStack.empty ())
					break ;
				mTempNode = std::move (mNodeStack[mNodeStack.tail ()]) ;
				mNodeStack.pop () ;
				if switch_case (TRUE) {
					if (mTempNode.P2 == VAR_NONE)
						discard ;
					for (auto &&i : mTempNode.P1)
						mFoundNodeProc[mTempNode.P2] (i) ;
					update_merge_found_node (mTempNode.P3) ;
					mFoundNodeBaseNodeHeap.add (std::move (mTempNode.P1)) ;
				}
			}
			update_heap () ;
		}

		inline void update_root_node () {
			INDEX ix = mNodeStack.insert () ;
			mNodeStack[ix].P1 = Deque<XmlParser> (mSequence.length ()) ;
			for (auto &&i : mSequence)
				mNodeStack[ix].P1.add (i.child ()) ;
			mNodeStack[ix].P2 = mRootType ;
			INDEX iy = mRoot ;
			if switch_case (TRUE) {
				if (mRootName.empty ())
					discard ;
				iy = mNodeHeap.alloc () ;
				mNodeHeap[iy].mName = std::move (mRootName) ;
				mNodeHeap[iy].mAttributeSet = mAttributeSoftSet.share () ;
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
			mNodeStack[ix].P3 = iy ;
		}

		inline void update_found_table_node (const XmlParser &node) {
			for (XmlParser i = node ,it ; i.exist () ; i = it) {
				it = i.brother () ;
				INDEX ix = find_found_node_name (i.name ()) ;
				INDEX iy = mFoundNode.insert () ;
				mFoundNode[iy].mName = i.name () ;
				mFoundNodeNameSet.add (mFoundNode[iy].mName ,iy) ;
				mFoundNode[iy].mType = node_type (i) ;
				if switch_case (TRUE) {
					if (ix == VAR_NONE)
						discard ;
					_DYNAMIC_ASSERT_ (mFoundNode[ix].mType != NODE_CLAZZ_FINAL) ;
					_DYNAMIC_ASSERT_ (mFoundNode[iy].mType != NODE_CLAZZ_FINAL) ;
				}
				if (mFoundNode[iy].mType == NODE_CLAZZ_FINAL)
					_DYNAMIC_ASSERT_ (i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				mFoundNode[iy].mAttributeSet = mAttributeSoftSet.share () ;
				mFoundNode[iy].mAttributeSet.appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				if (mFoundNodeBaseNodeHeap.empty ())
					mFoundNodeBaseNodeHeap.add (Deque<XmlParser> ()) ;
				mFoundNodeBaseNodeHeap.take (mFoundNode[iy].mBaseNode) ;
				mFoundNode[iy].mBaseNode.clear () ;
				mFoundNode[iy].mBaseNode.add (i.child ()) ;
			}
		}

		inline void update_found_object_node (const XmlParser &node) {
			for (XmlParser i = node ,it ; i.exist () ; i = it) {
				it = i.brother () ;
				INDEX ix = find_found_node_name (i.name ()) ;
				INDEX iy = ix ;
				if switch_case (TRUE) {
					if (iy != VAR_NONE)
						discard ;
					iy = mFoundNode.insert () ;
					mFoundNode[iy].mName = i.name () ;
					mFoundNodeNameSet.add (mFoundNode[iy].mName ,iy) ;
					mFoundNode[iy].mType = node_type (i) ;
					mFoundNode[iy].mAttributeSet = mAttributeSoftSet.share () ;
					if (mFoundNodeBaseNodeHeap.empty ())
						mFoundNodeBaseNodeHeap.add (Deque<XmlParser> ()) ;
					mFoundNodeBaseNodeHeap.take (mFoundNode[iy].mBaseNode) ;
					mFoundNode[iy].mBaseNode.clear () ;
				}
				if switch_case (TRUE) {
					if (ix == VAR_NONE)
						discard ;
					_DYNAMIC_ASSERT_ (ix == iy) ;
					_DYNAMIC_ASSERT_ (mFoundNode[ix].mType != NODE_CLAZZ_FINAL) ;
					_DYNAMIC_ASSERT_ (mFoundNode[iy].mType != NODE_CLAZZ_FINAL) ;
				}
				if (mFoundNode[iy].mType == NODE_CLAZZ_FINAL)
					_DYNAMIC_ASSERT_ (i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				mFoundNode[iy].mAttributeSet.appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				mFoundNode[iy].mBaseNode.add (i.child ()) ;
			}
		}

		inline void update_found_array_node (const XmlParser &node) {
			for (XmlParser i = node ,it ; i.exist () ; i = it) {
				it = i.brother () ;
				INDEX ix = mFoundNodeNameSet.head () ;
				if (ix != VAR_NONE)
					ix = mFoundNodeNameSet[ix].item ;
				INDEX iy = mFoundNode.insert () ;
				mFoundNode[iy].mName = i.name () ;
				mFoundNodeNameSet.add (mFoundNode[iy].mName ,iy) ;
				mFoundNode[iy].mType = node_type (i) ;
				if switch_case (TRUE) {
					if (ix == VAR_NONE)
						discard ;
					_DYNAMIC_ASSERT_ (mFoundNode[ix].mName == mFoundNode[iy].mName) ;
					_DYNAMIC_ASSERT_ (mFoundNode[ix].mType != NODE_CLAZZ_FINAL) ;
					_DYNAMIC_ASSERT_ (mFoundNode[iy].mType != NODE_CLAZZ_FINAL) ;
				}
				if (mFoundNode[iy].mType == NODE_CLAZZ_FINAL)
					_DYNAMIC_ASSERT_ (i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				mFoundNode[iy].mAttributeSet = mAttributeSoftSet.share () ;
				mFoundNode[iy].mAttributeSet.appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				if (mFoundNodeBaseNodeHeap.empty ())
					mFoundNodeBaseNodeHeap.add (Deque<XmlParser> ()) ;
				mFoundNodeBaseNodeHeap.take (mFoundNode[iy].mBaseNode) ;
				mFoundNode[iy].mBaseNode.clear () ;
				mFoundNode[iy].mBaseNode.add (i.child ()) ;
			}
		}

		inline INDEX find_found_node_name (const String<STRU8> &name) const {
			INDEX ix = mFoundNodeNameSet.find (name) ;
			if (ix == VAR_NONE)
				return VAR_NONE ;
			return mFoundNodeNameSet[ix].item ;
		}

		inline void update_merge_found_node (INDEX curr) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			for (auto &&i : mFoundNode) {
				iy = ix ;
				ix = mNodeHeap.alloc () ;
				mNodeHeap[ix].mName = std::move (i.mName) ;
				mNodeHeap[ix].mAttributeSet = std::move (i.mAttributeSet) ;
				mNodeHeap[ix].mParent = curr ;
				if switch_case (TRUE) {
					INDEX jx = mNodeHeap[ix].mParent ;
					if (jx == VAR_NONE)
						discard ;
					if (mNodeHeap[jx].mChild == VAR_NONE)
						mNodeHeap[jx].mChild = ix ;
					if switch_case (TRUE) {
						if (mNodeHeap[jx].mMemberSet.size () > 0)
							discard ;
						mNodeHeap[jx].mMemberSet = mMemberSoftSet.share () ;
						mNodeHeap[jx].mObjectSet = mObjectSoftSet.share () ;
					}
					mNodeHeap[jx].mMemberSet.add (mNodeHeap[jx].mMemberSet.length () ,ix) ;
					mNodeHeap[jx].mObjectSet.add (mNodeHeap[ix].mName ,ix) ;
				}
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
				if (iy != VAR_NONE)
					mNodeHeap[iy].mBrother = ix ;
				if (mRoot == VAR_NONE)
					mRoot = ix ;
				INDEX jy = mNodeStack.insert () ;
				mNodeStack[jy].P1 = std::move (i.mBaseNode) ;
				mNodeStack[jy].P2 = i.mType ;
				mNodeStack[jy].P3 = ix ;
			}
			mFoundNodeNameSet.clear () ;
			mFoundNode.clear () ;
		}

		inline void update_heap () {
			mAttributeSoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : _RANGE_ (0 ,mNodeHeap.size ())) {
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
	_CALL_ (Lambda ((*this) ,sequence)) ;
}

class JsonParser {
private:
	static constexpr auto NODE_CLAZZ_NULL = FLAG (1) ;
	static constexpr auto NODE_CLAZZ_STRING = FLAG (2) ;
	static constexpr auto NODE_CLAZZ_ARRAY = FLAG (3) ;
	static constexpr auto NODE_CLAZZ_OBJECT = FLAG (4) ;

	class Node {
	private:
		friend JsonParser ;
		AnyRef<void> mValue ;
		FLAG mClazz ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;

	public:
		inline Node () = default ;
	} ;

private:
	SharedRef<FixedBuffer<Node>> mHeap ;
	INDEX mIndex ;

public:
	JsonParser () {
		mIndex = VAR_NONE ;
	}

	BOOL exist () const {
		if (!mHeap.exist ())
			return FALSE ;
		if (mIndex == VAR_NONE)
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
		auto &r1y = mHeap.self[mIndex].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
		INDEX ix = r1y.find (key) ;
		if (ix == VAR_NONE)
			return JsonParser (mHeap ,VAR_NONE) ;
		return JsonParser (mHeap ,r1y[ix].item) ;
	}

	Array<JsonParser> child_array () const {
		Array<JsonParser> ret ;
		if switch_case (TRUE) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1y = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			ret = Array<JsonParser> (r1y.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : r1y)
				ret[iw++] = JsonParser (mHeap ,i.item) ;
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
		}
		return std::move (ret) ;
	}

	Array<JsonParser> child_array (LENGTH fixed_len) const {
		Array<JsonParser> ret = Array<JsonParser> (fixed_len) ;
		INDEX iw = 0 ;
		if switch_case (TRUE) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1y = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			for (auto &&i : r1y) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = JsonParser (mHeap ,i.item) ;
			}
		}
		return std::move (ret) ;
	}

	BOOL equal (const JsonParser &that) const {
		if (!exist ())
			if (!that.exist ())
				return TRUE ;
		if (!exist ())
			return FALSE ;
		if (!that.exist ())
			return FALSE ;
		if (&mHeap.self != &that.mHeap.self)
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

	const String<STRU8> &value () const & {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (string_type ()) ;
		return mHeap.self[mIndex].mValue.rebind<String<STRU8>> () ;
	}

	const String<STRU8> &value () && = delete ;

	template <class _ARG1>
	_ARG1 value (const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		_ARG1 ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (value ()) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL value (const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>) ;
		return value (def ,r1x) ;
	}

	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAR64 value (const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL32 value (const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	VAL64 value (const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>) ;
		return value (def ,r1x) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>) ;
		return value (def ,r1x) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_) ;
		return value (def ,r1x) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_) ;
		return value (def ,r1x) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_) ;
		return value (def ,r1x) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_) ;
		return value (def ,r1x) ;
	}

	void friend_write (TextWriter<STRU8> &writer) const ;

private:
	explicit JsonParser (const SharedRef<FixedBuffer<Node>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;

	Set<PTR<const String<STRU8>>> object_key_adress_set () const {
		Set<PTR<const String<STRU8>>> ret = Set<PTR<const String<STRU8>>> (mHeap->size ()) ;
		for (auto &&i : _RANGE_ (0 ,mHeap->size ())) {
			if (mHeap.self[i].mClazz != NODE_CLAZZ_OBJECT)
				continue ;
			auto &r1y = mHeap.self[i].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			for (auto &&j : r1y)
				ret.add (&j.key) ;
		}
		return std::move (ret) ;
	}

public:
	inline static JsonParser make (const PhanBuffer<const STRU8> &data) {
		JsonParser ret ;
		ret.initialize (data) ;
		return std::move (ret) ;
	}
} ;

inline void JsonParser::friend_write (TextWriter<STRU8> &writer) const {
	auto rax = Deque<ARRAY2<INDEX>> () ;
	auto rbx = Deque<ARRAY2<INDEX>> () ;
	rax.add (ARRAY2<INDEX> {mIndex ,FLAG (0)}) ;
	const auto r1x = object_key_adress_set () ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		const auto r2x = rax[rax.tail ()] ;
		rax.pop () ;
		auto fax = TRUE ;
		if switch_case (fax) {
			//@info: case 'null'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_NULL))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			writer << _PCSTRU8_ ("null") ;
		}
		if switch_case (fax) {
			//@info: case '"xxx"'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_STRING))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			auto &r3y = mHeap.self[r2x[0]].mValue.rebind<String<STRU8>> ().self ;
			writer << _PCSTRU8_ ("\"") ;
			writer << r3y ;
			writer << _PCSTRU8_ ("\"") ;
		}
		if switch_case (fax) {
			//@info: case '[(yyy(,yyy)*)?]'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_ARRAY))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			auto &r4y = mHeap.self[r2x[0]].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			rbx.clear () ;
			rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (2)}) ;
			INDEX ir = 0 ;
			for (auto &&i : r4y) {
				if (ir > 0)
					rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (3)}) ;
				ir++ ;
				rbx.add (ARRAY2<INDEX> {i.item ,FLAG (0)}) ;
			}
			rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (4)}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_case (fax) {
			//@info: case '{("xxx":yyy(,"xxx":yyy)*)?}'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_OBJECT))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			auto &r5y = mHeap.self[r2x[0]].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			rbx.clear () ;
			rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (5)}) ;
			INDEX ir = 0 ;
			for (auto &&i : r5y) {
				if (ir > 0)
					rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (6)}) ;
				ir++ ;
				INDEX ix = r1x.find (&i.key) ;
				_DEBUG_ASSERT_ (ix != VAR_NONE) ;
				rbx.add (ARRAY2<INDEX> {ix ,FLAG (1)}) ;
				rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (7)}) ;
				rbx.add (ARRAY2<INDEX> {i.item ,FLAG (0)}) ;
			}
			rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (8)}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if switch_case (fax) {
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(r2x[1] == 1))
				discard ;
			writer << _PCSTRU8_ ("\"") ;
			auto &r6y = (*r1x[r2x[0]].key) ;
			writer << r6y ;
			writer << _PCSTRU8_ ("\"") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 2))
				discard ;
			writer << _PCSTRU8_ ("[") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 3))
				discard ;
			writer << _PCSTRU8_ (",") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 4))
				discard ;
			writer << _PCSTRU8_ ("]") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 5))
				discard ;
			writer << _PCSTRU8_ ("{") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 6))
				discard ;
			writer << _PCSTRU8_ (",") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 7))
				discard ;
			writer << _PCSTRU8_ (":") ;
		}
		if switch_case (fax) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 8))
				discard ;
			writer << _PCSTRU8_ ("}") ;
		}
	}
}

inline void JsonParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		class RecursiveCounter :private Wrapped<LENGTH> {
		public:
			inline void lock () {
				using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
				_DYNAMIC_ASSERT_ (RecursiveCounter::mSelf < DEFAULT_RECURSIVE_SIZE::value) ;
				RecursiveCounter::mSelf++ ;
			}

			inline void unlock () {
				RecursiveCounter::mSelf-- ;
			}
		} ;

	private:
		JsonParser &mContext ;

		TextReader<STRU8> mTextReader ;
		RegularReader<ARGC<2>> mRis ;
		LENGTH mRecursiveCounter ;
		INDEX mLatestIndex ;
		String<STRU8> mLatestString ;

		SoftSet<INDEX ,INDEX> mArraySoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Node ,SAUTO> mNodeHeap ;
		SharedRef<FixedBuffer<Node>> mHeap ;
		INDEX mRoot ;

	public:
		inline explicit Lambda (JsonParser &context_ ,const PhanBuffer<const STRU8> &data) popping : mContext (context_) ,mTextReader (data) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = RegularReader<ARGC<2>> (PhanRef<TextReader<STRU8>>::make (mTextReader)) ;
			mRecursiveCounter = 0 ;
			mArraySoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Node ,SAUTO> () ;
			mHeap = SharedRef<FixedBuffer<Node>> () ;
			mRoot = VAR_NONE ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
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
		inline void update_shift_e0 () {
			update_shift_e11 () ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e10 () ;
			INDEX ix = mLatestIndex ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e12 () ;
			mLatestIndex = ix ;
		}

		//@info: $1->${value}
		inline void update_shift_e1 () {
			mRis >> RegularReader<>::HINT_VALUE_TEXT >> mLatestString ;
		}

		//@info: $2->true|TRUE|false|FALSE
		inline void update_shift_e2 () {
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('t')))
					discard ;
				mRis >> _PCSTRU8_ ("true") ;
				mLatestString = _PCSTRU8_ ("true") ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('T')))
					discard ;
				mRis >> _PCSTRU8_ ("TRUE") ;
				mLatestString = _PCSTRU8_ ("TRUE") ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('f')))
					discard ;
				mRis >> _PCSTRU8_ ("false") ;
				mLatestString = _PCSTRU8_ ("false") ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('F')))
					discard ;
				mRis >> _PCSTRU8_ ("FALSE") ;
				mLatestString = _PCSTRU8_ ("FALSE") ;
			}
			if switch_case (fax) {
				_STATIC_WARNING_ ("unexpected") ;
				_DYNAMIC_ASSERT_ (FALSE) ;
			}
		}

		//@info: $2x->null
		inline void update_shift_e2x () {
			mRis >> _PCSTRU8_ ("null") ;
			mLatestString = String<STRU8> () ;
		}

		//@info: $3->"${string}"
		inline void update_shift_e3 () {
			mRis >> RegularReader<>::HINT_STRING_TEXT >> mLatestString ;
		}

		//@info: $4->$1|$2|$2x|$3|$6|$9
		inline void update_shift_e4 (INDEX curr) {
			ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_<RecursiveCounter> (mRecursiveCounter)) ;
			INDEX ix = VAR_NONE ;
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('+') || mRis[0] == STRU8 ('-')))
					if (!(mRis[0] >= STRU8 ('0') && mRis[0] <= STRU8 ('9')))
						discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e1 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('t') || mRis[0] == STRU8 ('T')))
					if (!(mRis[0] == STRU8 ('f') || mRis[0] == STRU8 ('F')))
						discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e2 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('n')))
					discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e2x () ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_NULL ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('\"')))
					discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e3 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('[')))
					discard ;
				update_shift_e6 (curr) ;
				ix = mLatestIndex ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('{')))
					discard ;
				update_shift_e9 (curr) ;
				ix = mLatestIndex ;
			}
			if switch_case (fax) {
				_STATIC_WARNING_ ("unexpected") ;
				_DYNAMIC_ASSERT_ (FALSE) ;
			}
			mLatestIndex = ix ;
		}

		//@info: $5->$4|$4 , $5
		inline void update_shift_e5 (INDEX curr) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e4 (curr) ;
				auto &r1y = mNodeHeap[curr].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
				r1y.add (r1y.length () ,mLatestIndex) ;
				auto &r2y = _SWITCH_ (
					(ix == VAR_NONE) ? ix :
					mNodeHeap[iy].mBrother) ;
				r2y = mLatestIndex ;
				iy = mLatestIndex ;
				mRis >> RegularReader<>::SKIP_GAP ;
				if (mRis[0] != STRU8 (','))
					break ;
				mRis++ ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mLatestIndex = ix ;
		}

		//@info: $6->[ ]|[ $5 ]
		inline void update_shift_e6 (INDEX curr) {
			ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_<RecursiveCounter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("[") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<INDEX ,INDEX>>::make (mArraySoftSet.share ()) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_ARRAY ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> RegularReader<>::SKIP_GAP ;
			if switch_case (TRUE) {
				if (mRis[0] == STRU8 (']'))
					discard ;
				update_shift_e5 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("]") ;
			mLatestIndex = ix ;
		}

		//@info: $7->$3 : $4
		inline void update_shift_e7 (INDEX curr) {
			update_shift_e3 () ;
			auto &r1y = mNodeHeap[curr].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			INDEX ix = r1y.insert (std::move (mLatestString)) ;
			mRis >> RegularReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ (":") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e4 (curr) ;
			r1y[ix].item = mLatestIndex ;
		}

		//@info: $8->$7|$7 , $8
		inline void update_shift_e8 (INDEX curr) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e7 (curr) ;
				auto &r1y = _SWITCH_ (
					(ix == VAR_NONE) ? ix :
					mNodeHeap[iy].mBrother) ;
				r1y = mLatestIndex ;
				iy = mLatestIndex ;
				mRis >> RegularReader<>::SKIP_GAP ;
				if (mRis[0] != STRU8 (','))
					break ;
				mRis++ ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mLatestIndex = ix ;
		}

		//@info: $9->{ }|{ $8 }
		inline void update_shift_e9 (INDEX curr) {
			ScopedGuard<RecursiveCounter> ANONYMOUS (_CAST_<RecursiveCounter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("{") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<String<STRU8> ,INDEX>>::make (mObjectSoftSet.share ()) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_OBJECT ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> RegularReader<>::SKIP_GAP ;
			if switch_case (TRUE) {
				if (mRis[0] == STRU8 ('}'))
					discard ;
				update_shift_e8 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("}") ;
			mLatestIndex = ix ;
		}

		//@info: $10->${eps}|$4
		inline void update_shift_e10 () {
			INDEX ix = VAR_NONE ;
			if switch_case (TRUE) {
				if (mRis[0] == STRU8 ('\0'))
					discard ;
				update_shift_e4 (VAR_NONE) ;
				ix = mLatestIndex ;
			}
			mLatestIndex = ix ;
		}

		//@info: $11->${eps}
		inline void update_shift_e11 () {
			_STATIC_WARNING_ ("noop") ;
		}

		//@info: $12->${end}
		inline void update_shift_e12 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_heap () {
			mArraySoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : _RANGE_ (0 ,mNodeHeap.size ())) {
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
	_CALL_ (Lambda ((*this) ,data)) ;
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

	explicit CommandParser (VAR32 argc ,PTR<const PTR<STRA>> argv) {
		const auto r1x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8>::make () ;
			auto wos = TextWriter<STRU8> (ret.raw ()) ;
			for (auto &&i : _RANGE_ (1 ,LENGTH (argc))) {
				wos << _ASTOU8S_ (PTRTOARR[argv[i]]) ;
				wos << _PCSTRU8_ (" ") ;
			}
			wos << _EOS_ ;
			return std::move (ret) ;
		}) ;
		initialize (r1x.raw ()) ;
	}

	BOOL option (const String<STRU8> &tag) const {
		INDEX ix = mOptionSet.find (tag) ;
		if (ix == VAR_NONE)
			return FALSE ;
		return TRUE ;
	}

	const String<STRU8> &attribute (const String<STRU8> &tag) const & {
		auto &r1y = _CACHE_ ([] () {
			return String<STRU8> () ;
		}) ;
		INDEX ix = mAttributeSet.find (tag) ;
		if (ix == VAR_NONE)
			return r1y ;
		return mAttributeSet[ix].item ;
	}

	const String<STRU8> &attribute (const String<STRU8> &) && = delete ;

	template <class _ARG1>
	_ARG1 attribute (const String<STRU8> &tag ,const _ARG1 &def ,const Function<_ARG1 (const String<STRU8> &)> &convert) const {
		_ARG1 ret ;
		_CALL_TRY_ ([&] () {
			ret = convert (attribute (tag)) ;
		} ,[&] () {
			ret = def ;
		}) ;
		return std::move (ret) ;
	}

	BOOL attribute (const String<STRU8> &tag ,const BOOL &def) const {
		const auto r1x = Function<BOOL (const String<STRU8> &)> (&_PARSEBOOLS_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	PTR<const VOID> attribute (const String<STRU8> & ,const PTR<const VOID> &) const = delete ;

	VAR32 attribute (const String<STRU8> &tag ,const VAR32 &def) const {
		const auto r1x = Function<VAR32 (const String<STRU8> &)> (&_PARSEVAR32S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAR64 attribute (const String<STRU8> &tag ,const VAR64 &def) const {
		const auto r1x = Function<VAR64 (const String<STRU8> &)> (&_PARSEVAR64S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL32 attribute (const String<STRU8> &tag ,const VAL32 &def) const {
		const auto r1x = Function<VAL32 (const String<STRU8> &)> (&_PARSEVAL32S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	VAL64 attribute (const String<STRU8> &tag ,const VAL64 &def) const {
		const auto r1x = Function<VAL64 (const String<STRU8> &)> (&_PARSEVAL64S_<STRU8>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU8> attribute (const String<STRU8> &tag ,const String<STRU8> &def) const {
		const auto r1x = Function<String<STRU8> (const String<STRU8> &)> (&_COPY_<String<STRU8>>) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU16> attribute (const String<STRU8> &tag ,const String<STRU16> &def) const {
		const auto r1x = Function<String<STRU16> (const String<STRU8> &)> (&_U8STOU16S_) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRU32> attribute (const String<STRU8> &tag ,const String<STRU32> &def) const {
		const auto r1x = Function<String<STRU32> (const String<STRU8> &)> (&_U8STOU32S_) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRA> attribute (const String<STRU8> &tag ,const String<STRA> &def) const {
		const auto r1x = Function<String<STRA> (const String<STRU8> &)> (&_U8STOAS_) ;
		return attribute (tag ,def ,r1x) ;
	}

	String<STRW> attribute (const String<STRU8> &tag ,const String<STRW> &def) const {
		const auto r1x = Function<String<STRW> (const String<STRU8> &)> (&_U8STOWS_) ;
		return attribute (tag ,def ,r1x) ;
	}

	const Array<String<STRU8>> &command () const & {
		return mCommand ;
	}

	const Array<String<STRU8>> &command () && = delete ;

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;
} ;

inline void CommandParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		CommandParser &mContext ;

		TextReader<STRU8> mTextReader ;
		RegularReader<ARGC<2>> mRis ;
		String<STRU8> mLatestString ;

		Set<String<STRU8>> mOptionSet ;
		Set<String<STRU8> ,String<STRU8>> mAttributeSet ;
		SList<String<STRU8>> mCommandList ;
		Array<String<STRU8>> mCommand ;

	public:
		inline explicit Lambda (CommandParser &context_ ,const PhanBuffer<const STRU8> &data) popping : mContext (context_) ,mTextReader (data) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = RegularReader<ARGC<2>> (PhanRef<TextReader<STRU8>>::make (mTextReader)) ;
			mOptionSet = Set<String<STRU8>> () ;
			mAttributeSet = Set<String<STRU8> ,String<STRU8>> () ;
			mCommandList = SList<String<STRU8>> () ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
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
		inline void update_shift_e0 () {
			update_shift_e8 () ;
			mRis >> RegularReader<>::SKIP_GAP_SPACE_ONLY ;
			update_shift_e7 () ;
			mRis >> RegularReader<>::SKIP_GAP_SPACE_ONLY ;
			update_shift_e9 () ;
		}

		//@info: $1->${identity}
		inline void update_shift_e1 () {
			mRis >> RegularReader<>::HINT_IDENTIFIER_TEXT >> mLatestString ;
		}

		//@info: $2->"${string}"
		inline void update_shift_e2 () {
			mRis >> RegularReader<>::HINT_STRING_TEXT >> mLatestString ;
		}

		//@info: $3->${newgap}
		inline void update_shift_e3 () {
			mRis >> RegularReader<>::HINT_NEWGAP_TEXT >> mLatestString ;
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
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('=')))
					discard ;
				if (!(mRis[1] == STRU8 ('\"')))
					discard ;
				mRis >> _PCSTRU8_ ("=") ;
				update_shift_e2 () ;
				mAttributeSet[ix].item = std::move (mLatestString) ;
			}
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('=')))
					discard ;
				mRis >> _PCSTRU8_ ("=") ;
				update_shift_e3 () ;
				mAttributeSet[ix].item = std::move (mLatestString) ;
			}
			if switch_case (fax) {
				mAttributeSet[ix].item = _PCSTRU8_ ("TRUE") ;
			}
		}

		//@info: $6->$2|$3
		inline void update_shift_e6 () {
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(mRis[0] == STRU8 ('\"')))
					discard ;
				update_shift_e2 () ;
			}
			if switch_case (fax) {
				update_shift_e3 () ;
			}
			mCommandList.add (std::move (mLatestString)) ;
		}

		//@info: $7->${eps}|$4 $7|$5 $7|$6 $7
		inline void update_shift_e7 () {
			while (TRUE) {
				if (mRis[0] == STRU8 ('\0'))
					break ;
				auto fax = TRUE ;
				if switch_case (fax) {
					if (!(mRis[0] == STRU8 ('/')))
						discard ;
					update_shift_e4 () ;
				}
				if switch_case (fax) {
					if (!(mRis[0] == STRU8 ('-')))
						discard ;
					update_shift_e5 () ;
				}
				if switch_case (fax) {
					update_shift_e6 () ;
				}
				mRis >> RegularReader<>::SKIP_GAP_SPACE_ONLY ;
			}
		}

		//@info: $8->${eps}
		inline void update_shift_e8 () {
			_STATIC_WARNING_ ("noop") ;
		}

		//@info: $9->${end}
		inline void update_shift_e9 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_command () {
			mCommand = Array<String<STRU8>> (mCommandList.length ()) ;
			for (auto &&i : _RANGE_ (0 ,mCommandList.length ()))
				mCommand[i] = std::move (mCommandList[mCommandList.access (i)]) ;
		}

		inline void refresh () {
			mContext.mOptionSet = std::move (mOptionSet) ;
			mContext.mAttributeSet = std::move (mAttributeSet) ;
			mContext.mCommand = std::move (mCommand) ;
		}
	} ;
	_CALL_ (Lambda ((*this) ,data)) ;
}
} ;