﻿#include "export.h"

#include <csc_end.h>
#include <csignal>
#include <csetjmp>
#include <csc_begin.h>

namespace CSC {
struct ConfigProcLayout {
	Path mDataPath ;
} ;

class ConfigProcImplHolder final implement Fat<ConfigProcHolder ,ConfigProcLayout> {
public:
	void initialize () override {
		Singleton<Console>::expr.debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	}

	void set_data_dire (CR<String<STR>> dire) override {
		self.mDataPath = Path (dire) ;
		const auto r1x = FileProc::lock_dire (self.mDataPath) ;
		assume (r1x) ;
	}

	void enter () const override {
		std::signal (SIGINT ,cxx_signal_handle) ;
		std::signal (SIGILL ,cxx_signal_handle) ;
		std::signal (SIGFPE ,cxx_signal_handle) ;
		std::signal (SIGSEGV ,cxx_signal_handle) ;
		std::signal (SIGTERM ,cxx_signal_handle) ;
		std::signal (SIGABRT ,cxx_signal_handle) ;
	}

	void leave () const override {
		GlobalProc::shutdown () ;
	}

	static void cxx_signal_handle (int code) {
		Singleton<Console>::expr.trace () ;
		Singleton<Console>::expr.fatal (slice ("signal error")) ;
		Singleton<Console>::expr.fatal (slice ("code = ") ,build_code (code)) ;
		Singleton<Console>::expr.trace () ;
		RuntimeProc::process_exit () ;
	}

	static Slice build_code (int code) {
		if (code == SIGINT)
			return slice ("SIGINT") ;
		if (code == SIGILL)
			return slice ("SIGILL") ;
		if (code == SIGFPE)
			return slice ("SIGFPE") ;
		if (code == SIGSEGV)
			return slice ("SIGSEGV") ;
		if (code == SIGTERM)
			return slice ("SIGTERM") ;
		if (code == SIGABRT)
			return slice ("SIGABRT") ;
		return slice ("UNKNOWN") ;
	}
} ;

exports CR<OfThis<SharedRef<ConfigProcLayout>>> ConfigProcHolder::expr_m () {
	return memorize ([&] () {
		OfThis<SharedRef<ConfigProcLayout>> ret ;
		ret.mThis = SharedRef<ConfigProcLayout>::make () ;
		ConfigProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConfigProcHolder> ConfigProcHolder::hold (VR<ConfigProcLayout> that) {
	return VFat<ConfigProcHolder> (ConfigProcImplHolder () ,that) ;
}

exports CFat<ConfigProcHolder> ConfigProcHolder::hold (CR<ConfigProcLayout> that) {
	return CFat<ConfigProcHolder> (ConfigProcImplHolder () ,that) ;
}
} ;