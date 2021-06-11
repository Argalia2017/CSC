#include "csc2/csc.hpp"
#include "csc2/csc_core.hpp"

using namespace CSC ;

imports Auto get_int () {
	return VAR32 (2) ;
}

imports Auto get_float () {
	return SINGLE (3.1) ;
}

imports Auto get_string () {
	return Slice<STR> (TYPEAS<struct anonymous>::id ,"test") ;
}

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	const auto r1x = VAR32 (get_int ().fetch (TYPEAS<VAR32>::id)) ;
	assert (r1x == VAR32 (2)) ;
	const auto r2x = SINGLE (get_float ().fetch (TYPEAS<SINGLE>::id)) ;
	assert (r2x == SINGLE (3.1)) ;
	const auto r3x = Slice<STR> (get_string ().fetch (TYPEAS<Slice<STR>>::id)) ;
	assert (r3x == Slice<STR> (TYPEAS<struct anonymous>::id ,"test")) ;
	return 0 ;
}
#endif

#include "csc2/csc_core.hpp.inl"