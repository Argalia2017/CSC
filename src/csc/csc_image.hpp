﻿#pragma once

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

template <class A>
class RowProxy {
private:
	using ITEM = decltype (nullof (A).at (0 ,0)) ;

protected:
	XREF<A> mThat ;
	INDEX mY ;

public:
	implicit RowProxy () = delete ;

	explicit RowProxy (XREF<A> that ,CREF<INDEX> y) :mThat (that) {
		mY = y ;
	}

	forceinline XREF<ITEM> operator[] (CREF<INDEX> x) rightvalue {
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

	BOOL equal (CREF<ImageShape> that) const {
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

	forceinline BOOL operator== (CREF<ImageShape> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<ImageShape> that) const {
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

template <class A>
struct ImagePureLayout implement ImageLayout {
public:
	implicit ImagePureLayout () noexcept {
		noop (RefBuffer<A> ()) ;
	}
} ;

struct ImageHolder implement Interface {
	imports VFat<ImageHolder> hold (VREF<ImageLayout> that) ;
	imports CFat<ImageHolder> hold (CREF<ImageLayout> that) ;

	virtual void initialize (CREF<Unknown> holder ,RREF<ImageLayout> that) = 0 ;
	virtual void initialize (CREF<Unknown> holder ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> step_) = 0 ;
	virtual void initialize (CREF<ImageLayout> that) = 0 ;
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
	virtual void reset (CREF<LENGTH> bx_ ,CREF<LENGTH> by_ ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual void fill (CREF<Pointer> item) = 0 ;
	virtual void splice (CREF<INDEX> x ,CREF<INDEX> y ,CREF<ImageLayout> item) = 0 ;
} ;

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

	implicit Image (RREF<ImageLayout> that) {
		ImageHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,move (that)) ;
	}

	explicit Image (CREF<ImageShape> shape) {
		ImageHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,shape.mCX ,shape.mCY ,SIZE_OF<A>::expr) ;
	}

	explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
		ImageHolder::hold (thiz)->initialize (BufferUnknownBinder<A> () ,cx_ ,cy_ ,SIZE_OF<A>::expr) ;
	}

	implicit Image (CREF<Image> that) {
		ImageHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VREF<Image> operator= (CREF<Image> that) {
		return assign (thiz ,that) ;
	}

	implicit Image (RREF<Image> that) = default ;

	forceinline VREF<Image> operator= (RREF<Image> that) = default ;

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

	void reset (CREF<ImageShape> shape) {
		return ImageHolder::hold (thiz)->reset (shape.mBX ,shape.mBY ,shape.mCX ,shape.mCY) ;
	}

	void reset (CREF<LENGTH> bx_ ,CREF<LENGTH> by_ ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
		return ImageHolder::hold (thiz)->reset (bx_ ,by_ ,cx_ ,cy_) ;
	}

	VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
		return ImageHolder::hold (thiz)->at (x ,y) ;
	}

	VREF<A> at (CREF<Pixel> index) leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline VREF<A> operator[] (CREF<Pixel> index) leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<VREF<Image>> operator[] (CREF<INDEX> y) leftvalue {
		return RowProxy<VREF<Image>> (thiz ,y) ;
	}

	VREF<BoxLayout> raw () leftvalue {
		return ImageHolder::hold (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return ImageHolder::hold (thiz)->raw () ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return ImageHolder::hold (thiz)->at (x ,y) ;
	}

	CREF<A> at (CREF<Pixel> index) const leftvalue {
		return at (index.mX ,index.mY) ;
	}

	forceinline CREF<A> operator[] (CREF<Pixel> index) const leftvalue {
		return at (index) ;
	}

	forceinline RowProxy<CREF<Image>> operator[] (CREF<INDEX> y) const leftvalue {
		return RowProxy<CREF<Image>> (thiz ,y) ;
	}

	PixelIterator range () const {
		return PixelIterator (0 ,cx () ,0 ,cy ()) ;
	}

	void fill (CREF<A> item) {
		return ImageHolder::hold (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CREF<INDEX> x ,CREF<INDEX> y ,CREF<Image> item) {
		return ImageHolder::hold (thiz)->splice (x ,y ,item) ;
	}

	void splice (CREF<Pixel> index ,CREF<Image> item) {
		return splice (index.mX ,index.mY ,item) ;
	}
} ;

struct ImageProcLayout ;

struct ImageProcHolder implement Interface {
	imports CREF<OfThis<UniqueRef<ImageProcLayout>>> expr_m () ;
	imports VFat<ImageProcHolder> hold (VREF<ImageProcLayout> that) ;
	imports CFat<ImageProcHolder> hold (CREF<ImageProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual ImageLayout make_image (RREF<BoxLayout> image) const = 0 ;
	virtual ImageLayout make_image (CREF<ImageShape> shape) const = 0 ;
	virtual ImageLayout make_image (CREF<ImageShape> shape ,CREF<Clazz> clazz ,CREF<LENGTH> channel) const = 0 ;
	virtual VREF<Pointer> peek_image (VREF<ImageLayout> image) const = 0 ;
	virtual CREF<Pointer> peek_image (CREF<ImageLayout> image) const = 0 ;
	virtual ImageLayout load_image (CREF<String<STR>> file) const = 0 ;
	virtual void save_image (CREF<String<STR>> file ,CREF<ImageLayout> image) const = 0 ;
	virtual Color1B sampler (CREF<Image<Color1B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
	virtual Color2B sampler (CREF<Image<Color2B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
	virtual Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
	virtual Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
	virtual FLT32 sampler (CREF<Image<FLT32>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
	virtual FLT64 sampler (CREF<Image<FLT64>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
} ;

class ImageProc implement OfThis<UniqueRef<ImageProcLayout>> {
public:
	static CREF<ImageProc> expr_m () {
		return keep[TYPE<ImageProc>::expr] (ImageProcHolder::expr) ;
	}

	static ImageLayout make_image (RREF<BoxLayout> image) {
		return ImageProcHolder::hold (expr)->make_image (move (image)) ;
	}

	static ImageLayout make_image (CREF<ImageShape> shape) {
		return ImageProcHolder::hold (expr)->make_image (shape) ;
	}

	static ImageLayout make_image (CREF<ImageShape> shape ,CREF<Clazz> clazz ,CREF<LENGTH> channel) {
		return ImageProcHolder::hold (expr)->make_image (shape ,clazz ,channel) ;
	}

	template <class A>
	static VREF<A> peek_image (VREF<ImageLayout> image ,TYPE<A>) {
		return ImageProcHolder::hold (expr)->peek_image (image) ;
	}

	template <class A>
	static CREF<A> peek_image (CREF<ImageLayout> image ,TYPE<A>) {
		return ImageProcHolder::hold (expr)->peek_image (image) ;
	}

	static ImageLayout load_image (CREF<String<STR>> file) {
		return ImageProcHolder::hold (expr)->load_image (file) ;
	}

	static void save_image (CREF<String<STR>> file ,CREF<ImageLayout> image) {
		return ImageProcHolder::hold (expr)->save_image (file ,image) ;
	}

	template <class ARG1>
	static ARG1 sampler (CREF<Image<ARG1>> image ,CREF<FLT64> x ,CREF<FLT64> y) {
		return ImageProcHolder::hold (expr)->sampler (image ,x ,y) ;
	}
} ;

struct TensorDataType {
	enum {
		Flt16 ,
		Flt32 ,
		Flt64 ,
		Flt128 ,
		ETC
	} ;
} ;

struct TensorLayout {
	RefBuffer<BYTE> mTensor ;
	LENGTH mOffset ;
	LENGTH mWidth ;
	Just<TensorDataType> mType ;
	Buffer5<LENGTH> mShape ;
} ;

struct TensorHolder implement Interface {
	imports VFat<TensorHolder> hold (VREF<TensorLayout> that) ;
	imports CFat<TensorHolder> hold (CREF<TensorLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_ ,CREF<Just<TensorDataType>> type_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual Just<TensorDataType> type () const = 0 ;
	virtual LENGTH cx () const = 0 ;
	virtual LENGTH cy () const = 0 ;
	virtual LENGTH cz () const = 0 ;
	virtual LENGTH cw () const = 0 ;
	virtual TensorLayout recast (CREF<Just<TensorDataType>> type_) = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cz_ ,CREF<LENGTH> cw_) = 0 ;
	virtual VREF<Pointer> ref_m () leftvalue = 0 ;
	virtual CREF<Pointer> ref_m () const leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () leftvalue = 0 ;
	virtual Ref<RefBuffer<BYTE>> borrow () const leftvalue = 0 ;
} ;

class Tensor implement TensorLayout {
protected:
	using TensorLayout::mTensor ;
	using TensorLayout::mOffset ;
	using TensorLayout::mWidth ;
	using TensorLayout::mType ;
	using TensorLayout::mShape ;

public:
	implicit Tensor () = default ;

	explicit Tensor (CREF<LENGTH> size_ ,CREF<Just<TensorDataType>> type_) {
		TensorHolder::hold (thiz)->initialize (size_ ,type_) ;
	}

	LENGTH size () const {
		return TensorHolder::hold (thiz)->size () ;
	}

	Just<TensorDataType> type () const {
		return TensorHolder::hold (thiz)->type () ;
	}

	LENGTH cx () const {
		return TensorHolder::hold (thiz)->cx () ;
	}

	LENGTH cy () const {
		return TensorHolder::hold (thiz)->cy () ;
	}

	LENGTH cz () const {
		return TensorHolder::hold (thiz)->cz () ;
	}

	LENGTH cw () const {
		return TensorHolder::hold (thiz)->cw () ;
	}

	Tensor recast (CREF<Just<TensorDataType>> type_) {
		TensorLayout ret = TensorHolder::hold (thiz)->recast (type_) ;
		return move (keep[TYPE<Tensor>::expr] (ret)) ;
	}

	void reset () {
		return TensorHolder::hold (thiz)->reset () ;
	}

	void reset (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_ ,CREF<LENGTH> cz_ ,CREF<LENGTH> cw_) {
		return TensorHolder::hold (thiz)->reset (cx_ ,cy_ ,cz_ ,cw_) ;
	}

	VREF<ARR<STRA>> ref_m () leftvalue {
		return TensorHolder::hold (thiz)->ref ;
	}

	CREF<ARR<STRA>> ref_m () const leftvalue {
		return TensorHolder::hold (thiz)->ref ;
	}

	Ref<RefBuffer<BYTE>> borrow () leftvalue {
		return TensorHolder::hold (thiz)->borrow () ;
	}

	Ref<RefBuffer<BYTE>> borrow () const leftvalue {
		return TensorHolder::hold (thiz)->borrow () ;
	}

	Tensor sadd (CREF<Tensor> that) const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	forceinline Tensor operator+ (CREF<Tensor> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CREF<Tensor> that) {
		thiz = sadd (that) ;
	}

	Tensor ssub (CREF<Tensor> that) const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	forceinline Tensor operator- (CREF<Tensor> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CREF<Tensor> that) {
		thiz = ssub (that) ;
	}

	Tensor smul (CREF<Tensor> that) const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	forceinline Tensor operator* (CREF<Tensor> that) const {
		return smul (that) ;
	}

	forceinline void operator*= (CREF<Tensor> that) {
		thiz = smul (that) ;
	}

	Tensor sdiv (CREF<Tensor> that) const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	forceinline Tensor operator/ (CREF<Tensor> that) const {
		return sdiv (that) ;
	}

	forceinline void operator/= (CREF<Tensor> that) {
		thiz = sdiv (that) ;
	}

	Tensor smod (CREF<Tensor> that) const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	forceinline Tensor operator% (CREF<Tensor> that) const {
		return smod (that) ;
	}

	forceinline void operator%= (CREF<Tensor> that) {
		thiz = smod (that) ;
	}

	Tensor sabs () const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	Tensor minus () const {
		Tensor ret ;
		unimplemented () ;
		return move (ret) ;
	}

	forceinline Tensor operator- () const {
		return minus () ;
	}
} ;

struct DisjointLayout {
	Array<INDEX> mTable ;
} ;

struct DisjointHolder implement Interface {
	imports VFat<DisjointHolder> hold (VREF<DisjointLayout> that) ;
	imports CFat<DisjointHolder> hold (CREF<DisjointLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual INDEX lead (CREF<INDEX> from_) = 0 ;
	virtual void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) = 0 ;
	virtual BOOL edge (CREF<INDEX> from_ ,CREF<INDEX> to_) = 0 ;
	virtual LENGTH depth (CREF<INDEX> from_) = 0 ;
	virtual Deque<INDEX> cluster (CREF<INDEX> from_) = 0 ;
	virtual Array<INDEX> jump (CREF<INDEX> from_) = 0 ;
} ;

class Disjoint implement DisjointLayout {
protected:
	using DisjointLayout::mTable ;

public:
	implicit Disjoint () = default ;

	explicit Disjoint (CREF<LENGTH> size_) {
		DisjointHolder::hold (thiz)->initialize (size_) ;
	}

	LENGTH size () const {
		return DisjointHolder::hold (thiz)->size () ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) {
		return DisjointHolder::hold (thiz)->joint (from_ ,to_) ;
	}

	BOOL edge (CREF<INDEX> from_ ,CREF<INDEX> to_) {
		return DisjointHolder::hold (thiz)->edge (from_ ,to_) ;
	}

	LENGTH depth (CREF<INDEX> from_) {
		return DisjointHolder::hold (thiz)->depth (from_) ;
	}

	Deque<INDEX> cluster (CREF<INDEX> from_) {
		return DisjointHolder::hold (thiz)->cluster (from_) ;
	}

	Array<INDEX> jump (CREF<INDEX> from_) {
		return DisjointHolder::hold (thiz)->jump (from_) ;
	}
} ;

struct KMMatchLayout {
	LENGTH mSize ;
	FLT32 mThreshold ;
	Array<FLT32> mLove ;
	Array<FLT32> mUser ;
	Array<FLT32> mWork ;
	BitSet mUserVisit ;
	BitSet mWorkVisit ;
	Array<INDEX> mMatch ;
	Array<FLT32> mLack ;
} ;

struct KMMatchHolder implement Interface {
	imports VFat<KMMatchHolder> hold (VREF<KMMatchLayout> that) ;
	imports CFat<KMMatchHolder> hold (CREF<KMMatchLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual void set_threshold (CREF<FLT64> threshold) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual Array<INDEX> sort (RREF<Array<FLT32>> love) = 0 ;
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

	explicit KMMatch (CREF<LENGTH> size_) {
		KMMatchHolder::hold (thiz)->initialize (size_) ;
	}

	void set_threshold (CREF<FLT64> threshold) {
		return KMMatchHolder::hold (thiz)->set_threshold (threshold) ;
	}

	LENGTH size () const {
		return KMMatchHolder::hold (thiz)->size () ;
	}

	Array<INDEX> sort (RREF<Array<FLT32>> love) {
		return KMMatchHolder::hold (thiz)->sort (move (love)) ;
	}
} ;
} ;
