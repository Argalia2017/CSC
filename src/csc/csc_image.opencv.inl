#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_image.hpp"

#include "csc_end.h"
#ifdef __CSC_VER_DEBUG__
#define CV_IGNORE_DEBUG_BUILD_GUARD
#endif

#include <opencv2/opencv.hpp>
#include "csc_begin.h"

namespace CSC {
class ImageProcImplHolder final implement Fat<ImageProcHolder ,ImageProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	ImageLayout make_image (RR<BoxLayout> image) const override {
		ImageLayout ret ;
		auto &&rax = keep[TYPE<Box<UniqueRef<cv::Mat>>>::expr] (image).ref ;
		const auto r1x = Length (rax->cols) ;
		const auto r2x = Length (rax->rows) ;
		const auto r3x = align_of_cvmat_depth (rax->depth ()) ;
		const auto r4x = Length (rax->channels ()) ;
		const auto r5x = r3x * r4x ;
		const auto r6x = Length (rax->step[0]) ;
		assume (r6x % r5x == 0) ;
		const auto r7x = r6x / r5x ;
		const auto r8x = Flag (rax->data) ;
		const auto r9x = r7x * r2x ;
		const auto r10x = Slice (r8x ,r9x ,r5x) ;
		const auto r11x = choose_unknown (r3x) ;
		RefBufferHolder::hold (ret.mImage)->initialize (r11x ,r10x ,Box<UniqueRef<cv::Mat>>::make ()) ;
		auto &&rbx = keep[TYPE<Box<UniqueRef<cv::Mat>>>::expr] (RefBufferHolder::hold (ret.mImage)->raw ()).ref ;
		assign (rbx ,rax) ;
		ret.mWidth = r1x * 1024 + r3x ;
		ret.mStride = r7x ;
		ImageHolder::hold (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout make_image (CR<ImageShape> shape) const override {
		auto rax = Box<UniqueRef<cv::Mat>>::make () ;
		const auto r1x = CV_MAKE_TYPE (CV_8U ,Val32 (shape.mStep)) ;
		rax.ref = UniqueRef<cv::Mat> ([&] (VR<cv::Mat> me) {
			me = cv::Mat (cv::Size (Val32 (shape.mCX) ,Val32 (shape.mCY)) ,r1x) ;
			assume (!me.empty ()) ;
		} ,[&] (VR<cv::Mat> me) {
			noop () ;
		}) ;
		return make_image (move (rax)) ;
	}

	ImageLayout make_image (CR<ImageShape> shape ,CR<Clazz> clazz ,CR<Length> channel) const override {
		auto rax = Box<UniqueRef<cv::Mat>>::make () ;
		const auto r1x = cvmat_depth_of_clazz (clazz) ;
		const auto r2x = CV_MAKE_TYPE (Val32 (r1x) ,Val32 (channel)) ;
		rax.ref = UniqueRef<cv::Mat> ([&] (VR<cv::Mat> me) {
			me = cv::Mat (cv::Size (Val32 (shape.mCX) ,Val32 (shape.mCY)) ,r2x) ;
			assume (!me.empty ()) ;
		} ,[&] (VR<cv::Mat> me) {
			noop () ;
		}) ;
		return make_image (move (rax)) ;
	}

	Length cvmat_depth_of_clazz (CR<Clazz> clazz) const {
		if (clazz.type_align () == 1)
			return CV_8U ;
		if (clazz.type_align () == 2)
			return CV_16U ;
		if (clazz == Clazz (TYPE<Flt32>::expr))
			return CV_32F ;
		if (clazz == Clazz (TYPE<Flt64>::expr))
			return CV_64F ;
		assume (FALSE) ;
		return ZERO ;
	}

	Clazz clazz_of_cvmat_depth (CR<Length> depth) const {
		if (depth == CV_8U)
			return Clazz (TYPE<Byte>::expr) ;
		if (depth == CV_16U)
			return Clazz (TYPE<Word>::expr) ;
		if (depth == CV_32F)
			return Clazz (TYPE<Flt32>::expr) ;
		if (depth == CV_64F)
			return Clazz (TYPE<Flt64>::expr) ;
		assume (FALSE) ;
		return Clazz () ;
	}

	Length align_of_cvmat_depth (CR<Length> depth) const {
		if (depth == CV_8U)
			return 1 ;
		if (depth == CV_8S)
			return 1 ;
		if (depth == CV_16U)
			return 2 ;
		if (depth == CV_16S)
			return 2 ;
		if (depth == CV_32S)
			return 4 ;
		if (depth == CV_32F)
			return 4 ;
		if (depth == CV_64F)
			return 8 ;
		if (depth == CV_16F)
			return 2 ;
		assume (FALSE) ;
		return 0 ;
	}

	Unknown choose_unknown (CR<Length> align) const {
		if (align == SIZE_OF<Byte>::expr)
			return ArrayUnknownBinder<Byte> () ;
		if (align == SIZE_OF<Word>::expr)
			return ArrayUnknownBinder<Word> () ;
		if (align == SIZE_OF<Char>::expr)
			return ArrayUnknownBinder<Char> () ;
		if (align == SIZE_OF<Quad>::expr)
			return ArrayUnknownBinder<Quad> () ;
		assume (FALSE) ;
		return Unknown (ZERO) ;
	}

	VR<Pointer> peek_image (VR<ImageLayout> image) const override {
		assert (ImageHolder::hold (image)->fixed ()) ;
		auto &&rax = keep[TYPE<Box<UniqueRef<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return Pointer::from (rax.ref) ;
	}

	CR<Pointer> peek_image (CR<ImageLayout> image) const override {
		assert (ImageHolder::hold (image)->fixed ()) ;
		auto &&rax = keep[TYPE<Box<UniqueRef<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()) ;
		return Pointer::from (rax.ref) ;
	}

	ImageLayout load_image (CR<String<Str>> file) const override {
		auto rax = Box<UniqueRef<cv::Mat>>::make () ;
		const auto r1x = StringProc::stra_from (file) ;
		rax.ref = UniqueRef<cv::Mat> ([&] (VR<cv::Mat> me) {
			me = cv::imread (r1x.ref ,cv::IMREAD_UNCHANGED) ;
			assume (!me.empty ()) ;
		} ,[&] (VR<cv::Mat> me) {
			noop () ;
		}) ;
		return make_image (move (rax)) ;
	}

	ImageLayout convert_image (CR<ImageLayout> image ,CR<Length> channel) const override {
		assert (channel >= 1) ;
		assert (channel <= 4) ;
		auto &&rax = keep[TYPE<Box<UniqueRef<cv::Mat>>>::expr] (ImageHolder::hold (image)->raw ()).ref ;
		const auto r1x = ImageHolder::hold (image)->shape () ;
		const auto r2x = ImageHolder::hold (image)->step () ;
		const auto r3x = Length (rax->depth ()) ;
		const auto r4x = clazz_of_cvmat_depth (r3x) ;
		const auto r5x = align_of_cvmat_depth (r3x) ;
		ImageLayout ret = make_image (r1x ,r4x ,channel) ;
		const auto r6x = r2x / r5x ;
		const auto r7x = MathProc::min_of (channel ,r6x) * r5x ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r6x == channel)
				discard ;
			if ifdo (TRUE) {
				if (channel <= r6x)
					discard ;
				const auto r8x = ret.mImage.size () * r2x ;
				inline_memset (ret.mImage[0] ,r8x) ;
			}
			for (auto &&i : r1x.iter ()) {
				const auto r9x = address (ImageHolder::hold (ret)->at (i.mX ,i.mY)) ;
				const auto r10x = address (ImageHolder::hold (image)->at (i.mX ,i.mY)) ;
				inline_memcpy (Pointer::make (r9x) ,Pointer::make (r10x) ,r7x) ;
			}
		}
		if ifdo (act) {
			ImageHolder::hold (ret)->splice (0 ,0 ,image) ;
		}
		return move (ret) ;
	}

	void save_image (CR<String<Str>> file ,CR<ImageLayout> image) const override {
		const auto r1x = StringProc::stra_from (file) ;
		const auto r2x = ImageHolder::hold (image)->bx () ;
		const auto r3x = ImageHolder::hold (image)->by () ;
		const auto r4x = ImageHolder::hold (image)->cx () ;
		const auto r5x = ImageHolder::hold (image)->cy () ;
		auto &&rax = keep[TYPE<Box<UniqueRef<cv::Mat>>>::expr] (image.mImage.raw ()).ref ;
		const auto r6x = cv::Rect (Val32 (r2x) ,Val32 (r3x) ,Val32 (r4x) ,Val32 (r5x)) ;
		auto rbx = UniqueRef<cv::Mat> ([&] (VR<cv::Mat> me) {
			me = rax.ref (r6x) ;
		} ,[&] (VR<cv::Mat> me) {
			noop () ;
		}) ;
		cv::imwrite (r1x.ref ,rbx.ref) ;
	}

	Color1B sampler (CR<Image<Color1B>> image ,CR<Flt64> x ,CR<Flt64> y) const override {
		return sampler_f32_impl (image ,Flt32 (x) ,Flt32 (y)) ;
	}

	Color2B sampler (CR<Image<Color2B>> image ,CR<Flt64> x ,CR<Flt64> y) const override {
		return sampler_f32_impl (image ,Flt32 (x) ,Flt32 (y)) ;
	}

	Color3B sampler (CR<Image<Color3B>> image ,CR<Flt64> x ,CR<Flt64> y) const override {
		return sampler_f32_impl (image ,Flt32 (x) ,Flt32 (y)) ;
	}

	Color4B sampler (CR<Image<Color4B>> image ,CR<Flt64> x ,CR<Flt64> y) const override {
		return sampler_f32_impl (image ,Flt32 (x) ,Flt32 (y)) ;
	}

	template <class ARG1>
	forceinline ARG1 sampler_f32_impl (CR<Image<ARG1>> image ,CR<Flt32> x ,CR<Flt32> y) const {
		const auto r1x = Index (MathProc::round (x)) ;
		const auto r2x = Index (MathProc::round (y)) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = Flt32 (r3x) + Flt32 (0.5) - x ;
		const auto r8x = Flt32 (r4x) + Flt32 (0.5) - y ;
		const auto r9x = r7x * r8x ;
		const auto r10x = r7x * (1 - r8x) ;
		const auto r11x = (1 - r7x) * (1 - r8x) ;
		const auto r12x = (1 - r7x) * r8x ;
		const auto r13x = Color (image.at (r5x ,r6x)) ;
		const auto r14x = Color (image.at (r5x ,r4x)) ;
		const auto r15x = Color (image.at (r3x ,r4x)) ;
		const auto r16x = Color (image.at (r3x ,r6x)) ;
		auto rax = Color::all (0) ;
		rax += r13x * r9x ;
		rax += r14x * r10x ;
		rax += r15x * r11x ;
		rax += r16x * r12x ;
		rax = rax.sclamp () ;
		return rax.bgr () ;
	}

	Flt32 sampler (CR<Image<Flt32>> image ,CR<Flt64> x ,CR<Flt64> y) const override {
		return sampler_f64_impl (image ,x ,y) ;
	}

	Flt64 sampler (CR<Image<Flt64>> image ,CR<Flt64> x ,CR<Flt64> y) const override {
		return sampler_f64_impl (image ,x ,y) ;
	}

	template <class ARG1>
	forceinline ARG1 sampler_f64_impl (CR<Image<ARG1>> image ,CR<Flt64> x ,CR<Flt64> y) const {
		const auto r1x = Index (MathProc::round (x)) ;
		const auto r2x = Index (MathProc::round (y)) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = Flt64 (r3x) + Flt64 (0.5) - x ;
		const auto r8x = Flt64 (r4x) + Flt64 (0.5) - y ;
		const auto r9x = r7x * r8x ;
		const auto r10x = r7x * (1 - r8x) ;
		const auto r11x = (1 - r7x) * (1 - r8x) ;
		const auto r12x = (1 - r7x) * r8x ;
		const auto r13x = image.at (r5x ,r6x) ;
		const auto r14x = image.at (r5x ,r4x) ;
		const auto r15x = image.at (r3x ,r4x) ;
		const auto r16x = image.at (r3x ,r6x) ;
		auto rax = Flt64 (0) ;
		rax += r13x * r9x ;
		rax += r14x * r10x ;
		rax += r15x * r11x ;
		rax += r16x * r12x ;
		return ARG1 (rax) ;
	}
} ;

static const auto mImageProcExternal = External<ImageProcHolder ,ImageProcLayout> (ImageProcImplHolder ()) ;
} ;