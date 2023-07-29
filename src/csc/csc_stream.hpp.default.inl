#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require 'csc_stream.hpp'"
#endif

#include "csc_stream.hpp"

namespace CSC {
template <class DEPEND>
trait BYTEATTRIBUTE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		void initialize (RREF<CRef<Holder>> prefix) override {
			assert (FALSE) ;
		}

		BYTE ending_item () const override {
			return BYTE (0X00) ;
		}

		BYTE space_item () const override {
			return BYTE (0XCC) ;
		}

		BOOL is_big_endian () const override {
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (r1x) ;
				if (r2x[0] != BYTE (0X00))
					return FALSE ;
				return TRUE ;
			}) ;
		}
	} ;
} ;

template <>
exports auto BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename BYTEATTRIBUTE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Binder = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Binder ;
	using Layout = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Layout ;
	using ByteReader = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::ByteReader ;

	class ImplHolder implement Holder {
	protected:
		ByteAttribute mAttribute ;
		CRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		void initialize (RREF<CRef<RegBuffer<BYTE>>> stream) override {
			mAttribute = ByteAttribute (TRUE) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		VREF<ByteAttribute> attribute () leftvalue override {
			return mAttribute ;
		}

		LENGTH size () const override {
			return mStream->size () ;
		}

		LENGTH length () const override {
			return mRead ;
		}

		CREF<RegBuffer<BYTE>> raw (RREF<RegCaches<BYTE>> unnamed) const leftvalue override {
			return RegBuffer<BYTE>::from (mStream.self ,0 ,length () ,move (unnamed)) ;
		}

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (read_ >= 0) ;
			assert (write_ >= 0) ;
			assert (length () <= size ()) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () override {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () override {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		template <class ARG1>
		ARG1 poll (CREF<TYPEID<ARG1>> id) {
			ARG1 ret ;
			read (ret) ;
			return move (ret) ;
		}

		void read (VREF<BYTE> item) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (mRead >= mStream->size ())
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (act) {
				item = mAttribute.ending_item () ;
			}
		}

		void read (VREF<WORD> item) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				auto rax = BoxBuffer<BYTE ,SIZE_OF<WORD>> (0) ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr))
					read (rax[i]) ;
				item = bitwise[TYPEAS<WORD>::expr] (rax) ;
			}
			if ifswitch (act) {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<WORD>> (0) ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
					INDEX ix = SIZE_OF<WORD>::expr - 1 - i ;
					read (rax[ix]) ;
				}
				item = bitwise[TYPEAS<WORD>::expr] (rax) ;
			}
		}

		void read (VREF<CHAR> item) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> (0) ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr))
					read (rax[i]) ;
				item = bitwise[TYPEAS<CHAR>::expr] (rax) ;
			}
			if ifswitch (act) {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> (0) ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
					INDEX ix = SIZE_OF<CHAR>::expr - 1 - i ;
					read (rax[ix]) ;
				}
				item = bitwise[TYPEAS<CHAR>::expr] (rax) ;
			}
		}

		void read (VREF<DATA> item) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				auto rax = BoxBuffer<BYTE ,SIZE_OF<DATA>> (0) ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr))
					read (rax[i]) ;
				item = bitwise[TYPEAS<DATA>::expr] (rax) ;
			}
			if ifswitch (act) {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<DATA>> (0) ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
					INDEX ix = SIZE_OF<DATA>::expr - 1 - i ;
					read (rax[ix]) ;
				}
				item = bitwise[TYPEAS<DATA>::expr] (rax) ;
			}
		}

		void read (VREF<BOOL> item) override {
			using R1X = DEPENDENT<BYTE_BASE<BOOL> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<BOOL>::expr] (rax) ;
		}

		void read (VREF<VAL32> item) override {
			using R1X = DEPENDENT<BYTE_BASE<VAL32> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<VAL32>::expr] (rax) ;
		}

		void read (VREF<VAL64> item) override {
			using R1X = DEPENDENT<BYTE_BASE<VAL64> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<VAL64>::expr] (rax) ;
		}

		void read (VREF<SINGLE> item) override {
			using R1X = DEPENDENT<BYTE_BASE<SINGLE> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<SINGLE>::expr] (rax) ;
		}

		void read (VREF<DOUBLE> item) override {
			using R1X = DEPENDENT<BYTE_BASE<DOUBLE> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<DOUBLE>::expr] (rax) ;
		}

		void read (CREF<Slice<STR>> item) override {
			auto rax = BYTE () ;
			for (auto &&i : iter (0 ,item.size ())) {
				assume (vbetween (INDEX (item[i]) ,0 ,128)) ;
				read (rax) ;
				assume (rax == BYTE (item[i])) ;
			}
		}

		void read (VREF<String<STRA>> item) override {
			using R1X = DEPENDENT<BYTE_BASE<STRA> ,DEPEND> ;
			const auto r1x = poll (TYPEAS<VAL32>::expr) ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				assume (r1x <= VAL32_MAX) ;
				item = String<STRA> (r1x) ;
			}
			item.trunc (r1x) ;
			auto rax = R1X () ;
			for (auto &&i : iter (0 ,r1x)) {
				read (rax) ;
				item[i] = bitwise[TYPEAS<STRA>::expr] (rax) ;
			}
			read (rax) ;
			assume (rax == R1X (0X00)) ;
		}

		void read (VREF<String<STRW>> item) override {
			using R1X = DEPENDENT<BYTE_BASE<STRW> ,DEPEND> ;
			const auto r1x = poll (TYPEAS<VAL32>::expr) ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				assume (r1x <= VAL32_MAX) ;
				item = String<STRW> (r1x) ;
			}
			item.trunc (r1x) ;
			auto rax = R1X () ;
			for (auto &&i : iter (0 ,r1x)) {
				read (rax) ;
				item[i] = bitwise[TYPEAS<STRW>::expr] (rax) ;
			}
			read (rax) ;
			assume (rax == R1X (0X00)) ;
		}

		void read (VREF<String<STRU8>> item) override {
			using R1X = DEPENDENT<BYTE_BASE<STRU8> ,DEPEND> ;
			const auto r1x = poll (TYPEAS<VAL32>::expr) ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				assume (r1x <= VAL32_MAX) ;
				item = String<STRU8> (r1x) ;
			}
			item.trunc (r1x) ;
			auto rax = R1X () ;
			for (auto &&i : iter (0 ,r1x)) {
				read (rax) ;
				item[i] = bitwise[TYPEAS<STRU8>::expr] (rax) ;
			}
			read (rax) ;
			assume (rax == R1X (0X00)) ;
		}

		void read (VREF<String<STRU16>> item) override {
			using R1X = DEPENDENT<BYTE_BASE<STRU16> ,DEPEND> ;
			const auto r1x = poll (TYPEAS<VAL32>::expr) ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				assume (r1x <= VAL32_MAX) ;
				item = String<STRU16> (r1x) ;
			}
			item.trunc (r1x) ;
			auto rax = R1X () ;
			for (auto &&i : iter (0 ,r1x)) {
				read (rax) ;
				item[i] = bitwise[TYPEAS<STRU16>::expr] (rax) ;
			}
			read (rax) ;
			assume (rax == R1X (0X00)) ;
		}

		void read (VREF<String<STRU32>> item) override {
			using R1X = DEPENDENT<BYTE_BASE<STRU32> ,DEPEND> ;
			const auto r1x = poll (TYPEAS<VAL32>::expr) ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				assume (r1x <= VAL32_MAX) ;
				item = String<STRU32> (r1x) ;
			}
			item.trunc (r1x) ;
			auto rax = R1X () ;
			for (auto &&i : iter (0 ,r1x)) {
				read (rax) ;
				item[i] = bitwise[TYPEAS<STRU32>::expr] (rax) ;
			}
			read (rax) ;
			assume (rax == R1X (0X00)) ;
		}

		void read (VREF<Binder> item) override {
			auto rax = ByteReader (share ()) ;
			item.friend_read (rax) ;
		}

		Layout share () leftvalue {
			Layout ret ;
			ret.mThis = VRef<Holder>::reference (thiz) ;
			return move (ret) ;
		}

		void read_cls () override {
			reset () ;
		}

		void read_gap () override {
			const auto r1x = mAttribute.space_item () ;
			auto rax = BYTE () ;
			read (rax) ;
			assume (rax == r1x) ;
			read (rax) ;
			assume (rax == r1x) ;
		}

		void read_eos () override {
			const auto r1x = mAttribute.ending_item () ;
			auto rax = BYTE () ;
			for (auto &&i : iter (mRead ,mStream->size ())) {
				noop (i) ;
				read (rax) ;
				assume (rax == r1x) ;
			}
		}
	} ;
} ;

template <>
exports auto BYTEREADER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Holder ;
	using Binder = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Binder ;
	using Layout = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Layout ;
	using ByteWriter = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::ByteWriter ;

	class ImplHolder implement Holder {
	protected:
		ByteAttribute mAttribute ;
		VRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		void initialize (RREF<VRef<RegBuffer<BYTE>>> stream) override {
			mAttribute = ByteAttribute (TRUE) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		VREF<ByteAttribute> attribute () leftvalue override {
			return mAttribute ;
		}

		LENGTH size () const override {
			return mStream->size () ;
		}

		LENGTH length () const override {
			return mWrite ;
		}

		CREF<RegBuffer<BYTE>> raw (RREF<RegCaches<BYTE>> unnamed) const leftvalue override {
			return RegBuffer<BYTE>::from (mStream.self ,0 ,length () ,move (unnamed)) ;
		}

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (read_ >= 0) ;
			assert (write_ >= 0) ;
			assert (length () <= size ()) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () override {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () override {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		void write (CREF<BYTE> item) override {
			if ifswitch (TRUE) {
				if (mWrite >= mStream->size ())
					discard ;
				mStream.self[mWrite] = item ;
				mWrite++ ;
			}
		}

		void write (CREF<WORD> item) override {
			const auto r1x = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<WORD>>>::expr] (item) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr))
					write (r1x[i]) ;
			}
			if ifswitch (act) {
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
					INDEX ix = SIZE_OF<WORD>::expr - 1 - i ;
					write (r1x[ix]) ;
				}
			}
		}

		void write (CREF<CHAR> item) override {
			const auto r1x = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<CHAR>>>::expr] (item) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr))
					write (r1x[i]) ;
			}
			if ifswitch (act) {
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
					INDEX ix = SIZE_OF<CHAR>::expr - 1 - i ;
					write (r1x[ix]) ;
				}
			}
		}

		void write (CREF<DATA> item) override {
			const auto r1x = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<DATA>>>::expr] (item) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr))
					write (r1x[i]) ;
			}
			if ifswitch (act) {
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
					INDEX ix = SIZE_OF<DATA>::expr - 1 - i ;
					write (r1x[ix]) ;
				}
			}
		}

		void write (CREF<BOOL> item) override {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		void write (CREF<VAL32> item) override {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		void write (CREF<VAL64> item) override {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		void write (CREF<SINGLE> item) override {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		void write (CREF<DOUBLE> item) override {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		void write (CREF<Slice<STR>> item) override {
			for (auto &&i : iter (0 ,item.size ())) {
				assume (vbetween (INDEX (item[i]) ,0 ,128)) ;
				const auto r1x = BYTE (item[i]) ;
				write (r1x) ;
			}
		}

		void write (CREF<String<STRA>> item) override {
			const auto r1x = item.length () ;
			assume (r1x <= VAL32_MAX) ;
			write (VAL32 (r1x)) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = bitwise (item[i]) ;
				write (r2x) ;
			}
			const auto r3x = bitwise (STRA (0)) ;
			write (r3x) ;
		}

		void write (CREF<String<STRW>> item) override {
			const auto r1x = item.length () ;
			assume (r1x <= VAL32_MAX) ;
			write (VAL32 (r1x)) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = bitwise (item[i]) ;
				write (r2x) ;
			}
			const auto r3x = bitwise (STRW (0)) ;
			write (r3x) ;
		}

		void write (CREF<String<STRU8>> item) override {
			const auto r1x = item.length () ;
			assume (r1x <= VAL32_MAX) ;
			write (VAL32 (r1x)) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = bitwise (item[i]) ;
				write (r2x) ;
			}
			const auto r3x = bitwise (STRU8 (0)) ;
			write (r3x) ;
		}

		void write (CREF<String<STRU16>> item) override {
			const auto r1x = item.length () ;
			assume (r1x <= VAL32_MAX) ;
			write (VAL32 (r1x)) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = bitwise (item[i]) ;
				write (r2x) ;
			}
			const auto r3x = bitwise (STRU16 (0)) ;
			write (r3x) ;
		}

		void write (CREF<String<STRU32>> item) override {
			const auto r1x = item.length () ;
			assume (r1x <= VAL32_MAX) ;
			write (VAL32 (r1x)) ;
			for (auto &&i : iter (0 ,r1x)) {
				const auto r2x = bitwise (item[i]) ;
				write (r2x) ;
			}
			const auto r3x = bitwise (STRU32 (0)) ;
			write (r3x) ;
		}

		void write (VREF<Binder> item) override {
			auto rax = ByteWriter (share ()) ;
			item.friend_write (rax) ;
		}

		void write (CREF<Binder> item) override {
			auto rax = ByteWriter (share ()) ;
			item.friend_write (rax) ;
		}

		Layout share () leftvalue {
			Layout ret ;
			ret.mThis = VRef<Holder>::reference (thiz) ;
			return move (ret) ;
		}

		void write_cls () override {
			reset () ;
		}

		void write_gap () override {
			const auto r1x = mAttribute.space_item () ;
			write (r1x) ;
			write (r1x) ;
		}

		void write_eos () override {
			const auto r1x = mAttribute.ending_item () ;
			for (auto &&i : iter (mWrite ,mStream->size ())) {
				noop (i) ;
				write (r1x) ;
			}
		}
	} ;
} ;

template <>
exports auto BYTEWRITER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class ITEM>
trait TEXTATTRIBUTE_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using Holder = typename TEXTATTRIBUTE_HELP<ITEM ,ALWAYS>::Holder ;

	struct GAP_CLAZZ {
		enum {
			Space ,
			Endline ,
			EnumSize
		} ;
	} ;

	class ImplHolder implement Holder {
	protected:
		HashSet<ITEM> mSpaceSet ;
		ArrayList<ARRAY2<ITEM>> mEscapeList ;
		HashSet<ITEM> mEscapeWordSet ;
		HashSet<ITEM> mEscapeCtrlSet ;

	public:
		void initialize () override {
			mSpaceSet.add (ITEM (' ') ,GAP_CLAZZ::Space) ;
			mSpaceSet.add (ITEM ('\t') ,GAP_CLAZZ::Space) ;
			mSpaceSet.add (ITEM ('\b') ,GAP_CLAZZ::Space) ;
			mSpaceSet.add (ITEM ('\r') ,GAP_CLAZZ::Endline) ;
			mSpaceSet.add (ITEM ('\n') ,GAP_CLAZZ::Endline) ;
			mSpaceSet.add (ITEM ('\f') ,GAP_CLAZZ::Endline) ;
			escape_list_add (ITEM ('\\') ,ITEM ('\\')) ;
			escape_list_add (ITEM ('\\') ,ITEM ('\\')) ;
			escape_list_add (ITEM ('/') ,ITEM ('/')) ;
			escape_list_add (ITEM ('t') ,ITEM ('\t')) ;
			escape_list_add (ITEM ('b') ,ITEM ('\b')) ;
			escape_list_add (ITEM ('r') ,ITEM ('\r')) ;
			escape_list_add (ITEM ('n') ,ITEM ('\n')) ;
			escape_list_add (ITEM ('f') ,ITEM ('\f')) ;
			escape_list_add (ITEM ('\'') ,ITEM ('\'')) ;
			escape_list_add (ITEM ('\"') ,ITEM ('\"')) ;
			escape_list_add (ITEM ('u') ,ITEM (7)) ;
		}

		void initialize (RREF<CRef<Holder>> prefix) override {
			assert (FALSE) ;
		}

		void escape_list_add (CREF<ITEM> word ,CREF<ITEM> ctrl) {
			INDEX ix = mEscapeList.insert () ;
			mEscapeList[ix][0] = word ;
			mEscapeList[ix][1] = ctrl ;
			mEscapeWordSet.add (word ,ix) ;
			mEscapeCtrlSet.add (ctrl ,ix) ;
		}

		ITEM ending_item () const override {
			return ITEM (0) ;
		}

		BOOL is_gap (CREF<ITEM> str) const override {
			return mSpaceSet.find (str) != NONE ;
		}

		BOOL is_gap_space (CREF<ITEM> str) const override {
			return mSpaceSet.map (str) == GAP_CLAZZ::Space ;
		}

		BOOL is_gap_endline (CREF<ITEM> str) const override {
			return mSpaceSet.map (str) == GAP_CLAZZ::Endline ;
		}

		BOOL is_word (CREF<ITEM> str) const override {
			if (str >= ITEM ('a'))
				if (str <= ITEM ('z'))
					return TRUE ;
			if (str >= ITEM ('A'))
				if (str <= ITEM ('Z'))
					return TRUE ;
			if (str == ITEM ('_'))
				return TRUE ;
			return FALSE ;
		}

		ITEM word_lower_cast (CREF<ITEM> str) const override {
			if (str >= ITEM ('A'))
				if (str <= ITEM ('Z'))
					return ITEM (INDEX (str) - INDEX ('A') + INDEX ('a')) ;
			return str ;
		}

		ITEM word_upper_cast (CREF<ITEM> str) const override {
			if (str >= ITEM ('a'))
				if (str <= ITEM ('z'))
					return ITEM (INDEX (str) - INDEX ('a') + INDEX ('A')) ;
			return str ;
		}

		BOOL is_hyphen (CREF<ITEM> str) const override {
			if (str == ITEM (':'))
				return TRUE ;
			if (str == ITEM ('-'))
				return TRUE ;
			return FALSE ;
		}

		BOOL is_number (CREF<ITEM> str) const override {
			if (str >= ITEM ('0'))
				if (str <= ITEM ('9'))
					return TRUE ;
			return FALSE ;
		}

		BOOL is_hex_number (CREF<ITEM> str) const override {
			if (str >= ITEM ('a'))
				if (str <= ITEM ('f'))
					return TRUE ;
			if (str >= ITEM ('A'))
				if (str <= ITEM ('F'))
					return TRUE ;
			return FALSE ;
		}

		INDEX hex_from_str (CREF<ITEM> str) const override {
			if (is_number (str))
				return INDEX (str) - INDEX ('0') ;
			if (is_hex_number (str))
				return INDEX (word_upper_cast (str)) - INDEX ('A') + 10 ;
			assume (FALSE) ;
			return bad (TYPEAS<ITEM>::expr) ;
		}

		ITEM str_from_hex (CREF<INDEX> hex) const override {
			if (vbetween (hex ,0 ,10))
				return ITEM (INDEX ('0') + hex) ;
			if (vbetween (hex ,10 ,16))
				return ITEM (INDEX ('A') + hex - 10) ;
			assume (FALSE) ;
			return bad (TYPEAS<ITEM>::expr) ;
		}

		BOOL is_control (CREF<ITEM> str) const override {
			if (str == ITEM (127))
				return TRUE ;
			if (str <= ITEM (0))
				return FALSE ;
			if (str >= ITEM (32))
				return FALSE ;
			if (mSpaceSet.find (str) != NONE)
				return FALSE ;
			return TRUE ;
		}

		Optional<ITEM> escape_word_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeCtrlSet.map (str) ;
			if (ix == NONE)
				return FLAG (1) ;
			return Optional<ITEM>::make (mEscapeList[ix][0]) ;
		}

		Optional<ITEM> escape_ctrl_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeWordSet.map (str) ;
			if (ix == NONE)
				return FLAG (1) ;
			return Optional<ITEM>::make (mEscapeList[ix][1]) ;
		}

		LENGTH value_precision () const override {
			return 18 ;
		}

		LENGTH float_precision () const override {
			return 15 ;
		}

		LENGTH number_precision () const override {
			return 308 ;
		}
	} ;
} ;

exports auto TEXTATTRIBUTE_HELP<STRA ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRW ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRU8 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRU16 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRU32 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class ITEM>
trait TEXTREADER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using Holder = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Holder ;
	using Binder = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Binder ;
	using Layout = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Layout ;
	using TextReader = typename TEXTREADER_HELP<ITEM ,ALWAYS>::TextReader ;

	class ImplHolder implement Holder {
	protected:
		TextAttribute<ITEM> mAttribute ;
		CRef<RegBuffer<ITEM>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		void initialize (RREF<CRef<RegBuffer<ITEM>>> stream) override {
			mAttribute = TextAttribute<ITEM> (TRUE) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		VREF<TextAttribute<ITEM>> attribute () leftvalue override {
			return mAttribute ;
		}

		LENGTH size () const override {
			return mStream->size () ;
		}

		LENGTH length () const override {
			return mRead ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed) const leftvalue override {
			return RegBuffer<ITEM>::from (mStream.self ,0 ,length () ,move (unnamed)) ;
		}

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (read_ >= 0) ;
			assert (write_ >= 0) ;
			assert (length () <= size ()) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () override {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () override {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		template <class ARG1>
		ARG1 poll (CREF<TYPEID<ARG1>> id) {
			ARG1 ret ;
			read (ret) ;
			return move (ret) ;
		}

		void read (VREF<ITEM> item) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if (mRead >= mStream->size ())
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (act) {
				item = mAttribute.ending_item () ;
			}
		}

		void read (VREF<BOOL> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			auto act = TRUE ;
			if ifswitch (act) {
				if (rax != ITEM ('t'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('r')) ;
				read (rax) ;
				assume (rax == ITEM ('u')) ;
				read (rax) ;
				assume (rax == ITEM ('e')) ;
				item = TRUE ;
			}
			if ifswitch (act) {
				if (rax != ITEM ('T'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('R')) ;
				read (rax) ;
				assume (rax == ITEM ('U')) ;
				read (rax) ;
				assume (rax == ITEM ('E')) ;
				item = TRUE ;
			}
			if ifswitch (act) {
				if (rax != ITEM ('f'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('a')) ;
				read (rax) ;
				assume (rax == ITEM ('l')) ;
				read (rax) ;
				assume (rax == ITEM ('s')) ;
				read (rax) ;
				assume (rax == ITEM ('e')) ;
				item = FALSE ;
			}
			if ifswitch (act) {
				if (rax != ITEM ('F'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('A')) ;
				read (rax) ;
				assume (rax == ITEM ('L')) ;
				read (rax) ;
				assume (rax == ITEM ('S')) ;
				read (rax) ;
				assume (rax == ITEM ('E')) ;
				item = FALSE ;
			}
			if ifswitch (act) {
				assume (FALSE) ;
			}
		}

		void read (VREF<VAL32> item) override {
			const auto r1x = poll (TYPEAS<VAL64>::expr) ;
			assume (r1x >= VAL32_MIN) ;
			assume (r1x <= VAL32_MAX) ;
			item = VAL32 (r1x) ;
		}

		void read (VREF<VAL64> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			const auto r1x = BOOL (rax == ITEM ('-')) ;
			if ifswitch (TRUE) {
				if (rax != ITEM ('-'))
					if (rax != ITEM ('+'))
						discard ;
				read (rax) ;
			}
			auto act = TRUE ;
			if ifswitch (act) {
				if (rax != ITEM ('a'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('b')) ;
				read (rax) ;
				assume (rax == ITEM ('s')) ;
				item = VAL64_ABS ;
			}
			if ifswitch (act) {
				if (rax != ITEM ('A'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('B')) ;
				read (rax) ;
				assume (rax == ITEM ('S')) ;
				item = VAL64_ABS ;
			}
			if ifswitch (act) {
				assume (mAttribute.is_number (rax)) ;
				auto rbx = NOTATION () ;
				rbx.mRadix = 10 ;
				rbx.mSign = FALSE ;
				rbx.mPrecision = 0 ;
				read_value (rbx ,rax) ;
				item = rbx.mMantissa ;
			}
			if ifswitch (TRUE) {
				if ifnot (r1x)
					discard ;
				item = -item ;
			}
			push_back (rax) ;
		}

		void read_value (VREF<NOTATION> fexp10 ,VREF<ITEM> top) {
			assert (fexp10.mRadix == 10) ;
			const auto r1x = mAttribute.value_precision () ;
			const auto r2x = mAttribute.number_precision () ;
			fexp10.mMantissa = 0 ;
			fexp10.mPrecision = 0 ;
			fexp10.mExponent = 0 ;
			if ifswitch (TRUE) {
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					if ifnot (mAttribute.is_number (top))
						break ;
					if (fexp10.mPrecision > r1x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += mAttribute.hex_from_str (top) ;
					fexp10.mPrecision++ ;
					read (top) ;
				}
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					if ifnot (mAttribute.is_number (top))
						break ;
					if (fexp10.mPrecision > r2x)
						break ;
					fexp10.mExponent++ ;
					fexp10.mPrecision++ ;
					read (top) ;
				}
				assume (fexp10.mPrecision <= r1x) ;
			}
		}

		void read (VREF<SINGLE> item) override {
			const auto r1x = poll (TYPEAS<DOUBLE>::expr) ;
			assume (r1x >= SINGLE_MIN) ;
			assume (r1x <= SINGLE_MAX) ;
			item = SINGLE (r1x) ;
		}

		void read (VREF<DOUBLE> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			const auto r1x = BOOL (rax == ITEM ('-')) ;
			if ifswitch (TRUE) {
				if (rax != ITEM ('-'))
					if (rax != ITEM ('+'))
						discard ;
				read (rax) ;
			}
			auto act = TRUE ;
			if ifswitch (act) {
				if (rax != ITEM ('i'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('n')) ;
				read (rax) ;
				assume (rax == ITEM ('f')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (act) {
				if (rax != ITEM ('I'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('N')) ;
				read (rax) ;
				assume (rax == ITEM ('F')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (act) {
				assume (mAttribute.is_number (rax)) ;
				auto rbx = NOTATION () ;
				rbx.mRadix = 10 ;
				rbx.mSign = FALSE ;
				rbx.mPrecision = 0 ;
				read_float (rbx ,rax) ;
				rbx = FloatProc::fexp2_from_fexp10 (rbx) ;
				item = FloatProc::encode (rbx) ;
			}
			if ifswitch (TRUE) {
				if ifnot (r1x)
					discard ;
				item = -item ;
			}
			push_back (rax) ;
		}

		void read_float (VREF<NOTATION> fexp10 ,VREF<ITEM> top) {
			assert (fexp10.mRadix == 10) ;
			const auto r1x = mAttribute.value_precision () ;
			const auto r2x = mAttribute.number_precision () ;
			read_value (fexp10 ,top) ;
			if ifswitch (TRUE) {
				if (top != ITEM ('.'))
					discard ;
				read (top) ;
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					if ifnot (mAttribute.is_number (top))
						break ;
					if (fexp10.mPrecision > r1x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += mAttribute.hex_from_str (top) ;
					fexp10.mExponent-- ;
					fexp10.mPrecision++ ;
					read (top) ;
				}
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					if ifnot (mAttribute.is_number (top))
						break ;
					if (fexp10.mPrecision > r2x)
						break ;
					fexp10.mPrecision++ ;
					read (top) ;
				}
				assume (fexp10.mPrecision <= r2x) ;
			}
			if ifswitch (TRUE) {
				if (top != ITEM ('e'))
					if (top != ITEM ('E'))
						discard ;
				read (top) ;
				const auto r3x = BOOL (top == ITEM ('-')) ;
				if ifswitch (TRUE) {
					if (top != ITEM ('-'))
						if (top != ITEM ('+'))
							discard ;
					read (top) ;
				}
				assume (mAttribute.is_number (top)) ;
				auto rbx = NOTATION () ;
				rbx.mRadix = 10 ;
				rbx.mSign = r3x ;
				rbx.mPrecision = 0 ;
				read_value (rbx ,top) ;
				assume (rbx.mExponent == 0) ;
				const auto r4x = invoke ([&] () {
					if (r3x)
						return -rbx.mMantissa ;
					return rbx.mMantissa ;
				}) ;
				assume (MathProc::abs (r4x) < r2x) ;
				fexp10.mExponent += r4x ;
				assume (MathProc::abs (fexp10.mExponent) < r2x) ;
			}
		}

		void read (VREF<BYTE> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = BYTE (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<BYTE>::expr)) {
				noop (i) ;
				read (rax) ;
				const auto r1x = BYTE (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = BYTE (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		void read (VREF<WORD> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = WORD (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
				noop (i) ;
				read (rax) ;
				const auto r1x = WORD (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = WORD (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		void read (VREF<CHAR> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = CHAR (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
				noop (i) ;
				read (rax) ;
				const auto r1x = CHAR (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = CHAR (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		void read (VREF<DATA> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = DATA (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
				noop (i) ;
				read (rax) ;
				const auto r1x = DATA (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = DATA (mAttribute.hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		void read (CREF<Slice<STR>> item) override {
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,item.size ())) {
				assume (vbetween (INDEX (item[i]) ,0 ,128)) ;
				read (rax) ;
				assume (rax == ITEM (item[i])) ;
			}
		}

		void read (VREF<String<ITEM>> item) override {
			const auto r1x = next_gap_size () ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				assume (r1x <= VAL32_MAX) ;
				item = String<ITEM> (r1x) ;
			}
			item.trunc (r1x) ;
			for (auto &&i : iter (0 ,r1x))
				read (item[i]) ;
		}

		LENGTH next_gap_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
			read (rax) ;
			while (TRUE) {
				if (rax == mAttribute.ending_item ())
					break ;
				if (mAttribute.is_gap (rax))
					break ;
				ret++ ;
				read (rax) ;
			}
			mRead = r1x ;
			return move (ret) ;
		}

		void read (VREF<Binder> item) override {
			auto rax = TextReader (share ()) ;
			item.friend_read (rax) ;
		}

		Layout share () leftvalue {
			Layout ret ;
			ret.mThis = VRef<Holder>::reference (thiz) ;
			return move (ret) ;
		}

		void read_cls () override {
			reset () ;
		}

		void read_bom () override {
			read_bom (TYPEAS<ITEM>::expr) ;
		}

		void read_bom (CREF<TYPEID<STRA>> id) {
			noop () ;
		}

		void read_bom (CREF<TYPEID<STRW>> id) {
			read_bom (TYPEAS<STRUW>::expr) ;
		}

		void read_bom (CREF<TYPEID<STRU8>> id) {
			auto rax = ITEM () ;
			auto rbx = mRead ;
			if ifswitch (TRUE) {
				read (rax) ;
				if (rax != ITEM (0XEF))
					discard ;
				read (rax) ;
				if (rax != ITEM (0XBB))
					discard ;
				read (rax) ;
				if (rax != ITEM (0XBF))
					discard ;
				rbx = mRead ;
			}
			mRead = rbx ;
		}

		void read_bom (CREF<TYPEID<STRU16>> id) {
			auto rax = ITEM () ;
			auto rbx = mRead ;
			if ifswitch (TRUE) {
				read (rax) ;
				if (rax != ITEM (0XFEFF))
					discard ;
				read (rax) ;
				if (rax != ITEM (0XFFFE))
					discard ;
				rbx = mRead ;
			}
			mRead = rbx ;
		}

		void read_bom (CREF<TYPEID<STRU32>> id) {
			auto rax = ITEM () ;
			auto rbx = mRead ;
			if ifswitch (TRUE) {
				read (rax) ;
				if (rax != ITEM (0X0000FEFF))
					discard ;
				read (rax) ;
				if (rax != ITEM (0XFFFE0000))
					discard ;
				rbx = mRead ;
			}
			mRead = rbx ;
		}

		void read_gap () override {
			const auto r1x = next_not_gap_size () ;
			mRead += r1x ;
		}

		LENGTH next_not_gap_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
			read (rax) ;
			while (TRUE) {
				if (rax == mAttribute.ending_item ())
					break ;
				if ifnot (mAttribute.is_gap (rax))
					break ;
				ret++ ;
				read (rax) ;
			}
			mRead = r1x ;
			return move (ret) ;
		}

		void read_eos () override {
			const auto r1x = mAttribute.ending_item () ;
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == r1x) ;
		}

		void push_back (CREF<ITEM> top) {
			if ifswitch (TRUE) {
				mRead-- ;
				if (mRead < mStream->size () - 1)
					discard ;
				if (top != mAttribute.ending_item ())
					discard ;
				mRead++ ;
			}
		}
	} ;
} ;

exports auto TEXTREADER_HELP<STRA ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRW ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRU8 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRU16 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRU32 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait TEXTWRITER_WRITEVALUE_HELP ;

template <class ITEM>
trait TEXTWRITER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using Holder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Holder ;
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;
	using Layout = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Layout ;
	using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

	class ImplHolder implement Holder {
	protected:
		TextAttribute<ITEM> mAttribute ;
		VRef<RegBuffer<ITEM>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		void initialize (RREF<VRef<RegBuffer<ITEM>>> stream) override {
			mAttribute = TextAttribute<ITEM> (TRUE) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		VREF<TextAttribute<ITEM>> attribute () leftvalue override {
			return mAttribute ;
		}

		LENGTH size () const override {
			return mStream->size () ;
		}

		LENGTH length () const override {
			return mWrite ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed) const leftvalue override {
			return RegBuffer<ITEM>::from (mStream.self ,0 ,length () ,move (unnamed)) ;
		}

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (read_ >= 0) ;
			assert (write_ >= 0) ;
			assert (length () <= size ()) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () override {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () override {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		void write (CREF<ITEM> item) override {
			if ifswitch (TRUE) {
				if (mWrite >= mStream->size ())
					discard ;
				mStream.self[mWrite] = item ;
				mWrite++ ;
			}
		}

		void write (CREF<BOOL> item) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (item)
					discard ;
				write (ITEM ('t')) ;
				write (ITEM ('r')) ;
				write (ITEM ('u')) ;
				write (ITEM ('e')) ;
			}
			if ifswitch (act) {
				if (item)
					discard ;
				write (ITEM ('f')) ;
				write (ITEM ('a')) ;
				write (ITEM ('l')) ;
				write (ITEM ('s')) ;
				write (ITEM ('e')) ;
			}
		}

		void write (CREF<VAL32> item) override {
			write (VAL64 (item)) ;
		}

		void write (CREF<VAL64> item) override {
			using R1X = typename DEPENDENT<TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS> ,DEPEND>::WriteValue ;
			if ifswitch (TRUE) {
				if (item >= 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto act = TRUE ;
			if ifswitch (act) {
				if (item != VAL64_ABS)
					discard ;
				write (ITEM ('a')) ;
				write (ITEM ('b')) ;
				write (ITEM ('s')) ;
			}
			if ifswitch (act) {
				const auto r1x = invoke ([&] () {
					NOTATION ret ;
					ret.mRadix = 10 ;
					ret.mSign = FALSE ;
					ret.mPrecision = 0 ;
					ret.mMantissa = MathProc::abs (item) ;
					ret.mExponent = 0 ;
					return move (ret) ;
				}) ;
				auto rax = R1X (mAttribute ,r1x) ;
				rax.write_value () ;
				write (rax) ;
			}
		}

		void write (CREF<SINGLE> item) override {
			write (DOUBLE (item)) ;
		}

		void write (CREF<DOUBLE> item) override {
			using R1X = typename DEPENDENT<TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS> ,DEPEND>::WriteValue ;
			if ifswitch (TRUE) {
				if (item >= 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (MathProc::is_inf (item))
					discard ;
				write (ITEM ('i')) ;
				write (ITEM ('n')) ;
				write (ITEM ('f')) ;
			}
			if ifswitch (act) {
				const auto r1x = FloatProc::decode (MathProc::abs (item)) ;
				const auto r2x = FloatProc::fexp10_from_fexp2 (r1x) ;
				auto rax = R1X (mAttribute ,r2x) ;
				rax.write_float () ;
				write (rax) ;
			}
		}

		void write (CREF<BYTE> item) override {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<BYTE>::expr)) {
				INDEX ix = (SIZE_OF<BYTE>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & BYTE (0X0F)) ;
				write (mAttribute.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & BYTE (0X0F)) ;
				write (mAttribute.str_from_hex (r2x)) ;
			}
		}

		void write (CREF<WORD> item) override {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
				INDEX ix = (SIZE_OF<WORD>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & WORD (0X0F)) ;
				write (mAttribute.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & WORD (0X0F)) ;
				write (mAttribute.str_from_hex (r2x)) ;
			}
		}

		void write (CREF<CHAR> item) override {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
				INDEX ix = (SIZE_OF<CHAR>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & CHAR (0X0F)) ;
				write (mAttribute.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & CHAR (0X0F)) ;
				write (mAttribute.str_from_hex (r2x)) ;
			}
		}

		void write (CREF<DATA> item) override {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
				INDEX ix = (SIZE_OF<DATA>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & DATA (0X0F)) ;
				write (mAttribute.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & DATA (0X0F)) ;
				write (mAttribute.str_from_hex (r2x)) ;
			}
		}

		void write (CREF<Slice<STR>> item) override {
			for (auto &&i : iter (0 ,item.size ())) {
				assume (vbetween (INDEX (item[i]) ,0 ,128)) ;
				const auto r1x = ITEM (item[i]) ;
				write (r1x) ;
			}
		}

		void write (CREF<String<ITEM>> item) override {
			const auto r1x = item.length () ;
			assume (r1x <= VAL32_MAX) ;
			for (auto &&i : item)
				write (i) ;
		}

		void write (VREF<Binder> item) override {
			auto rax = TextWriter (share ()) ;
			item.friend_write (rax) ;
		}

		void write (CREF<Binder> item) override {
			auto rax = TextWriter (share ()) ;
			item.friend_write (rax) ;
		}

		Layout share () leftvalue {
			Layout ret ;
			ret.mThis = VRef<Holder>::reference (thiz) ;
			return move (ret) ;
		}

		void write_cls () override {
			reset () ;
		}

		void write_bom () override {
			write_bom (TYPEAS<ITEM>::expr) ;
		}

		void write_bom (CREF<TYPEID<STRA>> id) {
			noop () ;
		}

		void write_bom (CREF<TYPEID<STRW>> id) {
			write_bom (TYPEAS<STRUW>::expr) ;
		}

		void write_bom (CREF<TYPEID<STRU8>> id) {
			write (ITEM (0XEF)) ;
			write (ITEM (0XBB)) ;
			write (ITEM (0XBF)) ;
		}

		void write_bom (CREF<TYPEID<STRU16>> id) {
			write (ITEM (0XFEFF)) ;
			write (ITEM (0XFFFE)) ;
		}

		void write_bom (CREF<TYPEID<STRU32>> id) {
			write (ITEM (0X0000FEFF)) ;
			write (ITEM (0XFFFE0000)) ;
		}

		void write_gap () override {
			write (ITEM ('\r')) ;
			write (ITEM ('\n')) ;
		}

		void write_eos () override {
			const auto r1x = mAttribute.ending_item () ;
			write (r1x) ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS> {
	using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;

	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using NUMBER_SIZE = ENUMAS<VAL ,64> ;

	class WriteValue implement Binder {
	protected:
		TextAttribute<ITEM> mAttribute ;
		NOTATION mValue ;
		BoxBuffer<ITEM ,NUMBER_SIZE> mBuffer ;
		INDEX mWrite ;

	public:
		implicit WriteValue () = delete ;

		explicit WriteValue (CREF<TextAttribute<ITEM>> attribute_ ,CREF<NOTATION> value_) {
			assert (value_.mRadix == 10) ;
			mAttribute = attribute_ ;
			mValue = value_ ;
			mValue.mPrecision = 1 + MathProc::vlog (mValue.mMantissa ,VAL64 (10)) ;
			mBuffer = BoxBuffer<ITEM ,NUMBER_SIZE> (0) ;
			mWrite = mBuffer.size () ;
		}

		void friend_write (VREF<TextWriter> writer) const override {
			for (auto &&i : iter (mWrite ,mBuffer.size ()))
				writer.write (mBuffer[i]) ;
		}

		void write_value () {
			const auto r1x = mValue.mPrecision ;
			auto act = TRUE ;
			if ifswitch (act) {
				//@info: case '0'
				if (mValue.mMantissa != 0)
					discard ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('0') ;
			}
			if ifswitch (act) {
				//@info: case 'xxx'
				for (auto &&i : iter (0 ,r1x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
			}
			if ifswitch (TRUE) {
				if ifnot (mValue.mSign)
					discard ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('-') ;
			}
		}

		void write_float () {
			const auto r1x = mAttribute.float_precision () ;
			if ifswitch (TRUE) {
				if (mValue.mPrecision == 0)
					discard ;
				while (TRUE) {
					if (mValue.mMantissa == 0)
						break ;
					if (mValue.mMantissa % 10 != 0)
						break ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				const auto r2x = mValue.mPrecision - r1x ;
				for (auto &&i : iter (0 ,r2x - 1)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				if (r2x <= 0)
					discard ;
				mValue.mMantissa += VAL64 (mValue.mMantissa >= 5) * 5 ;
				mValue.mMantissa /= 10 ;
				mValue.mExponent++ ;
				mValue.mPrecision-- ;
			}
			const auto r3x = mValue.mPrecision ;
			const auto r4x = LENGTH (mValue.mExponent) ;
			auto act = TRUE ;
			if ifswitch (act) {
				//@info: case '0'
				if (mValue.mMantissa != 0)
					discard ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('0') ;
			}
			if ifswitch (act) {
				//@info: case 'x.xxxExxx'
				const auto r5x = r3x - 1 + r4x ;
				if (MathProc::abs (r5x) < r1x)
					discard ;
				const auto r6x = mValue ;
				mValue.mSign = BOOL (r5x < 0) ;
				mValue.mMantissa = MathProc::abs (r5x) ;
				mValue.mPrecision = 1 + MathProc::vlog (mValue.mMantissa ,VAL64 (10)) ;
				mValue.mExponent = 0 ;
				write_value () ;
				mValue = r6x ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('E') ;
				const auto r7x = vmax (LENGTH (r3x - 1 - r1x) ,0) ;
				for (auto &&i : iter (0 ,r7x)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				INDEX ix = mWrite - 1 ;
				for (auto &&i : iter (r7x ,r3x - 1)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mWrite += LENGTH (mBuffer[ix] == ITEM ('0')) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('.') ;
				mWrite += LENGTH (mBuffer[ix] == ITEM ('.')) ;
				mWrite-- ;
				mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
				mValue.mMantissa /= 10 ;
				mValue.mExponent++ ;
				mValue.mPrecision-- ;
			}
			if ifswitch (act) {
				//@info: case 'xxx000'
				if (r4x < 0)
					discard ;
				for (auto &&i : iter (0 ,r4x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = ITEM ('0') ;
				}
				for (auto &&i : iter (0 ,r3x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
			}
			if ifswitch (act) {
				//@info: case 'xxx.xxx'
				if (r4x < 1 - r3x)
					discard ;
				if (r4x >= 0)
					discard ;
				const auto r8x = vmax (LENGTH (-r4x - r1x) ,0) ;
				for (auto &&i : iter (0 ,r8x)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				INDEX ix = mWrite - 1 ;
				for (auto &&i : iter (r8x ,-r4x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mWrite += LENGTH (mBuffer[ix] == ITEM ('0')) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('.') ;
				mWrite += LENGTH (mBuffer[ix] == ITEM ('.')) ;
				for (auto &&i : iter (0 ,r3x + r4x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
			}
			if ifswitch (act) {
				//@info: case '0.000xxx'
				if (r4x >= 1 - r3x)
					discard ;
				if (r4x >= 0)
					discard ;
				const auto r9x = vmax (LENGTH (-r4x - r1x) ,0) ;
				for (auto &&i : iter (0 ,r9x)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				INDEX ix = mWrite - 1 ;
				for (auto &&i : iter (r9x ,r3x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mWrite += LENGTH (mBuffer[ix] == ITEM ('0')) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				const auto r10x = vmax (r9x ,r3x) ;
				for (auto &&i : iter (r10x ,-r4x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = ITEM ('0') ;
					mWrite += LENGTH (mBuffer[ix] == ITEM ('0')) ;
				}
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('.') ;
				mWrite += LENGTH (mBuffer[ix] == ITEM ('.')) ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('0') ;
			}
		}
	} ;
} ;

exports auto TEXTWRITER_HELP<STRA ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRW ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRU8 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRU16 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRU32 ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait REGULARREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename REGULARREADER_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		VRef<TextReader<STRU8>> mReader ;
		TextAttribute<STRU8> mAttribute ;
		Deque<STRU8> mCache ;
		Deque<STRU8> mBackupCache ;
		BOOL mHintString ;
		LENGTH mHintSize ;

	public:
		void initialize (RREF<VRef<TextReader<STRU8>>> reader ,CREF<LENGTH> ll_size) override {
			mReader = move (reader) ;
			mAttribute = mReader->attribute () ;
			mReader.self >> BOM ;
			mCache = Deque<STRU8> (ll_size) ;
			mBackupCache = Deque<STRU8> (ll_size) ;
			for (auto &&i : iter (0 ,ll_size)) {
				noop (i) ;
				const auto r1x = mReader->poll (TYPEAS<STRU8>::expr) ;
				mCache.add (r1x) ;
			}
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
		}

		CREF<STRU8> at (CREF<INDEX> index) const leftvalue override {
			return mCache[index] ;
		}

		void backup () {
			mBackupCache.clear () ;
			for (auto &&i : mCache)
				mBackupCache.add (i) ;
			mReader->backup () ;
		}

		void recover () {
			mReader->recover () ;
			swap (mBackupCache ,mCache) ;
		}

		void read () override {
			mCache.take () ;
			const auto r1x = mReader->poll (TYPEAS<STRU8>::expr) ;
			mCache.add (r1x) ;
		}

		void read (CREF<Slice<STR>> item) override {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (mCache[0] == STRU8 (item[i])) ;
				read () ;
			}
		}

		void read_hint_word () override {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			while (TRUE) {
				const auto r1x = invoke ([&] () {
					if (mAttribute.is_word (mCache[0]))
						return FALSE ;
					if (mAttribute.is_number (mCache[0]))
						return FALSE ;
					if (mAttribute.is_hyphen (mCache[0]))
						return FALSE ;
					return TRUE ;
				}) ;
				if (r1x)
					break ;
				read () ;
				mHintSize++ ;
			}
			recover () ;
		}

		void read_hint_scalar () override {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			if ifswitch (TRUE) {
				if ifnot (mCache[0] == STRU8 ('+'))
					if ifnot (mCache[0] == STRU8 ('-'))
						discard ;
				read () ;
				mHintSize++ ;
			}
			const auto r1x = mCache[0] ;
			assume (mAttribute.is_number (r1x)) ;
			read () ;
			mHintSize++ ;
			if ifswitch (TRUE) {
				if (r1x == STRU8 ('0'))
					discard ;
				while (TRUE) {
					if ifnot (mAttribute.is_number (mCache[0]))
						break ;
					read () ;
					mHintSize++ ;
				}
			}
			if ifswitch (TRUE) {
				if (mCache[0] != STRU8 ('.'))
					discard ;
				read () ;
				mHintSize++ ;
				while (TRUE) {
					if ifnot (mAttribute.is_number (mCache[0]))
						break ;
					read () ;
					mHintSize++ ;
				}
			}
			if ifswitch (TRUE) {
				if ifnot (mCache[0] == STRU8 ('e'))
					if ifnot (mCache[0] == STRU8 ('E'))
						discard ;
				read () ;
				mHintSize++ ;
				if ifswitch (TRUE) {
					if ifnot (mCache[0] == STRU8 ('+'))
						if ifnot (mCache[0] == STRU8 ('-'))
							discard ;
					read () ;
					mHintSize++ ;
				}
				assume (mAttribute.is_number (mCache[0])) ;
				read () ;
				mHintSize++ ;
				while (TRUE) {
					if ifnot (mAttribute.is_number (mCache[0]))
						break ;
					read () ;
					mHintSize++ ;
				}
			}
			recover () ;
		}

		void read_hint_string () override {
			mHintString = TRUE ;
			mHintSize = 0 ;
			backup () ;
			assume (mCache[0] == STRU8 ('\"')) ;
			read () ;
			while (TRUE) {
				if (mCache[0] == mAttribute.ending_item ())
					break ;
				if (mCache[0] == STRU8 ('\"'))
					break ;
				const auto r1x = mCache[0] ;
				read () ;
				auto act = TRUE ;
				if ifswitch (act) {
					if ifnot (r1x == STRU8 ('\\'))
						discard ;
					read () ;
					mHintSize++ ;
				}
				if ifswitch (act) {
					assume (ifnot (mAttribute.is_control (r1x))) ;
					mHintSize++ ;
				}
			}
			assume (mCache[0] == STRU8 ('\"')) ;
			read () ;
			recover () ;
		}

		void read_hint_endline () override {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			while (TRUE) {
				if (mCache[0] == mAttribute.ending_item ())
					break ;
				if (mAttribute.is_gap_endline (mCache[0]))
					break ;
				assume (ifnot (mAttribute.is_control (mCache[0]))) ;
				read () ;
				mHintSize++ ;
			}
			recover () ;
		}

		void read_skip_gap () override {
			while (TRUE) {
				if ifnot (mAttribute.is_gap (mCache[0]))
					break ;
				read () ;
			}
		}

		void read_skip_gap_space () override {
			while (TRUE) {
				if (mCache[0] == mAttribute.ending_item ())
					break ;
				if ifnot (mAttribute.is_gap_space (mCache[0]))
					break ;
				read () ;
			}
		}

		void read_skip_gap_endline () override {
			while (TRUE) {
				if (mCache[0] == mAttribute.ending_item ())
					break ;
				if ifnot (mAttribute.is_gap_endline (mCache[0]))
					break ;
				read () ;
			}
		}

		void read (VREF<String<STRU8>> item) override {
			const auto r1x = mHintString ;
			const auto r2x = mHintSize ;
			mHintString = FALSE ;
			mHintSize = 0 ;
			if ifswitch (TRUE) {
				if (item.size () >= r2x)
					discard ;
				assume (r2x <= VAL32_MAX) ;
				item = String<STRU8> (r2x) ;
			}
			item.trunc (r2x) ;
			if ifswitch (TRUE) {
				if ifnot (r1x)
					discard ;
				assume (mCache[0] == STRU8 ('\"')) ;
				read () ;
			}
			for (auto &&i : iter (0 ,r2x)) {
				const auto r3x = mCache[0] ;
				read () ;
				auto act = TRUE ;
				if ifswitch (act) {
					const auto r4x = BOOL (r3x == STRU8 ('\\')) ;
					if ifnot (r4x)
						discard ;
					item[i] = mAttribute.escape_ctrl_cast (mCache[0]).poll () ;
					read () ;
				}
				if ifswitch (act) {
					assume (ifnot (mAttribute.is_control (r3x))) ;
					item[i] = r3x ;
				}
			}
			if ifswitch (TRUE) {
				if ifnot (r1x)
					discard ;
				assume (mCache[0] == STRU8 ('\"')) ;
				read () ;
			}
		}
	} ;
} ;

template <>
exports auto REGULARREADER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename REGULARREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;