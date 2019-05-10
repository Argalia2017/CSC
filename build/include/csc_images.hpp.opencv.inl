#pragma once

#ifndef __CSC_IMAGES__
#error "∑(っ°Д° ;)っ : require 'csc_images.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#endif

#ifdef __CSC_DEPRECATED__
#ifdef _HAS_AUTO_PTR_ETC
#if (_HAS_AUTO_PTR_ETC == 0)
#include <random>
#include <algorithm>
#pragma push_macro ("random_shuffle")
#ifdef random_shuffle
#undef random_shuffle
#endif
#define random_shuffle(arg1 ,arg2) shuffle((arg1) ,(arg2) ,std::random_device ())
#endif
#endif
#endif

#ifdef __CSC_DEPRECATED__
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4265)
#pragma warning (disable :4266)
#pragma warning (disable :4435)
#pragma warning (disable :4711)
#pragma warning (disable :4819)
#pragma warning (disable :4946)
#pragma warning (disable: 4996)
#pragma warning (disable :6011)
#pragma warning (disable :6201)
#pragma warning (disable :6269)
#pragma warning (disable :6294)
#pragma warning (disable :6297)
#pragma warning (disable :6385)
#endif
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :26401)
#pragma warning (disable :26408)
#pragma warning (disable :26409)
#pragma warning (disable :26415)
#pragma warning (disable :26417)
#pragma warning (disable :26418)
#pragma warning (disable :26426)
#pragma warning (disable :26429)
#pragma warning (disable :26432)
#pragma warning (disable :26433)
#pragma warning (disable :26434)
#pragma warning (disable :26436)
#pragma warning (disable :26439)
#pragma warning (disable :26440)
#pragma warning (disable :26443)
#pragma warning (disable :26446)
#pragma warning (disable :26447)
#pragma warning (disable :26451)
#pragma warning (disable :26455)
#pragma warning (disable :26456)
#pragma warning (disable :26461)
#pragma warning (disable :26471)
#pragma warning (disable :26472)
#pragma warning (disable :26474)
#pragma warning (disable :26475)
#pragma warning (disable :26476)
#pragma warning (disable :26477)
#pragma warning (disable :26481)
#pragma warning (disable :26482)
#pragma warning (disable :26485)
#pragma warning (disable :26486)
#pragma warning (disable :26489)
#pragma warning (disable :26490)
#pragma warning (disable :26493)
#pragma warning (disable :26494)
#pragma warning (disable :26495)
#pragma warning (disable :26496)
#pragma warning (disable :26497)
#endif
#include <opencv2/opencv.hpp>
#pragma warning (pop)
#pragma endregion
#endif

#ifdef __CSC_DEPRECATED__
#ifdef _HAS_AUTO_PTR_ETC
#if (_HAS_AUTO_PTR_ETC == 0)
#undef random_shuffle
#pragma pop_macro ("random_shuffle")
#endif
#endif
#endif

#ifndef __OPENCV_ALL_HPP__
#ifndef OPENCV_ALL_HPP
#error "∑(っ°Д° ;)っ : require 'opencv2/opencv.hpp'"
#endif
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_opencv_world
#ifdef __CSC_DEBUG__
#define use_comment_lib_opencv_world "opencv_world" _STR_ (CV_MAJOR_VERSION) _STR_ (CV_MINOR_VERSION) _STR_ (CV_SUBMINOR_VERSION) "d.lib"
#else
#define use_comment_lib_opencv_world "opencv_world" _STR_ (CV_MAJOR_VERSION) _STR_ (CV_MINOR_VERSION) _STR_ (CV_SUBMINOR_VERSION) ".lib"
#endif
#endif
#pragma comment (lib ,use_comment_lib_opencv_world)
#undef use_comment_lib_opencv_world
#endif
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#define self to ()
#define implicit
#define popping
#define imports extern
#define exports
#endif

namespace CSC {
template <class>
class AbstractImage_Engine_OPENCV ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_BGR> :public AbstractImage<COLOR_BGR>::Abstract {
public:
	using NATIVE_TYPE = cv::Mat ;

public:
	AbstractImage_Engine_OPENCV () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	PACK<PTR<ARR<COLOR_BGR>> ,LENGTH[4]> layout (AnyRef<void> &_this) const override {
		PACK<PTR<ARR<COLOR_BGR>> ,LENGTH[4]> ret ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_BGR>> (r1.data) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		_DEBUG_ASSERT_ (r1.step.p != NULL) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_BGR)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &_this ,LENGTH cx ,LENGTH cy) const override {
		auto rax = cv::Mat (cv::Mat::zeros (VAR32 (cy) ,VAR32 (cx) ,CV_8UC3)) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC3) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC3) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		auto rax = AutoRef<std::vector<uchar>>::make () ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax.self ,r2) ;
		_DYNAMIC_ASSERT_ (rax->size () < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (rax->size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax.self[i]) ;
	}

	void load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		auto rax = cv::imread (_BUILDSTRS_<STRA> (file).raw ().self ,cv::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC3) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		const auto r2x = cv::imwrite (_BUILDSTRS_<STRA> (file).raw ().self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_BGRA> :public AbstractImage<COLOR_BGRA>::Abstract {
public:
	using NATIVE_TYPE = cv::Mat ;

public:
	AbstractImage_Engine_OPENCV () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	PACK<PTR<ARR<COLOR_BGRA>> ,LENGTH[4]> layout (AnyRef<void> &_this) const override {
		PACK<PTR<ARR<COLOR_BGRA>> ,LENGTH[4]> ret ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_BGRA>> (r1.data) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		_DEBUG_ASSERT_ (r1.step.p != NULL) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_BGRA)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &_this ,LENGTH cx ,LENGTH cy) const override {
		auto rax = cv::Mat (cv::Mat::zeros (VAR32 (cy) ,VAR32 (cx) ,CV_8UC4)) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC4) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC4) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		auto rax = AutoRef<std::vector<uchar>>::make () ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax.self ,r2) ;
		_DYNAMIC_ASSERT_ (rax->size () < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (rax->size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax.self[i]) ;
	}

	void load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		auto rax = cv::imread (_BUILDSTRS_<STRA> (file).raw ().self ,cv::IMREAD_UNCHANGED) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC4) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		const auto r2x = cv::imwrite (_BUILDSTRS_<STRA> (file).raw ().self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_GRAY> :public AbstractImage<COLOR_GRAY>::Abstract {
public:
	using NATIVE_TYPE = cv::Mat ;

public:
	AbstractImage_Engine_OPENCV () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	PACK<PTR<ARR<COLOR_GRAY>> ,LENGTH[4]> layout (AnyRef<void> &_this) const override {
		PACK<PTR<ARR<COLOR_GRAY>> ,LENGTH[4]> ret ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_GRAY>> (r1.data) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		_DEBUG_ASSERT_ (r1.step.p != NULL) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_GRAY)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &_this ,LENGTH cx ,LENGTH cy) const override {
		auto rax = cv::Mat (cv::Mat::zeros (VAR32 (cy) ,VAR32 (cx) ,CV_8UC1)) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		auto rax = AutoRef<std::vector<uchar>>::make () ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax.self ,r2) ;
		_DYNAMIC_ASSERT_ (rax->size () < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (rax->size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax.self[i]) ;
	}

	void load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		auto rax = cv::imread (_BUILDSTRS_<STRA> (file).raw ().self ,cv::IMREAD_GRAYSCALE) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_8UC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		const auto r2x = cv::imwrite (_BUILDSTRS_<STRA> (file).raw ().self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_GRAY32> :public AbstractImage<COLOR_GRAY32>::Abstract {
public:
	using NATIVE_TYPE = cv::Mat ;

public:
	AbstractImage_Engine_OPENCV () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	PACK<PTR<ARR<COLOR_GRAY32>> ,LENGTH[4]> layout (AnyRef<void> &_this) const override {
		PACK<PTR<ARR<COLOR_GRAY32>> ,LENGTH[4]> ret ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_GRAY32>> (r1.data) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		_DEBUG_ASSERT_ (r1.step.p != NULL) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_GRAY32)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &_this ,LENGTH cx ,LENGTH cy) const override {
		auto rax = cv::Mat (cv::Mat::zeros (VAR32 (cy) ,VAR32 (cx) ,CV_32FC1)) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_32FC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_32FC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		auto rax = AutoRef<std::vector<uchar>>::make () ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax.self ,r2) ;
		_DYNAMIC_ASSERT_ (rax->size () < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (rax->size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax.self[i]) ;
	}

	void load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		auto rax = cv::imread (_BUILDSTRS_<STRA> (file).raw ().self ,cv::IMREAD_REDUCED_GRAYSCALE_4) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_32FC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		const auto r2x = cv::imwrite (_BUILDSTRS_<STRA> (file).raw ().self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_GRAY64> :public AbstractImage<COLOR_GRAY64>::Abstract {
public:
	using NATIVE_TYPE = cv::Mat ;

public:
	AbstractImage_Engine_OPENCV () {
		_STATIC_ASSERT_ (_SIZEOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _SIZEOF_ (Interface)) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (REMOVE_CVR_TYPE<decltype (*this)>) == _ALIGNOF_ (Interface)) ;
	}

	PACK<PTR<ARR<COLOR_GRAY64>> ,LENGTH[4]> layout (AnyRef<void> &_this) const override {
		PACK<PTR<ARR<COLOR_GRAY64>> ,LENGTH[4]> ret ;
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_GRAY64>> (r1.data) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		_DEBUG_ASSERT_ (r1.step.p != NULL) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_GRAY64)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &_this ,LENGTH cx ,LENGTH cy) const override {
		auto rax = cv::Mat (cv::Mat::zeros (VAR32 (cy) ,VAR32 (cx) ,CV_64FC1)) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_64FC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void load_data (AnyRef<void> &_this ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_64FC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_data (const AnyRef<void> &_this ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		auto rax = AutoRef<std::vector<uchar>>::make () ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax.self ,r2) ;
		_DYNAMIC_ASSERT_ (rax->size () < VAR32_MAX) ;
		data = AutoBuffer<BYTE> (rax->size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax.self[i]) ;
	}

	void load_file (AnyRef<void> &_this ,const String<STR> &file) const override {
		auto rax = cv::imread (_BUILDSTRS_<STRA> (file).raw ().self ,cv::IMREAD_REDUCED_GRAYSCALE_8) ;
		_DYNAMIC_ASSERT_ (!rax.empty ()) ;
		_DYNAMIC_ASSERT_ (rax.type () == CV_64FC1) ;
		_this = AnyRef<NATIVE_TYPE>::make (std::move (rax)) ;
	}

	void save_file (const AnyRef<void> &_this ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = _this.rebind<NATIVE_TYPE> ().self ;
		const auto r1x = std::vector<VAR32> () ;
		auto &r2 = (param.exist ()) ? (param.rebind<std::vector<VAR32>> ().self) : r1x ;
		const auto r2x = cv::imwrite (_BUILDSTRS_<STRA> (file).raw ().self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r2x) ;
	}
} ;
} ;