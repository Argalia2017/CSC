#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
namespace ARRAY {
template <class...>
trait ARRAY_ITERATOR_HELP ;

template <class UNIT1 ,class UNIT2>
trait ARRAY_ITERATOR_HELP<UNIT1 ,UNIT2 ,ALWAYS> {
	class Iterator {
	private:
		CRef<UNIT1> mBase ;
		INDEX mBegin ;
		INDEX mEnd ;
		INDEX mCurr ;

	public:
		implicit Iterator () = delete ;

		explicit Iterator (RREF<CRef<UNIT1>> base) {
			mBase = move (base) ;
			mBegin = mBase->ibegin () ;
			mEnd = mBase->iend () ;
			mCurr = mBegin ;
		}

		Iterator begin () const {
			return thiz ;
		}

		Iterator end () const {
			return thiz ;
		}

		BOOL good () const {
			return mCurr != mEnd ;
		}

		inline BOOL operator== (CREF<Iterator>) const {
			return ifnot (good ()) ;
		}

		inline BOOL operator!= (CREF<Iterator>) const {
			return good () ;
		}

		CREF<UNIT2> at () const leftvalue {
			return mBase->at (mCurr) ;
		}

		inline CREF<UNIT2> operator* () const leftvalue {
			return at () ;
		}

		void next () {
			mCurr = mBase->inext (mCurr) ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

template <class UNTI1 ,class UNIT2>
using ArrayIterator = typename ARRAY_ITERATOR_HELP<UNTI1 ,UNIT2 ,ALWAYS>::Iterator ;

template <class...>
trait ARRAYRANGE_HELP ;

template <class UNIT1>
trait ARRAYRANGE_HELP<UNIT1 ,ALWAYS> {
	class ArrayRange extend Proxy {
	private:
		UNIT1 mBase ;

	public:
		imports CREF<ArrayRange> from (CREF<UNIT1> that) {
			return unsafe_deref (unsafe_cast (TYPEAS<TEMP<ArrayRange>>::id ,unsafe_deptr (that))) ;
		}

		INDEX ibegin () const {
			return mBase.ibegin () ;
		}

		INDEX iend () const {
			return mBase.iend () ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return mBase.inext (index) ;
		}

		CREF<INDEX> at (CREF<INDEX> index) const leftvalue {
			return index ;
		}
	} ;
} ;

template <class UNTI1>
using ArrayRange = typename ARRAYRANGE_HELP<UNTI1 ,ALWAYS>::ArrayRange ;

template <class...>
trait ARRAY_HELP ;

template <class ITEM ,class SIZE>
trait ARRAY_HELP<ITEM ,SIZE ,ALWAYS> {
	class Array {
	private:
		Buffer<ITEM ,SIZE> mArray ;

	public:
		implicit Array () {
			mArray = Buffer<ITEM ,SIZE> (0) ;
		}

		explicit Array (CREF<LENGTH> size_) {
			mArray = Buffer<ITEM ,SIZE> (size_) ;
		}

		explicit Array (CREF<csc_initializer_list<ITEM>> that) {
			mArray = Buffer<ITEM ,SIZE> (LENGTH (that.size ())) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mArray[ix] = move (i) ;
				ix++ ;
			}
		}

		imports Array make (CREF<LENGTH> begin_ ,CREF<LENGTH> end_) {
			const auto r1x = vmax (end_ - begin_ ,ZERO) ;
			Array ret = Array (r1x) ;
			INDEX ix = 0 ;
			for (auto &&i : CORE::iter (begin_ ,end_)) {
				ret[ix] = ITEM (i) ;
				ix++ ;
			}
			return move (ret) ;
		}

		LENGTH size () const {
			return mArray.size () ;
		}

		LENGTH length () const {
			return size () ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<VarBuffer<ITEM>> where_ = VarBuffer<ITEM> ()) leftvalue {
			auto rax = VRef<ARR<ITEM>>::reference (mArray.self) ;
			return RegBuffer<ITEM>::from (where_ ,move (rax) ,mArray.size ()) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<ConBuffer<ITEM>> where_ = ConBuffer<ITEM> ()) const leftvalue {
			auto rax = CRef<ARR<ITEM>>::reference (mArray.self) ;
			return RegBuffer<ITEM>::from (where_ ,move (rax) ,mArray.size ()) ;
		}

		void fill (CREF<ITEM> obj) {
			for (auto &&i : CORE::iter (0 ,size ()))
				mArray[i] = obj ;
		}

		ArrayIterator<Array ,ITEM> begin () const leftvalue {
			return ArrayIterator<Array ,ITEM> (CRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<Array ,ITEM> end () const leftvalue {
			return ArrayIterator<Array ,ITEM> (CRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<Array> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<Array>::from (thiz) ;
			return ArrayIterator<ArrayRange<Array> ,INDEX> (CRef<ArrayRange<Array>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			return ZERO ;
		}

		INDEX iend () const {
			return length () ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			return mArray[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mArray[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		BOOL equal (CREF<Array> that) const {
			return operator_equal (mArray ,that.mArray) ;
		}

		inline BOOL operator== (CREF<Array> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Array> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Array> that) const {
			return operator_compr (mArray ,that.mArray) ;
		}

		inline BOOL operator< (CREF<Array> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<Array> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<Array> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<Array> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			return operator_hash (mArray) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Array = typename ARRAY_HELP<ITEM ,SIZE ,ALWAYS>::Array ;

template <class ITEM>
using ARRAY1 = Array<ITEM ,RANK1> ;
template <class ITEM>
using ARRAY2 = Array<ITEM ,RANK2> ;
template <class ITEM>
using ARRAY3 = Array<ITEM ,RANK3> ;
template <class ITEM>
using ARRAY4 = Array<ITEM ,RANK4> ;
template <class ITEM>
using ARRAY5 = Array<ITEM ,RANK5> ;
template <class ITEM>
using ARRAY6 = Array<ITEM ,RANK6> ;
template <class ITEM>
using ARRAY7 = Array<ITEM ,RANK7> ;
template <class ITEM>
using ARRAY8 = Array<ITEM ,RANK8> ;
template <class ITEM>
using ARRAY9 = Array<ITEM ,RANK9> ;

template <class...>
trait STRING_HELP ;

template <class ITEM ,class SIZE>
trait STRING_HELP<ITEM ,SIZE ,ALWAYS> {
	using RESERVE_SIZE = ENUM_ADD<SIZE ,CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_IDEN ,ENUM_ZERO>> ;

	using STRING_BUFFER_SIZE = ENUMAS<VAL ,ENUMID<8192>> ;

	class String {
	private:
		Buffer<ITEM ,RESERVE_SIZE> mString ;

	public:
		implicit String () {
			mString = Buffer<ITEM ,RESERVE_SIZE> (0) ;
			clear () ;
		}

		template <class ARG1 = void ,class = ENABLE<IS_STR<DEPENDENT<ITEM ,ARG1>>>>
		implicit String (CREF<Slice<DEPENDENT<ITEM ,ARG1>>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (that.size ())) ;
			INDEX ix = 0 ;
			for (auto &&i : CORE::iter (0 ,that.size ())) {
				mString[ix] = that[i] ;
				ix++ ;
			}
			while (TRUE) {
				if (ix >= mString.size ())
					break ;
				mString[ix] = ITEM (0) ;
				ix++ ;
			}
		}

		explicit String (CREF<LENGTH> size_) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (size_)) ;
			clear () ;
		}

		explicit String (CREF<csc_initializer_list<ITEM>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (LENGTH (that.size ()))) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mString[ix] = move (i) ;
				ix++ ;
			}
			while (TRUE) {
				if (ix >= mString.size ())
					break ;
				mString[ix] = ITEM (0) ;
				ix++ ;
			}
		}

		template <class...ARG1>
		imports String make (XREF<ARG1>...objs) {
			String ret = String (STRING_BUFFER_SIZE::value) ;
			unimplemented () ;
			return move (ret) ;
		}

		LENGTH size () const {
			if (mString.size () == 0)
				return ZERO ;
			return mString.size () - 1 ;
		}

		LENGTH length () const {
			if (mString.size () == 0)
				return ZERO ;
			for (auto &&i : CORE::iter (0 ,size ())) {
				if (mString[i] == ITEM (0))
					return i ;
			}
			return size () ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<VarBuffer<ITEM>> where_ = VarBuffer<ITEM> ()) leftvalue {
			auto rax = VRef<ARR<ITEM>>::reference (mString.self) ;
			return RegBuffer<ITEM>::from (where_ ,move (rax) ,mString.size ()) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<ConBuffer<ITEM>> where_ = ConBuffer<ITEM> ()) const leftvalue {
			auto rax = CRef<ARR<ITEM>>::reference (mString.self) ;
			return RegBuffer<ITEM>::from (where_ ,move (rax) ,mString.size ()) ;
		}

		void clear () {
			for (auto &&i : CORE::iter (0 ,mString.size ()))
				mString[i] = ITEM (0) ;
		}

		void fill (CREF<ITEM> obj) {
			for (auto &&i : CORE::iter (0 ,size ()))
				mString[i] = obj ;
		}

		ArrayIterator<String ,ITEM> begin () const leftvalue {
			return ArrayIterator<String ,ITEM> (CRef<String>::reference (thiz)) ;
		}

		ArrayIterator<String ,ITEM> end () const leftvalue {
			return ArrayIterator<String ,ITEM> (CRef<String>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<String> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<String>::from (thiz) ;
			return ArrayIterator<ArrayRange<String> ,INDEX> (CRef<ArrayRange<String>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			return ZERO ;
		}

		INDEX iend () const {
			return length () ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (ensure_index (index)) ;
			return mString[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (ensure_index (index)) ;
			return mString[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		BOOL empty () const {
			if (mString.size () == 0)
				return TRUE ;
			if ifnot (operator_equal (mString[0] ,ITEM (0)))
				return FALSE ;
			return TRUE ;
		}

		BOOL equal (CREF<String> that) const {
			const auto r1x = size () ;
			const auto r2x = that.size () ;
			if (r1x != r2x)
				return FALSE ;
			if (r1x == 0)
				return TRUE ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (operator_equal (mString[ix] ,ITEM (0)))
					break ;
				if ifnot (operator_equal (mString[ix] ,that.mString[ix]))
					break ;
				ix++ ;
			}
			return operator_equal (mString[ix] ,that.mString[ix]) ;
		}

		inline BOOL operator== (CREF<String> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<String> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<String> that) const {
			const auto r1x = size () ;
			const auto r2x = that.size () ;
			if (r1x == 0)
				return operator_compr (r1x ,r2x) ;
			if (r2x == 0)
				return operator_compr (r1x ,r2x) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (operator_equal (mString[ix] ,ITEM (0)))
					break ;
				if ifnot (operator_equal (mString[ix] ,that.mString[ix]))
					break ;
				ix++ ;
			}
			return operator_compr (mString[ix] ,that.mString[ix]) ;
		}

		inline BOOL operator< (CREF<String> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<String> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<String> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<String> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			FLAG ret = hashcode () ;
			for (auto &&i : CORE::iter (0 ,size ())) {
				const auto r1x = operator_hash (mString[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		String concat (CREF<String> that) const {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			String ret = String (r1x + r2x) ;
			for (auto &&i : CORE::iter (0 ,r1x))
				ret.mString[i] = mString[i] ;
			for (auto &&i : CORE::iter (0 ,r2x))
				ret.mString[r1x + i] = that.mString[i] ;
			return move (ret) ;
		}

		inline String operator+ (CREF<String> that) const {
			return concat (that) ;
		}

	private:
		LENGTH reserve_size (CREF<LENGTH> size_) const {
			return size_ + LENGTH (size_ > 0) ;
		}

		INDEX ensure_index (CREF<INDEX> index) const {
			return vbetween (index ,0 ,size ()) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using String = typename STRING_HELP<ITEM ,SIZE ,ALWAYS>::String ;

template <class...>
trait SORTPROC_HELP ;

template <class UNIT1>
trait SORTPROC_HELP<UNIT1 ,ALWAYS> {
	class SortProc {
	public:
		imports CREF<SortProc> instance () {
			return memorize ([&] () {
				return SortProc () ;
			}) ;
		}

		imports void sort (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			const auto r1x = end_ - begin_ ;
			if (r1x <= 1)
				return ;
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			assert (vbetween (ix ,0 ,range_.size ())) ;
			assert (vbetween (iy ,0 ,range_.size ())) ;
			instance ().quick_sort (array_ ,range_ ,ix ,iy ,r1x) ;
		}

	private:
		void insert_sort (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb) const {
			for (auto &&i : iter (lb ,rb)) {
				INDEX ix = i + 1 ;
				INDEX iy = i ;
				const auto r1x = range_[ix] ;
				while (TRUE) {
					if (iy < lb)
						break ;
					const auto r2x = operator_compr (array_[r1x] ,array_[range_[iy]]) ;
					if (r2x >= ZERO)
						break ;
					range_[ix] = range_[iy] ;
					ix = iy ;
					iy-- ;
				}
				range_[ix] = r1x ;
			}
		}

		void quick_sort_partition (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<INDEX> mid_one) const {
			INDEX ix = lb ;
			INDEX iy = rb ;
			const auto r1x = range_[ix] ;
			while (TRUE) {
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r2x = operator_compr (array_[range_[iy]] ,array_[r1x]) ;
					if (r2x <= ZERO)
						break ;
					iy-- ;
				}
				if (ix >= iy)
					break ;
				range_[ix] = range_[iy] ;
				ix++ ;
				while (TRUE) {
					if (ix >= iy)
						break ;
					const auto r3x = operator_compr (array_[range_[ix]] ,array_[r1x]) ;
					if (r3x >= ZERO)
						break ;
					ix++ ;
				}
				if (ix >= iy)
					break ;
				range_[iy] = range_[ix] ;
				iy-- ;
			}
			range_[ix] = r1x ;
			mid_one = ix ;
		}

		void quick_sort (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> ideal) const {
			auto rax = ideal ;
			INDEX ix = lb ;
			INDEX iy = rb ;
			while (TRUE) {
				if (ix >= iy)
					break ;
				if (rax <= 0)
					break ;
				rax = rax / 2 + rax / 4 ;
				INDEX jx = NONE ;
				quick_sort_partition (array_ ,range_ ,ix ,iy ,jx) ;
				INDEX iz = jx - 1 ;
				quick_sort (array_ ,range_ ,ix ,iz ,rax) ;
				ix = jx + 1 ;
			}
			if (ix >= iy)
				return ;
			insert_sort (array_ ,range_ ,ix ,iy) ;
		}
	} ;
} ;

template <class UNIT1>
using SortProc = typename SORTPROC_HELP<UNIT1 ,ALWAYS>::SortProc ;

template <class...>
trait DEQUE_HELP ;

template <class ITEM ,class SIZE>
trait DEQUE_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	using RESERVE_SIZE = ENUM_ADD<SIZE ,CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_IDEN ,ENUM_ZERO>> ;

	struct NODE {
		ITEM mItem ;
	} ;

	class Deque {
	private:
		Buffer<NODE ,RESERVE_SIZE> mDeque ;
		INDEX mRead ;
		INDEX mWrite ;

	public:
		implicit Deque () {
			mDeque = Buffer<NODE ,RESERVE_SIZE> (0) ;
			clear () ;
		}

		explicit Deque (CREF<LENGTH> size_) {
			mDeque = Buffer<NODE ,RESERVE_SIZE> (reserve_size (size_)) ;
			clear () ;
		}

		explicit Deque (CREF<csc_initializer_list<ITEM>> that) {
			mDeque = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (LENGTH (that.size ()))) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			if (mDeque.size () == 0)
				return ZERO ;
			return mDeque.size () - 1 ;
		}

		LENGTH length () const {
			if (mDeque.size () == 0)
				return ZERO ;
			return (mWrite - mRead + mDeque.size ()) % mDeque.size () ;
		}

		void clear () {
			mRead = 0 ;
			mWrite = 0 ;
		}

		ArrayIterator<Deque ,ITEM> begin () const leftvalue {
			return ArrayIterator<Deque ,ITEM> (CRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<Deque ,ITEM> end () const leftvalue {
			return ArrayIterator<Deque ,ITEM> (CRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<Deque> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<Deque>::from (thiz) ;
			return ArrayIterator<ArrayRange<Deque> ,INDEX> (CRef<ArrayRange<Deque>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			if (mDeque.size () == 0)
				return ZERO ;
			return mRead ;
		}

		INDEX iend () const {
			if (mDeque.size () == 0)
				return ZERO ;
			return mWrite ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return (index + 1) % mDeque.size () ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (ensure_index (index)) ;
			return mDeque[index].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (ensure_index (index)) ;
			return mDeque[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			Array<INDEX> ret = range () ;
			SortProc<Deque>::sort (thiz ,ret ,0 ,ret.length () - 1) ;
			return move (ret) ;
		}

		BOOL empty () const {
			return mRead == mWrite ;
		}

		BOOL full () const {
			if (mDeque.size () == 0)
				return TRUE ;
			if (mRead != (mWrite + 1) % mDeque.size ())
				return FALSE ;
			return TRUE ;
		}

		void add (CREF<ITEM> item) {
			add (move (item)) ;
		}

		inline VREF<Deque> operator<< (CREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item) {
			update_resize () ;
			INDEX ix = mWrite ;
			mDeque[ix].mItem = move (item) ;
			mWrite = (mWrite + 1) % mDeque.size () ;
			update_resize () ;
		}

		inline VREF<Deque> operator<< (RREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void take () {
			assert (ifnot (empty ())) ;
			INDEX ix = mRead ;
			auto rax = move (mDeque[ix].mItem) ;
			noop (rax) ;
			mRead = (mRead + 1) % mDeque.size () ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = mRead ;
			item = move (mDeque[ix].mItem) ;
			mRead = (mRead + 1) % mDeque.size () ;
		}

		inline VREF<Deque> operator>> (VREF<ITEM> item) {
			take (item) ;
			return thiz ;
		}

		void push (CREF<ITEM> item) {
			push (move (item)) ;
		}

		void push (RREF<ITEM> item) {
			update_resize () ;
			INDEX ix = (mRead - 1 + mDeque.size ()) % mDeque.size () ;
			mDeque[ix].mItem = move (item) ;
			mRead = ix ;
			update_resize () ;
		}

		void pop () {
			assert (ifnot (empty ())) ;
			INDEX ix = (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
			auto rax = move (mDeque[ix].mItem) ;
			noop (rax) ;
			mWrite = ix ;
		}

		void pop (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
			item = move (mDeque[ix].mItem) ;
			mWrite = ix ;
		}

		INDEX head () const {
			if (empty ())
				return NONE ;
			return mRead ;
		}

		INDEX tail () const {
			if (empty ())
				return NONE ;
			return (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
		}

		INDEX insert () leftvalue {
			update_resize () ;
			INDEX ret = mWrite ;
			mWrite = (mWrite + 1) % mDeque.size () ;
			update_resize () ;
			return move (ret) ;
		}

	private:
		LENGTH reserve_size (CREF<LENGTH> size_) const {
			return size_ + LENGTH (size_ > 0) ;
		}

		INDEX ensure_index (CREF<INDEX> index) const {
			if (mRead >= mWrite)
				return vbetween (index ,mRead ,mWrite) ;
			if (vbetween (index ,0 ,mWrite))
				return TRUE ;
			if (vbetween (index ,mRead ,mDeque.size ()))
				return TRUE ;
			return FALSE ;
		}

		LENGTH resize_size () const {
			using R1X = typename ALLOCATOR_HELP<NODE ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_POW ;
			using R2X = typename ALLOCATOR_HELP<NODE ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_MIN_SIZE ;
			return vmax (mDeque.size () * R1X::value ,R2X::value) ;
		}

		void update_resize () {
			if (mDeque.size () > 0)
				if (mRead != mWrite)
					return ;
			const auto r1x = mDeque.size () ;
			mDeque.resize (resize_size ()) ;
			const auto r2x = mDeque.size () ;
			if ifswitch (TRUE) {
				if (mRead <= mWrite)
					discard ;
				for (auto &&i : CORE::iter (mWrite ,r1x)) {
					INDEX ix = r1x - 1 - i ;
					INDEX iy = r2x - 1 - i ;
					mDeque[iy].mItem = move (mDeque[ix].mItem) ;
				}
			}
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Deque = typename DEQUE_HELP<ITEM ,SIZE ,ALWAYS>::Deque ;

template <class...>
trait PRIORITY_HELP ;

template <class ITEM ,class SIZE>
trait PRIORITY_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	using RESERVE_SIZE = ENUM_ADD<SIZE ,CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_IDEN ,ENUM_ZERO>> ;

	struct NODE {
		ITEM mItem ;
		INDEX mMap ;
	} ;

	class Priority {
	private:
		Buffer<NODE ,RESERVE_SIZE> mPriority ;
		INDEX mWrite ;

	public:
		implicit Priority () {
			mPriority = Buffer<NODE ,RESERVE_SIZE> (0) ;
			clear () ;
		}

		explicit Priority (CREF<LENGTH> size_) {
			mPriority = Buffer<NODE ,RESERVE_SIZE> (reserve_size (size_)) ;
			clear () ;
		}

		explicit Priority (CREF<csc_initializer_list<ITEM>> that) {
			mPriority = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (LENGTH (that.size ()))) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			if (mPriority.size () == 0)
				return ZERO ;
			return mPriority.size () - 1 ;
		}

		LENGTH length () const {
			if (mPriority.size () == 0)
				return ZERO ;
			return mWrite ;
		}

		void clear () {
			mWrite = 0 ;
		}

		ArrayIterator<Priority ,ITEM> begin () const leftvalue {
			return ArrayIterator<Priority ,ITEM> (CRef<Priority>::reference (thiz)) ;
		}

		ArrayIterator<Priority ,ITEM> end () const leftvalue {
			return ArrayIterator<Priority ,ITEM> (CRef<Priority>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<Priority> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<Priority>::from (thiz) ;
			return ArrayIterator<ArrayRange<Priority> ,INDEX> (CRef<ArrayRange<Priority>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			if (mPriority.size () == 0)
				return ZERO ;
			return ZERO ;
		}

		INDEX iend () const {
			if (mPriority.size () == 0)
				return ZERO ;
			return mWrite ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (ensure_index (index)) ;
			return mPriority[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX get (CREF<INDEX> index) const {
			assert (ensure_index (index)) ;
			return mPriority[index].mMap ;
		}

		void set (CREF<INDEX> index ,CREF<INDEX> map_) {
			assert (ensure_index (index)) ;
			mPriority[index].mMap = map_ ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			Array<INDEX> ret = range () ;
			if ifswitch (TRUE) {
				INDEX ix = ret.length () - 1 ;
				while (TRUE) {
					if (ix < 0)
						break ;
					swap (ret[0] ,ret[ix]) ;
					compute_order (ret ,ix) ;
					ix-- ;
				}
			}
			const auto r1x = ret.size () ;
			for (auto &&i : CORE::iter (0 ,r1x / 2)) {
				INDEX iy = r1x - 1 - i ;
				swap (ret[i] ,ret[iy]) ;
			}
			return move (ret) ;
		}

		BOOL empty () const {
			return mWrite == ZERO ;
		}

		BOOL full () const {
			if (mPriority.size () == 0)
				return TRUE ;
			if (mWrite < mPriority.size () - 1)
				return FALSE ;
			return TRUE ;
		}

		void add (CREF<ITEM> item) {
			add (item ,NONE) ;
		}

		inline VREF<Priority> operator<< (CREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (item ,NONE) ;
		}

		inline VREF<Priority> operator<< (RREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			update_resize () ;
			INDEX ix = mWrite ;
			mPriority[ix].mItem = move (item) ;
			mPriority[ix].mMap = map_ ;
			mWrite++ ;
			update_insert (ix) ;
			update_resize () ;
		}

		void take () {
			assert (ifnot (empty ())) ;
			INDEX ix = mWrite - 1 ;
			mPriority[0] = move (mPriority[ix]) ;
			mWrite = ix ;
			update_insert (0) ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = mWrite - 1 ;
			item = move (mPriority[0].mItem) ;
			mPriority[0] = move (mPriority[ix]) ;
			mWrite = ix ;
			update_insert (0) ;
		}

		inline VREF<Priority> operator>> (VREF<ITEM> item) {
			take (item) ;
			return thiz ;
		}

		INDEX head () const {
			if (empty ())
				return NONE ;
			return ZERO ;
		}

		void remove (CREF<INDEX> index) {
			assert (ensure_index (index)) ;
			INDEX ix = mWrite - 1 ;
			mPriority[index] = move (mPriority[ix]) ;
			mWrite = ix ;
			update_insert (index) ;
		}

	private:
		LENGTH reserve_size (CREF<LENGTH> size_) const {
			return size_ + LENGTH (size_ > 0) ;
		}

		INDEX ensure_index (CREF<INDEX> index) const {
			return vbetween (index ,0 ,mWrite) ;
		}

		LENGTH resize_size () const {
			using R1X = typename ALLOCATOR_HELP<NODE ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_POW ;
			using R2X = typename ALLOCATOR_HELP<NODE ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_MIN_SIZE ;
			return vmax (mPriority.size () * R1X::value ,R2X::value) ;
		}

		void update_resize () {
			if (mPriority.size () > 0)
				if (mWrite < mPriority.size ())
					return ;
			mPriority.resize (resize_size ()) ;
		}

		INDEX parent (CREF<INDEX> curr) const {
			if (curr <= 0)
				return NONE ;
			return (curr - 1) / 2 ;
		}

		INDEX left_child (CREF<INDEX> curr) const {
			return curr * 2 + 1 ;
		}

		INDEX right_child (CREF<INDEX> curr) const {
			return curr * 2 + 2 ;
		}

		void update_insert (CREF<INDEX> curr) {
			INDEX ix = curr ;
			auto rax = move (mPriority[ix]) ;
			while (TRUE) {
				INDEX iy = parent (ix) ;
				if (iy < 0)
					break ;
				const auto r1x = operator_compr (rax.mItem ,mPriority[iy].mItem) ;
				if (r1x >= ZERO)
					break ;
				mPriority[ix] = move (mPriority[iy]) ;
				ix = iy ;
			}
			while (TRUE) {
				INDEX iy = left_child (ix) ;
				if (iy >= mWrite)
					break ;
				INDEX jx = ix ;
				if ifswitch (TRUE) {
					const auto r2x = operator_compr (rax.mItem ,mPriority[iy].mItem) ;
					if (r2x <= ZERO)
						discard ;
					jx = iy ;
				}
				INDEX iz = right_child (ix) ;
				if ifswitch (TRUE) {
					if (iz >= mWrite)
						discard ;
					const auto r3x = invoke ([&] () {
						if (jx == ix)
							return operator_compr (rax.mItem ,mPriority[iz].mItem) ;
						return operator_compr (mPriority[jx].mItem ,mPriority[iz].mItem) ;
					}) ;
					if (r3x <= ZERO)
						discard ;
					jx = iz ;
				}
				if (jx == ix)
					break ;
				mPriority[ix] = move (mPriority[jx]) ;
				ix = jx ;
			}
			mPriority[ix] = move (rax) ;
		}

		void compute_order (VREF<Array<INDEX>> range_ ,VREF<LENGTH> iw) const {
			INDEX ix = 0 ;
			const auto r1x = range_[ix] ;
			while (TRUE) {
				INDEX iy = left_child (ix) ;
				if (iy >= iw)
					break ;
				INDEX jx = ix ;
				if ifswitch (TRUE) {
					const auto r2x = operator_compr (mPriority[r1x].mItem ,mPriority[range_[iy]].mItem) ;
					if (r2x <= ZERO)
						discard ;
					jx = iy ;
				}
				INDEX iz = right_child (ix) ;
				if ifswitch (TRUE) {
					if (iz >= iw)
						discard ;
					const auto r3x = invoke ([&] () {
						if (jx == ix)
							return operator_compr (mPriority[r1x].mItem ,mPriority[range_[iz]].mItem) ;
						return operator_compr (mPriority[range_[jx]].mItem ,mPriority[range_[iz]].mItem) ;
					}) ;
					if (r3x <= ZERO)
						discard ;
					jx = iz ;
				}
				if (jx == ix)
					break ;
				range_[ix] = range_[jx] ;
				ix = jx ;
			}
			range_[ix] = r1x ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Priority = typename PRIORITY_HELP<ITEM ,SIZE ,ALWAYS>::Priority ;

template <class...>
trait LIST_HELP ;

template <class ITEM ,class SIZE>
trait LIST_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	class List {
	private:
		Allocator<NODE ,SIZE> mList ;
		INDEX mFirst ;
		INDEX mLast ;

	public:
		implicit List () {
			mList = Allocator<NODE ,SIZE> (0) ;
			clear () ;
		}

		explicit List (CREF<LENGTH> size_) {
			mList = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit List (CREF<csc_initializer_list<ITEM>> that) {
			mList = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			return mList.size () ;
		}

		LENGTH length () const {
			return mList.length () ;
		}

		void clear () {
			mList.clear () ;
			mFirst = NONE ;
			mLast = NONE ;
		}

		ArrayIterator<List ,ITEM> begin () const leftvalue {
			return ArrayIterator<List ,ITEM> (CRef<List>::reference (thiz)) ;
		}

		ArrayIterator<List ,ITEM> end () const leftvalue {
			return ArrayIterator<List ,ITEM> (CRef<List>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<List> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<List>::from (thiz) ;
			return ArrayIterator<ArrayRange<List> ,INDEX> (CRef<ArrayRange<List>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			return mFirst ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return mList[index].mNext ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			return mList[index].mItem.self ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mList[index].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			Array<INDEX> ret = range () ;
			SortProc<List>::sort (thiz ,ret ,0 ,ret.length () - 1) ;
			return move (ret) ;
		}

		BOOL empty () const {
			return mList.length () == 0 ;
		}

		BOOL full () const {
			return mList.length () == mList.size () ;
		}

		void add (CREF<ITEM> item) {
			add (move (item)) ;
		}

		inline VREF<List> operator<< (CREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = mList.alloc () ;
			mList[ix].mItem = move (rax) ;
			mList[ix].mPrev = mLast ;
			mList[ix].mNext = NONE ;
			curr_next (mLast ,ix) ;
			mLast = ix ;
		}

		inline VREF<List> operator<< (RREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void take () {
			assert (ifnot (empty ())) ;
			INDEX ix = mFirst ;
			mFirst = mList[ix].mNext ;
			curr_prev (mFirst ,NONE) ;
			mList.free (ix) ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = mFirst ;
			item = move (mList[ix].mItem.self) ;
			mFirst = mList[ix].mNext ;
			curr_prev (mFirst ,NONE) ;
			mList.free (ix) ;
		}

		inline VREF<List> operator>> (VREF<ITEM> item) {
			take (item) ;
			return thiz ;
		}

		void push (CREF<ITEM> item) {
			push (move (item)) ;
		}

		void push (RREF<ITEM> item) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = mList.alloc () ;
			mList[ix].mItem = move (rax) ;
			mList[ix].mPrev = NONE ;
			mList[ix].mNext = mFirst ;
			curr_prev (mFirst ,ix) ;
			mFirst = ix ;
		}

		void pop () {
			assert (ifnot (empty ())) ;
			INDEX ix = mLast ;
			mLast = mList[ix].mPrev ;
			curr_next (mLast ,NONE) ;
			mList.free (ix) ;
		}

		void pop (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = mLast ;
			item = move (mList[ix].mItem.self) ;
			mLast = mList[ix].mPrev ;
			curr_next (mLast ,NONE) ;
			mList.free (ix) ;
		}

		INDEX head () const {
			if (empty ())
				return NONE ;
			return mFirst ;
		}

		INDEX tail () const {
			if (empty ())
				return NONE ;
			return mLast ;
		}

		INDEX insert () leftvalue {
			auto rax = Box<ITEM>::make () ;
			INDEX ret = mList.alloc () ;
			mList[ret].mItem = move (rax) ;
			mList[ret].mPrev = mLast ;
			mList[ret].mNext = NONE ;
			curr_next (mLast ,ret) ;
			mLast = ret ;
			return move (ret) ;
		}

		INDEX insert (CREF<INDEX> index) leftvalue {
			auto rax = Box<ITEM>::make () ;
			INDEX ret = mList.alloc () ;
			mList[ret].mItem = move (rax) ;
			mList[ret].mPrev = mList[index].mPrev ;
			mList[ret].mNext = index ;
			mList[index].mPrev = ret ;
			curr_next (mList[index].mPrev ,ret) ;
			return move (ret) ;
		}

		void remove (CREF<INDEX> index) {
			curr_next (mList[index].mPrev ,mList[index].mNext) ;
			curr_prev (mList[index].mNext ,mList[index].mPrev) ;
			mList.free (index) ;
		}

		void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			if (index1 == index2)
				return ;
			const auto r1x = mList[index1].mPrev ;
			const auto r2x = mList[index1].mNext ;
			const auto r3x = mList[index2].mPrev ;
			const auto r4x = mList[index2].mNext ;
			curr_next (r1x ,index2) ;
			curr_prev (r2x ,index2) ;
			curr_next (r3x ,index1) ;
			curr_prev (r4x ,index1) ;
			sawp (mList[index1].mPrev ,mList[index2].mPrev) ;
			sawp (mList[index1].mNext ,mList[index2].mNext) ;
		}

		void order (CREF<Array<INDEX>> range_) {
			assert (range_.length () == length ()) ;
			if (range_.length () < 2)
				return ;
			for (auto &&i : range_.iter ()) {
				mList[range_[i]].mPrev = NONE ;
				mList[range_[i]].mNext = NONE ;
			}
			for (auto &&i : CORE::iter (0 ,1)) {
				mList[range_[i]].mPrev = NONE ;
				mList[range_[i]].mNext = range_[i + 1] ;
				mFirst = range_[i] ;
			}
			for (auto &&i : CORE::iter (1 ,range_.length () - 1)) {
				mList[range_[i]].mPrev = range_[i - 1] ;
				mList[range_[i]].mNext = range_[i + 1] ;
			}
			for (auto &&i : CORE::iter (range_.length () - 1 ,range_.length ())) {
				mList[range_[i]].mPrev = range_[i - 1] ;
				mList[range_[i]].mNext = NONE ;
				mLast = range_[i] ;
			}
		}

	private:
		void curr_next (CREF<INDEX> curr ,CREF<INDEX> next) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (curr != NONE)
					discard ;
				mFirst = next ;
			}
			if ifswitch (eax) {
				mList[curr].mNext = next ;
			}
		}

		void curr_prev (CREF<INDEX> curr ,CREF<INDEX> prev) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (curr != NONE)
					discard ;
				mLast = prev ;
			}
			if ifswitch (eax) {
				mList[curr].mPrev = prev ;
			}
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using List = typename LIST_HELP<ITEM ,SIZE ,ALWAYS>::List ;

template <class...>
trait ARRAYLIST_HELP ;

template <class ITEM ,class SIZE>
trait ARRAYLIST_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
	} ;

	class ArrayList {
	private:
		Allocator<NODE ,SIZE> mList ;
		Buffer<INDEX ,SIZE> mRange ;
		INDEX mFree ;

	public:
		implicit ArrayList () {
			mList = Allocator<NODE ,SIZE> (0) ;
			clear () ;
		}

		explicit ArrayList (CREF<LENGTH> size_) {
			mList = Allocator<NODE ,SIZE> (size_) ;
			mRange = Buffer<INDEX ,SIZE> (size_) ;
			clear () ;
		}

		explicit ArrayList (CREF<csc_initializer_list<ITEM>> that) {
			mList = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			return mList.size () ;
		}

		LENGTH length () const {
			return mList.length () ;
		}

		void clear () {
			mList.clear () ;
			for (auto &&i : CORE::iter (0 ,mRange.size ()))
				mRange[i] = NONE ;
			mFree = 0 ;
		}

		ArrayIterator<ArrayList ,ITEM> begin () const leftvalue {
			return ArrayIterator<ArrayList ,ITEM> (CRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<ArrayList ,ITEM> end () const leftvalue {
			return ArrayIterator<ArrayList ,ITEM> (CRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<ArrayList> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<ArrayList>::from (thiz) ;
			return ArrayIterator<ArrayRange<ArrayList> ,INDEX> (CRef<ArrayRange<ArrayList>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CORE::iter (0 ,mRange.size ())) {
				if (mRange[i] != NONE)
					return i ;
			}
			return NONE ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			const auto r1x = index + 1 ;
			for (auto &&i : CORE::iter (r1x ,mRange.size ())) {
				if (mRange[i] != NONE)
					return i ;
			}
			return NONE ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			return mList[mRange[index]].mItem.self ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mList[mRange[index]].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			Array<INDEX> ret = range () ;
			SortProc<ArrayList>::sort (thiz ,ret ,0 ,ret.length () - 1) ;
			return move (ret) ;
		}

		BOOL empty () const {
			return mList.length () == 0 ;
		}

		void add (CREF<ITEM> item) {
			add (move (item)) ;
		}

		inline VREF<ArrayList> operator<< (CREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = mList.alloc () ;
			update_range (ix) ;
			mList[ix].mItem = move (rax) ;
			INDEX jx = find_next_free () ;
			mRange[jx] = ix ;
		}

		inline VREF<ArrayList> operator<< (RREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		INDEX insert () leftvalue {
			auto rax = Box<ITEM>::make () ;
			INDEX ret = mList.alloc () ;
			update_range (ret) ;
			mList[ret].mItem = move (rax) ;
			INDEX jx = find_next_free () ;
			mRange[jx] = ret ;
			return move (ret) ;
		}

		INDEX insert (CREF<INDEX> index) leftvalue {
			if ifswitch (TRUE) {
				if (mRange[index] != NONE)
					discard ;
				auto rax = Box<ITEM>::make () ;
				INDEX ix = mList.alloc () ;
				update_range (ix) ;
				mList[ix].mItem = move (rax) ;
				mRange[index] = ix ;
			}
			return index ;
		}

		void remove (CREF<INDEX> index) {
			mList.free (mRange[index]) ;
			mRange[index] = NONE ;
		}

		void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			if (index1 == index2)
				return ;
			swap (mRange[index1] ,mRange[index2]) ;
		}

		void order (CREF<Array<INDEX>> range_) {
			assert (range_.length () == length ()) ;
			if (range_.length () < 2)
				return ;
			const auto r1x = move (mRange) ;
			mRange.resize (range_.length ()) ;
			for (auto &&i : range_.iter ()) {
				INDEX ix = r1x[range_[i]] ;
				mRange[i] = ix ;
			}
		}

		void remap () {
			if (mRange.size () == mList.length ())
				return ;
			mRange.resize (mList.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : CORE::iter (0 ,mList.size ())) {
				if ifnot (mList.used (i))
					continue ;
				mRange[ix] = i ;
				ix++ ;
			}
			assert (ix == mRange.size ()) ;
			mFree = 0 ;
		}

	private:
		INDEX find_next_free () leftvalue {
			assert (mRange.size () > 0) ;
			const auto r1x = mFree % mRange.size () ;
			mFree = r1x ;
			while (TRUE) {
				if (mFree >= mRange.size ())
					break ;
				if (mRange[mFree] == NONE)
					return mFree ;
				mFree++ ;
			}
			mFree = 0 ;
			while (TRUE) {
				if (mFree >= r1x)
					break ;
				if (mRange[mFree] == NONE)
					return mFree ;
				mFree++ ;
			}
			return NONE ;
		}

		void update_range (CREF<INDEX> curr) {
			const auto r1x = mRange.size () ;
			if (r1x == mList.size ())
				return ;
			mRange.resize (mList.size ()) ;
			for (auto &&i : CORE::iter (r1x ,mRange.size ()))
				mRange[i] = NONE ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using ArrayList = typename ARRAYLIST_HELP<ITEM ,SIZE ,ALWAYS>::ArrayList ;

template <class...>
trait BITPROXY_HELP ;

template <class UNIT1 ,class UNIT2>
trait BITPROXY_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_VARIABLE<UNIT1>>> {
	class BitProxy {
	private:
		VRef<UNIT2> mBase ;
		INDEX mY ;

	public:
		implicit BitProxy () = delete ;

		explicit BitProxy (RREF<VRef<UNIT2>> base ,CREF<INDEX> y) {
			mBase = move (base) ;
			mY = y ;
		}

		inline implicit operator BOOL () const rightvalue {
			return mBase->get (mY) ;
		}

		inline void operator= (CREF<BOOL> that) rightvalue {
			mBase->set (mY ,that) ;
		}
	} ;
} ;

template <class UNIT1 ,class UNIT2>
trait BITPROXY_HELP<UNIT1 ,UNIT2 ,REQUIRE<IS_CONSTANT<UNIT1>>> {
	class BitProxy {
	private:
		CRef<UNIT2> mBase ;
		INDEX mY ;

	public:
		implicit BitProxy () = delete ;

		explicit BitProxy (RREF<CRef<UNIT2>> base ,CREF<INDEX> y) {
			mBase = move (base) ;
			mY = y ;
		}

		inline implicit operator BOOL () const rightvalue {
			return mBase->get (mY) ;
		}
	} ;
} ;

template <class UNIT1>
using BitProxy = typename BITPROXY_HELP<XREF<UNIT1> ,REMOVE_REF<UNIT1> ,ALWAYS>::BitProxy ;

template <class...>
trait BITSET_HELP ;

template <class...>
trait BITSET_BYTECACHE_HELP ;

template <>
trait BITSET_BYTECACHE_HELP<ALWAYS> {
	using SIZE = ENUMAS<VAL ,ENUMID<256>> ;

	class ByteCache {
	private:
		BoxBuffer<LENGTH ,SIZE> mCache ;

	public:
		imports CREF<ByteCache> instance () {
			return memorize ([&] () {
				ByteCache ret ;
				ret.mCache = BoxBuffer<LENGTH ,SIZE> ({
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
				return move (ret) ;
			}) ;
		}

		inline LENGTH operator[] (CREF<BYTE> k) const {
			const auto r1x = INDEX (k) ;
			return mCache[r1x] ;
		}
	} ;
} ;

template <class SIZE>
trait BITSET_HELP<SIZE ,ALWAYS> {
	using RESERVE_SIZE = CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_MOD<ENUM_ADD<SIZE ,RANK7> ,RANK8> ,SIZE> ;
	using ITEM = INDEX ;

	class BitSet {
	private:
		Buffer<BYTE ,RESERVE_SIZE> mSet ;
		LENGTH mWidth ;

	public:
		implicit BitSet () {
			mSet = Buffer<BYTE ,RESERVE_SIZE> (0) ;
			mWidth = vmax (SIZE::value ,ZERO) ;
			clear () ;
		}

		explicit BitSet (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			mSet = Buffer<BYTE ,RESERVE_SIZE> (reserve_size (size_)) ;
			mWidth = size_ ;
			clear () ;
		}

		explicit BitSet (CREF<LENGTH> size_ ,CREF<Array<INDEX>> item) {
			assert (size_ >= 0) ;
			mSet = Buffer<BYTE ,RESERVE_SIZE> (reserve_size (size_)) ;
			mWidth = size_ ;
			clear () ;
			for (auto &&i : item)
				add (i) ;
		}

		LENGTH size () const {
			if (mSet.size () == 0)
				return ZERO ;
			return mWidth ;
		}

		LENGTH length () const {
			using R1X = typename BITSET_BYTECACHE_HELP<ALWAYS>::ByteCache ;
			LENGTH ret = 0 ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				ret += R1X::instance ()[mSet[i]] ;
			if ifswitch (TRUE) {
				if (mWidth % 8 == 0)
					discard ;
				const auto r1x = BYTE (0X01) << (mWidth % 8) ;
				const auto r2x = mSet[mWidth / 8] & ~BYTE (INDEX (r1x) - 1) ;
				ret -= R1X::instance ()[r2x] ;
			}
			return move (ret) ;
		}

		void clear () {
			fill (BYTE (0X00)) ;
		}

		void fill (CREF<BYTE> item) {
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				mSet[i] = item ;
		}

		ArrayIterator<BitSet ,ITEM> begin () const leftvalue {
			return ArrayIterator<BitSet ,ITEM> (CRef<BitSet>::reference (thiz)) ;
		}

		ArrayIterator<BitSet ,ITEM> end () const leftvalue {
			return ArrayIterator<BitSet ,ITEM> (CRef<BitSet>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<BitSet> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<BitSet>::from (thiz) ;
			return ArrayIterator<ArrayRange<BitSet> ,INDEX> (CRef<ArrayRange<BitSet>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CORE::iter (0 ,size ())) {
				if (get (i))
					return i ;
			}
			return NONE ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			const auto r1x = index + 1 ;
			for (auto &&i : CORE::iter (r1x ,size ())) {
				if (get (i))
					return i ;
			}
			return NONE ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return index ;
		}

		BOOL get (CREF<INDEX> index) const {
			assert (ensure_index (index)) ;
			INDEX ix = index / 8 ;
			const auto r1x = BYTE (0X01) << (index % 8) ;
			const auto r2x = mSet[ix] & r1x ;
			if (r2x == BYTE (0X00))
				return FALSE ;
			return TRUE ;
		}

		void set (CREF<INDEX> index ,CREF<BOOL> map_) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (map_)
					discard ;
				add (index) ;
			}
			if ifswitch (eax) {
				erase (index) ;
			}
		}

		inline BitProxy<VREF<BitSet>> operator[] (CREF<INDEX> index) leftvalue {
			return BitProxy<VREF<BitSet>> (VRef<BitSet>::reference (thiz) ,index) ;
		}

		inline BitProxy<CREF<BitSet>> operator[] (CREF<INDEX> index) const leftvalue {
			return BitProxy<CREF<BitSet>> (CRef<BitSet>::reference (thiz) ,index) ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		void add (CREF<INDEX> index) {
			assert (ensure_index (index)) ;
			INDEX ix = index / 8 ;
			const auto r1x = BYTE (0X01) << (index % 8) ;
			mSet[ix] |= r1x ;
		}

		inline VREF<BitSet> operator<< (CREF<INDEX> item) {
			add (item) ;
			return thiz ;
		}

		void erase (CREF<INDEX> index) {
			assert (ensure_index (index)) ;
			INDEX ix = index / 8 ;
			const auto r1x = BYTE (0X01) << (index % 8) ;
			mSet[ix] &= ~r1x ;
		}

		BOOL equal (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			INDEX ix = mSet.size () - 1 ;
			if (ix < 0)
				return TRUE ;
			for (auto &&i : CORE::iter (0 ,ix)) {
				if (mSet[i] != that.mSet[i])
					return FALSE ;
			}
			const auto r1x = mSet[ix] & BYTE (mWidth % 8 - 1) ;
			const auto r2x = that.mSet[ix] & BYTE (mWidth % 8 - 1) ;
			if (r1x != r2x)
				return FALSE ;
			return TRUE ;
		}

		inline BOOL operator== (CREF<BitSet> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<BitSet> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			INDEX ix = mSet.size () - 1 ;
			if (ix < 0)
				return ZERO ;
			for (auto &&i : CORE::iter (0 ,ix)) {
				const auto r1x = operator_compr (mSet[i] ,that.mSet[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			const auto r2x = BYTE (mWidth % 8 - 1) ;
			const auto r3x = mSet[ix] & r2x ;
			const auto r4x = that.mSet[ix] & r2x ;
			return operator_compr (r3x ,r4x) ;
		}

		inline BOOL operator< (CREF<BitSet> that) const {
			return compr (that) < ZERO ;
		}

		inline BOOL operator<= (CREF<BitSet> that) const {
			return compr (that) <= ZERO ;
		}

		inline BOOL operator> (CREF<BitSet> that) const {
			return compr (that) > ZERO ;
		}

		inline BOOL operator>= (CREF<BitSet> that) const {
			return compr (that) >= ZERO ;
		}

		FLAG hash () const {
			FLAG ret = hashcode () ;
			for (auto &&i : CORE::iter (0 ,size ())) {
				const auto r1x = operator_hash (mSet[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		BitSet band (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] & that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator& (CREF<BitSet> that) const {
			return band (that) ;
		}

		void bandto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				mSet[i] &= that.mSet[i] ;
		}

		inline void operator&= (CREF<BitSet> that) {
			bandto (that) ;
		}

		BitSet bor (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] | that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator| (CREF<BitSet> that) const {
			return bor (that) ;
		}

		void borto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				mSet[i] |= that.mSet[i] ;
		}

		inline void operator|= (CREF<BitSet> that) {
			borto (that) ;
		}

		BitSet bxor (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] ^ that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator^ (CREF<BitSet> that) const {
			return bxor (that) ;
		}

		void bxorto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				mSet[i] ^= that.mSet[i] ;
		}

		inline void operator^= (CREF<BitSet> that) {
			bxorto (that) ;
		}

		BitSet bsub (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] & ~that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator- (CREF<BitSet> that) const {
			return bsub (that) ;
		}

		void bsubto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				mSet[i] &= ~that.mSet[i] ;
		}

		inline void operator-= (CREF<BitSet> that) {
			bsubto (that) ;
		}

		BitSet bnot () const {
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CORE::iter (0 ,mSet.size ()))
				ret.mSet[i] = ~mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator~ () const {
			return bnot () ;
		}

	private:
		LENGTH reserve_size (CREF<LENGTH> size_) const {
			if (size_ <= 0)
				return size_ ;
			return (size_ + 7) / 8 ;
		}

		BOOL ensure_index (CREF<INDEX> index) const {
			return vbetween (index ,0 ,mWidth) ;
		}
	} ;
} ;

template <class SIZE = VARIABLE>
using BitSet = typename BITSET_HELP<SIZE ,ALWAYS>::BitSet ;

template <class...>
trait SET_HELP ;

template <class ITEM ,class SIZE>
trait SET_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mMap ;
		BOOL mRed ;
		BOOL mLR ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;
	} ;

	struct CHILD {
		INDEX mUp ;
		BOOL mLR ;
	} ;

	class Set {
	private:
		Allocator<NODE ,SIZE> mSet ;
		INDEX mRoot ;
		INDEX mTop ;

	public:
		implicit Set () {
			mSet = Allocator<NODE ,SIZE> () ;
			clear () ;
		}

		explicit Set (CREF<LENGTH> size_) {
			mSet = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit Set (CREF<csc_initializer_list<ITEM>> that) {
			mSet = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			return mSet.size () ;
		}

		LENGTH length () const {
			return mSet.length () ;
		}

		void clear () {
			mSet.clear () ;
			mRoot = NONE ;
			mTop = NONE ;
		}

		ArrayIterator<Set ,ITEM> begin () const leftvalue {
			return ArrayIterator<Set ,ITEM> (CRef<Set>::reference (thiz)) ;
		}

		ArrayIterator<Set ,ITEM> end () const leftvalue {
			return ArrayIterator<Set ,ITEM> (CRef<Set>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<Set> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<Set>::from (thiz) ;
			return ArrayIterator<ArrayRange<Set> ,INDEX> (CRef<ArrayRange<Set>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CORE::iter (0 ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			const auto r1x = index + 1 ;
			for (auto &&i : CORE::iter (r1x ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet[index].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX get (CREF<INDEX> index) const {
			return mSet[index].mMap ;
		}

		void set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet[index].mMap = map_ ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			compute_order (mRoot ,ret ,ix) ;
			assert (ix == ret.length ()) ;
			return move (ret) ;
		}

		BOOL empty () const {
			return mSet.length () == 0 ;
		}

		BOOL full () const {
			if (mSet.size () == 0)
				return TRUE ;
			if (mSet.length () < mSet.size ())
				return FALSE ;
			return TRUE ;
		}

		void add (CREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		inline VREF<Set> operator<< (CREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		inline VREF<Set> operator<< (RREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = find (rax) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mSet.alloc () ;
				mSet[ix].mItem = move (rax) ;
				mSet[ix].mMap = map_ ;
				mSet[ix].mRed = TRUE ;
				update_emplace (ix) ;
				update_insert (ix) ;
			}
		}

		void take () {
			assert (ifnot (empty ())) ;
			INDEX ix = head () ;
			remove (ix) ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = head () ;
			item = move (mSet[ix].mItem.self) ;
			remove (ix) ;
		}

		inline VREF<Set> operator>> (VREF<ITEM> item) {
			take (item) ;
			return thiz ;
		}

		INDEX head () const {
			INDEX ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = mSet[ret].mLeft ;
				if (r1x == NONE)
					break ;
				ret = r1x ;
			}
			return move (ret) ;
		}

		INDEX tail () const {
			INDEX ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = mSet[ret].mRight ;
				if (r1x == NONE)
					break ;
				ret = r1x ;
			}
			return move (ret) ;
		}

		void remove (CREF<INDEX> index) {
			if ifswitch (TRUE) {
				if (mSet[index].mLeft == NONE)
					discard ;
				if (mSet[index].mRight == NONE)
					discard ;
				eswap (index ,find_successor (index)) ;
			}
			const auto r1x = invoke ([&] () {
				if (mSet[index].mLeft != NONE)
					return mSet[index].mLeft ;
				if (mSet[index].mRight != NONE)
					return mSet[index].mRight ;
				return NONE ;
			}) ;
			curr_prev (r1x ,parent (index)) ;
			curr_next (parent (index) ,r1x) ;
			if ifswitch (TRUE) {
				if (mSet[index].mRed)
					discard ;
				update_remove (r1x ,mSet[index].mUp) ;
			}
			mSet.free (index) ;
		}

		INDEX find (CREF<ITEM> item) const {
			INDEX ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = operator_compr (item ,mSet[ret].mItem.self) ;
				if (r1x == ZERO)
					break ;
				const auto r2x = invoke ([&] () {
					if (r1x < ZERO)
						return mSet[ret].mLeft ;
					return mSet[ret].mRight ;
				}) ;
				ret = r2x ;
			}
			return move (ret) ;
		}

		INDEX map (CREF<ITEM> item) const {
			INDEX ix = find (item) ;
			if (ix == NONE)
				return NONE ;
			return get (ix) ;
		}

		void erase (CREF<ITEM> item) {
			const auto r1x = find (item) ;
			if (r1x == NONE)
				return ;
			remove (r1x) ;
		}

	private:
		void update_emplace (CREF<INDEX> curr) {
			INDEX ix = mRoot ;
			auto rax = left_child (NONE) ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				const auto r1x = operator_compr (mSet[curr].mItem.self ,mSet[ix].mItem.self) ;
				assert (r1x != ZERO) ;
				const auto r2x = invoke ([&] () {
					if (r1x < ZERO)
						return left_child (ix) ;
					return right_child (ix) ;
				}) ;
				rax = r2x ;
				const auto r3x = invoke ([&] () {
					if ifnot (rax.mLR)
						return mSet[rax.mUp].mLeft ;
					return mSet[rax.mUp].mRight ;
				}) ;
				ix = r3x ;
			}
			mSet[curr].mUp = rax.mUp ;
			mSet[curr].mLR = rax.mLR ;
			mSet[curr].mLeft = NONE ;
			mSet[curr].mRight = NONE ;
			curr_next (parent (curr) ,curr) ;
		}

		void update_insert (CREF<INDEX> curr) {
			INDEX ix = curr ;
			INDEX iy = mSet[ix].mUp ;
			while (TRUE) {
				if (iy == NONE)
					break ;
				if ifnot (mSet[iy].mRed)
					break ;
				INDEX iz = mSet[iy].mUp ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if (iy != mSet[iz].mLeft)
						discard ;
					update_insert_left (ix) ;
					ix = mTop ;
				}
				if ifswitch (eax) {
					if (iy != mSet[iz].mRight)
						discard ;
					update_insert_right (ix) ;
					ix = mTop ;
				}
				iy = mSet[ix].mUp ;
			}
			mSet[mRoot].mRed = FALSE ;
		}

		void update_insert_left (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mUp ;
			INDEX iy = mSet[ix].mUp ;
			INDEX iz = mSet[iy].mRight ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (iz == NONE)
					discard ;
				if ifnot (mSet[iz].mRed)
					discard ;
				mSet[iz].mRed = FALSE ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				mTop = iy ;
			}
			if ifswitch (eax) {
				if (curr != mSet[ix].mRight)
					discard ;
				rotate_left (mSet[iy].mLeft) ;
				mSet[curr].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_right (iy) ;
				mTop = ix ;
			}
			if ifswitch (eax) {
				if (curr != mSet[ix].mLeft)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_right (iy) ;
				mTop = curr ;
			}
		}

		void update_insert_right (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mUp ;
			INDEX iy = mSet[ix].mUp ;
			INDEX iz = mSet[iy].mLeft ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (iz == NONE)
					discard ;
				if ifnot (mSet[iz].mRed)
					discard ;
				mSet[iz].mRed = FALSE ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				mTop = iy ;
			}
			if ifswitch (eax) {
				if (curr != mSet[ix].mLeft)
					discard ;
				rotate_right (mSet[iy].mRight) ;
				mSet[curr].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_left (iy) ;
				mTop = ix ;
			}
			if ifswitch (eax) {
				if (curr != mSet[ix].mRight)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_left (iy) ;
				mTop = curr ;
			}
		}

		void update_remove (CREF<INDEX> curr ,CREF<INDEX> last) {
			INDEX ix = curr ;
			INDEX iy = last ;
			while (TRUE) {
				if (iy == NONE)
					break ;
				if (ix != NONE)
					if (mSet[ix].mRed)
						break ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (ix == mSet[iy].mLeft)
						discard ;
					update_remove_left (iy) ;
					ix = mTop ;
				}
				if ifswitch (eax) {
					if ifnot (ix == mSet[iy].mRight)
						discard ;
					update_remove_right (iy) ;
					ix = mTop ;
				}
				iy = mSet[ix].mUp ;
			}
			if (ix == NONE)
				return ;
			mSet[ix].mRed = FALSE ;
		}

		void update_remove_left (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mRight ;
			if ifswitch (TRUE) {
				if ifnot (mSet[ix].mRed)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[curr].mRed = TRUE ;
				rotate_left (curr) ;
				ix = mSet[curr].mRight ;
			}
			INDEX jx = mSet[ix].mLeft ;
			INDEX jy = mSet[ix].mRight ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (jx != NONE)
					if (mSet[jx].mRed)
						discard ;
				if (jy != NONE)
					if (mSet[jy].mRed)
						discard ;
				mSet[ix].mRed = TRUE ;
				mTop = curr ;
			}
			if ifswitch (eax) {
				if (jy != NONE)
					if (mSet[jy].mRed)
						discard ;
				mSet[jx].mRed = FALSE ;
				mSet[ix].mRed = TRUE ;
				rotate_right (ix) ;
				ix = mSet[curr].mRight ;
				jx = mSet[ix].mLeft ;
				jy = mSet[ix].mRight ;
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_left (curr) ;
				mTop = mRoot ;
			}
			if ifswitch (eax) {
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_left (curr) ;
				mTop = mRoot ;
			}
		}

		void update_remove_right (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mLeft ;
			if ifswitch (TRUE) {
				if ifnot (mSet[ix].mRed)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[curr].mRed = TRUE ;
				rotate_right (curr) ;
				ix = mSet[curr].mLeft ;
			}
			INDEX jx = mSet[ix].mRight ;
			INDEX jy = mSet[ix].mLeft ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (jx != NONE)
					if (mSet[jx].mRed)
						discard ;
				if (jy != NONE)
					if (mSet[jy].mRed)
						discard ;
				mSet[ix].mRed = TRUE ;
				mTop = curr ;
			}
			if ifswitch (eax) {
				if (jy != NONE)
					if (mSet[jy].mRed)
						discard ;
				mSet[jx].mRed = FALSE ;
				mSet[ix].mRed = TRUE ;
				rotate_left (ix) ;
				ix = mSet[curr].mLeft ;
				jx = mSet[ix].mRight ;
				jy = mSet[ix].mLeft ;
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_right (curr) ;
				mTop = mRoot ;
			}
			if ifswitch (eax) {
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_right (curr) ;
				mTop = mRoot ;
			}
		}

		void rotate_left (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mRight ;
			mSet[curr].mRight = mSet[ix].mLeft ;
			curr_prev (mSet[ix].mLeft ,right_child (curr)) ;
			mSet[ix].mUp = mSet[curr].mUp ;
			mSet[ix].mLR = mSet[curr].mLR ;
			mSet[ix].mLeft = curr ;
			mSet[curr].mUp = ix ;
			mSet[curr].mLR = FALSE ;
			curr_next (parent (ix) ,ix) ;
		}

		void rotate_right (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mLeft ;
			mSet[curr].mLeft = mSet[ix].mRight ;
			curr_prev (mSet[ix].mRight ,left_child (curr)) ;
			mSet[ix].mUp = mSet[curr].mUp ;
			mSet[ix].mLR = mSet[curr].mLR ;
			mSet[ix].mRight = curr ;
			mSet[curr].mUp = ix ;
			mSet[curr].mLR = TRUE ;
			curr_next (parent (ix) ,ix) ;
		}

		CHILD parent (CREF<INDEX> curr) const {
			CHILD ret ;
			ret.mUp = mSet[curr].mUp ;
			ret.mLR = mSet[curr].mLR ;
			return move (ret) ;
		}

		CHILD left_child (CREF<INDEX> curr) const {
			CHILD ret ;
			ret.mUp = curr ;
			ret.mLR = FALSE ;
			return move (ret) ;
		}

		CHILD right_child (CREF<INDEX> curr) const {
			CHILD ret ;
			ret.mUp = curr ;
			ret.mLR = TRUE ;
			return move (ret) ;
		}

		void curr_next (CREF<CHILD> up ,CREF<INDEX> next) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (up.mUp != NONE)
					discard ;
				mRoot = next ;
			}
			if ifswitch (eax) {
				if (up.mLR)
					discard ;
				mSet[up.mUp].mLeft = next ;
			}
			if ifswitch (eax) {
				mSet[up.mUp].mRight = next ;
			}
		}

		void curr_prev (CREF<INDEX> curr ,CREF<CHILD> up) {
			if ifswitch (TRUE) {
				if (curr == NONE)
					discard ;
				mSet[curr].mUp = up.mUp ;
				mSet[curr].mLR = up.mLR ;
			}
		}

		INDEX find_successor (CREF<INDEX> index) const {
			INDEX ret = mSet[index].mRight ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = mSet[ret].mLeft ;
				if (r1x == NONE)
					break ;
				ret = r1x ;
			}
			return move (ret) ;
		}

		void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			if (index1 == index2)
				return ;
			const auto r1x = parent (index1) ;
			const auto r2x = mSet[index1].mLeft ;
			const auto r3x = mSet[index1].mRight ;
			const auto r4x = parent (index2) ;
			const auto r5x = mSet[index2].mLeft ;
			const auto r6x = mSet[index2].mRight ;
			curr_next (r1x ,index2) ;
			curr_prev (r2x ,left_child (index2)) ;
			curr_prev (r3x ,right_child (index2)) ;
			curr_next (r4x ,index1) ;
			curr_prev (r5x ,left_child (index1)) ;
			curr_prev (r6x ,right_child (index1)) ;
			swap (mSet[index1].mRed ,mSet[index2].mRed) ;
			swap (mSet[index1].mUp ,mSet[index2].mUp) ;
			swap (mSet[index1].mLR ,mSet[index2].mLR) ;
			swap (mSet[index1].mLeft ,mSet[index2].mLeft) ;
			swap (mSet[index1].mRight ,mSet[index2].mRight) ;
		}

		void compute_order (CREF<INDEX> curr ,VREF<Array<INDEX>> range_ ,VREF<INDEX> iw) const {
			if (curr == NONE)
				return ;
			compute_order (mSet[curr].mLeft ,range_ ,iw) ;
			range_[iw] = curr ;
			iw++ ;
			compute_order (mSet[curr].mRight ,range_ ,iw) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Set = typename SET_HELP<ITEM ,SIZE ,ALWAYS>::Set ;

template <class...>
trait HASHSET_HELP ;

template <class ITEM ,class SIZE>
trait HASHSET_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mMap ;
		FLAG mHash ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	class HashSet {
	private:
		Allocator<NODE ,SIZE> mSet ;
		Buffer<INDEX ,SIZE> mRange ;

	public:
		implicit HashSet () {
			mSet = Allocator<NODE ,SIZE> () ;
			clear () ;
		}

		explicit HashSet (CREF<LENGTH> size_) {
			mSet = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit HashSet (CREF<csc_initializer_list<ITEM>> that) {
			mSet = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			return mSet.size () ;
		}

		LENGTH length () const {
			return mSet.length () ;
		}

		void clear () {
			mSet.clear () ;
			for (auto &&i : CORE::iter (0 ,mRange.size ()))
				mRange[i] = NONE ;
		}

		ArrayIterator<HashSet ,ITEM> begin () const leftvalue {
			return ArrayIterator<HashSet ,ITEM> (CRef<HashSet>::reference (thiz)) ;
		}

		ArrayIterator<HashSet ,ITEM> end () const leftvalue {
			return ArrayIterator<HashSet ,ITEM> (CRef<HashSet>::reference (thiz)) ;
		}

		ArrayIterator<ArrayRange<HashSet> ,INDEX> iter () const leftvalue {
			auto &&tmp = ArrayRange<HashSet>::from (thiz) ;
			return ArrayIterator<ArrayRange<HashSet> ,INDEX> (CRef<ArrayRange<HashSet>>::reference (tmp)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CORE::iter (0 ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			const auto r1x = index + 1 ;
			for (auto &&i : CORE::iter (r1x ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet[index].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX get (CREF<INDEX> index) const {
			return mSet[index].mMap ;
		}

		void set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet[index].mMap = map_ ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			INDEX jx = ibegin () ;
			INDEX jy = iend () ;
			while (TRUE) {
				if (jx == jy)
					break ;
				ret[ix] = jx ;
				ix++ ;
				jx = inext (jx) ;
			}
			return move (ret) ;
		}

		void add (CREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		inline VREF<HashSet> operator<< (CREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		inline VREF<HashSet> operator<< (RREF<ITEM> item) {
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = find (rax) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mSet.alloc () ;
				update_range (ix) ;
				mSet[ix].mItem = move (rax) ;
				mSet[ix].mMap = map_ ;
				mSet[ix].mHash = operator_hash (mSet[ix].mItem.self) ;
				update_emplace (ix) ;
			}
		}

		void remove (CREF<INDEX> index) {
			curr_next (mSet[index].mPrev ,mSet[index].mNext) ;
			curr_prev (mSet[index].mNext ,mSet[index].mPrev) ;
			mSet.free (index) ;
		}

		INDEX find (CREF<ITEM> item) const {
			INDEX ret = NONE ;
			if ifswitch (TRUE) {
				if (mSet.size () == 0)
					discard ;
				const auto r1x = operator_hash (item) ;
				ret = mRange[r1x % mRange.size ()] ;
				while (TRUE) {
					if (ret == NONE)
						break ;
					if (mSet[ret].mHash == r1x)
						if (operator_equal (item ,mSet[ret].mItem.self))
							break ;
					ret = mSet[ret].mNext ;
				}
			}
			return move (ret) ;
		}

		INDEX map (CREF<ITEM> item) const {
			INDEX ix = find (item) ;
			if (ix == NONE)
				return NONE ;
			return get (ix) ;
		}

		void erase (CREF<ITEM> item) {
			const auto r1x = find (item) ;
			if (r1x == NONE)
				return ;
			remove (r1x) ;
		}

	private:
		void update_range (CREF<INDEX> curr) {
			const auto r1x = mRange.size () ;
			if (r1x == mSet.size ())
				return ;
			mRange.resize (mSet.size ()) ;
			for (auto &&i : CORE::iter (0 ,mRange.size ()))
				mRange[i] = NONE ;
			for (auto &&i : CORE::iter (0 ,mSet.size ())) {
				if (i == curr)
					continue ;
				if ifnot (mSet.used (i))
					continue ;
				update_emplace (i) ;
			}
		}

		void update_emplace (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mHash % mRange.size () ;
			mSet[curr].mPrev = NONE ;
			mSet[curr].mNext = mRange[ix] ;
			mSet[mRange[ix]].mPrev = curr ;
			mRange[ix] = curr ;
		}

		void curr_next (CREF<INDEX> curr ,CREF<INDEX> next) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (curr != NONE)
					discard ;
				INDEX ix = mSet[curr].mHash % mRange.size () ;
				mRange[ix] = next ;
			}
			if ifswitch (eax) {
				mSet[curr].mNext = next ;
			}
		}

		void curr_prev (CREF<INDEX> curr ,CREF<INDEX> prev) {
			if ifswitch (TRUE) {
				if (curr == NONE)
					discard ;
				mSet[curr].mPrev = prev ;
			}
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using HashSet = typename HASHSET_HELP<ITEM ,SIZE ,ALWAYS>::HashSet ;
} ;
} ;

namespace CSC {
using namespace ARRAY ;
} ;
