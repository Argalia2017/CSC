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
	StrongRef<THIS_PACK> mThis ;

public:
	implicit Operand () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<U::CONSTEXPR_NOT<IS_PLACEHOLDER_HELP<_ARG1>> ,U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Operand>>>>>
	implicit Operand (_ARG1 &&that) {
		mThis = StrongRef<THIS_PACK>::make () ;
		mThis->mHolder = AnyRef<REMOVE_CVR_TYPE<_ARG1>>::make (_FORWARD_ (ARGV<_ARG1 &&>::ID ,that)) ;
	}

	BOOL exist () const {
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
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
			return Operand (ARGV<ARGVP<_ARG1>>::ID) ;
		}) ;
	}

private:
	template <class _ARG1>
	const _ARG1 &template_as (const ARGVF<_ARG1> &) const leftvalue {
		const auto r1x = exist () ;
		_DYNAMIC_ASSERT_ (r1x) ;
		const auto r2x = mThis->mHolder.type_mid () ;
		const auto r3x = _TYPEMID_ (ARGV<_ARG1>::ID) ;
		_DYNAMIC_ASSERT_ (r2x == r3x) ;
		return mThis->mHolder.rebind (ARGV<REMOVE_CVR_TYPE<_ARG1>>::ID).self ;
	}

	const Operand &template_as (const ARGVF<Operand> &) const leftvalue {
		return DEREF[this] ;
	}
} ;

class Operator {
private:
	class Holder
		:public Interface {
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

	struct Private {
		class DefHolder ;

		template <class ,class>
		class ImplHolder ;
	} ;

private:
	StrongRef<Holder> mThis ;

public:
	implicit Operator () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<U::CONSTEXPR_AND<U::CONSTEXPR_NOT<IS_PLACEHOLDER_HELP<_ARG1>> ,U::CONSTEXPR_NOT<IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Operator>>>>>
	implicit Operator (_ARG1 &&that) {
		struct Dependent ;
		using R1X = FUNCTION_OF_TYPE<_ARG1> ;
		using R2X = REPEAT_PARAMS_TYPE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<R1X>> ,const Operand &> ;
		_STATIC_ASSERT_ (U::CONSTEXPR_NOT<IS_REFERENCE_HELP<_ARG1>>::compile ()) ;
		using R3X = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<R1X ,R2X> ;
		const auto r1x = Function<R1X> (_FORWARD_ (ARGV<_ARG1 &&>::ID ,that)) ;
		mThis = StrongRef<R3X>::make (r1x) ;
	}

	LENGTH rank () const {
		if (!mThis.exist ())
			return VAR_NONE ;
		return mThis->rank () ;
	}

	template <class... _ARGS>
	Operand invoke (const LexicalNode &node ,const _ARGS &...funcval) const {
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) <= 9) ;
		_DYNAMIC_ASSERT_ (mThis.exist ()) ;
		return mThis->invoke (node ,funcval...) ;
	}

	template <class... _ARGS>
	inline Operand operator() (const LexicalNode &node ,const _ARGS &...funcval) const {
		return invoke (node ,funcval...) ;
	}
} ;

class Operator::Private::DefHolder
	:public Holder {
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
class Operator::Private::ImplHolder<DEF<UNIT1 (UNITS1...)> ,ARGVS<UNITS2...>>
	:public DefHolder {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS1...>) == _CAPACITYOF_ (ARGVS<UNITS2...>)) ;

private:
	Function<UNIT1 (UNITS1...)> mFunctor ;

public:
	template <class... _ARGS>
	explicit ImplHolder (_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) {}

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
class Operator::Private::ImplHolder<DEF<UNIT1 (const LexicalNode & ,UNITS1...)> ,ARGVS<Operand ,UNITS2...>>
	:public DefHolder {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS1...>) == _CAPACITYOF_ (ARGVS<UNITS2...>)) ;

private:
	Function<UNIT1 (const LexicalNode & ,UNITS1...)> mFunctor ;

public:
	template <class... _ARGS>
	explicit ImplHolder (_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::ID ,initval)...) {}

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
	using R1X = typename RETR_FUNC<U::CONSTEXPR_DECREASE<_ARG1>>::TYPE ;
	using TYPE = Expression<RANK1 ,R1X> ;
} ;

template <>
struct RETR_FUNC<ARGC<1>> {
	using TYPE = Operand ;
} ;

template <class _ARG1>
using RETR_FUNC_TYPE = typename RETR_FUNC<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>::TYPE ;
} ;

class LexicalNode
	:public VirtualObject<LexicalNode> {
private:
	template <class ,class>
	friend class Expression ;
	Operand mOperand ;
	Operator mOperator ;
	ARRAY3<StrongRef<LexicalNode>> mChild ;
	LENGTH mRank ;
	LENGTH mDepth ;

public:
	implicit LexicalNode () {
		mRank = VAR_NONE ;
		mDepth = 0 ;
	}
} ;

namespace U {
template <class>
struct CONSTEXPR_MAXOF ;

template <class _ARG1>
struct CONSTEXPR_MAXOF<ARGVS<_ARG1>> {
	imports constexpr VAR compile () {
		return _ARG1::compile () ;
	}
} ;

template <class _ARG1>
struct CONSTEXPR_MAXOF {
	imports constexpr VAR compile () {
		using R1X = CONSTEXPR_MAXOF<PARAMS_ONE_TYPE<_ARG1>> ;
		using R2X = CONSTEXPR_MAXOF<PARAMS_REST_TYPE<_ARG1>> ;
		using R3X = CONSTEXPR_MAX<R1X ,R2X> ;
		return R3X::compile () ;
	}
} ;
} ;

template <class RETR ,class... UNITS>
class Expression<SPECIALIZATION<Operand (UNITS...)> ,RETR> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) >= 0 && _CAPACITYOF_ (ARGVS<UNITS...>) <= 9) ;

protected:
	using RANK = DEF<Operand (UNITS...)> ;

protected:
	StrongRef<LexicalNode> mThis ;

protected:
	implicit Expression ()
		:Expression (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit Expression (const Operator &that)
		: Expression (ARGVP0) {
		_DYNAMIC_ASSERT_ (that.rank () == _CAPACITYOF_ (ARGVS<UNITS...>)) ;
		mThis->mOperator = that ;
		mThis->mDepth = 1 ;
	}

	const RETR &invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const leftvalue {
		_DYNAMIC_ASSERT_ (mThis.exist ()) ;
		if switch_once (TRUE) {
			if (mThis->mOperand.exist ())
				discard ;
			_DYNAMIC_ASSERT_ (mThis->mOperator.rank () == _CAPACITYOF_ (ARGVS<UNITS...>)) ;
			const auto r1x = _MOVE_ (mThis->mOperator) ;
			mThis->mOperand = r1x (mThis.self ,funcval...) ;
			mThis->mChild[0] = StrongRef<LexicalNode> () ;
			mThis->mChild[1] = StrongRef<LexicalNode> () ;
			mThis->mChild[2] = StrongRef<LexicalNode> () ;

		}
		return mThis->mOperand.as (ARGV<RETR>::ID) ;
	}

	Expression<RANK ,RETR> flip () const {
		Expression<RANK ,RETR> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,FORWARD_TRAITS_TYPE<UNITS> &&...ins) {
			auto &r1x = _CAST_ (ARGV<Expression<RANK ,RETR>>::ID ,node.mChild[0]) ;
			return r1x.template_flip_invoke (ARGV<ARGVS<UNITS...>>::ID ,ins...) ;
		}) ;
		ret.mThis->mChild[0] = mThis ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class... _ARGS ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<_ARGS...>> ,RETR>>>
	_RET flips (const ARGV<ARGVP<_ARGS>> &...) const {
		using R1X = U::RANK_FUNC_TYPE<U::CONSTEXPR_MAXOF<_ARGS...>> ;
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) == _CAPACITYOF_ (ARGVS<UNITS...>)) ;
		return template_flips (ARGV<R1X>::ID ,ARGV<ARGVS<_ARGS...>>::ID ,ARGV<FUNCTION_PARAMS_TYPE<R1X>>::ID) ;
	}

	Expression<RANK1 ,RETR> fold () const {
		struct Dependent ;
		using R1X = RANGE_PARAMS_TYPE<CAPACITY_OF_TYPE<ARGVS<UNITS...>>> ;
		Expression<RANK1 ,RETR> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const Operand &in1) {
			auto &r1x = _CAST_ (ARGV<Expression<RANK ,RETR>>::ID ,node.mChild[0]) ;
			auto &r2x = in1.as (ARGV<Expression<RANK1 ,RETR>>::ID) ;
			return r1x.template_fold_invoke (r2x ,ARGV<R1X>::ID) ;
		}) ;
		ret.mThis->mChild[0] = mThis ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>>> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	_RET concat (const Expression<_ARG1 ,_ARG2> &that) const {
		using R1X = U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>>> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ;
		return template_concat (ARGV<R1X>::ID ,that ,ARGV<FUNCTION_PARAMS_TYPE<R1X>>::ID) ;
	}

protected:
	explicit Expression (const DEF<decltype (ARGVP0)> &) {
		mThis = StrongRef<LexicalNode>::make () ;
		mThis->weak_of_this (mThis.recast (ARGV<Object>::ID)) ;
		mThis->mRank = _CAPACITYOF_ (FUNCTION_PARAMS_TYPE<RANK>) ;
	}

	template <class... _ARGS>
	const RETR &template_flip_invoke (const ARGVF<ARGVS<>> & ,const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const RETR &template_flip_invoke (const ARGVF<_ARG1> & ,const PARAMS_ONE_TYPE<_ARG1> &funcval_one ,const _ARGS &...funcval_rest) const leftvalue {
		return template_flip_invoke (ARGV<PARAMS_REST_TYPE<_ARG1>>::ID ,funcval_rest... ,funcval_one) ;
	}
	template <class _ARG1 ,class... _ARGS>
	const RETR &template_flips_invoke (const _ARG1 &parameter ,const ARGVF<ARGVS<_ARGS...>> &) const leftvalue {
		return invoke (parameter.pick (ARGV<ARGVP<_ARGS>>::ID)...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	Expression<_ARG1 ,RETR> template_flips (const ARGVF<_ARG1> & ,const ARGVF<_ARG2> & ,const ARGVF<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1 ,RETR> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1x = _CAST_ (ARGV<Expression<RANK ,RETR>>::ID ,node.mChild[0]) ;
			const auto r2x = TupleBinder<const _ARGS...> (ins...) ;
			return r1x.template_flips_invoke (r2x ,ARGV<_ARG2>::ID) ;
		}) ;
		ret.mThis->mChild[0] = mThis ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	template <class... _ARGS>
	const RETR &template_fold_invoke (const Expression<RANK1 ,RETR> &patch_ ,const ARGVF<ARGVS<>> & ,const _ARGS &...placeholder) const leftvalue {
		struct Dependent ;
		auto &r1x = _FORWARD_ (ARGV<DEPENDENT_TYPE<Expression<RANK1 ,RETR> ,Dependent>>::ID ,patch_) ;
		return invoke (r1x.invoke (Operand::nth (placeholder))...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const RETR &template_fold_invoke (const Expression<RANK1 ,RETR> &patch_ ,const ARGVF<_ARG1> & ,const _ARGS &...placeholder) const leftvalue {
		using R1X = PARAMS_ONE_TYPE<_ARG1> ;
		using R2X = PARAMS_REST_TYPE<_ARG1> ;
		auto &r1x = _NULL_ (ARGV<ARGVP<R1X>>::ID) ;
		return template_flip_invoke (patch_ ,ARGV<R2X>::ID ,placeholder... ,r1x) ;
	}

	template <class... _ARGS>
	const RETR &template_concat_invoke (const Tuple<> &parameter ,const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const RETR &template_concat_invoke (const _ARG1 &parameter ,const _ARGS &...funcval) const leftvalue {
		return template_concat_invoke (parameter.rest () ,funcval... ,parameter.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	const RETR &template_concat_patch (const Expression<_ARG1 ,_ARG3> &patch_ ,const ARGVF<ARGVS<>> & ,const _ARG2 &parameter ,const _ARGS &...funcval) const leftvalue {
		return template_concat_invoke (parameter ,patch_.invoke (funcval...)) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class _ARG4 ,class... _ARGS>
	const RETR &template_concat_patch (const Expression<_ARG1 ,_ARG4> &patch_ ,const ARGVF<_ARG2> & ,const _ARG3 &parameter ,const _ARGS &...funcval) const leftvalue {
		return template_concat_patch (patch_ ,ARGV<PARAMS_REST_TYPE<_ARG2>>::ID ,parameter.rest () ,funcval... ,parameter.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	Expression<_ARG1 ,RETR> template_concat (const ARGVF<_ARG1> & ,const Expression<_ARG2 ,_ARG3> &that ,const ARGVF<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1 ,RETR> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1x = _CAST_ (ARGV<Expression<RANK ,RETR>>::ID ,node.mChild[0]) ;
			auto &r2x = _CAST_ (ARGV<Expression<_ARG2 ,_ARG3>>::ID ,node.mChild[1]) ;
			const auto r3x = TupleBinder<const _ARGS...> (ins...) ;
			return r1x.template_concat_patch (r2x ,ARGV<FUNCTION_PARAMS_TYPE<_ARG2>>::ID ,r3x) ;
		}) ;
		ret.mThis->mChild[0] = mThis ;
		ret.mThis->mChild[1] = that.mThis ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth ,that.mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}
} ;

template <class RETR>
class Expression<RANK0 ,RETR>
	:private Expression<SPECIALIZATION<RANK0>> {
private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK0> ,RETR> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mThis ;

public:
	implicit Expression () = default ;

	implicit Expression (const Operand &that) {
		_DYNAMIC_ASSERT_ (that.exist ()) ;
		mThis->mOperand = that ;
		mThis->mDepth = 1 ;
	}

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {
		_STATIC_WARNING_ ("noop") ;
	}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const RETR &operator() (const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}
} ;

template <class RETR>
class Expression<RANK1 ,RETR>
	:private Expression<SPECIALIZATION<RANK1> ,RETR> {
private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK1> ,RETR> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mThis ;

public:
	implicit Expression () = default ;

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {
		_STATIC_WARNING_ ("noop") ;
	}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const RETR &operator() (const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	using SPECIALIZATION_BASE::flips ;

	Expression<RANK1 ,RETR> curry () const {
		return DEREF[this] ;
	}

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::concat ;

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK1>>> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	inline _RET operator+ (const Expression<_ARG1 ,_ARG2> &that) const {
		return concat (that) ;
	}
} ;

template <class RANK ,class RETR>
class Expression
	:private Expression<SPECIALIZATION<RANK> ,RETR> {

private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK> ,RETR> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mThis ;

public:
	implicit Expression () = default ;

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {
		_STATIC_WARNING_ ("noop") ;
	}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const RETR &operator() (const _ARGS &...funcval) const leftvalue {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	using SPECIALIZATION_BASE::flips ;

	template <class _RET = REMOVE_CVR_TYPE<Expression<RANK1 ,U::RETR_FUNC_TYPE<RANK>>>>
	_RET curry () const {
		struct Dependent ;
		using R1X = DEPENDENT_TYPE<Expression<RANK1 ,U::RETR_FUNC_TYPE<RANK>> ,Dependent> ;
		R1X ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const Operand &in1) {
			auto &r1x = _CAST_ (ARGV<Expression<RANK ,RETR>>::ID ,node.mChild[0]) ;
			auto rax = r1x.concat (in1).curry () ;
			return Operand (_MOVE_ (rax)) ;
		}) ;
		ret.mThis->mChild[0] = mThis ;
		ret.mThis->mDepth = MathProc::maxof (mThis->mDepth) + 1 ;
		return _MOVE_ (ret) ;
	}

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::concat ;

	template <class _ARG1 ,class _ARG2 ,class _RET = REMOVE_CVR_TYPE<Expression<U::RANK_FUNC_TYPE<U::CONSTEXPR_ADD<U::CONSTEXPR_DECREASE<CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<RANK>>> ,CAPACITY_OF_TYPE<FUNCTION_PARAMS_TYPE<_ARG1>>>> ,RETR>>>
	inline _RET operator+ (const Expression<_ARG1 ,_ARG2> &that) const {
		return concat (that) ;
	}
} ;
} ;