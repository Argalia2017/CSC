#pragma once

#ifndef __CSC_IMAGES__
#define __CSC_IMAGES__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class SIZE>
class ArrayRange
	:private Proxy {
	_STATIC_ASSERT_ (SIZE::value > 0) ;

private:
	struct Private {
		class Iterator ;
	} ;

private:
	Array<LENGTH ,SIZE> mRange ;

public:
	implicit ArrayRange () = delete ;

	explicit ArrayRange (const Array<LENGTH ,SIZE> &range_) {
		mRange = range_ ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET begin () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Private ,Dependent>::Iterator ;
		return Iterator (PhanRef<const ArrayRange>::make (DEREF[this]) ,0 ,first_item ()) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::Iterator>>
	_RET end () const {
		struct Dependent ;
		using Iterator = typename DEPENDENT_TYPE<Private ,Dependent>::Iterator ;
		const auto r1x = total_length () ;
		return Iterator (PhanRef<const ArrayRange>::make (DEREF[this]) ,r1x ,Array<LENGTH ,SIZE> ()) ;
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
class ArrayRange<SIZE>::Private::Iterator
	:private Proxy {
private:
	PhanRef<const ArrayRange> mBase ;
	INDEX mIndex ;
	Array<LENGTH ,SIZE> mItem ;

public:
	implicit Iterator () = delete ;

	explicit Iterator (PhanRef<const ArrayRange> &&base ,const INDEX &index ,Array<LENGTH ,SIZE> &&item) {
		mBase = _MOVE_ (base) ;
		mIndex = index ;
		mItem = _MOVE_ (item) ;
	}

	inline BOOL operator!= (const Iterator &that) const {
		return BOOL (mIndex != that.mIndex) ;
	}

	inline const Array<LENGTH ,SIZE> &operator* () const leftvalue {
		return mItem ;
	}

	inline void operator++ () {
		mIndex++ ;
		template_incrase (ARGV<DECREASE<SIZE>>::null) ;
	}

private:
	void template_incrase (const ARGVF<ZERO> &) {
		_DEBUG_ASSERT_ (mItem[0] < mBase->mRange[0]) ;
		mItem[0]++ ;
	}

	template <class _ARG1>
	void template_incrase (const ARGVF<_ARG1> &) {
		_STATIC_ASSERT_ (_ARG1::value > 0 && _ARG1::value < LENGTH (SIZE::value)) ;
		mItem[_ARG1::value]++ ;
		if (mItem[_ARG1::value] < mBase->mRange[_ARG1::value])
			return ;
		mItem[_ARG1::value] = 0 ;
		template_incrase (ARGV<DECREASE<_ARG1>>::null) ;
	}
} ;

template <class UNIT>
class Bitmap {
private:
	struct HEAP_PACK {
		SharedRef<FixedBuffer<UNIT>> mBuffer ;
		ARRAY5<LENGTH> mWidth ;
	} ;

	struct Private {
		template <class>
		class Row ;
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

	explicit Bitmap (const LENGTH &cx_ ,const LENGTH &cy_)
		:Bitmap (cx_ ,cy_ ,cx_ ,0) {
		_STATIC_WARNING_ ("noop") ;
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
		mHeap->mWidth[4] = r1x ;
		mImage = PhanBuffer<UNIT>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	explicit Bitmap (PhanBuffer<UNIT> &&image) {
		mHeap = SharedRef<HEAP_PACK>::make () ;
		mHeap->mWidth[0] = mImage.size () ;
		mHeap->mWidth[1] = 1 ;
		mHeap->mWidth[2] = mHeap->mWidth[0] ;
		mHeap->mWidth[3] = 0 ;
		mHeap->mWidth[4] = mImage.size () ;
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
		mHeap->mWidth[4] = mImage.size () ;
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
		const auto r1x = ARRAY5<LENGTH> {0 ,0 ,0 ,0 ,0} ;
		auto &r2x = _SWITCH_ (
			(mHeap.exist ()) ? mHeap->mWidth :
			r1x) ;
		mCX = r2x[0] ;
		mCY = r2x[1] ;
		mCW = r2x[2] ;
		mCK = r2x[3] ;
	}

	void reset (const LENGTH &cx_ ,const LENGTH &cy_ ,const LENGTH &cw_ ,const LENGTH &ck_) {
		_DEBUG_ASSERT_ (cx_ >= 0) ;
		_DEBUG_ASSERT_ (cy_ >= 0) ;
		_DEBUG_ASSERT_ (cx_ <= cw_) ;
		_DEBUG_ASSERT_ (ck_ >= 0) ;
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (cy_ * cw_ + ck_ <= mHeap->mWidth[4]) ;
		mCX = cx_ ;
		mCY = cy_ ;
		mCW = cw_ ;
		mCK = ck_ ;
	}

	Bitmap share () leftvalue {
		Bitmap ret ;
		ret.mHeap = mHeap ;
		ret.mImage = PhanBuffer<UNIT>::make (mImage) ;
		ret.mCX = mCX ;
		ret.mCY = mCY ;
		ret.mCW = mCW ;
		ret.mCK = mCK ;
		return _MOVE_ (ret) ;
	}

	ArrayRange<ARGC<2>> range () const {
		const auto r1x = ARRAY2<LENGTH> {mCY ,mCX} ;
		return ArrayRange<ARGC<2>> (r1x) ;
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
		struct Dependent ;
		using Row = typename DEPENDENT_TYPE<Private ,Dependent>::template Row<Bitmap> ;
		return Row (PhanRef<Bitmap>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<Bitmap>>>
	inline _RET operator[] (const INDEX &y) leftvalue {
		return get (y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const Bitmap>>>
	_RET get (const INDEX &y) const leftvalue {
		struct Dependent ;
		using Row = typename DEPENDENT_TYPE<Private ,Dependent>::template Row<const Bitmap> ;
		return Row (PhanRef<const Bitmap>::make (DEREF[this]) ,y) ;
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
		for (auto &&i : range ()) {
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
		for (auto &&i : range ())
			ret.get (i) = get (i) + that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator+ (const Bitmap &that) const {
		return add (that) ;
	}

	void addto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) - that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator- (const Bitmap &that) const {
		return sub (that) ;
	}

	void subto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) * that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator* (const Bitmap &that) const {
		return mul (that) ;
	}

	void multo (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) / that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator/ (const Bitmap &that) const {
		return div (that) ;
	}

	void divto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) % that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator% (const Bitmap &that) const {
		return mod (that) ;
	}

	void modto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
			get (i) %= that.get (i) ;
	}

	inline Bitmap &operator%= (const Bitmap &that) {
		modto (that) ;
		return DEREF[this] ;
	}

	Bitmap plus () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = +get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator+ () const {
		return plus () ;
	}

	Bitmap minus () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) & that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator& (const Bitmap &that) const {
		return band (that) ;
	}

	void bandto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) | that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator| (const Bitmap &that) const {
		return bor (that) ;
	}

	void borto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
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
		for (auto &&i : range ())
			ret.get (i) = get (i) ^ that.get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator^ (const Bitmap &that) const {
		return bxor (that) ;
	}

	void bxorto (const Bitmap &that) {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		for (auto &&i : range ())
			get (i) ^= that.get (i) ;
	}

	inline Bitmap &operator^= (const Bitmap &that) {
		bxorto (that) ;
		return DEREF[this] ;
	}

	Bitmap bnot () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = ~get (i) ;
		return _MOVE_ (ret) ;
	}

	inline Bitmap operator~ () const {
		return bnot () ;
	}

	Bitmap matrix_product (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCY) ;
		Bitmap ret = Bitmap (that.mCX ,mCY) ;
		const auto r1x = ARRAY2<LENGTH> {mCY ,that.mCX} ;
		for (auto &&i : ArrayRange<ARGC<2>> (r1x)) {
			ret.get (i) = UNIT (0) ;
			for (auto &&j : _RANGE_ (0 ,mCX))
				ret.get (i) += get (i[0] ,j) * that.get (j ,i[1]) ;
		}
		return _MOVE_ (ret) ;
	}

	Bitmap transpose () const {
		Bitmap ret = Bitmap (mCY ,mCX) ;
		for (auto &&i : range ())
			ret.get (i[1] ,i[0]) = get (i) ;
		return _MOVE_ (ret) ;
	}

	Bitmap horizontal_reverse () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i[0] ,(mCX + ~i[1])) ;
		return _MOVE_ (ret) ;
	}

	Bitmap vertical_reverse () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get ((mCY + ~i[0]) ,i[1]) ;
		return _MOVE_ (ret) ;
	}

	void fill (const UNIT &val) {
		for (auto &&i : range ())
			get (i) = val ;
	}
} ;

template <class UNIT>
template <class BASE>
class Bitmap<UNIT>::Private::Row
	:private Proxy {
private:
	PhanRef<BASE> mBase ;
	INDEX mY ;

public:
	implicit Row () = delete ;

	explicit Row (PhanRef<BASE> &&base ,const INDEX &y) {
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
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;
#endif

using COLOR_GRAY = BYTE ;
using COLOR_GRAY32 = VAL32 ;
using COLOR_GRAY64 = VAL64 ;
using COLOR_BGR = ARRAY3<BYTE> ;
using COLOR_BGRA = ARRAY4<BYTE> ;
using COLOR_LAB4 = CHAR ;
using COLOR_XYZ32 = ARRAY3<VAL32> ;
using COLOR_XYZ64 = ARRAY3<VAL64> ;

template <class UNIT>
class AbstractImage {
public:
	struct LAYOUT {
		PTR<ARR<UNIT>> mImage ;
		LENGTH mCX ;
		LENGTH mCY ;
		LENGTH mCW ;
		LENGTH mCK ;
	} ;

	class Abstract
		:public Interface {
	public:
		virtual void compute_layout (AnyRef<void> &holder ,LAYOUT &layout) const = 0 ;
		virtual void compute_load_data (AnyRef<void> &holder ,const LENGTH &cx_ ,const LENGTH &cy_) const = 0 ;
		virtual void compute_load_data (AnyRef<void> &holder ,const AutoBuffer<BYTE> &data) const = 0 ;
		virtual void compute_save_data (const AnyRef<void> &holder ,AutoBuffer<BYTE> &data ,const AnyRef<void> &option) const = 0 ;
		virtual void compute_load_data_file (AnyRef<void> &holder ,const String<STR> &file) const = 0 ;
		virtual void compute_save_data_file (const AnyRef<void> &holder ,const String<STR> &file ,const AnyRef<void> &option) const = 0 ;
	} ;

private:
	struct SELF_PACK {
		AnyRef<void> mHolder ;
		PhanBuffer<UNIT> mImage ;
		LENGTH mCX ;
		LENGTH mCY ;
		LENGTH mCW ;
		LENGTH mCK ;
	} ;

	struct Private {
		template <class>
		class Row ;

		template <class>
		class NativeProxy ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	SharedRef<SELF_PACK> mThis ;

public:
	implicit AbstractImage () = default ;

	explicit AbstractImage (PhanRef<const Abstract> &&abstract_) {
		mAbstract = _MOVE_ (abstract_) ;
		mThis = SharedRef<SELF_PACK>::make () ;
	}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

	ARRAY2<LENGTH> width () const {
		_DEBUG_ASSERT_ (exist ()) ;
		ARRAY2<LENGTH> ret ;
		ret[0] = mThis->mCX ;
		ret[1] = mThis->mCY ;
		return _MOVE_ (ret) ;
	}

	LENGTH cx () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mThis->mCX ;
	}

	LENGTH cy () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mThis->mCY ;
	}

	LENGTH cw () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mThis->mCW ;
	}

	LENGTH ck () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mThis->mCK ;
	}

	ArrayRange<ARGC<2>> range () const {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = ARRAY2<LENGTH> {mThis->mCY ,mThis->mCX} ;
		return ArrayRange<ARGC<2>> (r1x) ;
	}

	UNIT &get (const INDEX &y ,const INDEX &x) leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mThis->mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mThis->mCY) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	const UNIT &get (const INDEX &y ,const INDEX &x) const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mThis->mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mThis->mCY) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
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

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<AbstractImage>>>
	_RET get (const INDEX &y) leftvalue {
		struct Dependent ;
		using Row = typename DEPENDENT_TYPE<Private ,Dependent>::template Row<AbstractImage> ;
		return Row (PhanRef<AbstractImage>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<AbstractImage>>>
	inline _RET operator[] (const INDEX &y) leftvalue {
		return get (y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const AbstractImage>>>
	_RET get (const INDEX &y) const leftvalue {
		struct Dependent ;
		using Row = typename DEPENDENT_TYPE<Private ,Dependent>::template Row<const AbstractImage> ;
		return Row (PhanRef<const AbstractImage>::make (DEREF[this]) ,y) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Row<const AbstractImage>>>
	inline _RET operator[] (const INDEX &y) const leftvalue {
		return get (y) ;
	}

	template <class _ARG1 ,class _RET = REMOVE_CVR_TYPE<typename Private::template NativeProxy<_ARG1>>>
	_RET native (const ARGVF<_ARG1> &) side_effects {
		struct Dependent ;
		using NativeProxy = typename DEPENDENT_TYPE<Private ,Dependent>::template NativeProxy<_ARG1> ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		mThis->mImage = PhanBuffer<UNIT> () ;
		auto tmp = AbstractImage () ;
		tmp.mAbstract = PhanRef<const Abstract>::make (mAbstract) ;
		tmp.mThis = _COPY_ (mThis) ;
		return NativeProxy (_MOVE_ (tmp)) ;
	}

	Bitmap<UNIT> standardize () const {
		_DEBUG_ASSERT_ (exist ()) ;
		Bitmap<UNIT> ret = Bitmap<UNIT> (mThis->mCX ,mThis->mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ;
		return _MOVE_ (ret) ;
	}

	void load_data (const LENGTH &cx_ ,const LENGTH &cy_) {
		_DEBUG_ASSERT_ (cx_ >= 0 && cx_ < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cy_ >= 0 && cy_ < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cx_ * cy_ > 0) ;
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,cx_ ,cy_) ;
		update_layout () ;
	}

	void load_data (const AutoBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data) ;
		update_layout () ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<void> &option) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data ,option) ;
		update_layout () ;
	}

	void load_data_file (const String<STR> &file) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_data_file (mThis->mHolder ,file) ;
		update_layout () ;
	}

	void save_data_file (const String<STR> &file ,const AnyRef<void> &option) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_save_data_file (mThis->mHolder ,file ,option) ;
		update_layout () ;
	}

private:
	void update_layout () {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
		auto rax = LAYOUT () ;
		_ZERO_ (rax) ;
		mAbstract->compute_layout (mThis->mHolder ,rax) ;
		const auto r1x = rax.mCY * rax.mCW + rax.mCK ;
		mThis->mImage = PhanBuffer<UNIT>::make (DEREF[rax.mImage] ,r1x) ;
		mThis->mCX = rax.mCX ;
		mThis->mCY = rax.mCY ;
		mThis->mCW = rax.mCW ;
		mThis->mCK = rax.mCK ;
	}
} ;

template <class UNIT>
template <class BASE>
class AbstractImage<UNIT>::Private::Row
	:private Proxy {
private:
	PhanRef<BASE> mBase ;
	INDEX mY ;

public:
	implicit Row () = delete ;

	explicit Row (PhanRef<BASE> &&base ,const INDEX &y) {
		mBase = _MOVE_ (base) ;
		mY = y ;
	}

	inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (const INDEX &x) rightvalue {
		return mBase->get (mY ,x) ;
	}
} ;

template <class UNIT>
template <class UNIT_>
class AbstractImage<UNIT>::Private::NativeProxy
	:private Proxy {
private:
	UniqueRef<AbstractImage> mBase ;

public:
	implicit NativeProxy () = delete ;

	explicit NativeProxy (AbstractImage &&base) {
		mBase = UniqueRef<AbstractImage> ([&] (AbstractImage &me) {
			me = _MOVE_ (base) ;
		} ,[] (AbstractImage &me) {
			me.update_layout () ;
		}) ;
	}

	inline implicit operator UNIT_ & () const leftvalue {
		_DEBUG_ASSERT_ (mBase->mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mBase->mThis.exist ()) ;
		_DEBUG_ASSERT_ (mBase->mThis->mHolder.exist ()) ;
		return mBase->mThis->mHolder.rebind (ARGV<UNIT_>::null).self ;
	}
} ;
} ;