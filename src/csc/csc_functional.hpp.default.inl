#pragma once

#ifndef __CSC_FUNCTIONAL__
#error "∑(っ°Д° ;)っ : require 'csc_functional.hpp'"
#endif

#include "csc_functional.hpp"

namespace CSC {
template <class DEPEND>
trait SYNTAXTREE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::Holder ;
	using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;
	using LINK_MIN_SIZE = ENUMAS<VAL ,32> ;

	struct NODE {
		String<STR> mName ;
		FLAG mCabi ;
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

	using INDEX2X = Tuple<INDEX ,INDEX> ;

	class ImplHolder implement Holder {
	protected:
		List<NODE> mTree ;
		Array<ArrayList<NODE>> mThreadTree ;
		Set<FLAG> mTreeCabiSet ;
		CRef<BOOL> mEnableClean ;
		Deque<INDEX> mTreeStack ;
		Priority<INDEX2X> mPlayPriority ;
		Deque<INDEX2X> mTrackStack ;
		Deque<INDEX> mCleanQueue ;
		Deque<INDEX> mScanQueue ;

	public:
		void initialize () override {
			INDEX ix = mTree.insert () ;
			mTree[ix].mName = slice ("") ;
			mTree[ix].mCabi = 0 ;
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

		INDEX insert_node (CREF<Clazz> clazz) {
			const auto r1x = clazz.type_name () ;
			const auto r2x = clazz.type_cabi () ;
			INDEX ret = mTreeCabiSet.map (r2x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				ret = mTree.insert () ;
				mTreeCabiSet.add (r2x ,ret) ;
				mTree[ret].mName = r1x ;
				mTree[ret].mCabi = r2x ;
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

		void mark_as_function () override {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			mTree[ix].mIsFunction = TRUE ;
		}

		void mark_as_iteration () override {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			mTree[ix].mIsIteration = TRUE ;
			mTree[ix].mDepend = Set<INDEX> (LINK_MIN_SIZE::expr) ;
			mTree[ix].mDepend.add (ix) ;
		}

		void maybe (CREF<Clazz> clazz ,CREF<Binder> binder ,VREF<SyntaxTree> tree) override {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			INDEX iy = insert_node (clazz) ;
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
				mTree[iy].mValue = binder.friend_create (tree) ;
				mTreeStack.pop () ;
			}
		}

		CREF<AutoRef<>> stack (CREF<Clazz> clazz ,CREF<Binder> binder ,VREF<SyntaxTree> tree) leftvalue override {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			INDEX iy = insert_node (clazz) ;
			assume (ix != iy) ;
			if ifswitch (TRUE) {
				if (mTree[ix].mChild.find (iy) != NONE)
					discard ;
				if ifswitch (TRUE) {
					if ifnot (mTree[ix].mPlaying)
						discard ;
					assume (mTree[iy].mParent.find (ix) != NONE) ;
					assume (mTree[ix].mMaybe.find (iy) != NONE) ;
				}
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
				mTree[iy].mValue = binder.friend_create (tree) ;
				mTreeStack.pop () ;
			}
			if ifswitch (TRUE) {
				if ifnot (mTree[ix].mIsIteration)
					discard ;
				if ifnot (mTree[iy].mIsIteration)
					discard ;
				undo_scan (iy ,ix) ;
			}
			return mTree[iy].mValue ;
		}

		void depth_track (CREF<INDEX> curr ,CREF<INDEX> prev) {
			mTrackStack.clear () ;
			const auto r1x = INDEX2X (curr ,prev) ;
			mTrackStack.add (r1x) ;
			while (TRUE) {
				if (mTrackStack.empty ())
					break ;
				const auto r2x = mTrackStack[mTrackStack.tail ()] ;
				INDEX ix = r2x.m1st ;
				INDEX iy = r2x.m2nd ;
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
						const auto r5x = INDEX2X (mTree[ix].mOrder ,-mTree[ix].mDepth) ;
						mPlayPriority.add (r5x ,ix) ;
					}
					for (auto &&i : mTree[ix].mChild) {
						assume (i != curr) ;
						const auto r6x = INDEX2X (i ,ix) ;
						mTrackStack.add (r6x) ;
					}
					for (auto &&i : mTree[ix].mMaybe) {
						assume (i != curr) ;
						const auto r7x = INDEX2X (i ,ix) ;
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

		void once (RREF<Function<void>> actor) override {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			mTree[ix].mOnceActor = move (actor) ;
		}

		void then (RREF<Function<void>> actor) override {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			mTree[ix].mActor = move (actor) ;
		}

		void undo (CREF<Clazz> clazz) override {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			INDEX iy = insert_node (clazz) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			assume (mTree[ix].mDepth < mTree[iy].mDepth) ;
			if ifnot (mTree[iy].mPlayed)
				return ;
			for (auto &&i : mTree[iy].mDepend) {
				if ifnot (mTree[i].mPlayed)
					continue ;
				mTree[i].mOncePlayed = FALSE ;
				mTree[i].mPlayed = FALSE ;
				mTree[i].mLifetime = 0 ;
			}
		}

		CREF<AutoRef<>> later () leftvalue override {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (mTree[ix].mPlaying) ;
			assume (mTree[ix].mIsIteration) ;
			assume (mTree[ix].mLater.exist ()) ;
			return mTree[ix].mLater ;
		}

		VREF<AutoRef<>> later (CREF<Clazz> clazz) leftvalue override {
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			INDEX iy = insert_node (clazz) ;
			assume (ifnot (mTree[iy].mPlayed)) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			assume (mTree[ix].mDepth < mTree[iy].mDepth) ;
			return mTree[iy].mLater ;
		}

		void play () override {
			INDEX ix = curr_node () ;
			play_scan (ix) ;
			while (TRUE) {
				if (mPlayPriority.empty ())
					break ;
				INDEX iy = mPlayPriority.map_get (mPlayPriority.head ()) ;
				if (mTree[iy].mDepth <= mTree[ix].mDepth)
					break ;
				auto act = TRUE ;
				if ifswitch (act) {
					if (mTree[iy].mOncePlayed)
						discard ;
					assume (mTree[iy].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[iy].mOnceActor.exist ())
							discard ;
						const auto r1x = NowTimePoint::make () ;
						const auto r2x = HeapProc::instance ().usage_size () ;
						mTreeStack.add (iy) ;
						mTree[iy].mOnceActor () ;
						mTreeStack.pop () ;
						const auto r3x = HeapProc::instance ().usage_size () ;
						const auto r4x = NowTimePoint::make () ;
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
				if ifswitch (act) {
					if (mTree[iy].mPlayed)
						discard ;
					assume (mTree[iy].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[iy].mActor.exist ())
							discard ;
						const auto r5x = NowTimePoint::make () ;
						const auto r6x = HeapProc::instance ().usage_size () ;
						mTreeStack.add (iy) ;
						mTree[iy].mActor () ;
						mTreeStack.pop () ;
						const auto r7x = HeapProc::instance ().usage_size () ;
						const auto r8x = NowTimePoint::make () ;
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
				if ifswitch (act) {
					mPlayPriority.take () ;
					mTree[iy].mPlaying = FALSE ;
					play_clean () ;
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
			INDEX ix = NONE ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				mScanQueue.take (ix) ;
				const auto r1x = INDEX2X (mTree[ix].mOrder ,-mTree[ix].mDepth) ;
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

		void clean () override {
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

template <>
exports auto SYNTAXTREE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SYNTAXTREE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;