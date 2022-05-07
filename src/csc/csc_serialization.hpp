#pragma once

#ifndef __CSC_SERIALIZATION__
#define __CSC_SERIALIZATION__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
template <class...>
trait XMLPARSER_HELP ;

template <class...>
trait XMLPARSER_SERIALIZATION_HELP ;

template <class...>
trait XMLPARSER_COMBINATION_HELP ;

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
		Array<NODE> mTree ;
		Array<String<STRU8>> mAttribute ;
	} ;

	class XmlParser {
	protected:
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
			auto rax = R1X (RegBuffer<STRU8>::from (item ,0 ,item.size ())) ;
			rax.generate () ;
			return rax.poll () ;
		}

		imports XmlParser make (CREF<VarBuffer<STRU8>> item) {
			return make (RegBuffer<STRU8>::from (item)) ;
		}

		imports XmlParser make (CREF<ConBuffer<STRU8>> item) {
			return make (RegBuffer<STRU8>::from (item)) ;
		}

		imports XmlParser make (CREF<RegBuffer<BYTE>> item) {
			using R1X = typename XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS>::Serialization ;
			auto rax = R1X (RegBuffer<STRU8>::from (item ,0 ,item.size ())) ;
			rax.generate () ;
			return rax.poll () ;
		}

		imports XmlParser make (CREF<VarBuffer<BYTE>> item) {
			return make (RegBuffer<BYTE>::from (item)) ;
		}

		imports XmlParser make (CREF<ConBuffer<BYTE>> item) {
			return make (RegBuffer<BYTE>::from (item)) ;
		}

		imports XmlParser make (CREF<Array<XmlParser>> sequence) {
			using R1X = typename XMLPARSER_COMBINATION_HELP<DEPEND ,ALWAYS>::Combination ;
			auto rax = R1X (sequence) ;
			rax.generate () ;
			return rax.poll () ;
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
			return XmlParser (mHeap ,mHeap->mTree[mIndex].mParent) ;
		}

		XmlParser brother () const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			return XmlParser (mHeap ,mHeap->mTree[mIndex].mBrother) ;
		}

		XmlParser child () const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			return XmlParser (mHeap ,mHeap->mTree[mIndex].mChild) ;
		}

		XmlParser child (CREF<Slice<STRA>> name) const {
			return child (String<STRU8>::make (name)) ;
		}

		XmlParser child (CREF<Slice<STRW>> name) const {
			return child (String<STRU8>::make (name)) ;
		}

		XmlParser child (CREF<String<STRU8>> name) const {
			if ifnot (exist ())
				return XmlParser (mHeap ,NONE) ;
			INDEX ix = mHeap->mTree[mIndex].mObjectSet.map (name) ;
			return XmlParser (mHeap ,ix) ;
		}

		XmlParser child (CREF<String<STRU16>> name) const {
			return child (string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU16>>::id] (name)) ;
		}

		XmlParser child (CREF<String<STRU32>> name) const {
			return child (string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU32>>::id] (name)) ;
		}

		Array<XmlParser> child_array () const {
			Array<XmlParser> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				const auto r1x = Array<INDEX>::make (mHeap->mTree[mIndex].mArraySet.iter ()) ;
				ret = Array<XmlParser> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = XmlParser (mHeap ,mHeap->mTree[mIndex].mArraySet.get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<XmlParser> child_array (CREF<LENGTH> size_) const {
			auto rax = ArrayList<XmlParser> (size_) ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				for (auto &&i : mHeap->mTree[mIndex].mArraySet.iter ()) {
					if (rax.length () >= size_)
						break ;
					INDEX ix = rax.insert () ;
					rax[ix] = XmlParser (mHeap ,mHeap->mTree[mIndex].mArraySet.get (i)) ;
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
			if (address (mHeap->mTree) != address (that.mHeap->mTree))
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
			return mHeap->mTree[mIndex].mName ;
		}

		CREF<String<STRU8>> attribute (CREF<Slice<STRA>> tag) const leftvalue {
			return attribute (String<STRU8>::make (tag)) ;
		}

		CREF<String<STRU8>> attribute (CREF<Slice<STRW>> tag) const leftvalue {
			return attribute (String<STRU8>::make (tag)) ;
		}

		CREF<String<STRU8>> attribute (CREF<String<STRU8>> tag) const leftvalue {
			if ifnot (exist ())
				return String<STRU8>::zero () ;
			INDEX ix = mHeap->mTree[mIndex].mAttributeSet.map (tag) ;
			if (ix == NONE)
				return String<STRU8>::zero () ;
			return mHeap->mAttribute[ix] ;
		}

		CREF<String<STRU8>> attribute (CREF<String<STRU16>> tag) const leftvalue {
			return attribute (string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU16>>::id] (tag)) ;
		}

		CREF<String<STRU8>> attribute (CREF<String<STRU32>> tag) const leftvalue {
			return attribute (string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU32>>::id] (tag)) ;
		}

		CREF<String<STRU8>> fetch () const leftvalue {
			assume (exist ()) ;
			assume (mHeap->mTree[mIndex].mArraySet.size () == 0) ;
			assume (mHeap->mTree[mIndex].mObjectSet.size () == 0) ;
			assume (mHeap->mTree[mIndex].mAttributeSet.length () == 1) ;
			INDEX ix = mHeap->mAttribute.ibegin () ;
			return mHeap->mAttribute[ix] ;
		}

		template <class ARG1 ,class ARG2>
		ARG1 fetch (CREF<ARG1> def ,CREF<ARG2> cvt) const {
			using R1X = ARG1 ;
			auto rax = Optional<R1X> () ;
			try_invoke ([&] () {
				rax = Optional<R1X>::make (cvt (fetch ())) ;
			} ,[&] () {
				rax = Optional<R1X>::make (def) ;
			} ,[&] () {
				noop () ;
			}) ;
			return rax.poll () ;
		}

		BOOL fetch (CREF<BOOL> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<BOOL ,STRU8>>::id]) ;
		}

		VAL32 fetch (CREF<VAL32> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<VAL32 ,STRU8>>::id]) ;
		}

		VAL64 fetch (CREF<VAL64> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id]) ;
		}

		SINGLE fetch (CREF<SINGLE> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<SINGLE ,STRU8>>::id]) ;
		}

		DOUBLE fetch (CREF<DOUBLE> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<DOUBLE ,STRU8>>::id]) ;
		}

		BYTE fetch (CREF<BYTE> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<BYTE ,STRU8>>::id]) ;
		}

		WORD fetch (CREF<WORD> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<WORD ,STRU8>>::id]) ;
		}

		CHAR fetch (CREF<CHAR> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<CHAR ,STRU8>>::id]) ;
		}

		DATA fetch (CREF<DATA> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<DATA ,STRU8>>::id]) ;
		}

		String<STRA> fetch (CREF<String<STRA>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRA ,STRU8>>::id]) ;
		}

		String<STRW> fetch (CREF<String<STRW>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRW ,STRU8>>::id]) ;
		}

		String<STRU8> fetch (CREF<String<STRU8>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU8>>::id]) ;
		}

		String<STRU16> fetch (CREF<String<STRU16>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRU16 ,STRU8>>::id]) ;
		}

		String<STRU32> fetch (CREF<String<STRU32>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::id]) ;
		}
	} ;
} ;

template <class DEPEND>
trait XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;
	using XmlParser = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::XmlParser ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class Serialization extend XmlParser {
	protected:
		LENGTH mRecursiveCounter ;
		TextReader<STRU8> mTextReader ;
		RegularReader mReader ;
		List<NODE> mTree ;
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

		XmlParser poll () {
			auto &&tmp = keep[TYPEAS<VREF<XmlParser>>::id] (thiz) ;
			auto rax = HEAP () ;
			const auto r1x = shrink_order () ;
			rax.mTree = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mTree.iter ()) {
				INDEX ix = r1x[i] ;
				rax.mTree[ix].mName = move (mTree[i].mName) ;
				rax.mTree[ix].mAttributeSet = move (mTree[i].mAttributeSet) ;
				rax.mTree[ix].mArraySet = move (mTree[i].mArraySet) ;
				for (auto &&j : rax.mTree[ix].mArraySet.iter ()) {
					INDEX iy = r1x[rax.mTree[ix].mArraySet.get (j)] ;
					rax.mTree[ix].mArraySet.set (j ,iy) ;
				}
				rax.mTree[ix].mObjectSet = move (mTree[i].mObjectSet) ;
				for (auto &&j : rax.mTree[ix].mObjectSet.iter ()) {
					INDEX iy = r1x[rax.mTree[ix].mObjectSet.get (j)] ;
					rax.mTree[ix].mObjectSet.set (j ,iy) ;
				}
				rax.mTree[ix].mParent = r1x[mTree[i].mParent] ;
				rax.mTree[ix].mBrother = r1x[mTree[i].mBrother] ;
				rax.mTree[ix].mChild = r1x[mTree[i].mChild] ;
			}
			rax.mAttribute = Array<String<STRU8>> (mAttribute.length ()) ;
			for (auto &&i : mAttribute.iter ()) {
				assume (vbetween (i ,0 ,rax.mAttribute.length ())) ;
				rax.mAttribute[i] = move (mAttribute[i]) ;
			}
			tmp.mHeap = CRef<HEAP>::make (move (rax)) ;
			tmp.mIndex = 0 ;
			return move (tmp) ;
		}

		Array<INDEX> shrink_order () const {
			Array<INDEX> ret = Array<INDEX> (mTree.size ()) ;
			ret.fill (NONE) ;
			INDEX ix = 0 ;
			for (auto &&i : mTree.iter ()) {
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
			mReader >> RegularReader::SKIP_SPACE ;
			INDEX ix = mTree.insert () ;
			mTree[ix].mArraySet = mArraySet.share () ;
			mTree[ix].mObjectSet = mObjectSet.share () ;
			mTree[ix].mParent = NONE ;
			update_shift_e7 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mTree[ix].mBrother = NONE ;
			mReader >> RegularReader::SKIP_SPACE ;
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
			INDEX ix = mTree[curr].mAttributeSet.map (mLastString) ;
			assume (ix == NONE) ;
			ix = mAttribute.insert () ;
			mTree[curr].mAttributeSet.add (move (mLastString) ,ix) ;
			mReader >> RegularReader::SKIP_SPACE ;
			mReader >> slice ("=") ;
			mReader >> RegularReader::SKIP_SPACE ;
			update_shift_e2 () ;
			mAttribute[ix] = move (mLastString) ;
		}

		//@info: $4->${eps}|$3 $4
		void update_shift_e4 (CREF<INDEX> curr) {
			while (TRUE) {
				if ifnot (is_frist_identity ())
					break ;
				update_shift_e3 (curr) ;
				mReader >> RegularReader::SKIP_SPACE ;
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
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::value) ;
			mReader >> slice ("<") ;
			INDEX ix = mTree.insert () ;
			update_shift_e1 () ;
			mTree[ix].mName = move (mLastString) ;
			mTree[ix].mAttributeSet = mAttributeSet.share () ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
			mReader >> RegularReader::SKIP_SPACE ;
			update_shift_e4 (ix) ;
			mReader >> RegularReader::SKIP_SPACE ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mReader[0] != STRU8 ('>'))
					discard ;
				mReader++ ;
				mReader >> RegularReader::SKIP_SPACE ;
				mTree[ix].mArraySet = mArraySet.share () ;
				mTree[ix].mObjectSet = mObjectSet.share () ;
				update_shift_e7 (ix) ;
				mTree[ix].mChild = mLastIndex ;
				mReader >> RegularReader::SKIP_SPACE ;
				mReader >> slice ("</") ;
				update_shift_e1 () ;
				assume (mTree[ix].mName == mLastString) ;
				mReader >> RegularReader::SKIP_SPACE ;
				mReader >> slice (">") ;
			}
			if ifswitch (rxx) {
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
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::value) ;
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				const auto r1x = is_first_comment () ;
				const auto r2x = is_first_end_of_node () ;
				if ifnot (r1x)
					if ifnot (r2x)
						break ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (r1x)
						discard ;
					update_shift_e6 () ;
				}
				if ifswitch (rxx) {
					if ifnot (r2x)
						discard ;
					update_shift_e5 (curr) ;
					const auto r3x = mTree[curr].mArraySet.length () ;
					mTree[curr].mArraySet.add (r3x ,mLastIndex) ;
					mTree[curr].mObjectSet.add (mTree[mLastIndex].mName ,mLastIndex) ;
					auto ryx = TRUE ;
					if ifswitch (ryx) {
						if ifnot (ix == NONE)
							discard ;
						ix = mLastIndex ;
					}
					if ifswitch (ryx) {
						mTree[iy].mBrother = mLastIndex ;
					}
					iy = mLastIndex ;
				}
				mReader >> RegularReader::SKIP_SPACE ;
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
			mReader >> RegularReader::SKIP_SPACE ;
			mReader >> slice ("version") ;
			mReader >> RegularReader::SKIP_SPACE ;
			mReader >> slice ("=") ;
			mReader >> RegularReader::SKIP_SPACE ;
			mReader >> slice ("\"1.0\"") ;
			mReader >> RegularReader::SKIP_SPACE ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('?'))
					discard ;
				mReader >> slice ("encoding") ;
				mReader >> RegularReader::SKIP_SPACE ;
				mReader >> slice ("=") ;
				mReader >> RegularReader::SKIP_SPACE ;
				mReader >> slice ("\"utf-8\"") ;
				mReader >> RegularReader::SKIP_SPACE ;
			}
			mReader >> slice ("?>") ;
		}

		//@info: $9->${end}
		void update_shift_e9 () {
			assume (mReader[0] == STRU8 ('\0')) ;
		}
	} ;
} ;

template <class DEPEND>
trait XMLPARSER_COMBINATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;
	using XmlParser = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::XmlParser ;

	using NODE_CLAZZ_NULL = RANK0 ;
	using NODE_CLAZZ_STRING = RANK1 ;
	using NODE_CLAZZ_ARRAY = RANK2 ;
	using NODE_CLAZZ_OBJECT = RANK3 ;
	using NODE_CLAZZ_FINAL = RANK5 ;

	struct FOUND_NODE {
		String<STRU8> mName ;
		SoftSet<String<STRU8>> mAttributeSet ;
		FLAG mClazz ;
		List<XmlParser> mBaseNode ;
	} ;

	struct STACK_NODE {
		INDEX mParent ;
		FLAG mClazz ;
		List<XmlParser> mBaseNode ;
	} ;

	class Combination extend XmlParser {
	protected:
		Array<XmlParser> mSequence ;
		String<STRU8> mClazzString ;
		String<STRU8> mObjectClazzString ;
		String<STRU8> mArrayClazzString ;
		String<STRU8> mFinalClazzString ;
		List<NODE> mTree ;
		ArrayList<String<STRU8>> mAttribute ;
		SoftSet<String<STRU8>> mAttributeSet ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		List<STACK_NODE> mNodeStack ;
		List<FOUND_NODE> mFoundNode ;
		Set<String<STRU8>> mFoundNodeSet ;
		INDEX mRoot ;
		List<List<XmlParser>> mBaseNodeQueue ;
		STACK_NODE mTempNode ;

	public:
		implicit Combination () = delete ;

		explicit Combination (CREF<Array<XmlParser>> sequence) {
			mSequence = move (sequence) ;
			mClazzString = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::id] (slice ("type")) ;
			mObjectClazzString = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::id] (slice ("object")) ;
			mArrayClazzString = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::id] (slice ("array")) ;
			mFinalClazzString = string_cvt[TYPEAS<TYPEAS<STRU8 ,STR>>::id] (slice ("final")) ;
			mRoot = mTree.insert () ;
			mTree[mRoot].mArraySet = mArraySet.share () ;
			mTree[mRoot].mObjectSet = mObjectSet.share () ;
			mTree[mRoot].mParent = NONE ;
			mTree[mRoot].mChild = NONE ;
			mTree[mRoot].mBrother = NONE ;
		}

		XmlParser poll () {
			auto &&tmp = keep[TYPEAS<VREF<XmlParser>>::id] (thiz) ;
			auto rax = HEAP () ;
			const auto r1x = shrink_order () ;
			rax.mTree = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mTree.iter ()) {
				INDEX ix = r1x[i] ;
				rax.mTree[ix].mName = move (mTree[i].mName) ;
				rax.mTree[ix].mAttributeSet = move (mTree[i].mAttributeSet) ;
				rax.mTree[ix].mArraySet = move (mTree[i].mArraySet) ;
				for (auto &&j : rax.mTree[ix].mArraySet.iter ()) {
					INDEX iy = r1x[rax.mTree[ix].mArraySet.get (j)] ;
					rax.mTree[ix].mArraySet.set (j ,iy) ;
				}
				rax.mTree[ix].mObjectSet = move (mTree[i].mObjectSet) ;
				for (auto &&j : rax.mTree[ix].mObjectSet.iter ()) {
					INDEX iy = r1x[rax.mTree[ix].mObjectSet.get (j)] ;
					rax.mTree[ix].mObjectSet.set (j ,iy) ;
				}
				rax.mTree[ix].mParent = r1x[mTree[i].mParent] ;
				rax.mTree[ix].mBrother = r1x[mTree[i].mBrother] ;
				rax.mTree[ix].mChild = r1x[mTree[i].mChild] ;
			}
			rax.mAttribute = Array<String<STRU8>> (mAttribute.length ()) ;
			for (auto &&i : mAttribute.iter ()) {
				assume (vbetween (i ,0 ,rax.mAttribute.length ())) ;
				rax.mAttribute[i] = move (mAttribute[i]) ;
			}
			tmp.mHeap = CRef<HEAP>::make (move (rax)) ;
			tmp.mIndex = 0 ;
			return move (tmp) ;
		}

		Array<INDEX> shrink_order () const {
			Array<INDEX> ret = Array<INDEX> (mTree.size ()) ;
			ret.fill (NONE) ;
			INDEX ix = 0 ;
			if ifswitch (TRUE) {
				ret[mRoot] = ix ;
				ix++ ;
			}
			for (auto &&i : mTree.iter ()) {
				if (ix == mRoot)
					continue ;
				ret[i] = ix ;
				ix++ ;
			}
			return move (ret) ;
		}

		FLAG node_type (CREF<XmlParser> node) const {
			auto &&tmp = node.attribute (mClazzString) ;
			if (tmp == mObjectClazzString)
				return NODE_CLAZZ_OBJECT::value ;
			if (tmp == mArrayClazzString)
				return NODE_CLAZZ_ARRAY::value ;
			if (tmp == mFinalClazzString)
				return NODE_CLAZZ_FINAL::value ;
			assume (tmp.empty ()) ;
			return NODE_CLAZZ_FINAL::value ;
		}

		void generate () {
			update_root_node () ;
			while (TRUE) {
				if (mNodeStack.empty ())
					break ;
				mNodeStack.pop (mTempNode) ;
				for (auto &&i : mTempNode.mBaseNode) {
					auto rxx = TRUE ;
					if ifswitch (rxx) {
						if (mTempNode.mClazz != NODE_CLAZZ_OBJECT::value)
							discard ;
						update_found_object_node (i) ;
					}
					if ifswitch (rxx) {
						if (mTempNode.mClazz != NODE_CLAZZ_ARRAY::value)
							discard ;
						update_found_array_node (i) ;
					}
				}
				update_merge_found_node (mTempNode.mParent) ;
				mBaseNodeQueue.add (move (mTempNode.mBaseNode)) ;
			}
		}

		void update_root_node () {
			INDEX ix = find_exist_node () ;
			if (ix == NONE)
				return ;
			INDEX jx = mNodeStack.insert () ;
			mNodeStack[jx].mBaseNode = List<XmlParser> (mSequence.length ()) ;
			for (auto &&i : mSequence) {
				if ifnot (i.exist ())
					continue ;
				mNodeStack[jx].mBaseNode.add (i) ;
			}
			mNodeStack[jx].mClazz = NODE_CLAZZ_OBJECT::value ;
			mNodeStack[jx].mParent = mRoot ;
		}

		INDEX find_exist_node () const {
			for (auto &&i : mSequence.iter ()) {
				if (mSequence[i].exist ())
					return i ;
			}
			return NONE ;
		}

		void update_found_object_node (CREF<XmlParser> node) {
			auto rax = node ;
			while (TRUE) {
				if ifnot (rax.exist ())
					break ;
				const auto r1x = rax.name () ;
				const auto r2x = node_type (rax) ;
				INDEX ix = mFoundNodeSet.map (r1x) ;
				if ifswitch (TRUE) {
					if (ix == NONE)
						discard ;
					assume (mFoundNode[ix].mClazz == r2x) ;
					assume (r2x != NODE_CLAZZ_FINAL::value) ;
				}
				INDEX iy = ix ;
				if ifswitch (TRUE) {
					if (ix != NONE)
						discard ;
					iy = mFoundNode.insert () ;
					mFoundNodeSet.add (r1x ,iy) ;
					mFoundNode[iy].mName = r1x ;
					mFoundNode[iy].mClazz = r2x ;
					mFoundNode[iy].mAttributeSet = mAttributeSet.share () ;
					if ifswitch (TRUE) {
						if ifnot (mBaseNodeQueue.empty ())
							discard ;
						mBaseNodeQueue.add (List<XmlParser> ()) ;
					}
					mBaseNodeQueue.take (mFoundNode[iy].mBaseNode) ;
					mFoundNode[iy].mBaseNode.clear () ;
				}
				if ifswitch (TRUE) {
					auto &&tmp = rax.mHeap->mTree[rax.mIndex].mAttributeSet ;
					for (auto &&i : tmp.iter ()) {
						INDEX jx = tmp.get (i) ;
						INDEX jy = mFoundNode[ix].mAttributeSet.map (tmp[i]) ;
						if ifswitch (TRUE) {
							if (jy != NONE)
								discard ;
							jy = mAttribute.insert () ;
							mAttribute[jy] = rax.mHeap->mAttribute[jx] ;
							mFoundNode[ix].mAttributeSet.add (tmp[i] ,jy) ;
						}
					}
				}
				mFoundNode[iy].mBaseNode.add (rax.child ()) ;
				rax = rax.brother () ;
			}
		}

		void update_found_array_node (CREF<XmlParser> node) {
			auto rax = node ;
			while (TRUE) {
				if ifnot (rax.exist ())
					break ;
				const auto r1x = rax.name () ;
				const auto r2x = node_type (rax) ;
				INDEX ix = mFoundNode.head () ;
				if ifswitch (TRUE) {
					if (ix == NONE)
						discard ;
					assume (mFoundNode[ix].mName == r1x) ;
					assume (r2x != NODE_CLAZZ_FINAL::value) ;
				}
				INDEX iy = mFoundNode.insert () ;
				mFoundNodeSet.add (r1x ,iy) ;
				mFoundNode[iy].mName = r1x ;
				mFoundNode[iy].mClazz = r2x ;
				mFoundNode[iy].mAttributeSet = mAttributeSet.share () ;
				if ifswitch (TRUE) {
					auto &&tmp = rax.mHeap->mTree[rax.mIndex].mAttributeSet ;
					for (auto &&i : tmp.iter ()) {
						INDEX jx = tmp.get (i) ;
						INDEX jy = mAttribute.insert () ;
						mAttribute[jy] = rax.mHeap->mAttribute[jx] ;
						mFoundNode[ix].mAttributeSet.add (tmp[i] ,jy) ;
					}
				}
				if ifswitch (TRUE) {
					if ifnot (mBaseNodeQueue.empty ())
						discard ;
					mBaseNodeQueue.add (List<XmlParser> ()) ;
				}
				mBaseNodeQueue.take (mFoundNode[iy].mBaseNode) ;
				mFoundNode[iy].mBaseNode.clear () ;
				mFoundNode[iy].mBaseNode.add (rax.child ()) ;
				rax = rax.brother () ;
			}
		}

		void update_merge_found_node (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			for (auto &&i : mFoundNode) {
				INDEX iy = ix ;
				ix = mTree.insert () ;
				mTree[ix].mName = move (i.mName) ;
				mTree[ix].mAttributeSet = move (i.mAttributeSet) ;
				mTree[ix].mParent = curr ;
				if ifswitch (TRUE) {
					INDEX jx = mTree[ix].mParent ;
					if (jx == NONE)
						discard ;
					replace (mTree[jx].mChild ,NONE ,ix) ;
					if ifswitch (TRUE) {
						if (mTree[jx].mArraySet.size () > 0)
							discard ;
						mTree[jx].mArraySet = mArraySet.share () ;
						mTree[jx].mObjectSet = mObjectSet.share () ;
					}
					const auto r1x = mTree[jx].mArraySet.length () ;
					mTree[jx].mArraySet.add (r1x ,ix) ;
					mTree[jx].mObjectSet.add (mTree[ix].mName ,ix) ;
				}
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
				if ifswitch (TRUE) {
					if (iy == NONE)
						discard ;
					mTree[iy].mBrother = ix ;
				}
				INDEX jy = mNodeStack.insert () ;
				mNodeStack[jy].mBaseNode = move (i.mBaseNode) ;
				mNodeStack[jy].mClazz = i.mClazz ;
				mNodeStack[jy].mParent = ix ;
			}
			mFoundNodeSet.clear () ;
			mFoundNode.clear () ;
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
		Array<NODE> mTree ;
	} ;

	using NODE_CLAZZ_NULL = RANK0 ;
	using NODE_CLAZZ_STRING = RANK1 ;
	using NODE_CLAZZ_ARRAY = RANK2 ;
	using NODE_CLAZZ_OBJECT = RANK3 ;

	class JsonParser {
	protected:
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
			auto rax = R1X (RegBuffer<STRU8>::from (item ,0 ,item.size ())) ;
			rax.generate () ;
			return rax.poll () ;
		}

		imports JsonParser make (CREF<VarBuffer<STRU8>> item) {
			return make (RegBuffer<STRU8>::from (item)) ;
		}

		imports JsonParser make (CREF<ConBuffer<STRU8>> item) {
			return make (RegBuffer<STRU8>::from (item)) ;
		}

		imports JsonParser make (CREF<RegBuffer<BYTE>> item) {
			using R1X = typename JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS>::Serialization ;
			auto rax = R1X (RegBuffer<STRU8>::from (item ,0 ,item.size ())) ;
			rax.generate () ;
			return rax.poll () ;
		}

		imports JsonParser make (CREF<VarBuffer<BYTE>> item) {
			return make (RegBuffer<BYTE>::from (item)) ;
		}

		imports JsonParser make (CREF<ConBuffer<BYTE>> item) {
			return make (RegBuffer<BYTE>::from (item)) ;
		}

		BOOL exist () const {
			if (mHeap == NULL)
				return FALSE ;
			if (mIndex == NONE)
				return FALSE ;
			if (mHeap->mTree[mIndex].mClazz == NODE_CLAZZ_NULL::value)
				return FALSE ;
			return TRUE ;
		}

		BOOL string_type () const {
			return mHeap->mTree[mIndex].mClazz == NODE_CLAZZ_STRING::value ;
		}

		BOOL array_type () const {
			return mHeap->mTree[mIndex].mClazz == NODE_CLAZZ_ARRAY::value ;
		}

		BOOL object_type () const {
			return mHeap->mTree[mIndex].mClazz == NODE_CLAZZ_OBJECT::value ;
		}

		JsonParser root () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,0) ;
		}

		JsonParser parent () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,mHeap->mTree[mIndex].mParent) ;
		}

		JsonParser brother () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,mHeap->mTree[mIndex].mBrother) ;
		}

		JsonParser child () const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			return JsonParser (mHeap ,mHeap->mTree[mIndex].mChild) ;
		}

		JsonParser child (CREF<Slice<STRA>> name) const {
			return child (String<STRU8>::make (name)) ;
		}

		JsonParser child (CREF<Slice<STRW>> name) const {
			return child (String<STRU8>::make (name)) ;
		}

		JsonParser child (CREF<String<STRU8>> name) const {
			if ifnot (exist ())
				return JsonParser (mHeap ,NONE) ;
			if ifnot (object_type ())
				return JsonParser (mHeap ,NONE) ;
			INDEX ix = mHeap->mTree[mIndex].mObjectSet.map (name) ;
			return JsonParser (mHeap ,ix) ;
		}

		JsonParser child (CREF<String<STRU16>> name) const {
			return child (string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU16>>::id] (name)) ;
		}

		JsonParser child (CREF<String<STRU32>> name) const {
			return child (string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU32>>::id] (name)) ;
		}

		Array<JsonParser> child_array () const {
			Array<JsonParser> ret ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				if ifnot (array_type ())
					discard ;
				const auto r1x = Array<INDEX>::make (mHeap->mTree[mIndex].mArraySet.iter ()) ;
				ret = Array<JsonParser> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = JsonParser (mHeap ,mHeap->mTree[mIndex].mArraySet.get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<JsonParser> child_array (CREF<LENGTH> size_) const {
			auto rax = ArrayList<JsonParser> (size_) ;
			if ifswitch (TRUE) {
				if ifnot (exist ())
					discard ;
				for (auto &&i : mHeap->mTree[mIndex].mArraySet.iter ()) {
					if (rax.length () >= size_)
						continue ;
					INDEX ix = rax.insert () ;
					rax[ix] = JsonParser (mHeap ,mHeap->mTree[mIndex].mArraySet.get (i)) ;
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
			if (address (mHeap->mTree) != address (that.mHeap->mTree))
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

		CREF<String<STRU8>> fetch () const leftvalue {
			assume (exist ()) ;
			assume (string_type ()) ;
			return mHeap->mTree[mIndex].mValue ;
		}

		template <class ARG1 ,class ARG2>
		ARG1 fetch (CREF<ARG1> def ,CREF<ARG2> cvt) const {
			using R1X = ARG1 ;
			auto rax = Optional<R1X> () ;
			try_invoke ([&] () {
				rax = Optional<R1X>::make (cvt (fetch ())) ;
			} ,[&] () {
				rax = Optional<R1X>::make (def) ;
			} ,[&] () {
				noop () ;
			}) ;
			return rax.poll () ;
		}

		BOOL fetch (CREF<BOOL> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<BOOL ,STRU8>>::id]) ;
		}

		VAL32 fetch (CREF<VAL32> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<VAL32 ,STRU8>>::id]) ;
		}

		VAL64 fetch (CREF<VAL64> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id]) ;
		}

		SINGLE fetch (CREF<SINGLE> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<SINGLE ,STRU8>>::id]) ;
		}

		DOUBLE fetch (CREF<DOUBLE> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<DOUBLE ,STRU8>>::id]) ;
		}

		BYTE fetch (CREF<BYTE> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<BYTE ,STRU8>>::id]) ;
		}

		WORD fetch (CREF<WORD> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<WORD ,STRU8>>::id]) ;
		}

		CHAR fetch (CREF<CHAR> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<CHAR ,STRU8>>::id]) ;
		}

		DATA fetch (CREF<DATA> def) const {
			return fetch (def ,string_parse[TYPEAS<TYPEAS<DATA ,STRU8>>::id]) ;
		}

		String<STRA> fetch (CREF<String<STRA>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRA ,STRU8>>::id]) ;
		}

		String<STRW> fetch (CREF<String<STRW>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRW ,STRU8>>::id]) ;
		}

		String<STRU8> fetch (CREF<String<STRU8>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRU8 ,STRU8>>::id]) ;
		}

		String<STRU16> fetch (CREF<String<STRU16>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRU16 ,STRU8>>::id]) ;
		}

		String<STRU32> fetch (CREF<String<STRU32>> def) const {
			return fetch (def ,string_cvt[TYPEAS<TYPEAS<STRU32 ,STRU8>>::id]) ;
		}
	} ;
} ;

template <class DEPEND>
trait JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::HEAP ;
	using JsonParser = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::JsonParser ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	using NODE_CLAZZ_NULL = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_NULL ;
	using NODE_CLAZZ_STRING = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_STRING ;
	using NODE_CLAZZ_ARRAY = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_ARRAY ;
	using NODE_CLAZZ_OBJECT = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ_OBJECT ;

	class Serialization extend JsonParser {
	protected:
		LENGTH mRecursiveCounter ;
		TextReader<STRU8> mTextReader ;
		RegularReader mReader ;
		List<NODE> mTree ;
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

		JsonParser poll () {
			auto &&tmp = keep[TYPEAS<VREF<JsonParser>>::id] (thiz) ;
			auto rax = HEAP () ;
			const auto r1x = shrink_order () ;
			rax.mTree = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mTree.iter ()) {
				INDEX ix = r1x[i] ;
				rax.mTree[ix].mName = move (mTree[i].mName) ;
				rax.mTree[ix].mAttributeSet = move (mTree[i].mAttributeSet) ;
				rax.mTree[ix].mArraySet = move (mTree[i].mArraySet) ;
				for (auto &&j : rax.mTree[ix].mArraySet.iter ()) {
					INDEX iy = r1x[rax.mTree[ix].mArraySet.get (j)] ;
					rax.mTree[ix].mArraySet.set (j ,iy) ;
				}
				rax.mTree[ix].mObjectSet = move (mTree[i].mObjectSet) ;
				for (auto &&j : rax.mTree[ix].mObjectSet.iter ()) {
					INDEX iy = r1x[rax.mTree[ix].mObjectSet.get (j)] ;
					rax.mTree[ix].mObjectSet.set (j ,iy) ;
				}
				rax.mTree[ix].mClazz = mTree[i].mClazz ;
				rax.mTree[ix].mParent = r1x[mTree[i].mParent] ;
				rax.mTree[ix].mBrother = r1x[mTree[i].mBrother] ;
				rax.mTree[ix].mChild = r1x[mTree[i].mChild] ;
			}
			tmp.mHeap = CRef<HEAP>::make (move (rax)) ;
			tmp.mIndex = 0 ;
			return move (tmp) ;
		}

		Array<INDEX> shrink_order () const {
			Array<INDEX> ret = Array<INDEX> (mTree.size ()) ;
			ret.fill (NONE) ;
			INDEX ix = 0 ;
			for (auto &&i : mTree.iter ()) {
				ret[i] = ix ;
				ix++ ;
			}
			return move (ret) ;
		}

		void generate () {
			/*
			*	$0->$11 $10 $12
			*	$1->${scalar}
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
			mReader >> RegularReader::SKIP_SPACE ;
			update_shift_e10 () ;
			INDEX ix = mLastIndex ;
			mReader >> RegularReader::SKIP_SPACE ;
			update_shift_e12 () ;
			mLastIndex = ix ;
		}

		//@info: $1->${scalar}
		void update_shift_e1 () {
			mReader >> RegularReader::HINT_SCALAR >> mLastString ;
		}

		//@info: $2->true|TRUE|false|FALSE
		void update_shift_e2 () {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('t'))
					discard ;
				mReader >> slice ("true") ;
				mLastString = slice ("true") ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('T'))
					discard ;
				mReader >> slice ("TRUE") ;
				mLastString = slice ("TRUE") ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('f'))
					discard ;
				mReader >> slice ("false") ;
				mLastString = slice ("false") ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('F'))
					discard ;
				mReader >> slice ("FALSE") ;
				mLastString = slice ("FALSE") ;
			}
			if ifswitch (rxx) {
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
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::value) ;
			INDEX ix = NONE ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (is_first_number ())
					discard ;
				ix = mTree.insert () ;
				update_shift_e1 () ;
				mTree[ix].mValue = move (mLastString) ;
				mTree[ix].mClazz = NODE_CLAZZ_STRING::value ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (rxx) {
				if ifnot (is_first_boolean ())
					discard ;
				ix = mTree.insert () ;
				update_shift_e2 () ;
				mTree[ix].mValue = move (mLastString) ;
				mTree[ix].mClazz = NODE_CLAZZ_STRING::value ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('n'))
					discard ;
				ix = mTree.insert () ;
				update_shift_e2x () ;
				mTree[ix].mClazz = NODE_CLAZZ_NULL::value ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('\"'))
					discard ;
				ix = mTree.insert () ;
				update_shift_e3 () ;
				mTree[ix].mValue = move (mLastString) ;
				mTree[ix].mClazz = NODE_CLAZZ_STRING::value ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('['))
					discard ;
				update_shift_e6 (curr) ;
				ix = mLastIndex ;
			}
			if ifswitch (rxx) {
				if ifnot (mReader[0] == STRU8 ('{'))
					discard ;
				update_shift_e9 (curr) ;
				ix = mLastIndex ;
			}
			if ifswitch (rxx) {
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
				const auto r1x = mTree[curr].mArraySet.length () ;
				mTree[curr].mArraySet.add (r1x ,mLastIndex) ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (ix == NONE)
						discard ;
					ix = mLastIndex ;
				}
				if ifswitch (rxx) {
					mTree[iy].mBrother = mLastIndex ;
				}
				iy = mLastIndex ;
				mReader >> RegularReader::SKIP_SPACE ;
				if (mReader[0] != STRU8 (','))
					break ;
				mReader++ ;
				mReader >> RegularReader::SKIP_SPACE ;
			}
			mLastIndex = ix ;
		}

		//@info: $6->[ ]|[ $5 ]
		void update_shift_e6 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::value) ;
			mReader >> slice ("[") ;
			INDEX ix = mTree.insert () ;
			mTree[ix].mArraySet = mArraySet.share () ;
			mTree[ix].mClazz = NODE_CLAZZ_ARRAY::value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
			mReader >> RegularReader::SKIP_SPACE ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 (']'))
					discard ;
				update_shift_e5 (ix) ;
				mTree[ix].mChild = mLastIndex ;
				mReader >> RegularReader::SKIP_SPACE ;
			}
			mReader >> slice ("]") ;
			mLastIndex = ix ;
		}

		//@info: $7->$3 : $4
		void update_shift_e7 (CREF<INDEX> curr) {
			update_shift_e3 () ;
			auto rax = move (mLastString) ;
			mReader >> RegularReader::SKIP_SPACE ;
			mReader >> slice (":") ;
			mReader >> RegularReader::SKIP_SPACE ;
			update_shift_e4 (curr) ;
			mTree[curr].mObjectSet.add (move (rax) ,mLastIndex) ;
		}

		//@info: $8->$7|$7 , $8
		void update_shift_e8 (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				update_shift_e7 (curr) ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (ix == NONE)
						discard ;
					ix = mLastIndex ;
				}
				if ifswitch (rxx) {
					mTree[iy].mBrother = mLastIndex ;
				}
				iy = mLastIndex ;
				mReader >> RegularReader::SKIP_SPACE ;
				if (mReader[0] != STRU8 (','))
					break ;
				mReader++ ;
				mReader >> RegularReader::SKIP_SPACE ;
			}
			mLastIndex = ix ;
		}

		//@info: $9->{ }|{ $8 }
		void update_shift_e9 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::value) ;
			mReader >> slice ("{") ;
			INDEX ix = mTree.insert () ;
			mTree[ix].mObjectSet = mObjectSet.share () ;
			mTree[ix].mClazz = NODE_CLAZZ_OBJECT::value ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
			mReader >> RegularReader::SKIP_SPACE ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('}'))
					discard ;
				update_shift_e8 (ix) ;
				mTree[ix].mChild = mLastIndex ;
				mReader >> RegularReader::SKIP_SPACE ;
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