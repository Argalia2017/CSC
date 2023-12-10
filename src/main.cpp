#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

class XX {
public:
	XX () noexcept {
		cerr << "this = " << this << "\n" ;
	}
} ;

exports int main () {
	auto rax = Array<int> (20) ;
	rax.fill (22) ;
	return 0 ;
}