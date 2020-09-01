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
	using ITEM = DEF<decltype (_NULL_ (ARGV<BASE>::ID).get (_NULL_ (ARGV<const INDEX>::ID)))> ;

private:
	PhanRef<BASE> mBase ;
	INDEX mIndex ;

public:
	implicit ArrayIterator () = delete ;

	explicit ArrayIterator (PhanRef<BASE> &&base ,const INDEX &index) {
		mBase = _MOVE_ (base) ;
		mIndex = index ;
	}

	inline BOOL operator!= (const ArrayIterator &that) const {
		return BOOL (mIndex != that.mIndex) ;
	}

	inline ITEM operator* () const leftvalue {
		auto &r1x = _FORWARD_ (ARGV<const INDEX>::ID ,mIndex) ;
		return mBase->get (r1x) ;
	}

	inline void operator++ () {
		auto &r1x = _FORWARD_ (ARGV<const INDEX>::ID ,mIndex) ;
		mIndex = mBase->inext (r1x) ;
	}
} ;

class SortInvokeProc
	:private Wrapped<> {
public:
	template <class _ARG1 ,class _ARG2>
	imports void invoke (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &lb ,const INDEX &rb) {
		const auto r1x = rb - lb + 1 ;
		if (r1x <= 0)
			return ;
		_DEBUG_ASSERT_ (lb >= 0 && lb < out.size ()) ;
		_DEBUG_ASSERT_ (rb >= 0 && rb < out.size ()) ;
		quick_sort (array_ ,out ,lb ,rb ,r1x) ;
	}

private:
	template <class _ARG1 ,class _ARG2>
	imports void insert_sort (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &lb ,const INDEX &rb) {
		for (auto &&i : _RANGE_ (lb ,rb)) {
			INDEX ix = i + 1 ;
			INDEX iy = i ;
			auto rax = _MOVE_ (out[ix]) ;
			while (TRUE) {
				if (iy < lb)
					break ;
				if (array_[rax] >= array_[out[iy]])
					break ;
				out[ix] = _MOVE_ (out[iy]) ;
				ix = iy ;
				iy-- ;
			}
			out[ix] = _MOVE_ (rax) ;
		}
	}

	template <class _ARG1 ,class _ARG2>
	imports void quick_sort_partition (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &lb ,const INDEX &rb ,INDEX &mid_one) {
		INDEX ix = lb ;
		INDEX iy = rb ;
		auto rax = _MOVE_ (out[ix]) ;
		while (TRUE) {
			while (TRUE) {
				if (ix >= iy)
					break ;
				if (array_[out[iy]] <= array_[rax])
					break ;
				iy-- ;
			}
			if (ix >= iy)
				break ;
			out[ix++] = _MOVE_ (out[iy]) ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				if (array_[out[ix]] >= array_[rax])
					break ;
				ix++ ;
			}
			if (ix >= iy)
				break ;
			out[iy--] = _MOVE_ (out[ix]) ;
		}
		out[ix] = _MOVE_ (rax) ;
		mid_one = ix ;
	}

	template <class _ARG1 ,class _ARG2>
	imports void quick_sort (const _ARG1 &array_ ,_ARG2 &out ,const INDEX &lb ,const INDEX &rb ,const LENGTH &ideal) {
		auto rax = ideal ;
		INDEX ix = lb ;
		INDEX iy = rb ;
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

	implicit Array (const api::initializer_list<ITEM> &that)
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
		return ArrayIterator<Array> (PhanRef<Array>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const Array> begin () const leftvalue {
		return ArrayIterator<const Array> (PhanRef<const Array>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<Array> end () leftvalue {
		return ArrayIterator<Array> (PhanRef<Array>::make (DEREF[this]) ,iend ()) ;
	}

	ArrayIterator<const Array> end () const leftvalue {
		return ArrayIterator<const Array> (PhanRef<const Array>::make (DEREF[this]) ,iend ()) ;
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

	template <class... _ARGS>
	imports Array make (const _ARGS &...args) {
		const auto r1x = _RANGE_ (args...) ;
		Array ret = Array (r1x.size ()) ;
		INDEX iw = 0 ;
		for (auto &&i : r1x)
			ret[iw++] = i ;
		_DEBUG_ASSERT_ (iw == ret.size ()) ;
		return _MOVE_ (ret) ;
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
template <class _ARG1>
struct CONSTEXPR_RESERVE_SIZE {
	imports constexpr VAR compile () {
		using R1X = U::CONSTEXPR_COMPR_GT<_ARG1 ,ZERO> ;
		using R2X = CONDITIONAL_TYPE<R1X ,ARGC<1> ,ZERO> ;
		using R3X = CONSTEXPR_ADD<_ARG1 ,R2X> ;
		return R3X::compile () ;
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
	using RESERVE_SIZE = ARGC_TYPE<U::CONSTEXPR_RESERVE_SIZE<SIZE>> ;

private:
	Buffer<ITEM ,RESERVE_SIZE> mString ;

public:
	implicit String () {
		clear () ;
	}

	explicit String (const LENGTH &len)
		:String (ARGVP0 ,reserve_size (len)) {
		clear () ;
	}

	implicit String (const api::initializer_list<ITEM> &that)
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
		return ArrayIterator<String> (PhanRef<String>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const String> begin () const leftvalue {
		return ArrayIterator<const String> (PhanRef<const String>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<String> end () leftvalue {
		return ArrayIterator<String> (PhanRef<String>::make (DEREF[this]) ,iend ()) ;
	}

	ArrayIterator<const String> end () const leftvalue {
		return ArrayIterator<const String> (PhanRef<const String>::make (DEREF[this]) ,iend ()) ;
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
		if (_EBOOL_ (empty ()) != _EBOOL_ (that.empty ()))
			return FALSE ;
		if (empty ())
			return TRUE ;
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
			return ComprInvokeProc::invoke (r1x ,r2x) ;
		if (r2x == 0)
			return ComprInvokeProc::invoke (r1x ,r2x) ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (mString[ix] == ITEM (0))
				break ;
			if (mString[ix] != that.mString[ix])
				break ;
			ix++ ;
		}
		return ComprInvokeProc::invoke (mString[ix] ,that.mString[ix]) ;
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

	String segment (const INDEX &lb ,const INDEX &rb) const {
		String ret ;
		const auto r1x = rb - lb + 1 ;
		if switch_once (TRUE) {
			if (r1x <= 0)
				discard ;
			_DEBUG_ASSERT_ (lb >= 0 && lb < size ()) ;
			_DEBUG_ASSERT_ (rb >= 0 && rb < size ()) ;
			ret = String (r1x) ;
			for (auto &&i : _RANGE_ (0 ,r1x))
				ret.get (i) = get (lb + i) ;
		}
		return _MOVE_ (ret) ;
	}

	template <class... _ARGS>
	imports String make (const _ARGS &...initval) {
		struct Dependent ;
		using R1X = DEPENDENT_TYPE<TextWriter<ITEM> ,Dependent> ;
		_STATIC_ASSERT_ (IS_SAME_HELP<SIZE ,SAUTO>::compile ()) ;
		String ret = String (DEFAULT_LONGSTRING_SIZE::compile ()) ;
		auto rax = R1X (ret.raw ()) ;
		rax.prints (initval...) ;
		rax << R1X::EOS ;
		return _MOVE_ (ret) ;
	}

private:
	explicit String (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mString (len) {}

	imports LENGTH reserve_size (const LENGTH &len) {
		return len + _EBOOL_ (len > 0) ;
	}

	imports LENGTH plain_string_length (const ARR<ITEM> &val) {
		using R1X = U::CONSTEXPR_INCREASE<DEFAULT_HUGESTRING_SIZE> ;
		LENGTH ret = BasicProc::mem_chr (val ,R1X::compile () ,ITEM (0)) ;
		_DYNAMIC_ASSERT_ (ret >= 0 && ret <= DEFAULT_HUGESTRING_SIZE::compile ()) ;
		return _MOVE_ (ret) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Deque ;

template <class ITEM ,class SIZE>
class Deque {
private:
	using RESERVE_SIZE = ARGC_TYPE<U::CONSTEXPR_RESERVE_SIZE<SIZE>> ;

private:
	Buffer<ITEM ,RESERVE_SIZE> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	implicit Deque () {
		clear () ;
	}

	explicit Deque (const LENGTH &len)
		:Deque (ARGVP0 ,reserve_size (len)) {
		clear () ;
	}

	implicit Deque (const api::initializer_list<ITEM> &that)
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
		return ArrayIterator<Deque> (PhanRef<Deque>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const Deque> begin () const leftvalue {
		return ArrayIterator<const Deque> (PhanRef<const Deque>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<Deque> end () leftvalue {
		return ArrayIterator<Deque> (PhanRef<Deque>::make (DEREF[this]) ,iend ()) ;
	}

	ArrayIterator<const Deque> end () const leftvalue {
		return ArrayIterator<const Deque> (PhanRef<const Deque>::make (DEREF[this]) ,iend ()) ;
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
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		SortInvokeProc::invoke (DEREF[this] ,ret ,0 ,ret.length () - 1) ;
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

	void add (const REMOVE_CONST_TYPE<ITEM> &item) {
		if (mDeque.size () == 0)
			update_emplace () ;
		mDeque[mWrite] = _MOVE_ (item) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_emplace () ;
	}

	inline Deque &operator<< (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		if (mDeque.size () == 0)
			update_emplace () ;
		mDeque[mWrite] = _MOVE_ (item) ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_emplace () ;
	}

	inline Deque &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		reserve (val.length ()) ;
		for (auto &&i : val)
			add (i) ;
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

	void push (const REMOVE_CONST_TYPE<ITEM> &item) {
		if (mDeque.size () == 0)
			update_emplace () ;
		INDEX ix = (mRead - 1 + mDeque.size ()) % mDeque.size () ;
		mDeque[ix] = _MOVE_ (item) ;
		mRead = ix ;
		update_emplace () ;
	}

	void push (REMOVE_CONST_TYPE<ITEM> &&item) {
		if (mDeque.size () == 0)
			update_emplace () ;
		INDEX ix = (mRead - 1 + mDeque.size ()) % mDeque.size () ;
		mDeque[ix] = _MOVE_ (item) ;
		mRead = ix ;
		update_emplace () ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite = (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
	}

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mRead ;
	}

	INDEX tail () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
	}

	INDEX insert () {
		if (mDeque.size () == 0)
			update_emplace () ;
		INDEX ret = mWrite ;
		mWrite = (mWrite + 1) % mDeque.size () ;
		update_emplace () ;
		return _MOVE_ (ret) ;
	}

private:
	explicit Deque (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mDeque (len) {}

	imports LENGTH reserve_size (const LENGTH &len) {
		return len + _EBOOL_ (len > 0) ;
	}

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
		auto rax = mDeque.expand (mDeque.size () + r3x) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(mRead <= mWrite))
				discard ;
			BasicProc::mem_move (PTRTOARR[DEPTR[rax.self[mRead]]] ,PTRTOARR[DEPTR[mDeque.self[mRead]]] ,(mWrite - mRead)) ;
		}
		if switch_once (fax) {
			BasicProc::mem_move (rax.self ,mDeque.self ,mWrite) ;
			INDEX ix = mRead + rax.size () - mDeque.size () ;
			BasicProc::mem_move (PTRTOARR[DEPTR[rax.self[ix]]] ,PTRTOARR[DEPTR[mDeque.self[mRead]]] ,(mDeque.size () - mRead)) ;
			mRead = ix ;
		}
		mDeque.swap (rax) ;
	}

	void update_emplace () {
		if (mRead != mWrite)
			return ;
		auto rax = mDeque.expand (mDeque.expand_size ()) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (mRead != 0)
				discard ;
			BasicProc::mem_move (rax.self ,mDeque.self ,mDeque.size ()) ;
			mWrite = mDeque.size () ;
		}
		if switch_once (fax) {
			BasicProc::mem_move (rax.self ,mDeque.self ,mWrite) ;
			INDEX ix = mRead + rax.size () - mDeque.size () ;
			BasicProc::mem_move (PTRTOARR[DEPTR[rax.self[ix]]] ,PTRTOARR[DEPTR[mDeque.self[mRead]]] ,(mDeque.size () - mRead)) ;
			mRead = ix ;
		}
		mDeque.swap (rax) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Priority ;

template <class ITEM ,class SIZE>
class Priority {
private:
	struct NODE_PACK {
		ITEM mItem ;
		INDEX mMap ;
	} ;

	using RESERVE_SIZE = ARGC_TYPE<U::CONSTEXPR_RESERVE_SIZE<SIZE>> ;

private:
	Buffer<NODE_PACK ,RESERVE_SIZE> mPriority ;
	INDEX mWrite ;
	INDEX mTop ;

public:
	implicit Priority () {
		clear () ;
	}

	explicit Priority (const LENGTH &len)
		:Priority (ARGVP0 ,reserve_size (len)) {
		clear () ;
	}

	implicit Priority (const api::initializer_list<ITEM> &that)
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

	ArrayIterator<const Priority> begin () const leftvalue {
		return ArrayIterator<const Priority> (PhanRef<const Priority>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const Priority> end () const leftvalue {
		return ArrayIterator<const Priority> (PhanRef<const Priority>::make (DEREF[this]) ,iend ()) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mPriority[index].mItem ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&NODE_PACK::mItem ,item)) ;
		if (!(ret >= 0 && ret < mWrite))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	INDEX map_get (const INDEX &index) const {
		if (index == VAR_NONE)
			return VAR_NONE ;
		return mPriority[index].mMap ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		INDEX ix = ret.length () ;
		while (TRUE) {
			ix-- ;
			if (ix < 1)
				break ;
			_SWAP_ (ret[0] ,ret[ix]) ;
			compute_order (ret ,ix) ;
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

	void add (const REMOVE_CONST_TYPE<ITEM> &item ,const INDEX &map_) {
		if (mPriority.size () == 0)
			update_emplace () ;
		INDEX ix = mWrite ;
		mPriority[ix].mItem = _MOVE_ (item) ;
		mPriority[ix].mMap = map_ ;
		mWrite++ ;
		update_emplace () ;
		update_insert (ix) ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Priority &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item ,const INDEX &map_) {
		if (mPriority.size () == 0)
			update_emplace () ;
		INDEX ix = mWrite ;
		mPriority[ix].mItem = _MOVE_ (item) ;
		mPriority[ix].mMap = map_ ;
		mWrite++ ;
		update_emplace () ;
		update_insert (ix) ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		reserve (val.length ()) ;
		for (auto &&i : val) {
			const auto r1x = val.at (i) ;
			add (i ,val.map_get (r1x)) ;
		}
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mWrite - 1 ;
		mPriority[0] = _MOVE_ (mPriority[ix]) ;
		mWrite = ix ;
		update_insert (0) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = _MOVE_ (mPriority[0]) ;
		INDEX ix = mWrite - 1 ;
		mPriority[0] = _MOVE_ (mPriority[ix]) ;
		mWrite = ix ;
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

	INDEX insert (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	void remove (const INDEX &index) {
		_DEBUG_ASSERT_ (index >= 0 && index < mWrite) ;
		INDEX ix = mWrite - 1 ;
		mPriority[index] = _MOVE_ (mPriority[ix]) ;
		mWrite = ix ;
		update_insert (index) ;
	}

private:
	explicit Priority (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mPriority (len) {}

	imports LENGTH reserve_size (const LENGTH &len) {
		return len + _EBOOL_ (len > 0) ;
	}

	void reserve (const LENGTH &len) {
		const auto r1x = len - (size () - length ()) ;
		const auto r2x = VAR_ZERO ;
		const auto r3x = _MAX_ (r1x ,r2x) ;
		if (r3x == 0)
			return ;
		auto rax = mPriority.expand (mPriority.size () + r3x) ;
		BasicProc::mem_move (rax.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.swap (rax) ;
	}

	void update_emplace () {
		if (mWrite < mPriority.size ())
			return ;
		auto rax = mPriority.expand (mPriority.expand_size ()) ;
		BasicProc::mem_move (rax.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.swap (rax) ;
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
		auto rax = _MOVE_ (mPriority[ix]) ;
		while (TRUE) {
			INDEX iy = parent (ix) ;
			if (iy < 0)
				break ;
			if (rax.mItem >= mPriority[iy].mItem)
				break ;
			mPriority[ix] = _MOVE_ (mPriority[iy]) ;
			ix = iy ;
		}
		while (TRUE) {
			INDEX iy = left_child (ix) ;
			if (iy >= mWrite)
				break ;
			INDEX jx = ix ;
			if (rax.mItem > mPriority[iy].mItem)
				jx = iy ;
			INDEX iz = right_child (ix) ;
			auto &r1x = _SWITCH_ (
				(jx != ix) ? mPriority[jx].mItem :
				rax.mItem) ;
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
		mPriority[ix] = _MOVE_ (rax) ;
		mTop = ix ;
	}

	void compute_order (Array<INDEX> &out ,const LENGTH &len) const {
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

template <class ITEM ,class SIZE = SAUTO>
class List ;

template <class ITEM ,class SIZE>
class List {
private:
	struct NODE_PACK {
		ITEM mItem ;
		INDEX mLeft ;
		INDEX mRight ;

		template <class... _ARGS>
		explicit NODE_PACK (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) ,mLeft (VAR_NONE) ,mRight (VAR_NONE) {}
	} ;

private:
	Allocator<NODE_PACK ,SIZE> mList ;
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

	implicit List (const api::initializer_list<ITEM> &that)
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
		return ArrayIterator<List> (PhanRef<List>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const List> begin () const leftvalue {
		return ArrayIterator<const List> (PhanRef<const List>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<List> end () leftvalue {
		return ArrayIterator<List> (PhanRef<List>::make (DEREF[this]) ,iend ()) ;
	}

	ArrayIterator<const List> end () const leftvalue {
		return ArrayIterator<const List> (PhanRef<const List>::make (DEREF[this]) ,iend ()) ;
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
		return mList.at (_OFFSET_ (&NODE_PACK::mItem ,item)) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = range () ;
		SortInvokeProc::invoke (DEREF[this] ,ret ,0 ,ret.length () - 1) ;
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

	void add (const REMOVE_CONST_TYPE<ITEM> &item) {
		INDEX ix = mList.alloc (ARGVP0 ,_MOVE_ (item)) ;
		mList[ix].mLeft = mLast ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = ix ;
		mLast = ix ;
	}

	inline List &operator<< (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		INDEX ix = mList.alloc (ARGVP0 ,_MOVE_ (item)) ;
		mList[ix].mLeft = mLast ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = ix ;
		mLast = ix ;
	}

	inline List &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mList.reserve (val.length ()) ;
		for (auto &&i : val)
			add (i) ;
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

	void push (const REMOVE_CONST_TYPE<ITEM> &item) {
		INDEX ix = mList.alloc (ARGVP0 ,_MOVE_ (item)) ;
		mList[ix].mRight = mFirst ;
		auto &r1x = _SWITCH_ (
			(mFirst != VAR_NONE) ? mList[mFirst].mLeft :
			mLast) ;
		r1x = ix ;
		mFirst = ix ;
	}

	void push (REMOVE_CONST_TYPE<ITEM> &&item) {
		INDEX ix = mList.alloc (ARGVP0 ,_MOVE_ (item)) ;
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

	INDEX head () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mFirst ;
	}

	INDEX tail () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mLast ;
	}

	INDEX insert () {
		INDEX ret = mList.alloc (ARGVP0) ;
		mList[ret].mLeft = mLast ;
		auto &r1x = _SWITCH_ (
			(mLast != VAR_NONE) ? mList[mLast].mRight :
			mFirst) ;
		r1x = ret ;
		mLast = ret ;
		return _MOVE_ (ret) ;
	}

	void remove (const INDEX &index) {
		prev_next (index) = mList[index].mRight ;
		next_prev (index) = mList[index].mLeft ;
		mList.free (index) ;
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

	void esort (const Array<INDEX> &order_) {
		_DEBUG_ASSERT_ (order_.length () == length ()) ;
		if (order_.length () < 2)
			return ;
		for (auto &&i : order_) {
			if switch_once (TRUE) {
				if (mList[i].mLeft != VAR_NONE)
					discard ;
				if (mList[i].mRight != VAR_NONE)
					discard ;
				_DEBUG_ASSERT_ (FALSE) ;
			}
			mList[i].mLeft = VAR_NONE ;
			mList[i].mRight = VAR_NONE ;
		}
		for (auto &&i : _RANGE_ (0 ,1)) {
			mList[order_[i]].mLeft = VAR_NONE ;
			mList[order_[i]].mRight = order_[i + 1] ;
			mFirst = order_[i] ;
		}
		for (auto &&i : _RANGE_ (1 ,order_.length () - 1)) {
			mList[order_[i]].mLeft = order_[i - 1] ;
			mList[order_[i]].mRight = order_[i + 1] ;
		}
		for (auto &&i : _RANGE_ (order_.length () - 1 ,order_.length ())) {
			mList[order_[i]].mLeft = order_[i - 1] ;
			mList[order_[i]].mRight = VAR_NONE ;
			mLast = order_[i] ;
		}
	}

private:
	explicit List (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mList (len) {}

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
class ArrayList ;

template <class ITEM ,class SIZE>
class ArrayList {
private:
	struct NODE_PACK {
		ITEM mItem ;
		INDEX mIndex ;

		template <class... _ARGS>
		explicit NODE_PACK (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) ,mIndex (VAR_NONE) {}
	} ;

private:
	Allocator<NODE_PACK ,SIZE> mList ;
	Buffer<INDEX ,SIZE> mRange ;
	INDEX mFree ;

public:
	implicit ArrayList () {
		clear () ;
	}

	explicit ArrayList (LENGTH len)
		:ArrayList (ARGVP0 ,len) {
		clear () ;
	}

	implicit ArrayList (const api::initializer_list<ITEM> &that)
		: ArrayList (that.size ()) {
		for (auto &&i : that)
			add (i) ;
	}

	LENGTH size () const {
		return mRange.size () ;
	}

	LENGTH length () const {
		return mList.length () ;
	}

	void clear () {
		mList.clear () ;
		BasicProc::mem_fill (mRange.self ,mRange.size () ,VAR_NONE) ;
		mFree = 0 ;
	}

	INDEX ibegin () const {
		for (auto &&i : _RANGE_ (0 ,mRange.size ())) {
			if (mRange[i] != VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		const auto r1x = index + 1 ;
		for (auto &&i : _RANGE_ (r1x ,mRange.size ())) {
			if (mRange[i] != VAR_NONE)
				return i ;
		}
		return VAR_NONE ;
	}

	ArrayIterator<ArrayList> begin () leftvalue {
		return ArrayIterator<ArrayList> (PhanRef<ArrayList>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const ArrayList> begin () const leftvalue {
		return ArrayIterator<const ArrayList> (PhanRef<const ArrayList>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<ArrayList> end () leftvalue {
		return ArrayIterator<ArrayList> (PhanRef<ArrayList>::make (DEREF[this]) ,iend ()) ;
	}

	ArrayIterator<const ArrayList> end () const leftvalue {
		return ArrayIterator<const ArrayList> (PhanRef<const ArrayList>::make (DEREF[this]) ,iend ()) ;
	}

	ITEM &get (INDEX index) leftvalue {
		return mList[mRange[index]].mItem ;
	}

	inline ITEM &operator[] (INDEX index) leftvalue {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const leftvalue {
		return mList[mRange[index]].mItem ;
	}

	inline const ITEM &operator[] (INDEX index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mList.at (_OFFSET_ (&NODE_PACK::mItem ,item)) ;
		if (ret != VAR_NONE)
			ret = mList[ret].mIndex ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const ArrayList &that) const {
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

	inline BOOL operator== (const ArrayList &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const ArrayList &that) const {
		return !equal (that) ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item) {
		INDEX ix = mList.alloc (ARGVP0 ,_MOVE_ (item)) ;
		update_range (ix) ;
		mList[ix].mIndex = next_free_one () ;
		mRange[mList[ix].mIndex] = ix ;
	}

	inline ArrayList &operator<< (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		INDEX ix = mList.alloc (ARGVP0 ,_MOVE_ (item)) ;
		update_range (ix) ;
		mList[ix].mIndex = next_free_one () ;
		mRange[mList[ix].mIndex] = ix ;
	}

	inline ArrayList &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mList.reserve (val.length ()) ;
		for (auto &&i : val)
			add (i) ;
	}

	INDEX insert () {
		INDEX ix = mList.alloc (ARGVP0) ;
		update_range (ix) ;
		mList[ix].mIndex = next_free_one () ;
		mRange[mList[ix].mIndex] = ix ;
		return mList[ix].mIndex ;
	}

	INDEX insert (const INDEX &index) {
		if switch_once (TRUE) {
			if (mRange[index] != VAR_NONE)
				discard ;
			INDEX ix = mList.alloc (ARGVP0) ;
			update_range (ix) ;
			mList[ix].mIndex = index ;
			mRange[mList[ix].mIndex] = ix ;
		}
		return mList[mRange[index]].mIndex ;
	}

	void remove (const INDEX &index) {
		mList.free (mRange[index]) ;
		mRange[index] = VAR_NONE ;
	}

	void eswap (const INDEX &index1 ,const INDEX &index2) {
		if (index1 == index2)
			return ;
		_SWAP_ (mRange[index1] ,mRange[index2]) ;
		mList[mRange[index1]].mIndex = index1 ;
		mList[mRange[index2]].mIndex = index2 ;
	}

	void esort (const Array<INDEX> &order_) {
		_DEBUG_ASSERT_ (order_.length () == length ()) ;
		if (order_.length () < 2)
			return ;
		for (auto &&i : _RANGE_ (0 ,order_.length ())) {
			mList[mRange[order_[i]]].mIndex = i ;
			mRange[order_[i]] = VAR_NONE ;
		}
		BasicProc::mem_fill (mRange.self ,mRange.size () ,VAR_NONE) ;
		for (auto &&i : _RANGE_ (0 ,mList.size ())) {
			if (!mList.used (i))
				continue ;
			mRange[mList[i].mIndex] = i ;
		}
	}

	void remap () {
		if (mRange.size () == mList.length ())
			return ;
		auto rax = mRange.expand (mList.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : _RANGE_ (0 ,mRange.size ())) {
			if (mRange[i] == VAR_NONE)
				continue ;
			INDEX ix = iw++ ;
			rax[ix] = mRange[i] ;
			mList[rax[ix]].mIndex = ix ;
			mRange[i] = VAR_NONE ;
		}
		_DEBUG_ASSERT_ (iw == rax.size ()) ;
		mRange.swap (rax) ;
	}

private:
	explicit ArrayList (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mList (len) ,mRange (len) {}

	INDEX next_free_one () {
		_DEBUG_ASSERT_ (mRange.size () > 0) ;
		const auto r1x = mFree % mRange.size () ;
		mFree = r1x ;
		while (TRUE) {
			if (mFree >= mRange.size ())
				break ;
			if (mRange[mFree] == VAR_NONE)
				break ;
			mFree++ ;
		}
		if (mFree < mRange.size ())
			return mFree ;
		mFree = 0 ;
		while (TRUE) {
			if (mFree >= r1x)
				break ;
			if (mRange[mFree] == VAR_NONE)
				break ;
			mFree++ ;
		}
		if (mFree < r1x)
			return mFree ;
		return VAR_NONE ;
	}

	void update_range (const INDEX &curr) {
		if (mRange.size () == mList.size ())
			return ;
		if switch_once (TRUE) {
			auto rax = mRange.expand (mList.size ()) ;
			BasicProc::mem_fill (rax.self ,rax.size () ,VAR_NONE) ;
			mRange.swap (rax) ;
		}
		for (auto &&i : _RANGE_ (0 ,mList.size ())) {
			if (i == curr)
				continue ;
			if (!mList.used (i))
				continue ;
			mRange[mList[i].mIndex] = i ;
		}
	}
} ;

namespace U {
template <class UNIT>
struct CONSTEXPR_CEIL8_SIZE_SWITCH_CASE1 {
	imports constexpr UNIT invoke (const UNIT &len) {
		return len ;
	}
} ;

template <class UNIT>
struct CONSTEXPR_CEIL8_SIZE_SWITCH_CASE2 {
	imports constexpr UNIT invoke (const UNIT &len) {
		return (len + 7) / 8 ;
	}
} ;

template <class _ARG1>
struct CONSTEXPR_CEIL8_SIZE {
	imports constexpr VAR compile () {
		using R1X = U::CONSTEXPR_COMPR_GT<_ARG1 ,ZERO> ;
		using R2X = CONSTEXPR_MOD<CONSTEXPR_ADD<_ARG1 ,ARGC<7>> ,ARGC<8>> ;
		using R3X = CONDITIONAL_TYPE<R1X ,R2X ,_ARG1> ;
		return R3X::compile () ;
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

		template <class>
		class ArrayRange ;
	} ;

	using CEIL8_SIZE = ARGC_TYPE<U::CONSTEXPR_CEIL8_SIZE<SIZE>> ;

private:
	Buffer<BYTE ,CEIL8_SIZE> mSet ;
	LENGTH mWidth ;

public:
	implicit BitSet ()
		:BitSet (ARGVP0) {
		clear () ;
	}

	explicit BitSet (const LENGTH &len)
		:BitSet (ARGVP0 ,ceil8_size (len) ,forward_width (len)) {
		clear () ;
	}

	explicit BitSet (const Array<INDEX> &range_ ,const LENGTH &len)
		:BitSet (len) {
		for (auto &&i : range_)
			add (i) ;
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
		for (auto &&i : _RANGE_ (0 ,size ())) {
			if (get (i))
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		const auto r1x = index + 1 ;
		for (auto &&i : _RANGE_ (r1x ,size ())) {
			if (get (i))
				return i ;
		}
		return VAR_NONE ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template ArrayRange<const BitSet>>>
	ArrayIterator<const _RET> begin () const leftvalue {
		auto &r1x = _CAST_ (ARGV<const _RET>::ID ,DEREF[this]) ;
		return ArrayIterator<const _RET> (PhanRef<const _RET>::make (r1x) ,ibegin ()) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template ArrayRange<const BitSet>>>
	ArrayIterator<const _RET> end () const leftvalue {
		auto &r1x = _CAST_ (ARGV<const _RET>::ID ,DEREF[this]) ;
		return ArrayIterator<const _RET> (PhanRef<const _RET>::make (r1x) ,iend ()) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<BitSet>>>
	_RET get (const INDEX &index) leftvalue {
		struct Dependent ;
		using R1X = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<BitSet> ;
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return R1X (PhanRef<BitSet>::make (DEREF[this]) ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<BitSet>>>
	inline _RET operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<const BitSet>>>
	_RET get (const INDEX &index) const leftvalue {
		struct Dependent ;
		using R1X = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<const BitSet> ;
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return R1X (PhanRef<const BitSet>::make (DEREF[this]) ,index) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<typename Private::template Bit<const BitSet>>>
	inline _RET operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const DEF<typename Private::template Bit<BitSet>> &item) const {
		struct Dependent ;
		using R1X = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<BitSet> ;
		auto &r1x = _FORWARD_ (ARGV<R1X>::ID ,item) ;
		return r1x ;
	}

	INDEX at (const DEF<typename Private::template Bit<const BitSet>> &item) const {
		struct Dependent ;
		using R1X = typename DEPENDENT_TYPE<Private ,Dependent>::template Bit<const BitSet> ;
		auto &r1x = _FORWARD_ (ARGV<R1X>::ID ,item) ;
		return r1x ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const BitSet &that) const {
		_DEBUG_ASSERT_ (size () == that.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		if (ix < 0)
			return TRUE ;
		for (auto &&i : _RANGE_ (0 ,ix)) {
			if (mSet[i] != that.mSet[i])
				return FALSE ;
		}
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
		const auto r1x = BasicProc::mem_compr (mSet.self ,that.mSet.self ,ix) ;
		if (r1x != 0)
			return r1x ;
		const auto r2x = BYTE (mWidth % 8 - 1) ;
		const auto r3x = BYTE (mSet[ix] & r2x) ;
		const auto r4x = BYTE (that.mSet[ix] & r2x) ;
		return ComprInvokeProc::invoke (r3x ,r4x) ;
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
		const auto r1x = SIZE::compile () ;
		const auto r2x = VAR_ZERO ;
		mWidth = _MAX_ (r1x ,r2x) ;
	}

	explicit BitSet (const DEF<decltype (ARGVP0)> & ,const LENGTH &len ,const LENGTH &width)
		:mSet (len) ,mWidth (width) {}

	imports LENGTH ceil8_size (const LENGTH &len) {
		if (len < 0)
			return len ;
		return (len + 7) / 8 ;
	}

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
	PhanRef<BASE> mBase ;
	INDEX mIndex ;

public:
	implicit Bit () = delete ;

	explicit Bit (PhanRef<BASE> &&base ,const INDEX &index) {
		mBase = _MOVE_ (base) ;
		mIndex = index ;
	}

	inline implicit operator BOOL () rightvalue {
		const auto r1x = BYTE (BYTE (0X01) << (mIndex % 8)) ;
		const auto r2x = BYTE (mBase->mSet[mIndex / 8] & r1x) ;
		if (r2x == 0)
			return FALSE ;
		return TRUE ;
	}

	inline implicit operator BOOL () const leftvalue = delete ;

	inline void operator= (const BOOL &that) rightvalue {
		const auto r1x = BYTE (BYTE (0X01) << (mIndex % 8)) ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!that)
				discard ;
			mBase->mSet[mIndex / 8] |= r1x ;
		}
		if switch_once (fax) {
			mBase->mSet[mIndex / 8] &= ~r1x ;
		}
	}
} ;

template <class SIZE>
template <class BASE>
class BitSet<SIZE>::Private::ArrayRange
	:private Wrapped<BASE> {
private:
	using Wrapped<BASE>::mSelf ;

public:
	INDEX inext (const INDEX &index) const {
		return mSelf.inext (index) ;
	}

	const INDEX &get (const INDEX &index) const leftvalue {
		return index ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Set ;

template <class ITEM ,class SIZE>
class Set {
private:
	struct NODE_PACK {
		ITEM mItem ;
		INDEX mMap ;
		BOOL mRed ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;

		template <class... _ARGS>
		explicit NODE_PACK (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) ,mMap (VAR_NONE) ,mRed (FALSE) ,mUp (VAR_NONE) ,mLeft (VAR_NONE) ,mRight (VAR_NONE) {}
	} ;

private:
	Allocator<NODE_PACK ,SIZE> mSet ;
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

	implicit Set (const api::initializer_list<ITEM> &that)
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
		for (auto &&i : _RANGE_ (0 ,size ())) {
			if (mSet.used (i))
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		const auto r1x = index + 1 ;
		for (auto &&i : _RANGE_ (r1x ,size ())) {
			if (mSet.used (i))
				return i ;
		}
		return VAR_NONE ;
	}

	ArrayIterator<const Set> begin () const leftvalue {
		return ArrayIterator<const Set> (PhanRef<const Set>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const Set> end () const leftvalue {
		return ArrayIterator<const Set> (PhanRef<const Set>::make (DEREF[this]) ,iend ()) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mSet[index].mItem ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mSet.at (_OFFSET_ (&NODE_PACK::mItem ,item)) ;
	}

	INDEX map_get (const INDEX &index) const {
		if (index == VAR_NONE)
			return VAR_NONE ;
		return mSet[index].mMap ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		compute_order (mRoot ,ret ,iw) ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Set &operator<< (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (ARGVP0 ,_MOVE_ (item)) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mRed = TRUE ;
			update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline Set &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (ARGVP0 ,_MOVE_ (item)) ;
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
		for (auto &&i : val) {
			const auto r1x = val.at (i) ;
			add (i ,val.map_get (r1x)) ;
		}
	}

	INDEX head () const {
		INDEX ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			INDEX ix = mSet[ret].mLeft ;
			if (ix == VAR_NONE)
				break ;
			ret = ix ;
		}
		return _MOVE_ (ret) ;
	}

	INDEX tail () const {
		INDEX ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			INDEX ix = mSet[ret].mRight ;
			if (ix == VAR_NONE)
				break ;
			ret = ix ;
		}
		return _MOVE_ (ret) ;
	}

	INDEX insert (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CONST_TYPE<ITEM> &&item) {
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
		return map_get (find (item)) ;
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

	void update_emplace (const INDEX &curr ,const INDEX &last) {
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(curr == VAR_NONE))
				discard ;
			mTop = last ;
		}
		if switch_once (fax) {
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
		INDEX ret = mSet[index].mRight ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			INDEX ix = mSet[ret].mLeft ;
			if (ix == VAR_NONE)
				break ;
			ret = ix ;
		}
		return _MOVE_ (ret) ;
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

	void compute_order (const INDEX &curr ,Array<INDEX> &out ,INDEX &out_iw) const {
		if (curr == VAR_NONE)
			return ;
		INDEX iw = out_iw ;
		compute_order (mSet[curr].mLeft ,out ,iw) ;
		out[iw++] = curr ;
		compute_order (mSet[curr].mRight ,out ,iw) ;
		out_iw = iw ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class HashSet ;

template <class ITEM ,class SIZE>
class HashSet {
private:
	struct NODE_PACK {
		ITEM mItem ;
		INDEX mMap ;
		FLAG mHash ;
		INDEX mNext ;

		template <class... _ARGS>
		explicit NODE_PACK (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) ,mMap (VAR_NONE) ,mHash (0) ,mNext (VAR_NONE) {}
	} ;

private:
	Allocator<NODE_PACK ,SIZE> mSet ;
	Buffer<INDEX ,SIZE> mRange ;
	INDEX mTop ;

public:
	implicit HashSet () {
		clear () ;
	}

	explicit HashSet (const LENGTH &len)
		:HashSet (ARGVP0 ,len) {
		clear () ;
	}

	implicit HashSet (const api::initializer_list<ITEM> &that)
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
		BasicProc::mem_fill (mRange.self ,mRange.size () ,VAR_NONE) ;
		mTop = VAR_NONE ;
	}

	INDEX ibegin () const {
		for (auto &&i : _RANGE_ (0 ,size ())) {
			if (mSet.used (i))
				return i ;
		}
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (const INDEX &index) const {
		const auto r1x = index + 1 ;
		for (auto &&i : _RANGE_ (r1x ,size ())) {
			if (mSet.used (i))
				return i ;
		}
		return VAR_NONE ;
	}

	ArrayIterator<const HashSet> begin () const leftvalue {
		return ArrayIterator<const HashSet> (PhanRef<const HashSet>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const HashSet> end () const leftvalue {
		return ArrayIterator<const HashSet> (PhanRef<const HashSet>::make (DEREF[this]) ,iend ()) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mSet[index].mItem ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mSet.at (_OFFSET_ (&NODE_PACK::mItem ,item)) ;
	}

	INDEX map_get (const INDEX &index) const {
		if (index == VAR_NONE)
			return VAR_NONE ;
		return mSet[index].mMap ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline HashSet &operator<< (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (ARGVP0 ,_MOVE_ (item)) ;
			update_range (ix) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mHash = HashInvokeProc::invoke (item) ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline HashSet &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item ,const INDEX &map_) {
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet.alloc (ARGVP0 ,_MOVE_ (item)) ;
			update_range (ix) ;
			mSet[ix].mMap = map_ ;
			mSet[ix].mHash = HashInvokeProc::invoke (item) ;
			update_insert (ix) ;
		}
		mTop = ix ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		mSet.reserve (val.length ()) ;
		for (auto &&i : val) {
			const auto r1x = val.at (i) ;
			add (i ,val.map_get (r1x)) ;
		}
	}

	INDEX insert (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CONST_TYPE<ITEM> &&item) {
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
			const auto r1x = HashInvokeProc::invoke (item) ;
			_DEBUG_ASSERT_ (r1x >= 0) ;
			ret = mRange[r1x % mRange.size ()] ;
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
		return map_get (find (item)) ;
	}

	void erase (const ITEM &item) {
		INDEX ix = find (item) ;
		if (ix == VAR_NONE)
			return ;
		remove (ix) ;
	}

private:
	explicit HashSet (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mSet (len) ,mRange (len) {}

	void update_range (const INDEX &curr) {
		if (mRange.size () == mSet.size ())
			return ;
		if switch_once (TRUE) {
			auto rax = mRange.expand (mSet.size ()) ;
			BasicProc::mem_fill (rax.self ,rax.size () ,VAR_NONE) ;
			mRange.swap (rax) ;
		}
		for (auto &&i : _RANGE_ (0 ,mSet.size ())) {
			if (i == curr)
				continue ;
			if (!mSet.used (i))
				continue ;
			INDEX ix = mSet[i].mHash % mRange.size () ;
			mSet[i].mNext = mRange[ix] ;
			mRange[ix] = i ;
		}
	}

	void update_insert (const INDEX &curr) {
		INDEX ix = mSet[curr].mHash % mRange.size () ;
		mSet[curr].mNext = mRange[ix] ;
		mRange[ix] = curr ;
	}

	INDEX &prev_next (const INDEX &curr) leftvalue {
		INDEX ix = mSet[curr].mHash % mRange.size () ;
		if (mRange[ix] == curr)
			return mRange[ix] ;
		ix = mRange[ix] ;
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

template <class ITEM ,class SIZE = SAUTO>
class SoftSet ;

template <class ITEM ,class SIZE>
class SoftSet {
private:
	struct NODE_PACK {
		ITEM mItem ;
		INDEX mMap ;
		LENGTH mWeight ;
		INDEX mLeft ;
		INDEX mRight ;
		INDEX mNext ;

		template <class... _ARGS>
		explicit NODE_PACK (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval)
			:mItem (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) ,mMap (VAR_NONE) ,mWeight (0) ,mLeft (VAR_NONE) ,mRight (VAR_NONE) ,mNext (VAR_NONE) {}
	} ;

	struct HEAP_PACK {
		AutoRef<Allocator<NODE_PACK ,SIZE>> mBuffer ;
	} ;

private:
	SharedRef<HEAP_PACK> mHeap ;
	PhanRef<Allocator<NODE_PACK ,SIZE>> mSet ;
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
		mHeap->mBuffer = AutoRef<Allocator<NODE_PACK ,SIZE>>::make (len) ;
		mSet = PhanRef<Allocator<NODE_PACK ,SIZE>>::make (mHeap->mBuffer.self) ;
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

	SoftSet share () leftvalue {
		SoftSet ret ;
		ret.mHeap = mHeap ;
		ret.mSet = PhanRef<Allocator<NODE_PACK ,SIZE>>::make (ret.mHeap->mBuffer.self) ;
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

	ArrayIterator<const SoftSet> begin () const leftvalue {
		return ArrayIterator<const SoftSet> (PhanRef<const SoftSet>::make (DEREF[this]) ,ibegin ()) ;
	}

	ArrayIterator<const SoftSet> end () const leftvalue {
		return ArrayIterator<const SoftSet> (PhanRef<const SoftSet>::make (DEREF[this]) ,iend ()) ;
	}

	const ITEM &get (const INDEX &index) const leftvalue {
		return mSet.self[index].mItem ;
	}

	inline const ITEM &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mSet->at (_OFFSET_ (&NODE_PACK::mItem ,item)) ;
	}

	INDEX map_get (const INDEX &index) const {
		if (index == VAR_NONE)
			return VAR_NONE ;
		return mSet.self[index].mMap ;
	}

	Array<INDEX> range () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		INDEX ix = ibegin () ;
		INDEX iy = iend () ;
		while (TRUE) {
			if (ix == iy)
				break ;
			ret[iw++] = ix ;
			ix = inext (ix) ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	Array<INDEX> range_sort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		compute_order (mRoot ,ret ,iw) ;
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return _MOVE_ (ret) ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline SoftSet &operator<< (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (const REMOVE_CONST_TYPE<ITEM> &item ,const INDEX &map_) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (ARGVP0 ,_MOVE_ (item)) ;
			mSet.self[ix].mMap = map_ ;
			mSet.self[ix].mWeight = 1 ;
			auto &r1x = _SWITCH_ (
				(mLast != VAR_NONE) ? mSet.self[mLast].mNext :
				mFirst) ;
			r1x = ix ;
			mLast = ix ;
			mLength++ ;
			update_insert (mRoot ,ix) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item) ,VAR_NONE) ;
	}

	inline SoftSet &operator<< (REMOVE_CONST_TYPE<ITEM> &&item) {
		add (_MOVE_ (item)) ;
		return DEREF[this] ;
	}

	void add (REMOVE_CONST_TYPE<ITEM> &&item ,const INDEX &map_) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		INDEX ix = find (item) ;
		if switch_once (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			ix = mSet->alloc (ARGVP0 ,_MOVE_ (item)) ;
			mSet.self[ix].mMap = map_ ;
			mSet.self[ix].mWeight = 1 ;
			auto &r1x = _SWITCH_ (
				(mLast != VAR_NONE) ? mSet.self[mLast].mNext :
				mFirst) ;
			r1x = ix ;
			mLast = ix ;
			mLength++ ;
			update_insert (mRoot ,ix) ;
			mRoot = mTop ;
		}
		mTop = ix ;
	}

	template <class _ARG1>
	void appand (const _ARG1 &val) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		mSet->reserve (val.length ()) ;
		for (auto &&i : val) {
			const auto r1x = val.at (i) ;
			add (i ,val.map_get (r1x)) ;
		}
	}

	INDEX head () const {
		INDEX ret = VAR_NONE ;
		if (mHeap.exist ())
			ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			INDEX ix = mSet.self[ret].mLeft ;
			if (ix == VAR_NONE)
				break ;
			ret = ix ;
		}
		return _MOVE_ (ret) ;
	}

	INDEX tail () const {
		INDEX ret = VAR_NONE ;
		if (mHeap.exist ())
			ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			INDEX ix = mSet.self[ret].mRight ;
			if (ix == VAR_NONE)
				break ;
			ret = ix ;
		}
		return _MOVE_ (ret) ;
	}

	INDEX insert (const REMOVE_CONST_TYPE<ITEM> &item) {
		add (_MOVE_ (item)) ;
		return mTop ;
	}

	INDEX insert (REMOVE_CONST_TYPE<ITEM> &&item) {
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
		return map_get (find (item)) ;
	}

	void clean () {
		if (!mHeap.exist ())
			return ;
		mSet->clean () ;
	}

private:
	void update_insert (const INDEX &curr ,const INDEX &last) {
		INDEX ix = curr ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (!(ix != VAR_NONE))
				discard ;
			mSet.self[ix].mWeight++ ;
			const auto r1x = BOOL (mSet.self[last].mItem < mSet.self[ix].mItem) ;
			auto fbx = TRUE ;
			if switch_once (fbx) {
				if (!r1x)
					discard ;
				update_insert (mSet.self[ix].mLeft ,last) ;
				mSet.self[ix].mLeft = mTop ;
				update_insert_left (ix) ;
			}
			if switch_once (fbx) {
				update_insert (mSet.self[ix].mRight ,last) ;
				mSet.self[ix].mRight = mTop ;
				update_insert_right (ix) ;
			}
			ix = mTop ;
			mTop = ix ;
		}
		if switch_once (fax) {
			mTop = last ;
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

	void compute_order (const INDEX &curr ,Array<INDEX> &out ,INDEX &out_iw) const {
		if (curr == VAR_NONE)
			return ;
		INDEX iw = out_iw ;
		compute_order (mSet[curr].mLeft ,out ,iw) ;
		out[iw++] = curr ;
		compute_order (mSet[curr].mRight ,out ,iw) ;
		out_iw = iw ;
	}
} ;
} ;