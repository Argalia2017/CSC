#pragma once

#ifndef __CSC_MATRIX__
#define __CSC_MATRIX__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_image.hpp"
#include "csc_matrix.hpp"

namespace CSC {
template <class A>
class VectorImplHolder implement Fat<VectorHolder<A> ,VectorLayout<A>> {
public:
	void initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) override {
		thix.mVector[0] = x ;
		thix.mVector[1] = y ;
		thix.mVector[2] = z ;
		thix.mVector[3] = w ;
	}

	void initialize (CREF<PIXEL> xy) override {
		thix.mVector[0] = A (xy.mX) + A (0.5) ;
		thix.mVector[1] = A (xy.mY) + A (0.5) ;
		thix.mVector[2] = 0 ;
		thix.mVector[3] = 1 ;
	}

	void initialize (CREF<POINT2F> xy) override {
		thix.mVector[0] = A (xy.mX) ;
		thix.mVector[1] = A (xy.mY) ;
		thix.mVector[2] = 0 ;
		thix.mVector[3] = 1 ;
	}

	void initialize (CREF<POINT3F> xyz) override {
		thix.mVector[0] = A (xyz.mX) ;
		thix.mVector[1] = A (xyz.mY) ;
		thix.mVector[2] = A (xyz.mZ) ;
		thix.mVector[3] = 1 ;
	}

	CREF<VectorLayout<A>> zero () const override {
		return memorize ([&] () {
			VectorLayout<A> ret ;
			VectorHolder<A>::create (ret)->initialize (0 ,0 ,0 ,0) ;
			return move (ret) ;
		}) ;
	}

	CREF<VectorLayout<A>> axis_x () const override {
		return memorize ([&] () {
			VectorLayout<A> ret ;
			VectorHolder<A>::create (ret)->initialize (1 ,0 ,0 ,0) ;
			return move (ret) ;
		}) ;
	}

	CREF<VectorLayout<A>> axis_y () const override {
		return memorize ([&] () {
			VectorLayout<A> ret ;
			VectorHolder<A>::create (ret)->initialize (0 ,1 ,0 ,0) ;
			return move (ret) ;
		}) ;
	}

	CREF<VectorLayout<A>> axis_z () const override {
		return memorize ([&] () {
			VectorLayout<A> ret ;
			VectorHolder<A>::create (ret)->initialize (0 ,0 ,1 ,0) ;
			return move (ret) ;
		}) ;
	}

	CREF<VectorLayout<A>> axis_w () const override {
		return memorize ([&] () {
			VectorLayout<A> ret ;
			VectorHolder<A>::create (ret)->initialize (0 ,0 ,0 ,1) ;
			return move (ret) ;
		}) ;
	}

	POINT2F xy () const override {
		POINT2F ret ;
		ret.mX = FLT32 (thix.mVector[0]) ;
		ret.mY = FLT32 (thix.mVector[1]) ;
		return move (ret) ;
	}

	POINT3F xyz () const override {
		POINT3F ret ;
		ret.mX = FLT32 (thix.mVector[0]) ;
		ret.mY = FLT32 (thix.mVector[1]) ;
		ret.mZ = FLT32 (thix.mVector[2]) ;
		return move (ret) ;
	}

	VREF<A> at (CREF<INDEX> y) leftvalue override {
		return thix.mVector[y] ;
	}

	CREF<A> at (CREF<INDEX> y) const leftvalue override {
		return thix.mVector[y] ;
	}

	BOOL equal (CREF<VectorLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			if (thix.mVector[i] != that.mVector[i])
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<VectorLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = operator_compr (thix.mVector[i] ,that.mVector[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		operator_visit (visitor ,thix.mVector[0]) ;
		operator_visit (visitor ,thix.mVector[1]) ;
		operator_visit (visitor ,thix.mVector[2]) ;
		operator_visit (visitor ,thix.mVector[3]) ;
		visitor.end () ;
	}

	VectorLayout<A> add (CREF<VectorLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = thix.mVector[i] + that.mVector[i] ;
		}
		return move (ret) ;
	}

	void add_with (CREF<VectorLayout<A>> that) override {
		for (auto &&i : iter (0 ,4)) {
			thix.mVector[i] += that.mVector[i] ;
		}
	}

	VectorLayout<A> sub (CREF<VectorLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = thix.mVector[i] - that.mVector[i] ;
		}
		return move (ret) ;
	}

	void sub_with (CREF<VectorLayout<A>> that) override {
		for (auto &&i : iter (0 ,4)) {
			thix.mVector[i] -= that.mVector[i] ;
		}
	}

	VectorLayout<A> mul (CREF<A> scale) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = thix.mVector[i] * scale ;
		}
		return move (ret) ;
	}

	void mul_with (CREF<A> scale) override {
		for (auto &&i : iter (0 ,4)) {
			thix.mVector[i] *= scale ;
		}
	}

	A dot (CREF<VectorLayout<A>> that) const override {
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += thix.mVector[i] * that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout<A> mul (CREF<MatrixLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = A (0) ;
			rax += thix.mVector[0] * that.mMatrix[0 * 4 + i] ;
			rax += thix.mVector[1] * that.mMatrix[1 * 4 + i] ;
			rax += thix.mVector[2] * that.mMatrix[2 * 4 + i] ;
			rax += thix.mVector[3] * that.mMatrix[3 * 4 + i] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	VectorLayout<A> cross (CREF<VectorLayout<A>> that) const override {
		assume (thix.mVector[3] == 0) ;
		assume (that.mVector[3] == 0) ;
		VectorLayout<A> ret ;
		ret.mVector[0] = thix.mVector[1] * that.mVector[2] - thix.mVector[2] * that.mVector[1] ;
		ret.mVector[1] = thix.mVector[2] * that.mVector[0] - thix.mVector[0] * that.mVector[2] ;
		ret.mVector[2] = thix.mVector[0] * that.mVector[1] - thix.mVector[1] * that.mVector[0] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	void cross_with (CREF<VectorLayout<A>> that) override {
		thix = cross (that) ;
	}

	VectorLayout<A> minus () const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = -thix.mVector[i] ;
		}
		return move (ret) ;
	}

	A magnitude () const override {
		assume (thix.mVector[3] == 0) ;
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += MathProc::square (thix.mVector[i]) ;
		}
		return move (ret) ;
	}

	VectorLayout<A> normalize () const override {
		VectorLayout<A> ret ;
		const auto r1x = MathProc::inverse (magnitude ()) ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = thix.mVector[i] * r1x ;
		}
		return move (ret) ;
	}

	VectorLayout<A> projection () const override {
		VectorLayout<A> ret ;
		const auto r1x = MathProc::inverse (thix.mVector[3]) ;
		ret.mVector[0] = thix.mVector[0] * r1x ;
		ret.mVector[1] = thix.mVector[1] * r1x ;
		ret.mVector[2] = thix.mVector[2] * r1x ;
		ret.mVector[3] = 1 ;
		return move (ret) ;
	}

	VectorLayout<A> homogenize () const override {
		VectorLayout<A> ret ;
		ret.mVector[0] = thix.mVector[0] ;
		ret.mVector[1] = thix.mVector[1] ;
		ret.mVector[2] = thix.mVector[2] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}
} ;

exports VFat<VectorHolder<FLT32>> VectorHolder<FLT32>::create (VREF<VectorLayout<FLT32>> that) {
	return VFat<VectorHolder<FLT32>> (VectorImplHolder<FLT32> () ,that) ;
}

exports CFat<VectorHolder<FLT32>> VectorHolder<FLT32>::create (CREF<VectorLayout<FLT32>> that) {
	return CFat<VectorHolder<FLT32>> (VectorImplHolder<FLT32> () ,that) ;
}

exports VFat<VectorHolder<FLT64>> VectorHolder<FLT64>::create (VREF<VectorLayout<FLT64>> that) {
	return VFat<VectorHolder<FLT64>> (VectorImplHolder<FLT64> () ,that) ;
}

exports CFat<VectorHolder<FLT64>> VectorHolder<FLT64>::create (CREF<VectorLayout<FLT64>> that) {
	return CFat<VectorHolder<FLT64>> (VectorImplHolder<FLT64> () ,that) ;
}
} ;