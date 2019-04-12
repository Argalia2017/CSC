#include "util.h"

#ifdef __CSC__
namespace UNITTEST {
TEST_CLASS (UNITTEST_CSC) {
public:
	TEST_METHOD (TEST_CSC) {
		_STATIC_WARNING_ ("noop") ;
	}
} ;
} ;
#endif