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
struct StreamProcLayout ;

struct StreamProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<StreamProcLayout>>> expr_m () ;
	imports VFat<StreamProcHolder> hold (VR<StreamProcLayout> that) ;
	imports CFat<StreamProcHolder> hold (CR<StreamProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual BOOL big_endian () const = 0 ;
	virtual BOOL is_blank (CR<STRU32> str) const = 0 ;
	virtual BOOL is_space (CR<STRU32> str) const = 0 ;
	virtual BOOL is_endline (CR<STRU32> str) const = 0 ;
	virtual BOOL is_punct (CR<STRU32> str) const = 0 ;
	virtual BOOL is_hyphen (CR<STRU32> str) const = 0 ;
	virtual BOOL is_alpha (CR<STRU32> str) const = 0 ;
	virtual STRU32 alpha_lower (CR<STRU32> str) const = 0 ;
	virtual STRU32 alpha_upper (CR<STRU32> str) const = 0 ;
	virtual BOOL is_digit (CR<STRU32> str) const = 0 ;
	virtual BOOL is_hex_digit (CR<STRU32> str) const = 0 ;
	virtual INDEX hex_from_str (CR<STRU32> str) const = 0 ;
	virtual STRU32 str_from_hex (CR<INDEX> hex) const = 0 ;
	virtual BOOL is_word (CR<STRU32> str) const = 0 ;
	virtual BOOL is_ctrl (CR<STRU32> str) const = 0 ;
	virtual STRU32 word_from_ctrl (CR<STRU32> str) const = 0 ;
	virtual STRU32 ctrl_from_word (CR<STRU32> str) const = 0 ;
} ;

class StreamProc implement OfThis<UniqueRef<StreamProcLayout>> {
public:
	static CR<StreamProc> expr_m () {
		return keep[TYPE<StreamProc>::expr] (StreamProcHolder::expr) ;
	}

	static BOOL big_endian () {
		return StreamProcHolder::hold (expr)->big_endian () ;
	}

	static BOOL is_blank (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_blank (str) ;
	}

	static BOOL is_space (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_space (str) ;
	}

	static BOOL is_endline (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_endline (str) ;
	}

	static BOOL is_punct (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_punct (str) ;
	}

	static BOOL is_hyphen (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_hyphen (str) ;
	}

	static BOOL is_alpha (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_alpha (str) ;
	}

	static STRU32 alpha_lower (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->alpha_lower (str) ;
	}

	static STRU32 alpha_upper (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->alpha_upper (str) ;
	}

	static BOOL is_digit (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_digit (str) ;
	}

	static BOOL is_hex_digit (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_hex_digit (str) ;
	}

	static INDEX hex_from_str (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->hex_from_str (str) ;
	}

	static STRU32 str_from_hex (CR<INDEX> hex) {
		return StreamProcHolder::hold (expr)->str_from_hex (hex) ;
	}

	static BOOL is_word (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_word (str) ;
	}

	static BOOL is_ctrl (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->is_ctrl (str) ;
	}

	static STRU32 word_from_ctrl (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->word_from_ctrl (str) ;
	}

	static STRU32 ctrl_from_word (CR<STRU32> str) {
		return StreamProcHolder::hold (expr)->ctrl_from_word (str) ;
	}
} ;

struct StreamShape {
	INDEX mRead ;
	INDEX mWrite ;
} ;

static constexpr auto BOM = RANK1 () ;
static constexpr auto CAT = RANK2 () ;
static constexpr auto GAP = RANK3 () ;
static constexpr auto EOS = RANK4 () ;

struct FriendReader ;
struct FriendWriter ;

template <class...>
trait HAS_FRIEND_READ_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_READ_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_READ_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_read (nullof (VR<FriendReader>)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_READ = typename HAS_FRIEND_READ_HELP<A ,ALWAYS>::RET ;

struct FriendReader implement Interface {
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual StreamShape backup () const = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void read (VR<BOOL> item) = 0 ;
	virtual void read (VR<VAL32> item) = 0 ;
	virtual void read (VR<VAL64> item) = 0 ;
	virtual void read (VR<FLT32> item) = 0 ;
	virtual void read (VR<FLT64> item) = 0 ;
	virtual void read (VR<BYTE> item) = 0 ;
	virtual void read (VR<WORD> item) = 0 ;
	virtual void read (VR<CHAR> item) = 0 ;
	virtual void read (VR<QUAD> item) = 0 ;
	virtual void read (VR<STRU32> item) = 0 ;
	virtual void read (CR<Slice> item) = 0 ;
	virtual void read (VR<StringLayout> item) = 0 ;
	virtual void read (CR<typeof (BOM)> item) = 0 ;
	virtual void read (CR<typeof (CAT)> item) = 0 ;
	virtual void read (CR<typeof (GAP)> item) = 0 ;
	virtual void read (CR<typeof (EOS)> item) = 0 ;

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (XR<ARG1> item) {
		item.friend_read (thiz) ;
	}
} ;

template <class A>
class FriendReaderBinder final implement Fat<FriendReader ,A> {
public:
	static VFat<FriendReader> hold (VR<A> that) {
		return VFat<FriendReader> (FriendReaderBinder () ,that) ;
	}

	void reset () override {
		return thiz.self.reset () ;
	}

	void reset (CR<StreamShape> shape) override {
		return thiz.self.reset (shape) ;
	}

	StreamShape backup () const override {
		return thiz.self.backup () ;
	}

	BOOL good () const override {
		return thiz.self.good () ;
	}

	void read (VR<BOOL> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<VAL32> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<VAL64> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<FLT32> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<FLT64> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<BYTE> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<WORD> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<CHAR> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<QUAD> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<STRU32> item) override {
		return thiz.self.read (item) ;
	}

	void read (CR<Slice> item) override {
		return thiz.self.read (item) ;
	}

	void read (VR<StringLayout> item) override {
		return thiz.self.read (item) ;
	}

	void read (CR<typeof (BOM)> item) override {
		return thiz.self.read (item) ;
	}

	void read (CR<typeof (CAT)> item) override {
		return thiz.self.read (item) ;
	}

	void read (CR<typeof (GAP)> item) override {
		return thiz.self.read (item) ;
	}

	void read (CR<typeof (EOS)> item) override {
		return thiz.self.read (item) ;
	}
} ;

struct ByteReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	BOOL mDiffEndian ;
	Function<VR<ByteReaderLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct ByteReaderHolder implement Interface {
	imports VFat<ByteReaderHolder> hold (VR<ByteReaderLayout> that) ;
	imports CFat<ByteReaderHolder> hold (CR<ByteReaderLayout> that) ;

	virtual void initialize (RR<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<VR<ByteReaderLayout>>> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual StreamShape backup () const = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void read (VR<BOOL> item) = 0 ;
	virtual void read (VR<VAL32> item) = 0 ;
	virtual void read (VR<VAL64> item) = 0 ;
	virtual void read (VR<FLT32> item) = 0 ;
	virtual void read (VR<FLT64> item) = 0 ;
	virtual void read (VR<BYTE> item) = 0 ;
	virtual void read (VR<WORD> item) = 0 ;
	virtual void read (VR<CHAR> item) = 0 ;
	virtual void read (VR<QUAD> item) = 0 ;
	virtual void read (VR<STRU32> item) = 0 ;
	virtual void read (CR<Slice> item) = 0 ;
	virtual void read (VR<StringLayout> item) = 0 ;
	virtual void read (CR<typeof (BOM)> item) = 0 ;
	virtual void read (CR<typeof (CAT)> item) = 0 ;
	virtual void read (CR<typeof (GAP)> item) = 0 ;
	virtual void read (CR<typeof (EOS)> item) = 0 ;
} ;

class ByteReader implement ByteReaderLayout {
protected:
	using ByteReaderLayout::mStream ;
	using ByteReaderLayout::mDiffEndian ;
	using ByteReaderLayout::mOverflow ;
	using ByteReaderLayout::mRead ;
	using ByteReaderLayout::mWrite ;

public:
	implicit ByteReader () = default ;

	explicit ByteReader (RR<Ref<RefBuffer<BYTE>>> stream) {
		ByteReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CR<Function<VR<ByteReader>>> overflow) {
		return ByteReaderHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	LENGTH size () const {
		return ByteReaderHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return ByteReaderHolder::hold (thiz)->length () ;
	}

	void reset () {
		return ByteReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return ByteReaderHolder::hold (thiz)->reset (shape) ;
	}

	StreamShape backup () const {
		return ByteReaderHolder::hold (thiz)->backup () ;
	}

	BOOL good () const {
		return ByteReaderHolder::hold (thiz)->good () ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<BOOL> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<VAL32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<VAL64> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<FLT32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<FLT64> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<BYTE> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<WORD> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<CHAR> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<QUAD> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<QUAD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<STRU32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<STRU32> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<Slice> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (CR<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<StringLayout> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<StringLayout> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (BOM)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (CR<typeof (BOM)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (CAT)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (CR<typeof (CAT)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (GAP)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (CR<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (EOS)> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (CR<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (XR<ARG1> item) {
		const auto r1x = FriendReaderBinder<ByteReader>::hold (thiz) ;
		item.friend_read (r1x.ref) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VR<ByteReader> operator>> (XR<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct TextReaderLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	BOOL mDiffEndian ;
	Function<VR<TextReaderLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct TextReaderHolder implement Interface {
	imports VFat<TextReaderHolder> hold (VR<TextReaderLayout> that) ;
	imports CFat<TextReaderHolder> hold (CR<TextReaderLayout> that) ;

	virtual void initialize (RR<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<VR<TextReaderLayout>>> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual StreamShape backup () const = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void read (VR<BOOL> item) = 0 ;
	virtual void read (VR<VAL32> item) = 0 ;
	virtual void read (VR<VAL64> item) = 0 ;
	virtual void read (VR<FLT32> item) = 0 ;
	virtual void read (VR<FLT64> item) = 0 ;
	virtual void read (VR<BYTE> item) = 0 ;
	virtual void read (VR<WORD> item) = 0 ;
	virtual void read (VR<CHAR> item) = 0 ;
	virtual void read (VR<QUAD> item) = 0 ;
	virtual void read (VR<STRU32> item) = 0 ;
	virtual void read (CR<Slice> item) = 0 ;
	virtual void read (VR<StringLayout> item) = 0 ;
	virtual void read (CR<typeof (BOM)> item) = 0 ;
	virtual void read (CR<typeof (CAT)> item) = 0 ;
	virtual void read (CR<typeof (GAP)> item) = 0 ;
	virtual void read (CR<typeof (EOS)> item) = 0 ;
} ;

class TextReader implement TextReaderLayout {
protected:
	using TextReaderLayout::mStream ;
	using TextReaderLayout::mDiffEndian ;
	using TextReaderLayout::mOverflow ;
	using TextReaderLayout::mRead ;
	using TextReaderLayout::mWrite ;

public:
	implicit TextReader () = default ;

	explicit TextReader (RR<Ref<RefBuffer<BYTE>>> stream) {
		TextReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CR<Function<VR<TextReader>>> overflow) {
		return TextReaderHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	LENGTH size () const {
		return TextReaderHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return TextReaderHolder::hold (thiz)->length () ;
	}

	void reset () {
		return TextReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return TextReaderHolder::hold (thiz)->reset (shape) ;
	}

	StreamShape backup () const {
		return TextReaderHolder::hold (thiz)->backup () ;
	}

	BOOL good () const {
		return TextReaderHolder::hold (thiz)->good () ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<BOOL> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<BOOL> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<VAL32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<VAL32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<VAL64> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<VAL64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<FLT32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<FLT32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<FLT64> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<FLT64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<BYTE> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<BYTE> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<WORD> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<WORD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<CHAR> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<CHAR> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<QUAD> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<QUAD> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<STRU32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<STRU32> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<Slice> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (CR<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<StringLayout> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<StringLayout> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (BOM)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (CR<typeof (BOM)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (CAT)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (CR<typeof (CAT)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (GAP)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (CR<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (EOS)> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (CR<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (XR<ARG1> item) {
		const auto r1x = FriendReaderBinder<TextReader>::hold (thiz) ;
		item.friend_read (r1x.ref) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VR<TextReader> operator>> (XR<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

template <class...>
trait HAS_FRIEND_WRITE_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_WRITE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_WRITE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_write (nullof (VR<FriendWriter>)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_WRITE = typename HAS_FRIEND_WRITE_HELP<A ,ALWAYS>::RET ;

struct FriendWriter implement Interface {
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual StreamShape backup () const = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void write (CR<BOOL> item) = 0 ;
	virtual void write (CR<VAL32> item) = 0 ;
	virtual void write (CR<VAL64> item) = 0 ;
	virtual void write (CR<FLT32> item) = 0 ;
	virtual void write (CR<FLT64> item) = 0 ;
	virtual void write (CR<BYTE> item) = 0 ;
	virtual void write (CR<WORD> item) = 0 ;
	virtual void write (CR<CHAR> item) = 0 ;
	virtual void write (CR<QUAD> item) = 0 ;
	virtual void write (CR<STRU32> item) = 0 ;
	virtual void write (CR<Slice> item) = 0 ;
	virtual void write (CR<StringLayout> item) = 0 ;
	virtual void write (CR<typeof (BOM)> item) = 0 ;
	virtual void write (CR<typeof (CAT)> item) = 0 ;
	virtual void write (CR<typeof (GAP)> item) = 0 ;
	virtual void write (CR<typeof (EOS)> item) = 0 ;

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (XR<ARG1> item) {
		item.friend_write (thiz) ;
	}
} ;

template <class A>
class FriendWriterBinder final implement Fat<FriendWriter ,A> {
public:
	static VFat<FriendWriter> hold (VR<A> that) {
		return VFat<FriendWriter> (FriendWriterBinder () ,that) ;
	}

	void reset () override {
		return thiz.self.reset () ;
	}

	void reset (CR<StreamShape> shape) override {
		return thiz.self.reset (shape) ;
	}

	StreamShape backup () const override {
		return thiz.self.backup () ;
	}

	BOOL good () const override {
		return thiz.self.good () ;
	}

	void write (CR<BOOL> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<VAL32> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<VAL64> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<FLT32> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<FLT64> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<BYTE> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<WORD> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<CHAR> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<QUAD> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<STRU32> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<Slice> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<StringLayout> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<typeof (BOM)> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<typeof (CAT)> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<typeof (GAP)> item) override {
		return thiz.self.write (item) ;
	}

	void write (CR<typeof (EOS)> item) override {
		return thiz.self.write (item) ;
	}
} ;

struct ByteWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	BOOL mDiffEndian ;
	Function<VR<ByteWriterLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct ByteWriterHolder implement Interface {
	imports VFat<ByteWriterHolder> hold (VR<ByteWriterLayout> that) ;
	imports CFat<ByteWriterHolder> hold (CR<ByteWriterLayout> that) ;

	virtual void initialize (RR<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<VR<ByteWriterLayout>>> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual StreamShape backup () const = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void write (CR<BOOL> item) = 0 ;
	virtual void write (CR<VAL32> item) = 0 ;
	virtual void write (CR<VAL64> item) = 0 ;
	virtual void write (CR<FLT32> item) = 0 ;
	virtual void write (CR<FLT64> item) = 0 ;
	virtual void write (CR<BYTE> item) = 0 ;
	virtual void write (CR<WORD> item) = 0 ;
	virtual void write (CR<CHAR> item) = 0 ;
	virtual void write (CR<QUAD> item) = 0 ;
	virtual void write (CR<STRU32> item) = 0 ;
	virtual void write (CR<Slice> item) = 0 ;
	virtual void write (CR<StringLayout> item) = 0 ;
	virtual void write (CR<typeof (BOM)> item) = 0 ;
	virtual void write (CR<typeof (CAT)> item) = 0 ;
	virtual void write (CR<typeof (GAP)> item) = 0 ;
	virtual void write (CR<typeof (EOS)> item) = 0 ;
} ;

class ByteWriter implement ByteWriterLayout {
protected:
	using ByteWriterLayout::mStream ;
	using ByteWriterLayout::mDiffEndian ;
	using ByteWriterLayout::mOverflow ;
	using ByteWriterLayout::mRead ;
	using ByteWriterLayout::mWrite ;

public:
	implicit ByteWriter () = default ;

	explicit ByteWriter (RR<Ref<RefBuffer<BYTE>>> stream) {
		ByteWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CR<Function<VR<ByteWriter>>> overflow) {
		return ByteWriterHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	LENGTH size () const {
		return ByteWriterHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return ByteWriterHolder::hold (thiz)->length () ;
	}

	void reset () {
		return ByteWriterHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return ByteWriterHolder::hold (thiz)->reset (shape) ;
	}

	StreamShape backup () const {
		return ByteWriterHolder::hold (thiz)->backup () ;
	}

	BOOL good () const {
		return ByteWriterHolder::hold (thiz)->good () ;
	}

	void write (CR<BOOL> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<BOOL> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<VAL32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<VAL32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<VAL64> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<VAL64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<FLT32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<FLT32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<FLT64> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<FLT64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<BYTE> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<BYTE> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<WORD> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<WORD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<CHAR> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<CHAR> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<QUAD> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<QUAD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<STRU32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<STRU32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Slice> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<StringLayout> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<StringLayout> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (BOM)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<typeof (BOM)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (CAT)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<typeof (CAT)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (GAP)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (EOS)> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (XR<ARG1> item) {
		const auto r1x = FriendWriterBinder<ByteWriter>::hold (thiz) ;
		item.friend_write (r1x.ref) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VR<ByteWriter> operator<< (XR<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct TextWriterLayout {
	Ref<RefBuffer<BYTE>> mStream ;
	BOOL mDiffEndian ;
	Function<VR<TextWriterLayout>> mOverflow ;
	INDEX mRead ;
	INDEX mWrite ;
} ;

struct TextWriterHolder implement Interface {
	imports VFat<TextWriterHolder> hold (VR<TextWriterLayout> that) ;
	imports CFat<TextWriterHolder> hold (CR<TextWriterLayout> that) ;

	virtual void initialize (RR<Ref<RefBuffer<BYTE>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<VR<TextWriterLayout>>> overflow) = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual LENGTH length () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual StreamShape backup () const = 0 ;
	virtual BOOL good () const = 0 ;
	virtual void write (CR<BOOL> item) = 0 ;
	virtual void write (CR<VAL32> item) = 0 ;
	virtual void write (CR<VAL64> item) = 0 ;
	virtual void write (CR<FLT32> item) = 0 ;
	virtual void write (CR<FLT64> item) = 0 ;
	virtual void write (CR<BYTE> item) = 0 ;
	virtual void write (CR<WORD> item) = 0 ;
	virtual void write (CR<CHAR> item) = 0 ;
	virtual void write (CR<QUAD> item) = 0 ;
	virtual void write (CR<STRU32> item) = 0 ;
	virtual void write (CR<Slice> item) = 0 ;
	virtual void write (CR<StringLayout> item) = 0 ;
	virtual void write (CR<typeof (BOM)> item) = 0 ;
	virtual void write (CR<typeof (CAT)> item) = 0 ;
	virtual void write (CR<typeof (GAP)> item) = 0 ;
	virtual void write (CR<typeof (EOS)> item) = 0 ;
} ;

class TextWriter implement TextWriterLayout {
protected:
	using TextWriterLayout::mStream ;
	using TextWriterLayout::mDiffEndian ;
	using TextWriterLayout::mOverflow ;
	using TextWriterLayout::mRead ;
	using TextWriterLayout::mWrite ;

public:
	implicit TextWriter () = default ;

	explicit TextWriter (RR<Ref<RefBuffer<BYTE>>> stream) {
		TextWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}

	void use_overflow (CR<Function<VR<TextWriter>>> overflow) {
		return TextWriterHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	LENGTH size () const {
		return TextWriterHolder::hold (thiz)->size () ;
	}

	LENGTH length () const {
		return TextWriterHolder::hold (thiz)->length () ;
	}

	void reset () {
		return TextWriterHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return TextWriterHolder::hold (thiz)->reset (shape) ;
	}

	StreamShape backup () const {
		return TextWriterHolder::hold (thiz)->backup () ;
	}

	BOOL good () const {
		return TextWriterHolder::hold (thiz)->good () ;
	}

	void write (CR<BOOL> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<BOOL> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<VAL32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<VAL32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<VAL64> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<VAL64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<FLT32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<FLT32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<FLT64> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<FLT64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<BYTE> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<BYTE> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<WORD> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<WORD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<CHAR> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<CHAR> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<QUAD> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<QUAD> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<STRU32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<STRU32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Slice> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<StringLayout> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<StringLayout> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (BOM)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<typeof (BOM)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (CAT)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<typeof (CAT)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (GAP)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (EOS)> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (XR<ARG1> item) {
		const auto r1x = FriendWriterBinder<TextWriter>::hold (thiz) ;
		item.friend_write (r1x.ref) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VR<TextWriter> operator<< (XR<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

template <class A>
class StringParse implement Proxy {
public:
	static A make (CR<Slice> text) {
		return make (String<STR> (text)) ;
	}

	template <class ARG1>
	static A make (CR<String<ARG1>> text) {
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
	static String<A> make (CR<ARG1>...params) {
		String<A> ret = String<A> (SLICE_MAX_SIZE::expr) ;
		auto rax = TextWriter (ret.borrow ()) ;
		make_impl (rax ,params...) ;
		rax << EOS ;
		return move (ret) ;
	}

	forceinline static void make_impl (VR<TextWriter> writer) {
		noop () ;
	}

	template <class ARG1 ,class...ARG2>
	forceinline static void make_impl (VR<TextWriter> writer ,CR<ARG1> params1 ,CR<ARG2>...params2) {
		writer << params1 ;
		make_impl (writer ,params2...) ;
	}
} ;

struct FriendReading implement Interface {
	virtual void friend_read (VR<FriendReader> reader) = 0 ;
} ;

template <class A>
class FriendReadingBinder final implement Fat<FriendReading ,A> {
public:
	static VFat<FriendReading> hold (VR<A> that) {
		return VFat<FriendReading> (FriendReadingBinder () ,that) ;
	}

	void friend_read (VR<FriendReader> reader) const override {
		reader.read (thiz.self) ;
	}
} ;

struct FriendWriting implement Interface {
	virtual void friend_write (VR<FriendWriter> writer) const = 0 ;
} ;

template <class A>
class FriendWritingBinder final implement Fat<FriendWriting ,A> {
public:
	static CFat<FriendWriting> hold (CR<A> that) {
		return CFat<FriendWriting> (FriendWritingBinder () ,that) ;
	}

	void friend_write (VR<FriendWriter> writer) const override {
		writer.write (thiz.self) ;
	}
} ;

struct FormatLayout {
	Slice mFormat ;
	BufferX<FatLayout> mParams ;
	INDEX mWrite ;
} ;

struct FormatHolder implement Interface {
	imports VFat<FormatHolder> hold (VR<FormatLayout> that) ;
	imports CFat<FormatHolder> hold (CR<FormatLayout> that) ;

	virtual void initialize (CR<Slice> format) = 0 ;
	virtual void friend_write (VR<FriendWriter> writer) const = 0 ;
	virtual void once (CR<WrapperLayout> params) const = 0 ;
} ;

class Format implement FormatLayout {
protected:
	using FormatLayout::mFormat ;
	using FormatLayout::mParams ;
	using FormatLayout::mWrite ;

public:
	implicit Format () = default ;

	explicit Format (CR<Slice> format) {
		FormatHolder::hold (thiz)->initialize (format) ;
	}

	void friend_write (VR<FriendWriter> writer) const {
		return FormatHolder::hold (thiz)->friend_write (writer) ;
	}

	template <class...ARG1>
	void once (CR<ARG1>...params) const {
		return FormatHolder::hold (thiz)->once (MakeWrapper (FriendWritingBinder<ARG1>::hold (params)...)) ;
	}

	template <class...ARG1>
	CR<Format> operator() (CR<ARG1>...params) const {
		once (params...) ;
		return thiz ;
	}
} ;

inline CR<Format> PrintFormat (CR<Format> params) {
	return params ;
}

template <class...ARG1>
inline Format PrintFormat (CR<ARG1>...params) {
	Format ret = Format (slice ("${0}")) ;
	ret (params...) ;
	return move (ret) ;
}

struct StreamTextProcLayout ;

struct StreamTextProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<StreamTextProcLayout>>> expr_m () ;
	imports VFat<StreamTextProcHolder> hold (VR<StreamTextProcLayout> that) ;
	imports CFat<StreamTextProcHolder> hold (CR<StreamTextProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void read_keyword (VR<FriendReader> reader ,VR<String<STRU8>> item) const = 0 ;
	virtual void read_scalar (VR<FriendReader> reader ,VR<String<STRU8>> item) const = 0 ;
	virtual void read_escape (VR<FriendReader> reader ,VR<String<STRU8>> item) const = 0 ;
	virtual void write_escape (VR<FriendWriter> writer ,CR<String<STRU8>> item) const = 0 ;
	virtual void read_blank (VR<FriendReader> reader ,VR<String<STRU8>> item) const = 0 ;
	virtual void read_endline (VR<FriendReader> reader ,VR<String<STRU8>> item) const = 0 ;
	virtual void write_aligned (VR<FriendWriter> writer ,CR<VAL64> number ,CR<LENGTH> align) const = 0 ;
} ;

class StreamTextProc implement OfThis<UniqueRef<StreamTextProcLayout>> {
public:
	static CR<StreamTextProc> expr_m () {
		return keep[TYPE<StreamTextProc>::expr] (StreamTextProcHolder::expr) ;
	}

	static void read_keyword (VR<FriendReader> reader ,VR<String<STRU8>> item) {
		return StreamTextProcHolder::hold (expr)->read_keyword (reader ,item) ;
	}

	static void read_scalar (VR<FriendReader> reader ,VR<String<STRU8>> item) {
		return StreamTextProcHolder::hold (expr)->read_scalar (reader ,item) ;
	}

	static void read_escape (VR<FriendReader> reader ,VR<String<STRU8>> item) {
		return StreamTextProcHolder::hold (expr)->read_escape (reader ,item) ;
	}

	static void write_escape (VR<FriendWriter> writer ,CR<String<STRU8>> item) {
		return StreamTextProcHolder::hold (expr)->write_escape (writer ,item) ;
	}

	static void read_blank (VR<FriendReader> reader ,VR<String<STRU8>> item) {
		return StreamTextProcHolder::hold (expr)->read_blank (reader ,item) ;
	}

	static void read_endline (VR<FriendReader> reader ,VR<String<STRU8>> item) {
		return StreamTextProcHolder::hold (expr)->read_endline (reader ,item) ;
	}

	static void write_aligned (VR<FriendWriter> writer ,CR<VAL64> number ,CR<LENGTH> align) {
		return StreamTextProcHolder::hold (expr)->write_aligned (writer ,number ,align) ;
	}
} ;

class ReadKeyword {
protected:
	VR<String<STRU8>> mThat ;

public:
	implicit ReadKeyword () = delete ;

	explicit ReadKeyword (VR<String<STRU8>> that) :mThat (that) {}

	void friend_read (VR<FriendReader> reader) const {
		return StreamTextProc::read_keyword (reader ,mThat) ;
	}
} ;

class ReadScalar {
protected:
	VR<String<STRU8>> mThat ;

public:
	implicit ReadScalar () = delete ;

	explicit ReadScalar (VR<String<STRU8>> that) :mThat (that) {}

	void friend_read (VR<FriendReader> reader) const {
		return StreamTextProc::read_scalar (reader ,mThat) ;
	}
} ;

class ReadEscape {
protected:
	VR<String<STRU8>> mThat ;

public:
	implicit ReadEscape () = delete ;

	explicit ReadEscape (VR<String<STRU8>> that) :mThat (that) {}

	void friend_read (VR<FriendReader> reader) const {
		return StreamTextProc::read_escape (reader ,mThat) ;
	}
} ;

class WriteEscape {
protected:
	CR<String<STRU8>> mThat ;

public:
	implicit WriteEscape () = delete ;

	explicit WriteEscape (CR<String<STRU8>> that) :mThat (that) {}

	void friend_read (VR<FriendWriter> writer) const {
		return StreamTextProc::write_escape (writer ,mThat) ;
	}
} ;

class ReadBlank {
protected:
	VR<String<STRU8>> mThat ;

public:
	implicit ReadBlank () = delete ;

	explicit ReadBlank (VR<String<STRU8>> that) :mThat (that) {}

	void friend_read (VR<FriendReader> reader) const {
		return StreamTextProc::read_blank (reader ,mThat) ;
	}
} ;

class ReadEndline {
protected:
	VR<String<STRU8>> mThat ;

public:
	implicit ReadEndline () = delete ;

	explicit ReadEndline (VR<String<STRU8>> that) :mThat (that) {}

	void friend_read (VR<FriendReader> reader) const {
		return StreamTextProc::read_endline (reader ,mThat) ;
	}
} ;

class AlignedText {
protected:
	VAL64 mNumber ;
	LENGTH mAlign ;

public:
	implicit AlignedText () = delete ;

	explicit AlignedText (CR<VAL64> number ,CR<LENGTH> align) {
		mNumber = number ;
		mAlign = align ;
	}

	void friend_write (VR<FriendWriter> writer) const {
		return StreamTextProc::write_aligned (writer ,mNumber ,mAlign) ;
	}
} ;

struct CommaLayout ;

struct CommaHolder implement Interface {
	imports SharedRef<CommaLayout> create () ;
	imports VFat<CommaHolder> hold (VR<CommaLayout> that) ;
	imports CFat<CommaHolder> hold (CR<CommaLayout> that) ;

	virtual void initialize (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) = 0 ;
	virtual void friend_write (VR<FriendWriter> writer) = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
	virtual void tight () = 0 ;
} ;

class Comma implement OfThis<SharedRef<CommaLayout>> {
public:
	implicit Comma () = default ;

	explicit Comma (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) {
		mThis = CommaHolder::create () ;
		CommaHolder::hold (thiz)->initialize (indent ,comma ,endline) ;
	}

	void friend_write (VR<FriendWriter> writer) const {
		return CommaHolder::hold (thiz)->friend_write (writer) ;
	}

	void increase () const {
		return CommaHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) const {
		return increase () ;
	}

	void decrease () const {
		return CommaHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) const {
		return decrease () ;
	}

	void tight () const {
		return CommaHolder::hold (thiz)->tight () ;
	}
} ;

struct RegexLayout ;

struct RegexHolder implement Interface {
	imports AutoRef<RegexLayout> create () ;
	imports VFat<RegexHolder> hold (VR<RegexLayout> that) ;
	imports CFat<RegexHolder> hold (CR<RegexLayout> that) ;

	virtual void initialize (CR<String<STR>> format) = 0 ;
	virtual INDEX search (RR<Ref<String<STR>>> text ,CR<INDEX> offset) = 0 ;
	virtual Slice match (CR<INDEX> index) const = 0 ;
} ;

class Regex implement OfThis<AutoRef<RegexLayout>> {
public:
	implicit Regex () = default ;

	explicit Regex (CR<String<STR>> format) {
		mThis = RegexHolder::create () ;
		RegexHolder::hold (thiz)->initialize (format) ;
	}

	INDEX search (RR<Ref<String<STR>>> text ,CR<INDEX> offset) {
		return RegexHolder::hold (thiz)->search (move (text) ,offset) ;
	}

	Slice match (CR<INDEX> index) const {
		return RegexHolder::hold (thiz)->match (index) ;
	}
} ;
} ;