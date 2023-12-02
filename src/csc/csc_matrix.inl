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
	void initialize (RREF<VectorLayout<A>> that) override {
		fake = move (that) ;
	}

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
		assert (fake.mVector[3] == 0) ;
		assert (that.mVector[3] == 0) ;
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
		assert (fake.mVector[3] == 0) ;
		A ret = 0 ;
		ret += Math::square (fake.mVector[0]) ;
		ret += Math::square (fake.mVector[1]) ;
		ret += Math::square (fake.mVector[2]) ;
		ret = Math::sqrt (ret) ;
		return move (ret) ;
	}

	VectorLayout<A> normalize () const override {
		VectorLayout<A> ret ;
		const auto r1x = Math::inverse (magnitude ()) ;
		ret.mVector[0] = fake.mVector[0] * r1x ;
		ret.mVector[1] = fake.mVector[1] * r1x ;
		ret.mVector[2] = fake.mVector[2] * r1x ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout<A> projection () const override {
		VectorLayout<A> ret ;
		const auto r1x = Math::inverse (fake.mVector[3]) ;
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

template <class A>
exports VFat<VectorHolder<A>> VectorHolder<A>::create (VREF<VectorLayout<A>> that) {
	return VFat<VectorHolder<A>> (VectorImplHolder<A> () ,that) ;
}

template <class A>
exports CFat<VectorHolder<A>> VectorHolder<A>::create (CREF<VectorLayout<A>> that) {
	return CFat<VectorHolder<A>> (VectorImplHolder<A> () ,that) ;
}

template struct VectorHolder<FLT32> ;
template struct VectorHolder<FLT64> ;

template <class A>
class MatrixImplHolder implement Fat<MatrixHolder<A> ,MatrixLayout<A>> {
public:
	void initialize (RREF<MatrixLayout<A>> that) override {
		fake = move (that) ;
	}

	void initialize (CREF<VectorLayout<A>> x ,CREF<VectorLayout<A>> y ,CREF<VectorLayout<A>> z ,CREF<VectorLayout<A>> w) override {
		for (auto &&i : iter (0 ,4)) {
			at (fake ,0 ,i) = x.mVector[i] ;
			at (fake ,1 ,i) = y.mVector[i] ;
			at (fake ,2 ,i) = z.mVector[i] ;
			at (fake ,3 ,i) = w.mVector[i] ;
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
		return at (fake ,x ,y) ;
	}

	CREF<A> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (operator_between (x ,0 ,4)) ;
		assert (operator_between (y ,0 ,4)) ;
		return at (fake ,x ,y) ;
	}

	imports VREF<A> at (VREF<MatrixLayout<A>> layout ,CREF<INDEX> x ,CREF<INDEX> y) {
		return layout.mMatrix[x + y * 4] ;
	}

	imports CREF<A> at (CREF<MatrixLayout<A>> layout ,CREF<INDEX> x ,CREF<INDEX> y) {
		return layout.mMatrix[x + y * 4] ;
	}

	BOOL equal (CREF<MatrixLayout<A>> that) const override {
		for (auto &&i : iter (0 ,16)) {
			if (fake.mMatrix[i] != that.mMatrix[i])
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<MatrixLayout<A>> that) const override {
		for (auto &&i : iter (0 ,16)) {
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
			rax += at (fake ,0 ,i) * ret.mVector[0] ;
			rax += at (fake ,1 ,i) * ret.mVector[1] ;
			rax += at (fake ,2 ,i) * ret.mVector[2] ;
			rax += at (fake ,3 ,i) * ret.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> mul (CREF<MatrixLayout<A>> that) const override {
		MatrixLayout<A> ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			auto rax = A (0) ;
			rax += at (fake ,0 ,i.mY) * at (that ,i.mX ,0) ;
			rax += at (fake ,1 ,i.mY) * at (that ,i.mX ,1) ;
			rax += at (fake ,2 ,i.mY) * at (that ,i.mX ,2) ;
			rax += at (fake ,3 ,i.mY) * at (that ,i.mX ,3) ;
			at (ret ,i.mX ,i.mY) = rax ;
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
			at (ret ,i.mY ,i.mX) = at (fake ,i.mX ,i.mY) ;
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
					const auto r1x = -at (ret ,j ,i) ;
					at (ret ,j ,i) = at (ret ,j ,ix) ;
					at (ret ,j ,ix) = r1x ;
				}
			}
			const auto r2x = Math::inverse (at (ret ,i ,i)) ;
			if (r2x == 0)
				continue ;
			for (auto &&j : iter (i + 1 ,4)) {
				const auto r3x = at (ret ,i ,j) * r2x ;
				for (auto &&k : iter (i + 1 ,4))
					at (ret ,k ,j) -= r3x * at (ret ,k ,i) ;
				at (ret ,i ,j) = 0 ;
			}
		}
		return move (ret) ;
	}

	INDEX find_abs_max_row (CREF<INDEX> x) const {
		INDEX ret = NONE ;
		auto rax = A () ;
		for (auto &&i : iter (x ,4)) {
			const auto r1x = Math::abs (at (fake ,x ,i)) ;
			if (ret != NONE)
				if (rax >= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return move (ret) ;
	}

	MatrixLayout<A> homogenize () const override {
		MatrixLayout<A> ret = zero () ;
		for (auto &&i : iter (0 ,3 ,0 ,3)) {
			at (ret ,i.mX ,i.mY) = at (fake ,i.mX ,i.mY) ;
		}
		return move (ret) ;
	}

	A determinant () const override {
		const auto r1x = triangular () ;
		A ret = 1 ;
		for (auto &&i : iter (0 ,4))
			ret *= at (r1x ,i ,i) ;
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
			const auto r1x = at (fake ,jx ,ix) * (at (fake ,jy ,iy) * at (fake ,jz ,iz) - at (fake ,jz ,iy) * at (fake ,jy ,iz)) ;
			const auto r2x = at (fake ,jx ,iy) * (at (fake ,jy ,ix) * at (fake ,jz ,iz) - at (fake ,jz ,ix) * at (fake ,jy ,iz)) ;
			const auto r3x = at (fake ,jx ,iz) * (at (fake ,jy ,ix) * at (fake ,jz ,iy) - at (fake ,jz ,ix) * at (fake ,jy ,iy)) ;
			const auto r4x = r1x - r2x + r3x ;
			const auto r5x = invoke ([&] () {
				if ((i.mY + i.mX) % 2 == 0)
					return r4x ;
				return -r4x ;
			}) ;
			at (ret ,i.mY ,i.mX) = r5x ;
		}
		const auto r6x = Math::inverse (determinant ()) ;
		MatrixHolder<A>::create (ret)->mul_with (r6x) ;
		return move (ret) ;
	}

	A trace () const override {
		A ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += at (fake ,i ,i) ;
		}
		return move (ret) ;
	}
} ;

template <class A>
exports VFat<MatrixHolder<A>> MatrixHolder<A>::create (VREF<MatrixLayout<A>> that) {
	return VFat<MatrixHolder<A>> (MatrixImplHolder<A> () ,that) ;
}

template <class A>
exports CFat<MatrixHolder<A>> MatrixHolder<A>::create (CREF<MatrixLayout<A>> that) {
	return CFat<MatrixHolder<A>> (MatrixImplHolder<A> () ,that) ;
}

template struct MatrixHolder<FLT32> ;
template struct MatrixHolder<FLT64> ;

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
		const auto r7x = Math::sqrt (1 - Math::square (r4x)) ;
		const auto r8x = (r6x - r4x * r5x) * Math::inverse (r7x) ;
		const auto r9x = Math::sqrt (1 - Math::square (r5x) - Math::square (r8x)) ;
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
		const auto r2x = Math::cos (angle) ;
		const auto r3x = r1x * Math::sin (angle) ;
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
		ViewMatrix_initialize (x ,y) ;
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::XY)
				discard ;
			const auto r1x = Vector<A>::axis_x () ;
			const auto r2x = Vector<A>::axis_y () ;
			const auto r3x = Vector<A>::axis_z () ;
			const auto r4x = Vector<A>::axis_w () ;
			const auto r5x = Matrix<A> (r1x ,r2x ,r3x ,r4x) ;
			MatrixHolder<A>::create (fake)->mul_with (r5x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::XZ)
				discard ;
			const auto r6x = Vector<A>::axis_x () ;
			const auto r7x = -Vector<A>::axis_z () ;
			const auto r8x = Vector<A>::axis_y () ;
			const auto r9x = Vector<A>::axis_w () ;
			const auto r10x = Matrix<A> (r6x ,r7x ,r8x ,r9x) ;
			MatrixHolder<A>::create (fake)->mul_with (r10x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::YX)
				discard ;
			const auto r11x = Vector<A>::axis_y () ;
			const auto r12x = Vector<A>::axis_x () ;
			const auto r13x = -Vector<A>::axis_z () ;
			const auto r14x = Vector<A>::axis_w () ;
			const auto r15x = Matrix<A> (r11x ,r12x ,r13x ,r14x) ;
			MatrixHolder<A>::create (fake)->mul_with (r15x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::YZ)
				discard ;
			const auto r16x = Vector<A>::axis_z () ;
			const auto r17x = Vector<A>::axis_x () ;
			const auto r18x = Vector<A>::axis_y () ;
			const auto r19x = Vector<A>::axis_w () ;
			const auto r20x = Matrix<A> (r16x ,r17x ,r18x ,r19x) ;
			MatrixHolder<A>::create (fake)->mul_with (r20x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::ZX)
				discard ;
			const auto r21x = Vector<A>::axis_y () ;
			const auto r22x = Vector<A>::axis_z () ;
			const auto r23x = Vector<A>::axis_x () ;
			const auto r24x = Vector<A>::axis_w () ;
			const auto r25x = Matrix<A> (r21x ,r22x ,r23x ,r24x) ;
			MatrixHolder<A>::create (fake)->mul_with (r25x) ;
			return ;
		}
		if ifdo (TRUE) {
			if (flag != ViewMatrixFlag::ZY)
				discard ;
			const auto r26x = -Vector<A>::axis_z () ;
			const auto r27x = Vector<A>::axis_y () ;
			const auto r28x = Vector<A>::axis_x () ;
			const auto r29x = Vector<A>::axis_w () ;
			const auto r30x = Matrix<A> (r26x ,r27x ,r28x ,r29x) ;
			MatrixHolder<A>::create (fake)->mul_with (r30x) ;
			return ;
		}
	}

	void CrossProductMatrix_initialize (CREF<Vector<A>> xyz) override {
		Matrix<A> ret = Matrix<A>::zero () ;
		const auto r1x = xyz.homogenize () ;
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

template <class A>
exports VFat<MakeMatrixHolder<A>> MakeMatrixHolder<A>::create (VREF<MatrixLayout<A>> that) {
	return VFat<MakeMatrixHolder<A>> (MakeMatrixImplHolder<A> () ,that) ;
}

template <class A>
exports CFat<MakeMatrixHolder<A>> MakeMatrixHolder<A>::create (CREF<MatrixLayout<A>> that) {
	return CFat<MakeMatrixHolder<A>> (MakeMatrixImplHolder<A> () ,that) ;
}

template struct MakeMatrixHolder<FLT32> ;
template struct MakeMatrixHolder<FLT64> ;

template <class A>
class QuaternionImplHolder implement Fat<QuaternionHolder<A> ,QuaternionLayout<A>> {
public:
	void initialize (RREF<QuaternionLayout<A>> that) override {
		fake = move (that) ;
	}

	void initialize (CREF<A> x ,CREF<A> y ,CREF<A> z ,CREF<A> w) override {
		fake.mQuaternion[0] = x ;
		fake.mQuaternion[1] = y ;
		fake.mQuaternion[2] = z ;
		fake.mQuaternion[3] = w ;
	}

	void initialize (CREF<MatrixLayout<A>> that) override {
		const auto r1x = Matrix<A> (move (that)).homogenize () ;
		if ifdo (TRUE) {
			const auto r2x = 1 + r1x.trace () ;
			if (r2x < 1)
				discard ;
			const auto r3x = Math::sqrt (r2x) ;
			const auto r4x = Math::inverse (r3x) ;
			const auto r5x = (r1x[2][1] - r1x[1][2]) * r4x ;
			const auto r6x = (r1x[0][2] - r1x[2][0]) * r4x ;
			const auto r7x = (r1x[1][0] - r1x[0][1]) * r4x ;
			fake.mQuaternion[0] = r5x / 2 ;
			fake.mQuaternion[1] = r6x / 2 ;
			fake.mQuaternion[2] = r7x / 2 ;
			fake.mQuaternion[3] = r3x / 2 ;
			return ;
		}
		if ifdo (TRUE) {
			const auto r8x = 1 + r1x[0][0] - r1x[1][1] - r1x[2][2] ;
			if (r8x < 1)
				discard ;
			const auto r9x = Math::sqrt (r8x) ;
			const auto r10x = Math::inverse (r9x) ;
			const auto r11x = (r1x[1][0] + r1x[0][1]) * r10x ;
			const auto r12x = (r1x[0][2] + r1x[2][0]) * r10x ;
			const auto r13x = (r1x[2][1] - r1x[1][2]) * r10x ;
			fake.mQuaternion[0] = r9x / 2 ;
			fake.mQuaternion[1] = r11x / 2 ;
			fake.mQuaternion[2] = r12x / 2 ;
			fake.mQuaternion[3] = r13x / 2 ;
			return ;
		}
		if ifdo (TRUE) {
			const auto r14x = 1 - r1x[0][0] + r1x[1][1] - r1x[2][2] ;
			if (r14x < 1)
				discard ;
			const auto r15x = Math::sqrt (r14x) ;
			const auto r16x = Math::inverse (r15x) ;
			const auto r17x = (r1x[1][0] + r1x[0][1]) * r16x ;
			const auto r18x = (r1x[2][1] + r1x[1][2]) * r16x ;
			const auto r19x = (r1x[0][2] - r1x[2][0]) * r16x ;
			fake.mQuaternion[0] = r17x / 2 ;
			fake.mQuaternion[1] = r15x / 2 ;
			fake.mQuaternion[2] = r18x / 2 ;
			fake.mQuaternion[3] = r19x / 2 ;
			return ;
		}
		if ifdo (TRUE) {
			const auto r20x = 1 - r1x[0][0] - r1x[1][1] + r1x[2][2] ;
			if (r20x < 1)
				discard ;
			const auto r21x = Math::sqrt (r20x) ;
			const auto r22x = Math::inverse (r21x) ;
			const auto r23x = (r1x[0][2] + r1x[2][0]) * r22x ;
			const auto r24x = (r1x[2][1] + r1x[1][2]) * r22x ;
			const auto r25x = (r1x[1][0] - r1x[0][1]) * r22x ;
			fake.mQuaternion[0] = r23x / 2 ;
			fake.mQuaternion[1] = r24x / 2 ;
			fake.mQuaternion[2] = r21x / 2 ;
			fake.mQuaternion[3] = r25x / 2 ;
			return ;
		}
	}

	CREF<A> at (CREF<INDEX> x) const leftvalue override {
		return fake.mQuaternion[x] ;
	}

	BOOL equal (CREF<QuaternionLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			if (fake.mQuaternion[i] != that.mQuaternion[i])
				return FALSE ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<QuaternionLayout<A>> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = operator_compr (fake.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		for (auto &&i : iter (0 ,4)) {
			operator_visit (visitor ,fake.mQuaternion[i]) ;
		}
		visitor.end () ;
	}

	QuaternionLayout<A> mul (CREF<QuaternionLayout<A>> that) const {
		QuaternionLayout<A> ret ;
		const auto r1x = real (fake) ;
		const auto r2x = real (that) ;
		const auto r3x = fake.mQuaternion[3] ;
		const auto r4x = that.mQuaternion[3] ;
		const auto r5x = r3x * r2x + r4x * r1x + (r1x ^ r2x) ;
		const auto r6x = r3x * r4x - r1x * r2x ;
		ret.mQuaternion[0] = r5x[0] ;
		ret.mQuaternion[1] = r5x[1] ;
		ret.mQuaternion[2] = r5x[2] ;
		ret.mQuaternion[3] = r6x ;
		return move (ret) ;
	}

	void mul_with (CREF<QuaternionLayout<A>> that) override {
		fake = mul (that) ;
	}

	Vector<A> real (CREF<QuaternionLayout<A>> q) const {
		return Vector<A> (q.mQuaternion[0] ,q.mQuaternion[1] ,q.mQuaternion[2] ,0) ;
	}

	A angle () const {
		const auto r1x = real (fake).magnitude () ;
		const auto r2x = fake.mQuaternion[3] ;
		return Math::arctan (r1x ,r2x) * 2 ;
	}

	VectorLayout<A> vector () const override {
		const auto r1x = real (fake).normalize () ;
		const auto r2x = angle () ;
		return r1x * r2x ;
	}

	MatrixLayout<A> matrix () const override {
		const auto r1x = real (fake).normalize () ;
		const auto r2x = angle () ;
		return RotationMatrix<A>::make (r1x ,r2x) ;
	}
} ;

template <class A>
exports VFat<QuaternionHolder<A>> QuaternionHolder<A>::create (VREF<QuaternionLayout<A>> that) {
	return VFat<QuaternionHolder<A>> (QuaternionImplHolder<A> () ,that) ;
}

template <class A>
exports CFat<QuaternionHolder<A>> QuaternionHolder<A>::create (CREF<QuaternionLayout<A>> that) {
	return CFat<QuaternionHolder<A>> (QuaternionImplHolder<A> () ,that) ;
}

template struct QuaternionHolder<FLT32> ;
template struct QuaternionHolder<FLT64> ;
} ;