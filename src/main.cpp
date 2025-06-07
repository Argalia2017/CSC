#include "util.h"

#include "Common/export.h"

#include <csc_end.h>
#include <initializer_list>
#include <utility>
#include <csc_begin.h>

using namespace SOLUTION ;

struct FriendMonad implement Interface {
	virtual void execute (CREF<Pointer> context) const = 0 ;
} ;

class FriendMonadBinder final implement Fat<FriendMonad ,csc_handle_t> {
public:
	static VFat<FriendMonad> hold (VREF<csc_handle_t> that) {
		return VFat<FriendMonad> (FriendMonadBinder () ,that) ;
	}

	static CFat<FriendMonad> hold (CREF<csc_handle_t> that) {
		return CFat<FriendMonad> (FriendMonadBinder () ,that) ;
	}

	void execute (CREF<Pointer> context) const override {
		const auto r1x = DEF<DEF<void (CREF<Pointer>)> *> (self) ;
		return r1x (context) ;
	}
} ;

class Monad {
protected:
	Box<ReflectFriendBinder<FriendMonad ,FriendMonadBinder>> mBinder ;
	csc_handle_t mMonadFunc ;

public:
	implicit Monad () = delete ;

	template <class ARG1>
	explicit Monad (RREF<ARG1> that) {
		mMonadFunc = csc_handle_t (that) ;
		mBinder = Box<ReflectFriendBinder<FriendMonad ,FriendMonadBinder>>::make () ;
	}

	template <class ARG1>
	void once (CREF<ARG1> context) {
		return mBinder->hold (Pointer::from (mMonadFunc))->execute (Pointer::from (context)) ;
	}

	template <class ARG1>
	forceinline void operator() (CREF<ARG1> context) {
		return once (context) ;
	}
} ;

int main () {
	Singleton<Console>::expr.open (slice (".")) ;
	Singleton<Console>::expr.show () ;
	Singleton<Console>::expr.debug (slice ("library_file = ") ,RuntimeProc::library_file ()) ;
	ConfigProc::set_data_dire (slice (".")) ;
	ConfigProc::set_cxx_signal () ;

	auto rax = Monad ([] (CREF<int> param) {
		Singleton<Console>::expr.info (slice ("param = ") ,param) ;
	}) ;
	rax (10) ;

	GlobalProc::shutdown () ;
	return 0 ;
}