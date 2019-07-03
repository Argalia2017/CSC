#pragma once

#ifndef __CSC_IMAGES__
#error "∑(っ°Д° ;)っ : require 'csc_images.hpp'"
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
#pragma warning (disable :26477)
#endif
#include <freeimage.h>
#pragma warning (pop)
#pragma endregion
#endif

#ifndef FREEIMAGE_H
#error "∑(っ°Д° ;)っ : require 'freeimage.h'"
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_freeimage
#define use_comment_lib_freeimage "freeimage.lib"
#endif
#pragma comment (lib ,use_comment_lib_freeimage)
#undef use_comment_lib_freeimage
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
template <class>
class AbstractImage_Engine_FREEIMAGE ;

template <>
class AbstractImage_Engine_FREEIMAGE<COLOR_BGR> :public AbstractImage<COLOR_BGR>::Abstract {
public:
	using NATIVE_TYPE = UniqueRef<PTR<FIBITMAP>> ;

public:
	AbstractImage_Engine_FREEIMAGE () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void compute_layout (AnyRef<void> &_this ,PACK<PTR<ARR<COLOR_BGR>> ,LENGTH[4]> &layout) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		layout.P1 = &_LOAD_<ARR<COLOR_BGR>> (FreeImage_GetBits (r1)) ;
		layout.P2[0] = LENGTH (FreeImage_GetWidth (r1)) ;
		layout.P2[1] = LENGTH (FreeImage_GetHeight (r1)) ;
		layout.P2[2] = layout.P2[0] ;
		layout.P2[3] = 0 ;
	}

	void compute_load_data (AnyRef<void> &_this ,LENGTH _cx ,LENGTH _cy) const override {
		auto rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_Allocate (VAR32 (_cx) ,VAR32 (_cy) ,24) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		const auto r1x = COLOR_BGR {0 ,0 ,0} ;
		FreeImage_FillBackground (rax ,&r1x ,0) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		auto rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r1x = UniqueRef<PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>>> ([&] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				me.P2 = data ;
				me.P1 = FreeImage_OpenMemory (me.P2.self ,VARY (me.P2.size ())) ;
				_DYNAMIC_ASSERT_ (me.P1 != NULL) ;
			} ,[] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				_DEBUG_ASSERT_ (me.P1 != NULL) ;
				FreeImage_CloseMemory (me.P1) ;
			}) ;
			const auto r2x = FreeImage_GetFileTypeFromMemory (r1x->P1) ;
			me = FreeImage_LoadFromMemory (r2x ,r1x->P1) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r3x = FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r3x == FIT_BITMAP) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		_DEBUG_ASSERT_ (!param.exist ()) ;
		const auto r1x = UniqueRef<PTR<FIMEMORY>> ([&] (PTR<FIMEMORY> &me) {
			me = FreeImage_OpenMemory () ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIMEMORY> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_CloseMemory (me) ;
		}) ;
		const auto r2x = _this.rebind<NATIVE_TYPE> ()->self ;
		const auto r3x = FreeImage_SaveToMemory (FIF_BMP ,r2x ,r1x) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		auto rax = PACK<PTR<BYTE> ,VARY> () ;
		const auto r4x = FreeImage_AcquireMemory (r1x ,&(rax.P1 = NULL) ,&(rax.P2 = 0)) ;
		_DYNAMIC_ASSERT_ (r4x) ;
		_DYNAMIC_ASSERT_ (rax.P1 != NULL || rax.P2 == 0) ;
		_DYNAMIC_ASSERT_ (LENGTH (rax.P2) >= 0 && LENGTH (rax.P2) < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (LENGTH (rax.P2)) ;
		_MEMCOPY_ (data.self ,PTRTOARR[rax.P1] ,data.size ()) ;
	}

	void compute_load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		auto rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r2x = FreeImage_GetFileType (r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (r2x != FIF_UNKNOWN) ;
			me = FreeImage_Load (r2x ,r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		if (FreeImage_GetBPP (rax.self) == 24)
			return ;
		rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_ConvertTo24Bits (rax.self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		_DEBUG_ASSERT_ (!param.exist ()) ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		const auto r2x = FreeImage_Save (FIF_JPEG ,r1 ,r1x.raw ().self) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;

template <>
class AbstractImage_Engine_FREEIMAGE<COLOR_BGRA> :public AbstractImage<COLOR_BGRA>::Abstract {
public:
	using NATIVE_TYPE = UniqueRef<PTR<FIBITMAP>> ;

public:
	AbstractImage_Engine_FREEIMAGE () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	void compute_layout (AnyRef<void> &_this ,PACK<PTR<ARR<COLOR_BGRA>> ,LENGTH[4]> &layout) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		layout.P1 = &_LOAD_<ARR<COLOR_BGRA>> (FreeImage_GetBits (r1)) ;
		layout.P2[0] = LENGTH (FreeImage_GetWidth (r1)) ;
		layout.P2[1] = LENGTH (FreeImage_GetHeight (r1)) ;
		layout.P2[2] = layout.P2[0] ;
		layout.P2[3] = 0 ;
	}

	void compute_load_data (AnyRef<void> &_this ,LENGTH _cx ,LENGTH _cy) const override {
		auto rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_Allocate (VAR32 (_cx) ,VAR32 (_cy) ,32) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		const auto r1x = COLOR_BGRA {0 ,0 ,0 ,0} ;
		FreeImage_FillBackground (rax ,&r1x ,0) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		auto rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r1x = UniqueRef<PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>>> ([&] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				me.P2 = data ;
				me.P1 = FreeImage_OpenMemory (me.P2.self ,VARY (me.P2.size ())) ;
				_DYNAMIC_ASSERT_ (me.P1 != NULL) ;
			} ,[] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				_DEBUG_ASSERT_ (me.P1 != NULL) ;
				FreeImage_CloseMemory (me.P1) ;
			}) ;
			const auto r2x = FreeImage_GetFileTypeFromMemory (r1x->P1) ;
			me = FreeImage_LoadFromMemory (r2x ,r1x->P1) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r3x = FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r3x == FIT_BITMAP) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		_DEBUG_ASSERT_ (!param.exist ()) ;
		const auto r1x = UniqueRef<PTR<FIMEMORY>> ([&] (PTR<FIMEMORY> &me) {
			me = FreeImage_OpenMemory () ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIMEMORY> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_CloseMemory (me) ;
		}) ;
		const auto r2x = _this.rebind<NATIVE_TYPE> ()->self ;
		const auto r3x = FreeImage_SaveToMemory (FIF_BMP ,r2x ,r1x) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		auto rax = PACK<PTR<BYTE> ,VARY> () ;
		const auto r4x = FreeImage_AcquireMemory (r1x ,&(rax.P1 = NULL) ,&(rax.P2 = 0)) ;
		_DYNAMIC_ASSERT_ (r4x) ;
		_DYNAMIC_ASSERT_ (rax.P1 != NULL || rax.P2 == 0) ;
		_DYNAMIC_ASSERT_ (LENGTH (rax.P2) >= 0 && LENGTH (rax.P2) < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (LENGTH (rax.P2)) ;
		_MEMCOPY_ (data.self ,PTRTOARR[rax.P1] ,data.size ()) ;
	}

	void compute_load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		auto rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r2x = FreeImage_GetFileType (r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (r2x != FIF_UNKNOWN) ;
			me = FreeImage_Load (r2x ,r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		if (FreeImage_GetBPP (rax.self) == 32)
			return ;
		rax = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_ConvertTo32Bits (rax.self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			FreeImage_Unload (me) ;
		}) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void compute_save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		_DEBUG_ASSERT_ (!param.exist ()) ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		const auto r2x = FreeImage_Save (FIF_JPEG ,r1 ,r1x.raw ().self) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;
} ;