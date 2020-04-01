#pragma once

#ifndef __CSC_FUNCTIONAL__
#error "б╞(д├бузебу ;)д├ : require 'csc_functional.hpp'"
#endif

namespace CSC {
inline namespace FUNCTIONAL {
template <class...>
struct op_equal ;

template <class _ARG1>
struct op_equal<_ARG1 ,_ARG1> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG1 &arg2) {
			return BOOL (arg1 == arg2) ;
		}) ;
	}
} ;

template <class...>
struct op_compr ;

template <class _ARG1>
struct op_compr<_ARG1 ,_ARG1> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG1 &arg2) {
			return _MEMCOMPR_ (PTRTOARR[&arg1] ,PTRTOARR[&arg2] ,1) ;
		}) ;
	}
} ;

template <class...>
struct op_not ;

template <class _ARG1>
struct op_not<_ARG1> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			return _XVALUE_<_ARG1> (~arg1) ;
		}) ;
	}
} ;

template <>
struct op_not<BOOL> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const BOOL &arg1) {
			return BOOL (!arg1) ;
		}) ;
	}
} ;

template <class...>
struct op_and ;

template <class _ARG1>
struct op_and<_ARG1 ,_ARG1> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG1 &arg2) {
			return _XVALUE_<_ARG1> (arg1 & arg2) ;
		}) ;
	}
} ;

template <>
struct op_and<BOOL ,BOOL> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const BOOL &arg1 ,const BOOL &arg2) {
			return BOOL (arg1 && arg2) ;
		}) ;
	}
} ;

template <class...>
struct op_or ;

template <class _ARG1>
struct op_or<_ARG1 ,_ARG1> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG1 &arg2) {
			return _XVALUE_<_ARG1> (arg1 | arg2) ;
		}) ;
	}
} ;

template <>
struct op_or<BOOL ,BOOL> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const BOOL &arg1 ,const BOOL &arg2) {
			return BOOL (arg1 || arg2) ;
		}) ;
	}
} ;

template <class...>
struct op_xor ;

template <class _ARG1>
struct op_xor<_ARG1 ,_ARG1> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG1 &arg2) {
			return _XVALUE_<_ARG1> (arg1 ^ arg2) ;
		}) ;
	}
} ;

template <>
struct op_xor<BOOL ,BOOL> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const BOOL &arg1 ,const BOOL &arg2) {
			return BOOL (arg1 != arg2) ;
		}) ;
	}
} ;

template <class...>
struct op_plus ;

template <class _ARG1>
struct op_plus<_ARG1> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			return +arg1 ;
		}) ;
	}
} ;

template <class...>
struct op_minus ;

template <class _ARG1>
struct op_minus<_ARG1> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			return -arg1 ;
		}) ;
	}
} ;

template <class...>
struct op_increase ;

template <class _ARG1>
struct op_increase<_ARG1> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			_ARG1 ret = arg1 ;
			++ret ;
			return std::move (ret) ;
		}) ;
	}
} ;

template <class...>
struct op_decrease ;

template <class _ARG1>
struct op_decrease<_ARG1> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			_ARG1 ret = arg1 ;
			--ret ;
			return std::move (ret) ;
		}) ;
	}
} ;

template <class...>
struct op_add ;

template <class _ARG1 ,class _ARG2>
struct op_add<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 + arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_sub ;

template <class _ARG1 ,class _ARG2>
struct op_sub<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 - arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_mul ;

template <class _ARG1 ,class _ARG2>
struct op_mul<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 * arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_div ;

template <class _ARG1 ,class _ARG2>
struct op_div<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 / arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_mod ;

template <class _ARG1 ,class _ARG2>
struct op_mod<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 % arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_cross ;

template <class _ARG1 ,class _ARG2>
struct op_cross<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 ^ arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_cast ;

template <class _ARG1 ,class _ARG2>
struct op_cast<_ARG1 ,_ARG2> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			return _XVALUE_<_ARG2> (arg1) ;
		}) ;
	}
} ;

template <class...>
struct op_get ;

template <class _ARG1>
struct op_get<_ARG1 ,INDEX> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const INDEX &arg2) {
			return arg1[arg2] ;
		}) ;
	}
} ;

template <class...>
struct op_call ;

template <class _ARG1>
struct op_call<_ARG1> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const _ARG1 &arg1) {
			return arg1 () ;
		}) ;
	}
} ;

template <class _ARG1 ,class _ARG2>
struct op_call<_ARG1 ,_ARG2> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2) {
			return arg1 (arg2) ;
		}) ;
	}
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct op_call<_ARG1 ,_ARG2 ,_ARG3> {
	inline static Expression<RANK3> compile () {
		return Operator ([] (const _ARG1 &arg1 ,const _ARG2 &arg2 ,const _ARG3 &arg3) {
			return arg1 (arg2 ,arg3) ;
		}) ;
	}
} ;

template <class...>
struct op_switch ;

template <>
struct op_switch<BOOL ,Operand ,Operand> {
	inline static Expression<RANK3> compile () {
		return Operator ([] (const BOOL &flag ,const Operand &case1 ,const Operand &case2) {
			if (flag)
				return case1 ;
			return case2 ;
		}) ;
	}
} ;

template <class...>
struct op_assert ;

template <>
struct op_assert<BOOL ,Operand> {
	inline static Expression<RANK2> compile () {
		return Operator ([] (const BOOL &arg1 ,const Operand &arg2) {
			_DYNAMIC_ASSERT_ (arg1) ;
			return arg2 ;
		}) ;
	}
} ;

template <class...>
struct op_tuple ;

template <>
struct op_tuple<Operand> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const Operand &arg1) {
			const auto r1x = Operator ([] (const Operand &sw ,const Operand &arg1_) {
				if (sw == Operand::nth (ARGVP1))
					return arg1_ ;
				_DYNAMIC_ASSERT_ (FALSE) ;
				return Operand () ;
			}) ;
			return Expression<RANK2> (r1x).flip () + Expression<RANK0> (arg1) ;
		}) ;
	}
} ;

template <>
struct op_tuple<Operand ,Operand> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const Operand &arg1 ,const Operand &arg2) {
			const auto r1x = Operator ([] (const Operand &sw ,const Operand &arg1_ ,const Operand &arg2_) {
				if (sw == Operand::nth (ARGVP1))
					return arg1_ ;
				if (sw == Operand::nth (ARGVP2))
					return arg2_ ;
				_DYNAMIC_ASSERT_ (FALSE) ;
				return Operand () ;
			}) ;
			return Expression<RANK3> (r1x).flip () + Expression<RANK0> (arg1) + Expression<RANK0> (arg2) ;
		}) ;
	}
} ;

template <>
struct op_tuple<Operand ,Operand ,Operand> {
	inline static Expression<RANK1> compile () {
		return Operator ([] (const Operand &arg1 ,const Operand &arg2 ,const Operand &arg3) {
			const auto r1x = Operator ([] (const Operand &sw ,const Operand &arg1_ ,const Operand &arg2_ ,const Operand &arg3_) {
				if (sw == Operand::nth (ARGVP1))
					return arg1_ ;
				if (sw == Operand::nth (ARGVP2))
					return arg2_ ;
				if (sw == Operand::nth (ARGVP3))
					return arg3_ ;
				_DYNAMIC_ASSERT_ (FALSE) ;
				return Operand () ;
			}) ;
			return Expression<RANK4> (r1x).flip () + Expression<RANK0> (arg1) + Expression<RANK0> (arg2) + Expression<RANK0> (arg3) ;
		}) ;
	}
} ;
} ;
} ;