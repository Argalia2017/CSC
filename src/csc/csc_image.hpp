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

struct ColorLayout {
	BoxBuffer<FLT64 ,RANK4> mColor ;
} ;

class Color implement ColorLayout {
private:
	using ColorLayout::mColor ;

public:
	implicit Color () = default ;

	implicit Color (CREF<FLT64> that) {
		mColor[0] = that ;
		mColor[1] = that ;
		mColor[2] = that ;
		mColor[3] = that ;
	}

	implicit Color (CREF<Color3B> that) {
		mColor[0] = FLT64 (that.mB) ;
		mColor[1] = FLT64 (that.mG) ;
		mColor[2] = FLT64 (that.mR) ;
		mColor[3] = 0 ;
	}

	implicit Color (CREF<Color4B> that) {
		mColor[0] = FLT64 (that.mB) ;
		mColor[1] = FLT64 (that.mG) ;
		mColor[2] = FLT64 (that.mR) ;
		mColor[3] = FLT64 (that.mA) ;
	}

	explicit Color (CREF<FLT64> b ,CREF<FLT64> g ,CREF<FLT64> r ,CREF<FLT64> a) {
		mColor[0] = b ;
		mColor[1] = g ;
		mColor[2] = r ;
		mColor[3] = a ;
	}

	CREF<FLT64> at (CREF<INDEX> index) const leftvalue {
		return mColor[index] ;
	}

	forceinline CREF<FLT64> operator[] (CREF<INDEX> index) const leftvalue {
		return at (index) ;
	}

	Color add (CREF<Color> that) const {
		Color ret ;
		for (auto &&i : iter (0 ,4))
			ret.mColor[i] = mColor[i] + that.mColor[i] ;
		return move (ret) ;
	}

	forceinline Color operator+ (CREF<Color> that) const {
		return add (that) ;
	}

	forceinline void operator+= (CREF<Color> that) {
		thiz = add (that) ;
	}

	Color sub (CREF<Color> that) const {
		Color ret ;
		for (auto &&i : iter (0 ,4))
			ret.mColor[i] = mColor[i] - that.mColor[i] ;
		return move (ret) ;
	}

	forceinline Color operator- (CREF<Color> that) const {
		return sub (that) ;
	}

	forceinline void operator-= (CREF<Color> that) {
		thiz = sub (that) ;
	}

	Color mul (CREF<FLT64> scale) const {
		Color ret ;
		for (auto &&i : iter (0 ,4))
			ret.mColor[i] = mColor[i] * scale ;
		return move (ret) ;
	}

	forceinline Color operator* (CREF<FLT64> scale) const {
		return mul (scale) ;
	}

	forceinline void operator*= (CREF<FLT64> scale) {
		thiz = mul (scale) ;
	}

	Color div (CREF<FLT64> scale) const {
		Color ret ;
		for (auto &&i : iter (0 ,4))
			ret.mColor[i] = mColor[i] / scale ;
		return move (ret) ;
	}

	forceinline Color operator/ (CREF<FLT64> scale) const {
		return div (scale) ;
	}

	forceinline void operator/= (CREF<FLT64> scale) {
		thiz = div (scale) ;
	}

	void clamp (CREF<FLT64> lb ,CREF<FLT64> rb) {
		for (auto &&i : iter (0 ,4))
			mColor[i] = MathProc::clamp (mColor[i] ,lb ,rb) ;
	}
} ;

template <class A>
class RowProxy {
private:
	using ITEM = REF<typeof (PTR<A> (NULL)->at (0 ,0)) ,REFLECT_REF<A>> ;

protected:
	PTR<A> mThat ;
	INDEX mY ;

public:
	implicit RowProxy () = delete ;

	explicit RowProxy (XREF<A> that ,CREF<INDEX> y) :mThat ((&that)) {
		mY = y ;
	}

	forceinline XREF<ITEM> operator[] (CREF<INDEX> x) rightvalue {
		return mThat->at (x ,mY) ;
	}
} ;

struct ImageWidth {
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mStep ;
	LENGTH mChannel ;

public:
	LENGTH size () const {
		return mCX * mCY ;
	}

	BOOL equal (CREF<ImageWidth> that) const {
		if (mCX != that.mCX)
			return FALSE ;
		if (mCY != that.mCY)
			return FALSE ;
		if (mStep != that.mStep)
			return FALSE ;
		if (mChannel != that.mChannel)
			return FALSE ;
		return TRUE ;
	}

	forceinline BOOL operator== (CREF<ImageWidth> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<ImageWidth> that) const {
		return !(equal (that)) ;
	}
} ;

struct ImageLayout {
	RefBuffer<Pointer> mImage ;
	ImageWidth mWidth ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mSX ;
	LENGTH mSY ;
	LENGTH mOffset ;
} ;

template <class A>
struct ImageImplLayout implement ImageLayout {
public:
	implicit ImageImplLayout () noexcept {
		noop (RefBuffer<A> ()) ;
	}
} ;

struct ImageHolder implement Interface {
	imports VFat<ImageHolder> create (VREF<ImageLayout> that) ;
	imports CFat<ImageHolder> create (CREF<ImageLayout> that) ;

	virtual void initialize (CREF<Unknown> element ,RREF<ImageLayout> that) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) = 0 ;
	virtual void initialize (CREF<Unknown> element ,CREF<ImageWidth> width) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH step () const = 0 ;
	virtual LENGTH cx () const = 0 ;
	virtual LENGTH cy () const = 0 ;
	virtual LENGTH sx () const = 0 ;
	virtual LENGTH sy () const = 0 ;
	virtual LENGTH offset () const = 0 ;
	virtual ImageWidth width () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> cx_ ,CREF<INDEX> cy_ ,CREF<INDEX> sx_ ,CREF<INDEX> sy_ ,CREF<INDEX> offset_) = 0 ;
	virtual VREF<BoxLayout> raw () leftvalue = 0 ;
	virtual CREF<BoxLayout> raw () const leftvalue = 0 ;
	virtual VREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue = 0 ;
	virtual CREF<Pointer> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue = 0 ;
	virtual void fill (CREF<Pointer> item) = 0 ;
	virtual void splice (CREF<INDEX> x ,CREF<INDEX> y ,CREF<ImageLayout> item) = 0 ;
} ;

template <class A>
class Image implement ImageImplLayout<A> {
private:
	require (IS_TRIVIAL<A>) ;
	using ImageLayout = ImageImplLayout<A> ;

protected:
	using ImageLayout::mImage ;
	using ImageLayout::mWidth ;
	using ImageLayout::mCX ;
	using ImageLayout::mCY ;
	using ImageLayout::mSX ;
	using ImageLayout::mSY ;
	using ImageLayout::mOffset ;

public:
	implicit Image () = default ;

	implicit Image (RREF<ImageLayout> that) {
		ImageHolder::create (thiz)->initialize (RefUnknownBinder<A> () ,move (that)) ;
	}

	explicit Image (CREF<LENGTH> cx_ ,CREF<LENGTH> cy_) {
		ImageHolder::create (thiz)->initialize (RefUnknownBinder<A> () ,cx_ ,cy_) ;
	}

	explicit Image (CREF<ImageWidth> width) {
		ImageHolder::create (thiz)->initialize (RefUnknownBinder<A> () ,width) ;
	}

	BOOL exist () const {
		return ImageHolder::create (thiz)->exist () ;
	}

	LENGTH size () const {
		return ImageHolder::create (thiz)->size () ;
	}

	LENGTH step () const {
		return ImageHolder::create (thiz)->step () ;
	}

	LENGTH cx () const {
		return ImageHolder::create (thiz)->cx () ;
	}

	LENGTH cy () const {
		return ImageHolder::create (thiz)->cy () ;
	}

	LENGTH sx () const {
		return ImageHolder::create (thiz)->sx () ;
	}

	LENGTH sy () const {
		return ImageHolder::create (thiz)->sy () ;
	}

	LENGTH offset () const {
		return ImageHolder::create (thiz)->offset () ;
	}

	ImageWidth width () const {
		return ImageHolder::create (thiz)->width () ;
	}

	void reset () {
		return ImageHolder::create (thiz)->reset () ;
	}

	void reset (CREF<INDEX> cx_ ,CREF<INDEX> cy_ ,CREF<INDEX> sx_ ,CREF<INDEX> sy_ ,CREF<INDEX> offset_) {
		return ImageHolder::create (thiz)->reset (cx_ ,cy_ ,sx_ ,sy_ ,offset_) ;
	}

	VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue {
		return ImageHolder::create (thiz)->at (x ,y) ;
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
		return ImageHolder::create (thiz)->raw () ;
	}

	CREF<BoxLayout> raw () const leftvalue {
		return ImageHolder::create (thiz)->raw () ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue {
		return ImageHolder::create (thiz)->at (x ,y) ;
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
		return ImageHolder::create (thiz)->fill (Pointer::from (item)) ;
	}

	void splice (CREF<INDEX> x ,CREF<INDEX> y ,CREF<Image> item) {
		return ImageHolder::create (thiz)->splice (x ,y ,item) ;
	}

	Image add (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] + that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator+ (CREF<Image> that) const {
		return add (that) ;
	}

	forceinline void operator+= (CREF<Image> that) {
		thiz = add (that) ;
	}

	Image sub (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] - that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator- (CREF<Image> that) const {
		return sub (that) ;
	}

	forceinline void operator-= (CREF<Image> that) {
		thiz = sub (that) ;
	}

	Image mul (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] * that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator* (CREF<Image> that) const {
		return mul (that) ;
	}

	forceinline void operator*= (CREF<Image> that) {
		thiz = mul (that) ;
	}

	Image div (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] / that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator/ (CREF<Image> that) const {
		return div (that) ;
	}

	forceinline void operator/= (CREF<Image> that) {
		thiz = div (that) ;
	}

	Image mod (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] % that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator% (CREF<Image> that) const {
		return mod (that) ;
	}

	forceinline void operator%= (CREF<Image> that) {
		thiz = mod (that) ;
	}

	Image plus () const {
		const auto r1x = width () ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = +thiz[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator+ () const {
		return plus () ;
	}

	Image minus () const {
		const auto r1x = width () ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = -thiz[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator- () const {
		return minus () ;
	}

	Image band (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] & that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator& (CREF<Image> that) const {
		return band (that) ;
	}

	forceinline void operator&= (CREF<Image> that) {
		thiz = band (that) ;
	}

	Image bor (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] | that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator| (CREF<Image> that) const {
		return bor (that) ;
	}

	forceinline void operator|= (CREF<Image> that) {
		thiz = bor (that) ;
	}

	Image bxor (CREF<Image> that) const {
		const auto r1x = width () ;
		const auto r2x = that.width () ;
		assert (r1x == r2x) ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = thiz[i] ^ that[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator^ (CREF<Image> that) const {
		return bxor (that) ;
	}

	forceinline void operator^= (CREF<Image> that) {
		thiz = bxor (that) ;
	}

	Image bnot () const {
		const auto r1x = width () ;
		Image ret = Image (r1x) ;
		for (auto &&i : iter (0 ,r1x.mCX ,0 ,r1x.mCY)) {
			ret[i] = ~thiz[i] ;
		}
		return move (ret) ;
	}

	forceinline Image operator~ () const {
		return bnot () ;
	}
} ;

struct ImageProcPureLayout ;

struct ImageProcLayout {
	Ref<ImageProcPureLayout> mThis ;
} ;

struct ImageProcHolder implement Interface {
	imports VFat<ImageProcHolder> create (VREF<ImageProcLayout> that) ;
	imports CFat<ImageProcHolder> create (CREF<ImageProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual ImageLayout load (RREF<AutoRef<Pointer>> item) const = 0 ;
	virtual ImageLayout load (CREF<String<STR>> file) const = 0 ;
	virtual void save (CREF<String<STR>> file ,CREF<ImageLayout> image) const = 0 ;
	virtual Color3B sampler (CREF<Image<Color3B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
	virtual Color4B sampler (CREF<Image<Color4B>> image ,CREF<FLT64> x ,CREF<FLT64> y) const = 0 ;
} ;

class ImageProc implement ImageProcLayout {
protected:
	using ImageProcLayout::mThis ;

public:
	imports CREF<ImageProc> instance () {
		return memorize ([&] () {
			ImageProc ret ;
			ImageProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports ImageLayout load (RREF<AutoRef<Pointer>> item) {
		return ImageProcHolder::create (instance ())->load (move (item)) ;
	}

	imports ImageLayout load (CREF<String<STR>> file) {
		return ImageProcHolder::create (instance ())->load (file) ;
	}

	imports void save (CREF<String<STR>> file ,CREF<ImageLayout> image) {
		return ImageProcHolder::create (instance ())->save (file ,image) ;
	}

	template <class ARG1>
	imports ARG1 sampler (CREF<Image<ARG1>> image ,CREF<FLT64> x ,CREF<FLT64> y) {
		return ImageProcHolder::create (instance ())->sampler (image ,x ,y) ;
	}
} ;

struct SparseNode {
	INDEX mSrc ;
	INDEX mSrcNext ;
	INDEX mDst ;
	INDEX mDstNext ;
} ;

struct SparseLayout {
	Array<INDEX> mTable ;
	ArrayList<SparseNode> mEdge ;
} ;

struct SparseHolder implement Interface {
	imports VFat<SparseHolder> create (VREF<SparseLayout> that) ;
	imports CFat<SparseHolder> create (CREF<SparseLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) = 0 ;
	virtual BOOL contain (CREF<INDEX> from_ ,CREF<INDEX> to_) const = 0 ;
	virtual LENGTH depth (CREF<INDEX> from_) const = 0 ;
	virtual Deque<INDEX> cluster (CREF<INDEX> from_) const = 0 ;
} ;

class Sparse implement SparseLayout {
protected:
	using SparseLayout::mTable ;

public:
	implicit Sparse () = default ;

	explicit Sparse (CREF<LENGTH> size_) {
		SparseHolder::create (thiz)->initialize (size_) ;
	}

	LENGTH size () const {
		return SparseHolder::create (thiz)->size () ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) {
		return SparseHolder::create (thiz)->joint (from_ ,to_) ;
	}

	BOOL contain (CREF<INDEX> from_ ,CREF<INDEX> to_) const {
		return SparseHolder::create (thiz)->contain (from_ ,to_) ;
	}

	LENGTH depth (CREF<INDEX> from_) const {
		return SparseHolder::create (thiz)->depth (from_) ;
	}

	Deque<INDEX> cluster (CREF<INDEX> from_) const {
		return SparseHolder::create (thiz)->cluster (from_) ;
	}
} ;

struct DisjointLayout {
	SharedRef<Array<INDEX>> mTable ;
} ;

struct DisjointHolder implement Interface {
	imports VFat<DisjointHolder> create (VREF<DisjointLayout> that) ;
	imports CFat<DisjointHolder> create (CREF<DisjointLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual INDEX lead (CREF<INDEX> from_) const = 0 ;
	virtual void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) = 0 ;
	virtual BOOL contain (CREF<INDEX> from_ ,CREF<INDEX> to_) const = 0 ;
	virtual LENGTH depth (CREF<INDEX> from_) const = 0 ;
	virtual Deque<INDEX> cluster (CREF<INDEX> from_) const = 0 ;
	virtual Array<INDEX> jump (CREF<INDEX> from_) const = 0 ;
} ;

class Disjoint implement DisjointLayout {
protected:
	using DisjointLayout::mTable ;

public:
	implicit Disjoint () = default ;

	explicit Disjoint (CREF<LENGTH> size_) {
		DisjointHolder::create (thiz)->initialize (size_) ;
	}

	LENGTH size () const {
		return DisjointHolder::create (thiz)->size () ;
	}

	void joint (CREF<INDEX> from_ ,CREF<INDEX> to_) {
		return DisjointHolder::create (thiz)->joint (from_ ,to_) ;
	}

	BOOL contain (CREF<INDEX> from_ ,CREF<INDEX> to_) const {
		return DisjointHolder::create (thiz)->contain (from_ ,to_) ;
	}

	LENGTH depth (CREF<INDEX> from_) const {
		return DisjointHolder::create (thiz)->depth (from_) ;
	}

	Deque<INDEX> cluster (CREF<INDEX> from_) const {
		return DisjointHolder::create (thiz)->cluster (from_) ;
	}

	Array<INDEX> jump (CREF<INDEX> from_) const {
		return DisjointHolder::create (thiz)->jump (from_) ;
	}
} ;

struct KMMatchLayout {
	LENGTH mSize ;
	VAL32 mInfinity ;
	Array<VAL32> mLove ;
	Array<VAL32> mUser ;
	Array<VAL32> mWork ;
	BitSet mUserVisit ;
	BitSet mWorkVisit ;
	Array<INDEX> mMatch ;
	Array<VAL32> mLack ;
} ;

struct KMMatchHolder implement Interface {
	imports VFat<KMMatchHolder> create (VREF<KMMatchLayout> that) ;
	imports CFat<KMMatchHolder> create (CREF<KMMatchLayout> that) ;

	virtual void initialize (CREF<LENGTH> size_) = 0 ;
	virtual Array<INDEX> sort (CREF<Array<VAL32>> love) = 0 ;
} ;

class KMMatch implement KMMatchLayout {
protected:
	using KMMatchLayout::mSize ;
	using KMMatchLayout::mInfinity ;
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
		KMMatchHolder::create (thiz)->initialize (size_) ;
	}

	Array<INDEX> sort (CREF<Array<VAL32>> love) {
		return KMMatchHolder::create (thiz)->sort (love) ;
	}
} ;
} ;
