#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_stream.hpp"

#include "csc_end.h"
#include <cstdlib>
#include <clocale>
#include <regex>
#include <sstream>
#include "csc_begin.h"

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4996)
#endif

namespace CSC {
namespace std {
using namespace ::std ;

using ::setlocale ;
} ;
} ;

namespace CSC {
struct GapType {
	enum {
		Space ,
		Endline ,
		ETC
	} ;
} ;

struct StreamProcImplLayout {
	HashSet<STRU32> mBlankSet ;
	ArrayList<Tuple<STRU32 ,STRU32>> mEscapeList ;
	HashSet<STRU32> mEscapeWordSet ;
	HashSet<STRU32> mEscapeCtrlSet ;
} ;

class StreamProcImplHolder implement Fat<StreamProcHolder ,StreamProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<StreamProcImplLayout>::make () ;
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
		const auto r1x = WORD (0X00FF) ;
		const auto r2x = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<WORD>>>::expr] (r1x) ;
		return r2x[0] == BYTE (0X00) ;
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

	BOOL is_ctrl (CREF<STRU32> str) const override {
		return fake.mThis->mEscapeCtrlSet.contain (str) ;
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
	return VFat<StreamProcHolder> (StreamProcImplHolder () ,that) ;
}

exports CFat<StreamProcHolder> StreamProcHolder::create (CREF<StreamProcLayout> that) {
	return CFat<StreamProcHolder> (StreamProcImplHolder () ,that) ;
}

class ByteReaderImplHolder implement Fat<ByteReaderHolder ,ByteReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () == 1) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	void use_overflow (CREF<FunctionLayout> overflow) override {
		fake.mOverflow = move (keep[TYPE<Function<VREF<ByteReaderLayout>>>::expr] (overflow)) ;
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
		if ifdo (TRUE) {
			if (fake.mRead < fake.mWrite)
				discard ;
			fake.mOverflow (fake) ;
		}
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
		read_byte_impl (item) ;
	}

	void read (VREF<CHAR> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<QUAD> item) override {
		read_byte_impl (item) ;
	}

	template <class ARG1>
	void read_byte_impl (VREF<ARG1> item) {
		auto rax = Buffer<BYTE ,SIZE_OF<ARG1>> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if ((!StreamProc::big_endian ()))
				discard ;
			for (auto &&i : iter (0 ,rax.size ())) {
				INDEX ix = i ;
				read (rax[ix]) ;
			}
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,rax.size ())) {
				INDEX ix = rax.size () - 1 - i ;
				read (rax[ix]) ;
			}
		}
		item = bitwise[TYPE<ARG1>::expr] (rax) ;
	}

	void read (VREF<STRU32> item) override {
		auto rax = BYTE () ;
		read (rax) ;
		item = STRU32 (rax) ;
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
		read_string_impl (item) ;
	}

	void read (VREF<String<STRW>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU8>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU16>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU32>> item) override {
		read_string_impl (item) ;
	}

	template <class ARG1>
	void read_string_impl (VREF<String<ARG1>> item) {
		item.clear () ;
		auto rax = BYTE_BASE<ARG1> () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			item[i] = ARG1 (rax) ;
		}
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (BOM)>) override {
		noop () ;
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
} ;

exports VFat<ByteReaderHolder> ByteReaderHolder::create (VREF<ByteReaderLayout> that) {
	return VFat<ByteReaderHolder> (ByteReaderImplHolder () ,that) ;
}

exports CFat<ByteReaderHolder> ByteReaderHolder::create (CREF<ByteReaderLayout> that) {
	return CFat<ByteReaderHolder> (ByteReaderImplHolder () ,that) ;
}

class TextReaderImplHolder implement Fat<TextReaderHolder ,TextReaderLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () <= 4) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	void use_overflow (CREF<FunctionLayout> overflow) override {
		fake.mOverflow = move (keep[TYPE<Function<VREF<TextReaderLayout>>>::expr] (overflow)) ;
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
			push (rax) ;
			read (slice ("true")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('T'))
				discard ;
			push (rax) ;
			read (slice ("TRUE")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('f'))
				discard ;
			push (rax) ;
			read (slice ("false")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			if (rax != STRU32 ('F'))
				discard ;
			push (rax) ;
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
		auto rax = STRU32 () ;
		read (rax) ;
		const auto r1x = BOOL (rax == STRU32 ('-')) ;
		if ifdo (TRUE) {
			if (rax != STRU32 ('-'))
				if (rax != STRU32 ('+'))
					discard ;
			read (rax) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			assume (StreamProc::is_digit (rax)) ;
			auto rbx = Notation () ;
			rbx.mRadix = 10 ;
			rbx.mPrecision = 0 ;
			rbx.mSign = FALSE ;
			rbx.mMantissa = 0 ;
			rbx.mDownflow = 0 ;
			rbx.mExponent = 0 ;
			read_value (rbx ,rax) ;
			assume (rbx.mMantissa >= 0) ;
			item = rbx.mMantissa ;
		}
		if ifdo (TRUE) {
			if ((!r1x))
				discard ;
			item = -item ;
		}
		push (rax) ;
	}

	void read_value (VREF<Notation> fexp10 ,VREF<STRU32> top) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::value_precision () ;
		if ifdo (TRUE) {
			while (TRUE) {
				if ((!StreamProc::is_digit (top)))
					break ;
				if (fexp10.mPrecision > r1x - 1)
					break ;
				fexp10.mMantissa *= 10 ;
				fexp10.mMantissa += StreamProc::hex_from_str (top) ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			if ifdo (TRUE) {
				if ((!StreamProc::is_digit (top)))
					discard ;
				const auto r2x = fexp10.mMantissa * 10 + StreamProc::hex_from_str (top) ;
				if (r2x < 0)
					discard ;
				fexp10.mMantissa = r2x ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			while (TRUE) {
				if ((!StreamProc::is_digit (top)))
					break ;
				fexp10.mExponent++ ;
				read (top) ;
			}
		}
	}

	void read (VREF<FLT32> item) override {
		auto rax = FLT64 () ;
		read (rax) ;
		assume (rax >= FLT32_MIN) ;
		assume (rax <= FLT32_MAX) ;
		item = FLT32 (rax) ;
	}

	void read (VREF<FLT64> item) override {
		auto rax = STRU32 () ;
		read (rax) ;
		const auto r1x = BOOL (rax == STRU32 ('-')) ;
		if ifdo (TRUE) {
			if (rax != STRU32 ('-'))
				if (rax != STRU32 ('+'))
					discard ;
			read (rax) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			assume (StreamProc::is_digit (rax)) ;
			auto rbx = Notation () ;
			rbx.mRadix = 10 ;
			rbx.mPrecision = 0 ;
			rbx.mSign = FALSE ;
			rbx.mMantissa = 0 ;
			rbx.mDownflow = 0 ;
			rbx.mExponent = 0 ;
			read_float (rbx ,rax) ;
			assume (rbx.mMantissa >= 0) ;
			rbx = FloatProc::fexp2_from_fexp10 (rbx) ;
			item = FloatProc::encode (rbx) ;
		}
		if ifdo (TRUE) {
			if ((!r1x))
				discard ;
			item = -item ;
		}
		push (rax) ;
	}

	void read_float (VREF<Notation> fexp10 ,VREF<STRU32> top) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::value_precision () ;
		read_value (fexp10 ,top) ;
		if ifdo (TRUE) {
			if (top != STRU32 ('.'))
				discard ;
			read (top) ;
			while (TRUE) {
				if ((!StreamProc::is_digit (top)))
					break ;
				if (fexp10.mPrecision > r1x - 1)
					break ;
				fexp10.mMantissa *= 10 ;
				fexp10.mMantissa += StreamProc::hex_from_str (top) ;
				fexp10.mExponent-- ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			while (TRUE) {
				if ((!StreamProc::is_digit (top)))
					break ;
				read (top) ;
			}
		}
		if ifdo (TRUE) {
			if (top != STRU32 ('e'))
				if (top != STRU32 ('E'))
					discard ;
			read (top) ;
			const auto r2x = BOOL (top == STRU32 ('-')) ;
			if ifdo (TRUE) {
				if (top != STRU32 ('-'))
					if (top != STRU32 ('+'))
						discard ;
				read (top) ;
			}
			assume (StreamProc::is_digit (top)) ;
			auto rbx = Notation () ;
			rbx.mRadix = 10 ;
			rbx.mPrecision = 0 ;
			rbx.mSign = r2x ;
			read_value (rbx ,top) ;
			assume (rbx.mExponent == 0) ;
			const auto r3x = invoke ([&] () {
				if (r2x)
					return -rbx.mMantissa ;
				return rbx.mMantissa ;
			}) ;
			fexp10.mExponent += r3x ;
		}
	}

	void read (VREF<BYTE> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<WORD> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<CHAR> item) override {
		read_byte_impl (item) ;
	}

	void read (VREF<QUAD> item) override {
		read_byte_impl (item) ;
	}

	template <class ARG1>
	void read_byte_impl (VREF<ARG1> item) {
		auto rax = STRU32 () ;
		if ifdo (TRUE) {
			read (rax) ;
			assume (rax == STRU32 ('0')) ;
			read (rax) ;
			if (rax == STRU32 ('x'))
				discard ;
			if (rax == STRU32 ('X'))
				discard ;
			assume (FALSE) ;
		}
		item = ARG1 (0X00) ;
		for (auto &&i : iter (0 ,SIZE_OF<ARG1>::expr)) {
			noop (i) ;
			read (rax) ;
			const auto r1x = ARG1 (StreamProc::hex_from_str (rax)) ;
			item = (item << 4) | r1x ;
			read (rax) ;
			const auto r2x = ARG1 (StreamProc::hex_from_str (rax)) ;
			item = (item << 4) | r2x ;
		}
	}

	void read (VREF<STRU32> item) override {
		if ifdo (TRUE) {
			if (fake.mRead < fake.mWrite)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			if (fake.mRead >= fake.mWrite)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU8>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mStream.self)) ;
			item = tmp[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			if (fake.mRead >= fake.mWrite)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU16>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mStream.self)) ;
			item = tmp[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			if (fake.mRead >= fake.mWrite)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU32>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mStream.self)) ;
			item = tmp[fake.mRead] ;
			fake.mRead++ ;
		}
		if ifdo (act) {
			item = STRU32 (0X00) ;
		}
	}

	void push (CREF<STRU32> item) {
		auto act = TRUE ;
		if ifdo (act) {
			if (item != STRU32 (0X00))
				discard ;
			if (fake.mRead < fake.mWrite)
				discard ;
			noop () ;
		}
		if ifdo (act) {
			fake.mRead-- ;
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
		read_string_impl (item) ;
	}

	void read (VREF<String<STRW>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU8>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU16>> item) override {
		read_string_impl (item) ;
	}

	void read (VREF<String<STRU32>> item) override {
		read_string_impl (item) ;
	}

	template <class ARG1>
	void read_string_impl (VREF<String<ARG1>> item) {
		item.clear () ;
		auto rax = STRU32 () ;
		for (auto &&i : iter (0 ,item.size ())) {
			read (rax) ;
			item[i] = ARG1 (rax) ;
		}
	}

	void read (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void read (CREF<typeof (BOM)>) override {
		auto rax = STRU32 () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () == SIZE_OF<STRU8>::expr)
				discard ;
			read (rax) ;
			assume (rax == STRU32 (0XEF)) ;
			read (rax) ;
			assume (rax == STRU32 (0XBB)) ;
			read (rax) ;
			assume (rax == STRU32 (0XBF)) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () == SIZE_OF<STRU16>::expr)
				discard ;
			read (rax) ;
			assume (rax == STRU32 (0XFEFF)) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () == SIZE_OF<STRU32>::expr)
				discard ;
			read (rax) ;
			assume (rax == STRU32 (0X0000FEFF)) ;
		}
	}

	void read (CREF<typeof (GAP)>) override {
		auto rax = STRU32 () ;
		read (rax) ;
		while (TRUE) {
			if (rax == STRU32 (0X00))
				break ;
			if ((!StreamProc::is_blank (rax)))
				break ;
			read (rax) ;
		}
		push (rax) ;
	}

	void read (CREF<typeof (EOS)>) override {
		auto rax = STRU32 () ;
		read (rax) ;
		assume (rax == STRU32 (0X00)) ;
	}
} ;

exports VFat<TextReaderHolder> TextReaderHolder::create (VREF<TextReaderLayout> that) {
	return VFat<TextReaderHolder> (TextReaderImplHolder () ,that) ;
}

exports CFat<TextReaderHolder> TextReaderHolder::create (CREF<TextReaderLayout> that) {
	return CFat<TextReaderHolder> (TextReaderImplHolder () ,that) ;
}

class ByteWriterImplHolder implement Fat<ByteWriterHolder ,ByteWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () == 1) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	void use_overflow (CREF<FunctionLayout> overflow) override {
		fake.mOverflow = move (keep[TYPE<Function<VREF<ByteWriterLayout>>>::expr] (overflow)) ;
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
		if ifdo (TRUE) {
			if (fake.mWrite < fake.mRead)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mWrite >= fake.mRead)
				discard ;
			fake.mStream.self[fake.mWrite] = item ;
			fake.mWrite++ ;
		}
	}

	void write (CREF<WORD> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<CHAR> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<QUAD> item) override {
		write_byte_impl (item) ;
	}

	template <class ARG1>
	void write_byte_impl (CREF<ARG1> item) {
		const auto r1x = bitwise[TYPE<Buffer<BYTE ,SIZE_OF<ARG1>>>::expr] (item) ;
		auto act = TRUE ;
		if ifdo (act) {
			if ((!StreamProc::big_endian ()))
				discard ;
			for (auto &&i : iter (0 ,r1x.size ())) {
				INDEX ix = i ;
				write (r1x[ix]) ;
			}
		}
		if ifdo (act) {
			for (auto &&i : iter (0 ,r1x.size ())) {
				INDEX ix = r1x.size () - 1 - i ;
				write (r1x[ix]) ;
			}
		}
	}

	void write (CREF<STRU32> item) override {
		write (BYTE (item)) ;
	}

	void write (CREF<Slice> item) override {
		for (auto &&i : iter (0 ,item.size ())) {
			assume (inline_between (INDEX (item[i]) ,0 ,128)) ;
			write (item[i]) ;
		}
	}

	void write (CREF<String<STRA>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRW>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU8>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU16>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU32>> item) override {
		write_string_impl (item) ;
	}

	template <class ARG1>
	void write_string_impl (CREF<String<ARG1>> item) {
		const auto r1x = item.length () ;
		for (auto &&i : iter (0 ,r1x)) {
			const auto r2x = bitwise[TYPE<BYTE_BASE<ARG1>>::expr] (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CREF<typeof (CLS)>) override {
		reset () ;
	}

	void write (CREF<typeof (BOM)>) override {
		noop () ;
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
} ;

exports VFat<ByteWriterHolder> ByteWriterHolder::create (VREF<ByteWriterLayout> that) {
	return VFat<ByteWriterHolder> (ByteWriterImplHolder () ,that) ;
}

exports CFat<ByteWriterHolder> ByteWriterHolder::create (CREF<ByteWriterLayout> that) {
	return CFat<ByteWriterHolder> (ByteWriterImplHolder () ,that) ;
}

struct WriteValueBuffer {
	Buffer<STRU8 ,ENUM<64>> mBuffer ;
	INDEX mWrite ;
} ;

class TextWriterImplHolder implement Fat<TextWriterHolder ,TextWriterLayout> {
public:
	void initialize (RREF<Ref<RefBuffer<BYTE>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () <= 4) ;
		fake.mStream = move (stream) ;
		reset () ;
		backup () ;
	}

	void use_overflow (CREF<FunctionLayout> overflow) override {
		fake.mOverflow = move (keep[TYPE<Function<VREF<TextWriterLayout>>>::expr] (overflow)) ;
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
			if ((!item))
				discard ;
			write (slice ("true")) ;
		}
		if ifdo (act) {
			write (slice ("false")) ;
		}
	}

	void write (CREF<VAL32> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (STRU32 ('-')) ;
		}
		const auto r1x = VAL64 (MathProc::abs (item)) ;
		write (r1x) ;
	}

	void write (CREF<VAL64> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (STRU32 ('-')) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = FALSE ;
			rax.mMantissa = MathProc::abs (item) ;
			rax.mDownflow = 0 ;
			rax.mExponent = 0 ;
			rax.mPrecision = MathProc::log10 (rax.mMantissa) ;
			auto rbx = WriteValueBuffer () ;
			rbx.mWrite = rbx.mBuffer.size () ;
			write_value (rax ,rbx) ;
			for (auto &&i : iter (rbx.mWrite ,rbx.mBuffer.size ()))
				write (STRU32 (rbx.mBuffer[i])) ;
		}
	}

	void write_value (VREF<Notation> fexp10 ,VREF<WriteValueBuffer> wvb) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = fexp10.mPrecision ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != 0)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'xxx'
			for (auto &&i : iter (0 ,r1x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (TRUE) {
			if ((!fexp10.mSign))
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('-') ;
		}
	}

	void write (CREF<FLT32> item) override {
		const auto r1x = FLT64 (item) ;
		write (r1x) ;
	}

	void write (CREF<FLT64> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (STRU32 ('-')) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if ((!MathProc::is_inf (item)))
				discard ;
			write (slice ("infinity")) ;
		}
		if ifdo (act) {
			auto rax = FloatProc::decode (MathProc::abs (item)) ;
			rax = FloatProc::fexp10_from_fexp2 (rax) ;
			rax.mPrecision = MathProc::log10 (rax.mMantissa) ;
			auto rbx = WriteValueBuffer () ;
			rbx.mWrite = rbx.mBuffer.size () ;
			write_float (rax ,rbx) ;
			for (auto &&i : iter (rbx.mWrite ,rbx.mBuffer.size ()))
				write (STRU32 (rbx.mBuffer[i])) ;
		}
	}

	void write_float (VREF<Notation> fexp10 ,VREF<WriteValueBuffer> wvb) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::float_precision () ;
		if ifdo (TRUE) {
			if (fexp10.mPrecision == 0)
				discard ;
			while (TRUE) {
				if (fexp10.mMantissa == 0)
					break ;
				if (fexp10.mMantissa % 10 != 0)
					break ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			const auto r2x = fexp10.mPrecision - r1x ;
			for (auto &&i : iter (0 ,r2x - 1)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if (r2x <= 0)
				discard ;
			const auto r3x = MathProc::step (fexp10.mMantissa % 10 - 5) ;
			fexp10.mMantissa += r3x * 5 ;
			fexp10.mMantissa /= 10 ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
		const auto r4x = fexp10.mPrecision ;
		const auto r5x = LENGTH (fexp10.mExponent) ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != 0)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'x.xxxExxx'
			const auto r6x = r4x - 1 + r5x ;
			if (MathProc::abs (r6x) < r1x)
				discard ;
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = BOOL (r6x < 0) ;
			rax.mMantissa = MathProc::abs (r6x) ;
			rax.mDownflow = 0 ;
			rax.mExponent = 0 ;
			rax.mPrecision = MathProc::log10 (rax.mMantissa) ;
			write_value (rax ,wvb) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('E') ;
			const auto r7x = inline_max (LENGTH (r4x - 1 - r1x) ,0) ;
			for (auto &&i : iter (0 ,r7x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			INDEX ix = wvb.mWrite - 1 ;
			for (auto &&i : iter (r7x ,r4x - 1)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('.') ;
			wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('.')) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
			fexp10.mMantissa /= 10 ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
		if ifdo (act) {
			//@info: case 'xxx000'
			if (r5x < 0)
				discard ;
			for (auto &&i : iter (0 ,r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
			}
			for (auto &&i : iter (0 ,r4x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (act) {
			//@info: case 'xxx.xxx'
			if (r5x < 1 - r4x)
				discard ;
			if (r5x >= 0)
				discard ;
			const auto r8x = inline_max (LENGTH (-r5x - r1x) ,0) ;
			for (auto &&i : iter (0 ,r8x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			INDEX ix = wvb.mWrite - 1 ;
			for (auto &&i : iter (r8x ,-r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('.') ;
			wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('.')) ;
			for (auto &&i : iter (0 ,r4x + r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (act) {
			//@info: case '0.000xxx'
			if (r5x >= 1 - r4x)
				discard ;
			if (r5x >= 0)
				discard ;
			const auto r9x = inline_max (LENGTH (-r5x - r1x) ,ZERO) ;
			for (auto &&i : iter (0 ,r9x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			INDEX ix = wvb.mWrite - 1 ;
			for (auto &&i : iter (r9x ,r4x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU8 (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			const auto r10x = inline_max (r9x ,r4x) ;
			for (auto &&i : iter (r10x ,-r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
				wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('0')) ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('.') ;
			wvb.mWrite += LENGTH (wvb.mBuffer[ix] == STRU32 ('.')) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = STRU32 ('0') ;
		}
	}

	void write (CREF<BYTE> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<WORD> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<CHAR> item) override {
		write_byte_impl (item) ;
	}

	void write (CREF<QUAD> item) override {
		write_byte_impl (item) ;
	}

	template <class ARG1>
	void write_byte_impl (CREF<ARG1> item) {
		write (slice ("0X")) ;
		INDEX ix = SIZE_OF<ARG1>::expr * 8 ;
		for (auto &&i : iter (0 ,SIZE_OF<ARG1>::expr)) {
			noop (i) ;
			ix -= 4 ;
			const auto r1x = INDEX ((item >> ix) & ARG1 (0X0F)) ;
			write (StreamProc::str_from_hex (r1x)) ;
			ix -= 4 ;
			const auto r2x = INDEX ((item >> ix) & ARG1 (0X0F)) ;
			write (StreamProc::str_from_hex (r2x)) ;
		}
	}

	void write (CREF<STRU32> item) override {
		if ifdo (TRUE) {
			if (fake.mWrite < fake.mRead)
				discard ;
			fake.mOverflow (fake) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU8>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mStream.self)) ;
			tmp[fake.mWrite] = STRU8 (item) ;
			fake.mWrite++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU16>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mStream.self)) ;
			tmp[fake.mWrite] = STRU16 (item) ;
			fake.mWrite++ ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			if (fake.mWrite >= fake.mRead)
				discard ;
			auto &&tmp = keep[TYPE<RefBuffer<STRU32>>::expr] (keep[TYPE<RefBufferLayout>::expr] (fake.mStream.self)) ;
			tmp[fake.mWrite] = STRU32 (item) ;
			fake.mWrite++ ;
		}
		if ifdo (act) {
			assert (FALSE) ;
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
		write_string_impl (item) ;
	}

	void write (CREF<String<STRW>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU8>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU16>> item) override {
		write_string_impl (item) ;
	}

	void write (CREF<String<STRU32>> item) override {
		write_string_impl (item) ;
	}

	template <class ARG1>
	void write_string_impl (CREF<String<ARG1>> item) {
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
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU8>::expr)
				discard ;
			write (STRU32 (0XEF)) ;
			write (STRU32 (0XBB)) ;
			write (STRU32 (0XBF)) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU16>::expr)
				discard ;
			write (STRU32 (0XFEFF)) ;
		}
		if ifdo (act) {
			if (fake.mStream->step () != SIZE_OF<STRU32>::expr)
				discard ;
			write (STRU32 (0X0000FEFF)) ;
		}
	}

	void write (CREF<typeof (GAP)>) override {
		write (STRU32 ('\r')) ;
		write (STRU32 ('\n')) ;
	}

	void write (CREF<typeof (EOS)>) override {
		assume (fake.mWrite < fake.mRead) ;
		write (STRU32 (0X00)) ;
	}
} ;

exports VFat<TextWriterHolder> TextWriterHolder::create (VREF<TextWriterLayout> that) {
	return VFat<TextWriterHolder> (TextWriterImplHolder () ,that) ;
}

exports CFat<TextWriterHolder> TextWriterHolder::create (CREF<TextWriterLayout> that) {
	return CFat<TextWriterHolder> (TextWriterImplHolder () ,that) ;
}

class FormatImplHolder implement Fat<FormatHolder ,FormatLayout> {
public:
	void initialize (CREF<Slice> format) override {
		fake.mFormat = format ;
		fake.mTop = 0 ;
	}

	void friend_write (VREF<StreamWriter> writer) const override {
		auto rax = JustInt<FLAG> (0) ;
		for (auto &&i : iter (0 ,fake.mFormat.size ())) {
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (fake.mFormat[i] != STRU32 ('$'))
					discard ;
				rax = JustInt<FLAG> (2) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (2))
					discard ;
				if (fake.mFormat[i] != STRU32 ('{'))
					discard ;
				rax = JustInt<FLAG> (1) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (2))
					discard ;
				if ifdo (TRUE) {
					const auto r1x = StreamProc::hex_from_str (fake.mFormat[i]) ;
					if ((!inline_between (r1x ,0 ,fake.mTop)))
						discard ;
					const auto r2x = keep[TYPE<CFat<FormatFriend>>::expr] (Pointer::from (fake.mParams[r1x])) ;
					r2x->friend_write (writer) ;
				}
				rax = JustInt<FLAG> (0) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (1))
					discard ;
				if (fake.mFormat[i] != STRU32 ('*'))
					discard ;
				for (auto &&j : iter (0 ,fake.mTop)) {
					const auto r3x = keep[TYPE<CFat<FormatFriend>>::expr] (Pointer::from (fake.mParams[j])) ;
					r3x->friend_write (writer) ;
				}
				rax = JustInt<FLAG> (3) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (1))
					discard ;
				if ifdo (TRUE) {
					const auto r4x = StreamProc::hex_from_str (fake.mFormat[i]) ;
					if ((!inline_between (r4x ,0 ,fake.mTop)))
						discard ;
					const auto r5x = keep[TYPE<CFat<FormatFriend>>::expr] (Pointer::from (fake.mParams[r4x])) ;
					r5x->friend_write (writer) ;
				}
				rax = JustInt<FLAG> (3) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (3))
					discard ;
				assert (fake.mFormat[i] == STRU32 ('}')) ;
				rax = JustInt<FLAG> (0) ;
			}
			if ifdo (act) {
				assume (rax == JustInt<FLAG> (0)) ;
				writer.write (fake.mFormat[i]) ;
			}
		}
	}

	void then (CREF<WrapperLayout> params) override {
		fake.mTop = 0 ;
		for (auto &&i : WrapperIterator<FatLayout> (params)) {
			fake.mParams[fake.mTop] = i ;
			fake.mTop++ ;
		}
	}
} ;

exports VFat<FormatHolder> FormatHolder::create (VREF<FormatLayout> that) {
	return VFat<FormatHolder> (FormatImplHolder () ,that) ;
}

exports CFat<FormatHolder> FormatHolder::create (CREF<FormatLayout> that) {
	return CFat<FormatHolder> (FormatImplHolder () ,that) ;
}

template <class A>
struct FUNCTION_string_from_impl {
	template <class ARG1>
	forceinline CREF<String<A>> operator() (CREF<String<ARG1>> that) {
		assert (IS_SAME<A ,ARG1>::expr) ;
		return keep[TYPE<String<A>>::expr] (Pointer::from (that)) ;
	}

	template <class ARG1>
	forceinline RREF<String<A>> operator() (RREF<String<ARG1>> that) {
		assert (IS_SAME<A ,ARG1>::expr) ;
		return move (keep[TYPE<String<A>>::expr] (Pointer::from (that))) ;
	}
} ;

struct FUNCTION_string_from {
	template <class ARG1>
	forceinline consteval FUNCTION_string_from_impl<ARG1> operator[] (TYPE<ARG1>) const noexcept {
		return FUNCTION_string_from_impl<ARG1> () ;
	}
} ;

static constexpr auto string_from = FUNCTION_string_from () ;

class StringProcImplHolder implement Fat<StringProcHolder ,StringProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	String<STRA> stra_from_strw (CREF<String<STRW>> a) const override {
		String<STRA> ret = String<STRA> (a.length () * 2 + 1) ;
		std::setlocale (LC_CTYPE ,String<STRA>::zero ()) ;
		const auto r1x = (ret.size () + 1) * ret.step () ;
		const auto r2x = std::wcstombs (ret ,a ,r1x) ;
		assume (r2x >= 0) ;
		return move (ret) ;
	}

	String<STRA> stra_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STRA>::expr] (a.clone ()) ;
		if (IS_SAME<STR ,STRW>::expr)
			return stra_from_strw (string_from[TYPE<STRW>::expr] (a)) ;
		assert (FALSE) ;
		return String<STRA> () ;
	}

	String<STRW> strw_from_stra (CREF<String<STRA>> a) const override {
		String<STRW> ret = String<STRW> (a.length () + 1) ;
		std::setlocale (LC_CTYPE ,String<STRA>::zero ()) ;
		const auto r1x = (ret.size () + 1) * ret.step () ;
		const auto r2x = std::mbstowcs (ret ,a ,r1x) ;
		assume (r2x >= 0) ;
		return move (ret) ;
	}

	String<STRW> strw_from_strs (CREF<String<STR>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return strw_from_stra (string_from[TYPE<STRA>::expr] (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STRW>::expr] (a.clone ()) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stra (CREF<String<STRA>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (a.clone ()) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stra (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_strw (CREF<String<STRW>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_strw (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (a.clone ()) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STRU8> stru8_from_stru16 (CREF<String<STRU16>> a) const override {
		String<STRU8> ret = String<STRU8> (a.length () * 3) ;
		INDEX ix = 0 ;
		auto rax = JustInt<FLAG> (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == JustInt<FLAG> (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU16 (0X007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU16 (0X07FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] < STRU16 (0XD800))
					discard ;
				if (a[i] > STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = JustInt<FLAG> (1) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (1))
					discard ;
				if (a[i] < STRU16 (0XDC00))
					discard ;
				if (a[i] > STRU16 (0XDFFF))
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
				rax = JustInt<FLAG> (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = JustInt<FLAG> (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == JustInt<FLAG> (0))
				discard ;
			ret[ix] = STRU8 (0X3F) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU8> stru8_from_stru32 (CREF<String<STRU32>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<STRU8> ret = String<STRU8> (a.length () * 6) ;
		INDEX ix = 0 ;
		auto rax = JustInt<FLAG> (0) ;
		for (auto &&i : a.range ()) {
			if (rax == JustInt<FLAG> (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X0000007F))
					discard ;
				ret[ix] = STRU8 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X000007FF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X1F)) | STRU8 (0XC0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = (STRU8 (a[i] >> 12) & STRU8 (0X0F)) | STRU8 (0XE0) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i] >> 6) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
				ret[ix] = (STRU8 (a[i]) & STRU8 (0X3F)) | STRU8 (0X80) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X001FFFFF))
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
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X03FFFFFF))
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
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X7FFFFFFF))
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
				rax = JustInt<FLAG> (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU16> stru16_from_stru8 (CREF<String<STRU8>> a) const override {
		String<STRU16> ret = String<STRU16> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = JustInt<FLAG> (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == JustInt<FLAG> (99))
				continue ;
			if ifdo (TRUE) {
				auto act = TRUE ;
				if ifdo (act) {
					if (rax != JustInt<FLAG> (0))
						discard ;
					if (a[i] > STRU8 (0X7F))
						discard ;
					ret[ix] = STRU16 (a[i]) ;
					ix++ ;
				}
				if ifdo (act) {
					if (rax != JustInt<FLAG> (0))
						discard ;
					if (a[i] > STRU8 (0XDF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
					rax = JustInt<FLAG> (1) ;
				}
				if ifdo (act) {
					if (rax != JustInt<FLAG> (0))
						discard ;
					if (a[i] > STRU8 (0XEF))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
					rax = JustInt<FLAG> (2) ;
				}
				if ifdo (act) {
					if (rax != JustInt<FLAG> (0))
						discard ;
					if (a[i] > STRU8 (0XF7))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X07)) ;
					rax = JustInt<FLAG> (3) ;
				}
				if ifdo (act) {
					if (rax != JustInt<FLAG> (0))
						discard ;
					if (a[i] > STRU8 (0XFB))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X03)) ;
					rax = JustInt<FLAG> (4) ;
				}
				if ifdo (act) {
					if (rax != JustInt<FLAG> (0))
						discard ;
					if (a[i] > STRU8 (0XFD))
						discard ;
					rbx = STRU32 (a[i] & STRU8 (0X01)) ;
					rax = JustInt<FLAG> (5) ;
				}
				if ifdo (act) {
					if (rax != JustInt<FLAG> (1))
						discard ;
					if (a[i] > STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax = JustInt<FLAG> (10) ;
				}
				if ifdo (act) {
					if ((!inline_between (rax ,2 ,6)))
						discard ;
					if (a[i] > STRU8 (0XBF))
						discard ;
					rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
					rax-- ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = JustInt<FLAG> (99) ;
				}
			}
			if ifdo (TRUE) {
				if (rax != JustInt<FLAG> (10))
					discard ;
				auto act = TRUE ;
				if ifdo (act) {
					if (rbx > STRU32 (0X0000FFFF))
						discard ;
					ret[ix] = STRU16 (rbx) ;
					ix++ ;
					rax = JustInt<FLAG> (0) ;
				}
				if ifdo (act) {
					if (rbx > STRU32 (0X0010FFFF))
						discard ;
					rbx = STRU32 (rbx - STRU32 (0X00010000)) ;
					ret[ix] = (STRU16 (rbx >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
					ix++ ;
					ret[ix] = (STRU16 (rbx) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
					ix++ ;
					rax = JustInt<FLAG> (0) ;
				}
				if ifdo (act) {
					if (rbx > STRU32 (0X7FFFFFFF))
						discard ;
					ret[ix] = STRU16 (0X3F) ;
					ix++ ;
					rax = JustInt<FLAG> (0) ;
				}
				if ifdo (act) {
					ret.clear () ;
					rax = JustInt<FLAG> (99) ;
				}
			}
		}
		if ifdo (TRUE) {
			if (rax == JustInt<FLAG> (0))
				discard ;
			ret[ix] = STRU16 (0X3F) ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU16> stru16_from_stru32 (CREF<String<STRU32>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<STRU16> ret = String<STRU16> (a.length () * 2) ;
		INDEX ix = 0 ;
		auto rax = JustInt<FLAG> (0) ;
		for (auto &&i : a.range ()) {
			if (rax == JustInt<FLAG> (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X0000FFFF))
					discard ;
				ret[ix] = STRU16 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X0010FFFF))
					discard ;
				ret[ix] = (STRU16 ((a[i] - STRU32 (0X00010000)) >> 10) & STRU16 (0X03FF)) | STRU16 (0XD800) ;
				ix++ ;
				ret[ix] = (STRU16 (a[i] - STRU32 (0X00010000)) & STRU16 (0X03FF)) | STRU16 (0XDC00) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU32 (0X7FFFFFFF))
					discard ;
				ret[ix] = STRU16 (0X3F) ;
				ix++ ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = JustInt<FLAG> (99) ;
			}
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU32> stru32_from_stru8 (CREF<String<STRU8>> a) const override {
		//@info: 1 bytes [0,0X7F] 0xxxxxxx
		//@info: 2 bytes [0x80,0X7FF] 110xxxxx 10xxxxxx
		//@info: 3 bytes [0x800,0XFFFF] 1110xxxx 10xxxxxx 10xxxxxx
		//@info: 4 bytes [0x10000,0X1FFFFF] 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 5 bytes [0x200000,0X3FFFFFF] 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		//@info: 6 bytes [0x4000000,0X7FFFFFFF] 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		String<STRU32> ret = String<STRU32> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = JustInt<FLAG> (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == JustInt<FLAG> (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU8 (0X7F))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU8 (0XDF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X1F)) ;
				rax = JustInt<FLAG> (1) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU8 (0XEF))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X0F)) ;
				rax = JustInt<FLAG> (2) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU8 (0XF7))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X07)) ;
				rax = JustInt<FLAG> (3) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU8 (0XFB))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X03)) ;
				rax = JustInt<FLAG> (4) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU8 (0XFD))
					discard ;
				rbx = STRU32 (a[i] & STRU8 (0X01)) ;
				rax = JustInt<FLAG> (5) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (1))
					discard ;
				if (a[i] > STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = JustInt<FLAG> (0) ;
			}
			if ifdo (act) {
				if ((!inline_between (rax ,2 ,6)))
					discard ;
				if (a[i] > STRU8 (0XBF))
					discard ;
				rbx = STRU32 ((rbx << 6) | (a[i] & STRU8 (0X3F))) ;
				rax-- ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = JustInt<FLAG> (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == JustInt<FLAG> (0))
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRU32> stru32_from_stru16 (CREF<String<STRU16>> a) const override {
		//@info: utf16 [D800,DBFF] 110110xx xxxxxxxx [DC00,DFFF] 110111xx xxxxxxxx
		//@info: utf32 [0X10000,0X10FFFF]-[0,0XFFFFF] 0000xxxx xxxxxxxx xxxxxxxx
		String<STRU32> ret = String<STRU32> (a.length ()) ;
		INDEX ix = 0 ;
		auto rax = JustInt<FLAG> (0) ;
		auto rbx = STRU32 () ;
		for (auto &&i : a.range ()) {
			if (rax == JustInt<FLAG> (99))
				continue ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] > STRU16 (0X07FF))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				if (a[i] < STRU16 (0XD800))
					discard ;
				if (a[i] > STRU16 (0XDBFF))
					discard ;
				rbx = STRU32 (a[i] & STRU16 (0X03FF)) ;
				rax = JustInt<FLAG> (1) ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (0))
					discard ;
				ret[ix] = STRU32 (a[i]) ;
				ix++ ;
			}
			if ifdo (act) {
				if (rax != JustInt<FLAG> (1))
					discard ;
				if (a[i] < STRU16 (0XDC00))
					discard ;
				if (a[i] > STRU16 (0XDFFF))
					discard ;
				rbx = STRU32 (((rbx << 10) | (a[i] & STRU16 (0X03FF))) + STRU32 (0X00010000)) ;
				ret[ix] = rbx ;
				ix++ ;
				rax = JustInt<FLAG> (0) ;
			}
			if ifdo (act) {
				ret.clear () ;
				rax = JustInt<FLAG> (99) ;
			}
		}
		if ifdo (TRUE) {
			if (rax == JustInt<FLAG> (0))
				discard ;
			ret[ix] = STRU32 ('?') ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STRUA> strua_from_stra (RREF<String<STRA>> a) const override {
		return move (keep[TYPE<String<STRUA>>::expr] (keep[TYPE<StringLayout>::expr] (a))) ;
	}

	String<STRA> stra_from_strua (RREF<String<STRUA>> a) const override {
		return move (keep[TYPE<String<STRA>>::expr] (keep[TYPE<StringLayout>::expr] (a))) ;
	}

	String<STRUW> struw_from_strw (RREF<String<STRW>> a) const override {
		return move (keep[TYPE<String<STRUW>>::expr] (keep[TYPE<StringLayout>::expr] (a))) ;
	}

	String<STRW> strw_from_struw (RREF<String<STRUW>> a) const override {
		return move (keep[TYPE<String<STRW>>::expr] (keep[TYPE<StringLayout>::expr] (a))) ;
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
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (a.clone ())) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru8 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru8 (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru16 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (a.clone ())) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru32_from_stru16 (a))) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STRW> strw_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STRUW ,STRU8>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru8_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU16>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (stru16_from_stru32 (a))) ;
		if (IS_SAME<STRUW ,STRU32>::expr)
			return strw_from_struw (string_from[TYPE<STRUW>::expr] (a.clone ())) ;
		assert (FALSE) ;
		return String<STRW> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU8>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU16>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}

	String<STR> strs_from_stru (CREF<String<STRU32>> a) const override {
		if (IS_SAME<STR ,STRA>::expr)
			return string_from[TYPE<STR>::expr] (stra_from_stru (a)) ;
		if (IS_SAME<STR ,STRW>::expr)
			return string_from[TYPE<STR>::expr] (strw_from_stru (a)) ;
		assert (FALSE) ;
		return String<STR> () ;
	}
} ;

exports VFat<StringProcHolder> StringProcHolder::create (VREF<StringProcLayout> that) {
	return VFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

exports CFat<StringProcHolder> StringProcHolder::create (CREF<StringProcLayout> that) {
	return CFat<StringProcHolder> (StringProcImplHolder () ,that) ;
}

struct RegexImplLayout {
	std::basic_regex<STR> mRegex ;
	std::match_results<PTR<CREF<STR>>> mMatch ;
} ;

class RegexImplHolder implement Fat<RegexHolder ,RegexLayout> {
public:
	void initialize (CREF<String<STR>> format) override {
		fake.mThis = Ref<RegexImplLayout>::make () ;
		fake.mThis->mRegex = std::basic_regex<STR> (format) ;
	}

	BOOL search (CREF<String<STR>> text) override {
		const auto r1x = text.self ;
		const auto r2x = std::regex_search (r1x ,fake.mThis->mMatch ,fake.mThis->mRegex) ;
		if ((!r2x))
			return FALSE ;
		return TRUE ;
	}

	String<STR> brace (CREF<INDEX> index) const override {
		assert ((!fake.mThis->mMatch.empty ())) ;
		INDEX ix = index + 1 ;
		const auto r1x = FLAG (fake.mThis->mMatch[ix].first) ;
		const auto r2x = FLAG (fake.mThis->mMatch[ix].second) ;
		const auto r3x = (r2x - r1x) / SIZE_OF<STR>::expr ;
		const auto r4x = Slice (r1x ,r3x ,SIZE_OF<STR>::expr) ;
		return String<STR> (r4x) ;
	}
} ;

exports VFat<RegexHolder> RegexHolder::create (VREF<RegexLayout> that) {
	return VFat<RegexHolder> (RegexImplHolder () ,that) ;
}

exports CFat<RegexHolder> RegexHolder::create (CREF<RegexLayout> that) {
	return CFat<RegexHolder> (RegexImplHolder () ,that) ;
}

class StreamTextProcImplHolder implement Fat<StreamTextProcHolder ,StreamTextProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void read_identifer (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		if ifdo (TRUE) {
			reader.backup () ;
			reader.read (rax) ;
			while (TRUE) {
				if ((!StreamProc::is_word (rax)))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
			reader.recover () ;
		}
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void read_scalar (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		if ifdo (TRUE) {
			reader.backup () ;
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('+'))
					if (rax != STRU32 ('-'))
						discard ;
				rbx++ ;
				reader.read (rax) ;
			}
			while (TRUE) {
				if ((!StreamProc::is_digit (rax)))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
			if (rax != STRU32 ('E'))
				if (rax != STRU32 ('e'))
					discard ;
			rbx++ ;
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('+'))
					if (rax != STRU32 ('-'))
						discard ;
				rbx++ ;
				reader.read (rax) ;
			}
			while (TRUE) {
				if ((!StreamProc::is_digit (rax)))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
			reader.recover () ;
		}
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void read_escape (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		if ifdo (TRUE) {
			reader.backup () ;
			reader.read (rax) ;
			assume (rax == STRU32 ('"')) ;
			reader.read (rax) ;
			while (TRUE) {
				if (rax == STRU32 (0X00))
					break ;
				if (rax == STRU32 ('"'))
					break ;
				if ifdo (TRUE) {
					if (rax != STRU32 ('\\'))
						discard ;
					reader.read (rax) ;
				}
				rbx++ ;
				reader.read (rax) ;
			}
			assume (rax == STRU32 ('"')) ;
			reader.recover () ;
		}
		item = String<STRU8> (rbx) ;
		INDEX ix = 0 ;
		reader.read (rax) ;
		while (TRUE) {
			if (rax == STRU32 ('"'))
				break ;
			if ifdo (TRUE) {
				if (rax != STRU32 ('\\'))
					discard ;
				reader.read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[ix] = STRU8 (rax) ;
			ix++ ;
			reader.read (rax) ;
		}
	}

	void write_escape (VREF<StreamWriter> writer ,CREF<String<STRU8>> item) const override {
		writer.write (STRU32 ('"')) ;
		for (auto &&i : item) {
			auto act = TRUE ;
			if ifdo (act) {
				if ((!StreamProc::is_ctrl (i)))
					discard ;
				const auto r1x = StreamProc::word_from_ctrl (i) ;
				writer.write (STRU32 ('\\')) ;
				writer.write (r1x) ;
			}
			if ifdo (act) {
				writer.write (STRU32 (i)) ;
			}
		}
		writer.write (STRU32 ('"')) ;
	}

	void read_endline (VREF<StreamReader> reader ,VREF<String<STRU8>> item) const override {
		auto rax = STRU32 () ;
		auto rbx = ZERO ;
		if ifdo (TRUE) {
			reader.backup () ;
			reader.read (rax) ;
			while (TRUE) {
				if (rax == STRU32 (0X00))
					break ;
				if (StreamProc::is_endline (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
			reader.recover () ;
		}
		item = String<STRU8> (rbx) ;
		reader.read (item) ;
	}

	void write_aligned (VREF<StreamWriter> writer ,CREF<VAL64> number ,CREF<LENGTH> align) const override {
		auto rax = LENGTH (1) ;
		for (auto &&i : iter (0 ,align - 1)) {
			noop (i) ;
			rax *= 10 ;
			if (number >= rax)
				continue ;
			writer.write (STRU32 ('0')) ;
		}
		rax *= 10 ;
		const auto r1x = number % rax ;
		writer.write (r1x) ;
	}
} ;

exports VFat<StreamTextProcHolder> StreamTextProcHolder::create (VREF<StreamTextProcLayout> that) {
	return VFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}

exports CFat<StreamTextProcHolder> StreamTextProcHolder::create (CREF<StreamTextProcLayout> that) {
	return CFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}
} ;