#pragma once

#ifndef __CSC_IMAGES__
#error "∑(っ°Д° ;)っ : required csc_images.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#include <freeimage.h>
#pragma comment(lib ,"freeimage.lib")
#endif

#ifndef FREEIMAGE_H
#error "∑(っ°Д° ;)っ : required freeimage.h"
#endif

namespace CSC {
template <class>
class AbstractImage_Engine_FREEIMAGE ;

template <>
class AbstractImage_Engine_FREEIMAGE<COLOR_BGR> :public AbstractImage<COLOR_BGR>::Engine {
private:
	friend Singleton<AbstractImage_Engine_FREEIMAGE> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<UniqueRef<PTR<FIBITMAP>>>::make () ;
		image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_Allocate (VAR32 (cx) ,VAR32 (cy) ,24) ;
		} ,[] (PTR<FIBITMAP> &me) {
			if (me == NULL)
				return ;
			FreeImage_Unload (me) ;
			me = NULL ;
		}) ;
	}

	PACK<PTR<ARR<COLOR_BGR>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_BGR>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_BGR>> (&r1 ,_ADRESS_ (FreeImage_GetBits (r1))) ;
		ret.P2[0] = LENGTH (FreeImage_GetWidth (r1)) ;
		ret.P2[1] = LENGTH (FreeImage_GetHeight (r1)) ;
		ret.P2[2] = ret.P2[0] ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		image = AnyRef<UniqueRef<PTR<FIBITMAP>>>::make () ;
		image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = NULL ;
			const auto r1x = UniqueRef<PACK<AutoBuffer<BYTE> ,PTR<FIMEMORY>>> ([&] (PACK<AutoBuffer<BYTE> ,PTR<FIMEMORY>> &me) {
				me.P1 = data ;
				me.P2 = NULL ;
				me.P2 = FreeImage_OpenMemory (me.P1.self ,VARY (me.P1.size ())) ;
			} ,[] (PACK<AutoBuffer<BYTE> ,PTR<FIMEMORY>> &me) {
				if (me.P2 == NULL)
					return ;
				FreeImage_CloseMemory (me.P2) ;
				me.P2 = NULL ;
			}) ;
			me = FreeImage_LoadFromMemory (FreeImage_GetFileTypeFromMemory (r1x->P2) ,r1x->P2) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r2x = FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r2x == FIT_BITMAP) ;
		} ,[] (PTR<FIBITMAP> &me) {
			if (me == NULL)
				return ;
			FreeImage_Unload (me) ;
			me = NULL ;
		}) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		const auto r1x = UniqueRef<PTR<FIMEMORY>> ([&] (PTR<FIMEMORY> &me) {
			me = NULL ;
			me = FreeImage_OpenMemory () ;
		} ,[] (PTR<FIMEMORY> &me) {
			if (me == NULL)
				return ;
			FreeImage_CloseMemory (me) ;
			me = NULL ;
		}) ;
		const auto r2x = image.rebind<UniqueRef<PTR<FIBITMAP>>> ()->self ;
		const auto r3x = FreeImage_SaveToMemory (FIF_BMP ,r2x ,r1x) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		const auto r4x = _CALL_ ([&] () {
			PACK<PTR<BYTE> ,VARY> ret = {NULL ,0} ;
			const auto r5x = FreeImage_AcquireMemory (r1x ,&ret.P1 ,&ret.P2) ;
			_DYNAMIC_ASSERT_ (r5x) ;
			return std::move (ret) ;
		}) ;
		data = AutoBuffer<BYTE> (VAR (r4x.P2)) ;
		_MEMCOPY_ (data.self ,r4x.P1 ,data.size ()) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<UniqueRef<PTR<FIBITMAP>>>::make () ;
		auto &r1 = image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self ;
		r1 = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = NULL ;
			const auto r1x = FreeImage_GetFileType (_BUILDSTRS_<STRA> (file)) ;
			_DYNAMIC_ASSERT_ (r1x != FIF_UNKNOWN) ;
			me = FreeImage_Load (r1x ,_BUILDSTRS_<STRA> (file)) ;
		} ,[] (PTR<FIBITMAP> &me) {
			if (me == NULL)
				return ;
			FreeImage_Unload (me) ;
			me = NULL ;
		}) ;
		if (FreeImage_GetBPP (r1.self) != 24) {
			r1 = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
				me = NULL ;
				me = FreeImage_ConvertTo24Bits (r1.self) ;
			} ,[] (PTR<FIBITMAP> &me) {
				if (me == NULL)
					return ;
				FreeImage_Unload (me) ;
				me = NULL ;
			}) ;
		}
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self ;
		const auto r1x = FreeImage_Save (FIF_JPEG ,r1 ,_BUILDSTRS_<STRA> (file)) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_FREEIMAGE () {}

	~AbstractImage_Engine_FREEIMAGE () noexcept override {}
} ;

template <>
class AbstractImage_Engine_FREEIMAGE<COLOR_BGRA> :public AbstractImage<COLOR_BGRA>::Engine {
private:
	friend Singleton<AbstractImage_Engine_FREEIMAGE> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<UniqueRef<PTR<FIBITMAP>>>::make () ;
		image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = FreeImage_Allocate (VAR32 (cx) ,VAR32 (cy) ,32) ;
		} ,[] (PTR<FIBITMAP> &me) {
			if (me == NULL)
				return ;
			FreeImage_Unload (me) ;
			me = NULL ;
		}) ;
	}

	PACK<PTR<ARR<COLOR_BGRA>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_BGRA>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_BGRA>> (&r1 ,_ADRESS_ (FreeImage_GetBits (r1))) ;
		ret.P2[0] = LENGTH (FreeImage_GetWidth (r1)) ;
		ret.P2[1] = LENGTH (FreeImage_GetHeight (r1)) ;
		ret.P2[2] = ret.P2[0] ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		if (!image.exist ())
			image = AnyRef<UniqueRef<PTR<FIBITMAP>>>::make () ;
		image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = NULL ;
			const auto r1x = UniqueRef<PACK<AutoBuffer<BYTE> ,PTR<FIMEMORY>>> ([&] (PACK<AutoBuffer<BYTE> ,PTR<FIMEMORY>> &me) {
				me.P1 = data ;
				me.P2 = NULL ;
				me.P2 = FreeImage_OpenMemory (me.P1.self ,VARY (me.P1.size ())) ;
			} ,[] (PACK<AutoBuffer<BYTE> ,PTR<FIMEMORY>> &me) {
				if (me.P2 == NULL)
					return ;
				FreeImage_CloseMemory (me.P2) ;
				me.P2 = NULL ;
			}) ;
			me = FreeImage_LoadFromMemory (FreeImage_GetFileTypeFromMemory (r1x->P2) ,r1x->P2) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r2x = FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r2x == FIT_BITMAP) ;
		} ,[] (PTR<FIBITMAP> &me) {
			if (me == NULL)
				return ;
			FreeImage_Unload (me) ;
			me = NULL ;
		}) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		const auto r1x = UniqueRef<PTR<FIMEMORY>> ([&] (PTR<FIMEMORY> &me) {
			me = NULL ;
			me = FreeImage_OpenMemory () ;
		} ,[] (PTR<FIMEMORY> &me) {
			if (me == NULL)
				return ;
			FreeImage_CloseMemory (me) ;
			me = NULL ;
		}) ;
		const auto r2x = image.rebind<UniqueRef<PTR<FIBITMAP>>> ()->self ;
		const auto r3x = FreeImage_SaveToMemory (FIF_BMP ,r2x ,r1x) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		const auto r4x = _CALL_ ([&] () {
			PACK<PTR<BYTE> ,VARY> ret = {NULL ,0} ;
			const auto r5x = FreeImage_AcquireMemory (r1x ,&ret.P1 ,&ret.P2) ;
			_DYNAMIC_ASSERT_ (r5x) ;
			return std::move (ret) ;
		}) ;
		data = AutoBuffer<BYTE> (VAR (r4x.P2)) ;
		_MEMCOPY_ (data.self ,r4x.P1 ,data.size ()) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<UniqueRef<PTR<FIBITMAP>>>::make () ;
		auto &r1 = image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self ;
		r1 = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
			me = NULL ;
			const auto r1x = FreeImage_GetFileType (_BUILDSTRS_<STRA> (file)) ;
			_DYNAMIC_ASSERT_ (r1x != FIF_UNKNOWN) ;
			me = FreeImage_Load (r1x ,_BUILDSTRS_<STRA> (file)) ;
		} ,[] (PTR<FIBITMAP> &me) {
			if (me == NULL)
				return ;
			FreeImage_Unload (me) ;
			me = NULL ;
		}) ;
		if (FreeImage_GetBPP (r1.self) != 32) {
			r1 = UniqueRef<PTR<FIBITMAP>> ([&] (PTR<FIBITMAP> &me) {
				me = NULL ;
				me = FreeImage_ConvertTo32Bits (r1.self) ;
			} ,[] (PTR<FIBITMAP> &me) {
				if (me == NULL)
					return ;
				FreeImage_Unload (me) ;
				me = NULL ;
			}) ;
		}
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<UniqueRef<PTR<FIBITMAP>>> ().self ;
		const auto r1x = FreeImage_Save (FIF_JPEG ,r1 ,_BUILDSTRS_<STRA> (file)) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_FREEIMAGE () {}

	~AbstractImage_Engine_FREEIMAGE () noexcept override {}
} ;
} ;