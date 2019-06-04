#pragma once

#ifndef __CSC_IMAGES__
#define __CSC_IMAGES__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class SIZE>
class RangeFolder {
private:
	class Iterator {
	private:
		friend RangeFolder ;
		const RangeFolder &mBase ;
		Array<LENGTH ,SIZE> mItem ;
		INDEX mIndex ;

	public:
		inline Iterator () = delete ;

		inline BOOL operator!= (const Iterator &right) const {
			if (mIndex == right.mIndex)
				return FALSE ;
			return TRUE ;
		}

		inline const Array<LENGTH ,SIZE> &operator* () const {
			return mItem ;
		}

		inline void operator++ () {
			mIndex++ ;
			template_incrase (mBase.mRange ,mItem ,_NULL_<const ARGC<SIZE::value - 1>> ()) ;
		}

	private:
		inline explicit Iterator (const RangeFolder &base ,Array<LENGTH ,SIZE> &&item ,INDEX index) :mBase (base) ,mItem (std::move (item)) ,mIndex (index) {}
	} ;

private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	Array<LENGTH ,SIZE> mRange ;

public:
	inline RangeFolder () = delete ;

	inline explicit RangeFolder (const Array<LENGTH ,SIZE> &range) :mRange (range) {}

	inline Iterator begin () const {
		return Iterator (*this ,first_item () ,0) ;
	}

	inline Iterator end () const {
		return Iterator (*this ,first_item () ,total_length ()) ;
	}

private:
	inline LENGTH total_length () const {
		_DEBUG_ASSERT_ (mRange.length () > 0) ;
		LENGTH ret = 1 ;
		for (auto &&i : mRange) {
			_DEBUG_ASSERT_ (i > 0) ;
			_DEBUG_ASSERT_ (ret * i > 0) ;
			ret *= i ;
		}
		return std::move (ret) ;
	}

	inline Array<LENGTH ,SIZE> first_item () const {
		_DEBUG_ASSERT_ (mRange.length () > 0) ;
		Array<LENGTH ,SIZE> ret = Array<LENGTH ,SIZE> (mRange.size ()) ;
		ret.fill (0) ;
		return std::move (ret) ;
	}

	inline static void template_incrase (const Array<LENGTH ,SIZE> &range ,Array<LENGTH ,SIZE> &index ,const ARGC<0> &) {
		_DEBUG_ASSERT_ (index[0] < range[0]) ;
		index[0]++ ;
	}

	template <INDEX _VAL1>
	inline static void template_incrase (const Array<LENGTH ,SIZE> &range ,Array<LENGTH ,SIZE> &index ,const ARGC<_VAL1> &) {
		_STATIC_ASSERT_ (_VAL1 > 0 && _VAL1 < SIZE::value) ;
		index[_VAL1]++ ;
		if (index[_VAL1] < range[_VAL1])
			return ;
		index[_VAL1] = 0 ;
		template_incrase (range ,index ,_NULL_<const ARGC<_VAL1 - 1>> ()) ;
	}
} ;

template <class TYPE>
class SoftImage {
private:
	class Attribute final :private Interface {
	private:
		friend SoftImage ;
		SharedRef<FixedBuffer<TYPE>> mBuffer ;
		ARRAY5<LENGTH> mWidth ;
	} ;

	template <class BASE>
	class Row {
	private:
		friend SoftImage ;
		BASE &mBase ;
		INDEX mY ;

	public:
		inline Row () = delete ;

		inline Row (const Row &) = delete ;

		inline Row (Row &&) noexcept = default ;

		inline CAST_TRAITS_TYPE<TYPE ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) popping : mBase (base) ,mY (y) {}
	} ;

private:
	SharedRef<Attribute> mHolder ;
	PhanBuffer<TYPE> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	SoftImage () {
		mCX = 0 ;
		mCY = 0 ;
		mCW = 0 ;
		mCK = 0 ;
	}

	explicit SoftImage (LENGTH _cx ,LENGTH _cy) :SoftImage (_cx ,_cy ,_cx ,0) {}

	explicit SoftImage (LENGTH _cx ,LENGTH _cy ,LENGTH _cw ,LENGTH _ck) {
		_DEBUG_ASSERT_ (_cx >= 0 && _cy >= 0) ;
		_DEBUG_ASSERT_ (_cx <= _cw) ;
		_DEBUG_ASSERT_ (_ck >= 0) ;
		mHolder = SharedRef<Attribute>::make () ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			const auto r1x = _cy * _cw + _ck ;
			if (r1x == 0)
				continue ;
			mHolder->mBuffer = SharedRef<FixedBuffer<TYPE>>::make (r1x) ;
			mImage = PhanBuffer<TYPE>::make (mHolder->mBuffer.self) ;
		}
		mHolder->mWidth[0] = _cx ;
		mHolder->mWidth[1] = _cy ;
		mHolder->mWidth[2] = _cw ;
		mHolder->mWidth[3] = _ck ;
		reset () ;
	}

	explicit SoftImage (const PhanBuffer<TYPE> &image) {
		_DEBUG_ASSERT_ (image.size () > 0) ;
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mWidth[0] = mImage.size () ;
		mHolder->mWidth[1] = 1 ;
		mHolder->mWidth[2] = mImage.size () ;
		mHolder->mWidth[3] = 0 ;
		mImage = PhanBuffer<TYPE>::make (image) ;
		reset () ;
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

	RangeFolder<ARGC<2>> range () const {
		return RangeFolder<ARGC<2>> ({mCY ,mCX}) ;
	}

	PhanBuffer<TYPE> raw () & {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<TYPE>::make (mImage) ;
	}

	PhanBuffer<const TYPE> raw () const & {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<const TYPE>::make (mImage) ;
	}

	PhanBuffer<TYPE> raw () && = delete ;

	void reset () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mCX = mHolder->mWidth[0] ;
		mCY = mHolder->mWidth[1] ;
		mCW = mHolder->mWidth[2] ;
		mCK = mHolder->mWidth[3] ;
	}

	void reset (LENGTH _cx ,LENGTH _cy ,LENGTH _cw ,LENGTH _ck) {
		_DEBUG_ASSERT_ (_cx >= 0 && _cy >= 0) ;
		_DEBUG_ASSERT_ (_cx <= _cw) ;
		_DEBUG_ASSERT_ (_ck >= 0) ;
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		const auto r1x = mHolder->mWidth[1] * mHolder->mWidth[2] + mHolder->mWidth[3] ;
		_DEBUG_ASSERT_ (_cy * _cw + _ck <= r1x) ;
		(void) r1x ;
		mCX = _cx ;
		mCY = _cy ;
		mCW = _cw ;
		mCK = _ck ;
	}

	SoftImage copy () popping {
		SoftImage ret ;
		ret.mHolder = mHolder ;
		ret.mImage = PhanBuffer<TYPE>::make (mImage) ;
		ret.mCX = mCX ;
		ret.mCY = mCY ;
		ret.mCW = mCW ;
		ret.mCK = mCK ;
		return std::move (ret) ;
	}

	TYPE &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const TYPE &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	TYPE &get (INDEX ,INDEX) && = delete ;

	TYPE &get (const ARRAY2<INDEX> &index) & {
		return get (index[0] ,index[1]) ;
	}

	inline TYPE &operator[] (const ARRAY2<INDEX> &index) & {
		return get (index) ;
	}

	const TYPE &get (const ARRAY2<INDEX> &index) const & {
		return get (index[0] ,index[1]) ;
	}

	inline const TYPE &operator[] (const ARRAY2<INDEX> &index) const & {
		return get (index) ;
	}

	TYPE &get (const ARRAY2<INDEX> &) && = delete ;

	inline TYPE &operator[] (const ARRAY2<INDEX> &) && = delete ;

	Row<SoftImage> get (INDEX y) & {
		return Row<SoftImage> (*this ,y) ;
	}

	inline Row<SoftImage> operator[] (INDEX y) & {
		return get (y) ;
	}

	Row<const SoftImage> get (INDEX y) const & {
		return Row<const SoftImage> (*this ,y) ;
	}

	inline Row<const SoftImage> operator[] (INDEX y) const & {
		return get (y) ;
	}

	Row<SoftImage> get (INDEX) && = delete ;

	inline Row<SoftImage> operator[] (INDEX) && = delete ;

	BOOL equal (const SoftImage &right) const {
		if (mCX != right.mCX || mCY != right.mCY)
			return FALSE ;
		for (auto &&i : range ())
			if (get (i) != right.get (i))
				return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const SoftImage &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftImage &right) const {
		return !equal (right) ;
	}

	SoftImage add (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) + right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator+ (const SoftImage &right) const {
		return add (right) ;
	}

	void addto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) += right.get (i) ;
	}

	inline SoftImage &operator+= (const SoftImage &right) {
		addto (right) ;
		return *this ;
	}

	SoftImage sub (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) - right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator- (const SoftImage &right) const {
		return sub (right) ;
	}

	void subto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) -= right.get (i) ;
	}

	inline SoftImage &operator-= (const SoftImage &right) {
		subto (right) ;
		return *this ;
	}

	SoftImage mul (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) * right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator* (const SoftImage &right) const {
		return mul (right) ;
	}

	void multo (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) *= right.get (i) ;
	}

	inline SoftImage &operator*= (const SoftImage &right) {
		multo (right) ;
		return *this ;
	}

	SoftImage div (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) / right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator/ (const SoftImage &right) const {
		return div (right) ;
	}

	void divto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) /= right.get (i) ;
	}

	inline SoftImage &operator/= (const SoftImage &right) {
		divto (right) ;
		return *this ;
	}

	SoftImage mod (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) % right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator% (const SoftImage &right) const {
		return mod (right) ;
	}

	void modto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) %= right.get (i) ;
	}

	inline SoftImage &operator%= (const SoftImage &right) {
		modto (right) ;
		return *this ;
	}

	SoftImage plus () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = +get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator+ () const {
		return plus () ;
	}

	SoftImage minus () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = -get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator- () const {
		return minus () ;
	}

	SoftImage band (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) & right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator& (const SoftImage &right) const {
		return band (right) ;
	}

	void bandto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) &= right.get (i) ;
	}

	inline SoftImage &operator&= (const SoftImage &right) {
		bandto (right) ;
		return *this ;
	}

	SoftImage bor (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) | right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator| (const SoftImage &right) const {
		return bor (right) ;
	}

	void borto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) |= right.get (i) ;
	}

	inline SoftImage &operator|= (const SoftImage &right) {
		borto (right) ;
		return *this ;
	}

	SoftImage bxor (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ^ right.get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator^ (const SoftImage &right) const {
		return bxor (right) ;
	}

	void bxorto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (auto &&i : range ())
			get (i) ^= right.get (i) ;
	}

	inline SoftImage &operator^= (const SoftImage &right) {
		bxorto (right) ;
		return *this ;
	}

	SoftImage bnot () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = ~get (i) ;
		return std::move (ret) ;
	}

	inline SoftImage operator~ () const {
		return bnot () ;
	}

	SoftImage matrix_product (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCY) ;
		SoftImage ret = SoftImage (right.mCX ,mCY) ;
		for (auto &&i : RangeFolder<ARGC<2>> ({mCY ,right.mCX})) {
			ret.get (i) = TYPE (0) ;
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i) += get (i[0] ,j) * right.get (j ,i[1]) ;
		}
		return std::move (ret) ;
	}

	SoftImage transpose () const {
		SoftImage ret = SoftImage (mCY ,mCX) ;
		for (auto &&i : range ())
			ret.get (i[1] ,i[0]) = get (i) ;
		return std::move (ret) ;
	}

	SoftImage horizontal_reverse () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i[0] ,(mCX + ~i[1])) ;
		return std::move (ret) ;
	}

	SoftImage vertical_reverse () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get ((mCY + ~i[0]) ,i[1]) ;
		return std::move (ret) ;
	}

	void fill (const TYPE &val) {
		for (auto &&i : range ())
			get (i) = val ;
	}
} ;

using COLOR_GRAY = BYTE ;
using COLOR_GRAY32 = VAL32 ;
using COLOR_GRAY64 = VAL64 ;
using COLOR_BGR = ARRAY3<BYTE> ;
using COLOR_BGRA = ARRAY4<BYTE> ;
using COLOR_HSV = CHAR ;
using COLOR_YUV = ARRAY3<VAL32> ;
using COLOR_OPP = ARRAY3<VAL64> ;

template <class TYPE>
class AbstractImage {
public:
	exports struct Abstract :public Interface {
		virtual void compute_layout (AnyRef<void> &_this ,PACK<PTR<ARR<TYPE>> ,LENGTH[4]> &layout) const = 0 ;
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

		inline CAST_TRAITS_TYPE<TYPE ,BASE> &operator[] (INDEX x) && {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) popping : mBase (base) ,mY (y) {}
	} ;

	class Pack {
	private:
		friend AbstractImage ;
		AnyRef<void> mHolder ;
		PhanBuffer<TYPE> mImage ;
		LENGTH mCX ;
		LENGTH mCY ;
		LENGTH mCW ;
		LENGTH mCK ;
	} ;

	template <class _TYPE>
	class NativeProxy {
	private:
		friend AbstractImage ;
		PhanRef<const Abstract> mAbstract ;
		SharedRef<Pack> mThis ;

	public:
		inline NativeProxy () = delete ;

		inline ~NativeProxy () noexcept {
			_CALL_EH_ ([&] () {
				compute_update_layout (mAbstract ,mThis) ;
			}) ;
		}

		inline NativeProxy (const NativeProxy &) = delete ;
		inline NativeProxy &operator= (const NativeProxy &) = delete ;

		inline NativeProxy (NativeProxy &&) noexcept = default ;
		inline NativeProxy &operator= (NativeProxy &&) = delete ;

		inline implicit operator _TYPE & () const & {
			_DEBUG_ASSERT_ (mAbstract.exist ()) ;
			_DEBUG_ASSERT_ (mThis.exist ()) ;
			_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
			return mThis->mHolder.template rebind<_TYPE> ().self ;
		}

		inline implicit operator _TYPE & () && = delete ;

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<_TYPE & ,_RET>::value>>
		inline implicit operator _RET () const & {
			_DEBUG_ASSERT_ (mAbstract.exist ()) ;
			_DEBUG_ASSERT_ (mThis.exist ()) ;
			_DEBUG_ASSERT_ (mThis->mHolder.exist ()) ;
			return _RET (mThis->mHolder.template rebind<_TYPE> ().self) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

	private:
		inline explicit NativeProxy (const PhanRef<const Abstract> &_abstract ,const SharedRef<Pack> &_this) :mAbstract (PhanRef<const Abstract>::make (_abstract)) ,mThis (_this) {}
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	SharedRef<Pack> mThis ;

public:
	AbstractImage () = default ;

	explicit AbstractImage (const PhanRef<const Abstract> &_abstract) :AbstractImage (PhanRef<const Abstract>::make (_abstract) ,SharedRef<Pack>::make ()) {}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
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

	RangeFolder<ARGC<2>> range () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return RangeFolder<ARGC<2>> ({mThis->mCY ,mThis->mCX}) ;
	}

	TYPE &get (INDEX y ,INDEX x) & {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mThis->mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mThis->mCY) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	const TYPE &get (INDEX y ,INDEX x) const & {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mThis->mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mThis->mCY) ;
		_DEBUG_ASSERT_ (mThis->mImage.size () > 0) ;
		return mThis->mImage[y * mThis->mCW + x + mThis->mCK] ;
	}

	TYPE &get (INDEX ,INDEX) && = delete ;

	TYPE &get (const ARRAY2<INDEX> &index) & {
		return get (index[0] ,index[1]) ;
	}

	inline TYPE &operator[] (const ARRAY2<INDEX> &index) & {
		return get (index) ;
	}

	const TYPE &get (const ARRAY2<INDEX> &index) const & {
		return get (index[0] ,index[1]) ;
	}

	inline const TYPE &operator[] (const ARRAY2<INDEX> &index) const & {
		return get (index) ;
	}

	TYPE &get (const ARRAY2<INDEX> &) && = delete ;

	inline TYPE &operator[] (const ARRAY2<INDEX> &) && = delete ;

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
		mThis->mImage = PhanBuffer<TYPE> () ;
		return NativeProxy<_RET> (mAbstract ,mThis) ;
	}

	SoftImage<TYPE> standardize () const {
		_DEBUG_ASSERT_ (exist ()) ;
		SoftImage<TYPE> ret = SoftImage<TYPE> (mThis->mCX ,mThis->mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ;
		return std::move (ret) ;
	}

	void load_data (LENGTH _cx ,LENGTH _cy) {
		_DEBUG_ASSERT_ (_cx >= 0 && _cx < VAR32_MAX) ;
		_DEBUG_ASSERT_ (_cy >= 0 && _cy < VAR32_MAX) ;
		_DEBUG_ASSERT_ (_cx * _cy > 0) ;
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,_cx ,_cy) ;
		compute_update_layout (mAbstract ,mThis) ;
	}

	void load_data (const AutoBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data) ;
		compute_update_layout (mAbstract ,mThis) ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<void> &param) popping {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_load_data (mThis->mHolder ,data ,param) ;
		compute_update_layout (mAbstract ,mThis) ;
	}

	void load_file (const String<STR> &file) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		mAbstract->compute_load_file (mThis->mHolder ,file) ;
		compute_update_layout (mAbstract ,mThis) ;
	}

	void save_file (const String<STR> &file ,const AnyRef<void> &param) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_save_file (mThis->mHolder ,file ,param) ;
		compute_update_layout (mAbstract ,mThis) ;
	}

private:
	explicit AbstractImage (PhanRef<const Abstract> &&_abstract ,SharedRef<Pack> &&_this) :mAbstract (std::move (_abstract)) ,mThis (std::move (_this)) {}

private:
	static void compute_update_layout (PhanRef<const Abstract> &_abstract ,SharedRef<Pack> &_this) {
		_DEBUG_ASSERT_ (_abstract.exist ()) ;
		_DEBUG_ASSERT_ (_this.exist ()) ;
		_DEBUG_ASSERT_ (_this->mHolder.exist ()) ;
		auto rax = PACK<PTR<ARR<TYPE>> ,LENGTH[4]> () ;
		_ZERO_ (rax) ;
		_abstract->compute_layout (_this->mHolder ,rax) ;
		_this->mImage = PhanBuffer<TYPE>::make (*rax.P1 ,(rax.P2[1] * rax.P2[2] + rax.P2[3])) ;
		_this->mCX = rax.P2[0] ;
		_this->mCY = rax.P2[1] ;
		_this->mCW = rax.P2[2] ;
		_this->mCK = rax.P2[3] ;
	}
} ;
} ;