#pragma once

#ifndef __CSC_IMAGE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_image.hpp"

namespace CSC {
class ImageShapeImplHolder final implement Fat<ImageShapeHolder ,ImageShapeLayout> {
public:
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
		if (self.mStep != that.mStep)
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
		noop (r2x) ;
		assume (r2x == that.mImage.step ()) ;
		self = move (that) ;
	}

	void initialize (CR<Unknown> holder ,CR<Length> cx_ ,CR<Length> cy_ ,CR<Length> step_) override {
		assert (cx_ > 0) ;
		assert (cy_ > 0) ;
		const auto r1x = RFat<ReflectSize> (holder) ;
		const auto r2x = r1x->type_size () ;
		const auto r3x = cx_ * cy_ * step_ ;
		const auto r4x = inline_alignas (r3x ,r2x) / r2x ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mImage) ;
		RefBufferHolder::hold (rax)->initialize (holder ,r4x) ;
		rax.mSize = cx_ * cy_ ;
		rax.mStep = step_ ;
		self.mWidth = cx_ ;
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
		reset (0 ,0 ,self.mWidth ,r2x) ;
	}

	void reset (CR<Length> bx_ ,CR<Length> by_ ,CR<Length> cx_ ,CR<Length> cy_) override {
		assert (self.mImage.size () > 0) ;
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

struct ColorProcLayout {} ;

class ColorProcImplHolder final implement Fat<ColorProcHolder ,ColorProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Flt64 byte_norm (CR<Byte> c) const {
		return Flt64 (Val32 (c)) * MathProc::inverse (Flt64 (255)) ;
	}

	Flt64 byte_norm (CR<Word> c) const {
		return Flt64 (Val32 (c)) * MathProc::inverse (Flt64 (65535)) ;
	}

	Flt64 gray_from_bgr (CR<Color3B> a) const override {
		const auto r1x = byte_norm (a.mR) ;
		const auto r2x = byte_norm (a.mG) ;
		const auto r3x = byte_norm (a.mB) ;
		const auto r4x = 0.299 * r1x + 0.587 * r2x + 0.114 * r3x ;
		return MathProc::clamp (r4x ,0.0 ,1.0) ;
	}

	Color3B bgr_from_gray (CR<Flt64> a) const override {
		Color3B ret ;
		const auto r1x = MathProc::clamp (a ,0.0 ,1.0) * Flt64 (255) ;
		const auto r2x = Val32 (MathProc::round (r1x)) ;
		ret.mB = Byte (r2x) ;
		ret.mG = Byte (r2x) ;
		ret.mR = Byte (r2x) ;
		return move (ret) ;
	}

	Color3B jet_from_norm (CR<Flt64> a) const override {
		Color3B ret ;
		const auto r1x = MathProc::clamp (a ,0.0 ,1.0) * Flt64 (8 * 128) ;
		const auto r2x = Val32 (MathProc::round (r1x)) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x >= 128)
				discard ;
			ret.mR = Byte (128 + r2x) ;
			ret.mG = Byte (0) ;
			ret.mB = Byte (0) ;
		}
		if ifdo (act) {
			if (r2x >= 384)
				discard ;
			ret.mR = Byte (255) ;
			ret.mG = Byte (r2x - 128) ;
			ret.mB = Byte (0) ;
		}
		if ifdo (act) {
			if (r2x >= 640)
				discard ;
			ret.mR = ~Byte (r2x - 384) ;
			ret.mG = Byte (255) ;
			ret.mB = Byte (r2x - 384) ;
		}
		if ifdo (act) {
			if (r2x >= 896)
				discard ;
			ret.mR = Byte (0) ;
			ret.mG = ~Byte (r2x - 640) ;
			ret.mB = Byte (255) ;
		}
		if ifdo (act) {
			ret.mR = Byte (0) ;
			ret.mG = Byte (0) ;
			ret.mB = ~Byte (r2x - 896) ;
		}
		return move (ret) ;
	}

	Flt64 norm_from_jet (CR<Color3B> a) const override {
		const auto r1x = Val32 (a.mR) ;
		const auto r2x = Val32 (a.mG) ;
		const auto r3x = Val32 (a.mB) ;
		const auto r4x = MathProc::inverse (Flt64 (8 * 128)) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (a.mG != Byte (0))
				discard ;
			if (a.mB != Byte (0))
				discard ;
			const auto r5x = MathProc::max_of (r1x ,128) ;
			return (r5x - 128) * r4x ;
		}
		if ifdo (act) {
			if (a.mR != Byte (255))
				discard ;
			return (r2x + 128) * r4x ;
		}
		if ifdo (act) {
			if (a.mG != Byte (255))
				discard ;
			return (r3x + 384) * r4x ;
		}
		if ifdo (act) {
			if (a.mB != Byte (255))
				discard ;
			return (255 - r2x + 640) * r4x ;
		}
		if ifdo (act) {
			if (a.mR != Byte (0))
				discard ;
			if (a.mG != Byte (0))
				discard ;
			const auto r6x = MathProc::max_of (r3x ,127) ;
			return (255 - r6x + 896) * r4x ;
		}
		return 1 ;
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
			rax = r7x ;
		}
		if ifdo (act) {
			if (r5x != r1x)
				discard ;
			rax = (r2x - r3x) * r7x ;
			rax = MathProc::fmod (rax / 6) * 6 ;
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
		rax /= 360 ;
		rax += MathProc::step (-rax) ;
		const auto r8x = r6x * MathProc::inverse (r5x) ;
		ret.mB = Word (MathProc::lerp (rax ,Val32 (0) ,Val32 (65535))) ;
		ret.mG = Word (MathProc::lerp (r8x ,Val32 (0) ,Val32 (65535))) ;
		ret.mR = Word (MathProc::lerp (r5x ,Val32 (0) ,Val32 (65535))) ;
		return move (ret) ;
	}

	Color3B bgr_from_hsv (CR<Color3W> a) const override {
		Color3B ret ;
		const auto r1x = byte_norm (a.mB) * 360 ;
		const auto r2x = byte_norm (a.mG) ;
		const auto r3x = byte_norm (a.mR) ;
		const auto r4x = r3x * r2x ;
		const auto r5x = MathProc::fmod (r1x / 120) * 2 ;
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
		ret.mB = Byte (MathProc::lerp (rax[2] ,Val32 (0) ,Val32 (255))) ;
		ret.mG = Byte (MathProc::lerp (rax[1] ,Val32 (0) ,Val32 (255))) ;
		ret.mR = Byte (MathProc::lerp (rax[0] ,Val32 (0) ,Val32 (255))) ;
		return move (ret) ;
	}
} ;

exports CR<Like<UniqueRef<ColorProcLayout>>> ColorProcHolder::expr_m () {
	return memorize ([&] () {
		Like<UniqueRef<ColorProcLayout>> ret ;
		ret.mThis = UniqueRef<ColorProcLayout>::make () ;
		ColorProcHolder::hold (ret)->initialize () ;
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

exports CR<Like<UniqueRef<ImageProcLayout>>> ImageProcHolder::expr_m () {
	return memorize ([&] () {
		Like<UniqueRef<ImageProcLayout>> ret ;
		ret.mThis = UniqueRef<ImageProcLayout>::make () ;
		ImageProcHolder::hold (ret)->initialize () ;
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

class TensorImplHolder final implement Fat<TensorHolder ,TensorLayout> {
public:
	void initialize (RR<RefBufferLayout> that) override {
		unimplemented () ;
	}

	void initialize (CR<Length> size_ ,CR<Just<TensorType>> type_) override {
		const auto r1x = step_from_tensor_type (type_) ;
		const auto r2x = size_ * r1x + 16 ;
		self.mTensor = Ref<RefBuffer<Byte>>::make (r2x) ;
		inline_memset (Pointer::from (self.mTensor->ref) ,r2x) ;
		auto &&rax = keep[TYPE<RefBufferLayout>::expr] (self.mTensor.ref) ;
		rax.mBuffer = inline_alignas (rax.mBuffer ,16) ;
		rax.mSize = size_ ;
		rax.mStep = r1x ;
		self.mBuffer = rax.mBuffer ;
		self.mRank = 1 ;
		self.mStride[0] = r1x ;
		for (auto &&i : range (1 ,self.mStride.size ()))
			self.mStride[i] = size_ * r1x ;
		for (auto &&i : range (0 ,size_)) {
			const auto r3x = self.mBuffer + i * r1x ;
			auto &&rbx = keep[TYPE<Flt64>::expr] (Pointer::make (r3x)) ;
			rbx = Flt64 (i) ;
		}
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
		return 0 ;
	}

	Just<TensorType> tensor_type_from_step (CR<Length> step_) const {
		if (step_ == 4)
			return TensorType::Flt32 ;
		if (step_ == 8)
			return TensorType::Flt64 ;
		if (step_ == 16)
			return TensorType::Flt128 ;
		return 0 ;
	}

	Length size () const override {
		if (self.mTensor == NULL)
			return 0 ;
		return self.mStride[self.mRank] / self.mStride[0] ;
	}

	Just<TensorType> type () const override {
		if (self.mTensor == NULL)
			return TensorType::ETC ;
		return tensor_type_from_step (self.mStride[0]) ;
	}

	Length rank () const override {
		if (self.mTensor == NULL)
			return 0 ;
		return self.mRank ;
	}

	Length shape (CR<Index> index) const override {
		assert (index >= 0) ;
		if (self.mTensor == NULL)
			return 0 ;
		const auto r1x = self.mRank ;
		Index ix = inline_min (index ,r1x) ;
		Index iy = inline_min (index + 1 ,r1x) ;
		return self.mStride[iy] / self.mStride[ix] ;
	}

	TensorLayout share () const {
		TensorLayout ret ;
		ret.mTensor = self.mTensor.share () ;
		ret.mBuffer = self.mBuffer ;
		ret.mRank = self.mRank ;
		ret.mStride = self.mStride ;
		return move (ret) ;
	}

	TensorLayout recast (CR<Just<TensorType>> type_) const override {
		if (type () == type_)
			return share () ;
		TensorLayout ret ;
		TensorHolder::hold (ret)->initialize (size () ,type_) ;
		const auto r1x = choose_unknown (type_ ,type ()) ;
		const auto r2x = RFat<ReflectTensorPair> (r1x) ;
		const auto r3x = self.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r4x = ret.mBuffer + i * r3x ;
			const auto r5x = self.mBuffer + i * r3x ;
			r2x->clone (Pointer::make (r4x) ,Pointer::make (r5x)) ;
		}
		ret.mRank = self.mRank ;
		ret.mStride = self.mStride ;
		return move (ret) ;
	}


	TensorLayout reshape () const override {
		if (self.mTensor == NULL)
			return share () ;
		return reshape (MakeWrapper (size ())) ;
	}

	TensorLayout reshape (CR<Wrapper<Length>> shape_) const override {
		assert (self.mTensor != NULL) ;
		assert (shape_.rank () > 0) ;
		const auto r1x = self.mStride.size () - 1 ;
		assert (shape_.rank () <= r1x) ;
		TensorLayout ret = share () ;
		const auto r2x = inline_min (shape_.rank () ,r1x) ;
		ret.mRank = r2x ;
		for (auto &&i : range (1 ,r2x + 1)) {
			Index ix = i - 1 ;
			Index iy = r2x - i ;
			ret.mStride[i] = ret.mStride[ix] * shape_[iy] ;
		}
		for (auto &&i : range (r2x + 1 ,r1x + 1)) {
			Index ix = i - 1 ;
			ret.mStride[i] = ret.mStride[ix] ;
		}
		assume (TensorHolder::hold (ret)->size () == size ()) ;
		return move (ret) ;
	}

	Ref<RefBuffer<Byte>> borrow () const leftvalue override {
		return self.mTensor.share () ;
	}

	Flt64 get_float (CR<Flag> addr) const {
		if (self.mStride[0] == 4)
			return bitwise[TYPE<Flt32>::expr] (Pointer::make (addr)) ;
		if (self.mStride[0] == 8)
			return bitwise[TYPE<Flt64>::expr] (Pointer::make (addr)) ;
		assert (FALSE) ;
		return 0 ;
	}

	void get (CR<Index> i0 ,VR<Flt64> item) const override {
		const auto r1x = i0 * self.mStride[0] ;
		assert (inline_between (r1x ,0 ,self.mStride[4])) ;
		const auto r2x = self.mBuffer + r1x ;
		item = get_float (r2x) ;
	}

	void get (CR<Index> i0 ,CR<Index> i1 ,VR<Flt64> item) const override {
		const auto r1x = i1 * self.mStride[0] ;
		assert (inline_between (r1x ,0 ,self.mStride[1])) ;
		const auto r2x = i0 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[4])) ;
		const auto r3x = self.mBuffer + r1x + r2x ;
		item = get_float (r3x) ;
	}

	void get (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,VR<Flt64> item) const override {
		const auto r1x = i2 * self.mStride[0] ;
		assert (inline_between (r1x ,0 ,self.mStride[1])) ;
		const auto r2x = i1 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[2])) ;
		const auto r3x = i0 * self.mStride[2] ;
		assert (inline_between (r1x ,0 ,self.mStride[4])) ;
		const auto r4x = self.mBuffer + r1x + r2x + r3x ;
		item = get_float (r4x) ;
	}

	void get (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,CR<Index> i3 ,VR<Flt64> item) const override {
		const auto r1x = i3 * self.mStride[0] ;
		assert (inline_between (r1x ,0 ,self.mStride[1])) ;
		const auto r2x = i2 * self.mStride[1] ;
		assert (inline_between (r1x ,0 ,self.mStride[2])) ;
		const auto r3x = i1 * self.mStride[2] ;
		assert (inline_between (r1x ,0 ,self.mStride[3])) ;
		const auto r4x = i0 * self.mStride[3] ;
		assert (inline_between (r1x ,0 ,self.mStride[4])) ;
		const auto r5x = self.mBuffer + r1x + r2x + r3x + r4x ;
		item = get_float (r5x) ;
	}

	TensorLayout sadd (CR<TensorLayout> that) const override {
		TensorLayout ret ;
		const auto r1x = size () ;
		const auto r2x = TensorHolder::hold (that)->size () ;
		assume (r1x == r2x) ;
		const auto r3x = choose_unknown (type () ,TensorHolder::hold (that)->type ()) ;
		const auto r4x = RFat<ReflectTensorPair> (r3x) ;
		TensorHolder::hold (ret)->initialize (size () ,r4x->type ()) ;
		const auto r5x = self.mStride[0] ;
		const auto r6x = that.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r7x = self.mBuffer + i * r5x ;
			const auto r8x = that.mBuffer + i * r6x ;
			const auto r9x = ret.mBuffer + i * r5x ;
			r4x->sadd (Pointer::make (r7x) ,Pointer::make (r8x) ,Pointer::make (r9x)) ;
		}
		return move (ret) ;
	}

	TensorLayout ssub (CR<TensorLayout> that) const override {
		TensorLayout ret ;
		const auto r1x = size () ;
		const auto r2x = TensorHolder::hold (that)->size () ;
		assume (r1x == r2x) ;
		const auto r3x = choose_unknown (type () ,TensorHolder::hold (that)->type ()) ;
		const auto r4x = RFat<ReflectTensorPair> (r3x) ;
		TensorHolder::hold (ret)->initialize (size () ,r4x->type ()) ;
		const auto r5x = self.mStride[0] ;
		const auto r6x = that.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r7x = self.mBuffer + i * r5x ;
			const auto r8x = that.mBuffer + i * r6x ;
			const auto r9x = ret.mBuffer + i * r5x ;
			r4x->ssub (Pointer::make (r7x) ,Pointer::make (r8x) ,Pointer::make (r9x)) ;
		}
		return move (ret) ;
	}

	TensorLayout smul (CR<TensorLayout> that) const override {
		TensorLayout ret ;
		const auto r1x = size () ;
		const auto r2x = TensorHolder::hold (that)->size () ;
		assume (r1x == r2x) ;
		const auto r3x = choose_unknown (type () ,TensorHolder::hold (that)->type ()) ;
		const auto r4x = RFat<ReflectTensorPair> (r3x) ;
		TensorHolder::hold (ret)->initialize (size () ,r4x->type ()) ;
		const auto r5x = self.mStride[0] ;
		const auto r6x = that.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r7x = self.mBuffer + i * r5x ;
			const auto r8x = that.mBuffer + i * r6x ;
			const auto r9x = ret.mBuffer + i * r5x ;
			r4x->smul (Pointer::make (r7x) ,Pointer::make (r8x) ,Pointer::make (r9x)) ;
		}
		return move (ret) ;
	}

	TensorLayout sdiv (CR<TensorLayout> that) const override {
		TensorLayout ret ;
		const auto r1x = size () ;
		const auto r2x = TensorHolder::hold (that)->size () ;
		assume (r1x == r2x) ;
		const auto r3x = choose_unknown (type () ,TensorHolder::hold (that)->type ()) ;
		const auto r4x = RFat<ReflectTensorPair> (r3x) ;
		TensorHolder::hold (ret)->initialize (size () ,r4x->type ()) ;
		const auto r5x = self.mStride[0] ;
		const auto r6x = that.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r7x = self.mBuffer + i * r5x ;
			const auto r8x = that.mBuffer + i * r6x ;
			const auto r9x = ret.mBuffer + i * r5x ;
			r4x->sdiv (Pointer::make (r7x) ,Pointer::make (r8x) ,Pointer::make (r9x)) ;
		}
		return move (ret) ;
	}

	TensorLayout sabs () const override {
		TensorLayout ret ;
		const auto r1x = choose_unknown (type () ,type ()) ;
		const auto r2x = RFat<ReflectTensorPair> (r1x) ;
		TensorHolder::hold (ret)->initialize (size () ,r2x->type ()) ;
		const auto r3x = self.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r4x = self.mBuffer + i * r3x ;
			const auto r5x = ret.mBuffer + i * r3x ;
			r2x->sabs (Pointer::make (r4x) ,Pointer::make (r5x)) ;
		}
		return move (ret) ;
	}

	TensorLayout minus () const override {
		TensorLayout ret ;
		const auto r1x = choose_unknown (type () ,type ()) ;
		const auto r2x = RFat<ReflectTensorPair> (r1x) ;
		TensorHolder::hold (ret)->initialize (size () ,r2x->type ()) ;
		const auto r3x = self.mStride[0] ;
		for (auto &&i : range (0 ,size ())) {
			const auto r4x = self.mBuffer + i * r3x ;
			const auto r5x = ret.mBuffer + i * r3x ;
			r2x->minus (Pointer::make (r4x) ,Pointer::make (r5x)) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<TensorHolder> TensorHolder::hold (VR<TensorLayout> that) {
	return VFat<TensorHolder> (TensorImplHolder () ,that) ;
}

exports CFat<TensorHolder> TensorHolder::hold (CR<TensorLayout> that) {
	return CFat<TensorHolder> (TensorImplHolder () ,that) ;
}

class DisjointImplHolder final implement Fat<DisjointHolder ,DisjointLayout> {
public:
	void initialize (CR<Length> size_) override {
		self.mTable = Array<Index> (size_) ;
		self.mTable.fill (NONE) ;
	}

	Length size () const override {
		return self.mTable.size () ;
	}

	Index lead (CR<Index> from_) override {
		Index ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ix = parent (ix) ;
		}
		Index ret = ix ;
		if ifdo (TRUE) {
			if (ix == NONE)
				discard ;
			ix = from_ ;
			Index iy = NONE ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				iy = parent (ix) ;
				self.mTable[ix] = ret ;
				ix = iy ;
			}
		}
		return move (ret) ;
	}

	Index parent (CR<Index> curr) const {
		if (curr == self.mTable[curr])
			return NONE ;
		return self.mTable[curr] ;
	}

	void joint (CR<Index> from_ ,CR<Index> to_) override {
		Index ix = lead (from_) ;
		Index iy = lead (to_) ;
		self.mTable[ix] = ix ;
		self.mTable[iy] = ix ;
	}

	Bool edge (CR<Index> from_ ,CR<Index> to_) override {
		Index ix = lead (from_) ;
		Index iy = lead (to_) ;
		return ix == iy ;
	}

	Length depth (CR<Index> from_) override {
		Length ret = 0 ;
		Index ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ret++ ;
			ix = parent (ix) ;
		}
		return move (ret) ;
	}

	Deque<Index> cluster (CR<Index> from_) override {
		Deque<Index> ret ;
		Index ix = from_ ;
		while (TRUE) {
			if (ix == NONE)
				break ;
			ret.add (ix) ;
			ix = parent (ix) ;
		}
		return move (ret) ;
	}

	Array<Index> jump (CR<Index> from_) override {
		Array<Index> ret = Array<Index> (self.mTable.size ()) ;
		ret.fill (NONE) ;
		for (auto &&i : range (0 ,self.mTable.size ())) {
			Index ix = lead (i) ;
			if (ix == NONE)
				continue ;
			ret[ix] = ret[i] ;
			ret[i] = ix ;
		}
		return move (ret) ;
	}
} ;

exports VFat<DisjointHolder> DisjointHolder::hold (VR<DisjointLayout> that) {
	return VFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

exports CFat<DisjointHolder> DisjointHolder::hold (CR<DisjointLayout> that) {
	return CFat<DisjointHolder> (DisjointImplHolder () ,that) ;
}

class KMMatchImplHolder final implement Fat<KMMatchHolder ,KMMatchLayout> {
public:
	void initialize (CR<Length> size_) override {
		self.mSize = size_ ;
		self.mThreshold = Flt32 (0.1) ;
		self.mUser = Array<Flt32> (self.mSize) ;
		self.mWork = Array<Flt32> (self.mSize) ;
		self.mUserVisit = BitSet (self.mSize) ;
		self.mWorkVisit = BitSet (self.mSize) ;
		self.mMatch = Array<Index> (self.mSize) ;
		self.mLack = Array<Flt32> (self.mSize) ;
	}

	void set_threshold (CR<Flt64> threshold) override {
		self.mThreshold = Flt32 (threshold) ;
	}

	Length size () const override {
		return self.mSize ;
	}

	Array<Index> sort (CR<Image<Flt32>> love) override {
		assert (self.mMatch.size () > 0) ;
		assert (love.size () == MathProc::square (self.mSize)) ;
		self.mLove = Ref<Image<Flt32>>::reference (love) ;
		self.mUser.fill (0) ;
		self.mWork.fill (0) ;
		self.mUserVisit.clear () ;
		self.mWorkVisit.clear () ;
		self.mMatch.fill (NONE) ;
		self.mLack.fill (0) ;
		solve () ;
		return self.mMatch ;
	}

	void solve () {
		for (auto &&i : range (0 ,self.mSize)) {
			self.mUser[i] = -infinity ;
			for (auto &&j : range (0 ,self.mSize)) {
				const auto r1x = self.mLove.ref[i][j] ;
				self.mUser[i] = MathProc::max_of (self.mUser[i] ,r1x) ;
			}
		}
		for (auto &&i : range (0 ,self.mSize)) {
			self.mLack.fill (infinity) ;
			while (TRUE) {
				self.mUserVisit.clear () ;
				self.mWorkVisit.clear () ;
				if (dfs (i))
					break ;
				const auto r2x = invoke ([&] () {
					Flt32 ret = infinity ;
					for (auto &&j : range (0 ,self.mSize)) {
						if (self.mWorkVisit[j])
							continue ;
						ret = MathProc::min_of (ret ,self.mLack[j]) ;
					}
					return move (ret) ;
				}) ;
				for (auto &&j : range (0 ,self.mSize)) {
					if ifdo (TRUE) {
						if (!self.mUserVisit[j])
							discard ;
						self.mUser[j] -= r2x ;
					}
					if ifdo (TRUE) {
						if (!self.mWorkVisit[j])
							discard ;
						self.mWork[j] += r2x ;
					}
					if ifdo (TRUE) {
						if (self.mWorkVisit[j])
							discard ;
						self.mLack[j] -= r2x ;
					}
				}
			}
		}
	}

	Bool dfs (CR<Index> user) {
		self.mUserVisit[user] = TRUE ;
		for (auto &&i : range (0 ,self.mSize)) {
			if (self.mWorkVisit[i])
				continue ;
			const auto r1x = self.mLove.ref[user][i] ;
			const auto r2x = self.mUser[user] + self.mWork[i] - r1x ;
			if ifdo (TRUE) {
				if (r2x < self.mThreshold)
					discard ;
				self.mLack[i] = MathProc::min_of (self.mLack[i] ,r2x) ;
			}
			if (r2x >= self.mThreshold)
				continue ;
			self.mWorkVisit[i] = TRUE ;
			const auto r3x = self.mMatch[i] ;
			if ifdo (TRUE) {
				if (r3x != NONE)
					discard ;
				self.mMatch[i] = user ;
				return TRUE ;
			}
			if ifdo (TRUE) {
				if (!dfs (r3x))
					discard ;
				self.mMatch[i] = user ;
				return TRUE ;
			}
		}
		return FALSE ;
	}
} ;

exports VFat<KMMatchHolder> KMMatchHolder::hold (VR<KMMatchLayout> that) {
	return VFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}

exports CFat<KMMatchHolder> KMMatchHolder::hold (CR<KMMatchLayout> that) {
	return CFat<KMMatchHolder> (KMMatchImplHolder () ,that) ;
}
} ;
