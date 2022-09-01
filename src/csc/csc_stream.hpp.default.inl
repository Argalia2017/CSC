#pragma once

#ifndef __CSC_STREAM__
#error "∑(っ°Д° ;)っ : require 'csc_stream.hpp'"
#endif

namespace CSC {
template <class DEPEND>
trait BYTEATTRIBUTE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			noop () ;
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

exports auto BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BYTEATTRIBUTE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Holder ;
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
		implicit ImplHolder () = default ;

		void initialize (RREF<CRef<RegBuffer<BYTE>>> stream) override {
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

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (read_ <= write_) ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mRead >= mStream->size ())
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (rxx) {
				item = mAttribute.ending_item () ;
			}
		}

		void read (VREF<WORD> item) override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				auto rax = BoxBuffer<BYTE ,SIZE_OF<WORD>> () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr))
					read (rax[i]) ;
				item = bitwise[TYPEAS<WORD>::expr] (rax) ;
			}
			if ifswitch (rxx) {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<WORD>> () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
					INDEX ix = SIZE_OF<WORD>::expr - 1 - i ;
					read (rax[ix]) ;
				}
				item = bitwise[TYPEAS<WORD>::expr] (rax) ;
			}
		}

		void read (VREF<CHAR> item) override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr))
					read (rax[i]) ;
				item = bitwise[TYPEAS<CHAR>::expr] (rax) ;
			}
			if ifswitch (rxx) {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
					INDEX ix = SIZE_OF<CHAR>::expr - 1 - i ;
					read (rax[ix]) ;
				}
				item = bitwise[TYPEAS<CHAR>::expr] (rax) ;
			}
		}

		void read (VREF<DATA> item) override {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				auto rax = BoxBuffer<BYTE ,SIZE_OF<DATA>> () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr))
					read (rax[i]) ;
				item = bitwise[TYPEAS<DATA>::expr] (rax) ;
			}
			if ifswitch (rxx) {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<DATA>> () ;
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
				vbetween (INDEX (item[i]) ,0 ,128) ;
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
				item = String<STRA> (r1x) ;
			}
			item.clear () ;
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
				item = String<STRW> (r1x) ;
			}
			item.clear () ;
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
				item = String<STRU8> (r1x) ;
			}
			item.clear () ;
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
				item = String<STRU16> (r1x) ;
			}
			item.clear () ;
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
				item = String<STRU32> (r1x) ;
			}
			item.clear () ;
			auto rax = R1X () ;
			for (auto &&i : iter (0 ,r1x)) {
				read (rax) ;
				item[i] = bitwise[TYPEAS<STRU32>::expr] (rax) ;
			}
			read (rax) ;
			assume (rax == R1X (0X00)) ;
		}

		void read (VREF<Binder> item) override {
			auto rax = ByteReader (VRef<Holder>::reference (thiz)) ;
			item.friend_read (rax) ;
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

exports auto BYTEREADER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Binder = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Binder ;
	using Holder = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		ByteAttribute mAttribute ;
		VRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<VRef<RegBuffer<BYTE>>> stream) override {
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

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (read_ <= write_) ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr))
					write (r1x[i]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
					INDEX ix = SIZE_OF<WORD>::expr - 1 - i ;
					write (r1x[ix]) ;
				}
			}
		}

		void write (CREF<CHAR> item) override {
			const auto r1x = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<CHAR>>>::expr] (item) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr))
					write (r1x[i]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
					INDEX ix = SIZE_OF<CHAR>::expr - 1 - i ;
					write (r1x[ix]) ;
				}
			}
		}

		void write (CREF<DATA> item) override {
			const auto r1x = bitwise[TYPEAS<BoxBuffer<BYTE ,SIZE_OF<DATA>>>::expr] (item) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttribute.is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr))
					write (r1x[i]) ;
			}
			if ifswitch (rxx) {
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
				vbetween (INDEX (item[i]) ,0 ,128) ;
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

		void write (CREF<Binder> item) override {
			auto rax = ByteWriter (VRef<Holder>::reference (thiz)) ;
			item.friend_write (rax) ;
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

exports auto BYTEWRITER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class ITEM>
trait TEXTATTRIBUTE_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using Holder = typename TEXTATTRIBUTE_HELP<ITEM ,ALWAYS>::Holder ;

	using SPACE_CLAZZ_SPACE = RANK1 ;
	using SPACE_CLAZZ_ENDLINE = RANK2 ;

	class ImplHolder implement Holder {
	protected:
		HashSet<ITEM> mSpaceSet ;
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mSpaceSet.add (ITEM (' ') ,SPACE_CLAZZ_SPACE::expr) ;
			mSpaceSet.add (ITEM ('\t') ,SPACE_CLAZZ_SPACE::expr) ;
			mSpaceSet.add (ITEM ('\b') ,SPACE_CLAZZ_SPACE::expr) ;
			mSpaceSet.add (ITEM ('\r') ,SPACE_CLAZZ_ENDLINE::expr) ;
			mSpaceSet.add (ITEM ('\n') ,SPACE_CLAZZ_ENDLINE::expr) ;
			mSpaceSet.add (ITEM ('\f') ,SPACE_CLAZZ_ENDLINE::expr) ;
			mEscapeSet.add (ITEM ('\\') ,INDEX ('\\')) ;
			mEscapeSet.add (ITEM ('/') ,INDEX ('/')) ;
			mEscapeSet.add (ITEM ('t') ,INDEX ('\t')) ;
			mEscapeSet.add (ITEM ('b') ,INDEX ('\b')) ;
			mEscapeSet.add (ITEM ('r') ,INDEX ('\r')) ;
			mEscapeSet.add (ITEM ('n') ,INDEX ('\n')) ;
			mEscapeSet.add (ITEM ('f') ,INDEX ('\f')) ;
			mEscapeSet.add (ITEM ('\'') ,INDEX ('\'')) ;
			mEscapeSet.add (ITEM ('\"') ,INDEX ('\"')) ;
			mEscapeSet.add (ITEM ('u') ,INDEX (7)) ;
		}

		ITEM ending_item () const override {
			return ITEM (0) ;
		}

		BOOL is_space (CREF<ITEM> str) const override {
			return mSpaceSet.find (str) != NONE ;
		}

		BOOL is_endline_space (CREF<ITEM> str) const override {
			return mSpaceSet.map (str) != SPACE_CLAZZ_ENDLINE::expr ;
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

		ITEM lower_cast (CREF<ITEM> str) const {
			if (str >= ITEM ('A'))
				if (str <= ITEM ('Z'))
					return ITEM (INDEX (str) - INDEX ('A') + INDEX ('a')) ;
			return str ;
		}

		ITEM upper_cast (CREF<ITEM> str) const {
			if (str >= ITEM ('a'))
				if (str <= ITEM ('z'))
					return ITEM (INDEX (str) - INDEX ('a') + INDEX ('A')) ;
			return str ;
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
			return FALSE ;
		}

		INDEX hex_from_str (CREF<ITEM> str) const override {
			if (is_number (str))
				return INDEX (str) - INDEX ('0') ;
			const auto r1x = lower_cast (str) ;
			if (is_hex_number (r1x))
				return INDEX (r1x) - INDEX ('a') + 10 ;
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

		Optional<ITEM> escape_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeSet.map (str) ;
			if (ix == NONE)
				return FLAG (1) ;
			return Optional<ITEM>::make (ITEM (ix)) ;
		}

		LENGTH value_precision () const override {
			return 19 ;
		}

		LENGTH float_precision () const override {
			return 15 ;
		}

		LENGTH number_precision () const override {
			return 1024 ;
		}
	} ;
} ;

exports auto TEXTATTRIBUTE_HELP<STRA ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRW ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRU8 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRU16 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTATTRIBUTE_HELP<STRU32 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTATTRIBUTE_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class ITEM>
trait TEXTREADER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using Binder = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Binder ;
	using Holder = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Holder ;
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
		implicit ImplHolder () = default ;

		void initialize (RREF<CRef<RegBuffer<ITEM>>> stream) override {
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

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (read_ <= write_) ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mRead >= mStream->size ())
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (rxx) {
				item = mAttribute.ending_item () ;
			}
		}

		void read (VREF<BOOL> item) override {
			auto rax = ITEM () ;
			read (rax) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
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
			if ifswitch (rxx) {
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
			if ifswitch (rxx) {
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
			if ifswitch (rxx) {
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
			if ifswitch (rxx) {
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
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
			fexp10.mMantissa = mAttribute.hex_from_str (top) ;
			fexp10.mPrecision++ ;
			fexp10.mExponent = 0 ;
			if ifswitch (TRUE) {
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					read (top) ;
					if ifnot (mAttribute.is_number (top))
						break ;
					if (fexp10.mPrecision > r1x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += mAttribute.hex_from_str (top) ;
					fexp10.mPrecision++ ;
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
			}
			assume (fexp10.mPrecision <= r1x) ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (rax != ITEM ('i'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('n')) ;
				read (rax) ;
				assume (rax == ITEM ('f')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (rxx) {
				if (rax != ITEM ('I'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('N')) ;
				read (rax) ;
				assume (rax == ITEM ('F')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (rxx) {
				assume (mAttribute.is_number (rax)) ;
				auto rbx = NOTATION () ;
				rbx.mRadix = 10 ;
				rbx.mSign = FALSE ;
				rbx.mPrecision = 0 ;
				read_float (rbx ,rax) ;
				rbx = FloatProc::exp2_from_exp10 (rbx) ;
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
			const auto r1x = mAttribute.number_precision () ;
			read_value (fexp10 ,top) ;
			if ifswitch (TRUE) {
				if (top != ITEM ('.'))
					discard ;
				read (top) ;
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					read (top) ;
					if ifnot (mAttribute.is_number (top))
						break ;
					if (fexp10.mPrecision > r1x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += mAttribute.hex_from_str (top) ;
					fexp10.mExponent-- ;
					fexp10.mPrecision++ ;
				}
				while (TRUE) {
					if (top == mAttribute.ending_item ())
						break ;
					if ifnot (mAttribute.is_number (top))
						break ;
					read (top) ;
				}
			}
			if ifswitch (TRUE) {
				if (top != ITEM ('e'))
					if (top != ITEM ('E'))
						discard ;
				read (top) ;
				assume (mAttribute.is_number (top)) ;
				auto rbx = NOTATION () ;
				rbx.mRadix = 10 ;
				rbx.mSign = FALSE ;
				rbx.mPrecision = 0 ;
				read_value (rbx ,top) ;
				assume (MathProc::abs (rbx.mMantissa) < r1x) ;
				fexp10.mExponent += rbx.mMantissa ;
				assume (MathProc::abs (fexp10.mExponent) < r1x) ;
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
				vbetween (INDEX (item[i]) ,0 ,128) ;
				read (rax) ;
				assume (rax == ITEM (item[i])) ;
			}
		}

		void read (VREF<String<ITEM>> item) override {
			const auto r1x = next_space_size () ;
			if ifswitch (TRUE) {
				if (item.size () >= r1x)
					discard ;
				item = String<ITEM> (r1x) ;
			}
			item.clear () ;
			for (auto &&i : iter (0 ,r1x))
				read (item[i]) ;
		}

		LENGTH next_space_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
			read (rax) ;
			while (TRUE) {
				if (rax == mAttribute.ending_item ())
					break ;
				if (mAttribute.is_space (rax))
					break ;
				ret++ ;
				read (rax) ;
			}
			mRead = r1x ;
			return move (ret) ;
		}

		void read (VREF<Binder> item) override {
			auto rax = TextReader (VRef<Holder>::reference (thiz)) ;
			item.friend_read (rax) ;
		}

		void read_cls () override {
			reset () ;
		}

		void read_bom () override {
			template_bom (PHX ,TYPEAS<ITEM>::expr) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRA>>>
		void template_bom (CREF<typeof (PH5)> ,CREF<TYPEID<ARG1>> id) {
			noop () ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRW>>>
		void template_bom (CREF<typeof (PH4)> ,CREF<TYPEID<ARG1>> id) {
			template_bom (PHX ,TYPEAS<STRUW>::expr) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU8>>>
		void template_bom (CREF<typeof (PH3)> ,CREF<TYPEID<ARG1>> id) {
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

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU16>>>
		void template_bom (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id) {
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

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU32>>>
		void template_bom (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id) {
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
			const auto r1x = next_not_space_size () ;
			mRead += r1x ;
		}

		LENGTH next_not_space_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
			read (rax) ;
			while (TRUE) {
				if (rax == mAttribute.ending_item ())
					break ;
				if ifnot (mAttribute.is_space (rax))
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

exports auto TEXTREADER_HELP<STRA ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRW ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRU8 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRU16 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTREADER_HELP<STRU32 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTREADER_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class ITEM>
trait TEXTWRITER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;
	using Holder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Holder ;
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
		implicit ImplHolder () = default ;

		void initialize (RREF<VRef<RegBuffer<ITEM>>> stream) override {
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

		void reset () override {
			mRead = 0 ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (read_ <= write_) ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (item)
					discard ;
				write (ITEM ('t')) ;
				write (ITEM ('r')) ;
				write (ITEM ('u')) ;
				write (ITEM ('e')) ;
			}
			if ifswitch (rxx) {
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
			using R1X = typename TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS>::WriteValue ;
			if ifswitch (TRUE) {
				if (item >= 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (item != VAL64_ABS)
					discard ;
				write (ITEM ('a')) ;
				write (ITEM ('b')) ;
				write (ITEM ('s')) ;
			}
			if ifswitch (rxx) {
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
			using R1X = typename TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS>::WriteValue ;
			if ifswitch (TRUE) {
				if (item > 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (MathProc::is_infinite (item))
					discard ;
				write (ITEM ('i')) ;
				write (ITEM ('n')) ;
				write (ITEM ('f')) ;
			}
			if ifswitch (rxx) {
				const auto r1x = FloatProc::decode (MathProc::abs (item)) ;
				const auto r2x = FloatProc::exp10_from_exp2 (r1x) ;
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
				vbetween (INDEX (item[i]) ,0 ,128) ;
				const auto r1x = ITEM (item[i]) ;
				write (r1x) ;
			}
		}

		void write (CREF<String<ITEM>> item) override {
			for (auto &&i : item)
				write (i) ;
		}

		void write (CREF<Binder> item) override {
			auto rax = TextWriter (VRef<Holder>::reference (thiz)) ;
			item.friend_write (rax) ;
		}

		void write_cls () override {
			reset () ;
		}

		void write_bom () override {
			template_bom (PHX ,TYPEAS<ITEM>::expr) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRA>>>
		void template_bom (CREF<typeof (PH5)> ,CREF<TYPEID<ARG1>> id) {
			noop () ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRW>>>
		void template_bom (CREF<typeof (PH4)> ,CREF<TYPEID<ARG1>> id) {
			template_bom (PHX ,TYPEAS<STRUW>::expr) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU8>>>
		void template_bom (CREF<typeof (PH3)> ,CREF<TYPEID<ARG1>> id) {
			write (ITEM (0XEF)) ;
			write (ITEM (0XBB)) ;
			write (ITEM (0XBF)) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU16>>>
		void template_bom (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id) {
			write (ITEM (0XFEFF)) ;
			write (ITEM (0XFFFE)) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU32>>>
		void template_bom (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id) {
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
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;
	using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using NUMBER_SIZE = ENUMAS<VAL ,ENUMID<64>> ;

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
			mWrite = mBuffer.size () ;
		}

		void friend_write (VREF<TextWriter> writer) const override {
			for (auto &&i : iter (mWrite ,mBuffer.size ()))
				writer.write (mBuffer[i]) ;
		}

		void write_value () {
			const auto r1x = mValue.mPrecision ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				//@info: case '0'
				if (mValue.mMantissa != 0)
					discard ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('0') ;
			}
			if ifswitch (rxx) {
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
				mValue.mMantissa += MathProc::step (mValue.mMantissa - 5) * 5 ;
				mValue.mMantissa /= 10 ;
				mValue.mExponent++ ;
				mValue.mPrecision-- ;
			}
			const auto r3x = mValue.mPrecision ;
			const auto r4x = LENGTH (mValue.mExponent) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				//@info: case '0'
				if (mValue.mMantissa != 0)
					discard ;
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('0') ;
			}
			if ifswitch (rxx) {
				//@info: case 'x.xxxExxx'
				const auto r5x = r3x - 1 + r4x ;
				if (MathProc::abs (r5x) < r1x)
					discard ;
				write_value () ;
				if ifswitch (TRUE) {
					if (r5x <= 0)
						discard ;
					mWrite-- ;
					mBuffer[mWrite] = ITEM ('+') ;
				}
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('e') ;
				const auto r6x = vmax (LENGTH (r3x - 1 - r1x) ,ZERO) ;
				for (auto &&i : iter (0 ,r6x)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				INDEX ix = mWrite - 1 ;
				for (auto &&i : iter (r6x ,r3x - 1)) {
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
			if ifswitch (rxx) {
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
			if ifswitch (rxx) {
				//@info: case 'xxx.xxx'
				if (r4x < 1 - r3x)
					discard ;
				if (r4x >= 0)
					discard ;
				const auto r7x = vmax (LENGTH (-r4x - r1x) ,ZERO) ;
				for (auto &&i : iter (0 ,r7x)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				INDEX ix = mWrite - 1 ;
				for (auto &&i : iter (r7x ,-r4x)) {
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
			if ifswitch (rxx) {
				//@info: case '0.000xxx'
				if (r4x >= 1 - r3x)
					discard ;
				if (r4x >= 0)
					discard ;
				const auto r8x = vmax (LENGTH (-r4x - r1x) ,ZERO) ;
				for (auto &&i : iter (0 ,r8x)) {
					noop (i) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				INDEX ix = mWrite - 1 ;
				for (auto &&i : iter (r8x ,r3x)) {
					noop (i) ;
					mWrite-- ;
					mBuffer[mWrite] = mAttribute.str_from_hex (mValue.mMantissa % 10) ;
					mWrite += LENGTH (mBuffer[ix] == ITEM ('0')) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				const auto r9x = vmax (r8x ,r3x) ;
				for (auto &&i : iter (r9x ,-r4x)) {
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

exports auto TEXTWRITER_HELP<STRA ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRA ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRW ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRW ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRU8 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRU8 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRU16 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRU16 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

exports auto TEXTWRITER_HELP<STRU32 ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<STRU32 ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait REGULARREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename REGULARREADER_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		VRef<TextReader<STRU8>> mReader ;
		Deque<STRU8> mCache ;
		Deque<STRU8> mBackupCache ;
		BOOL mHintString ;
		LENGTH mHintSize ;

	public:
		implicit ImplHolder () = default ;

		void initialize (RREF<VRef<TextReader<STRU8>>> reader ,CREF<LENGTH> ll_size) override {
			mReader = move (reader) ;
			mReader.self >> TextReader<STRU8>::BOM ;
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

		void backup () override {
			mBackupCache.clear () ;
			for (auto &&i : mCache)
				mBackupCache.add (i) ;
			mReader->backup () ;
		}

		void recover () override {
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

		void read_hint_identifer () override {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->attribute () ;
			while (TRUE) {
				if ifswitch (TRUE) {
					if (mHintSize != 0)
						discard ;
					assume (r1x.is_word (mCache[0])) ;
				}
				const auto r2x = invoke ([&] () {
					if (r1x.is_word (mCache[0]))
						return FALSE ;
					if (r1x.is_number (mCache[0]))
						return FALSE ;
					if (mCache[0] == STRU8 ('-'))
						return FALSE ;
					return TRUE ;
				}) ;
				if (r2x)
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
			const auto r1x = mReader->attribute () ;
			if ifswitch (TRUE) {
				if ifnot (mCache[0] == STRU8 ('+'))
					if ifnot (mCache[0] == STRU8 ('-'))
						discard ;
				read () ;
				mHintSize++ ;
			}
			const auto r2x = mCache[0] ;
			assume (r1x.is_number (r2x)) ;
			read () ;
			mHintSize++ ;
			if ifswitch (TRUE) {
				if (r2x == STRU8 ('0'))
					discard ;
				while (TRUE) {
					if ifnot (r1x.is_number (mCache[0]))
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
					if ifnot (r1x.is_number (mCache[0]))
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
				assume (r1x.is_number (mCache[0])) ;
				read () ;
				mHintSize++ ;
				while (TRUE) {
					if ifnot (r1x.is_number (mCache[0]))
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
			const auto r1x = mReader->attribute () ;
			assume (mCache[0] == STRU8 ('\"')) ;
			read () ;
			while (TRUE) {
				if (mCache[0] == r1x.ending_item ())
					break ;
				if (mCache[0] == STRU8 ('\"'))
					break ;
				const auto r2x = mCache[0] ;
				read () ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (r2x == STRU8 ('\\'))
						discard ;
					read () ;
					mHintSize++ ;
				}
				if ifswitch (rxx) {
					assume (ifnot (r1x.is_control (r2x))) ;
					mHintSize++ ;
				}
			}
			assume (mCache[0] == STRU8 ('\"')) ;
			read () ;
			recover () ;
		}

		void read_hint_word_space () override {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->attribute () ;
			while (TRUE) {
				if (mCache[0] == r1x.ending_item ())
					break ;
				if (r1x.is_space (mCache[0]))
					break ;
				assume (ifnot (r1x.is_control (mCache[0]))) ;
				read () ;
				mHintSize++ ;
			}
			recover () ;
		}

		void read_hint_word_endline () override {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->attribute () ;
			while (TRUE) {
				if (mCache[0] == r1x.ending_item ())
					break ;
				if (r1x.is_endline_space (mCache[0]))
					break ;
				assume (ifnot (r1x.is_control (mCache[0]))) ;
				read () ;
				mHintSize++ ;
			}
			recover () ;
		}

		void read_skip_space () override {
			const auto r1x = mReader->attribute () ;
			while (TRUE) {
				if ifnot (r1x.is_space (mCache[0]))
					break ;
				read () ;
			}
		}

		void read_skip_space_only () override {
			const auto r1x = mReader->attribute () ;
			while (TRUE) {
				if (mCache[0] == r1x.ending_item ())
					break ;
				if ifnot (r1x.is_space (mCache[0]))
					break ;
				if (r1x.is_endline_space (mCache[0]))
					break ;
				read () ;
			}
		}

		void read_skip_space_endline () override {
			const auto r1x = mReader->attribute () ;
			while (TRUE) {
				if (mCache[0] == r1x.ending_item ())
					break ;
				if ifnot (r1x.is_endline_space (mCache[0]))
					break ;
				read () ;
			}
		}

		void read (VREF<String<STRU8>> item) override {
			const auto r1x = mReader->attribute () ;
			const auto r2x = mHintString ;
			const auto r3x = mHintSize ;
			mHintString = FALSE ;
			mHintSize = 0 ;
			if ifswitch (TRUE) {
				if (item.size () >= r3x)
					discard ;
				item = String<STRU8> (r3x) ;
			}
			item.clear () ;
			if ifswitch (TRUE) {
				if ifnot (r2x)
					discard ;
				assume (mCache[0] == STRU8 ('\"')) ;
				read () ;
			}
			for (auto &&i : iter (0 ,r3x)) {
				const auto r4x = mCache[0] ;
				read () ;
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					const auto r5x = BOOL (r4x == STRU8 ('\\')) ;
					if ifnot (r5x)
						discard ;
					item[i] = r1x.escape_cast (mCache[0]).poll () ;
					read () ;
				}
				if ifswitch (rxx) {
					assume (ifnot (r1x.is_control (r4x))) ;
					item[i] = r4x ;
				}
			}
			if ifswitch (TRUE) {
				if ifnot (r2x)
					discard ;
				assume (mCache[0] == STRU8 ('\"')) ;
				read () ;
			}
		}
	} ;
} ;

exports auto REGULARREADER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename REGULARREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;