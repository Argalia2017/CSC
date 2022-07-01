#pragma once

#ifndef __CSC_BASIC__
#error "∑(っ°Д° ;)っ : require 'csc_basic.hpp'"
#endif

namespace CSC {
template <class DEPEND>
trait GENERIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename GENERIC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		FLAG mPointer ;
		FLAG mQualifier ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<FLAG> addr ,CREF<FLAG> qualifier) override {
			mPointer = addr ;
			mQualifier = qualifier ;
		}

		FLAG pointer () const override {
			return mPointer ;
		}

		BOOL is_variable () const override {
			return mQualifier == VARIABLE::expr ;
		}

		BOOL is_constant () const override {
			return mQualifier == CONSTANT::expr ;
		}

		BOOL is_register () const override {
			return mQualifier == REGISTER::expr ;
		}
	} ;
} ;

template <>
exports auto GENERIC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> Box<FakeHolder> {
	using R1X = typename GENERIC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}

template <class DEPEND>
trait LATER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename LATER_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	using HEAP_SIZE = ENUMAS<VAL ,ENUMID<256>> ;

	template <class ARG1>
	using CRTP_HEAP = typename DEPENDENT<LATER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> ,ARG1>::HEAP ;

	struct OWNERSHIP {
		VRef<CRTP_HEAP<DEPEND>> mHeap ;
		INDEX mIndex ;
	} ;

	struct NODE {
		FLAG mTag ;
		Function<Auto> mExpr ;
		SharedRef<UniqueRef<OWNERSHIP>> mWeak ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	struct HEAP {
		BoxBuffer<NODE ,HEAP_SIZE> mList ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mFree ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<UniqueRef<OWNERSHIP>> mLater ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<FLAG> tag) override {
			const auto r1x = unique () ;
			assert (r1x.available ()) ;
			INDEX ix = r1x->mLast ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				if (r1x->mList[ix].mTag == tag)
					break ;
				ix = r1x->mList[ix].mPrev ;
			}
			if (ix == NONE)
				return ;
			assert (r1x->mList[ix].mPrev != USED) ;
			mLater = r1x->mList[ix].mWeak ;
			assume (mLater.available ()) ;
		}

		void initialize (CREF<FLAG> tag ,RREF<Function<Auto>> expr_) override {
			assert (expr_.exist ()) ;
			const auto r1x = unique () ;
			assert (r1x.available ()) ;
			INDEX ix = r1x->mFree ;
			assume (ix != NONE) ;
			auto rax = UniqueRef<OWNERSHIP> ([&] (VREF<OWNERSHIP> me) {
				me.mHeap = VRef<HEAP>::reference (r1x.self) ;
				me.mHeap->mList[ix].mTag = tag ;
				me.mHeap->mList[ix].mExpr = move (expr_) ;
				me.mHeap->mFree = me.mHeap->mList[ix].mNext ;
				me.mHeap->mList[ix].mPrev = me.mHeap->mLast ;
				me.mHeap->mList[ix].mNext = NONE ;
				curr_next (me.mHeap ,me.mHeap->mLast ,ix) ;
				me.mHeap->mLast = ix ;
				me.mIndex = ix ;
			} ,[] (VREF<OWNERSHIP> me) {
				INDEX ix = me.mIndex ;
				if (me.mHeap->mList[ix].mPrev == USED)
					return ;
				curr_next (me.mHeap ,me.mHeap->mList[ix].mPrev ,me.mHeap->mList[ix].mNext) ;
				curr_prev (me.mHeap ,me.mHeap->mList[ix].mNext ,me.mHeap->mList[ix].mPrev) ;
				me.mHeap->mList[ix].mPrev = USED ;
				me.mHeap->mList[ix].mNext = me.mHeap->mFree ;
				me.mHeap->mFree = ix ;
				me.mHeap->mList[ix].mTag = 0 ;
				me.mHeap->mList[ix].mExpr = Function<Auto> () ;
			}) ;
			mLater = SharedRef<UniqueRef<OWNERSHIP>>::make (move (rax)) ;
			r1x->mList[ix].mWeak = mLater.weak () ;
		}

		BOOL exist () const override {
			if ifnot (mLater.exist ())
				return FALSE ;
			if ifnot (mLater->exist ())
				return FALSE ;
			return TRUE ;
		}

		Auto invoke () const {
			return mLater->self.mHeap->mList[mLater->self.mIndex].mExpr () ;
		}

		imports SharedRef<HEAP> unique () {
			return memorize ([&] () {
				SharedRef<HEAP> ret = SharedRef<HEAP>::make () ;
				ret->mList = BoxBuffer<NODE ,HEAP_SIZE> (0) ;
				update_reserve (ret.self) ;
				return move (ret) ;
			}) ;
		}

		imports void update_reserve (VREF<HEAP> heap) {
			INDEX ix = NONE ;
			for (auto &&i : iter (0 ,heap.mList.size ())) {
				INDEX iy = heap.mList.size () - 1 - i ;
				heap.mList[iy].mPrev = USED ;
				heap.mList[iy].mNext = ix ;
				ix = iy ;
			}
			heap.mFirst = NONE ;
			heap.mLast = NONE ;
			heap.mFree = ix ;
		}

		imports void curr_next (VREF<HEAP> heap ,CREF<INDEX> curr ,CREF<INDEX> next) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (curr == NONE)
					discard ;
				heap.mList[curr].mNext = next ;
			}
			if ifswitch (rxx) {
				heap.mFirst = next ;
			}
		}

		imports void curr_prev (VREF<HEAP> heap ,CREF<INDEX> curr ,CREF<INDEX> prev) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (curr == NONE)
					discard ;
				heap.mList[curr].mPrev = prev ;
			}
			if ifswitch (rxx) {
				heap.mLast = prev ;
			}
		}
	} ;
} ;

template <>
exports auto LATER_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename LATER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait INTEGER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename INTEGER_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		VarBuffer<BYTE> mInteger ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<VAL64> value_ ,CREF<LENGTH> size_) override {
			mInteger = VarBuffer<BYTE> (size_) ;
			const auto r1x = vmin (mInteger.size () ,SIZE_OF<VAL64>::expr) ;
			const auto r2x = DATA (value_) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r3x = BYTE ((r2x >> (i * 8)) & DATA (0XFF)) ;
				mInteger[i] = r3x ;
			}
			const auto r4x = invoke ([&] () {
				if (value_ >= 0)
					return BYTE (0X00) ;
				return BYTE (0XFF) ;
			}) ;
			BufferProc::buf_fill (mInteger ,r4x ,r1x ,mInteger.size ()) ;
		}

		void initialize (CREF<TEMP<void>> integer_) override {
			auto rax = Box<VarBuffer<BYTE>> () ;
			rax.acquire (integer_) ;
			mInteger = move (rax.self) ;
		}

		Integer factory (RREF<VarBuffer<BYTE>> that) const {
			auto rax = Box<VarBuffer<BYTE>>::make (move (that)) ;
			Integer ret = Integer (unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (rax.self))) ;
			rax.release () ;
			return move (ret) ;
		}

		BOOL equal (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			assert (mInteger.size () == tmp.mInteger.size ()) ;
			return BufferProc::buf_equal (mInteger ,tmp.mInteger ,0 ,mInteger.size ()) ;
		}

		FLAG compr (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			assert (mInteger.size () == tmp.mInteger.size ()) ;
			return BufferProc::buf_compr (mInteger ,tmp.mInteger ,0 ,mInteger.size ()) ;
		}

		FLAG hash () const override {
			return BufferProc::buf_hash (mInteger ,0 ,mInteger.size ()) ;
		}

		Integer add (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			assert (mInteger.size () == tmp.mInteger.size ()) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				const auto r1x = VAL64 (mInteger[i]) + VAL64 (tmp.mInteger[i]) + rax ;
				const auto r2x = DATA (r1x) ;
				rbx[i] = BYTE (r2x & DATA (0XFF)) ;
				rax = VAL64 (r2x >> 8) ;
			}
			return factory (move (rbx)) ;
		}

		Integer sub (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			assert (mInteger.size () == tmp.mInteger.size ()) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				const auto r1x = VAL64 (mInteger[i]) - VAL64 (tmp.mInteger[i]) - rax ;
				const auto r2x = VAL64 (r1x < 0) ;
				const auto r3x = r1x + VAL64 (256) * r2x ;
				const auto r4x = DATA (r3x) ;
				rbx[i] = BYTE (r4x & DATA (0XFF)) ;
				rax = r2x ;
			}
			return factory (move (rbx)) ;
		}

		Integer mul (CREF<LENGTH> scale) const override {
			assert (scale >= VAL32_MIN) ;
			assert (scale <= VAL32_MAX) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				const auto r1x = VAL64 (mInteger[i]) * VAL64 (scale) + rax ;
				const auto r2x = DATA (r1x) ;
				rbx[i] = BYTE (r2x & DATA (0XFF)) ;
				rax = VAL64 (r2x >> 8) ;
			}
			return factory (move (rbx)) ;
		}

		Integer div (CREF<LENGTH> scale) const override {
			assert (scale >= VAL32_MIN) ;
			assert (scale <= VAL32_MAX) ;
			assert (scale != ZERO) ;
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			auto rax = VAL64 (0) ;
			for (auto &&i : iter (0 ,mInteger.size ())) {
				const auto r1x = VAL64 (mInteger[i]) + rax ;
				const auto r2x = r1x / VAL64 (scale) ;
				const auto r3x = DATA (r2x) ;
				rbx[i] = BYTE (r3x & DATA (0XFF)) ;
				const auto r4x = r1x - r2x * VAL64 (scale) ;
				rax = r4x * VAL64 (256) ;
			}
			return factory (move (rbx)) ;
		}

		Integer mod (CREF<LENGTH> scale) const override {
			const auto r1x = clone () ;
			return r1x - r1x / scale * scale ;
		}

		Integer clone () const override {
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			for (auto &&i : iter (0 ,mInteger.size ()))
				rbx[i] = mInteger[i] ;
			return factory (move (rbx)) ;
		}

		Integer minus () const override {
			auto rbx = VarBuffer<BYTE> (mInteger.size ()) ;
			for (auto &&i : iter (0 ,mInteger.size ()))
				rbx[i] = ~mInteger[i] ;
			Integer ret = factory (move (rbx)) ;
			ret++ ;
			return move (ret) ;
		}

		void increase () override {
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= mInteger.size ())
					break ;
				mInteger[ix] = BYTE (LENGTH (mInteger[ix]) + 1) ;
				if (mInteger[ix] != BYTE (0X00))
					break ;
				ix++ ;
			}
		}

		void decrease () override {
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= mInteger.size ())
					break ;
				mInteger[ix] = BYTE (LENGTH (mInteger[ix]) - 1) ;
				if (mInteger[ix] != BYTE (0XFF))
					break ;
				ix++ ;
			}
		}
	} ;
} ;

template <>
exports auto INTEGER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> Box<FakeHolder> {
	using R1X = typename INTEGER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	Box<FakeHolder> ret ;
	ret.acquire (TYPEAS<R1X>::expr) ;
	return move (ret) ;
}
} ;