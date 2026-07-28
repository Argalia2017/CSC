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
class LinearProcImplHolder final implement Fat<LinearProcHolder ,LinearProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	SVDResult solve_svd (CR<Matrix> a) const override {
		SVDResult ret ;
		const auto r1x = cvt_eigen_matrix (a) ;
		const auto r2x = csc_uint32_t (Eigen::ComputeFullU | Eigen::ComputeFullV) ;
		auto rax = Eigen::JacobiSVD<Eigen::Matrix4d> (r1x ,r2x) ;
		ret.mU = cvt_eigen_matrix (rax.matrixU ()) ;
		const auto r3x = Eigen::Vector4d (rax.singularValues ()) ;
		ret.mS = DiagMatrix (r3x[0] ,r3x[1] ,r3x[2] ,r3x[3]) ;
		ret.mV = cvt_eigen_matrix (rax.matrixV ()) ;
		return move (ret) ;
	}

	Eigen::Matrix4d cvt_eigen_matrix (CR<Matrix> a) const {
		Eigen::Matrix4d ret ;
		for (auto &&i : range (0 ,4 ,0 ,4))
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Matrix cvt_eigen_matrix (CR<Eigen::Matrix4d> a) const {
		Matrix ret ;
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			ret[i] = a (i.mY ,i.mX) ;
			assume (!MathProc::is_inf (ret[i])) ;
		}
		return move (ret) ;
	}

	Array<Flt64> solve_eig (CR<Image<Flt64>> a) const override {
		const auto r1x = MathProc::min_of (a.cx () ,a.cy ()) ;
		Array<Flt64> ret = Array<Flt64> (r1x) ;
		const auto r2x = cvt_eigen_image (a) ;
		const auto r3x = Eigen::EigenvaluesOnly ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r2x ,r3x) ;
		const auto r4x = rax.singularValues () ;
		for (auto &&i : ret.iter ())
			ret[i] = r4x[i] ;
		return move (ret) ;
	}

	Image<Flt64> solve_lsm (CR<Image<Flt64>> a) const override {
		Image<Flt64> ret = Image<Flt64> (1 ,a.cx ()) ;
		const auto r1x = cvt_eigen_image (a) ;
		const auto r2x = Eigen::ComputeFullV ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r1x ,r2x) ;
		Index ix = MathProc::min_of (Index (rax.rank ()) ,a.cx () - 1) ;
		assume (ix >= 0) ;
		const auto r3x = cvt_eigen_image (rax.matrixV ()) ;
		for (auto &&i : range (0 ,ret.cy ()))
			ret[i][0] = r3x[i][ix] ;
		return move (ret) ;
	}

	Image<Flt64> solve_lsm (CR<Image<Flt64>> a ,CR<Image<Flt64>> b) const override {
		const auto r1x = cvt_eigen_image (a) ;
		const auto r2x = cvt_eigen_image (b) ;
		const auto r3x = Eigen::MatrixXd (r1x.transpose () * r1x) ;
		const auto r4x = Eigen::MatrixXd (r1x.transpose () * r2x) ;
		const auto r5x = csc_uint32_t (Eigen::ComputeThinU | Eigen::ComputeThinV) ;
		auto rax = Eigen::JacobiSVD<Eigen::MatrixXd> (r3x ,r5x) ;
		const auto r6x = rax.solve (r4x) ;
		return cvt_eigen_image (r6x) ;
	}

	Image<Flt64> solve_inv (CR<Image<Flt64>> a) const override {
		const auto r1x = cvt_eigen_image (a) ;
		const auto r2x = Eigen::MatrixXd (r1x.completeOrthogonalDecomposition ().pseudoInverse ()) ;
		return cvt_eigen_image (r2x) ;
	}

	Eigen::MatrixXd cvt_eigen_image (CR<Image<Flt64>> a) const {
		Eigen::MatrixXd ret = Eigen::MatrixXd (a.cy () ,a.cx ()) ;
		for (auto &&i : a.iter ())
			ret (i.mY ,i.mX) = a[i] ;
		return move (ret) ;
	}

	Image<Flt64> cvt_eigen_image (CR<Eigen::MatrixXd> a) const {
		Image<Flt64> ret = Image<Flt64> (a.cols () ,a.rows ()) ;
		for (auto &&i : ret.iter ())
			ret[i] = a (i.mY ,i.mX) ;
		return move (ret) ;
	}
} ;

static const auto mLinearProcExternal = External<LinearProcHolder ,LinearProcLayout> (LinearProcImplHolder ()) ;
} ;