#include "../util.h"

namespace CSC {
struct ConfigProcLayout ;

struct ConfigProcHolder implement Interface {
	imports CREF<SharedRef<ConfigProcLayout>> instance () ;
	imports VFat<ConfigProcHolder> hold (VREF<SharedRef<ConfigProcLayout>> me) ;
	imports CFat<ConfigProcHolder> hold (CREF<SharedRef<ConfigProcLayout>> me) ;

	virtual void initialize () = 0 ;
	virtual void set_data_dire (CREF<String<STR>> path) const = 0 ;
	virtual void set_cxx_signal () const = 0 ;
} ;

class ConfigProc implement OfThis<SharedRef<ConfigProcLayout>> {
public:
	static CREF<ConfigProc> instance () {
		return Pointer::from (ConfigProcHolder::instance ()) ;
	}

	static void set_data_dire (CREF<String<STR>> dire) {
		return ConfigProcHolder::hold (instance ())->set_data_dire (dire) ;
	}

	static void set_cxx_signal () {
		return ConfigProcHolder::hold (instance ())->set_cxx_signal () ;
	}
} ;
} ;