#pragma once

#ifndef __CSC_IMAGES__
#error "∑(っ°Д° ;)っ : required csc_images.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#ifdef _HAS_AUTO_PTR_ETC
#if !(_HAS_AUTO_PTR_ETC)
#include <random>
#include <algorithm>

namespace std {
template <class _ARG>
inline void random_shuffle (const _ARG &ib ,const _ARG &ie) {
	shuffle (ib ,ie ,std::mt19937 (std::random_device () ())) ;
}
} ;
#endif
#endif
#endif

#ifndef __CSC_STRICTHPPINL__
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4265) //@info:warning C4265: “xxx”: 类有虚函数，但析构函数不是虚拟的
#pragma warning (disable :4266) //@info:warning C4266: “xxx”: 不重写基“xxx”中的虚拟成员函数；函数被隐藏
#pragma warning (disable :4435) //@info:warning C4435: “xxx”: /vd2 下的对象布局将因虚拟基“xxx”而更改
#pragma warning (disable :4711) //@info:warning C4711: 为自动内联扩展选定了函数“xxx”
#pragma warning (disable :4819) //@info:warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
#pragma warning (disable :4946) //@info:warning C4946: reinterpret_cast 在相关类之间使用: “xxx”和“xxx”
#pragma warning (disable: 4996) //@info:warning C4996: 'xxx': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.)
#pragma warning (disable :6011) //@info:warning C6011: 取消对 NULL 指针“xxx”的引用。查看第 xxx 行以找出可能会发生此情况的前一位置
#pragma warning (disable :6201) //@info:warning C6201: 索引“xxx”超出了“xxx”至“xxx”的有效范围(对于可能在堆栈中分配的缓冲区“xxx”)。
#pragma warning (disable :6294) //@info:warning C6294: for 循环定义错误: 初始条件不满足测试。 循环体未执行。
#pragma warning (disable :6297) //@info:warning C6297: 算术溢出: 对 32 位值进行移位操作，然后强制转换为 64 位值。 结果可能不是预期值。
#pragma warning (disable :6385) //@info:warning C6385: 从“xxx”中读取的数据无效: 可读大小为“xxx”个字节，但可能读取了“xxx”个字节。
#endif
#include <opencv2/opencv.hpp>
#pragma warning (pop)
#pragma endregion
#ifdef __CSC_DEBUG__
//@error:opencv_worldxxxd.lib would cause memory leaks once linked
#pragma comment (lib ,"opencv_world" _STR_ (CV_MAJOR_VERSION) _STR_ (CV_MINOR_VERSION) _STR_ (CV_SUBMINOR_VERSION) "d.lib")
#else
#pragma comment (lib ,"opencv_world" _STR_ (CV_MAJOR_VERSION) _STR_ (CV_MINOR_VERSION) _STR_ (CV_SUBMINOR_VERSION) ".lib")
#endif
#endif

#if !defined (__OPENCV_ALL_HPP__) && !defined (OPENCV_ALL_HPP)
#error "∑(っ°Д° ;)っ : required opencv2/opencv.hpp"
#endif

namespace CSC {
template <class>
class AbstractImage_Engine_OPENCV ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_BGR> :public AbstractImage<COLOR_BGR>::Engine {
private:
	friend Singleton<AbstractImage_Engine_OPENCV> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		image.rebind<cv::Mat> ().self = cv::Mat (VAR32 (cy) ,VAR32 (cx) ,CV_8UC3) ;
	}

	PACK<PTR<ARR<COLOR_BGR>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_BGR>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_BGR>> (&r1 ,_ADRESS_ (r1.data)) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_BGR)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () > 0 && data.size () < VAR32_MAX) ;
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::ImreadModes::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_8UC3) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		auto rax = std::vector<uchar> () ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax ,r2) ;
		data = AutoBuffer<BYTE> (rax.size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax[i]) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imread (_BUILDSTRS_<STRA> (file).self ,cv::ImreadModes::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_8UC3) ;
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		const auto r1x = cv::imwrite (_BUILDSTRS_<STRA> (file).self ,r1 ,r2) ;
		_DEBUG_ASSERT_ (r1x) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_OPENCV () {}

	~AbstractImage_Engine_OPENCV () noexcept override {}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_BGRA> :public AbstractImage<COLOR_BGRA>::Engine {
private:
	friend Singleton<AbstractImage_Engine_OPENCV> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		image.rebind<cv::Mat> ().self = cv::Mat (VAR32 (cy) ,VAR32 (cx) ,CV_8UC4) ;
	}

	PACK<PTR<ARR<COLOR_BGRA>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_BGRA>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_BGRA>> (&r1 ,_ADRESS_ (r1.data)) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_BGRA)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () > 0 && data.size () < VAR32_MAX) ;
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::ImreadModes::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_8UC4) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		auto rax = std::vector<uchar> () ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax ,r2) ;
		data = AutoBuffer<BYTE> (rax.size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax[i]) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imread (_BUILDSTRS_<STRA> (file).self ,cv::ImreadModes::IMREAD_UNCHANGED) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_8UC4) ;
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		const auto r1x = cv::imwrite (_BUILDSTRS_<STRA> (file).self ,r1 ,r2) ;
		_DEBUG_ASSERT_ (r1x) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_OPENCV () {}

	~AbstractImage_Engine_OPENCV () noexcept override {}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_GRAY> :public AbstractImage<COLOR_GRAY>::Engine {
private:
	friend Singleton<AbstractImage_Engine_OPENCV> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		image.rebind<cv::Mat> ().self = cv::Mat (VAR32 (cy) ,VAR32 (cx) ,CV_8U) ;
	}

	PACK<PTR<ARR<COLOR_GRAY>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_GRAY>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_GRAY>> (&r1 ,_ADRESS_ (r1.data)) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_GRAY)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () > 0 && data.size () < VAR32_MAX) ;
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::ImreadModes::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_8U) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		auto rax = std::vector<uchar> () ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax ,r2) ;
		data = AutoBuffer<BYTE> (rax.size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax[i]) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imread (_BUILDSTRS_<STRA> (file).self ,cv::ImreadModes::IMREAD_GRAYSCALE) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_8U) ;
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		const auto r1x = cv::imwrite (_BUILDSTRS_<STRA> (file).self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_OPENCV () {}

	~AbstractImage_Engine_OPENCV () noexcept override {}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_GRAY32> :public AbstractImage<COLOR_GRAY32>::Engine {
private:
	friend Singleton<AbstractImage_Engine_OPENCV> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		image.rebind<cv::Mat> ().self = cv::Mat (VAR32 (cy) ,VAR32 (cx) ,CV_32F) ;
	}

	PACK<PTR<ARR<COLOR_GRAY32>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_GRAY32>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_GRAY32>> (&r1 ,_ADRESS_ (r1.data)) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_GRAY32)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () > 0 && data.size () < VAR32_MAX) ;
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::ImreadModes::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_32F) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		auto rax = std::vector<uchar> () ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax ,r2) ;
		data = AutoBuffer<BYTE> (rax.size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax[i]) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imread (_BUILDSTRS_<STRA> (file).self ,cv::ImreadModes::IMREAD_REDUCED_GRAYSCALE_4) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_32F) ;
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		const auto r1x = cv::imwrite (_BUILDSTRS_<STRA> (file).self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_OPENCV () {}

	~AbstractImage_Engine_OPENCV () noexcept override {}
} ;

template <>
class AbstractImage_Engine_OPENCV<COLOR_GRAY64> :public AbstractImage<COLOR_GRAY64>::Engine {
private:
	friend Singleton<AbstractImage_Engine_OPENCV> ;

public:
	void layout (AnyRef<void> &image ,LENGTH cx ,LENGTH cy) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		image.rebind<cv::Mat> ().self = cv::Mat (VAR32 (cy) ,VAR32 (cx) ,CV_64F) ;
	}

	PACK<PTR<ARR<COLOR_GRAY64>> ,ARRAY4<LENGTH>> watch (AnyRef<void> &image) const override {
		PACK<PTR<ARR<COLOR_GRAY64>> ,ARRAY4<LENGTH>> ret ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		ret.P1 = &_LOAD_<ARR<COLOR_GRAY64>> (&r1 ,_ADRESS_ (r1.data)) ;
		ret.P2[0] = LENGTH (r1.cols) ;
		ret.P2[1] = LENGTH (r1.rows) ;
		ret.P2[2] = LENGTH (r1.step.p[0] / _SIZEOF_ (COLOR_GRAY64)) ;
		ret.P2[3] = 0 ;
		return std::move (ret) ;
	}

	void load_data (AnyRef<void> &image ,const AutoBuffer<BYTE> &data) const override {
		_DYNAMIC_ASSERT_ (data.size () > 0 && data.size () < VAR32_MAX) ;
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imdecode (cv::_InputArray (data.self ,VAR32 (data.size ())) ,cv::ImreadModes::IMREAD_COLOR) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_64F) ;
	}

	void save_data (const AnyRef<void> &image ,AutoBuffer<BYTE> &data ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		auto rax = std::vector<uchar> () ;
		cv::imencode (_PCSTRA_ ("bmp") ,r1 ,rax ,r2) ;
		data = AutoBuffer<BYTE> (rax.size ()) ;
		for (INDEX i = 0 ; i < data.size () ; i++)
			data[i] = BYTE (rax[i]) ;
	}

	void load_file (AnyRef<void> &image ,const String<STR> &file) const override {
		if (!image.exist ())
			image = AnyRef<cv::Mat>::make () ;
		auto &r1 = image.rebind<cv::Mat> ().self ;
		r1 = cv::imread (_BUILDSTRS_<STRA> (file).self ,cv::ImreadModes::IMREAD_REDUCED_GRAYSCALE_8) ;
		_DYNAMIC_ASSERT_ (!r1.empty () && r1.type () == CV_64F) ;
	}

	void save_file (const AnyRef<void> &image ,const String<STR> &file ,const AnyRef<void> &param) const override {
		auto &r1 = image.rebind<cv::Mat> ().self ;
		auto &r2 = param.rebind<std::vector<int>> ().self ;
		const auto r1x = cv::imwrite (_BUILDSTRS_<STRA> (file).self ,r1 ,r2) ;
		_DYNAMIC_ASSERT_ (r1x) ;
	}

private:
	AbstractImage_Engine_OPENCV () {}

	~AbstractImage_Engine_OPENCV () noexcept override {}
} ;
} ;