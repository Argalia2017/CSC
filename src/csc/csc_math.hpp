#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
static constexpr auto MATH_E = FLT64 (2.71828182845904523536) ;
static constexpr auto MATH_PI = FLT64 (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = FLT64 (1.41421356237309504880) ;
static constexpr auto MATH_PDF0 = FLT64 (0.39894228040143267794) ;
static constexpr auto MATH_R = MATH_PI / FLT64 (180) ;
static constexpr auto MATH_LN2 = FLT64 (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = FLT64 (2.30258509299404568402) ;

struct MathProcLayout ;

struct MathProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<MathProcLayout>>> expr_m () ;
	imports VFat<MathProcHolder> hold (VR<MathProcLayout> that) ;
	imports CFat<MathProcHolder> hold (CR<MathProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL is_inf (CR<FLT32> a) const = 0 ;
	virtual BOOL is_inf (CR<FLT64> a) const = 0 ;
	virtual BOOL is_low (CR<FLT32> a) const = 0 ;
	virtual BOOL is_low (CR<FLT64> a) const = 0 ;
	virtual VAL32 step (CR<VAL32> a) const = 0 ;
	virtual VAL64 step (CR<VAL64> a) const = 0 ;
	virtual FLT32 step (CR<FLT32> a) const = 0 ;
	virtual FLT64 step (CR<FLT64> a) const = 0 ;
	virtual FLT64 sign (CR<BOOL> a) const = 0 ;
	virtual VAL32 square (CR<VAL32> a) const = 0 ;
	virtual VAL64 square (CR<VAL64> a) const = 0 ;
	virtual FLT32 square (CR<FLT32> a) const = 0 ;
	virtual FLT64 square (CR<FLT64> a) const = 0 ;
	virtual FLT32 sqrt (CR<FLT32> a) const = 0 ;
	virtual FLT64 sqrt (CR<FLT64> a) const = 0 ;
	virtual VAL32 cubic (CR<VAL32> a) const = 0 ;
	virtual VAL64 cubic (CR<VAL64> a) const = 0 ;
	virtual FLT32 cubic (CR<FLT32> a) const = 0 ;
	virtual FLT64 cubic (CR<FLT64> a) const = 0 ;
	virtual FLT32 cbrt (CR<FLT32> a) const = 0 ;
	virtual FLT64 cbrt (CR<FLT64> a) const = 0 ;
	virtual FLT32 pow (CR<FLT32> a ,CR<VAL32> b) const = 0 ;
	virtual FLT64 pow (CR<FLT64> a ,CR<VAL32> b) const = 0 ;
	virtual FLT32 hypot (CR<FLT32> a ,CR<FLT32> b) const = 0 ;
	virtual FLT64 hypot (CR<FLT64> a ,CR<FLT64> b) const = 0 ;
	virtual VAL32 abs (CR<VAL32> a) const = 0 ;
	virtual VAL64 abs (CR<VAL64> a) const = 0 ;
	virtual FLT32 abs (CR<FLT32> a) const = 0 ;
	virtual FLT64 abs (CR<FLT64> a) const = 0 ;
	virtual FLT32 inverse (CR<FLT32> a) const = 0 ;
	virtual FLT64 inverse (CR<FLT64> a) const = 0 ;
	virtual FLT32 floor (CR<FLT32> a ,CR<FLT32> b) const = 0 ;
	virtual FLT64 floor (CR<FLT64> a ,CR<FLT64> b) const = 0 ;
	virtual FLT32 ceil (CR<FLT32> a ,CR<FLT32> b) const = 0 ;
	virtual FLT64 ceil (CR<FLT64> a ,CR<FLT64> b) const = 0 ;
	virtual FLT32 round (CR<FLT32> a) const = 0 ;
	virtual FLT64 round (CR<FLT64> a) const = 0 ;
	virtual FLT32 fmod (CR<FLT32> a) const = 0 ;
	virtual FLT64 fmod (CR<FLT64> a) const = 0 ;
	virtual VAL32 clamp (CR<VAL32> a ,CR<VAL32> min_ ,CR<VAL32> max_) const = 0 ;
	virtual VAL64 clamp (CR<VAL64> a ,CR<VAL64> min_ ,CR<VAL64> max_) const = 0 ;
	virtual FLT32 clamp (CR<FLT32> a ,CR<FLT32> min_ ,CR<FLT32> max_) const = 0 ;
	virtual FLT64 clamp (CR<FLT64> a ,CR<FLT64> min_ ,CR<FLT64> max_) const = 0 ;
	virtual VAL32 lerp (CR<FLT64> a ,CR<VAL32> min_ ,CR<VAL32> max_) const = 0 ;
	virtual VAL64 lerp (CR<FLT64> a ,CR<VAL64> min_ ,CR<VAL64> max_) const = 0 ;
	virtual FLT32 cos (CR<FLT32> a) const = 0 ;
	virtual FLT64 cos (CR<FLT64> a) const = 0 ;
	virtual FLT32 sin (CR<FLT32> a) const = 0 ;
	virtual FLT64 sin (CR<FLT64> a) const = 0 ;
	virtual FLT32 tan (CR<FLT32> a) const = 0 ;
	virtual FLT64 tan (CR<FLT64> a) const = 0 ;
	virtual FLT32 acos (CR<FLT32> a) const = 0 ;
	virtual FLT64 acos (CR<FLT64> a) const = 0 ;
	virtual FLT32 asin (CR<FLT32> a) const = 0 ;
	virtual FLT64 asin (CR<FLT64> a) const = 0 ;
	virtual FLT32 atan (CR<FLT32> y ,CR<FLT32> x) const = 0 ;
	virtual FLT64 atan (CR<FLT64> y ,CR<FLT64> x) const = 0 ;
	virtual FLT32 exp (CR<FLT32> a) const = 0 ;
	virtual FLT64 exp (CR<FLT64> a) const = 0 ;
	virtual FLT32 log (CR<FLT32> a) const = 0 ;
	virtual FLT64 log (CR<FLT64> a) const = 0 ;
	virtual FLT32 pdf (CR<FLT32> a) const = 0 ;
	virtual FLT64 pdf (CR<FLT64> a) const = 0 ;
	virtual FLT32 cbf (CR<FLT32> a) const = 0 ;
	virtual FLT64 cbf (CR<FLT64> a) const = 0 ;
	virtual BOOL all_of (CR<BOOL> a ,CR<WrapperLayout> b) const = 0 ;
	virtual BOOL any_of (CR<BOOL> a ,CR<WrapperLayout> b) const = 0 ;
	virtual VAL32 max_of (CR<VAL32> a ,CR<WrapperLayout> b) const = 0 ;
	virtual VAL64 max_of (CR<VAL64> a ,CR<WrapperLayout> b) const = 0 ;
	virtual FLT32 max_of (CR<FLT32> a ,CR<WrapperLayout> b) const = 0 ;
	virtual FLT64 max_of (CR<FLT64> a ,CR<WrapperLayout> b) const = 0 ;
	virtual VAL32 min_of (CR<VAL32> a ,CR<WrapperLayout> b) const = 0 ;
	virtual VAL64 min_of (CR<VAL64> a ,CR<WrapperLayout> b) const = 0 ;
	virtual FLT32 min_of (CR<FLT32> a ,CR<WrapperLayout> b) const = 0 ;
	virtual FLT64 min_of (CR<FLT64> a ,CR<WrapperLayout> b) const = 0 ;
} ;

class MathProc implement OfThis<UniqueRef<MathProcLayout>> {
public:
	static CR<MathProc> expr_m () {
		return keep[TYPE<MathProc>::expr] (MathProcHolder::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static BOOL is_inf (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->is_inf (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static BOOL is_low (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->is_low (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 step (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->step (a) ;
	}

	static FLT64 sign (CR<BOOL> a) {
		return MathProcHolder::hold (expr)->sign (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 square (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->square (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 sqrt (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->sqrt (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 cubic (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->cubic (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cbrt (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->cbrt (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 pow (CR<ARG1> a ,CR<VAL32> b) {
		return MathProcHolder::hold (expr)->pow (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 hypot (CR<ARG1> a ,CR<ARG1> b) {
		return MathProcHolder::hold (expr)->hypot (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 abs (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->abs (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 inverse (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->inverse (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 floor (CR<ARG1> a ,CR<ARG1> b) {
		return MathProcHolder::hold (expr)->floor (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 ceil (CR<ARG1> a ,CR<ARG1> b) {
		return MathProcHolder::hold (expr)->ceil (a ,b) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 round (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->round (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 fmod (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->fmod (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 clamp (CR<ARG1> a ,CR<ARG1> min_ ,CR<ARG1> max_) {
		return MathProcHolder::hold (expr)->clamp (a ,min_ ,max_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 lerp (CR<FLT64> a ,CR<ARG1> min_ ,CR<ARG1> max_) {
		return MathProcHolder::hold (expr)->lerp (a ,min_ ,max_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cos (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->cos (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 sin (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->sin (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 tan (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->tan (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 acos (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->acos (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 asin (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->asin (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 atan (CR<ARG1> y ,CR<ARG1> x) {
		return MathProcHolder::hold (expr)->atan (y ,x) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 exp (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->exp (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 log (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->log (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 pdf (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->pdf (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cbf (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->cbf (a) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_BOOL<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static BOOL all_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->all_of (a ,MakeWrapper (b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_BOOL<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static BOOL any_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->any_of (a ,MakeWrapper (b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_SCALAR<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static ARG1 max_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->max_of (a ,MakeWrapper (b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<IS_SCALAR<ARG1>> ,class = REQUIRE<ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>>>
	static ARG1 min_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->min_of (a ,MakeWrapper (b...)) ;
	}
} ;

struct NormalErrorLayout {
	LENGTH mCount ;
	FLT64 mMaxError ;
	FLT64 mAvgError ;
	FLT64 mStdError ;

public:
	implicit NormalErrorLayout () noexcept {
		mCount = 0 ;
		mMaxError = 0 ;
		mAvgError = 0 ;
		mStdError = 0 ;
	}
} ;

struct NormalErrorHolder implement Interface {
	imports VFat<NormalErrorHolder> hold (VR<NormalErrorLayout> that) ;
	imports CFat<NormalErrorHolder> hold (CR<NormalErrorLayout> that) ;

	virtual void concat (CR<FLT64> error) = 0 ;
} ;

class NormalError implement NormalErrorLayout {
public:
	implicit NormalError () = default ;

	void concat (CR<FLT64> error) { 
		return NormalErrorHolder::hold (thiz)->concat (error) ;
	}

	forceinline void operator+= (CR<FLT64> error) {
		return concat (error) ;
	}
} ;

struct Notation {
	FLAG mRadix ;
	LENGTH mPrecision ;
	BOOL mSign ;
	VAL64 mMantissa ;
	VAL64 mDownflow ;
	VAL64 mExponent ;
} ;

struct FEXP2CacheLayout ;

struct FEXP2CacheHolder implement Interface {
	imports CR<OfThis<UniqueRef<FEXP2CacheLayout>>> expr_m () ;
	imports VFat<FEXP2CacheHolder> hold (VR<FEXP2CacheLayout> that) ;
	imports CFat<FEXP2CacheHolder> hold (CR<FEXP2CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CR<VAL64> index ,VR<Notation> item) const = 0 ;
} ;

class FEXP2Cache implement OfThis<UniqueRef<FEXP2CacheLayout>> {
public:
	static CR<FEXP2Cache> expr_m () {
		return keep[TYPE<FEXP2Cache>::expr] (FEXP2CacheHolder::expr) ;
	}

	forceinline Notation operator[] (CR<VAL64> index) const {
		Notation ret ;
		FEXP2CacheHolder::hold (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FEXP10CacheLayout ;

struct FEXP10CacheHolder implement Interface {
	imports CR<OfThis<UniqueRef<FEXP10CacheLayout>>> expr_m () ;
	imports VFat<FEXP10CacheHolder> hold (VR<FEXP10CacheLayout> that) ;
	imports CFat<FEXP10CacheHolder> hold (CR<FEXP10CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CR<VAL64> index ,VR<Notation> item) const = 0 ;
} ;

class FEXP10Cache implement OfThis<UniqueRef<FEXP10CacheLayout>> {
public:
	static CR<FEXP10Cache> expr_m () {
		return keep[TYPE<FEXP10Cache>::expr] (FEXP10CacheHolder::expr) ;
	}

	forceinline Notation operator[] (CR<VAL64> index) const {
		Notation ret ;
		FEXP10CacheHolder::hold (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FloatProcLayout ;

struct FloatProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<FloatProcLayout>>> expr_m () ;
	imports VFat<FloatProcHolder> hold (VR<FloatProcLayout> that) ;
	imports CFat<FloatProcHolder> hold (CR<FloatProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH value_precision () const = 0 ;
	virtual LENGTH float_precision () const = 0 ;
	virtual FLT64 encode (CR<Notation> fexp2) const = 0 ;
	virtual Notation decode (CR<FLT64> float_) const = 0 ;
	virtual Notation fexp2_from_fexp10 (CR<Notation> fexp10) const = 0 ;
	virtual Notation fexp10_from_fexp2 (CR<Notation> fexp2) const = 0 ;
} ;

class FloatProc implement OfThis<UniqueRef<FloatProcLayout>> {
public:
	static CR<FloatProc> expr_m () {
		return keep[TYPE<FloatProc>::expr] (FloatProcHolder::expr) ;
	}

	static LENGTH value_precision () {
		return FloatProcHolder::hold (expr)->value_precision () ;
	}

	static LENGTH float_precision () {
		return FloatProcHolder::hold (expr)->float_precision () ;
	}

	static FLT64 encode (CR<Notation> fexp2) {
		return FloatProcHolder::hold (expr)->encode (fexp2) ;
	}

	static Notation decode (CR<FLT64> float_) {
		return FloatProcHolder::hold (expr)->decode (float_) ;
	}

	static Notation fexp2_from_fexp10 (CR<Notation> fexp10) {
		return FloatProcHolder::hold (expr)->fexp2_from_fexp10 (fexp10) ;
	}

	static Notation fexp10_from_fexp2 (CR<Notation> fexp2) {
		return FloatProcHolder::hold (expr)->fexp10_from_fexp2 (fexp2) ;
	}
} ;

struct ByteProcLayout ;

struct ByteProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<ByteProcLayout>>> expr_m () ;
	imports VFat<ByteProcHolder> hold (VR<ByteProcLayout> that) ;
	imports CFat<ByteProcHolder> hold (CR<ByteProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BYTE split_low (CR<WORD> a) const = 0 ;
	virtual WORD split_low (CR<CHAR> a) const = 0 ;
	virtual CHAR split_low (CR<QUAD> a) const = 0 ;
	virtual BYTE split_high (CR<WORD> a) const = 0 ;
	virtual WORD split_high (CR<CHAR> a) const = 0 ;
	virtual CHAR split_high (CR<QUAD> a) const = 0 ;
	virtual WORD merge (CR<BYTE> high_ ,CR<BYTE> low_) const = 0 ;
	virtual CHAR merge (CR<WORD> high_ ,CR<WORD> low_) const = 0 ;
	virtual QUAD merge (CR<CHAR> high_ ,CR<CHAR> low_) const = 0 ;
	virtual BYTE reverse (CR<BYTE> a) const = 0 ;
	virtual WORD reverse (CR<WORD> a) const = 0 ;
	virtual CHAR reverse (CR<CHAR> a) const = 0 ;
	virtual QUAD reverse (CR<QUAD> a) const = 0 ;
	virtual BOOL any_bit (CR<BYTE> a ,CR<BYTE> mask) const = 0 ;
	virtual BOOL any_bit (CR<WORD> a ,CR<WORD> mask) const = 0 ;
	virtual BOOL any_bit (CR<CHAR> a ,CR<CHAR> mask) const = 0 ;
	virtual BOOL any_bit (CR<QUAD> a ,CR<QUAD> mask) const = 0 ;
	virtual BOOL all_bit (CR<BYTE> a ,CR<BYTE> mask) const = 0 ;
	virtual BOOL all_bit (CR<WORD> a ,CR<WORD> mask) const = 0 ;
	virtual BOOL all_bit (CR<CHAR> a ,CR<CHAR> mask) const = 0 ;
	virtual BOOL all_bit (CR<QUAD> a ,CR<QUAD> mask) const = 0 ;
	virtual BYTE binary (CR<BYTE> a) const = 0 ;
	virtual WORD binary (CR<WORD> a) const = 0 ;
	virtual CHAR binary (CR<CHAR> a) const = 0 ;
	virtual QUAD binary (CR<QUAD> a) const = 0 ;
	virtual INDEX pow_bit (CR<LENGTH> nth) const = 0 ;
	virtual LENGTH popcount (CR<BYTE> a) const = 0 ;
	virtual LENGTH lowcount (CR<BYTE> a) const = 0 ;
} ;

class ByteProc implement OfThis<UniqueRef<ByteProcLayout>> {
public:
	static CR<ByteProc> expr_m () {
		return keep[TYPE<ByteProc>::expr] (ByteProcHolder::expr) ;
	}

	static BYTE split_low (CR<WORD> a) {
		return ByteProcHolder::hold (expr)->split_low (a) ;
	}

	static WORD split_low (CR<CHAR> a) {
		return ByteProcHolder::hold (expr)->split_low (a) ;
	}

	static CHAR split_low (CR<QUAD> a) {
		return ByteProcHolder::hold (expr)->split_low (a) ;
	}

	static BYTE split_high (CR<WORD> a) {
		return ByteProcHolder::hold (expr)->split_high (a) ;
	}

	static WORD split_high (CR<CHAR> a) {
		return ByteProcHolder::hold (expr)->split_high (a) ;
	}

	static CHAR split_high (CR<QUAD> a) {
		return ByteProcHolder::hold (expr)->split_high (a) ;
	}

	static WORD merge (CR<BYTE> high_ ,CR<BYTE> low_) {
		return ByteProcHolder::hold (expr)->merge (high_ ,low_) ;
	}

	static CHAR merge (CR<WORD> high_ ,CR<WORD> low_) {
		return ByteProcHolder::hold (expr)->merge (high_ ,low_) ;
	}

	static QUAD merge (CR<CHAR> high_ ,CR<CHAR> low_) {
		return ByteProcHolder::hold (expr)->merge (high_ ,low_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 reverse (CR<ARG1> a) {
		return ByteProcHolder::hold (expr)->reverse (a) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static BOOL any_bit (CR<ARG1> a ,CR<ARG2> mask) {
		return ByteProcHolder::hold (expr)->any_bit (a ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static BOOL all_bit (CR<ARG1> a ,CR<ARG2> mask) {
		return ByteProcHolder::hold (expr)->all_bit (a ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 binary (CR<ARG1> a) {
		return ByteProcHolder::hold (expr)->binary (a) ;
	}

	static INDEX pow_bit (CR<LENGTH> nth) {
		return ByteProcHolder::hold (expr)->pow_bit (nth) ;
	}

	static LENGTH popcount (CR<BYTE> a) {
		return ByteProcHolder::hold (expr)->popcount (a) ;
	}

	static LENGTH lowcount (CR<BYTE> a) {
		return ByteProcHolder::hold (expr)->lowcount (a) ;
	}
} ;

struct IntegerLayout {
	RefBuffer<BYTE> mInteger ;
	LENGTH mWidth ;

public:
	implicit IntegerLayout () noexcept {
		mWidth = 0 ;
	}
} ;

struct IntegerHolder implement Interface {
	imports VFat<IntegerHolder> hold (VR<IntegerLayout> that) ;
	imports CFat<IntegerHolder> hold (CR<IntegerLayout> that) ;

	virtual void initialize (CR<LENGTH> size_) = 0 ;
	virtual void initialize (CR<IntegerLayout> that) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual VAL64 fetch () const = 0 ;
	virtual void store (CR<VAL64> item) = 0 ;
	virtual BOOL equal (CR<IntegerLayout> that) const = 0 ;
	virtual FLAG compr (CR<IntegerLayout> that) const = 0 ;
	virtual void visit (VR<FriendVisitor> visitor) const = 0 ;
	virtual IntegerLayout sadd (CR<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout ssub (CR<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout smul (CR<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout sdiv (CR<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout smod (CR<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout sabs () const = 0 ;
	virtual IntegerLayout minus () const = 0 ;
	virtual IntegerLayout lshift (CR<LENGTH> scale) const = 0 ;
	virtual IntegerLayout rshift (CR<LENGTH> scale) const = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class Integer implement IntegerLayout {
protected:
	using IntegerLayout::mInteger ;

public:
	implicit Integer () = default ;

	explicit Integer (CR<VAL64> item) {
		IntegerHolder::hold (thiz)->initialize (SIZE_OF<VAL64>::expr) ;
		IntegerHolder::hold (thiz)->store (item) ;
	}

	static CR<Integer> zero () {
		return memorize ([&] () {
			return Integer (0) ;
		}) ;
	}

	implicit Integer (CR<Integer> that) {
		IntegerHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Integer> operator= (CR<Integer> that) {
		return assign (thiz ,that) ;
	}

	implicit Integer (RR<Integer> that) = default ;

	forceinline VR<Integer> operator= (RR<Integer> that) = default ;

	Integer clone () const {
		return move (thiz) ;
	}

	LENGTH size () const {
		return IntegerHolder::hold (thiz)->size () ;
	}

	VAL64 fetch () const {
		return IntegerHolder::hold (thiz)->fetch () ;
	}

	forceinline operator VAL64 () const {
		return fetch () ;
	}

	void store (CR<VAL64> item) {
		return IntegerHolder::hold (thiz)->store (item) ;
	}

	BOOL equal (CR<Integer> that) const {
		return IntegerHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Integer> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Integer> that) const {
		return (!equal (that)) ;
	}

	FLAG compr (CR<Integer> that) const {
		return IntegerHolder::hold (thiz)->compr (that) ;
	}

	forceinline BOOL operator< (CR<Integer> that) const {
		return compr (that) < ZERO ;
	}

	forceinline BOOL operator<= (CR<Integer> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline BOOL operator> (CR<Integer> that) const {
		return compr (that) > ZERO ;
	}

	forceinline BOOL operator>= (CR<Integer> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (VR<FriendVisitor> visitor) const {
		return IntegerHolder::hold (thiz)->visit (visitor) ;
	}

	Integer sadd (CR<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator+ (CR<Integer> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<Integer> that) {
		thiz = sadd (that) ;
	}

	Integer ssub (CR<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator- (CR<Integer> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<Integer> that) {
		thiz = ssub (that) ;
	}

	Integer smul (CR<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator* (CR<Integer> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CR<Integer> that) {
		thiz = smul (that) ;
	}

	Integer sdiv (CR<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator/ (CR<Integer> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CR<Integer> that) {
		thiz = sdiv (that) ;
	}

	Integer smod (CR<Integer> that) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->smod (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator% (CR<Integer> that) const {
		return smod (that) ;
	}

	forceinline void operator%= (CR<Integer> that) {
		thiz = smod (that) ;
	}

	Integer sabs () const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	Integer minus () const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator- () const {
		return minus () ;
	}

	Integer lshift (CR<LENGTH> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->lshift (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator<< (CR<LENGTH> scale) const {
		return lshift (scale) ;
	}

	Integer rshift (CR<LENGTH> scale) const {
		IntegerLayout ret = IntegerHolder::hold (thiz)->rshift (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	forceinline Integer operator>> (CR<LENGTH> scale) const {
		return rshift (scale) ;
	}

	void increase () {
		return IntegerHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) {
		return increase () ;
	}

	void decrease () {
		return IntegerHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) {
		return decrease () ;
	}
} ;

struct JetNode ;
using JetEvalFunction = Function<VR<JetNode> ,CR<WrapperLayout>> ;

struct JetNode {
	FLT64 mFX ;
	FLT64 mEX ;
	RefBuffer<FLT64> mDX ;
	INDEX mSlot ;
	JetEvalFunction mEval ;
	Ref<JetNode> mFake ;
	Ref<JetNode> mThat ;
} ;

struct JetLayout {
	Ref<JetNode> mThis ;
} ;

struct JetHolder implement Interface {
	imports VFat<JetHolder> hold (VR<JetLayout> that) ;
	imports CFat<JetHolder> hold (CR<JetLayout> that) ;

	virtual void initialize (CR<LENGTH> size_ ,CR<FLT64> item) = 0 ;
	virtual void initialize (CR<LENGTH> size_ ,CR<FLT64> item ,CR<INDEX> slot) = 0 ;
	virtual void initialize (CR<JetLayout> that) = 0 ;
	virtual FLT64 fx () const = 0 ;
	virtual FLT64 ex () const = 0 ;
	virtual FLT64 dx (CR<INDEX> slot) const = 0 ;
	virtual void once (CR<WrapperLayout> params) const = 0 ;
	virtual JetLayout sadd (CR<JetLayout> that) const = 0 ;
	virtual JetLayout ssub (CR<JetLayout> that) const = 0 ;
	virtual JetLayout smul (CR<JetLayout> that) const = 0 ;
	virtual JetLayout sdiv (CR<JetLayout> that) const = 0 ;
	virtual JetLayout inverse () const = 0 ;
	virtual JetLayout ssqrt () const = 0 ;
	virtual JetLayout scbrt () const = 0 ;
	virtual JetLayout spow (CR<VAL32> that) const = 0 ;
	virtual JetLayout shypot (CR<JetLayout> that) const = 0 ;
	virtual JetLayout sabs () const = 0 ;
	virtual JetLayout minus () const = 0 ;
	virtual JetLayout ssin () const = 0 ;
	virtual JetLayout scos () const = 0 ;
	virtual JetLayout stan () const = 0 ;
	virtual JetLayout sasin () const = 0 ;
	virtual JetLayout sacos () const = 0 ;
	virtual JetLayout satan (CR<JetLayout> that) const = 0 ;
	virtual JetLayout sexp () const = 0 ;
	virtual JetLayout slog () const = 0 ;
} ;

template <class A>
class Jet implement JetLayout {
protected:
	using JetLayout::mThis ;

public:
	implicit Jet () = default ;

	implicit Jet (CR<FLT64> item) {
		JetHolder::hold (thiz)->initialize (A::expr ,item) ;
	}

	explicit Jet (CR<FLT64> item ,CR<INDEX> slot) {
		JetHolder::hold (thiz)->initialize (A::expr ,item ,slot) ;
	}

	implicit Jet (CR<Jet> that) {
		JetHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Jet> operator= (CR<Jet> that) {
		return assign (thiz ,that) ;
	}

	implicit Jet (RR<Jet> that) = default ;

	forceinline VR<Jet> operator= (RR<Jet> that) = default ;

	FLT64 fx () const {
		return JetHolder::hold (thiz)->fx () ;
	}

	FLT64 ex () const {
		return JetHolder::hold (thiz)->ex () ;
	}

	FLT64 dx (CR<INDEX> slot) const {
		return JetHolder::hold (thiz)->dx (slot) ;
	}

	template <class...ARG1 ,class = REQUIRE<ENUM_ALL<IS_SAME<FLT64 ,ARG1>...>>>
	void once (CR<ARG1>...params) const {
		require (ENUM_EQUAL<RANK_OF<TYPE<ARG1...>> ,A>) ;
		return JetHolder::hold (thiz)->once (MakeWrapper (params...)) ;
	}

	template <class...ARG1 ,class = REQUIRE<ENUM_ALL<IS_SAME<FLT64 ,ARG1>...>>>
	forceinline void operator() (CR<ARG1>...params) const {
		return once (params...) ;
	}

	Jet sadd (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator+ (CR<Jet> that) const {
		return sadd (that) ;
	}

	forceinline VR<Jet> operator+= (CR<Jet> that) {
		thiz = sadd (that) ;
	}

	Jet ssub (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator- (CR<Jet> that) const {
		return ssub (that) ;
	}

	forceinline VR<Jet> operator-= (CR<Jet> that) {
		thiz = ssub (that) ;
	}

	Jet smul (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator* (CR<Jet> that) const {
		return smul (that) ;
	}

	forceinline VR<Jet> operator*= (CR<Jet> that) {
		thiz = smul (that) ;
	}

	Jet sdiv (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator/ (CR<Jet> that) const {
		return sdiv (that) ;
	}

	forceinline VR<Jet> operator/= (CR<Jet> that) {
		thiz = sdiv (that) ;
	}

	Jet inverse () const {
		JetLayout ret = JetHolder::hold (thiz)->inverse () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet ssqrt () const {
		JetLayout ret = JetHolder::hold (thiz)->ssqrt () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet scbrt () const {
		JetLayout ret = JetHolder::hold (thiz)->scbrt () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet spow (CR<VAL32> that) const {
		JetLayout ret = JetHolder::hold (thiz)->spow (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet shypot (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->shypot (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sabs () const {
		JetLayout ret = JetHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet minus () const {
		JetLayout ret = JetHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator- () const {
		return minus () ;
	}

	Jet ssin () const {
		JetLayout ret = JetHolder::hold (thiz)->ssin () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet scos () const {
		JetLayout ret = JetHolder::hold (thiz)->scos () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet stan () const {
		JetLayout ret = JetHolder::hold (thiz)->stan () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sasin () const {
		JetLayout ret = JetHolder::hold (thiz)->sasin () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sacos () const {
		JetLayout ret = JetHolder::hold (thiz)->sacos () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet satan (CR<JetLayout> that) const {
		JetLayout ret = JetHolder::hold (thiz)->satan (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet sexp () const {
		JetLayout ret = JetHolder::hold (thiz)->sexp () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	Jet slog () const {
		JetLayout ret = JetHolder::hold (thiz)->slog () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}
} ;

struct HashProcLayout ;

struct HashProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<HashProcLayout>>> expr_m () ;
	imports VFat<HashProcHolder> hold (VR<HashProcLayout> that) ;
	imports CFat<HashProcHolder> hold (CR<HashProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual CHAR fnvhash32 (CR<Pointer> src ,CR<LENGTH> size_) const = 0 ;
	virtual CHAR fnvhash32 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<CHAR> val) const = 0 ;
	virtual QUAD fnvhash64 (CR<Pointer> src ,CR<LENGTH> size_) const = 0 ;
	virtual QUAD fnvhash64 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<QUAD> val) const = 0 ;
	virtual BYTE crchash8 (CR<Pointer> src ,CR<LENGTH> size_) const = 0 ;
	virtual BYTE crchash8 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<BYTE> val) const = 0 ;
	virtual WORD crchash16 (CR<Pointer> src ,CR<LENGTH> size_) const = 0 ;
	virtual WORD crchash16 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<WORD> val) const = 0 ;
} ;

class HashProc implement OfThis<UniqueRef<HashProcLayout>> {
public:
	static CR<HashProc> expr_m () {
		return keep[TYPE<HashProc>::expr] (HashProcHolder::expr) ;
	}

	static CHAR fnvhash32 (CR<Pointer> src ,CR<LENGTH> size_) {
		return HashProcHolder::hold (expr)->fnvhash32 (src ,size_) ;
	}

	static CHAR fnvhash32 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<CHAR> val) {
		return HashProcHolder::hold (expr)->fnvhash32 (src ,size_ ,val) ;
	}

	static QUAD fnvhash64 (CR<Pointer> src ,CR<LENGTH> size_) {
		return HashProcHolder::hold (expr)->fnvhash64 (src ,size_) ;
	}

	static QUAD fnvhash64 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<QUAD> val) {
		return HashProcHolder::hold (expr)->fnvhash64 (src ,size_ ,val) ;
	}

	static BYTE crchash8 (CR<Pointer> src ,CR<LENGTH> size_) {
		return HashProcHolder::hold (expr)->crchash8 (src ,size_) ;
	}

	static BYTE crchash8 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<BYTE> val) {
		return HashProcHolder::hold (expr)->crchash8 (src ,size_ ,val) ;
	}

	static WORD crchash16 (CR<Pointer> src ,CR<LENGTH> size_) {
		return HashProcHolder::hold (expr)->crchash16 (src ,size_) ;
	}

	static WORD crchash16 (CR<Pointer> src ,CR<LENGTH> size_ ,CR<WORD> val) {
		return HashProcHolder::hold (expr)->crchash16 (src ,size_ ,val) ;
	}
} ;
} ;