#pragma once

#ifndef __CSC_GRAPHICS__
#define __CSC_GRAPHICS__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_images.hpp"
#include "csc_geometry.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"

namespace CSC {
class Mesh {
private:
	Set<ARRAY3<VAL32>> mVertexSet ;
	SoftList<ARRAY3<INDEX>> mElementList ;
	Array<Bitmap<COLOR_BGR>> mTexture ;

public:
	Mesh () = default ;

	const Set<ARRAY3<VAL32>> &vertex () const leftvalue {
		return mVertexSet ;
	}

	const SoftList<ARRAY3<INDEX>> &element () const leftvalue {
		return mElementList ;
	}

	const Array<Bitmap<COLOR_BGR>> &texture () const leftvalue {
		return mTexture ;
	}

	void add_vertex (const Set<ARRAY3<VAL32>> &vertex_) {
		mVertexSet.appand (vertex_) ;
	}

	void add_element (const Deque<ARRAY3<INDEX>> &element_) {
		mElementList.appand (element_) ;
	}

	void add_texture (Bitmap<COLOR_BGR> &&texture_) {
		mTexture = Array<Bitmap<COLOR_BGR>> (1) ;
		mTexture[0] = _MOVE_ (texture_) ;
	}
} ;

template <class REAL>
class Camera {
private:
	Vector<REAL> mEyeU ;
	Vector<REAL> mEyeV ;
	Vector<REAL> mEyeN ;
	Vector<REAL> mEyeP ;
	Mutable<Matrix<REAL>> mViewMatrix ;
	REAL mScreenW ;
	REAL mScreenH ;
	REAL mScreenD ;
	Matrix<REAL> mProjectionMatrix ;

public:
	Camera () {
		const auto r1x = Vector<REAL> {REAL (0) ,REAL (0) ,REAL (1) ,REAL (1)} ;
		const auto r2x = Vector<REAL> {REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)} ;
		const auto r3x = Vector<REAL> {REAL (0) ,REAL (1) ,REAL (0) ,REAL (0)} ;
		lookat (r1x ,r2x ,r3x) ;
		perspective (REAL (90) ,REAL (1) ,REAL (1) ,REAL (1000)) ;
	}

	void lookat (const Vector<REAL> &eye ,const Vector<REAL> &center ,const Vector<REAL> &up) {
		_DEBUG_ASSERT_ (eye[3] == REAL (1)) ;
		_DEBUG_ASSERT_ (center[3] == REAL (1)) ;
		_DEBUG_ASSERT_ (up[3] == REAL (0)) ;
		mEyeN = (center - eye).normalize () ;
		mEyeU = (mEyeN ^ up).normalize () ;
		mEyeV = (mEyeN ^ mEyeU).normalize () ;
		mEyeP = eye ;
		mViewMatrix.signal () ;
	}

	void translate (const REAL &distance_u ,const REAL &distance_v ,const REAL &distance_n) {
		mEyeP += mEyeU * distance_u + mEyeV * distance_v + mEyeN * distance_n ;
		mViewMatrix.signal () ;
	}

	//@info: 'angle_vn-angle_nu-angle_uv' equals to 'pitch-yaw-roll' (heading-pitch-bank)
	void rotate (const REAL &angle_vn ,const REAL &angle_nu ,const REAL &angle_uv) {
		if switch_case (TRUE) {
			if (angle_vn == REAL (0))
				discard ;
			const auto r1x = mEyeN * MathProc::cos (angle_vn) - mEyeV * MathProc::sin (angle_vn) ;
			const auto r2x = mEyeV * MathProc::cos (angle_vn) + mEyeN * MathProc::sin (angle_vn) ;
			mEyeN = r1x.normalize () ;
			mEyeV = r2x.normalize () ;
		}
		if switch_case (TRUE) {
			if (angle_nu == REAL (0))
				discard ;
			const auto r3x = mEyeU * MathProc::cos (angle_nu) - mEyeN * MathProc::sin (angle_nu) ;
			const auto r4x = mEyeN * MathProc::cos (angle_nu) + mEyeU * MathProc::sin (angle_nu) ;
			mEyeU = r3x.normalize () ;
			mEyeN = r4x.normalize () ;
		}
		if switch_case (TRUE) {
			if (angle_uv == REAL (0))
				discard ;
			const auto r5x = mEyeV * MathProc::cos (angle_uv) - mEyeU * MathProc::sin (angle_uv) ;
			const auto r6x = mEyeU * MathProc::cos (angle_uv) + mEyeV * MathProc::sin (angle_uv) ;
			mEyeV = r5x.normalize () ;
			mEyeU = r6x.normalize () ;
		}
		mViewMatrix.signal () ;
	}

	void circle (const REAL &near_ ,const REAL &angle_un ,const REAL &angle_nv) {
		mEyeP += mEyeN * near_ ;
		rotate (angle_nv ,angle_un ,0) ;
		mEyeP -= mEyeN * near_ ;
	}

	Matrix<REAL> view_matrix () const {
		const auto r1x = Function<DEF<void (Matrix<REAL> &)> NONE::*> (PhanRef<const Camera>::make (DEREF[this]) ,&Camera::compute_view_matrix) ;
		mViewMatrix.apply (r1x) ;
		return mViewMatrix ;
	}

	REAL screen_w () const {
		return mScreenW ;
	}

	REAL screen_h () const {
		return mScreenH ;
	}

	REAL screen_d () const {
		return mScreenD ;
	}

	void perspective (const REAL &fov ,const REAL &aspect ,const REAL &near_ ,const REAL &far_) {
		_DEBUG_ASSERT_ (fov > REAL (0) && fov < REAL (180)) ;
		_DEBUG_ASSERT_ (aspect > REAL (0)) ;
		const auto r1x = near_ * MathProc::tan (fov * REAL (MATH_PI / 180) * REAL (0.5)) ;
		const auto r2x = r1x * aspect ;
		frustum (-r2x ,r2x ,-r1x ,r1x ,near_ ,far_) ;
	}

	void frustum (const REAL &left ,const REAL &right ,const REAL &bottom ,const REAL &top ,const REAL &near_ ,const REAL &far_) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (near_ > REAL (0) && near_ < far_) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = far_ - near_ ;
		mProjectionMatrix[0][0] = REAL (2) * near_ * MathProc::inverse (mScreenW) ;
		mProjectionMatrix[0][1] = REAL (0) ;
		mProjectionMatrix[0][2] = (right + left) * MathProc::inverse (mScreenW) ;
		mProjectionMatrix[0][3] = REAL (0) ;
		mProjectionMatrix[1][0] = REAL (0) ;
		mProjectionMatrix[1][1] = REAL (2) * near_ * MathProc::inverse (mScreenH) ;
		mProjectionMatrix[1][2] = (top + bottom) * MathProc::inverse (mScreenH) ;
		mProjectionMatrix[1][3] = REAL (0) ;
		mProjectionMatrix[2][0] = REAL (0) ;
		mProjectionMatrix[2][1] = REAL (0) ;
		mProjectionMatrix[2][2] = -(far_ + near_) * MathProc::inverse (mScreenD) ;
		mProjectionMatrix[2][3] = -(REAL (2) * near_ * far_) * MathProc::inverse (mScreenD) ;
		mProjectionMatrix[3][0] = REAL (0) ;
		mProjectionMatrix[3][1] = REAL (0) ;
		mProjectionMatrix[3][2] = REAL (-1) ;
		mProjectionMatrix[3][3] = REAL (0) ;
	}

	void ortho (const REAL &left ,const REAL &right ,const REAL &bottom ,const REAL &top ,const REAL &near_ ,const REAL &far_) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (near_ > REAL (0) && near_ < far_) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = far_ - near_ ;
		mProjectionMatrix[0][0] = REAL (2) * MathProc::inverse (mScreenW) ;
		mProjectionMatrix[0][1] = REAL (0) ;
		mProjectionMatrix[0][2] = REAL (0) ;
		mProjectionMatrix[0][3] = REAL (0) ;
		mProjectionMatrix[1][0] = REAL (0) ;
		mProjectionMatrix[1][1] = REAL (2) * MathProc::inverse (mScreenH) ;
		mProjectionMatrix[1][2] = REAL (0) ;
		mProjectionMatrix[1][3] = REAL (0) ;
		mProjectionMatrix[2][0] = REAL (0) ;
		mProjectionMatrix[2][1] = REAL (0) ;
		mProjectionMatrix[2][2] = REAL (-2) * MathProc::inverse (mScreenD) ;
		mProjectionMatrix[2][3] = REAL (0) ;
		mProjectionMatrix[3][0] = -(right + left) * MathProc::inverse (mScreenW) ;
		mProjectionMatrix[3][1] = -(top + bottom) * MathProc::inverse (mScreenH) ;
		mProjectionMatrix[3][2] = -(far_ + near_) * MathProc::inverse (mScreenD) ;
		mProjectionMatrix[3][3] = REAL (1) ;
	}

	Matrix<REAL> projection_matrix () const {
		return mProjectionMatrix ;
	}

private:
	void compute_view_matrix (Matrix<REAL> &view_mat) const {
		const auto r1x = Vector<REAL> {-mEyeP[0] ,-mEyeP[1] ,-mEyeP[2] ,REAL (0)} ;
		view_mat[0][0] = mEyeU[0] ;
		view_mat[0][1] = -mEyeU[1] ;
		view_mat[0][2] = -mEyeU[2] ;
		view_mat[0][3] = mEyeU * r1x ;
		view_mat[1][0] = mEyeV[0] ;
		view_mat[1][1] = -mEyeV[1] ;
		view_mat[1][2] = -mEyeV[2] ;
		view_mat[1][3] = mEyeV * r1x ;
		view_mat[2][0] = mEyeN[0] ;
		view_mat[2][1] = -mEyeN[1] ;
		view_mat[2][2] = -mEyeN[2] ;
		view_mat[2][3] = mEyeN * r1x - REAL (1) ;
		view_mat[3][0] = REAL (0) ;
		view_mat[3][1] = REAL (0) ;
		view_mat[3][2] = REAL (0) ;
		view_mat[3][3] = REAL (1) ;
	}
} ;

class AbstractSprite ;

class AbstractShader {
public:
	exports class Abstract
		:public Interface {
	public:
		virtual void compute_load_data (AnyRef<void> &holder ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const = 0 ;
		virtual void compute_active_pipeline (AnyRef<void> &holder) const = 0 ;
		virtual void compute_uniform_find (AnyRef<void> &holder ,const String<STR> &name ,INDEX &index) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const VAR32 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const VAR64 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const VAL32 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const VAL64 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const Vector<VAL32> &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const Vector<VAL64> &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const Matrix<VAL32> &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &holder ,const INDEX &index ,const Matrix<VAL64> &data) const = 0 ;
		virtual void compute_sprite_load_data (AnyRef<void> &holder ,const Mesh &mesh) const = 0 ;
		virtual void compute_sprite_active_texture (AnyRef<void> &holder ,const INDEX &texture) const = 0 ;
		virtual void compute_sprite_draw (AnyRef<void> &holder) const = 0 ;
	} ;

private:
	friend AbstractSprite ;
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;
	Set<String<STR>> mUniformMappingSet ;

public:
	AbstractShader () = default ;

	explicit AbstractShader (const PhanRef<const Abstract> &abstract_) {
		mAbstract = PhanRef<const Abstract>::make (abstract_) ;
	}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

	void load_data (const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		mAbstract->compute_load_data (mHolder ,vs ,fs) ;
	}

	void active_pipeline () {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->compute_active_pipeline (mHolder) ;
	}

	void uniform (const String<STR> &name ,const VAR32 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const VAR64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL32 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		INDEX ix = mUniformMappingSet.map (name) ;
		if switch_case (TRUE) {
			if (ix != VAR_NONE)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,ix) ;
			mUniformMappingSet.add (name ,ix) ;
		}
		mAbstract->compute_uniform_write (mHolder ,ix ,data) ;
	}

	template <class _RET = NONE>
	DEPENDENT_TYPE<AbstractSprite ,_RET> create_sprite () popping {
		struct Dependent ;
		return DEPENDENT_TYPE<AbstractSprite ,Dependent> (mAbstract) ;
	}
} ;

class AbstractSprite {
public:
	using Abstract = typename AbstractShader::Abstract ;

private:
	friend AbstractShader ;
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;

public:
	AbstractSprite () = delete ;

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

	void load_data (const Mesh &mesh) {
		_DEBUG_ASSERT_ (mAbstract.exist ()) ;
		mAbstract->compute_sprite_load_data (mHolder ,mesh) ;
	}

	void active_texture (const INDEX &texture) {
		if (!exist ())
			return ;
		mAbstract->compute_sprite_active_texture (mHolder ,texture) ;
	}

	void draw () {
		if (!exist ())
			return ;
		mAbstract->compute_sprite_draw (mHolder) ;
	}

private:
	explicit AbstractSprite (const PhanRef<const Abstract> &abstract_) {
		mAbstract = PhanRef<const Abstract>::make (abstract_) ;
	}
} ;
} ;