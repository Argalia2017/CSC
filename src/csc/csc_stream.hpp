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

template <class...>
trait BYTEREADER_ATTRIBUTE_HELP ;

template <class DEPEND>
trait BYTEREADER_HELP<DEPEND ,ALWAYS> {
	struct Attribute implement Interface {
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	template <class ARG1>
	using CRTP_ByteReader = typename DEPENDENT<BYTEREADER_HELP<DEPEND ,ALWAYS> ,ARG1>::ByteReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<CRTP_ByteReader<DEPEND>> reader) = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<CRef<RegBuffer<BYTE>>> stream) = 0 ;
		virtual CRef<Attribute> get_attr () const = 0 ;
		virtual void set_attr (RREF<CRef<Attribute>> attr_) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual void reset () = 0 ;
		virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
		virtual void backup () = 0 ;
		virtual void recover () = 0 ;
		virtual void read (VREF<BYTE> item) = 0 ;
		virtual void read (VREF<WORD> item) = 0 ;
		virtual void read (VREF<CHAR> item) = 0 ;
		virtual void read (VREF<DATA> item) = 0 ;
		virtual void read (VREF<BOOL> item) = 0 ;
		virtual void read (VREF<VAL32> item) = 0 ;
		virtual void read (VREF<VAL64> item) = 0 ;
		virtual void read (VREF<SINGLE> item) = 0 ;
		virtual void read (VREF<DOUBLE> item) = 0 ;
		virtual void read (CREF<Slice<STR>> item) = 0 ;
		virtual void read (VREF<String<STRA>> item) = 0 ;
		virtual void read (VREF<String<STRW>> item) = 0 ;
		virtual void read (VREF<String<STRU8>> item) = 0 ;
		virtual void read (VREF<String<STRU16>> item) = 0 ;
		virtual void read (VREF<String<STRU32>> item) = 0 ;
		virtual void read (VREF<Binder> item) = 0 ;
		virtual void read_cls () = 0 ;
		virtual void read_gap () = 0 ;
		virtual void read_eos () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class ByteReader {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit ByteReader () = default ;

		explicit ByteReader (RREF<CRef<RegBuffer<BYTE>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		explicit ByteReader (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		CRef<Attribute> get_attr () const {
			return mThis->get_attr () ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			auto &&tmp = memorize ([&] () {
				return CRef<ARG1>::make () ;
			}) ;
			return mThis->set_attr (copy (tmp)) ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		void reset () {
			return mThis->reset () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			return mThis->reset (read_ ,write_) ;
		}

		void backup () {
			return mThis->backup () ;
		}

		void recover () {
			return mThis->recover () ;
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
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<DATA> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BOOL> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STR>> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (CREF<Slice<STR>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRA>> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<String<STRA>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRW>> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<String<STRW>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU8>> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<String<STRU8>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU16>> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<String<STRU16>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU32>> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<String<STRU32>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<Binder> item) {
			return mThis->read (item) ;
		}

		inline VREF<ByteReader> operator>> (VREF<Binder> item) {
			read (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void read (CREF<typeof (CLS)> item) {
			return mThis->read_cls () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void read (CREF<typeof (GAP)> item) {
			return mThis->read_gap () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void read (CREF<typeof (EOS)> item) {
			return mThis->read_eos () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (EOS)> item) {
			read (item) ;
			return thiz ;
		}
	} ;
} ;

template <class DEPEND>
trait BYTEREADER_ATTRIBUTE_HELP<DEPEND ,ALWAYS> {
	using SUPER = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::Attribute ;

	class Attribute implement SUPER {
	public:
		implicit Attribute () = default ;

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

using ByteReader = typename BYTEREADER_HELP<DEPEND ,ALWAYS>::ByteReader ;

template <class...>
trait BYTEWRITER_HELP ;

template <class...>
trait BYTEWRITER_IMPLHOLDER_HELP ;

template <class...>
trait BYTEWRITER_ATTRIBUTE_HELP ;

template <class DEPEND>
trait BYTEWRITER_HELP<DEPEND ,ALWAYS> {
	struct Attribute implement Interface {
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	template <class ARG1>
	using CRTP_ByteWriter = typename DEPENDENT<BYTEWRITER_HELP<DEPEND ,ALWAYS> ,ARG1>::ByteWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<CRTP_ByteWriter<DEPEND>> writer) const = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<RegBuffer<BYTE>>> stream) = 0 ;
		virtual CRef<Attribute> get_attr () const = 0 ;
		virtual void set_attr (RREF<CRef<Attribute>> attr_) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual void reset () = 0 ;
		virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
		virtual void backup () = 0 ;
		virtual void recover () = 0 ;
		virtual void write (CREF<BYTE> item) = 0 ;
		virtual void write (CREF<WORD> item) = 0 ;
		virtual void write (CREF<CHAR> item) = 0 ;
		virtual void write (CREF<DATA> item) = 0 ;
		virtual void write (CREF<BOOL> item) = 0 ;
		virtual void write (CREF<VAL32> item) = 0 ;
		virtual void write (CREF<VAL64> item) = 0 ;
		virtual void write (CREF<SINGLE> item) = 0 ;
		virtual void write (CREF<DOUBLE> item) = 0 ;
		virtual void write (CREF<Slice<STR>> item) = 0 ;
		virtual void write (CREF<String<STRA>> item) = 0 ;
		virtual void write (CREF<String<STRW>> item) = 0 ;
		virtual void write (CREF<String<STRU8>> item) = 0 ;
		virtual void write (CREF<String<STRU16>> item) = 0 ;
		virtual void write (CREF<String<STRU32>> item) = 0 ;
		virtual void write (CREF<Binder> item) = 0 ;
		virtual void write_cls () = 0 ;
		virtual void write_gap () = 0 ;
		virtual void write_eos () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class ByteWriter {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit ByteWriter () = default ;

		explicit ByteWriter (RREF<VRef<RegBuffer<BYTE>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		explicit ByteWriter (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		CRef<Attribute> get_attr () const {
			return mThis->get_attr () ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			auto &&tmp = memorize ([&] () {
				return CRef<ARG1>::make () ;
			}) ;
			return mThis->set_attr (copy (tmp)) ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		void reset () {
			return mThis->reset () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			return mThis->reset (read_ ,write_) ;
		}

		void backup () {
			return mThis->backup () ;
		}

		void recover () {
			return mThis->recover () ;
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
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<BYTE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<WORD> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<DATA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BOOL> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<BOOL> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<csc_const_pointer_t>) = delete ;

		inline VREF<ByteWriter> operator<< (CREF<csc_const_pointer_t>) = delete ;

		void write (CREF<VAL32> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<VAL32> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<VAL64> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<VAL64> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<SINGLE> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<SINGLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DOUBLE> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<DOUBLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Slice<STR>> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<Slice<STR>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRA>> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<String<STRA>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRW>> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<String<STRW>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRU8>> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<String<STRU8>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRU16>> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<String<STRU16>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<STRU32>> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<String<STRU32>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Binder> item) {
			return mThis->write (item) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<Binder> item) {
			write (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void write (CREF<typeof (CLS)>) {
			return mThis->write_cls () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void write (CREF<typeof (GAP)> item) {
			return mThis->write_gap () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void write (CREF<typeof (EOS)> item) {
			return mThis->write_eos () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}
	} ;
} ;

template <class DEPEND>
trait BYTEWRITER_ATTRIBUTE_HELP<DEPEND ,ALWAYS> {
	using SUPER = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::Attribute ;

	class Attribute implement SUPER {
	public:
		implicit Attribute () = default ;

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

using ByteWriter = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::ByteWriter ;

template <class...>
trait TEXTREADER_HELP ;

template <class...>
trait TEXTREADER_IMPLHOLDER_HELP ;

template <class...>
trait TEXTREADER_ATTRIBUTE_HELP ;

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
	using CRTP_TextReader = typename DEPENDENT<TEXTREADER_HELP<ITEM ,ALWAYS> ,ARG1>::TextReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<CRTP_TextReader<DEPEND>> reader) = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<CRef<RegBuffer<ITEM>>> stream) = 0 ;
		virtual CRef<Attribute> get_attr () const = 0 ;
		virtual void set_attr (RREF<CRef<Attribute>> attr_) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual void reset () = 0 ;
		virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
		virtual void backup () = 0 ;
		virtual void recover () = 0 ;
		virtual void read (VREF<ITEM> item) = 0 ;
		virtual void read (VREF<BOOL> item) = 0 ;
		virtual void read (VREF<VAL32> item) = 0 ;
		virtual void read (VREF<VAL64> item) = 0 ;
		virtual void read (VREF<SINGLE> item) = 0 ;
		virtual void read (VREF<DOUBLE> item) = 0 ;
		virtual void read (VREF<BYTE> item) = 0 ;
		virtual void read (VREF<WORD> item) = 0 ;
		virtual void read (VREF<CHAR> item) = 0 ;
		virtual void read (VREF<DATA> item) = 0 ;
		virtual void read (CREF<Slice<STR>> item) = 0 ;
		virtual void read (VREF<String<ITEM>> item) = 0 ;
		virtual void read (VREF<Binder> item) = 0 ;
		virtual void read_cls () = 0 ;
		virtual void read_bom () = 0 ;
		virtual void read_gap () = 0 ;
		virtual void read_eos () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class TextReader {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit TextReader () = default ;

		explicit TextReader (RREF<CRef<RegBuffer<ITEM>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		explicit TextReader (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		CRef<Attribute> get_attr () const {
			return mThis->get_attr () ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			auto &&tmp = memorize ([&] () {
				return CRef<ARG1>::make () ;
			}) ;
			return mThis->set_attr (copy (tmp)) ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		void reset () {
			return mThis->reset () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			return mThis->reset (read_ ,write_) ;
		}

		void backup () {
			return mThis->backup () ;
		}

		void recover () {
			return mThis->recover () ;
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
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<ITEM> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BOOL> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<BOOL> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL32> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<VAL32> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<VAL64> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<VAL64> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<SINGLE> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<SINGLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DOUBLE> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<DOUBLE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<BYTE> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<BYTE> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<WORD> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<WORD> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<CHAR> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<CHAR> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<DATA> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<DATA> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<Slice<STR>> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (CREF<Slice<STR>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<ITEM>> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<String<ITEM>> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<Binder> item) {
			return mThis->read (item) ;
		}

		inline VREF<TextReader> operator>> (VREF<Binder> item) {
			read (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void read (CREF<typeof (CLS)>) {
			return mThis->read_cls () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		imports void BOM (CREF<PlaceHolder<RANK2>>) {}

		void read (CREF<typeof (BOM)>) {
			return mThis->read_bom () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (BOM)> item) {
			read (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void read (CREF<typeof (GAP)> item) {
			return mThis->read_gap () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void read (CREF<typeof (EOS)> item) {
			return mThis->read_eos () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (EOS)> item) {
			read (item) ;
			return thiz ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTREADER_ATTRIBUTE_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using SUPER = typename TEXTREADER_HELP<ITEM ,ALWAYS>::Attribute ;

	using SPACE_CLAZZ_SPACE = RANK1 ;
	using SPACE_CLAZZ_ENDLINE = RANK2 ;

	class Attribute implement SUPER {
	protected:
		HashSet<ITEM> mSpaceSet ;
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit Attribute () {
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
trait TEXTWRITER_ATTRIBUTE_HELP ;

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

	template <class ARG1>
	using CRTP_TextWriter = typename DEPENDENT<TEXTWRITER_HELP<ITEM ,ALWAYS> ,ARG1>::TextWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<CRTP_TextWriter<DEPEND>> writer) const = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<RegBuffer<ITEM>>> stream) = 0 ;
		virtual CRef<Attribute> get_attr () const = 0 ;
		virtual void set_attr (RREF<CRef<Attribute>> attr_) = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual void reset () = 0 ;
		virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
		virtual void backup () = 0 ;
		virtual void recover () = 0 ;
		virtual void write (CREF<ITEM> item) = 0 ;
		virtual void write (CREF<BOOL> item) = 0 ;
		virtual void write (CREF<VAL32> item) = 0 ;
		virtual void write (CREF<VAL64> item) = 0 ;
		virtual void write (CREF<SINGLE> item) = 0 ;
		virtual void write (CREF<DOUBLE> item) = 0 ;
		virtual void write (CREF<BYTE> item) = 0 ;
		virtual void write (CREF<WORD> item) = 0 ;
		virtual void write (CREF<CHAR> item) = 0 ;
		virtual void write (CREF<DATA> item) = 0 ;
		virtual void write (CREF<Slice<STR>> item) = 0 ;
		virtual void write (CREF<String<ITEM>> item) = 0 ;
		virtual void write (CREF<Binder> item) = 0 ;
		virtual void write_cls () = 0 ;
		virtual void write_bom () = 0 ;
		virtual void write_gap () = 0 ;
		virtual void write_eos () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class TextWriter {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit TextWriter () = default ;

		explicit TextWriter (RREF<VRef<RegBuffer<ITEM>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		explicit TextWriter (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		CRef<Attribute> get_attr () const {
			return mThis->get_attr () ;
		}

		template <class ARG1>
		void set_attr (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Attribute ,ARG1>) ;
			auto &&tmp = memorize ([&] () {
				return CRef<ARG1>::make () ;
			}) ;
			return mThis->set_attr (copy (tmp)) ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		void reset () {
			return mThis->reset () ;
		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) {
			return mThis->reset (read_ ,write_) ;
		}

		void backup () {
			return mThis->backup () ;
		}

		void recover () {
			return mThis->recover () ;
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
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<ITEM> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BOOL> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<BOOL> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<csc_const_pointer_t>) = delete ;

		inline VREF<TextWriter> operator<< (CREF<csc_const_pointer_t>) = delete ;

		void write (CREF<VAL32> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<VAL32> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<VAL64> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<VAL64> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<SINGLE> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<SINGLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DOUBLE> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<DOUBLE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<BYTE> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<BYTE> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<WORD> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<WORD> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<CHAR> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<CHAR> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<DATA> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<DATA> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Slice<STR>> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<Slice<STR>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<String<ITEM>> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<String<ITEM>> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<Binder> item) {
			return mThis->write (item) ;
		}

		inline VREF<TextWriter> operator<< (CREF<Binder> item) {
			write (item) ;
			return thiz ;
		}

		imports void CLS (CREF<PlaceHolder<RANK1>>) {}

		void write (CREF<typeof (CLS)>) {
			return mThis->write_cls () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		imports void BOM (CREF<PlaceHolder<RANK2>>) {}

		void write (CREF<typeof (BOM)>) {
			return mThis->write_bom () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (BOM)> item) {
			write (item) ;
			return thiz ;
		}

		imports void GAP (CREF<PlaceHolder<RANK3>>) {}

		void write (CREF<typeof (GAP)> item) {
			return mThis->write_gap () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		imports void EOS (CREF<PlaceHolder<RANK4>>) {}

		void write (CREF<typeof (EOS)> item) {
			return mThis->write_eos () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTWRITER_ATTRIBUTE_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using SUPER = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::Attribute ;

	class Attribute implement SUPER {
	protected:
		HashSet<ITEM> mEscapeSet ;

	public:
		implicit Attribute () {
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
using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

template <class ITEM>
trait STRING_TEXTWRITER_HELP<ITEM ,ALWAYS> {
	using TextWriter = CSC::TextWriter<ITEM> ;
} ;

template <class...>
trait REGULARREADER_HELP ;

template <class...>
trait REGULARREADER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait REGULARREADER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<TextReader<STRU8>>> reader ,CREF<LENGTH> ll_size) = 0 ;
		virtual CREF<STRU8> at (CREF<INDEX> index) const leftvalue = 0 ;
		virtual void backup () = 0 ;
		virtual void recover () = 0 ;
		virtual void read () = 0 ;
		virtual void read (CREF<Slice<STR>> item) = 0 ;
		virtual void read_hint_identifer () = 0 ;
		virtual void read_hint_scalar () = 0 ;
		virtual void read_hint_string () = 0 ;
		virtual void read_hint_word_space () = 0 ;
		virtual void read_hint_word_endline () = 0 ;
		virtual void read_skip_space () = 0 ;
		virtual void read_skip_space_only () = 0 ;
		virtual void read_skip_space_endline () = 0 ;
		virtual void read (VREF<String<STRU8>> item) = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class RegularReader {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit RegularReader () = default ;

		explicit RegularReader (RREF<VRef<TextReader<STRU8>>> reader ,CREF<LENGTH> ll_size) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (reader) ,ll_size) ;
		}

		CREF<STRU8> at (CREF<INDEX> index) const leftvalue {
			return mThis->at (index) ;
		}

		inline CREF<STRU8> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void backup () {
			return mThis->backup () ;
		}

		void recover () {
			return mThis->recover () ;
		}

		void read () {
			return mThis->read () ;
		}

		inline void operator++ (csc_int32_t) {
			read () ;
		}

		void read (CREF<Slice<STR>> item) {
			return mThis->read (item) ;
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STR>> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_IDENTIFIER (CREF<PlaceHolder<RANK1>>) {}

		void read (CREF<typeof (HINT_IDENTIFIER)>) {
			return mThis->read_hint_identifer () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_IDENTIFIER)> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_SCALAR (CREF<PlaceHolder<RANK2>>) {}

		void read (CREF<typeof (HINT_SCALAR)>) {
			return mThis->read_hint_scalar () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_SCALAR)> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_STRING (CREF<PlaceHolder<RANK3>>) {}

		void read (CREF<typeof (HINT_STRING)>) {
			return mThis->read_hint_string () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_STRING)> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_WORD_SPACE (CREF<PlaceHolder<RANK4>>) {}

		void read (CREF<typeof (HINT_WORD_SPACE)>) {
			return mThis->read_hint_word_space () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_SPACE)> item) {
			read (item) ;
			return thiz ;
		}

		imports void HINT_WORD_ENDLINE (CREF<PlaceHolder<RANK5>>) {}

		void read (CREF<typeof (HINT_WORD_ENDLINE)>) {
			return mThis->read_hint_word_endline () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_ENDLINE)> item) {
			read (item) ;
			return thiz ;
		}

		imports void SKIP_SPACE (CREF<PlaceHolder<RANK6>>) {}

		void read (CREF<typeof (SKIP_SPACE)>) {
			return mThis->read_skip_space () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_SPACE)> item) {
			read (item) ;
			return thiz ;
		}

		imports void SKIP_SPACE_ONLY (CREF<PlaceHolder<RANK7>>) {}

		void read (CREF<typeof (SKIP_SPACE_ONLY)>) {
			return mThis->read_skip_space_only () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_SPACE_ONLY)> item) {
			read (item) ;
			return thiz ;
		}

		imports void SKIP_SPACE_ENDLINE (CREF<PlaceHolder<RANK8>>) {}

		void read (CREF<typeof (SKIP_SPACE_ENDLINE)>) {
			return mThis->read_skip_space_endline () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_SPACE_ENDLINE)> item) {
			read (item) ;
			return thiz ;
		}

		void read (VREF<String<STRU8>> item) {
			return mThis->read (item) ;
		}

		inline VREF<RegularReader> operator>> (VREF<String<STRU8>> item) {
			read (item) ;
			return thiz ;
		}
	} ;
} ;

using RegularReader = typename REGULARREADER_HELP<DEPEND ,ALWAYS>::RegularReader ;
} ;