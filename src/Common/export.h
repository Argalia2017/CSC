#include "../util.h"

namespace CSC {
struct ConfigProcImplLayout ;

struct ConfigProcLayout implement ThisLayout<SharedRef<ConfigProcImplLayout>> {} ;

struct ConfigProcHolder implement Interface {
	imports CREF<ConfigProcLayout> instance () ;
	imports VFat<ConfigProcHolder> hold (VREF<ConfigProcLayout> me) ;
	imports CFat<ConfigProcHolder> hold (CREF<ConfigProcLayout> me) ;

	virtual void initialize () = 0 ;
	virtual void set_data_dire (CREF<String<STR>> path) const = 0 ;
} ;

class ConfigProc implement ConfigProcLayout {
protected:
	using ConfigProcLayout::mThis ;

public:
	static CREF<ConfigProc> instance () {
		return keep[TYPE<ConfigProc>::expr] (ConfigProcHolder::instance ()) ;
	}

	static void set_data_dire (CREF<String<STR>> dire) {
		return ConfigProcHolder::hold (instance ())->set_data_dire (dire) ;
	}
} ;
} ;