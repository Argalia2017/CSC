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
		fake.mVector[0] = x ;
		fake.mVector[1] = y ;
		fake.mVector[2] = z ;
		fake.mVector[3] = w ;
	}

	void initialize (CREF<PIXEL> xy) override {
		fake.mVector[0] = A (xy.mX) + A (0.5) ;
		fake.mVector[1] = A (xy.mY) + A (0.5) ;
		fake.mVector[2] = 0 ;
		fake.mVector[3] = 1 ;
	}

	void initialize (CREF<POINT2F> xy) override {
		fake.mVector[0] = A (xy.mX) ;
		fake.mVector[1] = A (xy.mY) ;
		fake.mVector[2] = 0 ;
		fake.mVector[3] = 1 ;
	}

	void initialize (CREF<POINT3F> xyz) override {
		fake.mVector[0] = A (xyz.mX) ;
		fake.mVector[1] = A (xyz.mY) ;
		fake.mVector[2] = A (xyz.mZ) ;
		fake.mVector[3] = 1 ;
	}

	CREF<Pointer> zero () const override {
		return Pointer::from (memorize ([&] () {
			return Vector<A> (0 ,0 ,0 ,0) ;
		})) ;
	}

	CREF<Pointer> axis_x () const override {
		return Pointer::from (memorize ([&] () {
			return Vector<A> (1 ,0 ,0 ,0) ;
		})) ;
	}

	CREF<Pointer> axis_y () const override {
		return Pointer::from (memorize ([&] () {
			return Vector<A> (0 ,1 ,0 ,0) ;
		})) ;
	}

	CREF<Pointer> axis_z () const override {
		return Pointer::from (memorize ([&] () {
			return Vector<A> (0 ,0 ,1 ,0) ;
		})) ;
	}

	CREF<Pointer> axis_w () const override {
		return Pointer::from (memorize ([&] () {
			return Vector<A> (0 ,0 ,0 ,1) ;
		})) ;
	}

	POINT2F xy () const override {
		POINT2F ret ;
		ret.mX = FLT32 (fake.mVector[0]) ;
		ret.mY = FLT32 (fake.mVector[1]) ;
		return move (ret) ;
	}

	POINT3F xyz () const override {
		POINT3F ret ;
		ret.mX = FLT32 (fake.mVector[0]) ;
		ret.mY = FLT32 (fake.mVector[1]) ;
		ret.mZ = FLT32 (fake.mVector[2]) ;
		return move (ret) ;
	}

	VREF<A> at (CREF<INDEX> y) leftvalue override {
		return fake.mVector[y] ;
	}

	CREF<A> at (CREF<INDEX> y) const leftvalue override {
		return fake.mVector[y] ;
	}

	BOOL equal (CREF<VectorLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			if (fake.mVector[i] != that.mVector[i])
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<VectorLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = operator_compr (fake.mVector[i] ,that.mVector[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		for (auto &&i : iter (0 ,4)) {
			operator_visit (visitor ,fake.mVector[i]) ;
		}
		visitor.end () ;
	}

	VectorLayout<A> add (CREF<VectorLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] + that.mVector[i] ;
		}
		return move (ret) ;
	}

	void add_with (CREF<VectorLayout<A>> that) override {
		for (auto &&i : iter (0 ,4)) {
			fake.mVector[i] += that.mVector[i] ;
		}
	}

	VectorLayout<A> sub (CREF<VectorLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] - that.mVector[i] ;
		}
		return move (ret) ;
	}

	void sub_with (CREF<VectorLayout<A>> that) override {
		for (auto &&i : iter (0 ,4)) {
			fake.mVector[i] -= that.mVector[i] ;
		}
	}

	VectorLayout<A> mul (CREF<A> scale) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] * scale ;
		}
		return move (ret) ;
	}

	void mul_with (CREF<A> scale) override {
		for (auto &&i : iter (0 ,4)) {
			fake.mVector[i] *= scale ;
		}
	}

	A dot (CREF<VectorLayout<A>> that) const override {
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += fake.mVector[i] * that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout<A> mul (CREF<MatrixLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = A (0) ;
			rax += fake.mVector[0] * that.mMatrix[0 * 4 + i] ;
			rax += fake.mVector[1] * that.mMatrix[1 * 4 + i] ;
			rax += fake.mVector[2] * that.mMatrix[2 * 4 + i] ;
			rax += fake.mVector[3] * that.mMatrix[3 * 4 + i] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	VectorLayout<A> cross (CREF<VectorLayout<A>> that) const override {
		assume (fake.mVector[3] == 0) ;
		assume (that.mVector[3] == 0) ;
		VectorLayout<A> ret ;
		ret.mVector[0] = fake.mVector[1] * that.mVector[2] - fake.mVector[2] * that.mVector[1] ;
		ret.mVector[1] = fake.mVector[2] * that.mVector[0] - fake.mVector[0] * that.mVector[2] ;
		ret.mVector[2] = fake.mVector[0] * that.mVector[1] - fake.mVector[1] * that.mVector[0] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	void cross_with (CREF<VectorLayout<A>> that) override {
		fake = cross (that) ;
	}

	VectorLayout<A> minus () const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = -fake.mVector[i] ;
		}
		return move (ret) ;
	}

	A magnitude () const override {
		assume (fake.mVector[3] == 0) ;
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += MathProc::square (fake.mVector[i]) ;
		}
		return move (ret) ;
	}

	VectorLayout<A> normalize () const override {
		VectorLayout<A> ret ;
		const auto r1x = MathProc::inverse (magnitude ()) ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] * r1x ;
		}
		return move (ret) ;
	}

	VectorLayout<A> projection () const override {
		VectorLayout<A> ret ;
		const auto r1x = MathProc::inverse (fake.mVector[3]) ;
		ret.mVector[0] = fake.mVector[0] * r1x ;
		ret.mVector[1] = fake.mVector[1] * r1x ;
		ret.mVector[2] = fake.mVector[2] * r1x ;
		ret.mVector[3] = 1 ;
		return move (ret) ;
	}

	VectorLayout<A> homogenize () const override {
		VectorLayout<A> ret ;
		ret.mVector[0] = fake.mVector[0] ;
		ret.mVector[1] = fake.mVector[1] ;
		ret.mVector[2] = fake.mVector[2] ;
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

template <class A>
class MatrixImplHolder implement Fat<MatrixHolder<A> ,MatrixLayout<A>> {
public:
	void initialize (CREF<VectorLayout<A>> x ,CREF<VectorLayout<A>> y ,CREF<VectorLayout<A>> z ,CREF<VectorLayout<A>> w) override {
		for (auto &&i : iter (0 ,4)) {
			fake.mMatrix[i * 4 + 0] = x.mVector[i] ;
			fake.mMatrix[i * 4 + 1] = y.mVector[i] ;
			fake.mMatrix[i * 4 + 2] = z.mVector[i] ;
			fake.mMatrix[i * 4 + 3] = w.mVector[i] ;
		}
	}

	CREF<Pointer> zero () const override {
		return Pointer::from (memorize ([&] () {
			const auto r1x = Vector<A>::zero () ;
			return Matrix<A> (r1x ,r1x ,r1x ,r1x) ;
		})) ;
	}

	CREF<Pointer> identity () const override {
		return Pointer::from (memorize ([&] () {
			const auto r1x = Vector<A>::axis_x () ;
			const auto r2x = Vector<A>::axis_y () ;
			const auto r3x = Vector<A>::axis_z () ;
			const auto r4x = Vector<A>::axis_w () ;
			return Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
		})) ;
	}

	VREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (operator_between (x ,0 ,4)) ;
		assert (operator_between (y ,0 ,4)) ;
		return fake.mMatrix[y * 4 + x] ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (operator_between (x ,0 ,4)) ;
		assert (operator_between (y ,0 ,4)) ;
		return fake.mMatrix[y * 4 + x] ;
	}

	BOOL equal (CREF<MatrixLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			if (fake.mMatrix[i] != that.mMatrix[i])
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<MatrixLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = operator_compr (fake.mMatrix[i] ,that.mMatrix[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		for (auto &&i : iter (0 ,16)) {
			operator_visit (visitor ,fake.mMatrix[i]) ;
		}
		visitor.end () ;
	}

	MatrixLayout<A> add (CREF<MatrixLayout<A>> that) const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] + that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	void add_with (CREF<MatrixLayout<A>> that) override {
		for (auto &&i : iter (0 ,16)) {
			fake.mMatrix[i] += that.mMatrix[i] ;
		}
	}

	MatrixLayout<A> sub (CREF<MatrixLayout<A>> that) const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] - that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	void sub_with (CREF<MatrixLayout<A>> that) override {
		for (auto &&i : iter (0 ,16)) {
			fake.mMatrix[i] -= that.mMatrix[i] ;
		}
	}

	MatrixLayout<A> mul (CREF<A> scale) const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] * scale ;
		}
		return move (ret) ;
	}

	void mul_with (CREF<A> scale) override {
		for (auto &&i : iter (0 ,16)) {
			fake.mMatrix[i] *= scale ;
		}
	}

	VectorLayout<A> mul (CREF<VectorLayout<A>> that) const override {
		VectorLayout<A> ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = A (0) ;
			rax += fake.mMatrix[i * 4 + 0] * ret.mVector[0] ;
			rax += fake.mMatrix[i * 4 + 1] * ret.mVector[1] ;
			rax += fake.mMatrix[i * 4 + 2] * ret.mVector[2] ;
			rax += fake.mMatrix[i * 4 + 3] * ret.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> mul (CREF<MatrixLayout<A>> that) const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			auto rax = A (0) ;
			rax += fake.mMatrix[i.mY * 4 + 0] * that.mMatrix[0 * 4 + i.mX] ;
			rax += fake.mMatrix[i.mY * 4 + 1] * that.mMatrix[1 * 4 + i.mX] ;
			rax += fake.mMatrix[i.mY * 4 + 2] * that.mMatrix[2 * 4 + i.mX] ;
			rax += fake.mMatrix[i.mY * 4 + 3] * that.mMatrix[3 * 4 + i.mX] ;
			ret.mMatrix[i.mY * 4 + i.mX] = rax ;
		}
		return move (ret) ;
	}

	void mul_with (CREF<MatrixLayout<A>> that) override {
		fake = mul (that) ;
	}

	MatrixLayout<A> minus () const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = -fake.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> transpose () const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = i.mY * 4 + i.mX ;
			INDEX iy = i.mX * 4 + i.mY ;
			ret.mMatrix[ix] = fake.mMatrix[iy] ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> triangular () const override {
		MatrixLayout<A> ret ;
		unimplemented () ;
		return move (ret) ;
	}

	MatrixLayout<A> homogensize () const override {
		MatrixLayout<A> ret = zero () ;
		for (auto &&i : iter (0 ,3 ,0 ,3)) {
			INDEX ix = i.mY * 4 + i.mX ;
			ret.mMatrix[ix] = fake.mMatrix[ix] ;
		}
		return move (ret) ;
	}

	A determinant () const override {
		A ret = 0 ;
		unimplemented () ;
		return move (ret) ;
	}

	MatrixLayout<A> inverse () const override {
		MatrixLayout<A> ret ;
		unimplemented () ;
		return move (ret) ;
	}

	A trace () const override {
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += fake.mMatrix[i * 4 + i] ;
		}
		return move (ret) ;
	}

	RREF<Pointer> decompose () const override {
		unimplemented () ;
		return move (Pointer::make (0)) ;
	}

	RREF<Pointer> singular () const override {
		unimplemented () ;
		return move (Pointer::make (0)) ;
	}

	void DiagMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) override {
		fake = zero () ;
		MatrixHolder<A>::create (fake)->at (0 ,0) = x ;
		MatrixHolder<A>::create (fake)->at (1 ,1) = y ;
		MatrixHolder<A>::create (fake)->at (2 ,2) = z ;
		MatrixHolder<A>::create (fake)->at (3 ,3) = w ;
	}

	void ShearMatrix_initialize (CREF<VectorLayout<A>> x ,CREF<VectorLayout<A>> y ,CREF<VectorLayout<A>> z) override {
		unimplemented () ;
	}

	void RotationMatrix_initialize (CREF<VectorLayout<A>> normal ,CREF<A> angle) override {
		unimplemented () ;
	}

	void TranslationMatrix_initialize (CREF<VectorLayout<A>> xyz) override {
		TranslationMatrix_initialize (xyz.mVector[0] ,xyz.mVector[1] ,xyz.mVector[2]) ;
	}

	void TranslationMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z) override {
		fake = identity () ;
		MatrixHolder<A>::create (fake)->at (3 ,0) = x ;
		MatrixHolder<A>::create (fake)->at (3 ,1) = y ;
		MatrixHolder<A>::create (fake)->at (3 ,2) = z ;
		MatrixHolder<A>::create (fake)->at (3 ,3) = 1 ;
	}

	void PerspectiveMatrix_initialize (CREF<A> fx ,CREF<A> fy ,CREF<A> wx ,CREF<A> wy) override {
		unimplemented () ;
	}

	void ProjectionMatrix_initialize (CREF<VectorLayout<A>> normal ,CREF<VectorLayout<A>> center ,CREF<VectorLayout<A>> light) override {
		unimplemented () ;
	}

	void ViewMatrix_initialize (CREF<VectorLayout<A>> vx ,CREF<VectorLayout<A>> vy) override {
		unimplemented () ;
	}

	void ViewMatrix_initialize (CREF<VectorLayout<A>> vx ,CREF<VectorLayout<A>> vy ,CREF<FLAG> flag) override {
		unimplemented () ;
	}
} ;

exports VFat<MatrixHolder<FLT32>> MatrixHolder<FLT32>::create (VREF<MatrixLayout<FLT32>> that) {
	return VFat<MatrixHolder<FLT32>> (MatrixImplHolder<FLT32> () ,that) ;
}

exports CFat<MatrixHolder<FLT32>> MatrixHolder<FLT32>::create (CREF<MatrixLayout<FLT32>> that) {
	return CFat<MatrixHolder<FLT32>> (MatrixImplHolder<FLT32> () ,that) ;
}

exports VFat<MatrixHolder<FLT64>> MatrixHolder<FLT64>::create (VREF<MatrixLayout<FLT64>> that) {
	return VFat<MatrixHolder<FLT64>> (MatrixImplHolder<FLT64> () ,that) ;
}

exports CFat<MatrixHolder<FLT64>> MatrixHolder<FLT64>::create (CREF<MatrixLayout<FLT64>> that) {
	return CFat<MatrixHolder<FLT64>> (MatrixImplHolder<FLT64> () ,that) ;
}
} ;