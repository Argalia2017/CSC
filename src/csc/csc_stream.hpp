#pragma once

#ifndef __CSC_STREAM__
#define __CSC_STREAM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
template <class...>
trait BYTEREADER_HELP ;

template <class...>
trait BYTEREADER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BYTEREADER_HELP<DEPEND ,ALWAYS> {
	struct Attribute implement Interface {
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<BYTEREADER_HELP<DEPEND ,ALWAYS> ,ARG1>::Binder ;

	class ByteReader {
	protected:
		CRef<Attribute> mAttr ;
		CRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ByteReader () = default ;

		explicit ByteReader (RREF<CRef<RegBuffer<BYTE>>> stream) {
			using R1X = typename BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			set_attr (TYPEAS<R1X>::expr) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			mAttr = CRef<ARG1>::make () ;
		}

		LENGTH size () const {
			return mStream->size () ;
		}

		LENGTH length () const {
			return mRead ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = 0 ;
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
		ARG1 poll (CREF<TYPEID<ARG1>> id) {
			ARG1 ret ;
			read (ret) ;
			return move (ret) ;
		}

		void scans () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void scans (VREF<ARG1> item1 ,VREF<ARG2>...item2) {
			read (item1) ;
			scans (item2...) ;
		}

		void read (VREF<BYTE> item) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mRead >= mStream->size ())
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (rxx) {
				item = mAttr->ending_item () ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr))
					read (rax.self[i]) ;
				item = bitwise[TYPEAS<WORD>::expr] (rax.self) ;
			}
			if ifswitch (rxx) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<WORD>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
					INDEX ix = SIZE_OF<WORD>::expr - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise[TYPEAS<WORD>::expr] (rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr))
					read (rax.self[i]) ;
				item = bitwise[TYPEAS<CHAR>::expr] (rax.self) ;
			}
			if ifswitch (rxx) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<CHAR>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
					INDEX ix = SIZE_OF<CHAR>::expr - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise[TYPEAS<CHAR>::expr] (rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				auto rax = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr))
					read (rax.self[i]) ;
				item = bitwise[TYPEAS<DATA>::expr] (rax.self) ;
			}
			if ifswitch (rxx) {
				auto rax = Box<ARR<BYTE ,SIZE_OF<DATA>>>::make () ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
					INDEX ix = SIZE_OF<DATA>::expr - 1 - i ;
					read (rax.self[ix]) ;
				}
				item = bitwise[TYPEAS<DATA>::expr] (rax.self) ;
			}
		}

		inline VREF<ByteReader> operator>> (VREF<DATA> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BOOL> item) {
			using R1X = BYTE_BASE<BOOL> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<BOOL>::expr] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			using R1X = DEPENDENT<BYTE_BASE<VAL32> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<VAL32>::expr] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			using R1X = DEPENDENT<BYTE_BASE<VAL64> ,DEPEND> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<VAL64>::expr] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			using R1X = BYTE_BASE<SINGLE> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<SINGLE>::expr] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			using R1X = BYTE_BASE<DOUBLE> ;
			auto rax = R1X () ;
			read (rax) ;
			item = bitwise[TYPEAS<DOUBLE>::expr] (rax) ;
		}

		inline VREF<ByteReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STR>> item) {
			auto rax = BYTE () ;
			for (auto &&i : iter (0 ,item.size ())) {
				vbetween (INDEX (item[i]) ,0 ,128) ;
				read (rax) ;
				assume (rax == BYTE (item[i])) ;
			}
		}

		inline VREF<ByteReader> operator>> (CREF<Slice<STR>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRA>> item) {
			using R1X = BYTE_BASE<STRA> ;
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

		inline VREF<ByteReader> operator>> (VREF<String<STRA>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRW>> item) {
			using R1X = BYTE_BASE<STRW> ;
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

		inline VREF<ByteReader> operator>> (VREF<String<STRW>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU8>> item) {
			using R1X = BYTE_BASE<STRU8> ;
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

		inline VREF<ByteReader> operator>> (VREF<String<STRU8>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU16>> item) {
			using R1X = BYTE_BASE<STRU16> ;
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

		inline VREF<ByteReader> operator>> (VREF<String<STRU16>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU32>> item) {
			using R1X = BYTE_BASE<STRU32> ;
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

		inline VREF<ByteReader> operator>> (VREF<String<STRU32>> item) {
			read (item) ;
			return thiz ;
		}

		template <class ARG1 = DEPEND>
		void read (VREF<MACRO_Binder<ARG1>> item) {
			item.friend_read (thiz) ;
		}

		template <class ARG1 = DEPEND>
		inline VREF<ByteReader> operator>> (VREF<MACRO_Binder<ARG1>> item) {
			read (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void read (CREF<typeof (CLS)> item) {
			reset () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

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

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void read (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			auto rax = BYTE () ;
			for (auto &&i : iter (mRead ,mStream->size ())) {
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

	struct Binder implement Interface {
		virtual void friend_read (VREF<ByteReader> reader) = 0 ;
	} ;
} ;

template <class DEPEND>
trait BYTEREADER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Attribute = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Attribute ;

	class ImplHolder implement Attribute {
	public:
		implicit ImplHolder () = default ;

		BYTE ending_item () const override {
			return BYTE (0X00) ;
		}

		BYTE space_item () const override {
			return BYTE (0XCC) ;
		}

		BOOL is_big_endian () const override {
			using R1X = Box<ARR<BYTE ,SIZE_OF<WORD>>> ;
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = bitwise[TYPEAS<R1X>::expr] (r1x) ;
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
trait BYTEWRITER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BYTEWRITER_HELP<DEPEND ,ALWAYS> {
	struct Attribute implement Interface {
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	template <class ARG1>
	using MACRO_Binder = typename DEPENDENT<BYTEWRITER_HELP<DEPEND ,ALWAYS> ,ARG1>::Binder ;

	class ByteWriter {
	protected:
		CRef<Attribute> mAttr ;
		VRef<RegBuffer<BYTE>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit ByteWriter () = default ;

		explicit ByteWriter (RREF<VRef<RegBuffer<BYTE>>> stream) {
			using R1X = typename BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			set_attr (TYPEAS<R1X>::expr) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			mAttr = CRef<ARG1>::make () ;
		}

		LENGTH size () const {
			return mStream->size () ;
		}

		LENGTH length () const {
			return mWrite ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = 0 ;
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

		void prints () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void prints (CREF<ARG1> item1 ,CREF<ARG2>...item2) {
			write (item1) ;
			prints (item2...) ;
		}

		void write (CREF<BYTE> item) {
			if ifswitch (TRUE) {
				if (mWrite >= mStream->size ())
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
			using R1X = Box<ARR<BYTE ,SIZE_OF<WORD>>> ;
			const auto r1x = bitwise[TYPEAS<R1X>::expr] (item) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr))
					write (r1x.self[i]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
					INDEX ix = SIZE_OF<WORD>::expr - 1 - i ;
					write (r1x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			using R1X = Box<ARR<BYTE ,SIZE_OF<CHAR>>> ;
			const auto r1x = bitwise[TYPEAS<R1X>::expr] (item) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr))
					write (r1x.self[i]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
					INDEX ix = SIZE_OF<CHAR>::expr - 1 - i ;
					write (r1x.self[ix]) ;
				}
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			using R1X = Box<ARR<BYTE ,SIZE_OF<DATA>>> ;
			const auto r1x = bitwise[TYPEAS<R1X>::expr] (item) ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (mAttr->is_big_endian ())
					discard ;
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr))
					write (r1x.self[i]) ;
			}
			if ifswitch (rxx) {
				for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
					INDEX ix = SIZE_OF<DATA>::expr - 1 - i ;
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

		void write (CREF<csc_const_pointer_t>) = delete ;

		inline VREF<ByteWriter> operator<< (CREF<csc_const_pointer_t>) = delete ;

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

		void write (CREF<Slice<STR>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				vbetween (INDEX (item[i]) ,0 ,128) ;
				const auto r1x = BYTE (item[i]) ;
				write (r1x) ;
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<Slice<STR>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRA>> item) {
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

		inline VREF<ByteWriter> operator<< (CREF<String<STRA>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRW>> item) {
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

		inline VREF<ByteWriter> operator<< (CREF<String<STRW>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRU8>> item) {
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

		inline VREF<ByteWriter> operator<< (CREF<String<STRU8>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRU16>> item) {
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

		inline VREF<ByteWriter> operator<< (CREF<String<STRU16>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRU32>> item) {
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

		inline VREF<ByteWriter> operator<< (CREF<String<STRU32>> item) {
			write (item) ;
			return thiz ;
		}

		template <class ARG1 = DEPEND>
		void write (CREF<MACRO_Binder<ARG1>> item) {
			item.friend_write (thiz) ;
		}

		template <class ARG1 = DEPEND>
		inline VREF<ByteWriter> operator<< (CREF<MACRO_Binder<ARG1>> item) {
			write (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void write (CREF<typeof (CLS)>) {
			reset () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void write (CREF<typeof (GAP)> item) {
			const auto r1x = mAttr->space_item () ;
			write (r1x) ;
			write (r1x) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void write (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			for (auto &&i : iter (mWrite ,mStream->size ())) {
				noop (i) ;
				write (r1x) ;
			}
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}
	} ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<ByteWriter> writer) const = 0 ;
	} ;
} ;

template <class DEPEND>
trait BYTEWRITER_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Attribute = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Attribute ;

	class ImplHolder implement Attribute {
	public:
		implicit ImplHolder () = default ;

		BYTE ending_item () const override {
			return BYTE (0X00) ;
		}

		BYTE space_item () const override {
			return BYTE (0XCC) ;
		}

		BOOL is_big_endian () const override {
			using R1X = Box<ARR<BYTE ,SIZE_OF<WORD>>> ;
			return memorize ([&] () {
				const auto r1x = WORD (0X00FF) ;
				const auto r2x = bitwise[TYPEAS<R1X>::expr] (r1x) ;
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
trait TEXTREADER_IMPLHOLDER_HELP ;

template <class ITEM>
trait TEXTREADER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
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
	using MACRO_Binder = typename DEPENDENT<TEXTREADER_HELP<ITEM ,ALWAYS> ,ARG1>::Binder ;

	class TextReader {
	protected:
		CRef<Attribute> mAttr ;
		CRef<RegBuffer<ITEM>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit TextReader () = default ;

		explicit TextReader (RREF<CRef<RegBuffer<ITEM>>> stream) {
			using R1X = typename TEXTREADER_IMPLHOLDER_HELP<ITEM ,ALWAYS>::ImplHolder ;
			set_attr (TYPEAS<R1X>::expr) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			mAttr = CRef<ARG1>::make () ;
		}

		LENGTH size () const {
			return mStream->size () ;
		}

		LENGTH length () const {
			return mRead ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = 0 ;
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
		ARG1 poll (CREF<TYPEID<ARG1>> id) {
			ARG1 ret ;
			read (ret) ;
			return move (ret) ;
		}

		void scans () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void scans (VREF<ARG1> item1 ,VREF<ARG2>...item2) {
			read (item1) ;
			scans (item2...) ;
		}

		void read (VREF<ITEM> item) {
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if (mRead >= mStream->size ())
					discard ;
				item = mStream.self[mRead] ;
				mRead++ ;
			}
			if ifswitch (rxx) {
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

		inline VREF<TextReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			const auto r1x = poll (TYPEAS<VAL64>::expr) ;
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
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
			push_back (rax) ;
		}

		inline VREF<TextReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			const auto r1x = poll (TYPEAS<DOUBLE>::expr) ;
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
			push_back (rax) ;
		}

		inline VREF<TextReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BYTE> item) {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = BYTE (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<BYTE>::expr)) {
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
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = WORD (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
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
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = CHAR (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
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
			read (rax) ;
			assume (rax == ITEM ('0')) ;
			read (rax) ;
			assume (rax == ITEM ('X')) ;
			item = DATA (0X00) ;
			for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
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

		void read (CREF<Slice<STR>> item) {
			auto rax = ITEM () ;
			for (auto &&i : iter (0 ,item.size ())) {
				vbetween (INDEX (item[i]) ,0 ,128) ;
				read (rax) ;
				assume (rax == ITEM (item[i])) ;
			}
		}

		inline VREF<TextReader> operator>> (CREF<Slice<STR>> item) {
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

		template <class ARG1 = DEPEND>
		void read (VREF<MACRO_Binder<ARG1>> item) {
			item.friend_read (thiz) ;
		}

		template <class ARG1 = DEPEND>
		inline VREF<TextReader> operator>> (VREF<MACRO_Binder<ARG1>> item) {
			read (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void read (CREF<typeof (CLS)>) {
			reset () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		imports void BOM (CREF<PlaceHolder<RANK2>>) {}

		void read (CREF<typeof (BOM)>) {
			template_bom (PHX ,TYPEAS<ITEM>::expr) ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (BOM)> item) {
			read (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void read (CREF<typeof (GAP)> item) {
			const auto r1x = next_not_space_size () ;
			mRead += r1x ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

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
			return move (ret) ;
		}

		LENGTH next_not_space_size () {
			LENGTH ret = 0 ;
			auto rax = ITEM () ;
			const auto r1x = mRead ;
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
					if (fexp10.mPrecision > r1x)
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
				auto rbx = NOTATION () ;
				rbx.mRadix = 10 ;
				rbx.mSign = FALSE ;
				rbx.mPrecision = 0 ;
				read_value (rbx ,top) ;
				assume (MathProc::abs (rbx.mMantissa) < mAttr->float_precision ()) ;
				fexp10.mExponent += rbx.mMantissa ;
				assume (MathProc::abs (fexp10.mExponent) < mAttr->float_precision ()) ;
			}
		}

		void push_back (CREF<ITEM> top) {
			if ifswitch (TRUE) {
				mRead-- ;
				if (mRead < mStream->size () - 1)
					discard ;
				if (top != mAttr->ending_item ())
					discard ;
				mRead++ ;
			}
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
			read (rax) ;
			assume (rax == ITEM (0XEF)) ;
			read (rax) ;
			assume (rax == ITEM (0XBB)) ;
			read (rax) ;
			assume (rax == ITEM (0XBF)) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU16>>>
		void template_bom (CREF<typeof (PH2)> ,CREF<TYPEID<ARG1>> id) {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM (0XFEFF)) ;
			read (rax) ;
			assume (rax == ITEM (0XFFFE)) ;
		}

		template <class ARG1 ,class = REQUIRE<IS_SAME<ARG1 ,STRU32>>>
		void template_bom (CREF<typeof (PH1)> ,CREF<TYPEID<ARG1>> id) {
			auto rax = ITEM () ;
			read (rax) ;
			assume (rax == ITEM (0X0000FEFF)) ;
			read (rax) ;
			assume (rax == ITEM (0XFFFE0000)) ;
		}
	} ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<TextReader> reader) = 0 ;
	} ;
} ;

template <class ITEM>
trait TEXTREADER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using Attribute = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Attribute ;

	using SPACE_CLAZZ_WORD = RANK1 ;
	using SPACE_CLAZZ_ENDLINE = RANK2 ;

	class ImplHolder implement Attribute {
	protected:
		HashSet<ITEM> mSpaceSet ;
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit ImplHolder () {
			mSpaceSet.add (ITEM (' ') ,SPACE_CLAZZ_WORD::expr) ;
			mSpaceSet.add (ITEM ('\t') ,SPACE_CLAZZ_WORD::expr) ;
			mSpaceSet.add (ITEM ('\b') ,SPACE_CLAZZ_WORD::expr) ;
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

		ITEM lower_cast (CREF<ITEM> str) const override {
			if (str >= ITEM ('A'))
				if (str <= ITEM ('F'))
					return ITEM (INDEX (str) - INDEX ('A') + INDEX ('a')) ;
			return str ;
		}

		Optional<ITEM> escape_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeSet.map (str) ;
			if (ix == NONE)
				return FLAG (1) ;
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
trait TEXTWRITER_IMPLHOLDER_HELP ;

template <class...>
trait TEXTWRITER_WRITEVALUE_HELP ;

template <class ITEM>
trait TEXTWRITER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
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
	using MACRO_Binder = typename DEPENDENT<TEXTWRITER_HELP<ITEM ,ALWAYS> ,ARG1>::Binder ;

	class TextWriter {
	protected:
		CRef<Attribute> mAttr ;
		VRef<RegBuffer<ITEM>> mStream ;
		INDEX mRead ;
		INDEX mWrite ;
		INDEX mBackupRead ;
		INDEX mBackupWrite ;

	public:
		implicit TextWriter () = default ;

		explicit TextWriter (RREF<VRef<RegBuffer<ITEM>>> stream) {
			using R1X = typename TEXTWRITER_IMPLHOLDER_HELP<ITEM ,ALWAYS>::ImplHolder ;
			set_attr (TYPEAS<R1X>::expr) ;
			mStream = move (stream) ;
			reset () ;
			backup () ;
		}

		CRef<Attribute> get_attr () const {
			return mAttr ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			mAttr = CRef<ARG1>::make () ;
		}

		LENGTH size () const {
			return mStream->size () ;
		}

		LENGTH length () const {
			return mWrite ;
		}

		void reset () {
			mRead = 0 ;
			mWrite = 0 ;
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

		void prints () {
			noop () ;
		}

		template <class ARG1 ,class...ARG2>
		void prints (CREF<ARG1> item1 ,CREF<ARG2>...item2) {
			write (item1) ;
			prints (item2...) ;
		}

		void write (CREF<ITEM> item) {
			if ifswitch (TRUE) {
				if (mWrite >= mStream->size ())
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
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (item)
					discard ;
				write (mAttr->lower_upper_cast (ITEM ('t'))) ;
				write (mAttr->lower_upper_cast (ITEM ('r'))) ;
				write (mAttr->lower_upper_cast (ITEM ('u'))) ;
				write (mAttr->lower_upper_cast (ITEM ('e'))) ;
			}
			if ifswitch (rxx) {
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

		void write (CREF<csc_const_pointer_t>) = delete ;

		inline VREF<TextWriter> operator<< (CREF<csc_const_pointer_t>) = delete ;

		void write (CREF<VAL32> item) {
			write (VAL64 (item)) ;
		}

		inline VREF<TextWriter> operator<< (CREF<VAL32> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<VAL64> item) {
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
				write (mAttr->lower_upper_cast (ITEM ('a'))) ;
				write (mAttr->lower_upper_cast (ITEM ('b'))) ;
				write (mAttr->lower_upper_cast (ITEM ('s'))) ;
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
				auto rax = R1X (mAttr ,r1x) ;
				rax.write_value () ;
				write (rax) ;
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
			using R1X = typename TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS>::WriteValue ;
			if ifswitch (TRUE) {
				if (item > 0)
					discard ;
				write (ITEM ('-')) ;
			}
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (MathProc::infinite (item))
					discard ;
				write (mAttr->lower_upper_cast (ITEM ('i'))) ;
				write (mAttr->lower_upper_cast (ITEM ('n'))) ;
				write (mAttr->lower_upper_cast (ITEM ('f'))) ;
			}
			if ifswitch (rxx) {
				const auto r1x = FloatProc::decode (MathProc::abs (item)) ;
				const auto r2x = FloatProc::exp10_from_exp2 (r1x) ;
				auto rax = R1X (mAttr ,r2x) ;
				rax.write_float () ;
				write (rax) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<DOUBLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BYTE> item) {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<BYTE>::expr)) {
				INDEX ix = (SIZE_OF<BYTE>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & BYTE (0X0F)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & BYTE (0X0F)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<BYTE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<WORD> item) {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<WORD>::expr)) {
				INDEX ix = (SIZE_OF<WORD>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & WORD (0X0F)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & WORD (0X0F)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<CHAR>::expr)) {
				INDEX ix = (SIZE_OF<CHAR>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & CHAR (0X0F)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & CHAR (0X0F)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			write (ITEM ('0')) ;
			write (ITEM ('X')) ;
			for (auto &&i : iter (0 ,SIZE_OF<DATA>::expr)) {
				INDEX ix = (SIZE_OF<DATA>::expr - 1 - i) * 8 ;
				const auto r1x = INDEX ((item >> (ix + 4)) & DATA (0X0F)) ;
				write (mAttr->str_from_hex (r1x)) ;
				const auto r2x = INDEX ((item >> (ix + 0)) & DATA (0X0F)) ;
				write (mAttr->str_from_hex (r2x)) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<DATA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Slice<STR>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				vbetween (INDEX (item[i]) ,0 ,128) ;
				const auto r1x = ITEM (item[i]) ;
				write (r1x) ;
			}
		}

		inline VREF<TextWriter> operator<< (CREF<Slice<STR>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<ITEM>> item) {
			for (auto &&i : item)
				write (i) ;
		}

		inline VREF<TextWriter> operator<< (CREF<String<ITEM>> item) {
			write (item) ;
			return thiz ;
		}

		template <class ARG1 = DEPEND>
		void write (CREF<MACRO_Binder<ARG1>> item) {
			item.friend_write (thiz) ;
		}

		template <class ARG1 = DEPEND>
		inline VREF<TextWriter> operator<< (CREF<MACRO_Binder<ARG1>> item) {
			write (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void write (CREF<typeof (CLS)>) {
			reset () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		imports void BOM (CREF<PlaceHolder<RANK2>>) {}

		void write (CREF<typeof (BOM)>) {
			template_bom (PHX ,TYPEAS<ITEM>::expr) ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (BOM)> item) {
			write (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void write (CREF<typeof (GAP)> item) {
			write (ITEM ('\r')) ;
			write (ITEM ('\n')) ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void write (CREF<typeof (EOS)> item) {
			const auto r1x = mAttr->ending_item () ;
			write (r1x) ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}

	private:
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
	} ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<TextWriter> writer) const = 0 ;
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_IMPLHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using Attribute = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Attribute ;

	class ImplHolder implement Attribute {
	protected:
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit ImplHolder () {
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
			return bad (TYPEAS<ITEM>::expr) ;
		}

		Optional<ITEM> escape_cast (CREF<ITEM> str) const override {
			INDEX ix = mEscapeSet.map (str) ;
			if (ix == NONE)
				return FLAG (1) ;
			return Optional<ITEM>::make (ITEM (ix)) ;
		}

		LENGTH float_precision () const override {
			return 15 ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_WRITEVALUE_HELP<ITEM ,ALWAYS> {
	using Attribute = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Attribute ;
	using Binder = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Binder ;
	using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;
	using NUMBER_SIZE = ENUMAS<VAL ,ENUMID<64>> ;

	class WriteValue implement Binder {
	protected:
		CRef<Attribute> mAttr ;
		NOTATION mValue ;
		BoxBuffer<ITEM ,NUMBER_SIZE> mBuffer ;
		INDEX mWrite ;

	public:
		implicit WriteValue () = delete ;

		explicit WriteValue (CREF<CRef<Attribute>> attr_ ,CREF<NOTATION> value_) {
			assert (value_.mRadix == 10) ;
			mAttr = attr_ ;
			mValue = value_ ;
			mValue.mPrecision = 1 + MathProc::log10v (mValue.mMantissa) ;
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
					mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
			}
		}

		void write_float () {
			const auto r1x = mAttr->float_precision () ;
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
				mBuffer[mWrite] = mAttr->lower_upper_cast (ITEM ('e')) ;
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
					mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
					mWrite += LENGTH (mBuffer[ix] == ITEM ('0')) ;
					mValue.mMantissa /= 10 ;
					mValue.mExponent++ ;
					mValue.mPrecision-- ;
				}
				mWrite-- ;
				mBuffer[mWrite] = ITEM ('.') ;
				mWrite += LENGTH (mBuffer[ix] == ITEM ('.')) ;
				mWrite-- ;
				mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
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
					mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
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
					mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
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
					mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
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
					mBuffer[mWrite] = mAttr->str_from_hex (mValue.mMantissa % 10) ;
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

template <class ITEM>
using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

template <class ITEM>
trait STRING_TEXTWRITER_HELP<ITEM ,ALWAYS> {
	using TextWriter = CSC::TextWriter<ITEM> ;
} ;

template <class...>
trait REGULARREADER_HELP ;

template <class DEPEND>
trait REGULARREADER_HELP<DEPEND ,ALWAYS> {
	class RegularReader {
	protected:
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
				const auto r1x = mReader->poll (TYPEAS<STRU8>::expr) ;
				mCache.add (r1x) ;
			}
			mHintString = FALSE ;
			mHintSize = 0 ;
			backup () ;
		}

		CREF<STRU8> at (CREF<INDEX> index) const leftvalue {
			return mCache[index] ;
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
			const auto r1x = mReader->poll (TYPEAS<STRU8>::expr) ;
			mCache.add (r1x) ;
		}

		inline void operator++ (csc_int32_t) {
			read () ;
		}

		void read (CREF<Slice<STR>> item) {
			for (auto &&i : iter (0 ,item.size ())) {
				assert (vbetween (INDEX (item[i]) ,0 ,128)) ;
				assume (thiz[0] == STRU8 (item[i])) ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STR>> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_IDENTIFIER (CREF<PlaceHolder<RANK1>>) {}

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

		imports void HINT_SCALAR (CREF<PlaceHolder<RANK2>>) {}

		void read (CREF<typeof (HINT_SCALAR)>) {
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

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_SCALAR)>) {
			read (HINT_SCALAR) ;
			return thiz ;
		}

		imports void HINT_STRING (CREF<PlaceHolder<RANK3>>) {}

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
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					if ifnot (r2x == STRU8 ('\\'))
						discard ;
					thiz++ ;
					mHintSize++ ;
				}
				if ifswitch (rxx) {
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

		imports void HINT_WORD_SPACE (CREF<PlaceHolder<RANK4>>) {}

		void read (CREF<typeof (HINT_WORD_SPACE)>) {
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

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_SPACE)>) {
			read (HINT_WORD_SPACE) ;
			return thiz ;
		}

		imports void HINT_WORD_ENDLINE (CREF<PlaceHolder<RANK5>>) {}

		void read (CREF<typeof (HINT_WORD_ENDLINE)>) {
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

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_ENDLINE)>) {
			read (HINT_WORD_ENDLINE) ;
			return thiz ;
		}

		imports void SKIP_SPACE (CREF<PlaceHolder<RANK6>>) {}

		void read (CREF<typeof (SKIP_SPACE)>) {
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if ifnot (r1x->is_space (thiz[0]))
					break ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_SPACE)>) {
			read (SKIP_SPACE) ;
			return thiz ;
		}

		imports void SKIP_SPACE_WORD (CREF<PlaceHolder<RANK7>>) {}

		void read (CREF<typeof (SKIP_SPACE_WORD)>) {
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

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_SPACE_WORD)>) {
			read (SKIP_SPACE_WORD) ;
			return thiz ;
		}

		imports void SKIP_SPACE_ENDLINE (CREF<PlaceHolder<RANK8>>) {}

		void read (CREF<typeof (SKIP_SPACE_ENDLINE)>) {
			const auto r1x = mReader->get_attr () ;
			while (TRUE) {
				if (thiz[0] == r1x->ending_item ())
					break ;
				if ifnot (r1x->is_endline_space (thiz[0]))
					break ;
				thiz++ ;
			}
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_SPACE_ENDLINE)>) {
			read (SKIP_SPACE_ENDLINE) ;
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
				auto rxx = TRUE ;
				if ifswitch (rxx) {
					const auto r5x = BOOL (r4x == STRU8 ('\\')) ;
					if ifnot (r5x)
						discard ;
					item[i] = r1x->escape_cast (thiz[0]).poll () ;
					thiz++ ;
				}
				if ifswitch (rxx) {
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