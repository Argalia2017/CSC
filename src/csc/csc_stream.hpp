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
	imports CR<Like<UniqueRef<StreamProcLayout>>> expr_m () ;
	imports VFat<StreamProcHolder> hold (VR<StreamProcLayout> that) ;
	imports CFat<StreamProcHolder> hold (CR<StreamProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Bool big_endian () const = 0 ;
	virtual Bool is_blank (CR<Stru32> str) const = 0 ;
	virtual Bool is_space (CR<Stru32> str) const = 0 ;
	virtual Bool is_endline (CR<Stru32> str) const = 0 ;
	virtual Bool is_punct (CR<Stru32> str) const = 0 ;
	virtual Bool is_hyphen (CR<Stru32> str) const = 0 ;
	virtual Bool is_alpha (CR<Stru32> str) const = 0 ;
	virtual Stru32 alpha_lower (CR<Stru32> str) const = 0 ;
	virtual Stru32 alpha_upper (CR<Stru32> str) const = 0 ;
	virtual Bool is_digit (CR<Stru32> str) const = 0 ;
	virtual Bool is_hex_digit (CR<Stru32> str) const = 0 ;
	virtual Index hex_from_str (CR<Stru32> str) const = 0 ;
	virtual Stru32 str_from_hex (CR<Index> hex) const = 0 ;
	virtual Bool is_word (CR<Stru32> str) const = 0 ;
	virtual Bool is_ctrl (CR<Stru32> str) const = 0 ;
	virtual Stru32 word_from_ctrl (CR<Stru32> str) const = 0 ;
	virtual Stru32 ctrl_from_word (CR<Stru32> str) const = 0 ;
} ;

class StreamProc implement Like<UniqueRef<StreamProcLayout>> {
public:
	static CR<StreamProc> expr_m () {
		return keep[TYPE<StreamProc>::expr] (StreamProcHolder::expr) ;
	}

	static Bool big_endian () {
		return StreamProcHolder::hold (expr)->big_endian () ;
	}

	static Bool is_blank (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_blank (str) ;
	}

	static Bool is_space (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_space (str) ;
	}

	static Bool is_endline (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_endline (str) ;
	}

	static Bool is_punct (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_punct (str) ;
	}

	static Bool is_hyphen (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_hyphen (str) ;
	}

	static Bool is_alpha (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_alpha (str) ;
	}

	static Stru32 alpha_lower (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->alpha_lower (str) ;
	}

	static Stru32 alpha_upper (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->alpha_upper (str) ;
	}

	static Bool is_digit (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_digit (str) ;
	}

	static Bool is_hex_digit (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_hex_digit (str) ;
	}

	static Index hex_from_str (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->hex_from_str (str) ;
	}

	static Stru32 str_from_hex (CR<Index> hex) {
		return StreamProcHolder::hold (expr)->str_from_hex (hex) ;
	}

	static Bool is_word (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_word (str) ;
	}

	static Bool is_ctrl (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->is_ctrl (str) ;
	}

	static Stru32 word_from_ctrl (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->word_from_ctrl (str) ;
	}

	static Stru32 ctrl_from_word (CR<Stru32> str) {
		return StreamProcHolder::hold (expr)->ctrl_from_word (str) ;
	}
} ;

struct StreamShape {
	Index mRead ;
	Index mWrite ;
} ;

static constexpr auto BOM = RANK1 () ;
static constexpr auto CAT = RANK2 () ;
static constexpr auto GAP = RANK3 () ;
static constexpr auto EOS = RANK4 () ;

class ByteUnknownBinder final implement Fat<FriendUnknown ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		return ZERO ;
	}
} ;

class TextUnknownBinder final implement Fat<FriendUnknown ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		return ZERO ;
	}
} ;

class Reader ;

template <class...>
trait HAS_FRIEND_READ_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_READ_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_READ_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_read (nullof (VR<Reader>)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_READ = typename HAS_FRIEND_READ_HELP<A ,ALWAYS>::RET ;

struct ReaderLayout {
	Flag mHolder ;
	Ref<RefBuffer<Byte>> mStream ;
	Bool mDiffEndian ;
	Function<VR<ReaderLayout>> mOverflow ;
	Index mRead ;
	Index mWrite ;
} ;

struct ReaderHolder implement Interface {
	imports VFat<ReaderHolder> hold (VR<ReaderLayout> that) ;
	imports CFat<ReaderHolder> hold (CR<ReaderLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (RR<Ref<RefBuffer<Byte>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<VR<ReaderLayout>>> overflow) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length length () const = 0 ;
	virtual StreamShape shape () const = 0 ;
	virtual Bool good () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual void read (VR<Bool> item) = 0 ;
	virtual void read (VR<Val32> item) = 0 ;
	virtual void read (VR<Val64> item) = 0 ;
	virtual void read (VR<Flt32> item) = 0 ;
	virtual void read (VR<Flt64> item) = 0 ;
	virtual void read (VR<Byte> item) = 0 ;
	virtual void read (VR<Word> item) = 0 ;
	virtual void read (VR<Char> item) = 0 ;
	virtual void read (VR<Quad> item) = 0 ;
	virtual void read (VR<Stru32> item) = 0 ;
	virtual void read (CR<Slice> item) = 0 ;
	virtual void read (VR<StringLayout> item) = 0 ;
	virtual void read (CR<typeof (BOM)> item) = 0 ;
	virtual void read (CR<typeof (CAT)> item) = 0 ;
	virtual void read (CR<typeof (GAP)> item) = 0 ;
	virtual void read (CR<typeof (EOS)> item) = 0 ;
} ;

class Reader implement ReaderLayout {
protected:
	using ReaderLayout::mHolder ;
	using ReaderLayout::mStream ;
	using ReaderLayout::mDiffEndian ;
	using ReaderLayout::mOverflow ;
	using ReaderLayout::mRead ;
	using ReaderLayout::mWrite ;

public:
	implicit Reader () = default ;

	void use_overflow (CR<Function<VR<Reader>>> overflow) {
		return ReaderHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	Length size () const {
		return ReaderHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return ReaderHolder::hold (thiz)->length () ;
	}

	StreamShape shape () const {
		return ReaderHolder::hold (thiz)->shape () ;
	}

	Bool good () const {
		return ReaderHolder::hold (thiz)->good () ;
	}

	void reset () {
		return ReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return ReaderHolder::hold (thiz)->reset (shape) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<Bool> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Bool> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val32> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Val32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val64> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Val64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt32> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Flt32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt64> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Flt64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Byte> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Byte> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Word> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Word> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Char> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Char> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Quad> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Quad> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Stru32> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<Stru32> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<Slice> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (CR<Slice> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<StringLayout> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (VR<StringLayout> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (BOM)> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (CR<typeof (BOM)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (CAT)> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (CR<typeof (CAT)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (GAP)> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (CR<typeof (GAP)> item) {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (EOS)> item) {
		return ReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<Reader> operator>> (CR<typeof (EOS)> item) {
		read (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	void read (XR<ARG1> item) {
		item.friend_read (thiz) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_READ<ARG1>>>
	forceinline VR<Reader> operator>> (XR<ARG1> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct ByteReaderHolder implement ReaderHolder {
	imports VFat<ByteReaderHolder> hold (VR<ReaderLayout> that) ;
	imports CFat<ByteReaderHolder> hold (CR<ReaderLayout> that) ;
} ;

struct ByteReaderImplLayout implement Reader {
public:
	implicit ByteReaderImplLayout () noexcept {
		ByteReaderHolder::hold (thiz)->prepare (ByteUnknownBinder ()) ;
	}
} ;

class ByteReader implement ByteReaderImplLayout {
public:
	implicit ByteReader () = default ;

	explicit ByteReader (RR<Ref<RefBuffer<Byte>>> stream) {
		ByteReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}
} ;

struct TextReaderHolder implement ReaderHolder {
	imports VFat<TextReaderHolder> hold (VR<ReaderLayout> that) ;
	imports CFat<TextReaderHolder> hold (CR<ReaderLayout> that) ;
} ;

struct TextReaderImplLayout implement Reader {
public:
	implicit TextReaderImplLayout () noexcept {
		TextReaderHolder::hold (thiz)->prepare (TextUnknownBinder ()) ;
	}
} ;

class TextReader implement TextReaderImplLayout {
public:
	implicit TextReader () = default ;

	explicit TextReader (RR<Ref<RefBuffer<Byte>>> stream) {
		TextReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}
} ;

class Writer ;

template <class...>
trait HAS_FRIEND_WRITE_HELP ;

template <class A ,class OTHERWISE>
trait HAS_FRIEND_WRITE_HELP<A ,OTHERWISE> {
	using RET = ENUM_FALSE ;
} ;

template <class A>
trait HAS_FRIEND_WRITE_HELP<A ,REQUIRE<KILL<ENUM_TRUE ,typeof (nullof (A).friend_write (nullof (VR<Writer>)))>>> {
	using RET = ENUM_TRUE ;
} ;

template <class A>
using HAS_FRIEND_WRITE = typename HAS_FRIEND_WRITE_HELP<A ,ALWAYS>::RET ;

struct WriterLayout {
	Flag mHolder ;
	Ref<RefBuffer<Byte>> mStream ;
	Bool mDiffEndian ;
	Function<VR<WriterLayout>> mOverflow ;
	Index mRead ;
	Index mWrite ;
} ;

struct WriterHolder implement Interface {
	imports VFat<WriterHolder> hold (VR<WriterLayout> that) ;
	imports CFat<WriterHolder> hold (CR<WriterLayout> that) ;

	virtual void prepare (CR<Unknown> holder) = 0 ;
	virtual void initialize (RR<Ref<RefBuffer<Byte>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<VR<WriterLayout>>> overflow) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length length () const = 0 ;
	virtual StreamShape shape () const = 0 ;
	virtual Bool good () const = 0 ;
	virtual void reset () = 0 ;
	virtual void reset (CR<StreamShape> shape) = 0 ;
	virtual void write (CR<Bool> item) = 0 ;
	virtual void write (CR<Val32> item) = 0 ;
	virtual void write (CR<Val64> item) = 0 ;
	virtual void write (CR<Flt32> item) = 0 ;
	virtual void write (CR<Flt64> item) = 0 ;
	virtual void write (CR<Byte> item) = 0 ;
	virtual void write (CR<Word> item) = 0 ;
	virtual void write (CR<Char> item) = 0 ;
	virtual void write (CR<Quad> item) = 0 ;
	virtual void write (CR<Stru32> item) = 0 ;
	virtual void write (CR<Slice> item) = 0 ;
	virtual void write (CR<StringLayout> item) = 0 ;
	virtual void write (CR<typeof (BOM)> item) = 0 ;
	virtual void write (CR<typeof (CAT)> item) = 0 ;
	virtual void write (CR<typeof (GAP)> item) = 0 ;
	virtual void write (CR<typeof (EOS)> item) = 0 ;
} ;

class Writer implement WriterLayout {
protected:
	using WriterLayout::mHolder ;
	using WriterLayout::mStream ;
	using WriterLayout::mDiffEndian ;
	using WriterLayout::mOverflow ;
	using WriterLayout::mRead ;
	using WriterLayout::mWrite ;

public:
	implicit Writer () = default ;

	void use_overflow (CR<Function<VR<Writer>>> overflow) {
		return WriterHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	Length size () const {
		return WriterHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return WriterHolder::hold (thiz)->length () ;
	}

	StreamShape shape () const {
		return WriterHolder::hold (thiz)->shape () ;
	}

	Bool good () const {
		return WriterHolder::hold (thiz)->good () ;
	}

	void reset () {
		return WriterHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return WriterHolder::hold (thiz)->reset (shape) ;
	}

	void write (CR<Bool> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Bool> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val32> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Val32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val64> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Val64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt32> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Flt32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt64> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Flt64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Byte> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Byte> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Word> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Word> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Char> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Char> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Quad> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Quad> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Stru32> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Stru32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Slice> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<Slice> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<StringLayout> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<StringLayout> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (BOM)> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<typeof (BOM)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (CAT)> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<typeof (CAT)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (GAP)> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<typeof (GAP)> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (EOS)> item) {
		return WriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<Writer> operator<< (CR<typeof (EOS)> item) {
		write (item) ;
		return thiz ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	void write (XR<ARG1> item) {
		item.friend_write (thiz) ;
	}

	template <class ARG1 ,class = REQUIRE<HAS_FRIEND_WRITE<ARG1>>>
	forceinline VR<Writer> operator<< (XR<ARG1> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct ByteWriterHolder implement WriterHolder {
	imports VFat<ByteWriterHolder> hold (VR<WriterLayout> that) ;
	imports CFat<ByteWriterHolder> hold (CR<WriterLayout> that) ;
} ;

struct ByteWriterImplLayout implement Writer {
public:
	implicit ByteWriterImplLayout () noexcept {
		ByteWriterHolder::hold (thiz)->prepare (ByteUnknownBinder ()) ;
	}
} ;

class ByteWriter implement ByteWriterImplLayout {
public:
	implicit ByteWriter () = default ;

	explicit ByteWriter (RR<Ref<RefBuffer<Byte>>> stream) {
		ByteWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}
} ;

struct TextWriterHolder implement WriterHolder {
	imports VFat<TextWriterHolder> hold (VR<WriterLayout> that) ;
	imports CFat<TextWriterHolder> hold (CR<WriterLayout> that) ;
} ;

struct TextWriterImplLayout implement Writer {
public:
	implicit TextWriterImplLayout () noexcept {
		TextWriterHolder::hold (thiz)->prepare (TextUnknownBinder ()) ;
	}
} ;

class TextWriter implement TextWriterImplLayout {
public:
	implicit TextWriter () = default ;

	explicit TextWriter (RR<Ref<RefBuffer<Byte>>> stream) {
		TextWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}
} ;

template <class A>
class StringParse {
public:
	static A make (CR<Slice> text) {
		return make (String<Str> (text)) ;
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
class StringBuild {
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
	virtual void friend_read (VR<Reader> reader) = 0 ;
} ;

template <class A>
class FriendReadingBinder final implement Fat<FriendReading ,A> {
public:
	static VFat<FriendReading> hold (VR<A> that) {
		return VFat<FriendReading> (FriendReadingBinder () ,that) ;
	}

	void friend_read (VR<Reader> reader) const override {
		reader.read (thiz.self) ;
	}
} ;

struct FriendWriting implement Interface {
	virtual void friend_write (VR<Writer> writer) const = 0 ;
} ;

template <class A>
class FriendWritingBinder final implement Fat<FriendWriting ,A> {
public:
	static CFat<FriendWriting> hold (CR<A> that) {
		return CFat<FriendWriting> (FriendWritingBinder () ,that) ;
	}

	void friend_write (VR<Writer> writer) const override {
		writer.write (thiz.self) ;
	}
} ;

struct FormatLayout {
	Slice mFormat ;
	BufferX<FatLayout> mParams ;
	Index mWrite ;
} ;

struct FormatHolder implement Interface {
	imports VFat<FormatHolder> hold (VR<FormatLayout> that) ;
	imports CFat<FormatHolder> hold (CR<FormatLayout> that) ;

	virtual void initialize (CR<Slice> format) = 0 ;
	virtual void friend_write (VR<Writer> writer) const = 0 ;
	virtual void once (CR<Wrapper<FatLayout>> params) const = 0 ;
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

	void friend_write (VR<Writer> writer) const {
		return FormatHolder::hold (thiz)->friend_write (writer) ;
	}

	template <class...ARG1>
	void once (CR<ARG1>...params) const {
		return FormatHolder::hold (thiz)->once (MakeWrapper (keep[TYPE<FatLayout>::expr] (FriendWritingBinder<ARG1>::hold (params))...)) ;
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
	imports CR<Like<UniqueRef<StreamTextProcLayout>>> expr_m () ;
	imports VFat<StreamTextProcHolder> hold (VR<StreamTextProcLayout> that) ;
	imports CFat<StreamTextProcHolder> hold (CR<StreamTextProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void read_keyword (VR<Reader> reader ,VR<String<Stru8>> item) const = 0 ;
	virtual void read_scalar (VR<Reader> reader ,VR<String<Stru8>> item) const = 0 ;
	virtual void read_escape (VR<Reader> reader ,VR<String<Stru8>> item) const = 0 ;
	virtual void write_escape (VR<Writer> writer ,CR<String<Stru8>> item) const = 0 ;
	virtual void read_blank (VR<Reader> reader ,VR<String<Stru8>> item) const = 0 ;
	virtual void read_endline (VR<Reader> reader ,VR<String<Stru8>> item) const = 0 ;
	virtual void write_aligned (VR<Writer> writer ,CR<Val64> number ,CR<Length> align) const = 0 ;
} ;

class StreamTextProc implement Like<UniqueRef<StreamTextProcLayout>> {
public:
	static CR<StreamTextProc> expr_m () {
		return keep[TYPE<StreamTextProc>::expr] (StreamTextProcHolder::expr) ;
	}

	static void read_keyword (VR<Reader> reader ,VR<String<Stru8>> item) {
		return StreamTextProcHolder::hold (expr)->read_keyword (reader ,item) ;
	}

	static void read_scalar (VR<Reader> reader ,VR<String<Stru8>> item) {
		return StreamTextProcHolder::hold (expr)->read_scalar (reader ,item) ;
	}

	static void read_escape (VR<Reader> reader ,VR<String<Stru8>> item) {
		return StreamTextProcHolder::hold (expr)->read_escape (reader ,item) ;
	}

	static void write_escape (VR<Writer> writer ,CR<String<Stru8>> item) {
		return StreamTextProcHolder::hold (expr)->write_escape (writer ,item) ;
	}

	static void read_blank (VR<Reader> reader ,VR<String<Stru8>> item) {
		return StreamTextProcHolder::hold (expr)->read_blank (reader ,item) ;
	}

	static void read_endline (VR<Reader> reader ,VR<String<Stru8>> item) {
		return StreamTextProcHolder::hold (expr)->read_endline (reader ,item) ;
	}

	static void write_aligned (VR<Writer> writer ,CR<Val64> number ,CR<Length> align) {
		return StreamTextProcHolder::hold (expr)->write_aligned (writer ,number ,align) ;
	}
} ;

class ReadKeyword implement Proxy {
protected:
	VR<String<Stru8>> mThat ;

public:
	implicit ReadKeyword () = delete ;

	explicit ReadKeyword (VR<String<Stru8>> that) :mThat (that) {}

	void friend_read (VR<Reader> reader) const {
		return StreamTextProc::read_keyword (reader ,mThat) ;
	}
} ;

class ReadScalar implement Proxy {
protected:
	VR<String<Stru8>> mThat ;

public:
	implicit ReadScalar () = delete ;

	explicit ReadScalar (VR<String<Stru8>> that) :mThat (that) {}

	void friend_read (VR<Reader> reader) const {
		return StreamTextProc::read_scalar (reader ,mThat) ;
	}
} ;

class ReadEscape implement Proxy {
protected:
	VR<String<Stru8>> mThat ;

public:
	implicit ReadEscape () = delete ;

	explicit ReadEscape (VR<String<Stru8>> that) :mThat (that) {}

	void friend_read (VR<Reader> reader) const {
		return StreamTextProc::read_escape (reader ,mThat) ;
	}
} ;

class WriteEscape implement Proxy {
protected:
	CR<String<Stru8>> mThat ;

public:
	implicit WriteEscape () = delete ;

	explicit WriteEscape (CR<String<Stru8>> that) :mThat (that) {}

	void friend_read (VR<Writer> writer) const {
		return StreamTextProc::write_escape (writer ,mThat) ;
	}
} ;

class ReadBlank implement Proxy {
protected:
	VR<String<Stru8>> mThat ;

public:
	implicit ReadBlank () = delete ;

	explicit ReadBlank (VR<String<Stru8>> that) :mThat (that) {}

	void friend_read (VR<Reader> reader) const {
		return StreamTextProc::read_blank (reader ,mThat) ;
	}
} ;

class ReadEndline implement Proxy {
protected:
	VR<String<Stru8>> mThat ;

public:
	implicit ReadEndline () = delete ;

	explicit ReadEndline (VR<String<Stru8>> that) :mThat (that) {}

	void friend_read (VR<Reader> reader) const {
		return StreamTextProc::read_endline (reader ,mThat) ;
	}
} ;

class WriteAligned implement Proxy {
protected:
	Val64 mNumber ;
	Length mAlign ;

public:
	implicit WriteAligned () = delete ;

	explicit WriteAligned (CR<Val64> number ,CR<Length> align) {
		mNumber = number ;
		mAlign = align ;
	}

	void friend_write (VR<Writer> writer) const {
		return StreamTextProc::write_aligned (writer ,mNumber ,mAlign) ;
	}
} ;

struct CommaLayout ;

struct CommaHolder implement Interface {
	imports SharedRef<CommaLayout> create () ;
	imports VFat<CommaHolder> hold (VR<CommaLayout> that) ;
	imports CFat<CommaHolder> hold (CR<CommaLayout> that) ;

	virtual void initialize (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) = 0 ;
	virtual void friend_write (VR<Writer> writer) = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
	virtual void tight () = 0 ;
} ;

class Comma implement Like<SharedRef<CommaLayout>> {
public:
	implicit Comma () = default ;

	explicit Comma (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) {
		mThis = CommaHolder::create () ;
		CommaHolder::hold (thiz)->initialize (indent ,comma ,endline) ;
	}

	void friend_write (VR<Writer> writer) const {
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

	virtual void initialize (CR<String<Str>> format) = 0 ;
	virtual Index search (RR<Ref<String<Str>>> text ,CR<Index> offset) = 0 ;
	virtual Slice match (CR<Index> index) const = 0 ;
} ;

class Regex implement Like<AutoRef<RegexLayout>> {
public:
	implicit Regex () = default ;

	explicit Regex (CR<String<Str>> format) {
		mThis = RegexHolder::create () ;
		RegexHolder::hold (thiz)->initialize (format) ;
	}

	Index search (RR<Ref<String<Str>>> text ,CR<Index> offset) {
		return RegexHolder::hold (thiz)->search (move (text) ,offset) ;
	}

	Slice match (CR<Index> index) const {
		return RegexHolder::hold (thiz)->match (index) ;
	}
} ;
} ;