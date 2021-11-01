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
trait FILE_HELP ;

template <>
trait FILE_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual String<STR> path_name () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class File {
	private:
		VRef<Holder> mThis ;

	public:
		implicit File () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		String<STR> path () const {
			return mThis->path () ;
		}

		String<STR> name () const {
			return mThis->name () ;
		}

		String<STR> path_name () const {
			return mThis->path_name () ;
		}
	} ;
} ;

using File = typename FILE_HELP<ALWAYS>::File ;

template <class...>
trait DIRECTORY_HELP ;

template <>
trait DIRECTORY_HELP<ALWAYS> {
	struct Holder implement Interface {
		virtual String<STR> path () const = 0 ;
		virtual String<STR> name () const = 0 ;
		virtual String<STR> path_name () const = 0 ;
	} ;

	struct FUNCTION_make {
		imports VRef<Holder> extern_invoke () ;
	} ;

	class Directory {
	private:
		VRef<Holder> mThis ;

	public:
		implicit Directory () {
			mThis = FUNCTION_make::extern_invoke () ;
		}

		String<STR> path () const {
			return mThis->path () ;
		}

		String<STR> name () const {
			return mThis->name () ;
		}

		String<STR> path_name () const {
			return mThis->path_name () ;
		}
	} ;
} ;

using Directory = typename DIRECTORY_HELP<ALWAYS>::Directory ;

} ;
} ;

namespace CSC {
using namespace FILESYSTEM ;
} ;