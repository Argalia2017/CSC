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

template <class _ARG>
using BYTE_TRAITS_TYPE = typename U::BYTE_TRAITS<ARGC<_SIZEOF_ (_ARG)>>::TYPE ;

template <class _ARG>
using TEXT_TRAITS_TYPE = typename U::TEXT_TRAITS<ARGC<_SIZEOF_ (_ARG)>>::TYPE ;

using STRUA = TEXT_TRAITS_TYPE<STRA> ;
using STRUW = TEXT_TRAITS_TYPE<STRW> ;

template <class TYPE>
class EndianBytes :private Wrapped<BYTE[_SIZEOF_ (TYPE)]> {
private:
	inline static constexpr BYTE expr_endian_value (const ARGV<BYTE> &) {
		//@info: 'big endian' [0] -> 0X00
		return BYTE (0X00) ;
	}

	inline static constexpr WORD expr_endian_value (const ARGV<WORD> &) {
		//@info: 'big endian' [0] -> 0X00
		return WORD (0X0001) ;
	}

	inline static constexpr CHAR expr_endian_value (const ARGV<CHAR> &) {
		//@info: 'big endian' [0] -> 0X00
		return CHAR (0X00010203) ;
	}

	inline static constexpr DATA expr_endian_value (const ARGV<DATA> &) {
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
		const auto r1x = expr_endian_value (_NULL_<const ARGV<TYPE>> ()) ;
		return EndianBytes::mData[_CAST_<BYTE[_SIZEOF_ (TYPE)]> (r1x)[index]] ;
	}

	inline TYPE merge () const {
		TEMP<TYPE> ret ;
		_ZERO_ (ret) ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (EndianBytes::mData)) ; i++)
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

	Attribute &attr () const {
		return mHolder ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const BYTE> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const BYTE>::make (mStream ,length ()) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ie >= 0) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
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
		if (mRead >= mWrite) {
			data = mHolder->varify_ending_item () ;
		} else {
			data = mStream[mRead] ;
			mRead++ ;
		}
	}

	inline ByteReader &operator>> (BYTE &data) popping {
		read (data) ;
		return *this ;
	}

	void read (WORD &data) popping {
		auto rax = PACK<BYTE[_SIZEOF_ (WORD)]> () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (rax.P1)) ; i++)
			read (rax.P1[i]) ;
		data = _CAST_<EndianBytes<WORD>> (rax).merge () ;
	}

	inline ByteReader &operator>> (WORD &data) popping {
		read (data) ;
		return *this ;
	}

	void read (CHAR &data) popping {
		auto rax = PACK<BYTE[_SIZEOF_ (CHAR)]> () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (rax.P1)) ; i++)
			read (rax.P1[i]) ;
		data = _CAST_<EndianBytes<CHAR>> (rax).merge () ;
	}

	inline ByteReader &operator>> (CHAR &data) popping {
		read (data) ;
		return *this ;
	}

	void read (DATA &data) popping {
		auto rax = PACK<BYTE[_SIZEOF_ (DATA)]> () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (rax.P1)) ; i++)
			read (rax.P1[i]) ;
		data = _CAST_<EndianBytes<DATA>> (rax).merge () ;
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
		for (INDEX i = 0 ; i < data.size () ; i++)
			read (data[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (Array<_ARG1 ,_ARG2> &data) popping {
		read (data) ;
		return *this ;
	}

	template <class _ARG ,LENGTH _VAL>
	void read (const DEF<_ARG[_VAL]> &data) {
		_STATIC_ASSERT_ (stl::is_literals<_ARG>::value) ;
		_STATIC_ASSERT_ (_VAL <= VAR32_MAX) ;
		auto ris = copy () ;
		auto rax = _ARG () ;
		for (INDEX i = 0 ; i < _VAL - 1 ; i++) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data[i]) ;
		}
		*this = std::move (ris) ;
	}

	template <class _ARG ,LENGTH _VAL>
	inline ByteReader &operator>> (const DEF<_ARG[_VAL]> &data) {
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
		for (INDEX i = 0 ; i < data.size () ; i++)
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

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<ByteReader> ())) ,void>::value>>
	void read (_ARG &data) popping {
		data.friend_read (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<ByteReader> ())) ,void>::value>>
	inline ByteReader &operator>> (_ARG &data) popping {
		read (data) ;
		return *this ;
	}

	void read (PTR<void (ByteReader &)> proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline ByteReader &operator>> (PTR<void (ByteReader &)> proc) {
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

	Attribute &attr () const {
		return mHolder ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const BYTE> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const BYTE>::make (mStream ,length ()) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ie >= 0) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
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

	template <class _ARG ,LENGTH _VAL>
	void write (const DEF<_ARG[_VAL]> &data) {
		_STATIC_ASSERT_ (stl::is_literals<_ARG>::value) ;
		_STATIC_ASSERT_ (_VAL <= VAR32_MAX) ;
		for (INDEX i = 0 ; i < _VAL - 1 ; i++)
			write (data[i]) ;
	}

	template <class _ARG ,LENGTH _VAL>
	inline ByteWriter &operator<< (const DEF<_ARG[_VAL]> &data) {
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

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<ByteWriter> ())) ,void>::value>>
	void write (const _ARG &data) {
		data.friend_write (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<ByteWriter> ())) ,void>::value>>
	inline ByteWriter &operator<< (const _ARG &data) {
		write (data) ;
		return *this ;
	}

	void write (PTR<void (ByteWriter &)> proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline ByteWriter &operator<< (PTR<void (ByteWriter &)> proc) {
		write (proc) ;
		return *this ;
	}
} ;

template <class UNIT>
class TextReader {
private:
	class Attribute final :private Interface {
	private:
		friend TextReader ;
		BOOL mEndianFlag ;
		BOOL mEscapeFlag ;
		HashSet<UNIT ,UNIT> mEscapeSet ;
		HashSet<UNIT ,VAR32> mSpaceSet ;

	public:
		Attribute () {
			enable_endian (FALSE) ;
			enable_escape (FALSE) ;
		}

		void enable_endian (BOOL flag) {
			mEndianFlag = flag ;
		}

		UNIT convert_endian (const UNIT &arg) const {
			TEMP<UNIT> ret ;
			const auto r1x = mEndianFlag ? (_XVALUE_<const PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (UNIT)]> &)> &> (&_MEMRCOPY_<BYTE ,_SIZEOF_ (UNIT)>)) : (_XVALUE_<const PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (UNIT)]> &)> &> (&_MEMCOPY_<BYTE ,_SIZEOF_ (UNIT)>)) ;
			r1x (PTRTOARR[&_ZERO_ (ret).unused[0]] ,_CAST_<BYTE[_SIZEOF_ (UNIT)]> (arg)) ;
			return std::move (_CAST_<UNIT> (ret)) ;
		}

		UNIT varify_ending_item () const {
			return UNIT ('\0') ;
		}

		VAR64 varify_radix () const {
			return 10 ;
		}

		BOOL varify_number_item (const UNIT &item) const {
			return item >= UNIT ('0') && item <= UNIT ('9') ;
		}

		VAR64 convert_number_r (const UNIT &item) const {
			return VAR64 (item) - UNIT ('0') ;
		}

		void enable_escape (BOOL flag) {
			mEscapeFlag = flag ;
		}

		UNIT varify_escape_item () const {
			return UNIT ('\\') ;
		}

		BOOL varify_escape_r (const UNIT &item) const {
			return mEscapeFlag && item == varify_escape_item () ;
		}

		void modify_escape (const UNIT &key ,const UNIT &item) {
			_DEBUG_ASSERT_ (mEscapeSet.find (key) == VAR_NONE) ;
			mEscapeSet.add (key ,item) ;
		}

		UNIT convert_escape (const UNIT &key) const {
			INDEX ix = mEscapeSet.find (key) ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			return mEscapeSet[ix].item ;
		}

		BOOL varify_space (const UNIT &item) const {
			return mSpaceSet.find (item) != VAR_NONE ;
		}

		BOOL varify_space (const UNIT &item ,VAR32 group) const {
			INDEX ix = mSpaceSet.find (item) ;
			return ix != VAR_NONE && mSpaceSet[ix].item == group ;
		}

		void modify_space (const UNIT &item) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,0) ;
		}

		void modify_space (const UNIT &item ,VAR32 group) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,group) ;
		}

		BOOL varify_control (const UNIT &item) const {
			const auto r1x = (item >= UNIT (0) && item <= UNIT (32)) || item == UNIT (127) ;
			const auto r2x = varify_space (item) ;
			return r1x && !r2x ;
		}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_literals<UNIT>::value) ;
	SharedRef<Attribute> mHolder ;
	PhanBuffer<const UNIT> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextReader () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit TextReader (const PhanBuffer<const UNIT> &stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const UNIT>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const {
		return mHolder ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const UNIT> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const UNIT>::make (mStream ,length ()) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ie >= 0) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
		mRead = ib ;
		mWrite = ie ;
	}

	TextReader copy () popping {
		TextReader ret ;
		ret.mHolder = mHolder ;
		ret.mStream = PhanBuffer<const UNIT>::make (mStream) ;
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

	void read (UNIT &data) popping {
		const auto r1x = mRead < mWrite ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r1x)
				continue ;
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
		if (r1x)
			return ;
		data = mHolder->varify_ending_item () ;
	}

	inline TextReader &operator>> (UNIT &data) popping {
		read (data) ;
		return *this ;
	}

	void read (BOOL &data) popping {
		auto ris = copy () ;
		auto rax = UNIT () ;
		ris.read (rax) ;
		_DYNAMIC_ASSERT_ (rax == UNIT ('t') || rax == UNIT ('T') || rax == UNIT ('f') || rax == UNIT ('F')) ;
		if (rax == UNIT ('t')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('r')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('u')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('e')) ;
			data = TRUE ;
		} else if (rax == UNIT ('T')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('R')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('U')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('E')) ;
			data = TRUE ;
		} else if (rax == UNIT ('f')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('a')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('l')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('s')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('e')) ;
			data = FALSE ;
		} else if (rax == UNIT ('F')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('A')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('L')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('S')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('E')) ;
			data = FALSE ;
		}
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
		auto rax = UNIT () ;
		ris.read (rax) ;
		const auto r1x = rax == UNIT ('-') ;
		if (rax == UNIT ('+') || rax == UNIT ('-'))
			ris.read (rax) ;
		ris.try_read_number (data ,rax) ;
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
		auto rax = UNIT () ;
		ris.read (rax) ;
		const auto r1x = rax == UNIT ('-') ;
		if (rax == UNIT ('+') || rax == UNIT ('-'))
			ris.read (rax) ;
		const auto r2x = mHolder->varify_number_item (rax) ;
		_DYNAMIC_ASSERT_ (rax == UNIT ('i') || rax == UNIT ('I') || rax == UNIT ('n') || rax == UNIT ('N') || r2x) ;
		if (rax == UNIT ('i')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('n')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('f')) ;
			data = VAL64_INF ;
		} else if (rax == UNIT ('I')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('N')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('F')) ;
			data = VAL64_INF ;
		} else if (rax == UNIT ('n')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('a')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('n')) ;
			data = VAL64_NAN ;
		} else if (rax == UNIT ('N')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('A')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('N')) ;
			data = VAL64_NAN ;
		} else if (r2x) {
			ris.try_read_number (data ,rax) ;
		}
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

	template <LENGTH _VAL>
	void read (const DEF<UNIT[_VAL]> &data) {
		_STATIC_ASSERT_ (_VAL <= VAR32_MAX) ;
		auto ris = copy () ;
		auto rax = UNIT () ;
		for (INDEX i = 0 ; i < _VAL - 1 ; i++) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data[i]) ;
		}
		*this = std::move (ris) ;
	}

	template <LENGTH _VAL>
	inline TextReader &operator>> (const DEF<UNIT[_VAL]> &data) {
		read (data) ;
		return *this ;
	}

	template <class _ARG>
	void read (String<UNIT ,_ARG> &data) popping {
		const auto r1x = next_string_size () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (data.size () < r1x)
			data = String<UNIT ,_ARG> (r1x) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			read (data[i]) ;
	}

	template <class _ARG>
	inline TextReader &operator>> (String<UNIT ,_ARG> &data) popping {
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

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<TextReader> ())) ,void>::value>>
	void read (_ARG &data) popping {
		data.friend_read (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<TextReader> ())) ,void>::value>>
	inline TextReader &operator>> (_ARG &data) popping {
		read (data) ;
		return *this ;
	}

	void read (PTR<void (TextReader &)> proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline TextReader &operator>> (PTR<void (TextReader &)> proc) {
		read (proc) ;
		return *this ;
	}

private:
	LENGTH next_string_size () popping {
		LENGTH ret = 0 ;
		auto ris = copy () ;
		auto rax = UNIT () ;
		ris.read (rax) ;
		while (rax != mHolder->varify_ending_item () && !mHolder->varify_space (rax)) {
			ret++ ;
			ris.read (rax) ;
		}
		return std::move (ret) ;
	}

	void try_read_number (VAR64 &data ,const UNIT &first) popping {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		_DEBUG_ASSERT_ (mHolder->varify_number_item (first)) ;
		auto ris = copy () ;
		auto rax = first ;
		data = mHolder->convert_number_r (rax) ;
		ris.read (rax) ;
		while (mHolder->varify_number_item (rax)) {
			const auto r1x = data * mHolder->varify_radix () + mHolder->convert_number_r (rax) ;
			_DYNAMIC_ASSERT_ (data <= r1x) ;
			data = r1x ;
			*this = ris.copy () ;
			ris.read (rax) ;
		}
	}

	void try_read_number (VAL64 &data ,const UNIT &first) popping {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		_DEBUG_ASSERT_ (mHolder->varify_number_item (first)) ;
		auto ris = copy () ;
		auto rax = first ;
		while (mHolder->convert_number_r (rax) == 0) {
			*this = ris.copy () ;
			ris.read (rax) ;
		}
		ARRAY2<VAR64> rbx = ARRAY2<VAR64> {0 ,0} ;
		try_read_number_digital (rbx ,ris ,rax) ;
		try_read_number_decimal (rbx ,ris ,rax) ;
		if (rax == UNIT ('e') || rax == UNIT ('E')) {
			rbx[1] += ris.template read<VAR32> () ;
			*this = ris.copy () ;
		}
		const auto r3x = _IEEE754E10TOE2_ (rbx) ;
		data = _IEEE754ENCODE_ (r3x) ;
	}

	void try_read_number_digital (ARRAY2<VAR64> &data ,TextReader &reader ,UNIT &top) popping {
		if (!mHolder->varify_number_item (top))
			return ;
		data[0] = mHolder->convert_number_r (top) ;
		*this = reader.copy () ;
		reader.read (top) ;
		while (mHolder->varify_number_item (top)) {
			const auto r1x = data[0] * mHolder->varify_radix () + mHolder->convert_number_r (top) ;
			if (data[0] <= r1x) {
				data[0] = r1x ;
			} else {
				data[1]++ ;
			}
			*this = reader.copy () ;
			reader.read (top) ;
		}
	}

	void try_read_number_decimal (ARRAY2<VAR64> &data ,TextReader &reader ,UNIT &top) popping {
		if (top != UNIT ('.'))
			return ;
		*this = reader.copy () ;
		reader.read (top) ;
		_DYNAMIC_ASSERT_ (mHolder->varify_number_item (top)) ;
		while (mHolder->varify_number_item (top)) {
			const auto r1x = data[0] * mHolder->varify_radix () + mHolder->convert_number_r (top) ;
			if (data[0] <= r1x) {
				data[0] = r1x ;
				data[1]-- ;
			}
			*this = reader.copy () ;
			reader.read (top) ;
		}
	}
} ;

template <class UNIT>
class TextWriter {
private:
	class Attribute final :private Interface {
	private:
		friend TextWriter ;
		SharedRef<FixedBuffer<UNIT>> mBuffer ;
		BOOL mEscapeFlag ;
		HashSet<UNIT ,UNIT> mEscapeSet ;

	public:
		Attribute () {
			enable_escape (FALSE) ;
		}

		UNIT varify_ending_item () const {
			return UNIT ('\0') ;
		}

		VAR64 varify_radix () const {
			return 10 ;
		}

		BOOL varify_number_item (const UNIT &item) const {
			return item >= UNIT ('0') && item <= UNIT ('9') ;
		}

		UNIT convert_number_w (VAR64 number) const {
			return UNIT (UNIT ('0') + number) ;
		}

		void enable_escape (BOOL flag) {
			mEscapeFlag = flag ;
		}

		UNIT varify_escape_item () const {
			return UNIT ('\\') ;
		}

		BOOL varify_escape_w (const UNIT &key) const {
			return mEscapeFlag && mEscapeSet.find (key) != VAR_NONE ;
		}

		void modify_escape (const UNIT &key ,const UNIT &item) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mEscapeSet.find (key) == VAR_NONE) ;
			mEscapeSet.add (key ,item) ;
		}

		UNIT convert_escape (const UNIT &key) const {
			INDEX ix = mEscapeSet.find (key) ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			return mEscapeSet[ix].item ;
		}

		BOOL varify_space (const UNIT &item) const {
			return item == UNIT ('\r') || item == UNIT ('\n') ;
		}

		BOOL varify_control (const UNIT &item) const {
			const auto r1x = (item >= UNIT (0) && item <= UNIT (32)) || item == UNIT (127) ;
			const auto r2x = varify_space (item) ;
			return r1x && !r2x ;
		}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_literals<UNIT>::value) ;
	SharedRef<Attribute> mHolder ;
	PhanBuffer<UNIT> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextWriter () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit TextWriter (const PhanBuffer<UNIT> &stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<UNIT>::make (stream) ;
		reset () ;
	}

	explicit TextWriter (SharedRef<FixedBuffer<UNIT>> &&stream) {
		mHolder = SharedRef<Attribute>::make () ;
		mHolder->mBuffer = std::move (stream) ;
		mStream = PhanBuffer<UNIT>::make (mHolder->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const {
		return mHolder ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		if (mStream.size () == 0)
			return 0 ;
		return mWrite - mRead ;
	}

	PhanBuffer<const UNIT> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const UNIT>::make (mStream ,length ()) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX ib ,INDEX ie) {
		_DEBUG_ASSERT_ (ib >= 0 && ie >= 0) ;
		_DEBUG_ASSERT_ (ib <= ie) ;
		mRead = ib ;
		mWrite = ie ;
	}

	TextWriter copy () popping {
		TextWriter ret ;
		ret.mHolder = mHolder ;
		ret.mStream = PhanBuffer<UNIT>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void write (const UNIT &data) {
		if (mHolder->varify_escape_w (data)) {
			_DYNAMIC_ASSERT_ (mWrite + 1 < mStream.size ()) ;
			mStream[mWrite] = mHolder->varify_escape_item () ;
			mWrite++ ;
			const auto r1x = mHolder->convert_escape (data) ;
			mStream[mWrite] = r1x ;
			mWrite++ ;
		} else {
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = data ;
			mWrite++ ;
		}
	}

	inline TextWriter &operator<< (const UNIT &data) {
		write (data) ;
		return *this ;
	}

	void write (const BOOL &data) {
		static constexpr auto M_TRUE = PACK<UNIT[4]> ({
			UNIT ('t') ,UNIT ('r') ,UNIT ('u') ,UNIT ('e')}) ;
		static constexpr auto M_FALSE = PACK<UNIT[5]> ({
			UNIT ('f') ,UNIT ('a') ,UNIT ('l') ,UNIT ('s') ,UNIT ('e')}) ;
		const auto r1x = data ? (PhanBuffer<const UNIT>::make (M_TRUE.P1)) : (PhanBuffer<const UNIT>::make (M_FALSE.P1)) ;
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
		auto rax = Buffer<UNIT ,ARGC<128>> () ;
		INDEX iw = rax.size () ;
		try_write_number (data ,PhanBuffer<UNIT>::make (rax) ,iw) ;
		write (PhanBuffer<const UNIT>::make (PTRTOARR[&rax.self[iw]] ,(rax.size () - iw))) ;
	}

	inline TextWriter &operator<< (const VAR64 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL32 &data) {
		write (VAL64 (data)) ;
	}

	inline TextWriter &operator<< (const VAL32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL64 &data) {
		static constexpr auto M_NAN = PACK<UNIT[3]> ({
			UNIT ('n') ,UNIT ('a') ,UNIT ('n')}) ;
		static constexpr auto M_INF = PACK<UNIT[4]> ({
			UNIT ('+') ,UNIT ('i') ,UNIT ('n') ,UNIT ('f')}) ;
		static constexpr auto M_SINF = PACK<UNIT[4]> ({
			UNIT ('-') ,UNIT ('i') ,UNIT ('n') ,UNIT ('f')}) ;
		if (_ISNAN_ (data)) {
			write (PhanBuffer<const UNIT>::make (M_NAN.P1)) ;
		} else if (data > 0 && _ISINF_ (data)) {
			write (PhanBuffer<const UNIT>::make (M_INF.P1)) ;
		} else if (data < 0 && _ISINF_ (data)) {
			write (PhanBuffer<const UNIT>::make (M_SINF.P1)) ;
		} else {
			auto rax = Buffer<UNIT ,ARGC<256>> () ;
			INDEX iw = rax.size () ;
			try_write_number (data ,PhanBuffer<UNIT>::make (rax) ,iw) ;
			write (PhanBuffer<const UNIT>::make (PTRTOARR[&rax.self[iw]] ,(rax.size () - iw))) ;
		}
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

	template <LENGTH _VAL>
	void write (const DEF<UNIT[_VAL]> &data) {
		_STATIC_ASSERT_ (_VAL <= VAR32_MAX) ;
		for (INDEX i = 0 ; i < _VAL - 1 ; i++)
			write (data[i]) ;
	}

	template <LENGTH _VAL>
	inline TextWriter &operator<< (const DEF<UNIT[_VAL]> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG>
	void write (const String<UNIT ,_ARG> &data) {
		const auto r1x = data.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		for (auto &&i : data)
			write (i) ;
	}

	template <class _ARG>
	inline TextWriter &operator<< (const String<UNIT ,_ARG> &data) {
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

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<TextWriter> ())) ,void>::value>>
	void write (const _ARG &data) {
		data.friend_write (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<TextWriter> ())) ,void>::value>>
	inline TextWriter &operator<< (const _ARG &data) {
		write (data) ;
		return *this ;
	}

	void write (PTR<void (TextWriter &)> proc) {
		_DEBUG_ASSERT_ (proc != NULL) ;
		proc (*this) ;
	}

	inline TextWriter &operator<< (PTR<void (TextWriter &)> proc) {
		write (proc) ;
		return *this ;
	}

private:
	void try_write_number (const VAR64 &data ,const PhanBuffer<UNIT> &out ,INDEX &it) const {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		INDEX iw = it ;
		if (data > 0) {
			auto rax = data ;
			while (rax != 0) {
				out[--iw] = mHolder->convert_number_w (rax % mHolder->varify_radix ()) ;
				rax /= mHolder->varify_radix () ;
			}
		} else if (data < 0) {
			auto rax = data ;
			while (rax != 0) {
				out[--iw] = mHolder->convert_number_w (-rax % mHolder->varify_radix ()) ;
				rax /= mHolder->varify_radix () ;
			}
			out[--iw] = UNIT ('-') ;
		} else {
			out[--iw] = mHolder->convert_number_w (0) ;
		}
		it = iw ;
	}

	void try_write_number (const VAL64 &data ,const PhanBuffer<UNIT> &out ,INDEX &it) const {
		_DEBUG_ASSERT_ (mHolder->varify_radix () == 10) ;
		INDEX iw = it ;
		const auto r1x = _IEEE754DECODE_ (data) ;
		auto rax = _IEEE754E2TOE10_ (r1x) ;
		const auto r2x = rax[0] < 0 ;
		rax[0] = r2x ? (-rax[0]) : (rax[0]) ;
		const auto r3x = log_of_number (rax[0]) ;
		if (_ABS_ (r3x - 1 + rax[1]) >= 6) {
			const auto r4x = _MAX_ ((r3x - 7) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r4x - 1 ; i++) {
				rax[0] /= mHolder->varify_radix () ;
				rax[1]++ ;
			}
			_STATIC_WARNING_ ("unqualified") ;
			if (r4x > 0) {
				rax[0] = (rax[0] + mHolder->varify_radix () / 2) / mHolder->varify_radix () ;
				rax[1]++ ;
			}
			_STATIC_WARNING_ ("unqualified") ;
		} else if (rax[1] >= 1 - r3x && rax[1] < 0) {
			const auto r4x = _MAX_ (LENGTH (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r4x - 1 ; i++) {
				rax[0] /= mHolder->varify_radix () ;
				rax[1]++ ;
			}
			_STATIC_WARNING_ ("unqualified") ;
			if (r4x > 0) {
				rax[0] = (rax[0] + mHolder->varify_radix () / 2) / mHolder->varify_radix () ;
				rax[1]++ ;
			}
			_STATIC_WARNING_ ("unqualified") ;
		} else if (rax[1] < 1 - r3x) {
			const auto r4x = _MAX_ (LENGTH (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r4x - 1 ; i++) {
				rax[0] /= mHolder->varify_radix () ;
				rax[1]++ ;
			}
			_STATIC_WARNING_ ("unqualified") ;
			if (r4x > 0) {
				rax[0] = (rax[0] + mHolder->varify_radix () / 2) / mHolder->varify_radix () ;
				rax[1]++ ;
			}
			_STATIC_WARNING_ ("unqualified") ;
		}
		const auto r4x = log_of_number (rax[0]) ;
		if (_ABS_ (r4x - 1 + rax[1]) >= 6) {
			//@info: case 'x.xxxExxx'
			try_write_number ((r4x - 1 + rax[1]) ,out ,iw) ;
			_STATIC_WARNING_ ("unqualified") ;
			if (out[iw] != UNIT ('-'))
				out[--iw] = UNIT ('+') ;
			out[--iw] = UNIT ('e') ;
			const auto r5x = _MAX_ ((r4x - 7) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mHolder->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ; i < r4x - 1 ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
				rax[0] /= mHolder->varify_radix () ;
			}
			out[--iw] = UNIT ('.') ;
			iw += EFLAG (out[ix] == UNIT ('.')) ;
			out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
			rax[0] /= mHolder->varify_radix () ;
		} else if (rax[1] > 0) {
			//@info: case 'xxx000'
			for (INDEX i = 0 ,ie = LENGTH (rax[1]) ; i < ie ; i++)
				out[--iw] = mHolder->convert_number_w (0) ;
			for (INDEX i = 0 ; i < r4x ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				rax[0] /= mHolder->varify_radix () ;
			}
		} else if (rax[1] >= 1 - r4x && rax[1] < 0) {
			//@info: case 'xxx.xxx'
			const auto r5x = _MAX_ (LENGTH (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mHolder->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
				rax[0] /= mHolder->varify_radix () ;
			}
			out[--iw] = UNIT ('.') ;
			iw += EFLAG (out[ix] == UNIT ('.')) ;
			for (INDEX i = 0 ,ie = LENGTH (r4x + rax[1]) ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				rax[0] /= mHolder->varify_radix () ;
			}
		} else if (rax[1] < 1 - r4x && rax[1] < 0) {
			//@info: case '0.000xxx'
			const auto r5x = _MAX_ (LENGTH (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mHolder->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ; i < r4x ; i++) {
				out[--iw] = mHolder->convert_number_w (rax[0] % mHolder->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
				rax[0] /= mHolder->varify_radix () ;
			}
			for (INDEX i = _MAX_ (r5x ,r4x) ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = mHolder->convert_number_w (0) ;
				iw += EFLAG (out[ix] == mHolder->convert_number_w (0)) ;
			}
			out[--iw] = UNIT ('.') ;
			iw += EFLAG (out[ix] == UNIT ('.')) ;
			out[--iw] = mHolder->convert_number_w (0) ;
		} else if (rax[1] == 0) {
			//@info: case '0'
			out[--iw] = mHolder->convert_number_w (0) ;
		}
		if (r2x)
			out[--iw] = UNIT ('-') ;
		it = iw ;
	}

	LENGTH log_of_number (VAR64 arg) const {
		LENGTH ret = 0 ;
		for (VAR64 i = 1 ; i <= arg ; i *= mHolder->varify_radix ())
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

template <class _ARG>
inline void _CLS_ (TextReader<_ARG> &reader) {
	reader.reset () ;
}

template <class _ARG>
inline void _CLS_ (TextWriter<_ARG> &writer) {
	writer.reset () ;
}

template <class _ARG>
inline void _BOM_ (TextReader<_ARG> &reader) {
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
	ris.attr ().enable_endian (rax != 0XFEFF) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU32> &reader) {
	auto ris = reader.copy () ;
	auto rax = STRU32 () ;
	ris >> rax ;
	if (rax != STRU32 (0X0000FEFF) && rax != STRU32 (0XFFFE0000))
		return ;
	ris.attr ().enable_endian (rax != 0X0000FEFF) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRW> &reader) {
	_BOM_ (_CAST_<TextReader<STRUW>> (reader)) ;
}

template <class _ARG>
inline void _BOM_ (TextWriter<_ARG> &writer) {
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

template <class _ARG>
inline void _GAP_ (TextReader<_ARG> &reader) {
	auto ris = reader.copy () ;
	auto rax = _ARG () ;
	ris >> rax ;
	while (ris.attr ().varify_space (rax)) {
		reader = ris.copy () ;
		ris >> rax ;
	}
}

template <class _ARG>
inline void _GAP_ (TextWriter<_ARG> &writer) {
	_DYNAMIC_ASSERT_ (writer.length () + 2 < writer.size ()) ;
	writer.write (_ARG ('\r')) ;
	writer.write (_ARG ('\n')) ;
}

inline void _EOS_ (ByteReader &reader) {
	auto ris = reader.copy () ;
	auto rax = BYTE () ;
	while (ris.length () > 0) {
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == BYTE (0XFF)) ;
	}
	reader = std::move (ris) ;
}

inline void _EOS_ (ByteWriter &writer) {
	while (writer.length () < writer.size ())
		writer.write (BYTE (0XFF)) ;
}

template <class _ARG>
inline void _EOS_ (TextReader<_ARG> &reader) {
	auto ris = reader.copy () ;
	const auto r1x = ris.template read<_ARG> () ;
	_DYNAMIC_ASSERT_ (r1x == reader.attr ().varify_ending_item ()) ;
	reader = std::move (ris) ;
}

template <class _ARG>
inline void _EOS_ (TextWriter<_ARG> &writer) {
	writer.write (writer.attr ().varify_ending_item ()) ;
}
} ;

template <class...>
class StreamBinder ;

template <>
class StreamBinder<> {
private:
	FixedBuffer<BYTE> mUseless ;

public:
	inline StreamBinder () = default ;

	inline void friend_read (ByteReader &reader) const popping {
		_STATIC_WARNING_ ("noop") ;
	}

	inline void friend_write (ByteWriter &writer) const popping {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG>
	inline void friend_read (TextReader<_ARG> &reader) const popping {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG>
	inline void friend_write (TextWriter<_ARG> &writer) const popping {
		_STATIC_WARNING_ ("noop") ;
	}
} ;

template <class TYPE1 ,class... TYPES>
class StreamBinder<TYPE1 ,TYPES...> :private StreamBinder<TYPES...> {
private:
	_STATIC_ASSERT_ (!std::is_reference<TYPE1>::value) ;
	TYPE1 &mData ;

public:
	inline StreamBinder () = delete ;

	inline explicit StreamBinder (TYPE1 &arg1 ,TYPES &...args) popping :StreamBinder<TYPES...> (args...) ,mData (arg1) {}

	inline void friend_read (ByteReader &reader) const popping {
		reader >> mData ;
		StreamBinder<TYPES...>::friend_read (reader) ;
	}

	inline void friend_write (ByteWriter &writer) const popping {
		writer << mData ;
		StreamBinder<TYPES...>::friend_write (writer) ;
	}

	template <class _ARG>
	inline void friend_read (TextReader<_ARG> &reader) const popping {
		reader >> mData ;
		StreamBinder<TYPES...>::friend_read (reader) ;
	}

	template <class _ARG>
	inline void friend_write (TextWriter<_ARG> &writer) const popping {
		writer << mData ;
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
	class HINT_GAP_TEXT_TYPE ;
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
	static constexpr auto HINT_GAP_TEXT = ARGV<HINT_GAP_TEXT_TYPE> {} ;
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
		INDEX mWrite ;

	public:
		inline Shadow () = delete ;

		inline explicit Shadow (LLTextReader &context) popping {
			mReader = context.mReader->copy () ;
			mCache = context.mCache ;
			mWrite = context.mWrite ;
		}

		inline const STRU8 &operator[] (INDEX index) const {
			_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
			_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mCache.length ()) ;
			return mCache[(mWrite + index) % mCache.length ()] ;
		}

		inline void operator++ (int) {
			mReader.read (mCache[mWrite]) ;
			mWrite = (mWrite + 1) % mCache.length () ;
		}
	} ;

private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	PhanRef<TextReader<STRU8>> mReader ;
	Array<STRU8 ,SIZE> mCache ;
	INDEX mWrite ;
	BOOL mHintHyperTextFlag ;
	BOOL mHintStringTextFlag ;
	LENGTH mHintNextTextSize ;

public:
	LLTextReader () {
		mCache.fill (0) ;
		mWrite = 0 ;
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
		r1.modify_escape (STRU8 ('\\') ,STRU8 ('\\')) ;
		mReader.self >> _BOM_ ;
		for (INDEX i = 0 ; i < mCache.length () ; i++)
			mReader.self >> mCache[i] ;
		mWrite = 0 ;
		mHintHyperTextFlag = FALSE ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = 0 ;
	}

	Shadow shadow () popping {
		return Shadow (*this) ;
	}

	const STRU8 &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
		_DEBUG_ASSERT_ (mWrite >= 0 && mWrite < mCache.length ()) ;
		return mCache[(mWrite + index) % mCache.length ()] ;
	}

	inline const STRU8 &operator[] (INDEX index) const {
		return get (index) ;
	}

	void read () {
		mReader.self >> mCache[mWrite] ;
		mWrite = (mWrite + 1) % mCache.length () ;
	}

	inline void operator++ (int) {
		read () ;
	}

	template <LENGTH _VAL>
	void read (const DEF<STRU8[_VAL]> &data) {
		for (INDEX i = 0 ; i < _VAL - 1 ; i++) {
			_DYNAMIC_ASSERT_ (get (0) == data[i]) ;
			read () ;
		}
	}

	template <LENGTH _VAL>
	inline LLTextReader &operator>> (const DEF<STRU8[_VAL]> &data) {
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

	void read (ARGV<LLTextReader<void>::HINT_GAP_TEXT_TYPE>) {
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
		const auto r2x = get (0) ;
		if (r1x != STRU8 ('\r') || r2x != STRU8 ('\n'))
			return ;
		read () ;
	}

	template <class _ARG>
	inline LLTextReader &operator>> (ARGV<_ARG> data) {
		read (data) ;
		return *this ;
	}

	void read (String<STRU8> &data) popping {
		const auto r1x = _EXCHANGE_ (mHintHyperTextFlag ,FALSE) ;
		const auto r2x = _EXCHANGE_ (mHintStringTextFlag ,FALSE) ;
		const auto r3x = _EXCHANGE_ (mHintNextTextSize ,VAR_ZERO) ;
		if (r3x > 0 && data.size () != r3x)
			data = String<STRU8> (r3x) ;
		if (r2x) {
			_DYNAMIC_ASSERT_ (get (0) == STRU8 ('\"')) ;
			read () ;
		}
		for (INDEX i = 0 ; i < data.size () ; i++) {
			data[i] = get (0) ;
			read () ;
			if (!r1x)
				continue ;
			const auto r4x = data[i] == mReader->attr ().varify_escape_item () ;
			_DYNAMIC_ASSERT_ (r4x || !mReader->attr ().varify_control (data[i])) ;
			if (!r4x)
				continue ;
			data[i] = get (0) ;
			read () ;
			data[i] = mReader->attr ().convert_escape (data[i]) ;
		}
		if (r2x) {
			_DYNAMIC_ASSERT_ (get (0) == STRU8 ('\"')) ;
			read () ;
		}
		_STATIC_WARNING_ ("unqualified") ;
	}

	inline LLTextReader &operator>> (String<STRU8> &data) popping {
		read (data) ;
		return *this ;
	}

	LENGTH next_identifer_size () popping {
		LENGTH ret = 0 ;
		auto ris = shadow () ;
		while (TRUE) {
			const auto r2x = ris[0] >= STRU8 ('A') && ris[0] <= STRU8 ('Z') ;
			const auto r3x = ris[0] >= STRU8 ('a') && ris[0] <= STRU8 ('z') ;
			const auto r4x = ris[0] == STRU8 ('_') ;
			_DYNAMIC_ASSERT_ (ret != 0 || r2x || r3x || r4x) ;
			const auto r5x = ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9') ;
			const auto r6x = ris[0] == STRU8 ('-') || ris[0] == STRU8 ('.') || ris[0] == STRU8 (':') ;
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
		if (ris[0] == STRU8 ('+') || ris[0] == STRU8 ('-')) {
			ris++ ;
			ret++ ;
		}
		const auto r1x = ris[0] ;
		_DYNAMIC_ASSERT_ (r1x >= STRU8 ('0') && r1x <= STRU8 ('9')) ;
		ris++ ;
		ret++ ;
		while (r1x != STRU8 ('0') && ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
			ris++ ;
			ret++ ;
		}
		if (ris[0] == STRU8 ('.')) {
			ris++ ;
			ret++ ;
			while (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
				ris++ ;
				ret++ ;
			}
		}
		const auto r2x = ris[0] == STRU8 ('e') || ris[0] == STRU8 ('E') ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r2x)
				continue ;
			ris++ ;
			ret++ ;
			if (ris[0] != STRU8 ('+') && ris[0] != STRU8 ('-'))
				continue ;
			ris++ ;
			ret++ ;
		}
		if (r2x) {
			_DYNAMIC_ASSERT_ (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) ;
			ris++ ;
			ret++ ;
			while (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')) {
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
		while (ris[0] != STRU8 ('\0') && ris[0] != STRU8 ('\"')) {
			rax = ris[0] ;
			ris++ ;
			const auto r1x = rax == mReader->attr ().varify_escape_item () ;
			_DYNAMIC_ASSERT_ (r1x || !mReader->attr ().varify_control (rax)) ;
			if (r1x) {
				rax = ris[0] ;
				ris++ ;
				rax = mReader->attr ().convert_escape (rax) ;
			}
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
		while (ris[0] != r1.varify_ending_item () && !r1.varify_space (ris[0])) {
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
		while (ris[0] != r1.varify_ending_item () && !r1.varify_space (ris[0] ,2)) {
			_DYNAMIC_ASSERT_ (!r1.varify_control (ris[0])) ;
			ris++ ;
			ret++ ;
		}
		return std::move (ret) ;
	}
} ;
} ;