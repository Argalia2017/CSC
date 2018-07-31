#pragma once

#ifndef __CSC__
#define __CSC__
#endif

#ifdef _DEBUG
#define __CSC_DEBUG__
#endif

#ifdef _UNITTEST
#define __CSC_UNITTEST__
#endif

#ifdef _DEPRECATED
#define __CSC_DEPRECATED__
#endif

#ifdef _STRICTHPPINL
#define __CSC_STRICTHPPINL__
#endif

#ifdef __clang__
#define __CSC_COMPILER_CLANG__
#elif defined __GNUC__
#define __CSC_COMPILER_GNUC__
#elif defined _MSC_VER
#define __CSC_COMPILER_MSVC__
#endif

#if defined (__linux__) || defined (__linux)
#define __CSC_SYSTEM_LINUX__
#elif !defined (SAG_COM) && (defined (WIN32) || defined (_WIN32) || defined (__WIN32__))
#define __CSC_SYSTEM_WINDOWS__
#endif

#if defined (_M_IX86)
#define __CSC_PLATFORM_X86__
#elif defined (_M_AMD64)
#define __CSC_PLATFORM_X64__
#elif defined (_M_ARM)
#define __CSC_PLATFORM_ARM__
#elif defined (_M_ARM64)
#define __CSC_PLATFORM_ARM64__
#elif defined (_M_IA64)
#define __CSC_PLATFORM_IA64__
#endif

#ifdef _WINEXE
#define __CSC_TARGET_EXE__
#elif defined _WINDLL
#define __CSC_TARGET_DLL__
#elif defined _WINLIB
#define __CSC_TARGET_LIB__
#else
#define __CSC_TARGET_EXE__
#endif

#ifdef _WIN64
#define __CSC_CONFIG_VAR64__
#define __CSC_CONFIG_VAL64__
#elif defined _WIN32
#define __CSC_CONFIG_VAR32__
#define __CSC_CONFIG_VAL32__
#else
#define __CSC_CONFIG_VAR64__
#define __CSC_CONFIG_VAL64__
#endif

#ifdef _UNICODE
#define __CSC_CONFIG_STRW__
#elif _MBCS
#define __CSC_CONFIG_STRA__
#else
#define __CSC_CONFIG_STRA__
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4619) //@info:warning C4619 : #pragma warning : 无警告编号“xxx”
#pragma warning (disable :4100) //@info:warning C4100: “xxx”: 未引用的形参
#pragma warning (disable :4355) //@info:warning C4355: “this”: 用于基成员初始值设定项列表
#pragma warning (disable :4365) //@info:warning C4365: “xxx”: 从“xxx”转换到“xxx”，有符号/无符号不匹配
#pragma warning (disable :4464) //@info:warning C4464: 相对包含路径包括 ".."
#pragma warning (disable :4505) //@info:warning C4505: “xxx”: 未引用的本地函数已移除
#pragma warning (disable :4514) //@info:warning C4514: “xxx”: 未引用的内联函数已移除
#pragma warning (disable :4571) //@info:warning C4571: 信息: 自 Visual C++ 7.1 之后，catch(...) 语义发生了变化；不再捕获结构化的异常(SEH)
#pragma warning (disable :4574) //@info:warning C4574: “xxx”被定义为“0”: 您是否希望使用“#if xxx”?
#pragma warning (disable :4668) //@info:warning C4668: 没有将“xxx”定义为预处理器宏，用“0”替换“#if/#elif”
#pragma warning (disable :4710) //@info:warning C4710: “xxx”: 函数未内联
#pragma warning (disable :4711) //@info:warning C4711: 为自动内联扩展选定了函数“xxx”
#pragma warning (disable :4774) //@info:warning C4774: “xxx”: 参数 xxx 中应存在的格式字符串不为字符串字面量
#pragma warning (disable :4324) //@info:warning C4324: “xxx”: 由于对齐说明符，结构被填充
#pragma warning (disable :4820) //@info:warning C4820: “xxx”:“xxx”字节填充添加在 数据成员“xxx”后
#pragma warning (disable :4623) //@info:warning C4623: “xxx”: 已将默认构造函数隐式定义为“已删除”
#pragma warning (disable :4624) //@info:warning C4624: “xxx”: 已将析构函数隐式定义为“已删除”
#pragma warning (disable :4625) //@info:warning C4625: “xxx”: 已将复制构造函数隐式定义为“已删除”
#pragma warning (disable :4626) //@info:warning C4626: “xxx”: 已将对齐运算符隐式定义为“已删除”
#pragma warning (disable :5026) //@info:warning C5026: “xxx”: 已将移动构造函数隐式定义为“已删除”
#pragma warning (disable :5027) //@info:warning C5027: “xxx”: 已将移动赋值运算符隐式定义为“已删除”
#pragma warning (disable :5045) //@info:warning C5045: 如果指定了 /Qspectre 开关，编译器会插入内存负载的 Spectre 缓解
//@info see also 'https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md'
#pragma warning (disable :26439) //@info:warning C26439: This kind of function may not throw. Declare it 'noexcept' (f.6).
#pragma warning (disable :26495) //@info:warning C26495: Variable 'xxx' is uninitialized. Always initialize a member variable (type.6).
#pragma warning (disable :26434) //@info:warning C26434 : Function 'xxx' hides a non - virtual function 'xxx' (c.128).
#endif

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <limits>
#include <type_traits>
#include <initializer_list>
#include <new>
#include <exception>
#include <typeinfo>
#include <utility>

namespace CSC {
#define M_DATE __DATE__
#define M_TIME __TIME__
#define M_FILE __FILE__
#define M_LINE __LINE__

#ifdef __CSC_COMPILER_MSVC__
#define M_FUNC __FUNCSIG__
#elif defined __CSC_COMPILER_GNUC__
#define M_FUNC __PRETTY_FUNCTION__
#elif defined __CSC_COMPILER_CLANG__
#define M_FUNC __PRETTY_FUNCTION__
#else
#define M_FUNC __FUNCTION__
#endif

#define _UNW_X_(...) __VA_ARGS__
#define _UNW_(...) _UNW_X_(__VA_ARGS__)
#define _STR_X_(...) #__VA_ARGS__
#define _STR_(...) _STR_X_(__VA_ARGS__)
#define _CAT_X_(arg1 ,arg2) arg1 ## arg2
#define _CAT_(arg1 ,arg2) _CAT_X_(arg1 ,arg2)

#define _STATIC_ASSERT_(...) static_assert ((_UNW_ (__VA_ARGS__)) ,"static_assert failed : " _STR_ (__VA_ARGS__))

#define _STATIC_WARNING_(...) do { (void) (_UNW_ (__VA_ARGS__)) ; } while (FALSE)

#ifdef __CSC_DEBUG__
#ifdef __CSC_COMPILER_CLANG__
#define _DEBUG_ASSERT_ ([] (CSC::BOOL condition) { assert (condition) ; })
#else
#ifdef __CSC_SYSTEM_WINDOWS__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) __debugbreak () ; } while (FALSE)
#elif defined __CSC_SYSTEM_LINUX__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) __builtin_trap () ; } while (FALSE)
#else
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) assert (FALSE) ; } while (FALSE)
#endif
#endif
#else
#define _DEBUG_ASSERT_(...) do {} while (FALSE) ;
#endif

#ifdef __CSC_COMPILER_CLANG__
#define _DYNAMIC_ASSERT_ ([] (CSC::BOOL condition) { if (!condition) throw CSC::Exception::make ("dynamic_assert failed : clang-unknown : at " ,M_FUNC ," in " M_FILE " ," _STR_ (M_LINE)) ; })
#elif defined __CSC_COMPILER_GNUC__
#define _DYNAMIC_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception::make ("dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " M_FILE " ," _STR_ (M_LINE)) ; } while (FALSE)
#else
#define _DYNAMIC_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception::make ("dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," _STR_ (M_LINE)) ; } while (FALSE)
#endif

#ifdef __CSC_UNITTEST__
#ifdef __CSC_COMPILER_CLANG__
#define _UNITTEST_ASSERT_ ([] (CSC::BOOL condition) { if (!condition) CSC::_UNITTEST_ASSERT_HANDLER_ (CSC::Exception::make ("unittest_assert failed : clang-unknown : at " ,M_FUNC ," in " M_FILE " ," _STR_ (M_LINE)).what ()) ; })
#elif defined __CSC_COMPILER_GNUC__
#define _UNITTEST_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) CSC::_UNITTEST_ASSERT_HANDLER_ (CSC::Exception::make ("unittest_assert failed : " ,_STR_ (__VA_ARGS__) ," : at " ,M_FUNC ," in " ,M_FILE ," ," ,_STR_ (M_LINE)).what ()) ; } while (FALSE)
#else
#define _UNITTEST_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) CSC::_UNITTEST_ASSERT_HANDLER_ ("unittest_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," _STR_ (M_LINE)) ; } while (FALSE)
#endif
#else
#define _UNITTEST_ASSERT_(...) do {} while (FALSE) ;
#endif

#define ANONYMOUS _CAT_ (anonymous_ ,M_LINE)

#ifdef __CSC__
#ifdef self
#error "∑(っ°Д° ;)っ : 'self' has been already defined"
#endif
#define self to ()

#ifdef implicit
#error "∑(っ°Д° ;)っ : 'implicit' has been already defined"
#endif
#define implicit

#ifdef export
#error "∑(っ°Д° ;)っ : 'export' has been already defined"
#endif
#define export

#ifdef import
#error "∑(っ°Д° ;)っ : 'import' has been already defined"
#endif
#define import extern

#ifdef popping
#error "∑(っ°Д° ;)っ : 'popping' has been already defined"
#endif
//@error:side-effect right-expression may change left-expression impliedly
#define popping
#endif

using BOOL = bool ;

#ifdef FALSE
#undef FALSE
#endif
#define FALSE false
#ifdef TRUE
#undef TRUE
#endif
#define TRUE true

using VAR32 = std::int32_t ;
using VAR64 = std::int64_t ;

static constexpr VAR32 VAR32_MAX = VAR32 (2147483647) ;
static constexpr VAR32 VAR32_MIN = ~VAR32_MAX ;
static constexpr VAR64 VAR64_MAX = VAR64 (9223372036854775807) ;
static constexpr VAR64 VAR64_MIN = ~VAR64_MAX ;

#ifdef __CSC_CONFIG_VAR32__
using VAR = VAR32 ;

static constexpr VAR VAR_MAX = VAR32_MAX ;
static constexpr VAR VAR_MIN = VAR32_MIN ;
#elif defined __CSC_CONFIG_VAR64__
using VAR = VAR64 ;

static constexpr VAR VAR_MAX = VAR64_MAX ;
static constexpr VAR VAR_MIN = VAR64_MIN ;
#endif

using VARX = long ;
using VARY = unsigned long ;

static constexpr VAR VAR_ZERO = VAR (+0) ;
static constexpr VAR VAR_NONE = VAR (-1) ;
static constexpr VAR VAR_USED = VAR (-2) ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

enum EFLAG :VAR ;

#define _SIZEOF_(...) (CSC::LENGTH (sizeof (_UNW_ (__VA_ARGS__))))
#define _ALIGNOF_(...) (CSC::LENGTH (alignof (_UNW_ (__VA_ARGS__))))
#define _SIZESOF_(...) (CSC::LENGTH (sizeof... (_UNW_ (__VA_ARGS__))))

#define _COUNTOF_(...) (CSC::COUNTOF_TRAITS_TYPE<CSC::REMOVE_CVR_TYPE<decltype (_UNW_ (__VA_ARGS__))>>::value)

using VAL32 = float ;
using VAL64 = double ;

static constexpr VAL32 VAL32_MAX = VAL32 (3.402823466E+38) ;
static constexpr VAL32 VAL32_MIN = VAL32 (1.175494351E-38) ;
static constexpr VAL32 VAL32_EPS = VAL32 (1.192092896E-07) ;
static constexpr VAL64 VAL64_MAX = VAL64 (1.7976931348623158E+308) ;
static constexpr VAL64 VAL64_MIN = VAL64 (2.2250738585072014E-308) ;
static constexpr VAL64 VAL64_EPS = VAL64 (2.2204460492503131E-016) ;
static constexpr VAL32 VAL32_INF = std::numeric_limits<VAL32>::infinity () ;
static constexpr VAL32 VAL32_NAN = std::numeric_limits<VAL32>::quiet_NaN () ;
static constexpr VAL32 VAL32_IND = -VAL32_NAN ;
static constexpr VAL64 VAL64_INF = std::numeric_limits<VAL64>::infinity () ;
static constexpr VAL64 VAL64_NAN = std::numeric_limits<VAL64>::quiet_NaN () ;
static constexpr VAL64 VAL64_IND = -VAL64_NAN ;

#ifdef __CSC_CONFIG_VAL32__
using VAL = VAL32 ;

static constexpr VAL VAL_MAX = VAL32_MAX ;
static constexpr VAL VAL_MIN = VAL32_MIN ;
static constexpr VAL VAL_EPS = VAL32_EPS ;
static constexpr VAL VAL_INF = VAL32_INF ;
static constexpr VAL VAL_NAN = VAL32_NAN ;
static constexpr VAL VAL_IND = VAL32_IND ;
#elif defined __CSC_CONFIG_VAL64__
using VAL = VAL64 ;

static constexpr VAL VAL_MAX = VAL64_MAX ;
static constexpr VAL VAL_MIN = VAL64_MIN ;
static constexpr VAL VAL_EPS = VAL64_EPS ;
static constexpr VAL VAL_INF = VAL64_INF ;
static constexpr VAL VAL_NAN = VAL64_NAN ;
static constexpr VAL VAL_IND = VAL64_IND ;
#endif

using VALX = long double ;

static constexpr VALX VALX_E = VALX (2.71828182845904523536) ;
static constexpr VALX VALX_PI = VALX (3.14159265358979323846) ;
static constexpr VALX VALX_SQRT2 = VALX (1.41421356237309504880) ;
static constexpr VALX VALX_LOG2E = VALX (1.44269504088896340736) ;
static constexpr VALX VALX_LOG10E = VALX (0.434294481903251827651) ;
static constexpr VALX VALX_LOGE2 = VALX (0.693147180559945309417) ;
static constexpr VALX VALX_LOGE10 = VALX (2.30258509299404568402) ;

inline namespace STD {
template <class _ARG>
inline constexpr _ARG _ABS_ (const _ARG &arg) {
	return arg < 0 ? -arg : +arg ;
}

template <class _ARG>
inline constexpr _ARG _SQE_ (const _ARG &arg) {
	return arg * arg ;
}

template <class _ARG>
inline constexpr const _ARG &_MIN_ (const _ARG &arg1 ,const _ARG &arg2) {
	return !(arg2 < arg1) ? arg1 : arg2 ;
}

template <class _ARG>
inline constexpr const _ARG &_MAX_ (const _ARG &arg1 ,const _ARG &arg2) {
	return !(arg1 < arg2) ? arg1 : arg2 ;
}
} ;

template <class TYPE>
using DEF = TYPE ;

template <class TYPE>
using PTR = TYPE * ;

template <class TYPE>
using ARR = TYPE[] ;

#ifdef VOID
#undef VOID
#endif
using VOID = void ;

#ifdef NULL
#undef NULL
#endif
#define NULL nullptr

struct NONE ;

using BYTE = std::uint8_t ;
using WORD = std::uint16_t ;
using CHAR = std::uint32_t ;
using DATA = std::uint64_t ;

struct __uint128_t {
	alignas (16) DEF<BYTE[16]> unused ;
} ;

using MEGA = __uint128_t ;

using STRU8 = BYTE ;
using STRU16 = WORD ;
using STRU32 = CHAR ;

#define _PCSTRU8_(arg) CSC::_CAST_<const CSC::STRU8[_COUNTOF_ (_CAT_ (u8 ,arg))]> (_CAT_ (u8 ,arg))
#define _PCSTRU16_(arg) CSC::_CAST_<const CSC::STRU16[_COUNTOF_ (_CAT_ (u ,arg))]> (_CAT_ (u ,arg))
#define _PCSTRU32_(arg) CSC::_CAST_<const CSC::STRU32[_COUNTOF_ (_CAT_ (U ,arg))]> (_CAT_ (U ,arg))

using STRA = char ;
using STRW = wchar_t ;

#define _PCSTRA_(arg) CSC::_CAST_<const CSC::STRA[_COUNTOF_ (arg)]> (arg)
#define _PCSTRW_(arg) CSC::_CAST_<const CSC::STRW[_COUNTOF_ (_CAT_ (L ,arg))]> (_CAT_ (L ,arg))

#ifdef __CSC_CONFIG_STRA__
using STR = STRA ;

#define _PCSTR_ _PCSTRA_
#elif defined __CSC_CONFIG_STRW__
using STR = STRW ;

#define _PCSTR_ _PCSTRW_
#endif

template <VAR _VAL>
struct ARGC :public std::integral_constant<VAR ,_VAL> {} ;

using EXPANDING_SIZE = ARGC<128> ;
using RECURSIVE_SIZE = ARGC<256> ;
using SHORTSTRING_SIZE = ARGC<511> ;
using LONGSTRING_SIZE = ARGC<4095> ;
using HUGESTRING_SIZE = ARGC<4194303> ;

template <VAR...>
struct ARGCS ;

template <>
struct ARGCS<> :public ARGC<0> {} ;

template <VAR _VAL1 ,VAR..._VALS>
struct ARGCS<_VAL1 ,_VALS...> :public ARGC<_VAL1 + ARGCS<_VALS...>::value> {} ;

template <class>
struct ARGV {} ;

template <>
struct ARGV<ARGC<0>> {} ;

template <LENGTH _VAL>
struct ARGV<ARGC<_VAL>> :public ARGV<ARGC<_VAL - 1>> {} ;

static constexpr ARGV<ARGC<0>> ARGV0 {} ;
static constexpr ARGV<ARGC<1>> ARGV1 {} ;
static constexpr ARGV<ARGC<2>> ARGV2 {} ;
static constexpr ARGV<ARGC<3>> ARGV3 {} ;
static constexpr ARGV<ARGC<4>> ARGV4 {} ;
static constexpr ARGV<ARGC<5>> ARGV5 {} ;
static constexpr ARGV<ARGC<6>> ARGV6 {} ;
static constexpr ARGV<ARGC<7>> ARGV7 {} ;
static constexpr ARGV<ARGC<8>> ARGV8 {} ;
static constexpr ARGV<ARGC<9>> ARGV9 {} ;

static constexpr ARGV<NONE> ARGV_NONE {} ;
static constexpr ARGV<VOID> ARGV_VOID {} ;

template <class...>
struct ARGVS {} ;

template <class ,BOOL ,BOOL ,BOOL>
struct CONSTRUCTIBLE {} ;

template <class TYPE>
struct TEMP {
	alignas (TYPE) DEF<BYTE[_SIZEOF_ (TYPE)]> unused ;
} ;

template <class...>
struct PACK ;

template <>
struct PACK<> {} ;

template <class TYPE1>
struct PACK<TYPE1> {
	TYPE1 P1 ;
} ;

template <class TYPE1 ,class TYPE2>
struct PACK<TYPE1 ,TYPE2> {
	TYPE1 P1 ;
	TYPE2 P2 ;
} ;

template <class TYPE1 ,class TYPE2 ,class TYPE3>
struct PACK<TYPE1 ,TYPE2 ,TYPE3> {
	TYPE1 P1 ;
	TYPE2 P2 ;
	TYPE3 P3 ;
} ;

template <BOOL _VAL>
using ENABLE_TYPE = typename std::enable_if<_VAL>::type ;

template <class _ARG>
using RESULT_TYPE = typename std::result_of<_ARG>::type ;

template <BOOL _VAL ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = typename std::conditional<_VAL ,_ARG1 ,_ARG2>::type ;

template <class _ARG>
using REMOVE_REFERENCE_TYPE = typename std::remove_reference<_ARG>::type ;

template <class _ARG>
using REMOVE_CONST_TYPE = typename std::remove_const<_ARG>::type ;

template <class _ARG>
using REMOVE_VOLATILE_TYPE = typename std::remove_volatile<_ARG>::type ;

template <class _ARG>
using REMOVE_POINTER_TYPE = typename std::remove_pointer<_ARG>::type ;

template <class _ARG>
using REMOVE_EXTEND_TYPE = typename std::remove_extent<_ARG>::type ;

template <class _ARG>
using REMOVE_CVR_TYPE = REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<REMOVE_REFERENCE_TYPE<_ARG>>> ;

namespace U {
template <class _ARG>
struct COUNTOF_TRAITS {
	using TYPE = ARGC<1> ;
} ;

template <class _ARG ,LENGTH _VAL>
struct COUNTOF_TRAITS<_ARG[_VAL]> {
	using TYPE = ARGC<_VAL> ;
} ;
} ;

template <class _ARG>
using COUNTOF_TRAITS_TYPE = typename U::COUNTOF_TRAITS<_ARG>::TYPE ;

namespace U {
template <class _ARG>
struct TEMP_TRAITS {
	using TYPE = _ARG ;
} ;

template <class _ARG>
struct TEMP_TRAITS<TEMP<_ARG>> {
	using TYPE = _ARG ;
} ;
} ;

template <class _ARG>
using REMOVE_TEMP_TYPE = typename U::TEMP_TRAITS<_ARG>::TYPE ;

namespace U {
template <class>
struct MEMBER_POINTER_TRAITS ;

template <class _ARG1 ,class _ARG2>
struct MEMBER_POINTER_TRAITS<_ARG1 _ARG2::*> {
	using TYPE = _ARG1 ;
} ;
} ;

template <class _ARG>
using REMOVE_MEMBER_POINTER_TYPE = typename U::MEMBER_POINTER_TRAITS<_ARG>::TYPE ;

namespace U {
template <class>
struct MEMBEROF_TRAITS ;

template <class _ARG1 ,class _ARG2>
struct MEMBEROF_TRAITS<_ARG1 _ARG2::*> {
	using TYPE = _ARG2 ;
} ;
} ;

template <class _ARG>
using MEMBEROF_TRAITS_TYPE = typename U::MEMBEROF_TRAITS<_ARG>::TYPE ;

namespace U {
template <class _ARG>
struct FORWARD_TRAITS {
	using TYPE = const _ARG & ;
} ;

template <class _ARG>
struct FORWARD_TRAITS<_ARG &> {
	using TYPE = _ARG & ;
} ;

template <class _ARG>
struct FORWARD_TRAITS<_ARG &&> {
	using TYPE = _ARG ;
} ;
} ;

template <class _ARG>
using FORWARD_TRAITS_TYPE = typename U::FORWARD_TRAITS<_ARG>::TYPE ;

template <class _RET>
inline _RET &_NULL_ () noexcept {
	static constexpr PTR<_RET> M_NULL = NULL ;
	return *M_NULL ;
}

template <class _ARG>
inline LENGTH _ADRESS_ (PTR<_ARG> arg) noexcept {
	return LENGTH (arg) ;
}

namespace U {
template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &> {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> & ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,_ARG2 &&> {
	using TYPE = REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const _ARG2 &&> {
	using TYPE = const REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,volatile _ARG2 &&> {
	using TYPE = volatile REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;

template <class _ARG1 ,class _ARG2>
struct CAST_TRAITS<_ARG1 ,const volatile _ARG2 &&> {
	using TYPE = const volatile REMOVE_REFERENCE_TYPE<_ARG1> && ;
} ;
} ;

template <class _ARG1 ,class _ARG2>
using CAST_TRAITS_TYPE = typename U::CAST_TRAITS<_ARG1 ,_ARG2>::TYPE ;

//@error:not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG>
inline CAST_TRAITS_TYPE<_RET ,_ARG> &_CAST_ (_ARG &arg) noexcept {
	_STATIC_ASSERT_ (!std::is_pointer<_RET>::value || !std::is_scalar<_ARG>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_ARG>::value || !std::is_scalar<_RET>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG) % _ALIGNOF_ (_RET) == 0) ;
	return reinterpret_cast<CAST_TRAITS_TYPE<_RET ,_ARG> &> (arg) ;
}

namespace U {
template <class ,class ,class>
struct LOAD_TRAITS {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1 ,class _ARG2>
struct LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<1>> {
	using TYPE = typename LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<2>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
struct LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<2>> {
	using TYPE = typename LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<3>>::TYPE ;
} ;

template <class _ARG>
struct LOAD_TRAITS<_ARG ,_ARG ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<ARR<_ARG> ,_ARG ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG ,LENGTH _VAL>
struct LOAD_TRAITS<ARR<_ARG> ,_ARG[_VAL] ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG ,LENGTH _VAL>
struct LOAD_TRAITS<_ARG[_VAL] ,ARR<_ARG> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<STRA> ,ARR<BYTE> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRA> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRW> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,VOID ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<NONE ,_ARG ,ARGC<2>> {
	using TYPE = ARGC<std::is_class<_ARG>::value> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<_ARG ,NONE ,ARGC<2>> {
	using TYPE = ARGC<std::is_class<_ARG>::value> ;
} ;

template <class _ARG1 ,class _ARG2>
struct LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<3>> {
	using TYPE = ARGC<std::is_pod<_ARG1>::value && std::is_pod<_ARG2>::value && _SIZEOF_ (_ARG2) >= _SIZEOF_ (_ARG1)> ;
} ;
} ;

template <class _ARG1 ,class _ARG2>
using LOAD_TRATIS_TYPE = typename U::LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<1>>::TYPE ;

//@error:not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG>
inline CAST_TRAITS_TYPE<_RET ,_ARG> &_LOAD_ (PTR<_ARG> adress) noexcept {
	_STATIC_ASSERT_ (LOAD_TRATIS_TYPE<_RET ,REMOVE_CVR_TYPE<_ARG>>::value) ;
	_DEBUG_ASSERT_ (adress != NULL) ;
	_DEBUG_ASSERT_ (_ADRESS_ (adress) % _ALIGNOF_ (_RET) == 0) ;
	return *reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG>>> (adress) ;
}

//@error:not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG>
inline CAST_TRAITS_TYPE<_RET ,_ARG> &_LOAD_ (PTR<_ARG> ,LENGTH adress) noexcept {
	_DEBUG_ASSERT_ (adress != 0) ;
	_DEBUG_ASSERT_ (adress % _ALIGNOF_ (_RET) == 0) ;
	return *reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG>>> (adress) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (DEF<_ARG1 _ARG2::*> arg1 ,PTR<_ARG3> arg2) noexcept {
	_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG3> ,_ARG1>::value) ;
	return _LOAD_<CAST_TRAITS_TYPE<_ARG2 ,_ARG3>> (arg2 ,_ADRESS_ (arg2) - _ADRESS_ (&(_NULL_<_ARG2> ().*arg1))) ;
}

//@error:undefined behavior but accepted in ['MSCV' ,'GNUC' ,'CLANG']
template <class _ARG ,class = CSC::ENABLE_TYPE<std::is_rvalue_reference<_ARG &&>::value>>
inline REMOVE_REFERENCE_TYPE<_ARG> &_XVALUE_ (_ARG &&arg) noexcept {
	_STATIC_ASSERT_ (std::is_pod<_ARG>::value) ;
	return arg ;
}

template <class _RET ,class = CSC::ENABLE_TYPE<std::is_lvalue_reference<_RET>::value>>
inline REMOVE_REFERENCE_TYPE<_RET> &_XVALUE_ (REMOVE_REFERENCE_TYPE<_RET> &arg) noexcept {
	return arg ;
}

template <class _ARG>
inline void _ZERO_ (_ARG &arg) noexcept {
	_STATIC_ASSERT_ (std::is_pod<_ARG>::value) ;
	_CAST_<TEMP<_ARG>> (arg) = {0} ;
}

template <class _ARG>
inline _ARG _COPY_ (const _ARG &arg) {
	return arg ;
}

template <class _ARG>
inline void _SWAP_ (_ARG &arg1 ,_ARG &arg2) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG>::value && std::is_nothrow_move_assignable<_ARG>::value) ;
	auto rax = std::move (arg1) ;
	arg1 = std::move (arg2) ;
	arg2 = std::move (rax) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _SWAP_ (DEF<_ARG[_VAL]> &arg1 ,DEF<_ARG[_VAL]> &arg2) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG>::value && std::is_nothrow_move_assignable<_ARG>::value) ;
	_SWAP_ (_CAST_<PACK<_ARG[_VAL]>> (arg1) ,_CAST_<PACK<_ARG[_VAL]>> (arg2)) ;
}

template <class _ARG>
inline _ARG _EXCHANGE_ (_ARG &arg) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG>::value) ;
	_ARG ret = arg ;
	_ZERO_ (arg) ;
	return std::move (ret) ;
}

template <class _ARG1 ,class _ARG2>
inline _ARG1 _EXCHANGE_ (_ARG1 &arg1 ,_ARG2 &&arg2) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_ARG1 ret = arg1 ;
	arg1 = arg2 ;
	return std::move (ret) ;
}

template <class _ARG1 ,class... _ARGS>
inline PTR<TEMP<_ARG1>> _CREATE_ (PTR<TEMP<_ARG1>> arg1 ,_ARGS &&...args) popping {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG1>::value) ;
	_DEBUG_ASSERT_ (arg1 != NULL) ;
	const auto r1x = new (arg1) _ARG1 (std::forward<_ARGS> (args)...) ;
	(void) r1x ;
	_DEBUG_ASSERT_ (r1x == &_CAST_<_ARG1> (*arg1)) ;
	return arg1 ;
}

template <class _ARG>
inline void _DESTROY_ (PTR<TEMP<_ARG>> arg) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG>::value) ;
	_DEBUG_ASSERT_ (arg != NULL) ;
	_CAST_<_ARG> (*arg).~_ARG () ;
}

template <class _ARG ,class = ENABLE_TYPE<std::is_same<_ARG ,ARR<TEMP<REMOVE_TEMP_TYPE<REMOVE_EXTEND_TYPE<_ARG>>>>>::value>>
inline PTR<_ARG> _CREATE_ (PTR<_ARG> dst ,LENGTH len) popping ;

template <class _ARG ,class = ENABLE_TYPE<std::is_same<_ARG ,ARR<TEMP<REMOVE_TEMP_TYPE<REMOVE_EXTEND_TYPE<_ARG>>>>>::value>>
inline void _DESTROY_ (PTR<_ARG> dst ,LENGTH len) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<REMOVE_TEMP_TYPE<REMOVE_EXTEND_TYPE<_ARG>>>::value) ;
	_DEBUG_ASSERT_ (dst != NULL) ;
	for (INDEX i = 0 ; i < len ; i++)
		_DESTROY_ (&(*dst)[i]) ;
}

template <class _ARG>
inline auto _CALL_ (const _ARG &arg) popping->decltype (arg ()) {
	return arg () ;
}

//@error:assure ruined object when an exception was thrown
template <class _ARG>
inline void _CALL_TRY_ (const _ARG &arg) {
	_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<_ARG ()> ,void>::value) ;
	try {
		arg () ;
		return ;
	} catch (const std::exception &e) {
		e.what () ;
	}
}

//@error:assure ruined object when an exception was thrown
template <class _ARG1 ,class _ARG2>
inline auto _CALL_TRY_ (const _ARG1 &arg1 ,const _ARG2 &arg2) popping->RESULT_TYPE<_ARG1 ()> {
	_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<_ARG1 ()> ,RESULT_TYPE<_ARG2 ()>>::value) ;
	try {
		return arg1 () ;
	} catch (const std::exception &e) {
		e.what () ;
	}
	return arg2 () ;
}

template <class _ARG>
inline void _CALL_TRY_ (const _ARG &arg ,const decltype (std::nothrow) &) noexcept {
	arg () ;
}

namespace U {
template <template <class...> class _ARGT ,class... _ARGS>
struct OPERATOR_DEDUCE {
	using TYPE = _ARGT<_ARGS...> ;
} ;
} ;

template <template <class...> class _ARGT ,class... _ARGS>
using OPERATOR_DEDUCE_TYPE = typename U::OPERATOR_DEDUCE<_ARGT ,_ARGS...>::TYPE ;

template <template <class...> class _RETT ,class... _ARGS>
inline OPERATOR_DEDUCE_TYPE<_RETT ,REMOVE_CVR_TYPE<_ARGS>...> _DEDUCE_ (_ARGS &&...args) {
	return OPERATOR_DEDUCE_TYPE<_RETT ,REMOVE_CVR_TYPE<_ARGS>...> (std::forward<_ARGS> (args)...) ;
}

inline namespace STD {
template <class _ARG>
inline BOOL _MEMEQUAL_ (const ARR<_ARG> src1 ,const ARR<_ARG> src2 ,LENGTH len) {
	_DEBUG_ASSERT_ ((src1 != NULL && src2 != NULL && len > 0) || len == 0) ;
	if (src1 == src2)
		return TRUE ;
	for (INDEX i = 0 ; i < len ; i++)
		if (!(src1[i] == src2[i]))
			return FALSE ;
	return TRUE ;
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMEQUAL_ (const ARR<_ARG> src1 ,const DEF<_ARG[_VAL]> &src2) {
	return _MEMEQUAL_ (src1 ,src2 ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMEQUAL_ (const DEF<_ARG[_VAL]> &src1 ,const ARR<_ARG> src2) {
	return _MEMEQUAL_ (src1 ,src2 ,_VAL) ;
}

template <class _ARG>
inline BOOL _MEMLESS_ (const ARR<_ARG> src1 ,const ARR<_ARG> src2 ,LENGTH len) {
	_DEBUG_ASSERT_ ((src1 != NULL && src2 != NULL && len > 0) || len == 0) ;
	if (src1 == src2)
		return FALSE ;
	for (INDEX i = 0 ; i < len ; i++)
		if (!(src1[i] == src2[i]))
			return src1[i] < src2[i] ;
	return FALSE ;
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMLESS_ (const ARR<_ARG> src1 ,const DEF<_ARG[_VAL]> &src2) {
	return _MEMLESS_ (src1 ,src2 ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMLESS_ (const DEF<_ARG[_VAL]> &src1 ,const ARR<_ARG> src2) {
	return _MEMLESS_ (src1 ,src2 ,_VAL) ;
}

template <class _ARG>
inline FLAG _MEMHASH_ (const ARR<_ARG> src ,LENGTH len) {
	_STATIC_ASSERT_ (std::is_same<_ARG ,BYTE>::value) ;
	_DEBUG_ASSERT_ ((src != NULL && len > 0) || len == 0) ;
#ifdef __CSC_CONFIG_VAR32__
	FLAG ret = -2128831035 ;
	const auto r1x = VAR (16777619) ;
#elif defined __CSC_CONFIG_VAR64__
	FLAG ret = -3750763034362895579 ;
	const auto r1x = VAR (1099511628211) ;
#endif
	for (INDEX i = 0 ; i < len ; i++) {
		ret ^= FLAG (src[i]) ;
		ret *= r1x ;
	}
	ret &= VAR_MAX ;
	return std::move (ret) ;
}

template <class _ARG ,LENGTH _VAL>
inline FLAG _MEMHASH_ (const DEF<_ARG[_VAL]> &src) {
	return _MEMHASH_ (src ,_VAL) ;
}

template <class _ARG>
inline INDEX _MEMCHR_ (const ARR<_ARG> src ,LENGTH len ,const _ARG &val) {
	_DEBUG_ASSERT_ ((src != NULL && len > 0) || len == 0) ;
	for (INDEX i = 0 ; i < len ; i++)
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
}

template <class _ARG ,LENGTH _VAL>
inline INDEX _MEMCHR_ (const DEF<_ARG[_VAL]> &src ,const _ARG &val) {
	return _MEMCHR_ (src ,_VAL ,val) ;
}

template <class _ARG>
inline INDEX _MEMRCHR_ (const ARR<_ARG> src ,LENGTH len ,const _ARG &val) {
	_DEBUG_ASSERT_ ((src != NULL && len > 0) || len == 0) ;
	for (INDEX i = len - 1 ; i >= 0 ; i--)
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
}

template <class _ARG ,LENGTH _VAL>
inline INDEX _MEMRCHR_ (const DEF<_ARG[_VAL]> &src ,const _ARG &val) {
	return _MEMRCHR_ (src ,_VAL ,val) ;
}

template <class _ARG>
inline void _MEMCOPY_ (ARR<_ARG> dst ,const ARR<_ARG> src ,LENGTH len) {
	_DEBUG_ASSERT_ ((dst != NULL && src != NULL && len > 0) || len == 0) ;
	if (dst == src)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
	for (INDEX i = 0 ; i < len ; i++)
		dst[i] = src[i] ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMCOPY_ (ARR<_ARG> dst ,const DEF<_ARG[_VAL]> &src) {
	_MEMCOPY_ (dst ,src ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMCOPY_ (DEF<_ARG[_VAL]> &dst ,const ARR<_ARG> src) {
	_MEMCOPY_ (dst ,src ,_VAL) ;
}

template <class _ARG>
inline void _MEMRCOPY_ (ARR<_ARG> dst ,const ARR<_ARG> src ,LENGTH len) {
	_DEBUG_ASSERT_ ((dst != NULL && src != NULL && len > 0) || len == 0) ;
	_DEBUG_ASSERT_ (dst == src || _ABS_ (dst - src) >= len) ;
	if (dst == src) {
		for (INDEX i = 0 ; i < len + ~i ; i++) {
			const auto r1x = dst[i] ;
			dst[i] = dst[len + ~i] ;
			dst[len + ~i] = r1x ;
		}
	} else {
		for (INDEX i = 0 ; i < len ; i++)
			dst[i] = src[len + ~i] ;
	}
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMRCOPY_ (ARR<_ARG> dst ,const DEF<_ARG[_VAL]> &src) {
	_MEMRCOPY_ (dst ,src ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMRCOPY_ (DEF<_ARG[_VAL]> &dst ,const ARR<_ARG> src) {
	_MEMRCOPY_ (dst ,src ,_VAL) ;
}

template <class _ARG>
inline void _MEMMOVE_ (ARR<_ARG> dst1 ,ARR<_ARG> dst2 ,LENGTH len) {
	_DEBUG_ASSERT_ ((dst1 != NULL && dst2 != NULL && len > 0) || len == 0) ;
	if (dst1 < dst2) {
		for (INDEX i = 0 ; i < len ; i++)
			dst1[i] = std::move (dst2[i]) ;
	} else if (dst1 > dst2) {
		for (INDEX i = len - 1 ; i >= 0 ; i--)
			dst1[i] = std::move (dst2[i]) ;
	}
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMMOVE_ (ARR<_ARG> dst1 ,DEF<_ARG[_VAL]> &dst2) {
	_MEMMOVE_ (dst1 ,dst2 ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMMOVE_ (DEF<_ARG[_VAL]> &dst1 ,ARR<_ARG> dst2) {
	_MEMMOVE_ (dst1 ,dst2 ,_VAL) ;
}

template <class _ARG>
inline void _MEMSWAP_ (ARR<_ARG> dst1 ,ARR<_ARG> dst2 ,LENGTH len) {
	_DEBUG_ASSERT_ ((dst1 != NULL && dst2 != NULL && len > 0) || len == 0) ;
	if (dst1 == dst2)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst1 - dst2) >= len) ;
	for (INDEX i = 0 ; i < len ; i++)
		_SWAP_ (dst1[i] ,dst2[i]) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMSWAP_ (ARR<_ARG> dst1 ,DEF<_ARG[_VAL]> &dst2) {
	_MEMSWAP_ (dst1 ,dst2 ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMSWAP_ (DEF<_ARG[_VAL]> &dst1 ,ARR<_ARG> dst2) {
	_MEMSWAP_ (dst1 ,dst2 ,_VAL) ;
}

template <class _ARG>
inline void _MEMCROL_ (ARR<_ARG> dst ,LENGTH len ,INDEX it ,INDEX jt) {
	_DEBUG_ASSERT_ ((dst != NULL && len > 0) || len == 0) ;
	_DEBUG_ASSERT_ (it >= 0 && it < len && jt >= 0 && jt < len) ;
	INDEX ix = 0 ;
	INDEX iy = len ;
	auto rax = std::move (dst[jt]) ;
	if (it <= jt) {
		iy = jt ;
		jt = ix ;
	}
	while (jt > ix && it < iy) {
		for (INDEX i = it ; i < iy ; i++) {
			dst[jt + (i - it)] = std::move (dst[ix + (i - it)]) ;
			dst[ix + (i - it)] = std::move (dst[i]) ;
		}
		const auto r1x = (iy - it) % (jt - ix) ;
		INDEX kx = ix + (iy - it) ;
		INDEX ky = kx + ((jt - ix) - r1x) ;
		INDEX kz = ky + (it - jt) ;
		_MEMMOVE_ (&dst[kz] ,&dst[ky] ,r1x) ;
		ix = kx ;
		it = kz ;
		jt = ky ;
	}
	_MEMMOVE_ (&dst[ix] ,&dst[it] ,iy - it) ;
	if (it < iy)
		jt = ix + (iy - it) ;
	dst[jt] = std::move (rax) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMCROL_ (DEF<_ARG[_VAL]> &dst ,INDEX it ,INDEX jt) {
	_MEMCROL_ (dst ,_VAL ,it ,jt) ;
}

template <class _ARG>
inline void _MEMFILL_ (ARR<_ARG> dst ,LENGTH len ,const _ARG &val) {
	_DEBUG_ASSERT_ ((dst != NULL && len > 0) || len == 0) ;
	for (INDEX i = 0 ; i < len ; i++)
		dst[i] = val ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMFILL_ (DEF<_ARG[_VAL]> &dst ,const _ARG &val) {
	_MEMFILL_ (dst ,_VAL ,val) ;
}
} ;

class Exception :public std::exception {
private:
	template <class>
	class StringException ;

	inline static constexpr LENGTH cvt_minus_one (LENGTH arg) {
		return arg - 1 ;
	}

private:
	PTR<const STRA> mWhat ;

public:
	inline Exception () = delete ;

	template <LENGTH _VAL>
	inline explicit Exception (const DEF<STRA[_VAL]> &what) noexcept :mWhat (what) {}

	template <LENGTH _VAL>
	inline explicit Exception (DEF<STRA[_VAL]> &) = delete ;

	inline PTR<const STRA> what () const noexcept override final {
		return mWhat ;
	}

public:
	template <LENGTH _VAL>
	inline static Exception make (const DEF<STRA[_VAL]> &arg) noexcept {
		return Exception (arg) ;
	}

	template <LENGTH _VAL1 ,LENGTH... _VALS>
	inline static StringException<ARGCS<1 ,cvt_minus_one (_VAL1) ,cvt_minus_one (_VALS)...>> make (const DEF<STRA[_VAL1]> &arg1 ,const DEF<STRA[_VALS]> &...args) noexcept {
		return StringException<ARGCS<1 ,cvt_minus_one (_VAL1) ,cvt_minus_one (_VALS)...>> (arg1 ,args...) ;
	}
} ;

template <class SIZE>
class Exception::StringException :public Exception {
private:
	DEF<STRA[SIZE::value]> mString ;

public:
	inline StringException () = delete ;

	template <LENGTH... _VALS>
	inline explicit StringException (const DEF<STRA[_VALS]> &...args) noexcept :Exception (_XVALUE_<const DEF<STRA[SIZE::value]> &> (mString)) {
		_STATIC_ASSERT_ (ARGCS<1 ,cvt_minus_one (_VALS)...>::value == SIZE::value) ;
		write_string (0 ,args...) ;
	}

private:
	inline void write_string (INDEX it) noexcept {
		mString[it] = 0 ;
	}

	template <LENGTH _VAL1 ,LENGTH... _VALS>
	inline void write_string (INDEX it ,const DEF<STRA[_VAL1]> &arg1 ,const DEF<STRA[_VALS]> &...args) noexcept {
		_MEMCOPY_ (&mString[it] ,arg1) ;
		write_string (it + _VAL1 - 1 ,args...) ;
	}
} ;

inline import DEF<void (PTR<const STRA>)> _UNITTEST_ASSERT_HANDLER_ ;

class Interface {
public:
	inline Interface () = default ;
	inline virtual ~Interface () noexcept = default ;
	inline Interface (const Interface &) = delete ;
	inline Interface &operator= (const Interface &) = delete ;
	inline Interface (Interface &&) = delete ;
	inline Interface &operator= (Interface &&) = delete ;
} ;

template <class TYPE>
class Wrapped {
protected:
	TYPE mData ;

public:
	inline Wrapped () = delete ;
	inline ~Wrapped () = delete ;
	inline Wrapped (const Wrapped &) = delete ;
	inline Wrapped &operator= (const Wrapped &) = delete ;
	inline Wrapped (Wrapped &&) = delete ;
	inline Wrapped &operator= (Wrapped &&) = delete ;
} ;

template <>
class Wrapped<void> {
public:
	inline Wrapped () = delete ;
	inline ~Wrapped () = delete ;
	inline Wrapped (const Wrapped &) = delete ;
	inline Wrapped &operator= (const Wrapped &) = delete ;
	inline Wrapped (Wrapped &&) = delete ;
	inline Wrapped &operator= (Wrapped &&) = delete ;
} ;

template <class TYPE>
class Singleton {
private:
	_STATIC_ASSERT_ (std::is_class<TYPE>::value) ;
	friend TYPE ;
	PTR<TYPE> mPointer ;

private:
	inline Singleton () {
		mPointer = new TYPE () ;
	}

	inline ~Singleton () noexcept {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline Singleton (const Singleton &) = delete ;
	inline Singleton &operator= (const Singleton &) = delete ;
	inline Singleton (Singleton &&) = delete ;
	inline Singleton &operator= (Singleton &&) = delete ;

	inline implicit operator TYPE & () const {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		return *mPointer ;
	}

public:
	//@error:static instance between DLL ruins Singleton
	inline static TYPE &instance () ;
} ;

template <class TYPE>
class ScopedHolder {
private:
	PTR<TYPE> mPointer ;

public:
	inline ScopedHolder () = delete ;

	inline explicit ScopedHolder (PTR<TYPE> &&pointer) noexcept :mPointer (pointer) {}

	inline ~ScopedHolder () noexcept {
		if (mPointer == NULL)
			return ;
		template_delete (*this) ;
		mPointer = NULL ;
	}

	inline ScopedHolder (const ScopedHolder &) = delete ;
	inline ScopedHolder &operator= (const ScopedHolder &) = delete ;
	inline ScopedHolder (ScopedHolder &&) = delete ;
	inline ScopedHolder &operator= (ScopedHolder &&) = delete ;

	inline implicit operator const PTR<TYPE> & () const noexcept {
		return mPointer ;
	}

	inline ScopedHolder &operator= (decltype (NULL)) noexcept {
		mPointer = NULL ;
		return *this ;
	}

private:
	template <class _ARG>
	inline static void template_delete (ScopedHolder<_ARG> &me) noexcept {
		delete me.mPointer ;
	}

	template <class _ARG>
	inline static void template_delete (ScopedHolder<ARR<_ARG>> &me) noexcept {
		delete[] _XVALUE_<const PTR<_ARG> &> (*me.mPointer) ;
	}

	template <class _ARG ,LENGTH _VAL>
	inline static void template_delete (ScopedHolder<_ARG[_VAL]> &me) noexcept {
		delete[] me.mPointer ;
	}
} ;

template <class TYPE>
class ScopedGuard {
private:
	TYPE & mLock ;

public:
	inline ScopedGuard () = delete ;

	inline explicit ScopedGuard (TYPE &lock) :mLock (lock) {
		mLock.lock () ;
	}

	inline ~ScopedGuard () noexcept {
#ifdef __CSC_DEBUG__
		_CALL_TRY_ ([&] () {
			mLock.unlock () ;
		} ,std::nothrow) ;
#else
		_CALL_TRY_ ([&] () {
			mLock.unlock () ;
		}) ;
#endif
	}

	inline ScopedGuard (const ScopedGuard &) = delete ;
	inline ScopedGuard &operator= (const ScopedGuard &) = delete ;
	inline ScopedGuard (ScopedGuard &&) = delete ;
	inline ScopedGuard &operator= (ScopedGuard &&) = delete ;
} ;

template <class _ARG ,class>
inline PTR<_ARG> _CREATE_ (PTR<_ARG> dst ,LENGTH len) popping {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<REMOVE_TEMP_TYPE<REMOVE_EXTEND_TYPE<_ARG>>>::value) ;
	INDEX iw = 0 ;
	const auto r1x = [&] () noexcept {
		for (INDEX i = 0 ; i < iw ; i++)
			_DESTROY_ (&(*dst)[i]) ;
		iw = VAR_NONE ;
	} ;
	class Finally :private Wrapped<decltype (r1x)> {
	public:
		inline void lock () const noexcept {}

		inline void unlock () const noexcept {
			Finally::mData () ;
		}
	} ;
	ScopedGuard<const Finally> ANONYMOUS (_CAST_<Finally> (r1x)) ;
	for (INDEX i = iw ; i < len ; i++) {
		_CREATE_ (&(*dst)[i]) ;
		iw++ ;
	}
	iw = VAR_NONE ;
	return dst ;
}

template <class>
class AutoRef ;

template <class TYPE>
class AutoRef<CONSTRUCTIBLE<TYPE ,TRUE ,FALSE ,TRUE>> {
private:
	class Holder {
	private:
		friend AutoRef ;
		friend AutoRef<TYPE> ;
		TYPE mData ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend AutoRef<TYPE> ;
	PTR<Holder> mPointer ;

public:
	inline AutoRef () noexcept {
		mPointer = NULL ;
	}

	inline ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline AutoRef (const AutoRef &) = delete ;
	inline AutoRef &operator= (const AutoRef &) = delete ;

	inline AutoRef (AutoRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~AutoRef () ;
		new (this) AutoRef (std::move (right)) ;
		return *this ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :mPointer (pointer) {}
} ;

template <class TYPE>
class AutoRef<CONSTRUCTIBLE<TYPE ,TRUE ,TRUE ,TRUE>> {
private:
	class Holder {
	private:
		friend AutoRef ;
		friend AutoRef<TYPE> ;
		TYPE mData ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend AutoRef<TYPE> ;
	PTR<Holder> mPointer ;

public:
	inline AutoRef () noexcept {
		mPointer = NULL ;
	}

	inline ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline AutoRef (const AutoRef &right) {
		ScopedHolder<Holder> sgd (right.mPointer != NULL ? new Holder (_XVALUE_<const TYPE &> (right.mPointer->mData)) : NULL) ;
		mPointer = sgd ;
		sgd = NULL ;
	}

	inline AutoRef &operator= (const AutoRef &right) {
		if (this == &right)
			return *this ;
		this->~AutoRef () ;
		new (this) AutoRef (std::move (right)) ;
		return *this ;
	}

	inline AutoRef (AutoRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~AutoRef () ;
		new (this) AutoRef (std::move (right)) ;
		return *this ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :mPointer (pointer) {}
} ;

template <class TYPE>
class AutoRef :private AutoRef<CONSTRUCTIBLE<TYPE ,TRUE ,std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value ,TRUE>> {
private:
	using BASE_TYPE = AutoRef<CONSTRUCTIBLE<TYPE ,TRUE ,std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value ,TRUE>> ;
	using Holder = typename BASE_TYPE::Holder ;

private:
	using BASE_TYPE::mPointer ;

public:
	inline AutoRef () = default ;

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline TYPE &to () {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mData ;
	}

	inline implicit operator TYPE & () {
		return to () ;
	}

	inline PTR<TYPE> operator-> () {
		return &to () ;
	}

	inline const TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mData ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

	inline AutoRef &operator= (const TYPE &arg) {
		to () = std::move (arg) ;
		return *this ;
	}

	inline AutoRef &operator= (TYPE &&arg) {
		to () = std::move (arg) ;
		return *this ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :BASE_TYPE (pointer) {}

public:
	template <class... _ARGS>
	inline static AutoRef make (_ARGS &&...args) {
		ScopedHolder<Holder> sgd (new Holder (std::forward<_ARGS> (args)...)) ;
		AutoRef ret = AutoRef (sgd) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <class TYPE>
class SharedRef {
private:
	class Holder {
	private:
		friend SharedRef ;
		TYPE mData ;
		LENGTH mCounter ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) ,mCounter (0) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline SharedRef () noexcept {
		mPointer = NULL ;
	}

	inline ~SharedRef () noexcept {
		if (mPointer == NULL)
			return ;
		const auto r1x = --mPointer->mCounter == 0 ;
		if (r1x)
			delete mPointer ;
		mPointer = NULL ;
	}

	inline SharedRef (const SharedRef &right) :SharedRef (right.mPointer) {}

	inline SharedRef &operator= (const SharedRef &right) {
		if (this == &right)
			return *this ;
		this->~SharedRef () ;
		new (this) SharedRef (std::move (right)) ;
		return *this ;
	}

	inline SharedRef (SharedRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline SharedRef &operator= (SharedRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~SharedRef () ;
		new (this) SharedRef (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mData ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

	inline PTR<TYPE> operator-> () const {
		return &to () ;
	}

	inline const SharedRef &operator= (const TYPE &arg) const {
		to () = std::move (arg) ;
		return *this ;
	}

	inline const SharedRef &operator= (TYPE &&arg) const {
		to () = std::move (arg) ;
		return *this ;
	}

private:
	inline explicit SharedRef (PTR<Holder> pointer) :mPointer (pointer) {
		if (mPointer != NULL)
			++mPointer->mCounter ;
	}

public:
	template <class... _ARGS>
	inline static SharedRef make (_ARGS &&...args) {
		ScopedHolder<Holder> sgd (new Holder (std::forward<_ARGS> (args)...)) ;
		SharedRef ret = SharedRef (sgd) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <class TYPE>
class AnyRef {
private:
	class Holder :public Interface {
	public:
		virtual const std::type_info &type () const = 0 ;
	} ;

	template <class _TYPE>
	class ImplHolder :public Holder {
	private:
		friend AnyRef ;
		_TYPE mData ;

	public:
		template <class... _ARGS>
		inline explicit ImplHolder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}

		inline const std::type_info &type () const override {
			return typeid (_TYPE) ;
		}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline AnyRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG>
	inline implicit AnyRef (AnyRef<_ARG> &&right) : AnyRef (std::move (right.template rebind<TYPE> ())) {}

	inline ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~AnyRef () ;
		new (this) AnyRef (std::move (right)) ;
		return *this ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () {
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const {
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline const std::type_info &type () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->type () ;
	}

	inline TYPE &to () {
		//@error:RTTI might be different between DLL
		_DEBUG_ASSERT_ (type () == typeid (TYPE)) ;
		return static_cast<PTR<ImplHolder<TYPE>>> (mPointer)->mData ;
	}

	inline implicit operator TYPE & () {
		return to () ;
	}

	inline PTR<TYPE> operator-> () {
		return &to () ;
	}

	inline const TYPE &to () const {
		//@error:RTTI might be different between DLL
		_DEBUG_ASSERT_ (type () == typeid (TYPE)) ;
		return static_cast<PTR<ImplHolder<TYPE>>> (mPointer)->mData ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

	inline AnyRef &operator= (const TYPE &arg) {
		to () = std::move (arg) ;
		return *this ;
	}

	inline AnyRef &operator= (TYPE &&arg) {
		to () = std::move (arg) ;
		return *this ;
	}

private:
	inline explicit AnyRef (PTR<Holder> pointer) :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static AnyRef make (_ARGS &&...args) {
		ScopedHolder<ImplHolder<TYPE>> sgd (new ImplHolder<TYPE> (std::forward<_ARGS> (args)...)) ;
		AnyRef ret = AnyRef (sgd) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <>
class AnyRef<void> {
private:
	class Holder :public Interface {
	public:
		virtual const std::type_info &type () const = 0 ;
	} ;

private:
	PTR<Holder> mPointer ;

public:
	inline AnyRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG>
	inline implicit AnyRef (AnyRef<_ARG> &&right) :AnyRef (std::move (right.template rebind<void> ())) {}

	inline ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~AnyRef () ;
		new (this) AnyRef (std::move (right)) ;
		return *this ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () {
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const {
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline const std::type_info &type () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->type () ;
	}
} ;

template <class TYPE>
class UniqueRef {
private:
	class Holder :public Interface {
	public:
		virtual void release () noexcept = 0 ;
	} ;

	template <class _TYPE>
	class ImplHolder :public Holder {
	private:
		friend UniqueRef ;
		TYPE mData ;
		_TYPE mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (_TYPE &&functor) :mFunctor (std::move (functor)) {}

		inline void release () noexcept override {
#ifdef __CSC_DEBUG__
			_CALL_TRY_ ([&] () {
				mFunctor (mData) ;
			} ,std::nothrow) ;
#else
			_CALL_TRY_ ([&] () {
				mFunctor (mData) ;
			}) ;
#endif
		}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline UniqueRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG> ,UniqueRef>::value>>
	inline explicit UniqueRef (_ARG &&destructor) {
		_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<_ARG (TYPE &)> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG ,PTR<void (TYPE &)>>::value) ;
		ScopedHolder<ImplHolder<_ARG>> sgd (new ImplHolder<_ARG> (std::forward<_ARG> (destructor))) ;
		mPointer = sgd ;
		sgd = NULL ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (const _ARG1 &constructor ,_ARG2 &&destructor) {
		_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<_ARG1 (TYPE &)> ,void>::value && std::is_same<RESULT_TYPE<_ARG2 (TYPE &)> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG2 ,PTR<void (TYPE &)>>::value) ;
		ScopedHolder<ImplHolder<_ARG2>> sgd (new ImplHolder<_ARG2> (std::forward<_ARG2> (destructor))) ;
		constructor (_XVALUE_<const PTR<ImplHolder<_ARG2>> &> (sgd)->mData) ;
		mPointer = sgd ;
		sgd = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->release () ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~UniqueRef () ;
		new (this) UniqueRef (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline const TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return static_cast<PTR<ImplHolder<PTR<void (TYPE &)>>>> (mPointer)->mData ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

private:
	inline explicit UniqueRef (PTR<Holder> pointer) :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static UniqueRef make (_ARGS &&...args) {
		ScopedHolder<ImplHolder<PTR<void (TYPE &)>>> sgd (new ImplHolder<PTR<void (TYPE &)>> ([] (TYPE &) {})) ;
		const auto r1x = _XVALUE_<const PTR<ImplHolder<PTR<void (TYPE &)>>> &> (sgd) ;
		r1x->mData = TYPE (std::forward<_ARGS> (args)...) ;
		UniqueRef ret = UniqueRef (_XVALUE_<const PTR<Holder> &> (r1x)) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <>
class UniqueRef<void> {
private:
	class Holder :public Interface {
	public:
		virtual void release () noexcept = 0 ;
	} ;

	template <class _TYPE>
	class ImplHolder :public Holder {
	private:
		_TYPE mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (_TYPE &&functor) :mFunctor (std::move (functor)) {}

		inline void release () noexcept override {
#ifdef __CSC_DEBUG__
			_CALL_TRY_ (mFunctor ,std::nothrow) ;
#else
			_CALL_TRY_ (mFunctor) ;
#endif
		}
	} ;

private:
	PTR<Holder> mPointer ;

public:
	inline UniqueRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG> ,UniqueRef>::value>>
	inline explicit UniqueRef (_ARG &&destructor) {
		_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<_ARG ()> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG ,PTR<void ()>>::value) ;
		ScopedHolder<ImplHolder<_ARG>> sgd (new ImplHolder<_ARG> (std::forward<_ARG> (destructor))) ;
		mPointer = sgd ;
		sgd = NULL ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (const _ARG1 &constructor ,_ARG2 &&destructor) {
		_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<_ARG1 ()> ,void>::value && std::is_same<RESULT_TYPE<_ARG2 ()> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG2 ,PTR<void ()>>::value) ;
		ScopedHolder<ImplHolder<_ARG2>> sgd (new ImplHolder<_ARG2> (std::forward<_ARG2> (destructor))) ;
		constructor () ;
		mPointer = sgd ;
		sgd = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->release () ;
		delete mPointer ;
		mPointer = NULL ;
	}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~UniqueRef () ;
		new (this) UniqueRef (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}
} ;

template <class ,class>
class Ref ;

using SPHAN = ARGC<-3> ;
using SCPHAN = ARGC<-4> ;
using SMPHAN = ARGC<-5> ;

template <class TYPE>
class Ref<TYPE ,SCPHAN> {
private:
	PTR<const TYPE> mPointer ;

public:
	inline Ref () noexcept {
		mPointer = NULL ;
	}

	inline ~Ref () noexcept {
		mPointer = NULL ;
	}

	inline Ref (const Ref &) = delete ;
	inline Ref &operator= (const Ref &) = delete ;

	inline Ref (Ref &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline Ref &operator= (Ref &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Ref () ;
		new (this) Ref (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline const TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return *mPointer ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

private:
	inline explicit Ref (PTR<const TYPE> pointer) :mPointer (pointer) {}

public:
	//@error:phantom means deliver pointer without holder
	inline static Ref make (PTR<const TYPE> arg) {
		return Ref (arg) ;
	}

	template <class _ARG>
	inline static Ref make (const Ref<TYPE ,_ARG> &arg) {
		return make (&arg.self) ;
	}
} ;

template <class TYPE>
class Ref<TYPE ,SMPHAN> {
private:
	PTR<TYPE> mPointer ;

public:
	inline Ref () noexcept {
		mPointer = NULL ;
	}

	inline ~Ref () noexcept {
		mPointer = NULL ;
	}

	inline Ref (const Ref &) = delete ;
	inline Ref &operator= (const Ref &) = delete ;

	inline Ref (Ref &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline Ref &operator= (Ref &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Ref () ;
		new (this) Ref (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline TYPE &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return *mPointer ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

	inline PTR<TYPE> operator-> () const {
		return &to () ;
	}

	inline implicit operator const Ref<TYPE ,SCPHAN> & () const {
		return _CAST_<Ref<TYPE ,SCPHAN>> (*this) ;
	}

private:
	inline explicit Ref (PTR<TYPE> pointer) :mPointer (pointer) {}

public:
	//@error:phantom means deliver pointer without holder
	inline static Ref make (PTR<TYPE> arg) {
		return Ref (arg) ;
	}

	template <class _ARG>
	inline static Ref make (Ref<TYPE ,_ARG> &arg) {
		return make (&arg.self) ;
	}

	inline static Ref make (const Ref<TYPE ,SMPHAN> &arg) {
		return make (&arg.self) ;
	}
} ;

template <class TYPE>
using PhanRef = Ref<REMOVE_CONST_TYPE<TYPE> ,ARGC<std::is_const<TYPE>::value ? SCPHAN::value : SMPHAN::value>> ;

template <class>
class Function ;

template <class TYPE1 ,class... TYPES>
class Function<TYPE1 (TYPES...)> {
private:
	class Holder :public Interface {
	public:
		virtual TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping = 0 ;
	} ;

	template <class>
	class ImplHolder ;

private:
	PTR<Holder> mFunction1 ;
	PTR<TYPE1 (TYPES...)> mFunction2 ;

public:
	inline Function () noexcept {
		mFunction1 = NULL ;
		mFunction2 = NULL ;
	}

	inline implicit Function (PTR<TYPE1 (TYPES...)> right) {
		mFunction1 = NULL ;
		mFunction2 = right ;
	}

	template <class _ARG ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG> ,Function>::value>>
	inline implicit Function (_ARG &&right) {
		_STATIC_ASSERT_ (std::is_same<RESULT_TYPE<REMOVE_CVR_TYPE<_ARG> (TYPES...)> ,TYPE1>::value) ;
		ScopedHolder<ImplHolder<REMOVE_CVR_TYPE<_ARG>>> sgd (new ImplHolder<REMOVE_CVR_TYPE<_ARG>> (std::forward<_ARG> (right))) ;
		mFunction1 = sgd ;
		mFunction2 = NULL ;
		sgd = NULL ;
	}

	inline ~Function () noexcept {
		if (mFunction1 == NULL && mFunction2 == NULL)
			return ;
		delete mFunction1 ;
		mFunction1 = NULL ;
		mFunction2 = NULL ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&right) noexcept {
		mFunction1 = _EXCHANGE_ (right.mFunction1) ;
		mFunction2 = _EXCHANGE_ (right.mFunction2) ;
	}

	inline Function &operator= (Function &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Function () ;
		new (this) Function (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mFunction1 != NULL || mFunction2 != NULL ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mFunction1 != NULL)
			return mFunction1->invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
		return mFunction2 (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

	inline TYPE1 operator() (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

private:
	inline explicit Function (PTR<Holder> func) :mFunction1 (func) ,mFunction2 (NULL) {}

public:
	//@info:the function is unimplemented without 'csc_ext.hpp'
	template <class... _ARGS>
	inline static Function make (PTR<TYPE1 (TYPES... ,_ARGS...)> arg1 ,const REMOVE_CVR_TYPE<_ARGS> &...args) ;
} ;

template <class TYPE1 ,class... TYPES>
template <class _TYPE>
class Function<TYPE1 (TYPES...)>::ImplHolder :public Function<TYPE1 (TYPES...)>::Holder {
private:
	_TYPE mFunctor ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const _TYPE &functor) :mFunctor (std::move (functor)) {}

	inline explicit ImplHolder (_TYPE &&functor) :mFunctor (std::move (functor)) {}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const override popping {
		return mFunctor (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
class Function<DEF<TYPE1 (TYPES...)> NONE::*> {
private:
	class IMPLHOLDER ;

	class Holder :public Interface {
	public:
		friend Function ;
		virtual PTR<Holder> adress_copy (PTR<TEMP<IMPLHOLDER>> adress) const noexcept = 0 ;
		virtual TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping = 0 ;
	} ;

	template <class>
	class ImplHolder ;

	class IMPLHOLDER :public Holder {
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4371) //@info:warning C4371: “xxx”: 由于成员“xxx”更好的封装，类的布局可能与早期版本的编译器有所不同
#endif

	private:
		PTR<NONE> mContext ;
		DEF<DEF<TYPE1 (TYPES...)> NONE::*> mFunction ;

	public:
		inline IMPLHOLDER () = delete ;

		inline PTR<Holder> adress_copy (PTR<TEMP<IMPLHOLDER>> adress) const noexcept override ;
		inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const override popping ;

#pragma warning (pop)
#pragma endregion
	} ;

private:
	TEMP<IMPLHOLDER> mImplHolder ;
	PTR<Holder> mFunction ;

public:
	inline Function () = delete ;

	template <class _ARG>
	inline explicit Function (const PhanRef<_ARG> &context ,DEF<DEF<TYPE1 (TYPES...)> _ARG::*> function) noexcept {
		mFunction = ImplHolder<_ARG>::adress_create (&mImplHolder ,&context.self ,function) ;
		_DEBUG_ASSERT_ (mFunction == &_CAST_<IMPLHOLDER> (mImplHolder)) ;
	}

	template <class _ARG>
	inline explicit Function (const PhanRef<const _ARG> &context ,DEF<DEF<TYPE1 (TYPES...) const> _ARG::*> function) noexcept {
		mFunction = ImplHolder<const _ARG>::adress_create (&mImplHolder ,&context.self ,function) ;
		_DEBUG_ASSERT_ (mFunction == &_CAST_<IMPLHOLDER> (mImplHolder)) ;
	}

	inline ~Function () noexcept {
		if (mFunction == NULL)
			return ;
		const auto r1x = _XVALUE_<const PTR<const Holder> &> (&_CAST_<IMPLHOLDER> (mImplHolder)) ;
		r1x->~Holder () ;
		mFunction = NULL ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&right) noexcept {
		mFunction = right.mFunction ;
		if (mFunction == NULL)
			return ;
		const auto r1x = _XVALUE_<const PTR<const Holder> &> (&_CAST_<IMPLHOLDER> (right.mImplHolder)) ;
		mFunction = r1x->adress_copy (&mImplHolder) ;
		_DEBUG_ASSERT_ (mFunction == &_CAST_<IMPLHOLDER> (mImplHolder)) ;
	}

	inline Function &operator= (Function &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Function () ;
		new (this) Function (std::move (right)) ;
		return *this ;
	}

	inline BOOL exist () const {
		return mFunction != NULL ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = _XVALUE_<const PTR<const Holder> &> (&_CAST_<IMPLHOLDER> (mImplHolder)) ;
		return r1x->invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

	inline TYPE1 operator() (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class _TYPE>
class Function<DEF<TYPE1 (TYPES...)> NONE::*>::ImplHolder :public Holder {
private:
	PTR<_TYPE> mContext ;
	DEF<DEF<TYPE1 (TYPES...)> _TYPE::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<_TYPE> context ,DEF<DEF<TYPE1 (TYPES...)> _TYPE::*> function) noexcept :mContext (context) ,mFunction (function) {}

	inline PTR<Holder> adress_copy (PTR<TEMP<IMPLHOLDER>> adress) const noexcept override {
		return adress_create (adress ,mContext ,mFunction) ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const override popping {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

public:
	inline static PTR<Holder> adress_create (PTR<TEMP<IMPLHOLDER>> adress ,PTR<_TYPE> context ,DEF<DEF<TYPE1 (TYPES...)> _TYPE::*> function) noexcept {
		const auto r1x = _CREATE_ (&_LOAD_<TEMP<ImplHolder>> (adress) ,context ,function) ;
		return &_CAST_<ImplHolder> (*r1x) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class _TYPE>
class Function<DEF<TYPE1 (TYPES...)> NONE::*>::ImplHolder<const _TYPE> :public Holder {
private:
	PTR<const _TYPE> mContext ;
	DEF<DEF<TYPE1 (TYPES...) const> _TYPE::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<const _TYPE> context ,DEF<DEF<TYPE1 (TYPES...) const> _TYPE::*> function) noexcept :mContext (context) ,mFunction (function) {}

	inline PTR<Holder> adress_copy (PTR<TEMP<IMPLHOLDER>> adress) const noexcept override {
		return adress_create (adress ,mContext ,mFunction) ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const override popping {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

public:
	inline static PTR<Holder> adress_create (PTR<TEMP<IMPLHOLDER>> adress ,PTR<const _TYPE> context ,DEF<DEF<TYPE1 (TYPES...) const> _TYPE::*> function) noexcept {
		const auto r1x = _CREATE_ (&_LOAD_<TEMP<ImplHolder>> (adress) ,context ,function) ;
		return &_CAST_<ImplHolder> (*r1x) ;
	}
} ;

namespace U {
template <class _ARG1 ,class... _ARGS>
struct OPERATOR_DEDUCE<Function ,PTR<_ARG1 (_ARGS...)>> {
	using TYPE = Function<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct OPERATOR_DEDUCE<Function ,PhanRef<_ARG2> ,DEF<_ARG1 (_ARGS...)> _ARG2::*> {
	using TYPE = Function<DEF<_ARG1 (_ARGS...)> NONE::*> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct OPERATOR_DEDUCE<Function ,PhanRef<_ARG2> ,DEF<_ARG1 (_ARGS...) const> _ARG2::*> {
	using TYPE = Function<DEF<_ARG1 (_ARGS...)> NONE::*> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct OPERATOR_DEDUCE<Function ,PhanRef<const _ARG2> ,DEF<_ARG1 (_ARGS...) const> _ARG2::*> {
	using TYPE = Function<DEF<_ARG1 (_ARGS...)> NONE::*> ;
} ;
} ;

template <class ,class>
class Buffer ;

using SFLEX = ARGC<0> ;
using SFIXED = ARGC<-1> ;
using SAUTO = ARGC<-2> ;
using SPHAN = ARGC<-3> ;
using SCPHAN = ARGC<-4> ;
using SMPHAN = ARGC<-5> ;

template <class TYPE ,LENGTH SIZE>
class Buffer<TYPE ,ARGC<SIZE>> {
private:
	_STATIC_ASSERT_ (SIZE > 0) ;
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	DEF<TYPE[SIZE]> mBuffer ;

public:
	inline Buffer () = default ;

	inline implicit Buffer (const DEF<TYPE[SIZE]> &right) :Buffer (std::move (_CAST_<Buffer> (right))) {}

	inline implicit Buffer (DEF<TYPE[SIZE]> &&right) : Buffer (std::move (_CAST_<Buffer> (right))) {}

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len >= 0 && len <= SIZE) ;
	}

	inline ARR<TYPE> &to () {
		return _LOAD_<ARR<TYPE>> (mBuffer) ;
	}

	inline implicit operator ARR<TYPE> & () {
		return to () ;
	}

	inline const ARR<TYPE> &to () const {
		return _LOAD_<ARR<TYPE>> (mBuffer) ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline LENGTH size () const {
		return SIZE ;
	}

	inline TYPE &get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline TYPE &operator[] (INDEX index) {
		return get (index) ;
	}

	inline const TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &val) const {
		INDEX ret = &val - mBuffer ;
		if (ret < 0 || ret >= size ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &right) const {
		return _MEMEQUAL_ (mBuffer ,right.mBuffer ,SIZE) ;
	}

	inline BOOL operator== (const Buffer &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Buffer &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Buffer &right) const {
		return _MEMLESS_ (mBuffer ,right.mBuffer ,SIZE) ;
	}

	inline BOOL operator< (const Buffer &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Buffer &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Buffer &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Buffer &right) const {
		return !right.less (*this) ;
	}

	inline Buffer expand () const {
		return expand (0) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Buffer> () ;
	}

	inline void expand (Buffer &&arg) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

public:
	inline static Buffer &from (DEF<TYPE[SIZE]> &me) {
		return _CAST_<Buffer> (me) ;
	}

	inline static const Buffer &from (const DEF<TYPE[SIZE]> &me) {
		return _CAST_<Buffer> (me) ;
	}
} ;

template <class TYPE>
class Buffer<TYPE ,SFIXED> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<ARR<TYPE>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer () noexcept {
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len >= 0) ;
		ScopedHolder<ARR<TYPE>> sgd (len > 0 ? &_LOAD_<ARR<TYPE>> (new TYPE[len]) : NULL) ;
		mBuffer = sgd ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		delete[] _XVALUE_<const PTR<TYPE> &> (*mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;
	inline Buffer (Buffer &&) = delete ;
	inline Buffer &operator= (Buffer &&) = delete ;

	inline ARR<TYPE> &to () {
		return *mBuffer ;
	}

	inline implicit operator ARR<TYPE> & () {
		return to () ;
	}

	inline const ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline LENGTH size () const {
		return mSize ;
	}

	inline TYPE &get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline TYPE &operator[] (INDEX index) {
		return get (index) ;
	}

	inline const TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &val) const {
		INDEX ret = &val - *mBuffer ;
		if (ret < 0 || ret >= size ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &right) const {
		return mSize == right.mSize && _MEMEQUAL_ (*mBuffer ,*right.mBuffer ,right.mSize) ;
	}

	inline BOOL operator== (const Buffer &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Buffer &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Buffer &right) const {
		for (INDEX i = 0 ,ie = _MIN_ (mSize ,right.mSize) ; i < ie ; i++)
			if ((*mBuffer)[i] != (*right.mBuffer)[i])
				return (*mBuffer)[i] < (*right.mBuffer)[i] ;
		return mSize < right.mSize ;
	}

	inline BOOL operator< (const Buffer &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Buffer &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Buffer &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Buffer &right) const {
		return !right.less (*this) ;
	}

	inline Buffer<TYPE ,SAUTO> expand () const {
		return expand (0) ;
	}

	inline Buffer<TYPE ,SAUTO> expand (LENGTH len) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Buffer<TYPE ,SAUTO> () ;
	}

	inline void expand (Buffer<TYPE ,SAUTO> &&arg) {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;

template <class TYPE>
using FixedBuffer = Buffer<TYPE ,SFIXED> ;

template <class TYPE>
class Buffer<TYPE ,SAUTO> ;

template <class TYPE>
class Buffer<CONSTRUCTIBLE<TYPE ,TRUE ,FALSE ,TRUE> ,SAUTO> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend Buffer<TYPE ,SAUTO> ;
	PTR<ARR<TYPE>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer () noexcept {
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len >= 0) ;
		ScopedHolder<ARR<TYPE>> sgd (len > 0 ? &_LOAD_<ARR<TYPE>> (new TYPE[len]) : NULL) ;
		mBuffer = sgd ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		delete[] _XVALUE_<const PTR<TYPE> &> (*mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;

	inline Buffer (Buffer &&right) noexcept {
		mBuffer = _EXCHANGE_ (right.mBuffer) ;
		mSize = _EXCHANGE_ (right.mSize) ;
	}

	inline Buffer &operator= (Buffer &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Buffer () ;
		new (this) Buffer (std::move (right)) ;
		return *this ;
	}
} ;

template <class TYPE>
class Buffer<CONSTRUCTIBLE<TYPE ,TRUE ,TRUE ,TRUE> ,SAUTO> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend Buffer<TYPE ,SAUTO> ;
	PTR<ARR<TYPE>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer () noexcept {
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len >= 0) ;
		ScopedHolder<ARR<TYPE>> sgd (len > 0 ? &_LOAD_<ARR<TYPE>> (new TYPE[len]) : NULL) ;
		mBuffer = sgd ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		delete[] _XVALUE_<const PTR<TYPE> &> (*mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &right) {
		ScopedHolder<ARR<TYPE>> sgd (right.mSize > 0 ? &_LOAD_<ARR<TYPE>> (new TYPE[right.mSize]) : NULL) ;
		_MEMCOPY_ (*sgd ,*right.mBuffer ,right.mSize) ;
		mBuffer = sgd ;
		mSize = right.mSize ;
		sgd = NULL ;
	}

	inline Buffer &operator= (const Buffer &right) {
		if (this == &right)
			return *this ;
		this->~Buffer () ;
		new (this) Buffer (std::move (right)) ;
		return *this ;
	}

	inline Buffer (Buffer &&right) noexcept {
		mBuffer = _EXCHANGE_ (right.mBuffer) ;
		mSize = _EXCHANGE_ (right.mSize) ;
	}

	inline Buffer &operator= (Buffer &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Buffer () ;
		new (this) Buffer (std::move (right)) ;
		return *this ;
	}
} ;

template <class TYPE>
class Buffer<TYPE ,SAUTO> :private Buffer<CONSTRUCTIBLE<TYPE ,TRUE ,std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value ,TRUE> ,SAUTO> {
private:
	using BASE_TYPE = Buffer<CONSTRUCTIBLE<TYPE ,TRUE ,std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value ,TRUE> ,SAUTO> ;
	using BASE_TYPE::mBuffer ;
	using BASE_TYPE::mSize ;

public:
	inline Buffer () = default ;

	inline explicit Buffer (LENGTH len) :BASE_TYPE (len) {}

	inline ARR<TYPE> &to () {
		return *mBuffer ;
	}

	inline implicit operator ARR<TYPE> & () {
		return to () ;
	}

	inline const ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline LENGTH size () const {
		return mSize ;
	}

	inline TYPE &get (INDEX index) {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline TYPE &operator[] (INDEX index) {
		return get (index) ;
	}

	inline const TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &val) const {
		INDEX ret = &val - *mBuffer ;
		if (ret < 0 || ret >= size ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &right) const {
		return mSize == right.mSize && _MEMEQUAL_ (*mBuffer ,*right.mBuffer ,right.mSize) ;
	}

	inline BOOL operator== (const Buffer &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Buffer &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Buffer &right) const {
		for (INDEX i = 0 ,ie = _MIN_ (mSize ,right.mSize) ; i < ie ; i++)
			if ((*mBuffer)[i] != (*right.mBuffer)[i])
				return (*mBuffer)[i] < (*right.mBuffer)[i] ;
		return mSize < right.mSize ;
	}

	inline BOOL operator< (const Buffer &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Buffer &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Buffer &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Buffer &right) const {
		return !right.less (*this) ;
	}

	inline Buffer expand () const {
		const auto r1x = _MAX_ (LENGTH (VALX_SQRT2 * mSize) ,mSize + EXPANDING_SIZE::value) ;
		return expand (r1x) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (len >= 0) ;
		return Buffer (len) ;
	}

	inline void expand (Buffer &&arg) {
		*this = std::move (arg) ;
	}
} ;

template <class TYPE>
using AutoBuffer = Buffer<TYPE ,SAUTO> ;

template <class TYPE>
class Buffer<TYPE ,SCPHAN> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<const ARR<TYPE>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer () noexcept {
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len == 0) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;

	inline Buffer (Buffer &&right) noexcept {
		mBuffer = _EXCHANGE_ (right.mBuffer) ;
		mSize = _EXCHANGE_ (right.mSize) ;
	}

	inline Buffer &operator= (Buffer &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Buffer () ;
		new (this) Buffer (std::move (right)) ;
		return *this ;
	}

	inline const ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline LENGTH size () const {
		return mSize ;
	}

	inline const TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &val) const {
		INDEX ret = &val - *mBuffer ;
		if (ret < 0 || ret >= size ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &right) const {
		return mSize == right.mSize && _MEMEQUAL_ (*mBuffer ,*right.mBuffer ,right.mSize) ;
	}

	inline BOOL operator== (const Buffer &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Buffer &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Buffer &right) const {
		for (INDEX i = 0 ,ie = _MIN_ (mSize ,right.mSize) ; i < ie ; i++)
			if ((*mBuffer)[i] != (*right.mBuffer)[i])
				return (*mBuffer)[i] < (*right.mBuffer)[i] ;
		return mSize < right.mSize ;
	}

	inline BOOL operator< (const Buffer &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Buffer &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Buffer &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Buffer &right) const {
		return !right.less (*this) ;
	}

	inline Buffer expand () const {
		return expand (0) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Buffer> () ;
	}

	inline void expand (Buffer &&arg) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

private:
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<_ARG ,ARR<const TYPE>>::value>>
	inline explicit Buffer (PTR<_ARG> src ,LENGTH len) :mBuffer (src) ,mSize (len) {}

public:
	//@error:phantom means deliver pointer without holder
	inline static Buffer make (const ARR<TYPE> src ,LENGTH len) {
		_DEBUG_ASSERT_ ((src != NULL && len > 0) || len == 0) ;
		const auto r1x = len > 0 ? &_LOAD_<ARR<TYPE>> (src) : NULL ;
		return Buffer (r1x ,len) ;
	}

	template <LENGTH _VAL>
	inline static Buffer make (const DEF<TYPE[_VAL]> &src) {
		return make (src ,_VAL) ;
	}

	template <class _ARG>
	inline static Buffer make (const Buffer<TYPE ,_ARG> &src) {
		return make (src ,src.size ()) ;
	}
} ;

template <class TYPE>
class Buffer<TYPE ,SMPHAN> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<ARR<TYPE>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer () noexcept {
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len == 0) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;

	inline Buffer (Buffer &&right) noexcept {
		mBuffer = _EXCHANGE_ (right.mBuffer) ;
		mSize = _EXCHANGE_ (right.mSize) ;
	}

	inline Buffer &operator= (Buffer &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Buffer () ;
		new (this) Buffer (std::move (right)) ;
		return *this ;
	}

	inline ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator ARR<TYPE> & () const {
		return to () ;
	}

	inline implicit operator const Buffer<TYPE ,SCPHAN> & () const {
		return _CAST_<Buffer<TYPE ,SCPHAN>> (*this) ;
	}

	inline LENGTH size () const {
		return mSize ;
	}

	inline TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &val) const {
		INDEX ret = &val - *mBuffer ;
		if (ret < 0 || ret >= size ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &right) const {
		return mSize == right.mSize && _MEMEQUAL_ (*mBuffer ,*right.mBuffer ,right.mSize) ;
	}

	inline BOOL operator== (const Buffer &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Buffer &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Buffer &right) const {
		for (INDEX i = 0 ,ie = _MIN_ (mSize ,right.mSize) ; i < ie ; i++)
			if ((*mBuffer)[i] != (*right.mBuffer)[i])
				return (*mBuffer)[i] < (*right.mBuffer)[i] ;
		return mSize < right.mSize ;
	}

	inline BOOL operator< (const Buffer &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Buffer &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Buffer &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Buffer &right) const {
		return !right.less (*this) ;
	}

	inline Buffer expand () const {
		return expand (0) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return _NULL_<Buffer> () ;
	}

	inline void expand (Buffer &&arg) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

private:
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<_ARG ,ARR<TYPE>>::value>>
	inline explicit Buffer (PTR<_ARG> src ,LENGTH len) :mBuffer (src) ,mSize (len) {}

public:
	//@error:phantom means deliver pointer without holder
	inline static Buffer make (ARR<TYPE> src ,LENGTH len) {
		_DEBUG_ASSERT_ ((src != NULL && len > 0) || len == 0) ;
		const auto r1x = len > 0 ? &_LOAD_<ARR<TYPE>> (src) : NULL ;
		return Buffer (r1x ,len) ;
	}

	template <LENGTH _VAL>
	inline static Buffer make (DEF<TYPE[_VAL]> &src) {
		return make (src ,_VAL) ;
	}

	template <class _ARG>
	inline static Buffer make (Buffer<TYPE ,_ARG> &src) {
		return make (src ,src.size ()) ;
	}

	inline static Buffer make (const Buffer<TYPE ,SMPHAN> &src) {
		return make (src ,src.size ()) ;
	}
} ;

template <class TYPE>
using PhanBuffer = Buffer<REMOVE_CONST_TYPE<TYPE> ,CONDITIONAL_TYPE<std::is_const<TYPE>::value ,SCPHAN ,SMPHAN>> ;

template <class ,class>
class Allocator ;

template <class TYPE ,class SIZE>
class Allocator<CONSTRUCTIBLE<TYPE ,TRUE ,FALSE ,FALSE> ,SIZE> {
private:
	//@error:memory alignment reduce utilization ratio of memory
	class Pack {
	private:
		friend Allocator ;
		friend Allocator<TYPE ,SIZE> ;
		TEMP<TYPE> mData ;
		INDEX mNext ;
	} ;

	class RealPack {
	private:
		friend Allocator ;
		friend Allocator<TYPE ,SIZE> ;
		TYPE mData ;
		INDEX mNext ;
	} ;

public:
	using BUFFER = Buffer<Pack ,SIZE> ;

private:
	friend Allocator<TYPE ,SIZE> ;
	Buffer<Pack ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		update (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		update (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (BUFFER &&src) :mAllocator (std::move (src)) {
		update (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		clear () ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;
	inline Allocator (Allocator &&) = delete ;
	inline Allocator &operator= (Allocator &&) = delete ;

	inline void clear () noexcept {
		mLength = 0 ;
		mFree = VAR_NONE ;
		for (INDEX i = mAllocator.size () - 1 ; i >= 0 ; i--) {
			if (mAllocator[i].mNext == VAR_USED)
				_DESTROY_ (&mAllocator[i].mData) ;
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}

private:
	inline void update (INDEX _length ,INDEX _free) {
		mLength = _length ;
		mFree = _free ;
		for (INDEX i = mAllocator.size () - 1 ; i >= _length ; i--) {
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}
} ;

template <class TYPE ,class SIZE>
class Allocator<CONSTRUCTIBLE<TYPE ,TRUE ,FALSE ,TRUE> ,SIZE> {
private:
	//@error:memory alignment reduce utilization ratio of memory
	class Pack {
	private:
		friend Allocator ;
		friend Allocator<TYPE ,SIZE> ;
		TEMP<TYPE> mData ;
		INDEX mNext ;
	} ;

	class RealPack {
	private:
		friend Allocator ;
		friend Allocator<TYPE ,SIZE> ;
		TYPE mData ;
		INDEX mNext ;
	} ;

public:
	using BUFFER = Buffer<Pack ,SIZE> ;

private:
	friend Allocator<TYPE ,SIZE> ;
	Buffer<Pack ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		update (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		update (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (BUFFER &&src) :mAllocator (std::move (src)) {
		update (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		clear () ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;

	inline Allocator (Allocator &&right) noexcept :mAllocator (std::move (right.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		INDEX iw = std::is_pod<TYPE>::value ? right.mAllocator.size () : VAR_ZERO ;
		for (INDEX i = iw ; i < right.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<RealPack> (right.mAllocator[i]).mData)) ;
			iw++ ;
		}
		mLength = right.mLength ;
		mFree = right.mFree ;
		if (right.mAllocator.size () == 0)
			right.clear () ;
	}

	inline Allocator &operator= (Allocator &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Allocator () ;
		new (this) Allocator (std::move (right)) ;
		return *this ;
	}

	inline void clear () noexcept {
		mLength = 0 ;
		mFree = VAR_NONE ;
		for (INDEX i = mAllocator.size () - 1 ; i >= 0 ; i--) {
			if (mAllocator[i].mNext == VAR_USED)
				_DESTROY_ (&mAllocator[i].mData) ;
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}

private:
	inline void update (INDEX _length ,INDEX _free) {
		mLength = _length ;
		mFree = _free ;
		for (INDEX i = mAllocator.size () - 1 ; i >= _length ; i--) {
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}
} ;

template <class TYPE ,class SIZE>
class Allocator<CONSTRUCTIBLE<TYPE ,TRUE ,TRUE ,TRUE> ,SIZE> {
private:
	//@error:memory alignment reduce utilization ratio of memory
	class Pack {
	private:
		friend Allocator ;
		friend Allocator<TYPE ,SIZE> ;
		TEMP<TYPE> mData ;
		INDEX mNext ;
	} ;

	class RealPack {
	private:
		friend Allocator ;
		friend Allocator<TYPE ,SIZE> ;
		TYPE mData ;
		INDEX mNext ;
	} ;

public:
	using BUFFER = Buffer<Pack ,SIZE> ;

private:
	friend Allocator<TYPE ,SIZE> ;
	Buffer<Pack ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		update (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		update (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (BUFFER &&src) :mAllocator (std::move (src)) {
		update (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		clear () ;
	}

	inline Allocator (const Allocator &right) :mAllocator (std::move (right.mAllocator)) {
		INDEX iw = std::is_pod<TYPE>::value ? right.mAllocator.size () : 0 ;
		const auto r1x = [&] () noexcept {
			for (INDEX i = 0 ; i < iw ; i++) {
				if (mAllocator[i].mNext != VAR_USED)
					continue ;
				_DESTROY_ (&mAllocator[i].mData) ;
			}
			iw = VAR_NONE ;
		} ;
		class Finally :private Wrapped<decltype (r1x)> {
		public:
			inline void lock () const noexcept {}

			inline void unlock () const noexcept {
				Finally::mData () ;
			}
		} ;
		ScopedGuard<const Finally> ANONYMOUS (_CAST_<Finally> (r1x)) ;
		for (INDEX i = iw ; i < right.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<RealPack> (right.mAllocator[i]).mData)) ;
			iw++ ;
		}
		mLength = right.mLength ;
		mFree = right.mFree ;
		iw = VAR_NONE ;
	}

	inline Allocator &operator= (const Allocator &right) {
		if (this == &right)
			return *this ;
		this->~Allocator () ;
		new (this) Allocator (std::move (right)) ;
		return *this ;
	}

	inline Allocator (Allocator &&right) noexcept :mAllocator (std::move (right.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		INDEX iw = std::is_pod<TYPE>::value ? right.mAllocator.size () : VAR_ZERO ;
		for (INDEX i = iw ; i < right.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<RealPack> (right.mAllocator[i]).mData)) ;
			iw++ ;
		}
		mLength = right.mLength ;
		mFree = right.mFree ;
		if (right.mAllocator.size () == 0)
			right.clear () ;
	}

	inline Allocator &operator= (Allocator &&right) noexcept {
		if (this == &right)
			return *this ;
		this->~Allocator () ;
		new (this) Allocator (std::move (right)) ;
		return *this ;
	}

	inline void clear () noexcept {
		mLength = 0 ;
		mFree = VAR_NONE ;
		for (INDEX i = mAllocator.size () - 1 ; i >= 0 ; i--) {
			if (mAllocator[i].mNext == VAR_USED)
				_DESTROY_ (&mAllocator[i].mData) ;
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}

private:
	inline void update (INDEX _length ,INDEX _free) {
		mLength = _length ;
		mFree = _free ;
		for (INDEX i = mAllocator.size () - 1 ; i >= _length ; i--) {
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}
} ;

template <class TYPE ,class SIZE>
class Allocator :private Allocator<CONSTRUCTIBLE<TYPE ,TRUE ,std::is_copy_constructible<Buffer<TYPE ,SIZE>>::value && std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value ,std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value> ,SIZE> {
private:
	using BASE_TYPE = Allocator<CONSTRUCTIBLE<TYPE ,TRUE ,std::is_copy_constructible<Buffer<TYPE ,SIZE>>::value && std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value ,std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value> ,SIZE> ;
	using Pack = typename BASE_TYPE::Pack ;
	using RealPack = typename BASE_TYPE::RealPack ;

public:
	using BUFFER = typename BASE_TYPE::BUFFER ;

private:
	using BASE_TYPE::mAllocator ;
	using BASE_TYPE::mLength ;
	using BASE_TYPE::mFree ;

public:
	inline Allocator () = default ;

	inline explicit Allocator (LENGTH len) :BASE_TYPE (len) {}

	inline explicit Allocator (BUFFER &&src) :BASE_TYPE (std::move (src)) {}

	inline LENGTH size () const {
		return mAllocator.size () ;
	}

	inline LENGTH length () const {
		return mLength ;
	}

	using BASE_TYPE::clear ;

	inline BOOL used (INDEX index) const {
		return mAllocator[index].mNext == VAR_USED ;
	}

	inline TYPE &get (INDEX index) {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<RealPack> (mAllocator[index]).mData ;
	}

	inline TYPE &operator[] (INDEX index) {
		return get (index) ;
	}

	inline const TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<RealPack> (mAllocator[index]).mData ;
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &val) const {
		INDEX ret = mAllocator.at (_CAST_<Pack> (_OFFSET_ (&RealPack::mData ,&val))) ;
		if (!used (ret))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	template <class... _ARGS>
	inline INDEX alloc (_ARGS &&...args) popping {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		if (mLength >= mAllocator.size ()) {
			auto rax = mAllocator.expand () ;
			_CREATE_ (&rax[mLength].mData ,std::forward<_ARGS> (args)...) ;
			for (INDEX i = 0 ; i < mAllocator.size () ; i++) {
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<RealPack> (mAllocator[i]).mData)) ;
				rax[i].mNext = VAR_USED ;
			}
			mAllocator.expand (std::move (rax)) ;
			update (mLength ,mFree) ;
		} else {
			_CREATE_ (&mAllocator[mFree].mData ,std::forward<_ARGS> (args)...) ;
		}
		INDEX ret = mFree ;
		mFree = mAllocator[mFree].mNext ;
		mAllocator[ret].mNext = VAR_USED ;
		mLength++ ;
		return std::move (ret) ;
	}

	inline void free (INDEX index) noexcept {
		_DEBUG_ASSERT_ (used (index)) ;
		_DESTROY_ (&mAllocator[index].mData) ;
		mAllocator[index].mNext = mFree ;
		mFree = index ;
		mLength-- ;
	}

	inline void reserve (LENGTH len) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		const auto r1x = mAllocator.size () ;
		_DEBUG_ASSERT_ (len >= 0) ;
		len = _MAX_ (len - (r1x - mLength) ,VAR_ZERO) ;
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (r1x + len > r1x) ;
		auto rax = mAllocator.expand (r1x + len) ;
		for (INDEX i = 0 ; i < r1x ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<RealPack> (mAllocator[i]).mData)) ;
			rax[i].mNext = mAllocator[i].mNext ;
		}
		mAllocator.expand (std::move (rax)) ;
		update (r1x ,mFree) ;
	}

	inline void shrink () {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		const auto r1x = _CALL_ ([&] () {
			INDEX ret ;
			for (ret = mAllocator.size () - 1 ; ret >= 0 && mAllocator[ret].mNext != VAR_USED ; ret--)
				(void) ret ;
			ret++ ;
			return std::move (ret) ;
		}) ;
		if (r1x == mAllocator.size ())
			return ;
		auto rax = mAllocator.expand (r1x) ;
		mFree = VAR_NONE ;
		for (INDEX i = r1x - 1 ; i >= 0 ; i--) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<RealPack> (mAllocator[i]).mData)) ;
			rax[i].mNext = mAllocator[i].mNext ;
			if (rax[i].mNext == VAR_USED)
				continue ;
			rax[i].mNext = mFree ;
			mFree = i ;
		}
		mAllocator.expand (std::move (rax)) ;
	}

private:
	using BASE_TYPE::update ;
} ;
} ;