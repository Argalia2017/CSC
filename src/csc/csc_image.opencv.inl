#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

#include "csc_end.h"
#ifdef __CSC_VER_DEBUG__
#define CV_IGNORE_DEBUG_BUILD_GUARD
#endif

#include <opencv2/opencv.hpp>
#include "csc_begin.h"

namespace CSC {
struct ImageToolPureLayout {} ;

class ImageProcImplement implement Fat<ImageToolHolder ,ImageToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<ImageToolPureLayout>::make () ;
	}

	ImageLayout load (CREF<String> file) const override {
		ImageLayout ret ;
		const auto r1x = StringTool::cvt_stra (file) ;
		auto rax = Box<cv::Mat>::make (cv::imread (r1x.raw (TYPE<STRA>::expr) ,cv::IMREAD_COLOR)) ;
		assume (ifnot (rax->empty ())) ;
		const auto r2x = rax->size () ;
		const auto r3x = Clazz (TYPE<COLOR3B>::expr) ;
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

	void save (CREF<String> file ,CREF<ImageLayout> image) const override {
		assert (image.mWidth.mClazz == Clazz (TYPE<COLOR3B>::expr)) ;
		const auto r1x = StringTool::cvt_stra (file) ;
		auto &&tmp = keep[TYPE<cv::Mat>::expr] (image.mImage.raw ()) ;
		cv::imwrite (r1x.raw (TYPE<STRA>::expr) ,tmp) ;
	}

	COLOR3B sampler (TYPEID<COLOR3B> ,CREF<ImageLayout> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		COLOR3B ret ;
		auto &&tmp = keep[TYPE<Image<COLOR3B>>::expr] (image) ;
		const auto r2x = MathTool::clamp (VAL (MathTool::round (x ,FLT64 (1))) ,ZERO ,image.mCX - 1) ;
		const auto r3x = MathTool::clamp (VAL (MathTool::round (y ,FLT64 (1))) ,ZERO ,image.mCY - 1) ;
		const auto r4x = MathTool::max_of (r2x - 1 ,ZERO) ;
		const auto r5x = MathTool::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = FLT64 (r2x) + FLT64 (0.5) - x ;
		const auto r7x = FLT64 (r3x) + FLT64 (0.5) - y ;
		const auto r8x = 1 - r6x ;
		const auto r9x = 1 - r7x ;
		const auto r10x = Color (tmp.at (r4x ,r5x)) ;
		const auto r11x = Color (tmp.at (r4x ,r3x)) ;
		const auto r12x = Color (tmp.at (r2x ,r3x)) ;
		const auto r13x = Color (tmp.at (r2x ,r5x)) ;
		if ifdo (TRUE) {
			auto rax = Color (0) ;
			rax += r10x * (r6x * r7x) ;
			rax += r11x * (r6x * r9x) ;
			rax += r12x * (r8x * r9x) ;
			rax += r13x * (r8x * r7x) ;
			rax.clamp (0 ,255) ;
			ret.mB = BYTE (rax[0]) ;
			ret.mG = BYTE (rax[1]) ;
			ret.mR = BYTE (rax[2]) ;
		}
		return move (ret) ;
	}

	COLOR4B sampler (TYPEID<COLOR4B> ,CREF<ImageLayout> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		COLOR4B ret ;
		auto &&tmp = keep[TYPE<Image<COLOR4B>>::expr] (image) ;
		const auto r2x = MathTool::clamp (VAL (MathTool::round (x ,FLT64 (1))) ,ZERO ,image.mCX - 1) ;
		const auto r3x = MathTool::clamp (VAL (MathTool::round (y ,FLT64 (1))) ,ZERO ,image.mCY - 1) ;
		const auto r4x = MathTool::max_of (r2x - 1 ,ZERO) ;
		const auto r5x = MathTool::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = FLT64 (r2x) + FLT64 (0.5) - x ;
		const auto r7x = FLT64 (r3x) + FLT64 (0.5) - y ;
		const auto r8x = 1 - r6x ;
		const auto r9x = 1 - r7x ;
		const auto r10x = Color (tmp.at (r4x ,r5x)) ;
		const auto r11x = Color (tmp.at (r4x ,r3x)) ;
		const auto r12x = Color (tmp.at (r2x ,r3x)) ;
		const auto r13x = Color (tmp.at (r2x ,r5x)) ;
		if ifdo (TRUE) {
			auto rax = Color (0) ;
			rax += r10x * (r6x * r7x) ;
			rax += r11x * (r6x * r9x) ;
			rax += r12x * (r8x * r9x) ;
			rax += r13x * (r8x * r7x) ;
			rax.clamp (0 ,255) ;
			ret.mB = BYTE (rax[0]) ;
			ret.mG = BYTE (rax[1]) ;
			ret.mR = BYTE (rax[2]) ;
			ret.mA = BYTE (rax[3]) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<ImageToolHolder> ImageToolHolder::create (VREF<ImageToolLayout> that) {
	return VFat<ImageToolHolder> (ImageProcImplement () ,Pointer::from (that)) ;
}

exports CFat<ImageToolHolder> ImageToolHolder::create (CREF<ImageToolLayout> that) {
	return CFat<ImageToolHolder> (ImageProcImplement () ,Pointer::from (that)) ;
}
} ;