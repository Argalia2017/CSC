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
template <class>
struct RANK_FUNC ;

template <class... _ARGS>
struct RANK_FUNC<ARGVS<_ARGS...>> {
	using TYPE = PTR<Operand (const _ARGS &...)> ;
} ;

template <class _ARG1>
using RANK_FUNC_TYPE = typename RANK_FUNC<REPEAT_OF_TYPE<_ARG1 ,Operand>>::TYPE ;
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

namespace U {
template <class>
struct CONTEXT_RANK_FUNC ;

template <class... _ARGS>
struct CONTEXT_RANK_FUNC<PTR<Operand (const _ARGS &...)>> {
	using TYPE = PTR<Operand (const LexicalNode & ,const _ARGS &...)> ;
} ;

template <class _ARG1>
using CONTEXT_RANK_FUNC_TYPE = typename CONTEXT_RANK_FUNC<_ARG1>::TYPE ;
} ;

using CONTEXT_RANK0 = U::CONTEXT_RANK_FUNC_TYPE<RANK0> ;
using CONTEXT_RANK1 = U::CONTEXT_RANK_FUNC_TYPE<RANK1> ;
using CONTEXT_RANK2 = U::CONTEXT_RANK_FUNC_TYPE<RANK2> ;
using CONTEXT_RANK3 = U::CONTEXT_RANK_FUNC_TYPE<RANK3> ;

class Operand {
private:
	class Lazy {
	private:
		friend Operand ;
		AnyRef<void> mValue ;
		AnyRef<void> mEvaluation ;
	} ;

private:
	SharedRef<Lazy> mLazy ;

public:
	inline Operand () = default ;

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Operand>::value>>
	inline implicit Operand (_ARG1 &&that) {
		mLazy = SharedRef<Lazy>::make () ;
		mLazy->mValue = AnyRef<REMOVE_CVR_TYPE<_ARG1>>::make (std::forward<_ARG1> (that)) ;
	}

	inline BOOL exist () const {
		if (!mLazy.exist ())
			return FALSE ;
		return mLazy->mValue.exist () ;
	}

	template <class _RET>
	inline const _RET &as () const & {
		return template_as (_NULL_<ARGV<REMOVE_CVR_TYPE<_RET>>> ()) ;
	}

	template <class _RET>
	inline const _RET &as () const && = delete ;

private:
	template <class _ARG1>
	inline const _ARG1 &template_as (const ARGV<_ARG1> &) const {
		const auto r1x = exist () ;
		_DYNAMIC_ASSERT_ (r1x) ;
		const auto r2x = mLazy->mValue.typemid () ;
		const auto r3x = _TYPEMID_<_ARG1> () ;
		_DYNAMIC_ASSERT_ (r2x == r3x) ;
		return mLazy->mValue.template rebind<_ARG1> ().self ;
	}

	inline const Operand &template_as (const ARGV<Operand> &) const {
		return (*this) ;
	}
} ;

class Operator {
private:
	exports struct Holder :public Interface {
		virtual LENGTH rank () const = 0 ;
		virtual Operand invoke () const = 0 ;
		virtual Operand invoke (const Operand &in1) const = 0 ;
		virtual Operand invoke (const Operand &in1 ,const Operand &in2) const = 0 ;
		virtual Operand invoke (const Operand &in1 ,const Operand &in2 ,const Operand &in3) const = 0 ;
		virtual Operand invoke (const LexicalNode &node) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand &in1) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) const = 0 ;
		virtual Operand invoke (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) const = 0 ;
	} ;

	template <class>
	class ImplHolder ;

private:
	PTR<Holder> mHolder ;

public:
	inline Operator () {
		mHolder = NULL ;
	}

	inline implicit Operator (const RANK0 &that) ;

	inline implicit Operator (const RANK1 &that) ;

	inline implicit Operator (const RANK2 &that) ;

	inline implicit Operator (const RANK3 &that) ;

	inline implicit Operator (const CONTEXT_RANK0 &that) ;

	inline implicit Operator (const CONTEXT_RANK1 &that) ;

	inline implicit Operator (const CONTEXT_RANK2 &that) ;

	inline implicit Operator (const CONTEXT_RANK3 &that) ;

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Operator>::value>>
	inline implicit Operator (const _ARG1 &that) ;

	inline LENGTH rank () const {
		if (mHolder == NULL)
			return VAR_NONE ;
		return mHolder->rank () ;
	}

	inline Operand operator() () const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke () ;
	}

	inline Operand operator() (const Operand &in1) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (in1) ;
	}

	inline Operand operator() (const Operand &in1 ,const Operand &in2) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (in1 ,in2) ;
	}

	inline Operand operator() (const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (in1 ,in2 ,in3) ;
	}

	inline Operand operator() (const LexicalNode &node) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (node) ;
	}

	inline Operand operator() (const LexicalNode &node ,const Operand &in1) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (node ,in1) ;
	}

	inline Operand operator() (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (node ,in1 ,in2) ;
	}

	inline Operand operator() (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		_DYNAMIC_ASSERT_ (mHolder != NULL) ;
		return mHolder->invoke (node ,in1 ,in2 ,in3) ;
	}
} ;

template <>
class Operator::ImplHolder<void> :public Holder {
public:
	inline ImplHolder () = default ;

	inline LENGTH rank () const override {
		return VAR_NONE ;
	}

	inline Operand invoke () const override {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	inline Operand invoke (const Operand &in1) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	inline Operand invoke (const Operand &in1 ,const Operand &in2) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	inline Operand invoke (const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Operand () ;
	}

	inline Operand invoke (const LexicalNode &node) const override {
		return invoke () ;
	}

	inline Operand invoke (const LexicalNode &node ,const Operand &in1) const {
		return invoke (in1) ;
	}

	inline Operand invoke (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) const {
		return invoke (in1 ,in2) ;
	}

	inline Operand invoke (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		return invoke (in1 ,in2 ,in3) ;
	}
} ;

template <>
class Operator::ImplHolder<RANK0> :public ImplHolder<void> {
private:
	RANK0 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK0 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK0>>) ;
	}

	inline Operand invoke () const override {
		return mFunction () ;
	}
} ;

inline Operator::Operator (const RANK0 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<RANK0 ,AutoRef<Operator::ImplHolder<RANK0>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<RANK0>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<RANK1> :public ImplHolder<void> {
private:
	RANK1 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK1 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK1>>) ;
	}

	inline Operand invoke (const Operand &in1) const {
		return mFunction (in1) ;
	}
} ;

inline Operator::Operator (const RANK1 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<RANK1 ,AutoRef<Operator::ImplHolder<RANK1>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<RANK1>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<RANK2> :public ImplHolder<void> {
private:
	RANK2 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK2 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK2>>) ;
	}

	inline Operand invoke (const Operand &in1 ,const Operand &in2) const {
		return mFunction (in1 ,in2) ;
	}
} ;

inline Operator::Operator (const RANK2 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<RANK2 ,AutoRef<Operator::ImplHolder<RANK2>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<RANK2>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<RANK3> :public ImplHolder<void> {
private:
	RANK3 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK3 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK3>>) ;
	}

	inline Operand invoke (const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		return mFunction (in1 ,in2 ,in3) ;
	}
} ;

inline Operator::Operator (const RANK3 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<RANK3 ,AutoRef<Operator::ImplHolder<RANK3>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<RANK3>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<CONTEXT_RANK0> :public ImplHolder<void> {
private:
	CONTEXT_RANK0 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const CONTEXT_RANK0 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<CONTEXT_RANK0>>) ;
	}

	inline Operand invoke (const LexicalNode &node) const override {
		return mFunction (node) ;
	}
} ;

inline Operator::Operator (const CONTEXT_RANK0 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<CONTEXT_RANK0 ,AutoRef<Operator::ImplHolder<CONTEXT_RANK0>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<CONTEXT_RANK0>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<CONTEXT_RANK1> :public ImplHolder<void> {
private:
	CONTEXT_RANK1 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const CONTEXT_RANK1 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<CONTEXT_RANK1>>) ;
	}

	inline Operand invoke (const LexicalNode &node ,const Operand &in1) const {
		return mFunction (node ,in1) ;
	}
} ;

inline Operator::Operator (const CONTEXT_RANK1 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<CONTEXT_RANK1 ,AutoRef<Operator::ImplHolder<CONTEXT_RANK1>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<CONTEXT_RANK1>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<CONTEXT_RANK2> :public ImplHolder<void> {
private:
	CONTEXT_RANK2 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const CONTEXT_RANK2 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<CONTEXT_RANK2>>) ;
	}

	inline Operand invoke (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) const {
		return mFunction (node ,in1 ,in2) ;
	}
} ;

inline Operator::Operator (const CONTEXT_RANK2 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<CONTEXT_RANK2 ,AutoRef<Operator::ImplHolder<CONTEXT_RANK2>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<CONTEXT_RANK2>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <>
class Operator::ImplHolder<CONTEXT_RANK3> :public ImplHolder<void> {
private:
	CONTEXT_RANK3 mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const CONTEXT_RANK3 &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<CONTEXT_RANK3>>) ;
	}

	inline Operand invoke (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		return mFunction (node ,in1 ,in2 ,in3) ;
	}
} ;

inline Operator::Operator (const CONTEXT_RANK3 &that) {
	_DEBUG_ASSERT_ (that != NULL) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<CONTEXT_RANK3 ,AutoRef<Operator::ImplHolder<CONTEXT_RANK3>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<CONTEXT_RANK3>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <class _ARG1>
class Operator::ImplHolder<PTR<_ARG1 ()>> :public ImplHolder<void> {
private:
	using RANK = PTR<_ARG1 ()> ;

private:
	RANK mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return 0 ;
	}

	inline Operand invoke () const {
		auto tmp = mFunction () ;
		return Operand (std::move (tmp)) ;
	}
} ;

template <class _ARG1 ,class _ARG2>
class Operator::ImplHolder<PTR<_ARG1 (_ARG2)>> :public ImplHolder<void> {
private:
	using RANK = PTR<_ARG1 (_ARG2)> ;

private:
	RANK mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return 1 ;
	}

	inline Operand invoke (const Operand &in1) const {
		auto &r1y = in1.template as<_ARG2> () ;
		auto tmp = mFunction (r1y) ;
		return Operand (std::move (tmp)) ;
	}
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
class Operator::ImplHolder<PTR<_ARG1 (_ARG2 ,_ARG3)>> :public ImplHolder<void> {
private:
	using RANK = PTR<_ARG1 (_ARG2 ,_ARG3)> ;

private:
	RANK mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return 2 ;
	}

	inline Operand invoke (const Operand &in1 ,const Operand &in2) const {
		auto &r1y = in1.template as<_ARG2> () ;
		auto &r2y = in2.template as<_ARG3> () ;
		auto tmp = mFunction (r1y ,r2y) ;
		return Operand (std::move (tmp)) ;
	}
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class _ARG4>
class Operator::ImplHolder<PTR<_ARG1 (_ARG2 ,_ARG3 ,_ARG4)>> :public ImplHolder<void> {
private:
	using RANK = PTR<_ARG1 (_ARG2 ,_ARG3 ,_ARG4)> ;

private:
	RANK mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const RANK &function_) :mFunction (function_) {}

	inline LENGTH rank () const override {
		return 3 ;
	}

	inline Operand invoke (const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		auto &r1y = in1.template as<_ARG2> () ;
		auto &r2y = in2.template as<_ARG3> () ;
		auto &r3y = in3.template as<_ARG4> () ;
		auto tmp = mFunction (r1y ,r2y ,r3y) ;
		return Operand (std::move (tmp)) ;
	}
} ;

template <class _ARG1 ,class>
inline Operator::Operator (const _ARG1 &that) {
	using RANK = PTR<REMOVE_FUNCATTR_TYPE<REMOVE_MEMPTR_TYPE<DEF<decltype (&_ARG1::operator())>>>> ;
	_STATIC_ASSERT_ (std::is_convertible<_ARG1 ,RANK>::value) ;
	auto &r1y = _CACHE_ ([&] () {
		return SharedRef<Set<RANK ,AutoRef<Operator::ImplHolder<RANK>>>>::make () ;
	}) ;
	INDEX ix = r1y->find (that) ;
	if switch_case (TRUE) {
		if (ix != VAR_NONE)
			discard ;
		ix = r1y->insert (that) ;
		r1y.self[ix].item = AutoRef<Operator::ImplHolder<RANK>>::make (that) ;
	}
	mHolder = &r1y.self[ix].item.self ;
}

template <class ,class = Operand>
class Expression ;

class LexicalNode :public Object::Virtual {
private:
	template <class ,class>
	friend class Expression ;
	Operand mOperand ;
	Operator mOperator ;
	ARRAY4<StrongRef<LexicalNode>> mChild ;

public:
	inline LexicalNode () :Object (_NULL_<ARGV<LexicalNode>> ()) {}
} ;

template <class TYPE>
class LexicalTree {
private:
	template <class ,class>
	friend class Expression ;
	StrongRef<LexicalNode> mTree ;

public:
	inline LexicalTree () {
		mTree = StrongRef<LexicalNode>::make () ;
	}

	inline const StrongRef<LexicalNode> &to () const {
		return mTree ;
	}

	inline implicit operator const StrongRef<LexicalNode> & () const {
		return to () ;
	}

private:
	inline static const TYPE &from (const StrongRef<LexicalNode> &me) {
		return _CAST_<TYPE> (me) ;
	}
} ;

template <class HINT>
class Expression<RANK0 ,HINT> :private LexicalTree<Expression<RANK0 ,HINT>> {
private:
	template <class ,class>
	friend class Expression ;
	using LexicalTree<Expression<RANK0 ,HINT>>::mTree ;

public:
	inline Expression () = default ;

	inline implicit Expression (const Operand &that) : Expression () {
		_DYNAMIC_ASSERT_ (that.exist ()) ;
		mTree->mOperand = that ;
	}

	inline implicit Expression (const Operator &that) : Expression () {
		_DYNAMIC_ASSERT_ (that.rank () == _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK0>>)) ;
		mTree->mOperator = that ;
	}

	inline implicit operator const Expression<RANK0> & () const {
		return Expression<RANK0>::from ((*this)) ;
	}

	inline HINT operator() () const {
		_DYNAMIC_ASSERT_ (mTree.exist ()) ;
		if (mTree->mOperand.exist ())
			return mTree->mOperand.template as<HINT> () ;
		const auto r1x = mTree->mOperator (mTree.self) ;
		return r1x.template as<HINT> () ;
	}

	inline Expression<RANK0> operator- (const Expression<RANK1> &that) const ;

	inline Expression<RANK1> operator- (const Expression<RANK2> &that) const ;

	inline Expression<RANK2> operator- (const Expression<RANK3> &that) const ;
} ;

namespace U {
template <class _ARG1>
inline constexpr LENGTH constexpr_max_value (const ARGV<ARGVS<_ARG1>> &) {
	return _ARG1::value ;
}

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
inline constexpr LENGTH constexpr_max_value (const ARGV<ARGVS<_ARG1 ,_ARG2 ,_ARGS...>> &) {
	using BIGGER = ARGC<_MAX_<VAR> (_ARG1::value ,_ARG2::value)> ;
	return constexpr_max_value (_NULL_<ARGV<ARGVS<BIGGER ,_ARGS...>>> ()) ;
}
} ;

inline namespace FUNCTIONAL {
template <class _ARG1 ,class... _ARGS>
inline _ARG1 &_PICK_ (const DEF<decltype (ARGVP1)> & ,_ARG1 &arg1 ,_ARGS &...args) {
	return arg1 ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP2)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<1> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 1) ;
	return _PICK_ (ARGVP1 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP3)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<2> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 2) ;
	return _PICK_ (ARGVP2 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP4)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<3> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 3) ;
	return _PICK_ (ARGVP3 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP5)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<4> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 4) ;
	return _PICK_ (ARGVP4 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP6)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<5> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 5) ;
	return _PICK_ (ARGVP5 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP7)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<6> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 6) ;
	return _PICK_ (ARGVP6 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP8)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<7> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 7) ;
	return _PICK_ (ARGVP7 ,args...) ;
}

template <class _ARG1 ,class... _ARGS>
inline auto _PICK_ (const DEF<decltype (ARGVP9)> & ,_ARG1 &arg1 ,_ARGS &...args)
->DEF<INDEX_TO_TYPE<ARGC<8> ,ARGVS<_ARG1 ,_ARGS...>>> {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) >= 8) ;
	return _PICK_ (ARGVP8 ,args...) ;
}
} ;

template <class HINT>
class Expression<RANK1 ,HINT> :private LexicalTree<Expression<RANK1 ,HINT>> {
private:
	using CURRY_TYPE = Expression<RANK1 ,HINT> ;

	template <class... _ARGS>
	struct FLIP_TYPE {
		using FLIP_RANK = U::RANK_FUNC_TYPE<ARGC<U::constexpr_max_value (_NULL_<ARGV<ARGVS<_ARGS...>>> ())>> ;
		using TYPE = Expression<FLIP_RANK> ;
	} ;

private:
	template <class ,class>
	friend class Expression ;
	using LexicalTree<Expression<RANK1 ,HINT>>::mTree ;

public:
	inline Expression () = default ;

	inline implicit Expression (const Operator &that) : Expression () {
		_DYNAMIC_ASSERT_ (that.rank () == _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK1>>)) ;
		mTree->mOperator = that ;
	}

	inline implicit operator const Expression<RANK1> & () const {
		return Expression<RANK1>::from ((*this)) ;
	}

	inline HINT operator() (const Operand &in1) const {
		_DYNAMIC_ASSERT_ (mTree.exist ()) ;
		if (mTree->mOperand.exist ())
			return mTree->mOperand.template as<HINT> () ;
		const auto r1x = mTree->mOperator (mTree.self ,in1) ;
		return r1x.template as<HINT> () ;
	}

	template <class _ARG1>
	inline auto flip (const ARGV<ARGVP<_ARG1>> &) const
		->DEF<typename FLIP_TYPE<_ARG1>::TYPE> {
		using FLIP_RANK = typename FLIP_TYPE<_ARG1>::FLIP_RANK ;
		return template_flip (_NULL_<ARGV<ARGVS<_ARG1>>> () ,_NULL_<ARGV<FLIP_RANK>> () ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<FLIP_RANK>>>> ()) ;
	}

	inline CURRY_TYPE curry () const {
		return (*this) ;
	}

	inline Expression<RANK0> operator+ (const Expression<RANK0> &that) const ;

	inline Expression<RANK1> operator+ (const Expression<RANK1> &that) const ;

	inline Expression<RANK2> operator+ (const Expression<RANK2> &that) const ;

	inline Expression<RANK3> operator+ (const Expression<RANK3> &that) const ;

	inline Expression<RANK1> operator- (const Expression<RANK1> &that) const ;

	inline Expression<RANK2> operator- (const Expression<RANK2> &that) const ;

	inline Expression<RANK3> operator- (const Expression<RANK3> &that) const ;

private:
	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline Expression<_ARG2> template_flip (const ARGV<ARGVS<_ARG1>> & ,const ARGV<_ARG2> & ,const ARGV<ARGVS<_ARGS...>> &) const {
		Expression<_ARG2> ret ;
		ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<_ARG2>> ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1y = Expression<RANK1>::from (node.mChild[0]) ;
			auto &r2y = _PICK_ (_NULL_<ARGV<ARGVP<_ARG1>>> () ,ins...) ;
			return r1y (r2y) ;
		}) ;
		ret.mTree->mChild[0] = (*this) ;
		return std::move (ret) ;
	}
} ;

template <class HINT>
class Expression<RANK2 ,HINT> :private LexicalTree<Expression<RANK2 ,HINT>> {
private:
	using CURRY_TYPE = Expression<RANK1 ,typename Expression<RANK1>::CURRY_TYPE> ;

	template <class... _ARGS>
	struct FLIP_TYPE {
		using FLIP_RANK = U::RANK_FUNC_TYPE<ARGC<U::constexpr_max_value (_NULL_<ARGV<ARGVS<_ARGS...>>> ())>> ;
		using TYPE = Expression<FLIP_RANK> ;
	} ;

private:
	template <class ,class>
	friend class Expression ;
	using LexicalTree<Expression<RANK2 ,HINT>>::mTree ;

public:
	inline Expression () = default ;

	inline implicit Expression (const Operator &that) : Expression () {
		_DYNAMIC_ASSERT_ (that.rank () == _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK2>>)) ;
		mTree->mOperator = that ;
	}

	inline implicit operator const Expression<RANK2> & () const {
		return Expression<RANK2>::from ((*this)) ;
	}

	inline HINT operator() (const Operand &in1 ,const Operand &in2) const {
		_DYNAMIC_ASSERT_ (mTree.exist ()) ;
		if (mTree->mOperand.exist ())
			return mTree->mOperand.template as<HINT> () ;
		const auto r1x = mTree->mOperator (mTree.self ,in1 ,in2) ;
		return r1x.template as<HINT> () ;
	}

	template <class _ARG1 ,class _ARG2>
	inline auto flip (const ARGV<ARGVP<_ARG1>> & ,const ARGV<ARGVP<_ARG2>> &) const
		->DEF<typename FLIP_TYPE<_ARG1 ,_ARG2>::TYPE> {
		using FLIP_RANK = typename FLIP_TYPE<_ARG1 ,_ARG2>::FLIP_RANK ;
		return template_flip (_NULL_<ARGV<ARGVS<_ARG1 ,_ARG2>>> () ,_NULL_<ARGV<FLIP_RANK>> () ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<FLIP_RANK>>>> ()) ;
	}

	inline CURRY_TYPE curry () const {
		CURRY_TYPE ret ;
		ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK1>> ([] (const LexicalNode &node ,const Operand &in1) {
			auto &r1y = Expression<RANK2>::from (node.mChild[0]) ;
			auto tmp = (r1y + in1).curry () ;
			return Operand (std::move (tmp)) ;
		}) ;
		ret.mTree->mChild[0] = (*this) ;
		return std::move (ret) ;
	}

	inline Expression<RANK1> operator+ (const Expression<RANK0> &that) const ;

	inline Expression<RANK2> operator+ (const Expression<RANK1> &that) const ;

	inline Expression<RANK3> operator+ (const Expression<RANK2> &that) const ;

	inline Expression<RANK2> operator- (const Expression<RANK1> &that) const ;

	inline Expression<RANK3> operator- (const Expression<RANK2> &that) const ;

private:
	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline Expression<_ARG3> template_flip (const ARGV<ARGVS<_ARG1 ,_ARG2>> & ,const ARGV<_ARG3> & ,const ARGV<ARGVS<_ARGS...>> &) const {
		Expression<_ARG3> ret ;
		ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<_ARG3>> ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1y = Expression<RANK2>::from (node.mChild[0]) ;
			auto &r2y = _PICK_ (_NULL_<ARGV<ARGVP<_ARG1>>> () ,ins...) ;
			auto &r3y = _PICK_ (_NULL_<ARGV<ARGVP<_ARG2>>> () ,ins...) ;
			return r1y (r2y ,r3y) ;
		}) ;
		ret.mTree->mChild[0] = (*this) ;
		return std::move (ret) ;
	}
} ;

template <class HINT>
class Expression<RANK3 ,HINT> :private LexicalTree<Expression<RANK3 ,HINT>> {
private:
	using CURRY_TYPE = Expression<RANK1 ,typename Expression<RANK2>::CURRY_TYPE> ;

	template <class... _ARGS>
	struct FLIP_TYPE {
		using FLIP_RANK = U::RANK_FUNC_TYPE<ARGC<U::constexpr_max_value (_NULL_<ARGV<ARGVS<_ARGS...>>> ())>> ;
		using TYPE = Expression<FLIP_RANK> ;
	} ;

private:
	template <class ,class>
	friend class Expression ;
	using LexicalTree<Expression<RANK3 ,HINT>>::mTree ;

public:
	inline Expression () = default ;

	inline implicit Expression (const Operator &that) : Expression () {
		_DYNAMIC_ASSERT_ (that.rank () == _CAPACITYOF_ (INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<RANK3>>)) ;
		mTree->mOperator = that ;
	}

	inline implicit operator const Expression<RANK3> & () const {
		return Expression<RANK3>::from ((*this)) ;
	}

	inline HINT operator() (const Operand &in1 ,const Operand &in2 ,const Operand &in3) const {
		_DYNAMIC_ASSERT_ (mTree.exist ()) ;
		if (mTree->mOperand.exist ())
			return mTree->mOperand.template as<HINT> () ;
		const auto r1x = mTree->mOperator (mTree.self ,in1 ,in2 ,in3) ;
		return r1x.template as<HINT> () ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3>
	inline auto flip (const ARGV<ARGVP<_ARG1>> & ,const ARGV<ARGVP<_ARG2>> & ,const ARGV<ARGVP<_ARG3>> &) const
		->DEF<typename FLIP_TYPE<_ARG1 ,_ARG2 ,_ARG3>::TYPE> {
		using FLIP_RANK = typename FLIP_TYPE<_ARG1 ,_ARG2 ,_ARG3>::FLIP_RANK ;
		return template_flip (_NULL_<ARGV<ARGVS<_ARG1 ,_ARG2 ,_ARG3>>> () ,_NULL_<ARGV<FLIP_RANK>> () ,_NULL_<ARGV<INVOKE_PARAMS_TYPE<REMOVE_POINTER_TYPE<FLIP_RANK>>>> ()) ;
	}

	inline CURRY_TYPE curry () const {
		CURRY_TYPE ret ;
		ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK1>> ([] (const LexicalNode &node ,const Operand &in1) {
			auto &r1y = Expression<RANK3>::from (node.mChild[0]) ;
			auto tmp = (r1y + in1).curry () ;
			return Operand (std::move (tmp)) ;
		}) ;
		ret.mTree->mChild[0] = (*this) ;
		return std::move (ret) ;
	}

	inline Expression<RANK2> operator+ (const Expression<RANK0> &that) const ;

	inline Expression<RANK3> operator+ (const Expression<RANK1> &that) const ;

	inline Expression<RANK3> operator- (const Expression<RANK1> &that) const ;

private:
	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class _ARG4 ,class... _ARGS>
	inline Expression<_ARG4> template_flip (const ARGV<ARGVS<_ARG1 ,_ARG2 ,_ARG3>> & ,const ARGV<_ARG4> & ,const ARGV<ARGVS<_ARGS...>> &) const {
		Expression<_ARG4> ret ;
		ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<_ARG4>> ([] (const LexicalNode &node ,const _ARGS &...ins) {
			auto &r1y = Expression<RANK3>::from (node.mChild[0]) ;
			auto &r2y = _PICK_ (_NULL_<ARGV<ARGVP<_ARG1>>> () ,ins...) ;
			auto &r3y = _PICK_ (_NULL_<ARGV<ARGVP<_ARG2>>> () ,ins...) ;
			auto &r4y = _PICK_ (_NULL_<ARGV<ARGVP<_ARG3>>> () ,ins...) ;
			return r1y (r2y ,r3y ,r4y) ;
		}) ;
		ret.mTree->mChild[0] = (*this) ;
		return std::move (ret) ;
	}
} ;

template <class HINT>
inline Expression<RANK0> Expression<RANK0 ,HINT>::operator- (const Expression<RANK1> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK1> Expression<RANK0 ,HINT>::operator- (const Expression<RANK2> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK2> Expression<RANK0 ,HINT>::operator- (const Expression<RANK3> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK0> Expression<RANK1 ,HINT>::operator+ (const Expression<RANK0> &that) const {
	Expression<RANK0> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK0>> ([] (const LexicalNode &node) {
		auto &r1y = Expression<RANK1>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK0>::from (node.mChild[1]) ;
		return r1y (r2y ()) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK1> Expression<RANK1 ,HINT>::operator+ (const Expression<RANK1> &that) const {
	Expression<RANK1> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK1>> ([] (const LexicalNode &node ,const Operand &in1) {
		auto &r1y = Expression<RANK1>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK1>::from (node.mChild[1]) ;
		return r1y (r2y (in1)) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK2> Expression<RANK1 ,HINT>::operator+ (const Expression<RANK2> &that) const {
	Expression<RANK2> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK2>> ([] (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) {
		auto &r1y = Expression<RANK1>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK2>::from (node.mChild[1]) ;
		return r1y (r2y (in1 ,in2)) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK3> Expression<RANK1 ,HINT>::operator+ (const Expression<RANK3> &that) const {
	Expression<RANK3> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK3>> ([] (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) {
		auto &r1y = Expression<RANK1>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK3>::from (node.mChild[1]) ;
		return r1y (r2y (in1 ,in2 ,in3)) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK1> Expression<RANK1 ,HINT>::operator- (const Expression<RANK1> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK2> Expression<RANK1 ,HINT>::operator- (const Expression<RANK2> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK3> Expression<RANK1 ,HINT>::operator- (const Expression<RANK3> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK1> Expression<RANK2 ,HINT>::operator+ (const Expression<RANK0> &that) const {
	Expression<RANK1> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK1>> ([] (const LexicalNode &node ,const Operand &in1) {
		auto &r1y = Expression<RANK2>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK0>::from (node.mChild[1]) ;
		return r1y (r2y () ,in1) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK2> Expression<RANK2 ,HINT>::operator+ (const Expression<RANK1> &that) const {
	Expression<RANK2> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK2>> ([] (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) {
		auto &r1y = Expression<RANK2>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK1>::from (node.mChild[1]) ;
		return r1y (r2y (in1) ,in2) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK3> Expression<RANK2 ,HINT>::operator+ (const Expression<RANK2> &that) const {
	Expression<RANK3> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK3>> ([] (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) {
		auto &r1y = Expression<RANK2>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK2>::from (node.mChild[1]) ;
		return r1y (r2y (in1 ,in2) ,in3) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK2> Expression<RANK2 ,HINT>::operator- (const Expression<RANK1> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK3> Expression<RANK2 ,HINT>::operator- (const Expression<RANK2> &that) const {
	return that + (*this) ;
}

template <class HINT>
inline Expression<RANK2> Expression<RANK3 ,HINT>::operator+ (const Expression<RANK0> &that) const {
	Expression<RANK2> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK2>> ([] (const LexicalNode &node ,const Operand &in1 ,const Operand &in2) {
		auto &r1y = Expression<RANK3>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK0>::from (node.mChild[1]) ;
		return r1y (r2y () ,in1 ,in2) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK3> Expression<RANK3 ,HINT>::operator+ (const Expression<RANK1> &that) const {
	Expression<RANK3> ret ;
	ret.mTree->mOperator = _XVALUE_<U::CONTEXT_RANK_FUNC_TYPE<RANK3>> ([] (const LexicalNode &node ,const Operand &in1 ,const Operand &in2 ,const Operand &in3) {
		auto &r1y = Expression<RANK3>::from (node.mChild[0]) ;
		auto &r2y = Expression<RANK1>::from (node.mChild[1]) ;
		return r1y (r2y (in1) ,in2 ,in3) ;
	}) ;
	ret.mTree->mChild[0] = (*this) ;
	ret.mTree->mChild[1] = that ;
	return std::move (ret) ;
}

template <class HINT>
inline Expression<RANK3> Expression<RANK3 ,HINT>::operator- (const Expression<RANK1> &that) const {
	return that + (*this) ;
}
} ;