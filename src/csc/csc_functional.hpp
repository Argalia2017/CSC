#pragma once

#ifndef __CSC_FUNCTIONAL__
#define __CSC_FUNCTIONAL__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
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
#include "csc_thread.hpp"

namespace CSC {
template <class...>
trait SYNTAXTREE_HELP ;

template <class...>
trait SYNTAXTREE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SYNTAXTREE_HELP<DEPEND ,ALWAYS> {
	using LINK_MIN_SIZE = ENUMAS<VAL ,ENUMID<32>> ;

	struct NODE {
		String<STR> mName ;
		FLAG mCABI ;
		AutoRef<> mValue ;
		AutoRef<> mLater ;
		Function<void> mOnceActor ;
		Function<void> mActor ;
		BOOL mIsFunction ;
		BOOL mIsIteration ;
		BOOL mPlaying ;
		BOOL mOncePlayed ;
		BOOL mPlayed ;
		LENGTH mLifetime ;
		Set<INDEX> mParent ;
		Set<INDEX> mDepend ;
		Set<INDEX> mChild ;
		Set<INDEX> mMaybe ;
		LENGTH mOrder ;
		LENGTH mDepth ;
		LENGTH mMemoryUsage ;
		LENGTH mTimeCost ;
	} ;

	using ARRAY2X = Tuple<INDEX ,INDEX> ;

	class SyntaxTree {
	protected:
		List<NODE> mTree ;
		Array<ArrayList<NODE>> mThreadTree ;
		Set<FLAG> mTreeCABISet ;
		CRef<BOOL> mEnableClean ;
		Deque<INDEX> mTreeStack ;
		Priority<ARRAY2X> mPlayPriority ;
		Deque<ARRAY2X> mTrackStack ;
		Deque<INDEX> mCleanQueue ;
		Deque<INDEX> mScanQueue ;

	public:
		implicit SyntaxTree () {
			INDEX ix = mTree.insert () ;
			mTree[ix].mName = slice ("") ;
			mTree[ix].mCABI = 0 ;
			mTree[ix].mIsFunction = FALSE ;
			mTree[ix].mIsIteration = FALSE ;
			mTree[ix].mPlaying = FALSE ;
			mTree[ix].mOncePlayed = FALSE ;
			mTree[ix].mPlayed = FALSE ;
			mTree[ix].mLifetime = 0 ;
			mTree[ix].mParent = Set<INDEX> (LINK_MIN_SIZE::expr) ;
			mTree[ix].mChild = Set<INDEX> (LINK_MIN_SIZE::expr) ;
			mTree[ix].mMaybe = Set<INDEX> (LINK_MIN_SIZE::expr) ;
			mTree[ix].mOrder = 0 ;
			mTree[ix].mDepth = 0 ;
			mTree[ix].mMemoryUsage = 0 ;
			mTree[ix].mTimeCost = 0 ;
			mTreeStack.add (ix) ;
		}

		void mark_as_function () {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			mTree[ix].mIsFunction = TRUE ;
		}

		void mark_as_iteration () {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			mTree[ix].mIsIteration = TRUE ;
			mTree[ix].mDepend = Set<INDEX> (LINK_MIN_SIZE::expr) ;
			mTree[ix].mDepend.add (ix) ;
		}

		template <class ARG1>
		void maybe (CREF<TYPEID<ARG1>> id) {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			INDEX iy = insert_node (id) ;
			assume (ix != iy) ;
			if ifswitch (TRUE) {
				if (mTree[ix].mMaybe.find (iy) != NONE)
					discard ;
				mTree[iy].mParent.add (ix) ;
				mTree[ix].mMaybe.add (iy) ;
				depth_track (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTree[iy].mValue.exist ())
					discard ;
				assume (ifnot (mTree[iy].mOncePlayed)) ;
				assume (ifnot (mTree[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				auto rax = AutoRef<Box<ARG1>>::make () ;
				//@warn: mValue depend on it's address
				rax->acquire (id ,thiz) ;
				mTree[iy].mValue = move (rax) ;
				mTreeStack.pop () ;
			}
		}

		template <class ARG1>
		CREF<ARG1> stack (CREF<TYPEID<ARG1>> id) leftvalue {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			INDEX iy = insert_node (id) ;
			assume (ix != iy) ;
			if ifswitch (TRUE) {
				if ifnot (mTree[ix].mPlaying)
					discard ;
				assume (mTree[iy].mParent.find (ix) != NONE) ;
				assume (mTree[ix].mMaybe.find (iy) != NONE) ;
			}
			if ifswitch (TRUE) {
				if (mTree[ix].mChild.find (iy) != NONE)
					discard ;
				mTree[iy].mParent.add (ix) ;
				mTree[ix].mChild.add (iy) ;
				depth_track (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTree[iy].mValue.exist ())
					discard ;
				assume (ifnot (mTree[iy].mOncePlayed)) ;
				assume (ifnot (mTree[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				auto rax = AutoRef<Box<ARG1>>::make () ;
				//@warn: mValue depend on it's address
				rax->acquire (id ,thiz) ;
				mTree[iy].mValue = move (rax) ;
				mTreeStack.pop () ;
			}
			if ifswitch (TRUE) {
				if ifnot (mTree[ix].mIsIteration)
					discard ;
				if ifnot (mTree[iy].mIsIteration)
					discard ;
				undo_scan (iy ,ix) ;
			}
			auto &&tmp = mTree[iy].mValue.rebind (TYPEAS<Box<ARG1>>::expr) ;
			return tmp->self ;
		}

		template <class ARG1>
		CREF<ARG1> later (CREF<TYPEID<ARG1>> id) leftvalue {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (mTree[ix].mPlaying) ;
			assume (mTree[ix].mIsIteration) ;
			assume (mTree[ix].mLater.exist ()) ;
			auto &&tmp = mTree[ix].mLater.rebind (TYPEAS<CRef<ARG1>>::expr) ;
			return tmp->self ;
		}

		void once (RREF<Function<void>> actor) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			assume (ifnot (mTree[ix].mIsIteration)) ;
			mTree[ix].mOnceActor = move (actor) ;
		}

		void then (RREF<Function<void>> actor) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			mTree[ix].mActor = move (actor) ;
		}

		template <class ARG1>
		void undo (CREF<TYPEID<ARG1>> id) {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			assume (ifnot (mTree[ix].mIsIteration)) ;
			INDEX iy = insert_node (id) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			assume (mTree[ix].mDepth < mTree[iy].mDepth) ;
			if ifnot (mTree[iy].mPlayed)
				return ;
			for (auto &&i : mTree[iy].mDepend) {
				if ifnot (mTree[i].mPlayed)
					continue ;
				mTree[i].mPlayed = FALSE ;
				mTree[i].mLifetime = 0 ;
			}
		}

		template <class ARG1 ,class ARG2>
		void redo (CREF<ARG1> id ,CREF<ARG2> obj) {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			assume (ifnot (mTree[ix].mIsIteration)) ;
			INDEX iy = insert_node (id) ;
			assume (ifnot (mTree[iy].mPlayed)) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			assume (mTree[ix].mDepth < mTree[iy].mDepth) ;
			if ifswitch (TRUE) {
				if (mTree[iy].mLater.exist ())
					if (mTree[iy].mLater.clazz () == Clazz (TYPEAS<CRef<ARG2>>::expr))
						discard ;
				mTree[iy].mLater = AutoRef<CRef<ARG2>>::make () ;
			}
			auto &&tmp = mTree[iy].mLater.rebind (TYPEAS<CRef<ARG2>>::expr) ;
			tmp.self = CRef<ARG2>::reference (obj) ;
		}

		template <class ARG1 ,class ARG2>
		void redo (CREF<ARG1> ,RREF<ARG2>) = delete ;

		void play () {
			INDEX ix = curr_node () ;
			play_scan (ix) ;
			while (TRUE) {
				if (mPlayPriority.empty ())
					break ;
				INDEX iy = mPlayPriority.get (mPlayPriority.head ()) ;
				if (mTree[iy].mDepth <= mTree[ix].mDepth)
					break ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if (mTree[iy].mOncePlayed)
						discard ;
					assume (mTree[iy].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[iy].mOnceActor.exist ())
							discard ;
						const auto r1x = NowTimePoint () ;
						const auto r2x = HeapProc::instance ().usage_size () ;
						mTreeStack.add (iy) ;
						mTree[iy].mOnceActor () ;
						mTreeStack.pop () ;
						const auto r3x = HeapProc::instance ().usage_size () ;
						const auto r4x = NowTimePoint () ;
						mTree[iy].mMemoryUsage += LENGTH (r3x - r2x) ;
						mTree[iy].mTimeCost += (r4x - r1x).seconds () ;
					}
					mTree[iy].mOncePlayed = TRUE ;
					mTree[iy].mLifetime = 0 ;
					for (auto &&i : mTree[iy].mMaybe) {
						if (mTree[iy].mChild.find (i) != NONE)
							continue ;
						mTree[i].mParent.erase (iy) ;
						if (mTree[i].mLifetime < mTree[i].mParent.length ())
							continue ;
						mCleanQueue.add (i) ;
					}
					mTree[iy].mMaybe.clear () ;
					play_scan (iy) ;
				}
				if ifswitch (rxx) {
					if (mTree[iy].mPlayed)
						discard ;
					assume (mTree[iy].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[iy].mActor.exist ())
							discard ;
						const auto r5x = NowTimePoint () ;
						const auto r6x = HeapProc::instance ().usage_size () ;
						mTreeStack.add (iy) ;
						mTree[iy].mActor () ;
						mTreeStack.pop () ;
						const auto r7x = HeapProc::instance ().usage_size () ;
						const auto r8x = NowTimePoint () ;
						mTree[iy].mMemoryUsage += LENGTH (r7x - r6x) ;
						mTree[iy].mTimeCost += (r8x - r5x).seconds () ;
					}
					mTree[iy].mPlayed = TRUE ;
					mTree[iy].mLifetime = 0 ;
					if ifswitch (TRUE) {
						if (mTree[iy].mIsFunction)
							discard ;
						if (mTree[iy].mIsIteration)
							discard ;
						for (auto &&i : mTree[iy].mChild) {
							mTree[i].mLifetime++ ;
							if (mTree[i].mLifetime < mTree[i].mParent.length ())
								continue ;
							mCleanQueue.add (i) ;
						}
					}
				}
				if ifswitch (rxx) {
					mPlayPriority.take () ;
					mTree[iy].mPlaying = FALSE ;
					play_clean () ;
				}
			}
		}

		void clean () {
			INDEX ix = curr_node () ;
			assume (ix == root_node ()) ;
			mEnableClean = CRef<BOOL>::make (TRUE) ;
			mCleanQueue.clear () ;
			for (auto &&i : mTree.iter ()) {
				if (i == ix)
					continue ;
				if (mTree[i].mLifetime < mTree[i].mParent.length ())
					continue ;
				mCleanQueue.add (i) ;
			}
			play_clean () ;
		}

	private:
		template <class ARG1>
		INDEX insert_node (CREF<TYPEID<ARG1>> id) {
			const auto r1x = Clazz (id) ;
			const auto r2x = r1x.type_cabi () ;
			INDEX ret = mTreeCABISet.map (r2x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				ret = mTree.insert () ;
				mTreeCABISet.add (r2x ,ret) ;
				mTree[ret].mName = r1x.type_name () ;
				mTree[ret].mCABI = r2x ;
				mTree[ret].mIsFunction = FALSE ;
				mTree[ret].mIsIteration = FALSE ;
				mTree[ret].mPlaying = FALSE ;
				mTree[ret].mOncePlayed = FALSE ;
				mTree[ret].mPlayed = FALSE ;
				mTree[ret].mLifetime = 0 ;
				mTree[ret].mParent = Set<INDEX> (LINK_MIN_SIZE::expr) ;
				mTree[ret].mChild = Set<INDEX> (LINK_MIN_SIZE::expr) ;
				mTree[ret].mMaybe = Set<INDEX> (LINK_MIN_SIZE::expr) ;
				INDEX ix = root_node () ;
				mTree[ret].mOrder = mTree[ix].mChild.length () ;
				mTree[ret].mDepth = 1 ;
				mTree[ret].mMemoryUsage = 0 ;
				mTree[ret].mTimeCost = 0 ;
			}
			return move (ret) ;
		}

		INDEX root_node () const {
			return mTreeStack[mTreeStack.head ()] ;
		}

		INDEX curr_node () const {
			return mTreeStack[mTreeStack.tail ()] ;
		}

		void depth_track (CREF<INDEX> curr ,CREF<INDEX> prev) {
			mTrackStack.clear () ;
			const auto r1x = ARRAY2X (curr ,prev) ;
			mTrackStack.add (r1x) ;
			while (TRUE) {
				if (mTrackStack.empty ())
					break ;
				const auto r2x = mTrackStack[mTrackStack.tail ()] ;
				INDEX ix = r2x.mP1st ;
				INDEX iy = r2x.mP2nd ;
				mTrackStack.pop () ;
				if ifswitch (TRUE) {
					const auto r3x = mTree[iy].mOrder ;
					const auto r4x = mTree[iy].mDepth + 1 ;
					if (mTree[ix].mOrder <= r3x)
						if (mTree[ix].mDepth >= r4x)
							discard ;
					if ifswitch (TRUE) {
						if (iy == root_node ())
							discard ;
						mTree[ix].mOrder = MathProc::min_of (mTree[ix].mOrder ,r3x) ;
					}
					mTree[ix].mDepth = MathProc::max_of (mTree[ix].mDepth ,r4x) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[ix].mPlaying)
							discard ;
						const auto r5x = ARRAY2X (mTree[ix].mOrder ,-mTree[ix].mDepth) ;
						mPlayPriority.add (r5x ,ix) ;
					}
					for (auto &&i : mTree[ix].mChild) {
						assume (i != curr) ;
						const auto r6x = ARRAY2X (i ,ix) ;
						mTrackStack.add (r6x) ;
					}
					for (auto &&i : mTree[ix].mMaybe) {
						assume (i != curr) ;
						const auto r7x = ARRAY2X (i ,ix) ;
						mTrackStack.add (r7x) ;
					}
				}
			}
		}

		void undo_scan (CREF<INDEX> curr ,CREF<INDEX> prev) {
			mScanQueue.clear () ;
			mScanQueue.add (curr) ;
			INDEX ix = NONE ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				mScanQueue.take (ix) ;
				for (auto &&i : mTree[ix].mChild) {
					if ifnot (mTree[i].mIsIteration)
						continue ;
					mScanQueue.add (i) ;
				}
				mTree[ix].mDepend.add (prev) ;
			}
		}

		void play_scan (CREF<INDEX> curr) {
			mScanQueue.clear () ;
			for (auto &&i : mTree[curr].mChild) {
				if (mTree[i].mPlaying)
					continue ;
				if (mTree[i].mPlayed)
					continue ;
				mScanQueue.add (i) ;
			}
			INDEX ix = NONE ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				mScanQueue.take (ix) ;
				const auto r1x = ARRAY2X (mTree[ix].mOrder ,-mTree[ix].mDepth) ;
				mPlayPriority.add (r1x ,ix) ;
				for (auto &&i : mTree[ix].mChild) {
					if (mTree[i].mPlaying)
						continue ;
					if (mTree[i].mPlayed)
						continue ;
					mScanQueue.add (i) ;
				}
				mTree[ix].mPlaying = TRUE ;
			}
		}

		void play_clean () {
			if (mEnableClean == NULL)
				return ;
			if ifnot (mEnableClean.self)
				return ;
			INDEX ix = NONE ;
			while (TRUE) {
				if (mCleanQueue.empty ())
					break ;
				mCleanQueue.take (ix) ;
				assume (ifnot (mTree[ix].mPlaying)) ;
				if ifswitch (TRUE) {
					if ifnot (mTree[ix].mIsFunction)
						if ifnot (mTree[ix].mIsIteration)
							discard ;
					for (auto &&i : mTree[ix].mChild) {
						mTree[i].mLifetime++ ;
						if (mTree[i].mLifetime < mTree[i].mParent.length ())
							continue ;
						mCleanQueue.add (i) ;
					}
				}
				if ifswitch (TRUE) {
					if ifnot (mTree[ix].mValue.exist ())
						discard ;
					mTree[ix].mActor = Function<void> () ;
					mTree[ix].mOnceActor = Function<void> () ;
					const auto r1x = HeapProc::instance ().usage_size () ;
					mTree[ix].mValue = AutoRef<> () ;
					const auto r2x = HeapProc::instance ().usage_size () ;
					const auto r3x = LENGTH (r1x - r2x) ;
					mTree[ix].mMemoryUsage = MathProc::min_of (mTree[ix].mMemoryUsage ,r3x) ;
				}
			}
		}
	} ;
} ;

using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;
} ;