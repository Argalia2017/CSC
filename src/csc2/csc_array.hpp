#pragma once

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
namespace U {
template <class UNIT1>
trait ITERATOR_HELP<UNIT1 ,REQUIRE<IS_CLASS<UNIT1>>> {
	class ArrayIterator {
	private:
		CRef<UNIT1> mBase ;
		INDEX mBegin ;
		INDEX mEnd ;
		INDEX mCurr ;

	public:
		implicit ArrayIterator () = delete ;

		explicit ArrayIterator (CRef<UNIT1> base) {
			mBase = base ;
			mBegin = mBase->ibegin () ;
			mEnd = mBase->iend () ;
			mCurr = mBegin ;
		}

		BOOL good () const {
			return mCurr != mEnd ;
		}

		inline BOOL operator== (CREF<ArrayIterator>) const {
			return ifnot (good ()) ;
		}

		inline BOOL operator!= (CREF<ArrayIterator>) const {
			return good () ;
		}

		CREF<INDEX> at () const leftvalue {
			return mCurr ;
		}

		inline CREF<INDEX> operator* () const leftvalue {
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
} ;

template <class UNTI1>
using ArrayIterator = typename U::ITERATOR_HELP<UNTI1 ,ALWAYS>::ArrayIterator ;

namespace U {
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

		explicit Array (CREF<std::initializer_list<ITEM>> that) {
			mArray = Buffer<ITEM ,SIZE> (LENGTH (that.size ())) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mArray[ix] = move (i) ;
				ix++ ;
			}
		}

		imports Array make_range (CREF<LENGTH> begin_ ,CREF<LENGTH> end_) {
			const auto r1x = max (end_ - begin_ ,ZERO) ;
			Array ret = Array (r1x) ;
			INDEX ix = 0 ;
			for (auto &&i : range (begin_ ,end_)) {
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

		VREF<RegBuffer<ITEM>> raw () leftvalue {
			//@mark
			return bad (TYPEAS<RegBuffer<ITEM>>::id) ;
		}

		CREF<RegBuffer<ITEM>> raw () const leftvalue {
			//@mark
			return bad (TYPEAS<RegBuffer<ITEM>>::id) ;
		}

		void fill (CREF<ITEM> obj) {
			for (auto &&i : range (0 ,size ()))
				mArray[i] = obj ;
		}

		INDEX ibegin () const {
			return 0 ;
		}

		INDEX iend () const {
			return length () ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		ArrayIterator<Array> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Array> (CRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<Array> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Array> (CRef<Array>::reference (thiz)) ;
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
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using Array = typename U::ARRAY_HELP<ITEM ,SIZE ,ALWAYS>::Array ;

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

namespace U {
template <class...>
trait STRING_HELP ;

template <class ITEM ,class SIZE>
trait STRING_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	using RESERVE_SIZE = ENUM_ADD<SIZE ,CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_IDEN ,ENUM_ZERO>> ;

	class String {
	private:
		Buffer<ITEM ,RESERVE_SIZE> mString ;

	public:
		implicit String () {
			mString = Buffer<ITEM ,RESERVE_SIZE> (0) ;
			clear () ;
		}

		template <class ARG1 = ALWAYS ,class = ENABLE<IS_STR<DEPENDENT<ITEM ,ARG1>>>>
		implicit String (CREF<Slice<DEPENDENT<ITEM ,ARG1>>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (that.size ())) ;
			INDEX ix = 0 ;
			for (auto &&i : range (0 ,that.size ())) {
				mString[ix] = that[i] ;
				ix++ ;
			}
			if (ix >= mString.size ())
				return ;
			mString[ix] = ITEM (0) ;
		}

		explicit String (CREF<LENGTH> size_) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (size_)) ;
			clear () ;
		}

		explicit String (CREF<std::initializer_list<ITEM>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (LENGTH (that.size ()))) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mString[ix] = i ;
				ix++ ;
			}
			if (ix >= mString.size ())
				return ;
			mString[ix] = ITEM (0) ;
		}

		template <class...ARG1>
		imports String make_print (XREF<ARG1>...obj) {
			//@mark
		}

		LENGTH size () const {
			if (mString.size () == 0)
				return ZERO ;
			return mString.size () - 1 ;
		}

		LENGTH length () const {
			if (mString.size () == 0)
				return ZERO ;
			for (auto &&i : range (0 ,size ())) {
				if (mString[i] == ITEM (0))
					return i ;
			}
			return size () ;
		}

		VREF<RegBuffer<ITEM>> raw () leftvalue {
			//@mark
			return bad (TYPEAS<RegBuffer<ITEM>>::id) ;
		}

		CREF<RegBuffer<ITEM>> raw () const leftvalue {
			//@mark
			return bad (TYPEAS<RegBuffer<ITEM>>::id) ;
		}

		void clear () {
			for (auto &&i : range (0 ,mString.size ()))
				mString[i] = ITEM (0) ;
		}

		void fill (CREF<ITEM> obj) {
			for (auto &&i : range (0 ,size ()))
				mString[i] = obj ;
		}

		INDEX ibegin () const {
			return 0 ;
		}

		INDEX iend () const {
			return length () ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		ArrayIterator<String> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<String> (CRef<String>::reference (thiz)) ;
		}

		ArrayIterator<String> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<String> (CRef<String>::reference (thiz)) ;
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

		String concat (CREF<String> that) const {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			String ret = String (r1x + r2x) ;
			for (auto &&i : range (0 ,r1x))
				ret.mString[i] = mString[i] ;
			for (auto &&i : range (0 ,r2x))
				ret.mString[r1x + i] = that.mString[i] ;
			return move (ret) ;
		}

		inline String operator+ (CREF<String> that) const {
			return concat (that) ;
		}

		BOOL equal (CREF<String> that) const {
			if (empty () != that.empty ())
				return FALSE ;
			if (empty ())
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
			for (auto &&i : range (0 ,size ())) {
				const auto r1x = operator_hash (mString[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

	private:
		imports LENGTH reserve_size (CREF<LENGTH> size_) {
			return size_ + LENGTH (size_ > 0) ;
		}

		INDEX ensure_index (CREF<INDEX> index) const {
			return between (index ,0 ,size ()) ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using String = typename U::STRING_HELP<ITEM ,SIZE ,ALWAYS>::String ;

namespace U {
template <class...>
trait SORT_PROC_HELP ;

template <class UNIT1>
trait SORT_PROC_HELP<UNIT1 ,ALWAYS> {
	class SortProc {
	public:
		imports void sort (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> begin_ ,CREF<INDEX> end_) {
			const auto r1x = end_ - begin_ ;
			if (r1x <= 1)
				return ;
			INDEX ix = begin_ ;
			INDEX iy = end_ - 1 ;
			assert (between (ix ,0 ,range_.size ())) ;
			assert (between (iy ,0 ,range_.size ())) ;
			quick_sort (array_ ,range_ ,ix ,iy ,r1x) ;
		}

	private:
		imports void insert_sort (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb) {
			for (auto &&i : range (lb ,rb)) {
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

		imports void quick_sort_partition (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,VREF<INDEX> mid_one) {
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
					const auto r2x = operator_compr (array_[range_[ix]] ,array_[r1x]) ;
					if (r2x >= ZERO)
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

		imports void quick_sort (CREF<UNIT1> array_ ,VREF<Array<INDEX>> range_ ,CREF<INDEX> lb ,CREF<INDEX> rb ,CREF<LENGTH> ideal) {
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
} ;

namespace U {
template <class...>
trait DEQUE_HELP ;

template <class ITEM ,class SIZE>
trait DEQUE_HELP<ITEM ,SIZE ,REQUIRE<IS_DEFAULT<ITEM>>> {
	using RESERVE_SIZE = ENUM_ADD<SIZE ,CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_IDEN ,ENUM_ZERO>> ;

	class Deque {
	private:
		Buffer<ITEM ,RESERVE_SIZE> mDeque ;
		INDEX mRead ;
		INDEX mWrite ;

	public:
		implicit Deque () {
			mDeque = Buffer<ITEM ,RESERVE_SIZE> (0) ;
			clear () ;
		}

		explicit Deque (CREF<LENGTH> size_) {
			mDeque = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (size_)) ;
			clear () ;
		}

		explicit Deque (CREF<std::initializer_list<ITEM>> that) {
			mDeque = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (LENGTH (that.size ()))) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mDeque[ix] = i ;
				ix++ ;
			}
			mRead = 0 ;
			mWrite = ix ;
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

		ArrayIterator<Deque> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Deque> (CRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<Deque> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Deque> (CRef<Deque>::reference (thiz)) ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (ensure_index (index)) ;
			return mDeque[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (ensure_index (index)) ;
			return mDeque[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			auto &&thiz = *this ;
			using R1X = typename U::SORT_PROC_HELP<Deque ,ALWAYS>::SortProc ;
			Array<INDEX> ret = range () ;
			R1X::sort (thiz ,ret ,0 ,ret.length () - 1) ;
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
			auto &&thiz = *this ;
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item) {
			update_resize () ;
			mDeque[mWrite] = move (item) ;
			mWrite = (mWrite + 1) % mDeque.size () ;
			update_resize () ;
		}

		inline VREF<Deque> operator<< (RREF<ITEM> item) {
			auto &&thiz = *this ;
			add (move (item)) ;
			return thiz ;
		}

		void take () {
			assert (ifnot (empty ())) ;
			mRead = (mRead + 1) % mDeque.size () ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			item = move (mDeque[mRead]) ;
			mRead = (mRead + 1) % mDeque.size () ;
		}

		inline VREF<Deque> operator>> (VREF<ITEM> item) {
			auto &&thiz = *this ;
			take (item) ;
			return thiz ;
		}

		void push (CREF<ITEM> item) {
			push (move (item)) ;
		}

		void push (RREF<ITEM> item) {
			update_resize () ;
			INDEX ix = (mRead - 1 + mDeque.size ()) % mDeque.size () ;
			mDeque[ix] = move (item) ;
			mRead = ix ;
			update_resize () ;
		}

		void pop () {
			assert (ifnot (empty ())) ;
			mWrite = (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
		}

		INDEX head () const {
			assert (ifnot (empty ())) ;
			return mRead ;
		}

		INDEX tail () const {
			assert (ifnot (empty ())) ;
			return (mWrite - 1 + mDeque.size ()) % mDeque.size () ;
		}

		INDEX insert () {
			update_resize () ;
			INDEX ret = mWrite ;
			mWrite = (mWrite + 1) % mDeque.size () ;
			update_resize () ;
			return move (ret) ;
		}

	private:
		imports LENGTH reserve_size (CREF<LENGTH> size_) {
			return size_ + LENGTH (size_ > 0) ;
		}

		INDEX ensure_index (CREF<INDEX> index) const {
			if (mRead >= mWrite)
				return between (index ,mRead ,mWrite) ;
			if (between (index ,0 ,mWrite))
				return TRUE ;
			if (between (index ,mRead ,mDeque.size ()))
				return TRUE ;
			return FALSE ;
		}

		LENGTH resize_size () const {
			using R1X = typename ALLOCATOR_HELP<ITEM ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_POW ;
			using R2X = typename ALLOCATOR_HELP<ITEM ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_MIN_SIZE ;
			return max (mDeque.size () * R1X::value ,R2X::value) ;
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
				for (auto &&i : range (mWrite ,r1x)) {
					INDEX ix = r1x - 1 - i ;
					INDEX iy = r2x - 1 - i ;
					mDeque[iy] = move (mDeque[ix]) ;
				}
			}
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using Deque = typename U::DEQUE_HELP<ITEM ,SIZE ,ALWAYS>::Deque ;

namespace U {
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
			mPriority = Buffer<NODE ,RESERVE_SIZE> (size_) ;
			clear () ;
		}

		explicit Priority (CREF<std::initializer_list<ITEM>> that) {
			mPriority = Buffer<NODE ,RESERVE_SIZE> (LENGTH (that.size ())) ;
			clear () ;
			for (auto &&i : that)
				add (i) ;
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

		ArrayIterator<Priority> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Priority> (CRef<Priority>::reference (thiz)) ;
		}

		ArrayIterator<Priority> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Priority> (CRef<Priority>::reference (thiz)) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (ensure_index (index)) ;
			return mPriority[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX get (CREF<INDEX> index) const {
			if (index == NONE)
				return NONE ;
			assert (ensure_index (index)) ;
			return mPriority[index].mMap ;
		}

		void set (CREF<INDEX> index ,CREF<INDEX> map_) {
			assert (ensure_index (index)) ;
			mPriority[index].mMap = map_ ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			Array<INDEX> ret = range () ;
			INDEX ix = ret.length () ;
			while (TRUE) {
				ix-- ;
				if (ix < 1)
					break ;
				swap (ret[0] ,ret[ix]) ;
				compute_order (ret ,ix) ;
			}
			const auto r1x = ret.size () ;
			for (auto &&i : range (0 ,r1x / 2)) {
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
			auto &&thiz = *this ;
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
			auto &&thiz = *this ;
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
			item = move (mPriority[0].mItem) ;
			INDEX ix = mWrite - 1 ;
			mPriority[0] = move (mPriority[ix]) ;
			mWrite = ix ;
			update_insert (0) ;
		}

		inline VREF<Priority> operator>> (VREF<ITEM> item) {
			auto &&thiz = *this ;
			take (item) ;
			return thiz ;
		}

		INDEX head () const {
			assert (ifnot (empty ())) ;
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
		imports LENGTH reserve_size (CREF<LENGTH> size_) {
			return size_ + LENGTH (size_ > 0) ;
		}

		INDEX ensure_index (CREF<INDEX> index) const {
			return between (index ,0 ,mWrite) ;
		}

		LENGTH resize_size () const {
			using R1X = typename ALLOCATOR_HELP<ITEM ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_POW ;
			using R2X = typename ALLOCATOR_HELP<ITEM ,RESERVE_SIZE ,ALWAYS>::ALLOCATOR_MIN_SIZE ;
			return max (mPriority.size () * R1X::value ,R2X::value) ;
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

		void compute_order (VREF<Array<INDEX>> range_ ,VREF<LENGTH> write_) const {
			INDEX ix = 0 ;
			const auto r1x = range_[ix] ;
			while (TRUE) {
				INDEX iy = left_child (ix) ;
				if (iy >= mWrite)
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
					if (iz >= mWrite)
						discard ;
					const auto r3x = invoke ([&] () {
						if (jx == ix)
							return operator_compr (r1x ,mPriority[range_[iz]].mItem) ;
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
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using Priority = typename U::PRIORITY_HELP<ITEM ,SIZE ,ALWAYS>::Priority ;

namespace U {
template <class...>
trait LIST_HELP ;

template <class ITEM ,class SIZE>
trait LIST_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mLeft ;
		INDEX mRight ;
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

		explicit List (CREF<std::initializer_list<ITEM>> that) {
			mList = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
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
			mFirst = NONE ;
			mLast = NONE ;
		}

		INDEX ibegin () const {
			return mFirst ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return mList[index].mRight ;
		}

		ArrayIterator<List> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<List> (CRef<List>::reference (thiz)) ;
		}

		ArrayIterator<List> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<List> (CRef<List>::reference (thiz)) ;
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
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			auto &&thiz = *this ;
			using R1X = typename U::SORT_PROC_HELP<List ,ALWAYS>::SortProc ;
			Array<INDEX> ret = range () ;
			R1X::sort (thiz ,ret ,0 ,ret.length () - 1) ;
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
			auto &&thiz = *this ;
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = mList.alloc () ;
			mList[ix].mItem = move (rax) ;
			mList[ix].mLeft = mLast ;
			mList[ix].mRight = NONE ;
			curr_next (mLast) = ix ;
			mLast = ix ;
		}

		inline VREF<List> operator<< (RREF<ITEM> item) {
			auto &&thiz = *this ;
			add (move (item)) ;
			return thiz ;
		}

		void take () {
			assert (ifnot (empty ())) ;
			INDEX ix = mFirst ;
			mFirst = mList[ix].mRight ;
			curr_prev (mFirst) = NONE ;
			mList.free (ix) ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = mFirst ;
			item = move (mList[ix].mItem.self) ;
			mFirst = mList[ix].mRight ;
			curr_prev (mFirst) = NONE ;
			mList.free (ix) ;
		}

		inline VREF<List> operator>> (VREF<ITEM> item) {
			auto &&thiz = *this ;
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
			mList[ix].mLeft = NONE ;
			mList[ix].mRight = mFirst ;
			curr_prev (mFirst) = ix ;
			mFirst = ix ;
		}

		void pop () {
			assert (ifnot (empty ())) ;
			INDEX ix = mLast ;
			mLast = mList[ix].mLeft ;
			curr_next (mLast) = NONE ;
			mList.free (ix) ;
		}

		INDEX head () const {
			assert (ifnot (empty ())) ;
			return mFirst ;
		}

		INDEX tail () const {
			assert (ifnot (empty ())) ;
			return mLast ;
		}

		INDEX insert () {
			auto rax = Box<ITEM>::make () ;
			INDEX ret = mList.alloc () ;
			mList[ret].mItem = move (rax) ;
			mList[ret].mLeft = mLast ;
			mList[ret].mRight = NONE ;
			curr_next (mLast) = ret ;
			mLast = ret ;
			return move (ret) ;
		}

		void remove (CREF<INDEX> index) {
			curr_next (mList[index].mLeft) = mList[index].mRight ;
			curr_prev (mList[index].mRight) = mList[index].mLeft ;
			mList.free (index) ;
		}

		void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			if (index1 == index2)
				return ;
			const auto r1x = mList[index1].mLeft ;
			const auto r2x = mList[index1].mRight ;
			const auto r3x = mList[index2].mLeft ;
			const auto r4x = mList[index2].mRight ;
			curr_next (r1x) = index2 ;
			curr_prev (r2x) = index2 ;
			curr_next (r3x) = index1 ;
			curr_prev (r4x) = index1 ;
			sawp (mList[index1].mLeft ,mList[index2].mLeft) ;
			sawp (mList[index1].mRight ,mList[index2].mRight) ;
		}

		void esort (CREF<Array<INDEX>> range_) {
			assert (range_.length () == length ()) ;
			if (range_.length () < 2)
				return ;
			for (auto &&i : range_) {
				mList[i].mLeft = NONE ;
				mList[i].mRight = NONE ;
			}
			for (auto &&i : range (0 ,1)) {
				mList[range_[i]].mLeft = NONE ;
				mList[range_[i]].mRight = range_[i + 1] ;
				mFirst = range_[i] ;
			}
			for (auto &&i : range (1 ,range_.length () - 1)) {
				mList[range_[i]].mLeft = range_[i - 1] ;
				mList[range_[i]].mRight = range_[i + 1] ;
			}
			for (auto &&i : range (range_.length () - 1 ,range_.length ())) {
				mList[range_[i]].mLeft = range_[i - 1] ;
				mList[range_[i]].mRight = NONE ;
				mLast = range_[i] ;
			}
		}

	private:
		VREF<INDEX> curr_next (CREF<INDEX> curr) leftvalue {
			if (curr == NONE)
				return mFirst ;
			return mList[curr].mRight ;
		}

		VREF<INDEX> curr_prev (CREF<INDEX> curr) leftvalue {
			if (curr == NONE)
				return mFirst ;
			return mList[curr].mLeft ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using List = typename U::LIST_HELP<ITEM ,SIZE ,ALWAYS>::List ;

namespace U {
template <class...>
trait ARRAYLIST_HELP ;

template <class ITEM ,class SIZE>
trait ARRAYLIST_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mIndex ;
	} ;

	class ArrayList {
	private:
		Allocator<NODE ,SIZE> mList ;
		Buffer<INDEX ,SIZE> mRange ;
		INDEX mFree ;

	public:
		implicit ArrayList () {
			mList = Allocator<NODE ,SIZE> (0) ;
			mRange = Buffer<INDEX ,SIZE> (0) ;
			clear () ;
		}

		explicit ArrayList (CREF<LENGTH> size_) {
			mList = Allocator<NODE ,SIZE> (size_) ;
			mRange = Buffer<INDEX ,SIZE> (size_) ;
			clear () ;
		}

		explicit ArrayList (CREF<std::initializer_list<ITEM>> that) {
			mList = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			mRange = Buffer<INDEX ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
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
			for (auto &&i : range (0 ,mRange.size ()))
				mRange[i] = NONE ;
			mFree = 0 ;
		}

		INDEX ibegin () const {
			for (auto &&i : range (0 ,mRange.size ())) {
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
			for (auto &&i : range (r1x ,mRange.size ())) {
				if (mRange[i] != NONE)
					return i ;
			}
			return NONE ;
		}

		ArrayIterator<ArrayList> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<ArrayList> (CRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<ArrayList> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<ArrayList> (CRef<ArrayList>::reference (thiz)) ;
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
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () const {
			auto &&thiz = *this ;
			using R1X = typename U::SORT_PROC_HELP<ArrayList ,ALWAYS>::SortProc ;
			Array<INDEX> ret = range () ;
			R1X::sort (thiz ,ret ,0 ,ret.length () - 1) ;
			return move (ret) ;
		}

		BOOL empty () const {
			return mList.length () == 0 ;
		}

		void add (CREF<ITEM> item) {
			add (move (item)) ;
		}

		inline VREF<ArrayList> operator<< (CREF<ITEM> item) {
			auto &&thiz = *this ;
			add (move (item)) ;
			return thiz ;
		}

		void add (RREF<ITEM> item) {
			auto rax = Box<ITEM>::make (move (item)) ;
			INDEX ix = mList.alloc () ;
			update_range (ix) ;
			mList[ix].mItem = move (rax) ;
			mList[ix].mIndex = next_free_one () ;
			mRange[mList[ix].mIndex] = ix ;
		}

		inline VREF<ArrayList> operator<< (RREF<ITEM> item) {
			auto &&thiz = *this ;
			add (move (item)) ;
			return thiz ;
		}

		INDEX insert () {
			auto rax = Box<ITEM>::make () ;
			INDEX ret = mList.alloc () ;
			update_range (ret) ;
			mList[ret].mItem = move (rax) ;
			mList[ret].mIndex = next_free_one () ;
			mRange[mList[ret].mIndex] = ret ;
			return move (ret) ;
		}

		void remove (CREF<INDEX> index) {
			mList.free (mRange[index]) ;
			mRange[index] = NONE ;
		}

		void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			if (index1 == index2)
				return ;
			swap (mRange[index1] ,mRange[index2]) ;
			mList[mRange[index1]].mIndex = index1 ;
			mList[mRange[index2]].mIndex = index2 ;
		}

		void esort (CREF<Array<INDEX>> range_) {
			assert (range_.length () == length ()) ;
			if (range_.length () < 2)
				return ;
			for (auto &&i : range (0 ,range_.length ())) {
				INDEX ix = mRange[range_[i]] ;
				mList[ix].mIndex = i ;
			}
			INDEX ix = 0 ;
			for (auto &&i : range (0 ,mList.size ())) {
				if ifnot (mList.used (i))
					continue ;
				mList[i].mIndex = ix ;
				mRange[mList[i].mIndex] = i ;
				ix++ ;
			}
			mFree = 0 ;
		}

		void remap () {
			if (mRange.size () == mList.length ())
				return ;
			mRange.resize (mList.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : range (0 ,mList.size ())) {
				if ifnot (mList.used (i))
					continue ;
				mList[i].mIndex = ix ;
				mRange[mList[i].mIndex] = i ;
				ix++ ;
			}
			mFree = 0 ;
		}

	private:
		INDEX next_free_one () {
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
			if (mRange.size () == mList.size ())
				return ;
			const auto r1x = mRange.size () ;
			assert (r1x < mList.size ()) ;
			mRange.resize (mList.size ()) ;
			for (auto &&i : range (r1x ,mRange.size ()))
				mRange[i] = NONE ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using ArrayList = typename U::ARRAYLIST_HELP<ITEM ,SIZE ,ALWAYS>::ArrayList ;

namespace U {
template <class...>
trait BITSET_HELP ;

template <class ITEM ,class SIZE>
trait BITSET_HELP<ITEM ,SIZE ,ALWAYS> {
	using RESERVE_SIZE = CONDITIONAL<ENUM_GT_ZERO<SIZE> ,ENUM_MOD<ENUM_ADD<SIZE ,RANK7> ,RANK8> ,SIZE> ;

	class Bit extend Proxy {

	} ;

	class BitSet {
	private:
		Buffer<BYTE ,RESERVE_SIZE> mSet ;
		LENGTH mWidth ;

	public:
		implicit BitSet () {
			mSet = Buffer<BYTE ,RESERVE_SIZE> (0) ;
			mWidth = max (SIZE::value ,ZERO) ;
			clear () ;
		}

		explicit BitSet (CREF<LENGTH> size_) {
			assert (size_ >= 0) ;
			mSet = Buffer<BYTE ,RESERVE_SIZE> (reserve_size (size_)) ;
			mWidth = size_ ;
			clear () ;
		}

		explicit BitSet (CREF<LENGTH> size_ ,CREF<Array<INDEX>> range_) {
			assert (size_ >= 0) ;
			mSet = Buffer<BYTE ,RESERVE_SIZE> (reserve_size (size_)) ;
			mWidth = size_ ;
			for (auto &&i : range_)
				add (i) ;
		}

		LENGTH size () const {
			if (mSet.size () == 0)
				return 0 ;
			return mWidth ;
		}

		LENGTH length () const {
			static constexpr auto M_LENGTH = PACK<INDEX[256]> ({
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
			for (auto &&i : range (0 ,mSet.size ()))
				ret += M_LENGTH.mP1[mSet[i]] ;
			if ifswitch (TRUE) {
				if (mWidth % 8 == 0)
					discard ;
				const auto r1x = BYTE (BYTE (0X01) << (mWidth % 8)) ;
				const auto r2x = BYTE (mSet[mWidth / 8] & ~BYTE (INDEX (r1x) - 1)) ;
				ret -= M_LENGTH.mP1[INDEX (r2x)] ;
			}
			return move (ret) ;
		}

		void clear () {
			fill (BYTE (0X00)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : range (0 ,size ())) {
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
			for (auto &&i : range (r1x ,size ())) {
				if (get (i))
					return i ;
			}
			return NONE ;
		}

		ArrayIterator<BitSet> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<BitSet> (CRef<BitSet>::reference (thiz)) ;
		}

		ArrayIterator<BitSet> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<BitSet> (CRef<BitSet>::reference (thiz)) ;
		}

		BOOL get (CREF<INDEX> index) const leftvalue {
			assert (ensure_index (index)) ;
			const auto r1x = BYTE (BYTE (0X01) << (index % 8)) ;
			const auto r2x = BYTE (mSet[index / 8] & r1x) ;
			return BOOL (r2x != 0) ;
		}

		inline BOOL operator[] (CREF<INDEX> index) const leftvalue {
			return get (index) ;
		}

		void set (CREF<INDEX> index ,CREF<BOOL> map_) {
			auto rax = TRUE ;
			if ifswitch (rax) {
				if ifnot (map_)
					discard ;
				add (index) ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				erase (index) ;
				rax = TRUE ;
			}
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		BOOL equal (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			INDEX ix = mSet.size () - 1 ;
			if (ix < 0)
				return TRUE ;
			for (auto &&i : range (0 ,ix)) {
				if (mSet[i] != that.mSet[i])
					return FALSE ;
			}
			const auto r1x = BYTE (mSet[ix] & (mWidth % 8 - 1)) ;
			const auto r2x = BYTE (that.mSet[ix] & (mWidth % 8 - 1)) ;
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
				return 0 ;
			for (auto &&i : range (0 ,ix)) {
				const auto r1x = operator_compr (mSet[i] ,that.mSet[i]) ;
				if (r1x != ZERO)
					return r1x ;
			}
			const auto r2x = BYTE (mWidth % 8 - 1) ;
			const auto r3x = BYTE (mSet[ix] & r2x) ;
			const auto r4x = BYTE (that.mSet[ix] & r2x) ;
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
			for (auto &&i : range (0 ,size ())) {
				const auto r1x = operator_hash (mSet[i]) ;
				ret = hashcode (ret ,r1x) ;
			}
			return move (ret) ;
		}

		void add (CREF<INDEX> index) {
			assert (ensure_index (index)) ;
			const auto r1x = BYTE (BYTE (0X01) << (index % 8)) ;
			mSet[index / 8] |= r1x ;
		}

		inline VREF<BitSet> operator<< (CREF<INDEX> item) {
			auto &&thiz = *this ;
			add (item) ;
			return thiz ;
		}

		void erase (CREF<INDEX> index) {
			assert (ensure_index (index)) ;
			const auto r1x = BYTE (BYTE (0X01) << (index % 8)) ;
			mSet[index / 8] &= BYTE (~r1x) ;
		}

		BitSet band (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : range (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] & that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator& (CREF<BitSet> that) const {
			return band (that) ;
		}

		void bandto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : range (0 ,mSet.size ()))
				mSet[i] &= that.mSet[i] ;
		}

		inline VREF<BitSet> operator&= (CREF<BitSet> that) {
			auto &&thiz = *this ;
			bandto (that) ;
			return thiz ;
		}

		BitSet bor (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : range (0 ,mSet.size ()))
				ret.mSet[i] = BYTE (mSet[i] | that.mSet[i]) ;
			return move (ret) ;
		}

		inline BitSet operator| (CREF<BitSet> that) const {
			return bor (that) ;
		}

		void borto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : range (0 ,mSet.size ()))
				mSet[i] |= that.mSet[i] ;
		}

		inline VREF<BitSet> operator|= (CREF<BitSet> that) {
			auto &&thiz = *this ;
			borto (that) ;
			return thiz ;
		}

		BitSet bxor (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : range (0 ,mSet.size ()))
				ret.mSet[i] = BYTE (mSet[i] ^ that.mSet[i]) ;
			return move (ret) ;
		}

		inline BitSet operator^ (CREF<BitSet> that) const {
			return bxor (that) ;
		}

		void bxorto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : range (0 ,mSet.size ()))
				mSet[i] ^= that.mSet[i] ;
		}

		inline VREF<BitSet> operator^= (CREF<BitSet> that) {
			auto &&thiz = *this ;
			bxorto (that) ;
			return thiz ;
		}

		BitSet bsub (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : range (0 ,mSet.size ()))
				ret.mSet[i] = BYTE (mSet[i] & ~that.mSet[i]) ;
			return move (ret) ;
		}

		inline BitSet operator- (CREF<BitSet> that) const {
			return bsub (that) ;
		}

		void bsubto (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : range (0 ,mSet.size ()))
				mSet[i] &= BYTE (~that.mSet[i]) ;
		}

		inline VREF<BitSet> operator-= (CREF<BitSet> that) {
			auto &&thiz = *this ;
			bsubto (that) ;
			return thiz ;
		}

		BitSet bnot () const {
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : range (0 ,mSet.size ()))
				ret.mSet[i] = BYTE (~mSet[i]) ;
			return move (ret) ;
		}

		inline BitSet operator~ () const {
			return bnot () ;
		}

		void fill (CREF<BYTE> item) {
			for (auto &&i : range (0 ,mSet.size ()))
				mSet[i] = item ;
		}

	private:
		imports LENGTH reserve_size (CREF<LENGTH> size_) {
			if (size_ <= 0)
				return size_ ;
			return (size_ + 7) / 8 ;
		}

		BOOL ensure_index (CREF<INDEX> index) const {
			return between (index ,0 ,mWidth) ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using BitSet = typename U::BITSET_HELP<ITEM ,SIZE ,ALWAYS>::BitSet ;

namespace U {
template <class...>
trait SET_HELP ;

template <class ITEM ,class SIZE>
trait SET_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mMap ;
		BOOL mRed ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;
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

		explicit Set (CREF<std::initializer_list<ITEM>> that) {
			mSet = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
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
			mRoot = NONE ;
		}

		INDEX ibegin () const {
			for (auto &&i : range (0 ,size ())) {
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
			for (auto &&i : range (r1x ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		ArrayIterator<Set> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Set> (CRef<Set>::reference (thiz)) ;
		}

		ArrayIterator<Set> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<Set> (CRef<Set>::reference (thiz)) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet[index].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX get (CREF<INDEX> index) const {
			if (index == NONE)
				return NONE ;
			return mSet[index].mMap ;
		}

		void set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet[index].mMap = map_ ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		Array<INDEX> range_sort () {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			compute_order (mRoot ,ret ,ix) ;
			return move (ret) ;
		}

		void add (CREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		inline VREF<Set> operator<< (CREF<ITEM> item) {
			auto &&thiz = *this ;
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
			auto &&thiz = *this ;
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

		INDEX head () const {
			INDEX ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				INDEX ix = mSet[ret].mLeft ;
				if (ix == NONE)
					break ;
				ret = ix ;
			}
			return move (ret) ;
		}

		INDEX tail () const {
			INDEX ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				INDEX ix = mSet[ret].mRight ;
				if (ix == NONE)
					break ;
				ret = ix ;
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
			curr_next (mSet[index].mUp) = r1x ;
			assert (r1x == NONE || curr_next (mSet[r1x].mUp) == r1x) ;
			curr_prev (r1x) = mSet[index].mUp ;
			if ifswitch (TRUE) {
				if (mSet[index].mRed)
					discard ;
				update_remove (r1x ,parent (index)) ;
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
			return get (find (item)) ;
		}

		void erase (CREF<ITEM> item) {
			INDEX ix = find (item) ;
			if (ix == NONE)
				return ;
			remove (ix) ;
		}

	private:
		void update_emplace (CREF<INDEX> curr) {
			INDEX ix = mRoot ;
			INDEX iy = NONE ;
			while (TRUE) {
				if (ix == NONE)
					break ;
				const auto r1x = operator_compr (mSet[curr].mItem.self ,mSet[ix].mItem.self) ;
				if (r1x == ZERO)
					break ;
				const auto r2x = invoke ([&] () {
					if (r1x < 0)
						return left_child (ix) ;
					return right_child (ix) ;
				}) ;
				iy = r2x ;
				ix = curr_next (iy) ;
			}
			assert (ix == NONE) ;
			mSet[curr].mUp = iy ;
			mSet[curr].mLeft = NONE ;
			mSet[curr].mRight = NONE ;
			curr_next (mSet[curr].mUp) = curr ;
			assert (curr_next (mSet[curr].mUp) == curr) ;
		}

		void update_insert (CREF<INDEX> curr) {
			INDEX ix = curr ;
			INDEX iy = parent (ix) ;
			while (TRUE) {
				if (iy == NONE)
					break ;
				if ifnot (mSet[iy].mRed)
					break ;
				INDEX iz = parent (iy) ;
				auto rax = TRUE ;
				if ifswitch (rax) {
					if (iy != mSet[iz].mLeft)
						discard ;
					update_insert_left (ix) ;
					ix = mTop ;
					rax = TRUE ;
				}
				if ifswitch (rax) {
					if (iy != mSet[iz].mRight)
						discard ;
					update_insert_right (ix) ;
					ix = mTop ;
					rax = TRUE ;
				}
				iy = parent (ix) ;
			}
			mSet[mRoot].mRed = FALSE ;
		}

		void update_insert_left (CREF<INDEX> curr) {
			INDEX ix = parent (curr) ;
			INDEX iy = parent (ix) ;
			INDEX iz = mSet[iy].mRight ;
			auto rax = TRUE ;
			if ifswitch (rax) {
				if (iz == NONE)
					discard ;
				if ifnot (mSet[iz].mRed)
					discard ;
				mSet[iz].mRed = FALSE ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				mTop = iy ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				if (curr != mSet[ix].mRight)
					discard ;
				rotate_left (mSet[iy].mLeft) ;
				mSet[curr].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_right (iy) ;
				mTop = ix ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				if (curr != mSet[ix].mLeft)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_right (iy) ;
				mTop = curr ;
				rax = TRUE ;
			}
		}

		void update_insert_right (CREF<INDEX> curr) {
			INDEX ix = parent (curr) ;
			INDEX iy = parent (ix) ;
			INDEX iz = mSet[iy].mLeft ;
			auto rax = TRUE ;
			if ifswitch (rax) {
				if (iz == NONE)
					discard ;
				if ifnot (mSet[iz].mRed)
					discard ;
				mSet[iz].mRed = FALSE ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				mTop = iy ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				if (curr != mSet[ix].mLeft)
					discard ;
				rotate_right (mSet[iy].mRight) ;
				mSet[curr].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_left (iy) ;
				mTop = ix ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				if (curr != mSet[ix].mRight)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_left (iy) ;
				mTop = curr ;
				rax = TRUE ;
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
				auto rax = TRUE ;
				if ifswitch (rax) {
					if (ix != mSet[iy].mLeft)
						discard ;
					update_remove_left (iy) ;
					ix = mTop ;
					rax = TRUE ;
				}
				if ifswitch (rax) {
					if (ix != mSet[iy].mRight)
						discard ;
					update_remove_right (iy) ;
					ix = mTop ;
					rax = TRUE ;
				}
				iy = parent (ix) ;
			}
			if (ix == NONE)
				return ;
			mSet[ix].mRed = FALSE ;
		}

		void update_remove_left (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mRight ;
			const auto r1x = mSet[ix].mUp ;
			if ifswitch (TRUE) {
				if ifnot (mSet[ix].mRed)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[curr].mRed = TRUE ;
				rotate_left (curr) ;
				ix = curr_next (r1x) ;
			}
			INDEX jx = mSet[ix].mLeft ;
			INDEX jy = mSet[ix].mRight ;
			const auto r3x = BOOL (jx != NONE && mSet[jx].mRed) ;
			const auto r4x = BOOL (jy != NONE && mSet[jy].mRed) ;
			auto rax = TRUE ;
			if ifswitch (rax) {
				if (r3x)
					discard ;
				if (r4x)
					discard ;
				mSet[ix].mRed = TRUE ;
				mTop = curr ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				if (r4x)
					discard ;
				mSet[jx].mRed = FALSE ;
				mSet[ix].mRed = TRUE ;
				rotate_right (ix) ;
				ix = curr_next (r1x) ;
				jx = mSet[ix].mLeft ;
				jy = mSet[ix].mRight ;
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_left (curr) ;
				mTop = mRoot ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_left (curr) ;
				mTop = mRoot ;
				rax = TRUE ;
			}
		}

		void update_remove_right (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mLeft ;
			const auto r1x = mSet[ix].mUp ;
			if ifswitch (TRUE) {
				if ifnot (mSet[ix].mRed)
					discard ;
				mSet[ix].mRed = FALSE ;
				mSet[curr].mRed = TRUE ;
				rotate_right (curr) ;
				ix = curr_next (r1x) ;
			}
			INDEX jx = mSet[ix].mRight ;
			INDEX jy = mSet[ix].mLeft ;
			const auto r3x = BOOL (jx != NONE && mSet[jx].mRed) ;
			const auto r4x = BOOL (jy != NONE && mSet[jy].mRed) ;
			auto rax = TRUE ;
			if ifswitch (rax) {
				if (r3x)
					discard ;
				if (r4x)
					discard ;
				mSet[ix].mRed = TRUE ;
				mTop = curr ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				if (r4x)
					discard ;
				mSet[jx].mRed = FALSE ;
				mSet[ix].mRed = TRUE ;
				rotate_left (ix) ;
				ix = curr_next (r1x) ;
				jx = mSet[ix].mRight ;
				jy = mSet[ix].mLeft ;
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_right (curr) ;
				mTop = mRoot ;
				rax = TRUE ;
			}
			if ifswitch (rax) {
				mSet[ix].mRed = mSet[curr].mRed ;
				mSet[curr].mRed = FALSE ;
				mSet[jy].mRed = FALSE ;
				rotate_right (curr) ;
				mTop = mRoot ;
				rax = TRUE ;
			}
		}

		void rotate_left (CREF<INDEX> curr) {
			INDEX iy = curr ;
			INDEX ix = mSet[curr].mRight ;
			mSet[curr].mRight = mSet[ix].mLeft ;
			curr_prev (mSet[ix].mLeft) = right_child (curr) ;
			assert (mSet[ix].mLeft == NONE || curr_next (mSet[mSet[ix].mLeft].mUp) == mSet[ix].mLeft) ;
			mSet[ix].mLeft = curr ;
			mSet[ix].mUp = mSet[curr].mUp ;
			mSet[curr].mUp = left_child (ix) ;
			curr_next (mSet[ix].mUp) = ix ;
			dynamic_watch (iy) ;
			assert (curr_next (mSet[ix].mUp) == ix) ;
			assert (curr_next (mSet[iy].mUp) == iy) ;
			assert (curr_next (mSet[curr].mUp) == curr) ;
		}

		void rotate_right (CREF<INDEX> curr) {
			INDEX iy = curr ;
			INDEX ix = mSet[curr].mLeft ;
			mSet[curr].mLeft = mSet[ix].mRight ;
			curr_prev (mSet[ix].mRight) = left_child (curr) ;
			assert (mSet[ix].mRight == NONE || curr_next (mSet[mSet[ix].mRight].mUp) == mSet[ix].mRight) ;
			mSet[ix].mRight = curr ;
			mSet[ix].mUp = mSet[curr].mUp ;
			mSet[curr].mUp = right_child (ix) ;
			curr_next (mSet[ix].mUp) = ix ;
			dynamic_watch (iy) ;
			assert (curr_next (mSet[ix].mUp) == ix) ;
			assert (curr_next (mSet[iy].mUp) == iy) ;
			assert (curr_next (mSet[curr].mUp) == curr) ;
		}

		INDEX parent (CREF<INDEX> curr) {
			if (mSet[curr].mUp == NONE)
				return NONE ;
			return mSet[curr].mUp / 2 ;
		}

		INDEX left_child (CREF<INDEX> curr) {
			return curr * 2 ;
		}

		INDEX right_child (CREF<INDEX> curr) {
			return curr * 2 + 1 ;
		}

		VREF<INDEX> curr_next (CREF<INDEX> curr) leftvalue {
			if (curr == NONE)
				return mRoot ;
			INDEX ix = curr / 2 ;
			if (ix * 2 == curr)
				return mSet[ix].mLeft ;
			return mSet[ix].mRight ;
		}

		VREF<INDEX> curr_prev (CREF<INDEX> curr) leftvalue {
			if (curr == NONE)
				return mTop ;
			return mSet[curr].mUp ;
		}

		INDEX find_successor (CREF<INDEX> index) const {
			INDEX ret = mSet[index].mRight ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				INDEX ix = mSet[ret].mLeft ;
				if (ix == NONE)
					break ;
				ret = ix ;
			}
			return move (ret) ;
		}

		void eswap (CREF<INDEX> index1 ,CREF<INDEX> index2) {
			if (index1 == index2)
				return ;
			const auto r1x = mSet[index1].mUp ;
			const auto r2x = mSet[index1].mLeft ;
			const auto r3x = mSet[index1].mRight ;
			const auto r4x = mSet[index2].mUp ;
			const auto r5x = mSet[index2].mLeft ;
			const auto r6x = mSet[index2].mRight ;
			curr_next (r1x) = index2 ;
			curr_prev (r2x) = index2 ;
			curr_prev (r3x) = index2 ;
			curr_next (r4x) = index1 ;
			curr_prev (r5x) = index1 ;
			curr_prev (r6x) = index1 ;
			swap (mSet[index1].mRed ,mSet[index2].mRed) ;
			swap (mSet[index1].mUp ,mSet[index2].mUp) ;
			swap (mSet[index1].mLeft ,mSet[index2].mLeft) ;
			swap (mSet[index1].mRight ,mSet[index2].mRight) ;
			assert (curr_next (mSet[index1].mUp) == index1) ;
			assert (curr_next (mSet[index2].mUp) == index2) ;
		}

		void compute_order (CREF<INDEX> curr ,VREF<Array<INDEX>> range_ ,VREF<INDEX> write_) {
			if (curr == NONE)
				return ;
			assert (curr_next (mSet[curr].mUp) == curr) ;
			INDEX ix = write_ ;
			compute_order (mSet[curr].mLeft ,range_ ,ix) ;
			range_[ix] = curr ;
			ix++ ;
			compute_order (mSet[curr].mRight ,range_ ,ix) ;
			write_ = ix ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using Set = typename U::SET_HELP<ITEM ,SIZE ,ALWAYS>::Set ;

namespace U {
template <class...>
trait HASHSET_HELP ;

template <class ITEM ,class SIZE>
trait HASHSET_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		Box<ITEM> mItem ;
		INDEX mMap ;
		FLAG mHash ;
		INDEX mLeft ;
		INDEX mRight ;
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

		explicit HashSet (CREF<std::initializer_list<ITEM>> that) {
			mSet = Allocator<NODE ,SIZE> (LENGTH (that.size ())) ;
			clear () ;
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
			for (auto &&i : range (0 ,mRange.size ()))
				mRange[i] = NONE ;
		}

		INDEX ibegin () const {
			for (auto &&i : range (0 ,size ())) {
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
			for (auto &&i : range (r1x ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		ArrayIterator<HashSet> begin () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<HashSet> (CRef<HashSet>::reference (thiz)) ;
		}

		ArrayIterator<HashSet> end () const leftvalue {
			auto &&thiz = *this ;
			return ArrayIterator<HashSet> (CRef<HashSet>::reference (thiz)) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet[index].mItem.self ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX get (CREF<INDEX> index) const {
			if (index == NONE)
				return NONE ;
			return mSet[index].mMap ;
		}

		void set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet[index].mMap = map_ ;
		}

		Array<INDEX> range () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = ibegin () ;
			INDEX iy = iend () ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix == iy)
					break ;
				ret[jx] = ix ;
				jx++ ;
				ix = inext (ix) ;
			}
			return move (ret) ;
		}

		void add (CREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		inline VREF<HashSet> operator<< (CREF<ITEM> item) {
			auto &&thiz = *this ;
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
			auto &&thiz = *this ;
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
			curr_next (mSet[index].mLeft) = mSet[index].mRight ;
			curr_prev (mSet[index].mRight) = mSet[index].mLeft ;
			mSet.free (index) ;
		}

		INDEX find (CREF<ITEM> item) const {
			INDEX ret = NONE ;
			if ifswitch (TRUE) {
				if (mSet.size () == 0)
					discard ;
				const auto r1x = operator_hash (item) ;
				assert (r1x >= 0) ;
				ret = mRange[r1x % mRange.size ()] ;
				while (TRUE) {
					if (ret == NONE)
						break ;
					if (mSet[ret].mHash == r1x)
						if (operator_equal (item ,mSet[ret].mItem.self))
							break ;
					ret = mSet[ret].mRight ;
				}
			}
			return move (ret) ;
		}

		INDEX map (CREF<ITEM> item) const {
			return get (find (item)) ;
		}

		void erase (CREF<ITEM> item) {
			INDEX ix = find (item) ;
			if (ix == NONE)
				return ;
			remove (ix) ;
		}

	private:
		void update_range (CREF<INDEX> curr) {
			if (mRange.size () == mSet.size ())
				return ;
			const auto r1x = mRange.size () ;
			assert (r1x < mSet.size ()) ;
			mRange.resize (mSet.size ()) ;
			for (auto &&i : range (0 ,mRange.size ()))
				mRange[i] = NONE ;
			for (auto &&i : range (0 ,mSet.size ())) {
				if (i == curr)
					continue ;
				if ifnot (mSet.used (i))
					continue ;
				update_emplace (i) ;
			}
		}

		void update_emplace (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mHash % mRange.size () ;
			mSet[curr].mLeft = NONE ;
			mSet[curr].mRight = mRange[ix] ;
			mSet[mRange[ix]].mLeft = curr ;
			mRange[ix] = curr ;
		}

		VREF<INDEX> curr_next (CREF<INDEX> curr) leftvalue {
			INDEX ix = mSet[curr].mHash % mRange.size () ;
			if (curr == NONE)
				return mRange[ix] ;
			return mSet[curr].mRight ;
		}

		VREF<INDEX> curr_prev (CREF<INDEX> curr) leftvalue {
			INDEX ix = mSet[curr].mHash % mRange.size () ;
			if (curr == NONE)
				return mRange[ix] ;
			return mSet[curr].mLeft ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE = VARBUFFER_SIZE>
using HashSet = typename U::HASHSET_HELP<ITEM ,SIZE ,ALWAYS>::HashSet ;


} ;