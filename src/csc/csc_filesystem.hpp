#pragma once

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
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
	using FILE_NAME_SSIZE = ENUMAS<VAL ,ENUMID<1023>> ;

	struct Holder implement Interface {
		virtual void init_file (CREF<String<STR>> file_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual LENGTH file_size () const = 0 ;
		virtual VarBuffer<BYTE> load () const = 0 ;
		virtual void load (VREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual void save (CREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual CRef<RegBuffer<BYTE>> load_asset () const = 0 ;
		virtual BOOL find () const = 0 ;
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
			mThis->init_file (file_) ;
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

		BOOL find () const {
			return mThis->find () ;
		}

		LENGTH file_size () const {
			return mThis->file_size () ;
		}

		void erase () const {
			return mThis->erase () ;
		}

		void copy_from (CREF<File> that) const {
			return mThis->copy_from (that.mThis) ;
		}

		void move_from (CREF<File> that) const {
			return mThis->copy_from (that.mThis) ;
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
	using DIRECTORY_CHILD_SIZE = ENUMAS<VAL ,ENUMID<65536>> ;

	struct Holder implement Interface {
		virtual void init_dire (CREF<String<STR>> dire_) = 0 ;
		virtual Auto native () const leftvalue = 0 ;
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual String<STR> absolute () const = 0 ;
		virtual LENGTH depth () const = 0 ;
		virtual void parent_from (CREF<Holder> a) = 0 ;
		virtual void brother_from (CREF<Holder> a) = 0 ;
		virtual void child_from (CREF<Holder> a) = 0 ;
		virtual BOOL find () const = 0 ;
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
			mThis->init_dire (dire_) ;
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

		String<STR> absolute () const {
			return mThis->absolute () ;
		}

		LENGTH depth () const {
			return mThis->depth () ;
		}

		Directory parent () const {
			Directory ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->parent_from (mThis) ;
			return move (ret) ;
		}

		Directory brother () const {
			Directory ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->brother_from (mThis) ;
			return move (ret) ;
		}

		Directory child () const {
			Directory ret ;
			ret.mThis = FUNCTION_extern::invoke () ;
			ret.mThis->child_from (mThis) ;
			return move (ret) ;
		}

		ArrayList<Directory> load () const {
			ArrayList<Directory> ret ;
			auto rax = Directory () ;
			rax.mThis = FUNCTION_extern::invoke () ;
			rax.mThis->child_from (mThis) ;
			while (TRUE) {
				if ifnot (rax.mThis->find ())
					break ;
				ret.add (move (rax)) ;
				rax.mThis = FUNCTION_extern::invoke () ;
				rax.mThis->brother_from (mThis) ;
			}
			return move (ret) ;
		}

		BOOL find () const {
			return mThis->find () ;
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

template <class DEPEND>
trait DIRECTORY_DECOUPLE_HELP<DEPEND ,ALWAYS> {
	using SUPER = typename TEXTREADER_IMPLHOLDER_HELP<STR ,ALWAYS>::ImplAttribute ;

	class ImplAttribute implement SUPER {
	public:
		BOOL is_space (CREF<STR> str) const override {
			if (str == STR ('\\'))
				return TRUE ;
			if (str == STR ('/'))
				return TRUE ;
			return FALSE ;
		}

		BOOL is_endline_space (CREF<STR> str) const override {
			return FALSE ;
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
		virtual void init_read (CREF<String<STR>> file_) const = 0 ;
		virtual void init_write (CREF<String<STR>> file_) const = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual void reset () = 0 ;
		virtual void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) = 0 ;
		virtual void backup () = 0 ;
		virtual void recover () = 0 ;
		virtual void read (VREF<RegBuffer<BYTE>> item) = 0 ;
		virtual void write (CREF<RegBuffer<BYTE>> item) = 0 ;
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

		explicit StreamFile (CREF<String<STR>> file_ ,CREF<BOOL> readonly) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (readonly)
					discard ;
				mThis = FUNCTION_extern::invoke () ;
				mThis->init_read (file_) ;
			}
			if ifswitch (eax) {
				mThis = FUNCTION_extern::invoke () ;
				mThis->init_write (file_) ;
			}
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
		
		void read (VREF<RegBuffer<BYTE>> item) {
			return mThis->read (item) ;
		}

		void read (VREF<VarBuffer<BYTE>> item) const {
			return read (RegBuffer<BYTE>::from (item)) ;
		}

		void read (VREF<RegBuffer<WORD>> item) {
			unimplemented () ;
		}

		void read (VREF<VarBuffer<WORD>> item) const {
			return read (RegBuffer<WORD>::from (item)) ;
		}

		void read (VREF<RegBuffer<CHAR>> item) {
			unimplemented () ;
		}

		void read (VREF<VarBuffer<CHAR>> item) const {
			return read (RegBuffer<CHAR>::from (item)) ;
		}

		void read (VREF<RegBuffer<DATA>> item) {
			unimplemented () ;
		}

		void read (VREF<VarBuffer<DATA>> item) const {
			return read (RegBuffer<DATA>::from (item)) ;
		}

		void write (CREF<RegBuffer<BYTE>> item) {
			return mThis->write (item) ;
		}

		void write (CREF<VarBuffer<BYTE>> item) const {
			return write (RegBuffer<BYTE>::from (item)) ;
		}

		void write (CREF<ConBuffer<BYTE>> item) const {
			return write (RegBuffer<BYTE>::from (item)) ;
		}

		void write (CREF<RegBuffer<WORD>> item) {
			unimplemented () ;
		}

		void write (CREF<VarBuffer<WORD>> item) const {
			return write (RegBuffer<WORD>::from (item)) ;
		}

		void write (CREF<ConBuffer<WORD>> item) const {
			return write (RegBuffer<WORD>::from (item)) ;
		}

		void write (CREF<RegBuffer<CHAR>> item) {
			unimplemented () ;
		}

		void write (CREF<VarBuffer<CHAR>> item) const {
			return write (RegBuffer<CHAR>::from (item)) ;
		}

		void write (CREF<ConBuffer<CHAR>> item) const {
			return write (RegBuffer<CHAR>::from (item)) ;
		}

		void write (CREF<RegBuffer<DATA>> item) {
			unimplemented () ;
		}

		void write (CREF<VarBuffer<DATA>> item) const {
			return write (RegBuffer<DATA>::from (item)) ;
		}

		void write (CREF<ConBuffer<DATA>> item) const {
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
trait BUFFERFILE_IMPLHOLDER_HELP ;

template <class ITEM>
trait BUFFERFILE_HELP<ITEM ,REQUIRE<IS_TRIVIAL<ITEM>>> {
	using BUFFER = DEPENDENT<ARR<BYTE> ,ITEM> ;

	struct Holder implement Interface {
		virtual void init_read (CREF<String<STR>> file_) const = 0 ;
		virtual void init_write (CREF<String<STR>> file_) const = 0 ;
		virtual LENGTH size () const = 0 ;
		virtual void read (CREF<INDEX> index ,VREF<BUFFER> buffer ,CREF<LENGTH> size_) = 0 ;
		virtual void write (CREF<INDEX> index ,CREF<BUFFER> buffer ,CREF<LENGTH> size_) = 0 ;
		virtual void resize (CREF<LENGTH> size_) = 0 ;
		virtual void flush () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class BufferFile {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit BufferFile () = default ;

		explicit BufferFile (CREF<String<STR>> file_ ,CREF<BOOL> readonly) {
			auto eax = TRUE ;
			if ifswitch (eax) {
				if ifnot (readonly)
					discard ;
				mThis = FUNCTION_extern::invoke () ;
				mThis->init_read (file_) ;
			}
			if ifswitch (eax) {
				mThis = FUNCTION_extern::invoke () ;
				mThis->init_write (file_) ;
			}
		}

		LENGTH size () const {
			return mThis->size () ;
		}

		ITEM get (CREF<INDEX> index) {
			using R1X = SIZE_OF<ITEM> ;
			ITEM ret ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<BYTE ,R1X>>>::id] (unsafe_deptr (ret))) ;
			mThis->read (index ,unsafe_array (tmp[0]) ,R1X::value) ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		void set (CREF<INDEX> index ,CREF<ITEM> item) {
			using R1X = SIZE_OF<ITEM> ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<BYTE ,R1X>>>::id] (unsafe_deptr (item))) ;
			mThis->write (index ,unsafe_array (tmp[0]) ,R1X::value) ;
		}

		void resize (CREF<INDEX> size_) {
			return mThis->resize (size_) ;
		}

		void flush () {
			return mThis->flush () ;
		}
	} ;
} ;

template <class ITEM>
using BufferFile = typename BUFFERFILE_HELP<ITEM ,ALWAYS>::BufferFile ;
} ;