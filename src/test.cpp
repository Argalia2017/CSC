#include "util.h"

using namespace CSC ;

exports void test_main () {
	/*
	^\t+struct Holder implement Interface \{(\r\n(?!\} ;)[^\r]*)*\r\n\t+(?!(return|implicit|explicit))(\w+ )?\w+(<[^\r]*>)? \w+ \(([^\r]*)\) (\w+ )*\{(?!\r\n(\t+Scope<Mutex> anonymous \(mMutex\) ;\r\n)?\t+return)
	
	*/
}