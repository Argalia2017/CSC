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
	inline LENGTH size () const {
		return _SIZEOF_ (TYPE) ;
	}

	inline const BYTE &operator[] (INDEX index) const {
		const auto r1x = expr_endian_index (_NULL_<const ARGV<TYPE>> ()) ;
		return EndianBytes::mSelf[_CAST_<BYTE[_SIZEOF_ (TYPE)]> (r1x)[index]] ;
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
		for (INDEX i = 0 ; i < size () ; i++)
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
			return BYTE (0X00) ;
		}

		BYTE varify_space_item () const {
			return BYTE (0X00) ;
		}
	} ;

private:
	SharedRef<Attribute> mHeap ;
	PhanBuffer<const BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteReader () {
		reset () ;
	}

	explicit ByteReader (const PhanBuffer<const BYTE> &stream) {
		mHeap = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const BYTE>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHeap ;
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
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX _read ,INDEX _write) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (_read >= 0 && _read < mStream.size ()) ;
		_DEBUG_ASSERT_ (_write >= 0 && _write < mStream.size ()) ;
		_DEBUG_ASSERT_ (_read <= _write) ;
		mRead = _read ;
		mWrite = _write ;
	}

	ByteReader copy () popping {
		ByteReader ret ;
		ret.mHeap = mHeap ;
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
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mRead < mWrite) ;
			data = mStream[mRead] ;
			mRead++ ;
		} ,[&] (BOOL &_case_req) {
			data = mHeap->varify_ending_item () ;
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

	template <class _ARG1>
	void read (const Plain<_ARG1> &data) {
		auto rax = _ARG1 () ;
		for (INDEX i = 0 ; i < data.size () ; i++) {
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data.self[i]) ;
		}
	}

	template <class _ARG1>
	inline ByteReader &operator>> (const Plain<_ARG1> &data) {
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
			return BYTE (0X00) ;
		}

		BYTE varify_space_item () const {
			return BYTE (0X00) ;
		}
	} ;

private:
	SharedRef<Attribute> mHeap ;
	PhanBuffer<BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteWriter () {
		reset () ;
	}

	explicit ByteWriter (const PhanBuffer<BYTE> &stream) {
		mHeap = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<BYTE>::make (stream) ;
		reset () ;
	}

	explicit ByteWriter (SharedRef<FixedBuffer<BYTE>> &&stream) {
		mHeap = SharedRef<Attribute>::make () ;
		mHeap->mBuffer = std::move (stream) ;
		mStream = PhanBuffer<BYTE>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHeap ;
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
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX _read ,INDEX _write) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (_read >= 0 && _read < mStream.size ()) ;
		_DEBUG_ASSERT_ (_write >= 0 && _write < mStream.size ()) ;
		_DEBUG_ASSERT_ (_read <= _write) ;
		mRead = _read ;
		mWrite = _write ;
	}

	ByteWriter copy () popping {
		ByteWriter ret ;
		ret.mHeap = mHeap ;
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
		for (INDEX i = 0 ; i < r1.size () ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const WORD &data) {
		write (data) ;
		return *this ;
	}

	void write (const CHAR &data) {
		auto &r1 = _CAST_<EndianBytes<CHAR>> (data) ;
		for (INDEX i = 0 ; i < r1.size () ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const CHAR &data) {
		write (data) ;
		return *this ;
	}

	void write (const DATA &data) {
		auto &r1 = _CAST_<EndianBytes<DATA>> (data) ;
		for (INDEX i = 0 ; i < r1.size () ; i++)
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

	template <class _ARG1>
	void write (const Plain<_ARG1> &data) {
		for (INDEX i = 0 ; i < data.size () ; i++)
			write (data.self[i]) ;
	}

	template <class _ARG1>
	inline ByteWriter &operator<< (const Plain<_ARG1> &data) {
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
		HashSet<STRX ,FLAG> mSpaceSet ;

	public:
		Attribute () {
			enable_endian (FALSE) ;
			enable_escape (FALSE) ;
		}

		STRX varify_ending_item () const {
			return STRX ('\0') ;
		}

		void enable_endian (BOOL flag) {
			mEndianFlag = flag ;
		}

		STRX convert_endian (const STRX &item) const {
			TEMP<STRX> ret ;
			const auto r1x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (STRX)]> &)>> (_MEMRCOPY_) ;
			const auto r2x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (STRX)]> &)>> (_MEMCOPY_) ;
			const auto r3x = mEndianFlag ? r1x : r2x ;
			r3x (PTRTOARR[_ZERO_ (ret).unused] ,_CAST_<BYTE[_SIZEOF_ (STRX)]> (item)) ;
			return std::move (_CAST_<STRX> (ret)) ;
		}

		VAR64 varify_radix () const {
			return 10 ;
		}

		LENGTH varify_val32_precision () {
			return 6 ;
		}

		LENGTH varify_val64_precision () {
			_STATIC_WARNING_ ("mark") ;
			return 13 ;
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

		void modify_escape_r (const STRX &str_a ,const STRX &str_e) {
			_DEBUG_ASSERT_ (str_e != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mEscapeSet.find (str_a) == VAR_NONE) ;
			mEscapeSet.add (str_a ,str_e) ;
		}

		STRX convert_escape_r (const STRX &str_a) const {
			INDEX ix = mEscapeSet.find (str_a) ;
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
	SharedRef<Attribute> mHeap ;
	PhanBuffer<const STRX> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextReader () {
		reset () ;
	}

	explicit TextReader (const PhanBuffer<const STRX> &stream) {
		mHeap = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const STRX>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHeap ;
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
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX _read ,INDEX _write) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (_read >= 0 && _read < mStream.size ()) ;
		_DEBUG_ASSERT_ (_write >= 0 && _write < mStream.size ()) ;
		_DEBUG_ASSERT_ (_read <= _write) ;
		mRead = _read ;
		mWrite = _write ;
	}

	TextReader copy () popping {
		TextReader ret ;
		ret.mHeap = mHeap ;
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
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mRead < mWrite) ;
			for (FOR_ONCE_DO_WHILE) {
				data = mHeap->convert_endian (mStream[mRead]) ;
				const auto r2x = mHeap->varify_escape_r (data) ;
				mRead++ ;
				if (!r2x)
					continue ;
				_DYNAMIC_ASSERT_ (mRead < mWrite) ;
				data = mHeap->convert_endian (mStream[mRead]) ;
				data = mHeap->convert_escape_r (data) ;
				mRead++ ;
			}
		} ,[&] (BOOL &_case_req) {
			data = mHeap->varify_ending_item () ;
		}) ;
	}

	inline TextReader &operator>> (STRX &data) popping {
		read (data) ;
		return *this ;
	}

	void read (BOOL &data) popping {
		auto rax = STRX () ;
		read (rax) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('t')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('r')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('u')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('e')) ;
			data = TRUE ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('T')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('R')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('U')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('E')) ;
			data = TRUE ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('f')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('a')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('l')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('s')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('e')) ;
			data = FALSE ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('F')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('A')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('L')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('S')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('E')) ;
			data = FALSE ;
		} ,[&] (BOOL &_case_req) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}) ;
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
		auto rax = STRX () ;
		read (rax) ;
		const auto r1x = BOOL (rax == STRX ('-')) ;
		if (rax == STRX ('+') || rax == STRX ('-'))
			read (rax) ;
		compute_read_number (data ,*this ,rax) ;
		if (r1x)
			data = -data ;
	}

	inline TextReader &operator>> (VAR64 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL32 &data) popping {
		const auto r1x = read<VAL64> () ;
		const auto r2x = BOOL (_ISINF_ (r1x) || _ISNAN_ (r1x)) ;
		const auto r3x = BOOL (r1x >= VAL32_MIN && r1x <= VAL32_MAX) ;
		_DYNAMIC_ASSERT_ (r2x || r3x) ;
		data = VAL32 (r1x) ;
	}

	inline TextReader &operator>> (VAL32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL64 &data) popping {
		auto rax = STRX () ;
		read (rax) ;
		const auto r1x = BOOL (rax == STRX ('-')) ;
		if (rax == STRX ('+') || rax == STRX ('-'))
			read (rax) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('i')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('n')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('f')) ;
			data = VAL64_INF ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('I')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('N')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('F')) ;
			data = VAL64_INF ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('n')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('a')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('n')) ;
			data = VAL64_NAN ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == STRX ('N')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('A')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == STRX ('N')) ;
			data = VAL64_NAN ;
		} ,[&] (BOOL &_case_req) {
			const auto r2x = mHeap->varify_number_item (rax) ;
			_CASE_REQUIRE_ (r2x) ;
			compute_read_number (data ,*this ,rax) ;
		} ,[&] (BOOL &_case_req) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}) ;
		for (FOR_ONCE_DO_WHILE) {
			if (!r1x)
				break ;
			data = -data ;
		}
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

	void read (const Plain<STRX> &data) {
		auto rax = STRX () ;
		for (INDEX i = 0 ; i < data.size () ; i++) {
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data.self[i]) ;
		}
	}

	inline TextReader &operator>> (const Plain<STRX> &data) {
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
			if (rax == mHeap->varify_ending_item ())
				break ;
			if (mHeap->varify_space (rax))
				break ;
			ret++ ;
			ris.read (rax) ;
		}
		return std::move (ret) ;
	}

	void compute_read_number (VAR64 &data ,TextReader &reader ,STRX &top) const {
		_DEBUG_ASSERT_ (mHeap->varify_number_item (top)) ;
		auto ris = reader.copy () ;
		data = mHeap->convert_number_r (top) ;
		ris.read (top) ;
		while (TRUE) {
			if (!mHeap->varify_number_item (top))
				break ;
			const auto r1x = data * mHeap->varify_radix () + mHeap->convert_number_r (top) ;
			_DYNAMIC_ASSERT_ (data <= r1x) ;
			data = r1x ;
			reader = ris.copy () ;
			ris.read (top) ;
		}
	}

	void compute_read_number (VAL64 &data ,TextReader &reader ,STRX &top) const {
		_DEBUG_ASSERT_ (mHeap->varify_number_item (top)) ;
		auto ris = reader.copy () ;
		while (TRUE) {
			if (mHeap->convert_number_r (top) != 0)
				break ;
			reader = ris.copy () ;
			ris.read (top) ;
		}
		auto rax = ARRAY3<VAR64> {0 ,0 ,0} ;
		for (FOR_ONCE_DO_WHILE) {
			if (!mHeap->varify_number_item (top))
				continue ;
			rax[0] = mHeap->convert_number_r (top) ;
			reader = ris.copy () ;
			ris.read (top) ;
			while (TRUE) {
				if (!mHeap->varify_number_item (top))
					break ;
				_CALL_IF_ ([&] (BOOL &_case_req) {
					const auto r1x = rax[0] * mHeap->varify_radix () + mHeap->convert_number_r (top) ;
					_CASE_REQUIRE_ (rax[0] < r1x) ;
					rax[0] = r1x ;
				} ,[&] (BOOL &_case_req) {
					rax[1]++ ;
				}) ;
				reader = ris.copy () ;
				ris.read (top) ;
			}
		}
		for (FOR_ONCE_DO_WHILE) {
			if (top != STRX ('.'))
				continue ;
			reader = ris.copy () ;
			ris.read (top) ;
			_DYNAMIC_ASSERT_ (mHeap->varify_number_item (top)) ;
			while (TRUE) {
				if (!mHeap->varify_number_item (top))
					break ;
				for (FOR_ONCE_DO_WHILE) {
					const auto r2x = rax[0] * mHeap->varify_radix () + mHeap->convert_number_r (top) ;
					if (rax[0] > r2x)
						continue ;
					rax[0] = r2x ;
					rax[1]-- ;
				}
				reader = ris.copy () ;
				ris.read (top) ;
			}
		}
		for (FOR_ONCE_DO_WHILE) {
			if (top != STRX ('e') && top != STRX ('E'))
				continue ;
			const auto r3x = ris.template read<VAR32> () ;
			rax[1] += r3x ;
			reader = ris.copy () ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (rax[0] >= 0)
				continue ;
			rax[0] = -rax[0] ;
			rax[2] = -1 ;
			if (rax[0] >= 0)
				continue ;
			rax[0] = -(rax[0] / mHeap->varify_radix ()) ;
			rax[1]++ ;
		}
		const auto r4x = _IEEE754_E10TOE2_ (rax) ;
		data = _IEEE754_ENCODE_ (r4x) ;
	}
} ;

template <class STRX>
class TextWriter {
private:
	class Attribute final :private Interface {
	private:
		friend TextWriter ;
		SharedRef<FixedBuffer<STRX>> mBuffer ;
		BOOL mEndianFlag ;
		BOOL mEscapeFlag ;
		HashSet<STRX ,STRX> mEscapeSet ;

	public:
		Attribute () {
			enable_endian (FALSE) ;
			enable_escape (FALSE) ;
		}

		STRX varify_ending_item () const {
			return STRX ('\0') ;
		}

		void enable_endian (BOOL flag) {
			mEndianFlag = flag ;
		}

		STRX convert_endian (const STRX &item) const {
			TEMP<STRX> ret ;
			const auto r1x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (STRX)]> &)>> (_MEMRCOPY_) ;
			const auto r2x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (STRX)]> &)>> (_MEMCOPY_) ;
			const auto r3x = mEndianFlag ? r1x : r2x ;
			r3x (PTRTOARR[_ZERO_ (ret).unused] ,_CAST_<BYTE[_SIZEOF_ (STRX)]> (item)) ;
			return std::move (_CAST_<STRX> (ret)) ;
		}

		VAR64 varify_radix () const {
			return 10 ;
		}

		LENGTH varify_val32_precision () {
			return 6 ;
		}

		LENGTH varify_val64_precision () {
			_STATIC_WARNING_ ("mark") ;
			return 13 ;
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

		void modify_escape_w (const STRX &str_a ,const STRX &str_e) {
			_DEBUG_ASSERT_ (str_a != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (mEscapeSet.find (str_e) == VAR_NONE) ;
			mEscapeSet.add (str_e ,str_a) ;
		}

		STRX convert_escape_w (const STRX &str_e) const {
			INDEX ix = mEscapeSet.find (str_e) ;
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
	SharedRef<Attribute> mHeap ;
	PhanBuffer<STRX> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextWriter () {
		reset () ;
	}

	explicit TextWriter (const PhanBuffer<STRX> &stream) {
		mHeap = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<STRX>::make (stream) ;
		reset () ;
	}

	explicit TextWriter (SharedRef<FixedBuffer<STRX>> &&stream) {
		mHeap = SharedRef<Attribute>::make () ;
		mHeap->mBuffer = std::move (stream) ;
		mStream = PhanBuffer<STRX>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const & {
		return mHeap ;
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
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX _read ,INDEX _write) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (_read >= 0 && _read < mStream.size ()) ;
		_DEBUG_ASSERT_ (_write >= 0 && _write < mStream.size ()) ;
		_DEBUG_ASSERT_ (_read <= _write) ;
		mRead = _read ;
		mWrite = _write ;
	}

	TextWriter copy () popping {
		TextWriter ret ;
		ret.mHeap = mHeap ;
		ret.mStream = PhanBuffer<STRX>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void write (const STRX &data) {
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mHeap->varify_escape_w (data)) ;
			_DYNAMIC_ASSERT_ (mWrite + 1 < mStream.size ()) ;
			const auto r1x = mHeap->varify_escape_item () ;
			mStream[mWrite] = mHeap->convert_endian (r1x) ;
			mWrite++ ;
			const auto r2x = mHeap->convert_escape_w (data) ;
			mStream[mWrite] = mHeap->convert_endian (r2x) ;
			mWrite++ ;
		} ,[&] (BOOL &_case_req) {
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = mHeap->convert_endian (data) ;
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
		INDEX ix = rax.size () ;
		compute_write_number (data ,PhanBuffer<STRX>::make (rax) ,ix) ;
		write (PhanBuffer<const STRX>::make (PTRTOARR[&rax.self[ix]] ,(rax.size () - ix))) ;
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
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISNAN_ (data)) ;
			write (PhanBuffer<const STRX>::make (M_NAN.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data > 0) ;
			write (PhanBuffer<const STRX>::make (M_INF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data < 0) ;
			write (PhanBuffer<const STRX>::make (M_SINF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			auto rax = Buffer<STRX ,ARGC<256>> () ;
			INDEX ix = rax.size () ;
			const auto r1x = mHeap->varify_val32_precision () ;
			compute_write_number (data ,r1x ,PhanBuffer<STRX>::make (rax) ,ix) ;
			write (PhanBuffer<const STRX>::make (PTRTOARR[&rax.self[ix]] ,(rax.size () - ix))) ;
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
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISNAN_ (data)) ;
			write (PhanBuffer<const STRX>::make (M_NAN.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data > 0) ;
			write (PhanBuffer<const STRX>::make (M_INF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data < 0) ;
			write (PhanBuffer<const STRX>::make (M_SINF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			auto rax = Buffer<STRX ,ARGC<256>> () ;
			INDEX ix = rax.size () ;
			const auto r1x = mHeap->varify_val64_precision () ;
			compute_write_number (data ,r1x ,PhanBuffer<STRX>::make (rax) ,ix) ;
			write (PhanBuffer<const STRX>::make (PTRTOARR[&rax.self[ix]] ,(rax.size () - ix))) ;
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

	void write (const Plain<STRX> &data) {
		for (INDEX i = 0 ; i < data.size () ; i++)
			write (data.self[i]) ;
	}

	inline TextWriter &operator<< (const Plain<STRX> &data) {
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
		INDEX iw = it ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (data > 0) ;
			auto rax = data ;
			while (TRUE) {
				if (rax == 0)
					break ;
				out[--iw] = mHeap->convert_number_w (rax % mHeap->varify_radix ()) ;
				rax /= mHeap->varify_radix () ;
			}
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (data < 0) ;
			auto rax = data ;
			while (TRUE) {
				if (rax == 0)
					break ;
				out[--iw] = mHeap->convert_number_w (-rax % mHeap->varify_radix ()) ;
				rax /= mHeap->varify_radix () ;
			}
			out[--iw] = STRX ('-') ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (data == 0) ;
			out[--iw] = mHeap->convert_number_w (0) ;
		}) ;
		it = iw ;
	}

	void compute_write_number (const VAL64 &data ,LENGTH precision ,const PhanBuffer<STRX> &out ,INDEX &it) const {
		INDEX iw = it ;
		const auto r1x = _IEEE754_DECODE_ (data) ;
		auto rax = _IEEE754_E2TOE10_ (r1x) ;
		const auto r3x = log_of_number (rax[0]) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ABS_ (r3x - 1 + rax[1]) >= precision) ;
			for (FOR_ONCE_DO_WHILE) {
				const auto r4x = _MAX_ ((r3x - 1 - precision) ,VAR_ZERO) ;
				for (INDEX i = 0 ,ie = r4x - 1 ; i < ie ; i++) {
					rax[0] /= mHeap->varify_radix () ;
					rax[1]++ ;
				}
				if (r4x <= 0)
					continue ;
				rax[0] = _ROUND_ (rax[0] ,mHeap->varify_radix ()) / mHeap->varify_radix () ;
				rax[1]++ ;
			}
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax[1] >= 1 - r3x) ;
			_CASE_REQUIRE_ (rax[1] < 0) ;
			for (FOR_ONCE_DO_WHILE) {
				const auto r10x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
				for (INDEX i = 0 ,ie = r10x - 1 ; i < ie ; i++) {
					rax[0] /= mHeap->varify_radix () ;
					rax[1]++ ;
				}
				if (r10x <= 0)
					continue ;
				rax[0] = _ROUND_ (rax[0] ,mHeap->varify_radix ()) / mHeap->varify_radix () ;
				rax[1]++ ;
			}
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax[1] < 1 - r3x) ;
			for (FOR_ONCE_DO_WHILE) {
				const auto r9x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
				for (INDEX i = 0 ,ie = r9x - 1 ; i < ie ; i++) {
					rax[0] /= mHeap->varify_radix () ;
					rax[1]++ ;
				}
				if (r9x <= 0)
					continue ;
				rax[0] = _ROUND_ (rax[0] ,mHeap->varify_radix ()) / mHeap->varify_radix () ;
				rax[1]++ ;
			}
		}) ;
		const auto r8x = log_of_number (rax[0]) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			//@info: case 'x.xxxExxx'
			const auto r11x = r8x - 1 + rax[1] ;
			_CASE_REQUIRE_ (_ABS_ (r11x) >= precision) ;
			compute_write_number (r11x ,out ,iw) ;
			for (FOR_ONCE_DO_WHILE) {
				if (r11x <= 0)
					continue ;
				out[--iw] = STRX ('+') ;
			}
			out[--iw] = STRX ('e') ;
			const auto r5x = _MAX_ ((r8x - 1 - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mHeap->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ,ie = r8x - 1 ; i < ie ; i++) {
				out[--iw] = mHeap->convert_number_w (rax[0] % mHeap->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHeap->convert_number_w (0)) ;
				rax[0] /= mHeap->varify_radix () ;
			}
			out[--iw] = STRX ('.') ;
			iw += EFLAG (out[ix] == STRX ('.')) ;
			out[--iw] = mHeap->convert_number_w (rax[0] % mHeap->varify_radix ()) ;
			rax[0] /= mHeap->varify_radix () ;
		} ,[&] (BOOL &_case_req) {
			//@info: case 'xxx000'
			_CASE_REQUIRE_ (rax[1] > 0) ;
			for (INDEX i = 0 ,ie = LENGTH (rax[1]) ; i < ie ; i++)
				out[--iw] = mHeap->convert_number_w (0) ;
			for (INDEX i = 0 ; i < r8x ; i++) {
				out[--iw] = mHeap->convert_number_w (rax[0] % mHeap->varify_radix ()) ;
				rax[0] /= mHeap->varify_radix () ;
			}
		} ,[&] (BOOL &_case_req) {
			//@info: case 'xxx.xxx'
			_CASE_REQUIRE_ (rax[1] >= 1 - r8x) ;
			_CASE_REQUIRE_ (rax[1] < 0) ;
			const auto r7x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r7x ; i++)
				rax[0] /= mHeap->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r7x ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = mHeap->convert_number_w (rax[0] % mHeap->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHeap->convert_number_w (0)) ;
				rax[0] /= mHeap->varify_radix () ;
			}
			out[--iw] = STRX ('.') ;
			iw += EFLAG (out[ix] == STRX ('.')) ;
			for (INDEX i = 0 ,ie = LENGTH (r8x + rax[1]) ; i < ie ; i++) {
				out[--iw] = mHeap->convert_number_w (rax[0] % mHeap->varify_radix ()) ;
				rax[0] /= mHeap->varify_radix () ;
			}
		} ,[&] (BOOL &_case_req) {
			//@info: case '0.000xxx'
			_CASE_REQUIRE_ (rax[1] < 1 - r8x) ;
			_CASE_REQUIRE_ (rax[1] < 0) ;
			const auto r6x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r6x ; i++)
				rax[0] /= mHeap->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r6x ; i < r8x ; i++) {
				out[--iw] = mHeap->convert_number_w (rax[0] % mHeap->varify_radix ()) ;
				iw += EFLAG (out[ix] == mHeap->convert_number_w (0)) ;
				rax[0] /= mHeap->varify_radix () ;
			}
			for (INDEX i = _MAX_ (r6x ,r8x) ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = mHeap->convert_number_w (0) ;
				iw += EFLAG (out[ix] == mHeap->convert_number_w (0)) ;
			}
			out[--iw] = STRX ('.') ;
			iw += EFLAG (out[ix] == STRX ('.')) ;
			out[--iw] = mHeap->convert_number_w (0) ;
		} ,[&] (BOOL &_case_req) {
			//@info: case '0'
			_CASE_REQUIRE_ (rax[1] == 0) ;
			out[--iw] = mHeap->convert_number_w (0) ;
		}) ;
		for (FOR_ONCE_DO_WHILE) {
			if (rax[2] == 0)
				continue ;
			out[--iw] = STRX ('-') ;
		}
		it = iw ;
	}

	LENGTH log_of_number (VAR64 arg1) const {
		LENGTH ret = 0 ;
		auto rax = VAR64 (1) ;
		while (TRUE) {
			if (rax > arg1)
				break ;
			ret++ ;
			rax *= mHeap->varify_radix () ;
		}
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
	static constexpr auto M_BOM = PACK<STRU8[3]> ({
		STRU8 (0XEF) ,STRU8 (0XBB) ,STRU8 (0XBF)}) ;
	auto ris = reader.copy () ;
	auto rax = STRU8 () ;
	ris >> rax ;
	if (rax != M_BOM.P1[0])
		return ;
	ris >> rax ;
	if (rax != M_BOM.P1[1])
		return ;
	ris >> rax ;
	if (rax != M_BOM.P1[2])
		return ;
	ris.attr ().enable_endian (FALSE) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU16> &reader) {
	static constexpr auto M_BOM = PACK<STRU16[2]> ({
		STRU16 (0XFEFF) ,STRU16 (0XFFFE)}) ;
	auto ris = reader.copy () ;
	auto rax = STRU16 () ;
	ris >> rax ;
	if (rax != M_BOM.P1[0] && rax != M_BOM.P1[1])
		return ;
	const auto r1x = BOOL (rax != M_BOM.P1[0]) ;
	ris.attr ().enable_endian (r1x) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU32> &reader) {
	static constexpr auto M_BOM = PACK<STRU32[2]> ({
		STRU32 (0X0000FEFF) ,STRU32 (0XFFFE0000)}) ;
	auto ris = reader.copy () ;
	auto rax = STRU32 () ;
	ris >> rax ;
	if (rax != M_BOM.P1[0] && rax != M_BOM.P1[1])
		return ;
	const auto r1x = BOOL (rax != M_BOM.P1[0]) ;
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
	auto wos = writer.copy () ;
	wos << PhanBuffer<const STRU8>::make (M_BOM.P1) ;
	writer = std::move (wos) ;
}

inline void _BOM_ (TextWriter<STRU16> &writer) {
	static constexpr auto M_BOM = PACK<STRU16[1]> ({
		STRU16 (0XFEFF)}) ;
	auto wos = writer.copy () ;
	wos << PhanBuffer<const STRU16>::make (M_BOM.P1) ;
	writer = std::move (wos) ;
}

inline void _BOM_ (TextWriter<STRU32> &writer) {
	static constexpr auto M_BOM = PACK<STRU32[1]> ({
		STRU32 (0X0000FEFF)}) ;
	auto wos = writer.copy () ;
	wos << PhanBuffer<const STRU32>::make (M_BOM.P1) ;
	writer = std::move (wos) ;
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
	_DYNAMIC_ASSERT_ (rax == ris.attr ().varify_space_item ()) ;
	reader = std::move (ris) ;
}

inline void _GAP_ (ByteWriter &writer) {
	auto wos = writer.copy () ;
	wos << wos.attr ().varify_space_item () ;
	writer = std::move (wos) ;
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
	auto wos = writer.copy () ;
	_DYNAMIC_ASSERT_ (wos.length () + 2 < wos.size ()) ;
	wos << _ARG1 ('\r') << _ARG1 ('\n') ;
	writer = std::move (wos) ;
}

inline void _EOS_ (ByteReader &reader) {
	auto ris = reader.copy () ;
	auto rax = BYTE () ;
	const auto r1x = ris.attr ().varify_ending_item () ;
	while (TRUE) {
		if (ris.length () == 0)
			break ;
		ris >> rax ;
		_DYNAMIC_ASSERT_ (rax == r1x) ;
	}
	reader = std::move (ris) ;
}

inline void _EOS_ (ByteWriter &writer) {
	auto wos = writer.copy () ;
	const auto r1x = wos.attr ().varify_ending_item () ;
	while (wos.length () < wos.size ())
		wos << r1x ;
	writer = std::move (wos) ;
}

template <class _ARG1>
inline void _EOS_ (TextReader<_ARG1> &reader) {
	auto ris = reader.copy () ;
	auto rax = _ARG1 () ;
	ris.read (rax) ;
	_DYNAMIC_ASSERT_ (rax == ris.attr ().varify_ending_item ()) ;
	reader = std::move (ris) ;
}

template <class _ARG1>
inline void _EOS_ (TextWriter<_ARG1> &writer) {
	auto wos = writer.copy () ;
	wos << wos.attr ().varify_ending_item () ;
	writer = std::move (wos) ;
}
} ;

inline namespace S {
inline void _PRINTS_ (ByteWriter &writer) {
	_STATIC_WARNING_ ("noop") ;
}

template <class _ARG1 ,class... _ARGS>
inline void _PRINTS_ (ByteWriter &writer ,const _ARG1 &arg1 ,const _ARGS &...args) {
	writer << arg1 ;
	_PRINTS_ (writer ,args...) ;
}

template <class _ARG1>
inline void _PRINTS_ (TextWriter<_ARG1> &writer) {
	_STATIC_WARNING_ ("noop") ;
}

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
inline void _PRINTS_ (TextWriter<_ARG1> &writer ,const _ARG2 &arg1 ,const _ARGS &...args) {
	writer << arg1 ;
	_PRINTS_ (writer ,args...) ;
}
} ;

template <class SIZE = VOID>
class LLTextReader ;

template <>
class LLTextReader<void> :private Wrapped<void> {
private:
	template <class>
	friend class LLTextReader ;

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
		INDEX mPeek ;

	public:
		inline Shadow () = delete ;

		inline explicit Shadow (TextReader<STRU8> &&reader ,const Array<STRU8 ,SIZE> &cache ,INDEX _peek) :mReader (std::move (reader)) ,mCache (cache) ,mPeek (_peek) {}

		inline const STRU8 &operator[] (INDEX index) const {
			_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
			_DEBUG_ASSERT_ (mPeek >= 0 && mPeek < mCache.length ()) ;
			return mCache[(mPeek + index) % mCache.length ()] ;
		}

		inline void operator++ (int) {
			mReader.read (mCache[mPeek]) ;
			mPeek = (mPeek + 1) % mCache.length () ;
		}
	} ;

private:
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	PhanRef<TextReader<STRU8>> mReader ;
	Array<STRU8 ,SIZE> mCache ;
	INDEX mPeek ;
	BOOL mHintStringTextFlag ;
	LENGTH mHintNextTextSize ;

public:
	LLTextReader () {
		mCache.fill (0) ;
		mPeek = 0 ;
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
		r1.modify_escape_r (STRU8 ('t') ,STRU8 ('\t')) ;
		r1.modify_escape_r (STRU8 ('r') ,STRU8 ('\r')) ;
		r1.modify_escape_r (STRU8 ('n') ,STRU8 ('\n')) ;
		r1.modify_escape_r (STRU8 ('f') ,STRU8 ('\f')) ;
		r1.modify_escape_r (STRU8 ('\"') ,STRU8 ('\"')) ;
		r1.modify_escape_r (STRU8 ('/') ,STRU8 ('/')) ;
		r1.modify_escape_r (STRU8 ('\\') ,STRU8 ('\\')) ;
		_STATIC_WARNING_ ("mark") ;
		//@info: disable default escape-str convertion 
		r1.enable_escape (FALSE) ;
		mReader.self >> _BOM_ ;
		for (INDEX i = 0 ; i < mCache.length () ; i++)
			mReader.self >> mCache[i] ;
		mPeek = 0 ;
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = 0 ;
	}

	Shadow shadow () popping {
		return Shadow (mReader->copy () ,mCache ,mPeek) ;
	}

	const STRU8 &get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < mCache.length ()) ;
		_DEBUG_ASSERT_ (mPeek >= 0 && mPeek < mCache.length ()) ;
		return mCache[(mPeek + index) % mCache.length ()] ;
	}

	inline const STRU8 &operator[] (INDEX index) const & {
		return get (index) ;
	}

	const STRU8 &get (INDEX) && = delete ;

	inline const STRU8 &operator[] (INDEX) && = delete ;

	void read () {
		mReader.self >> mCache[mPeek] ;
		mPeek = (mPeek + 1) % mCache.length () ;
	}

	inline void operator++ (int) {
		read () ;
	}

	void read (const Plain<STRU8> &data) {
		for (INDEX i = 0 ; i < data.size () ; i++) {
			_DYNAMIC_ASSERT_ (get (0) == data.self[i]) ;
			read () ;
		}
	}

	inline LLTextReader &operator>> (const Plain<STRU8> &data) {
		read (data) ;
		return *this ;
	}

	void read (ARGV<LLTextReader<void>::HINT_IDENTIFY_TEXT_TYPE>) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_identifer_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_VALUE_TEXT_TYPE>) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_value_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_STRING_TEXT_TYPE>) {
		mHintStringTextFlag = TRUE ;
		mHintNextTextSize = next_string_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_NEWGAP_TEXT_TYPE>) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_gap_text_size () ;
	}

	void read (ARGV<LLTextReader<void>::HINT_NEWLINE_TEXT_TYPE>) {
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
		const auto r2x = _EXCHANGE_ (mHintStringTextFlag ,FALSE) ;
		const auto r3x = _EXCHANGE_ (mHintNextTextSize ,VAR_ZERO) ;
		if (data.size () < r3x)
			data = String<STRU8> (r3x) ;
		data.clear () ;
		auto rax = STRU8 () ;
		for (FOR_ONCE_DO_WHILE) {
			if (!r2x)
				continue ;
			_DYNAMIC_ASSERT_ (get (0) == STRU8 ('\"')) ;
			read () ;
		}
		for (INDEX i = 0 ; i < r3x ; i++) {
			_CALL_IF_ ([&] (BOOL &_case_req) {
				rax = get (0) ;
				read () ;
				const auto r4x = BOOL (rax == mReader->attr ().varify_escape_item ()) ;
				_CASE_REQUIRE_ (r4x) ;
				rax = get (0) ;
				read () ;
				rax = mReader->attr ().convert_escape_r (rax) ;
				data[i] = rax ;
			} ,[&] (BOOL &_case_req) {
				_DYNAMIC_ASSERT_ (!mReader->attr ().varify_control (rax)) ;
				data[i] = rax ;
			}) ;
		}
		for (FOR_ONCE_DO_WHILE) {
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

private:
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
		for (FOR_ONCE_DO_WHILE) {
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
		for (FOR_ONCE_DO_WHILE) {
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
		for (FOR_ONCE_DO_WHILE) {
			if (ris[0] != STRU8 ('e') && ris[0] != STRU8 ('E'))
				continue ;
			ris++ ;
			ret++ ;
			for (FOR_ONCE_DO_WHILE) {
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
			_CALL_IF_ ([&] (BOOL &_case_req) {
				rax = ris[0] ;
				ris++ ;
				const auto r1x = BOOL (rax == mReader->attr ().varify_escape_item ()) ;
				_CASE_REQUIRE_ (r1x) ;
				rax = ris[0] ;
				ris++ ;
				rax = mReader->attr ().convert_escape_r (rax) ;
				ret++ ;
			} ,[&] (BOOL &_case_req) {
				_DYNAMIC_ASSERT_ (!mReader->attr ().varify_control (rax)) ;
				ret++ ;
			}) ;
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