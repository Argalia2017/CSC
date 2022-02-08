#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
namespace STREAM {
template <class...>
trait BYTEREADER_HELP ;

template <class...>
trait BYTEREADER_BINDER_HELP ;

template <class...>
trait BYTEREADER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BYTEREADER_BINDER_HELP<DEPEND ,ALWAYS> {
	using ByteReader = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::ByteReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<ByteReader> reader) = 0 ;
	} ;
} ;

template <class DEPEND>
trait BYTEREADER_HELP<DEPEND ,ALWAYS> {
	struct Attribute implement Interface {
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<BYTEREADER_BINDER_HELP<DEPEND ,ALWAYS> ,ARG1>::Binder ;

	class ByteReader {
	private:
		CRef<Attribute> mAttr ;
		CRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ByteReader () = default ;

		explicit ByteReader (RREF<CRef<RegBuffer<BYTE>>> stream) {
			set_attr () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		void set_attr () {
			using R1X = typename BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplAttribute ;
			auto &&tmp = memorize ([&] () {
				return CRef<R1X>::make () ;
			}) ;
			set_attr (copy (tmp)) ;
		}

		void set_attr (RREF<CRef<Attribute>> attr_) {
			mAttr = move (attr_) ;
		}

		LENGTH length () const {
			return mRead ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = mStream->size () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (read_ <= write_) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		template <class ARG1>
		REMOVE_ALL<ARG1> poll (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			R1X ret ;
			read (ret) ;
			return move (ret) ;
		}

		void scans () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void scans (XREF<ARG1> item1 ,XREF<ARG2>...items) {
			read (item1) ;
			scans (items...) ;
		}

		void read (VREF<BYTE> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mRead >= mWrite)
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (eax) {
				item = mAttr->ending_item () ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value))
					read (rax.self[i]) ;
				item = bitwise[TYPEAS<WORD>::id] (rax.self) ;
			}
			if ifswitch (eax) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value)) {
					INDEX ix = SIZE_OF<WORD>::value - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise[TYPEAS<WORD>::id] (rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value))
					read (rax.self[i]) ;
				item = bitwise[TYPEAS<CHAR>::id] (rax.self) ;
			}
			if ifswitch (eax) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value)) {
					INDEX ix = SIZE_OF<CHAR>::value - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise[TYPEAS<CHAR>::id] (rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value))
					read (rax.self[i]) ;
				item = bitwise[TYPEAS<DATA>::id] (rax.self) ;
			}
			if ifswitch (eax) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value)) {
					INDEX ix = SIZE_OF<DATA>::value - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise[TYPEAS<DATA>::id] (rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<DATA> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BOOL> item) {
			using R1X = BYTE_BASE<SIZE_OF<BOOL> ,ALIGN_OF<BOOL>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<BOOL>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			using R1X = BYTE_BASE<SIZE_OF<VAL32> ,ALIGN_OF<VAL32>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<VAL32>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			using R1X = BYTE_BASE<SIZE_OF<VAL64> ,ALIGN_OF<VAL64>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<VAL64>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			using R1X = BYTE_BASE<SIZE_OF<SINGLE> ,ALIGN_OF<SINGLE>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<SINGLE>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			using R1X = BYTE_BASE<SIZE_OF<DOUBLE> ,ALIGN_OF<DOUBLE>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<DOUBLE>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRA> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRA> ,ALIGN_OF<STRA>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<STRA>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRA> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRW> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRW> ,ALIGN_OF<STRW>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<STRW>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRW> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRU8> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRU8> ,ALIGN_OF<STRU8>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<STRU8>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRU8> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRU16> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRU16> ,ALIGN_OF<STRU16>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<STRU16>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRU16> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRU32> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRU32> ,ALIGN_OF<STRU32>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<STRU32>::id] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRU32> item) {
			read (item) ;
			return thiz ;
		}

		template <class ARG1 = void>
		void read (VREF<MACRO_Binder<ARG1>> item) {
			item.friend_read (thiz) ;
		}

		template <class ARG1 = void>
		inline VREF<ByteReader> operator>> (VREF<MACRO_Binder<ARG1>> item) {
			read (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {
			noop () ;
		}

		void read (CREF<typeof (CLS)> item) {
			reset () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {
			noop () ;
		}

		void read (CREF<typeof (GAP)> item) {
			const auto r1x = mAttr->space_item () ;
			auto rax = BYTE () ;
			read (rax) ;
			assume (rax == r1x) ;
			read (rax) ;
			assume (rax == r1x) ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {
			noop () ;
		}

		void read (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			auto rax = BYTE () ;
			for (auto &&i : iter (mRead ,mWrite)) {
				noop (i) ;
				read (rax) ;
				assume (rax == r1x) ;
			}
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (EOS)> item) {
			read (item) ;
			return thiz ;
		}
	} ;
} ;

template <class DEPEND>
trait BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Attribute = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Attribute ;

	class ImplAttribute implement Attribute {
	public:
		BYTE ending_item () const override {
			return BYTE (0X00) ;
		}

		BYTE space_item () const override {
			return BYTE (0XCC) ;
		}

		BOOL is_big_endian () const override {
			using R1X = typename BOX_HELP<ARR<BYTE ,SIZE_OF<WORD>> ,ALWAYS>::HEAP ;
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (bitwise[TYPEAS<R1X>::id] (r1x)) ;
				if (r2x.self[0] != BYTE (0X00))
					return FALSE ;
				return TRUE ;
			}) ;
		}
	} ;
} ;

using ByteReader = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::ByteReader ;

template <class...>
trait BYTEWRITER_HELP ;

template <class...>
trait BYTEWRITER_BINDER_HELP ;

template <class...>
trait BYTEWRITER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BYTEWRITER_BINDER_HELP<DEPEND ,ALWAYS> {
	using ByteWriter = typename DEPENDENT<BYTEWRITER_HELP<DEPEND ,ALWAYS> ,DEPEND>::ByteWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<ByteWriter> writer) const = 0 ;
	} ;
} ;

template <class DEPEND>
trait BYTEWRITER_HELP<DEPEND ,ALWAYS> {
	struct Attribute implement Interface {
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<BYTEWRITER_BINDER_HELP<DEPEND ,ALWAYS> ,ARG1>::Binder ;

	class ByteWriter {
	private:
		CRef<Attribute> mAttr ;
		VRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ByteWriter () = default ;

		explicit ByteWriter (RREF<VRef<RegBuffer<BYTE>>> stream) {
			set_attr () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		void set_attr () {
			using R1X = typename BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplAttribute ;
			auto &&tmp = memorize ([&] () {
				return CRef<R1X>::make () ;
			}) ;
			set_attr (copy (tmp)) ;
		}

		void set_attr (RREF<CRef<Attribute>> attr_) {
			mAttr = move (attr_) ;
		}

		LENGTH length () const {
			return mWrite ;
		}

		void reset () {
			mRead = mStream->size () ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (write_ <= read_) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		void prints () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void prints (XREF<ARG1> item1 ,XREF<ARG2>...items) {
			write (item1) ;
			prints (items...) ;
		}

		void write (CREF<BYTE> item) {
			if ifswitch (TRUE) {
				if (mWrite >= mRead)
					discard ;
				mStream.self[mWrite] = item ;
				mWrite++ ;
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<BYTE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<WORD> item) {
			using R1X = typename BOX_HELP<ARR<BYTE ,SIZE_OF<WORD>> ,ALWAYS>::HEAP ;
			const auto r1x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (bitwise[TYPEAS<R1X>::id] (item)) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value))
					write (r1x.self[i]) ;
			}
			if ifswitch (eax) {
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value)) {
					INDEX ix = SIZE_OF<WORD>::value - 1 - i ;
					write (r1x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			using R1X = typename BOX_HELP<ARR<BYTE ,SIZE_OF<CHAR>> ,ALWAYS>::HEAP ;
			const auto r1x = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make (bitwise[TYPEAS<R1X>::id] (item)) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value))
					write (r1x.self[i]) ;
			}
			if ifswitch (eax) {
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value)) {
					INDEX ix = SIZE_OF<CHAR>::value - 1 - i ;
					write (r1x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			using R1X = typename BOX_HELP<ARR<BYTE ,SIZE_OF<DATA>> ,ALWAYS>::HEAP ;
			const auto r1x = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make (bitwise[TYPEAS<R1X>::id] (item)) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value))
					write (r1x.self[i]) ;
			}
			if ifswitch (eax) {
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value)) {
					INDEX ix = SIZE_OF<DATA>::value - 1 - i ;
					write (r1x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<DATA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BOOL> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<BOOL> item) {
			write (item) ;
			return thiz ;
		}

		csc_pointer_t write (CREF<csc_pointer_t>) = delete ;

		inline VREF<ByteWriter> operator<< (CREF<csc_pointer_t>) = delete ;

		void write (CREF<VAL32> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<VAL32> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<VAL64> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<VAL64> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<SINGLE> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<SINGLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DOUBLE> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<DOUBLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<STRA> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<STRA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<STRW> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<STRW> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<STRU8> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<STRU8> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<STRU16> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<STRU16> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<STRU32> item) {
			const auto r1x = bitwise (item) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<STRU32> item) {
			write (item) ;
			return thiz ;
		}

		template <class ARG1 = void>
		void write (CREF<MACRO_Binder<ARG1>> item) {
			item.friend_write (thiz) ;
		}

		template <class ARG1 = void>
		inline VREF<ByteWriter> operator<< (CREF<MACRO_Binder<ARG1>> item) {
			write (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {
			noop () ;
		}

		void write (CREF<typeof (CLS)>) {
			reset () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {
			noop () ;
		}

		void write (CREF<typeof (GAP)> item) {
			const auto r1x = mAttr->space_item () ;
			write (r1x) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {
			noop () ;
		}

		void write (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			for (auto &&i : iter (mWrite ,mRead))
				write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}
	} ;
} ;

template <class DEPEND>
trait BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Attribute = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Attribute ;

	class ImplAttribute implement Attribute {
	public:
		BYTE ending_item () const override {
			return BYTE (0X00) ;
		}

		BYTE space_item () const override {
			return BYTE (0XCC) ;
		}

		BOOL is_big_endian () const override {
			using R1X = typename BOX_HELP<ARR<BYTE ,SIZE_OF<WORD>> ,ALWAYS>::HEAP ;
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (bitwise[TYPEAS<R1X>::id] (r1x)) ;
				if (r2x.self[0] != BYTE (0X00))
					return FALSE ;
				return TRUE ;
			}) ;
		}
	} ;
} ;

using ByteWriter = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::ByteWriter ;

template <class...>
trait TEXTREADER_HELP ;

template <class...>
trait TEXTREADER_BINDER_HELP ;

template <class...>
trait TEXTREADER_IMPLHOLDER_HELP ;

template <class ITEM>
trait TEXTREADER_BINDER_HELP<ITEM ,ALWAYS> {
	using TextReader = typename TEXTREADER_HELP<ITEM ,ALWAYS>::TextReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<TextReader> reader) = 0 ;
	} ;
} ;

template <class ITEM>
trait TEXTREADER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	struct Attribute implement Interface {
		virtual ITEM ending_item () const = 0 ;
		virtual BOOL is_space (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_endline_space (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_word (CREF<ITEM> str) const = 0 ;
		virtual ITEM lower_cast (CREF<ITEM> str) const = 0 ;
		virtual Optional<ITEM> escape_cast (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_number (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_hex_number (CREF<ITEM> str) const = 0 ;
		virtual INDEX hex_from_str (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_control (CREF<ITEM> str) const = 0 ;
		virtual LENGTH value_precision () const = 0 ;
		virtual LENGTH float_precision () const = 0 ;
	} ;

	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<TEXTREADER_BINDER_HELP<ITEM ,ALWAYS> ,ARG1>::Binder ;

	class TextReader {
	private:
		CRef<Attribute> mAttr ;
		CRef<RegBuffer<ITEM>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit TextReader () = default ;

		explicit TextReader (RREF<CRef<RegBuffer<ITEM>>> stream) {
			set_attr () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		void set_attr () {
			using R1X = typename TEXTREADER_IMPLHOLDER_HELP<ITEM ,ALWAYS>::ImplAttribute ;
			auto &&tmp = memorize ([&] () {
				return CRef<R1X>::make () ;
			}) ;
			set_attr (copy (tmp)) ;
		}

		void set_attr (RREF<CRef<Attribute>> attr_) {
			mAttr = move (attr_) ;
		}

		LENGTH length () const {
			return mRead ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = mStream->size () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (read_ <= write_) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		template <class ARG1>
		REMOVE_ALL<ARG1> poll (XREF<ARG1> id) {
			using R1X = REMOVE_ALL<ARG1> ;
			R1X ret ;
			read (ret) ;
			return move (ret) ;
		}

		void scans () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void scans (XREF<ARG1> item1 ,XREF<ARG2>...items) {
			read (item1) ;
			scans (items...) ;
		}

		void read (VREF<ITEM> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (mRead >= mWrite)
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (eax) {
				item = mAttr->ending_item () ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<ITEM> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BOOL> item) {
			auto rax = ITEM () ;
			read (rax) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
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
			if ifswitch (eax) {
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
			if ifswitch (eax) {
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
			if ifswitch (eax) {
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
			if ifswitch (eax) {
				assume (FALSE) ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			const auto r1x = read (TYPEAS<VAL64>::id) ;
			assume (r1x >= VAL32_MIN) ;
			assume (r1x <= VAL32_MAX) ;
			item = VAL32 (r1x) ;
		}

		inline VREF<TextReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			auto rax = ITEM () ;
			read (rax) ;
			const auto r1x = BOOL (rax == ITEM ('-')) ;
			if ifswitch (TRUE) {
				if (rax != ITEM ('-'))
					if (rax != ITEM ('+'))
						discard ;
				read (rax) ;
			}
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (rax != ITEM ('a'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('b')) ;
				read (rax) ;
				assume (rax == ITEM ('s')) ;
				item = VAL64_ABS ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('A'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('B')) ;
				read (rax) ;
				assume (rax == ITEM ('S')) ;
				item = VAL64_ABS ;
			}
			if ifswitch (eax) {
				assume (mAttr->is_number (rax)) ;
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
			if ifswitch (TRUE) {
				if (mRead >= mWrite)
					if (rax == mAttr->ending_item ())
						discard ;
				mRead-- ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			const auto r1x = read (TYPEAS<DOUBLE>::id) ;
			assume (r1x >= SINGLE_MIN) ;
			assume (r1x <= SINGLE_MAX) ;
			item = SINGLE (r1x) ;
		}

		inline VREF<TextReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			auto rax = ITEM () ;
			read (rax) ;
			const auto r1x = BOOL (rax == ITEM ('-')) ;
			if ifswitch (TRUE) {
				if (rax != ITEM ('-'))
					if (rax != ITEM ('+'))
						discard ;
				read (rax) ;
			}
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (rax != ITEM ('i'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('n')) ;
				read (rax) ;
				assume (rax == ITEM ('f')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('I'))
					discard ;
				read (rax) ;
				assume (rax == ITEM ('N')) ;
				read (rax) ;
				assume (rax == ITEM ('F')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (eax) {
				assume (mAttr->is_number (rax)) ;
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
			if ifswitch (TRUE) {
				if (mRead >= mWrite)
					if (rax == mAttr->ending_item ())
						discard ;
				mRead-- ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BYTE> item) {
			auto rax = ITEM () ;
			item = BYTE (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<BYTE>::value)) {
				read (rax) ;
				const auto r1x = BYTE (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = BYTE (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			auto rax = ITEM () ;
			item = WORD (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<WORD>::value)) {
				read (rax) ;
				const auto r1x = WORD (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = WORD (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			auto rax = ITEM () ;
			item = CHAR (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value)) {
				read (rax) ;
				const auto r1x = CHAR (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = CHAR (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			auto rax = ITEM () ;
			item = DATA (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<DATA>::value)) {
				read (rax) ;
				const auto r1x = DATA (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = DATA (mAttr->hex_from_str (rax)) ;
				item = (item << 4) | r2x ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<DATA> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<ITEM>> item) {
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,item.size ())) {
				read (rax) ;
				assume (rax == item[i]) ;
			}
		}

		inline VREF<TextReader> operator>> (CREF<Slice<ITEM>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<ITEM>> item) {
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

		inline VREF<TextReader> operator>> (VREF<String<ITEM>> item) {
			read (item) ;
			return thiz ;
		}

		template <class ARG1 = void>
		void read (VREF<MACRO_Binder<ARG1>> item) {
			item.friend_read (thiz) ;
		}

		template <class ARG1 = void>
		inline VREF<TextReader> operator>> (VREF<MACRO_Binder<ARG1>> item) {
			read (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {
			noop () ;
		}

		void read (CREF<typeof (CLS)>) {
			reset () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		imports void BOM (CREF<PlaceHolder<RANK2>>) {
			noop () ;
		}

		void read (CREF<typeof (BOM)>) {
			reset () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (BOM)> item) {
			read (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {
			noop () ;
		}

		void read (CREF<typeof (GAP)> item) {
			const auto r1x = next_not_space_size () ;
			mRead += r1x ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {
			noop () ;
		}

		void read (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == r1x) ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (EOS)> item) {
			read (item) ;
			return thiz ;
		}

	private:
		LENGTH next_space_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
			const auto r2x = mWrite ;
			read (rax) ;
			while (TRUE) {
				if (rax == mAttr->ending_item ())
					break ;
				if (mAttr->is_space (rax))
					break ;
				ret++ ;
				read (rax) ;
			}
			mRead = r1x ;
			mWrite = r2x ;
			return move (ret) ;
		}

		LENGTH next_not_space_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
			const auto r2x = mWrite ;
			read (rax) ;
			while (TRUE) {
				if (rax == mAttr->ending_item ())
					break ;
				if ifnot (mAttr->is_space (rax))
					break ;
				ret++ ;
				read (rax) ;
			}
			mRead = r1x ;
			mWrite = r2x ;
			return move (ret) ;
		}

		void read_value (VREF<NOTATION> fexp10 ,VREF<ITEM> top) {
			assert (fexp10.mRadix == 10) ;
			const auto r1x = mAttr->value_precision () ;
			const auto r2x = mAttr->float_precision () ;
			fexp10.mMantissa = mAttr->hex_from_str (top) ;
			fexp10.mPrecision++ ;
			fexp10.mExponent = 0 ;
			if ifswitch (TRUE) {
				while (TRUE) {
					if (top == mAttr->ending_item ())
						break ;
					read (top) ;
					if ifnot (mAttr->is_number (top))
						break ;
					if (fexp10.mPrecision > r1x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += mAttr->hex_from_str (top) ;
					fexp10.mPrecision++ ;
				}
				while (TRUE) {
					if (top == mAttr->ending_item ())
						break ;
					if ifnot (mAttr->is_number (top))
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

		void read_float (VREF<NOTATION> fexp10 ,VREF<ITEM> top) {
			assert (fexp10.mRadix == 10) ;
			const auto r1x = mAttr->float_precision () ;
			read_value (fexp10 ,top) ;
			if ifswitch (TRUE) {
				if (top != ITEM ('.'))
					discard ;
				read (top) ;
				while (TRUE) {
					if (top == mAttr->ending_item ())
						break ;
					read (top) ;
					if ifnot (mAttr->is_number (top))
						break ;
					if (fexp10 > r1x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += mAttr->hex_from_str (top) ;
					fexp10.mExponent-- ;
					fexp10.mPrecision++ ;
				}
				while (TRUE) {
					if (top == mAttr->ending_item ())
						break ;
					if ifnot (mAttr->is_number (top))
						break ;
					read (top) ;
				}
			}
			if ifswitch (TRUE) {
				if (top != ITEM ('e'))
					if (top != ITEM ('E'))
						discard ;
				read (top) ;
				assume (mAttr->is_number (top)) ;
				auto rbx = ZERO ;
				read_value (rbx ,top) ;
				assume (MathProc::abs (rbx) < mAttr->float_precision ()) ;
				fexp10.mExponent += rbx ;
				assume (MathProc::abs (fexp10.mExponent) < mAttr->float_precision ()) ;
			}
		}
	} ;
} ;

template <class ITEM>
trait TEXTREADER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	using Attribute = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Attribute ;

	using SPACE_ENUM_SPACE = RANK1 ;
	using SPACE_ENUM_ENDLINE = RANK2 ;

	class ImplAttribute implement Attribute {
	private:
		HashSet<ITEM> mSpaceSet ;
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit ImplAttribute () {
			mSpaceSet.add (ITEM (' ') ,SPACE_ENUM_SPACE::value) ;
			mSpaceSet.add (ITEM ('\t') ,SPACE_ENUM_SPACE::value) ;
			mSpaceSet.add (ITEM ('\b') ,SPACE_ENUM_SPACE::value) ;
			mSpaceSet.add (ITEM ('\r') ,SPACE_ENUM_ENDLINE::value) ;
			mSpaceSet.add (ITEM ('\n') ,SPACE_ENUM_ENDLINE::value) ;
			mSpaceSet.add (ITEM ('\f') ,SPACE_ENUM_ENDLINE::value) ;
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

		BOOL is_endline_space (CREF<ITEM> str) const override {
			return mSpaceSet.map (str) != SPACE_ENUM_ENDLINE::value ;
		}

		BOOL is_space (CREF<ITEM> str) const override {
			return mSpaceSet.find (str) != NONE ;
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

		ITEM lower_cast (CREF<ITEM> str) const override {
			if (str >= ITEM ('A'))
				if (str <= ITEM ('F'))
					return ITEM (INDEX (str) - INDEX ('A') + INDEX ('a')) ;
			return str ;
		}

		Optional<ITEM> escape_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeSet.map (str) ;
			if (ix == NONE)
				return Optional<ITEM>::none () ;
			return Optional<ITEM>::make (ITEM (ix)) ;
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
			return bad (TYPEAS<ITEM>::id) ;
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

		LENGTH value_precision () const override {
			return 19 ;
		}

		LENGTH float_precision () const override {
			return 1024 ;
		}
	} ;
} ;

template <class ITEM>
using TextReader = typename TEXTREADER_HELP<ITEM ,ALWAYS>::TextReader ;

template <class...>
trait TEXTWRITER_HELP ;

template <class...>
trait TEXTWRITER_BINDER_HELP ;

template <class...>
trait TEXTWRITER_IMPLHOLDER_HELP ;

template <class ITEM>
trait TEXTWRITER_BINDER_HELP<ITEM ,ALWAYS> {
	using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<TextWriter> writer) const = 0 ;
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	struct Attribute implement Interface {
		virtual ITEM ending_item () const = 0 ;
		virtual BOOL is_space (CREF<ITEM> str) const = 0 ;
		virtual ITEM lower_upper_cast (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_number (CREF<INDEX> hex) const = 0 ;
		virtual BOOL is_hex_number (CREF<INDEX> hex) const = 0 ;
		virtual ITEM str_from_hex (CREF<INDEX> hex) const = 0 ;
		virtual Optional<ITEM> escape_cast (CREF<ITEM> str) const = 0 ;
		virtual LENGTH float_precision () const = 0 ;
	} ;

	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<TEXTWRITER_BINDER_HELP<ITEM ,ALWAYS> ,ARG1>::Binder ;

	using NUMBER_SIZE = ENUMAS<VAL ,ENUMID<64>> ;

	class TextWriter {
	private:
		CRef<Attribute> mAttr ;
		VRef<RegBuffer<ITEM>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit TextWriter () = default ;

		explicit TextWriter (RREF<VRef<RegBuffer<ITEM>>> stream) {
			set_attr () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		void set_attr () {
			using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<ITEM ,ALWAYS>::ImplAttribute ;
			auto &&tmp = memorize ([&] () {
				return CRef<R1X>::make () ;
			}) ;
			set_attr (copy (tmp)) ;
		}

		void set_attr (RREF<CRef<Attribute>> attr_) {
			mAttr = move (attr_) ;
		}

		LENGTH length () const {
			return mWrite ;
		}

		void reset () {
			mRead = mStream->size () ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream->size ())) ;
			assert (vbetween (write_ ,0 ,mStream->size ())) ;
			assert (write_ <= read_) ;
			mRead = read_ ;
			mWrite = write_ ;
		}

		void backup () {
			mBackupRead = mRead ;
			mBackupWrite = mWrite ;
		}

		void recover () {
			mRead = mBackupRead ;
			mWrite = mBackupWrite ;
		}

		void prints () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void prints (XREF<ARG1> item1 ,XREF<ARG2>...items) {
			write (item1) ;
			prints (items...) ;
		}

		void write (CREF<ITEM> item) {
			if ifswitch (TRUE) {
				if (mWrite >= mRead)
					discard ;
				mStream.self[mWrite] = item ;
				mWrite++ ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<ITEM> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BOOL> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (item)
					discard ;
				write (mAttr->lower_upper_cast (ITEM ('t'))) ;
				write (mAttr->lower_upper_cast (ITEM ('r'))) ;
				write (mAttr->lower_upper_cast (ITEM ('u'))) ;
				write (mAttr->lower_upper_cast (ITEM ('e'))) ;
			}
			if ifswitch (eax) {
				write (mAttr->lower_upper_cast (ITEM ('f'))) ;
				write (mAttr->lower_upper_cast (ITEM ('a'))) ;
				write (mAttr->lower_upper_cast (ITEM ('l'))) ;
				write (mAttr->lower_upper_cast (ITEM ('s'))) ;
				write (mAttr->lower_upper_cast (ITEM ('e'))) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<BOOL> item) {
			write (item) ;
			return thiz ;
		}

		csc_pointer_t write (CREF<csc_pointer_t>) = delete ;

		inline VREF<TextWriter> operator<< (CREF<csc_pointer_t>) = delete ;

		void write (CREF<VAL32> item) {
			write (VAL64 (item)) ;
		}

		inline VREF<TextWriter> operator<< (CREF<VAL32> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<VAL64> item) {
			if ifswitch (TRUE) {
				if (item >= 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto eax = TRUE ;
			if ifswitch (eax) {
				if (item != VAL64_ABS)
					discard ;
				write (mAttr->lower_upper_cast (ITEM ('a'))) ;
				write (mAttr->lower_upper_cast (ITEM ('b'))) ;
				write (mAttr->lower_upper_cast (ITEM ('s'))) ;
			}
			if ifswitch (eax) {
				auto rbx = BoxBuffer<ITEM ,NUMBER_SIZE> (0) ;
				INDEX ix = rbx.size () ;
				auto rcx = NOTATION () ;
				rcx.mRadix = 10 ;
				rcx.mSign = FALSE ;
				rcx.mPrecision = 0 ;
				rcx.mMantissa = MathProc::abs (item) ;
				rcx.mExponent = 0 ;
				write_value (rcx ,rbx ,ix) ;
				for (auto &&i : iter (ix ,rbx.size ()))
					write (rbx[i]) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<VAL64> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<SINGLE> item) {
			write (DOUBLE (item)) ;
		}

		inline VREF<TextWriter> operator<< (CREF<SINGLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DOUBLE> item) {
			if ifswitch (TRUE) {
				if (item > 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (MathProc::infinite (item))
					discard ;
				write (mAttr->lower_upper_cast (ITEM ('i'))) ;
				write (mAttr->lower_upper_cast (ITEM ('n'))) ;
				write (mAttr->lower_upper_cast (ITEM ('f'))) ;
			}
			if ifswitch (eax) {
				auto rbx = BoxBuffer<ITEM ,NUMBER_SIZE> (0) ;
				INDEX ix = rbx.size () ;
				auto rcx = FloatProc::decode (MathProc::abs (item)) ;
				rcx = FloatProc::exp10_from_exp2 (rcx) ;
				write_float (rcx ,rbx ,ix) ;
				for (auto &&i : iter (ix ,rbx.size ()))
					write (rbx[i]) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<DOUBLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BYTE> item) {
			for (auto &&i : iter (0 ,SIZE_OF<BYTE>::value)) {
				INDEX ix = SIZE_OF<BYTE>::value - 1 - i ;
				const auto r1x = INDEX ((item >> (8 * ix + 4)) & BYTE (0XFF)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & BYTE (0XFF)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<BYTE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<WORD> item) {
			for (auto &&i : iter (0 ,SIZE_OF<WORD>::value)) {
				INDEX ix = SIZE_OF<WORD>::value - 1 - i ;
				const auto r1x = INDEX ((item >> (8 * ix + 4)) & WORD (0XFF)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & WORD (0XFF)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value)) {
				INDEX ix = SIZE_OF<CHAR>::value - 1 - i ;
				const auto r1x = INDEX ((item >> (8 * ix + 4)) & CHAR (0XFF)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & CHAR (0XFF)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			for (auto &&i : iter (0 ,SIZE_OF<DATA>::value)) {
				INDEX ix = SIZE_OF<DATA>::value - 1 - i ;
				const auto r1x = INDEX ((item >> (8 * ix + 4)) & DATA (0XFF)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & DATA (0XFF)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<DATA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Slice<ITEM>> item) {
			for (auto &&i : iter (0 ,item.size ()))
				write (item[i]) ;
		}

		inline VREF<TextWriter> operator<< (CREF<Slice<ITEM>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<ITEM>> item) {
			for (auto &&i : item.iter ())
				write (item[i]) ;
		}

		inline VREF<TextWriter> operator<< (CREF<String<ITEM>> item) {
			write (item) ;
			return thiz ;
		}

		template <class ARG1 = void>
		void write (CREF<MACRO_Binder<ARG1>> item) {
			item.friend_write (thiz) ;
		}

		template <class ARG1 = void>
		inline VREF<TextWriter> operator<< (CREF<MACRO_Binder<ARG1>> item) {
			write (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {
			noop () ;
		}

		void write (CREF<typeof (CLS)>) {
			reset () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		imports void BOM (CREF<PlaceHolder<RANK2>>) {
			noop () ;
		}

		void write (CREF<typeof (BOM)>) {
			reset () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (BOM)> item) {
			write (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {
			noop () ;
		}

		void write (CREF<typeof (GAP)> item) {
			write (ITEM ('\r')) ;
			write (ITEM ('\n')) ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {
			noop () ;
		}

		void write (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			write (r1x) ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}

	private:
		void write_value (VREF<NOTATION> fexp10 ,VREF<BoxBuffer<ITEM ,NUMBER_SIZE>> out ,VREF<INDEX> iw) {
			assert (fexp10.mRadix == 10) ;
			assert (fexp10.mExponent == 0) ;
			fexp10.mPrecision = vlog10 (fexp10.mMantissa) ;
			const auto r1x = fexp10.mPrecision ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				//@info: case '0'
				if (fexp10.mMantissa != 0)
					discard ;
				iw-- ;
				out[iw] = ITEM ('0') ;
			}
			if ifswitch (eax) {
				//@info: case 'xxx'
				for (auto &&i : iter (0 ,r1x)) {
					noop (i) ;
					iw-- ;
					out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
			}
		}

		void write_float (VREF<NOTATION> fexp10 ,VREF<BoxBuffer<ITEM ,NUMBER_SIZE>> out ,VREF<INDEX> iw) {
			assert (fexp10.mRadix == 10) ;
			fexp10.mPrecision = vlog10 (fexp10.mMantissa) ;
			const auto r1x = mAttr->float_precision () ;
			if ifswitch (TRUE) {
				if (fexp10.mPrecision == 0)
					discard ;
				while (TRUE) {
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
				fexp10.mMantissa = MathProc::round (fexp10.mMantissa ,VAL64 (10)) / 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			const auto r3x = fexp10.mPrecision ;
			const auto r4x = LENGTH (fexp10.mExponent) ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				//@info: case '0'
				if (fexp10.mMantissa != 0)
					discard ;
				iw-- ;
				out[iw] = ITEM ('0') ;
			}
			if ifswitch (eax) {
				//@info: case 'x.xxxExxx'
				const auto r5x = r3x - 1 + r4x ;
				if (MathProc::abs (r5x) < r1x)
					discard ;
				write_value (r5x ,out ,iw) ;
				if ifswitch (TRUE) {
					if (r5x <= 0)
						discard ;
					iw-- ;
					out[iw] = ITEM ('+') ;
				}
				iw-- ;
				out[iw] = mAttr->lower_upper_cast (ITEM ('e')) ;
				const auto r6x = vmax (LENGTH (r3x - 1 - r1x) ,ZERO) ;
				for (auto &&i : iter (0 ,r6x)) {
					noop (i) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				INDEX ix = iw - 1 ;
				for (auto &&i : iter (r6x ,r3x - 1)) {
					noop (i) ;
					iw-- ;
					out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				iw-- ;
				out[iw] = ITEM ('.') ;
				iw += LENGTH (out[ix] == ITEM ('.')) ;
				iw-- ;
				out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if ifswitch (eax) {
				//@info: case 'xxx000'
				if (r4x < 0)
					discard ;
				for (auto &&i : iter (0 ,r4x)) {
					noop (i) ;
					iw-- ;
					out[iw] = ITEM ('0') ;
				}
				for (auto &&i : iter (0 ,r3x)) {
					noop (i) ;
					iw-- ;
					out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
			}
			if ifswitch (eax) {
				//@info: case 'xxx.xxx'
				if (r4x < 1 - r3x)
					discard ;
				if (r4x >= 0)
					discard ;
				const auto r7x = vmax (LENGTH (-r4x - r1x) ,ZERO) ;
				for (auto &&i : iter (0 ,r7x)) {
					noop (i) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				INDEX ix = iw - 1 ;
				for (auto &&i : iter (r7x ,-r4x)) {
					noop (i) ;
					iw-- ;
					out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				iw-- ;
				out[iw] = ITEM ('.') ;
				iw += LENGTH (out[ix] == ITEM ('.')) ;
				for (auto &&i : iter (0 ,r3x + r4x)) {
					noop (i) ;
					iw-- ;
					out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
			}
			if ifswitch (eax) {
				//@info: case '0.000xxx'
				if (r4x >= 1 - r3x)
					discard ;
				if (r4x >= 0)
					discard ;
				const auto r8x = vmax (LENGTH (-r4x - r1x) ,ZERO) ;
				for (auto &&i : iter (0 ,r8x)) {
					noop (i) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				INDEX ix = iw - 1 ;
				for (auto &&i : iter (r8x ,r3x)) {
					noop (i) ;
					iw-- ;
					out[iw] = mAttr->str_from_hex (fexp10.mMantissa % 10) ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				const auto r9x = vmax (r8x ,r3x) ;
				for (auto &&i : iter (r9x ,-r4x)) {
					noop (i) ;
					iw-- ;
					out[iw] = ITEM ('0') ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
				}
				iw-- ;
				out[iw] = ITEM ('.') ;
				iw += LENGTH (out[ix] == ITEM ('.')) ;
				iw-- ;
				out[iw] = ITEM ('0') ;
			}
		}

		LENGTH vlog10 (CREF<VAL64> x) const {
			LENGTH ret = 0 ;
			auto rax = x ;
			while (TRUE) {
				if (rax == 0)
					break ;
				ret++ ;
				rax /= 10 ;
			}
			return move (ret) ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	using Attribute = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Attribute ;

	class ImplAttribute implement Attribute {
	private:
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit ImplAttribute () {
			mEscapeSet.add (ITEM ('\\') ,INDEX ('\\')) ;
			mEscapeSet.add (ITEM ('/') ,INDEX ('/')) ;
			mEscapeSet.add (ITEM ('\t') ,INDEX ('t')) ;
			mEscapeSet.add (ITEM ('\b') ,INDEX ('b')) ;
			mEscapeSet.add (ITEM ('\r') ,INDEX ('r')) ;
			mEscapeSet.add (ITEM ('\n') ,INDEX ('n')) ;
			mEscapeSet.add (ITEM ('\f') ,INDEX ('f')) ;
			mEscapeSet.add (ITEM ('\'') ,INDEX ('\'')) ;
			mEscapeSet.add (ITEM ('\"') ,INDEX ('\"')) ;
			mEscapeSet.add (ITEM (7) ,INDEX ('u')) ;
		}

		ITEM ending_item () const override {
			return ITEM (0) ;
		}

		BOOL is_space (CREF<ITEM> str) const override {
			return FALSE ;
		}

		ITEM lower_upper_cast (CREF<ITEM> str) const override {
			return str ;
		}

		BOOL is_number (CREF<INDEX> hex) const override {
			return vbetween (hex ,0 ,10) ;
		}

		BOOL is_hex_number (CREF<INDEX> hex) const override {
			return vbetween (hex ,10 ,16) ;
		}

		ITEM str_from_hex (CREF<INDEX> hex) const override {
			if (is_number (hex))
				return ITEM (INDEX ('0') + hex) ;
			const auto r1x = lower_upper_cast (ITEM ('a')) ;
			if (is_hex_number (hex))
				return ITEM (INDEX (r1x) + hex - 10) ;
			assume (FALSE) ;
			return bad (TYPEAS<ITEM>::id) ;
		}

		Optional<ITEM> escape_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeSet.map (str) ;
			if (ix == NONE)
				return Optional<ITEM>::none () ;
			return Optional<ITEM>::make (ITEM (ix)) ;
		}

		LENGTH float_precision () const override {
			return 15 ;
		}
	} ;
} ;

template <class ITEM>
using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

template <class...>
trait REGULARSTRING_HELP ;

template <class ITEM>
trait REGULARSTRING_HELP<ITEM ,ALWAYS> {
	class EscapeString extend Proxy {
	private:
		String<ITEM> mBase ;

	public:
		imports CREF<EscapeString> from (CREF<String<ITEM>> that) {
			return unsafe_deref (unsafe_cast[TYPEAS<TEMP<EscapeString>>::id] (unsafe_deptr (that))) ;
		}

		inline friend VREF<TextWriter<ITEM>> operator<< (VREF<TextWriter<ITEM>> wos ,CREF<EscapeString> thiz_) {
			const auto r1x = wos.get_attr () ;
			wos << ITEM ('\"') ;
			for (auto &&i : thiz_.mBase) {
				auto eax = TRUE ;
				if ifswitch (eax) {
					const auto r2x = r1x->escape_cast (i) ;
					if ifnot (r2x.exist ())
						discard ;
					wos << ITEM ('\\') ;
					wos << r2x.self ;
				}
				if ifswitch (eax) {
					wos << i ;
				}
			}
			wos << ITEM ('\"') ;
			return wos ;
		}
	} ;
} ;

template <class ITEM>
using EscapeString = typename REGULARSTRING_HELP<ITEM ,ALWAYS>::EscapeString ;

template <class...>
trait REPEATSTRING_HELP ;

template <class ITEM>
trait REPEATSTRING_HELP<ITEM ,ALWAYS> {
	using Binder = typename TEXTWRITER_BINDER_HELP<ITEM ,ALWAYS>::Binder ;
	using COUNTER_MAX_DEPTH = ENUMAS<VAL ,ENUMID<256>> ;

	class RepeatString implement Binder {
	private:
		String<ITEM> mGapText ;
		String<ITEM> mCommaText ;
		LENGTH mCounter ;
		Cell<LENGTH> mTightCounter ;
		Deque<Cell<BOOL>> mFirst ;

	public:
		implicit RepeatString () = delete ;

		explicit RepeatString (CREF<String<ITEM>> gap_text ,CREF<String<ITEM>> comma_text) {
			mGapText = gap_text ;
			mCommaText = comma_text ;
			mCounter = 0 ;
			mTightCounter = Cell<LENGTH>::make (COUNTER_MAX_DEPTH::value) ;
			mFirst = Deque<Cell<BOOL>> (COUNTER_MAX_DEPTH::value * 2) ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		void friend_write (VREF<TextWriter<ITEM>> writer) const override {
			INDEX ix = mFirst.tail () ;
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (mCounter > mTightCounter.fetch ())
					discard ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					writer << mCommaText ;
				}
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (eax) {
				if (mCounter != mTightCounter.fetch ())
					discard ;
				if ifnot (mFirst[ix].fetch ())
					discard ;
				mTightCounter.store (COUNTER_MAX_DEPTH::value) ;
				mFirst[ix].store (FALSE) ;
			}
			if ifswitch (eax) {
				writer << TextWriter<STRU8>::GAP ;
				if ifswitch (TRUE) {
					if (mFirst[ix].fetch ())
						discard ;
					writer << mCommaText ;
				}
				mFirst[ix].store (FALSE) ;
				for (auto &&i : iter (0 ,mCounter)) {
					noop (i) ;
					writer << mGapText ;
				}
			}
		}

		void tight () {
			mTightCounter.store (mCounter) ;
		}

		void enter () {
			assume (mCounter < COUNTER_MAX_DEPTH::value) ;
			mCounter++ ;
			mFirst.add (Cell<BOOL>::make (TRUE)) ;
		}

		void leave () {
			mFirst.pop () ;
			INDEX ix = mFirst.tail () ;
			mFirst[ix].store (TRUE) ;
			mCounter-- ;
		}
	} ;
} ;

template <class ITEM>
using RepeatString = typename REPEATSTRING_HELP<ITEM ,ALWAYS>::RepeatString ;

template <class...>
trait REGULARREADER_HELP ;

template <class DEPEND>
trait REGULARREADER_HELP<DEPEND ,ALWAYS> {
	class RegularReader {
	private:
		VRef<TextReader<STRU8>> mReader ;
		Deque<STRU8> mCache ;
		Deque<STRU8> mBackupCache ;
		BOOL mHintString ;
		LENGTH mHintSize ;

	public:
		implicit RegularReader () = default ;

		explicit RegularReader (RREF<VRef<TextReader<STRU8>>> reader ,CREF<LENGTH> ll_size) {
			mReader = move (reader) ;
			mReader.self >> TextReader<STRU8>::BOM ;
			mCache = Deque<STRU8> (ll_size) ;
			mBackupCache = Deque<STRU8> (ll_size) ;
			for (auto &&i : iter (0 ,ll_size)) {
				noop (i) ;
				INDEX ix = mCache.insert () ;
				mReader.self >> mCache[ix] ;
			}
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
		}

		CREF<STRU8> at (CREF<INDEX> index) const leftvalue {
			return mCache[mCache.access (index)] ;
		}

		inline CREF<STRU8> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
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

		void read () {
			mCache.take () ;
			INDEX ix = mCache.insert () ;
			mReader.self >> mCache[ix] ;
		}

		inline void operator++ (int) {
			read () ;
		}

		void read (CREF<Slice<STRA>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (thiz[0] == STRU8 (item[i])) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STRA>> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STRW>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (thiz[0] == STRU8 (item[i])) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STRW>> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STRU8>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (thiz[0] == STRU8 (item[i])) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STRU8>> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STRU16>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (thiz[0] == STRU8 (item[i])) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STRU16>> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STRU32>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (thiz[0] == STRU8 (item[i])) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STRU32>> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_IDENTIFIER (CREF<PlaceHolder<RANK1>>) {
			noop () ;
		}

		void read (CREF<typeof (HINT_IDENTIFIER)>) {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if ifswitch (TRUE) {
					if (mHintSize != 0)
						discard ;
					assume (r1x->is_word (thiz[0])) ;
				}
				const auto r2x = invoke ([&] () {
					if (r1x->is_word (thiz[0]))
						return FALSE ;
					if (r1x->is_number (thiz[0]))
						return FALSE ;
					if (thiz[0] == STRU8 ('-'))
						return FALSE ;
					return TRUE ;
				}) ;
				if (r2x)
					break ;
				thiz++ ;
				mHintSize++ ;
			}
			recover () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_IDENTIFIER)>) {
			read (HINT_IDENTIFIER) ;
			return thiz ;
		}

		imports void HINT_VALUE (CREF<PlaceHolder<RANK2>>) {
			noop () ;
		}

		void read (CREF<typeof (HINT_VALUE)>) {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->get_attr () ;
			if ifswitch (TRUE) {
				if ifnot (thiz[0] == STRU8 ('+'))
					if ifnot (thiz[0] == STRU8 ('-'))
						discard ;
				thiz++ ;
				mHintSize++ ;
			}
			const auto r2x = thiz[0] ;
			assume (r1x->is_number (r2x)) ;
			thiz++ ;
			mHintSize++ ;
			if ifswitch (TRUE) {
				if (r2x == STRU8 ('0'))
					discard ;
				while (TRUE) {
					if ifnot (r1x->is_number (thiz[0]))
						break ;
					thiz++ ;
					mHintSize++ ;
				}
			}
			if ifswitch (TRUE) {
				if (thiz[0] != STRU8 ('.'))
					discard ;
				thiz++ ;
				mHintSize++ ;
				while (TRUE) {
					if ifnot (r1x->is_number (thiz[0]))
						break ;
					thiz++ ;
					mHintSize++ ;
				}
			}
			if ifswitch (TRUE) {
				if ifnot (thiz[0] == STRU8 ('e'))
					if ifnot (thiz[0] == STRU8 ('E'))
						discard ;
				thiz++ ;
				mHintSize++ ;
				if ifswitch (TRUE) {
					if ifnot (thiz[0] == STRU8 ('+'))
						if ifnot (thiz[0] == STRU8 ('-'))
							discard ;
					thiz++ ;
					mHintSize++ ;
				}
				assume (r1x->is_number (thiz[0])) ;
				thiz++ ;
				mHintSize++ ;
				while (TRUE) {
					if ifnot (r1x->is_number (thiz[0]))
						break ;
					thiz++ ;
					mHintSize++ ;
				}
			}
			recover () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_VALUE)>) {
			read (HINT_VALUE) ;
			return thiz ;
		}

		imports void HINT_STRING (CREF<PlaceHolder<RANK3>>) {
			noop () ;
		}

		void read (CREF<typeof (HINT_STRING)>) {
			mHintString = TRUE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->get_attr () ;
			assume (thiz[0] == STRU8 ('\"')) ;
			thiz++ ;
			while (TRUE) {
				if (thiz[0] == r1x->ending_item ())
					break ;
				if (thiz[0] == STRU8 ('\"'))
					break ;
				const auto r2x = thiz[0] ;
				thiz++ ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					if ifnot (r2x == STRU8 ('\\'))
						discard ;
					thiz++ ;
					mHintSize++ ;
				}
				if ifswitch (eax) {
					assume (ifnot (r1x->is_control (r2x))) ;
					mHintSize++ ;
				}
			}
			assume (thiz[0] == STRU8 ('\"')) ;
			thiz++ ;
			recover () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_STRING)>) {
			read (HINT_STRING) ;
			return thiz ;
		}

		imports void HINT_WORD_GAP (CREF<PlaceHolder<RANK4>>) {
			noop () ;
		}

		void read (CREF<typeof (HINT_WORD_GAP)>) {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if (thiz[0] == r1x->ending_item ())
					break ;
				if (r1x->is_space (thiz[0]))
					break ;
				assume (ifnot (r1x->is_control (thiz[0]))) ;
				thiz++ ;
				mHintSize++ ;
			}
			recover () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_GAP)>) {
			read (HINT_WORD_GAP) ;
			return thiz ;
		}

		imports void HINT_WORD_GAP_ENDLINE (CREF<PlaceHolder<RANK5>>) {
			noop () ;
		}

		void read (CREF<typeof (HINT_WORD_GAP_ENDLINE)>) {
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if (thiz[0] == r1x->ending_item ())
					break ;
				if (r1x->is_endline_space (thiz[0]))
					break ;
				assume (ifnot (r1x->is_control (thiz[0]))) ;
				thiz++ ;
				mHintSize++ ;
			}
			recover () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_GAP_ENDLINE)>) {
			read (HINT_WORD_GAP_ENDLINE) ;
			return thiz ;
		}

		imports void SKIP_GAP (CREF<PlaceHolder<RANK6>>) {
			noop () ;
		}

		void read (CREF<typeof (SKIP_GAP)>) {
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if ifnot (r1x->is_space (thiz[0]))
					break ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_GAP)>) {
			read (SKIP_GAP) ;
			return thiz ;
		}

		imports void SKIP_GAP_SPACE (CREF<PlaceHolder<RANK7>>) {
			noop () ;
		}

		void read (CREF<typeof (SKIP_GAP_SPACE)>) {
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if (thiz[0] == r1x->ending_item ())
					break ;
				if ifnot (r1x->is_space (thiz[0]))
					break ;
				if (r1x->is_endline_space (thiz[0]))
					break ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_GAP_SPACE)>) {
			read (SKIP_GAP_SPACE) ;
			return thiz ;
		}

		imports void SKIP_GAP_ENDLINE (CREF<PlaceHolder<RANK8>>) {
			noop () ;
		}

		void read (CREF<typeof (SKIP_GAP_ENDLINE)>) {
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if (thiz[0] == r1x->ending_item ())
					break ;
				if ifnot (r1x->is_endline_space (thiz[0]))
					break ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_GAP_ENDLINE)>) {
			read (SKIP_GAP_ENDLINE) ;
			return thiz ;
		}

		void read (VREF<String<STRU8>> item) {
			const auto r1x = mReader->get_attr () ;
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
				assume (thiz[0] == STRU8 ('\"')) ;
				thiz++ ;
			}
			for (auto &&i : iter (0 ,r3x)) {
				const auto r4x = thiz[0] ;
				thiz++ ;
				auto eax = TRUE ;
				if ifswitch (eax) {
					const auto r5x = BOOL (r4x == STRU8 ('\\')) ;
					if ifnot (r5x)
						discard ;
					item[i] = r1x->escape_cast (thiz[0]) ;
					thiz++ ;
				}
				if ifswitch (eax) {
					assume (ifnot (r1x->is_control (r4x))) ;
					item[i] = r4x ;
				}
			}
			if ifswitch (TRUE) {
				if ifnot (r2x)
					discard ;
				assume (thiz[0] == STRU8 ('\"')) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (VREF<String<STRU8>> item) {
			read (item) ;
			return thiz ;
		}
	} ;
} ;

using RegularReader = typename REGULARREADER_HELP<DEPEND ,ALWAYS>::RegularReader ;
} ;
} ;

namespace CSC {
using namespace STREAM ;
} ;