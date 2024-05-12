#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_matrix.hpp"

#include "csc_end.h"
#include <Eigen/Dense>

#include <unsupported/Eigen/NonLinearOptimization>
#include "csc_begin.h"

namespace CSC {
struct MatrixToolPureLayout {} ;

class MatrixProcImplement implement Fat<MatrixToolHolder ,MatrixToolLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<MatrixToolPureLayout>::make () ;
	}

	XTR_RESULT solve_xtr (CREF<Matrix> a) const override {
		XTR_RESULT ret ;
		ret.mX = a.homogenize () ;
		ret.mR = Matrix::identity () ;
		ret.mT = Matrix::identity () ;
		auto rax = TRUE ;
		while (TRUE) {
			rax = FALSE ;
			if ifdo (TRUE) {
				if (MathTool::inverse (ret.mX[1][0]) == 0)
					discard ;
				const auto r1x = ret.mX[1][0] ;
				const auto r2x = ret.mX[1][1] ;
				const auto r3x = MathTool::inverse (Vector (r1x ,r2x ,0 ,0).magnitude ()) ;
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
				if (MathTool::inverse (ret.mX[2][0]) == 0)
					discard ;
				const auto r5x = ret.mX[2][0] ;
				const auto r6x = ret.mX[2][2] ;
				const auto r7x = MathTool::inverse (Vector (r5x ,r6x ,0 ,0).magnitude ()) ;
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
				if (MathTool::inverse (ret.mX[2][1]) == 0)
					discard ;
				const auto r9x = ret.mX[2][1] ;
				const auto r10x = ret.mX[2][2] ;
				const auto r11x = MathTool::inverse (Vector (r9x ,r10x ,0 ,0).magnitude ()) ;
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

	SVD_RESULT solve_svd (CREF<Matrix> a) const override {
		SVD_RESULT ret ;
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

exports VFat<MatrixToolHolder> MatrixToolHolder::create (VREF<MatrixToolLayout> that) {
	return VFat<MatrixToolHolder> (MatrixProcImplement () ,Pointer::from (that)) ;
}

exports CFat<MatrixToolHolder> MatrixToolHolder::create (CREF<MatrixToolLayout> that) {
	return CFat<MatrixToolHolder> (MatrixProcImplement () ,Pointer::from (that)) ;
}
} ;