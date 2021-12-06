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
trait BYTEREADER_SUPER_HELP ;

template <class...>
trait BYTEREADER_BINDER_HELP ;

template <>
trait BYTEREADER_SUPER_HELP<ALWAYS> {
	class Attribute {
	public:
		implicit Attribute () = default ;

		BYTE ending_item () const {
			return BYTE (0X00) ;
		}

		BOOL is_big_endian () const {
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (r1x) ;
				if (r2x.self[0] != BYTE (0X00))
					return FALSE ;
				return TRUE ;
			}) ;
		}
	} ;
} ;

template <>
trait BYTEREADER_HELP<ALWAYS> {
	using Attribute = typename BYTEREADER_SUPER_HELP<ALWAYS>::Attribute ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<BYTEREADER_BINDER_HELP<ALWAYS> ,ARG1>::Binder ;

	class ByteReader {
	private:
		CRef<Attribute> mAttribute ;
		ConBuffer<BYTE> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ByteReader () = default ;

		explicit ByteReader (RREF<ConBuffer<BYTE>> stream) {
			mAttribute = CRef<Attribute>::make () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CREF<Attribute> m_attr () const leftvalue {
			return mAttribute.self ;
		}

		CREF<RegBuffer<BYTE>> raw (RREF<ConBuffer<BYTE>> where_ = ConBuffer<BYTE> ()) const leftvalue {
			auto rax = CRef<ARR<BYTE>>::reference (mStream.self) ;
			return RegBuffer<BYTE>::from (where_ ,move (rax) ,mStream.size ()) ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = mStream.size () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream.size ())) ;
			assert (vbetween (write_ ,0 ,mStream.size ())) ;
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
		REMOVE_ALL<ARG1> poll (XREF<ARG1> id) leftvalue {
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
				item = mStream[mRead] ;
				mRead++ ;
			}
			if ifswitch (eax) {
				item = attr.ending_item () ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (attr.is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value))
					read (rax.self[i]) ;
				item = bitwise (TYPEAS<WORD>::id ,rax.self) ;
			}
			if ifswitch (eax) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value)) {
					INDEX ix = SIZE_OF<WORD>::value - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise (TYPEAS<WORD>::id ,rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (attr.is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value))
					read (rax.self[i]) ;
				item = bitwise (TYPEAS<CHAR>::id ,rax.self) ;
			}
			if ifswitch (eax) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value)) {
					INDEX ix = SIZE_OF<CHAR>::value - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise (TYPEAS<CHAR>::id ,rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (attr.is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value))
					read (rax.self[i]) ;
				item = bitwise (TYPEAS<DATA>::id ,rax.self) ;
			}
			if ifswitch (eax) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value)) {
					INDEX ix = SIZE_OF<DATA>::value - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise (TYPEAS<DATA>::id ,rax.self) ;
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
			item = bitwise (TYPEAS<BOOL>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			using R1X = BYTE_BASE<SIZE_OF<VAL32> ,ALIGN_OF<VAL32>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<VAL32>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			using R1X = BYTE_BASE<SIZE_OF<VAL64> ,ALIGN_OF<VAL64>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<VAL64>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			using R1X = BYTE_BASE<SIZE_OF<SINGLE> ,ALIGN_OF<SINGLE>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<SINGLE>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			using R1X = BYTE_BASE<SIZE_OF<DOUBLE> ,ALIGN_OF<DOUBLE>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<DOUBLE>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRA> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRA> ,ALIGN_OF<STRA>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<STRA>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRA> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRW> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRW> ,ALIGN_OF<STRW>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<STRW>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRW> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRU8> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRU8> ,ALIGN_OF<STRU8>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<STRU8>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRU8> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRU16> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRU16> ,ALIGN_OF<STRU16>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<STRU16>::id ,rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<STRU16> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<STRU32> item) {
			using R1X = BYTE_BASE<SIZE_OF<STRU32> ,ALIGN_OF<STRU32>> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise (TYPEAS<STRU32>::id ,rax) ;
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
	} ;
} ;

template <>
trait BYTEREADER_BINDER_HELP<ALWAYS> {
	using ByteReader = typename BYTEREADER_HELP<ALWAYS>::ByteReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<ByteReader> reader) = 0 ;
	} ;
} ;

using ByteReader = typename BYTEREADER_HELP<ALWAYS>::ByteReader ;

template <class...>
trait BYTEWRITER_HELP ;

template <class...>
trait BYTEWRITER_BINDER_HELP ;

template <class...>
trait BYTEWRITER_SUPER_HELP ;

template <class UNIT1>
trait BYTEWRITER_BINDER_HELP<UNIT1 ,ALWAYS> {
	using ByteWriter = typename DEPENDENT<BYTEWRITER_HELP<ALWAYS> ,UNIT1>::ByteWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<ByteWriter> writer) = 0 ;
	} ;
} ;

template <>
trait BYTEWRITER_SUPER_HELP<ALWAYS> {
	class Attribute {
	public:
		implicit Attribute () = default ;

		BOOL is_big_endian () const {
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (r1x) ;
				if (r2x.self[0] != BYTE (0X00))
					return FALSE ;
				return TRUE ;
			}) ;
		}
	} ;
} ;

template <>
trait BYTEWRITER_HELP<ALWAYS> {
	using Attribute = typename BYTEWRITER_SUPER_HELP<ALWAYS>::Attribute ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<BYTEWRITER_BINDER_HELP<ALWAYS> ,ARG1>::Binder ;

	class ByteWriter {
	private:
		CRef<Attribute> mAttribute ;
		VarBuffer<BYTE> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ByteWriter () = default ;

		explicit ByteWriter (RREF<ConBuffer<BYTE>> stream) {
			mAttribute = CRef<Attribute>::make () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CREF<Attribute> m_attr () const leftvalue {
			return mAttribute.self ;
		}

		CREF<RegBuffer<BYTE>> raw (RREF<ConBuffer<BYTE>> where_ = ConBuffer<BYTE> ()) const leftvalue {
			auto rax = CRef<ARR<BYTE>>::reference (mStream.self) ;
			return RegBuffer<BYTE>::from (where_ ,move (rax) ,mStream.size ()) ;
		}

		void reset () {
			mRead = mStream.size () ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream.size ())) ;
			assert (vbetween (write_ ,0 ,mStream.size ())) ;
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
				mStream[mWrite] = item ;
				mWrite++ ;
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<BYTE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<WORD> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (attr.is_big_endian ())
					discard ;
				const auto r1x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (item) ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value))
					write (r1x.self[i]) ;
			}
			if ifswitch (eax) {
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make (item) ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::value)) {
					INDEX ix = SIZE_OF<WORD>::value - 1 - i ;
					write (r2x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (attr.is_big_endian ())
					discard ;
				const auto r1x = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make (item) ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value))
					write (r1x.self[i]) ;
			}
			if ifswitch (eax) {
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make (item) ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::value)) {
					INDEX ix = SIZE_OF<CHAR>::value - 1 - i ;
					write (r2x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (attr.is_big_endian ())
					discard ;
				const auto r1x = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make (item) ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value))
					write (r1x.self[i]) ;
			}
			if ifswitch (eax) {
				const auto r2x = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make (item) ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::value)) {
					INDEX ix = SIZE_OF<DATA>::value - 1 - i ;
					write (r2x.self[ix]) ;
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
		void write (VREF<MACRO_Binder<ARG1>> item) {
			item.friend_write (thiz) ;
		}

		template <class ARG1 = void>
		inline VREF<ByteWriter> operator<< (VREF<MACRO_Binder<ARG1>> item) {
			write (item) ;
			return thiz ;
		}
	} ;
} ;

using ByteWriter = typename BYTEWRITER_HELP<ALWAYS>::ByteWriter ;

template <class...>
trait TEXTREADER_HELP ;

template <class...>
trait TEXTREADER_SUPER_HELP ;

template <class...>
trait TEXTREADER_BINDER_HELP ;

template <class ITEM>
trait TEXTREADER_SUPER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	class Attribute {
	public:
		implicit Attribute () = default ;

		ITEM ending_item () const {
			return ITEM (0) ;
		}

		BOOL is_number (CREF<ITEM> str) const {
			if (str >= ITEM ('0'))
				if (str <= ITEM ('9'))
					return TRUE ;
			return FALSE ;
		}

		BOOL is_letter (CREF<ITEM> str) const {
			if (str >= ITEM ('a'))
				if (str <= ITEM ('z'))
					return TRUE ;
			if (str >= ITEM ('A'))
				if (str <= ITEM ('Z'))
					return TRUE ;
			return FALSE ;
		}

		INDEX hex_from_str (CREF<ITEM> str) const {
			if (is_number (str))
				return INDEX (str) - INDEX ('0') ;
			if (is_letter (str))
				return INDEX (str) - INDEX ('a') + 10 ;
			dynamic_assert (FALSE) ;
			return bad (TYPEAS<INDEX>::id) ;
		}

		LENGTH value_precision () const {
			return 19 ;
		}

		LENGTH float_precision () const {
			return 1024 ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTREADER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	using NOTATION = typename FLOATPROC_HELP<ALWAYS>::NOTATION ;
	using Attribute = typename TEXTREADER_SUPER_HELP<ITEM ,ALWAYS>::Attribute ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<TEXTREADER_BINDER_HELP<ITEM ,ALWAYS> ,ARG1>::Binder ;

	class TextReader {
	private:
		CRef<Attribute> mAttribute ;
		ConBuffer<ITEM> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit TextReader () = default ;

		explicit TextReader (RREF<ConBuffer<ITEM>> stream) {
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CREF<Attribute> m_attr () const leftvalue {
			return mAttribute.self ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<ConBuffer<ITEM>> where_ = ConBuffer<ITEM> ()) const leftvalue {
			auto rax = CRef<ARR<ITEM>>::reference (mStream.self) ;
			return RegBuffer<ITEM>::from (where_ ,move (rax) ,mStream.size ()) ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = mStream.size () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream.size ())) ;
			assert (vbetween (write_ ,0 ,mStream.size ())) ;
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
		REMOVE_ALL<ARG1> poll (XREF<ARG1> id) leftvalue {
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
				item = mStream[mRead] ;
				mRead++ ;
			}
			if ifswitch (eax) {
				item = attr.ending_item () ;
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
				dynamic_assert (rax == ITEM ('r')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('u')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('e')) ;
				item = TRUE ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('f'))
					discard ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('a')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('l')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('s')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('e')) ;
				item = FALSE ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('T'))
					discard ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('R')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('U')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('E')) ;
				item = TRUE ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('F'))
					discard ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('A')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('L')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('S')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('E')) ;
				item = FALSE ;
			}
			if ifswitch (eax) {
				dynamic_assert (FALSE) ;
			}
		}

		inline VREF<TextReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			const auto r1x = read (TYPEAS<VAL64>::id) ;
			dynamic_assert (r1x >= VAL32_MIN) ;
			dynamic_assert (r1x <= VAL32_MAX) ;
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
				dynamic_assert (rax == ITEM ('b')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('s')) ;
				item = VAL64_ABS ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('A'))
					discard ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('B')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('S')) ;
				item = VAL64_ABS ;
			}
			if ifswitch (eax) {
				dynamic_assert (attr.is_number (rax)) ;
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
		}

		inline VREF<TextReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			const auto r1x = read (TYPEAS<DOUBLE>::id) ;
			dynamic_assert (r1x >= SINGLE_MIN) ;
			dynamic_assert (r1x <= SINGLE_MAX) ;
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
				dynamic_assert (rax == ITEM ('n')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('f')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (eax) {
				if (rax != ITEM ('I'))
					discard ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('N')) ;
				read (rax) ;
				dynamic_assert (rax == ITEM ('F')) ;
				item = DOUBLE_INF ;
			}
			if ifswitch (eax) {
				dynamic_assert (attr.is_number (rax)) ;
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
				const auto r1x = BYTE (attr.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = BYTE (attr.hex_from_str (rax)) ;
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
				const auto r1x = WORD (attr.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = WORD (attr.hex_from_str (rax)) ;
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
				const auto r1x = CHAR (attr.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = CHAR (attr.hex_from_str (rax)) ;
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
				const auto r1x = DATA (attr.hex_from_str (rax)) ;
				item = (item << 4) | r1x ;
				read (rax) ;
				const auto r2x = DATA (attr.hex_from_str (rax)) ;
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
				dynamic_assert (rax == item[i]) ;
			}
		}

		inline VREF<TextReader> operator>> (CREF<Slice<ITEM>> item) {
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

	private:
		void read_value (VREF<NOTATION> fexp10 ,VREF<ITEM> top) {
			assert (fexp10.mRadix == 10) ;
			const auto r100x = attr.value_precision () ;
			const auto r101x = attr.float_precision () ;
			fexp10.mMantissa = attr.hex_from_str (top) ;
			fexp10.mPrecision++ ;
			fexp10.mExponent = 0 ;
			if ifswitch (TRUE) {
				while (TRUE) {
					backup () ;
					read (top) ;
					if ifnot (attr.is_number (top))
						break ;
					if (fexp10.mPrecision > r100x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += attr.hex_from_str (top) ;
					fexp10.mPrecision++ ;
				}
				while (TRUE) {
					if ifnot (attr.is_number (top))
						break ;
					if (fexp10.mPrecision > r101x)
						break ;
					fexp10.mExponent++ ;
					fexp10.mPrecision++ ;
					backup () ;
					read (top) ;
				}
			}
			dynamic_assert (fexp10.mPrecision <= r100x) ;
			recover () ;
		}

		void read_float (VREF<NOTATION> fexp10 ,VREF<ITEM> top) {
			assert (fexp10.mRadix == 10) ;
			const auto r101x = attr.float_precision () ;
			read_value (fexp10 ,top) ;
			read (top) ;
			if ifswitch (TRUE) {
				if (top != ITEM ('.'))
					discard ;
				read (top) ;
				while (TRUE) {
					backup () ;
					read (top) ;
					if ifnot (attr.is_number (top))
						break ;
					if (fexp10 > r101x)
						break ;
					fexp10.mMantissa *= 10 ;
					fexp10.mMantissa += attr.hex_from_str (top) ;
					fexp10.mExponent-- ;
					fexp10.mPrecision++ ;
				}
				while (TRUE) {
					if ifnot (attr.is_number (top))
						break ;
					backup () ;
					read (top) ;
				}
			}
			if ifswitch (TRUE) {
				if (top != ITEM ('e'))
					if (top != ITEM ('E'))
						discard ;
				backup () ;
				read (top) ;
				dynamic_assert (attr.is_number (top)) ;
				auto rbx = ZERO ;
				read_value (rbx ,top) ;
				dynamic_assert (MathProc::abs (rbx) < attr.float_precision ()) ;
				fexp10.mExponent += rbx ;
				dynamic_assert (MathProc::abs (fexp10.mExponent) < attr.float_precision ()) ;
			}
			recover () ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTREADER_BINDER_HELP<ITEM ,ALWAYS> {
	using TextReader = typename TEXTREADER_HELP<ITEM ,ALWAYS>::TextReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<TextReader> reader) = 0 ;
	} ;
} ;

template <class ITEM>
using TextReader = typename TEXTREADER_HELP<ITEM ,ALWAYS>::TextReader ;

template <class...>
trait TEXTWRITER_HELP ;

template <class...>
trait TEXTWRITER_SUPER_HELP ;

template <class...>
trait TEXTWRITER_BINDER_HELP ;

template <class ITEM>
trait TEXTWRITER_SUPER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	class Attribute {
	public:
		implicit Attribute () = default ;

		ITEM ending_item () const {
			return ITEM (0) ;
		}

		ITEM lower_upper_cast (CREF<ITEM> str) const {
			return str ;
		}

		ITEM str_from_hex (CREF<INDEX> hex) const {
			if (vbetween (hex ,0 ,10))
				return ITEM (INDEX ('0') + hex) ;
			const auto r1x = lower_upper_cast (ITEM ('a')) ;
			if (vbetween (hex ,10 ,16))
				return ITEM (INDEX (r1x) + hex - 10) ;
			dynamic_assert (FALSE) ;
			return bad (TYPEAS<ITEM>::id) ;
		}

		LENGTH float_precision () const {
			return 15 ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_HELP<ITEM ,REQUIRE<IS_STR<ITEM>>> {
	using NOTATION = typename FLOATPROC_HELP<ALWAYS>::NOTATION ;
	using Attribute = typename TEXTWRITER_SUPER_HELP<ITEM ,ALWAYS>::Attribute ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<TEXTWRITER_BINDER_HELP<ITEM ,ALWAYS> ,ARG1>::Binder ;

	using NUMBER_SIZE = ENUMAS<VAL ,ENUMID<64>> ;

	class TextWriter {
	private:
		CRef<Attribute> mAttribute ;
		ConBuffer<ITEM> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit TextWriter () = default ;

		explicit TextWriter (RREF<VarBuffer<ITEM>> stream) {
			mAttribute = CRef<Attribute>::make () ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CREF<Attribute> m_attr () const leftvalue {
			return mAttribute.self ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<ConBuffer<ITEM>> where_ = ConBuffer<ITEM> ()) const leftvalue {
			auto rax = CRef<ARR<ITEM>>::reference (mStream.self) ;
			return RegBuffer<ITEM>::from (where_ ,move (rax) ,mStream.size ()) ;
		}

		void reset () {
			mRead = mStream.size () ;
			mWrite = 0 ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			assert (vbetween (read_ ,0 ,mStream.size ())) ;
			assert (vbetween (write_ ,0 ,mStream.size ())) ;
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
				mStream[mWrite] = item ;
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
				write (attr.lower_upper_cast (ITEM ('t'))) ;
				write (attr.lower_upper_cast (ITEM ('r'))) ;
				write (attr.lower_upper_cast (ITEM ('u'))) ;
				write (attr.lower_upper_cast (ITEM ('e'))) ;
			}
			if ifswitch (eax) {
				write (attr.lower_upper_cast (ITEM ('f'))) ;
				write (attr.lower_upper_cast (ITEM ('a'))) ;
				write (attr.lower_upper_cast (ITEM ('l'))) ;
				write (attr.lower_upper_cast (ITEM ('s'))) ;
				write (attr.lower_upper_cast (ITEM ('e'))) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<BOOL> item) {
			write (item) ;
			return thiz ;
		}

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
				write (attr.lower_upper_cast (ITEM ('a'))) ;
				write (attr.lower_upper_cast (ITEM ('b'))) ;
				write (attr.lower_upper_cast (ITEM ('s'))) ;
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
				if ifnot (MathProc::is_infinity (item))
					discard ;
				write (attr.lower_upper_cast (ITEM ('i'))) ;
				write (attr.lower_upper_cast (ITEM ('n'))) ;
				write (attr.lower_upper_cast (ITEM ('f'))) ;
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
				write (attr.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & BYTE (0XFF)) ;
				write (attr.str_from_hex (r2x)) ;
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
				write (attr.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & WORD (0XFF)) ;
				write (attr.str_from_hex (r2x)) ;
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
				write (attr.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & CHAR (0XFF)) ;
				write (attr.str_from_hex (r2x)) ;
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
				write (attr.str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (8 * ix + 0)) & DATA (0XFF)) ;
				write (attr.str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<DATA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Slice<ITEM>> item) {
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,item.size ()))
				write (item[i]) ;
		}

		inline VREF<TextWriter> operator<< (CREF<Slice<ITEM>> item) {
			read (item) ;
			return thiz ;
		}

		template <class ARG1 = void>
		void write (VREF<MACRO_Binder<ARG1>> item) {
			item.friend_write (thiz) ;
		}

		template <class ARG1 = void>
		inline VREF<TextWriter> operator<< (VREF<MACRO_Binder<ARG1>> item) {
			write (item) ;
			return thiz ;
		}

	private:
		void write_value (VREF<NOTATION> fexp10 ,VREF<BoxBuffer<ITEM ,NUMBER_SIZE>> out ,VREF<INDEX> iw) {
			assert (fexp10.mRadix == 10) ;
			assert (fexp10.mExponent == 0) ;
			fexp10.mPrecision = vlog10 (fexp10.mMantissa) ;
			const auto r30x = fexp10.mPrecision ;
			const auto r31x = LENGTH (fexp10.mExponent) ;
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
				for (auto &&i : iter (0 ,r30x)) {
					noop (i) ;
					iw-- ;
					out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
			}
		}

		void write_float (VREF<NOTATION> fexp10 ,VREF<BoxBuffer<ITEM ,NUMBER_SIZE>> out ,VREF<INDEX> iw) {
			assert (fexp10.mRadix == 10) ;
			fexp10.mPrecision = vlog10 (fexp10.mMantissa) ;
			const auto r101x = attr.float_precision () ;
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
				const auto r2x = fexp10.mPrecision - r101x ;
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
			const auto r30x = fexp10.mPrecision ;
			const auto r31x = LENGTH (fexp10.mExponent) ;
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
				const auto r4x = r30x - 1 + r31x ;
				if (MathProc::abs (r4x) < r101x)
					discard ;
				write_value (r4x ,out ,iw) ;
				if ifswitch (TRUE) {
					if (r4x <= 0)
						discard ;
					iw-- ;
					out[iw] = ITEM ('+') ;
				}
				iw-- ;
				out[iw] = ITEM ('e') ;
				const auto r5x = vmax (LENGTH (r30x - 1 - r101x) ,ZERO) ;
				for (auto &&i : iter (0 ,r5x)) {
					noop (i) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				INDEX ix = iw - 1 ;
				for (auto &&i : iter (r5x ,r30x - 1)) {
					noop (i) ;
					iw-- ;
					out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				iw-- ;
				out[iw] = ITEM ('.') ;
				iw += LENGTH (out[ix] == ITEM ('.')) ;
				iw-- ;
				out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
				fexp10.mMantissa /= 10 ;
				fexp10.mExponent++ ;
				fexp10.mPrecision-- ;
			}
			if ifswitch (eax) {
				//@info: case 'xxx000'
				if (r31x < 0)
					discard ;
				for (auto &&i : iter (0 ,r31x)) {
					noop (i) ;
					iw-- ;
					out[iw] = ITEM ('0') ;
				}
				for (auto &&i : iter (0 ,r30x)) {
					noop (i) ;
					iw-- ;
					out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
			}
			if ifswitch (eax) {
				//@info: case 'xxx.xxx'
				if (r31x < 1 - r30x)
					discard ;
				if (r31x >= 0)
					discard ;
				const auto r6x = vmax (LENGTH (-r31x - r101x) ,ZERO) ;
				for (auto &&i : iter (0 ,r6x)) {
					noop (i) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				INDEX ix = iw - 1 ;
				for (auto &&i : iter (r6x ,-r31x)) {
					noop (i) ;
					iw-- ;
					out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				iw-- ;
				out[iw] = ITEM ('.') ;
				iw += LENGTH (out[ix] == ITEM ('.')) ;
				for (auto &&i : iter (0 ,r30x + r31x)) {
					noop (i) ;
					iw-- ;
					out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
			}
			if ifswitch (eax) {
				//@info: case '0.000xxx'
				if (r31x >= 1 - r30x)
					discard ;
				if (r31x >= 0)
					discard ;
				const auto r7x = vmax (LENGTH (-r31x - r101x) ,ZERO) ;
				for (auto &&i : iter (0 ,r7x)) {
					noop (i) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				INDEX ix = iw - 1 ;
				for (auto &&i : iter (r7x ,r30x)) {
					noop (i) ;
					iw-- ;
					out[iw] = attr.str_from_hex (fexp10.mMantissa % 10) ;
					iw += LENGTH (out[ix] == ITEM ('0')) ;
					fexp10.mMantissa /= 10 ;
					fexp10.mExponent++ ;
					fexp10.mPrecision-- ;
				}
				const auto r8x = vmax (r7x ,r30x) ;
				for (auto &&i : iter (r8x ,-r31x)) {
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
trait TEXTWRITER_BINDER_HELP<ITEM ,ALWAYS> {
	using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<TextWriter> writer) = 0 ;
	} ;
} ;

template <class ITEM>
using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;
} ;
} ;

namespace CSC {
using namespace STREAM ;
} ;