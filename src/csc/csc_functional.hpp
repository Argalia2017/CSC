#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_FUNCTIONAL__
#define __CSC_FUNCTIONAL__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"
#include "csc_geometry.hpp"
#include "csc_algorithm.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"

namespace CSC {
template <class...>
trait SYNTAXTREE_HELP ;

template <class...>
trait SYNTAXTREE_IMPLHOLDER_HELP ;

template <class...>
trait SYNTAXTREE_IMPLBINDER_HELP ;

template <class DEPEND>
trait SYNTAXTREE_HELP<DEPEND ,ALWAYS> {
	class SyntaxTree ;

	struct Binder implement Interface {
		virtual AutoRef<> friend_create (VREF<SyntaxTree> tree) const = 0 ;
	} ;

	struct Holder implement Interface {
		imports VRef<Holder> create () ;

		virtual void initialize () = 0 ;
		virtual void mark_as_function () = 0 ;
		virtual void mark_as_iteration () = 0 ;
		virtual void maybe (CREF<Clazz> clazz ,CREF<Binder> binder) = 0 ;
		virtual CREF<AutoRef<>> stack (CREF<Clazz> clazz ,CREF<Binder> binder) leftvalue = 0 ;
		virtual void once (RREF<Function<void>> actor) = 0 ;
		virtual void then (RREF<Function<void>> actor) = 0 ;
		virtual void undo (CREF<Clazz> clazz) = 0 ;
		virtual CREF<AutoRef<>> later () leftvalue = 0 ;
		virtual VREF<AutoRef<>> later (CREF<Clazz> clazz) leftvalue = 0 ;
		virtual void play () = 0 ;
		virtual void clean () = 0 ;
	} ;

	struct Layout {
		VRef<Holder> mThis ;
	} ;

	class SyntaxTree implement Layout {
	protected:
		using Layout::mThis ;

	public:
		implicit SyntaxTree () = default ;

		implicit SyntaxTree (RREF<Layout> that) {
			mThis = move (that.mThis) ;
		}

		explicit SyntaxTree (CREF<BoolProxy> ok) {
			mThis = Holder::create () ;
			mThis->initialize () ;
		}

		void mark_as_function () {
			return mThis->mark_as_function () ;
		}

		void mark_as_iteration () {
			return mThis->mark_as_iteration () ;
		}

		template <class ARG1>
		void maybe (TYPEID<ARG1> id) {
			using R1X = typename KILL<SYNTAXTREE_IMPLBINDER_HELP<ARG1 ,ALWAYS> ,DEPEND>::ImplBinder ;
			const auto r1x = Clazz (id) ;
			auto rax = R1X () ;
			return mThis->maybe (r1x ,rax) ;
		}

		template <class ARG1>
		CREF<ARG1> stack (TYPEID<ARG1> id) leftvalue {
			using R1X = typename KILL<SYNTAXTREE_IMPLBINDER_HELP<ARG1 ,ALWAYS> ,DEPEND>::ImplBinder ;
			const auto r1x = Clazz (id) ;
			auto rax = R1X () ;
			return AutoRef<ARG1>::from (mThis->stack (r1x ,rax)).self ;
		}

		void once (RREF<Function<void>> actor) {
			return mThis->once (move (actor)) ;
		}

		void then (RREF<Function<void>> actor) {
			return mThis->then (move (actor)) ;
		}

		template <class ARG1>
		void undo (TYPEID<ARG1> id) {
			const auto r1x = Clazz (id) ;
			return mThis->undo (r1x) ;
		}

		template <class ARG1>
		CREF<ARG1> later (TYPEID<ARG1> id) leftvalue {
			return AutoRef<ARG1>::from (mThis->later ()).self ;
		}

		template <class ARG1 ,class ARG2>
		void later (TYPEID<ARG1> id ,XREF<ARG2> value_) {
			using R1X = REMOVE_REF<ARG2> ;
			const auto r1x = Clazz (id) ;
			auto &&tmp = mThis->later (r1x) ;
			auto rax = AutoRef<R1X> (move (tmp)) ;
			if ifswitch (TRUE) {
				if (rax.good ())
					discard ;
				rax = AutoRef<R1X>::make () ;
			}
			rax.self = forward[TYPEAS<ARG2>::expr] (value_) ;
			tmp = move (rax) ;
		}

		void play () {
			return mThis->play () ;
		}

		void clean () {
			return mThis->clean () ;
		}
	} ;
} ;

template <class UNIT>
trait SYNTAXTREE_IMPLBINDER_HELP<UNIT ,ALWAYS> {
	using Binder = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::Binder ;
	using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;

	class ImplBinder implement Binder {
	protected:
		CRef<UNIT> mThat ;

	public:
		AutoRef<> friend_create (VREF<SyntaxTree> tree) const override {
			return AutoRef<UNIT>::make (tree) ;
		}
	} ;
} ;

using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;

template <class...>
trait OPERAND_HELP ;

template <class RANK>
trait OPERAND_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	class Operand ;
} ;

template <class RANK>
trait OPERAND_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	class Operand ;
} ;
} ;