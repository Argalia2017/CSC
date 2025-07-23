#include "../util.h"

namespace CSC {
struct ConfigProcLayout ;

struct ConfigProcHolder implement Interface {
	imports CREF<OfThis<SharedRef<ConfigProcLayout>>> expr_m () ;
	imports VFat<ConfigProcHolder> hold (VREF<ConfigProcLayout> me) ;
	imports CFat<ConfigProcHolder> hold (CREF<ConfigProcLayout> me) ;

	virtual void initialize () = 0 ;
	virtual void set_data_dire (CREF<String<STR>> path) = 0 ;
	virtual void set_cxx_signal () = 0 ;
} ;

class ConfigProc implement OfThis<SharedRef<ConfigProcLayout>> {
public:
	static CREF<ConfigProc> expr_m () {
		return keep[TYPE<ConfigProc>::expr] (ConfigProcHolder::expr) ;
	}

	static void set_data_dire (CREF<String<STR>> dire) {
		return ConfigProcHolder::hold (expr)->set_data_dire (dire) ;
	}

	static void set_cxx_signal () {
		return ConfigProcHolder::hold (expr)->set_cxx_signal () ;
	}
} ;
} ;