#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_DEBUGGER__
#define __CSC_DEBUGGER__
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
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"

namespace CSC {
template <class...>
trait CONSOLE_HELP ;

template <class...>
trait CONSOLE_IMPLHOLDER_HELP ;

template <class...>
trait CONSOLE_IMPLBINDER_HELP ;

template <class DEPEND>
trait CONSOLE_HELP<DEPEND ,ALWAYS> {
	using Binder = typename TEXTWRITER_HELP<STR ,ALWAYS>::Binder ;

	struct OPTION_CLAZZ {
		enum {
			Default ,
			NoPrint ,
			NoFatal ,
			NoError ,
			NoWarn ,
			NoInfo ,
			NoDebug ,
			NoVerbose ,
			EnumSize
		} ;
	} ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void enable_option (CREF<FLAG> option) const = 0 ;
		virtual void print (CREF<Binder> msg) const = 0 ;
		virtual void fatal (CREF<Binder> msg) const = 0 ;
		virtual void error (CREF<Binder> msg) const = 0 ;
		virtual void warn (CREF<Binder> msg) const = 0 ;
		virtual void info (CREF<Binder> msg) const = 0 ;
		virtual void debug (CREF<Binder> msg) const = 0 ;
		virtual void verbose (CREF<Binder> msg) const = 0 ;
		virtual void open (CREF<String<STR>> dire) const = 0 ;
		virtual void show () const = 0 ;
		virtual void hide () const = 0 ;
		virtual void pause () const = 0 ;
		virtual void clear () const = 0 ;
	} ;

	class Console {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Console> instance () {
			return memorize ([&] () {
				Console ret ;
				ret.mMutex = RecursiveMutex::make () ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		void enable_option (CREF<FLAG> option) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->enable_option (option) ;
		}

		template <class...ARG1>
		void print (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->print (rax) ;
		}

		template <class...ARG1>
		void fatal (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->fatal (rax) ;
		}

		template <class...ARG1>
		void error (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->error (rax) ;
		}

		template <class...ARG1>
		void warn (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->warn (rax) ;
		}

		template <class...ARG1>
		void info (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->info (rax) ;
		}

		template <class...ARG1>
		void debug (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->debug (rax) ;
		}

		template <class...ARG1>
		void verbose (CREF<ARG1>...msg) const {
			using R1X = typename KILL<CONSOLE_IMPLBINDER_HELP<TYPEAS<ARG1...> ,ALWAYS> ,DEPEND>::ImplBinder ;
			Scope<Mutex> anonymous (mMutex) ;
			const auto r1x = capture (msg...) ;
			auto rax = R1X (CRef<Capture<ARG1...>>::reference (r1x)) ;
			return mThis->verbose (rax) ;
		}

		void open (CREF<String<STR>> dire) const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->open (dire) ;
		}

		void show () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->show () ;
		}

		void hide () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->hide () ;
		}

		void pause () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->pause () ;
		}

		void clear () const {
			Scope<Mutex> anonymous (mMutex) ;
			return mThis->clear () ;
		}
	} ;
} ;

template <class...UNIT>
trait CONSOLE_IMPLBINDER_HELP<TYPEAS<UNIT...> ,ALWAYS> {
	using Binder = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Binder ;

	class ImplBinder implement Binder {
	protected:
		CRef<Capture<UNIT...>> mThat ;

	public:
		implicit ImplBinder () = delete ;

		explicit ImplBinder (RREF<CRef<Capture<UNIT...>>> that) {
			mThat = move (that) ;
		}

		void friend_write (VREF<TextWriter<STR>> writer) const override {
			//@fatal: fuck msvc
			mThat.self ([&] (CREF<UNIT>...obj) {
				writer.prints (obj...) ;
			}) ;
		}
	} ;
} ;

using Console = typename CONSOLE_HELP<DEPEND ,ALWAYS>::Console ;

template <class...>
trait REPORTER_HELP ;

template <class...>
trait REPORTER_IMPLHOLDER_HELP ;

template <class DEPEND>
trait REPORTER_HELP<DEPEND ,ALWAYS> {
	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void detect_memory_leaks () const = 0 ;
		virtual void detect_crash_signal () const = 0 ;
		virtual Array<FLAG> captrue_stack_trace () const = 0 ;
		virtual String<STR> symbol_from_address (CREF<FLAG> addr) const = 0 ;
	} ;

	class Reporter {
	protected:
		Mutex mMutex ;
		VRef<Holder> mThis ;

	public:
		imports CREF<Reporter> instance () {
			return memorize ([&] () {
				Reporter ret ;
				ret.mMutex = RecursiveMutex::make () ;
				ret.mThis = Holder::create () ;
				ret.mThis->initialize () ;
				return move (ret) ;
			}) ;
		}

		void detect_memory_leaks () const {
			return mThis->detect_memory_leaks () ;
		}

		void detect_crash_signal () const {
			return mThis->detect_crash_signal () ;
		}

		Array<FLAG> captrue_stack_trace () const {
			return mThis->captrue_stack_trace () ;
		}

		String<STR> symbol_from_address (CREF<FLAG> addr) const {
			return mThis->symbol_from_address (addr) ;
		}
	} ;
} ;

using Reporter = typename REPORTER_HELP<DEPEND ,ALWAYS>::Reporter ;
} ;