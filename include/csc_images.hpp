#pragma once

#ifndef __CSC_IMAGES__
#define __CSC_IMAGES__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class SIZE>
class ArrayRange final {
private:
	class Iterator {
	private:
		friend ArrayRange ;
		const ArrayRange &mBase ;
		Array<LENGTH ,SIZE> mItem ;
		INDEX mIndex ;

	public:
		inline Iterator () = delete ;

		inline BOOL operator== (const Iterator &that) const {
			return BOOL (mIndex == that.mIndex) ;
		}

		inline BOOL operator!= (const Iterator &that) const {
			return BOOL (mIndex != that.mIndex) ;
		}

		inline const Array<LENGTH ,SIZE> &operator* () const {
			return mItem ;
		}

		inline void operator++ () {
			mIndex++ ;
			Detail::template_incrase (mBase.mRange ,mItem ,_NULL_<ARGV<ARGC<SIZE::value - 1>>> ()) ;
		}

	private:
		inline explicit Iterator (const ArrayRange &base ,Array<LENGTH ,SIZE> &&item ,INDEX index) :mBase (base) ,mItem (std::move (item)) ,mIndex (index) {}
	} ;

private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	class Detail ;
	Array<LENGTH ,SIZE> mRange ;

public:
	inline ArrayRange () = delete ;

	inline explicit ArrayRange (const Array<LENGTH ,SIZE> &range) :mRange (range) {}

	inline Iterator begin () const {
		return Iterator (*this ,Detail::first_item (mRange) ,0) ;
	}

	inline Iterator end () const {
		return Iterator (*this ,Detail::first_item (mRange) ,Detail::total_length (mRange)) ;
	}

private:
	class Detail :private Wrapped<void> {
	public:
		inline static LENGTH total_length (const Array<LENGTH ,SIZE> &range) {
			LENGTH ret = 1 ;
			for (auto &&i : range) {
				_DEBUG_ASSERT_ (i >= 0) ;
				_DEBUG_ASSERT_ (ret * i >= 0) ;
				ret *= i ;
			}
			return std::move (ret) ;
		}

		inline static Array<LENGTH ,SIZE> first_item (const Array<LENGTH ,SIZE> &range) {
			Array<LENGTH ,SIZE> ret = Array<LENGTH ,SIZE> (range.size ()) ;
			ret.fill (0) ;
			return std::move (ret) ;
		}

		inline static void template_incrase (const Array<LENGTH ,SIZE> &range ,Array<LENGTH ,SIZE> &index ,const ARGV<ARGC<0>> &) {
			_DEBUG_ASSERT_ (index[0] < range[0]) ;
			index[0]++ ;
		}

		template <class _ARG1>
		inline static void template_incrase (const Array<LENGTH ,SIZE> &range ,Array<LENGTH ,SIZE> &index ,const ARGV<_ARG1> &) {
			_STATIC_ASSERT_ (BOOL (LENGTH (_ARG1::value) > 0 && LENGTH (_ARG1::value) < LENGTH (SIZE::value))) ;
			index[_ARG1::value]++ ;
			if (index[_ARG1::value] < range[_ARG1::value])
				return ;
			index[_ARG1::value] = 0 ;
			template_incrase (range ,index ,_NULL_<ARGV<ARGC<_ARG1::value - 1>>> ()) ;
		}
	} ;
} ;

template <class UNIT>
class Bitmap {
private:
	template <class BASE>
	class Row {
	private:
		friend Bitmap ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline Row (const Row &) = delete ;

		inline Row (Row &&) noexcept = default ;

		inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) popping : mBase (base) ,mY (y) {}
	} ;

	struct HEAP {
		SharedRef<FixedBuffer<UNIT>> mBuffer ;
		ARRAY5<LENGTH> mWidth ;
	} ;

private:
	SharedRef<HEAP> mHeap ;
	PhanBuffer<UNIT> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	Bitmap () {
		reset () ;
	}

	explicit Bitmap (LENGTH _cx ,LENGTH _cy) :Bitmap (_cx ,_cy ,_cx ,0) {}

	explicit Bitmap (LENGTH _cx ,LENGTH _cy ,LENGTH _cw ,LENGTH _ck) {
		_DEBUG_ASSERT_ (_cx >= 0) ;
		_DEBUG_ASSERT_ (_cy >= 0) ;
		_DEBUG_ASSERT_ (_cx <= _cw) ;
		_DEBUG_ASSERT_ (_ck >= 0) ;
		mHeap = SharedRef<HEAP>::make () ;
		const auto r1x = _cy * _cw + _ck ;
		mHeap->mBuffer = SharedRef<FixedBuffer<UNIT>>::make (r1x) ;
		mHeap->mWidth[0] = _cx ;
		mHeap->mWidth[1] = _cy ;
		mHeap->mWidth[2] = _cw ;
		mHeap->mWidth[3] = _ck ;
		mImage = PhanBuffer<UNIT>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	explicit Bitmap (const PhanBuffer<UNIT> &image) {
		mHeap = SharedRef<HEAP>::make () ;
		mHeap->mWidth[0] = mImage.size () ;
		mHeap->mWidth[1] = 1 ;
		mHeap->mWidth[2] = mImage.size () ;
		mHeap->mWidth[3] = 0 ;
		mImage = PhanBuffer<UNIT>::make (image) ;
		reset () ;
	}

	explicit Bitmap (SharedRef<FixedBuffer<UNIT>> &&image) {
		mHeap = SharedRef<HEAP>::make () ;
		mHeap->mBuffer = std::move (image) ;
		mHeap->mWidth[0] = mImage.size () ;
		mHeap->mWidth[1] = 1 ;
		mHeap->mWidth[2] = mImage.size () ;
		mHeap->mWidth[3] = 0 ;
		mImage = PhanBuffer<UNIT>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	ARRAY2<LENGTH> width () const {
		return ARRAY2<LENGTH> {mCX ,mCY} ;
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

	ArrayRange<ARGC<2>> range () const {
		return ArrayRange<ARGC<2>> ({mCY ,mCX}) ;
	}

	PhanBuffer<UNIT> raw () & {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<UNIT>::make (mImage) ;
	}

	PhanBuffer<const UNIT> raw () const & {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<const UNIT>::make (mImage) ;
	}

	PhanBuffer<UNIT> raw () && = delete ;

	void reset () {
		const auto r1x = ARRAY5<LENGTH> {0 ,0 ,0 ,0 ,0} ;
		auto &r1 = (mHeap.exist ()) ? (mHeap->mWidth) : r1x ;
		mCX = r1[0] ;
		mCY = r1[1] ;
		mCW = r1[2] ;
		mCK = r1[3] ;
	}

	void reset (LENGTH _cx ,LENGTH _cy ,LENGTH _cw ,LENGTH _ck) {
		_DEBUG_ASSERT_ (_cx >= 0) ;
		_DEBUG_ASSERT_ (_cy >= 0) ;
		_DEBUG_ASSERT_ (_cx <= _cw) ;
		_DEBUG_ASSERT_ (_ck >= 0) ;
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		const auto r1x = mHeap->mWidth[1] * mHeap->mWidth[2] + mHeap->mWidth[3] ;
		_DEBUG_ASSERT_ (_cy * _cw + _ck <= r1x) ;
		(void) r1x ;
		mCX = _cx ;
		mCY = _cy ;
		mCW = _cw ;
		mCK = _ck ;
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

	UNIT &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (BOOL (x >= 0 && x < mCX)) ;
		_DEBUG_ASSERT_ (BOOL (y >= 0 && y < mCY)) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const UNIT &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (BOOL (x >= 0 && x < mCX)) ;
		_DEBUG_ASSERT_ (BOOL (y >= 0 && y < mCY)) ;
		return mImage[y * mCW + x + mCK] ;
	}

	UNIT &get (INDEX ,INDEX) && = delete ;

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

	UNIT &get (const ARRAY2<INDEX> &) && = delete ;

	inline UNIT &operator[] (const ARRAY2<INDEX> &) && = delete ;

	Row<Bitmap> get (INDEX y) & {
		return Row<Bitmap> (*this ,y) ;
	}

	inline Row<Bitmap> operator[] (INDEX y) & {
		return get (y) ;
	}

	Row<const Bitmap> get (INDEX y) const & {
		return Row<const Bitmap> (*this ,y) ;
	}

	inline Row<const Bitmap> operator[] (INDEX y) const & {
		return get (y) ;
	}

	Row<Bitmap> get (INDEX) && = delete ;

	inline Row<Bitmap> operator[] (INDEX) && = delete ;

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
		return *this ;
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
		return *this ;
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
		return *this ;
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
		return *this ;
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
		return *this ;
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
		return *this ;
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
		return *this ;
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
		return *this ;
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
		for (auto &&i : ArrayRange<ARGC<2>> ({mCY ,that.mCX})) {
			ret.get (i) = UNIT (0) ;
			for (INDEX j = 0 ; j < mCX ; j++)
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
	exports struct Abstract :public Interface {
		virtual void compute_layout (AnyRef<void> &_this ,PACK<PTR<ARR<UNIT>> ,LENGTH[4]> &layout) const = 0 ;
		virtual void compute_load_data (AnyRef<void> &_this ,LENGTH _cx ,LENGTH _cy) const = 0 ;
		virtual void compute_load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const = 0 ;
		virtual void compute_save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const = 0 ;
		virtual void compute_load_file (AnyRef<void> &_this ,const String<STR> &file) const = 0 ;
		virtual void compute_save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const = 0 ;
	} ;

private:
	template <class BASE>
	class Row {
	private:
		friend AbstractImage ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline Row (const Row &) = delete ;

		inline Row (Row &&) noexcept = default ;

		inline CAST_TRAITS_TYPE<UNIT ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) popping : mBase (base) ,mY (y) {}
	} ;

	class Holder {
	private:
		friend AbstractImage ;
		AnyRef<void> mHolder ;
		PhanBuffer<UNIT> mImage ;
		LENGTH mCX ;
		LENGTH mCY ;
		LENGTH mCW ;
		LENGTH mCK ;
	} ;

	template <class _UNIT>
	class NativeProxy {
	private:
		friend AbstractImage ;
		PhanRef<const Abstract> mAbstract ;
		SharedRef<Holder> mThis ;

	public:
		inline NativeProxy () = delete ;

		inline ~NativeProxy () noexcept {
			_CALL_EH_ ([&] () {
				Detail::static_update_layout (mAbstract ,mThis) ;
			}) ;
		}

		inline NativeProxy (const NativeProxy &) = delete ;
		inline NativeProxy &operator= (const NativeProxy &) = delete ;

		inline NativeProxy (NativeProxy &&) noexcept = default ;
		inline NativeProxy &operator= (NativeProxy &&) = delete ;

		inline implicit operator _UNIT & () const & {
			_DEBUG_ASSERT_ (mAbstract.exist ()) ;
			_DEBUG_ASSERT_ (mThis.exist ()) ;
			_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
			return mThis->mHolder.template rebind<_UNIT> ().self ;
		}

		inline implicit operator _UNIT & () && = delete ;

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<_UNIT & ,_RET>::value>>
		inline implicit operator _RET () const & {
			_DEBUG_ASSERT_ (mAbstract.exist ()) ;
			_DEBUG_ASSERT_ (mThis.exist ()) ;
			_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
			return _RET (mThis->mHolder.template rebind<_UNIT> ().self) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

	private:
		inline explicit NativeProxy (const PhanRef<const Abstract> &_abstract ,const SharedRef<Holder> &_this) :mAbstract (PhanRef<const Abstract>::make (_abstract)) ,mThis (_this) {}
	} ;

private:
	class Detail ;
	PhanRef<const Abstract> mAbstract ;
	SharedRef<Holder> mThis ;

public:
	AbstractImage () = default ;

	explicit AbstractImage (const PhanRef<const Abstract> &_abstract) :AbstractImage (PhanRef<const Abstract>::make (_abstract) ,SharedRef<Holder>::make ()) {}

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
		return ARRAY2<LENGTH> {mThis->mCX ,mThis->mCY} ;
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
		return ArrayRange<ARGC<2>> ({mThis->mCY ,mThis->mCX}) ;
	}

	UNIT &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (BOOL (x >= 0 && x < mThis->mCX)) ;
		_DEBUG_ASSERT_ (BOOL (y >= 0 && y < mThis->mCY)) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	const UNIT &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (BOOL (x >= 0 && x < mThis->mCX)) ;
		_DEBUG_ASSERT_ (BOOL (y >= 0 && y < mThis->mCY)) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	UNIT &get (INDEX ,INDEX) && = delete ;

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

	UNIT &get (const ARRAY2<INDEX> &) && = delete ;

	inline UNIT &operator[] (const ARRAY2<INDEX> &) && = delete ;

	Row<AbstractImage> get (INDEX y) & {
		return Row<AbstractImage> (*this ,y) ;
	}

	inline Row<AbstractImage> operator[] (INDEX y) & {
		return get (y) ;
	}

	Row<const AbstractImage> get (INDEX y) const & {
		return Row<const AbstractImage> (*this ,y) ;
	}

	inline Row<const AbstractImage> operator[] (INDEX y) const & {
		return get (y) ;
	}

	Row<AbstractImage> get (INDEX) && = delete ;

	inline Row<AbstractImage> operator[] (INDEX) && = delete ;


	template <class _RET>
	inline NativeProxy<_RET> native () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		mThis->mImage = PhanBuffer<UNIT> () ;
		return NativeProxy<_RET> (mAbstract ,mThis) ;
	}

	Bitmap<UNIT> standardize () const {
		_DEBUG_ASSERT_ (exist ()) ;
		Bitmap<UNIT> ret = Bitmap<UNIT> (mThis->mCX ,mThis->mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ;
		return std::move (ret) ;
	}

	void load_data (LENGTH _cx ,LENGTH _cy) {
		_DEBUG_ASSERT_ (BOOL (_cx >= 0 && _cx < VAR32_MAX)) ;
		_DEBUG_ASSERT_ (BOOL (_cy >= 0 && _cy < VAR32_MAX)) ;
		_DEBUG_ASSERT_ (_cx * _cy > 0) ;
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,_cx ,_cy) ;
		Detail::static_update_layout (mAbstract ,mThis) ;
	}

	void load_data (const AutoBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data) ;
		Detail::static_update_layout (mAbstract ,mThis) ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<void> &param) popping {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data ,param) ;
		Detail::static_update_layout (mAbstract ,mThis) ;
	}

	void load_file (const String<STR> &file) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_file (mThis->mHolder ,file) ;
		Detail::static_update_layout (mAbstract ,mThis) ;
	}

	void save_file (const String<STR> &file ,const AnyRef<void> &param) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_save_file (mThis->mHolder ,file ,param) ;
		Detail::static_update_layout (mAbstract ,mThis) ;
	}

private:
	explicit AbstractImage (PhanRef<const Abstract> &&_abstract ,SharedRef<Holder> &&_this) :mAbstract (std::move (_abstract)) ,mThis (std::move (_this)) {}

private:
	class Detail :private Wrapped<void> {
	public:
		inline static void static_update_layout (PhanRef<const Abstract> &_abstract ,SharedRef<Holder> &_this) {
			_DEBUG_ASSERT_ (_abstract.exist ()) ;
			_DEBUG_ASSERT_ (_this.exist ()) ;
			_DEBUG_ASSERT_ (_this->mHolder.exist ()) ;
			auto rax = PACK<PTR<ARR<UNIT>> ,LENGTH[4]> () ;
			_ZERO_ (rax) ;
			_abstract->compute_layout (_this->mHolder ,rax) ;
			_this->mImage = PhanBuffer<UNIT>::make (*rax.P1 ,(rax.P2[1] * rax.P2[2] + rax.P2[3])) ;
			_this->mCX = rax.P2[0] ;
			_this->mCY = rax.P2[1] ;
			_this->mCW = rax.P2[2] ;
			_this->mCK = rax.P2[3] ;
		}
	} ;
} ;
} ;