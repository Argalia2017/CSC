#pragma once

#ifndef __CSC_SERIALIZE__
#define __CSC_SERIALIZE__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
template <class SIZE>
class LLTextReader {
private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	TextReader<STRU8> mReader ;
	Array<STRU8 ,SIZE> mCache ;
	INDEX mWrite ;

public:
	LLTextReader () {
		mWrite = 0 ;
	}

	explicit LLTextReader (const TextReader<STRU8> &reader) :mReader (reader.clone ()) {
		update_reader () ;
	}

	LLTextReader clone () const {
		LLTextReader ret ;
		ret.mReader = mReader.clone () ;
		ret.mCache = mCache ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	const STRU8 &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
#ifdef __CSC_COMPILER_GNUC__
		//@info:gcc may doubt some 'mWrite' will cause overflow
		_DEBUG_ASSERT_ ((mWrite + index) % mCache.length () >= 0) ;
		_DEBUG_ASSERT_ ((mWrite + index) % mCache.length () < mCache.length ()) ;
#endif
		return mCache[(mWrite + index) % mCache.length ()] ;
	}

	inline const STRU8 &operator[] (INDEX index) const {
		return get (index) ;
	}

	void shift () {
		mReader.read (mCache[mWrite]) ;
		mWrite = (mWrite + 1) % mCache.length () ;
	}

	inline void operator++ (int) {
		shift () ;
	}

	template <LENGTH _VAL>
	void shift (const DEF<STRU8[_VAL]> &val) {
		for (INDEX i = 0 ; i < _VAL - 1 ; i++) {
			_DYNAMIC_ASSERT_ (get (0) == val[i]) ;
			shift () ;
		}
	}

	template <LENGTH _VAL>
	inline LLTextReader &operator<< (const DEF<STRU8[_VAL]> &val) {
		shift (val) ;
		return *this ;
	}

	void skip () {
		while (mReader.attr ().varify_space (get (0)))
			shift () ;
	}

	void skip_newline () {
		skip_space () ;
		while (mReader.attr ().varify_space (get (0) ,2))
			shift () ;
		skip_space () ;
	}

	void skip_space () {
		while (mReader.attr ().varify_space (get (0) ,1))
			shift () ;
	}

	void read_text (String<STRU8> &dst) {
		for (INDEX i = 0 ; i < dst.size () ; i++) {
			dst[i] = get (0) ;
			shift () ;
		}
	}

	String<STRU8> read_identifer () popping {
		const auto r1x = _CALL_ ([&] () {
			LENGTH ret = 0 ;
			LLTextReader ris = clone () ;
			_DYNAMIC_ASSERT_ ((ris[0] >= STRU8 ('A') && ris[0] <= STRU8 ('Z')) || (ris[0] >= STRU8 ('a') && ris[0] <= STRU8 ('z')) || ris[0] == STRU8 ('_')) ;
			ris++ ;
			ret++ ;
			while ((ris[0] >= STRU8 ('A') && ris[0] <= STRU8 ('Z')) || (ris[0] >= STRU8 ('a') && ris[0] <= STRU8 ('z')) || (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) || ris[0] == STRU8 ('_') || ris[0] == STRU8 ('-') || ris[0] == STRU8 ('.')) {
				ris++ ;
				ret++ ;
			}
			return std::move (ret) ;
		}) ;
		String<STRU8> ret = String<STRU8> (r1x) ;
		read_text (ret) ;
		return std::move (ret) ;
	}

	String<STRU8> read_value () popping {
		const auto r1x = _CALL_ ([&] () {
			LENGTH ret = 0 ;
			LLTextReader ris = clone () ;
			if (ris[0] == STRU8 ('+') || ris[0] == STRU8 ('-')) {
				ris++ ;
				ret++ ;
			}
			if (ris[0] == STRU8 ('0')) {
				ris++ ;
				ret++ ;
			} else if (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
				ris++ ;
				ret++ ;
				while (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
					ris++ ;
					ret++ ;
				}
			} else {
				_DYNAMIC_ASSERT_ (FALSE) ;
			}
			if (ris[0] == STRU8 ('.')) {
				ris++ ;
				ret++ ;
				while (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
					ris++ ;
					ret++ ;
				}
			}
			if (ris[0] == STRU8 ('e') || ris[0] == STRU8 ('E')) {
				ris++ ;
				ret++ ;
				if (ris[0] == STRU8 ('+') || ris[0] == STRU8 ('-')) {
					ris++ ;
					ret++ ;
				}
				_DYNAMIC_ASSERT_ (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) ;
				ris++ ;
				ret++ ;
				while (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
					ris++ ;
					ret++ ;
				}
			}
			return std::move (ret) ;
		}) ;
		String<STRU8> ret = String<STRU8> (r1x) ;
		read_text (ret) ;
		return std::move (ret) ;
	}

	void read_hypertext (String<STRU8> &dst) {
		for (INDEX i = 0 ; i < dst.size () ; i++)
			hypertext_str (dst[i]) ;
	}

	String<STRU8> read_string () popping {
		const auto r1x = _CALL_ ([&] () {
			LENGTH ret = 0 ;
			LLTextReader ris = clone () ;
			auto rax = STRU8 () ;
			ris.shift (_PCSTRU8_ ("\"")) ;
			while (ris[0] != STRU8 ('\0') && ris[0] != STRU8 ('\"')) {
				ris.hypertext_str (rax) ;
				ret++ ;
			}
			ris.shift (_PCSTRU8_ ("\"")) ;
			return std::move (ret) ;
		}) ;
		String<STRU8> ret = String<STRU8> (r1x) ;
		shift () ;
		read_hypertext (ret) ;
		shift () ;
		return std::move (ret) ;
	}

	String<STRU8> read_line () popping {
		const auto r1x = _CALL_ ([&] () {
			LENGTH ret = 0 ;
			LLTextReader ris = clone () ;
			auto rax = STRU8 () ;
			while (!mReader.attr ().varify_space (ris[0] ,2)) {
				ris.hypertext_str (rax) ;
				ret++ ;
			}
			return std::move (ret) ;
		}) ;
		String<STRU8> ret = String<STRU8> (r1x) ;
		read_hypertext (ret) ;
		skip_newline () ;
		return std::move (ret) ;
	}

private:
	void update_reader () {
		auto &r1 = mReader.attr () ;
		r1.modify_space (STRU8 (' ') ,1) ;
		r1.modify_space (STRU8 ('\t') ,1) ;
		r1.modify_space (STRU8 ('\r') ,2) ;
		r1.modify_space (STRU8 ('\n') ,2) ;
		r1.modify_space (STRU8 ('\f') ,2) ;
		r1.modify_escape (STRU8 ('t') ,STRU8 ('\t')) ;
		r1.modify_escape (STRU8 ('r') ,STRU8 ('\r')) ;
		r1.modify_escape (STRU8 ('n') ,STRU8 ('\n')) ;
		r1.modify_escape (STRU8 ('f') ,STRU8 ('\f')) ;
		r1.modify_escape (STRU8 ('\"') ,STRU8 ('\"')) ;
		r1.modify_escape (STRU8 ('\\') ,STRU8 ('\\')) ;
		mReader >> _BOM_ ;
		for (INDEX i = 0 ; i < mCache.length () ; i++)
			mReader.read (mCache[i]) ;
		mWrite = 0 ;
	}

	void hypertext_str (STRU8 &str) {
		str = get (0) ;
		shift () ;
		if (str == mReader.attr ().varify_escape_item ()) {
			str = get (0) ;
			shift () ;
			str = mReader.attr ().convert_escape (str) ;
		} else {
			_DYNAMIC_ASSERT_ (!mReader.attr ().varify_control (str)) ;
		}
	}
} ;

class XmlParser {
private:
	class Pack {
	private:
		friend XmlParser ;
		String<STRU8> mName ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSet ;
		SoftSet<String<STRU8> ,INDEX> mMemberSet ;
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

	LENGTH length () const {
		return exist () ? mHeap.self[mIndex].mMemberSet.length () : 0 ;
	}

	XmlParser root () const {
		INDEX ix = exist () ? 0 : VAR_NONE ;
		return XmlParser (mHeap ,ix) ;
	}

	XmlParser parent () const {
		INDEX ix = exist () ? mHeap.self[mIndex].mParent : VAR_NONE ;
		return XmlParser (mHeap ,ix) ;
	}

	XmlParser brother () const {
		INDEX ix = exist () ? mHeap.self[mIndex].mBrother : VAR_NONE ;
		return XmlParser (mHeap ,ix) ;
	}

	XmlParser brother (const String<STRU8> &name) const {
		for (XmlParser i = brother () ; i.exist () ; i = i.brother ())
			if (i.name () == name)
				return i ;
		return XmlParser (mHeap ,VAR_NONE) ;
	}

	XmlParser child () const {
		INDEX ix = exist () ? mHeap.self[mIndex].mChild : VAR_NONE ;
		return XmlParser (mHeap ,ix) ;
	}

	XmlParser child (const String<STRU8> &name) const {
		for (XmlParser i = child () ; i.exist () ; i = i.brother ())
			if (i.name () == name)
				return i ;
		return XmlParser (mHeap ,VAR_NONE) ;
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
		static const String<STRU8> mNullString ;
		if (!exist ())
			return mNullString ;
		INDEX ix = mHeap.self[mIndex].mAttributeSet.find (tag) ;
		if (ix == VAR_NONE)
			return mNullString ;
		return mHeap.self[mIndex].mAttributeSet[ix].item ;
	}

	const String<STRU8> &value () const {
		_DYNAMIC_ASSERT_ (exist ()) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mMemberSet.size () == 0) ;
		_DYNAMIC_ASSERT_ (mHeap.self[mIndex].mAttributeSet.length () == 1) ;
		INDEX ix = mHeap.self[mIndex].mAttributeSet.ibegin () ;
		return mHeap.self[mIndex].mAttributeSet[ix].item ;
	}

	template <class _RET>
	_RET value (const _RET &def ,const Function<_RET (const String<STRU8> &)> &cvt) const {
		return _CALL_TRY_ ([&] () {
			return cvt (value ()) ;
		} ,[&] () {
			return def ;
		}) ;
	}

	BOOL value (const BOOL &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEBOOLS_<STRU8>)) ;
	}

	PTR<VOID> value (const String<STRU8> & ,const PTR<VOID> &) const = delete ;
	PTR<const VOID> value (const String<STRU8> & ,const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAR32S_<STRU8>)) ;
	}

	VAR64 value (const VAR64 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAR64S_<STRU8>)) ;
	}

	VAL32 value (const VAL32 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAL32S_<STRU8>)) ;
	}

	VAL64 value (const VAL64 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAL64S_<STRU8>)) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		return value<String<STRU8>> (def ,&OPERATOR_CONVERT_TYPE<String<STRU8>>::invoke) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		return value<String<STRU16>> (def ,&OPERATOR_CONVERT_TYPE<String<STRU16>>::invoke) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		return value<String<STRU32>> (def ,&OPERATOR_CONVERT_TYPE<String<STRU32>>::invoke) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		return value<String<STRA>> (def ,&OPERATOR_CONVERT_TYPE<String<STRA>>::invoke) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		return value<String<STRW>> (def ,&OPERATOR_CONVERT_TYPE<String<STRW>>::invoke) ;
	}

	String<STRU8> serialize (LENGTH len) const ;

private:
	explicit XmlParser (const SharedRef<FixedBuffer<Pack>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

	void initialize (const PhanBuffer<const STRU8> &data) ;

public:
	inline static XmlParser make (const PhanBuffer<const STRU8> &data) {
		XmlParser ret ;
		ret.initialize (data) ;
		return std::move (ret) ;
	}

	inline static XmlParser make (const Array<XmlParser> &sequence) ;
} ;

inline String<STRU8> XmlParser::serialize (LENGTH len) const {
	String<STRU8> ret = String<STRU8> (len) ;
	auto wos = TextWriter<STRU8> (ret.raw ()) ;
	auto rax = Stack<ARRAY2<INDEX>> () ;
	rax.add ({mIndex ,VAR_NONE}) ;
	while (!rax.empty ()) {
		const auto r1x = rax.poll () ;
		if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mName.empty ()) {
			//@info:case '<?xml ...>'
			auto &r1 = mHeap.self[r1x[0]] ;
			wos << _PCSTRU8_ ("<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n") ;
			rax.add ({r1.mChild ,VAR_NONE}) ;
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mChild == VAR_NONE) {
			//@info:case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?/>'
			auto &r1 = mHeap.self[r1x[0]] ;
			wos << _PCSTRU8_ ("<") << r1.mName << _PCSTRU8_ (" ") ;
			for (INDEX i = r1.mAttributeSet.ibegin () ,ie = r1.mAttributeSet.iend () ; i != ie ; i = r1.mAttributeSet.inext (i)) {
				wos << r1.mAttributeSet[i].key ;
				wos << _PCSTRU8_ ("=\"") ;
				wos << r1.mAttributeSet[i].item << _PCSTRU8_ ("\" ") ;
			}
			wos << _PCSTRU8_ ("/>") ;
			rax.add ({r1.mBrother ,r1x[1]}) ;
		} else if (r1x[0] != VAR_NONE) {
			//@info:case '<xxx ("xxx"="xxx"( "xxx"="xxx")?)?>'
			auto &r1 = mHeap.self[r1x[0]] ;
			wos << _PCSTRU8_ ("<") << r1.mName << _PCSTRU8_ (" ") ;
			for (INDEX i = r1.mAttributeSet.ibegin () ,ie = r1.mAttributeSet.iend () ; i != ie ; i = r1.mAttributeSet.inext (i)) {
				wos << r1.mAttributeSet[i].key ;
				wos << _PCSTRU8_ ("=\"") ;
				wos << r1.mAttributeSet[i].item << _PCSTRU8_ ("\" ") ;
			}
			wos << _PCSTRU8_ (">") ;
			rax.add ({r1.mBrother ,r1x[1]}) ;
			rax.add ({r1.mChild ,r1x[0]}) ;
		} else if (r1x[1] != VAR_NONE) {
			//@info:case '</xxx>'
			wos << _PCSTRU8_ ("</") << mHeap.self[r1x[1]].mName << _PCSTRU8_ (">") ;
		}
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

//@warn:recursive
inline void XmlParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		class Counter :private Wrapped<LENGTH> {
		public:
			void lock () {
				Counter::mData++ ;
				_DYNAMIC_ASSERT_ (Counter::mData < RECURSIVE_SIZE::value) ;
			}

			void unlock () {
				Counter::mData-- ;
			}
		} ;

	private:
		XmlParser & mContext ;
		TextReader<STRU8> mTextReader ;
		LLTextReader<ARGC<2>> mRis ;
		LENGTH mRecursiveDepth ;
		SoftSet<String<STRU8> ,String<STRU8>> mAttributeSoftSet ;
		SoftSet<String<STRU8> ,INDEX> mMemberSoftSet ;
		Allocator<Pack ,SAUTO> mNodeHeap ;
		INDEX mLast ;
		String<STRU8> mLastString ;
		INDEX mRoot ;

	public:
		inline explicit Lambda (XmlParser &context ,const PhanBuffer<const STRU8> &data) :mContext (context) {
			mTextReader = TextReader<STRU8> (data) ;
		}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = LLTextReader<ARGC<2>> (mTextReader) ;
			mRecursiveDepth = 0 ;
			mAttributeSoftSet = SoftSet<String<STRU8> ,String<STRU8>> (0) ;
			mMemberSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Pack ,SAUTO> () ;
			mRoot = VAR_NONE ;
		}

		inline void generate () {
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
			$9->ε
			*/
			update_shift_e0 () ;
			mRoot = mLast ;
		}

		//@info:$0->$8 $7
		inline void update_shift_e0 () {
			update_shift_e8 () ;
			mRis.skip () ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mMemberSet = mMemberSoftSet ;
			mNodeHeap[ix].mMemberSet.clear () ;
			mNodeHeap[ix].mParent = VAR_NONE ;
			update_shift_e7 (ix) ;
			mNodeHeap[ix].mChild = mLast ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mRis.skip () ;
			update_shift_e9 () ;
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
			mLast = ix ;
		}

		//@info:$1->记号串
		inline void update_shift_e1 () {
			mLastString = mRis.read_identifer () ;
		}

		//@info:$2->"文本串"
		inline void update_shift_e2 () {
			mLastString = mRis.read_string () ;
		}

		//@info:$3->$1 = $2
		inline void update_shift_e3 (INDEX it) {
			update_shift_e1 () ;
			INDEX ix = mNodeHeap[it].mAttributeSet.insert (std::move (mLastString)) ;
			_DYNAMIC_ASSERT_ (mNodeHeap[it].mAttributeSet[ix].item.empty ()) ;
			mRis.skip () ;
			mRis << _PCSTRU8_ ("=") ;
			mRis.skip () ;
			update_shift_e2 () ;
			mNodeHeap[it].mAttributeSet[ix].item = std::move (mLastString) ;
		}

		//@info:$4->ε|$3 $4
		inline void update_shift_e4 (INDEX it) {
			while ((mRis[0] >= STRU8 ('A') && mRis[0] <= STRU8 ('Z')) || (mRis[0] >= STRU8 ('a') && mRis[0] <= STRU8 ('z')) || mRis[0] == STRU8 ('_')) {
				update_shift_e3 (it) ;
				mRis.skip () ;
			}
		}

		//@info:$5-><$1 $4 />|<$1 $4 > $7 </$1 >
		inline void update_shift_e5 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveDepth)) ;
			mRis << _PCSTRU8_ ("<") ;
			INDEX ix = mNodeHeap.alloc () ;
			update_shift_e1 () ;
			mNodeHeap[ix].mName = std::move (mLastString) ;
			mNodeHeap[ix].mAttributeSet = mAttributeSoftSet ;
			mNodeHeap[ix].mAttributeSet.clear () ;
			mNodeHeap[ix].mParent = it ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis.skip () ;
			update_shift_e4 (ix) ;
			mRis.skip () ;
			if (mRis[0] == STRU8 ('>')) {
				mRis++ ;
				mRis.skip () ;
				mNodeHeap[ix].mMemberSet = mMemberSoftSet ;
				mNodeHeap[ix].mMemberSet.clear () ;
				update_shift_e7 (ix) ;
				mNodeHeap[ix].mChild = mLast ;
				mRis.skip () ;
				mRis << _PCSTRU8_ ("</") ;
				update_shift_e1 () ;
				_DYNAMIC_ASSERT_ (mNodeHeap[ix].mName == mLastString) ;
				mRis.skip () ;
				mRis << _PCSTRU8_ (">") ;
			} else {
				mNodeHeap[ix].mMemberSet = SoftSet<String<STRU8> ,INDEX> () ;
				mRis << _PCSTRU8_ ("/>") ;
			}
			mLast = ix ;
		}

		//@info:$6-><!--注释-->
		inline void update_shift_e6 () {
			mRis << _PCSTRU8_ ("<!--") ;
			while (mRis[0] != STRU8 ('\0') && (mRis[0] != STRU8 ('-') || mRis[1] != STRU8 ('-')))
				mRis++ ;
			mRis << _PCSTRU8_ ("-->") ;
		}

		//@info:$7->ε|$5 $7|$6 $7
		inline void update_shift_e7 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveDepth)) ;
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				if (mRis[0] == STRU8 ('<') && mRis[1] == STRU8 ('!')) {
					update_shift_e6 () ;
				} else if (mRis[0] == STRU8 ('<') && mRis[1] != STRU8 ('/')) {
					update_shift_e5 (it) ;
					mNodeHeap[it].mMemberSet.add (mNodeHeap[mLast].mName ,mLast) ;
					(ix == VAR_NONE ? ix : mNodeHeap[iy].mBrother) = mLast ;
					iy = mLast ;
				} else {
					break ;
				}
				mRis.skip () ;
			}
			mLast = ix ;
		}

		//@info:$8->ε|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		inline void update_shift_e8 () {
			if (mRis[0] == STRU8 ('<') && mRis[1] == STRU8 ('?')) {
				mRis++ ;
				mRis++ ;
				mRis << _PCSTRU8_ ("xml") ;
				mRis.skip () ;
				mRis << _PCSTRU8_ ("version") ;
				mRis.skip () ;
				mRis << _PCSTRU8_ ("=") ;
				mRis.skip () ;
				mRis << _PCSTRU8_ ("\"1.0\"") ;
				mRis.skip () ;
				if (mRis[0] != STRU8 ('?')) {
					mRis << _PCSTRU8_ ("encoding") ;
					mRis.skip () ;
					mRis << _PCSTRU8_ ("=") ;
					mRis.skip () ;
					mRis << _PCSTRU8_ ("\"utf-8\"") ;
					mRis.skip () ;
				}
				mRis << _PCSTRU8_ ("?>") ;
			}
		}

		//@info:$9->ε
		inline void update_shift_e9 () {}

		inline void refresh () {
			mAttributeSoftSet.shrink () ;
			mMemberSoftSet.shrink () ;
			mContext.mIndex = mRoot ;
			mContext.mHeap = _CALL_ ([&] () {
				SharedRef<FixedBuffer<Pack>> ret = SharedRef<FixedBuffer<Pack>>::make (mNodeHeap.length ()) ;
				INDEX iw = 0 ;
				for (INDEX i = 0 ; i < mNodeHeap.size () && mNodeHeap.used (i) ; i++)
					ret.self[iw++] = std::move (mNodeHeap[i]) ;
				_DEBUG_ASSERT_ (iw == ret->size ()) ;
				return std::move (ret) ;
			}) ;
		}
	} ;
	Lambda (*this ,data) () ;
}

inline XmlParser XmlParser::make (const Array<XmlParser> &sequence) {
	//@warn:unimplemented
	_DEBUG_ASSERT_ (FALSE) ;
	return _NULL_<XmlParser> () ;
}

class JsonParser {
private:
	class Pack {
	private:
		friend JsonParser ;
		AnyRef<void> mValue ;
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
		return mHeap.self[mIndex].mValue.type () == typeid (String<STRU8>) ;
	}

	BOOL array_type () const {
		return mHeap.self[mIndex].mValue.type () == typeid (SoftSet<INDEX ,INDEX>) ;
	}

	BOOL object_type () const {
		return mHeap.self[mIndex].mValue.type () == typeid (SoftSet<String<STRU8> ,INDEX>) ;
	}

	LENGTH length () const {
		if (!exist ())
			return 0 ;
		if (string_type ())
			return 1 ;
		if (array_type ())
			return mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ()->length () ;
		if (object_type ())
			return mHeap.self[mIndex].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ()->length () ;
		return VAR_NONE ;
	}

	JsonParser root () const {
		INDEX ix = exist () ? 0 : VAR_NONE ;
		return JsonParser (mHeap ,ix) ;
	}

	JsonParser parent () const {
		INDEX ix = exist () ? mHeap.self[mIndex].mParent : VAR_NONE ;
		return JsonParser (mHeap ,ix) ;
	}

	JsonParser brother () const {
		INDEX ix = exist () ? mHeap.self[mIndex].mBrother : VAR_NONE ;
		return JsonParser (mHeap ,ix) ;
	}

	JsonParser child () const {
		INDEX ix = exist () ? mHeap.self[mIndex].mChild : VAR_NONE ;
		return JsonParser (mHeap ,ix) ;
	}

	JsonParser child (INDEX pos) const {
		if (!exist ())
			return *this ;
		INDEX ix = VAR_NONE ;
		if (array_type ()) {
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			if (pos >= 0 && pos < r1.length ())
				ix = r1[r1.find (pos)].item ;
		}
		return JsonParser (mHeap ,ix) ;
	}

	JsonParser child (const String<STRU8> &key) const {
		if (!exist ())
			return *this ;
		INDEX ix = VAR_NONE ;
		if (object_type ()) {
			auto &r1 = mHeap.self[mIndex].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			ix = r1.find (key) ;
			if (ix != VAR_NONE)
				ix = r1[ix].item ;
		}
		return JsonParser (mHeap ,ix) ;
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

	template <class _RET>
	_RET value (const _RET &def ,const Function<_RET (const String<STRU8> &)> &cvt) const {
		return _CALL_TRY_ ([&] () {
			return cvt (value ()) ;
		} ,[&] () {
			return def ;
		}) ;
	}

	BOOL value (const BOOL &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEBOOLS_<STRU8>)) ;
	}

	PTR<VOID> value (const PTR<VOID> &) const = delete ;
	PTR<const VOID> value (const PTR<const VOID> &) const = delete ;

	VAR32 value (const VAR32 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAR32S_<STRU8>)) ;
	}

	VAR64 value (const VAR64 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAR64S_<STRU8>)) ;
	}

	VAL32 value (const VAL32 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAL32S_<STRU8>)) ;
	}

	VAL64 value (const VAL64 &def) const {
		return value (def ,_DEDUCE_<Function> (&_PARSEVAL64S_<STRU8>)) ;
	}

	String<STRU8> value (const String<STRU8> &def) const {
		return value<String<STRU8>> (def ,&OPERATOR_CONVERT_TYPE<String<STRU8>>::invoke) ;
	}

	String<STRU16> value (const String<STRU16> &def) const {
		return value<String<STRU16>> (def ,&OPERATOR_CONVERT_TYPE<String<STRU16>>::invoke) ;
	}

	String<STRU32> value (const String<STRU32> &def) const {
		return value<String<STRU32>> (def ,&OPERATOR_CONVERT_TYPE<String<STRU32>>::invoke) ;
	}

	String<STRA> value (const String<STRA> &def) const {
		return value<String<STRA>> (def ,&OPERATOR_CONVERT_TYPE<String<STRA>>::invoke) ;
	}

	String<STRW> value (const String<STRW> &def) const {
		return value<String<STRW>> (def ,&OPERATOR_CONVERT_TYPE<String<STRW>>::invoke) ;
	}

	String<STRU8> serialize (LENGTH len) const ;

private:
	explicit JsonParser (const SharedRef<FixedBuffer<Pack>> &heap ,INDEX index) :mHeap (heap) ,mIndex (index) {}

	void initialize (const PhanBuffer<const STRU8> &data) ;

public:
	inline static JsonParser make (const PhanBuffer<const STRU8> &data) {
		JsonParser ret ;
		ret.initialize (data) ;
		return std::move (ret) ;
	}
} ;

inline String<STRU8> JsonParser::serialize (LENGTH len) const {
	String<STRU8> ret = String<STRU8> (len) ;
	auto wos = TextWriter<STRU8> (ret.raw ()) ;
	auto rax = Stack<ARRAY3<INDEX>> () ;
	rax.add ({mIndex ,VAR_NONE ,VAR_NONE}) ;
	while (!rax.empty ()) {
		const auto r1x = rax.poll () ;
		if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mValue.type () == typeid (String<STRU8>)) {
			//@info:case '"xxx"'
			auto &r1 = mHeap.self[r1x[0]].mValue.rebind<String<STRU8>> ().self ;
			wos << _PCSTRU8_ ("\"") ;
			wos << r1 ;
			wos << _PCSTRU8_ ("\"") ;
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mValue.type () == typeid (SoftSet<INDEX ,INDEX>)) {
			//@info:case '[(@(,@)*)?]'
			auto &r1 = mHeap.self[r1x[0]].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
			if (r1x[1] == VAR_NONE && r1x[2] == VAR_NONE) {
				wos << _PCSTRU8_ ("[") ;
				rax.add ({r1x[0] ,r1.ibegin () ,VAR (0)}) ;
			} else if (r1x[1] != r1.iend ()) {
				if (r1x[2] == VAR (1))
					wos << _PCSTRU8_ (",") ;
				rax.add ({r1x[0] ,r1.inext (r1x[1]) ,VAR (1)}) ;
				rax.add ({r1[r1x[1]].item ,VAR_NONE ,VAR_NONE}) ;
			} else {
				wos << _PCSTRU8_ ("]") ;
			}
		} else if (r1x[0] != VAR_NONE && mHeap.self[r1x[0]].mValue.type () == typeid (SoftSet<String<STRU8> ,INDEX>)) {
			//@info:case '{("xxx":@(,"xxx":@)*)?}'
			auto &r1 = mHeap.self[r1x[0]].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			if (r1x[1] == VAR_NONE && r1x[2] == VAR_NONE) {
				wos << _PCSTRU8_ ("{") ;
				rax.add ({r1x[0] ,r1.ibegin () ,VAR (0)}) ;
			} else if (r1x[1] != r1.iend ()) {
				if (r1x[2] == VAR (1))
					wos << _PCSTRU8_ (",") ;
				wos << _PCSTRU8_ ("\"") ;
				wos << r1[r1x[1]].key ;
				wos << _PCSTRU8_ ("\"") ;
				wos << _PCSTRU8_ (":") ;
				rax.add ({r1x[0] ,r1.inext (r1x[1]) ,VAR (1)}) ;
				rax.add ({r1[r1x[1]].item ,VAR_NONE ,VAR_NONE}) ;
			} else {
				wos << _PCSTRU8_ ("}") ;
			}
		}
	}
	wos << _EOS_ ;
	return std::move (ret) ;
}

//@warn:recursive
inline void JsonParser::initialize (const PhanBuffer<const STRU8> &data) {
	class Lambda {
	private:
		class Counter :private Wrapped<LENGTH> {
		public:
			void lock () {
				Counter::mData++ ;
				_DYNAMIC_ASSERT_ (Counter::mData < RECURSIVE_SIZE::value) ;
			}

			void unlock () {
				Counter::mData-- ;
			}
		} ;

	private:
		JsonParser & mContext ;
		TextReader<STRU8> mTextReader ;
		LLTextReader<ARGC<2>> mRis ;
		LENGTH mRecursiveDepth ;
		SoftSet<INDEX ,INDEX> mArraySoftSet ;
		SoftSet<String<STRU8> ,INDEX> mObjectSoftSet ;
		Allocator<Pack ,SAUTO> mNodeHeap ;
		INDEX mLast ;
		String<STRU8> mLastString ;
		INDEX mRoot ;

	public:
		inline explicit Lambda (JsonParser &context ,const PhanBuffer<const STRU8> &data) :mContext (context) {
			mTextReader = TextReader<STRU8> (data) ;
		}

		inline void operator() () {
			prepare () ;
			generate () ;
			refresh () ;
		}

	private:
		inline void prepare () {
			mRis = LLTextReader<ARGC<2>> (mTextReader) ;
			mRecursiveDepth = 0 ;
			mArraySoftSet = SoftSet<INDEX ,INDEX> (0) ;
			mObjectSoftSet = SoftSet<String<STRU8> ,INDEX> (0) ;
			mNodeHeap = Allocator<Pack ,SAUTO> () ;
			mRoot = VAR_NONE ;
		}

		inline void generate () {
			/*
			$0->$11 $10 $11
			$1->数值串
			$2->"文本串"
			$3->true|false|null
			$4->$1|$2|$3|$6|$9
			$5->$4|$4 , $5
			$6->[ ]|[ $5 ]
			$7->$2 : $4
			$8->$7|$7 , $8
			$9->{ }|{ $8 }
			$10->ε|$4
			$11->ε
			*/
			update_shift_e0 () ;
			mRoot = mLast ;
		}

		//@info:$0->$11 $10 $11
		inline void update_shift_e0 () {
			update_shift_e11 () ;
			mRis.skip () ;
			update_shift_e10 () ;
			INDEX ix = mLast ;
			mRis.skip () ;
			update_shift_e11 () ;
			_DYNAMIC_ASSERT_ (mRis[0] == STRU8 ('\0')) ;
			mLast = ix ;
		}

		//@info:$1->数值串
		inline void update_shift_e1 () {
			mLastString = mRis.read_value () ;
		}

		//@info:$2->"文本串"
		inline void update_shift_e2 () {
			mLastString = mRis.read_string () ;
		}

		//@info:$3->true|false|null
		inline void update_shift_e3 () {
			if (mRis[0] == STRU8 ('t')) {
				mRis << _PCSTRU8_ ("true") ;
				mLastString = _PCSTRU8_ ("true") ;
			} else if (mRis[0] == STRU8 ('f')) {
				mRis << _PCSTRU8_ ("false") ;
				mLastString = _PCSTRU8_ ("true") ;
			} else {
				mRis << _PCSTRU8_ ("null") ;
				mLastString = String<STRU8> () ;
			}
		}

		//@info:$4->$1|$2|$3|$6|$9
		inline void update_shift_e4 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveDepth)) ;
			INDEX ix = VAR_NONE ;
			if (mRis[0] == STRU8 ('+') || mRis[0] == STRU8 ('-') || (mRis[0] >= STRU8 ('0') && mRis[0] <= STRU8 ('9'))) {
				ix = mNodeHeap.alloc () ;
				update_shift_e1 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLastString)) ;
				mNodeHeap[ix].mParent = it ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			} else if (mRis[0] == STRU8 ('\"')) {
				ix = mNodeHeap.alloc () ;
				update_shift_e2 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLastString)) ;
				mNodeHeap[ix].mParent = it ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			} else if (mRis[0] == STRU8 ('[')) {
				update_shift_e6 (it) ;
				ix = mLast ;
			} else if (mRis[0] == STRU8 ('{')) {
				update_shift_e9 (it) ;
				ix = mLast ;
			} else {
				ix = mNodeHeap.alloc () ;
				update_shift_e3 () ;
				mNodeHeap[ix].mValue = AnyRef<String<STRU8>>::make (std::move (mLastString)) ;
				mNodeHeap[ix].mParent = it ;
				mNodeHeap[ix].mBrother = VAR_NONE ;
				mNodeHeap[ix].mChild = VAR_NONE ;
			}
			mLast = ix ;
		}

		//@info:$5->$4|$4 , $5
		inline void update_shift_e5 (INDEX it) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e4 (it) ;
				auto &r1 = mNodeHeap[it].mValue.rebind<SoftSet<INDEX ,INDEX>> ().self ;
				r1.add (r1.length () ,mLast) ;
				(ix == VAR_NONE ? ix : mNodeHeap[iy].mBrother) = mLast ;
				iy = mLast ;
				mRis.skip () ;
				if (mRis[0] != STRU8 (','))
					break ;
				mRis++ ;
				mRis.skip () ;
			}
			mLast = ix ;
		}

		//@info:$6->[ ]|[ $5 ]
		inline void update_shift_e6 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveDepth)) ;
			mRis << _PCSTRU8_ ("[") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<INDEX ,INDEX>>::make (mArraySoftSet) ;
			mNodeHeap[ix].mValue.rebind<SoftSet<INDEX ,INDEX>> ()->clear () ;
			mNodeHeap[ix].mParent = it ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis.skip () ;
			if (mRis[0] != STRU8 (']')) {
				update_shift_e5 (ix) ;
				mNodeHeap[ix].mChild = mLast ;
				mRis.skip () ;
			}
			mRis << _PCSTRU8_ ("]") ;
			mLast = ix ;
		}

		//@info:$7->$2 : $4
		inline void update_shift_e7 (INDEX it) {
			update_shift_e2 () ;
			auto &r1 = mNodeHeap[it].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ().self ;
			INDEX ix = r1.insert (std::move (mLastString)) ;
			mRis.skip () ;
			mRis << _PCSTRU8_ (":") ;
			mRis.skip () ;
			update_shift_e4 (it) ;
			r1[ix].item = mLast ;
		}

		//@info:$8->$7|$7 , $8
		inline void update_shift_e8 (INDEX it) {
			INDEX ix = VAR_NONE ;
			INDEX iy = VAR_NONE ;
			while (TRUE) {
				update_shift_e7 (it) ;
				(ix == VAR_NONE ? ix : mNodeHeap[iy].mBrother) = mLast ;
				iy = mLast ;
				mRis.skip () ;
				if (mRis[0] != STRU8 (','))
					break ;
				mRis++ ;
				mRis.skip () ;
			}
			mLast = ix ;
		}

		//@info:$9->{ }|{ $8 }
		inline void update_shift_e9 (INDEX it) {
			ScopedGuard<Counter> ANONYMOUS (_CAST_<Counter> (mRecursiveDepth)) ;
			mRis << _PCSTRU8_ ("{") ;
			INDEX ix = mNodeHeap.alloc () ;
			mNodeHeap[ix].mValue = AnyRef<SoftSet<String<STRU8> ,INDEX>>::make (mObjectSoftSet) ;
			mNodeHeap[ix].mValue.rebind<SoftSet<String<STRU8> ,INDEX>> ()->clear () ;
			mNodeHeap[ix].mParent = it ;
			mNodeHeap[ix].mBrother = VAR_NONE ;
			mNodeHeap[ix].mChild = VAR_NONE ;
			mRis.skip () ;
			if (mRis[0] != STRU8 ('}')) {
				update_shift_e8 (ix) ; ;
				mNodeHeap[ix].mChild = mLast ;
				mRis.skip () ;
			}
			mRis << _PCSTRU8_ ("}") ;
			mLast = ix ;
		}

		//@info:$10->ε|$4
		inline void update_shift_e10 () {
			INDEX ix = VAR_NONE ;
			if (mRis[0] != STRU8 ('\0')) {
				update_shift_e4 (VAR_NONE) ;
				ix = mLast ;
			}
			mLast = ix ;
		}

		//@info:$11->ε
		inline void update_shift_e11 () {}

		inline void refresh () {
			mArraySoftSet.shrink () ;
			mObjectSoftSet.shrink () ;
			mContext.mIndex = mRoot ;
			mContext.mHeap = _CALL_ ([&] () {
				SharedRef<FixedBuffer<Pack>> ret = SharedRef<FixedBuffer<Pack>>::make (mNodeHeap.length ()) ;
				INDEX iw = 0 ;
				for (INDEX i = 0 ; i < mNodeHeap.size () && mNodeHeap.used (i) ; i++)
					ret.self[iw++] = std::move (mNodeHeap[i]) ;
				_DEBUG_ASSERT_ (iw == ret->size ()) ;
				return std::move (ret) ;
			}) ;
		}
	} ;
	Lambda (*this ,data) () ;
}

#ifdef __CSC_DEPRECATED__
class CommandParser {
public:
	CommandParser () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;
#endif
} ;