#pragma once

#ifndef __CSC_ALGORITHM__
#define __CSC_ALGORITHM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"
#include "csc_geometry.hpp"

namespace CSC {
template <class...>
trait SORTPROC_HELP ;

template <class...>
trait SORTPROC_IMPLHOLDER_HELP ;

template <class...>
trait SORTPROC_IMPLBINDER_HELP ;

template <class DEPEND>
trait SORTPROC_HELP<DEPEND ,ALWAYS> {
	struct Binder implement Interface {
		virtual FLAG friend_compare (CREF<INDEX> index1 ,CREF<INDEX> index2) const = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void sort (CREF<Binder> binder ,VREF<Array<INDEX>> range_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class SortProc {
	protected:
		VRef<Holder> mThis ;

	public:
		imports CREF<SortProc> instance () {
			return memorize ([&] () {
				SortProc ret ;
				ret.mThis = FUNCTION_extern::invoke () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		template <class ARG1>
		inline Array<INDEX> operator() (CREF<ARG1> array_) const {
			Array<INDEX> ret = Array<INDEX>::make (array_.iter ()) ;
			sort (array_ ,ret ,0 ,ret.length ()) ;
			return move (ret) ;
		}

		template <class ARG1>
		imports void sort (CREF<ARG1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			using R1X = typename SORTPROC_IMPLBINDER_HELP<ARG1 ,ALWAYS>::ImplBinder ;
			auto rax = R1X (CRef<ARG1>::reference (array_)) ;
			return instance ().mThis->sort (rax ,range_ ,begin_ ,end_) ;
		}
	} ;
} ;

template <class UNIT>
trait SORTPROC_IMPLBINDER_HELP<UNIT ,ALWAYS> {
	using Binder = typename SORTPROC_HELP<DEPEND ,ALWAYS>::Binder ;

	class ImplBinder implement Binder {
	protected:
		CRef<UNIT> mThat ;

	public:
		implicit ImplBinder () = delete ;

		explicit ImplBinder (RREF<CRef<UNIT>> that) {
			mThat = move (that) ;
		}

		FLAG friend_compare (CREF<INDEX> index1 ,CREF<INDEX> index2) const override {
			return operator_compr (mThat.self[index1] ,mThat.self[index2]) ;
		}
	} ;
} ;

using SortProc = typename SORTPROC_HELP<DEPEND ,ALWAYS>::SortProc ;

template <class...>
trait DISJOINTTABLE_HELP ;

template <class...>
trait DISJOINTTABLE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait DISJOINTTABLE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<LENGTH> size_) = 0 ;
		virtual void clear () = 0 ;
		virtual INDEX lead (CREF<INDEX> index) = 0 ;
		virtual void joint (CREF<INDEX> index1 ,CREF<INDEX> index2) = 0 ;
		virtual BitSet<> filter (CREF<INDEX> index ,RREF<BitSet<>> res) = 0 ;
		virtual Array<INDEX> linkage () = 0 ;
		virtual Array<BitSet<>> closure () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class DisjointTable {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit DisjointTable () = default ;

		explicit DisjointTable (CREF<SizeProxy> size_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (size_) ;
		}

		void clear () {
			return mThis->clear () ;
		}

		INDEX lead (CREF<INDEX> index) {
			return mThis->lead (index) ;
		}

		void joint (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			return mThis->joint (index1 ,index2) ;
		}

		BitSet<> filter (CREF<INDEX> index ,RREF<BitSet<>> res) {
			return mThis->filter (index ,move (res)) ;
		}

		Array<INDEX> linkage () {
			return mThis->linkage () ;
		}

		Array<BitSet<>> closure () {
			return mThis->closure () ;
		}
	} ;
} ;

using DisjointTable = typename DISJOINTTABLE_HELP<DEPEND ,ALWAYS>::DisjointTable ;

template <class...>
trait BINARYTABLE_HELP ;

template <class...>
trait BINARYTABLE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BINARYTABLE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<LENGTH> size_) = 0 ;
		virtual void clear () = 0 ;
		virtual LENGTH count (CREF<INDEX> from_) const = 0 ;
		virtual void link (CREF<INDEX> from_ ,CREF<INDEX> to_) = 0 ;
		virtual void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) = 0 ;
		virtual BOOL get (CREF<INDEX> from_ ,CREF<INDEX> to_) const = 0 ;
		virtual BitSet<> filter (CREF<INDEX> from_ ,RREF<BitSet<>> res) const = 0 ;
		virtual void optimize () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class BinaryTable {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit BinaryTable () = default ;

		explicit BinaryTable (CREF<SizeProxy> size_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (size_) ;
		}

		void clear () {
			return mThis->clear () ;
		}

		LENGTH count (CREF<INDEX> from_) const {
			return mThis->count (from_) ;
		}

		void link (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			return mThis->link (from_ ,to_) ;
		}

		void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) {
			return mThis->joint (from_ ,to_) ;
		}

		BOOL get (CREF<INDEX> from_ ,CREF<INDEX> to_) const {
			return mThis->get (from_ ,to_) ;
		}

		BitSet<> filter (CREF<INDEX> from_ ,RREF<BitSet<>> res) const {
			return mThis->filter (from_ ,move (res)) ;
		}

		void optimize () {
			return mThis->optimize () ;
		}
	} ;
} ;

using BinaryTable = typename BINARYTABLE_HELP<DEPEND ,ALWAYS>::BinaryTable ;

template <class...>
trait SEGMENTTABLE_HELP ;

template <class...>
trait SEGMENTTABLE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait SEGMENTTABLE_HELP<DEPEND ,ALWAYS> {
	using FLOAT = DOUBLE ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void set_tolerance (CREF<FLOAT> tolerance) = 0 ;
		virtual void add (CREF<FLOAT> lb ,CREF<FLOAT> rb) = 0 ;
		virtual void erase (CREF<FLOAT> lb ,CREF<FLOAT> rb) = 0 ;
		virtual FLOAT percent (CREF<FLOAT> lb ,CREF<FLOAT> rb) = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class SegmentTable {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit SegmentTable () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		void set_tolerance (CREF<FLOAT> tolerance) {
			return mThis->set_tolerance (tolerance) ;
		}

		void add (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			return mThis->add (lb ,rb) ;
		}

		void erase (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			return mThis->erase (lb ,rb) ;
		}

		FLOAT percent (CREF<FLOAT> lb ,CREF<FLOAT> rb) {
			return mThis->percent (lb ,rb) ;
		}
	} ;
} ;

using SegmentTable = typename SEGMENTTABLE_HELP<DEPEND ,ALWAYS>::SegmentTable ;
} ;