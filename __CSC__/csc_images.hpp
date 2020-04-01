#pragma once

#ifndef __CSC_IMAGES__
#define __CSC_IMAGES__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class SIZE>
class ArrayRange final
	:private Proxy {
	_STATIC_ASSERT_ (SIZE::value > 0) ;

private:
	struct Detail ;
	Array<LENGTH ,SIZE> mRange ;

public:
	inline ArrayRange () = delete ;

	inline explicit ArrayRange (const Array<LENGTH ,SIZE> &range_)
		:mRange (range_) {}

	inline DEF<typename Detail::Iterator> begin () const {
		using Iterator = typename Detail::Iterator ;
		return Iterator ((*this) ,0 ,first_item ()) ;
	}

	inline DEF<typename Detail::Iterator> end () const {
		using Iterator = typename Detail::Iterator ;
		return Iterator ((*this) ,total_length () ,Array<LENGTH ,SIZE> ()) ;
	}

private:
	inline LENGTH total_length () const {
		LENGTH ret = 1 ;
		for (auto &&i : mRange) {
			_DEBUG_ASSERT_ (i >= 0) ;
			ret *= i ;
			_DEBUG_ASSERT_ (ret >= 0) ;
		}
		return std::move (ret) ;
	}

	inline Array<LENGTH ,SIZE> first_item () const {
		Array<LENGTH ,SIZE> ret = Array<LENGTH ,SIZE> (mRange.size ()) ;
		ret.fill (0) ;
		return std::move (ret) ;
	}
} ;

template <class SIZE>
struct ArrayRange<SIZE>::Detail {
	class Iterator final
		:private Proxy {
	private:
		friend ArrayRange ;
		const ArrayRange &mBase ;
		INDEX mIndex ;
		Array<LENGTH ,SIZE> mItem ;

	public:
		inline Iterator () = delete ;

		inline BOOL operator!= (const Iterator &that) const {
			return BOOL (mIndex != that.mIndex) ;
		}

		inline const Array<LENGTH ,SIZE> &operator* () const {
			return mItem ;
		}

		inline void operator++ () {
			mIndex++ ;
			template_incrase (_NULL_<ARGV<DECREASE<SIZE>>> ()) ;
		}

	private:
		inline explicit Iterator (const ArrayRange &base ,INDEX index ,Array<LENGTH ,SIZE> &&item)
			: mBase (base) ,mIndex (index) ,mItem (std::move (item)) {}

	private:
		inline void template_incrase (const ARGV<ZERO> &) {
			_DEBUG_ASSERT_ (mItem[0] < mBase.mRange[0]) ;
			mItem[0]++ ;
		}

		template <class _ARG1>
		inline void template_incrase (const ARGV<_ARG1> &) {
			_STATIC_ASSERT_ (_ARG1::value > 0 && _ARG1::value < LENGTH (SIZE::value)) ;
			mItem[_ARG1::value]++ ;
			if (mItem[_ARG1::value] < mBase.mRange[_ARG1::value])
				return ;
			mItem[_ARG1::value] = 0 ;
			template_incrase (_NULL_<ARGV<DECREASE<_ARG1>>> ()) ;
		}
	} ;
} ;

template <class UNIT>
class Bitmap {
private:
	class Heap {
	private:
		friend Bitmap ;
		SharedRef<FixedBuffer<UNIT>> mBuffer ;
		ARRAY5<LENGTH> mWidth ;
	} ;

private:
	struct Detail ;
	SharedRef<Heap> mHeap ;
	PhanBuffer<UNIT> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	Bitmap () {
		reset () ;
	}

	explicit Bitmap (LENGTH cx_ ,LENGTH cy_)
		:Bitmap (cx_ ,cy_ ,cx_ ,0) {}

	explicit Bitmap (LENGTH cx_ ,LENGTH cy_ ,LENGTH cw_ ,LENGTH ck_) {
		_DEBUG_ASSERT_ (cx_ >= 0) ;
		_DEBUG_ASSERT_ (cy_ >= 0) ;
		_DEBUG_ASSERT_ (cx_ <= cw_) ;
		_DEBUG_ASSERT_ (ck_ >= 0) ;
		mHeap = SharedRef<Heap>::make () ;
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

	explicit Bitmap (const PhanBuffer<UNIT> &image) {
		mHeap = SharedRef<Heap>::make () ;
		mHeap->mWidth[0] = mImage.size () ;
		mHeap->mWidth[1] = 1 ;
		mHeap->mWidth[2] = mHeap->mWidth[0] ;
		mHeap->mWidth[3] = 0 ;
		mHeap->mWidth[4] = mImage.size () ;
		mImage = PhanBuffer<UNIT>::make (image) ;
		reset () ;
	}

	explicit Bitmap (SharedRef<FixedBuffer<UNIT>> &&image) {
		mHeap = SharedRef<Heap>::make () ;
		mHeap->mBuffer = std::move (image) ;
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
		return std::move (ret) ;
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

	PhanBuffer<UNIT> raw () & {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<UNIT>::make (mImage) ;
	}

	PhanBuffer<const UNIT> raw () const & {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<const UNIT>::make (mImage) ;
	}

	auto raw () && ->void = delete ;

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

	void reset (LENGTH cx_ ,LENGTH cy_ ,LENGTH cw_ ,LENGTH ck_) {
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

	Bitmap copy () popping {
		Bitmap ret ;
		ret.mHeap = mHeap ;
		ret.mImage = PhanBuffer<UNIT>::make (mImage) ;
		ret.mCX = mCX ;
		ret.mCY = mCY ;
		ret.mCW = mCW ;
		ret.mCK = mCK ;
		return std::move (ret) ;
	}

	ArrayRange<ARGC<2>> range () const {
		const auto r1x = ARRAY2<LENGTH> {mCY ,mCX} ;
		return ArrayRange<ARGC<2>> (r1x) ;
	}

	UNIT &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const UNIT &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	auto get (INDEX ,INDEX) && ->void = delete ;

	UNIT &get (const ARRAY2<INDEX> &index) & {
		return get (index[0] ,index[1]) ;
	}

	inline UNIT &operator[] (const ARRAY2<INDEX> &index) & {
		return get (index) ;
	}

	const UNIT &get (const ARRAY2<INDEX> &index) const & {
		return get (index[0] ,index[1]) ;
	}

	inline const UNIT &operator[] (const ARRAY2<INDEX> &index) const & {
		return get (index) ;
	}

	auto get (const ARRAY2<INDEX> &) && ->void = delete ;

	inline auto operator[] (const ARRAY2<INDEX> &) && ->void = delete ;

	DEF<typename Detail::template Row<Bitmap>> get (INDEX y) & {
		using Row = typename Detail::template Row<Bitmap> ;
		return Row ((*this) ,y) ;
	}

	inline auto operator[] (INDEX y) &
		->DEF<decltype (_NULL_<Bitmap> ().get (_NULL_<INDEX> ()))> {
		return get (y) ;
	}

	DEF<typename Detail::template Row<const Bitmap>> get (INDEX y) const & {
		using Row = typename Detail::template Row<const Bitmap> ;
		return Row ((*this) ,y) ;
	}

	inline auto operator[] (INDEX y) const &
		->DEF<decltype (_NULL_<const Bitmap> ().get (_NULL_<INDEX> ()))> {
		return get (y) ;
	}

	auto get (INDEX) && ->void = delete ;

	inline auto operator[] (INDEX) && ->void = delete ;

	BOOL equal (const Bitmap &that) const {
		if (mCX != that.mCX)
			return FALSE ;
		if (mCY != that.mCY)
			return FALSE ;
		for (auto &&i : range ())
			if (get (i) != that.get (i))
				return FALSE ;
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
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap sub (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) - that.get (i) ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap mul (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) * that.get (i) ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap div (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) / that.get (i) ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap mod (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) % that.get (i) ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap plus () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = +get (i) ;
		return std::move (ret) ;
	}

	inline Bitmap operator+ () const {
		return plus () ;
	}

	Bitmap minus () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = -get (i) ;
		return std::move (ret) ;
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
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap bor (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) | that.get (i) ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap bxor (const Bitmap &that) const {
		_DEBUG_ASSERT_ (mCX == that.mCX) ;
		_DEBUG_ASSERT_ (mCY == that.mCY) ;
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ^ that.get (i) ;
		return std::move (ret) ;
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
		return (*this) ;
	}

	Bitmap bnot () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = ~get (i) ;
		return std::move (ret) ;
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
		return std::move (ret) ;
	}

	Bitmap transpose () const {
		Bitmap ret = Bitmap (mCY ,mCX) ;
		for (auto &&i : range ())
			ret.get (i[1] ,i[0]) = get (i) ;
		return std::move (ret) ;
	}

	Bitmap horizontal_reverse () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i[0] ,(mCX + ~i[1])) ;
		return std::move (ret) ;
	}

	Bitmap vertical_reverse () const {
		Bitmap ret = Bitmap (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get ((mCY + ~i[0]) ,i[1]) ;
		return std::move (ret) ;
	}

	void fill (const UNIT &val) {
		for (auto &&i : range ())
			get (i) = val ;
	}
} ;

template <class UNIT>
struct Bitmap<UNIT>::Detail {
	template <class BASE>
	class Row final
		:private Proxy {
	private:
		friend Bitmap ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y)
			: mBase (base) ,mY (y) {}
	} ;
} ;

#ifdef __CSC_DEPRECATED__
template <class ITEM>
class Graph {
public:
	Graph () {
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

	exports class Abstract
		:public Interface {
	public:
		virtual void compute_layout (AnyRef<void> &this_ ,LAYOUT &layout) const = 0 ;
		virtual void compute_load_data (AnyRef<void> &this_ ,LENGTH cx_ ,LENGTH cy_) const = 0 ;
		virtual void compute_load_data (AnyRef<void> &this_ ,const AutoBuffer<BYTE> &data) const = 0 ;
		virtual void compute_save_data (const AnyRef<void> &this_ ,AutoBuffer<BYTE> &data ,const AnyRef<void> &option) const = 0 ;
		virtual void compute_load_data_file (AnyRef<void> &this_ ,const String<STR> &file) const = 0 ;
		virtual void compute_save_data_file (const AnyRef<void> &this_ ,const String<STR> &file ,const AnyRef<void> &option) const = 0 ;
	} ;

private:
	class Pack {
	private:
		friend AbstractImage ;
		AnyRef<void> mHolder ;
		PhanBuffer<UNIT> mImage ;
		LENGTH mCX ;
		LENGTH mCY ;
		LENGTH mCW ;
		LENGTH mCK ;
	} ;

private:
	struct Detail ;
	PhanRef<const Abstract> mAbstract ;
	SharedRef<Pack> mThis ;

public:
	AbstractImage () = default ;

	explicit AbstractImage (const PhanRef<const Abstract> &abstract_)
		:AbstractImage (PhanRef<const Abstract>::make (abstract_) ,SharedRef<Pack>::make ()) {}

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
		return std::move (ret) ;
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

	UNIT &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mThis->mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mThis->mCY) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	const UNIT &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mThis->mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mThis->mCY) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	auto get (INDEX ,INDEX) && ->void = delete ;

	UNIT &get (const ARRAY2<INDEX> &index) & {
		return get (index[0] ,index[1]) ;
	}

	inline UNIT &operator[] (const ARRAY2<INDEX> &index) & {
		return get (index) ;
	}

	const UNIT &get (const ARRAY2<INDEX> &index) const & {
		return get (index[0] ,index[1]) ;
	}

	inline const UNIT &operator[] (const ARRAY2<INDEX> &index) const & {
		return get (index) ;
	}

	auto get (const ARRAY2<INDEX> &) && ->void = delete ;

	inline auto operator[] (const ARRAY2<INDEX> &) && ->void = delete ;

	DEF<typename Detail::template Row<AbstractImage>> get (INDEX y) & {
		using AbstractImage = typename Detail::template Row<AbstractImage> ;
		return AbstractImage ((*this) ,y) ;
	}

	inline auto operator[] (INDEX y) &
		->DEF<decltype (_NULL_<AbstractImage> ().get (_NULL_<INDEX> ()))> {
		return get (y) ;
	}

	DEF<typename Detail::template Row<const AbstractImage>> get (INDEX y) const & {
		using AbstractImage = typename Detail::template Row<const AbstractImage> ;
		return AbstractImage ((*this) ,y) ;
	}

	inline auto operator[] (INDEX y) const &
		->DEF<decltype (_NULL_<const AbstractImage> ().get (_NULL_<INDEX> ()))> {
		return get (y) ;
	}

	auto get (INDEX) && ->void = delete ;

	inline auto operator[] (INDEX) && ->void = delete ;

	template <class _RET>
	inline DEF<typename Detail::template NativeProxy<_RET>> native () popping {
		using NativeProxy = typename Detail::template NativeProxy<_RET> ;
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		mThis->mImage = PhanBuffer<UNIT> () ;
		return NativeProxy (mAbstract ,mThis) ;
	}

	Bitmap<UNIT> standardize () const {
		_DEBUG_ASSERT_ (exist ()) ;
		Bitmap<UNIT> ret = Bitmap<UNIT> (mThis->mCX ,mThis->mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ;
		return std::move (ret) ;
	}

	void load_data (LENGTH cx_ ,LENGTH cy_) {
		_DEBUG_ASSERT_ (cx_ >= 0 && cx_ < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cy_ >= 0 && cy_ < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cx_ * cy_ > 0) ;
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,cx_ ,cy_) ;
		static_update_layout (mAbstract ,mThis) ;
	}

	void load_data (const AutoBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data) ;
		static_update_layout (mAbstract ,mThis) ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<void> &option) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data ,option) ;
		static_update_layout (mAbstract ,mThis) ;
	}

	void load_data_file (const String<STR> &file) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_data_file (mThis->mHolder ,file) ;
		static_update_layout (mAbstract ,mThis) ;
	}

	void save_data_file (const String<STR> &file ,const AnyRef<void> &option) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_save_data_file (mThis->mHolder ,file ,option) ;
		static_update_layout (mAbstract ,mThis) ;
	}

private:
	explicit AbstractImage (PhanRef<const Abstract> &&abstract_ ,SharedRef<Pack> &&this_)
		:mAbstract (std::move (abstract_)) ,mThis (std::move (this_)) {}

private:
	inline static void static_update_layout (PhanRef<const Abstract> &abstract_ ,SharedRef<Pack> &this_) {
		_DEBUG_ASSERT_ (abstract_.exist ()) ;
		_DEBUG_ASSERT_ (this_.exist ()) ;
		_DEBUG_ASSERT_ (this_->mHolder.exist ()) ;
		auto rax = LAYOUT () ;
		_ZERO_ (rax) ;
		abstract_->compute_layout (this_->mHolder ,rax) ;
		const auto r1x = rax.mCY * rax.mCW + rax.mCK ;
		this_->mImage = PhanBuffer<UNIT>::make ((*rax.mImage) ,r1x) ;
		this_->mCX = rax.mCX ;
		this_->mCY = rax.mCY ;
		this_->mCW = rax.mCW ;
		this_->mCK = rax.mCK ;
	}
} ;

template <class UNIT>
struct AbstractImage<UNIT>::Detail {
	template <class BASE>
	class Row final
		:private Proxy {
	private:
		friend AbstractImage ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y)
			: mBase (base) ,mY (y) {}
	} ;

	template <class UNIT_>
	class NativeProxy final
		:private Proxy {
	private:
		friend AbstractImage ;
		PhanRef<const Abstract> mAbstract ;
		SharedRef<Pack> mThis ;

	public:
		inline NativeProxy () = delete ;

		inline ~NativeProxy () noexcept {
			_CALL_TRY_ ([&] () {
				static_update_layout (mAbstract ,mThis) ;
			} ,[&] () {
				_STATIC_WARNING_ ("noop") ;
			}) ;
		}

		inline NativeProxy (const NativeProxy &) = default ;
		inline NativeProxy &operator= (const NativeProxy &) = default ;

		inline NativeProxy (NativeProxy &&) = default ;
		inline NativeProxy &operator= (NativeProxy &&) = default ;

		inline implicit operator UNIT_ & () const & {
			_DEBUG_ASSERT_ (mAbstract.exist ()) ;
			_DEBUG_ASSERT_ (mThis.exist ()) ;
			_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
			return mThis->mHolder.template rebind<UNIT_> ().self ;
		}

		inline implicit operator UNIT_ & () && = delete ;

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<UNIT_ & ,_RET>::value>>
		inline implicit operator _RET () const & {
			_DEBUG_ASSERT_ (mAbstract.exist ()) ;
			_DEBUG_ASSERT_ (mThis.exist ()) ;
			_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
			return _RET (_XVALUE_<UNIT_> (mThis->mHolder.template rebind<UNIT_> ().self)) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

	private:
		inline explicit NativeProxy (const PhanRef<const Abstract> &abstract_ ,const SharedRef<Pack> &this_)
			:mAbstract (PhanRef<const Abstract>::make (abstract_)) ,mThis (this_) {}
	} ;
} ;
} ;