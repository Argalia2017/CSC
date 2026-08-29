#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
static constexpr auto MATH_E = Flt64 (2.71828182845904523536) ;
static constexpr auto MATH_PI = Flt64 (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = Flt64 (1.41421356237309504880) ;
static constexpr auto MATH_PDF0 = Flt64 (0.398942280401432677939) ;
static constexpr auto MATH_R = Flt64 (0.0174532925199432957692) ;
static constexpr auto MATH_LN2 = Flt64 (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = Flt64 (2.30258509299404568402) ;

struct MathProcLayout ;

struct MathProcHolder implement Interface {
	imports CR<Super<UniqueRef<MathProcLayout>>> expr_m () ;
	imports VFat<MathProcHolder> hold (VR<MathProcLayout> that) ;
	imports CFat<MathProcHolder> hold (CR<MathProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Bool is_inf (CR<Flt32> a) const = 0 ;
	virtual Bool is_inf (CR<Flt64> a) const = 0 ;
	virtual Bool is_low (CR<Flt32> a) const = 0 ;
	virtual Bool is_low (CR<Flt64> a) const = 0 ;
	virtual Val32 step (CR<Val32> a) const = 0 ;
	virtual Val64 step (CR<Val64> a) const = 0 ;
	virtual Flt32 step (CR<Flt32> a) const = 0 ;
	virtual Flt64 step (CR<Flt64> a) const = 0 ;
	virtual Val32 delta (CR<Val32> a) const = 0 ;
	virtual Val64 delta (CR<Val64> a) const = 0 ;
	virtual Flt32 delta (CR<Flt32> a) const = 0 ;
	virtual Flt64 delta (CR<Flt64> a) const = 0 ;
	virtual Val32 sign (CR<Val32> a) const = 0 ;
	virtual Val64 sign (CR<Val64> a) const = 0 ;
	virtual Flt32 sign (CR<Flt32> a) const = 0 ;
	virtual Flt64 sign (CR<Flt64> a) const = 0 ;
	virtual Val32 square (CR<Val32> a) const = 0 ;
	virtual Val64 square (CR<Val64> a) const = 0 ;
	virtual Flt32 square (CR<Flt32> a) const = 0 ;
	virtual Flt64 square (CR<Flt64> a) const = 0 ;
	virtual Flt32 sqrt (CR<Flt32> a) const = 0 ;
	virtual Flt64 sqrt (CR<Flt64> a) const = 0 ;
	virtual Val32 cubic (CR<Val32> a) const = 0 ;
	virtual Val64 cubic (CR<Val64> a) const = 0 ;
	virtual Flt32 cubic (CR<Flt32> a) const = 0 ;
	virtual Flt64 cubic (CR<Flt64> a) const = 0 ;
	virtual Flt32 cbrt (CR<Flt32> a) const = 0 ;
	virtual Flt64 cbrt (CR<Flt64> a) const = 0 ;
	virtual Flt32 pow (CR<Flt32> a ,CR<Val32> b) const = 0 ;
	virtual Flt64 pow (CR<Flt64> a ,CR<Val32> b) const = 0 ;
	virtual Flt32 hypot (CR<Flt32> a ,CR<Flt32> b) const = 0 ;
	virtual Flt64 hypot (CR<Flt64> a ,CR<Flt64> b) const = 0 ;
	virtual Val32 abs (CR<Val32> a) const = 0 ;
	virtual Val64 abs (CR<Val64> a) const = 0 ;
	virtual Flt32 abs (CR<Flt32> a) const = 0 ;
	virtual Flt64 abs (CR<Flt64> a) const = 0 ;
	virtual Flt32 inverse (CR<Flt32> a) const = 0 ;
	virtual Flt64 inverse (CR<Flt64> a) const = 0 ;
	virtual Flt32 floor (CR<Flt32> a ,CR<Flt32> b) const = 0 ;
	virtual Flt64 floor (CR<Flt64> a ,CR<Flt64> b) const = 0 ;
	virtual Flt32 ceil (CR<Flt32> a ,CR<Flt32> b) const = 0 ;
	virtual Flt64 ceil (CR<Flt64> a ,CR<Flt64> b) const = 0 ;
	virtual Flt32 round (CR<Flt32> a) const = 0 ;
	virtual Flt64 round (CR<Flt64> a) const = 0 ;
	virtual Val32 wrap (CR<Val32> a ,CR<Val32> max_) const = 0 ;
	virtual Val64 wrap (CR<Val64> a ,CR<Val64> max_) const = 0 ;
	virtual Val32 clamp (CR<Val32> a ,CR<Val32> min_ ,CR<Val32> max_) const = 0 ;
	virtual Val64 clamp (CR<Val64> a ,CR<Val64> min_ ,CR<Val64> max_) const = 0 ;
	virtual Flt32 clamp (CR<Flt32> a ,CR<Flt32> min_ ,CR<Flt32> max_) const = 0 ;
	virtual Flt64 clamp (CR<Flt64> a ,CR<Flt64> min_ ,CR<Flt64> max_) const = 0 ;
	virtual Val32 lerp (CR<Flt64> a ,CR<Val32> min_ ,CR<Val32> max_) const = 0 ;
	virtual Val64 lerp (CR<Flt64> a ,CR<Val64> min_ ,CR<Val64> max_) const = 0 ;
	virtual Flt32 lerp (CR<Flt64> a ,CR<Flt32> min_ ,CR<Flt32> max_) const = 0 ;
	virtual Flt64 lerp (CR<Flt64> a ,CR<Flt64> min_ ,CR<Flt64> max_) const = 0 ;
	virtual Flt32 cos (CR<Flt32> a) const = 0 ;
	virtual Flt64 cos (CR<Flt64> a) const = 0 ;
	virtual Flt32 sin (CR<Flt32> a) const = 0 ;
	virtual Flt64 sin (CR<Flt64> a) const = 0 ;
	virtual Flt32 tan (CR<Flt32> a) const = 0 ;
	virtual Flt64 tan (CR<Flt64> a) const = 0 ;
	virtual Flt32 acos (CR<Flt32> a) const = 0 ;
	virtual Flt64 acos (CR<Flt64> a) const = 0 ;
	virtual Flt32 asin (CR<Flt32> a) const = 0 ;
	virtual Flt64 asin (CR<Flt64> a) const = 0 ;
	virtual Flt32 atan (CR<Flt32> a) const = 0 ;
	virtual Flt64 atan (CR<Flt64> a) const = 0 ;
	virtual Flt32 atan (CR<Flt32> y ,CR<Flt32> x) const = 0 ;
	virtual Flt64 atan (CR<Flt64> y ,CR<Flt64> x) const = 0 ;
	virtual Flt32 exp (CR<Flt32> a) const = 0 ;
	virtual Flt64 exp (CR<Flt64> a) const = 0 ;
	virtual Flt32 log (CR<Flt32> a) const = 0 ;
	virtual Flt64 log (CR<Flt64> a) const = 0 ;
	virtual Val64 exp2_bit (CR<Val64> a) const = 0 ;
	virtual Val64 log2_bit (CR<Val64> a) const = 0 ;
	virtual Val64 exp10_bit (CR<Val64> a) const = 0 ;
	virtual Val64 log10_bit (CR<Val64> a) const = 0 ;
	virtual Flt32 pdf (CR<Flt32> a) const = 0 ;
	virtual Flt64 pdf (CR<Flt64> a) const = 0 ;
	virtual Flt32 cbf (CR<Flt32> a) const = 0 ;
	virtual Flt64 cbf (CR<Flt64> a) const = 0 ;
	virtual Bool all_of (CR<Wrapper<Bool>> b) const = 0 ;
	virtual Bool any_of (CR<Wrapper<Bool>> b) const = 0 ;
	virtual Val32 max_of (CR<Wrapper<Val32>> b) const = 0 ;
	virtual Val64 max_of (CR<Wrapper<Val64>> b) const = 0 ;
	virtual Flt32 max_of (CR<Wrapper<Flt32>> b) const = 0 ;
	virtual Flt64 max_of (CR<Wrapper<Flt64>> b) const = 0 ;
	virtual Val32 min_of (CR<Wrapper<Val32>> b) const = 0 ;
	virtual Val64 min_of (CR<Wrapper<Val64>> b) const = 0 ;
	virtual Flt32 min_of (CR<Wrapper<Flt32>> b) const = 0 ;
	virtual Flt64 min_of (CR<Wrapper<Flt64>> b) const = 0 ;
	virtual Bool mid_of (CR<Val32> a ,CR<Val32> min_ ,CR<Val32> max_) const = 0 ;
	virtual Bool mid_of (CR<Val64> a ,CR<Val64> min_ ,CR<Val64> max_) const = 0 ;
	virtual Bool mid_of (CR<Flt32> a ,CR<Flt32> min_ ,CR<Flt32> max_) const = 0 ;
	virtual Bool mid_of (CR<Flt64> a ,CR<Flt64> min_ ,CR<Flt64> max_) const = 0 ;
} ;

class MathProc implement Super<UniqueRef<MathProcLayout>> {
public:
	static CR<MathProc> expr_m () {
		return keep[TYPE<MathProc>::expr] (MathProcHolder::expr) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static Bool is_inf (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->is_inf (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static Bool is_low (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->is_low (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 step (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->step (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 delta (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->delta (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 sign (CR<ARG1> a) {
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
	static ARG1 pow (CR<ARG1> a ,CR<Val32> b) {
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

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 wrap (CR<ARG1> a ,CR<ARG1> max_) {
		return MathProcHolder::hold (expr)->wrap (a ,max_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 clamp (CR<ARG1> a ,CR<ARG1> min_ ,CR<ARG1> max_) {
		return MathProcHolder::hold (expr)->clamp (a ,min_ ,max_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_SCALAR<ARG1>>>
	static ARG1 lerp (CR<Flt64> a ,CR<ARG1> min_ ,CR<ARG1> max_) {
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
	static ARG1 atan (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->atan (a) ;
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

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 exp2_bit (CR<ARG1> a) {
		return ARG1 (MathProcHolder::hold (expr)->exp2_bit (Val64 (a))) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 log2_bit (CR<ARG1> a) {
		return ARG1 (MathProcHolder::hold (expr)->log2_bit (Val64 (a))) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 exp10_bit (CR<ARG1> a) {
		return ARG1 (MathProcHolder::hold (expr)->exp10_bit (Val64 (a))) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	static ARG1 log10_bit (CR<ARG1> a) {
		return ARG1 (MathProcHolder::hold (expr)->log10_bit (Val64 (a))) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 pdf (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->pdf (a) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	static ARG1 cbf (CR<ARG1> a) {
		return MathProcHolder::hold (expr)->cbf (a) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_ALL<IS_BOOL<ARG1> ,IS_SAME<ARG1 ,ARG2>...>>>
	static Bool all_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->all_of (MakeWrapper (a ,b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_ALL<IS_BOOL<ARG1> ,IS_SAME<ARG1 ,ARG2>...>>>
	static Bool any_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->any_of (MakeWrapper (a ,b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_ALL<IS_SCALAR<ARG1> ,IS_SAME<ARG1 ,ARG2>...>>>
	static ARG1 max_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->max_of (MakeWrapper (a ,b...)) ;
	}

	template <class ARG1 ,class...ARG2 ,class = REQUIRE<ENUM_ALL<IS_SCALAR<ARG1> ,IS_SAME<ARG1 ,ARG2>...>>>
	static ARG1 min_of (CR<ARG1> a ,CR<ARG2>...b) {
		return MathProcHolder::hold (expr)->min_of (MakeWrapper (a ,b...)) ;
	}

	template <class ARG1 ,class ARG2>
	static ARG1 mid_of (CR<ARG1> a ,CR<ARG2> min_ ,CR<ARG2> max_) {
		return MathProcHolder::hold (expr)->mid_of (a ,ARG1 (min_) ,ARG1 (max_)) ;
	}
} ;

struct NormalErrorLayout {
	Length mCount ;
	Flt64 mMax ;
	Flt64 mAvg ;
	Flt64 mStd ;
	Flt64 mPro ;

public:
	implicit NormalErrorLayout () noexcept {
		mCount = 0 ;
		mMax = 0 ;
		mAvg = 0 ;
		mStd = 0 ;
		mPro = 0 ;
	}
} ;

struct NormalErrorHolder implement Interface {
	imports VFat<NormalErrorHolder> hold (VR<NormalErrorLayout> that) ;
	imports CFat<NormalErrorHolder> hold (CR<NormalErrorLayout> that) ;

	virtual void concat (CR<Flt64> error) = 0 ;
} ;

class NormalError implement NormalErrorLayout {
public:
	implicit NormalError () = default ;

	void concat (CR<Flt64> error) {
		return NormalErrorHolder::hold (thiz)->concat (error) ;
	}

	forceinline void operator+= (CR<Flt64> error) {
		return concat (error) ;
	}
} ;

struct Notation {
	Flag mRadix ;
	Length mPrecision ;
	Bool mSign ;
	Quad mMantissa ;
	Quad mDownflow ;
	Length mExponent ;
} ;

struct FEXP2CacheLayout ;

struct FEXP2CacheHolder implement Interface {
	imports CR<Super<UniqueRef<FEXP2CacheLayout>>> expr_m () ;
	imports VFat<FEXP2CacheHolder> hold (VR<FEXP2CacheLayout> that) ;
	imports CFat<FEXP2CacheHolder> hold (CR<FEXP2CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CR<Val64> index ,VR<Notation> item) const = 0 ;
} ;

class FEXP2Cache implement Super<UniqueRef<FEXP2CacheLayout>> {
public:
	static CR<FEXP2Cache> expr_m () {
		return keep[TYPE<FEXP2Cache>::expr] (FEXP2CacheHolder::expr) ;
	}

	forceinline Notation operator[] (CR<Val64> index) const {
		Notation ret ;
		FEXP2CacheHolder::hold (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FEXP10CacheLayout ;

struct FEXP10CacheHolder implement Interface {
	imports CR<Super<UniqueRef<FEXP10CacheLayout>>> expr_m () ;
	imports VFat<FEXP10CacheHolder> hold (VR<FEXP10CacheLayout> that) ;
	imports CFat<FEXP10CacheHolder> hold (CR<FEXP10CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CR<Val64> index ,VR<Notation> item) const = 0 ;
} ;

class FEXP10Cache implement Super<UniqueRef<FEXP10CacheLayout>> {
public:
	static CR<FEXP10Cache> expr_m () {
		return keep[TYPE<FEXP10Cache>::expr] (FEXP10CacheHolder::expr) ;
	}

	forceinline Notation operator[] (CR<Val64> index) const {
		Notation ret ;
		FEXP10CacheHolder::hold (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FloatProcLayout ;

struct FloatProcHolder implement Interface {
	imports CR<Super<UniqueRef<FloatProcLayout>>> expr_m () ;
	imports VFat<FloatProcHolder> hold (VR<FloatProcLayout> that) ;
	imports CFat<FloatProcHolder> hold (CR<FloatProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Length value_precision () const = 0 ;
	virtual Length float_precision () const = 0 ;
	virtual Flt64 encode (CR<Notation> fexp2) const = 0 ;
	virtual Notation decode (CR<Flt64> float_) const = 0 ;
	virtual Notation fexp2_from_fexp10 (CR<Notation> fexp10) const = 0 ;
	virtual Notation fexp10_from_fexp2 (CR<Notation> fexp2) const = 0 ;
} ;

class FloatProc implement Super<UniqueRef<FloatProcLayout>> {
public:
	static CR<FloatProc> expr_m () {
		return keep[TYPE<FloatProc>::expr] (FloatProcHolder::expr) ;
	}

	static Length value_precision () {
		return FloatProcHolder::hold (expr)->value_precision () ;
	}

	static Length float_precision () {
		return FloatProcHolder::hold (expr)->float_precision () ;
	}

	static Flt64 encode (CR<Notation> fexp2) {
		return FloatProcHolder::hold (expr)->encode (fexp2) ;
	}

	static Notation decode (CR<Flt64> float_) {
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
	imports CR<Super<UniqueRef<ByteProcLayout>>> expr_m () ;
	imports VFat<ByteProcHolder> hold (VR<ByteProcLayout> that) ;
	imports CFat<ByteProcHolder> hold (CR<ByteProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Byte split_low (CR<Word> a) const = 0 ;
	virtual Word split_low (CR<Char> a) const = 0 ;
	virtual Char split_low (CR<Quad> a) const = 0 ;
	virtual Byte split_high (CR<Word> a) const = 0 ;
	virtual Word split_high (CR<Char> a) const = 0 ;
	virtual Char split_high (CR<Quad> a) const = 0 ;
	virtual Word merge (CR<Byte> high_ ,CR<Byte> low_) const = 0 ;
	virtual Char merge (CR<Word> high_ ,CR<Word> low_) const = 0 ;
	virtual Quad merge (CR<Char> high_ ,CR<Char> low_) const = 0 ;
	virtual Byte shift (CR<Byte> high_ ,CR<Byte> low_ ,CR<Index> high_bit) const = 0 ;
	virtual Word shift (CR<Word> high_ ,CR<Word> low_ ,CR<Index> high_bit) const = 0 ;
	virtual Char shift (CR<Char> high_ ,CR<Char> low_ ,CR<Index> high_bit) const = 0 ;
	virtual Quad shift (CR<Quad> high_ ,CR<Quad> low_ ,CR<Index> high_bit) const = 0 ;
	virtual Byte reverse (CR<Byte> a) const = 0 ;
	virtual Word reverse (CR<Word> a) const = 0 ;
	virtual Char reverse (CR<Char> a) const = 0 ;
	virtual Quad reverse (CR<Quad> a) const = 0 ;
	virtual Bool any_bit (CR<Byte> a ,CR<Byte> mask) const = 0 ;
	virtual Bool any_bit (CR<Word> a ,CR<Word> mask) const = 0 ;
	virtual Bool any_bit (CR<Char> a ,CR<Char> mask) const = 0 ;
	virtual Bool any_bit (CR<Quad> a ,CR<Quad> mask) const = 0 ;
	virtual Bool all_bit (CR<Byte> a ,CR<Byte> mask) const = 0 ;
	virtual Bool all_bit (CR<Word> a ,CR<Word> mask) const = 0 ;
	virtual Bool all_bit (CR<Char> a ,CR<Char> mask) const = 0 ;
	virtual Bool all_bit (CR<Quad> a ,CR<Quad> mask) const = 0 ;
	virtual Byte binary (CR<Byte> a) const = 0 ;
	virtual Word binary (CR<Word> a) const = 0 ;
	virtual Char binary (CR<Char> a) const = 0 ;
	virtual Quad binary (CR<Quad> a) const = 0 ;
	virtual Length pop_count (CR<Byte> a) const = 0 ;
	virtual Length low_count (CR<Byte> a) const = 0 ;
} ;

class ByteProc implement Super<UniqueRef<ByteProcLayout>> {
public:
	static CR<ByteProc> expr_m () {
		return keep[TYPE<ByteProc>::expr] (ByteProcHolder::expr) ;
	}

	static Byte split_low (CR<Word> a) {
		return ByteProcHolder::hold (expr)->split_low (a) ;
	}

	static Word split_low (CR<Char> a) {
		return ByteProcHolder::hold (expr)->split_low (a) ;
	}

	static Char split_low (CR<Quad> a) {
		return ByteProcHolder::hold (expr)->split_low (a) ;
	}

	static Byte split_high (CR<Word> a) {
		return ByteProcHolder::hold (expr)->split_high (a) ;
	}

	static Word split_high (CR<Char> a) {
		return ByteProcHolder::hold (expr)->split_high (a) ;
	}

	static Char split_high (CR<Quad> a) {
		return ByteProcHolder::hold (expr)->split_high (a) ;
	}

	static Word merge (CR<Byte> high_ ,CR<Byte> low_) {
		return ByteProcHolder::hold (expr)->merge (high_ ,low_) ;
	}

	static Char merge (CR<Word> high_ ,CR<Word> low_) {
		return ByteProcHolder::hold (expr)->merge (high_ ,low_) ;
	}

	static Quad merge (CR<Char> high_ ,CR<Char> low_) {
		return ByteProcHolder::hold (expr)->merge (high_ ,low_) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 shift (CR<ARG1> high_ ,CR<ARG1> low_ ,CR<Index> high_bit) {
		return ByteProcHolder::hold (expr)->shift (high_ ,low_ ,high_bit) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 reverse (CR<ARG1> a) {
		return ByteProcHolder::hold (expr)->reverse (a) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static Bool any_bit (CR<ARG1> a ,CR<ARG2> mask) {
		return ByteProcHolder::hold (expr)->any_bit (a ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class ARG2 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static Bool all_bit (CR<ARG1> a ,CR<ARG2> mask) {
		return ByteProcHolder::hold (expr)->all_bit (a ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_BYTE<ARG1>>>
	static ARG1 binary (CR<ARG1> a) {
		return ByteProcHolder::hold (expr)->binary (a) ;
	}

	static Length pop_count (CR<Byte> a) {
		return ByteProcHolder::hold (expr)->pop_count (a) ;
	}

	static Length low_count (CR<Byte> a) {
		return ByteProcHolder::hold (expr)->low_count (a) ;
	}
} ;

struct BigRealLayout {
	RefBuffer<Byte> mReal ;
	Length mWidth ;
	Length mShift ;

public:
	implicit BigRealLayout () noexcept {
		mWidth = 0 ;
		mShift = 0 ;
	}
} ;

struct BigRealHolder implement Interface {
	imports VFat<BigRealHolder> hold (VR<BigRealLayout> that) ;
	imports CFat<BigRealHolder> hold (CR<BigRealLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void initialize (CR<BigRealLayout> that) = 0 ;
	virtual Length size () const = 0 ;
	virtual Index precision () const = 0 ;
	virtual Flt64 fetch () const = 0 ;
	virtual void store (CR<Flt64> item) = 0 ;
	virtual Bool equal (CR<BigRealLayout> that) const = 0 ;
	virtual Flag compr (CR<BigRealLayout> that) const = 0 ;
	virtual void visit (CR<Visitor> visitor) const = 0 ;
	virtual BigRealLayout sadd (CR<BigRealLayout> that) const = 0 ;
	virtual BigRealLayout ssub (CR<BigRealLayout> that) const = 0 ;
	virtual BigRealLayout smul (CR<BigRealLayout> that) const = 0 ;
	virtual BigRealLayout sdiv (CR<BigRealLayout> that) const = 0 ;
	virtual BigRealLayout smod (CR<BigRealLayout> that) const = 0 ;
	virtual BigRealLayout sabs () const = 0 ;
	virtual BigRealLayout minus () const = 0 ;
	virtual BigRealLayout shift (CR<Length> scale) const = 0 ;
	virtual BigRealLayout sround () const = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class BigReal implement BigRealLayout {
protected:
	using BigRealLayout::mReal ;
	using BigRealLayout::mWidth ;
	using BigRealLayout::mShift ;

public:
	implicit BigReal () = default ;

	explicit BigReal (CR<Flt64> item) {
		BigRealHolder::hold (thiz)->initialize (512) ;
		BigRealHolder::hold (thiz)->store (item) ;
	}

	static CR<BigReal> zero () {
		return memorize ([&] () {
			return BigReal (0) ;
		}) ;
	}

	static CR<BigReal> iden () {
		return memorize ([&] () {
			return BigReal (1) ;
		}) ;
	}

	implicit BigReal (CR<BigReal> that) {
		BigRealHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<BigReal> operator= (CR<BigReal> that) {
		return assign (thiz ,that) ;
	}

	implicit BigReal (RR<BigReal> that) = default ;

	forceinline VR<BigReal> operator= (RR<BigReal> that) = default ;

	BigReal clone () const {
		return move (thiz) ;
	}

	Length size () const {
		return BigRealHolder::hold (thiz)->size () ;
	}

	Index precision () const {
		return BigRealHolder::hold (thiz)->precision () ;
	}

	Flt64 fetch () const {
		return BigRealHolder::hold (thiz)->fetch () ;
	}

	forceinline explicit operator Flt64 () const {
		return fetch () ;
	}

	void store (CR<Flt64> item) {
		return BigRealHolder::hold (thiz)->store (item) ;
	}

	Bool equal (CR<BigReal> that) const {
		return BigRealHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<BigReal> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<BigReal> that) const {
		return (!equal (that)) ;
	}

	Flag compr (CR<BigReal> that) const {
		return BigRealHolder::hold (thiz)->compr (that) ;
	}

	forceinline Bool operator< (CR<BigReal> that) const {
		return compr (that) < ZERO ;
	}

	forceinline Bool operator<= (CR<BigReal> that) const {
		return compr (that) <= ZERO ;
	}

	forceinline Bool operator> (CR<BigReal> that) const {
		return compr (that) > ZERO ;
	}

	forceinline Bool operator>= (CR<BigReal> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CR<Visitor> visitor) const {
		return BigRealHolder::hold (thiz)->visit (visitor) ;
	}

	BigReal sadd (CR<BigReal> that) const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator+ (CR<Flt64> that) const = delete ;

	forceinline BigReal operator+ (CR<BigReal> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<BigReal> that) {
		thiz = sadd (that) ;
	}

	BigReal ssub (CR<BigReal> that) const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator- (CR<Flt64> that) const = delete ;

	forceinline BigReal operator- (CR<BigReal> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<BigReal> that) {
		thiz = ssub (that) ;
	}

	BigReal smul (CR<BigReal> that) const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator* (CR<Flt64> that) const = delete ;

	forceinline BigReal operator* (CR<BigReal> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CR<BigReal> that) {
		thiz = smul (that) ;
	}

	BigReal sdiv (CR<BigReal> that) const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator/ (CR<Flt64> that) const = delete ;

	forceinline BigReal operator/ (CR<BigReal> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CR<BigReal> that) {
		thiz = sdiv (that) ;
	}

	BigReal smod (CR<BigReal> that) const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->smod (that) ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator% (CR<Flt64> that) const = delete ;

	forceinline BigReal operator% (CR<BigReal> that) const {
		return smod (that) ;
	}

	forceinline void operator%= (CR<BigReal> that) {
		thiz = smod (that) ;
	}

	BigReal sabs () const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	BigReal minus () const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator+ () const {
		return thiz ;
	}

	forceinline BigReal operator- () const {
		return minus () ;
	}

	BigReal shift (CR<Length> scale) const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->shift (scale) ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	forceinline BigReal operator<< (CR<Length> scale) const {
		return shift (+scale) ;
	}

	forceinline BigReal operator>> (CR<Length> scale) const {
		return shift (-scale) ;
	}

	BigReal sround () const {
		BigRealLayout ret = BigRealHolder::hold (thiz)->sround () ;
		return move (keep[TYPE<BigReal>::expr] (ret)) ;
	}

	void increase () {
		return BigRealHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) {
		return increase () ;
	}

	void decrease () {
		return BigRealHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) {
		return decrease () ;
	}
} ;

struct JetNode ;
struct JetTree ;

using JetIndex = Tuple<Index ,Index> ;

struct JetLayout {
	SharedRef<JetTree> mThis ;
	JetIndex mIndex ;
} ;

struct JetHolder implement Interface {
	imports VFat<JetHolder> hold (VR<JetLayout> that) ;
	imports CFat<JetHolder> hold (CR<JetLayout> that) ;

	virtual void initialize (CR<Length> size_ ,CR<Flt64> item) = 0 ;
	virtual void initialize (CR<Length> size_ ,CR<Flt64> item ,CR<Index> slot) = 0 ;
	virtual Flt64 fx () const = 0 ;
	virtual Flt64 ex () const = 0 ;
	virtual Flt64 dx (CR<Index> slot) const = 0 ;
	virtual void once (CR<Wrapper<Flt64>> params) const = 0 ;
	virtual JetLayout sadd (CR<JetLayout> that) const = 0 ;
	virtual JetLayout ssub (CR<JetLayout> that) const = 0 ;
	virtual JetLayout smul (CR<JetLayout> that) const = 0 ;
	virtual JetLayout sdiv (CR<JetLayout> that) const = 0 ;
	virtual JetLayout inverse () const = 0 ;
	virtual JetLayout ssqrt () const = 0 ;
	virtual JetLayout scbrt () const = 0 ;
	virtual JetLayout spow (CR<Val32> that) const = 0 ;
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
	virtual JetLayout relu () const = 0 ;
} ;

template <class A>
class Jet implement JetLayout {
protected:
	using JetLayout::mThis ;
	using JetLayout::mIndex ;

public:
	implicit Jet () = default ;

	implicit Jet (CR<Flt64> item) {
		JetHolder::hold (thiz)->initialize (A::expr ,item) ;
	}

	explicit Jet (CR<Flt64> item ,CR<Index> slot) {
		JetHolder::hold (thiz)->initialize (A::expr ,item ,slot) ;
	}

	Flt64 fx () const {
		return JetHolder::hold (thiz)->fx () ;
	}

	Flt64 ex () const {
		return JetHolder::hold (thiz)->ex () ;
	}

	Flt64 dx (CR<Index> slot) const {
		return JetHolder::hold (thiz)->dx (slot) ;
	}

	template <class...ARG1>
	void once (CR<ARG1>...params) const {
		return JetHolder::hold (thiz)->once (MakeWrapper (Flt64 (params)...)) ;
	}

	template <class...ARG1>
	forceinline CR<Jet> operator() (CR<ARG1>...params) const {
		once (params...) ;
		return thiz ;
	}

	Jet sadd (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator+ (CR<Jet> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<Jet> that) {
		thiz = sadd (that) ;
	}

	Jet ssub (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator- (CR<Jet> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<Jet> that) {
		thiz = ssub (that) ;
	}

	Jet smul (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator* (CR<Jet> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CR<Jet> that) {
		thiz = smul (that) ;
	}

	Jet sdiv (CR<Jet> that) const {
		JetLayout ret = JetHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}

	forceinline Jet operator/ (CR<Jet> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CR<Jet> that) {
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

	Jet spow (CR<Val32> that) const {
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

	forceinline Jet operator+ () const {
		return thiz ;
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

	Jet relu () const {
		JetLayout ret = JetHolder::hold (thiz)->relu () ;
		return move (keep[TYPE<Jet>::expr] (ret)) ;
	}
} ;

struct HashProcLayout ;

struct HashProcHolder implement Interface {
	imports CR<Super<UniqueRef<HashProcLayout>>> expr_m () ;
	imports VFat<HashProcHolder> hold (VR<HashProcLayout> that) ;
	imports CFat<HashProcHolder> hold (CR<HashProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Char fnvhash32 (CR<Pointer> src ,CR<Length> size_) const = 0 ;
	virtual Char fnvhash32 (CR<Pointer> src ,CR<Length> size_ ,CR<Char> val) const = 0 ;
	virtual Quad fnvhash64 (CR<Pointer> src ,CR<Length> size_) const = 0 ;
	virtual Quad fnvhash64 (CR<Pointer> src ,CR<Length> size_ ,CR<Quad> val) const = 0 ;
	virtual Byte crchash8 (CR<Pointer> src ,CR<Length> size_) const = 0 ;
	virtual Byte crchash8 (CR<Pointer> src ,CR<Length> size_ ,CR<Byte> val) const = 0 ;
	virtual Word crchash16 (CR<Pointer> src ,CR<Length> size_) const = 0 ;
	virtual Word crchash16 (CR<Pointer> src ,CR<Length> size_ ,CR<Word> val) const = 0 ;
	virtual Char crchash32 (CR<Pointer> src ,CR<Length> size_) const = 0 ;
	virtual Char crchash32 (CR<Pointer> src ,CR<Length> size_ ,CR<Char> val) const = 0 ;
} ;

class HashProc implement Super<UniqueRef<HashProcLayout>> {
public:
	static CR<HashProc> expr_m () {
		return keep[TYPE<HashProc>::expr] (HashProcHolder::expr) ;
	}

	static Char fnvhash32 (CR<Pointer> src ,CR<Length> size_) {
		return HashProcHolder::hold (expr)->fnvhash32 (src ,size_) ;
	}

	static Char fnvhash32 (CR<Pointer> src ,CR<Length> size_ ,CR<Char> val) {
		return HashProcHolder::hold (expr)->fnvhash32 (src ,size_ ,val) ;
	}

	static Quad fnvhash64 (CR<Pointer> src ,CR<Length> size_) {
		return HashProcHolder::hold (expr)->fnvhash64 (src ,size_) ;
	}

	static Quad fnvhash64 (CR<Pointer> src ,CR<Length> size_ ,CR<Quad> val) {
		return HashProcHolder::hold (expr)->fnvhash64 (src ,size_ ,val) ;
	}

	static Byte crchash8 (CR<Pointer> src ,CR<Length> size_) {
		return HashProcHolder::hold (expr)->crchash8 (src ,size_) ;
	}

	static Byte crchash8 (CR<Pointer> src ,CR<Length> size_ ,CR<Byte> val) {
		return HashProcHolder::hold (expr)->crchash8 (src ,size_ ,val) ;
	}

	static Word crchash16 (CR<Pointer> src ,CR<Length> size_) {
		return HashProcHolder::hold (expr)->crchash16 (src ,size_) ;
	}

	static Word crchash16 (CR<Pointer> src ,CR<Length> size_ ,CR<Word> val) {
		return HashProcHolder::hold (expr)->crchash16 (src ,size_ ,val) ;
	}

	static Char crchash32 (CR<Pointer> src ,CR<Length> size_) {
		return HashProcHolder::hold (expr)->crchash32 (src ,size_) ;
	}

	static Char crchash32 (CR<Pointer> src ,CR<Length> size_ ,CR<Char> val) {
		return HashProcHolder::hold (expr)->crchash32 (src ,size_ ,val) ;
	}
} ;
} ;