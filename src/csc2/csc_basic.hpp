#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
class BasicProc extend Proxy {
public:
} ;

namespace U {
template <class...>
trait ATOMIC_HELP ;

template <class UNIT1>
trait ATOMIC_HELP<UNIT1 ,REQUIRE<IS_VAR<UNIT1>>> {
	class Atomic {
	private:
		mutable std::atomic<UNIT1> mValue ;

	public:
		implicit Atomic () = default ;

		implicit Atomic (CREF<Atomic> that) noexcept {
			mValue = that.mValue ;
		}

		inline VREF<Atomic> operator= (CREF<Atomic> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		UNIT1 fetch () const {
			return mValue.load (std::memory_order::memory_order_seq_cst) ;
		}

		void store (CREF<UNIT1> obj) const {
			mValue.store (obj ,std::memory_order::memory_order_seq_cst) ;
		}

		UNIT1 exchange (CREF<UNIT1> obj) const {
			return mValue.exchange (obj ,std::memory_order::memory_order_seq_cst) ;
		}

		BOOL change (VREF<UNIT1> expect ,CREF<UNIT1> obj) const {
			return mValue.compare_exchange_strong (expect ,obj ,std::memory_order::memory_order_seq_cst) ;
		}

		UNIT1 increase () const {
			return mValue.fetch_add (1 ,std::memory_order::memory_order_seq_cst) + 1 ;
		}

		void operator++ (int) const {
			const auto r1x = increase () ;
			noop (r1x) ;
		}

		UNIT1 decrease () const {
			return mValue.fetch_add (1 ,std::memory_order::memory_order_seq_cst) - 1 ;
		}

		void operator-- (int) const {
			const auto r1x = decrease () ;
			noop (r1x) ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using Atomic = typename U::ATOMIC_HELP<UNIT1 ,ALWAYS>::Atomic ;

namespace U {
template <class...>
trait AUTOREF_HELP ;

template <class UNIT1>
trait AUTOREF_HELP<UNIT1 ,REQUIRE<IS_CLONEABLE<UNIT1>>> {
	class AutoRef {
	private:
		Box<UNIT1> mThis ;

	public:
		implicit AutoRef () {
			mThis = Box<UNIT1>::make () ;
		}

		template <class...ARGS>
		imports AutoRef make (XREF<ARGS>...objs) {
			AutoRef ret = AutoRef (PH0) ;
			ret.mThis = Box<UNIT1>::make (forward[TYPEAS<ARGS>::id] (objs)...) ;
			return move (ret) ;
		}

		implicit AutoRef (CREF<AutoRef> that) :AutoRef (PH0) {
			if ifnot (that.exist ())
				return ;
			mThis = that.mThis.clone () ;
		}

		implicit VREF<AutoRef> operator= (CREF<AutoRef> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit AutoRef (RREF<AutoRef> that) :AutoRef (PH0) {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		implicit VREF<AutoRef> operator= (RREF<AutoRef> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		VREF<UNIT1> to () leftvalue {
			return mThis.self ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			return mThis.self ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

	private:
		explicit AutoRef (CREF<typeof (PH0)>) {
			noop () ;
		}
	} ;
} ;

template <class UNIT1>
trait AUTOREF_HELP<UNIT1 ,REQUIRE<ENUM_NOT<IS_CLONEABLE<UNIT1>>>> {
	class AutoRef {
	private:
		Box<UNIT1> mThis ;

	public:
		implicit AutoRef () {
			mThis = Box<UNIT1>::make () ;
		}

		template <class...ARGS>
		imports AutoRef make (XREF<ARGS>...objs) {
			AutoRef ret = AutoRef (PH0) ;
			ret.mThis = Box<UNIT1>::make (forward[TYPEAS<ARGS>::id] (objs)...) ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

		VREF<UNIT1> to () leftvalue {
			return mThis.self ;
		}

		inline implicit operator VREF<UNIT1> () leftvalue {
			return self ;
		}

		inline PTR<VREF<UNIT1>> operator-> () leftvalue {
			return &self ;
		}

		CREF<UNIT1> to () const leftvalue {
			return mThis.self ;
		}

		inline implicit operator CREF<UNIT1> () const leftvalue {
			return self ;
		}

		inline PTR<CREF<UNIT1>> operator-> () const leftvalue {
			return &self ;
		}

	private:
		explicit AutoRef (CREF<typeof (PH0)>) {
			noop () ;
		}
	} ;
} ;
} ;

template <class UNIT1>
using AutoRef = typename U::AUTOREF_HELP<UNIT1 ,ALWAYS>::AutoRef ;

namespace U {
template <class...>
trait ANYREF_HELP ;

template <class...>
trait ANYREFHOLDER_HELP ;

template <>
trait ANYREFHOLDER_HELP<ALWAYS> {
	struct AnyRefHolder implement Interface {
		
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,IS_VOID<UNIT1>> {
	class AnyRef {
	private:
		Box<UNIT1> mThis ;

	public:
		implicit AnyRef () = default ;

		template <class...ARGS>
		imports AnyRef make (XREF<ARGS>...objs) {
			AnyRef ret = AnyRef (PH0) ;
			ret.mThis = Box<UNIT1>::make (forward[TYPEAS<ARGS>::id] (objs)...) ;
			return move (ret) ;
		}

		BOOL exist () const {
			return mThis != NULL ;
		}

	private:
		explicit AnyRef (CREF<typeof (PH0)>) {
			noop () ;
		}
	} ;
} ;

template <class UNIT1>
trait ANYREF_HELP<UNIT1 ,ENUM_NOT<IS_VOID<UNIT1>>> {

} ;
} ;

namespace U {
template <class...>
trait SHAREDREF_HELP ;
} ;

namespace U {
template <class...>
trait FUNCTION_HELP ;

template <class...>
trait FUNCTION_IMPLHOLDER_HELP ;

template <class...>
trait FUNCTION_PUREHOLDER_HELP ;

template <class RETURN ,class...PARAMS>
trait FUNCTION_HELP<RETURN ,TYPEAS<PARAMS...> ,ALWAYS> {
	struct FunctionHolder implement Interface {
		virtual RETURN invoke (XREF<PARAMS>...params) const = 0 ;
	} ;

	using Holder = FunctionHolder ;

	class Function {
	private:
		RC<Holder> mThis ;

	public:
		implicit Function () = default ;

		BOOL exist () const {
			return mThis != NULL ;
		}

		LENGTH rank () const {
			return COUNT_OF<TYPEAS<PARAMS...>>::value ;
		}

		RETURN invoke (XREF<PARAMS>...params) const {
			assert (exist ()) ;
			return mThis->invoke (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}

		inline RETURN operator() (XREF<PARAMS>...params) const {
			return invoke (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}
	} ;
} ;

template <class RETURN ,class...PARAMS ,class UNIT1>
trait FUNCTION_IMPLHOLDER_HELP<RETURN ,TYPEAS<PARAMS...> ,UNIT1 ,ALWAYS> {
	using Holder = typename FUNCTION_HELP<RETURN ,TYPEAS<PARAMS...>>::FunctionHolder ;

	class ImplHolder implement Holder {
	private:
		Box<UNIT1> mFunctor ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<Box<UNIT1>> functor) {
			mFunctor = move (functor) ;
		}

		RETURN invoke (XREF<PARAMS>...params) const override {
			return mFunctor.self (forward[TYPEAS<PARAMS>::id] (params)...) ;
		}
	} ;
} ;
} ;

template <class RETURN ,class PARAMS>
using Function = typename U::FUNCTION_HELP<RETURN ,PARAMS ,ALWAYS>::Function ;

namespace U {
template <class...>
trait UNIQUEREF_HELP ;
} ;

using BUFFER_FLEX_SIZE = ENUMAS<VAR ,U::ENUMID<-1>> ;
using BUFFER_AUTO_SIZE = ENUMAS<VAR ,U::ENUMID<-2>> ;
using BUFFER_FIXX_SIZE = ENUMAS<VAR ,U::ENUMID<-3>> ;
using BUFFER_PHAN_SIZE = ENUMAS<VAR ,U::ENUMID<-4>> ;

namespace U {
template <class...>
trait BUFFER_HELP ;

template <class ITEM ,class SIZE>
trait BUFFER_HELP<ITEM ,SIZE ,REQUIRE<ENUM_GT_ZERO<SIZE>>> {
	class Buffer {
	private:
		DEF<ITEM[SIZE::value]> mBuffer ;

	public:
		implicit Buffer () = default ;

		LENGTH size () const {
			return SIZE::value ;
		}

		LENGTH addr () const {
			return address (mBuffer) ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mBuffer[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
			return mBuffer[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			assert (FALSE) ;
		}
	} ;
} ;

template <class...>
trait BUFFERHOLDER_HELP ;

template <>
trait BUFFERHOLDER_HELP<ALWAYS> {
	struct BufferHolder implement Interface {
		virtual void finalize () = 0 ;
		virtual void destroy () = 0 ;
	} ;
} ;

template <class ITEM>
trait BUFFER_HELP<ITEM ,BUFFER_AUTO_SIZE ,REQUIRE<IS_CLONEABLE<ITEM>>> {
	using Holder = typename BUFFERHOLDER_HELP<ALWAYS>::BufferHolder ;

	class Buffer {
	private:
		PTR<VREF<Holder>> mHolder ;
		PTR<VREF<ITEM[]>> mBuffer ;
		LENGTH mSize ;

	public:
		implicit Buffer () noexcept {
			mHolder = NULL ;
			mBuffer = NULL ;
			mSize = 0 ;
		}

		explicit Buffer (CREF<LENGTH> size_) :Buffer () {
			
		}

		implicit ~Buffer () noexcept {
			if (mHolder == NULL)
				return ;
			mHolder->finalize () ;
			mHolder->destroy () ;
			mHolder = NULL ;
			mBuffer = NULL ;
			mSize = 0 ;
		}

		implicit Buffer (CREF<Buffer> that) :Buffer () {
		
		}

		inline VREF<Buffer> operator= (CREF<Buffer> that) {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		implicit Buffer (RREF<Buffer> that) noexcept :Buffer () {
			auto &&thiz = *this ;
			swap (thiz ,that) ;
		}

		inline VREF<Buffer> operator= (RREF<Buffer> that) noexcept {
			auto &&thiz = *this ;
			swap (thiz ,move (that)) ;
			return thiz ;
		}

		LENGTH size () const {
			return mSize ;
		}

		LENGTH addr () const {
			return address ((*mBuffer)) ;
		}

		VREF<ITEM> at (CREF<INDEX> index) leftvalue {
			assert (between (index ,0 ,size ())) ;
			return (*mBuffer)[index] ;
		}

		inline VREF<ITEM> operator[] (CREF<INDEX> index) leftvalue {
			return at (index) ;
		}

		CREF<ITEM> at (CREF<INDEX> index) const leftvalue {
			assert (between (index ,0 ,size ())) ;
			return (*mBuffer)[index] ;
		}

		inline CREF<ITEM> operator[] (CREF<INDEX> index) const leftvalue {
			return at (index) ;
		}

		void resize (CREF<LENGTH> size_) {
			assert (FALSE) ;
		}
	} ;
} ;
} ;

template <class ITEM ,class SIZE>
using Buffer = typename U::BUFFER_HELP<ITEM ,SIZE ,ALWAYS>::Buffer ;

template <class ITEM>
using AutoBuffer = typename U::BUFFER_HELP<ITEM ,BUFFER_AUTO_SIZE ,ALWAYS>::Buffer ;

namespace U {
template <class...>
trait ALLOCATOR_HELP ;
} ;

namespace U {
template <class...>
trait INTEGER_HELP ;
} ;

namespace U {
template <class...>
trait TUPLE_HELP ;
} ;

namespace U {
template <class...>
trait VARIANT_HELP ;
} ;

namespace U {
template <class...>
trait LATER_HELP ;
} ;
} ;