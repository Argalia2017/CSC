#include "util.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

class Test {
protected:
	FLT64 mMinX ;
	FLT64 mMaxX ;
	Buffer3<FLT64> mT1 ;
	Buffer3<FLT64> mT2 ;
	Array<Vector> mPoint ;
	Array<FLT64> mWeight ;
	FLT64 mMinWeight ;
	FLT64 mMaxWeight ;

public:
	void load () {
		mMinX = 0 ;
		mMaxX = 100 ;
		const auto r1x = CurrentRandom () ;
		mT1[0] = r1x.random_float (4) * 10 ;
		mT1[1] = r1x.random_float (4) * 10 ;
		mT1[2] = r1x.random_float (4) * 10 ;
		mPoint = Array<Vector> (1000) ;
		mWeight = Array<FLT64> (1000) ;
		auto rax = FLT64 (0) ;
		for (auto &&i : mPoint.iter ()) {
			const auto r2x = r1x.random_float (4) * (mMaxX - mMinX) + mMinX ;
			const auto r3x = sample (mT1 ,r2x) ;
			mPoint[i][0] = r3x[0] + r1x.random_normal () ;
			mPoint[i][1] = r3x[1] + r1x.random_normal () ;
			mPoint[i][2] = 0 ;
			mPoint[i][3] = 1 ;
		}
		for (auto &&i : mWeight.iter ()) {
			const auto r4x = MathProc::abs (mPoint[i][0]) ;
			mWeight[i] = r4x ;
			rax += r4x ;
		}
		rax = MathProc::inverse (rax) ;
		mMinWeight = +FLT64_MAX ;
		mMaxWeight = -FLT64_MAX ;
		for (auto &&i : mWeight.iter ()) {
			mWeight[i] *= rax ;
			mMinWeight = MathProc::min_of (mMinWeight ,mWeight[i]) ;
			mMaxWeight = MathProc::max_of (mMaxWeight ,mWeight[i]) ;
		}
	}

	void estimate () {
		const auto r1x = invoke ([&] () {
			Array<FLT64> ret = Array<FLT64> (6) ;
			ret.fill (0) ;
			for (auto &&i : mPoint.iter ()) {
				const auto r2x = mPoint[i] ;
				const auto r3x = mWeight[i] ;
				ret[0] += r3x ;
				ret[1] += r3x * r2x[0] ;
				ret[2] += r3x * r2x[1] ;
				ret[3] += r3x * r2x[0] * r2x[0] ;
				ret[4] += r3x * r2x[0] * r2x[1] ;
				ret[5] += r3x * r2x[1] * r2x[1] ;
			}
			return move (ret) ;
		}) ;
		const auto r4x = r1x[0] * r1x[3] - r1x[1] * r1x[1] ;
		const auto r5x = r1x[0] * r1x[4] - r1x[1] * r1x[2] ;
		const auto r6x = r1x[3] * r1x[2] - r1x[1] * r1x[4] ;
		mT2[0] = r6x ;
		mT2[1] = r5x ;
		mT2[2] = -r4x ;
	}

	Vector sample (CR<Buffer3<FLT64>> a ,CR<FLT64> x) const {
		const auto r1x = a[0] ;
		const auto r2x = a[1] * x ;
		const auto r3x = (r1x + r2x) * -MathProc::inverse (a[2]) ;
		return Vector (x ,r3x ,0 ,1) ;
	}

	Color3B color_from_depth (CR<FLT64> depth ,CR<FLT64> min_ ,CR<FLT64> max_) const {
		Color3B ret ;
		const auto r1x = (depth - min_) * MathProc::inverse (max_ - min_) ;
		const auto r2x = MathProc::lerp (r1x ,ZERO ,LENGTH (255)) ;
		auto act = TRUE ;
		if ifdo (act) {
			const auto r3x = 4 * r2x ;
			if (r3x >= 256)
				discard ;
			ret.mR = BYTE (255) ;
			ret.mG = BYTE (r3x) ;
			ret.mB = BYTE (0) ;
		}
		if ifdo (act) {
			const auto r4x = 4 * (r2x - 64) ;
			if (r4x >= 256)
				discard ;
			ret.mR = BYTE (255 - r4x) ;
			ret.mG = BYTE (255) ;
			ret.mB = BYTE (r4x) ;
		}
		if ifdo (act) {
			const auto r5x = 4 * (r2x - 128) ;
			if (r5x >= 256)
				discard ;
			ret.mR = BYTE (0) ;
			ret.mG = BYTE (255 - r5x) ;
			ret.mB = BYTE (255) ;
		}
		if ifdo (act) {
			const auto r6x = r2x - 192 ;
			ret.mR = BYTE (0) ;
			ret.mG = BYTE (0) ;
			ret.mB = BYTE (255 - r6x) ;
		}
		return move (ret) ;
	}

	void save (CR<String<STR>> file) {
		auto mWriter = StreamFileTextWriter (file) ;
		const auto r1x = GAP ;
		const auto r2x = slice (" ") ;
		mWriter.ref << slice ("ply") << r1x ;
		mWriter.ref << slice ("format ascii 1.0") << r1x ;
		mWriter.ref << slice ("element vertex ") << mPoint.length () + 4 << r1x ;
		mWriter.ref << slice ("property float x") << r1x ;
		mWriter.ref << slice ("property float y") << r1x ;
		mWriter.ref << slice ("property float z") << r1x ;
		mWriter.ref << slice ("property uchar red") << r1x ;
		mWriter.ref << slice ("property uchar green") << r1x ;
		mWriter.ref << slice ("property uchar blue") << r1x ;
		mWriter.ref << slice ("element edge ") << 2 << r1x ;
		mWriter.ref << slice ("property int vertex1") << r1x ;
		mWriter.ref << slice ("property int vertex2") << r1x ;
		mWriter.ref << slice ("property uchar red") << r1x ;
		mWriter.ref << slice ("property uchar green") << r1x ;
		mWriter.ref << slice ("property uchar blue") << r1x ;
		mWriter.ref << slice ("end_header") << r1x ;
		for (auto &&i : mPoint.iter ()) {
			const auto r3x = mPoint[i] ;
			const auto r4x = color_from_depth (mWeight[i] ,mMinWeight ,mMaxWeight) ;
			mWriter.ref << r3x[0] << r2x ;
			mWriter.ref << r3x[1] << r2x ;
			mWriter.ref << r3x[2] << r1x ;
			mWriter.ref << VAL32 (r4x.mR) << r2x ;
			mWriter.ref << VAL32 (r4x.mG) << r2x ;
			mWriter.ref << VAL32 (r4x.mB) << r1x ;
		}
		const auto r5x = sample (mT1 ,mMinX) ;
		const auto r6x = sample (mT1 ,mMaxX) ;
		const auto r7x = sample (mT2 ,mMinX) ;
		const auto r8x = sample (mT2 ,mMaxX) ;
		const auto r9x = Array<Vector> ({r5x ,r6x ,r8x ,r7x}) ;
		for (auto &&i : r9x) {
			mWriter.ref << i[0] << r2x ;
			mWriter.ref << i[1] << r2x ;
			mWriter.ref << i[2] << r1x ;
			mWriter.ref << VAL32 (128) << r2x ;
			mWriter.ref << VAL32 (128) << r2x ;
			mWriter.ref << VAL32 (128) << r1x ;
		}
		if ifdo (TRUE) {
			INDEX ix = mPoint.length () ;
			mWriter.ref << ix + 0 << r2x ;
			mWriter.ref << ix + 1 << r1x ;
			mWriter.ref << VAL32 (0) << r2x ;
			mWriter.ref << VAL32 (128) << r2x ;
			mWriter.ref << VAL32 (0) << r1x ;
			mWriter.ref << ix + 2 << r2x ;
			mWriter.ref << ix + 3 << r1x ;
			mWriter.ref << VAL32 (128) << r2x ;
			mWriter.ref << VAL32 (0) << r2x ;
			mWriter.ref << VAL32 (0) << r1x ;
		}
		mWriter.flush () ;
	}
} ;

int main () {
	auto rax = Test () ;
	rax.load () ;
	rax.estimate () ;
	rax.save (slice ("1.ply")) ;
	notice (rax) ;

	return 0 ;
}