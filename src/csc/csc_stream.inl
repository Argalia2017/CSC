#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_stream.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <regex>
#include <sstream>
#include "csc_begin.h"

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4996)
#endif

namespace CSC {
struct StreamProcPureLayout {
	HashSet<STRU32> mBlankSet ;
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

class StreamProcImplement implement Fat<StreamProcHolder ,StreamProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<StreamProcPureLayout>::make () ;
		fake.mThis->mBlankSet.add (STRU32 (' ') ,GapType::Space) ;
		fake.mThis->mBlankSet.add (STRU32 ('\t') ,GapType::Space) ;
		fake.mThis->mBlankSet.add (STRU32 ('\b') ,GapType::Space) ;
		fake.mThis->mBlankSet.add (STRU32 ('\r') ,GapType::Endline) ;
		fake.mThis->mBlankSet.add (STRU32 ('\n') ,GapType::Endline) ;
		fake.mThis->mBlankSet.add (STRU32 ('\f') ,GapType::Endline) ;
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
		escape_list_add (STRU32 ('u') ,STRU32 (0X1A)) ;
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

	BOOL is_blank (CREF<STRU32> str) const override {
		return fake.mThis->mBlankSet.contain (str) ;
	}

	BOOL is_space (CREF<STRU32> str) const override {
		return fake.mThis->mBlankSet.map (str) == GapType::Space ;
	}

	BOOL is_endline (CREF<STRU32> str) const override {
		return fake.mThis->mBlankSet.map (str) == GapType::Endline ;
	}

	BOOL is_alpha (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('z'))
				return TRUE ;
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('Z'))
				return TRUE ;
		return FALSE ;
	}

	STRU32 alpha_lower (CREF<STRU32> str) const override {
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('Z'))
				return str - STRU32 ('A') + STRU32 ('a') ;
		return str ;
	}

	STRU32 alpha_upper (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('z'))
				return str - STRU32 ('a') + STRU32 ('A') ;
		return str ;
	}

	BOOL is_digit (CREF<STRU32> str) const override {
		if (str >= STRU32 ('0'))
			if (str <= STRU32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	BOOL is_hex_digit (CREF<STRU32> str) const override {
		if (str >= STRU32 ('a'))
			if (str <= STRU32 ('f'))
				return TRUE ;
		if (str >= STRU32 ('A'))
			if (str <= STRU32 ('F'))
				return TRUE ;
		return FALSE ;
	}

	INDEX hex_from_str (CREF<STRU32> str) const override {
		if (is_digit (str))
			return INDEX (str - STRU32 ('0')) ;
		if (is_hex_digit (str))
			return INDEX (alpha_upper (str) - STRU32 ('A') + 10) ;
		assume (FALSE) ;
		return INDEX () ;
	}

	STRU32 str_from_hex (CREF<INDEX> hex) const override {
		if (inline_between (hex ,0 ,10))
			return STRU32 ('0') + STRU32 (hex) ;
		if (inline_between (hex ,10 ,16))
			return STRU32 ('A') + STRU32 (hex) - 10 ;
		assume (FALSE) ;
		return STRU32 () ;
	}

	BOOL is_word (CREF<STRU32> str) const override {
		if (is_alpha (str))
			return TRUE ;
		if (is_digit (str))
			return TRUE ;
		if (str == STRU32 ('_'))
			return TRUE ;
		return FALSE ;
	}

	BOOL is_control (CREF<STRU32> str) const override {
		if (str == STRU32 (127))
			return TRUE ;
		if (str >= STRU32 (32))
			return FALSE ;
		if (fake.mThis->mBlankSet.contain (str))
			return FALSE ;
		return TRUE ;
	}

	STRU32 word_from_ctrl (CREF<STRU32> str) const override {
		INDEX ix = fake.mThis->mEscapeCtrlSet.map (str) ;
		assume (ix != NONE) ;
		return fake.mThis->mEscapeList[ix].m1st ;
	}

	STRU32 ctrl_from_word (CREF<STRU32> str) const override {
		INDEX ix = fake.mThis->mEscapeWordSet.map (str) ;
		assume (ix != NONE) ;
		return fake.mThis->mEscapeList[ix].m2nd ;
	}
} ;

exports VFat<StreamProcHolder> StreamProcHolder::create (VREF<StreamProcLayout> that) {
	return VFat<StreamProcHolder> (StreamProcImplement () ,that) ;
}

exports CFat<StreamProcHolder> StreamProcHolder::create (CREF<StreamProcLayout> that) {
	return CFat<StreamProcHolder> (StreamProcImplement () ,that) ;
}

class ByteReaderImplement implement Fat<ByteReaderHolder ,ByteReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assume (stream != NULL) ;
		assume (stream->step () == 1) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mRead ;
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
			if ifnot (StreamProc::big_endian ())
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
			if ifnot (StreamProc::big_endian ())
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
			if ifnot (StreamProc::big_endian ())
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

	void read (CREF<Slice> item) override {
		auto rax = BYTE () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			assume (STRU32 (rax) == item[i]) ;
		}
	}

	void read (VREF<String<STRA>> item) override {
		auto rax = VAL32 () ;
		auto rbx = VAL32 () ;
		auto rcx = BYTE_BASE<STRA> () ;
		read (rax) ;
		read (rbx) ;
		assume (rbx == SIZE_OF<STRA>::expr) ;
		item = String<STRA> (rax) ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rcx) ;
			item[i] = STRA (rcx) ;
		}
	}

	void read (VREF<String<STRW>> item) override {
		auto rax = VAL32 () ;
		auto rbx = VAL32 () ;
		auto rcx = BYTE_BASE<STRW> () ;
		read (rax) ;
		read (rbx) ;
		assume (rbx == SIZE_OF<STRW>::expr) ;
		item = String<STRW> (rax) ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rcx) ;
			item[i] = STRW (rcx) ;
		}
	}

	void read (VREF<String<STRU8>> item) override {
		auto rax = VAL32 () ;
		auto rbx = VAL32 () ;
		auto rcx = BYTE_BASE<STRU8> () ;
		read (rax) ;
		read (rbx) ;
		assume (rbx == SIZE_OF<STRU8>::expr) ;
		item = String<STRU8> (rax) ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rcx) ;
			item[i] = STRU8 (rcx) ;
		}
	}

	void read (VREF<String<STRU16>> item) override {
		auto rax = VAL32 () ;
		auto rbx = VAL32 () ;
		auto rcx = BYTE_BASE<STRU16> () ;
		read (rax) ;
		read (rbx) ;
		assume (rbx == SIZE_OF<STRU16>::expr) ;
		item = String<STRU16> (rax) ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rcx) ;
			item[i] = STRU16 (rcx) ;
		}
	}

	void read (VREF<String<STRU32>> item) override {
		auto rax = VAL32 () ;
		auto rbx = VAL32 () ;
		auto rcx = BYTE_BASE<STRU32> () ;
		read (rax) ;
		read (rbx) ;
		assume (rbx == SIZE_OF<STRU32>::expr) ;
		item = String<STRU32> (rax) ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rcx) ;
			item[i] = STRU32 (rcx) ;
		}
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (GAP)>) override {
		auto rax = BYTE () ;
		read (rax) ;
		assume (rax == BYTE (0X5D)) ;
		read (rax) ;
		assume (rax == BYTE (0X5B)) ;
	}

	void read (CREF<typeof (EOS)>) override {
		assume (fake.mRead >= fake.mWrite) ;
	}

	void read (CREF<VFat<ByteReaderFriend>> item) override {
		item->friend_read (keep[TYPE<ByteReader>::expr] (fake)) ;
	}
} ;

exports VFat<ByteReaderHolder> ByteReaderHolder::create (VREF<ByteReaderLayout> that) {
	return VFat<ByteReaderHolder> (ByteReaderImplement () ,that) ;
}

exports CFat<ByteReaderHolder> ByteReaderHolder::create (CREF<ByteReaderLayout> that) {
	return CFat<ByteReaderHolder> (ByteReaderImplement () ,that) ;
}

class ByteWriterImplement implement Fat<ByteWriterHolder ,ByteWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assume (stream != NULL) ;
		assume (stream->step () == 1) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mWrite ;
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
			if ifnot (StreamProc::big_endian ())
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
			if ifnot (StreamProc::big_endian ())
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
			if ifnot (StreamProc::big_endian ())
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

	void write (CREF<Slice> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			const auto r1x = BYTE (item[i]) ;
			write (r1x) ;
		}
	}

	void write (CREF<String<STRA>> item) override {
		const auto r1x = VAL32 (item.length ()) ;
		const auto r2x = VAL32 (item.step ()) ;
		write (r1x) ;
		write (r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = bitwise[TYPE<BYTE_BASE<STRA>>::expr] (item[i]) ;
			write (r3x) ;
		}
	}

	void write (CREF<String<STRW>> item) override {
		const auto r1x = VAL32 (item.length ()) ;
		const auto r2x = VAL32 (item.step ()) ;
		write (r1x) ;
		write (r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = bitwise[TYPE<BYTE_BASE<STRW>>::expr] (item[i]) ;
			write (r3x) ;
		}
	}

	void write (CREF<String<STRU8>> item) override {
		const auto r1x = VAL32 (item.length ()) ;
		const auto r2x = VAL32 (item.step ()) ;
		write (r1x) ;
		write (r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = bitwise[TYPE<BYTE_BASE<STRU8>>::expr] (item[i]) ;
			write (r3x) ;
		}
	}

	void write (CREF<String<STRU16>> item) override {
		const auto r1x = VAL32 (item.length ()) ;
		const auto r2x = VAL32 (item.step ()) ;
		write (r1x) ;
		write (r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = bitwise[TYPE<BYTE_BASE<STRU16>>::expr] (item[i]) ;
			write (r3x) ;
		}
	}

	void write (CREF<String<STRU32>> item) override {
		const auto r1x = VAL32 (item.length ()) ;
		const auto r2x = VAL32 (item.step ()) ;
		write (r1x) ;
		write (r2x) ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r3x = bitwise[TYPE<BYTE_BASE<STRU32>>::expr] (item[i]) ;
			write (r3x) ;
		}
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (GAP)>) override {
		write (BYTE (0X5D)) ;
		write (BYTE (0X5B)) ;
	}

	void write (CREF<typeof (EOS)>) override {
		while (TRUE) {
			if (fake.mWrite >= fake.mRead)
				break ;
			write (BYTE (0X00)) ;
		}
	}

	void write (CREF<CFat<ByteWriterFriend>> item) override {
		item->friend_write (keep[TYPE<ByteWriter>::expr] (fake)) ;
	}
} ;

exports VFat<ByteWriterHolder> ByteWriterHolder::create (VREF<ByteWriterLayout> that) {
	return VFat<ByteWriterHolder> (ByteWriterImplement () ,that) ;
}

exports CFat<ByteWriterHolder> ByteWriterHolder::create (CREF<ByteWriterLayout> that) {
	return CFat<ByteWriterHolder> (ByteWriterImplement () ,that) ;
}

class TextReaderImplement implement Fat<TextReaderHolder ,TextReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assume (stream != NULL) ;
		assume (stream->step () <= 4) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mRead ;
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
		auto rax = STRU32 () ;
		read (rax) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (rax != STRU32 ('t'))
				discard ;
			fake.mRead-- ;
			read (slice ("true")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('T'))
				discard ;
			fake.mRead-- ;
			read (slice ("TRUE")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('f'))
				discard ;
			fake.mRead-- ;
			read (slice ("false")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('F'))
				discard ;
			fake.mRead-- ;
			read (slice ("FALSE")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read (VREF<VAL32> item) override {
		auto rax = VAL64 () ;
		read (rax) ;
		assume (rax >= VAL32_MIN) ;
		assume (rax <= VAL32_MAX) ;
		item = VAL32 (rax) ;
	}

	void read (VREF<VAL64> item) override {
		//@mark
		const auto r1x = CPTR<char> (&fake.mStream.self[fake.mRead]) ;
		const auto r2x = inline_min (64 ,fake.mWrite - fake.mRead) ;
		auto rax = std::stringstream (std::string (r1x ,r2x)) ;
		rax >> item ;
	}

	void read (VREF<FLT32> item) override {
		auto rax = FLT64 () ;
		read (rax) ;
		assume (rax >= FLT32_MIN) ;
		assume (rax <= FLT32_MAX) ;
		item = FLT32 (rax) ;
	}

	void read (VREF<FLT64> item) override {
		//@mark
		const auto r1x = CPTR<char> (&fake.mStream.self[fake.mRead]) ;
		const auto r2x = inline_min (64 ,fake.mWrite - fake.mRead) ;
		auto rax = std::stringstream (std::string (r1x ,r2x)) ;
		rax >> item ;
	}

	void read (VREF<BYTE> item) override {
		assume (fake.mStream->step () == SIZE_OF<BYTE>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<BYTE>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void read (VREF<WORD> item) override {
		assume (fake.mStream->step () == SIZE_OF<WORD>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<WORD>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void read (VREF<CHAR> item) override {
		assume (fake.mStream->step () == SIZE_OF<CHAR>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<CHAR>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void read (VREF<QUAD> item) override {
		assume (fake.mStream->step () == SIZE_OF<QUAD>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<QUAD>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void read (VREF<STRU32> item) {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			auto rax = BYTE_BASE<STRU8> () ;
			read (rax) ;
			item = STRU32 (rax) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			auto rax = BYTE_BASE<STRU16> () ;
			read (rax) ;
			item = STRU32 (rax) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			auto rax = BYTE_BASE<STRU32> () ;
			read (rax) ;
			item = STRU32 (rax) ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read (CREF<Slice> item) override {
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			read (rax) ;
			assume (rax == item[i]) ;
		}
	}

	void read (VREF<String<STRA>> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
		const auto r1x = next_reading () ;
		item = String<STRA> (r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = STRA (rax) ;
		}
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
	}

	void read (VREF<String<STRW>> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
		const auto r1x = next_reading () ;
		item = String<STRW> (r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = STRW (rax) ;
		}
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
	}

	void read (VREF<String<STRU8>> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
		const auto r1x = next_reading () ;
		item = String<STRU8> (r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = STRU8 (rax) ;
		}
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
	}

	void read (VREF<String<STRU16>> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
		const auto r1x = next_reading () ;
		item = String<STRU16> (r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = STRU16 (rax) ;
		}
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
	}

	void read (VREF<String<STRU32>> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
		const auto r1x = next_reading () ;
		item = String<STRU32> (r1x) ;
		for (auto &&i : iter (0 ,r1x)) {
			read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = STRU32 (rax) ;
		}
		read (rax) ;
		assume (rax == STRU32 ('\"')) ;
	}

	LENGTH next_reading () {
		LENGTH ret = 0 ;
		const auto r1x = fake.mRead ;
		auto rax = STRU32 () ;
		read (rax) ;
		while (TRUE) {
			if (rax == STRU32 ('\"'))
				break ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				read (rax) ;
			}
			ret++ ;
			read (rax) ;
		}
		fake.mRead = r1x ;
		return move (ret) ;
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (BOM)>) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 (0XEF)) ;
		read (rax) ;
		assume (rax == STRU32 (0XBB)) ;
		read (rax) ;
		assume (rax == STRU32 (0XBF)) ;
	}

	void read (CREF<typeof (GAP)>) override {
		const auto r1x = next_read_gap () ;
		fake.mRead += r1x ;
	}

	LENGTH next_read_gap () {
		LENGTH ret = 0 ;
		const auto r1x = fake.mRead ;
		auto rax = STRU32 () ;
		read (rax) ;
		while (TRUE) {
			if (rax == STRU32 (0X00))
				break ;
			if (StreamProc::is_blank (rax))
				break ;
			ret++ ;
			read (rax) ;
		}
		fake.mRead = r1x ;
		return move (ret) ;
	}

	void read (CREF<typeof (EOS)>) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 (0X00)) ;
	}

	void read (CREF<VFat<TextReaderFriend>> item) override {
		item->friend_read (keep[TYPE<TextReader>::expr] (fake)) ;
	}
} ;

exports VFat<TextReaderHolder> TextReaderHolder::create (VREF<TextReaderLayout> that) {
	return VFat<TextReaderHolder> (TextReaderImplement () ,that) ;
}

exports CFat<TextReaderHolder> TextReaderHolder::create (CREF<TextReaderLayout> that) {
	return CFat<TextReaderHolder> (TextReaderImplement () ,that) ;
}

class TextWriterImplement implement Fat<TextWriterHolder ,TextWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assume (stream != NULL) ;
		assume (stream->step () <= 4) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	LENGTH size () const override {
		return fake.mStream->size () ;
	}

	LENGTH length () const override {
		return fake.mWrite ;
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
			write (slice ("TRUE")) ;
		}
		if ifdo (act) {
			write (slice ("FALSE")) ;
		}
	}

	void write (CREF<VAL32> item) override {
		const auto r1x = VAL64 (item) ;
		write (r1x) ;
	}

	void write (CREF<VAL64> item) override {
		//@mark
		auto rax = std::stringstream () ;
		rax << item ;
		const auto r1x = rax.str () ;
		for (auto &&i : iter (0 ,r1x.length ())) {
			write (BYTE (r1x[i])) ;
		}
	}

	void write (CREF<FLT32> item) override {
		const auto r1x = FLT64 (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT64> item) override {
		//@mark
		auto rax = std::stringstream () ;
		rax << item ;
		const auto r1x = rax.str () ;
		for (auto &&i : iter (0 ,r1x.length ())) {
			write (BYTE (r1x[i])) ;
		}
	}

	void write (CREF<BYTE> item) override {
		assume (fake.mStream->step () == SIZE_OF<BYTE>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<BYTE>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<WORD> item) override {
		assume (fake.mStream->step () == SIZE_OF<WORD>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<WORD>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<CHAR> item) override {
		assume (fake.mStream->step () == SIZE_OF<CHAR>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<CHAR>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<QUAD> item) override {
		assume (fake.mStream->step () == SIZE_OF<QUAD>::expr) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<QUAD>>::expr] (Pointer::from (fake.mStream.self)) ;
			tmp[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<STRU32> item) {
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			const auto r1x = BYTE_BASE<STRU8> (item) ;
			write (r1x) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			const auto r2x = BYTE_BASE<STRU16> (item) ;
			write (r2x) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			const auto r3x = BYTE_BASE<STRU32> (item) ;
			write (r3x) ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void write (CREF<Slice> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			const auto r1x = item[i] ;
			write (r1x) ;
		}
	}

	void write (CREF<String<STRA>> item) override {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<String<STRW>> item) override {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<String<STRU8>> item) override {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<String<STRU16>> item) override {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<String<STRU32>> item) override {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = STRU32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (BOM)>) override {
		write (STRU32 (0XEF)) ;
		write (STRU32 (0XBB)) ;
		write (STRU32 (0XBF)) ;
	}

	void write (CREF<typeof (GAP)>) override {
		write (slice ("\r\n")) ;
	}

	void write (CREF<typeof (EOS)>) override {
		while (TRUE) {
			if (fake.mWrite >= fake.mRead)
				break ;
			write (STRU32 (0X00)) ;
		}
	}

	void write (CREF<CFat<TextWriterFriend>> item) override {
		item->friend_write (keep[TYPE<TextWriter>::expr] (fake)) ;
	}
} ;

exports VFat<TextWriterHolder> TextWriterHolder::create (VREF<TextWriterLayout> that) {
	return VFat<TextWriterHolder> (TextWriterImplement () ,that) ;
}

exports CFat<TextWriterHolder> TextWriterHolder::create (CREF<TextWriterLayout> that) {
	return CFat<TextWriterHolder> (TextWriterImplement () ,that) ;
}

template <class A>
struct FUNTION_string_from_impl {
	template <class ARG1>
	forceinline CREF<String<A>> operator() (CREF<String<ARG1>> that) {
		assume (IS_SAME<A ,ARG1>::expr) ;
		return keep[TYPE<String<A>>::expr] (Pointer::from (that)) ;
	}

	template <class ARG1>
	forceinline RREF<String<A>> operator() (RREF<String<ARG1>> that) {
		assume (IS_SAME<A ,ARG1>::expr) ;
		return move (keep[TYPE<String<A>>::expr] (Pointer::from (that))) ;
	}
} ;

struct FUNTION_string_from {
	template <class ARG1>
	forceinline consteval FUNTION_string_from_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNTION_string_from_impl<ARG1> () ;
	}
} ;

static constexpr auto string_from = FUNTION_string_from () ;

struct StringProcPureLayout {} ;

class StringProcImplement implement Fat<StringProcHolder ,StringProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<StringProcPureLayout>::make () ;
	}

	String<STRA> stra_from_strw (CREF<String<STRW>> a) const override {
		String<STRA> ret = String<STRA> (a.length () * 2 + 1) ;
		std::setlocale (LC_CTYPE ,"") ;
		const auto r1x = (ret.size () + 1) * ret.step () ;
		const auto r2x = std::wcstombs (ret.raw () ,a.raw () ,r1x) ;
		assume (r2x >= 0) ;
		return move (ret) ;
	}

	String<STRA> stra_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STRA>::expr] (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return stra_from_strw (string_from[TYPE<STRW>::expr] (a)) ;
		assume (FALSE) ;
		return String<STRA> () ;
	}

	String<STRW> strw_from_stra (CREF<String<STRA>> a) const override {
		String<STRW> ret = String<STRW> (a.length () + 1) ;
		std::setlocale (LC_CTYPE ,"") ;
		const auto r1x = (ret.size () + 1) * ret.step () ;
		const auto r2x = std::mbstowcs (ret.raw () ,a.raw () ,r1x) ;
		assume (r2x >= 0) ;
		return move (ret) ;
	}

	String<STRW> strw_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return strw_from_stra (string_from[TYPE<STRA>::expr] (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STRW>::expr] (a) ;
		assume (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stra (CREF<String<STRA>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (a) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stra (a)) ;
		assume (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_strw (CREF<String<STRW>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_strw (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (a) ;
		assume (FALSE) ;
		return String<STR> () ;
	}

	String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) const override {
		String<STRU8> ret = String<STRU8> (a.length () * 3) ;
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

	String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) const override {
		/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
		String<STRU8> ret = String<STRU8> (a.length () * 6) ;
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

	String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) const override {
		String<STRU16> ret = String<STRU16> (a.length ()) ;
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

	String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) const override {
		/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
		String<STRU16> ret = String<STRU16> (a.length () * 2) ;
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

	String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) const override {
		/*
			*	1 bytes [0,0X7F] 0xxxxxxx
			*	2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
			*	3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
			*	4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*	6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			*/
		String<STRU32> ret = String<STRU32> (a.length ()) ;
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

	String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) const override {
		/*
			*	utf16 surrogate pairs [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
			*	utf16-utf32 surrogate pairs [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
			*/
		String<STRU32> ret = String<STRU32> (a.length ()) ;
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

	String<STRUA> strua_from_stra (RREF<String<STRA>> a) const override {
		return string_from[TYPE<STRUA>::expr] (a) ;
	}

	String<STRA> stra_from_strua (RREF<String<STRUA>> a) const override {
		return string_from[TYPE<STRA>::expr] (a) ;
	}

	String<STRUW> struw_from_strw (RREF<String<STRW>> a) const override {
		return string_from[TYPE<STRUW>::expr] (a) ;
	}

	String<STRW> strw_from_struw (RREF<String<STRUW>> a) const override {
		return string_from[TYPE<STRW>::expr] (a) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU8>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU16>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRA> stra_from_stru (CREF<String<STRU32>> a) const override {
		return stra_from_strw (strw_from_stru (a)) ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru8 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru8 (a))) ;
		assume (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru16 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru16 (a))) ;
		assume (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (move (a))) ;
		assume (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assume (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assume (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assume (FALSE) ;
		return String<STR> () ;
	}
} ;

exports VFat<StringProcHolder> StringProcHolder::create (VREF<StringProcLayout> that) {
	return VFat<StringProcHolder> (StringProcImplement () ,that) ;
}

exports CFat<StringProcHolder> StringProcHolder::create (CREF<StringProcLayout> that) {
	return CFat<StringProcHolder> (StringProcImplement () ,that) ;
}

struct RegexPureLayout {
	std::regex mRegex ;
	std::cmatch mMatch ;
} ;

class RegexImplement implement Fat<RegexHolder ,RegexLayout> {
public:
	void initialize (CREF<String<STRA>> text) override {
		fake.mThis = Ref<RegexPureLayout>::make () ;
		fake.mThis->mRegex = std::regex (text.raw ()) ;
	}

	INDEX search (CREF<String<STRA>> text ,CREF<INDEX> index) override {
		const auto r1x = text.raw () ;
		const auto r2x = std::regex_search (r1x ,fake.mThis->mMatch ,fake.mThis->mRegex) ;
		if ifnot (r2x)
			return NONE ;
		const auto r3x = FLAG (fake.mThis->mMatch.suffix ().second) ;
		return r3x - FLAG (r1x) ;
	}

	String<STRA> match (CREF<INDEX> index) const override {
		const auto r1x = FLAG (fake.mThis->mMatch[index].first) ;
		const auto r2x = FLAG (fake.mThis->mMatch[index].second) ;
		const auto r3x = Slice (r1x ,r2x - r1x ,SIZE_OF<STRU8>::expr) ;
		return String<STRA> (r3x) ;
	}
} ;

exports VFat<RegexHolder> RegexHolder::create (VREF<RegexLayout> that) {
	return VFat<RegexHolder> (RegexImplement () ,that) ;
}

exports CFat<RegexHolder> RegexHolder::create (CREF<RegexLayout> that) {
	return CFat<RegexHolder> (RegexImplement () ,that) ;
}

struct XmlParserNode {
	String<STRU8> mName ;
	String<STRU8> mString ;
	SortedList<INDEX> mArraySet ;
	SortedList<String<STRU8>> mObjectSet ;
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
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
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

	XmlParserLayout child (CREF<Slice> name) const override {
		XmlParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectSet.map (name) ;
		}
		return move (ret) ;
	}

	XmlParserLayout child (CREF<String<STRU8>> name) const override {
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

	CREF<String<STRU8>> name () const leftvalue override {
		if ifnot (exist ())
			return String<STRU8>::error () ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			BOOL ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			VAL32 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			VAL64 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			FLT32 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			FLT64 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return fake.mThis->mTree[fake.mIndex].mString ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	template <class ARG1>
	Array<ARG1> fetch_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list (size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}
} ;

exports VFat<XmlParserHolder> XmlParserHolder::create (VREF<XmlParserLayout> that) {
	return VFat<XmlParserHolder> (XmlParserImplement () ,that) ;
}

exports CFat<XmlParserHolder> XmlParserHolder::create (CREF<XmlParserLayout> that) {
	return CFat<XmlParserHolder> (XmlParserImplement () ,that) ;
}

struct JsonParserNode {
	String<STRU8> mName ;
	String<STRU8> mString ;
	SortedList<INDEX> mArraySet ;
	SortedList<String<STRU8>> mObjectSet ;
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
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
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

	JsonParserLayout child (CREF<Slice> name) const override {
		JsonParserLayout ret ;
		if ifdo (TRUE) {
			if ifnot (exist ())
				discard ;
			ret.mThis = fake.mThis.share () ;
			ret.mIndex = fake.mThis->mTree[fake.mIndex].mObjectSet.map (name) ;
		}
		return move (ret) ;
	}

	JsonParserLayout child (CREF<String<STRU8>> name) const override {
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

	CREF<String<STRU8>> name () const leftvalue override {
		if ifnot (exist ())
			return String<STRU8>::error () ;
		return fake.mThis->mTree[fake.mIndex].mName ;
	}

	BOOL fetch (CREF<BOOL> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			BOOL ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL32 fetch (CREF<VAL32> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			VAL32 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	VAL64 fetch (CREF<VAL64> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			VAL64 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT32 fetch (CREF<FLT32> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			FLT32 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	FLT64 fetch (CREF<FLT64> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			FLT64 ret ;
			auto rax = TextReader (fake.mThis->mTree[fake.mIndex].mString.borrow ()) ;
			rax >> ret ;
			rax >> EOS ;
			return move (ret) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRA> fetch (CREF<String<STRA>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::stra_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRW> fetch (CREF<String<STRW>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::strw_from_stru (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU8> fetch (CREF<String<STRU8>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return fake.mThis->mTree[fake.mIndex].mString ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU16> fetch (CREF<String<STRU16>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::stru16_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	String<STRU32> fetch (CREF<String<STRU32>> def) const override {
		if ifnot (exist ())
			return def ;
		try {
			return StringProc::stru32_from_stru8 (fake.mThis->mTree[fake.mIndex].mString) ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
		return def ;
	}

	Array<BOOL> fetch (CREF<BOOL> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL32> fetch (CREF<VAL32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<VAL64> fetch (CREF<VAL64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT32> fetch (CREF<FLT32> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<FLT64> fetch (CREF<FLT64> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRA>> fetch (CREF<String<STRA>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRW>> fetch (CREF<String<STRW>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU8>> fetch (CREF<String<STRU8>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU16>> fetch (CREF<String<STRU16>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	Array<String<STRU32>> fetch (CREF<String<STRU32>> def ,CREF<LENGTH> size_) const override {
		return fetch_impl (def ,size_) ;
	}

	template <class ARG1>
	Array<ARG1> fetch_impl (CREF<ARG1> def ,CREF<LENGTH> size_) const {
		const auto r1x = list (size_) ;
		Array<ARG1> ret = Array<ARG1> (r1x.size ()) ;
		for (auto &&i : iter (0 ,ret.size ()))
			ret[i] = fetch (def) ;
		return move (ret) ;
	}
} ;

exports VFat<JsonParserHolder> JsonParserHolder::create (VREF<JsonParserLayout> that) {
	return VFat<JsonParserHolder> (JsonParserImplement () ,that) ;
}

exports CFat<JsonParserHolder> JsonParserHolder::create (CREF<JsonParserLayout> that) {
	return CFat<JsonParserHolder> (JsonParserImplement () ,that) ;
}

struct PlyParserProperty {
	String<STR> mName ;
	FLAG mType ;
	FLAG mListType ;
} ;

struct PlyParserElement {
	String<STR> mName ;
	LENGTH mSize ;
	ArrayList<PlyParserProperty> mPropertyList ;
	Set<String<STR>> mPropertySet ;
} ;

struct PlyParserHeader {
	String<STR> mFormat ;
	ArrayList<PlyParserElement> mElementList ;
	Set<String<STR>> mElementSet ;
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
	PlyParserHeader mHeader ;
	Array<Array<PlyParserBody>> mBody ;
	Deque<BYTE> mPlyBYTE ;
	Deque<WORD> mPlyWORD ;
	Deque<CHAR> mPlyCHAR ;
	Deque<QUAD> mPlyQUAD ;
} ;

class PlyParserImplement implement Fat<PlyParserHolder ,PlyParserLayout> {
public:
	void initialize (CREF<RefBuffer<BYTE>> stream) override {
		unimplemented () ;
	}

	INDEX find_element (CREF<Slice> name) const override {
		return fake.mThis->mHeader.mElementSet.map (name) ;
	}

	LENGTH element_size (CREF<INDEX> element) const override {
		return fake.mThis->mHeader.mElementList[element].mSize ;
	}

	INDEX find_property (CREF<INDEX> element ,CREF<Slice> name) const override {
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
	return VFat<PlyParserHolder> (PlyParserImplement () ,that) ;
}

exports CFat<PlyParserHolder> PlyParserHolder::create (CREF<PlyParserLayout> that) {
	return CFat<PlyParserHolder> (PlyParserImplement () ,that) ;
}
} ;