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
			rax += fake.mVector[0] * that.mMatrix[i + 0 * 4] ;
			rax += fake.mVector[1] * that.mMatrix[i + 1 * 4] ;
			rax += fake.mVector[2] * that.mMatrix[i + 2 * 4] ;
			rax += fake.mVector[3] * that.mMatrix[i + 3 * 4] ;
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
			load (fake ,0 ,i) = x.mVector[i] ;
			load (fake ,1 ,i) = y.mVector[i] ;
			load (fake ,2 ,i) = z.mVector[i] ;
			load (fake ,3 ,i) = w.mVector[i] ;
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
		return load (fake ,x ,y) ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (operator_between (x ,0 ,4)) ;
		assert (operator_between (y ,0 ,4)) ;
		return load (fake ,x ,y) ;
	}

	VREF<A> load (VREF<MatrixLayout<A>> matrix ,CREF<INDEX> x ,CREF<INDEX> y) const {
		return matrix.mMatrix[x + y * 4] ;
	}

	CREF<A> load (CREF<MatrixLayout<A>> matrix ,CREF<INDEX> x ,CREF<INDEX> y) const {
		return matrix.mMatrix[x + y * 4] ;
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
			rax += load (fake ,0 ,i) * ret.mVector[0] ;
			rax += load (fake ,1 ,i) * ret.mVector[1] ;
			rax += load (fake ,2 ,i) * ret.mVector[2] ;
			rax += load (fake ,3 ,i) * ret.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> mul (CREF<MatrixLayout<A>> that) const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			auto rax = A (0) ;
			rax += load (fake ,0 ,i.mY) * load (that ,i.mX ,0) ;
			rax += load (fake ,1 ,i.mY) * load (that ,i.mX ,1) ;
			rax += load (fake ,2 ,i.mY) * load (that ,i.mX ,2) ;
			rax += load (fake ,3 ,i.mY) * load (that ,i.mX ,3) ;
			load (ret ,i.mX ,i.mY) = rax ;
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
			load (ret ,i.mY ,i.mX) = load (fake ,i.mX ,i.mY) ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> triangular () const override {
		MatrixLayout<A> ret = fake ;
		for (auto &&i : iter (0 ,4)) {
			if ifdo (TRUE) {
				INDEX ix = find_abs_max_row (i) ;
				if (ix == i)
					discard ;
				for (auto &&j : iter (i ,4)) {
					const auto r1x = -load (ret ,j ,i) ;
					load (ret ,j ,i) = load (ret ,j ,ix) ;
					load (ret ,j ,ix) = r1x ;
				}
			}
			const auto r2x = MathProc::inverse (load (ret ,i ,i)) ;
			if (r2x == 0)
				continue ;
			for (auto &&j : iter (i + 1 ,4)) {
				const auto r3x = load (ret ,i ,j) * r2x ;
				for (auto &&k : iter (i + 1 ,4))
					load (ret ,k ,j) -= r3x * load (ret ,k ,i) ;
				load (ret ,i ,j) = 0 ;
			}
		}
		return move (ret) ;
	}

	INDEX find_abs_max_row (CREF<INDEX> x) const {
		INDEX ret = NONE ;
		auto rax = A () ;
		for (auto &&i : iter (x ,4)) {
			const auto r1x = MathProc::abs (load (fake ,x ,i)) ;
			if (ret != NONE)
				if (rax >= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> homogensize () const override {
		MatrixLayout<A> ret = zero () ;
		for (auto &&i : iter (0 ,3 ,0 ,3)) {
			load (ret ,i.mX ,i.mY) = load (fake ,i.mX ,i.mY) ;
		}
		return move (ret) ;
	}

	A determinant () const override {
		const auto r1x = triangular () ;
		A ret = A (1) ;
		for (auto &&i : iter (0 ,4))
			ret *= load (r1x ,i ,i) ;
		return move (ret) ;
	}

	MatrixLayout<A> inverse () const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = LENGTH (i.mY == 0) ;
			INDEX iy = ix + 1 + LENGTH (i.mY == 1) ;
			INDEX iz = iy + 1 + LENGTH (i.mY == 2) ;
			INDEX jx = LENGTH (i.mX == 0) ;
			INDEX jy = jx + 1 + LENGTH (i.mX == 1) ;
			INDEX jz = jy + 1 + LENGTH (i.mX == 2) ;
			const auto r1x = load (fake ,jx ,ix) * (load (fake ,jy ,iy) * load (fake ,jz ,iz) - load (fake ,jz ,iy) * load (fake ,jy ,iz)) ;
			const auto r2x = load (fake ,jx ,iy) * (load (fake ,jy ,ix) * load (fake ,jz ,iz) - load (fake ,jz ,ix) * load (fake ,jy ,iz)) ;
			const auto r3x = load (fake ,jx ,iz) * (load (fake ,jy ,ix) * load (fake ,jz ,iy) - load (fake ,jz ,ix) * load (fake ,jy ,iy)) ;
			const auto r4x = r1x - r2x + r3x ;
			const auto r5x = invoke ([&] () {
				if ((i.mY + i.mX) % 2 == 0)
					return r4x ;
				return -r4x ;
			}) ;
			load (ret ,i.mY ,i.mX) = r5x ;
		}
		const auto r6x = MathProc::inverse (determinant ()) ;
		MatrixHolder<A>::create (ret)->mul_with (r6x) ;
		return move (ret) ;
	}

	A trace () const override {
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += load (fake ,i ,i) ;
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

template <class A>
class MakeMatrixImplHolder implement Fat<MakeMatrixHolder<A> ,MatrixLayout<A>> {
public:
	void DiagMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		ret[0][0] = x ;
		ret[1][1] = y ;
		ret[2][2] = z ;
		ret[1][3] = w ;
		fake = move (ret) ;
	}

	void ShearMatrix_initialize (CREF<Vector<A>> x ,CREF<Vector<A>> y ,CREF<Vector<A>> z) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = z.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = r2x * r3x ;
		const auto r7x = MathProc::sqrt (A (1) - MathProc::square (r4x)) ;
		const auto r8x = (r6x - r4x * r5x) * MathProc::inverse (r7x) ;
		const auto r9x = MathProc::sqrt (A (1) - MathProc::square (r5x) - MathProc::square (r8x)) ;
		ret[0][0] = 1 ;
		ret[0][1] = r4x ;
		ret[1][1] = r7x ;
		ret[0][2] = r5x ;
		ret[1][2] = r8x ;
		ret[2][3] = r9x ;
		ret[3][3] = 1 ;
		fake = move (ret) ;
	}

	void RotationMatrix_initialize (CREF<Vector<A>> normal ,CREF<A> angle) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		const auto r1x = normal.normalize () ;
		const auto r2x = MathProc::cos (angle) ;
		const auto r3x = r1x * MathProc::sin (angle) ;
		const auto r4x = r1x * (1 - r2x) ;
		ret[0][0] = r1x[0] * r4x[0] + r2x ;
		ret[1][0] = r1x[1] * r4x[0] + r3x[2] ;
		ret[2][0] = r1x[2] * r4x[0] - r3x[1] ;
		ret[0][1] = r1x[0] * r4x[1] - r3x[2] ;
		ret[1][1] = r1x[1] * r4x[1] + r2x ;
		ret[2][1] = r1x[2] * r4x[1] + r3x[0] ;
		ret[0][2] = r1x[0] * r4x[2] + r3x[1] ;
		ret[1][2] = r1x[1] * r4x[2] - r3x[0] ;
		ret[2][2] = r1x[2] * r4x[2] + r2x ;
		ret[3][3] = 1 ;
		fake = move (ret) ;
	}

	void TranslationMatrix_initialize (CREF<Vector<A>> xyz) override {
		TranslationMatrix_initialize (xyz[0] ,xyz[1] ,xyz[2]) ;
	}

	void TranslationMatrix_initialize (CREF<A> x ,CREF<A> y ,CREF<A> z) override {
		Matrix<A> ret = Matrix<A>::identity () ;
		ret[0][3] = x ;
		ret[1][3] = y ;
		ret[2][3] = z ;
		fake = move (ret) ;
	}

	void PerspectiveMatrix_initialize (CREF<A> fx ,CREF<A> fy ,CREF<A> wx ,CREF<A> wy) override {
		assert (fx > 0) ;
		assert (fy > 0) ;
		Matrix<A> ret = Matrix<A>::zero () ;
		ret[0][0] = fx ;
		ret[1][1] = fy ;
		ret[0][2] = wx ;
		ret[1][2] = wx ;
		ret[3][2] = 1 ;
		ret[3][3] = 1 ;
		fake = move (ret) ;
	}

	void ProjectionMatrix_initialize (CREF<Vector<A>> normal ,CREF<Vector<A>> center ,CREF<Vector<A>> light) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		const auto r1x = normal.normalize () ;
		const auto r2x = center * r1x ;
		const auto r3x = light.normalize () ;
		const auto r4x = r1x * r3x ;
		ret[0][0] = r4x - r1x[0] * r3x[0] ;
		ret[1][0] = -r1x[0] * r3x[1] ;
		ret[2][0] = -r1x[0] * r3x[2] ;
		ret[0][1] = -r1x[1] * r3x[0] ;
		ret[1][1] = r4x - r1x[1] * r3x[1] ;
		ret[2][1] = -r1x[1] * r3x[2] ;
		ret[0][2] = -r1x[2] * r3x[0] ;
		ret[1][2] = -r1x[2] * r3x[1] ;
		ret[2][2] = r4x - r1x[2] * r3x[2] ;
		ret[0][3] = r2x * r3x[0] ;
		ret[1][3] = r2x * r3x[1] ;
		ret[2][3] = r2x * r3x[2] ;
		ret[3][3] = r4x ;
		fake = move (ret) ;
	}

	void ViewMatrix_initialize (CREF<Vector<A>> x ,CREF<Vector<A>> y) override {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		const auto r5x = Vector<A>::axis_w () ;
		fake = Matrix<A> (r1x ,r4x ,r3x ,r5x) ;
	}

	void ViewMatrix_initialize (CREF<Vector<A>> x ,CREF<Vector<A>> y ,CREF<FLAG> flag) override {
		//@mark
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::XY)
				discard ;
			const auto r1x = Vector<A>::axis_x () ;
			const auto r2x = Vector<A>::axis_y () ;
			const auto r3x = Vector<A>::axis_z () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::XZ)
				discard ;
			const auto r1x = Vector<A>::axis_x () ;
			const auto r2x = -Vector<A>::axis_z () ;
			const auto r3x = Vector<A>::axis_y () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::YX)
				discard ;
			const auto r1x = Vector<A>::axis_y () ;
			const auto r2x = Vector<A>::axis_x () ;
			const auto r3x = -Vector<A>::axis_z () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::YZ)
				discard ;
			const auto r1x = Vector<A>::axis_z () ;
			const auto r2x = Vector<A>::axis_x () ;
			const auto r3x = Vector<A>::axis_y () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::ZX)
				discard ;
			const auto r1x = Vector<A>::axis_y () ;
			const auto r2x = Vector<A>::axis_z () ;
			const auto r3x = Vector<A>::axis_x () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::ZY)
				discard ;
			const auto r1x = -Vector<A>::axis_z () ;
			const auto r2x = Vector<A>::axis_y () ;
			const auto r3x = Vector<A>::axis_x () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			return ;
		}
	}

	void CrossProductMatrix_initialize (CREF<Vector<A>> xyz) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		const auto r1x = xyz ;
		assert (r1x[3] == 0) ;
		ret[1][0] = r1x[2] ;
		ret[2][0] = -r1x[1] ;
		ret[0][1] = -r1x[2] ;
		ret[2][1] = r1x[0] ;
		ret[0][2] = r1x[1] ;
		ret[1][2] = -r1x[0] ;
		fake = move (ret) ;
	}

	void SymmetryMatrix_initialize (CREF<Vector<A>> x ,CREF<Vector<A>> y) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		const auto r1x = x ;
		const auto r2x = y ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret[i] = r1x[i.mY] * r2x[i.mX] ;
		}
		fake = move (ret) ;
	}
} ;

exports VFat<MakeMatrixHolder<FLT32>> MakeMatrixHolder<FLT32>::create (VREF<MatrixLayout<FLT32>> that) {
	return VFat<MakeMatrixHolder<FLT32>> (MakeMatrixImplHolder<FLT32> () ,that) ;
}

exports CFat<MakeMatrixHolder<FLT32>> MakeMatrixHolder<FLT32>::create (CREF<MatrixLayout<FLT32>> that) {
	return CFat<MakeMatrixHolder<FLT32>> (MakeMatrixImplHolder<FLT32> () ,that) ;
}

exports VFat<MakeMatrixHolder<FLT64>> MakeMatrixHolder<FLT64>::create (VREF<MatrixLayout<FLT64>> that) {
	return VFat<MakeMatrixHolder<FLT64>> (MakeMatrixImplHolder<FLT64> () ,that) ;
}

exports CFat<MakeMatrixHolder<FLT64>> MakeMatrixHolder<FLT64>::create (CREF<MatrixLayout<FLT64>> that) {
	return CFat<MakeMatrixHolder<FLT64>> (MakeMatrixImplHolder<FLT64> () ,that) ;
}
} ;