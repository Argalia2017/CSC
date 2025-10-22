#pragma once

#ifndef __CSC_CORE__
#define __CSC_CORE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"

namespace CSC {
struct FUNCTION_noop {
	forceinline void operator() () const noexcept {
		return ;
	}

	template <class ARG1>
	forceinline void operator() (CR<ARG1> a) const noexcept {
		return ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

struct FUNCTION_address {
	template <class ARG1>
	forceinline FLAG operator() (CR<ARG1> a) const noexcept {
		return FLAG (&Pointer::from (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

struct FUNCTION_inline_ifdo {
	forceinline BOOL operator() (CR<BOOL> flag) const noexcept {
		return FALSE ;
	}

	forceinline BOOL operator() (VR<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto inline_ifdo = FUNCTION_inline_ifdo () ;

struct CoreProcHolder implement Interface {
	imports CR<CoreProcHolder> expr_m () ;

	virtual BOOL inline_unittest () const = 0 ;
	virtual void inline_abort () const = 0 ;
	virtual void inline_notice (VR<Pointer> src) const = 0 ;
	virtual FLAG inline_type_name (CR<Pointer> squalor ,CR<FLAG> func_) const = 0 ;
	virtual Tuple<FLAG ,FLAG> inline_list_pair (CR<Pointer> squalor ,CR<LENGTH> step_) const = 0 ;
	virtual void inline_memset (VR<Pointer> dst ,CR<LENGTH> size_) const = 0 ;
	virtual void inline_memcpy (VR<Pointer> dst ,CR<Pointer> src ,CR<LENGTH> size_) const = 0 ;
	virtual FLAG inline_memcmp (CR<Pointer> dst ,CR<Pointer> src ,CR<LENGTH> size_) const = 0 ;
} ;

struct FUNCTION_inline_unittest {
	forceinline BOOL operator() () const noexcept {
		return CoreProcHolder::expr.inline_unittest () ;
	}
} ;

static constexpr auto inline_unittest = FUNCTION_inline_unittest () ;

struct FUNCTION_inline_abort {
	forceinline void operator() () const noexcept {
		return CoreProcHolder::expr.inline_abort () ;
	}
} ;

static constexpr auto inline_abort = FUNCTION_inline_abort () ;

struct FUNCTION_inline_notice {
	template <class ARG1 ,class ARG2>
	forceinline void operator() (TYPE<ARG1> ,XR<ARG2> src) const noexcept {
		static volatile auto mInstance = (&src) ;
		return CoreProcHolder::expr.inline_notice (Pointer::make (FLAG (mInstance))) ;
	}
} ;

static constexpr auto inline_notice = FUNCTION_inline_notice () ;

struct FUNCTION_inline_type_name {
	forceinline FLAG operator() (CR<Pointer> squalor ,CR<FLAG> func_) const noexcept {
		return CoreProcHolder::expr.inline_type_name (squalor ,func_) ;
	}
} ;

static constexpr auto inline_type_name = FUNCTION_inline_type_name () ;

struct FUNCTION_inline_list_pair {
	forceinline Tuple<FLAG ,FLAG> operator() (CR<Pointer> squalor ,CR<LENGTH> step_) const noexcept {
		return CoreProcHolder::expr.inline_list_pair (squalor ,step_) ;
	}
} ;

static constexpr auto inline_list_pair = FUNCTION_inline_list_pair () ;

struct FUNCTION_inline_memset {
	forceinline void operator() (VR<Pointer> dst ,CR<LENGTH> size_) const noexcept {
		return __macro_memset (dst ,size_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline void operator() (VR<ARG1> dst) const noexcept {
		return thiz (Pointer::from (dst) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memset = FUNCTION_inline_memset () ;

struct FUNCTION_inline_memcpy {
	forceinline void operator() (VR<Pointer> dst ,CR<Pointer> src ,CR<LENGTH> size_) const noexcept {
		return __macro_memcpy (dst ,src ,size_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline void operator() (VR<ARG1> dst ,CR<ARG1> src) const noexcept {
		return thiz (Pointer::from (dst) ,Pointer::from (src) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memcpy = FUNCTION_inline_memcpy () ;

struct FUNCTION_inline_memcmp {
	forceinline FLAG operator() (CR<Pointer> dst ,CR<Pointer> src ,CR<LENGTH> size_) const noexcept {
		return __macro_memcmp (dst ,src ,size_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline FLAG operator() (CR<ARG1> dst ,CR<ARG1> src) const noexcept {
		return thiz (Pointer::from (dst) ,Pointer::from (src) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto inline_memcmp = FUNCTION_inline_memcmp () ;

template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class A>
struct FUNCTION_keep_impl0 {
	template <class ARG1 ,class = REQUIRE<IS_RELATE<VR<A> ,VR<ARG1>>>>
	forceinline VR<A> operator() (VR<ARG1> a) const noexcept {
		return static_cast<VR<A>> (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_RELATE<CR<A> ,CR<ARG1>>>>
	forceinline CR<A> operator() (CR<ARG1> a) const noexcept {
		return static_cast<CR<A>> (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_RELATE<RR<A> ,RR<ARG1>>>>
	forceinline RR<A> operator() (RR<ARG1> a) const noexcept {
		return static_cast<RR<A>> (a) ;
	}
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,ORDINARY>>> {
	using RET = FUNCTION_keep_impl0<A> ;
} ;

template <class A>
struct FUNCTION_keep_impl1 {
	template <class ARG1 ,class = REQUIRE<IS_RELATE<VR<A> ,VR<ARG1>>>>
	forceinline VR<A> operator() (VR<ARG1> a) const noexcept {
		return static_cast<VR<A>> (a) ;
	}

	template <class ARG1>
	forceinline VR<A> operator() (CR<ARG1> a) const noexcept = delete ;

	template <class ARG1>
	forceinline VR<A> operator() (RR<ARG1> a) const noexcept = delete ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	using RET = FUNCTION_keep_impl1<A> ;
} ;

template <class A>
struct FUNCTION_keep_impl2 {
	template <class ARG1 ,class = REQUIRE<IS_RELATE<CR<A> ,VR<ARG1>>>>
	forceinline CR<A> operator() (VR<ARG1> a) const noexcept {
		return static_cast<CR<A>> (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_RELATE<CR<A> ,CR<ARG1>>>>
	forceinline CR<A> operator() (CR<ARG1> a) const noexcept {
		return static_cast<CR<A>> (a) ;
	}

	template <class ARG1>
	forceinline CR<A> operator() (RR<ARG1> a) const noexcept = delete ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	using RET = FUNCTION_keep_impl2<A> ;
} ;

template <class A>
struct FUNCTION_keep_impl3 {
	template <class ARG1 ,class = REQUIRE<IS_RELATE<RR<A> ,VR<ARG1>>>>
	forceinline RR<A> operator() (VR<ARG1> a) const noexcept {
		return static_cast<RR<A>> (a) ;
	}

	template <class ARG1>
	forceinline RR<A> operator() (CR<ARG1> a) const noexcept = delete ;

	template <class ARG1 ,class = REQUIRE<IS_RELATE<RR<A> ,RR<ARG1>>>>
	forceinline RR<A> operator() (RR<ARG1> a) const noexcept {
		return static_cast<RR<A>> (a) ;
	}
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,REGISTER>>> {
	using RET = FUNCTION_keep_impl3<A> ;
} ;

template <class A>
using FUNCTION_keep_impl = typename FUNCTION_keep_impl_HELP<REMOVE_CVR<A> ,REFLECT_REF<A> ,ALWAYS>::RET ;

struct FUNCTION_keep {
	template <class ARG1>
	forceinline consteval FUNCTION_keep_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_keep_impl<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	forceinline RR<ARG1> operator() (VR<ARG1> a) const noexcept {
		return static_cast<RR<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline ARG1 operator() (CR<ARG1> a) const {
		require (IS_CLONEABLE<ARG1>) ;
		return static_cast<CR<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline RR<ARG1> operator() (RR<ARG1> a) const noexcept {
		return static_cast<RR<ARG1>> (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_swap {
	template <class ARG1>
	forceinline void operator() (VR<ARG1> a ,VR<ARG1> b) const noexcept {
		//@warn: no class should depend on its address
		require (IS_DEFAULT<ARG1>) ;
		auto rax = Union<ARG1> () ;
		inline_memcpy (Pointer::from (rax) ,Pointer::from (a) ,SIZE_OF<ARG1>::expr) ;
		inline_memcpy (Pointer::from (a) ,Pointer::from (b) ,SIZE_OF<ARG1>::expr) ;
		inline_memcpy (Pointer::from (b) ,Pointer::from (rax) ,SIZE_OF<ARG1>::expr) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

struct FUNCTION_assign {
	template <class ARG1>
	forceinline VR<ARG1> operator() (VR<ARG1> a ,VR<ARG1> b) const noexcept {
		if (address (a) == address (b))
			return a ;
		a.~ARG1 () ;
		new (csc_device_t (&a)) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1>
	forceinline VR<ARG1> operator() (VR<ARG1> a ,CR<ARG1> b) const noexcept {
		if (address (a) == address (b))
			return a ;
		a.~ARG1 () ;
		new (csc_device_t (&a)) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1>
	forceinline VR<ARG1> operator() (VR<ARG1> a ,RR<ARG1> b) const noexcept = delete ;
} ;

static constexpr auto assign = FUNCTION_assign () ;

struct FUNCTION_replace {
	template <class ARG1 ,class ARG2>
	forceinline void operator() (VR<ARG1> a ,CR<ARG1> expect ,CR<ARG2> item) const noexcept {
		if (a != expect)
			return ;
		a = ARG1 (item) ;
	}
} ;

static constexpr auto replace = FUNCTION_replace () ;

struct FUNCTION_invoke {
	template <class ARG1>
	forceinline FUNCTION_RETURN<ARG1> operator() (RR<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

template <class A>
struct FUNCTION_bitwise_impl {
	template <class ARG1>
	forceinline A operator() (CR<ARG1> a) const noexcept {
		require (IS_TRIVIAL<A>) ;
		using R1X = A ;
		using R2X = CONDITIONAL<IS_SAME<ARG1 ,Pointer> ,A ,ARG1> ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		A ret ;
		inline_memcpy (Pointer::from (ret) ,Pointer::from (a) ,SIZE_OF<R1X>::expr) ;
		return move (ret) ;
	}
} ;

struct FUNCTION_bitwise {
	template <class ARG1>
	forceinline consteval FUNCTION_bitwise_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_bitwise_impl<ARG1> () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_memorize {
	template <class ARG1>
	forceinline CR<FUNCTION_RETURN<ARG1>> operator() (CR<ARG1> func) const {
		static const auto mInstance = func () ;
		return mInstance ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

struct FUNCTION_inline_vptr {
	forceinline FLAG operator() (CR<Interface> binder) const noexcept {
		return bitwise[TYPE<FLAG>::expr] (binder) ;
	}
} ;

static constexpr auto inline_vptr = FUNCTION_inline_vptr () ;

struct FUNCTION_inline_min {
	forceinline VAL operator() (CR<VAL> a ,CR<VAL> b) const noexcept {
		if (a <= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_min = FUNCTION_inline_min () ;

struct FUNCTION_inline_max {
	forceinline VAL operator() (CR<VAL> a ,CR<VAL> b) const noexcept {
		if (a >= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_max = FUNCTION_inline_max () ;

struct FUNCTION_inline_between {
	forceinline BOOL operator() (CR<VAL> val ,CR<VAL> begin_ ,CR<VAL> end_) const noexcept {
		if (val < begin_)
			return FALSE ;
		if (val >= end_)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto inline_between = FUNCTION_inline_between () ;

struct FUNCTION_inline_alignas {
	forceinline VAL operator() (CR<VAL> val ,CR<VAL> align) const noexcept {
		assert (align > 0) ;
		const auto r1x = val / align * align ;
		if (r1x == val)
			return r1x ;
		return r1x + align ;
	}
} ;

static constexpr auto inline_alignas = FUNCTION_inline_alignas () ;

struct FUNCTION_inline_clone {
	template <class ARG1>
	forceinline void operator() (VR<ARG1> a ,CR<ARG1> b) const noexcept {
		return clone_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLONEABLE<ARG1>>>
	forceinline void clone_impl (CR<typeof (PH3)> ,VR<ARG1> a ,CR<ARG1> b) const {
		assign (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_CLONE<ARG1>>>
	forceinline void clone_impl (CR<typeof (PH2)> ,VR<ARG1> a ,CR<ARG1> b) const {
		a = b.clone () ;
	}

	template <class ARG1>
	forceinline void clone_impl (CR<typeof (PH1)> ,VR<ARG1> a ,CR<ARG1> b) const {
		assert (FALSE) ;
	}
} ;

static constexpr auto inline_clone = FUNCTION_inline_clone () ;

struct FUNCTION_inline_equal {
	template <class ARG1>
	forceinline BOOL operator() (CR<ARG1> a ,CR<ARG1> b) const noexcept {
		return equal_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_EQUALABLE<ARG1>>>
	forceinline BOOL equal_impl (CR<typeof (PH4)> ,CR<ARG1> a ,CR<ARG1> b) const {
		return a == b ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_EQUAL<ARG1>>>
	forceinline BOOL equal_impl (CR<typeof (PH3)> ,CR<ARG1> a ,CR<ARG1> b) const {
		return a.equal (b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline BOOL equal_impl (CR<typeof (PH2)> ,CR<ARG1> a ,CR<ARG1> b) const {
		return inline_memcmp (a ,b) == ZERO ;
	}

	template <class ARG1>
	forceinline BOOL equal_impl (CR<typeof (PH1)> ,CR<ARG1> a ,CR<ARG1> b) const {
		assert (FALSE) ;
		return FALSE ;
	}
} ;

static constexpr auto inline_equal = FUNCTION_inline_equal () ;

struct FUNCTION_inline_compr {
	template <class ARG1>
	forceinline FLAG operator() (CR<ARG1> a ,CR<ARG1> b) const noexcept {
		return compr_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_COMPRABLE<ARG1>>>
	forceinline FLAG compr_impl (CR<typeof (PH4)> ,CR<ARG1> a ,CR<ARG1> b) const {
		if (a < b)
			return NONE ;
		if (a > b)
			return IDEN ;
		return ZERO ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_COMPR<ARG1>>>
	forceinline FLAG compr_impl (CR<typeof (PH3)> ,CR<ARG1> a ,CR<ARG1> b) const {
		return a.compr (b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_TRIVIAL<ARG1>>>
	forceinline FLAG compr_impl (CR<typeof (PH2)> ,CR<ARG1> a ,CR<ARG1> b) const {
		return inline_memcmp (a ,b) ;
	}

	template <class ARG1>
	forceinline FLAG compr_impl (CR<typeof (PH1)> ,CR<ARG1> a ,CR<ARG1> b) const {
		assert (FALSE) ;
		return ZERO ;
	}
} ;

static constexpr auto inline_compr = FUNCTION_inline_compr () ;

struct FUNCTION_inline_visit {
	template <class ARG1>
	forceinline void operator() (VR<FriendVisitor> visitor ,CR<ARG1> a) const noexcept {
		return visit_impl (PHX ,visitor ,a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline void visit_impl (CR<typeof (PH3)> ,VR<FriendVisitor> visitor ,CR<ARG1> a) const {
		const auto r1x = bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a) ;
		visitor.push (r1x) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_VISIT<ARG1>>>
	forceinline void visit_impl (CR<typeof (PH2)> ,VR<FriendVisitor> visitor ,CR<ARG1> a) const {
		return a.visit (visitor) ;
	}

	template <class ARG1>
	forceinline void visit_impl (CR<typeof (PH1)> ,VR<FriendVisitor> visitor ,CR<ARG1> a) const {
		assert (FALSE) ;
	}
} ;

static constexpr auto inline_visit = FUNCTION_inline_visit () ;

struct IndexIteratorLayout {
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit IndexIteratorLayout () noexcept {
		mBegin = 0 ;
		mEnd = 0 ;
		mPeek = 0 ;
	}
} ;

class IndexIterator implement IndexIteratorLayout {
protected:
	using IndexIteratorLayout::mBegin ;
	using IndexIteratorLayout::mEnd ;
	using IndexIteratorLayout::mPeek ;

public:
	implicit IndexIterator () = default ;

	explicit IndexIterator (CR<INDEX> begin_ ,CR<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = inline_max (begin_ ,end_) ;
		mPeek = mBegin ;
		if (length () > 0)
			return ;
		mPeek = mEnd ;
	}

	LENGTH length () const {
		return mEnd - mBegin ;
	}

	IndexIterator begin () const {
		return thiz ;
	}

	IndexIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek != mEnd ;
	}

	forceinline BOOL operator== (CR<IndexIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CR<IndexIterator>) const {
		return good () ;
	}

	CR<INDEX> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CR<INDEX> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek++ ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct Pixel {
	INDEX mX ;
	INDEX mY ;
} ;

struct PixelIteratorLayout {
	Pixel mBegin ;
	Pixel mEnd ;
	Pixel mPeek ;

public:
	implicit PixelIteratorLayout () noexcept {
		mBegin.mX = 0 ;
		mBegin.mY = 0 ;
		mEnd.mX = 0 ;
		mEnd.mY = 0 ;
		mPeek.mX = 0 ;
		mPeek.mY = 0 ;
	}
} ;

class PixelIterator implement PixelIteratorLayout {
protected:
	using PixelIteratorLayout::mBegin ;
	using PixelIteratorLayout::mEnd ;
	using PixelIteratorLayout::mPeek ;

public:
	implicit PixelIterator () = default ;

	explicit PixelIterator (CR<INDEX> begin_x ,CR<INDEX> end_x ,CR<INDEX> begin_y ,CR<INDEX> end_y) {
		mBegin.mX = begin_x ;
		mBegin.mY = begin_y ;
		mEnd.mX = inline_max (begin_x ,end_x) ;
		mEnd.mY = inline_max (begin_y ,end_y) ;
		mPeek = mBegin ;
		if (length () > 0)
			return ;
		mPeek = mEnd ;
	}

	LENGTH length () const {
		return (mEnd.mX - mBegin.mX) * (mEnd.mY - mBegin.mY) ;
	}

	PixelIterator begin () const {
		return thiz ;
	}

	PixelIterator end () const {
		return thiz ;
	}

	BOOL good () const {
		return mPeek.mY != mEnd.mY ;
	}

	forceinline BOOL operator== (CR<PixelIterator>) const {
		return (!good ()) ;
	}

	forceinline BOOL operator!= (CR<PixelIterator>) const {
		return good () ;
	}

	CR<Pixel> peek () const leftvalue {
		return mPeek ;
	}

	forceinline CR<Pixel> operator* () const leftvalue {
		return peek () ;
	}

	void next () {
		mPeek.mX++ ;
		if (mPeek.mX < mEnd.mX)
			return ;
		mPeek.mX = mBegin.mX ;
		mPeek.mY++ ;
	}

	forceinline void operator++ () {
		next () ;
	}
} ;

struct FUNCTION_range {
	forceinline IndexIterator operator() (CR<INDEX> begin_ ,CR<INDEX> end_) const noexcept {
		return IndexIterator (begin_ ,end_) ;
	}

	forceinline PixelIterator operator() (CR<INDEX> begin_x ,CR<INDEX> end_x ,CR<INDEX> begin_y ,CR<INDEX> end_y) const noexcept {
		return PixelIterator (begin_x ,end_x ,begin_y ,end_y) ;
	}
} ;

static constexpr auto range = FUNCTION_range () ;

struct FatLayout {
	FLAG mHolder ;
	FLAG mLayout ;
} ;

template <class A ,class B>
class Fat implement A {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mLayout ;

public:
	VR<B> self_m () leftvalue {
		return Pointer::make (mLayout) ;
	}

	CR<B> self_m () const leftvalue {
		return Pointer::make (mLayout) ;
	}
} ;

template <class A>
class VFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit VFat () = delete ;

	template <class ARG1 ,class ARG2>
	explicit VFat (CR<ARG1> holder ,VR<ARG2> that) {
		require (IS_EXTEND<A ,ARG1>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG1> ,SIZE_OF<VFat>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG1> ,ALIGN_OF<VFat>>) ;
		using R1X = typeof (nullof (ARG1).self) ;
		require (IS_SAME<R1X ,ARG2>) ;
		mHolder = inline_vptr (holder) ;
		mLayout = address (that) ;
	}

	VR<A> ref_m () const {
		return Pointer::from (const_cast<VR<VFat>> (thiz)) ;
	}

	forceinline PTR<VR<A>> operator-> () const {
		return (&ref) ;
	}
} ;

template <class A>
class CFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit CFat () = delete ;

	template <class ARG1 ,class ARG2>
	explicit CFat (CR<ARG1> holder ,CR<ARG2> that) {
		require (IS_EXTEND<A ,ARG1>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG1> ,SIZE_OF<CFat>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG1> ,ALIGN_OF<CFat>>) ;
		using R1X = typeof (nullof (ARG1).self) ;
		require (IS_SAME<R1X ,ARG2>) ;
		mHolder = inline_vptr (holder) ;
		mLayout = address (that) ;
	}

	CR<A> ref_m () const {
		return Pointer::from (const_cast<CR<CFat>> (thiz)) ;
	}

	forceinline PTR<CR<A>> operator-> () const {
		return (&ref) ;
	}
} ;

template <class A>
class RFat implement FatLayout {
private:
	require (IS_INTERFACE<A>) ;

protected:
	using FatLayout::mHolder ;
	using FatLayout::mLayout ;

public:
	implicit RFat () = delete ;

	explicit RFat (CR<FriendUnknown> unknown) {
		mHolder = unknown.reflect (A::expr) ;
		assert (mHolder != ZERO) ;
		mLayout = ZERO ;
	}

	CR<A> ref_m () const {
		return Pointer::from (const_cast<CR<RFat>> (thiz)) ;
	}

	forceinline PTR<CR<A>> operator-> () const {
		return (&ref) ;
	}
} ;

template <class A>
class SimpleUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == A::expr)
			return inline_vptr (A ()) ;
		return ZERO ;
	}
} ;

class Unknown {
protected:
	FLAG mHolder ;

public:
	implicit Unknown () = delete ;

	explicit Unknown (CR<FLAG> that) {
		assert (that != ZERO) ;
		mHolder = that ;
	}

	implicit Unknown (CR<FriendUnknown> that) {
		mHolder = inline_vptr (that) ;
	}

	forceinline operator CR<FriendUnknown> () const {
		return Pointer::from (thiz) ;
	}
} ;

struct ReflectSize implement Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 100 ;
	}
} ;

template <class A>
class ReflectSizeBinder final implement Fat<ReflectSize ,Proxy> {
public:
	LENGTH type_size () const override {
		return SIZE_OF<A>::expr ;
	}

	LENGTH type_align () const override {
		return ALIGN_OF<A>::expr ;
	}
} ;

struct ReflectCreate implement Interface {
	virtual void create (VR<Pointer> a ,CR<LENGTH> size_) const noexcept = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 101 ;
	}
} ;

template <class A>
class ReflectCreateBinder final implement Fat<ReflectCreate ,Proxy> {
private:
	require (IS_DEFAULT<A>) ;

public:
	void create (VR<Pointer> a ,CR<LENGTH> size_) const noexcept override {
		if (MACRO_IS_TRIVIAL_CONSTRUCTIBLE<A>::expr)
			return ;
		auto &&rax = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : range (0 ,size_)) {
			new (csc_device_t (&rax[i])) A () ;
		}
	}
} ;

struct ReflectDestroy implement Interface {
	virtual void destroy (VR<Pointer> a ,CR<LENGTH> size_) const noexcept = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 102 ;
	}
} ;

template <class A>
class ReflectDestroyBinder final implement Fat<ReflectDestroy ,Proxy> {
public:
	void destroy (VR<Pointer> a ,CR<LENGTH> size_) const noexcept override {
		if (MACRO_IS_TRIVIAL_DESTRUCTIBLE<A>::expr)
			return ;
		auto &&rax = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : range (0 ,size_)) {
			rax[i].~A () ;
		}
	}
} ;

struct ReflectAssign implement Interface {
	virtual void assign (VR<Pointer> a ,VR<Pointer> b) const noexcept = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 103 ;
	}
} ;

template <class A>
class ReflectAssignBinder final implement Fat<ReflectAssign ,Proxy> {
public:
	void assign (VR<Pointer> a ,VR<Pointer> b) const noexcept override {
		CSC::assign (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectCompile implement Interface {
	virtual void compile (VR<Pointer> a ,VR<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 104 ;
	}
} ;

template <class A>
class ReflectCompileBinder final implement Fat<ReflectCompile ,Proxy> {
public:
	void compile (VR<Pointer> a ,VR<Pointer> b) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		return rax.compile (b) ;
	}
} ;

struct ReflectTuple implement Interface {
	virtual LENGTH tuple_m1st () const = 0 ;
	virtual LENGTH tuple_m2nd () const = 0 ;
	virtual LENGTH tuple_m3rd () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 105 ;
	}
} ;

template <class A>
class ReflectTupleBinder final implement Fat<ReflectTuple ,Proxy> {
public:
	LENGTH tuple_m1st () const override {
		return tuple_m1st_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_M1ST<ARG1>>>
	forceinline LENGTH tuple_m1st_impl (CR<typeof (PH2)> ,TYPE<ARG1>) const {
		auto &&rax = keep[TYPE<A>::expr] (Pointer::make (SIZE_OF<A>::expr)) ;
		return address (rax.m1st) - address (rax) ;
	}

	template <class ARG1>
	forceinline LENGTH tuple_m1st_impl (CR<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}

	LENGTH tuple_m2nd () const override {
		return tuple_m2nd_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_M2ND<ARG1>>>
	forceinline LENGTH tuple_m2nd_impl (CR<typeof (PH2)> ,TYPE<ARG1>) const {
		auto &&rax = keep[TYPE<A>::expr] (Pointer::make (SIZE_OF<A>::expr)) ;
		return address (rax.m2nd) - address (rax) ;
	}

	template <class ARG1>
	forceinline LENGTH tuple_m2nd_impl (CR<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}

	LENGTH tuple_m3rd () const override {
		return tuple_m3rd_impl (PHX ,TYPE<A>::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_M3RD<ARG1>>>
	forceinline LENGTH tuple_m3rd_impl (CR<typeof (PH2)> ,TYPE<ARG1>) const {
		auto &&rax = keep[TYPE<A>::expr] (Pointer::make (SIZE_OF<A>::expr)) ;
		return address (rax.m3rd) - address (rax) ;
	}

	template <class ARG1>
	forceinline LENGTH tuple_m3rd_impl (CR<typeof (PH1)> ,TYPE<ARG1>) const {
		assert (FALSE) ;
		return 0 ;
	}
} ;

struct BoxLayout {
	FLAG mHolder ;

public:
	implicit BoxLayout () noexcept {
		mHolder = ZERO ;
	}

	implicit ~BoxLayout () noexcept ;

	implicit BoxLayout (CR<BoxLayout> that) = delete ;

	forceinline VR<BoxLayout> operator= (CR<BoxLayout> that) = delete ;

	implicit BoxLayout (RR<BoxLayout> that) noexcept :BoxLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<BoxLayout> operator= (RR<BoxLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct BoxHolder implement Interface {
	imports VFat<BoxHolder> hold (VR<BoxLayout> that) ;
	imports CFat<BoxHolder> hold (CR<BoxLayout> that) ;

	virtual void initialize (CR<Unknown> holder) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual void remake (CR<Unknown> holder) = 0 ;
	virtual void acquire (CR<BoxLayout> that) = 0 ;
	virtual void release () = 0 ;
} ;

inline BoxLayout::~BoxLayout () noexcept {
	BoxHolder::hold (thiz)->destroy () ;
}

template <class A>
class BoxUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A ,class B = A>
class Box implement BoxLayout {
protected:
	using BoxLayout::mHolder ;
	Union<B> mStorage ;

public:
	implicit Box () = default ;

	implicit Box (CR<typeof (NULL)>) {}

	template <class ARG1 ,class = REQUIRE<IS_NOT_SAME<ARG1 ,B>>>
	implicit Box (RR<Box<A ,ARG1>> that) {
		require (ENUM_EQUAL<SIZE_OF<B> ,SIZE_OF<ARG1>>) ;
		require (ENUM_EQUAL<ALIGN_OF<B> ,ALIGN_OF<ARG1>>) ;
		acquire (that) ;
		that.release () ;
	}

	template <class...ARG1>
	static Box make (XR<ARG1>...initval) {
		Box ret ;
		ret.remake (keep[TYPE<XR<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	static Box zeroize () {
		Box ret ;
		BoxHolder::hold (ret)->initialize (BoxUnknownBinder<A> ()) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return BoxHolder::hold (thiz)->exist () ;
	}

	forceinline BOOL operator== (CR<typeof (NULL)>) const {
		return (!exist ()) ;
	}

	forceinline BOOL operator!= (CR<typeof (NULL)>) const {
		return exist () ;
	}

	Unknown unknown () const {
		return BoxHolder::hold (thiz)->unknown () ;
	}

	VR<A> ref_m () leftvalue {
		return BoxHolder::hold (thiz)->ref ;
	}

	forceinline PTR<VR<A>> operator-> () leftvalue {
		return (&ref) ;
	}

	CR<A> ref_m () const leftvalue {
		return BoxHolder::hold (thiz)->ref ;
	}

	forceinline PTR<CR<A>> operator-> () const leftvalue {
		return (&ref) ;
	}

	template <class...ARG1>
	void remake (XR<ARG1>...initval) {
		if (exist ())
			return ;
		new (csc_device_t (&mStorage)) A (keep[TYPE<XR<ARG1>>::expr] (initval)...) ;
		BoxHolder::hold (thiz)->remake (BoxUnknownBinder<A> ()) ;
	}

	void acquire (CR<BoxLayout> that) {
		return BoxHolder::hold (thiz)->acquire (that) ;
	}

	void release () {
		return BoxHolder::hold (thiz)->release () ;
	}
} ;

struct PinLayout {
	FLAG mLayout ;

public:
	implicit PinLayout () noexcept {
		mLayout = ZERO ;
	}

	implicit PinLayout (CR<PinLayout> that) = delete ;

	forceinline VR<PinLayout> operator= (CR<PinLayout> that) = delete ;

	implicit PinLayout (RR<PinLayout> that) noexcept :PinLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<PinLayout> operator= (RR<PinLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

template <class A>
class Pin implement PinLayout {
protected:
	using PinLayout::mLayout ;
	mutable Union<A> mStorage ;

public:
	implicit Pin () = delete ;

	explicit Pin (CR<A> that) {
		mLayout = address (that) ;
		auto &&rax = keep[TYPE<Union<A>>::expr] (Pointer::make (mLayout)) ;
		mStorage = rax ;
	}

	explicit Pin (RR<A> that) = delete ;

	implicit ~Pin () noexcept {
		if (mLayout == ZERO)
			return ;
		auto &&rax = keep[TYPE<Union<A>>::expr] (Pointer::make (mLayout)) ;
		rax = mStorage ;
	}

	implicit Pin (CR<Pin> that) = delete ;

	forceinline VR<Pin> operator= (CR<Pin> that) = delete ;

	implicit Pin (RR<Pin> that) = default ;

	forceinline VR<Pin> operator= (RR<Pin> that) = default ;

	VR<A> ref_m () const leftvalue {
		return Pointer::from (mStorage) ;
	}

	forceinline PTR<VR<A>> operator-> () const leftvalue {
		return (&ref) ;
	}
} ;

struct RefLayout {
	FLAG mHandle ;
	FLAG mLayout ;

public:
	implicit RefLayout () noexcept {
		mHandle = ZERO ;
		mLayout = ZERO ;
	}

	implicit ~RefLayout () noexcept ;

	implicit RefLayout (CR<RefLayout> that) = delete ;

	forceinline VR<RefLayout> operator= (CR<RefLayout> that) = delete ;

	implicit RefLayout (RR<RefLayout> that) noexcept :RefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VR<RefLayout> operator= (RR<RefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefHolder implement Interface {
	imports VFat<RefHolder> hold (VR<RefLayout> that) ;
	imports CFat<RefHolder> hold (CR<RefLayout> that) ;

	virtual void initialize (RR<BoxLayout> item) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Unknown> extend ,CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<FLAG> layout) = 0 ;
	virtual void destroy () = 0 ;
	virtual RefLayout share () const = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual Unknown unknown () const = 0 ;
	virtual VR<Pointer> ref_m () leftvalue = 0 ;
	virtual CR<Pointer> ref_m () const leftvalue = 0 ;
	virtual BOOL exclusive () const = 0 ;
} ;

inline RefLayout::~RefLayout () noexcept {
	RefHolder::hold (thiz)->destroy () ;
}

template <class A>
class RefUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Ref implement RefLayout {
protected:
	using RefLayout::mHandle ;
	using RefLayout::mLayout ;

public:
	implicit Ref () = default ;

	implicit Ref (CR<typeof (NULL)>) {}

	template <class...ARG1>
	static Ref make (XR<ARG1>...initval) {
		Ref ret ;
		auto rax = Box<A>::make (keep[TYPE<XR<ARG1>>::expr] (initval)...) ;
		RefHolder::hold (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	static Ref reference (VR<A> that) {
		Ref ret ;
		ret.mHandle = VARIABLE::expr ;
		ret.mLayout = address (that) ;
		return move (ret) ;
	}

	static Ref reference (CR<A> that) {
		Ref ret ;
		ret.mHandle = CONSTANT::expr ;
		ret.mLayout = address (that) ;
		return move (ret) ;
	}

	static Ref reference (RR<A> that) = delete ;

	Ref share () const {
		RefLayout ret = RefHolder::hold (thiz)->share () ;
		return move (keep[TYPE<Ref>::expr] (ret)) ;
	}

	BOOL exist () const {
		return RefHolder::hold (thiz)->exist () ;
	}

	forceinline BOOL operator== (CR<typeof (NULL)>) const {
		return (!exist ()) ;
	}

	forceinline BOOL operator!= (CR<typeof (NULL)>) const {
		return exist () ;
	}

	Unknown unknown () const {
		return RefHolder::hold (thiz)->unknown () ;
	}

	VR<A> ref_m () leftvalue {
		return RefHolder::hold (thiz)->ref ;
	}

	forceinline PTR<VR<A>> operator-> () leftvalue {
		return (&ref) ;
	}

	CR<A> ref_m () const leftvalue {
		return RefHolder::hold (thiz)->ref ;
	}

	forceinline PTR<CR<A>> operator-> () const leftvalue {
		return (&ref) ;
	}

	BOOL exclusive () const {
		return RefHolder::hold (thiz)->exclusive () ;
	}
} ;

struct HeapLayout {
	FLAG mHolder ;

public:
	implicit HeapLayout () noexcept {
		mHolder = ZERO ;
	}
} ;

struct HeapHolder implement Interface {
	imports CR<HeapLayout> expr_m () ;
	imports VFat<HeapHolder> hold (VR<HeapLayout> that) ;
	imports CFat<HeapHolder> hold (CR<HeapLayout> that) ;

	virtual void initialize () = 0 ;
	virtual INDEX stack () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual FLAG alloc (CR<LENGTH> size_) const = 0 ;
	virtual void free (CR<FLAG> layout) const = 0 ;
} ;

class Heap implement HeapLayout {
protected:
	using HeapLayout::mHolder ;

public:
	static CR<Heap> expr_m () {
		return keep[TYPE<Heap>::expr] (HeapHolder::expr) ;
	}

	INDEX stack () const {
		return HeapHolder::hold (thiz)->stack () ;
	}

	LENGTH length () const {
		return HeapHolder::hold (thiz)->length () ;
	}

	FLAG alloc (CR<LENGTH> size_) const {
		return HeapHolder::hold (thiz)->alloc (size_) ;
	}

	void free (CR<FLAG> layout) const {
		return HeapHolder::hold (thiz)->free (layout) ;
	}
} ;

struct SliceLayout {
	FLAG mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;

public:
	implicit SliceLayout () noexcept {
		mBuffer = ZERO ;
		mSize = 0 ;
		mStep = 0 ;
	}
} ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> hold (VR<SliceLayout> that) ;
	imports CFat<SliceHolder> hold (CR<SliceLayout> that) ;

	virtual void initialize (CR<FLAG> buffer ,CR<LENGTH> size_ ,CR<LENGTH> step_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual void get (CR<INDEX> index ,VR<STRU32> item) const = 0 ;
	virtual BOOL equal (CR<SliceLayout> that) const = 0 ;
	virtual FLAG compr (CR<SliceLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual SliceLayout eos () const = 0 ;
} ;

using SLICE_MAX_SIZE = ENUM<4096> ;

class Slice implement SliceLayout {
protected:
	using SliceLayout::mBuffer ;
	using SliceLayout::mSize ;
	using SliceLayout::mStep ;

public:
	implicit Slice () = default ;

	template <class ARG1 ,class = REQUIRE<IS_TEXT<ARRAY_ITEM<ARG1>>>>
	explicit Slice (CR<ARG1> that) {
		const auto r1x = address (that) ;
		const auto r2x = ENUM_DEC<ARRAY_SIZE<ARG1>>::expr ;
		const auto r3x = SIZE_OF<ARRAY_ITEM<ARG1>>::expr ;
		SliceHolder::hold (thiz)->initialize (r1x ,r2x ,r3x) ;
	}

	explicit Slice (CR<FLAG> buffer ,CR<LENGTH> size_ ,CR<LENGTH> step_) {
		SliceHolder::hold (thiz)->initialize (buffer ,size_ ,step_) ;
	}

	LENGTH size () const {
		return SliceHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return SliceHolder::hold (thiz)->step () ;
	}

	void get (CR<INDEX> index ,VR<STRU32> item) const {
		return SliceHolder::hold (thiz)->get (index ,item) ;
	}

	forceinline STRU32 operator[] (CR<INDEX> index) const {
		STRU32 ret ;
		get (index ,ret) ;
		return move (ret) ;
	}

	BOOL equal (CR<Slice> that) const {
		return SliceHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Slice> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Slice> that) const {
		return SliceHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Slice> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Slice> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Slice> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Slice> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return SliceHolder::hold (thiz)->visit (visitor) ;
	}

	Slice eos () const {
		SliceLayout ret = SliceHolder::hold (thiz)->eos () ;
		return move (keep[TYPE<Slice>::expr] (ret)) ;
	}
} ;

struct ExceptionLayout {
	Slice mWhat ;
	Slice mFunc ;
	Slice mFile ;
	Slice mLine ;
} ;

struct ExceptionHolder implement Interface {
	imports VFat<ExceptionHolder> hold (VR<ExceptionLayout> that) ;
	imports CFat<ExceptionHolder> hold (CR<ExceptionLayout> that) ;

	virtual void initialize (CR<Slice> what_ ,CR<Slice> func_ ,CR<Slice> file_ ,CR<Slice> line_) = 0 ;
	virtual Slice what () const = 0 ;
	virtual Slice func () const = 0 ;
	virtual Slice file () const = 0 ;
	virtual Slice line () const = 0 ;
	virtual void event () const = 0 ;
	virtual void raise () const = 0 ;
} ;

class Exception implement ExceptionLayout {
protected:
	using ExceptionLayout::mWhat ;
	using ExceptionLayout::mFunc ;
	using ExceptionLayout::mFile ;
	using ExceptionLayout::mLine ;

public:
	implicit Exception () = default ;

	explicit Exception (CR<Slice> what_) {
		ExceptionHolder::hold (thiz)->initialize (what_ ,Slice () ,Slice () ,Slice ()) ;
	}

	explicit Exception (CR<Slice> what_ ,CR<Slice> func_ ,CR<Slice> file_ ,CR<Slice> line_) {
		ExceptionHolder::hold (thiz)->initialize (what_ ,func_ ,file_ ,line_) ;
	}

	Slice what () const {
		return ExceptionHolder::hold (thiz)->what () ;
	}

	Slice func () const {
		return ExceptionHolder::hold (thiz)->func () ;
	}

	Slice file () const {
		return ExceptionHolder::hold (thiz)->file () ;
	}

	Slice line () const {
		return ExceptionHolder::hold (thiz)->line () ;
	}

	void event () const {
		return ExceptionHolder::hold (thiz)->event () ;
	}

	void raise () const {
		return ExceptionHolder::hold (thiz)->raise () ;
	}
} ;

struct FUNCTION_unimplemented {
	forceinline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

template <class A>
struct Like {
	A mThis ;

public:
	implicit Like () = default ;

	implicit Like (RR<A> that) :mThis (move (that)) {}

	using ITEM = typeof (nullof (A).ref) ;

	VR<ITEM> ref_m () const leftvalue {
		return const_cast<VR<ITEM>> (mThis.ref) ;
	}

	forceinline operator VR<ITEM> () const leftvalue {
		return ref ;
	}
} ;

struct ExternalLayout {
	FatLayout mImplHolder ;
} ;

template <class A ,class B>
class External implement ExternalLayout {
public:
	implicit External () = default ;

	template <class ARG1>
	explicit External (CR<ARG1> holder) {
		require (IS_EXTEND<A ,ARG1>) ;
		require (ENUM_EQUAL<SIZE_OF<ARG1> ,SIZE_OF<FatLayout>>) ;
		require (ENUM_EQUAL<ALIGN_OF<ARG1> ,ALIGN_OF<FatLayout>>) ;
		using R1X = typeof (nullof (ARG1).self) ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<B>>) ;
		require (ENUM_EQUAL<ALIGN_OF<R1X> ,ALIGN_OF<B>>) ;
		inline_memcpy (Pointer::from (root_ptr ().mImplHolder) ,Pointer::from (holder) ,SIZE_OF<FatLayout>::expr) ;
	}

	static VR<ExternalLayout> root_ptr () ;

	static CR<Fat<A ,B>> expr_m () {
		return Pointer::from (root_ptr ().mImplHolder) ;
	}
} ;

template <class A ,class B>
inline VR<ExternalLayout> External<A ,B>::root_ptr () {
	static auto mInstance = ExternalLayout () ;
	return mInstance ;
}

struct ReflectClone implement Interface {
	virtual void clone (VR<Pointer> a ,CR<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 110 ;
	}
} ;

template <class A>
class ReflectCloneBinder final implement Fat<ReflectClone ,Proxy> {
public:
	void clone (VR<Pointer> a ,CR<Pointer> b) const override {
		return inline_clone (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectEqual implement Interface {
	virtual BOOL equal (CR<Pointer> a ,CR<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 111 ;
	}
} ;

template <class A>
class ReflectEqualBinder final implement Fat<ReflectEqual ,Proxy> {
public:
	BOOL equal (CR<Pointer> a ,CR<Pointer> b) const override {
		return inline_equal (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectCompr implement Interface {
	virtual FLAG compr (CR<Pointer> a ,CR<Pointer> b) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 112 ;
	}
} ;

template <class A>
class ReflectComprBinder final implement Fat<ReflectCompr ,Proxy> {
public:
	FLAG compr (CR<Pointer> a ,CR<Pointer> b) const override {
		return inline_compr (keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}
} ;

struct ReflectVisit implement Interface {
	virtual void visit (VR<FriendVisitor> visitor ,CR<Pointer> a) const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 113 ;
	}
} ;

template <class A>
class ReflectVisitBinder final implement Fat<ReflectVisit ,Proxy> {
public:
	void visit (VR<FriendVisitor> visitor ,CR<Pointer> a) const override {
		return inline_visit (visitor ,keep[TYPE<A>::expr] (a)) ;
	}
} ;

struct ReflectGuid implement Interface {
	virtual FLAG type_guid () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 114 ;
	}
} ;

template <class A>
class ReflectGuidBinder final implement Fat<ReflectGuid ,Proxy> {
public:
	FLAG type_guid () const override {
		return memorize ([&] () {
			return inline_vptr (thiz) ;
		}) ;
	}
} ;

struct ReflectName implement Interface {
	virtual Slice type_name () const = 0 ;

	forceinline static consteval FLAG expr_m () noexcept {
		return 115 ;
	}
} ;

template <class A>
class ReflectNameBinder final implement Fat<ReflectName ,Proxy> {
public:
	Slice type_name () const override {
		const auto r1x = inline_type_name (Pointer::from (thiz) ,address (__macro_type_rtti)) ;
		return Slice (r1x ,SLICE_MAX_SIZE::expr ,1).eos () ;
	}
} ;

struct ClazzTree ;

struct ClazzLayout {
	Ref<ClazzTree> mThis ;
} ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> hold (VR<ClazzLayout> that) ;
	imports CFat<ClazzHolder> hold (CR<ClazzLayout> that) ;

	virtual void initialize (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<ClazzLayout> that) = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_guid () const = 0 ;
	virtual Slice type_name () const = 0 ;
	virtual BOOL equal (CR<ClazzLayout> that) const = 0 ;
	virtual FLAG compr (CR<ClazzLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
} ;

template <class A>
class ClazzUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectGuidBinder<A>::expr)
			return inline_vptr (ReflectGuidBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_vptr (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
} ;

class Clazz implement ClazzLayout {
protected:
	using ClazzLayout::mThis ;

public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPE<ARG1>) {
		ClazzHolder::hold (thiz)->initialize (ClazzUnknownBinder<ARG1> ()) ;
	}

	implicit Clazz (CR<Clazz> that) {
		ClazzHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Clazz> operator= (CR<Clazz> that) {
		return assign (thiz ,that) ;
	}

	implicit Clazz (RR<Clazz> that) = default ;

	forceinline VR<Clazz> operator= (RR<Clazz> that) = default ;

	LENGTH type_size () const {
		return ClazzHolder::hold (thiz)->type_size () ;
	}

	LENGTH type_align () const {
		return ClazzHolder::hold (thiz)->type_align () ;
	}

	FLAG type_guid () const {
		return ClazzHolder::hold (thiz)->type_guid () ;
	}

	Slice type_name () const {
		return ClazzHolder::hold (thiz)->type_name () ;
	}

	BOOL equal (CR<Clazz> that) const {
		return ClazzHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Clazz> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Clazz> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Clazz> that) const {
		return ClazzHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Clazz> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Clazz> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Clazz> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Clazz> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return ClazzHolder::hold (thiz)->visit (visitor) ;
	}
} ;

struct ScopeLayout {
	FLAG mLayout ;

public:
	implicit ScopeLayout () noexcept {
		mLayout = ZERO ;
	}
} ;

template <class A>
class Scope implement ScopeLayout {
protected:
	using ScopeLayout::mLayout ;

public:
	implicit Scope () = delete ;

	explicit Scope (CR<A> that) {
		const auto r1x = address (that) ;
		auto &&rax = keep[TYPE<VR<A>>::expr] (Pointer::make (r1x)) ;
		rax.enter () ;
		mLayout = r1x ;
	}

	explicit Scope (RR<A> that) = delete ;

	implicit ~Scope () noexcept {
		if (mLayout == ZERO)
			return ;
		auto &&rax = keep[TYPE<VR<A>>::expr] (Pointer::make (mLayout)) ;
		rax.leave () ;
		mLayout = ZERO ;
	}

	implicit Scope (CR<Scope> that) = delete ;

	forceinline VR<Scope> operator= (CR<Scope> that) = delete ;

	implicit Scope (RR<Scope> that) = delete ;

	forceinline VR<Scope> operator= (RR<Scope> that) = delete ;
} ;
} ;