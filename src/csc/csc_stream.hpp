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
struct StreamProcImplLayout ;

struct StreamProcLayout {
	Ref<StreamProcImplLayout> mThis ;
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
	virtual BOOL is_ctrl (CREF<STRU32> str) const = 0 ;
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

	imports BOOL is_ctrl (CREF<STRU32> str) {
		return StreamProcHolder::create (instance ())->is_ctrl (str) ;
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

struct StreamReader ;

template <class...>
trait HAS_FRIEND_READ_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_READ_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_READ_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_read (nullof (StreamReader)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_READ = typename HAS_FRIEND_READ_HELP<A ,ALWAYS>::RET ;

struct StreamReader implement Interface {
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

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (VREF<ARG1> item) {
		item.friend_read (thiz) ;
	}
} ;

template <class A>
class StreamReaderBinder implement Fat<StreamReader ,A> {
public:
	imports VFat<StreamReader> create (VREF<A> that) {
		return VFat<StreamReader> (StreamReaderBinder () ,that) ;
	}

	void backup () override {
		return thiz.fake.backup () ;
	}

	void recover () override {
		return thiz.fake.recover () ;
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
} ;

struct ByteReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<ByteReaderLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct ByteReaderHolder implement Interface {
	imports VFat<ByteReaderHolder> create (VREF<ByteReaderLayout> that) ;
	imports CFat<ByteReaderHolder> create (CREF<ByteReaderLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
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

	void use_overflow (CREF<Function<VREF<ByteReader>>> overflow) {
		return ByteReaderHolder::create (thiz)->use_overflow (overflow) ;
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

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (VREF<ARG1> item) {
		item.friend_read (StreamReaderBinder<ByteReader>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VREF<ByteReader> operator>> (VREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct TextReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<TextReaderLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct TextReaderHolder implement Interface {
	imports VFat<TextReaderHolder> create (VREF<TextReaderLayout> that) ;
	imports CFat<TextReaderHolder> create (CREF<TextReaderLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
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

	void use_overflow (CREF<Function<VREF<TextReader>>> overflow) {
		return TextReaderHolder::create (thiz)->use_overflow (overflow) ;
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

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (VREF<ARG1> item) {
		item.friend_read (StreamReaderBinder<TextReader>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VREF<TextReader> operator>> (VREF<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct StreamWriter ;

template <class...>
trait HAS_FRIEND_WRITE_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_WRITE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_WRITE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_write (nullof (StreamWriter)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_WRITE = typename HAS_FRIEND_WRITE_HELP<A ,ALWAYS>::RET ;

struct StreamWriter implement Interface {
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

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (CREF<ARG1> item) {
		item.friend_write (thiz) ;
	}
} ;

template <class A>
class StreamWriterBinder implement Fat<StreamWriter ,A> {
public:
	imports VFat<StreamWriter> create (VREF<A> that) {
		return VFat<StreamWriter> (StreamWriterBinder () ,that) ;
	}

	void backup () override {
		return thiz.fake.backup () ;
	}

	void recover () override {
		return thiz.fake.recover () ;
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
} ;

struct ByteWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<ByteWriterLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct ByteWriterHolder implement Interface {
	imports VFat<ByteWriterHolder> create (VREF<ByteWriterLayout> that) ;
	imports CFat<ByteWriterHolder> create (CREF<ByteWriterLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
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

	void use_overflow (CREF<Function<VREF<ByteWriter>>> overflow) {
		return ByteWriterHolder::create (thiz)->use_overflow (overflow) ;
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

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (CREF<ARG1> item) {
		item.friend_write (StreamWriterBinder<ByteWriter>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VREF<ByteWriter> operator<< (CREF<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct TextWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	Function<VREF<TextWriterLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
	INDEX mBackupRead ;
	INDEX mBackupWrite ;
} ;

struct TextWriterHolder implement Interface {
	imports VFat<TextWriterHolder> create (VREF<TextWriterLayout> that) ;
	imports CFat<TextWriterHolder> create (CREF<TextWriterLayout> that) ;

	virtual void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CREF<FunctionLayout> overflow) = 0 ;
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

	void use_overflow (CREF<Function<VREF<TextWriter>>> overflow) {
		return TextWriterHolder::create (thiz)->use_overflow (overflow) ;
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

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (CREF<ARG1> item) {
		item.friend_write (StreamWriterBinder<TextWriter>::create (thiz).self) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VREF<TextWriter> operator<< (CREF<ARG1> item) {
		write (item) ;
		return thiz ;
	}
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

template <class A>
class StringBuild implement Proxy {
public:
	template <class...ARG1>
	imports String<A> make (CREF<ARG1>...params) {
		String<A> ret = String<A> (SLICE_MAX_SIZE::expr) ;
		auto rax = TextWriter (ret.borrow ()) ;
		make_impl (rax ,params...) ;
		rax << EOS ;
		return move (ret) ;
	}

	imports forceinline void make_impl (VREF<TextWriter> writer) {
		noop () ;
	}

	template <class ARG1 ,class...ARG2>
	imports forceinline void make_impl (VREF<TextWriter> writer ,CREF<ARG1> params1 ,CREF<ARG2>...params2) {
		writer << params1 ;
		make_impl (writer ,params2...) ;
	}
} ;

struct FormatFriend implement Interface {
	virtual void friend_write (VREF<StreamWriter> writer) const = 0 ;
} ;

template <class A>
class FormatFriendBinder implement Fat<FormatFriend ,A> {
public:
	imports CFat<FormatFriend> create (CREF<A> that) {
		return CFat<FormatFriend> (FormatFriendBinder () ,that) ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		writer.write (thiz.fake) ;
	}
} ;

struct FormatLayout {
	Slice mFormat ;
	Pin<BufferX<FatLayout>> mParams ;
	Pin<INDEX> mWrite ;

public:
	implicit FormatLayout () = default ;

	implicit FormatLayout (CREF<FormatLayout> that) = delete ;

	forceinline VREF<FormatLayout> operator= (CREF<FormatLayout> that) = delete ;

	implicit FormatLayout (RREF<FormatLayout> that) noexcept :FormatLayout () {
		swap (thiz ,that) ;
	}

	forceinline VREF<FormatLayout> operator= (RREF<FormatLayout> that) noexcept {
		return assign (thiz ,that) ;
	}
} ;

struct FormatHolder implement Interface {
	imports VFat<FormatHolder> create (VREF<FormatLayout> that) ;
	imports CFat<FormatHolder> create (CREF<FormatLayout> that) ;

	virtual void initialize (CREF<Slice> format) = 0 ;
	virtual void friend_write (VREF<StreamWriter> writer) const = 0 ;
	virtual void then (CREF<WrapperLayout> params) const = 0 ;
} ;

class Format implement FormatLayout {
protected:
	using FormatLayout::mFormat ;
	using FormatLayout::mParams ;
	using FormatLayout::mWrite ;

public:
	implicit Format () = default ;

	explicit Format (CREF<Slice> format) {
		FormatHolder::create (thiz)->initialize (format) ;
	}

	void friend_write (VREF<StreamWriter> writer) const {
		return FormatHolder::create (thiz)->friend_write (writer) ;
	}

	template <class...ARG1>
	void then (CREF<ARG1>...params) const {
		return FormatHolder::create (thiz)->then (MakeWrapper (FormatFriendBinder<ARG1>::create (params)...)) ;
	}

	template <class...ARG1>
	CREF<Format> operator() (CREF<ARG1>...params) const {
		then (params...) ;
		return thiz ;
	}
} ;

inline CREF<Format> PrintFormat (CREF<Format> params) {
	return params ;
}

template <class...ARG1>
inline Format PrintFormat (CREF<ARG1>...params) {
	Format ret = Format (slice ("${0}")) ;
	ret (params...) ;
	return move (ret) ;
}

struct StringProcLayout {
	RefLayout mThis ;
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

struct RegexImplLayout ;

struct RegexLayout {
	Ref<RegexImplLayout> mThis ;
} ;

struct RegexHolder implement Interface {
	imports VFat<RegexHolder> create (VREF<RegexLayout> that) ;
	imports CFat<RegexHolder> create (CREF<RegexLayout> that) ;

	virtual void initialize (CREF<String<STR>> format) = 0 ;
	virtual INDEX search (CREF<String<STR>> text ,CREF<INDEX> offset) = 0 ;
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

	INDEX search (CREF<String<STR>> text ,CREF<INDEX> offset) {
		return RegexHolder::create (thiz)->search (text ,offset) ;
	}

	String<STR> match (CREF<INDEX> index) const {
		return RegexHolder::create (thiz)->match (index) ;
	}
} ;

struct StreamTextProcLayout {
	RefLayout mThis ;
} ;

struct StreamTextProcHolder implement Interface {
	imports VFat<StreamTextProcHolder> create (VREF<StreamTextProcLayout> that) ;
	imports CFat<StreamTextProcHolder> create (CREF<StreamTextProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void read_identifer (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void read_scalar (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void read_escape (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void write_escape (VREF<StreamWriter> writer ,CREF<String<STRU8>> item) const = 0 ;
	virtual void read_endline (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const = 0 ;
	virtual void write_aligned (VREF<StreamWriter> writer ,CREF<VAL64> number ,CREF<LENGTH> align) const = 0 ;
} ;

class StreamTextProc implement StreamTextProcLayout {
protected:
	using StreamTextProcLayout::mThis ;

public:
	imports CREF<StreamTextProc> instance () {
		return memorize ([&] () {
			StreamTextProc ret ;
			StreamTextProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports void read_identifer (VREF<StreamReader> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::create (instance ())->read_identifer (reader ,item) ;
	}

	imports void read_scalar (VREF<StreamReader> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::create (instance ())->read_scalar (reader ,item) ;
	}

	imports void read_escape (VREF<StreamReader> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::create (instance ())->read_escape (reader ,item) ;
	}

	imports void write_escape (VREF<StreamWriter> writer ,CREF<String<STRU8>> item) {
		return StreamTextProcHolder::create (instance ())->write_escape (writer ,item) ;
	}

	imports void read_endline (VREF<StreamReader> reader ,VREF<String<STRU8>> item) {
		return StreamTextProcHolder::create (instance ())->read_endline (reader ,item) ;
	}

	imports void write_aligned (VREF<StreamWriter> writer ,CREF<VAL64> number ,CREF<LENGTH> align) {
		return StreamTextProcHolder::create (instance ())->write_aligned (writer ,number ,align) ;
	}
} ;

class IdentifierText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<IdentifierText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_identifer (StreamReaderBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

class ScalarText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<ScalarText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_scalar (StreamReaderBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

class EscapeText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<EscapeText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	imports CREF<EscapeText> from (CREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	imports CREF<EscapeText> from (RREF<String<STRU8>> that) = delete ;

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_escape (StreamReaderBinder<ARG1>::create (reader) ,mThat) ;
	}

	template <class ARG1>
	void friend_write (VREF<ARG1> writer) const {
		return StreamTextProc::write_escape (StreamWriterBinder<ARG1>::create (writer) ,mThat) ;
	}
} ;

class EndlineText implement Proxy {
protected:
	String<STRU8> mThat ;

public:
	imports VREF<EndlineText> from (VREF<String<STRU8>> that) {
		return Pointer::from (that) ;
	}

	template <class ARG1>
	void friend_read (VREF<ARG1> reader) {
		return StreamTextProc::read_endline (StreamReaderBinder<ARG1>::create (reader) ,mThat) ;
	}
} ;

struct AlignedTextLayout {
	VAL64 mNumber ;
	LENGTH mAlign ;
} ;

class AlignedText implement AlignedTextLayout {
protected:
	using AlignedTextLayout::mNumber ;
	using AlignedTextLayout::mAlign ;

public:
	implicit AlignedText () = delete ;

	explicit AlignedText (CREF<VAL64> number ,CREF<LENGTH> align) {
		mNumber = number ;
		mAlign = align ;
	}

	template <class ARG1>
	void friend_write (VREF<ARG1> writer) const {
		return StreamTextProc::write_aligned (StreamWriterBinder<ARG1>::create (writer) ,mNumber ,mAlign) ;
	}
} ;
} ;