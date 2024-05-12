#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_stream.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <regex>
#include "csc_begin.h"

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4996)
#endif

namespace CSC {
struct StreamToolPureLayout {
	HashSet<STRU32> mSpaceSet ;
	ArrayList<Tuple<STRU32 ,STRU32>> mEscapeList ;
	HashSet<STRU32> mEscapeWordSet ;
	HashSet<STRU32> mEscapeCtrlSet ;
} ;

struct GapType {
	enum {
		Space ,
		Endline ,
		ETC
	} ;
} ;

class StreamProcImplement implement Fat<StreamToolHolder ,StreamToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<StreamToolPureLayout>::make () ;
		fake.mThis->mSpaceSet.add (STRU32 (' ') ,GapType::Space) ;
		fake.mThis->mSpaceSet.add (STRU32 ('\t') ,GapType::Space) ;
		fake.mThis->mSpaceSet.add (STRU32 ('\b') ,GapType::Space) ;
		fake.mThis->mSpaceSet.add (STRU32 ('\r') ,GapType::Endline) ;
		fake.mThis->mSpaceSet.add (STRU32 ('\n') ,GapType::Endline) ;
		fake.mThis->mSpaceSet.add (STRU32 ('\f') ,GapType::Endline) ;
		escape_list_add (STRU32 ('\\') ,STRU32 ('\\')) ;
		escape_list_add (STRU32 ('\\') ,STRU32 ('\\')) ;
		escape_list_add (STRU32 ('/') ,STRU32 ('/')) ;
		escape_list_add (STRU32 ('t') ,STRU32 ('\t')) ;
		escape_list_add (STRU32 ('b') ,STRU32 ('\b')) ;
		escape_list_add (STRU32 ('r') ,STRU32 ('\r')) ;
		escape_list_add (STRU32 ('n') ,STRU32 ('\n')) ;
		escape_list_add (STRU32 ('f') ,STRU32 ('\f')) ;
		escape_list_add (STRU32 ('\'') ,STRU32 ('\'')) ;
		escape_list_add (STRU32 ('\"') ,STRU32 ('\"')) ;
		escape_list_add (STRU32 ('u') ,STRU32 (7)) ;
	}

	void escape_list_add (CREF<STRU32> word ,CREF<STRU32> ctrl) {
		INDEX ix = fake.mThis->mEscapeList.insert () ;
		fake.mThis->mEscapeList[ix].m1st = word ;
		fake.mThis->mEscapeList[ix].m2nd = ctrl ;
		fake.mThis->mEscapeWordSet.add (word ,ix) ;
		fake.mThis->mEscapeCtrlSet.add (ctrl ,ix) ;
	}

	BOOL big_endian () const override {
		return memorize ([&] () {
			const auto r1x = WORD (0X00FF) ;
			const auto r2x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (r1x) ;
			return r2x[0] == BYTE (0X00) ;
		}) ;
	}

	BOOL is_gap (CREF<STRU32> str) const override {
		return fake.mThis->mSpaceSet.contain (str) ;
	}

	BOOL is_gap_space (CREF<STRU32> str) const override {
		return fake.mThis->mSpaceSet.map (str) == GapType::Space ;
	}

	BOOL is_gap_endline (CREF<STRU32> str) const override {
		return fake.mThis->mSpaceSet.map (str) == GapType::Endline ;
	}

	BOOL is_word (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('z'))
				return TRUE ;
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('Z'))
				return TRUE ;
		if (str == STRU32 ('_'))
			return TRUE ;
		return FALSE ;
	}

	STRU32 word_lower (CREF<STRU32> str) const override {
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('Z'))
				return STRU32 (INDEX (str) - INDEX ('A') + INDEX ('a')) ;
		return str ;
	}

	STRU32 word_upper (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('z'))
				return STRU32 (INDEX (str) - INDEX ('a') + INDEX ('A')) ;
		return str ;
	}

	BOOL is_number (CREF<STRU32> str) const override {
		if (str >= STRU32 ('0'))
			if (str <= STRU32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_hex_number (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('f'))
				return TRUE ;
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('F'))
				return TRUE ;
		return FALSE ;
	}

	INDEX hex_from_str (CREF<STRU32> str) const override {
		if (is_number (str))
			return INDEX (str) - INDEX ('0') ;
		if (is_hex_number (str))
			return INDEX (word_upper (str)) - INDEX ('A') + 10 ;
		assume (FALSE) ;
		return INDEX () ;
	}

	STRU32 str_from_hex (CREF<INDEX> hex) const override {
		if (inline_between (hex ,0 ,10))
			return STRU32 (INDEX ('0') + hex) ;
		if (inline_between (hex ,10 ,16))
			return STRU32 (INDEX ('A') + hex - 10) ;
		assume (FALSE) ;
		return STRU32 () ;
	}

	BOOL is_control (CREF<STRU32> str) const override {
		if (str == STRU32 (127))
			return TRUE ;
		if (str >= STRU32 (32))
			return FALSE ;
		if (fake.mThis->mSpaceSet.contain (str))
			return FALSE ;
		return TRUE ;
	}
} ;

exports VFat<StreamToolHolder> StreamToolHolder::create (VREF<StreamToolLayout> that) {
	return VFat<StreamToolHolder> (StreamProcImplement () ,Pointer::from (that)) ;
}

exports CFat<StreamToolHolder> StreamToolHolder::create (CREF<StreamToolLayout> that) {
	return CFat<StreamToolHolder> (StreamProcImplement () ,Pointer::from (that)) ;
}

class ByteReaderImplement implement Fat<ByteReaderHolder ,ByteReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	void reset () override {
		fake.mRead = 0 ;
		fake.mWrite = fake.mStream->size () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
		fake.mRead = read_ ;
		fake.mWrite = write_ ;
	}

	void backup () override {
		fake.mBackupRead = fake.mRead ;
		fake.mBackupWrite = fake.mWrite ;
	}

	void recover () override {
		fake.mRead = fake.mBackupRead ;
		fake.mWrite = fake.mBackupWrite ;
	}

	void read (VREF<BOOL> item) override {
		auto rax = BYTE_BASE<BOOL> () ;
		read (rax) ;
		item = bitwise[TYPE<BOOL>::expr] (rax) ;
	}

	void read (VREF<VAL32> item) override {
		auto rax = BYTE_BASE<VAL32> () ;
		read (rax) ;
		item = bitwise[TYPE<VAL32>::expr] (rax) ;
	}

	void read (VREF<VAL64> item) override {
		auto rax = BYTE_BASE<VAL64> () ;
		read (rax) ;
		item = bitwise[TYPE<VAL64>::expr] (rax) ;
	}

	void read (VREF<FLT32> item) override {
		auto rax = BYTE_BASE<FLT32> () ;
		read (rax) ;
		item = bitwise[TYPE<FLT32>::expr] (rax) ;
	}

	void read (VREF<FLT64> item) override {
		auto rax = BYTE_BASE<FLT64> () ;
		read (rax) ;
		item = bitwise[TYPE<FLT64>::expr] (rax) ;
	}

	void read (VREF<BYTE> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mRead >= fake.mWrite)
				discard ;
			item = fake.mStream.self[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			item = BYTE (0X00) ;
		}
	}

	void read (VREF<WORD> item) override {
		auto rax = BoxBuffer<BYTE ,SIZE_OF<WORD>> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (StreamTool::big_endian ())
				discard ;
			for (auto &&i : iter (0 ,rax.size ()))
				read (rax[i]) ;
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,rax.size ())) {
				INDEX ix = rax.size () - 1 - i ;
				read (rax[ix]) ;
			}
		}
		item = bitwise[TYPE<WORD>::expr] (rax) ;
	}

	void read (VREF<CHAR> item) override {
		auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (StreamTool::big_endian ())
				discard ;
			for (auto &&i : iter (0 ,rax.size ()))
				read (rax[i]) ;
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,rax.size ())) {
				INDEX ix = rax.size () - 1 - i ;
				read (rax[ix]) ;
			}
		}
		item = bitwise[TYPE<CHAR>::expr] (rax) ;
	}

	void read (VREF<QUAD> item) override {
		auto rax = BoxBuffer<BYTE ,SIZE_OF<QUAD>> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (StreamTool::big_endian ())
				discard ;
			for (auto &&i : iter (0 ,rax.size ()))
				read (rax[i]) ;
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,rax.size ())) {
				INDEX ix = rax.size () - 1 - i ;
				read (rax[ix]) ;
			}
		}
		item = bitwise[TYPE<QUAD>::expr] (rax) ;
	}

	void read (CREF<Slice<STRA>> item) override {
		auto rax = BYTE_BASE<STRA> () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			assume (STRA (rax) == item[i]) ;
		}
	}

	void read (CREF<Slice<STRW>> item) override {
		auto rax = BYTE_BASE<STRW> () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			assume (STRW (rax) == item[i]) ;
		}
	}

	void read (CREF<Slice<STRU8>> item) override {
		auto rax = BYTE_BASE<STRU8> () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			assume (STRU8 (rax) == item[i]) ;
		}
	}

	void read (CREF<Slice<STRU16>> item) override {
		auto rax = BYTE_BASE<STRU16> () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			assume (STRU16 (rax) == item[i]) ;
		}
	}

	void read (CREF<Slice<STRU32>> item) override {
		auto rax = BYTE_BASE<STRU32> () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			assume (STRU32 (rax) == item[i]) ;
		}
	}

	void read (VREF<String> item) override {
		auto rax = VAL32 () ;
		auto rbx = VAL32 () ;
		read (rax) ;
		read (rbx) ;
		if ifdo (TRUE) {
			if (rbx == SIZE_OF<STRU8>::expr)
				discard ;
			if (rbx == SIZE_OF<STRU16>::expr)
				discard ;
			if (rbx == SIZE_OF<STRU32>::expr)
				discard ;
			assume (FALSE) ;
		}
		item = String (rax ,rbx) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (rbx != SIZE_OF<STRU8>::expr)
				discard ;
			for (auto &&i : iter (0 ,rax)) {
				auto &&tmp = keep[TYPE<BYTE_BASE<STRU8>>::expr] (item.at (i)) ;
				read (tmp) ;
			}
		}
		if ifdo (act) {
			if (rbx != SIZE_OF<STRU16>::expr)
				discard ;
			for (auto &&i : iter (0 ,rax)) {
				auto &&tmp = keep[TYPE<BYTE_BASE<STRU16>>::expr] (item.at (i)) ;
				read (tmp) ;
			}
		}
		if ifdo (act) {
			if (rbx != SIZE_OF<STRU32>::expr)
				discard ;
			for (auto &&i : iter (0 ,rax)) {
				auto &&tmp = keep[TYPE<BYTE_BASE<STRU32>>::expr] (item.at (i)) ;
				read (tmp) ;
			}
		}
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (GAP)>) override {
		auto rax = BYTE () ;
		read (rax) ;
		assume (rax == BYTE (0XCC)) ;
		read (rax) ;
		assume (rax == BYTE (0XCC)) ;
	}

	void read (CREF<typeof (EOS)>) override {
		assume (fake.mRead >= fake.mWrite) ;
	}
} ;

exports VFat<ByteReaderHolder> ByteReaderHolder::create (VREF<ByteReaderLayout> that) {
	return VFat<ByteReaderHolder> (ByteReaderImplement () ,Pointer::from (that)) ;
}

exports CFat<ByteReaderHolder> ByteReaderHolder::create (CREF<ByteReaderLayout> that) {
	return CFat<ByteReaderHolder> (ByteReaderImplement () ,Pointer::from (that)) ;
}

class ByteWriterImplement implement Fat<ByteWriterHolder ,ByteWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	void reset () override {
		fake.mRead = fake.mStream->size () ;
		fake.mWrite = 0 ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
		fake.mRead = read_ ;
		fake.mWrite = write_ ;
	}

	void backup () override {
		fake.mBackupRead = fake.mRead ;
		fake.mBackupWrite = fake.mWrite ;
	}

	void recover () override {
		fake.mRead = fake.mBackupRead ;
		fake.mWrite = fake.mBackupWrite ;
	}

	void write (CREF<BOOL> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<BOOL>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<VAL32> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<VAL32>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<VAL64> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<VAL64>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT32> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<FLT32>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT64> item) override {
		const auto r1x = bitwise[TYPE<BYTE_BASE<FLT64>>::expr] (item) ;
		write (r1x) ;
	}

	void write (CREF<BYTE> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			fake.mStream.self[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<WORD> item) override {
		const auto r1x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (item) ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (StreamTool::big_endian ())
				discard ;
			for (auto &&i : iter (0 ,r1x.size ()))
				write (r1x[i]) ;
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,r1x.size ())) {
				INDEX ix = r1x.size () - 1 - i ;
				write (r1x[ix]) ;
			}
		}
	}

	void write (CREF<CHAR> item) override {
		const auto r1x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<CHAR>>>::expr] (item) ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (StreamTool::big_endian ())
				discard ;
			for (auto &&i : iter (0 ,r1x.size ()))
				write (r1x[i]) ;
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,r1x.size ())) {
				INDEX ix = r1x.size () - 1 - i ;
				write (r1x[ix]) ;
			}
		}
	}

	void write (CREF<QUAD> item) override {
		const auto r1x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<QUAD>>>::expr] (item) ;
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (StreamTool::big_endian ())
				discard ;
			for (auto &&i : iter (0 ,r1x.size ()))
				write (r1x[i]) ;
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,r1x.size ())) {
				INDEX ix = r1x.size () - 1 - i ;
				write (r1x[ix]) ;
			}
		}
	}

	void write (CREF<Slice<STRA>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRA>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRW>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRW>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRU8>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRU8>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRU16>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRU16>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRU32>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRU32>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<String> item) override {
		const auto r1x = VAL32 (item.length ()) ;
		const auto r2x = VAL32 (item.step ()) ;
		write (r1x) ;
		write (r2x) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != SIZE_OF<STRU8>::expr)
				discard ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r3x = bitwise[TYPE<BYTE_BASE<STRU8>>::expr] (item.at (i)) ;
				write (r3x) ;
			}
		}
		if ifdo (act) {
			if (r2x != SIZE_OF<STRU16>::expr)
				discard ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r4x = keep[TYPE<BYTE_BASE<STRU16>>::expr] (item.at (i)) ;
				write (r4x) ;
			}
		}
		if ifdo (act) {
			if (r2x != SIZE_OF<STRU32>::expr)
				discard ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r5x = keep[TYPE<BYTE_BASE<STRU32>>::expr] (item.at (i)) ;
				write (r5x) ;
			}
		}
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (GAP)>) override {
		write (BYTE (0XCC)) ;
		write (BYTE (0XCC)) ;
	}

	void write (CREF<typeof (EOS)>) override {
		while (TRUE) {
			if (fake.mWrite >= fake.mRead)
				break ;
			write (BYTE (0X00)) ;
		}
	}
} ;

exports VFat<ByteWriterHolder> ByteWriterHolder::create (VREF<ByteWriterLayout> that) {
	return VFat<ByteWriterHolder> (ByteWriterImplement () ,Pointer::from (that)) ;
}

exports CFat<ByteWriterHolder> ByteWriterHolder::create (CREF<ByteWriterLayout> that) {
	return CFat<ByteWriterHolder> (ByteWriterImplement () ,Pointer::from (that)) ;
}

class TextReaderImplement implement Fat<TextReaderHolder ,TextReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	void reset () override {
		fake.mRead = 0 ;
		fake.mWrite = fake.mStream->size () ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
		fake.mRead = read_ ;
		fake.mWrite = write_ ;
	}

	void backup () override {
		fake.mBackupRead = fake.mRead ;
		fake.mBackupWrite = fake.mWrite ;
	}

	void recover () override {
		fake.mRead = fake.mBackupRead ;
		fake.mWrite = fake.mBackupWrite ;
	}

	void read (VREF<BOOL> item) override {
		unimplemented () ;
	}

	void read (VREF<VAL32> item) override {
		unimplemented () ;
	}

	void read (VREF<VAL64> item) override {
		unimplemented () ;
	}

	void read (VREF<FLT32> item) override {
		unimplemented () ;
	}

	void read (VREF<FLT64> item) override {
		unimplemented () ;
	}

	void read (VREF<BYTE> item) override {
		unimplemented () ;
	}

	void read (VREF<WORD> item) override {
		unimplemented () ;
	}

	void read (VREF<CHAR> item) override {
		unimplemented () ;
	}

	void read (VREF<QUAD> item) override {
		unimplemented () ;
	}

	void read (CREF<Slice<STRA>> item) override {
		unimplemented () ;
	}

	void read (CREF<Slice<STRW>> item) override {
		unimplemented () ;
	}

	void read (CREF<Slice<STRU8>> item) override {
		unimplemented () ;
	}

	void read (CREF<Slice<STRU16>> item) override {
		unimplemented () ;
	}

	void read (CREF<Slice<STRU32>> item) override {
		unimplemented () ;
	}

	void read (VREF<String> item) override {
		unimplemented () ;
	}

	void read (CREF<typeof (CLS)>) override {
		unimplemented () ;
	}

	void read (CREF<typeof (BOM)>) override {
		unimplemented () ;
	}

	void read (CREF<typeof (GAP)>) override {
		unimplemented () ;
	}

	void read (CREF<typeof (EOS)>) override {
		unimplemented () ;
	}
} ;

exports VFat<TextReaderHolder> TextReaderHolder::create (VREF<TextReaderLayout> that) {
	return VFat<TextReaderHolder> (TextReaderImplement () ,Pointer::from (that)) ;
}

exports CFat<TextReaderHolder> TextReaderHolder::create (CREF<TextReaderLayout> that) {
	return CFat<TextReaderHolder> (TextReaderImplement () ,Pointer::from (that)) ;
}

class TextWriterImplement implement Fat<TextWriterHolder ,TextWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	void reset () override {
		fake.mRead = fake.mStream->size () ;
		fake.mWrite = 0 ;
	}

	void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
		fake.mRead = read_ ;
		fake.mWrite = write_ ;
	}

	void backup () override {
		fake.mBackupRead = fake.mRead ;
		fake.mBackupWrite = fake.mWrite ;
	}

	void recover () override {
		fake.mRead = fake.mBackupRead ;
		fake.mWrite = fake.mBackupWrite ;
	}

	void write (CREF<BOOL> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if ifnot (act)
				discard ;
			write (Slice<STRU8> ("TRUE")) ;
		}
		if ifdo (act) {
			write (Slice<STRU8> ("FALSE")) ;
		}
	}

	void write (CREF<VAL32> item) override {
		unimplemented () ;
	}

	void write (CREF<VAL64> item) override {
		unimplemented () ;
	}

	void write (CREF<FLT32> item) override {
		unimplemented () ;
	}

	void write (CREF<FLT64> item) override {
		unimplemented () ;
	}

	void write (CREF<BYTE> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			fake.mStream.self[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<WORD> item) override {
		const auto r1x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (item) ;
		for (auto &&i : iter (0 ,r1x.size ()))
			write (r1x[i]) ;
	}

	void write (CREF<CHAR> item) override {
		const auto r1x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<CHAR>>>::expr] (item) ;
		for (auto &&i : iter (0 ,r1x.size ()))
			write (r1x[i]) ;
	}

	void write (CREF<QUAD> item) override {
		const auto r1x = bitwise[TYPE<BoxBuffer<BYTE ,SIZE_OF<QUAD>>>::expr] (item) ;
		for (auto &&i : iter (0 ,r1x.size ()))
			write (r1x[i]) ;
	}

	void write (CREF<Slice<STRA>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRA>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRW>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRW>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRU8>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRU8>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRU16>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRU16>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<Slice<STRU32>> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = bitwise[TYPE<BYTE_BASE<STRU32>>::expr] (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<String> item) override {
		unimplemented () ;
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (BOM)>) override {
		unimplemented () ;
	}

	void write (CREF<typeof (GAP)>) override {
		write (Slice<STRU8> ("\r\n")) ;
	}

	void write (CREF<typeof (EOS)>) override {
		while (TRUE) {
			if (fake.mWrite >= fake.mRead)
				break ;
			write (BYTE (0X00)) ;
		}
	}
} ;

exports VFat<TextWriterHolder> TextWriterHolder::create (VREF<TextWriterLayout> that) {
	return VFat<TextWriterHolder> (TextWriterImplement () ,Pointer::from (that)) ;
}

exports CFat<TextWriterHolder> TextWriterHolder::create (CREF<TextWriterLayout> that) {
	return CFat<TextWriterHolder> (TextWriterImplement () ,Pointer::from (that)) ;
}

struct StringToolPureLayout {} ;

class StringProcImplement implement Fat<StringToolHolder ,StringToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<StringToolPureLayout>::make () ;
	}

	BOOL parse_bool (CREF<String> a) const override {
		BOOL ret ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (a.raw ()) ;
		tmp.mSize = a.size () ;
		tmp.mStep = a.step () ;
		auto rax = TextReader (move (rbx)) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}

	String build_bool (CREF<BOOL> a) const override {
		String ret = String (64 ,SIZE_OF<STR>::expr) ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (ret.raw ()) ;
		tmp.mSize = ret.size () ;
		tmp.mStep = ret.step () ;
		auto rax = TextWriter (move (rbx)) ;
		rax << a ;
		rax << EOS ;
		return move (ret) ;
	}

	VAL32 parse_val32 (CREF<String> a) const override {
		VAL32 ret ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (a.raw ()) ;
		tmp.mSize = a.size () ;
		tmp.mStep = a.step () ;
		auto rax = TextReader (move (rbx)) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}

	String build_val32 (CREF<VAL32> a) const override {
		String ret = String (64 ,SIZE_OF<STR>::expr) ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (ret.raw ()) ;
		tmp.mSize = ret.size () ;
		tmp.mStep = ret.step () ;
		auto rax = TextWriter (move (rbx)) ;
		rax << a ;
		rax << EOS ;
		return move (ret) ;
	}

	VAL64 parse_val64 (CREF<String> a) const override {
		VAL64 ret ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (a.raw ()) ;
		tmp.mSize = a.size () ;
		tmp.mStep = a.step () ;
		auto rax = TextReader (move (rbx)) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}

	String build_val64 (CREF<VAL64> a) const override {
		String ret = String (64 ,SIZE_OF<STR>::expr) ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (ret.raw ()) ;
		tmp.mSize = ret.size () ;
		tmp.mStep = ret.step () ;
		auto rax = TextWriter (move (rbx)) ;
		rax << a ;
		rax << EOS ;
		return move (ret) ;
	}

	FLT32 parse_flt32 (CREF<String> a) const override {
		FLT32 ret ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (a.raw ()) ;
		tmp.mSize = a.size () ;
		tmp.mStep = a.step () ;
		auto rax = TextReader (move (rbx)) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}

	String build_flt32 (CREF<FLT32> a) const override {
		String ret = String (64 ,SIZE_OF<STR>::expr) ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (ret.raw ()) ;
		tmp.mSize = ret.size () ;
		tmp.mStep = ret.step () ;
		auto rax = TextWriter (move (rbx)) ;
		rax << a ;
		rax << EOS ;
		return move (ret) ;
	}

	FLT64 parse_flt64 (CREF<String> a) const override {
		FLT64 ret ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (a.raw ()) ;
		tmp.mSize = a.size () ;
		tmp.mStep = a.step () ;
		auto rax = TextReader (move (rbx)) ;
		rax >> ret ;
		rax >> EOS ;
		return move (ret) ;
	}

	String build_flt64 (CREF<FLT64> a) const override {
		String ret = String (64 ,SIZE_OF<STR>::expr) ;
		auto rbx = Ref<RefBuffer<BYTE>>::make () ;
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (rbx.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = address (ret.raw ()) ;
		tmp.mSize = ret.size () ;
		tmp.mStep = ret.step () ;
		auto rax = TextWriter (move (rbx)) ;
		rax << a ;
		rax << EOS ;
		return move (ret) ;
	}

	String stra_from_strw (CREF<String> a) const override {
		assert (a.step () == SIZE_OF<STRW>::expr) ;
		String ret = String (a.length () * 2 + 1 ,SIZE_OF<STRA>::expr) ;
		auto rax = csc_size_t () ;
		const auto r1x = wcstombs_s ((&rax) ,ret.raw (TYPE<STRA>::expr) ,VAL32 (ret.size ()) ,a.raw (TYPE<STRW>::expr) ,_TRUNCATE) ;
		assume (r1x == 0) ;
		return move (ret) ;
	}

	String strw_from_stra (CREF<String> a) const override {
		assert (a.step () == SIZE_OF<STRA>::expr) ;
		String ret = String (a.length () + 1 ,SIZE_OF<STRW>::expr) ;
		auto rax = csc_size_t () ;
		const auto r1x = mbstowcs_s ((&rax) ,ret.raw (TYPE<STRW>::expr) ,VAL32 (ret.size ()) ,a.raw (TYPE<STRA>::expr) ,_TRUNCATE) ;
		assume (r1x == 0) ;
		return move (ret) ;
	}

	String stru8_from_stru16 (CREF<String> a) const override {
		String ret = String (a.length () * 3 ,SIZE_OF<STRU8>::expr) ;
		INDEX ix = 0 ;
		auto rax = ZERO ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == NONE)
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU16 (0X007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU16 (0X07FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] >= STRU16 (0XD800))
					discard ;
				if ifnot (a[i] <= STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = 1 ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 1)
					discard ;
				if ifnot (a[i] >= STRU16 (0XDC00))
					discard ;
				if ifnot (a[i] <= STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = (STRU8 (rbx >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (rbx) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				rax = 0 ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = NONE ;
			}
		}
		if ifdo (TRUE) {
			if (rax == 0)
				discard ;
			ret[ix] = STRU8 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String stru8_from_stru32 (CREF<String> a) const override {
		/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
		String ret = String (a.length () * 6 ,SIZE_OF<STRU8>::expr) ;
		INDEX ix = 0 ;
		auto rax = ZERO ;
		for (auto &&i : a.range ()) {
			if (rax == NONE)
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X0000007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X000007FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X001FFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X07)) | STRU8 (0XF0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X03FFFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 24) & STRU8 (0X03)) | STRU8 (0XF8) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 30) & STRU8 (0X01)) | STRU8 (0XFC) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 24) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 18) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = NONE ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String stru16_from_stru8 (CREF<String> a) const override {
		String ret = String (a.length () ,SIZE_OF<STRU16>::expr) ;
		INDEX ix = 0 ;
		auto rax = ZERO ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == NONE)
				continue ;
			if ifdo (TRUE) {
				auto act = TRUE ;
				if ifdo (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (a[i] <= STRU8 (0X7F))
						discard ;
					ret[ix] = STRU16 (a[i]) ;
					ix++ ;
				}
				if ifdo (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (a[i] <= STRU8 (0XDF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
					rax = 1 ;
				}
				if ifdo (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (a[i] <= STRU8 (0XEF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
					rax = 2 ;
				}
				if ifdo (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (a[i] <= STRU8 (0XF7))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X07)) ;
					rax = 3 ;
				}
				if ifdo (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (a[i] <= STRU8 (0XFB))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X03)) ;
					rax = 4 ;
				}
				if ifdo (act) {
					if ifnot (rax == 0)
						discard ;
					if ifnot (a[i] <= STRU8 (0XFD))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X01)) ;
					rax = 5 ;
				}
				if ifdo (act) {
					if ifnot (rax == 1)
						discard ;
					if ifnot (a[i] <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax = 10 ;
				}
				if ifdo (act) {
					if ifnot (inline_between (rax ,2 ,6))
						discard ;
					if ifnot (a[i] <= STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
			if ifdo (TRUE) {
				if (rax != 10)
					discard ;
				auto act = TRUE ;
				if ifdo (act) {
					if ifnot (rbx <= STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (rbx) ;
					ix++ ;
					rax = 0 ;
				}
				if ifdo (act) {
					if ifnot (rbx <= STRU32 (0X0010FFFF))
						discard ;
					rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
					ret[ix] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
					rax = 0 ;
				}
				if ifdo (act) {
					if ifnot (rbx <= STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 ('?') ;
					ix++ ;
					rax = 0 ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = NONE ;
				}
			}
		}
		if ifdo (TRUE) {
			if (rax == 0)
				discard ;
			ret[ix] = STRU16 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String stru16_from_stru32 (CREF<String> a) const override {
		/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
		String ret = String (a.length () * 2 ,SIZE_OF<STRU16>::expr) ;
		INDEX ix = 0 ;
		auto rax = ZERO ;
		for (auto &&i : a.range ()) {
			if (rax == NONE)
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = STRU16 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X0010FFFF))
					discard ;
				ret[ix] = (STRU16 ((a[i] - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
				ix++ ;
				ret[ix] = (STRU16 (a[i] - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = STRU16 ('?') ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = NONE ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String stru32_from_stru8 (CREF<String> a) const override {
		/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
		String ret = String (a.length () ,SIZE_OF<STRU32>::expr) ;
		INDEX ix = 0 ;
		auto rax = ZERO ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == NONE)
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU8 (0X7F))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU8 (0XDF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
				rax = 1 ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU8 (0XEF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
				rax = 2 ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU8 (0XF7))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X07)) ;
				rax = 3 ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU8 (0XFB))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X03)) ;
				rax = 4 ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU8 (0XFD))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X01)) ;
				rax = 5 ;
			}
			if ifdo (act) {
				if ifnot (rax == 1)
					discard ;
				if ifnot (a[i] <= STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = 0 ;
			}
			if ifdo (act) {
				if ifnot (inline_between (rax ,2 ,6))
					discard ;
				if ifnot (a[i] <= STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				rax-- ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = NONE ;
			}
		}
		if ifdo (TRUE) {
			if (rax == 0)
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String stru32_from_stru16 (CREF<String> a) const override {
		/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
		String ret = String (a.length () ,SIZE_OF<STRU32>::expr) ;
		INDEX ix = 0 ;
		auto rax = ZERO ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == NONE)
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] <= STRU16 (0X07FF))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				if ifnot (a[i] >= STRU16 (0XD800))
					discard ;
				if ifnot (a[i] <= STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = 1 ;
			}
			if ifdo (act) {
				if ifnot (rax == 0)
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if ifnot (rax == 1)
					discard ;
				if ifnot (a[i] >= STRU16 (0XDC00))
					discard ;
				if ifnot (a[i] <= STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = 0 ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = NONE ;
			}
		}
		if ifdo (TRUE) {
			if (rax == 0)
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String cvt_stra (CREF<String> a) const override {
		String ret = String (a.length () ,SIZE_OF<STRA>::expr) ;
		auto rax = STRU32 () ;
		for (auto &&i : a.range ()) {
			a.get (i ,rax) ;
			assume (rax <= 127) ;
			ret.set (i ,rax) ;
		}
		return move (ret) ;
	}

	String cvt_strw (CREF<String> a) const override {
		if (IS_SAME<TEXT_BASE<STRW> ,STRU8>::expr)
			if (a.step () == SIZE_OF<STRU8>::expr)
				return a ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU8>::expr)
			if (a.step () == SIZE_OF<STRU16>::expr)
				return stru8_from_stru16 (a) ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU8>::expr)
			if (a.step () == SIZE_OF<STRU32>::expr)
				return stru8_from_stru32 (a) ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU16>::expr)
			if (a.step () == SIZE_OF<STRU8>::expr)
				return stru16_from_stru8 (a) ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU16>::expr)
			if (a.step () == SIZE_OF<STRU16>::expr)
				return a ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU16>::expr)
			if (a.step () == SIZE_OF<STRU32>::expr)
				return stru16_from_stru32 (a) ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU32>::expr)
			if (a.step () == SIZE_OF<STRU8>::expr)
				return stru32_from_stru8 (a) ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU32>::expr)
			if (a.step () == SIZE_OF<STRU16>::expr)
				return stru32_from_stru16 (a) ;
		if (IS_SAME<TEXT_BASE<STRW> ,STRU32>::expr)
			if (a.step () == SIZE_OF<STRU32>::expr)
				return a ;
		assume (FALSE) ;
		return String () ;
	}

	String cvt_strs (CREF<String> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return cvt_stra (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return cvt_strw (a) ;
		assume (FALSE) ;
		return String () ;
	}
} ;

exports VFat<StringToolHolder> StringToolHolder::create (VREF<StringToolLayout> that) {
	return VFat<StringToolHolder> (StringProcImplement () ,Pointer::from (that)) ;
}

exports CFat<StringToolHolder> StringToolHolder::create (CREF<StringToolLayout> that) {
	return CFat<StringToolHolder> (StringProcImplement () ,Pointer::from (that)) ;
}

struct RegexPureLayout {
	std::regex mRegex ;
	std::cmatch mCMatch ;
} ;

class RegexImplement implement Fat<RegexHolder ,RegexLayout> {
public:
	void initialize (CREF<String> text) override {
		fake.mThis = Ref<RegexPureLayout>::make () ;
		fake.mThis->mRegex = std::regex (text.raw (TYPE<STRA>::expr)) ;
	}

	INDEX search (CREF<String> text ,CREF<INDEX> index) override {
		assume (text.step () == 1) ;
		const auto r1x = text.raw (TYPE<STRA>::expr) ;
		const auto r2x = std::regex_search (r1x ,fake.mThis->mCMatch ,fake.mThis->mRegex) ;
		if ifnot (r2x)
			return NONE ;
		const auto r3x = FLAG (fake.mThis->mCMatch.suffix ().second) ;
		return r3x - FLAG (r1x) ;
	}

	String match (CREF<INDEX> index) const override {
		const auto r1x = FLAG (fake.mThis->mCMatch[index].first) ;
		const auto r2x = FLAG (fake.mThis->mCMatch[index].second) ;
		const auto r3x = Slice<STRA> (r1x ,r2x - r1x ,1) ;
		return String (r3x) ;
	}
} ;

exports VFat<RegexHolder> RegexHolder::create (VREF<RegexLayout> that) {
	return VFat<RegexHolder> (RegexImplement () ,Pointer::from (that)) ;
}

exports CFat<RegexHolder> RegexHolder::create (CREF<RegexLayout> that) {
	return CFat<RegexHolder> (RegexImplement () ,Pointer::from (that)) ;
}

struct XmlParserNode {
	String mName ;
	String mString ;
	SortedList<INDEX> mArraySet ;
	SortedList<String> mObjectSet ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct XmlParserPureLayout {
	Array<XmlParserNode> mTree ;
	INDEX mRoot ;
} ;

class XmlParserImplement implement Fat<XmlParserHolder ,XmlParserLayout> {
public:
	void initialize (CREF<RefBuffer<STRU8>> stream) override {
		unimplemented () ;
	}

	void initialize (CREF<XmlParserLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mIndex = that.mIndex ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	XmlParserLayout root () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	XmlParserLayout parent () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	XmlParserLayout brother () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	XmlParserLayout child () const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<INDEX> index) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArraySet.map (index) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<String> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectSet.map (name) ;
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list () const override {
		Array<XmlParserLayout> ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArraySet.length () ;
			ret = Array<XmlParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArraySet.map (r1x) ;
			}
		}
		return move (ret) ;
	}

	Array<XmlParserLayout> list (CREF<LENGTH> size_) const override {
		Array<XmlParserLayout> ret = Array<XmlParserLayout> (size_) ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArraySet.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArraySet.map (r1x) ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<XmlParserLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if ifnot (fake.mThis.exist ())
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String> name () const leftvalue override {
		if ifnot (exist ())
			return String::error () ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_bool (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_val32 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_val64 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_flt32 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_flt64 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String fetch (CREF<String> def) const override {
		try {
			assume (exist ()) ;
			return fake.mThis->mTree[fake.mIndex].mString ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<BOOL> ret = Array<BOOL> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<VAL32> ret = Array<VAL32> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<VAL64> ret = Array<VAL64> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<FLT32> ret = Array<FLT32> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<FLT64> ret = Array<FLT64> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<String> fetch (CREF<String> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<String> ret = Array<String> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}
} ;

exports VFat<XmlParserHolder> XmlParserHolder::create (VREF<XmlParserLayout> that) {
	return VFat<XmlParserHolder> (XmlParserImplement () ,Pointer::from (that)) ;
}

exports CFat<XmlParserHolder> XmlParserHolder::create (CREF<XmlParserLayout> that) {
	return CFat<XmlParserHolder> (XmlParserImplement () ,Pointer::from (that)) ;
}

struct JsonParserNode {
	String mName ;
	String mString ;
	SortedList<INDEX> mArraySet ;
	SortedList<String> mObjectSet ;
	INDEX mParent ;
	INDEX mBrother ;
	INDEX mChild ;
} ;

struct JsonParserPureLayout {
	Array<JsonParserNode> mTree ;
	INDEX mRoot ;
} ;

class JsonParserImplement implement Fat<JsonParserHolder ,JsonParserLayout> {
public:
	void initialize (CREF<RefBuffer<STRU8>> stream) override {
		unimplemented () ;
	}

	void initialize (CREF<JsonParserLayout> that) override {
		fake.mThis = that.mThis.share () ;
		fake.mIndex = that.mIndex ;
	}

	BOOL exist () const override {
		if (fake.mThis == NULL)
			return FALSE ;
		if (fake.mIndex == NONE)
			return FALSE ;
		return TRUE ;
	}

	JsonParserLayout root () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mRoot ;
		}
		return move (ret) ;
	}

	JsonParserLayout parent () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mParent ;
		}
		return move (ret) ;
	}

	JsonParserLayout brother () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mBrother ;
		}
		return move (ret) ;
	}

	JsonParserLayout child () const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mChild ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<INDEX> index) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mArraySet.map (index) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<String> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectSet.map (name) ;
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list () const override {
		Array<JsonParserLayout> ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArraySet.length () ;
			ret = Array<JsonParserLayout> (r1x) ;
			for (auto &&i : iter (0 ,r1x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArraySet.map (r1x) ;
			}
		}
		return move (ret) ;
	}

	Array<JsonParserLayout> list (CREF<LENGTH> size_) const override {
		Array<JsonParserLayout> ret = Array<JsonParserLayout> (size_) ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			const auto r1x = fake.mThis->mTree[fake.mIndex].mArraySet.length () ;
			const auto r2x = inline_min (r1x ,size_) ;
			for (auto &&i : iter (0 ,r2x)) {
				ret[i].mThis = fake.mThis.share () ;
				ret[i].mIndex = fake.mThis->mTree[fake.mIndex].mArraySet.map (r1x) ;
			}
		}
		return move (ret) ;
	}

	BOOL equal (CREF<JsonParserLayout> that) const override {
		const auto r1x = inline_compr (fake.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if ifnot (fake.mThis.exist ())
			return TRUE ;
		if (address (fake.mThis.self) != address (that.mThis.self))
			return FALSE ;
		if (fake.mIndex != that.mIndex)
			return FALSE ;
		return FALSE ;
	}

	CREF<String> name () const leftvalue override {
		if ifnot (exist ())
			return String::error () ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_bool (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_val32 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_val64 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_flt32 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		try {
			assume (exist ()) ;
			return StringTool::parse_flt64 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String fetch (CREF<String> def) const override {
		try {
			assume (exist ()) ;
			return fake.mThis->mTree[fake.mIndex].mString ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<BOOL> ret = Array<BOOL> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<VAL32> ret = Array<VAL32> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<VAL64> ret = Array<VAL64> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<FLT32> ret = Array<FLT32> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<FLT64> ret = Array<FLT64> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}

	Array<String> fetch (CREF<String> def ,CREF<LENGTH> size_) const override {
		const auto r1x = list (size_) ;
		Array<String> ret = Array<String> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}
} ;

exports VFat<JsonParserHolder> JsonParserHolder::create (VREF<JsonParserLayout> that) {
	return VFat<JsonParserHolder> (JsonParserImplement () ,Pointer::from (that)) ;
}

exports CFat<JsonParserHolder> JsonParserHolder::create (CREF<JsonParserLayout> that) {
	return CFat<JsonParserHolder> (JsonParserImplement () ,Pointer::from (that)) ;
}

struct PlyParserProperty {
	String mName ;
	FLAG mType ;
	FLAG mListType ;
} ;

struct PlyParserElement {
	String mName ;
	LENGTH mSize ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String> mPropertySet ;
} ;

struct HEADER {
	String mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String> mElementSet ;
	LENGTH mBodyOffset ;
} ;

struct PlyParserBody {
	FLAG mType ;
	Array<Tuple<INDEX ,INDEX>> mLine ;
} ;

struct PlyParserBodyType {
	enum {
		Bool ,
		Val32 ,
		Val64 ,
		Flt32 ,
		Flt64 ,
		Byte ,
		Word ,
		Char ,
		Quad ,
		Null ,
		ETC
	} ;
} ;

struct PlyParserPureLayout {
	HEADER mHeader ;
	Array<Array<PlyParserBody>> mBody ;
	Deque<BYTE> mPlyBYTE ;
	Deque<WORD> mPlyWORD ;
	Deque<CHAR> mPlyCHAR ;
	Deque<QUAD> mPlyQUAD ;
} ;

class PlyParserImplement implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CREF<RefBuffer<STRU8>> stream) override {
		unimplemented () ;
	}

	INDEX find_element (CREF<String> name) const override {
		return fake.mThis->mHeader.mElementSet.map (name) ;
	}

	LENGTH element_size (CREF<INDEX> element) const override {
		return fake.mThis->mHeader.mElementList[element].mSize ;
	}

	INDEX find_property (CREF<INDEX> element ,CREF<String> name) const override {
		return fake.mThis->mHeader.mElementList[element].mPropertySet.map (name) ;
	}

	LENGTH property_size (CREF<INDEX> element ,CREF<INDEX> line ,CREF<INDEX> property) const override {
		const auto r1x = fake.mThis->mBody[element][property].mLine[line] ;
		return r1x.m2nd - r1x.m1st ;
	}

	void guide_new (CREF<INDEX> element) override {
		fake.mGuide.mElement = element ;
		fake.mGuide.mProperty.clear () ;
		fake.mGuide.mPropertyIndex = 0 ;
		fake.mGuide.mLineIndex = 0 ;
		fake.mGuide.mPlyIndex = NONE ;
	}

	void guide_put (CREF<INDEX> property) override {
		assert (fake.mGuide.mElement != NONE) ;
		assert (fake.mGuide.mPlyIndex == NONE) ;
		fake.mGuide.mProperty.add (property) ;
	}

	void guide_jmp () {
		assert (fake.mGuide.mElement != NONE) ;
		INDEX ix = fake.mGuide.mElement ;
		INDEX jx = NONE ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mGuide.mPlyIndex != NONE)
				discard ;
			fake.mGuide.mLineIndex = 0 ;
			fake.mGuide.mPropertyIndex = 0 ;
			assume (fake.mGuide.mPropertyIndex < fake.mGuide.mProperty.length ()) ;
			assume (fake.mGuide.mLineIndex < fake.mThis->mHeader.mElementList[ix].mSize) ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			fake.mGuide.mPlyIndex = fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m1st ;
		}
		if ifdo (act) {
			fake.mGuide.mPlyIndex++ ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			if (fake.mGuide.mPlyIndex >= fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m2nd)
				discard ;
		}
		if ifdo (act) {
			fake.mGuide.mPropertyIndex++ ;
			if (fake.mGuide.mPropertyIndex >= fake.mGuide.mProperty.length ())
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			fake.mGuide.mPlyIndex = fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m1st ;
		}
		if ifdo (act) {
			fake.mGuide.mLineIndex++ ;
			fake.mGuide.mPropertyIndex = 0 ;
			if (fake.mGuide.mLineIndex >= fake.mThis->mHeader.mElementList[ix].mSize)
				discard ;
			jx = fake.mGuide.mProperty[fake.mGuide.mPropertyIndex] ;
			fake.mGuide.mPlyIndex = fake.mThis->mBody[ix][jx].mLine[fake.mGuide.mLineIndex].m1st ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
		if ifdo (TRUE) {
			fake.mGuide.mPlyType = fake.mThis->mHeader.mElementList[ix].mPropertyList[jx].mListType ;
			if (fake.mGuide.mPlyType != PlyParserBodyType::Null)
				discard ;
			fake.mGuide.mPlyType = fake.mThis->mHeader.mElementList[ix].mPropertyList[jx].mType ;
		}
	}

	void read (VREF<BOOL> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Bool) ;
		item = bitwise[TYPE<BOOL>::expr] (fake.mThis->mPlyBYTE[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<VAL32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Val32) ;
		item = bitwise[TYPE<VAL32>::expr] (fake.mThis->mPlyCHAR[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<VAL64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Val64) ;
		item = bitwise[TYPE<VAL64>::expr] (fake.mThis->mPlyQUAD[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<FLT32> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Flt32) ;
		item = bitwise[TYPE<FLT32>::expr] (fake.mThis->mPlyCHAR[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<FLT64> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Flt64) ;
		item = bitwise[TYPE<FLT64>::expr] (fake.mThis->mPlyQUAD[fake.mGuide.mPlyIndex]) ;
	}

	void read (VREF<BYTE> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Byte) ;
		item = fake.mThis->mPlyBYTE[fake.mGuide.mPlyIndex] ;
	}

	void read (VREF<WORD> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Byte) ;
		item = fake.mThis->mPlyWORD[fake.mGuide.mPlyIndex] ;
	}

	void read (VREF<CHAR> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Byte) ;
		item = fake.mThis->mPlyCHAR[fake.mGuide.mPlyIndex] ;
	}

	void read (VREF<QUAD> item) override {
		guide_jmp () ;
		assume (fake.mGuide.mPlyType == PlyParserBodyType::Byte) ;
		item = fake.mThis->mPlyQUAD[fake.mGuide.mPlyIndex] ;
	}
} ;

exports VFat<PlyParserHolder> PlyParserHolder::create (VREF<PlyParserLayout> that) {
	return VFat<PlyParserHolder> (PlyParserImplement () ,Pointer::from (that)) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::create (CREF<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplement () ,Pointer::from (that)) ;
}
} ;