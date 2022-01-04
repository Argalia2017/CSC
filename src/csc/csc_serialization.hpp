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
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<RecursiveCounter>>::id ,unsafe_deptr (that))) ;
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
		Set<String<STRU8>> mAttributeSet ;
		Set<INDEX> mArraySet ;
		Set<String<STRU8>> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

	struct HEAP {
		Array<NODE> mList ;
		ArrayList<String<STRU8>> mAttribute ;
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
			auto rax = TEMP<R1X> () ;
			auto &&tmp = R1X::from (rax ,item) ;
			tmp.generate () ;
			ret.mHeap = CRef<HEAP>::make (tmp.poll_heap ()) ;
			ret.mIndex = tmp.root () ;
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
				const auto r1x = Array<INDEX>::make_range (mHeap->mList[mIndex].mArraySet.iter ()) ;
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
			const auto r1x = Array<INDEX>::make_range (rax.iter ()) ;
			Array<XmlParser> ret = Array<XmlParser> (size_) ;
			for (auto &&i : ret.iter ())
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
			R1X ret ;
			try_invoke ([&] () {
				ret = cvt (attribute (tag)) ;
			} ,[&] () {
				ret = forward (TYPEAS<ARG1>::id ,def) ;
			}) ;
			return move (ret) ;
		}

		BOOL attribute (CREF<String<STRU8>> tag ,CREF<BOOL> def) const {
			using R1X = FUNCTION_string_parse<BOOL ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		csc_pointer_t attribute (CREF<String<STRU8>> ,CREF<csc_pointer_t>) const = delete ;

		VAL32 attribute (CREF<String<STRU8>> tag ,CREF<VAL32> def) const {
			using R1X = FUNCTION_string_parse<VAL32 ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		VAL64 attribute (CREF<String<STRU8>> tag ,CREF<VAL64> def) const {
			using R1X = FUNCTION_string_parse<VAL64 ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		SINGLE attribute (CREF<String<STRU8>> tag ,CREF<SINGLE> def) const {
			using R1X = FUNCTION_string_parse<SINGLE ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		DOUBLE attribute (CREF<String<STRU8>> tag ,CREF<DOUBLE> def) const {
			using R1X = FUNCTION_string_parse<DOUBLE ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		BYTE attribute (CREF<String<STRU8>> tag ,CREF<BYTE> def) const {
			using R1X = FUNCTION_string_parse<BYTE ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		WORD attribute (CREF<String<STRU8>> tag ,CREF<WORD> def) const {
			using R1X = FUNCTION_string_parse<WORD ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		CHAR attribute (CREF<String<STRU8>> tag ,CREF<CHAR> def) const {
			using R1X = FUNCTION_string_parse<CHAR ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		DATA attribute (CREF<String<STRU8>> tag ,CREF<DATA> def) const {
			using R1X = FUNCTION_string_parse<DATA ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		String<STRA> attribute (CREF<String<STRU8>> tag ,CREF<String<STRA>> def) const {
			using R1X = FUNCTION_string_cvt<STRA ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		String<STRW> attribute (CREF<String<STRU8>> tag ,CREF<String<STRW>> def) const {
			using R1X = FUNCTION_string_cvt<STRW ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		String<STRU8> attribute (CREF<String<STRU8>> tag ,CREF<String<STRU8>> def) const {
			using R1X = FUNCTION_string_cvt<STRU8 ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		String<STRU16> attribute (CREF<String<STRU8>> tag ,CREF<String<STRU16>> def) const {
			using R1X = FUNCTION_string_cvt<STRU16 ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		String<STRU32> attribute (CREF<String<STRU8>> tag ,CREF<String<STRU32>> def) const {
			using R1X = FUNCTION_string_cvt<STRU32 ,STRU8> ;
			return attribute (tag ,def ,R1X ()) ;
		}

		CREF<String<STRU8>> value () const leftvalue {
			assume (exist ()) ;
			assume (mHeap->mList[mIndex].mArraySet.size () == 0) ;
			assume (mHeap->mList[mIndex].mObjectSet.size () == 0) ;
			assume (mHeap->mList[mIndex].mAttributeSet.length () == 1) ;
			INDEX ix = 0 ;
			return mHeap->mAttribute[ix] ;
		}

		template <class ARG1 ,class ARG2>
		REMOVE_ALL<ARG1> value (XREF<ARG1> def ,XREF<ARG2> cvt) const {
			using R1X = REMOVE_ALL<ARG1> ;
			R1X ret ;
			try_invoke ([&] () {
				ret = cvt (value ()) ;
			} ,[&] () {
				ret = forward (TYPEAS<ARG1>::id ,def) ;
			}) ;
			return move (ret) ;
		}

		BOOL value (CREF<BOOL> def) const {
			using R1X = FUNCTION_string_parse<BOOL ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		csc_pointer_t value (CREF<csc_pointer_t>) const = delete ;

		VAL32 value (CREF<VAL32> def) const {
			using R1X = FUNCTION_string_parse<VAL32 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		VAL64 value (CREF<VAL64> def) const {
			using R1X = FUNCTION_string_parse<VAL64 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		SINGLE value (CREF<SINGLE> def) const {
			using R1X = FUNCTION_string_parse<SINGLE ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		DOUBLE value (CREF<DOUBLE> def) const {
			using R1X = FUNCTION_string_parse<DOUBLE ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		BYTE value (CREF<BYTE> def) const {
			using R1X = FUNCTION_string_parse<BYTE ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		WORD value (CREF<WORD> def) const {
			using R1X = FUNCTION_string_parse<WORD ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		CHAR value (CREF<CHAR> def) const {
			using R1X = FUNCTION_string_parse<CHAR ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		DATA value (CREF<DATA> def) const {
			using R1X = FUNCTION_string_parse<DATA ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRA> value (CREF<String<STRA>> def) const {
			using R1X = FUNCTION_string_cvt<STRA ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRW> value (CREF<String<STRW>> def) const {
			using R1X = FUNCTION_string_cvt<STRW ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRU8> value (CREF<String<STRU8>> def) const {
			using R1X = FUNCTION_string_cvt<STRU8 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRU16> value (CREF<String<STRU16>> def) const {
			using R1X = FUNCTION_string_cvt<STRU16 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRU32> value (CREF<String<STRU32>> def) const {
			using R1X = FUNCTION_string_cvt<STRU32 ,STRU8> ;
			return value (def ,R1X ()) ;
		}
	} ;
} ;

template <class DEPEND>
trait XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;

	class Serialization extend Proxy {
	private:
		CRef<RegBuffer<STRU8>> mData ;
		LENGTH mRecursiveCounter ;
		RegularReader mReader ;
		ArrayList<NODE> mList ;
		ArrayList<String<STRU8>> mAttribute ;
		INDEX mRoot ;
		INDEX mLastIndex ;
		String<STRU8> mLastString ;

	public:
		imports VREF<Serialization> from (VREF<TEMP<Serialization>> where_ ,CREF<RegBuffer<STRU8>> item) {
			auto &&tmp = unsafe_deref (where_) ;
			unimplemented () ;
			return tmp ;
		}

		HEAP poll_heap () {
			HEAP ret ;
			const auto r1x = shrink_map () ;
			ret.mList = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mList.iter ()) {
				INDEX ix = r1x[i] ;
				ret.mList[ix].mName = move (mList[i].mName) ;
				ret.mList[ix].mAttributeSet = move (mList[i].mAttributeSet) ;
				ret.mList[ix].mArraySet = Set<INDEX> (mList[i].mArraySet.length ()) ;
				for (auto &&j : mList[i].mArraySet.iter ()) {
					const auto r2x = r1x[mList[i].mArraySet.get (j)] ;
					ret.mList[ix].mArraySet.add (mList[i].mArraySet[j] ,r2x) ;
				}
				ret.mList[ix].mObjectSet = Set<String<STRU8>> (mList[i].mObjectSet.length ()) ;
				for (auto &&j : mList[i].mObjectSet.iter ()) {
					const auto r2x = r1x[mList[i].mObjectSet.get (j)] ;
					ret.mList[ix].mObjectSet.add (mList[i].mObjectSet[j] ,r2x) ;
				}
				ret.mList[ix].mParent = r1x[mList[i].mParent] ;
				ret.mList[ix].mBrother = r1x[mList[i].mBrother] ;
				ret.mList[ix].mChild = r1x[mList[i].mChild] ;
			}
			ret.mAttribute = move (mAttribute) ;
			return move (ret) ;
		}

		Array<INDEX> shrink_map () const {
			Array<INDEX> ret = Array<INDEX> (mList.size ()) ;
			ret.fill (NONE) ;
			INDEX ix = 0 ;
			for (auto &&i : mList.iter ()) {
				ret[i] = ix ;
				ix++ ;
			}
			return move (ret) ;
		}

		INDEX root () const {
			return mLastIndex ;
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
			mRoot = mLastIndex ;
		}

		void update_shift_e0 () {
			update_shift_e8 () ;
			mReader >> RegularReader::SKIP_GAP ;
			INDEX ix = mList.insert () ;
			mList[ix].mArraySet = Set<INDEX> () ;
			mList[ix].mObjectSet = Set<String<STRU8>> () ;
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
				if (!(mReader[0] >= STRU8 ('A') && mReader[0] <= STRU8 ('Z')))
					if (!(mReader[0] >= STRU8 ('a') && mReader[0] <= STRU8 ('z')))
						if (!(mReader[0] == STRU8 ('_')))
							break ;
				update_shift_e3 (curr) ;
				mReader >> RegularReader::SKIP_GAP ;
			}
		}

		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		void update_shift_e5 (CREF<INDEX> curr) {
			Scope<VREF<RecursiveCounter>> anonymous (RecursiveCounter::from (mRecursiveCounter)) ;
			mReader >> slice ("<") ;
			INDEX ix = mList.insert () ;
			update_shift_e1 () ;
			mList[ix].mName = move (mLastString) ;
			mList[ix].mAttributeSet = Set<String<STRU8>> () ;
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
				mList[ix].mArraySet = Set<INDEX> () ;
				mList[ix].mObjectSet = Set<String<STRU8>> () ;
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
				const auto r1x = BOOL (mReader[0] == STRU8 ('<') && mReader[1] == STRU8 ('!')) ;
				const auto r2x = BOOL (mReader[0] == STRU8 ('<') && mReader[1] != STRU8 ('/')) ;
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
		Set<INDEX> mArraySet ;
		Set<String<STRU8>> mObjectSet ;
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
			auto rax = TEMP<R1X> () ;
			auto &&tmp = R1X::from (rax ,item) ;
			tmp.generate () ;
			ret.mHeap = CRef<HEAP>::make (tmp.poll_heap ()) ;
			ret.mIndex = tmp.root () ;
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

		JsonParser child (CREF<String<STRU8>> key) const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			if ifnot (object_type ())
				return JsonParser (mHeap ,NONE) ;
			INDEX ix = mHeap->mList[mIndex].mObjectSet.map (key) ;
			return JsonParser (mHeap ,ix) ;
		}

		Array<JsonParser> child_array () const {
			Array<JsonParser> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				if ifnot (array_type ())
					discard ;
				const auto r1x = Array<INDEX>::make_range (mHeap->mList[mIndex].mArraySet.iter ()) ;
				ret = Array<JsonParser> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = JsonParser (mHeap ,mHeap->mList[mIndex].mArraySet.get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<JsonParser> child_array (CREF<LENGTH> size_) const {
			auto rax = Deque<JsonParser> (size_) ;
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
			const auto r1x = Array<INDEX>::make_range (rax.iter ()) ;
			Array<JsonParser> ret = Array<JsonParser> (size_) ;
			for (auto &&i : ret.iter ())
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
			assume (mHeap->mList[mIndex].mArraySet.size () == 0) ;
			assume (mHeap->mList[mIndex].mObjectSet.size () == 0) ;
			return mHeap->mList[mIndex].mValue ;
		}

		template <class ARG1 ,class ARG2>
		REMOVE_ALL<ARG1> value (XREF<ARG1> def ,XREF<ARG2> cvt) const {
			using R1X = REMOVE_ALL<ARG1> ;
			R1X ret ;
			try_invoke ([&] () {
				ret = cvt (value ()) ;
			} ,[&] () {
				ret = forward (TYPEAS<ARG1>::id ,def) ;
			}) ;
			return move (ret) ;
		}

		BOOL value (CREF<BOOL> def) const {
			using R1X = FUNCTION_string_parse<BOOL ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		csc_pointer_t value (CREF<csc_pointer_t>) const = delete ;

		VAL32 value (CREF<VAL32> def) const {
			using R1X = FUNCTION_string_parse<VAL32 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		VAL64 value (CREF<VAL64> def) const {
			using R1X = FUNCTION_string_parse<VAL64 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		SINGLE value (CREF<SINGLE> def) const {
			using R1X = FUNCTION_string_parse<SINGLE ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		DOUBLE value (CREF<DOUBLE> def) const {
			using R1X = FUNCTION_string_parse<DOUBLE ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		BYTE value (CREF<BYTE> def) const {
			using R1X = FUNCTION_string_parse<BYTE ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		WORD value (CREF<WORD> def) const {
			using R1X = FUNCTION_string_parse<WORD ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		CHAR value (CREF<CHAR> def) const {
			using R1X = FUNCTION_string_parse<CHAR ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		DATA value (CREF<DATA> def) const {
			using R1X = FUNCTION_string_parse<DATA ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRA> value (CREF<String<STRA>> def) const {
			using R1X = FUNCTION_string_cvt<STRA ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRW> value (CREF<String<STRW>> def) const {
			using R1X = FUNCTION_string_cvt<STRW ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRU8> value (CREF<String<STRU8>> def) const {
			using R1X = FUNCTION_string_cvt<STRU8 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRU16> value (CREF<String<STRU16>> def) const {
			using R1X = FUNCTION_string_cvt<STRU16 ,STRU8> ;
			return value (def ,R1X ()) ;
		}

		String<STRU32> value (CREF<String<STRU32>> def) const {
			using R1X = FUNCTION_string_cvt<STRU32 ,STRU8> ;
			return value (def ,R1X ()) ;
		}
	} ;
} ;

template <class DEPEND>
trait JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;

	class Serialization extend Proxy {
	public:
		imports VREF<Serialization> from (VREF<TEMP<Serialization>> where_ ,CREF<RegBuffer<STRU8>> item) {
			auto &&tmp = unsafe_deref (where_) ;
			unimplemented () ;
			return tmp ;
		}

		HEAP poll_heap () {
			unimplemented () ;
			return bad (TYPEAS<HEAP>::id) ;
		}

		INDEX root () const {
			unimplemented () ;
			return bad (TYPEAS<INDEX>::id) ;
		}

		void generate () {

		}
	} ;
} ;

using JsonParser = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::JsonParser ;
} ;
} ;

namespace CSC {
using namespace SERIALIZATION ;
} ;