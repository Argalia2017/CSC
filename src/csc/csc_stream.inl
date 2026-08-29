#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_stream.hpp"

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
		if (inline_mid (hex ,0 ,10))
			return Stru32 ('0') + Stru32 (hex) ;
		if (inline_mid (hex ,10 ,16))
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

exports CR<Super<UniqueRef<StreamProcLayout>>> StreamProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<StreamProcLayout>> ret ;
		ret.mThis = UniqueRef<StreamProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		StreamProcHolder::hold (rax.ref)->initialize () ;
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
		set_tab (0) ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_tab (CR<Length> tab_align) override {
		self.mTabIndex = self.mRead ;
		self.mTabAlign = tab_align ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mWrite ;
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
			self.mOverflow (Pointer::from (self)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mRead < self.mWrite)
				discard ;
			item = Byte (0X00) ;
		}
		if ifdo (act) {
			item = self.mStream.ref[self.mRead] ;
			self.mRead++ ;
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
		auto rax = Char (0X00) ;
		auto rbx = Byte () ;
		Index ix = 0 ;
		while (TRUE) {
			read (rbx) ;
			if (!ByteProc::any_bit (rbx ,Byte (0X80)))
				break ;
			rax |= Char (rbx & Byte (0X7F)) << ix ;
			ix += 7 ;
		}
		rax |= Char (rbx) << ix ;
		item = Stru32 (rax) ;
	}

	void read (CR<Slice> item) override {
		auto rax = Stru32 () ;
		for (auto &&i : range (0 ,item.size ())) {
			assume (inline_mid (Index (item[i]) ,0 ,128)) ;
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

	void read (CR<typeof (TAB)>) override {
		if (self.mTabAlign <= 0)
			return ;
		const auto r1x = inline_max (self.mRead - self.mTabIndex ,0) ;
		self.mRead += self.mTabAlign - r1x ;
		self.mTabIndex = self.mRead ;
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
		set_tab (0) ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_tab (CR<Length> tab_align) override {
		self.mTabIndex = self.mRead ;
		self.mTabAlign = tab_align ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mWrite ;
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
			rbx.mMantissa = Quad (0X00) ;
			rbx.mDownflow = Quad (0X00) ;
			rbx.mExponent = 0 ;
			read_value (rbx ,rax) ;
			assume (Val64 (rbx.mMantissa) >= 0) ;
			item = Val64 (rbx.mMantissa) ;
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
				const auto r2x = Val64 (fexp10.mMantissa) * 10 + StreamProc::hex_from_str (top) ;
				fexp10.mMantissa = Quad (r2x) ;
				fexp10.mPrecision++ ;
				read (top) ;
			}
			if ifdo (TRUE) {
				if (!StreamProc::is_digit (top))
					discard ;
				const auto r3x = Val64 (fexp10.mMantissa) * 10 + StreamProc::hex_from_str (top) ;
				if (r3x < 0)
					discard ;
				fexp10.mMantissa = Quad (r3x) ;
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
			rbx.mMantissa = Quad (0X00) ;
			rbx.mDownflow = Quad (0X00) ;
			rbx.mExponent = 0 ;
			read_float (rbx ,rax) ;
			assume (Val64 (rbx.mMantissa) >= 0) ;
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
				const auto r2x = Val64 (fexp10.mMantissa) * 10 + StreamProc::hex_from_str (top) ;
				fexp10.mMantissa = Quad (r2x) ;
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
			const auto r3x = Bool (top == Stru32 ('-')) ;
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
			rbx.mSign = r3x ;
			read_value (rbx ,top) ;
			assume (rbx.mExponent == 0) ;
			const auto r4x = Val64 (rbx.mMantissa) ;
			const auto r5x = r3x ? -r4x : r4x ;
			fexp10.mExponent += r5x ;
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
			self.mOverflow (Pointer::from (self)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mRead < self.mWrite)
				discard ;
			item = Stru32 (0X00) ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 1)
				discard ;
			item = Stru (bitwise (self.mStream.ref[self.mRead])) ;
			self.mRead++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 2)
				discard ;
			item = Stru16 (bitwise (self.mStream.ref[self.mRead])) ;
			self.mRead++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 4)
				discard ;
			item = Stru32 (bitwise (self.mStream.ref[self.mRead])) ;
			self.mRead++ ;
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
			assume (inline_mid (Index (item[i]) ,0 ,128)) ;
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

	void read (CR<typeof (TAB)>) override {
		if (self.mTabAlign <= 0)
			return ;
		const auto r1x = inline_max (self.mRead - self.mTabIndex ,0) ;
		self.mRead += self.mTabAlign - r1x ;
		self.mTabIndex = self.mRead ;
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
		set_tab (0) ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_tab (CR<Length> tab_align) override {
		self.mTabIndex = self.mWrite ;
		self.mTabAlign = tab_align ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mRead ;
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
			self.mOverflow (Pointer::from (self)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mWrite < self.mRead)
				discard ;
			noop () ;
		}
		if ifdo (act) {
			self.mStream.ref[self.mWrite] = item ;
			self.mWrite++ ;
		}
	}

	void write (CR<Word> item) override {
		const auto r1x = self.mDiffEndian ? ByteProc::reverse (item) : item ;
		const auto r2x = Buffer<Byte ,SIZE_OF<Word>> (bitwise (r1x)) ;
		for (auto &&i : range (0 ,r2x.size ())) {
			write (r2x[i]) ;
		}
	}

	void write (CR<Char> item) override {
		const auto r1x = self.mDiffEndian ? ByteProc::reverse (item) : item ;
		const auto r2x = Buffer<Byte ,SIZE_OF<Char>> (bitwise (r1x)) ;
		for (auto &&i : range (0 ,r2x.size ())) {
			write (r2x[i]) ;
		}
	}

	void write (CR<Quad> item) override {
		const auto r1x = self.mDiffEndian ? ByteProc::reverse (item) : item ;
		const auto r2x = Buffer<Byte ,SIZE_OF<Quad>> (bitwise (r1x)) ;
		for (auto &&i : range (0 ,r2x.size ())) {
			write (r2x[i]) ;
		}
	}

	void write (CR<Stru32> item) override {
		auto rax = Char (item) ;
		while (TRUE) {
			if (!ByteProc::any_bit (rax ,Char (0XFFFFFF80)))
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
			assume (inline_mid (Index (item[i]) ,0 ,128)) ;
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

	void write (CR<typeof (TAB)>) override {
		if (self.mTabAlign <= 0)
			return ;
		const auto r1x = inline_max (self.mWrite - self.mTabIndex ,0) ;
		const auto r2x = inline_max (r1x - self.mTabAlign ,0) ;
		self.mWrite -= r2x ;
		for (auto &&i : range (0 ,self.mTabAlign - r1x)) {
			noop (i) ;
			write (Stru32 (' ')) ;
		}
		self.mTabIndex = self.mWrite ;
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
		set_tab (0) ;
	}

	void use_overflow (CR<Function<CR<Pointer>>> overflow) override {
		self.mOverflow = overflow ;
	}

	void set_tab (CR<Length> tab_align) override {
		self.mTabIndex = self.mWrite ;
		self.mTabAlign = tab_align ;
	}

	Length size () const override {
		if (self.mStream == NULL)
			return 0 ;
		return self.mRead ;
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
			rax.mMantissa = Quad (MathProc::abs (item)) ;
			rax.mDownflow = Quad (0X00) ;
			rax.mExponent = 0 ;
			rax.mPrecision = Length (MathProc::log10_bit (Val64 (rax.mMantissa))) ;
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
			if (fexp10.mMantissa != Quad (0X00))
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'xxx'
			for (auto &&i : range (0 ,r1x)) {
				noop (i) ;
				wvb.mWrite-- ;
				const auto r2x = Val64 (fexp10.mMantissa) ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r2x % 10)) ;
				fexp10.mMantissa = Quad (r2x / 10) ;
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
			rax.mPrecision = Length (MathProc::log10_bit (Val64 (rax.mMantissa))) ;
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
			normalize_fexp10 (fexp10) ;
			const auto r2x = fexp10.mPrecision - r1x ;
			for (auto &&i : range (0 ,r2x - 1)) {
				noop (i) ;
				const auto r3x = Val64 (fexp10.mMantissa) ;
				fexp10.mMantissa = Quad (r3x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if (r2x <= 0)
				discard ;
			const auto r4x = MathProc::step (Val64 (fexp10.mMantissa) % 10 - 5) * 5 ;
			const auto r5x = (Val64 (fexp10.mMantissa) + r4x) / 10 ;
			fexp10.mMantissa = Quad (r5x) ;
			fexp10.mExponent++ ;
			fexp10.mPrecision = Length (MathProc::log10_bit (Val64 (fexp10.mMantissa))) ;
			normalize_fexp10 (fexp10) ;
		}
		const auto r6x = fexp10.mPrecision ;
		const auto r7x = Length (fexp10.mExponent) ;
		auto act = TRUE ;
		if ifdo (act) {
			//@info: case '0'
			if (fexp10.mMantissa != Quad (0X00))
				discard ;
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
		}
		if ifdo (act) {
			//@info: case 'x.xxxExxx'
			const auto r8x = r6x - 1 + r7x ;
			if (MathProc::abs (r8x) < r1x)
				discard ;
			auto rax = Notation () ;
			rax.mRadix = 10 ;
			rax.mSign = Bool (r8x < 0) ;
			rax.mMantissa = Quad (MathProc::abs (r8x)) ;
			rax.mDownflow = Quad (0X00) ;
			rax.mExponent = 0 ;
			rax.mPrecision = Length (MathProc::log10_bit (Val64 (rax.mMantissa))) ;
			write_value (rax ,wvb) ;
			if ifdo (TRUE) {
				if (rax.mSign)
					discard ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('+') ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('E') ;
			const auto r9x = inline_max (Length (r6x - 1 - r1x) ,0) ;
			for (auto &&i : range (0 ,r9x)) {
				noop (i) ;
				const auto r10x = Val64 (fexp10.mMantissa) ;
				fexp10.mMantissa = Quad (r10x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			normalize_fexp10 (fexp10) ;
			const auto r11x = fexp10.mPrecision - 1 ;
			for (auto &&i : range (0 ,r11x)) {
				noop (i) ;
				wvb.mWrite-- ;
				const auto r12x = Val64 (fexp10.mMantissa) ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r12x % 10)) ;
				fexp10.mMantissa = Quad (r12x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if ifdo (TRUE) {
				if (r11x <= 0)
					discard ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('.') ;
			}
			wvb.mWrite-- ;
			const auto r13x = Val64 (fexp10.mMantissa) ;
			wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r13x % 10)) ;
			fexp10.mMantissa = Quad (r13x / 10) ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
		if ifdo (act) {
			//@info: case 'xxx000'
			if (r7x < 0)
				discard ;
			for (auto &&i : range (0 ,r7x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
			}
			for (auto &&i : range (0 ,r6x)) {
				noop (i) ;
				wvb.mWrite-- ;
				const auto r14x = Val64 (fexp10.mMantissa) ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r14x % 10)) ;
				fexp10.mMantissa = Quad (r14x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (act) {
			//@info: case 'xxx.xxx'
			if (r7x < 1 - r6x)
				discard ;
			if (r7x >= 0)
				discard ;
			const auto r15x = inline_max (Length (-r7x - r1x) ,0) ;
			for (auto &&i : range (0 ,r15x)) {
				noop (i) ;
				const auto r16x = Val64 (fexp10.mMantissa) ;
				fexp10.mMantissa = Quad (r16x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			normalize_fexp10 (fexp10) ;
			const auto r17x = Length (-fexp10.mExponent) ;
			const auto r18x = fexp10.mPrecision - r17x ;
			for (auto &&i : range (0 ,r17x)) {
				noop (i) ;
				wvb.mWrite-- ;
				const auto r19x = Val64 (fexp10.mMantissa) ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r19x % 10)) ;
				fexp10.mMantissa = Quad (r19x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if ifdo (TRUE) {
				if (r17x <= 0)
					discard ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('.') ;
			}
			for (auto &&i : range (0 ,r18x)) {
				noop (i) ;
				wvb.mWrite-- ;
				const auto r20x = Val64 (fexp10.mMantissa) ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r20x % 10)) ;
				fexp10.mMantissa = Quad (r20x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
		}
		if ifdo (act) {
			//@info: case '0.000xxx'
			if (r7x >= 1 - r6x)
				discard ;
			if (r7x >= 0)
				discard ;
			const auto r21x = inline_max (Length (-r7x - r1x) ,ZERO) ;
			for (auto &&i : range (0 ,r21x)) {
				noop (i) ;
				const auto r22x = Val64 (fexp10.mMantissa) ;
				fexp10.mMantissa = Quad (r22x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			normalize_fexp10 (fexp10) ;
			const auto r23x = fexp10.mPrecision ;
			const auto r24x = Length (-fexp10.mExponent) - r23x ;
			for (auto &&i : range (0 ,r23x)) {
				noop (i) ;
				wvb.mWrite-- ;
				const auto r25x = Val64 (fexp10.mMantissa) ;
				wvb.mBuffer[wvb.mWrite] = Stru (StreamProc::str_from_hex (r25x % 10)) ;
				fexp10.mMantissa = Quad (r25x / 10) ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			for (auto &&i : range (0 ,r24x)) {
				noop (i) ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
			}
			if ifdo (TRUE) {
				if (r23x <= 0)
					discard ;
				wvb.mWrite-- ;
				wvb.mBuffer[wvb.mWrite] = Stru32 ('.') ;
			}
			wvb.mWrite-- ;
			wvb.mBuffer[wvb.mWrite] = Stru32 ('0') ;
		}
	}

	void normalize_fexp10 (VR<Notation> fexp10) {
		while (TRUE) {
			if (fexp10.mMantissa == Quad (0X00))
				break ;
			const auto r1x = Val64 (fexp10.mMantissa) ;
			if (r1x % 10 != 0)
				break ;
			fexp10.mMantissa = Quad (r1x / 10) ;
			fexp10.mExponent++ ;
			fexp10.mPrecision-- ;
		}
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
			self.mOverflow (Pointer::from (self)) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mWrite < self.mRead)
				discard ;
			noop () ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 1)
				discard ;
			bitwise (self.mStream.ref[self.mWrite]) = Stru (item) ;
			self.mWrite++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 2)
				discard ;
			bitwise (self.mStream.ref[self.mWrite]) = Stru16 (item) ;
			self.mWrite++ ;
		}
		if ifdo (act) {
			if (self.mStream->step () != 4)
				discard ;
			bitwise (self.mStream.ref[self.mWrite]) = Stru32 (item) ;
			self.mWrite++ ;
		}
	}

	void write (CR<Slice> item) override {
		for (auto &&i : range (0 ,item.size ())) {
			assume (inline_mid (Index (item[i]) ,0 ,128)) ;
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

	void write (CR<typeof (TAB)>) override {
		if (self.mTabAlign <= 0)
			return ;
		const auto r1x = inline_max (self.mWrite - self.mTabIndex ,0) ;
		const auto r2x = inline_max (r1x - self.mTabAlign ,0) ;
		self.mWrite -= r2x ;
		for (auto &&i : range (0 ,self.mTabAlign - r1x)) {
			noop (i) ;
			write (Stru32 (' ')) ;
		}
		self.mTabIndex = self.mWrite ;
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
		self.mWrite = 0 ;
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
					if (!inline_mid (r1x ,0 ,self.mWrite))
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
					if (!inline_mid (r2x ,0 ,self.mWrite))
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
		assert (params.rank () <= self.mParams.size ()) ;
		Index ix = 0 ;
		for (auto &&i : range (0 ,params.rank ())) {
			self.mPin->mParams[ix] = params[i] ;
			ix++ ;
		}
		self.mPin->mWrite = ix ;
	}
} ;

exports VFat<FormatHolder> FormatHolder::hold (VR<FormatLayout> that) {
	return VFat<FormatHolder> (FormatImplHolder () ,that) ;
}

exports CFat<FormatHolder> FormatHolder::hold (CR<FormatLayout> that) {
	return CFat<FormatHolder> (FormatImplHolder () ,that) ;
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
		assert (inline_mid (align ,0 ,rax.mBuffer.size ())) ;
		rax.mWrite = rax.mBuffer.size () ;
		auto rbx = MathProc::abs (number) ;
		for (auto &&i : range (0 ,align)) {
			noop (i) ;
			rax.mWrite-- ;
			rax.mBuffer[rax.mWrite] = Stru (StreamProc::str_from_hex (rbx % 10)) ;
			rbx /= 10 ;
		}
		for (auto &&i : range (rax.mWrite ,rax.mBuffer.size ())) {
			writer.write (Stru32 (rax.mBuffer[i])) ;
		}
	}

	void read_base64u (CR<Reader> reader ,VR<RefBuffer<Byte>> item) const override {
		static const ARR<Val32 ,ENUM<256>> mCache {
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,62 ,-1 ,62 ,-1 ,63 ,
			52 ,53 ,54 ,55 ,56 ,57 ,58 ,59 ,60 ,61 ,-1 ,-1 ,-1 ,64 ,-1 ,-1 ,
			-1 ,+0 ,+1 ,+2 ,+3 ,+4 ,+5 ,+6 ,+7 ,+8 ,+9 ,10 ,11 ,12 ,13 ,14 ,
			15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24 ,25 ,-1 ,-1 ,-1 ,-1 ,63 ,
			-1 ,26 ,27 ,28 ,29 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,40 ,
			41 ,42 ,43 ,44 ,45 ,46 ,47 ,48 ,49 ,50 ,51 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
			-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1} ;
		auto rax = Stru32 () ;
		const auto r1x = reader.shape () ;
		if ifdo (TRUE) {
			reader.read (rax) ;
			while (TRUE) {
				if (rax == Stru32 (0X00))
					break ;
				const auto r2x = mCache[Val32 (Byte (rax))] ;
				if (r2x == -1)
					break ;
				reader.read (rax) ;
			}
		}
		const auto r3x = reader.length () - 1 - r1x.mRead ;
		assume (r3x % 4 == 0) ;
		reader.reset (r1x) ;
		const auto r4x = r3x / 4 ;
		auto rbx = Buffer4<Byte> () ;
		item = RefBuffer<Byte> (r4x * 3) ;
		for (auto &&i : range (0 ,r4x)) {
			Index ix = i * 3 ;
			reader >> rax ;
			rbx[0] = Byte (rax) ;
			reader >> rax ;
			rbx[1] = Byte (rax) ;
			reader >> rax ;
			rbx[2] = Byte (rax) ;
			reader >> rax ;
			rbx[3] = Byte (rax) ;
			const auto r5x = Byte (mCache[Val32 (rbx[0])]) ;
			const auto r6x = Byte (mCache[Val32 (rbx[1])]) ;
			const auto r7x = Byte (mCache[Val32 (rbx[2])]) ;
			const auto r8x = Byte (mCache[Val32 (rbx[3])]) ;
			item[ix + 0] = ByteProc::shift (r5x ,(r6x << 2) ,6) ;
			item[ix + 1] = ByteProc::shift (r6x ,(r7x << 2) ,4) ;
			item[ix + 2] = ByteProc::shift (r7x ,(r8x << 2) ,2) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (Stru32 (rbx[2]) != Stru32 ('='))
				discard ;
			if (Stru32 (rbx[3]) != Stru32 ('='))
				discard ;
			Index ix = (r4x - 1) * 3 ;
			const auto r9x = Byte (mCache[Val32 (rbx[0])]) ;
			const auto r10x = Byte (mCache[Val32 (rbx[1])]) ;
			item[ix + 0] = ByteProc::shift (r9x ,(r10x << 2) ,6) ;
			item.resize (ix + 1) ;
		}
		if ifdo (act) {
			if (Stru32 (rbx[3]) != Stru32 ('='))
				discard ;
			Index ix = (r4x - 1) * 3 ;
			const auto r11x = Byte (mCache[Val32 (rbx[0])]) ;
			const auto r12x = Byte (mCache[Val32 (rbx[1])]) ;
			const auto r13x = Byte (mCache[Val32 (rbx[2])]) ;
			item[ix + 0] = ByteProc::shift (r11x ,(r12x << 2) ,6) ;
			item[ix + 1] = ByteProc::shift (r12x ,(r13x << 2) ,4) ;
			item.resize (ix + 2) ;
		}
	}

	void write_base64u (CR<Writer> writer ,CR<RefBuffer<Byte>> item) const override {
		static const ARR<Stra ,ENUM<65>> mCache {
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"} ;
		const auto r1x = item.size () / 3 ;
		const auto r2x = item.size () - r1x * 3 ;
		auto rax = Buffer4<Val32> () ;
		for (auto &&i : range (0 ,r1x)) {
			Index ix = i * 3 ;
			const auto r3x = item[ix + 0] ;
			const auto r4x = item[ix + 1] ;
			const auto r5x = item[ix + 2] ;
			rax[0] = Val32 ((r3x >> 2) & Byte (0X3F)) ;
			rax[1] = Val32 (ByteProc::shift (r3x ,r4x ,4) & Byte (0X3F)) ;
			rax[2] = Val32 (ByteProc::shift (r4x ,r5x ,6) & Byte (0X3F)) ;
			rax[3] = Val32 (r5x & Byte (0X3F)) ;
			writer.write (Stru32 (mCache[rax[0]])) ;
			writer.write (Stru32 (mCache[rax[1]])) ;
			writer.write (Stru32 (mCache[rax[2]])) ;
			writer.write (Stru32 (mCache[rax[3]])) ;
		}
		auto act = TRUE ;
		if ifdo (act) {
			if (r2x != 1)
				discard ;
			Index ix = item.size () - r2x ;
			const auto r6x = item[ix + 0] ;
			rax[0] = Val32 ((r6x >> 2) & Byte (0X3F)) ;
			rax[1] = Val32 ((r6x << 4) & Byte (0X3F)) ;
			writer.write (Stru32 (mCache[rax[0]])) ;
			writer.write (Stru32 (mCache[rax[1]])) ;
			writer.write (Stru32 ('=')) ;
			writer.write (Stru32 ('=')) ;
		}
		if ifdo (act) {
			if (r2x != 2)
				discard ;
			Index ix = item.size () - r2x ;
			const auto r7x = item[ix + 0] ;
			const auto r8x = item[ix + 1] ;
			rax[0] = Val32 ((r7x >> 2) & Byte (0X3F)) ;
			rax[1] = Val32 (ByteProc::shift (r7x ,r8x ,4) & Byte (0X3F)) ;
			rax[2] = Val32 ((r8x << 2) & Byte (0X3F)) ;
			writer.write (Stru32 (mCache[rax[0]])) ;
			writer.write (Stru32 (mCache[rax[1]])) ;
			writer.write (Stru32 (mCache[rax[2]])) ;
			writer.write (Stru32 ('=')) ;
		}
	}
} ;

exports CR<Super<UniqueRef<StreamTextProcLayout>>> StreamTextProcHolder::expr_m () {
	return memorize ([&] () {
		Super<UniqueRef<StreamTextProcLayout>> ret ;
		ret.mThis = UniqueRef<StreamTextProcLayout>::make () ;
		auto rax = ret.mThis.borrow () ;
		StreamTextProcHolder::hold (rax.ref)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<StreamTextProcHolder> StreamTextProcHolder::hold (VR<StreamTextProcLayout> that) {
	return VFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}

exports CFat<StreamTextProcHolder> StreamTextProcHolder::hold (CR<StreamTextProcLayout> that) {
	return CFat<StreamTextProcHolder> (StreamTextProcImplHolder () ,that) ;
}
} ;