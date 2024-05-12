#pragma once

#ifndef __CSC_MATH__
#define __CSC_MATH__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
struct MathProcPureLayout ;

struct MathProcLayout {
	Ref<MathProcPureLayout> mThis ;
} ;

struct MathProcHolder implement Interface {
	imports VFat<MathProcHolder> create (VREF<MathProcLayout> that) ;
	imports CFat<MathProcHolder> create (CREF<MathProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL is_inf (CREF<FLT32> a) const = 0 ;
	virtual BOOL is_inf (CREF<FLT64> a) const = 0 ;
	virtual VAL32 step (CREF<VAL32> a) const = 0 ;
	virtual VAL64 step (CREF<VAL64> a) const = 0 ;
	virtual FLT32 step (CREF<FLT32> a) const = 0 ;
	virtual FLT64 step (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sign (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sign (CREF<FLT64> a) const = 0 ;
	virtual VAL32 square (CREF<VAL32> a) const = 0 ;
	virtual VAL64 square (CREF<VAL64> a) const = 0 ;
	virtual FLT32 square (CREF<FLT32> a) const = 0 ;
	virtual FLT64 square (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sqrt (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sqrt (CREF<FLT64> a) const = 0 ;
	virtual VAL32 cubic (CREF<VAL32> a) const = 0 ;
	virtual VAL64 cubic (CREF<VAL64> a) const = 0 ;
	virtual FLT32 cubic (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cubic (CREF<FLT64> a) const = 0 ;
	virtual FLT32 cbrt (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cbrt (CREF<FLT64> a) const = 0 ;
	virtual VAL32 abs (CREF<VAL32> a) const = 0 ;
	virtual VAL64 abs (CREF<VAL64> a) const = 0 ;
	virtual FLT32 abs (CREF<FLT32> a) const = 0 ;
	virtual FLT64 abs (CREF<FLT64> a) const = 0 ;
	virtual FLT32 inverse (CREF<FLT32> a) const = 0 ;
	virtual FLT64 inverse (CREF<FLT64> a) const = 0 ;
	virtual FLT32 floor (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 floor (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual FLT32 round (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 round (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual FLT32 ceil (CREF<FLT32> a ,CREF<FLT32> b) const = 0 ;
	virtual FLT64 ceil (CREF<FLT64> a ,CREF<FLT64> b) const = 0 ;
	virtual VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const = 0 ;
	virtual VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const = 0 ;
	virtual FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> lb ,CREF<FLT32> rb) const = 0 ;
	virtual FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> lb ,CREF<FLT64> rb) const = 0 ;
	virtual FLT32 cos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 sin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 sin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 tan (CREF<FLT32> a) const = 0 ;
	virtual FLT64 tan (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arccos (CREF<FLT32> a) const = 0 ;
	virtual FLT64 arccos (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arcsin (CREF<FLT32> a) const = 0 ;
	virtual FLT64 arcsin (CREF<FLT64> a) const = 0 ;
	virtual FLT32 arctan (CREF<FLT32> y ,CREF<FLT32> x) const = 0 ;
	virtual FLT64 arctan (CREF<FLT64> y ,CREF<FLT64> x) const = 0 ;
	virtual FLT32 exp (CREF<FLT32> a) const = 0 ;
	virtual FLT64 exp (CREF<FLT64> a) const = 0 ;
	virtual FLT32 log (CREF<FLT32> a) const = 0 ;
	virtual FLT64 log (CREF<FLT64> a) const = 0 ;
	virtual FLT32 pdf (CREF<FLT32> a) const = 0 ;
	virtual FLT64 pdf (CREF<FLT64> a) const = 0 ;
	virtual FLT32 cbf (CREF<FLT32> a) const = 0 ;
	virtual FLT64 cbf (CREF<FLT64> a) const = 0 ;
	virtual BOOL all_of (CREF<BOOL> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual BOOL any_of (CREF<BOOL> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual VAL32 max_of (CREF<VAL32> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual VAL64 max_of (CREF<VAL64> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual FLT32 max_of (CREF<FLT32> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual FLT64 max_of (CREF<FLT64> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual VAL32 min_of (CREF<VAL32> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual VAL64 min_of (CREF<VAL64> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual FLT32 min_of (CREF<FLT32> a ,CREF<CaptureLayout> b) const = 0 ;
	virtual FLT64 min_of (CREF<FLT64> a ,CREF<CaptureLayout> b) const = 0 ;
} ;

class MathProc implement MathProcLayout {
protected:
	using MathProcLayout::mThis ;

public:
	imports CREF<MathProc> instance () {
		return memorize ([&] () {
			MathProc ret ;
			MathProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	template <class ARG1>
	imports BOOL is_inf (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->is_inf (a) ;
	}

	template <class ARG1>
	imports ARG1 step (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->step (a) ;
	}

	template <class ARG1>
	imports ARG1 sign (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->sign (a) ;
	}

	template <class ARG1>
	imports ARG1 square (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->square (a) ;
	}

	template <class ARG1>
	imports ARG1 sqrt (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->sqrt (a) ;
	}

	template <class ARG1>
	imports ARG1 cubic (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->cubic (a) ;
	}

	template <class ARG1>
	imports ARG1 cbrt (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->cbrt (a) ;
	}

	template <class ARG1>
	imports ARG1 abs (CREF<ARG1> a) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->abs (a) ;
	}

	template <class ARG1>
	imports ARG1 inverse (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->inverse (a) ;
	}

	template <class ARG1>
	imports ARG1 floor (CREF<ARG1> a ,CREF<ARG1> b) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->floor (a ,b) ;
	}

	template <class ARG1>
	imports ARG1 round (CREF<ARG1> a ,CREF<ARG1> b) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->round (a ,b) ;
	}

	template <class ARG1>
	imports ARG1 ceil (CREF<ARG1> a ,CREF<ARG1> b) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->ceil (a ,b) ;
	}

	template <class ARG1>
	imports ARG1 clamp (CREF<ARG1> a ,CREF<ARG1> lb ,CREF<ARG1> rb) {
		require (IS_SCALAR<ARG1>) ;
		return MathProcHolder::create (instance ())->clamp (a ,lb ,rb) ;
	}

	template <class ARG1>
	imports ARG1 cos (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->cos (a) ;
	}

	template <class ARG1>
	imports ARG1 sin (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->sin (a) ;
	}

	template <class ARG1>
	imports ARG1 tan (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->tan (a) ;
	}

	template <class ARG1>
	imports ARG1 arccos (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->arccos (a) ;
	}

	template <class ARG1>
	imports ARG1 arcsin (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->arcsin (a) ;
	}

	template <class ARG1>
	imports ARG1 arctan (CREF<ARG1> y ,CREF<ARG1> x) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->arctan (y ,x) ;
	}

	template <class ARG1>
	imports ARG1 exp (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->exp (a) ;
	}

	template <class ARG1>
	imports ARG1 log (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->log (a) ;
	}

	template <class ARG1>
	imports ARG1 pdf (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->pdf (a) ;
	}

	template <class ARG1>
	imports ARG1 cbf (CREF<ARG1> a) {
		require (IS_FLOAT<ARG1>) ;
		return MathProcHolder::create (instance ())->cbf (a) ;
	}

	template <class ARG1 ,class...ARG2>
	imports BOOL all_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		require (ENUM_ALL<IS_BOOL<ARG1>>) ;
		require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
		return MathProcHolder::create (instance ())->all_of (a ,Capture<CREF<ARG2>...> (b...)) ;
	}

	template <class ARG1 ,class...ARG2>
	imports BOOL any_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		require (ENUM_ALL<IS_BOOL<ARG1>>) ;
		require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
		return MathProcHolder::create (instance ())->any_of (a ,Capture<CREF<ARG2>...> (b...)) ;
	}

	template <class ARG1 ,class...ARG2>
	imports ARG1 max_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		require (ENUM_ALL<IS_SCALAR<ARG1>>) ;
		require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
		return MathProcHolder::create (instance ())->max_of (a ,Capture<CREF<ARG2>...> (b...)) ;
	}

	template <class ARG1 ,class...ARG2>
	imports ARG1 min_of (CREF<ARG1> a ,CREF<ARG2>...b) {
		require (ENUM_ALL<IS_SCALAR<ARG1>>) ;
		require (ENUM_ALL<IS_SAME<ARG1 ,ARG2>...>) ;
		return MathProcHolder::create (instance ())->min_of (a ,Capture<CREF<ARG2>...> (b...)) ;
	}
} ;

struct NormalError {
	LENGTH mCount ;
	FLT64 mMaxError ;
	FLT64 mAvgError ;
	FLT64 mStdError ;

public:
	forceinline void operator+= (CREF<FLT64> error) {
		const auto r1x = FLT64 (mCount) ;
		const auto r2x = MathProc::inverse (r1x + 1) ;
		const auto r3x = error - mAvgError ;
		mMaxError = MathProc::max_of (mMaxError ,error) ;
		mAvgError = mAvgError + r3x * r2x ;
		const auto r4x = r1x * r2x * MathProc::square (mStdError) + r1x * MathProc::square (r3x * r2x) ;
		mStdError = MathProc::sqrt (r4x) ;
		mCount = LENGTH (r1x + 1) ;
	}
} ;

struct Notation {
	FLAG mRadix ;
	BOOL mSign ;
	LENGTH mPrecision ;
	VAL64 mMantissa ;
	VAL64 mDownflow ;
	VAL64 mExponent ;
} ;

struct FEXP2CacheLayout {} ;

struct FEXP2CacheHolder implement Interface {
	imports VFat<FEXP2CacheHolder> create (VREF<FEXP2CacheLayout> that) ;
	imports CFat<FEXP2CacheHolder> create (CREF<FEXP2CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CREF<VAL64> index ,VREF<Notation> item) const = 0 ;
} ;

class FEXP2Cache implement FEXP2CacheLayout {
public:
	imports CREF<FEXP2Cache> instance () {
		return memorize ([&] () {
			FEXP2Cache ret ;
			FEXP2CacheHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	forceinline Notation operator[] (CREF<VAL64> index) const {
		Notation ret ;
		FEXP2CacheHolder::create (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FEXP10CacheLayout {} ;

struct FEXP10CacheHolder implement Interface {
	imports VFat<FEXP10CacheHolder> create (VREF<FEXP10CacheLayout> that) ;
	imports CFat<FEXP10CacheHolder> create (CREF<FEXP10CacheLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void get (CREF<VAL64> index ,VREF<Notation> item) const = 0 ;
} ;

class FEXP10Cache implement FEXP10CacheLayout {
public:
	imports CREF<FEXP10Cache> instance () {
		return memorize ([&] () {
			FEXP10Cache ret ;
			FEXP10CacheHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	forceinline Notation operator[] (CREF<VAL64> index) const {
		Notation ret ;
		FEXP10CacheHolder::create (thiz)->get (index ,ret) ;
		return move (ret) ;
	}
} ;

struct FloatProcPureLayout ;

struct FloatProcLayout {
	Ref<FloatProcPureLayout> mThis ;
} ;

struct FloatProcHolder implement Interface {
	imports VFat<FloatProcHolder> create (VREF<FloatProcLayout> that) ;
	imports CFat<FloatProcHolder> create (CREF<FloatProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual FLT64 encode (CREF<Notation> fexp2) const = 0 ;
	virtual Notation decode (CREF<FLT64> float_) const = 0 ;
	virtual Notation fexp2_from_fexp10 (CREF<Notation> fexp10) const = 0 ;
	virtual Notation fexp10_from_fexp2 (CREF<Notation> fexp2) const = 0 ;
} ;

class FloatProc implement FloatProcLayout {
protected:
	using FloatProcLayout::mThis ;

public:
	imports CREF<FloatProc> instance () {
		return memorize ([&] () {
			FloatProc ret ;
			FloatProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports FLT64 encode (CREF<Notation> fexp2) {
		return FloatProcHolder::create (instance ())->encode (fexp2) ;
	}

	imports Notation decode (CREF<FLT64> float_) {
		return FloatProcHolder::create (instance ())->decode (float_) ;
	}

	imports Notation fexp2_from_fexp10 (CREF<Notation> fexp10) {
		return FloatProcHolder::create (instance ())->fexp2_from_fexp10 (fexp10) ;
	}

	imports Notation fexp10_from_fexp2 (CREF<Notation> fexp2) {
		return FloatProcHolder::create (instance ())->fexp10_from_fexp2 (fexp2) ;
	}
} ;

struct ByteProcPureLayout ;

struct ByteProcLayout {
	Ref<ByteProcPureLayout> mThis ;
} ;

struct ByteProcHolder implement Interface {
	imports VFat<ByteProcHolder> create (VREF<ByteProcLayout> that) ;
	imports CFat<ByteProcHolder> create (CREF<ByteProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BYTE bit_low (CREF<WORD> a) const = 0 ;
	virtual WORD bit_low (CREF<CHAR> a) const = 0 ;
	virtual CHAR bit_low (CREF<QUAD> a) const = 0 ;
	virtual BYTE bit_high (CREF<WORD> a) const = 0 ;
	virtual WORD bit_high (CREF<CHAR> a) const = 0 ;
	virtual CHAR bit_high (CREF<QUAD> a) const = 0 ;
	virtual WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) const = 0 ;
	virtual CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) const = 0 ;
	virtual QUAD bit_merge (CREF<CHAR> high ,CREF<CHAR> low) const = 0 ;
	virtual BOOL bit_any (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
	virtual BOOL bit_any (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
	virtual BOOL bit_any (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
	virtual BOOL bit_any (CREF<QUAD> base ,CREF<QUAD> mask) const = 0 ;
	virtual BOOL bit_all (CREF<BYTE> base ,CREF<BYTE> mask) const = 0 ;
	virtual BOOL bit_all (CREF<WORD> base ,CREF<WORD> mask) const = 0 ;
	virtual BOOL bit_all (CREF<CHAR> base ,CREF<CHAR> mask) const = 0 ;
	virtual BOOL bit_all (CREF<QUAD> base ,CREF<QUAD> mask) const = 0 ;
	virtual BYTE bit_reverse (CREF<BYTE> a) const = 0 ;
	virtual WORD bit_reverse (CREF<WORD> a) const = 0 ;
	virtual CHAR bit_reverse (CREF<CHAR> a) const = 0 ;
	virtual QUAD bit_reverse (CREF<QUAD> a) const = 0 ;
	virtual INDEX bit_pow (CREF<LENGTH> nth) const = 0 ;
	virtual LENGTH popcount (CREF<BYTE> a) const = 0 ;
	virtual LENGTH lowcount (CREF<BYTE> a) const = 0 ;
} ;

class ByteProc implement ByteProcLayout {
protected:
	using ByteProcLayout::mThis ;

public:
	imports CREF<ByteProc> instance () {
		return memorize ([&] () {
			ByteProc ret ;
			ByteProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports BYTE bit_low (CREF<WORD> a) {
		return ByteProcHolder::create (instance ())->bit_low (a) ;
	}

	imports WORD bit_low (CREF<CHAR> a) {
		return ByteProcHolder::create (instance ())->bit_low (a) ;
	}

	imports CHAR bit_low (CREF<QUAD> a) {
		return ByteProcHolder::create (instance ())->bit_low (a) ;
	}

	imports BYTE bit_high (CREF<WORD> a) {
		return ByteProcHolder::create (instance ())->bit_high (a) ;
	}

	imports WORD bit_high (CREF<CHAR> a) {
		return ByteProcHolder::create (instance ())->bit_high (a) ;
	}

	imports CHAR bit_high (CREF<QUAD> a) {
		return ByteProcHolder::create (instance ())->bit_high (a) ;
	}

	imports WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) {
		return ByteProcHolder::create (instance ())->bit_merge (high ,low) ;
	}

	imports CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) {
		return ByteProcHolder::create (instance ())->bit_merge (high ,low) ;
	}

	imports QUAD bit_merge (CREF<CHAR> high ,CREF<CHAR> low) {
		return ByteProcHolder::create (instance ())->bit_merge (high ,low) ;
	}

	template <class ARG1 ,class ARG2>
	imports BOOL bit_any (CREF<ARG1> base ,CREF<ARG2> mask) {
		require (IS_BYTE<ARG1>) ;
		return ByteProcHolder::create (instance ())->bit_any (base ,ARG1 (mask)) ;
	}

	template <class ARG1 ,class ARG2>
	imports BOOL bit_all (CREF<ARG1> base ,CREF<ARG2> mask) {
		require (IS_BYTE<ARG1>) ;
		return ByteProcHolder::create (instance ())->bit_all (base ,ARG1 (mask)) ;
	}

	imports BYTE bit_reverse (CREF<BYTE> a) {
		return ByteProcHolder::create (instance ())->bit_reverse (a) ;
	}

	imports WORD bit_reverse (CREF<WORD> a) {
		return ByteProcHolder::create (instance ())->bit_reverse (a) ;
	}

	imports CHAR bit_reverse (CREF<CHAR> a) {
		return ByteProcHolder::create (instance ())->bit_reverse (a) ;
	}

	imports QUAD bit_reverse (CREF<QUAD> a) {
		return ByteProcHolder::create (instance ())->bit_reverse (a) ;
	}

	imports INDEX bit_pow (CREF<LENGTH> nth) {
		return ByteProcHolder::create (instance ())->bit_pow (nth) ;
	}

	imports LENGTH popcount (CREF<BYTE> a) {
		return ByteProcHolder::create (instance ())->popcount (a) ;
	}

	imports LENGTH lowcount (CREF<BYTE> a) {
		return ByteProcHolder::create (instance ())->lowcount (a) ;
	}
} ;

struct IntegerLayout {
	RefBuffer<BYTE> mInteger ;
} ;

struct IntegerHolder implement Interface {
	imports VFat<IntegerHolder> create (VREF<IntegerLayout> that) ;
	imports CFat<IntegerHolder> create (CREF<IntegerLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_ ,CREF<VAL64> item) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH precision () const = 0 ;
	virtual void get (VREF<VAL64> item) const = 0 ;
	virtual void set (CREF<VAL64> item) = 0 ;
	virtual BOOL equal (CREF<IntegerLayout> that) const = 0 ;
	virtual FLAG compr (CREF<IntegerLayout> that) const = 0 ;
	virtual void visit (CREF<Visitor> visitor) const = 0 ;
	virtual IntegerLayout add (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout sub (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout mul (CREF<IntegerLayout> that) const = 0 ;
	virtual IntegerLayout mul (CREF<VAL64> scale) const = 0 ;
	virtual IntegerLayout div (CREF<VAL64> scale) const = 0 ;
	virtual IntegerLayout mod (CREF<VAL64> scale) const = 0 ;
	virtual IntegerLayout plus () const = 0 ;
	virtual IntegerLayout minus () const = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class Integer implement IntegerLayout {
protected:
	using IntegerLayout::mInteger ;

public:
	implicit Integer () = default ;

	explicit Integer (CREF<LENGTH> size_ ,CREF<VAL64> item) {
		IntegerHolder::create (thiz)->initialize (size_ ,item) ;
	}

	LENGTH size () const {
		return IntegerHolder::create (thiz)->size () ;
	}

	LENGTH precision () const {
		return IntegerHolder::create (thiz)->precision () ;
	}

	void get (VREF<VAL64> item) const {
		return IntegerHolder::create (thiz)->get (item) ;
	}

	void set (CREF<VAL64> item) {
		return IntegerHolder::create (thiz)->set (item) ;
	}

	BOOL equal (CREF<Integer> that) const {
		return IntegerHolder::create (thiz)->equal (that) ;
	}

	inline BOOL operator== (CREF<Integer> that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (CREF<Integer> that) const {
		return ifnot (equal (that)) ;
	}

	FLAG compr (CREF<Integer> that) const {
		return IntegerHolder::create (thiz)->compr (that) ;
	}

	inline BOOL operator< (CREF<Integer> that) const {
		return compr (that) < ZERO ;
	}

	inline BOOL operator<= (CREF<Integer> that) const {
		return compr (that) <= ZERO ;
	}

	inline BOOL operator> (CREF<Integer> that) const {
		return compr (that) > ZERO ;
	}

	inline BOOL operator>= (CREF<Integer> that) const {
		return compr (that) >= ZERO ;
	}

	void visit (CREF<Visitor> visitor) const {
		return IntegerHolder::create (thiz)->visit (visitor) ;
	}

	Integer add (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::create (thiz)->add (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator+ (CREF<Integer> that) const {
		return add (that) ;
	}

	inline void operator+= (CREF<Integer> that) {
		thiz = add (that) ;
	}

	Integer sub (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::create (thiz)->sub (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator- (CREF<Integer> that) const {
		return sub (that) ;
	}

	inline void operator-= (CREF<Integer> that) {
		thiz = sub (that) ;
	}

	Integer mul (CREF<Integer> that) const {
		IntegerLayout ret = IntegerHolder::create (thiz)->mul (that) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator* (CREF<Integer> that) const {
		return mul (that) ;
	}

	inline void operator*= (CREF<Integer> that) {
		thiz = mul (that) ;
	}

	Integer mul (CREF<VAL64> scale) const {
		IntegerLayout ret = IntegerHolder::create (thiz)->mul (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator* (CREF<VAL64> scale) const {
		return mul (scale) ;
	}

	inline void operator*= (CREF<VAL64> scale) {
		thiz = mul (scale) ;
	}

	Integer div (CREF<VAL64> scale) const {
		IntegerLayout ret = IntegerHolder::create (thiz)->div (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator/ (CREF<VAL64> scale) const {
		return div (scale) ;
	}

	inline void operator/= (CREF<VAL64> scale) {
		thiz = div (scale) ;
	}

	Integer mod (CREF<VAL64> scale) const {
		IntegerLayout ret = IntegerHolder::create (thiz)->mod (scale) ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator% (CREF<VAL64> scale) const {
		return mod (scale) ;
	}

	inline void operator%= (CREF<VAL64> scale) {
		thiz = mod (scale) ;
	}

	Integer plus () const {
		IntegerLayout ret = IntegerHolder::create (thiz)->plus () ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator+ () const {
		return plus () ;
	}

	Integer minus () const {
		IntegerLayout ret = IntegerHolder::create (thiz)->minus () ;
		return move (keep[TYPE<Integer>::expr] (ret)) ;
	}

	inline Integer operator- () const {
		return minus () ;
	}

	void increase () {
		return IntegerHolder::create (thiz)->increase () ;
	}

	inline void operator++ (VAL32) {
		increase () ;
	}

	void decrease () {
		return IntegerHolder::create (thiz)->decrease () ;
	}

	inline void operator-- (VAL32) {
		decrease () ;
	}
} ;
} ;