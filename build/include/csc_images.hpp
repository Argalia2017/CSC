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
			return mIndex != right.mIndex ;
		}

		inline const Array<LENGTH ,SIZE> &operator* () const {
			return mItem ;
		}

		inline void operator++ () {
			mIndex++ ;
			mBase.template_incrase (mItem ,_NULL_<const ARGC<SIZE::value - 1>> ()) ;
		}

	private:
		inline explicit Iterator (const RangeFolder &base ,Array<LENGTH ,SIZE> &&item ,INDEX index) :mBase (base) ,mItem (std::move (item)) ,mIndex (index) {}
	} ;

private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	Array<LENGTH ,SIZE> mRange ;

public:
	inline RangeFolder () = delete ;

	inline explicit RangeFolder (const std::initializer_list<LENGTH> &range) :mRange (range) {}

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

	inline void template_incrase (Array<LENGTH ,SIZE> &index ,const ARGC<0> &) const {
		_DEBUG_ASSERT_ (index[0] < mRange[0]) ;
		index[0]++ ;
	}

	template <INDEX _VAL1>
	inline void template_incrase (Array<LENGTH ,SIZE> &index ,const ARGC<_VAL1> &) const {
		_STATIC_ASSERT_ (_VAL1 > 0 && _VAL1 < SIZE::value) ;
		index[_VAL1]++ ;
		if (index[_VAL1] < mRange[_VAL1])
			return ;
		index[_VAL1] = 0 ;
		template_incrase (index ,_NULL_<const ARGC<_VAL1 - 1>> ()) ;
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

	explicit SoftImage (LENGTH cx ,LENGTH cy) :SoftImage (cx ,cy ,cx ,0) {}

	explicit SoftImage (LENGTH cx ,LENGTH cy ,LENGTH cw ,LENGTH ck) {
		_DEBUG_ASSERT_ (cx >= 0 && cy >= 0 && cx <= cw && ck >= 0) ;
		mHolder = SharedRef<Attribute>::make () ;
		const auto r1x = cy * cw + ck ;
		if (r1x > 0) {
			mHolder->mBuffer = SharedRef<FixedBuffer<TYPE>>::make (r1x) ;
			mImage = PhanBuffer<TYPE>::make (mHolder->mBuffer.self) ;
		}
		mHolder->mWidth[0] = cx ;
		mHolder->mWidth[1] = cy ;
		mHolder->mWidth[2] = cw ;
		mHolder->mWidth[3] = ck ;
		mHolder->mWidth[4] = r1x ;
		reset () ;
	}

	explicit SoftImage (const PhanBuffer<TYPE> &image) {
		_DEBUG_ASSERT_ (image.size () > 0) ;
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mWidth[0] = mImage.size () ;
		mHolder->mWidth[1] = 1 ;
		mHolder->mWidth[2] = mImage.size () ;
		mHolder->mWidth[3] = 0 ;
		mHolder->mWidth[4] = mImage.size () ;
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

	PhanBuffer<TYPE> raw () {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<TYPE>::make (mImage) ;
	}

	PhanBuffer<const TYPE> raw () const {
		_DYNAMIC_ASSERT_ (mImage.size () > 0) ;
		return PhanBuffer<const TYPE>::make (mImage) ;
	}

	void reset () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mCX = mHolder->mWidth[0] ;
		mCY = mHolder->mWidth[1] ;
		mCW = mHolder->mWidth[2] ;
		mCK = mHolder->mWidth[3] ;
	}

	void reset (LENGTH cx ,LENGTH cy ,LENGTH cw ,LENGTH ck) {
		_DEBUG_ASSERT_ (cx >= 0 && cy >= 0 && cx <= cw && ck >= 0) ;
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		_DEBUG_ASSERT_ (cy * cw + ck < mHolder->mWidth[4]) ;
		mCX = cx ;
		mCY = cy ;
		mCW = cw ;
		mCK = ck ;
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

	TYPE &get (INDEX y ,INDEX x) {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	TYPE &get (const ARRAY2<INDEX> &index) {
		return get (index[0] ,index[1]) ;
	}

	inline TYPE &operator[] (const ARRAY2<INDEX> &index) {
		return get (index) ;
	}

	const TYPE &get (INDEX y ,INDEX x) const {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const TYPE &get (const ARRAY2<INDEX> &index) const {
		return get (index[0] ,index[1]) ;
	}

	inline const TYPE &operator[] (const ARRAY2<INDEX> &index) const {
		return get (index) ;
	}

	Row<SoftImage> get (INDEX y) {
		return Row<SoftImage> (*this ,y) ;
	}

	inline Row<SoftImage> operator[] (INDEX y) {
		return get (y) ;
	}

	Row<const SoftImage> get (INDEX y) const {
		return Row<const SoftImage> (*this ,y) ;
	}

	inline Row<const SoftImage> operator[] (INDEX y) const {
		return get (y) ;
	}

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

	SoftImage mul (const TYPE &scale) const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) * scale ;
		return std::move (ret) ;
	}

	inline SoftImage operator* (const TYPE &scale) const {
		return mul (scale) ;
	}

	void multo (const TYPE &scale) {
		for (auto &&i : range ())
			get (i) *= scale ;
	}

	inline SoftImage &operator*= (const TYPE &scale) {
		multo (scale) ;
		return *this ;
	}

	SoftImage div (const TYPE &scale) const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) / scale ;
		return std::move (ret) ;
	}

	inline SoftImage operator/ (const TYPE &scale) const {
		return div (scale) ;
	}

	void divto (const TYPE &scale) {
		for (auto &&i : range ())
			get (i) /= scale ;
	}

	inline SoftImage &operator/= (const TYPE &scale) {
		divto (scale) ;
		return *this ;
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

	SoftImage mul (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCY) ;
		SoftImage ret = SoftImage (right.mCX ,mCY) ;
		for (auto &&i : RangeFolder<ARGC<2>> {mCY ,right.mCX}) {
			ret.get (i) = TYPE (0) ;
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i) += get (i[0] ,j) * right.get (j ,i[1]) ;
		}
		return std::move (ret) ;
	}

	inline SoftImage operator* (const SoftImage &right) const {
		return mul (right) ;
	}

	void multo (const SoftImage &right) {
		*this = mul (right) ;
	}

	inline SoftImage &operator*= (const SoftImage &right) {
		multo (right) ;
		return *this ;
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
		virtual PACK<PTR<ARR<TYPE>> ,LENGTH[4]> layout (AnyRef<void> &_this) const = 0 ;
		virtual void load_data (AnyRef<void> &_this ,LENGTH cx ,LENGTH cy) const = 0 ;
		virtual void load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const = 0 ;
		virtual void save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const = 0 ;
		virtual void load_file (AnyRef<void> &_this ,const String<STR> &file) const = 0 ;
		virtual void save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const = 0 ;
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

	template <class BASE ,class _TYPE>
	class NativeProxy {
	private:
		friend AbstractImage ;
		BASE &mBase ;

	public:
		inline NativeProxy () = delete ;

		inline ~NativeProxy () noexcept {
			_CALL_TRY_ ([&] () {
				mBase.update_layout () ;
			} ,std::nothrow) ;
		}

		inline NativeProxy (const NativeProxy &) = delete ;
		inline NativeProxy &operator= (const NativeProxy &) = delete ;

		inline NativeProxy (NativeProxy &&) noexcept = default ;
		inline NativeProxy &operator= (NativeProxy &&) = delete ;

		inline implicit operator CAST_TRAITS_TYPE<_TYPE ,BASE> & () const & {
			_DEBUG_ASSERT_ (mBase.exist ()) ;
			return mBase.mHolder.template rebind<_TYPE> ().self ;
		}

		inline implicit operator CAST_TRAITS_TYPE<_TYPE ,BASE> & () && = delete ;

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<CAST_TRAITS_TYPE<_TYPE ,BASE> & ,_RET>::value>>
		inline implicit operator _RET () const & {
			_DEBUG_ASSERT_ (mBase.exist ()) ;
			return _RET (mBase.mHolder.template rebind<_TYPE> ().self) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

	private:
		inline explicit NativeProxy (BASE &base) popping : mBase (base) {}
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;
	PhanBuffer<TYPE> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	AbstractImage () = default ;

	explicit AbstractImage (const PhanRef<const Abstract> &engine) :mAbstract (PhanRef<const Abstract>::make (engine)) {}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		return mHolder.exist () ;
	}

	LENGTH cx () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mCX ;
	}

	LENGTH cy () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mCY ;
	}

	LENGTH cw () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mCW ;
	}

	LENGTH ck () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mCK ;
	}

	RangeFolder<ARGC<2>> range () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return RangeFolder<ARGC<2>> ({mCY ,mCX}) ;
	}

	TYPE &get (INDEX y ,INDEX x) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	TYPE &get (const ARRAY2<INDEX> &index) {
		return get (index[0] ,index[1]) ;
	}

	inline TYPE &operator[] (const ARRAY2<INDEX> &index) {
		return get (index) ;
	}

	const TYPE &get (INDEX y ,INDEX x) const {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return mImage[y * mCW + x + mCK] ;
	}

	const TYPE &get (const ARRAY2<INDEX> &index) const {
		return get (index[0] ,index[1]) ;
	}

	inline const TYPE &operator[] (const ARRAY2<INDEX> &index) const {
		return get (index) ;
	}

	Row<AbstractImage> get (INDEX y) {
		return Row<AbstractImage> (*this ,y) ;
	}

	inline Row<AbstractImage> operator[] (INDEX y) {
		return get (y) ;
	}

	Row<const AbstractImage> get (INDEX y) const {
		return Row<const AbstractImage> (*this ,y) ;
	}

	inline Row<const AbstractImage> operator[] (INDEX y) const {
		return get (y) ;
	}

	template <class _RET>
	NativeProxy<AbstractImage ,_RET> native () {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		return NativeProxy<AbstractImage ,_RET> (*this) ;
	}

	SoftImage<TYPE> standardize () const {
		_DEBUG_ASSERT_ (exist ()) ;
		SoftImage<TYPE> ret = SoftImage<TYPE> (mCX ,mCY) ;
		for (auto &&i : range ())
			ret.get (i) = get (i) ;
		return std::move (ret) ;
	}

	void load_data (LENGTH cx ,LENGTH cy) {
		_DEBUG_ASSERT_ (cx >= 0 && cx < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cy >= 0 && cy < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cx * cy > 0) ;
		mAbstract->load_data (mHolder ,cx ,cy) ;
		update_layout () ;
	}

	void load_data (const AutoBuffer<BYTE> &data) {
		mAbstract->load_data (mHolder ,data) ;
		update_layout () ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<void> &param) popping {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->load_data (mHolder ,data ,param) ;
		update_layout () ;
	}

	void load_file (const String<STR> &file) {
		mAbstract->load_file (mHolder ,file) ;
		update_layout () ;
	}

	void save_file (const String<STR> &file ,const AnyRef<void> &param) {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->save_file (mHolder ,file ,param) ;
		update_layout () ;
	}

private:
	void update_layout () {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mAbstract->layout (mHolder) ;
		mImage = PhanBuffer<TYPE>::make (*r1x.P1 ,(r1x.P2[1] * r1x.P2[2] + r1x.P2[3])) ;
		mCX = r1x.P2[0] ;
		mCY = r1x.P2[1] ;
		mCW = r1x.P2[2] ;
		mCK = r1x.P2[3] ;
	}
} ;
} ;