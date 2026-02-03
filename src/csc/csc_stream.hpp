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
	imports CR<Super<Ref<StreamProcLayout>>> expr_m () ;
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

class StreamProc implement Super<Ref<StreamProcLayout>> {
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

class Reader ;
class Writer ;

struct ReadingHolder implement Interface {
	virtual void friend_read (CR<Reader> reader) = 0 ;
} ;

class Reading implement Proxy {
protected:
	VFat<ReadingHolder> mThat ;

public:
	implicit Reading () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<ReadingHolder ,ARG1>>>
	implicit Reading (CR<VFat<ARG1>> that) :mThat (that) {}

	void friend_read (CR<Reader> reader) const {
		return mThat->friend_read (reader) ;
	}
} ;

struct ReaderHolder implement Interface {
	virtual void initialize (RR<Ref<RefBuffer<Byte>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<CR<Pointer>>> overflow) = 0 ;
	virtual void set_cats (CR<Array<Slice>> cats) = 0 ;
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

class Reader implement Proxy {
protected:
	VFat<ReaderHolder> mThat ;

public:
	implicit Reader () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<ReaderHolder ,ARG1>>>
	implicit Reader (CR<VFat<ARG1>> that) :mThat (that) {}

	void use_overflow (CR<Function<CR<Reader>>> overflow) const {
		return mThat->use_overflow (Pointer::from (overflow)) ;
	}

	void set_cats (CR<Array<Slice>> cats) const {
		return mThat->set_cats (cats) ;
	}

	Length size () const {
		return mThat->size () ;
	}

	Length length () const {
		return mThat->length () ;
	}

	StreamShape shape () const {
		return mThat->shape () ;
	}

	Bool good () const {
		return mThat->good () ;
	}

	void reset () const {
		return mThat->reset () ;
	}

	void reset (CR<StreamShape> shape) const {
		return mThat->reset (shape) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) const {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<Bool> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Bool> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val32> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Val32> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val64> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Val64> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt32> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Flt32> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt64> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Flt64> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Byte> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Byte> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Word> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Word> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Char> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Char> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Quad> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Quad> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<Stru32> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<Stru32> item) const {
		read (item) ;
		return thiz ;
	}

	void read (CR<Slice> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (CR<Slice> item) const {
		read (item) ;
		return thiz ;
	}

	void read (VR<StringLayout> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (VR<StringLayout> item) const {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (BOM)> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (CR<typeof (BOM)> item) const {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (CAT)> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (CR<typeof (CAT)> item) const {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (GAP)> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (CR<typeof (GAP)> item) const {
		read (item) ;
		return thiz ;
	}

	void read (CR<typeof (EOS)> item) const {
		return mThat->read (item) ;
	}

	forceinline CR<Reader> operator>> (CR<typeof (EOS)> item) const {
		read (item) ;
		return thiz ;
	}

	void read (CR<Reading> item) const {
		item.friend_read (thiz) ;
	}

	forceinline CR<Reader> operator>> (CR<Reading> item) const {
		read (item) ;
		return thiz ;
	}
} ;

struct ByteReaderLayout {
	Ref<RefBuffer<Byte>> mStream ;
	Index mRead ;
	Index mWrite ;
	Bool mDiffEndian ;
	Function<CR<Pointer>> mOverflow ;
	Array<Slice> mCats ;
	Index mCatIndex ;
} ;

struct ByteReaderHolder implement ReaderHolder {
	imports VFat<ByteReaderHolder> hold (VR<ByteReaderLayout> that) ;
	imports CFat<ByteReaderHolder> hold (CR<ByteReaderLayout> that) ;
} ;

class ByteReader implement ByteReaderLayout {
public:
	implicit ByteReader () = default ;

	explicit ByteReader (RR<Ref<RefBuffer<Byte>>> stream) {
		ByteReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}

	implicit operator Reader () leftvalue {
		return ByteReaderHolder::hold (thiz) ;
	}

	void use_overflow (CR<Function<VR<ByteReader>>> overflow) {
		return ByteReaderHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	void set_cats (CR<Array<Slice>> cats) {
		return ByteReaderHolder::hold (thiz)->set_cats (cats) ;
	}

	Length size () const {
		return ByteReaderHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return ByteReaderHolder::hold (thiz)->length () ;
	}

	StreamShape shape () const {
		return ByteReaderHolder::hold (thiz)->shape () ;
	}

	Bool good () const {
		return ByteReaderHolder::hold (thiz)->good () ;
	}

	void reset () {
		return ByteReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return ByteReaderHolder::hold (thiz)->reset (shape) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<Bool> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Bool> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Val32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val64> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Val64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Flt32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt64> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Flt64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Byte> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Byte> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Word> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Word> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Char> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Char> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Quad> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Quad> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Stru32> item) {
		return ByteReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<ByteReader> operator>> (VR<Stru32> item) {
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

	void read (CR<Reading> item) {
		item.friend_read (thiz) ;
	}

	forceinline VR<ByteReader> operator>> (CR<Reading> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct TextReaderLayout {
	Ref<RefBuffer<Byte>> mStream ;
	Index mRead ;
	Index mWrite ;
	Bool mDiffEndian ;
	Function<CR<Pointer>> mOverflow ;
	Array<Slice> mCats ;
	Index mCatIndex ;
} ;

struct TextReaderHolder implement ReaderHolder {
	imports VFat<TextReaderHolder> hold (VR<TextReaderLayout> that) ;
	imports CFat<TextReaderHolder> hold (CR<TextReaderLayout> that) ;
} ;

class TextReader implement TextReaderLayout {
public:
	implicit TextReader () = default ;

	explicit TextReader (RR<Ref<RefBuffer<Byte>>> stream) {
		TextReaderHolder::hold (thiz)->initialize (move (stream)) ;
	}

	implicit operator Reader () leftvalue {
		return TextReaderHolder::hold (thiz) ;
	}

	void use_overflow (CR<Function<VR<TextReader>>> overflow) {
		return TextReaderHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	void set_cats (CR<Array<Slice>> cats) {
		return TextReaderHolder::hold (thiz)->set_cats (cats) ;
	}

	Length size () const {
		return TextReaderHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return TextReaderHolder::hold (thiz)->length () ;
	}

	StreamShape shape () const {
		return TextReaderHolder::hold (thiz)->shape () ;
	}

	Bool good () const {
		return TextReaderHolder::hold (thiz)->good () ;
	}

	void reset () {
		return TextReaderHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return TextReaderHolder::hold (thiz)->reset (shape) ;
	}

	template <class ARG1>
	ARG1 poll (TYPE<ARG1>) {
		ARG1 ret ;
		read (ret) ;
		return move (ret) ;
	}

	void read (VR<Bool> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Bool> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Val32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Val64> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Val64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Flt32> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Flt64> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Flt64> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Byte> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Byte> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Word> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Word> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Char> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Char> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Quad> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Quad> item) {
		read (item) ;
		return thiz ;
	}

	void read (VR<Stru32> item) {
		return TextReaderHolder::hold (thiz)->read (item) ;
	}

	forceinline VR<TextReader> operator>> (VR<Stru32> item) {
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

	void read (CR<Reading> item) {
		item.friend_read (thiz) ;
	}

	forceinline VR<TextReader> operator>> (CR<Reading> item) {
		read (item) ;
		return thiz ;
	}
} ;

struct WritingHolder implement Interface {
	virtual void friend_write (CR<Writer> writer) const = 0 ;
} ;

class Writing implement Proxy {
protected:
	CFat<WritingHolder> mThat ;

public:
	implicit Writing () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<WritingHolder ,ARG1>>>
	implicit Writing (CR<CFat<ARG1>> that) :mThat (that) {}

	void friend_write (CR<Writer> writer) const {
		return mThat->friend_write (writer) ;
	}
} ;

struct WriterHolder implement Interface {
	virtual void initialize (RR<Ref<RefBuffer<Byte>>> stream) = 0 ;
	virtual void use_overflow (CR<Function<CR<Pointer>>> overflow) = 0 ;
	virtual void set_cats (CR<Array<Slice>> cats) = 0 ;
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

class Writer implement Proxy {
protected:
	VFat<WriterHolder> mThat ;

public:
	implicit Writer () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<WriterHolder ,ARG1>>>
	implicit Writer (CR<VFat<ARG1>> that) :mThat (that) {}

	void use_overflow (CR<Function<CR<Writer>>> overflow) const {
		return mThat->use_overflow (Pointer::from (overflow)) ;
	}

	void set_cats (CR<Array<Slice>> cats) const {
		return mThat->set_cats (cats) ;
	}

	Length size () const {
		return mThat->size () ;
	}

	Length length () const {
		return mThat->length () ;
	}

	StreamShape shape () const {
		return mThat->shape () ;
	}

	Bool good () const {
		return mThat->good () ;
	}

	void reset () const {
		return mThat->reset () ;
	}

	void reset (CR<StreamShape> shape) const {
		return mThat->reset (shape) ;
	}

	void write (CR<Bool> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Bool> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val32> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Val32> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val64> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Val64> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt32> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Flt32> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt64> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Flt64> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Byte> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Byte> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Word> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Word> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Char> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Char> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Quad> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Quad> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Stru32> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Stru32> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Slice> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<Slice> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<StringLayout> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<StringLayout> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (BOM)> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<typeof (BOM)> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (CAT)> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<typeof (CAT)> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (GAP)> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<typeof (GAP)> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<typeof (EOS)> item) const {
		return mThat->write (item) ;
	}

	forceinline CR<Writer> operator<< (CR<typeof (EOS)> item) const {
		write (item) ;
		return thiz ;
	}

	void write (CR<Writing> item) const {
		item.friend_write (thiz) ;
	}

	forceinline CR<Writer> operator<< (CR<Writing> item) const {
		write (item) ;
		return thiz ;
	}
} ;

struct ByteWriterLayout {
	Ref<RefBuffer<Byte>> mStream ;
	Index mRead ;
	Index mWrite ;
	Bool mDiffEndian ;
	Function<CR<Pointer>> mOverflow ;
	Array<Slice> mCats ;
	Index mCatIndex ;
} ;

struct ByteWriterHolder implement WriterHolder {
	imports VFat<ByteWriterHolder> hold (VR<ByteWriterLayout> that) ;
	imports CFat<ByteWriterHolder> hold (CR<ByteWriterLayout> that) ;
} ;

class ByteWriter implement ByteWriterLayout {
public:
	implicit ByteWriter () = default ;

	explicit ByteWriter (RR<Ref<RefBuffer<Byte>>> stream) {
		ByteWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}

	implicit operator Writer () leftvalue {
		return ByteWriterHolder::hold (thiz) ;
	}

	void use_overflow (CR<Function<VR<ByteWriter>>> overflow) {
		return ByteWriterHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	void set_cats (CR<Array<Slice>> cats) {
		return ByteWriterHolder::hold (thiz)->set_cats (cats) ;
	}

	Length size () const {
		return ByteWriterHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return ByteWriterHolder::hold (thiz)->length () ;
	}

	StreamShape shape () const {
		return ByteWriterHolder::hold (thiz)->shape () ;
	}

	Bool good () const {
		return ByteWriterHolder::hold (thiz)->good () ;
	}

	void reset () {
		return ByteWriterHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return ByteWriterHolder::hold (thiz)->reset (shape) ;
	}

	void write (CR<Bool> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Bool> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Val32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val64> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Val64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Flt32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt64> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Flt64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Byte> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Byte> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Word> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Word> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Char> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Char> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Quad> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Quad> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Stru32> item) {
		return ByteWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Stru32> item) {
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

	void write (CR<Writing> item) {
		item.friend_write (thiz) ;
	}

	forceinline VR<ByteWriter> operator<< (CR<Writing> item) {
		write (item) ;
		return thiz ;
	}
} ;

struct TextWriterLayout {
	Ref<RefBuffer<Byte>> mStream ;
	Index mRead ;
	Index mWrite ;
	Bool mDiffEndian ;
	Function<CR<Pointer>> mOverflow ;
	Array<Slice> mCats ;
	Index mCatIndex ;
} ;

struct TextWriterHolder implement WriterHolder {
	imports VFat<TextWriterHolder> hold (VR<TextWriterLayout> that) ;
	imports CFat<TextWriterHolder> hold (CR<TextWriterLayout> that) ;
} ;

class TextWriter implement TextWriterLayout {
public:
	implicit TextWriter () = default ;

	explicit TextWriter (RR<Ref<RefBuffer<Byte>>> stream) {
		TextWriterHolder::hold (thiz)->initialize (move (stream)) ;
	}

	implicit operator Writer () leftvalue {
		return TextWriterHolder::hold (thiz) ;
	}

	void use_overflow (CR<Function<VR<TextWriter>>> overflow) {
		return TextWriterHolder::hold (thiz)->use_overflow (Pointer::from (overflow)) ;
	}

	void set_cats (CR<Array<Slice>> cats) {
		return TextWriterHolder::hold (thiz)->set_cats (cats) ;
	}

	Length size () const {
		return TextWriterHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return TextWriterHolder::hold (thiz)->length () ;
	}

	StreamShape shape () const {
		return TextWriterHolder::hold (thiz)->shape () ;
	}

	Bool good () const {
		return TextWriterHolder::hold (thiz)->good () ;
	}

	void reset () {
		return TextWriterHolder::hold (thiz)->reset () ;
	}

	void reset (CR<StreamShape> shape) {
		return TextWriterHolder::hold (thiz)->reset (shape) ;
	}

	void write (CR<Bool> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Bool> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Val32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Val64> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Val64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Flt32> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Flt64> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Flt64> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Byte> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Byte> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Word> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Word> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Char> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Char> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Quad> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Quad> item) {
		write (item) ;
		return thiz ;
	}

	void write (CR<Stru32> item) {
		return TextWriterHolder::hold (thiz)->write (item) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Stru32> item) {
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

	void write (CR<Writing> item) {
		item.friend_write (thiz) ;
	}

	forceinline VR<TextWriter> operator<< (CR<Writing> item) {
		write (item) ;
		return thiz ;
	}
} ;

template <class A>
class StringParse implement Proxy {
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

template <class A>
class PrintWritingBinder final implement Fat<WritingHolder ,A> {
public:
	void friend_write (CR<Writer> writer) const override {
		return writer.write (thiz.self) ;
	}
} ;

template <class ARG1>
inline FatLayout PrintWriting (CR<ARG1> param) {
	return CFat<WritingHolder> (PrintWritingBinder<ARG1> () ,param) ;
}

struct FormatLayout {
	Slice mFormat ;
	BufferX<FatLayout> mParams ;
	Index mWrite ;
} ;

struct FormatHolder implement Interface {
	imports VFat<FormatHolder> hold (VR<FormatLayout> that) ;
	imports CFat<FormatHolder> hold (CR<FormatLayout> that) ;

	virtual void initialize (CR<Slice> format) = 0 ;
	virtual void friend_write (CR<Writer> writer) const = 0 ;
	virtual void once (CR<Wrapper<FatLayout>> params) const = 0 ;
} ;

class FormatWritingBinder final implement Fat<WritingHolder ,FormatLayout> {
public:
	void friend_write (CR<Writer> writer) const override {
		return FormatHolder::hold (self)->friend_write (writer) ;
	}
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

	implicit operator Writing () const leftvalue {
		return CFat<WritingHolder> (FormatWritingBinder () ,thiz) ;
	}

	void friend_write (CR<Writer> writer) const {
		return FormatHolder::hold (thiz)->friend_write (writer) ;
	}

	template <class...ARG1>
	void once (CR<ARG1>...params) const {
		return FormatHolder::hold (thiz)->once (MakeWrapper (PrintWriting (params)...)) ;
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
	imports CR<Super<Ref<StreamTextProcLayout>>> expr_m () ;
	imports VFat<StreamTextProcHolder> hold (VR<StreamTextProcLayout> that) ;
	imports CFat<StreamTextProcHolder> hold (CR<StreamTextProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void read_keyword (CR<Reader> reader ,VR<String<Stru>> item) const = 0 ;
	virtual void read_scalar (CR<Reader> reader ,VR<String<Stru>> item) const = 0 ;
	virtual void read_escape (CR<Reader> reader ,VR<String<Stru>> item) const = 0 ;
	virtual void write_escape (CR<Writer> writer ,CR<String<Stru>> item) const = 0 ;
	virtual void read_blank (CR<Reader> reader ,VR<String<Stru>> item) const = 0 ;
	virtual void read_endline (CR<Reader> reader ,VR<String<Stru>> item) const = 0 ;
	virtual void write_aligned (CR<Writer> writer ,CR<Val64> number ,CR<Length> align) const = 0 ;
} ;

class StreamTextProc implement Super<Ref<StreamTextProcLayout>> {
public:
	static CR<StreamTextProc> expr_m () {
		return keep[TYPE<StreamTextProc>::expr] (StreamTextProcHolder::expr) ;
	}

	static void read_keyword (CR<Reader> reader ,VR<String<Stru>> item) {
		return StreamTextProcHolder::hold (expr)->read_keyword (reader ,item) ;
	}

	static void read_scalar (CR<Reader> reader ,VR<String<Stru>> item) {
		return StreamTextProcHolder::hold (expr)->read_scalar (reader ,item) ;
	}

	static void read_escape (CR<Reader> reader ,VR<String<Stru>> item) {
		return StreamTextProcHolder::hold (expr)->read_escape (reader ,item) ;
	}

	static void write_escape (CR<Writer> writer ,CR<String<Stru>> item) {
		return StreamTextProcHolder::hold (expr)->write_escape (writer ,item) ;
	}

	static void read_blank (CR<Reader> reader ,VR<String<Stru>> item) {
		return StreamTextProcHolder::hold (expr)->read_blank (reader ,item) ;
	}

	static void read_endline (CR<Reader> reader ,VR<String<Stru>> item) {
		return StreamTextProcHolder::hold (expr)->read_endline (reader ,item) ;
	}

	static void write_aligned (CR<Writer> writer ,CR<Val64> number ,CR<Length> align) {
		return StreamTextProcHolder::hold (expr)->write_aligned (writer ,number ,align) ;
	}
} ;

class ReadKeywordReadingBinder final implement Fat<ReadingHolder ,String<Stru>> {
public:
	void friend_read (CR<Reader> reader) override {
		return StreamTextProc::read_keyword (reader ,self) ;
	}
} ;

inline VFat<ReadingHolder> ReadKeyword (VR<String<Stru>> that) {
	return VFat<ReadingHolder> (ReadKeywordReadingBinder () ,that) ;
}

class ReadScalarReadingBinder final implement Fat<ReadingHolder ,String<Stru>> {
public:
	void friend_read (CR<Reader> reader) override {
		return StreamTextProc::read_scalar (reader ,self) ;
	}
} ;

inline VFat<ReadingHolder> ReadScalar (VR<String<Stru>> that) {
	return VFat<ReadingHolder> (ReadScalarReadingBinder () ,that) ;
}

class ReadEscapeReadingBinder final implement Fat<ReadingHolder ,String<Stru>> {
public:
	void friend_read (CR<Reader> reader) override {
		return StreamTextProc::read_escape (reader ,self) ;
	}
} ;

inline VFat<ReadingHolder> ReadEscape (VR<String<Stru>> that) {
	return VFat<ReadingHolder> (ReadEscapeReadingBinder () ,that) ;
}

class WriteEscapeWritingBinder final implement Fat<WritingHolder ,String<Stru>> {
public:
	void friend_write (CR<Writer> writer) const override {
		return StreamTextProc::write_escape (writer ,self) ;
	}
} ;

inline CFat<WritingHolder> WriteEscape (CR<String<Stru>> that) {
	return CFat<WritingHolder> (WriteEscapeWritingBinder () ,that) ;
}

class ReadBlankReadingBinder final implement Fat<ReadingHolder ,String<Stru>> {
public:
	void friend_read (CR<Reader> reader) override {
		return StreamTextProc::read_blank (reader ,self) ;
	}
} ;

inline VFat<ReadingHolder> ReadBlank (VR<String<Stru>> that) {
	return VFat<ReadingHolder> (ReadBlankReadingBinder () ,that) ;
}

class ReadEndlineReadingBinder final implement Fat<ReadingHolder ,String<Stru>> {
public:
	void friend_read (CR<Reader> reader) override {
		return StreamTextProc::read_endline (reader ,self) ;
	}
} ;

inline VFat<ReadingHolder> ReadEndline (VR<String<Stru>> that) {
	return VFat<ReadingHolder> (ReadEndlineReadingBinder () ,that) ;
}

class WriteAlignedWritingBinder final implement Fat<WritingHolder ,Tuple<Val64 ,Length>> {
public:
	void friend_write (CR<Writer> writer) const override {
		return StreamTextProc::write_aligned (writer ,self.m1st ,self.m2nd) ;
	}
} ;

inline CFat<WritingHolder> WriteAligned (CR<Tuple<Val64 ,Length>> that) {
	return CFat<WritingHolder> (WriteAlignedWritingBinder () ,that) ;
}

struct CommaLayout ;

struct CommaHolder implement Interface {
	imports Ref<CommaLayout> create () ;
	imports VFat<CommaHolder> hold (VR<CommaLayout> that) ;
	imports CFat<CommaHolder> hold (CR<CommaLayout> that) ;

	virtual void initialize (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) = 0 ;
	virtual void friend_write (CR<Writer> writer) = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
	virtual void tight () = 0 ;
} ;

class CommaWritingBinder final implement Fat<WritingHolder ,Super<Ref<CommaLayout>>> {
public:
	void friend_write (CR<Writer> writer) const override {
		return CommaHolder::hold (self)->friend_write (writer) ;
	}
} ;

class Comma implement Super<Ref<CommaLayout>> {
public:
	implicit Comma () = default ;

	explicit Comma (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) {
		mThis = CommaHolder::create () ;
		CommaHolder::hold (thiz)->initialize (indent ,comma ,endline) ;
	}

	implicit operator Writing () const leftvalue {
		return CFat<WritingHolder> (CommaWritingBinder () ,thiz) ;
	}

	void friend_write (CR<Writer> writer) const {
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
	imports Ref<RegexLayout> create () ;
	imports VFat<RegexHolder> hold (VR<RegexLayout> that) ;
	imports CFat<RegexHolder> hold (CR<RegexLayout> that) ;

	virtual void initialize (CR<String<Str>> format) = 0 ;
	virtual Index search (RR<Ref<String<Str>>> text ,CR<Index> offset) = 0 ;
	virtual Slice match (CR<Index> index) const = 0 ;
} ;

class Regex implement Super<Ref<RegexLayout>> {
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