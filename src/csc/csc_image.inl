#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

namespace CSC {
class ImageShapeImplHolder final implement Fat<ImageShapeHolder ,ImageShapeLayout> {
public:
	void initialize (CR<Length> cx_ ,CR<Length> cy_ ,CR<Length> step_) override {
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		assert (step_ >= 0) ;
		self.mCX = cx_ ;
		self.mCY = cy_ ;
		self.mStep = step_ ;
	}

	Length size () const override {
		return self.mCX * self.mCY ;
	}

	Length step () const override {
		return self.mStep ;
	}

	Bool equal (CR<ImageShapeLayout> that) const override {
		if (self.mCX != that.mCX)
			return FALSE ;
		if (self.mCY != that.mCY)
			return FALSE ;
		if (self.mStep != 0)
			if (that.mStep != 0)
				if (self.mStep != that.mStep)
					return FALSE ;
		return TRUE ;
	}

	Bool contain (CR<Pixel> item) const override {
		if (!inline_between (item.mX ,0 ,self.mCX))
			return FALSE ;
		if (!inline_between (item.mY ,0 ,self.mCY))
			return FALSE ;
		return TRUE ;
	}
} ;

exports VFat<ImageShapeHolder> ImageShapeHolder::hold (VR<ImageShapeLayout> that) {
	return VFat<ImageShapeHolder> (ImageShapeImplHolder () ,that) ;
}

exports CFat<ImageShapeHolder> ImageShapeHolder::hold (CR<ImageShapeLayout> that) {
	return CFat<ImageShapeHolder> (ImageShapeImplHolder () ,that) ;
}

class ImageImplHolder final implement Fat<ImageHolder ,ImageLayout> {
public:
	void prepare (CR<Unknown> holder) override {
		RefBufferHolder::hold (self.mImage)->prepare (holder) ;
	}

	void initialize (CR<Unknown> holder ,RR<ImageLayout> that) override {
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		noop (r2x) ;
		noop (r3x) ;
		assume (r2x == that.mImage.step ()) ;
		assume (r3x == that.mWidth % 1024) ;
		self = move (that) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> cx_ ,CR<Length> cy_ ,CR<Length> step_) override {
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = r1x->type_align () ;
		const auto r4x = cx_ * cy_ * step_ ;
		const auto r5x = inline_alignas (r4x ,r2x) / r2x ;
		if ifdo (TRUE) {
			auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mImage) ;
			RefBufferHolder::hold (rax)->initialize (holder ,r5x) ;
			rax.mSize = cx_ * cy_ ;
			rax.mStep = step_ ;
		}
		self.mWidth = cx_ * 1024 + r3x ;
		self.mStride = cx_ ;
		reset () ;
	}

	void initialize (CR<ImageLayout> that) override {
		const auto r1x = ImageHolder::hold (that)->shape () ;
		if (r1x.size () == 0)
			return ;
		initialize (that.mImage.unknown () ,r1x.mCX ,r1x.mCY ,r1x.mStep) ;
		splice (0 ,0 ,that) ;
	}

	Bool fixed () const override {
		return self.mImage.fixed () ;
	}

	Length size () const override {
		return self.mImage.size () ;
	}

	Length step () const override {
		return self.mImage.step () ;
	}

	Length stride () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mStride ;
	}

	Bool continous () const override {
		if (bx () != 0)
			return FALSE ;
		if (by () != 0)
			return FALSE ;
		if (cx () != stride ())
			return FALSE ;
		return TRUE ;
	}

	Length bx () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mBX ;
	}

	Length by () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mBY ;
	}

	Length cx () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mCX ;
	}

	Length cy () const override {
		if (!self.mImage.exist ())
			return 0 ;
		return self.mCY ;
	}

	ImageShape shape () const override {
		ImageShape ret ;
		ret.mCX = cx () ;
		ret.mCY = cy () ;
		ret.mStep = step () ;
		return move (ret) ;
	}

	void reset () override {
		self.mBX = 0 ;
		self.mBY = 0 ;
		self.mCX = 0 ;
		self.mCY = 0 ;
		if (self.mImage.size () == 0)
			return ;
		const auto r1x = inline_max (self.mStride ,1) ;
		const auto r2x = self.mImage.size () / r1x ;
		const auto r3x = self.mWidth / 1024 ;
		reset (0 ,0 ,r3x ,r2x) ;
	}

	void reset (CR<Length> bx_ ,CR<Length> by_ ,CR<Length> cx_ ,CR<Length> cy_) override {
		assert (self.mImage.size () > 0) ;
		assert (bx_ >= 0) ;
		assert (by_ >= 0) ;
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		const auto r1x = bx_ + cx_ - 1 ;
		const auto r2x = by_ + cy_ - 1 ;
		assert (r1x < self.mStride) ;
		const auto r3x = r1x + r2x * self.mStride ;
		noop (r3x) ;
		assert (r3x < self.mImage.size ()) ;
		self.mBX = bx_ ;
		self.mBY = by_ ;
		self.mCX = cx_ ;
		self.mCY = cy_ ;
	}

	VR<BoxLayout> raw () leftvalue override {
		return RefBufferHolder::hold (self.mImage)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue override {
		return RefBufferHolder::hold (self.mImage)->raw () ;
	}

	VR<Pointer> at (CR<Index> x ,CR<Index> y) leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		Index ix = (x + self.mBX) + (y + self.mBY) * self.mStride ;
		return self.mImage.at (ix) ;
	}

	CR<Pointer> at (CR<Index> x ,CR<Index> y) const leftvalue override {
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		Index ix = (x + self.mBX) + (y + self.mBY) * self.mStride ;
		return self.mImage.at (ix) ;
	}

	void fill (CR<Pointer> item) override {
		const auto r1x = step () ;
		if ifdo (TRUE) {
			if (cy () == 0)
				discard ;
			for (auto &&i : range (0 ,cx ())) {
				inline_memcpy (at (i ,0) ,item ,r1x) ;
			}
		}
		const auto r2x = cx () * r1x ;
		for (auto &&i : range (1 ,cy ())) {
			inline_memcpy (at (0 ,i) ,at (0 ,0) ,r2x) ;
		}
	}

	void splice (CR<Index> x ,CR<Index> y ,CR<ImageLayout> item) override {
		const auto r1x = ImageHolder::hold (item)->cx () ;
		const auto r2x = ImageHolder::hold (item)->cy () ;
		if (r1x == 0)
			return ;
		if (r2x == 0)
			return ;
		assert (inline_between (x ,0 ,cx ())) ;
		assert (inline_between (y ,0 ,cy ())) ;
		assert (x + r1x <= cx ()) ;
		assert (y + r2x <= cy ()) ;
		const auto r3x = ImageHolder::hold (item)->step () ;
		assert (step () == r3x) ;
		const auto r4x = r1x * r3x ;
		for (auto &&i : range (0 ,r2x)) {
			Index ix = x + 0 ;
			Index iy = y + i ;
			inline_memcpy (at (ix ,iy) ,ImageHolder::hold (item)->at (0 ,i) ,r4x) ;
		}
	}
} ;

exports VFat<ImageHolder> ImageHolder::hold (VR<ImageLayout> that) {
	return VFat<ImageHolder> (ImageImplHolder () ,that) ;
}

exports CFat<ImageHolder> ImageHolder::hold (CR<ImageLayout> that) {
	return CFat<ImageHolder> (ImageImplHolder () ,that) ;
}

class ColorImplHolder final implement Fat<ColorHolder ,ColorLayout> {
private:
	using COLOR_SHIFT = ENUM<1024> ;

public:
	void initialize (CR<Val32> item) override {
		self.mWhite = Val32 (255) * Val32 (COLOR_SHIFT::expr) ;
		const auto r1x = item * Val32 (COLOR_SHIFT::expr) ;
		self.mColor[0] = r1x ;
		self.mColor[1] = r1x ;
		self.mColor[2] = r1x ;
		self.mColor[3] = r1x ;
	}

	void initialize (CR<Flag> buffer ,CR<Length> size_ ,CR<Length> step_) override {
		assert (size_ > 0) ;
		assert (step_ > 0) ;
		const auto r1x = size_ / step_ ;
		auto act = TRUE ;
		if ifdo (act) {
			if (step_ != 1)
				discard ;
			self.mWhite = Val32 (~Byte (0X00)) ;
			for (auto &&i : range (0 ,r1x)) {
				const auto r2x = buffer + i * step_ ;
				const auto r3x = Byte (bitwise (Pointer::make (r2x))) ;
				self.mColor[i] = Val32 (r3x) ;
			}
		}
		if ifdo (act) {
			if (step_ != 2)
				discard ;
			self.mWhite = Val32 (~Word (0X00)) ;
			for (auto &&i : range (0 ,r1x)) {
				const auto r4x = buffer + i * step_ ;
				const auto r5x = Word (bitwise (Pointer::make (r4x))) ;
				self.mColor[i] = Val32 (r5x) ;
			}
		}
		if ifdo (act) {
			if (step_ != 4)
				discard ;
			const auto r6x = Flt32 (65535) ;
			self.mWhite = Val32 (r6x) ;
			for (auto &&i : range (0 ,r1x)) {
				const auto r7x = buffer + i * step_ ;
				const auto r8x = Flt32 (bitwise (Pointer::make (r7x))) ;
				self.mColor[i] = Val32 (r8x * r6x) ;
			}
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
		for (auto &&i : range (r1x ,4)) {
			self.mColor[i] = 0 ;
		}
		self.mWhite *= Val32 (COLOR_SHIFT::expr) ;
		for (auto &&i : range (0 ,4)) {
			self.mColor[i] *= Val32 (COLOR_SHIFT::expr) ;
		}
	}

	void get (CR<Index> index ,VR<Val32> item) const override {
		item = self.mColor[index] / Val32 (COLOR_SHIFT::expr) ;
	}

	ColorLayout stretch (CR<Val32> white) const override {
		ColorLayout ret ;
		const auto r1x = Flt64 (white) * MathProc::inverse (Flt64 (self.mWhite)) ;
		ret = smul (r1x) ;
		ret.mWhite = white ;
		return move (ret) ;
	}

	ColorLayout sadd (CR<ColorLayout> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = self.mColor[i] + that.mColor[i] ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout ssub (CR<ColorLayout> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = self.mColor[i] - that.mColor[i] ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout smul (CR<Flt64> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			const auto r1x = Flt64 (self.mColor[i]) * that ;
			ret.mColor[i] = Val32 (r1x) ;
		}
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout sdiv (CR<Flt64> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			const auto r1x = Flt64 (self.mColor[i]) / that ;
			ret.mColor[i] = Val32 (r1x) ;
		}
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout smul (CR<ColorLayout> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			const auto r1x = Val64 (self.mColor[i]) * Val64 (that.mColor[i]) ;
			ret.mColor[i] = Val32 (r1x / that.mWhite) ;
		}
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout sabs () const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = MathProc::abs (self.mColor[i]) ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout minus () const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = -self.mColor[i] ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout smax (CR<ColorLayout> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = MathProc::max_of (self.mColor[i] ,that.mColor[i]) ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout smin (CR<ColorLayout> that) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = MathProc::min_of (self.mColor[i] ,that.mColor[i]) ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout sclamp () const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = MathProc::clamp (self.mColor[i] ,Val32 (0) ,self.mWhite) ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout swrap () const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = MathProc::wrap (self.mColor[i] ,self.mWhite) ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}

	ColorLayout slerp (CR<ColorLayout> into ,CR<Flt64> t) const override {
		ColorLayout ret ;
		for (auto &&i : range (0 ,4))
			ret.mColor[i] = MathProc::lerp (t ,self.mColor[i] ,into.mColor[i]) ;
		ret.mWhite = self.mWhite ;
		return move (ret) ;
	}
} ;

exports VFat<ColorHolder> ColorHolder::hold (VR<ColorLayout> that) {
	return VFat<ColorHolder> (ColorImplHolder () ,that) ;
}

exports CFat<ColorHolder> ColorHolder::hold (CR<ColorLayout> that) {
	return CFat<ColorHolder> (ColorImplHolder () ,that) ;
}

struct ColorProcLayout {} ;

class ColorProcImplHolder final implement Fat<ColorProcHolder ,ColorProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Flt64 byte_norm (CR<Byte> a) const override {
		return Flt64 (a) * MathProc::inverse (Flt32 (255)) ;
	}

	Byte byte_norm (CR<Flt64> a) const override {
		const auto r1x = Val32 (MathProc::round (a * 255)) ;
		return Byte (MathProc::clamp (r1x ,Val32 (0) ,Val32 (255))) ;
	}

	Flt64 word_norm (CR<Word> a) const override {
		return Flt64 (a) * MathProc::inverse (Flt32 (65535)) ;
	}

	Word word_norm (CR<Flt64> a) const override {
		const auto r1x = Val32 (MathProc::round (a * 65535)) ;
		return Word (MathProc::clamp (r1x ,Val32 (0) ,Val32 (65535))) ;
	}

	Byte gray_from_bgr (CR<Color3B> a) const override {
		const auto r1x = byte_norm (a.mR) ;
		const auto r2x = byte_norm (a.mG) ;
		const auto r3x = byte_norm (a.mB) ;
		const auto r4x = 0.299 * r1x + 0.587 * r2x + 0.114 * r3x ;
		return byte_norm (r4x) ;
	}

	Word gray_from_bgr (CR<Color3W> a) const override {
		const auto r1x = word_norm (a.mR) ;
		const auto r2x = word_norm (a.mG) ;
		const auto r3x = word_norm (a.mB) ;
		const auto r4x = 0.299 * r1x + 0.587 * r2x + 0.114 * r3x ;
		return word_norm (r4x) ;
	}

	Color3B bgr_from_gray (CR<Byte> a) const override {
		Color3B ret ;
		ret.mB = a ;
		ret.mG = a ;
		ret.mR = a ;
		return move (ret) ;
	}

	Color3W bgr_from_gray (CR<Word> a) const override {
		Color3W ret ;
		ret.mB = a ;
		ret.mG = a ;
		ret.mR = a ;
		return move (ret) ;
	}

	Color3B jet_from_norm (CR<Flt64> a) const override {
		Color3B ret ;
		const auto r1x = Val32 (MathProc::round (a * 1024)) ;
		const auto r2x = MathProc::clamp (r1x ,Val32 (0) ,Val32 (1024)) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x >= 128)
				discard ;
			ret.mR = Byte (0) ;
			ret.mG = Byte (0) ;
			ret.mB = Byte (128 + r2x) ;
		}
		if ifdo (act) {
			if (r2x >= 384)
				discard ;
			ret.mR = Byte (0) ;
			ret.mG = Byte (r2x - 128) ;
			ret.mB = Byte (255) ;
		}
		if ifdo (act) {
			if (r2x >= 640)
				discard ;
			ret.mR = Byte (r2x - 384) ;
			ret.mG = Byte (255) ;
			ret.mB = Byte (255 - (r2x - 384)) ;
		}
		if ifdo (act) {
			if (r2x >= 896)
				discard ;
			ret.mR = Byte (255) ;
			ret.mG = Byte (255 - (r2x - 640)) ;
			ret.mB = Byte (0) ;
		}
		if ifdo (act) {
			if (r2x >= 1024)
				discard ;
			ret.mR = Byte (255 - (r2x - 896)) ;
			ret.mG = Byte (0) ;
			ret.mB = Byte (0) ;
		}
		if ifdo (act) {
			ret.mR = Byte (128) ;
			ret.mG = Byte (0) ;
			ret.mB = Byte (0) ;
		}
		return move (ret) ;
	}

	Flt64 norm_from_jet (CR<Color3B> a) const override {
		const auto r1x = Val32 (a.mR) ;
		const auto r2x = Val32 (a.mG) ;
		const auto r3x = Val32 (a.mB) ;
		const auto r4x = MathProc::inverse (Flt64 (1024)) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (a.mR != Byte (0))
				discard ;
			if (a.mG != Byte (0))
				discard ;
			const auto r5x = MathProc::max_of (r3x ,128) ;
			return (r5x - 128) * r4x ;
		}
		if ifdo (act) {
			if (a.mR != Byte (0))
				discard ;
			if (a.mB != Byte (255))
				discard ;
			return (r2x + 128) * r4x ;
		}
		if ifdo (act) {
			if (a.mG != Byte (255))
				discard ;
			return (r1x + 384) * r4x ;
		}
		if ifdo (act) {
			if (a.mR != Byte (255))
				discard ;
			if (a.mB != Byte (0))
				discard ;
			return (640 + (255 - r2x)) * r4x ;
		}
		if ifdo (act) {
			if (a.mG != Byte (0))
				discard ;
			if (a.mB != Byte (0))
				discard ;
			const auto r6x = MathProc::max_of (r1x ,128) ;
			return (896 + (255 - r6x)) * r4x ;
		}
		return Flt64 (1) ;
	}

	Color3W hsv_from_bgr (CR<Color3B> a) const override {
		Color3W ret ;
		const auto r1x = byte_norm (a.mR) ;
		const auto r2x = byte_norm (a.mG) ;
		const auto r3x = byte_norm (a.mB) ;
		const auto r4x = MathProc::min_of (r1x ,r2x ,r3x) ;
		const auto r5x = MathProc::max_of (r1x ,r2x ,r3x) ;
		const auto r6x = r5x - r4x ;
		const auto r7x = MathProc::inverse (r6x) ;
		auto rax = Flt64 (0) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r6x > 0)
				discard ;
			rax = Flt64 (0) ;
		}
		if ifdo (act) {
			if (r5x != r1x)
				discard ;
			rax = (r2x - r3x) * r7x ;
			rax -= MathProc::floor (rax ,Flt64 (6)) ;
		}
		if ifdo (act) {
			if (r5x != r2x)
				discard ;
			rax = (r3x - r1x) * r7x + 2 ;
		}
		if ifdo (act) {
			if (r5x != r3x)
				discard ;
			rax = (r1x - r2x) * r7x + 4 ;
		}
		rax *= 60 ;
		rax -= MathProc::floor (rax ,Flt64 (360)) ;
		rax /= 360 ;
		const auto r8x = r6x * MathProc::inverse (r5x) ;
		ret.mB = word_norm (rax) ;
		ret.mG = word_norm (r8x) ;
		ret.mR = word_norm (r5x) ;
		return move (ret) ;
	}

	Color3B bgr_from_hsv (CR<Color3W> a) const override {
		Color3B ret ;
		const auto r1x = word_norm (a.mB) * 360 ;
		const auto r2x = word_norm (a.mG) ;
		const auto r3x = word_norm (a.mR) ;
		const auto r4x = r3x * r2x ;
		const auto r5x = (r1x - MathProc::floor (r1x ,Flt64 (120))) / 60 ;
		const auto r6x = r4x * (1 - MathProc::abs (r5x - 1)) ;
		const auto r7x = r3x - r4x ;
		auto rax = Buffer3<Flt64> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r1x >= 60)
				discard ;
			rax[0] = r4x ;
			rax[1] = r6x ;
			rax[2] = 0 ;
		}
		if ifdo (act) {
			if (r1x >= 120)
				discard ;
			rax[0] = r6x ;
			rax[1] = r4x ;
			rax[2] = 0 ;
		}
		if ifdo (act) {
			if (r1x >= 180)
				discard ;
			rax[0] = 0 ;
			rax[1] = r4x ;
			rax[2] = r6x ;
		}
		if ifdo (act) {
			if (r1x >= 240)
				discard ;
			rax[0] = 0 ;
			rax[1] = r6x ;
			rax[2] = r4x ;
		}
		if ifdo (act) {
			if (r1x >= 300)
				discard ;
			rax[0] = r6x ;
			rax[1] = 0 ;
			rax[2] = r4x ;
		}
		if ifdo (act) {
			rax[0] = r4x ;
			rax[1] = 0 ;
			rax[2] = r6x ;
		}
		rax[0] += r7x ;
		rax[1] += r7x ;
		rax[2] += r7x ;
		ret.mB = byte_norm (rax[2]) ;
		ret.mG = byte_norm (rax[1]) ;
		ret.mR = byte_norm (rax[0]) ;
		return move (ret) ;
	}
} ;

exports CR<Super<UniqueRef<ColorProcLayout>>> ColorProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<ColorProcLayout>> ret ;
		ret.mThis = UniqueRef<ColorProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		ColorProcHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ColorProcHolder> ColorProcHolder::hold (VR<ColorProcLayout> that) {
	return VFat<ColorProcHolder> (ColorProcImplHolder () ,that) ;
}

exports CFat<ColorProcHolder> ColorProcHolder::hold (CR<ColorProcLayout> that) {
	return CFat<ColorProcHolder> (ColorProcImplHolder () ,that) ;
}

template class External<ImageProcHolder ,ImageProcLayout> ;

struct ImageProcLayout {
	UniqueRef<Bool> mContext ;
} ;

exports CR<Super<UniqueRef<ImageProcLayout>>> ImageProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<ImageProcLayout>> ret ;
		ret.mThis = UniqueRef<ImageProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		ImageProcHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ImageProcHolder> ImageProcHolder::hold (VR<ImageProcLayout> that) {
	return VFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::expr ,that) ;
}

exports CFat<ImageProcHolder> ImageProcHolder::hold (CR<ImageProcLayout> that) {
	return CFat<ImageProcHolder> (External<ImageProcHolder ,ImageProcLayout>::expr ,that) ;
}

struct ReflectTensorPair implement Interface {
	virtual Just<TensorType> type () const = 0 ;
	virtual void clone (VR<Pointer> a ,CR<Pointer> b) const = 0 ;
	virtual void sadd (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const = 0 ;
	virtual void ssub (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const = 0 ;
	virtual void smul (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const = 0 ;
	virtual void sdiv (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const = 0 ;
	virtual void sabs (CR<Pointer> a ,VR<Pointer> c) const = 0 ;
	virtual void minus (CR<Pointer> a ,VR<Pointer> c) const = 0 ;

	forceinline static consteval Flag expr_m () noexcept {
		return 501 ;
	}
} ;

template <class A ,class B>
class ReflectTensorPairBinder final implement Fat<ReflectTensorPair ,void> {
private:
	using C = CONDITIONAL<ENUM_COMPR_GTEQ<SIZE_OF<A> ,SIZE_OF<B>> ,A ,B> ;

public:
	Just<TensorType> type () const override {
		const auto r1x = SIZE_OF<C>::expr ;
		if (r1x == 4)
			return TensorType::Flt32 ;
		if (r1x == 8)
			return TensorType::Flt64 ;
		if (r1x == 16)
			return TensorType::Flt128 ;
		assert (FALSE) ;
		return 0 ;
	}

	void clone (VR<Pointer> a ,CR<Pointer> b) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rbx = keep[TYPE<B>::expr] (b) ;
		rax = A (rbx) ;
	}

	void sadd (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rbx = keep[TYPE<B>::expr] (b) ;
		auto &&rcx = keep[TYPE<C>::expr] (c) ;
		rcx = C (rax) + C (rbx) ;
	}

	void ssub (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rbx = keep[TYPE<B>::expr] (b) ;
		auto &&rcx = keep[TYPE<C>::expr] (c) ;
		rcx = C (rax) - C (rbx) ;
	}

	void smul (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rbx = keep[TYPE<B>::expr] (b) ;
		auto &&rcx = keep[TYPE<C>::expr] (c) ;
		rcx = C (rax) * C (rbx) ;
	}

	void sdiv (CR<Pointer> a ,CR<Pointer> b ,VR<Pointer> c) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rbx = keep[TYPE<B>::expr] (b) ;
		auto &&rcx = keep[TYPE<C>::expr] (c) ;
		rcx = C (rax) / C (rbx) ;
	}

	void sabs (CR<Pointer> a ,VR<Pointer> c) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rcx = keep[TYPE<C>::expr] (c) ;
		rcx = MathProc::abs (C (rax)) ;
	}

	void minus (CR<Pointer> a ,VR<Pointer> c) const override {
		auto &&rax = keep[TYPE<A>::expr] (a) ;
		auto &&rcx = keep[TYPE<C>::expr] (c) ;
		rcx = -C (rax) ;
	}
} ;

template <class A ,class B>
class TensorPairUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		if (uuid == ReflectTensorPairBinder<A ,B>::expr)
			return inline_vptr (ReflectTensorPairBinder<A ,B> ()) ;
		return ZERO ;
	}
} ;

struct TensorTree {
	RefBuffer<Byte> mTensor ;
	Index mCheck ;
} ;

class TensorImplHolder final implement Fat<TensorHolder ,TensorLayout> {
public:
	void initialize (RR<RefBufferLayout> that) override {
		self.mThis = SharedRef<TensorTree>::make () ;
		auto &&rax = keep[TYPE<RefBuffer<Byte>>::expr] (that) ;
		const auto r1x = rax.size () ;
		assert (address (rax.ref) % 16 == 0) ;
		self.mThis->mTensor = move (rax) ;
		self.mThis->mCheck = 0 ;
		self.mWidth = r1x ;
		self.mSpan = Array<Slice> (2) ;
		self.mSpan[0] = Slice (0 ,r1x ,1) ;
		self.mSpan[1] = Slice::one (0) ;
	}

	void initialize (CR<Length> size_ ,CR<Just<TensorType>> type_) override {
		noop (Array<TensorSlice0> ()) ;
		noop (Array<TensorSlice1> ()) ;
		noop (Array<TensorSlice2> ()) ;
		self.mThis = SharedRef<TensorTree>::make () ;
		const auto r1x = step_from_tensor_type (type_) ;
		const auto r2x = size_ * r1x + 16 ;
		self.mThis->mTensor = RefBuffer<Byte> (r2x) ;
		inline_memset (Pointer::from (self.mThis->mTensor.ref) ,r2x) ;
		if ifdo (TRUE) {
			auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mThis->mTensor) ;
			rax.mBuffer = inline_alignas (rax.mBuffer ,16) ;
			rax.mSize = size_ ;
			rax.mStep = r1x ;
		}
		self.mThis->mCheck = 0 ;
		self.mWidth = size_ ;
		self.mSpan = Array<Slice> (2) ;
		self.mSpan[0] = Slice (0 ,size_ ,1) ;
		self.mSpan[1] = Slice::one (0) ;
	}

	void initialize (CR<TensorLayout> that) override {
		self.mThis = that.mThis ;
		self.mWidth = that.mWidth ;
		self.mSpan = that.mSpan ;
	}

	Unknown choose_unknown (CR<Just<TensorType>> dst ,CR<Just<TensorType>> src) const {
		if (dst == TensorType::Flt32)
			if (src == TensorType::Flt32)
				return TensorPairUnknownBinder<Flt32 ,Flt32> () ;
		if (dst == TensorType::Flt32)
			if (src == TensorType::Flt64)
				return TensorPairUnknownBinder<Flt32 ,Flt64> () ;
		if (dst == TensorType::Flt64)
			if (src == TensorType::Flt32)
				return TensorPairUnknownBinder<Flt64 ,Flt32> () ;
		if (dst == TensorType::Flt64)
			if (src == TensorType::Flt64)
				return TensorPairUnknownBinder<Flt64 ,Flt64> () ;
		assume (FALSE) ;
		return Unknown (ZERO) ;
	}

	Length step_from_tensor_type (CR<Just<TensorType>> type_) const {
		if (type_ == TensorType::Flt32)
			return 4 ;
		if (type_ == TensorType::Flt64)
			return 8 ;
		if (type_ == TensorType::Flt128)
			return 16 ;
		assume (FALSE) ;
		return 0 ;
	}

	Just<TensorType> tensor_type_from_step (CR<Length> step_) const {
		if (step_ == 4)
			return TensorType::Flt32 ;
		if (step_ == 8)
			return TensorType::Flt64 ;
		if (step_ == 16)
			return TensorType::Flt128 ;
		assume (FALSE) ;
		return 0 ;
	}

	Length rank () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mSpan.length () - 1 ;
	}

	Length size () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mWidth ;
	}

	Just<TensorType> type () const override {
		if (!self.mThis.exist ())
			return TensorType::ETC ;
		return tensor_type_from_step (self.mThis->mTensor.step ()) ;
	}

	Array<Slice> shape () const override {
		return self.mSpan ;
	}

	Length shape (CR<Index> index) const override {
		assert (index >= 0) ;
		if (!self.mThis.exist ())
			return 0 ;
		Index ix = MathProc::min_of (index ,self.mSpan.length () - 1) ;
		return self.mSpan[ix].size () ;
	}

	Ref<RefBuffer<Byte>> borrow () const leftvalue override {
		assert (self.mThis.exist ()) ;
		assert (self.mThis->mTensor.exist ()) ;
		return Ref<RefBuffer<Byte>>::reference (self.mThis->mTensor) ;
	}

	void get (CR<Index> index ,VR<Flt64> item) const override {
		assert (self.mThis.exist ()) ;
		const auto r1x = MathProc::wrap (index ,self.mSpan[0].size ()) ;
		Index ix = self.mSpan[0].offset (r1x) ;
		for (auto &&i : range (1 ,self.mSpan.length ())) {
			ix += self.mSpan[i].offset (0) ;
		}
		const auto r2x = self.mThis->mTensor.step () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != 4)
				discard ;
			item = Flt32 (bitwise (self.mThis->mTensor[ix])) ;
		}
		if ifdo (act) {
			if (r2x != 8)
				discard ;
			item = Flt64 (bitwise (self.mThis->mTensor[ix])) ;
		}
		if ifdo (act) {
			if (r2x != 16)
				discard ;
			unimplemented () ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	TensorLayout share () const {
		TensorLayout ret ;
		TensorHolder::hold (ret)->initialize (self) ;
		return move (ret) ;
	}

	TensorLayout recast (CR<Just<TensorType>> type_) const override {
		if (type () == type_)
			return share () ;
		TensorLayout ret ;
		const auto r1x = self.mThis->mTensor.size () ;
		TensorHolder::hold (ret)->initialize (r1x ,type_) ;
		const auto r2x = choose_unknown (type_ ,type ()) ;
		const auto r3x = RFat<ReflectTensorPair> (r2x) ;
		auto &&rax = ret.mThis->mTensor.ref ;
		auto &&rbx = self.mThis->mTensor.ref ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r4x = address (rax[i]) ;
			const auto r5x = address (rbx[i]) ;
			r3x->clone (Pointer::make (r4x) ,Pointer::make (r5x)) ;
		}
		ret.mWidth = self.mWidth ;
		ret.mSpan = self.mSpan ;
		return move (ret) ;
	}

	TensorLayout reshape (CR<Wrapper<Length>> shape_) const override {
		const auto r1x = shape_.rank () ;
		assert (r1x > 0) ;
		TensorLayout ret ;
		ret.mThis = self.mThis ;
		ret.mWidth = self.mWidth ;
		const auto r2x = r1x + 1 ;
		ret.mSpan = Array<Slice> (r2x) ;
		auto rax = IDEN ;
		auto rbx = ZERO ;
		for (auto &&i : range (0 ,r1x))
			rax *= shape_[i] ;
		assume (TensorHolder::hold (self)->size () == rax) ;
		if ifdo (TRUE) {
			Index ix = self.mSpan.length () - 2 ;
			rax = self.mSpan[ix].step () ;
		}
		for (auto &&i : range (0 ,r1x)) {
			Index ix = r1x - 1 - i ;
			ret.mSpan[ix] = Slice (0 ,shape_[ix] ,rax) ;
			rax *= shape_[ix] ;
		}
		for (auto &&i : self.mSpan.iter ())
			rbx += self.mSpan[i].offset (0) ;
		if ifdo (TRUE) {
			Index ix = r2x - 1 ;
			ret.mSpan[ix] = Slice::one (0) ;
			ret.mSpan[0] = slice_add (ret.mSpan[0] ,rbx) ;
		}
		return move (ret) ;
	}

	TensorLayout span (CR<Wrapper<Slice>> shape_) const override {
		const auto r1x = shape_.rank () ;
		assert (r1x > 0) ;
		TensorLayout ret ;
		ret.mThis = self.mThis ;
		ret.mWidth = self.mWidth ;
		const auto r2x = rank () ;
		assert (r1x <= r2x) ;
		const auto r3x = invoke ([&] () {
			Length ret = 0 ;
			for (auto &&i : range (0 ,r1x)) {
				if (shape_[i].size () != 1)
					continue ;
				ret++ ;
			}
			return move (ret) ;
		}) ;
		const auto r4x = self.mSpan.length () - r3x ;
		ret.mSpan = Array<Slice> (r4x) ;
		auto rax = IDEN ;
		auto rbx = ZERO ;
		Index jx = 0 ;
		for (auto &&i : self.mSpan.iter ()) {
			const auto r5x = i < r1x ? shape_[i] : Slice::all () ;
			if ifdo (TRUE) {
				if (r5x.size () != 1)
					discard ;
				const auto r6x = MathProc::wrap (r5x.offset (0) ,self.mSpan[i].size ()) ;
				rbx += self.mSpan[i].offset (r6x) ;
			}
			if ifdo (TRUE) {
				if (r5x.size () == 1)
					discard ;
				assume (r5x.size () <= self.mSpan[i].size ()) ;
				const auto r7x = r5x.size () < 0 ? self.mSpan[i].size () : r5x.size () ;
				const auto r8x = self.mSpan[i].offset (0) + rbx ;
				const auto r9x = self.mSpan[i].step () ;
				ret.mSpan[jx] = Slice (r8x ,r7x ,r9x) ;
				jx++ ;
				rax *= r7x ;
				rbx = 0 ;
			}
		}
		assume (TensorHolder::hold (ret)->size () >= rax) ;
		if ifdo (TRUE) {
			if ifdo (TRUE) {
				if (jx >= r4x)
					discard ;
				ret.mSpan[jx] = Slice::one (0) ;
				jx++ ;
			}
			ret.mSpan[0] = slice_add (ret.mSpan[0] ,rbx) ;
		}
		ret.mWidth = rax ;
		return move (ret) ;
	}

	TensorLayout squeeze () const override {
		TensorLayout ret ;
		ret.mThis = self.mThis ;
		ret.mWidth = self.mWidth ;
		const auto r1x = rank () ;
		const auto r2x = invoke ([&] () {
			Length ret = 0 ;
			for (auto &&i : range (0 ,r1x)) {
				if (self.mSpan[i].size () != 1)
					continue ;
				ret++ ;
			}
			return move (ret) ;
		}) ;
		const auto r3x = self.mSpan.length () - r2x ;
		ret.mSpan = Array<Slice> (r3x) ;
		auto rbx = ZERO ;
		Index jx = 0 ;
		for (auto &&i : self.mSpan.iter ()) {
			const auto r5x = self.mSpan[i] ;
			if ifdo (TRUE) {
				if (r5x.size () != 1)
					discard ;
				rbx += r5x.offset (0) ;
			}
			if ifdo (TRUE) {
				if (r5x.size () == 1)
					discard ;
				ret.mSpan[jx] = slice_add (r5x ,rbx) ;
				jx++ ;
				rbx = 0 ;
			}
		}
		if ifdo (TRUE) {
			if ifdo (TRUE) {
				if (jx >= r3x)
					discard ;
				ret.mSpan[jx] = Slice::one (0) ;
				jx++ ;
			}
			ret.mSpan[0] = slice_add (ret.mSpan[0] ,rbx) ;
		}
		return move (ret) ;
	}

	Slice slice_add (CR<Slice> a ,CR<Index> b) const {
		return Slice (a.offset (0) + b ,a.size () ,a.step ()) ;
	}
} ;

exports VFat<TensorHolder> TensorHolder::hold (VR<TensorLayout> that) {
	return VFat<TensorHolder> (TensorImplHolder () ,that) ;
}

exports CFat<TensorHolder> TensorHolder::hold (CR<TensorLayout> that) {
	return CFat<TensorHolder> (TensorImplHolder () ,that) ;
}
} ;
