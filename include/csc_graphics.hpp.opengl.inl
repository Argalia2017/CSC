#pragma once

#ifndef __CSC_GRAPHICS__
#error "∑(っ°Д° ;)っ : required csc_graphics.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039) //@info:warning C5039: “xxx”: 可能引发函数的指针或引用传递到 -EHc 下的外部 C 函数。如果该函数引发异常，未定义行为可能发生。
#endif
#include <GL/glew.h>
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut.h>
#undef FREEGLUT_LIB_PRAGMAS
#pragma warning (pop)
#pragma endregion
#pragma comment(lib ,"glew32.lib")
#pragma comment (lib, "freeglut.lib")
#endif

#ifndef __GLEW_H__
#error "∑(っ°Д° ;)っ : required GL/glew.h"
#endif

#ifndef __FREEGLUT_H__
#error "∑(っ°Д° ;)っ : required GL/freeglut.h"
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

class AbstractShader_Engine_OPENGL :public AbstractShader::Engine {
private:
	friend Singleton<AbstractShader_Engine_OPENGL> ;

public:
	void attach_program (AnyRef<void> &shader ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const {
		if (!shader.exist ())
			shader = AnyRef<UniqueRef<CHAR>>::make () ;
		shader.rebind<UniqueRef<CHAR>> ().self = UniqueRef<CHAR> ([&] (CHAR &me) {
			me = 0 ;
			me = glCreateProgram () ;
			const auto r1x = glCreateShader (GL_VERTEX_SHADER) ;
			glShaderSource (r1x ,1 ,&_XVALUE_ (&_LOAD_<ARR<STRA>> (&vs.self)[0]) ,&_XVALUE_ (VAR32 (vs.size ()))) ;
			glCompileShader (r1x) ;
			shaderiv (r1x) ;
			glAttachShader (me ,r1x) ;
			const auto r2x = glCreateShader (GL_FRAGMENT_SHADER) ;
			glShaderSource (r2x ,1 ,&_XVALUE_ (&_LOAD_<ARR<STRA>> (&fs.self)[0]) ,&_XVALUE_ (VAR32 (fs.size ()))) ;
			glCompileShader (r2x) ;
			shaderiv (r2x) ;
			glAttachShader (me ,r2x) ;
			glLinkProgram (me) ;
			programiv (me) ;
		} ,[] (CHAR &me) {
			if (me == 0)
				return ;
			glDeleteProgram (me) ;
			me = 0 ;
		}) ;
	}

	void active_program (const AnyRef<void> &shader) const {
		auto &r1 = shader.rebind<UniqueRef<CHAR>> ().self ;
		glUseProgram (r1) ;
	}

	FLAG uniform_find (const AnyRef<void> &shader ,const String<STRU8> &key) const {
		auto &r1 = shader.rebind<UniqueRef<CHAR>> ().self ;
		return FLAG (glGetUniformLocation (r1 ,_UASTOAS_ (key))) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAR32 &val) const {
		glUniform1i (VAR32 (loc) ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAR64 &val) const {
		glUniform1i64NV (VAR32 (loc) ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAL32 &val) const {
		glUniform1f (VAR32 (loc) ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const VAL64 &val) const {
		glUniform1d (VAR32 (loc) ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Vector<VAL32> &val) const {
		glUniform3fv (VAR32 (loc) ,1 ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Vector<VAL64> &val) const {
		glUniform3dv (VAR32 (loc) ,1 ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Matrix<VAL32> &val) const {
		glUniformMatrix4fv (VAR32 (loc) ,1 ,GL_TRUE ,val) ;
	}

	void uniform_write (const AnyRef<void> &shader ,FLAG loc ,const Matrix<VAL64> &val) const {
		glUniformMatrix4dv (VAR32 (loc) ,1 ,GL_TRUE ,val) ;
	}

private:
	AbstractShader_Engine_OPENGL () {}

	~AbstractShader_Engine_OPENGL () noexcept override {}

	static void shaderiv (CHAR shader) {
		const auto r1x = _CALL_ ([&] () {
			VAR32 ret ;
			glGetShaderiv (shader ,GL_COMPILE_STATUS ,&ret) ;
			if (ret == 0) {
				glGetShaderiv (shader ,GL_INFO_LOG_LENGTH ,&ret) ;
			} else {
				ret = 0 ;
			}
			return std::move (ret) ;
		}) ;
		const auto r2x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8> (r1x) ;
			if (ret.size () > 0)
				glGetShaderInfoLog (shader ,VAR32 (ret.size ()) ,NULL ,_UASTOAS_ (ret).raw ()) ;
			return std::move (ret) ;
		}) ;
		_DEBUG_ASSERT_ (r2x.empty ()) ;
	}

	static void programiv (CHAR shader) {
		const auto r1x = _CALL_ ([&] () {
			VAR32 ret ;
			glGetShaderiv (shader ,GL_LINK_STATUS ,&ret) ;
			if (ret == 0) {
				glGetProgramiv (shader ,GL_INFO_LOG_LENGTH ,&ret) ;
			} else {
				ret = 0 ;
			}
			return std::move (ret) ;
		}) ;
		const auto r2x = _CALL_ ([&] () {
			String<STRU8> ret = String<STRU8> (r1x) ;
			if (ret.size () > 0)
				glGetShaderInfoLog (shader ,VAR32 (ret.size ()) ,NULL ,_UASTOAS_ (ret).raw ()) ;
			return std::move (ret) ;
		}) ;
		_DEBUG_ASSERT_ (r2x.empty ()) ;
	}
} ;

class AbstractShaderSprite_Engine_OPENGL :public AbstractShaderSprite::Engine {
private:
	//@error:bind to (layout = 1) Vec3 in GLSL
	static constexpr CHAR LAYOUT_POSITION = 1 ;
	//@error:bind to (layout = 2) Vec2 in GLSL
	static constexpr CHAR LAYOUT_TEXCOORD = 2 ;
	//@error:bind to (layout = 3) Vec3 in GLSL
	static constexpr CHAR LAYOUT_NORMAL = 3 ;

	class Pack {
	private:
		friend AbstractShaderSprite_Engine_OPENGL ;
		UniqueRef<CHAR> mVAO ;
		UniqueRef<AutoBuffer<CHAR>> mVBO ;
		UniqueRef<AutoBuffer<CHAR>> mVTO ;
		LENGTH mSize ;
		FLAG mMode ;
	} ;

private:
	friend Singleton<AbstractShaderSprite_Engine_OPENGL> ;

public:
	void attach_mesh (AnyRef<void> &sprite ,const Mesh &mesh) const {
		auto &r1 = sprite.rebind<Pack> ().self ;
		r1.mVAO = UniqueRef<CHAR> ([&] (CHAR &me) {
			glGenVertexArrays (1 ,&me) ;
		} ,[] (CHAR &me) {
			glDeleteVertexArrays (1 ,&me) ;
		}) ;
		r1.mVBO = UniqueRef<AutoBuffer<CHAR>> ([&] (AutoBuffer<CHAR> &me) {
			me = AutoBuffer<CHAR> (1) ;
			glGenBuffers (VAR32 (me.size ()) ,me.self) ;
		} ,[] (AutoBuffer<CHAR> &me) {
			glDeleteBuffers (VAR32 (me.size ()) ,me.self) ;
		}) ;
		r1.mVTO = UniqueRef<AutoBuffer<CHAR>> ([&] (AutoBuffer<CHAR> &me) {
			me = AutoBuffer<CHAR> (1) ;
			glGenTextures (VAR32 (me.size ()) ,me.self) ;
		} ,[] (AutoBuffer<CHAR> &me) {
			glDeleteTextures (VAR32 (me.size ()) ,me.self) ;
		}) ;
	}

	void draw (const AnyRef<void> &sprite) const {
		auto &r1 = sprite.rebind<Pack> ().self ;
		glBindVertexArray (r1.mVAO) ;
		if (r1.mVTO.exist ()) {
			glActiveTexture (GL_TEXTURE_2D) ;
			glBindTexture (GL_TEXTURE_2D ,r1.mVTO.self[0]) ;
		}
		glDrawArrays (CHAR (r1.mMode) ,0 ,VAR32 (r1.mSize)) ;
	}

private:
	AbstractShaderSprite_Engine_OPENGL () {}

	~AbstractShaderSprite_Engine_OPENGL () noexcept override {}

	static void transfer (Pack &sprite ,const Array<ARRAY1<ARRAY3<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_POINTS ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,12 ,NULL) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY2<ARRAY3<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_LINES ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,12 ,NULL) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY2<ARRAY5<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_LINES ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,20 ,NULL) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r1x = &_NULL_<const BYTE> () + LENGTH (12) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,20 ,r1x) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY3<ARRAY3<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_TRIANGLES ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,12 ,NULL) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY3<ARRAY5<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_TRIANGLES ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,20 ,NULL) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r1x = &_NULL_<const BYTE> () + LENGTH (12) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,20 ,r1x) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY3<ARRAY8<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_TRIANGLES ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,32 ,NULL) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r1x = &_NULL_<const BYTE> () + LENGTH (12) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,32 ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_NORMAL) ;
		const auto r2x = &_NULL_<const BYTE> () + LENGTH (20) ;
		glVertexAttribPointer (LAYOUT_NORMAL ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,32 ,r2x) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY4<ARRAY3<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_QUADS ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,12 ,NULL) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY4<ARRAY5<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_QUADS ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,20 ,NULL) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r1x = &_NULL_<const BYTE> () + LENGTH (12) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,20 ,r1x) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const Array<ARRAY4<ARRAY8<VAL32>>> &vbo) {
		sprite.mSize = vbo.length () ;
		sprite.mMode = GL_QUADS ;
		glBindVertexArray (sprite.mVAO) ;
		glBindBuffer (GL_ARRAY_BUFFER ,sprite.mVBO.self[0]) ;
		glBufferData (GL_ARRAY_BUFFER ,sprite.mSize * _SIZEOF_ (vbo[0]) ,&vbo[0][0] ,GL_STATIC_DRAW) ;
		glEnableVertexAttribArray (LAYOUT_POSITION) ;
		glVertexAttribPointer (LAYOUT_POSITION ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,32 ,NULL) ;
		glEnableVertexAttribArray (LAYOUT_TEXCOORD) ;
		const auto r1x = &_NULL_<const BYTE> () + LENGTH (12) ;
		glVertexAttribPointer (LAYOUT_TEXCOORD ,2 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,32 ,r1x) ;
		glEnableVertexAttribArray (LAYOUT_NORMAL) ;
		const auto r2x = &_NULL_<const BYTE> () + LENGTH (20) ;
		glVertexAttribPointer (LAYOUT_NORMAL ,3 ,OPENGL_TRAITS_TYPE<VAL32>::value ,GL_FALSE ,32 ,r2x) ;
		glBindVertexArray (0) ;
	}

	static void transfer (Pack &sprite ,const AbstractImage<COLOR_BGR> &image) {
		glBindVertexArray (sprite.mVAO) ;
		glBindTexture (GL_TEXTURE_2D ,sprite.mVTO.self[0]) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_WRAP_S ,GL_REPEAT) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_WRAP_T ,GL_REPEAT) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_MAG_FILTER ,GL_LINEAR) ;
		glTexParameteri (GL_TEXTURE_2D ,GL_TEXTURE_MIN_FILTER ,GL_LINEAR) ;
		glTexImage2D (GL_TEXTURE_2D ,0 ,GL_RGB ,VAR32 (image.cx ()) ,VAR32 (image.cy ()) ,0 ,GL_BGR ,OPENGL_TRAITS_TYPE<BYTE>::value ,image) ;
		glBindVertexArray (0) ;
	}
} ;
} ;