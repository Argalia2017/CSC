#pragma once

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
trait SYNTAXTREE_CREATOR_HELP ;

template <class DEPEND>
trait SYNTAXTREE_HELP<DEPEND ,ALWAYS> {
	struct Binder implement Interface {
		virtual AutoRef<> friend_create () = 0 ;
	} ;

	struct Holder implement Interface {
		virtual void initialize () = 0 ;
		virtual void mark_as_function () = 0 ;
		virtual void mark_as_iteration () = 0 ;
		virtual void maybe (CREF<Clazz> clazz ,VREF<Binder> op) = 0 ;
		virtual CREF<AutoRef<>> stack (CREF<Clazz> clazz ,VREF<Binder> op) leftvalue = 0 ;
		virtual CREF<AutoRef<>> later () leftvalue = 0 ;
		virtual void once (RREF<Function<void>> actor) = 0 ;
		virtual void then (RREF<Function<void>> actor) = 0 ;
		virtual void undo (CREF<Clazz> clazz) = 0 ;
		virtual VREF<AutoRef<>> redo (CREF<Clazz> clazz) leftvalue = 0 ;
		virtual void play () = 0 ;
		virtual void clean () = 0 ;
	} ;

	struct FUNCTION_extern {
		imports VRef<Holder> invoke () ;
	} ;

	class SyntaxTree {
	protected:
		VRef<Holder> mThis ;

	public:
		implicit SyntaxTree () {
			mThis = FUNCTION_extern::invoke () ;
			mThis->initialize () ;
		}

		void mark_as_function () {
			return mThis->mark_as_function () ;
		}

		void mark_as_iteration () {
			return mThis->mark_as_iteration () ;
		}

		template <class ARG1>
		void maybe (CREF<TYPEID<ARG1>> id) {
			using R1X = typename SYNTAXTREE_CREATOR_HELP<ARG1 ,ALWAYS>::Creator ;
			const auto r1x = Clazz (id) ;
			auto rax = R1X (VRef<SyntaxTree>::reference (thiz)) ;
			return mThis->maybe (r1x ,rax) ;
		}

		template <class ARG1>
		CREF<ARG1> stack (CREF<TYPEID<ARG1>> id) leftvalue {
			using R1X = typename SYNTAXTREE_CREATOR_HELP<ARG1 ,ALWAYS>::Creator ;
			const auto r1x = Clazz (id) ;
			auto rax = R1X (VRef<SyntaxTree>::reference (thiz)) ;
			auto &&tmp = mThis->stack (r1x ,rax).rebind (id) ;
			return tmp.self ;
		}

		template <class ARG1>
		CREF<ARG1> later (CREF<TYPEID<ARG1>> id) leftvalue {
			auto &&tmp = mThis->later ().rebind (TYPEAS<CRef<ARG1>>::expr) ;
			return tmp.self ;
		}

		void once (RREF<Function<void>> actor) {
			return mThis->once (move (actor)) ;
		}

		void then (RREF<Function<void>> actor) {
			return mThis->then (move (actor)) ;
		}

		template <class ARG1>
		void undo (CREF<TYPEID<ARG1>> id) {
			const auto r1x = Clazz (id) ;
			return mThis->undo (r1x) ;
		}

		template <class ARG1 ,class ARG2>
		void redo (CREF<TYPEID<ARG1>> id ,CREF<ARG2> refer) {
			const auto r1x = Clazz (id) ;
			auto &&tmp = mThis->redo (r1x) ;
			if ifswitch (TRUE) {
				if (tmp.exist ())
					if (tmp.clazz () == r1x)
						discard ;
				tmp = AutoRef<CRef<ARG2>>::make () ;
			}
			tmp.rebind (TYPEAS<CRef<ARG2>>::expr).self = CRef<ARG2>::reference (refer) ;
		}

		template <class ARG1 ,class ARG2>
		void redo (CREF<TYPEID<ARG1>> ,RREF<ARG2>) = delete ;

		void play () {
			return mThis->play () ;
		}

		void clean () {
			return mThis->clean () ;
		}
	} ;
} ;

template <class UNIT>
trait SYNTAXTREE_CREATOR_HELP<UNIT ,ALWAYS> {
	using Binder = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::Binder ;
	using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;

	class Creator implement Binder {
	protected:
		VRef<SyntaxTree> mContext ;

	public:
		implicit Creator () = delete ;

		explicit Creator (RREF<VRef<SyntaxTree>> context) {
			mContext = move (context) ;
		}

		AutoRef<> friend_create () override {
			return AutoRef<UNIT>::make (mContext.self) ;
		}
	} ;
} ;

using SyntaxTree = typename SYNTAXTREE_HELP<DEPEND ,ALWAYS>::SyntaxTree ;

template <class...>
trait OPERAND_HELP ;

template <class...>
trait OPERAND_HOLDER_HELP ;

template <class DEPEND>
trait OPERAND_HOLDER_HELP<DEPEND ,ALWAYS> {
	struct NODE {

	} ;

	struct HEAP {
		Allocator<NODE ,VARIABLE> mList ;
	} ;

	class Operand {
	protected:
		SharedRef<HEAP> mHeap ;
		INDEX mIndex ;

	public:
		implicit Operand () {
			mIndex = NONE ;
		}
	} ;
} ;

template <class RANK>
trait OPERAND_HELP<RANK ,REQUIRE<ENUM_EQ_ZERO<RANK>>> {
	using SUPER = typename OPERAND_HOLDER_HELP<DEPEND ,ALWAYS>::Operand ;

	class Operand extend SUPER {
	protected:
		using SUPER::mHeap ;
		using SUPER::mIndex ;

	public:
		implicit Operand () = default ;

		LENGTH rank () const {
			return RANK::value ;
		}

		template <class ARG1>
		CREF<ARG1> eval (CREF<TYPEID<ARG1>> id) const leftvalue {
			unimplemented () ;
			return bad (TYPEAS<ARG1>::expr) ;
		}
	} ;
} ;

template <class RANK>
trait OPERAND_HELP<RANK ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using SUPER = typename OPERAND_HOLDER_HELP<DEPEND ,ALWAYS>::Operand ;

	using ITEM = typename OPERAND_HELP<ENUM_DEC<RANK> ,ALWAYS>::Operand ;

	class Operand extend SUPER {
	protected:
		using SUPER::mHeap ;
		using SUPER::mIndex ;

	public:
		implicit Operand () = default ;

		LENGTH rank () const {
			return RANK::value ;
		}

		LENGTH size () const {
			unimplemented () ;
			return bad (TYPEAS<LENGTH>::expr) ;
		}

		ITEM at (CREF<INDEX> index) const {
			unimplemented () ;
			return bad (TYPEAS<ITEM>::expr) ;
		}

		inline ITEM operator[] (CREF<INDEX> index) const {
			return at (index) ;
		}
	} ;
} ;

template <class...>
trait OPERATOR_HELP ;

template <class RANK ,class ITEM>
trait OPERATOR_HELP<RANK ,ITEM ,REQUIRE<ENUM_GT_ZERO<RANK>>> {
	using SUPER = typename OPERAND_HOLDER_HELP<DEPEND ,ALWAYS>::Operand ;
	using Operand = ITEM ;

	class Operator extend SUPER {
	protected:
		using SUPER::mHeap ;
		using SUPER::mIndex ;

	public:
		implicit Operator () = default ;

		LENGTH rank () const {
			return RANK::value ;
		}

		Operand invoke () const {
			unimplemented () ;
			return bad (TYPEAS<Operand>::expr) ;
		}

		inline Operand operator() () const {
			return invoke () ;
		}
	} ;
} ;

template <class RANK ,class ITEM>
trait OPERATOR_HELP<RANK ,ITEM ,REQUIRE<ENUM_EQ_IDEN<RANK>>> {
	using SUPER = typename OPERAND_HOLDER_HELP<DEPEND ,ALWAYS>::Operand ;
	using Operand = ITEM ;

	template <class ARG1>
	using CastOperator = typename DEPENDENT<OPERATOR_HELP<ARG1 ,Operand ,ALWAYS> ,DEPEND>::Operator ;

	template <class ARG1>
	using PartOperator = CastOperator<ENUM_SUB<RANK ,COUNT_OF<ARG1>>> ;

	template <class ARG1>
	using ConcatOperator = CastOperator<ENUM_ADD<ENUM_DEC<RANK> ,ARG1>> ;

	class Operator extend SUPER {
	protected:
		using SUPER::mHeap ;
		using SUPER::mIndex ;

	public:
		implicit Operator () = default ;

		LENGTH rank () const {
			return RANK::value ;
		}

		PartOperator<TYPEAS<Operand>> part (CREF<Operand> obj) const {
			unimplemented () ;
			return bad (TYPEAS<PartOperator<TYPEAS<Operand>>>::expr) ;
		}

		template <class ARG1>
		ConcatOperator<ARG1> concat (CREF<TYPEID<ARG1>> id ,CREF<CastOperator<ARG1>> that) const {
			unimplemented () ;
			return bad (TYPEAS<ConcatOperator<ARG1>>::expr) ;
		}

		template <class ARG1>
		Operator flip (CREF<TYPEID<ARG1>> id) const {
			require (ENUM_EQUAL<ARG1 ,RANK0>) ;
			return thiz ;
		}

		Operator curry () const {
			return thiz ;
		}

		Operand invoke (CREF<Operand> params) const {
			unimplemented () ;
			return bad (TYPEAS<Operand>::expr) ;
		}

		inline Operand operator() (CREF<Operand> params) const {
			return invoke (params) ;
		}
	} ;
} ;

template <class RANK ,class ITEM>
trait OPERATOR_HELP<RANK ,ITEM ,REQUIRE<ENUM_GT_IDEN<RANK>>> {
	using SUPER = typename OPERAND_HOLDER_HELP<DEPEND ,ALWAYS>::Operand ;
	using Operand = ITEM ;

	template <class ARG1>
	using CastOperator = typename DEPENDENT<OPERATOR_HELP<ARG1 ,Operand ,ALWAYS> ,DEPEND>::Operator ;

	template <class ARG1>
	using PartOperator = CastOperator<ENUM_SUB<RANK ,COUNT_OF<ARG1>>> ;

	template <class ARG1>
	using ConcatOperator = CastOperator<ENUM_ADD<ENUM_DEC<RANK> ,ARG1>> ;

	class Operator extend SUPER {
	protected:
		using SUPER::mHeap ;
		using SUPER::mIndex ;

	public:
		implicit Operator () = default ;

		LENGTH rank () const {
			return RANK::value ;
		}

		template <class...ARG1>
		PartOperator<TYPEAS<Operand ,ARG1...>> part (CREF<Operand> obj1 ,CREF<ARG1>...obj2) const {
			require (ENUM_COMPR_LTEQ<COUNT_OF<TYPEAS<Operand ,ARG1...>> ,RANK>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,Operand>...>) ;
			using R1X = TYPEAS<Operand ,ARG1...> ;
			unimplemented () ;
			return bad (TYPEAS<PartOperator<R1X>>::expr) ;
		}

		template <class ARG1>
		ConcatOperator<ARG1> concat (CREF<TYPEID<ARG1>> id ,CREF<CastOperator<ARG1>> that) const {
			unimplemented () ;
			return bad (TYPEAS<ConcatOperator<ARG1>>::expr) ;
		}

		template <class...ARG1>
		Operator flip (CREF<TYPEID<ARG1>>...id) const {
			require (ENUM_EQUAL<COUNT_OF<TYPEAS<ARG1...>> ,RANK>) ;
			unimplemented () ;
			return thiz ;
		}

		CastOperator<RANK1> curry () const {
			unimplemented () ;
			return bad (TYPEAS<CastOperator<RANK1>>::expr) ;
		}

		template <class...ARG1>
		Operand invoke (CREF<Operand> params1 ,CREF<ARG1>...params2) const {
			require (ENUM_EQUAL<COUNT_OF<TYPEAS<Operand ,ARG1...>> ,RANK>) ;
			require (ENUM_ALL<IS_SAME<ARG1 ,Operand>...>) ;
			unimplemented () ;
			return bad (TYPEAS<Operand>::expr) ;
		}

		template <class...ARG1>
		inline Operand operator() (CREF<Operand> params1 ,CREF<ARG1>...params2) const {
			return invoke (params1 ,params2...) ;
		}
	} ;
} ;
} ;