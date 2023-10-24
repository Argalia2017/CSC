//#pragma once
//
//#include "csc.hpp"
//#include "csc_type.hpp"
//
//namespace CSC {
//class CaptureBase {
//protected:
//	LENGTH mRank ;
//
//public:
//	implicit CaptureBase () noexcept {
//		mRank = 0 ;
//	}
//
//	template <class...ARG1>
//	void invoke (CREF<void (*) (XREF<ARG1>...)> proc) const {
//		assert (proc != NULL) ;
//		assert (mRank == COUNT_OF<TYPEAS<ARG1...>>::expr) ;
//		invoke_impl (reinterpret_cast<CREF<void (*) (KILL<csc_pointer_t ,ARG1>...)>> (proc)) ;
//	}
//
//	template <class ARG1 ,class ARG2 ,class...ARG3>
//	void invoke_impl (CREF<ARG1> proc ,TYPEID<ARG2> ,CREF<ARG3>...param) const {
//		require (ENUM_GT_ZERO<ARG2>) ;
//		invoke_impl (proc ,TYPEAS<ENUM_DEC<ARG2>>::expr ,param... ,ENUM_CHECK<ARG2>::expr) ;
//	}
//
//	template <class ARG1 ,class...ARG2>
//	void invoke_impl (CREF<ARG1> proc ,TYPEID<ENUM_ZERO> ,CREF<ARG2>...param) const {
//		proc (param...) ;
//	}
//
//	template <class ARG1>
//	inline void operator() (CREF<ARG1> proc) const {
//		return invoke (proc) ;
//	}
//} ;
//
//template <class...T>
//class Capture {} ;
//
//struct FUNCTION_operator_capture {
//	template <class...ARG1>
//	Capture<XREF<ARG1>> operator() (XREF<ARG1>...initval) const {
//		return Capture<XREF<ARG1>> (forward<ARG1> (initval)...) ;
//	}
//} ;
//
//static constexpr auto operator_capture = FUNCTION_operator_capture () ;
//
//struct MakeBinder implement Interface {
//	virtual void invoke (csc_pointer_t where ,CREF<CaptureBase> initval) const = 0 ;
//} ;
//
//template <class>
//struct MakeImplBinder ;
//
//template <class R ,class...P>
//struct MakeImplBinder<R (P...)> implement MakeBinder {
//	void invoke (csc_pointer_t where ,CREF<CaptureBase> capture) const {
//		capture ([] (XREF<P>...initval) {
//			new (where) R (forward<P> (initval)...) ;
//		}) ;
//	}
//} ;
//
//
//class BoxBase {} ;
//
//template <class T>
//class BoseWrap1 implement BoxBase {
//protected:
//	TEMP<T> mValue ;
//
//public:
//	implicit BoseWrap1 () = default ;
//
//	BOOL exist () const {
//		return true ;
//	}
//
//	void acquire (CREF<BoxBase> that) {
//		acquire_impl (static_cast<CREF<BoseWrap1>> (that)) ;
//	}
//
//	void acquire_impl (CREF<BoseWrap1> that) {
//		mValue = that.mValue ;
//	}
//
//	void release () {
//		operator_noop () ;
//	}
//} ;
//
//template <class T>
//class BoseWrap2 implement BoxBase {
//protected:
//	TEMP<T> mValue ;
//
//public:
//	implicit BoseWrap2 () noexcept {
//		operator_init (mValue) ;
//	}
//
//	implicit ~BoseWrap2 () noexcept {
//		if ifnot (exist ())
//			return ;
//		operator_drop (mValue) ;
//		operator_init (mValue) ;
//	}
//
//	implicit BoseWrap2 (RREF<BoseWrap2> that) noexcept {
//		mValue = that.mValue ;
//		operator_init (that.mValue) ;
//	}
//
//	implicit BoseWrap2 &operator= (RREF<BoseWrap2> that) noexcept {
//		if (&thiz == &that)
//			return &thiz ;
//		operator_swap (thiz ,that) ;
//		return &thiz ;
//	}
//
//	BOOL exist () const {
//		const auto r1x = FLAG (operator_bitwise (fake)) ;
//		return r1x != 0 ;
//	}
//
//	void acquire (CREF<BoxBase> that) {
//		acquire_impl (static_cast<CREF<BoseWrap2>> (that)) ;
//	}
//
//	void acquire_impl (CREF<BoseWrap2> that) {
//		mValue = that.mValue ;
//	}
//
//	void release () {
//		operator_init (mValue) ;
//	}
//
//private:
//	CREF<Interface> fake_m () const leftvalue {
//		return reinterpret_cast<CREF<Interface>> (mValue) ;
//	}
//} ;
//
//template <class T>
//class BoseWrap3 implement BoxBase {
//protected:
//	TEMP<T> mValue ;
//	BOOL mExist ;
//
//public:
//	implicit BoseWrap3 () noexcept {
//		mExist = false ;
//	}
//
//	implicit ~BoseWrap3 () noexcept {
//		if ifnot (exist ())
//			return ;
//		operator_drop (mValue) ;
//		mExist = false ;
//	}
//
//	implicit BoseWrap3 (RREF<BoseWrap3> that) noexcept {
//		mValue = that.mValue ;
//		mExist = that.mExist ;
//		that.mExist = false ;
//	}
//
//	implicit BoseWrap3 &operator= (RREF<BoseWrap3> that) noexcept {
//		if (&thiz == &that)
//			return &thiz ;
//		operator_swap (thiz ,that) ;
//		return &thiz ;
//	}
//
//	BOOL exist () const {
//		return mExist ;
//	}
//
//	void acquire (CREF<BoxBase> that) {
//		acquire_impl (static_cast<CREF<BoseWrap3>> (that)) ;
//	}
//
//	void acquire_impl (CREF<BoseWrap3> that) {
//		mValue = that.mValue ;
//		mExist = that.mExist ;
//	}
//
//	void release () {
//		mExist = false ;
//	}
//} ;
//
//template <class...>
//struct BoseWrapHelp ;
//
//template <class T>
//struct BoseWrapHelp<T ,REQUIRE<IS_TRIVIAL<T>>> {
//	using RET = BoseWrap1<T> ;
//} ;
//
//template <class T>
//struct BoseWrapHelp<T ,REQUIRE<IS_POLYMORPHIC<T>>> {
//	using RET = BoseWrap2<T> ;
//} ;
//
//template <class T>
//using BoseWrap = typename BoseWrapHelp<T ,ALWAYS>::RET ;
//
//template <class T>
//class Box implement BoseWrap<T> {
//protected:
//	using BoseWrap<T>::mValue ;
//
//public:
//	implicit Box () = default ;
//
//	template <class...ARG1>
//	static forceinline Box make (XREF<ARG1>...initval) {
//		return make_impl (MakeImplBinder<T (ARG1...)> () ,operator_capture (initval)) ;
//	}
//
//	static Box make_impl (CREF<MakeBinder> binder ,CREF<CaptureBase> initval) {
//		Box ret ;
//		binder->invoke (&ret.mValue ,initval) ;
//		return move (ret) ;
//	}
//
//	BOOL exist () const {
//		return BoseWrap<T>::exist () ;
//	}
//
//	VREF<T> self_m () leftvalue {
//		assume (exist ()) ;
//		return reinterpret_cast<VREF<T>> (mValue) ;
//	}
//
//	PTR<VREF<T>> operator-> () leftvalue {
//		return &self ;
//	}
//
//	CREF<T> self_m () const leftvalue {
//		assume (exist ()) ;
//		return reinterpret_cast<CREF<T>> (mValue) ;
//	}
//
//	PTR<CREF<T>> operator-> () const leftvalue {
//		return &self ;
//	}
//
//	void acquire (CREF<BoxBase> that) {
//		return BoseWrap<T>::acquire (that) ;
//	}
//
//	void release () {
//		return BoseWrap<T>::release () ;
//	}
//} ;
//} ;