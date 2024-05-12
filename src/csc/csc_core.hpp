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
	forceinline void operator() (CREF<ARG1> a) const noexcept {
		return ;
	}
} ;

static constexpr auto noop = FUNCTION_noop () ;

struct FUNCTION_inline_ifdo {
	forceinline BOOL operator() (CREF<BOOL> flag) const noexcept {
		return FALSE ;
	}

	forceinline BOOL operator() (VREF<BOOL> flag) const noexcept {
		flag = FALSE ;
		return FALSE ;
	}
} ;

static constexpr auto inline_ifdo = FUNCTION_inline_ifdo () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_inline_break {
	forceinline void operator() () const noexcept {
		__debugbreak () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_inline_break {
	forceinline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_inline_break {
	forceinline void operator() () const noexcept {
		__builtin_trap () ;
	}
} ;
#endif

static constexpr auto inline_break = FUNCTION_inline_break () ;

#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_inline_abort {
	forceinline __declspec (noreturn) void operator() () const noexcept {
		noop () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_inline_abort {
	forceinline void operator() () const noexcept {
		__builtin_abort () ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_inline_abort {
	forceinline void operator() () const noexcept {
		__builtin_abort () ;
	}
} ;
#endif

static constexpr auto inline_abort = FUNCTION_inline_abort () ;

struct FUNCTION_bad {
	template <class ARG1>
	forceinline ARG1 operator() (TYPEID<ARG1>) const noexcept {
		assert (FALSE) ;
	}
} ;

static constexpr auto bad = FUNCTION_bad () ;

struct FUNCTION_inline_barrier {
	template <class ARG1 ,class ARG2>
	forceinline void operator() (TYPEID<ARG1> ,CREF<ARG2> a) const noexcept {
		static auto mInstance = CPTR<void (CREF<ARG2>)> (NULL) ;
		if (mInstance == NULL)
			return ;
		mInstance (a) ;
	}
} ;

static constexpr auto inline_barrier = FUNCTION_inline_barrier () ;

class Pointer implement Proxy {
public:
	imports VREF<Pointer> make (CREF<FLAG> that) {
		return (*reinterpret_cast<VPTR<Pointer>> (that)) ;
	}

	template <class ARG1>
	imports VREF<Pointer> from (VREF<ARG1> that) {
		return reinterpret_cast<VREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports CREF<Pointer> from (CREF<ARG1> that) {
		return reinterpret_cast<CREF<Pointer>> (that) ;
	}

	template <class ARG1>
	imports RREF<Pointer> from (RREF<ARG1> that) = delete ;

	template <class ARG1>
	forceinline operator VREF<ARG1> () leftvalue {
		return reinterpret_cast<VREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator RREF<ARG1> () leftvalue = delete ;

	template <class ARG1>
	forceinline operator ARG1 () leftvalue = delete ;

	template <class ARG1>
	forceinline operator CREF<ARG1> () const leftvalue {
		return reinterpret_cast<CREF<ARG1>> (thiz) ;
	}

	template <class ARG1>
	forceinline operator RREF<ARG1> () const leftvalue = delete ;

	template <class ARG1>
	forceinline operator ARG1 () const leftvalue = delete ;

	template <class ARG1>
	forceinline operator RREF<ARG1> () rightvalue = delete ;

	template <class ARG1>
	forceinline operator ARG1 () rightvalue = delete ;
} ;

struct CoreTool implement Proxy {
	imports void inline_memset (VREF<Pointer> dst ,CREF<LENGTH> size_) noexcept ;

	imports void inline_memcpy (VREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept ;

	imports BOOL inline_memcmp (CREF<Pointer> dst ,CREF<Pointer> src ,CREF<LENGTH> size_) noexcept ;

	imports LENGTH inline_memchr (CREF<Pointer> src ,CREF<LENGTH> size_) noexcept ;
} ;

struct FUNCTION_address {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a) const noexcept {
		return FLAG (&Pointer::from (a)) ;
	}
} ;

static constexpr auto address = FUNCTION_address () ;

template <class...>
trait FUNCTION_keep_impl_HELP ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,ORDINARY>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<VREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline RREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<RREF<A>> (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,VARIABLE>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline VREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<VREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline VREF<A> operator() (CREF<ARG1> a) const noexcept = delete ;

		template <class ARG1>
		forceinline VREF<A> operator() (RREF<ARG1> a) const noexcept = delete ;
	} ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,CONSTANT>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline CREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline CREF<A> operator() (CREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline CREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<CREF<A>> (a) ;
		}
	} ;
} ;

template <class A ,class B>
trait FUNCTION_keep_impl_HELP<A ,B ,REQUIRE<IS_SAME<B ,REGISTER>>> {
	struct FUNCTION_keep_impl {
		template <class ARG1>
		forceinline RREF<A> operator() (VREF<ARG1> a) const noexcept {
			return static_cast<RREF<A>> (a) ;
		}

		template <class ARG1>
		forceinline RREF<A> operator() (CREF<ARG1> a) const noexcept = delete ;

		template <class ARG1>
		forceinline RREF<A> operator() (RREF<ARG1> a) const noexcept {
			return static_cast<RREF<A>> (a) ;
		}
	} ;
} ;

template <class A>
using FUNCTION_keep_impl = typename FUNCTION_keep_impl_HELP<REMOVE_REF<A> ,REFLECT_REF<A> ,ALWAYS>::FUNCTION_keep_impl ;

struct FUNCTION_keep {
	template <class ARG1>
	forceinline consteval FUNCTION_keep_impl<ARG1> operator[] (TYPEID<ARG1>) const noexcept {
		return FUNCTION_keep_impl<ARG1> () ;
	}
} ;

static constexpr auto keep = FUNCTION_keep () ;

struct FUNCTION_move {
	template <class ARG1>
	forceinline RREF<ARG1> operator() (VREF<ARG1> a) const noexcept {
		return static_cast<RREF<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline ARG1 operator() (CREF<ARG1> a) const {
		return static_cast<CREF<ARG1>> (a) ;
	}

	template <class ARG1>
	forceinline RREF<ARG1> operator() (RREF<ARG1> a) const noexcept {
		return static_cast<RREF<ARG1>> (a) ;
	}
} ;

static constexpr auto move = FUNCTION_move () ;

struct FUNCTION_swap {
	template <class ARG1>
	forceinline void operator() (VREF<ARG1> a ,VREF<ARG1> b) const noexcept {
		//@warn: no class should depend on its address
		auto &&tmp1 = keep[TYPE<TEMP<ARG1>>::expr] (Pointer::from (a)) ;
		auto &&tmp2 = keep[TYPE<TEMP<ARG1>>::expr] (Pointer::from (b)) ;
		const auto r1x = tmp1 ;
		tmp1 = tmp2 ;
		tmp2 = r1x ;
		barrier (tmp1) ;
		barrier (tmp2) ;
	}
} ;

static constexpr auto swap = FUNCTION_swap () ;

struct FUNCTION_assign {
	template <class ARG1 ,class ARG2>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,VREF<ARG2> b) const noexcept {
		a.~ARG1 () ;
		new (csc_new_pointer_t (&a)) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1 ,class ARG2>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,CREF<ARG2> b) const noexcept {
		a.~ARG1 () ;
		new (csc_new_pointer_t (&a)) ARG1 (move (b)) ;
		return a ;
	}

	template <class ARG1 ,class ARG2>
	forceinline VREF<ARG1> operator() (VREF<ARG1> a ,RREF<ARG2> b) const noexcept = delete ;
} ;

static constexpr auto assign = FUNCTION_assign () ;

struct FUNCTION_invoke {
	template <class ARG1>
	forceinline FUNCTION_RETURN<ARG1> operator() (RREF<ARG1> func) const {
		return func () ;
	}
} ;

static constexpr auto invoke = FUNCTION_invoke () ;

template <class A>
struct FUNCTION_bitwise_impl {
	template <class ARG1>
	forceinline A operator() (CREF<ARG1> a) const noexcept {
		require (IS_TRIVIAL<A>) ;
		using R1X = A ;
		using R2X = CONDITIONAL<IS_SAME<ARG1 ,Pointer> ,A ,ARG1> ;
		require (ENUM_EQUAL<SIZE_OF<R1X> ,SIZE_OF<R2X>>) ;
		A ret ;
		CoreTool::inline_memcpy (Pointer::from (ret) ,Pointer::from (a) ,SIZE_OF<R1X>::expr) ;
		return move (ret) ;
	}
} ;

struct FUNCTION_bitwise {
	template <class ARG1>
	forceinline consteval FUNCTION_bitwise_impl<ARG1> operator[] (TYPEID<ARG1>) const noexcept {
		return FUNCTION_bitwise_impl<ARG1> () ;
	}
} ;

static constexpr auto bitwise = FUNCTION_bitwise () ;

struct FUNCTION_inline_code {
	forceinline FLAG operator() (CREF<Interface> binder) const noexcept {
		return bitwise[TYPE<FLAG>::expr] (binder) ;
	}
} ;

static constexpr auto inline_code = FUNCTION_inline_code () ;

struct FUNCTION_inline_min {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a <= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_min = FUNCTION_inline_min () ;

struct FUNCTION_inline_max {
	forceinline VAL operator() (CREF<VAL> a ,CREF<VAL> b) const noexcept {
		if (a >= b)
			return a ;
		return b ;
	}
} ;

static constexpr auto inline_max = FUNCTION_inline_max () ;

struct FUNCTION_inline_between {
	forceinline BOOL operator() (CREF<VAL> curr ,CREF<VAL> lb ,CREF<VAL> rb) const noexcept {
		if (curr < lb)
			return FALSE ;
		if (curr >= rb)
			return FALSE ;
		return TRUE ;
	}
} ;

static constexpr auto inline_between = FUNCTION_inline_between () ;

struct FUNCTION_inline_alignas {
	forceinline VAL operator() (CREF<VAL> curr ,CREF<VAL> base) const noexcept {
		assert (base > 0) ;
		const auto r1x = curr / base * base ;
		if (r1x == curr)
			return r1x ;
		return r1x + base ;
	}
} ;

static constexpr auto inline_alignas = FUNCTION_inline_alignas () ;

struct FUNCTION_inline_equal {
	template <class ARG1>
	forceinline BOOL operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return equal_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH3)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a == b ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.equal (b) ;
	}
} ;

static constexpr auto inline_equal = FUNCTION_inline_equal () ;

struct FUNCTION_inline_compr {
	template <class ARG1>
	forceinline FLAG operator() (CREF<ARG1> a ,CREF<ARG1> b) const noexcept {
		return compr_impl (PHX ,a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		if (a < b)
			return NONE ;
		if (a > b)
			return IDEN ;
		return ZERO ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline FLAG compr_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return a.compr (b) ;
	}
} ;

static constexpr auto inline_compr = FUNCTION_inline_compr () ;

struct Visitor implement Interface {
	virtual void initialize () = 0 ;
	virtual void begin () const = 0 ;
	virtual void end () const = 0 ;
	virtual void push (CREF<BYTE> a) const = 0 ;
	virtual void push (CREF<WORD> a) const = 0 ;
	virtual void push (CREF<CHAR> a) const = 0 ;
	virtual void push (CREF<QUAD> a) const = 0 ;
	virtual FLAG peek () const = 0 ;
} ;

struct FUNCTION_inline_visit {
	template <class ARG1>
	forceinline void operator() (CREF<Visitor> visitor ,CREF<ARG1> a) const noexcept {
		return visit_impl (PHX ,visitor ,a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BASIC<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH2)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		const auto r1x = bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (a) ;
		visitor.push (r1x) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_CLASS<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH1)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		return a.visit (visitor) ;
	}
} ;

static constexpr auto inline_visit = FUNCTION_inline_visit () ;

template <class...>
trait TUPLE_HELP ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQ_ZERO<RANK_OF<PARAMS>>>> {
	struct Tuple {} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<RANK_OF<PARAMS> ,RANK1>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<RANK_OF<PARAMS> ,RANK2>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
		TYPE_M2ND_ONE<PARAMS> m2nd ;
	} ;
} ;

template <class PARAMS>
trait TUPLE_HELP<PARAMS ,REQUIRE<ENUM_EQUAL<RANK_OF<PARAMS> ,RANK3>>> {
	struct Tuple {
		TYPE_M1ST_ONE<PARAMS> m1st ;
		TYPE_M2ND_ONE<PARAMS> m2nd ;
		TYPE_M3RD_ONE<PARAMS> m3rd ;
	} ;
} ;

template <class...A>
using Tuple = typename TUPLE_HELP<TYPE<A...> ,ALWAYS>::Tuple ;

class IndexIterator {
protected:
	INDEX mBegin ;
	INDEX mEnd ;
	INDEX mPeek ;

public:
	implicit IndexIterator () = delete ;

	explicit IndexIterator (CREF<INDEX> begin_ ,CREF<INDEX> end_) {
		mBegin = begin_ ;
		mEnd = inline_max (begin_ ,end_) ;
		mPeek = mBegin ;
	}

	LENGTH size () const {
		return mEnd - mBegin ;
	}

	IndexIterator begin () const {
		return thiz ;
	}

	IndexIterator end () const {
		return thiz ;
	}

	forceinline BOOL operator== (CREF<IndexIterator>) const {
		return mPeek == mEnd ;
	}

	forceinline BOOL operator!= (CREF<IndexIterator>) const {
		return mPeek != mEnd ;
	}

	forceinline CREF<INDEX> operator* () const leftvalue {
		return mPeek ;
	}

	forceinline void operator++ () {
		mPeek++ ;
	}
} ;

struct PIXEL {
	INDEX mX ;
	INDEX mY ;
} ;

class PixelIterator {
protected:
	PIXEL mBegin ;
	PIXEL mEnd ;
	PIXEL mPeek ;

public:
	implicit PixelIterator () = delete ;

	explicit PixelIterator (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) {
		mBegin.mX = begin_x ;
		mBegin.mY = begin_y ;
		mEnd.mX = inline_max (begin_x ,end_x) ;
		mEnd.mY = inline_max (begin_y ,end_y) ;
		mPeek = mBegin ;
		if (size () > 0)
			return ;
		mPeek = mEnd ;
	}

	LENGTH size () const {
		return (mEnd.mX - mBegin.mX) * (mEnd.mY - mBegin.mY) ;
	}

	PixelIterator begin () const {
		return thiz ;
	}

	PixelIterator end () const {
		return thiz ;
	}

	forceinline BOOL operator== (CREF<PixelIterator>) const {
		return mPeek.mY == mEnd.mY ;
	}

	forceinline BOOL operator!= (CREF<PixelIterator>) const {
		return mPeek.mY != mEnd.mY ;
	}

	forceinline CREF<PIXEL> operator* () const leftvalue {
		return mPeek ;
	}

	forceinline void operator++ () {
		mPeek.mX++ ;
		if (mPeek.mX < mEnd.mX)
			return ;
		mPeek.mX = 0 ;
		mPeek.mY++ ;
	}
} ;

struct FUNCTION_iter {
	forceinline IndexIterator operator() (CREF<INDEX> begin_ ,CREF<INDEX> end_) const noexcept {
		return IndexIterator (begin_ ,end_) ;
	}

	forceinline PixelIterator operator() (CREF<INDEX> begin_x ,CREF<INDEX> end_x ,CREF<INDEX> begin_y ,CREF<INDEX> end_y) const noexcept {
		return PixelIterator (begin_x ,end_x ,begin_y ,end_y) ;
	}
} ;

static constexpr auto iter = FUNCTION_iter () ;

template <class A ,class B>
class Fat implement A {
protected:
	FLAG mPointer ;

public:
	VREF<B> fake_m () leftvalue {
		return Pointer::make (mPointer) ;
	}

	CREF<B> fake_m () const leftvalue {
		return Pointer::make (mPointer) ;
	}
} ;

template <class A>
class VFat {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit VFat () = delete ;

	explicit VFat (CREF<Interface> holder ,VREF<Pointer> pointer) {
		mHolder = inline_code (holder) ;
		mPointer = address (pointer) ;
	}

	forceinline VPTR<A> operator-> () const {
		return VPTR<A> (address (thiz)) ;
	}
} ;

template <class A>
class CFat {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit CFat () = delete ;

	explicit CFat (CREF<Interface> holder ,CREF<Pointer> pointer) {
		mHolder = inline_code (holder) ;
		mPointer = address (pointer) ;
	}

	forceinline CPTR<A> operator-> () const {
		return CPTR<A> (address (thiz)) ;
	}
} ;

template <class A>
class RFat {
private:
	require (IS_INTERFACE<A>) ;

protected:
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RFat () = delete ;

	explicit RFat (CREF<Unknown> holder) {
		mHolder = holder.reflect (A::expr) ;
		mPointer = ZERO ;
	}

	forceinline CPTR<A> operator-> () const {
		return CPTR<A> (address (thiz)) ;
	}
} ;

struct ReflectSize implement Interface {
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 100 ;
	}
} ;

template <class A>
class ReflectSizeBinder implement ReflectSize {
public:
	LENGTH type_size () const override {
		return SIZE_OF<A>::expr ;
	}

	LENGTH type_align () const override {
		return ALIGN_OF<A>::expr ;
	}
} ;

struct ReflectCreate implement Interface {
	virtual void create (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 101 ;
	}
} ;

template <class A>
class ReflectCreateBinder implement ReflectCreate {
public:
	void create (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept override {
		require (IS_DEFAULT<A>) ;
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&tmp = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : iter (0 ,size_)) {
			new (csc_new_pointer_t (&tmp[i])) A () ;
		}
	}
} ;

struct ReflectDestroy implement Interface {
	virtual void destroy (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 102 ;
	}
} ;

template <class A>
class ReflectDestroyBinder implement ReflectDestroy {
public:
	void destroy (VREF<Pointer> a ,CREF<LENGTH> size_) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&tmp = keep[TYPE<ARR<A>>::expr] (a) ;
		for (auto &&i : iter (0 ,size_)) {
			tmp[i].~A () ;
		}
	}
} ;

struct ReflectMove implement Interface {
	virtual void init (VREF<Pointer> a) const noexcept = 0 ;
	virtual void copy (VREF<Pointer> a ,CREF<Pointer> b) const noexcept = 0 ;
	virtual void drop (VREF<Pointer> a) const noexcept = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 103 ;
	}
} ;

template <class A>
class ReflectMoveBinder implement ReflectMove {
public:
	void init (VREF<Pointer> a) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		CoreTool::inline_memset (a ,SIZE_OF<A>::expr) ;
	}

	void copy (VREF<Pointer> a ,CREF<Pointer> b) const noexcept override {
		CoreTool::inline_memcpy (a ,b ,SIZE_OF<A>::expr) ;
	}

	void drop (VREF<Pointer> a) const noexcept override {
		if (IS_TRIVIAL<A>::expr)
			return ;
		auto &&tmp = keep[TYPE<A>::expr] (a) ;
		tmp.~A () ;
	}
} ;

struct ReflectPair implement Interface {
	virtual LENGTH offset () const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 104 ;
	}
} ;

template <class A ,class B>
struct PAIR {
	A mItem ;
	B mNode ;
} ;

template <class A>
class ReflectPairBinder implement ReflectPair {
public:
	LENGTH offset () const override {
		return address (CPTR<A> (NULL)->mNode) ;
	}
} ;

template <class A>
class Pin {
protected:
	mutable TEMP<A> mStorage ;

public:
	implicit Pin () noexcept {
		new (csc_new_pointer_t (&mStorage)) A () ;
	}

	implicit ~Pin () noexcept {
		self.~A () ;
	}

	implicit Pin (CREF<Pin> that) :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (CREF<Pin> that) {
		return thiz ;
	}

	implicit Pin (RREF<Pin> that) noexcept :Pin () {
		noop () ;
	}

	forceinline VREF<Pin> operator= (RREF<Pin> that) noexcept {
		return thiz ;
	}

	VREF<A> self_m () const leftvalue {
		return Pointer::from (mStorage) ;
	}

	forceinline operator VREF<A> () const leftvalue {
		return self ;
	}

	forceinline VPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}
} ;

struct BoxLayout {
	FLAG mHolder ;

public:
	implicit BoxLayout () noexcept {
		mHolder = ZERO ;
	}

	implicit ~BoxLayout () noexcept ;

	implicit BoxLayout (CREF<BoxLayout> that) = delete ;

	forceinline VREF<BoxLayout> operator= (CREF<BoxLayout> that) = delete ;

	implicit BoxLayout (RREF<BoxLayout> that) noexcept :BoxLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<BoxLayout> operator= (RREF<BoxLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct BoxHolder implement Interface {
	imports VFat<BoxHolder> create (VREF<BoxLayout> that) ;
	imports CFat<BoxHolder> create (CREF<BoxLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void destroy () = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual CREF<Unknown> unknown () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
	virtual void acquire (CREF<BoxLayout> that) = 0 ;
	virtual void release () = 0 ;
} ;

inline BoxLayout::~BoxLayout () noexcept {
	BoxHolder::create (thiz)->destroy () ;
}

template <class A>
struct BoxUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Box implement BoxLayout {
protected:
	using BoxLayout::mHolder ;
	TEMP<A> mStorage ;

public:
	implicit Box () = default ;

	implicit Box (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class...ARG1>
	imports Box make (XREF<ARG1>...initval) {
		Box ret ;
		BoxHolder::create (ret)->initialize (BoxUnknownBinder<A> ()) ;
		new (csc_new_pointer_t (&ret.mStorage)) A (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		return move (ret) ;
	}

	BOOL exist () const {
		return BoxHolder::create (thiz)->exist () ;
	}

	CREF<Unknown> unknown () const leftvalue {
		return BoxHolder::create (thiz)->unknown () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	void acquire (CREF<BoxLayout> that) {
		return BoxHolder::create (thiz)->acquire (that) ;
	}

	void release () {
		return BoxHolder::create (thiz)->release () ;
	}
} ;

template <class A>
class BoxRef implement BoxLayout {
protected:
	using BoxLayout::mHolder ;

public:
	implicit BoxRef () = default ;

	implicit BoxRef (CREF<typeof (NULL)>) {
		noop () ;
	}

	implicit BoxRef (RREF<Box<A>> that) noexcept {
		BoxHolder::create (thiz)->acquire (that) ;
		BoxHolder::create (that)->release () ;
	}

	forceinline VREF<BoxRef> operator= (RREF<Box<A>> that) noexcept {
		return assign (thiz ,that) ;
	}

	BOOL exist () const {
		return BoxHolder::create (thiz)->exist () ;
	}

	CREF<Unknown> unknown () const leftvalue {
		return BoxHolder::create (thiz)->unknown () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return BoxHolder::create (thiz)->self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}

	void acquire (CREF<BoxLayout> that) {
		return BoxHolder::create (thiz)->acquire (that) ;
	}

	void release () {
		return BoxHolder::create (thiz)->release () ;
	}
} ;

struct FUNCTION_memorize {
	template <class ARG1>
	forceinline CREF<FUNCTION_RETURN<ARG1>> operator() (CREF<ARG1> func) const {
		static const auto mInstance = func () ;
		return mInstance ;
	}
} ;

static constexpr auto memorize = FUNCTION_memorize () ;

struct RefLayout {
	FLAG mHolder ;
	FLAG mPointer ;

public:
	implicit RefLayout () noexcept {
		mHolder = ZERO ;
		mPointer = ZERO ;
	}

	implicit ~RefLayout () noexcept ;

	implicit RefLayout (CREF<RefLayout> that) = delete ;

	forceinline VREF<RefLayout> operator= (CREF<RefLayout> that) = delete ;

	implicit RefLayout (RREF<RefLayout> that) noexcept :RefLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<RefLayout> operator= (RREF<RefLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct RefHolder implement Interface {
	imports VFat<RefHolder> create (VREF<RefLayout> that) ;
	imports CFat<RefHolder> create (CREF<RefLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<Unknown> element ,CREF<LENGTH> size_) = 0 ;
	virtual void destroy () = 0 ;
	virtual RefLayout share () const = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual CREF<Unknown> unknown () const leftvalue = 0 ;
	virtual VREF<Pointer> self_m () leftvalue = 0 ;
	virtual CREF<Pointer> self_m () const leftvalue = 0 ;
} ;

inline RefLayout::~RefLayout () noexcept {
	RefHolder::create (thiz)->destroy () ;
}

template <class A>
struct RefUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_code (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Ref implement RefLayout {
protected:
	using RefLayout::mHolder ;
	using RefLayout::mPointer ;

public:
	implicit Ref () = default ;

	implicit Ref (CREF<typeof (NULL)>) {
		noop () ;
	}

	template <class...ARG1>
	imports Ref make (XREF<ARG1>...initval) {
		Ref ret ;
		auto rax = Box<A>::make (keep[TYPE<XREF<ARG1>>::expr] (initval)...) ;
		RefHolder::create (ret)->initialize (move (rax)) ;
		return move (ret) ;
	}

	imports Ref reference (VREF<A> that) {
		Ref ret ;
		ret.mPointer = address (that) ;
		return move (ret) ;
	}

	Ref share () const {
		RefLayout ret = RefHolder::create (thiz)->share () ;
		return move (keep[TYPE<Ref>::expr] (ret)) ;
	}

	BOOL exist () const {
		return RefHolder::create (thiz)->exist () ;
	}

	CREF<Unknown> unknown () const leftvalue {
		return RefHolder::create (thiz)->unknown () ;
	}

	forceinline BOOL operator== (CREF<typeof (NULL)>) const {
		return ifnot (exist ()) ;
	}

	forceinline BOOL operator!= (CREF<typeof (NULL)>) const {
		return exist () ;
	}

	VREF<A> self_m () leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	forceinline VPTR<A> operator-> () leftvalue {
		return (&self) ;
	}

	CREF<A> self_m () const leftvalue {
		return RefHolder::create (thiz)->self ;
	}

	forceinline CPTR<A> operator-> () const leftvalue {
		return (&self) ;
	}
} ;

struct HeapToolPureLayout ;

struct HeapToolLayout {
	Ref<HeapToolPureLayout> mThis ;
} ;

struct HeapToolHolder implement Interface {
	imports VFat<HeapToolHolder> create (VREF<HeapToolLayout> that) ;
	imports CFat<HeapToolHolder> create (CREF<HeapToolLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void stack (CREF<FLAG> buffer ,CREF<LENGTH> size_) const = 0 ;
	virtual FLAG alloc (CREF<LENGTH> size_) const = 0 ;
	virtual void free (CREF<FLAG> addr) const = 0 ;
} ;

class HeapTool implement HeapToolLayout {
protected:
	using HeapToolLayout::mThis ;

public:
	imports CREF<HeapTool> instance () {
		return memorize ([&] () {
			HeapTool ret ;
			HeapToolHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports LENGTH length () {
		return HeapToolHolder::create (instance ())->length () ;
	}

	imports void stack (CREF<FLAG> buffer ,CREF<LENGTH> size_) {
		return HeapToolHolder::create (instance ())->stack (buffer ,size_) ;
	}

	imports FLAG alloc (CREF<LENGTH> size_) {
		return HeapToolHolder::create (instance ())->alloc (size_) ;
	}

	imports void free (CREF<FLAG> addr) {
		return HeapToolHolder::create (instance ())->free (addr) ;
	}
} ;
struct CaptureLayout {
	LENGTH mRank ;
} ;

template <class...PARAMS>
class Capture implement CaptureLayout {
private:
	using RANK = RANK_OF<TYPE<PARAMS...>> ;
	using CAPTURE = Tuple<ARR<FLAG ,ENUM_MAX<RANK ,RANK1>>> ;

protected:
	using CaptureLayout::mRank ;
	CAPTURE mStorage ;

public:
	explicit Capture (CREF<REMOVE_REF<PARAMS>>...params) {
		mRank = RANK::expr ;
		mStorage = CAPTURE {address (params)...} ;
	}

	imports CREF<Capture> from (CREF<CaptureLayout> that) {
		assert (that.mRank == RANK::expr) ;
		return Pointer::from (that) ;
	}

	imports CREF<Capture> from (RREF<CaptureLayout> that) = delete ;

	template <class ARG1>
	void invoke (CREF<ARG1> func) const {
		using R1X = TYPE_SENQUENCE<RANK> ;
		return invoke_impl (func ,TYPE<R1X>::expr) ;
	}

	template <class ARG1 ,class...ARG2>
	void invoke_impl (CREF<ARG1> func ,TYPEID<TYPE<ARG2...>>) const {
		return func (keep[TYPE<XREF<PARAMS>>::expr] (Pointer::make (mStorage.m1st[ARG2::expr]))...) ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> func) const {
		return invoke (func) ;
	}
} ;

template <class A>
class CaptureIterator {
protected:
	FLAG mBegin ;
	FLAG mEnd ;
	FLAG mPeek ;

public:
	implicit CaptureIterator () = delete ;

	explicit CaptureIterator (CREF<FLAG> begin_ ,CREF<FLAG> end_) {
		assert (inline_alignas (begin_ ,SIZE_OF<FLAG>::expr) == begin_) ;
		assert (inline_alignas (end_ ,SIZE_OF<FLAG>::expr) == end_) ;
		mBegin = begin_ ;
		mEnd = inline_max (begin_ ,end_) ;
		mPeek = mBegin ;
	}

	LENGTH size () const {
		return (mEnd - mBegin) / SIZE_OF<FLAG>::expr ;
	}

	CaptureIterator begin () const {
		return thiz ;
	}

	CaptureIterator end () const {
		return thiz ;
	}

	forceinline BOOL operator== (CREF<CaptureIterator>) const {
		return mPeek == mEnd ;
	}

	forceinline BOOL operator!= (CREF<CaptureIterator>) const {
		return mPeek != mEnd ;
	}

	forceinline CREF<A> operator* () const leftvalue {
		const auto r1x = bitwise[TYPE<FLAG>::expr] (Pointer::make (mPeek)) ;
		return Pointer::make (r1x) ;
	}

	forceinline void operator++ () {
		mPeek += SIZE_OF<FLAG>::expr ;
	}
} ;

template <class...>
trait CAPTURE_BIND_HELP ;

template <class...A>
trait CAPTURE_BIND_HELP<TYPE<A...> ,ALWAYS> {
	using RET = Capture<A...> ;
} ;

template <class A>
using CaptureBind = typename CAPTURE_BIND_HELP<A ,ALWAYS>::RET ;

struct SliceLayout {
	FLAG mBuffer ;
	LENGTH mSize ;
	LENGTH mStep ;
} ;

struct SliceHolder implement Interface {
	imports VFat<SliceHolder> create (VREF<SliceLayout> that) ;
	imports CFat<SliceHolder> create (CREF<SliceLayout> that) ;

	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual void get (CREF<INDEX> index ,VREF<STRU32> item) const = 0 ;
	virtual BOOL equal (CREF<SliceLayout> that) const = 0 ;
	virtual FLAG compr (CREF<SliceLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
class Slice implement SliceLayout {
protected:
	using SliceLayout::mBuffer ;
	using SliceLayout::mSize ;
	using SliceLayout::mStep ;

public:
	implicit Slice () = default ;

	template <class ARG1 ,class = REQUIRE<IS_TEXT<ARRAY_ITEM<ARG1>>>>
	explicit Slice (CREF<ARG1> that) {
		mBuffer = address (that) ;
		mSize = ENUM_DEC<ARRAY_SIZE<ARG1>>::expr ;
		mStep = ALIGN_OF<ARRAY_ITEM<ARG1>>::expr ;
	}

	explicit Slice (CREF<FLAG> buffer ,CREF<LENGTH> size_ ,CREF<LENGTH> step_) {
		mBuffer = buffer ;
		mSize = size_ ;
		mStep = step_ ;
	}

	LENGTH size () const {
		return SliceHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return SliceHolder::create (thiz)->step () ;
	}

	A at (CREF<INDEX> index) const {
		auto rax = STRU32 () ;
		SliceHolder::create (thiz)->get (index ,rax) ;
		return A (rax) ;
	}

	forceinline A operator[] (CREF<INDEX> index) const {
		return at (index) ;
	}

	BOOL equal (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Slice> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Slice> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Slice> that) const {
		return SliceHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Slice> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Slice> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Slice> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Slice> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return SliceHolder::create (thiz)->visit (visitor) ;
	}
} ;

class Exception {
protected:
	Slice<STR> mWhat ;
	Slice<STR> mFunc ;
	Slice<STR> mFile ;
	Slice<STR> mLine ;

public:
	implicit Exception () = default ;

	explicit Exception (CREF<Slice<STR>> what_ ,CREF<Slice<STR>> func_ ,CREF<Slice<STR>> file_ ,CREF<Slice<STR>> line_) {
		mWhat = what_ ;
		mFunc = func_ ;
		mFile = file_ ;
		mLine = line_ ;
	}

	Slice<STR> what () const {
		return mWhat ;
	}

	Slice<STR> func () const {
		return mFunc ;
	}

	Slice<STR> file () const {
		return mFile ;
	}

	Slice<STR> line () const {
		return mLine ;
	}

	void raise () {
		throw thiz ;
	}
} ;

struct FUNCTION_unimplemented {
	forceinline void operator() () const {
		assume (FALSE) ;
	}
} ;

static constexpr auto unimplemented = FUNCTION_unimplemented () ;

struct ReflectClone implement Interface {
	virtual void clone (VREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 110 ;
	}
} ;

template <class A>
class ReflectCloneBinder implement ReflectClone {
public:
	void clone (VREF<Pointer> a ,CREF<Pointer> b) const override {
		return clone_impl (PHX ,keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_CLONE<ARG1>>>
	forceinline void clone_impl (CREF<typeof (PH2)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		assign (a ,b) ;
	}

	template <class ARG1>
	forceinline void clone_impl (CREF<typeof (PH1)> ,VREF<ARG1> a ,CREF<ARG1> b) const {
		assume (FALSE) ;
	}
} ;

struct ReflectEqual implement Interface {
	virtual BOOL equal (CREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 111 ;
	}
} ;

template <class A>
class ReflectEqualBinder implement ReflectEqual {
public:
	BOOL equal (CREF<Pointer> a ,CREF<Pointer> b) const override {
		return equal_impl (PHX ,keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_EQUAL<ARG1>>>
	forceinline BOOL equal_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return inline_equal (a ,b) ;
	}

	template <class ARG1>
	forceinline BOOL equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		assume (FALSE) ;
		return BOOL () ;
	}
} ;

struct ReflectCompr implement Interface {
	virtual FLAG compr (CREF<Pointer> a ,CREF<Pointer> b) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 112 ;
	}
} ;

template <class A>
class ReflectComprBinder implement ReflectCompr {
public:
	FLAG compr (CREF<Pointer> a ,CREF<Pointer> b) const override {
		return equal_impl (PHX ,keep[TYPE<A>::expr] (a) ,keep[TYPE<A>::expr] (b)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_COMPR<ARG1>>>
	forceinline FLAG equal_impl (CREF<typeof (PH2)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		return inline_compr (a ,b) ;
	}

	template <class ARG1>
	forceinline FLAG equal_impl (CREF<typeof (PH1)> ,CREF<ARG1> a ,CREF<ARG1> b) const {
		assume (FALSE) ;
		return FLAG () ;
	}
} ;

struct ReflectVisit implement Interface {
	virtual void visit (CREF<Visitor> visitor ,CREF<Pointer> a) const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 113 ;
	}
} ;

template <class A>
class ReflectVisitBinder implement ReflectVisit {
public:
	void visit (CREF<Visitor> visitor ,CREF<Pointer> a) const override {
		return visit_impl (PHX ,visitor ,keep[TYPE<A>::expr] (a)) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_VISIT<ARG1>>>
	forceinline void visit_impl (CREF<typeof (PH2)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		return inline_visit (visitor ,a) ;
	}

	template <class ARG1>
	forceinline void visit_impl (CREF<typeof (PH1)> ,CREF<Visitor> visitor ,CREF<ARG1> a) const {
		assume (FALSE) ;
	}
} ;

struct ReflectCode implement Interface {
	virtual FLAG type_code () const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 114 ;
	}
} ;

template <class A>
class ReflectCodeBinder implement ReflectCode {
public:
	FLAG type_code () const override {
		return inline_code (thiz) ;
	}
} ;

struct ReflectName implement Interface {
	virtual Slice<STR> type_name () const = 0 ;

	imports forceinline consteval csc_diff_t expr_m () noexcept {
		return 115 ;
	}
} ;

#ifdef __CSC_CXX_RTTI__
template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice<STR> type_name () const override {
		const auto r1x = FLAG (typeid (A).name ()) ;
		const auto r2x = CoreTool::inline_memchr (Pointer::make (r1x) ,4096) ;
		return Slice<STR> (r1x ,r2x ,1) ;
	}
} ;
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_MSVC__
template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice<STR> type_name () const override {
		return Slice<STR> (__FUNCSIG__) ;
	}
} ;
#endif
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_GNUC__
template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice<STR> type_name () const override {
		return Slice<STR> (__PRETTY_FUNCTION__) ;
	}
} ;
#endif
#endif

#ifndef __CSC_CXX_RTTI__
#ifdef __CSC_COMPILER_CLANG__
template <class A>
class ReflectNameBinder implement ReflectName {
public:
	Slice<STR> type_name () const override {
		return Slice<STR> (__PRETTY_FUNCTION__) ;
	}
} ;
#endif
#endif

struct ClazzPureLayout ;

struct ClazzLayout {
	Ref<ClazzPureLayout> mThis ;
} ;

struct ClazzHolder implement Interface {
	imports VFat<ClazzHolder> create (VREF<ClazzLayout> that) ;
	imports CFat<ClazzHolder> create (CREF<ClazzLayout> that) ;

	virtual void initialize (CREF<Unknown> holder) = 0 ;
	virtual void initialize (CREF<ClazzLayout> that) = 0 ;
	virtual LENGTH type_size () const = 0 ;
	virtual LENGTH type_align () const = 0 ;
	virtual FLAG type_code () const = 0 ;
	virtual Slice<STR> type_name () const = 0 ;
	virtual BOOL equal (CREF<ClazzLayout> that) const = 0 ;
	virtual FLAG compr (CREF<ClazzLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
} ;

template <class A>
struct ClazzUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCodeBinder<A>::expr)
			return inline_code (ReflectCodeBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_code (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
} ;

class Clazz implement ClazzLayout {
protected:
	using ClazzLayout::mThis ;

public:
	implicit Clazz () = default ;

	template <class ARG1>
	explicit Clazz (TYPEID<ARG1>) {
		ClazzHolder::create (thiz)->initialize (ClazzUnknownBinder<ARG1> ()) ;
	}

	implicit Clazz (CREF<Clazz> that) {
		ClazzHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Clazz> operator= (CREF<Clazz> that) {
		return assign (thiz ,that) ;
	}

	implicit Clazz (RREF<Clazz> that) = default ;

	forceinline VREF<Clazz> operator= (RREF<Clazz> that) = default ;

	LENGTH type_size () const {
		return ClazzHolder::create (thiz)->type_size () ;
	}

	LENGTH type_align () const {
		return ClazzHolder::create (thiz)->type_align () ;
	}

	FLAG type_code () const {
		return ClazzHolder::create (thiz)->type_code () ;
	}

	Slice<STR> type_name () const {
		return ClazzHolder::create (thiz)->type_name () ;
	}

	BOOL equal (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Clazz> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Clazz> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Clazz> that) const {
		return ClazzHolder::create (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CREF<Clazz> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CREF<Clazz> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CREF<Clazz> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CREF<Clazz> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return ClazzHolder::create (thiz)->visit (visitor) ;
	}
} ;

struct AutoLayout {
	BoxLayout mValue ;
} ;

struct AutoHolder implement Interface {
	imports VFat<AutoHolder> create (VREF<AutoLayout> that) ;
	imports CFat<AutoHolder> create (CREF<AutoLayout> that) ;

	virtual void initialize (RREF<BoxLayout> item) = 0 ;
	virtual void poll (CREF<Unknown> holder ,VREF<BoxLayout> item) = 0 ;
} ;

class Auto implement AutoLayout {
private:
	using FAKE_SIZE = ENUM<1024> ;
	using FAKE_ALIGN = RANK8 ;

protected:
	using AutoLayout::mValue ;
	TEMP<Storage<FAKE_SIZE ,FAKE_ALIGN>> mStorage ;

public:
	implicit Auto () = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_SAME<ARG1 ,Auto>>>>
	implicit Auto (RREF<ARG1> that) {
		require (ENUM_COMPR_LTEQ<SIZE_OF<ARG1> ,FAKE_SIZE>) ;
		require (ENUM_COMPR_LTEQ<ALIGN_OF<ARG1> ,FAKE_ALIGN>) ;
		auto rax = Box<ARG1>::make (move (that)) ;
		AutoHolder::create (thiz)->initialize (move (rax)) ;
	}

	template <class ARG1>
	ARG1 poll (TYPEID<ARG1>) {
		auto rax = Box<ARG1> () ;
		AutoHolder::create (thiz)->poll (BoxUnknownBinder<ARG1> () ,rax) ;
		return move (rax.self) ;
	}

	template <class ARG1>
	forceinline operator ARG1 () rightvalue {
		return poll (TYPE<ARG1>::expr) ;
	}
} ;

template <class A>
class Scope {
protected:
	CPTR<A> mThat ;
	BOOL mOwner ;

public:
	implicit Scope () = delete ;

	explicit Scope (CREF<A> that) : mThat ((&that)) {
		mThat->enter () ;
		mOwner = TRUE ;
	}

	implicit ~Scope () noexcept {
		if ifnot (mOwner)
			return ;
		mThat->leave () ;
		mOwner = FALSE ;
	}

	implicit Scope (CREF<Scope> that) = delete ;

	forceinline VREF<Scope> operator= (CREF<Scope> that) = delete ;

	implicit Scope (RREF<Scope> that) noexcept :Scope () {
		swap (thiz ,that) ;
	}

	forceinline VREF<Scope> operator= (RREF<Scope> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;
} ;