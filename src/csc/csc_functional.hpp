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

template <class DEPEND>
trait SYNTAXTREE_HELP<DEPEND ,ALWAYS> {
	using SYNTEAXTREE_LINK_MIN_SIZE = ENUMAS<VAL ,ENUMID<32>> ;

	struct NODE {
		String<STR> mName ;
		FLAG mCABI ;
		FLAG mThread ;
		AutoRef<> mValue ;
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
		VRef<INDEX> mLater ;
		LENGTH mOrder ;
		LENGTH mDepth ;
		LENGTH mUsage ;
	} ;

	using ARRAY2X = Tuple<INDEX ,INDEX> ;

	class SyntaxTree {
	private:
		List<NODE> mTreeList ;
		Set<ARRAY2X> mTreeCABISet ;
		VRef<LENGTH> mThreadGenerator ;
		Deque<INDEX> mTreeStack ;
		Priority<ARRAY2X> mPlayPriority ;
		Deque<ARRAY2X> mDepthStack ;
		CRef<BOOL> mEnableClean ;
		Deque<INDEX> mCleanQueue ;
		Deque<INDEX> mUndoQueue ;
		Deque<INDEX> mScanQueue ;

	public:
		implicit SyntaxTree () {
			INDEX ix = mTreeList.insert () ;
			mTreeList[ix].mName = slice ("") ;
			mTreeList[ix].mCABI = 0 ;
			mTreeList[ix].mThread = 0 ;
			mTreeList[ix].mValue = AutoRef<> () ;
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
			mTreeList[ix].mLater = NULL ;
			mTreeList[ix].mOrder = 0 ;
			mTreeList[ix].mDepth = 0 ;
			mTreeList[ix].mUsage = 0 ;
			mTreeStack.add (ix) ;
		}

		SyntaxTree thread () {
			SyntaxTree ret ;
			if ifswitch (TRUE) {
				if (mThreadGenerator != NULL)
					discard ;
				mThreadGenerator = VRef<LENGTH>::make (0) ;
			}
			INDEX ix = ret.root_node () ;
			ret.mTreeList[ix].mThread = mThreadGenerator.self ;
			mThreadGenerator.self++ ;
			return move (ret) ;
		}

		void mark_as_function () {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mPlaying)) ;
			mTreeList[ix].mIsFunction = TRUE ;
		}

		void mark_as_iteration () {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mPlaying)) ;
			mTreeList[ix].mIsIteration = TRUE ;
		}

		template <class ARG1>
		void later (XREF<ARG1> id) {
			INDEX ix = curr_node () ;
			assume (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mPlaying)) ;
			assume (mTreeList[ix].mIsIteration) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id ,ix) ;
			assume (ix != iy) ;
			mTreeList[iy].mLater = VRef<INDEX>::make (ix) ;
		}

		template <class ARG1>
		void maybe (XREF<ARG1> id) {
			INDEX ix = curr_node () ;
			assume (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mPlaying)) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id ,ix) ;
			assume (ix != iy) ;
			if ifswitch (TRUE) {
				mTreeList[iy].mParent.add (ix) ;
				mTreeList[ix].mMaybe.add (iy) ;
				track_depth (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTreeList[iy].mValue.exist ())
					discard ;
				assume (ifnot (mTreeList[iy].mOncePlayed)) ;
				assume (ifnot (mTreeList[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				mTreeList[iy].mValue = AutoRef<ARG1>::make (thiz) ;
				mTreeStack.pop () ;
			}
		}

		template <class ARG1>
		CREF<REMOVE_ALL<ARG1>> value (XREF<ARG1> id) leftvalue {
			INDEX ix = curr_node () ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mOncePlayed)) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id ,ix) ;
			assume (ix != iy) ;
			if ifswitch (TRUE) {
				if ifnot (mTreeList[ix].mPlaying)
					discard ;
				assume (mTreeList[ix].mMaybe.find (iy) != NONE) ;
				if ifnot (mTreeList[iy].mIsIteration)
					discard ;
				mTreeList[ix].mIsIteration = TRUE ;
			}
			if ifswitch (TRUE) {
				mTreeList[iy].mParent.add (ix) ;
				mTreeList[ix].mChild.add (iy) ;
				track_depth (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTreeList[iy].mValue.exist ())
					discard ;
				assume (ifnot (mTreeList[iy].mOncePlayed)) ;
				assume (ifnot (mTreeList[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				mTreeList[iy].mValue = AutoRef<ARG1>::make (thiz) ;
				mTreeStack.pop () ;
			}
			return mTreeList[iy].mValue.rebind (TYPEAS<ARG1>::id).self ;
		}

		void once (RREF<Function<void>> actor) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTreeList[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mPlaying)) ;
			assume (ifnot (mTreeList[ix].mIsFunction)) ;
			assume (ifnot (mTreeList[ix].mIsIteration)) ;
			mTreeList[ix].mOnceActor = move (actor) ;
		}

		void then (RREF<Function<void>> actor) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mOncePlayed)) ;
			mTreeList[ix].mActor = move (actor) ;
		}

		template <class ARG1>
		void undo (XREF<ARG1> id) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTreeList[ix].mActor.exist ())) ;
			assume (ifnot (mTreeList[ix].mOncePlayed)) ;
			INDEX iy = insert_node (TYPEAS<ARG1>::id ,ix) ;
			assume (ix != iy) ;
			assume (mTreeList[iy].mIsIteration) ;
			if ifswitch (TRUE) {
				if (ix == root_node ())
					discard ;
				assume (mTreeList[ix].mPlaying) ;
				assume (mTreeList[ix].mMaybe.find (iy) != NONE) ;
			}
			if ifnot (mTreeList[iy].mPlayed)
				return ;
			mUndoQueue.clear () ;
			mUndoQueue.add (iy) ;
			while (TRUE) {
				if (mUndoQueue.empty ())
					break ;
				INDEX jx = mUndoQueue[mUndoQueue.head ()] ;
				mUndoQueue.take () ;
				if ifswitch (TRUE) {
					for (auto &&i : mTreeList[jx].mParent) {
						if ifnot (mTreeList[i].mPlayed)
							continue ;
						mUndoQueue.add (i) ;
					}
					mTreeList[jx].mPlayed = FALSE ;

				}
			}
		}

		void play () {
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
					assume (mTreeList[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTreeList[jx].mOnceActor.exist ())
							discard ;
						const auto r2x = unsafe_usage () ;
						mTreeStack.add (jx) ;
						mTreeList[jx].mOnceActor () ;
						mTreeStack.pop () ;
						const auto r3x = unsafe_usage () ;
						mTreeList[jx].mUsage += r3x - r2x ;
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
					assume (mTreeList[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTreeList[jx].mActor.exist ())
							discard ;
						const auto r4x = unsafe_usage () ;
						mTreeStack.add (jx) ;
						mTreeList[jx].mActor () ;
						mTreeStack.pop () ;
						const auto r5x = unsafe_usage () ;
						mTreeList[jx].mUsage += r5x - r4x ;
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
			INDEX ix = curr_node () ;
			assume (ix == root_node ()) ;
			mEnableClean = CRef<BOOL>::make (TRUE) ;
			mCleanQueue.clear () ;
			for (auto &&i : mTreeList.iter ()) {
				if (i == ix)
					continue ;
				if (mTreeList[i].mParent.length () > 0)
					continue ;
				mCleanQueue.add (i) ;
			}
			play_clean () ;
		}

	private:
		template <class ARG1>
		INDEX insert_node (XREF<ARG1> id ,CREF<INDEX> prev) {
			const auto r1x = operator_cabi (id) ;
			const auto r2x = ARRAY2X (r1x ,mTreeList[prev].mThread) ;
			INDEX ret = mTreeCABISet.map (r2x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				ret = mTreeList.insert () ;
				mTreeCABISet.add (r2x ,ret) ;
				mTreeList[ret].mName = operator_name (id) ;
				mTreeList[ret].mCABI = r1x ;
				mTreeList[ret].mThread = mTreeList[prev].mThread ;
				mTreeList[ret].mValue = AutoRef<> () ;
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
				mTreeList[ret].mLater = NULL ;
				INDEX ix = root_node () ;
				mTreeList[ret].mOrder = mTreeList[ix].mChild.length () ;
				mTreeList[ret].mDepth = 1 ;
				mTreeList[ret].mUsage = 0 ;
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
					const auto r3x = mTreeList[iy].mOrder ;
					const auto r4x = mTreeList[iy].mDepth + 1 ;
					if (mTreeList[ix].mOrder <= r3x)
						if (mTreeList[ix].mDepth >= r4x)
							discard ;
					if ifswitch (TRUE) {
						if (iy == root_node ())
							discard ;
						mTreeList[ix].mOrder = min_of (mTreeList[ix].mOrder ,r3x) ;
					}
					mTreeList[ix].mDepth = max_of (mTreeList[ix].mDepth ,r4x) ;
					if ifswitch (TRUE) {
						if ifnot (mTreeList[ix].mPlaying)
							discard ;
						const auto r5x = ARRAY2X (mTreeList[ix].mOrder ,-mTreeList[ix].mDepth) ;
						mPlayPriority.add (r5x ,ix) ;
					}
					for (auto &&i : mTreeList[ix].mChild) {
						assume (i != curr) ;
						const auto r6x = ARRAY2X (i ,ix) ;
						mDepthStack.add (r6x) ;
					}
					for (auto &&i : mTreeList[ix].mMaybe) {
						assume (i != curr) ;
						const auto r7x = ARRAY2X (i ,ix) ;
						mDepthStack.add (r7x) ;
					}
				}
			}
		}

		void play_scan (CREF<INDEX> curr) {
			mScanQueue.clear () ;
			for (auto &&i : mTreeList[curr].mChild) {
				if (mTreeList[i].mPlaying)
					continue ;
				if (mTreeList[i].mPlayed)
					continue ;
				mScanQueue.add (i) ;
			}
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				INDEX jx = mScanQueue[mScanQueue.head ()] ;
				mScanQueue.take () ;
				if ifswitch (TRUE) {
					const auto r1x = ARRAY2X (mTreeList[jx].mOrder ,-mTreeList[jx].mDepth) ;
					mPlayPriority.add (r1x ,jx) ;
					for (auto &&i : mTreeList[jx].mChild) {
						if (mTreeList[i].mPlaying)
							continue ;
						if (mTreeList[i].mPlayed)
							continue ;
						mScanQueue.add (i) ;
					}
					mTreeList[jx].mPlaying = TRUE ;
				}
			}
		}

		void play_clean () {
			if ifnot (mEnableClean.self)
				return ;
			while (TRUE) {
				if (mCleanQueue.empty ())
					break ;
				INDEX jx = mCleanQueue[mCleanQueue.head ()] ;
				mCleanQueue.take () ;
				assume (ifnot (mTreeList[jx].mPlaying)) ;
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
				mTreeList[jx].mValue = AutoRef<> () ;
			}
		}
	} ;
} ;

using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;

template <class...>
trait OPERAND_HELP ;

template <class DEPEND>
trait OPERAND_HELP<DEPEND ,ALWAYS> {
	class Operand ;
} ;

template <class...>
trait OPERANDLIST_HELP ;

template <class DEPEND>
trait OPERANDLIST_HELP<DEPEND ,ALWAYS> {
	class OperandList ;
} ;
} ;
} ;

namespace CSC {
using namespace FUNCTIONAL ;
} ;