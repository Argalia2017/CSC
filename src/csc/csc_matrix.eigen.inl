#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_matrix.hpp"

#include "csc_end.h"
#include "csc_matrix.eigen.fix.h"
#include <Eigen/Dense>
#include "csc_begin.h"

namespace CSC {
class MatrixProcImplHolder final implement Fat<MatrixProcHolder ,MatrixProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	TRSResult solve_trs (CR<Matrix> a) const override {
		TRSResult ret ;
		const auto r1x = a * Vector::axis_x () ;
		const auto r2x = a * Vector::axis_y () ;
		const auto r3x = a * Vector::axis_z () ;
		const auto r4x = a * Vector::axis_w () ;
		const auto r5x = Matrix (r1x.normalize () ,r2x.normalize () ,r3x.normalize () ,Vector::axis_w ()) ;
		const auto r6x = MathProc::sign (r5x.determinant ()) ;
		const auto r7x = DiagMatrix (r6x ,r6x ,r6x ,1) ;
		ret.mT = TranslationMatrix (r4x) ;
		ret.mR = Quaternion (r5x * r7x).matrix () ;
		ret.mS = r7x * DiagMatrix (r1x.magnitude () ,r2x.magnitude () ,r3x.magnitude ()) ;
		return move (ret) ;
	}

	KRTResult solve_krt (CR<Matrix> a) const override {
		KRTResult ret ;
		ret.mK = a.homogenize () ;
		ret.mR = Matrix::iden () ;
		ret.mT = Matrix::iden () ;
		auto rax = TRUE ;
		while (TRUE) {
			rax = FALSE ;
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mK[1][0]) == 0)
					discard ;
				const auto r1x = ret.mK[1][0] ;
				const auto r2x = ret.mK[1][1] ;
				const auto r3x = MathProc::inverse (Vector (r1x ,r2x ,0 ,0).magnitude ()) ;
				const auto r4x = invoke ([&] () {
					Matrix ret = Matrix::iden () ;
					ret[0][0] = r2x * r3x ;
					ret[1][1] = ret[0][0] ;
					ret[0][1] = r1x * r3x ;
					ret[1][0] = -ret[0][1] ;
					return move (ret) ;
				}) ;
				ret.mK = ret.mK * r4x ;
				ret.mR = r4x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mK[2][0]) == 0)
					discard ;
				const auto r5x = ret.mK[2][0] ;
				const auto r6x = ret.mK[2][2] ;
				const auto r7x = MathProc::inverse (Vector (r5x ,r6x ,0 ,0).magnitude ()) ;
				const auto r8x = invoke ([&] () {
					Matrix ret = Matrix::iden () ;
					ret[0][0] = r6x * r7x ;
					ret[2][2] = ret[0][0] ;
					ret[0][2] = r5x * r7x ;
					ret[2][0] = -ret[0][2] ;
					return move (ret) ;
				}) ;
				ret.mK = ret.mK * r8x ;
				ret.mR = r8x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if ifdo (TRUE) {
				if (MathProc::inverse (ret.mK[2][1]) == 0)
					discard ;
				const auto r9x = ret.mK[2][1] ;
				const auto r10x = ret.mK[2][2] ;
				const auto r11x = MathProc::inverse (Vector (r9x ,r10x ,0 ,0).magnitude ()) ;
				const auto r12x = invoke ([&] () {
					Matrix ret = Matrix::iden () ;
					ret[1][1] = r10x * r11x ;
					ret[2][2] = ret[1][1] ;
					ret[1][2] = r9x * r11x ;
					ret[2][1] = -ret[1][2] ;
					return move (ret) ;
				}) ;
				ret.mK = ret.mK * r12x ;
				ret.mR = r12x.transpose () * ret.mR ;
				rax = TRUE ;
			}
			if (!rax)
				break ;
		}
		return move (ret) ;
	}

	SVDResult solve_svd (CR<Matrix> a) const override {
		SVDResult ret ;
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = csc_uint32_t (Eigen::ComputeFullU | Eigen::ComputeFullV) ;
		auto rax = Eigen::JacobiSVD<Eigen::Matrix4d> (r1x ,r2x) ;
		ret.mU = cvt_csc_matrix (rax.matrixU ()) ;
		const auto r3x = Eigen::Vector4d (rax.singularValues ()) ;
		ret.mS = DiagMatrix (r3x[0] ,r3x[1] ,r3x[2] ,r3x[3]) ;
		ret.mV = cvt_csc_matrix (rax.matrixV ()) ;
		return move (ret) ;
	}

	Eigen::Matrix4d cvt_eigen_matrix (CR<Matrix> a) const {
		Eigen::Matrix4d ret ;
		for (auto &&i : range (0 ,4 ,0 ,4))
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Matrix cvt_csc_matrix (CR<Eigen::Matrix4d> a) const {
		Matrix ret ;
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			ret[i] = a (i.mY ,i.mX) ;
			assume (!MathProc::is_inf (ret[i])) ;
		}
		return move (ret) ;
	}
} ;

static const auto mMatrixProcExternal = External<MatrixProcHolder ,MatrixProcLayout> (MatrixProcImplHolder ()) ;

class LinearProcImplHolder final implement Fat<LinearProcHolder ,LinearProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Image<Flt64> solve_lsm (CR<Image<Flt64>> a) const override {
		Image<Flt64> ret = Image<Flt64> (1 ,a.cx ()) ;
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = Eigen::ComputeFullV ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r1x ,r2x) ;
		Index ix = MathProc::min_of (Index (rax.rank ()) ,a.cx () - 1) ;
		assume (ix >= 0) ;
		const auto r3x = cvt_csc_matrix (rax.matrixV ()) ;
		for (auto &&i : range (0 ,ret.cy ()))
			ret[i][0] = r3x[i][ix] ;
		return move (ret) ;
	}

	Image<Flt64> solve_lsm (CR<Image<Flt64>> a ,CR<Image<Flt64>> b) const override {
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = cvt_eigen_matrix (b) ;
		const auto r3x = Eigen::MatrixXd (r1x.transpose () * r1x) ;
		const auto r4x = Eigen::MatrixXd (r1x.transpose () * r2x) ;
		const auto r5x = csc_uint32_t (Eigen::ComputeThinU | Eigen::ComputeThinV) ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r3x ,r5x) ;
		const auto r6x = rax.solve (r4x) ;
		return cvt_csc_matrix (r6x) ;
	}

	Image<Flt64> solve_inv (CR<Image<Flt64>> a) const override {
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = Eigen::MatrixXd (r1x.completeOrthogonalDecomposition ().pseudoInverse ()) ;
		return cvt_csc_matrix (r2x) ;
	}

	Eigen::MatrixXd cvt_eigen_matrix (CR<Image<Flt64>> a) const {
		Eigen::MatrixXd ret = Eigen::MatrixXd (a.cy () ,a.cx ()) ;
		for (auto &&i : a.iter ())
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Image<Flt64> cvt_csc_matrix (CR<Eigen::MatrixXd> a) const {
		Image<Flt64> ret = Image<Flt64> (a.cols () ,a.rows ()) ;
		for (auto &&i : ret.iter ())
			ret[i] = a (i.mY ,i.mX) ;
		return move (ret) ;
	}
} ;

static const auto mLinearProcExternal = External<LinearProcHolder ,LinearProcLayout> (LinearProcImplHolder ()) ;
} ;