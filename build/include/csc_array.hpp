#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"

namespace CSC {
template <class BASE>
class ForwardIterator final {
private:
	using ITERATOR = decltype (_NULL_<const BASE> ().ibegin ()) ;

private:
	BASE &mBase ;
	ITERATOR mIndex ;

public:
	inline ForwardIterator () = delete ;

	inline BOOL operator!= (const ForwardIterator &right) const {
		return mIndex != right.mIndex ;
	}

	inline auto operator* () const->decltype (_NULL_<BASE> ().get (_NULL_<const ITERATOR> ())) {
		return mBase.get (mIndex) ;
	}

	inline void operator++ () {
		mIndex = _XVALUE_<const BASE &> (mBase).inext (mIndex) ;
	}

private:
	inline explicit ForwardIterator (BASE &base ,ITERATOR &&index) :mBase (base) ,mIndex (std::move (index)) {}

public:
	inline static ForwardIterator friend_begin (BASE &base) {
		return ForwardIterator (base ,_XVALUE_<const BASE &> (base).ibegin ()) ;
	}

	inline static ForwardIterator friend_end (BASE &base) {
		return ForwardIterator (base ,_XVALUE_<const BASE &> (base).iend ()) ;
	}
} ;

inline namespace S {
template <class _ARG ,class = ENABLE_TYPE<!std::is_reference<decltype (_NULL_<const _ARG> ().ibegin ())>::value>>
inline ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>> begin (_ARG &&arg) {
	_STATIC_ASSERT_ (std::is_lvalue_reference<_ARG &&>::value) ;
	return ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>>::friend_begin (arg) ;
}

template <class _ARG ,class = ENABLE_TYPE<!std::is_reference<decltype (_NULL_<const _ARG> ().iend ())>::value>>
inline ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>> end (_ARG &&arg) {
	_STATIC_ASSERT_ (std::is_lvalue_reference<_ARG &&>::value) ;
	return ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>>::friend_end (arg) ;
}
} ;

namespace U {
template <class ,class>
struct OPERATOR_SORT ;

template <>
struct OPERATOR_SORT<void ,void> {
	template <class _ARG1 ,class _ARG2>
	inline static void invoke (const _ARG1 &array ,_ARG2 &out ,INDEX ib ,INDEX ie) {
		OPERATOR_SORT<_ARG1 ,_ARG2>::invoke (array ,out ,ib ,ie) ;
	}

	class ForwardArray :private Wrapped<ARGV<NONE>> {
	public:
		template <class _ARG>
		inline _ARG &operator[] (_ARG &arg) const {
			return arg ;
		}
	} ;

	template <class _ARG>
	inline static void invoke (_ARG &out ,INDEX ib ,INDEX ie) {
		OPERATOR_SORT<ForwardArray ,_ARG>::invoke (_CAST_<ForwardArray> (ARGV_NONE) ,out ,ib ,ie) ;
	}
} ;

template <class TYPE1 ,class TYPE2>
struct OPERATOR_SORT {
	template <class _ARG1 ,class _ARG2>
	inline static void insert_sort (const _ARG1 &array ,_ARG2 &out ,INDEX ib ,INDEX jb) {
		for (INDEX i = ib + 1 ; i <= jb ; i++) {
			INDEX iw = i ;
			auto rax = std::move (out[iw]) ;
			while (iw - 1 >= ib && array[rax] < array[out[iw - 1]]) {
				out[iw] = std::move (out[iw - 1]) ;
				iw-- ;
			}
			out[iw] = std::move (rax) ;
		}
	}

	template <class _ARG1 ,class _ARG2>
	inline static void quick_sort_partition (const _ARG1 &array ,_ARG2 &out ,INDEX ib ,INDEX jb ,INDEX &it) {
		INDEX iw = ib ;
		INDEX jw = jb ;
		auto rax = std::move (out[iw]) ;
		while (TRUE) {
			while (iw < jw && array[rax] < array[out[jw]])
				jw-- ;
			if (iw >= jw)
				break ;
			out[iw++] = std::move (out[jw]) ;
			while (iw < jw && array[rax] > array[out[iw]])
				iw++ ;
			if (iw >= jw)
				break ;
			out[jw--] = std::move (out[iw]) ;
		}
		out[iw] = std::move (rax) ;
		it = iw ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void quick_sort (const _ARG1 &array ,_ARG2 &out ,INDEX ib ,INDEX jb ,LENGTH ideal) {
		INDEX iw = ib ;
		while (iw < jb && ideal > 0) {
			ideal = ideal / 2 + ideal / 4 ;
			INDEX ir = VAR_NONE ;
			quick_sort_partition (array ,out ,iw ,jb ,ir) ;
			quick_sort (array ,out ,iw ,(ir - 1) ,ideal) ;
			iw = ir + 1 ;
		}
		if (iw >= jb)
			return ;
		insert_sort (array ,out ,iw ,jb) ;
	}

	inline static void invoke (const TYPE1 &array ,TYPE2 &out ,INDEX ib ,INDEX ie) {
		quick_sort (array ,out ,ib ,(ie - 1) ,ie) ;
	}
} ;
} ;

namespace U {
template <class TYPE>
struct OPERATOR_HASH {
	template <class _ARG>
	inline static FLAG template_hash (const _ARG &left ,const ARGV<ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().hash ()) ,FLAG>::value>> & ,const decltype (ARGVP2) &) {
		return left.hash () ;
	}

	template <class _ARG>
	inline static FLAG template_hash (const _ARG &left ,const ARGV<ENABLE_TYPE<std::is_pod<_ARG>::value>> & ,const decltype (ARGVP1) &) {
		return _MEMHASH_ (_CAST_<BYTE[_SIZEOF_ (_ARG)]> (left)) ;
	}

	inline static FLAG invoke (const TYPE &left) {
		FLAG ret = template_hash (left ,ARGV_VOID ,ARGVP9) ;
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

	implicit Array (const std::initializer_list<ITEM> &right) : Array (right.size ()) {
		_DEBUG_ASSERT_ (size () == LENGTH (right.size ())) ;
		INDEX iw = 0 ;
		for (auto &&i : right)
			mArray[iw++] = i ;
	}

	LENGTH size () const {
		return mArray.size () ;
	}

	LENGTH length () const {
		return size () ;
	}

	PhanBuffer<ITEM> raw () {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<ITEM>::make (mArray ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mArray ,length ()) ;
	}

	ITEM &get (INDEX index) {
		return mArray[index] ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const {
		return mArray[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mArray.at (item) ;
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

	BOOL equal (const Array &right) const {
		return mArray == right.mArray ;
	}

	inline BOOL operator== (const Array &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Array &right) const {
		return !equal (right) ;
	}

	BOOL less (const Array &right) const {
		return mArray < right.mArray ;
	}

	inline BOOL operator< (const Array &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Array &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Array &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Array &right) const {
		return !right.less (*this) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	void sort () {
		U::OPERATOR_SORT<void ,void>::invoke (mArray ,0 ,mArray.size ()) ;
	}

	void fill (const ITEM &val) {
		for (INDEX i = 0 ; i < mArray.size () ; i++)
			mArray[i] = val ;
	}

	template <class _ARG>
	void fill (const Array<ITEM ,_ARG> &val) {
		INDEX ir = 0 ;
		for (INDEX i = 0 ; i < mArray.size () ; i++) {
			mArray[i] = val[ir] ;
			ir = (ir + 1) % val.size () ;
		}
	}
} ;

template <class TYPE>
using ARRAY1 = Array<TYPE ,ARGC<1>> ;
template <class TYPE>
using ARRAY2 = Array<TYPE ,ARGC<2>> ;
template <class TYPE>
using ARRAY3 = Array<TYPE ,ARGC<3>> ;
template <class TYPE>
using ARRAY4 = Array<TYPE ,ARGC<4>> ;
template <class TYPE>
using ARRAY5 = Array<TYPE ,ARGC<5>> ;
template <class TYPE>
using ARRAY6 = Array<TYPE ,ARGC<6>> ;
template <class TYPE>
using ARRAY7 = Array<TYPE ,ARGC<7>> ;
template <class TYPE>
using ARRAY8 = Array<TYPE ,ARGC<8>> ;
template <class TYPE>
using ARRAY9 = Array<TYPE ,ARGC<9>> ;
template <class TYPE>
using ARRAY10 = Array<TYPE ,ARGC<10>> ;
template <class TYPE>
using ARRAY11 = Array<TYPE ,ARGC<11>> ;
template <class TYPE>
using ARRAY12 = Array<TYPE ,ARGC<12>> ;
template <class TYPE>
using ARRAY13 = Array<TYPE ,ARGC<13>> ;
template <class TYPE>
using ARRAY14 = Array<TYPE ,ARGC<14>> ;
template <class TYPE>
using ARRAY15 = Array<TYPE ,ARGC<15>> ;
template <class TYPE>
using ARRAY16 = Array<TYPE ,ARGC<16>> ;

template <class ITEM ,class SIZE = SAUTO>
class String ;

template <class ITEM ,class SIZE>
class String {
private:
	inline static constexpr LENGTH expr_size (LENGTH len) {
		return (len > 0) ? (len + 1) : len ;
	}

private:
	Buffer<ITEM ,ARGC<expr_size (SIZE::value)>> mString ;

public:
	String () {
		clear () ;
	}

	explicit String (LENGTH len) :String (expr_size (len) ,ARGVP0) {
		clear () ;
	}

	implicit String (const std::initializer_list<ITEM> &right) : String (right.size ()) {
		INDEX iw = 0 ;
		for (auto &&i : right)
			mString[iw++] = i ;
		mString[iw] = ITEM (0) ;
	}

	implicit String (const ARR<ITEM> &right) :String (_MEMCHR_ (right ,DEFAULT_HUGEBUFFER_SIZE::value ,ITEM (0))) {
		_MEMCOPY_ (mString.self ,right ,size ()) ;
	}

	template <LENGTH _VAL>
	implicit String (const DEF<ITEM[_VAL]> &right) : String (_VAL - 1) {
		_MEMCOPY_ (mString.self ,PTRTOARR[&right[0]] ,size ()) ;
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

	PhanBuffer<ITEM> raw () {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<ITEM>::make (mString ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mString ,length ()) ;
	}

	void clear () {
		if (mString.size () == 0)
			return ;
		mString[0] = ITEM (0) ;
		mString[size ()] = ITEM (0) ;
	}

	ITEM &get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mString[index] ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mString[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mString.at (item) ;
		if (ret < 0 || ret >= length ())
			ret = VAR_NONE ;
		return std::move (ret) ;
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

	BOOL equal (const String &right) const {
		if (size () == 0 || right.size () == 0)
			return size () == 0 && right.size () == 0 ;
		INDEX ir = 0 ;
		while (mString[ir] != ITEM (0) && mString[ir] == right.mString[ir])
			ir++ ;
		return mString[ir] == right.mString[ir] ;
	}

	inline BOOL operator== (const String &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const String &right) const {
		return !equal (right) ;
	}

	BOOL less (const String &right) const {
		if (size () == 0 || right.size () == 0)
			return right.size () != 0 ;
		INDEX ir = 0 ;
		while (mString[ir] != ITEM (0) && mString[ir] == right.mString[ir])
			ir++ ;
		return mString[ir] < right.mString[ir] ;
	}

	inline BOOL operator< (const String &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const String &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const String &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const String &right) const {
		return !right.less (*this) ;
	}

	BOOL empty () const {
		return mString.size () == 0 || mString[0] == ITEM (0) ;
	}

	String add (const String &right) const {
		const auto r1x = length () ;
		const auto r2x = right.length () ;
		String ret = String (r1x + r2x) ;
		_MEMCOPY_ (ret.mString.self ,mString.self ,r1x) ;
		_MEMCOPY_ (PTRTOARR[&ret.mString.self[r1x]] ,right.mString.self ,r2x) ;
		return std::move (ret) ;
	}

	inline String operator+ (const String &right) const {
		return add (right) ;
	}

	template <LENGTH _VAL>
	void addto (const DEF<ITEM[_VAL]> &right) {
		const auto r1x = length () ;
		const auto r2x = _VAL - 1 ;
		if (r1x + r2x <= size ()) {
			_MEMCOPY_ (PTRTOARR[&mString.self[r1x]] ,PTRTOARR[&right[0]] ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		} else {
			*this = add (right) ;
		}
	}

	template <LENGTH _VAL>
	inline String &operator+= (const DEF<ITEM[_VAL]> &right) {
		addto (right) ;
		return *this ;
	}

	void addto (const String &right) {
		const auto r1x = length () ;
		const auto r2x = right.length () ;
		if (r1x + r2x <= size () && mString.size () > 0) {
			_MEMCOPY_ (PTRTOARR[&mString.self[r1x]] ,right.mString.self ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		} else {
			*this = add (right) ;
		}
	}

	inline String &operator+= (const String &right) {
		addto (right) ;
		return *this ;
	}

	String segment (INDEX ib ,INDEX ie) const {
		_DEBUG_ASSERT_ (ib >= 0 && ib <= ie && ie <= size ()) ;
		String ret = String (ie - ib) ;
		for (INDEX i = 0 ; i < ret.size () ; i++)
			ret.get (i) = get (ib + i) ;
		return std::move (ret) ;
	}

private:
	explicit String (LENGTH len ,const decltype (ARGVP0) &) :mString (len) {}

public:
	//@info: the function is incompleted without 'csc_string.hpp'
	template <class... _ARGS>
	inline static String make (const _ARGS &...args) ;
} ;

template <class ITEM ,class SIZE = SAUTO>
class Stack ;

template <class ITEM ,class SIZE>
class Stack {
private:
	class AccessArray :private Wrapped<Stack> {
	public:
		inline ITEM &operator[] (INDEX index) {
			return AccessArray::mData[AccessArray::mData.access (index)] ;
		}

		inline const ITEM &operator[] (INDEX index) const {
			return AccessArray::mData[AccessArray::mData.access (index)] ;
		}
	} ;

	inline static constexpr LENGTH expr_size (LENGTH len) {
		return (len > 0) ? (len + 1) : len ;
	}

private:
	Buffer<ITEM ,ARGC<expr_size (SIZE::value)>> mStack ;
	INDEX mWrite ;

public:
	Stack () {
		clear () ;
	}

	explicit Stack (LENGTH len) :Stack (expr_size (len) ,ARGVP0) {
		clear () ;
	}

	implicit Stack (const std::initializer_list<ITEM> &right) : Stack (right.size ()) {
		for (auto &&i : right)
			add (i) ;
	}

	LENGTH size () const {
		if (mStack.size () == 0)
			return 0 ;
		return mStack.size () - 1 ;
	}

	LENGTH length () const {
		if (mStack.size () == 0)
			return 0 ;
		return mWrite ;
	}

	void clear () {
		mWrite = 0 ;
	}

	ITEM &get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return mStack[index] ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return mStack[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mStack.at (item) ;
		if (ret < 0 || ret >= length ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	INDEX access (INDEX pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		return pos ;
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

	BOOL equal (const Stack &right) const {
		return length () == right.length () && _MEMEQUAL_ (mStack ,right.mStack ,length ()) ;
	}

	inline BOOL operator== (const Stack &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Stack &right) const {
		return !equal (right) ;
	}

	BOOL empty () const {
		return mWrite == 0 ;
	}

	BOOL full () const {
		return mWrite == size () ;
	}

	void add (const ITEM &item) {
		reserve (1) ;
		mStack[mWrite] = std::move (item) ;
		update_resize () ;
	}

	inline Stack &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		reserve (1) ;
		mStack[mWrite] = std::move (item) ;
		update_resize () ;
	}

	inline Stack &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	void add_sort (const ITEM &item) {
		reserve (1) ;
		INDEX iw = mWrite ;
		while (iw - 1 >= 0 && mStack[iw - 1] < item) {
			mStack[iw] = std::move (mStack[iw - 1]) ;
			iw-- ;
		}
		mStack[iw] = std::move (item) ;
		update_resize () ;
	}

	void add_sort (ITEM &&item) {
		reserve (1) ;
		INDEX iw = mWrite ;
		while (iw - 1 >= 0 && mStack[iw - 1] < item) {
			mStack[iw] = std::move (mStack[iw - 1]) ;
			iw-- ;
		}
		mStack[iw] = std::move (item) ;
		update_resize () ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite-- ;
	}

	void take (ITEM &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mStack[mWrite - 1]) ;
		mWrite-- ;
	}

	inline Stack &operator>> (ITEM &item) popping {
		take (item) ;
		return *this ;
	}

	INDEX peek () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mWrite - 1 ;
	}

	INDEX insert () popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		update_resize () ;
		return std::move (ret) ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite-- ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		U::OPERATOR_SORT<void ,void>::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		U::OPERATOR_SORT<void ,void>::invoke (_CAST_<AccessArray> (*this) ,0 ,length ()) ;
	}

private:
	explicit Stack (LENGTH len ,const decltype (ARGVP0) &) :mStack (len) {}

private:
	void reserve (LENGTH len) {
		const auto r1x = _MAX_ (len - (mStack.size () - length ()) ,VAR_ZERO) ;
		if (r1x == 0)
			return ;
		auto rax = mStack.expand (mStack.size () + r1x) ;
		_MEMMOVE_ (rax.self ,mStack.self ,mStack.size ()) ;
		mStack.expand (std::move (rax)) ;
	}

	void update_resize () {
		_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mStack.size ()) ;
		mWrite++ ;
		if (mWrite < mStack.size ())
			return ;
		auto rax = mStack.expand () ;
		_MEMMOVE_ (rax.self ,mStack.self ,mStack.size ()) ;
		mStack.expand (std::move (rax)) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Queue ;

template <class ITEM ,class SIZE>
class Queue {
private:
	class AccessArray :private Wrapped<Queue> {
	public:
		inline ITEM &operator[] (INDEX index) {
			return AccessArray::mData[AccessArray::mData.access (index)] ;
		}

		inline const ITEM &operator[] (INDEX index) const {
			return AccessArray::mData[AccessArray::mData.access (index)] ;
		}
	} ;

	inline static constexpr LENGTH expr_size (LENGTH len) {
		return (len > 0) ? (len + 1) : len ;
	}

private:
	Buffer<ITEM ,ARGC<expr_size (SIZE::value)>> mQueue ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	Queue () {
		clear () ;
	}

	explicit Queue (LENGTH len) :Queue (expr_size (len) ,ARGVP0) {
		clear () ;
	}

	implicit Queue (const std::initializer_list<ITEM> &right) : Queue (right.size ()) {
		for (auto &&i : right)
			add (i) ;
	}

	LENGTH size () const {
		if (mQueue.size () == 0)
			return 0 ;
		return mQueue.size () - 1 ;
	}

	LENGTH length () const {
		if (mQueue.size () == 0)
			return 0 ;
		return (mWrite - mRead + mQueue.size ()) % mQueue.size () ;
	}

	void clear () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	//@warn: index would be no longer valid once resized
	ITEM &get (INDEX index) {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mQueue[index] ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	//@warn: index would be no longer valid once resized
	const ITEM &get (INDEX index) const {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mQueue[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mQueue.at (item) ;
		if (!ensure_index (ret))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	INDEX access (INDEX pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		return (mRead + pos) % mQueue.size () ;
	}

	INDEX ibegin () const {
		if (mQueue.size () == 0)
			return VAR_NONE ;
		return mRead ;
	}

	INDEX iend () const {
		if (mQueue.size () == 0)
			return VAR_NONE ;
		return mWrite ;
	}

	INDEX inext (INDEX index) const {
		return (index + 1) % mQueue.size () ;
	}

	BOOL equal (const Queue &right) const {
		if (length () != right.length ())
			return FALSE ;
		INDEX ir = ibegin () ;
		INDEX ie = iend () ;
		INDEX jr = right.ibegin () ;
		INDEX je = right.iend () ;
		while (ir != ie && jr != je && get (ir) == right.get (jr)) {
			ir = inext (ir) ;
			jr = right.inext (jr) ;
		}
		return ir == ie && jr == je ;
	}

	inline BOOL operator== (const Queue &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Queue &right) const {
		return !equal (right) ;
	}

	BOOL empty () const {
		return mRead == mWrite ;
	}

	BOOL full () const {
		return mQueue.size () == 0 || mRead == (mWrite + 1) % mQueue.size () ;
	}

	void add (const ITEM &item) {
		reserve (1) ;
		mQueue[mWrite] = std::move (item) ;
		update_resize () ;
	}

	inline Queue &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		reserve (1) ;
		mQueue[mWrite] = std::move (item) ;
		update_resize () ;
	}

	inline Queue &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mRead = (mRead + 1) % mQueue.size () ;
	}

	void take (ITEM &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mQueue[mRead]) ;
		mRead = (mRead + 1) % mQueue.size () ;
	}

	inline Queue &operator>> (ITEM &item) popping {
		take (item) ;
		return *this ;
	}

	INDEX peek () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mRead ;
	}

	INDEX insert () popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		update_resize () ;
		return std::move (ret) ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite = (mWrite - 1 + mQueue.size ()) % mQueue.size () ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		U::OPERATOR_SORT<void ,void>::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		U::OPERATOR_SORT<void ,void>::invoke (_CAST_<AccessArray> (*this) ,0 ,length ()) ;
	}

private:
	explicit Queue (LENGTH len ,const decltype (ARGVP0) &) :mQueue (len) {}

private:
	BOOL ensure_index (INDEX index) const {
		if (mRead <= mWrite && !(index >= mRead && index < mWrite))
			return FALSE ;
		if (mRead > mWrite && !(index >= 0 && index < mWrite) && !(index >= mRead && index < mQueue.size ()))
			return FALSE ;
		return TRUE ;
	}

	void reserve (LENGTH len) {
		const auto r1x = _MAX_ (len - (mQueue.size () - length ()) ,VAR_ZERO) ;
		if (r1x == 0)
			return ;
		if (mRead <= mWrite) {
			auto rax = mQueue.expand (mQueue.size () + r1x) ;
			_MEMMOVE_ (PTRTOARR[&rax.self[mRead]] ,PTRTOARR[&mQueue.self[mRead]] ,(mWrite - mRead)) ;
			mQueue.expand (std::move (rax)) ;
		} else {
			auto rax = mQueue.expand (mQueue.size () + r1x) ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			INDEX ix = mRead + rax.size () - mQueue.size () ;
			_MEMMOVE_ (PTRTOARR[&rax.self[ix]] ,PTRTOARR[&mQueue.self[mRead]] ,(mQueue.size () - mRead)) ;
			mQueue.expand (std::move (rax)) ;
			mRead = ix ;
		}
	}

	void update_resize () {
		_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mQueue.size ()) ;
		mWrite = (mWrite + 1) % mQueue.size () ;
		if (mRead != mWrite)
			return ;
		auto rax = mQueue.expand () ;
		_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
		INDEX ix = (mRead > 0) ? (mRead + rax.size () - mQueue.size ()) : 0 ;
		INDEX iy = (mRead > 0) ? mWrite : (mQueue.size ()) ;
		_MEMMOVE_ (PTRTOARR[&rax.self[ix]] ,PTRTOARR[&mQueue.self[mRead]] ,(mQueue.size () - mRead)) ;
		mQueue.expand (std::move (rax)) ;
		mRead = ix ;
		mWrite = iy ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class Priority ;

template <class KEY ,class ITEM ,class SIZE>
class Priority<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_TYPE = Priority<KEY ,ITEM ,SIZE> ;

	class Node {
	private:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
		ITEM mItem ;
	} ;

	using NodePair = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair {
	public:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mPriority[index].mKey) ,item (base.mPriority[index].mItem) {}
	} ;

	inline static constexpr LENGTH expr_size (LENGTH len) {
		return (len > 0) ? (len + 1) : len ;
	}

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,ARGC<expr_size (SIZE::value)>> mPriority ;
	INDEX mWrite ;

public:
	Priority () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	explicit Priority (LENGTH len) :Priority (expr_size (len) ,ARGVP0) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		mPriority[ix].mItem = std::move (item) ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize () ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
	}

	void add (const NodePair &item) {
		add (item.key ,std::move (item)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		mPriority[ix].mItem = std::move (item) ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize () ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

private:
	explicit Priority (LENGTH len ,const decltype (ARGVP0) &) :mPriority (len) {}
} ;

template <class KEY ,class SIZE>
class Priority<KEY ,SPECIALIZATION<void> ,SIZE> {
private:
	using SPECIALIZATION_TYPE = Priority<KEY ,void ,SIZE> ;

	class Node {
	private:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		KEY mKey ;
	} ;

	using NodePair = PACK<KEY> ;

	template <class BASE>
	class Pair {
	public:
		friend Priority ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mPriority[index].mKey) {}
	} ;

	inline static constexpr LENGTH expr_size (LENGTH len) {
		return (len > 0) ? (len + 1) : len ;
	}

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,ARGC<expr_size (SIZE::value)>> mPriority ;
	INDEX mWrite ;

public:
	Priority () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	explicit Priority (LENGTH len) :Priority (expr_size (len) ,ARGVP0) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	void add (const KEY &key) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize () ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (1) ;
		INDEX ix = mWrite ;
		mPriority[ix].mKey = std::move (key) ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize () ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

private:
	explicit Priority (LENGTH len ,const decltype (ARGVP0) &) :mPriority (len) {}
} ;

template <class KEY ,class ITEM ,class SIZE>
class Priority :private Priority<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Priority<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using NodePair = typename SPECIALIZATION_BASE::NodePair ;
	template <class _ARG>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mPriority ;
	using SPECIALIZATION_BASE::mWrite ;

public:
	Priority () = default ;

	explicit Priority (LENGTH len) :SPECIALIZATION_BASE (len) {}

	implicit Priority (const std::initializer_list<NodePair> &right) : Priority (right.size ()) {
		for (auto &&i : right)
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

	//@warn: index would be no longer valid every time revised
	Pair<Priority> get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return Pair<Priority> (*this ,index) ;
	}

	inline Pair<Priority> operator[] (INDEX index) {
		return get (index) ;
	}

	//@warn: index would be no longer valid every time revised
	Pair<const Priority> get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return Pair<const Priority> (*this ,index) ;
	}

	inline Pair<const Priority> operator[] (INDEX index) const {
		return get (index) ;
	}

#ifdef __CSC_COMPILER_MSVC__
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<REMOVE_CONST_TYPE<_ARG> ,Priority>::value>>
	INDEX at (const Pair<_ARG> &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&Node::mKey ,item.key)) ;
		if (ret < 0 || ret >= length ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}
#else
	INDEX at (const Pair<Priority> &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&Node::mKey ,item.key)) ;
		if (ret < 0 || ret >= length ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	INDEX at (const Pair<const Priority> &item) const {
		INDEX ret = mPriority.at (_OFFSET_ (&Node::mKey ,item.key)) ;
		if (ret < 0 || ret >= length ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}
#endif

	INDEX ibegin () const {
		return 0 ;
	}

	INDEX iend () const {
		return length () ;
	}

	INDEX inext (INDEX index) const {
		return index + 1 ;
	}

	BOOL equal (const Priority &right) const {
		return length () == right.length () && _MEMEQUAL_ (mPriority ,right.mPriority ,length ()) ;
	}

	inline BOOL operator== (const Priority &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Priority &right) const {
		return !equal (right) ;
	}

	BOOL empty () const {
		return mWrite == 0 ;
	}

	BOOL full () const {
		return mWrite == size () ;
	}

	using SPECIALIZATION_BASE::add ;

	inline Priority &operator<< (const NodePair &item) {
		add (std::move (item)) ;
		return *this ;
	}

	inline Priority &operator<< (NodePair &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	using SPECIALIZATION_BASE::appand ;

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mPriority[0] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (0) ;
	}

	void take (NodePair &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mPriority[0]) ;
		mPriority[0] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (0) ;
	}

	inline Priority &operator>> (NodePair &item) popping {
		take (item) ;
		return *this ;
	}

	INDEX peek () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return 0 ;
	}

	INDEX insert (const KEY &key) popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		update_resize () ;
		update_insert (ret) ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		reserve (1) ;
		INDEX ret = mWrite ;
		update_resize () ;
		update_insert (ret) ;
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		mPriority[index] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update_insert (index) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		while (iw >= 2) {
			iw-- ;
			_SWAP_ (ret[0] ,ret[iw]) ;
			compute_esort (ret ,iw) ;
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
		mPriority.expand (std::move (rax)) ;
	}

	void update_resize () {
		_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mPriority.size ()) ;
		mWrite++ ;
		if (mWrite < mPriority.size ())
			return ;
		auto rax = mPriority.expand () ;
		_MEMMOVE_ (rax.self ,mPriority.self ,mPriority.size ()) ;
		mPriority.expand (std::move (rax)) ;
	}

	void update_insert (INDEX it) {
		INDEX iw = it ;
		auto rax = std::move (mPriority[iw]) ;
		while (TRUE) {
			//@info: '(0 - 1) >> 1' is not the same as '(0 - 1) / 2'
			INDEX ix = (iw - 1) >> 1 ;
			if (ix < 0)
				break ;
			if (rax.mKey >= mPriority[ix].mKey)
				break ;
			mPriority[iw] = std::move (mPriority[ix]) ;
			iw = ix ;
		}
		while (TRUE) {
			INDEX ix = iw * 2 + 1 ;
			if (ix >= mWrite)
				break ;
			INDEX iy = iw ;
			if (rax.mKey > mPriority[ix].mKey)
				iy = ix ;
			ix = ix + 1 ;
			auto &r1 = (iy != iw) ? (mPriority[iy].mKey) : (rax.mKey) ;
			if (ix < mWrite && r1 > mPriority[ix].mKey)
				iy = ix ;
			if (iy == iw)
				break ;
			mPriority[iw] = std::move (mPriority[iy]) ;
			iw = iy ;
		}
		mPriority[iw] = std::move (rax) ;
	}

	void compute_esort (Array<INDEX> &out ,LENGTH len) const {
		INDEX iw = 0 ;
		const auto r1x = out[iw] ;
		while (TRUE) {
			INDEX ix = iw * 2 + 1 ;
			if (ix >= len)
				break ;
			INDEX iy = iw ;
			if (mPriority[r1x].mKey > mPriority[out[ix]].mKey)
				iy = ix ;
			ix = ix + 1 ;
			auto &r1 = (iy != iw) ? (mPriority[out[iy]].mKey) : (mPriority[r1x].mKey) ;
			if (ix < len && r1 > mPriority[out[ix]].mKey)
				iy = ix ;
			if (iy == iw)
				break ;
			out[iw] = out[iy] ;
			iw = iy ;
		}
		out[iw] = r1x ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Deque ;

template <class ITEM ,class SIZE>
class Deque {
private:
	class Node {
	private:
		friend Deque ;
		ITEM mItem ;
		INDEX mHead ;

	public:
		inline Node () = delete ;

		inline explicit Node (INDEX head) :mHead (head) {}

		inline explicit Node (const ITEM &item ,INDEX head) : mItem (std::move (item)) ,mHead (head) {}

		inline explicit Node (ITEM &&item ,INDEX head) : mItem (std::move (item)) ,mHead (head) {}
	} ;

private:
	Allocator<Node ,SIZE> mList ;
	Buffer<Buffer<INDEX ,ARGC<2>> ,SIZE> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	Deque () {
		clear () ;
	}

	explicit Deque (LENGTH len) :Deque (len ,ARGVP0) {
		clear () ;
	}

	implicit Deque (const std::initializer_list<ITEM> &right) : Deque (right.size ()) {
		for (auto &&i : right)
			add (i) ;
	}

	LENGTH size () const {
		return mList.size () ;
	}

	LENGTH length () const {
		return mList.length () ;
	}

	void clear () {
		const auto r1x = Buffer<INDEX ,ARGC<2>> ({VAR_NONE ,VAR_ZERO}) ;
		_MEMFILL_ (mDeque.self ,mDeque.size () ,r1x) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	ITEM &get (INDEX index) {
		return mList[index].mItem ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const {
		return mList[index].mItem ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mList.at (_OFFSET_ (&Node::mItem ,item)) ;
	}

	INDEX access (INDEX pos) const {
		_DEBUG_ASSERT_ (pos >= 0 && pos < length ()) ;
		if (mWrite - mRead + 1 == mList.length ())
			return mDeque[mRead + pos][0] ;
		if (mWrite - mRead == mList.length () && mDeque[mWrite][0] == VAR_NONE)
			return mDeque[mRead + pos][0] ;
		if (mWrite - mRead == mList.length () && mDeque[mRead][0] == VAR_NONE)
			return mDeque[mRead + pos + 1][0] ;
		return access (pos ,mRead ,mWrite) ;
	}

	INDEX ibegin () const {
		for (INDEX i = mRead ,ie = _MIN_ ((mWrite + 1) ,mDeque.size ()) ; i < ie ; i++)
			if (mDeque[i][0] != VAR_NONE)
				return mDeque[i][0] ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = mList[index].mHead + 1 ; i <= mWrite ; i++)
			if (mDeque[i][0] != VAR_NONE)
				return mDeque[i][0] ;
		return VAR_NONE ;
	}

	BOOL equal (const Deque &right) const {
		if (length () != right.length ())
			return FALSE ;
		INDEX ir = ibegin () ;
		INDEX ie = iend () ;
		INDEX jr = right.ibegin () ;
		INDEX je = right.iend () ;
		while (ir != ie && jr != je && get (ir) == right.get (jr)) {
			ir = inext (ir) ;
			jr = right.inext (jr) ;
		}
		return ir == ie && jr == je ;
	}

	inline BOOL operator== (const Deque &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Deque &right) const {
		return !equal (right) ;
	}

	BOOL empty () const {
		return mList.length () == 0 ;
	}

	BOOL full () const {
		return mList.length () == mList.size () ;
	}

	void add (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ix) ;
		update_compress_left (mWrite ,ix) ;
	}

	inline Deque &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ix) ;
		update_compress_left (mWrite ,ix) ;
	}

	inline Deque &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		mList.reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		mList.reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mRead][0] == VAR_NONE)
			mRead++ ;
		INDEX ix = mDeque[mRead][0] ;
		update_remove (ix) ;
		mList.free (ix) ;
	}

	void take (ITEM &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mRead][0] == VAR_NONE)
			mRead++ ;
		INDEX ix = mDeque[mRead][0] ;
		item = std::move (mList[ix].mItem) ;
		update_remove (mRead) ;
		mList.free (ix) ;
	}

	inline Deque &operator>> (ITEM &item) popping {
		take (item) ;
		return *this ;
	}

	INDEX insert () popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		update_resize (ret) ;
		update_compress_left (mWrite ,ret) ;
		return std::move (ret) ;
	}

	INDEX insert (INDEX index) popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		update_resize (ret) ;
		const auto r1x = (index != VAR_NONE) ? (mList[index].mHead) : mWrite ;
		update_compress_left (r1x ,ret) ;
		return std::move (ret) ;
	}

	void swap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		update_rewrite (mList[index1].mHead ,index2) ;
		update_rewrite (mList[index2].mHead ,index1) ;
	}

	void splice (INDEX index ,INDEX ib ,INDEX jb) {
		INDEX ix = mList[index].mHead ;
		INDEX jx = mList[ib].mHead ;
		INDEX jy = mList[jb].mHead ;
		_DEBUG_ASSERT_ (jx <= jy) ;
		_DEBUG_ASSERT_ (ix < jx || ix > jy) ;
		if (position_before (index) - 1 == position_before (jb))
			return ;
		update_compress_splice_force (ix ,ib ,(ib + jy - jx + 1)) ;
	}

	void remove (INDEX index) {
		update_remove (mList[index].mHead) ;
		mList.free (index) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		U::OPERATOR_SORT<void ,void>::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		for (INDEX i = 0 ; i < r1x.length () ; i++)
			update_rewrite (i ,r1x[i]) ;
		for (INDEX i = r1x.length () ; i < mDeque.size () ; i++)
			update_remove (i) ;
		mRead = 0 ;
		mWrite = _MAX_ ((r1x.length () - 1) ,VAR_ZERO) ;
	}

private:
	explicit Deque (LENGTH len ,const decltype (ARGVP0) &) :mList (len) ,mDeque (len) {}

private:
	INDEX access (INDEX pos ,INDEX ib ,INDEX jb) const {
		INDEX ret = VAR_NONE ;
		while (TRUE) {
			if (ib > jb)
				break ;
			ret = ib + (jb - ib + 1) / 2 ;
			INDEX ix = position_before (ret) ;
			if (ix == pos && mDeque[ret][0] != VAR_NONE)
				break ;
			const auto r1x = (ix < pos) ? (ret + 1) : (ret - 1) ;
			auto &r1 = (ix < pos) ? ib : jb ;
			r1 = r1x ;
		}
		ret = mDeque[ret][0] ;
		return std::move (ret) ;
	}

	INDEX position_before (INDEX it) const {
		INDEX ret = 0 ;
		INDEX iw = it ;
		while (iw >= 0) {
			ret += mDeque[iw][1] ;
			iw -= (iw + 1) & -(iw + 1) ;
		}
		ret-- ;
		return std::move (ret) ;
	}

	void update_resize (INDEX it) {
		if (mDeque.size () == mList.size ())
			return ;
		auto rax = mDeque.expand (mList.size ()) ;
		const auto r1x = Buffer<INDEX ,ARGC<2>> ({VAR_NONE ,VAR_ZERO}) ;
		_MEMFILL_ (rax.self ,rax.size () ,r1x) ;
		for (INDEX i = 0 ; i < mList.size () ; i++) {
			if (i == it)
				continue ;
			if (!mList.used (i))
				continue ;
			update_rewrite (mList[i].mHead ,i) ;
		}
		mDeque.expand (std::move (rax)) ;
	}

	void update_compress_left (INDEX it ,INDEX jt) {
		if (mDeque[it][0] == VAR_NONE) {
			update_rewrite (it ,jt) ;
			mWrite = _MIN_ ((it + 1) ,(mDeque.size () - 1)) ;
		} else if (it + 1 < mDeque.size () && mDeque[it + 1][0] == VAR_NONE) {
			update_rewrite ((it + 1) ,jt) ;
			mWrite = _MIN_ ((it + 2) ,(mDeque.size () - 1)) ;
		} else {
			update_compress_left_force (it ,jt) ;
		}
	}

	void update_compress_left_force (INDEX it ,INDEX jt) {
		INDEX iw = it ;
		INDEX jw = jt ;
		for (INDEX i = 0 ; i < mList.length () ; i++) {
			while (mRead != iw && mDeque[mRead][0] == VAR_NONE)
				mRead++ ;
			const auto r1x = mDeque[i][0] ;
			if (mRead == iw && r1x == VAR_NONE) {
				update_rewrite (i ,jw) ;
				jw = r1x ;
				iw = VAR_NONE ;
			} else if (mRead == iw && r1x != VAR_NONE) {
				update_rewrite (i ,jw) ;
				jw = r1x ;
				iw++ ;
			} else if (mRead != i) {
				update_rewrite (i ,mDeque[mRead][0]) ;
				update_remove (mRead) ;
			}
			mRead++ ;
		}
		mRead = 0 ;
		mWrite = _MIN_ (mList.length () ,(mDeque.size () - 1)) ;
	}

	void update_compress_right (INDEX it ,INDEX jt) {
		if (mDeque[it][0] == VAR_NONE) {
			update_rewrite (it ,jt) ;
			mRead = _MAX_ ((it - 1) ,VAR_ZERO) ;
		} else if (it - 1 >= 0 && mDeque[it - 1][0] == VAR_NONE) {
			update_rewrite ((it - 1) ,jt) ;
			mRead = _MAX_ ((it - 2) ,VAR_ZERO) ;
		} else {
			update_compress_right_force (it ,jt) ;
		}
	}

	void update_compress_right_force (INDEX it ,INDEX jt) {
		INDEX iw = it ;
		INDEX jw = jt ;
		for (INDEX i = (mDeque.size () - 1) ,je = mDeque.size () - mList.length () ; i >= je ; i--) {
			while (mWrite != iw && mDeque[mWrite][0] == VAR_NONE)
				mWrite-- ;
			const auto r1x = mDeque[i][0] ;
			if (mWrite == iw && r1x == VAR_NONE) {
				update_rewrite (i ,jw) ;
				jw = r1x ;
				iw = VAR_NONE ;
			} else if (mWrite == iw && r1x != VAR_NONE) {
				update_rewrite (i ,jw) ;
				jw = r1x ;
				iw-- ;
			} else if (mWrite != i) {
				update_rewrite (i ,mDeque[mWrite][0]) ;
				update_remove (mWrite) ;
			}
			mWrite-- ;
		}
		mRead = _MAX_ (mDeque.size () - 1 - mList.length () ,VAR_ZERO) ;
		mWrite = mDeque.size () - 1 ;
	}

	void update_compress_splice_force (INDEX it ,INDEX ib ,INDEX ie) {
		auto rax = Queue<INDEX> (ie - ib) ;
		for (INDEX i = 0 ; i < rax.size () ; i++) {
			rax.add (mDeque[i + ib][0]) ;
			update_remove (i + ib) ;
		}
		INDEX iw = it ;
		INDEX jw = rax[rax.peek ()] ;
		rax.take () ;
		for (INDEX i = 0 ; i < mList.length () ; i++) {
			while (mRead != iw && mDeque[mRead][0] == VAR_NONE)
				mRead++ ;
			const auto r1x = mDeque[i][0] ;
			if (mRead == iw && rax.empty () && r1x == VAR_NONE) {
				update_rewrite (i ,jw) ;
				iw = VAR_NONE ;
			} else if (mRead == iw && rax.empty () && r1x != VAR_NONE) {
				update_rewrite (i ,jw) ;
				iw = VAR_NONE ;
				rax.add (r1x) ;
			} else if (mRead == iw && !rax.empty () && r1x == VAR_NONE) {
				update_rewrite (i ,jw) ;
				rax.take (jw) ;
				iw++ ;
			} else if (mRead == iw && !rax.empty () && r1x != VAR_NONE) {
				update_rewrite (i ,jw) ;
				rax.take (jw) ;
				iw++ ;
				rax.add (r1x) ;
			} else if (mRead != i) {
				update_rewrite (i ,mDeque[mRead][0]) ;
				update_remove (mRead) ;
			}
			mRead++ ;
		}
		mRead = 0 ;
		mWrite = _MIN_ (mList.length () ,(mDeque.size () - 1)) ;
	}

	void update_rewrite (INDEX it ,INDEX jt) {
		_DEBUG_ASSERT_ (jt != VAR_NONE) ;
		INDEX iw = it ;
		const auto r1x = mDeque[it][0] ;
		mDeque[iw][0] = jt ;
		mList[jt].mHead = iw ;
		if (r1x != VAR_NONE)
			return ;
		while (iw < mDeque.size ()) {
			mDeque[iw][1]++ ;
			iw += (iw + 1) & -(iw + 1) ;
		}
	}

	void update_remove (INDEX it) {
		INDEX iw = it ;
		mDeque[iw][0] = VAR_NONE ;
		while (iw < mDeque.size ()) {
			mDeque[iw][1]-- ;
			iw += (iw + 1) & -(iw + 1) ;
		}
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

	explicit QList (LENGTH len) :QList (len ,ARGVP0) {
		clear () ;
	}

	implicit QList (const std::initializer_list<ITEM> &right) : QList (right.size ()) {
		for (auto &&i : right)
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

	ITEM &get (INDEX index) {
		return mList[index].mItem ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	const ITEM &get (INDEX index) const {
		return mList[index].mItem ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		return mList.at (_OFFSET_ (&Node::mItem ,item)) ;
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

	BOOL equal (const QList &right) const {
		if (length () != right.length ())
			return FALSE ;
		INDEX ir = ibegin () ;
		INDEX ie = iend () ;
		INDEX jr = right.ibegin () ;
		INDEX je = right.iend () ;
		while (ir != ie && jr != je && get (ir) == right.get (jr)) {
			ir = inext (ir) ;
			jr = right.inext (jr) ;
		}
		return ir == ie && jr == je ;
	}

	inline BOOL operator== (const QList &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const QList &right) const {
		return !equal (right) ;
	}

	BOOL empty () const {
		return mFirst == VAR_NONE ;
	}

	BOOL full () const {
		return mList.length () == mList.size () ;
	}

	void add (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,mLast ,VAR_NONE) ;
		auto &r1 = (mLast != VAR_NONE) ? (mList[mLast].mRight) : mFirst ;
		r1 = ix ;
		mLast = ix ;
	}

	inline QList &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,mLast ,VAR_NONE) ;
		auto &r1 = (mLast != VAR_NONE) ? (mList[mLast].mRight) : mFirst ;
		r1 = ix ;
		mLast = ix ;
	}

	inline QList &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		mList.reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		mList.reserve (src.length ()) ;
		for (auto &&i : src)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		mFirst = mList[ix].mRight ;
		auto &r1 = (mFirst != VAR_NONE) ? (mList[mFirst].mLeft) : mLast ;
		r1 = VAR_NONE ;
		mList.free (ix) ;
	}

	void take (ITEM &item) popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		item = std::move (mList[ix].mItem) ;
		mFirst = mList[ix].mRight ;
		auto &r1 = (mFirst != VAR_NONE) ? (mList[mFirst].mLeft) : mLast ;
		r1 = VAR_NONE ;
		mList.free (ix) ;
	}

	inline QList &operator>> (ITEM &item) popping {
		take (item) ;
		return *this ;
	}

	INDEX peek () const {
		_DEBUG_ASSERT_ (!empty ()) ;
		return mFirst ;
	}

	INDEX insert () popping {
		INDEX ret = mList.alloc (mLast ,VAR_NONE) ;
		auto &r1 = (mLast != VAR_NONE) ? (mList[mLast].mRight) : mFirst ;
		r1 = ret ;
		mLast = ret ;
		return std::move (ret) ;
	}

	INDEX insert (INDEX index) popping {
		INDEX ret = mList.alloc (VAR_NONE ,index) ;
		auto &r1 = (index != VAR_NONE) ? (mList[index].mLeft) : mLast ;
		mList[ret].mLeft = r1 ;
		auto &r2 = (r1 != VAR_NONE) ? (mList[r1].mRight) : mFirst ;
		r2 = ret ;
		r1 = ret ;
		return std::move (ret) ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mLast ;
		mLast = mList[ix].mLeft ;
		auto &r1 = (mLast != VAR_NONE) ? (mList[mLast].mRight) : mFirst ;
		r1 = VAR_NONE ;
		mList.free (ix) ;
	}

	void swap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1 = prev_next (index1) ;
		auto &r2 = prev_next (index2) ;
		auto &r3 = next_prev (index1) ;
		auto &r4 = next_prev (index2) ;
		r1 = index2 ;
		r2 = index1 ;
		r3 = index2 ;
		r4 = index1 ;
		_SWAP_ (mList[index1].mLeft ,mList[index2].mLeft) ;
		_SWAP_ (mList[index1].mRight ,mList[index2].mRight) ;
	}

	void splice (INDEX index ,INDEX ib ,INDEX jb) {
		for (INDEX i = ib ,ie = inext (jb) ; i != ie ; i = inext (i)) {
			_DEBUG_ASSERT_ (i != VAR_NONE) ;
			_DEBUG_ASSERT_ (index != i) ;
			(void) i ;
		}
		if (index != VAR_NONE && mList[index].mLeft == jb)
			return ;
		prev_next (ib) = mList[jb].mRight ;
		next_prev (jb) = mList[ib].mLeft ;
		mList[jb].mRight = index ;
		auto &r1 = (index != VAR_NONE) ? (mList[index].mLeft) : mLast ;
		mList[ib].mLeft = r1 ;
		auto &r2 = (r1 != VAR_NONE) ? (mList[r1].mRight) : mFirst ;
		r2 = ib ;
		r1 = jb ;
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
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		U::OPERATOR_SORT<void ,void>::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		for (INDEX i = 1 ; i + 1 < r1x.length () ; i++) {
			mList[r1x[i]].mLeft = r1x[i - 1] ;
			mList[r1x[i]].mRight = r1x[i + 1] ;
		}
		mFirst = r1x[0] ;
		mList[mFirst].mLeft = VAR_NONE ;
		mList[mFirst].mRight = r1x[1] ;
		mLast = r1x[r1x.length () - 1] ;
		mList[mLast].mLeft = r1x[r1x.length () - 2] ;
		mList[mLast].mRight = VAR_NONE ;
	}

private:
	explicit QList (LENGTH len ,const decltype (ARGVP0) &) :mList (len) {}

private:
	INDEX &prev_next (INDEX it) {
		if (mList[it].mLeft == VAR_NONE)
			return mFirst ;
		return mList[mList[it].mLeft].mRight ;
	}

	INDEX &next_prev (INDEX it) {
		if (mList[it].mRight == VAR_NONE)
			return mLast ;
		return mList[mList[it].mRight].mLeft ;
	}
} ;

template <class SIZE = SAUTO>
class BitSet ;

template <class SIZE>
class BitSet {
private:
	inline static constexpr LENGTH expr_size (LENGTH len) {
		return (len > 0) ? ((len + 7) / 8) : len ;
	}

	template <class BASE>
	class Bit {
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
			return (mBase.mSet[mIndex / 8] & (BYTE (0X01) << (mIndex % 8))) != 0 ;
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

		inline void operator= (const BOOL &right) && {
			const auto r1x = right ? (~mBase.mSet[mIndex / 8]) : (mBase.mSet[mIndex / 8]) ;
			const auto r2x = BYTE (r1x & (BYTE (0X01) << (mIndex % 8))) ;
			mBase.mSet[mIndex / 8] = BYTE (mBase.mSet[mIndex / 8] ^ r2x) ;
		}

		inline void operator= (Bit<BitSet> &&right) && {
			std::move (*this) = BOOL (std::move (right)) ;
		}

		inline void operator= (Bit<const BitSet> &&right) && {
			std::move (*this) = BOOL (std::move (right)) ;
		}

	private:
		inline explicit Bit (BASE &base ,INDEX index) :mBase (base) ,mIndex (index) {}
	} ;

private:
	Buffer<BYTE ,ARGC<expr_size (SIZE::value)>> mSet ;
	LENGTH mWidth ;

public:
	BitSet () :BitSet (0 ,0 ,ARGVP0) {
		clear () ;
	}

	explicit BitSet (LENGTH len) :BitSet (expr_size (len) ,expr_width (len) ,ARGVP0) {
		clear () ;
	}

	implicit BitSet (const std::initializer_list<INDEX> &right) : BitSet (expr_size (right)) {
		for (auto &&i : right)
			get (i) = TRUE ;
	}

	LENGTH size () const {
		if (mSet.size () == 0)
			return 0 ;
		return mWidth ;
	}

	LENGTH length () const {
		static constexpr auto M_TABLE = PACK<VAR32[256]> ({
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
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret += M_TABLE.P1[mSet[i]] ;
		if (mWidth % 8 != 0)
			ret -= M_TABLE.P1[mSet[mWidth / 8] & ~((VAR (1) << (mWidth % 8)) - 1)] ;
		return std::move (ret) ;
	}

	void clear () {
		fill (BYTE (0X00)) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	Bit<BitSet> get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit<BitSet> (*this ,index) ;
	}

	inline Bit<BitSet> operator[] (INDEX index) {
		return get (index) ;
	}

	//@info: 'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	Bit<const BitSet> get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit<const BitSet> (*this ,index) ;
	}

	inline Bit<const BitSet> operator[] (INDEX index) const {
		return get (index) ;
	}

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

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (get (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = index + 1 ; i < size () ; i++)
			if (get (i))
				return i ;
		return VAR_NONE ;
	}

	BOOL equal (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		if (ix < 0)
			return TRUE ;
		for (INDEX i = 0 ; i < ix ; i++)
			if (mSet[i] != right.mSet[i])
				return FALSE ;
		const auto r1x = mSet[ix] & (mWidth % 8 - 1) ;
		const auto r2x = right.mSet[ix] & (mWidth % 8 - 1) ;
		return r1x == r2x ;
	}

	inline BOOL operator== (const BitSet &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const BitSet &right) const {
		return !equal (right) ;
	}

	BOOL less (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		if (ix < 0)
			return FALSE ;
		for (INDEX i = 0 ; i < ix ; i++)
			if (mSet[i] != right.mSet[i])
				return mSet[i] < right.mSet[i] ;
		const auto r1x = mSet[ix] & (mWidth % 8 - 1) ;
		const auto r2x = right.mSet[ix] & (mWidth % 8 - 1) ;
		return r1x < r2x ;
	}

	inline BOOL operator< (const BitSet &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const BitSet &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const BitSet &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const BitSet &right) const {
		return !right.less (*this) ;
	}

	void add (INDEX item) {
		_DYNAMIC_ASSERT_ (item >= 0 && item < mWidth) ;
		get (item) = TRUE ;
	}

	inline BitSet &operator<< (INDEX item) {
		add (item) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		for (auto &&i : src)
			add (i) ;
	}

	void erase (INDEX item) {
		get (item) = FALSE ;
	}

	BitSet band (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret.mSet[i] = mSet[i] & right.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator& (const BitSet &right) const {
		return band (right) ;
	}

	void bandto (const BitSet &right) {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			mSet[i] &= right.mSet[i] ;
	}

	inline BitSet &operator&= (const BitSet &right) {
		bandto (right) ;
		return *this ;
	}

	BitSet bor (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret.mSet[i] = mSet[i] | right.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator| (const BitSet &right) const {
		return bor (right) ;
	}

	void borto (const BitSet &right) {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			mSet[i] |= right.mSet[i] ;
	}

	inline BitSet &operator|= (const BitSet &right) {
		borto (right) ;
		return *this ;
	}

	BitSet bxor (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret.mSet[i] = mSet[i] ^ right.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator^ (const BitSet &right) const {
		return bxor (right) ;
	}

	void bxorto (const BitSet &right) {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			mSet[i] ^= right.mSet[i] ;
	}

	inline BitSet &operator^= (const BitSet &right) {
		bxorto (right) ;
		return *this ;
	}

	BitSet bsub (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret.mSet[i] = mSet[i] & ~right.mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator- (const BitSet &right) const {
		return bsub (right) ;
	}

	void bsubto (const BitSet &right) {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			mSet[i] &= ~right.mSet[i] ;
	}

	inline BitSet &operator-= (const BitSet &right) {
		bsubto (right) ;
		return *this ;
	}

	BitSet bnot () const {
		BitSet ret = BitSet (mWidth) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret.mSet[i] = ~mSet[i] ;
		return std::move (ret) ;
	}

	inline BitSet operator~ () const {
		return bnot () ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		return std::move (ret) ;
	}

	void fill (const BYTE &val) {
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			mSet[i] = val ;
	}

	template <class _ARG>
	void fill (const Array<BYTE ,_ARG> &val) {
		INDEX ir = 0 ;
		for (INDEX i = 0 ; i < mSet.size () ; i++) {
			mSet[i] = val[ir] ;
			ir = (ir + 1) % val.size () ;
		}
	}

private:
	explicit BitSet (LENGTH len ,LENGTH width ,const decltype (ARGVP0) &) :mSet (len) ,mWidth (width) {}

private:
	inline static LENGTH expr_size (const std::initializer_list<INDEX> &right) {
		LENGTH ret = VAR_MIN ;
		for (auto &&i : right)
			ret = _MAX_ (ret ,i) ;
		_DEBUG_ASSERT_ (ret >= 0) ;
		ret++ ;
		return std::move (ret) ;
	}

	inline static LENGTH expr_width (LENGTH width) {
		_DEBUG_ASSERT_ (width >= 0 && width < VAR32_MAX) ;
		return width ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class Set ;

template <class KEY ,class ITEM ,class SIZE>
class Set<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
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

		inline explicit Node (KEY &&key ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (const KEY &key ,ITEM &&item ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mItem (std::move (item)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mItem (std::move (item)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}
	} ;

	using NodePair = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair {
	public:
		friend Set ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) ,item (base.mSet[index].mItem) {}
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	Set () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	explicit Set (LENGTH len) :Set (len ,ARGVP0) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

private:
	explicit Set (LENGTH len ,const decltype (ARGVP0) &) :mSet (len) {}
} ;

template <class KEY ,class SIZE>
class Set<KEY ,SPECIALIZATION<void> ,SIZE> {
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

	using NodePair = PACK<KEY> ;

	template <class BASE>
	class Pair {
	public:
		friend Set ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) {}
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;
	INDEX mTop ;

public:
	Set () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	explicit Set (LENGTH len) :Set (len ,ARGVP0) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	void add (const KEY &key) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_emplace (mRoot ,ix) ;
			mRoot = mTop ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

private:
	explicit Set (LENGTH len ,const decltype (ARGVP0) &) :mSet (len) {}
} ;

template <class KEY ,class ITEM ,class SIZE>
class Set :private Set<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Set<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using NodePair = typename SPECIALIZATION_BASE::NodePair ;
	template <class _ARG>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mSet ;
	using SPECIALIZATION_BASE::mRoot ;
	using SPECIALIZATION_BASE::mTop ;

public:
	Set () = default ;

	explicit Set (LENGTH len) :SPECIALIZATION_BASE (len) {}

	implicit Set (const std::initializer_list<NodePair> &right) : Set (right.size ()) {
		for (auto &&i : right)
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

	Pair<Set> get (INDEX index) {
		return Pair<Set> (*this ,index) ;
	}

	inline Pair<Set> operator[] (INDEX index) {
		return get (index) ;
	}

	Pair<const Set> get (INDEX index) const {
		return Pair<const Set> (*this ,index) ;
	}

	inline Pair<const Set> operator[] (INDEX index) const {
		return get (index) ;
	}

#ifdef __CSC_COMPILER_MSVC__
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<REMOVE_CONST_TYPE<_ARG> ,Set>::value>>
	INDEX at (const Pair<_ARG> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}
#else
	INDEX at (const Pair<Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	INDEX at (const Pair<const Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}
#endif

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = index + 1 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	BOOL equal (const Set &right) const {
		return equal_each (right ,mRoot ,right.mRoot) ;
	}

	inline BOOL operator== (const Set &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Set &right) const {
		return !equal (right) ;
	}

	using SPECIALIZATION_BASE::add ;

	inline Set &operator<< (const NodePair &item) {
		add (std::move (item)) ;
		return *this ;
	}

	inline Set &operator<< (NodePair &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	using SPECIALIZATION_BASE::appand ;

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
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
		if (ret == VAR_NONE) {
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			update_emplace (mRoot ,ret) ;
			mRoot = mTop ;
			update_insert (ret) ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		if (mSet[index].mLeft != VAR_NONE && mSet[index].mRight != VAR_NONE)
			swap (index ,successor_one (index)) ;
		INDEX ix = (mSet[index].mLeft != VAR_NONE) ? (mSet[index].mLeft) : (mSet[index].mRight) ;
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
			const auto r1x = key < mSet[ret].mKey ;
			if (!r1x && !(mSet[ret].mKey < key))
				break ;
			ret = r1x ? (mSet[ret].mLeft) : (mSet[ret].mRight) ;
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
		return std::move (ret) ;
	}

private:
	BOOL equal_each (const Set &right ,INDEX it ,INDEX jt) const {
		if (it == VAR_NONE || jt == VAR_NONE)
			return it == VAR_NONE && jt == VAR_NONE ;
		if (!equal_each (right ,mSet[it].mLeft ,right.mSet[jt].mLeft))
			return FALSE ;
		if (mSet[it].mKey != right.mSet[jt].mKey)
			return FALSE ;
		if (!equal_each (right ,mSet[it].mRight ,right.mSet[jt].mRight))
			return FALSE ;
		return TRUE ;
	}

	void update_emplace (INDEX it ,INDEX jt) {
		if (it == VAR_NONE) {
			mTop = jt ;
		} else {
			mSet[jt].mUp = it ;
			auto &r1 = (mSet[jt].mKey < mSet[it].mKey) ? (mSet[it].mLeft) : (mSet[it].mRight) ;
			update_emplace (r1 ,jt) ;
			r1 = mTop ;
			mTop = it ;
		}
	}

	void update_insert (INDEX it) {
		INDEX ir = it ;
		while (TRUE) {
			INDEX ix = mSet[ir].mUp ;
			if (ix == VAR_NONE)
				break ;
			if (!mSet[ix].mRed)
				break ;
			const auto r1x = (ix == mSet[mSet[ix].mUp].mLeft) ? (&Set::update_insert_left) : (&Set::update_insert_right) ;
			(this->*r1x) (ir) ;
			ir = mTop ;
		}
		mSet[mRoot].mRed = FALSE ;
	}

	void update_insert_left (INDEX it) {
		INDEX ix = mSet[it].mUp ;
		INDEX iy = mSet[ix].mUp ;
		if (mSet[iy].mRight != VAR_NONE && mSet[mSet[iy].mRight].mRed) {
			mSet[mSet[iy].mRight].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			mTop = iy ;
		} else if (it == mSet[ix].mRight) {
			auto &r1 = mSet[iy].mLeft ;
			rotate_left (r1) ;
			r1 = mTop ;
			mSet[it].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2 = prev_next (iy) ;
			rotate_right (r2) ;
			r2 = mTop ;
			mTop = ix ;
		} else if (it == mSet[ix].mLeft) {
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2 = prev_next (iy) ;
			rotate_right (r2) ;
			r2 = mTop ;
			mTop = it ;
		}
	}

	void update_insert_right (INDEX it) {
		INDEX ix = mSet[it].mUp ;
		INDEX iy = mSet[ix].mUp ;
		if (mSet[iy].mLeft != VAR_NONE && mSet[mSet[iy].mLeft].mRed) {
			mSet[mSet[iy].mLeft].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			mTop = iy ;
		} else if (it == mSet[ix].mLeft) {
			auto &r1 = mSet[iy].mRight ;
			rotate_right (r1) ;
			r1 = mTop ;
			mSet[it].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2 = prev_next (iy) ;
			rotate_left (r2) ;
			r2 = mTop ;
			mTop = ix ;
		} else if (it == mSet[ix].mRight) {
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			auto &r2 = prev_next (iy) ;
			rotate_left (r2) ;
			r2 = mTop ;
			mTop = it ;
		}
	}

	void update_remove (INDEX it ,INDEX jt) {
		INDEX ir = it ;
		INDEX jr = jt ;
		while (TRUE) {
			if (jr == VAR_NONE)
				break ;
			if (ir != VAR_NONE && mSet[ir].mRed)
				break ;
			const auto r1x = (ir == mSet[jr].mLeft) ? (&Set::update_remove_left) : (&Set::update_remove_right) ;
			(this->*r1x) (ir ,jr) ;
			ir = mTop ;
			jr = mSet[ir].mUp ;
		}
		if (ir == VAR_NONE)
			return ;
		mSet[ir].mRed = FALSE ;
	}

	void update_remove_left (INDEX it ,INDEX jt) {
		auto &r1 = mSet[jt].mRight ;
		if (mSet[r1].mRed) {
			mSet[r1].mRed = FALSE ;
			mSet[jt].mRed = TRUE ;
			auto &r2 = prev_next (jt) ;
			rotate_left (r2) ;
			r2 = mTop ;
		}
		const auto r1x = mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed ;
		const auto r2x = mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed ;
		if (r1x && r2x) {
			mSet[r1].mRed = TRUE ;
			mTop = jt ;
		} else if (r2x) {
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			mSet[r1].mRed = TRUE ;
			rotate_right (r1) ;
			r1 = mTop ;
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mRight].mRed = FALSE ;
			auto &r3 = prev_next (jt) ;
			rotate_left (r3) ;
			r3 = mTop ;
			mTop = mRoot ;
		} else {
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mRight].mRed = FALSE ;
			auto &r3 = prev_next (jt) ;
			rotate_left (r3) ;
			r3 = mTop ;
			mTop = mRoot ;
		}
	}

	void update_remove_right (INDEX it ,INDEX jt) {
		auto &r1 = mSet[jt].mLeft ;
		if (mSet[r1].mRed) {
			mSet[r1].mRed = FALSE ;
			mSet[jt].mRed = TRUE ;
			auto &r2 = prev_next (jt) ;
			rotate_right (r2) ;
			r2 = mTop ;
		}
		const auto r1x = mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed ;
		const auto r2x = mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed ;
		if (r1x && r2x) {
			mSet[r1].mRed = TRUE ;
			mTop = jt ;
		} else if (r2x) {
			mSet[mSet[r1].mRight].mRed = FALSE ;
			mSet[r1].mRed = TRUE ;
			rotate_left (r1) ;
			r1 = mTop ;
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			auto &r3 = prev_next (jt) ;
			rotate_right (r3) ;
			r3 = mTop ;
			mTop = mRoot ;
		} else {
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			auto &r3 = prev_next (jt) ;
			rotate_right (r3) ;
			r3 = mTop ;
			mTop = mRoot ;
		}
	}

	void rotate_left (INDEX it) {
		INDEX ix = mSet[it].mRight ;
		mSet[it].mRight = mSet[ix].mLeft ;
		if (mSet[ix].mLeft != VAR_NONE)
			mSet[mSet[ix].mLeft].mUp = it ;
		mSet[ix].mLeft = it ;
		mSet[ix].mUp = mSet[it].mUp ;
		mSet[it].mUp = ix ;
		mTop = ix ;
	}

	void rotate_right (INDEX it) {
		INDEX ix = mSet[it].mLeft ;
		mSet[it].mLeft = mSet[ix].mRight ;
		if (mSet[ix].mRight != VAR_NONE)
			mSet[mSet[ix].mRight].mUp = it ;
		mSet[ix].mRight = it ;
		mSet[ix].mUp = mSet[it].mUp ;
		mSet[it].mUp = ix ;
		mTop = ix ;
	}

	INDEX &prev_next (INDEX it) {
		if (mSet[it].mUp == VAR_NONE)
			return mRoot ;
		if (mSet[mSet[it].mUp].mLeft == it)
			return mSet[mSet[it].mUp].mLeft ;
		return mSet[mSet[it].mUp].mRight ;
	}

	INDEX successor_one (INDEX index) const {
		for (INDEX i = mSet[index].mRight ; i != VAR_NONE ; i = mSet[i].mLeft)
			if (mSet[i].mLeft == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	void swap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1 = prev_next (index2) ;
		auto &r2 = (mSet[index2].mLeft != VAR_NONE) ? (mSet[mSet[index2].mLeft].mUp) : index1 ;
		auto &r3 = (mSet[index2].mRight != VAR_NONE) ? (mSet[mSet[index2].mRight].mUp) : index1 ;
		auto &r4 = prev_next (index1) ;
		auto &r5 = (mSet[index1].mLeft != VAR_NONE) ? (mSet[mSet[index1].mLeft].mUp) : index2 ;
		auto &r6 = (mSet[index1].mRight != VAR_NONE) ? (mSet[mSet[index1].mRight].mUp) : index2 ;
		r1 = index1 ;
		r2 = index1 ;
		r3 = index1 ;
		r4 = index2 ;
		r5 = index2 ;
		r6 = index2 ;
		_SWAP_ (mSet[index1].mRed ,mSet[index2].mRed) ;
		_SWAP_ (mSet[index1].mUp ,mSet[index2].mUp) ;
		_SWAP_ (mSet[index1].mLeft ,mSet[index2].mLeft) ;
		_SWAP_ (mSet[index1].mRight ,mSet[index2].mRight) ;
	}

	void compute_esort (INDEX it ,Array<INDEX> &out ,INDEX &jt) const {
		if (it == VAR_NONE)
			return ;
		INDEX iw = jt ;
		compute_esort (mSet[it].mLeft ,out ,iw) ;
		out[iw++] = it ;
		compute_esort (mSet[it].mRight ,out ,iw) ;
		jt = iw ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class HashSet ;

template <class KEY ,class ITEM ,class SIZE>
class HashSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
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

	using NodePair = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair {
	public:
		friend HashSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) ,item (base.mSet[index].mItem) {}
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	Buffer<INDEX ,SIZE> mHead ;
	INDEX mTop ;

public:
	HashSet () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	explicit HashSet (LENGTH len) :HashSet (len ,ARGVP0) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize (ix) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize (ix) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

private:
	explicit HashSet (LENGTH len ,const decltype (ARGVP0) &) :mSet (len) ,mHead (len) {}
} ;

template <class KEY ,class SIZE>
class HashSet<KEY ,SPECIALIZATION<void> ,SIZE> {
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

	using NodePair = PACK<KEY> ;

	template <class BASE>
	class Pair {
	public:
		friend HashSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) {}
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Allocator<Node ,SIZE> mSet ;
	Buffer<INDEX ,SIZE> mHead ;
	INDEX mTop ;

public:
	HashSet () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	explicit HashSet (LENGTH len) :HashSet (len ,ARGVP0) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	void add (const KEY &key) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize (ix) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
			ix = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_resize (ix) ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (ix) ;
		}
		mTop = ix ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		mSet.reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

private:
	explicit HashSet (LENGTH len ,const decltype (ARGVP0) &) :mSet (len) ,mHead (len) {}
} ;

template <class KEY ,class ITEM ,class SIZE>
class HashSet :private HashSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = HashSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using NodePair = typename SPECIALIZATION_BASE::NodePair ;
	template <class _ARG>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mSet ;
	using SPECIALIZATION_BASE::mHead ;
	using SPECIALIZATION_BASE::mTop ;

public:
	HashSet () = default ;

	explicit HashSet (LENGTH len) :SPECIALIZATION_BASE (len) {}

	implicit HashSet (const std::initializer_list<NodePair> &right) : HashSet (right.size ()) {
		for (auto &&i : right)
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

	Pair<HashSet> get (INDEX index) {
		return Pair<HashSet> (*this ,index) ;
	}

	inline Pair<HashSet> operator[] (INDEX index) {
		return get (index) ;
	}

	Pair<const HashSet> get (INDEX index) const {
		return Pair<const HashSet> (*this ,index) ;
	}

	inline Pair<const HashSet> operator[] (INDEX index) const {
		return get (index) ;
	}

#ifdef __CSC_COMPILER_MSVC__
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<REMOVE_CONST_TYPE<_ARG> ,HashSet>::value>>
	INDEX at (const Pair<_ARG> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}
#else
	INDEX at (const Pair<HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	INDEX at (const Pair<const HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}
#endif

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		for (INDEX i = index + 1 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return VAR_NONE ;
	}

	BOOL equal (const HashSet &right) const {
		if (length () != right.length ())
			return FALSE ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (!equal_each (right ,i))
				return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const HashSet &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const HashSet &right) const {
		return !equal (right) ;
	}

	using SPECIALIZATION_BASE::add ;

	inline HashSet &operator<< (const NodePair &item) {
		add (std::move (item)) ;
		return *this ;
	}

	inline HashSet &operator<< (NodePair &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	using SPECIALIZATION_BASE::appand ;

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_resize (ret) ;
			update_insert (ret) ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
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
		if (size () > 0) {
			const auto r1x = U::OPERATOR_HASH<KEY>::invoke (key) ;
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
	BOOL equal_each (const HashSet &right ,INDEX it) const {
		INDEX ix = right.find (mSet[it].mKey) ;
		return ix != VAR_NONE && mSet[it].mItem == right.mSet[ix].mItem ;
	}

	void update_resize (INDEX it) {
		if (mHead.size () == mSet.size ())
			return ;
		auto rax = mHead.expand (mSet.size ()) ;
		_MEMFILL_ (rax.self ,rax.size () ,VAR_NONE) ;
		for (INDEX i = 0 ; i < mSet.size () ; i++) {
			if (i == it)
				continue ;
			if (!mSet.used (i))
				continue ;
			INDEX ix = mSet[i].mHash % rax.size () ;
			mSet[i].mNext = rax[ix] ;
			rax[ix] = i ;
		}
		mHead.expand (std::move (rax)) ;
	}

	void update_insert (INDEX it) {
		INDEX ix = mSet[it].mHash % mHead.size () ;
		mSet[it].mNext = mHead[ix] ;
		mHead[ix] = it ;
	}

	INDEX &prev_next (INDEX it) {
		INDEX ix = mSet[it].mHash % mHead.size () ;
		if (mHead[ix] == it)
			return mHead[ix] ;
		for (INDEX i = mHead[ix] ; mSet[i].mNext != VAR_NONE ; i = mSet[i].mNext)
			if (mSet[i].mNext == it)
				return mSet[i].mNext ;
		_DEBUG_ASSERT_ (FALSE) ;
		return VAR_NONE ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class SoftSet ;

template <class KEY ,class ITEM ,class SIZE>
class SoftSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
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

	using NodePair = PACK<KEY ,ITEM> ;

	template <class BASE>
	class Pair {
	public:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet.self[index].mKey) ,item (base.mSet.self[index].mItem) {}
	} ;

	class Attribute {
	private:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		SharedRef<Allocator<Node ,SIZE>> mHeap ;
		LENGTH mLength ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mRoot ;
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	SharedRef<Attribute> mHolder ;
	PhanRef<Allocator<Node ,SIZE>> mSet ;
	INDEX mTop ;

public:
	SoftSet () = default ;

	explicit SoftSet (LENGTH len) {
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mHeap = SharedRef<Allocator<Node ,SIZE>>::make (len) ;
		mHolder->mLength = 0 ;
		mHolder->mFirst = VAR_NONE ;
		mHolder->mLast = VAR_NONE ;
		mHolder->mRoot = VAR_NONE ;
		mSet = PhanRef<Allocator<Node ,SIZE>>::make (mHolder->mHeap.self) ;
	}

	void add (const KEY &key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1 = (mHolder->mLast != VAR_NONE) ? (mSet.self[mHolder->mLast].mNext) : (mHolder->mFirst) ;
			r1 = ix ;
			mHolder->mLast = ix ;
			mHolder->mLength++ ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (mHolder->mRoot) ;
			mHolder->mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	void add (KEY &&key ,const ITEM &item) {
		add (std::move (key) ,_COPY_ (item)) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1 = (mHolder->mLast != VAR_NONE) ? (mSet.self[mHolder->mLast].mNext) : (mHolder->mFirst) ;
			r1 = ix ;
			mHolder->mLast = ix ;
			mHolder->mLength++ ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (mHolder->mRoot) ;
			mHolder->mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mSet->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mSet->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key ,std::move (i.item)) ;
	}
} ;

template <class KEY ,class SIZE>
class SoftSet<KEY ,SPECIALIZATION<void> ,SIZE> {
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

	using NodePair = PACK<KEY> ;

	template <class BASE>
	class Pair {
	public:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		const KEY &key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		//@info: be careful about key's lifetime
		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () && {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet.self[index].mKey) {}
	} ;

	class Attribute {
	private:
		friend SoftSet ;
		friend SPECIALIZATION_TYPE ;
		SharedRef<Allocator<Node ,SIZE>> mHeap ;
		LENGTH mLength ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mRoot ;
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	SharedRef<Attribute> mHolder ;
	PhanRef<Allocator<Node ,SIZE>> mSet ;
	INDEX mTop ;

public:
	SoftSet () = default ;

	explicit SoftSet (LENGTH len) {
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mHeap = SharedRef<Allocator<Node ,SIZE>>::make (len) ;
		mHolder->mLength = 0 ;
		mHolder->mFirst = VAR_NONE ;
		mHolder->mLast = VAR_NONE ;
		mHolder->mRoot = VAR_NONE ;
		mSet = PhanRef<Allocator<Node ,SIZE>>::make (mHolder->mHeap.self) ;
	}

	void add (const KEY &key) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1 = (mHolder->mLast != VAR_NONE) ? (mSet.self[mHolder->mLast].mNext) : (mHolder->mFirst) ;
			r1 = ix ;
			mHolder->mLast = ix ;
			mHolder->mLength++ ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (mHolder->mRoot) ;
			mHolder->mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (const NodePair &item) {
		add (std::move (item.P1)) ;
	}

	void add (KEY &&key) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		INDEX ix = static_cast<PTR<SPECIALIZATION_TYPE>> (this)->find (key) ;
		if (ix == VAR_NONE) {
			ix = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1 = (mHolder->mLast != VAR_NONE) ? (mSet.self[mHolder->mLast].mNext) : (mHolder->mFirst) ;
			r1 = ix ;
			mHolder->mLast = ix ;
			mHolder->mLength++ ;
			static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_insert (mHolder->mRoot) ;
			mHolder->mRoot = mTop ;
		}
		mTop = ix ;
	}

	void add (NodePair &&item) {
		add (std::move (item.P1)) ;
	}

	template <class _ARG>
	void appand (const _ARG &src) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mSet->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}

	template <class _ARG>
	void appand (_ARG &&src) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mSet->reserve (src.length ()) ;
		for (auto &&i : src)
			add (i.key) ;
	}
} ;

template <class KEY ,class ITEM ,class SIZE>
class SoftSet :private SoftSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> {
private:
	using SPECIALIZATION_BASE = SoftSet<KEY ,SPECIALIZATION<ITEM> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;
	using NodePair = typename SPECIALIZATION_BASE::NodePair ;
	template <class _ARG>
	using Pair = typename SPECIALIZATION_BASE::template Pair<_ARG> ;
	using Attribute = typename SPECIALIZATION_BASE::Attribute ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mHolder ;
	using SPECIALIZATION_BASE::mSet ;
	using SPECIALIZATION_BASE::mTop ;

public:
	SoftSet () = default ;

	explicit SoftSet (LENGTH len) :SPECIALIZATION_BASE (len) {}

	LENGTH capacity () const {
		if (!mHolder.exist ())
			return 0 ;
		return mSet->size () ;
	}

	LENGTH size () const {
		if (!mHolder.exist ())
			return 0 ;
		return mSet->size () - (mSet->length () - mHolder->mLength) ;
	}

	LENGTH length () const {
		if (!mHolder.exist ())
			return 0 ;
		return mHolder->mLength ;
	}

	void reset () {
		if (!mHolder.exist ())
			return ;
		auto rax = SharedRef<Attribute>::make () ;
		rax->mHeap = mHolder->mHeap ;
		rax->mLength = 0 ;
		rax->mFirst = VAR_NONE ;
		rax->mLast = VAR_NONE ;
		rax->mRoot = VAR_NONE ;
		mHolder = std::move (rax) ;
		mSet = PhanRef<Allocator<Node ,SIZE>>::make (mHolder->mHeap.self) ;
	}

	SoftSet copy () popping {
		SoftSet ret ;
		ret.mHolder = mHolder ;
		ret.mSet = PhanRef<Allocator<Node ,SIZE>>::make (mSet) ;
		ret.mTop = mTop ;
		return std::move (ret) ;
	}

	Pair<SoftSet> get (INDEX index) {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		return Pair<SoftSet> (*this ,index) ;
	}

	inline Pair<SoftSet> operator[] (INDEX index) {
		return get (index) ;
	}

	Pair<const SoftSet> get (INDEX index) const {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		return Pair<const SoftSet> (*this ,index) ;
	}

	inline Pair<const SoftSet> operator[] (INDEX index) const {
		return get (index) ;
	}

#ifdef __CSC_COMPILER_MSVC__
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<REMOVE_CONST_TYPE<_ARG> ,SoftSet>::value>>
	INDEX at (const Pair<_ARG> &item) const {
		return mSet->at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}
#else
	INDEX at (const Pair<SoftSet> &item) const {
		return mSet->at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}

	INDEX at (const Pair<const SoftSet> &item) const {
		return mSet->at (_OFFSET_ (&Node::mKey ,item.key)) ;
	}
#endif

	INDEX ibegin () const {
		if (!mHolder.exist ())
			return VAR_NONE ;
		return mHolder->mFirst ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return mSet.self[index].mNext ;
	}

	BOOL equal (const SoftSet &right) const {
		const auto r1x = (mHolder.exist ()) ? (mHolder->mRoot) : VAR_NONE ;
		const auto r2x = (right.mHolder.exist ()) ? (right.mHolder->mRoot) : VAR_NONE ;
		return equal_each (right ,r1x ,r2x) ;
	}

	inline BOOL operator== (const SoftSet &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftSet &right) const {
		return !equal (right) ;
	}

	using SPECIALIZATION_BASE::add ;

	inline SoftSet &operator<< (const NodePair &item) {
		add (std::move (item)) ;
		return *this ;
	}

	inline SoftSet &operator<< (NodePair &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	using SPECIALIZATION_BASE::appand ;

	INDEX insert (const KEY &key) popping {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1 = (mHolder->mLast != VAR_NONE) ? (mSet.self[mHolder->mLast].mNext) : (mHolder->mFirst) ;
			r1 = ret ;
			mHolder->mLast = ret ;
			mHolder->mLength++ ;
			update_insert (mHolder->mRoot) ;
			mHolder->mRoot = mTop ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSet->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			auto &r1 = (mHolder->mLast != VAR_NONE) ? (mSet.self[mHolder->mLast].mNext) : (mHolder->mFirst) ;
			r1 = ret ;
			mHolder->mLast = ret ;
			mHolder->mLength++ ;
			update_insert (mHolder->mRoot) ;
			mHolder->mRoot = mTop ;
		}
		mTop = ret ;
		return std::move (ret) ;
	}

	INDEX min_one () const {
		INDEX ix = (mHolder.exist ()) ? (mHolder->mRoot) : VAR_NONE ;
		for (INDEX i = ix ; i != VAR_NONE ; i = mSet.self[i].mLeft)
			if (mSet.self[i].mLeft == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	INDEX max_one () const {
		INDEX ix = (mHolder.exist ()) ? (mHolder->mRoot) : VAR_NONE ;
		for (INDEX i = ix ; i != VAR_NONE ; i = mSet.self[i].mRight)
			if (mSet.self[i].mRight == VAR_NONE)
				return i ;
		return VAR_NONE ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = (mHolder.exist ()) ? (mHolder->mRoot) : VAR_NONE ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = key < mSet.self[ret].mKey ;
			if (!r1x && !(mSet.self[ret].mKey < key))
				break ;
			ret = r1x ? (mSet.self[ret].mLeft) : (mSet.self[ret].mRight) ;
		}
		return std::move (ret) ;
	}

	void clean () {
		if (!mHolder.exist ())
			return ;
		mSet->clean () ;
	}

private:
	BOOL equal_each (const SoftSet &right ,INDEX it ,INDEX jt) const {
		if (it == VAR_NONE || jt == VAR_NONE)
			return it == VAR_NONE && jt == VAR_NONE ;
		if (!equal_each (right ,mSet.self[it].mLeft ,right.mSet.self[jt].mLeft))
			return FALSE ;
		if (mSet.self[it].mKey != right.mSet.self[jt].mKey)
			return FALSE ;
		if (!equal_each (right ,mSet.self[it].mRight ,right.mSet.self[jt].mRight))
			return FALSE ;
		return TRUE ;
	}

	void update_insert (INDEX it) {
		INDEX ix = it ;
		if (ix == VAR_NONE) {
			mTop = mHolder->mLast ;
		} else {
			mSet.self[ix].mWeight++ ;
			const auto r1x = mSet.self[mHolder->mLast].mKey < mSet.self[ix].mKey ;
			auto &r2 = r1x ? (mSet.self[ix].mLeft) : (mSet.self[ix].mRight) ;
			update_insert (r2) ;
			r2 = mTop ;
			const auto r2x = r1x ? (&SoftSet::update_insert_left) : (&SoftSet::update_insert_right) ;
			(this->*r2x) (ix) ;
			ix = mTop ;
			mTop = ix ;
		}
	}

	void update_insert_left (INDEX it) {
		INDEX ir = it ;
		const auto r1x = (mSet.self[ir].mRight != VAR_NONE) ? (mSet.self[mSet.self[ir].mRight].mWeight) : 0 ;
		const auto r2x = (mSet.self[ir].mLeft != VAR_NONE && mSet.self[mSet.self[ir].mLeft].mLeft != VAR_NONE) ? (mSet.self[mSet.self[mSet.self[ir].mLeft].mLeft].mWeight) : 0 ;
		const auto r3x = (mSet.self[ir].mLeft != VAR_NONE && mSet.self[mSet.self[ir].mLeft].mRight != VAR_NONE) ? (mSet.self[mSet.self[mSet.self[ir].mLeft].mRight].mWeight) : 0 ;
		mTop = ir ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if (r1x >= r2x) {
			auto &r1 = mSet.self[ir].mLeft ;
			rotate_left (r1) ;
			r1 = mTop ;
		}
		rotate_right (ir) ;
		ir = mTop ;
		auto &r2 = mSet.self[ir].mLeft ;
		update_insert_left (r2) ;
		r2 = mTop ;
		auto &r3 = mSet.self[ir].mRight ;
		update_insert_right (r3) ;
		r3 = mTop ;
		update_insert_left (ir) ;
		ir = mTop ;
		update_insert_right (ir) ;
		ir = mTop ;
		mTop = ir ;
	}

	void update_insert_right (INDEX it) {
		INDEX ir = it ;
		const auto r1x = (mSet.self[ir].mLeft != VAR_NONE) ? (mSet.self[mSet.self[ir].mLeft].mWeight) : 0 ;
		const auto r2x = (mSet.self[ir].mRight != VAR_NONE && mSet.self[mSet.self[ir].mRight].mRight != VAR_NONE) ? (mSet.self[mSet.self[mSet.self[ir].mRight].mRight].mWeight) : 0 ;
		const auto r3x = (mSet.self[ir].mRight != VAR_NONE && mSet.self[mSet.self[ir].mRight].mLeft != VAR_NONE) ? (mSet.self[mSet.self[mSet.self[ir].mRight].mLeft].mWeight) : 0 ;
		mTop = ir ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if (r1x >= r2x) {
			auto &r1 = mSet.self[ir].mRight ;
			rotate_right (r1) ;
			r1 = mTop ;
		}
		rotate_left (ir) ;
		ir = mTop ;
		auto &r2 = mSet.self[ir].mLeft ;
		update_insert_left (r2) ;
		r2 = mTop ;
		auto &r3 = mSet.self[ir].mRight ;
		update_insert_right (r3) ;
		r3 = mTop ;
		update_insert_left (ir) ;
		ir = mTop ;
		update_insert_right (ir) ;
		ir = mTop ;
		mTop = ir ;
	}

	void rotate_left (INDEX it) {
		INDEX ix = mSet.self[it].mRight ;
		mSet.self[it].mRight = mSet.self[ix].mLeft ;
		mSet.self[ix].mLeft = it ;
		mSet.self[ix].mWeight = mSet.self[it].mWeight ;
		const auto r1x = (mSet.self[it].mLeft != VAR_NONE) ? (mSet.self[mSet.self[it].mLeft].mWeight) : 0 ;
		const auto r2x = (mSet.self[it].mRight != VAR_NONE) ? (mSet.self[mSet.self[it].mRight].mWeight) : 0 ;
		mSet.self[it].mWeight = r1x + r2x + 1 ;
		mTop = ix ;
	}

	void rotate_right (INDEX it) {
		INDEX ix = mSet.self[it].mLeft ;
		mSet.self[it].mLeft = mSet.self[ix].mRight ;
		mSet.self[ix].mRight = it ;
		mSet.self[ix].mWeight = mSet.self[it].mWeight ;
		const auto r1x = (mSet.self[it].mLeft != VAR_NONE) ? (mSet.self[mSet.self[it].mLeft].mWeight) : 0 ;
		const auto r2x = (mSet.self[it].mRight != VAR_NONE) ? (mSet.self[mSet.self[it].mRight].mWeight) : 0 ;
		mSet.self[it].mWeight = r1x + r2x + 1 ;
		mTop = ix ;
	}
} ;
} ;