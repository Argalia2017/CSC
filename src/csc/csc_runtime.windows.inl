#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require windows.h"
#endif

#include "csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif
#include "csc_begin.h"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <exception>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include "csc_begin.h"

namespace CSC {
struct RuntimeToolPureLayout {} ;

class RuntimeProcImplement implement Fat<RuntimeToolHolder ,RuntimeToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<RuntimeToolPureLayout>::make () ;
	}

	LENGTH thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	FLAG thread_uid () const override {
		const auto r1x = std::this_thread::get_id () ;
		return FLAG (bitwise[TYPE<CHAR>::expr] (r1x)) ;
	}

	void thread_sleep (CREF<Time> time) const override {
		auto &&tmp = keep[TYPE<TimeLayout>::expr] (time) ;
		std::this_thread::sleep_for (tmp.mThis->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	FLAG process_uid () const override {
		return FLAG (GetCurrentProcessId ()) ;
	}

	void process_exit () const override {
		std::quick_exit (0) ;
	}

	FLAG random_seed () const override {
		return invoke (std::random_device ()) ;
	}

	String working_path () const override {
		String ret = String::make () ;
		GetCurrentDirectory (VAL32 (ret.size ()) ,ret.raw ()) ;
		ret = Path (ret).child ().path () ;
		return move (ret) ;
	}

	String module_path () const override {
		String ret = String::make () ;
		GetModuleFileName (NULL ,ret.raw () ,VAL32 (ret.size ())) ;
		ret = Path (ret).path () ;
		return move (ret) ;
	}

	String module_name () const override {
		String ret = String::make () ;
		GetModuleFileName (NULL ,ret.raw () ,VAL32 (ret.size ())) ;
		ret = Path (ret).name () ;
		return move (ret) ;
	}
} ;

exports VFat<RuntimeToolHolder> RuntimeToolHolder::create (VREF<RuntimeToolLayout> that) {
	return VFat<RuntimeToolHolder> (RuntimeProcImplement () ,Pointer::from (that)) ;
}

exports CFat<RuntimeToolHolder> RuntimeToolHolder::create (CREF<RuntimeToolLayout> that) {
	return CFat<RuntimeToolHolder> (RuntimeProcImplement () ,Pointer::from (that)) ;
}

struct ModulePureLayout {
	UniqueRef<HMODULE> mModule ;
	FLAG mLastError ;
	Pin<String> mLastFormat ;
} ;

class ModuleImplement implement Fat<ModuleHolder ,ModuleLayout> {
public:
	void initialize (CREF<String> file) override {
		const auto r1x = Path (file).name () ;
		assert (r1x.length () > 0) ;
		fake.mThis->mLastFormat.self = String::make () ;
		fake.mThis->mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
			me = GetModuleHandle (r1x.raw ()) ;
			if (me != NULL)
				return ;
			me = LoadLibrary (file.raw ()) ;
			if (me != NULL)
				return ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		} ,[&] (VREF<HMODULE> me) {
			noop () ;
		}) ;
	}

	FLAG load (CREF<String> name) override {
		assert (name.length () > 0) ;
		const auto r1x = StringTool::cvt_stra (name) ;
		FLAG ret = FLAG (GetProcAddress (fake.mThis->mModule ,r1x.raw (TYPE<STRA>::expr))) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String error () const override {
		const auto r1x = csc_enum_t (fake.mThis->mLastError) ;
		const auto r2x = csc_enum_t (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
		const auto r3x = VAL32 (fake.mThis->mLastFormat->size ()) ;
		FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,r1x ,r2x ,fake.mThis->mLastFormat->raw () ,r3x ,NULL) ;
		return String::make (slice ("LastError = ") ,FLAG (r1x) ,slice (" : ") ,fake.mThis->mLastFormat) ;
	}
} ;

exports VFat<ModuleHolder> ModuleHolder::create (VREF<ModuleLayout> that) {
	return VFat<ModuleHolder> (ModuleImplement () ,Pointer::from (that)) ;
}

exports CFat<ModuleHolder> ModuleHolder::create (CREF<ModuleLayout> that) {
	return CFat<ModuleHolder> (ModuleImplement () ,Pointer::from (that)) ;
}

struct ConsolePureLayout {
	UniqueRef<HANDLE> mConsole ;
} ;

class ConsoleImplement implement Fat<ConsoleHolder ,ConsolePureLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	void set_option (CREF<FLAG> option) override {
		unimplemented () ;
	}

	void print (CREF<String> msg) override {
		unimplemented () ;
	}

	void fatal (CREF<String> msg) override {
		unimplemented () ;
	}

	void error (CREF<String> msg) override {
		unimplemented () ;
	}

	void warn (CREF<String> msg) override {
		unimplemented () ;
	}

	void info (CREF<String> msg) override {
		unimplemented () ;
	}

	void debug (CREF<String> msg) override {
		unimplemented () ;
	}

	void verbose (CREF<String> msg) override {
		unimplemented () ;
	}

	void open (CREF<String> dire) override {
		unimplemented () ;
	}

	void show () override {
		if (fake.mConsole.exist ())
			return ;
		fake.mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			AllocConsole () ;
			me = GetStdHandle (STD_ERROR_HANDLE) ;
		} ,[&] (VREF<HANDLE> me) {
			FreeConsole () ;
		}) ;
	}

	void hide () override {
		fake.mConsole = UniqueRef<HANDLE> () ;
	}

	void pause () override {
		unimplemented () ;
	}

	void clear () override {
		unimplemented () ;
	}
} ;

exports SharedRef<ConsolePureLayout> ConsoleHolder::create () {
	return SharedRef<ConsolePureLayout>::make () ;
}

exports VFat<ConsoleHolder> ConsoleHolder::create (VREF<ConsolePureLayout> that) {
	return VFat<ConsoleHolder> (ConsoleImplement () ,Pointer::from (that)) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::create (CREF<ConsolePureLayout> that) {
	return CFat<ConsoleHolder> (ConsoleImplement () ,Pointer::from (that)) ;
}
} ;