#include "util.h"

#include <iostream>

using namespace CSC ;
using namespace std ;

#pragma warning (disable :4189)

struct X {
	int mX ;

public:
	implicit X () noexcept {
		static auto mInstance = 0 ;
		mX = mInstance++ ;
		cerr << this << "create: {" << mX << "}" << "\n" ;
	}

	implicit ~X () noexcept {
		cerr << this << "destroy: {" << mX << "}" << "\n" ;
	}

	implicit X (CREF<X>) = delete ;

	forceinline VREF<X> operator= (CREF<X>) = delete ;

	implicit X (RREF<X> that) noexcept {
		mX = that.mX ;
		that.mX = -1 ;
	}

	forceinline VREF<X> operator= (RREF<X> that) noexcept {
		if (address (thiz) == address (that))
			return thiz ;
		CSC::swap (thiz ,CSC::move (that)) ;
		return thiz ;
	}
} ;

template <>
class ReflectEqualBinder<X> final implement ReflectEqual {
public:
	BOOL equal (CREF<Pointer> this_ ,CREF<Pointer> that_) const override {
		return FALSE ;
	}
} ;

template <>
class ReflectComprBinder<X> final implement ReflectCompr {
public:
	FLAG compr (CREF<Pointer> this_ ,CREF<Pointer> that_) const override {
		return FALSE ;
	}
} ;

template <>
class ReflectVisitBinder<X> final implement ReflectVisit {
public:
	void visit (CREF<Visitor> visitor ,CREF<Pointer> this_) const override {
		noop () ;
	}
} ;

exports int main () {
	if ifdo (TRUE) {
		const auto r1x = SharedRef<X>::make () ;
		const auto r2x = SharedRef<X>::make () ;
		const auto r3x = r1x ;
	}
	if ifdo (TRUE) {
		const auto r103x = RefBuffer<AllocatorNode<ListNode<int>>> () ;
		const auto r102x = Allocator<ListNode<int>> () ;
		auto rax = List<int> () ;
		rax.add (1) ;
		rax.add (2) ;
		rax.add (3) ;
		rax.add (4) ;
		cerr << "rax = [" << "\n" ;
		for (auto &&i : rax.iter ()) {
			cerr << rax[i] << " ," ;
		}
		cerr << "\n" ;
		cerr << "]" << "\n" ;
		auto rbx = List<X> () ;
		rbx.add (X ()) ;
		rbx.add (X ()) ;
		rbx.add (X ()) ;
		rbx.add (X ()) ;
		cerr << "rax = [" << "\n" ;
		for (auto &&i : rbx.iter ()) {
			cerr << rbx[i].mX << " ," ;
		}
		cerr << "\n" ;
		cerr << "]" << "\n" ;
	}
	return 0 ;
}