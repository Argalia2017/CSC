#pragma once

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
template <class...>
trait FILE_HELP ;

template <class...>
trait FILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait FILE_HELP<DEPEND ,ALWAYS> {
	using RETRY_TIMES = RANK2 ;

	struct Holder implement Interface {
		virtual void initialize (CREF<String<STR>> file_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual VAL64 length () const = 0 ;
		virtual VarBuffer<BYTE> load () const = 0 ;
		virtual void load (VREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual void save (CREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual CRef<RegBuffer<BYTE>> load_asset () const = 0 ;
		virtual BOOL available () const = 0 ;
		virtual void erase () const = 0 ;
		virtual void copy_from (CREF<Holder> that) const = 0 ;
		virtual void move_from (CREF<Holder> that) const = 0 ;
		virtual void link_from (CREF<Holder> that) const = 0 ;
		virtual BOOL identical (CREF<Holder> that) const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class File {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit File () = default ;

		explicit File (CREF<String<STR>> file_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (file_) ;
		}

		Auto native () const leftvalue {
			return mThis->native () ;
		}

		VarBuffer<BYTE> load () const {
			return mThis->load () ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const {
			return mThis->load (item) ;
		}

		void load (VREF<VarBuffer<BYTE>> item) const {
			return load (RegBuffer<BYTE>::from (item)) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const {
			return mThis->save (item) ;
		}

		void save (CREF<VarBuffer<BYTE>> item) const {
			return save (RegBuffer<BYTE>::from (item)) ;
		}

		void save (CREF<ConBuffer<BYTE>> item) const {
			return save (RegBuffer<BYTE>::from (item)) ;
		}

		CRef<RegBuffer<BYTE>> load_asset () const {
			return mThis->load_asset () ;
		}

		BOOL available () const {
			return mThis->available () ;
		}

		VAL64 length () const {
			return mThis->length () ;
		}

		void erase () const {
			return mThis->erase () ;
		}

		void copy_from (CREF<File> that) const {
			return mThis->copy_from (that.mThis) ;
		}

		void move_from (CREF<File> that) const {
			return mThis->move_from (that.mThis) ;
		}

		void link_from (CREF<File> that) const {
			return mThis->link_from (that.mThis) ;
		}

		BOOL identical (CREF<File> that) const {
			return mThis->identical (that.mThis) ;
		}
	} ;
} ;

using File = typename FILE_HELP<DEPEND ,ALWAYS>::File ;

template <class...>
trait DIRECTORY_HELP ;

template <class...>
trait DIRECTORY_DECOUPLE_HELP ;

template <class...>
trait DIRECTORY_IMPLHOLDER_HELP ;

template <class DEPEND>
trait DIRECTORY_HELP<DEPEND ,ALWAYS> {
	struct CHILD {
		String<STR> mFile ;
		BOOL mIsFile ;
		BOOL mIsDire ;
		BOOL mIsLink ;
	} ;

	struct Holder implement Interface {
		virtual void initialize (CREF<String<STR>> dire_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual LENGTH depth () const = 0 ;
		virtual String<STR> absolute () const = 0 ;
		virtual ArrayList<CHILD> load () const = 0 ;
		virtual void fresh () const = 0 ;
		virtual BOOL available () const = 0 ;
		virtual BOOL lock () const = 0 ;
		virtual void build () const = 0 ;
		virtual void erase () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class Directory {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit Directory () = default ;

		explicit Directory (CREF<String<STR>> dire_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (dire_) ;
		}

		Auto native () const leftvalue {
			return mThis->native () ;
		}

		String<STR> path () const {
			return mThis->path () ;
		}

		String<STR> name () const {
			return mThis->name () ;
		}

		LENGTH depth () const {
			return mThis->depth () ;
		}

		String<STR> absolute () const {
			return mThis->absolute () ;
		}

		ArrayList<CHILD> load () const {
			return mThis->load () ;
		}

		void fresh () const {
			return mThis->fresh () ;
		}

		BOOL available () const {
			return mThis->available () ;
		}

		BOOL lock () const {
			return mThis->lock () ;
		}

		void build () const {
			return mThis->build () ;
		}

		void erase () const {
			return mThis->erase () ;
		}

		void clear () const {
			return mThis->clear () ;
		}
	} ;
} ;

using Directory = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Directory ;

template <class...>
trait STREAMFILE_HELP ;

template <class...>
trait STREAMFILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait STREAMFILE_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<String<STR>> file_) = 0 ;
		virtual void open () = 0 ;
		virtual void create () = 0 ;
		virtual void append () = 0 ;
		virtual void close () = 0 ;
		virtual BOOL link (CREF<BOOL> readable ,CREF<BOOL> writable) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<BYTE>> item) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<WORD>> item) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<CHAR>> item) = 0 ;
		virtual LENGTH read (VREF<RegBuffer<DATA>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<BYTE>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<WORD>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<CHAR>> item) = 0 ;
		virtual LENGTH write (CREF<RegBuffer<DATA>> item) = 0 ;
		virtual void flush () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class StreamFile {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit StreamFile () = default ;

		explicit StreamFile (CREF<String<STR>> file_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (file_) ;
		}

		BOOL link (CREF<BOOL> readable ,CREF<BOOL> writable) {
			return mThis->link (readable ,writable) ;
		}

		LENGTH read (VREF<RegBuffer<BYTE>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<BYTE>> item) {
			return read (RegBuffer<BYTE>::from (item)) ;
		}

		LENGTH read (VREF<RegBuffer<WORD>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<WORD>> item) {
			return read (RegBuffer<WORD>::from (item)) ;
		}

		LENGTH read (VREF<RegBuffer<CHAR>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<CHAR>> item) {
			return read (RegBuffer<CHAR>::from (item)) ;
		}

		LENGTH read (VREF<RegBuffer<DATA>> item) {
			return mThis->read (item) ;
		}

		LENGTH read (VREF<VarBuffer<DATA>> item) {
			return read (RegBuffer<DATA>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<BYTE>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<BYTE>> item) {
			return write (RegBuffer<BYTE>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<BYTE>> item) {
			return write (RegBuffer<BYTE>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<WORD>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<WORD>> item) {
			return write (RegBuffer<WORD>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<WORD>> item) {
			return write (RegBuffer<WORD>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<CHAR>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<CHAR>> item) {
			return write (RegBuffer<CHAR>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<CHAR>> item) {
			return write (RegBuffer<CHAR>::from (item)) ;
		}

		LENGTH write (CREF<RegBuffer<DATA>> item) {
			return mThis->write (item) ;
		}

		LENGTH write (CREF<VarBuffer<DATA>> item) {
			return write (RegBuffer<DATA>::from (item)) ;
		}

		LENGTH write (CREF<ConBuffer<DATA>> item) {
			return write (RegBuffer<DATA>::from (item)) ;
		}

		void flush () {
			return mThis->flush () ;
		}
	} ;
} ;

using StreamFile = typename STREAMFILE_HELP<DEPEND ,ALWAYS>::StreamFile ;

template <class...>
trait BUFFERFILE_HELP ;

template <class...>
trait BUFFERFILE_HOLDER_HELP ;

template <class...>
trait BUFFERFILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		virtual void initialize (CREF<String<STR>> file_ ,CREF<Clazz> clazz) = 0 ;
		virtual void set_cache_size (CREF<LENGTH> size_) = 0 ;
		virtual void open () = 0 ;
		virtual void create () = 0 ;
		virtual void append () = 0 ;
		virtual void close () = 0 ;
		virtual BOOL link (CREF<BOOL> readable ,CREF<BOOL> writable) = 0 ;
		virtual VAL64 length () const = 0 ;
		virtual VAL64 insert (CREF<VAL64> size_) = 0 ;
		virtual void get (CREF<VAL64> index ,VREF<RegBuffer<BYTE>> item) = 0 ;
		virtual void set (CREF<VAL64> index ,CREF<RegBuffer<BYTE>> item) = 0 ;
		virtual void flush () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;
} ;

template <class ITEM>
trait BUFFERFILE_HELP<ITEM ,ALWAYS> {
	using Holder = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using FUNCTION_extern = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern ;

	using SIZE = SIZE_OF<ITEM> ;

	class BufferFile {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit BufferFile () = default ;

		explicit BufferFile (CREF<String<STR>> file_) {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize (file_ ,Clazz (TYPEAS<ITEM>::expr)) ;
		}

		void set_cache_size (CREF<LENGTH> size_) {
			return mThis->set_cache_size (size_) ;
		}

		BOOL link (CREF<BOOL> readable ,CREF<BOOL> writable) {
			return mThis->link (readable ,writable) ;
		}

		VAL64 length () const {
			return mThis->length () ;
		}

		VAL64 insert (CREF<VAL64> size_) {
			return mThis->insert (size_) ;
		}

		ITEM get (CREF<VAL64> index) {
			ITEM ret ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (ret)) ;
			mThis->get (index ,RegBuffer<BYTE>::from (tmp ,0 ,SIZE::expr)) ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		void set (CREF<VAL64> index ,CREF<ITEM> item) {
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item)) ;
			mThis->set (index ,RegBuffer<BYTE>::from (tmp ,0 ,SIZE::expr)) ;
			unsafe_barrier () ;
		}

		void flush () {
			return mThis->flush () ;
		}
	} ;
} ;

template <class ITEM>
using BufferFile = typename BUFFERFILE_HELP<ITEM ,ALWAYS>::BufferFile ;
} ;