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
class ImageProcImplHolder implement Fat<ImageProcHolder ,ImageProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	ImageLayout load (RREF<AutoRef<Pointer>> item) const override {
		ImageLayout ret ;
		auto rax = move (AutoRef<Pin<cv::Mat>> (move (item))->self) ;
		const auto r1x = rax.size () ;
		const auto r2x = size_of_cvmat_depth (rax.depth ()) ;
		const auto r3x = rax.channels ();
		auto &&tmp1 = keep[TYPE<RefBufferLayout>::expr] (ret.mImage) ;
		tmp1.mBuffer = FLAG (rax.data) ;
		tmp1.mSize = r1x.area () * r3x ;
		tmp1.mStep = r2x ;
		ret.mWidth.mCX = r1x.width ;
		ret.mWidth.mCY = r1x.height ;
		ret.mWidth.mStep = r2x * r3x ;
		ret.mWidth.mChannel = r3x ;
		RefBufferHolder::create (ret.mImage)->initialize (Box<Pin<cv::Mat>>::make ()) ;
		auto &&tmp2 = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (ret.mImage.raw ()).self ;
		tmp2.self = move (rax) ;
		ImageHolder::create (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout load (CREF<String<STR>> file) const override {
		ImageLayout ret ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		auto rax = cv::imread (r1x.self ,cv::IMREAD_UNCHANGED) ;
		assume (!(rax.empty ())) ;
		const auto r2x = rax.size () ;
		const auto r3x = size_of_cvmat_depth (rax.depth ()) ;
		const auto r4x = rax.channels () ;
		auto &&tmp1 = keep[TYPE<RefBufferLayout>::expr] (ret.mImage) ;
		tmp1.mBuffer = FLAG (rax.data) ;
		tmp1.mSize = r2x.area () * r4x ;
		tmp1.mStep = r3x ;
		ret.mWidth.mCX = r2x.width ;
		ret.mWidth.mCY = r2x.height ;
		ret.mWidth.mStep = r3x * r4x ;
		ret.mWidth.mChannel = r4x ;
		RefBufferHolder::create (ret.mImage)->initialize (Box<Pin<cv::Mat>>::make ()) ;
		auto &&tmp2 = keep[TYPE<Box<Pin<cv::Mat>>>::expr] (ret.mImage.raw ()).self ;
		tmp2.self = move (rax) ;
		ImageHolder::create (ret)->reset () ;
		return move (ret) ;
	}

	LENGTH size_of_cvmat_depth (CREF<int> depth) const {
		if (depth == CV_8U)
			return 1 ;
		if (depth == CV_16U)
			return 2 ;
		if (depth == CV_8S)
			return 1 ;
		if (depth == CV_16S)
			return 2 ;
		if (depth == CV_32S)
			return 4 ;
		if (depth == CV_16F)
			return 2 ;
		if (depth == CV_32F)
			return 4 ;
		if (depth == CV_64F)
			return 8 ;
		assume (FALSE) ;
		return 0 ;
	}

	void save (CREF<String<STR>> file ,CREF<ImageLayout> image) const override {
		const auto r1x = StringProc::stra_from_strs (file) ;
		const auto r2x = keep[TYPE<Box<cv::Mat>>::expr] (image.mImage.raw ()).self ;
		cv::imwrite (r1x.self ,r2x) ;
	}

	Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		Color3B ret ;
		auto &&tmp = keep[TYPE<Image<Color3B>>::expr] (image) ;
		const auto r1x = VAL (MathProc::round (x ,FLT64 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT64 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
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

	Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		Color4B ret ;
		auto &&tmp = keep[TYPE<Image<Color4B>>::expr] (image) ;
		const auto r1x = VAL (MathProc::round (x ,FLT64 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT64 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
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

static const auto anonymous = External<ImageProcHolder ,ImageProcLayout>::declare ([] () {
	return inline_hold (ImageProcImplHolder ()) ;
}) ;
} ;