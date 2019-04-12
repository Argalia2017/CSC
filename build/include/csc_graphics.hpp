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
	Array<SoftImage<COLOR_BGR>> mTexture ;

public:
	Mesh () = default ;

	const Set<ARRAY3<VAL32>> &vertex () const {
		return mVertexSet ;
	}

	const Queue<ARRAY3<INDEX>> &element () const {
		return mElementList ;
	}

	const Array<SoftImage<COLOR_BGR>> &texture () const {
		return mTexture ;
	}

	void add_vertex (const Set<ARRAY3<VAL32>> &vertex) {
		mVertexSet.appand (vertex) ;
	}

	void add_element (const Queue<ARRAY3<INDEX>> &element) {
		mElementList.appand (element) ;
	}

	void add_texture (SoftImage<COLOR_BGR> &&texture) {
		mTexture = Array<SoftImage<COLOR_BGR>> (1) ;
		mTexture[0] = std::move (texture) ;
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

	const Vector<UNIT> &eye_u () const {
		return mEyeU ;
	}

	const Vector<UNIT> &eye_v () const {
		return mEyeV ;
	}

	const Vector<UNIT> &eye_n () const {
		return mEyeN ;
	}

	const Vector<UNIT> &eye_p () const {
		return mEyeP ;
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

	//@info: angle_vn-angle_nu-angle_uv <<==>> pitch-yaw-roll (heading-pitch-bank)
	void rotate (const UNIT &angle_vn ,const UNIT &angle_nu ,const UNIT &angle_uv) {
		if (angle_vn != UNIT (0)) {
			const auto r1x = mEyeN * _COS_ (angle_vn) - mEyeV * _SIN_ (angle_vn) ;
			const auto r2x = mEyeV * _COS_ (angle_vn) + mEyeN * _SIN_ (angle_vn) ;
			mEyeN = r1x.normalize () ;
			mEyeV = r2x.normalize () ;
		}
		if (angle_nu != UNIT (0)) {
			const auto r1x = mEyeU * _COS_ (angle_nu) - mEyeN * _SIN_ (angle_nu) ;
			const auto r2x = mEyeN * _COS_ (angle_nu) + mEyeU * _SIN_ (angle_nu) ;
			mEyeU = r1x.normalize () ;
			mEyeN = r2x.normalize () ;
		}
		if (angle_uv != UNIT (0)) {
			const auto r1x = mEyeV * _COS_ (angle_uv) - mEyeU * _SIN_ (angle_uv) ;
			const auto r2x = mEyeU * _COS_ (angle_uv) + mEyeV * _SIN_ (angle_uv) ;
			mEyeV = r1x.normalize () ;
			mEyeU = r2x.normalize () ;
		}
		mViewMatrix.signal () ;
	}

	void circle (const UNIT &_near ,const UNIT &angle_un ,const UNIT &angle_nv) {
		mEyeP += mEyeN * _near ;
		rotate (angle_nv ,angle_un ,0) ;
		mEyeP -= mEyeN * _near ;
	}

	const Matrix<UNIT> &view () const {
		mViewMatrix.apply (Function<DEF<void (Matrix<UNIT> &)> NONE::*> (PhanRef<const Camera>::make (*this) ,&Camera::compute_view_matrix)) ;
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
		const auto r1x = _near * _TAN_ (fov * UNIT (VALX_PI / 180) * UNIT (0.5)) ;
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
		mProjectionMatrix[0][0] = UNIT (2) * _near / mScreenW ;
		mProjectionMatrix[0][1] = UNIT (0) ;
		mProjectionMatrix[0][2] = (right + left) / mScreenW ;
		mProjectionMatrix[0][3] = UNIT (0) ;
		mProjectionMatrix[1][0] = UNIT (0) ;
		mProjectionMatrix[1][1] = UNIT (2) * _near / mScreenH ;
		mProjectionMatrix[1][2] = (top + bottom) / mScreenH ;
		mProjectionMatrix[1][3] = UNIT (0) ;
		mProjectionMatrix[2][0] = UNIT (0) ;
		mProjectionMatrix[2][1] = UNIT (0) ;
		mProjectionMatrix[2][2] = -(_far + _near) / mScreenD ;
		mProjectionMatrix[2][3] = -(UNIT (2) * _near * _far) / mScreenD ;
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
		mProjectionMatrix[0][0] = UNIT (2) / mScreenW ;
		mProjectionMatrix[0][1] = UNIT (0) ;
		mProjectionMatrix[0][2] = UNIT (0) ;
		mProjectionMatrix[0][3] = UNIT (0) ;
		mProjectionMatrix[1][0] = UNIT (0) ;
		mProjectionMatrix[1][1] = UNIT (2) / mScreenH ;
		mProjectionMatrix[1][2] = UNIT (0) ;
		mProjectionMatrix[1][3] = UNIT (0) ;
		mProjectionMatrix[2][0] = UNIT (0) ;
		mProjectionMatrix[2][1] = UNIT (0) ;
		mProjectionMatrix[2][2] = UNIT (-2) / mScreenD ;
		mProjectionMatrix[2][3] = UNIT (0) ;
		mProjectionMatrix[3][0] = -(right + left) / mScreenW ;
		mProjectionMatrix[3][1] = -(top + bottom) / mScreenH ;
		mProjectionMatrix[3][2] = -(_far + _near) / mScreenD ;
		mProjectionMatrix[3][3] = UNIT (1) ;
	}

	const Matrix<UNIT> &projection () const {
		return mProjectionMatrix ;
	}

private:
	void compute_view_matrix (Matrix<UNIT> &me) const {
		const auto r1x = Vector<UNIT> {-mEyeP[0] ,-mEyeP[1] ,-mEyeP[2] ,UNIT (0)} ;
		me[0][0] = mEyeU[0] ;
		me[0][1] = -mEyeU[1] ;
		me[0][2] = -mEyeU[2] ;
		me[0][3] = mEyeU * r1x ;
		me[1][0] = mEyeV[0] ;
		me[1][1] = -mEyeV[1] ;
		me[1][2] = -mEyeV[2] ;
		me[1][3] = mEyeV * r1x ;
		me[2][0] = mEyeN[0] ;
		me[2][1] = -mEyeN[1] ;
		me[2][2] = -mEyeN[2] ;
		me[2][3] = mEyeN * r1x - UNIT (1) ;
		me[3][0] = UNIT (0) ;
		me[3][1] = UNIT (0) ;
		me[3][2] = UNIT (0) ;
		me[3][3] = UNIT (1) ;
	}
} ;

class AbstractShader {
public:
	class Sprite ;

	export struct Abstract :public Interface {
		virtual void load_data (AnyRef<void> &_this ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const = 0 ;
		virtual void active (AnyRef<void> &_this) const = 0 ;
		virtual INDEX uniform_find (const AnyRef<void> &_this ,const String<STR> &name) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAR32 &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAR64 &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAL32 &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAL64 &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Vector<VAL32> &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Vector<VAL64> &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Matrix<VAL32> &data) const = 0 ;
		virtual void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Matrix<VAL64> &data) const = 0 ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;
	Set<String<STR> ,INDEX> mUniformSet ;

public:
	AbstractShader () = default ;

	explicit AbstractShader (const PhanRef<const Abstract> &engine) :mAbstract (PhanRef<const Abstract>::make (engine)) {}

	BOOL exist () const {
		return mAbstract.exist () && mHolder.exist () ;
	}

	void load_data (const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) {
		mAbstract->load_data (mHolder ,vs ,fs) ;
	}

	void active () {
		_DEBUG_ASSERT_ (exist ()) ;
		mAbstract->active (mHolder) ;
	}

	void uniform (const String<STR> &name ,const VAR32 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAR64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL32 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const VAL64 &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Vector<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL32> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}

	void uniform (const String<STR> &name ,const Matrix<VAL64> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = mUniformSet.length () ;
		INDEX ix = mUniformSet.insert (name) ;
		if (mUniformSet.length () > r1x)
			mUniformSet[ix].item = mAbstract->uniform_find (mHolder ,name) ;
		mAbstract->uniform_write (mHolder ,mUniformSet[ix].item ,data) ;
	}
} ;

class AbstractShader::Sprite {
public:
	export struct Abstract :public Interface {
		virtual void load_data (AnyRef<void> &_this ,const Mesh &mesh) const = 0 ;
		virtual void use_texture (AnyRef<void> &_this ,INDEX texture) const = 0 ;
		virtual void draw (const AnyRef<void> &_this) const = 0 ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;

public:
	Sprite () = default ;

	explicit Sprite (const PhanRef<const Abstract> &engine) :mAbstract (PhanRef<const Abstract>::make (engine)) {}

	BOOL exist () const {
		return mAbstract.exist () && mHolder.exist () ;
	}

	void load_data (const Mesh &mesh) {
		mAbstract->load_data (mHolder ,mesh) ;
	}

	void use_texture (INDEX texture) {
		if (!exist ())
			return ;
		mAbstract->use_texture (mHolder ,texture) ;
	}

	void draw () {
		if (!exist ())
			return ;
		mAbstract->draw (mHolder) ;
	}
} ;
} ;