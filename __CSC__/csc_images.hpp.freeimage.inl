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
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef discard
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
#pragma pop_macro ("discard")
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
		_STATIC_ASSERT_ (_SIZEOF_ (decltype ((*this))) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (decltype ((*this))) == _ALIGNOF_ (Interface)) ;
	}

	void compute_layout (AnyRef<void> &this_ ,AbstractImage<COLOR_BGR>::LAYOUT &layout) const override {
		auto &r1y = this_.rebind<NATIVE_TYPE> ().self ;
		const auto r2x = FreeImage_GetBits (r1y) ;
		const auto r3x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + _ADDRESS_ (r2x)) ;
		auto &r4y = _LOAD_<ARR<COLOR_BGR>> (r3x) ;
		layout.mImage = &r4y ;
		layout.mCX = LENGTH (FreeImage_GetWidth (r1y)) ;
		layout.mCY = LENGTH (FreeImage_GetHeight (r1y)) ;
		layout.mCW = layout.mCX ;
		layout.mCK = 0 ;
	}

	void compute_load_data (AnyRef<void> &this_ ,LENGTH cx_ ,LENGTH cy_) const override {
		const auto r1x = cx_ * cy_ * 3 ;
		_DEBUG_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		(void) r1x ;
		auto tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_Allocate (VAR32 (cx_) ,VAR32 (cy_) ,24) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		const auto r2x = COLOR_BGR {0 ,0 ,0} ;
		FreeImage_FillBackground (tmp.self ,&r2x ,0) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_load_data (AnyRef<void> &this_ ,const AutoBuffer<BYTE> &data) const override {
		auto tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r1x = UniqueRef<PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>>> ([&] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				me.P2 = data ;
				me.P1 = FreeImage_OpenMemory (me.P2.self ,VARY (me.P2.size ())) ;
				_DYNAMIC_ASSERT_ (me.P1 != NULL) ;
			} ,[] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				FreeImage_CloseMemory (me.P1) ;
			}) ;
			const auto r2x = FreeImage_GetFileTypeFromMemory (r1x->P1) ;
			me = FreeImage_LoadFromMemory (r2x ,r1x->P1) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r3x = FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r3x == FIT_BITMAP) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_save_data (const AnyRef<void> &this_ ,AutoBuffer<BYTE> &data ,const AnyRef<void> &option) const override {
		_DEBUG_ASSERT_ (!option.exist ()) ;
		const auto r1x = UniqueRef<PTR<FIMEMORY>> ([&] (PTR<FIMEMORY> &me) {
			me = FreeImage_OpenMemory () ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIMEMORY> &me) {
			FreeImage_CloseMemory (me) ;
		}) ;
		const auto r2x = this_.rebind<NATIVE_TYPE> ()->self ;
		const auto r3x = FreeImage_SaveToMemory (FIF_BMP ,r2x ,r1x.self) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		auto tmp = PACK<PTR<BYTE> ,VARY> () ;
		tmp.P1 = NULL ;
		tmp.P2 = VARY (0) ;
		const auto r4x = FreeImage_AcquireMemory (r1x.self ,&tmp.P1 ,&tmp.P2) ;
		_DYNAMIC_ASSERT_ (r4x) ;
		if SWITCH_CASE (TRUE) {
			if (LENGTH (tmp.P2) == 0)
				discard ;
			_DYNAMIC_ASSERT_ (tmp.P1 != NULL) ;
			_DYNAMIC_ASSERT_ (LENGTH (tmp.P2) >= 0 && LENGTH (tmp.P2) < VAR32_MAX) ;
		}
		data = AutoBuffer<BYTE> (LENGTH (tmp.P2)) ;
		_MEMCOPY_ (data.self ,PTRTOARR[tmp.P1] ,data.size ()) ;
	}

	void compute_load_data_file (AnyRef<void> &this_ ,const String<STR> &file) const override {
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		auto tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r2x = FreeImage_GetFileType (r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (r2x != FIF_UNKNOWN) ;
			me = FreeImage_Load (r2x ,r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		if (FreeImage_GetBPP (tmp.self) == 24)
			return ;
		tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_ConvertTo24Bits (tmp.self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_save_data_file (const AnyRef<void> &this_ ,const String<STR> &file ,const AnyRef<void> &option) const override {
		_DEBUG_ASSERT_ (!option.exist ()) ;
		auto &r1y = this_.rebind<NATIVE_TYPE> ().self ;
		const auto r2x = _BUILDSTRS_<STRA> (file) ;
		const auto r3x = FreeImage_Save (FIF_JPEG ,r1y ,r2x.raw ().self) ;
		_DYNAMIC_ASSERT_ (r3x) ;
	}
} ;

template <>
class AbstractImage_Engine_FREEIMAGE<COLOR_BGRA> :public AbstractImage<COLOR_BGRA>::Abstract {
public:
	using NATIVE_TYPE = UniqueRef<PTR<FIBITMAP>> ;

public:
	AbstractImage_Engine_FREEIMAGE () {
		_STATIC_ASSERT_ (_SIZEOF_ (decltype ((*this))) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (decltype ((*this))) == _ALIGNOF_ (Interface)) ;
	}

	void compute_layout (AnyRef<void> &this_ ,AbstractImage<COLOR_BGRA>::LAYOUT &layout) const override {
		auto &r1y = this_.rebind<NATIVE_TYPE> ().self ;
		const auto r2x = FreeImage_GetBits (r1y) ;
		const auto r3x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + _ADDRESS_ (r2x)) ;
		auto &r4y = _LOAD_<ARR<COLOR_BGRA>> (r3x) ;
		layout.mImage = &r4y ;
		layout.mCX = LENGTH (FreeImage_GetWidth (r1y)) ;
		layout.mCY = LENGTH (FreeImage_GetHeight (r1y)) ;
		layout.mCW = layout.mCX ;
		layout.mCK = 0 ;
	}

	void compute_load_data (AnyRef<void> &this_ ,LENGTH cx_ ,LENGTH cy_) const override {
		const auto r1x = cx_ * cy_ * 4 ;
		_DEBUG_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		(void) r1x ;
		auto tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_Allocate (VAR32 (cx_) ,VAR32 (cy_) ,32) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		const auto r2x = COLOR_BGRA {0 ,0 ,0 ,0} ;
		FreeImage_FillBackground (tmp.self ,&r2x ,0) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_load_data (AnyRef<void> &this_ ,const AutoBuffer<BYTE> &data) const override {
		auto tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r1x = UniqueRef<PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>>> ([&] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				me.P2 = data ;
				me.P1 = FreeImage_OpenMemory (me.P2.self ,VARY (me.P2.size ())) ;
				_DYNAMIC_ASSERT_ (me.P1 != NULL) ;
			} ,[] (PACK<PTR<FIMEMORY> ,AutoBuffer<BYTE>> &me) {
				FreeImage_CloseMemory (me.P1) ;
			}) ;
			const auto r2x = FreeImage_GetFileTypeFromMemory (r1x->P1) ;
			me = FreeImage_LoadFromMemory (r2x ,r1x->P1) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r3x = FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r3x == FIT_BITMAP) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_save_data (const AnyRef<void> &this_ ,AutoBuffer<BYTE> &data ,const AnyRef<void> &option) const override {
		_DEBUG_ASSERT_ (!option.exist ()) ;
		const auto r1x = UniqueRef<PTR<FIMEMORY>> ([&] (PTR<FIMEMORY> &me) {
			me = FreeImage_OpenMemory () ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIMEMORY> &me) {
			FreeImage_CloseMemory (me) ;
		}) ;
		const auto r2x = this_.rebind<NATIVE_TYPE> ()->self ;
		const auto r3x = FreeImage_SaveToMemory (FIF_BMP ,r2x ,r1x.self) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		auto tmp = PACK<PTR<BYTE> ,VARY> () ;
		tmp.P1 = NULL ;
		tmp.P2 = VARY (0) ;
		const auto r4x = FreeImage_AcquireMemory (r1x.self ,&tmp.P1 ,&tmp.P2) ;
		_DYNAMIC_ASSERT_ (r4x) ;
		if SWITCH_CASE (TRUE) {
			if (LENGTH (tmp.P2) == 0)
				discard ;
			_DYNAMIC_ASSERT_ (tmp.P1 != NULL) ;
			_DYNAMIC_ASSERT_ (LENGTH (tmp.P2) >= 0 && LENGTH (tmp.P2) < VAR32_MAX) ;
		}
		data = AutoBuffer<BYTE> (LENGTH (tmp.P2)) ;
		_MEMCOPY_ (data.self ,PTRTOARR[tmp.P1] ,data.size ()) ;
	}

	void compute_load_data_file (AnyRef<void> &this_ ,const String<STR> &file) const override {
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		auto tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			const auto r2x = FreeImage_GetFileType (r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (r2x != FIF_UNKNOWN) ;
			me = FreeImage_Load (r2x ,r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		if (FreeImage_GetBPP (tmp.self) == 32)
			return ;
		tmp = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_ConvertTo32Bits (tmp.self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FIBITMAP> &me) {
			FreeImage_Unload (me) ;
		}) ;
		this_ = AnyRef<NATIVE_TYPE>::make (std::move (tmp)) ;
	}

	void compute_save_data_file (const AnyRef<void> &this_ ,const String<STR> &file ,const AnyRef<void> &option) const override {
		_DEBUG_ASSERT_ (!option.exist ()) ;
		auto &r1y = this_.rebind<NATIVE_TYPE> ().self ;
		const auto r2x = _BUILDSTRS_<STRA> (file) ;
		const auto r3x = FreeImage_Save (FIF_JPEG ,r1y ,r2x.raw ().self) ;
		_DYNAMIC_ASSERT_ (r3x) ;
	}
} ;
} ;