#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
namespace U {
template <class ,class>
struct BYTE_TRAITS ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (BYTE)> ,ARGC<_ALIGNOF_ (BYTE)>> {
	using TYPE = BYTE ;
} ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (WORD)> ,ARGC<_ALIGNOF_ (WORD)>> {
	using TYPE = WORD ;
} ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (CHAR)> ,ARGC<_ALIGNOF_ (CHAR)>> {
	using TYPE = CHAR ;
} ;

template <>
struct BYTE_TRAITS<ARGC<_SIZEOF_ (DATA)> ,ARGC<_ALIGNOF_ (DATA)>> {
	using TYPE = DATA ;
} ;

template <class ,class>
struct TEXT_TRAITS ;

template <>
struct TEXT_TRAITS<ARGC<_SIZEOF_ (STRU8)> ,ARGC<_ALIGNOF_ (STRU8)>> {
	using TYPE = STRU8 ;
} ;

template <>
struct TEXT_TRAITS<ARGC<_SIZEOF_ (STRU16)> ,ARGC<_ALIGNOF_ (STRU16)>> {
	using TYPE = STRU16 ;
} ;

template <>
struct TEXT_TRAITS<ARGC<_SIZEOF_ (STRU32)> ,ARGC<_ALIGNOF_ (STRU32)>> {
	using TYPE = STRU32 ;
} ;
} ;

template <class _ARG1>
using BYTE_TRAITS_TYPE = typename U::BYTE_TRAITS<ARGC<_SIZEOF_ (_ARG1)> ,ARGC<_ALIGNOF_ (_ARG1)>>::TYPE ;

template <class _ARG1>
using TEXT_TRAITS_TYPE = typename U::TEXT_TRAITS<ARGC<_SIZEOF_ (_ARG1)> ,ARGC<_ALIGNOF_ (_ARG1)>>::TYPE ;

using STRUA = TEXT_TRAITS_TYPE<STRA> ;
using STRUW = TEXT_TRAITS_TYPE<STRW> ;

template <class UNIT>
class EndianBytes :private Wrapped<BYTE[_SIZEOF_ (UNIT)]> {
private:
	inline static constexpr BYTE constexpr_big_endian (const ARGV<BYTE> &) {
		//@info: 'big endian' [0] -> 0X00
		return BYTE (0X00) ;
	}

	inline static constexpr WORD constexpr_big_endian (const ARGV<WORD> &) {
		//@info: 'big endian' [0] -> 0X00
		return WORD (0X0001) ;
	}

	inline static constexpr CHAR constexpr_big_endian (const ARGV<CHAR> &) {
		//@info: 'big endian' [0] -> 0X00
		return CHAR (0X00010203) ;
	}

	inline static constexpr DATA constexpr_big_endian (const ARGV<DATA> &) {
		//@info: 'big endian' [0] -> 0X00
		return DATA (0X0001020304050607) ;
	}

private:
	_STATIC_ASSERT_ (stl::is_byte_xyz<UNIT>::value) ;

public:
	inline LENGTH size () const {
		return _SIZEOF_ (UNIT) ;
	}

	inline const BYTE &operator[] (INDEX index) const {
		const auto r1x = constexpr_big_endian (_NULL_<ARGV<UNIT>> ()) ;
		return EndianBytes::mSelf[_CAST_<BYTE[_SIZEOF_ (UNIT)]> (r1x)[index]] ;
	}

	template <class _RET ,class = ENABLE_TYPE<std::is_convertible<const UNIT & ,_RET>::value>>
	inline implicit operator _RET () const & {
		return _RET (big_endian_value ()) ;
	}

	template <class _RET>
	inline implicit operator _RET () && = delete ;

private:
	inline UNIT big_endian_value () const {
		TEMP<UNIT> ret ;
		_ZERO_ (ret) ;
		for (INDEX i = 0 ; i < size () ; i++)
			ret.unused[i] = (*this)[i] ;
		return std::move (_CAST_<UNIT> (ret)) ;
	}
} ;

class ByteReader {
private:
	struct HEAP {} ;

	class Attribute :private Wrapped<HEAP> {
	public:
		BYTE varify_ending_item () const {
			return BYTE (0X00) ;
		}

		BYTE varify_space_item () const {
			return BYTE (0X00) ;
		}
	} ;

private:
	SharedRef<HEAP> mHeap ;
	PhanBuffer<const BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteReader () {
		reset () ;
	}

	explicit ByteReader (const PhanBuffer<const BYTE> &stream) {
		mHeap = SharedRef<HEAP>::make () ;
		mStream = PhanBuffer<const BYTE>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const & {
		return _CAST_<Attribute> (mHeap.self) ;
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
		_DEBUG_ASSERT_ (BOOL (_read >= 0 && _read < mStream.size ())) ;
		_DEBUG_ASSERT_ (BOOL (_write >= 0 && _write < mStream.size ())) ;
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
			data = attr ().varify_ending_item () ;
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
		_DYNAMIC_ASSERT_ (BOOL (r1x >= 0 && r1x < VAR32_MAX)) ;
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
		_STATIC_ASSERT_ (stl::is_str_xyz<_ARG1>::value) ;
		const auto r1x = LENGTH (read<VAR32> ()) ;
		_DYNAMIC_ASSERT_ (BOOL (r1x >= 0 && r1x < VAR32_MAX)) ;
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

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<ByteReader> ()))> ,void>::value>>
	void read (_ARG1 &data) popping {
		data.friend_read (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<ByteReader> ()))> ,void>::value>>
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
	struct HEAP {
		SharedRef<FixedBuffer<BYTE>> mBuffer ;
	} ;

	class Attribute :private Wrapped<HEAP> {
	public:
		BYTE varify_ending_item () const {
			return BYTE (0X00) ;
		}

		BYTE varify_space_item () const {
			return BYTE (0X00) ;
		}
	} ;

private:
	SharedRef<HEAP> mHeap ;
	PhanBuffer<BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteWriter () {
		reset () ;
	}

	explicit ByteWriter (const PhanBuffer<BYTE> &stream) {
		mHeap = SharedRef<HEAP>::make () ;
		mStream = PhanBuffer<BYTE>::make (stream) ;
		reset () ;
	}

	explicit ByteWriter (SharedRef<FixedBuffer<BYTE>> &&stream) {
		mHeap = SharedRef<HEAP>::make () ;
		mHeap->mBuffer = std::move (stream) ;
		mStream = PhanBuffer<BYTE>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const & {
		return _CAST_<Attribute> (mHeap.self) ;
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
		_DEBUG_ASSERT_ (BOOL (_read >= 0 && _read < mStream.size ())) ;
		_DEBUG_ASSERT_ (BOOL (_write >= 0 && _write < mStream.size ())) ;
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

	void write (const PTR<const VOID> &) = delete ;

	inline ByteWriter &operator<< (const PTR<const VOID> &) = delete ;

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
		_DYNAMIC_ASSERT_ (BOOL (r1x >= 0 && r1x < VAR32_MAX)) ;
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
		_STATIC_ASSERT_ (stl::is_str_xyz<_ARG1>::value) ;
		const auto r1x = data.length () ;
		_DYNAMIC_ASSERT_ (BOOL (r1x >= 0 && r1x < VAR32_MAX)) ;
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

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<ByteWriter> ()))> ,void>::value>>
	void write (const _ARG1 &data) {
		data.friend_write (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<ByteWriter> ()))> ,void>::value>>
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

template <class REAL>
class TextReader {
private:
	struct HEAP {
		BOOL mEndianFlag ;
		BOOL mEscapeFlag ;
		HashSet<REAL ,REAL> mEscapeSet ;
		HashSet<REAL ,FLAG> mSpaceSet ;
	} ;

	class Attribute :private Wrapped<HEAP> {
	public:
		REAL varify_ending_item () const {
			return REAL ('\0') ;
		}

		void enable_endian (BOOL flag) {
			Attribute::mSelf.mEndianFlag = flag ;
		}

		REAL convert_endian (const REAL &item) const {
			TEMP<REAL> ret ;
			const auto r1x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (REAL)]> &)>> (_MEMRCOPY_) ;
			const auto r2x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (REAL)]> &)>> (_MEMCOPY_) ;
			const auto r3x = Attribute::mSelf.mEndianFlag ? r1x : r2x ;
			_ZERO_ (ret) ;
			r3x (PTRTOARR[ret.unused] ,_CAST_<BYTE[_SIZEOF_ (REAL)]> (item)) ;
			return std::move (_CAST_<REAL> (ret)) ;
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

		BOOL varify_number_item (const REAL &item) const {
			if (!BOOL (item >= REAL ('0') && item <= REAL ('9')))
				return FALSE ;
			return TRUE ;
		}

		VAR64 convert_number_r (const REAL &item) const {
			return VAR64 (item) - REAL ('0') ;
		}

		void enable_escape (BOOL flag) {
			Attribute::mSelf.mEscapeFlag = flag ;
		}

		REAL varify_escape_item () const {
			return REAL ('\\') ;
		}

		BOOL varify_escape_r (const REAL &item) const {
			if (!Attribute::mSelf.mEscapeFlag)
				return FALSE ;
			if (item != varify_escape_item ())
				return FALSE ;
			return TRUE ;
		}

		void modify_escape_r (const REAL &str_a ,const REAL &str_e) {
			_DEBUG_ASSERT_ (str_e != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (Attribute::mSelf.mEscapeSet.find (str_a) == VAR_NONE) ;
			Attribute::mSelf.mEscapeSet.add (str_a ,str_e) ;
		}

		REAL convert_escape_r (const REAL &str_a) const {
			INDEX ix = Attribute::mSelf.mEscapeSet.find (str_a) ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			return Attribute::mSelf.mEscapeSet[ix].item ;
		}

		BOOL varify_space (const REAL &item) const {
			INDEX ix = Attribute::mSelf.mSpaceSet.find (item) ;
			if (ix == VAR_NONE)
				return FALSE ;
			return TRUE ;
		}

		BOOL varify_space (const REAL &item ,VAR32 group) const {
			INDEX ix = Attribute::mSelf.mSpaceSet.find (item) ;
			if (ix == VAR_NONE)
				return FALSE ;
			if (Attribute::mSelf.mSpaceSet[ix].item != group)
				return FALSE ;
			return TRUE ;
		}

		void modify_space (const REAL &item ,VAR32 group) {
			_DEBUG_ASSERT_ (item != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (Attribute::mSelf.mSpaceSet.find (item) == VAR_NONE) ;
			Attribute::mSelf.mSpaceSet.add (item ,group) ;
		}

		BOOL varify_control (const REAL &item) const {
			const auto r1x = BOOL (item >= REAL (0) && item <= REAL (32)) ;
			const auto r2x = BOOL (item == REAL (127)) ;
			if (!r1x && !r2x)
				return FALSE ;
			if (varify_space (item))
				return FALSE ;
			return TRUE ;
		}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_str_xyz<REAL>::value) ;
	SharedRef<HEAP> mHeap ;
	PhanBuffer<const REAL> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextReader () {
		reset () ;
	}

	explicit TextReader (const PhanBuffer<const REAL> &stream) {
		mHeap = SharedRef<HEAP>::make () ;
		attr ().enable_endian (FALSE) ;
		attr ().enable_escape (FALSE) ;
		mStream = PhanBuffer<const REAL>::make (stream) ;
		reset () ;
	}

	Attribute &attr () const & {
		return _CAST_<Attribute> (mHeap.self) ;
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

	PhanBuffer<const REAL> raw () const & {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const REAL>::make (mStream ,length ()) ;
	}

	PhanBuffer<const REAL> raw () && = delete ;

	void reset () {
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	void reset (INDEX _read ,INDEX _write) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (BOOL (_read >= 0 && _read < mStream.size ())) ;
		_DEBUG_ASSERT_ (BOOL (_write >= 0 && _write < mStream.size ())) ;
		_DEBUG_ASSERT_ (_read <= _write) ;
		mRead = _read ;
		mWrite = _write ;
	}

	TextReader copy () popping {
		TextReader ret ;
		ret.mHeap = mHeap ;
		ret.mStream = PhanBuffer<const REAL>::make (mStream) ;
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

	void read (REAL &data) popping {
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (mRead < mWrite) ;
			for (FOR_ONCE_DO_WHILE) {
				data = attr ().convert_endian (mStream[mRead]) ;
				const auto r2x = attr ().varify_escape_r (data) ;
				mRead++ ;
				if (!r2x)
					discard ;
				_DYNAMIC_ASSERT_ (mRead < mWrite) ;
				data = attr ().convert_endian (mStream[mRead]) ;
				data = attr ().convert_escape_r (data) ;
				mRead++ ;
			}
		} ,[&] (BOOL &_case_req) {
			data = attr ().varify_ending_item () ;
		}) ;
	}

	inline TextReader &operator>> (REAL &data) popping {
		read (data) ;
		return *this ;
	}

	void read (BOOL &data) popping {
		auto rax = REAL () ;
		read (rax) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('t')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('r')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('u')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('e')) ;
			data = TRUE ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('T')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('R')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('U')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('E')) ;
			data = TRUE ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('f')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('a')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('l')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('s')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('e')) ;
			data = FALSE ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('F')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('A')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('L')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('S')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('E')) ;
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
		_DYNAMIC_ASSERT_ (BOOL (r1x >= VAR32_MIN && r1x <= VAR32_MAX)) ;
		data = VAR32 (r1x) ;
	}

	inline TextReader &operator>> (VAR32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAR64 &data) popping {
		auto rax = REAL () ;
		read (rax) ;
		const auto r1x = BOOL (rax == REAL ('-')) ;
		for (FOR_ONCE_DO_WHILE) {
			if (!BOOL (rax == REAL ('+') || rax == REAL ('-')))
				discard ;
			read (rax) ;
		}
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
		for (FOR_ONCE_DO_WHILE) {
			if (_ISINF_ (r1x))
				discard ;
			if (_ISNAN_ (r1x))
				discard ;
			_DYNAMIC_ASSERT_ (BOOL (r1x >= VAL32_MIN && r1x <= VAL32_MAX)) ;
		}
		data = VAL32 (r1x) ;
	}

	inline TextReader &operator>> (VAL32 &data) popping {
		read (data) ;
		return *this ;
	}

	void read (VAL64 &data) popping {
		auto rax = REAL () ;
		read (rax) ;
		const auto r1x = BOOL (rax == REAL ('-')) ;
		for (FOR_ONCE_DO_WHILE) {
			if (!BOOL (rax == REAL ('+') || rax == REAL ('-')))
				discard ;
			read (rax) ;
		}
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('i')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('n')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('f')) ;
			data = VAL64_INF ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('I')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('N')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('F')) ;
			data = VAL64_INF ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('n')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('a')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('n')) ;
			data = VAL64_NAN ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (rax == REAL ('N')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('A')) ;
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == REAL ('N')) ;
			data = VAL64_NAN ;
		} ,[&] (BOOL &_case_req) {
			const auto r2x = attr ().varify_number_item (rax) ;
			_CASE_REQUIRE_ (r2x) ;
			compute_read_number (data ,*this ,rax) ;
		} ,[&] (BOOL &_case_req) {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}) ;
		if (r1x)
			data = -data ;
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

	void read (const Plain<REAL> &data) {
		auto rax = REAL () ;
		for (INDEX i = 0 ; i < data.size () ; i++) {
			read (rax) ;
			_DYNAMIC_ASSERT_ (rax == data.self[i]) ;
		}
	}

	inline TextReader &operator>> (const Plain<REAL> &data) {
		read (data) ;
		return *this ;
	}

	template <class _ARG1>
	void read (String<REAL ,_ARG1> &data) popping {
		const auto r1x = next_string_size () ;
		_DYNAMIC_ASSERT_ (BOOL (r1x >= 0 && r1x < VAR32_MAX)) ;
		if (data.size () < r1x)
			data = String<REAL ,_ARG1> (r1x) ;
		data.clear () ;
		for (INDEX i = 0 ; i < r1x ; i++)
			read (data[i]) ;
	}

	template <class _ARG1>
	inline TextReader &operator>> (String<REAL ,_ARG1> &data) popping {
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

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<TextReader> ()))> ,void>::value>>
	void read (_ARG1 &data) popping {
		data.friend_read (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<_ARG1> ().friend_read (_NULL_<TextReader> ()))> ,void>::value>>
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
		auto rax = REAL () ;
		ris.read (rax) ;
		while (TRUE) {
			if (rax == attr ().varify_ending_item ())
				break ;
			if (attr ().varify_space (rax))
				break ;
			ret++ ;
			ris.read (rax) ;
		}
		return std::move (ret) ;
	}

	void compute_read_number (VAR64 &data ,TextReader &reader ,REAL &top) const {
		_DEBUG_ASSERT_ (attr ().varify_number_item (top)) ;
		auto ris = reader.copy () ;
		data = attr ().convert_number_r (top) ;
		ris.read (top) ;
		while (TRUE) {
			if (!attr ().varify_number_item (top))
				break ;
			const auto r1x = data * attr ().varify_radix () + attr ().convert_number_r (top) ;
			_DYNAMIC_ASSERT_ (data <= r1x) ;
			data = r1x ;
			reader = ris.copy () ;
			ris.read (top) ;
		}
	}

	void compute_read_number (VAL64 &data ,TextReader &reader ,REAL &top) const {
		_DEBUG_ASSERT_ (attr ().varify_number_item (top)) ;
		auto ris = reader.copy () ;
		while (TRUE) {
			if (attr ().convert_number_r (top) != 0)
				break ;
			reader = ris.copy () ;
			ris.read (top) ;
		}
		auto rax = ARRAY3<VAR64> {0 ,0 ,0} ;
		for (FOR_ONCE_DO_WHILE) {
			if (!attr ().varify_number_item (top))
				discard ;
			rax[0] = attr ().convert_number_r (top) ;
			reader = ris.copy () ;
			ris.read (top) ;
			while (TRUE) {
				if (!attr ().varify_number_item (top))
					break ;
				_CALL_IF_ ([&] (BOOL &_case_req) {
					const auto r1x = rax[0] * attr ().varify_radix () + attr ().convert_number_r (top) ;
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
			if (top != REAL ('.'))
				discard ;
			reader = ris.copy () ;
			ris.read (top) ;
			_DYNAMIC_ASSERT_ (attr ().varify_number_item (top)) ;
			while (TRUE) {
				if (!attr ().varify_number_item (top))
					break ;
				for (FOR_ONCE_DO_WHILE) {
					const auto r2x = rax[0] * attr ().varify_radix () + attr ().convert_number_r (top) ;
					if (rax[0] > r2x)
						discard ;
					rax[0] = r2x ;
					rax[1]-- ;
				}
				reader = ris.copy () ;
				ris.read (top) ;
			}
		}
		for (FOR_ONCE_DO_WHILE) {
			if (!BOOL (top == REAL ('e') || top == REAL ('E')))
				discard ;
			const auto r3x = ris.template read<VAR32> () ;
			rax[1] += r3x ;
			reader = ris.copy () ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (rax[0] >= 0)
				discard ;
			rax[0] = -rax[0] ;
			rax[2] = -1 ;
			if (rax[0] >= 0)
				discard ;
			rax[0] = -(rax[0] / attr ().varify_radix ()) ;
			rax[1]++ ;
		}
		const auto r4x = _IEEE754_E10TOE2_ (rax) ;
		data = _IEEE754_ENCODE_ (r4x) ;
	}
} ;

template <class REAL>
class TextWriter {
private:
	struct HEAP {
		SharedRef<FixedBuffer<REAL>> mBuffer ;
		BOOL mEndianFlag ;
		BOOL mEscapeFlag ;
		HashSet<REAL ,REAL> mEscapeSet ;
	} ;

	class Attribute :private Wrapped<HEAP> {
	public:
		REAL varify_ending_item () const {
			return REAL ('\0') ;
		}

		void enable_endian (BOOL flag) {
			Attribute::mSelf.mEndianFlag = flag ;
		}

		REAL convert_endian (const REAL &item) const {
			TEMP<REAL> ret ;
			const auto r1x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (REAL)]> &)>> (_MEMRCOPY_) ;
			const auto r2x = _XVALUE_<PTR<void (ARR<BYTE> & ,const DEF<BYTE[_SIZEOF_ (REAL)]> &)>> (_MEMCOPY_) ;
			const auto r3x = Attribute::mSelf.mEndianFlag ? r1x : r2x ;
			_ZERO_ (ret) ;
			r3x (PTRTOARR[ret.unused] ,_CAST_<BYTE[_SIZEOF_ (REAL)]> (item)) ;
			return std::move (_CAST_<REAL> (ret)) ;
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

		BOOL varify_number_item (const REAL &item) const {
			if (!BOOL (item >= REAL ('0') && item <= REAL ('9')))
				return FALSE ;
			return TRUE ;
		}

		REAL convert_number_w (VAR64 number) const {
			return REAL (REAL ('0') + number) ;
		}

		void enable_escape (BOOL flag) {
			Attribute::mSelf.mEscapeFlag = flag ;
		}

		REAL varify_escape_item () const {
			return REAL ('\\') ;
		}

		BOOL varify_escape_w (const REAL &key) const {
			if (!Attribute::mSelf.mEscapeFlag)
				return FALSE ;
			if (Attribute::mSelf.mEscapeSet.find (key) == VAR_NONE)
				return FALSE ;
			return TRUE ;
		}

		void modify_escape_w (const REAL &str_a ,const REAL &str_e) {
			_DEBUG_ASSERT_ (str_a != varify_ending_item ()) ;
			_DEBUG_ASSERT_ (Attribute::mSelf.mEscapeSet.find (str_e) == VAR_NONE) ;
			Attribute::mSelf.mEscapeSet.add (str_e ,str_a) ;
		}

		REAL convert_escape_w (const REAL &str_e) const {
			INDEX ix = Attribute::mSelf.mEscapeSet.find (str_e) ;
			_DYNAMIC_ASSERT_ (ix != VAR_NONE) ;
			return Attribute::mSelf.mEscapeSet[ix].item ;
		}

		BOOL varify_space (const REAL &item) const {
			if (!BOOL (item == REAL ('\r') || item == REAL ('\n')))
				return FALSE ;
			return TRUE ;
		}

		BOOL varify_control (const REAL &item) const {
			const auto r1x = BOOL (item >= REAL (0) && item <= REAL (32)) ;
			const auto r2x = BOOL (item == REAL (127)) ;
			if (!r1x && !r2x)
				return FALSE ;
			if (varify_space (item))
				return FALSE ;
			return TRUE ;
		}
	} ;

private:
	_STATIC_ASSERT_ (stl::is_str_xyz<REAL>::value) ;
	SharedRef<HEAP> mHeap ;
	PhanBuffer<REAL> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextWriter () {
		reset () ;
	}

	explicit TextWriter (const PhanBuffer<REAL> &stream) {
		mHeap = SharedRef<HEAP>::make () ;
		attr ().enable_endian (FALSE) ;
		attr ().enable_escape (FALSE) ;
		mStream = PhanBuffer<REAL>::make (stream) ;
		reset () ;
	}

	explicit TextWriter (SharedRef<FixedBuffer<REAL>> &&stream) {
		mHeap = SharedRef<HEAP>::make () ;
		mHeap->mBuffer = std::move (stream) ;
		attr ().enable_endian (FALSE) ;
		attr ().enable_escape (FALSE) ;
		mStream = PhanBuffer<REAL>::make (mHeap->mBuffer.self) ;
		reset () ;
	}

	Attribute &attr () const & {
		return _CAST_<Attribute> (mHeap.self) ;
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

	PhanBuffer<const REAL> raw () const & {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const REAL>::make (mStream ,length ()) ;
	}

	PhanBuffer<const REAL> raw () && = delete ;

	void reset () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	void reset (INDEX _read ,INDEX _write) {
		_DEBUG_ASSERT_ (mHeap.exist ()) ;
		_DEBUG_ASSERT_ (BOOL (_read >= 0 && _read < mStream.size ())) ;
		_DEBUG_ASSERT_ (BOOL (_write >= 0 && _write < mStream.size ())) ;
		_DEBUG_ASSERT_ (_read <= _write) ;
		mRead = _read ;
		mWrite = _write ;
	}

	TextWriter copy () popping {
		TextWriter ret ;
		ret.mHeap = mHeap ;
		ret.mStream = PhanBuffer<REAL>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void write (const REAL &data) {
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (attr ().varify_escape_w (data)) ;
			_DYNAMIC_ASSERT_ (mWrite + 1 < mStream.size ()) ;
			const auto r1x = attr ().varify_escape_item () ;
			mStream[mWrite] = attr ().convert_endian (r1x) ;
			mWrite++ ;
			const auto r2x = attr ().convert_escape_w (data) ;
			mStream[mWrite] = attr ().convert_endian (r2x) ;
			mWrite++ ;
		} ,[&] (BOOL &_case_req) {
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = attr ().convert_endian (data) ;
			mWrite++ ;
		}) ;
	}

	inline TextWriter &operator<< (const REAL &data) {
		write (data) ;
		return *this ;
	}

	void write (const BOOL &data) {
		static constexpr auto M_TRUE = PACK<REAL[4]> ({
			REAL ('t') ,REAL ('r') ,REAL ('u') ,REAL ('e')}) ;
		static constexpr auto M_FALSE = PACK<REAL[5]> ({
			REAL ('f') ,REAL ('a') ,REAL ('l') ,REAL ('s') ,REAL ('e')}) ;
		const auto r1x = data ? (PhanBuffer<const REAL>::make (M_TRUE.P1)) : (PhanBuffer<const REAL>::make (M_FALSE.P1)) ;
		write (r1x) ;
	}

	inline TextWriter &operator<< (const BOOL &data) {
		write (data) ;
		return *this ;
	}

	void write (const PTR<const VOID> &) = delete ;

	inline TextWriter &operator<< (const PTR<const VOID> &) = delete ;

	void write (const VAR32 &data) {
		write (VAR64 (data)) ;
	}

	inline TextWriter &operator<< (const VAR32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAR64 &data) {
		auto rax = Buffer<REAL ,ARGC<128>> () ;
		INDEX ix = rax.size () ;
		compute_write_number (data ,PhanBuffer<REAL>::make (rax) ,ix) ;
		write (PhanBuffer<const REAL>::make (PTRTOARR[&rax.self[ix]] ,(rax.size () - ix))) ;
	}

	inline TextWriter &operator<< (const VAR64 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL32 &data) {
		static constexpr auto M_NAN = PACK<REAL[3]> ({
			REAL ('n') ,REAL ('a') ,REAL ('n')}) ;
		static constexpr auto M_INF = PACK<REAL[4]> ({
			REAL ('+') ,REAL ('i') ,REAL ('n') ,REAL ('f')}) ;
		static constexpr auto M_SINF = PACK<REAL[4]> ({
			REAL ('-') ,REAL ('i') ,REAL ('n') ,REAL ('f')}) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISNAN_ (data)) ;
			write (PhanBuffer<const REAL>::make (M_NAN.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data > 0) ;
			write (PhanBuffer<const REAL>::make (M_INF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data < 0) ;
			write (PhanBuffer<const REAL>::make (M_SINF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			auto rax = Buffer<REAL ,ARGC<256>> () ;
			INDEX ix = rax.size () ;
			const auto r1x = attr ().varify_val32_precision () ;
			compute_write_number (data ,r1x ,PhanBuffer<REAL>::make (rax) ,ix) ;
			write (PhanBuffer<const REAL>::make (PTRTOARR[&rax.self[ix]] ,(rax.size () - ix))) ;
		}) ;
	}

	inline TextWriter &operator<< (const VAL32 &data) {
		write (data) ;
		return *this ;
	}

	void write (const VAL64 &data) {
		static constexpr auto M_NAN = PACK<REAL[3]> ({
			REAL ('n') ,REAL ('a') ,REAL ('n')}) ;
		static constexpr auto M_INF = PACK<REAL[4]> ({
			REAL ('+') ,REAL ('i') ,REAL ('n') ,REAL ('f')}) ;
		static constexpr auto M_SINF = PACK<REAL[4]> ({
			REAL ('-') ,REAL ('i') ,REAL ('n') ,REAL ('f')}) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISNAN_ (data)) ;
			write (PhanBuffer<const REAL>::make (M_NAN.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data > 0) ;
			write (PhanBuffer<const REAL>::make (M_INF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (_ISINF_ (data)) ;
			_CASE_REQUIRE_ (data < 0) ;
			write (PhanBuffer<const REAL>::make (M_SINF.P1)) ;
		} ,[&] (BOOL &_case_req) {
			auto rax = Buffer<REAL ,ARGC<256>> () ;
			INDEX ix = rax.size () ;
			const auto r1x = attr ().varify_val64_precision () ;
			compute_write_number (data ,r1x ,PhanBuffer<REAL>::make (rax) ,ix) ;
			write (PhanBuffer<const REAL>::make (PTRTOARR[&rax.self[ix]] ,(rax.size () - ix))) ;
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

	void write (const Plain<REAL> &data) {
		for (INDEX i = 0 ; i < data.size () ; i++)
			write (data.self[i]) ;
	}

	inline TextWriter &operator<< (const Plain<REAL> &data) {
		write (data) ;
		return *this ;
	}

	template <class _ARG1>
	void write (const String<REAL ,_ARG1> &data) {
		const auto r1x = data.length () ;
		_DYNAMIC_ASSERT_ (BOOL (r1x >= 0 && r1x < VAR32_MAX)) ;
		for (auto &&i : data)
			write (i) ;
	}

	template <class _ARG1>
	inline TextWriter &operator<< (const String<REAL ,_ARG1> &data) {
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

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<TextWriter> ()))> ,void>::value>>
	void write (const _ARG1 &data) {
		data.friend_write (*this) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const _ARG1> ().friend_write (_NULL_<TextWriter> ()))> ,void>::value>>
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
	void compute_write_number (const VAR64 &data ,const PhanBuffer<REAL> &out ,INDEX &it) const {
		INDEX iw = it ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (data > 0) ;
			auto rax = data ;
			while (TRUE) {
				if (rax == 0)
					break ;
				out[--iw] = attr ().convert_number_w (rax % attr ().varify_radix ()) ;
				rax /= attr ().varify_radix () ;
			}
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (data < 0) ;
			auto rax = data ;
			while (TRUE) {
				if (rax == 0)
					break ;
				out[--iw] = attr ().convert_number_w (-rax % attr ().varify_radix ()) ;
				rax /= attr ().varify_radix () ;
			}
			out[--iw] = REAL ('-') ;
		} ,[&] (BOOL &_case_req) {
			_CASE_REQUIRE_ (data == 0) ;
			out[--iw] = attr ().convert_number_w (0) ;
		}) ;
		it = iw ;
	}

	void compute_write_number (const VAL64 &data ,LENGTH precision ,const PhanBuffer<REAL> &out ,INDEX &it) const {
		INDEX iw = it ;
		const auto r1x = _IEEE754_DECODE_ (data) ;
		auto rax = _IEEE754_E2TOE10_ (r1x) ;
		const auto r3x = log_of_number (rax[0]) ;
		for (FOR_ONCE_DO_WHILE) {
			const auto r4x = r3x - precision ;
			for (INDEX i = 0 ,ie = r4x - 1 ; i < ie ; i++) {
				rax[0] /= attr ().varify_radix () ;
				rax[1]++ ;
			}
			if (r4x <= 0)
				discard ;
			rax[0] = _ROUND_ (rax[0] ,attr ().varify_radix ()) / attr ().varify_radix () ;
			rax[1]++ ;
		}
		const auto r8x = log_of_number (rax[0]) ;
		_CALL_IF_ ([&] (BOOL &_case_req) {
			//@info: case 'x.xxxExxx'
			const auto r11x = r8x - 1 + rax[1] ;
			_CASE_REQUIRE_ (_ABS_ (r11x) >= precision) ;
			compute_write_number (r11x ,out ,iw) ;
			for (FOR_ONCE_DO_WHILE) {
				if (r11x <= 0)
					discard ;
				out[--iw] = REAL ('+') ;
			}
			out[--iw] = REAL ('e') ;
			const auto r5x = _MAX_ ((r8x - 1 - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= attr ().varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ,ie = r8x - 1 ; i < ie ; i++) {
				out[--iw] = attr ().convert_number_w (rax[0] % attr ().varify_radix ()) ;
				iw += EFLAG (out[ix] == attr ().convert_number_w (0)) ;
				rax[0] /= attr ().varify_radix () ;
			}
			out[--iw] = REAL ('.') ;
			iw += EFLAG (out[ix] == REAL ('.')) ;
			out[--iw] = attr ().convert_number_w (rax[0] % attr ().varify_radix ()) ;
			rax[0] /= attr ().varify_radix () ;
		} ,[&] (BOOL &_case_req) {
			//@info: case 'xxx000'
			_CASE_REQUIRE_ (rax[1] > 0) ;
			for (INDEX i = 0 ,ie = LENGTH (rax[1]) ; i < ie ; i++)
				out[--iw] = attr ().convert_number_w (0) ;
			for (INDEX i = 0 ; i < r8x ; i++) {
				out[--iw] = attr ().convert_number_w (rax[0] % attr ().varify_radix ()) ;
				rax[0] /= attr ().varify_radix () ;
			}
		} ,[&] (BOOL &_case_req) {
			//@info: case 'xxx.xxx'
			_CASE_REQUIRE_ (rax[1] >= 1 - r8x) ;
			_CASE_REQUIRE_ (rax[1] < 0) ;
			const auto r7x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r7x ; i++)
				rax[0] /= attr ().varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r7x ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = attr ().convert_number_w (rax[0] % attr ().varify_radix ()) ;
				iw += EFLAG (out[ix] == attr ().convert_number_w (0)) ;
				rax[0] /= attr ().varify_radix () ;
			}
			out[--iw] = REAL ('.') ;
			iw += EFLAG (out[ix] == REAL ('.')) ;
			for (INDEX i = 0 ,ie = LENGTH (r8x + rax[1]) ; i < ie ; i++) {
				out[--iw] = attr ().convert_number_w (rax[0] % attr ().varify_radix ()) ;
				rax[0] /= attr ().varify_radix () ;
			}
		} ,[&] (BOOL &_case_req) {
			//@info: case '0.000xxx'
			_CASE_REQUIRE_ (rax[1] < 1 - r8x) ;
			_CASE_REQUIRE_ (rax[1] < 0) ;
			const auto r6x = _MAX_ (LENGTH (-rax[1] - precision) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r6x ; i++)
				rax[0] /= attr ().varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r6x ; i < r8x ; i++) {
				out[--iw] = attr ().convert_number_w (rax[0] % attr ().varify_radix ()) ;
				iw += EFLAG (out[ix] == attr ().convert_number_w (0)) ;
				rax[0] /= attr ().varify_radix () ;
			}
			for (INDEX i = _MAX_ (r6x ,r8x) ,ie = LENGTH (-rax[1]) ; i < ie ; i++) {
				out[--iw] = attr ().convert_number_w (0) ;
				iw += EFLAG (out[ix] == attr ().convert_number_w (0)) ;
			}
			out[--iw] = REAL ('.') ;
			iw += EFLAG (out[ix] == REAL ('.')) ;
			out[--iw] = attr ().convert_number_w (0) ;
		} ,[&] (BOOL &_case_req) {
			//@info: case '0'
			_CASE_REQUIRE_ (rax[0] == 0) ;
			out[--iw] = attr ().convert_number_w (0) ;
		}) ;
		for (FOR_ONCE_DO_WHILE) {
			if (rax[2] == 0)
				discard ;
			out[--iw] = REAL ('-') ;
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
			rax *= attr ().varify_radix () ;
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
	if (!BOOL (rax == M_BOM.P1[0] || rax == M_BOM.P1[1]))
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
	if (!BOOL (rax == M_BOM.P1[0] || rax == M_BOM.P1[1]))
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
	class SKIP_GAP_ENDLINE_ONLY_TYPE ;
	class SKIP_LINE_TYPE ;

public:
	static constexpr auto HINT_IDENTIFY_TEXT = ARGV<HINT_IDENTIFY_TEXT_TYPE> {} ;
	static constexpr auto HINT_VALUE_TEXT = ARGV<HINT_VALUE_TEXT_TYPE> {} ;
	static constexpr auto HINT_STRING_TEXT = ARGV<HINT_STRING_TEXT_TYPE> {} ;
	static constexpr auto HINT_NEWGAP_TEXT = ARGV<HINT_NEWGAP_TEXT_TYPE> {} ;
	static constexpr auto HINT_NEWLINE_TEXT = ARGV<HINT_NEWLINE_TEXT_TYPE> {} ;
	static constexpr auto SKIP_GAP = ARGV<SKIP_GAP_TYPE> {} ;
	static constexpr auto SKIP_GAP_SPACE_ONLY = ARGV<SKIP_GAP_SPACE_ONLY_TYPE> {} ;
	static constexpr auto SKIP_GAP_ENDLINE_ONLY = ARGV<SKIP_GAP_ENDLINE_ONLY_TYPE> {} ;
	static constexpr auto SKIP_LINE = ARGV<SKIP_LINE_TYPE> {} ;
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
			_DEBUG_ASSERT_ (BOOL (index >= 0 && index < mCache.length ())) ;
			_DEBUG_ASSERT_ (BOOL (mPeek >= 0 && mPeek < mCache.length ())) ;
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
		r1.modify_space (STRU8 ('\v') ,1) ;
		r1.modify_space (STRU8 ('\r') ,2) ;
		r1.modify_space (STRU8 ('\n') ,2) ;
		r1.modify_space (STRU8 ('\f') ,2) ;
		r1.modify_escape_r (STRU8 ('t') ,STRU8 ('\t')) ;
		r1.modify_escape_r (STRU8 ('v') ,STRU8 ('\v')) ;
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
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < mCache.length ())) ;
		_DEBUG_ASSERT_ (BOOL (mPeek >= 0 && mPeek < mCache.length ())) ;
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

	void read (const ARGV<LLTextReader<void>::HINT_IDENTIFY_TEXT_TYPE> &) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_identifer_size () ;
	}

	void read (const ARGV<LLTextReader<void>::HINT_VALUE_TEXT_TYPE> &) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_value_size () ;
	}

	void read (const ARGV<LLTextReader<void>::HINT_STRING_TEXT_TYPE> &) {
		mHintStringTextFlag = TRUE ;
		mHintNextTextSize = next_string_size () ;
	}

	void read (const ARGV<LLTextReader<void>::HINT_NEWGAP_TEXT_TYPE> &) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_gap_text_size () ;
	}

	void read (const ARGV<LLTextReader<void>::HINT_NEWLINE_TEXT_TYPE> &) {
		mHintStringTextFlag = FALSE ;
		mHintNextTextSize = next_newline_text_size () ;
	}

	void read (const ARGV<LLTextReader<void>::SKIP_GAP_TYPE> &) {
		while (mReader->attr ().varify_space (get (0)))
			read () ;
	}

	void read (const ARGV<LLTextReader<void>::SKIP_GAP_SPACE_ONLY_TYPE> &) {
		while (mReader->attr ().varify_space (get (0) ,1))
			read () ;
	}

	void read (const ARGV<LLTextReader<void>::SKIP_GAP_ENDLINE_ONLY_TYPE> &) {
		while (mReader->attr ().varify_space (get (0) ,2))
			read () ;
	}

	void read (const ARGV<LLTextReader<void>::SKIP_LINE_TYPE> &) {
		auto rax = STRU8 () ;
		while (TRUE) {
			rax = get (0) ;
			const auto r2x = BOOL (rax == STRU8 ('\r') || rax == STRU8 ('\n') || rax == STRU8 ('\f')) ;
			if (r2x)
				break ;
			read () ;
		}
		if (rax != STRU8 ('\r'))
			return ;
		const auto r3x = get (0) ;
		if (r3x != STRU8 ('\n'))
			return ;
		read () ;
	}

	template <class _ARG1>
	inline LLTextReader &operator>> (const ARGV<_ARG1> &) {
		read (_NULL_<ARGV<_ARG1>> ()) ;
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
				discard ;
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
				discard ;
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
			for (FOR_ONCE_DO_WHILE) {
				if (ret > 0)
					discard ;
				_DYNAMIC_ASSERT_ (BOOL (r2x || r3x || r4x)) ;
			}
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
			if (!BOOL (ris[0] == STRU8 ('+') || ris[0] == STRU8 ('-')))
				discard ;
			ris++ ;
			ret++ ;
		}
		const auto r1x = ris[0] ;
		_DYNAMIC_ASSERT_ (BOOL (r1x >= STRU8 ('0') && r1x <= STRU8 ('9'))) ;
		ris++ ;
		ret++ ;
		while (TRUE) {
			if (r1x == STRU8 ('0'))
				break ;
			if (!BOOL (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')))
				break ;
			ris++ ;
			ret++ ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (ris[0] != STRU8 ('.'))
				discard ;
			ris++ ;
			ret++ ;
			while (TRUE) {
				if (!BOOL (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')))
					break ;
				ris++ ;
				ret++ ;
			}
		}
		for (FOR_ONCE_DO_WHILE) {
			if (!BOOL (ris[0] == STRU8 ('e') || ris[0] == STRU8 ('E')))
				discard ;
			ris++ ;
			ret++ ;
			for (FOR_ONCE_DO_WHILE) {
				if (!BOOL (ris[0] == STRU8 ('+') || ris[0] == STRU8 ('-')))
					discard ;
				ris++ ;
				ret++ ;
			}
			_DYNAMIC_ASSERT_ (BOOL (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9'))) ;
			ris++ ;
			ret++ ;
			while (TRUE) {
				if (!BOOL (ris[0] >= STRU8 ('0') && ris[0] <= STRU8 ('9')))
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