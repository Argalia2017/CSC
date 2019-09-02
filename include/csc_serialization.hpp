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
		INDEX ix = VAR_NONE ;
		for (FOR_ONCE_DO) {
			if (!exist ())
				discard ;
			INDEX jx = mHeap.self[mIndex].mObjectSet.find (name) ;
			if (jx == VAR_NONE)
				discard ;
			ix = mHeap.self[mIndex].mObjectSet[jx].item ;
		}
		return XmlParser (mHeap ,ix) ;
	}

	Array<XmlParser> child_array () const {
		Array<XmlParser> ret ;
		for (FOR_ONCE_DO) {
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
		for (FOR_ONCE_DO) {
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
		if (!exist () && !that.exist ())
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
		INDEX ix = mHeap.self[mIndex].mAttributeSet.min_one () ;
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

	void serialize (TextWriter<STRU8> &writer) const ;

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

inline void XmlParser::serialize (TextWriter<STRU8> &writer) const {
	auto rax = Deque<ARRAY2<INDEX>> () ;
	auto rbx = Deque<ARRAY2<INDEX>> () ;
	rax.add (ARRAY2<INDEX> {mIndex ,FLAG (0)}) ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		const auto r1x = std::move (rax[rax.tail ()]) ;
		rax.pop () ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			//@info: case '<?xml ...>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x[0]].mName.empty ()))
				discard ;
			if (!(r1x[1] == 0))
				discard ;
			auto &r1 = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<?xml version=\"1.0\" encoding=\"utf-8\" ?>") ;
			writer << _GAP_ ;
			rbx.clear () ;
			for (INDEX i = r1.mChild ; i != VAR_NONE ; i = mHeap.self[i].mBrother)
				rbx.add (ARRAY2<INDEX> {i ,FLAG (0)}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if SWITCH_CASE (ifa) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?/>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x[0]].mChild == VAR_NONE))
				discard ;
			if (!(r1x[1] == 0))
				discard ;
			auto &r2 = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<") << r2.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r2.mAttributeSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << i.item << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ ("/>") ;
		}
		if SWITCH_CASE (ifa) {
			//@info: case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r1x[0]].mChild != VAR_NONE))
				discard ;
			if (!(r1x[1] == 0))
				discard ;
			auto &r3 = mHeap.self[r1x[0]] ;
			writer << _PCSTRU8_ ("<") << r3.mName << _PCSTRU8_ (" ") ;
			for (auto &&i : r3.mAttributeSet) {
				writer << i.key ;
				writer << _PCSTRU8_ ("=\"") ;
				writer << i.item << _PCSTRU8_ ("\" ") ;
			}
			writer << _PCSTRU8_ (">") ;
			rbx.clear () ;
			for (INDEX i = r3.mChild ; i != VAR_NONE ; i = mHeap.self[i].mBrother)
				rbx.add (ARRAY2<INDEX> {i ,FLAG (0)}) ;
			rbx.add (ARRAY2<INDEX> {r1x[0] ,FLAG (1)}) ;
			while (TRUE) {
				if (rbx.empty ())
					break ;
				rax.add (rbx[rbx.tail ()]) ;
				rbx.pop () ;
			}
		}
		if SWITCH_CASE (ifa) {
			//@info: case '</xxx>'
			if (!(r1x[0] != VAR_NONE))
				discard ;
			if (!(r1x[1] == 1))
				discard ;
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
				using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
				_DYNAMIC_ASSERT_ (Counter::mSelf < DEFAULT_RECURSIVE_SIZE::value) ;
				Counter::mSelf++ ;
			}

			inline void unlock () {
				Counter::mSelf-- ;
			}
		} ;

	private:
		XmlParser &mContext ;

		TextReader<STRU8> mTextReader ;
		RegularReader<ARGC<2>> mRis ;
		LENGTH mRecursiveCounter ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<INDEX ,INDEX> mMemberSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Node ,SAUTO> mNodeHeap ;
		SharedRef<FixedBuffer<Node>> mHeap ;
		INDEX mRoot ;
		INDEX mLatestIndex ;
		String<STRU8> mLatestString ;

	public:
		inline explicit Lambda (XmlParser &context ,const PhanBuffer<const STRU8> &data) popping : mContext (context) ,mTextReader (data) {}

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
			$0->$8 $7 $9
			$1->${identity}
			$2->"${string}"
			$3->$1 = $2
			$4->${eps}|$3 $4
			$5-><$1 $4 />|<$1 $4 > $7 </$1 >
			$6-><!--comment-->
			$7->${eps}|$5 $7|$6 $7
			$8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
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
				const auto r1x = BOOL (mRis[0] >= STRU8 ('A') && mRis[0] <= STRU8 ('Z')) ;
				const auto r2x = BOOL (mRis[0] >= STRU8 ('a') && mRis[0] <= STRU8 ('z')) ;
				const auto r3x = BOOL (mRis[0] == STRU8 ('_')) ;
				if (!r1x && !r2x && !r3x)
					break ;
				update_shift_e3 (curr) ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
		}

		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		inline void update_shift_e5 (INDEX curr) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
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
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
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
			if SWITCH_CASE (ifa) {
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
				if (mRis[0] == STRU8 ('-') && mRis[1] == STRU8 ('-'))
					break ;
				mRis++ ;
			}
			mRis >> _PCSTRU8_ ("-->") ;
		}

		//@info: $7->${eps}|$5 $7|$6 $7
		inline void update_shift_e7 (INDEX curr) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				const auto r1x = BOOL (mRis[0] == STRU8 ('<') && mRis[1] == STRU8 ('!')) ;
				const auto r2x = BOOL (mRis[0] == STRU8 ('<') && mRis[1] != STRU8 ('/')) ;
				if (!r1x && !r2x)
					break ;
				auto ifa = FALSE ;
				if SWITCH_CASE (ifa) {
					if (!r1x)
						discard ;
					update_shift_e6 () ;
				}
				if SWITCH_CASE (ifa) {
					if (!r2x)
						discard ;
					update_shift_e5 (curr) ;
					mNodeHeap[curr].mMemberSet.add (mNodeHeap[curr].mMemberSet.length () ,mLatestIndex) ;
					mNodeHeap[curr].mObjectSet.add (mNodeHeap[mLatestIndex].mName ,mLatestIndex) ;
					auto &r1 = _SWITCH_ (
						(ix == VAR_NONE) ? ix :
						(mNodeHeap[iy].mBrother)) ;
					r1 = mLatestIndex ;
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
			for (FOR_ONCE_DO) {
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

		//@info: $9->#
		inline void update_shift_e9 () {
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
		}

		inline void update_heap () {
			mAttributeSoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ,ie = mNodeHeap.size () ; i < ie ; i++) {
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
		XmlParser &mContext ;
		const Array<XmlParser> &mSequence ;
		const String<STRU8> mClazzString = String<STRU8> (_PCSTRU8_ ("type")) ;
		const String<STRU8> mTableClazzString = String<STRU8> (_PCSTRU8_ ("table")) ;
		const String<STRU8> mObjectClazzString = String<STRU8> (_PCSTRU8_ ("object")) ;
		const String<STRU8> mArrayClazzString = String<STRU8> (_PCSTRU8_ ("array")) ;
		const String<STRU8> mFinalClazzString = String<STRU8> (_PCSTRU8_ ("final")) ;

		Deque<PACK<Deque<XmlParser> ,INDEX ,ARRAY2<INDEX>>> mNodeStack ;
		ARRAY4<Function<DEF<void (const XmlParser &)> NONE::*>> mFoundNodeProc ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<INDEX ,INDEX> mMemberSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Node ,SAUTO> mNodeHeap ;
		String<STRU8> mRootName ;
		INDEX mRootType ;
		Deque<ARRAY4<INDEX>> mFoundNodeList ;
		Set<String<STRU8> ,INDEX> mFoundNodeNameSet ;
		Deque<SoftSet<String<STRU8> ,String<STRU8>>> mFoundNodeAttributeSetList ;
		Deque<FLAG> mFoundNodeTypeList ;
		ARRAY2<Deque<Deque<XmlParser>>> mFoundNodeBaseList ;
		SharedRef<FixedBuffer<Node>> mHeap ;
		INDEX mRoot ;

		PACK<Deque<XmlParser> ,INDEX ,ARRAY2<INDEX>> mTempNode ;

	public:
		inline explicit Lambda (XmlParser &context ,const Array<XmlParser> &sequence) popping : mContext (context) ,mSequence (sequence) {}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mNodeStack = Deque<PACK<Deque<XmlParser> ,INDEX ,ARRAY2<INDEX>>> () ;
			mFoundNodeProc[0] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_table_node) ;
			mFoundNodeProc[1] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_object_node) ;
			mFoundNodeProc[2] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_array_node) ;
			mFoundNodeProc[3] = Function<DEF<void (const XmlParser &)> NONE::*> (PhanRef<Lambda>::make ((*this)) ,&Lambda::update_found_table_node) ;
			mAttributeSoftSet = SoftSet<String<STRU8> ,String<STRU8>> (0) ;
			mMemberSoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Node ,SAUTO> () ;
			for (FOR_ONCE_DO) {
				mRootName.clear () ;
				mRootType = VAR_NONE ;
				INDEX ix = normal_node_one () ;
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
			mFoundNodeList = Deque<ARRAY4<INDEX>> () ;
			mFoundNodeNameSet = Set<String<STRU8> ,INDEX> () ;
			mFoundNodeAttributeSetList = Deque<SoftSet<String<STRU8> ,String<STRU8>>> () ;
			mFoundNodeTypeList = Deque<FLAG> () ;
			mFoundNodeBaseList[0] = Deque<Deque<XmlParser>> () ;
			mFoundNodeBaseList[1] = Deque<Deque<XmlParser>> () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make () ;
		}

		inline INDEX normal_node_one () const {
			for (auto &&i : mSequence)
				if (i.exist ())
					return mSequence.at (i) ;
			return VAR_NONE ;
		}

		inline INDEX node_type (const XmlParser &node) const {
			auto &r1 = node.attribute (mClazzString) ;
			if (r1 == mTableClazzString)
				return NODE_CLAZZ_TABLE ;
			if (r1 == mObjectClazzString)
				return NODE_CLAZZ_OBJECT ;
			if (r1 == mArrayClazzString)
				return NODE_CLAZZ_ARRAY ;
			if (r1 == mFinalClazzString)
				return NODE_CLAZZ_FINAL ;
			_DYNAMIC_ASSERT_ (r1.empty ()) ;
			return NODE_CLAZZ_TABLE ;
		}

		inline void generate () {
			update_root_node () ;
			while (TRUE) {
				if (mNodeStack.empty ())
					break ;
				mTempNode = std::move (mNodeStack[mNodeStack.tail ()]) ;
				mNodeStack.pop () ;
				for (FOR_ONCE_DO) {
					if (mTempNode.P2 == VAR_NONE)
						discard ;
					for (auto &&i : mTempNode.P1)
						mFoundNodeProc[mTempNode.P2] (i) ;
					update_merge_found_node (mTempNode.P3[0]) ;
					mFoundNodeBaseList[1].add (std::move (mTempNode.P1)) ;
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
			for (FOR_ONCE_DO) {
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
			mNodeStack[ix].P3[0] = iy ;
			mNodeStack[ix].P3[1] = VAR_NONE ;
		}

		inline void update_found_table_node (const XmlParser &node) {
			for (XmlParser i = node ; i.exist () ; i = i.brother ()) {
				INDEX ix = find_found_node_name (i.name ()) ;
				INDEX iy = mFoundNodeList.insert () ;
				const auto r1x = mFoundNodeNameSet.length () ;
				INDEX jx = mFoundNodeNameSet.insert (i.name ()) ;
				for (FOR_ONCE_DO) {
					if (mFoundNodeNameSet.length () == r1x)
						discard ;
					mFoundNodeNameSet[jx].item = iy ;
				}
				INDEX jy = mFoundNodeAttributeSetList.insert () ;
				INDEX jz = mFoundNodeBaseList[0].insert () ;
				INDEX kx = mFoundNodeTypeList.insert () ;
				mFoundNodeTypeList[kx] = node_type (i) ;
				mFoundNodeList[iy][0] = jx ;
				mFoundNodeList[iy][1] = jy ;
				mFoundNodeList[iy][2] = kx ;
				mFoundNodeList[iy][3] = jz ;
				for (FOR_ONCE_DO) {
					if (ix == VAR_NONE)
						discard ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[ix][2]] != NODE_CLAZZ_FINAL) ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[iy][2]] != NODE_CLAZZ_FINAL) ;
				}
				for (FOR_ONCE_DO) {
					if (mFoundNodeTypeList[mFoundNodeList[iy][2]] != NODE_CLAZZ_FINAL)
						discard ;
					_DYNAMIC_ASSERT_ (i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				}
				mFoundNodeAttributeSetList[jy] = mAttributeSoftSet.share () ;
				mFoundNodeAttributeSetList[jy].appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				if (mFoundNodeBaseList[1].empty ())
					mFoundNodeBaseList[1].add (Deque<XmlParser> ()) ;
				mFoundNodeBaseList[1].take (mFoundNodeBaseList[0][jz]) ;
				mFoundNodeBaseList[0][jz].clear () ;
				mFoundNodeBaseList[0][jz].add (i.child ()) ;
			}
		}

		inline void update_found_object_node (const XmlParser &node) {
			for (XmlParser i = node ; i.exist () ; i = i.brother ()) {
				INDEX ix = find_found_node_name (i.name ()) ;
				INDEX iy = ix ;
				for (FOR_ONCE_DO) {
					if (iy != VAR_NONE)
						discard ;
					iy = mFoundNodeList.insert () ;
					const auto r1x = mFoundNodeNameSet.length () ;
					INDEX jx = mFoundNodeNameSet.insert (i.name ()) ;
					for (FOR_ONCE_DO) {
						if (mFoundNodeNameSet.length () == r1x)
							discard ;
						mFoundNodeNameSet[jx].item = iy ;
					}
					INDEX jy = mFoundNodeAttributeSetList.insert () ;
					INDEX jz = mFoundNodeBaseList[0].insert () ;
					INDEX kx = mFoundNodeTypeList.insert () ;
					mFoundNodeTypeList[kx] = node_type (i) ;
					mFoundNodeList[iy][0] = jx ;
					mFoundNodeList[iy][1] = jy ;
					mFoundNodeList[iy][2] = kx ;
					mFoundNodeList[iy][3] = jz ;
					mFoundNodeAttributeSetList[jy] = mAttributeSoftSet.share () ;
					if (mFoundNodeBaseList[1].empty ())
						mFoundNodeBaseList[1].add (Deque<XmlParser> ()) ;
					mFoundNodeBaseList[1].take (mFoundNodeBaseList[0][jz]) ;
					mFoundNodeBaseList[0][jz].clear () ;
				}
				for (FOR_ONCE_DO) {
					if (ix == VAR_NONE)
						discard ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[ix][2]] == mFoundNodeTypeList[mFoundNodeList[iy][2]]) ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[ix][2]] != NODE_CLAZZ_FINAL) ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[iy][2]] != NODE_CLAZZ_FINAL) ;
				}
				for (FOR_ONCE_DO) {
					if (mFoundNodeTypeList[mFoundNodeList[iy][2]] != NODE_CLAZZ_FINAL)
						discard ;
					_DYNAMIC_ASSERT_ (i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				}
				mFoundNodeAttributeSetList[mFoundNodeList[iy][1]].appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				mFoundNodeBaseList[0][mFoundNodeList[iy][3]].add (i.child ()) ;
			}
		}

		inline void update_found_array_node (const XmlParser &node) {
			if (!mFoundNodeList.empty ())
				return ;
			for (XmlParser i = node ; i.exist () ; i = i.brother ()) {
				INDEX ix = mFoundNodeNameSet.min_one () ;
				INDEX iy = mFoundNodeList.insert () ;
				const auto r1x = mFoundNodeNameSet.length () ;
				INDEX jx = mFoundNodeNameSet.insert (i.name ()) ;
				for (FOR_ONCE_DO) {
					if (mFoundNodeNameSet.length () == r1x)
						discard ;
					mFoundNodeNameSet[jx].item = iy ;
				}
				INDEX jy = mFoundNodeAttributeSetList.insert () ;
				INDEX jz = mFoundNodeBaseList[0].insert () ;
				INDEX kx = mFoundNodeTypeList.insert () ;
				mFoundNodeTypeList[kx] = node_type (i) ;
				mFoundNodeList[iy][0] = jx ;
				mFoundNodeList[iy][1] = jy ;
				mFoundNodeList[iy][2] = kx ;
				mFoundNodeList[iy][3] = jz ;
				for (FOR_ONCE_DO) {
					if (ix == VAR_NONE)
						discard ;
					_DYNAMIC_ASSERT_ (mFoundNodeList[ix][0] == mFoundNodeList[iy][0]) ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[ix][2]] != NODE_CLAZZ_FINAL) ;
					_DYNAMIC_ASSERT_ (mFoundNodeTypeList[mFoundNodeList[iy][2]] != NODE_CLAZZ_FINAL) ;
				}
				for (FOR_ONCE_DO) {
					if (mFoundNodeTypeList[mFoundNodeList[iy][2]] != NODE_CLAZZ_FINAL)
						discard ;
					_DYNAMIC_ASSERT_ (i.mHeap.self[i.mIndex].mAttributeSet.length () == 1) ;
				}
				mFoundNodeAttributeSetList[jy] = mAttributeSoftSet.share () ;
				mFoundNodeAttributeSetList[jy].appand (i.mHeap.self[i.mIndex].mAttributeSet) ;
				if (mFoundNodeBaseList[1].empty ())
					mFoundNodeBaseList[1].add (Deque<XmlParser> ()) ;
				mFoundNodeBaseList[1].take (mFoundNodeBaseList[0][jz]) ;
				mFoundNodeBaseList[0][jz].clear () ;
				mFoundNodeBaseList[0][jz].add (i.child ()) ;
			}
		}

		inline INDEX find_found_node_name (const String<STRU8> &name) const {
			INDEX ret = mFoundNodeNameSet.find (name) ;
			if (ret != VAR_NONE)
				ret = mFoundNodeNameSet[ret].item ;
			return std::move (ret) ;
		}

		inline void update_merge_found_node (INDEX curr) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			for (auto &&i : mFoundNodeList) {
				iy = ix ;
				ix = mNodeHeap.alloc () ;
				mNodeHeap[ix].mName = std::move (mFoundNodeNameSet[i[0]]) ;
				mNodeHeap[ix].mAttributeSet = std::move (mFoundNodeAttributeSetList[i[1]]) ;
				mNodeHeap[ix].mParent = curr ;
				for (FOR_ONCE_DO) {
					INDEX jx = mNodeHeap[ix].mParent ;
					if (jx == VAR_NONE)
						discard ;
					if (mNodeHeap[jx].mChild == VAR_NONE)
						mNodeHeap[jx].mChild = ix ;
					for (FOR_ONCE_DO) {
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
				mNodeStack[jy].P1 = std::move (mFoundNodeBaseList[0][i[3]]) ;
				mNodeStack[jy].P2 = mFoundNodeTypeList[i[2]] ;
				mNodeStack[jy].P3[0] = ix ;
				mNodeStack[jy].P3[1] = VAR_NONE ;
			}
			mFoundNodeNameSet.clear () ;
			mFoundNodeAttributeSetList.clear () ;
			mFoundNodeTypeList.clear () ;
			mFoundNodeBaseList[1].appand (std::move (mFoundNodeBaseList[0])) ;
			mFoundNodeBaseList[0].clear () ;
			mFoundNodeList.clear () ;
		}

		inline void update_heap () {
			mAttributeSoftSet.clean () ;
			mObjectSoftSet.clean () ;
			mHeap = SharedRef<FixedBuffer<Node>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ,ie = mNodeHeap.size () ; i < ie ; i++) {
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
		INDEX ix = VAR_NONE ;
		for (FOR_ONCE_DO) {
			if (!exist ())
				discard ;
			if (!object_type ())
				discard ;
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			INDEX jx = r1.find (key) ;
			if (jx == VAR_NONE)
				discard ;
			ix = r1[jx].item ;
		}
		return JsonParser (mHeap ,ix) ;
	}

	Array<JsonParser> child_array () const {
		Array<JsonParser> ret ;
		for (FOR_ONCE_DO) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			ret = Array<JsonParser> (r1.length ()) ;
			INDEX iw = 0 ;
			for (auto &&i : r1)
				ret[iw++] = JsonParser (mHeap ,i.item) ;
			_DEBUG_ASSERT_ (iw == ret.length ()) ;
		}
		return std::move (ret) ;
	}

	Array<JsonParser> child_array (LENGTH fixed_len) const {
		Array<JsonParser> ret = Array<JsonParser> (fixed_len) ;
		INDEX iw = 0 ;
		for (FOR_ONCE_DO) {
			if (!exist ())
				discard ;
			if (!array_type ())
				discard ;
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			for (auto &&i : r1) {
				INDEX ix = iw++ ;
				if (ix >= ret.size ())
					continue ;
				ret[ix] = JsonParser (mHeap ,i.item) ;
			}
		}
		return std::move (ret) ;
	}

	BOOL equal (const JsonParser &that) const {
		if (!exist () && !that.exist ())
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

	void serialize (TextWriter<STRU8> &writer) const ;

private:
	explicit JsonParser (const SharedRef<FixedBuffer<Node>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

private:
	void initialize (const PhanBuffer<const STRU8> &data) ;

	Set<PTR<const String<STRU8>>> object_key_adress_set () const {
		Set<PTR<const String<STRU8>>> ret = Set<PTR<const String<STRU8>>> (mHeap->size ()) ;
		for (INDEX i = 0 ,ie = mHeap->size () ; i < ie ; i++) {
			if (mHeap.self[i].mClazz != NODE_CLAZZ_OBJECT)
				continue ;
			auto &r1 = mHeap.self[i].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			for (auto &&j : r1)
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

inline void JsonParser::serialize (TextWriter<STRU8> &writer) const {
	auto rax = Deque<ARRAY2<INDEX>> () ;
	auto rbx = Deque<ARRAY2<INDEX>> () ;
	rax.add (ARRAY2<INDEX> {mIndex ,FLAG (0)}) ;
	const auto r1x = object_key_adress_set () ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		const auto r2x = std::move (rax[rax.tail ()]) ;
		rax.pop () ;
		auto ifa = FALSE ;
		if SWITCH_CASE (ifa) {
			//@info: case 'null'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_NULL))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			writer << _PCSTRU8_ ("null") ;
		}
		if SWITCH_CASE (ifa) {
			//@info: case '"xxx"'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_STRING))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			auto &r2 = mHeap.self[r2x[0]].mValue.rebind<String<STRU8>> ().self ;
			writer << _PCSTRU8_ ("\"") ;
			writer << r2 ;
			writer << _PCSTRU8_ ("\"") ;
		}
		if SWITCH_CASE (ifa) {
			//@info: case '[(yyy(,yyy)*)?]'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_ARRAY))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			auto &r3 = mHeap.self[r2x[0]].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			rbx.clear () ;
			rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (2)}) ;
			INDEX ir = 0 ;
			for (auto &&i : r3) {
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
		if SWITCH_CASE (ifa) {
			//@info: case '{("xxx":yyy(,"xxx":yyy)*)?}'
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(mHeap.self[r2x[0]].mClazz == NODE_CLAZZ_OBJECT))
				discard ;
			if (!(r2x[1] == 0))
				discard ;
			auto &r4 = mHeap.self[r2x[0]].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			rbx.clear () ;
			rbx.add (ARRAY2<INDEX> {VAR_NONE ,FLAG (5)}) ;
			INDEX ir = 0 ;
			for (auto &&i : r4) {
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
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] != VAR_NONE))
				discard ;
			if (!(r2x[1] == 1))
				discard ;
			writer << _PCSTRU8_ ("\"") ;
			const auto r3x = r1x[r2x[0]].key ;
			writer << (*r3x) ;
			writer << _PCSTRU8_ ("\"") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 2))
				discard ;
			writer << _PCSTRU8_ ("[") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 3))
				discard ;
			writer << _PCSTRU8_ (",") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 4))
				discard ;
			writer << _PCSTRU8_ ("]") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 5))
				discard ;
			writer << _PCSTRU8_ ("{") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 6))
				discard ;
			writer << _PCSTRU8_ (",") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 7))
				discard ;
			writer << _PCSTRU8_ (":") ;
		}
		if SWITCH_CASE (ifa) {
			if (!(r2x[0] == VAR_NONE))
				discard ;
			if (!(r2x[1] == 8))
				discard ;
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
				using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
				_DYNAMIC_ASSERT_ (Counter::mSelf < DEFAULT_RECURSIVE_SIZE::value) ;
				Counter::mSelf++ ;
			}

			inline void unlock () {
				Counter::mSelf-- ;
			}
		} ;

	private:
		JsonParser &mContext ;

		TextReader<STRU8> mTextReader ;
		RegularReader<ARGC<2>> mRis ;
		LENGTH mRecursiveCounter ;
		SoftSet<INDEX ,INDEX> mArraySoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Node ,SAUTO> mNodeHeap ;
		SharedRef<FixedBuffer<Node>> mHeap ;
		INDEX mRoot ;
		INDEX mLatestIndex ;
		String<STRU8> mLatestString ;

	public:
		inline explicit Lambda (JsonParser &context ,const PhanBuffer<const STRU8> &data) popping : mContext (context) ,mTextReader (data) {}

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
			$0->$11 $10 $12
			$1->${value}
			$2->true|TRUE|false|FALSE
			$2x->null
			$3->"${string}"
			$4->$1|$2|$2x|$3|$6|$9
			$5->$4|$4 , $5
			$6->[ ]|[ $5 ]
			$7->$2 : $4
			$8->$7|$7 , $8
			$9->{ }|{ $8 }
			$10->${eps}|$4
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
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('t')))
					discard ;
				mRis >> _PCSTRU8_ ("true") ;
				mLatestString = _PCSTRU8_ ("true") ;
			}
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('T')))
					discard ;
				mRis >> _PCSTRU8_ ("TRUE") ;
				mLatestString = _PCSTRU8_ ("TRUE") ;
			}
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('f')))
					discard ;
				mRis >> _PCSTRU8_ ("false") ;
				mLatestString = _PCSTRU8_ ("false") ;
			}
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('F')))
					discard ;
				mRis >> _PCSTRU8_ ("FALSE") ;
				mLatestString = _PCSTRU8_ ("FALSE") ;
			}
			if SWITCH_CASE (ifa) {
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
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			INDEX ix = VAR_NONE ;
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
				const auto r1x = BOOL (mRis[0] == STRU8 ('+') || mRis[0] == STRU8 ('-')) ;
				const auto r2x = BOOL (mRis[0] >= STRU8 ('0') && mRis[0] <= STRU8 ('9')) ;
				if (!(r1x || r2x))
					discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e1 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if SWITCH_CASE (ifa) {
				const auto r3x = BOOL (mRis[0] == STRU8 ('t') || mRis[0] == STRU8 ('T')) ;
				const auto r4x = BOOL (mRis[0] == STRU8 ('f') || mRis[0] == STRU8 ('F')) ;
				if (!(r3x || r4x))
					discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e2 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLatestString)) ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_STRING ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('n')))
					discard ;
				ix = mNodeHeap.alloc () ;
				update_shift_e2x () ;
				mNodeHeap[ix].mClazz = NODE_CLAZZ_NULL ;
				mNodeHeap[ix].mParent = curr ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			if SWITCH_CASE (ifa) {
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
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('[')))
					discard ;
				update_shift_e6 (curr) ;
				ix = mLatestIndex ;
			}
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('{')))
					discard ;
				update_shift_e9 (curr) ;
				ix = mLatestIndex ;
			}
			if SWITCH_CASE (ifa) {
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
				auto &r1 = mNodeHeap[curr].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
				r1.add (r1.length () ,mLatestIndex) ;
				auto &r2 = _SWITCH_ (
					(ix == VAR_NONE) ? ix :
					(mNodeHeap[iy].mBrother)) ;
				r2 = mLatestIndex ;
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
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("[") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<INDEX ,INDEX>>::make (mArraySoftSet.share ()) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_ARRAY ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> RegularReader<>::SKIP_GAP ;
			for (FOR_ONCE_DO) {
				if (mRis[0] == STRU8 (']'))
					discard ;
				update_shift_e5 (ix) ;
				mNodeHeap[ix].mChild = mLatestIndex ;
				mRis >> RegularReader<>::SKIP_GAP ;
			}
			mRis >> _PCSTRU8_ ("]") ;
			mLatestIndex = ix ;
		}

		//@info: $7->$2 : $4
		inline void update_shift_e7 (INDEX curr) {
			update_shift_e3 () ;
			auto &r1 = mNodeHeap[curr].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			INDEX ix = r1.insert (std::move (mLatestString)) ;
			mRis >> RegularReader<>::SKIP_GAP ;
			mRis >> _PCSTRU8_ (":") ;
			mRis >> RegularReader<>::SKIP_GAP ;
			update_shift_e4 (curr) ;
			r1[ix].item = mLatestIndex ;
		}

		//@info: $8->$7|$7 , $8
		inline void update_shift_e8 (INDEX curr) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e7 (curr) ;
				auto &r1 = _SWITCH_ (
					(ix == VAR_NONE) ? ix :
					(mNodeHeap[iy].mBrother)) ;
				r1 = mLatestIndex ;
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
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveCounter)) ;
			mRis >> _PCSTRU8_ ("{") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<String<STRU8> ,INDEX>>::make (mObjectSoftSet.share ()) ;
			mNodeHeap[ix].mClazz = NODE_CLAZZ_OBJECT ;
			mNodeHeap[ix].mParent = curr ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis >> RegularReader<>::SKIP_GAP ;
			for (FOR_ONCE_DO) {
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
			for (FOR_ONCE_DO) {
				if (mRis[0] == STRU8 ('\0'))
					discard ;
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
			mHeap = SharedRef<FixedBuffer<Node>>::make (mNodeHeap.length ()) ;
			INDEX iw = 0 ;
			for (INDEX i = 0 ,ie = mNodeHeap.size () ; i < ie ; i++) {
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

#ifdef __CSC_DEPRECATED__
	explicit CommandParser (VAR32 argc ,PTR<const PTR<STRA>> argv) {
		const auto r1x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8>::make () ;
			auto wos = TextWriter<STRU8> (ret.raw ()) ;
			for (INDEX i = 1 ,ie = LENGTH (argc) ; i < ie ; i++) {
				wos << _ASTOU8S_ (PTRTOARR[argv[i]]) ;
				wos << _PCSTRU8_ (" ") ;
			}
			wos << _EOS_ ;
			return std::move (ret) ;
		}) ;
		initialize (r1x.raw ()) ;
	}
#endif

	BOOL option (const String<STRU8> &tag) const {
		INDEX ix = mOptionSet.find (tag) ;
		if (ix == VAR_NONE)
			return FALSE ;
		return TRUE ;
	}

	const String<STRU8> &attribute (const String<STRU8> &tag) const & {
		auto &r1 = _CACHE_ ([] () {
			return String<STRU8> () ;
		}) ;
		INDEX ix = mAttributeSet.find (tag) ;
		if (ix == VAR_NONE)
			return r1 ;
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
		Set<String<STRU8>> mOptionSet ;
		Set<String<STRU8> ,String<STRU8>> mAttributeSet ;
		Deque<String<STRU8>> mCommandList ;
		Array<String<STRU8>> mCommand ;
		String<STRU8> mLatestString ;

	public:
		inline explicit Lambda (CommandParser &context ,const PhanBuffer<const STRU8> &data) popping : mContext (context) ,mTextReader (data) {}

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
			mCommandList = Deque<String<STRU8>> () ;
		}

		inline void generate () {
			_STATIC_WARNING_ ("note") ;
			/*
			$0->$8 $7 $9
			$1->${identity}
			$2->"${string}"
			$3->${newgap}
			$4->/$1
			$5->-$1|-$1=$2|-$1=$3
			$6->$2|$3
			$7->${eps}|$4 $7|$5 $7|$6 $7
			$8->#
			$9->#
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
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('=')))
					discard ;
				if (!(mRis[1] == STRU8 ('\"')))
					discard ;
				mRis >> _PCSTRU8_ ("=") ;
				update_shift_e2 () ;
				mAttributeSet[ix].item = std::move (mLatestString) ;
			}
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('=')))
					discard ;
				mRis >> _PCSTRU8_ ("=") ;
				update_shift_e3 () ;
				mAttributeSet[ix].item = std::move (mLatestString) ;
			}
			if SWITCH_CASE (ifa) {
				mAttributeSet[ix].item = _PCSTRU8_ ("TRUE") ;
			}
		}

		//@info: $6->$2|$3
		inline void update_shift_e6 () {
			auto ifa = FALSE ;
			if SWITCH_CASE (ifa) {
				if (!(mRis[0] == STRU8 ('\"')))
					discard ;
				update_shift_e2 () ;
			}
			if SWITCH_CASE (ifa) {
				update_shift_e3 () ;
			}
			mCommandList.add (std::move (mLatestString)) ;
		}

		//@info: $7->${eps}|$4 $7|$5 $7|$6 $7
		inline void update_shift_e7 () {
			while (TRUE) {
				if (mRis[0] == STRU8 ('\0'))
					break ;
				auto ifa = FALSE ;
				if SWITCH_CASE (ifa) {
					if (!(mRis[0] == STRU8 ('/')))
						discard ;
					update_shift_e4 () ;
				}
				if SWITCH_CASE (ifa) {
					if (!(mRis[0] == STRU8 ('-')))
						discard ;
					update_shift_e5 () ;
				}
				if SWITCH_CASE (ifa) {
					update_shift_e6 () ;
				}
				mRis >> RegularReader<>::SKIP_GAP_SPACE_ONLY ;
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
	_CALL_ (Lambda ((*this) ,data)) ;
}
} ;