#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <csc_begin.h>

using namespace SOLUTION ;

struct B ;

struct A {
	String<STR> mName ;
	UniqueRef<B> mB ;
} ;

struct B {
	String<STR> mName ;
	UniqueRef<A> mA ;
} ;

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	if ifdo (TRUE) {
		auto rbx = B () ;
		auto rax = A () ;
		rax.mName = slice ("A") ;
		rax.mB = UniqueRef<B> ([&] (VREF<B> me) {
			Singleton<Console>::instance ().info (slice ("A::create")) ;
		} ,[&] (VREF<B> me) {
			Singleton<Console>::instance ().info (slice ("A::destroy")) ;
		}) ;
		rbx.mName = slice ("B") ;
		rbx.mA = UniqueRef<A> ([&] (VREF<A> me) {
			Singleton<Console>::instance ().info (slice ("B::create")) ;
		} ,[&] (VREF<A> me) {
			Singleton<Console>::instance ().info (slice ("B::destroy")) ;
		}) ;
		rbx.mA.depend (rax.mB) ;
	}

	GlobalProc::shutdown () ;
	return 0 ; 
}