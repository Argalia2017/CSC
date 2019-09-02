#pragma once

#ifndef __CSC_GRAPHICS__
#define __CSC_GRAPHICS__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
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
	Deque<ARRAY3<INDEX>> mElementList ;
	Array<Bitmap<COLOR_BGR>> mTexture ;

public:
	Mesh () = default ;

	const Set<ARRAY3<VAL32>> &vertex () const & {
		return mVertexSet ;
	}

	const Set<ARRAY3<VAL32>> &vertex () && = delete ;

	const Deque<ARRAY3<INDEX>> &element () const & {
		return mElementList ;
	}

	const Deque<ARRAY3<INDEX>> &element () && = delete ;

	const Array<Bitmap<COLOR_BGR>> &texture () const & {
		return mTexture ;
	}

	const Array<Bitmap<COLOR_BGR>> &texture () && = delete ;

	void add_vertex (const Set<ARRAY3<VAL32>> &_vertex) {
		mVertexSet.appand (_vertex) ;
	}

	void add_element (const Deque<ARRAY3<INDEX>> &_element) {
		mElementList.appand (_element) ;
	}

	void add_texture (Bitmap<COLOR_BGR> &&_texture) {
		mTexture = Array<Bitmap<COLOR_BGR>> (1) ;
		mTexture[0] = std::move (_texture) ;
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
		const auto r1x = ARRAY3<Vector<REAL>> ({
			Vector<REAL> {REAL (0) ,REAL (0) ,REAL (1) ,REAL (1)} ,
			Vector<REAL> {REAL (0) ,REAL (0) ,REAL (0) ,REAL (1)} ,
			Vector<REAL> {REAL (0) ,REAL (1) ,REAL (0) ,REAL (0)}}) ;
		lookat (r1x[0] ,r1x[1] ,r1x[2]) ;
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
		for (FOR_ONCE_DO) {
			if (angle_vn == REAL (0))
				discard ;
			const auto r1x = mEyeN * _COS_ (angle_vn) - mEyeV * _SIN_ (angle_vn) ;
			const auto r2x = mEyeV * _COS_ (angle_vn) + mEyeN * _SIN_ (angle_vn) ;
			mEyeN = r1x.normalize () ;
			mEyeV = r2x.normalize () ;
		}
		for (FOR_ONCE_DO) {
			if (angle_nu == REAL (0))
				discard ;
			const auto r3x = mEyeU * _COS_ (angle_nu) - mEyeN * _SIN_ (angle_nu) ;
			const auto r4x = mEyeN * _COS_ (angle_nu) + mEyeU * _SIN_ (angle_nu) ;
			mEyeU = r3x.normalize () ;
			mEyeN = r4x.normalize () ;
		}
		for (FOR_ONCE_DO) {
			if (angle_uv == REAL (0))
				discard ;
			const auto r5x = mEyeV * _COS_ (angle_uv) - mEyeU * _SIN_ (angle_uv) ;
			const auto r6x = mEyeU * _COS_ (angle_uv) + mEyeV * _SIN_ (angle_uv) ;
			mEyeV = r5x.normalize () ;
			mEyeU = r6x.normalize () ;
		}
		mViewMatrix.signal () ;
	}

	void circle (const REAL &_near ,const REAL &angle_un ,const REAL &angle_nv) {
		mEyeP += mEyeN * _near ;
		rotate (angle_nv ,angle_un ,0) ;
		mEyeP -= mEyeN * _near ;
	}

	Matrix<REAL> view_matrix () const {
		const auto r1x = Function<DEF<void (Matrix<REAL> &)> NONE::*> (PhanRef<const Camera>::make ((*this)) ,&Camera::compute_view_matrix) ;
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

	void perspective (const REAL &fov ,const REAL &aspect ,const REAL &_near ,const REAL &_far) {
		_DEBUG_ASSERT_ (fov > REAL (0) && fov < REAL (180)) ;
		_DEBUG_ASSERT_ (aspect > REAL (0)) ;
		const auto r1x = _near * _TAN_ (fov * REAL (MATH_PI / 180) * REAL (0.5)) ;
		const auto r2x = r1x * aspect ;
		frustum (-r2x ,r2x ,-r1x ,r1x ,_near ,_far) ;
	}

	void frustum (const REAL &left ,const REAL &right ,const REAL &bottom ,const REAL &top ,const REAL &_near ,const REAL &_far) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (_near > REAL (0) && _near < _far) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = _far - _near ;
		mProjectionMatrix[0][0] = REAL (2) * _near * _PINV_ (mScreenW) ;
		mProjectionMatrix[0][1] = REAL (0) ;
		mProjectionMatrix[0][2] = (right + left) * _PINV_ (mScreenW) ;
		mProjectionMatrix[0][3] = REAL (0) ;
		mProjectionMatrix[1][0] = REAL (0) ;
		mProjectionMatrix[1][1] = REAL (2) * _near * _PINV_ (mScreenH) ;
		mProjectionMatrix[1][2] = (top + bottom) * _PINV_ (mScreenH) ;
		mProjectionMatrix[1][3] = REAL (0) ;
		mProjectionMatrix[2][0] = REAL (0) ;
		mProjectionMatrix[2][1] = REAL (0) ;
		mProjectionMatrix[2][2] = -(_far + _near) * _PINV_ (mScreenD) ;
		mProjectionMatrix[2][3] = -(REAL (2) * _near * _far) * _PINV_ (mScreenD) ;
		mProjectionMatrix[3][0] = REAL (0) ;
		mProjectionMatrix[3][1] = REAL (0) ;
		mProjectionMatrix[3][2] = REAL (-1) ;
		mProjectionMatrix[3][3] = REAL (0) ;
	}

	void ortho (const REAL &left ,const REAL &right ,const REAL &bottom ,const REAL &top ,const REAL &_near ,const REAL &_far) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (_near > REAL (0) && _near < _far) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = _far - _near ;
		mProjectionMatrix[0][0] = REAL (2) * _PINV_ (mScreenW) ;
		mProjectionMatrix[0][1] = REAL (0) ;
		mProjectionMatrix[0][2] = REAL (0) ;
		mProjectionMatrix[0][3] = REAL (0) ;
		mProjectionMatrix[1][0] = REAL (0) ;
		mProjectionMatrix[1][1] = REAL (2) * _PINV_ (mScreenH) ;
		mProjectionMatrix[1][2] = REAL (0) ;
		mProjectionMatrix[1][3] = REAL (0) ;
		mProjectionMatrix[2][0] = REAL (0) ;
		mProjectionMatrix[2][1] = REAL (0) ;
		mProjectionMatrix[2][2] = REAL (-2) * _PINV_ (mScreenD) ;
		mProjectionMatrix[2][3] = REAL (0) ;
		mProjectionMatrix[3][0] = -(right + left) * _PINV_ (mScreenW) ;
		mProjectionMatrix[3][1] = -(top + bottom) * _PINV_ (mScreenH) ;
		mProjectionMatrix[3][2] = -(_far + _near) * _PINV_ (mScreenD) ;
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

class AbstractShader {
public:
	exports struct Abstract :public Interface {
		virtual void compute_load_data (AnyRef<void> &_this ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const = 0 ;
		virtual void compute_active_pipeline (AnyRef<void> &_this) const = 0 ;
		virtual void compute_uniform_find (AnyRef<void> &_this ,const String<STR> &name ,INDEX &index) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAR32 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAR64 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAL32 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAL64 &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Vector<VAL32> &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Vector<VAL64> &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Matrix<VAL32> &data) const = 0 ;
		virtual void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Matrix<VAL64> &data) const = 0 ;
		virtual void compute_sprite_load_data (AnyRef<void> &_this ,const Mesh &mesh) const = 0 ;
		virtual void compute_sprite_active_texture (AnyRef<void> &_this ,INDEX texture) const = 0 ;
		virtual void compute_sprite_draw (AnyRef<void> &_this) const = 0 ;
	} ;

	class Sprite ;

private:
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;
	Set<String<STR> ,INDEX> mUniformSet ;

public:
	AbstractShader () = default ;

	explicit AbstractShader (const PhanRef<const Abstract> &_abstract) :mAbstract (PhanRef<const Abstract>::make (_abstract)) {}

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
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAR64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL32 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO) {
			if (mUniformSet.length () == r1x)
				discard ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	Sprite create_sprite () popping ;
} ;

class AbstractShader::Sprite {
private:
	friend AbstractShader ;
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;

public:
	Sprite () = delete ;

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

	void active_texture (INDEX texture) {
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
	explicit Sprite (const PhanRef<const Abstract> &_abstract) :mAbstract (PhanRef<const Abstract>::make (_abstract)) {}
} ;

inline AbstractShader::Sprite AbstractShader::create_sprite () popping {
	return Sprite (mAbstract) ;
}
} ;