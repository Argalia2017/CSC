#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_SERIALIZATION__
#error "∑(っ°Д° ;)っ : require 'csc_serialization.hpp'"
#endif

#include "csc_serialization.hpp"

namespace CSC {
template <class...>
trait XMLPARSER_SERIALIZATION_HELP ;

template <class...>
trait XMLPARSER_COMBINATION_HELP ;

template <class DEPEND>
trait XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename XMLPARSER_HELP<DEPEND ,ALWAYS>::Layout ;

	struct NODE {
		String<STRU8> mName ;
		SoftSet<String<STRU8>> mMemberSet ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mParent ;
		INDEX mBrother ;
		INDEX mChild ;
	} ;

	struct HEAP {
		Array<NODE> mTree ;
		Array<String<STRU8>> mMember ;
	} ;

	class ImplHolder implement Holder {
	protected:
		CRef<HEAP> mHeap ;
		INDEX mIndex ;

	public:
		void initialize () override {
			mIndex = NONE ;
		}

		void initialize (CREF<RegBuffer<STRU8>> stream) override {
			using R1X = typename DEPENDENT<XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> ,DEPEND>::Serialization ;
			auto rax = R1X (stream.borrow ()) ;
			rax.generate () ;
			mHeap = CRef<HEAP>::make (rax.poll ()) ;
			mIndex = 0 ;
		}

		CRef<HEAP> get_mHeap () const {
			return mHeap ;
		}

		INDEX get_mIndex () const {
			return mIndex ;
		}

		Layout clone () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mIndex) ;
		}

		Layout factory (CREF<INDEX> index) const {
			Layout ret ;
			auto rax = VRef<ImplHolder>::make () ;
			rax->mHeap = mHeap ;
			rax->mIndex = index ;
			ret.mThis = move (rax) ;
			return move (ret) ;
		}

		BOOL good () const override {
			if (mHeap == NULL)
				return FALSE ;
			if (mIndex == NONE)
				return FALSE ;
			return TRUE ;
		}

		Layout root () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (0) ;
		}

		Layout parent () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mHeap->mTree[mIndex].mParent) ;
		}

		Layout brother () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mHeap->mTree[mIndex].mBrother) ;
		}

		Layout child () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mHeap->mTree[mIndex].mChild) ;
		}

		Layout child (CREF<String<STRU8>> name) const override {
			if ifnot (good ())
				return factory (NONE) ;
			INDEX ix = mHeap->mTree[mIndex].mObjectSet.map (name) ;
			return factory (ix) ;
		}

		Array<Layout> children () const override {
			Array<Layout> ret ;
			if ifswitch (TRUE) {
				if ifnot (good ())
					discard ;
				auto &&tmp = mHeap->mTree[mIndex].mArraySet ;
				const auto r1x = IterArray<INDEX>::make (tmp.iter ()) ;
				ret = Array<Layout> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = factory (tmp.map_get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<Layout> children (CREF<LENGTH> size_) const override {
			Array<Layout> ret = Array<Layout> (size_) ;
			if ifswitch (TRUE) {
				if ifnot (good ())
					discard ;
				auto &&tmp = mHeap->mTree[mIndex].mArraySet ;
				const auto r1x = IterArray<INDEX>::make (tmp.iter ()) ;
				for (auto &&i : ret.iter ()) {
					if (i >= r1x.size ())
						break ;
					ret[i] = factory (tmp.map_get (r1x[i])) ;
				}
			}
			return move (ret) ;
		}

		Layout concat (CREF<Layout> that) const override {
			using R1X = typename DEPENDENT<XMLPARSER_COMBINATION_HELP<DEPEND ,ALWAYS> ,DEPEND>::Combination ;
			auto rax = R1X (factory (mIndex) ,XmlParser (Layout ())) ;
			rax.generate () ;
			auto rbx = ImplHolder () ;
			rbx.mHeap = CRef<HEAP>::make (rax.poll ()) ;
			rbx.mIndex = 0 ;
			return rbx.factory (0) ;
		}

		BOOL equal (CREF<Layout> that) const override {
			return equal (keep[TYPEAS<CREF<ImplHolder>>::expr] (that.mThis.self)) ;
		}

		BOOL equal (CREF<ImplHolder> that) const {
			if (good () != that.good ())
				return FALSE ;
			if ifnot (good ())
				return TRUE ;
			if (address (mHeap->mTree) != address (that.mHeap->mTree))
				return FALSE ;
			if (mIndex != that.mIndex)
				return FALSE ;
			return TRUE ;
		}

		CREF<String<STRU8>> member () const leftvalue override {
			if ifnot (good ())
				return String<STRU8>::zero () ;
			return mHeap->mTree[mIndex].mName ;
		}

		CREF<String<STRU8>> member (CREF<String<STRU8>> tag) const leftvalue override {
			if ifnot (good ())
				return String<STRU8>::zero () ;
			INDEX ix = mHeap->mTree[mIndex].mMemberSet.map (tag) ;
			if (ix == NONE)
				return String<STRU8>::zero () ;
			return mHeap->mMember[ix] ;
		}

		CREF<String<STRU8>> fetch () const leftvalue override {
			assume (good ()) ;
			assume (mHeap->mTree[mIndex].mArraySet.size () == 0) ;
			assume (mHeap->mTree[mIndex].mObjectSet.size () == 0) ;
			assume (mHeap->mTree[mIndex].mMemberSet.length () == 1) ;
			return mHeap->mMember[0] ;
		}

		BOOL fetch (CREF<BOOL> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<BOOL ,STRU8>::expr]) ;
		}

		VAL32 fetch (CREF<VAL32> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<VAL32 ,STRU8>::expr]) ;
		}

		VAL64 fetch (CREF<VAL64> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<VAL64 ,STRU8>::expr]) ;
		}

		SINGLE fetch (CREF<SINGLE> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<SINGLE ,STRU8>::expr]) ;
		}

		DOUBLE fetch (CREF<DOUBLE> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<DOUBLE ,STRU8>::expr]) ;
		}

		BYTE fetch (CREF<BYTE> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<BYTE ,STRU8>::expr]) ;
		}

		WORD fetch (CREF<WORD> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<WORD ,STRU8>::expr]) ;
		}

		CHAR fetch (CREF<CHAR> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<CHAR ,STRU8>::expr]) ;
		}

		DATA fetch (CREF<DATA> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<DATA ,STRU8>::expr]) ;
		}

		String<STRA> fetch (CREF<String<STRA>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRA ,STRU8>::expr]) ;
		}

		String<STRW> fetch (CREF<String<STRW>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRW ,STRU8>::expr]) ;
		}

		String<STRU8> fetch (CREF<String<STRU8>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRU8 ,STRU8>::expr]) ;
		}

		String<STRU16> fetch (CREF<String<STRU16>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRU16 ,STRU8>::expr]) ;
		}

		String<STRU32> fetch (CREF<String<STRU32>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRU32 ,STRU8>::expr]) ;
		}

		template <class ARG1 ,class ARG2>
		forceinline ARG1 template_fetch (CREF<ARG1> def ,CREF<ARG2> cvt) const {
			auto rax = Optional<ARG1> () ;
			try_invoke ([&] () {
				rax = Optional<ARG1>::make (cvt (fetch ())) ;
			} ,[&] () {
				rax = Optional<ARG1>::make (def) ;
			} ,[&] () {
				noop () ;
			}) ;
			return rax.poll () ;
		}
	} ;
} ;

template <class DEPEND>
trait XMLPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,256> ;

	class Serialization {
	protected:
		LENGTH mRecursiveCounter ;
		TextReader<STRU8> mTextReader ;
		TextAttribute<STRU8> mAttribute ;
		RegularReader mReader ;
		List<NODE> mTree ;
		ArrayList<String<STRU8>> mMember ;
		SoftSet<String<STRU8>> mMemberSet ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mLastIndex ;
		String<STRU8> mLastString ;

	public:
		implicit Serialization () = delete ;

		explicit Serialization (RREF<CRef<RegBuffer<STRU8>>> stream) {
			mRecursiveCounter = 0 ;
			mTextReader = TextReader<STRU8> (move (stream)) ;
			mAttribute = mTextReader.attribute () ;
			mReader = RegularReader (VRef<TextReader<STRU8>>::reference (mTextReader) ,3) ;
		}

		HEAP poll () {
			HEAP ret ;
			const auto r1x = shrink_order () ;
			ret.mTree = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mTree.iter ()) {
				INDEX ix = r1x.map (i) ;
				ret.mTree[ix].mName = move (mTree[i].mName) ;
				ret.mTree[ix].mMemberSet = move (mTree[i].mMemberSet) ;
				ret.mTree[ix].mMemberSet.order () ;
				ret.mTree[ix].mArraySet = move (mTree[i].mArraySet) ;
				for (auto &&j : ret.mTree[ix].mArraySet.iter ()) {
					INDEX iy = ret.mTree[ix].mArraySet.map_get (j) ;
					ret.mTree[ix].mArraySet.map_set (j ,r1x.map (iy)) ;
				}
				ret.mTree[ix].mArraySet.order () ;
				ret.mTree[ix].mObjectSet = move (mTree[i].mObjectSet) ;
				for (auto &&j : ret.mTree[ix].mObjectSet.iter ()) {
					INDEX iy = ret.mTree[ix].mObjectSet.map_get (j) ;
					ret.mTree[ix].mObjectSet.map_set (j ,r1x.map (iy)) ;
				}
				ret.mTree[ix].mObjectSet.order () ;
				ret.mTree[ix].mParent = r1x.map (mTree[i].mParent) ;
				ret.mTree[ix].mBrother = r1x.map (mTree[i].mBrother) ;
				ret.mTree[ix].mChild = r1x.map (mTree[i].mChild) ;
			}
			ret.mMember = Array<String<STRU8>> (mMember.length ()) ;
			for (auto &&i : mMember.iter ()) {
				assume (vbetween (i ,0 ,ret.mMember.length ())) ;
				ret.mMember[i] = move (mMember[i]) ;
			}
			return move (ret) ;
		}

		Set<INDEX> shrink_order () const {
			Set<INDEX> ret = Set<INDEX> (mTree.length ()) ;
			for (auto &&i : mTree.iter ())
				ret.add (i ,ret.length ()) ;
			return move (ret) ;
		}

		void generate () {
			/*
			*	$0->$8 $7 $9
			*	$1->${identifier}
			*	$2->"${string}"
			*	$3->$1 = $2
			*	$4->${eps}|$3 $4
			*	$5-><$1 $4 />|<$1 $4 > $7 </$1 >
			*	$6-><!--comment-->
			*	$7->${eps}|$5 $7|$6 $7
			*	$8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
			*	$9->${end}
			*/
			read_shift_e0 () ;
			assume (mLastIndex == 0) ;
		}

		void read_shift_e0 () {
			read_shift_e8 () ;
			mReader >> SKIP_GAP ;
			INDEX ix = mTree.insert () ;
			mTree[ix].mArraySet = mArraySet.share () ;
			mTree[ix].mObjectSet = mObjectSet.share () ;
			mTree[ix].mParent = NONE ;
			read_shift_e7 (ix) ;
			mTree[ix].mChild = mLastIndex ;
			mTree[ix].mBrother = NONE ;
			mReader >> SKIP_GAP ;
			read_shift_e9 () ;
			mLastIndex = ix ;
		}

		//@info: $1->${identifier}
		void read_shift_e1 () {
			mReader >> HINT_WORD >> mLastString ;
		}

		//@info: $2->"${string}"
		void read_shift_e2 () {
			mReader >> HINT_STRING >> mLastString ;
		}

		//@info: $3->$1 = $2
		void read_shift_e3 (CREF<INDEX> curr) {
			read_shift_e1 () ;
			INDEX ix = mTree[curr].mMemberSet.map (mLastString) ;
			assume (ix == NONE) ;
			ix = mMember.insert () ;
			mTree[curr].mMemberSet.add (move (mLastString) ,ix) ;
			mReader >> SKIP_GAP ;
			mReader >> slice ("=") ;
			mReader >> SKIP_GAP ;
			read_shift_e2 () ;
			mMember[ix] = move (mLastString) ;
		}

		//@info: $4->${eps}|$3 $4
		void read_shift_e4 (CREF<INDEX> curr) {
			while (TRUE) {
				if ifnot (is_frist_identifier ())
					break ;
				read_shift_e3 (curr) ;
				mReader >> SKIP_GAP ;
			}
		}

		BOOL is_frist_identifier () const {
			if (mAttribute.is_word (mReader[0]))
				return TRUE ;
			return FALSE ;
		}

		//@info: $5-><$1 $4 />|<$1 $4 > $7 </$1 >
		void read_shift_e5 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::expr) ;
			mReader >> slice ("<") ;
			INDEX ix = mTree.insert () ;
			read_shift_e1 () ;
			mTree[ix].mName = move (mLastString) ;
			mTree[ix].mMemberSet = mMemberSet.share () ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
			mReader >> SKIP_GAP ;
			read_shift_e4 (ix) ;
			mReader >> SKIP_GAP ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (mReader[0] != STRU8 ('>'))
					discard ;
				mReader++ ;
				mReader >> SKIP_GAP ;
				mTree[ix].mArraySet = mArraySet.share () ;
				mTree[ix].mObjectSet = mObjectSet.share () ;
				read_shift_e7 (ix) ;
				mTree[ix].mChild = mLastIndex ;
				mReader >> SKIP_GAP ;
				mReader >> slice ("</") ;
				read_shift_e1 () ;
				assume (mTree[ix].mName == mLastString) ;
				mReader >> SKIP_GAP ;
				mReader >> slice (">") ;
			}
			if ifswitch (act) {
				mReader >> slice ("/>") ;
			}
			mLastIndex = ix ;
		}

		//@info: $6-><!--${comment}-->
		void read_shift_e6 () {
			mReader >> slice ("<!--") ;
			while (TRUE) {
				if (mReader[0] == STRU8 (0))
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
		void read_shift_e7 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::expr) ;
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				const auto r1x = is_first_of_comment () ;
				const auto r2x = is_first_of_object () ;
				if ifnot (r1x)
					if ifnot (r2x)
						break ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (r1x)
						discard ;
					read_shift_e6 () ;
				}
				if ifswitch (act) {
					if ifnot (r2x)
						discard ;
					if (ix != NONE)
						discard ;
					read_shift_e5 (curr) ;
					const auto r3x = mTree[curr].mArraySet.length () ;
					mTree[curr].mArraySet.add (r3x ,mLastIndex) ;
					mTree[curr].mObjectSet.add (mTree[mLastIndex].mName ,mLastIndex) ;
					ix = mLastIndex ;
					iy = mLastIndex ;
				}
				if ifswitch (act) {
					if ifnot (r2x)
						discard ;
					read_shift_e5 (curr) ;
					const auto r4x = mTree[curr].mArraySet.length () ;
					mTree[curr].mArraySet.add (r4x ,mLastIndex) ;
					mTree[curr].mObjectSet.add (mTree[mLastIndex].mName ,mLastIndex) ;
					mTree[iy].mBrother = mLastIndex ;
					iy = mLastIndex ;
				}
				mReader >> SKIP_GAP ;
			}
			mLastIndex = ix ;
		}

		BOOL is_first_of_comment () const {
			if (mReader[0] == STRU8 ('<'))
				if (mReader[1] == STRU8 ('!'))
					return TRUE ;
			return FALSE ;
		}

		BOOL is_first_of_object () const {
			if (mReader[0] == STRU8 ('<'))
				if (mReader[1] != STRU8 ('/'))
					return TRUE ;
			return FALSE ;
		}

		//@info: $8->${eps}|<?xml version = "1.0" ?>|<?xml version = "1.0" encoding = "utf-8" ?>
		void read_shift_e8 () {
			if (mReader[0] != STRU8 ('<'))
				return ;
			if (mReader[1] != STRU8 ('?'))
				return ;
			mReader++ ;
			mReader++ ;
			mReader >> slice ("xml") ;
			mReader >> SKIP_GAP ;
			mReader >> slice ("version") ;
			mReader >> SKIP_GAP ;
			mReader >> slice ("=") ;
			mReader >> SKIP_GAP ;
			mReader >> slice ("\"1.0\"") ;
			mReader >> SKIP_GAP ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('?'))
					discard ;
				mReader >> slice ("encoding") ;
				mReader >> SKIP_GAP ;
				mReader >> slice ("=") ;
				mReader >> SKIP_GAP ;
				mReader >> slice ("\"utf-8\"") ;
				mReader >> SKIP_GAP ;
			}
			mReader >> slice ("?>") ;
		}

		//@info: $9->${end}
		void read_shift_e9 () {
			assume (mReader[0] == STRU8 (0)) ;
		}
	} ;
} ;

template <class DEPEND>
trait XMLPARSER_COMBINATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
	using Holder = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::Layout ;
	using ImplHolder = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;

	struct NODE_CLAZZ {
		enum {
			Null ,
			String ,
			Array ,
			Object ,
			Final ,
			EnumSize
		} ;
	} ;

	struct FOUND_NODE {
		String<STRU8> mName ;
		SoftSet<String<STRU8>> mMemberSet ;
		FLAG mClazz ;
		List<XmlParser> mBaseNode ;
	} ;

	struct STACK_NODE {
		INDEX mParent ;
		FLAG mClazz ;
		List<XmlParser> mBaseNode ;
	} ;

	class Combination {
	protected:
		Array<XmlParser> mSequence ;
		String<STRU8> mTypeString ;
		String<STRU8> mObjectClazzString ;
		String<STRU8> mArrayClazzString ;
		String<STRU8> mFinalClazzString ;
		List<NODE> mTree ;
		ArrayList<String<STRU8>> mMember ;
		SoftSet<String<STRU8>> mMemberSet ;
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

		explicit Combination (CREF<XmlParser> first ,CREF<XmlParser> second) {
			mSequence = Array<XmlParser> (2) ;
			mSequence[0] = first ;
			mSequence[1] = second ;
			mTypeString = string_cvt[TYPEAS<STRU8 ,STR>::expr] (slice ("type")) ;
			mObjectClazzString = string_cvt[TYPEAS<STRU8 ,STR>::expr] (slice ("object")) ;
			mArrayClazzString = string_cvt[TYPEAS<STRU8 ,STR>::expr] (slice ("array")) ;
			mFinalClazzString = string_cvt[TYPEAS<STRU8 ,STR>::expr] (slice ("final")) ;
			mRoot = mTree.insert () ;
			mTree[mRoot].mArraySet = mArraySet.share () ;
			mTree[mRoot].mObjectSet = mObjectSet.share () ;
			mTree[mRoot].mParent = NONE ;
			mTree[mRoot].mChild = NONE ;
			mTree[mRoot].mBrother = NONE ;
		}

		HEAP poll () {
			HEAP ret ;
			const auto r1x = shrink_order () ;
			ret.mTree = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mTree.iter ()) {
				INDEX ix = r1x.map (i) ;
				ret.mTree[ix].mName = move (mTree[i].mName) ;
				ret.mTree[ix].mMemberSet = move (mTree[i].mMemberSet) ;
				ret.mTree[ix].mMemberSet.order () ;
				ret.mTree[ix].mArraySet = move (mTree[i].mArraySet) ;
				for (auto &&j : ret.mTree[ix].mArraySet.iter ()) {
					INDEX iy = ret.mTree[ix].mArraySet.map_get (j) ;
					ret.mTree[ix].mArraySet.map_set (j ,r1x.map (iy)) ;
				}
				ret.mTree[ix].mArraySet.order () ;
				ret.mTree[ix].mObjectSet = move (mTree[i].mObjectSet) ;
				for (auto &&j : ret.mTree[ix].mObjectSet.iter ()) {
					INDEX iy = ret.mTree[ix].mObjectSet.map_get (j) ;
					ret.mTree[ix].mObjectSet.map_set (j ,r1x.map (iy)) ;
				}
				ret.mTree[ix].mObjectSet.order () ;
				ret.mTree[ix].mParent = r1x.map (mTree[i].mParent) ;
				ret.mTree[ix].mBrother = r1x.map (mTree[i].mBrother) ;
				ret.mTree[ix].mChild = r1x.map (mTree[i].mChild) ;
			}
			ret.mMember = Array<String<STRU8>> (mMember.length ()) ;
			for (auto &&i : mMember.iter ()) {
				assume (vbetween (i ,0 ,ret.mMember.length ())) ;
				ret.mMember[i] = move (mMember[i]) ;
			}
			return move (ret) ;
		}

		Set<INDEX> shrink_order () const {
			Set<INDEX> ret = Set<INDEX> (mTree.length ()) ;
			ret.add (mRoot ,ret.length ()) ;
			for (auto &&i : mTree.iter ())
				ret.add (i ,ret.length ()) ;
			return move (ret) ;
		}

		FLAG node_type (CREF<XmlParser> node) const {
			auto &&tmp = node.member (mTypeString) ;
			if (tmp == mObjectClazzString)
				return NODE_CLAZZ::Object ;
			if (tmp == mArrayClazzString)
				return NODE_CLAZZ::Array ;
			if (tmp == mFinalClazzString)
				return NODE_CLAZZ::Final ;
			assume (tmp.empty ()) ;
			return NODE_CLAZZ::Final ;
		}

		void generate () {
			update_root_node () ;
			while (TRUE) {
				if (mNodeStack.empty ())
					break ;
				mNodeStack.pop (mTempNode) ;
				for (auto &&i : mTempNode.mBaseNode) {
					auto act = TRUE ;
					if ifswitch (act) {
						if (mTempNode.mClazz != NODE_CLAZZ::Object)
							discard ;
						update_found_object_node (i) ;
					}
					if ifswitch (act) {
						if (mTempNode.mClazz != NODE_CLAZZ::Array)
							discard ;
						update_found_array_node (i) ;
					}
				}
				update_merge_found_node (mTempNode.mParent) ;
				mBaseNodeQueue.add (move (mTempNode.mBaseNode)) ;
			}
		}

		void update_root_node () {
			INDEX ix = find_good_node () ;
			if (ix == NONE)
				return ;
			INDEX jx = mNodeStack.insert () ;
			mNodeStack[jx].mBaseNode = List<XmlParser> (mSequence.length ()) ;
			for (auto &&i : mSequence) {
				if ifnot (i.good ())
					continue ;
				mNodeStack[jx].mBaseNode.add (i) ;
			}
			mNodeStack[jx].mClazz = NODE_CLAZZ::Object ;
			mNodeStack[jx].mParent = mRoot ;
		}

		INDEX find_good_node () const {
			for (auto &&i : mSequence.iter ()) {
				if (mSequence[i].good ())
					return i ;
			}
			return NONE ;
		}

		void update_found_object_node (CREF<XmlParser> node) {
			auto rax = node ;
			while (TRUE) {
				if ifnot (rax.good ())
					break ;
				const auto r1x = rax.member () ;
				const auto r2x = node_type (rax) ;
				INDEX ix = mFoundNodeSet.map (r1x) ;
				if ifswitch (TRUE) {
					if (ix == NONE)
						discard ;
					assume (mFoundNode[ix].mClazz == r2x) ;
					assume (r2x != NODE_CLAZZ::Final) ;
				}
				INDEX iy = ix ;
				if ifswitch (TRUE) {
					if (iy != NONE)
						discard ;
					iy = mFoundNode.insert () ;
					mFoundNodeSet.add (r1x ,iy) ;
					mFoundNode[iy].mName = r1x ;
					mFoundNode[iy].mClazz = r2x ;
					mFoundNode[iy].mMemberSet = mMemberSet.share () ;
					if ifswitch (TRUE) {
						if ifnot (mBaseNodeQueue.empty ())
							discard ;
						mBaseNodeQueue.add (List<XmlParser> ()) ;
					}
					mBaseNodeQueue.take (mFoundNode[iy].mBaseNode) ;
					mFoundNode[iy].mBaseNode.clear () ;
				}
				if ifswitch (TRUE) {
					const auto r3x = down_cast (rax).get_mHeap () ;
					const auto r4x = down_cast (rax).get_mIndex () ;
					copy_object_member (iy ,r3x->mTree[r4x].mMemberSet ,r3x->mMember) ;
				}
				mFoundNode[iy].mBaseNode.add (rax.child ()) ;
				rax = rax.brother () ;
			}
		}

		void copy_object_member (CREF<INDEX> curr ,CREF<SoftSet<String<STRU8>>> set_ ,CREF<Array<String<STRU8>>> member) {
			for (auto &&i : set_.iter ()) {
				INDEX jx = set_.map_get (i) ;
				INDEX jy = mFoundNode[curr].mMemberSet.map (set_[i]) ;
				if ifswitch (TRUE) {
					if (jy != NONE)
						discard ;
					jy = mMember.insert () ;
					mMember[jy] = member[jx] ;
					mFoundNode[curr].mMemberSet.add (set_[i] ,jy) ;
				}
			}
		}

		void update_found_array_node (CREF<XmlParser> node) {
			auto rax = node ;
			while (TRUE) {
				if ifnot (rax.good ())
					break ;
				const auto r1x = rax.member () ;
				const auto r2x = node_type (rax) ;
				INDEX ix = mFoundNode.head () ;
				if ifswitch (TRUE) {
					if (ix == NONE)
						discard ;
					assume (mFoundNode[ix].mName == r1x) ;
					assume (r2x != NODE_CLAZZ::Final) ;
				}
				INDEX iy = mFoundNode.insert () ;
				mFoundNodeSet.add (r1x ,iy) ;
				mFoundNode[iy].mName = r1x ;
				mFoundNode[iy].mClazz = r2x ;
				mFoundNode[iy].mMemberSet = mMemberSet.share () ;
				if ifswitch (TRUE) {
					const auto r3x = down_cast (rax).get_mHeap () ;
					const auto r4x = down_cast (rax).get_mIndex () ;
					copy_array_member (ix ,r3x->mTree[r4x].mMemberSet ,r3x->mMember) ;
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

		void copy_array_member (CREF<INDEX> curr ,CREF<SoftSet<String<STRU8>>> set_ ,CREF<Array<String<STRU8>>> member) {
			for (auto &&i : set_.iter ()) {
				INDEX jx = set_.map_get (i) ;
				INDEX jy = mMember.insert () ;
				mMember[jy] = member[jx] ;
				mFoundNode[curr].mMemberSet.add (set_[i] ,jy) ;
			}
		}

		CREF<ImplHolder> down_cast (CREF<Layout> that) const {
			return keep[TYPEAS<CREF<ImplHolder>>::expr] (that.mThis.self) ;
		}

		void update_merge_found_node (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			for (auto &&i : mFoundNode) {
				INDEX iy = ix ;
				ix = mTree.insert () ;
				mTree[ix].mName = move (i.mName) ;
				mTree[ix].mMemberSet = move (i.mMemberSet) ;
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

template <>
exports auto XMLPARSER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename XMLPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait JSONPARSER_SERIALIZATION_HELP ;

template <class DEPEND>
trait JSONPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Layout = typename JSONPARSER_HELP<DEPEND ,ALWAYS>::Layout ;

	struct NODE {
		String<STRU8> mName ;
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

	struct NODE_CLAZZ {
		enum {
			Null ,
			String ,
			Array ,
			Object ,
			EnumSize
		} ;
	} ;

	class ImplHolder implement Holder {
	protected:
		CRef<HEAP> mHeap ;
		INDEX mIndex ;

	public:
		void initialize () override {
			mIndex = NONE ;
		}

		void initialize (CREF<RegBuffer<STRU8>> stream) override {
			using R1X = typename DEPENDENT<JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> ,DEPEND>::Serialization ;
			auto rax = R1X (stream.borrow ()) ;
			rax.generate () ;
			mHeap = CRef<HEAP>::make (rax.poll ()) ;
			mIndex = 0 ;
		}

		Layout clone () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mIndex) ;
		}

		Layout factory (CREF<INDEX> index) const {
			Layout ret ;
			auto rax = VRef<ImplHolder>::make () ;
			rax->mHeap = mHeap ;
			rax->mIndex = index ;
			ret.mThis = move (rax) ;
			return move (ret) ;
		}

		BOOL good () const override {
			if (mHeap == NULL)
				return FALSE ;
			if (mIndex == NONE)
				return FALSE ;
			if (mHeap->mTree[mIndex].mClazz == NODE_CLAZZ::Null)
				return FALSE ;
			return TRUE ;
		}

		BOOL string_type () const override {
			return mHeap->mTree[mIndex].mClazz == NODE_CLAZZ::String ;
		}

		BOOL array_type () const override {
			return mHeap->mTree[mIndex].mClazz == NODE_CLAZZ::Array ;
		}

		BOOL object_type () const override {
			return mHeap->mTree[mIndex].mClazz == NODE_CLAZZ::Object ;
		}

		Layout root () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (0) ;
		}

		Layout parent () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mHeap->mTree[mIndex].mParent) ;
		}

		Layout brother () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mHeap->mTree[mIndex].mBrother) ;
		}

		Layout child () const override {
			if ifnot (good ())
				return factory (NONE) ;
			return factory (mHeap->mTree[mIndex].mChild) ;
		}

		Layout child (CREF<String<STRU8>> name) const override {
			if ifnot (good ())
				return factory (NONE) ;
			if ifnot (object_type ())
				return factory (NONE) ;
			INDEX ix = mHeap->mTree[mIndex].mObjectSet.map (name) ;
			return factory (ix) ;
		}

		Array<Layout> children () const override {
			Array<Layout> ret ;
			if ifswitch (TRUE) {
				if ifnot (good ())
					discard ;
				if ifnot (array_type ())
					discard ;
				auto &&tmp = mHeap->mTree[mIndex].mArraySet ;
				const auto r1x = IterArray<INDEX>::make (tmp.iter ()) ;
				ret = Array<Layout> (r1x.length ()) ;
				for (auto &&i : ret.iter ())
					ret[i] = factory (tmp.map_get (r1x[i])) ;
			}
			return move (ret) ;
		}

		Array<Layout> children (CREF<LENGTH> size_) const override {
			Array<Layout> ret = Array<Layout> (size_) ;
			if ifswitch (TRUE) {
				if ifnot (good ())
					discard ;
				if ifnot (array_type ())
					discard ;
				auto &&tmp = mHeap->mTree[mIndex].mArraySet ;
				const auto r1x = IterArray<INDEX>::make (tmp.iter ()) ;
				for (auto &&i : ret.iter ()) {
					if (i >= r1x.size ())
						break ;
					ret[i] = factory (tmp.map_get (r1x[i])) ;
				}
			}
			return move (ret) ;
		}

		BOOL equal (CREF<Layout> that) const override {
			return equal (keep[TYPEAS<CREF<ImplHolder>>::expr] (that.mThis.self)) ;
		}

		BOOL equal (CREF<ImplHolder> that) const {
			if (good () != that.good ())
				return FALSE ;
			if ifnot (good ())
				return TRUE ;
			if (address (mHeap->mTree) != address (that.mHeap->mTree))
				return FALSE ;
			if (mIndex != that.mIndex)
				return FALSE ;
			return TRUE ;
		}

		CREF<String<STRU8>> member () const leftvalue override {
			if ifnot (good ())
				return String<STRU8>::zero () ;
			return mHeap->mTree[mIndex].mName ;
		}

		CREF<String<STRU8>> fetch () const leftvalue override {
			assume (good ()) ;
			assume (string_type ()) ;
			return mHeap->mTree[mIndex].mValue ;
		}

		BOOL fetch (CREF<BOOL> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<BOOL ,STRU8>::expr]) ;
		}

		VAL32 fetch (CREF<VAL32> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<VAL32 ,STRU8>::expr]) ;
		}

		VAL64 fetch (CREF<VAL64> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<VAL64 ,STRU8>::expr]) ;
		}

		SINGLE fetch (CREF<SINGLE> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<SINGLE ,STRU8>::expr]) ;
		}

		DOUBLE fetch (CREF<DOUBLE> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<DOUBLE ,STRU8>::expr]) ;
		}

		BYTE fetch (CREF<BYTE> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<BYTE ,STRU8>::expr]) ;
		}

		WORD fetch (CREF<WORD> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<WORD ,STRU8>::expr]) ;
		}

		CHAR fetch (CREF<CHAR> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<CHAR ,STRU8>::expr]) ;
		}

		DATA fetch (CREF<DATA> def) const override {
			return template_fetch (def ,string_parse[TYPEAS<DATA ,STRU8>::expr]) ;
		}

		String<STRA> fetch (CREF<String<STRA>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRA ,STRU8>::expr]) ;
		}

		String<STRW> fetch (CREF<String<STRW>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRW ,STRU8>::expr]) ;
		}

		String<STRU8> fetch (CREF<String<STRU8>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRU8 ,STRU8>::expr]) ;
		}

		String<STRU16> fetch (CREF<String<STRU16>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRU16 ,STRU8>::expr]) ;
		}

		String<STRU32> fetch (CREF<String<STRU32>> def) const override {
			return template_fetch (def ,string_cvt[TYPEAS<STRU32 ,STRU8>::expr]) ;
		}

		template <class ARG1 ,class ARG2>
		forceinline ARG1 template_fetch (CREF<ARG1> def ,CREF<ARG2> cvt) const {
			auto rax = Optional<ARG1> () ;
			try_invoke ([&] () {
				rax = Optional<ARG1>::make (cvt (fetch ())) ;
			} ,[&] () {
				rax = Optional<ARG1>::make (def) ;
			} ,[&] () {
				noop () ;
			}) ;
			return rax.poll () ;
		}
	} ;
} ;

template <class DEPEND>
trait JSONPARSER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using NODE = typename JSONPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::NODE ;
	using HEAP = typename JSONPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
	using NODE_CLAZZ = typename JSONPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::NODE_CLAZZ ;

	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,256> ;

	class Serialization {
	protected:
		LENGTH mRecursiveCounter ;
		TextReader<STRU8> mTextReader ;
		TextAttribute<STRU8> mAttribute ;
		RegularReader mReader ;
		List<NODE> mTree ;
		SoftSet<INDEX> mArraySet ;
		SoftSet<String<STRU8>> mObjectSet ;
		INDEX mLastIndex ;
		String<STRU8> mLastString ;

	public:
		implicit Serialization () = delete ;

		explicit Serialization (RREF<CRef<RegBuffer<STRU8>>> stream) {
			mRecursiveCounter = 0 ;
			mTextReader = TextReader<STRU8> (move (stream)) ;
			mAttribute = mTextReader.attribute () ;
			mReader = RegularReader (VRef<TextReader<STRU8>>::reference (mTextReader) ,3) ;
		}

		HEAP poll () {
			HEAP ret ;
			const auto r1x = shrink_order () ;
			ret.mTree = Array<NODE> (r1x.length ()) ;
			for (auto &&i : mTree.iter ()) {
				INDEX ix = r1x[i] ;
				ret.mTree[ix].mName = move (mTree[i].mName) ;
				ret.mTree[ix].mValue = move (mTree[i].mValue) ;
				ret.mTree[ix].mArraySet = move (mTree[i].mArraySet) ;
				for (auto &&j : ret.mTree[ix].mArraySet.iter ()) {
					INDEX iy = ret.mTree[ix].mArraySet.map_get (j) ;
					ret.mTree[ix].mArraySet.map_set (j ,r1x.map (iy)) ;
				}
				ret.mTree[ix].mArraySet.order () ;
				ret.mTree[ix].mObjectSet = move (mTree[i].mObjectSet) ;
				for (auto &&j : ret.mTree[ix].mObjectSet.iter ()) {
					INDEX iy = ret.mTree[ix].mObjectSet.map_get (j) ;
					ret.mTree[ix].mObjectSet.map_set (j ,r1x.map (iy)) ;
				}
				ret.mTree[ix].mObjectSet.order () ;
				ret.mTree[ix].mClazz = mTree[i].mClazz ;
				ret.mTree[ix].mParent = r1x.map (mTree[i].mParent) ;
				ret.mTree[ix].mBrother = r1x.map (mTree[i].mBrother) ;
				ret.mTree[ix].mChild = r1x.map (mTree[i].mChild) ;
			}
			return move (ret) ;
		}

		Set<INDEX> shrink_order () const {
			Set<INDEX> ret = Set<INDEX> (mTree.length ()) ;
			for (auto &&i : mTree.iter ())
				ret.add (i ,ret.length ()) ;
			return move (ret) ;
		}

		void generate () {
			/*
			*	$0->$11 $10 $12
			*	$1->${scalar}
			*	$2->true|false|null
			*	$3->"${string}"
			*	$4->$1|$2|$3|$6|$9
			*	$5->$4|$4 , $5
			*	$6->[ ]|[ $5 ]
			*	$7->$3 : $4
			*	$8->$7|$7 , $8
			*	$9->{ }|{ $8 }
			*	$10->${eps}|$4
			*	$11->${eps}
			*	$12->${end}
			*/
			read_shift_e0 () ;
			assert (mLastIndex == 0) ;
		}

		//@info: $0->$11 $10 $12
		void read_shift_e0 () {
			read_shift_e11 () ;
			mReader >> SKIP_GAP ;
			read_shift_e10 () ;
			INDEX ix = mLastIndex ;
			mReader >> SKIP_GAP ;
			read_shift_e12 () ;
			mLastIndex = ix ;
		}

		//@info: $1->${scalar}
		void read_shift_e1 () {
			mReader >> HINT_SCALAR >> mLastString ;
		}

		//@info: $2->true|false|null
		void read_shift_e2 () {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('t'))
					discard ;
				mReader >> slice ("true") ;
				mLastString = PrintString<STRU8>::make (slice ("true")) ;
			}
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('f'))
					discard ;
				mReader >> slice ("false") ;
				mLastString = PrintString<STRU8>::make (slice ("false")) ;
			}
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('n'))
					discard ;
				mReader >> slice ("null") ;
				mLastString = String<STRU8> () ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
		}

		//@info: $3->"${string}"
		void read_shift_e3 () {
			mReader >> HINT_STRING >> mLastString ;
		}

		//@info: $4->$1|$2|$3|$6|$9
		void read_shift_e4 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::expr) ;
			INDEX ix = NONE ;
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (is_first_of_number ())
					discard ;
				ix = mTree.insert () ;
				mTree[ix].mName = move (mLastString) ;
				read_shift_e1 () ;
				mTree[ix].mValue = move (mLastString) ;
				mTree[ix].mClazz = NODE_CLAZZ::String ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (act) {
				if ifnot (is_first_of_boolean ())
					discard ;
				ix = mTree.insert () ;
				mTree[ix].mName = move (mLastString) ;
				read_shift_e2 () ;
				mTree[ix].mValue = move (mLastString) ;
				mTree[ix].mClazz = NODE_CLAZZ::String ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('n'))
					discard ;
				ix = mTree.insert () ;
				mTree[ix].mName = move (mLastString) ;
				read_shift_e2 () ;
				mTree[ix].mClazz = NODE_CLAZZ::Null ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('\"'))
					discard ;
				ix = mTree.insert () ;
				mTree[ix].mName = move (mLastString) ;
				read_shift_e3 () ;
				mTree[ix].mValue = move (mLastString) ;
				mTree[ix].mClazz = NODE_CLAZZ::String ;
				mTree[ix].mParent = curr ;
				mTree[ix].mBrother = NONE ;
				mTree[ix].mChild = NONE ;
			}
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('['))
					discard ;
				read_shift_e6 (curr) ;
				ix = mLastIndex ;
			}
			if ifswitch (act) {
				if ifnot (mReader[0] == STRU8 ('{'))
					discard ;
				read_shift_e9 (curr) ;
				ix = mLastIndex ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
			mLastIndex = ix ;
		}

		BOOL is_first_of_number () const {
			if (mReader[0] == STRU8 ('+'))
				return TRUE ;
			if (mReader[0] == STRU8 ('-'))
				return TRUE ;
			if (mAttribute.is_number (mReader[0]))
				return TRUE ;
			return FALSE ;
		}

		BOOL is_first_of_boolean () const {
			if (mReader[0] == STRU8 ('t'))
				return TRUE ;
			if (mReader[0] == STRU8 ('f'))
				return TRUE ;
			return FALSE ;
		}

		//@info: $5->$4|$4 , $5
		void read_shift_e5 (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				read_shift_e4 (curr) ;
				const auto r1x = mTree[curr].mArraySet.length () ;
				mTree[curr].mArraySet.add (r1x ,mLastIndex) ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (ix == NONE)
						discard ;
					ix = mLastIndex ;
				}
				if ifswitch (act) {
					mTree[iy].mBrother = mLastIndex ;
				}
				iy = mLastIndex ;
				mReader >> SKIP_GAP ;
				if (mReader[0] != STRU8 (','))
					break ;
				mReader++ ;
				mReader >> SKIP_GAP ;
			}
			mLastIndex = ix ;
		}

		//@info: $6->[ ]|[ $5 ]
		void read_shift_e6 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::expr) ;
			mReader >> slice ("[") ;
			INDEX ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			mTree[ix].mArraySet = mArraySet.share () ;
			mTree[ix].mClazz = NODE_CLAZZ::Array ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
			mReader >> SKIP_GAP ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 (']'))
					discard ;
				read_shift_e5 (ix) ;
				mTree[ix].mChild = mLastIndex ;
				mReader >> SKIP_GAP ;
			}
			mReader >> slice ("]") ;
			mLastIndex = ix ;
		}

		//@info: $7->$3 : $4
		void read_shift_e7 (CREF<INDEX> curr) {
			read_shift_e3 () ;
			auto rax = mLastString ;
			mReader >> SKIP_GAP ;
			mReader >> slice (":") ;
			mReader >> SKIP_GAP ;
			read_shift_e4 (curr) ;
			mTree[curr].mObjectSet.add (move (rax) ,mLastIndex) ;
		}

		//@info: $8->$7|$7 , $8
		void read_shift_e8 (CREF<INDEX> curr) {
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			while (TRUE) {
				read_shift_e7 (curr) ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (ix == NONE)
						discard ;
					ix = mLastIndex ;
				}
				if ifswitch (act) {
					mTree[iy].mBrother = mLastIndex ;
				}
				iy = mLastIndex ;
				mReader >> SKIP_GAP ;
				if (mReader[0] != STRU8 (','))
					break ;
				mReader++ ;
				mReader >> SKIP_GAP ;
			}
			mLastIndex = ix ;
		}

		//@info: $9->{ }|{ $8 }
		void read_shift_e9 (CREF<INDEX> curr) {
			Scope<ScopeCounter> anonymous (ScopeCounter::from (mRecursiveCounter)) ;
			assume (mRecursiveCounter < COUNTER_MAX_DEPTH::expr) ;
			mReader >> slice ("{") ;
			INDEX ix = mTree.insert () ;
			mTree[ix].mName = move (mLastString) ;
			mTree[ix].mObjectSet = mObjectSet.share () ;
			mTree[ix].mClazz = NODE_CLAZZ::Object ;
			mTree[ix].mParent = curr ;
			mTree[ix].mBrother = NONE ;
			mTree[ix].mChild = NONE ;
			mReader >> SKIP_GAP ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 ('}'))
					discard ;
				read_shift_e8 (ix) ;
				mTree[ix].mChild = mLastIndex ;
				mReader >> SKIP_GAP ;
			}
			mReader >> slice ("}") ;
			mLastIndex = ix ;
		}

		//@info: $10->${eps}|$4
		void read_shift_e10 () {
			INDEX ix = NONE ;
			if ifswitch (TRUE) {
				if (mReader[0] == STRU8 (0))
					discard ;
				read_shift_e4 (NONE) ;
				ix = mLastIndex ;
			}
			mLastIndex = ix ;
		}

		//@info: $11->${eps}
		void read_shift_e11 () {
			noop () ;
		}

		//@info: $12->${end}
		void read_shift_e12 () {
			assume (mReader[0] == STRU8 (0)) ;
		}
	} ;
} ;

template <>
exports auto JSONPARSER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename JSONPARSER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait PLYREADER_SERIALIZATION_HELP ;

template <class DEPEND>
trait PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename PLYREADER_HELP<DEPEND ,ALWAYS>::Holder ;

	struct ORDINARY {
		String<STRU8> mName ;
		FLAG mClazz ;
		FLAG mListClazz ;
	} ;

	struct ELEMENT {
		String<STRU8> mName ;
		LENGTH mSize ;
		ArrayList<ORDINARY> mPropertyList ;
		Set<String<STRU8>> mPropertySet ;
	} ;

	struct HEADER {
		String<STRU8> mFormat ;
		ArrayList<ELEMENT> mElementList ;
		Set<String<STRU8>> mElementSet ;
		LENGTH mBodyOffset ;
	} ;

	using INDEX2X = Tuple<INDEX ,INDEX> ;

	struct BODY {
		FLAG mClazz ;
		Array<INDEX2X> mLine ;
	} ;

	struct BODY_CLAZZ {
		enum {
			Null ,
			Bool ,
			Val8 ,
			Val16 ,
			Val32 ,
			Val64 ,
			Single ,
			Double ,
			Byte ,
			Word ,
			Char ,
			Data ,
			EnumSize
		} ;
	} ;

	struct GUIDE {
		INDEX mElement ;
		ArrayList<INDEX> mProperty ;
		INDEX mPropertyIndex ;
		INDEX mLineIndex ;
		INDEX mPlyIndex ;
		FLAG mPlyClazz ;
	} ;

	class ImplHolder implement Holder {
	protected:
		HEADER mHeader ;
		Array<Array<BODY>> mBody ;
		Deque<BYTE> mPlyBYTE ;
		Deque<WORD> mPlyWORD ;
		Deque<CHAR> mPlyCHAR ;
		Deque<DATA> mPlyDATA ;
		GUIDE mGuide ;

	public:
		void initialize (CREF<RegBuffer<STRU8>> stream) override {
			using R1X = typename DEPENDENT<PLYREADER_SERIALIZATION_HELP<DEPEND ,ALWAYS> ,DEPEND>::Serialization ;
			auto rax = R1X (stream.borrow ()) ;
			rax.generate () ;
			mHeader = rax.poll_header () ;
			mBody = rax.poll_body () ;
			mPlyBYTE = rax.poll_ply_byte () ;
			mPlyWORD = rax.poll_ply_word () ;
			mPlyCHAR = rax.poll_ply_char () ;
			mPlyDATA = rax.poll_ply_data () ;
			mGuide.mElement = NONE ;
		}

		INDEX find_element (CREF<String<STRU8>> name) const override {
			return mHeader.mElementSet.map (name) ;
		}

		LENGTH element_size (CREF<INDEX> element) const override {
			return mHeader.mElementList[element].mSize ;
		}

		INDEX find_property (CREF<INDEX> element ,CREF<String<STRU8>> name) const override {
			return mHeader.mElementList[element].mPropertySet.map (name) ;
		}

		LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const override {
			return mBody[element][property].mLine[line].m2nd - mBody[element][property].mLine[line].m1st ;
		}

		void guide_new (CREF<INDEX> element) override {
			assume (element != NONE) ;
			mGuide.mElement = element ;
			mGuide.mProperty.clear () ;
			mGuide.mPropertyIndex = 0 ;
			mGuide.mLineIndex = 0 ;
			mGuide.mPlyIndex = NONE ;
		}

		void guide_put (CREF<INDEX> property) override {
			assume (property != NONE) ;
			assert (mGuide.mElement != NONE) ;
			assert (mGuide.mPlyIndex == NONE) ;
			mGuide.mProperty.add (property) ;
		}

		void guide_jmp () {
			assert (mGuide.mElement != NONE) ;
			INDEX ix = mGuide.mElement ;
			INDEX jx = NONE ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (mGuide.mPlyIndex != NONE)
					discard ;
				mGuide.mLineIndex = 0 ;
				mGuide.mPropertyIndex = 0 ;
				assume (mGuide.mPropertyIndex < mGuide.mProperty.length ()) ;
				assume (mGuide.mLineIndex < mHeader.mElementList[ix].mSize) ;
				jx = mGuide.mProperty[mGuide.mPropertyIndex] ;
				mGuide.mPlyIndex = mBody[ix][jx].mLine[mGuide.mLineIndex].m1st ;
			}
			if ifswitch (act) {
				mGuide.mPlyIndex++ ;
				jx = mGuide.mProperty[mGuide.mPropertyIndex] ;
				if (mGuide.mPlyIndex >= mBody[ix][jx].mLine[mGuide.mLineIndex].m2nd)
					discard ;
			}
			if ifswitch (act) {
				mGuide.mPropertyIndex++ ;
				if (mGuide.mPropertyIndex >= mGuide.mProperty.length ())
					discard ;
				jx = mGuide.mProperty[mGuide.mPropertyIndex] ;
				mGuide.mPlyIndex = mBody[ix][jx].mLine[mGuide.mLineIndex].m1st ;
			}
			if ifswitch (act) {
				mGuide.mLineIndex++ ;
				mGuide.mPropertyIndex = 0 ;
				if (mGuide.mLineIndex >= mHeader.mElementList[ix].mSize)
					discard ;
				jx = mGuide.mProperty[mGuide.mPropertyIndex] ;
				mGuide.mPlyIndex = mBody[ix][jx].mLine[mGuide.mLineIndex].m1st ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
			if ifswitch (TRUE) {
				mGuide.mPlyClazz = mHeader.mElementList[ix].mPropertyList[jx].mListClazz ;
				if (mGuide.mPlyClazz != BODY_CLAZZ::Null)
					discard ;
				mGuide.mPlyClazz = mHeader.mElementList[ix].mPropertyList[jx].mClazz ;
			}
		}

		void read (VREF<BOOL> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Bool) ;
			item = BOOL (mPlyBYTE[mGuide.mPlyIndex] != BYTE (0X00)) ;
		}

		void read (VREF<VAL32> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Val32) ;
			item = bitwise[TYPEAS<VAL32>::expr] (mPlyCHAR[mGuide.mPlyIndex]) ;
		}

		void read (VREF<VAL64> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Val64) ;
			item = bitwise[TYPEAS<VAL64>::expr] (mPlyDATA[mGuide.mPlyIndex]) ;
		}

		void read (VREF<SINGLE> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Single) ;
			item = bitwise[TYPEAS<SINGLE>::expr] (mPlyCHAR[mGuide.mPlyIndex]) ;
		}

		void read (VREF<DOUBLE> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Double) ;
			item = bitwise[TYPEAS<DOUBLE>::expr] (mPlyDATA[mGuide.mPlyIndex]) ;
		}

		void read (VREF<BYTE> item) override {
			guide_jmp () ;
			if ifswitch (TRUE) {
				if (mGuide.mPlyClazz == BODY_CLAZZ::Val8)
					discard ;
				assume (mGuide.mPlyClazz == BODY_CLAZZ::Byte) ;
			}
			item = mPlyBYTE[mGuide.mPlyIndex] ;
		}

		void read (VREF<WORD> item) override {
			guide_jmp () ;
			if ifswitch (TRUE) {
				if (mGuide.mPlyClazz == BODY_CLAZZ::Val16)
					discard ;
				assume (mGuide.mPlyClazz == BODY_CLAZZ::Word) ;
			}
			item = mPlyWORD[mGuide.mPlyIndex] ;
		}

		void read (VREF<CHAR> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Char) ;
			item = mPlyCHAR[mGuide.mPlyIndex] ;
		}

		void read (VREF<DATA> item) override {
			guide_jmp () ;
			assume (mGuide.mPlyClazz == BODY_CLAZZ::Data) ;
			item = mPlyDATA[mGuide.mPlyIndex] ;
		}
	} ;
} ;

template <class DEPEND>
trait PLYREADER_SERIALIZATION_HELP<DEPEND ,ALWAYS> {
	using ELEMENT = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ELEMENT ;
	using ORDINARY = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ORDINARY ;
	using HEADER = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEADER ;
	using BODY = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::BODY ;
	using INDEX2X = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::INDEX2X ;
	using BODY_CLAZZ = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::BODY_CLAZZ ;

	class Serialization {
	protected:
		CRef<RegBuffer<STRU8>> mStream ;
		ByteReader mByteReader ;
		TextReader<STRU8> mTextReader ;
		RegularReader mReader ;
		HEADER mHeader ;
		BOOL mBitwiseReverseFlag ;
		Array<Array<BODY>> mBody ;
		Deque<BYTE> mPlyBYTE ;
		Deque<WORD> mPlyWORD ;
		Deque<CHAR> mPlyCHAR ;
		Deque<DATA> mPlyDATA ;
		String<STRU8> mLastType ;
		String<STRU8> mLastString ;

	public:
		implicit Serialization () = delete ;

		explicit Serialization (RREF<CRef<RegBuffer<STRU8>>> stream) {
			mStream = move (stream) ;
		}

		HEADER poll_header () {
			return move (mHeader) ;
		}

		Array<Array<BODY>> poll_body () {
			return move (mBody) ;
		}

		Deque<BYTE> poll_ply_byte () {
			return move (mPlyBYTE) ;
		}

		Deque<WORD> poll_ply_word () {
			return move (mPlyWORD) ;
		}

		Deque<CHAR> poll_ply_char () {
			return move (mPlyCHAR) ;
		}

		Deque<DATA> poll_ply_data () {
			return move (mPlyDATA) ;
		}

		void generate () {
			read_header () ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (mHeader.mFormat != Slice<STRU8> ("ascii"))
					discard ;
				read_body_text () ;
			}
			if ifswitch (act) {
				if (mHeader.mFormat != Slice<STRU8> ("binary_big_endian"))
					discard ;
				mBitwiseReverseFlag = FALSE ;
				read_body_binary () ;
			}
			if ifswitch (act) {
				if (mHeader.mFormat != Slice<STRU8> ("binary_little_endian"))
					discard ;
				mBitwiseReverseFlag = TRUE ;
				read_body_binary () ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
		}

		void read_header () {
			mTextReader = TextReader<STRU8> (mStream->borrow ()) ;
			mReader = RegularReader (VRef<TextReader<STRU8>>::reference (mTextReader) ,2) ;
			mLastString = String<STRU8> () ;
			mLastType = String<STRU8> () ;
			mReader >> slice ("ply") ;
			mReader >> SKIP_GAP_SPACE ;
			mReader >> SKIP_GAP_ENDLINE ;
			mReader >> slice ("format") ;
			mReader >> SKIP_GAP_SPACE ;
			mReader >> HINT_WORD >> mLastString ;
			mHeader.mFormat = mLastString ;
			mReader >> HINT_ENDLINE >> mLastString ;
			mReader >> SKIP_GAP_SPACE ;
			mReader >> SKIP_GAP_ENDLINE ;
			INDEX ix = NONE ;
			INDEX iy = NONE ;
			const auto r1x = invoke ([&] () {
				Set<String<STRU8>> ret = Set<String<STRU8>> (100) ;
				ret.add (Slice<STRU8> ("float") ,BODY_CLAZZ::Single) ;
				ret.add (Slice<STRU8> ("single") ,BODY_CLAZZ::Single) ;
				ret.add (Slice<STRU8> ("double") ,BODY_CLAZZ::Double) ;
				ret.add (Slice<STRU8> ("char") ,BODY_CLAZZ::Val8) ;
				ret.add (Slice<STRU8> ("uchar") ,BODY_CLAZZ::Val8) ;
				ret.add (Slice<STRU8> ("short") ,BODY_CLAZZ::Val16) ;
				ret.add (Slice<STRU8> ("ushort") ,BODY_CLAZZ::Val16) ;
				ret.add (Slice<STRU8> ("int") ,BODY_CLAZZ::Val32) ;
				ret.add (Slice<STRU8> ("uint") ,BODY_CLAZZ::Val32) ;
				ret.add (Slice<STRU8> ("int32") ,BODY_CLAZZ::Val32) ;
				ret.add (Slice<STRU8> ("int64") ,BODY_CLAZZ::Val64) ;
				ret.add (Slice<STRU8> ("uint8") ,BODY_CLAZZ::Byte) ;
				ret.add (Slice<STRU8> ("uint16") ,BODY_CLAZZ::Word) ;
				ret.add (Slice<STRU8> ("uint32") ,BODY_CLAZZ::Char) ;
				ret.add (Slice<STRU8> ("uint64") ,BODY_CLAZZ::Data) ;
				return move (ret) ;
			}) ;
			const auto r2x = invoke ([&] () {
				Set<String<STRU8>> ret = Set<String<STRU8>> (100) ;
				ret.add (Slice<STRU8> ("char") ,BODY_CLAZZ::Byte) ;
				ret.add (Slice<STRU8> ("uchar") ,BODY_CLAZZ::Byte) ;
				ret.add (Slice<STRU8> ("short") ,BODY_CLAZZ::Word) ;
				ret.add (Slice<STRU8> ("ushort") ,BODY_CLAZZ::Word) ;
				ret.add (Slice<STRU8> ("int") ,BODY_CLAZZ::Char) ;
				ret.add (Slice<STRU8> ("uint") ,BODY_CLAZZ::Char) ;
				ret.add (Slice<STRU8> ("int32") ,BODY_CLAZZ::Char) ;
				return move (ret) ;
			}) ;
			while (TRUE) {
				mReader >> HINT_WORD >> mLastString ;
				if (mLastString == Slice<STRU8> ("end_header"))
					break ;
				mReader >> SKIP_GAP_SPACE ;
				auto act = TRUE ;
				if ifswitch (act) {
					if (mLastString != Slice<STRU8> ("element"))
						discard ;
					ix = mHeader.mElementList.insert () ;
					mReader >> HINT_WORD >> mLastString ;
					mHeader.mElementList[ix].mName = mLastString ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> HINT_SCALAR >> mLastString ;
					const auto r3x = string_parse[TYPEAS<INDEX ,STRU8>::expr] (mLastString) ;
					assume (r3x >= 0) ;
					mHeader.mElementList[ix].mSize = r3x ;
					mHeader.mElementList[ix].mPropertyList = ArrayList<ORDINARY> (r3x) ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> SKIP_GAP_ENDLINE ;
				}
				if ifswitch (act) {
					if (mLastString != Slice<STRU8> ("property"))
						discard ;
					assume (ix != NONE) ;
					mReader >> HINT_WORD >> mLastType ;
					mReader >> SKIP_GAP_SPACE ;
					if (mLastType != Slice<STRU8> ("list"))
						discard ;
					iy = mHeader.mElementList[ix].mPropertyList.insert () ;
					mReader >> HINT_WORD >> mLastString ;
					const auto r4x = r2x.map (mLastString) ;
					assume (r4x != NONE) ;
					mHeader.mElementList[ix].mPropertyList[iy].mClazz = r4x ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> HINT_WORD >> mLastString ;
					const auto r5x = r1x.map (mLastString) ;
					assume (r5x != NONE) ;
					mHeader.mElementList[ix].mPropertyList[iy].mListClazz = r5x ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> HINT_WORD >> mLastString ;
					mHeader.mElementList[ix].mPropertyList[iy].mName = mLastString ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> SKIP_GAP_ENDLINE ;
				}
				if ifswitch (act) {
					if (mLastString != Slice<STRU8> ("property"))
						discard ;
					assume (ix != NONE) ;
					iy = mHeader.mElementList[ix].mPropertyList.insert () ;
					const auto r6x = r1x.map (mLastType) ;
					assume (r6x != NONE) ;
					mHeader.mElementList[ix].mPropertyList[iy].mClazz = r6x ;
					mReader >> SKIP_GAP_SPACE ;
					mHeader.mElementList[ix].mPropertyList[iy].mListClazz = BODY_CLAZZ::Null ;
					mReader >> HINT_WORD >> mLastString ;
					mHeader.mElementList[ix].mPropertyList[iy].mName = mLastString ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> SKIP_GAP_ENDLINE ;
				}
				if ifswitch (act) {
					if (mLastString != Slice<STRU8> ("comment"))
						discard ;
					mReader >> HINT_ENDLINE >> mLastString ;
					mReader >> SKIP_GAP_SPACE ;
					mReader >> SKIP_GAP_ENDLINE ;
				}
				if ifswitch (act) {
					assume (FALSE) ;
				}
			}
			if ifswitch (TRUE) {
				if (mReader[0] != STRU8 ('\r'))
					discard ;
				if (mReader[1] != STRU8 ('\n'))
					discard ;
				mReader++ ;
			}
			assume (mReader[0] == STRU8 ('\n')) ;
			mHeader.mBodyOffset = mTextReader.length () - 1 ;
			for (auto &&i : mHeader.mElementList) {
				i.mPropertyList.remap () ;
				i.mPropertySet = Set<String<STRU8>> (i.mPropertyList.length ()) ;
				for (auto &&j : i.mPropertyList.iter ())
					i.mPropertySet.add (i.mPropertyList[j].mName ,j) ;
			}
			if ifswitch (TRUE) {
				mHeader.mElementList.remap () ;
				mHeader.mElementSet = Set<String<STRU8>> (mHeader.mElementList.length ()) ;
				for (auto &&j : mHeader.mElementList.iter ())
					mHeader.mElementSet.add (mHeader.mElementList[j].mName ,j) ;
			}
		}

		void read_body_text () {
			mTextReader = TextReader<STRU8> (RegBuffer<STRU8>::from (unsafe_deptr (mStream.self[0]) ,mHeader.mBodyOffset ,mStream->size ()).borrow ()) ;
			mTextReader >> GAP ;
			mBody = Array<Array<BODY>> (mHeader.mElementList.length ()) ;
			for (auto &&i : mHeader.mElementList.iter ()) {
				mBody[i] = Array<BODY> (mHeader.mElementList[i].mPropertyList.length ()) ;
				for (auto &&j : mHeader.mElementList[i].mPropertyList.iter ()) {
					mBody[i][j].mClazz = BODY_CLAZZ::Null ;
					mBody[i][j].mLine = Array<INDEX2X> (mHeader.mElementList[i].mSize) ;
				}
				for (auto &&j : iter (0 ,mHeader.mElementList[i].mSize)) {
					for (auto &&k : mHeader.mElementList[i].mPropertyList.iter ()) {
						read_body_text_item (i ,k ,j) ;
						read_body_text_list (i ,k ,j) ;
					}
				}
			}
			mTextReader >> GAP ;
			mTextReader >> EOS ;
		}

		void read_body_text_item (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
			const auto r1x = mHeader.mElementList[element].mPropertyList[property].mClazz ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Single)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r2x = mTextReader.poll (TYPEAS<SINGLE>::expr) ;
				mPlyCHAR.add (bitwise[TYPEAS<CHAR>::expr] (r2x)) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Double)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r3x = mTextReader.poll (TYPEAS<DOUBLE>::expr) ;
				mPlyDATA.add (bitwise[TYPEAS<DATA>::expr] (r3x)) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val8)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r4x = mTextReader.poll (TYPEAS<VAL32>::expr) ;
				mPlyBYTE.add (BYTE (r4x)) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val16)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r4x = mTextReader.poll (TYPEAS<VAL32>::expr) ;
				mPlyWORD.add (WORD (r4x)) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val32)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r4x = mTextReader.poll (TYPEAS<VAL32>::expr) ;
				mPlyCHAR.add (bitwise[TYPEAS<CHAR>::expr] (r4x)) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val64)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r5x = mTextReader.poll (TYPEAS<VAL64>::expr) ;
				mPlyDATA.add (bitwise[TYPEAS<DATA>::expr] (r5x)) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Byte)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r6x = mTextReader.poll (TYPEAS<BYTE>::expr) ;
				mPlyBYTE.add (r6x) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Word)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r7x = mTextReader.poll (TYPEAS<WORD>::expr) ;
				mPlyWORD.add (r7x) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Char)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r8x = mTextReader.poll (TYPEAS<CHAR>::expr) ;
				mPlyCHAR.add (r8x) ;
				mTextReader >> GAP ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Data)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r9x = mTextReader.poll (TYPEAS<DATA>::expr) ;
				mPlyDATA.add (r9x) ;
				mTextReader >> GAP ;
			}
		}

		void read_body_text_list (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
			const auto r1x = mHeader.mElementList[element].mPropertyList[property].mListClazz ;
			if (r1x == BODY_CLAZZ::Null)
				return ;
			const auto r2x = invoke ([&] () {
				if (mBody[element][property].mClazz == BODY_CLAZZ::Byte)
					return LENGTH (mPlyBYTE[mBody[element][property].mLine[line].m1st]) ;
				if (mBody[element][property].mClazz == BODY_CLAZZ::Word)
					return LENGTH (mPlyWORD[mBody[element][property].mLine[line].m1st]) ;
				if (mBody[element][property].mClazz == BODY_CLAZZ::Char)
					return LENGTH (mPlyCHAR[mBody[element][property].mLine[line].m1st]) ;
				assume (FALSE) ;
				return ZERO ;
			}) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Single)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r3x = mTextReader.poll (TYPEAS<SINGLE>::expr) ;
					mPlyCHAR.add (bitwise[TYPEAS<CHAR>::expr] (r3x)) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Double)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r4x = mTextReader.poll (TYPEAS<DOUBLE>::expr) ;
					mPlyDATA.add (bitwise[TYPEAS<DATA>::expr] (r4x)) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val8)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r5x = mTextReader.poll (TYPEAS<VAL32>::expr) ;
					mPlyBYTE.add (BYTE (r5x)) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val16)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r5x = mTextReader.poll (TYPEAS<VAL32>::expr) ;
					mPlyWORD.add (WORD (r5x)) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val32)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r5x = mTextReader.poll (TYPEAS<VAL32>::expr) ;
					mPlyCHAR.add (bitwise[TYPEAS<CHAR>::expr] (r5x)) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val64)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r6x = mTextReader.poll (TYPEAS<VAL64>::expr) ;
					mPlyDATA.add (bitwise[TYPEAS<DATA>::expr] (r6x)) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Byte)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r7x = mTextReader.poll (TYPEAS<BYTE>::expr) ;
					mPlyBYTE.add (r7x) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Word)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r8x = mTextReader.poll (TYPEAS<WORD>::expr) ;
					mPlyWORD.add (r8x) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Char)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r9x = mTextReader.poll (TYPEAS<CHAR>::expr) ;
					mPlyCHAR.add (r9x) ;
					mTextReader >> GAP ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Data)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				mTextReader >> GAP ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r10x = mTextReader.poll (TYPEAS<DATA>::expr) ;
					mPlyDATA.add (r10x) ;
					mTextReader >> GAP ;
				}
			}
		}

		void read_body_binary () {
			mByteReader = ByteReader (RegBuffer<BYTE>::from (unsafe_deptr (mStream.self[0]) ,mHeader.mBodyOffset ,mStream->size ()).borrow ()) ;
			mBody = Array<Array<BODY>> (mHeader.mElementList.length ()) ;
			for (auto &&i : mHeader.mElementList.iter ()) {
				mBody[i] = Array<BODY> (mHeader.mElementList[i].mPropertyList.length ()) ;
				for (auto &&j : mHeader.mElementList[i].mPropertyList.iter ()) {
					mBody[i][j].mClazz = BODY_CLAZZ::Null ;
					mBody[i][j].mLine = Array<INDEX2X> (mHeader.mElementList[i].mSize) ;
				}
				for (auto &&j : iter (0 ,mHeader.mElementList[i].mSize)) {
					for (auto &&k : mHeader.mElementList[i].mPropertyList.iter ()) {
						read_body_binary_item (i ,k ,j) ;
						read_body_binary_list (i ,k ,j) ;
					}
				}
			}
			mByteReader >> EOS ;
		}

		void read_body_binary_item (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
			const auto r1x = mHeader.mElementList[element].mPropertyList[property].mClazz ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Single)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r2x = mByteReader.poll (TYPEAS<SINGLE>::expr) ;
				const auto r3x = bitwise[TYPEAS<CHAR>::expr] (r2x) ;
				mPlyCHAR.add (bitwise_reverse (r3x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Double)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r4x = mByteReader.poll (TYPEAS<DOUBLE>::expr) ;
				const auto r5x = bitwise[TYPEAS<DATA>::expr] (r4x) ;
				mPlyDATA.add (bitwise_reverse (r5x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val8)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r6x = mByteReader.poll (TYPEAS<BYTE>::expr) ;
				mPlyBYTE.add (bitwise_reverse (r6x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val16)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r6x = mByteReader.poll (TYPEAS<WORD>::expr) ;
				mPlyWORD.add (bitwise_reverse (r6x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val32)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r6x = mByteReader.poll (TYPEAS<VAL32>::expr) ;
				const auto r7x = bitwise[TYPEAS<CHAR>::expr] (r6x) ;
				mPlyCHAR.add (bitwise_reverse (r7x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val64)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r8x = mByteReader.poll (TYPEAS<VAL64>::expr) ;
				const auto r9x = bitwise[TYPEAS<DATA>::expr] (r8x) ;
				mPlyDATA.add (bitwise_reverse (r9x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Byte)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r10x = mByteReader.poll (TYPEAS<BYTE>::expr) ;
				mPlyBYTE.add (bitwise_reverse (r10x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Word)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r11x = mByteReader.poll (TYPEAS<WORD>::expr) ;
				mPlyWORD.add (bitwise_reverse (r11x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Char)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r12x = mByteReader.poll (TYPEAS<CHAR>::expr) ;
				mPlyCHAR.add (bitwise_reverse (r12x)) ;
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Data)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + 2 ;
				const auto r13x = mByteReader.poll (TYPEAS<DATA>::expr) ;
				mPlyDATA.add (bitwise_reverse (r13x)) ;
			}
		}

		void read_body_binary_list (CREF<INDEX> element ,CREF<INDEX> property ,CREF<INDEX> line) {
			const auto r1x = mHeader.mElementList[element].mPropertyList[property].mListClazz ;
			if (r1x == BODY_CLAZZ::Null)
				return ;
			const auto r2x = invoke ([&] () {
				if (mBody[element][property].mClazz == BODY_CLAZZ::Byte)
					return LENGTH (mPlyBYTE[mBody[element][property].mLine[line].m1st]) ;
				if (mBody[element][property].mClazz == BODY_CLAZZ::Word)
					return LENGTH (mPlyWORD[mBody[element][property].mLine[line].m1st]) ;
				if (mBody[element][property].mClazz == BODY_CLAZZ::Char)
					return LENGTH (mPlyCHAR[mBody[element][property].mLine[line].m1st]) ;
				assume (FALSE) ;
				return ZERO ;
			}) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Single)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r3x = mByteReader.poll (TYPEAS<SINGLE>::expr) ;
					const auto r4x = bitwise[TYPEAS<CHAR>::expr] (r3x) ;
					mPlyCHAR.add (bitwise_reverse (r4x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Double)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r5x = mByteReader.poll (TYPEAS<DOUBLE>::expr) ;
					const auto r6x = bitwise[TYPEAS<DATA>::expr] (r5x) ;
					mPlyDATA.add (bitwise_reverse (r6x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val8)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r7x = mByteReader.poll (TYPEAS<BYTE>::expr) ;
					mPlyBYTE.add (bitwise_reverse (r7x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val16)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r7x = mByteReader.poll (TYPEAS<WORD>::expr) ;
					mPlyWORD.add (bitwise_reverse (r7x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val32)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r7x = mByteReader.poll (TYPEAS<VAL32>::expr) ;
					const auto r8x = bitwise[TYPEAS<CHAR>::expr] (r7x) ;
					mPlyCHAR.add (bitwise_reverse (r8x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Val64)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r9x = mByteReader.poll (TYPEAS<VAL64>::expr) ;
					const auto r10x = bitwise[TYPEAS<DATA>::expr] (r9x) ;
					mPlyDATA.add (bitwise_reverse (r10x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Byte)
					discard ;
				INDEX ix = mPlyBYTE.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r11x = mByteReader.poll (TYPEAS<BYTE>::expr) ;
					mPlyBYTE.add (bitwise_reverse (r11x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Word)
					discard ;
				INDEX ix = mPlyWORD.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r12x = mByteReader.poll (TYPEAS<WORD>::expr) ;
					mPlyWORD.add (bitwise_reverse (r12x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Char)
					discard ;
				INDEX ix = mPlyCHAR.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r13x = mByteReader.poll (TYPEAS<CHAR>::expr) ;
					mPlyCHAR.add (bitwise_reverse (r13x)) ;
				}
			}
			if ifswitch (act) {
				if (r1x != BODY_CLAZZ::Data)
					discard ;
				INDEX ix = mPlyDATA.tail () ;
				mBody[element][property].mClazz = r1x ;
				mBody[element][property].mLine[line].m1st = ix + 1 ;
				mBody[element][property].mLine[line].m2nd = ix + r2x + 1 ;
				for (auto &&i : iter (0 ,r2x)) {
					noop (i) ;
					const auto r14x = mByteReader.poll (TYPEAS<DATA>::expr) ;
					mPlyDATA.add (bitwise_reverse (r14x)) ;
				}
			}
		}

		BYTE bitwise_reverse (CREF<BYTE> obj) const {
			if ifnot (mBitwiseReverseFlag)
				return obj ;
			return BitProc::bit_reverse (obj) ;
		}

		WORD bitwise_reverse (CREF<WORD> obj) const {
			if ifnot (mBitwiseReverseFlag)
				return obj ;
			return BitProc::bit_reverse (obj) ;
		}

		CHAR bitwise_reverse (CREF<CHAR> obj) const {
			if ifnot (mBitwiseReverseFlag)
				return obj ;
			return BitProc::bit_reverse (obj) ;
		}

		DATA bitwise_reverse (CREF<DATA> obj) const {
			if ifnot (mBitwiseReverseFlag)
				return obj ;
			return BitProc::bit_reverse (obj) ;
		}
	} ;
} ;

template <>
exports auto PLYREADER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename PLYREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;