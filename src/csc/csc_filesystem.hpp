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

template <class DEPEND>
trait FILE_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SIZE = ENUMAS<VAL ,ENUMID<1023>> ;

	struct Holder implement Interface {
		virtual void init_file (CREF<String<STR>> file) = 0 ;
		virtual Auto native () const = 0 ;
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual String<STR> full_name () const = 0 ;
		virtual VarBuffer<BYTE> load () const = 0 ;
		virtual void load (VREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual void save (CREF<RegBuffer<BYTE>> item) const = 0 ;
		virtual CREF<RegBuffer<BYTE>> load_assert (CREF<FLAG> uuid) const = 0 ;
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

		void load (VREF<RegBuffer<BYTE>> item) const {
			return mThis->load (item) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const {
			return mThis->save (item) ;
		}

		CREF<RegBuffer<BYTE>> load_assert (CREF<FLAG> uuid) const {
			return mThis->load_assert (uuid) ;
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

using File = typename FILE_HELP<DEPEND ,ALWAYS>::File ;

template <class...>
trait DIRECTORY_HELP ;

template <class DEPEND>
trait DIRECTORY_HELP<DEPEND ,ALWAYS> {
	using DIRECTORY_CHILD_SIZE = ENUMAS<VAL ,ENUMID<65536>> ;

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

using Directory = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Directory ;

template <class...>
trait STREAMLOADER_HELP ;

template <class DEPEND>
trait STREAMLOADER_HELP<DEPEND ,ALWAYS> {
	class StreamLoader ;
} ;

using StreamLoader = typename STREAMLOADER_HELP<DEPEND ,ALWAYS>::StreamLoader ;

template <class...>
trait BUFFERLOADER_HELP ;

template <class DEPEND>
trait BUFFERLOADER_HELP<DEPEND ,ALWAYS> {
	class BufferLoader ;
} ;

using BufferLoader = typename BUFFERLOADER_HELP<DEPEND ,ALWAYS>::BufferLoader ;
} ;
} ;

namespace CSC {
using namespace FILESYSTEM ;
} ;