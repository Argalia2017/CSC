#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"

namespace CSC {
template <class BASE>
class ForwardIterator {
private:
	class IterableArray :private Wrapped<BASE> {
	public:
		inline ForwardIterator begin () {
			return ForwardIterator (IterableArray::mData ,IterableArray::mData.ibegin ()) ;
		}

		inline ForwardIterator begin () const {
			return ForwardIterator (IterableArray::mData ,IterableArray::mData.ibegin ()) ;
		}

		inline ForwardIterator end () {
			return ForwardIterator (IterableArray::mData ,IterableArray::mData.iend ()) ;
		}

		inline ForwardIterator end () const {
			return ForwardIterator (IterableArray::mData ,IterableArray::mData.iend ()) ;
		}
	} ;

private:
	BASE & mBase ;
	INDEX mIndex ;

public:
	inline ForwardIterator () = delete ;

	inline auto operator* () const->decltype (mBase[mIndex]) {
		return mBase[mIndex] ;
	}

	inline BOOL operator!= (const ForwardIterator &right) const {
		return mIndex != right.mIndex ;
	}

	inline ForwardIterator &operator++ () {
		mIndex = mBase.inext (mIndex) ;
		return *this ;
	}

private:
	inline explicit ForwardIterator (BASE &base ,INDEX index) :mBase (base) ,mIndex (index) {}

public:
	inline static CAST_TRAITS_TYPE<IterableArray ,BASE> &warp (BASE &arg) {
		return _CAST_<IterableArray> (arg) ;
	}
} ;

inline namespace ARRAY {
template <class _ARG>
inline auto begin (_ARG &&arg)->decltype (ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>>::warp (arg).begin ()) {
	_STATIC_ASSERT_ (std::is_lvalue_reference<_ARG &&>::value) ;
	return ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>>::warp (arg).begin () ;
}

template <class _ARG>
inline auto end (_ARG &&arg)->decltype (ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>>::warp (arg).end ()) {
	_STATIC_ASSERT_ (std::is_lvalue_reference<_ARG &&>::value) ;
	return ForwardIterator<REMOVE_REFERENCE_TYPE<_ARG>>::warp (arg).end () ;
}
} ;

namespace U {
template <class>
struct OPERATOR_HASH ;

template <>
struct OPERATOR_HASH<void> {
	template <class _ARG>
	inline static FLAG invoke (const _ARG &arg) {
		return OPERATOR_HASH<_ARG>::invoke (arg) ;
	}
} ;

template <class TYPE>
struct OPERATOR_HASH {
	template <class _ARG>
	inline static FLAG hash (const _ARG &left ,const ARGV<ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().hash ()) ,FLAG>::value>> & ,const ARGV<ARGC<2>> &) {
		return left.hash () ;
	}

	template <class _ARG>
	inline static FLAG hash (const _ARG &left ,const ARGV<ENABLE_TYPE<std::is_pod<_ARG>::value>> & ,const ARGV<ARGC<1>> &) {
		return _MEMHASH_ (_CAST_<BYTE[_SIZEOF_ (_ARG)]> (left) ,_SIZEOF_ (left)) ;
	}

	inline static FLAG invoke (const TYPE &left) {
		FLAG ret = hash (left ,ARGV_VOID ,ARGV9) ;
		_DEBUG_ASSERT_ (ret > 0) ;
		return std::move (ret) ;
	}
} ;
} ;

using OPERATOR_HASH_TYPE = U::OPERATOR_HASH<void> ;

namespace U {
template <class ,class>
struct OPERATOR_SORT ;

template <>
struct OPERATOR_SORT<void ,void> {
	template <class _ARG1 ,class _ARG2>
	inline static void invoke (const _ARG1 &src ,_ARG2 &dst ,INDEX ib ,INDEX ie) {
		OPERATOR_SORT<_ARG1 ,_ARG2>::invoke (src ,dst ,ib ,ie) ;
	}

	class ForwardArray :private Wrapped<void> {
	public:
		template <class _ARG>
		inline _ARG &operator[] (_ARG &arg) const {
			return arg ;
		}
	} ;

	template <class _ARG>
	inline static void invoke (_ARG &dst ,INDEX ib ,INDEX ie) {
		OPERATOR_SORT<ForwardArray ,_ARG>::invoke (_NULL_<ForwardArray> () ,dst ,ib ,ie) ;
	}
} ;

template <class TYPE1 ,class TYPE2>
struct OPERATOR_SORT {
	template <class _ARG1 ,class _ARG2>
	inline static void insert_sort_each (const _ARG1 &src ,_ARG2 &dst ,INDEX ib ,INDEX ie) {
		auto rax = std::move (dst[ie]) ;
		INDEX iw = ie ;
		while (iw - 1 >= ib && src[rax] < src[dst[iw - 1]]) {
			dst[iw] = std::move (dst[iw - 1]) ;
			iw-- ;
		}
		dst[iw] = std::move (rax) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void insert_sort (const _ARG1 &src ,_ARG2 &dst ,INDEX ib ,INDEX jb) {
		for (INDEX i = ib + 1 ; i <= jb ; i++)
			insert_sort_each (src ,dst ,ib ,i) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void quick_sort_partition (const _ARG1 &src ,_ARG2 &dst ,INDEX ib ,INDEX jb ,INDEX &it) {
		INDEX iw = ib ;
		INDEX jw = jb ;
		auto rax = std::move (dst[iw]) ;
		while (TRUE) {
			while (iw < jw && src[rax] < src[dst[jw]])
				jw-- ;
			if (iw >= jw)
				break ;
			dst[iw++] = std::move (dst[jw]) ;
			while (iw < jw && src[rax] > src[dst[iw]])
				iw++ ;
			if (iw >= jw)
				break ;
			dst[jw--] = std::move (dst[iw]) ;
		}
		dst[iw] = std::move (rax) ;
		it = iw ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void quick_sort (const _ARG1 &src ,_ARG2 &dst ,INDEX ib ,INDEX jb ,LENGTH ideal) {
		while (ib < jb && ideal > 0) {
			ideal = ideal / 2 + ideal / 4 ;
			INDEX ir = VAR_NONE ;
			quick_sort_partition (src ,dst ,ib ,jb ,ir) ;
			quick_sort (src ,dst ,ib ,ir - 1 ,ideal) ;
			ib = ir + 1 ;
		}
		if (ib < jb)
			insert_sort (src ,dst ,ib ,jb) ;
	}

	inline static void invoke (const TYPE1 &src ,TYPE2 &dst ,INDEX ib ,INDEX ie) {
		quick_sort (src ,dst ,ib ,ie - 1 ,ie) ;
	}
} ;
} ;

using OPERATOR_SORT_TYPE = U::OPERATOR_SORT<void ,void> ;

template <class ITEM ,class SIZE = SAUTO>
class Array {
public:
	using BUFFER = Buffer<ITEM ,SIZE> ;

private:
	Buffer<ITEM ,SIZE> mArray ;

public:
	Array () = default ;

	explicit Array (LENGTH len) :mArray (len) {}

	explicit Array (BUFFER &&src) :mArray (std::move (src)) {}

	implicit Array (const std::initializer_list<ITEM> &right) : Array (right.size ()) {
		_DEBUG_ASSERT_ (size () == LENGTH (right.size ())) ;
		INDEX iw = 0 ;
		for (auto &i : right)
			mArray[iw++] = i ;
	}

	const ARR<ITEM> &to () const {
		return mArray ;
	}

	inline implicit operator const ARR<ITEM> & () const {
		return to () ;
	}

	PhanBuffer<ITEM> raw () {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<ITEM>::make (mArray.self ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const {
		_DYNAMIC_ASSERT_ (mArray.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mArray.self ,length ()) ;
	}

	LENGTH size () const {
		return mArray.size () ;
	}

	LENGTH length () const {
		return size () ;
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
		OPERATOR_SORT_TYPE::invoke (mArray ,0 ,mArray.size ()) ;
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

public:
	inline static Array &from (Buffer<ITEM ,SIZE> &me) {
		return _CAST_<Array> (me) ;
	}

	inline static const Array &from (const Buffer<ITEM ,SIZE> &me) {
		return _CAST_<Array> (me) ;
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
class String {
private:
	inline static constexpr LENGTH cvt_size (LENGTH len) {
		return len > 0 ? len + 1 : len ;
	}

public:
	using BUFFER = Buffer<ITEM ,ARGC<cvt_size (SIZE::value)>> ;

private:
	Buffer<ITEM ,ARGC<cvt_size (SIZE::value)>> mString ;

public:
	String () {
		clear () ;
	}

	explicit String (LENGTH len) :mString (cvt_size (len)) {
		clear () ;
	}

	explicit String (BUFFER &&src) :mString (std::move (src)) {
		clear () ;
	}

	implicit String (const std::initializer_list<ITEM> &right) : String (right.size ()) {
		INDEX iw = 0 ;
		for (auto &i : right)
			mString[iw++] = i ;
		mString[iw] = ITEM (0) ;
	}

	implicit String (const ARR<ITEM> right) :String (_MEMCHR_ (right ,VAR_MAX ,ITEM (0))) {
		if (mString.size () > 0)
			_MEMCOPY_ (mString.self ,right ,size ()) ;
	}

	template <LENGTH _VAL>
	implicit String (const DEF<ITEM[_VAL]> &right) : String (_VAL) {
		_MEMCOPY_ (mString.self ,right ,size ()) ;
	}

	const ARR<ITEM> &to () const {
		return mString ;
	}

	inline implicit operator const ARR<ITEM> & () const {
		return to () ;
	}

	PhanBuffer<ITEM> raw () {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<ITEM>::make (mString.self ,size ()) ;
	}

	PhanBuffer<const ITEM> raw () const {
		_DYNAMIC_ASSERT_ (mString.size () > 0) ;
		return PhanBuffer<const ITEM>::make (mString.self ,length ()) ;
	}

	LENGTH size () const {
		return mString.size () != 0 ? mString.size () - 1 : 0 ;
	}

	LENGTH length () const {
		return mString.size () != 0 ? _MEMCHR_ (mString.self ,mString.size () ,ITEM (0)) : 0 ;
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
		INDEX ir = VAR_NONE ;
		for (ir = 0 ; mString[ir] != ITEM (0) ; ir++)
			if (mString[ir] != right.mString[ir])
				return FALSE ;
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
		INDEX ir = VAR_NONE ;
		for (ir = 0 ; mString[ir] != ITEM (0) ; ir++)
			if (mString[ir] != right.mString[ir])
				return mString[ir] < right.mString[ir] ;
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

	String add (const ITEM &right) const {
		const auto r1x = length () ;
		String ret = String (r1x + 1) ;
		_MEMCOPY_ (ret.mString.self ,mString.self ,r1x) ;
		_MEMCOPY_ (&ret.mString[r1x] ,&right ,1) ;
		return std::move (ret) ;
	}

	inline String operator+ (const ITEM &right) const {
		return add (right) ;
	}

	void addto (const ITEM &right) {
		const auto r1x = length () ;
		if (r1x + 1 <= size ()) {
			_MEMCOPY_ (&mString[r1x] ,&right ,1) ;
			mString[r1x + 1] = ITEM (0) ;
		} else {
			*this = add (right) ;
		}
	}

	inline String &operator+= (const ITEM &right) {
		addto (right) ;
		return *this ;
	}

	String add (const String &right) const {
		const auto r1x = length () ;
		const auto r2x = right.length () ;
		String ret = String (r1x + r2x) ;
		_MEMCOPY_ (ret.mString.self ,mString.self ,r1x) ;
		if (ret.mString.size () > 0)
			_MEMCOPY_ (&ret.mString[r1x] ,right.mString.self ,r2x) ;
		return std::move (ret) ;
	}

	inline String operator+ (const String &right) const {
		return add (right) ;
	}

	void addto (const String &right) {
		const auto r1x = length () ;
		const auto r2x = right.length () ;
		if (r1x + r2x <= size () && mString.size () > 0) {
			_MEMCOPY_ (&mString[r1x] ,right.mString.self ,r2x) ;
			mString[r1x + r2x] = ITEM (0) ;
		} else {
			*this = add (right) ;
		}
	}

	inline String &operator+= (const String &right) {
		addto (right) ;
		return *this ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	void fill (const ITEM &val) {
		for (INDEX i = 0 ; i < size () ; i++)
			mString[i] = val ;
	}

	template <class _ARG>
	void fill (const Array<ITEM ,_ARG> &val) {
		INDEX ir = 0 ;
		for (INDEX i = 0 ; i < size () ; i++) {
			mString[i] = val[ir] ;
			ir = (ir + 1) % val.size () ;
		}
	}

public:
	inline static String &from (Buffer<ITEM ,ARGC<cvt_size (SIZE::value)>> &me) {
		_DEBUG_ASSERT_ (me[me.size () - 1] == 0) ;
		return _CAST_<String> (me) ;
	}

	inline static const String &from (const Buffer<ITEM ,ARGC<cvt_size (SIZE::value)>> &me) {
		_DEBUG_ASSERT_ (me[me.size () - 1] == 0) ;
		return _CAST_<String> (me) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
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

public:
	using BUFFER = Buffer<ITEM ,SIZE> ;

private:
	Buffer<ITEM ,SIZE> mStack ;
	INDEX mWrite ;

public:
	Stack () {
		clear () ;
	}

	explicit Stack (LENGTH len) :mStack (len) {
		clear () ;
	}

	explicit Stack (BUFFER &&src) :mStack (std::move (src)) {
		clear () ;
	}

	implicit Stack (const std::initializer_list<ITEM> &right) : Stack (right.size ()) {
		for (auto &i : right)
			add (i) ;
	}

	LENGTH size () const {
		return mStack.size () ;
	}

	LENGTH length () const {
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
		return ~pos + length () ;
	}

	INDEX ibegin () const {
		return length () - 1 ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return index - 1 ;
	}

	BOOL equal (const Stack &right) const {
		if (length () != right.length ())
			return FALSE ;
		for (INDEX i = ibegin () ,ie = iend () ,j = right.ibegin () ,je = right.iend () ; i != ie && j != je ; i = inext (i) ,j = right.inext (j))
			if (get (i) != right.get (j))
				return FALSE ;
		return TRUE ;
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

	INDEX emplace () popping {
		if (full ()) {
			auto rax = mStack.expand () ;
			_MEMMOVE_ (rax.self ,mStack.self ,mWrite) ;
			mStack.expand (std::move (rax)) ;
		}
		INDEX ret = mWrite++ ;
		return std::move (ret) ;
	}

	void add (const ITEM &item) {
		if (full ()) {
			auto rax = mStack.expand () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mStack.self ,mWrite) ;
			mStack.expand (std::move (rax)) ;
		} else {
			mStack[mWrite] = std::move (item) ;
		}
		mWrite++ ;
	}

	inline Stack &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		if (full ()) {
			auto rax = mStack.expand () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mStack.self ,mWrite) ;
			mStack.expand (std::move (rax)) ;
		} else {
			mStack[mWrite] = std::move (item) ;
		}
		mWrite++ ;
	}

	inline Stack &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		const auto r1x = _MAX_ (arg.length () - (size () - length ()) ,VAR_ZERO) ;
		if (r1x > 0) {
			auto rax = mStack.expand (mStack.size () + r1x) ;
			_MEMMOVE_ (rax.self ,mStack.self ,mWrite) ;
			mStack.expand (std::move (rax)) ;
		}
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		const auto r1x = _MAX_ (arg.length () - (size () - length ()) ,VAR_ZERO) ;
		if (r1x > 0) {
			auto rax = mStack.expand (mStack.size () + r1x) ;
			_MEMMOVE_ (rax.self ,mStack.self ,mWrite) ;
			mStack.expand (std::move (rax)) ;
		}
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		const auto r1x = std::move (mStack[mWrite - 1]) ;
		(void) r1x ;
		mWrite-- ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mStack[mWrite - 1]) ;
		mWrite-- ;
	}

	inline Stack &operator>> (ITEM &item) {
		take (item) ;
		return *this ;
	}

	ITEM poll () popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		ITEM ret = std::move (mStack[mWrite - 1]) ;
		mWrite-- ;
		return std::move (ret) ;
	}

	Array<ITEM> poll (LENGTH len) popping {
		_DEBUG_ASSERT_ (len >= 0 && len <= length ()) ;
		Array<ITEM> ret = Array<ITEM> (len) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			take (ret[i]) ;
		return std::move (ret) ;
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
		OPERATOR_SORT_TYPE::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		OPERATOR_SORT_TYPE::invoke (_CAST_<AccessArray> (*this) ,0 ,length ()) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
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

	inline static constexpr LENGTH cvt_size (LENGTH len) {
		return len > 0 ? len + 1 : len ;
	}

public:
	using BUFFER = Buffer<ITEM ,ARGC<cvt_size (SIZE::value)>> ;

private:
	Buffer<ITEM ,ARGC<cvt_size (SIZE::value)>> mQueue ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	Queue () {
		clear () ;
	}

	explicit Queue (LENGTH len) :mQueue (cvt_size (len)) {
		clear () ;
	}

	explicit Queue (BUFFER &&src) :mQueue (std::move (src)) {
		clear () ;
	}

	implicit Queue (const std::initializer_list<ITEM> &right) : Queue (right.size ()) {
		for (auto &i : right)
			add (i) ;
	}

	LENGTH size () const {
		return mQueue.size () != 0 ? mQueue.size () - 1 : 0 ;
	}

	LENGTH length () const {
		return mQueue.size () != 0 ? (mWrite - mRead + mQueue.size ()) % mQueue.size () : 0 ;
	}

	void clear () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	//@error:index would be no longer valid once resized
	ITEM &get (INDEX index) {
		_DEBUG_ASSERT_ (ensure_index (index)) ;
		return mQueue[index] ;
	}

	inline ITEM &operator[] (INDEX index) {
		return get (index) ;
	}

	//@error:index would be no longer valid once resized
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
		return mRead ;
	}

	INDEX iend () const {
		return mWrite ;
	}

	INDEX inext (INDEX index) const {
		return (index + 1) % mQueue.size () ;
	}

	BOOL equal (const Queue &right) const {
		if (length () != right.length ())
			return FALSE ;
		for (INDEX i = ibegin () ,ie = iend () ,j = right.ibegin () ,je = right.iend () ; i != ie && j != je ; i = inext (i) ,j = right.inext (j))
			if (get (i) != right.get (j))
				return FALSE ;
		return TRUE ;
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
		return mQueue.size () != 0 ? mRead == (mWrite + 1) % mQueue.size () : TRUE ;
	}

	INDEX emplace () popping {
		if (full ()) {
			auto rax = mQueue.expand () ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		}
		INDEX ret = mWrite ;
		mWrite = (mWrite + 1) % mQueue.size () ;
		return std::move (ret) ;
	}

	void add (const ITEM &item) {
		if (full ()) {
			auto rax = mQueue.expand () ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		} else {
			mQueue[mWrite] = std::move (item) ;
		}
		mWrite = (mWrite + 1) % mQueue.size () ;
	}

	inline Queue &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		if (full ()) {
			auto rax = mQueue.expand () ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		} else {
			mQueue[mWrite] = std::move (item) ;
		}
		mWrite = (mWrite + 1) % mQueue.size () ;
	}

	inline Queue &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		const auto r1x = _MAX_ (arg.length () - (size () - length ()) ,VAR_ZERO) ;
		if (r1x > 0) {
			auto rax = mQueue.expand (mQueue.size () + r1x) ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		}
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		const auto r1x = _MAX_ (arg.length () - (size () - length ()) ,VAR_ZERO) ;
		if (r1x > 0) {
			auto rax = mQueue.expand (mQueue.size () + r1x) ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		}
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		const auto r1x = std::move (mQueue[mRead]) ;
		(void) r1x ;
		mRead = (mRead + 1) % mQueue.size () ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mQueue[mRead]) ;
		mRead = (mRead + 1) % mQueue.size () ;
	}

	inline Queue &operator>> (ITEM &item) {
		take (item) ;
		return *this ;
	}

	ITEM poll () popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		ITEM ret = std::move (mQueue[mRead]) ;
		mRead = (mRead + 1) % mQueue.size () ;
		return std::move (ret) ;
	}

	Array<ITEM> poll (LENGTH len) popping {
		_DEBUG_ASSERT_ (len >= 0 && len <= length ()) ;
		Array<ITEM> ret = Array<ITEM> (len) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			take (ret[i]) ;
		return std::move (ret) ;
	}

	void push (const ITEM &item) {
		if (full ()) {
			auto rax = mQueue.expand () ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			rax[(mRead - 1 + rax.size ()) % rax.size ()] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		} else {
			mQueue[(mRead - 1 + mQueue.size ()) % mQueue.size ()] = std::move (item) ;
		}
		mRead = (mRead - 1 + mQueue.size ()) % mQueue.size () ;
	}

	void push (ITEM &&item) {
		if (full ()) {
			auto rax = mQueue.expand () ;
			if (mRead > mWrite)
				mRead += rax.size () - mQueue.size () ;
			rax[(mRead - 1 + rax.size ()) % rax.size ()] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mQueue.self ,mWrite) ;
			if (mRead > mWrite)
				_MEMMOVE_ (&rax[mRead] ,&mQueue[mRead - (rax.size () - mQueue.size ())] ,rax.size () - mRead) ;
			mQueue.expand (std::move (rax)) ;
		} else {
			mQueue[(mRead - 1 + mQueue.size ()) % mQueue.size ()] = std::move (item) ;
		}
		mRead = (mRead - 1 + mQueue.size ()) % mQueue.size () ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite = (mWrite - 1 + mQueue.size ()) % mQueue.size () ;
	}

	void pop (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = (mWrite - 1 + mQueue.size ()) % mQueue.size () ;
		item = std::move (mQueue[ix]) ;
		mWrite = ix ;
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
		OPERATOR_SORT_TYPE::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		OPERATOR_SORT_TYPE::invoke (_CAST_<AccessArray> (*this) ,0 ,length ()) ;
	}

private:
	BOOL ensure_index (INDEX index) const {
		if (mRead <= mWrite && !(index >= mRead && index < mWrite))
			return FALSE ;
		if (mRead > mWrite && !(index >= 0 && index < mWrite) && !(index >= mRead && index < mQueue.size ()))
			return FALSE ;
		return TRUE ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class Priority {
public:
	using BUFFER = Buffer<ITEM ,SIZE> ;

private:
	Buffer<ITEM ,SIZE> mPriority ;
	INDEX mWrite ;

public:
	Priority () {
		clear () ;
	}

	explicit Priority (LENGTH len) :mPriority (len) {
		clear () ;
	}

	explicit Priority (BUFFER &&src) :mPriority (std::move (src)) {
		clear () ;
	}

	implicit Priority (const std::initializer_list<ITEM> &right) : Priority (right.size ()) {
		for (auto &i : right)
			add (i) ;
	}

	LENGTH size () const {
		return mPriority.size () ;
	}

	LENGTH length () const {
		return mWrite ;
	}

	void clear () {
		mWrite = 0 ;
	}

	//@error:index would be no longer valid every time revised
	const ITEM &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		return mPriority[index] ;
	}

	inline const ITEM &operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const ITEM &item) const {
		INDEX ret = mPriority.at (item) ;
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

	BOOL equal (const Priority &right) const {
		return length () == right.length () && _MEMEQUAL_ (mPriority ,right.mPriority ,right.length ()) ;
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

	void add (const ITEM &item) {
		if (full ()) {
			auto rax = mPriority.expand () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mPriority.self ,mWrite) ;
			mPriority.expand (std::move (rax)) ;
		} else {
			mPriority[mWrite] = std::move (item) ;
		}
		INDEX ix = mWrite++ ;
		update (ix) ;
	}

	inline Priority &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		if (full ()) {
			auto rax = mPriority.expand () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mPriority.self ,mWrite) ;
			mPriority.expand (std::move (rax)) ;
		} else {
			mPriority[mWrite] = std::move (item) ;
		}
		INDEX ix = mWrite++ ;
		update (ix) ;
	}

	inline Priority &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		const auto r1x = _MAX_ (arg.length () - (size () - length ()) ,VAR_ZERO) ;
		if (r1x > 0) {
			auto rax = mPriority.expand (mPriority.size () + r1x) ;
			_MEMMOVE_ (rax.self ,mPriority.self ,mWrite) ;
			mPriority.expand (std::move (rax)) ;
		}
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		const auto r1x = _MAX_ (arg.length () - (size () - length ()) ,VAR_ZERO) ;
		if (r1x > 0) {
			auto rax = mPriority.expand (mPriority.size () + r1x) ;
			_MEMMOVE_ (rax.self ,mPriority.self ,mWrite) ;
			mPriority.expand (std::move (rax)) ;
		}
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		mWrite-- ;
		mPriority[0] = std::move (mPriority[mWrite]) ;
		update (0) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		item = std::move (mPriority[0]) ;
		mWrite-- ;
		mPriority[0] = std::move (mPriority[mWrite]) ;
		update (0) ;
	}

	inline Priority &operator>> (ITEM &item) {
		take (item) ;
		return *this ;
	}

	ITEM poll () popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		ITEM ret = std::move (mPriority[0]) ;
		mWrite-- ;
		mPriority[0] = std::move (mPriority[mWrite]) ;
		update (0) ;
		return std::move (ret) ;
	}

	Array<ITEM> poll (LENGTH len) popping {
		_DEBUG_ASSERT_ (len >= 0 && len <= length ()) ;
		Array<ITEM> ret = Array<ITEM> (len) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			take (ret[i]) ;
		return std::move (ret) ;
	}

	INDEX insert (const ITEM &item) popping {
		if (full ()) {
			auto rax = mPriority.expand () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mPriority.self ,mWrite) ;
			mPriority.expand (std::move (rax)) ;
		} else {
			mPriority[mWrite] = std::move (item) ;
		}
		INDEX ret = mWrite++ ;
		update (ret) ;
		return std::move (ret) ;
	}

	INDEX insert (ITEM &&item) popping {
		if (full ()) {
			auto rax = mPriority.expand () ;
			rax[mWrite] = std::move (item) ;
			_MEMMOVE_ (rax.self ,mPriority.self ,mWrite) ;
			mPriority.expand (std::move (rax)) ;
		} else {
			mPriority[mWrite] = std::move (item) ;
		}
		INDEX ret = mWrite++ ;
		update (ret) ;
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < length ()) ;
		mPriority[index] = std::move (mPriority[mWrite - 1]) ;
		mWrite-- ;
		update (index) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	void erase (const ITEM &item) {
		INDEX ix = find (item) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		OPERATOR_SORT_TYPE::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

private:
	void update (INDEX it) {
		auto rax = std::move (mPriority[it]) ;
		while (TRUE) {
			//@info:'(0 - 1) >> 1' is not the same as '(0 - 1) / 2'
			INDEX ix = (it - 1) >> 1 ;
			if (ix < 0)
				break ;
			if (rax >= mPriority[ix])
				break ;
			mPriority[it] = std::move (mPriority[ix]) ;
			it = ix ;
		}
		while (TRUE) {
			INDEX ix = it * 2 + 1 ;
			if (ix >= length ())
				break ;
			INDEX iy = it ;
			if (mPriority[iy] > mPriority[ix])
				iy = ix ;
			ix = ix + 1 ;
			if (ix < length () && (iy != it ? mPriority[iy] : rax) > mPriority[ix])
				iy = ix ;
			if (iy == it)
				break ;
			mPriority[it] = std::move (mPriority[iy]) ;
			it = iy ;
		}
		mPriority[it] = std::move (rax) ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
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
	AutoBuffer<Buffer<INDEX ,ARGC<2>>> mDeque ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	using BUFFER = Allocator<Node ,SIZE> ;

public:
	Deque () {
		clear () ;
	}

	explicit Deque (LENGTH len) :mList (len) {
		mDeque = AutoBuffer<Buffer<INDEX ,ARGC<2>>> (mList.size ()) ;
		clear () ;
	}

	explicit Deque (BUFFER &&src) :mList (std::move (src)) {
		mDeque = AutoBuffer<Buffer<INDEX ,ARGC<2>>> (mList.size ()) ;
		clear () ;
	}

	explicit Deque (BUFFER &&src ,AutoBuffer<Buffer<INDEX ,ARGC<2>>> &&ext) :mList (std::move (src)) {
		mDeque = std::move (ext) ;
		_DEBUG_ASSERT_ (mDeque.size () == mList.size ()) ;
		clear () ;
	}

	implicit Deque (const std::initializer_list<ITEM> &right) : Deque (right.size ()) {
		for (auto &i : right)
			add (i) ;
	}

	LENGTH size () const {
		return mList.size () ;
	}

	LENGTH length () const {
		return mList.length () ;
	}

	void clear () {
		_MEMFILL_ (mDeque.self ,mDeque.size () ,Buffer<INDEX ,ARGC<2>> ({VAR_NONE ,VAR_ZERO})) ;
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
		return mList.at (_OFFSET_ (&Node::mItem ,&item)) ;
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
		for (INDEX i = mRead ; i <= mWrite ; i++)
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
		for (INDEX i = ibegin () ,ie = iend () ,j = right.ibegin () ,je = right.iend () ; i != ie && j != je ; i = inext (i) ,j = right.inext (j))
			if (get (i) != right.get (j))
				return FALSE ;
		return TRUE ;
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

	INDEX emplace () popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		update_resize (ret) ;
		update_compress_left (mWrite ,ret) ;
		return std::move (ret) ;
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
	void appand (const _ARG &arg) {
		mList.reserve (arg.length ()) ;
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mList.reserve (arg.length ()) ;
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mRead][0] == VAR_NONE)
			mRead++ ;
		INDEX ix = mDeque[mRead][0] ;
		update_remove (mRead) ;
		mList.free (ix) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mRead][0] == VAR_NONE)
			mRead++ ;
		INDEX ix = mDeque[mRead][0] ;
		item = std::move (mList[ix].mItem) ;
		update_remove (mRead) ;
		mList.free (ix) ;
	}

	inline Deque &operator>> (ITEM &item) {
		take (item) ;
		return *this ;
	}

	ITEM poll () popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mRead][0] == VAR_NONE)
			mRead++ ;
		INDEX ix = mDeque[mRead][0] ;
		ITEM ret = std::move (mList[ix].mItem) ;
		update_remove (mRead) ;
		mList.free (ix) ;
		return std::move (ret) ;
	}

	Array<ITEM> poll (LENGTH len) popping {
		_DEBUG_ASSERT_ (len >= 0 && len <= length ()) ;
		Array<ITEM> ret = Array<ITEM> (len) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			take (ret[i]) ;
		return std::move (ret) ;
	}

	void push (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ix) ;
		update_compress_right (mRead ,ix) ;
	}

	void push (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ix) ;
		update_compress_right (mRead ,ix) ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mWrite][0] == VAR_NONE)
			mWrite-- ;
		INDEX ix = mDeque[mWrite][0] ;
		update_remove (mWrite) ;
		mList.free (ix) ;
	}

	void pop (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		while (mDeque[mWrite][0] == VAR_NONE)
			mWrite-- ;
		INDEX ix = mDeque[mWrite][0] ;
		update_remove (mWrite) ;
		mList.free (ix) ;
	}

	INDEX insert (INDEX index ,const ITEM &item) popping {
		INDEX ret = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ret) ;
		update_compress_right (mList[index].mHead ,ret) ;
		return std::move (ret) ;
	}

	INDEX insert (INDEX index ,ITEM &&item) popping {
		INDEX ret = mList.alloc (std::move (item) ,VAR_NONE) ;
		update_resize (ret) ;
		update_compress_right (mList[index].mHead ,ret) ;
		return std::move (ret) ;
	}

	void swap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		update_rewrite (mList[index1].mHead ,index2) ;
		update_rewrite (mList[index2].mHead ,index1) ;
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

	void erase (const ITEM &item) {
		INDEX ix = find (item) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		OPERATOR_SORT_TYPE::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		for (INDEX i = 0 ; i < r1x.length () ; i++)
			update_rewrite (i ,r1x[i]) ;
		for (INDEX i = r1x.size () ; i < mDeque.size () ; i++)
			update_remove (i) ;
	}

private:
	INDEX access (INDEX pos ,INDEX it ,INDEX jt) const {
		INDEX ret = VAR_NONE ;
		while (TRUE) {
			if (it > jt)
				break ;
			ret = it + (jt - it + 1) / 2 ;
			INDEX ix = position_before (ret) ;
			if (ix == pos && mDeque[ret][0] != VAR_NONE)
				break ;
			if (ix < pos) {
				it = ret + 1 ;
			} else {
				jt = ret - 1 ;
			}
		}
		ret = mDeque[ret][0] ;
		return std::move (ret) ;
	}

	INDEX position_before (INDEX it) const {
		INDEX ret = 0 ;
		while (it >= 0) {
			ret += mDeque[it][1] ;
			it++ ;
			it -= it & -it ;
			it-- ;
		}
		ret-- ;
		return std::move (ret) ;
	}

	void update_resize (INDEX it) {
		if (mDeque.size () == mList.size ())
			return ;
		mDeque = AutoBuffer<Buffer<INDEX ,ARGC<2>>> (mList.size ()) ;
		_MEMFILL_ (mDeque.self ,mDeque.size () ,Buffer<INDEX ,ARGC<2>> ({VAR_NONE ,VAR_ZERO})) ;
		for (INDEX i = 0 ; i < mList.size () ; i++)
			if (mList.used (i) && i != it)
				update_rewrite (mList[i].mHead ,i) ;
	}

	void update_compress_left (INDEX it ,INDEX jt) {
		if (mDeque[it][0] == VAR_NONE) {
			update_rewrite (it ,jt) ;
			if (mWrite == it)
				mWrite = _MIN_ (mWrite + 1 ,mDeque.size () - 1) ;
		} else if (it + 1 < mDeque.size () && mDeque[it + 1][0] == VAR_NONE) {
			update_rewrite (it + 1 ,jt) ;
			if (mWrite == it)
				mWrite = _MIN_ (mWrite + 2 ,mDeque.size () - 1) ;
		} else {
			for (INDEX i = 0 ; i < mList.length () ; i++) {
				while (mRead != it && mDeque[mRead][0] == VAR_NONE)
					mRead++ ;
				if (mRead == i && mRead != it && mDeque[mRead][0] != VAR_NONE) {
					mRead++ ;
				} else if (mRead == it) {
					INDEX ix = mDeque[i][0] ;
					update_rewrite (i ,jt) ;
					jt = ix ;
					it++ ;
					mRead++ ;
				} else {
					update_rewrite (i ,mDeque[mRead][0]) ;
					update_remove (mRead) ;
					mRead++ ;
				}
			}
			mRead = 0 ;
			mWrite = _MIN_ (mList.length () ,mDeque.size () - 1) ;
		}
	}

	void update_compress_right (INDEX it ,INDEX jt) {
		if (mDeque[it][0] == VAR_NONE) {
			update_rewrite (it ,jt) ;
			if (mRead == it)
				mRead = _MAX_ (mRead - 1 ,VAR_ZERO) ;
		} else if (it - 1 >= 0 && mDeque[it - 1][0] == VAR_NONE) {
			update_rewrite (it - 1 ,jt) ;
			if (mRead == it)
				mRead = _MAX_ (mRead - 2 ,VAR_ZERO) ;
		} else {
			for (INDEX i = mDeque.size () - 1 ,je = mDeque.size () - mList.length () ; i >= je ; i--) {
				while (mWrite != it && mDeque[mWrite][0] == VAR_NONE)
					mWrite-- ;
				if (mWrite == i && mWrite != it && mDeque[mWrite][0] != VAR_NONE) {
					mWrite-- ;
				} else if (mWrite == it) {
					INDEX ix = mDeque[i][0] ;
					update_rewrite (i ,jt) ;
					jt = ix ;
					it-- ;
					mWrite-- ;
				} else {
					update_rewrite (i ,mDeque[mWrite][0]) ;
					update_remove (mWrite) ;
					mWrite-- ;
				}
			}
			mRead = _MAX_ (mDeque.size () - 1 - mList.length () ,VAR_ZERO) ;
			mWrite = mDeque.size () - 1 ;
		}
	}

	void update_rewrite (INDEX it ,INDEX jt) {
		const auto r1x = mDeque[it][0] ;
		mDeque[it][0] = jt ;
		mList[jt].mHead = it ;
		if (r1x != VAR_NONE)
			return ;
		while (it < mDeque.size ()) {
			mDeque[it][1]++ ;
			it++ ;
			it += it & -it ;
			it-- ;
		}
	}

	void update_remove (INDEX it) {
		mDeque[it][0] = VAR_NONE ;
		while (it < mDeque.size ()) {
			mDeque[it][1]-- ;
			it++ ;
			it += it & -it ;
			it-- ;
		}
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
class SList {
private:
	class Node {
	private:
		friend SList ;
		ITEM mItem ;
		INDEX mRight ;

	public:
		inline Node () = delete ;

		inline explicit Node (INDEX right) :mRight (right) {}

		inline explicit Node (const ITEM &item ,INDEX right) : mItem (std::move (item)) ,mRight (right) {}

		inline explicit Node (ITEM &&item ,INDEX right) : mItem (std::move (item)) ,mRight (right) {}
	} ;

public:
	using BUFFER = Allocator<Node ,SIZE> ;

private:
	Allocator<Node ,SIZE> mList ;
	INDEX mLast ;

public:
	SList () {
		clear () ;
	}

	explicit SList (LENGTH len) :mList (len) {
		clear () ;
	}

	explicit SList (BUFFER &&src) :mList (std::move (src)) {
		clear () ;
	}

	implicit SList (const std::initializer_list<ITEM> &right) : SList (right.size ()) {
		for (auto &i : right)
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
		return mList.at (_OFFSET_ (&Node::mItem ,&item)) ;
	}

	INDEX ibegin () const {
		return mLast != VAR_NONE ? mList[mLast].mRight : VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return index != mLast ? mList[index].mRight : VAR_NONE ;
	}

	BOOL equal (const SList &right) const {
		if (length () != right.length ())
			return FALSE ;
		for (INDEX i = ibegin () ,ie = iend () ,j = right.ibegin () ,je = right.iend () ; i != ie && j != je ; i = inext (i) ,j = right.inext (j))
			if (get (i) != right.get (j))
				return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const SList &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SList &right) const {
		return !equal (right) ;
	}

	BOOL empty () const {
		return mLast == VAR_NONE ;
	}

	BOOL full () const {
		return mList.length () == mList.size () ;
	}

	INDEX emplace () popping {
		INDEX ret = mList.alloc (VAR_NONE) ;
		mList[ret].mRight = mLast != VAR_NONE ? mList[mLast].mRight : ret ;
		(mLast != VAR_NONE ? mList[mLast].mRight : mLast) = ret ;
		return std::move (ret) ;
	}

	void add (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		mList[ix].mRight = mLast != VAR_NONE ? mList[mLast].mRight : ix ;
		(mLast != VAR_NONE ? mList[mLast].mRight : mLast) = ix ;
	}

	inline SList &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE) ;
		mList[ix].mRight = mLast != VAR_NONE ? mList[mLast].mRight : ix ;
		(mLast != VAR_NONE ? mList[mLast].mRight : mLast) = ix ;
	}

	inline SList &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		mList.reserve (arg.length ()) ;
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mList.reserve (arg.length ()) ;
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mList[mLast].mRight ;
		INDEX iy = ix != mLast ? mList[ix].mRight : VAR_NONE ;
		(iy != VAR_NONE ? mList[mLast].mRight : mLast) = iy ;
		mList.free (ix) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mList[mLast].mRight ;
		item = std::move (mList[ix].mItem) ;
		INDEX iy = ix != mLast ? mList[ix].mRight : VAR_NONE ;
		(iy != VAR_NONE ? mList[mLast].mRight : mLast) = iy ;
		mList.free (ix) ;
	}

	inline SList &operator>> (ITEM &item) {
		take (item) ;
		return *this ;
	}

	ITEM poll () popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mList[mLast].mRight ;
		ITEM ret = std::move (mList[ix].mItem) ;
		INDEX iy = ix != mLast ? mList[ix].mRight : VAR_NONE ;
		(iy != VAR_NONE ? mList[mLast].mRight : mLast) = iy ;
		mList.free (ix) ;
		return std::move (ret) ;
	}

	Array<ITEM> poll (LENGTH len) popping {
		_DEBUG_ASSERT_ (len >= 0 && len <= length ()) ;
		Array<ITEM> ret = Array<ITEM> (len) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			take (ret[i]) ;
		return std::move (ret) ;
	}

	INDEX insert_after (INDEX index ,const ITEM &item) popping {
		INDEX ret = mList.alloc (std::move (item) ,mList[index].mRight) ;
		mList[index].mRight = ret ;
		if (mLast == index)
			mLast = ret ;
		return std::move (ret) ;
	}

	INDEX insert_after (INDEX index ,ITEM &&item) popping {
		INDEX ret = mList.alloc (std::move (item) ,mList[index].mRight) ;
		mList[index].mRight = ret ;
		if (mLast == index)
			mLast = ret ;
		return std::move (ret) ;
	}

	void swap_after (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1 = mList[index1].mRight ;
		auto &r2 = mList[index2].mRight ;
		index1 = r1 ;
		index2 = r2 ;
		r1 = index2 ;
		r2 = index1 ;
		_SWAP_ (mList[index1].mRight ,mList[index2].mRight) ;
		if (mLast == index1 || mLast == index2)
			mLast = mLast != index1 ? index1 : index2 ;
	}

	void remove_after (INDEX index) {
		auto &r1 = mList[index].mRight ;
		INDEX ix = r1 ;
		r1 = mList[ix].mRight ;
		if (mLast == ix)
			mLast = mLast != index ? index : VAR_NONE ;
		mList.free (ix) ;
	}

	INDEX find (const ITEM &item) const {
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			if (get (i) == item)
				return i ;
		return VAR_NONE ;
	}

	INDEX find_before (const ITEM &item) const {
		if (mLast == VAR_NONE || mList[mList[mLast].mRight].mItem == item)
			return mLast ;
		for (INDEX i = mList[mLast].mRight ; i != mLast ; i = mList[i].mRight)
			if (mList[mList[i].mRight].mItem == item)
				return i ;
		return VAR_NONE ;
	}

	void erase (const ITEM &item) {
		INDEX ix = find_before (item) ;
		if (ix != VAR_NONE)
			remove_after (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		OPERATOR_SORT_TYPE::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		for (INDEX i = 0 ; i + 1 < r1x.length () ; i++)
			mList[r1x[i]].mRight = r1x[i + 1] ;
		mLast = r1x[r1x.size () - 1] ;
		mList[mLast].mRight = r1x[0] ;
	}
} ;

template <class ITEM ,class SIZE = SAUTO>
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

public:
	using BUFFER = Allocator<Node ,SIZE> ;

private:
	Allocator<Node ,SIZE> mList ;
	INDEX mFirst ;
	INDEX mLast ;

public:
	QList () {
		clear () ;
	}

	explicit QList (LENGTH len) :mList (len) {
		clear () ;
	}

	explicit QList (BUFFER &&src) :mList (std::move (src)) {
		clear () ;
	}

	implicit QList (const std::initializer_list<ITEM> &right) : QList (right.size ()) {
		for (auto &i : right)
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
		return mList.at (_OFFSET_ (&Node::mItem ,&item)) ;
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
		for (INDEX i = ibegin () ,ie = iend () ,j = right.ibegin () ,je = right.iend () ; i != ie && j != je ; i = inext (i) ,j = right.inext (j))
			if (get (i) != right.get (j))
				return FALSE ;
		return TRUE ;
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

	INDEX emplace () popping {
		INDEX ret = mList.alloc (mLast ,VAR_NONE) ;
		(mLast != VAR_NONE ? mList[mLast].mRight : mFirst) = ret ;
		mLast = ret ;
		return std::move (ret) ;
	}

	void add (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,mLast ,VAR_NONE) ;
		(mLast != VAR_NONE ? mList[mLast].mRight : mFirst) = ix ;
		mLast = ix ;
	}

	inline QList &operator<< (const ITEM &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,mLast ,VAR_NONE) ;
		(mLast != VAR_NONE ? mList[mLast].mRight : mFirst) = ix ;
		mLast = ix ;
	}

	inline QList &operator<< (ITEM &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		mList.reserve (arg.length ()) ;
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mList.reserve (arg.length ()) ;
		for (auto &i : arg)
			add (std::move (i)) ;
	}

	void take () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		mFirst = mList[mFirst].mRight ;
		if (mFirst == VAR_NONE)
			mLast = VAR_NONE ;
		mList.free (ix) ;
	}

	void take (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		item = std::move (mList[mFirst].mItem) ;
		mFirst = mList[mFirst].mRight ;
		if (mFirst == VAR_NONE)
			mLast = VAR_NONE ;
		mList.free (ix) ;
	}

	inline QList &operator>> (ITEM &item) {
		take (item) ;
		return *this ;
	}

	ITEM poll () popping {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mFirst ;
		ITEM ret = std::move (mList[mFirst].mItem) ;
		mFirst = mList[mFirst].mRight ;
		if (mFirst == VAR_NONE)
			mLast = VAR_NONE ;
		mList.free (ix) ;
		return std::move (ret) ;
	}

	Array<ITEM> poll (LENGTH len) popping {
		_DEBUG_ASSERT_ (len >= 0 && len <= length ()) ;
		Array<ITEM> ret = Array<ITEM> (len) ;
		for (INDEX i = 0 ; i < ret.length () ; i++)
			take (ret[i]) ;
		return std::move (ret) ;
	}

	void push (const ITEM &item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE ,mFirst) ;
		(mFirst != VAR_NONE ? mList[mFirst].mLeft : mLast) = ix ;
		mFirst = ix ;
	}

	void push (ITEM &&item) {
		INDEX ix = mList.alloc (std::move (item) ,VAR_NONE ,mFirst) ;
		(mFirst != VAR_NONE ? mList[mFirst].mLeft : mLast) = ix ;
		mFirst = ix ;
	}

	void pop () {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mLast ;
		mLast = mList[mFirst].mLeft ;
		if (mLast == VAR_NONE)
			mFirst = VAR_NONE ;
		mList.free (ix) ;
	}

	void pop (ITEM &item) {
		_DEBUG_ASSERT_ (!empty ()) ;
		INDEX ix = mLast ;
		item = std::move (mList[mLast].mItem) ;
		mLast = mList[mFirst].mLeft ;
		if (mLast == VAR_NONE)
			mFirst = VAR_NONE ;
		mList.free (ix) ;
	}

	INDEX insert (INDEX index ,const ITEM &item) popping {
		INDEX ret = mList.alloc (std::move (item) ,mList[index].mLeft ,index) ;
		prev_next (index) = ret ;
		mList[index].mLeft = ret ;
		return std::move (ret) ;
	}

	INDEX insert (INDEX index ,ITEM &&item) popping {
		INDEX ret = mList.alloc (std::move (item) ,mList[index].mLeft ,index) ;
		prev_next (index) = ret ;
		mList[index].mLeft = ret ;
		return std::move (ret) ;
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

	void erase (const ITEM &item) {
		INDEX ix = find (item) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		for (INDEX i = ibegin () ,ie = iend () ; i != ie ; i = inext (i))
			ret[iw++] = i ;
		OPERATOR_SORT_TYPE::invoke (*this ,ret ,0 ,ret.size ()) ;
		return std::move (ret) ;
	}

	void sort () {
		if (length () < 2)
			return ;
		const auto r1x = esort () ;
		mFirst = r1x[0] ;
		mList[mFirst].mLeft = VAR_NONE ;
		mList[mFirst].mRight = r1x[1] ;
		for (INDEX i = 1 ; i + 1 < r1x.length () ; i++) {
			mList[r1x[i]].mLeft = r1x[i - 1] ;
			mList[r1x[i]].mRight = r1x[i + 1] ;
		}
		mLast = r1x[r1x.length () - 1] ;
		mList[mLast].mLeft = r1x[r1x.length () - 2] ;
		mList[mLast].mRight = VAR_NONE ;
	}

private:
	INDEX & prev_next (INDEX it) {
		return mList[it].mLeft != VAR_NONE ? mList[mList[it].mLeft].mRight : mFirst ;
	}

	INDEX &next_prev (INDEX it) {
		return mList[it].mRight != VAR_NONE ? mList[mList[it].mRight].mLeft : mLast ;
	}
} ;

template <class SIZE = SAUTO>
class BitSet {
private:
	inline static constexpr LENGTH cvt_size (LENGTH len) {
		return len > 0 ? (len + 7) / 8 : len ;
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

		inline Bit &&operator= (const BOOL &right) && {
			const auto r1x = right ? ~mBase.mSet[mIndex / 8] : mBase.mSet[mIndex / 8] ;
			mBase.mSet[mIndex / 8] ^= r1x & (BYTE (0X01) << (mIndex % 8)) ;
			return std::move (*this) ;
		}

		inline Bit &&operator= (Bit<BitSet> &&right) && {
			std::move (*this) = BOOL (std::move (right)) ;
			return std::move (*this) ;
		}

		inline Bit &&operator= (Bit<const BitSet> &&right) && {
			std::move (*this) = BOOL (std::move (right)) ;
			return std::move (*this) ;
	}

	private:
		inline explicit Bit (BASE &base ,INDEX index) :mBase (base) ,mIndex (index) {}
} ;

private:
	Buffer<BYTE ,ARGC<cvt_size (SIZE::value)>> mSet ;
	LENGTH mWidth ;

public:
	BitSet () {
		mWidth = 0 ;
		clear () ;
	}

	explicit BitSet (LENGTH len) :mSet (cvt_size (len)) {
		_DEBUG_ASSERT_ (len >= 0 && len <= VAR32_MAX) ;
		mWidth = len ;
		clear () ;
	}

	implicit BitSet (const std::initializer_list<INDEX> &right) :BitSet (cvt_size (right)) {
		for (auto &i : right)
			get (i) = TRUE ;
	}

	LENGTH size () const {
		return mWidth ;
	}

	LENGTH length () const {
		static constexpr ARR<VAR32> M_TABLE = {
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
			4 ,5 ,5 ,6 ,5 ,6 ,6 ,7 ,5 ,6 ,6 ,7 ,6 ,7 ,7 ,8} ;
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mSet.size () ; i++)
			ret += M_TABLE[mSet[i]] ;
		if (mWidth % 8 != 0)
			ret -= M_TABLE[mSet[mWidth / 8] & ~((VAR (1) << (mWidth % 8)) - 1)] ;
		return std::move (ret) ;
	}

	void clear () {
		fill (BYTE (0X00)) ;
	}

	//@info:'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	Bit<BitSet> get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit<BitSet> (*this ,index) ;
	}

	inline Bit<BitSet> operator[] (INDEX index) {
		return get (index) ;
	}

	//@info:'Bit &&' convert to 'BOOL' implicitly while 'const Bit &' convert to 'VAR' implicitly
	Bit<const BitSet> get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < mWidth) ;
		return Bit<const BitSet> (*this ,index) ;
	}

	inline Bit<const BitSet> operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const Bit<BitSet> &item) const {
		return this == &item.mBase ? INDEX (item) : VAR_NONE ;
	}

	INDEX at (const Bit<const BitSet> &item) const {
		return this == &item.mBase ? INDEX (item) : VAR_NONE ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (get (i))
				return i ;
		return size () ;
	}

	INDEX iend () const {
		return size () ;
	}

	INDEX inext (INDEX index) const {
		for (index++ ; index < size () && !get (index) ; index++)
			(void) index ;
		return index ;
	}

	BOOL equal (const BitSet &right) const {
		_DEBUG_ASSERT_ (size () == right.size ()) ;
		INDEX ix = mSet.size () - 1 ;
		for (INDEX i = 0 ; i < ix ; i++)
			if (mSet[i] != right.mSet[i])
				return FALSE ;
		return ix < 0 || (mSet[ix] & (mWidth % 8 - 1)) == (right.mSet[ix] & (mWidth % 8 - 1)) ;
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
		for (INDEX i = 0 ; i < ix ; i++)
			if (mSet[i] != right.mSet[i])
				return mSet[i] < right.mSet[i] ;
		return ix >= 0 && (mSet[ix] & (mWidth % 8 - 1)) < (right.mSet[ix] & (mWidth % 8 - 1)) ;
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

	void add (INDEX index) {
		get (index) = TRUE ;
	}

	inline BitSet &operator<< (INDEX index) {
		add (index) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		for (const auto &i : arg)
			add (i) ;
	}

	void remove (INDEX index) {
		get (index) = FALSE ;
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
	static LENGTH cvt_size (const std::initializer_list<INDEX> &right) {
		LENGTH ret = VAR_MIN ;
		for (auto &i : right)
			ret = _MAX_ (ret ,i) ;
		_DEBUG_ASSERT_ (ret >= 0) ;
		ret++ ;
		return std::move (ret) ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class Set {
private:
	template <class BASE>
	class Pair {
	private:
		friend Set ;

	public:
		const KEY & key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () const {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) ,item (base.mSet[index].mItem) {}
	} ;

	class Node {
	private:
		friend Set ;
		template <class>
		friend class Pair ;
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

		inline explicit Node (const KEY &key ,const ITEM &item ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mItem (std::move (item)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,BOOL red ,INDEX up ,INDEX left ,INDEX right) : mKey (std::move (key)) ,mItem (std::move (item)) ,mRed (red) ,mUp (up) ,mLeft (left) ,mRight (right) {}
	} ;

public:
	using BUFFER = Allocator<Node ,SIZE> ;

private:
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;

public:
	Set () {
		clear () ;
	}

	explicit Set (LENGTH len) :mSet (len) {
		clear () ;
	}

	explicit Set (BUFFER &&src) :mSet (std::move (src)) {
		clear () ;
	}

	implicit Set (const std::initializer_list<PACK<KEY ,ITEM>> &right) : Set (right.size ()) {
		for (auto &i : right)
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

	INDEX at (const Pair<Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX at (const Pair<const Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return size () ;
	}

	INDEX iend () const {
		return size () ;
	}

	INDEX inext (INDEX index) const {
		for (index++ ; index < size () && !mSet.used (index) ; index++) {
		}
		return index ;
	}

	BOOL equal (const Set &right) const {
		return equal (right ,mRoot ,right.mRoot) ;
	}

	inline BOOL operator== (const Set &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Set &right) const {
		return !equal (right) ;
	}

	void add (const KEY &key ,const ITEM &item) {
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		insert_next (mRoot ,ix) ;
		update_insert (ix) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		insert_next (mRoot ,ix) ;
		update_insert (ix) ;
	}

	void add (const PACK<KEY ,ITEM> &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	inline Set &operator<< (const PACK<KEY ,ITEM> &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (KEY &&key ,const ITEM &item) {
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		insert_next (mRoot ,ix) ;
		update_insert (ix) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		insert_next (mRoot ,ix) ;
		update_insert (ix) ;
	}

	void add (PACK<KEY ,ITEM> &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	inline Set &operator<< (PACK<KEY ,ITEM> &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			insert_next (mRoot ,ret) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			insert_next (mRoot ,ret) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		if (mSet[index].mLeft != VAR_NONE && mSet[index].mRight != VAR_NONE)
			swap (index ,successor_one (index)) ;
		INDEX ix = mSet[index].mLeft != VAR_NONE ? mSet[index].mLeft : mSet[index].mRight ;
		prev_next (index) = ix ;
		if (ix != VAR_NONE)
			mSet[ix].mUp = mSet[index].mUp ;
		if (!mSet[index].mRed)
			update_remove (ix ,mSet[index].mUp) ;
		mSet.free (index) ;
	}

	INDEX min_one () const {
		INDEX ret = mRoot ;
		if (ret != VAR_NONE) {
			while (mSet[ret].mLeft != VAR_NONE)
				ret = mSet[ret].mLeft ;
		}
		return std::move (ret) ;
	}

	INDEX max_one () const {
		INDEX ret = mRoot ;
		if (ret != VAR_NONE) {
			while (mSet[ret].mRight != VAR_NONE)
				ret = mSet[ret].mRight ;
		}
		return std::move (ret) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = key < mSet[ret].mKey ;
			if (!r1x && !(mSet[ret].mKey < key))
				break ;
			ret = r1x ? mSet[ret].mLeft : mSet[ret].mRight ;
		}
		return std::move (ret) ;
	}

	void erase (const KEY &key) {
		INDEX ix = find (key) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		esort (mRoot ,ret ,iw) ;
		return std::move (ret) ;
	}

private:
	BOOL equal (const Set &right ,INDEX it ,INDEX jt) const {
		if (it == VAR_NONE || jt == VAR_NONE)
			return it == VAR_NONE && jt == VAR_NONE ;
		if (!equal (right ,mSet[it].mLeft ,right.mSet[jt].mLeft))
			return FALSE ;
		if (mSet[it].mKey != right.mSet[jt].mKey)
			return FALSE ;
		if (!equal (right ,mSet[it].mRight ,right.mSet[jt].mRight))
			return FALSE ;
		return TRUE ;
	}

	void insert_next (INDEX &it ,INDEX jt) {
		if (it == VAR_NONE) {
			it = jt ;
			return ;
		}
		mSet[jt].mUp = it ;
		auto &r1 = mSet[jt].mKey < mSet[it].mKey ? mSet[it].mLeft : mSet[it].mRight ;
		insert_next (r1 ,jt) ;
	}

	void update_insert (INDEX it) {
		while (TRUE) {
			INDEX ix = mSet[it].mUp ;
			if (ix == VAR_NONE || !mSet[ix].mRed)
				break ;
			const auto r1x = ix == mSet[mSet[ix].mUp].mLeft ? &Set::update_insert_left : &Set::update_insert_right ;
			(this->*r1x) (it) ;
		}
		mSet[mRoot].mRed = FALSE ;
	}

	void update_insert_left (INDEX &it) {
		INDEX ix = mSet[it].mUp ;
		INDEX iy = mSet[ix].mUp ;
		if (mSet[iy].mRight != VAR_NONE && mSet[mSet[iy].mRight].mRed) {
			mSet[mSet[iy].mRight].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			it = iy ;
		} else if (it == mSet[ix].mRight) {
			rotate_left (mSet[iy].mLeft) ;
			mSet[it].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_right (prev_next (iy)) ;
			it = ix ;
		} else {
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_right (prev_next (iy)) ;
		}
	}

	void update_insert_right (INDEX &it) {
		INDEX ix = mSet[it].mUp ;
		INDEX iy = mSet[ix].mUp ;
		if (mSet[iy].mLeft != VAR_NONE && mSet[mSet[iy].mLeft].mRed) {
			mSet[mSet[iy].mLeft].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			it = iy ;
		} else if (it == mSet[ix].mLeft) {
			rotate_right (mSet[iy].mRight) ;
			mSet[it].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_left (prev_next (iy)) ;
			it = ix ;
		} else {
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_left (prev_next (iy)) ;
		}
	}

	void update_remove (INDEX it ,INDEX jt) {
		while (TRUE) {
			if (jt == VAR_NONE)
				break ;
			if (it != VAR_NONE && mSet[it].mRed)
				break ;
			const auto r1x = it == mSet[jt].mLeft ? &Set::update_remove_left : &Set::update_remove_right ;
			(this->*r1x) (it ,jt) ;
			jt = mSet[it].mUp ;
		}
		if (it != VAR_NONE)
			mSet[it].mRed = FALSE ;
	}

	void update_remove_left (INDEX &it ,INDEX jt) {
		auto &r1 = mSet[jt].mRight ;
		if (mSet[r1].mRed) {
			mSet[r1].mRed = FALSE ;
			mSet[jt].mRed = TRUE ;
			rotate_left (prev_next (jt)) ;
		}
		if ((mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed) && (mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed)) {
			mSet[r1].mRed = TRUE ;
			it = jt ;
		} else if (mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed) {
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			mSet[r1].mRed = TRUE ;
			rotate_right (r1) ;
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mRight].mRed = FALSE ;
			rotate_left (prev_next (jt)) ;
			it = mRoot ;
		} else {
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mRight].mRed = FALSE ;
			rotate_left (prev_next (jt)) ;
			it = mRoot ;
		}
	}

	void update_remove_right (INDEX &it ,INDEX jt) {
		auto &r1 = mSet[jt].mLeft ;
		if (mSet[r1].mRed) {
			mSet[r1].mRed = FALSE ;
			mSet[jt].mRed = TRUE ;
			rotate_right (prev_next (jt)) ;
		}
		if ((mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed) && (mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed)) {
			mSet[r1].mRed = TRUE ;
			it = jt ;
		} else if (mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed) {
			mSet[mSet[r1].mRight].mRed = FALSE ;
			mSet[r1].mRed = TRUE ;
			rotate_left (r1) ;
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			rotate_right (prev_next (jt)) ;
			it = mRoot ;
		} else {
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			rotate_right (prev_next (jt)) ;
			it = mRoot ;
		}
	}

	void rotate_left (INDEX &it) {
		INDEX ix = mSet[it].mRight ;
		mSet[it].mRight = mSet[ix].mLeft ;
		if (mSet[ix].mLeft != VAR_NONE)
			mSet[mSet[ix].mLeft].mUp = it ;
		mSet[ix].mLeft = it ;
		mSet[ix].mUp = mSet[it].mUp ;
		mSet[it].mUp = ix ;
		it = ix ;
	}

	void rotate_right (INDEX &it) {
		INDEX ix = mSet[it].mLeft ;
		mSet[it].mLeft = mSet[ix].mRight ;
		if (mSet[ix].mRight != VAR_NONE)
			mSet[mSet[ix].mRight].mUp = it ;
		mSet[ix].mRight = it ;
		mSet[ix].mUp = mSet[it].mUp ;
		mSet[it].mUp = ix ;
		it = ix ;
	}

	INDEX &prev_next (INDEX it) {
		return mSet[it].mUp == VAR_NONE ? mRoot : mSet[mSet[it].mUp].mLeft == it ? mSet[mSet[it].mUp].mLeft : mSet[mSet[it].mUp].mRight ;
	}

	INDEX successor_one (INDEX index) const {
		for (index = mSet[index].mRight ; mSet[index].mLeft != VAR_NONE ; index = mSet[index].mLeft)
			(void) index ;
		return index ;
	}

	void swap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1 = prev_next (index2) ;
		auto &r2 = mSet[index2].mLeft != VAR_NONE ? mSet[mSet[index2].mLeft].mUp : index1 ;
		auto &r3 = mSet[index2].mRight != VAR_NONE ? mSet[mSet[index2].mRight].mUp : index1 ;
		auto &r4 = prev_next (index1) ;
		auto &r5 = mSet[index1].mLeft != VAR_NONE ? mSet[mSet[index1].mLeft].mUp : index2 ;
		auto &r6 = mSet[index1].mRight != VAR_NONE ? mSet[mSet[index1].mRight].mUp : index2 ;
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

	void esort (INDEX it ,Array<INDEX> &dst ,INDEX &iw) const {
		if (it == VAR_NONE)
			return ;
		esort (mSet[it].mLeft ,dst ,iw) ;
		dst[iw++] = it ;
		esort (mSet[it].mRight ,dst ,iw) ;
	}
} ;

template <class KEY ,class SIZE>
class Set<KEY ,void ,SIZE> {
private:
	class Node {
	private:
		friend Set ;
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

	template <class BASE>
	class Pair {
	private:
		friend Set ;

	public:
		const KEY & key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () const {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) {}
	} ;

public:
	using BUFFER = Allocator<Node ,SIZE> ;

private:
	Allocator<Node ,SIZE> mSet ;
	INDEX mRoot ;

public:
	Set () {
		clear () ;
	}

	explicit Set (LENGTH len) :mSet (len) {
		clear () ;
	}

	explicit Set (BUFFER &&src) :mSet (std::move (src)) {
		clear () ;
	}

	implicit Set (const std::initializer_list<KEY> &right) : Set (right.size ()) {
		for (auto &i : right)
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

	INDEX at (const Pair<Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX at (const Pair<const Set> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return size () ;
	}

	INDEX iend () const {
		return size () ;
	}

	INDEX inext (INDEX index) const {
		for (index++ ; index < size () && !mSet.used (index) ; index++)
			(void) index ;
		return index ;
	}

	BOOL equal (const Set &right) const {
		return equal (right ,mRoot ,right.mRoot) ;
	}

	inline BOOL operator== (const Set &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Set &right) const {
		return !equal (right) ;
	}

	void add (const KEY &key) {
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		insert_next (mRoot ,ix) ;
		update_insert (ix) ;
	}

	inline Set &operator<< (const KEY &key) {
		add (std::move (key)) ;
		return *this ;
	}

	void add (KEY &&key) {
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		insert_next (mRoot ,ix) ;
		update_insert (ix) ;
	}

	inline Set &operator<< (KEY &&key) {
		add (std::move (key)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (std::move (arg[i])) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (std::move (arg[i])) ;
	}

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			insert_next (mRoot ,ret) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSet.alloc (std::move (key) ,TRUE ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			insert_next (mRoot ,ret) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		if (mSet[index].mLeft != VAR_NONE && mSet[index].mRight != VAR_NONE)
			swap (index ,successor_one (index)) ;
		INDEX ix = mSet[index].mLeft != VAR_NONE ? mSet[index].mLeft : mSet[index].mRight ;
		prev_next (index) = ix ;
		if (ix != VAR_NONE)
			mSet[ix].mUp = mSet[index].mUp ;
		if (!mSet[index].mRed)
			update_remove (ix ,mSet[index].mUp) ;
		mSet.free (index) ;
	}

	INDEX min_one () const {
		INDEX ret = mRoot ;
		if (ret != VAR_NONE) {
			while (mSet[ret].mLeft != VAR_NONE)
				ret = mSet[ret].mLeft ;
		}
		return std::move (ret) ;
	}

	INDEX max_one () const {
		INDEX ret = mRoot ;
		if (ret != VAR_NONE) {
			while (mSet[ret].mRight != VAR_NONE)
				ret = mSet[ret].mRight ;
		}
		return std::move (ret) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = mRoot ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = key < mSet[ret].mKey ;
			if (!r1x && !(mSet[ret].mKey < key))
				break ;
			ret = r1x ? mSet[ret].mLeft : mSet[ret].mRight ;
		}
		return std::move (ret) ;
	}

	void erase (const KEY &key) {
		INDEX ix = find (key) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

	Array<INDEX> esort () const {
		Array<INDEX> ret = Array<INDEX> (length ()) ;
		INDEX iw = 0 ;
		esort (mRoot ,ret ,iw) ;
		return std::move (ret) ;
	}

private:
	BOOL equal (const Set &right ,INDEX it ,INDEX jt) const {
		if (it == VAR_NONE || jt == VAR_NONE)
			return it == VAR_NONE && jt == VAR_NONE ;
		if (!equal (right ,mSet[it].mLeft ,right.mSet[jt].mLeft))
			return FALSE ;
		if (mSet[it].mKey != right.mSet[jt].mKey)
			return FALSE ;
		if (!equal (right ,mSet[it].mRight ,right.mSet[jt].mRight))
			return FALSE ;
		return TRUE ;
	}

	void insert_next (INDEX &it ,INDEX jt) {
		if (it == VAR_NONE) {
			it = jt ;
			return ;
		}
		mSet[jt].mUp = it ;
		auto &r1 = mSet[jt].mKey < mSet[it].mKey ? mSet[it].mLeft : mSet[it].mRight ;
		insert_next (r1 ,jt) ;
	}

	void update_insert (INDEX it) {
		while (TRUE) {
			INDEX ix = mSet[it].mUp ;
			if (ix == VAR_NONE || !mSet[ix].mRed)
				break ;
			const auto r1x = ix == mSet[mSet[ix].mUp].mLeft ? &Set::update_insert_left : &Set::update_insert_right ;
			(this->*r1x) (it) ;
		}
		mSet[mRoot].mRed = FALSE ;
	}

	void update_insert_left (INDEX &it) {
		INDEX ix = mSet[it].mUp ;
		INDEX iy = mSet[ix].mUp ;
		if (mSet[iy].mRight != VAR_NONE && mSet[mSet[iy].mRight].mRed) {
			mSet[mSet[iy].mRight].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			it = iy ;
		} else if (it == mSet[ix].mRight) {
			rotate_left (mSet[iy].mLeft) ;
			mSet[it].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_right (prev_next (iy)) ;
			it = ix ;
		} else {
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_right (prev_next (iy)) ;
		}
	}

	void update_insert_right (INDEX &it) {
		INDEX ix = mSet[it].mUp ;
		INDEX iy = mSet[ix].mUp ;
		if (mSet[iy].mLeft != VAR_NONE && mSet[mSet[iy].mLeft].mRed) {
			mSet[mSet[iy].mLeft].mRed = FALSE ;
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			it = iy ;
		} else if (it == mSet[ix].mLeft) {
			rotate_right (mSet[iy].mRight) ;
			mSet[it].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_left (prev_next (iy)) ;
			it = ix ;
		} else {
			mSet[ix].mRed = FALSE ;
			mSet[iy].mRed = TRUE ;
			rotate_left (prev_next (iy)) ;
		}
	}

	void update_remove (INDEX it ,INDEX jt) {
		while (TRUE) {
			if (jt == VAR_NONE)
				break ;
			if (it != VAR_NONE && mSet[it].mRed)
				break ;
			const auto r1x = it == mSet[jt].mLeft ? &Set::update_remove_left : &Set::update_remove_right ;
			(this->*r1x) (it ,jt) ;
			jt = mSet[it].mUp ;
		}
		if (it != VAR_NONE)
			mSet[it].mRed = FALSE ;
	}

	void update_remove_left (INDEX &it ,INDEX jt) {
		auto &r1 = mSet[jt].mRight ;
		if (mSet[r1].mRed) {
			mSet[r1].mRed = FALSE ;
			mSet[jt].mRed = TRUE ;
			rotate_left (prev_next (jt)) ;
		}
		if ((mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed) && (mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed)) {
			mSet[r1].mRed = TRUE ;
			it = jt ;
		} else if (mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed) {
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			mSet[r1].mRed = TRUE ;
			rotate_right (r1) ;
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mRight].mRed = FALSE ;
			rotate_left (prev_next (jt)) ;
			it = mRoot ;
		} else {
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mRight].mRed = FALSE ;
			rotate_left (prev_next (jt)) ;
			it = mRoot ;
		}
	}

	void update_remove_right (INDEX &it ,INDEX jt) {
		auto &r1 = mSet[jt].mLeft ;
		if (mSet[r1].mRed) {
			mSet[r1].mRed = FALSE ;
			mSet[jt].mRed = TRUE ;
			rotate_right (prev_next (jt)) ;
		}
		if ((mSet[r1].mRight == VAR_NONE || !mSet[mSet[r1].mRight].mRed) && (mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed)) {
			mSet[r1].mRed = TRUE ;
			it = jt ;
		} else if (mSet[r1].mLeft == VAR_NONE || !mSet[mSet[r1].mLeft].mRed) {
			mSet[mSet[r1].mRight].mRed = FALSE ;
			mSet[r1].mRed = TRUE ;
			rotate_left (r1) ;
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			rotate_right (prev_next (jt)) ;
			it = mRoot ;
		} else {
			mSet[r1].mRed = mSet[jt].mRed ;
			mSet[jt].mRed = FALSE ;
			mSet[mSet[r1].mLeft].mRed = FALSE ;
			rotate_right (prev_next (jt)) ;
			it = mRoot ;
		}
	}

	void rotate_left (INDEX &it) {
		INDEX ix = mSet[it].mRight ;
		mSet[it].mRight = mSet[ix].mLeft ;
		if (mSet[ix].mLeft != VAR_NONE)
			mSet[mSet[ix].mLeft].mUp = it ;
		mSet[ix].mLeft = it ;
		mSet[ix].mUp = mSet[it].mUp ;
		mSet[it].mUp = ix ;
		it = ix ;
	}

	void rotate_right (INDEX &it) {
		INDEX ix = mSet[it].mLeft ;
		mSet[it].mLeft = mSet[ix].mRight ;
		if (mSet[ix].mRight != VAR_NONE)
			mSet[mSet[ix].mRight].mUp = it ;
		mSet[ix].mRight = it ;
		mSet[ix].mUp = mSet[it].mUp ;
		mSet[it].mUp = ix ;
		it = ix ;
	}

	INDEX &prev_next (INDEX it) {
		return mSet[it].mUp == VAR_NONE ? mRoot : mSet[mSet[it].mUp].mLeft == it ? mSet[mSet[it].mUp].mLeft : mSet[mSet[it].mUp].mRight ;
	}

	INDEX successor_one (INDEX index) const {
		for (index = mSet[index].mRight ; mSet[index].mLeft != VAR_NONE ; index = mSet[index].mLeft)
			(void) index ;
		return index ;
	}

	void swap (INDEX index1 ,INDEX index2) {
		if (index1 == index2)
			return ;
		auto &r1 = prev_next (index2) ;
		auto &r2 = mSet[index2].mLeft != VAR_NONE ? mSet[mSet[index2].mLeft].mUp : index1 ;
		auto &r3 = mSet[index2].mRight != VAR_NONE ? mSet[mSet[index2].mRight].mUp : index1 ;
		auto &r4 = prev_next (index1) ;
		auto &r5 = mSet[index1].mLeft != VAR_NONE ? mSet[mSet[index1].mLeft].mUp : index2 ;
		auto &r6 = mSet[index1].mRight != VAR_NONE ? mSet[mSet[index1].mRight].mUp : index2 ;
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

	void esort (INDEX it ,Array<INDEX> &dst ,INDEX &iw) const {
		if (it == VAR_NONE)
			return ;
		esort (mSet[it].mLeft ,dst ,iw) ;
		dst[iw++] = it ;
		esort (mSet[it].mRight ,dst ,iw) ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class HashSet {
private:
	class Node {
	private:
		friend HashSet ;
		KEY mKey ;
		ITEM mItem ;
		FLAG mHash ;
		INDEX mNext ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,FLAG hash ,INDEX next) :mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (KEY &&key ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (const KEY &key ,const ITEM &item ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mHash (hash) ,mNext (next) {}
	} ;

	template <class BASE>
	class Pair {
	private:
		friend HashSet ;

	public:
		const KEY & key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () const {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) ,item (base.mSet[index].mItem) {}
	} ;

public:
	using BUFFER = Allocator<Node ,SIZE> ;

private:
	Allocator<Node ,SIZE> mSet ;
	AutoBuffer<INDEX> mHead ;

public:
	HashSet () {
		clear () ;
	}

	explicit HashSet (LENGTH len) :mSet (len) {
		mHead = AutoBuffer<INDEX> (mSet.size ()) ;
		clear () ;
	}

	explicit HashSet (BUFFER &&src) :mSet (std::move (src)) {
		mHead = AutoBuffer<INDEX> (mSet.size ()) ;
		clear () ;
	}

	explicit HashSet (BUFFER &&src ,AutoBuffer<INDEX> &&ext) :mSet (std::move (src)) {
		mHead = std::move (ext) ;
		_DEBUG_ASSERT_ (mHead.size () == mSet.size ()) ;
		clear () ;
	}

	implicit HashSet (const std::initializer_list<PACK<KEY ,ITEM>> &right) : HashSet (right.size ()) {
		for (auto &i : right)
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

	INDEX at (const Pair<HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX at (const Pair<const HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return size () ;
	}

	INDEX iend () const {
		return size () ;
	}

	INDEX inext (INDEX index) const {
		for (index++ ; index < size () && !mSet.used (index) ; index++)
			(void) index ;
		return index ;
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

	void add (const KEY &key ,const ITEM &item) {
		if (find (key) != VAR_NONE)
			return ;
		const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
		update_insert (ix) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		if (find (key) != VAR_NONE)
			return ;
		const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
		update_insert (ix) ;
	}

	void add (const PACK<KEY ,ITEM> &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	inline HashSet &operator<< (const PACK<KEY ,ITEM> &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (KEY &&key ,const ITEM &item) {
		if (find (key) != VAR_NONE)
			return ;
		const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
		update_insert (ix) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		if (find (key) != VAR_NONE)
			return ;
		const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
		INDEX ix = mSet.alloc (std::move (key) ,std::move (item) ,r1x ,VAR_NONE) ;
		update_insert (ix) ;
	}

	void add (PACK<KEY ,ITEM> &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	inline HashSet &operator<< (PACK<KEY ,ITEM> &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		prev_next (index) = mSet[index].mNext ;
		mSet.free (index) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = VAR_NONE ;
		if (size () > 0) {
			const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
			_DEBUG_ASSERT_ (r1x >= 0) ;
			for (ret = mHead[r1x % mHead.size ()] ; ret != VAR_NONE && !(mSet[ret].mHash == r1x && mSet[ret].mKey == key) ; ret = mSet[ret].mNext)
				(void) ret ;
		}
		return std::move (ret) ;
	}

	void erase (const KEY &key) {
		INDEX ix = find (key) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

private:
	INDEX & prev_next (INDEX it) {
		INDEX ix = mSet[it].mHash % mHead.size () ;
		if (mHead[ix] == it)
			return mHead[ix] ;
		for (INDEX i = mHead[ix] ; mSet[i].mNext != VAR_NONE ; i = mSet[i].mNext)
			if (mSet[i].mNext == it)
				return mSet[i].mNext ;
		return _NULL_<INDEX> () ;
	}

	void update_insert (INDEX it) {
		if (mHead.size () != mSet.size ()) {
			mHead = AutoBuffer<INDEX> (mSet.size ()) ;
			_MEMFILL_ (mHead.self ,mHead.size () ,VAR_NONE) ;
			for (INDEX i = 0 ; i < mSet.size () ; i++) {
				if (!mSet.used (i))
					continue ;
				INDEX ix = mSet[i].mHash % mHead.size () ;
				mSet[i].mNext = mHead[ix] ;
				mHead[ix] = i ;
			}
		} else {
			INDEX ix = mSet[it].mHash % mHead.size () ;
			mSet[it].mNext = mHead[ix] ;
			mHead[ix] = it ;
		}
	}

	BOOL equal_each (const HashSet &right ,INDEX it) const {
		INDEX ix = right.find (mSet[it].mKey) ;
		return ix != VAR_NONE && mSet[it].mItem == right.mSet[ix].mItem ;
	}
} ;

template <class KEY ,class SIZE>
class HashSet<KEY ,void ,SIZE> {
private:
	class Node {
	private:
		friend HashSet ;
		KEY mKey ;
		FLAG mHash ;
		INDEX mNext ;

	public:
		inline Node () = delete ;

		inline explicit Node (const KEY &key ,FLAG hash ,INDEX next) :mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}

		inline explicit Node (KEY &&key ,FLAG hash ,INDEX next) : mKey (std::move (key)) ,mHash (hash) ,mNext (next) {}
	} ;

	template <class BASE>
	class Pair {
	private:
		friend HashSet ;

	public:
		const KEY & key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () const {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSet[index].mKey) {}
	} ;

public:
	using BUFFER = Allocator<Node ,SIZE> ;

private:
	Allocator<Node ,SIZE> mSet ;
	AutoBuffer<INDEX> mHead ;

public:
	HashSet () {
		clear () ;
	}

	explicit HashSet (LENGTH len) :mSet (len) {
		mHead = AutoBuffer<INDEX> (mSet.size ()) ;
		clear () ;
	}

	explicit HashSet (BUFFER &&src) :mSet (std::move (src)) {
		mHead = AutoBuffer<INDEX> (mSet.size ()) ;
		clear () ;
	}

	explicit HashSet (BUFFER &&src ,AutoBuffer<INDEX> &&ext) :mSet (std::move (src)) {
		mHead = std::move (ext) ;
		_DEBUG_ASSERT_ (mHead.size () == mSet.size ()) ;
		clear () ;
	}

	implicit HashSet (const std::initializer_list<KEY> &right) : HashSet (right.size ()) {
		for (auto &i : right)
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

	INDEX at (const Pair<HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX at (const Pair<const HashSet> &item) const {
		return mSet.at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX ibegin () const {
		for (INDEX i = 0 ; i < size () ; i++)
			if (mSet.used (i))
				return i ;
		return size () ;
	}

	INDEX iend () const {
		return size () ;
	}

	INDEX inext (INDEX index) const {
		for (index++ ; index < size () && !mSet.used (index) ; index++)
			(void) index ;
		return index ;
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

	void add (const KEY &key) {
		if (find (key) != VAR_NONE)
			return ;
		const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
		INDEX ix = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
		update_insert (ix) ;
	}

	inline HashSet &operator<< (const KEY &key) {
		add (std::move (key)) ;
		return *this ;
	}

	void add (KEY &&key) {
		if (find (key) != VAR_NONE)
			return ;
		const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
		INDEX ix = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
		update_insert (ix) ;
	}

	inline HashSet &operator<< (KEY &&key) {
		add (std::move (key)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		mSet.reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	INDEX insert (const KEY &key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
			ret = mSet.alloc (std::move (key) ,r1x ,VAR_NONE) ;
			update_insert (ret) ;
		}
		return std::move (ret) ;
	}

	void remove (INDEX index) {
		prev_next (index) = mSet[index].mNext ;
		mSet.free (index) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = VAR_NONE ;
		if (size () > 0) {
			const auto r1x = OPERATOR_HASH_TYPE::invoke (key) ;
			_DEBUG_ASSERT_ (r1x >= 0) ;
			for (ret = mHead[r1x % mHead.size ()] ; ret != VAR_NONE && !(mSet[ret].mHash == r1x && mSet[ret].mKey == key) ; ret = mSet[ret].mNext)
				(void) ret ;
		}
		return std::move (ret) ;
	}

	void erase (const KEY &key) {
		INDEX ix = find (key) ;
		if (ix != VAR_NONE)
			remove (ix) ;
	}

private:
	INDEX & prev_next (INDEX it) {
		INDEX ix = mSet[it].mHash % mHead.size () ;
		if (mHead[ix] == it)
			return mHead[ix] ;
		for (INDEX i = mHead[ix] ; mSet[i].mNext != VAR_NONE ; i = mSet[i].mNext)
			if (mSet[i].mNext == it)
				return mSet[i].mNext ;
		return _NULL_<INDEX> () ;
	}

	void update_insert (INDEX it) {
		if (mHead.size () != mSet.size ()) {
			mHead = AutoBuffer<INDEX> (mSet.size ()) ;
			_MEMFILL_ (mHead.self ,mHead.size () ,VAR_NONE) ;
			for (INDEX i = 0 ; i < mSet.size () ; i++) {
				if (!mSet.used (i))
					continue ;
				INDEX ix = mSet[i].mHash % mHead.size () ;
				mSet[i].mNext = mHead[ix] ;
				mHead[ix] = i ;
			}
		} else {
			INDEX ix = mSet[it].mHash % mHead.size () ;
			mSet[it].mNext = mHead[ix] ;
			mHead[ix] = it ;
		}
	}

	BOOL equal_each (const HashSet &right ,INDEX it) const {
		INDEX ix = right.find (mSet[it].mKey) ;
		return ix != VAR_NONE && mSet[it].mItem == right.mSet[ix].mItem ;
	}
} ;

template <class KEY ,class ITEM = void ,class SIZE = SAUTO>
class SoftSet {
private:
	class Node {
	private:
		friend SoftSet ;
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

		inline explicit Node (const KEY &key ,const ITEM &item ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}

		inline explicit Node (KEY &&key ,ITEM &&item ,LENGTH weight ,INDEX left ,INDEX right ,INDEX next) : mKey (std::move (key)) ,mItem (std::move (item)) ,mWeight (weight) ,mLeft (left) ,mRight (right) ,mNext (next) {}
	} ;

	class Pack {
	private:
		friend SoftSet ;
		SharedRef<Allocator<Node ,SIZE>> mHeap ;
		LENGTH mLength ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mRoot ;
	} ;

	template <class BASE>
	class Pair {
	private:
		friend SoftSet ;

	public:
		const KEY & key ;
		CAST_TRAITS_TYPE<ITEM ,BASE> &item ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () const {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSoftSet->mHeap.self[index].mKey) ,item (base.mSoftSet->mHeap.self[index].mItem) {}
	} ;

private:
	SharedRef<Pack> mSoftSet ;

public:
	SoftSet () = default ;

	explicit SoftSet (LENGTH len) {
		mSoftSet = SharedRef<Pack>::make () ;
		mSoftSet->mHeap = SharedRef<Allocator<Node ,SIZE>>::make (len) ;
		mSoftSet->mLength = 0 ;
		mSoftSet->mFirst = VAR_NONE ;
		mSoftSet->mLast = VAR_NONE ;
		mSoftSet->mRoot = VAR_NONE ;
	}

	LENGTH size () const {
		return mSoftSet.exist () ? mSoftSet->mHeap->size () : 0 ;
	}

	LENGTH length () const {
		return mSoftSet.exist () ? mSoftSet->mLength : 0 ;
	}

	//@info:'clear' does not remove any item from soft heap in fact
	void clear () {
		const auto r1x = mSoftSet->mHeap ;
		mSoftSet = SharedRef<Pack>::make () ;
		mSoftSet->mHeap = r1x ;
		mSoftSet->mLength = 0 ;
		mSoftSet->mFirst = VAR_NONE ;
		mSoftSet->mLast = VAR_NONE ;
		mSoftSet->mRoot = VAR_NONE ;
	}

	Pair<SoftSet> get (INDEX index) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		return Pair<SoftSet> (*this ,index) ;
	}

	inline Pair<SoftSet> operator[] (INDEX index) {
		return get (index) ;
	}

	Pair<const SoftSet> get (INDEX index) const {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		return Pair<const SoftSet> (*this ,index) ;
	}

	inline Pair<const SoftSet> operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const Pair<SoftSet> &item) const {
		return mSoftSet->mHeap->at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX at (const Pair<const SoftSet> &item) const {
		return mSoftSet->mHeap->at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX ibegin () const {
		return mSoftSet.exist () ? mSoftSet->mFirst : VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return mSoftSet->mHeap.self[index].mNext ;
	}

	BOOL equal (const SoftSet &right) const {
		INDEX ix = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		INDEX iy = right.mSoftSet.exist () ? right.mSoftSet->mRoot : VAR_NONE ;
		return equal (right ,ix ,iy) ;
	}

	inline BOOL operator== (const SoftSet &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftSet &right) const {
		return !equal (right) ;
	}

	void add (const KEY &key ,const ITEM &item) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSoftSet->mHeap->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ix ;
		mSoftSet->mLast = ix ;
		mSoftSet->mLength++ ;
		update_insert (mSoftSet->mRoot) ;
	}

	void add (const KEY &key ,ITEM &&item) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSoftSet->mHeap->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ix ;
		mSoftSet->mLast = ix ;
		mSoftSet->mLength++ ;
		update_insert (mSoftSet->mRoot) ;
	}

	void add (const PACK<KEY ,ITEM> &item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	inline SoftSet &operator<< (const PACK<KEY ,ITEM> &item) {
		add (std::move (item)) ;
		return *this ;
	}

	void add (KEY &&key ,const ITEM &item) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSoftSet->mHeap->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ix ;
		mSoftSet->mLast = ix ;
		mSoftSet->mLength++ ;
		update_insert (mSoftSet->mRoot) ;
	}

	void add (KEY &&key ,ITEM &&item) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSoftSet->mHeap->alloc (std::move (key) ,std::move (item) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ix ;
		mSoftSet->mLast = ix ;
		mSoftSet->mLength++ ;
		update_insert (mSoftSet->mRoot) ;
	}

	void add (PACK<KEY ,ITEM> &&item) {
		add (std::move (item.P1) ,std::move (item.P2)) ;
	}

	inline SoftSet &operator<< (PACK<KEY ,ITEM> &&item) {
		add (std::move (item)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		mSoftSet->mHeap->reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		mSoftSet->mHeap->reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i] ,std::move (arg[i].item)) ;
	}

	INDEX insert (const KEY &key) popping {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSoftSet->mHeap->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ret ;
			mSoftSet->mLast = ret ;
			mSoftSet->mLength++ ;
			update_insert (mSoftSet->mRoot) ;
		}
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSoftSet->mHeap->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ret ;
			mSoftSet->mLast = ret ;
			mSoftSet->mLength++ ;
			update_insert (mSoftSet->mRoot) ;
		}
		return std::move (ret) ;
	}

	INDEX min_one () const {
		INDEX ret = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		if (ret != VAR_NONE) {
			auto &r1 = mSoftSet->mHeap.self ;
			while (r1[ret].mLeft != VAR_NONE)
				ret = r1[ret].mLeft ;
		}
		return std::move (ret) ;
	}

	INDEX max_one () const {
		INDEX ret = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		if (ret != VAR_NONE) {
			auto &r1 = mSoftSet->mHeap.self ;
			while (r1[ret].mRight != VAR_NONE)
				ret = r1[ret].mRight ;
		}
		return std::move (ret) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		auto &r1 = mSoftSet->mHeap.self ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = key < r1[ret].mKey ;
			if (!r1x && !(r1[ret].mKey < key))
				break ;
			ret = r1x ? r1[ret].mLeft : r1[ret].mRight ;
		}
		return std::move (ret) ;
	}

	void shrink () {
		if (mSoftSet.exist ())
			mSoftSet->mHeap->shrink () ;
	}

private:
	BOOL equal (const SoftSet &right ,INDEX it ,INDEX jt) const {
		if (it == VAR_NONE || jt == VAR_NONE)
			return it == VAR_NONE && jt == VAR_NONE ;
		auto &r1 = mSoftSet->mHeap.self[it] ;
		auto &r2 = right.mSoftSet->mHeap.self[jt] ;
		if (!equal (right ,r1.mLeft ,r2.mLeft))
			return FALSE ;
		if (r1.mKey != r2.mKey)
			return FALSE ;
		if (!equal (right ,r1.mRight ,r2.mRight))
			return FALSE ;
		return TRUE ;
	}

	void update_insert (INDEX &it) {
		if (it == VAR_NONE) {
			it = mSoftSet->mLast ;
		} else {
			auto &r1 = mSoftSet->mHeap.self ;
			r1[it].mWeight++ ;
			const auto r1x = r1[mSoftSet->mLast].mKey < r1[it].mKey ;
			auto &r2 = r1x ? r1[it].mLeft : r1[it].mRight ;
			update_insert (r2) ;
			const auto r2x = r1x ? &SoftSet::update_insert_left : &SoftSet::update_insert_right ;
			(this->*r2x) (it) ;
		}
	}

	void update_insert_left (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		const auto r1x = r1[it].mRight != VAR_NONE ? r1[r1[it].mRight].mWeight : 0 ;
		const auto r2x = r1[it].mLeft != VAR_NONE && r1[r1[it].mLeft].mLeft != VAR_NONE ? r1[r1[r1[it].mLeft].mLeft].mWeight : 0 ;
		const auto r3x = r1[it].mLeft != VAR_NONE && r1[r1[it].mLeft].mRight != VAR_NONE ? r1[r1[r1[it].mLeft].mRight].mWeight : 0 ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if (r1x >= r2x)
			rotate_left (r1[it].mLeft) ;
		rotate_right (it) ;
		update_insert_left (r1[it].mLeft) ;
		update_insert_right (r1[it].mRight) ;
		update_insert_left (it) ;
		update_insert_right (it) ;
	}

	void update_insert_right (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		const auto r1x = r1[it].mLeft != VAR_NONE ? r1[r1[it].mLeft].mWeight : 0 ;
		const auto r2x = r1[it].mRight != VAR_NONE && r1[r1[it].mRight].mRight != VAR_NONE ? r1[r1[r1[it].mRight].mRight].mWeight : 0 ;
		const auto r3x = r1[it].mRight != VAR_NONE && r1[r1[it].mRight].mLeft != VAR_NONE ? r1[r1[r1[it].mRight].mLeft].mWeight : 0 ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if (r1x >= r2x)
			rotate_right (r1[it].mRight) ;
		rotate_left (it) ;
		update_insert_left (r1[it].mLeft) ;
		update_insert_right (r1[it].mRight) ;
		update_insert_left (it) ;
		update_insert_right (it) ;
	}

	void rotate_left (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		INDEX ix = r1[it].mRight ;
		r1[it].mRight = r1[ix].mLeft ;
		r1[ix].mLeft = it ;
		r1[ix].mWeight = r1[it].mWeight ;
		r1[it].mWeight = (r1[it].mLeft != VAR_NONE ? r1[r1[it].mLeft].mWeight : 0) + (r1[it].mRight != VAR_NONE ? r1[r1[it].mRight].mWeight : 0) + 1 ;
		it = ix ;
	}

	void rotate_right (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		INDEX ix = r1[it].mLeft ;
		r1[it].mLeft = r1[ix].mRight ;
		r1[ix].mRight = it ;
		r1[ix].mWeight = r1[it].mWeight ;
		r1[it].mWeight = (r1[it].mLeft != VAR_NONE ? r1[r1[it].mLeft].mWeight : 0) + (r1[it].mRight != VAR_NONE ? r1[r1[it].mRight].mWeight : 0) + 1 ;
		it = ix ;
	}
} ;

template <class KEY ,class SIZE>
class SoftSet<KEY ,void ,SIZE> {
private:
	class Node {
	private:
		friend SoftSet ;
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

	class Pack {
	private:
		friend SoftSet ;
		SharedRef<Allocator<Node ,SIZE>> mHeap ;
		LENGTH mLength ;
		INDEX mFirst ;
		INDEX mLast ;
		INDEX mRoot ;
	} ;

	template <class BASE>
	class Pair {
	private:
		friend SoftSet ;

	public:
		const KEY & key ;

	public:
		inline Pair () = delete ;

		inline Pair (const Pair &) = delete ;

		inline Pair (Pair &&) noexcept = default ;

		inline implicit operator const KEY & () const {
			return key ;
		}

	private:
		inline explicit Pair (BASE &base ,INDEX index) :key (base.mSoftSet->mHeap.self[index].mKey) {}
	} ;

private:
	SharedRef<Pack> mSoftSet ;

public:
	SoftSet () = default ;

	explicit SoftSet (LENGTH len) {
		mSoftSet = SharedRef<Pack>::make () ;
		mSoftSet->mHeap = SharedRef<Allocator<Node ,SIZE>>::make (len) ;
		mSoftSet->mLength = 0 ;
		mSoftSet->mFirst = VAR_NONE ;
		mSoftSet->mLast = VAR_NONE ;
		mSoftSet->mRoot = VAR_NONE ;
	}

	LENGTH size () const {
		return mSoftSet.exist () ? mSoftSet->mHeap->size () : 0 ;
	}

	LENGTH length () const {
		return mSoftSet.exist () ? mSoftSet->mLength : 0 ;
	}

	//@info:'clear' does not remove any item from soft heap in fact
	void clear () {
		const auto r1x = mSoftSet->mHeap ;
		mSoftSet = SharedRef<Pack>::make () ;
		mSoftSet->mHeap = r1x ;
		mSoftSet->mLength = 0 ;
		mSoftSet->mFirst = VAR_NONE ;
		mSoftSet->mLast = VAR_NONE ;
		mSoftSet->mRoot = VAR_NONE ;
	}

	Pair<SoftSet> get (INDEX index) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		return Pair<SoftSet> (*this ,index) ;
	}

	inline Pair<SoftSet> operator[] (INDEX index) {
		return get (index) ;
	}

	Pair<const SoftSet> get (INDEX index) const {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		return Pair<const SoftSet> (*this ,index) ;
	}

	inline Pair<const SoftSet> operator[] (INDEX index) const {
		return get (index) ;
	}

	INDEX at (const Pair<SoftSet> &item) const {
		return mSoftSet->mHeap->at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX at (const Pair<const SoftSet> &item) const {
		return mSoftSet->mHeap->at (_OFFSET_ (&Node::mKey ,&item.key)) ;
	}

	INDEX ibegin () const {
		return mSoftSet.exist () ? mSoftSet->mFirst : VAR_NONE ;
	}

	INDEX iend () const {
		return VAR_NONE ;
	}

	INDEX inext (INDEX index) const {
		return mSoftSet->mHeap.self[index].mNext ;
	}

	BOOL equal (const SoftSet &right) const {
		INDEX ix = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		INDEX iy = right.mSoftSet.exist () ? right.mSoftSet->mRoot : VAR_NONE ;
		return equal (right ,ix ,iy) ;
	}

	inline BOOL operator== (const SoftSet &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftSet &right) const {
		return !equal (right) ;
	}

	void add (const KEY &key) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSoftSet->mHeap->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ix ;
		mSoftSet->mLast = ix ;
		mSoftSet->mLength++ ;
		update_insert (mSoftSet->mRoot) ;
	}

	inline SoftSet &operator<< (const KEY &key) {
		add (std::move (key)) ;
		return *this ;
	}

	void add (KEY &&key) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		if (find (key) != VAR_NONE)
			return ;
		INDEX ix = mSoftSet->mHeap->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
		(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ix ;
		mSoftSet->mLast = ix ;
		mSoftSet->mLength++ ;
		update_insert (mSoftSet->mRoot) ;
	}

	inline SoftSet &operator<< (KEY &&key) {
		add (std::move (key)) ;
		return *this ;
	}

	template <class _ARG>
	void appand (const _ARG &arg) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		mSoftSet->mHeap->reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i]) ;
	}

	template <class _ARG>
	void appand (_ARG &&arg) {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		mSoftSet->mHeap->reserve (arg.length ()) ;
		for (INDEX i = arg.ibegin () ,ie = arg.iend () ; i != ie ; i = arg.inext (i))
			add (arg[i]) ;
	}

	INDEX insert (const KEY &key) popping {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSoftSet->mHeap->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ret ;
			mSoftSet->mLast = ret ;
			mSoftSet->mLength++ ;
			update_insert (mSoftSet->mRoot) ;
		}
		return std::move (ret) ;
	}

	INDEX insert (KEY &&key) popping {
		_DEBUG_ASSERT_ (mSoftSet.exist ()) ;
		INDEX ret = find (key) ;
		if (ret == VAR_NONE) {
			ret = mSoftSet->mHeap->alloc (std::move (key) ,1 ,VAR_NONE ,VAR_NONE ,VAR_NONE) ;
			(mSoftSet->mLast != VAR_NONE ? mSoftSet->mHeap.self[mSoftSet->mLast].mNext : mSoftSet->mFirst) = ret ;
			mSoftSet->mLast = ret ;
			mSoftSet->mLength++ ;
			update_insert (mSoftSet->mRoot) ;
		}
		return std::move (ret) ;
	}

	INDEX min_one () const {
		INDEX ret = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		if (ret != VAR_NONE) {
			auto &r1 = mSoftSet->mHeap.self ;
			while (r1[ret].mLeft != VAR_NONE)
				ret = r1[ret].mLeft ;
		}
		return std::move (ret) ;
	}

	INDEX max_one () const {
		INDEX ret = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		if (ret != VAR_NONE) {
			auto &r1 = mSoftSet->mHeap.self ;
			while (r1[ret].mRight != VAR_NONE)
				ret = r1[ret].mRight ;
		}
		return std::move (ret) ;
	}

	INDEX find (const KEY &key) const {
		INDEX ret = mSoftSet.exist () ? mSoftSet->mRoot : VAR_NONE ;
		auto &r1 = mSoftSet->mHeap.self ;
		while (TRUE) {
			if (ret == VAR_NONE)
				break ;
			const auto r1x = key < r1[ret].mKey ;
			if (!r1x && !(r1[ret].mKey < key))
				break ;
			ret = r1x ? r1[ret].mLeft : r1[ret].mRight ;
		}
		return std::move (ret) ;
	}

	void shrink () {
		if (mSoftSet.exist ())
			mSoftSet->mHeap->shrink () ;
	}

private:
	BOOL equal (const SoftSet &right ,INDEX it ,INDEX jt) const {
		if (it == VAR_NONE || jt == VAR_NONE)
			return it == VAR_NONE && jt == VAR_NONE ;
		auto &r1 = mSoftSet->mHeap.self[it] ;
		auto &r2 = right.mSoftSet->mHeap.self[jt] ;
		if (!equal (right ,r1.mLeft ,r2.mLeft))
			return FALSE ;
		if (r1.mKey != r2.mKey)
			return FALSE ;
		if (!equal (right ,r1.mRight ,r2.mRight))
			return FALSE ;
		return TRUE ;
	}

	void update_insert (INDEX &it) {
		if (it == VAR_NONE) {
			it = mSoftSet->mLast ;
		} else {
			auto &r1 = mSoftSet->mHeap.self ;
			r1[it].mWeight++ ;
			const auto r1x = r1[mSoftSet->mLast].mKey < r1[it].mKey ;
			auto &r2 = r1x ? r1[it].mLeft : r1[it].mRight ;
			update_insert (r2) ;
			const auto r2x = r1x ? &SoftSet::update_insert_left : &SoftSet::update_insert_right ;
			(this->*r2x) (it) ;
		}
	}

	void update_insert_left (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		const auto r1x = r1[it].mRight != VAR_NONE ? r1[r1[it].mRight].mWeight : 0 ;
		const auto r2x = r1[it].mLeft != VAR_NONE && r1[r1[it].mLeft].mLeft != VAR_NONE ? r1[r1[r1[it].mLeft].mLeft].mWeight : 0 ;
		const auto r3x = r1[it].mLeft != VAR_NONE && r1[r1[it].mLeft].mRight != VAR_NONE ? r1[r1[r1[it].mLeft].mRight].mWeight : 0 ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if (r1x >= r2x)
			rotate_left (r1[it].mLeft) ;
		rotate_right (it) ;
		update_insert_left (r1[it].mLeft) ;
		update_insert_right (r1[it].mRight) ;
		update_insert_left (it) ;
		update_insert_right (it) ;
	}

	void update_insert_right (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		const auto r1x = r1[it].mLeft != VAR_NONE ? r1[r1[it].mLeft].mWeight : 0 ;
		const auto r2x = r1[it].mRight != VAR_NONE && r1[r1[it].mRight].mRight != VAR_NONE ? r1[r1[r1[it].mRight].mRight].mWeight : 0 ;
		const auto r3x = r1[it].mRight != VAR_NONE && r1[r1[it].mRight].mLeft != VAR_NONE ? r1[r1[r1[it].mRight].mLeft].mWeight : 0 ;
		if (r1x >= r2x && r1x >= r3x)
			return ;
		if (r1x >= r2x)
			rotate_right (r1[it].mRight) ;
		rotate_left (it) ;
		update_insert_left (r1[it].mLeft) ;
		update_insert_right (r1[it].mRight) ;
		update_insert_left (it) ;
		update_insert_right (it) ;
	}

	void rotate_left (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		INDEX ix = r1[it].mRight ;
		r1[it].mRight = r1[ix].mLeft ;
		r1[ix].mLeft = it ;
		r1[ix].mWeight = r1[it].mWeight ;
		r1[it].mWeight = (r1[it].mLeft != VAR_NONE ? r1[r1[it].mLeft].mWeight : 0) + (r1[it].mRight != VAR_NONE ? r1[r1[it].mRight].mWeight : 0) + 1 ;
		it = ix ;
	}

	void rotate_right (INDEX &it) {
		auto &r1 = mSoftSet->mHeap.self ;
		INDEX ix = r1[it].mLeft ;
		r1[it].mLeft = r1[ix].mRight ;
		r1[ix].mRight = it ;
		r1[ix].mWeight = r1[it].mWeight ;
		r1[it].mWeight = (r1[it].mLeft != VAR_NONE ? r1[r1[it].mLeft].mWeight : 0) + (r1[it].mRight != VAR_NONE ? r1[r1[it].mRight].mWeight : 0) + 1 ;
		it = ix ;
	}
} ;
} ;