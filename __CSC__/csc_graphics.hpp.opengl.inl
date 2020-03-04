#pragma once

#ifndef __CSC_GRAPHICS__
#error "∑(っ°Д° ;)っ : require 'csc_graphics.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef switch_case
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039)
#endif
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :26440)
#endif
#include <GL/glew.h>
#ifdef FREEGLUT_LIB_PRAGMAS
#error "∑(っ°Д° ;)っ : dont use 'FREEGLUT_LIB_PRAGMAS'"
#endif
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut.h>
#undef FREEGLUT_LIB_PRAGMAS
#pragma warning (pop)
#pragma endregion
#endif

#ifndef __GLEW_H__
#error "∑(っ°Д° ;)っ : require 'GL/glew.h'"
#endif

#ifndef __FREEGLUT_H__
#error "∑(っ°Д° ;)っ : require 'GL/freeglut.h'"
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_glew32
#define use_comment_lib_glew32 "glew32.lib"
#endif
#ifndef use_comment_lib_freeglut
#define use_comment_lib_freeglut "freeglut.lib"
#endif
#ifndef use_comment_lib_glu32
#define use_comment_lib_glu32 "glu32.lib"
#endif
#ifndef use_comment_lib_opengl32
#define use_comment_lib_opengl32 "opengl32.lib"
#endif
#pragma comment (lib ,use_comment_lib_glew32)
#pragma comment (lib ,use_comment_lib_freeglut)
#pragma comment (lib ,use_comment_lib_glu32)
#pragma comment (lib ,use_comment_lib_opengl32)
#undef use_comment_lib_glew32
#undef use_comment_lib_freeglut
#undef use_comment_lib_glu32
#undef use_comment_lib_opengl32
#endif
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
class AbstractShader_Engine_OPENGL :public AbstractShader::Abstract {
private:
	//@warn: bind to (layout == 1) Vec3 in GLSL
	static constexpr auto LAYOUT_POSITION = CHAR (1) ;
	//@warn: bind to (layout == 2) Vec2 in GLSL
	static constexpr auto LAYOUT_TEXCOORD = CHAR (2) ;
	//@warn: bind to (layout == 3) Vec3 in GLSL
	static constexpr auto LAYOUT_NORMAL = CHAR (3) ;

	class Holder {
	private:
		friend AbstractShader_Engine_OPENGL ;
		UniqueRef<CHAR> mVAO ;
		UniqueRef<AutoBuffer<CHAR>> mVBO ;
		UniqueRef<AutoBuffer<CHAR>> mVTO ;
		LENGTH mSize ;
		FLAG mMode ;
		INDEX mTexture ;
	} ;

public:
	using NATIVE_TYPE = UniqueRef<CHAR> ;
	using SPRITE_NATIVE_TYPE = Holder ;

public:
	AbstractShader_Engine_OPENGL () {
		_STATIC_ASSERT_ (_SIZEOF_ (decltype ((*this))) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (decltype ((*this))) == _ALIGNOF_ (Interface)) ;
	}

	void compute_load_data (AnyRef<void> &this_ ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const override {
		_DEBUG_ASSERT_ (vs.size () < VAR32_MAX) ;
		_DEBUG_ASSERT_ (fs.size () < VAR32_MAX) ;
		auto tmp = UniqueRef<CHAR> ([&] (CHAR &me) {
			me = glCreateProgram () ;
			_DYNAMIC_ASSERT_ (me != 0) ;
			const auto r1x = glCreateShader (GL_VERTEX_SHADER) ;
			const auto r2x = _LOAD_<ARR<STRA>> (&vs.self) ;
			const auto r3x = VAR32 (vs.size ()) ;
			glShaderSource (r1x ,1 ,&r2x ,&r3x) ;
			glCompileShader (r1x) ;
			compute_check_shaderiv (r1x) ;
			glAttachShader (me ,r1x) ;
			const auto r4x = glCreateShader (GL_FRAGMENT_SHADER) ;
			const auto r5x = _LOAD_<ARR<STRA>> (&fs.self) ;
			const auto r6x = VAR32 (fs.size ()) ;
			glShaderSource (r4x ,1 ,&r5x ,&r6x) ;
			glCompileShader (r4x) ;
			compute_check_shaderiv (r4x) ;
			glAttachShader (me ,r4x) ;
			glLinkProgram (me) ;
			compute_check_programiv (me) ;
		} ,[] (CHAR &me) {
			glDeleteProgram (me) ;
		}) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_active_pipeline (AnyRef<void> &this_) const override {
		auto &r1y = this_.rebind<NATIVE_TYPE> ().self ;
		glUseProgram (r1y) ;
	}

	void compute_uniform_find (AnyRef<void> &this_ ,const String<STR> &name ,INDEX &index) const override {
		auto &r1y = this_.rebind<NATIVE_TYPE> ().self ;
		const auto r2x = identity_name (name) ;
		index = INDEX (glGetUniformLocation (r1y ,r2x.raw ().self)) ;
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const VAR32 &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		glUniform1i (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const VAR64 &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		glUniform1i64NV (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const VAL32 &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		glUniform1f (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const VAL64 &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		glUniform1d (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const Vector<VAL32> &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		const auto r1x = ARRAY4<VAL32> {data[0] ,data[1] ,data[2] ,data[3]} ;
		glUniform4fv (VAR32 (index) ,1 ,r1x.raw ().self) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const Vector<VAL64> &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		const auto r1x = ARRAY4<VAL64> {data[0] ,data[1] ,data[2] ,data[3]} ;
		glUniform4dv (VAR32 (index) ,1 ,r1x.raw ().self) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const Matrix<VAL32> &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		const auto r1x = ARRAY16<VAL32> ({
			data[0][0] ,data[0][1] ,data[0][2] ,data[0][3] ,
			data[1][0] ,data[1][1] ,data[1][2] ,data[1][3] ,
			data[2][0] ,data[2][1] ,data[2][2] ,data[2][3] ,
			data[3][0] ,data[3][1] ,data[3][2] ,data[3][3]}) ;
		glUniformMatrix4fv (VAR32 (index) ,1 ,GL_TRUE ,r1x.raw ().self) ;
	}

	void compute_uniform_write (AnyRef<void> &this_ ,INDEX index ,const Matrix<VAL64> &data) const override {
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
		const auto r1x = ARRAY16<VAL64> ({
			data[0][0] ,data[0][1] ,data[0][2] ,data[0][3] ,
			data[1][0] ,data[1][1] ,data[1][2] ,data[1][3] ,
			data[2][0] ,data[2][1] ,data[2][2] ,data[2][3] ,
			data[3][0] ,data[3][1] ,data[3][2] ,data[3][3]}) ;
		glUniformMatrix4dv (VAR32 (index) ,1 ,GL_TRUE ,r1x.raw ().self) ;
	}

	void compute_sprite_load_data (AnyRef<void> &this_ ,const Mesh &mesh) const override {
		auto tmp = Holder () ;
		tmp.mVAO = UniqueRef<CHAR> ([&] (CHAR &me) {
			glGenVertexArrays (1 ,&me) ;
			_DYNAMIC_ASSERT_ (me != GL_INVALID_VALUE) ;
		} ,[] (CHAR &me) {
			glDeleteVertexArrays (1 ,&me) ;
		}) ;
		tmp.mVBO = UniqueRef<AutoBuffer<CHAR>> ([&] (AutoBuffer<CHAR> &me) {
			me = AutoBuffer<CHAR> (1) ;
			glGenBuffers (VAR32 (me.size ()) ,me.self) ;
		} ,[] (AutoBuffer<CHAR> &me) {
			glDeleteBuffers (VAR32 (me.size ()) ,me.self) ;
		}) ;
		tmp.mVTO = UniqueRef<AutoBuffer<CHAR>> ([&] (AutoBuffer<CHAR> &me) {
			me = AutoBuffer<CHAR> (1) ;
			glGenTextures (VAR32 (me.size ()) ,me.self) ;
		} ,[] (AutoBuffer<CHAR> &me) {
			glDeleteTextures (VAR32 (me.size ()) ,me.self) ;
		}) ;
		compute_transfer_data (tmp ,bind_vertex (mesh.vertex () ,mesh.element ())) ;
		compute_transfer_data (tmp ,mesh.texture ()[0]) ;
		this_ = AnyRef<SPRITE_NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_sprite_active_texture (AnyRef<void> &this_ ,INDEX texture) const override {
		auto &r1y = this_.rebind<SPRITE_NATIVE_TYPE> ().self ;
		_DYNAMIC_ASSERT_ (texture >= 0 && texture < r1y.mVTO->size ()) ;
		r1y.mTexture = texture ;
	}

	void compute_sprite_draw (AnyRef<void> &this_) const override {
		auto &r1y = this_.rebind<SPRITE_NATIVE_TYPE> ().self ;
		glBindVertexArray (r1y.mVAO) ;
		if switch_case (TRUE) {
			if (r1y.mTexture == VAR_NONE)
				discard ;
			glActiveTexture (GL_TEXTURE_2D) ;
			glBindTexture (GL_TEXTURE_2D ,r1y.mVTO.self[r1y.mTexture]) ;
		}
		glDrawArrays (CHAR (r1y.mMode) ,0 ,VAR32 (r1y.mSize)) ;
	}

private:
	void compute_check_shaderiv (CHAR shader) const {
		auto rax = ARRAY2<VAR32> () ;
		rax[0] = GL_FALSE ;
		glGetShaderiv (shader ,GL_COMPILE_STATUS ,&rax[0]) ;
		if (rax[0] == GL_TRUE)
			return ;
		rax[1] = 0 ;
		glGetShaderiv (shader ,GL_INFO_LOG_LENGTH ,&rax[1]) ;
		if (rax[1] <= 0)
			return ;
		auto rbx = String<STRA> (rax[1]) ;
		glGetShaderInfoLog (shader ,VAR32 (rbx.size ()) ,NULL ,rbx.raw ().self) ;
		_DYNAMIC_ASSERT_ (rbx.empty ()) ;
	}

	void compute_check_programiv (CHAR shader) const {
		auto rax = ARRAY2<VAR32> () ;
		rax[0] = GL_FALSE ;
		glGetProgramiv (shader ,GL_LINK_STATUS ,&rax[0]) ;
		if (rax[0] == GL_TRUE)
			return ;
		rax[1] = 0 ;
		glGetProgramiv (shader ,GL_INFO_LOG_LENGTH ,&rax[1]) ;
		if (rax[1] <= 0)
			return ;
		auto rbx = String<STRA> (rax[1]) ;
		glGetProgramInfoLog (shader ,VAR32 (rbx.size ()) ,NULL ,rbx.raw ().self) ;
		_DYNAMIC_ASSERT_ (rbx.empty ()) ;
	}

	Array<ARRAY1<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const SList<ARRAY1<INDEX>> &element) const {
		Array<ARRAY1<ARRAY3<VAL32>>> ret = Array<ARRAY1<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	Array<ARRAY2<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const SList<ARRAY2<INDEX>> &element) const {
		Array<ARRAY2<ARRAY3<VAL32>>> ret = Array<ARRAY2<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
			ret[ix][1] = vertex[i[1]] ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	Array<ARRAY3<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const SList<ARRAY3<INDEX>> &element) const {
		Array<ARRAY3<ARRAY3<VAL32>>> ret = Array<ARRAY3<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
			ret[ix][1] = vertex[i[1]] ;
			ret[ix][2] = vertex[i[2]] ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	Array<ARRAY4<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const SList<ARRAY4<INDEX>> &element) const {
		Array<ARRAY4<ARRAY3<VAL32>>> ret = Array<ARRAY4<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
			ret[ix][1] = vertex[i[1]] ;
			ret[ix][2] = vertex[i[2]] ;
			ret[ix][3] = vertex[i[3]] ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY1<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_POINTS ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY2<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_LINES ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY2<ARRAY5<VAL32>>> &vbo) const {
		using VERTEX = ARRAY5<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_LINES ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		const auto r1x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + VAR_ZERO) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r2x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 3 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r2x) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY3<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_TRIANGLES ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY3<ARRAY5<VAL32>>> &vbo) const {
		using VERTEX = ARRAY5<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_TRIANGLES ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		const auto r1x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + VAR_ZERO) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r2x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 3 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r2x) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY3<ARRAY8<VAL32>>> &vbo) const {
		using VERTEX = ARRAY8<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_TRIANGLES ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		const auto r1x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + VAR_ZERO) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r2x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 3 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r2x) ;
		glEnableVertexAttribArray (LAYOUT_NORMAL) ;
		const auto r3x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 5 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_NORMAL ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r3x) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY4<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_QUADS ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY4<ARRAY5<VAL32>>> &vbo) const {
		using VERTEX = ARRAY5<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_QUADS ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		const auto r1x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + VAR_ZERO) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r2x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 3 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r2x) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Array<ARRAY4<ARRAY8<VAL32>>> &vbo) const {
		using VERTEX = ARRAY8<VAL32> ;
		self_.mSize = vbo.length () * vbo[0].length () ;
		self_.mMode = GL_QUADS ;
		glBindVertexArray (self_.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,self_.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,(self_.mSize * _SIZEOF_ (VERTEX)) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		const auto r1x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + VAR_ZERO) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r2x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 3 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r2x) ;
		glEnableVertexAttribArray (LAYOUT_NORMAL) ;
		const auto r3x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + 5 * _SIZEOF_ (VAL32)) ;
		glVertexAttribPointer (LAYOUT_NORMAL ,3 ,GL_FLOAT ,GL_FALSE ,_SIZEOF_ (VERTEX) ,r3x) ;
		glBindVertexArray (0) ;
	}

	void compute_transfer_data (Holder &self_ ,const Bitmap<COLOR_BGR> &image) const {
		self_.mTexture = 0 ;
		glBindVertexArray (self_.mVAO) ;
		glBindTexture (GL_TEXTURE_2D ,self_.mVTO.self[0]) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_WRAP_S ,GL_REPEAT) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_WRAP_T ,GL_REPEAT) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_MAG_FILTER ,GL_LINEAR) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_MIN_FILTER ,GL_LINEAR) ;
		glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,VAR32 (image.cx ()) ,VAR32 (image.cy ()) ,0 ,GL_BGR ,GL_UNSIGNED_BYTE ,image.raw ().self) ;
		glBindVertexArray (0) ;
	}

	inline String<STRA> identity_name (const String<STR> &name) const {
		String<STRA> ret = String<STRA> (name.length ()) ;
		for (auto &&i : _RANGE_ (0 ,ret.size ())) {
			const auto r1x = BOOL (name[i] >= STR ('a') && name[i] <= STR ('z')) ;
			const auto r2x = BOOL (name[i] >= STR ('A') && name[i] <= STR ('Z')) ;
			const auto r3x = BOOL (name[i] >= STR ('0') && name[i] <= STR ('9')) ;
			const auto r4x = BOOL (name[i] == STR ('_')) ;
			_DEBUG_ASSERT_ (r1x || r2x || r3x || r4x) ;
			ret[i] = STRA (name[i]) ;
		}
		return std::move (ret) ;
	}

	inline void compute_check_error (UniqueRef<CHAR> &self_) const {
		const auto r1x = glGetError () ;
		_DYNAMIC_ASSERT_ (r1x == GL_NO_ERROR) ;
	}
} ;
} ;