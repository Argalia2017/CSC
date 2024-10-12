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
class PinnedMat implement ThisLayout<Ref<Pin<cv::Mat>>> {
private:
	using ThisLayout<Ref<Pin<cv::Mat>>>::mThis ;

public:
	explicit PinnedMat () {
		mThis = Ref<Pin<cv::Mat>>::make () ;
		mThis->remake () ;
	}

	VREF<cv::Mat> self_m () leftvalue {
		return mThis->self ;
	}

	CREF<cv::Mat> self_m () const leftvalue {
		return mThis->self ;
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
		RefBufferHolder::create (rax)->initialize (r5x ,move (image)) ;
		rax.mBuffer = FLAG (r1x.data) ;
		rax.mSize = r2x.area () ;
		rax.mStep = r3x * r4x ;
		ret.mWidth.mCX = r2x.width ;
		ret.mWidth.mCY = r2x.height ;
		ret.mWidth.mStep = r3x * r4x ;
		ImageHolder::create (ret)->reset () ;
		return move (ret) ;
	}

	ImageLayout make_image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> align ,CREF<LENGTH> channel) const override {
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

	Color1B sampler (CREF<Image<Color1B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,TYPE<Color1B>::expr ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color2B sampler (CREF<Image<Color2B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,TYPE<Color2B>::expr ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,TYPE<Color3B>::expr ,FLT32 (x) ,FLT32 (y)) ;
	}

	Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const override {
		return sampler_impl (image ,TYPE<Color4B>::expr ,FLT32 (x) ,FLT32 (y)) ;
	}

	template <class ARG1>
	ARG1 sampler_impl (CREF<Image<ARG1>> image ,TYPE<ARG1> ,CREF<FLT32> x ,CREF<FLT32> y) const {
		ARG1 ret ;
		const auto r1x = VAL (MathProc::round (x ,FLT32 (1))) ;
		const auto r2x = VAL (MathProc::round (y ,FLT32 (1))) ;
		const auto r3x = MathProc::clamp (r1x ,ZERO ,image.cx () - 1) ;
		const auto r4x = MathProc::clamp (r2x ,ZERO ,image.cy () - 1) ;
		const auto r5x = MathProc::max_of (r3x - 1 ,ZERO) ;
		const auto r6x = MathProc::max_of (r4x - 1 ,ZERO) ;
		const auto r7x = FLT32 (r3x) + FLT32 (0.5) - x ;
		const auto r8x = FLT32 (r4x) + FLT32 (0.5) - y ;
		const auto r9x = 1 - r7x ;
		const auto r10x = 1 - r8x ;
		if ifdo (TRUE) {
			const auto r11x = cvt_colorf (image.at (r5x ,r6x)) ;
			const auto r12x = cvt_colorf (image.at (r5x ,r4x)) ;
			const auto r13x = cvt_colorf (image.at (r3x ,r4x)) ;
			const auto r14x = cvt_colorf (image.at (r3x ,r6x)) ;
			auto rax = Buffer<FLT32 ,SIZE_OF<ARG1>> () ;
			for (auto &&i : iter (0 ,rax.size ())) {
				rax[i] = 0 ;
				rax[i] += r11x[i] * (r7x * r8x) ;
				rax[i] += r12x[i] * (r7x * r10x) ;
				rax[i] += r13x[i] * (r9x * r10x) ;
				rax[i] += r14x[i] * (r9x * r8x) ;
				rax[i] = MathProc::clamp (rax[i] ,FLT32 (0) ,FLT32 (255)) ;
			}
			ret = cvt_colorb (rax) ;
		}
		return move (ret) ;
	}

	Buffer1<FLT32> cvt_colorf (CREF<Color1B> a) const {
		Buffer1<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		return move (ret) ;
	}

	Buffer2<FLT32> cvt_colorf (CREF<Color2B> a) const {
		Buffer2<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		ret[1] = FLT32 (a.mG) ;
		return move (ret) ;
	}

	Buffer3<FLT32> cvt_colorf (CREF<Color3B> a) const {
		Buffer3<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		ret[1] = FLT32 (a.mG) ;
		ret[2] = FLT32 (a.mR) ;
		return move (ret) ;
	}

	Buffer4<FLT32> cvt_colorf (CREF<Color4B> a) const {
		Buffer4<FLT32> ret ;
		ret[0] = FLT32 (a.mB) ;
		ret[1] = FLT32 (a.mG) ;
		ret[2] = FLT32 (a.mR) ;
		ret[3] = FLT32 (a.mA) ;
		return move (ret) ;
	}

	Color1B cvt_colorb (CREF<Buffer1<FLT32>> a) const {
		Color1B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		return move (ret) ;
	}

	Color2B cvt_colorb (CREF<Buffer2<FLT32>> a) const {
		Color2B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		ret.mG = BYTE (VAL32 (a[1])) ;
		return move (ret) ;
	}

	Color3B cvt_colorb (CREF<Buffer3<FLT32>> a) const {
		Color3B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		ret.mG = BYTE (VAL32 (a[1])) ;
		ret.mR = BYTE (VAL32 (a[2])) ;
		return move (ret) ;
	}

	Color4B cvt_colorb (CREF<Buffer4<FLT32>> a) const {
		Color4B ret ;
		ret.mB = BYTE (VAL32 (a[0])) ;
		ret.mG = BYTE (VAL32 (a[1])) ;
		ret.mR = BYTE (VAL32 (a[2])) ;
		ret.mA = BYTE (VAL32 (a[3])) ;
		return move (ret) ;
	}
} ;

static const auto mImageProcExternal = External<ImageProcHolder ,ImageProcLayout>::declare (ImageProcImplHolder ()) ;
} ;