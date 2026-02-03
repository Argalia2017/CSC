#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_stream.hpp"

#include "csc_end.h"
#include <regex>
#include "csc_begin.h"

namespace CSC {
struct StreamProcLayout {
	Slice mBlankSlice ;
	Slice mPunctSlice ;
	Slice mAlphaSlice ;
	Slice mDigitSlice ;
	Slice mEscapeWordSlice ;
	Slice mEscapeCtrlSlice ;
} ;

class StreamProcImplHolder final implement Fat<StreamProcHolder ,StreamProcLayout> {
public:
	void initialize () override {
		self.mBlankSlice = slice ("\b\t\n\v\f\r ") ;
		self.mPunctSlice = slice ("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~") ;
		self.mAlphaSlice = slice ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") ;
		self.mDigitSlice = slice ("0123456789") ;
		self.mEscapeWordSlice = slice ("\\/tvbrnf\'\"?u") ;
		self.mEscapeCtrlSlice = slice ("\\/\t\v\b\r\n\f\'\"?\a") ;
	}

	Bool big_endian () const override {
		const auto r1x = QUAD_ENDIAN ;
		const auto r2x = Buffer<Char ,RANK2> (bitwise (r1x)) ;
		return r2x[0] != CHAR_ENDIAN ;
	}

	Bool is_blank (CR<Stru32> str) const override {
		const auto r1x = self.mBlankSlice ;
		for (auto &&i : range (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return TRUE ;
		}
		return FALSE ;
	}

	Bool is_space (CR<Stru32> str) const override {
		if (str == Stru32 (' '))
			return TRUE ;
		if (str == Stru32 ('\t'))
			return TRUE ;
		if (str == Stru32 ('\b'))
			return TRUE ;
		return FALSE ;
	}

	Bool is_endline (CR<Stru32> str) const override {
		if (str == Stru32 ('\r'))
			return TRUE ;
		if (str == Stru32 ('\n'))
			return TRUE ;
		if (str == Stru32 ('\v'))
			return TRUE ;
		if (str == Stru32 ('\f'))
			return TRUE ;
		return FALSE ;
	}

	Bool is_punct (CR<Stru32> str) const override {
		const auto r1x = self.mPunctSlice ;
		for (auto &&i : range (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return TRUE ;
		}
		return FALSE ;
	}

	Bool is_hyphen (CR<Stru32> str) const override {
		if (str == Stru32 ('-'))
			return TRUE ;
		if (str == Stru32 (':'))
			return TRUE ;
		if (str == Stru32 ('.'))
			return TRUE ;
		return FALSE ;
	}

	Bool is_alpha (CR<Stru32> str) const override {
		if (str >= Stru32 ('a'))
			if (str <= Stru32 ('z'))
				return TRUE ;
		if (str >= Stru32 ('A'))
			if (str <= Stru32 ('Z'))
				return TRUE ;
		return FALSE ;
	}

	Stru32 alpha_lower (CR<Stru32> str) const override {
		if (str >= Stru32 ('A'))
			if (str <= Stru32 ('Z'))
				return str - Stru32 ('A') + Stru32 ('a') ;
		return str ;
	}

	Stru32 alpha_upper (CR<Stru32> str) const override {
		if (str >= Stru32 ('a'))
			if (str <= Stru32 ('z'))
				return str - Stru32 ('a') + Stru32 ('A') ;
		return str ;
	}

	Bool is_digit (CR<Stru32> str) const override {
		if (str >= Stru32 ('0'))
			if (str <= Stru32 ('9'))
				return TRUE ;
		return FALSE ;
	}

	Bool is_hex_digit (CR<Stru32> str) const override {
		if (str >= Stru32 ('a'))
			if (str <= Stru32 ('f'))
				return TRUE ;
		if (str >= Stru32 ('A'))
			if (str <= Stru32 ('F'))
				return TRUE ;
		return FALSE ;
	}

	Index hex_from_str (CR<Stru32> str) const override {
		if (is_digit (str))
			return Index (str - Stru32 ('0')) ;
		if (is_hex_digit (str))
			return Index (alpha_upper (str) - Stru32 ('A') + 10) ;
		assume (FALSE) ;
		return Index () ;
	}

	Stru32 str_from_hex (CR<Index> hex) const override {
		if (inline_between (hex ,0 ,10))
			return Stru32 ('0') + Stru32 (hex) ;
		if (inline_between (hex ,10 ,16))
			return Stru32 ('A') + Stru32 (hex) - 10 ;
		assume (FALSE) ;
		return Stru32 () ;
	}

	Bool is_word (CR<Stru32> str) const override {
		if (is_alpha (str))
			return TRUE ;
		if (is_digit (str))
			return TRUE ;
		if (str == Stru32 ('_'))
			return TRUE ;
		return FALSE ;
	}

	Bool is_ctrl (CR<Stru32> str) const override {
		const auto r1x = self.mEscapeCtrlSlice ;
		for (auto &&i : range (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return TRUE ;
		}
		return FALSE ;
	}

	Stru32 word_from_ctrl (CR<Stru32> str) const override {
		const auto r1x = self.mEscapeWordSlice ;
		const auto r2x = self.mEscapeCtrlSlice ;
		for (auto &&i : range (0 ,r2x.size ())) {
			if (r2x[i] == str)
				return r1x[i] ;
		}
		assume (FALSE) ;
		return Stru32 ('?') ;
	}

	Stru32 ctrl_from_word (CR<Stru32> str) const override {
		const auto r1x = self.mEscapeWordSlice ;
		const auto r2x = self.mEscapeCtrlSlice ;
		for (auto &&i : range (0 ,r1x.size ())) {
			if (r1x[i] == str)
				return r2x[i] ;
		}
		assume (FALSE) ;
		return Stru32 ('?') ;
	}
} ;

exports CR<Super<Ref<StreamProcLayout>>> StreamProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<StreamProcLayout>> ret ;
		ret.mThis = Ref<StreamProcLayout>::make () ;
		StreamProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StreamProcHolder> StreamProcHolder::hold (VR<StreamProcLayout> that) {
	return VFat<StreamProcHolder> (StreamProcImplHolder () ,that) ;
}

exports CFat<StreamProcHolder> StreamProcHolder::hold (CR<StreamProcLayout> that) {
	return CFat<StreamProcHolder> (StreamProcImplHolder () ,that) ;
}

class ByteReaderImplHolder final implement Fat<ByteReaderHolder ,ByteReaderLayout> {
public:
	void initialize (RR<Ref<RefBuffer<Byte>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () == 1) ;
		self.mStream = move (stream) ;
		self.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_cats (CR<Array<Slice>> cats) override {
		self.mCats = cats ;
		self.mCatIndex = 0 ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mStream->size () ;
	}

	Length length () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mRead ;
	}

	StreamShape shape () const override {
		StreamShape ret ;
		ret.mRead = self.mRead ;
		ret.mWrite = self.mWrite ;
		return move (ret) ;
	}

	Bool good () const override {
		return length () < size () ;
	}

	void reset () override {
		self.mRead = 0 ;
		self.mWrite = self.mStream->size () ;
	}

	void reset (CR<StreamShape> shape) override {
		self.mRead = shape.mRead ;
		self.mWrite = shape.mWrite ;
	}

	void read (VR<Bool> item) override {
		auto rax = BYTE_BASE<Bool> () ;
		read (rax) ;
		item = bitwise (rax) ;
	}

	void read (VR<Val32> item) override {
		auto rax = BYTE_BASE<Val32> () ;
		read (rax) ;
		item = bitwise (rax) ;
	}

	void read (VR<Val64> item) override {
		auto rax = BYTE_BASE<Val64> () ;
		read (rax) ;
		item = bitwise (rax) ;
	}

	void read (VR<Flt32> item) override {
		auto rax = BYTE_BASE<Flt32> () ;
		read (rax) ;
		item = bitwise (rax) ;
	}

	void read (VR<Flt64> item) override {
		auto rax = BYTE_BASE<Flt64> () ;
		read (rax) ;
		item = bitwise (rax) ;
	}

	void read (VR<Byte> item) override {
		if ifdo (TRUE) {
			if (self.mRead < self.mWrite)
				discard ;
			self.mOverflow (Pointer::from (thiz)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mRead >= self.mWrite)
				discard ;
			item = self.mStream.ref[self.mRead] ;
			self.mRead++ ;
		}
		if ifdo (act) {
			item = Byte (0X00) ;
		}
	}

	void read (VR<Word> item) override {
		read_byte_impl (item) ;
	}

	void read (VR<Char> item) override {
		read_byte_impl (item) ;
	}

	void read (VR<Quad> item) override {
		read_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void read_byte_impl (VR<ARG1> item) {
		auto rax = Buffer<Byte ,SIZE_OF<ARG1>> () ;
		for (auto &&i : range (0 ,rax.size ())) {
			read (rax[i]) ;
		}
		item = bitwise (rax) ;
		if ifdo (TRUE) {
			if (!self.mDiffEndian)
				discard ;
			item = ByteProc::reverse (item) ;
		}
	}

	void read (VR<Stru32> item) override {
		item = 0 ;
		auto rax = Byte () ;
		Index ix = 0 ;
		while (TRUE) {
			read (rax) ;
			const auto r1x = Stru32 (rax) ;
			if (r1x < Stru32 (0X80))
				break ;
			item |= Stru32 (rax & Byte (0X7F)) << ix ;
			ix += 7 ;
		}
		item |= Stru32 (rax) << ix ;
	}

	void read (CR<Slice> item) override {
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,item.size ())) {
			assume (inline_between (Index (item[i]) ,0 ,128)) ;
			read (rax) ;
			assume (rax == item[i]) ;
		}
	}

	void read (VR<StringLayout> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru>>::expr] (item) ;
			if (rax.step () != 1)
				discard ;
			read_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru16>>::expr] (item) ;
			if (rax.step () != 2)
				discard ;
			read_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru32>>::expr] (item) ;
			if (rax.step () != 4)
				discard ;
			read_string_impl (rax) ;
		}
	}

	template <class ARG1>
	forceinline void read_string_impl (VR<String<ARG1>> item) {
		item.clear () ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,item.size ())) {
			read (rax) ;
			item[i] = ARG1 (rax) ;
		}
	}

	void read (CR<typeof (BOM)>) override {
		self.mDiffEndian = !self.mDiffEndian ;
	}

	void read (CR<typeof (CAT)>) override {
		if (self.mCats.size () == 0)
			return ;
		read (self.mCats[self.mCatIndex]) ;
		self.mCatIndex = (self.mCatIndex + 1) % self.mCats.size () ;
	}

	void read (CR<typeof (GAP)>) override {
		auto rax = Byte () ;
		read (rax) ;
		assume (rax == Byte (0X5D)) ;
		read (rax) ;
		assume (rax == Byte (0X5B)) ;
	}

	void read (CR<typeof (EOS)>) override {
		auto rax = Byte () ;
		while (TRUE) {
			if (self.mRead >= self.mWrite)
				break ;
			read (rax) ;
			assume (rax == Byte (0X00)) ;
		}
	}
} ;

exports VFat<ByteReaderHolder> ByteReaderHolder::hold (VR<ByteReaderLayout> that) {
	return VFat<ByteReaderHolder> (ByteReaderImplHolder () ,that) ;
}

exports CFat<ByteReaderHolder> ByteReaderHolder::hold (CR<ByteReaderLayout> that) {
	return CFat<ByteReaderHolder> (ByteReaderImplHolder () ,that) ;
}

class TextReaderImplHolder final implement Fat<TextReaderHolder ,TextReaderLayout> {
public:
	void initialize (RR<Ref<RefBuffer<Byte>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () <= 4) ;
		self.mStream = move (stream) ;
		self.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_cats (CR<Array<Slice>> cats) override {
		self.mCats = cats ;
		self.mCatIndex = 0 ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mStream->size () ;
	}

	Length length () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mRead ;
	}

	StreamShape shape () const override {
		StreamShape ret ;
		ret.mRead = self.mRead ;
		ret.mWrite = self.mWrite ;
		return move (ret) ;
	}

	Bool good () const override {
		return length () < size () ;
	}

	void reset () override {
		self.mRead = 0 ;
		self.mWrite = self.mStream->size () ;
	}

	void reset (CR<StreamShape> shape) override {
		self.mRead = shape.mRead ;
		self.mWrite = shape.mWrite ;
	}

	void read (VR<Bool> item) override {
		auto rax = Stru32 () ;
		read (rax) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (rax != Stru32 ('t'))
				discard ;
			push (rax) ;
			read (slice ("true")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != Stru32 ('T'))
				discard ;
			push (rax) ;
			read (slice ("TRUE")) ;
			item = TRUE ;
		}
		if ifdo (act) {
			if (rax != Stru32 ('f'))
				discard ;
			push (rax) ;
			read (slice ("false")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			if (rax != Stru32 ('F'))
				discard ;
			push (rax) ;
			read (slice ("FALSE")) ;
			item = FALSE ;
		}
		if ifdo (act) {
			assume (FALSE) ;
		}
	}

	void read (VR<Val32> item) override {
		auto rax = Val64 () ;
		read (rax) ;
		assume (rax >= VAL32_MIN) ;
		assume (rax <= VAL32_MAX) ;
		item = Val32 (rax) ;
	}

	void read (VR<Val64> item) override {
		auto rax = Stru32 () ;
		read (rax) ;
		const auto r1x = Bool (rax == Stru32 ('-')) ;
		if ifdo (TRUE) {
			if (rax != Stru32 ('-'))
				if (rax != Stru32 ('+'))
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
			if (!r1x)
				discard ;
			item = -item ;
		}
		push (rax) ;
	}

	void read_value (VR<Notation> fexp10 ,VR<Stru32> top) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::value_precision () ;
		if ifdo (TRUE) {
			while (TRUE) {
				if (!StreamProc::is_digit (top))
					break ;
				if (fexp10.mPrecision > r1x - 1)
					break ;
				fexp10.mMantissa *= 10 ;
				fexp10.mMantissa += StreamProc::hex_from_str (top) ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			if ifdo (TRUE) {
				if (!StreamProc::is_digit (top))
					discard ;
				const auto r2x = fexp10.mMantissa * 10 + StreamProc::hex_from_str (top) ;
				if (r2x < 0)
					discard ;
				fexp10.mMantissa = r2x ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (top))
					break ;
				fexp10.mExponent++ ;
				read (top) ;
			}
		}
	}

	void read (VR<Flt32> item) override {
		auto rax = Flt64 () ;
		read (rax) ;
		assume (rax >= FLT32_MIN) ;
		assume (rax <= FLT32_MAX) ;
		item = Flt32 (rax) ;
	}

	void read (VR<Flt64> item) override {
		auto rax = Stru32 () ;
		read (rax) ;
		const auto r1x = Bool (rax == Stru32 ('-')) ;
		if ifdo (TRUE) {
			if (rax != Stru32 ('-'))
				if (rax != Stru32 ('+'))
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
			if (!r1x)
				discard ;
			item = -item ;
		}
		push (rax) ;
	}

	void read_float (VR<Notation> fexp10 ,VR<Stru32> top) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = FloatProc::value_precision () ;
		read_value (fexp10 ,top) ;
		if ifdo (TRUE) {
			if (top != Stru32 ('.'))
				discard ;
			read (top) ;
			while (TRUE) {
				if (!StreamProc::is_digit (top))
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
				if (!StreamProc::is_digit (top))
					break ;
				read (top) ;
			}
		}
		if ifdo (TRUE) {
			if (top != Stru32 ('e'))
				if (top != Stru32 ('E'))
					discard ;
			read (top) ;
			const auto r2x = Bool (top == Stru32 ('-')) ;
			if ifdo (TRUE) {
				if (top != Stru32 ('-'))
					if (top != Stru32 ('+'))
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

	void read (VR<Byte> item) override {
		read_byte_impl (item) ;
	}

	void read (VR<Word> item) override {
		read_byte_impl (item) ;
	}

	void read (VR<Char> item) override {
		read_byte_impl (item) ;
	}

	void read (VR<Quad> item) override {
		read_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void read_byte_impl (VR<ARG1> item) {
		auto rax = Stru32 () ;
		item = ARG1 (0X00) ;
		for (auto &&i : range (0 ,SIZE_OF<ARG1>::expr)) {
			noop (i) ;
			read (rax) ;
			const auto r1x = ARG1 (StreamProc::hex_from_str (rax)) ;
			item = (item << 4) | r1x ;
			read (rax) ;
			const auto r2x = ARG1 (StreamProc::hex_from_str (rax)) ;
			item = (item << 4) | r2x ;
		}
	}

	void read (VR<Stru32> item) override {
		if ifdo (TRUE) {
			if (self.mRead < self.mWrite)
				discard ;
			self.mOverflow (Pointer::from (thiz)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mStream->step () != 1)
				discard ;
			if (self.mRead >= self.mWrite)
				discard ;
			item = Stru (bitwise (self.mStream.ref[self.mRead])) ;
			self.mRead++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 2)
				discard ;
			if (self.mRead >= self.mWrite)
				discard ;
			item = Stru16 (bitwise (self.mStream.ref[self.mRead])) ;
			self.mRead++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 4)
				discard ;
			if (self.mRead >= self.mWrite)
				discard ;
			item = Stru32 (bitwise (self.mStream.ref[self.mRead])) ;
			self.mRead++ ;
		}
		if ifdo (act) {
			item = Stru32 (0X00) ;
		}
	}

	void push (CR<Stru32> item) {
		auto act = TRUE ;
		if ifdo (act) {
			if (item != Stru32 (0X00))
				discard ;
			if (self.mRead < self.mWrite)
				discard ;
			noop () ;
		}
		if ifdo (act) {
			self.mRead-- ;
		}
	}

	void read (CR<Slice> item) override {
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,item.size ())) {
			assume (inline_between (Index (item[i]) ,0 ,128)) ;
			read (rax) ;
			assume (rax == item[i]) ;
		}
	}

	void read (VR<StringLayout> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru>>::expr] (item) ;
			if (rax.step () != 1)
				discard ;
			read_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru16>>::expr] (item) ;
			if (rax.step () != 2)
				discard ;
			read_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru32>>::expr] (item) ;
			if (rax.step () != 4)
				discard ;
			read_string_impl (rax) ;
		}
	}

	template <class ARG1>
	forceinline void read_string_impl (VR<String<ARG1>> item) {
		item.clear () ;
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,item.size ())) {
			read (rax) ;
			item[i] = ARG1 (rax) ;
		}
	}

	void read (CR<typeof (BOM)>) override {
		auto rax = Stru32 () ;
		const auto r1x = shape () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mStream->step () != 1)
				discard ;
			read (rax) ;
			if (rax != Stru32 (0XEF))
				discard ;
			read (rax) ;
			if (rax != Stru32 (0XBB))
				discard ;
			read (rax) ;
			if (rax != Stru32 (0XBF))
				discard ;
			noop (rax) ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 2)
				discard ;
			read (rax) ;
			if (rax != Stru32 (0XFEFF))
				if (rax != Stru32 (0XFFFE))
					discard ;
			self.mDiffEndian = rax != Stru32 (0XFEFF) ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 4)
				discard ;
			read (rax) ;
			if (rax != Stru32 (0X0000FEFF))
				if (rax != Stru32 (0XFFFE0000))
					discard ;
			self.mDiffEndian = rax != Stru32 (0X0000FEFF) ;
		}
		if ifdo (act) {
			reset (r1x) ;
		}
	}

	void read (CR<typeof (CAT)>) override {
		if (self.mCats.size () == 0)
			return ;
		read (self.mCats[self.mCatIndex]) ;
		self.mCatIndex = (self.mCatIndex + 1) % self.mCats.size () ;
	}

	void cat_prefix () {
		if (self.mCatIndex == NONE)
			return ;
		read (self.mCats[self.mCatIndex]) ;
		if ifdo (TRUE) {
			self.mCatIndex++ ;
			if (self.mCatIndex < self.mCats.size ())
				discard ;
			self.mCatIndex = 0 ;
		}
	}

	void read (CR<typeof (GAP)>) override {
		auto rax = Stru32 () ;
		read (rax) ;
		while (TRUE) {
			if (rax == Stru32 (0X00))
				break ;
			if (!StreamProc::is_blank (rax))
				break ;
			read (rax) ;
		}
		push (rax) ;
	}

	void read (CR<typeof (EOS)>) override {
		auto rax = Stru32 () ;
		read (rax) ;
		assume (rax == Stru32 (0X00)) ;
	}
} ;

exports VFat<TextReaderHolder> TextReaderHolder::hold (VR<TextReaderLayout> that) {
	return VFat<TextReaderHolder> (TextReaderImplHolder () ,that) ;
}

exports CFat<TextReaderHolder> TextReaderHolder::hold (CR<TextReaderLayout> that) {
	return CFat<TextReaderHolder> (TextReaderImplHolder () ,that) ;
}

class ByteWriterImplHolder final implement Fat<ByteWriterHolder ,ByteWriterLayout> {
public:
	void initialize (RR<Ref<RefBuffer<Byte>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () == 1) ;
		self.mStream = move (stream) ;
		self.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_cats (CR<Array<Slice>> cats) override {
		self.mCats = cats ;
		self.mCatIndex = 0 ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mStream->size () ;
	}

	Length length () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mWrite ;
	}

	StreamShape shape () const override {
		StreamShape ret ;
		ret.mRead = self.mRead ;
		ret.mWrite = self.mWrite ;
		return move (ret) ;
	}

	Bool good () const override {
		return length () < size () ;
	}

	void reset () override {
		self.mRead = self.mStream->size () ;
		self.mWrite = 0 ;
	}

	void reset (CR<StreamShape> shape) override {
		self.mRead = shape.mRead ;
		self.mWrite = shape.mWrite ;
	}

	void write (CR<Bool> item) override {
		const auto r1x = BYTE_BASE<Bool> (bitwise (item)) ;
		write (r1x) ;
	}

	void write (CR<Val32> item) override {
		const auto r1x = BYTE_BASE<Val32> (bitwise (item)) ;
		write (r1x) ;
	}

	void write (CR<Val64> item) override {
		const auto r1x = BYTE_BASE<Val64> (bitwise (item)) ;
		write (r1x) ;
	}

	void write (CR<Flt32> item) override {
		const auto r1x = BYTE_BASE<Flt32> (bitwise (item)) ;
		write (r1x) ;
	}

	void write (CR<Flt64> item) override {
		const auto r1x = BYTE_BASE<Flt64> (bitwise (item)) ;
		write (r1x) ;
	}

	void write (CR<Byte> item) override {
		if ifdo (TRUE) {
			if (self.mWrite < self.mRead)
				discard ;
			self.mOverflow (Pointer::from (thiz)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mWrite >= self.mRead)
				discard ;
			self.mStream.ref[self.mWrite] = item ;
			self.mWrite++ ;
		}
	}

	void write (CR<Word> item) override {
		auto rax = item ;
		if ifdo (TRUE) {
			if (!self.mDiffEndian)
				discard ;
			rax = ByteProc::reverse (rax) ;
		}
		const auto r1x = Buffer<Byte ,SIZE_OF<Word>> (bitwise (rax)) ;
		for (auto &&i : range (0 ,r1x.size ())) {
			write (r1x[i]) ;
		}
	}

	void write (CR<Char> item) override {
		auto rax = item ;
		if ifdo (TRUE) {
			if (!self.mDiffEndian)
				discard ;
			rax = ByteProc::reverse (rax) ;
		}
		const auto r1x = Buffer<Byte ,SIZE_OF<Char>> (bitwise (rax)) ;
		for (auto &&i : range (0 ,r1x.size ())) {
			write (r1x[i]) ;
		}
	}

	void write (CR<Quad> item) override {
		auto rax = item ;
		if ifdo (TRUE) {
			if (!self.mDiffEndian)
				discard ;
			rax = ByteProc::reverse (rax) ;
		}
		const auto r1x = Buffer<Byte ,SIZE_OF<Quad>> (bitwise (rax)) ;
		for (auto &&i : range (0 ,r1x.size ())) {
			write (r1x[i]) ;
		}
	}

	void write (CR<Stru32> item) override {
		auto rax = item ;
		while (TRUE) {
			if (rax < Stru32 (0X80))
				break ;
			const auto r1x = (Byte (rax) & Byte (0X7F)) | Byte (0X80) ;
			write (r1x) ;
			rax = rax >> 7 ;
		}
		const auto r2x = Byte (rax) ;
		write (r2x) ;
	}

	void write (CR<Slice> item) override {
		for (auto &&i : range (0 ,item.size ())) {
			assume (inline_between (Index (item[i]) ,0 ,128)) ;
			write (item[i]) ;
		}
	}

	void write (CR<StringLayout> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru>>::expr] (item) ;
			if (rax.step () != 1)
				discard ;
			write_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru16>>::expr] (item) ;
			if (rax.step () != 2)
				discard ;
			write_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru32>>::expr] (item) ;
			if (rax.step () != 4)
				discard ;
			write_string_impl (rax) ;
		}
	}

	template <class ARG1>
	forceinline void write_string_impl (CR<String<ARG1>> item) {
		const auto r1x = item.length () ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = Stru32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CR<typeof (BOM)>) override {
		self.mDiffEndian = !self.mDiffEndian ;
	}

	void write (CR<typeof (CAT)>) override {
		if (self.mCats.size () == 0)
			return ;
		write (self.mCats[self.mCatIndex]) ;
		self.mCatIndex = (self.mCatIndex + 1) % self.mCats.size () ;
	}

	void cat_prefix () {
		if (self.mCatIndex == NONE)
			return ;
		write (self.mCats[self.mCatIndex]) ;
		if ifdo (TRUE) {
			self.mCatIndex++ ;
			if (self.mCatIndex < self.mCats.size ())
				discard ;
			self.mCatIndex = 0 ;
		}
	}

	void write (CR<typeof (GAP)>) override {
		write (Byte (0X5D)) ;
		write (Byte (0X5B)) ;
	}

	void write (CR<typeof (EOS)>) override {
		while (TRUE) {
			if (self.mWrite >= self.mRead)
				break ;
			write (Byte (0X00)) ;
		}
	}
} ;

exports VFat<ByteWriterHolder> ByteWriterHolder::hold (VR<ByteWriterLayout> that) {
	return VFat<ByteWriterHolder> (ByteWriterImplHolder () ,that) ;
}

exports CFat<ByteWriterHolder> ByteWriterHolder::hold (CR<ByteWriterLayout> that) {
	return CFat<ByteWriterHolder> (ByteWriterImplHolder () ,that) ;
}

struct WriteValueBuffer {
	Buffer<Stru ,ENUM<64>> mBuffer ;
	Index mWrite ;
} ;

class TextWriterImplHolder final implement Fat<TextWriterHolder ,TextWriterLayout> {
public:
	void initialize (RR<Ref<RefBuffer<Byte>>> stream) override {
		assert (stream != NULL) ;
		assert (stream->step () <= 4) ;
		self.mStream = move (stream) ;
		self.mDiffEndian = FALSE ;
		reset () ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_cats (CR<Array<Slice>> cats) override {
		self.mCats = cats ;
		self.mCatIndex = 0 ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mStream->size () ;
	}

	Length length () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mWrite ;
	}

	StreamShape shape () const override {
		StreamShape ret ;
		ret.mRead = self.mRead ;
		ret.mWrite = self.mWrite ;
		return move (ret) ;
	}

	Bool good () const override {
		return length () < size () ;
	}

	void reset () override {
		self.mRead = self.mStream->size () ;
		self.mWrite = 0 ;
	}

	void reset (CR<StreamShape> shape) override {
		self.mRead = shape.mRead ;
		self.mWrite = shape.mWrite ;
	}

	void write (CR<Bool> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (!item)
				discard ;
			write (slice ("true")) ;
		}
		if ifdo (act) {
			write (slice ("false")) ;
		}
	}

	void write (CR<Val32> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (Stru32 ('-')) ;
		}
		const auto r1x = Val64 (MathProc::abs (item)) ;
		write (r1x) ;
	}

	void write (CR<Val64> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (Stru32 ('-')) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = FALSE ;
			rax.mMantissa = MathProc::abs (item) ;
			rax.mDownflow = 0 ;
			rax.mExponent = 0 ;
			rax.mPrecision = Length (log10p (rax.mMantissa)) ;
			auto rbx = WriteValueBuffer () ;
			rbx.mWrite = rbx.mBuffer.size () ;
			write_value (rax ,rbx) ;
			for (auto &&i : range (rbx.mWrite ,rbx.mBuffer.size ()))
				write (Stru32 (rbx.mBuffer[i])) ;
		}
	}

	void write_value (VR<Notation> fexp10 ,VR<WriteValueBuffer> wvb) {
		assert (fexp10.mRadix == 10) ;
		const auto r1x = fexp10.mPrecision ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != 0)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'xxx'
			for (auto &&i : range (0 ,r1x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (TRUE) {
			if (!fexp10.mSign)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('-') ;
		}
	}

	void write (CR<Flt32> item) override {
		const auto r1x = Flt64 (item) ;
		write (r1x) ;
	}

	void write (CR<Flt64> item) override {
		if ifdo (TRUE) {
			if (item >= 0)
				discard ;
			write (Stru32 ('-')) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (!MathProc::is_inf (item))
				discard ;
			write (slice ("infinity")) ;
		}
		if ifdo (act) {
			auto rax = FloatProc::decode (MathProc::abs (item)) ;
			rax = FloatProc::fexp10_from_fexp2 (rax) ;
			rax.mPrecision = Length (log10p (rax.mMantissa)) ;
			auto rbx = WriteValueBuffer () ;
			rbx.mWrite = rbx.mBuffer.size () ;
			write_float (rax ,rbx) ;
			for (auto &&i : range (rbx.mWrite ,rbx.mBuffer.size ()))
				write (Stru32 (rbx.mBuffer[i])) ;
		}
	}

	void write_float (VR<Notation> fexp10 ,VR<WriteValueBuffer> wvb) {
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
			for (auto &&i : range (0 ,r2x - 1)) {
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
		const auto r5x = Length (fexp10.mExponent) ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != 0)
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'x.xxxExxx'
			const auto r6x = r4x - 1 + r5x ;
			if (MathProc::abs (r6x) < r1x)
				discard ;
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = Bool (r6x < 0) ;
			rax.mMantissa = MathProc::abs (r6x) ;
			rax.mDownflow = 0 ;
			rax.mExponent = 0 ;
			rax.mPrecision = Length (log10p (rax.mMantissa)) ;
			write_value (rax ,wvb) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('E') ;
			const auto r7x = inline_max (Length (r4x - 1 - r1x) ,0) ;
			for (auto &&i : range (0 ,r7x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			Index ix = wvb.mWrite - 1 ;
			for (auto &&i : range (r7x ,r4x - 1)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('.') ;
			wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('.')) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
			fexp10.mMantissa /= 10 ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
		if ifdo (act) {
			//@info: case 'xxx000'
			if (r5x < 0)
				discard ;
			for (auto &&i : range (0 ,r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
			}
			for (auto &&i : range (0 ,r4x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
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
			const auto r8x = inline_max (Length (-r5x - r1x) ,0) ;
			for (auto &&i : range (0 ,r8x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			Index ix = wvb.mWrite - 1 ;
			for (auto &&i : range (r8x ,-r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('.') ;
			wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('.')) ;
			for (auto &&i : range (0 ,r4x + r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
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
			const auto r9x = inline_max (Length (-r5x - r1x) ,ZERO) ;
			for (auto &&i : range (0 ,r9x)) {
				noop (i) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			Index ix = wvb.mWrite - 1 ;
			for (auto &&i : range (r9x ,r4x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (fexp10.mMantissa % 10)) ;
				wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('0')) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			const auto r10x = inline_max (r9x ,r4x) ;
			for (auto &&i : range (r10x ,-r5x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
				wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('0')) ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('.') ;
			wvb.mWrite += Length (wvb.mBuffer[ix] == Stru32 ('.')) ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
		}
	}

	Val64 log10p (CR<Val64> a) const {
		Val64 ret = 0 ;
		auto rax = a ;
		while (TRUE) {
			if (rax == 0)
				break ;
			ret++ ;
			rax /= 10 ;
		}
		return move (ret) ;
	}

	void write (CR<Byte> item) override {
		write_byte_impl (item) ;
	}

	void write (CR<Word> item) override {
		write_byte_impl (item) ;
	}

	void write (CR<Char> item) override {
		write_byte_impl (item) ;
	}

	void write (CR<Quad> item) override {
		write_byte_impl (item) ;
	}

	template <class ARG1>
	forceinline void write_byte_impl (CR<ARG1> item) {
		Index ix = SIZE_OF<ARG1>::expr * 8 ;
		for (auto &&i : range (0 ,SIZE_OF<ARG1>::expr)) {
			noop (i) ;
			ix -= 4 ;
			const auto r1x = Index ((item >> ix) & ARG1 (0X0F)) ;
			write (StreamProc::str_from_hex (r1x)) ;
			ix -= 4 ;
			const auto r2x = Index ((item >> ix) & ARG1 (0X0F)) ;
			write (StreamProc::str_from_hex (r2x)) ;
		}
	}

	void write (CR<Stru32> item) override {
		if ifdo (TRUE) {
			if (self.mWrite < self.mRead)
				discard ;
			self.mOverflow (Pointer::from (thiz)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mStream->step () != 1)
				discard ;
			if (self.mWrite >= self.mRead)
				discard ;
			bitwise (self.mStream.ref[self.mWrite]) = Stru (item) ;
			self.mWrite++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 2)
				discard ;
			if (self.mWrite >= self.mRead)
				discard ;
			bitwise (self.mStream.ref[self.mWrite]) = Stru16 (item) ;
			self.mWrite++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 4)
				discard ;
			if (self.mWrite >= self.mRead)
				discard ;
			bitwise (self.mStream.ref[self.mWrite]) = Stru32 (item) ;
			self.mWrite++ ;
		}
	}

	void write (CR<Slice> item) override {
		for (auto &&i : range (0 ,item.size ())) {
			assume (inline_between (Index (item[i]) ,0 ,128)) ;
			write (item[i]) ;
		}
	}

	void write (CR<StringLayout> item) override {
		auto act = TRUE ;
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru>>::expr] (item) ;
			if (rax.step () != 1)
				discard ;
			write_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru16>>::expr] (item) ;
			if (rax.step () != 2)
				discard ;
			write_string_impl (rax) ;
		}
		if ifdo (act) {
			auto &&rax = keep[TYPE<String<Stru32>>::expr] (item) ;
			if (rax.step () != 4)
				discard ;
			write_string_impl (rax) ;
		}
	}

	template <class ARG1>
	forceinline void write_string_impl (CR<String<ARG1>> item) {
		const auto r1x = item.length () ;
		for (auto &&i : range (0 ,r1x)) {
			const auto r2x = Stru32 (item[i]) ;
			write (r2x) ;
		}
	}

	void write (CR<typeof (BOM)>) override {
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mStream->step () != 1)
				discard ;
			write (Stru32 (0XEF)) ;
			write (Stru32 (0XBB)) ;
			write (Stru32 (0XBF)) ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 2)
				discard ;
			write (Stru32 (0XFEFF)) ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 4)
				discard ;
			write (Stru32 (0X0000FEFF)) ;
		}
	}

	void write (CR<typeof (CAT)>) override {
		if (self.mCats.size () == 0)
			return ;
		write (self.mCats[self.mCatIndex]) ;
		self.mCatIndex = (self.mCatIndex + 1) % self.mCats.size () ;
	}

	void cat_prefix () {
		if (self.mCatIndex == NONE)
			return ;
		write (self.mCats[self.mCatIndex]) ;
		if ifdo (TRUE) {
			self.mCatIndex++ ;
			if (self.mCatIndex < self.mCats.size ())
				discard ;
			self.mCatIndex = 0 ;
		}
	}

	void write (CR<typeof (GAP)>) override {
		write (Stru32 ('\r')) ;
		write (Stru32 ('\n')) ;
	}

	void write (CR<typeof (EOS)>) override {
		assume (self.mWrite < self.mRead) ;
		write (Stru32 (0X00)) ;
	}
} ;

exports VFat<TextWriterHolder> TextWriterHolder::hold (VR<TextWriterLayout> that) {
	return VFat<TextWriterHolder> (TextWriterImplHolder () ,that) ;
}

exports CFat<TextWriterHolder> TextWriterHolder::hold (CR<TextWriterLayout> that) {
	return CFat<TextWriterHolder> (TextWriterImplHolder () ,that) ;
}

class FormatImplHolder final implement Fat<FormatHolder ,FormatLayout> {
public:
	void initialize (CR<Slice> format) override {
		self.mFormat = format ;
	}

	void friend_write (CR<Writer> writer) const override {
		auto rax = Flag (0) ;
		for (auto &&i : range (0 ,self.mFormat.size ())) {
			auto act = TRUE ;
			if ifdo (act) {
				if (rax != Flag (0))
					discard ;
				if (self.mFormat[i] != Stru32 ('$'))
					discard ;
				rax = Flag (2) ;
			}
			if ifdo (act) {
				if (rax != Flag (2))
					discard ;
				if (self.mFormat[i] != Stru32 ('{'))
					discard ;
				rax = Flag (1) ;
			}
			if ifdo (act) {
				if (rax != Flag (2))
					discard ;
				if ifdo (TRUE) {
					const auto r1x = StreamProc::hex_from_str (self.mFormat[i]) - 1 ;
					if (!inline_between (r1x ,0 ,self.mWrite))
						discard ;
					auto &&rbx = keep[TYPE<VFat<WritingHolder>>::expr] (self.mParams[r1x]) ;
					rbx->friend_write (writer) ;
				}
				rax = Flag (0) ;
			}
			if ifdo (act) {
				if (rax != Flag (1))
					discard ;
				if (self.mFormat[i] != Stru32 ('0'))
					discard ;
				for (auto &&j : range (0 ,self.mWrite)) {
					auto &&rbx = keep[TYPE<VFat<WritingHolder>>::expr] (self.mParams[j]) ;
					rbx->friend_write (writer) ;
				}
				rax = Flag (3) ;
			}
			if ifdo (act) {
				if (rax != Flag (1))
					discard ;
				if ifdo (TRUE) {
					const auto r2x = StreamProc::hex_from_str (self.mFormat[i]) - 1 ;
					if (!inline_between (r2x ,0 ,self.mWrite))
						discard ;
					auto &&rbx = keep[TYPE<VFat<WritingHolder>>::expr] (self.mParams[r2x]) ;
					rbx->friend_write (writer) ;
				}
				rax = Flag (3) ;
			}
			if ifdo (act) {
				if (rax != Flag (3))
					discard ;
				assert (self.mFormat[i] == Stru32 ('}')) ;
				rax = Flag (0) ;
			}
			if ifdo (act) {
				assume (rax == Flag (0)) ;
				writer.write (self.mFormat[i]) ;
			}
		}
	}

	void once (CR<Wrapper<FatLayout>> params) const override {
		const auto r1x = Pin<BufferX<FatLayout>> (self.mParams) ;
		const auto r2x = Pin<Length> (self.mWrite) ;
		Index ix = 0 ;
		for (auto &&i : range (0 ,params.rank ())) {
			r1x.ref[ix] = params[i] ;
			ix++ ;
		}
		r2x.ref = ix ;
	}
} ;

exports VFat<FormatHolder> FormatHolder::hold (VR<FormatLayout> that) {
	return VFat<FormatHolder> (FormatImplHolder () ,that) ;
}

exports CFat<FormatHolder> FormatHolder::hold (CR<FormatLayout> that) {
	return CFat<FormatHolder> (FormatImplHolder () ,that) ;
}

struct StreamTextProcLayout {} ;

class StreamTextProcImplHolder final implement Fat<StreamTextProcHolder ,StreamTextProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void read_keyword (CR<Reader> reader ,VR<String<Stru>> item) const override {
		auto rax = Stru32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.shape () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			if (!StreamProc::is_word (rax))
				break ;
			rbx++ ;
			reader.read (rax) ;
			while (TRUE) {
				if (!StreamProc::is_word (rax))
					if (!StreamProc::is_hyphen (rax))
						break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<Stru> (rbx) ;
		reader.read (item) ;
	}

	void read_scalar (CR<Reader> reader ,VR<String<Stru>> item) const override {
		auto rax = Stru32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.shape () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != Stru32 ('+'))
					if (rax != Stru32 ('-'))
						discard ;
				rbx++ ;
				reader.read (rax) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
			if ifdo (TRUE) {
				if (rax != Stru32 ('.'))
					discard ;
				rbx++ ;
				reader.read (rax) ;
				while (TRUE) {
					if (!StreamProc::is_digit (rax))
						break ;
					rbx++ ;
					reader.read (rax) ;
				}
			}
			if (rax != Stru32 ('E'))
				if (rax != Stru32 ('e'))
					discard ;
			rbx++ ;
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != Stru32 ('+'))
					if (rax != Stru32 ('-'))
						discard ;
				rbx++ ;
				reader.read (rax) ;
			}
			while (TRUE) {
				if (!StreamProc::is_digit (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<Stru> (rbx) ;
		reader.read (item) ;
	}

	void read_escape (CR<Reader> reader ,VR<String<Stru>> item) const override {
		auto rax = Stru32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.shape () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			assume (rax == Stru32 ('\"')) ;
			reader.read (rax) ;
			while (TRUE) {
				if (rax == Stru32 (0X00))
					break ;
				if (rax == Stru32 ('\"'))
					break ;
				if ifdo (TRUE) {
					if (rax != Stru32 ('\\'))
						discard ;
					reader.read (rax) ;
				}
				rbx++ ;
				reader.read (rax) ;
			}
			assume (rax == Stru32 ('\"')) ;
		}
		reader.reset (r1x) ;
		item = String<Stru> (rbx) ;
		reader.read (rax) ;
		for (auto &&i : range (0 ,rbx)) {
			reader.read (rax) ;
			if ifdo (TRUE) {
				if (rax != Stru32 ('\\'))
					discard ;
				reader.read (rax) ;
				rax = StreamProc::ctrl_from_word (rax) ;
			}
			item[i] = Stru (rax) ;
		}
		reader.read (rax) ;
	}

	void write_escape (CR<Writer> writer ,CR<String<Stru>> item) const override {
		writer.write (Stru32 ('\"')) ;
		for (auto &&i : item) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!StreamProc::is_ctrl (i))
					discard ;
				const auto r1x = StreamProc::word_from_ctrl (i) ;
				writer.write (Stru32 ('\\')) ;
				writer.write (r1x) ;
			}
			if ifdo (act) {
				writer.write (Stru32 (i)) ;
			}
		}
		writer.write (Stru32 ('\"')) ;
	}

	void read_blank (CR<Reader> reader ,VR<String<Stru>> item) const override {
		auto rax = Stru32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.shape () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			while (TRUE) {
				if (rax == Stru32 (0X00))
					break ;
				if (StreamProc::is_space (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<Stru> (rbx) ;
		reader.read (item) ;
	}

	void read_endline (CR<Reader> reader ,VR<String<Stru>> item) const override {
		auto rax = Stru32 () ;
		auto rbx = ZERO ;
		const auto r1x = reader.shape () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			while (TRUE) {
				if (rax == Stru32 (0X00))
					break ;
				if (StreamProc::is_endline (rax))
					break ;
				rbx++ ;
				reader.read (rax) ;
			}
		}
		reader.reset (r1x) ;
		item = String<Stru> (rbx) ;
		reader.read (item) ;
	}

	void write_aligned (CR<Writer> writer ,CR<Val64> number ,CR<Length> align) const override {
		auto rax = WriteValueBuffer () ;
		assert (inline_between (align ,0 ,rax.mBuffer.size ())) ;
		rax.mWrite = rax.mBuffer.size () ;
		auto rbx = MathProc::abs (number) ;
		for (auto &&i : range (0 ,align)) {
			noop (i) ;
			rax.mWrite-- ;
			rax.mBuffer[rax.mWrite] = Stru (Stru32 ('0') + rbx % 10) ;
			rbx /= 10 ;
		}
		for (auto &&i : range (rax.mWrite ,rax.mBuffer.size ())) {
			writer.write (Stru32 (rax.mBuffer[i])) ;
		}
	}
} ;

exports CR<Super<Ref<StreamTextProcLayout>>> StreamTextProcHolder::expr_m () {
	return memorize ([&] () {
		Super<Ref<StreamTextProcLayout>> ret ;
		ret.mThis = Ref<StreamTextProcLayout>::make () ;
		StreamTextProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StreamTextProcHolder> StreamTextProcHolder::hold (VR<StreamTextProcLayout> that) {
	return VFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}

exports CFat<StreamTextProcHolder> StreamTextProcHolder::hold (CR<StreamTextProcLayout> that) {
	return CFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}

struct CommaLayout {
	Slice mIndent ;
	Slice mComma ;
	Slice mEndline ;
	Length mDepth ;
	Deque<Bool> mFirst ;
	Length mTight ;
	Length mLastTight ;
} ;

class CommaImplHolder final implement Fat<CommaHolder ,CommaLayout> {
public:
	void initialize (CR<Slice> indent ,CR<Slice> comma ,CR<Slice> endline) override {
		self.mIndent = indent ;
		self.mComma = comma ;
		self.mEndline = endline ;
		self.mDepth = 0 ;
		self.mTight = 255 ;
		self.mLastTight = 0 ;
	}

	void friend_write (CR<Writer> writer) override {
		if ifdo (TRUE) {
			if (self.mDepth >= self.mTight + self.mLastTight)
				discard ;
			writer.write (self.mEndline) ;
		}
		if ifdo (TRUE) {
			if (self.mFirst.empty ())
				discard ;
			Index ix = self.mFirst.tail () ;
			if ifdo (TRUE) {
				if (self.mFirst[ix])
					discard ;
				writer.write (self.mComma) ;
			}
			self.mFirst[ix] = FALSE ;
		}
		if ifdo (TRUE) {
			if (self.mDepth >= self.mTight + self.mLastTight)
				discard ;
			for (auto &&i : range (0 ,self.mDepth)) {
				noop (i) ;
				writer.write (self.mIndent) ;
			}
		}
		self.mLastTight = 0 ;
	}

	void increase () override {
		self.mDepth++ ;
		if ifdo (TRUE) {
			if (self.mFirst.empty ())
				discard ;
			self.mFirst[self.mFirst.tail ()] = TRUE ;
		}
		self.mFirst.add (TRUE) ;
	}

	void decrease () override {
		self.mFirst.pop () ;
		self.mDepth-- ;
		self.mLastTight = self.mTight - 256 ;
		self.mTight = 255 ;
	}

	void tight () override {
		self.mTight = inline_min (self.mTight ,self.mDepth) ;
	}
} ;

exports Ref<CommaLayout> CommaHolder::create () {
	return Ref<CommaLayout>::make () ;
}

exports VFat<CommaHolder> CommaHolder::hold (VR<CommaLayout> that) {
	return VFat<CommaHolder> (CommaImplHolder () ,that) ;
}

exports CFat<CommaHolder> CommaHolder::hold (CR<CommaLayout> that) {
	return CFat<CommaHolder> (CommaImplHolder () ,that) ;
}

struct RegexLayout {
	std::basic_regex<Str> mRegex ;
	std::match_results<PTR<CR<Str>>> mMatch ;
	Ref<String<Str>> mText ;
} ;

class RegexImplHolder final implement Fat<RegexHolder ,RegexLayout> {
public:
	void initialize (CR<String<Str>> format) override {
		self.mRegex = std::basic_regex<Str> (format) ;
	}

	Index search (RR<Ref<String<Str>>> text ,CR<Index> offset) override {
		self.mText = move (text) ;
		const auto r1x = (&self.mText.ref[offset]) ;
		const auto r2x = std::regex_search (r1x ,self.mMatch ,self.mRegex) ;
		if (!r2x)
			return NONE ;
		const auto r3x = Flag (self.mMatch[0].first) ;
		const auto r4x = (r3x - Flag (r1x)) / SIZE_OF<Str>::expr ;
		return offset + r4x ;
	}

	Slice match (CR<Index> index) const override {
		assert (!self.mMatch.empty ()) ;
		assert (inline_between (index ,0 ,self.mMatch.size ())) ;
		const auto r1x = Flag (self.mMatch[index].first) ;
		const auto r2x = Flag (self.mMatch[index].second) ;
		const auto r3x = (r2x - r1x) / SIZE_OF<Str>::expr ;
		return Slice (r1x ,r3x ,SIZE_OF<Str>::expr) ;
	}
} ;

exports Ref<RegexLayout> RegexHolder::create () {
	return Ref<RegexLayout>::make () ;
}

exports VFat<RegexHolder> RegexHolder::hold (VR<RegexLayout> that) {
	return VFat<RegexHolder> (RegexImplHolder () ,that) ;
}

exports CFat<RegexHolder> RegexHolder::hold (CR<RegexLayout> that) {
	return CFat<RegexHolder> (RegexImplHolder () ,that) ;
}
} ;