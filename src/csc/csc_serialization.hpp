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
namespace SERIALIZATION {
template <class...>
trait RECURSIVECOUNTER_HELP ;

template <class DEPEND>
trait RECURSIVECOUNTER_HELP<DEPEND ,ALWAYS> {
	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class RecursiveCounter extend Proxy {
	private:
		LENGTH mBase ;

	public:
		imports VREF<RecursiveCounter> from (VREF<LENGTH> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<RecursiveCounter>>::id] (unsafe_deptr (that))) ;
		}

		void enter () {
			assume (mBase < COUNTER_MAX_DEPTH::value) ;
			mBase++ ;
		}

		void leave () {
			mBase-- ;
		}
	} ;
} ;

using RecursiveCounter = typename RECURSIVECOUNTER_HELP<DEPEND ,ALWAYS>::RecursiveCounter ;

template <class...>
trait XMLPARSER_HELP ;

template <class...>
trait XMLPARSER_SERIALIZATION_HELP ;

template <class DEPEND>
trait XMLPARSER_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		String<STRU8> mName ;
		SoftSet<String<STRU8>> mAttributeSet ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

	struct HEAP {
		Array<NODE> mList ;
		Array<String<STRU8>> mAttribute ;
	} ;

	class XmlParser {
	private:
		CRef<HEAP> mHeap ;
		INDEX mIndex ;

	public:
		implicit XmlParser () {
			mIndex = NONE ;
		}

		explicit XmlParser (CREF<CRef<HEAP>> heap ,CREF<INDEX> index) {
			mHeap = heap ;
			mIndex = index ;
		}

		imports XmlParser make (CREF<RegBuffer<STRU8>> item) {
			using R1X = typename XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS>::Serialization ;
			XmlParser ret ;
			auto rax = R1X (CRef<RegBuffer<STRU8>>::reference (item)) ;
			rax.generate () ;
			ret.mHeap = CRef<HEAP>::make (rax.poll_heap ()) ;
			ret.mIndex = 0 ;
			return move (ret) ;
		}

		imports XmlParser make (CREF<Array<XmlParser>> sequence) {
			XmlParser ret ;
			unimplemented () ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mHeap == NULL)
				return FALSE ;
			if (mIndex == NONE)
				return FALSE ;
			return TRUE ;
		}

		XmlParser root () const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			return XmlParser (mHeap ,0) ;
		}

		XmlParser parent () const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			return XmlParser (mHeap ,mHeap->mList[mIndex].mParent) ;
		}

		XmlParser brother () const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			return XmlParser (mHeap ,mHeap->mList[mIndex].mBrother) ;
		}

		XmlParser child () const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			return XmlParser (mHeap ,mHeap->mList[mIndex].mChild) ;
		}

		XmlParser child (CREF<String<STRU8>> name) const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			INDEX ix = mHeap->mList[mIndex].mObjectSet.map (name) ;
			return XmlParser (mHeap ,ix) ;
		}

		Array<XmlParser> child_array () const {
			Array<XmlParser> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				const auto r1x = Array<INDEX>::make (mHeap->mList[mIndex].mArraySet.iter ()) ;
				ret = Array<XmlParser> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = XmlParser (mHeap ,mHeap->mList[mIndex].mArraySet.get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<XmlParser> child_array (CREF<LENGTH> size_) const {
			auto rax = ArrayList<XmlParser> (size_) ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				for (auto &&i : mHeap->mList[mIndex].mArraySet.iter ()) {
					if (rax.length () >= size_)
						break ;
					INDEX ix = rax.insert () ;
					rax[ix] = XmlParser (mHeap ,mHeap->mList[mIndex].mArraySet.get (i)) ;
				}
			}
			const auto r1x = Array<INDEX>::make (rax.iter ()) ;
			Array<XmlParser> ret = Array<XmlParser> (size_) ;
			for (auto &&i : r1x.iter ())
				ret[i] = move (rax[r1x[i]]) ;
			return move (ret) ;
		}

		BOOL equal (CREF<XmlParser> that) const {
			if (exist () != that.exist ())
				return FALSE ;
			if ifnot (exist ())
				return TRUE ;
			if (address (mHeap->mList) != address (that.mHeap->mList))
				return FALSE ;
			if (mIndex != that.mIndex)
				return FALSE ;
			return TRUE ;
		}

		inline BOOL operator== (CREF<XmlParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<XmlParser> that) const {
			return ifnot (equal (that)) ;
		}

		CREF<String<STRU8>> name () const leftvalue {
			assume (exist ()) ;
			return mHeap->mList[mIndex].mName ;
		}

		CREF<String<STRU8>> attribute (CREF<String<STRU8>> tag) const leftvalue {
			if ifnot (exist ())
				return String<STRU8>::zero () ;
			INDEX ix = mHeap->mList[mIndex].mAttributeSet.map (tag) ;
			if (ix == NONE)
				return String<STRU8>::zero () ;
			return mHeap->mAttribute[ix] ;
		}

		template <class ARG1 ,class ARG2>
		REMOVE_ALL<ARG1> attribute (CREF<String<STRU8>> tag ,XREF<ARG1> def ,XREF<ARG2> cvt) const {
			using R1X = REMOVE_ALL<ARG1> ;
			auto rax = Box<R1X> () ;
			try_invoke ([&] () {
				rax = Box<R1X>::make (cvt (attribute (tag))) ;
			} ,[&] () {
				rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (def)) ;
			}) ;
			return move (rax.self) ;
		}

		BOOL attribute (CREF<String<STRU8>> tag ,CREF<BOOL> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<BOOL ,STRU8>>::id]) ;
		}

		csc_pointer_t attribute (CREF<String<STRU8>> ,CREF<csc_pointer_t>) const = delete ;

		VAL32 attribute (CREF<String<STRU8>> tag ,CREF<VAL32> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<VAL32 ,STRU8>>::id]) ;
		}

		VAL64 attribute (CREF<String<STRU8>> tag ,CREF<VAL64> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id]) ;
		}

		SINGLE attribute (CREF<String<STRU8>> tag ,CREF<SINGLE> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<SINGLE ,STRU8>>::id]) ;
		}

		DOUBLE attribute (CREF<String<STRU8>> tag ,CREF<DOUBLE> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<DOUBLE ,STRU8>>::id]) ;
		}

		BYTE attribute (CREF<String<STRU8>> tag ,CREF<BYTE> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<BYTE ,STRU8>>::id]) ;
		}

		WORD attribute (CREF<String<STRU8>> tag ,CREF<WORD> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<WORD ,STRU8>>::id]) ;
		}

		CHAR attribute (CREF<String<STRU8>> tag ,CREF<CHAR> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<CHAR ,STRU8>>::id]) ;
		}

		DATA attribute (CREF<String<STRU8>> tag ,CREF<DATA> def) const {
			return attribute (tag ,def ,string_parse[TYPEAS<TYPEAS<DATA ,STRU8>>::id]) ;
		}

		String<STRA> attribute (CREF<String<STRU8>> tag ,CREF<String<STRA>> def) const {
			return attribute (tag ,def ,string_cvt[TYPEAS<TYPEAS<STRA ,STRU8>>::id]) ;
		}

		String<STRW> attribute (CREF<String<STRU8>> tag ,CREF<String<STRW>> def) const {
			return attribute (tag ,def ,string_cvt[TYPEAS<TYPEAS<STRW ,STRU8>>::id]) ;
		}

		String<STRU8> attribute (CREF<String<STRU8>> tag ,CREF<String<STRU8>> def) const {
			return attribute (tag ,def ,string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU8>>::id]) ;
		}

		String<STRU16> attribute (CREF<String<STRU8>> tag ,CREF<String<STRU16>> def) const {
			return attribute (tag ,def ,string_cvt[TYPEAS<TYPEAS<STRU16 ,STRU8>>::id]) ;
		}

		String<STRU32> attribute (CREF<String<STRU8>> tag ,CREF<String<STRU32>> def) const {
			return attribute (tag ,def ,string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::id]) ;
		}

		CREF<String<STRU8>> value () const leftvalue {
			assume (exist ()) ;
			assume (mHeap->mList[mIndex].mArraySet.size () == 0) ;
			assume (mHeap->mList[mIndex].mObjectSet.size () == 0) ;
			assume (mHeap->mList[mIndex].mAttributeSet.length () == 1) ;
			INDEX ix = mHeap->mAttribute.ibegin () ;
			return mHeap->mAttribute[ix] ;
		}

		template <class ARG1 ,class ARG2>
		REMOVE_ALL<ARG1> value (XREF<ARG1> def ,XREF<ARG2> cvt) const {
			using R1X = REMOVE_ALL<ARG1> ;
			auto rax = Box<R1X> () ;
			try_invoke ([&] () {
				rax = Box<R1X>::make (cvt (value ())) ;
			} ,[&] () {
				rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (def)) ;
			}) ;
			return move (rax.self) ;
		}

		BOOL value (CREF<BOOL> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<BOOL ,STRU8>>::id]) ;
		}

		csc_pointer_t value (CREF<csc_pointer_t>) const = delete ;

		VAL32 value (CREF<VAL32> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<VAL32 ,STRU8>>::id]) ;
		}

		VAL64 value (CREF<VAL64> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id]) ;
		}

		SINGLE value (CREF<SINGLE> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<SINGLE ,STRU8>>::id]) ;
		}

		DOUBLE value (CREF<DOUBLE> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<DOUBLE ,STRU8>>::id]) ;
		}

		BYTE value (CREF<BYTE> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<BYTE ,STRU8>>::id]) ;
		}

		WORD value (CREF<WORD> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<WORD ,STRU8>>::id]) ;
		}

		CHAR value (CREF<CHAR> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<CHAR ,STRU8>>::id]) ;
		}

		DATA value (CREF<DATA> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<DATA ,STRU8>>::id]) ;
		}

		String<STRA> value (CREF<String<STRA>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRA ,STRU8>>::id]) ;
		}

		String<STRW> value (CREF<String<STRW>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRW ,STRU8>>::id]) ;
		}

		String<STRU8> value (CREF<String<STRU8>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU8>>::id]) ;
		}

		String<STRU16> value (CREF<String<STRU16>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRU16 ,STRU8>>::id]) ;
		}

		String<STRU32> value (CREF<String<STRU32>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::id]) ;
		}
	} ;
} ;

template <class DEPEND>
trait XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;

	class Serialization {
	private:
		LENGTH mRecursiveCounter ;
		TextReader<STRU8> mTextReader ;
		RegularReader mReader ;
		ArrayList<NODE> mList ;
		ArrayList<String<STRU8>> mAttribute ;
		SoftSet<String<STRU8>> mAttributeSet ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mLastIndex ;
		String<STRU8> mLastString ;

	public:
		implicit Serialization () = delete ;

		explicit Serialization (RREF<CRef<RegBuffer<STRU8>>> stream) {
			mRecursiveCounter = 0 ;
			mTextReader = TextReader<STRU8> (move (stream)) ;
			mReader = RegularReader (VRef<TextReader<STRU8>>::reference (mTextReader) ,3) ;
		}

		HEAP poll_heap () {
			HEAP ret ;
			const auto r1x = shrink_order () ;
			ret.mList = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mList.iter ()) {
				INDEX ix = r1x[i] ;
				ret.mList[ix].mName = move (mList[i].mName) ;
				ret.mList[ix].mAttributeSet = move (mList[i].mAttributeSet) ;
				ret.mList[ix].mArraySet = move (mList[i].mArraySet) ;
				for (auto &&j : ret.mList[ix].mArraySet.iter ()) {
					INDEX iy = r1x[ret.mList[ix].mArraySet.get (j)] ;
					ret.mList[ix].mArraySet.set (j ,iy) ;
				}
				ret.mList[ix].mObjectSet = move (mList[i].mObjectSet) ;
				for (auto &&j : ret.mList[ix].mObjectSet.iter ()) {
					INDEX iy = r1x[ret.mList[ix].mObjectSet.get (j)] ;
					ret.mList[ix].mObjectSet.set (j ,iy) ;
				}
				ret.mList[ix].mParent = r1x[mList[i].mParent] ;
				ret.mList[ix].mBrother = r1x[mList[i].mBrother] ;
				ret.mList[ix].mChild = r1x[mList[i].mChild] ;
			}
			ret.mAttribute = Array<String<STRU8>> (mAttribute.length ()) ;
			for (auto &&i : mAttribute.iter ()) {
				assume (vbetween (i ,0 ,ret.mAttribute.length ())) ;
				ret.mAttribute[i] = move (mAttribute[i]) ;
			}
			return move (ret) ;
		}

		Array<INDEX> shrink_order () const {
			Array<INDEX> ret = Array<INDEX> (mList.size ()) ;
			ret.fill (NONE) ;
			INDEX ix = 0 ;
			for (auto &&i : mList.iter ()) {
				ret[i] = ix ;
				ix++ ;
			}
			return move (ret) ;
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
			assume (mLastIndex == 0) ;
		}

		void update_shift_e0 () {
			update_shift_e8 () ;
			mReader >> RegularReader::SKIP_GAP ;
			INDEX ix = mList.insert () ;
			mList[ix].mArraySet = mArraySet.share () ;
			mList[ix].mObjectSet = mObjectSet.share () ;
			mList[ix].mParent = NONE ;
			update_shift_e7 (ix) ;
			mList[ix].mChild = mLastIndex ;
			mList[ix].mBrother = NONE ;
			mReader >> RegularReader::SKIP_GAP ;
			update_shift_e9 () ;
			mLastIndex = ix ;
		}

		//@info: $1->${identity}
		void update_shift_e1 () {
			mReader >> RegularReader::HINT_IDENTIFIER >> mLastString ;
		}

		//@info: $2->"${string}"
		void update_shift_e2 () {
			mReader >> RegularReader::HINT_STRING >> mLastString ;
		}

		//@info: $3->$1 = $2
		void update_shift_e3 (CREF<INDEX> curr) {
			update_shift_e1 () ;
			INDEX ix = mList[curr].mAttributeSet.map (mLastString) ;
			assume (ix == NONE) ;
			ix = mAttribute.insert () ;
			mList[curr].mAttributeSet.add (move (mLastString) ,ix) ;
			mReader >> RegularReader::SKIP_GAP ;
			mReader >> slice ("=") ;
			mReader >> RegularReader::SKIP_GAP ;
			update_shift_e2 () ;
			mAttribute[ix] = move (mLastString) ;
		}

		//@info: $4->${eps}|$3 $4
		void update_shift_e4 (CREF<INDEX> curr) {
			while (TRUE) {
				if ifnot (is_frist_identity ())
					break ;
				update_shift_e3 (curr) ;
				mReader >> RegularReader::SKIP_GAP ;
			}
		}

		BOOL is_frist_identity () const {
			if (mReader[0] >= STRU8 ('A'))
				if (mReader[0] <= STRU8 ('Z'))
					return TRUE ;
			if (mReader[0] >= STRU8 ('a'))
				if (mReader[0] <= STRU8 ('z'))
					return TRUE ;
			if (mReader[0] == STRU8 ('_'))
				return TRUE ;
			return FALSE ;
		}

		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		void update_shift_e5 (CREF<INDEX> curr) {
			Scope<VREF<RecursiveCounter>> anonymous (RecursiveCounter::from (mRecursiveCounter)) ;
			mReader >> slice ("<") ;
			INDEX ix = mList.insert () ;
			update_shift_e1 () ;
			mList[ix].mName = move (mLastString) ;
			mList[ix].mAttributeSet = mAttributeSet.share () ;
			mList[ix].mParent = curr ;
			mList[ix].mBrother = NONE ;
			mList[ix].mChild = NONE ;
			mReader >> RegularReader::SKIP_GAP ;
			update_shift_e4 (ix) ;
			mReader >> RegularReader::SKIP_GAP ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mReader[0] != STRU8 ('>'))
					discard ;
				mReader++ ;
				mReader >> RegularReader::SKIP_GAP ;
				mList[ix].mArraySet = mArraySet.share () ;
				mList[ix].mObjectSet = mObjectSet.share () ;
				update_shift_e7 (ix) ;
				mList[ix].mChild = mLastIndex ;
				mReader >> RegularReader::SKIP_GAP ;
				mReader >> slice ("</") ;
				update_shift_e1 () ;
				assume (mList[ix].mName == mLastString) ;
				mReader >> RegularReader::SKIP_GAP ;
				mReader >> slice (">") ;
			}
			if ifswitch (eax) {
				mReader >> slice ("/>") ;
			}
			mLastIndex = ix ;
		}

		//@info: $6-><!--${comment}-->
		void update_shift_e6 () {
			mReader >> slice ("<!--") ;
			while (TRUE) {
				if (mReader[0] == STRU8 ('\0'))
					break ;
				if (mReader[0] == STRU8 ('-'))
					if (mReader[1] == STRU8 ('-'))
						if (mReader[2] == STRU8 ('>'))
							break ;
				mReader++ ;
			}
			mReader >> slice ("-->") ;
		}

		//@info: $7->${eps}|$5 $7|$6 $7
		void update_shift_e7 (CREF<INDEX> curr) {
			Scope<VREF<RecursiveCounter>> anonymous (RecursiveCounter::from (mRecursiveCounter)) ;
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				const auto r1x = is_first_comment () ;
				const auto r2x = is_first_end_of_node () ;
				if ifnot (r1x)
					if ifnot (r2x)
						break ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (r1x)
						discard ;
					update_shift_e6 () ;
				}
				if ifswitch (eax) {
					if ifnot (r2x)
						discard ;
					update_shift_e5 (curr) ;
					const auto r3x = mList[curr].mArraySet.length () ;
					mList[curr].mArraySet.add (r3x ,mLastIndex) ;
					mList[curr].mObjectSet.add (mList[mLastIndex].mName ,mLastIndex) ;
					auto ebx = TRUE ;
					if ifswitch (ebx) {
						if ifnot (ix == NONE)
							discard ;
						ix = mLastIndex ;
					}
					if ifswitch (ebx) {
						mList[iy].mBrother = mLastIndex ;
					}
					iy = mLastIndex ;
				}
				mReader >> RegularReader::SKIP_GAP ;
			}
			mLastIndex = ix ;
		}

		BOOL is_first_comment () const {
			if (mReader[0] == STRU8 ('<'))
				if (mReader[1] == STRU8 ('!'))
					return TRUE ;
			return FALSE ;
		}

		BOOL is_first_end_of_node () const {
			if (mReader[0] == STRU8 ('<'))
				if (mReader[1] == STRU8 ('/'))
					return TRUE ;
			return FALSE ;
		}

		//@info: $8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		void update_shift_e8 () {
			if (mReader[0] != STRU8 ('<'))
				return ;
			if (mReader[1] != STRU8 ('?'))
				return ;
			mReader++ ;
			mReader++ ;
			mReader >> slice ("xml") ;
			mReader >> RegularReader::SKIP_GAP ;
			mReader >> slice ("version") ;
			mReader >> RegularReader::SKIP_GAP ;
			mReader >> slice ("=") ;
			mReader >> RegularReader::SKIP_GAP ;
			mReader >> slice ("\"1.0\"") ;
			mReader >> RegularReader::SKIP_GAP ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('?'))
					discard ;
				mReader >> slice ("encoding") ;
				mReader >> RegularReader::SKIP_GAP ;
				mReader >> slice ("=") ;
				mReader >> RegularReader::SKIP_GAP ;
				mReader >> slice ("\"utf-8\"") ;
				mReader >> RegularReader::SKIP_GAP ;
			}
			mReader >> slice ("?>") ;
		}

		//@info: $9->${end}
		void update_shift_e9 () {
			assume (mReader[0] == STRU8 ('\0')) ;
		}
	} ;
} ;

using XmlParser = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::XmlParser ;

template <class...>
trait JSONPARSER_HELP ;

template <class...>
trait JSONPARSER_SERIALIZATION_HELP ;

template <class DEPEND>
trait JSONPARSER_HELP<DEPEND ,ALWAYS> {
	struct NODE {
		String<STRU8> mValue ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		FLAG mClazz ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

	struct HEAP {
		Array<NODE> mList ;
	} ;

	using NODE_CLAZZ_NULL = RANK0 ;
	using NODE_CLAZZ_STRING = RANK1 ;
	using NODE_CLAZZ_ARRAY = RANK2 ;
	using NODE_CLAZZ_OBJECT = RANK3 ;

	class JsonParser {
	private:
		CRef<HEAP> mHeap ;
		INDEX mIndex ;

	public:
		implicit JsonParser () {
			mIndex = NONE ;
		}

		explicit JsonParser (CREF<CRef<HEAP>> heap ,CREF<INDEX> index) {
			mHeap = heap ;
			mIndex = index ;
		}

		imports JsonParser make (CREF<RegBuffer<STRU8>> item) {
			using R1X = typename JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS>::Serialization ;
			JsonParser ret ;
			auto rax = R1X (CRef<RegBuffer<STRU8>>::reference (item)) ;
			rax.generate () ;
			ret.mHeap = CRef<HEAP>::make (rax.poll_heap ()) ;
			ret.mIndex = 0 ;
			return move (ret) ;
		}

		BOOL exist () const {
			if (mHeap == NULL)
				return FALSE ;
			if (mIndex == NONE)
				return FALSE ;
			if (mHeap->mList[mIndex].mClazz == NODE_CLAZZ_NULL::value)
				return FALSE ;
			return TRUE ;
		}

		BOOL string_type () const {
			if (mHeap->mList[mIndex].mClazz != NODE_CLAZZ_STRING::value)
				return FALSE ;
			return TRUE ;
		}

		BOOL array_type () const {
			if (mHeap->mList[mIndex].mClazz != NODE_CLAZZ_ARRAY::value)
				return FALSE ;
			return TRUE ;
		}

		BOOL object_type () const {
			if (mHeap->mList[mIndex].mClazz != NODE_CLAZZ_OBJECT::value)
				return FALSE ;
			return TRUE ;
		}

		JsonParser root () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,0) ;
		}

		JsonParser parent () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,mHeap->mList[mIndex].mParent) ;
		}

		JsonParser brother () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,mHeap->mList[mIndex].mBrother) ;
		}

		JsonParser child () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,mHeap->mList[mIndex].mChild) ;
		}

		JsonParser child (CREF<String<STRU8>> name) const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			if ifnot (object_type ())
				return JsonParser (mHeap ,NONE) ;
			INDEX ix = mHeap->mList[mIndex].mObjectSet.map (name) ;
			return JsonParser (mHeap ,ix) ;
		}

		Array<JsonParser> child_array () const {
			Array<JsonParser> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				if ifnot (array_type ())
					discard ;
				const auto r1x = Array<INDEX>::make (mHeap->mList[mIndex].mArraySet.iter ()) ;
				ret = Array<JsonParser> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = JsonParser (mHeap ,mHeap->mList[mIndex].mArraySet.get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<JsonParser> child_array (CREF<LENGTH> size_) const {
			auto rax = ArrayList<JsonParser> (size_) ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				for (auto &&i : mHeap->mList[mIndex].mArraySet.iter ()) {
					if (rax.length () >= size_)
						continue ;
					INDEX ix = rax.insert () ;
					rax[ix] = JsonParser (mHeap ,mHeap->mList[mIndex].mArraySet.get (i)) ;
				}
			}
			const auto r1x = Array<INDEX>::make (rax.iter ()) ;
			Array<JsonParser> ret = Array<JsonParser> (size_) ;
			for (auto &&i : r1x.iter ())
				ret[i] = move (rax[r1x[i]]) ;
			return move (ret) ;
		}

		BOOL equal (CREF<JsonParser> that) const {
			if (exist () != that.exist ())
				return FALSE ;
			if ifnot (exist ())
				return TRUE ;
			if (address (mHeap->mList) != address (that.mHeap->mList))
				return FALSE ;
			if (mIndex != that.mIndex)
				return FALSE ;
			return TRUE ;
		}

		inline BOOL operator== (CREF<JsonParser> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<JsonParser> that) const {
			return ifnot (equal (that)) ;
		}

		CREF<String<STRU8>> value () const leftvalue {
			assume (exist ()) ;
			assume (string_type ()) ;
			return mHeap->mList[mIndex].mValue ;
		}

		template <class ARG1 ,class ARG2>
		REMOVE_ALL<ARG1> value (XREF<ARG1> def ,XREF<ARG2> cvt) const {
			using R1X = REMOVE_ALL<ARG1> ;
			auto rax = Box<R1X> () ;
			try_invoke ([&] () {
				rax = Box<R1X>::make (cvt (value ())) ;
			} ,[&] () {
				rax = Box<R1X>::make (forward[TYPEAS<ARG1>::id] (def)) ;
			}) ;
			return move (rax.self) ;
		}

		BOOL value (CREF<BOOL> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<BOOL ,STRU8>>::id]) ;
		}

		csc_pointer_t value (CREF<csc_pointer_t>) const = delete ;

		VAL32 value (CREF<VAL32> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<VAL32 ,STRU8>>::id]) ;
		}

		VAL64 value (CREF<VAL64> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id]) ;
		}

		SINGLE value (CREF<SINGLE> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<SINGLE ,STRU8>>::id]) ;
		}

		DOUBLE value (CREF<DOUBLE> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<DOUBLE ,STRU8>>::id]) ;
		}

		BYTE value (CREF<BYTE> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<BYTE ,STRU8>>::id]) ;
		}

		WORD value (CREF<WORD> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<WORD ,STRU8>>::id]) ;
		}

		CHAR value (CREF<CHAR> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<CHAR ,STRU8>>::id]) ;
		}

		DATA value (CREF<DATA> def) const {
			return value (def ,string_parse[TYPEAS<TYPEAS<DATA ,STRU8>>::id]) ;
		}

		String<STRA> value (CREF<String<STRA>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRA ,STRU8>>::id]) ;
		}

		String<STRW> value (CREF<String<STRW>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRW ,STRU8>>::id]) ;
		}

		String<STRU8> value (CREF<String<STRU8>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU8>>::id]) ;
		}

		String<STRU16> value (CREF<String<STRU16>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRU16 ,STRU8>>::id]) ;
		}

		String<STRU32> value (CREF<String<STRU32>> def) const {
			return value (def ,string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::id]) ;
		}
	} ;
} ;

template <class DEPEND>
trait JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;

	using NODE_CLAZZ_NULL = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_NULL ;
	using NODE_CLAZZ_STRING = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_STRING ;
	using NODE_CLAZZ_ARRAY = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_ARRAY ;
	using NODE_CLAZZ_OBJECT = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_OBJECT ;

	class Serialization {
	private:
		LENGTH mRecursiveCounter ;
		TextReader<STRU8> mTextReader ;
		RegularReader mReader ;
		ArrayList<NODE> mList ;
		SoftSet<String<STRU8>> mAttributeSet ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mLastIndex ;
		String<STRU8> mLastString ;

	public:
		implicit Serialization () = delete ;

		explicit Serialization (RREF<CRef<RegBuffer<STRU8>>> stream) {
			mRecursiveCounter = 0 ;
			mTextReader = TextReader<STRU8> (move (stream)) ;
			mReader = RegularReader (VRef<TextReader<STRU8>>::reference (mTextReader) ,3) ;
		}

		HEAP poll_heap () {
			HEAP ret ;
			const auto r1x = shrink_order () ;
			ret.mList = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mList.iter ()) {
				INDEX ix = r1x[i] ;
				ret.mList[ix].mName = move (mList[i].mName) ;
				ret.mList[ix].mAttributeSet = move (mList[i].mAttributeSet) ;
				ret.mList[ix].mArraySet = move (mList[i].mArraySet) ;
				for (auto &&j : ret.mList[ix].mArraySet.iter ()) {
					INDEX iy = r1x[ret.mList[ix].mArraySet.get (j)] ;
					ret.mList[ix].mArraySet.set (j ,iy) ;
				}
				ret.mList[ix].mObjectSet = move (mList[i].mObjectSet) ;
				for (auto &&j : ret.mList[ix].mObjectSet.iter ()) {
					INDEX iy = r1x[ret.mList[ix].mObjectSet.get (j)] ;
					ret.mList[ix].mObjectSet.set (j ,iy) ;
				}
				ret.mList[ix].mClazz = mList[i].mClazz ;
				ret.mList[ix].mParent = r1x[mList[i].mParent] ;
				ret.mList[ix].mBrother = r1x[mList[i].mBrother] ;
				ret.mList[ix].mChild = r1x[mList[i].mChild] ;
			}
			return move (ret) ;
		}

		Array<INDEX> shrink_order () const {
			Array<INDEX> ret = Array<INDEX> (mList.size ()) ;
			ret.fill (NONE) ;
			INDEX ix = 0 ;
			for (auto &&i : mList.iter ()) {
				ret[i] = ix ;
				ix++ ;
			}
			return move (ret) ;
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
			assert (mLastIndex == 0) ;
		}

		//@info: $0->$11 $10 $12
		void update_shift_e0 () {
			update_shift_e11 () ;
			mReader >> RegularReader::SKIP_GAP ;
			update_shift_e10 () ;
			INDEX ix = mLastIndex ;
			mReader >> RegularReader::SKIP_GAP ;
			update_shift_e12 () ;
			mLastIndex = ix ;
		}

		//@info: $1->${value}
		void update_shift_e1 () {
			mReader >> RegularReader::HINT_VALUE >> mLastString ;
		}

		//@info: $2->true|TRUE|false|FALSE
		void update_shift_e2 () {
			auto fax = TRUE ;
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('t'))
					discard ;
				mReader >> slice ("true") ;
				mLastString = slice ("true") ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('T'))
					discard ;
				mReader >> slice ("TRUE") ;
				mLastString = slice ("TRUE") ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('f'))
					discard ;
				mReader >> slice ("false") ;
				mLastString = slice ("false") ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('F'))
					discard ;
				mReader >> slice ("FALSE") ;
				mLastString = slice ("FALSE") ;
			}
			if ifswitch (fax) {
				assume (FALSE) ;
			}
		}

		//@info: $2x->null
		void update_shift_e2x () {
			mReader >> slice ("null") ;
			mLastString = String<STRU8> () ;
		}

		//@info: $3->"${string}"
		void update_shift_e3 () {
			mReader >> RegularReader::HINT_STRING >> mLastString ;
		}

		//@info: $4->$1|$2|$2x|$3|$6|$9
		void update_shift_e4 (CREF<INDEX> curr) {
			Scope<VREF<RecursiveCounter>> anonymous (RecursiveCounter::from (mRecursiveCounter)) ;
			INDEX ix = NONE ;
			auto fax = TRUE ;
			if ifswitch (fax) {
				if ifnot (is_first_number ())
					discard ;
				ix = mList.insert () ;
				update_shift_e1 () ;
				mList[ix].mValue = move (mLastString) ;
				mList[ix].mClazz = NODE_CLAZZ_STRING::value ;
				mList[ix].mParent = curr ;
				mList[ix].mBrother = NONE ;
				mList[ix].mChild = NONE ;
			}
			if ifswitch (fax) {
				if ifnot (is_first_boolean ())
					discard ;
				ix = mList.insert () ;
				update_shift_e2 () ;
				mList[ix].mValue = move (mLastString) ;
				mList[ix].mClazz = NODE_CLAZZ_STRING::value ;
				mList[ix].mParent = curr ;
				mList[ix].mBrother = NONE ;
				mList[ix].mChild = NONE ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('n'))
					discard ;
				ix = mList.insert () ;
				update_shift_e2x () ;
				mList[ix].mClazz = NODE_CLAZZ_NULL::value ;
				mList[ix].mParent = curr ;
				mList[ix].mBrother = NONE ;
				mList[ix].mChild = NONE ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('\"'))
					discard ;
				ix = mList.insert () ;
				update_shift_e3 () ;
				mList[ix].mValue = move (mLastString) ;
				mList[ix].mClazz = NODE_CLAZZ_STRING::value ;
				mList[ix].mParent = curr ;
				mList[ix].mBrother = NONE ;
				mList[ix].mChild = NONE ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('['))
					discard ;
				update_shift_e6 (curr) ;
				ix = mLastIndex ;
			}
			if ifswitch (fax) {
				if ifnot (mReader[0] == STRU8 ('{'))
					discard ;
				update_shift_e9 (curr) ;
				ix = mLastIndex ;
			}
			if ifswitch (fax) {
				assume (FALSE) ;
			}
			mLastIndex = ix ;
		}

		BOOL is_first_number () const {
			if (mReader[0] == STRU8 ('+'))
				return TRUE ;
			if (mReader[0] == STRU8 ('-'))
				return TRUE ;
			if (mReader[0] >= STRU8 ('0'))
				if (mReader[0] <= STRU8 ('9'))
					return TRUE ;
			return FALSE ;
		}

		BOOL is_first_boolean () const {
			if (mReader[0] == STRU8 ('t'))
				return TRUE ;
			if (mReader[0] == STRU8 ('T'))
				return TRUE ;
			if (mReader[0] == STRU8 ('f'))
				return TRUE ;
			if (mReader[0] == STRU8 ('F'))
				return TRUE ;
			return FALSE ;
		}

		//@info: $5->$4|$4 , $5
		void update_shift_e5 (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				update_shift_e4 (curr) ;
				const auto r2x = mList[curr].mArraySet.length () ;
				mList[curr].mArraySet.add (r2x ,mLastIndex) ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (ix == NONE)
						discard ;
					ix = mLastIndex ;
				}
				if ifswitch (eax) {
					mList[iy].mBrother = mLastIndex ;
				}
				iy = mLastIndex ;
				mReader >> RegularReader::SKIP_GAP ;
				if (mReader[0] != STRU8 (','))
					break ;
				mReader++ ;
				mReader >> RegularReader::SKIP_GAP ;
			}
			mLastIndex = ix ;
		}

		//@info: $6->[ ]|[ $5 ]
		void update_shift_e6 (CREF<INDEX> curr) {
			Scope<VREF<RecursiveCounter>> anonymous (RecursiveCounter::from (mRecursiveCounter)) ;
			mReader >> slice ("[") ;
			INDEX ix = mList.insert () ;
			mList[ix].mArraySet = mArraySet.share () ;
			mList[ix].mClazz = NODE_CLAZZ_ARRAY::value ;
			mList[ix].mParent = curr ;
			mList[ix].mBrother = NONE ;
			mList[ix].mChild = NONE ;
			mReader >> RegularReader::SKIP_GAP ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 (']'))
					discard ;
				update_shift_e5 (ix) ;
				mList[ix].mChild = mLastIndex ;
				mReader >> RegularReader::SKIP_GAP ;
			}
			mReader >> slice ("]") ;
			mLastIndex = ix ;
		}

		//@info: $7->$3 : $4
		void update_shift_e7 (CREF<INDEX> curr) {
			update_shift_e3 () ;
			auto rax = move (mLastString) ;
			mReader >> RegularReader::SKIP_GAP ;
			mReader >> slice (":") ;
			mReader >> RegularReader::SKIP_GAP ;
			update_shift_e4 (curr) ;
			mList[curr].mObjectSet.add (move (rax) ,mLastIndex) ;
		}

		//@info: $8->$7|$7 , $8
		void update_shift_e8 (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				update_shift_e7 (curr) ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (ix == NONE)
						discard ;
					ix = mLastIndex ;
				}
				if ifswitch (eax) {
					mList[iy].mBrother = mLastIndex ;
				}
				iy = mLastIndex ;
				mReader >> RegularReader::SKIP_GAP ;
				if (mReader[0] != STRU8 (','))
					break ;
				mReader++ ;
				mReader >> RegularReader::SKIP_GAP ;
			}
			mLastIndex = ix ;
		}

		//@info: $9->{ }|{ $8 }
		void update_shift_e9 (CREF<INDEX> curr) {
			Scope<VREF<RecursiveCounter>> anonymous (RecursiveCounter::from (mRecursiveCounter)) ;
			mReader >> slice ("{") ;
			INDEX ix = mList.insert () ;
			mList[ix].mObjectSet = mObjectSet.share () ;
			mList[ix].mClazz = NODE_CLAZZ_OBJECT::value ;
			mList[ix].mParent = curr ;
			mList[ix].mBrother = NONE ;
			mList[ix].mChild = NONE ;
			mReader >> RegularReader::SKIP_GAP ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('}'))
					discard ;
				update_shift_e8 (ix) ;
				mList[ix].mChild = mLastIndex ;
				mReader >> RegularReader::SKIP_GAP ;
			}
			mReader >> slice ("}") ;
			mLastIndex = ix ;
		}

		//@info: $10->${eps}|$4
		void update_shift_e10 () {
			INDEX ix = NONE ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('\0'))
					discard ;
				update_shift_e4 (NONE) ;
				ix = mLastIndex ;
			}
			mLastIndex = ix ;
		}

		//@info: $11->${eps}
		void update_shift_e11 () {
			noop () ;
		}

		//@info: $12->${end}
		void update_shift_e12 () {
			assume (mReader[0] == STRU8 ('\0')) ;
		}
	} ;
} ;

using JsonParser = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::JsonParser ;
} ;
} ;

namespace CSC {
using namespace SERIALIZATION ;
} ;