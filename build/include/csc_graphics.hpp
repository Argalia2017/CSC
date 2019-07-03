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
	Queue<ARRAY3<INDEX>> mElementList ;
	Array<Bitmap<COLOR_BGR>> mTexture ;

public:
	Mesh () = default ;

	const Set<ARRAY3<VAL32>> &vertex () const & {
		return mVertexSet ;
	}

	const Set<ARRAY3<VAL32>> &vertex () && = delete ;

	const Queue<ARRAY3<INDEX>> &element () const & {
		return mElementList ;
	}

	const Queue<ARRAY3<INDEX>> &element () && = delete ;

	const Array<Bitmap<COLOR_BGR>> &texture () const & {
		return mTexture ;
	}

	const Array<Bitmap<COLOR_BGR>> &texture () && = delete ;

	void add_vertex (const Set<ARRAY3<VAL32>> &_vertex) {
		mVertexSet.appand (_vertex) ;
	}

	void add_element (const Queue<ARRAY3<INDEX>> &_element) {
		mElementList.appand (_element) ;
	}

	void add_texture (Bitmap<COLOR_BGR> &&_texture) {
		mTexture = Array<Bitmap<COLOR_BGR>> (1) ;
		mTexture[0] = std::move (_texture) ;
	}
} ;

template <class UNIT>
class Camera {
private:
	Vector<UNIT> mEyeU ;
	Vector<UNIT> mEyeV ;
	Vector<UNIT> mEyeN ;
	Vector<UNIT> mEyeP ;
	Mutable<Matrix<UNIT>> mViewMatrix ;
	UNIT mScreenW ;
	UNIT mScreenH ;
	UNIT mScreenD ;
	Matrix<UNIT> mProjectionMatrix ;

public:
	Camera () {
		const auto r1x = ARRAY3<Vector<UNIT>> ({
			Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (1) ,UNIT (1)} ,
			Vector<UNIT> {UNIT (0) ,UNIT (0) ,UNIT (0) ,UNIT (1)} ,
			Vector<UNIT> {UNIT (0) ,UNIT (1) ,UNIT (0) ,UNIT (0)}}) ;
		lookat (r1x[0] ,r1x[1] ,r1x[2]) ;
		perspective (UNIT (90) ,UNIT (1) ,UNIT (1) ,UNIT (1000)) ;
	}

	void lookat (const Vector<UNIT> &eye ,const Vector<UNIT> &center ,const Vector<UNIT> &up) {
		_DEBUG_ASSERT_ (eye[3] == UNIT (1) && center[3] == UNIT (1) && up[3] == UNIT (0)) ;
		mEyeN = (center - eye).normalize () ;
		mEyeU = (mEyeN ^ up).normalize () ;
		mEyeV = (mEyeN ^ mEyeU).normalize () ;
		mEyeP = eye ;
		mViewMatrix.signal () ;
	}

	void translate (const UNIT &distance_u ,const UNIT &distance_v ,const UNIT &distance_n) {
		mEyeP += mEyeU * distance_u + mEyeV * distance_v + mEyeN * distance_n ;
		mViewMatrix.signal () ;
	}

	//@info: 'angle_vn-angle_nu-angle_uv' equals to 'pitch-yaw-roll' (heading-pitch-bank)
	void rotate (const UNIT &angle_vn ,const UNIT &angle_nu ,const UNIT &angle_uv) {
		for (FOR_ONCE_DO_WHILE) {
			if (angle_vn == UNIT (0))
				continue ;
			const auto r1x = mEyeN * _COS_ (angle_vn) - mEyeV * _SIN_ (angle_vn) ;
			const auto r2x = mEyeV * _COS_ (angle_vn) + mEyeN * _SIN_ (angle_vn) ;
			mEyeN = r1x.normalize () ;
			mEyeV = r2x.normalize () ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (angle_nu == UNIT (0))
				continue ;
			const auto r3x = mEyeU * _COS_ (angle_nu) - mEyeN * _SIN_ (angle_nu) ;
			const auto r4x = mEyeN * _COS_ (angle_nu) + mEyeU * _SIN_ (angle_nu) ;
			mEyeU = r3x.normalize () ;
			mEyeN = r4x.normalize () ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (angle_uv == UNIT (0))
				continue ;
			const auto r5x = mEyeV * _COS_ (angle_uv) - mEyeU * _SIN_ (angle_uv) ;
			const auto r6x = mEyeU * _COS_ (angle_uv) + mEyeV * _SIN_ (angle_uv) ;
			mEyeV = r5x.normalize () ;
			mEyeU = r6x.normalize () ;
		}
		mViewMatrix.signal () ;
	}

	void circle (const UNIT &_near ,const UNIT &angle_un ,const UNIT &angle_nv) {
		mEyeP += mEyeN * _near ;
		rotate (angle_nv ,angle_un ,0) ;
		mEyeP -= mEyeN * _near ;
	}

	Matrix<UNIT> view () const {
		const auto r1x = Function<DEF<void (Matrix<UNIT> &)> NONE::*> (PhanRef<const Camera>::make (*this) ,&Camera::compute_view_matrix) ;
		mViewMatrix.apply (r1x) ;
		return mViewMatrix ;
	}

	UNIT screen_w () const {
		return mScreenW ;
	}

	UNIT screen_h () const {
		return mScreenH ;
	}

	UNIT screen_d () const {
		return mScreenD ;
	}

	void perspective (const UNIT &fov ,const UNIT &aspect ,const UNIT &_near ,const UNIT &_far) {
		_DEBUG_ASSERT_ (fov > UNIT (0) && fov < UNIT (180)) ;
		_DEBUG_ASSERT_ (aspect > UNIT (0)) ;
		const auto r1x = _near * _TAN_ (fov * UNIT (MATH_PI / 180) * UNIT (0.5)) ;
		const auto r2x = r1x * aspect ;
		frustum (-r2x ,r2x ,-r1x ,r1x ,_near ,_far) ;
	}

	void frustum (const UNIT &left ,const UNIT &right ,const UNIT &bottom ,const UNIT &top ,const UNIT &_near ,const UNIT &_far) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (_far > _near && _near > UNIT (0)) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = _far - _near ;
		mProjectionMatrix[0][0] = UNIT (2) * _near * _PINV_ (mScreenW) ;
		mProjectionMatrix[0][1] = UNIT (0) ;
		mProjectionMatrix[0][2] = (right + left) * _PINV_ (mScreenW) ;
		mProjectionMatrix[0][3] = UNIT (0) ;
		mProjectionMatrix[1][0] = UNIT (0) ;
		mProjectionMatrix[1][1] = UNIT (2) * _near * _PINV_ (mScreenH) ;
		mProjectionMatrix[1][2] = (top + bottom) * _PINV_ (mScreenH) ;
		mProjectionMatrix[1][3] = UNIT (0) ;
		mProjectionMatrix[2][0] = UNIT (0) ;
		mProjectionMatrix[2][1] = UNIT (0) ;
		mProjectionMatrix[2][2] = -(_far + _near) * _PINV_ (mScreenD) ;
		mProjectionMatrix[2][3] = -(UNIT (2) * _near * _far) * _PINV_ (mScreenD) ;
		mProjectionMatrix[3][0] = UNIT (0) ;
		mProjectionMatrix[3][1] = UNIT (0) ;
		mProjectionMatrix[3][2] = UNIT (-1) ;
		mProjectionMatrix[3][3] = UNIT (0) ;
	}

	void ortho (const UNIT &left ,const UNIT &right ,const UNIT &bottom ,const UNIT &top ,const UNIT &_near ,const UNIT &_far) {
		_DEBUG_ASSERT_ (right > left) ;
		_DEBUG_ASSERT_ (top > bottom) ;
		_DEBUG_ASSERT_ (_far > _near && _near > UNIT (0)) ;
		mScreenW = right - left ;
		mScreenH = top - bottom ;
		mScreenD = _far - _near ;
		mProjectionMatrix[0][0] = UNIT (2) * _PINV_ (mScreenW) ;
		mProjectionMatrix[0][1] = UNIT (0) ;
		mProjectionMatrix[0][2] = UNIT (0) ;
		mProjectionMatrix[0][3] = UNIT (0) ;
		mProjectionMatrix[1][0] = UNIT (0) ;
		mProjectionMatrix[1][1] = UNIT (2) * _PINV_ (mScreenH) ;
		mProjectionMatrix[1][2] = UNIT (0) ;
		mProjectionMatrix[1][3] = UNIT (0) ;
		mProjectionMatrix[2][0] = UNIT (0) ;
		mProjectionMatrix[2][1] = UNIT (0) ;
		mProjectionMatrix[2][2] = UNIT (-2) * _PINV_ (mScreenD) ;
		mProjectionMatrix[2][3] = UNIT (0) ;
		mProjectionMatrix[3][0] = -(right + left) * _PINV_ (mScreenW) ;
		mProjectionMatrix[3][1] = -(top + bottom) * _PINV_ (mScreenH) ;
		mProjectionMatrix[3][2] = -(_far + _near) * _PINV_ (mScreenD) ;
		mProjectionMatrix[3][3] = UNIT (1) ;
	}

	Matrix<UNIT> projection () const {
		return mProjectionMatrix ;
	}

private:
	void compute_view_matrix (Matrix<UNIT> &view_mat) const {
		const auto r1x = Vector<UNIT> {-mEyeP[0] ,-mEyeP[1] ,-mEyeP[2] ,UNIT (0)} ;
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
		view_mat[2][3] = mEyeN * r1x - UNIT (1) ;
		view_mat[3][0] = UNIT (0) ;
		view_mat[3][1] = UNIT (0) ;
		view_mat[3][2] = UNIT (0) ;
		view_mat[3][3] = UNIT (1) ;
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
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAR64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL32 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
			mAbstract->compute_uniform_find (mHolder ,name ,mUniformSet[ix].item) ;
		}
		mAbstract->compute_uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		for (FOR_ONCE_DO_WHILE) {
			if (mUniformSet.length () == r1x)
				continue ;
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