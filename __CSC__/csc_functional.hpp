#pragma once

#ifndef __CSC_FUNCTIONAL__
#define __CSC_FUNCTIONAL__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

namespace CSC {
class Operand ;

namespace U {
template <class ,class>
struct RANK_FUNC ;

template <class _ARG1 ,class... _ARGS>
struct RANK_FUNC<_ARG1 ,ARGVS<_ARGS...>> {
	using TYPE = PTR<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1>
using RANK_FUNC_TYPE = typename RANK_FUNC<Operand ,REPEAT_PARAMS_TYPE<_ARG1 ,const Operand &>>::TYPE ;
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
	class Pack {
	private:
		friend Operand ;
		AnyRef<void> mHolder ;
	} ;

private:
	SharedRef<Pack> mThis ;

public:
	Operand () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Operand>::value>>
	explicit Operand (_ARG1 &&that) {
		mThis = SharedRef<Pack>::make () ;
		mThis->mHolder = AnyRef<REMOVE_CVR_TYPE<_ARG1>>::make (std::forward<_ARG1> (that)) ;
	}

	BOOL exist () const {
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

	BOOL equal (const Operand &that) const {
		if (!mThis.exist ())
			if (!that.mThis.exist ())
				return TRUE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!that.mThis.exist ())
			return FALSE ;
		if (&mThis.self != &that.mThis.self)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Operand &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Operand &that) const {
		return !equal (that) ;
	}

	template <class _RET>
	const _RET &as () const & {
		return template_as (_NULL_<ARGV<REMOVE_CVR_TYPE<_RET>>> ()) ;
	}

	template <class _RET>
	auto as () && ->void = delete ;

public:
	template <class _ARG1>
	inline static const Operand &nth (const ARGV<ARGVP<_ARG1>> &) {
		return _CACHE_ ([&] () {
			return Operand (_NULL_<ARGV<ARGVP<_ARG1>>> ()) ;
		}) ;
	}

private:
	template <class _ARG1>
	const _ARG1 &template_as (const ARGV<_ARG1> &) const {
		const auto r1x = exist () ;
		_DYNAMIC_ASSERT_ (r1x) ;
		const auto r2x = mThis->mHolder.typemid () ;
		const auto r3x = _TYPEMID_<_ARG1> () ;
		_DYNAMIC_ASSERT_ (r2x == r3x) ;
		return mThis->mHolder.template rebind<REMOVE_CVR_TYPE<_ARG1>> ().self ;
	}

	const Operand &template_as (const ARGV<Operand> &) const {
		return (*this) ;
	}
} ;

class Operator {
private:
	exports class Functor
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

private:
	struct Detail ;
	StrongRef<Functor> mThis ;

public:
	Operator () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Operator>::value>>
	explicit Operator (const _ARG1 &that) {
		struct Dependent ;
		using FUNC_HINT = REMOVE_FUNCATTR_TYPE<REMOVE_MEMPTR_TYPE<DEF<decltype (&_ARG1::operator())>>> ;
		using ImplFunctor = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplFunctor<PTR<FUNC_HINT> ,REPEAT_PARAMS_TYPE<ARGC<_CAPACITYOF_ (INVOKE_PARAMS_TYPE<FUNC_HINT>)> ,Operand>> ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG1 ,PTR<FUNC_HINT>>::value) ;
		_STATIC_ASSERT_ (stl::is_complete<ImplFunctor>::value) ;
		const auto r1x = _XVALUE_<PTR<FUNC_HINT>> (that) ;
		mThis = StrongRef<ImplFunctor>::make (r1x) ;
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

struct Operator::Detail {
	template <class ,class>
	class ImplFunctor ;
} ;

template <>
class Operator::Detail::ImplFunctor<void ,void>
	:public Functor {
public:
	ImplFunctor () = default ;

	LENGTH rank () const override {
		return VAR_NONE ;
	}

	Operand invoke (const LexicalNode &node) const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	Operand invoke (const LexicalNode &node ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand & ,const Operand &) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}
} ;

template <class UNIT1 ,class... UNITS1 ,class... UNITS2>
class Operator::Detail::ImplFunctor<PTR<UNIT1 (UNITS1...)> ,ARGVS<UNITS2...>>
	:public ImplFunctor<void ,void> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS1...>) == _CAPACITYOF_ (ARGVS<UNITS2...>)) ;

private:
	Function<UNIT1 (UNITS1...)> mFunction ;

public:
	ImplFunctor () = delete ;

	explicit ImplFunctor (const PTR<UNIT1 (UNITS1...)> &func)
		:mFunction (func) {}

	LENGTH rank () const override {
		return _CAPACITYOF_ (ARGVS<UNITS2...>) ;
	}

	Operand invoke (const LexicalNode &node ,const UNITS2 &...funcval) const {
		auto tmp = template_invoke (TupleBinder<const UNITS2...> (funcval...) ,_NULL_<ARGV<ARGVS<UNITS1...>>> ()) ;
		return Operand (std::move (tmp)) ;
	}

private:
	UNIT1 template_invoke (const Tuple<> &parameter ,const ARGV<ARGVS<>> & ,FORWARD_TRAITS_TYPE<UNITS1> &&...funcval) const {
		return mFunction (std::forward<FORWARD_TRAITS_TYPE<UNITS1>> (funcval)...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	UNIT1 template_invoke (const _ARG1 &parameter ,const ARGV<_ARG2> & ,_ARGS &&...funcval) const {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG2> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG2> ;
		auto &r1x = parameter.one ().template as<ONE_HINT> () ;
		return template_invoke (parameter.rest () ,_NULL_<ARGV<REST_HINT>> () ,std::forward<_ARGS> (funcval)... ,r1x) ;
	}
} ;

template <class UNIT1 ,class... UNITS1 ,class... UNITS2>
class Operator::Detail::ImplFunctor<PTR<UNIT1 (const LexicalNode & ,UNITS1...)> ,ARGVS<Operand ,UNITS2...>>
	:public ImplFunctor<void ,void> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS1...>) == _CAPACITYOF_ (ARGVS<UNITS2...>)) ;

private:
	Function<UNIT1 (const LexicalNode & ,UNITS1...)> mFunction ;

public:
	ImplFunctor () = delete ;

	explicit ImplFunctor (const PTR<UNIT1 (const LexicalNode & ,UNITS1...)> &func)
		:mFunction (func) {}

	LENGTH rank () const override {
		return _CAPACITYOF_ (ARGVS<UNITS2...>) ;
	}

	Operand invoke (const LexicalNode &node ,const UNITS2 &...funcval) const override {
		auto tmp = template_invoke (TupleBinder<const UNITS2...> (funcval...) ,_NULL_<ARGV<ARGVS<UNITS1...>>> () ,node) ;
		return Operand (std::move (tmp)) ;
	}

private:
	UNIT1 template_invoke (const Tuple<> &parameter ,const ARGV<ARGVS<>> & ,const LexicalNode &node ,FORWARD_TRAITS_TYPE<UNITS1> &&...funcval) const {
		return mFunction (node ,std::forward<FORWARD_TRAITS_TYPE<UNITS1>> (funcval)...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	UNIT1 template_invoke (const _ARG1 &parameter ,const ARGV<_ARG2> & ,_ARGS &&...funcval) const {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG2> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG2> ;
		auto &r1x = parameter.one ().template as<ONE_HINT> () ;
		return template_invoke (parameter.rest () ,_NULL_<ARGV<REST_HINT>> () ,std::forward<_ARGS> (funcval)... ,r1x) ;
	}
} ;

template <class>
class Expression ;

class LexicalNode
	:public Object::Virtual {
private:
	template <class>
	friend class Expression ;
	Operand mOperand ;
	Operator mOperator ;
	ARRAY3<StrongRef<LexicalNode>> mChild ;
	LENGTH mDepth ;

public:
	LexicalNode ()
		:Object (_NULL_<ARGV<LexicalNode>> ()) ,mDepth (0) {}
} ;

template <class UNIT>
class LexicalTree {
private:
	template <class>
	friend class Expression ;
	StrongRef<LexicalNode> mThis ;

public:
	LexicalTree () {
		mThis = StrongRef<LexicalNode>::make () ;
	}

	const StrongRef<LexicalNode> &to () const {
		return mThis ;
	}

	inline implicit operator const StrongRef<LexicalNode> & () const {
		return to () ;
	}

private:
	inline static const UNIT &from (const StrongRef<LexicalNode> &me) {
		return _CAST_<UNIT> (me) ;
	}
} ;

namespace U {
inline constexpr LENGTH constexpr_max_value (const ARGV<ARGVS<>> &) {
	//@error: fuck clang
	return 0 ;
}

template <class _ARG1>
inline constexpr LENGTH constexpr_max_value (const ARGV<ARGVS<_ARG1>> &) {
	return _ARG1::value ;
}

template <class _ARG1>
inline constexpr LENGTH constexpr_max_value (const ARGV<_ARG1> &) {
	using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
	using TWO_HINT = ARGVS_ONE_TYPE<ARGVS_REST_TYPE<_ARG1>> ;
	using BIGGER_HINT = ARGVS<ARGC<_MAX_<VAR> (ONE_HINT::value ,TWO_HINT::value)>> ;
	using REST_HINT = ARGVS_REST_TYPE<ARGVS_REST_TYPE<_ARG1>> ;
	return constexpr_max_value (_NULL_<ARGV<ARGVS_CAT_TYPE<BIGGER_HINT ,REST_HINT>>> ()) ;
}
} ;

template <class... UNITS>
class Expression<SPECIALIZATION<PTR<Operand (const UNITS &...)>>>
	:private LexicalTree<Expression<PTR<Operand (const UNITS &...)>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) >= 0 && _CAPACITYOF_ (ARGVS<UNITS...>) <= 9) ;

private:
	using RANK = PTR<Operand (const UNITS &...)> ;

	template <class... _ARGS>
	using FLIP_RETURN_HINT = Expression<U::RANK_FUNC_TYPE<ARGC<U::constexpr_max_value (_NULL_<ARGV<ARGVS<_ARGS...>>> ())>>> ;

	template <class _ARG1>
	using CONCAT_RETURN_HINT = Expression<U::RANK_FUNC_TYPE<ARGC<_CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK>>) - 1 + _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<_ARG1>>)>>> ;

	using Dependent = Expression ;

private:
	template <class>
	friend class Expression ;
	using LexicalTree<Expression<RANK>>::mThis ;

public:
	Expression () = default ;

	implicit Expression (const Operator &that) {
		_DYNAMIC_ASSERT_ (that.rank () == _CAPACITYOF_ (ARGVS<UNITS...>)) ;
		mThis->mOperator = that ;
		mThis->mDepth = 1 ;
	}

	const Operand &invoke (const UNITS &...funcval) const {
		_DYNAMIC_ASSERT_ (mThis.exist ()) ;
		if switch_case (TRUE) {
			if (mThis->mOperand.exist ())
				discard ;
			mThis->mOperand = mThis->mOperator (mThis.self ,funcval...) ;
			mThis->mOperator = Operator () ;
			mThis->mChild[0] = StrongRef<LexicalNode> () ;
			mThis->mChild[1] = StrongRef<LexicalNode> () ;
			mThis->mChild[2] = StrongRef<LexicalNode> () ;

		}
		return mThis->mOperand ;
	}

	Expression<RANK> flip () const {
		Expression<RANK> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const UNITS &...ins) {
			auto &r1x = Expression<RANK>::from (node.mChild[0]) ;
			return r1x.template_flip_invoke (_NULL_<ARGV<ARGVS<UNITS...>>> () ,ins...) ;
		}) ;
		ret.mThis->mChild[0] = (*this) ;
		ret.mThis->mDepth = _MAXOF_ (mThis->mDepth) + 1 ;
		return std::move (ret) ;
	}

	template <class... _ARGS>
	FLIP_RETURN_HINT<_ARGS...> flip (const ARGV<ARGVP<_ARGS>> &...) const {
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) == _CAPACITYOF_ (ARGVS<UNITS...>)) ;
		using FLIP_RANK_HINT = U::RANK_FUNC_TYPE<ARGC<U::constexpr_max_value (_NULL_<ARGV<ARGVS<_ARGS...>>> ())>> ;
		return template_flip2 (_NULL_<ARGV<FLIP_RANK_HINT>> () ,_NULL_<ARGV<ARGVS<_ARGS...>>> () ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<FLIP_RANK_HINT>>>> ()) ;
	}

	DEPENDENT_TYPE<Expression<RANK1> ,Dependent> curry () const {
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) >= 2 && _CAPACITYOF_ (ARGVS<UNITS...>) <= 9) ;
		DEPENDENT_TYPE<Expression<RANK1> ,Dependent> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const Operand &in1) {
			auto &r1x = Expression<RANK>::from (node.mChild[0]) ;
			auto tmp = r1x.concat (in1).curry () ;
			return Operand (std::move (tmp)) ;
		}) ;
		ret.mThis->mChild[0] = (*this) ;
		ret.mThis->mDepth = _MAXOF_ (mThis->mDepth) + 1 ;
		return std::move (ret) ;
	}

	DEPENDENT_TYPE<Expression<RANK1> ,Dependent> fold () const {
		DEPENDENT_TYPE<Expression<RANK1> ,Dependent> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const Operand &in1) {
			auto &r1x = Expression<RANK>::from (node.mChild[0]) ;
			auto &r2x = in1.template as<DEPENDENT_TYPE<Expression<RANK1> ,Dependent>> () ;
			return r1x.template_fold_invoke (r2x ,_NULL_<ARGV<SEQUENCE_PARAMS_TYPE<ARGC<_CAPACITYOF_ (ARGVS<UNITS...>)>>>>) ;
		}) ;
		ret.mThis->mChild[0] = (*this) ;
		ret.mThis->mDepth = _MAXOF_ (mThis->mDepth) + 1 ;
		return std::move (ret) ;
	}

	template <class _ARG1>
	CONCAT_RETURN_HINT<_ARG1> concat (const Expression<_ARG1> &that) const {
		using CONCAT_RANK_HINT = U::RANK_FUNC_TYPE<ARGC<_CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK>>) - 1 + _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<_ARG1>>)>> ;
		return template_concat (_NULL_<ARGV<CONCAT_RANK_HINT>> () ,that ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<CONCAT_RANK_HINT>>>> ()) ;
	}

private:
	template <class... _ARGS>
	const Operand &template_flip_invoke (const ARGV<ARGVS<>> & ,const _ARGS &...funcval) const {
		return invoke (funcval...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const Operand &template_flip_invoke (const ARGV<_ARG1> & ,const ARGVS_ONE_TYPE<_ARG1> &funcval_one ,const _ARGS &...funcval_rest) const {
		return template_flip_invoke (_NULL_<ARGV<ARGVS_REST_TYPE<_ARG1>>> () ,funcval_rest... ,funcval_one) ;
	}
	template <class _ARG1 ,class... _ARGS>
	const Operand &template_flip2_invoke (const _ARG1 &parameter ,const ARGV<ARGVS<_ARGS...>> &) const {
		return invoke (parameter.pick (_NULL_<ARGV<ARGVP<_ARGS>>> ())...) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	Expression<_ARG1> template_flip2 (const ARGV<_ARG1> & ,const ARGV<_ARG2> & ,const ARGV<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1x = Expression<RANK>::from (node.mChild[0]) ;
			const auto r2x = TupleBinder<const _ARGS...> (ins...) ;
			return r1x.template_flip2_invoke (r2x ,_NULL_<ARGV<_ARG2>> ()) ;
		}) ;
		ret.mThis->mChild[0] = (*this) ;
		ret.mThis->mDepth = _MAXOF_ (mThis->mDepth) + 1 ;
		return std::move (ret) ;
	}

	template <class... _ARGS>
	const Operand &template_fold_invoke (const DEPENDENT_TYPE<Expression<RANK1> ,Dependent> &patch_ ,const ARGV<ARGVS<>> & ,const _ARGS &...placeholder) const {
		return invoke (patch_.invoke (Operand::nth (placeholder))...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const Operand &template_fold_invoke (const DEPENDENT_TYPE<Expression<RANK1> ,Dependent> &patch_ ,const ARGV<_ARG1> & ,const _ARGS &...placeholder) const {
		return template_flip_invoke (patch_ ,_NULL_<ARGV<ARGVS_REST_TYPE<_ARG1>>> () ,placeholder... ,_NULL_<ARGVP<ARGVS_ONE_TYPE<_ARG1>>>) ;
	}

	template <class... _ARGS>
	const Operand &template_concat_invoke (const Tuple<> &parameter ,const _ARGS &...funcval) const {
		return invoke (funcval...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	const Operand &template_concat_invoke (const _ARG1 &parameter ,const _ARGS &...funcval) const {
		return template_concat_invoke (parameter.rest () ,funcval... ,parameter.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	const Operand &template_concat_patch (const Expression<_ARG1> &patch_ ,const ARGV<ARGVS<>> & ,const _ARG2 &parameter ,const _ARGS &...funcval) const {
		return template_concat_invoke (parameter ,patch_.invoke (funcval...)) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	const Operand &template_concat_patch (const Expression<_ARG1> &patch_ ,const ARGV<_ARG2> & ,const _ARG3 &parameter ,const _ARGS &...funcval) const {
		return template_concat_patch (patch_ ,_NULL_<ARGV<ARGVS_REST_TYPE<_ARG2>>> () ,parameter.rest () ,funcval... ,parameter.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	Expression<_ARG1> template_concat (const ARGV<_ARG1> & ,const Expression<_ARG2> &that ,const ARGV<ARGVS<_ARGS...>> &) const {
		Expression<_ARG1> ret ;
		ret.mThis->mOperator = Operator ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1x = Expression<RANK>::from (node.mChild[0]) ;
			auto &r2x = Expression<_ARG2>::from (node.mChild[1]) ;
			const auto r3x = TupleBinder<const _ARGS...> (ins...) ;
			return r1x.template_concat_patch (r2x ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<_ARG2>>>> () ,r3x) ;
		}) ;
		ret.mThis->mChild[0] = (*this) ;
		ret.mThis->mChild[1] = that ;
		ret.mThis->mDepth = _MAXOF_ (mThis->mDepth ,that.mThis->mDepth) + 1 ;
		return std::move (ret) ;
	}
} ;

template <>
class Expression<RANK0>
	:private Expression<SPECIALIZATION<RANK0>> {
private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK0>> ;

private:
	template <class>
	friend class Expression ;
	using LexicalTree<Expression<RANK0>>::mThis ;

public:
	Expression () = default ;

	implicit Expression (const Operand &that) {
		_DYNAMIC_ASSERT_ (that.exist ()) ;
		mThis->mOperand = that ;
		mThis->mDepth = 1 ;
	}

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const Operand &operator() (const _ARGS &...funcval) const {
		return invoke (funcval...) ;
	}
} ;

template <>
class Expression<RANK1>
	:private Expression<SPECIALIZATION<RANK1>> {
private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK1>> ;

	template <class _ARG1>
	using CONCAT_RETURN_HINT = Expression<U::RANK_FUNC_TYPE<ARGC<_CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK1>>) - 1 + _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<_ARG1>>)>>> ;

private:
	template <class>
	friend class Expression ;
	using LexicalTree<Expression<RANK1>>::mThis ;

public:
	Expression () = default ;

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const Operand &operator() (const _ARGS &...funcval) const {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	Expression<RANK1> curry () const {
		return (*this) ;
	}

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::concat ;

	template <class _ARG1>
	inline auto operator+ (const Expression<_ARG1> &that) const
		->DEF<decltype (_NULL_<const Expression> ().concat (_NULL_<Expression<_ARG1>> ()))> {
		return concat (that) ;
	}
} ;

template <class RANK>
class Expression
	:private Expression<SPECIALIZATION<RANK>> {

private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK>> ;

	template <class _ARG1>
	using CONCAT_RETURN_HINT = Expression<U::RANK_FUNC_TYPE<ARGC<_CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK>>) - 1 + _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<_ARG1>>)>>> ;

private:
	template <class>
	friend class Expression ;
	using LexicalTree<Expression<RANK>>::mThis ;

public:
	Expression () = default ;

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const Operand &operator() (const _ARGS &...funcval) const {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	using SPECIALIZATION_BASE::curry ;

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::concat ;

	template <class _ARG1>
	inline auto operator+ (const Expression<_ARG1> &that) const
		->DEF<decltype (_NULL_<const Expression> ().concat (_NULL_<Expression<_ARG1>> ()))> {
		return concat (that) ;
	}
} ;

template <>
class Expression<RANK3>
	:private Expression<SPECIALIZATION<RANK3>> {
private:
	using SPECIALIZATION_BASE = Expression<SPECIALIZATION<RANK3>> ;

	template <class _ARG1>
	using CONCAT_RETURN_HINT = Expression<U::RANK_FUNC_TYPE<ARGC<_CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK3>>) - 1 + _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<_ARG1>>)>>> ;

private:
	template <class>
	friend class Expression ;
	using LexicalTree<Expression<RANK3>>::mThis ;

public:
	Expression () = default ;

	implicit Expression (const Operator &that)
		:SPECIALIZATION_BASE (that) {}

	using SPECIALIZATION_BASE::invoke ;

	template <class... _ARGS>
	inline const Operand &operator() (const _ARGS &...funcval) const {
		return invoke (funcval...) ;
	}

	using SPECIALIZATION_BASE::flip ;

	using SPECIALIZATION_BASE::curry ;

	using SPECIALIZATION_BASE::fold ;

	using SPECIALIZATION_BASE::concat ;

	template <class _ARG1>
	inline auto operator+ (const Expression<_ARG1> &that) const
		->DEF<decltype (_NULL_<const Expression> ().concat (_NULL_<Expression<_ARG1>> ()))> {
		return concat (that) ;
	}
} ;
} ;