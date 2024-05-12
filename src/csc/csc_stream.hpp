#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"

namespace CSC {
struct StreamProcPureLayout ;

struct StreamProcLayout {
	Ref<StreamProcPureLayout> mThis ;
} ;

struct StreamProcHolder implement Interface {
	imports VFat<StreamProcHolder> create (VREF<StreamProcLayout> that) ;
	imports CFat<StreamProcHolder> create (CREF<StreamProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL big_endian () const = 0 ;
	virtual BOOL is_blank (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_space (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_endline (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_alpha (CREF<STRU32> str) const = 0 ;
	virtual STRU32 alpha_lower (CREF<STRU32> str) const = 0 ;
	virtual STRU32 alpha_upper (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_digit (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_hex_digit (CREF<STRU32> str) const = 0 ;
	virtual INDEX hex_from_str (CREF<STRU32> str) const = 0 ;
	virtual STRU32 str_from_hex (CREF<INDEX> hex) const = 0 ;
	virtual BOOL is_word (CREF<STRU32> str) const = 0 ;
	virtual BOOL is_control (CREF<STRU32> str) const = 0 ;
	virtual STRU32 word_from_ctrl (CREF<STRU32> str) const = 0 ;
	virtual STRU32 ctrl_from_word (CREF<STRU32> str) const = 0 ;
} ;

class StreamProc implement StreamProcLayout {
protected:
	using StreamProcLayout::mThis ;

public:
	imports CREF<StreamProc> instance () {
		return memorize ([&] () {
			StreamProc ret ;
			StreamProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports BOOL big_endian () {
		return StreamProcHolder::create (instance ())->big_endian () ;
	}

	imports BOOL is_blank (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_blank (str) ;
	}

	imports BOOL is_space (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_space (str) ;
	}

	imports BOOL is_endline (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_endline (str) ;
	}

	imports BOOL is_alpha (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_alpha (str) ;
	}

	imports STRU32 alpha_lower (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->alpha_lower (str) ;
	}

	imports STRU32 alpha_upper (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->alpha_upper (str) ;
	}

	imports BOOL is_digit (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_digit (str) ;
	}

	imports BOOL is_hex_digit (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_hex_digit (str) ;
	}

	imports INDEX hex_from_str (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->hex_from_str (str) ;
	}

	imports STRU32 str_from_hex (CREF<INDEX> hex) {
		return StreamProcHolder::create (instance ())->str_from_hex (hex) ;
	}

	imports BOOL is_word (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_control (str) ;
	}

	imports BOOL is_control (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_control (str) ;
	}

	imports STRU32 word_from_ctrl (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->word_from_ctrl (str) ;
	}

	imports STRU32 ctrl_from_word (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->ctrl_from_word (str) ;
	}
} ;

static constexpr auto CLS = TYPE<PlaceHolder<RANK1>>::expr ;
static constexpr auto BOM = TYPE<PlaceHolder<RANK2>>::expr ;
static constexpr auto GAP = TYPE<PlaceHolder<RANK3>>::expr ;
static constexpr auto EOS = TYPE<PlaceHolder<RANK4>>::expr ;

class ByteReader ;

struct ByteReaderFriend implement Interface {
	virtual void friend_read (VREF<ByteReader> reader) = 0 ;
} ;

template <class A>
class ByteReaderFriendBinder implement Fat<ByteReaderFriend ,A> {
public:
	void friend_read (VREF<ByteReader> reader) override {
		return fake.friend_read (reader) ;
	}
} ;

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
	virtual LENGTH length () const = 0 ;
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
	virtual void read (CREF<Slice> item) = 0 ;
	virtual void read (VREF<String<STRA>> item) = 0 ;
	virtual void read (VREF<String<STRW>> item) = 0 ;
	virtual void read (VREF<String<STRU8>> item) = 0 ;
	virtual void read (VREF<String<STRU16>> item) = 0 ;
	virtual void read (VREF<String<STRU32>> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;
	virtual void read (CREF<VFat<ByteReaderFriend>> item) = 0 ;
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

	LENGTH length () const {
		return ByteReaderHolder::create (thiz)->length () ;
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

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
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

	void read (CREF<Slice> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRA>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRA>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRW>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRW>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU8>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRU8>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU16>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRU16>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU32>> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<String<STRU32>> item) {
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

	template <class ARG1>
	void read (VREF<ARG1> item) {
		const auto r1x = VFat<ByteReaderFriend> (ByteReaderFriendBinder<ARG1> () ,item) ;
		return ByteReaderHolder::create (thiz)->read (r1x) ;
	}

	template <class ARG1>
	forceinline VREF<ByteReader> operator>> (VREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

class ByteWriter ;

struct ByteWriterFriend implement Interface {
	virtual void friend_write (VREF<ByteWriter> reader) const = 0 ;
} ;

template <class A>
class ByteWriterFriendBinder implement Fat<ByteWriterFriend ,A> {
public:
	void friend_write (VREF<ByteWriter> reader) const override {
		return fake.friend_write (reader) ;
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
	virtual LENGTH length () const = 0 ;
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
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
	virtual void write (CREF<CFat<ByteWriterFriend>> item) = 0 ;
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

	LENGTH length () const {
		return ByteWriterHolder::create (thiz)->length () ;
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

	void write (CREF<csc_const_pointer_t> item) = delete ;

	forceinline VREF<ByteWriter> operator<< (CREF<csc_const_pointer_t> item) = delete ;

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

	void write (CREF<Slice> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRA>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRA>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRW>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRW>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU8>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRU8>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU16>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRU16>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU32>> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<String<STRU32>> item) {
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

	template <class ARG1>
	void write (CREF<ARG1> item) {
		const auto r1x = CFat<ByteWriterFriend> (ByteWriterFriendBinder<ARG1> () ,item) ;
		return ByteWriterHolder::create (thiz)->write (r1x) ;
	}

	template <class ARG1>
	forceinline VREF<ByteWriter> operator<< (CREF<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

class TextReader ;

struct TextReaderFriend implement Interface {
	virtual void friend_read (VREF<TextReader> reader) = 0 ;
} ;

template <class A>
class TextReaderFriendBinder implement Fat<TextReaderFriend ,A> {
public:
	void friend_read (VREF<TextReader> reader) override {
		return fake.friend_read (reader) ;
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
	virtual LENGTH length () const = 0 ;
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
	virtual void read (CREF<Slice> item) = 0 ;
	virtual void read (VREF<String<STRA>> item) = 0 ;
	virtual void read (VREF<String<STRW>> item) = 0 ;
	virtual void read (VREF<String<STRU8>> item) = 0 ;
	virtual void read (VREF<String<STRU16>> item) = 0 ;
	virtual void read (VREF<String<STRU32>> item) = 0 ;
	virtual void read (CREF<typeof (CLS)> item) = 0 ;
	virtual void read (CREF<typeof (BOM)> item) = 0 ;
	virtual void read (CREF<typeof (GAP)> item) = 0 ;
	virtual void read (CREF<typeof (EOS)> item) = 0 ;
	virtual void read (CREF<VFat<TextReaderFriend>> item) = 0 ;
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

	LENGTH length () const {
		return TextReaderHolder::create (thiz)->length () ;
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

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
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

	void read (CREF<Slice> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (CREF<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRA>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRA>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRW>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRW>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU8>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRU8>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU16>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRU16>> item) {
		read (item) ;
		return thiz ;
	}

	void read (VREF<String<STRU32>> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<String<STRU32>> item) {
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

	template <class ARG1>
	void read (VREF<ARG1> item) {
		const auto r1x = VFat<TextReaderFriend> (TextReaderFriendBinder<ARG1> () ,item) ;
		return TextReaderHolder::create (thiz)->read (r1x) ;
	}

	template <class ARG1>
	forceinline VREF<TextReader> operator>> (VREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

class TextWriter ;

struct TextWriterFriend implement Interface {
	virtual void friend_write (VREF<TextWriter> reader) const = 0 ;
} ;

template <class A>
class TextWriterFriendBinder implement Fat<TextWriterFriend ,A> {
public:
	void friend_write (VREF<TextWriter> reader) const override {
		return fake.friend_write (reader) ;
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
	virtual LENGTH length () const = 0 ;
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
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
	virtual void write (CREF<CFat<TextWriterFriend>> item) = 0 ;
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

	LENGTH length () const {
		return TextWriterHolder::create (thiz)->length () ;
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

	void write (CREF<csc_const_pointer_t> item) = delete ;

	forceinline VREF<TextWriter> operator<< (CREF<csc_const_pointer_t> item) = delete ;

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

	void write (CREF<Slice> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRA>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRA>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRW>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRW>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU8>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRU8>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU16>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRU16>> item) {
		write (item) ;
		return thiz ;
	}

	void write (CREF<String<STRU32>> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<String<STRU32>> item) {
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

	template <class ARG1>
	void write (CREF<ARG1> item) {
		const auto r1x = CFat<TextWriterFriend> (TextWriterFriendBinder<ARG1> () ,item) ;
		return TextWriterHolder::create (thiz)->write (r1x) ;
	}

	template <class ARG1>
	forceinline VREF<TextWriter> operator<< (CREF<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

template <class A>
template <class...ARG1>
inline String<A> String<A>::make (ARG1 &&...initval) {
	String<A> ret = String<A> (4096) ;
	auto rax = TextWriter (ret.borrow ()) ;
	const auto r1x = csc_initializer_list_t<FLAG> ({(rax << initval ,ZERO)...}) ;
	noop (r1x) ;
	rax << EOS ;
	return move (ret) ;
}

struct StringProcPureLayout ;

struct StringProcLayout {
	Ref<StringProcPureLayout> mThis ;
} ;

struct StringProcHolder implement Interface {
	imports VFat<StringProcHolder> create (VREF<StringProcLayout> that) ;
	imports CFat<StringProcHolder> create (CREF<StringProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual String<STRA> stra_from_strw (CREF<String<STRW>> a) const = 0 ;
	virtual String<STRA> stra_from_strs (CREF<String<STR>> a) const = 0 ;
	virtual String<STRW> strw_from_stra (CREF<String<STRA>> a) const = 0 ;
	virtual String<STRW> strw_from_strs (CREF<String<STR>> a) const = 0 ;
	virtual String<STR> strs_from_stra (CREF<String<STRA>> a) const = 0 ;
	virtual String<STR> strs_from_strw (CREF<String<STRW>> a) const = 0 ;
	virtual String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRUA> strua_from_stra (RREF<String<STRA>> a) const = 0 ;
	virtual String<STRA> stra_from_strua (RREF<String<STRUA>> a) const = 0 ;
	virtual String<STRUW> struw_from_strw (RREF<String<STRW>> a) const = 0 ;
	virtual String<STRW> strw_from_struw (RREF<String<STRUW>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRA> stra_from_stru (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STRW> strw_from_stru (CREF<String<STRU32>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CREF<String<STRU8>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CREF<String<STRU16>> a) const = 0 ;
	virtual String<STR> strs_from_stru (CREF<String<STRU32>> a) const = 0 ;
} ;

class StringProc implement StringProcLayout {
protected:
	using StringProcLayout::mThis ;

public:
	imports CREF<StringProc> instance () {
		return memorize ([&] () {
			StringProc ret ;
			StringProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports String<STRA> stra_from_strw (CREF<String<STRW>> a) {
		return StringProcHolder::create (instance ())->stra_from_strw (a) ;
	}

	imports String<STRA> stra_from_strs (CREF<String<STR>> a) {
		return StringProcHolder::create (instance ())->stra_from_strs (a) ;
	}

	imports String<STRW> strw_from_stra (CREF<String<STRA>> a) {
		return StringProcHolder::create (instance ())->strw_from_stra (a) ;
	}

	imports String<STRW> strw_from_strs (CREF<String<STR>> a) {
		return StringProcHolder::create (instance ())->strw_from_strs (a) ;
	}

	imports String<STR> strs_from_stra (CREF<String<STRA>> a) {
		return StringProcHolder::create (instance ())->strs_from_stra (a) ;
	}

	imports String<STR> strs_from_strw (CREF<String<STRW>> a) {
		return StringProcHolder::create (instance ())->strs_from_strw (a) ;
	}

	imports String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) {
		return StringProcHolder::create (instance ())->stru8_from_stru16 (a) ;
	}

	imports String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) {
		return StringProcHolder::create (instance ())->stru8_from_stru32 (a) ;
	}

	imports String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) {
		return StringProcHolder::create (instance ())->stru16_from_stru8 (a) ;
	}

	imports String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) {
		return StringProcHolder::create (instance ())->stru16_from_stru32 (a) ;
	}

	imports String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) {
		return StringProcHolder::create (instance ())->stru32_from_stru8 (a) ;
	}

	imports String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) {
		return StringProcHolder::create (instance ())->stru32_from_stru16 (a) ;
	}

	imports String<STRUA> strua_from_stra (RREF<String<STRA>> a) {
		return StringProcHolder::create (instance ())->strua_from_stra (move (a)) ;
	}

	imports String<STRA> stra_from_strua (RREF<String<STRUA>> a) {
		return StringProcHolder::create (instance ())->stra_from_strua (move (a)) ;
	}

	imports String<STRUW> struw_from_strw (RREF<String<STRW>> a) {
		return StringProcHolder::create (instance ())->struw_from_strw (move (a)) ;
	}

	imports String<STRW> strw_from_struw (RREF<String<STRUW>> a) {
		return StringProcHolder::create (instance ())->strw_from_struw (move (a)) ;
	}

	imports String<STRA> stra_from_stru (CREF<String<STRU8>> a) {
		return StringProcHolder::create (instance ())->stra_from_stru (a) ;
	}

	imports String<STRA> stra_from_stru (CREF<String<STRU16>> a) {
		return StringProcHolder::create (instance ())->stra_from_stru (a) ;
	}

	imports String<STRA> stra_from_stru (CREF<String<STRU32>> a) {
		return StringProcHolder::create (instance ())->stra_from_stru (a) ;
	}

	imports String<STRW> strw_from_stru (CREF<String<STRU8>> a) {
		return StringProcHolder::create (instance ())->strw_from_stru (a) ;
	}

	imports String<STRW> strw_from_stru (CREF<String<STRU16>> a) {
		return StringProcHolder::create (instance ())->strw_from_stru (a) ;
	}

	imports String<STRW> strw_from_stru (CREF<String<STRU32>> a) {
		return StringProcHolder::create (instance ())->strw_from_stru (a) ;
	}

	imports String<STR> strs_from_stru (CREF<String<STRU8>> a) {
		return StringProcHolder::create (instance ())->strs_from_stru (a) ;
	}

	imports String<STR> strs_from_stru (CREF<String<STRU16>> a) {
		return StringProcHolder::create (instance ())->strs_from_stru (a) ;
	}

	imports String<STR> strs_from_stru (CREF<String<STRU32>> a) {
		return StringProcHolder::create (instance ())->strs_from_stru (a) ;
	}
} ;

class AlignText {
protected:
	VAL64 mValue ;
	VAL64 mAlign ;

public:
	implicit AlignText () = delete ;

	explicit AlignText (CREF<VAL64> value ,CREF<VAL64> align) {
		mValue = value ;
		mAlign = align ;
	}

	void friend_write (VREF<TextWriter> reader) const {
		assume (mValue >= 0) ;
		assume (mAlign >= 1) ;
		auto rcx = 1 ;
		for (auto &&i : iter (0 ,mAlign - 1)) {
			noop (i) ;
			rcx *= 10 ;
			if (mValue >= rcx)
				continue ;
			reader << slice ("0") ;
		}
		const auto r1x = mValue % rcx ;
		reader << r1x ;
	}
} ;

struct RegexPureLayout ;

struct RegexLayout {
	Ref<RegexPureLayout> mThis ;
} ;

struct RegexHolder implement Interface {
	imports VFat<RegexHolder> create (VREF<RegexLayout> that) ;
	imports CFat<RegexHolder> create (CREF<RegexLayout> that) ;

	virtual void initialize (CREF<String<STRA>> pattern) = 0 ;
	virtual INDEX search (CREF<String<STRA>> text ,CREF<INDEX> index) = 0 ;
	virtual String<STRA> match (CREF<INDEX> index) const = 0 ;
} ;

class Regex implement RegexLayout {
protected:
	using RegexLayout::mThis ;

public:
	implicit Regex () = default ;

	explicit Regex (CREF<String<STRA>> pattern) {
		RegexHolder::create (thiz)->initialize (pattern) ;
	}

	INDEX search (CREF<String<STRA>> text ,CREF<INDEX> index) {
		return RegexHolder::create (thiz)->search (text ,index) ;
	}

	String<STRA> match (CREF<INDEX> index) const {
		return RegexHolder::create (thiz)->match (index) ;
	}
} ;

struct XmlParserPureLayout ;

struct XmlParserLayout {
	Ref<XmlParserPureLayout> mThis ;
	INDEX mIndex ;
} ;

struct XmlParserHolder implement Interface {
	imports VFat<XmlParserHolder> create (VREF<XmlParserLayout> that) ;
	imports CFat<XmlParserHolder> create (CREF<XmlParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual void initialize (CREF<XmlParserLayout> that) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual XmlParserLayout root () const = 0 ;
	virtual XmlParserLayout parent () const = 0 ;
	virtual XmlParserLayout brother () const = 0 ;
	virtual XmlParserLayout child () const = 0 ;
	virtual XmlParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual XmlParserLayout child (CREF<Slice> name) const = 0 ;
	virtual XmlParserLayout child (CREF<String<STRU8>> name) const = 0 ;
	virtual Array<XmlParserLayout> list () const = 0 ;
	virtual Array<XmlParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<XmlParserLayout> that) const = 0 ;
	virtual CREF<String<STRU8>> name () const leftvalue = 0 ;
	virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
	virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
	virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
	virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
	virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
	virtual String<STRA> fetch (CREF<String<STRA>> def) const = 0 ;
	virtual String<STRW> fetch (CREF<String<STRW>> def) const = 0 ;
	virtual String<STRU8> fetch (CREF<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> fetch (CREF<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> fetch (CREF<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class XmlParser implement XmlParserLayout {
protected:
	using XmlParserLayout::mThis ;
	using XmlParserLayout::mIndex ;

public:
	implicit XmlParser () = default ;

	explicit XmlParser (CREF<RefBuffer<BYTE>> stream) {
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

	XmlParser child (CREF<Slice> name) const {
		XmlParserLayout ret = XmlParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<XmlParser>::expr] (ret)) ;
	}

	XmlParser child (CREF<String<STRU8>> name) const {
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

	CREF<String<STRU8>> name () const leftvalue {
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

	String<STRA> fetch (CREF<String<STRA>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const {
		return XmlParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const {
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

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const {
		return XmlParserHolder::create (thiz)->fetch (def ,size_) ;
	}
} ;

struct JsonParserPureLayout ;

struct JsonParserLayout {
	Ref<JsonParserPureLayout> mThis ;
	INDEX mIndex ;
} ;

struct JsonParserHolder implement Interface {
	imports VFat<JsonParserHolder> create (VREF<JsonParserLayout> that) ;
	imports CFat<JsonParserHolder> create (CREF<JsonParserLayout> that) ;

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual void initialize (CREF<JsonParserLayout> that) = 0 ;
	virtual BOOL exist () const = 0 ;
	virtual JsonParserLayout root () const = 0 ;
	virtual JsonParserLayout parent () const = 0 ;
	virtual JsonParserLayout brother () const = 0 ;
	virtual JsonParserLayout child () const = 0 ;
	virtual JsonParserLayout child (CREF<INDEX> index) const = 0 ;
	virtual JsonParserLayout child (CREF<Slice> name) const = 0 ;
	virtual JsonParserLayout child (CREF<String<STRU8>> name) const = 0 ;
	virtual Array<JsonParserLayout> list () const = 0 ;
	virtual Array<JsonParserLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<JsonParserLayout> that) const = 0 ;
	virtual CREF<String<STRU8>> name () const leftvalue = 0 ;
	virtual BOOL fetch (CREF<BOOL> def) const = 0 ;
	virtual VAL32 fetch (CREF<VAL32> def) const = 0 ;
	virtual VAL64 fetch (CREF<VAL64> def) const = 0 ;
	virtual FLT32 fetch (CREF<FLT32> def) const = 0 ;
	virtual FLT64 fetch (CREF<FLT64> def) const = 0 ;
	virtual String<STRA> fetch (CREF<String<STRA>> def) const = 0 ;
	virtual String<STRW> fetch (CREF<String<STRW>> def) const = 0 ;
	virtual String<STRU8> fetch (CREF<String<STRU8>> def) const = 0 ;
	virtual String<STRU16> fetch (CREF<String<STRU16>> def) const = 0 ;
	virtual String<STRU32> fetch (CREF<String<STRU32>> def) const = 0 ;
	virtual Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const = 0 ;
	virtual Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const = 0 ;
} ;

class JsonParser implement JsonParserLayout {
protected:
	using JsonParserLayout::mThis ;
	using JsonParserLayout::mIndex ;

public:
	implicit JsonParser () = default ;

	explicit JsonParser (CREF<RefBuffer<BYTE>> stream) {
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

	JsonParser child (CREF<Slice> name) const {
		JsonParserLayout ret = JsonParserHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<JsonParser>::expr] (ret)) ;
	}

	JsonParser child (CREF<String<STRU8>> name) const {
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

	CREF<String<STRU8>> name () const leftvalue {
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

	String<STRA> fetch (CREF<String<STRA>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const {
		return JsonParserHolder::create (thiz)->fetch (def) ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const {
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

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const {
		return JsonParserHolder::create (thiz)->fetch (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const {
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

	virtual void initialize (CREF<RefBuffer<BYTE>> stream) = 0 ;
	virtual INDEX find_element (CREF<Slice> name) const = 0 ;
	virtual LENGTH element_size (CREF<INDEX> element) const = 0 ;
	virtual INDEX find_property (CREF<INDEX> element ,CREF<Slice> name) const = 0 ;
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

	explicit PlyParser (CREF<RefBuffer<BYTE>> stream) {
		PlyParserHolder::create (thiz)->initialize (stream) ;
	}

	INDEX find_element (CREF<Slice> name) const {
		return PlyParserHolder::create (thiz)->find_element (name) ;
	}

	LENGTH element_size (CREF<INDEX> element) const {
		return PlyParserHolder::create (thiz)->element_size (element) ;
	}

	INDEX find_property (CREF<INDEX> element ,CREF<Slice> name) const {
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