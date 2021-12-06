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
namespace FILESYSTEM {
template <class...>
trait DIRECTORY_HELP ;

template <class...>
trait FILE_HELP ;

template <>
trait FILE_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_file (CREF<String<STR>> file) = 0 ;
		virtual Auto native () const = 0 ;
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual String<STR> full_name () const = 0 ;
		virtual VarBuffer<BYTE> load () const = 0 ;
		virtual void load (VREF<RegBuffer<BYTE>> data) const = 0 ;
		virtual void save (CREF<RegBuffer<BYTE>> data) const = 0 ;
		virtual CREF<RegBuffer<BYTE>> load_assert (VREF<ConBuffer<BYTE>> data) const = 0 ;
		virtual BOOL find () const = 0 ;
		virtual void erase () const = 0 ;
		virtual void copy_from (CREF<Holder> that) const = 0 ;
		virtual void move_from (CREF<Holder> that) const = 0 ;
		virtual void link_from (CREF<Holder> that) const = 0 ;
		virtual BOOL identical (CREF<Holder> that) const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class File {
	private:
		VRef<Holder> mThis ;

	public:
		implicit File () = default ;

		imports File make_file (CREF<String<STR>> file) {
			File ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_file (file) ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		String<STR> path () const {
			return mThis->path () ;
		}

		String<STR> name () const {
			return mThis->name () ;
		}

		String<STR> full_name () const {
			return mThis->full_name () ;
		}

		VarBuffer<BYTE> load () const {
			return mThis->load () ;
		}

		void load (VREF<RegBuffer<BYTE>> data) const {
			return mThis->load (data) ;
		}

		void save (CREF<RegBuffer<BYTE>> data) const {
			return mThis->save (data) ;
		}

		CREF<RegBuffer<BYTE>> load_assert (RREF<ConBuffer<BYTE>> where_ = ConBuffer<BYTE> ()) {
			return mThis->load_assert (where_) ;
		}

		BOOL find () const {
			return mThis->find () ;
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

using File = typename FILE_HELP<ALWAYS>::File ;

template <class...>
trait DIRECTORY_HELP ;

template <>
trait DIRECTORY_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual void init_file (CREF<String<STR>> file) = 0 ;
		virtual Auto native () const = 0 ;
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual String<STR> full_name () const = 0 ;
		virtual LENGTH depth () const = 0 ;
		virtual void parent_from (CREF<Holder> a) = 0 ;
		virtual void brother_from (CREF<Holder> a) = 0 ;
		virtual void child_from (CREF<Holder> a ,CREF<String<STR>> file) = 0 ;
		virtual LENGTH child_size () const = 0 ;
		virtual void child_from (CREF<Holder> a ,CREF<INDEX> index) = 0 ;
		virtual BOOL find () const = 0 ;
		virtual BOOL lock () const = 0 ;
		virtual void build () const = 0 ;
		virtual void erase () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;

	class Directory {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Directory () = default ;

		imports Directory make_file (CREF<String<STR>> file) {
			Directory ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->init_file (file) ;
			return move (ret) ;
		}

		Auto native () const {
			return mThis->native () ;
		}

		String<STR> path () const {
			return mThis->path () ;
		}

		String<STR> name () const {
			return mThis->name () ;
		}

		String<STR> full_name () const {
			return mThis->full_name () ;
		}
		
		LENGTH depth () const {
			return mThis->depth () ;
		}

		Directory parent () const {
			Directory ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->parent_from (mThis) ;
			return move (ret) ;
		}

		Directory brother () const {
			Directory ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->brother_from (mThis) ;
			return move (ret) ;
		}

		Directory child (CREF<String<STR>> file) const {
			Directory ret ;
			ret.mThis = FUNCTION_link::invoke () ;
			ret.mThis->child_from (mThis ,file) ;
			return move (ret) ;
		}

		File file () const {
			return File::make_file (full_name ()) ;
		}

		Array<Directory> load () const {
			Array<Directory> ret = Array<Directory> (mThis->child_size ()) ;
			for (auto &&i : ret.iter ()) {
				ret[i].mThis = FUNCTION_link::invoke () ;
				ret[i].mThis->child_from (mThis ,i) ;
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

using Directory = typename DIRECTORY_HELP<ALWAYS>::Directory ;

template <class...>
trait STREAMLOADER_HELP ;

template <>
trait STREAMLOADER_HELP<ALWAYS> {
	class StreamLoader {} ;
} ;

using StreamLoader = typename STREAMLOADER_HELP<ALWAYS>::StreamLoader ;

template <class...>
trait BUFFERLOADER_HELP ;

template <>
trait BUFFERLOADER_HELP<ALWAYS> {
	class BufferLoader {} ;
} ;

using BufferLoader = typename BUFFERLOADER_HELP<ALWAYS>::BufferLoader ;

} ;
} ;

namespace CSC {
using namespace FILESYSTEM ;
} ;