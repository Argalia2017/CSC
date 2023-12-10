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
	auto rax = RefAllocator<Tuple<int ,float>> (20) ;
	INDEX ix = NONE ;
	ix = rax.alloc () ;
	rax[ix].m1st = 2 ;
	rax[ix].m2nd = 0 ;
	ix = rax.alloc () ;
	rax[ix].m1st = 4 ;
	ix = rax.alloc () ;
	INDEX iy = ix ;
	rax[ix].m1st = 6 ;
	ix = rax.alloc () ;
	rax[ix].m1st = 7 ;
	ix = rax.alloc () ;
	rax[ix].m1st = 2 ;
	rax.free (iy) ;
	ix = rax.alloc () ;
	rax[ix].m1st = 3 ;
	return 0 ;
}