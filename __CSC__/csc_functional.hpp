#pragma once

#ifndef __CSC_FUNCTIONAL__
#define __CSC_FUNCTIONAL__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
class Operand ;

namespace U {
template <class _ARG1>
using RANK_FUNC_TYPE = FUNCTION_BIND_TYPE<Operand ,REPEAT_PARAMS_TYPE<_ARG1 ,const Operand &>> ;
} ;

using RANK0 = U::RANK_FUNC_TYPE<ZERO> ;
using RANK1 = U::RANK_FUNC_TYPE<ARGC<1>> ;
using RANK2 = U::RANK_FUNC_TYPE<ARGC<2>> ;
using RANK3 = U::RANK_FUNC_TYPE<ARGC<3>> ;
using RANK4 = U::RANK_FUNC_TYPE<ARGC<4>> ;
using RANK5 = U::RANK_FUNC_TYPE<ARGC<5>> ;
using RANK6 = U::RANK_FUNC_TYPE<ARGC<6>> ;
using RANK7 = U::RANK_FUNC_TYPE<ARGC<7>> ;
using RANK8 = U::RANK_FUNC_TYPE<ARGC<8>> ;
using RANK9 = U::RANK_FUNC_TYPE<ARGC<9>> ;

class LexicalNode ;

class Operand {
private:
	struct THIS_PACK {
		AnyRef<> mHolder ;
	} ;

private:
	SharedRef<THIS_PACK> mThis ;

public:
	implicit Operand () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<U::CONSTEXPR_NOT<IS_PLACEHOLDER_HELP<_ARG1>> ,U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Operand>>>>>
	implicit Operand (_ARG1 &&that) {
		mThis = SharedRef<THIS_PACK>::make () ;
		mThis->mHolder = AnyRef<REMOVE_CVR_TYPE<_ARG1>>::make (_FORWARD_ (ARGV<_ARG1 &&>::ID ,that)) ;
	}

	BOOL exist () const {
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1>
	BOOL available (const ARGVF<_ARG1> &) const {
		_STATIC_ASSERT_ (U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Operand>>::compile ()) ;
		if (!exist ())
			return FALSE ;
		const auto r1x = mThis->mHolder.type_mid () ;
		const auto r2x = _TYPEMID_ (ARGV<_ARG1>::ID) ;
		if (r1x != r2x)
			return FALSE ;
		return TRUE ;
	}

	BOOL equal (const Operand &that) const {
		if (_EBOOL_ (mThis.exist ()) != _EBOOL_ (that.mThis.exist ()))
			return FALSE ;
		if (!mThis.exist ())
			return TRUE ;
		if (DEPTR[mThis.self] != DEPTR[that.mThis.self])
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Operand &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Operand &that) const {
		return !equal (that) ;
	}

	template <class _ARG1>
	const _ARG1 &as (const ARGVF<_ARG1> &) const leftvalue {
		return template_as (ARGV<REMOVE_CVR_TYPE<_ARG1>>::ID) ;
	}

	template <class _ARG1>
	imports const Operand &nth (const ARGV<ARGVP<_ARG1>> &) {
		return _CACHE_ ([&] () {
			return Operand (ARGV<ARGVP<_ARG1>> {}) ;
		}) ;
	}

private:
	template <class _ARG1>
	const _ARG1 &template_as (const ARGVF<_ARG1> &) const leftvalue {
		_DYNAMIC_ASSERT_ (available (ARGV<_ARG1>::ID)) ;
		return mThis->mHolder.rebind (ARGV<REMOVE_CVR_TYPE<_ARG1>>::ID).self ;
	}

	const Operand &template_as (const ARGVF<Operand> &) const leftvalue {
		return DEREF[this] ;
	}
} ;

class Operator {
private:
	struct Private {
		class DefHolder ;

		template <class ,class>
		class ImplHolder ;
	} ;

	class Holder :
		delegate public Interface {
	public:
		virtual LENGTH rank () const = 0 ;
		virtual Operand invoke (const LexicalNode &node) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const = 0 ;
	} ;

	struct THIS_PACK {
		AnyRef<> mHolder ;
		PTR<Holder> mPointer ;
	} ;

private:
	SharedRef<THIS_PACK> mThis ;

public:
	implicit Operator () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<U::CONSTEXPR_NOT<IS_PLACEHOLDER_HELP<_ARG1>> ,U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Operator>>>>>
	implicit Operator (_ARG1 &&that) {
		using R1X = FUNCTION_OF_TYPE<_ARG1> ;
		using R2X = REPEAT_PARAMS_TYPE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<R1X>> ,Operand> ;
		_STATIC_ASSERT_ (U::CONSTEXPR_NOT<IS_REFERENCE_HELP<_ARG1>>::compile ()) ;
		using R3X = typename DEPENDENT_TYPE<Private ,struct ANONYMOUS>::template ImplHolder<R1X ,R2X> ;
		mThis = SharedRef<THIS_PACK>::make () ;
		auto rax = Function<R1X> (_FORWARD_ (ARGV<_ARG1 &&>::ID ,that)) ;
		mThis->mHolder = AnyRef<R3X>::make (ARGVP0 ,_MOVE_ (rax)) ;
		mThis->mPointer = DEPTR[mThis->mHolder.rebind (ARGV<R3X>::ID).self] ;
	}

	LENGTH rank () const {
		if (!mThis.exist ())
			return VAR_NONE ;
		return mThis->mPointer->rank () ;
	}

	template <class... _ARGS>
	Operand invoke (const LexicalNode &node ,const _ARGS &...funcval) const {
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) <= 9) ;
		_DYNAMIC_ASSERT_ (mThis.exist ()) ;
		return mThis->mPointer->invoke (node ,funcval...) ;
	}

	template <class... _ARGS>
	inline Operand operator() (const LexicalNode &node ,const _ARGS &...funcval) const {
		return invoke (node ,funcval...) ;
	}
} ;

class Operator::Private::DefHolder :
	delegate public Holder {
public:
	implicit DefHolder () = default ;

	Operand invoke (const LexicalNode &node) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}
} ;

template <class UNIT1 ,class... UNITS1 ,class... UNITS2>
class Operator::Private::ImplHolder<DEF<UNIT1 (UNITS1...)> ,ARGVS<UNITS2...>> :
	delegate public DefHolder {
private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS1...>) == _CAPACITYOF_ (ARGVS<UNITS2...>)) ;

private:
	Function<UNIT1 (UNITS1...)> mFunctor ;

public:
	implicit ImplHolder () = delete ;

	template <class... _ARGS>
	explicit ImplHolder (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval) :
		delegate mFunctor (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) {}

	LENGTH rank () const override {
		return _CAPACITYOF_ (ARGVS<UNITS2...>) ;
	}

	Operand invoke (const LexicalNode &node ,FORWARD_TRAITS_TYPE<UNITS2> &&...funcval) const {
		const auto r1x = TupleBinder<const UNITS2...> (funcval...) ;
		auto rax = template_invoke (r1x ,ARGV<ARGVS<UNITS1...>>::ID) ;
		return Operand (_MOVE_ (rax)) ;
	}

private:
	UNIT1 template_invoke (const Tuple<> &parameter ,const ARGVF<ARGVS<>> & ,FORWARD_TRAITS_TYPE<UNITS1> &&...funcval) const {
		return mFunctor (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS1> &&>::ID ,funcval)...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	UNIT1 template_invoke (const _ARG1 &parameter ,const ARGVF<_ARG2> & ,_ARGS &&...funcval) const {
		using R1X = PARAMS_ONE_TYPE<_ARG2> ;
		using R2X = PARAMS_REST_TYPE<_ARG2> ;
		auto &r1x = parameter.one ().as (ARGV<R1X>::ID) ;
		return template_invoke (parameter.rest () ,ARGV<R2X>::ID ,_FORWARD_ (ARGV<_ARGS &&>::ID ,funcval)... ,r1x) ;
	}
} ;

template <class UNIT1 ,class... UNITS1 ,class... UNITS2>
class Operator::Private::ImplHolder<DEF<UNIT1 (const LexicalNode & ,UNITS1...)> ,ARGVS<Operand ,UNITS2...>> :
	delegate public DefHolder {
private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS1...>) == _CAPACITYOF_ (ARGVS<UNITS2...>)) ;

private:
	Function<UNIT1 (const LexicalNode & ,UNITS1...)> mFunctor ;

public:
	implicit ImplHolder () = delete ;

	template <class... _ARGS>
	explicit ImplHolder (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval) :
		delegate mFunctor (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) {}

	LENGTH rank () const override {
		return _CAPACITYOF_ (ARGVS<UNITS2...>) ;
	}

	Operand invoke (const LexicalNode &node ,FORWARD_TRAITS_TYPE<UNITS2> &&...funcval) const override {
		const auto r1x = TupleBinder<const UNITS2...> (funcval...) ;
		auto rax = template_invoke (r1x ,ARGV<ARGVS<UNITS1...>>::ID ,node) ;
		return Operand (_MOVE_ (rax)) ;
	}

private:
	UNIT1 template_invoke (const Tuple<> &parameter ,const ARGVF<ARGVS<>> & ,const LexicalNode &node ,FORWARD_TRAITS_TYPE<UNITS1> &&...funcval) const {
		return mFunctor (node ,_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS1> &&>::ID ,funcval)...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	UNIT1 template_invoke (const _ARG1 &parameter ,const ARGVF<_ARG2> & ,_ARGS &&...funcval) const {
		using R1X = PARAMS_ONE_TYPE<_ARG2> ;
		using R2X = PARAMS_REST_TYPE<_ARG2> ;
		auto &r1x = parameter.one ().as (ARGV<R1X>::ID) ;
		return template_invoke (parameter.rest () ,ARGV<R2X>::ID ,_FORWARD_ (ARGV<_ARGS &&>::ID ,funcval)... ,r1x) ;
	}
} ;

template <class RANK ,class RETR = Operand>
class Expression ;

namespace U {
template <class _ARG1>
struct RETR_FUNC {
	_STATIC_ASSERT_ (U::CONSTEXPR_COMPR_GT<U::CONSTEXPR_DECREASE<_ARG1> ,ZERO>::compile ()) ;
	using R1X = ARGC_TYPE<U::CONSTEXPR_DECREASE<_ARG1>> ;
	using R2X = typename RETR_FUNC<R1X>::TYPE ;
	using TYPE = Expression<RANK1 ,R2X> ;
} ;

template <>
struct RETR_FUNC<ZERO> {
	using TYPE = Operand ;
} ;

template <>
struct RETR_FUNC<ARGC<1>> {
	using TYPE = Operand ;
} ;

template <class _ARG1>
using RETR_FUNC_TYPE = typename RETR_FUNC<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>::TYPE ;
} ;

class LexicalNode {
private:
	struct NODE_PACK {
		Operand mOperand ;
		Operator mOperator ;
		LENGTH mRank ;
		LENGTH mDepth ;
		INDEX mCurr ;
		ARRAY3<INDEX> mChild ;
	} ;

private:
	template <class ,class>
	friend class Expression ;
	SharedRef<List<NODE_PACK>> mTree ;
	INDEX mCurr ;

public:
	implicit LexicalNode () = default ;

	explicit LexicalNode (const SharedRef<List<NODE_PACK>> &tree) {
		mTree = tree ;
		mCurr = mTree->insert () ;
		mTree.self[mCurr].mCurr = mCurr ;
		mTree.self[mCurr].mRank = VAR_NONE ;
		mTree.self[mCurr].mDepth = VAR_NONE ;
	}

	explicit LexicalNode (const SharedRef<List<NODE_PACK>> &tree ,const INDEX &curr) {
		_DEBUG_ASSERT_ (tree.self[curr].mCurr == curr) ;
		mTree = tree ;
		mCurr = curr ;
	}

	NODE_PACK &to () leftvalue {
		_DYNAMIC_ASSERT_ (mTree.exist ()) ;
		return mTree.self[mCurr] ;
	}

	inline implicit operator NODE_PACK & () leftvalue {
		return self ;
	}

	inline PTR<NODE_PACK> operator-> () leftvalue {
		_DYNAMIC_ASSERT_ (mTree.exist ()) ;
		return DEPTR[self] ;
	}

	const NODE_PACK &to () const leftvalue {
		return mTree.self[mCurr] ;
	}

	inline implicit operator const NODE_PACK & () const leftvalue {
		return self ;
	}

	inline PTR<const NODE_PACK> operator-> () const leftvalue {
		return DEPTR[self] ;
	}
} ;

namespace U {
template <class _ARG1>
struct CONSTEXPR_MAXOF {
	imports constexpr VAR compile () {
		using R1X = PARAMS_ONE_TYPE<_ARG1> ;
		using R2X = CONSTEXPR_MAXOF<PARAMS_REST_TYPE<_ARG1>> ;
		using R3X = CONSTEXPR_MAX<R1X ,R2X> ;
		return R3X::compile () ;
	}
} ;

template <class _ARG1>
struct CONSTEXPR_MAXOF<ARGVS<_ARG1>> {
	imports constexpr VAR compile () {
		return _ARG1::compile () ;
	}
} ;
} ;

template <class RETR ,class... UNITS>
class Expression<SPECIALIZATION<Operand (UNITS...)> ,RETR> {
protected:
	using RANK = DEF<Operand (UNITS...)> ;
	using RANK_SIZE = CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>> ;

	_STATIC_ASSERT_ (U::CONSTEXPR_RANGE_CHECK<RANK_SIZE ,ZERO ,ARGC<10>>::compile ()) ;

protected:
	template <class ,class>
	friend class Expression ;
	LexicalNode mThis ;

public:
	implicit Expression () :
		delegate Expression (ARGVP0) {
		_NOOP_ () ;
	}

	explicit Expression (const LexicalNode &node) {
		_DYNAMIC_ASSERT_ (node->mRank == RANK_SIZE::compile ()) ;
		mThis = node ;
	}

	const RETR &invoke (const UNITS &...funcval) const leftvalue {
		_DYNAMIC_ASSERT_ (mThis->mOperator.rank () == RANK_SIZE::compile ()) ;
		mThis->mOperand = mThis->mOperator (mThis.self ,funcval...) ;
		return mThis->mOperand.as (ARGV<RETR>::ID) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK ,RETR>>>
	_RET flip () const {
		_STATIC_ASSERT_ (U::CONSTEXPR_COMPR_LT_EQ<RANK_SIZE ,ARGC<2>>::compile ()) ;
		return template_flip () ;
	}

	template <class... _ARGS ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<ARGVS<_ARGS...>>> ,RETR>>>
	_RET flips (const ARGV<ARGVP<_ARGS>> &...) const {
		using R1X = U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<ARGVS<_ARGS...>>> ;
		_STATIC_ASSERT_ (U::CONSTEXPR_EQUAL<CAPACITY_OF_TYPE<ARGVS<_ARGS...>> ,RANK_SIZE>::compile ()) ;
		return template_flips (ARGV<R1X>::ID ,ARGV<ARGVS<_ARGS...>>::ID ,ARGV<FUNCTION_PARAMS_TYPE<R1X>>::ID) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK0 ,Expression<RANK ,RETR>>>>
	_RET fold () const {
		Expression<RANK0 ,Expression<RANK ,RETR>> ret ;
		ret.mThis = LexicalNode (mThis.mTree) ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node) {
			using R1X = U::RANK_FUNC_TYPE<CAPACITY_OF_TYPE<ARGVS<>>> ;
			_STATIC_ASSERT_ (IS_SAME_HELP<RANK0 ,R1X>::compile ()) ;
			const auto r1x = Expression<RANK ,RETR> (LexicalNode (node.mTree ,node->mChild[0])) ;
			_STATIC_ASSERT_ (IS_SAME_HELP<DEF<decltype (r1x)> ,Expression<RANK ,RETR>>::compile ()) ;
			return Operand (r1x) ;
		}) ;
		ret.mThis->mChild[0] = mThis->mCurr ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK1 ,U::RETR_FUNC_TYPE<RANK>>>>
	_RET curry () const {
		return template_curry (ARGV<RANK>::ID) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<RANK_SIZE> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	_RET bind (const Expression<_ARG1 ,_ARG2> &that) const {
		using R1X = U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<RANK_SIZE> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ;
		return template_bind (ARGV<R1X>::ID ,that ,ARGV<FUNCTION_PARAMS_TYPE<R1X>>::ID) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<ARGVS<U::CONSTEXPR_DECREASE<RANK_SIZE> ,ZERO>>> ,Expression<_ARG1 ,RETR>>>>
	_RET link (const Expression<RANK0 ,Expression<_ARG1 ,_ARG2>> &that) const {
		using R1X = U::RANK_FUNC_TYPE<U::CONSTEXPR_DECREASE<RANK_SIZE>> ;
		return template_link (ARGV<R1X>::ID ,that ,ARGV<FUNCTION_PARAMS_TYPE<R1X>>::ID) ;
	}

protected:
	explicit Expression (const DEF<decltype (ARGVP0)> &) {
		using R1X = typename LexicalNode::NODE_PACK ;
		const auto r1x = SharedRef<List<R1X>>::make () ;
		mThis = LexicalNode (r1x) ;
		mThis.mTree.self[mThis.mCurr].mRank = RANK_SIZE::compile () ;
		mThis.mTree.self[mThis.mCurr].mDepth = 0 ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	const RETR &template_flip_invoke (const ARGVF<ARGVS<>> & ,const _ARG2 &parameter ,const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	const RETR &template_flip_invoke (const ARGVF<ARGVS<_ARG1>> & ,const _ARG2 &parameter ,const _ARGS &...funcval) const leftvalue {
		return invoke (parameter.one () ,funcval...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	const RETR &template_flip_invoke (const ARGVF<_ARG1> & ,const _ARG2 &parameter ,const _ARGS &...funcval) const leftvalue {
		return template_flip_invoke (ARGV<PARAMS_REST_TYPE<_ARG1>>::ID ,parameter.rest () ,funcval... ,parameter.one ()) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK ,RETR>>>
	_RET template_flip () const {
		Expression<RANK ,RETR> ret ;
		ret.mThis = LexicalNode (mThis.mTree) ;
		_STATIC_ASSERT_ (IS_SAME_HELP<RANK ,U::RANK_FUNC_TYPE<CAPACITY_OF_TYPE<ARGVS<UNITS...>>>>::compile ()) ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const UNITS &...ins) {
			const auto r1x = Expression<RANK ,RETR> (LexicalNode (node.mTree ,node->mChild[0])) ;
			const auto r2x = TupleBinder<const UNITS...> (ins...) ;
			auto &r3x = r1x.template_flip_invoke (ARGV<ARGVS<const UNITS...>>::ID ,r2x) ;
			_NOOP_ (r3x) ;
			return r1x.mThis->mOperand ;
		}) ;
		ret.mThis->mChild[0] = mThis->mCurr ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const RETR &template_flips_patch (const _ARG1 &parameter ,const ARGVF<ARGVS<_ARGS...>> &) const leftvalue {
		return invoke (parameter.pick (ARGV<ARGVP<_ARGS>> {})...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS ,class _RET = REMOVE_CVR_TYPE<Expression<_ARG1 ,RETR>>>
	_RET template_flips (const ARGVF<_ARG1> & ,const ARGVF<_ARG2> & ,const ARGVF<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1 ,RETR> ret ;
		ret.mThis = LexicalNode (mThis.mTree) ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			using R1X = U::RANK_FUNC_TYPE<CAPACITY_OF_TYPE<ARGVS<_ARGS...>>> ;
			_STATIC_ASSERT_ (IS_SAME_HELP<_ARG1 ,R1X>::compile ()) ;
			const auto r1x = Expression<RANK ,RETR> (LexicalNode (node.mTree ,node->mChild[0])) ;
			const auto r2x = TupleBinder<const _ARGS...> (ins...) ;
			auto &r3x = r1x.template_flips_patch (r2x ,ARGV<_ARG2>::ID) ;
			_NOOP_ (r3x) ;
			return r1x.mThis->mOperand ;
		}) ;
		ret.mThis->mChild[0] = mThis->mCurr ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK ,RETR>>>
	_RET template_curry (const ARGVF<RANK1> &) const {
		return _CAST_ (ARGV<Expression<RANK ,RETR>>::ID ,DEREF[this]) ;
	}

	template <class _ARG1 ,class _RET = REMOVE_CVR_TYPE<Expression<RANK1 ,U::RETR_FUNC_TYPE<_ARG1>>>>
	_RET template_curry (const ARGVF<_ARG1> &) const {
		Expression<RANK1 ,U::RETR_FUNC_TYPE<_ARG1>> ret ;
		ret.mThis = LexicalNode (mThis.mTree) ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const Operand &in1) {
			using R2X = U::RANK_FUNC_TYPE<CAPACITY_OF_TYPE<ARGVS<Operand>>> ;
			_STATIC_ASSERT_ (IS_SAME_HELP<RANK1 ,R2X>::compile ()) ;
			const auto r1x = Expression<_ARG1 ,RETR> (LexicalNode (node.mTree ,node->mChild[0])) ;
			const auto r2x = Expression<RANK0> (in1) ;
			const auto r3x = r1x.bind (r2x).curry () ;
			_STATIC_ASSERT_ (IS_SAME_HELP<DEF<decltype (r3x)> ,U::RETR_FUNC_TYPE<_ARG1>>::compile ()) ;
			return Operand (r3x) ;
		}) ;
		ret.mThis->mChild[0] = mThis->mCurr ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class... _ARGS>
	const RETR &template_bind_invoke (const Tuple<> &parameter ,const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const RETR &template_bind_invoke (const _ARG1 &parameter ,const _ARGS &...funcval) const leftvalue {
		return template_bind_invoke (parameter.rest () ,funcval... ,parameter.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	const RETR &template_bind_patch (const Expression<_ARG1 ,_ARG2> &patch_ ,const _ARG3 &parameter ,const ARGVF<ARGVS<>> & ,const _ARGS &...funcval) const leftvalue {
		return template_bind_invoke (parameter ,patch_.invoke (funcval...)) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class _ARG4 ,class... _ARGS>
	const RETR &template_bind_patch (const Expression<_ARG1 ,_ARG2> &patch_ ,const _ARG3 &parameter ,const ARGVF<_ARG4> & ,const _ARGS &...funcval) const leftvalue {
		return template_bind_patch (patch_ ,parameter.rest () ,ARGV<PARAMS_REST_TYPE<_ARG4>>::ID ,funcval... ,parameter.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS ,class _RET = REMOVE_CVR_TYPE<Expression<_ARG1 ,RETR>>>
	_RET template_bind (const ARGVF<_ARG1> & ,const Expression<_ARG2 ,_ARG3> &that ,const ARGVF<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1 ,RETR> ret ;
		ret.mThis = LexicalNode (mThis.mTree) ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			using R1X = U::RANK_FUNC_TYPE<CAPACITY_OF_TYPE<ARGVS<_ARGS...>>> ;
			_STATIC_ASSERT_ (IS_SAME_HELP<_ARG1 ,R1X>::compile ()) ;
			const auto r1x = Expression<RANK ,RETR> (LexicalNode (node.mTree ,node->mChild[0])) ;
			const auto r2x = Expression<_ARG2 ,_ARG3> (LexicalNode (node.mTree ,node->mChild[1])) ;
			const auto r3x = TupleBinder<const _ARGS...> (ins...) ;
			auto &r4x = r1x.template_bind_patch (r2x ,r3x ,ARGV<FUNCTION_PARAMS_TYPE<_ARG2>>::ID) ;
			_NOOP_ (r4x) ;
			return r1x.mThis->mOperand ;
		}) ;
		ret.mThis->mChild[0] = mThis->mCurr ;
		ret.mThis->mChild[1] = that.mThis->mCurr ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth ,that.mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK1 ,RETR>>>
	_RET template_link_patch (const Expression<RANK1 ,RETR> &patch_) const {
		return patch_ ;
	}

	template <class _ARG1 ,class... _ARGS ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_DECREASE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	_RET template_link_patch (const Expression<_ARG1 ,RETR> &patch_ ,const Expression<RANK0> &in1 ,const _ARGS &...ins) const {
		using R1X = U::CONSTEXPR_DECREASE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>> ;
		using R2X = U::CONSTEXPR_INCREASE<CAPACITY_OF_TYPE<ARGVS<_ARGS...>>> ;
		_STATIC_ASSERT_ (U::CONSTEXPR_EQUAL<R1X ,R2X>::compile ()) ;
		return template_link_patch (patch_.bind (in1) ,ins...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS ,class _RET = REMOVE_CVR_TYPE<Expression<_ARG1 ,Expression<_ARG2 ,RETR>>>>
	_RET template_link (const ARGVF<_ARG1> & ,const Expression<RANK0 ,Expression<_ARG2 ,_ARG3>> &that ,const ARGVF<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1 ,Expression<_ARG2 ,RETR>> ret ;
		ret.mThis = LexicalNode (mThis.mTree) ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			using R1X = U::RANK_FUNC_TYPE<CAPACITY_OF_TYPE<ARGVS<_ARGS...>>> ;
			_STATIC_ASSERT_ (IS_SAME_HELP<_ARG1 ,R1X>::compile ()) ;
			const auto r1x = Expression<RANK ,RETR> (LexicalNode (node.mTree ,node->mChild[0])) ;
			const auto r2x = Expression<RANK0 ,Expression<_ARG2 ,_ARG3>> (LexicalNode (node.mTree ,node->mChild[1])) ;
			const auto r3x = r1x.template_flip () ;
			const auto r4x = r1x.template_link_patch (r3x ,Expression<RANK0> (ins)...) ;
			const auto r5x = r4x.bind (r2x.invoke ()) ;
			_STATIC_ASSERT_ (IS_SAME_HELP<DEF<decltype (r5x)> ,Expression<_ARG2 ,RETR>>::compile ()) ;
			return Operand (r5x) ;
		}) ;
		ret.mThis->mChild[0] = mThis->mCurr ;
		ret.mThis->mChild[1] = that.mThis->mCurr ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth ,that.mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;

	}
} ;

template <class RETR>
class Expression<RANK0 ,RETR> :
	delegate private Expression<SPECIALIZATION<RANK0> ,RETR> {
private:
	using RANK = RANK0 ;
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK> ,RETR> ;
	using RANK_SIZE = CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>> ;

private:
	template <class ,class>
	friend class Expression ;
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mThis ;

public:
	implicit Expression () = default ;

	implicit Expression (const Operand &that) {
		_DYNAMIC_ASSERT_ (that.exist ()) ;
		mThis->mOperand = that ;
		mThis->mDepth = 1 ;
	}

	implicit Expression (const Operator &that) :
		delegate SPECIALIZATION_BASE (that) {
		_DYNAMIC_ASSERT_ (that.rank () == RANK_SIZE::compile ()) ;
		mThis->mOperator = that ;
		mThis->mDepth = 1 ;
	}

	explicit Expression (const LexicalNode &node) :
		delegate SPECIALIZATION_BASE (node) {
		_NOOP_ () ;
	}

	const RETR &invoke () const leftvalue {
		if switch_once (TRUE) {
			if (mThis->mOperator.rank () == VAR_NONE)
				discard ;
			_DYNAMIC_ASSERT_ (mThis->mOperator.rank () == 0) ;
			const auto r1x = _MOVE_ (mThis->mOperator) ;
			mThis->mOperand = r1x (mThis.self) ;
			mThis->mChild[0] = LexicalNode () ;
			mThis->mChild[1] = LexicalNode () ;
			mThis->mChild[2] = LexicalNode () ;
		}
		return mThis->mOperand.as (ARGV<RETR>::ID) ;
	}

	template <class... _ARGS>
	inline const RETR &operator() (const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}
} ;

template <class RETR>
class Expression<RANK1 ,RETR> :
	delegate private Expression<SPECIALIZATION<RANK1> ,RETR> {
private:
	using RANK = RANK1 ;
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK> ,RETR> ;
	using RANK_SIZE = CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>> ;

private:
	template <class ,class>
	friend class Expression ;
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mThis ;

public:
	implicit Expression () = default ;

	implicit Expression (const Operator &that) :
		delegate SPECIALIZATION_BASE (that) {
		_DYNAMIC_ASSERT_ (that.rank () == RANK_SIZE::compile ()) ;
		mThis->mOperator = that ;
		mThis->mDepth = 1 ;
	}

	explicit Expression (const LexicalNode &node) :
		delegate SPECIALIZATION_BASE (node) {
		_NOOP_ () ;
	}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const RETR &operator() (const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	using SPECIALIZATION_BASE::flips ;

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::curry ;

	using SPECIALIZATION_BASE::bind ;

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<RANK_SIZE> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	inline _RET operator+ (const Expression<_ARG1 ,_ARG2> &that) const {
		return bind (that) ;
	}

	using SPECIALIZATION_BASE::link ;

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<ARGVS<U::CONSTEXPR_DECREASE<RANK_SIZE> ,ZERO>>> ,Expression<_ARG1 ,RETR>>>>
	inline _RET operator- (const Expression<RANK0 ,Expression<_ARG1 ,_ARG2>> &that) const {
		return link (that) ;
	}
} ;

template <class RANK ,class RETR>
class Expression :
	delegate private Expression<SPECIALIZATION<RANK> ,RETR> {
private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK> ,RETR> ;
	using RANK_SIZE = CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>> ;

private:
	template <class ,class>
	friend class Expression ;
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mThis ;

public:
	implicit Expression () = default ;

	implicit Expression (const Operator &that) :
		delegate SPECIALIZATION_BASE (that) {
		_DYNAMIC_ASSERT_ (that.rank () == RANK_SIZE::compile ()) ;
		mThis->mOperator = that ;
		mThis->mDepth = 1 ;
	}

	explicit Expression (const LexicalNode &node) :
		delegate SPECIALIZATION_BASE (node) {
		_NOOP_ () ;
	}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const RETR &operator() (const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	using SPECIALIZATION_BASE::flips ;

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::curry ;

	using SPECIALIZATION_BASE::bind ;

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<RANK_SIZE> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	inline _RET operator+ (const Expression<_ARG1 ,_ARG2> &that) const {
		return bind (that) ;
	}

	using SPECIALIZATION_BASE::link ;

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<ARGVS<U::CONSTEXPR_DECREASE<RANK_SIZE> ,ZERO>>> ,Expression<_ARG1 ,RETR>>>>
	inline _RET operator- (const Expression<RANK0 ,Expression<_ARG1 ,_ARG2>> &that) const {
		return link (that) ;
	}
} ;
} ;