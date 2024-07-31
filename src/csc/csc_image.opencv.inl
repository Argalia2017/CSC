#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC system_header
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang system_header
#endif

#include "csc_image.hpp"

#include "csc_end.h"
#ifdef __CSC_VER_DEBUG__
#define CV_IGNORE_DEBUG_BUILD_GUARD
#endif

#include <opencv2/opencv.hpp>
#include "csc_begin.h"

namespace CSC {
class PinnedMat implement Ref<Pin<cv::Mat>> {
private:
	using SUPER = Ref<Pin<cv::Mat>> ;

public:
	explicit PinnedMat () {
		auto &&rax = keep[TYPE<SUPER>::expr] (thiz) ;
		rax = SUPER::make () ;
		rax->remake () ;
	}

	VREF<cv::Mat> self_m () leftvalue {
		auto &&rax = keep[TYPE<SUPER>::expr] (thiz) ;
		return rax->self ;
	}

	CREF<cv::Mat> self_m () const leftvalue {
		auto &&rax = keep[TYPE<SUPER>::expr] (thiz) ;
		return rax->self ;
	}
} ;

class ImageProcImplHolder implement Fat<ImageProcHolder ,ImageProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	ImageLayout make_image (RREF<BoxLayout> image) const override {
		ImageLayout ret ;
		const auto r1x = keep[TYPE<Box<PinnedMat>>::expr] (image)->self ;
		const auto r2x = r1x.size () ;
		const auto r3x = align_of_cvmat_depth (r1x.depth ()) ;
		const auto r4x = r1x.channels () ;
		const auto r5x = choose_cvmat_unknown (r3x) ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (ret.mImage) ;
		RefBufferHolder::create (ret.mImage)->initialize (r5x ,move (image)) ;
		rax.mBuffer = FLAG (r1x.data) ;
		rax.mSize = r2x.area () ;
		rax.mStep = r3x * r4x ;
		ret.mWidth.mCX = r2x.width ;
		ret.mWidth.mCY = r2x.height ;
		ret.mWidth.mStep = r3x * r4x ;
		ImageHolder::create (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout load_image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> align ,CREF<LENGTH> channel) const override {
		auto rax = Box<PinnedMat>::make () ;
		const auto r1x = cvmat_depth_of_align (align) ;
		const auto r2x = CV_MAKE_TYPE (VAL32 (r1x) ,VAL32 (channel)) ;
		rax->self = cv::Mat (cv::Size (VAL32 (cx_) ,VAL32 (cy_)) ,r2x) ;
		return make_image (move (rax)) ;
	}

	ImageLayout load_image (CREF<String<STR>> file) const override {
		auto rax = Box<PinnedMat>::make () ;
		const auto r1x = StringProc::stra_from_strs (file) ;
		rax->self = cv::imread (r1x.self ,cv::IMREAD_UNCHANGED) ;
		assume (!rax->self.empty ()) ;
		return make_image (move (rax)) ;
	}

	LENGTH cvmat_depth_of_align (CREF<LENGTH> align) const {
		if (align == 1)
			return CV_8U ;
		if (align == 2)
			return CV_16U ;
		if (align == 4)
			return CV_32F ;
		if (align == 8)
			return CV_64F ;
		assume (FALSE) ;
		return 0 ;
	}

	LENGTH align_of_cvmat_depth (CREF<LENGTH> depth) const {
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

	RFat<Unknown> choose_cvmat_unknown (CREF<LENGTH> align) const {
		if (align == SIZE_OF<BYTE>::expr)
			return RFat<Unknown> (BufferUnknownBinder<BYTE> () ,NULL) ;
		if (align == SIZE_OF<WORD>::expr)
			return RFat<Unknown> (BufferUnknownBinder<WORD> () ,NULL) ;
		if (align == SIZE_OF<CHAR>::expr)
			return RFat<Unknown> (BufferUnknownBinder<CHAR> () ,NULL) ;
		if (align == SIZE_OF<QUAD>::expr)
			return RFat<Unknown> (BufferUnknownBinder<QUAD> () ,NULL) ;
		assume (FALSE) ;
		return RFat<Unknown> (BufferUnknownBinder<BYTE> () ,NULL) ; ;
	}

	void save_image (CREF<String<STR>> file ,CREF<ImageLayout> image) const override {
		const auto r1x = StringProc::stra_from_strs (file) ;
		const auto r2x = keep[TYPE<Box<PinnedMat>>::expr] (image.mImage.raw ())->self ;
		cv::imwrite (r1x.self ,r2x) ;
	}

	Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		Color3B ret ;
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
		const auto r11x = cvt_color3 (image.at (r5x ,r6x)) ;
		const auto r12x = cvt_color3 (image.at (r5x ,r4x)) ;
		const auto r13x = cvt_color3 (image.at (r3x ,r4x)) ;
		const auto r14x = cvt_color3 (image.at (r3x ,r6x)) ;
		if ifdo (TRUE) {
			auto rax = Buffer3<FLT64> ({0 ,0 ,0}) ;
			for (auto &&i : iter (0 ,rax.size ())) {
				rax[i] += r11x[i] * (r7x * r8x) ;
				rax[i] += r12x[i] * (r7x * r10x) ;
				rax[i] += r13x[i] * (r9x * r10x) ;
				rax[i] += r14x[i] * (r9x * r8x) ;
				rax[i] = MathProc::clamp (rax[i] ,FLT64 (0) ,FLT64 (255)) ;
			}
			ret.mB = BYTE (VAL32 (rax[0])) ;
			ret.mG = BYTE (VAL32 (rax[1])) ;
			ret.mR = BYTE (VAL32 (rax[2])) ;
		}
		return move (ret) ;
	}

	Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		Color4B ret ;
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
		const auto r11x = cvt_color4 (image.at (r5x ,r6x)) ;
		const auto r12x = cvt_color4 (image.at (r5x ,r4x)) ;
		const auto r13x = cvt_color4 (image.at (r3x ,r4x)) ;
		const auto r14x = cvt_color4 (image.at (r3x ,r6x)) ;
		if ifdo (TRUE) {
			auto rax = Buffer4<FLT64> ({0 ,0 ,0 ,0}) ;
			for (auto &&i : iter (0 ,rax.size ())) {
				rax[i] += r11x[i] * (r7x * r8x) ;
				rax[i] += r12x[i] * (r7x * r10x) ;
				rax[i] += r13x[i] * (r9x * r10x) ;
				rax[i] += r14x[i] * (r9x * r8x) ;
				rax[i] = MathProc::clamp (rax[i] ,FLT64 (0) ,FLT64 (255)) ;
			}
			ret.mB = BYTE (VAL32 (rax[0])) ;
			ret.mG = BYTE (VAL32 (rax[1])) ;
			ret.mR = BYTE (VAL32 (rax[2])) ;
			ret.mA = BYTE (VAL32 (rax[3])) ;
		}
		return move (ret) ;
	}

	Buffer3<FLT64> cvt_color3 (CREF<Color3B> a) const {
		Buffer3<FLT64> ret ;
		ret[0] = FLT64 (a.mB) ;
		ret[1] = FLT64 (a.mG) ;
		ret[2] = FLT64 (a.mR) ;
		return move (ret) ;
	}

	Buffer4<FLT64> cvt_color4 (CREF<Color4B> a) const {
		Buffer4<FLT64> ret ;
		ret[0] = FLT64 (a.mB) ;
		ret[1] = FLT64 (a.mG) ;
		ret[2] = FLT64 (a.mR) ;
		ret[3] = FLT64 (a.mA) ;
		return move (ret) ;
	}
} ;

static const auto mImageProcExternal = External<ImageProcHolder ,ImageProcLayout>::declare (ImageProcImplHolder ()) ;
} ;