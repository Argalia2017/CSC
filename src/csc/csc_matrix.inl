#pragma once

#ifndef __CSC_MATRIX__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_matrix.hpp"

namespace CSC {
class VectorImplHolder final implement Fat<VectorHolder ,VectorLayout> {
public:
	void initialize (CR<Buffer<Flt64 ,RANK4>> that) override {
		self.mVector[0] = that[0] ;
		self.mVector[1] = that[1] ;
		self.mVector[2] = that[2] ;
		self.mVector[3] = that[3] ;
	}

	void initialize (CR<Flt64> x ,CR<Flt64> y ,CR<Flt64> z ,CR<Flt64> w) override {
		self.mVector[0] = x ;
		self.mVector[1] = y ;
		self.mVector[2] = z ;
		self.mVector[3] = w ;
	}

	void initialize (CR<Pixel> point) override {
		self.mVector[0] = Flt64 (point.mX) + Flt64 (0.5) ;
		self.mVector[1] = Flt64 (point.mY) + Flt64 (0.5) ;
		self.mVector[2] = 0 ;
		self.mVector[3] = 1 ;
	}

	void initialize (CR<Point2F> point) override {
		self.mVector[0] = Flt64 (point.mX) ;
		self.mVector[1] = Flt64 (point.mY) ;
		self.mVector[2] = 0 ;
		self.mVector[3] = 1 ;
	}

	void initialize (CR<Point3F> point) override {
		self.mVector[0] = Flt64 (point.mX) ;
		self.mVector[1] = Flt64 (point.mY) ;
		self.mVector[2] = Flt64 (point.mZ) ;
		self.mVector[3] = 1 ;
	}

	VR<Flt64> at (CR<Index> y) leftvalue override {
		return self.mVector[y] ;
	}

	CR<Flt64> at (CR<Index> y) const leftvalue override {
		return self.mVector[y] ;
	}

	static Index mm (CR<Index> x ,CR<Index> y) {
		return x + y * 4 ;
	}

	Bool equal (CR<VectorLayout> that) const override {
		for (auto &&i : range (0 ,4)) {
			const auto r1x = inline_equal (self.mVector[i] ,that.mVector[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	Flag compr (CR<VectorLayout> that) const override {
		for (auto &&i : range (0 ,4)) {
			const auto r1x = inline_compr (self.mVector[i] ,that.mVector[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		for (auto &&i : range (0 ,4)) {
			inline_visit (visitor ,self.mVector[i]) ;
		}
		visitor.leave () ;
	}

	VectorLayout sadd (CR<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			ret.mVector[i] = self.mVector[i] + that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout ssub (CR<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			ret.mVector[i] = self.mVector[i] - that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CR<Flt64> that) const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			ret.mVector[i] = self.mVector[i] * that ;
		}
		return move (ret) ;
	}

	VectorLayout sdiv (CR<Flt64> that) const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			ret.mVector[i] = self.mVector[i] / that ;
		}
		return move (ret) ;
	}

	Flt64 dot (CR<VectorLayout> that) const override {
		Flt64 ret = 0 ;
		for (auto &&i : range (0 ,4)) {
			ret += self.mVector[i] * that.mVector[i] ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CR<MatrixLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			auto rax = Flt64 (0) ;
			rax += self.mVector[0] * that.mMatrix[mm (i ,0)] ;
			rax += self.mVector[1] * that.mMatrix[mm (i ,1)] ;
			rax += self.mVector[2] * that.mMatrix[mm (i ,2)] ;
			rax += self.mVector[3] * that.mMatrix[mm (i ,3)] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	VectorLayout cross (CR<VectorLayout> that) const override {
		assert (self.mVector[3] == 0) ;
		assert (that.mVector[3] == 0) ;
		VectorLayout ret ;
		ret.mVector[0] = self.mVector[1] * that.mVector[2] - self.mVector[2] * that.mVector[1] ;
		ret.mVector[1] = self.mVector[2] * that.mVector[0] - self.mVector[0] * that.mVector[2] ;
		ret.mVector[2] = self.mVector[0] * that.mVector[1] - self.mVector[1] * that.mVector[0] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout sabs () const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			ret.mVector[i] = MathProc::abs (self.mVector[i]) ;
		}
		return move (ret) ;
	}

	VectorLayout minus () const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			ret.mVector[i] = -self.mVector[i] ;
		}
		return move (ret) ;
	}

	Flt64 magnitude () const override {
		assert (self.mVector[3] == 0) ;
		Flt64 ret = 0 ;
		ret += MathProc::square (self.mVector[0]) ;
		ret += MathProc::square (self.mVector[1]) ;
		ret += MathProc::square (self.mVector[2]) ;
		ret = MathProc::sqrt (ret) ;
		return move (ret) ;
	}

	VectorLayout normalize () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (magnitude ()) ;
		ret.mVector[0] = self.mVector[0] * r1x ;
		ret.mVector[1] = self.mVector[1] * r1x ;
		ret.mVector[2] = self.mVector[2] * r1x ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}

	VectorLayout projection () const override {
		VectorLayout ret ;
		const auto r1x = MathProc::inverse (self.mVector[3]) ;
		ret.mVector[0] = self.mVector[0] * r1x ;
		ret.mVector[1] = self.mVector[1] * r1x ;
		ret.mVector[2] = self.mVector[2] * r1x ;
		ret.mVector[3] = 1 ;
		return move (ret) ;
	}

	VectorLayout homogenize () const override {
		VectorLayout ret ;
		ret.mVector[0] = self.mVector[0] ;
		ret.mVector[1] = self.mVector[1] ;
		ret.mVector[2] = self.mVector[2] ;
		ret.mVector[3] = 0 ;
		return move (ret) ;
	}
} ;

exports VFat<VectorHolder> VectorHolder::hold (VR<VectorLayout> that) {
	return VFat<VectorHolder> (VectorImplHolder () ,that) ;
}

exports CFat<VectorHolder> VectorHolder::hold (CR<VectorLayout> that) {
	return CFat<VectorHolder> (VectorImplHolder () ,that) ;
}

class MatrixImplHolder final implement Fat<MatrixHolder ,MatrixLayout> {
public:
	void initialize (CR<Buffer<Flt64 ,ENUM<16>>> that) override {
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			Index ix = i.mY * 4 + i.mX ;
			self.mMatrix[mm (i.mX ,i.mY)] = that[ix] ;
		}
	}

	void initialize (CR<VectorLayout> x ,CR<VectorLayout> y ,CR<VectorLayout> z ,CR<VectorLayout> w) override {
		for (auto &&i : range (0 ,4)) {
			self.mMatrix[mm (0 ,i)] = x.mVector[i] ;
			self.mMatrix[mm (1 ,i)] = y.mVector[i] ;
			self.mMatrix[mm (2 ,i)] = z.mVector[i] ;
			self.mMatrix[mm (3 ,i)] = w.mVector[i] ;
		}
	}

	VR<Flt64> at (CR<Index> x ,CR<Index> y) leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return self.mMatrix[mm (x ,y)] ;
	}

	CR<Flt64> at (CR<Index> x ,CR<Index> y) const leftvalue override {
		assert (inline_between (x ,0 ,4)) ;
		assert (inline_between (y ,0 ,4)) ;
		return self.mMatrix[mm (x ,y)] ;
	}

	static Index mm (CR<Index> x ,CR<Index> y) {
		return x + y * 4 ;
	}

	Bool equal (CR<MatrixLayout> that) const override {
		for (auto &&i : range (0 ,16)) {
			const auto r1x = inline_equal (self.mMatrix[i] ,that.mMatrix[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	Flag compr (CR<MatrixLayout> that) const override {
		for (auto &&i : range (0 ,16)) {
			const auto r1x = inline_compr (self.mMatrix[i] ,that.mMatrix[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		for (auto &&i : range (0 ,16)) {
			inline_visit (visitor ,self.mMatrix[i]) ;
		}
		visitor.leave () ;
	}

	MatrixLayout sadd (CR<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] + that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout ssub (CR<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] - that.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout smul (CR<Flt64> that) const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] * that ;
		}
		return move (ret) ;
	}

	MatrixLayout sdiv (CR<Flt64> that) const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,16)) {
			ret.mMatrix[i] = self.mMatrix[i] / that ;
		}
		return move (ret) ;
	}

	VectorLayout smul (CR<VectorLayout> that) const override {
		VectorLayout ret ;
		for (auto &&i : range (0 ,4)) {
			auto rax = Flt64 (0) ;
			rax += self.mMatrix[mm (0 ,i)] * that.mVector[0] ;
			rax += self.mMatrix[mm (1 ,i)] * that.mVector[1] ;
			rax += self.mMatrix[mm (2 ,i)] * that.mVector[2] ;
			rax += self.mMatrix[mm (3 ,i)] * that.mVector[3] ;
			ret.mVector[i] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout smul (CR<MatrixLayout> that) const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			auto rax = Flt64 (0) ;
			rax += self.mMatrix[mm (0 ,i.mY)] * that.mMatrix[mm (i.mX ,0)] ;
			rax += self.mMatrix[mm (1 ,i.mY)] * that.mMatrix[mm (i.mX ,1)] ;
			rax += self.mMatrix[mm (2 ,i.mY)] * that.mMatrix[mm (i.mX ,2)] ;
			rax += self.mMatrix[mm (3 ,i.mY)] * that.mMatrix[mm (i.mX ,3)] ;
			ret.mMatrix[mm (i.mX ,i.mY)] = rax ;
		}
		return move (ret) ;
	}

	MatrixLayout sabs () const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,16)) {
			ret.mMatrix[i] = MathProc::abs (self.mMatrix[i]) ;
		}
		return move (ret) ;
	}

	MatrixLayout minus () const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,16)) {
			ret.mMatrix[i] = -self.mMatrix[i] ;
		}
		return move (ret) ;
	}

	MatrixLayout transpose () const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			ret.mMatrix[mm (i.mY ,i.mX)] = self.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	MatrixLayout triangular () const override {
		MatrixLayout ret = self ;
		for (auto &&i : range (0 ,4)) {
			if ifdo (TRUE) {
				Index ix = find_abs_max_row (ret ,i) ;
				if (ix == i)
					discard ;
				for (auto &&j : range (i ,4)) {
					const auto r1x = -ret.mMatrix[mm (j ,i)] ;
					ret.mMatrix[mm (j ,i)] = ret.mMatrix[mm (j ,ix)] ;
					ret.mMatrix[mm (j ,ix)] = r1x ;
				}
			}
			const auto r2x = MathProc::inverse (ret.mMatrix[mm (i ,i)]) ;
			if (r2x == 0)
				continue ;
			for (auto &&j : range (i + 1 ,4)) {
				const auto r3x = ret.mMatrix[mm (i ,j)] * r2x ;
				for (auto &&k : range (i + 1 ,4))
					ret.mMatrix[mm (k ,j)] -= r3x * ret.mMatrix[mm (k ,i)] ;
				ret.mMatrix[mm (i ,j)] = 0 ;
			}
		}
		return move (ret) ;
	}

	Index find_abs_max_row (CR<MatrixLayout> that ,CR<Index> x) const {
		Index ret = NONE ;
		auto rax = Flt64 () ;
		for (auto &&i : range (x ,4)) {
			const auto r1x = MathProc::abs (that.mMatrix[mm (x ,i)]) ;
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
		for (auto &&i : range (0 ,3 ,0 ,3)) {
			ret.mMatrix[mm (i.mX ,i.mY)] = self.mMatrix[mm (i.mX ,i.mY)] ;
		}
		return move (ret) ;
	}

	Flt64 determinant () const override {
		const auto r1x = triangular () ;
		Flt64 ret = 1 ;
		for (auto &&i : range (0 ,4))
			ret *= r1x.mMatrix[mm (i ,i)] ;
		return move (ret) ;
	}

	MatrixLayout adjoint () const override {
		MatrixLayout ret ;
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			Index ix = Length (i.mY == 0) ;
			Index iy = Length (i.mY == 1) + ix + 1 ;
			Index iz = Length (i.mY == 2) + iy + 1 ;
			Index jx = Length (i.mX == 0) ;
			Index jy = Length (i.mX == 1) + jx + 1 ;
			Index jz = Length (i.mX == 2) + jy + 1 ;
			const auto r1x = self.mMatrix[mm (jy ,iy)] * self.mMatrix[mm (jz ,iz)] - self.mMatrix[mm (jz ,iy)] * self.mMatrix[mm (jy ,iz)] ;
			const auto r2x = self.mMatrix[mm (jy ,ix)] * self.mMatrix[mm (jz ,iz)] - self.mMatrix[mm (jz ,ix)] * self.mMatrix[mm (jy ,iz)] ;
			const auto r3x = self.mMatrix[mm (jy ,ix)] * self.mMatrix[mm (jz ,iy)] - self.mMatrix[mm (jz ,ix)] * self.mMatrix[mm (jy ,iy)] ;
			const auto r4x = self.mMatrix[mm (jx ,ix)] * r1x ;
			const auto r5x = self.mMatrix[mm (jx ,iy)] * r2x ;
			const auto r6x = self.mMatrix[mm (jx ,iz)] * r3x ;
			const auto r7x = Flt64 (0.5 - (i.mY + i.mX) % 2) ;
			const auto r8x = (r4x - r5x + r6x) * MathProc::sign (r7x) ;
			ret.mMatrix[mm (i.mY ,i.mX)] = r8x ;
		}
		return move (ret) ;
	}

	MatrixLayout inverse () const override {
		const auto r1x = MathProc::inverse (determinant ()) ;
		assume (r1x != 0) ;
		MatrixLayout ret = adjoint () ;
		ret = MatrixHolder::hold (ret)->smul (r1x) ;
		return move (ret) ;
	}

	Flt64 trace () const override {
		Flt64 ret = 0 ;
		for (auto &&i : range (0 ,4)) {
			ret += self.mMatrix[mm (i ,i)] ;
		}
		return move (ret) ;
	}
} ;

exports VFat<MatrixHolder> MatrixHolder::hold (VR<MatrixLayout> that) {
	return VFat<MatrixHolder> (MatrixImplHolder () ,that) ;
}

exports CFat<MatrixHolder> MatrixHolder::hold (CR<MatrixLayout> that) {
	return CFat<MatrixHolder> (MatrixImplHolder () ,that) ;
}

class MakeMatrixImplHolder final implement Fat<MakeMatrixHolder ,MatrixLayout> {
public:
	void make_DiagMatrix (CR<Flt64> x ,CR<Flt64> y ,CR<Flt64> z ,CR<Flt64> w) override {
		Matrix ret = Matrix::zero () ;
		ret[0][0] = x ;
		ret[1][1] = y ;
		ret[2][2] = z ;
		ret[3][3] = w ;
		self = move (ret) ;
	}

	void make_ShearMatrix (CR<Vector> x ,CR<Vector> y ,CR<Vector> z) override {
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
		self = move (ret) ;
	}

	void make_RotationMatrix (CR<Vector> normal ,CR<Flt64> angle) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = normal.normalize () ;
		const auto r2x = angle * MathProc::step (r1x.magnitude () - 0.5) ;
		const auto r3x = MathProc::cos (r2x) ;
		const auto r4x = r1x * MathProc::sin (r2x) ;
		const auto r5x = r1x * (1 - r3x) ;
		ret[0][0] = r1x[0] * r5x[0] + r3x ;
		ret[1][0] = r1x[1] * r5x[0] + r4x[2] ;
		ret[2][0] = r1x[2] * r5x[0] - r4x[1] ;
		ret[0][1] = r1x[0] * r5x[1] - r4x[2] ;
		ret[1][1] = r1x[1] * r5x[1] + r3x ;
		ret[2][1] = r1x[2] * r5x[1] + r4x[0] ;
		ret[0][2] = r1x[0] * r5x[2] + r4x[1] ;
		ret[1][2] = r1x[1] * r5x[2] - r4x[0] ;
		ret[2][2] = r1x[2] * r5x[2] + r3x ;
		ret[3][3] = 1 ;
		self = move (ret) ;
	}

	void make_RotationMatrix (CR<Vector> from ,CR<Vector> into) override {
		const auto r1x = from.normalize () ;
		const auto r2x = into.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = MathProc::acos (r1x * r2x) ;
		make_RotationMatrix (r3x ,r4x) ;
	}

	void make_UnitaryMatrix (CR<Matrix> trs) override {
		const auto r1x = DiagMatrix (1 ,1 ,1 ,0) ;
		const auto r2x = trs * Vector::axis_x () ;
		const auto r3x = trs * Vector::axis_y () ;
		const auto r4x = trs * Vector::axis_z () ;
		const auto r5x = MathProc::inverse (r2x.magnitude ()) * MathProc::sign (r2x[0]) ;
		const auto r6x = MathProc::inverse (r3x.magnitude ()) * MathProc::sign (r3x[1]) ;
		const auto r7x = MathProc::inverse (r4x.magnitude ()) * MathProc::sign (r4x[2]) ;
		const auto r8x = DiagMatrix (r5x ,r6x ,r7x ,1) ;
		self = trs * r1x * r8x + Matrix::axis_w () ;
	}

	void make_TranslationMatrix (CR<Flt64> x ,CR<Flt64> y ,CR<Flt64> z) override {
		Matrix ret = Matrix::iden () ;
		ret[0][3] = x ;
		ret[1][3] = y ;
		ret[2][3] = z ;
		self = move (ret) ;
	}

	void make_PerspectiveMatrix (CR<Flt64> fovx ,CR<ImageShape> shape) override {
		assert (fovx > 0) ;
		const auto r1x = Flt64 (shape.mCX) * Flt64 (0.5) ;
		const auto r2x = Flt64 (shape.mCY) * Flt64 (0.5) ;
		const auto r3x = MathProc::tan (fovx * Flt64 (0.5)) ;
		const auto r4x = MathProc::inverse (r3x) * r1x ;
		const auto r5x = MathProc::inverse (r3x) * r2x ;
		make_PerspectiveMatrix (r4x ,r5x ,r1x ,r2x) ;
	}

	void make_PerspectiveMatrix (CR<Flt64> fx ,CR<Flt64> fy ,CR<Flt64> cx ,CR<Flt64> cy) override {
		assert (fx > 0) ;
		assert (fy > 0) ;
		Matrix ret = Matrix::zero () ;
		ret[0][0] = fx ;
		ret[1][1] = fy ;
		ret[0][2] = cx ;
		ret[1][2] = cy ;
		ret[3][2] = 1 ;
		ret[2][3] = 1 ;
		self = move (ret) ;
	}

	void make_ProjectionMatrix (CR<Vector> normal ,CR<Vector> center ,CR<Vector> light) override {
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
		self = move (ret) ;
	}

	void make_ViewMatrix (CR<Vector> x ,CR<Vector> y) override {
		const auto r1x = x.normalize () ;
		const auto r2x = y.normalize () ;
		const auto r3x = (r1x ^ r2x).normalize () ;
		const auto r4x = (r3x ^ r1x).normalize () ;
		const auto r5x = Vector::axis_w () ;
		self = Matrix (r1x ,r4x ,r3x ,r5x) ;
	}

	void make_ViewMatrix (CR<Vector> x ,CR<Vector> y ,CR<Just<ViewMatrixOption>> option) override {
		make_ViewMatrix (x ,y) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ViewMatrixOption::XYZ)
				discard ;
			const auto r1x = Vector::axis_x () ;
			const auto r2x = Vector::axis_y () ;
			const auto r3x = Vector::axis_z () ;
			const auto r4x = Vector::axis_w () ;
			const auto r5x = Matrix (r1x ,r2x ,r3x ,r4x) ;
			self = MatrixHolder::hold (self)->smul (r5x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::XZY)
				discard ;
			const auto r6x = Vector::axis_x () ;
			const auto r7x = -Vector::axis_z () ;
			const auto r8x = Vector::axis_y () ;
			const auto r9x = Vector::axis_w () ;
			const auto r10x = Matrix (r6x ,r7x ,r8x ,r9x) ;
			self = MatrixHolder::hold (self)->smul (r10x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::YXZ)
				discard ;
			const auto r11x = Vector::axis_y () ;
			const auto r12x = Vector::axis_x () ;
			const auto r13x = -Vector::axis_z () ;
			const auto r14x = Vector::axis_w () ;
			const auto r15x = Matrix (r11x ,r12x ,r13x ,r14x) ;
			self = MatrixHolder::hold (self)->smul (r15x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::YZX)
				discard ;
			const auto r16x = Vector::axis_z () ;
			const auto r17x = Vector::axis_x () ;
			const auto r18x = Vector::axis_y () ;
			const auto r19x = Vector::axis_w () ;
			const auto r20x = Matrix (r16x ,r17x ,r18x ,r19x) ;
			self = MatrixHolder::hold (self)->smul (r20x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::ZXY)
				discard ;
			const auto r21x = Vector::axis_y () ;
			const auto r22x = Vector::axis_z () ;
			const auto r23x = Vector::axis_x () ;
			const auto r24x = Vector::axis_w () ;
			const auto r25x = Matrix (r21x ,r22x ,r23x ,r24x) ;
			self = MatrixHolder::hold (self)->smul (r25x) ;
		}
		if ifdo (act) {
			if (option != ViewMatrixOption::ZYX)
				discard ;
			const auto r26x = -Vector::axis_z () ;
			const auto r27x = Vector::axis_y () ;
			const auto r28x = Vector::axis_x () ;
			const auto r29x = Vector::axis_w () ;
			const auto r30x = Matrix (r26x ,r27x ,r28x ,r29x) ;
			self = MatrixHolder::hold (self)->smul (r30x) ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	void make_CrossProductMatrix (CR<Vector> xyz) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = xyz.homogenize () ;
		ret[1][0] = r1x[2] ;
		ret[2][0] = -r1x[1] ;
		ret[0][1] = -r1x[2] ;
		ret[2][1] = r1x[0] ;
		ret[0][2] = r1x[1] ;
		ret[1][2] = -r1x[0] ;
		self = move (ret) ;
	}

	void make_OuterProductMatrix (CR<Vector> x ,CR<Vector> y) override {
		Matrix ret = Matrix::zero () ;
		const auto r1x = x ;
		const auto r2x = y ;
		for (auto &&i : range (0 ,4 ,0 ,4)) {
			ret[i] = r1x[i.mY] * r2x[i.mX] ;
		}
		self = move (ret) ;
	}

	void make_HomographyMatrix (CR<Flt64> d) override {
		Matrix ret = Matrix::zero () ;
		ret[0][0] = d ;
		ret[1][1] = d ;
		ret[2][2] = d ;
		ret[3][2] = 1 ;
		ret[3][3] = d ;
		self = move (ret) ;
	}

	void make_HomographyMatrix (CR<Vector> t ,CR<Vector> n ,CR<Vector> c) override {
		const auto r1x = (c - t) * n ;
		const auto r2x = OuterProductMatrix (t ,n) ;
		const auto r3x = r2x + Matrix::iden () * r1x ;
		self = r3x ;
	}
} ;

exports VFat<MakeMatrixHolder> MakeMatrixHolder::hold (VR<MatrixLayout> that) {
	return VFat<MakeMatrixHolder> (MakeMatrixImplHolder () ,that) ;
}

exports CFat<MakeMatrixHolder> MakeMatrixHolder::hold (CR<MatrixLayout> that) {
	return CFat<MakeMatrixHolder> (MakeMatrixImplHolder () ,that) ;
}

template class External<MatrixProcHolder ,MatrixProcLayout> ;

struct MatrixProcLayout {} ;

exports CR<Like<UniqueRef<MatrixProcLayout>>> MatrixProcHolder::expr_m () {
	return memorize ([&] () {
		Like<UniqueRef<MatrixProcLayout>> ret ;
		ret.mThis = UniqueRef<MatrixProcLayout>::make () ;
		MatrixProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<MatrixProcHolder> MatrixProcHolder::hold (VR<MatrixProcLayout> that) {
	return VFat<MatrixProcHolder> (External<MatrixProcHolder ,MatrixProcLayout>::expr ,that) ;
}

exports CFat<MatrixProcHolder> MatrixProcHolder::hold (CR<MatrixProcLayout> that) {
	return CFat<MatrixProcHolder> (External<MatrixProcHolder ,MatrixProcLayout>::expr ,that) ;
}

class DuplexMatrixImplHolder final implement Fat<DuplexMatrixHolder ,DuplexMatrixLayout> {
public:
	void initialize (CR<Matrix> that) override {
		self.mDuplexMatrix[0] = that ;
		self.mDuplexMatrix[1] = self.mDuplexMatrix[0].inverse () ;
		if ifdo (TRUE) {
			if (self.mDuplexMatrix[0][3][0] != 0)
				discard ;
			if (self.mDuplexMatrix[0][3][1] != 0)
				discard ;
			if (self.mDuplexMatrix[0][3][2] != 0)
				discard ;
			if (self.mDuplexMatrix[0][3][3] != 1)
				discard ;
			self.mDuplexMatrix[1][3][3] = 1 ;
		}
	}

	DuplexMatrixLayout inverse () const override {
		DuplexMatrixLayout ret ;
		ret.mDuplexMatrix[0] = self.mDuplexMatrix[1] ;
		ret.mDuplexMatrix[1] = self.mDuplexMatrix[0] ;
		return move (ret) ;
	}
} ;

exports VFat<DuplexMatrixHolder> DuplexMatrixHolder::hold (VR<DuplexMatrixLayout> that) {
	return VFat<DuplexMatrixHolder> (DuplexMatrixImplHolder () ,that) ;
}

exports CFat<DuplexMatrixHolder> DuplexMatrixHolder::hold (CR<DuplexMatrixLayout> that) {
	return CFat<DuplexMatrixHolder> (DuplexMatrixImplHolder () ,that) ;
}

class QuaternionImplHolder final implement Fat<QuaternionHolder ,QuaternionLayout> {
public:
	void initialize (CR<Flt64> w ,CR<Flt64> x ,CR<Flt64> y ,CR<Flt64> z) override {
		self.mQuaternion[1] = x ;
		self.mQuaternion[2] = y ;
		self.mQuaternion[3] = z ;
		self.mQuaternion[0] = w ;
		normalized () ;
	}

	void initialize (CR<Vector> that) override {
		const auto r1x = that.magnitude () ;
		const auto r2x = that.normalize () ;
		const auto r3x = MathProc::sin (r1x / 2) ;
		const auto r4x = MathProc::cos (r1x / 2) ;
		self.mQuaternion[1] = r2x[0] * r3x ;
		self.mQuaternion[2] = r2x[1] * r3x ;
		self.mQuaternion[3] = r2x[2] * r3x ;
		self.mQuaternion[0] = r4x ;
		normalized () ;
	}

	void initialize (CR<Matrix> that) override {
		const auto r1x = that.homogenize () ;
		auto act = TRUE ;
		if ifdo (act) {
			const auto r2x = 1 + r1x.trace () ;
			if (r2x < 1)
				discard ;
			const auto r3x = MathProc::sqrt (r2x) ;
			const auto r4x = MathProc::inverse (r3x) ;
			self.mQuaternion[1] = (r1x[2][1] - r1x[1][2]) * r4x ;
			self.mQuaternion[2] = (r1x[0][2] - r1x[2][0]) * r4x ;
			self.mQuaternion[3] = (r1x[1][0] - r1x[0][1]) * r4x ;
			self.mQuaternion[0] = r3x ;
		}
		if ifdo (act) {
			const auto r5x = 1 + r1x[0][0] - r1x[1][1] - r1x[2][2] ;
			if (r5x < 1)
				discard ;
			const auto r6x = MathProc::sqrt (r5x) ;
			const auto r7x = MathProc::inverse (r6x) ;
			self.mQuaternion[1] = r6x ;
			self.mQuaternion[2] = (r1x[1][0] + r1x[0][1]) * r7x ;
			self.mQuaternion[3] = (r1x[0][2] + r1x[2][0]) * r7x ;
			self.mQuaternion[0] = (r1x[2][1] - r1x[1][2]) * r7x ;
		}
		if ifdo (act) {
			const auto r8x = 1 - r1x[0][0] + r1x[1][1] - r1x[2][2] ;
			if (r8x < 1)
				discard ;
			const auto r9x = MathProc::sqrt (r8x) ;
			const auto r10x = MathProc::inverse (r9x) ;
			self.mQuaternion[1] = (r1x[1][0] + r1x[0][1]) * r10x ;
			self.mQuaternion[2] = r9x ;
			self.mQuaternion[3] = (r1x[2][1] + r1x[1][2]) * r10x ;
			self.mQuaternion[0] = (r1x[0][2] - r1x[2][0]) * r10x ;
		}
		if ifdo (act) {
			const auto r11x = 1 - r1x[0][0] - r1x[1][1] + r1x[2][2] ;
			if (r11x < 1)
				discard ;
			const auto r12x = MathProc::sqrt (r11x) ;
			const auto r13x = MathProc::inverse (r12x) ;
			self.mQuaternion[1] = (r1x[0][2] + r1x[2][0]) * r13x ;
			self.mQuaternion[2] = (r1x[2][1] + r1x[1][2]) * r13x ;
			self.mQuaternion[3] = r12x ;
			self.mQuaternion[0] = (r1x[1][0] - r1x[0][1]) * r13x ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
		normalized () ;
	}

	void initialize (CR<EulerAngle> that) override {
		const auto r1x = axis_of_index () ;
		const auto r2x = RotationMatrix (r1x[0] ,that.mPitch) ;
		const auto r3x = RotationMatrix (r1x[1] ,that.mYaw) ;
		const auto r4x = RotationMatrix (r1x[2] ,that.mRoll) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::XYZ)
				discard ;
			//@info: We are using intrinsic rotation
			//@info: First, rotation RXi = RX
			//@info: Second, rotation RYi = RXi * RY * RXi.inverse
			//@info: Third, rotation RZi = (RYi * RXi) * RZ * (RYi * RXi).inverse
			//@info: Finally, rotation RZi * RYi * RXi = RX * RY * RZ
			//@info: So, in the XYZ rotation order, RX is the first rotation, but RZ is applied last
			const auto r5x = r2x * r3x * r4x ;
			initialize (r5x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::XZY)
				discard ;
			const auto r6x = r2x * r4x * r3x ;
			initialize (r6x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::YXZ)
				discard ;
			const auto r7x = r3x * r2x * r4x ;
			initialize (r7x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::YZX)
				discard ;
			const auto r8x = r3x * r4x * r2x ;
			initialize (r8x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::ZXY)
				discard ;
			const auto r9x = r4x * r2x * r3x ;
			initialize (r9x) ;
		}
		if ifdo (act) {
			if (that.mType != ViewMatrixOption::ZYX)
				discard ;
			const auto r10x = r4x * r3x * r2x ;
			initialize (r10x) ;
		}
	}

	void normalized () {
		const auto r1x = MathProc::square (self.mQuaternion[1]) ;
		const auto r2x = MathProc::square (self.mQuaternion[2]) ;
		const auto r3x = MathProc::square (self.mQuaternion[3]) ;
		const auto r4x = MathProc::square (self.mQuaternion[0]) ;
		const auto r5x = MathProc::sqrt (r1x + r2x + r3x + r4x) ;
		const auto r6x = MathProc::inverse (r5x) ;
		self.mQuaternion[1] *= r6x ;
		self.mQuaternion[2] *= r6x ;
		self.mQuaternion[3] *= r6x ;
		self.mQuaternion[0] *= r6x ;
	}

	CR<Flt64> at (CR<Index> y) const leftvalue override {
		return self.mQuaternion[y] ;
	}

	Bool equal (CR<QuaternionLayout> that) const override {
		for (auto &&i : range (0 ,4)) {
			const auto r1x = inline_equal (self.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (!r1x)
				return r1x ;
		}
		return TRUE ;
	}

	Flag compr (CR<QuaternionLayout> that) const override {
		for (auto &&i : range (0 ,4)) {
			const auto r1x = inline_compr (self.mQuaternion[i] ,that.mQuaternion[i]) ;
			if (r1x != ZERO)
				return r1x ;
		}
		return ZERO ;
	}

	void visit (CR<Visitor> visitor) const override {
		visitor.enter () ;
		for (auto &&i : range (0 ,4)) {
			inline_visit (visitor ,self.mQuaternion[i]) ;
		}
		visitor.leave () ;
	}

	QuaternionLayout smul (CR<QuaternionLayout> that) const override {
		QuaternionLayout ret ;
		const auto r1x = axis (self) ;
		const auto r2x = axis (that) ;
		const auto r3x = self.mQuaternion[0] ;
		const auto r4x = that.mQuaternion[0] ;
		const auto r5x = r3x * r2x + r4x * r1x + (r1x ^ r2x) ;
		const auto r6x = r3x * r4x - r1x * r2x ;
		ret.mQuaternion[1] = r5x[0] ;
		ret.mQuaternion[2] = r5x[1] ;
		ret.mQuaternion[3] = r5x[2] ;
		ret.mQuaternion[0] = r6x ;
		return move (ret) ;
	}

	static Vector axis (CR<QuaternionLayout> q) {
		return Vector (q.mQuaternion[1] ,q.mQuaternion[2] ,q.mQuaternion[3] ,0) ;
	}

	Flt64 angle () const {
		const auto r1x = axis (self).magnitude () ;
		const auto r2x = self.mQuaternion[0] ;
		return MathProc::atan (r1x ,r2x) * 2 ;
	}

	Vector vector () const override {
		const auto r1x = axis (self).normalize () ;
		const auto r2x = angle () ;
		return r1x * r2x ;
	}

	Matrix matrix () const override {
		const auto r1x = axis (self).normalize () ;
		const auto r2x = angle () ;
		return RotationMatrix (r1x ,r2x) ;
	}

	EulerAngle euler (CR<Just<ViewMatrixOption>> type) const override {
		EulerAngle ret ;
		const auto r1x = matrix () ;
		const auto r2x = DiagMatrix (+1 ,-1 ,-1) ;
		const auto r3x = DiagMatrix (-1 ,+1 ,-1) ;
		const auto r4x = DiagMatrix (-1 ,-1 ,+1) ;
		const auto r5x = Buffer3<Matrix> ({r2x ,r3x ,r4x}) ;
		auto rax = Buffer3<Matrix> () ;
		auto act = TRUE ;
		if ifdo (act) {
			if (type != ViewMatrixOption::XYZ)
				discard ;
			rax = euler_decompose (r1x ,0 ,1 ,2) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[0] = rax[0] * r5x[0] ;
				rax[1] = r5x[0] * rax[1] ;
			}
			if ifdo (TRUE) {
				if (rax[1][1][1] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[2] ;
				rax[2] = r5x[2] * rax[2] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::XZY)
				discard ;
			rax = euler_decompose (r1x ,0 ,2 ,1) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[0] = rax[0] * r5x[0] ;
				rax[2] = r5x[0] * rax[2] ;
			}
			if ifdo (TRUE) {
				if (rax[2][2][2] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[1] ;
				rax[1] = r5x[1] * rax[1] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::YXZ)
				discard ;
			rax = euler_decompose (r1x ,1 ,0 ,2) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[1] ;
				rax[0] = r5x[1] * rax[0] ;
				rax[0] = rax[0] * r5x[2] ;
				rax[2] = r5x[2] * rax[2] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::YZX)
				discard ;
			rax = euler_decompose (r1x ,1 ,2 ,0) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[0] ;
				rax[0] = r5x[0] * rax[0] ;
			}
			if ifdo (TRUE) {
				if (rax[2][2][2] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[1] ;
				rax[2] = r5x[1] * rax[2] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::ZXY)
				discard ;
			rax = euler_decompose (r1x ,2 ,0 ,1) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[2] ;
				rax[0] = r5x[2] * rax[0] ;
				rax[0] = rax[0] * r5x[1] ;
				rax[1] = r5x[1] * rax[1] ;
			}
		}
		if ifdo (act) {
			if (type != ViewMatrixOption::ZYX)
				discard ;
			rax = euler_decompose (r1x ,2 ,1 ,0) ;
			if ifdo (TRUE) {
				if (rax[0][1][1] >= 0)
					discard ;
				rax[1] = rax[1] * r5x[0] ;
				rax[0] = r5x[0] * rax[0] ;
			}
			if ifdo (TRUE) {
				if (rax[1][1][1] >= 0)
					discard ;
				rax[2] = rax[2] * r5x[2] ;
				rax[1] = r5x[2] * rax[1] ;
			}
		}
		assert (MathProc::abs (rax[0][0][0] - 1) < FLT32_EPS) ;
		assert (MathProc::abs (rax[1][1][1] - 1) < FLT32_EPS) ;
		assert (MathProc::abs (rax[2][2][2] - 1) < FLT32_EPS) ;
		ret.mType = type ;
		ret.mPitch = MathProc::atan (rax[0][2][1] ,rax[0][1][1]) ;
		ret.mYaw = MathProc::atan (rax[1][0][2] ,rax[1][2][2]) ;
		ret.mRoll = MathProc::atan (rax[2][1][0] ,rax[2][0][0]) ;
		return move (ret) ;
	}

	Buffer3<Matrix> euler_decompose (CR<Matrix> rot ,CR<Index> x ,CR<Index> y ,CR<Index> z) const {
		Buffer3<Matrix> ret ;
		const auto r1x = axis_of_index () ;
		const auto r2x = rot * r1x[z] ;
		const auto r3x = rotate_angle (r2x ,r1x[x] ,r1x[y]) ;
		const auto r4x = -r3x.fetch () ;
		ret[x] = RotationMatrix (r1x[x] ,r4x) ;
		const auto r5x = ret[x].transpose () * rot ;
		const auto r6x = MathProc::atan (r5x[x][z] ,r5x[z][z]) * rotate_sign (y ,z) ;
		ret[y] = RotationMatrix (r1x[y] ,r6x) ;
		const auto r7x = ret[y].transpose () * r5x ;
		const auto r8x = MathProc::atan (r7x[y][x] ,r7x[x][x]) * rotate_sign (z ,x) ;
		ret[z] = RotationMatrix (r1x[z] ,r8x) ;
		const auto r9x = ret[z].transpose () * r7x - Matrix::iden () ;
		for (auto &&j : range (0 ,4 ,0 ,4)) {
			assert (MathProc::abs (r9x[j]) < FLT32_EPS) ;
		}
		return move (ret) ;
	}

	Buffer3<Vector> axis_of_index () const {
		Buffer3<Vector> ret ;
		ret[0] = Vector::axis_x () ;
		ret[1] = Vector::axis_y () ;
		ret[2] = Vector::axis_z () ;
		return move (ret) ;
	}

	Flt64 rotate_sign (CR<Index> y ,CR<Index> z) const {
		if (y == 0)
			if (z == 2)
				return -1 ;
		if (y == 1)
			if (z == 0)
				return -1 ;
		if (y == 2)
			if (z == 1)
				return -1 ;
		return +1 ;
	}

	Optional<Flt64> rotate_angle (CR<Vector> dir ,CR<Vector> rot_axis ,CR<Vector> normal) const {
		const auto r1x = CrossProductMatrix (rot_axis) ;
		const auto r2x = r1x * r1x ;
		const auto r3x = normal * (Matrix::iden () + r2x) * dir ;
		const auto r4x = normal * r1x * dir ;
		const auto r5x = -(normal * r2x * dir) ;
		const auto r6x = MathProc::inverse (MathProc::hypot (r4x ,r5x)) ;
		const auto r7x = r3x * r6x ;
		if (MathProc::abs (r7x) >= 1)
			return Optional<Flt64>::error (1) ;
		const auto r8x = r4x * r6x ;
		const auto r9x = r5x * r6x ;
		const auto r10x = MathProc::atan (r8x ,r9x) ;
		const auto r11x = MathProc::acos (-r7x) ;
		const auto r12x = r10x - r11x ;
		return ortho_angle (r12x) ;
	}

	Flt64 ortho_angle (CR<Flt64> x) const {
		const auto r1x = MathProc::floor (x ,MATH_PI * 2) ;
		const auto r2x = x - r1x - MATH_PI ;
		if (r2x <= -MATH_PI / 2)
			return r2x + MATH_PI ;
		if (r2x >= +MATH_PI / 2)
			return r2x - MATH_PI ;
		return r2x ;
	}
} ;

exports VFat<QuaternionHolder> QuaternionHolder::hold (VR<QuaternionLayout> that) {
	return VFat<QuaternionHolder> (QuaternionImplHolder () ,that) ;
}

exports CFat<QuaternionHolder> QuaternionHolder::hold (CR<QuaternionLayout> that) {
	return CFat<QuaternionHolder> (QuaternionImplHolder () ,that) ;
}

template class External<LinearProcHolder ,LinearProcLayout> ;

struct LinearProcLayout {} ;

exports CR<Like<UniqueRef<LinearProcLayout>>> LinearProcHolder::expr_m () {
	return memorize ([&] () {
		Like<UniqueRef<LinearProcLayout>> ret ;
		ret.mThis = UniqueRef<LinearProcLayout>::make () ;
		LinearProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<LinearProcHolder> LinearProcHolder::hold (VR<LinearProcLayout> that) {
	return VFat<LinearProcHolder> (External<LinearProcHolder ,LinearProcLayout>::expr ,that) ;
}

exports CFat<LinearProcHolder> LinearProcHolder::hold (CR<LinearProcLayout> that) {
	return CFat<LinearProcHolder> (External<LinearProcHolder ,LinearProcLayout>::expr ,that) ;
}

template class External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout> ;

struct KDTreeDataset ;
struct KDTreeKNNSearch ;

struct PointCloudKDTreeLayout {
	Ref<KDTreeDataset> mDataset ;
	Ref<KDTreeKNNSearch> mKNNSearch ;
} ;

exports AutoRef<PointCloudKDTreeLayout> PointCloudKDTreeHolder::create () {
	return AutoRef<PointCloudKDTreeLayout>::make () ;
}

exports VFat<PointCloudKDTreeHolder> PointCloudKDTreeHolder::hold (VR<PointCloudKDTreeLayout> that) {
	return VFat<PointCloudKDTreeHolder> (External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::expr ,that) ;
}

exports CFat<PointCloudKDTreeHolder> PointCloudKDTreeHolder::hold (CR<PointCloudKDTreeLayout> that) {
	return CFat<PointCloudKDTreeHolder> (External<PointCloudKDTreeHolder ,PointCloudKDTreeLayout>::expr ,that) ;
}

class PointCloudImplHolder final implement Fat<PointCloudHolder ,PointCloudLayout> {
public:
	void initialize (RR<Ref<Array<Point2F>>> pointcloud) override {
		self.mRank = 2 ;
		auto &&rax = keep[TYPE<Ref<Array<Point2F>>>::expr] (Pointer::from (self.mPointCloud)) ;
		rax = move (pointcloud) ;
		self.mWorld = Matrix::iden () ;
	}

	void initialize (RR<Ref<Array<Point3F>>> pointcloud) override {
		self.mRank = 3 ;
		auto &&rax = keep[TYPE<Ref<Array<Point3F>>>::expr] (Pointer::from (self.mPointCloud)) ;
		rax = move (pointcloud) ;
		self.mWorld = Matrix::iden () ;
	}

	void initialize (RR<Ref<Array<Vector>>> pointcloud) override {
		self.mRank = 0 ;
		auto &&rax = keep[TYPE<Ref<Array<Vector>>>::expr] (Pointer::from (self.mPointCloud)) ;
		rax = move (pointcloud) ;
		self.mWorld = Matrix::iden () ;
	}

	Length size () const override {
		return self.mPointCloud->size () ;
	}

	void get (CR<Index> index ,VR<Vector> item) const override {
		auto act = TRUE ;
		if ifdo (act) {
			if (self.mRank != 0)
				discard ;
			item = Vector (keep[TYPE<Vector>::expr] (self.mPointCloud.ref[index])) ;
			item = item * self.mWorld ;
		}
		if ifdo (act) {
			if (self.mRank != 2)
				discard ;
			item = Vector (keep[TYPE<Point2F>::expr] (self.mPointCloud.ref[index])) ;
			item = item * self.mWorld ;
		}
		if ifdo (act) {
			if (self.mRank != 3)
				discard ;
			item = Vector (keep[TYPE<Point3F>::expr] (self.mPointCloud.ref[index])) ;
			item = item * self.mWorld ;
		}
		if ifdo (act) {
			assert (FALSE) ;
		}
	}

	Vector pca_center () const {
		Vector ret = Vector::zero () ;
		auto rax = Vector () ;
		for (auto &&i : self.mPointCloud->iter ()) {
			get (i ,rax) ;
			ret += rax ;
		}
		ret = ret.projection () ;
		return move (ret) ;
	}

	Matrix pca_matrix () const override {
		const auto r1x = pca_center () ;
		const auto r2x = invoke ([&] () {
			Matrix ret = Matrix::zero () ;
			auto rax = Vector () ;
			for (auto &&i : self.mPointCloud->iter ()) {
				get (i ,rax) ;
				const auto r3x = rax - r1x ;
				ret[0][0] += MathProc::square (r3x[0]) ;
				ret[0][1] += r3x[0] * r3x[1] ;
				ret[0][2] += r3x[0] * r3x[2] ;
				ret[1][1] += MathProc::square (r3x[1]) ;
				ret[1][2] += r3x[1] * r3x[2] ;
				ret[2][2] += MathProc::square (r3x[2]) ;
			}
			ret[1][0] = ret[0][1] ;
			ret[2][0] = ret[0][2] ;
			ret[2][1] = ret[1][2] ;
			return move (ret) ;
		}) ;
		const auto r4x = MatrixProc::solve_svd (r2x) ;
		const auto r5x = TranslationMatrix (r1x) ;
		const auto r6x = UnitaryMatrix (r4x.mV) ;
		const auto r7x = DiagMatrix (sqrt_fix (r4x.mS[0][0]) ,sqrt_fix (r4x.mS[1][1]) ,sqrt_fix (r4x.mS[2][2])) ;
		return r5x * r6x * r7x ;
	}

	Flt64 sqrt_fix (CR<Flt64> a) const {
		if (a < FLT64_EPS)
			return 1 ;
		return MathProc::sqrt (a) ;
	}

	Vector box_center () const {
		Vector ret = Vector::zero () ;
		Index ix = (size () - 1) / 2 ;
		Index iy = size () / 2 ;
		auto rax = Vector () ;
		get (ix ,rax) ;
		ret += rax ;
		get (iy ,rax) ;
		ret += rax ;
		ret = ret.projection () ;
		return move (ret) ;
	}

	Matrix box_matrix () const override {
		const auto r1x = box_center () ;
		const auto r2x = bound () ;
		const auto r10x = (Vector (r2x.mMax) - r1x).sabs () ;
		const auto r11x = (Vector (r2x.mMin) - r1x).sabs () ;
		const auto r3x = MathProc::max_of (r10x[0] ,r11x[0] ,Flt64 (1)) ;
		const auto r4x = MathProc::max_of (r10x[1] ,r11x[1] ,Flt64 (1)) ;
		const auto r5x = MathProc::max_of (r10x[2] ,r11x[2] ,Flt64 (1)) ;
		const auto r7x = TranslationMatrix (r1x) ;
		const auto r8x = DiagMatrix (r3x ,r4x ,r5x) ;
		return r7x * r8x ;
	}

	Matrix box_matrix (CR<Flt64> ax ,CR<Flt64> ay ,CR<Flt64> az) const override {
		const auto r1x = box_center () ;
		const auto r2x = bound () ;
		const auto r3x = Vector (r2x.mMax) - Vector (r2x.mMin) ;
		const auto r4x = r3x[0] * MathProc::inverse (ax) ;
		const auto r5x = r3x[1] * MathProc::inverse (ay) ;
		const auto r6x = r3x[2] * MathProc::inverse (az) ;
		const auto r7x = MathProc::max_of (r4x ,r5x ,r6x) ;
		const auto r8x = TranslationMatrix (r1x) ;
		const auto r9x = DiagMatrix (ax * r7x ,ay * r7x ,az * r7x) ;
		return r8x * r9x ;
	}

	Line3F bound () const override {
		Line3F ret ;
		ret.mMin.mX = +infinity ;
		ret.mMin.mY = +infinity ;
		ret.mMin.mZ = +infinity ;
		ret.mMax.mX = -infinity ;
		ret.mMax.mY = -infinity ;
		ret.mMax.mZ = -infinity ;
		auto rax = Vector () ;
		for (auto &&i : self.mPointCloud->iter ()) {
			get (i ,rax) ;
			ret.mMin.mX = MathProc::min_of (ret.mMin.mX ,Flt32 (rax[0])) ;
			ret.mMin.mY = MathProc::min_of (ret.mMin.mY ,Flt32 (rax[1])) ;
			ret.mMin.mZ = MathProc::min_of (ret.mMin.mZ ,Flt32 (rax[2])) ;
			ret.mMax.mX = MathProc::max_of (ret.mMax.mX ,Flt32 (rax[0])) ;
			ret.mMax.mY = MathProc::max_of (ret.mMax.mY ,Flt32 (rax[1])) ;
			ret.mMax.mZ = MathProc::max_of (ret.mMax.mZ ,Flt32 (rax[2])) ;
		}
		return move (ret) ;
	}

	PointCloudLayout smul (CR<Matrix> that) const override {
		PointCloudLayout ret ;
		ret.mRank = self.mRank ;
		ret.mPointCloud = self.mPointCloud.share () ;
		ret.mWorld = self.mWorld * that ;
		return move (ret) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor) const override {
		if ifdo (TRUE) {
			if (self.mKDTree.mThis.exist ())
				discard ;
			const auto r1x = Pin<PointCloudKDTree> (self.mKDTree) ;
			PointCloudKDTreeHolder::hold (r1x.ref)->initialize (self.mPointCloud.ref) ;
		}
		return PointCloudKDTreeHolder::hold (self.mKDTree)->search (center ,neighbor) ;
	}

	Array<Index> search (CR<Vector> center ,CR<Length> neighbor ,CR<Flt64> radius) const override {
		if ifdo (TRUE) {
			if (self.mKDTree.mThis.exist ())
				discard ;
			const auto r1x = Pin<PointCloudKDTree> (self.mKDTree) ;
			PointCloudKDTreeHolder::hold (r1x.ref)->initialize (self.mPointCloud.ref) ;
		}
		return PointCloudKDTreeHolder::hold (self.mKDTree)->search (center ,neighbor ,radius) ;
	}
} ;

exports VFat<PointCloudHolder> PointCloudHolder::hold (VR<PointCloudLayout> that) {
	return VFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}

exports CFat<PointCloudHolder> PointCloudHolder::hold (CR<PointCloudLayout> that) {
	return CFat<PointCloudHolder> (PointCloudImplHolder () ,that) ;
}

class TPSFitImplHolder final implement Fat<TPSFitHolder ,TPSFitLayout> {
public:
	void compute (CR<Array<Vector>> dst ,CR<Array<Vector>> src) override {
		assert (dst.size () == src.size ()) ;
		const auto r1x = src.size () ;
		const auto r2x = PointCloud (Ref<Array<Vector>>::reference (src)) ;
		const auto r3x = PointCloud (Ref<Array<Vector>>::reference (dst)) ;
		self.mNSrc = r2x.pca_matrix () ;
		self.mNDst = r3x.pca_matrix () ;
		const auto r4x = self.mNSrc[1] * r2x ;
		const auto r5x = self.mNDst[1] * r3x ;
		self.mQA = Image<Flt64> (r1x + 4 ,r1x + 4) ;
		self.mQB = Image<Flt64> (3 ,r1x + 4) ;
		for (auto &&i : range (0 ,r1x ,0 ,r1x)) {
			if (i.mY > i.mX)
				continue ;
			const auto r6x = (r4x[i.mY] - r4x[i.mX]).magnitude () ;
			const auto r7x = basic_function (r6x) ;
			self.mQA.at (i.mX ,i.mY) = r7x ;
			self.mQA.at (i.mY ,i.mX) = r7x ;
		}
		for (auto &&i : range (0 ,r1x)) {
			const auto r8x = r4x[i] ;
			const auto r9x = r5x[i] ;
			for (auto &&j : range (0 ,4)) {
				self.mQA.at (r1x + j ,i) = r8x[j] ;
				self.mQA.at (i ,r1x + j) = r8x[j] ;
			}
			for (auto &&j : range (0 ,3)) {
				self.mQB.at (j ,i) = r9x[j] ;
			}
		}
		for (auto &&i : range (0 ,4)) {
			for (auto &&j : range (0 ,4)) {
				self.mQA.at (r1x + j ,r1x + i) = 0 ;
			}
			for (auto &&j : range (0 ,3)) {
				self.mQB.at (j ,r1x + i) = 0 ;
			}
		}
		self.mQC = LinearProc::solve_lsm (self.mQA ,self.mQB) ;
		self.mPSrc = Array<Vector> (r1x) ;
		for (auto &&i : self.mPSrc.iter ()) {
			self.mPSrc[i] = r4x[i] ;
		}
	}

	Vector smul (CR<Vector> that) const override {
		assert (self.mQC.size () > 0) ;
		Vector ret = Vector::axis_w () ;
		const auto r1x = self.mPSrc.length () ;
		const auto r2x = self.mNSrc[1] * that ;
		for (auto &&i : range (0 ,r1x)) {
			for (auto &&j : range (0 ,3)) {
				const auto r3x = (r2x - self.mPSrc[i]).magnitude () ;
				const auto r4x = basic_function (r3x) ;
				ret[j] += self.mQC.at (j ,i) * r4x ;
			}
		}
		for (auto &&i : range (0 ,4)) {
			for (auto &&j : range (0 ,3)) {
				ret[j] += self.mQC.at (j ,r1x + i) * r2x[i] ;
			}
		}
		ret = self.mNDst[0] * ret ;
		return move (ret) ;
	}

	Flt64 basic_function (CR<Flt64> r) const {
		return MathProc::square (r) * MathProc::log (r + FLT64_EPS) ;
	}
} ;

exports VFat<TPSFitHolder> TPSFitHolder::hold (VR<TPSFitLayout> that) {
	return VFat<TPSFitHolder> (TPSFitImplHolder () ,that) ;
}

exports CFat<TPSFitHolder> TPSFitHolder::hold (CR<TPSFitLayout> that) {
	return CFat<TPSFitHolder> (TPSFitImplHolder () ,that) ;
}
} ;