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
		return StreamProcHolder::create (instance ())->is_word (str) ;
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

struct StreamReaderFriend ;

struct StreamReader implement Interface {
	virtual void read (VREF<BOOL> item) = 0 ;
	virtual void read (VREF<VAL32> item) = 0 ;
	virtual void read (VREF<VAL64> item) = 0 ;
	virtual void read (VREF<FLT32> item) = 0 ;
	virtual void read (VREF<FLT64> item) = 0 ;
	virtual void read (VREF<BYTE> item) = 0 ;
	virtual void read (VREF<WORD> item) = 0 ;
	virtual void read (VREF<CHAR> item) = 0 ;
	virtual void read (VREF<QUAD> item) = 0 ;
	virtual void read (VREF<STRU32> item) = 0 ;
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
	virtual void read (VREF<StreamReaderFriend> item) = 0 ;
} ;

template <class A>
class StreamReaderBinder implement Fat<StreamReader ,A> {
public:
	imports VFat<StreamReader> create (VREF<A> that) {
		return VFat<StreamReader> (StreamReaderBinder () ,that) ;
	}

	void read (VREF<BOOL> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<VAL32> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<VAL64> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<FLT32> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<FLT64> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<BYTE> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<WORD> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<CHAR> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<QUAD> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<STRU32> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<Slice> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRA>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRW>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRU8>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRU16>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<String<STRU32>> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (CLS)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (BOM)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (GAP)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (CREF<typeof (EOS)> item) override {
		return thiz.fake.read (item) ;
	}

	void read (VREF<StreamReaderFriend> item) override {
		return thiz.fake.read (item) ;
	}
} ;

struct StreamReaderFriend implement Interface {
	virtual void friend_read (VREF<StreamReader> reader) = 0 ;
} ;

template <class A>
class StreamReaderFriendBinder implement Fat<StreamReaderFriend ,A> {
public:
	imports VFat<StreamReaderFriend> create (VREF<A> that) {
		return VFat<StreamReaderFriend> (StreamReaderFriendBinder () ,that) ;
	}

	void friend_read (VREF<StreamReader> reader) override {
		reader.read (thiz.fake) ;
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
	virtual void read (VREF<STRU32> item) = 0 ;
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
	virtual void read (VREF<StreamReaderFriend> item) = 0 ;
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

	void read (VREF<STRU32> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<STRU32> item) {
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

	void read (CREF<typeof (BOM)> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (CREF<typeof (BOM)> item) {
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

	void read (VREF<StreamReaderFriend> item) {
		return ByteReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<ByteReader> operator>> (VREF<StreamReaderFriend> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	void read (VREF<ARG1> item) = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	forceinline VREF<ByteReader> operator>> (VREF<ARG1> item) = delete ;
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
	virtual void read (VREF<STRU32> item) = 0 ;
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
	virtual void read (VREF<StreamReaderFriend> item) = 0 ;
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

	void read (VREF<STRU32> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<STRU32> item) {
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

	void read (VREF<StreamReaderFriend> item) {
		return TextReaderHolder::create (thiz)->read (item) ;
	}

	forceinline VREF<TextReader> operator>> (VREF<StreamReaderFriend> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	void read (VREF<ARG1> item) = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	forceinline VREF<ByteReader> operator>> (VREF<ARG1> item) = delete ;
} ;

struct ReflectWriter implement Interface {
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<STRU32> item) = 0 ;
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;

	imports forceinline consteval FLAG expr_m () noexcept {
		return 202 ;
	}
} ;

struct StreamWriterFriend ;

struct StreamWriter implement Interface {
	virtual void write (CREF<BOOL> item) = 0 ;
	virtual void write (CREF<VAL32> item) = 0 ;
	virtual void write (CREF<VAL64> item) = 0 ;
	virtual void write (CREF<FLT32> item) = 0 ;
	virtual void write (CREF<FLT64> item) = 0 ;
	virtual void write (CREF<BYTE> item) = 0 ;
	virtual void write (CREF<WORD> item) = 0 ;
	virtual void write (CREF<CHAR> item) = 0 ;
	virtual void write (CREF<QUAD> item) = 0 ;
	virtual void write (CREF<STRU32> item) = 0 ;
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
	virtual void write (CREF<StreamWriterFriend> item) = 0 ;
} ;

template <class A>
class StreamWriterBinder implement Fat<StreamWriter ,A> {
public:
	imports VFat<StreamWriter> create (VREF<A> that) {
		return VFat<StreamWriter> (StreamWriterBinder () ,that) ;
	}

	void write (CREF<BOOL> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<VAL32> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<VAL64> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<FLT32> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<FLT64> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<BYTE> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<WORD> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<CHAR> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<QUAD> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<STRU32> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<Slice> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRA>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRW>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRU8>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRU16>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<String<STRU32>> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (BOM)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (CLS)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (GAP)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<typeof (EOS)> item) override {
		return thiz.fake.write (item) ;
	}

	void write (CREF<StreamWriterFriend> item) override {
		return thiz.fake.write (item) ;
	}
} ;

struct StreamWriterFriend implement Interface {
	virtual void friend_write (VREF<StreamWriter> writer) const = 0 ;
} ;

template <class A>
class StreamWriterFriendBinder implement Fat<StreamWriterFriend ,A> {
public:
	imports CFat<StreamWriterFriend> create (CREF<A> that) {
		return CFat<StreamWriterFriend> (StreamWriterFriendBinder () ,that) ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		writer.write (thiz.fake) ;
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
	virtual void write (CREF<STRU32> item) = 0 ;
	virtual void write (CREF<Slice> item) = 0 ;
	virtual void write (CREF<String<STRA>> item) = 0 ;
	virtual void write (CREF<String<STRW>> item) = 0 ;
	virtual void write (CREF<String<STRU8>> item) = 0 ;
	virtual void write (CREF<String<STRU16>> item) = 0 ;
	virtual void write (CREF<String<STRU32>> item) = 0 ;
	virtual void write (CREF<typeof (BOM)> item) = 0 ;
	virtual void write (CREF<typeof (CLS)> item) = 0 ;
	virtual void write (CREF<typeof (GAP)> item) = 0 ;
	virtual void write (CREF<typeof (EOS)> item) = 0 ;
	virtual void write (CREF<StreamWriterFriend> item) = 0 ;
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

	void write (CREF<STRU32> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<STRU32> item) {
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

	void write (CREF<typeof (BOM)> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<typeof (BOM)> item) {
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

	void write (CREF<StreamWriterFriend> item) {
		return ByteWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<ByteWriter> operator<< (CREF<StreamWriterFriend> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	void write (CREF<ARG1> item) = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	forceinline VREF<ByteWriter> operator<< (CREF<ARG1> item) = delete ;
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
	virtual void write (CREF<STRU32> item) = 0 ;
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
	virtual void write (CREF<StreamWriterFriend> item) = 0 ;
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

	void write (CREF<STRU32> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<STRU32> item) {
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

	void write (CREF<StreamWriterFriend> item) {
		return TextWriterHolder::create (thiz)->write (item) ;
	}

	forceinline VREF<TextWriter> operator<< (CREF<StreamWriterFriend> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	void write (CREF<ARG1> item) = delete ;

	template <class ARG1 ,class = REQUIRE<ENUM_NOT<IS_OBJECT<ARG1>>>>
	forceinline VREF<TextWriter> operator<< (CREF<ARG1> item) = delete ;
} ;

template <class A>
class StringParse implement Proxy {
public:
	imports A make (CREF<Slice> text) {
		return make (String<STR> (text)) ;
	}

	template <class ARG1>
	imports A make (CREF<String<ARG1>> text) {
		A ret ;
		auto rax = TextReader (text.borrow ()) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}
} ;

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated"
#pragma clang diagnostic ignored "-Wdangling"
#endif

template <class A>
class StringBuild implement Proxy {
public:
	template <class...ARG1>
	imports String<A> make (CREF<ARG1>...params) {
		String<A> ret = String<A> (DEFAULT_SLICE_SIZE::expr) ;
		auto rax = TextWriter (ret.borrow ()) ;
		const auto r1x = csc_initializer_list_t<FLAG> ({(rax << params ,ZERO)...}) ;
		noop (r1x) ;
		rax << EOS ;
		return move (ret) ;
	}
} ;

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic pop
#endif

struct FormatLayout {
	Slice mFormat ;
	BoxBuffer<RFat<StreamWriterFriend> ,RANKX> mParams ;
	INDEX mWrite ;
} ;

struct FormatHolder implement Interface {
	imports VFat<FormatHolder> create (VREF<FormatLayout> that) ;
	imports CFat<FormatHolder> create (CREF<FormatLayout> that) ;

	virtual void initialize (CREF<Slice> format) = 0 ;
	virtual void friend_write (VREF<StreamWriter> writer) const = 0 ;
	virtual void invoke (CREF<WrapperLayout> params) = 0 ;
} ;

class Format implement FormatLayout ,public StreamWriterFriend {
protected:
	using FormatLayout::mFormat ;
	using FormatLayout::mParams ;
	using FormatLayout::mWrite ;

public:
	implicit Format () = default ;

	implicit Format (CREF<Slice> format) {
		FormatHolder::create (thiz)->initialize (format) ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		return FormatHolder::create (thiz)->friend_write (writer) ;
	}

	template <class...ARG1>
	void invoke (CREF<ARG1>...params) {
		return FormatHolder::create (thiz)->invoke (MakeWrapper (StreamWriterFriendBinder<ARG1>::create (params)...)) ;
	}

	template <class...ARG1>
	VREF<Format> operator() (CREF<ARG1>...params) {
		invoke (params...) ;
		return thiz ;
	}
} ;

inline CREF<Format> PrintFormat (CREF<Format> params) {
	return params ;
}

template <class...ARG1>
inline Format PrintFormat (CREF<ARG1>...params) {
	Format ret = slice ("${*}") ;
	ret (params...) ;
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

struct RegexPureLayout ;

struct RegexLayout {
	Ref<RegexPureLayout> mThis ;
} ;

struct RegexHolder implement Interface {
	imports VFat<RegexHolder> create (VREF<RegexLayout> that) ;
	imports CFat<RegexHolder> create (CREF<RegexLayout> that) ;

	virtual void initialize (CREF<String<STR>> format) = 0 ;
	virtual INDEX search (CREF<String<STR>> text ,CREF<INDEX> index) = 0 ;
	virtual String<STR> match (CREF<INDEX> index) const = 0 ;
} ;

class Regex implement RegexLayout {
protected:
	using RegexLayout::mThis ;

public:
	implicit Regex () = default ;

	explicit Regex (CREF<String<STR>> format) {
		RegexHolder::create (thiz)->initialize (format) ;
	}

	INDEX search (CREF<String<STR>> text ,CREF<INDEX> index) {
		return RegexHolder::create (thiz)->search (text ,index) ;
	}

	String<STR> match (CREF<INDEX> index) const {
		return RegexHolder::create (thiz)->match (index) ;
	}
} ;
} ;