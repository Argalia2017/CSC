#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_array.hpp"

namespace CSC {
namespace U {
template <LENGTH>
struct BYTE_TRAITS ;

template <>
struct BYTE_TRAITS<_SIZEOF_ (BYTE)> {
	using TYPE = BYTE ;
} ;

template <>
struct BYTE_TRAITS<_SIZEOF_ (WORD)> {
	using TYPE = WORD ;
} ;

template <>
struct BYTE_TRAITS<_SIZEOF_ (CHAR)> {
	using TYPE = CHAR ;
} ;

template <>
struct BYTE_TRAITS<_SIZEOF_ (DATA)> {
	using TYPE = DATA ;
} ;

template <LENGTH>
struct TEXT_TRAITS ;

template <>
struct TEXT_TRAITS<_SIZEOF_ (STRU8)> {
	using TYPE = STRU8 ;
} ;

template <>
struct TEXT_TRAITS<_SIZEOF_ (STRU16)> {
	using TYPE = STRU16 ;
} ;

template <>
struct TEXT_TRAITS<_SIZEOF_ (STRU32)> {
	using TYPE = STRU32 ;
} ;
} ;

template <class _ARG>
using BYTE_TRAITS_TYPE = typename U::BYTE_TRAITS<_SIZEOF_ (_ARG)>::TYPE ;

template <class _ARG>
using TEXT_TRAITS_TYPE = typename U::TEXT_TRAITS<_SIZEOF_ (_ARG)>::TYPE ;

using STRUA = TEXT_TRAITS_TYPE<STRA> ;
using STRUW = TEXT_TRAITS_TYPE<STRW> ;

namespace U {
template <class>
struct ENDIAN_TRAITS ;

template <>
struct ENDIAN_TRAITS<BYTE> {
	//@info:'big endian' [0] -> 0X00
	static constexpr BYTE value = BYTE (0X00) ;
} ;

template <>
struct ENDIAN_TRAITS<WORD> {
	//@info:'big endian' [0] -> 0X00
	static constexpr WORD value = WORD (0X0001) ;
} ;

template <>
struct ENDIAN_TRAITS<CHAR> {
	//@info:'big endian' [0] -> 0X00
	static constexpr CHAR value = CHAR (0X00010203) ;
} ;

template <>
struct ENDIAN_TRAITS<DATA> {
	//@info:'big endian' [0] -> 0X00
	static constexpr DATA value = DATA (0X0001020304050607) ;
} ;
} ;

template <class _ARG>
class EndianBytes :public Wrapped<BYTE[_SIZEOF_ (_ARG)]> {
public:
	inline BYTE &operator[] (INDEX index) {
		return EndianBytes::mData[_CAST_<BYTE[_SIZEOF_ (_ARG)]> (U::ENDIAN_TRAITS<_ARG>::value)[index]] ;
	}

	inline const BYTE &operator[] (INDEX index) const {
		return EndianBytes::mData[_CAST_<BYTE[_SIZEOF_ (_ARG)]> (U::ENDIAN_TRAITS<_ARG>::value)[index]] ;
	}
} ;

class ByteReader {
private:
	class Attribute {
	private:
		friend ByteReader ;

	public:
		BYTE varify_ending_item () const {
			return 0 ;
		}
	} ;

private:
	SharedRef<Attribute> mAttr ;
	PhanBuffer<const BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteReader () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit ByteReader (const PhanBuffer<const BYTE> &stream) {
		mAttr = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const BYTE>::make (stream) ;
		reset () ;
	}

	const ARR<BYTE> &to () const {
		return mStream ;
	}

	inline implicit operator const ARR<BYTE> & () const {
		return to () ;
	}

	PhanBuffer<const BYTE> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const BYTE>::make (mStream ,length ()) ;
	}

	Attribute &attr () {
		return mAttr ;
	}

	const Attribute &attr () const {
		return mAttr ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		return mWrite - mRead ;
	}

	ByteReader clone () const {
		ByteReader ret ;
		ret.mAttr = mAttr ;
		ret.mStream = PhanBuffer<const BYTE>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	template <class _RET>
	_RET read () popping {
		_RET ret ;
		read (ret) ;
		return std::move (ret) ;
	}

	void read (BYTE &val) {
		if (mRead + _SIZEOF_ (val) > mWrite) {
			val = mAttr->varify_ending_item () ;
		} else {
			val = mStream[mRead] ;
			mRead++ ;
		}
	}

	inline ByteReader &operator>> (BYTE &val) {
		read (val) ;
		return *this ;
	}

	void read (WORD &val) {
		auto &r1 = _CAST_<EndianBytes<REMOVE_CVR_TYPE<decltype (val)>>> (val) ;
		for (INDEX i = 0 ; i < _SIZEOF_ (val) ; i++)
			read (r1[i]) ;
	}

	inline ByteReader &operator>> (WORD &val) {
		read (val) ;
		return *this ;
	}

	void read (CHAR &val) {
		auto &r1 = _CAST_<EndianBytes<REMOVE_CVR_TYPE<decltype (val)>>> (val) ;
		for (INDEX i = 0 ; i < _SIZEOF_ (val) ; i++)
			read (r1[i]) ;
	}

	inline ByteReader &operator>> (CHAR &val) {
		read (val) ;
		return *this ;
	}

	void read (DATA &val) {
		auto &r1 = _CAST_<EndianBytes<REMOVE_CVR_TYPE<decltype (val)>>> (val) ;
		for (INDEX i = 0 ; i < _SIZEOF_ (val) ; i++)
			read (r1[i]) ;
	}

	inline ByteReader &operator>> (DATA &val) {
		read (val) ;
		return *this ;
	}

	void read (BOOL &val) {
		read (_CAST_<BYTE_TRAITS_TYPE<BOOL>> (val)) ;
	}

	inline ByteReader &operator>> (BOOL &val) {
		read (val) ;
		return *this ;
	}

	void read (PTR<VOID> &) = delete ;
	inline ByteReader &operator>> (PTR<VOID> &) = delete ;
	void read (PTR<const VOID> &) = delete ;
	inline ByteReader &operator>> (PTR<const VOID> &) = delete ;

	void read (VAR32 &val) {
		read (_CAST_<BYTE_TRAITS_TYPE<VAR32>> (val)) ;
	}

	inline ByteReader &operator>> (VAR32 &val) {
		read (val) ;
		return *this ;
	}

	void read (VAR64 &val) {
		read (_CAST_<BYTE_TRAITS_TYPE<VAR64>> (val)) ;
	}

	inline ByteReader &operator>> (VAR64 &val) {
		read (val) ;
		return *this ;
	}

	void read (VAL32 &val) {
		read (_CAST_<BYTE_TRAITS_TYPE<VAL32>> (val)) ;
	}

	inline ByteReader &operator>> (VAL32 &val) {
		read (val) ;
		return *this ;
	}

	void read (VAL64 &val) {
		read (_CAST_<BYTE_TRAITS_TYPE<VAL64>> (val)) ;
	}

	inline ByteReader &operator>> (VAL64 &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Array<_ARG1 ,_ARG2> &val) {
		const auto r1x = LENGTH (read<VAR32> ()) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (val.size () < r1x)
			val = Array<_ARG1 ,_ARG2> (r1x) ;
		for (INDEX i = 0 ; i < val.size () ; i++)
			read (_CAST_<BYTE_TRAITS_TYPE<_ARG1>> (val[i])) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (Array<_ARG1 ,_ARG2> &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG ,LENGTH _VAL>
	void read (const DEF<_ARG[_VAL]> &val) {
		_STATIC_ASSERT_ (std::is_same<_ARG ,STRU8>::value || std::is_same<_ARG ,STRU16>::value || std::is_same<_ARG ,STRU32>::value || std::is_same<_ARG ,STRA>::value || std::is_same<_ARG ,STRW>::value) ;
		auto ris = clone () ;
		auto rax = _ARG () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (val) ; i++) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == val[i]) ;
		}
		*this = std::move (ris) ;
	}

	template <class _ARG ,LENGTH _VAL>
	inline ByteReader &operator>> (const DEF<_ARG[_VAL]> &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (String<_ARG1 ,_ARG2> &val) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,STRU8>::value || std::is_same<_ARG1 ,STRU16>::value || std::is_same<_ARG1 ,STRU32>::value || std::is_same<_ARG1 ,STRA>::value || std::is_same<_ARG1 ,STRW>::value) ;
		const auto r1x = LENGTH (read<VAR32> ()) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (val.size () < r1x)
			val = String<_ARG1 ,_ARG2> (r1x) ;
		for (INDEX i = 0 ; i < val.size () ; i++)
			read (_CAST_<BYTE_TRAITS_TYPE<_ARG1>> (val[i])) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (String<_ARG1 ,_ARG2> &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<ByteReader> ())) ,void>::value>>
	void read (_ARG &val) {
		val.friend_read (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<ByteReader> ())) ,void>::value>>
	inline ByteReader &operator>> (_ARG &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Buffer<_ARG1 ,_ARG2> &val) {
		for (INDEX i = 0 ; i < val.size () ; i++)
			read (val[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteReader &operator>> (Buffer<_ARG1 ,_ARG2> &val) {
		read (val) ;
		return *this ;
	}

	void read (PTR<void (ByteReader &)> proc) {
		proc (*this) ;
	}

	inline ByteReader &operator>> (PTR<void (ByteReader &)> proc) {
		read (proc) ;
		return *this ;
	}
} ;

class ByteWriter {
private:
	class Attribute {
	private:
		friend ByteWriter ;
		SharedRef<FixedBuffer<BYTE>> mAgentHolder ;

	public:
		BYTE varify_ending_item () const {
			return 0 ;
		}
	} ;

private:
	SharedRef<Attribute> mAttr ;
	PhanBuffer<BYTE> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	ByteWriter () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit ByteWriter (const PhanBuffer<BYTE> &stream) {
		mAttr = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<BYTE>::make (stream) ;
		reset () ;
	}

	explicit ByteWriter (SharedRef<FixedBuffer<BYTE>> &&stream) {
		mAttr = SharedRef<Attribute>::make () ;
		mAttr->mAgentHolder = std::move (stream) ;
		mStream = PhanBuffer<BYTE>::make (mAttr->mAgentHolder.self) ;
		reset () ;
	}

	const ARR<BYTE> &to () const {
		return mStream ;
	}

	inline implicit operator const ARR<BYTE> & () const {
		return to () ;
	}

	PhanBuffer<const BYTE> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const BYTE>::make (mStream ,length ()) ;
	}

	Attribute &attr () {
		return mAttr ;
	}

	const Attribute &attr () const {
		return mAttr ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		return mWrite - mRead ;
	}

	ByteWriter clone () const {
		ByteWriter ret ;
		ret.mAttr = mAttr ;
		ret.mStream = PhanBuffer<BYTE>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	void write (const BYTE &val) {
		_DYNAMIC_ASSERT_ (mWrite + _SIZEOF_ (val) < mStream.size ()) ;
		mStream[mWrite] = val ;
	}

	inline ByteWriter &operator<< (const BYTE &val) {
		write (val) ;
		return *this ;
	}

	void write (const WORD &val) {
		auto &r1 = _CAST_<EndianBytes<REMOVE_CVR_TYPE<decltype (val)>>> (val) ;
		for (INDEX i = 0 ; i < _SIZEOF_ (val) ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const WORD &val) {
		write (val) ;
		return *this ;
	}

	void write (const CHAR &val) {
		auto &r1 = _CAST_<EndianBytes<REMOVE_CVR_TYPE<decltype (val)>>> (val) ;
		for (INDEX i = 0 ; i < _SIZEOF_ (val) ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const CHAR &val) {
		write (val) ;
		return *this ;
	}

	void write (const DATA &val) {
		auto &r1 = _CAST_<EndianBytes<REMOVE_CVR_TYPE<decltype (val)>>> (val) ;
		for (INDEX i = 0 ; i < _SIZEOF_ (val) ; i++)
			write (r1[i]) ;
	}

	inline ByteWriter &operator<< (const DATA &val) {
		write (val) ;
		return *this ;
	}

	void write (const BOOL &val) {
		write (_CAST_<BYTE_TRAITS_TYPE<BOOL>> (val)) ;
	}

	inline ByteWriter &operator<< (const BOOL &val) {
		write (val) ;
		return *this ;
	}

	void write (const PTR<VOID> &) = delete ;
	inline ByteWriter &operator<< (const PTR<VOID> &) = delete ;
	void write (const PTR<const VOID> &) = delete ;
	inline ByteWriter &operator<< (const PTR<const VOID> &) = delete ;

	void write (const VAR32 &val) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAR32>> (val)) ;
	}

	inline ByteWriter &operator<< (const VAR32 &val) {
		write (val) ;
		return *this ;
	}

	void write (const VAR64 &val) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAR64>> (val)) ;
	}

	inline ByteWriter &operator<< (const VAR64 &val) {
		write (val) ;
		return *this ;
	}

	void write (const VAL32 &val) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAL32>> (val)) ;
	}

	inline ByteWriter &operator<< (const VAL32 &val) {
		write (val) ;
		return *this ;
	}

	void write (const VAL64 &val) {
		write (_CAST_<BYTE_TRAITS_TYPE<VAL64>> (val)) ;
	}

	inline ByteWriter &operator<< (const VAL64 &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Array<_ARG1 ,_ARG2> &val) {
		const auto r1x = val.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		write (VAR32 (r1x)) ;
		for (auto &i : val)
			write (_CAST_<BYTE_TRAITS_TYPE<_ARG1>> (i)) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteWriter &operator<< (const Array<_ARG1 ,_ARG2> &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG ,LENGTH _VAL>
	void write (const DEF<_ARG[_VAL]> &val) {
		_STATIC_ASSERT_ (std::is_same<_ARG ,STRU8>::value || std::is_same<_ARG ,STRU16>::value || std::is_same<_ARG ,STRU32>::value || std::is_same<_ARG ,STRA>::value || std::is_same<_ARG ,STRW>::value) ;
		write (String<_ARG ,SCPHAN>::from (PhanBuffer<const _ARG>::make (val))) ;
	}

	template <class _ARG ,LENGTH _VAL>
	inline ByteWriter &operator<< (const DEF<_ARG[_VAL]> &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const String<_ARG1 ,_ARG2> &val) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,STRU8>::value || std::is_same<_ARG1 ,STRU16>::value || std::is_same<_ARG1 ,STRU32>::value || std::is_same<_ARG1 ,STRA>::value || std::is_same<_ARG1 ,STRW>::value) ;
		const auto r1x = val.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		write (VAR32 (r1x)) ;
		for (auto &i : val)
			write (_CAST_<BYTE_TRAITS_TYPE<_ARG1>> (i)) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteWriter &operator<< (const String<_ARG1 ,_ARG2> &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<ByteWriter> ())) ,void>::value>>
	void write (const _ARG &val) {
		val.friend_write (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<ByteWriter> ())) ,void>::value>>
	inline ByteWriter &operator<< (const _ARG &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Buffer<_ARG1 ,_ARG2> &val) {
		for (INDEX i = 0 ; i < val.size () ; i++)
			write (val[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline ByteWriter &operator<< (const Buffer<_ARG1 ,_ARG2> &val) {
		write (val) ;
		return *this ;
	}

	void write (PTR<void (ByteWriter &)> proc) {
		proc (*this) ;
	}

	inline ByteWriter &operator<< (PTR<void (ByteWriter &)> proc) {
		write (proc) ;
		return *this ;
	}
} ;

inline namespace STREAM {
inline VAL64 _DOUBLEENCODE_ (const ARRAY2<VAR64> &arg) {
	VAL64 ret = 0 ;
	auto &r1 = _CAST_<DATA> (ret) ;
	auto &r2 = _CAST_<ARRAY2<DATA>> (arg) ;
	const auto r1x = _CALL_ ([&] () {
		ARRAY3<DATA> ret = {r2[0] ,r2[1] ,r2[0]} ;
		if ((ret[0] & DATA (0X8000000000000000)) != 0)
			ret[0] = ~ret[0] + 1 ;
		while (ret[0] != 0 && (ret[0] & ~0X000FFFFFFFFFFFFF) != 0X0010000000000000) {
			ret[0] <<= 1 ;
			ret[1]-- ;
		}
		const auto r2x = VAR64 (ret[1]) <= -1075 ;
		while (VAR64 (ret[1]) <= -1075) {
			ret[0] >>= 1 ;
			ret[1]++ ;
		}
		ret[1] = ret[1] + 1075 - EFLAG (r2x) ;
		ret[1] <<= 52 ;
		if (ret[0] == 0)
			ret[1] = 0 ;
		return std::move (ret) ;
	}) ;
	_DYNAMIC_ASSERT_ ((r1x[0] & ~0X001FFFFFFFFFFFFF) == 0) ;
	_DYNAMIC_ASSERT_ ((r1x[1] & ~0X7FF0000000000000) == 0) ;
	r1 |= r1x[0] & DATA (0X000FFFFFFFFFFFFF) ;
	r1 |= r1x[1] & DATA (0X7FF0000000000000) ;
	r1 |= r1x[2] & DATA (0X8000000000000000) ;
	return std::move (ret) ;
}

inline ARRAY2<VAR64> _DOUBLEDECODE_ (const VAL64 &arg) {
	ARRAY2<VAR64> ret ;
	auto &r1 = _CAST_<ARRAY2<DATA>> (ret) ;
	auto &r2 = _CAST_<DATA> (arg) ;
	r1[0] = r2 & DATA (0X000FFFFFFFFFFFFF) ;
	const auto r1x = r2 & DATA (0X7FF0000000000000) ;
	if (r1x != 0)
		r1[0] |= DATA (0X0010000000000000) ;
	r1[1] = r1x >> 52 ;
	r1[1] -= DATA (r1x != 0 ? 1075 : 1074) ;
	while (r1[0] != 0 && (r1[0] & DATA (0X0000000000000001)) == 0) {
		r1[0] >>= 1 ;
		r1[1]++ ;
	}
	if ((r2 & DATA (0X8000000000000000)) != 0)
		r1[0] = ~r1[0] + 1 ;
	if (r2 == 0)
		r1 = {0 ,0} ;
	return std::move (ret) ;
}

inline import DEF<ARRAY2<VAR64> (const ARRAY2<VAR64> &arg)> _DOUBLEE2TOE10_ ;
inline import DEF<ARRAY2<VAR64> (const ARRAY2<VAR64> &arg)> _DOUBLEE10TOE2_ ;
} ;

template <class UNIT>
class TextReader {
private:
	class Attribute {
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

		template <class _ARG>
		_ARG convert_endian (const _ARG &arg) const {
			_STATIC_ASSERT_ (std::is_pod<_ARG>::value) ;
			_ARG ret = arg ;
			if (mEndianFlag)
				_MEMRCOPY_ (_CAST_<BYTE[_SIZEOF_ (_ARG)]> (ret) ,_CAST_<BYTE[_SIZEOF_ (_ARG)]> (arg) ,_SIZEOF_ (_ARG)) ;
			return std::move (ret) ;
		}

		UNIT varify_ending_item () const {
			return UNIT ('\0') ;
		}

		VAR32 varify_radix () const {
			return 10 ;
		}

		BOOL varify_number_item (const UNIT &item) const {
			return item >= UNIT ('0') && item <= UNIT ('9') ;
		}

		VAR32 convert_number_r (const UNIT &item) const {
			return VAR32 (item - UNIT ('0')) ;
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
			_DEBUG_ASSERT_ (item != varify_ending_item () && mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,0) ;
		}

		void modify_space (const UNIT &item ,VAR32 group) {
			_DEBUG_ASSERT_ (item != varify_ending_item () && mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,group) ;
		}

		BOOL varify_control (const UNIT &item) const {
			return ((item >= UNIT (0) && item <= UNIT (32)) || item == UNIT (127)) && !varify_space (item) ;
		}
	} ;

private:
	_STATIC_ASSERT_ (std::is_same<UNIT ,STRU8>::value || std::is_same<UNIT ,STRU16>::value || std::is_same<UNIT ,STRU32>::value || std::is_same<UNIT ,STRA>::value || std::is_same<UNIT ,STRW>::value) ;
	SharedRef<Attribute> mAttr ;
	PhanBuffer<const UNIT> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextReader () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit TextReader (const PhanBuffer<const UNIT> &stream) {
		mAttr = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<const UNIT>::make (stream) ;
		reset () ;
	}

	const ARR<UNIT> &to () const {
		return mStream ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	PhanBuffer<const UNIT> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const UNIT>::make (mStream ,length ()) ;
	}

	Attribute &attr () {
		return mAttr ;
	}

	const Attribute &attr () const {
		return mAttr ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		return mWrite - mRead ;
	}

	TextReader clone () const {
		TextReader ret ;
		ret.mAttr = mAttr ;
		ret.mStream = PhanBuffer<const UNIT>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = mStream.size () ;
	}

	template <class _RET>
	_RET read () popping {
		_RET ret ;
		read (ret) ;
		return std::move (ret) ;
	}

	void read (UNIT &val) {
		if (mRead >= mWrite) {
			val = mAttr->varify_ending_item () ;
		} else {
			val = mAttr->convert_endian (mStream[mRead]) ;
			mRead++ ;
			if (mAttr->varify_escape_r (val)) {
				_DYNAMIC_ASSERT_ (mRead < mWrite) ;
				val = mAttr->convert_endian (mStream[mRead]) ;
				mRead++ ;
				val = mAttr->convert_escape (val) ;
			}
		}
	}

	inline TextReader &operator>> (UNIT &val) {
		read (val) ;
		return *this ;
	}

	void read (BOOL &val) {
		auto ris = clone () ;
		auto rax = UNIT () ;
		ris.read (rax) ;
		if (rax == UNIT ('t')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('r')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('u')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('e')) ;
			val = TRUE ;
		} else if (rax == UNIT ('T')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('R')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('U')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('E')) ;
			val = TRUE ;
		} else if (rax == UNIT ('f')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('a')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('l')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('s')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('e')) ;
			val = FALSE ;
		} else if (rax == UNIT ('F')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('A')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('L')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('S')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('E')) ;
			val = FALSE ;
		} else {
			_DYNAMIC_ASSERT_ (FALSE) ;
		}
		*this = std::move (ris) ;
	}

	inline TextReader &operator>> (BOOL &val) {
		read (val) ;
		return *this ;
	}

	void read (PTR<VOID> &) = delete ;
	inline TextReader &operator>> (PTR<VOID> &) = delete ;
	void read (PTR<const VOID> &) = delete ;
	inline TextReader &operator>> (PTR<const VOID> &) = delete ;

	void read (VAR32 &val) {
		const auto r1x = read<VAR64> () ;
		_DYNAMIC_ASSERT_ (r1x >= VAR32_MIN && r1x <= VAR32_MAX) ;
		val = VAR32 (r1x) ;
	}

	inline TextReader &operator>> (VAR32 &val) {
		read (val) ;
		return *this ;
	}

	void read (VAR64 &val) {
		auto ris = clone () ;
		auto rax = UNIT () ;
		ris.read (rax) ;
		const auto r1x = rax == UNIT ('-') ;
		if (rax == UNIT ('+') || rax == UNIT ('-'))
			ris.read (rax) ;
		ris.read_number (val ,rax) ;
		val = r1x ? -val : val ;
		*this = std::move (ris) ;
	}

	inline TextReader &operator>> (VAR64 &val) {
		read (val) ;
		return *this ;
	}

	void read (VAL32 &val) {
		val = VAL32 (read<VAL64> ()) ;
	}

	inline TextReader &operator>> (VAL32 &val) {
		read (val) ;
		return *this ;
	}

	void read (VAL64 &val) {
		auto ris = clone () ;
		auto rax = UNIT () ;
		ris.read (rax) ;
		const auto r1x = rax == UNIT ('-') ;
		if (rax == UNIT ('+') || rax == UNIT ('-'))
			ris.read (rax) ;
		if (rax == UNIT ('i')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('n')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('f')) ;
			val = VAL64_INF ;
		} else if (rax == UNIT ('I')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('N')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('F')) ;
			val = VAL64_INF ;
		} else if (rax == UNIT ('n')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('a')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('n')) ;
			ris.read_ind (val ,rax ,r1x) ;
		} else if (rax == UNIT ('N')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('A')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('N')) ;
			ris.read_ind (val ,rax ,r1x) ;
		} else {
			ris.read_number (val ,rax) ;
		}
		val = r1x ? -val : val ;
		*this = std::move (ris) ;
	}

	inline TextReader &operator>> (VAL64 &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Array<_ARG1 ,_ARG2> &val) {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextReader &operator>> (Array<_ARG1 ,_ARG2> &val) {
		read (val) ;
		return *this ;
	}

	template <LENGTH _VAL>
	void read (const DEF<UNIT[_VAL]> &val) {
		auto ris = clone () ;
		auto rax = UNIT () ;
		for (INDEX i = 0 ; i < _COUNTOF_ (val) ; i++) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == val[i]) ;
		}
		*this = std::move (ris) ;
	}

	template <LENGTH _VAL>
	inline TextReader &operator>> (const DEF<UNIT[_VAL]> &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG>
	void read (String<UNIT ,_ARG> &val) {
		const auto r1x = _CALL_ ([&] () {
			LENGTH ret = 0 ;
			auto ris = clone () ;
			auto rax = UNIT () ;
			ris.read (rax) ;
			while (rax != mAttr->varify_ending_item () && !mAttr->varify_space (rax)) {
				ret++ ;
				ris.read (rax) ;
			}
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		if (val.size () < r1x)
			val = String<UNIT ,_ARG> (r1x) ;
		for (INDEX i = 0 ; i < val.size () ; i++)
			read (val[i]) ;
	}

	template <class _ARG>
	inline TextReader &operator>> (String<UNIT ,_ARG> &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<TextReader> ())) ,void>::value>>
	void read (_ARG &val) {
		val.friend_read (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<_ARG> ().friend_read (_NULL_<TextReader> ())) ,void>::value>>
	inline TextReader &operator>> (_ARG &val) {
		read (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void read (Buffer<_ARG1 ,_ARG2> &val) {
		for (INDEX i = 0 ; i < val.size () ; i++)
			read (val[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextReader &operator>> (Buffer<_ARG1 ,_ARG2> &val) {
		read (val) ;
		return *this ;
	}

	void read (PTR<void (TextReader &)> proc) {
		proc (*this) ;
	}

	inline TextReader &operator>> (PTR<void (TextReader &)> proc) {
		read (proc) ;
		return *this ;
	}

private:
	void read_number (VAR64 &val ,const UNIT &first) {
		auto ris = clone () ;
		auto rax = first ;
		_DYNAMIC_ASSERT_ (mAttr->varify_number_item (rax)) ;
		val = mAttr->convert_number_r (rax) ;
		ris.read (rax) ;
		while (mAttr->varify_number_item (rax)) {
			const auto r1x = val * mAttr->varify_radix () + mAttr->convert_number_r (rax) ;
			_DYNAMIC_ASSERT_ (val <= r1x) ;
			val = r1x ;
			*this = ris.clone () ;
			ris.read (rax) ;
		}
	}

	void read_number (VAL64 &val ,const UNIT &first) {
		const auto r1x = _CALL_ ([&] () {
			ARRAY2<VAR64> ret = {0 ,0} ;
			auto ris = clone () ;
			auto rax = first ;
			_DYNAMIC_ASSERT_ (mAttr->varify_number_item (rax)) ;
			ret[0] = 0 ;
			while (mAttr->convert_number_r (rax) == 0) {
				*this = ris.clone () ;
				ris.read (rax) ;
			}
			if (mAttr->varify_number_item (rax)) {
				ret[0] = mAttr->convert_number_r (rax) ;
				*this = ris.clone () ;
				ris.read (rax) ;
				while (mAttr->varify_number_item (rax)) {
					const auto r2x = ret[0] * mAttr->varify_radix () + mAttr->convert_number_r (rax) ;
					if (ret[0] <= r2x) {
						ret[0] = r2x ;
					} else {
						ret[1]++ ;
					}
					*this = ris.clone () ;
					ris.read (rax) ;
				}
			}
			if (rax == UNIT ('.')) {
				*this = ris.clone () ;
				ris.read (rax) ;
				_DYNAMIC_ASSERT_ (mAttr->varify_number_item (rax)) ;
				while (mAttr->varify_number_item (rax)) {
					const auto r2x = ret[0] * mAttr->varify_radix () + mAttr->convert_number_r (rax) ;
					if (ret[0] <= r2x) {
						ret[0] = r2x ;
						ret[1]-- ;
					}
					*this = ris.clone () ;
					ris.read (rax) ;
				}
			}
			if (rax == UNIT ('e') || rax == UNIT ('E')) {
				ret[1] += ris.read<VAR32> () ;
				*this = ris.clone () ;
			}
			return std::move (ret) ;
		}) ;
		const auto r3x = _DOUBLEE10TOE2_ (r1x) ;
		val = _DOUBLEENCODE_ (r3x) ;
	}

	void read_ind (VAL64 &val ,const UNIT &first ,BOOL negative) {
		auto ris = clone () ;
		auto rax = first ;
		ris.read (rax) ;
		const auto r1x = rax == UNIT ('(') ;
		if (r1x && first == UNIT ('n')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('i')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('n')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('d')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT (')')) ;
			*this = ris.clone () ;
		} else if (r1x && first == UNIT ('N')) {
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('I')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('N')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT ('D')) ;
			ris.read (rax) ;
			_DYNAMIC_ASSERT_ (rax == UNIT (')')) ;
			*this = ris.clone () ;
		}
		val = !negative && r1x ? VAL64_IND : VAL64_NAN ;
	}
} ;

template <class UNIT>
class TextWriter {
private:
	class Attribute {
	private:
		friend TextWriter ;
		SharedRef<FixedBuffer<UNIT>> mAgentHolder ;
		BOOL mEscapeFlag ;
		HashSet<UNIT ,UNIT> mEscapeSet ;
		HashSet<UNIT ,VAR32> mSpaceSet ;

	public:
		Attribute () {
			enable_escape (FALSE) ;
		}

		UNIT varify_ending_item () const {
			return UNIT ('\0') ;
		}

		VAR32 varify_radix () const {
			return 10 ;
		}

		BOOL varify_number_item (const UNIT &item) const {
			return item >= UNIT ('0') && item <= UNIT ('9') ;
		}

		UNIT convert_number_w (VAR32 number) const {
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
			_DEBUG_ASSERT_ (item != varify_ending_item () && mEscapeSet.find (key) == VAR_NONE) ;
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

		BOOL varify_space (const UNIT &item ,BOOL group) const {
			INDEX ix = mSpaceSet.find (item) ;
			return ix != VAR_NONE && mSpaceSet[ix].item == group ;
		}

		void modify_space (const UNIT &item) {
			_DEBUG_ASSERT_ (item != varify_ending_item () && mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,0) ;
		}

		void modify_space (const UNIT &item ,BOOL group) {
			_DEBUG_ASSERT_ (mSpaceSet.find (item) == VAR_NONE) ;
			mSpaceSet.add (item ,group) ;
		}

		BOOL varify_control (const UNIT &item) const {
			return ((item >= UNIT (0) && item <= UNIT (32)) || item == UNIT (127)) && !varify_space (item) ;
		}
	} ;

private:
	_STATIC_ASSERT_ (std::is_same<UNIT ,STRU8>::value || std::is_same<UNIT ,STRU16>::value || std::is_same<UNIT ,STRU32>::value || std::is_same<UNIT ,STRA>::value || std::is_same<UNIT ,STRW>::value) ;
	SharedRef<Attribute> mAttr ;
	PhanBuffer<UNIT> mStream ;
	INDEX mRead ;
	INDEX mWrite ;

public:
	TextWriter () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	explicit TextWriter (const PhanBuffer<UNIT> &stream) {
		mAttr = SharedRef<Attribute>::make () ;
		mStream = PhanBuffer<UNIT>::make (stream) ;
		reset () ;
	}

	explicit TextWriter (SharedRef<FixedBuffer<UNIT>> &&stream) {
		mAttr = SharedRef<Attribute>::make () ;
		mAttr->mAgentHolder = std::move (stream) ;
		mStream = PhanBuffer<UNIT>::make (mAttr->mAgentHolder.self) ;
		reset () ;
	}

	const ARR<UNIT> &to () const {
		return mStream ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	PhanBuffer<const UNIT> raw () const {
		_DYNAMIC_ASSERT_ (size () > 0) ;
		return PhanBuffer<const UNIT>::make (mStream ,length ()) ;
	}

	Attribute &attr () {
		return mAttr ;
	}

	const Attribute &attr () const {
		return mAttr ;
	}

	LENGTH size () const {
		return mStream.size () ;
	}

	LENGTH length () const {
		return mWrite - mRead ;
	}

	TextWriter clone () const {
		TextWriter ret ;
		ret.mAttr = mAttr ;
		ret.mStream = PhanBuffer<UNIT>::make (mStream) ;
		ret.mRead = mRead ;
		ret.mWrite = mWrite ;
		return std::move (ret) ;
	}

	void reset () {
		mRead = 0 ;
		mWrite = 0 ;
	}

	void write (const UNIT &val) {
		const auto r1x = val ;
		if (mAttr->varify_escape_w (r1x)) {
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = mAttr->varify_escape_item () ;
			mWrite++ ;
			const auto r2x = mAttr->convert_escape (r1x) ;
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = r2x ;
			mWrite++ ;
		} else {
			_DYNAMIC_ASSERT_ (mWrite < mStream.size ()) ;
			mStream[mWrite] = r1x ;
			mWrite++ ;
		}
	}

	inline TextWriter &operator<< (const UNIT &val) {
		write (val) ;
		return *this ;
	}

	void write (const BOOL &val) {
		static constexpr ARR<UNIT> M_TRUE = {UNIT ('t') ,UNIT ('r') ,UNIT ('u') ,UNIT ('e')} ;
		static constexpr ARR<UNIT> M_FALSE = {UNIT ('f') ,UNIT ('a') ,UNIT ('l') ,UNIT ('s') ,UNIT ('e')} ;
		const auto r1x = val ? PhanBuffer<const UNIT>::make (M_TRUE) : PhanBuffer<const UNIT>::make (M_FALSE) ;
		write (r1x) ;
	}

	inline TextWriter &operator<< (const BOOL &val) {
		write (val) ;
		return *this ;
	}

	void write (const PTR<VOID> &) = delete ;
	inline TextWriter &operator<< (const PTR<VOID> &) = delete ;
	void write (const PTR<const VOID> &) = delete ;
	inline TextWriter &operator<< (const PTR<const VOID> &) = delete ;

	void write (const VAR32 &val) {
		write (VAR64 (val)) ;
	}

	inline TextWriter &operator<< (const VAR32 &val) {
		write (val) ;
		return *this ;
	}

	void write (const VAR64 &val) {
		auto rax = Buffer<UNIT ,ARGC<128>> () ;
		INDEX iw = rax.size () ;
		write_number (val ,PhanBuffer<UNIT>::make (rax) ,iw) ;
		write (PhanBuffer<const UNIT>::make (&rax[iw] ,rax.size () - iw)) ;
	}

	inline TextWriter &operator<< (const VAR64 &val) {
		write (val) ;
		return *this ;
	}

	void write (const VAL32 &val) {
		write (VAL64 (val)) ;
	}

	inline TextWriter &operator<< (const VAL32 &val) {
		write (val) ;
		return *this ;
	}

	void write (const VAL64 &val) {
		static constexpr ARR<UNIT> M_NAN = {UNIT ('n') ,UNIT ('a') ,UNIT ('n')} ;
		static constexpr ARR<UNIT> M_IND = {UNIT ('-') ,UNIT ('n') ,UNIT ('a') ,UNIT ('n') ,UNIT ('(') ,UNIT ('i') ,UNIT ('n') ,UNIT ('d') ,UNIT (')')} ;
		static constexpr ARR<UNIT> M_INF = {UNIT ('i') ,UNIT ('n') ,UNIT ('f')} ;
		if (val != val) {
			const auto r1x = (_CAST_<DATA> (val) & DATA (0X8000000000000000)) != 0 ? PhanBuffer<const UNIT>::make (M_IND) : PhanBuffer<const UNIT>::make (M_NAN) ;
			write (r1x) ;
		} else if (val >= VAL64_INF) {
			write (PhanBuffer<const UNIT>::make (M_INF)) ;
		} else if (val <= -VAL64_INF) {
			write (UNIT ('-')) ;
			write (PhanBuffer<const UNIT>::make (M_INF)) ;
		} else {
			auto rax = Buffer<UNIT ,ARGC<256>> () ;
			INDEX iw = rax.size () ;
			write_number (val ,PhanBuffer<UNIT>::make (rax) ,iw) ;
			write (PhanBuffer<const UNIT>::make (&rax[iw] ,rax.size () - iw)) ;
		}
	}

	inline TextWriter &operator<< (const VAL64 &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Array<_ARG1 ,_ARG2> &val) {
		const auto r1x = val.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextWriter &operator<< (const Array<_ARG1 ,_ARG2> &val) {
		write (val) ;
		return *this ;
	}

	template <LENGTH _VAL>
	void write (const DEF<UNIT[_VAL]> &val) {
		write (String<UNIT ,SCPHAN>::from (PhanBuffer<const UNIT>::make (val))) ;
	}

	template <LENGTH _VAL>
	inline TextWriter &operator<< (const DEF<UNIT[_VAL]> &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG>
	void write (const String<UNIT ,_ARG> &val) {
		const auto r1x = val.length () ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		for (auto &i : val)
			write (i) ;
	}

	template <class _ARG>
	inline TextWriter &operator<< (const String<UNIT ,_ARG> &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<TextWriter> ())) ,void>::value>>
	void write (const _ARG &val) {
		val.friend_write (*this) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<decltype (_NULL_<const _ARG> ().friend_write (_NULL_<TextWriter> ())) ,void>::value>>
	inline TextWriter &operator<< (const _ARG &val) {
		write (val) ;
		return *this ;
	}

	template <class _ARG1 ,class _ARG2>
	void write (const Buffer<_ARG1 ,_ARG2> &val) {
		for (INDEX i = 0 ; i < val.size () ; i++)
			write (val[i]) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline TextWriter &operator<< (const Buffer<_ARG1 ,_ARG2> &val) {
		write (val) ;
		return *this ;
	}

	void write (PTR<void (TextWriter &)> proc) {
		proc (*this) ;
	}

	inline TextWriter &operator<< (PTR<void (TextWriter &)> proc) {
		write (proc) ;
		return *this ;
	}

private:
	void write_number (const VAR64 &val ,const PhanBuffer<UNIT> &tmp ,INDEX &iw) const {
		if (val > 0) {
			auto rax = val ;
			while (rax != 0) {
				tmp[--iw] = mAttr->convert_number_w (rax % mAttr->varify_radix ()) ;
				rax /= mAttr->varify_radix () ;
			}
		} else if (val < 0) {
			auto rax = val ;
			while (rax != 0) {
				tmp[--iw] = mAttr->convert_number_w (-rax % mAttr->varify_radix ()) ;
				rax /= mAttr->varify_radix () ;
			}
			tmp[--iw] = UNIT ('-') ;
		} else {
			tmp[--iw] = mAttr->convert_number_w (0) ;
		}
	}

	void write_number (const VAL64 &val ,const PhanBuffer<UNIT> &tmp ,INDEX &iw) const {
		const auto r1x = _DOUBLEDECODE_ (val) ;
		auto rax = _DOUBLEE2TOE10_ (r1x) ;
		const auto r2x = rax[0] < 0 ;
		rax[0] = r2x ? -rax[0] : rax[0] ;
		const auto r3x = log_of_number (rax[0]) ;
		if (_ABS_ (r3x - 1 + rax[1]) >= 6) {
			const auto r4x = _MAX_ (r3x - 7 ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r4x - 1 ; i++) {
				rax[0] /= mAttr->varify_radix () ;
				rax[1]++ ;
			}
			if (r4x > 0) {
				rax[0] = (rax[0] + mAttr->varify_radix () / 2) / mAttr->varify_radix () ;
				rax[1]++ ;
			}
		} else if (rax[1] >= 1 - r3x && rax[1] < 0) {
			const auto r4x = _MAX_ (VAR (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r4x - 1 ; i++) {
				rax[0] /= mAttr->varify_radix () ;
				rax[1]++ ;
			}
			if (r4x > 0) {
				rax[0] = (rax[0] + mAttr->varify_radix () / 2) / mAttr->varify_radix () ;
				rax[1]++ ;
			}
		} else if (rax[1] < 1 - r3x) {
			const auto r4x = _MAX_ (VAR (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r4x - 1 ; i++) {
				rax[0] /= mAttr->varify_radix () ;
				rax[1]++ ;
			}
			if (r4x > 0) {
				rax[0] = (rax[0] + mAttr->varify_radix () / 2) / mAttr->varify_radix () ;
				rax[1]++ ;
			}
		}
		const auto r4x = log_of_number (rax[0]) ;
		if (_ABS_ (r4x - 1 + rax[1]) >= 6) {
			//@info:case 'x.xxxExxx'
			write_number (r4x - 1 + rax[1] ,tmp ,iw) ;
			if (tmp[iw] != UNIT ('-'))
				tmp[--iw] = UNIT ('+') ;
			tmp[--iw] = UNIT ('e') ;
			const auto r5x = _MAX_ (r4x - 7 ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mAttr->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ; i < r4x - 1 ; i++) {
				tmp[--iw] = mAttr->convert_number_w (rax[0] % mAttr->varify_radix ()) ;
				if (tmp[ix] == mAttr->convert_number_w (0))
					iw++ ;
				rax[0] /= mAttr->varify_radix () ;
			}
			tmp[--iw] = UNIT ('.') ;
			if (tmp[ix] == UNIT ('.'))
				iw++ ;
			tmp[--iw] = mAttr->convert_number_w (rax[0] % mAttr->varify_radix ()) ;
			rax[0] /= mAttr->varify_radix () ;
		} else if (rax[1] > 0) {
			//@info:case 'xxx000'
			for (INDEX i = 0 ; i < VAR (rax[1]) ; i++)
				tmp[--iw] = mAttr->convert_number_w (0) ;
			for (INDEX i = 0 ; i < r4x ; i++) {
				tmp[--iw] = mAttr->convert_number_w (rax[0] % mAttr->varify_radix ()) ;
				rax[0] /= mAttr->varify_radix () ;
			}
		} else if (rax[1] >= 1 - r4x && rax[1] < 0) {
			//@info:case 'xxx.xxx'
			const auto r5x = _MAX_ (VAR (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mAttr->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ; i < VAR (-rax[1]) ; i++) {
				tmp[--iw] = mAttr->convert_number_w (rax[0] % mAttr->varify_radix ()) ;
				if (tmp[ix] == mAttr->convert_number_w (0))
					iw++ ;
				rax[0] /= mAttr->varify_radix () ;
			}
			tmp[--iw] = UNIT ('.') ;
			if (tmp[ix] == UNIT ('.'))
				iw++ ;
			for (INDEX i = 0 ; i < VAR (r4x + rax[1]) ; i++) {
				tmp[--iw] = mAttr->convert_number_w (rax[0] % mAttr->varify_radix ()) ;
				rax[0] /= mAttr->varify_radix () ;
			}
		} else if (rax[1] < 1 - r4x) {
			//@info:case '0.000xxx'
			const auto r5x = _MAX_ (VAR (-rax[1] - 6) ,VAR_ZERO) ;
			for (INDEX i = 0 ; i < r5x ; i++)
				rax[0] /= mAttr->varify_radix () ;
			INDEX ix = iw - 1 ;
			for (INDEX i = r5x ; i < r4x ; i++) {
				tmp[--iw] = mAttr->convert_number_w (rax[0] % mAttr->varify_radix ()) ;
				if (tmp[ix] == mAttr->convert_number_w (0))
					iw++ ;
				rax[0] /= mAttr->varify_radix () ;
			}
			for (INDEX i = _MAX_ (r5x ,r4x) ; i < VAR (-rax[1]) ; i++) {
				tmp[--iw] = mAttr->convert_number_w (0) ;
				if (tmp[ix] == mAttr->convert_number_w (0))
					iw++ ;
			}
			tmp[--iw] = UNIT ('.') ;
			if (tmp[ix] == UNIT ('.'))
				iw++ ;
			tmp[--iw] = mAttr->convert_number_w (0) ;
		} else {
			//@info:case '0'
			tmp[--iw] = mAttr->convert_number_w (0) ;
		}
		if (r2x)
			tmp[--iw] = UNIT ('-') ;
	}

	LENGTH log_of_number (VAR64 arg) const {
		LENGTH ret = 0 ;
		for (VAR64 i = 1 ; i <= arg ; i *= mAttr->varify_radix ())
			ret++ ;
		return std::move (ret) ;
	}
} ;

inline namespace STREAM {
template <class _ARG>
inline void _BOM_ (TextReader<_ARG> &reader) {}

inline void _BOM_ (TextReader<STRU8> &reader) {
	auto ris = reader.clone () ;
	auto rax = STRU8 () ;
	ris.read (rax) ;
	if (rax != STRU8 (0XEF))
		return ;
	ris.read (rax) ;
	if (rax != STRU8 (0XBB))
		return ;
	ris.read (rax) ;
	if (rax != STRU8 (0XBF))
		return ;
	ris.attr ().enable_endian (FALSE) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU16> &reader) {
	auto ris = reader.clone () ;
	auto rax = STRU16 () ;
	ris.read (rax) ;
	if (rax != STRU16 (0XFEFF) && rax != STRU16 (0XFFFE))
		return ;
	ris.attr ().enable_endian (rax != 0XFEFF) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRU32> &reader) {
	auto ris = reader.clone () ;
	auto rax = STRU32 () ;
	ris.read (rax) ;
	if (rax != STRU32 (0X0000FEFF) && rax != STRU32 (0XFFFE0000))
		return ;
	ris.attr ().enable_endian (rax != 0X0000FEFF) ;
	reader = std::move (ris) ;
}

inline void _BOM_ (TextReader<STRW> &reader) {
	_BOM_ (_CAST_<TextReader<STRUW>> (reader)) ;
}

template <class _ARG>
inline void _BOM_ (TextWriter<_ARG> &writer) {}

inline void _BOM_ (TextWriter<STRU8> &writer) {
	static constexpr ARR<STRU8> M_BOM = {STRU8 (0XEF) ,STRU8 (0XBB) ,STRU8 (0XBF)} ;
	writer << PhanBuffer<const STRU8>::make (M_BOM) ;
}

inline void _BOM_ (TextWriter<STRU16> &writer) {
	static constexpr ARR<STRU16> M_BOM = {STRU16 (0XFEFF)} ;
	writer << PhanBuffer<const STRU16>::make (M_BOM) ;
}

inline void _BOM_ (TextWriter<STRU32> &writer) {
	static constexpr ARR<STRU32> M_BOM = {STRU32 (0X0000FEFF)} ;
	writer << PhanBuffer<const STRU32>::make (M_BOM) ;
}

inline void _BOM_ (TextWriter<STRA> &writer) {}

inline void _BOM_ (TextWriter<STRW> &writer) {
	_BOM_ (_CAST_<TextWriter<STRUW>> (writer)) ;
}

template <class _ARG>
inline void _GAP_ (TextReader<_ARG> &reader) {
	auto ris = reader.clone () ;
	auto rax = _ARG () ;
	ris.read (rax) ;
	while (ris.attr ().varify_space (rax)) {
		reader = ris.clone () ;
		ris.read (rax) ;
	}
}

template <class _ARG>
inline void _EOS_ (TextReader<_ARG> &reader) {
	_DYNAMIC_ASSERT_ (reader.length () == 0) ;
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
public:
	inline StreamBinder () = default ;

	inline StreamBinder (const StreamBinder &) = delete ;
	inline StreamBinder &operator= (const StreamBinder &) = delete ;
	inline StreamBinder (StreamBinder &&) = delete ;
	inline StreamBinder &operator= (StreamBinder &&) = delete ;

	void friend_read (ByteReader &reader) const {}

	void friend_write (ByteWriter &writer) const {}

	template <class _ARG>
	void friend_read (TextReader<_ARG> &reader) const {}

	template <class _ARG>
	void friend_write (TextWriter<_ARG> &writer) const {}
} ;

template <class TYPE1 ,class... TYPES>
class StreamBinder<TYPE1 ,TYPES...> :private StreamBinder<TYPES...> {
private:
	TYPE1 & mData ;

public:
	inline StreamBinder () = delete ;

	inline explicit StreamBinder (TYPE1 &arg1 ,TYPES &...args) :StreamBinder<TYPES...> (args...) ,mData (arg1) {}

	void friend_read (ByteReader &reader) const {
		reader >> mData ;
		StreamBinder<TYPES...>::friend_read (reader) ;
	}

	void friend_write (ByteWriter &writer) const {
		writer << mData ;
		StreamBinder<TYPES...>::friend_write (writer) ;
	}

	template <class _ARG>
	void friend_read (TextReader<_ARG> &reader) const {
		reader >> mData ;
		StreamBinder<TYPES...>::friend_read (reader) ;
	}

	template <class _ARG>
	void friend_write (TextWriter<_ARG> &writer) const {
		writer << mData ;
		StreamBinder<TYPES...>::friend_write (writer) ;
	}
} ;
} ;

#ifndef __CSC_STRICTHPPINL__
#include "csc_stream.hpp.inl"
#endif