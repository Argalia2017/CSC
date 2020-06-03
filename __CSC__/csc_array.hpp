#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
template <class BASE>
class ArrayIterator
	:private Proxy {
private:
	friend BASE ;
	BASE &mBase ;
	INDEX mIndex ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (BASE &base ,const INDEX &index)
		:mBase (base) ,mIndex (index) {}

	inline BOOL operator!= (const ArrayIterator &that) const {
		return BOOL (mIndex != that.mIndex) ;
	}

	template <class _RET = DEF<decltype (_NULL_ (ARGV<BASE>::null).get (_NULL_ (ARGV<const INDEX>::null)))>>
	inline _RET operator* () const leftvalue {
		return mBase.get (_XVALUE_ (ARGV<const INDEX>::null ,mIndex)) ;
	}

	inline void operator++ () {
		mIndex = mBase.inext (_XVALUE_ (ARGV<const INDEX>::null ,mIndex)) ;
	}
} ;

namespace U {
struct OPERATOR_SORT {
	template <class _ARG1 ,class _ARG2>
	imports void insert_sort (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &seg_a ,const INDEX &seg_b) {
		for (auto &&i : _RANGE_ (seg_a ,seg_b)) {
			INDEX ix = i + 1 ;
			INDEX iy = i ;
			auto tmp = _MOVE_ (out[ix]) ;
			while (TRUE) {
				if (iy < seg_a)
					break ;
				if (array_[tmp] >= array_[out[iy]])
					break ;
				out[ix] = _MOVE_ (out[iy]) ;
				ix = iy ;
				iy-- ;
			}
			out[ix] = _MOVE_ (tmp) ;
		}
	}

	template <class _ARG1 ,class _ARG2>
	imports void quick_sort_partition (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &seg_a ,const INDEX &seg_b ,INDEX &mid_one) {
		INDEX ix = seg_a ;
		INDEX iy = seg_b ;
		auto tmp = _MOVE_ (out[ix]) ;
		while (TRUE) {
			while (TRUE) {
				if (ix >= iy)
					break ;
				if (array_[out[iy]] <= array_[tmp])
					break ;
				iy-- ;
			}
			if (ix >= iy)
				break ;
			out[ix++] = _MOVE_ (out[iy]) ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				if (array_[out[ix]] >= array_[tmp])
					break ;
				ix++ ;
			}
			if (ix >= iy)
				break ;
			out[iy--] = _MOVE_ (out[ix]) ;
		}
		out[ix] = _MOVE_ (tmp) ;
		mid_one = ix ;
	}

	template <class _ARG1 ,class _ARG2>
	imports void quick_sort (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &seg_a ,const INDEX &seg_b ,const LENGTH &ideal) {
		auto rax = ideal ;
		INDEX ix = seg_a ;
		INDEX iy = seg_b ;
		while (TRUE) {
			if (ix >= iy)
				break ;
			if (rax <= 0)
				break ;
			rax = rax / 2 + rax / 4 ;
			INDEX jx = VAR_NONE ;
			quick_sort_partition (array_ ,out ,ix ,iy ,jx) ;
			INDEX iz = jx - 1 ;
			quick_sort (array_ ,out ,ix ,iz ,rax) ;
			ix = jx + 1 ;
		}
		if (ix >= iy)
			return ;
		insert_sort (array_ ,out ,ix ,iy) ;
	}

	template <class _ARG1 ,class _ARG2>
	imports void invoke (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &seg ,const LENGTH &seg_len) {
		_DEBUG_ASSERT_ (seg_len > 0) ;
		_DEBUG_ASSERT_ (seg >= 0 && seg <= out.size () - seg_len) ;
		quick_sort (array_ ,out ,seg ,(seg + seg_len - 1) ,seg_len) ;
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
	implicit Array () = default ;

	explicit Array (const LENGTH &len)
		:Array (ARGVP0 ,len) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit Array (const stl::initializer_list<ITEM> &that)
		: Array (that.size ()) {
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

	PhanBuffer<ITEM> raw () leftvalue {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<ITEM>::make (mArray ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const leftvalue {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mArray ,length ()) ;
	}

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (const INDEX &index) const {
		return index + 1 ;
	}

	ArrayIterator<Array> begin () leftvalue {
		return ArrayIterator<Array> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const Array> begin () const leftvalue {
		return ArrayIterator<const Array> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<Array> end () leftvalue {
		return ArrayIterator<Array> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const Array> end () const leftvalue {
		return ArrayIterator<const Array> (DEREF[this] ,iend ()) ;
	}

	ITEM &get (const INDEX &index) leftvalue {
		return mArray[index] ;
	}

	inline ITEM &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mArray[index] ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

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

	void fill (const ITEM &item) {
		for (auto &&i : _RANGE_ (0 ,mArray.size ()))
			mArray[i] = item ;
	}

private:
	explicit Array (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mArray (len) {}
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

namespace U {
struct CONSTEXPR_RESERVE_SIZE {
	imports constexpr LENGTH invoke (const LENGTH &len) {
		return len + _EBOOL_ (len > 0) ;
	}
} ;
} ;

template <class>
class TextWriter ;

template <class ITEM ,class SIZE = SAUTO>
class String ;

template <class ITEM ,class SIZE>
class String {
private:
	Buffer<ITEM ,ARGC<(U::CONSTEXPR_RESERVE_SIZE::invoke (SIZE::value))>> mString ;

public:
	implicit String () {
		clear () ;
	}

	explicit String (const LENGTH &len)
		:String (ARGVP0 ,U::CONSTEXPR_RESERVE_SIZE::invoke (len)) {
		clear () ;
	}

	implicit String (const stl::initializer_list<ITEM> &that)
		: String (that.size ()) {
		_DEBUG_ASSERT_ (size () == LENGTH (that.size ())) ;
		INDEX iw = 0 ;
		for (auto &&i : that)
			mString[iw++] = i ;
		mString[iw++] = ITEM (0) ;
		_DEBUG_ASSERT_ (iw == mString.size ()) ;
	}

	implicit String (const ARR<ITEM> &that)
		:String (plain_string_length (that)) {
		BasicProc::mem_copy (mString.self ,that ,size ()) ;
	}

	implicit String (const Plain<ITEM> &that)
		: String (that.size ()) {
		BasicProc::mem_copy (mString.self ,that.self ,size ()) ;
	}

	LENGTH size () const {
		if (mString.size () == 0)
			return 0 ;
		return mString.size () - 1 ;
	}

	LENGTH length () const {
		if (mString.size () == 0)
			return 0 ;
		return BasicProc::mem_chr (mString.self ,mString.size () ,ITEM (0)) ;
	}

	PhanBuffer<ITEM> raw () leftvalue {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<ITEM>::make (mString ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const leftvalue {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mString ,length ()) ;
	}

	void clear () {
		if (mString.size () == 0)
			return ;
		BasicProc::mem_fill (mString.self ,mString.size () ,ITEM (0)) ;
	}

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (const INDEX &index) const {
		return index + 1 ;
	}

	ArrayIterator<String> begin () leftvalue {
		return ArrayIterator<String> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const String> begin () const leftvalue {
		return ArrayIterator<const String> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<String> end () leftvalue {
		return ArrayIterator<String> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const String> end () const leftvalue {
		return ArrayIterator<const String> (DEREF[this] ,iend ()) ;
	}

	ITEM &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mString[index] ;
	}

	inline ITEM &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mString[index] ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mString.at (item) ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const String &that) const {
		if (empty ())
			if (that.empty ())
				return TRUE ;
		if (empty ())
			return FALSE ;
		if (that.empty ())
			return FALSE ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (mString[ix] == ITEM (0))
				break ;
			if (mString[ix] != that.mString[ix])
				break ;
			ix++ ;
		}
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
		if (!BasicProc::mem_equal (mString.self ,that.self ,(that.size () + 1)))
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
			return U::OPERATOR_COMPR::invoke (r1x ,r2x) ;
		if (r2x == 0)
			return U::OPERATOR_COMPR::invoke (r1x ,r2x) ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (mString[ix] == ITEM (0))
				break ;
			if (mString[ix] != that.mString[ix])
				break ;
			ix++ ;
		}
		return U::OPERATOR_COMPR::invoke (mString[ix] ,that.mString[ix]) ;
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
		BasicProc::mem_copy (ret.mString.self ,mString.self ,r1x) ;
		BasicProc::mem_copy (PTRTOARR[DEPTR[ret.mString.self[r1x]]] ,that.mString.self ,r2x) ;
		return _MOVE_ (ret) ;
	}

	inline String operator+ (const String &that) const {
		return concat (that) ;
	}

	inline String operator- (const String &that) const {
		return that.concat (DEREF[this]) ;
	}

	void concatto (const String &that) {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mString.size () > 0))
				discard ;
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			if (!(r1x + r2x <= size ()))
				discard ;
			BasicProc::mem_copy (PTRTOARR[DEPTR[mString.self[r1x]]] ,that.mString.self ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		}
		if switch_once (fax) {
			DEREF[this] = concat (that) ;
		}
	}

	inline String &operator+= (const String &that) {
		concatto (that) ;
		return DEREF[this] ;
	}

	void concatto (const Plain<ITEM> &that) {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mString.size () > 0))
				discard ;
			const auto r1x = length () ;
			const auto r2x = that.size () ;
			if (!(r1x + r2x <= size ()))
				discard ;
			BasicProc::mem_copy (PTRTOARR[DEPTR[mString.self[r1x]]] ,that.self ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		}
		if switch_once (fax) {
			DEREF[this] = concat (that) ;
		}
	}

	inline String &operator+= (const Plain<ITEM> &that) {
		concatto (that) ;
		return DEREF[this] ;
	}

	String segment (const INDEX &seg ,const LENGTH &seg_len) const {
		_DEBUG_ASSERT_ (seg_len > 0) ;
		_DEBUG_ASSERT_ (seg >= 0 && seg <= size () - seg_len) ;
		String ret = String (seg_len) ;
		for (auto &&i : _RANGE_ (0 ,ret.size ()))
			ret.get (i) = get (seg + i) ;
		return _MOVE_ (ret) ;
	}

public:
	template <class... _ARGS>
	imports String make (const _ARGS &...initval) {
		struct Dependent ;
		using TextWriter_ITEM = DEPENDENT_TYPE<TextWriter<ITEM> ,Dependent> ;
		_STATIC_ASSERT_ (stl::is_same<SIZE ,SAUTO>::value) ;
		String ret = String (DEFAULT_LONGSTRING_SIZE::value) ;
		auto rax = TextWriter_ITEM (ret.raw ()) ;
		rax.prints (initval...) ;
		rax << TextWriter_ITEM::EOS ;
		return _MOVE_ (ret) ;
	}

private:
	explicit String (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mString (len) {}

private:
	imports LENGTH plain_string_length (const ARR<ITEM> &val) {
		const auto r1x = DEFAULT_HUGESTRING_SIZE::value + 1 ;
		LENGTH ret = BasicProc::mem_chr (val ,r1x ,ITEM (0)) ;
		_DYNAMIC_ASSERT_ (ret >= 0 && ret <= DEFAULT_HUGESTRING_SIZE::value) ;
		return _MOVE_ (ret) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Deque ;

template <class ITEM ,class SIZE>
class Deque {
private:
	Buffer<ITEM ,ARGC<(U::CONSTEXPR_RESERVE_SIZE::invoke (SIZE::value))>> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit Deque () {
		clear () ;
	}

	explicit Deque (const LENGTH &len)
		:Deque (ARGVP0 ,U::CONSTEXPR_RESERVE_SIZE::invoke (len)) {
		clear () ;
	}

	implicit Deque (const stl::initializer_list<ITEM> &that)
		: Deque (that.size ()) {
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

	INDEX inext (const INDEX &index) const {
		return (index + 1) % mDeque.size () ;
	}

	ArrayIterator<Deque> begin () leftvalue {
		return ArrayIterator<Deque> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const Deque> begin () const leftvalue {
		return ArrayIterator<const Deque> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<Deque> end () leftvalue {
		return ArrayIterator<Deque> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const Deque> end () const leftvalue {
		return ArrayIterator<const Deque> (DEREF[this] ,iend ()) ;
	}

	//@warn: index would be no longer valid once resized
	ITEM &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mDeque[index] ;
	}

	inline ITEM &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	//@warn: index would be no longer valid once resized
	const ITEM &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mDeque[index] ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mDeque.at (item) ;
		if (!ensure_index (ret))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	INDEX access (const INDEX &pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		return (mRead + pos) % mDeque.size () ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		U::OPERATOR_SORT::invoke (DEREF[this] ,ret ,0 ,ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const Deque &that) const {
		if (length () != that.length ())
			return FALSE ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		INDEX jx = that.ibegin () ;
		INDEX jy = that.iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			if (jx == jy)
				break ;
			if (get (ix) != that.get (jx))
				break ;
			ix = inext (ix) ;
			jx = that.inext (jx) ;
		}
		if (ix != iy)
			return FALSE ;
		if (jx != jy)
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

	void add (const REMOVE_CVR_TYPE<ITEM> &item) {
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		mDeque[mWrite] = _MOVE_ (item) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_resize () ;
	}

	inline Deque &operator<< (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		mDeque[mWrite] = _MOVE_ (item) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_resize () ;
	}

	inline Deque &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i]) ;
		}
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mRead = (mRead + 1) % mDeque.size () ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = _MOVE_ (mDeque[mRead]) ;
		mRead = (mRead + 1) % mDeque.size () ;
	}

	inline Deque &operator>> (ITEM &item) {
		take (item) ;
		return DEREF[this] ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mRead ;
	}

	INDEX tail () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
	}

	INDEX insert () side_effects {
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		INDEX ret = mWrite ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_resize () ;
		return _MOVE_ (ret) ;
	}

	INDEX insert_sort (const REMOVE_CVR_TYPE<ITEM> &item) side_effects {
		_DEBUG_ASSERT_ (mRead == 0) ;
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		INDEX ret = mWrite ;
		while (TRUE) {
			if (ret - 1 < 0)
				break ;
			if (mDeque[ret - 1] >= item)
				break ;
			mDeque[ret] = _MOVE_ (mDeque[ret - 1]) ;
			ret-- ;
		}
		mDeque[ret] = _MOVE_ (item) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_resize () ;
		return _MOVE_ (ret) ;
	}

	INDEX insert_sort (REMOVE_CVR_TYPE<ITEM> &&item) side_effects {
		_DEBUG_ASSERT_ (mRead == 0) ;
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		INDEX ret = mWrite ;
		while (TRUE) {
			if (ret - 1 < 0)
				break ;
			if (mDeque[ret - 1] >= item)
				break ;
			mDeque[ret] = _MOVE_ (mDeque[ret - 1]) ;
			ret-- ;
		}
		mDeque[ret] = _MOVE_ (item) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_resize () ;
		return _MOVE_ (ret) ;
	}

	void push (const REMOVE_CVR_TYPE<ITEM> &item) {
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		INDEX ix = (mRead - 1 + mDeque.size ()) % mDeque.size () ;
		mDeque[ix] = _MOVE_ (item) ;
		mRead = ix ;
		update_resize () ;
	}

	void push (REMOVE_CVR_TYPE<ITEM> &&item) {
		if (mDeque.size () == 0)
			reserve (mDeque.expand_size ()) ;
		INDEX ix = (mRead - 1 + mDeque.size ()) % mDeque.size () ;
		mDeque[ix] = _MOVE_ (item) ;
		mRead = ix ;
		update_resize () ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite = (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
	}

private:
	explicit Deque (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mDeque (len) {}

private:
	BOOL ensure_index (const INDEX &index) const {
		if (mRead <= mWrite)
			if (!(index >= mRead && index < mWrite))
				return FALSE ;
		if (mRead > mWrite)
			if (!(index >= 0 && index < mWrite))
				if (!(index >= mRead && index < mDeque.size ()))
					return FALSE ;
		return TRUE ;
	}

	void reserve (const LENGTH &len) {
		const auto r1x = len - (size () - length ()) ;
		const auto r2x = VAR_ZERO ;
		const auto r3x = _MAX_ (r1x ,r2x) ;
		if (r3x == 0)
			return ;
		auto tmp = mDeque.expand (mDeque.size () + r3x) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRead <= mWrite))
				discard ;
			BasicProc::mem_move (PTRTOARR[DEPTR[tmp.self[mRead]]] ,PTRTOARR[DEPTR[mDeque.self[mRead]]] ,(mWrite - mRead)) ;
		}
		if switch_once (fax) {
			if (!(mRead > mWrite))
				discard ;
			BasicProc::mem_move (tmp.self ,mDeque.self ,mWrite) ;
			INDEX ix = mRead + tmp.size () - mDeque.size () ;
			BasicProc::mem_move (PTRTOARR[DEPTR[tmp.self[ix]]] ,PTRTOARR[DEPTR[mDeque.self[mRead]]] ,(mDeque.size () - mRead)) ;
			mRead = ix ;
		}
		mDeque.swap (tmp) ;
	}

	void update_resize () {
		if (mRead != mWrite)
			return ;
		auto tmp = mDeque.expand (mDeque.expand_size ()) ;
		BasicProc::mem_move (tmp.self ,mDeque.self ,mWrite) ;
		INDEX ix = 0 ;
		INDEX iy = mDeque.size () ;
		if switch_once (TRUE) {
			if (mRead == 0)
				discard ;
			ix = mRead + tmp.size () - mDeque.size () ;
			iy = mWrite ;
		}
		BasicProc::mem_move (PTRTOARR[DEPTR[tmp.self[ix]]] ,PTRTOARR[DEPTR[mDeque.self[mRead]]] ,(mDeque.size () - mRead)) ;
		mRead = ix ;
		mWrite = iy ;
		mDeque.swap (tmp) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Priority ;

template <class ITEM ,class SIZE>
class Priority {
private:
	struct NODE {
		ITEM mItem ;
		INDEX mMap ;
	} ;

	struct Private {
		template <class>
		class Pair ;
	} ;

private:
	Buffer<NODE ,ARGC<(U::CONSTEXPR_RESERVE_SIZE::invoke (SIZE::value))>> mPriority ;
	INDEX mWrite ;
	INDEX mTop ;

public:
	implicit Priority () {
		clear () ;
	}

	explicit Priority (const LENGTH &len)
		:Priority (ARGVP0 ,U::CONSTEXPR_RESERVE_SIZE::invoke (len)) {
		clear () ;
	}

	implicit Priority (const stl::initializer_list<ITEM> &that)
		: Priority (that.size ()) {
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
		mTop = VAR_NONE ;
	}

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (const INDEX &index) const {
		return index + 1 ;
	}

	ArrayIterator<Priority> begin () leftvalue {
		return ArrayIterator<Priority> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const Priority> begin () const leftvalue {
		return ArrayIterator<const Priority> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<Priority> end () leftvalue {
		return ArrayIterator<Priority> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const Priority> end () const leftvalue {
		return ArrayIterator<const Priority> (DEREF[this] ,iend ()) ;
	}

	//@warn: index would be no longer valid every time revised
	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<Priority>>>
	_RET get (const INDEX &index) leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<Priority> ;
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<Priority>>>
	inline _RET operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	//@warn: index would be no longer valid every time revised
	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const Priority>>>
	_RET get (const INDEX &index) const leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const Priority> ;
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const Priority>>>
	inline _RET operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&NODE::mItem ,item)) ;
		if (!(ret >= 0 && ret < mWrite))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	INDEX at (const DEF<typename Private::template Pair<Priority>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<Priority> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	INDEX at (const DEF<typename Private::template Pair<const Priority>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const Priority> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		INDEX ix = ret.length () ;
		INDEX iy = ix - 1 ;
		while (TRUE) {
			if (iy < 1)
				break ;
			_SWAP_ (ret[0] ,ret[iy]) ;
			compute_esort (ret ,iy) ;
			ix = iy ;
			iy-- ;
		}
		if (ret.size () > 0)
			BasicProc::mem_rcopy (ret.raw ().self ,ret.raw ().self ,ret.size ()) ;
		return _MOVE_ (ret) ;
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

	void add (const ITEM &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Priority &operator<< (const ITEM &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item ,const INDEX &map_) {
		if (mPriority.size () == 0)
			reserve (mPriority.expand_size ()) ;
		INDEX ix = mWrite ;
		mPriority[ix].mItem = _MOVE_ (item) ;
		mPriority[ix].mMap = map_ ;
		mWrite++ ;
		update_resize () ;
		update_insert (ix) ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Priority &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item ,const INDEX &map_) {
		if (mPriority.size () == 0)
			reserve (mPriority.expand_size ()) ;
		INDEX ix = mWrite ;
		mPriority[ix].mItem = _MOVE_ (item) ;
		mPriority[ix].mMap = map_ ;
		mWrite++ ;
		update_resize () ;
		update_insert (ix) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i].key ,val[i].sid) ;
		}
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mPriority[0] = _MOVE_ (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (0) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = _MOVE_ (mPriority[0]) ;
		mPriority[0] = _MOVE_ (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (0) ;
	}

	inline Priority &operator>> (ITEM &item) {
		take (item) ;
		return DEREF[this] ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return 0 ;
	}

	INDEX insert (const REMOVE_CVR_TYPE<ITEM> &item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CVR_TYPE<ITEM> &&item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	void remove (const INDEX &index) {
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		mPriority[index] = _MOVE_ (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (index) ;
	}

private:
	explicit Priority (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mPriority (len) {}

private:
	void reserve (const LENGTH &len) {
		const auto r1x = len - (size () - length ()) ;
		const auto r2x = VAR_ZERO ;
		const auto r3x = _MAX_ (r1x ,r2x) ;
		if (r3x == 0)
			return ;
		auto tmp = mPriority.expand (mPriority.size () + r3x) ;
		BasicProc::mem_move (tmp.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.swap (tmp) ;
	}

	void update_resize () {
		if (mWrite < mPriority.size ())
			return ;
		auto tmp = mPriority.expand (mPriority.expand_size ()) ;
		BasicProc::mem_move (tmp.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.swap (tmp) ;
	}

	INDEX parent (INDEX curr) const {
		if (curr == 0)
			return VAR_NONE ;
		return (curr - 1) / 2 ;
	}

	INDEX left_child (INDEX curr) const {
		return curr * 2 + 1 ;
	}

	INDEX right_child (INDEX curr) const {
		return curr * 2 + 2 ;
	}

	void update_insert (const INDEX &curr) {
		INDEX ix = curr ;
		auto tmp = _MOVE_ (mPriority[ix]) ;
		while (TRUE) {
			INDEX iy = parent (ix) ;
			if (iy < 0)
				break ;
			if (tmp.mItem >= mPriority[iy].mItem)
				break ;
			mPriority[ix] = _MOVE_ (mPriority[iy]) ;
			ix = iy ;
		}
		while (TRUE) {
			INDEX iy = left_child (ix) ;
			if (iy >= mWrite)
				break ;
			INDEX jx = ix ;
			if (tmp.mItem > mPriority[iy].mItem)
				jx = iy ;
			INDEX iz = right_child (ix) ;
			auto &r1x = _SWITCH_ (
				(jx != ix) ? mPriority[jx].mItem :
				tmp.mItem) ;
			if switch_once (TRUE) {
				if (iz >= mWrite)
					discard ;
				if (r1x <= mPriority[iz].mItem)
					discard ;
				jx = iz ;
			}
			if (jx == ix)
				break ;
			mPriority[ix] = _MOVE_ (mPriority[jx]) ;
			ix = jx ;
		}
		mPriority[ix] = _MOVE_ (tmp) ;
		mTop = ix ;
	}

	void compute_esort (Array<INDEX> &out ,const LENGTH &len) const {
		INDEX ix = 0 ;
		const auto r1x = out[ix] ;
		while (TRUE) {
			INDEX iy = left_child (ix) ;
			if (iy >= len)
				break ;
			INDEX jx = ix ;
			if (mPriority[r1x].mItem > mPriority[out[iy]].mItem)
				jx = iy ;
			INDEX iz = right_child (ix) ;
			auto &r2x = _SWITCH_ (
				(jx != ix) ? mPriority[out[jx]].mItem :
				mPriority[r1x].mItem) ;
			if switch_once (TRUE) {
				if (iz >= len)
					discard ;
				if (r2x <= mPriority[out[iz]].mItem)
					discard ;
				jx = iz ;
			}
			if (jx == ix)
				break ;
			out[ix] = out[jx] ;
			ix = jx ;
		}
		out[ix] = r1x ;
	}
} ;

template <class ITEM ,class SIZE>
template <class BASE>
class Priority<ITEM ,SIZE>::Private::Pair
	:private Proxy {
public:
	const ITEM &key ;
	CAST_TRAITS_TYPE<INDEX ,BASE> &sid ;

public:
	implicit Pair () = delete ;

	explicit Pair (BASE &base ,const INDEX &index)
		: key (base.mPriority[index].mItem) ,sid (base.mPriority[index].mMap) {}

	inline implicit operator const ITEM & () rightvalue {
		return key ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class List ;

template <class ITEM ,class SIZE>
class List {
private:
	class Node {
	private:
		friend List ;
		ITEM mItem ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		template <class... _ARGS>
		explicit Node (_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS>::null ,initval)...) ,mLeft (VAR_NONE) ,mRight (VAR_NONE) {}
	} ;

private:
	Allocator<Node ,SIZE> mList ;
	INDEX mFirst ;
	INDEX mLast ;

public:
	implicit List () {
		clear () ;
	}

	explicit List (const LENGTH &len)
		:List (ARGVP0 ,len) {
		clear () ;
	}

	implicit List (const stl::initializer_list<ITEM> &that)
		: List (that.size ()) {
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

	INDEX inext (const INDEX &index) const {
		return mList[index].mRight ;
	}

	ArrayIterator<List> begin () leftvalue {
		return ArrayIterator<List> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const List> begin () const leftvalue {
		return ArrayIterator<const List> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<List> end () leftvalue {
		return ArrayIterator<List> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const List> end () const leftvalue {
		return ArrayIterator<const List> (DEREF[this] ,iend ()) ;
	}

	ITEM &get (const INDEX &index) leftvalue {
		return mList[index].mItem ;
	}

	inline ITEM &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mList[index].mItem ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mList.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		U::OPERATOR_SORT::invoke (DEREF[this] ,ret ,0 ,ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const List &that) const {
		if (length () != that.length ())
			return FALSE ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		INDEX jx = that.ibegin () ;
		INDEX jy = that.iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			if (jx == jy)
				break ;
			if (get (ix) != that.get (jx))
				break ;
			ix = inext (ix) ;
			jx = that.inext (jx) ;
		}
		if (ix != iy)
			return FALSE ;
		if (jx != jy)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const List &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const List &that) const {
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

	void add (const REMOVE_CVR_TYPE<ITEM> &item) {
		INDEX ix = mList.alloc (_MOVE_ (item)) ;
		mList[ix].mLeft = mLast ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = ix ;
		mLast = ix ;
	}

	inline List &operator<< (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		INDEX ix = mList.alloc (_MOVE_ (item)) ;
		mList[ix].mLeft = mLast ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = ix ;
		mLast = ix ;
	}

	inline List &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mList.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i]) ;
		}
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		mFirst = mList[ix].mRight ;
		auto &r1x = _SWITCH_ (
			(mFirst != VAR_NONE) ? mList[mFirst].mLeft :
			mLast) ;
		r1x = VAR_NONE ;
		mList.free (ix) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		item = _MOVE_ (mList[ix].mItem) ;
		mFirst = mList[ix].mRight ;
		auto &r1x = _SWITCH_ (
			(mFirst != VAR_NONE) ? mList[mFirst].mLeft :
			mLast) ;
		r1x = VAR_NONE ;
		mList.free (ix) ;
	}

	inline List &operator>> (ITEM &item) {
		take (item) ;
		return DEREF[this] ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mFirst ;
	}

	INDEX tail () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mLast ;
	}

	INDEX insert () side_effects {
		INDEX ret = mList.alloc () ;
		mList[ret].mLeft = mLast ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = ret ;
		mLast = ret ;
		return _MOVE_ (ret) ;
	}

	INDEX insert_before (const INDEX &index) side_effects {
		auto &r1x = _SWITCH_ (
			(index != VAR_NONE) ? mList[index].mLeft :
			mLast) ;
		INDEX ret = mList.alloc () ;
		mList[ret].mLeft = r1x ;
		mList[ret].mRight = index ;
		auto &r2x = _SWITCH_ (
			(r1x != VAR_NONE) ? mList[r1x].mRight :
			mFirst) ;
		r2x = ret ;
		r1x = ret ;
		return _MOVE_ (ret) ;
	}

	INDEX insert_after (const INDEX &index) side_effects {
		auto &r1x = _SWITCH_ (
			(index != VAR_NONE) ? mList[index].mRight :
			mFirst) ;
		INDEX ret = mList.alloc () ;
		mList[ret].mLeft = r1x ;
		mList[ret].mRight = index ;
		auto &r2x = _SWITCH_ (
			(r1x != VAR_NONE) ? mList[r1x].mLeft :
			mLast) ;
		r2x = ret ;
		r1x = ret ;
		return _MOVE_ (ret) ;
	}

	void push (const REMOVE_CVR_TYPE<ITEM> &item) {
		INDEX ix = mList.alloc (_MOVE_ (item)) ;
		mList[ix].mRight = mFirst ;
		auto &r1x = _SWITCH_ (
			(mFirst != VAR_NONE) ? mList[mFirst].mLeft :
			mLast) ;
		r1x = ix ;
		mFirst = ix ;
	}

	void push (REMOVE_CVR_TYPE<ITEM> &&item) {
		INDEX ix = mList.alloc (_MOVE_ (item)) ;
		mList[ix].mRight = mFirst ;
		auto &r1x = _SWITCH_ (
			(mFirst != VAR_NONE) ? mList[mFirst].mLeft :
			mLast) ;
		r1x = ix ;
		mFirst = ix ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mLast ;
		mLast = mList[ix].mLeft ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = VAR_NONE ;
		mList.free (ix) ;
	}

	void eswap (const INDEX &index1 ,const INDEX &index2) {
		if (index1 == index2)
			return ;
		auto &r1x = prev_next (index1) ;
		auto &r2x = prev_next (index2) ;
		auto &r3x = next_prev (index1) ;
		auto &r4x = next_prev (index2) ;
		r1x = index2 ;
		r2x = index1 ;
		r3x = index2 ;
		r4x = index1 ;
		_SWAP_ (mList[index1].mLeft ,mList[index2].mLeft) ;
		_SWAP_ (mList[index1].mRight ,mList[index2].mRight) ;
	}

	void splice_before (const INDEX &index ,const INDEX &last) {
		prev_next (last) = mList[last].mRight ;
		next_prev (last) = mList[last].mLeft ;
		auto &r1x = _SWITCH_ (
			(index != VAR_NONE) ? mList[index].mLeft :
			mLast) ;
		mList[last].mLeft = r1x ;
		mList[last].mRight = index ;
		auto &r2x = _SWITCH_ (
			(r1x != VAR_NONE) ? mList[r1x].mRight :
			mFirst) ;
		r2x = last ;
		r1x = last ;
	}

	void splice_after (const INDEX &index ,const INDEX &last) {
		prev_next (last) = mList[last].mRight ;
		next_prev (last) = mList[last].mLeft ;
		auto &r1x = _SWITCH_ (
			(index != VAR_NONE) ? mList[index].mRight :
			mFirst) ;
		mList[last].mLeft = index ;
		mList[last].mRight = r1x ;
		auto &r2x = _SWITCH_ (
			(r1x != VAR_NONE) ? mList[r1x].mLeft :
			mLast) ;
		r2x = last ;
		r1x = last ;
	}

	void remove (const INDEX &index) {
		prev_next (index) = mList[index].mRight ;
		next_prev (index) = mList[index].mLeft ;
		mList.free (index) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			if (get (i) == item)
				return i ;
		}
		return VAR_NONE ;
	}

	void sort (const Array<INDEX> &order) {
		_DEBUG_ASSERT_ (order.length () == length ()) ;
		if (order.length () < 2)
			return ;
		for (auto &&i : _RANGE_ (0 ,1)) {
			mList[order[i]].mLeft = VAR_NONE ;
			mList[order[i]].mRight = order[i + 1] ;
			mFirst = order[i] ;
		}
		for (auto &&i : _RANGE_ (1 ,order.length () - 1)) {
			mList[order[i]].mLeft = order[i - 1] ;
			mList[order[i]].mRight = order[i + 1] ;
		}
		for (auto &&i : _RANGE_ (order.length () - 1 ,order.length ())) {
			mList[order[i]].mLeft = order[i - 1] ;
			mList[order[i]].mRight = VAR_NONE ;
			mLast = order[i] ;
		}
	}

private:
	explicit List (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mList (len) {}

private:
	INDEX &prev_next (const INDEX &curr) leftvalue {
		if (mList[curr].mLeft == VAR_NONE)
			return mFirst ;
		return mList[mList[curr].mLeft].mRight ;
	}

	INDEX &next_prev (const INDEX &curr) leftvalue {
		if (mList[curr].mRight == VAR_NONE)
			return mLast ;
		return mList[mList[curr].mRight].mLeft ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class SoftList ;

template <class ITEM ,class SIZE>
class SoftList {
private:
	class Node {
	private:
		friend SoftList ;
		ITEM mItem ;
		INDEX mSeq ;

	public:
		template <class... _ARGS>
		explicit Node (_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS>::null ,initval)...) ,mSeq (VAR_NONE) {}
	} ;

	struct TREE_NODE {
		INDEX mIndex ;
		LENGTH mCount ;
	} ;

private:
	Allocator<Node ,SIZE> mList ;
	Buffer<TREE_NODE ,SIZE> mHead ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit SoftList () {
		clear () ;
	}

	explicit SoftList (const LENGTH &len)
		:SoftList (ARGVP0 ,len) {
		clear () ;
	}

	implicit SoftList (const stl::initializer_list<ITEM> &that)
		: SoftList (that.size ()) {
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
		const auto r1x = TREE_NODE {VAR_NONE ,VAR_ZERO} ;
		BasicProc::mem_fill (mHead.self ,mHead.size () ,r1x) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	INDEX ibegin () const {
		if (mHead.size () == 0)
			return VAR_NONE ;
		for (auto &&i : _RANGE_ (mRead ,mWrite + 1))
			if (mHead[i].mIndex != VAR_NONE)
				return mHead[i].mIndex ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		for (auto &&i : _RANGE_ (mList[index].mSeq + 1 ,mWrite + 1))
			if (mHead[i].mIndex != VAR_NONE)
				return mHead[i].mIndex ;
		return VAR_NONE ;
	}

	ArrayIterator<SoftList> begin () leftvalue {
		return ArrayIterator<SoftList> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const SoftList> begin () const leftvalue {
		return ArrayIterator<const SoftList> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<SoftList> end () leftvalue {
		return ArrayIterator<SoftList> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const SoftList> end () const leftvalue {
		return ArrayIterator<const SoftList> (DEREF[this] ,iend ()) ;
	}

	ITEM &get (const INDEX &index) leftvalue {
		return mList[index].mItem ;
	}

	inline ITEM &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mList[index].mItem ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mList.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	INDEX access (const INDEX &pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		if (mWrite - mRead + 1 == mList.length ())
			return mHead[mRead + pos].mIndex ;
		if (mWrite - mRead == mList.length ())
			if (mHead[mWrite].mIndex == VAR_NONE)
				return mHead[mRead + pos].mIndex ;
		INDEX ix = access (pos ,mRead ,mWrite) ;
		return mHead[ix].mIndex ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		U::OPERATOR_SORT::invoke (DEREF[this] ,ret ,0 ,ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const SoftList &that) const {
		if (length () != that.length ())
			return FALSE ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		INDEX jx = that.ibegin () ;
		INDEX jy = that.iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			if (jx == jy)
				break ;
			if (get (ix) != that.get (jx))
				break ;
			ix = inext (ix) ;
			jx = that.inext (jx) ;
		}
		if (ix != iy)
			return FALSE ;
		if (jx != jy)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const SoftList &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const SoftList &that) const {
		return !equal (that) ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item) {
		INDEX ix = mList.alloc (_MOVE_ (item)) ;
		update_resize (ix) ;
		update_compress_left (mWrite ,ix) ;
	}

	inline SoftList &operator<< (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		INDEX ix = mList.alloc (_MOVE_ (item)) ;
		update_resize (ix) ;
		update_compress_left (mWrite ,ix) ;
	}

	inline SoftList &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mList.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i]) ;
		}
	}

	INDEX insert () side_effects {
		INDEX ret = mList.alloc () ;
		update_resize (ret) ;
		update_compress_left (mWrite ,ret) ;
		return _MOVE_ (ret) ;
	}

	INDEX insert_before (const INDEX &index) side_effects {
		INDEX ret = mList.alloc () ;
		update_resize (ret) ;
		const auto r1x = _CALL_ ([&] () {
			if (index != VAR_NONE)
				return mList[index].mSeq ;
			return mWrite ;
		}) ;
		update_compress_left (r1x ,ret) ;
		return _MOVE_ (ret) ;
	}

	INDEX insert_after (const INDEX &index) side_effects {
		INDEX ret = mList.alloc () ;
		update_resize (ret) ;
		const auto r1x = _CALL_ ([&] () {
			if (index != VAR_NONE)
				return mList[index].mSeq + 1 ;
			return mRead ;
		}) ;
		update_compress_left (r1x ,ret) ;
		return _MOVE_ (ret) ;
	}

	void eswap (const INDEX &index1 ,const INDEX &index2) {
		if (index1 == index2)
			return ;
		sequence_rewrite (mList[index1].mSeq ,index2) ;
		sequence_rewrite (mList[index2].mSeq ,index1) ;
	}

	void splice_before (const INDEX &index ,const INDEX &last) {
		sequence_remove (mList[last].mSeq) ;
		const auto r1x = _CALL_ ([&] () {
			if (index != VAR_NONE)
				return mList[index].mSeq ;
			return mWrite ;
		}) ;
		update_compress_left (r1x ,last) ;
	}

	void splice_after (const INDEX &index ,const INDEX &last) {
		sequence_remove (mList[last].mSeq) ;
		const auto r1x = _CALL_ ([&] () {
			if (index != VAR_NONE)
				return mList[index].mSeq + 1 ;
			return mRead ;
		}) ;
		update_compress_left (r1x ,last) ;
	}

	void remove (const INDEX &index) {
		sequence_remove (mList[index].mSeq) ;
		mList.free (index) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			if (get (i) == item)
				return i ;
		}
		return VAR_NONE ;
	}

	void sort (const Array<INDEX> &order) {
		_DEBUG_ASSERT_ (order.length () == length ()) ;
		if (order.length () < 2)
			return ;
		for (auto &&i : _RANGE_ (0 ,order.length ()))
			sequence_rewrite (i ,order[i]) ;
		for (auto &&i : _RANGE_ (order.length () ,mHead.size ()))
			sequence_remove (i) ;
		mRead = 0 ;
		mWrite = order.length () - 1 ;
	}

private:
	explicit SoftList (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mList (len) ,mHead (len) {}

private:
	INDEX access (const INDEX &pos ,const INDEX &seg_a ,const LENGTH &seg_b) const {
		INDEX ret = VAR_NONE ;
		INDEX ix = seg_a ;
		INDEX iy = seg_b ;
		while (TRUE) {
			if (ix > iy)
				break ;
			ret = ix + (iy - ix) / 2 ;
			INDEX jx = position_before (ret) ;
			if (jx == pos)
				if (mHead[ret].mIndex != VAR_NONE)
					break ;
			auto fax = TRUE ;
			if switch_once (fax) {
				if (!(jx < pos))
					discard ;
				ix = ret + 1 ;
			}
			if switch_once (fax) {
				iy = ret - 1 ;
			}
		}
		_DEBUG_ASSERT_ (ret != VAR_NONE) ;
		return _MOVE_ (ret) ;
	}

	INDEX position_before (const INDEX &curr) const {
		INDEX ret = 0 ;
		INDEX ix = curr ;
		while (TRUE) {
			if (ix < 0)
				break ;
			ret += mHead[ix].mCount ;
			ix -= (ix + 1) & -(ix + 1) ;
		}
		ret-- ;
		return _MOVE_ (ret) ;
	}

	void update_resize (const INDEX &curr) {
		if (mHead.size () == mList.size ())
			return ;
		auto tmp = mHead.expand (mList.size ()) ;
		const auto r1x = TREE_NODE {VAR_NONE ,VAR_ZERO} ;
		BasicProc::mem_fill (tmp.self ,tmp.size () ,r1x) ;
		mHead.swap (tmp) ;
		for (auto &&i : _RANGE_ (0 ,mList.size ())) {
			if (i == curr)
				continue ;
			if (!mList.used (i))
				continue ;
			sequence_rewrite (mList[i].mSeq ,i) ;
		}
	}

	void update_compress_left (const INDEX &curr ,const INDEX &last) {
		auto fax = TRUE ;
		const auto r1x = mHead.size () - 1 ;
		if switch_once (fax) {
			if (!(mHead[curr].mIndex == VAR_NONE))
				discard ;
			sequence_rewrite (curr ,last) ;
			const auto r2x = curr + 1 ;
			mWrite = _MIN_ (r2x ,r1x) ;
		}
		if switch_once (fax) {
			INDEX ix = curr + 1 ;
			if (!(ix < mHead.size ()))
				discard ;
			if (!(mHead[ix].mIndex == VAR_NONE))
				discard ;
			sequence_rewrite (ix ,last) ;
			const auto r3x = ix + 1 ;
			mWrite = _MIN_ (r3x ,r1x) ;
		}
		if switch_once (fax) {
			update_compress_left_force (curr ,last) ;
		}
	}

	void update_compress_left_force (const INDEX &curr ,const INDEX &last) {
		INDEX ix = curr ;
		INDEX iy = last ;
		for (auto &&i : _RANGE_ (0 ,mList.length ())) {
			while (TRUE) {
				if (mRead == ix)
					break ;
				if (mHead[mRead].mIndex != VAR_NONE)
					break ;
				mRead++ ;
			}
			const auto r1x = mHead[i].mIndex ;
			auto fax = TRUE ;
			if switch_once (fax) {
				if (!(mRead == ix))
					discard ;
				if (!(r1x == VAR_NONE))
					discard ;
				sequence_rewrite (i ,iy) ;
				iy = r1x ;
				ix = VAR_NONE ;
			}
			if switch_once (fax) {
				if (!(mRead == ix))
					discard ;
				if (!(r1x != VAR_NONE))
					discard ;
				sequence_rewrite (i ,iy) ;
				iy = r1x ;
				ix++ ;
			}
			if switch_once (fax) {
				if (!(mRead != i))
					discard ;
				sequence_rewrite (i ,mHead[mRead].mIndex) ;
				sequence_remove (mRead) ;
			}
			mRead++ ;
		}
		mRead = 0 ;
		const auto r2x = mList.length () ;
		const auto r3x = mHead.size () - 1 ;
		mWrite = _MIN_ (r2x ,r3x) ;
	}

	void sequence_rewrite (const INDEX &curr ,const INDEX &index) {
		_DEBUG_ASSERT_ (index != VAR_NONE) ;
		INDEX ix = curr ;
		const auto r1x = mHead[curr].mIndex ;
		mHead[ix].mIndex = index ;
		mList[index].mSeq = ix ;
		if (r1x != VAR_NONE)
			return ;
		while (TRUE) {
			if (ix >= mHead.size ())
				break ;
			mHead[ix].mCount++ ;
			ix += (ix + 1) & -(ix + 1) ;
		}
	}

	void sequence_remove (const INDEX &curr) {
		INDEX ix = curr ;
		mHead[ix].mIndex = VAR_NONE ;
		while (TRUE) {
			if (ix >= mHead.size ())
				break ;
			mHead[ix].mCount-- ;
			ix += (ix + 1) & -(ix + 1) ;
		}
	}
} ;

namespace U {
template <class UNIT>
struct CONSTEXPR_CEIL8_SIZE_SWITCH {
	imports constexpr UNIT case1 (const UNIT &len) {
		return len ;
	}

	imports constexpr UNIT case2 (const UNIT &len) {
		return (len + 7) / 8 ;
	}
} ;

struct CONSTEXPR_CEIL8_SIZE {
	imports constexpr LENGTH invoke (const LENGTH &len) {
		return _SWITCH_ (
			(len <= 0) ? CONSTEXPR_CEIL8_SIZE_SWITCH<LENGTH>::case1 :
			CONSTEXPR_CEIL8_SIZE_SWITCH<LENGTH>::case2)
			(len) ;
	}
} ;
} ;

template <class SIZE = SAUTO>
class BitSet ;

template <class SIZE>
class BitSet {
private:
	struct Private {
		template <class>
		class Bit ;
	} ;

private:
	Buffer<BYTE ,ARGC<(U::CONSTEXPR_CEIL8_SIZE::invoke (SIZE::value))>> mSet ;
	LENGTH mWidth ;

public:
	implicit BitSet ()
		:BitSet (ARGVP0) {
		clear () ;
	}

	explicit BitSet (const LENGTH &len)
		:BitSet (ARGVP0 ,U::CONSTEXPR_CEIL8_SIZE::invoke (len) ,forward_width (len)) {
		clear () ;
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
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			ret += M_LENGTH.mP1[mSet[i]] ;
		if switch_once (TRUE) {
			if (mWidth % 8 == 0)
				discard ;
			const auto r1x = BYTE (BYTE (0X01) << (mWidth % 8)) ;
			const auto r2x = BYTE (mSet[mWidth / 8] & ~BYTE (INDEX (r1x) - 1)) ;
			ret -= M_LENGTH.mP1[INDEX (r2x)] ;
		}
		return _MOVE_ (ret) ;
	}

	void clear () {
		fill (BYTE (0X00)) ;
	}

	INDEX ibegin () const {
		for (auto &&i : _RANGE_ (0 ,size ()))
			if (get (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		for (auto &&i : _RANGE_ (index + 1 ,size ()))
			if (get (i))
				return i ;
		return VAR_NONE ;
	}

	ArrayIterator<BitSet> begin () leftvalue {
		return ArrayIterator<BitSet> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const BitSet> begin () const leftvalue {
		return ArrayIterator<const BitSet> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<BitSet> end () leftvalue {
		return ArrayIterator<BitSet> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const BitSet> end () const leftvalue {
		return ArrayIterator<const BitSet> (DEREF[this] ,iend ()) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<BitSet>>>
	_RET get (const INDEX &index) leftvalue {
		struct Dependent ;
		using Bit = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<BitSet> ;
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<BitSet>>>
	inline _RET operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<const BitSet>>>
	_RET get (const INDEX &index) const leftvalue {
		struct Dependent ;
		using Bit = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<const BitSet> ;
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<const BitSet>>>
	inline _RET operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const DEF<typename Private::template Bit<BitSet>> &item) const {
		struct Dependent ;
		using Bit = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<BitSet> ;
		auto &r1x = _XVALUE_ (ARGV<Bit>::null ,item) ;
		if (this != DEPTR[r1x.mBase])
			return VAR_NONE ;
		return r1x ;
	}

	INDEX at (const DEF<typename Private::template Bit<const BitSet>> &item) const {
		struct Dependent ;
		using Bit = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<const BitSet> ;
		auto &r1x = _XVALUE_ (ARGV<Bit>::null ,item) ;
		if (this != DEPTR[r1x.mBase])
			return VAR_NONE ;
		return r1x ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		if (ix < 0)
			return TRUE ;
		for (auto &&i : _RANGE_ (0 ,ix))
			if (mSet[i] != that.mSet[i])
				return FALSE ;
		const auto r1x = BYTE (mSet[ix] & (mWidth % 8 - 1)) ;
		const auto r2x = BYTE (that.mSet[ix] & (mWidth % 8 - 1)) ;
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
		const auto r1x = BasicProc::mem_compr (mSet ,that.mSet ,ix) ;
		if (r1x != 0)
			return r1x ;
		const auto r2x = BYTE (mSet[ix] & (mWidth % 8 - 1)) ;
		const auto r3x = BYTE (that.mSet[ix] & (mWidth % 8 - 1)) ;
		return U::OPERATOR_COMPR::invoke (r2x ,r3x) ;
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
		return DEREF[this] ;
	}

	void erase (const INDEX &item) {
		_DYNAMIC_ASSERT_ (item >= 0 && item < mWidth) ;
		get (item) = FALSE ;
	}

	BitSet band (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			ret.mSet[i] = mSet[i] & that.mSet[i] ;
		return _MOVE_ (ret) ;
	}

	inline BitSet operator& (const BitSet &that) const {
		return band (that) ;
	}

	void bandto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			mSet[i] &= that.mSet[i] ;
	}

	inline BitSet &operator&= (const BitSet &that) {
		bandto (that) ;
		return DEREF[this] ;
	}

	BitSet bor (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			ret.mSet[i] = mSet[i] | that.mSet[i] ;
		return _MOVE_ (ret) ;
	}

	inline BitSet operator| (const BitSet &that) const {
		return bor (that) ;
	}

	void borto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			mSet[i] |= that.mSet[i] ;
	}

	inline BitSet &operator|= (const BitSet &that) {
		borto (that) ;
		return DEREF[this] ;
	}

	BitSet bxor (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			ret.mSet[i] = mSet[i] ^ that.mSet[i] ;
		return _MOVE_ (ret) ;
	}

	inline BitSet operator^ (const BitSet &that) const {
		return bxor (that) ;
	}

	void bxorto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			mSet[i] ^= that.mSet[i] ;
	}

	inline BitSet &operator^= (const BitSet &that) {
		bxorto (that) ;
		return DEREF[this] ;
	}

	BitSet bsub (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			ret.mSet[i] = mSet[i] & ~that.mSet[i] ;
		return _MOVE_ (ret) ;
	}

	inline BitSet operator- (const BitSet &that) const {
		return bsub (that) ;
	}

	void bsubto (const BitSet &that) {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			mSet[i] &= ~that.mSet[i] ;
	}

	inline BitSet &operator-= (const BitSet &that) {
		bsubto (that) ;
		return DEREF[this] ;
	}

	BitSet bnot () const {
		BitSet ret = BitSet (mWidth) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			ret.mSet[i] = ~mSet[i] ;
		return _MOVE_ (ret) ;
	}

	inline BitSet operator~ () const {
		return bnot () ;
	}

	void fill (const BYTE &item) {
		for (auto &&i : _RANGE_ (0 ,mSet.size ()))
			mSet[i] = item ;
	}

private:
	explicit BitSet (const DEF<decltype (ARGVP0)> &) {
		const auto r1x = LENGTH (SIZE::value) ;
		const auto r2x = VAR_ZERO ;
		mWidth = _MAX_ (r1x ,r2x) ;
	}

	explicit BitSet (const DEF<decltype (ARGVP0)> & ,const LENGTH &len ,const LENGTH &width)
		:mSet (len) ,mWidth (width) {}

private:
	imports LENGTH forward_width (const LENGTH &width) {
		_DEBUG_ASSERT_ (width >= 0 && width < VAR32_MAX) ;
		return width ;
	}
} ;

template <class SIZE>
template <class BASE>
class BitSet<SIZE>::Private::Bit
	:private Proxy {
private:
	friend BitSet ;
	BASE &mBase ;
	INDEX mIndex ;

public:
	implicit Bit () = delete ;

	explicit Bit (BASE &base ,const INDEX &index)
		: mBase (base) ,mIndex (index) {}

	inline implicit operator BOOL () rightvalue {
		const auto r1x = BYTE (BYTE (0X01) << (mIndex % 8)) ;
		const auto r2x = BYTE (mBase.mSet[mIndex / 8] & r1x) ;
		if (r2x == 0)
			return FALSE ;
		return TRUE ;
	}

#ifdef __CSC_CONFIG_VAR32__
	inline implicit operator VAR32 () const leftvalue {
		return VAR32 (mIndex) ;
	}

	inline explicit operator VAR64 () const leftvalue {
		return VAR64 (mIndex) ;
	}
#endif

#ifdef __CSC_CONFIG_VAR64__
	inline explicit operator VAR32 () const leftvalue {
		return VAR32 (mIndex) ;
	}

	inline implicit operator VAR64 () const leftvalue {
		return VAR64 (mIndex) ;
	}
#endif

	inline void operator= (const BOOL &that) rightvalue {
		const auto r1x = BYTE (BYTE (0X01) << (mIndex % 8)) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!that)
				discard ;
			mBase.mSet[mIndex / 8] |= r1x ;
		}
		if switch_once (fax) {
			mBase.mSet[mIndex / 8] &= ~r1x ;
		}
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Set ;

template <class ITEM ,class SIZE>
class Set {
private:
	class Node {
	private:
		friend Set ;
		ITEM mItem ;
		INDEX mMap ;
		BOOL mRed ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;

	public:
		template <class... _ARGS>
		explicit Node (_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS>::null ,initval)...) ,mMap (VAR_NONE) ,mRed (FALSE) ,mUp (VAR_NONE) ,mLeft (VAR_NONE) ,mRight (VAR_NONE) {}
	} ;

	struct Private {
		template <class>
		class Pair ;
	} ;

private:
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	implicit Set () {
		clear () ;
	}

	explicit Set (const LENGTH &len)
		:Set (ARGVP0 ,len) {
		clear () ;
	}

	implicit Set (const stl::initializer_list<ITEM> &that)
		: Set (that.size ()) {
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
		mTop = VAR_NONE ;
	}

	INDEX ibegin () const {
		for (auto &&i : _RANGE_ (0 ,size ()))
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		for (auto &&i : _RANGE_ (index + 1 ,size ()))
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	ArrayIterator<Set> begin () leftvalue {
		return ArrayIterator<Set> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const Set> begin () const leftvalue {
		return ArrayIterator<const Set> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<Set> end () leftvalue {
		return ArrayIterator<Set> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const Set> end () const leftvalue {
		return ArrayIterator<const Set> (DEREF[this] ,iend ()) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<Set>>>
	_RET get (const INDEX &index) leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<Set> ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<Set>>>
	inline _RET operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const Set>>>
	_RET get (const INDEX &index) const leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const Set> ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const Set>>>
	inline _RET operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mSet.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	INDEX at (const DEF<typename Private::template Pair<Set>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<Set> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	INDEX at (const DEF<typename Private::template Pair<const Set>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const Set> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		compute_esort (mRoot ,ret ,iw) ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Set &operator<< (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (_MOVE_ (item)) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mRed = TRUE ;
			update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Set &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (_MOVE_ (item)) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mRed = TRUE ;
			update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i].key ,val[i].sid) ;
		}
	}

	INDEX head () const {
		for (INDEX i = mRoot ,it ; i != VAR_NONE ; i = it) {
			it = mSet[i].mLeft ;
			if (it == VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX tail () const {
		for (INDEX i = mRoot ,it ; i != VAR_NONE ; i = it) {
			it = mSet[i].mRight ;
			if (it == VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX insert (const REMOVE_CVR_TYPE<ITEM> &item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CVR_TYPE<ITEM> &&item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	void remove (const INDEX &index) {
		if switch_once (TRUE) {
			if (mSet[index].mLeft == VAR_NONE)
				discard ;
			if (mSet[index].mRight == VAR_NONE)
				discard ;
			eswap (index ,find_successor (index)) ;
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

	INDEX find (const ITEM &item) const {
		INDEX ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = BOOL (item < mSet[ret].mItem) ;
			if (!r1x)
				if (!(item > mSet[ret].mItem))
					break ;
			auto &r2x = _SWITCH_ (
				r1x ? mSet[ret].mLeft :
				mSet[ret].mRight) ;
			ret = r2x ;
		}
		return _MOVE_ (ret) ;
	}

	INDEX map (const ITEM &item) const {
		INDEX ix = find (item) ;
		if (ix == VAR_NONE)
			return VAR_NONE ;
		return mSet[ix].mMap ;
	}

	void erase (const ITEM &item) {
		INDEX ix = find (item) ;
		if (ix == VAR_NONE)
			return ;
		remove (ix) ;
	}

private:
	explicit Set (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mSet (len) {}

private:
	void update_emplace (const INDEX &curr ,const INDEX &last) {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(curr == VAR_NONE))
				discard ;
			mTop = last ;
		}
		if switch_once (fax) {
			if (!(curr != VAR_NONE))
				discard ;
			mSet[last].mUp = curr ;
			auto &r1x = _SWITCH_ (
				(mSet[last].mItem < mSet[curr].mItem) ? mSet[curr].mLeft :
				mSet[curr].mRight) ;
			update_emplace (r1x ,last) ;
			r1x = mTop ;
			mTop = curr ;
		}
	}

	void update_insert (const INDEX &curr) {
		INDEX ix = curr ;
		while (TRUE) {
			INDEX jx = mSet[ix].mUp ;
			if (jx == VAR_NONE)
				break ;
			if (!mSet[jx].mRed)
				break ;
			auto fax = TRUE ;
			if switch_once (fax) {
				if (!(jx == mSet[mSet[jx].mUp].mLeft))
					discard ;
				update_insert_left (ix) ;
			}
			if switch_once (fax) {
				if (!(jx == mSet[mSet[jx].mUp].mRight))
					discard ;
				update_insert_right (ix) ;
			}
			ix = mTop ;
		}
		mSet[mRoot].mRed = FALSE ;
	}

	void update_insert_left (const INDEX &curr) {
		INDEX ix = mSet[curr].mUp ;
		INDEX iy = mSet[ix].mUp ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mSet[iy].mRight != VAR_NONE))
				discard ;
			if (!mSet[mSet[iy].mRight].mRed)
				discard ;
			mSet[mSet[iy].mRight].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			mTop = iy ;
		}
		if switch_once (fax) {
			if (!(curr == mSet[ix].mRight))
				discard ;
			auto &r1x = mSet[iy].mLeft ;
			rotate_left (r1x) ;
			r1x = mTop ;
			mSet[curr].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2x = prev_next (iy) ;
			rotate_right (r2x) ;
			r2x = mTop ;
			mTop = ix ;
		}
		if switch_once (fax) {
			if (!(curr == mSet[ix].mLeft))
				discard ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r3x = prev_next (iy) ;
			rotate_right (r3x) ;
			r3x = mTop ;
			mTop = curr ;
		}
	}

	void update_insert_right (const INDEX &curr) {
		INDEX ix = mSet[curr].mUp ;
		INDEX iy = mSet[ix].mUp ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mSet[iy].mLeft != VAR_NONE))
				discard ;
			if (!mSet[mSet[iy].mLeft].mRed)
				discard ;
			mSet[mSet[iy].mLeft].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			mTop = iy ;
		}
		if switch_once (fax) {
			if (!(curr == mSet[ix].mLeft))
				discard ;
			auto &r1x = mSet[iy].mRight ;
			rotate_right (r1x) ;
			r1x = mTop ;
			mSet[curr].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2x = prev_next (iy) ;
			rotate_left (r2x) ;
			r2x = mTop ;
			mTop = ix ;
		}
		if switch_once (fax) {
			if (!(curr == mSet[ix].mRight))
				discard ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r3x = prev_next (iy) ;
			rotate_left (r3x) ;
			r3x = mTop ;
			mTop = curr ;
		}
	}

	void update_remove (const INDEX &curr ,const INDEX &last) {
		INDEX ix = curr ;
		INDEX iy = last ;
		while (TRUE) {
			if (iy == VAR_NONE)
				break ;
			if (ix != VAR_NONE)
				if (mSet[ix].mRed)
					break ;
			auto fax = TRUE ;
			if switch_once (fax) {
				if (!(ix == mSet[iy].mLeft))
					discard ;
				update_remove_left (ix ,iy) ;
			}
			if switch_once (fax) {
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

	void update_remove_left (const INDEX &curr ,const INDEX &last) {
		auto &r1x = mSet[last].mRight ;
		if switch_once (TRUE) {
			if (!mSet[r1x].mRed)
				discard ;
			mSet[r1x].mRed = FALSE ;
			mSet[last].mRed = TRUE ;
			auto &r2x = prev_next (last) ;
			rotate_left (r2x) ;
			r2x = mTop ;
		}
		const auto r3x = BOOL (mSet[r1x].mLeft != VAR_NONE && mSet[mSet[r1x].mLeft].mRed) ;
		const auto r4x = BOOL (mSet[r1x].mRight != VAR_NONE && mSet[mSet[r1x].mRight].mRed) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (r3x)
				discard ;
			if (r4x)
				discard ;
			mSet[r1x].mRed = TRUE ;
			mTop = last ;
		}
		if switch_once (fax) {
			if (r4x)
				discard ;
			mSet[mSet[r1x].mLeft].mRed = FALSE ;
			mSet[r1x].mRed = TRUE ;
			rotate_right (r1x) ;
			r1x = mTop ;
			mSet[r1x].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1x].mRight].mRed = FALSE ;
			auto &r5x = prev_next (last) ;
			rotate_left (r5x) ;
			r5x = mTop ;
			mTop = mRoot ;
		}
		if switch_once (fax) {
			mSet[r1x].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1x].mRight].mRed = FALSE ;
			auto &r6x = prev_next (last) ;
			rotate_left (r6x) ;
			r6x = mTop ;
			mTop = mRoot ;
		}
	}

	void update_remove_right (const INDEX &curr ,const INDEX &last) {
		auto &r1x = mSet[last].mLeft ;
		if switch_once (TRUE) {
			if (!mSet[r1x].mRed)
				discard ;
			mSet[r1x].mRed = FALSE ;
			mSet[last].mRed = TRUE ;
			auto &r2x = prev_next (last) ;
			rotate_right (r2x) ;
			r2x = mTop ;
		}
		const auto r3x = BOOL (mSet[r1x].mRight != VAR_NONE && mSet[mSet[r1x].mRight].mRed) ;
		const auto r4x = BOOL (mSet[r1x].mLeft != VAR_NONE && mSet[mSet[r1x].mLeft].mRed) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (r3x)
				discard ;
			if (r4x)
				discard ;
			mSet[r1x].mRed = TRUE ;
			mTop = last ;
		}
		if switch_once (fax) {
			if (r4x)
				discard ;
			mSet[mSet[r1x].mRight].mRed = FALSE ;
			mSet[r1x].mRed = TRUE ;
			rotate_left (r1x) ;
			r1x = mTop ;
			mSet[r1x].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1x].mLeft].mRed = FALSE ;
			auto &r5x = prev_next (last) ;
			rotate_right (r5x) ;
			r5x = mTop ;
			mTop = mRoot ;
		}
		if switch_once (fax) {
			mSet[r1x].mRed = mSet[last].mRed ;
			mSet[last].mRed = FALSE ;
			mSet[mSet[r1x].mLeft].mRed = FALSE ;
			auto &r6x = prev_next (last) ;
			rotate_right (r6x) ;
			r6x = mTop ;
			mTop = mRoot ;
		}
	}

	void rotate_left (const INDEX &curr) {
		INDEX ix = mSet[curr].mRight ;
		mSet[curr].mRight = mSet[ix].mLeft ;
		if (mSet[ix].mLeft != VAR_NONE)
			mSet[mSet[ix].mLeft].mUp = curr ;
		mSet[ix].mLeft = curr ;
		mSet[ix].mUp = mSet[curr].mUp ;
		mSet[curr].mUp = ix ;
		mTop = ix ;
	}

	void rotate_right (const INDEX &curr) {
		INDEX ix = mSet[curr].mLeft ;
		mSet[curr].mLeft = mSet[ix].mRight ;
		if (mSet[ix].mRight != VAR_NONE)
			mSet[mSet[ix].mRight].mUp = curr ;
		mSet[ix].mRight = curr ;
		mSet[ix].mUp = mSet[curr].mUp ;
		mSet[curr].mUp = ix ;
		mTop = ix ;
	}

	INDEX &prev_next (const INDEX &curr) leftvalue {
		if (mSet[curr].mUp == VAR_NONE)
			return mRoot ;
		if (mSet[mSet[curr].mUp].mLeft == curr)
			return mSet[mSet[curr].mUp].mLeft ;
		return mSet[mSet[curr].mUp].mRight ;
	}

	INDEX find_successor (const INDEX &index) const {
		for (INDEX i = mSet[index].mRight ,it ; i != VAR_NONE ; i = it) {
			it = mSet[i].mLeft ;
			if (it == VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	void eswap (const INDEX &index1 ,const INDEX &index2) {
		if (index1 == index2)
			return ;
		auto rax = INDEX () ;
		auto &r1x = prev_next (index2) ;
		auto &r2x = _SWITCH_ (
			(mSet[index2].mLeft != VAR_NONE) ? mSet[mSet[index2].mLeft].mUp :
			rax) ;
		auto &r3x = _SWITCH_ (
			(mSet[index2].mRight != VAR_NONE) ? mSet[mSet[index2].mRight].mUp :
			rax) ;
		auto &r4x = prev_next (index1) ;
		auto &r5x = _SWITCH_ (
			(mSet[index1].mLeft != VAR_NONE) ? mSet[mSet[index1].mLeft].mUp :
			rax) ;
		auto &r6x = _SWITCH_ (
			(mSet[index1].mRight != VAR_NONE) ? mSet[mSet[index1].mRight].mUp :
			rax) ;
		r1x = index1 ;
		r2x = index1 ;
		r3x = index1 ;
		r4x = index2 ;
		r5x = index2 ;
		r6x = index2 ;
		_SWAP_ (mSet[index1].mRed ,mSet[index2].mRed) ;
		_SWAP_ (mSet[index1].mUp ,mSet[index2].mUp) ;
		_SWAP_ (mSet[index1].mLeft ,mSet[index2].mLeft) ;
		_SWAP_ (mSet[index1].mRight ,mSet[index2].mRight) ;
	}

	void compute_esort (const INDEX &curr ,Array<INDEX> &out ,INDEX &out_i) const {
		if (curr == VAR_NONE)
			return ;
		INDEX iw = out_i ;
		compute_esort (mSet[curr].mLeft ,out ,iw) ;
		out[iw++] = curr ;
		compute_esort (mSet[curr].mRight ,out ,iw) ;
		out_i = iw ;
	}
} ;

template <class ITEM ,class SIZE>
template <class BASE>
class Set<ITEM ,SIZE>::Private::Pair
	:private Proxy {
public:
	const ITEM &key ;
	CAST_TRAITS_TYPE<INDEX ,BASE> &sid ;

public:
	implicit Pair () = delete ;

	explicit Pair (BASE &base ,const INDEX &index)
		: key (base.mSet[index].mItem) ,sid (base.mSet[index].mMap) {}

	inline implicit operator const ITEM & () rightvalue {
		return key ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class HashSet ;

template <class ITEM ,class SIZE>
class HashSet {
private:
	class Node {
	private:
		friend HashSet ;
		ITEM mItem ;
		INDEX mMap ;
		FLAG mHash ;
		INDEX mNext ;

	public:
		template <class... _ARGS>
		explicit Node (_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS>::null ,initval)...) ,mMap (VAR_NONE) ,mHash (0) ,mNext (VAR_NONE) {}
	} ;

	struct Private {
		template <class>
		class Pair ;
	} ;

private:
	Allocator<Node ,SIZE> mSet ;
	Buffer<INDEX ,SIZE> mHead ;
	INDEX mTop ;

public:
	implicit HashSet () {
		clear () ;
	}

	explicit HashSet (const LENGTH &len)
		:HashSet (ARGVP0 ,len) {
		clear () ;
	}

	implicit HashSet (const stl::initializer_list<ITEM> &that)
		: HashSet (that.size ()) {
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
		BasicProc::mem_fill (mHead.self ,mHead.size () ,VAR_NONE) ;
		mTop = VAR_NONE ;
	}

	INDEX ibegin () const {
		for (auto &&i : _RANGE_ (0 ,size ()))
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		for (auto &&i : _RANGE_ (index + 1 ,size ()))
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	ArrayIterator<HashSet> begin () leftvalue {
		return ArrayIterator<HashSet> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const HashSet> begin () const leftvalue {
		return ArrayIterator<const HashSet> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<HashSet> end () leftvalue {
		return ArrayIterator<HashSet> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const HashSet> end () const leftvalue {
		return ArrayIterator<const HashSet> (DEREF[this] ,iend ()) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<HashSet>>>
	_RET get (const INDEX &index) leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<HashSet> ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<HashSet>>>
	inline _RET operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const HashSet>>>
	_RET get (const INDEX &index) const leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const HashSet> ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const HashSet>>>
	inline _RET operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mSet.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	INDEX at (const DEF<typename Private::template Pair<HashSet>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<HashSet> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	INDEX at (const DEF<typename Private::template Pair<const HashSet>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const HashSet> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline HashSet &operator<< (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (_MOVE_ (item)) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mHash = U::OPERATOR_HASH::invoke (item) ;
			update_resize (ix) ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline HashSet &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (_MOVE_ (item)) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mHash = U::OPERATOR_HASH::invoke (item) ;
			update_resize (ix) ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i].key ,val[i].sid) ;
		}
	}

	INDEX insert (const REMOVE_CVR_TYPE<ITEM> &item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CVR_TYPE<ITEM> &&item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	void remove (const INDEX &index) {
		prev_next (index) = mSet[index].mNext ;
		mSet.free (index) ;
	}

	INDEX find (const ITEM &item) const {
		INDEX ret = VAR_NONE ;
		if switch_once (TRUE) {
			if (size () == 0)
				discard ;
			const auto r1x = U::OPERATOR_HASH::invoke (item) ;
			_DEBUG_ASSERT_ (r1x >= 0) ;
			ret = mHead[r1x % mHead.size ()] ;
			while (TRUE) {
				if (ret == VAR_NONE)
					break ;
				if (mSet[ret].mHash == r1x)
					if (mSet[ret].mItem == item)
						break ;
				ret = mSet[ret].mNext ;
			}
		}
		return _MOVE_ (ret) ;
	}

	INDEX map (const ITEM &item) const {
		INDEX ix = find (item) ;
		if (ix == VAR_NONE)
			return VAR_NONE ;
		return mSet[ix].mMap ;
	}

	void erase (const ITEM &item) {
		INDEX ix = find (item) ;
		if (ix == VAR_NONE)
			return ;
		remove (ix) ;
	}

private:
	explicit HashSet (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mSet (len) ,mHead (len) {}

private:
	void update_resize (const INDEX &curr) {
		if (mHead.size () == mSet.size ())
			return ;
		auto tmp = mHead.expand (mSet.size ()) ;
		BasicProc::mem_fill (tmp.self ,tmp.size () ,VAR_NONE) ;
		mHead.swap (tmp) ;
		for (auto &&i : _RANGE_ (0 ,mSet.size ())) {
			if (i == curr)
				continue ;
			if (!mSet.used (i))
				continue ;
			INDEX ix = mSet[i].mHash % mHead.size () ;
			mSet[i].mNext = mHead[ix] ;
			mHead[ix] = i ;
		}
	}

	void update_insert (const INDEX &curr) {
		INDEX ix = mSet[curr].mHash % mHead.size () ;
		mSet[curr].mNext = mHead[ix] ;
		mHead[ix] = curr ;
	}

	INDEX &prev_next (const INDEX &curr) leftvalue {
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
} ;

template <class ITEM ,class SIZE>
template <class BASE>
class HashSet<ITEM ,SIZE>::Private::Pair
	:private Proxy {
public:
	const ITEM &key ;
	CAST_TRAITS_TYPE<ITEM ,BASE> &sid ;

public:
	implicit Pair () = delete ;

	explicit Pair (BASE &base ,const INDEX &index)
		: key (base.mSet[index].mItem) ,sid (base.mSet[index].mMap) {}

	inline implicit operator const ITEM & () rightvalue {
		return key ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class SoftSet ;

template <class ITEM ,class SIZE>
class SoftSet {
private:
	class Node {
	private:
		friend SoftSet ;
		ITEM mItem ;
		INDEX mMap ;
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
		INDEX mNext ;

	public:
		template <class... _ARGS>
		explicit Node (_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS>::null ,initval)...) ,mMap (VAR_NONE) ,mWeight (0) ,mLeft (VAR_NONE) ,mRight (VAR_NONE) ,mNext (VAR_NONE) {}
	} ;

	struct HEAP_PACK {
		AutoRef<Allocator<Node ,SIZE>> mBuffer ;
	} ;

	struct Private {
		template <class>
		class Pair ;
	} ;

private:
	SharedRef<HEAP_PACK> mHeap ;
	PhanRef<Allocator<Node ,SIZE>> mSet ;
	LENGTH mLength ;
	INDEX mFirst ;
	INDEX mLast ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	implicit SoftSet () {
		mLength = 0 ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
		mRoot = VAR_NONE ;
		mTop = VAR_NONE ;
	}

	explicit SoftSet (const LENGTH &len) {
		mHeap = SharedRef<HEAP_PACK>::make () ;
		mHeap->mBuffer = AutoRef<Allocator<Node ,SIZE>>::make (len) ;
		mSet = PhanRef<Allocator<Node ,SIZE>>::make (mHeap->mBuffer.self) ;
		mLength = 0 ;
		mFirst = VAR_NONE ;
		mLast = VAR_NONE ;
		mRoot = VAR_NONE ;
		mTop = VAR_NONE ;
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

	SoftSet share () side_effects {
		SoftSet ret ;
		ret.mHeap = mHeap ;
		ret.mSet = PhanRef<Allocator<Node ,SIZE>>::make (ret.mHeap->mBuffer.self) ;
		ret.mLength = 0 ;
		ret.mFirst = VAR_NONE ;
		ret.mLast = VAR_NONE ;
		ret.mRoot = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	INDEX ibegin () const {
		if (!mHeap.exist ())
			return VAR_NONE ;
		return mFirst ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		return mSet.self[index].mNext ;
	}

	ArrayIterator<SoftSet> begin () leftvalue {
		return ArrayIterator<SoftSet> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<const SoftSet> begin () const leftvalue {
		return ArrayIterator<const SoftSet> (DEREF[this] ,ibegin ()) ;
	}

	ArrayIterator<SoftSet> end () leftvalue {
		return ArrayIterator<SoftSet> (DEREF[this] ,iend ()) ;
	}

	ArrayIterator<const SoftSet> end () const leftvalue {
		return ArrayIterator<const SoftSet> (DEREF[this] ,iend ()) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<SoftSet>>>
	_RET get (const INDEX &index) leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<SoftSet> ;
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<SoftSet>>>
	inline _RET operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const SoftSet>>>
	_RET get (const INDEX &index) const leftvalue {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const SoftSet> ;
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		return Pair (DEREF[this] ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Pair<const SoftSet>>>
	inline _RET operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mSet.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	INDEX at (const DEF<typename Private::template Pair<SoftSet>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<SoftSet> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	INDEX at (const DEF<typename Private::template Pair<const SoftSet>> &item) const {
		struct Dependent ;
		using Pair = typename DEPENDENT_TYPE<Private ,Dependent>::template Pair<const SoftSet> ;
		auto &r1x = _XVALUE_ (ARGV<Pair>::null ,item) ;
		return at (r1x.key) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,it ,ie = iend () ; i != ie ; i = it) {
			it = inext (i) ;
			ret[iw++] = i ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		compute_esort (mRoot ,ret ,iw) ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline SoftSet &operator<< (const REMOVE_CVR_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CVR_TYPE<ITEM> &item ,const INDEX &map_) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (_MOVE_ (item)) ;
			mSet.self[ix].mMap = map_ ;
			mSet.self[ix].mWeight = 1 ;
			auto &r1x = _SWITCH_ (
				(mLast != VAR_NONE) ? mSet.self[mLast].mNext :
				mFirst) ;
			r1x = ix ;
			mLast = ix ;
			mLength++ ;
			update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline SoftSet &operator<< (REMOVE_CVR_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CVR_TYPE<ITEM> &&item ,const INDEX &map_) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (_MOVE_ (item)) ;
			mSet.self[ix].mMap = map_ ;
			mSet.self[ix].mWeight = 1 ;
			auto &r1x = _SWITCH_ (
				(mLast != VAR_NONE) ? mSet.self[mLast].mNext :
				mFirst) ;
			r1x = ix ;
			mLast = ix ;
			mLength++ ;
			update_insert (mRoot) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		mSet->reserve (val.length ()) ;
		for (INDEX i = val.ibegin () ,it ,ie = val.iend () ; i != ie ; i = it) {
			it = val.inext (i) ;
			add (val[i].key ,val[i].sid) ;
		}
	}

	INDEX head () const {
		if (!mHeap.exist ())
			return VAR_NONE ;
		for (INDEX i = mRoot ,it ; i != VAR_NONE ; i = it) {
			it = mSet.self[i].mLeft ;
			if (it == VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX tail () const {
		if (!mHeap.exist ())
			return VAR_NONE ;
		for (INDEX i = mRoot ,it ; i != VAR_NONE ; i = it) {
			it = mSet.self[i].mRight ;
			if (it == VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX insert (const REMOVE_CVR_TYPE<ITEM> &item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CVR_TYPE<ITEM> &&item) side_effects {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX find (const ITEM &item) const {
		INDEX ret = VAR_NONE ;
		if (mHeap.exist ())
			ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = BOOL (item < mSet.self[ret].mItem) ;
			if (!r1x)
				if (!(item > mSet.self[ret].mItem))
					break ;
			auto &r2x = _SWITCH_ (
				r1x ? mSet.self[ret].mLeft :
				mSet.self[ret].mRight) ;
			ret = r2x ;
		}
		return _MOVE_ (ret) ;
	}

	INDEX map (const ITEM &item) const {
		INDEX ix = find (item) ;
		if (ix == VAR_NONE)
			return VAR_NONE ;
		return mSet.self[ix].mMap ;
	}

	void clean () {
		if (!mHeap.exist ())
			return ;
		mSet->clean () ;
	}

private:
	void update_insert (const INDEX &curr) {
		INDEX ix = curr ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(ix != VAR_NONE))
				discard ;
			mSet.self[ix].mWeight++ ;
			const auto r1x = BOOL (mSet.self[mLast].mItem < mSet.self[ix].mItem) ;
			auto fbx = TRUE ;
			if switch_once (fbx) {
				if (!r1x)
					discard ;
				update_insert (mSet.self[ix].mLeft) ;
				mSet.self[ix].mLeft = mTop ;
				update_insert_left (ix) ;
			}
			if switch_once (fbx) {
				if (r1x)
					discard ;
				update_insert (mSet.self[ix].mRight) ;
				mSet.self[ix].mRight = mTop ;
				update_insert_right (ix) ;
			}
			ix = mTop ;
			mTop = ix ;
		}
		if switch_once (fax) {
			mTop = mLast ;
		}
	}

	void update_insert_left (const INDEX &curr) {
		INDEX ix = curr ;
		mTop = ix ;
		if (mSet.self[ix].mLeft == VAR_NONE)
			return ;
		const auto r1x = node_weight (mSet.self[ix].mRight) ;
		const auto r2x = node_weight (mSet.self[mSet.self[ix].mLeft].mLeft) ;
		const auto r3x = node_weight (mSet.self[mSet.self[ix].mLeft].mRight) ;
		mTop = ix ;
		if (r1x >= _MAX_ (r2x ,r3x))
			return ;
		if switch_once (TRUE) {
			if (r1x < r2x)
				discard ;
			auto &r4x = mSet.self[ix].mLeft ;
			rotate_left (r4x) ;
			r4x = mTop ;
		}
		rotate_right (ix) ;
		ix = mTop ;
		auto &r5x = mSet.self[ix].mLeft ;
		update_insert_left (r5x) ;
		r5x = mTop ;
		auto &r6x = mSet.self[ix].mRight ;
		update_insert_right (r6x) ;
		r6x = mTop ;
		update_insert_left (ix) ;
		ix = mTop ;
		update_insert_right (ix) ;
		ix = mTop ;
		mTop = ix ;
	}

	void update_insert_right (const INDEX &curr) {
		INDEX ix = curr ;
		mTop = ix ;
		if (mSet.self[ix].mRight == VAR_NONE)
			return ;
		const auto r1x = node_weight (mSet.self[ix].mLeft) ;
		const auto r2x = node_weight (mSet.self[mSet.self[ix].mRight].mRight) ;
		const auto r3x = node_weight (mSet.self[mSet.self[ix].mRight].mLeft) ;
		mTop = ix ;
		if (r1x >= _MAX_ (r2x ,r3x))
			return ;
		if switch_once (TRUE) {
			if (r1x < r2x)
				discard ;
			auto &r4x = mSet.self[ix].mRight ;
			rotate_right (r4x) ;
			r4x = mTop ;
		}
		rotate_left (ix) ;
		ix = mTop ;
		auto &r5x = mSet.self[ix].mLeft ;
		update_insert_left (r5x) ;
		r5x = mTop ;
		auto &r6x = mSet.self[ix].mRight ;
		update_insert_right (r6x) ;
		r6x = mTop ;
		update_insert_left (ix) ;
		ix = mTop ;
		update_insert_right (ix) ;
		ix = mTop ;
		mTop = ix ;
	}

	void rotate_left (const INDEX &curr) {
		INDEX ix = mSet.self[curr].mRight ;
		mSet.self[curr].mRight = mSet.self[ix].mLeft ;
		mSet.self[ix].mLeft = curr ;
		mSet.self[ix].mWeight = mSet.self[curr].mWeight ;
		const auto r1x = node_weight (mSet.self[curr].mLeft) ;
		const auto r2x = node_weight (mSet.self[curr].mRight) ;
		mSet.self[curr].mWeight = r1x + r2x + 1 ;
		mTop = ix ;
	}

	void rotate_right (const INDEX &curr) {
		INDEX ix = mSet.self[curr].mLeft ;
		mSet.self[curr].mLeft = mSet.self[ix].mRight ;
		mSet.self[ix].mRight = curr ;
		mSet.self[ix].mWeight = mSet.self[curr].mWeight ;
		const auto r1x = node_weight (mSet.self[curr].mLeft) ;
		const auto r2x = node_weight (mSet.self[curr].mRight) ;
		mSet.self[curr].mWeight = r1x + r2x + 1 ;
		mTop = ix ;
	}

	LENGTH node_weight (const INDEX &curr) const {
		if (curr == VAR_NONE)
			return 0 ;
		return mSet.self[curr].mWeight ;
	}

	void compute_esort (const INDEX &curr ,Array<INDEX> &out ,INDEX &out_i) const {
		if (curr == VAR_NONE)
			return ;
		INDEX iw = out_i ;
		compute_esort (mSet[curr].mLeft ,out ,iw) ;
		out[iw++] = curr ;
		compute_esort (mSet[curr].mRight ,out ,iw) ;
		out_i = iw ;
	}
} ;

template <class ITEM ,class SIZE>
template <class BASE>
class SoftSet<ITEM ,SIZE>::Private::Pair
	:private Proxy {
public:
	const ITEM &key ;
	CAST_TRAITS_TYPE<INDEX ,BASE> &sid ;

public:
	implicit Pair () = delete ;

	explicit Pair (BASE &base ,const INDEX &index)
		: key (base.mSet.self[index].mItem) ,sid (base.mSet.self[index].mMap) {}

	inline implicit operator const ITEM & () rightvalue {
		return key ;
	}
} ;
} ;