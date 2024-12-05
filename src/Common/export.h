#include "../util.h"

namespace CSC {
struct ConfigHolder implement Interface {
	imports DLLEXTERN AutoRef<ConfigHolder> DLLAPI create () ;

	virtual void initialize () = 0 ;
	virtual void execute () = 0 ;
} ;

class Config {
protected:
	AutoRef<ConfigHolder> mThis ;

public:
	explicit Config () {
		mThis = ConfigHolder::create () ;
		mThis->initialize () ;
	}

	void execute () {
		return mThis->execute () ;
	}
} ;
} ;