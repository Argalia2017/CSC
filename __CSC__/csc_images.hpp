#pragma once

#ifndef __CSC_IMAGES__
#define __CSC_IMAGES__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class SIZE>
class ArrayRange :
	delegate private Proxy {
private:
	_STATIC_ASSERT_ (U::CONSTEXPR_COMPR_GT<SIZE ,ZERO>::compile ()) ;

	struct Private {
		class Iterator ;
	} ;

private:
	Array<LENGTH ,SIZE> mRange ;
	LENGTH mSize ;

public:
	implicit ArrayRange () = delete ;

	explicit ArrayRange (const Array<LENGTH ,SIZE> &range_) {
		mRange = range_ ;
		mSize = total_length () ;
	}

	LENGTH size () const {
		return mSize ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET begin () const {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::Iterator ;
		const auto r1x = first_item () ;
		return R1X (PhanRef<const ArrayRange>::make (DEREF[this]) ,0 ,r1x) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET end () const {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::Iterator ;
		const auto r1x = first_item () ;
		return R1X (PhanRef<const ArrayRange>::make (DEREF[this]) ,mSize ,r1x) ;
	}

private:
	LENGTH total_length () const {
		LENGTH ret = 1 ;
		for (auto &&i : mRange) {
			_DEBUG_ASSERT_ (i >= 0) ;
			ret *= i ;
			_DEBUG_ASSERT_ (ret >= 0) ;
		}
		return _MOVE_ (ret) ;
	}

	Array<LENGTH ,SIZE> first_item () const {
		Array<LENGTH ,SIZE> ret = Array<LENGTH ,SIZE> (mRange.size ()) ;
		ret.fill (0) ;
		return _MOVE_ (ret) ;
	}
} ;

template <class SIZE>
class ArrayRange<SIZE>::Private::Iterator :
	delegate private Proxy {
private:
	PhanRef<const ArrayRange> mBase ;
	INDEX mIndex ;
	Array<LENGTH ,SIZE> mItem ;

public:
	implicit Iterator () = delete ;

	explicit Iterator (REMOVE_CONST_TYPE<PhanRef<const ArrayRange>> &&base ,const INDEX &index ,const Array<LENGTH ,SIZE> &item) {
		mBase = _MOVE_ (base) ;
		mIndex = index ;
		mItem = item ;
	}

	inline BOOL operator!= (const Iterator &that) const {
		return BOOL (mIndex != that.mIndex) ;
	}

	inline const Array<LENGTH ,SIZE> &operator* () const leftvalue {
		return mItem ;
	}

	inline void operator++ () {
		mIndex++ ;
		template_incrase (ARGV<U::CONSTEXPR_DECREASE<SIZE>>::ID) ;
	}

private:
	void template_incrase (const ARGVF<ZERO> &) {
		_DEBUG_ASSERT_ (mItem[0] < mBase->mRange[0]) ;
		mItem[0]++ ;
	}

	template <class _ARG1>
	void template_incrase (const ARGVF<_ARG1> &) {
		using R1X = ARGC_TYPE<U::CONSTEXPR_DECREASE<_ARG1>> ;
		_STATIC_ASSERT_ (U::CONSTEXPR_RANGE_CHECK<R1X ,ZERO ,SIZE>::compile ()) ;
		mItem[_ARG1::compile ()]++ ;
		if (mItem[_ARG1::compile ()] < mBase->mRange[_ARG1::compile ()])
			return ;
		mItem[_ARG1::compile ()] = 0 ;
		template_incrase (ARGV<R1X>::ID) ;
	}
} ;

template <class UNIT>
class Bitmap {
private:
	struct Private {
		template <class>
		class Row ;
	} ;

	struct HEAP_PACK {
		SharedRef<FixedBuffer<UNIT>> mBuffer ;
		ARRAY4<LENGTH> mWidth ;
		LENGTH mFullSize ;
	} ;

private:
	SharedRef<HEAP_PACK> mHeap ;
	PhanBuffer<UNIT> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	implicit Bitmap () {
		reset () ;
	}

	explicit Bitmap (const LENGTH &cx_ ,const LENGTH &cy_) :
		delegate Bitmap (cx_ ,cy_ ,cx_ ,0) {
		_NOOP_ () ;
	}

	explicit Bitmap (const LENGTH &cx_ ,const LENGTH &cy_ ,const LENGTH &cw_ ,const LENGTH &ck_) {
		_DEBUG_ASSERT_ (cx_ >= 0) ;
		_DEBUG_ASSERT_ (cy_ >= 0) ;
		_DEBUG_ASSERT_ (cx_ <= cw_) ;
		_DEBUG_ASSERT_ (ck_ >= 0) ;
		mHeap = SharedRef<HEAP_PACK>::make () ;
		const auto r1x = cy_ * cw_ + ck_ ;
		mHeap->mBuffer = SharedRef<FixedBuffer<UNIT>>::make (r1x) ;
		mHeap->mWidth[0] = cx_ ;
		mHeap->mWidth[1] = cy_ ;
		mHeap->mWidth[2] = cw_ ;
		mHeap->mWidth[3] = ck_ ;
		mHeap->mFullSize = r1x ;
		mImage = PhanBuffer<UNIT>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	explicit Bitmap (REMOVE_CONST_TYPE<PhanBuffer<UNIT>> &&image) {
		mHeap = SharedRef<HEAP_PACK>::make () ;
		mHeap->mWidth[0] = mImage.size () ;
		mHeap->mWidth[1] = 1 ;
		mHeap->mWidth[2] = mHeap->mWidth[0] ;
		mHeap->mWidth[3] = 0 ;
		mHeap->mFullSize = mImage.size () ;
		mImage = _MOVE_ (image) ;
		reset () ;
	}

	explicit Bitmap (const SharedRef<FixedBuffer<UNIT>> &image) {
		mHeap = SharedRef<HEAP_PACK>::make () ;
		mHeap->mBuffer = image ;
		mHeap->mWidth[0] = mImage.size () ;
		mHeap->mWidth[1] = 1 ;
		mHeap->mWidth[2] = mHeap->mWidth[0] ;
		mHeap->mWidth[3] = 0 ;
		mHeap->mFullSize = mImage.size () ;
		mImage = PhanBuffer<UNIT>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	ARRAY2<LENGTH> width () const {
		ARRAY2<LENGTH> ret ;
		ret[0] = mCX ;
		ret[1] = mCY ;
		return _MOVE_ (ret) ;
	}

	LENGTH cx () const {
		return mCX ;
	}

	LENGTH cy () const {
		return mCY ;
	}

	LENGTH cw () const {
		return mCW ;
	}

	LENGTH ck () const {
		return mCK ;
	}

	PhanBuffer<UNIT> raw () leftvalue {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<UNIT>::make (mImage) ;
	}

	PhanBuffer<const UNIT> raw () const leftvalue {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<const UNIT>::make (mImage) ;
	}

	void reset () {
		const auto r1x = _CALL_ ([&] () {
			if (mHeap.exist ())
				return mHeap->mWidth ;
			return ARRAY4<LENGTH> ({0 ,0 ,0 ,0}) ;
		}) ;
		mCX = r1x[0] ;
		mCY = r1x[1] ;
		mCW = r1x[2] ;
		mCK = r1x[3] ;
	}

	void reset (const LENGTH &cx_ ,const LENGTH &cy_ ,const LENGTH &cw_ ,const LENGTH &ck_) {
		_DEBUG_ASSERT_ (cx_ >= 0) ;
		_DEBUG_ASSERT_ (cy_ >= 0) ;
		_DEBUG_ASSERT_ (cx_ <= cw_) ;
		_DEBUG_ASSERT_ (ck_ >= 0) ;
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (cy_ * cw_ + ck_ <= mHeap->mFullSize) ;
		mCX = cx_ ;
		mCY = cy_ ;
		mCW = cw_ ;
		mCK = ck_ ;
	}

	Bitmap share () const {
		Bitmap ret ;
		ret.mHeap = mHeap ;
		ret.mImage = PhanBuffer<UNIT>::make (mImage) ;
		ret.mCX = mCX ;
		ret.mCY = mCY ;
		ret.mCW = mCW ;
		ret.mCK = mCK ;
		return _MOVE_ (ret) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<decltype (_RANGE_ (_NULL_ (ARGV<const ARRAY2<LENGTH>>::ID)))>>
	_RET array_range () const {
		const auto r1x = ARRAY2<LENGTH> ({mCY ,mCX}) ;
		return _RANGE_ (r1x) ;
	}

	UNIT &get (const INDEX &y ,const INDEX &x) leftvalue {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const UNIT &get (const INDEX &y ,const INDEX &x) const leftvalue {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	UNIT &get (const ARRAY2<INDEX> &index) leftvalue {
		return get (index[0] ,index[1]) ;
	}

	inline UNIT &operator[] (const ARRAY2<INDEX> &index) leftvalue {
		return get (index) ;
	}

	const UNIT &get (const ARRAY2<INDEX> &index) const leftvalue {
		return get (index[0] ,index[1]) ;
	}

	inline const UNIT &operator[] (const ARRAY2<INDEX> &index) const leftvalue {
		return get (index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Bitmap>>>
	_RET get (const INDEX &y) leftvalue {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template Row<Bitmap> ;
		return R1X (PhanRef<Bitmap>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Bitmap>>>
	inline _RET operator[] (const INDEX &y) leftvalue {
		return get (y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Bitmap>>>
	_RET get (const INDEX &y) const leftvalue {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template Row<const Bitmap> ;
		return R1X (PhanRef<const Bitmap>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Bitmap>>>
	inline _RET operator[] (const INDEX &y) const leftvalue {
		return get (y) ;
	}

	BOOL equal (const Bitmap &that) const {
		if (mCX != that.mCX)
			return FALSE ;
		if (mCY != that.mCY)
			return FALSE ;
		for (auto &&i : array_range ()) {
			if (get (i) != that.get (i))
				return FALSE ;
		}
		return TRUE ;
	}

	inline BOOL operator== (const Bitmap &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Bitmap &that) const {
		return !equal (that) ;
	}

	Bitmap add (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) + that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator+ (const Bitmap &that) const {
		return add (that) ;
	}

	void addto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) += that.get (i) ;
	}

	inline Bitmap &operator+= (const Bitmap &that) {
		addto (that) ;
		return DEREF[this] ;
	}

	Bitmap sub (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) - that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator- (const Bitmap &that) const {
		return sub (that) ;
	}

	void subto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) -= that.get (i) ;
	}

	inline Bitmap &operator-= (const Bitmap &that) {
		subto (that) ;
		return DEREF[this] ;
	}

	Bitmap mul (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) * that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator* (const Bitmap &that) const {
		return mul (that) ;
	}

	void multo (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) *= that.get (i) ;
	}

	inline Bitmap &operator*= (const Bitmap &that) {
		multo (that) ;
		return DEREF[this] ;
	}

	Bitmap div (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) / that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator/ (const Bitmap &that) const {
		return div (that) ;
	}

	void divto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) /= that.get (i) ;
	}

	inline Bitmap &operator/= (const Bitmap &that) {
		divto (that) ;
		return DEREF[this] ;
	}

	Bitmap mod (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) % that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator% (const Bitmap &that) const {
		return mod (that) ;
	}

	void modto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) %= that.get (i) ;
	}

	inline Bitmap &operator%= (const Bitmap &that) {
		modto (that) ;
		return DEREF[this] ;
	}

	Bitmap plus () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = +get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator+ () const {
		return plus () ;
	}

	Bitmap minus () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = -get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator- () const {
		return minus () ;
	}

	Bitmap band (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) & that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator& (const Bitmap &that) const {
		return band (that) ;
	}

	void bandto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) &= that.get (i) ;
	}

	inline Bitmap &operator&= (const Bitmap &that) {
		bandto (that) ;
		return DEREF[this] ;
	}

	Bitmap bor (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) | that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator| (const Bitmap &that) const {
		return bor (that) ;
	}

	void borto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) |= that.get (i) ;
	}

	inline Bitmap &operator|= (const Bitmap &that) {
		borto (that) ;
		return DEREF[this] ;
	}

	Bitmap bxor (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) ^ that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator^ (const Bitmap &that) const {
		return bxor (that) ;
	}

	void bxorto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : array_range ())
			get (i) ^= that.get (i) ;
	}

	inline Bitmap &operator^= (const Bitmap &that) {
		bxorto (that) ;
		return DEREF[this] ;
	}

	Bitmap bnot () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = ~get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator~ () const {
		return bnot () ;
	}

	Bitmap matrix_product (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCY) ;
		Bitmap ret = Bitmap (that.mCX ,mCY) ;
		const auto r1x = ARRAY2<LENGTH> ({mCY ,that.mCX}) ;
		for (auto &&i : _RANGE_ (r1x)) {
			ret.get (i) = UNIT (0) ;
			for (auto &&j : _RANGE_ (0 ,mCX))
				ret.get (i) += get (i[0] ,j) * that.get (j ,i[1]) ;
		}
		return _MOVE_ (ret) ;
	}

	Bitmap transpose () const {
		Bitmap ret = Bitmap (mCY ,mCX) ;
		for (auto &&i : array_range ())
			ret.get (i[1] ,i[0]) = get (i) ;
		return _MOVE_ (ret) ;
	}

	Bitmap horizontal_reverse () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i[0] ,(mCX + ~i[1])) ;
		return _MOVE_ (ret) ;
	}

	Bitmap vertical_reverse () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get ((mCY + ~i[0]) ,i[1]) ;
		return _MOVE_ (ret) ;
	}

	void fill (const UNIT &val) {
		for (auto &&i : array_range ())
			get (i) = val ;
	}
} ;

template <class UNIT>
template <class BASE>
class Bitmap<UNIT>::Private::Row :
	delegate private Proxy {
private:
	PhanRef<BASE> mBase ;
	INDEX mY ;

public:
	implicit Row () = delete ;

	explicit Row (REMOVE_CONST_TYPE<PhanRef<BASE>> &&base ,const INDEX &y) {
		mBase = _MOVE_ (base) ;
		mY = y ;
	}

	inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (const INDEX &x) rightvalue {
		return mBase->get (mY ,x) ;
	}
} ;

#ifdef __CSC_DEPRECATED__
template <class ITEM>
class Graph {
public:
	implicit Graph () {
		_UNIMPLEMENTED_ () ;
	}
} ;
#endif

template <class UNIT>
class Image {
private:
	struct Private {
		class Implement ;

		template <class>
		class Row ;

		template <class>
		class NativeProxy ;
	} ;

	struct LAYOUT_PACK {
		PTR<ARR<UNIT>> mImage ;
		LENGTH mCX ;
		LENGTH mCY ;
		LENGTH mCW ;
		LENGTH mCK ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual LAYOUT_PACK layout () = 0 ;
		virtual Reference native () = 0 ;
		virtual Reference native () const = 0 ;
		virtual void load_data (const LENGTH &cx_ ,const LENGTH &cy_) = 0 ;
		virtual void load_data (const AutoBuffer<BYTE> &data) = 0 ;
		virtual void save_data (AutoBuffer<BYTE> &data ,const AnyRef<> &option) const = 0 ;
		virtual void load_data_file (const String<STR> &file) = 0 ;
		virtual void save_data_file (const String<STR> &file ,const AnyRef<> &option) const = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;
	PhanBuffer<UNIT> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	implicit Image () ;

	ARRAY2<LENGTH> width () const {
		ARRAY2<LENGTH> ret ;
		ret[0] = mCX ;
		ret[1] = mCY ;
		return _MOVE_ (ret) ;
	}

	LENGTH cx () const {
		if (!mThis.exist ())
			return 0 ;
		return mCX ;
	}

	LENGTH cy () const {
		if (!mThis.exist ())
			return 0 ;
		return mCY ;
	}

	LENGTH cw () const {
		if (!mThis.exist ())
			return 0 ;
		return mCW ;
	}

	LENGTH ck () const {
		if (!mThis.exist ())
			return 0 ;
		return mCK ;
	}

	template <class _RET = REMOVE_CVR_TYPE<decltype (_RANGE_ (_NULL_ (ARGV<const ARRAY2<LENGTH>>::ID)))>>
	_RET array_range () const {
		const auto r1x = ARRAY2<LENGTH> ({mCY ,mCX}) ;
		return _RANGE_ (r1x) ;
	}

	UNIT &get (const INDEX &y ,const INDEX &x) leftvalue {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		_DEBUG_ASSERT_ (mImage.size () > 0) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const UNIT &get (const INDEX &y ,const INDEX &x) const leftvalue {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		_DEBUG_ASSERT_ (mImage.size () > 0) ;
		return mImage[y * mCW + x + mCK] ;
	}

	UNIT &get (const ARRAY2<INDEX> &index) leftvalue {
		return get (index[0] ,index[1]) ;
	}

	inline UNIT &operator[] (const ARRAY2<INDEX> &index) leftvalue {
		return get (index) ;
	}

	const UNIT &get (const ARRAY2<INDEX> &index) const leftvalue {
		return get (index[0] ,index[1]) ;
	}

	inline const UNIT &operator[] (const ARRAY2<INDEX> &index) const leftvalue {
		return get (index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Image>>>
	_RET get (const INDEX &y) leftvalue {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template Row<Image> ;
		return R1X (PhanRef<Image>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Image>>>
	inline _RET operator[] (const INDEX &y) leftvalue {
		return get (y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Image>>>
	_RET get (const INDEX &y) const leftvalue {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template Row<const Image> ;
		return R1X (PhanRef<const Image>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Image>>>
	inline _RET operator[] (const INDEX &y) const leftvalue {
		return get (y) ;
	}

	template <class _ARG1 ,class _RET = REMOVE_CVR_TYPE<typename Private::template NativeProxy<_ARG1>>>
	_RET native (const ARGVF<_ARG1> &) {
		using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template NativeProxy<_ARG1> ;
		return R1X (PhanRef<Image>::make (DEREF[this])) ;
	}

	Bitmap<UNIT> standardize () const {
		Bitmap<UNIT> ret = Bitmap<UNIT> (mCX ,mCY) ;
		for (auto &&i : array_range ())
			ret.get (i) = get (i) ;
		return _MOVE_ (ret) ;
	}

	void load_data (const LENGTH &cx_ ,const LENGTH &cy_) {
		_DEBUG_ASSERT_ (cx_ >= 0 && cx_ < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cy_ >= 0 && cy_ < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cx_ * cy_ > 0) ;
		mThis->load_data (cx_ ,cy_) ;
		update_layout () ;
	}

	void load_data (const AutoBuffer<BYTE> &data) {
		mThis->load_data (data) ;
		update_layout () ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<> &option) {
		mThis->load_data (data ,option) ;
		update_layout () ;
	}

	void load_data_file (const String<STR> &file) {
		mThis->load_data_file (file) ;
		update_layout () ;
	}

	void save_data_file (const String<STR> &file ,const AnyRef<> &option) {
		mThis->save_data_file (file ,option) ;
		update_layout () ;
	}

private:
	void update_layout () {
		const auto r1x = mThis->layout () ;
		const auto r2x = r1x.mCY * r1x.mCW + r1x.mCK ;
		mImage = PhanBuffer<UNIT>::make (DEREF[r1x.mImage] ,r2x) ;
		mCX = r1x.mCX ;
		mCY = r1x.mCY ;
		mCW = r1x.mCW ;
		mCK = r1x.mCK ;
	}
} ;

template <class UNIT>
template <class BASE>
class Image<UNIT>::Private::Row :
	delegate private Proxy {
private:
	PhanRef<BASE> mBase ;
	INDEX mY ;

public:
	implicit Row () = delete ;

	explicit Row (REMOVE_CONST_TYPE<PhanRef<BASE>> &&base ,const INDEX &y) {
		mBase = _MOVE_ (base) ;
		mY = y ;
	}

	inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (const INDEX &x) rightvalue {
		return mBase->get (mY ,x) ;
	}
} ;

template <class UNIT>
template <class UNIT_>
class Image<UNIT>::Private::NativeProxy :
	delegate private Proxy {
private:
	UniqueRef<PhanRef<Image>> mKeep ;

public:
	implicit NativeProxy () = delete ;

	explicit NativeProxy (REMOVE_CONST_TYPE<PhanRef<Image>> &&base) {
		mKeep = UniqueRef<PhanRef<Image>> ([&] (PhanRef<Image> &me) {
			me = _MOVE_ (base) ;
		} ,[] (PhanRef<Image> &me) {
			if (!me.exist ())
				return ;
			me->update_layout () ;
		}) ;
	}

	UNIT_ &to () const leftvalue {
		_DEBUG_ASSERT_ (mKeep.exist ()) ;
		auto &r1x = _FORWARD_ (ARGV<CAST_TRAITS_TYPE<StrongRef<Abstract> ,UNIT_>>::ID ,mKeep.self->mThis) ;
		const auto r2x = r1x->native () ;
		return SafeReference<UNIT_> (r2x) ;
	}

	inline implicit operator UNIT_ & () const leftvalue {
		return self ;
	}
} ;

using COLOR_BGR = ARRAY3<BYTE> ;
using COLOR_BGRA = ARRAY4<BYTE> ;
using COLOR_GRAY = BYTE ;
using COLOR_GRAY32 = VAL32 ;
using COLOR_GRAY64 = VAL64 ;

template <>
exports Image<COLOR_BGR>::Image () ;

template <>
exports Image<COLOR_BGRA>::Image () ;

template <>
exports Image<COLOR_GRAY>::Image () ;

template <>
exports Image<COLOR_GRAY32>::Image () ;

template <>
exports Image<COLOR_GRAY64>::Image () ;
} ;