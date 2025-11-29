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
class RowProxy {
private:
	using ITEM = decltype (nullof (A).at (0 ,0)) ;

protected:
	XR<A> mThat ;
	INDEX mY ;

public:
	implicit RowProxy () = delete ;

	explicit RowProxy (XR<A> that ,CR<INDEX> y) :mThat (that) {
		mY = y ;
	}

	forceinline XR<ITEM> operator[] (CR<INDEX> x) rightvalue {
		return mThat.at (x ,mY) ;
	}
} ;

struct ImageShapeLayout {
	LENGTH mBX ;
	LENGTH mBY ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mStep ;

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

	LENGTH size () const {
		return mCX * mCY ;
	}

	BOOL equal (CR<ImageShape> that) const {
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

	forceinline BOOL operator== (CR<ImageShape> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<ImageShape> that) const {
		return (!equal (that)) ;
	}
} ;

struct ImageLayout {
	RefBuffer<Pointer> mImage ;
	LENGTH mWidth ;
	LENGTH mStride ;
	LENGTH mBX ;
	LENGTH mBY ;
	LENGTH mCX ;
	LENGTH mCY ;
} ;

struct ImageHolder implement Interface {
	imports VFat<ImageHolder> hold (VR<ImageLayout> that) ;
	imports CFat<ImageHolder> hold (CR<ImageLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (CR<Unknown> holder ,RR<ImageLayout> that) = 0 ;
	virtual void initialize (CR<Unknown> holder ,CR<LENGTH> cx_ ,CR<LENGTH> cy_ ,CR<LENGTH> step_) = 0 ;
	virtual void initialize (CR<ImageLayout> that) = 0 ;
	virtual BOOL fixed () const = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH stride () const = 0 ;
	virtual BOOL continous () const = 0 ;
	virtual LENGTH bx () const = 0 ;
	virtual LENGTH by () const = 0 ;
	virtual LENGTH cx () const = 0 ;
	virtual LENGTH cy () const = 0 ;
	virtual ImageShape shape () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<LENGTH> bx_ ,CR<LENGTH> by_ ,CR<LENGTH> cx_ ,CR<LENGTH> cy_) = 0 ;
	virtual VR<BoxLayout> raw () leftvalue = 0 ;
	virtual CR<BoxLayout> raw () const leftvalue = 0 ;
	virtual VR<Pointer> at (CR<INDEX> x ,CR<INDEX> y) leftvalue = 0 ;
	virtual CR<Pointer> at (CR<INDEX> x ,CR<INDEX> y) const leftvalue = 0 ;
	virtual void fill (CR<Pointer> item) = 0 ;
	virtual void splice (CR<INDEX> x ,CR<INDEX> y ,CR<ImageLayout> item) = 0 ;
} ;

template <class A>
struct ImagePureLayout implement ImageLayout {
public:
	implicit ImagePureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
		ImageHolder::hold (thiz)->prepare (ArrayUnknownBinder<A> ()) ;
	}
} ;

template <>
struct ImagePureLayout<Pointer> implement ImageLayout {} ;

template <class A>
class Image implement ImagePureLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;

protected:
	using ImagePureLayout<A>::mImage ;
	using ImagePureLayout<A>::mWidth ;
	using ImagePureLayout<A>::mStride ;
	using ImagePureLayout<A>::mBX ;
	using ImagePureLayout<A>::mBY ;
	using ImagePureLayout<A>::mCX ;
	using ImagePureLayout<A>::mCY ;

public:
	implicit Image () = default ;

	implicit Image (RR<ImageLayout> that) {
		ImageHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,move (that)) ;
	}

	explicit Image (CR<ImageShape> shape_) {
		ImageHolder::hold (thiz)->initialize (ArrayUnknownBinder<A> () ,shape_.mCX ,shape_.mCY ,SIZE_OF<A>::expr) ;
	}

	explicit Image (CR<LENGTH> cx_ ,CR<LENGTH> cy_) {
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

	BOOL fixed () const {
		return ImageHolder::hold (thiz)->fixed () ;
	}

	LENGTH size () const {
		return ImageHolder::hold (thiz)->size () ;
	}

	LENGTH step () const {
		return ImageHolder::hold (thiz)->step () ;
	}

	LENGTH stride () const {
		return ImageHolder::hold (thiz)->stride () ;
	}

	BOOL continous () const {
		return ImageHolder::hold (thiz)->continous () ;
	}

	LENGTH bx () const {
		return ImageHolder::hold (thiz)->bx () ;
	}

	LENGTH by () const {
		return ImageHolder::hold (thiz)->by () ;
	}

	LENGTH cx () const {
		return ImageHolder::hold (thiz)->cx () ;
	}

	LENGTH cy () const {
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

	void reset (CR<LENGTH> bx_ ,CR<LENGTH> by_ ,CR<LENGTH> cx_ ,CR<LENGTH> cy_) {
		return ImageHolder::hold (thiz)->reset (bx_ ,by_ ,cx_ ,cy_) ;
	}

	VR<A> at (CR<INDEX> x ,CR<INDEX> y) leftvalue {
		return ImageHolder::hold (thiz)->at (x ,y) ;
	}

	VR<A> at (CR<Pixel> index) leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline VR<A> operator[] (CR<Pixel> index) leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<VR<Image>> operator[] (CR<INDEX> y) leftvalue {
		return RowProxy<VR<Image>> (thiz ,y) ;
	}

	VR<BoxLayout> raw () leftvalue {
		return ImageHolder::hold (thiz)->raw () ;
	}

	CR<BoxLayout> raw () const leftvalue {
		return ImageHolder::hold (thiz)->raw () ;
	}

	CR<A> at (CR<INDEX> x ,CR<INDEX> y) const leftvalue {
		return ImageHolder::hold (thiz)->at (x ,y) ;
	}

	CR<A> at (CR<Pixel> index) const leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline CR<A> operator[] (CR<Pixel> index) const leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<CR<Image>> operator[] (CR<INDEX> y) const leftvalue {
		return RowProxy<CR<Image>> (thiz ,y) ;
	}

	PixelIterator iter () const {
		return PixelIterator (0 ,cx () ,0 ,cy ()) ;
	}

	void fill (CR<A> item) {
		return ImageHolder::hold (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CR<INDEX> x ,CR<INDEX> y ,CR<Image> item) {
		return ImageHolder::hold (thiz)->splice (x ,y ,item) ;
	}

	void splice (CR<Pixel> index ,CR<Image> item) {
		return splice (index.mX ,index.mY ,item) ;
	}
} ;

struct Color1B {
	BYTE mB ;
} ;

struct Color2B {
	BYTE mB ;
	BYTE mG ;
} ;

struct Color3B {
	BYTE mB ;
	BYTE mG ;
	BYTE mR ;
} ;

struct Color4B {
	BYTE mB ;
	BYTE mG ;
	BYTE mR ;
	BYTE mA ;
} ;

static constexpr auto COLOR_BLACK = Color3B ({BYTE (0X00) ,BYTE (0X00) ,BYTE (0X00)}) ;
static constexpr auto COLOR_WHITE = Color3B ({BYTE (0XFF) ,BYTE (0XFF) ,BYTE (0XFF)}) ;
static constexpr auto COLOR_GRAY = Color3B ({BYTE (0X80) ,BYTE (0X80) ,BYTE (0X80)}) ;
static constexpr auto COLOR_RED = Color3B ({BYTE (0X00) ,BYTE (0X00) ,BYTE (0XFF)}) ;
static constexpr auto COLOR_GREEN = Color3B ({BYTE (0X00) ,BYTE (0XFF) ,BYTE (0X00)}) ;
static constexpr auto COLOR_BLUE = Color3B ({BYTE (0XFF) ,BYTE (0X00) ,BYTE (0X00)}) ;
static constexpr auto COLOR_YELLOW = Color3B ({BYTE (0X00) ,BYTE (0XFF) ,BYTE (0XFF)}) ;
static constexpr auto COLOR_PURPLE = Color3B ({BYTE (0XFF) ,BYTE (0X00) ,BYTE (0XFF)}) ;
static constexpr auto COLOR_CYAN = Color3B ({BYTE (0XFF) ,BYTE (0XFF) ,BYTE (0X00)}) ;

struct ColorProcLayout ;

struct ColorProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<ColorProcLayout>>> expr_m () ;
	imports VFat<ColorProcHolder> hold (VR<ColorProcLayout> that) ;
	imports CFat<ColorProcHolder> hold (CR<ColorProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual FLT64 gray_from_bgr (CR<Color3B> a) const = 0 ;
	virtual Color3B bgr_from_gray (CR<FLT64> a) const = 0 ;
	virtual Color3B jet_from_norm (CR<FLT64> a) const = 0 ;
	virtual FLT64 norm_from_jet (CR<Color3B> a) const = 0 ;
	virtual Color3B hsv_from_bgr (CR<Color3B> a) const = 0 ;
	virtual Color3B bgr_from_hsv (CR<Color3B> a) const = 0 ;
} ;

class ColorProc implement OfThis<UniqueRef<ColorProcLayout>> {
public:
	static CR<ColorProc> expr_m () {
		return keep[TYPE<ColorProc>::expr] (ColorProcHolder::expr) ;
	}

	imports FLT64 gray_from_bgr (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->gray_from_bgr (a) ;
	}

	imports Color3B bgr_from_gray (CR<FLT64> a) {
		return ColorProcHolder::hold (expr)->bgr_from_gray (a) ;
	}

	imports Color3B jet_from_norm (CR<FLT64> a) {
		return ColorProcHolder::hold (expr)->jet_from_norm (a) ;
	}

	imports FLT64 norm_from_jet (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->norm_from_jet (a) ;
	}

	imports Color3B hsv_from_bgr (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->hsv_from_bgr (a) ;
	}

	imports Color3B bgr_from_hsv (CR<Color3B> a) {
		return ColorProcHolder::hold (expr)->bgr_from_hsv (a) ;
	}
} ;

struct ImageProcLayout ;

struct ImageProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<ImageProcLayout>>> expr_m () ;
	imports VFat<ImageProcHolder> hold (VR<ImageProcLayout> that) ;
	imports CFat<ImageProcHolder> hold (CR<ImageProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual ImageLayout make_image (RR<BoxLayout> image) const = 0 ;
	virtual ImageLayout make_image (CR<ImageShape> shape) const = 0 ;
	virtual ImageLayout make_image (CR<ImageShape> shape ,CR<Clazz> clazz ,CR<LENGTH> channel) const = 0 ;
	virtual VR<Pointer> peek_image (VR<ImageLayout> image) const = 0 ;
	virtual CR<Pointer> peek_image (CR<ImageLayout> image) const = 0 ;
	virtual ImageLayout load_image (CR<String<STR>> file) const = 0 ;
	virtual void save_image (CR<String<STR>> file ,CR<ImageLayout> image) const = 0 ;
	virtual Color1B sampler (CR<Image<Color1B>> image ,CR<FLT64> x ,CR<FLT64> y) const = 0 ;
	virtual Color2B sampler (CR<Image<Color2B>> image ,CR<FLT64> x ,CR<FLT64> y) const = 0 ;
	virtual Color3B sampler (CR<Image<Color3B>> image ,CR<FLT64> x ,CR<FLT64> y) const = 0 ;
	virtual Color4B sampler (CR<Image<Color4B>> image ,CR<FLT64> x ,CR<FLT64> y) const = 0 ;
	virtual FLT32 sampler (CR<Image<FLT32>> image ,CR<FLT64> x ,CR<FLT64> y) const = 0 ;
	virtual FLT64 sampler (CR<Image<FLT64>> image ,CR<FLT64> x ,CR<FLT64> y) const = 0 ;
} ;

class ImageProc implement OfThis<UniqueRef<ImageProcLayout>> {
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

	static ImageLayout make_image (CR<ImageShape> shape ,CR<Clazz> clazz ,CR<LENGTH> channel) {
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

	static ImageLayout load_image (CR<String<STR>> file) {
		return ImageProcHolder::hold (expr)->load_image (file) ;
	}

	static void save_image (CR<String<STR>> file ,CR<ImageLayout> image) {
		return ImageProcHolder::hold (expr)->save_image (file ,image) ;
	}

	template <class ARG1>
	static ARG1 sampler (CR<Image<ARG1>> image ,CR<FLT64> x ,CR<FLT64> y) {
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

class FltProxy {
protected:
	FLAG mBuffer ;
	LENGTH mStep ;

public:
	implicit FltProxy () = delete ;

	explicit FltProxy (CR<FLAG> buffer_ ,CR<LENGTH> step_) {
		mBuffer = buffer_ ;
		mStep = step_ ;
	}

	forceinline operator FLT32 () const {
		if (mStep == 4)
			return FLT32 (bitwise[TYPE<FLT32>::expr] (Pointer::make (mBuffer))) ;
		if (mStep == 8)
			return FLT32 (bitwise[TYPE<FLT64>::expr] (Pointer::make (mBuffer))) ;
		assert (FALSE) ;
		return 0 ;
	}

	forceinline operator FLT64 () const {
		if (mStep == 4)
			return FLT64 (bitwise[TYPE<FLT32>::expr] (Pointer::make (mBuffer))) ;
		if (mStep == 8)
			return FLT64 (bitwise[TYPE<FLT64>::expr] (Pointer::make (mBuffer))) ;
		assert (FALSE) ;
		return 0 ;
	}
} ;

struct TensorLayout {
	Ref<RefBuffer<BYTE>> mTensor ;
	FLAG mHolder ;
	FLAG mBuffer ;
	LENGTH mRank ;
	Buffer5<LENGTH> mStride ;
} ;

struct TensorHolder implement Interface {
	imports VFat<TensorHolder> hold (VR<TensorLayout> that) ;
	imports CFat<TensorHolder> hold (CR<TensorLayout> that) ;

	virtual void initialize (CR<RefBufferLayout> that) = 0 ;
	virtual void initialize (VR<FarBufferLayout> that) = 0 ;
	virtual void initialize (CR<LENGTH> size_ ,CR<Just<TensorType>> type_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual Just<TensorType> type () const = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual LENGTH shape (CR<INDEX> index) const = 0 ;
	virtual TensorLayout recast (CR<Just<TensorType>> type_) const = 0 ;
	virtual TensorLayout reshape () const = 0 ;
	virtual TensorLayout reshape (CR<WrapperLayout> shape_) const = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () const leftvalue = 0 ;
	virtual FltProxy at (CR<INDEX> i1) const = 0 ;
	virtual FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2) const = 0 ;
	virtual FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2 ,CR<INDEX> i3) const = 0 ;
	virtual FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2 ,CR<INDEX> i3 ,CR<INDEX> i4) const = 0 ;
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
	implicit Tensor (CR<RefBuffer<ARG1>> that) {
		TensorHolder::hold (thiz)->initialize (that) ;
	}

	template <class ARG1 ,class = REQUIRE<IS_FLOAT<ARG1>>>
	implicit Tensor (VR<FarBuffer<ARG1>> that) {
		TensorHolder::hold (thiz)->initialize (that) ;
	}

	explicit Tensor (CR<LENGTH> size_ ,CR<Just<TensorType>> type_) {
		TensorHolder::hold (thiz)->initialize (size_ ,type_) ;
	}

	LENGTH size () const {
		return TensorHolder::hold (thiz)->size () ;
	}

	Just<TensorType> type () const {
		return TensorHolder::hold (thiz)->type () ;
	}

	LENGTH rank () const {
		return TensorHolder::hold (thiz)->rank () ;
	}

	LENGTH shape (CR<INDEX> index) const {
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
		TensorLayout ret = TensorHolder::hold (thiz)->reshape (MakeWrapper (LENGTH (shape_)...)) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue {
		return TensorHolder::hold (thiz)->borrow () ;
	}

	FltProxy at (CR<INDEX> i1) const {
		return TensorHolder::hold (thiz)->at (i1) ;
	}

	FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2) const {
		return TensorHolder::hold (thiz)->at (i1 ,i2) ;
	}

	FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2 ,CR<INDEX> i3) const {
		return TensorHolder::hold (thiz)->at (i1 ,i2 ,i3) ;
	}

	FltProxy at (CR<INDEX> i1 ,CR<INDEX> i2 ,CR<INDEX> i3 ,CR<INDEX> i4) const {
		return TensorHolder::hold (thiz)->at (i1 ,i2 ,i3 ,i4) ;
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
	Array<INDEX> mTable ;
} ;

struct DisjointHolder implement Interface {
	imports VFat<DisjointHolder> hold (VR<DisjointLayout> that) ;
	imports CFat<DisjointHolder> hold (CR<DisjointLayout> that) ;

	virtual void initialize (CR<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual INDEX lead (CR<INDEX> from_) = 0 ;
	virtual void joint (CR<INDEX> from_ ,CR<INDEX> to_) = 0 ;
	virtual BOOL edge (CR<INDEX> from_ ,CR<INDEX> to_) = 0 ;
	virtual LENGTH depth (CR<INDEX> from_) = 0 ;
	virtual Deque<INDEX> cluster (CR<INDEX> from_) = 0 ;
	virtual Array<INDEX> jump (CR<INDEX> from_) = 0 ;
} ;

class Disjoint implement DisjointLayout {
protected:
	using DisjointLayout::mTable ;

public:
	implicit Disjoint () = default ;

	explicit Disjoint (CR<LENGTH> size_) {
		DisjointHolder::hold (thiz)->initialize (size_) ;
	}

	LENGTH size () const {
		return DisjointHolder::hold (thiz)->size () ;
	}

	void joint (CR<INDEX> from_ ,CR<INDEX> to_) {
		return DisjointHolder::hold (thiz)->joint (from_ ,to_) ;
	}

	BOOL edge (CR<INDEX> from_ ,CR<INDEX> to_) {
		return DisjointHolder::hold (thiz)->edge (from_ ,to_) ;
	}

	LENGTH depth (CR<INDEX> from_) {
		return DisjointHolder::hold (thiz)->depth (from_) ;
	}

	Deque<INDEX> cluster (CR<INDEX> from_) {
		return DisjointHolder::hold (thiz)->cluster (from_) ;
	}

	Array<INDEX> jump (CR<INDEX> from_) {
		return DisjointHolder::hold (thiz)->jump (from_) ;
	}
} ;

struct KMMatchLayout {
	LENGTH mSize ;
	FLT32 mThreshold ;
	Ref<Image<FLT32>> mLove ;
	Array<FLT32> mUser ;
	Array<FLT32> mWork ;
	BitSet mUserVisit ;
	BitSet mWorkVisit ;
	Array<INDEX> mMatch ;
	Array<FLT32> mLack ;
} ;

struct KMMatchHolder implement Interface {
	imports VFat<KMMatchHolder> hold (VR<KMMatchLayout> that) ;
	imports CFat<KMMatchHolder> hold (CR<KMMatchLayout> that) ;

	virtual void initialize (CR<LENGTH> size_) = 0 ;
	virtual void set_threshold (CR<FLT64> threshold) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual Array<INDEX> sort (CR<Image<FLT32>> love) = 0 ;
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

	explicit KMMatch (CR<LENGTH> size_) {
		KMMatchHolder::hold (thiz)->initialize (size_) ;
	}

	void set_threshold (CR<FLT64> threshold) {
		return KMMatchHolder::hold (thiz)->set_threshold (threshold) ;
	}

	LENGTH size () const {
		return KMMatchHolder::hold (thiz)->size () ;
	}

	Array<INDEX> sort (CR<Image<FLT32>> love) {
		return KMMatchHolder::hold (thiz)->sort (love) ;
	}
} ;
} ;
