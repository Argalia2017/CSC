#include "export.h"

namespace CSC {
class ConfigImplHolder implement ConfigHolder {
protected:


public:
	void initialize () override {

	}

	void execute () override {
		const auto r1x = RuntimeProc::library_name () ;
		Singleton<Console>::instance ().info (slice ("library_name = ") ,r1x) ;
		const auto r2x = QUAD (address (Heap::instance ())) ;
		Singleton<Console>::instance ().info (slice ("heap_address = ") ,r2x) ;
		const auto r3x = QUAD (address (Singleton<Console>::instance ())) ;
		Singleton<Console>::instance ().info (slice ("console_address = ") ,r3x) ;
	}
} ;

exports DLLEXTERN AutoRef<ConfigHolder> DLLAPI ConfigHolder::create () {
	return AutoRef<ConfigImplHolder>::make () ;
}
} ;