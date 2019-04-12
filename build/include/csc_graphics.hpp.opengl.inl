#pragma once

#ifndef __CSC_GRAPHICS__
#error "∑(っ°Д° ;)っ : require 'csc_graphics.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef import
#undef export
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("import")
#pragma pop_macro ("export")
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
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("import")
#pragma push_macro ("export")
#define self to ()
#define implicit
#define popping
#define import extern
#define export
#endif

namespace CSC {
namespace U {
template <class>
struct OPENGL_TRAITS ;

template <>
struct OPENGL_TRAITS<BYTE> {
	using TYPE = ARGC<GL_UNSIGNED_BYTE> ;
} ;

template <>
struct OPENGL_TRAITS<WORD> {
	using TYPE = ARGC<GL_UNSIGNED_SHORT> ;
} ;

template <>
struct OPENGL_TRAITS<CHAR> {
	using TYPE = ARGC<GL_UNSIGNED_INT> ;
} ;

template <>
struct OPENGL_TRAITS<VAR32> {
	using TYPE = ARGC<GL_INT> ;
} ;

template <>
struct OPENGL_TRAITS<VAL32> {
	using TYPE = ARGC<GL_FLOAT> ;
} ;

template <>
struct OPENGL_TRAITS<VAL64> {
	using TYPE = ARGC<GL_DOUBLE> ;
} ;
} ;

template <class _ARG>
using OPENGL_TRAITS_TYPE = typename U::OPENGL_TRAITS<_ARG>::TYPE ;

class AbstractShader_Engine_OPENGL :public AbstractShader::Abstract {
public:
	AbstractShader_Engine_OPENGL () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void load_data (AnyRef<void> &_this ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const override {
		auto rax = UniqueRef<CHAR> ([&] (CHAR &me) {
			me = glCreateProgram () ;
			_DYNAMIC_ASSERT_ (me != 0) ;
			const auto r1x = glCreateShader (GL_VERTEX_SHADER) ;
			const auto r2x = _LOAD_<ARR<STRA>> (vs.self) ;
			const auto r3x = VAR32 (vs.size ()) ;
			glShaderSource (r1x ,1 ,&r2x ,&r3x) ;
			glCompileShader (r1x) ;
			attach_shaderiv (r1x) ;
			glAttachShader (me ,r1x) ;
			const auto r4x = glCreateShader (GL_FRAGMENT_SHADER) ;
			const auto r5x = _LOAD_<ARR<STRA>> (fs.self) ;
			const auto r6x = VAR32 (fs.size ()) ;
			glShaderSource (r4x ,1 ,&r5x ,&r6x) ;
			glCompileShader (r4x) ;
			attach_shaderiv (r4x) ;
			glAttachShader (me ,r4x) ;
			glLinkProgram (me) ;
			attach_programiv (me) ;
		} ,[] (CHAR &me) {
			_DEBUG_ASSERT_ (me != 0) ;
			glDeleteProgram (me) ;
		}) ;
		_this = AnyRef<UniqueRef<CHAR>>::make (std::move (rax)) ;
	}

	void active (AnyRef<void> &_this) const override {
		auto &r1 = _this.rebind<UniqueRef<CHAR>> ().self ;
		glUseProgram (r1) ;
	}

	INDEX uniform_find (const AnyRef<void> &_this ,const String<STR> &name) const override {
		auto &r1 = _this.rebind<UniqueRef<CHAR>> ().self ;
		const auto r1x = _CALL_ ([&] () {
			String<STRA> ret = String<STRA> (name.length ()) ;
			for (INDEX i = 0 ; i < ret.size () ; i++) {
				_DEBUG_ASSERT_ (VAR32 (name[i]) >= 32 && VAR32 (name[i]) <= 126) ;
				ret[i] = STRA (name[i]) ;
			}
			return std::move (ret) ;
		}) ;
		INDEX ret = INDEX (glGetUniformLocation (r1 ,r1x.raw ().self)) ;
		_DEBUG_ASSERT_ (ret != GL_INVALID_VALUE) ;
		return std::move (ret) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAR32 &data) const override {
		glUniform1i (VAR32 (index) ,data) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAR64 &data) const override {
		glUniform1i64NV (VAR32 (index) ,data) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAL32 &data) const override {
		glUniform1f (VAR32 (index) ,data) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const VAL64 &data) const override {
		glUniform1d (VAR32 (index) ,data) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Vector<VAL32> &data) const override {
		glUniform3fv (VAR32 (index) ,1 ,data.raw ().self) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Vector<VAL64> &data) const override {
		glUniform3dv (VAR32 (index) ,1 ,data.raw ().self) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Matrix<VAL32> &data) const override {
		glUniformMatrix4fv (VAR32 (index) ,1 ,GL_TRUE ,data.raw ().self) ;
	}

	void uniform_write (const AnyRef<void> &_this ,INDEX index ,const Matrix<VAL64> &data) const override {
		glUniformMatrix4dv (VAR32 (index) ,1 ,GL_TRUE ,data.raw ().self) ;
	}

private:
	void attach_shaderiv (CHAR shader) const {
		auto rax = VAR32 () ;
		glGetShaderiv (shader ,GL_COMPILE_STATUS ,&(rax = GL_FALSE)) ;
		if (rax == GL_FALSE) {
			glGetShaderiv (shader ,GL_INFO_LOG_LENGTH ,&(rax = 0)) ;
		} else {
			rax = 0 ;
		}
		auto rbx = String<STRA> (rax) ;
		if (rbx.size () > 0)
			glGetShaderInfoLog (shader ,VAR32 (rbx.size ()) ,NULL ,rbx.raw ().self) ;
		_DEBUG_ASSERT_ (rbx.empty ()) ;
	}

	void attach_programiv (CHAR shader) const {
		auto rax = VAR32 () ;
		glGetProgramiv (shader ,GL_LINK_STATUS ,&(rax = GL_FALSE)) ;
		if (rax == GL_FALSE) {
			glGetProgramiv (shader ,GL_INFO_LOG_LENGTH ,&(rax = 0)) ;
		} else {
			rax = 0 ;
		}
		auto rbx = String<STRA> (rax) ;
		if (rbx.size () > 0)
			glGetProgramInfoLog (shader ,VAR32 (rbx.size ()) ,NULL ,rbx.raw ().self) ;
		_DEBUG_ASSERT_ (rbx.empty ()) ;
	}

private:
	static void debug_check_error (UniqueRef<CHAR> &_self) {
		const auto r1x = glGetError () ;
		_DYNAMIC_ASSERT_ (r1x == GL_NO_ERROR) ;
		(void) r1x ;
	}
} ;

class AbstractShader_Sprite_Engine_OPENGL :public AbstractShader::Sprite::Abstract {
private:
	//@warn: bind to (layout == 1) Vec3 in GLSL
	static constexpr auto LAYOUT_POSITION = CHAR (1) ;
	//@warn: bind to (layout == 2) Vec2 in GLSL
	static constexpr auto LAYOUT_TEXCOORD = CHAR (2) ;
	//@warn: bind to (layout == 3) Vec3 in GLSL
	static constexpr auto LAYOUT_NORMAL = CHAR (3) ;

	class Pack {
	private:
		friend AbstractShader_Sprite_Engine_OPENGL ;
		UniqueRef<CHAR> mVAO ;
		UniqueRef<AutoBuffer<CHAR>> mVBO ;
		UniqueRef<AutoBuffer<CHAR>> mVTO ;
		LENGTH mSize ;
		FLAG mMode ;
		INDEX mTexture ;
	} ;

public:
	AbstractShader_Sprite_Engine_OPENGL () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void load_data (AnyRef<void> &_this ,const Mesh &mesh) const override {
		auto rax = Pack () ;
		rax.mVAO = UniqueRef<CHAR> ([&] (CHAR &me) {
			glGenVertexArrays (1 ,&me) ;
			_DYNAMIC_ASSERT_ (me != GL_INVALID_VALUE) ;
		} ,[] (CHAR &me) {
			_DEBUG_ASSERT_ (me != GL_INVALID_VALUE) ;
			glDeleteVertexArrays (1 ,&me) ;

		}) ;
		rax.mVBO = UniqueRef<AutoBuffer<CHAR>> ([&] (AutoBuffer<CHAR> &me) {
			me = AutoBuffer<CHAR> (1) ;
			glGenBuffers (VAR32 (me.size ()) ,me.self) ;
		} ,[] (AutoBuffer<CHAR> &me) {
			_DEBUG_ASSERT_ (me.self != NULL) ;
			glDeleteBuffers (VAR32 (me.size ()) ,me.self) ;
		}) ;
		rax.mVTO = UniqueRef<AutoBuffer<CHAR>> ([&] (AutoBuffer<CHAR> &me) {
			me = AutoBuffer<CHAR> (1) ;
			glGenTextures (VAR32 (me.size ()) ,me.self) ;
		} ,[] (AutoBuffer<CHAR> &me) {
			_DEBUG_ASSERT_ (me.self != NULL) ;
			glDeleteTextures (VAR32 (me.size ()) ,me.self) ;
		}) ;
		attach_transfer (rax ,bind_vertex (mesh.vertex () ,mesh.element ())) ;
		attach_transfer (rax ,mesh.texture ()[0]) ;
		_this = AnyRef<Pack>::make (std::move (rax)) ;
	}

	void use_texture (AnyRef<void> &_this ,INDEX texture) const override {
		auto &r1 = _this.rebind<Pack> ().self ;
		_DYNAMIC_ASSERT_ (texture >= 0 && texture < r1.mVTO->size ()) ;
		r1.mTexture = texture ;
	}

	void draw (const AnyRef<void> &_this) const override {
		auto &r1 = _this.rebind<Pack> ().self ;
		glBindVertexArray (r1.mVAO) ;
		if (r1.mTexture != VAR_NONE) {
			glActiveTexture (GL_TEXTURE_2D) ;
			glBindTexture (GL_TEXTURE_2D ,r1.mVTO.self[r1.mTexture]) ;
		}
		glDrawArrays (CHAR (r1.mMode) ,0 ,VAR32 (r1.mSize)) ;
	}

private:
	Array<ARRAY1<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const Queue<ARRAY1<INDEX>> &element) const {
		Array<ARRAY1<ARRAY3<VAL32>>> ret = Array<ARRAY1<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
		}
		return std::move (ret) ;
	}

	Array<ARRAY2<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const Queue<ARRAY2<INDEX>> &element) const {
		Array<ARRAY2<ARRAY3<VAL32>>> ret = Array<ARRAY2<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
			ret[ix][1] = vertex[i[1]] ;
		}
		return std::move (ret) ;
	}

	Array<ARRAY3<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const Queue<ARRAY3<INDEX>> &element) const {
		Array<ARRAY3<ARRAY3<VAL32>>> ret = Array<ARRAY3<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
			ret[ix][1] = vertex[i[1]] ;
			ret[ix][2] = vertex[i[2]] ;
		}
		return std::move (ret) ;
	}

	Array<ARRAY4<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const Queue<ARRAY4<INDEX>> &element) const {
		Array<ARRAY4<ARRAY3<VAL32>>> ret = Array<ARRAY4<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
			ret[ix][1] = vertex[i[1]] ;
			ret[ix][2] = vertex[i[2]] ;
			ret[ix][3] = vertex[i[3]] ;
		}
		return std::move (ret) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY1<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_POINTS ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY2<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_LINES ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY2<ARRAY5<VAL32>>> &vbo) const {
		using VERTEX = ARRAY5<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_LINES ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[0]) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[3]) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY3<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_TRIANGLES ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY3<ARRAY5<VAL32>>> &vbo) const {
		using VERTEX = ARRAY5<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_TRIANGLES ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[0]) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[3]) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY3<ARRAY8<VAL32>>> &vbo) const {
		using VERTEX = ARRAY8<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_TRIANGLES ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[0]) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[3]) ;
		glEnableVertexAttribArray (LAYOUT_NORMAL) ;
		glVertexAttribPointer (LAYOUT_NORMAL ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[5]) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY4<ARRAY3<VAL32>>> &vbo) const {
		using VERTEX = ARRAY3<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_QUADS ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,NULL) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY4<ARRAY5<VAL32>>> &vbo) const {
		using VERTEX = ARRAY5<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_QUADS ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[0]) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[3]) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const Array<ARRAY4<ARRAY8<VAL32>>> &vbo) const {
		using VERTEX = ARRAY8<VAL32> ;
		_self.mSize = vbo.length () * vbo[0].length () ;
		_self.mMode = GL_QUADS ;
		glBindVertexArray (_self.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,_self.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,_self.mSize * _SIZEOF_ (decltype (vbo[0][0])) ,vbo[0][0].raw ().self ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[0]) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[3]) ;
		glEnableVertexAttribArray (LAYOUT_NORMAL) ;
		glVertexAttribPointer (LAYOUT_NORMAL ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,_SIZEOF_ (VERTEX) ,&_NULL_<VERTEX> ()[5]) ;
		glBindVertexArray (0) ;
	}

	void attach_transfer (Pack &_self ,const SoftImage<COLOR_BGR> &image) const {
		_self.mTexture = 0 ;
		glBindVertexArray (_self.mVAO) ;
		glBindTexture (GL_TEXTURE_2D ,_self.mVTO.self[0]) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_WRAP_S ,GL_REPEAT) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_WRAP_T ,GL_REPEAT) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_MAG_FILTER ,GL_LINEAR) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_MIN_FILTER ,GL_LINEAR) ;
		glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,VAR32 (image.cx ()) ,VAR32 (image.cy ()) ,0 ,GL_BGR ,OPENGL_TRAITS_TYPE<BYTE>::value ,image.raw ().self) ;
		glBindVertexArray (0) ;
	}

private:
	static void debug_check_error (Pack &_self) {
		const auto r1x = glGetError () ;
		_DYNAMIC_ASSERT_ (r1x == GL_NO_ERROR) ;
		(void) r1x ;
	}
} ;
} ;