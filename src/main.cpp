#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <csc_begin.h>

using namespace SOLUTION ;

struct A implement KeyBase<A> {
	INDEX mId ;
} ;

template struct KeyRootHolder<A> ;

exports CREF<KeyBaseLayout> KeyRootHolder<A>::instance () {
	return memorize ([&] () {
		KeyBaseLayout ret ;
		KeyBaseHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

int main () {
	Singleton<Console>::instance ().open (slice (".")) ;
	Singleton<Console>::instance ().show () ;
	Singleton<Console>::instance ().debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	const auto r1x = CurrentRandom () ;
	const auto rank = 1024 * 1024 ;
	auto rax = List<A> (rank) ;
	if ifdo (TRUE) {
		INDEX ix = rax.insert () ;
		rax[ix].mId = NONE ;
		rax[ix].set_def () ;
	}
	if ifdo (TRUE) {
		INDEX ix = rax.insert () ;
		rax[ix].mId = 30 ;
		rax[ix].set_key (30) ;
	}
	if ifdo (TRUE) {
		INDEX ix = rax.insert () ;
		rax[ix].mId = 30 ;
		rax[ix].set_key (30) ;
	}
	const auto r15x = CurrentTime () ;
	for (auto &&i : iter (3 ,rank)) {
		const auto r2x = i ;
		INDEX ix = rax.insert () ;
		rax[ix].mId = r2x ;
		rax[ix].set_key (r2x) ;
	}
	const auto r10x = CurrentTime () ;
	for (auto &&i : iter (0 ,10000)) {
		const auto r4x = r1x.random_value (0 ,rank - 1) ;
		const auto r3x = Key<A> (rax[r4x].mId) ;
		assume (r3x->mId == rax[r4x].mId) ;
	}
	const auto r11x = CurrentTime () ;
	for (auto &&i : iter (0 ,10000)) {
		const auto r4x = r1x.random_value (0 ,rank - 1) ;
		const auto r3x = Key<A> (rax[r4x].mId) ;
		assume (r3x->mId == rax[r4x].mId) ;
	}
	const auto r12x = CurrentTime () ;
	const auto r16x = (r10x - r15x).milliseconds () ;
	const auto r13x = (r11x - r10x).milliseconds () ;
	const auto r14x = (r12x - r11x).milliseconds () ;
	watch (r16x) ;
	watch (r13x) ;
	watch (r14x) ;

	GlobalProc::shutdown () ;
	return 0 ;
}