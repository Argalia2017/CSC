#pragma once

#ifndef __CSC_MATH__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_math.hpp"

#include "csc_end.h"
#include <cmath>
#include "csc_begin.h"

namespace CSC {
struct MathProcPureLayout {
	FLT64 mE ;
	FLT64 mPi ;
	FLT64 mInvSqrt2Pi ;
	FLT64 mSqrt2 ;
} ;

class MathProcImplement implement Fat<MathProcHolder ,MathProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<MathProcPureLayout>::make () ;
		fake.mThis->mE = 2.71828182845904523536 ;
		fake.mThis->mPi = 3.14159265358979323846 ;
		fake.mThis->mInvSqrt2Pi = 0.39894228040143267794 ;
		fake.mThis->mSqrt2 = 1.41421356237309504880 ;
	}

	BOOL is_inf (CREF<FLT32> a) const override {
		return std::isinf (a) ;
	}

	BOOL is_inf (CREF<FLT64> a) const override {
		return std::isinf (a) ;
	}

	VAL32 step (CREF<VAL32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	VAL64 step (CREF<VAL64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT32 step (CREF<FLT32> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT64 step (CREF<FLT64> a) const override {
		if (a > 0)
			return 1 ;
		return 0 ;
	}

	FLT32 sign (CREF<FLT32> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	FLT64 sign (CREF<FLT64> a) const override {
		if (a >= 0)
			return 1 ;
		return -1 ;
	}

	VAL32 square (CREF<VAL32> a) const override {
		return a * a ;
	}

	VAL64 square (CREF<VAL64> a) const override {
		return a * a ;
	}

	FLT32 square (CREF<FLT32> a) const override {
		return a * a ;
	}

	FLT64 square (CREF<FLT64> a) const override {
		return a * a ;
	}

	FLT32 sqrt (CREF<FLT32> a) const override {
		return std::sqrt (a) ;
	}

	FLT64 sqrt (CREF<FLT64> a) const override {
		return std::sqrt (a) ;
	}

	VAL32 cubic (CREF<VAL32> a) const override {
		return a * a * a ;
	}

	VAL64 cubic (CREF<VAL64> a) const override {
		return a * a * a ;
	}

	FLT32 cubic (CREF<FLT32> a) const override {
		return a * a * a ;
	}

	FLT64 cubic (CREF<FLT64> a) const override {
		return a * a * a ;
	}

	FLT32 cbrt (CREF<FLT32> a) const override {
		return std::cbrt (a) ;
	}

	FLT64 cbrt (CREF<FLT64> a) const override {
		return std::cbrt (a) ;
	}

	VAL32 abs (CREF<VAL32> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	VAL64 abs (CREF<VAL64> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT32 abs (CREF<FLT32> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT64 abs (CREF<FLT64> a) const override {
		if (a < 0)
			return -a ;
		return a ;
	}

	FLT32 inverse (CREF<FLT32> a) const override {
		if (abs (a) < FLT32_EPS)
			return 0 ;
		return 1 / a ;
	}

	FLT64 inverse (CREF<FLT64> a) const override {
		if (abs (a) < FLT64_EPS)
			return 0 ;
		return 1 / a ;
	}

	FLT32 floor (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	FLT64 floor (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::floor (a * inverse (b)) * b ;
	}

	FLT32 round (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::round (a * inverse (b)) * b ;
	}

	FLT64 round (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::round (a * inverse (b)) * b ;
	}

	FLT32 ceil (CREF<FLT32> a ,CREF<FLT32> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	FLT64 ceil (CREF<FLT64> a ,CREF<FLT64> b) const override {
		return std::ceil (a * inverse (b)) * b ;
	}

	VAL32 clamp (CREF<VAL32> a ,CREF<VAL32> lb ,CREF<VAL32> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	VAL64 clamp (CREF<VAL64> a ,CREF<VAL64> lb ,CREF<VAL64> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT32 clamp (CREF<FLT32> a ,CREF<FLT32> lb ,CREF<FLT32> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT64 clamp (CREF<FLT64> a ,CREF<FLT64> lb ,CREF<FLT64> rb) const override {
		if (a <= lb)
			return lb ;
		if (a >= rb)
			return rb ;
		return a ;
	}

	FLT32 cos (CREF<FLT32> a) const override {
		return std::cos (a) ;
	}

	FLT64 cos (CREF<FLT64> a) const override {
		return std::cos (a) ;
	}

	FLT32 sin (CREF<FLT32> a) const override {
		return std::sin (a) ;
	}

	FLT64 sin (CREF<FLT64> a) const override {
		return std::sin (a) ;
	}

	FLT32 tan (CREF<FLT32> a) const override {
		return std::tan (a) ;
	}

	FLT64 tan (CREF<FLT64> a) const override {
		return std::tan (a) ;
	}

	FLT32 arccos (CREF<FLT32> a) const override {
		return std::acos (a) ;
	}

	FLT64 arccos (CREF<FLT64> a) const override {
		return std::acos (a) ;
	}

	FLT32 arcsin (CREF<FLT32> a) const override {
		return std::asin (a) ;
	}

	FLT64 arcsin (CREF<FLT64> a) const override {
		return std::asin (a) ;
	}

	FLT32 arctan (CREF<FLT32> y ,CREF<FLT32> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT64 arctan (CREF<FLT64> y ,CREF<FLT64> x) const override {
		return std::atan2 (y ,x) ;
	}

	FLT32 exp (CREF<FLT32> a) const override {
		return std::exp (a) ;
	}

	FLT64 exp (CREF<FLT64> a) const override {
		return std::exp (a) ;
	}

	FLT32 log (CREF<FLT32> a) const override {
		return std::log (a) ;
	}

	FLT64 log (CREF<FLT64> a) const override {
		return std::log (a) ;
	}

	FLT32 pdf (CREF<FLT32> a) const override {
		const auto r1x = -square (a) * FLT32 (0.5) ;
		return exp (r1x) * FLT32 (fake.mThis->mInvSqrt2Pi) ;
	}

	FLT64 pdf (CREF<FLT64> a) const override {
		const auto r1x = -square (a) * FLT64 (0.5) ;
		return exp (r1x) * FLT64 (fake.mThis->mInvSqrt2Pi) ;
	}

	FLT32 cbf (CREF<FLT32> a) const override {
		const auto r1x = a * FLT32 (inverse (fake.mThis->mSqrt2)) ;
		return (1 + std::erf (r1x)) * FLT32 (0.5) ;
	}

	FLT64 cbf (CREF<FLT64> a) const override {
		const auto r1x = a * FLT64 (inverse (fake.mThis->mSqrt2)) ;
		return (1 + std::erf (r1x)) * FLT64 (0.5) ;
	}

	BOOL all_of (CREF<BOOL> a ,CREF<CaptureLayout> b) const override {
		if ifnot (a)
			return FALSE ;
		for (auto &&i : CaptureIterator<BOOL> (b)) {
			if ifnot (i)
				return FALSE ;
		}
		return TRUE ;
	}

	BOOL any_of (CREF<BOOL> a ,CREF<CaptureLayout> b) const override {
		if (a)
			return TRUE ;
		for (auto &&i : CaptureIterator<BOOL> (b)) {
			if (i)
				return TRUE ;
		}
		return FALSE ;
	}

	VAL32 max_of (CREF<VAL32> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	VAL64 max_of (CREF<VAL64> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	FLT32 max_of (CREF<FLT32> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	FLT64 max_of (CREF<FLT64> a ,CREF<CaptureLayout> b) const override {
		return max_of_impl (a ,b) ;
	}

	template <class ARG1>
	ARG1 max_of_impl (CREF<ARG1> a ,CREF<CaptureLayout> b) const {
		ARG1 ret = a ;
		for (auto &&i : CaptureIterator<ARG1> (b)) {
			if (ret >= i)
				continue ;
			ret = i ;
		}
		return move (ret) ;
	}

	VAL32 min_of (CREF<VAL32> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	VAL64 min_of (CREF<VAL64> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	FLT32 min_of (CREF<FLT32> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	FLT64 min_of (CREF<FLT64> a ,CREF<CaptureLayout> b) const override {
		return min_of_impl (a ,b) ;
	}

	template <class ARG1>
	ARG1 min_of_impl (CREF<ARG1> a ,CREF<CaptureLayout> b) const {
		ARG1 ret = a ;
		for (auto &&i : CaptureIterator<ARG1> (b)) {
			if (ret <= i)
				continue ;
			ret = i ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MathProcHolder> MathProcHolder::create (VREF<MathProcLayout> that) {
	return VFat<MathProcHolder> (MathProcImplement () ,that) ;
}

exports CFat<MathProcHolder> MathProcHolder::create (CREF<MathProcLayout> that) {
	return CFat<MathProcHolder> (MathProcImplement () ,that) ;
}

class FloatProcImplement implement Fat<FloatProcHolder ,FloatProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	FLT64 encode (CREF<Notation> fexp2) const override {
		assert (fexp2.mRadix == 2) ;
		auto rax = fexp2 ;
		if ifdo (TRUE) {
			if (rax.mMantissa == 0)
				discard ;
			while (TRUE) {
				if ifnot (ByteProc::bit_any (QUAD (rax.mMantissa) ,QUAD (0XFFE0000000000000)))
					break ;
				rax.mMantissa = VAL64 (QUAD (rax.mMantissa) >> 1) ;
				rax.mExponent++ ;
			}
			while (TRUE) {
				if (ByteProc::bit_any (QUAD (rax.mMantissa) ,QUAD (0XFFF0000000000000)))
					break ;
				rax.mMantissa = VAL64 (QUAD (rax.mMantissa) << 1) ;
				rax.mExponent-- ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = VAL64 (-1074) - rax.mExponent ;
			if (r1x <= 0)
				discard ;
			rax.mMantissa = VAL64 (QUAD (rax.mMantissa) >> r1x) ;
			rax.mExponent = -1075 ;
		}
		rax.mExponent += 1075 ;
		if ifdo (TRUE) {
			if (rax.mMantissa != 0)
				discard ;
			rax.mExponent = 0 ;
		}
		const auto r2x = invoke ([&] () {
			if ifnot (fexp2.mSign)
				return QUAD (0X00) ;
			return QUAD (0X8000000000000000) ;
		}) ;
		const auto r3x = (QUAD (rax.mExponent) << 52) & QUAD (0X7FF0000000000000) ;
		const auto r4x = QUAD (rax.mMantissa) & QUAD (0X000FFFFFFFFFFFFF) ;
		const auto r5x = r2x | r3x | r4x ;
		return bitwise[TYPE<FLT64>::expr] (r5x) ;
	}

	Notation decode (CREF<FLT64> float_) const override {
		Notation ret ;
		ret.mRadix = 2 ;
		const auto r1x = bitwise[TYPE<QUAD>::expr] (float_) ;
		const auto r2x = r1x & QUAD (0X7FF0000000000000) ;
		const auto r3x = r1x & QUAD (0X000FFFFFFFFFFFFF) ;
		ret.mSign = ByteProc::bit_any (r1x ,QUAD (0X8000000000000000)) ;
		ret.mMantissa = VAL64 (r3x) ;
		ret.mPrecision = 0 ;
		if ifdo (TRUE) {
			if (r2x == QUAD (0X00))
				discard ;
			ret.mMantissa = VAL64 (QUAD (ret.mMantissa) | QUAD (0X0010000000000000)) ;
		}
		ret.mExponent = VAL64 (r2x >> 52) ;
		ret.mExponent -= 1075 - LENGTH (r2x == QUAD (0X00)) ;
		if ifdo (TRUE) {
			if (ret.mMantissa != 0)
				discard ;
			ret.mExponent = 0 ;
		}
		if ifdo (TRUE) {
			if (ret.mMantissa == 0)
				discard ;
			while (TRUE) {
				if (ByteProc::bit_any (QUAD (ret.mMantissa) ,QUAD (0X0000000000000001)))
					break ;
				ret.mMantissa = VAL64 (QUAD (ret.mMantissa) >> 1) ;
				ret.mExponent++ ;
			}
		}
		return move (ret) ;
	}

	Notation fexp2_multiply (CREF<Notation> obj1 ,CREF<Notation> obj2) const {
		assert (obj1.mRadix == 2) ;
		assert (obj2.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
		const auto r1x = LENGTH (32) ;
		const auto r2x = QUAD (ByteProc::bit_pow (r1x) - 1) ;
		const auto r3x = ByteProc::bit_pow (r1x - 1) ;
		const auto r4x = VAL64 (QUAD (obj1.mMantissa) >> r1x) ;
		const auto r5x = VAL64 (QUAD (obj1.mMantissa) & r2x) ;
		const auto r6x = VAL64 (QUAD (obj2.mMantissa) >> r1x) ;
		const auto r7x = VAL64 (QUAD (obj2.mMantissa) & r2x) ;
		const auto r8x = r4x * r6x ;
		const auto r9x = r5x * r6x ;
		const auto r10x = r4x * r7x ;
		const auto r11x = r5x * r7x ;
		//@warn: lose a part of precision
		auto act = TRUE ;
		if ifdo (act) {
			if (r8x == 0)
				discard ;
			const auto r12x = VAL64 (QUAD (r9x) >> r1x) ;
			const auto r13x = VAL64 (QUAD (r9x) & r2x) ;
			const auto r14x = VAL64 (QUAD (r10x) >> r1x) ;
			const auto r15x = VAL64 (QUAD (r10x) & r2x) ;
			const auto r16x = VAL64 (QUAD (r11x) >> r1x) ;
			const auto r17x = VAL64 (QUAD (r13x + r15x + r16x + r3x) >> r1x) ;
			ret.mMantissa = r8x + r12x + r14x + r17x ;
			ret.mPrecision = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent + r1x * 2 ;
		}
		if ifdo (act) {
			if (r9x == 0)
				if (r10x == 0)
					discard ;
			const auto r18x = VAL64 (QUAD (r11x + r3x) >> r1x) ;
			ret.mMantissa = r9x + r10x + r18x ;
			ret.mPrecision = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent + r1x ;
		}
		if ifdo (act) {
			ret.mMantissa = r11x ;
			ret.mPrecision = 0 ;
			ret.mExponent = obj1.mExponent + obj2.mExponent ;
		}
		return move (ret) ;
	}

	Notation fexp2_from_fexp10 (CREF<Notation> fexp10) const override {
		assert (fexp10.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = 2 ;
		ret.mSign = fexp10.mSign ;
		ret.mMantissa = fexp10.mMantissa ;
		ret.mPrecision = 0 ;
		ret.mExponent = 0 ;
		const auto r1x = FEXP2Cache::instance ()[fexp10.mExponent] ;
		ret = fexp2_multiply (ret ,r1x) ;
		return move (ret) ;
	}

	Notation fexp10_overflow (CREF<Notation> fexp10 ,CREF<VAL64> half) const {
		Notation ret = fexp10 ;
		if ifdo (TRUE) {
			if (ret.mMantissa >= 0)
				discard ;
			ret.mMantissa = VAL64 (QUAD (ret.mMantissa) >> 1) ;
			ret.mMantissa /= 5 ;
			ret.mExponent++ ;
		}
		const auto r1x = MathProc::square (half) ;
		while (TRUE) {
			if (ret.mMantissa < r1x)
				break ;
			ret.mMantissa /= 10 ;
			ret.mExponent++ ;
		}
		return move (ret) ;
	}

	Notation fexp10_multiply (CREF<Notation> obj1 ,CREF<Notation> obj2) const {
		assert (obj1.mRadix == 10) ;
		assert (obj2.mRadix == 10) ;
		Notation ret ;
		ret.mRadix = 10 ;
		ret.mSign = MathProc::any_of (obj1.mSign ,obj2.mSign) ;
		const auto r1x = LENGTH (9) ;
		const auto r2x = VAL64 (1000000000) ;
		const auto r3x = VAL64 (500000000) ;
		const auto r4x = fexp10_overflow (obj1 ,r2x) ;
		const auto r5x = fexp10_overflow (obj2 ,r2x) ;
		const auto r6x = r4x.mMantissa / r2x ;
		const auto r7x = r4x.mMantissa % r2x ;
		const auto r8x = r5x.mMantissa / r2x ;
		const auto r9x = r5x.mMantissa % r2x ;
		const auto r10x = r6x * r8x ;
		const auto r11x = r7x * r8x ;
		const auto r12x = r6x * r9x ;
		const auto r13x = r7x * r9x ;
		//@warn: lose a part of precision
		auto act = TRUE ;
		if ifdo (act) {
			if (r10x == 0)
				discard ;
			const auto r14x = r11x / r2x ;
			const auto r15x = r11x % r2x ;
			const auto r16x = r12x / r2x ;
			const auto r17x = r12x % r2x ;
			const auto r18x = r13x / r2x ;
			const auto r19x = (r15x + r17x + r18x + r3x) / r2x ;
			ret.mMantissa = r10x + r14x + r16x + r19x ;
			ret.mPrecision = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent + r1x * 2 ;
		}
		if ifdo (act) {
			if (r11x == 0)
				if (r12x == 0)
					discard ;
			const auto r20x = (r13x + r3x) / r2x ;
			ret.mMantissa = r11x + r12x + r20x ;
			ret.mPrecision = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent + r1x ;
		}
		if ifdo (act) {
			ret.mMantissa = r13x ;
			ret.mPrecision = 0 ;
			ret.mExponent = r4x.mExponent + r5x.mExponent ;
		}
		return move (ret) ;
	}

	Notation fexp10_from_fexp2 (CREF<Notation> fexp2) const override {
		assert (fexp2.mRadix == 2) ;
		Notation ret ;
		ret.mRadix = 10 ;
		ret.mSign = fexp2.mSign ;
		ret.mMantissa = fexp2.mMantissa ;
		ret.mPrecision = 0 ;
		ret.mExponent = 0 ;
		const auto r1x = FEXP10Cache::instance ()[fexp2.mExponent] ;
		ret = fexp10_multiply (ret ,r1x) ;
		return move (ret) ;
	}
} ;

exports VFat<FloatProcHolder> FloatProcHolder::create (VREF<FloatProcLayout> that) {
	return VFat<FloatProcHolder> (FloatProcImplement () ,that) ;
}

exports CFat<FloatProcHolder> FloatProcHolder::create (CREF<FloatProcLayout> that) {
	return CFat<FloatProcHolder> (FloatProcImplement () ,that) ;
}

class ByteProcImplement implement Fat<ByteProcHolder ,ByteProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	BYTE bit_low (CREF<WORD> a) const override {
		return BYTE (a) ;
	}

	WORD bit_low (CREF<CHAR> a) const override {
		return WORD (a) ;
	}

	CHAR bit_low (CREF<QUAD> a) const override {
		return CHAR (a) ;
	}

	BYTE bit_high (CREF<WORD> a) const override {
		return BYTE (a >> 8) ;
	}

	WORD bit_high (CREF<CHAR> a) const override {
		return WORD (a >> 16) ;
	}

	CHAR bit_high (CREF<QUAD> a) const override {
		return CHAR (a >> 32) ;
	}

	WORD bit_merge (CREF<BYTE> high ,CREF<BYTE> low) const override {
		return (WORD (high) << 8) | WORD (low) ;
	}

	CHAR bit_merge (CREF<WORD> high ,CREF<WORD> low) const override {
		return (CHAR (high) << 16) | CHAR (low) ;
	}

	QUAD bit_merge (CREF<CHAR> high ,CREF<CHAR> low) const override {
		return (QUAD (high) << 32) | QUAD (low) ;
	}

	BOOL bit_any (CREF<BYTE> base ,CREF<BYTE> mask) const override {
		return (base & mask) != BYTE (0X00) ;
	}

	BOOL bit_any (CREF<WORD> base ,CREF<WORD> mask) const override {
		return (base & mask) != WORD (0X00) ;
	}

	BOOL bit_any (CREF<CHAR> base ,CREF<CHAR> mask) const override {
		return (base & mask) != CHAR (0X00) ;
	}

	BOOL bit_any (CREF<QUAD> base ,CREF<QUAD> mask) const override {
		return (base & mask) != QUAD (0X00) ;
	}

	BOOL bit_all (CREF<BYTE> base ,CREF<BYTE> mask) const override {
		return (base & mask) == mask ;
	}

	BOOL bit_all (CREF<WORD> base ,CREF<WORD> mask) const override {
		return (base & mask) == mask ;
	}

	BOOL bit_all (CREF<CHAR> base ,CREF<CHAR> mask) const override {
		return (base & mask) == mask ;
	}

	BOOL bit_all (CREF<QUAD> base ,CREF<QUAD> mask) const override {
		return (base & mask) == mask ;
	}

	BYTE bit_reverse (CREF<BYTE> a) const override {
		return a ;
	}

	WORD bit_reverse (CREF<WORD> a) const override {
		auto rax = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (a) ;
		swap (rax[0] ,rax[1]) ;
		return bitwise[TYPE<WORD>::expr] (rax) ;
	}

	CHAR bit_reverse (CREF<CHAR> a) const override {
		auto rax = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<CHAR>>>::expr] (a) ;
		swap (rax[0] ,rax[3]) ;
		swap (rax[1] ,rax[2]) ;
		return bitwise[TYPE<CHAR>::expr] (rax) ;
	}

	QUAD bit_reverse (CREF<QUAD> a) const override {
		auto rax = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<QUAD>>>::expr] (a) ;
		swap (rax[0] ,rax[7]) ;
		swap (rax[1] ,rax[6]) ;
		swap (rax[2] ,rax[5]) ;
		swap (rax[3] ,rax[4]) ;
		return bitwise[TYPE<QUAD>::expr] (rax) ;
	}

	INDEX bit_pow (CREF<LENGTH> nth) const override {
		return INDEX (QUAD (0X01) << nth) ;
	}

	LENGTH popcount (CREF<BYTE> a) const override {
		static const DEF<LENGTH[256]> mCache {
			0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8} ;
		return mCache[INDEX (a)] ;
	}

	LENGTH lowcount (CREF<BYTE> a) const override {
		static const DEF<LENGTH[256]> mCache {
			8 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			7 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
			4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0} ;
		return mCache[INDEX (a)] ;
	}
} ;

exports VFat<ByteProcHolder> ByteProcHolder::create (VREF<ByteProcLayout> that) {
	return VFat<ByteProcHolder> (ByteProcImplement () ,that) ;
}

exports CFat<ByteProcHolder> ByteProcHolder::create (CREF<ByteProcLayout> that) {
	return CFat<ByteProcHolder> (ByteProcImplement () ,that) ;
}

class IntegerImplement implement Fat<IntegerHolder ,IntegerLayout> {
public:
	void initialize (CREF<LENGTH> size_ ,CREF<VAL64> item) override {
		fake.mInteger = RefBuffer<BYTE> (size_) ;
		set (item) ;
	}

	LENGTH size () const override {
		return fake.mInteger.size () ;
	}

	LENGTH precision () const override {
		const auto r1x = find_highest () ;
		if (r1x == NONE)
			return 0 ;
		const auto r2x = ByteProc::lowcount (fake.mInteger[r1x]) + 1 ;
		const auto r3x = r1x * 8 + r2x ;
		return r3x ;
	}

	INDEX find_highest () const {
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			INDEX ix = fake.mInteger.size () - 1 - i ;
			if (fake.mInteger[ix] != BYTE (0X00))
				return ix ;
		}
		return NONE ;
	}

	void get (VREF<VAL64> item) const override {
		item = 0 ;
		const auto r1x = MathProc::min_of (fake.mInteger.size () ,SIZE_OF<VAL64>::expr) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = QUAD (fake.mInteger[i]) << (i * 8) ;
			item = VAL64 (QUAD (item) | r2x) ;
		}
	}

	void set (CREF<VAL64> item) override {
		const auto r1x = MathProc::min_of (fake.mInteger.size () ,SIZE_OF<VAL64>::expr) ;
		const auto r2x = QUAD (item) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = BYTE (r2x >> (i * 8)) ;
			fake.mInteger[i] = r3x ;
		}
		const auto r4x = invoke ([&] () {
			if (item >= 0)
				return BYTE (0X00) ;
			return BYTE (0XFF) ;
		}) ;
		for (auto &&i : iter (r1x ,fake.mInteger.size ()))
			fake.mInteger[i] = r4x ;
	}

	BOOL equal (CREF<IntegerLayout> that) const override {
		const auto r1x = fake.mInteger.size () ;
		const auto r2x = that.mInteger.size () ;
		if (r1x != r2x)
			return FALSE ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = inline_equal (fake.mInteger[i] ,that.mInteger[i]) ;
			if ifnot (r3x)
				return r3x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<IntegerLayout> that) const override {
		const auto r1x = fake.mInteger.size () ;
		const auto r2x = that.mInteger.size () ;
		const auto r3x = inline_compr (r1x ,r2x) ;
		if (r3x != ZERO)
			return r3x ;
		if ifdo (TRUE) {
			if (r1x == 0)
				discard ;
			const auto r4x = fake.mInteger[0] & BYTE (0X80) ;
			const auto r5x = that.mInteger[0] & BYTE (0X80) ;
			const auto r6x = inline_compr (r4x ,r5x) ;
			if (r6x == ZERO)
				discard ;
			return -r6x ;
		}
		for (auto &&i : iter (0 ,r1x)) {
			const auto r7x = inline_compr (fake.mInteger[i] ,that.mInteger[i]) ;
			if (r7x != ZERO)
				return r7x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		const auto r1x = fake.mInteger.size () ;
		for (auto &&i : iter (0 ,r1x)) {
			visitor.push (fake.mInteger[i]) ;
		}
		visitor.end () ;
	}

	IntegerLayout add (CREF<IntegerLayout> that) const override {
		assert (fake.mInteger.size () == that.mInteger.size ()) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			const auto r1x = VAL64 (fake.mInteger[i]) + VAL64 (that.mInteger[i]) + rax ;
			const auto r2x = QUAD (r1x) ;
			ret.mInteger[i] = BYTE (r2x) ;
			rax = VAL64 (r2x >> 8) ;
		}
		return move (ret) ;
	}

	IntegerLayout sub (CREF<IntegerLayout> that) const override {
		assert (fake.mInteger.size () == that.mInteger.size ()) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			const auto r1x = VAL64 (fake.mInteger[i]) - VAL64 (that.mInteger[i]) - rax ;
			const auto r2x = VAL64 (r1x < 0) ;
			const auto r3x = r1x + 256 * r2x ;
			const auto r4x = QUAD (r3x) ;
			ret.mInteger[i] = BYTE (r4x) ;
			rax = r2x ;
		}
		return move (ret) ;
	}

	IntegerLayout mul (CREF<IntegerLayout> that) const override {
		assert (fake.mInteger.size () == that.mInteger.size ()) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ())) {
			for (auto &&j : iter (0 ,i + 1)) {
				const auto r1x = VAL64 (fake.mInteger[j]) * VAL64 (that.mInteger[i - j]) ;
				rax += r1x ;
			}
			const auto r2x = QUAD (rax) ;
			ret.mInteger[i] = BYTE (r2x) ;
			rax = VAL64 (r2x >> 8) ;
		}
		return move (ret) ;
	}

	IntegerLayout mul (CREF<VAL64> scale) const override {
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		const auto r1x = find_highest () + 1 ;
		for (auto &&i : iter (r1x ,fake.mInteger.size ()))
			ret.mInteger[i] = BYTE (0X00) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = VAL64 (fake.mInteger[i]) * scale + rax ;
			const auto r3x = QUAD (r2x) ;
			ret.mInteger[i] = BYTE (r3x) ;
			rax = VAL64 (r3x >> 8) ;
		}
		INDEX ix = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			const auto r4x = QUAD (rax) ;
			ret.mInteger[ix] = BYTE (r4x) ;
			rax = VAL64 (r4x >> 8) ;
			ix++ ;
		}
		return move (ret) ;
	}

	IntegerLayout div (CREF<VAL64> scale) const override {
		assert (scale != ZERO) ;
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		auto rax = VAL64 (0) ;
		const auto r1x = find_highest () + 1 ;
		for (auto &&i : iter (r1x ,fake.mInteger.size ()))
			ret.mInteger[i] = BYTE (0X00) ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			const auto r2x = VAL64 (fake.mInteger[ix]) + rax ;
			const auto r3x = r2x / scale ;
			const auto r4x = QUAD (r3x) ;
			ret.mInteger[ix] = BYTE (r4x) ;
			const auto r5x = r2x - r3x * scale ;
			rax = VAL64 (QUAD (r5x) << 8) ;
		}
		return move (ret) ;
	}

	IntegerLayout mod (CREF<VAL64> scale) const override {
		assert (scale != ZERO) ;
		auto rax = VAL64 (0) ;
		const auto r1x = find_highest () + 1 ;
		for (auto &&i : iter (0 ,r1x)) {
			INDEX ix = r1x - 1 - i ;
			const auto r2x = VAL64 (fake.mInteger[ix]) + rax ;
			const auto r3x = r2x / scale ;
			const auto r4x = r2x - r3x * scale ;
			rax = VAL64 (QUAD (r4x) << 8) ;
		}
		return Integer (fake.mInteger.size () ,rax) ;
	}

	IntegerLayout plus () const override {
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ()))
			ret.mInteger[i] = fake.mInteger[i] ;
		return move (ret) ;
	}

	IntegerLayout minus () const override {
		IntegerLayout ret ;
		ret.mInteger = RefBuffer<BYTE> (fake.mInteger.size ()) ;
		for (auto &&i : iter (0 ,fake.mInteger.size ()))
			ret.mInteger[i] = ~fake.mInteger[i] ;
		IntegerHolder::create (ret)->increase () ;
		return move (ret) ;
	}

	void increase () override {
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= fake.mInteger.size ())
				break ;
			fake.mInteger[ix] = BYTE (LENGTH (fake.mInteger[ix]) + 1) ;
			if (fake.mInteger[ix] != BYTE (0X00))
				break ;
			ix++ ;
		}
	}

	void decrease () override {
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= fake.mInteger.size ())
				break ;
			fake.mInteger[ix] = BYTE (LENGTH (fake.mInteger[ix]) - 1) ;
			if (fake.mInteger[ix] != BYTE (0XFF))
				break ;
			ix++ ;
		}
	}
} ;

exports VFat<IntegerHolder> IntegerHolder::create (VREF<IntegerLayout> that) {
	return VFat<IntegerHolder> (IntegerImplement () ,that) ;
}

exports CFat<IntegerHolder> IntegerHolder::create (CREF<IntegerLayout> that) {
	return CFat<IntegerHolder> (IntegerImplement () ,that) ;
}
} ;