#pragma once

#ifndef __CSC_IMAGE__
#define __CSC_IMAGE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"

namespace CSC {
template <class A>
class RowProxy implement Proxy {
private:
	using ITEM = decltype (nullof (A).at (0 ,0)) ;

protected:
	XR<A> mThat ;
	Index mY ;

public:
	implicit RowProxy () = delete ;

	explicit RowProxy (XR<A> that ,CR<Index> y) :mThat (that) {
		mY = y ;
	}

	forceinline XR<ITEM> operator[] (CR<Index> x) rightvalue {
		return mThat.at (x ,mY) ;
	}
} ;

struct ImageShapeLayout {
	Length mBX ;
	Length mBY ;
	Length mCX ;
	Length mCY ;
	Length mStep ;

public:
	implicit ImageShapeLayout () noexcept {
		mBX = 0 ;
		mBY = 0 ;
		mCX = 0 ;
		mCY = 0 ;
		mStep = 0 ;
	}
} ;

class ImageShape implement ImageShapeLayout {
public:
	implicit ImageShape () = default ;

	Length size () const {
		return mCX * mCY ;
	}

	Bool equal (CR<ImageShape> that) const {
		if (mBX != that.mBX)
			return FALSE ;
		if (mBY != that.mBY)
			return FALSE ;
		if (mCX != that.mCX)
			return FALSE ;
		if (mCY != that.mCY)
			return FALSE ;
		return TRUE ;
	}

	forceinline Bool operator== (CR<ImageShape> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<ImageShape> that) const {
		return (!equal (that)) ;
	}
} ;

struct ImageLayout {
	RefBuffer<Pointer> mImage ;
	Length mWidth ;
	Length mStride ;
	Length mBX ;
	Length mBY ;
	Length mCX ;
	Length mCY ;
} ;

struct ImageHolder implement Interface {
	imports VFat<ImageHolder> hold (VR<ImageLayout> that) ;
	imports CFat<ImageHolder> hold (CR<ImageLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,RR<ImageLayout> that) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<Length> cx_ ,CR<Length> cy_ ,CR<Length> step_) = 0 ;
	virtual void initialize (CR<ImageLayout> that) = 0 ;
	virtual Bool fixed () const = 0 ;
	virtual Length size () const = 0 ;
	virtual Length step () const = 0 ;
	virtual Length stride () const = 0 ;
	virtual Bool continous () const = 0 ;
	virtual Length bx () const = 0 ;
	virtual Length by () const = 0 ;
	virtual Length cx () const = 0 ;
	virtual Length cy () const = 0 ;
	virtual ImageShape shape () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<Length> bx_ ,CR<Length> by_ ,CR<Length> cx_ ,CR<Length> cy_) = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual VR<Pointer> at (CR<Index> x ,CR<Index> y) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<Index> x ,CR<Index> y) const leftvalue = 0 ;
	virtual void fill (CR<Pointer> item) = 0 ;
	virtual void splice (CR<Index> x ,CR<Index> y ,CR<ImageLayout> item) = 0 ;
} ;

template <class A>
struct ImageImplLayout implement ImageLayout {
public:
	implicit ImageImplLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		ImageHolder::hold (thiz)->prepare (ArrayUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ImageImplLayout<Pointer> implement ImageLayout {} ;

template <class A>
class Image implement ImageImplLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	using ImageImplLayout<A>::mImage ;
	using ImageImplLayout<A>::mWidth ;
	using ImageImplLayout<A>::mStride ;
	using ImageImplLayout<A>::mBX ;
	using ImageImplLayout<A>::mBY ;
	using ImageImplLayout<A>::mCX ;
	using ImageImplLayout<A>::mCY ;

public:
	implicit Image () = default ;

	implicit Image (RR<ImageLayout> that) {
		ImageHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,move (that)) ;
	}

	explicit Image (CR<ImageShape> shape_) {
		ImageHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,shape_.mCX ,shape_.mCY ,SIZE_OF<A>::expr) ;
	}

	explicit Image (CR<Length> cx_ ,CR<Length> cy_) {
		ImageHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,cx_ ,cy_ ,SIZE_OF<A>::expr) ;
	}

	implicit Image (CR<Image> that) {
		ImageHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Image> operator= (CR<Image> that) {
		return assign (thiz ,that) ;
	}

	implicit Image (RR<Image> that) = default ;

	forceinline VR<Image> operator= (RR<Image> that) = default ;

	Image clone () const {
		return move (thiz) ;
	}

	Bool fixed () const {
		return ImageHolder::hold (thiz)->fixed () ;
	}

	Length size () const {
		return ImageHolder::hold (thiz)->size () ;
	}

	Length step () const {
		return ImageHolder::hold (thiz)->step () ;
	}

	Length stride () const {
		return ImageHolder::hold (thiz)->stride () ;
	}

	Bool continous () const {
		return ImageHolder::hold (thiz)->continous () ;
	}

	Length bx () const {
		return ImageHolder::hold (thiz)->bx () ;
	}

	Length by () const {
		return ImageHolder::hold (thiz)->by () ;
	}

	Length cx () const {
		return ImageHolder::hold (thiz)->cx () ;
	}

	Length cy () const {
		return ImageHolder::hold (thiz)->cy () ;
	}

	ImageShape shape () const {
		return ImageHolder::hold (thiz)->shape () ;
	}

	void reset () {
		return ImageHolder::hold (thiz)->reset () ;
	}

	void reset (CR<ImageShape> shape_) {
		return ImageHolder::hold (thiz)->reset (shape_.mBX ,shape_.mBY ,shape_.mCX ,shape_.mCY) ;
	}

	void reset (CR<Length> bx_ ,CR<Length> by_ ,CR<Length> cx_ ,CR<Length> cy_) {
		return ImageHolder::hold (thiz)->reset (bx_ ,by_ ,cx_ ,cy_) ;
	}

	VR<A> at (CR<Index> x ,CR<Index> y) leftvalue {
		return ImageHolder::hold (thiz)->at (x ,y) ;
	}

	VR<A> at (CR<Pixel> index) leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline VR<A> operator[] (CR<Pixel> index) leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<VR<Image>> operator[] (CR<Index> y) leftvalue {
		return RowProxy<VR<Image>> (thiz ,y) ;
	}

	VR<BoxLayout> raw () leftvalue {
		return ImageHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return ImageHolder::hold (thiz)->raw () ;
	}

	CR<A> at (CR<Index> x ,CR<Index> y) const leftvalue {
		return ImageHolder::hold (thiz)->at (x ,y) ;
	}

	CR<A> at (CR<Pixel> index) const leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline CR<A> operator[] (CR<Pixel> index) const leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<CR<Image>> operator[] (CR<Index> y) const leftvalue {
		return RowProxy<CR<Image>> (thiz ,y) ;
	}

	PixelIterator iter () const {
		return PixelIterator (0 ,cx () ,0 ,cy ()) ;
	}

	void fill (CR<A> item) {
		return ImageHolder::hold (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CR<Index> x ,CR<Index> y ,CR<Image> item) {
		return ImageHolder::hold (thiz)->splice (x ,y ,item) ;
	}

	void splice (CR<Pixel> index ,CR<Image> item) {
		return splice (index.mX ,index.mY ,item) ;
	}
} ;

struct Color1B {
	Byte mB ;
} ;

struct Color1W {
	Word mB ;
} ;

struct Color2B {
	Byte mB ;
	Byte mG ;
} ;

struct Color2W {
	Word mB ;
	Word mG ;
} ;

struct Color3B {
	Byte mB ;
	Byte mG ;
	Byte mR ;
} ;

struct Color3W {
	Word mB ;
	Word mG ;
	Word mR ;
} ;

struct Color4B {
	Byte mB ;
	Byte mG ;
	Byte mR ;
	Byte mA ;
} ;

struct Color4W {
	Word mB ;
	Word mG ;
	Word mR ;
	Word mA ;
} ;

static constexpr auto COLOR_BLACK = Color3B ({Byte (0X00) ,Byte (0X00) ,Byte (0X00)}) ;
static constexpr auto COLOR_WHITE = Color3B ({Byte (0XFF) ,Byte (0XFF) ,Byte (0XFF)}) ;
static constexpr auto COLOR_GRAY = Color3B ({Byte (0X80) ,Byte (0X80) ,Byte (0X80)}) ;
static constexpr auto COLOR_RED = Color3B ({Byte (0X00) ,Byte (0X00) ,Byte (0XFF)}) ;
static constexpr auto COLOR_GREEN = Color3B ({Byte (0X00) ,Byte (0XFF) ,Byte (0X00)}) ;
static constexpr auto COLOR_BLUE = Color3B ({Byte (0XFF) ,Byte (0X00) ,Byte (0X00)}) ;
static constexpr auto COLOR_YELLOW = Color3B ({Byte (0X00) ,Byte (0XFF) ,Byte (0XFF)}) ;
static constexpr auto COLOR_PURPLE = Color3B ({Byte (0XFF) ,Byte (0X00) ,Byte (0XFF)}) ;
static constexpr auto COLOR_CYAN = Color3B ({Byte (0XFF) ,Byte (0XFF) ,Byte (0X00)}) ;

struct ColorProcLayout ;

struct ColorProcHolder implement Interface {
	imports CR<Like<UniqueRef<ColorProcLayout>>> expr_m () ;
	imports VFat<ColorProcHolder> hold (VR<ColorProcLayout> that) ;
	imports CFat<ColorProcHolder> hold (CR<ColorProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Flt64 gray_from_bgr (CR<Color3B> a) const = 0 ;
	virtual Color3B bgr_from_gray (CR<Flt64> a) const = 0 ;
	virtual Color3B jet_from_norm (CR<Flt64> a) const = 0 ;
	virtual Flt64 norm_from_jet (CR<Color3B> a) const = 0 ;
	virtual Color3W hsv_from_bgr (CR<Color3B> a) const = 0 ;
	virtual Color3B bgr_from_hsv (CR<Color3W> a) const = 0 ;
} ;

class ColorProc implement Like<UniqueRef<ColorProcLayout>> {
public:
	static CR<ColorProc> expr_m () {
		return keep[TYPE<ColorProc>::expr] (ColorProcHolder::expr) ;
	}

	imports Flt64 gray_from_bgr (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->gray_from_bgr (a) ;
	}

	imports Color3B bgr_from_gray (CR<Flt64> a) {
		return ColorProcHolder::hold (expr)->bgr_from_gray (a) ;
	}

	imports Color3B jet_from_norm (CR<Flt64> a) {
		return ColorProcHolder::hold (expr)->jet_from_norm (a) ;
	}

	imports Flt64 norm_from_jet (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->norm_from_jet (a) ;
	}

	imports Color3W hsv_from_bgr (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->hsv_from_bgr (a) ;
	}

	imports Color3B bgr_from_hsv (CR<Color3W> a) {
		return ColorProcHolder::hold (expr)->bgr_from_hsv (a) ;
	}
} ;

struct ImageProcLayout ;

struct ImageProcHolder implement Interface {
	imports CR<Like<UniqueRef<ImageProcLayout>>> expr_m () ;
	imports VFat<ImageProcHolder> hold (VR<ImageProcLayout> that) ;
	imports CFat<ImageProcHolder> hold (CR<ImageProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual ImageLayout make_image (RR<BoxLayout> image) const = 0 ;
	virtual ImageLayout make_image (CR<ImageShape> shape) const = 0 ;
	virtual ImageLayout make_image (CR<ImageShape> shape ,CR<Clazz> clazz ,CR<Length> channel) const = 0 ;
	virtual VR<Pointer> peek_image (VR<ImageLayout> image) const = 0 ;
	virtual CR<Pointer> peek_image (CR<ImageLayout> image) const = 0 ;
	virtual ImageLayout load_image (CR<String<Str>> file) const = 0 ;
	virtual void save_image (CR<String<Str>> file ,CR<ImageLayout> image) const = 0 ;
	virtual Color1B sampler (CR<Image<Color1B>> image ,CR<Flt64> x ,CR<Flt64> y) const = 0 ;
	virtual Color2B sampler (CR<Image<Color2B>> image ,CR<Flt64> x ,CR<Flt64> y) const = 0 ;
	virtual Color3B sampler (CR<Image<Color3B>> image ,CR<Flt64> x ,CR<Flt64> y) const = 0 ;
	virtual Color4B sampler (CR<Image<Color4B>> image ,CR<Flt64> x ,CR<Flt64> y) const = 0 ;
	virtual Flt32 sampler (CR<Image<Flt32>> image ,CR<Flt64> x ,CR<Flt64> y) const = 0 ;
	virtual Flt64 sampler (CR<Image<Flt64>> image ,CR<Flt64> x ,CR<Flt64> y) const = 0 ;
} ;

class ImageProc implement Like<UniqueRef<ImageProcLayout>> {
public:
	static CR<ImageProc> expr_m () {
		return keep[TYPE<ImageProc>::expr] (ImageProcHolder::expr) ;
	}

	static ImageLayout make_image (RR<BoxLayout> image) {
		return ImageProcHolder::hold (expr)->make_image (move (image)) ;
	}

	static ImageLayout make_image (CR<ImageShape> shape) {
		return ImageProcHolder::hold (expr)->make_image (shape) ;
	}

	static ImageLayout make_image (CR<ImageShape> shape ,CR<Clazz> clazz ,CR<Length> channel) {
		return ImageProcHolder::hold (expr)->make_image (shape ,clazz ,channel) ;
	}

	template <class A>
	static VR<A> peek_image (VR<ImageLayout> image ,TYPE<A>) {
		return ImageProcHolder::hold (expr)->peek_image (image) ;
	}

	template <class A>
	static CR<A> peek_image (CR<ImageLayout> image ,TYPE<A>) {
		return ImageProcHolder::hold (expr)->peek_image (image) ;
	}

	static ImageLayout load_image (CR<String<Str>> file) {
		return ImageProcHolder::hold (expr)->load_image (file) ;
	}

	static void save_image (CR<String<Str>> file ,CR<ImageLayout> image) {
		return ImageProcHolder::hold (expr)->save_image (file ,image) ;
	}

	template <class ARG1>
	static ARG1 sampler (CR<Image<ARG1>> image ,CR<Flt64> x ,CR<Flt64> y) {
		return ImageProcHolder::hold (expr)->sampler (image ,x ,y) ;
	}
} ;

struct TensorType {
	enum {
		Flt32 ,
		Flt64 ,
		Flt128 ,
		ETC
	} ;
} ;

struct TensorLayout {
	Ref<RefBuffer<Byte>> mTensor ;
	Flag mHolder ;
	Flag mBuffer ;
	Length mRank ;
	Buffer5<Length> mStride ;
} ;

struct TensorHolder implement Interface {
	imports VFat<TensorHolder> hold (VR<TensorLayout> that) ;
	imports CFat<TensorHolder> hold (CR<TensorLayout> that) ;

	virtual void initialize (RR<RefBufferLayout> that) = 0 ;
	virtual void initialize (CR<Length> size_ ,CR<Just<TensorType>> type_) = 0 ;
	virtual Length size () const = 0 ;
	virtual Just<TensorType> type () const = 0 ;
	virtual Length rank () const = 0 ;
	virtual Length shape (CR<Index> index) const = 0 ;
	virtual TensorLayout recast (CR<Just<TensorType>> type_) const = 0 ;
	virtual TensorLayout reshape () const = 0 ;
	virtual TensorLayout reshape (CR<Wrapper<Length>> shape_) const = 0 ;
	virtual Ref<RefBuffer<Byte>> borrow () const leftvalue = 0 ;
	virtual void get (CR<Index> i0 ,VR<Flt64> item) const = 0 ;
	virtual void get (CR<Index> i0 ,CR<Index> i1 ,VR<Flt64> item) const = 0 ;
	virtual void get (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,VR<Flt64> item) const = 0 ;
	virtual void get (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,CR<Index> i3 ,VR<Flt64> item) const = 0 ;
	virtual TensorLayout sadd (CR<TensorLayout> that) const = 0 ;
	virtual TensorLayout ssub (CR<TensorLayout> that) const = 0 ;
	virtual TensorLayout smul (CR<TensorLayout> that) const = 0 ;
	virtual TensorLayout sdiv (CR<TensorLayout> that) const = 0 ;
	virtual TensorLayout sabs () const = 0 ;
	virtual TensorLayout minus () const = 0 ;
} ;

class Tensor implement TensorLayout {
protected:
	using TensorLayout::mTensor ;
	using TensorLayout::mHolder ;
	using TensorLayout::mBuffer ;
	using TensorLayout::mRank ;
	using TensorLayout::mStride ;

public:
	implicit Tensor () = default ;

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	implicit Tensor (RR<RefBuffer<ARG1>> that) {
		TensorHolder::hold (thiz)->initialize (move (that)) ;
	}

	explicit Tensor (CR<Length> size_ ,CR<Just<TensorType>> type_) {
		TensorHolder::hold (thiz)->initialize (size_ ,type_) ;
	}

	Length size () const {
		return TensorHolder::hold (thiz)->size () ;
	}

	Just<TensorType> type () const {
		return TensorHolder::hold (thiz)->type () ;
	}

	Length rank () const {
		return TensorHolder::hold (thiz)->rank () ;
	}

	Length shape (CR<Index> index) const {
		return TensorHolder::hold (thiz)->shape (index) ;
	}

	Tensor recast (CR<Just<TensorType>> type_) const {
		TensorLayout ret = TensorHolder::hold (thiz)->recast (type_) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	Tensor reshape () const {
		TensorLayout ret = TensorHolder::hold (thiz)->reshape () ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	template <class...ARG1>
	Tensor reshape (CR<ARG1>...shape_) const {
		TensorLayout ret = TensorHolder::hold (thiz)->reshape (MakeWrapper (Length (shape_)...)) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	Ref<RefBuffer<Byte>> borrow () const leftvalue {
		return TensorHolder::hold (thiz)->borrow () ;
	}

	void get (CR<Index> i0 ,VR<Flt64> item) const {
		return TensorHolder::hold (thiz)->get (i0 ,item) ;
	}

	inline Flt64 operator() (CR<Index> i0) const {
		Flt64 ret ;
		get (i0 ,ret) ;
		return move (ret) ;
	}

	void get (CR<Index> i0 ,CR<Index> i1 ,VR<Flt64> item) const {
		return TensorHolder::hold (thiz)->get (i0 ,i1 ,item) ;
	}

	inline Flt64 operator() (CR<Index> i0 ,CR<Index> i1) const {
		Flt64 ret ;
		get (i0 ,i1 ,ret) ;
		return move (ret) ;
	}

	void get (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,VR<Flt64> item) const {
		return TensorHolder::hold (thiz)->get (i0 ,i1 ,i2 ,item) ;
	}

	inline Flt64 operator() (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2) const {
		Flt64 ret ;
		get (i0 ,i1 ,i2 ,ret) ;
		return move (ret) ;
	}

	void get (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,CR<Index> i3 ,VR<Flt64> item) const {
		return TensorHolder::hold (thiz)->get (i0 ,i1 ,i2 ,i3 ,item) ;
	}

	inline Flt64 operator() (CR<Index> i0 ,CR<Index> i1 ,CR<Index> i2 ,CR<Index> i3) const {
		Flt64 ret ;
		get (i0 ,i1 ,i2 ,i3 ,ret) ;
		return move (ret) ;
	}

	Tensor sadd (CR<Tensor> that) const {
		TensorLayout ret = TensorHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	forceinline Tensor operator+ (CR<Tensor> that) const {
		return sadd (that) ;
	}

	Tensor ssub (CR<Tensor> that) const {
		TensorLayout ret = TensorHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	forceinline Tensor operator- (CR<Tensor> that) const {
		return ssub (that) ;
	}

	Tensor smul (CR<Tensor> that) const {
		TensorLayout ret = TensorHolder::hold (thiz)->smul (that) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	forceinline Tensor operator* (CR<Tensor> that) const {
		return smul (that) ;
	}

	Tensor sdiv (CR<Tensor> that) const {
		TensorLayout ret = TensorHolder::hold (thiz)->sdiv (that) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	forceinline Tensor operator/ (CR<Tensor> that) const {
		return sdiv (that) ;
	}

	Tensor sabs () const {
		TensorLayout ret = TensorHolder::hold (thiz)->sabs () ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	Tensor minus () const {
		TensorLayout ret = TensorHolder::hold (thiz)->minus () ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	forceinline Tensor operator- () const {
		return minus () ;
	}
} ;

struct DisjointLayout {
	Array<Index> mTable ;
} ;

struct DisjointHolder implement Interface {
	imports VFat<DisjointHolder> hold (VR<DisjointLayout> that) ;
	imports CFat<DisjointHolder> hold (CR<DisjointLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual Length size () const = 0 ;
	virtual Index lead (CR<Index> from_) = 0 ;
	virtual void joint (CR<Index> from_ ,CR<Index> to_) = 0 ;
	virtual Bool edge (CR<Index> from_ ,CR<Index> to_) = 0 ;
	virtual Length depth (CR<Index> from_) = 0 ;
	virtual Deque<Index> cluster (CR<Index> from_) = 0 ;
	virtual Array<Index> jump (CR<Index> from_) = 0 ;
} ;

class Disjoint implement DisjointLayout {
protected:
	using DisjointLayout::mTable ;

public:
	implicit Disjoint () = default ;

	explicit Disjoint (CR<Length> size_) {
		DisjointHolder::hold (thiz)->initialize (size_) ;
	}

	Length size () const {
		return DisjointHolder::hold (thiz)->size () ;
	}

	void joint (CR<Index> from_ ,CR<Index> to_) {
		return DisjointHolder::hold (thiz)->joint (from_ ,to_) ;
	}

	Bool edge (CR<Index> from_ ,CR<Index> to_) {
		return DisjointHolder::hold (thiz)->edge (from_ ,to_) ;
	}

	Length depth (CR<Index> from_) {
		return DisjointHolder::hold (thiz)->depth (from_) ;
	}

	Deque<Index> cluster (CR<Index> from_) {
		return DisjointHolder::hold (thiz)->cluster (from_) ;
	}

	Array<Index> jump (CR<Index> from_) {
		return DisjointHolder::hold (thiz)->jump (from_) ;
	}
} ;

struct KMMatchLayout {
	Length mSize ;
	Flt32 mThreshold ;
	Ref<Image<Flt32>> mLove ;
	Array<Flt32> mUser ;
	Array<Flt32> mWork ;
	BitSet mUserVisit ;
	BitSet mWorkVisit ;
	Array<Index> mMatch ;
	Array<Flt32> mLack ;
} ;

struct KMMatchHolder implement Interface {
	imports VFat<KMMatchHolder> hold (VR<KMMatchLayout> that) ;
	imports CFat<KMMatchHolder> hold (CR<KMMatchLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual void set_threshold (CR<Flt64> threshold) = 0 ;
	virtual Length size () const = 0 ;
	virtual Array<Index> sort (CR<Image<Flt32>> love) = 0 ;
} ;

class KMMatch implement KMMatchLayout {
protected:
	using KMMatchLayout::mSize ;
	using KMMatchLayout::mThreshold ;
	using KMMatchLayout::mLove ;
	using KMMatchLayout::mUser ;
	using KMMatchLayout::mWork ;
	using KMMatchLayout::mUserVisit ;
	using KMMatchLayout::mWorkVisit ;
	using KMMatchLayout::mMatch ;
	using KMMatchLayout::mLack ;

public:
	implicit KMMatch () = default ;

	explicit KMMatch (CR<Length> size_) {
		KMMatchHolder::hold (thiz)->initialize (size_) ;
	}

	void set_threshold (CR<Flt64> threshold) {
		return KMMatchHolder::hold (thiz)->set_threshold (threshold) ;
	}

	Length size () const {
		return KMMatchHolder::hold (thiz)->size () ;
	}

	Array<Index> sort (CR<Image<Flt32>> love) {
		return KMMatchHolder::hold (thiz)->sort (love) ;
	}
} ;
} ;
