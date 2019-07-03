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
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
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

template <class _ARG1>
using OPENGL_TRAITS_TYPE = typename U::OPENGL_TRAITS<_ARG1>::TYPE ;

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
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void compute_load_data (AnyRef<void> &_this ,const PhanBuffer<const BYTE> &vs ,const PhanBuffer<const BYTE> &fs) const override {
		auto rax = UniqueRef<CHAR> ([&] (CHAR &me) {
			me = glCreateProgram () ;
			_DYNAMIC_ASSERT_ (me != 0) ;
			const auto r1x = glCreateShader (GL_VERTEX_SHADER) ;
			const auto r2x = _LOAD_<ARR<STRA>> (vs.self) ;
			const auto r3x = VAR32 (vs.size ()) ;
			glShaderSource (r1x ,1 ,&r2x ,&r3x) ;
			glCompileShader (r1x) ;
			check_shaderiv (r1x) ;
			glAttachShader (me ,r1x) ;
			const auto r4x = glCreateShader (GL_FRAGMENT_SHADER) ;
			const auto r5x = _LOAD_<ARR<STRA>> (fs.self) ;
			const auto r6x = VAR32 (fs.size ()) ;
			glShaderSource (r4x ,1 ,&r5x ,&r6x) ;
			glCompileShader (r4x) ;
			check_shaderiv (r4x) ;
			glAttachShader (me ,r4x) ;
			glLinkProgram (me) ;
			check_programiv (me) ;
		} ,[] (CHAR &me) {
			_DEBUG_ASSERT_ (me != 0) ;
			glDeleteProgram (me) ;
		}) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_active_pipeline (AnyRef<void> &_this) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		glUseProgram (r1) ;
	}

	void compute_uniform_find (AnyRef<void> &_this ,const String<STR> &name ,INDEX &index) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = identity_name (name) ;
		index = INDEX (glGetUniformLocation (r1 ,r1x.raw ().self)) ;
		_DEBUG_ASSERT_ (index != GL_INVALID_VALUE) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAR32 &data) const override {
		glUniform1i (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAR64 &data) const override {
		glUniform1i64NV (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAL32 &data) const override {
		glUniform1f (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const VAL64 &data) const override {
		glUniform1d (VAR32 (index) ,data) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Vector<VAL32> &data) const override {
		const auto r1x = ARRAY4<VAL32> {data[0] ,data[1] ,data[2] ,data[3]} ;
		glUniform4fv (VAR32 (index) ,1 ,r1x.raw ().self) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Vector<VAL64> &data) const override {
		const auto r1x = ARRAY4<VAL64> {data[0] ,data[1] ,data[2] ,data[3]} ;
		glUniform4dv (VAR32 (index) ,1 ,r1x.raw ().self) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Matrix<VAL32> &data) const override {
		const auto r1x = ARRAY16<VAL32> ({
			data[0][0] ,data[0][1] ,data[0][2] ,data[0][3] ,
			data[1][0] ,data[1][1] ,data[1][2] ,data[1][3] ,
			data[2][0] ,data[2][1] ,data[2][2] ,data[2][3] ,
			data[3][0] ,data[3][1] ,data[3][2] ,data[3][3]}) ;
		glUniformMatrix4fv (VAR32 (index) ,1 ,GL_TRUE ,r1x.raw ().self) ;
	}

	void compute_uniform_write (AnyRef<void> &_this ,INDEX index ,const Matrix<VAL64> &data) const override {
		const auto r1x = ARRAY16<VAL64> ({
			data[0][0] ,data[0][1] ,data[0][2] ,data[0][3] ,
			data[1][0] ,data[1][1] ,data[1][2] ,data[1][3] ,
			data[2][0] ,data[2][1] ,data[2][2] ,data[2][3] ,
			data[3][0] ,data[3][1] ,data[3][2] ,data[3][3]}) ;
		glUniformMatrix4dv (VAR32 (index) ,1 ,GL_TRUE ,r1x.raw ().self) ;
	}

	void compute_sprite_load_data (AnyRef<void> &_this ,const Mesh &mesh) const override {
		auto rax = Holder () ;
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
		compute_transfer_data (rax ,bind_vertex (mesh.vertex () ,mesh.element ())) ;
		compute_transfer_data (rax ,mesh.texture ()[0]) ;
		_this = AnyRef<SPRITE_NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_sprite_active_texture (AnyRef<void> &_this ,INDEX texture) const override {
		auto &r1 = _this.rebind<SPRITE_NATIVE_TYPE> ().self ;
		_DYNAMIC_ASSERT_ (texture >= 0 && texture < r1.mVTO->size ()) ;
		r1.mTexture = texture ;
	}

	void compute_sprite_draw (AnyRef<void> &_this) const override {
		auto &r1 = _this.rebind<SPRITE_NATIVE_TYPE> ().self ;
		glBindVertexArray (r1.mVAO) ;
		for (FOR_ONCE_DO_WHILE) {
			if (r1.mTexture == VAR_NONE)
				continue ;
			glActiveTexture (GL_TEXTURE_2D) ;
			glBindTexture (GL_TEXTURE_2D ,r1.mVTO.self[r1.mTexture]) ;
		}
		glDrawArrays (CHAR (r1.mMode) ,0 ,VAR32 (r1.mSize)) ;
	}

private:
	String<STRA> identity_name (const String<STR> &name) const {
		String<STRA> ret = String<STRA> (name.length ()) ;
		for (INDEX i = 0 ; i < ret.size () ; i++) {
			const auto r2x = BOOL (name[i] >= STR ('a') && name[i] <= STR ('z')) ;
			const auto r3x = BOOL (name[i] >= STR ('A') && name[i] <= STR ('Z')) ;
			const auto r4x = BOOL (name[i] >= STR ('0') && name[i] <= STR ('9')) ;
			const auto r5x = BOOL (name[i] == STR ('_')) ;
			_DEBUG_ASSERT_ (r2x || r3x || r4x || r5x) ;
			ret[i] = STRA (name[i]) ;
		}
		return std::move (ret) ;
	}

	void check_shaderiv (CHAR shader) const {
		auto rax = ARRAY2<VAR32> () ;
		glGetShaderiv (shader ,GL_COMPILE_STATUS ,&(rax[0] = GL_FALSE)) ;
		if (rax[0] == GL_TRUE)
			return ;
		glGetShaderiv (shader ,GL_INFO_LOG_LENGTH ,&(rax[1] = 0)) ;
		if (rax[1] <= 0)
			return ;
		auto rbx = String<STRA> (rax[1]) ;
		glGetShaderInfoLog (shader ,VAR32 (rbx.size ()) ,NULL ,rbx.raw ().self) ;
		_DYNAMIC_ASSERT_ (rbx.empty ()) ;
	}

	void check_programiv (CHAR shader) const {
		auto rax = ARRAY2<VAR32> () ;
		glGetProgramiv (shader ,GL_LINK_STATUS ,&(rax[0] = GL_FALSE)) ;
		if (rax[0] == GL_TRUE)
			return ;
		glGetProgramiv (shader ,GL_INFO_LOG_LENGTH ,&(rax[1] = 0)) ;
		if (rax[1] <= 0)
			return ;
		auto rbx = String<STRA> (rax[1]) ;
		glGetProgramInfoLog (shader ,VAR32 (rbx.size ()) ,NULL ,rbx.raw ().self) ;
		_DYNAMIC_ASSERT_ (rbx.empty ()) ;
	}

	Array<ARRAY1<ARRAY3<VAL32>>> bind_vertex (const Set<ARRAY3<VAL32>> &vertex ,const Queue<ARRAY1<INDEX>> &element) const {
		Array<ARRAY1<ARRAY3<VAL32>>> ret = Array<ARRAY1<ARRAY3<VAL32>>> (element.length ()) ;
		INDEX iw = 0 ;
		for (auto &&i : element) {
			INDEX ix = iw++ ;
			ret[ix][0] = vertex[i[0]] ;
		}
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
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
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
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
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
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
		_DEBUG_ASSERT_ (iw == ret.length ()) ;
		return std::move (ret) ;
	}

	void compute_transfer_data (Holder &_self ,const Array<ARRAY1<ARRAY3<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY2<ARRAY3<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY2<ARRAY5<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY3<ARRAY3<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY3<ARRAY5<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY3<ARRAY8<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY4<ARRAY3<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY4<ARRAY5<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Array<ARRAY4<ARRAY8<VAL32>>> &vbo) const {
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

	void compute_transfer_data (Holder &_self ,const Bitmap<COLOR_BGR> &image) const {
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
	static void compute_check_error (UniqueRef<CHAR> &_self) {
		const auto r1x = glGetError () ;
		_DYNAMIC_ASSERT_ (r1x == GL_NO_ERROR) ;
	}
} ;
} ;