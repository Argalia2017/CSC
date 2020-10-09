#pragma once

#ifndef __CSC_IMAGES__
#error "∑(っ°Д° ;)っ : require 'csc_images.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("delegate")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_once")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef delegate
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_once
#undef discard
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#pragma warning (disable :26477)
#endif
#include <freeimage.h>
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif
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
#pragma pop_macro ("delegate")
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_once")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using ::FIMEMORY ;

using ::FreeImage_GetBits ;
using ::FreeImage_GetWidth ;
using ::FreeImage_GetHeight ;
using ::FreeImage_Allocate ;
using ::FreeImage_Unload ;
using ::FreeImage_FillBackground ;
using ::FreeImage_OpenMemory ;
using ::FreeImage_CloseMemory ;
using ::FreeImage_GetFileTypeFromMemory ;
using ::FreeImage_LoadFromMemory ;
using ::FreeImage_GetImageType ;
using ::FreeImage_SaveToMemory ;
using ::FreeImage_AcquireMemory ;
using ::FreeImage_GetFileType ;
using ::FreeImage_Load ;
using ::FreeImage_GetBPP ;
using ::FreeImage_ConvertTo24Bits ;
using ::FreeImage_Save ;
using ::FreeImage_ConvertTo32Bits ;
} ;

template <>
class Image<COLOR_BGR>::Private::Implement :
	delegate public Image<COLOR_BGR>::Abstract {
private:
	using LAYOUT_PACK = Image<COLOR_BGR>::LAYOUT_PACK ;
	using HFIBITMAP = PTR<FIBITMAP> ;

private:
	UniqueRef<HFIBITMAP> mHolder ;

public:
	implicit Implement () = default ;

	Reference native () override {
		return SafeReference<UniqueRef<HFIBITMAP>> (mHolder) ;
	}

	Reference native () const override {
		return SafeReference<const UniqueRef<HFIBITMAP>> (mHolder) ;
	}

	LAYOUT_PACK layout () override {
		LAYOUT_PACK ret ;
		const auto r1x = api::FreeImage_GetBits (mHolder) ;
		const auto r2x = _POINTER_CAST_ (ARGV<ARR<COLOR_BGR>>::ID ,r1x) ;
		ret.mImage = r2x ;
		ret.mCX = LENGTH (api::FreeImage_GetWidth (mHolder)) ;
		ret.mCY = LENGTH (api::FreeImage_GetHeight (mHolder)) ;
		ret.mCW = ret.mCX ;
		ret.mCK = 0 ;
		return _MOVE_ (ret) ;
	}

	void load_data (const LENGTH &cx_ ,const LENGTH &cy_) override {
		const auto r1x = cx_ * cy_ * 3 ;
		_STATIC_UNUSED_ (r1x) ;
		_DEBUG_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		auto rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
			me = api::FreeImage_Allocate (VAR32 (cx_) ,VAR32 (cy_) ,VAR32 (_SIZEOF_ (COLOR_BGR))) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HFIBITMAP &me) {
			api::FreeImage_Unload (me) ;
		}) ;
		const auto r2x = COLOR_BGR ({0 ,0 ,0}) ;
		api::FreeImage_FillBackground (rax.self ,DEPTR[r2x] ,0) ;
		mHolder = _MOVE_ (rax) ;
	}

	void load_data (const AutoBuffer<BYTE> &data) override {
		using R1X = PTR<api::FIMEMORY> ;
		auto rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
			const auto r1x = UniqueRef<PACK<R1X ,AutoBuffer<BYTE>>> ([&] (PACK<R1X ,AutoBuffer<BYTE>> &me) {
				me.mP2 = data ;
				me.mP1 = api::FreeImage_OpenMemory (me.mP2.self ,VARY (me.mP2.size ())) ;
				_DYNAMIC_ASSERT_ (me.mP1 != NULL) ;
			} ,[] (PACK<R1X ,AutoBuffer<BYTE>> &me) {
				api::FreeImage_CloseMemory (me.mP1) ;
			}) ;
			const auto r2x = api::FreeImage_GetFileTypeFromMemory (r1x->mP1) ;
			me = api::FreeImage_LoadFromMemory (r2x ,r1x->mP1) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r3x = api::FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r3x == FIT_BITMAP) ;
		} ,[] (HFIBITMAP &me) {
			api::FreeImage_Unload (me) ;
		}) ;
		mHolder = _MOVE_ (rax) ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<> &option) const override {
		using R1X = PTR<api::FIMEMORY> ;
		_DEBUG_ASSERT_ (!option.exist ()) ;
		const auto r1x = UniqueRef<R1X> ([&] (R1X &me) {
			me = api::FreeImage_OpenMemory () ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (R1X &me) {
			api::FreeImage_CloseMemory (me) ;
		}) ;
		const auto r2x = api::FreeImage_SaveToMemory (FIF_BMP ,mHolder ,r1x.self) ;
		_DYNAMIC_ASSERT_ (r2x) ;
		auto rax = PTR<BYTE> (NULL) ;
		auto rbx = VARY (0) ;
		const auto r3x = api::FreeImage_AcquireMemory (r1x.self ,DEPTR[rax] ,DEPTR[rbx]) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		if switch_once (TRUE) {
			if (LENGTH (rbx) == 0)
				discard ;
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			_DYNAMIC_ASSERT_ (LENGTH (rbx) >= 0 && LENGTH (rbx) < VAR32_MAX) ;
		}
		data = AutoBuffer<BYTE> (LENGTH (rbx)) ;
		BasicProc::mem_move (data.self ,PTRTOARR[rax] ,data.size ()) ;
	}

	void load_data_file (const String<STR> &file) override {
		const auto r1x = StringProc::build_strs (ARGV<STRA>::ID ,file) ;
		auto rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
			const auto r2x = api::FreeImage_GetFileType (r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (r2x != FIF_UNKNOWN) ;
			me = api::FreeImage_Load (r2x ,r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HFIBITMAP &me) {
			api::FreeImage_Unload (me) ;
		}) ;
		if switch_once (TRUE) {
			const auto r3x = LENGTH (api::FreeImage_GetBPP (rax.self)) ;
			if (r3x == _SIZEOF_ (COLOR_BGR))
				discard ;
			rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
				me = api::FreeImage_ConvertTo24Bits (rax.self) ;
				_DYNAMIC_ASSERT_ (me != NULL) ;
			} ,[] (HFIBITMAP &me) {
				api::FreeImage_Unload (me) ;
			}) ;
		}
		mHolder = _MOVE_ (rax) ;
	}

	void save_data_file (const String<STR> &file ,const AnyRef<> &option) const override {
		_DEBUG_ASSERT_ (!option.exist ()) ;
		const auto r1x = StringProc::build_strs (ARGV<STRA>::ID ,file) ;
		const auto r2x = api::FreeImage_Save (FIF_JPEG ,mHolder ,r1x.raw ().self) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;

template <>
class Image<COLOR_BGRA>::Private::Implement :
	delegate public Image<COLOR_BGRA>::Abstract {
private:
	using LAYOUT_PACK = Image<COLOR_BGRA>::LAYOUT_PACK ;
	using HFIBITMAP = PTR<FIBITMAP> ;

private:
	UniqueRef<HFIBITMAP> mHolder ;

public:
	implicit Implement () = default ;

	Reference native () override {
		return SafeReference<UniqueRef<HFIBITMAP>> (mHolder) ;
	}

	Reference native () const override {
		return SafeReference<const UniqueRef<HFIBITMAP>> (mHolder) ;
	}

	LAYOUT_PACK layout () override {
		LAYOUT_PACK ret ;
		const auto r1x = api::FreeImage_GetBits (mHolder) ;
		const auto r2x = _POINTER_CAST_ (ARGV<ARR<COLOR_BGRA>>::ID ,r1x) ;
		ret.mImage = r2x ;
		ret.mCX = LENGTH (api::FreeImage_GetWidth (mHolder)) ;
		ret.mCY = LENGTH (api::FreeImage_GetHeight (mHolder)) ;
		ret.mCW = ret.mCX ;
		ret.mCK = 0 ;
		return _MOVE_ (ret) ;
	}

	void load_data (const LENGTH &cx_ ,const LENGTH &cy_) override {
		const auto r1x = cx_ * cy_ * 3 ;
		_STATIC_UNUSED_ (r1x) ;
		_DEBUG_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		auto rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
			me = api::FreeImage_Allocate (VAR32 (cx_) ,VAR32 (cy_) ,VAR32 (_SIZEOF_ (COLOR_BGRA))) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HFIBITMAP &me) {
			api::FreeImage_Unload (me) ;
		}) ;
		const auto r2x = COLOR_BGRA ({0 ,0 ,0 ,0});
		api::FreeImage_FillBackground (rax.self ,DEPTR[r2x] ,0) ;
		mHolder = _MOVE_ (rax) ;
	}

	void load_data (const AutoBuffer<BYTE> &data) override {
		using R1X = PTR<api::FIMEMORY> ;
		auto rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
			const auto r1x = UniqueRef<PACK<R1X ,AutoBuffer<BYTE>>> ([&] (PACK<R1X ,AutoBuffer<BYTE>> &me) {
				me.mP2 = data ;
				me.mP1 = api::FreeImage_OpenMemory (me.mP2.self ,VARY (me.mP2.size ())) ;
				_DYNAMIC_ASSERT_ (me.mP1 != NULL) ;
			} ,[] (PACK<R1X ,AutoBuffer<BYTE>> &me) {
				api::FreeImage_CloseMemory (me.mP1) ;
			}) ;
			const auto r2x = api::FreeImage_GetFileTypeFromMemory (r1x->mP1) ;
			me = api::FreeImage_LoadFromMemory (r2x ,r1x->mP1) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			const auto r3x = api::FreeImage_GetImageType (me) ;
			_DYNAMIC_ASSERT_ (r3x == FIT_BITMAP) ;
		} ,[] (HFIBITMAP &me) {
			api::FreeImage_Unload (me) ;
		}) ;
		mHolder = _MOVE_ (rax) ;
	}

	void save_data (AutoBuffer<BYTE> &data ,const AnyRef<> &option) const override {
		using R1X = PTR<api::FIMEMORY> ;
		_DEBUG_ASSERT_ (!option.exist ()) ;
		const auto r1x = UniqueRef<R1X> ([&] (R1X &me) {
			me = api::FreeImage_OpenMemory () ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (R1X &me) {
			api::FreeImage_CloseMemory (me) ;
		}) ;
		const auto r2x = api::FreeImage_SaveToMemory (FIF_BMP ,mHolder ,r1x.self) ;
		_DYNAMIC_ASSERT_ (r2x) ;
		auto rax = PTR<BYTE> (NULL) ;
		auto rbx = VARY (0) ;
		const auto r3x = api::FreeImage_AcquireMemory (r1x.self ,DEPTR[rax] ,DEPTR[rbx]) ;
		_DYNAMIC_ASSERT_ (r3x) ;
		if switch_once (TRUE) {
			if (LENGTH (rbx) == 0)
				discard ;
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			_DYNAMIC_ASSERT_ (LENGTH (rbx) >= 0 && LENGTH (rbx) < VAR32_MAX) ;
		}
		data = AutoBuffer<BYTE> (LENGTH (rbx)) ;
		BasicProc::mem_move (data.self ,PTRTOARR[rax] ,data.size ()) ;
	}

	void load_data_file (const String<STR> &file) override {
		const auto r1x = StringProc::build_strs (ARGV<STRA>::ID ,file) ;
		auto rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
			const auto r2x = api::FreeImage_GetFileType (r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (r2x != FIF_UNKNOWN) ;
			me = api::FreeImage_Load (r2x ,r1x.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HFIBITMAP &me) {
			api::FreeImage_Unload (me) ;
		}) ;
		if switch_once (TRUE) {
			const auto r3x = LENGTH (api::FreeImage_GetBPP (rax.self)) ;
			if (r3x == _SIZEOF_ (COLOR_BGRA))
				discard ;
			rax = UniqueRef<HFIBITMAP> ([&] (HFIBITMAP &me) {
				me = api::FreeImage_ConvertTo24Bits (rax.self) ;
				_DYNAMIC_ASSERT_ (me != NULL) ;
			} ,[] (HFIBITMAP &me) {
				api::FreeImage_Unload (me) ;
			}) ;
		}
		mHolder = _MOVE_ (rax) ;
	}

	void save_data_file (const String<STR> &file ,const AnyRef<> &option) const override {
		_DEBUG_ASSERT_ (!option.exist ()) ;
		const auto r1x = StringProc::build_strs (ARGV<STRA>::ID ,file) ;
		const auto r2x = api::FreeImage_Save (FIF_JPEG ,mHolder ,r1x.raw ().self) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;
} ;