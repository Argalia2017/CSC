#pragma once

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_filesystem.hpp"

namespace CSC {
namespace DEBUGGER {
template <class...>
trait CONSOLE_HELP ;

template <>
trait CONSOLE_HELP<ALWAYS> {
	using Binder = typename TEXTWRITER_BINDER_HELP<STR ,ALWAYS>::Binder ;

	struct Holder implement Interface {
		virtual LENGTH buffer_size () const = 0 ;
		virtual void enable_option (CREF<FLAG> option) const = 0 ;
		virtual void disable_option (CREF<FLAG> option) const = 0 ;
		virtual void print (CREF<Binder> msg) const = 0 ;
		virtual void fatal (CREF<Binder> msg) const = 0 ;
		virtual void error (CREF<Binder> msg) const = 0 ;
		virtual void warn (CREF<Binder> msg) const = 0 ;
		virtual void info (CREF<Binder> msg) const = 0 ;
		virtual void debug (CREF<Binder> msg) const = 0 ;
		virtual void verbose (CREF<Binder> msg) const = 0 ;
		virtual void attach_log (CREF<String<STR>> file) const = 0 ;
		virtual void log (CREF<RegBuffer<STR>> tag ,CREF<Binder> msg) const = 0 ;
		virtual void show () const = 0 ;
		virtual void hide () const = 0 ;
		virtual void pause () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	struct FUNCTION_link {
		imports VRef<Holder> invoke () ;
	} ;
	
	class Console {
	private:
		Box<Mutex> mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Console> instance () {
			return memorize ([&] () {
				Console ret ;
				ret.mMutex = Box<Mutex>::make (Mutex::make_recursive_mutex ()) ;
				ret.mThis = FUNCTION_link::invoke () ;
				return move (ret) ;
			}) ;
		}

		LENGTH buffer_size () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->buffer_size () ;
		}

		void enable_option (CREF<FLAG> option) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->enable_option (option) ;
		}

		void disable_option (CREF<FLAG> option) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->disable_option (option) ;
		}

		template <class...ARGS>
		void print (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->print (R1X (ARGVP0 ,msg...)) ;
		}

		template <class...ARGS>
		void fatal (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->fatal (R1X (ARGVP0 ,msg...)) ;
		}

		template <class...ARGS>
		void error (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->error (R1X (ARGVP0 ,msg...)) ;
		}

		template <class...ARGS>
		void warn (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->warn (R1X (ARGVP0 ,msg...)) ;
		}

		template <class...ARGS>
		void info (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->info (R1X (ARGVP0 ,msg...)) ;
		}

		template <class...ARGS>
		void debug (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->debug (R1X (ARGVP0 ,msg...)) ;
		}

		template <class...ARGS>
		void verbose (XREF<ARGS>...msg) const {
			//using R1X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplBinder<_ARGS...> ;
			//Scope<CREF<Mutex>> anonymous (mMutex) ;
			//return mThis->verbose (R1X (ARGVP0 ,msg...)) ;
		}

		void attach_log (CREF<String<STR>> file) const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->attach_log (file) ;
		}

		void show () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->show () ;
		}

		void pause () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->pause () ;
		}

		void clear () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->clear () ;
		}

		void hide () const {
			Scope<CREF<Mutex>> anonymous (mMutex) ;
			return mThis->hide () ;
		}
	} ;
} ;
} ;
} ;

namespace CSC {
using namespace DEBUGGER ;
} ;