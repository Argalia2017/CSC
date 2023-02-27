#pragma once

#ifndef __CSC_ARRAY__
#define __CSC_ARRAY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"

namespace CSC {
template <class...>
trait SPANITERATOR_HELP ;

template <class UNIT ,class SIDE>
trait SPANITERATOR_HELP<UNIT ,SIDE ,REQUIRE<IS_SAME<SIDE ,REGISTER>>> {
	class SpanIterator {
	protected:
		LENGTH mRank ;
		FLAG mBegin ;
		FLAG mEnd ;

	public:
		implicit SpanIterator () = delete ;

		implicit SpanIterator (CREF<csc_initializer_t<UNIT>> that) {
			const auto r1x = FUNCTION_internel_span () ;
			const auto r2x = r1x (that) ;
			mRank = (r2x.mEnd - r2x.mBegin) / r2x.mStep ;
			mBegin = r2x.mBegin ;
			mEnd = r2x.mEnd ;
		}

		SpanIterator begin () const {
			return thiz ;
		}

		SpanIterator end () const {
			return thiz ;
		}

		LENGTH rank () const {
			return mRank ;
		}

		BOOL bad () const {
			return mBegin == mEnd ;
		}

		inline BOOL operator== (CREF<SpanIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<SpanIterator>) const {
			return ifnot (bad ()) ;
		}

		VREF<UNIT> peek () leftvalue {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (mBegin))) ;
		}

		inline VREF<UNIT> operator* () leftvalue {
			return peek () ;
		}

		void next () {
			mBegin += SIZE_OF<UNIT>::expr ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

template <class UNIT ,class SIDE>
trait SPANITERATOR_HELP<UNIT ,SIDE ,REQUIRE<IS_VOID<SIDE>>> {
	class SpanIterator {
	protected:
		LENGTH mRank ;
		FLAG mBegin ;
		FLAG mEnd ;

	public:
		implicit SpanIterator () = delete ;

		template <class ARG1 ,class = REQUIRE<ENUM_ALL<ENUM_NOT<IS_EXTEND<SpanIterator ,ARG1>> ,IS_SAME<ARG1 ,typename ARG1::HELP::Capture>>>>
		implicit SpanIterator (RREF<ARG1> that) {
			using R1X = typename ARG1::HELP::PARAMS ;
			using R3X = TYPE_REPEAT<UNIT ,COUNT_OF<R1X>> ;
			require (IS_SAME<R1X ,R3X>) ;
			mRank = COUNT_OF<R1X>::expr ;
			mBegin = address (that) ;
			mEnd = mBegin + mRank * SIZE_OF<FLAG>::expr ;
		}

		SpanIterator begin () const {
			return thiz ;
		}

		SpanIterator end () const {
			return thiz ;
		}

		LENGTH rank () const {
			return mRank ;
		}

		BOOL bad () const {
			return mBegin == mEnd ;
		}

		inline BOOL operator== (CREF<SpanIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<SpanIterator>) const {
			return ifnot (bad ()) ;
		}

		CREF<UNIT> peek () const leftvalue {
			auto rax = ZERO ;
			unsafe_sync (unsafe_deptr (rax) ,unsafe_pointer (mBegin)) ;
			unsafe_launder (rax) ;
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<UNIT>>::expr] (unsafe_pointer (rax))) ;
		}

		inline CREF<UNIT> operator* () const leftvalue {
			return peek () ;
		}

		void next () {
			mBegin += SIZE_OF<FLAG>::expr ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

template <class UNIT ,class SIDE = void>
using SpanIterator = typename SPANITERATOR_HELP<UNIT ,SIDE ,ALWAYS>::SpanIterator ;

template <class...>
trait ARRAYITERATOR_HELP ;

template <class UNIT ,class ITEM ,class COND>
trait ARRAYITERATOR_HELP<UNIT ,ITEM ,COND ,REQUIRE<ENUM_ALL<COND ,ENUM_NOT<IS_VOID<ITEM>>>>> {
	class ArrayIterator {
	protected:
		VRef<UNIT> mArray ;
		INDEX mBegin ;
		INDEX mEnd ;

	public:
		implicit ArrayIterator () = delete ;

		explicit ArrayIterator (RREF<VRef<UNIT>> array_) {
			mArray = move (array_) ;
			mBegin = mArray->ibegin () ;
			mEnd = mArray->iend () ;
		}

		LENGTH rank () const {
			return mArray->length () ;
		}

		BOOL bad () const {
			return mBegin == mEnd ;
		}

		inline BOOL operator== (CREF<ArrayIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<ArrayIterator>) const {
			return ifnot (bad ()) ;
		}

		VREF<ITEM> peek () leftvalue {
			return mArray->at (mBegin) ;
		}

		inline VREF<ITEM> operator* () leftvalue {
			return peek () ;
		}

		void next () {
			mBegin = mArray->inext (mBegin) ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

template <class UNIT ,class ITEM ,class COND>
trait ARRAYITERATOR_HELP<UNIT ,ITEM ,COND ,REQUIRE<ENUM_ALL<ENUM_NOT<COND> ,ENUM_NOT<IS_VOID<ITEM>>>>> {
	class ArrayIterator {
	protected:
		CRef<UNIT> mArray ;
		INDEX mBegin ;
		INDEX mEnd ;

	public:
		implicit ArrayIterator () = delete ;

		explicit ArrayIterator (RREF<CRef<UNIT>> array_) {
			mArray = move (array_) ;
			mBegin = mArray->ibegin () ;
			mEnd = mArray->iend () ;
		}

		LENGTH rank () const {
			return mArray->length () ;
		}

		BOOL bad () const {
			return mBegin == mEnd ;
		}

		inline BOOL operator== (CREF<ArrayIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<ArrayIterator>) const {
			return ifnot (bad ()) ;
		}

		CREF<ITEM> peek () const leftvalue {
			return mArray->at (mBegin) ;
		}

		inline CREF<ITEM> operator* () const leftvalue {
			return peek () ;
		}

		void next () {
			mBegin = mArray->inext (mBegin) ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

template <class UNIT ,class ITEM ,class COND>
trait ARRAYITERATOR_HELP<UNIT ,ITEM ,COND ,REQUIRE<IS_VOID<ITEM>>> {
	class ArrayIterator {
	protected:
		CRef<UNIT> mArray ;
		INDEX mBegin ;
		INDEX mEnd ;

	public:
		implicit ArrayIterator () = delete ;

		explicit ArrayIterator (RREF<CRef<UNIT>> array_) {
			mArray = move (array_) ;
			mBegin = mArray->ibegin () ;
			mEnd = mArray->iend () ;
		}

		ArrayIterator begin () const {
			return thiz ;
		}

		ArrayIterator end () const {
			return thiz ;
		}

		LENGTH rank () const {
			return mArray->length () ;
		}

		BOOL bad () const {
			return mBegin == mEnd ;
		}

		inline BOOL operator== (CREF<ArrayIterator>) const {
			return bad () ;
		}

		inline BOOL operator!= (CREF<ArrayIterator>) const {
			return ifnot (bad ()) ;
		}

		CREF<INDEX> peek () const leftvalue {
			return mBegin ;
		}

		inline CREF<INDEX> operator* () const leftvalue {
			return peek () ;
		}

		void next () {
			mBegin = mArray->inext (mBegin) ;
		}

		inline void operator++ () {
			next () ;
		}
	} ;
} ;

template <class UNIT ,class ITEM = void>
using ArrayIterator = typename ARRAYITERATOR_HELP<REMOVE_REF<UNIT> ,ITEM ,IS_VARIABLE<UNIT> ,ALWAYS>::ArrayIterator ;

template <class...>
trait ARRAY_HELP ;

template <class ITEM ,class SIZE>
trait ARRAY_HELP<ITEM ,SIZE ,ALWAYS> {
	class Array {
	protected:
		Buffer<ITEM ,SIZE> mArray ;

	public:
		implicit Array () = default ;

		explicit Array (CREF<SizeProxy> size_) {
			mArray = Buffer<ITEM ,SIZE> (size_) ;
		}

		explicit Array (CREF<csc_initializer_t<ITEM>> that)
			:Array (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit Array (CREF<SpanIterator<ITEM>> that) {
			mArray = Buffer<ITEM ,SIZE> (that.rank ()) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mArray[ix] = move (i) ;
				ix++ ;
			}
		}

		explicit Array (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mArray = Buffer<ITEM ,SIZE> (that.rank ()) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mArray[ix] = move (i) ;
				ix++ ;
			}
		}

		LENGTH size () const {
			return mArray.size () ;
		}

		LENGTH length () const {
			return size () ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) leftvalue {
			return RegBuffer<ITEM>::from (mArray ,0 ,size () ,move (unnamed)) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return RegBuffer<ITEM>::from (mArray ,0 ,length () ,move (unnamed)) ;
		}

		void fill (CREF<ITEM> item) {
			BufferProc<ITEM>::buf_fill (mArray ,item ,0 ,size ()) ;
		}

		ArrayIterator<VREF<Array> ,ITEM> begin () leftvalue {
			return ArrayIterator<VREF<Array> ,ITEM> (VRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<VREF<Array> ,ITEM> end () leftvalue {
			return ArrayIterator<VREF<Array> ,ITEM> (VRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<CREF<Array> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<Array> ,ITEM> (CRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<CREF<Array> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<Array> ,ITEM> (CRef<Array>::reference (thiz)) ;
		}

		ArrayIterator<Array> iter () const leftvalue {
			return ArrayIterator<Array> (CRef<Array>::reference (thiz)) ;
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

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		BOOL equal (CREF<Array> that) const {
			if (size () != that.size ())
				return FALSE ;
			return BufferProc<ITEM>::buf_equal (mArray ,that.mArray ,0 ,size ()) ;
		}

		inline BOOL operator== (CREF<Array> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Array> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<Array> that) const {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			const auto r3x = vmin (r1x ,r2x) ;
			const auto r4x = BufferProc<ITEM>::buf_compr (mArray ,that.mArray ,0 ,r3x) ;
			if (r4x != ZERO)
				return r4x ;
			return operator_compr (r1x ,r2x) ;
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
			return BufferProc<ITEM>::buf_hash (mArray ,0 ,size ()) ;
		}
	} ;

	class IterArray extend Proxy {
	public:
		template <class ARG1>
		imports Array make (CREF<ARG1> iterator) {
			Array ret = Array (iterator.begin ().rank ()) ;
			INDEX ix = 0 ;
			for (auto &&i : iterator) {
				ret[ix] = ITEM (i) ;
				ix++ ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Array = typename ARRAY_HELP<ITEM ,SIZE ,ALWAYS>::Array ;
template <class ITEM ,class SIZE = VARIABLE>
using IterArray = typename ARRAY_HELP<ITEM ,SIZE ,ALWAYS>::IterArray ;

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

template <class...>
trait STRING_TEXTWRITER_HELP ;

template <class ITEM ,class SIZE>
trait STRING_HELP<ITEM ,SIZE ,REQUIRE<IS_TEXT<ITEM>>> {
	using RESERVE_SIZE = CONDITIONAL<ENUM_COMPR_GTEQ<SIZE ,ENUM_ZERO> ,ENUM_INC<SIZE> ,SIZE> ;
	using BUFFER_SSIZE = ENUMAS<VAL ,8191> ;

	class String {
	protected:
		Buffer<ITEM ,RESERVE_SIZE> mString ;

	public:
		implicit String () = default ;

		implicit String (CREF<Slice<ITEM>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (that.size ())) ;
			clear () ;
			cover_with (that) ;
		}

		explicit String (CREF<SizeProxy> size_) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (size_)) ;
			clear () ;
		}

		explicit String (CREF<csc_initializer_t<ITEM>> that)
			:String (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit String (CREF<SpanIterator<ITEM>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (that.rank ())) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mString[ix] = move (i) ;
				ix++ ;
			}
			trunc (ix) ;
		}

		explicit String (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mString = Buffer<ITEM ,RESERVE_SIZE> (reserve_size (that.rank ())) ;
			INDEX ix = 0 ;
			for (auto &&i : that) {
				mString[ix] = move (i) ;
				ix++ ;
			}
			trunc (ix) ;
		}

		imports CREF<String> zero () {
			return memorize ([&] () {
				String ret ;
				ret.mString = Buffer<ITEM ,RESERVE_SIZE> (1) ;
				ret.clear () ;
				return move (ret) ;
			}) ;
		}

		LENGTH size () const {
			if (mString.size () == 0)
				return 0 ;
			return mString.size () - 1 ;
		}

		LENGTH length () const {
			if (mString.size () == 0)
				return 0 ;
			for (auto &&i : CSC::iter (0 ,size ())) {
				if (operator_equal (mString[i] ,ITEM (0)))
					return i ;
			}
			return size () ;
		}

		VREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) leftvalue {
			return RegBuffer<ITEM>::from (mString ,0 ,size () ,move (unnamed)) ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return RegBuffer<ITEM>::from (mString ,0 ,length () ,move (unnamed)) ;
		}

		void clear () {
			trunc (0) ;
			trunc (size ()) ;
		}

		void fill (CREF<ITEM> item) {
			BufferProc<ITEM>::buf_fill (mString ,item ,0 ,size ()) ;
		}

		ArrayIterator<VREF<String> ,ITEM> begin () leftvalue {
			return ArrayIterator<VREF<String> ,ITEM> (VRef<String>::reference (thiz)) ;
		}

		ArrayIterator<VREF<String> ,ITEM> end () leftvalue {
			return ArrayIterator<VREF<String> ,ITEM> (VRef<String>::reference (thiz)) ;
		}

		ArrayIterator<CREF<String> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<String> ,ITEM> (CRef<String>::reference (thiz)) ;
		}

		ArrayIterator<CREF<String> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<String> ,ITEM> (CRef<String>::reference (thiz)) ;
		}

		ArrayIterator<String> iter () const leftvalue {
			return ArrayIterator<String> (CRef<String>::reference (thiz)) ;
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

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			return mString[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mString[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		BOOL empty () const {
			if (mString.size () == 0)
				return TRUE ;
			if ifnot (operator_equal (mString[0] ,ITEM (0)))
				return FALSE ;
			return TRUE ;
		}

		BOOL equal (CREF<String> that) const {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			if (r1x != r2x)
				return FALSE ;
			return BufferProc<ITEM>::buf_equal (mString ,that.mString ,0 ,r1x) ;
		}

		inline BOOL operator== (CREF<String> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<String> that) const {
			return ifnot (equal (that)) ;
		}

		BOOL equal (CREF<Slice<ITEM>> that) const {
			const auto r1x = length () ;
			const auto r2x = that.size () ;
			if (r1x != r2x)
				return FALSE ;
			for (auto &&i : CSC::iter (0 ,r1x)) {
				if (mString[i] != that[i])
					return FALSE ;
			}
			return TRUE ;
		}

		inline BOOL operator== (CREF<Slice<ITEM>> that) const {
			return equal (that) ;
		}

		inline BOOL operator!= (CREF<Slice<ITEM>> that) const {
			return ifnot (equal (that)) ;
		}

		FLAG compr (CREF<String> that) const {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			const auto r3x = vmin (r1x ,r2x) ;
			const auto r4x = BufferProc<ITEM>::buf_compr (mString ,that.mString ,0 ,r3x) ;
			if (r4x != ZERO)
				return r4x ;
			return operator_compr (r1x ,r2x) ;
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
			const auto r1x = length () ;
			return BufferProc<ITEM>::buf_hash (mString ,0 ,r1x) ;
		}

		void cover_with (CREF<String> that) {
			const auto r1x = vmin (size () ,that.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : CSC::iter (0 ,r1x)) {
				mString[ix] = that[i] ;
				ix++ ;
			}
			trunc (ix) ;
		}

		inline void operator-= (CREF<String> that) {
			return cover_with (that) ;
		}

		void cover_with (CREF<Slice<ITEM>> that) {
			const auto r1x = vmin (size () ,that.size ()) ;
			INDEX ix = 0 ;
			for (auto &&i : CSC::iter (0 ,r1x)) {
				mString[ix] = that[i] ;
				ix++ ;
			}
			trunc (ix) ;
		}

		inline void operator-= (CREF<Slice<ITEM>> that) {
			return cover_with (that) ;
		}

		String concat (CREF<String> that) const {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			String ret = String (r1x + r2x) ;
			BufferProc<ITEM>::buf_copy (ret.mString ,mString ,0 ,r1x) ;
			INDEX ix = r1x ;
			for (auto &&i : CSC::iter (0 ,r2x)) {
				ret.mString[ix] = that.mString[i] ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		inline String operator+ (CREF<String> that) const {
			return concat (that) ;
		}

		String concat (CREF<Slice<ITEM>> that) const {
			const auto r1x = length () ;
			const auto r2x = that.size () ;
			String ret = String (r1x + r2x) ;
			BufferProc<ITEM>::buf_copy (ret.mString ,mString ,0 ,r1x) ;
			INDEX ix = r1x ;
			for (auto &&i : CSC::iter (0 ,r2x)) {
				ret.mString[ix] = that[i] ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		inline String operator+ (CREF<Slice<ITEM>> that) const {
			return concat (that) ;
		}

		void concat_with (CREF<String> that) {
			const auto r1x = length () ;
			const auto r2x = that.length () ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x + r2x >= size ())
					discard ;
				INDEX ix = r1x ;
				for (auto &&i : CSC::iter (0 ,r2x)) {
					mString[ix] = that.mString[i] ;
					ix++ ;
				}
				trunc (ix) ;
			}
			if ifswitch (act) {
				thiz = concat (that) ;
			}
		}

		inline void operator+= (CREF<String> that) {
			concat_with (that) ;
		}

		void concat_with (CREF<Slice<ITEM>> that) {
			const auto r1x = length () ;
			const auto r2x = that.size () ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x + r2x >= size ())
					discard ;
				INDEX ix = r1x ;
				for (auto &&i : CSC::iter (0 ,r2x)) {
					mString[ix] = that[i] ;
					ix++ ;
				}
				trunc (ix) ;
			}
			if ifswitch (act) {
				thiz = concat (that) ;
			}
		}

		inline void operator+= (CREF<Slice<ITEM>> that) {
			concat_with (that) ;
		}

		String segment (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
			assert (vbetween (begin_ ,0 ,size ())) ;
			assert (vbetween (end_ ,0 ,size () + 1)) ;
			const auto r1x = end_ - begin_ ;
			assert (r1x >= 0) ;
			String ret = String (r1x) ;
			INDEX ix = 0 ;
			for (auto &&i : CSC::iter (begin_ ,end_)) {
				ret.mString[ix] = mString[i] ;
				ix++ ;
			}
			ret.trunc (ix) ;
			return move (ret) ;
		}

		void trunc (CREF<INDEX> index) {
			INDEX ix = vmax (index ,0) ;
			if (ix >= mString.size ())
				return ;
			mString[ix] = ITEM (0) ;
		}

	private:
		LENGTH reserve_size (CREF<LENGTH> size_) const {
			if (size_ < 0)
				return size_ ;
			return size_ + 1 ;
		}
	} ;

	class PrintString extend Proxy {
	public:
		template <class...ARG1>
		imports String make (CREF<ARG1>...obj) {
			using R1X = typename DEPENDENT<STRING_TEXTWRITER_HELP<ITEM ,ALWAYS> ,TYPEAS<ARG1...>>::TextWriter ;
			static constexpr auto EOS = DEPENDENT<STRING_TEXTWRITER_HELP<ITEM ,ALWAYS> ,TYPEAS<ARG1...>>::EOS ;
			String ret = String (BUFFER_SSIZE::expr) ;
			auto rax = R1X (ret.raw ().borrow ()) ;
			rax.prints (obj...) ;
			rax << EOS ;
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using String = typename STRING_HELP<ITEM ,SIZE ,ALWAYS>::String ;
template <class ITEM ,class SIZE = VARIABLE>
using PrintString = typename STRING_HELP<ITEM ,SIZE ,ALWAYS>::PrintString ;

template <class...>
trait DEQUE_HELP ;

template <class...>
trait DEQUE_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait DEQUE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		ITEM mItem ;
	} ;

	class Deque {
	protected:
		Buffer<NODE ,SIZE> mDeque ;
		INDEX mRead ;
		INDEX mWrite ;

	public:
		implicit Deque () noexcept {
			mRead = 0 ;
			mWrite = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait DEQUE_HELP<ITEM ,SIZE ,ALWAYS> {
	using NODE = typename DEQUE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename DEQUE_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Deque ;

	class Deque extend SUPER {
	protected:
		using SUPER::mDeque ;
		using SUPER::mRead ;
		using SUPER::mWrite ;

	public:
		implicit Deque () = default ;

		explicit Deque (CREF<SizeProxy> size_) {
			mDeque = Buffer<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit Deque (CREF<csc_initializer_t<ITEM>> that)
			:Deque (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit Deque (CREF<SpanIterator<ITEM>> that) {
			mDeque = Buffer<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		explicit Deque (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mDeque = Buffer<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			return mDeque.size () ;
		}

		LENGTH length () const {
			if (mDeque.size () == 0)
				return 0 ;
			return mWrite - mRead ;
		}

		void clear () {
			mRead = 0 ;
			mWrite = 0 ;
		}

		ArrayIterator<VREF<Deque> ,ITEM> begin () leftvalue {
			return ArrayIterator<VREF<Deque> ,ITEM> (VRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<VREF<Deque> ,ITEM> end () leftvalue {
			return ArrayIterator<VREF<Deque> ,ITEM> (VRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<CREF<Deque> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<Deque> ,ITEM> (CRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<CREF<Deque> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<Deque> ,ITEM> (CRef<Deque>::reference (thiz)) ;
		}

		ArrayIterator<Deque> iter () const leftvalue {
			return ArrayIterator<Deque> (CRef<Deque>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			if (mDeque.size () == 0)
				return NONE ;
			return 0 ;
		}

		INDEX iend () const {
			if (mDeque.size () == 0)
				return NONE ;
			return mWrite - mRead ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (vbetween (index ,0 ,mWrite - mRead)) ;
			assert (mDeque.size () > 0) ;
			INDEX ix = (index + mRead) % mDeque.size () ;
			return mDeque[ix].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,mWrite - mRead)) ;
			assert (mDeque.size () > 0) ;
			INDEX ix = (index + mRead) % mDeque.size () ;
			return mDeque[ix].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		BOOL empty () const {
			if (mDeque.size () == 0)
				return TRUE ;
			if (mWrite - mRead > 0)
				return FALSE ;
			return TRUE ;
		}

		BOOL full () const {
			if (mDeque.size () == 0)
				return TRUE ;
			if (mWrite - mRead < mDeque.size ())
				return FALSE ;
			return TRUE ;
		}

		void add (CREF<ITEM> item) {
			add (move (item)) ;
		}

		void add (RREF<ITEM> item) {
			auto rax = move (item) ;
			update_resize () ;
			assert (mDeque.size () > 0) ;
			INDEX ix = mWrite % mDeque.size () ;
			mDeque[ix].mItem = move (rax) ;
			mWrite++ ;
		}

		void take () {
			assert (ifnot (empty ())) ;
			mRead++ ;
			update_index () ;
		}

		void take (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			INDEX ix = mRead ;
			item = move (mDeque[ix].mItem) ;
			mRead++ ;
			update_index () ;
		}

		void push (CREF<ITEM> item) {
			push (move (item)) ;
		}

		void push (RREF<ITEM> item) {
			auto rax = move (item) ;
			update_resize () ;
			INDEX ix = mRead ;
			replace (ix ,ZERO ,mDeque.size ()) ;
			ix-- ;
			mDeque[ix].mItem = move (rax) ;
			mRead = ix ;
			update_index () ;
		}

		void pop () {
			assert (ifnot (empty ())) ;
			mWrite-- ;
		}

		void pop (VREF<ITEM> item) {
			assert (ifnot (empty ())) ;
			assert (mDeque.size () > 0) ;
			INDEX ix = mWrite % mDeque.size () ;
			replace (ix ,ZERO ,mDeque.size ()) ;
			ix-- ;
			item = move (mDeque[ix].mItem) ;
			mWrite-- ;
		}

		INDEX head () const {
			if (empty ())
				return NONE ;
			return 0 ;
		}

		INDEX tail () const {
			if (empty ())
				return NONE ;
			return mWrite - mRead - 1 ;
		}

	private:
		void update_index () {
			if ifswitch (TRUE) {
				if (mRead < mDeque.size ())
					discard ;
				assert (mDeque.size () > 0) ;
				mRead = mRead % mDeque.size () ;
				mWrite = mWrite % mDeque.size () ;
			}
			if (mRead <= mWrite)
				return ;
			mWrite += mDeque.size () ;
		}

		void update_resize () {
			using R1X = typename ALLOCATOR_HELP<ITEM ,SIZE ,ALWAYS>::RESIZE_POW ;
			using R2X = typename ALLOCATOR_HELP<ITEM ,SIZE ,ALWAYS>::RESIZE_MIN_SIZE ;
			if (length () < size ())
				return ;
			const auto r1x = mDeque.size () ;
			const auto r2x = vmax (r1x * R1X::expr ,R2X::expr) ;
			mDeque.resize (r2x) ;
			const auto r3x = mDeque.size () ;
			if ifswitch (TRUE) {
				if (mRead == 0)
					discard ;
				for (auto &&i : CSC::iter (0 ,r1x - mRead)) {
					INDEX ix = r1x - 1 - i ;
					INDEX iy = r3x - 1 - i ;
					mDeque[iy].mItem = move (mDeque[ix].mItem) ;
				}
				mRead += r3x - r1x ;
				mWrite = mRead + r1x ;
			}
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Deque = typename DEQUE_HELP<ITEM ,SIZE ,ALWAYS>::Deque ;

template <class...>
trait PRIORITY_HELP ;

template <class...>
trait PRIORITY_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait PRIORITY_HOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		ITEM mItem ;
		INDEX mMap ;
	} ;

	class Priority {
	protected:
		Buffer<NODE ,SIZE> mPriority ;
		INDEX mRead ;
		INDEX mWrite ;

	public:
		implicit Priority () noexcept {
			mRead = 0 ;
			mWrite = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait PRIORITY_HELP<ITEM ,SIZE ,ALWAYS> {
	using NODE = typename PRIORITY_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename PRIORITY_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Priority ;

	class Priority extend SUPER {
	protected:
		using SUPER::mPriority ;
		using SUPER::mRead ;
		using SUPER::mWrite ;

	public:
		implicit Priority () = default ;

		explicit Priority (CREF<SizeProxy> size_) {
			mPriority = Buffer<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit Priority (CREF<csc_initializer_t<ITEM>> that)
			:Priority (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit Priority (CREF<SpanIterator<ITEM>> that) {
			mPriority = Buffer<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		explicit Priority (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mPriority = Buffer<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		LENGTH size () const {
			return mPriority.size () ;
		}

		LENGTH length () const {
			if (mPriority.size () == 0)
				return 0 ;
			return mWrite ;
		}

		void clear () {
			mRead = 0 ;
			mWrite = 0 ;
		}

		ArrayIterator<CREF<Priority> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<Priority> ,ITEM> (CRef<Priority>::reference (thiz)) ;
		}

		ArrayIterator<CREF<Priority> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<Priority> ,ITEM> (CRef<Priority>::reference (thiz)) ;
		}

		ArrayIterator<Priority> iter () const leftvalue {
			return ArrayIterator<Priority> (CRef<Priority>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			if (mPriority.size () == 0)
				return NONE ;
			return 0 ;
		}

		INDEX iend () const {
			if (mPriority.size () == 0)
				return NONE ;
			return mWrite ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return index + 1 ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (vbetween (index ,0 ,mWrite)) ;
			return mPriority[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		INDEX map_get (CREF<INDEX> index) const {
			assert (vbetween (index ,0 ,mWrite)) ;
			return mPriority[index].mMap ;
		}

		void map_set (CREF<INDEX> index ,CREF<INDEX> map_) {
			assert (vbetween (index ,0 ,mWrite)) ;
			mPriority[index].mMap = map_ ;
		}

		Array<INDEX> map_all () const {
			const auto r1x = IterArray<INDEX>::make (iter ()) ;
			Array<INDEX> ret = Array<INDEX> (r1x.length ()) ;
			for (auto &&i : r1x.iter ())
				ret[i] = mPriority[r1x[i]].mMap ;
			return move (ret) ;
		}

		BOOL empty () const {
			if (mPriority.size () == 0)
				return TRUE ;
			if (mWrite > 0)
				return FALSE ;
			return TRUE ;
		}

		BOOL full () const {
			if (mPriority.size () == 0)
				return TRUE ;
			if (mWrite < mPriority.size ())
				return FALSE ;
			return TRUE ;
		}

		void add (CREF<ITEM> item) {
			add (item ,NONE) ;
		}

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (item ,NONE) ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			auto rax = move (item) ;
			update_resize () ;
			INDEX ix = mWrite ;
			mPriority[ix].mItem = move (rax) ;
			mPriority[ix].mMap = map_ ;
			mWrite++ ;
			update_insert (ix) ;
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

		INDEX head () const {
			if (empty ())
				return NONE ;
			return 0 ;
		}

		void remove (CREF<INDEX> index) {
			assert (vbetween (index ,0 ,mWrite)) ;
			INDEX ix = mWrite - 1 ;
			mPriority[index] = move (mPriority[ix]) ;
			mWrite = ix ;
			update_insert (index) ;
		}

	private:
		void update_resize () {
			using R1X = typename ALLOCATOR_HELP<NODE ,SIZE ,ALWAYS>::RESIZE_POW ;
			using R2X = typename ALLOCATOR_HELP<NODE ,SIZE ,ALWAYS>::RESIZE_MIN_SIZE ;
			if (length () < size ())
				return ;
			const auto r1x = mPriority.size () ;
			const auto r2x = vmax (r1x * R1X::expr ,R2X::expr) ;
			mPriority.resize (r2x) ;
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
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using Priority = typename PRIORITY_HELP<ITEM ,SIZE ,ALWAYS>::Priority ;

template <class...>
trait LIST_HELP ;

template <class...>
trait LIST_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait LIST_HOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		ITEM mItem ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	class List {
	protected:
		Allocator<NODE ,SIZE> mList ;
		INDEX mFirst ;
		INDEX mLast ;

	public:
		implicit List () noexcept {
			mFirst = NONE ;
			mLast = NONE ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait LIST_HELP<ITEM ,SIZE ,ALWAYS> {
	using NODE = typename LIST_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename LIST_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::List ;

	class List extend SUPER {
	protected:
		using SUPER::mList ;
		using SUPER::mFirst ;
		using SUPER::mLast ;

	public:
		implicit List () = default ;

		explicit List (CREF<SizeProxy> size_) {
			mList = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit List (CREF<csc_initializer_t<ITEM>> that)
			:List (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit List (CREF<SpanIterator<ITEM>> that) {
			mList = Allocator<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		explicit List (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mList = Allocator<NODE ,SIZE> (that.rank ()) ;
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

		void reserve (CREF<LENGTH> size_) {
			const auto r1x = size_ - (mList.size () - mList.length () ()) ;
			if (r1x <= 0)
				return ;
			const auto r2x = r1x + mList.size () ;
			mList.resize (r2x) ;
		}

		ArrayIterator<VREF<List> ,ITEM> begin () leftvalue {
			return ArrayIterator<VREF<List> ,ITEM> (VRef<List>::reference (thiz)) ;
		}

		ArrayIterator<VREF<List> ,ITEM> end () leftvalue {
			return ArrayIterator<VREF<List> ,ITEM> (VRef<List>::reference (thiz)) ;
		}

		ArrayIterator<CREF<List> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<List> ,ITEM> (CRef<List>::reference (thiz)) ;
		}

		ArrayIterator<CREF<List> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<List> ,ITEM> (CRef<List>::reference (thiz)) ;
		}

		ArrayIterator<List> iter () const leftvalue {
			return ArrayIterator<List> (CRef<List>::reference (thiz)) ;
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
			return mList[index].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mList[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
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

		void add (RREF<ITEM> item) {
			auto rax = move (item) ;
			INDEX ix = mList.alloc () ;
			mList[ix].mItem = move (rax) ;
			mList[ix].mPrev = mLast ;
			mList[ix].mNext = NONE ;
			curr_next (mLast ,ix) ;
			mLast = ix ;
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
			item = move (mList[ix].mItem) ;
			mFirst = mList[ix].mNext ;
			curr_prev (mFirst ,NONE) ;
			mList.free (ix) ;
		}

		void push (CREF<ITEM> item) {
			push (move (item)) ;
		}

		void push (RREF<ITEM> item) {
			auto rax = move (item) ;
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
			item = move (mList[ix].mItem) ;
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

		INDEX insert () {
			INDEX ret = mList.alloc () ;
			mList[ret].mPrev = mLast ;
			mList[ret].mNext = NONE ;
			curr_next (mLast ,ret) ;
			mLast = ret ;
			return move (ret) ;
		}

		INDEX insert (CREF<INDEX> index) {
			INDEX ret = mList.alloc () ;
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
			if (length () < 2)
				return ;
			for (auto &&i : range_) {
				mList[i].mPrev = NONE ;
				mList[i].mNext = NONE ;
			}
			for (auto &&i : CSC::iter (0 ,1)) {
				mList[range_[i]].mPrev = NONE ;
				mList[range_[i]].mNext = range_[i + 1] ;
				mFirst = range_[i] ;
			}
			for (auto &&i : CSC::iter (1 ,range_.length () - 1)) {
				mList[range_[i]].mPrev = range_[i - 1] ;
				mList[range_[i]].mNext = range_[i + 1] ;
			}
			for (auto &&i : CSC::iter (range_.length () - 1 ,range_.length ())) {
				mList[range_[i]].mPrev = range_[i - 1] ;
				mList[range_[i]].mNext = NONE ;
				mLast = range_[i] ;
			}
		}

	private:
		void curr_next (CREF<INDEX> curr ,CREF<INDEX> next) {
			auto act = TRUE ;
			if ifswitch (act) {
				if (curr == NONE)
					discard ;
				mList[curr].mNext = next ;
			}
			if ifswitch (act) {
				mFirst = next ;
			}
		}

		void curr_prev (CREF<INDEX> curr ,CREF<INDEX> prev) {
			auto act = TRUE ;
			if ifswitch (act) {
				if (curr == NONE)
					discard ;
				mList[curr].mPrev = prev ;
			}
			if ifswitch (act) {
				mLast = prev ;
			}
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using List = typename LIST_HELP<ITEM ,SIZE ,ALWAYS>::List ;

template <class...>
trait ARRAYLIST_HELP ;

template <class...>
trait ARRAYLIST_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait ARRAYLIST_HOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		ITEM mItem ;
	} ;

	class ArrayList {
	protected:
		Allocator<NODE ,SIZE> mList ;
		Buffer<INDEX ,SIZE> mRange ;
		INDEX mFree ;

	public:
		implicit ArrayList () noexcept {
			mFree = 0 ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait ARRAYLIST_HELP<ITEM ,SIZE ,ALWAYS> {
	using NODE = typename ARRAYLIST_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename ARRAYLIST_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::ArrayList ;

	class ArrayList extend SUPER {
	protected:
		using SUPER::mList ;
		using SUPER::mRange ;
		using SUPER::mFree ;

	public:
		implicit ArrayList () = default ;

		explicit ArrayList (CREF<SizeProxy> size_) {
			mList = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit ArrayList (CREF<csc_initializer_t<ITEM>> that)
			:ArrayList (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit ArrayList (CREF<SpanIterator<ITEM>> that) {
			mList = Allocator<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		explicit ArrayList (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mList = Allocator<NODE ,SIZE> (that.rank ()) ;
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
			BufferProc<INDEX>::buf_fill (mRange ,NONE ,0 ,mRange.size ()) ;
			mFree = 0 ;
		}

		void reserve (CREF<LENGTH> size_) {
			const auto r1x = size_ - (mList.size () - mList.length () ()) ;
			if (r1x <= 0)
				return ;
			const auto r2x = r1x + mList.size () ;
			mList.resize (r2x) ;
		}

		ArrayIterator<VREF<ArrayList> ,ITEM> begin () leftvalue {
			return ArrayIterator<VREF<ArrayList> ,ITEM> (VRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<VREF<ArrayList> ,ITEM> end () leftvalue {
			return ArrayIterator<VREF<ArrayList> ,ITEM> (VRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<CREF<ArrayList> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<ArrayList> ,ITEM> (CRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<CREF<ArrayList> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<ArrayList> ,ITEM> (CRef<ArrayList>::reference (thiz)) ;
		}

		ArrayIterator<ArrayList> iter () const leftvalue {
			return ArrayIterator<ArrayList> (CRef<ArrayList>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CSC::iter (0 ,mRange.size ())) {
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
			for (auto &&i : CSC::iter (r1x ,mRange.size ())) {
				if (mRange[i] != NONE)
					return i ;
			}
			return NONE ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			return mList[mRange[index]].mItem ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mList[mRange[index]].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		BOOL empty () const {
			return mList.length () == 0 ;
		}

		void add (CREF<ITEM> item) {
			add (move (item)) ;
		}

		void add (RREF<ITEM> item) {
			auto rax = move (item) ;
			INDEX ix = mList.alloc () ;
			update_range (ix) ;
			mList[ix].mItem = move (rax) ;
			INDEX jx = find_next_free () ;
			mRange[jx] = ix ;
		}

		INDEX insert () {
			INDEX ret = mList.alloc () ;
			update_range (ret) ;
			INDEX jx = find_next_free () ;
			mRange[jx] = ret ;
			return move (ret) ;
		}

		INDEX insert (CREF<INDEX> index) {
			if ifswitch (TRUE) {
				if (mRange[index] != NONE)
					discard ;
				INDEX ix = mList.alloc () ;
				update_range (ix) ;
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
			if (length () < 2)
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
			for (auto &&i : CSC::iter (0 ,mList.size ())) {
				if ifnot (mList.used (i))
					continue ;
				mRange[ix] = i ;
				ix++ ;
			}
			assert (ix == mRange.size ()) ;
			mFree = 0 ;
		}

	private:
		INDEX find_next_free () {
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
			BufferProc<INDEX>::buf_fill (mRange ,NONE ,r1x ,mRange.size ()) ;
		}
	} ;
} ;

template <class ITEM ,class SIZE = VARIABLE>
using ArrayList = typename ARRAYLIST_HELP<ITEM ,SIZE ,ALWAYS>::ArrayList ;

template <class...>
trait BITPROXY_HELP ;

template <class UNIT ,class COND>
trait BITPROXY_HELP<UNIT ,COND ,REQUIRE<COND>> {
	class BitProxy {
	protected:
		VRef<UNIT> mBitSet ;
		INDEX mY ;

	public:
		implicit BitProxy () = delete ;

		explicit BitProxy (RREF<VRef<UNIT>> array_ ,CREF<INDEX> y_) {
			mBitSet = move (array_) ;
			mY = y_ ;
		}

		inline implicit operator BOOL () rightvalue {
			return mBitSet->map_get (mY) ;
		}

		inline void operator= (CREF<BOOL> that) rightvalue {
			mBitSet->map_set (mY ,that) ;
		}
	} ;
} ;

template <class UNIT ,class COND>
trait BITPROXY_HELP<UNIT ,COND ,REQUIRE<ENUM_NOT<COND>>> {
	class BitProxy {
	protected:
		CRef<UNIT> mBitSet ;
		INDEX mY ;

	public:
		implicit BitProxy () = delete ;

		explicit BitProxy (RREF<CRef<UNIT>> array_ ,CREF<INDEX> y_) {
			mBitSet = move (array_) ;
			mY = y_ ;
		}

		inline implicit operator BOOL () rightvalue {
			return mBitSet->map_get (mY) ;
		}
	} ;
} ;

template <class UNIT>
using BitProxy = typename BITPROXY_HELP<REMOVE_REF<UNIT> ,IS_VARIABLE<UNIT> ,ALWAYS>::BitProxy ;

template <class...>
trait BITSET_HELP ;

template <class...>
trait BITSET_HOLDER_HELP ;

template <class...>
trait BITSET_BYTELCACHE_HELP ;

template <class...>
trait BITSET_BYTEFCACHE_HELP ;

template <class SIZE>
trait BITSET_HOLDER_HELP<SIZE ,ALWAYS> {
	using BUFFER_SIZE = ENUM_DIV<ENUM_ALIGN<SIZE ,RANK8> ,RANK8> ;
	using RESERVE_SIZE = CONDITIONAL<ENUM_COMPR_GTEQ<SIZE ,ENUM_ZERO> ,BUFFER_SIZE ,SIZE> ;

	class BitSet {
	protected:
		Buffer<BYTE ,RESERVE_SIZE> mSet ;
		LENGTH mWidth ;

	public:
		implicit BitSet () noexcept {
			mWidth = vmax (SIZE::expr ,0) ;
		}
	} ;
} ;

template <class SIZE>
trait BITSET_HELP<SIZE ,ALWAYS> {
	using RESERVE_SIZE = typename BITSET_HOLDER_HELP<SIZE ,ALWAYS>::RESERVE_SIZE ;
	using SUPER = typename BITSET_HOLDER_HELP<SIZE ,ALWAYS>::BitSet ;
	using ITEM = INDEX ;

	class BitSet extend SUPER {
	protected:
		using SUPER::mSet ;
		using SUPER::mWidth ;

	public:
		implicit BitSet () = default ;

		explicit BitSet (CREF<SizeProxy> size_) {
			assert (size_ >= 0) ;
			mSet = Buffer<BYTE ,RESERVE_SIZE> (reserve_size (size_)) ;
			mWidth = vmax (SIZE::expr ,size_) ;
			clear () ;
		}

		LENGTH size () const {
			if (mSet.size () == 0)
				return 0 ;
			return mWidth ;
		}

		LENGTH length () const {
			using R1X = typename DEPENDENT<BITSET_BYTELCACHE_HELP<DEPEND ,ALWAYS> ,SIZE>::ByteLCache ;
			LENGTH ret = 0 ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
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
			BufferProc<BYTE>::buf_fill (mSet ,item ,0 ,mSet.size ()) ;
		}

		ArrayIterator<CREF<BitSet> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<BitSet> ,ITEM> (CRef<BitSet>::reference (thiz)) ;
		}

		ArrayIterator<CREF<BitSet> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<BitSet> ,ITEM> (CRef<BitSet>::reference (thiz)) ;
		}

		ArrayIterator<BitSet> iter () const leftvalue {
			return ArrayIterator<BitSet> (CRef<BitSet>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			INDEX ix = 0 ;
			const auto r1x = find_next (ix) ;
			if (r1x >= size ())
				return NONE ;
			return r1x ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			INDEX ix = index / 8 ;
			if ifswitch (TRUE) {
				const auto r1x = index % 8 + 1 ;
				if (r1x == 8)
					discard ;
				const auto r2x = BYTE (0X01) << r1x ;
				const auto r3x = mSet[ix] & ~BYTE (INDEX (r2x) - 1) ;
				if (r3x == BYTE (0X00))
					discard ;
				const auto r4x = find_first (r3x) ;
				const auto r5x = ix * 8 + r4x ;
				if (r5x >= size ())
					return NONE ;
				return r5x ;
			}
			ix++ ;
			const auto r6x = find_next (ix) ;
			if (r6x >= size ())
				return NONE ;
			return r6x ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return index ;
		}

		inline BitProxy<VREF<BitSet>> operator[] (CREF<INDEX> index) leftvalue {
			return BitProxy<VREF<BitSet>> (VRef<BitSet>::reference (thiz) ,index) ;
		}

		inline BitProxy<CREF<BitSet>> operator[] (CREF<INDEX> index) const leftvalue {
			return BitProxy<CREF<BitSet>> (CRef<BitSet>::reference (thiz) ,index) ;
		}

		INDEX access (CREF<BitProxy<VREF<BitSet>>> item) const {
			unimplemented () ;
			return NONE ;
		}

		INDEX access (CREF<BitProxy<CREF<BitSet>>> item) const {
			unimplemented () ;
			return NONE ;
		}

		BOOL map_get (CREF<INDEX> index) const {
			assert (vbetween (index ,0 ,mWidth)) ;
			INDEX ix = index / 8 ;
			const auto r1x = BYTE (0X01) << (index % 8) ;
			const auto r2x = mSet[ix] & r1x ;
			if (r2x == BYTE (0X00))
				return FALSE ;
			return TRUE ;
		}

		void map_set (CREF<INDEX> index ,CREF<BOOL> map_) {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (map_)
					discard ;
				add (index) ;
			}
			if ifswitch (act) {
				erase (index) ;
			}
		}

		void add (CREF<ITEM> item) {
			assume (vbetween (item ,0 ,mWidth)) ;
			INDEX ix = item / 8 ;
			const auto r1x = BYTE (0X01) << (item % 8) ;
			mSet[ix] |= r1x ;
		}

		void erase (CREF<ITEM> item) {
			assume (vbetween (item ,0 ,mWidth)) ;
			INDEX ix = item / 8 ;
			const auto r1x = BYTE (0X01) << (item % 8) ;
			mSet[ix] &= ~r1x ;
		}

		BOOL equal (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			const auto r1x = mSet.size () - 1 ;
			if (r1x < 0)
				return TRUE ;
			const auto r2x = BufferProc<BYTE>::buf_equal (mSet ,that.mSet ,0 ,r1x) ;
			if ifnot (r2x)
				return FALSE ;
			if (last_byte () != that.last_byte ())
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
			const auto r1x = mSet.size () - 1 ;
			if (r1x < 0)
				return ZERO ;
			const auto r2x = BufferProc<BYTE>::buf_compr (mSet ,that.mSet ,0 ,r1x) ;
			if (r2x != ZERO)
				return r2x ;
			return operator_compr (last_byte () ,that.last_byte ()) ;
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
			const auto r1x = mSet.size () - 1 ;
			if (r1x < 0)
				return hashcode () ;
			const auto r2x = BufferProc<BYTE>::buf_hash (mSet ,0 ,r1x) ;
			const auto r3x = operator_hash (last_byte ()) ;
			return hashcode (r2x ,r3x) ;
		}

		BitSet band (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] & that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator& (CREF<BitSet> that) const {
			return band (that) ;
		}

		void band_with (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				mSet[i] &= that.mSet[i] ;
		}

		inline void operator&= (CREF<BitSet> that) {
			band_with (that) ;
		}

		BitSet bor (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] | that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator| (CREF<BitSet> that) const {
			return bor (that) ;
		}

		void bor_with (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				mSet[i] |= that.mSet[i] ;
		}

		inline void operator|= (CREF<BitSet> that) {
			bor_with (that) ;
		}

		BitSet bxor (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] ^ that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator^ (CREF<BitSet> that) const {
			return bxor (that) ;
		}

		void bxor_with (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				mSet[i] ^= that.mSet[i] ;
		}

		inline void operator^= (CREF<BitSet> that) {
			bxor_with (that) ;
		}

		BitSet bsub (CREF<BitSet> that) const {
			assert (size () == that.size ()) ;
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				ret.mSet[i] = mSet[i] & ~that.mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator- (CREF<BitSet> that) const {
			return bsub (that) ;
		}

		void bsub_with (CREF<BitSet> that) {
			assert (size () == that.size ()) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				mSet[i] &= ~that.mSet[i] ;
		}

		inline void operator-= (CREF<BitSet> that) {
			bsub_with (that) ;
		}

		BitSet bnot () const {
			BitSet ret = BitSet (mWidth) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ()))
				ret.mSet[i] = ~mSet[i] ;
			return move (ret) ;
		}

		inline BitSet operator~ () const {
			return bnot () ;
		}

	private:
		LENGTH reserve_size (CREF<LENGTH> size_) const {
			if (size_ < 0)
				return size_ ;
			return (size_ + 7) / 8 ;
		}

		INDEX find_first (CREF<BYTE> obj) const {
			using R1X = typename DEPENDENT<BITSET_BYTEFCACHE_HELP<DEPEND ,ALWAYS> ,SIZE>::ByteFCache ;
			const auto r1x = obj & BYTE (INDEX (~obj) + 1) ;
			return R1X::instance ()[r1x] ;
		}

		INDEX find_next (VREF<INDEX> iw) const {
			while (TRUE) {
				if (iw >= mSet.size ())
					break ;
				if (mSet[iw] != BYTE (0X00))
					break ;
				iw++ ;
			}
			if (iw >= mSet.size ())
				return NONE ;
			const auto r1x = find_first (mSet[iw]) ;
			return iw * 8 + r1x ;
		}

		BYTE last_byte () const {
			INDEX ix = mSet.size () - 1 ;
			return mSet[ix] & BYTE (mWidth % 8 - 1) ;
		}
	} ;
} ;

template <class DEPEND>
trait BITSET_BYTELCACHE_HELP<DEPEND ,ALWAYS> {
	using SIZE = ENUMAS<VAL ,256> ;

	class ByteLCache {
	protected:
		BoxBuffer<LENGTH ,SIZE> mCache ;

	public:
		imports CREF<ByteLCache> instance () {
			static const LENGTH M_LCACHE[] {
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
			return memorize ([&] () {
				ByteLCache ret ;
				ret.mCache = BoxBuffer<LENGTH ,SIZE> (M_LCACHE) ;
				return move (ret) ;
			}) ;
		}

		inline LENGTH operator[] (CREF<BYTE> k) const {
			const auto r1x = INDEX (k) ;
			return mCache[r1x] ;
		}
	} ;
} ;

template <class DEPEND>
trait BITSET_BYTEFCACHE_HELP<DEPEND ,ALWAYS> {
	using SIZE = ENUMAS<VAL ,256> ;

	class ByteFCache {
	protected:
		BoxBuffer<LENGTH ,SIZE> mCache ;

	public:
		imports CREF<ByteFCache> instance () {
			static const INDEX M_FCACHE[] {
				8 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				7 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				6 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				5 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,
				4 ,0 ,1 ,0 ,2 ,0 ,1 ,0 ,3 ,0 ,1 ,0 ,2 ,0 ,1 ,0} ;
			return memorize ([&] () {
				ByteFCache ret ;
				ret.mCache = BoxBuffer<INDEX ,SIZE> (M_FCACHE) ;
				return move (ret) ;
			}) ;
		}

		inline LENGTH operator[] (CREF<BYTE> k) const {
			const auto r1x = INDEX (k) ;
			return mCache[r1x] ;
		}
	} ;
} ;

template <class SIZE = VARIABLE>
using BitSet = typename BITSET_HELP<SIZE ,ALWAYS>::BitSet ;

template <class...>
trait SET_HELP ;

template <class...>
trait SET_HOLDER_HELP ;

template <class ITEM ,class SIZE>
trait SET_HOLDER_HELP<ITEM ,SIZE ,ALWAYS> {
	struct NODE {
		ITEM mItem ;
		INDEX mMap ;
		BOOL mRed ;
		INDEX mUp ;
		INDEX mLeft ;
		INDEX mRight ;
	} ;

	class Set {
	protected:
		Allocator<NODE ,SIZE> mSet ;
		INDEX mRoot ;
		INDEX mTop ;

	public:
		implicit Set () noexcept {
			mRoot = NONE ;
			mTop = NONE ;
		}
	} ;
} ;

template <class ITEM ,class SIZE>
trait SET_HELP<ITEM ,SIZE ,ALWAYS> {
	using NODE = typename SET_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::NODE ;
	using SUPER = typename SET_HOLDER_HELP<ITEM ,SIZE ,ALWAYS>::Set ;

	struct CHILD {
		INDEX mUp ;
		BOOL mLR ;
	} ;

	class Set extend SUPER {
	protected:
		using SUPER::mSet ;
		using SUPER::mRoot ;
		using SUPER::mTop ;

	public:
		implicit Set () = default ;

		explicit Set (CREF<SizeProxy> size_) {
			mSet = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit Set (CREF<csc_initializer_t<ITEM>> that)
			:Set (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit Set (CREF<SpanIterator<ITEM>> that) {
			mSet = Allocator<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		explicit Set (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mSet = Allocator<NODE ,SIZE> (that.rank ()) ;
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

		void reserve (CREF<LENGTH> size_) {
			const auto r1x = size_ - (mSet.size () - mSet.length () ()) ;
			if (r1x <= 0)
				return ;
			const auto r2x = r1x + mSet.size () ;
			mSet.resize (r2x) ;
		}

		ArrayIterator<CREF<Set> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<Set> ,ITEM> (CRef<Set>::reference (thiz)) ;
		}

		ArrayIterator<CREF<Set> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<Set> ,ITEM> (CRef<Set>::reference (thiz)) ;
		}

		ArrayIterator<Set> iter () const leftvalue {
			return ArrayIterator<Set> (CRef<Set>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CSC::iter (0 ,size ())) {
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
			for (auto &&i : CSC::iter (r1x ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		Array<INDEX> esort () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			compute_order (mRoot ,ret ,ix) ;
			assert (ix == ret.length ()) ;
			return move (ret) ;
		}

		INDEX map_get (CREF<INDEX> index) const {
			return mSet[index].mMap ;
		}

		void map_set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet[index].mMap = map_ ;
		}

		Array<INDEX> map_all () const {
			const auto r1x = IterArray<INDEX>::make (iter ()) ;
			Array<INDEX> ret = Array<INDEX> (r1x.length ()) ;
			for (auto &&i : r1x.iter ())
				ret[i] = mSet[r1x[i]].mMap ;
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

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			auto rax = move (item) ;
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
			item = move (mSet[ix].mItem) ;
			remove (ix) ;
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
			const auto r2x = parent (index) ;
			curr_prev (r1x ,r2x) ;
			curr_next (r2x ,r1x) ;
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
				const auto r1x = operator_compr (item ,mSet[ret].mItem) ;
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
			return map_get (ix) ;
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
				const auto r1x = operator_compr (mSet[curr].mItem ,mSet[ix].mItem) ;
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
			mSet[curr].mLeft = NONE ;
			mSet[curr].mRight = NONE ;
			curr_next (rax ,curr) ;
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
				auto act = TRUE ;
				if ifswitch (act) {
					if (iy != mSet[iz].mLeft)
						discard ;
					update_insert_left (ix) ;
					ix = mTop ;
				}
				if ifswitch (act) {
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
			auto act = TRUE ;
			if ifswitch (act) {
				if (iz == NONE)
					discard ;
				if ifnot (mSet[iz].mRed)
					discard ;
				mSet[iz].mRed = FALSE ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				mTop = iy ;
			}
			if ifswitch (act) {
				if (curr != mSet[ix].mRight)
					discard ;
				rotate_left (mSet[iy].mLeft) ;
				mSet[curr].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_right (iy) ;
				mTop = ix ;
			}
			if ifswitch (act) {
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
			auto act = TRUE ;
			if ifswitch (act) {
				if (iz == NONE)
					discard ;
				if ifnot (mSet[iz].mRed)
					discard ;
				mSet[iz].mRed = FALSE ;
				mSet[ix].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				mTop = iy ;
			}
			if ifswitch (act) {
				if (curr != mSet[ix].mLeft)
					discard ;
				rotate_right (mSet[iy].mRight) ;
				mSet[curr].mRed = FALSE ;
				mSet[iy].mRed = TRUE ;
				rotate_left (iy) ;
				mTop = ix ;
			}
			if ifswitch (act) {
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
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (ix == mSet[iy].mLeft)
						discard ;
					update_remove_left (iy) ;
					ix = mTop ;
				}
				if ifswitch (act) {
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
			auto act = TRUE ;
			if ifswitch (act) {
				if (jx != NONE)
					if (mSet[jx].mRed)
						discard ;
				if (jy != NONE)
					if (mSet[jy].mRed)
						discard ;
				mSet[ix].mRed = TRUE ;
				mTop = curr ;
			}
			if ifswitch (act) {
				if (jx == NONE)
					discard ;
				if ifnot (mSet[jx].mRed)
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
			if ifswitch (act) {
				if (jy == NONE)
					discard ;
				if ifnot (mSet[jy].mRed)
					discard ;
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
			auto act = TRUE ;
			if ifswitch (act) {
				if (jx != NONE)
					if (mSet[jx].mRed)
						discard ;
				if (jy != NONE)
					if (mSet[jy].mRed)
						discard ;
				mSet[ix].mRed = TRUE ;
				mTop = curr ;
			}
			if ifswitch (act) {
				if (jx == NONE)
					discard ;
				if ifnot (mSet[jx].mRed)
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
			if ifswitch (act) {
				if (jy == NONE)
					discard ;
				if ifnot (mSet[jy].mRed)
					discard ;
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
			const auto r1x = parent (curr) ;
			mSet[ix].mUp = mSet[curr].mUp ;
			mSet[ix].mLeft = curr ;
			mSet[curr].mUp = ix ;
			curr_next (r1x ,ix) ;
		}

		void rotate_right (CREF<INDEX> curr) {
			INDEX ix = mSet[curr].mLeft ;
			mSet[curr].mLeft = mSet[ix].mRight ;
			curr_prev (mSet[ix].mRight ,left_child (curr)) ;
			const auto r1x = parent (curr) ;
			mSet[ix].mUp = mSet[curr].mUp ;
			mSet[ix].mRight = curr ;
			mSet[curr].mUp = ix ;
			curr_next (r1x ,ix) ;
		}

		CHILD parent (CREF<INDEX> curr) const {
			CHILD ret ;
			ret.mUp = mSet[curr].mUp ;
			ret.mLR = FALSE ;
			if ifswitch (TRUE) {
				if (ret.mUp == NONE)
					discard ;
				if (mSet[ret.mUp].mLeft == curr)
					discard ;
				assume (mSet[ret.mUp].mRight == curr) ;
				ret.mLR = TRUE ;
			}
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
			auto act = TRUE ;
			if ifswitch (act) {
				if (up.mUp != NONE)
					discard ;
				mRoot = next ;
			}
			if ifswitch (act) {
				if (up.mLR)
					discard ;
				mSet[up.mUp].mLeft = next ;
			}
			if ifswitch (act) {
				mSet[up.mUp].mRight = next ;
			}
		}

		void curr_prev (CREF<INDEX> curr ,CREF<CHILD> up) {
			if ifswitch (TRUE) {
				if (curr == NONE)
					discard ;
				mSet[curr].mUp = up.mUp ;
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
			swap (mSet[index1].mLeft ,mSet[index2].mLeft) ;
			swap (mSet[index1].mRight ,mSet[index2].mRight) ;
		}

		void compute_order (CREF<INDEX> curr ,VREF<Array<INDEX>> result ,VREF<INDEX> iw) const {
			if (curr == NONE)
				return ;
			compute_order (mSet[curr].mLeft ,result ,iw) ;
			result[iw] = curr ;
			iw++ ;
			compute_order (mSet[curr].mRight ,result ,iw) ;
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
		ITEM mItem ;
		INDEX mMap ;
		FLAG mHash ;
		INDEX mPrev ;
		INDEX mNext ;
	} ;

	class HashSet {
	protected:
		Allocator<NODE ,SIZE> mSet ;
		Buffer<INDEX ,SIZE> mRange ;

	public:
		implicit HashSet () = default ;

		explicit HashSet (CREF<SizeProxy> size_) {
			mSet = Allocator<NODE ,SIZE> (size_) ;
			clear () ;
		}

		explicit HashSet (CREF<csc_initializer_t<ITEM>> that)
			:HashSet (SpanIterator<ITEM ,REGISTER> (that)) {}

		explicit HashSet (CREF<SpanIterator<ITEM>> that) {
			mSet = Allocator<NODE ,SIZE> (that.rank ()) ;
			clear () ;
			for (auto &&i : that)
				add (move (i)) ;
		}

		explicit HashSet (RREF<SpanIterator<ITEM ,REGISTER>> that) {
			mSet = Allocator<NODE ,SIZE> (that.rank ()) ;
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
			BufferProc<INDEX>::buf_fill (mRange ,NONE ,0 ,mRange.size ()) ;
		}

		void reserve (CREF<LENGTH> size_) {
			const auto r1x = size_ - (mSet.size () - mSet.length () ()) ;
			if (r1x <= 0)
				return ;
			const auto r2x = r1x + mSet.size () ;
			mSet.resize (r2x) ;
		}

		ArrayIterator<CREF<HashSet> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<HashSet> ,ITEM> (CRef<HashSet>::reference (thiz)) ;
		}

		ArrayIterator<CREF<HashSet> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<HashSet> ,ITEM> (CRef<HashSet>::reference (thiz)) ;
		}

		ArrayIterator<HashSet> iter () const leftvalue {
			return ArrayIterator<HashSet> (CRef<HashSet>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			for (auto &&i : CSC::iter (0 ,size ())) {
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
			for (auto &&i : CSC::iter (r1x ,size ())) {
				if (mSet.used (i))
					return i ;
			}
			return NONE ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		INDEX map_get (CREF<INDEX> index) const {
			return mSet[index].mMap ;
		}

		void map_set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet[index].mMap = map_ ;
		}

		Array<INDEX> map_all () const {
			const auto r1x = IterArray<INDEX>::make (iter ()) ;
			Array<INDEX> ret = Array<INDEX> (r1x.length ()) ;
			for (auto &&i : r1x.iter ())
				ret[i] = mSet[r1x[i]].mMap ;
			return move (ret) ;
		}

		void add (CREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			auto rax = move (item) ;
			INDEX ix = find (rax) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mSet.alloc () ;
				update_range (ix) ;
				mSet[ix].mItem = move (rax) ;
				mSet[ix].mMap = map_ ;
				mSet[ix].mHash = operator_hash (mSet[ix].mItem) ;
				update_emplace (ix) ;
			}
		}

		void remove (CREF<INDEX> index) {
			curr_next (mSet[index].mPrev ,mSet[index].mHash ,mSet[index].mNext) ;
			curr_prev (mSet[index].mNext ,mSet[index].mHash ,mSet[index].mPrev) ;
			mSet.free (index) ;
		}

		INDEX find (CREF<ITEM> item) const {
			INDEX ret = NONE ;
			if ifswitch (TRUE) {
				if (mRange.size () == 0)
					discard ;
				const auto r1x = operator_hash (item) ;
				INDEX ix = r1x % mRange.size () ;
				ret = mRange[ix] ;
				while (TRUE) {
					if (ret == NONE)
						break ;
					if (mSet[ret].mHash == r1x)
						if (operator_equal (item ,mSet[ret].mItem))
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
			return map_get (ix) ;
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
			BufferProc<INDEX>::buf_fill (mRange ,NONE ,0 ,mRange.size ()) ;
			for (auto &&i : CSC::iter (0 ,mSet.size ())) {
				if (i == curr)
					continue ;
				if ifnot (mSet.used (i))
					continue ;
				update_emplace (i) ;
			}
		}

		void update_emplace (CREF<INDEX> curr) {
			assert (mRange.size () > 0) ;
			INDEX ix = mSet[curr].mHash % mRange.size () ;
			mSet[curr].mPrev = NONE ;
			mSet[curr].mNext = mRange[ix] ;
			curr_prev (mRange[ix] ,0 ,curr) ;
			mRange[ix] = curr ;
		}

		void curr_next (CREF<INDEX> curr ,CREF<FLAG> hash ,CREF<INDEX> next) {
			auto act = TRUE ;
			if ifswitch (act) {
				if (curr == NONE)
					discard ;
				mSet[curr].mNext = next ;
			}
			if ifswitch (act) {
				assert (mRange.size () > 0) ;
				INDEX ix = hash % mRange.size () ;
				mRange[ix] = next ;
			}
		}

		void curr_prev (CREF<INDEX> curr ,CREF<FLAG> hash ,CREF<INDEX> prev) {
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

template <class...>
trait SOFTSET_HELP ;

template <class...>
trait SOFTSET_HOLDER_HELP ;

template <class ITEM>
trait SOFTSET_HOLDER_HELP<ITEM ,ALWAYS> {
	using SIZE = VARIABLE ;

	struct NODE {
		ITEM mItem ;
		INDEX mMap ;
		LENGTH mWeight ;
		INDEX mNext ;
		INDEX mLeft ;
		INDEX mRight ;
	} ;

	class SoftSet {
	protected:
		SharedRef<Allocator<NODE ,SIZE>> mHeap ;
		VRef<Allocator<NODE ,SIZE>> mSet ;
		LENGTH mLength ;
		INDEX mRoot ;
		INDEX mFirst ;

	public:
		implicit SoftSet () noexcept {
			mLength = 0 ;
			mRoot = NONE ;
			mFirst = NONE ;
		}
	} ;
} ;

template <class ITEM>
trait SOFTSET_HELP<ITEM ,ALWAYS> {
	using SIZE = typename SOFTSET_HOLDER_HELP<ITEM ,ALWAYS>::SIZE ;
	using NODE = typename SOFTSET_HOLDER_HELP<ITEM ,ALWAYS>::NODE ;
	using SUPER = typename SOFTSET_HOLDER_HELP<ITEM ,ALWAYS>::SoftSet ;

	class SoftSet extend SUPER {
	protected:
		using SUPER::mHeap ;
		using SUPER::mSet ;
		using SUPER::mLength ;
		using SUPER::mRoot ;
		using SUPER::mFirst ;

	public:
		implicit SoftSet () = default ;

		explicit SoftSet (CREF<SizeProxy> size_) {
			mHeap = SharedRef<Allocator<NODE ,SIZE>>::make (size_) ;
			mSet = VRef<Allocator<NODE ,SIZE>>::reference (mHeap.self) ;
		}

		SoftSet share () {
			SoftSet ret ;
			if ifswitch (TRUE) {
				if (mHeap.exist ())
					discard ;
				mHeap = SharedRef<Allocator<NODE ,SIZE>>::make (0) ;
				mSet = VRef<Allocator<NODE ,SIZE>>::reference (mHeap.self) ;
			}
			ret.mHeap = mHeap ;
			ret.mSet = VRef<Allocator<NODE ,SIZE>>::reference (ret.mHeap.self) ;
			return move (ret) ;
		}

		LENGTH size () const {
			if (mSet == NULL)
				return 0 ;
			return mSet->size () - mSet->length () + mLength ;
		}

		LENGTH length () const {
			if (mSet == NULL)
				return 0 ;
			return mLength ;
		}

		ArrayIterator<CREF<SoftSet> ,ITEM> begin () const leftvalue {
			return ArrayIterator<CREF<SoftSet> ,ITEM> (CRef<SoftSet>::reference (thiz)) ;
		}

		ArrayIterator<CREF<SoftSet> ,ITEM> end () const leftvalue {
			return ArrayIterator<CREF<SoftSet> ,ITEM> (CRef<SoftSet>::reference (thiz)) ;
		}

		ArrayIterator<SoftSet> iter () const leftvalue {
			return ArrayIterator<SoftSet> (CRef<SoftSet>::reference (thiz)) ;
		}

		INDEX ibegin () const {
			return mFirst ;
		}

		INDEX iend () const {
			return NONE ;
		}

		INDEX inext (CREF<INDEX> index) const {
			return mSet.self[index].mNext ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			return mSet.self[index].mItem ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		INDEX access (CREF<ITEM> item) const {
			unimplemented () ;
			return NONE ;
		}

		Array<INDEX> esort () const {
			Array<INDEX> ret = Array<INDEX> (length ()) ;
			INDEX ix = 0 ;
			compute_order (mRoot ,ret ,ix) ;
			assert (ix == ret.length ()) ;
			return move (ret) ;
		}

		INDEX map_get (CREF<INDEX> index) const {
			return mSet.self[index].mMap ;
		}

		void map_set (CREF<INDEX> index ,CREF<INDEX> map_) {
			mSet.self[index].mMap = map_ ;
		}

		Array<INDEX> map_all () const {
			const auto r1x = IterArray<INDEX>::make (iter ()) ;
			Array<INDEX> ret = Array<INDEX> (r1x.length ()) ;
			for (auto &&i : r1x.iter ())
				ret[i] = mSet.self[r1x[i]].mMap ;
			return move (ret) ;
		}

		void add (CREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		void add (CREF<ITEM> item ,CREF<INDEX> map_) {
			add (move (item) ,map_) ;
		}

		void add (RREF<ITEM> item) {
			add (move (item) ,NONE) ;
		}

		void add (RREF<ITEM> item ,CREF<INDEX> map_) {
			auto rax = move (item) ;
			INDEX ix = find (rax) ;
			if ifswitch (TRUE) {
				if (ix != NONE)
					discard ;
				ix = mSet->alloc () ;
				mSet.self[ix].mItem = move (rax) ;
				mSet.self[ix].mMap = map_ ;
				mSet.self[ix].mWeight = 1 ;
				mSet.self[ix].mNext = mFirst ;
				mSet.self[ix].mLeft = NONE ;
				mSet.self[ix].mRight = NONE ;
				mFirst = ix ;
				mLength++ ;
				update_insert (mRoot) ;
			}
		}

		INDEX find (CREF<ITEM> item) const {
			INDEX ret = mRoot ;
			while (TRUE) {
				if (ret == NONE)
					break ;
				const auto r1x = operator_compr (item ,mSet.self[ret].mItem) ;
				if (r1x == ZERO)
					break ;
				const auto r2x = invoke ([&] () {
					if (r1x < ZERO)
						return mSet.self[ret].mLeft ;
					return mSet.self[ret].mRight ;
				}) ;
				ret = r2x ;
			}
			return move (ret) ;
		}

		INDEX map (CREF<ITEM> item) const {
			INDEX ix = find (item) ;
			if (ix == NONE)
				return NONE ;
			return map_get (ix) ;
		}

		void order () {
			if (length () < 2)
				return ;
			const auto r1x = esort () ;
			for (auto &&i : CSC::iter (0 ,1)) {
				mSet.self[r1x[i]].mNext = r1x[i + 1] ;
				mFirst = r1x[i] ;
			}
			for (auto &&i : CSC::iter (1 ,r1x.length () - 1))
				mSet.self[r1x[i]].mNext = r1x[i + 1] ;
			for (auto &&i : CSC::iter (r1x.length () - 1 ,r1x.length ()))
				mSet.self[r1x[i]].mNext = NONE ;
		}

	private:
		void update_insert (VREF<INDEX> curr) {
			if ifswitch (TRUE) {
				if (curr != NONE)
					discard ;
				curr = mFirst ;
				return ;
			}
			INDEX ix = curr ;
			const auto r1x = operator_compr (mSet.self[mFirst].mItem ,mSet.self[ix].mItem) ;
			assert (r1x != ZERO) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (r1x > ZERO)
					discard ;
				mSet.self[ix].mWeight++ ;
				update_insert (mSet.self[ix].mLeft) ;
				update_insert_left (ix) ;
				curr = ix ;
			}
			if ifswitch (act) {
				mSet.self[ix].mWeight++ ;
				update_insert (mSet.self[ix].mRight) ;
				update_insert_right (ix) ;
				curr = ix ;
			}
		}

		void update_insert_left (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (mSet.self[ix].mLeft == NONE)
				return ;
			const auto r1x = node_weight (mSet.self[ix].mRight) ;
			const auto r2x = node_weight (mSet.self[mSet.self[ix].mLeft].mLeft) ;
			const auto r3x = node_weight (mSet.self[mSet.self[ix].mLeft].mRight) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_left (mSet.self[ix].mLeft) ;
			}
			rotate_right (ix) ;
			update_insert_left (mSet.self[ix].mLeft) ;
			update_insert_right (mSet.self[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void update_insert_right (VREF<INDEX> curr) {
			INDEX ix = curr ;
			if (mSet.self[ix].mRight == NONE)
				return ;
			const auto r1x = node_weight (mSet.self[ix].mLeft) ;
			const auto r2x = node_weight (mSet.self[mSet.self[ix].mRight].mRight) ;
			const auto r3x = node_weight (mSet.self[mSet.self[ix].mRight].mLeft) ;
			if (r1x >= vmax (r2x ,r3x))
				return ;
			if ifswitch (TRUE) {
				if (r1x < r2x)
					discard ;
				rotate_right (mSet.self[ix].mRight) ;
			}
			rotate_left (ix) ;
			update_insert_left (mSet.self[ix].mLeft) ;
			update_insert_right (mSet.self[ix].mRight) ;
			update_insert_left (ix) ;
			update_insert_right (ix) ;
			curr = ix ;
		}

		void rotate_left (VREF<INDEX> curr) {
			INDEX ix = mSet.self[curr].mRight ;
			mSet.self[curr].mRight = mSet.self[ix].mLeft ;
			mSet.self[ix].mLeft = curr ;
			mSet.self[ix].mWeight = mSet.self[curr].mWeight ;
			const auto r1x = node_weight (mSet.self[curr].mLeft) ;
			const auto r2x = node_weight (mSet.self[curr].mRight) ;
			mSet.self[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		void rotate_right (VREF<INDEX> curr) {
			INDEX ix = mSet.self[curr].mLeft ;
			mSet.self[curr].mLeft = mSet.self[ix].mRight ;
			mSet.self[ix].mRight = curr ;
			mSet.self[ix].mWeight = mSet.self[curr].mWeight ;
			const auto r1x = node_weight (mSet.self[curr].mLeft) ;
			const auto r2x = node_weight (mSet.self[curr].mRight) ;
			mSet.self[curr].mWeight = r1x + r2x + 1 ;
			curr = ix ;
		}

		LENGTH node_weight (CREF<INDEX> curr) const {
			if (curr == NONE)
				return 0 ;
			return mSet.self[curr].mWeight ;
		}

		void compute_order (CREF<INDEX> curr ,VREF<Array<INDEX>> result ,VREF<INDEX> iw) const {
			if (curr == NONE)
				return ;
			compute_order (mSet.self[curr].mLeft ,result ,iw) ;
			result[iw] = curr ;
			iw++ ;
			compute_order (mSet.self[curr].mRight ,result ,iw) ;
		}
	} ;
} ;

template <class ITEM>
using SoftSet = typename SOFTSET_HELP<ITEM ,ALWAYS>::SoftSet ;
} ;