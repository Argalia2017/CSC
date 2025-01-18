#include "export.h"

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
		Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	}

	void set_data_dire (CREF<String<STR>> dire) override {
		fake.mDataPath = Path (dire) ;
		const auto r1x = FileProc::lock_dire (fake.mDataPath) ;
		assume (r1x) ;
	}

	void set_cxx_signal () override {
		std::signal (SIGINT ,cxx_signal_handle) ;
		std::signal (SIGILL ,cxx_signal_handle) ;
		std::signal (SIGFPE ,cxx_signal_handle) ;
		std::signal (SIGSEGV ,cxx_signal_handle) ;
		std::signal (SIGTERM ,cxx_signal_handle) ;
		std::signal (SIGABRT ,cxx_signal_handle) ;
	}

	static void cxx_signal_handle (int code) {
		Singleton<Console>::instance ().trace () ;
		Singleton<Console>::instance ().fatal (slice ("signal error")) ;
		Singleton<Console>::instance ().fatal (slice ("code = ") ,build_code (code)) ;
		Singleton<Console>::instance ().trace () ;
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

exports CREF<SharedRef<ConfigProcLayout>> ConfigProcHolder::instance () {
	return memorize ([&] () {
		SharedRef<ConfigProcLayout> ret = SharedRef<ConfigProcLayout>::make () ;
		ConfigProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConfigProcHolder> ConfigProcHolder::hold (VREF<ConfigProcLayout> that) {
	return VFat<ConfigProcHolder> (ConfigProcImplHolder () ,that) ;
}

exports CFat<ConfigProcHolder> ConfigProcHolder::hold (CREF<ConfigProcLayout> that) {
	return CFat<ConfigProcHolder> (ConfigProcImplHolder () ,that) ;
}
} ;