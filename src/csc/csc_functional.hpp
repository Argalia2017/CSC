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
		Set<INDEX> mChild ;
		Set<INDEX> mDepend ;
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
		Set<FLAG> mTreeCABISet ;
		CRef<BOOL> mEnableClean ;
		Deque<INDEX> mTreeStack ;
		Priority<ARRAY2X> mPlayPriority ;
		Deque<ARRAY2X> mDepthStack ;
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
			mTree[ix].mParent = Set<INDEX> (LINK_MIN_SIZE::value) ;
			mTree[ix].mChild = Set<INDEX> (LINK_MIN_SIZE::value) ;
			mTree[ix].mDepend = Set<INDEX> (LINK_MIN_SIZE::value) ;
			mTree[ix].mMaybe = Set<INDEX> (LINK_MIN_SIZE::value) ;
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
		}

		template <class ARG1>
		void maybe (CREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			INDEX iy = insert_node (id) ;
			assume (ix != iy) ;
			if ifswitch (TRUE) {
				mTree[iy].mParent.add (ix) ;
				mTree[ix].mMaybe.add (iy) ;
				track_depth (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTree[iy].mValue.exist ())
					discard ;
				assume (ifnot (mTree[iy].mOncePlayed)) ;
				assume (ifnot (mTree[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				auto rax = AutoRef<Box<R1X>>::make () ;
				//@warn: mValue depend on it's address
				rax->acquire (id ,thiz) ;
				mTree[iy].mValue = move (rax) ;
				mTreeStack.pop () ;
			}
		}

		template <class ARG1>
		CREF<REMOVE_ALL<ARG1>> stack (CREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
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
				mTree[iy].mParent.add (ix) ;
				mTree[ix].mChild.add (iy) ;
				track_depth (iy ,ix) ;
			}
			if ifswitch (TRUE) {
				if (mTree[iy].mValue.exist ())
					discard ;
				assume (ifnot (mTree[iy].mOncePlayed)) ;
				assume (ifnot (mTree[iy].mPlayed)) ;
				mTreeStack.add (iy) ;
				auto rax = AutoRef<Box<R1X>>::make () ;
				//@warn: mValue depend on it's address
				rax->acquire (id ,thiz) ;
				mTree[iy].mValue = move (rax) ;
				mTreeStack.pop () ;
			}
			if ifswitch (TRUE) {
				if (mTree[ix].mPlaying)
					discard ;
				if ifnot (mTree[iy].mIsIteration)
					discard ;
				mTree[ix].mIsIteration = TRUE ;
			}
			return AutoRef<Box<R1X>>::from (mTree[iy].mValue)->self ;
		}

		template <class ARG1>
		CREF<REMOVE_ALL<ARG1>> later (CREF<ARG1> id) leftvalue {
			using R1X = CRef<REMOVE_ALL<ARG1>> ;
			INDEX ix = curr_node () ;
			assume (mTree[ix].mPlaying) ;
			assume (mTree[ix].mIsIteration) ;
			assume (mTree[ix].mLater.exist ()) ;
			return AutoRef<R1X>::from (mTree[ix].mLater)->self ;
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
		void undo (CREF<ARG1> id) {
			INDEX ix = curr_node () ;
			INDEX iy = insert_node (id) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			assume (mTree[ix].mDepth < mTree[iy].mDepth) ;
			if ifnot (mTree[iy].mPlayed)
				return ;
			undo_scan (iy) ;
			for (auto &&i : mTree[iy].mDepend) {
				mTree[i].mPlayed = FALSE ;
				mTree[i].mLifetime = 0 ;
			}
		}

		template <class ARG1 ,class ARG2>
		void redo (CREF<ARG1> id ,CREF<ARG2> obj) {
			INDEX ix = curr_node () ;
			INDEX iy = insert_node (id) ;
			assume (ifnot (mTree[iy].mPlayed)) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			assume (mTree[ix].mDepth < mTree[iy].mDepth) ;
			mTree[iy].mLater = AutoRef<CRef<ARG2>>::make (CRef<ARG2>::reference (obj)) ;
		}

		template <class ARG1 ,class ARG2>
		void redo (CREF<ARG1> ,RREF<ARG2>) = delete ;

		void play () {
			INDEX ix = curr_node () ;
			play_scan (ix) ;
			while (TRUE) {
				if (mPlayPriority.empty ())
					break ;
				INDEX jx = mPlayPriority.get (mPlayPriority.head ()) ;
				if (jx == ix)
					break ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if (mTree[jx].mOncePlayed)
						discard ;
					assume (mTree[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[jx].mOnceActor.exist ())
							discard ;
						const auto r1x = TimePoint::make_now () ;
						const auto r2x = HeapProc::instance ().usage_size () ;
						mTreeStack.add (jx) ;
						mTree[jx].mOnceActor () ;
						mTreeStack.pop () ;
						const auto r3x = HeapProc::instance ().usage_size () ;
						const auto r4x = TimePoint::make_now () ;
						mTree[jx].mMemoryUsage += LENGTH (r3x - r2x) ;
						mTree[jx].mTimeCost += (r4x - r1x).seconds () ;
					}
					mTree[jx].mOncePlayed = TRUE ;
					mTree[jx].mLifetime = 0 ;
					for (auto &&i : mTree[jx].mMaybe) {
						if (mTree[jx].mChild.find (i) != NONE)
							continue ;
						mTree[i].mParent.erase (jx) ;
						if (mTree[i].mLifetime < mTree[i].mParent.length ())
							continue ;
						mCleanQueue.add (i) ;
					}
					mTree[jx].mMaybe.clear () ;
					play_scan (jx) ;
				}
				if ifswitch (rxx) {
					if (mTree[jx].mPlayed)
						discard ;
					assume (mTree[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[jx].mActor.exist ())
							discard ;
						const auto r5x = TimePoint::make_now () ;
						const auto r6x = HeapProc::instance ().usage_size () ;
						mTreeStack.add (jx) ;
						mTree[jx].mActor () ;
						mTreeStack.pop () ;
						const auto r7x = HeapProc::instance ().usage_size () ;
						const auto r8x = TimePoint::make_now () ;
						mTree[jx].mMemoryUsage += LENGTH (r7x - r6x) ;
						mTree[jx].mTimeCost += (r8x - r5x).seconds () ;
					}
					mTree[jx].mPlayed = TRUE ;
					mTree[jx].mLifetime = 0 ;
					if ifswitch (TRUE) {
						if (mTree[jx].mIsFunction)
							discard ;
						if (mTree[jx].mIsIteration)
							discard ;
						for (auto &&i : mTree[jx].mChild) {
							mTree[i].mLifetime++ ;
							if (mTree[i].mLifetime < mTree[i].mParent.length ())
								continue ;
							mCleanQueue.add (i) ;
						}
					}
				}
				if ifswitch (rxx) {
					mPlayPriority.take () ;
					mTree[jx].mPlaying = FALSE ;
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
		INDEX insert_node (CREF<ARG1> id) {
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
				mTree[ret].mParent = Set<INDEX> (LINK_MIN_SIZE::value) ;
				mTree[ret].mChild = Set<INDEX> (LINK_MIN_SIZE::value) ;
				mTree[ret].mDepend = Set<INDEX> (LINK_MIN_SIZE::value) ;
				mTree[ret].mMaybe = Set<INDEX> (LINK_MIN_SIZE::value) ;
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

		void track_depth (CREF<INDEX> curr ,CREF<INDEX> prev) {
			mDepthStack.clear () ;
			const auto r1x = ARRAY2X (curr ,prev) ;
			mDepthStack.add (r1x) ;
			while (TRUE) {
				if (mDepthStack.empty ())
					break ;
				const auto r2x = mDepthStack[mDepthStack.tail ()] ;
				INDEX ix = r2x.mP1st ;
				INDEX iy = r2x.mP2nd ;
				mDepthStack.pop () ;
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
						mDepthStack.add (r6x) ;
					}
					for (auto &&i : mTree[ix].mMaybe) {
						assume (i != curr) ;
						const auto r7x = ARRAY2X (i ,ix) ;
						mDepthStack.add (r7x) ;
					}
				}
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
			INDEX jx = NONE ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				mScanQueue.take (jx) ;
				const auto r1x = ARRAY2X (mTree[jx].mOrder ,-mTree[jx].mDepth) ;
				mPlayPriority.add (r1x ,jx) ;
				for (auto &&i : mTree[jx].mChild) {
					if (mTree[i].mPlaying)
						continue ;
					if (mTree[i].mPlayed)
						continue ;
					mScanQueue.add (i) ;
				}
				mTree[jx].mPlaying = TRUE ;
			}
		}

		void undo_scan (CREF<INDEX> curr) {
			if ifnot (mTree[curr].mPlayed)
				return ;
			if (mTree[curr].mDepend.length () > 0)
				return ;
			mScanQueue.clear () ;
			mScanQueue.add (curr) ;
			INDEX jx = NONE ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				mScanQueue.take (jx) ;
				for (auto &&i : mTree[jx].mParent) {
					if ifnot (mTree[i].mPlayed)
						continue ;
					mScanQueue.add (i) ;
				}
				mTree[curr].mDepend.add (jx) ;
			}
		}

		void play_clean () {
			if (mEnableClean == NULL)
				return ;
			if ifnot (mEnableClean.self)
				return ;
			INDEX jx = NONE ;
			while (TRUE) {
				if (mCleanQueue.empty ())
					break ;
				mCleanQueue.take (jx) ;
				assume (ifnot (mTree[jx].mPlaying)) ;
				if ifswitch (TRUE) {			
					for (auto &&i : mTree[jx].mChild) {
						if (mTree[i].mLifetime < mTree[i].mParent.length ())
							continue ;
						mCleanQueue.add (i) ;
					}
				}
				if ifswitch (TRUE) {
					if ifnot (mTree[jx].mValue.exist ())
						discard ;
					mTree[jx].mActor = Function<void> () ;
					mTree[jx].mOnceActor = Function<void> () ;
					const auto r1x = HeapProc::instance ().usage_size () ;
					mTree[jx].mValue = AutoRef<> () ;
					const auto r2x = HeapProc::instance ().usage_size () ;
					const auto r3x = LENGTH (r1x - r2x) ;
					mTree[jx].mMemoryUsage = MathProc::min_of (mTree[jx].mMemoryUsage ,r3x) ;
				}
			}
		}
	} ;
} ;

using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;
} ;