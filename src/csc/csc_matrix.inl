#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_matrix.hpp"

namespace CSC {
class VectorImplement implement Fat<VectorHolder ,VectorLayout> {
private:
	using Fat<VectorHolder ,VectorLayout>::fake_m ;

public:
	void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		fake.mVector[0] = x ;
		fake.mVector[1] = y ;
		fake.mVector[2] = z ;
		fake.mVector[3] = w ;
	}

	void initialize (CREF<Pixel> point) override {
		fake.mVector[0] = FLT64 (point.mX) + FLT64 (0.5) ;
		fake.mVector[1] = FLT64 (point.mY) + FLT64 (0.5) ;
		fake.mVector[2] = 0 ;
		fake.mVector[3] = 1 ;
	}

	void initialize (CREF<Point2F> point) override {
		fake.mVector[0] = FLT64 (point.mX) ;
		fake.mVector[1] = FLT64 (point.mY) ;
		fake.mVector[2] = 0 ;
		fake.mVector[3] = 1 ;
	}

	void initialize (CREF<Point3F> point) override {
		fake.mVector[0] = FLT64 (point.mX) ;
		fake.mVector[1] = FLT64 (point.mY) ;
		fake.mVector[2] = FLT64 (point.mZ) ;
		fake.mVector[3] = 1 ;
	}

	Point2F xy () const override {
		Point2F ret ;
		ret.mX = FLT32 (fake.mVector[0]) ;
		ret.mY = FLT32 (fake.mVector[1]) ;
		return move (ret) ;
	}

	Point3F xyz () const override {
		Point3F ret ;
		ret.mX = FLT32 (fake.mVector[0]) ;
		ret.mY = FLT32 (fake.mVector[1]) ;
		ret.mZ = FLT32 (fake.mVector[2]) ;
		return move (ret) ;
	}

	VREF<FLT64> at (CREF<INDEX> y) leftvalue override {
		return fake.mVector[y] ;
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue override {
		return fake.mVector[y] ;
	}

	BOOL equal (CREF<VectorLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_equal (fake.mVector[i] ,that.mVector[i]) ;
			if ifnot (r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<VectorLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_compr (fake.mVector[i] ,that.mVector[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		for (auto &&i : iter (0 ,4)) {
			inline_visit (visitor ,fake.mVector[i]) ;
		}
		visitor.end () ;
	}

	VectorLayout add (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] + that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout sub (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] - that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout mul (CREF<FLT64> scale) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] * scale ;
		}
		return move (ret) ;
	}

	VectorLayout div (CREF<FLT64> scale) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = fake.mVector[i] / scale ;
		}
		return move (ret) ;
	}

	FLT64 dot (CREF<VectorLayout> that) const override {
		FLT64 ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += fake.mVector[i] * that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout mul (CREF<MatrixLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += fake.mVector[0] * that.mMatrix[i + 0 * 4] ;
			rax += fake.mVector[1] * that.mMatrix[i + 1 * 4] ;
			rax += fake.mVector[2] * that.mMatrix[i + 2 * 4] ;
			rax += fake.mVector[3] * that.mMatrix[i + 3 * 4] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	VectorLayout cross (CREF<VectorLayout> that) const override {
		assert (fake.mVector[3] == 0) ;
		assert (that.mVector[3] == 0) ;
		VectorLayout ret ;
		ret.mVector[0] = fake.mVector[1] * that.mVector[2] - fake.mVector[2] * that.mVector[1] ;
		ret.mVector[1] = fake.mVector[2] * that.mVector[0] - fake.mVector[0] * that.mVector[2] ;
		ret.mVector[2] = fake.mVector[0] * that.mVector[1] - fake.mVector[1] * that.mVector[0] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout plus () const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = +fake.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout minus () const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			ret.mVector[i] = -fake.mVector[i] ;
		}
		return move (ret) ;
	}

	FLT64 magnitude () const override {
		assert (fake.mVector[3] == 0) ;
		FLT64 ret = 0 ;
		ret += MathProc::square (fake.mVector[0]) ;
		ret += MathProc::square (fake.mVector[1]) ;
		ret += MathProc::square (fake.mVector[2]) ;
		ret = MathProc::sqrt (ret) ;
		return move (ret) ;
	}

	VectorLayout normalize () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (magnitude ()) ;
		ret.mVector[0] = fake.mVector[0] * r1x ;
		ret.mVector[1] = fake.mVector[1] * r1x ;
		ret.mVector[2] = fake.mVector[2] * r1x ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout projection () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (fake.mVector[3]) ;
		ret.mVector[0] = fake.mVector[0] * r1x ;
		ret.mVector[1] = fake.mVector[1] * r1x ;
		ret.mVector[2] = fake.mVector[2] * r1x ;
		ret.mVector[3] = 1 ;
		return move (ret) ;
	}

	VectorLayout homogenize () const override {
		VectorLayout ret ;
		ret.mVector[0] = fake.mVector[0] ;
		ret.mVector[1] = fake.mVector[1] ;
		ret.mVector[2] = fake.mVector[2] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}
} ;

exports VFat<VectorHolder> VectorHolder::create (VREF<VectorLayout> that) {
	return VFat<VectorHolder> (VectorImplement () ,that) ;
}

exports CFat<VectorHolder> VectorHolder::create (CREF<VectorLayout> that) {
	return CFat<VectorHolder> (VectorImplement () ,that) ;
}

class MatrixImplement implement Fat<MatrixHolder ,MatrixLayout> {
private:
	using Fat<MatrixHolder ,MatrixLayout>::fake_m ;

public:
	void initialize (CREF<VectorLayout> x ,CREF<VectorLayout> y ,CREF<VectorLayout> z ,CREF<VectorLayout> w) override {
		for (auto &&i : iter (0 ,4)) {
			fake.mMatrix[mm (0 ,i)] = x.mVector[i] ;
			fake.mMatrix[mm (1 ,i)] = y.mVector[i] ;
			fake.mMatrix[mm (2 ,i)] = z.mVector[i] ;
			fake.mMatrix[mm (3 ,i)] = w.mVector[i] ;
		}
	}

	VREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return fake.mMatrix[mm (x ,y)] ;
	}

	CREF<FLT64> at (CREF<INDEX> x ,CREF<INDEX> y) const leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return fake.mMatrix[mm (x ,y)] ;
	}

	imports INDEX mm (CREF<INDEX> x ,CREF<INDEX> y) {
		return x + y * 4 ;
	}

	BOOL equal (CREF<MatrixLayout> that) const override {
		for (auto &&i : iter (0 ,16)) {
			const auto r1x = inline_equal (fake.mMatrix[i] ,that.mMatrix[i]) ;
			if ifnot (r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<MatrixLayout> that) const override {
		for (auto &&i : iter (0 ,16)) {
			const auto r1x = inline_compr (fake.mMatrix[i] ,that.mMatrix[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		for (auto &&i : iter (0 ,16)) {
			inline_visit (visitor ,fake.mMatrix[i]) ;
		}
		visitor.end () ;
	}

	MatrixLayout add (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] + that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout sub (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] - that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout mul (CREF<FLT64> scale) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] * scale ;
		}
		return move (ret) ;
	}

	MatrixLayout div (CREF<FLT64> scale) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = fake.mMatrix[i] / scale ;
		}
		return move (ret) ;
	}

	VectorLayout mul (CREF<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : iter (0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += fake.mMatrix[mm (0 ,i)] * that.mVector[0] ;
			rax += fake.mMatrix[mm (1 ,i)] * that.mVector[1] ;
			rax += fake.mMatrix[mm (2 ,i)] * that.mVector[2] ;
			rax += fake.mMatrix[mm (3 ,i)] * that.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout mul (CREF<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			auto rax = FLT64 (0) ;
			rax += fake.mMatrix[mm (0 ,i.mY)] * that.mMatrix[mm (i.mX ,0)] ;
			rax += fake.mMatrix[mm (1 ,i.mY)] * that.mMatrix[mm (i.mX ,1)] ;
			rax += fake.mMatrix[mm (2 ,i.mY)] * that.mMatrix[mm (i.mX ,2)] ;
			rax += fake.mMatrix[mm (3 ,i.mY)] * that.mMatrix[mm (i.mX ,3)] ;
			ret.mMatrix[mm (i.mX ,i.mY)] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout plus () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = +fake.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout minus () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,16)) {
			ret.mMatrix[i] = -fake.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout transpose () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret.mMatrix[mm (i.mY ,i.mX)] = fake.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	MatrixLayout triangular () const override {
		MatrixLayout ret = fake ;
		for (auto &&i : iter (0 ,4)) {
			if ifdo (TRUE) {
				INDEX ix = find_abs_max_row (i) ;
				if (ix == i)
					discard ;
				for (auto &&j : iter (i ,4)) {
					const auto r1x = -ret.mMatrix[mm (j ,i)] ;
					ret.mMatrix[mm (j ,i)] = ret.mMatrix[mm (j ,ix)] ;
					ret.mMatrix[mm (j ,ix)] = r1x ;
				}
			}
			const auto r2x = MathProc::inverse (ret.mMatrix[mm (i ,i)]) ;
			if (r2x == 0)
				continue ;
			for (auto &&j : iter (i + 1 ,4)) {
				const auto r3x = ret.mMatrix[mm (i ,j)] * r2x ;
				for (auto &&k : iter (i + 1 ,4))
					ret.mMatrix[mm (k ,j)] -= r3x * ret.mMatrix[mm (k ,i)] ;
				ret.mMatrix[mm (i ,j)] = 0 ;
			}
		}
		return move (ret) ;
	}

	INDEX find_abs_max_row (CREF<INDEX> x) const {
		INDEX ret = NONE ;
		auto rax = FLT64 () ;
		for (auto &&i : iter (x ,4)) {
			const auto r1x = MathProc::abs (fake.mMatrix[mm (x ,i)]) ;
			if (ret != NONE)
				if (rax >= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return move (ret) ;
	}

	MatrixLayout homogenize () const override {
		MatrixLayout ret = Matrix::zero () ;
		for (auto &&i : iter (0 ,3 ,0 ,3)) {
			ret.mMatrix[mm (i.mX ,i.mY)] = fake.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	FLT64 det () const override {
		const auto r1x = triangular () ;
		FLT64 ret = 1 ;
		for (auto &&i : iter (0 ,4))
			ret *= r1x.mMatrix[mm (i ,i)] ;
		return move (ret) ;
	}

	MatrixLayout inverse () const override {
		MatrixLayout ret ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			INDEX ix = LENGTH (i.mY == 0) ;
			INDEX iy = LENGTH (i.mY == 1) + ix + 1 ;
			INDEX iz = LENGTH (i.mY == 2) + iy + 1 ;
			INDEX jx = LENGTH (i.mX == 0) ;
			INDEX jy = LENGTH (i.mX == 1) + jx + 1 ;
			INDEX jz = LENGTH (i.mX == 2) + jy + 1 ;
			const auto r1x = fake.mMatrix[mm (jy ,iy)] * fake.mMatrix[mm (jz ,iz)] - fake.mMatrix[mm (jz ,iy)] * fake.mMatrix[mm (jy ,iz)] ;
			const auto r2x = fake.mMatrix[mm (jy ,ix)] * fake.mMatrix[mm (jz ,iz)] - fake.mMatrix[mm (jz ,ix)] * fake.mMatrix[mm (jy ,iz)] ;
			const auto r3x = fake.mMatrix[mm (jy ,ix)] * fake.mMatrix[mm (jz ,iy)] - fake.mMatrix[mm (jz ,ix)] * fake.mMatrix[mm (jy ,iy)] ;
			const auto r4x = fake.mMatrix[mm (jx ,ix)] * r1x ;
			const auto r5x = fake.mMatrix[mm (jx ,iy)] * r2x ;
			const auto r6x = fake.mMatrix[mm (jx ,iz)] * r3x ;
			const auto r7x = (r4x - r5x + r6x) * det_sign (i.mY + i.mX) ;
			ret.mMatrix[mm (i.mY ,i.mX)] = r7x ;
		}
		const auto r8x = MathProc::inverse (det ()) ;
		ret = MatrixHolder::create (ret)->mul (r8x) ;
		return move (ret) ;
	}

	FLT64 det_sign (CREF<INDEX> index) const {
		if (index % 2 == 0)
			return 1 ;
		return -1 ;
	}

	FLT64 trace () const override {
		FLT64 ret = 0 ;
		for (auto &&i : iter (0 ,4)) {
			ret += fake.mMatrix[mm (i ,i)] ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MatrixHolder> MatrixHolder::create (VREF<MatrixLayout> that) {
	return VFat<MatrixHolder> (MatrixImplement () ,that) ;
}

exports CFat<MatrixHolder> MatrixHolder::create (CREF<MatrixLayout> that) {
	return CFat<MatrixHolder> (MatrixImplement () ,that) ;
}

class MakeMatrixImplement implement Fat<MakeMatrixHolder ,MatrixLayout> {
private:
	using Fat<MakeMatrixHolder ,MatrixLayout>::fake_m ;

public:
	void DiagMatrix_initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		Matrix ret = Matrix::zero () ;
		ret[0][0] = x ;
		ret[1][1] = y ;
		ret[2][2] = z ;
		ret[1][3] = w ;
		fake = move (ret) ;
	}

	void ShearMatrix_initialize (CREF<Vector> x ,CREF<Vector> y ,CREF<Vector> z) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = z.normalize () ;
		const auto r4x = r1x * r2x ;
		const auto r5x = r1x * r3x ;
		const auto r6x = r2x * r3x ;
		const auto r7x = MathProc::sqrt (1 - MathProc::square (r4x)) ;
		const auto r8x = (r6x - r4x * r5x) * MathProc::inverse (r7x) ;
		const auto r9x = MathProc::sqrt (1 - MathProc::square (r5x) - MathProc::square (r8x)) ;
		ret[0][0] = 1 ;
		ret[0][1] = r4x ;
		ret[1][1] = r7x ;
		ret[0][2] = r5x ;
		ret[1][2] = r8x ;
		ret[2][3] = r9x ;
		ret[3][3] = 1 ;
		fake = move (ret) ;
	}

	void RotationMatrix_initialize (CREF<Vector> normal ,CREF<FLT64> angle) override {
		Matrix ret = Matrix::zero () ;
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

	void TranslationMatrix_initialize (CREF<Vector> xyz) override {
		TranslationMatrix_initialize (xyz[0] ,xyz[1] ,xyz[2]) ;
	}

	void TranslationMatrix_initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z) override {
		Matrix ret = Matrix::identity () ;
		ret[0][3] = x ;
		ret[1][3] = y ;
		ret[2][3] = z ;
		fake = move (ret) ;
	}

	void PerspectiveMatrix_initialize (CREF<FLT64> fx ,CREF<FLT64> fy ,CREF<FLT64> wx ,CREF<FLT64> wy) override {
		assert (fx > 0) ;
		assert (fy > 0) ;
		Matrix ret = Matrix::zero () ;
		ret[0][0] = fx ;
		ret[1][1] = fy ;
		ret[0][2] = wx ;
		ret[1][2] = wx ;
		ret[3][2] = 1 ;
		ret[2][3] = 1 ;
		fake = move (ret) ;
	}

	void ProjectionMatrix_initialize (CREF<Vector> normal ,CREF<Vector> center ,CREF<Vector> light) override {
		Matrix ret = Matrix::zero () ;
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

	void ViewMatrix_initialize (CREF<Vector> x ,CREF<Vector> y) override {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		const auto r5x = Vector::axis_w () ;
		fake = Matrix (r1x ,r4x ,r3x ,r5x) ;
	}

	void ViewMatrix_initialize (CREF<Vector> x ,CREF<Vector> y ,CREF<FLAG> flag) override {
		ViewMatrix_initialize (x ,y) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (flag != ViewMatrixOption::XY)
				discard ;
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			fake = MatrixHolder::create (fake)->mul (r5x) ;
		}
		if ifdo (act) {
			if (flag != ViewMatrixOption::XZ)
				discard ;
			const auto r6x = Vector::axis_x () ;
			const auto r7x = -Vector::axis_z () ;
			const auto r8x = Vector::axis_y () ;
			const auto r9x = Vector::axis_w () ;
			const auto r10x = Matrix (r6x ,r7x ,r8x ,r9x) ;
			fake = MatrixHolder::create (fake)->mul (r10x) ;
		}
		if ifdo (act) {
			if (flag != ViewMatrixOption::YX)
				discard ;
			const auto r11x = Vector::axis_y () ;
			const auto r12x = Vector::axis_x () ;
			const auto r13x = -Vector::axis_z () ;
			const auto r14x = Vector::axis_w () ;
			const auto r15x = Matrix (r11x ,r12x ,r13x ,r14x) ;
			fake = MatrixHolder::create (fake)->mul (r15x) ;
		}
		if ifdo (act) {
			if (flag != ViewMatrixOption::YZ)
				discard ;
			const auto r16x = Vector::axis_z () ;
			const auto r17x = Vector::axis_x () ;
			const auto r18x = Vector::axis_y () ;
			const auto r19x = Vector::axis_w () ;
			const auto r20x = Matrix (r16x ,r17x ,r18x ,r19x) ;
			fake = MatrixHolder::create (fake)->mul (r20x) ;
		}
		if ifdo (act) {
			if (flag != ViewMatrixOption::ZX)
				discard ;
			const auto r21x = Vector::axis_y () ;
			const auto r22x = Vector::axis_z () ;
			const auto r23x = Vector::axis_x () ;
			const auto r24x = Vector::axis_w () ;
			const auto r25x = Matrix (r21x ,r22x ,r23x ,r24x) ;
			fake = MatrixHolder::create (fake)->mul (r25x) ;
		}
		if ifdo (act) {
			if (flag != ViewMatrixOption::ZY)
				discard ;
			const auto r26x = -Vector::axis_z () ;
			const auto r27x = Vector::axis_y () ;
			const auto r28x = Vector::axis_x () ;
			const auto r29x = Vector::axis_w () ;
			const auto r30x = Matrix (r26x ,r27x ,r28x ,r29x) ;
			fake = MatrixHolder::create (fake)->mul (r30x) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void CrossProductMatrix_initialize (CREF<Vector> xyz) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = xyz.homogenize () ;
		ret[1][0] = r1x[2] ;
		ret[2][0] = -r1x[1] ;
		ret[0][1] = -r1x[2] ;
		ret[2][1] = r1x[0] ;
		ret[0][2] = r1x[1] ;
		ret[1][2] = -r1x[0] ;
		fake = move (ret) ;
	}

	void SymmetryMatrix_initialize (CREF<Vector> x ,CREF<Vector> y) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = x ;
		const auto r2x = y ;
		for (auto &&i : iter (0 ,4 ,0 ,4)) {
			ret[i] = r1x[i.mY] * r2x[i.mX] ;
		}
		fake = move (ret) ;
	}
} ;

exports VFat<MakeMatrixHolder> MakeMatrixHolder::create (VREF<MatrixLayout> that) {
	return VFat<MakeMatrixHolder> (MakeMatrixImplement () ,that) ;
}

exports CFat<MakeMatrixHolder> MakeMatrixHolder::create (CREF<MatrixLayout> that) {
	return CFat<MakeMatrixHolder> (MakeMatrixImplement () ,that) ;
}

#ifndef __CSC_API_WITH_EIGEN__
class MatrixProcImplement implement Fat<MatrixProcHolder ,MatrixProcLayout> {} ;

exports VFat<MatrixProcHolder> MatrixProcHolder::create (VREF<MatrixProcLayout> that) {
	assume (FALSE) ;
	return VFat<MatrixProcHolder> (keep[TYPE<MatrixProcImplement>::expr] (Pointer::make (0)) ,that) ;
}

exports CFat<MatrixProcHolder> MatrixProcHolder::create (CREF<MatrixProcLayout> that) {
	assume (FALSE) ;
	return CFat<MatrixProcHolder> (keep[TYPE<MatrixProcImplement>::expr] (Pointer::make (0)) ,that) ;
}
#endif

class QuaternionImplement implement Fat<QuaternionHolder ,QuaternionLayout> {
private:
	using Fat<QuaternionHolder ,QuaternionLayout>::fake_m ;

public:
	void initialize (CREF<FLT64> x ,CREF<FLT64> y ,CREF<FLT64> z ,CREF<FLT64> w) override {
		fake.mQuaternion[0] = x ;
		fake.mQuaternion[1] = y ;
		fake.mQuaternion[2] = z ;
		fake.mQuaternion[3] = w ;
	}

	void initialize (CREF<MatrixLayout> that) override {
		const auto r1x = move (keep[TYPE<Matrix>::expr] (that)) ;
		const auto r2x = r1x.homogenize () ;
		auto act = TRUE ;
		if ifdo (act) {
			const auto r3x = 1 + r2x.trace () ;
			if (r3x < 1)
				discard ;
			const auto r4x = MathProc::sqrt (r3x) ;
			const auto r5x = MathProc::inverse (r4x) ;
			const auto r6x = (r2x[2][1] - r2x[1][2]) * r5x ;
			const auto r7x = (r2x[0][2] - r2x[2][0]) * r5x ;
			const auto r8x = (r2x[1][0] - r2x[0][1]) * r5x ;
			fake.mQuaternion[0] = r6x / 2 ;
			fake.mQuaternion[1] = r7x / 2 ;
			fake.mQuaternion[2] = r8x / 2 ;
			fake.mQuaternion[3] = r4x / 2 ;
		}
		if ifdo (act) {
			const auto r9x = 1 + r2x[0][0] - r2x[1][1] - r2x[2][2] ;
			if (r9x < 1)
				discard ;
			const auto r10x = MathProc::sqrt (r9x) ;
			const auto r11x = MathProc::inverse (r10x) ;
			const auto r12x = (r2x[1][0] + r2x[0][1]) * r11x ;
			const auto r13x = (r2x[0][2] + r2x[2][0]) * r11x ;
			const auto r14x = (r2x[2][1] - r2x[1][2]) * r11x ;
			fake.mQuaternion[0] = r10x / 2 ;
			fake.mQuaternion[1] = r12x / 2 ;
			fake.mQuaternion[2] = r13x / 2 ;
			fake.mQuaternion[3] = r14x / 2 ;
		}
		if ifdo (act) {
			const auto r15x = 1 - r2x[0][0] + r2x[1][1] - r2x[2][2] ;
			if (r15x < 1)
				discard ;
			const auto r16x = MathProc::sqrt (r15x) ;
			const auto r17x = MathProc::inverse (r16x) ;
			const auto r18x = (r2x[1][0] + r2x[0][1]) * r17x ;
			const auto r19x = (r2x[2][1] + r2x[1][2]) * r17x ;
			const auto r20x = (r2x[0][2] - r2x[2][0]) * r17x ;
			fake.mQuaternion[0] = r18x / 2 ;
			fake.mQuaternion[1] = r16x / 2 ;
			fake.mQuaternion[2] = r19x / 2 ;
			fake.mQuaternion[3] = r20x / 2 ;
		}
		if ifdo (act) {
			const auto r21x = 1 - r2x[0][0] - r2x[1][1] + r2x[2][2] ;
			if (r21x < 1)
				discard ;
			const auto r22x = MathProc::sqrt (r21x) ;
			const auto r23x = MathProc::inverse (r22x) ;
			const auto r24x = (r2x[0][2] + r2x[2][0]) * r23x ;
			const auto r25x = (r2x[2][1] + r2x[1][2]) * r23x ;
			const auto r26x = (r2x[1][0] - r2x[0][1]) * r23x ;
			fake.mQuaternion[0] = r24x / 2 ;
			fake.mQuaternion[1] = r25x / 2 ;
			fake.mQuaternion[2] = r22x / 2 ;
			fake.mQuaternion[3] = r26x / 2 ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	CREF<FLT64> at (CREF<INDEX> y) const leftvalue override {
		return fake.mQuaternion[y] ;
	}

	BOOL equal (CREF<QuaternionLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_equal (fake.mQuaternion[i] ,that.mQuaternion[i]) ;
			if ifnot (r1x)
				return r1x ;
		}
		return TRUE ;
	}

	FLAG compr (CREF<QuaternionLayout> that) const override {
		for (auto &&i : iter (0 ,4)) {
			const auto r1x = inline_compr (fake.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CREF<Visitor> visitor) const override {
		visitor.begin () ;
		for (auto &&i : iter (0 ,4)) {
			inline_visit (visitor ,fake.mQuaternion[i]) ;
		}
		visitor.end () ;
	}

	QuaternionLayout mul (CREF<QuaternionLayout> that) const override {
		QuaternionLayout ret ;
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

	Vector real (CREF<QuaternionLayout> q) const {
		return Vector (q.mQuaternion[0] ,q.mQuaternion[1] ,q.mQuaternion[2] ,0) ;
	}

	FLT64 angle () const {
		const auto r1x = real (fake).magnitude () ;
		const auto r2x = fake.mQuaternion[3] ;
		return MathProc::arctan (r1x ,r2x) * 2 ;
	}

	VectorLayout vector () const override {
		const auto r1x = real (fake).normalize () ;
		const auto r2x = angle () ;
		return r1x * r2x ;
	}

	MatrixLayout matrix () const override {
		const auto r1x = real (fake).normalize () ;
		const auto r2x = angle () ;
		return RotationMatrix::make (r1x ,r2x) ;
	}
} ;

exports VFat<QuaternionHolder> QuaternionHolder::create (VREF<QuaternionLayout> that) {
	return VFat<QuaternionHolder> (QuaternionImplement () ,that) ;
}

exports CFat<QuaternionHolder> QuaternionHolder::create (CREF<QuaternionLayout> that) {
	return CFat<QuaternionHolder> (QuaternionImplement () ,that) ;
}

template <class A>
class PointCloudImplLayout implement Proxy {
private:
	Array<A> mPointCloud ;

public:
	imports VREF<PointCloudImplLayout> from (VREF<PointCloudLayout> that) {
		return Pointer::from (that) ;
	}

	imports CREF<PointCloudImplLayout> from (CREF<PointCloudLayout> that) {
		return Pointer::from (that) ;
	}

	LENGTH size () const {
		return mPointCloud.size () ;
	}

	void get (CREF<INDEX> index ,VREF<Vector> item) const {
		item = Vector (mPointCloud[index]) ;
	}

	Vector center () const {
		Vector ret = Vector::axis_w () ;
		for (auto &&i : iter (0 ,mPointCloud.size ())) {
			const auto r1x = Vector (mPointCloud[i]) ;
			ret += r1x ;
		}
		ret = ret.projection () ;
		return move (ret) ;
	}

	Matrix svd_matrix () const {
		const auto r1x = center () ;
		const auto r2x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			for (auto &&i : iter (0 ,mPointCloud.size ())) {
				const auto r3x = Vector (mPointCloud[i]) ;
				const auto r4x = (r3x - r1x).normalize () ;
				ret[0][0] += MathProc::square (r4x[0]) ;
				ret[0][1] += r4x[0] * r4x[1] ;
				ret[0][2] += r4x[0] * r4x[2] ;
				ret[1][1] += MathProc::square (r4x[1]) ;
				ret[1][2] += r4x[1] * r4x[2] ;
				ret[2][2] += MathProc::square (r4x[2]) ;
			}
			ret[1][0] = ret[0][1] ;
			ret[2][0] = ret[0][2] ;
			ret[2][1] = ret[1][2] ;
			return move (ret) ;
		}) ;
		const auto r5x = MatrixProc::solve_svd (r2x) ;
		const auto r6x = TranslationMatrix::make (r1x[0] ,r1x[1] ,r1x[2]) ;
		const auto r7x = DiagMatrix::make (MathProc::sqrt (r5x.mS[0][0]) ,MathProc::sqrt (r5x.mS[1][1]) ,MathProc::sqrt (r5x.mS[2][2]) ,1) ;
		const auto r8x = r6x * r5x.mV * rank_fix (r7x) ;
		return r8x ;
	}

	Matrix rank_fix (CREF<Matrix> a) const {
		Matrix ret = a ;
		for (auto &&i : iter (0 ,4)) {
			if (MathProc::inverse (ret[i][i]) != 0)
				continue ;
			ret[i][i] = 1 ;
		}
		return move (ret) ;
	}

	Matrix box_matrix () const {
		const auto r1x = bound () ;
		const auto r2x = invoke ([&] () {
			Vector ret = Vector::zero () ;
			INDEX ix = (size () - 1) / 2 ;
			INDEX iy = size () / 2 ;
			ret += Vector (mPointCloud[ix]) ;
			ret += Vector (mPointCloud[iy]) ;
			ret = ret.projection () ;
			return move (ret) ;
		}) ;
		const auto r3x = TranslationMatrix::make (r2x[0] ,r2x[1] ,r2x[2]) ;
		const auto r4x = DiagMatrix::make (r1x.mMax.mX - r1x.mMin.mX ,r1x.mMax.mY - r1x.mMin.mY ,1 ,1) ;
		return r3x * r4x ;
	}

	Rect3F bound () const {
		Rect3F ret ;
		ret.mMin.mX = +FLT64_INF ;
		ret.mMin.mY = +FLT64_INF ;
		ret.mMin.mZ = +FLT64_INF ;
		ret.mMax.mX = -FLT64_INF ;
		ret.mMax.mY = -FLT64_INF ;
		ret.mMax.mZ = -FLT64_INF ;
		for (auto &&i : iter (0 ,mPointCloud.size ())) {
			const auto r1x = Vector (mPointCloud[i]) ;
			ret.mMin.mX = MathProc::min_of (ret.mMin.mX ,FLT32 (r1x[0])) ;
			ret.mMin.mY = MathProc::min_of (ret.mMin.mY ,FLT32 (r1x[1])) ;
			ret.mMin.mZ = MathProc::min_of (ret.mMin.mZ ,FLT32 (r1x[2])) ;
			ret.mMax.mX = MathProc::max_of (ret.mMax.mX ,FLT32 (r1x[0])) ;
			ret.mMax.mY = MathProc::max_of (ret.mMax.mY ,FLT32 (r1x[1])) ;
			ret.mMax.mZ = MathProc::max_of (ret.mMax.mZ ,FLT32 (r1x[2])) ;
		}
		return move (ret) ;
	}
} ;

class PointCloudImplement implement Fat<PointCloudHolder ,PointCloudLayout> {
public:
	void initialize (RREF<Array<Point2F>> that) override {
		fake.mRank = 2 ;
		fake.mPointCloud = move (that) ;
	}

	void initialize (RREF<Array<Point3F>> that) override {
		fake.mRank = 3 ;
		fake.mPointCloud = move (that) ;
	}

	LENGTH size () const override {
		if (fake.mRank == 2)
			return PointCloudImplLayout<Point2F>::from (fake).size () ;
		if (fake.mRank == 3)
			return PointCloudImplLayout<Point3F>::from (fake).size () ;
		assume (FALSE) ;
		return 0 ;
	}

	void get (CREF<INDEX> index ,VREF<Vector> item) const override {
		if (fake.mRank == 2)
			return PointCloudImplLayout<Point2F>::from (fake).get (index ,item) ;
		if (fake.mRank == 3)
			return PointCloudImplLayout<Point3F>::from (fake).get (index ,item) ;
		assume (FALSE) ;
		return ;
	}

	Vector center () const override {
		if (fake.mRank == 2)
			return PointCloudImplLayout<Point2F>::from (fake).center () ;
		if (fake.mRank == 3)
			return PointCloudImplLayout<Point3F>::from (fake).center () ;
		assume (FALSE) ;
		return Vector () ;
	}

	Matrix svd_matrix () const override {
		if (fake.mRank == 2)
			return PointCloudImplLayout<Point2F>::from (fake).svd_matrix () ;
		if (fake.mRank == 3)
			return PointCloudImplLayout<Point3F>::from (fake).svd_matrix () ;
		assume (FALSE) ;
		return Matrix () ;
	}

	Matrix box_matrix () const override {
		if (fake.mRank == 2)
			return PointCloudImplLayout<Point2F>::from (fake).box_matrix () ;
		if (fake.mRank == 3)
			return PointCloudImplLayout<Point3F>::from (fake).box_matrix () ;
		assume (FALSE) ;
		return Matrix () ;
	}

	Rect3F bound () const override {
		if (fake.mRank == 2)
			return PointCloudImplLayout<Point2F>::from (fake).bound () ;
		if (fake.mRank == 3)
			return PointCloudImplLayout<Point3F>::from (fake).bound () ;
		assume (FALSE) ;
		return Rect3F () ;
	}
} ;

exports VFat<PointCloudHolder> PointCloudHolder::create (VREF<PointCloudLayout> that) {
	return VFat<PointCloudHolder> (PointCloudImplement () ,that) ;
}

exports CFat<PointCloudHolder> PointCloudHolder::create (CREF<PointCloudLayout> that) {
	return CFat<PointCloudHolder> (PointCloudImplement () ,that) ;
}
} ;