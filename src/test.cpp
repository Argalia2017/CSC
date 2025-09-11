#include "util.h"

using namespace SOLUTION ;

struct TPSFitLayout {
	DuplexMatrix mNSrc ;
	DuplexMatrix mNDst ;
	Array<Vector> mPSrc ;
	Image<FLT64> mQA ;
	Image<FLT64> mQB ;
	Image<FLT64> mQC ;
} ;

struct TPSFitHolder implement Interface {
	imports VFat<TPSFitHolder> hold (VR<TPSFitLayout> that) ;
	imports CFat<TPSFitHolder> hold (CR<TPSFitLayout> that) ;

	virtual void compute (CR<Array<Vector>> dst ,CR<Array<Vector>> src) = 0 ;
	virtual Vector smul (CR<Vector> point) const = 0 ;
} ;

class TPSFit implement TPSFitLayout {
protected:
	using TPSFitLayout::mNSrc ;
	using TPSFitLayout::mNDst ;
	using TPSFitLayout::mPSrc ;
	using TPSFitLayout::mQA ;
	using TPSFitLayout::mQB ;
	using TPSFitLayout::mQC ;

public:
	implicit TPSFit () = default ;

	void compute (CR<Array<Vector>> dst ,CR<Array<Vector>> src) {
		TPSFitHolder::hold (thiz)->compute (dst ,src) ;
	}

	Vector smul (CR<Vector> that) const {
		return TPSFitHolder::hold (thiz)->smul (that) ;
	}

	forceinline Vector operator() (CR<Vector> that) const {
		return smul (that) ;
	}

	forceinline friend Vector operator* (CR<TPSFit> thiz_ ,CR<Vector> that) {
		return thiz_.smul (that) ;
	}
} ;

class TPSFitImplHolder final implement Fat<TPSFitHolder ,TPSFitLayout> {
public:
	void compute (CR<Array<Vector>> dst ,CR<Array<Vector>> src) {
		assert (dst.size () == src.size ()) ;
		const auto r1x = src.size () ;
		const auto r2x = PointCloud (Ref<Array<Vector>>::reference (src)) ;
		const auto r3x = PointCloud (Ref<Array<Vector>>::reference (dst)) ;
		self.mNSrc = r2x.pca_matrix () ;
		self.mNDst = r3x.pca_matrix () ;
		const auto r4x = self.mNSrc[1] * r2x ;
		const auto r5x = self.mNDst[1] * r3x ;
		self.mQA = Image<FLT64> (r1x + 4 ,r1x + 4) ;
		self.mQB = Image<FLT64> (3 ,r1x + 4) ;
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

	Vector smul (CR<Vector> that) const {
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

	FLT64 basic_function (CR<FLT64> r) const {
		return MathProc::square (r) * MathProc::log (r + FLT64_EPS) ;
	}
} ;

exports VFat<TPSFitHolder> TPSFitHolder::hold (VR<TPSFitLayout> that) {
	return VFat<TPSFitHolder> (TPSFitImplHolder () ,that) ;
}

exports CFat<TPSFitHolder> TPSFitHolder::hold (CR<TPSFitLayout> that) {
	return CFat<TPSFitHolder> (TPSFitImplHolder () ,that) ;
}

int test () {
	const auto r1x = CurrentRandom () ;
	const auto r2x = invoke ([&] () {
		Array<Vector> ret = Array<Vector> (10) ;
		for (auto &&i : ret.iter ()) {
			const auto r3x = r1x.random_float (10000) * 0.01 ;
			ret[i] = Vector (FLT64 (i) + r3x ,0 ,0 ,1) ;
		}
		return move (ret) ;
	}) ;
	const auto r4x = invoke ([&] () {
		Array<Vector> ret = Array<Vector> (10) ;
		for (auto &&i : ret.iter ()) {
			const auto r5x = r1x.random_float (10000) * 0.01 ;
			ret[i] = Vector (FLT64 (i) + r5x ,2 ,1 ,1) ;
		}
		return move (ret) ;
	}) ;
	auto rax = TPSFit () ;
	rax.compute (r4x ,r2x) ;
	auto rbx = NormalError () ;
	for (auto &&i : r2x.iter ()) {
		const auto r6x = rax * r2x[i] ;
		const auto r7x = (r4x[i] - r6x).magnitude () ;
		rbx += r7x ;
	}
	Singleton<Console>::expr.debug (slice ("mMaxError = ") ,rbx.mMaxError) ;
	Singleton<Console>::expr.debug (slice ("mAvgError = ") ,rbx.mAvgError) ;
	Singleton<Console>::expr.debug (slice ("mStdError = ") ,rbx.mStdError) ;
	return 0 ;
}