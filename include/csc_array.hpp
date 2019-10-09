#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"

namespace CSC {
template <class BASE>
class ArrayIterator final {
private:
	using ITERATOR = DEF<decltype (_NULL_<const BASE> ().ibegin ())> ;

private:
	BASE &mBase ;
	ITERATOR mIndex ;

public:
	inline ArrayIterator () = delete ;

	inline ArrayIterator (const ArrayIterator &) = delete ;
	inline ArrayIterator &operator= (const ArrayIterator &) = delete ;

	inline ArrayIterator (ArrayIterator &&) noexcept = default ;
	inline ArrayIterator &operator= (ArrayIterator &&) = delete ;

	inline BOOL operator== (const ArrayIterator &that) const {
		return BOOL (mIndex == that.mIndex) ;
	}

	inline BOOL operator!= (const ArrayIterator &that) const {
		return BOOL (mIndex != that.mIndex) ;
	}

	inline DEF<decltype (_NULL_<BASE> ().get (_NULL_<const ITERATOR> ()))> operator* () const {
		return mBase.get (mIndex) ;
	}

	inline void operator++ () {
		mIndex = _XVALUE_<const BASE> (mBase).inext (mIndex) ;
	}

private:
	inline explicit ArrayIterator (BASE &base ,ITERATOR &&index) popping : mBase (base) ,mIndex (std::move (index)) {}

public:
	inline static ArrayIterator friend_begin (BASE &base) popping {
		return ArrayIterator (base ,_XVALUE_<const BASE> (base).ibegin ()) ;
	}

	inline static ArrayIterator friend_end (BASE &base) popping {
		return ArrayIterator (base ,_XVALUE_<const BASE> (base).iend ()) ;
	}
} ;

inline namespace S {
template <class _ARG1 ,class = ENABLE_TYPE<!std::is_reference<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().ibegin ())>::value>>
inline ArrayIterator<REMOVE_REFERENCE_TYPE<FORWARD_TRAITS_TYPE<_ARG1>>> begin (_ARG1 &&array_) popping {
	return ArrayIterator<REMOVE_REFERENCE_TYPE<FORWARD_TRAITS_TYPE<_ARG1>>>::friend_begin (array_) ;
}

template <class _ARG1 ,class = ENABLE_TYPE<!std::is_reference<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().iend ())>::value>>
inline ArrayIterator<REMOVE_REFERENCE_TYPE<FORWARD_TRAITS_TYPE<_ARG1>>> end (_ARG1 &&array_) popping {
	return ArrayIterator<REMOVE_REFERENCE_TYPE<FORWARD_TRAITS_TYPE<_ARG1>>>::friend_end (array_) ;
}
} ;

template <class SIZE>
class ArrayRange ;

template <>
class ArrayRange<ARGC<0>> final {
private:
	INDEX mIBegin ;
	INDEX mIEnd ;

public:
	inline ArrayRange () = delete ;

	inline explicit ArrayRange (INDEX ibegin_ ,INDEX iend_) :mIBegin (ibegin_) ,mIEnd (iend_) {}

	inline INDEX ibegin () const {
		_DEBUG_ASSERT_ (mIBegin >= 0 && mIBegin <= mIEnd) ;
		return mIBegin ;
	}

	inline INDEX iend () const {
		_DEBUG_ASSERT_ (mIBegin >= 0 && mIBegin <= mIEnd) ;
		return mIEnd ;
	}

	inline INDEX inext (INDEX index) const {
		return index + 1 ;
	}

	inline const INDEX &get (const INDEX &index) const {
		return index ;
	}
} ;

namespace U {
struct OPERATOR_SORT {
	template <class _ARG1 ,class _ARG2>
	inline static void static_insert_sort (const _ARG1 &array ,_ARG2 &out ,INDEX seg_a ,INDEX seg_b) {
		for (INDEX i = seg_a + 1 ,ie = seg_b + 1 ; i < ie ; i++) {
			INDEX ix = i ;
			auto rax = std::move (out[ix]) ;
			while (TRUE) {
				if (ix - 1 < seg_a)
					break ;
				if (array[rax] >= array[out[ix - 1]])
					break ;
				out[ix] = std::move (out[ix - 1]) ;
				ix-- ;
			}
			out[ix] = std::move (rax) ;
		}
	}

	template <class _ARG1 ,class _ARG2>
	inline static void static_quick_sort_partition (const _ARG1 &array ,_ARG2 &out ,INDEX seg_a ,INDEX seg_b ,INDEX &mid_one) {
		INDEX ix = seg_a ;
		INDEX iy = seg_b ;
		auto rax = std::move (out[ix]) ;
		while (TRUE) {
			while (ix < iy && array[rax] < array[out[iy]])
				iy-- ;
			if (ix >= iy)
				break ;
			out[ix++] = std::move (out[iy]) ;
			while (ix < iy && array[rax] > array[out[ix]])
				ix++ ;
			if (ix >= iy)
				break ;
			out[iy--] = std::move (out[ix]) ;
		}
		out[ix] = std::move (rax) ;
		mid_one = ix ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void static_quick_sort (const _ARG1 &array ,_ARG2 &out ,INDEX seg_a ,INDEX seg_b ,LENGTH ideal) {
		INDEX ix = seg_a ;
		while (TRUE) {
			if (ix >= seg_b)
				break ;
			if (ideal <= 0)
				break ;
			ideal = ideal / 2 + ideal / 4 ;
			INDEX jx = VAR_NONE ;
			static_quick_sort_partition (array ,out ,ix ,seg_b ,jx) ;
			static_quick_sort (array ,out ,ix ,(jx - 1) ,ideal) ;
			ix = jx + 1 ;
		}
		if (ix >= seg_b)
			return ;
		static_insert_sort (array ,out ,ix ,seg_b) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void invoke (const _ARG1 &array ,_ARG2 &out ,INDEX seg ,LENGTH seg_len) {
		_DEBUG_ASSERT_ (seg_len > 0) ;
		_DEBUG_ASSERT_ (seg >= 0 && seg <= out.size () - seg_len) ;
		static_quick_sort (array ,out ,seg ,(seg + seg_len - 1) ,seg_len) ;
	}

	class ForwardArray :private Wrapped<decltype (ARGVPY)> {
	public:
		template <class _ARG1>
		inline constexpr _ARG1 &operator[] (_ARG1 &val) const {
			return val ;
		}
	} ;

	template <class _ARG1>
	inline static void invoke (_ARG1 &out ,INDEX seg ,LENGTH seg_len) {
		invoke (_CAST_<ForwardArray> (ARGVPY) ,out ,seg ,seg_len) ;
	}
} ;
} ;

namespace U {
struct OPERATOR_HASH {
	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().hash ())> ,FLAG>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		return self_.hash () ;
	}

	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<std::is_pod<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		return _MEMHASH_ (PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (self_)] ,_SIZEOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline static FLAG invoke (const _ARG1 &self_) {
		FLAG ret = template_hash (self_ ,ARGVPX ,ARGVP9) ;
		ret &= VAR_MAX ;
		return std::move (ret) ;
	}
} ;
} ;

template <class ITEM ,class SIZE = SAUTO>
class Array ;

template <class ITEM ,class SIZE>
class Array {
private:
	Buffer<ITEM ,SIZE> mArray ;

public:
	Array () = default ;

	explicit Array (LENGTH len) :mArray (len) {}

	implicit Array (const std::initializer_list<ITEM> &that) : Array (that.size ()) {
		_DEBUG_ASSERT_ (size () == LENGTH (that.size ())) ;
		INDEX iw = 0 ;
		for (auto &&i : that)
			mArray[iw++] = i ;
		_DEBUG_ASSERT_ (iw == mArray.size ()) ;
	}

	LENGTH size () const {
		return mArray.size () ;
	}

	LENGTH length () const {
		return size () ;
	}

	PhanBuffer<ITEM> raw () & {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<ITEM>::make (mArray ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const & {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mArray ,length ()) ;
	}

	PhanBuffer<ITEM> raw () && = delete ;

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (INDEX index) const {
		return index + 1 ;
	}

	ITEM &get (INDEX index) & {
		return mArray[index] ;
	}

	inline ITEM &operator[] (INDEX index) & {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const & {
		return mArray[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const & {
		return get (index) ;
	}

	ITEM &get (INDEX) && = delete ;

	inline ITEM &operator[] (INDEX) && = delete ;

	INDEX at (const ITEM &item) const {
		return mArray.at (item) ;
	}

	BOOL equal (const Array &that) const {
		return BOOL (mArray == that.mArray) ;
	}

	inline BOOL operator== (const Array &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Array &that) const {
		return !equal (that) ;
	}

	FLAG compr (const Array &that) const {
		return mArray.compr (that.mArray) ;
	}

	inline BOOL operator< (const Array &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Array &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Array &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Array &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	void sort () {
		U::OPERATOR_SORT::invoke (mArray ,0 ,mArray.size ()) ;
	}

	void fill (const ITEM &val) {
		for (INDEX i = 0 ,ie = mArray.size () ; i < ie ; i++)
			mArray[i] = val ;
	}
} ;

template <class ITEM>
using ARRAY1 = Array<ITEM ,ARGC<1>> ;
template <class ITEM>
using ARRAY2 = Array<ITEM ,ARGC<2>> ;
template <class ITEM>
using ARRAY3 = Array<ITEM ,ARGC<3>> ;
template <class ITEM>
using ARRAY4 = Array<ITEM ,ARGC<4>> ;
template <class ITEM>
using ARRAY5 = Array<ITEM ,ARGC<5>> ;
template <class ITEM>
using ARRAY6 = Array<ITEM ,ARGC<6>> ;
template <class ITEM>
using ARRAY7 = Array<ITEM ,ARGC<7>> ;
template <class ITEM>
using ARRAY8 = Array<ITEM ,ARGC<8>> ;
template <class ITEM>
using ARRAY9 = Array<ITEM ,ARGC<9>> ;
template <class ITEM>
using ARRAY10 = Array<ITEM ,ARGC<10>> ;
template <class ITEM>
using ARRAY11 = Array<ITEM ,ARGC<11>> ;
template <class ITEM>
using ARRAY12 = Array<ITEM ,ARGC<12>> ;
template <class ITEM>
using ARRAY13 = Array<ITEM ,ARGC<13>> ;
template <class ITEM>
using ARRAY14 = Array<ITEM ,ARGC<14>> ;
template <class ITEM>
using ARRAY15 = Array<ITEM ,ARGC<15>> ;
template <class ITEM>
using ARRAY16 = Array<ITEM ,ARGC<16>> ;

template <class ITEM ,class SIZE = SAUTO>
class String ;

template <class ITEM ,class SIZE>
class String {
private:
	inline static constexpr LENGTH constexpr_size (LENGTH len) {
		return _SWITCH_ (
			(len <= 0) ? len :
			(len + 1)) ;
	}

private:
	class Detail ;
	Buffer<ITEM ,ARGC<constexpr_size (SIZE::value)>> mString ;

public:
	String () {
		clear () ;
	}

	explicit String (LENGTH len) :String (ARGVP0 ,constexpr_size (len)) {
		clear () ;
	}

	implicit String (const std::initializer_list<ITEM> &that) : String (that.size ()) {
		_DEBUG_ASSERT_ (size () == LENGTH (that.size ())) ;
		INDEX iw = 0 ;
		for (auto &&i : that)
			mString[iw++] = i ;
		mString[iw++] = ITEM (0) ;
		_DEBUG_ASSERT_ (iw == mString.size ()) ;
	}

	implicit String (const ARR<ITEM> &that) :String (Detail::plain_string_length (that)) {
		_MEMCOPY_ (mString.self ,that ,size ()) ;
	}

	implicit String (const Plain<ITEM> &that) : String (that.size ()) {
		_MEMCOPY_ (mString.self ,that.self ,size ()) ;
	}

	LENGTH size () const {
		if (mString.size () == 0)
			return 0 ;
		return mString.size () - 1 ;
	}

	LENGTH length () const {
		if (mString.size () == 0)
			return 0 ;
		return _MEMCHR_ (mString.self ,mString.size () ,ITEM (0)) ;
	}

	PhanBuffer<ITEM> raw () & {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<ITEM>::make (mString ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const & {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mString ,length ()) ;
	}

	PhanBuffer<ITEM> raw () && = delete ;

	void clear () {
		if (mString.size () == 0)
			return ;
		_MEMFILL_ (mString.self ,mString.size () ,ITEM (0)) ;
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

	ITEM &get (INDEX index) & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mString[index] ;
	}

	inline ITEM &operator[] (INDEX index) & {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mString[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const & {
		return get (index) ;
	}

	ITEM &get (INDEX) && = delete ;

	inline ITEM &operator[] (INDEX) && = delete ;

	INDEX at (const ITEM &item) const {
		INDEX ret = mString.at (item) ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	BOOL equal (const String &that) const {
		if (empty () && that.empty ())
			return TRUE ;
		if (empty ())
			return FALSE ;
		if (that.empty ())
			return FALSE ;
		INDEX ix = 0 ;
		while (mString[ix] != ITEM (0) && mString[ix] == that.mString[ix])
			ix++ ;
		if (mString[ix] != that.mString[ix])
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const String &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const String &that) const {
		return !equal (that) ;
	}

	BOOL equal (const Plain<ITEM> &that) const {
		if (mString.size () < that.size () + 1)
			return FALSE ;
		if (!_MEMEQUAL_ (mString.self ,that.self ,(that.size () + 1)))
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Plain<ITEM> &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Plain<ITEM> &that) const {
		return !equal (that) ;
	}

	FLAG compr (const String &that) const {
		const auto r1x = size () ;
		const auto r2x = that.size () ;
		if (r1x == 0)
			return _MEMCOMPR_ (PTRTOARR[&r1x] ,PTRTOARR[&r2x] ,1) ;
		if (r2x == 0)
			return _MEMCOMPR_ (PTRTOARR[&r1x] ,PTRTOARR[&r2x] ,1) ;
		INDEX ix = 0 ;
		while (mString[ix] != ITEM (0) && mString[ix] == that.mString[ix])
			ix++ ;
		return _MEMCOMPR_ (PTRTOARR[&mString[ix]] ,PTRTOARR[&that.mString[ix]] ,1) ;
	}

	inline BOOL operator< (const String &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const String &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const String &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const String &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	BOOL empty () const {
		if (mString.size () == 0)
			return TRUE ;
		if (mString[0] != ITEM (0))
			return FALSE ;
		return TRUE ;
	}

	String concat (const String &that) const {
		const auto r1x = length () ;
		const auto r2x = that.length () ;
		String ret = String (r1x + r2x) ;
		_MEMCOPY_ (ret.mString.self ,mString.self ,r1x) ;
		_MEMCOPY_ (PTRTOARR[&ret.mString.self[r1x]] ,that.mString.self ,r2x) ;
		return std::move (ret) ;
	}

	inline String operator+ (const String &that) const {
		return concat (that) ;
	}

	inline String operator- (const String &that) const {
		return that.concat ((*this)) ;
	}

	void concatto (const String &that) {
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(mString.size () > 0))
				discard ;
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			if (!(r1x + r2x <= size ()))
				discard ;
			_MEMCOPY_ (PTRTOARR[&mString.self[r1x]] ,that.mString.self ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		}
		if SWITCH_CASE (fax) {
			(*this) = concat (that) ;
		}
	}

	inline String &operator+= (const String &that) {
		concatto (that) ;
		return (*this) ;
	}

	void concatto (const Plain<ITEM> &that) {
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(mString.size () > 0))
				discard ;
			const auto r1x = length () ;
			const auto r2x = that.size () ;
			if (!(r1x + r2x <= size ()))
				discard ;
			_MEMCOPY_ (PTRTOARR[&mString.self[r1x]] ,that.self ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		}
		if SWITCH_CASE (fax) {
			(*this) = concat (that) ;
		}
	}

	inline String &operator+= (const Plain<ITEM> &that) {
		concatto (that) ;
		return (*this) ;
	}

	String segment (INDEX seg ,LENGTH seg_len) const {
		_DEBUG_ASSERT_ (seg_len > 0) ;
		_DEBUG_ASSERT_ (seg >= 0 && seg <= size () - seg_len) ;
		String ret = String (seg_len) ;
		for (INDEX i = 0 ,ie = ret.size () ; i < ie ; i++)
			ret.get (i) = get (seg + i) ;
		return std::move (ret) ;
	}

private:
	explicit String (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mString (len) {}

public:
	//@info: this function is incompleted without 'csc_string.hpp'
	template <class... _ARGS>
	inline static String make (const _ARGS &...initval) ;

private:
	class Detail :private Wrapped<void> {
	public:
		inline static LENGTH plain_string_length (const ARR<ITEM> &val) {
			using DEFAULT_HUGESTRING_SIZE = ARGC<8388607> ;
			LENGTH ret = _MEMCHR_ (val ,(DEFAULT_HUGESTRING_SIZE::value + 1) ,ITEM (0)) ;
			_DYNAMIC_ASSERT_ (ret >= 0 && ret <= DEFAULT_HUGESTRING_SIZE::value) ;
			return std::move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = SAUTO>
class Deque ;

template <class ITEM ,class SIZE>
class Deque {
private:
	class AccessArray :private Wrapped<Deque> {
	public:
		inline ITEM &operator[] (INDEX index) {
			return AccessArray::mSelf[AccessArray::mSelf.access (index)] ;
		}

		inline const ITEM &operator[] (INDEX index) const {
			return AccessArray::mSelf[AccessArray::mSelf.access (index)] ;
		}
	} ;

	inline static constexpr LENGTH constexpr_size (LENGTH len) {
		return _SWITCH_ (
			(len <= 0) ? len :
			(len + 1)) ;
	}

private:
	Buffer<ITEM ,ARGC<constexpr_size (SIZE::value)>> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	Deque () {
		clear () ;
	}

	explicit Deque (LENGTH len) :Deque (ARGVP0 ,constexpr_size (len)) {
		clear () ;
	}

	implicit Deque (const std::initializer_list<ITEM> &that) : Deque (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		if (mDeque.size () == 0)
			return 0 ;
		return mDeque.size () - 1 ;
	}

	LENGTH length () const {
		if (mDeque.size () == 0)
			return 0 ;
		return (mWrite - mRead + mDeque.size ()) % mDeque.size () ;
	}

	void clear () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	INDEX ibegin () const {
		if (mDeque.size () == 0)
			return VAR_NONE ;
		return mRead ;
	}

	INDEX iend () const {
		if (mDeque.size () == 0)
			return VAR_NONE ;
		return mWrite ;
	}

	INDEX inext (INDEX index) const {
		return (index + 1) % mDeque.size () ;
	}

	//@warn: index would be no longer valid once resized
	ITEM &get (INDEX index) & {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mDeque[index] ;
	}

	inline ITEM &operator[] (INDEX index) & {
		return get (index) ;
	}

	//@warn: index would be no longer valid once resized
	const ITEM &get (INDEX index) const & {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mDeque[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const & {
		return get (index) ;
	}

	ITEM &get (INDEX) && = delete ;

	inline ITEM &operator[] (INDEX) && = delete ;

	INDEX at (const ITEM &item) const {
		INDEX ret = mDeque.at (item) ;
		if (!ensure_index (ret))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	INDEX access (INDEX pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		return (mRead + pos) % mDeque.size () ;
	}

	BOOL equal (const Deque &that) const {
		if (length () != that.length ())
			return FALSE ;
		INDEX ib = ibegin () ;
		INDEX ie = iend () ;
		INDEX jb = that.ibegin () ;
		INDEX je = that.iend () ;
		while (TRUE) {
			if (ib == ie)
				break ;
			if (jb == je)
				break ;
			if (get (ib) != that.get (jb))
				break ;
			ib = inext (ib) ;
			jb = that.inext (jb) ;
		}
		if (ib != ie)
			return FALSE ;
		if (jb != je)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Deque &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Deque &that) const {
		return !equal (that) ;
	}

	BOOL empty () const {
		if (mRead != mWrite)
			return FALSE ;
		return TRUE ;
	}

	BOOL full () const {
		if (mDeque.size () == 0)
			return TRUE ;
		if (mRead != (mWrite + 1) % mDeque.size ())
			return FALSE ;
		return TRUE ;
	}

	void add (const ITEM &item) {
		reserve (1) ;
		mDeque[mWrite] = std::move (item) ;
		update_resize () ;
	}

	inline Deque &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	void add (ITEM &&item) {
		reserve (1) ;
		mDeque[mWrite] = std::move (item) ;
		update_resize () ;
	}

	inline Deque &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mRead = (mRead + 1) % mDeque.size () ;
	}

	void take (ITEM &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mDeque[mRead]) ;
		mRead = (mRead + 1) % mDeque.size () ;
	}

	inline Deque &operator>> (ITEM &item) popping {
		take (item) ;
		return (*this) ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mRead ;
	}

	INDEX tail () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
	}

	INDEX insert () popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		update_resize () ;
		return std::move (ret) ;
	}

	INDEX insert_sort (const ITEM &item) popping {
		_DEBUG_ASSERT_ (mRead == 0) ;
		reserve (1) ;
		INDEX ret = mWrite ;
		while (TRUE) {
			if (ret - 1 < 0)
				break ;
			if (mDeque[ret - 1] >= item)
				break ;
			mDeque[ret] = std::move (mDeque[ret - 1]) ;
			ret-- ;
		}
		mDeque[ret] = std::move (item) ;
		update_resize () ;
		return std::move (ret) ;
	}

	INDEX insert_sort (ITEM &&item) popping {
		_DEBUG_ASSERT_ (mRead == 0) ;
		reserve (1) ;
		INDEX ret = mWrite ;
		while (TRUE) {
			if (ret - 1 < 0)
				break ;
			if (mDeque[ret - 1] >= item)
				break ;
			mDeque[ret] = std::move (mDeque[ret - 1]) ;
			ret-- ;
		}
		mDeque[ret] = std::move (item) ;
		update_resize () ;
		return std::move (ret) ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite = (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		U::OPERATOR_SORT::invoke ((*this) ,ret ,0 ,ret.length ()) ;
		return std::move (ret) ;
	}

	void sort () {
		U::OPERATOR_SORT::invoke (_CAST_<AccessArray> ((*this)) ,0 ,length ()) ;
	}

private:
	explicit Deque (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mDeque (len) {}

private:
	BOOL ensure_index (INDEX index) const {
		const auto r1x = BOOL (mRead <= mWrite) ;
		const auto r2x = BOOL (index >= mRead && index < mWrite) ;
		if (r1x && !r2x)
			return FALSE ;
		const auto r3x = BOOL (index >= 0 && index < mWrite) ;
		const auto r4x = BOOL (index >= mRead && index < mDeque.size ()) ;
		if (!r1x && !r3x && !r4x)
			return FALSE ;
		return TRUE ;
	}

	void reserve (LENGTH len) {
		const auto r1x = _MAX_ (len - (mDeque.size () - length ()) ,VAR_ZERO) ;
		if (r1x == 0)
			return ;
		auto rax = mDeque.expand (mDeque.size () + r1x) ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(mRead <= mWrite))
				discard ;
			_MEMMOVE_ (PTRTOARR[&rax.self[mRead]] ,PTRTOARR[&mDeque.self[mRead]] ,(mWrite - mRead)) ;
		}
		if SWITCH_CASE (fax) {
			if (!(mRead > mWrite))
				discard ;
			_MEMMOVE_ (rax.self ,mDeque.self ,mWrite) ;
			INDEX ix = mRead + rax.size () - mDeque.size () ;
			_MEMMOVE_ (PTRTOARR[&rax.self[ix]] ,PTRTOARR[&mDeque.self[mRead]] ,(mDeque.size () - mRead)) ;
			mRead = ix ;
		}
		mDeque.swap (rax) ;
	}

	void update_resize () {
		_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mDeque.size ()) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		if (mRead != mWrite)
			return ;
		auto rax = mDeque.expand () ;
		_MEMMOVE_ (rax.self ,mDeque.self ,mWrite) ;
		INDEX ix = 0 ;
		INDEX iy = mDeque.size () ;
		if SWITCH_ONCE (TRUE) {
			if (mRead == 0)
				discard ;
			ix = mRead + rax.size () - mDeque.size () ;
			iy = mWrite ;
		}
		_MEMMOVE_ (PTRTOARR[&rax.self[ix]] ,PTRTOARR[&mDeque.self[mRead]] ,(mDeque.size () - mRead)) ;
		mDeque.swap (rax) ;
		mRead = ix ;
		mWrite = iy ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class Priority ;

template <class KEY ,class ITEM ,class SIZE>
class Priority<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Priority<KEY ,ITEM ,SIZE> ;

	class Node {
	private:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		ITEM mItem ;

	public:
		inline Node () = default ;
	} ;

	using PAIR_TYPE = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair final {
	public:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mPriority[index].mKey) ,item (base.mPriority[index].mItem) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

	inline static constexpr LENGTH constexpr_size (LENGTH len) {
		return _SWITCH_ (
			(len <= 0) ? len :
			(len + 1)) ;
	}

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,ARGC<constexpr_size (SIZE::value)>> mPriority ;
	INDEX mWrite ;
	INDEX mTop ;

public:
	Priority () {
		spec.clear () ;
	}

	explicit Priority (LENGTH len) :Priority (ARGVP0 ,constexpr_size (len)) {
		spec.clear () ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		spec.reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		mPriority[ix].mItem = std::move (item) ;
		spec.update_resize () ;
		spec.update_insert (ix) ;
	}

	void add (const PAIR_TYPE &item) {
		add (item.key ,std::move (item)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		spec.reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		mPriority[ix].mItem = std::move (item) ;
		spec.update_resize () ;
		spec.update_insert (ix) ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		spec.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		spec.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

private:
	explicit Priority (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mPriority (len) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class SIZE>
class Priority<KEY ,SPECIALIZATION<void> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Priority<KEY ,void ,SIZE> ;

	class Node {
	private:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;

	public:
		inline Node () = default ;
	} ;

	using PAIR_TYPE = PACK<KEY> ;

	template <class BASE>
	class Pair final {
	public:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mPriority[index].mKey) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

	inline static constexpr LENGTH constexpr_size (LENGTH len) {
		return _SWITCH_ (
			(len <= 0) ? len :
			(len + 1)) ;
	}

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,ARGC<constexpr_size (SIZE::value)>> mPriority ;
	INDEX mWrite ;
	INDEX mTop ;

public:
	Priority () {
		spec.clear () ;
	}

	explicit Priority (LENGTH len) :Priority (ARGVP0 ,constexpr_size (len)) {
		spec.clear () ;
	}

	void add (const KEY &key) {
		spec.reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		spec.update_resize () ;
		spec.update_insert (ix) ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		spec.reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		spec.update_resize () ;
		spec.update_insert (ix) ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		spec.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		spec.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

private:
	explicit Priority (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mPriority (len) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class ITEM ,class SIZE>
class Priority :private Priority<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Priority<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using PAIR_TYPE = typename SPECIALIZATION_BASE::PAIR_TYPE ;
	template <class _ARG1>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG1> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mPriority ;
	using SPECIALIZATION_BASE::mWrite ;
	using SPECIALIZATION_BASE::mTop ;

public:
	Priority () = default ;

	explicit Priority (LENGTH len) :SPECIALIZATION_BASE (len) {}

	implicit Priority (const std::initializer_list<PAIR_TYPE> &that) : Priority (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		if (mPriority.size () == 0)
			return 0 ;
		return mPriority.size () - 1 ;
	}

	LENGTH length () const {
		if (mPriority.size () == 0)
			return 0 ;
		return mWrite ;
	}

	void clear () {
		mWrite = 0 ;
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

	//@warn: index would be no longer valid every time revised
	Pair<Priority> get (INDEX index) & {
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		return Pair<Priority> ((*this) ,index) ;
	}

	inline Pair<Priority> operator[] (INDEX index) & {
		return get (index) ;
	}

	//@warn: index would be no longer valid every time revised
	Pair<const Priority> get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		return Pair<const Priority> ((*this) ,index) ;
	}

	inline Pair<const Priority> operator[] (INDEX index) const & {
		return get (index) ;
	}

	Pair<Priority> get (INDEX) && = delete ;

	inline Pair<Priority> operator[] (INDEX) && = delete ;

	INDEX at (const Pair<Priority> &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&Node::mKey ,item.key)) ;
		if (!(ret >= 0 && ret < mWrite))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	//@error: vs2015 is too useless to compile without hint
	INDEX at (const typename SPECIALIZATION_BASE::Pair_const_BASE &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&Node::mKey ,item.key)) ;
		if (!(ret >= 0 && ret < mWrite))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	BOOL empty () const {
		if (mWrite > 0)
			return FALSE ;
		return TRUE ;
	}

	BOOL full () const {
		if (mWrite < mPriority.size ())
			return FALSE ;
		return TRUE ;
	}

	using SPECIALIZATION_BASE::add ;

	inline Priority &operator<< (const PAIR_TYPE &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	inline Priority &operator<< (PAIR_TYPE &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	using SPECIALIZATION_BASE::appand ;

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mPriority[0] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (0) ;
	}

	void take (PAIR_TYPE &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mPriority[0]) ;
		mPriority[0] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (0) ;
	}

	inline Priority &operator>> (PAIR_TYPE &item) popping {
		take (item) ;
		return (*this) ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return 0 ;
	}

	INDEX insert (const KEY &key) popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		mPriority[ret].mKey = std::move (key) ;
		update_resize () ;
		update_insert (ret) ;
		ret = mTop ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		mPriority[ret].mKey = std::move (key) ;
		update_resize () ;
		update_insert (ret) ;
		ret = mTop ;
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		mPriority[index] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (index) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		INDEX ix = ret.length () ;
		while (TRUE) {
			if (ix - 1 < 1)
				break ;
			_SWAP_ (ret[0] ,ret[ix - 1]) ;
			compute_esort (ret ,ix - 1) ;
			ix-- ;
		}
		if (ret.size () > 0)
			_MEMRCOPY_ (ret.raw ().self ,ret.raw ().self ,ret.size ()) ;
		return std::move (ret) ;
	}

private:
	void reserve (LENGTH len) {
		const auto r1x = _MAX_ (len - (mPriority.size () - length ()) ,VAR_ZERO) ;
		if (r1x == 0)
			return ;
		auto rax = mPriority.expand (mPriority.size () + r1x) ;
		_MEMMOVE_ (rax.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.swap (rax) ;
	}

	void update_resize () {
		_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mPriority.size ()) ;
		mWrite++ ;
		if (mWrite < mPriority.size ())
			return ;
		auto rax = mPriority.expand () ;
		_MEMMOVE_ (rax.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.swap (rax) ;
	}

	void update_insert (INDEX curr) {
		INDEX ix = curr ;
		auto rax = std::move (mPriority[ix]) ;
		while (TRUE) {
			//@info: '-1 >> 1' is not the same as '-1 / 2'
			INDEX iy = (ix - 1) >> 1 ;
			if (iy < 0)
				break ;
			if (rax.mKey >= mPriority[iy].mKey)
				break ;
			mPriority[ix] = std::move (mPriority[iy]) ;
			ix = iy ;
		}
		while (TRUE) {
			INDEX iy = ix * 2 + 1 ;
			if (iy >= mWrite)
				break ;
			INDEX jx = ix ;
			if (rax.mKey > mPriority[iy].mKey)
				jx = iy ;
			iy++ ;
			auto &r1y = _SWITCH_ (
				(jx != ix) ? (mPriority[jx].mKey) :
				(rax.mKey)) ;
			if SWITCH_ONCE (TRUE) {
				if (iy >= mWrite)
					discard ;
				if (r1y <= mPriority[iy].mKey)
					discard ;
				jx = iy ;
			}
			if (jx == ix)
				break ;
			mPriority[ix] = std::move (mPriority[jx]) ;
			ix = jx ;
		}
		mPriority[ix] = std::move (rax) ;
		mTop = ix ;
	}

	void compute_esort (Array<INDEX> &out ,LENGTH len) const {
		INDEX ix = 0 ;
		const auto r1x = out[ix] ;
		while (TRUE) {
			INDEX iy = ix * 2 + 1 ;
			if (iy >= len)
				break ;
			INDEX jx = ix ;
			if (mPriority[r1x].mKey > mPriority[out[iy]].mKey)
				jx = iy ;
			iy++ ;
			auto &r2y = _SWITCH_ (
				(jx != ix) ? (mPriority[out[jx]].mKey) :
				(mPriority[r1x].mKey)) ;
			if SWITCH_ONCE (TRUE) {
				if (iy >= len)
					discard ;
				if (r2y <= mPriority[out[iy]].mKey)
					discard ;
				jx = iy ;
			}
			if (jx == ix)
				break ;
			out[ix] = out[jx] ;
			ix = jx ;
		}
		out[ix] = r1x ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class QList ;

template <class ITEM ,class SIZE>
class QList {
private:
	class Node {
	private:
		friend QList ;
		ITEM mItem ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		inline Node () = delete ;

		inline explicit Node (INDEX left ,INDEX right) :mLeft (left) ,mRight (right) {}

		inline explicit Node (const ITEM &item ,INDEX left ,INDEX right) : mItem (std::move (item)) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (ITEM &&item ,INDEX left ,INDEX right) : mItem (std::move (item)) ,mLeft (left) ,mRight (right) {}
	} ;

private:
	Allocator<Node ,SIZE> mList ;
	INDEX mFirst ;
	INDEX mLast ;

public:
	QList () {
		clear () ;
	}

	explicit QList (LENGTH len) :QList (ARGVP0 ,len) {
		clear () ;
	}

	implicit QList (const std::initializer_list<ITEM> &that) : QList (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		return mList.size () ;
	}

	LENGTH length () const {
		return mList.length () ;
	}

	void clear () {
		mList.clear () ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
	}

	INDEX ibegin () const {
		return mFirst ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return mList[index].mRight ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	ITEM &get (INDEX index) & {
		return mList[index].mItem ;
	}

	inline ITEM &operator[] (INDEX index) & {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const & {
		return mList[index].mItem ;
	}

	inline const ITEM &operator[] (INDEX index) const & {
		return get (index) ;
	}

	ITEM &get (INDEX) && = delete ;

	inline ITEM &operator[] (INDEX) && = delete ;

	INDEX at (const ITEM &item) const {
		return mList.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	BOOL equal (const QList &that) const {
		if (length () != that.length ())
			return FALSE ;
		INDEX ib = ibegin () ;
		INDEX ie = iend () ;
		INDEX jb = that.ibegin () ;
		INDEX je = that.iend () ;
		while (TRUE) {
			if (ib == ie)
				break ;
			if (jb == je)
				break ;
			if (get (ib) != that.get (jb))
				break ;
			ib = inext (ib) ;
			jb = that.inext (jb) ;
		}
		if (ib != ie)
			return FALSE ;
		if (jb != je)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const QList &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const QList &that) const {
		return !equal (that) ;
	}

	BOOL empty () const {
		if (mList.length () > 0)
			return FALSE ;
		return TRUE ;
	}

	BOOL full () const {
		if (mList.length () < mList.size ())
			return FALSE ;
		return TRUE ;
	}

	void add (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,mLast ,VAR_NONE) ;
		auto &r1y = _SWITCH_ (
			(mLast != VAR_NONE) ? (mList[mLast].mRight) :
			mFirst) ;
		r1y = ix ;
		mLast = ix ;
	}

	inline QList &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	void add (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,mLast ,VAR_NONE) ;
		auto &r1y = _SWITCH_ (
			(mLast != VAR_NONE) ? (mList[mLast].mRight) :
			mFirst) ;
		r1y = ix ;
		mLast = ix ;
	}

	inline QList &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mList.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		mList.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		mFirst = mList[ix].mRight ;
		auto &r1y = _SWITCH_ (
			(mFirst != VAR_NONE) ? (mList[mFirst].mLeft) :
			mLast) ;
		r1y = VAR_NONE ;
		mList.free (ix) ;
	}

	void take (ITEM &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		item = std::move (mList[ix].mItem) ;
		mFirst = mList[ix].mRight ;
		auto &r1y = _SWITCH_ (
			(mFirst != VAR_NONE) ? (mList[mFirst].mLeft) :
			mLast) ;
		r1y = VAR_NONE ;
		mList.free (ix) ;
	}

	inline QList &operator>> (ITEM &item) popping {
		take (item) ;
		return (*this) ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mFirst ;
	}

	INDEX tail () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mLast ;
	}

	INDEX insert () popping {
		INDEX ret = mList.alloc (mLast ,VAR_NONE) ;
		auto &r1y = _SWITCH_ (
			(mLast != VAR_NONE) ? (mList[mLast].mRight) :
			mFirst) ;
		r1y = ret ;
		mLast = ret ;
		return std::move (ret) ;
	}

	INDEX insert_before (INDEX index) popping {
		INDEX ret = mList.alloc (VAR_NONE ,index) ;
		auto &r1y = _SWITCH_ (
			(index != VAR_NONE) ? (mList[index].mLeft) :
			mLast) ;
		mList[ret].mLeft = r1y ;
		auto &r2y = _SWITCH_ (
			(r1y != VAR_NONE) ? (mList[r1y].mRight) :
			mFirst) ;
		r2y = ret ;
		r1y = ret ;
		return std::move (ret) ;
	}

	INDEX insert_after (INDEX index) popping {
		const auto r1x = _SWITCH_ (
			(index != VAR_NONE) ? (mList[index].mRight) :
			mFirst) ;
		return insert_before (r1x) ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mLast ;
		mLast = mList[ix].mLeft ;
		auto &r1y = _SWITCH_ (
			(mLast != VAR_NONE) ? (mList[mLast].mRight) :
			mFirst) ;
		r1y = VAR_NONE ;
		mList.free (ix) ;
	}

	void eswap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1y = prev_next (index1) ;
		auto &r2y = prev_next (index2) ;
		auto &r3y = next_prev (index1) ;
		auto &r4y = next_prev (index2) ;
		r1y = index2 ;
		r2y = index1 ;
		r3y = index2 ;
		r4y = index1 ;
		_SWAP_ (mList[index1].mLeft ,mList[index2].mLeft) ;
		_SWAP_ (mList[index1].mRight ,mList[index2].mRight) ;
	}

	void splice_before (INDEX index ,INDEX seg) {
		prev_next (seg) = mList[seg].mRight ;
		next_prev (seg) = mList[seg].mLeft ;
		auto &r1y = _SWITCH_ (
			(index != VAR_NONE) ? (mList[index].mLeft) :
			mLast) ;
		mList[seg].mLeft = r1y ;
		auto &r2y = _SWITCH_ (
			(r1y != VAR_NONE) ? (mList[r1y].mRight) :
			mFirst) ;
		r2y = seg ;
		r1y = seg ;
	}

	void splice_after (INDEX index ,INDEX seg) {
		const auto r1x = _SWITCH_ (
			(index != VAR_NONE) ? (mList[index].mRight) :
			mFirst) ;
		splice_before (r1x ,seg) ;
	}

	void remove (INDEX index) {
		prev_next (index) = mList[index].mRight ;
		next_prev (index) = mList[index].mLeft ;
		mList.free (index) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = range () ;
		U::OPERATOR_SORT::invoke ((*this) ,ret ,0 ,ret.length ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		_DEBUG_ASSERT_ (r1x.length () >= 2) ;
		for (INDEX i = 0 ,ie = 1 ; i < ie ; i++) {
			mList[r1x[i]].mLeft = VAR_NONE ;
			mList[r1x[i]].mRight = r1x[i + 1] ;
			mFirst = r1x[i] ;
		}
		for (INDEX i = 1 ,ie = r1x.length () - 1 ; i < ie ; i++) {
			mList[r1x[i]].mLeft = r1x[i - 1] ;
			mList[r1x[i]].mRight = r1x[i + 1] ;
		}
		for (INDEX i = r1x.length () - 1 ,ie = r1x.length () ; i < ie ; i++) {
			mList[r1x[i]].mLeft = r1x[i - 1] ;
			mList[r1x[i]].mRight = VAR_NONE ;
			mLast = r1x[i] ;
		}
	}

private:
	explicit QList (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mList (len) {}

private:
	INDEX &prev_next (INDEX curr) & {
		if (mList[curr].mLeft == VAR_NONE)
			return mFirst ;
		return mList[mList[curr].mLeft].mRight ;
	}

	INDEX &prev_next (INDEX) && = delete ;

	INDEX &next_prev (INDEX curr) & {
		if (mList[curr].mRight == VAR_NONE)
			return mLast ;
		return mList[mList[curr].mRight].mLeft ;
	}

	INDEX &next_prev (INDEX) && = delete ;
} ;

template <class ITEM ,class SIZE = SAUTO>
class SList ;

template <class ITEM ,class SIZE>
class SList {
private:
	class Node {
	private:
		friend SList ;
		ITEM mItem ;
		INDEX mSeq ;

	public:
		inline Node () = delete ;

		inline explicit Node (INDEX seq) :mSeq (seq) {}

		inline explicit Node (const ITEM &item ,INDEX seq) : mItem (std::move (item)) ,mSeq (seq) {}

		inline explicit Node (ITEM &&item ,INDEX seq) : mItem (std::move (item)) ,mSeq (seq) {}
	} ;

private:
	Allocator<Node ,SIZE> mList ;
	Buffer<Buffer<INDEX ,ARGC<2>> ,SIZE> mHead ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	SList () {
		clear () ;
	}

	explicit SList (LENGTH len) :SList (ARGVP0 ,len) {
		clear () ;
	}

	implicit SList (const std::initializer_list<ITEM> &that) : SList (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		return mList.size () ;
	}

	LENGTH length () const {
		return mList.length () ;
	}

	void clear () {
		const auto r1x = Buffer<INDEX ,ARGC<2>> ({
			VAR_NONE ,
			VAR_ZERO}) ;
		_MEMFILL_ (mHead.self ,mHead.size () ,r1x) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	INDEX ibegin () const {
		if (mHead.size () == 0)
			return VAR_NONE ;
		for (INDEX i = mRead ,ie = mWrite + 1 ; i < ie ; i++)
			if (mHead[i][0] != VAR_NONE)
				return mHead[i][0] ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = mList[index].mSeq + 1 ,ie = mWrite + 1 ; i < ie ; i++)
			if (mHead[i][0] != VAR_NONE)
				return mHead[i][0] ;
		return VAR_NONE ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	ITEM &get (INDEX index) & {
		return mList[index].mItem ;
	}

	inline ITEM &operator[] (INDEX index) & {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const & {
		return mList[index].mItem ;
	}

	inline const ITEM &operator[] (INDEX index) const & {
		return get (index) ;
	}

	ITEM &get (INDEX) && = delete ;

	inline ITEM &operator[] (INDEX) && = delete ;

	INDEX at (const ITEM &item) const {
		return mList.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	INDEX access (INDEX pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		if (mWrite - mRead + 1 == mList.length ())
			return mHead[mRead + pos][0] ;
		if (mWrite - mRead == mList.length () && mHead[mWrite][0] == VAR_NONE)
			return mHead[mRead + pos][0] ;
		return access (pos ,mRead ,(mWrite - mRead + 1)) ;
	}

	BOOL equal (const SList &that) const {
		if (length () != that.length ())
			return FALSE ;
		INDEX ib = ibegin () ;
		INDEX ie = iend () ;
		INDEX jb = that.ibegin () ;
		INDEX je = that.iend () ;
		while (TRUE) {
			if (ib == ie)
				break ;
			if (jb == je)
				break ;
			if (get (ib) != that.get (jb))
				break ;
			ib = inext (ib) ;
			jb = that.inext (jb) ;
		}
		if (ib != ie)
			return FALSE ;
		if (jb != je)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const SList &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const SList &that) const {
		return !equal (that) ;
	}

	void add (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ix) ;
		update_compress_left (mWrite ,ix) ;
	}

	inline SList &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	void add (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ix) ;
		update_compress_left (mWrite ,ix) ;
	}

	inline SList &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mList.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		mList.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	INDEX insert () popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		update_resize (ret) ;
		update_compress_left (mWrite ,ret) ;
		return std::move (ret) ;
	}

	INDEX insert_before (INDEX index) popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		update_resize (ret) ;
		const auto r1x = _SWITCH_ (
			(index != VAR_NONE) ? (mList[index].mSeq) :
			mWrite) ;
		update_compress_left (r1x ,ret) ;
		return std::move (ret) ;
	}

	INDEX insert_after (INDEX index) popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		update_resize (ret) ;
		const auto r1x = _SWITCH_ (
			(index != VAR_NONE) ? (mList[index].mSeq + 1) :
			mRead) ;
		update_compress_left (r1x ,ret) ;
		return std::move (ret) ;
	}

	void eswap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		sequence_rewrite (mList[index1].mSeq ,index2) ;
		sequence_rewrite (mList[index2].mSeq ,index1) ;
	}

	void splice_before (INDEX index ,INDEX seg) {
		sequence_remove (mList[seg].mSeq) ;
		const auto r1x = insert_before (index ,seg) ;
		(void) r1x ;
	}

	void splice_after (INDEX index ,INDEX seg) {
		sequence_remove (mList[seg].mSeq) ;
		const auto r1x = insert_after (index ,seg) ;
		(void) r1x ;
	}

	void remove (INDEX index) {
		sequence_remove (mList[index].mSeq) ;
		mList.free (index) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = range () ;
		U::OPERATOR_SORT::invoke ((*this) ,ret ,0 ,ret.length ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		for (INDEX i = 0 ,ie = r1x.length () ; i < ie ; i++)
			sequence_rewrite (i ,r1x[i]) ;
		for (INDEX i = r1x.length () ,ie = mHead.size () ; i < ie ; i++)
			sequence_remove (i) ;
		mRead = 0 ;
		mWrite = _MAX_ ((r1x.length () - 1) ,VAR_ZERO) ;
	}

	void reverse () {
		if (mHead.size () == 0)
			return ;
		INDEX ix = mRead ;
		INDEX iy = mWrite ;
		while (TRUE) {
			while (ix < iy && mHead[ix] != VAR_NONE)
				ix++ ;
			if (ix >= iy)
				break ;
			while (ix < iy && mHead[iy] != VAR_NONE)
				iy-- ;
			if (ix >= iy)
				break ;
			const auto r1x = mHead[ix] ;
			const auto r2x = mHead[iy] ;
			sequence_rewrite (ix ,r2x) ;
			sequence_rewrite (iy ,r1x) ;
		}
	}

private:
	explicit SList (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mList (len) ,mHead (len) {}

private:
	INDEX access (INDEX pos ,INDEX seg ,LENGTH seg_len) const {
		_DEBUG_ASSERT_ (seg_len > 0) ;
		_DEBUG_ASSERT_ (seg >= 0 && seg <= mHead.size () - seg_len) ;
		INDEX ret = VAR_NONE ;
		INDEX ix = seg ;
		INDEX iy = seg + seg_len - 1 ;
		while (TRUE) {
			if (ix > iy)
				break ;
			ret = ix + (iy - ix) / 2 ;
			INDEX jx = position_before (ret) ;
			if (jx == pos && mHead[ret][0] != VAR_NONE)
				break ;
			auto &r1y = _SWITCH_ (
				(jx < pos) ? ix :
				iy) ;
			const auto r2x = _SWITCH_ (
				(jx < pos) ? (ret + 1) :
				(ret - 1)) ;
			r1y = r2x ;
		}
		_DEBUG_ASSERT_ (ret != VAR_NONE) ;
		ret = mHead[ret][0] ;
		return std::move (ret) ;
	}

	INDEX position_before (INDEX curr) const {
		INDEX ret = 0 ;
		INDEX ix = curr ;
		while (TRUE) {
			if (ix < 0)
				break ;
			ret += mHead[ix][1] ;
			ix -= (ix + 1) & -(ix + 1) ;
		}
		ret-- ;
		return std::move (ret) ;
	}

	void update_resize (INDEX curr) {
		if (mHead.size () == mList.size ())
			return ;
		auto rax = mHead.expand (mList.size ()) ;
		const auto r1x = Buffer<INDEX ,ARGC<2>> ({
			VAR_NONE ,
			VAR_ZERO}) ;
		_MEMFILL_ (rax.self ,rax.size () ,r1x) ;
		for (INDEX i = 0 ,ie = mList.size () ; i < ie ; i++) {
			if (i == curr)
				continue ;
			if (!mList.used (i))
				continue ;
			sequence_rewrite (mList[i].mSeq ,i) ;
		}
		mHead.swap (rax) ;
	}

	void update_compress_left (INDEX curr ,INDEX last) {
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(mHead[curr][0] == VAR_NONE))
				discard ;
			sequence_rewrite (curr ,last) ;
			mWrite = _MIN_ ((curr + 1) ,(mHead.size () - 1)) ;
		}
		if SWITCH_CASE (fax) {
			INDEX ix = curr + 1 ;
			if (!(ix < mHead.size ()))
				discard ;
			if (!(mHead[ix][0] == VAR_NONE))
				discard ;
			sequence_rewrite (ix ,last) ;
			mWrite = _MIN_ ((ix + 1) ,(mHead.size () - 1)) ;
		}
		if SWITCH_CASE (fax) {
			update_compress_left_force (curr ,last) ;
		}
	}

	void update_compress_left_force (INDEX curr ,INDEX last) {
		INDEX ix = curr ;
		INDEX iy = last ;
		for (INDEX i = 0 ,ie = mList.length () ; i < ie ; i++) {
			while (mRead != ix && mHead[mRead][0] == VAR_NONE)
				mRead++ ;
			const auto r1x = mHead[i][0] ;
			auto fax = FALSE ;
			if SWITCH_CASE (fax) {
				if (!(mRead == ix))
					discard ;
				if (!(r1x == VAR_NONE))
					discard ;
				sequence_rewrite (i ,iy) ;
				iy = r1x ;
				ix = VAR_NONE ;
			}
			if SWITCH_CASE (fax) {
				if (!(mRead == ix))
					discard ;
				if (!(r1x != VAR_NONE))
					discard ;
				sequence_rewrite (i ,iy) ;
				iy = r1x ;
				ix++ ;
			}
			if SWITCH_CASE (fax) {
				if (!(mRead != i))
					discard ;
				sequence_rewrite (i ,mHead[mRead][0]) ;
				sequence_remove (mRead) ;
			}
			mRead++ ;
		}
		mRead = 0 ;
		mWrite = _MIN_ (mList.length () ,(mHead.size () - 1)) ;
	}

	void sequence_rewrite (INDEX curr ,INDEX index) {
		_DEBUG_ASSERT_ (index != VAR_NONE) ;
		INDEX ix = curr ;
		const auto r1x = mHead[curr][0] ;
		mHead[ix][0] = index ;
		mList[index].mSeq = ix ;
		if (r1x != VAR_NONE)
			return ;
		while (TRUE) {
			if (ix >= mHead.size ())
				break ;
			mHead[ix][1]++ ;
			ix += (ix + 1) & -(ix + 1) ;
		}
	}

	void sequence_remove (INDEX curr) {
		INDEX ix = curr ;
		mHead[ix][0] = VAR_NONE ;
		while (TRUE) {
			if (ix >= mHead.size ())
				break ;
			mHead[ix][1]-- ;
			ix += (ix + 1) & -(ix + 1) ;
		}
	}
} ;

template <class SIZE = SAUTO>
class BitSet ;

template <class SIZE>
class BitSet {
private:
	inline static constexpr LENGTH constexpr_size (LENGTH len) {
		return _SWITCH_ (
			(len <= 0) ? len :
			((len + 7) / 8)) ;
	}

	template <class BASE>
	class Bit final {
	private:
		friend BitSet ;
		BASE &mBase ;
		INDEX mIndex ;

	public:
		inline Bit () = delete ;

		inline Bit (const Bit &) = delete ;

		inline Bit (Bit &&) noexcept = default ;

		inline explicit operator BOOL () const & = delete ;

		inline implicit operator BOOL () && {
			const auto r1x = mBase.mSet[mIndex / 8] & (BYTE (0X01) << (mIndex % 8)) ;
			if (r1x == 0)
				return FALSE ;
			return TRUE ;
		}

#ifdef __CSC_CONFIG_VAR32__
		inline implicit operator VAR32 () const & {
			return VAR32 (mIndex) ;
		}

		inline explicit operator VAR32 () && = delete ;

		inline explicit operator VAR64 () const & {
			return VAR64 (mIndex) ;
		}

		inline explicit operator VAR64 () && = delete ;
#elif defined __CSC_CONFIG_VAR64__
		inline explicit operator VAR32 () const & {
			return VAR32 (mIndex) ;
		}

		inline explicit operator VAR32 () && = delete ;

		inline implicit operator VAR64 () const & {
			return VAR64 (mIndex) ;
		}

		inline explicit operator VAR64 () && = delete ;
#endif

		inline void operator= (const BOOL &that) && {
			const auto r1x = mBase.mSet[mIndex / 8] ;
			const auto r2x = _SWITCH_ (
				that ? (~r1x) :
				r1x) ;
			const auto r3x = BYTE (r2x & (BYTE (0X01) << (mIndex % 8))) ;
			mBase.mSet[mIndex / 8] = BYTE (r1x ^ r3x) ;
		}

		inline void operator= (Bit<BitSet> &&that) && {
			const auto r1x = BOOL (std::move (that)) ;
			std::move ((*this)) = r1x ;
		}

		inline void operator= (Bit<const BitSet> &&that) && {
			const auto r1x = BOOL (std::move (that)) ;
			std::move ((*this)) = r1x ;
		}

	private:
		inline explicit Bit (BASE &base ,INDEX index) popping : mBase (base) ,mIndex (index) {}
	} ;

private:
	class Detail ;
	Buffer<BYTE ,ARGC<constexpr_size (SIZE::value)>> mSet ;
	LENGTH mWidth ;

public:
	BitSet () :BitSet (ARGVP0) {
		clear () ;
	}

	explicit BitSet (LENGTH len) :BitSet (ARGVP0 ,constexpr_size (len) ,Detail::forward_width (len)) {
		clear () ;
	}

	implicit BitSet (const std::initializer_list<INDEX> &that) : BitSet (Detail::forward_size (that)) {
		for (auto &&i : that)
			get (i) = TRUE ;
	}

	LENGTH size () const {
		if (mSet.size () == 0)
			return 0 ;
		return mWidth ;
	}

	LENGTH length () const {
		static constexpr auto M_LENGTH = PACK<VAR32[256]> ({
			0 ,1 ,1 ,2 ,1 ,2 ,2 ,3 ,1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			1 ,2 ,2 ,3 ,2 ,3 ,3 ,4 ,2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			2 ,3 ,3 ,4 ,3 ,4 ,4 ,5 ,3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			3 ,4 ,4 ,5 ,4 ,5 ,5 ,6 ,4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,
			4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8}) ;
		LENGTH ret = 0 ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			ret += M_LENGTH.P1[mSet[i]] ;
		if (mWidth % 8 != 0)
			ret -= M_LENGTH.P1[mSet[mWidth / 8] & ~((VAR (1) << (mWidth % 8)) - 1)] ;
		return std::move (ret) ;
	}

	void clear () {
		fill (BYTE (0X00)) ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ,ie = size () ; i < ie ; i++)
			if (get (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = index + 1 ,ie = size () ; i < ie ; i++)
			if (get (i))
				return i ;
		return VAR_NONE ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	Bit<BitSet> get (INDEX index) & {
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit<BitSet> ((*this) ,index) ;
	}

	inline Bit<BitSet> operator[] (INDEX index) & {
		return get (index) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	Bit<const BitSet> get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit<const BitSet> ((*this) ,index) ;
	}

	inline Bit<const BitSet> operator[] (INDEX index) const & {
		return get (index) ;
	}

	Bit<BitSet> get (INDEX) && = delete ;

	inline Bit<BitSet> operator[] (INDEX) && = delete ;

	INDEX at (const Bit<BitSet> &item) const {
		if (this != &item.mBase)
			return VAR_NONE ;
		return INDEX (item) ;
	}

	INDEX at (const Bit<const BitSet> &item) const {
		if (this == &item.mBase)
			return VAR_NONE ;
		return INDEX (item) ;
	}

	BOOL equal (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		if (ix < 0)
			return TRUE ;
		for (INDEX i = 0 ,ie = ix ; i < ie ; i++)
			if (mSet[i] != that.mSet[i])
				return FALSE ;
		const auto r1x = mSet[ix] & (mWidth % 8 - 1) ;
		const auto r2x = that.mSet[ix] & (mWidth % 8 - 1) ;
		if (r1x != r2x)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const BitSet &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const BitSet &that) const {
		return !equal (that) ;
	}

	FLAG compr (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		if (ix < 0)
			return 0 ;
		const auto r1x = _MEMCOMPR_ (mSet ,that.mSet ,ix) ;
		if (r1x != 0)
			return r1x ;
		const auto r2x = mSet[ix] & (mWidth % 8 - 1) ;
		const auto r3x = that.mSet[ix] & (mWidth % 8 - 1) ;
		return _MEMCOMPR_ (PTRTOARR[&r2x] ,PTRTOARR[&r3x] ,1) ;
	}

	inline BOOL operator< (const BitSet &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const BitSet &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const BitSet &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const BitSet &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	void add (const INDEX &item) {
		_DYNAMIC_ASSERT_ (item >= 0 && item < mWidth) ;
		get (item) = TRUE ;
	}

	inline BitSet &operator<< (const INDEX &item) {
		add (item) ;
		return (*this) ;
	}

	void erase (const INDEX &item) {
		_DYNAMIC_ASSERT_ (item >= 0 && item < mWidth) ;
		get (item) = FALSE ;
	}

	BitSet band (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			ret.mSet[i] = mSet[i] & that.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator& (const BitSet &that) const {
		return band (that) ;
	}

	void bandto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			mSet[i] &= that.mSet[i] ;
	}

	inline BitSet &operator&= (const BitSet &that) {
		bandto (that) ;
		return (*this) ;
	}

	BitSet bor (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			ret.mSet[i] = mSet[i] | that.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator| (const BitSet &that) const {
		return bor (that) ;
	}

	void borto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			mSet[i] |= that.mSet[i] ;
	}

	inline BitSet &operator|= (const BitSet &that) {
		borto (that) ;
		return (*this) ;
	}

	BitSet bxor (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			ret.mSet[i] = mSet[i] ^ that.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator^ (const BitSet &that) const {
		return bxor (that) ;
	}

	void bxorto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			mSet[i] ^= that.mSet[i] ;
	}

	inline BitSet &operator^= (const BitSet &that) {
		bxorto (that) ;
		return (*this) ;
	}

	BitSet bsub (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			ret.mSet[i] = mSet[i] & ~that.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator- (const BitSet &that) const {
		return bsub (that) ;
	}

	void bsubto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			mSet[i] &= ~that.mSet[i] ;
	}

	inline BitSet &operator-= (const BitSet &that) {
		bsubto (that) ;
		return (*this) ;
	}

	BitSet bnot () const {
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			ret.mSet[i] = ~mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator~ () const {
		return bnot () ;
	}

	void fill (const BYTE &val) {
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++)
			mSet[i] = val ;
	}

private:
	explicit BitSet (const DEF<decltype (ARGVP0)> &) :mWidth (0) {}

	explicit BitSet (const DEF<decltype (ARGVP0)> & ,LENGTH len ,LENGTH width) :mSet (len) ,mWidth (width) {}

private:
	class Detail :private Wrapped<void> {
	public:
		inline static LENGTH forward_size (const std::initializer_list<INDEX> &that) {
			LENGTH ret = VAR_NONE ;
			for (auto &&i : that)
				ret = _MAX_ (ret ,i) ;
			_DEBUG_ASSERT_ (ret >= 0) ;
			ret++ ;
			return std::move (ret) ;
		}

		inline static LENGTH forward_width (LENGTH width) {
			_DEBUG_ASSERT_ (width >= 0 && width < VAR32_MAX) ;
			return width ;
		}
	} ;
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class Set ;

template <class KEY ,class ITEM ,class SIZE>
class Set<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Set<KEY ,ITEM ,SIZE> ;

	class Node {
	private:
		friend Set ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		ITEM mItem ;
		BOOL mRed ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,BOOL red ,INDEX up ,INDEX left ,INDEX right) :mKey (std::move (key)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (KEY &&key ,BOOL red ,INDEX up ,INDEX left ,INDEX right) :mKey (std::move (key)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (const KEY &key ,ITEM &&item ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mItem (std::move (item)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mItem (std::move (item)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}
	} ;

	using PAIR_TYPE = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair final {
	public:
		friend Set ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mSet[index].mKey) ,item (base.mSet[index].mItem) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	Set () {
		spec.clear () ;
	}

	explicit Set (LENGTH len) :Set (ARGVP0 ,len) {
		spec.clear () ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			spec.update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			spec.update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

private:
	explicit Set (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mSet (len) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class SIZE>
class Set<KEY ,SPECIALIZATION<void> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Set<KEY ,void ,SIZE> ;

	class Node {
	private:
		friend Set ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		BOOL mRed ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,BOOL red ,INDEX up ,INDEX left ,INDEX right) :mKey (std::move (key)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (KEY &&key ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}
	} ;

	using PAIR_TYPE = PACK<KEY> ;

	template <class BASE>
	class Pair final {
	public:
		friend Set ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mSet[index].mKey) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	Set () {
		spec.clear () ;
	}

	explicit Set (LENGTH len) :Set (ARGVP0 ,len) {
		spec.clear () ;
	}

	void add (const KEY &key) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			spec.update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			spec.update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

private:
	explicit Set (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mSet (len) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class ITEM ,class SIZE>
class Set :private Set<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Set<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using PAIR_TYPE = typename SPECIALIZATION_BASE::PAIR_TYPE ;
	template <class _ARG1>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG1> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mSet ;
	using SPECIALIZATION_BASE::mRoot ;
	using SPECIALIZATION_BASE::mTop ;

public:
	Set () = default ;

	explicit Set (LENGTH len) :SPECIALIZATION_BASE (len) {}

	implicit Set (const std::initializer_list<PAIR_TYPE> &that) : Set (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		return mSet.size () ;
	}

	LENGTH length () const {
		return mSet.length () ;
	}

	void clear () {
		mSet.clear () ;
		mRoot = VAR_NONE ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ,ie = size () ; i < ie ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = index + 1 ,ie = size () ; i < ie ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	Pair<Set> get (INDEX index) & {
		return Pair<Set> ((*this) ,index) ;
	}

	inline Pair<Set> operator[] (INDEX index) & {
		return get (index) ;
	}

	Pair<const Set> get (INDEX index) const & {
		return Pair<const Set> ((*this) ,index) ;
	}

	inline Pair<const Set> operator[] (INDEX index) const & {
		return get (index) ;
	}

	Pair<Set> get (INDEX) && = delete ;

	inline Pair<Set> operator[] (INDEX) && = delete ;

	INDEX at (const Pair<Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	//@error: vs2015 is too useless to compile without hint
	INDEX at (const typename SPECIALIZATION_BASE::Pair_const_BASE &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	using SPECIALIZATION_BASE::add ;

	inline Set &operator<< (const PAIR_TYPE &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	inline Set &operator<< (PAIR_TYPE &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	using SPECIALIZATION_BASE::appand ;

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ret != VAR_NONE)
				discard ;
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			update_emplace (mRoot ,ret) ;
			mRoot = mTop ;
			update_insert (ret) ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ret != VAR_NONE)
				discard ;
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			update_emplace (mRoot ,ret) ;
			mRoot = mTop ;
			update_insert (ret) ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		if SWITCH_ONCE (TRUE) {
			if (mSet[index].mLeft == VAR_NONE)
				discard ;
			if (mSet[index].mRight == VAR_NONE)
				discard ;
			eswap (index ,successor_one (index)) ;
		}
		INDEX ix = mSet[index].mLeft ;
		if (ix == VAR_NONE)
			ix = mSet[index].mRight ;
		prev_next (index) = ix ;
		if (ix != VAR_NONE)
			mSet[ix].mUp = mSet[index].mUp ;
		if (!mSet[index].mRed)
			update_remove (ix ,mSet[index].mUp) ;
		mSet.free (index) ;
	}

	INDEX min_one () const {
		for (INDEX i = mRoot ; i != VAR_NONE ; i = mSet[i].mLeft)
			if (mSet[i].mLeft == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	INDEX max_one () const {
		for (INDEX i = mRoot ; i != VAR_NONE ; i = mSet[i].mRight)
			if (mSet[i].mRight == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = BOOL (key < mSet[ret].mKey) ;
			if (!r1x && !(key > mSet[ret].mKey))
				break ;
			const auto r2x = _SWITCH_ (
				r1x ? (mSet[ret].mLeft) :
				(mSet[ret].mRight)) ;
			ret = r2x ;
		}
		return std::move (ret) ;
	}

	void erase (const KEY &key) {
		INDEX ix = find (key) ;
		if (ix == VAR_NONE)
			return ;
		remove (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		compute_esort (mRoot ,ret ,iw) ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

private:
	void update_emplace (INDEX curr ,INDEX last) {
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(curr == VAR_NONE))
				discard ;
			mTop = last ;
		}
		if SWITCH_CASE (fax) {
			if (!(curr != VAR_NONE))
				discard ;
			mSet[last].mUp = curr ;
			auto &r1y = _SWITCH_ (
				(mSet[last].mKey < mSet[curr].mKey) ? (mSet[curr].mLeft) :
				(mSet[curr].mRight)) ;
			update_emplace (r1y ,last) ;
			r1y = mTop ;
			mTop = curr ;
		}
	}

	void update_insert (INDEX curr) {
		INDEX ix = curr ;
		while (TRUE) {
			INDEX jx = mSet[ix].mUp ;
			if (jx == VAR_NONE)
				break ;
			if (!mSet[jx].mRed)
				break ;
			auto fax = FALSE ;
			if SWITCH_CASE (fax) {
				if (!(jx == mSet[mSet[jx].mUp].mLeft))
					discard ;
				update_insert_left (ix) ;
			}
			if SWITCH_CASE (fax) {
				if (!(jx == mSet[mSet[jx].mUp].mRight))
					discard ;
				update_insert_right (ix) ;
			}
			ix = mTop ;
		}
		mSet[mRoot].mRed = FALSE ;
	}

	void update_insert_left (INDEX curr) {
		INDEX ix = mSet[curr].mUp ;
		INDEX iy = mSet[ix].mUp ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(mSet[iy].mRight != VAR_NONE))
				discard ;
			if (!(mSet[mSet[iy].mRight].mRed))
				discard ;
			mSet[mSet[iy].mRight].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			mTop = iy ;
		}
		if SWITCH_CASE (fax) {
			if (!(curr == mSet[ix].mRight))
				discard ;
			auto &r1y = mSet[iy].mLeft ;
			rotate_left (r1y) ;
			r1y = mTop ;
			mSet[curr].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2y = prev_next (iy) ;
			rotate_right (r2y) ;
			r2y = mTop ;
			mTop = ix ;
		}
		if SWITCH_CASE (fax) {
			if (!(curr == mSet[ix].mLeft))
				discard ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r3y = prev_next (iy) ;
			rotate_right (r3y) ;
			r3y = mTop ;
			mTop = curr ;
		}
	}

	void update_insert_right (INDEX curr) {
		INDEX ix = mSet[curr].mUp ;
		INDEX iy = mSet[ix].mUp ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(mSet[iy].mLeft != VAR_NONE))
				discard ;
			if (!(mSet[mSet[iy].mLeft].mRed))
				discard ;
			mSet[mSet[iy].mLeft].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			mTop = iy ;
		}
		if SWITCH_CASE (fax) {
			if (!(curr == mSet[ix].mLeft))
				discard ;
			auto &r1y = mSet[iy].mRight ;
			rotate_right (r1y) ;
			r1y = mTop ;
			mSet[curr].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2y = prev_next (iy) ;
			rotate_left (r2y) ;
			r2y = mTop ;
			mTop = ix ;
		}
		if SWITCH_CASE (fax) {
			if (!(curr == mSet[ix].mRight))
				discard ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r3y = prev_next (iy) ;
			rotate_left (r3y) ;
			r3y = mTop ;
			mTop = curr ;
		}
	}

	void update_remove (INDEX curr ,INDEX last) {
		INDEX ix = curr ;
		INDEX iy = last ;
		while (TRUE) {
			if (iy == VAR_NONE)
				break ;
			if (ix != VAR_NONE && mSet[ix].mRed)
				break ;
			auto fax = FALSE ;
			if SWITCH_CASE (fax) {
				if (!(ix == mSet[iy].mLeft))
					discard ;
				update_remove_left (ix ,iy) ;
			}
			if SWITCH_CASE (fax) {
				if (!(ix == mSet[iy].mRight))
					discard ;
				update_remove_right (ix ,iy) ;
			}
			ix = mTop ;
			iy = mSet[ix].mUp ;
		}
		if (ix == VAR_NONE)
			return ;
		mSet[ix].mRed = FALSE ;
	}

	void update_remove_left (INDEX curr ,INDEX last) {
		auto &r1y = mSet[last].mRight ;
		if SWITCH_ONCE (TRUE) {
			if (!mSet[r1y].mRed)
				discard ;
			mSet[r1y].mRed = FALSE ;
			mSet[last].mRed = TRUE ;
			auto &r2y = prev_next (last) ;
			rotate_left (r2y) ;
			r2y = mTop ;
		}
		const auto r3x = BOOL (mSet[r1y].mLeft != VAR_NONE && mSet[mSet[r1y].mLeft].mRed) ;
		const auto r4x = BOOL (mSet[r1y].mRight != VAR_NONE && mSet[mSet[r1y].mRight].mRed) ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (r3x)
				discard ;
			if (r4x)
				discard ;
			mSet[r1y].mRed = TRUE ;
			mTop = last ;
		}
		if SWITCH_CASE (fax) {
			if (r4x)
				discard ;
			mSet[mSet[r1y].mLeft].mRed = FALSE ;
			mSet[r1y].mRed = TRUE ;
			rotate_right (r1y) ;
			r1y = mTop ;
			mSet[r1y].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1y].mRight].mRed = FALSE ;
			auto &r5y = prev_next (last) ;
			rotate_left (r5y) ;
			r5y = mTop ;
			mTop = mRoot ;
		}
		if SWITCH_CASE (fax) {
			mSet[r1y].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1y].mRight].mRed = FALSE ;
			auto &r6y = prev_next (last) ;
			rotate_left (r6y) ;
			r6y = mTop ;
			mTop = mRoot ;
		}
	}

	void update_remove_right (INDEX curr ,INDEX last) {
		auto &r1y = mSet[last].mLeft ;
		if SWITCH_ONCE (TRUE) {
			if (!mSet[r1y].mRed)
				discard ;
			mSet[r1y].mRed = FALSE ;
			mSet[last].mRed = TRUE ;
			auto &r2y = prev_next (last) ;
			rotate_right (r2y) ;
			r2y = mTop ;
		}
		const auto r3x = BOOL (mSet[r1y].mRight != VAR_NONE && mSet[mSet[r1y].mRight].mRed) ;
		const auto r4x = BOOL (mSet[r1y].mLeft != VAR_NONE && mSet[mSet[r1y].mLeft].mRed) ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (r3x)
				discard ;
			if (r4x)
				discard ;
			mSet[r1y].mRed = TRUE ;
			mTop = last ;
		}
		if SWITCH_CASE (fax) {
			if (r4x)
				discard ;
			mSet[mSet[r1y].mRight].mRed = FALSE ;
			mSet[r1y].mRed = TRUE ;
			rotate_left (r1y) ;
			r1y = mTop ;
			mSet[r1y].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1y].mLeft].mRed = FALSE ;
			auto &r5y = prev_next (last) ;
			rotate_right (r5y) ;
			r5y = mTop ;
			mTop = mRoot ;
		}
		if SWITCH_CASE (fax) {
			mSet[r1y].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1y].mLeft].mRed = FALSE ;
			auto &r6y = prev_next (last) ;
			rotate_right (r6y) ;
			r6y = mTop ;
			mTop = mRoot ;
		}
	}

	void rotate_left (INDEX curr) {
		INDEX ix = mSet[curr].mRight ;
		mSet[curr].mRight = mSet[ix].mLeft ;
		if (mSet[ix].mLeft != VAR_NONE)
			mSet[mSet[ix].mLeft].mUp = curr ;
		mSet[ix].mLeft = curr ;
		mSet[ix].mUp = mSet[curr].mUp ;
		mSet[curr].mUp = ix ;
		mTop = ix ;
	}

	void rotate_right (INDEX curr) {
		INDEX ix = mSet[curr].mLeft ;
		mSet[curr].mLeft = mSet[ix].mRight ;
		if (mSet[ix].mRight != VAR_NONE)
			mSet[mSet[ix].mRight].mUp = curr ;
		mSet[ix].mRight = curr ;
		mSet[ix].mUp = mSet[curr].mUp ;
		mSet[curr].mUp = ix ;
		mTop = ix ;
	}

	INDEX &prev_next (INDEX curr) & {
		if (mSet[curr].mUp == VAR_NONE)
			return mRoot ;
		if (mSet[mSet[curr].mUp].mLeft == curr)
			return mSet[mSet[curr].mUp].mLeft ;
		return mSet[mSet[curr].mUp].mRight ;
	}

	INDEX &prev_next (INDEX) && = delete ;

	INDEX successor_one (INDEX index) const {
		for (INDEX i = mSet[index].mRight ; i != VAR_NONE ; i = mSet[i].mLeft)
			if (mSet[i].mLeft == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	void eswap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1y = prev_next (index2) ;
		auto &r2y = _SWITCH_ (
			(mSet[index2].mLeft != VAR_NONE) ? (mSet[mSet[index2].mLeft].mUp) :
			index1) ;
		auto &r3y = _SWITCH_ (
			(mSet[index2].mRight != VAR_NONE) ? (mSet[mSet[index2].mRight].mUp) :
			index1) ;
		auto &r4y = prev_next (index1) ;
		auto &r5y = _SWITCH_ (
			(mSet[index1].mLeft != VAR_NONE) ? (mSet[mSet[index1].mLeft].mUp) :
			index2) ;
		auto &r6y = _SWITCH_ (
			(mSet[index1].mRight != VAR_NONE) ? (mSet[mSet[index1].mRight].mUp) :
			index2) ;
		r1y = index1 ;
		r2y = index1 ;
		r3y = index1 ;
		r4y = index2 ;
		r5y = index2 ;
		r6y = index2 ;
		_SWAP_ (mSet[index1].mRed ,mSet[index2].mRed) ;
		_SWAP_ (mSet[index1].mUp ,mSet[index2].mUp) ;
		_SWAP_ (mSet[index1].mLeft ,mSet[index2].mLeft) ;
		_SWAP_ (mSet[index1].mRight ,mSet[index2].mRight) ;
	}

	void compute_esort (INDEX curr ,Array<INDEX> &out ,INDEX &out_i) const {
		if (curr == VAR_NONE)
			return ;
		INDEX iw = out_i ;
		compute_esort (mSet[curr].mLeft ,out ,iw) ;
		out[iw++] = curr ;
		compute_esort (mSet[curr].mRight ,out ,iw) ;
		out_i = iw ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class HashSet ;

template <class KEY ,class ITEM ,class SIZE>
class HashSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = HashSet<KEY ,ITEM ,SIZE> ;

	class Node {
	private:
		friend HashSet ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		ITEM mItem ;
		FLAG mHash ;
		INDEX mNext ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,FLAG hash ,INDEX next) :mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (KEY &&key ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (const KEY &key ,ITEM &&item ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mHash (hash) ,mNext (next) {}
	} ;

	using PAIR_TYPE = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair final {
	public:
		friend HashSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mSet[index].mKey) ,item (base.mSet[index].mItem) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	Buffer<INDEX ,SIZE> mHead ;
	INDEX mTop ;

public:
	HashSet () {
		spec.clear () ;
	}

	explicit HashSet (LENGTH len) :HashSet (ARGVP0 ,len) {
		spec.clear () ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
			spec.update_resize (ix) ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
			spec.update_resize (ix) ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

private:
	explicit HashSet (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mSet (len) ,mHead (len) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class SIZE>
class HashSet<KEY ,SPECIALIZATION<void> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = HashSet<KEY ,void ,SIZE> ;

	class Node {
	private:
		friend HashSet ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		FLAG mHash ;
		INDEX mNext ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,FLAG hash ,INDEX next) :mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (KEY &&key ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}
	} ;

	using PAIR_TYPE = PACK<KEY> ;

	template <class BASE>
	class Pair final {
	public:
		friend HashSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mSet[index].mKey) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	Buffer<INDEX ,SIZE> mHead ;
	INDEX mTop ;

public:
	HashSet () {
		spec.clear () ;
	}

	explicit HashSet (LENGTH len) :HashSet (ARGVP0 ,len) {
		spec.clear () ;
	}

	void add (const KEY &key) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			spec.update_resize (ix) ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			spec.update_resize (ix) ;
			spec.update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

private:
	explicit HashSet (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mSet (len) ,mHead (len) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class ITEM ,class SIZE>
class HashSet :private HashSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = HashSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using PAIR_TYPE = typename SPECIALIZATION_BASE::PAIR_TYPE ;
	template <class _ARG1>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG1> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mSet ;
	using SPECIALIZATION_BASE::mHead ;
	using SPECIALIZATION_BASE::mTop ;

public:
	HashSet () = default ;

	explicit HashSet (LENGTH len) :SPECIALIZATION_BASE (len) {}

	implicit HashSet (const std::initializer_list<PAIR_TYPE> &that) : HashSet (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		return mSet.size () ;
	}

	LENGTH length () const {
		return mSet.length () ;
	}

	void clear () {
		mSet.clear () ;
		_MEMFILL_ (mHead.self ,mHead.size () ,VAR_NONE) ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ,ie = size () ; i < ie ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = index + 1 ,ie = size () ; i < ie ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	Pair<HashSet> get (INDEX index) & {
		return Pair<HashSet> ((*this) ,index) ;
	}

	inline Pair<HashSet> operator[] (INDEX index) & {
		return get (index) ;
	}

	Pair<const HashSet> get (INDEX index) const & {
		return Pair<const HashSet> ((*this) ,index) ;
	}

	inline Pair<const HashSet> operator[] (INDEX index) const & {
		return get (index) ;
	}

	Pair<HashSet> get (INDEX) && = delete ;

	inline Pair<HashSet> operator[] (INDEX) && = delete ;

	INDEX at (const Pair<HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	//@error: vs2015 is too useless to compile without hint
	INDEX at (const typename SPECIALIZATION_BASE::Pair_const_BASE &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	using SPECIALIZATION_BASE::add ;

	inline HashSet &operator<< (const PAIR_TYPE &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	inline HashSet &operator<< (PAIR_TYPE &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	using SPECIALIZATION_BASE::appand ;

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ret != VAR_NONE)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_resize (ret) ;
			update_insert (ret) ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ret != VAR_NONE)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_resize (ret) ;
			update_insert (ret) ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		prev_next (index) = mSet[index].mNext ;
		mSet.free (index) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = VAR_NONE ;
		if SWITCH_ONCE (TRUE) {
			if (size () == 0)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (key) ;
			_DEBUG_ASSERT_ (r1x >= 0) ;
			ret = mHead[r1x % mHead.size ()] ;
			while (TRUE) {
				if (ret == VAR_NONE)
					break ;
				if (mSet[ret].mHash == r1x && mSet[ret].mKey == key)
					break ;
				ret = mSet[ret].mNext ;
			}
		}
		return std::move (ret) ;
	}

	void erase (const KEY &key) {
		INDEX ix = find (key) ;
		if (ix == VAR_NONE)
			return ;
		remove (ix) ;
	}

private:
	void update_resize (INDEX curr) {
		if (mHead.size () == mSet.size ())
			return ;
		auto rax = mHead.expand (mSet.size ()) ;
		_MEMFILL_ (rax.self ,rax.size () ,VAR_NONE) ;
		for (INDEX i = 0 ,ie = mSet.size () ; i < ie ; i++) {
			if (i == curr)
				continue ;
			if (!mSet.used (i))
				continue ;
			INDEX ix = mSet[i].mHash % rax.size () ;
			mSet[i].mNext = rax[ix] ;
			rax[ix] = i ;
		}
		mHead.swap (rax) ;
	}

	void update_insert (INDEX curr) {
		INDEX ix = mSet[curr].mHash % mHead.size () ;
		mSet[curr].mNext = mHead[ix] ;
		mHead[ix] = curr ;
	}

	INDEX &prev_next (INDEX curr) & {
		INDEX ix = mSet[curr].mHash % mHead.size () ;
		if (mHead[ix] == curr)
			return mHead[ix] ;
		ix = mHead[ix] ;
		while (TRUE) {
			if (mSet[ix].mNext == VAR_NONE)
				break ;
			if (mSet[ix].mNext == curr)
				break ;
			ix = mSet[ix].mNext ;
		}
		_DEBUG_ASSERT_ (mSet[ix].mNext == curr) ;
		return mSet[ix].mNext ;
	}

	INDEX &prev_next (INDEX) && = delete ;
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class SoftSet ;

template <class KEY ,class ITEM ,class SIZE>
class SoftSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = SoftSet<KEY ,ITEM ,SIZE> ;

	class Node {
	private:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		ITEM mItem ;
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
		INDEX mNext ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) :mKey (std::move (key)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}

		inline explicit Node (KEY &&key ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) : mKey (std::move (key)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}

		inline explicit Node (const KEY &key ,ITEM &&item ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}
	} ;

	using PAIR_TYPE = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair final {
	public:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mSet.self[index].mKey) ,item (base.mSet.self[index].mItem) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

private:
	friend SPECIALIZATION_TYPE ;
	SharedRef<Allocator<Node ,SIZE>> mHeap ;
	PhanRef<Allocator<Node ,SIZE>> mSet ;
	LENGTH mLength ;
	INDEX mFirst ;
	INDEX mLast ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	SoftSet () {
		mLength = 0 ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
		mRoot = VAR_NONE ;
	}

	explicit SoftSet (LENGTH len) {
		mHeap = SharedRef<Allocator<Node ,SIZE>>::make (len) ;
		mSet = PhanRef<Allocator<Node ,SIZE>>::make (mHeap.self) ;
		mLength = 0 ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
		mRoot = VAR_NONE ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1y = _SWITCH_ (
				(mLast != VAR_NONE) ? (mSet.self[mLast].mNext) :
				(mFirst)) ;
			r1y = ix ;
			mLast = ix ;
			mLength++ ;
			spec.update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1y = _SWITCH_ (
				(mLast != VAR_NONE) ? (mSet.self[mLast].mNext) :
				(mFirst)) ;
			r1y = ix ;
			mLast = ix ;
			mLength++ ;
			spec.update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		mSet->reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		mSet->reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (val[i].key ,std::move (val[i].item)) ;
	}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class SIZE>
class SoftSet<KEY ,SPECIALIZATION<void> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = SoftSet<KEY ,void ,SIZE> ;

	class Node {
	private:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
		INDEX mNext ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) :mKey (std::move (key)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}

		inline explicit Node (KEY &&key ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) : mKey (std::move (key)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}
	} ;

	using PAIR_TYPE = PACK<KEY> ;

	template <class BASE>
	class Pair final {
	public:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) popping : key (base.mSet.self[index].mKey) {}
	} ;

	using Pair_BASE = Pair<SPECIALIZATION_TYPE> ;
	using Pair_const_BASE = Pair<const SPECIALIZATION_TYPE> ;

private:
	friend SPECIALIZATION_TYPE ;
	SharedRef<Allocator<Node ,SIZE>> mHeap ;
	PhanRef<Allocator<Node ,SIZE>> mSet ;
	LENGTH mLength ;
	INDEX mFirst ;
	INDEX mLast ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	SoftSet () {
		mLength = 0 ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
		mRoot = VAR_NONE ;
	}

	explicit SoftSet (LENGTH len) {
		mHeap = SharedRef<Allocator<Node ,SIZE>>::make (len) ;
		mSet = PhanRef<Allocator<Node ,SIZE>>::make (mHeap.self) ;
		mLength = 0 ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
		mRoot = VAR_NONE ;
	}

	void add (const KEY &key) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1y = _SWITCH_ (
				(mLast != VAR_NONE) ? (mSet.self[mLast].mNext) :
				(mFirst)) ;
			r1y = ix ;
			mLast = ix ;
			mLength++ ;
			spec.update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (const PAIR_TYPE &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = spec.find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1y = _SWITCH_ (
				(mLast != VAR_NONE) ? (mSet.self[mLast].mNext) :
				(mFirst)) ;
			r1y = ix ;
			mLast = ix ;
			mLength++ ;
			spec.update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (PAIR_TYPE &&item) {
		add (std::move (item.P1)) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		mSet->reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

	template <class _ARG1>
	void appand (_ARG1 &&val) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		mSet->reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,ie = val.iend () ; i != ie ; i = val.inext (i))
			add (std::move (val[i])) ;
	}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class KEY ,class ITEM ,class SIZE>
class SoftSet :private SoftSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = SoftSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using PAIR_TYPE = typename SPECIALIZATION_BASE::PAIR_TYPE ;
	template <class _ARG1>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG1> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mHeap ;
	using SPECIALIZATION_BASE::mSet ;
	using SPECIALIZATION_BASE::mLength ;
	using SPECIALIZATION_BASE::mFirst ;
	using SPECIALIZATION_BASE::mLast ;
	using SPECIALIZATION_BASE::mRoot ;
	using SPECIALIZATION_BASE::mTop ;

public:
	SoftSet () = default ;

	explicit SoftSet (LENGTH len) :SPECIALIZATION_BASE (len) {}

	LENGTH capacity () const {
		if (!mHeap.exist ())
			return 0 ;
		return mSet->size () ;
	}

	LENGTH size () const {
		if (!mHeap.exist ())
			return 0 ;
		return mSet->size () - (mSet->length () - mLength) ;
	}

	LENGTH length () const {
		if (!mHeap.exist ())
			return 0 ;
		return mLength ;
	}

	inline SoftSet share () popping {
		SoftSet ret ;
		ret.mHeap = mHeap ;
		ret.mSet = PhanRef<Allocator<Node ,SIZE>>::make (ret.mHeap.self) ;
		ret.mLength = 0 ;
		ret.mFirst = VAR_NONE ;
		ret.mLast = VAR_NONE ;
		ret.mRoot = VAR_NONE ;
		return std::move (ret) ;
	}

	INDEX ibegin () const {
		if (!mHeap.exist ())
			return VAR_NONE ;
		return mFirst ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return mSet.self[index].mNext ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	Pair<SoftSet> get (INDEX index) & {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		return Pair<SoftSet> ((*this) ,index) ;
	}

	inline Pair<SoftSet> operator[] (INDEX index) & {
		return get (index) ;
	}

	Pair<const SoftSet> get (INDEX index) const & {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		return Pair<const SoftSet> ((*this) ,index) ;
	}

	inline Pair<const SoftSet> operator[] (INDEX index) const & {
		return get (index) ;
	}

	Pair<SoftSet> get (INDEX) && = delete ;

	inline Pair<SoftSet> operator[] (INDEX) && = delete ;

	INDEX at (const Pair<SoftSet> &item) const {
		return mSet->at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	//@error: vs2015 is too useless to compile without hint
	INDEX at (const typename SPECIALIZATION_BASE::Pair_const_BASE &item) const {
		return mSet->at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	using SPECIALIZATION_BASE::add ;

	inline SoftSet &operator<< (const PAIR_TYPE &item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	inline SoftSet &operator<< (PAIR_TYPE &&item) {
		add (std::move (item)) ;
		return (*this) ;
	}

	using SPECIALIZATION_BASE::appand ;

	INDEX insert (const KEY &key) popping {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ret = find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ret != VAR_NONE)
				discard ;
			ret = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1y = _SWITCH_ (
				(mLast != VAR_NONE) ? (mSet.self[mLast].mNext) :
				(mFirst)) ;
			r1y = ret ;
			mLast = ret ;
			mLength++ ;
			update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ret = find (key) ;
		if SWITCH_ONCE (TRUE) {
			if (ret != VAR_NONE)
				discard ;
			ret = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1y = _SWITCH_ (
				(mLast != VAR_NONE) ? (mSet.self[mLast].mNext) :
				(mFirst)) ;
			r1y = ret ;
			mLast = ret ;
			mLength++ ;
			update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX min_one () const {
		if (!mHeap.exist ())
			return VAR_NONE ;
		for (INDEX i = mRoot ; i != VAR_NONE ; i = mSet.self[i].mLeft)
			if (mSet.self[i].mLeft == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	INDEX max_one () const {
		if (!mHeap.exist ())
			return VAR_NONE ;
		for (INDEX i = mRoot ; i != VAR_NONE ; i = mSet.self[i].mRight)
			if (mSet.self[i].mRight == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = VAR_NONE ;
		if (mHeap.exist ())
			ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = BOOL (key < mSet.self[ret].mKey) ;
			if (!r1x && !(key > mSet.self[ret].mKey))
				break ;
			const auto r2x = _SWITCH_ (
				r1x ? (mSet.self[ret].mLeft) :
				(mSet.self[ret].mRight)) ;
			ret = r2x ;
		}
		return std::move (ret) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		compute_esort (mRoot ,ret ,iw) ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	void clean () {
		if (!mHeap.exist ())
			return ;
		mSet->clean () ;
	}

private:
	void update_insert (INDEX curr) {
		INDEX ix = curr ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(ix != VAR_NONE))
				discard ;
			mSet.self[ix].mWeight++ ;
			const auto r1x = BOOL (mSet.self[mLast].mKey < mSet.self[ix].mKey) ;
			auto fbx = FALSE ;
			if SWITCH_CASE (fbx) {
				if (!r1x)
					discard ;
				update_insert (mSet.self[ix].mLeft) ;
				mSet.self[ix].mLeft = mTop ;
				update_insert_left (ix) ;
			}
			if SWITCH_CASE (fbx) {
				if (r1x)
					discard ;
				update_insert (mSet.self[ix].mRight) ;
				mSet.self[ix].mRight = mTop ;
				update_insert_right (ix) ;
			}
			ix = mTop ;
			mTop = ix ;
		}
		if SWITCH_CASE (fax) {
			mTop = mLast ;
		}
	}

	void update_insert_left (INDEX curr) {
		INDEX ix = curr ;
		mTop = ix ;
		if (mSet.self[ix].mLeft == VAR_NONE)
			return ;
		const auto r1x = node_weight (mSet.self[ix].mRight) ;
		const auto r2x = node_weight (mSet.self[mSet.self[ix].mLeft].mLeft) ;
		const auto r3x = node_weight (mSet.self[mSet.self[ix].mLeft].mRight) ;
		mTop = ix ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if SWITCH_ONCE (TRUE) {
			if (r1x < r2x)
				discard ;
			auto &r4y = mSet.self[ix].mLeft ;
			rotate_left (r4y) ;
			r4y = mTop ;
		}
		rotate_right (ix) ;
		ix = mTop ;
		auto &r5y = mSet.self[ix].mLeft ;
		update_insert_left (r5y) ;
		r5y = mTop ;
		auto &r6y = mSet.self[ix].mRight ;
		update_insert_right (r6y) ;
		r6y = mTop ;
		update_insert_left (ix) ;
		ix = mTop ;
		update_insert_right (ix) ;
		ix = mTop ;
		mTop = ix ;
	}

	void update_insert_right (INDEX curr) {
		INDEX ix = curr ;
		mTop = ix ;
		if (mSet.self[ix].mRight == VAR_NONE)
			return ;
		const auto r1x = node_weight (mSet.self[ix].mLeft) ;
		const auto r2x = node_weight (mSet.self[mSet.self[ix].mRight].mRight) ;
		const auto r3x = node_weight (mSet.self[mSet.self[ix].mRight].mLeft) ;
		mTop = ix ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if SWITCH_ONCE (TRUE) {
			if (r1x < r2x)
				discard ;
			auto &r4y = mSet.self[ix].mRight ;
			rotate_right (r4y) ;
			r4y = mTop ;
		}
		rotate_left (ix) ;
		ix = mTop ;
		auto &r5y = mSet.self[ix].mLeft ;
		update_insert_left (r5y) ;
		r5y = mTop ;
		auto &r6y = mSet.self[ix].mRight ;
		update_insert_right (r6y) ;
		r6y = mTop ;
		update_insert_left (ix) ;
		ix = mTop ;
		update_insert_right (ix) ;
		ix = mTop ;
		mTop = ix ;
	}

	void rotate_left (INDEX curr) {
		INDEX ix = mSet.self[curr].mRight ;
		mSet.self[curr].mRight = mSet.self[ix].mLeft ;
		mSet.self[ix].mLeft = curr ;
		mSet.self[ix].mWeight = mSet.self[curr].mWeight ;
		const auto r1x = node_weight (mSet.self[curr].mLeft) ;
		const auto r2x = node_weight (mSet.self[curr].mRight) ;
		mSet.self[curr].mWeight = r1x + r2x + 1 ;
		mTop = ix ;
	}

	void rotate_right (INDEX curr) {
		INDEX ix = mSet.self[curr].mLeft ;
		mSet.self[curr].mLeft = mSet.self[ix].mRight ;
		mSet.self[ix].mRight = curr ;
		mSet.self[ix].mWeight = mSet.self[curr].mWeight ;
		const auto r1x = node_weight (mSet.self[curr].mLeft) ;
		const auto r2x = node_weight (mSet.self[curr].mRight) ;
		mSet.self[curr].mWeight = r1x + r2x + 1 ;
		mTop = ix ;
	}

	LENGTH node_weight (INDEX curr) const {
		if (curr == VAR_NONE)
			return 0 ;
		return mSet.self[curr].mWeight ;
	}

	void compute_esort (INDEX curr ,Array<INDEX> &out ,INDEX &out_i) const {
		if (curr == VAR_NONE)
			return ;
		INDEX iw = out_i ;
		compute_esort (mSet[curr].mLeft ,out ,iw) ;
		out[iw++] = curr ;
		compute_esort (mSet[curr].mRight ,out ,iw) ;
		out_i = iw ;
	}
} ;
} ;