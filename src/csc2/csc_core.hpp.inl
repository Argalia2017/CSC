#pragma once

#include "csc_core.hpp"

namespace CSC {
namespace U {
struct ANY_HELP<>::DETAIL::BLOCK {
	FEAT mBlockCheck1 ;
	FEAT mBlockCheck2 ;
} ;

struct ANY_HELP<>::DETAIL::SLOT {
	BOOL mUsed ;
	LENGTH mBlockSize ;
	INDEX mBlock ;
} ;

class ANY_HELP<>::DETAIL::AnyHeap {
private:
	static constexpr auto BLOCK_SIZE = 256 ;
	static constexpr auto SLOT_SIZE = 16 ;
	static constexpr auto BLOCK_CHECK = FEAT (0XCCCCCCCCCCCCCCCC) ;

public:
	template <class ARG1>
	imports auto alloc (CREF<ARG1> id)
		->UNSAFE_PTR<REMOVE_ALL<ARG1>> {
		using R1X = typeof (id) ;
		require (IS_TRIVIAL<R1X>) ;
		const auto r3x = property (slot_array ()) ;
		const auto r4x = property (block_array ()) ;
		INDEX ix = alloc (SIZEOF<R1X>::value ,ALIGNOF<R1X>::value) ;
		assert (ix != NONE) ;
		INDEX jx = property[r3x][ix].mBlock ;
		const auto r2x = address (property[r4x][jx]) + SIZEOF<BLOCK>::value ;
		const auto r5x = alignto (r2x ,ALIGNOF<R1X>::value) ;
		unsafe_barrier () ;
		return reinterpret_cast<UNSAFE_PTR<R1X>> (r5x) ;
	}

	imports auto alloc (CREF<LENGTH> size_len ,CREF<LENGTH> align_len)
		->INDEX {
		assert (size_len > ZERO) ;
		assert (align_len > ZERO) ;
		const auto r3x = property (slot_array ()) ;
		const auto r4x = property (block_array ()) ;
		INDEX ix = empry_slot (property[r3x]) ;
		INDEX iy = ix + 1 ;
		assert (between (ix ,0 ,SLOT_SIZE)) ;
		assert (between (iy ,0 ,SLOT_SIZE)) ;
		const auto r10x = align_len - ALIGNOF<BLOCK>::value ;
		const auto r12x = size_len + max (r10x ,ZERO) ;
		const auto r13x = (r12x + SIZEOF<BLOCK>::value - 1) / SIZEOF<BLOCK>::value ;
		property[r3x][ix].mBlockSize += property[r3x][iy].mBlockSize ;
		property[r3x][iy].mBlockSize = 0 ;
		property[r3x][iy].mBlock = NONE ;
		const auto r14x = property[r3x][ix].mBlockSize - 1 - r13x ;
		if (r14x <= 0)
			return NONE ;
		property[r3x][ix].mBlockSize -= r14x ;
		property[r3x][iy].mBlockSize += r14x ;
		INDEX jx = property[r3x][ix].mBlock + 1 + r13x ;
		property[r3x][iy].mBlock = jx ;
		property[r4x][jx].mBlockCheck1 = BLOCK_CHECK ;
		property[r4x][jx].mBlockCheck2 = BLOCK_CHECK ;
		return ix ;
	}

	imports void popup () {
		const auto r3x = property (slot_array ()) ;
		INDEX ix = empry_slot (property[r3x]) ;
		assert (between (ix ,0 ,SLOT_SIZE)) ;
		property[r3x][ix].mUsed = TRUE ;
	}

	imports void free (CREF<LENGTH> addr) {
		const auto r3x = property (slot_array ()) ;
		const auto r4x = property (block_array ()) ;
		INDEX ix = empry_slot (property[r3x]) ;
		INDEX iy = ix - 1 ;
		assert (between (ix ,0 ,SLOT_SIZE)) ;
		assert (between (iy ,0 ,SLOT_SIZE)) ;
		INDEX jx = property[r3x][iy].mBlock ;
		assert (jx != NONE) ;
		assert (property[r4x][jx].mBlockCheck1 == BLOCK_CHECK) ;
		assert (property[r4x][jx].mBlockCheck2 == BLOCK_CHECK) ;
		const auto r10x = address (property[r4x][jx]) + SIZEOF<BLOCK>::value ;
		const auto r11x = r10x + (property[r3x][ix].mBlockSize - 1) * SIZEOF<BLOCK>::value ;
		assert (between (addr ,r10x ,r11x)) ;
		property[r3x][iy].mUsed = FALSE ;
		property[r3x][iy].mBlockSize += property[r3x][ix].mBlockSize ;
		property[r3x][ix].mBlockSize = 0 ;
		property[r3x][ix].mBlock = NONE ;
	}

private:
	imports auto block_array ()
		->VREF<BLOCK[BLOCK_SIZE]> {
		thread_local auto M_BLOCK = block_array_init () ;
		return M_BLOCK.mP1 ;
	}

	imports auto block_array_init ()
		->PACK<BLOCK[BLOCK_SIZE]> {
		PACK<BLOCK[BLOCK_SIZE]> ret ;
		ret.mP1[0].mBlockCheck1 = BLOCK_CHECK ;
		ret.mP1[0].mBlockCheck2 = BLOCK_CHECK ;
		return forward (ret) ;
	}

	imports auto slot_array ()
		->VREF<SLOT[SLOT_SIZE]> {
		thread_local auto M_SLOT = slot_array_init () ;
		return M_SLOT.mP1 ;
	}

	imports auto slot_array_init ()
		->PACK<SLOT[SLOT_SIZE]> {
		PACK<SLOT[SLOT_SIZE]> ret ;
		const auto r1x = SLOT {FALSE ,0 ,NONE} ;
		for (auto &&i : range (1 ,SLOT_SIZE))
			ret.mP1[i] = r1x ;
		ret.mP1[0].mUsed = FALSE ;
		ret.mP1[0].mBlockSize = BLOCK_SIZE ;
		ret.mP1[0].mBlock = 0 ;
		return forward (ret) ;
	}

	imports auto empry_slot (CREF<SLOT[SLOT_SIZE]> array_)
		->INDEX {
		for (auto &&i : range (0 ,SLOT_SIZE)) {
			if ifnot (array_[i].mUsed)
				return i ;
		}
		return NONE ;
	}
} ;

template <class UNIT1>
class ANY_IMPLHOLDER_HELP<UNIT1>::ImplHolder :
	public Holder {
private:
	using AnyHeap = typename DETAIL::AnyHeap ;

private:
	UNIT1 mValue ;

public:
	implicit ImplHolder () = delete ;

	explicit ImplHolder (RREF<UNIT1> that) :
		mValue (forward (that)) {}

	void destroy () override {
		auto &&thiz = property[this] ;
		auto rax = TEMP<ImplHolder> () ;
		unsafe_zeroize (rax) ;
		swap (rax ,unsafe_deptr[thiz]) ;
		unsafe_destroy (rax) ;
		unsafe_barrier () ;
		AnyHeap::free (address (thiz)) ;
	}

	LENGTH unsafe_addr () override {
		return address (mValue) ;
	}

	LENGTH type_size () const override {
		return SIZEOF<ImplHolder>::value ;
	}

	LENGTH type_align () const override {
		return ALIGNOF<ImplHolder>::value ;
	}

	FLAG type_cabi () const override {
		return operator_cabi (typeas<ImplHolder>::id) ;
	}
} ;

template <class UNIT1>
exports auto ANY_IMPLHOLDER_HELP<UNIT1>::EXTERN::create (RREF<UNIT1> that)
->UNSAFE_PTR<Holder> {
	using R1X = typeof (that) ;
	using R2X = typename ANY_IMPLHOLDER_HELP<R1X>::ImplHolder ;
	using R3X = typename DETAIL::AnyHeap ;
	const auto r1x = R3X::alloc (typeas<TEMP<R2X>>::id) ;
	unsafe_create (property[r1x] ,forward (that)) ;
	R3X::popup () ;
	return property (unsafe_deref[property[r1x]]) ;
} ;
} ;

namespace U {
template <class UNIT1 ,class UNTI2>
class BOX_IMPLHOLDER_HELP<UNIT1 ,UNTI2>::ImplHolder :
	public Holder {
private:
	UNTI2 mValue ;

public:
	implicit ImplHolder () = delete ;

	explicit ImplHolder (RREF<UNTI2> that) :
		mValue (forward (that)) {}

	void destroy () override {
		auto &&thiz = property[this] ;
		delete property (thiz) ;
	}

	VREF<UNIT1> to () leftvalue override {
		return mValue ;
	}

	CREF<UNIT1> to () const leftvalue override {
		return mValue ;
	}
} ;

template <class UNIT1 ,class UNIT2>
exports auto BOX_IMPLHOLDER_HELP<UNIT1 ,UNIT2>::EXTERN::create (RREF<UNIT2> that)
->UNSAFE_PTR<Holder> {
	return new ImplHolder (forward (that)) ;
} ;
} ;

namespace U {
template <class UNIT1>
class RC_IMPLHOLDER_HELP<UNIT1>::ImplHolder :
	public Holder {
private:
	UNIT1 mValue ;
	LENGTH mCounter ;

public:
	implicit ImplHolder () = delete ;

	explicit ImplHolder (RREF<UNIT1> that) :
		mValue (forward (that)) ,
		mCounter (ZERO) {}

	void destroy () override {
		auto &&thiz = property[this] ;
		delete property (thiz) ;
	}

	CREF<UNIT1> to () const leftvalue override {
		return mValue ;
	}

	LENGTH increase () override {
		mCounter = mCounter + 1 ;
		return mCounter ;
	}

	LENGTH decrease () override {
		mCounter = mCounter - 1 ;
		return mCounter ;
	}
} ;

template <class UNIT1>
exports auto RC_IMPLHOLDER_HELP<UNIT1>::EXTERN::create (RREF<UNIT1> that)
->UNSAFE_PTR<Holder> {
	return new ImplHolder (forward (that)) ;
} ;
} ;
} ;