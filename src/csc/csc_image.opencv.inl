#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_end.h"
#ifdef __CSC_VER_DEBUG__
#define CV_IGNORE_DEBUG_BUILD_GUARD
#endif

#include <opencv2/opencv.hpp>
#include "csc_begin.h"

namespace CSC {
struct ImageProcPureLayout {} ;

class ImageProcImplement implement Fat<ImageProcHolder ,ImageProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<ImageProcPureLayout>::make () ;
	}

	ImageLayout load (CREF<String<STR>> file) const override {
		ImageLayout ret ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		auto rax = Box<cv::Mat>::make (cv::imread (r1x.raw () ,cv::IMREAD_COLOR)) ;
		assume (ifnot (rax->empty ())) ;
		const auto r2x = rax->size () ;
		const auto r3x = Clazz (TYPE<Color3B>::expr) ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (ret.mImage) ;
		tmp.mBuffer = FLAG (rax->data) ;
		tmp.mSize = r2x.area () ;
		tmp.mStep = r3x.type_size () ;
		ret.mWidth.mCX = r2x.width ;
		ret.mWidth.mCY = r2x.height ;
		ret.mWidth.mClazz = r3x ;
		RefBufferHolder::create (ret.mImage)->initialize (move (rax)) ;
		ImageHolder::create (ret)->reset () ;
		return move (ret) ;
	}

	void save (CREF<String<STR>> file ,CREF<ImageLayout> image) const override {
		assert (image.mWidth.mClazz == Clazz (TYPE<Color3B>::expr)) ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		auto &&tmp = keep[TYPE<cv::Mat>::expr] (RefBufferHolder::create (image.mImage)->raw ()) ;
		cv::imwrite (r1x.raw () ,tmp) ;
	}

	Color3B sampler (TYPE<Color3B> ,CREF<ImageLayout> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		Color3B ret ;
		auto &&tmp = keep[TYPE<Image<Color3B>>::expr] (image) ;
		const auto r1x = VAL (MathProc::round (x ,FLT64 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT64 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.mCX - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.mCY - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT64 (r3x) + FLT64 (0.5) - x ;
		const auto r8x = FLT64 (r4x) + FLT64 (0.5) - y ;
		const auto r9x = 1 - r7x ;
		const auto r10x = 1 - r8x ;
		const auto r11x = Color (tmp.at (r5x ,r6x)) ;
		const auto r12x = Color (tmp.at (r5x ,r4x)) ;
		const auto r13x = Color (tmp.at (r3x ,r4x)) ;
		const auto r14x = Color (tmp.at (r3x ,r6x)) ;
		if ifdo (TRUE) {
			auto rax = Color (0) ;
			rax += r11x * (r7x * r8x) ;
			rax += r12x * (r7x * r10x) ;
			rax += r13x * (r9x * r10x) ;
			rax += r14x * (r9x * r8x) ;
			rax.clamp (0 ,255) ;
			ret.mB = BYTE (VAL32 (rax[0])) ;
			ret.mG = BYTE (VAL32 (rax[1])) ;
			ret.mR = BYTE (VAL32 (rax[2])) ;
		}
		return move (ret) ;
	}

	Color4B sampler (TYPE<Color4B> ,CREF<ImageLayout> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		Color4B ret ;
		auto &&tmp = keep[TYPE<Image<Color4B>>::expr] (image) ;
		const auto r1x = VAL (MathProc::round (x ,FLT64 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT64 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.mCX - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.mCY - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT64 (r3x) + FLT64 (0.5) - x ;
		const auto r8x = FLT64 (r4x) + FLT64 (0.5) - y ;
		const auto r9x = 1 - r7x ;
		const auto r10x = 1 - r8x ;
		const auto r11x = Color (tmp.at (r5x ,r6x)) ;
		const auto r12x = Color (tmp.at (r5x ,r4x)) ;
		const auto r13x = Color (tmp.at (r3x ,r4x)) ;
		const auto r14x = Color (tmp.at (r3x ,r6x)) ;
		if ifdo (TRUE) {
			auto rax = Color (0) ;
			rax += r11x * (r7x * r8x) ;
			rax += r12x * (r7x * r10x) ;
			rax += r13x * (r9x * r10x) ;
			rax += r14x * (r9x * r8x) ;
			rax.clamp (0 ,255) ;
			ret.mB = BYTE (VAL32 (rax[0])) ;
			ret.mG = BYTE (VAL32 (rax[1])) ;
			ret.mR = BYTE (VAL32 (rax[2])) ;
			ret.mA = BYTE (VAL32 (rax[3])) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<ImageProcHolder> ImageProcHolder::create (VREF<ImageProcLayout> that) {
	return VFat<ImageProcHolder> (ImageProcImplement () ,that) ;
}

exports CFat<ImageProcHolder> ImageProcHolder::create (CREF<ImageProcLayout> that) {
	return CFat<ImageProcHolder> (ImageProcImplement () ,that) ;
}
} ;