#pragma once

#ifndef __CSC_FUNCTIONAL__
#define __CSC_FUNCTIONAL__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"
#include "csc_geometry.hpp"
#include "csc_algorithm.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
namespace FUNCTIONAL {
template <class...>
trait SYNTAXTREE_HELP ;

template <>
trait SYNTAXTREE_HELP<ALWAYS> {
	using SYNTEAXTREE_LINK_MIN_SIZE = ENUMAS<VAL ,ENUMID<32>> ;

	struct NODE {
		FLAG mCABI ;
		String<STR> mName ;
		AnyRef<> mValue ;
		Function<void> mOnceActor ;
		Function<void> mActor ;
		BOOL mIsFunction ;
		BOOL mIsIteration ;
		BOOL mPlaying ;
		BOOL mOncePlayed ;
		BOOL mPlayed ;
		Set<INDEX> mParent ;
		Set<INDEX> mChild ;
		Set<INDEX> mMaybe ;
		LENGTH mOrder ;
		LENGTH mDepth ;
	} ;

	using ARRAY2X = Tuple<INDEX ,INDEX> ;

	class SyntaxTree {
	private:
		Box<Mutex> mMutex ;
		List<NODE> mTreeList ;
		Set<INDEX> mTreeCABISet ;
		Deque<INDEX> mTreeStack ;
		Priority<ARRAY2X> mPlayPriority ;
		Deque<ARRAY2X> mDepthStack ;
		BOOL mEnableClean ;
		Deque<INDEX> mCleanQueue ;
		Deque<INDEX> mUndoQueue ;
		Deque<INDEX> mScanQueue ;

	public:
		implicit SyntaxTree () {
			mMutex = Box<Mutex>::make (Mutex::make_recursive_mutex ()) ;
			INDEX ix = mTreeList.insert () ;
			mTreeList[ix].mCABI = 0 ;
			mTreeList[ix].mName = slice ("") ;
			mTreeList[ix].mValue = AnyRef<> () ;
			mTreeList[ix].mOnceActor = Function<void> () ;
			mTreeList[ix].mActor = Function<void> () ;
			mTreeList[ix].mIsFunction = FALSE ;
			mTreeList[ix].mIsIteration = FALSE ;
			mTreeList[ix].mPlaying = FALSE ;
			mTreeList[ix].mOncePlayed = FALSE ;
			mTreeList[ix].mPlayed = FALSE ;
			mTreeList[ix].mParent = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			mTreeList[ix].mChild = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			mTreeList[ix].mMaybe = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			mTreeList[ix].mOrder = 0 ;
			mTreeList[ix].mDepth = 0 ;
			mTreeStack.add (ix) ;
		}

		void mark_as_function () {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ix != root_node ()) ;
			dynamic_assert (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mPlaying)) ;
			mTreeList[ix].mIsFunction = TRUE ;
		}

		void mark_as_iteration () {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ix != root_node ()) ;
			dynamic_assert (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mPlaying)) ;
			mTreeList[ix].mIsIteration = TRUE ;
		}

		template <class ARG1>
		void maybe (XREF<ARG1> id) {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mPlaying)) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id) ;
			dynamic_assert (ix != iy) ;
			if ifswitch (TRUE) {
				mTreeList[iy].mParent.add (ix) ;
				mTreeList[ix].mMaybe.add (iy) ;
				track_depth (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTreeList[iy].mValue.exist ())
					discard ;
				dynamic_assert (ifnot (mTreeList[iy].mOncePlayed)) ;
				dynamic_assert (ifnot (mTreeList[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				mTreeList[iy].mValue = AnyRef<ARG1>::make (thiz) ;
				mTreeStack.pop () ;
			}
		}

		template <class ARG1>
		CREF<REMOVE_ALL<ARG1>> value (XREF<ARG1> id) leftvalue {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mOncePlayed)) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id) ;
			dynamic_assert (ix != iy) ;
			if ifswitch (TRUE) {
				if (!mTreeList[ix].mPlaying)
					discard ;
				dynamic_assert (mTreeList[ix].mMaybe.find (iy) != NONE) ;
			}
			if ifswitch (TRUE) {
				mTreeList[iy].mParent.add (ix) ;
				mTreeList[ix].mChild.add (iy) ;
				mTreeList[ix].mMaybe.erase (iy) ;
				track_depth (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTreeList[iy].mValue.exist ())
					discard ;
				dynamic_assert (ifnot (mTreeList[iy].mOncePlayed)) ;
				dynamic_assert (ifnot (mTreeList[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				mTreeList[iy].mValue = AnyRef<ARG1>::make (thiz) ;
				mTreeStack.pop () ;
			}
			return mTreeList[iy].mValue.rebind (TYPEAS<ARG1>::id).self ;
		}

		void once (RREF<Function<void>> actor) {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ix != root_node ()) ;
			dynamic_assert (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mPlaying)) ;
			dynamic_assert (ifnot (mTreeList[ix].mIsFunction)) ;
			dynamic_assert (ifnot (mTreeList[ix].mIsIteration)) ;
			mTreeList[ix].mOnceActor = move (actor) ;
		}

		void then (RREF<Function<void>> actor) {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ix != root_node ()) ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mOncePlayed)) ;
			mTreeList[ix].mActor = move (actor) ;
		}

		template <class ARG1>
		void undo (CREF<ARG1> id) {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ix != root_node ()) ;
			dynamic_assert (ifnot (mTreeList[ix].mActor.exist ())) ;
			dynamic_assert (ifnot (mTreeList[ix].mOncePlayed)) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id) ;
			dynamic_assert (ix != iy) ;
			dynamic_assert (mTreeList[iy].mIsIteration) ;
			if ifswitch (TRUE) {
				if (ix == root_node ())
					discard ;
				dynamic_assert (mTreeList[ix].mPlaying) ;
				dynamic_assert (mTreeList[ix].mMaybe.find (iy) != NONE) ;
			}
			mUndoQueue.clear () ;
			mUndoQueue.add (iy) ;
			while (TRUE) {
				if (mUndoQueue.empty ())
					break ;
				INDEX jx = mUndoQueue[mUndoQueue.head ()] ;
				mUndoQueue.take () ;
				if ifswitch (TRUE) {
					if (!mTreeList[jx].mPlayed)
						discard ;
					for (auto &&i : mTreeList[jx].mParent)
						mUndoQueue.add (i) ;
					mTreeList[jx].mPlayed = FALSE ;
				}
			}
		}

		void play () {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			mCleanQueue.clear () ;
			INDEX ix = curr_node () ;
			const auto r1x = mTreeList[ix].mDepth ;
			play_scan (ix) ;
			while (TRUE) {
				if (mPlayPriority.empty ())
					break ;
				INDEX jx = mPlayPriority.get (mPlayPriority.head ()) ;
				if (mTreeList[jx].mDepth <= r1x)
					break ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if (mTreeList[jx].mOncePlayed)
						discard ;
					dynamic_assert (mTreeList[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if (!mTreeList[jx].mOnceActor.exist ())
							discard ;
						mTreeStack.add (jx) ;
						mTreeList[jx].mOnceActor () ;
						mTreeStack.pop () ;
					}
					mTreeList[jx].mOncePlayed = TRUE ;
					for (auto &&i : mTreeList[jx].mMaybe) {
						if (mTreeList[jx].mChild.find (i) != NONE)
							continue ;
						mTreeList[i].mParent.erase (jx) ;
						if (mTreeList[i].mParent.length () > 0)
							continue ;
						mCleanQueue.add (i) ;
					}
					mTreeList[jx].mMaybe.clear () ;
					play_scan (jx) ;
				}
				if ifswitch (eax) {
					if (mTreeList[jx].mPlayed)
						discard ;
					dynamic_assert (mTreeList[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if (!mTreeList[jx].mActor.exist ())
							discard ;
						mTreeStack.add (jx) ;
						mTreeList[jx].mActor () ;
						mTreeStack.pop () ;
					}
					mTreeList[jx].mPlayed = TRUE ;
					if ifswitch (TRUE) {
						if (mTreeList[jx].mIsFunction)
							discard ;
						for (auto &&i : mTreeList[jx].mChild) {
							mTreeList[i].mParent.erase (jx) ;
							if (mTreeList[i].mParent.length () > 0)
								continue ;
							mCleanQueue.add (i) ;
						}
						mTreeList[jx].mChild.clear () ;
						mTreeList[jx].mActor = Function<void> () ;
					}
				}
				if ifswitch (eax) {
					mPlayPriority.take () ;
					mTreeList[jx].mPlaying = FALSE ;
					play_clean () ;
				}
			}
		}

		void clean () {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			INDEX ix = curr_node () ;
			dynamic_assert (ix == root_node ()) ;
			mEnableClean = TRUE ;
			mCleanQueue.clear () ;
			for (auto &&i : mTreeList.iter ()) {
				if (mTreeList[i].mParent.length () > 0)
					continue ;
				if (i == ix)
					continue ;
				mCleanQueue.add (i) ;
			}
			play_clean () ;
		}

	private:
		template <class ARG1>
		INDEX insert_node (XREF<ARG1> id) {
			const auto r1x = operator_cabi (TYPEAS<ARG1>::id) ;
			const auto r2x = invoke ([&] () {
				String<STR> ret ;
				unimplemented () ;
				return move (ret) ;
			}) ;
			INDEX ret = mTreeCABISet.map (r1x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				ret = mTreeList.insert () ;
				mTreeCABISet.add (r1x ,ret) ;
				mTreeList[ret].mCABI = r1x ;
				mTreeList[ret].mName = r2x ;
				mTreeList[ret].mValue = AnyRef<> () ;
				mTreeList[ret].mOnceActor = Function<void> () ;
				mTreeList[ret].mActor = Function<void> () ;
				mTreeList[ret].mIsFunction = FALSE ;
				mTreeList[ret].mIsIteration = FALSE ;
				mTreeList[ret].mPlaying = FALSE ;
				mTreeList[ret].mOncePlayed = FALSE ;
				mTreeList[ret].mPlayed = FALSE ;
				mTreeList[ret].mParent = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
				mTreeList[ret].mChild = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
				mTreeList[ret].mMaybe = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
				INDEX ix = root_node () ;
				mTreeList[ret].mOrder = mTreeList[ix].mChild.length () ;
				mTreeList[ret].mDepth = 1 ;
			}
			return move (ret) ;
		}

		INDEX root_node () const {
			return mTreeStack[mTreeStack.head ()] ;
		}

		INDEX curr_node () const {
			return mTreeStack[mTreeStack.tail ()] ;
		}

		void track_depth (CREF<INDEX> curr ,CREF<INDEX> prev) {
			mDepthStack.clear () ;
			const auto r1x = ARRAY2X (curr ,prev) ;
			mDepthStack.add (r1x) ;
			while (TRUE) {
				if (mDepthStack.empty ())
					break ;
				const auto r2x = mDepthStack[mDepthStack.tail ()] ;
				INDEX ix = r2x.pick (TYPEAS<RANK0>::id) ;
				INDEX iy = r2x.pick (TYPEAS<RANK1>::id) ;
				mDepthStack.pop () ;
				if ifswitch (TRUE) {
					const auto r4x = mTreeList[iy].mOrder ;
					const auto r5x = mTreeList[iy].mDepth + 1 ;
					if (mTreeList[ix].mOrder <= r4x)
						if (mTreeList[ix].mDepth >= r5x)
							discard ;
					if ifswitch (TRUE) {
						if (iy == root_node ())
							discard ;
						mTreeList[ix].mOrder = min_of (mTreeList[ix].mOrder ,r4x) ;
					}
					mTreeList[ix].mDepth = max_of (mTreeList[ix].mDepth ,r5x) ;
					if ifswitch (TRUE) {
						if (!mTreeList[ix].mPlaying)
							discard ;
						const auto r6x = ARRAY2X (mTreeList[ix].mOrder ,-mTreeList[ix].mDepth) ;
						mPlayPriority.add (r6x ,ix) ;
					}
					for (auto &&i : mTreeList[ix].mChild) {
						const auto r3x = ARRAY2X (i ,ix) ;
						mDepthStack.add (r3x) ;
					}
					for (auto &&i : mTreeList[ix].mMaybe) {
						const auto r3x = ARRAY2X (i ,ix) ;
						mDepthStack.add (r3x) ;
					}
				}
			}
		}

		void play_scan (CREF<INDEX> curr) {
			if (mTreeList[curr].mPlayed)
				return ;
			mScanQueue.clear () ;
			for (auto &&i : mTreeList[curr].mChild)
				mScanQueue.add (i) ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				INDEX jx = mScanQueue[mScanQueue.head ()] ;
				mScanQueue.take () ;
				if ifswitch (TRUE) {
					if (mTreeList[jx].mPlayed)
						discard ;
					if (mTreeList[jx].mPlaying)
						discard ;
					const auto r1x = ARRAY2X (mTreeList[jx].mOrder ,-mTreeList[jx].mDepth) ;
					mPlayPriority.add (r1x ,jx) ;
					for (auto &&i : mTreeList[jx].mChild)
						mScanQueue.add (i) ;
					mTreeList[jx].mPlaying = TRUE ;
				}
			}
		}

		void play_clean () {
			if (!mEnableClean)
				return ;
			while (TRUE) {
				if (mCleanQueue.empty ())
					break ;
				INDEX jx = mCleanQueue[mCleanQueue.head ()] ;
				mCleanQueue.take () ;
				dynamic_assert (ifnot (mTreeList[jx].mPlaying)) ;
				if ifswitch (TRUE) {
					for (auto &&i : mTreeList[jx].mChild) {
						mTreeList[i].mParent.erase (jx) ;
						if (mTreeList[i].mParent.length () > 0)
							continue ;
						mCleanQueue.add (i) ;
					}
					mTreeList[jx].mChild.clear () ;
					mTreeList[jx].mActor = Function<void> () ;
				}
				mTreeList[jx].mValue = AnyRef<> () ;
			}
		}
	} ;
} ;

template <class...>
trait OPERAND_HELP ;

template <class...>
trait OPERANDLIST_HELP ;

template <class...>
trait EXPRESSION_HELP ;

template <class...>
trait EXPRESSION_SUPER_HELP ;

template <>
trait EXPRESSION_SUPER_HELP<ALWAYS> {
	struct NODE {

	} ;

	class Expression {
	private:
		template <class...>
		friend trait EXPRESSION_HELP ;

	private:
		CRef<NODE> mThis ;

	public:
		implicit Expression () = default ;
	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_EQUAL<RANK ,RANK0>>> {
	using SUPER = typename EXPRESSION_SUPER_HELP<ALWAYS>::Expression ;

	class Expression extend SUPER {
	private:
		using SUPER::mThis ;

	public:
		implicit Expression () = default ;
	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_EQUAL<RANK ,RANK1>>> {
	using SUPER = typename EXPRESSION_SUPER_HELP<ALWAYS>::Expression ;

	class Expression extend SUPER {
	private:
		using SUPER::mThis ;

	public:
		implicit Expression () = default ;
	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_EQUAL<RANK ,RANK2>>> {
	using SUPER = typename EXPRESSION_SUPER_HELP<ALWAYS>::Expression ;

	class Expression extend SUPER {
	private:
		using SUPER::mThis ;

	public:
		implicit Expression () = default ;
	} ;
} ;

template <class RANK>
trait EXPRESSION_HELP<RANK ,REQUIRE<ENUM_COMPR_GT<RANK ,RANK3>>> {
	using SUPER = typename EXPRESSION_SUPER_HELP<ALWAYS>::Expression ;

	class Expression extend SUPER {
	private:
		using SUPER::mThis ;

	public:
		implicit Expression () = default ;
	} ;
} ;
} ;
} ;

namespace CSC {
using namespace FUNCTIONAL ;
} ;