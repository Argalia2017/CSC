#include "util.h"

namespace UNITTEST {
imports Auto get_int () {
	return VAR32 (2) ;
}

imports Auto get_float () {
	return SINGLE (3.1) ;
}

imports Auto get_string () {
	return Slice<STR> ("test" ,"2") ;
}

struct A :public Interface {
	virtual void test () = 0 ;
} ;

struct ImplA :public A {
	void test () override {
		const auto r1x = VAR32 (get_int ().cast (TYPEAS<VAR32>::id)) ;
		assert (r1x == VAR32 (2)) ;
		const auto r2x = SINGLE (get_float ().cast (TYPEAS<SINGLE>::id)) ;
		assert (r2x == SINGLE (3.1)) ;
		const auto r3x = Slice<STR> (get_string ().cast (TYPEAS<Slice<STR>>::id)) ;
		assert (r3x == Slice<STR> ("test" ,"2")) ;
		Clazz (TYPEAS<int>::id).type_name () ;
	}
} ;
} ;

#ifdef __CSC_TARGET_EXE__
exports int main () noexcept {
	using namespace UNITTEST ;
	auto rax = Box<A>::make (TYPEAS<ImplA>::id) ;
	rax->test () ;
	return 0 ;
}
#endif

#include "csc2/csc_core.hpp.inl"