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
#include "csc_thread.hpp"

namespace CSC {
template <class...>
trait SYNTAXTREE_HELP ;

template <class DEPEND>
trait SYNTAXTREE_HELP<DEPEND ,ALWAYS> {
	using SYNTEAXTREE_LINK_MIN_SIZE = ENUMAS<VAL ,ENUMID<32>> ;

	struct NODE {
		String<STR> mName ;
		FLAG mCABI ;
		AutoRef<> mValue ;
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
		Set<INDEX> mMaybe ;
		Set<INDEX> mDepend ;
		INDEX mLater ;
		LENGTH mOrder ;
		LENGTH mDepth ;
		LENGTH mUsage ;
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
			mTree[ix].mLifetime = 1 ;
			mTree[ix].mParent = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			mTree[ix].mChild = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			mTree[ix].mMaybe = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			mTree[ix].mLater = NONE ;
			mTree[ix].mOrder = 0 ;
			mTree[ix].mDepth = 0 ;
			mTree[ix].mUsage = 0 ;
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
		void maybe (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			INDEX iy = insert_node (TYPEAS<R1X>::id) ;
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
				rax->acquire (TYPEAS<R1X>::id ,thiz) ;
				mTree[iy].mValue = move (rax) ;
				mTreeStack.pop () ;
			}
		}

		template <class ARG1>
		CREF<REMOVE_ALL<ARG1>> value (XREF<ARG1> id) leftvalue {
			using R1X = REMOVE_ALL<ARG1> ;
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			INDEX iy = insert_node (TYPEAS<R1X>::id) ;
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
				rax->acquire (TYPEAS<R1X>::id ,thiz) ;
				mTree[iy].mValue = move (rax) ;
				mTreeStack.pop () ;
			}
			return AutoRef<Box<R1X>>::from (mTree[iy].mValue)->self ;
		}

		void once (RREF<Function<void>> actor) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mOnceActor.exist ())) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mPlaying)) ;
			assume (ifnot (mTree[ix].mIsFunction)) ;
			mTree[ix].mOnceActor = move (actor) ;
		}

		void then (RREF<Function<void>> actor) {
			INDEX ix = curr_node () ;
			assume (ix != root_node ()) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			assume (ifnot (mTree[ix].mIsIteration)) ;
			mTree[ix].mActor = move (actor) ;
		}

		template <class ARG1>
		void later (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			INDEX ix = curr_node () ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			assume (mTree[ix].mIsIteration) ;
			INDEX iy = insert_node (TYPEAS<R1X>::id) ;
			assume (ix != iy) ;
			assume (mTree[iy].mLater == NONE) ;
			const auto r1x = mTree[ix].mValue.clazz () ;
			const auto r2x = mTree[iy].mValue.clazz () ;
			assume (r1x.type_size () == r2x.type_size ()) ;
			assume (r1x.type_align () == r2x.type_align ()) ;
			mTree[iy].mLater = ix ;
			auto &&tmp_a = unsafe_deref (unsafe_cast[TYPEAS<TEMP<AutoRef<R1X>>>::id] (unsafe_deptr (mTree[ix].mValue))) ;
			auto &&tmp_b = unsafe_deref (unsafe_cast[TYPEAS<TEMP<AutoRef<R1X>>>::id] (unsafe_deptr (mTree[iy].mValue))) ;
			mTree[ix].mActor = Function<void> ([&] () {
				swap (tmp_a.self ,tmp_b.self) ;
			}) ;
		}

		template <class ARG1>
		void undo (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			INDEX ix = curr_node () ;
			assume (ix == root_node ()) ;
			assume (ifnot (mTree[ix].mActor.exist ())) ;
			assume (ifnot (mTree[ix].mOncePlayed)) ;
			INDEX iy = insert_node (TYPEAS<R1X>::id) ;
			assume (ix != iy) ;
			assume (mTree[iy].mIsIteration) ;
			undo_scan (iy) ;
			for (auto &&i : mTree[iy].mDepend) {
				mTree[i].mPlayed = FALSE ;
				mTree[i].mLifetime = mTree[i].mParent.length () ;
			}
		}

		void play () {
			INDEX ix = curr_node () ;
			const auto r1x = mTree[ix].mDepth ;
			play_scan (ix) ;
			while (TRUE) {
				if (mPlayPriority.empty ())
					break ;
				INDEX jx = mPlayPriority.get (mPlayPriority.head ()) ;
				if (mTree[jx].mDepth <= r1x)
					break ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if (mTree[jx].mOncePlayed)
						discard ;
					assume (mTree[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[jx].mOnceActor.exist ())
							discard ;
						const auto r2x = HeapProc::instance ().usage () ;
						mTreeStack.add (jx) ;
						mTree[jx].mOnceActor () ;
						mTreeStack.pop () ;
						const auto r3x = HeapProc::instance ().usage () ;
						mTree[jx].mUsage += r3x - r2x ;
					}
					mTree[jx].mOncePlayed = TRUE ;
					for (auto &&i : mTree[jx].mMaybe) {
						if (mTree[jx].mChild.find (i) != NONE)
							continue ;
						mTree[i].mLifetime-- ;
						if (mTree[i].mLifetime > 0)
							continue ;
						mCleanQueue.add (i) ;
					}
					play_scan (jx) ;
					if ifnot (mTree[jx].mIsIteration)
						discard ;
					mTree[jx].mPlayed = TRUE ;
					mTree[jx].mLifetime = mTree[jx].mParent.length () ;
					if ifswitch (TRUE) {
						if (mTree[jx].mIsFunction)
							discard ;
						for (auto &&i : mTree[jx].mChild) {
							mTree[i].mLifetime-- ;
							if (mTree[i].mLifetime > 0)
								continue ;
							mCleanQueue.add (i) ;
						}
					}
				}
				if ifswitch (eax) {
					if (mTree[jx].mPlayed)
						discard ;
					assume (mTree[jx].mValue.exist ()) ;
					if ifswitch (TRUE) {
						if ifnot (mTree[jx].mActor.exist ())
							discard ;
						const auto r4x = HeapProc::instance ().usage () ;
						mTreeStack.add (jx) ;
						mTree[jx].mActor () ;
						mTreeStack.pop () ;
						const auto r5x = HeapProc::instance ().usage () ;
						mTree[jx].mUsage += r5x - r4x ;
					}
					mTree[jx].mPlayed = TRUE ;
					mTree[jx].mLifetime = mTree[jx].mParent.length () ;
					if ifswitch (TRUE) {
						if (mTree[jx].mIsFunction)
							discard ;
						for (auto &&i : mTree[jx].mChild) {
							mTree[i].mLifetime-- ;
							if (mTree[i].mLifetime > 0)
								continue ;
							mCleanQueue.add (i) ;
						}
					}
				}
				if ifswitch (eax) {
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
				if (mTree[i].mLifetime > 0)
					continue ;
				mCleanQueue.add (i) ;
			}
			play_clean () ;
		}

	private:
		template <class ARG1>
		INDEX insert_node (XREF<ARG1> id) {
			const auto r1x = operator_cabi (id) ;
			INDEX ret = mTreeCABISet.map (r1x) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				ret = mTree.insert () ;
				mTreeCABISet.add (r1x ,ret) ;
				mTree[ret].mName = operator_name (id) ;
				mTree[ret].mCABI = r1x ;
				mTree[ret].mIsFunction = FALSE ;
				mTree[ret].mIsIteration = FALSE ;
				mTree[ret].mPlaying = FALSE ;
				mTree[ret].mOncePlayed = FALSE ;
				mTree[ret].mPlayed = FALSE ;
				mTree[ret].mLifetime = 1 ;
				mTree[ret].mParent = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
				mTree[ret].mChild = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
				mTree[ret].mMaybe = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
				mTree[ret].mLater = NONE ;
				INDEX ix = root_node () ;
				mTree[ret].mOrder = mTree[ix].mChild.length () ;
				mTree[ret].mDepth = 1 ;
				mTree[ret].mUsage = 0 ;
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
					const auto r3x = mTree[iy].mOrder ;
					const auto r4x = mTree[iy].mDepth + 1 ;
					if (mTree[ix].mOrder <= r3x)
						if (mTree[ix].mDepth >= r4x)
							discard ;
					if ifswitch (TRUE) {
						if (iy == root_node ())
							discard ;
						mTree[ix].mOrder = min_of (mTree[ix].mOrder ,r3x) ;
					}
					mTree[ix].mDepth = max_of (mTree[ix].mDepth ,r4x) ;
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
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				INDEX jx = mScanQueue[mScanQueue.head ()] ;
				mScanQueue.take () ;
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
			if (mTree[curr].mDepend.size () > 0)
				return ;
			mScanQueue.clear () ;
			mScanQueue.add (curr) ;
			mTree[curr].mDepend = Set<INDEX> (SYNTEAXTREE_LINK_MIN_SIZE::value) ;
			while (TRUE) {
				if (mScanQueue.empty ())
					break ;
				INDEX jx = mScanQueue[mScanQueue.head ()] ;
				mScanQueue.take () ;
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
			while (TRUE) {
				if (mCleanQueue.empty ())
					break ;
				INDEX jx = mCleanQueue[mCleanQueue.head ()] ;
				mCleanQueue.take () ;
				assume (ifnot (mTree[jx].mPlaying)) ;
				for (auto &&i : mTree[jx].mChild) {
					mTree[i].mLifetime-- ;
					if (mTree[i].mLifetime > 0)
						continue ;
					mCleanQueue.add (i) ;
				}
				mTree[jx].mActor = Function<void> () ;
				mTree[jx].mValue = AutoRef<> () ;
			}
		}
	} ;
} ;

using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;
} ;