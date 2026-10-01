#pragma once

#ifndef __CSC_NUMERIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_numeric.hpp"

#ifdef ERROR
#pragma push_macro ("ERROR")
#undef ERROR
#endif

#include "csc_end.h"
#include "csc_matrix.eigen.fix.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#include <corecrt_math_defines.h>
#endif
#include <math.h>
#include <vector>
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include "csc_begin.h"

#ifndef ERROR
#pragma pop_macro ("ERROR")
#endif

namespace CSC {
struct NumericProblemLayout {
	Array<Flt64> mCoeffient ;
	Index mWrite ;
	AutoRef<ceres::Problem> mProblem ;
	AutoRef<ceres::Solver::Options> mOptions ;
	AutoRef<ceres::Solver::Summary> mSummary ;
} ;

class NumericProblemImplHolder final implement Fat<NumericProblemHolder ,NumericProblemLayout> {
public:
	void initialize (CR<Length> size_) override {
		assert (size_ > 0) ;
		self.mCoeffient = Array<Flt64> (size_) ;
		self.mWrite = 0 ;
		clear () ;
	}

	void clear () override {
		self.mWrite = 0 ;
		self.mProblem = AutoRef<ceres::Problem>::make () ;
		self.mOptions = AutoRef<ceres::Solver::Options>::make () ;
		self.mSummary = AutoRef<ceres::Solver::Summary>::make () ;
		self.mOptions->logging_type = ceres::LoggingType::SILENT ;
		self.mOptions->minimizer_progress_to_stdout = FALSE ;
	}

	Length size () const override {
		return self.mCoeffient.size () ;
	}

	VR<Flt64> at (CR<Index> index) leftvalue override {
		assert (inline_mid (index ,0 ,self.mWrite)) ;
		return self.mCoeffient[index] ;
	}

	CR<Flt64> at (CR<Index> index) const leftvalue override {
		assert (inline_mid (index ,0 ,self.mWrite)) ;
		return self.mCoeffient[index] ;
	}

	Index insert (CR<Length> size_) override {
		assert (size_ > 0) ;
		Index ret = self.mWrite ;
		self.mWrite += size_ ;
		assert (self.mWrite <= self.mCoeffient.size ()) ;
		return move (ret) ;
	}

	void set_const (CR<Index> index) override {
		self.mProblem->SetParameterBlockConstant ((&self.mCoeffient[index])) ;
	}

	void set_bound (CR<Index> index ,CR<Flt64> low ,CR<Flt64> high) override {
		assume (low <= high) ;
		if ifdo (TRUE) {
			if (MathProc::is_inf (low))
				discard ;
			self.mProblem->SetParameterLowerBound ((&self.mCoeffient[index]) ,0 ,low) ;
		}
		if ifdo (TRUE) {
			if (MathProc::is_inf (high))
				discard ;
			self.mProblem->SetParameterUpperBound ((&self.mCoeffient[index]) ,0 ,high) ;
		}
	}

	void add_block (RR<AutoRef<Pointer>> cost_function ,CR<Wrapper<Index>> coeff) override {
		assume (coeff.rank () > 0) ;
		auto rax = std::vector<PTR<VR<Flt64>>> () ;
		for (auto &&i : range (0 ,coeff.rank ()))
			rax.push_back ((&self.mCoeffient[coeff[i]])) ;
		const auto r1x = cost_function.rebind (TYPE<PTR<VR<ceres::CostFunction>>>::expr).ref ;
		const auto r2x = new ceres::TrivialLoss () ;
		self.mProblem->AddResidualBlock (r1x ,r2x ,rax) ;
	}

	NumericResult solve () override {
		NumericResult ret ;
		ceres::Solve (self.mOptions.ref ,(&self.mProblem.ref) ,(&self.mSummary.ref)) ;
		ret.mInitialCost = self.mSummary->initial_cost ;
		ret.mFinalCost = self.mSummary->final_cost ;
		ret.mIteration = Length (self.mSummary->iterations.size ()) ;
		ret.mConverged = self.mSummary->termination_type == ceres::TerminationType::CONVERGENCE ;
		return move (ret) ;
	}
} ;

static const auto mNumericProblemExternal = External<NumericProblemHolder ,NumericProblemLayout> (NumericProblemImplHolder ()) ;
} ;
