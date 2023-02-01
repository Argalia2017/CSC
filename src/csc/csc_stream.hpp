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
static constexpr auto CLS = TYPEAS<PlaceHolder<RANK1>>::expr ;
static constexpr auto BOM = TYPEAS<PlaceHolder<RANK2>>::expr ;
static constexpr auto GAP = TYPEAS<PlaceHolder<RANK3>>::expr ;
static constexpr auto EOS = TYPEAS<PlaceHolder<RANK4>>::expr ;

template <class...>
trait BYTEATTRIBUTE_HELP ;

template <class...>
trait BYTEATTRIBUTE_PUREHOLDER_HELP ;

template <class...>
trait BYTEATTRIBUTE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void initialize (RREF<CRef<Holder>> prefix) = 0 ;
		virtual BYTE ending_item () const = 0 ;
		virtual BYTE space_item () const = 0 ;
		virtual BOOL is_big_endian () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class ByteAttribute {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit ByteAttribute () = default ;

		explicit ByteAttribute (CREF<typeof (PH0)>) {
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize () ;
			mThis = rax.as_cref () ;
		}

		template <class ARG1>
		void derive (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Holder ,ARG1>) ;
			auto rax = VRef<ARG1>::make () ;
			rax->initialize (move (mThis)) ;
			mThis = rax.as_cref () ;
		}

		BYTE ending_item () const {
			return mThis->ending_item () ;
		}

		BYTE space_item () const {
			return mThis->space_item () ;
		}

		BOOL is_big_endian () const {
			return mThis->is_big_endian () ;
		}
	} ;
} ;

template <class DEPEND>
trait BYTEATTRIBUTE_PUREHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		CRef<Holder> mPrefix ;

	public:
		void initialize () override {
			assert (FALSE) ;
		}
		
		void initialize (RREF<CRef<Holder>> prefix) override {
			mPrefix = move (prefix) ;
		}

		BYTE ending_item () const override {
			return mPrefix->ending_item () ;
		}

		BYTE space_item () const override {
			return mPrefix->space_item () ;
		}

		BOOL is_big_endian () const override {
			return mPrefix->is_big_endian () ;
		}
	} ;
} ;

using ByteAttribute = typename BYTEATTRIBUTE_HELP<DEPEND ,ALWAYS>::ByteAttribute ;

template <class...>
trait BYTEREADER_HELP ;

template <class...>
trait BYTEREADER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BYTEREADER_HELP<DEPEND ,ALWAYS> {
	class ByteReader ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<ByteReader> reader) = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<CRef<RegBuffer<BYTE>>> stream) = 0 ;
		virtual VREF<ByteAttribute> attribute () leftvalue = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual CREF<RegBuffer<BYTE>> raw (RREF<RegCaches<BYTE>> unnamed) const leftvalue = 0 ;
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

		explicit ByteReader (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		explicit ByteReader (RREF<CRef<RegBuffer<BYTE>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		explicit ByteReader (RREF<VRef<RegBuffer<BYTE>>> stream)
			:ByteReader (stream.as_cref ()) {}

		VREF<ByteAttribute> attribute () leftvalue {
			return mThis->attribute () ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		CREF<RegBuffer<BYTE>> raw (RREF<RegCaches<BYTE>> unnamed = RegCaches<BYTE> ()) const leftvalue {
			return mThis->raw (move (unnamed)) ;
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
			return item.friend_read (thiz) ;
		}

		inline VREF<ByteReader> operator>> (VREF<Binder> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (CLS)> item) {
			return mThis->read_cls () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (GAP)> item) {
			return mThis->read_gap () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (EOS)> item) {
			return mThis->read_eos () ;
		}

		inline VREF<ByteReader> operator>> (CREF<typeof (EOS)> item) {
			read (item) ;
			return thiz ;
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
	class ByteWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<ByteWriter> writer) const = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<RegBuffer<BYTE>>> stream) = 0 ;
		virtual VREF<ByteAttribute> attribute () leftvalue = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual CREF<RegBuffer<BYTE>> raw (RREF<RegCaches<BYTE>> unnamed) const leftvalue = 0 ;
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

		explicit ByteWriter (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		explicit ByteWriter (RREF<VRef<RegBuffer<BYTE>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		VREF<ByteAttribute> attribute () leftvalue {
			return mThis->attribute () ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		CREF<RegBuffer<BYTE>> raw (RREF<RegCaches<BYTE>> unnamed = RegCaches<BYTE> ()) const leftvalue {
			return mThis->raw (move (unnamed)) ;
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
			return item.friend_write (thiz) ;
		}

		inline VREF<ByteWriter> operator<< (CREF<Binder> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<typeof (CLS)>) {
			return mThis->write_cls () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<typeof (GAP)> item) {
			return mThis->write_gap () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<typeof (EOS)> item) {
			return mThis->write_eos () ;
		}

		inline VREF<ByteWriter> operator<< (CREF<typeof (EOS)> item) {
			write (item) ;
			return thiz ;
		}
	} ;
} ;

using ByteWriter = typename BYTEWRITER_HELP<DEPEND ,ALWAYS>::ByteWriter ;

template <class...>
trait TEXTATTRIBUTE_HELP ;

template <class...>
trait TEXTATTRIBUTE_PUREHOLDER_HELP ;

template <class...>
trait TEXTATTRIBUTE_IMPLHOLDER_HELP ;

template <class ITEM>
trait TEXTATTRIBUTE_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void initialize (RREF<CRef<Holder>> prefix) = 0 ;
		virtual ITEM ending_item () const = 0 ;
		virtual BOOL is_gap (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_gap_space (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_gap_endline (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_word (CREF<ITEM> str) const = 0 ;
		virtual ITEM word_lower_cast (CREF<ITEM> str) const = 0 ;
		virtual ITEM word_upper_cast (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_number (CREF<ITEM> str) const = 0 ;
		virtual BOOL is_hex_number (CREF<ITEM> str) const = 0 ;
		virtual INDEX hex_from_str (CREF<ITEM> str) const = 0 ;
		virtual ITEM str_from_hex (CREF<INDEX> hex) const = 0 ;
		virtual BOOL is_control (CREF<ITEM> str) const = 0 ;
		virtual Optional<ITEM> escape_word_cast (CREF<ITEM> str) const = 0 ;
		virtual Optional<ITEM> escape_ctrl_cast (CREF<ITEM> str) const = 0 ;
		virtual LENGTH value_precision () const = 0 ;
		virtual LENGTH float_precision () const = 0 ;
		virtual LENGTH number_precision () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class TextAttribute {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit TextAttribute () = default ;

		explicit TextAttribute (CREF<typeof (PH0)>) {
			auto rax = FUNCTION_extern::invoke () ;
			rax->initialize () ;
			mThis = rax.as_cref () ;
		}

		template <class ARG1>
		void derive (CREF<TYPEID<ARG1>> id) {
			require (IS_EXTEND<Holder ,ARG1>) ;
			auto rax = VRef<ARG1>::make () ;
			rax->initialize (move (mThis)) ;
			mThis = rax.as_cref () ;
		}

		ITEM ending_item () const {
			return mThis->ending_item () ;
		}

		BOOL is_gap (CREF<ITEM> str) const {
			return mThis->is_gap (str) ;
		}

		BOOL is_gap_space (CREF<ITEM> str) const {
			return mThis->is_gap_space (str) ;
		}

		BOOL is_gap_endline (CREF<ITEM> str) const {
			return mThis->is_gap_endline (str) ;
		}

		BOOL is_word (CREF<ITEM> str) const {
			return mThis->is_word (str) ;
		}

		ITEM word_lower_cast (CREF<ITEM> str) const {
			return mThis->word_lower_cast (str) ;
		}

		ITEM word_upper_cast (CREF<ITEM> str) const {
			return mThis->word_upper_cast (str) ;
		}

		BOOL is_number (CREF<ITEM> str) const {
			return mThis->is_number (str) ;
		}

		BOOL is_hex_number (CREF<ITEM> str) const {
			return mThis->is_hex_number (str) ;
		}

		INDEX hex_from_str (CREF<ITEM> str) const {
			return mThis->hex_from_str (str) ;
		}

		ITEM str_from_hex (CREF<INDEX> hex) const {
			return mThis->str_from_hex (hex) ;
		}

		BOOL is_control (CREF<ITEM> str) const {
			return mThis->is_control (str) ;
		}

		Optional<ITEM> escape_word_cast (CREF<ITEM> str) const {
			return mThis->escape_word_cast (str) ;
		}

		Optional<ITEM> escape_ctrl_cast (CREF<ITEM> str) const {
			return mThis->escape_ctrl_cast (str) ;
		}

		LENGTH value_precision () const {
			return mThis->value_precision () ;
		}

		LENGTH float_precision () const {
			return mThis->float_precision () ;
		}

		LENGTH number_precision () const {
			return mThis->number_precision () ;
		}
	} ;
} ;

template <class ITEM>
trait TEXTATTRIBUTE_PUREHOLDER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	using Holder = typename TEXTATTRIBUTE_HELP<ITEM ,ALWAYS>::Holder ;

	class PureHolder implement Holder {
	protected:
		CRef<Holder> mPrefix ;

	public:
		void initialize () override {
			assert (FALSE) ;
		}

		void initialize (RREF<CRef<Holder>> prefix) override {
			mPrefix = move (prefix) ;
		}

		ITEM ending_item () const override {
			return mPrefix->ending_item () ;
		}

		BOOL is_gap (CREF<ITEM> str) const override {
			return mPrefix->is_gap (str) ;
		}

		BOOL is_gap_space (CREF<ITEM> str) const override {
			return mPrefix->is_gap_space (str) ;
		}

		BOOL is_gap_endline (CREF<ITEM> str) const override {
			return mPrefix->is_gap_endline (str) ;
		}

		BOOL is_word (CREF<ITEM> str) const override {
			return mPrefix->is_word (str) ;
		}

		ITEM word_lower_cast (CREF<ITEM> str) const override {
			return mPrefix->word_lower_cast (str) ;
		}

		ITEM word_upper_cast (CREF<ITEM> str) const override {
			return mPrefix->word_upper_cast (str) ;
		}

		BOOL is_number (CREF<ITEM> str) const override {
			return mPrefix->is_number (str) ;
		}

		BOOL is_hex_number (CREF<ITEM> str) const override {
			return mPrefix->is_hex_number (str) ;
		}

		INDEX hex_from_str (CREF<ITEM> str) const override {
			return mPrefix->hex_from_str (str) ;
		}

		ITEM str_from_hex (CREF<INDEX> hex) const override {
			return mPrefix->str_from_hex (hex) ;
		}

		BOOL is_control (CREF<ITEM> str) const override {
			return mPrefix->is_control (str) ;
		}

		Optional<ITEM> escape_word_cast (CREF<ITEM> str) const override {
			return mPrefix->escape_word_cast (str) ;
		}

		Optional<ITEM> escape_ctrl_cast (CREF<ITEM> str) const override {
			return mPrefix->escape_ctrl_cast (str) ;
		}

		LENGTH value_precision () const override {
			return mPrefix->value_precision () ;
		}

		LENGTH float_precision () const override {
			return mPrefix->float_precision () ;
		}

		LENGTH number_precision () const override {
			return mPrefix->number_precision () ;
		}
	} ;
} ;

template <class ITEM>
using TextAttribute = typename TEXTATTRIBUTE_HELP<ITEM ,ALWAYS>::TextAttribute ;

template <class...>
trait TEXTREADER_HELP ;

template <class...>
trait TEXTREADER_IMPLHOLDER_HELP ;

template <class ITEM>
trait TEXTREADER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	class TextReader ;

	using NOTATION = typename FLOATPROC_HELP<DEPEND ,ALWAYS>::NOTATION ;

	struct Binder implement Interface {
		virtual void friend_read (VREF<TextReader> reader) = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<CRef<RegBuffer<ITEM>>> stream) = 0 ;
		virtual VREF<TextAttribute<ITEM>> attribute () leftvalue = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed) const leftvalue = 0 ;
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

		explicit TextReader (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		explicit TextReader (RREF<CRef<RegBuffer<ITEM>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		explicit TextReader (RREF<VRef<RegBuffer<ITEM>>> stream)
			:TextReader (stream.as_cref ()) {}

		VREF<TextAttribute<ITEM>> attribute () leftvalue {
			return mThis->attribute () ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return mThis->raw (move (unnamed)) ;
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
			return item.friend_read (thiz) ;
		}

		inline VREF<TextReader> operator>> (VREF<Binder> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (CLS)>) {
			return mThis->read_cls () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (CLS)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (BOM)>) {
			return mThis->read_bom () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (BOM)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (GAP)> item) {
			return mThis->read_gap () ;
		}

		inline VREF<TextReader> operator>> (CREF<typeof (GAP)> item) {
			read (item) ;
			return thiz ;
		}

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
using TextReader = typename TEXTREADER_HELP<ITEM ,ALWAYS>::TextReader ;

template <class...>
trait TEXTWRITER_HELP ;

template <class...>
trait TEXTWRITER_IMPLHOLDER_HELP ;

template <class ITEM>
trait TEXTWRITER_HELP<ITEM ,REQUIRE<IS_TEXT<ITEM>>> {
	class TextWriter ;

	struct Binder implement Interface {
		virtual void friend_write (VREF<TextWriter> writer) const = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<RegBuffer<ITEM>>> stream) = 0 ;
		virtual VREF<TextAttribute<ITEM>> attribute () leftvalue = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed) const leftvalue = 0 ;
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

		explicit TextWriter (RREF<VRef<Holder>> that) {
			mThis = move (that) ;
		}

		explicit TextWriter (RREF<VRef<RegBuffer<ITEM>>> stream) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (move (stream)) ;
		}

		VREF<TextAttribute<ITEM>> attribute () leftvalue {
			return mThis->attribute () ;
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		LENGTH length () const {
			return mThis->length () ;
		}

		CREF<RegBuffer<ITEM>> raw (RREF<RegCaches<ITEM>> unnamed = RegCaches<ITEM> ()) const leftvalue {
			return mThis->raw (move (unnamed)) ;
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

		inline VREF<ByteWriter> operator<< (CREF<csc_const_pointer_t>) = delete ;

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
			return item.friend_write (thiz) ;
		}

		inline VREF<TextWriter> operator<< (CREF<Binder> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<typeof (CLS)>) {
			return mThis->write_cls () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (CLS)> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<typeof (BOM)>) {
			return mThis->write_bom () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (BOM)> item) {
			write (item) ;
			return thiz ;
		}

		void write (CREF<typeof (GAP)> item) {
			return mThis->write_gap () ;
		}

		inline VREF<TextWriter> operator<< (CREF<typeof (GAP)> item) {
			write (item) ;
			return thiz ;
		}

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
using TextWriter = typename TEXTWRITER_HELP<ITEM ,ALWAYS>::TextWriter ;

template <class ITEM>
trait STRING_TEXTWRITER_HELP<ITEM ,ALWAYS> {
	using TextWriter = CSC::TextWriter<ITEM> ;

	static constexpr auto EOS = CSC::EOS ;
} ;

static constexpr auto HINT_IDENTIFIER = TYPEAS<PlaceHolder<ENUMAS<VAL ,10>>>::expr ;
static constexpr auto HINT_SCALAR = TYPEAS<PlaceHolder<ENUMAS<VAL ,11>>>::expr ;
static constexpr auto HINT_STRING = TYPEAS<PlaceHolder<ENUMAS<VAL ,12>>>::expr ;
static constexpr auto HINT_WORD_ENDLINE = TYPEAS<PlaceHolder<ENUMAS<VAL ,13>>>::expr ;
static constexpr auto SKIP_GAP = TYPEAS<PlaceHolder<ENUMAS<VAL ,20>>>::expr ;
static constexpr auto SKIP_GAP_SPACE = TYPEAS<PlaceHolder<ENUMAS<VAL ,21>>>::expr ;
static constexpr auto SKIP_GAP_ENDLINE = TYPEAS<PlaceHolder<ENUMAS<VAL ,22>>>::expr ;

template <class...>
trait REGULARREADER_HELP ;

template <class...>
trait REGULARREADER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait REGULARREADER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (RREF<VRef<TextReader<STRU8>>> reader ,CREF<LENGTH> ll_size) = 0 ;
		virtual CREF<STRU8> at (CREF<INDEX> index) const leftvalue = 0 ;
		virtual void read () = 0 ;
		virtual void read (CREF<Slice<STR>> item) = 0 ;
		virtual void read_hint_identifer () = 0 ;
		virtual void read_hint_scalar () = 0 ;
		virtual void read_hint_string () = 0 ;
		virtual void read_hint_word_endline () = 0 ;
		virtual void read_skip_gap () = 0 ;
		virtual void read_skip_gap_space () = 0 ;
		virtual void read_skip_gap_endline () = 0 ;
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

		void read () {
			return mThis->read () ;
		}

		inline void operator++ (VAL32) {
			read () ;
		}

		void read (CREF<Slice<STR>> item) {
			return mThis->read (item) ;
		}

		inline VREF<RegularReader> operator>> (CREF<Slice<STR>> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (HINT_IDENTIFIER)>) {
			return mThis->read_hint_identifer () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_IDENTIFIER)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (HINT_SCALAR)>) {
			return mThis->read_hint_scalar () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_SCALAR)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (HINT_STRING)>) {
			return mThis->read_hint_string () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_STRING)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (HINT_WORD_ENDLINE)>) {
			return mThis->read_hint_word_endline () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (HINT_WORD_ENDLINE)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (SKIP_GAP)>) {
			return mThis->read_skip_gap () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_GAP)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (SKIP_GAP_SPACE)>) {
			return mThis->read_skip_gap_space () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_GAP_SPACE)> item) {
			read (item) ;
			return thiz ;
		}

		void read (CREF<typeof (SKIP_GAP_ENDLINE)>) {
			return mThis->read_skip_gap_endline () ;
		}

		inline VREF<RegularReader> operator>> (CREF<typeof (SKIP_GAP_ENDLINE)> item) {
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