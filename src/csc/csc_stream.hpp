#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
struct StreamToolPureLayout ;

struct StreamToolLayout {
	Ref<StreamToolPureLayout> mThis ;
} ;

struct StreamToolHolder implement Interface {
	imports VFat<StreamToolHolder> create (VREF<StreamToolLayout> that) ;
	imports CFat<StreamToolHolder> create (CREF<StreamToolLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL big_endian () const = 0 ;
	virtual BOOL is_gap (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_gap_space (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_gap_endline (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_word (CREF<STRU32> str) const = 0 ;
	virtual STRU32 word_lower (CREF<STRU32> str) const = 0 ;
	virtual STRU32 word_upper (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_number (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_hex_number (CREF<STRU32> str) const = 0 ;
	virtual INDEX hex_from_str (CREF<STRU32> str) const = 0 ;
	virtual STRU32 str_from_hex (CREF<INDEX> hex) const = 0 ;
	virtual BOOL is_control (CREF<STRU32> str) const = 0 ;
} ;

class StreamTool implement StreamToolLayout {
protected:
	using StreamToolLayout::mThis ;

public:
	imports CREF<StreamTool> instance () {
		return memorize ([&] () {
			StreamTool ret ;
			StreamToolHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports BOOL big_endian () {
		return StreamToolHolder::create (instance ())->big_endian () ;
	}

	imports BOOL is_gap (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_gap (str) ;
	}

	imports BOOL is_gap_space (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_gap_space (str) ;
	}

	imports BOOL is_gap_endline (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_gap_endline (str) ;
	}

	imports BOOL is_word (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_word (str) ;
	}

	imports STRU32 word_lower (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->word_lower (str) ;
	}

	imports STRU32 word_upper (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->word_upper (str) ;
	}

	imports BOOL is_number (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_number (str) ;
	}

	imports BOOL is_hex_number (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_hex_number (str) ;
	}

	imports INDEX hex_from_str (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->hex_from_str (str) ;
	}

	imports STRU32 str_from_hex (CREF<INDEX> hex) {
		return StreamToolHolder::create (instance ())->str_from_hex (hex) ;
	}

	imports BOOL is_control (CREF<STRU32> str) {
		return StreamToolHolder::create (instance ())->is_control (str) ;
	}
} ;

static constexpr auto CLS = TYPE<PlaceHolder<RANK1>>::expr ;
static constexpr auto BOM = TYPE<PlaceHolder<RANK2>>::expr ;
static constexpr auto GAP = TYPE<PlaceHolder<RANK3>>::expr ;
static constexpr auto EOS = TYPE<PlaceHolder<RANK4>>::expr ;

struct ByteReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct ByteReaderHolder implement Interface {
	imports VFat<ByteReaderHolder> create (VREF<ByteReaderLayout> that) ;
	imports CFat<ByteReaderHolder> create (CREF<ByteReaderLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
	virtual void read (CREF<Slice<STRA>> item) = 0 ;
	virtual void read (CREF<Slice<STRW>> item) = 0 ;
	virtual void read (CREF<Slice<STRU8>> item) = 0 ;
	virtual void read (CREF<Slice<STRU16>> item) = 0 ;
	virtual void read (CREF<Slice<STRU32>> item) = 0 ;
	virtual void read (VREF<String> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;
} ;

class ByteReader implement ByteReaderLayout {
protected:
	using ByteReaderLayout::mStream ;
	using ByteReaderLayout::mRead ;
	using ByteReaderLayout::mWrite ;
	using ByteReaderLayout::mBackupRead ;
	using ByteReaderLayout::mBackupWrite ;

public:
	implicit ByteReader () = default ;

	explicit ByteReader (RREF<Ref<RefBuffer<BYTE>>> stream) {
		ByteReaderHolder::create (thiz)->initialize (move (stream)) ;
	}

	LENGTH size () const {
		return ByteReaderHolder::create (thiz)->size () ;
	}

	void reset () {
		return ByteReaderHolder::create (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return ByteReaderHolder::create (thiz)->reset (read_ ,write_) ;
	}

	void backup () {
		return ByteReaderHolder::create (thiz)->backup () ;
	}

	void recover () {
		return ByteReaderHolder::create (thiz)->recover () ;
	}

	void read (VREF<BOOL> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL32> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL64> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT32> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT64> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<BYTE> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<WORD> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<CHAR> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<QUAD> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<QUAD> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRA>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<Slice<STRA>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRW>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<Slice<STRW>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRU8>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<Slice<STRU8>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRU16>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<Slice<STRU16>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRU32>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<Slice<STRU32>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (CLS)> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (CLS)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (GAP)> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (EOS)> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct ByteWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct ByteWriterHolder implement Interface {
	imports VFat<ByteWriterHolder> create (VREF<ByteWriterLayout> that) ;
	imports CFat<ByteWriterHolder> create (CREF<ByteWriterLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<Slice<STRA>> item) = 0 ;
	virtual void write (CREF<Slice<STRW>> item) = 0 ;
	virtual void write (CREF<Slice<STRU8>> item) = 0 ;
	virtual void write (CREF<Slice<STRU16>> item) = 0 ;
	virtual void write (CREF<Slice<STRU32>> item) = 0 ;
	virtual void write (CREF<String> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
} ;

class ByteWriter implement ByteWriterLayout {
protected:
	using ByteWriterLayout::mStream ;
	using ByteWriterLayout::mRead ;
	using ByteWriterLayout::mWrite ;
	using ByteWriterLayout::mBackupRead ;
	using ByteWriterLayout::mBackupWrite ;

public:
	implicit ByteWriter () = default ;

	explicit ByteWriter (RREF<Ref<RefBuffer<BYTE>>> stream) {
		ByteWriterHolder::create (thiz)->initialize (move (stream)) ;
	}

	LENGTH size () const {
		return ByteWriterHolder::create (thiz)->size () ;
	}

	void reset () {
		return ByteWriterHolder::create (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return ByteWriterHolder::create (thiz)->reset (read_ ,write_) ;
	}

	void backup () {
		return ByteWriterHolder::create (thiz)->backup () ;
	}

	void recover () {
		return ByteWriterHolder::create (thiz)->recover () ;
	}

	void write (CREF<BOOL> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<BOOL> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL32> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<VAL32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL64> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<VAL64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT32> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<FLT32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT64> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<FLT64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<BYTE> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<BYTE> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<WORD> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<WORD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<CHAR> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<CHAR> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<QUAD> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<QUAD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRA>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice<STRA>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRW>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice<STRW>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRU8>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice<STRU8>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRU16>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice<STRU16>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRU32>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice<STRU32>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (CLS)> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (CLS)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (GAP)> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (EOS)> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct TextReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct TextReaderHolder implement Interface {
	imports VFat<TextReaderHolder> create (VREF<TextReaderLayout> that) ;
	imports CFat<TextReaderHolder> create (CREF<TextReaderLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
	virtual void read (CREF<Slice<STRA>> item) = 0 ;
	virtual void read (CREF<Slice<STRW>> item) = 0 ;
	virtual void read (CREF<Slice<STRU8>> item) = 0 ;
	virtual void read (CREF<Slice<STRU16>> item) = 0 ;
	virtual void read (CREF<Slice<STRU32>> item) = 0 ;
	virtual void read (VREF<String> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (BOM)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;
} ;

class TextReader implement TextReaderLayout {
protected:
	using TextReaderLayout::mStream ;
	using TextReaderLayout::mRead ;
	using TextReaderLayout::mWrite ;
	using TextReaderLayout::mBackupRead ;
	using TextReaderLayout::mBackupWrite ;

public:
	implicit TextReader () = default ;

	explicit TextReader (RREF<Ref<RefBuffer<BYTE>>> stream) {
		TextReaderHolder::create (thiz)->initialize (move (stream)) ;
	}

	LENGTH size () const {
		return TextReaderHolder::create (thiz)->size () ;
	}

	void reset () {
		return TextReaderHolder::create (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return TextReaderHolder::create (thiz)->reset (read_ ,write_) ;
	}

	void backup () {
		return TextReaderHolder::create (thiz)->backup () ;
	}

	void recover () {
		return TextReaderHolder::create (thiz)->recover () ;
	}

	void read (VREF<BOOL> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL32> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<VAL64> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT32> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<FLT64> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<BYTE> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<WORD> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<CHAR> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<QUAD> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<QUAD> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRA>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<Slice<STRA>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRW>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<Slice<STRW>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRU8>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<Slice<STRU8>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRU16>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<Slice<STRU16>> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<Slice<STRU32>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<Slice<STRU32>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (CLS)> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (CLS)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (BOM)> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (BOM)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (GAP)> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CREF<typeof (EOS)> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct TextWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct TextWriterHolder implement Interface {
	imports VFat<TextWriterHolder> create (VREF<TextWriterLayout> that) ;
	imports CFat<TextWriterHolder> create (CREF<TextWriterLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
	virtual void backup () = 0 ;
	virtual void recover () = 0 ;
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<Slice<STRA>> item) = 0 ;
	virtual void write (CREF<Slice<STRW>> item) = 0 ;
	virtual void write (CREF<Slice<STRU8>> item) = 0 ;
	virtual void write (CREF<Slice<STRU16>> item) = 0 ;
	virtual void write (CREF<Slice<STRU32>> item) = 0 ;
	virtual void write (CREF<String> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
} ;

class TextWriter implement TextWriterLayout {
protected:
	using TextWriterLayout::mStream ;
	using TextWriterLayout::mRead ;
	using TextWriterLayout::mWrite ;
	using TextWriterLayout::mBackupRead ;
	using TextWriterLayout::mBackupWrite ;

public:
	implicit TextWriter () = default ;

	explicit TextWriter (RREF<Ref<RefBuffer<BYTE>>> stream) {
		TextWriterHolder::create (thiz)->initialize (move (stream)) ;
	}

	LENGTH size () const {
		return TextWriterHolder::create (thiz)->size () ;
	}

	void reset () {
		return TextWriterHolder::create (thiz)->reset () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
		return TextWriterHolder::create (thiz)->reset (read_ ,write_) ;
	}

	void backup () {
		return TextWriterHolder::create (thiz)->backup () ;
	}

	void recover () {
		return TextWriterHolder::create (thiz)->recover () ;
	}

	void write (CREF<BOOL> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<BOOL> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL32> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<VAL32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<VAL64> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<VAL64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT32> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<FLT32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<FLT64> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<FLT64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<BYTE> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<BYTE> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<WORD> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<WORD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<CHAR> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<CHAR> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<QUAD> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<QUAD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRA>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice<STRA>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRW>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice<STRW>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRU8>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice<STRU8>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRU16>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice<STRU16>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<Slice<STRU32>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice<STRU32>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (CLS)> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (CLS)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (BOM)> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (BOM)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (GAP)> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<typeof (EOS)> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct StringToolPureLayout ;

struct StringToolLayout {
	Ref<StringToolPureLayout> mThis ;
} ;

struct StringToolHolder implement Interface {
	imports VFat<StringToolHolder> create (VREF<StringToolLayout> that) ;
	imports CFat<StringToolHolder> create (CREF<StringToolLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL parse_bool (CREF<String> a) const = 0 ;
	virtual String build_bool (CREF<BOOL> a) const = 0 ;
	virtual VAL32 parse_val32 (CREF<String> a) const = 0 ;
	virtual String build_val32 (CREF<VAL32> a) const = 0 ;
	virtual VAL64 parse_val64 (CREF<String> a) const = 0 ;
	virtual String build_val64 (CREF<VAL64> a) const = 0 ;
	virtual FLT32 parse_flt32 (CREF<String> a) const = 0 ;
	virtual String build_flt32 (CREF<FLT32> a) const = 0 ;
	virtual FLT64 parse_flt64 (CREF<String> a) const = 0 ;
	virtual String build_flt64 (CREF<FLT64> a) const = 0 ;
	virtual String stra_from_strw (CREF<String> a) const = 0 ;
	virtual String strw_from_stra (CREF<String> a) const = 0 ;
	virtual String stru8_from_stru16 (CREF<String> a) const = 0 ;
	virtual String stru8_from_stru32 (CREF<String> a) const = 0 ;
	virtual String stru16_from_stru8 (CREF<String> a) const = 0 ;
	virtual String stru16_from_stru32 (CREF<String> a) const = 0 ;
	virtual String stru32_from_stru8 (CREF<String> a) const = 0 ;
	virtual String stru32_from_stru16 (CREF<String> a) const = 0 ;
	virtual String cvt_stra (CREF<String> a) const = 0 ;
	virtual String cvt_strw (CREF<String> a) const = 0 ;
	virtual String cvt_strs (CREF<String> a) const = 0 ;
} ;

class StringTool implement StringToolLayout {
protected:
	using StringToolLayout::mThis ;

public:
	imports CREF<StringTool> instance () {
		return memorize ([&] () {
			StringTool ret ;
			StringToolHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports BOOL parse_bool (CREF<String> a) {
		return StringToolHolder::create (instance ())->parse_bool (a) ;
	}

	imports String build_bool (CREF<BOOL> a) {
		return StringToolHolder::create (instance ())->build_bool (a) ;
	}

	imports VAL32 parse_val32 (CREF<String> a) {
		return StringToolHolder::create (instance ())->parse_val32 (a) ;
	}

	imports String build_val32 (CREF<VAL32> a) {
		return StringToolHolder::create (instance ())->build_val32 (a) ;
	}

	imports VAL64 parse_val64 (CREF<String> a) {
		return StringToolHolder::create (instance ())->parse_val64 (a) ;
	}

	imports String build_val64 (CREF<VAL64> a) {
		return StringToolHolder::create (instance ())->build_val64 (a) ;
	}

	imports FLT32 parse_flt32 (CREF<String> a) {
		return StringToolHolder::create (instance ())->parse_flt32 (a) ;
	}

	imports String build_flt32 (CREF<FLT32> a) {
		return StringToolHolder::create (instance ())->build_flt32 (a) ;
	}

	imports FLT64 parse_flt64 (CREF<String> a) {
		return StringToolHolder::create (instance ())->parse_flt64 (a) ;
	}

	imports String build_flt64 (CREF<FLT64> a) {
		return StringToolHolder::create (instance ())->build_flt64 (a) ;
	}

	imports String stra_from_strw (CREF<String> a) {
		return StringToolHolder::create (instance ())->stra_from_strw (a) ;
	}

	imports String strw_from_stra (CREF<String> a) {
		return StringToolHolder::create (instance ())->strw_from_stra (a) ;
	}

	imports String stru8_from_stru16 (CREF<String> a) {
		return StringToolHolder::create (instance ())->stru8_from_stru16 (a) ;
	}

	imports String stru8_from_stru32 (CREF<String> a) {
		return StringToolHolder::create (instance ())->stru8_from_stru32 (a) ;
	}

	imports String stru16_from_stru8 (CREF<String> a) {
		return StringToolHolder::create (instance ())->stru16_from_stru8 (a) ;
	}

	imports String stru16_from_stru32 (CREF<String> a) {
		return StringToolHolder::create (instance ())->stru16_from_stru32 (a) ;
	}

	imports String stru32_from_stru8 (CREF<String> a) {
		return StringToolHolder::create (instance ())->stru32_from_stru8 (a) ;
	}

	imports String stru32_from_stru16 (CREF<String> a) {
		return StringToolHolder::create (instance ())->stru32_from_stru16 (a) ;
	}

	imports String cvt_stra (CREF<String> a) {
		return StringToolHolder::create (instance ())->cvt_stra (a) ;
	}

	imports String cvt_strw (CREF<String> a) {
		return StringToolHolder::create (instance ())->cvt_strw (a) ;
	}

	imports String cvt_strs (CREF<String> a) {
		return StringToolHolder::create (instance ())->cvt_strs (a) ;
	}
} ;

struct RegexPureLayout ;

struct RegexLayout {
	Ref<RegexPureLayout> mThis ;
} ;

struct RegexHolder implement Interface {
	imports VFat<RegexHolder> create (VREF<RegexLayout> that) ;
	imports CFat<RegexHolder> create (CREF<RegexLayout> that) ;

	virtual void initialize (CREF<String> pattern) = 0 ;
	virtual INDEX search (CREF<String> text ,CREF<INDEX> index) = 0 ;
	virtual String match (CREF<INDEX> index) const = 0 ;
} ;

class Regex implement RegexLayout {
protected:
	using RegexLayout::mThis ;

public:
	implicit Regex () = default ;

	explicit Regex (CREF<String> pattern) {
		RegexHolder::create (thiz)->initialize (pattern) ;
	}

	INDEX search (CREF<String> text ,CREF<INDEX> index) {
		return RegexHolder::create (thiz)->search (text ,index) ;
	}

	String match (CREF<INDEX> index) const {
		return RegexHolder::create (thiz)->match (index) ;
	}
} ;

struct XmlParserPureLayout ;

struct XmlParserLayout {
	Ref<XmlParserPureLayout> mThis ;
	INDEX mIndex ;

public:
	implicit XmlParserLayout () noexcept {
		mIndex = NONE ;
	}
} ;

struct XmlParserHolder implement Interface {
	imports VFat<XmlParserHolder> create (VREF<XmlParserLayout> that) ;
	imports CFat<XmlParserHolder> create (CREF<XmlParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<STRU8>> stream) = 0 ;
	virtual void initialize (CREF<XmlParserLayout> that) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual XmlParserLayout root () const = 0 ;
	virtual XmlParserLayout parent () const = 0 ;
	virtual XmlParserLayout brother () const = 0 ;
	virtual XmlParserLayout child () const = 0 ;
	virtual XmlParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual XmlParserLayout child (CREF<String> name) const = 0 ;
	virtual Array<XmlParserLayout> list () const = 0 ;
	virtual Array<XmlParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<XmlParserLayout> that) const = 0 ;
	virtual CREF<String> name () const leftvalue = 0 ;
	virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
	virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
	virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
	virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
	virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
	virtual String fetch (CREF<String> def) const = 0 ;
	virtual Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String> fetch (CREF<String> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class XmlParser implement XmlParserLayout {
protected:
	using XmlParserLayout::mThis ;
	using XmlParserLayout::mIndex ;

public:
	implicit XmlParser () = default ;

	explicit XmlParser (CREF<RefBuffer<STRU8>> stream) {
		XmlParserHolder::create (thiz)->initialize (stream) ;
	}

	implicit XmlParser (CREF<XmlParser> that) {
		XmlParserHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<XmlParser> operator= (CREF<XmlParser> that) {
		return assign (thiz ,that) ;
	}

	implicit XmlParser (RREF<XmlParser> that) = default ;

	forceinline VREF<XmlParser> operator= (RREF<XmlParser> that) = default ;

	BOOL exist () const {
		return XmlParserHolder::create (thiz)->exist () ;
	}

	XmlParser root () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->root () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser parent () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->parent () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser brother () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->brother () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child () const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child () ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<INDEX> index) const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child (index) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<String> name) const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	Array<XmlParser> list () const {
		Array<XmlParserLayout> ret = XmlParserHolder::create (thiz)->list () ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	Array<XmlParser> list (CREF<LENGTH> size_) const {
		Array<XmlParserLayout> ret = XmlParserHolder::create (thiz)->list (size_) ;
		return move (keep[TYPE<Array<XmlParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	BOOL equal (CREF<XmlParser> that) const {
		return XmlParserHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<XmlParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<XmlParser> that) const {
		return ifnot (equal (that)) ;
	}

	CREF<String> name () const leftvalue {
		return XmlParserHolder::create (thiz)->name () ;
	}

	BOOL fetch (CREF<BOOL> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	VAL32 fetch (CREF<VAL32> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	VAL64 fetch (CREF<VAL64> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	FLT32 fetch (CREF<FLT32> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	FLT64 fetch (CREF<FLT64> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String fetch (CREF<String> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String> fetch (CREF<String> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}
} ;

struct JsonParserPureLayout ;

struct JsonParserLayout {
	Ref<JsonParserPureLayout> mThis ;
	INDEX mIndex ;

public:
	implicit JsonParserLayout () noexcept {
		mIndex = NONE ;
	}
} ;

struct JsonParserHolder implement Interface {
	imports VFat<JsonParserHolder> create (VREF<JsonParserLayout> that) ;
	imports CFat<JsonParserHolder> create (CREF<JsonParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<STRU8>> stream) = 0 ;
	virtual void initialize (CREF<JsonParserLayout> that) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual JsonParserLayout root () const = 0 ;
	virtual JsonParserLayout parent () const = 0 ;
	virtual JsonParserLayout brother () const = 0 ;
	virtual JsonParserLayout child () const = 0 ;
	virtual JsonParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual JsonParserLayout child (CREF<String> name) const = 0 ;
	virtual Array<JsonParserLayout> list () const = 0 ;
	virtual Array<JsonParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<JsonParserLayout> that) const = 0 ;
	virtual CREF<String> name () const leftvalue = 0 ;
	virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
	virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
	virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
	virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
	virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
	virtual String fetch (CREF<String> def) const = 0 ;
	virtual Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String> fetch (CREF<String> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class JsonParser implement JsonParserLayout {
protected:
	using JsonParserLayout::mThis ;
	using JsonParserLayout::mIndex ;

public:
	implicit JsonParser () = default ;

	explicit JsonParser (CREF<RefBuffer<STRU8>> stream) {
		JsonParserHolder::create (thiz)->initialize (stream) ;
	}

	implicit JsonParser (CREF<JsonParser> that) {
		JsonParserHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<JsonParser> operator= (CREF<JsonParser> that) {
		return assign (thiz ,that) ;
	}

	implicit JsonParser (RREF<JsonParser> that) = default ;

	forceinline VREF<JsonParser> operator= (RREF<JsonParser> that) = default ;

	BOOL exist () const {
		return JsonParserHolder::create (thiz)->exist () ;
	}

	JsonParser root () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->root () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser parent () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->parent () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser brother () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->brother () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child () const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child () ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<INDEX> index) const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child (index) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<String> name) const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	Array<JsonParser> list () const {
		Array<JsonParserLayout> ret = JsonParserHolder::create (thiz)->list () ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	Array<JsonParser> list (CREF<LENGTH> size_) const {
		Array<JsonParserLayout> ret = JsonParserHolder::create (thiz)->list (size_) ;
		return move (keep[TYPE<Array<JsonParser>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	BOOL equal (CREF<JsonParser> that) const {
		return JsonParserHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<JsonParser> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<JsonParser> that) const {
		return ifnot (equal (that)) ;
	}

	CREF<String> name () const leftvalue {
		return JsonParserHolder::create (thiz)->name () ;
	}

	BOOL fetch (CREF<BOOL> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	VAL32 fetch (CREF<VAL32> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	VAL64 fetch (CREF<VAL64> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	FLT32 fetch (CREF<FLT32> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	FLT64 fetch (CREF<FLT64> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String fetch (CREF<String> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String> fetch (CREF<String> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}
} ;

struct PlyParserGuide {
	INDEX mElement ;
	ArrayList<INDEX> mProperty ;
	INDEX mPropertyIndex ;
	INDEX mLineIndex ;
	INDEX mPlyIndex ;
	FLAG mPlyType ;
} ;

struct PlyParserPureLayout ;

struct PlyParserLayout {
	Ref<PlyParserPureLayout> mThis ;
	PlyParserGuide mGuide ;
} ;

struct PlyParserHolder implement Interface {
	imports VFat<PlyParserHolder> create (VREF<PlyParserLayout> that) ;
	imports CFat<PlyParserHolder> create (CREF<PlyParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<STRU8>> stream) = 0 ;
	virtual INDEX find_element (CREF<String> name) const = 0 ;
	virtual LENGTH element_size (CREF<INDEX> element) const = 0 ;
	virtual INDEX find_property (CREF<INDEX> element ,CREF<String> name) const = 0 ;
	virtual LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const = 0 ;
	virtual void guide_new (CREF<INDEX> element) = 0 ;
	virtual void guide_put (CREF<INDEX> property) = 0 ;
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
} ;

class PlyParser implement PlyParserLayout {
protected:
	using PlyParserLayout::mThis ;

public:
	implicit PlyParser () = default ;

	explicit PlyParser (CREF<RefBuffer<STRU8>> stream) {
		PlyParserHolder::create (thiz)->initialize (stream) ;
	}

	INDEX find_element (CREF<String> name) const {
		return PlyParserHolder::create (thiz)->find_element (name) ;
	}

	LENGTH element_size (CREF<INDEX> element) const {
		return PlyParserHolder::create (thiz)->element_size (element) ;
	}

	INDEX find_property (CREF<INDEX> element ,CREF<String> name) const {
		return PlyParserHolder::create (thiz)->find_property (element ,name) ;
	}

	LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const {
		return PlyParserHolder::create (thiz)->property_size (element ,line ,property) ;
	}

	void guide_new (CREF<INDEX> element) {
		return PlyParserHolder::create (thiz)->guide_new (element) ;
	}

	void guide_put (CREF<INDEX> property) {
		return PlyParserHolder::create (thiz)->guide_put (property) ;
	}

	void read (VREF<BOOL> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<VAL32> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<VAL64> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<FLT32> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<FLT64> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<BYTE> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<WORD> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<CHAR> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}

	void read (VREF<QUAD> item) {
		return PlyParserHolder::create (thiz)->read (item) ;
	}
} ;
} ;