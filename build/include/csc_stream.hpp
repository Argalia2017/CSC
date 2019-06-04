#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
namespace U {
template <class>
struct BYTE_TRAITS ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (BYTE)>> {
	using TYPE = BYTE ;
} ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (WORD)>> {
	using TYPE = WORD ;
} ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (CHAR)>> {
	using TYPE = CHAR ;
} ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (DATA)>> {
	using TYPE = DATA ;
} ;

template <class>
struct TEXT_TRAITS ;

template <>
struct TEXT_TRAITS<ARGC<_SIZEOF_ (STRU8)>> {
	using TYPE = STRU8 ;
} ;

template <>
struct TEXT_TRAITS<ARGC<_SIZEOF_ (STRU16)>> {
	using TYPE = STRU16 ;
} ;

template <>
struct TEXT_TRAITS<ARGC<_SIZEOF_ (STRU32)>> {
	using TYPE = STRU32 ;
} ;
} ;

template <class _ARG1>
using BYTE_TRAITS_TYPE = typename U::BYTE_TRAITS<ARGC<_SIZEOF_ (_ARG1)>>::TYPE ;

template <class _ARG1>
using TEXT_TRAITS_TYPE = typename U::TEXT_TRAITS<ARGC<_SIZEOF_ (_ARG1)>>::TYPE ;

using STRUA = TEXT_TRAITS_TYPE<STRA> ;
using STRUW = TEXT_TRAITS_TYPE<STRW> ;

template <class TYPE>
class EndianBytes :private Wrapped<BYTE[_SIZEOF_ (TYPE)]> {
private:
	inline static constexpr BYTE expr_endian_index (const ARGV<BYTE> &) {
		//@info: 'big endian' [0] -> 0X00
		return BYTE (0X00) ;
	}

	inline static constexpr WORD expr_endian_index (const ARGV<WORD> &) {
		//@info: 'big endian' [0] -> 0X00
		return WORD (0X0001) ;
	}

	inline static constexpr CHAR expr_endian_index (const ARGV<CHAR> &) {
		//@info: 'big endian' [0] -> 0X00
		return CHAR (0X00010203) ;
	}

	inline static constexpr DATA expr_endian_index (const ARGV<DATA> &) {
		//@info: 'big endian' [0] -> 0X00
		return DATA (0X0001020304050607) ;
	}

private:
	_STATIC_ASSERT_ (std::is_same<TYPE ,BYTE>::value || std::is_same<TYPE ,WORD>::value || std::is_same<TYPE ,CHAR>::value || std::is_same<TYPE ,DATA>::value) ;

public:
	inline LENGTH capacity () const {
		return _SIZEOF_ (TYPE) ;
	}

	inline const BYTE &operator[] (INDEX index) const {
		const auto r1x = expr_endian_index (_NULL_<const ARGV<TYPE>> ()) ;
		return EndianBytes::mData[_CAST_<BYTE[_SIZEOF_ (TYPE)]> (r1x)[index]] ;
	}

	template <class _RET ,class = ENABLE_TYPE<std::is_convertible<const TYPE & ,_RET>::value>>
	inline implicit operator _RET () const & {
		return _RET (big_endian_value ()) ;
	}

	template <class _RET>
	inline implicit operator _RET () && = delete ;

private:
	inline TYPE big_endian_value () const {
		TEMP<TYPE> ret ;
		_ZERO_ (ret) ;
		for (INDEX i = 0 ; i < capacity () ; i++)
			ret.unused[i] = (*this)[i] ;
		return std::move (_CAST_<TYPE> (ret)) ;
	}
} ;

class ByteReader {
private:
	class Attribute final :private Interface {
	private:
		friend ByteReader ;

	public:
		Attribute () {
			_STATIC_WARNING_ ("noop") ;
		}

		BYTE varify_ending_item () const {
			return 0 ;
		}
	} ;

private:
	SharedRef<Attribute> mHolder ;
	PhanBuffer<const BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteReader () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit ByteReader (const PhanBuffer<const BYTE> &stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const BYTE>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHolder ;
	}

	Attribute &attr () && = delete ;

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const BYTE> raw () const & {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const BYTE>::make (mStream ,length ()) ;
	}

	PhanBuffer<const BYTE> raw () && = delete ;

	void reset () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ib <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ie >= 0 && ie <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = ib ;
		mWrite = ie ;
	}

	ByteReader copy () popping {
		ByteReader ret ;
		ret.mHolder = mHolder ;
		ret.mStream = PhanBuffer<const BYTE>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	template <class _RET>
	_RET read () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_RET ret ;
		read (ret) ;
		return std::move (ret) ;
	}

	void read (BYTE &data) popping {
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (mRead >= mWrite)
				discard ;
			data = mStream[mRead] ;
			mRead++ ;
		} ,[&] (BOOL &if_flag) {
			data = mHolder->varify_ending_item () ;
		}) ;
	}

	inline ByteReader &operator>> (BYTE &data) popping {
		read (data) ;
		return *this ;
	}

	void read (WORD &data) popping {
		auto rax = PACK<BYTE[_SIZEOF_ (WORD)]> () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (rax.P1)) ; i++)
			read (rax.P1[i]) ;
		data = _CAST_<EndianBytes<WORD>> (rax) ;
	}

	inline ByteReader &operator>> (WORD &data) popping {
		read (data) ;
		return *this ;
	}

	void read (CHAR &data) popping {
		auto rax = PACK<BYTE[_SIZEOF_ (CHAR)]> () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (rax.P1)) ; i++)
			read (rax.P1[i]) ;
		data = _CAST_<EndianBytes<CHAR>> (rax) ;
	}

	inline ByteReader &operator>> (CHAR &data) popping {
		read (data) ;
		return *this ;
	}

	void read (DATA &data) popping {
		auto rax = PACK<BYTE[_SIZEOF_ (DATA)]> () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (rax.P1)) ; i++)
			read (rax.P1[i]) ;
		data = _CAST_<EndianBytes<DATA>> (rax) ;
	}

	inline ByteReader &operator>> (DATA &data) popping {
		read (data) ;
		return *this ;
	}

	void read (BOOL &data) popping {
		read (_CAST_<BYTE_TRAITS_TYPE<BOOL>> (data)) ;
	}

	inline ByteReader &operator>> (BOOL &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAR32 &data) popping {
		read (_CAST_<BYTE_TRAITS_TYPE<VAR32>> (data)) ;
	}

	inline ByteReader &operator>> (VAR32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAR64 &data) popping {
		read (_CAST_<BYTE_TRAITS_TYPE<VAR64>> (data)) ;
	}

	inline ByteReader &operator>> (VAR64 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL32 &data) popping {
		read (_CAST_<BYTE_TRAITS_TYPE<VAL32>> (data)) ;
	}

	inline ByteReader &operator>> (VAL32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL64 &data) popping {
		read (_CAST_<BYTE_TRAITS_TYPE<VAL64>> (data)) ;
	}

	inline ByteReader &operator>> (VAL64 &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Array<_ARG1 ,_ARG2> &data) popping {
		const auto r1x = LENGTH (read<VAR32> ()) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (data.size () < r1x)
			data = Array<_ARG1 ,_ARG2> (r1x) ;
		for (INDEX i = 0 ; i < r1x ; i++)
			read (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (Array<_ARG1 ,_ARG2> &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,LENGTH _VAL1>
	void read (const DEF<_ARG1[_VAL1]> &data) {
		_STATIC_ASSERT_ (stl::is_literals<_ARG1>::value) ;
		auto ris = copy () ;
		auto rax = _ARG1 () ;
		for (INDEX i = 0 ; i < _VAL1 - 1 ; i++) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data[i]) ;
		}
		*this = std::move (ris) ;
	}

	template <class _ARG1 ,LENGTH _VAL1>
	inline ByteReader &operator>> (const DEF<_ARG1[_VAL1]> &data) {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (String<_ARG1 ,_ARG2> &data) popping {
		_STATIC_ASSERT_ (stl::is_literals<_ARG1>::value) ;
		const auto r1x = LENGTH (read<VAR32> ()) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (data.size () < r1x)
			data = String<_ARG1 ,_ARG2> (r1x) ;
		data.clear () ;
		for (INDEX i = 0 ; i < r1x ; i++)
			read (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (String<_ARG1 ,_ARG2> &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Buffer<_ARG1 ,_ARG2> &data) popping {
		for (INDEX i = 0 ; i < data.size () ; i++)
			read (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (Buffer<_ARG1 ,_ARG2> &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<ByteReader> ())) ,void>::value>>
	void read (_ARG1 &data) popping {
		data.friend_read (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<ByteReader> ())) ,void>::value>>
	inline ByteReader &operator>> (_ARG1 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (const PTR<void (ByteReader &)> &proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline ByteReader &operator>> (const PTR<void (ByteReader &)> &proc) {
		read (proc) ;
		return *this ;
	}
} ;

class ByteWriter {
private:
	class Attribute final :private Interface {
	private:
		friend ByteWriter ;
		SharedRef<FixedBuffer<BYTE>> mBuffer ;

	public:
		Attribute () {
			_STATIC_WARNING_ ("noop") ;
		}

		BYTE varify_ending_item () const {
			return 0 ;
		}
	} ;

private:
	SharedRef<Attribute> mHolder ;
	PhanBuffer<BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteWriter () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit ByteWriter (const PhanBuffer<BYTE> &stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<BYTE>::make (stream) ;
		reset () ;
	}

	explicit ByteWriter (SharedRef<FixedBuffer<BYTE>> &&stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mBuffer = std::move (stream) ;
		mStream = PhanBuffer<BYTE>::make (mHolder->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHolder ;
	}

	Attribute &attr () && = delete ;

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const BYTE> raw () const & {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const BYTE>::make (mStream ,length ()) ;
	}

	PhanBuffer<const BYTE> raw () && = delete ;

	void reset () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ib <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ie >= 0 && ie <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = ib ;
		mWrite = ie ;
	}

	ByteWriter copy () popping {
		ByteWriter ret ;
		ret.mHolder = mHolder ;
		ret.mStream = PhanBuffer<BYTE>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void write (const BYTE &data) {
		_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
		mStream[mWrite] = data ;
		mWrite++ ;
	}

	inline ByteWriter &operator<< (const BYTE &data) {
		write (data) ;
		return *this ;
	}

	void write (const WORD &data) {
		auto &r1 = _CAST_<EndianBytes<WORD>> (data) ;
		for (INDEX i = 0 ; i < r1.capacity () ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const WORD &data) {
		write (data) ;
		return *this ;
	}

	void write (const CHAR &data) {
		auto &r1 = _CAST_<EndianBytes<CHAR>> (data) ;
		for (INDEX i = 0 ; i < r1.capacity () ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const CHAR &data) {
		write (data) ;
		return *this ;
	}

	void write (const DATA &data) {
		auto &r1 = _CAST_<EndianBytes<DATA>> (data) ;
		for (INDEX i = 0 ; i < r1.capacity () ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const DATA &data) {
		write (data) ;
		return *this ;
	}

	void write (const BOOL &data) {
		write (_CAST_<BYTE_TRAITS_TYPE<BOOL>> (data)) ;
	}

	inline ByteWriter &operator<< (const BOOL &data) {
		write (data) ;
		return *this ;
	}

	void write (const PTR<const void> &) = delete ;

	inline ByteWriter &operator<< (const PTR<const void> &) = delete ;

	void write (const VAR32 &data) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAR32>> (data)) ;
	}

	inline ByteWriter &operator<< (const VAR32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAR64 &data) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAR64>> (data)) ;
	}

	inline ByteWriter &operator<< (const VAR64 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL32 &data) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAL32>> (data)) ;
	}

	inline ByteWriter &operator<< (const VAL32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL64 &data) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAL64>> (data)) ;
	}

	inline ByteWriter &operator<< (const VAL64 &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Array<_ARG1 ,_ARG2> &data) {
		const auto r1x = data.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		write (VAR32 (r1x)) ;
		for (auto &&i : data)
			write (i) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteWriter &operator<< (const Array<_ARG1 ,_ARG2> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,LENGTH _VAL1>
	void write (const DEF<_ARG1[_VAL1]> &data) {
		_STATIC_ASSERT_ (stl::is_literals<_ARG1>::value) ;
		for (INDEX i = 0 ; i < _VAL1 - 1 ; i++)
			write (data[i]) ;
	}

	template <class _ARG1 ,LENGTH _VAL1>
	inline ByteWriter &operator<< (const DEF<_ARG1[_VAL1]> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const String<_ARG1 ,_ARG2> &data) {
		_STATIC_ASSERT_ (stl::is_literals<_ARG1>::value) ;
		const auto r1x = data.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		write (VAR32 (r1x)) ;
		for (auto &&i : data)
			write (i) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteWriter &operator<< (const String<_ARG1 ,_ARG2> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Buffer<_ARG1 ,_ARG2> &data) {
		for (INDEX i = 0 ; i < data.size () ; i++)
			write (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteWriter &operator<< (const Buffer<_ARG1 ,_ARG2> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<ByteWriter> ())) ,void>::value>>
	void write (const _ARG1 &data) {
		data.friend_write (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<ByteWriter> ())) ,void>::value>>
	inline ByteWriter &operator<< (const _ARG1 &data) {
		write (data) ;
		return *this ;
	}

	void write (const PTR<void (ByteWriter &)> &proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline ByteWriter &operator<< (const PTR<void (ByteWriter &)> &proc) {
		write (proc) ;
		return *this ;
	}
} ;

template <class STRX>
class TextReader {
private:
	class Attribute final :private Interface {
	private:
		friend TextReader ;
		BOOL mEndianFlag ;
		BOOL mEscapeFlag ;
		HashSet<STRX ,STRX> mEscapeSet ;
		HashSet<STRX ,VAR32> mSpaceSet ;

	public:
		Attribute () {
			enable_endian (FALSE) ;
			enable_escape (FALSE) ;
		}

		void enable_endian (BOOL flag) {
			mEndianFlag = flag ;
		}

		STRX convert_endian (const STRX &item) const {
			TEMP<STRX> ret ;
			const auto r1x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (STRX)]> &)>> (_MEMRCOPY_) ;
			const auto r2x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (STRX)]> &)>> (_MEMCOPY_) ;
			const auto r3x = mEndianFlag ? r1x : r2x ;
			r3x (PTRTOARR[&_ZERO_ (ret).unused[0]] ,_CAST_<BYTE[_SIZEOF_ (STRX)]> (item)) ;
			return std::move (_CAST_<STRX> (ret)) ;
		}

		STRX varify_ending_item () const {
			return STRX ('\0') ;
		}

		VAR64 varify_radix () const {
			return 10 ;
		}

		BOOL varify_number_item (const STRX &item) const {
			if (item < STRX ('0') || item > STRX ('9'))
				return FALSE ;
			return TRUE ;
		}

		VAR64 convert_number_r (const STRX &item) const {
			return VAR64 (item) - STRX ('0') ;
		}

		void enable_escape (BOOL flag) {
			mEscapeFlag = flag ;
		}

		STRX varify_escape_item () const {
			return STRX ('\\') ;
		}

		BOOL varify_escape_r (const STRX &item) const {
			if (!mEscapeFlag)
				return FALSE ;
			if (item != varify_escape_item ())
				return FALSE ;
			return TRUE ;
		}

		void modify_escape (const STRX &key ,const STRX &item) {
			_DEBUG_ASSERT_ (mEscapeSet.find (key) == VAR_NONE) ;
			mEscapeSet.add (key ,item) ;
		}

		STRX convert_escape (const STRX &key) const {
			INDEX ix = mEscapeSet.find (key) ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			return mEscapeSet[ix].item ;
		}

		BOOL varify_space (const STRX &item) const {
			INDEX ix = mSpaceSet.find (item) ;
			if (ix == VAR_NONE)
				return FALSE ;
			return TRUE ;
		}

		BOOL varify_space (const STRX &item ,VAR32 group) const {
			INDEX ix = mSpaceSet.find (item) ;
			if (ix == VAR_NONE)
				return FALSE ;
			if (mSpaceSet[ix].item != group)
				return FALSE ;
			return TRUE ;
		}

		void modify_space (const STRX &item) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,0) ;
		}

		void modify_space (const STRX &item ,VAR32 group) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,group) ;
		}

		BOOL varify_control (const STRX &item) const {
			const auto r1x = BOOL (item >= STRX (0) && item <= STRX (32)) ;
			const auto r2x = BOOL (item == STRX (127)) ;
			if (!r1x && !r2x)
				return FALSE ;
			if (varify_space (item))
				return FALSE ;
			return TRUE ;
		}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_literals<STRX>::value) ;
	SharedRef<Attribute> mHolder ;
	PhanBuffer<const STRX> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextReader () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit TextReader (const PhanBuffer<const STRX> &stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const STRX>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHolder ;
	}

	Attribute &attr () && = delete ;

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const STRX> raw () const & {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const STRX>::make (mStream ,length ()) ;
	}

	PhanBuffer<const STRX> raw () && = delete ;

	void reset () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ib <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ie >= 0 && ie <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = ib ;
		mWrite = ie ;
	}

	TextReader copy () popping {
		TextReader ret ;
		ret.mHolder = mHolder ;
		ret.mStream = PhanBuffer<const STRX>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	template <class _RET>
	_RET read () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_RET ret ;
		read (ret) ;
		return std::move (ret) ;
	}

	void read (STRX &data) popping {
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (mRead >= mWrite)
				discard ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				data = mHolder->convert_endian (mStream[mRead]) ;
				const auto r2x = mHolder->varify_escape_r (data) ;
				mRead++ ;
				if (!r2x)
					continue ;
				_DYNAMIC_ASSERT_ (mRead < mWrite) ;
				data = mHolder->convert_endian (mStream[mRead]) ;
				data = mHolder->convert_escape (data) ;
				mRead++ ;
			}
		} ,[&] (BOOL &if_flag) {
			data = mHolder->varify_ending_item () ;
		}) ;
	}

	inline TextReader &operator>> (STRX &data) popping {
		read (data) ;
		return *this ;
	}

	void read (BOOL &data) popping {
		auto ris = copy () ;
		auto rax = STRX () ;
		ris.read (rax) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (rax != STRX ('t'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('r')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('u')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('e')) ;
			data = TRUE ;
		} ,[&] (BOOL &if_flag) {
			if (rax != STRX ('T'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('R')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('U')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('E')) ;
			data = TRUE ;
		} ,[&] (BOOL &if_flag) {
			if (rax != STRX ('f'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('a')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('l')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('s')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('e')) ;
			data = FALSE ;
		} ,[&] (BOOL &if_flag) {
			if (rax != STRX ('F'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('A')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('L')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('S')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('E')) ;
			data = FALSE ;
		} ,[&] (BOOL &if_flag) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}) ;
		*this = std::move (ris) ;
	}

	inline TextReader &operator>> (BOOL &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAR32 &data) popping {
		const auto r1x = read<VAR64> () ;
		_DYNAMIC_ASSERT_ (r1x >= VAR32_MIN && r1x <= VAR32_MAX) ;
		data = VAR32 (r1x) ;
	}

	inline TextReader &operator>> (VAR32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAR64 &data) popping {
		auto ris = copy () ;
		auto rax = STRX () ;
		ris.read (rax) ;
		const auto r1x = BOOL (rax == STRX ('-')) ;
		if (rax == STRX ('+') || rax == STRX ('-'))
			ris.read (rax) ;
		compute_read_number (data ,ris ,rax) ;
		if (r1x)
			data = -data ;
		*this = std::move (ris) ;
	}

	inline TextReader &operator>> (VAR64 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL32 &data) popping {
		data = VAL32 (read<VAL64> ()) ;
	}

	inline TextReader &operator>> (VAL32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL64 &data) popping {
		auto ris = copy () ;
		auto rax = STRX () ;
		ris.read (rax) ;
		const auto r1x = BOOL (rax == STRX ('-')) ;
		if (rax == STRX ('+') || rax == STRX ('-'))
			ris.read (rax) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (rax != STRX ('i'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('n')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('f')) ;
			data = VAL64_INF ;
		} ,[&] (BOOL &if_flag) {
			if (rax != STRX ('I'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('N')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('F')) ;
			data = VAL64_INF ;
		} ,[&] (BOOL &if_flag) {
			if (rax != STRX ('n'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('a')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('n')) ;
			data = VAL64_NAN ;
		} ,[&] (BOOL &if_flag) {
			if (rax != STRX ('N'))
				discard ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('A')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('N')) ;
			data = VAL64_NAN ;
		} ,[&] (BOOL &if_flag) {
			const auto r2x = mHolder->varify_number_item (rax) ;
			if (!r2x)
				discard ;
			compute_read_number (data ,ris ,rax) ;
		} ,[&] (BOOL &if_flag) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}) ;
		if (r1x)
			data = -data ;
		*this = std::move (ris) ;
	}

	inline TextReader &operator>> (VAL64 &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Array<_ARG1 ,_ARG2> &data) popping {
		_DEBUG_ASSERT_ (FALSE) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextReader &operator>> (Array<_ARG1 ,_ARG2> &data) popping {
		read (data) ;
		return *this ;
	}

	template <LENGTH _VAL1>
	void read (const DEF<STRX[_VAL1]> &data) {
		_STATIC_ASSERT_ (stl::is_literals<STRX>::value) ;
		auto ris = copy () ;
		auto rax = STRX () ;
		for (INDEX i = 0 ; i < _VAL1 - 1 ; i++) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data[i]) ;
		}
		*this = std::move (ris) ;
	}

	template <LENGTH _VAL1>
	inline TextReader &operator>> (const DEF<STRX[_VAL1]> &data) {
		read (data) ;
		return *this ;
	}

	template <class _ARG1>
	void read (String<STRX ,_ARG1> &data) popping {
		const auto r1x = next_string_size () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (data.size () < r1x)
			data = String<STRX ,_ARG1> (r1x) ;
		data.clear () ;
		for (INDEX i = 0 ; i < r1x ; i++)
			read (data[i]) ;
	}

	template <class _ARG1>
	inline TextReader &operator>> (String<STRX ,_ARG1> &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Buffer<_ARG1 ,_ARG2> &data) popping {
		for (INDEX i = 0 ; i < data.size () ; i++)
			read (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextReader &operator>> (Buffer<_ARG1 ,_ARG2> &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<TextReader> ())) ,void>::value>>
	void read (_ARG1 &data) popping {
		data.friend_read (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<TextReader> ())) ,void>::value>>
	inline TextReader &operator>> (_ARG1 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (const PTR<void (TextReader &)> &proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline TextReader &operator>> (const PTR<void (TextReader &)> &proc) {
		read (proc) ;
		return *this ;
	}

private:
	LENGTH next_string_size () popping {
		LENGTH ret = 0 ;
		auto ris = copy () ;
		auto rax = STRX () ;
		ris.read (rax) ;
		while (TRUE) {
			if (rax == mHolder->varify_ending_item ())
				break ;
			if (mHolder->varify_space (rax))
				break ;
			ret++ ;
			ris.read (rax) ;
		}
		return std::move (ret) ;
	}

	void compute_read_number (VAR64 &data ,TextReader &reader ,STRX &top) const {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		_DEBUG_ASSERT_ (mHolder->varify_number_item (top)) ;
		auto ris = reader.copy () ;
		data = mHolder->convert_number_r (top) ;
		ris.read (top) ;
		while (TRUE) {
			if (!mHolder->varify_number_item (top))
				break ;
			const auto r1x = data * mHolder->varify_radix () + mHolder->convert_number_r (top) ;
			_DYNAMIC_ASSERT_ (data <= r1x) ;
			data = r1x ;
			reader = ris.copy () ;
			ris.read (top) ;
		}
	}

	void compute_read_number (VAL64 &data ,TextReader &reader ,STRX &top) const {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		_DEBUG_ASSERT_ (mHolder->varify_number_item (top)) ;
		auto ris = reader.copy () ;
		while (TRUE) {
			if (mHolder->convert_number_r (top) != 0)
				break ;
			reader = ris.copy () ;
			ris.read (top) ;
		}
		auto rax = ARRAY2<VAR64> {0 ,0} ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!mHolder->varify_number_item (top))
				continue ;
			rax[0] = mHolder->convert_number_r (top) ;
			reader = ris.copy () ;
			ris.read (top) ;
			while (TRUE) {
				if (!mHolder->varify_number_item (top))
					break ;
				_CALL_IF_ ([&] (BOOL &if_flag) {
					const auto r1x = rax[0] * mHolder->varify_radix () + mHolder->convert_number_r (top) ;
					if (rax[0] > r1x)
						discard ;
					rax[0] = r1x ;
				} ,[&] (BOOL &if_flag) {
					rax[1]++ ;
				}) ;
				reader = ris.copy () ;
				ris.read (top) ;
			}
		}
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (top != STRX ('.'))
				continue ;
			reader = ris.copy () ;
			ris.read (top) ;
			_DYNAMIC_ASSERT_ (mHolder->varify_number_item (top)) ;
			while (TRUE) {
				if (!mHolder->varify_number_item (top))
					break ;
				for (FOR_ONCE_DO_WHILE_FALSE) {
					const auto r2x = rax[0] * mHolder->varify_radix () + mHolder->convert_number_r (top) ;
					if (rax[0] > r2x)
						continue ;
					rax[0] = r2x ;
					rax[1]-- ;
				}
				reader = ris.copy () ;
				ris.read (top) ;
			}
		}
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (top != STRX ('e') && top != STRX ('E'))
				continue ;
			rax[1] += ris.template read<VAR32> () ;
			reader = ris.copy () ;
		}
		const auto r3x = _IEEE754_E10TOE2_ (rax) ;
		data = _IEEE754_ENCODE_ (r3x) ;
	}
} ;

template <class STRX>
class TextWriter {
private:
	class Attribute final :private Interface {
	private:
		friend TextWriter ;
		SharedRef<FixedBuffer<STRX>> mBuffer ;
		BOOL mEscapeFlag ;
		HashSet<STRX ,STRX> mEscapeSet ;

	public:
		Attribute () {
			enable_escape (FALSE) ;
		}

		STRX varify_ending_item () const {
			return STRX ('\0') ;
		}

		VAR64 varify_radix () const {
			return 10 ;
		}

		BOOL varify_number_item (const STRX &item) const {
			if (item < STRX ('0') || item > STRX ('9'))
				return FALSE ;
			return TRUE ;
		}

		STRX convert_number_w (VAR64 number) const {
			return STRX (STRX ('0') + number) ;
		}

		void enable_escape (BOOL flag) {
			mEscapeFlag = flag ;
		}

		STRX varify_escape_item () const {
			return STRX ('\\') ;
		}

		BOOL varify_escape_w (const STRX &key) const {
			if (!mEscapeFlag)
				return FALSE ;
			if (mEscapeSet.find (key) == VAR_NONE)
				return FALSE ;
			return TRUE ;
		}

		void modify_escape (const STRX &key ,const STRX &item) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mEscapeSet.find (key) == VAR_NONE) ;
			mEscapeSet.add (key ,item) ;
		}

		STRX convert_escape (const STRX &key) const {
			INDEX ix = mEscapeSet.find (key) ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			return mEscapeSet[ix].item ;
		}

		BOOL varify_space (const STRX &item) const {
			if (item != STRX ('\r') && item != STRX ('\n'))
				return FALSE ;
			return TRUE ;
		}

		BOOL varify_control (const STRX &item) const {
			const auto r1x = BOOL (item >= STRX (0) && item <= STRX (32)) ;
			const auto r2x = BOOL (item == STRX (127)) ;
			if (!r1x && !r2x)
				return FALSE ;
			if (varify_space (item))
				return FALSE ;
			return TRUE ;
		}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_literals<STRX>::value) ;
	SharedRef<Attribute> mHolder ;
	PhanBuffer<STRX> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextWriter () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit TextWriter (const PhanBuffer<STRX> &stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<STRX>::make (stream) ;
		reset () ;
	}

	explicit TextWriter (SharedRef<FixedBuffer<STRX>> &&stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mBuffer = std::move (stream) ;
		mStream = PhanBuffer<STRX>::make (mHolder->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHolder ;
	}

	Attribute &attr () && = delete ;

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const STRX> raw () const & {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const STRX>::make (mStream ,length ()) ;
	}

	PhanBuffer<const STRX> raw () && = delete ;

	void reset () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ib <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ie >= 0 && ie <= mStream.size ()) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		mRead = ib ;
		mWrite = ie ;
	}

	TextWriter copy () popping {
		TextWriter ret ;
		ret.mHolder = mHolder ;
		ret.mStream = PhanBuffer<STRX>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void write (const STRX &data) {
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (!mHolder->varify_escape_w (data))
				discard ;
			_DYNAMIC_ASSERT_ (mWrite + 1 < mStream.size ()) ;
			mStream[mWrite] = mHolder->varify_escape_item () ;
			mWrite++ ;
			const auto r1x = mHolder->convert_escape (data) ;
			mStream[mWrite] = r1x ;
			mWrite++ ;
		} ,[&] (BOOL &if_flag) {
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = data ;
			mWrite++ ;
		}) ;
	}

	inline TextWriter &operator<< (const STRX &data) {
		write (data) ;
		return *this ;
	}

	void write (const BOOL &data) {
		static constexpr auto M_TRUE = PACK<STRX[4]> ({
			STRX ('t') ,STRX ('r') ,STRX ('u') ,STRX ('e')}) ;
		static constexpr auto M_FALSE = PACK<STRX[5]> ({
			STRX ('f') ,STRX ('a') ,STRX ('l') ,STRX ('s') ,STRX ('e')}) ;
		const auto r1x = data ? (PhanBuffer<const STRX>::make (M_TRUE.P1)) : (PhanBuffer<const STRX>::make (M_FALSE.P1)) ;
		write (r1x) ;
	}

	inline TextWriter &operator<< (const BOOL &data) {
		write (data) ;
		return *this ;
	}

	void write (const PTR<const void> &) = delete ;

	inline TextWriter &operator<< (const PTR<const void> &) = delete ;

	void write (const VAR32 &data) {
		write (VAR64 (data)) ;
	}

	inline TextWriter &operator<< (const VAR32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAR64 &data) {
		auto rax = Buffer<STRX ,ARGC<128>> () ;
		INDEX iw = rax.size () ;
		compute_write_number (data ,PhanBuffer<STRX>::make (rax) ,iw) ;
		write (PhanBuffer<const STRX>::make (PTRTOARR[&rax.self[iw]] ,(rax.size () - iw))) ;
	}

	inline TextWriter &operator<< (const VAR64 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL32 &data) {
		static constexpr auto M_NAN = PACK<STRX[3]> ({
			STRX ('n') ,STRX ('a') ,STRX ('n')}) ;
		static constexpr auto M_INF = PACK<STRX[4]> ({
			STRX ('+') ,STRX ('i') ,STRX ('n') ,STRX ('f')}) ;
		static constexpr auto M_SINF = PACK<STRX[4]> ({
			STRX ('-') ,STRX ('i') ,STRX ('n') ,STRX ('f')}) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (!_ISNAN_ (data))
				discard ;
			write (PhanBuffer<const STRX>::make (M_NAN.P1)) ;
		} ,[&] (BOOL &if_flag) {
			if (!_ISINF_ (data))
				discard ;
			if (data < 0)
				discard ;
			write (PhanBuffer<const STRX>::make (M_INF.P1)) ;
		} ,[&] (BOOL &if_flag) {
			if (!_ISINF_ (data))
				discard ;
			if (data > 0)
				discard ;
			write (PhanBuffer<const STRX>::make (M_SINF.P1)) ;
		} ,[&] (BOOL &if_flag) {
			auto rax = Buffer<STRX ,ARGC<256>> () ;
			INDEX iw = rax.size () ;
			compute_write_number (data ,VAL32_PCS ,PhanBuffer<STRX>::make (rax) ,iw) ;
			write (PhanBuffer<const STRX>::make (PTRTOARR[&rax.self[iw]] ,(rax.size () - iw))) ;
		}) ;
	}

	inline TextWriter &operator<< (const VAL32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL64 &data) {
		static constexpr auto M_NAN = PACK<STRX[3]> ({
			STRX ('n') ,STRX ('a') ,STRX ('n')}) ;
		static constexpr auto M_INF = PACK<STRX[4]> ({
			STRX ('+') ,STRX ('i') ,STRX ('n') ,STRX ('f')}) ;
		static constexpr auto M_SINF = PACK<STRX[4]> ({
			STRX ('-') ,STRX ('i') ,STRX ('n') ,STRX ('f')}) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (!_ISNAN_ (data))
				discard ;
			write (PhanBuffer<const STRX>::make (M_NAN.P1)) ;
		} ,[&] (BOOL &if_flag) {
			if (!_ISINF_ (data))
				discard ;
			if (data < 0)
				discard ;
			write (PhanBuffer<const STRX>::make (M_INF.P1)) ;
		} ,[&] (BOOL &if_flag) {
			if (!_ISINF_ (data))
				discard ;
			if (data > 0)
				discard ;
			write (PhanBuffer<const STRX>::make (M_SINF.P1)) ;
		} ,[&] (BOOL &if_flag) {
			auto rax = Buffer<STRX ,ARGC<256>> () ;
			INDEX iw = rax.size () ;
			compute_write_number (data ,VAL64_PCS ,PhanBuffer<STRX>::make (rax) ,iw) ;
			write (PhanBuffer<const STRX>::make (PTRTOARR[&rax.self[iw]] ,(rax.size () - iw))) ;
		}) ;
	}

	inline TextWriter &operator<< (const VAL64 &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Array<_ARG1 ,_ARG2> &data) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextWriter &operator<< (const Array<_ARG1 ,_ARG2> &data) {
		write (data) ;
		return *this ;
	}

	template <LENGTH _VAL1>
	void write (const DEF<STRX[_VAL1]> &data) {
		_STATIC_ASSERT_ (stl::is_literals<STRX>::value) ;
		for (INDEX i = 0 ; i < _VAL1 - 1 ; i++)
			write (data[i]) ;
	}

	template <LENGTH _VAL1>
	inline TextWriter &operator<< (const DEF<STRX[_VAL1]> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1>
	void write (const String<STRX ,_ARG1> &data) {
		const auto r1x = data.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		for (auto &&i : data)
			write (i) ;
	}

	template <class _ARG1>
	inline TextWriter &operator<< (const String<STRX ,_ARG1> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Buffer<_ARG1 ,_ARG2> &data) {
		for (INDEX i = 0 ; i < data.size () ; i++)
			write (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextWriter &operator<< (const Buffer<_ARG1 ,_ARG2> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<TextWriter> ())) ,void>::value>>
	void write (const _ARG1 &data) {
		data.friend_write (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<TextWriter> ())) ,void>::value>>
	inline TextWriter &operator<< (const _ARG1 &data) {
		write (data) ;
		return *this ;
	}

	void write (const PTR<void (TextWriter &)> &proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline TextWriter &operator<< (const PTR<void (TextWriter &)> &proc) {
		write (proc) ;
		return *this ;
	}

private:
	void compute_write_number (const VAR64 &data ,const PhanBuffer<STRX> &out ,INDEX &it) const {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		INDEX iw = it ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (data <= 0)
				discard ;
			auto rax = data ;
			while (TRUE) {
				if (rax == 0)
					break ;
				out[--iw] = mHolder->convert_number_w (rax % mHolder->varify_radix ()) ;
				rax /= mHolder->varify_radix () ;
			}
		} ,[&] (BOOL &if_flag) {
			if (data >= 0)
				discard ;
			auto rax = data ;
			while (TRUE) {
				if (rax == 0)
					break ;
				out[--iw] = mHolder->convert_number_w (-rax % mHolder->varify_radix ()) ;
				rax /= mHolder->varify_radix () ;
			}
			out[--iw] = STRX ('-') ;
		} ,[&] (BOOL &if_flag) {
			out[--iw] = mHolder->convert_number_w (0) ;
		}) ;
		it = iw ;
	}

	void compute_write_number (const VAL64 &data ,LENGTH precision ,const PhanBuffer<STRX> &out ,INDEX &it) const {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		INDEX iw = it ;
		const auto r1x = _IEEE754_DECODE_ (data) ;
		auto rax = _IEEE754_E2TOE10_ (r1x) ;
		const auto r2x = BOOL (rax[0] < 0) ;
		if (r2x)
			rax[0] = -rax[0] ;
		const auto r3x = log_of_number (rax[0]) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (_ABS_ (r3x - 1 + rax[1]) < precision)
				discard ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				const auto r4x = _MAX_ ((r3x - 1 - precision) ,VAR_ZERO) ;
				for (INDEX i = 0 ,ie = r4x - 1 ; i < ie ; i++) {
					rax[0] /= mHolder->varify_radix () ;
					rax[1]++ ;
				}
				if (r4x <= 0)
					continue ;
				rax[0] = _ROUND_ (rax[0] ,mHolder->varify_radix ()) / mHolder->varify_radix () ;
				rax[1]++ ;
			}
		} ,[&] (BOOL &if_flag) {
			if (rax[1] < 1 - r3x)
				discard ;
			if (rax[1] >= 0)
				discard ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				const auto r10x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
				for (INDEX i = 0 ,ie = r10x - 1 ; i < ie ; i++) {
					rax[0] /= mHolder->varify_radix () ;
					rax[1]++ ;
				}
				if (r10x <= 0)
					continue ;
				rax[0] = _ROUND_ (rax[0] ,mHolder->varify_radix ()) / mHolder->varify_radix () ;
				rax[1]++ ;
			}
		} ,[&] (BOOL &if_flag) {
			if (rax[1] >= 1 - r3x)
				discard ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				const auto r9x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
				for (INDEX i = 0 ,ie = r9x - 1 ; i < ie ; i++) {
					rax[0] /= mHolder->varify_radix () ;
					rax[1]++ ;
				}
				if (r9x <= 0)
					continue ;
				rax[0] = _ROUND_ (rax[0] ,mHolder->varify_radix ()) / mHolder->varify_radix () ;
				rax[1]++ ;
			}
		}) ;
		const auto r8x = log_of_number (rax[0]) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			//@info: case 'x.xxxExxx'
			if (_ABS_ (r8x - 1 + rax[1]) < precision)
				discard ;
			compute_write_number ((r8x - 1 + rax[1]) ,out ,iw) ;
			if (out[iw] != STRX ('-'))
				out[--iw] = STRX ('+') ;
			out[--iw] = STRX ('e') ;
			const auto r5x = _MAX_ ((r8x - 1 - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mHolder->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ,ie = r8x - 1 ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
				rax[0] /= mHolder->varify_radix () ;
			}
			out[--iw] = STRX ('.') ;
			iw += EFLAG (out[ix] == STRX ('.')) ;
			out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
			rax[0] /= mHolder->varify_radix () ;
		} ,[&] (BOOL &if_flag) {
			//@info: case 'xxx000'
			if (rax[1] <= 0)
				discard ;
			for (INDEX i = 0 ,ie = LENGTH (rax[1]) ; i < ie ; i++)
				out[--iw] = mHolder->convert_number_w (0) ;
			for (INDEX i = 0 ; i < r8x ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				rax[0] /= mHolder->varify_radix () ;
			}
		} ,[&] (BOOL &if_flag) {
			//@info: case 'xxx.xxx'
			if (rax[1] < 1 - r8x)
				discard ;
			if (rax[1] >= 0)
				discard ;
			const auto r7x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r7x ; i++)
				rax[0] /= mHolder->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r7x ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
				rax[0] /= mHolder->varify_radix () ;
			}
			out[--iw] = STRX ('.') ;
			iw += EFLAG (out[ix] == STRX ('.')) ;
			for (INDEX i = 0 ,ie = LENGTH (r8x + rax[1]) ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				rax[0] /= mHolder->varify_radix () ;
			}
		} ,[&] (BOOL &if_flag) {
			//@info: case '0.000xxx'
			if (rax[1] >= 1 - r8x)
				discard ;
			if (rax[1] >= 0)
				discard ;
			const auto r6x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r6x ; i++)
				rax[0] /= mHolder->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r6x ; i < r8x ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
				rax[0] /= mHolder->varify_radix () ;
			}
			for (INDEX i = _MAX_ (r6x ,r8x) ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (0) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
			}
			out[--iw] = STRX ('.') ;
			iw += EFLAG (out[ix] == STRX ('.')) ;
			out[--iw] = mHolder->convert_number_w (0) ;
		} ,[&] (BOOL &if_flag) {
			//@info: case '0'
			if (rax[1] != 0)
				discard ;
			out[--iw] = mHolder->convert_number_w (0) ;
		}) ;
		if (r2x)
			out[--iw] = STRX ('-') ;
		it = iw ;
	}

	LENGTH log_of_number (VAR64 arg1) const {
		LENGTH ret = 0 ;
		for (VAR64 i = 1 ; i <= arg1 ; i *= mHolder->varify_radix ())
			ret++ ;
		return std::move (ret) ;
	}
} ;

inline namespace S {
inline void _CLS_ (ByteReader &reader) {
	reader.reset () ;
}

inline void _CLS_ (ByteWriter &writer) {
	writer.reset () ;
}

template <class _ARG1>
inline void _CLS_ (TextReader<_ARG1> &reader) {
	reader.reset () ;
}

template <class _ARG1>
inline void _CLS_ (TextWriter<_ARG1> &writer) {
	writer.reset () ;
}

template <class _ARG1>
inline void _BOM_ (TextReader<_ARG1> &reader) {
	_STATIC_WARNING_ ("noop") ;
}

inline void _BOM_ (TextReader<STRU8> &reader) {
	auto ris = reader.copy () ;
	auto rax = STRU8 () ;
	ris >> rax ;
	if (rax != STRU8 (0XEF))
		return ;
	ris >> rax ;
	if (rax != STRU8 (0XBB))
		return ;
	ris >> rax ;
	if (rax != STRU8 (0XBF))
		return ;
	ris.attr ().enable_endian (FALSE) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU16> &reader) {
	auto ris = reader.copy () ;
	auto rax = STRU16 () ;
	ris >> rax ;
	if (rax != STRU16 (0XFEFF) && rax != STRU16 (0XFFFE))
		return ;
	const auto r1x = BOOL (rax != 0XFEFF) ;
	ris.attr ().enable_endian (r1x) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU32> &reader) {
	auto ris = reader.copy () ;
	auto rax = STRU32 () ;
	ris >> rax ;
	if (rax != STRU32 (0X0000FEFF) && rax != STRU32 (0XFFFE0000))
		return ;
	const auto r1x = BOOL (rax != 0X0000FEFF) ;
	ris.attr ().enable_endian (r1x) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRW> &reader) {
	_BOM_ (_CAST_<TextReader<STRUW>> (reader)) ;
}

template <class _ARG1>
inline void _BOM_ (TextWriter<_ARG1> &writer) {
	_STATIC_WARNING_ ("noop") ;
}

inline void _BOM_ (TextWriter<STRU8> &writer) {
	static constexpr auto M_BOM = PACK<STRU8[3]> ({
		STRU8 (0XEF) ,STRU8 (0XBB) ,STRU8 (0XBF)}) ;
	writer << PhanBuffer<const STRU8>::make (M_BOM.P1) ;
}

inline void _BOM_ (TextWriter<STRU16> &writer) {
	static constexpr auto M_BOM = PACK<STRU16[1]> ({
		STRU16 (0XFEFF)}) ;
	writer << PhanBuffer<const STRU16>::make (M_BOM.P1) ;
}

inline void _BOM_ (TextWriter<STRU32> &writer) {
	static constexpr auto M_BOM = PACK<STRU32[1]> ({
		STRU32 (0X0000FEFF)}) ;
	writer << PhanBuffer<const STRU32>::make (M_BOM.P1) ;
}

inline void _BOM_ (TextWriter<STRA> &writer) {
	_STATIC_WARNING_ ("noop") ;
}

inline void _BOM_ (TextWriter<STRW> &writer) {
	_BOM_ (_CAST_<TextWriter<STRUW>> (writer)) ;
}

inline void _GAP_ (ByteReader &reader) {
	auto ris = reader.copy () ;
	auto rax = DATA () ;
	ris >> rax ;
	_DYNAMIC_ASSERT_ (rax == 0) ;
	reader = std::move (ris) ;
}

inline void _GAP_ (ByteWriter &writer) {
	writer << DATA (0) ;
}

template <class _ARG1>
inline void _GAP_ (TextReader<_ARG1> &reader) {
	auto ris = reader.copy () ;
	auto rax = _ARG1 () ;
	ris >> rax ;
	while (TRUE) {
		if (!ris.attr ().varify_space (rax))
			break ;
		reader = ris.copy () ;
		ris >> rax ;
	}
}

template <class _ARG1>
inline void _GAP_ (TextWriter<_ARG1> &writer) {
	_DYNAMIC_ASSERT_ (writer.length () + 2 < writer.size ()) ;
	writer.write (_ARG1 ('\r')) ;
	writer.write (_ARG1 ('\n')) ;
}

inline void _EOS_ (ByteReader &reader) {
	auto ris = reader.copy () ;
	auto rax = BYTE () ;
	while (TRUE) {
		if (ris.length () == 0)
			break ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == BYTE (0XFF)) ;
	}
	reader = std::move (ris) ;
}

inline void _EOS_ (ByteWriter &writer) {
	while (writer.length () < writer.size ())
		writer.write (BYTE (0XFF)) ;
}

template <class _ARG1>
inline void _EOS_ (TextReader<_ARG1> &reader) {
	auto ris = reader.copy () ;
	const auto r1x = ris.template read<_ARG1> () ;
	_DYNAMIC_ASSERT_ (r1x == reader.attr ().varify_ending_item ()) ;
	reader = std::move (ris) ;
}

template <class _ARG1>
inline void _EOS_ (TextWriter<_ARG1> &writer) {
	writer.write (writer.attr ().varify_ending_item ()) ;
}
} ;

template <class...>
class StreamBinder ;

template <>
class StreamBinder<> {
public:
	inline StreamBinder () = default ;

	inline StreamBinder (const StreamBinder &) = delete ;
	inline StreamBinder &operator= (const StreamBinder &) = delete ;

	inline StreamBinder (StreamBinder &&) noexcept = default ;
	inline StreamBinder &operator= (StreamBinder &&) = delete ;

	inline void friend_read (ByteReader &reader) const {
		_STATIC_WARNING_ ("noop") ;
	}

	inline void friend_write (ByteWriter &writer) const {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline void friend_read (TextReader<_ARG1> &reader) const {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline void friend_write (TextWriter<_ARG1> &writer) const {
		_STATIC_WARNING_ ("noop") ;
	}
} ;

template <class TYPE1 ,class... TYPES>
class StreamBinder<TYPE1 ,TYPES...> :private StreamBinder<TYPES...> {
private:
	_STATIC_ASSERT_ (!std::is_reference<TYPE1>::value) ;
	TYPE1 &mBase ;

public:
	inline StreamBinder () = delete ;

	inline explicit StreamBinder (TYPE1 &arg1 ,TYPES &...args) popping :StreamBinder<TYPES...> (args...) ,mBase (arg1) {}

	inline void friend_read (ByteReader &reader) const {
		reader >> mBase ;
		StreamBinder<TYPES...>::friend_read (reader) ;
	}

	inline void friend_write (ByteWriter &writer) const {
		writer << mBase ;
		StreamBinder<TYPES...>::friend_write (writer) ;
	}

	template <class _ARG1>
	inline void friend_read (TextReader<_ARG1> &reader) const {
		reader >> mBase ;
		StreamBinder<TYPES...>::friend_read (reader) ;
	}

	template <class _ARG1>
	inline void friend_write (TextWriter<_ARG1> &writer) const {
		writer << mBase ;
		StreamBinder<TYPES...>::friend_write (writer) ;
	}
} ;

template <class SIZE = void>
class LLTextReader ;

template <>
class LLTextReader<void> final :private Wrapped<void> {
private:
	template <class>
	friend class LLTextReader ;

	class HINT_HYPER_TEXT_TYPE ;
	class HINT_IDENTIFY_TEXT_TYPE ;
	class HINT_VALUE_TEXT_TYPE ;
	class HINT_STRING_TEXT_TYPE ;
	class HINT_NEWGAP_TEXT_TYPE ;
	class HINT_NEWLINE_TEXT_TYPE ;
	class SKIP_GAP_TYPE ;
	class SKIP_GAP_SPACE_ONLY_TYPE ;
	class SKIP_GAP_NEWLINE_ONLY_TYPE ;
	class SKIP_GAP_ENDLINE_TYPE ;

public:
	static constexpr auto HINT_HYPER_TEXT = ARGV<HINT_HYPER_TEXT_TYPE> {} ;
	static constexpr auto HINT_IDENTIFY_TEXT = ARGV<HINT_IDENTIFY_TEXT_TYPE> {} ;
	static constexpr auto HINT_VALUE_TEXT = ARGV<HINT_VALUE_TEXT_TYPE> {} ;
	static constexpr auto HINT_STRING_TEXT = ARGV<HINT_STRING_TEXT_TYPE> {} ;
	static constexpr auto HINT_NEWGAP_TEXT = ARGV<HINT_NEWGAP_TEXT_TYPE> {} ;
	static constexpr auto HINT_NEWLINE_TEXT = ARGV<HINT_NEWLINE_TEXT_TYPE> {} ;
	static constexpr auto SKIP_GAP = ARGV<SKIP_GAP_TYPE> {} ;
	static constexpr auto SKIP_GAP_SPACE_ONLY = ARGV<SKIP_GAP_SPACE_ONLY_TYPE> {} ;
	static constexpr auto SKIP_GAP_NEWLINE_ONLY = ARGV<SKIP_GAP_NEWLINE_ONLY_TYPE> {} ;
	static constexpr auto SKIP_GAP_ENDLINE = ARGV<SKIP_GAP_ENDLINE_TYPE> {} ;
} ;

template <class SIZE>
class LLTextReader {
private:
	class Shadow {
	private:
		TextReader<STRU8> mReader ;
		Array<STRU8 ,SIZE> mCache ;
		INDEX mRead ;

	public:
		inline Shadow () = delete ;

		inline explicit Shadow (LLTextReader &context) popping {
			mReader = context.mReader->copy () ;
			mCache = context.mCache ;
			mRead = context.mRead ;
		}

		inline const STRU8 &operator[] (INDEX index) const {
			_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
			_DEBUG_ASSERT_ (mRead >= 0 && mRead < mCache.length ()) ;
			return mCache[(mRead + index) % mCache.length ()] ;
		}

		inline void operator++ (int) {
			mReader.read (mCache[mRead]) ;
			mRead = (mRead + 1) % mCache.length () ;
		}
	} ;

private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	PhanRef<TextReader<STRU8>> mReader ;
	Array<STRU8 ,SIZE> mCache ;
	INDEX mRead ;
	BOOL mHintHyperTextFlag ;
	BOOL mHintStringTextFlag ;
	LENGTH mHintNextTextSize ;

public:
	LLTextReader () {
		mCache.fill (0) ;
		mRead = 0 ;
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = 0 ;
	}

	explicit LLTextReader (const PhanRef<TextReader<STRU8>> &reader) {
		_DEBUG_ASSERT_ (reader.exist ()) ;
		mReader = PhanRef<TextReader<STRU8>>::make (reader) ;
		auto &r1 = mReader->attr () ;
		r1.modify_space (STRU8 (' ') ,1) ;
		r1.modify_space (STRU8 ('\t') ,1) ;
		r1.modify_space (STRU8 ('\r') ,2) ;
		r1.modify_space (STRU8 ('\n') ,2) ;
		r1.modify_space (STRU8 ('\f') ,2) ;
		r1.modify_escape (STRU8 ('t') ,STRU8 ('\t')) ;
		r1.modify_escape (STRU8 ('r') ,STRU8 ('\r')) ;
		r1.modify_escape (STRU8 ('n') ,STRU8 ('\n')) ;
		r1.modify_escape (STRU8 ('f') ,STRU8 ('\f')) ;
		r1.modify_escape (STRU8 ('\"') ,STRU8 ('\"')) ;
		r1.modify_escape (STRU8 ('/') ,STRU8 ('/')) ;
		r1.modify_escape (STRU8 ('\\') ,STRU8 ('\\')) ;
		mReader.self >> _BOM_ ;
		for (INDEX i = 0 ; i < mCache.length () ; i++)
			mReader.self >> mCache[i] ;
		mRead = 0 ;
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = 0 ;
	}

	Shadow shadow () popping {
		return Shadow (*this) ;
	}

	const STRU8 &get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
		_DEBUG_ASSERT_ (mRead >= 0 && mRead < mCache.length ()) ;
		return mCache[(mRead + index) % mCache.length ()] ;
	}

	inline const STRU8 &operator[] (INDEX index) const & {
		return get (index) ;
	}

	const STRU8 &get (INDEX) && = delete ;

	inline const STRU8 &operator[] (INDEX) && = delete ;

	void read () {
		mReader.self >> mCache[mRead] ;
		mRead = (mRead + 1) % mCache.length () ;
	}

	inline void operator++ (int) {
		read () ;
	}

	template <LENGTH _VAL1>
	void read (const DEF<STRU8[_VAL1]> &data) {
		for (INDEX i = 0 ; i < _VAL1 - 1 ; i++) {
			_DYNAMIC_ASSERT_ (get (0) == data[i]) ;
			read () ;
		}
	}

	template <LENGTH _VAL1>
	inline LLTextReader &operator>> (const DEF<STRU8[_VAL1]> &data) {
		read (data) ;
		return *this ;
	}

	void read (ARGV<LLTextReader<void>::HINT_HYPER_TEXT_TYPE>) {
		mHintHyperTextFlag = TRUE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = 0 ;
	}

	void read (ARGV<LLTextReader<void>::HINT_IDENTIFY_TEXT_TYPE>) {
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_identifer_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_VALUE_TEXT_TYPE>) {
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_value_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_STRING_TEXT_TYPE>) {
		mHintHyperTextFlag = TRUE ;
		mHintStringTextFlag = TRUE ;
		mHintNextTextSize = next_string_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_NEWGAP_TEXT_TYPE>) {
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_gap_text_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_NEWLINE_TEXT_TYPE>) {
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_newline_text_size () ;
	}

	void read (ARGV<LLTextReader<void>::SKIP_GAP_TYPE>) {
		while (mReader->attr ().varify_space (get (0)))
			read () ;
	}

	void read (ARGV<LLTextReader<void>::SKIP_GAP_SPACE_ONLY_TYPE>) {
		while (mReader->attr ().varify_space (get (0) ,1))
			read () ;
	}

	void read (ARGV<LLTextReader<void>::SKIP_GAP_NEWLINE_ONLY_TYPE>) {
		while (mReader->attr ().varify_space (get (0) ,2))
			read () ;
	}

	void read (ARGV<LLTextReader<void>::SKIP_GAP_ENDLINE_TYPE>) {
		const auto r1x = get (0) ;
		_DYNAMIC_ASSERT_ (r1x == STRU8 ('\r') || r1x == STRU8 ('\n') || r1x == STRU8 ('\f')) ;
		read () ;
		if (r1x != STRU8 ('\r'))
			return ;
		const auto r2x = get (0) ;
		if (r2x != STRU8 ('\n'))
			return ;
		read () ;
	}

	template <class _ARG1>
	inline LLTextReader &operator>> (ARGV<_ARG1> data) {
		read (data) ;
		return *this ;
	}

	void read (String<STRU8> &data) popping {
		const auto r1x = _EXCHANGE_ (mHintHyperTextFlag ,FALSE) ;
		const auto r2x = _EXCHANGE_ (mHintStringTextFlag ,FALSE) ;
		const auto r3x = _EXCHANGE_ (mHintNextTextSize ,VAR_ZERO) ;
		if (data.size () < r3x)
			data = String<STRU8> (r3x) ;
		data.clear () ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r2x)
				continue ;
			_DYNAMIC_ASSERT_ (get (0) == STRU8 ('\"')) ;
			read () ;
		}
		for (INDEX i = 0 ; i < r3x ; i++) {
			data[i] = get (0) ;
			read () ;
			if (!r1x)
				continue ;
			_CALL_IF_ ([&] (BOOL &if_flag) {
				const auto r4x = BOOL (data[i] == mReader->attr ().varify_escape_item ()) ;
				if (!r4x)
					discard ;
				data[i] = get (0) ;
				read () ;
				data[i] = mReader->attr ().convert_escape (data[i]) ;
			} ,[&] (BOOL &if_flag) {
				_DYNAMIC_ASSERT_ (!mReader->attr ().varify_control (data[i])) ;
			}) ;
		}
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r2x)
				continue ;
			_DYNAMIC_ASSERT_ (get (0) == STRU8 ('\"')) ;
			read () ;
		}
	}

	inline LLTextReader &operator>> (String<STRU8> &data) popping {
		read (data) ;
		return *this ;
	}

	LENGTH next_identifer_size () popping {
		LENGTH ret = 0 ;
		auto ris = shadow () ;
		while (TRUE) {
			const auto r2x = BOOL (ris[0] >= STRU8 ('A') && ris[0] <= STRU8 ('Z')) ;
			const auto r3x = BOOL (ris[0] >= STRU8 ('a') && ris[0] <= STRU8 ('z')) ;
			const auto r4x = BOOL (ris[0] == STRU8 ('_')) ;
			_DYNAMIC_ASSERT_ (ret > 0 || r2x || r3x || r4x) ;
			const auto r5x = BOOL (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) ;
			const auto r6x = BOOL (ris[0] == STRU8 ('-') || ris[0] == STRU8 ('.') || ris[0] == STRU8 (':')) ;
			if (!r2x && !r3x && !r4x && !r5x && !r6x)
				break ;
			ris++ ;
			ret++ ;
		}
		return std::move (ret) ;
	}

	LENGTH next_value_size () popping {
		LENGTH ret = 0 ;
		auto ris = shadow () ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (ris[0] != STRU8 ('+') && ris[0] != STRU8 ('-'))
				continue ;
			ris++ ;
			ret++ ;
		}
		const auto r1x = ris[0] ;
		_DYNAMIC_ASSERT_ (r1x >= STRU8 ('0') && r1x <= STRU8 ('9')) ;
		ris++ ;
		ret++ ;
		while (TRUE) {
			if (r1x == STRU8 ('0'))
				break ;
			if (!(ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')))
				break ;
			ris++ ;
			ret++ ;
		}
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (ris[0] != STRU8 ('.'))
				continue ;
			ris++ ;
			ret++ ;
			while (TRUE) {
				if (!(ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')))
					break ;
				ris++ ;
				ret++ ;
			}
		}
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (ris[0] != STRU8 ('e') && ris[0] != STRU8 ('E'))
				continue ;
			ris++ ;
			ret++ ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				if (ris[0] != STRU8 ('+') && ris[0] != STRU8 ('-'))
					continue ;
				ris++ ;
				ret++ ;
			}
			_DYNAMIC_ASSERT_ (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) ;
			ris++ ;
			ret++ ;
			while (TRUE) {
				if (!(ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')))
					break ;
				ris++ ;
				ret++ ;
			}
		}
		return std::move (ret) ;
	}

	LENGTH next_string_size () popping {
		LENGTH ret = 0 ;
		auto ris = shadow () ;
		auto rax = STRU8 () ;
		_DYNAMIC_ASSERT_ (ris[0] == STRU8 ('\"')) ;
		ris++ ;
		while (TRUE) {
			if (ris[0] == STRU8 ('\0'))
				break ;
			if (ris[0] == STRU8 ('\"'))
				break ;
			rax = ris[0] ;
			ris++ ;
			_CALL_IF_ ([&] (BOOL &if_flag) {
				const auto r1x = BOOL (rax == mReader->attr ().varify_escape_item ()) ;
				if (!r1x)
					discard ;
				rax = ris[0] ;
				ris++ ;
				rax = mReader->attr ().convert_escape (rax) ;
			} ,[&] (BOOL &if_flag) {
				_DYNAMIC_ASSERT_ (!mReader->attr ().varify_control (rax)) ;
			}) ;
			ret++ ;
		}
		_DYNAMIC_ASSERT_ (ris[0] == STRU8 ('\"')) ;
		ris++ ;
		return std::move (ret) ;
	}

	LENGTH next_gap_text_size () popping {
		LENGTH ret = 0 ;
		auto ris = shadow () ;
		auto &r1 = mReader->attr () ;
		while (TRUE) {
			if (ris[0] == r1.varify_ending_item ())
				break ;
			if (r1.varify_space (ris[0]))
				break ;
			_DYNAMIC_ASSERT_ (!r1.varify_control (ris[0])) ;
			ris++ ;
			ret++ ;
		}
		return std::move (ret) ;
	}

	LENGTH next_newline_text_size () popping {
		LENGTH ret = 0 ;
		auto ris = shadow () ;
		auto &r1 = mReader->attr () ;
		while (TRUE) {
			if (ris[0] == r1.varify_ending_item ())
				break ;
			if (r1.varify_space (ris[0] ,2))
				break ;
			_DYNAMIC_ASSERT_ (!r1.varify_control (ris[0])) ;
			ris++ ;
			ret++ ;
		}
		return std::move (ret) ;
	}
} ;
} ;