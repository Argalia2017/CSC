#pragma once

#ifndef __CSC_IMAGES__
#define __CSC_IMAGES__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class ITEM ,class RANK>
class Dimension {
private:
	_STATIC_ASSERT_ (RANK::value > 1) ;
	friend Dimension<ITEM ,ARGC<RANK::value + 1>> ;
	PhanBuffer<ITEM> mArray ;
	SharedRef<FixedBuffer<LENGTH>> mWidth ;

public:
	Dimension () = default ;

	explicit Dimension (const PhanBuffer<ITEM> &array ,const std::initializer_list<LENGTH> &width) {
		_DEBUG_ASSERT_ (array.size () == mWidth.self[RANK::value]) ;
		mArray = PhanBuffer<ITEM>::make (array) ;
		mWidth = cvt_size (width) ;
	}

	PhanBuffer<const LENGTH> type () const {
		return PhanBuffer<const LENGTH>::make (mWidth.self) ;
	}

	ARR<ITEM> &to () const {
		_DEBUG_ASSERT_ (mWidth.exist ()) ;
		return mArray ;
	}

	inline implicit operator ARR<ITEM> & () const {
		return to () ;
	}

	LENGTH size () const {
		return mWidth.self[RANK::value] / mWidth.self[RANK::value - 1] ;
	}

	LENGTH length () const {
		return size () ;
	}

	Dimension<ITEM ,ARGC<RANK::value - 1>> get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return Dimension<ITEM ,ARGC<RANK::value - 1>> (&mArray[index * mWidth.self[RANK::value - 1]] ,mWidth) ;
	}

	inline Dimension<ITEM ,ARGC<RANK::value - 1>> operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (INDEX index) const {
		return index + 1 ;
	}

	BOOL equal (const Dimension &right) const {
		return size () == right.size () && _MEMEQUAL_ (mArray.self ,right.mArray.self ,mWidth.self[RANK::value - 1]) ;
	}

	inline BOOL operator== (const Dimension &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Dimension &right) const {
		return !equal (right) ;
	}

private:
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<_ARG ,ARR<REMOVE_EXTEND_TYPE<_ARG>>>::value>>
	explicit Dimension (PTR<_ARG> array ,const SharedRef<FixedBuffer<LENGTH>> &width) :mArray (array) ,mWidth (width) {}

private:
	static SharedRef<FixedBuffer<LENGTH>> cvt_size (const std::initializer_list<LENGTH> &width) {
		_DEBUG_ASSERT_ (width.size () == RANK::value) ;
		SharedRef<FixedBuffer<LENGTH>> ret = SharedRef<FixedBuffer<LENGTH>>::make (RANK::value + 1) ;
		INDEX iw = 0 ;
		ret.self[iw++] = 1 ;
		for (auto &i : Stack<LENGTH> (width)) {
			_DEBUG_ASSERT_ (i > 0) ;
			const auto r1x = ret.self[iw - 1] * i ;
			ret.self[iw++] = r1x ;
		}
		return std::move (ret) ;
	}
} ;

template <class ITEM>
class Dimension<ITEM ,ARGC<1>> {
private:
	using RANK = ARGC<1> ;

private:
	friend Dimension<ITEM ,ARGC<RANK::value + 1>> ;
	PhanBuffer<ITEM> mArray ;
	SharedRef<FixedBuffer<LENGTH>> mWidth ;

public:
	Dimension () = default ;

	explicit Dimension (const PhanBuffer<ITEM> &array ,const std::initializer_list<LENGTH> &width) {
		_DEBUG_ASSERT_ (array.size () == mWidth.self[RANK::value]) ;
		mArray = PhanBuffer<ITEM>::make (array) ;
		mWidth = cvt_size (width) ;
	}

	PhanBuffer<const LENGTH> type () const {
		return PhanBuffer<const LENGTH>::make (mWidth.self) ;
	}

	ARR<ITEM> &to () const {
		_DEBUG_ASSERT_ (mWidth.exist ()) ;
		return mArray ;
	}

	inline implicit operator ARR<ITEM> & () const {
		return to () ;
	}

	LENGTH size () const {
		return mWidth.self[RANK::value] / mWidth.self[RANK::value - 1] ;
	}

	LENGTH length () const {
		return size () ;
	}

	ITEM &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return mArray[index] ;
	}

	inline ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (INDEX index) const {
		return index + 1 ;
	}

	BOOL equal (const Dimension &right) const {
		return size () == right.size () && _MEMEQUAL_ (mArray.self ,right.mArray.self ,mWidth.self[RANK::value - 1]) ;
	}

	inline BOOL operator== (const Dimension &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Dimension &right) const {
		return !equal (right) ;
	}

private:
	explicit Dimension (PTR<ITEM> array ,const SharedRef<FixedBuffer<LENGTH>> &width) :mArray (array) ,mWidth (width) {}

private:
	static SharedRef<FixedBuffer<LENGTH>> cvt_size (const std::initializer_list<LENGTH> &width) {
		_DEBUG_ASSERT_ (width.size () == RANK::value) ;
		SharedRef<FixedBuffer<LENGTH>> ret = SharedRef<FixedBuffer<LENGTH>>::make (RANK::value + 1) ;
		INDEX iw = 0 ;
		ret.self[iw++] = 1 ;
		for (auto &i : Stack<LENGTH> (width)) {
			_DEBUG_ASSERT_ (i > 0) ;
			const auto r1x = ret.self[iw - 1] * i ;
			ret.self[iw++] = r1x ;
		}
		return std::move (ret) ;
	}
} ;

template <class ITEM>
class SoftImage {
private:
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

		inline auto operator[] (INDEX x) const->decltype (mBase.get (mY ,x)) & {
			_DEBUG_ASSERT_ (x >= 0 && x < mBase.mCX) ;
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) :mBase (base) ,mY (y) {}
	} ;

private:
	SharedRef<FixedBuffer<ITEM>> mHolder ;
	PTR<ARR<ITEM>> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	SoftImage () {
		mImage = NULL ;
		mCX = 0 ;
		mCY = 0 ;
		mCW = 0 ;
		mCK = 0 ;
	}

	explicit SoftImage (LENGTH cx ,LENGTH cy) :SoftImage (cx ,cy ,cx ,0) {}

	explicit SoftImage (LENGTH cx ,LENGTH cy ,LENGTH cw ,LENGTH ck) {
		_DEBUG_ASSERT_ (cx >= 0 && cy >= 0 && cx <= cw && ck >= 0) ;
		mHolder = SharedRef<FixedBuffer<ITEM>>::make (cy * cw + ck) ;
		mImage = &mHolder->self ;
		mCX = cx ;
		mCY = cy ;
		mCW = cw ;
		mCK = ck ;
	}

	const ARR<ITEM> &to () const {
		return *mImage ;
	}

	inline implicit operator const ARR<ITEM> & () const {
		return to () ;
	}

	LENGTH size () const {
		return mHolder.exist () ? mHolder->size () : 0 ;
	}

	LENGTH length () const {
		return mCX * mCY ;
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

	ITEM &get (INDEX y ,INDEX x) {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return (*mImage)[y * mCW + x + mCK] ;
	}

	const ITEM &get (INDEX y ,INDEX x) const {
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return (*mImage)[y * mCW + x + mCK] ;
	}

	Row<SoftImage> get (INDEX y) {
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return Row<SoftImage> (*this ,y) ;
	}

	inline Row<SoftImage> operator[] (INDEX y) {
		return get (y) ;
	}

	Row<const SoftImage> get (INDEX y) const {
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return Row<const SoftImage> (*this ,y) ;
	}

	inline Row<const SoftImage> operator[] (INDEX y) const {
		return get (y) ;
	}

	BOOL equal (const SoftImage &right) const {
		if (mCX != right.mCX || mCY != right.mCY)
			return FALSE ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				if (get (i ,j) != right.get (i ,j))
					return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const SoftImage &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftImage &right) const {
		return !equal (right) ;
	}

	void layout (LENGTH cx ,LENGTH cy ,LENGTH cw ,LENGTH ck) {
		_DEBUG_ASSERT_ (cx >= 0 && cy >= 0 && cx <= cw && ck >= 0) ;
		_DEBUG_ASSERT_ (cy * cw + ck < size ()) ;
		mCX = cx ;
		mCY = cy ;
		mCW = cw ;
		mCK = ck ;
	}

	SoftImage mul (const ITEM &scale) const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (i ,j) * scale ;
		return std::move (ret) ;
	}

	inline SoftImage operator* (const ITEM &scale) const {
		return mul (scale) ;
	}

	void multo (const ITEM &scale) {
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				get (i ,j) *= scale ;
	}

	inline SoftImage &operator*= (const ITEM &scale) {
		multo (scale) ;
		return *this ;
	}

	SoftImage div (const ITEM &scale) const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (i ,j) / scale ;
		return std::move (ret) ;
	}

	inline SoftImage operator/ (const ITEM &scale) const {
		return div (scale) ;
	}

	void divto (const ITEM &scale) {
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				get (i ,j) /= scale ;
	}

	inline SoftImage &operator/= (const ITEM &scale) {
		divto (scale) ;
		return *this ;
	}

	SoftImage add (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (i ,j) + right.get (i ,j) ;
		return std::move (ret) ;
	}

	inline SoftImage operator+ (const SoftImage &right) const {
		return add (right) ;
	}

	void addto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				get (i ,j) += right.get (i ,j) ;
	}

	inline SoftImage &operator+= (const SoftImage &right) {
		addto (right) ;
		return *this ;
	}

	SoftImage sub (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (i ,j) - right.get (i ,j) ;
		return std::move (ret) ;
	}

	inline SoftImage operator- (const SoftImage &right) const {
		return sub (right) ;
	}

	void subto (const SoftImage &right) {
		_DEBUG_ASSERT_ (mCX == right.mCX && mCY == right.mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				get (i ,j) -= right.get (i ,j) ;
	}

	inline SoftImage &operator-= (const SoftImage &right) {
		subto (right) ;
		return *this ;
	}

	SoftImage plus () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = +get (i ,j) ;
		return std::move (ret) ;
	}

	inline SoftImage operator+ () const {
		return plus () ;
	}

	SoftImage minus () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = -get (i ,j) ;
		return std::move (ret) ;
	}

	inline SoftImage operator- () const {
		return minus () ;
	}

	SoftImage mul (const SoftImage &right) const {
		_DEBUG_ASSERT_ (mCX == right.mCY) ;
		SoftImage ret = SoftImage (mCY ,right.mCX) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < right.mCX ; j++) {
				ret.get (i ,j) = ITEM (0) ;
				for (INDEX k = 0 ; k < mCX ; k++)
					ret.get (i ,j) += get (i ,k) * right.get (k ,j) ;
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
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (j ,i) = get (i ,j) ;
		return std::move (ret) ;
	}

	SoftImage horizontal_reverse () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (i ,mCX + ~j) ;
		return std::move (ret) ;
	}

	SoftImage vertical_reverse () const {
		SoftImage ret = SoftImage (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (mCY + ~i ,j) ;
		return std::move (ret) ;
	}

	void fill (const ITEM &val) {
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				get (i ,j) = val ;
	}

	template <class _ARG>
	void fill (const Array<ITEM ,_ARG> &val) {
		INDEX ir = 0 ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++) {
				get (i ,j) = val[ir] ;
				ir = (ir + 1) % val.length () ;
			}
	}
} ;

using COLOR_GRAY = BYTE ;
using COLOR_GRAY32 = VAL32 ;
using COLOR_GRAY64 = VAL64 ;
using COLOR_BGR = ARRAY3<BYTE> ;
using COLOR_BGRA = ARRAY4<BYTE> ;
using COLOR_YUV = ARRAY3<VAL32> ;
using COLOR_OPP = ARRAY3<VAL64> ;
using COLOR_HSV = ARRAY3<BYTE> ;

template <class TYPE>
class AbstractImage {
public:
	class Engine :public Interface {
	public:
		virtual void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const = 0 ;
		virtual PACK<PTR<ARR<TYPE>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const = 0 ;
		virtual void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const = 0 ;
		virtual void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const = 0 ;
		virtual void load_file (AnyRef<void> &image ,const String<STR> &file) const = 0 ;
		virtual void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const = 0 ;
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

		inline auto operator[] (INDEX x) const->decltype (mBase.get (mY ,x)) & {
			return mBase.get (mY ,x) ;
		}

	private:
		inline explicit Row (BASE &base ,INDEX y) :mBase (base) ,mY (y) {}
	} ;

	template <class BASE ,class _TYPE>
	class NativeProxy {
	private:
		friend AbstractImage ;
		BASE &mBase ;

	public:
		inline NativeProxy () = delete ;

		inline ~NativeProxy () noexcept {
			_CALL_TRY_ (Function<DEF<void ()> NONE::*> (PhanRef<BASE>::make (&mBase) ,&BASE::native_update) ,std::nothrow) ;
		}

		inline NativeProxy (const NativeProxy &) = delete ;

		inline NativeProxy (NativeProxy &&) noexcept = default ;

		inline CAST_TRAITS_TYPE<_TYPE ,BASE> &to () const & {
			_DEBUG_ASSERT_ (mBase.exist ()) ;
			return mBase.mHolder.template rebind<_TYPE> ().self ;
		}

		inline implicit operator CAST_TRAITS_TYPE<_TYPE ,BASE> & () const & {
			return to () ;
		}

		inline CAST_TRAITS_TYPE<_TYPE ,BASE> &to () && = delete ;

		inline implicit operator CAST_TRAITS_TYPE<_TYPE ,BASE> & () && = delete ;

	private:
		inline explicit NativeProxy (BASE &base) :mBase (base) {}
	} ;

private:
	PhanRef<const Engine> mEngine ;
	AnyRef<void> mHolder ;
	PTR<ARR<TYPE>> mImage ;
	LENGTH mCX ;
	LENGTH mCY ;
	LENGTH mCW ;
	LENGTH mCK ;

public:
	AbstractImage () = default ;

	explicit AbstractImage (const Engine &engine) {
		mEngine = PhanRef<const Engine>::make (&engine) ;
	}

	BOOL exist () const {
		return mEngine.exist () && mHolder.exist () ;
	}

	const ARR<TYPE> &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return *mImage ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
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

	TYPE &get (INDEX y ,INDEX x) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return (*mImage)[y * mCW + x + mCK] ;
	}

	const TYPE &get (INDEX y ,INDEX x) const {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (x >= 0 && x < mCX) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return (*mImage)[y * mCW + x + mCK] ;
	}

	Row<AbstractImage> get (INDEX y) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return Row<AbstractImage> (*this ,y) ;
	}

	inline Row<AbstractImage> operator[] (INDEX y) {
		return get (y) ;
	}

	Row<const AbstractImage> get (INDEX y) const {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (y >= 0 && y < mCY) ;
		return Row<const AbstractImage> (*this ,y) ;
	}

	inline Row<const AbstractImage> operator[] (INDEX y) const {
		return get (y) ;
	}

	BOOL equal (const AbstractImage &right) const {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mCX != right.mCX || mCY != right.mCY)
			return FALSE ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				if (get (i ,j) != right.get (i ,j))
					return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const AbstractImage &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const AbstractImage &right) const {
		return !equal (right) ;
	}

	template <class _RET>
	NativeProxy<AbstractImage ,_RET> native () {
		_DEBUG_ASSERT_ (exist ()) ;
		return NativeProxy<AbstractImage ,_RET> (*this) ;
	}

	template <class _RET>
	NativeProxy<const AbstractImage ,_RET> native () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return NativeProxy<const AbstractImage ,_RET> (*this) ;
	}

	void layout (LENGTH cx ,LENGTH cy) {
		_DEBUG_ASSERT_ (cx > 0 && cx <= VAR32_MAX && cy > 0 && cy <= VAR32_MAX && cx * cy > 0) ;
		mEngine->layout (mHolder ,cx ,cy) ;
		native_update () ;
	}

	void load_file (const String<STR> &file) {
		mEngine->load_file (mHolder ,file) ;
		native_update () ;
	}

	void save_file (const String<STR> &file ,const AnyRef<void> &param) {
		mEngine->save_file (mHolder ,file ,param) ;
		native_update () ;
	}

	AbstractImage clone () const {
		AbstractImage ret = AbstractImage (mEngine) ;
		ret.layout (mCX ,mCY) ;
		for (INDEX i = 0 ; i < mCY ; i++)
			for (INDEX j = 0 ; j < mCX ; j++)
				ret.get (i ,j) = get (i ,j) ;
		return std::move (ret) ;
	}

private:
	void native_update () {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mEngine->watch (mHolder) ;
		mImage = r1x.P1 ;
		mCX = r1x.P2[0] ;
		mCY = r1x.P2[1] ;
		mCW = r1x.P2[2] ;
		mCK = r1x.P2[3] ;
	}

	void native_update () const {
		_DEBUG_ASSERT_ (exist ()) ;
	}
} ;
} ;