#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_matrix.hpp"

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_end.h"
#ifdef __CSC_CXX_LATEST__
namespace std {
template<class _Fn>
class unary_negate
{	// functor adapter !_Func(left)
public:
	typedef typename _Fn::argument_type argument_type;
	typedef bool result_type;

	constexpr explicit unary_negate (const _Fn &_Func)
		: _Functor (_Func) {	// construct from functor
	}

	constexpr bool operator()(const argument_type &_Left) const {	// apply functor to operand
		return (!_Functor (_Left));
	}

private:
	_Fn _Functor;	// the functor to apply
};

template<class _Fn>
class binary_negate
{	// functor adapter !_Func(left, right)
public:
	typedef typename _Fn::first_argument_type first_argument_type;
	typedef typename _Fn::second_argument_type second_argument_type;
	typedef bool result_type;

	constexpr explicit binary_negate (const _Fn &_Func)
		: _Functor (_Func) {	// construct from functor
	}

	constexpr bool operator()(const first_argument_type &_Left ,
		const second_argument_type &_Right) const {	// apply functor to operands
		return (!_Functor (_Left ,_Right));
	}

private:
	_Fn _Functor;	// the functor to apply
};
} ;
#endif

#include <Eigen/Dense>

#include <unsupported/Eigen/NonLinearOptimization>
#include "csc_begin.h"

namespace CSC {
struct MatrixProcPureLayout {} ;

class MatrixProcImplement implement Fat<MatrixProcHolder ,MatrixProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<MatrixProcPureLayout>::make () ;
	}

	XTRResult solve_xtr (CREF<Matrix> a) const override {
		XTRResult ret ;
		ret.mX = a.homogenize () ;
		ret.mR = Matrix::identity () ;
		ret.mT = Matrix::identity () ;
		auto rax = TRUE ;
		while (TRUE) {
			rax = FALSE ;
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mX[1][0]) == 0)
					discard ;
				const auto r1x = ret.mX[1][0] ;
				const auto r2x = ret.mX[1][1] ;
				const auto r3x = MathProc::inverse (Vector (r1x ,r2x ,0 ,0).magnitude ()) ;
				const auto r4x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[0][0] = r2x * r3x ;
					ret[1][1] = ret[0][0] ;
					ret[0][1] = r1x * r3x ;
					ret[1][0] = -ret[0][1] ;
					return move (ret) ;
				}) ;
				ret.mX = ret.mX * r4x ;
				ret.mR = r4x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mX[2][0]) == 0)
					discard ;
				const auto r5x = ret.mX[2][0] ;
				const auto r6x = ret.mX[2][2] ;
				const auto r7x = MathProc::inverse (Vector (r5x ,r6x ,0 ,0).magnitude ()) ;
				const auto r8x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[0][0] = r6x * r7x ;
					ret[2][2] = ret[0][0] ;
					ret[0][2] = r5x * r7x ;
					ret[2][0] = -ret[0][2] ;
					return move (ret) ;
				}) ;
				ret.mX = ret.mX * r8x ;
				ret.mR = r8x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mX[2][1]) == 0)
					discard ;
				const auto r9x = ret.mX[2][1] ;
				const auto r10x = ret.mX[2][2] ;
				const auto r11x = MathProc::inverse (Vector (r9x ,r10x ,0 ,0).magnitude ()) ;
				const auto r12x = invoke ([&] () {
					Matrix ret = Matrix::identity () ;
					ret[1][1] = r10x * r11x ;
					ret[2][2] = ret[1][1] ;
					ret[1][2] = r9x * r11x ;
					ret[2][1] = -ret[1][2] ;
					return move (ret) ;
				}) ;
				ret.mX = ret.mX * r12x ;
				ret.mR = r12x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifnot (rax)
				break ;
		}
		return move (ret) ;
	}

	SVDResult solve_svd (CREF<Matrix> a) const override {
		SVDResult ret ;
		const auto r1x = cvt_eigen_matrix (a) ;
		auto rax = Eigen::JacobiSVD<Eigen::Matrix4d> (r1x ,Eigen::ComputeFullU | Eigen::ComputeFullV) ;
		ret.mU = cvt_csc_matrix (rax.matrixU ()) ;
		const auto r2x = Eigen::Vector4d (rax.singularValues ()) ;
		ret.mS = DiagMatrix::make (r2x[0] ,r2x[1] ,r2x[2] ,r2x[3]) ;
		ret.mV = cvt_csc_matrix (rax.matrixV ()) ;
		return move (ret) ;
	}

	Eigen::Matrix4d cvt_eigen_matrix (CREF<Matrix> a) const {
		Eigen::Matrix4d ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4))
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Matrix cvt_csc_matrix (CREF<Eigen::Matrix4d> a) const {
		Matrix ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret[i] = a (i.mY ,i.mX) ;
			assume (ifnot (isnan (ret[i]))) ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MatrixProcHolder> MatrixProcHolder::create (VREF<MatrixProcLayout> that) {
	return VFat<MatrixProcHolder> (MatrixProcImplement () ,that) ;
}

exports CFat<MatrixProcHolder> MatrixProcHolder::create (CREF<MatrixProcLayout> that) {
	return CFat<MatrixProcHolder> (MatrixProcImplement () ,that) ;
}
} ;