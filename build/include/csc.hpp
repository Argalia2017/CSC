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

#ifdef __clang__
#define __CSC_COMPILER_CLANG__
#elif defined __GNUC__
#define __CSC_COMPILER_GNUC__
#elif defined _MSC_VER
#define __CSC_COMPILER_MSVC__
#endif

#if defined (linux) || defined (__linux) || defined (__linux__)
#define __CSC_SYSTEM_LINUX__
#elif defined SAG_COM
#elif defined (WIN32) || defined (_WIN32) || defined (__WIN32__)
#define __CSC_SYSTEM_WINDOWS__
#endif

#ifdef _M_IX86
#define __CSC_PLATFORM_X86__
#elif defined _M_AMD64
#define __CSC_PLATFORM_X64__
#elif defined _M_ARM
#define __CSC_PLATFORM_ARM__
#elif defined _M_ARM64
#define __CSC_PLATFORM_ARM64__
#elif defined _M_IA64
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
#elif defined _MBCS
#define __CSC_CONFIG_STRA__
#else
#define __CSC_CONFIG_STRA__
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4068) //@info: warning C4068: 未知的杂注
#pragma warning (disable :4619) //@info: warning C4619: #pragma warning : 无警告编号“xxx”
#pragma warning (disable :4100) //@info: warning C4100: “xxx”: 未引用的形参
#pragma warning (disable :4365) //@info: warning C4365: “xxx”: 从“xxx”转换到“xxx”，有符号/无符号不匹配
#pragma warning (disable :4371) //@info: warning C4371: “xxx”: 由于成员“xxx”更好的封装，类的布局可能与早期版本的编译器有所不同
#pragma warning (disable :4464) //@info: warning C4464: 相对包含路径包括 ".."
#pragma warning (disable :4505) //@info: warning C4505: “xxx”: 未引用的本地函数已移除
#pragma warning (disable :4514) //@info: warning C4514: “xxx”: 未引用的内联函数已移除
#pragma warning (disable :4571) //@info: warning C4571: 信息: 自 Visual C++ 7.1 之后，catch(...) 语义发生了变化；不再捕获结构化的异常(SEH)
#pragma warning (disable :4574) //@info: warning C4574: “xxx”被定义为“0”: 您是否希望使用“#if xxx”?
#pragma warning (disable :4668) //@info: warning C4668: 没有将“xxx”定义为预处理器宏，用“0”替换“#if/#elif”
#pragma warning (disable :4710) //@info: warning C4710: “xxx”: 函数未内联
#pragma warning (disable :4711) //@info: warning C4711: 为自动内联扩展选定了函数“xxx”
#pragma warning (disable :4774) //@info: warning C4774: “xxx”: 参数 ? 中应存在的格式字符串不为字符串字面量
#pragma warning (disable :4324) //@info: warning C4324: “xxx”: 由于对齐说明符，结构被填充
#pragma warning (disable :4820) //@info: warning C4820: “xxx”:“xxx”字节填充添加在 数据成员“xxx”后
#pragma warning (disable :4623) //@info: warning C4623: “xxx”: 已将默认构造函数隐式定义为“已删除”
#pragma warning (disable :4624) //@info: warning C4624: “xxx”: 已将析构函数隐式定义为“已删除”
#pragma warning (disable :4625) //@info: warning C4625: “xxx”: 已将复制构造函数隐式定义为“已删除”
#pragma warning (disable :4626) //@info: warning C4626: “xxx”: 已将对齐运算符隐式定义为“已删除”
#pragma warning (disable :5026) //@info: warning C5026: “xxx”: 已将移动构造函数隐式定义为“已删除”
#pragma warning (disable :5027) //@info: warning C5027: “xxx”: 已将移动赋值运算符隐式定义为“已删除”
#pragma warning (disable :5045) //@info: warning C5045: 如果指定了 /Qspectre 开关，编译器会插入内存负载的 Spectre 缓解
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
//@info see also 'https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md'
#pragma warning (default :26401) //@info: warning C26401: Do not delete a raw pointer that is not an owner<T> (i.11).
#pragma warning (default :26403) //@info: warning C26403: Reset or explicitly delete an owner<T> pointer 'xxx' (r.3).
#pragma warning (default :26406) //@info: warning C26406: Do not assign a raw pointer to an owner<T> (r.3).
#pragma warning (disable :26409) //@info: warning C26409: Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
#pragma warning (default :26410) //@info: warning C26410: The parameter 'xxx' is a reference to const unique pointer, use const T* or const T& instead (r.32).
#pragma warning (default :26411) //@info: warning C26411: The parameter 'xxx' is a reference to unique pointer and it is never reassigned or reset, use T* or T& instead (r.33).
#pragma warning (default :26415) //@info: warning C26415: Smart pointer parameter 'xxx' is used only to access contained pointer. Use T* or T& instead (r.30).
#pragma warning (default :26416) //@info: warning C26416: Shared pointer parameter 'xxx' is passed by rvalue reference. Pass by value instead (r.34).
#pragma warning (default :26426) //@info: warning C26426: Global initializer calls a non-constexpr function 'xxx' (i.22).
#pragma warning (default :26429) //@info: warning C26429: Symbol 'xxx' is never tested for nullness, it can be marked as not_null (f.23).
#pragma warning (default :26432) //@info: warning C26432: If you define or delete any default operation in the type 'xxx', define or delete them all (c.21).
#pragma warning (disable :26434) //@info: warning C26434: Function 'xxx' hides a non-virtual function 'xxx' (c.128).
#pragma warning (disable :26435) //@info: warning C26435: Function 'xxx' should specify exactly one of 'virtual', 'override', or 'final' (c.128).
#pragma warning (disable :26439) //@info: warning C26439: This kind of function may not throw. Declare it 'noexcept' (f.6).
#pragma warning (disable :26440) //@info: warning C26440: Function 'xxx' can be declared 'noexcept' (f.6).
#pragma warning (default :26443) //@info: warning C26443: Overriding destructor should not use explicit 'override' or 'virtual' specifiers (c.128).
#pragma warning (disable :26446) //@info: warning C26446: Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
#pragma warning (disable :26447) //@info: warning C26447: The function is declared 'noexcept' but calls function 'xxx' which may throw exceptions (f.6).
#pragma warning (disable :26455) //@info: warning C26455: Default constructor may not throw. Declare it 'noexcept' (f.6).
#pragma warning (default :26460) //@info: warning C26460: The reference argument 'xxx' for function 'xxx' can be marked as const (con.3).
#pragma warning (default :26466) //@info: warning C26466: Don't use static_cast downcasts. A cast from a polymorphic type should use dynamic_cast (type.2).
#pragma warning (default :26471) //@info: warning C26471: Don't use reinterpret_cast. A cast from void* can use static_cast (type.1).
#pragma warning (default :26473) //@info: warning C26473: Don't cast between pointer types where the source type and the target type are the same (type.1).
#pragma warning (disable :26475) //@info: warning C26475: Do not use function style C-casts (es.49).
#pragma warning (disable :26481) //@info: warning C26481: Don't use pointer arithmetic. Use span instead (bounds.1).
#pragma warning (disable :26482) //@info: warning C26482: Only index into arrays using constant expressions (bounds.2).
#pragma warning (disable :26485) //@info: warning C26485: Expression 'xxx': No array to pointer decay (bounds.3).
#pragma warning (disable :26486) //@info: warning C26486: Don't pass a pointer that may be invalid to a function. Parameter ? 'xxx' in call to 'xxx' may be invalid (lifetime.1).
#pragma warning (default :26487) //@info: warning C26487: Don't return a pointer that may be invalid (lifetime.1).
#pragma warning (disable :26489) //@info: warning C26489: Don't dereference a pointer that may be invalid: 'xxx'. 'xxx' may have been invalidated at line ? (lifetime.1).
#pragma warning (default :26490) //@info: warning C26490: Don't use reinterpret_cast (type.1).
#pragma warning (default :26491) //@info: warning C26491: Don't use static_cast downcasts (type.2).
#pragma warning (disable :26493) //@info: warning C26493: Don't use C-style casts (type.4).
#pragma warning (disable :26495) //@info: warning C26495: Variable 'xxx' is uninitialized. Always initialize a member variable (type.6).
#pragma warning (default :26496) //@info: warning C26496: The variable 'xxx' is assigned only once, mark it as const (con.4).
#endif
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
namespace stl {
using std::int32_t ;
using std::int64_t ;
using std::uint8_t ;
using std::uint16_t ;
using std::uint32_t ;
using std::uint64_t ;

using std::move ;
using std::forward ;
using std::nothrow ;

#ifdef __CSC_COMPILER_GNUC__
//@error: 'std::max_align_t' is not avaliable in g++4.8
using ::max_align_t ;
#else
using std::max_align_t ;
#endif

using std::numeric_limits ;
using std::initializer_list ;

using std::true_type ;
using std::false_type ;
using std::is_reference ;
using std::is_lvalue_reference ;
using std::is_rvalue_reference ;
using std::is_const ;
using std::is_volatile ;
using std::is_class ;
using std::is_pointer ;
using std::is_member_pointer ;
using std::is_array ;
using std::is_function ;
using std::is_pod ;
using std::is_same ;
using std::is_base_of ;
using std::is_abstract ;
using std::is_default_constructible ;
using std::is_constructible ;
using std::is_nothrow_destructible ;
using std::is_copy_constructible ;
using std::is_copy_assignable ;
using std::is_nothrow_move_constructible ;
using std::is_nothrow_move_assignable ;
using std::is_convertible ;

#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4996)
#endif
using std::result_of ;
#pragma warning (pop)
#pragma endregion

using std::enable_if ;
using std::conditional ;
using std::remove_reference ;
using std::remove_const ;
using std::remove_volatile ;
using std::remove_pointer ;
using std::remove_extent ;
} ;

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

#define _STATIC_WARNING_(...)

#ifdef __CSC_DEBUG__
#ifdef __CSC_SYSTEM_WINDOWS__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) __debugbreak () ; } while (FALSE)
#elif defined __CSC_SYSTEM_LINUX__
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) __builtin_trap () ; } while (FALSE)
#else
#define _DEBUG_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) assert (FALSE) ; } while (FALSE)
#endif
#else
#define _DEBUG_ASSERT_(...) do {} while (FALSE)
#endif

#ifdef __CSC_COMPILER_CLANG__
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " M_FILE " ," _STR_ (M_LINE)) ; } while (FALSE)
#elif defined __CSC_COMPILER_GNUC__
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " M_FILE " ," _STR_ (M_LINE)) ; } while (FALSE)
#elif defined __CSC_COMPILER_MSVC__
#define _DYNAMIC_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception (_PCSTR_ ("dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," _STR_ (M_LINE))) ; } while (FALSE)
#endif

#ifdef __CSC_UNITTEST__
#define _UNITTEST_WATCH_(...) do { struct ARGVPL ; CSC::GlobalWatch::watch (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,_PCSTR_ (_STR_ (__VA_ARGS__)) ,(_UNW_ (__VA_ARGS__))) ; } while (FALSE)
#else
#define _UNITTEST_WATCH_(...) do {} while (FALSE)
#endif

#define ANONYMOUS _CAT_ (_anonymous_ ,M_LINE)

#ifdef __CSC_COMPILER_MSVC__
#define FOR_ONCE_DO_WHILE_FALSE auto ANONYMOUS : CSC::DEF<ARGVS<>[1]> {}
#elif defined __CSC_COMPILER_GNUC__
#define FOR_ONCE_DO_WHILE_FALSE __attribute__ ((unused)) auto &ANONYMOUS : CSC::_NULL_<CSC::DEF<int[1]>> ()
#else
#define FOR_ONCE_DO_WHILE_FALSE auto &ANONYMOUS : CSC::_NULL_<CSC::DEF<int[1]>> ()
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

static constexpr auto VAR32_MAX = VAR32 (2147483647) ;
static constexpr auto VAR32_MIN = ~VAR32_MAX ;
static constexpr auto VAR64_MAX = VAR64 (9223372036854775807) ;
static constexpr auto VAR64_MIN = ~VAR64_MAX ;

#ifdef __CSC_CONFIG_VAR32__
using VAR = VAR32 ;

static constexpr auto VAR_MAX = VAR32_MAX ;
static constexpr auto VAR_MIN = VAR32_MIN ;
#elif defined __CSC_CONFIG_VAR64__
using VAR = VAR64 ;

static constexpr auto VAR_MAX = VAR64_MAX ;
static constexpr auto VAR_MIN = VAR64_MIN ;
#endif

using VARX = long ;
using VARY = unsigned long ;

static constexpr auto VAR_ZERO = VAR (+0) ;
static constexpr auto VAR_NONE = VAR (-1) ;
static constexpr auto VAR_USED = VAR (-2) ;

using INDEX = VAR ;
using LENGTH = VAR ;
using FLAG = VAR ;

enum EFLAG :VAR ;

#define _SIZEOF_(...) (CSC::LENGTH (sizeof (CSC::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>)))
#define _ALIGNOF_(...) (CSC::LENGTH (alignof (CSC::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>)))
#define _CAPACITYOF_(...) (CSC::LENGTH (sizeof... (_UNW_ (__VA_ARGS__))))
#define _COUNTOF_(...) (CSC::U::COUNTOF_TRAITS<CSC::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>>::TYPE::value)

using VAL32 = float ;
using VAL64 = double ;

static constexpr auto VAL32_MAX = VAL32 (3.402823466E+38) ;
static constexpr auto VAL32_MIN = VAL32 (1.175494351E-38) ;
static constexpr auto VAL32_EPS = VAL32 (1.192092896E-07) ;
static constexpr auto VAL64_MAX = VAL64 (1.7976931348623158E+308) ;
static constexpr auto VAL64_MIN = VAL64 (2.2250738585072014E-308) ;
static constexpr auto VAL64_EPS = VAL64 (2.2204460492503131E-016) ;
static constexpr auto VAL32_INF = std::numeric_limits<VAL32>::infinity () ;
static constexpr auto VAL32_NAN = std::numeric_limits<VAL32>::quiet_NaN () ;
static constexpr auto VAL64_INF = std::numeric_limits<VAL64>::infinity () ;
static constexpr auto VAL64_NAN = std::numeric_limits<VAL64>::quiet_NaN () ;

#ifdef __CSC_CONFIG_VAL32__
using VAL = VAL32 ;

static constexpr auto VAL_MAX = VAL32_MAX ;
static constexpr auto VAL_MIN = VAL32_MIN ;
static constexpr auto VAL_EPS = VAL32_EPS ;
static constexpr auto VAL_INF = VAL32_INF ;
static constexpr auto VAL_NAN = VAL32_NAN ;
#elif defined __CSC_CONFIG_VAL64__
using VAL = VAL64 ;

static constexpr auto VAL_MAX = VAL64_MAX ;
static constexpr auto VAL_MIN = VAL64_MIN ;
static constexpr auto VAL_EPS = VAL64_EPS ;
static constexpr auto VAL_INF = VAL64_INF ;
static constexpr auto VAL_NAN = VAL64_NAN ;
#endif

using VALX = long double ;

static constexpr auto VALX_E = VALX (2.71828182845904523536) ;
static constexpr auto VALX_PI = VALX (3.14159265358979323846) ;
static constexpr auto VALX_SQRT2 = VALX (1.41421356237309504880) ;
static constexpr auto VALX_LOG2E = VALX (1.44269504088896340736) ;
static constexpr auto VALX_LOG10E = VALX (0.434294481903251827651) ;
static constexpr auto VALX_LOGE2 = VALX (0.693147180559945309417) ;
static constexpr auto VALX_LOGE10 = VALX (2.30258509299404568402) ;

inline namespace S {
template <class _ARG>
inline constexpr _ARG _ABS_ (const _ARG &arg) {
	return (arg < 0) ? (-arg) : (+arg) ;
}

template <class _ARG>
inline constexpr _ARG _SQE_ (const _ARG &arg) {
	return arg * arg ;
}

template <class _ARG>
inline constexpr const _ARG &_MIN_ (const _ARG &arg1 ,const _ARG &arg2) {
	return (!(arg2 < arg1)) ? arg1 : arg2 ;
}

template <class _ARG>
inline constexpr const _ARG &_MAX_ (const _ARG &arg1 ,const _ARG &arg2) {
	return (!(arg1 < arg2)) ? arg1 : arg2 ;
}
} ;

template <class TYPE>
using DEF = TYPE ;

template <class TYPE>
using PTR = TYPE * ;

#ifdef __CSC_COMPILER_GNUC__
//@error: poor gcc may check range of ARR
template <class TYPE>
using ARR = TYPE[0] ;
#else
template <class TYPE>
using ARR = TYPE[] ;
#endif

namespace U {
struct OPERATOR_PTRTOARR {
	template <class _ARG>
	inline ARR<_ARG> &operator[] (const PTR<_ARG> &arg) const noexcept {
		return *PTR<ARR<_ARG>> (arg) ;
	}
} ;
} ;

static constexpr auto PTRTOARR = U::OPERATOR_PTRTOARR {} ;

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
using STRU16 = char16_t ;
using STRU32 = char32_t ;

#define _PCSTRU8_(arg) CSC::_CAST_<const CSC::STRU8[_COUNTOF_ (decltype (_CAT_ (u8 ,arg)))]> (_CAT_ (u8 ,arg))
#define _PCSTRU16_(arg) CSC::_CAST_<const CSC::STRU16[_COUNTOF_ (decltype (_CAT_ (u ,arg)))]> (_CAT_ (u ,arg))
#define _PCSTRU32_(arg) CSC::_CAST_<const CSC::STRU32[_COUNTOF_ (decltype (_CAT_ (U ,arg)))]> (_CAT_ (U ,arg))

using STRA = char ;
using STRW = wchar_t ;

#define _PCSTRA_(arg) CSC::_CAST_<const CSC::STRA[_COUNTOF_ (decltype (arg))]> (arg)
#define _PCSTRW_(arg) CSC::_CAST_<const CSC::STRW[_COUNTOF_ (decltype (_CAT_ (L ,arg)))]> (_CAT_ (L ,arg))

#ifdef __CSC_CONFIG_STRA__
using STR = STRA ;

#define _PCSTR_ _PCSTRA_
#elif defined __CSC_CONFIG_STRW__
using STR = STRW ;

#define _PCSTR_ _PCSTRW_
#endif

namespace stl {
template <class>
struct is_arithmetic :public false_type {} ;

template <>
struct is_arithmetic<VAR32> :public true_type {} ;

template <>
struct is_arithmetic<VAR64> :public true_type {} ;

#ifdef __CSC_COMPILER_MSVC__
template <>
struct is_arithmetic<VARX> :public true_type {} ;

template <>
struct is_arithmetic<VARY> :public true_type {} ;
#endif

template <>
struct is_arithmetic<VAL32> :public true_type {} ;

template <>
struct is_arithmetic<VAL64> :public true_type {} ;

template <>
struct is_arithmetic<VALX> :public true_type {} ;
} ;

namespace stl {
template <class>
struct is_literals :public false_type {} ;

template <>
struct is_literals<STRU8> :public true_type {} ;

template <>
struct is_literals<STRU16> :public true_type {} ;

template <>
struct is_literals<STRU32> :public true_type {} ;

template <>
struct is_literals<STRA> :public true_type {} ;

template <>
struct is_literals<STRW> :public true_type {} ;
} ;

template <VAR _VAL>
struct ARGC {
	enum :VAR {
		value = _VAL
	} ;
} ;

template <VAR...>
struct ARGCS ;

template <>
struct ARGCS<> :public ARGC<0> {} ;

template <VAR _VAL1 ,VAR..._VALS>
struct ARGCS<_VAL1 ,_VALS...> :public ARGC<_VAL1 + ARGCS<_VALS...>::value> {} ;

template <class>
struct ARGV {} ;

template <class...>
struct ARGVS {} ;

template <VAR>
struct ARGVP ;

template <>
struct ARGV<ARGVP<0>> {} ;

template <VAR _VAL>
struct ARGV<ARGVP<_VAL>> :public ARGV<ARGVP<_VAL - 1>> {
	_STATIC_ASSERT_ (_VAL > 0) ;
} ;

static constexpr auto ARGVP0 = ARGV<ARGVP<0>> {} ;
static constexpr auto ARGVP1 = ARGV<ARGVP<1>> {} ;
static constexpr auto ARGVP2 = ARGV<ARGVP<2>> {} ;
static constexpr auto ARGVP3 = ARGV<ARGVP<3>> {} ;
static constexpr auto ARGVP4 = ARGV<ARGVP<4>> {} ;
static constexpr auto ARGVP5 = ARGV<ARGVP<5>> {} ;
static constexpr auto ARGVP6 = ARGV<ARGVP<6>> {} ;
static constexpr auto ARGVP7 = ARGV<ARGVP<7>> {} ;
static constexpr auto ARGVP8 = ARGV<ARGVP<8>> {} ;
static constexpr auto ARGVP9 = ARGV<ARGVP<9>> {} ;

static constexpr auto ARGV_VOID = ARGV<VOID> {} ;
static constexpr auto ARGV_NONE = ARGV<NONE> {} ;

using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
using DEFAULT_LONGSTRING_SIZE = ARGC<8195> ;
using DEFAULT_HUGEBUFFER_SIZE = ARGC<8388608> ;
using DEFAULT_TIMEOUT_SIZE = ARGC<30000> ;
using DEFAULT_EXPANDGUARD_SIZE = ARGC<65536> ;

template <class ,BOOL...>
struct SPECIALIZATION ;

template <class>
struct TEMP ;

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

namespace U {
template <class _ARG>
struct REMOVE_TEMP {
	using TYPE = _ARG ;
} ;

template <class _ARG>
struct REMOVE_TEMP<TEMP<_ARG>> {
	using TYPE = _ARG ;
} ;

template <class>
struct REMOVE_MEMBER_POINTER ;

template <class _ARG1 ,class _ARG2>
struct REMOVE_MEMBER_POINTER<_ARG1 _ARG2::*> {
	using TYPE = _ARG1 ;
} ;

template <class>
struct MEMBEROF_TRAITS ;

template <class _ARG1 ,class _ARG2>
struct MEMBEROF_TRAITS<_ARG1 _ARG2::*> {
	using TYPE = _ARG2 ;
} ;

template <class>
struct COUNTOF_TRAITS ;

template <class _ARG>
struct COUNTOF_TRAITS<ARR<_ARG>> {
	using TYPE = ARGC<0> ;
} ;

template <class _ARG ,LENGTH _VAL>
struct COUNTOF_TRAITS<_ARG[_VAL]> {
	using TYPE = ARGC<_VAL> ;
} ;

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

template <BOOL _VAL>
using ENABLE_TYPE = typename stl::enable_if<_VAL ,VOID>::type ;

template <BOOL _VAL ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = typename stl::conditional<_VAL ,_ARG1 ,_ARG2>::type ;

template <class _ARG>
using REMOVE_REFERENCE_TYPE = typename stl::remove_reference<_ARG>::type ;

template <class _ARG>
using REMOVE_CONST_TYPE = typename stl::remove_const<_ARG>::type ;

template <class _ARG>
using REMOVE_VOLATILE_TYPE = typename stl::remove_volatile<_ARG>::type ;

template <class _ARG>
using REMOVE_POINTER_TYPE = typename stl::remove_pointer<_ARG>::type ;

template <class _ARG>
using REMOVE_ARRAY_TYPE = typename stl::remove_extent<_ARG>::type ;

template <class _ARG>
using REMOVE_TEMP_TYPE = typename U::REMOVE_TEMP<_ARG>::TYPE ;

template <class _ARG>
using REMOVE_CVR_TYPE = REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<REMOVE_REFERENCE_TYPE<_ARG>>> ;

template <class _ARG>
using REMOVE_MEMBER_POINTER_TYPE = typename U::REMOVE_MEMBER_POINTER<_ARG>::TYPE ;

template <class _ARG>
using MEMBEROF_TRAITS_TYPE = typename U::MEMBEROF_TRAITS<_ARG>::TYPE ;

template <class _ARG>
using FORWARD_TRAITS_TYPE = typename U::FORWARD_TRAITS<_ARG>::TYPE ;

template <class _ARG>
using RESULTOF_TYPE = typename stl::result_of<_ARG>::type ;

template <class TYPE>
struct TEMP {
	_STATIC_ASSERT_ (!std::is_reference<TYPE>::value) ;
	alignas (TYPE) DEF<BYTE[_SIZEOF_ (TYPE)]> unused ;
} ;

template <class _RET>
inline constexpr _RET &_NULL_ () {
	return *PTR<REMOVE_REFERENCE_TYPE<_RET>> (NULL) ;
}

template <class _ARG>
inline constexpr LENGTH _ADDRESS_ (const PTR<_ARG> &arg) {
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

//@warn: not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG>
inline CAST_TRAITS_TYPE<_RET ,_ARG> &_CAST_ (_ARG &arg) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_RET>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_ARG>::value || std::is_same<_RET ,TEMP<_ARG>>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG) % _ALIGNOF_ (_RET) == 0) ;
	return *reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG>>> (_ADDRESS_ (&arg)) ;
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

template <class _ARG>
struct LOAD_TRAITS<_ARG ,TEMP<_ARG> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<TEMP<_ARG> ,_ARG ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<ARR<_ARG> ,ARR<TEMP<_ARG>> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<ARR<TEMP<_ARG>> ,ARR<_ARG> ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<_ARG ,NONE ,ARGC<1>> {
	using TYPE = ARGC<std::is_class<_ARG>::value> ;
} ;

template <class _ARG>
struct LOAD_TRAITS<NONE ,_ARG ,ARGC<1>> {
	using TYPE = ARGC<std::is_class<_ARG>::value> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,VOID ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRU8> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRU16> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <>
struct LOAD_TRAITS<ARR<BYTE> ,ARR<STRU32> ,ARGC<2>> {
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

template <class _ARG>
struct LOAD_TRAITS<_ARG ,BYTE ,ARGC<2>> {
	//@info: not recommend
	using TYPE = ARGC<TRUE> ;
} ;
} ;

template <class _ARG1 ,class _ARG2>
using LOAD_TRATIS_TYPE = typename U::LOAD_TRAITS<_ARG1 ,_ARG2 ,ARGC<1>>::TYPE ;

//@warn: not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG>
inline CAST_TRAITS_TYPE<_RET ,_ARG> &_LOAD_ (PTR<_ARG> address) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (LOAD_TRATIS_TYPE<REMOVE_CVR_TYPE<_RET> ,REMOVE_CVR_TYPE<_ARG>>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<std::is_same<REMOVE_CVR_TYPE<_RET> ,NONE>::value ,ARGV<NONE> ,REMOVE_ARRAY_TYPE<_RET>>) ;
	_DEBUG_ASSERT_ (_ADDRESS_ (address) % r1x == 0) ;
	(void) r1x ;
	return *reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG>>> (address) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (DEF<_ARG1 _ARG2::*> arg1 ,const _ARG3 &arg2) noexcept {
	_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG3> ,_ARG1>::value) ;
	_DEBUG_ASSERT_ (arg1 != NULL) ;
	const auto r1x = &_NULL_<BYTE> () + _ADDRESS_ (&arg2) - _ADDRESS_ (&(_NULL_<_ARG2> ().*arg1)) ;
	return _LOAD_<CAST_TRAITS_TYPE<_ARG2 ,_ARG3>> (r1x) ;
}

template <class _RET>
inline REMOVE_REFERENCE_TYPE<_RET> &_XVALUE_ (REMOVE_REFERENCE_TYPE<_RET> &arg) noexcept {
	_STATIC_ASSERT_ (std::is_lvalue_reference<_RET>::value) ;
	//@warn: required 'std::launder'
	return arg ;
}

template <class _ARG>
inline _ARG &_ZERO_ (_ARG &arg) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG>::value) ;
	_CAST_<TEMP<_ARG>> (arg) = TEMP<_ARG> {0} ;
	return arg ;
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
	_SWAP_ (_OFFSET_ (&PACK<_ARG[_VAL]>::P1 ,arg1) ,_OFFSET_ (&PACK<_ARG[_VAL]>::P1 ,arg2)) ;
}

template <class _ARG1 ,class... _ARGS>
inline void _CREATE_ (PTR<TEMP<_ARG1>> address ,_ARGS &&...args) {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG1>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = new (address) _ARG1 (std::forward<_ARGS> (args)...) ;
	_DEBUG_ASSERT_ (r1x == &_LOAD_<_ARG1> (address)) ;
	(void) r1x ;
}

template <class _ARG>
inline void _DESTROY_ (PTR<TEMP<_ARG>> address) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	_LOAD_<_ARG> (address).~_ARG () ;
}

template <class _ARG>
inline FLAG _TYPEID_ (const ARGV<_ARG> &) noexcept ;

template <class _RET>
inline FLAG _TYPEID_ () noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return _TYPEID_ (_NULL_<const ARGV<REMOVE_CVR_TYPE<_RET>>> ()) ;
}

template <class _ARG>
inline RESULTOF_TYPE<_ARG ()> _CALL_ (_ARG &&arg) popping {
	return arg () ;
}

//@warn: assure ruined object when an exception was thrown
template <class _ARG1 ,class _ARG2>
inline void _CALL_TRY_ (const _ARG1 &arg1 ,const _ARG2 &arg2) ;

template <class _ARG>
inline void _CALL_TRY_ (const _ARG &arg ,const decltype (std::nothrow) &) noexcept {
	arg () ;
}

template <class _ARG>
inline const RESULTOF_TYPE<_ARG ()> &_CACHE_ (const _ARG &arg) popping {
	_STATIC_ASSERT_ (!std::is_reference<RESULTOF_TYPE<_ARG ()>>::value) ;
	static const RESULTOF_TYPE<_ARG ()> mInstance = arg () ;
	return mInstance ;
}

inline namespace S {
template <class _ARG>
inline BOOL _MEMEQUAL_ (const ARR<_ARG> &src1 ,const ARR<_ARG> &src2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (src1 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (src2 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	if (src1 == src2)
		return TRUE ;
	for (INDEX i = 0 ; i < len ; i++)
		if (!(src1[i] == src2[i]))
			return FALSE ;
	return TRUE ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMEQUAL_ (const ARR<_ARG> &src1 ,const DEF<_ARG[_VAL]> &src2) {
	return _MEMEQUAL_ (src1 ,PTRTOARR[&src2[0]] ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMEQUAL_ (const DEF<_ARG[_VAL]> &src1 ,const ARR<_ARG> &src2) {
	return _MEMEQUAL_ (PTRTOARR[&src1[0]] ,src2 ,_VAL) ;
}

template <class _ARG>
inline BOOL _MEMLESS_ (const ARR<_ARG> &src1 ,const ARR<_ARG> &src2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (src1 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (src2 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	if (src1 == src2)
		return FALSE ;
	for (INDEX i = 0 ; i < len ; i++)
		if (!(src1[i] == src2[i]))
			return src1[i] < src2[i] ;
	return FALSE ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMLESS_ (const ARR<_ARG> &src1 ,const DEF<_ARG[_VAL]> &src2) {
	return _MEMLESS_ (src1 ,PTRTOARR[&src2[0]] ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline BOOL _MEMLESS_ (const DEF<_ARG[_VAL]> &src1 ,const ARR<_ARG> &src2) {
	return _MEMLESS_ (PTRTOARR[&src1[0]] ,src2 ,_VAL) ;
}

template <class _ARG>
inline FLAG _MEMHASH_ (const ARR<_ARG> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_STATIC_ASSERT_ (std::is_same<_ARG ,BYTE>::value) ;
	_DEBUG_ASSERT_ (src != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
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
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline FLAG _MEMHASH_ (const DEF<_ARG[_VAL]> &src) {
	return _MEMHASH_ (PTRTOARR[&src[0]] ,_VAL) ;
}

template <class _ARG>
inline INDEX _MEMCHR_ (const ARR<_ARG> &src ,LENGTH len ,const _ARG &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (src != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	for (INDEX i = 0 ; i < len ; i++)
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline INDEX _MEMCHR_ (const DEF<_ARG[_VAL]> &src ,const _ARG &val) {
	return _MEMCHR_ (PTRTOARR[&src[0]] ,_VAL ,val) ;
}

template <class _ARG>
inline INDEX _MEMRCHR_ (const ARR<_ARG> &src ,LENGTH len ,const _ARG &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (src != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	for (INDEX i = len - 1 ; i >= 0 ; i--)
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline INDEX _MEMRCHR_ (const DEF<_ARG[_VAL]> &src ,const _ARG &val) {
	return _MEMRCHR_ (PTRTOARR[&src[0]] ,_VAL ,val) ;
}

template <class _ARG>
inline void _MEMCOPY_ (ARR<_ARG> &dst ,const ARR<_ARG> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (src != NULL || len == 0) ;
	_DEBUG_ASSERT_ (dst != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst == src)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
	for (INDEX i = 0 ; i < len ; i++)
		dst[i] = src[i] ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMCOPY_ (ARR<_ARG> &dst ,const DEF<_ARG[_VAL]> &src) {
	_MEMCOPY_ (dst ,PTRTOARR[&src[0]] ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMCOPY_ (DEF<_ARG[_VAL]> &dst ,const ARR<_ARG> &src) {
	_MEMCOPY_ (PTRTOARR[&dst[0]] ,src ,_VAL) ;
}

template <class _ARG>
inline void _MEMRCOPY_ (ARR<_ARG> &dst ,const ARR<_ARG> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (src != NULL || len == 0) ;
	_DEBUG_ASSERT_ (dst != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	_DEBUG_ASSERT_ (dst == src || _ABS_ (dst - src) >= len) ;
	if (dst == NULL || src == NULL)
		return ;
	if (dst == src) {
		for (INDEX i = 0 ; i < len + ~i ; i++) {
			const auto r1x = dst[i] ;
			dst[i] = dst[len + ~i] ;
			dst[len + ~i] = r1x ;
		}
	} else if (dst != src) {
		for (INDEX i = 0 ; i < len ; i++)
			dst[i] = src[len + ~i] ;
	}
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMRCOPY_ (ARR<_ARG> &dst ,const DEF<_ARG[_VAL]> &src) {
	_MEMRCOPY_ (dst ,PTRTOARR[&src[0]] ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMRCOPY_ (DEF<_ARG[_VAL]> &dst ,const ARR<_ARG> &src) {
	_MEMRCOPY_ (PTRTOARR[&dst[0]] ,src ,_VAL) ;
}

template <class _ARG>
inline void _MEMMOVE_ (ARR<_ARG> &dst1 ,ARR<_ARG> &dst2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (dst1 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (dst2 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst1 == dst2)
		return ;
	if (dst1 < dst2) {
		for (INDEX i = 0 ; i < len ; i++)
			dst1[i] = std::move (dst2[i]) ;
	} else if (dst1 > dst2) {
		for (INDEX i = len - 1 ; i >= 0 ; i--)
			dst1[i] = std::move (dst2[i]) ;
	}
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMMOVE_ (ARR<_ARG> &dst1 ,DEF<_ARG[_VAL]> &dst2) {
	_MEMMOVE_ (dst1 ,PTRTOARR[&dst2[0]] ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMMOVE_ (DEF<_ARG[_VAL]> &dst1 ,ARR<_ARG> &dst2) {
	_MEMMOVE_ (PTRTOARR[&dst1[0]] ,dst2 ,_VAL) ;
}

template <class _ARG>
inline void _MEMSWAP_ (ARR<_ARG> &dst1 ,ARR<_ARG> &dst2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (dst1 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (dst2 != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst1 == dst2)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst1 - dst2) >= len) ;
	for (INDEX i = 0 ; i < len ; i++)
		_SWAP_ (dst1[i] ,dst2[i]) ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMSWAP_ (ARR<_ARG> &dst1 ,DEF<_ARG[_VAL]> &dst2) {
	_MEMSWAP_ (dst1 ,PTRTOARR[&dst2[0]] ,_VAL) ;
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMSWAP_ (DEF<_ARG[_VAL]> &dst1 ,ARR<_ARG> &dst2) {
	_MEMSWAP_ (PTRTOARR[&dst1[0]] ,dst2 ,_VAL) ;
}

template <class _ARG>
inline void _MEMFILL_ (ARR<_ARG> &dst ,LENGTH len ,const _ARG &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (dst != NULL || len == 0) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	for (INDEX i = 0 ; i < len ; i++)
		dst[i] = val ;
#pragma GCC diagnostic pop
}

template <class _ARG ,LENGTH _VAL>
inline void _MEMFILL_ (DEF<_ARG[_VAL]> &dst ,const _ARG &val) {
	_MEMFILL_ (PTRTOARR[&dst[0]] ,_VAL ,val) ;
}
} ;

class Exception final {
private:
	inline static constexpr LENGTH expr_sub (LENGTH arg) {
		return arg - 1 ;
	}

	template <class SIZE>
	class Holder {
	private:
		DEF<STR[SIZE::value]> mString ;

	public:
		inline Holder () = delete ;

		template <LENGTH... _VALS>
		inline explicit Holder (const DEF<STRA[_VALS]> &...args) noexcept {
			_STATIC_ASSERT_ (ARGCS<1 ,expr_sub (_VALS)...>::value == SIZE::value) ;
			template_write (mString ,_NULL_<const ARGC<0>> () ,args...) ;
		}

		template <LENGTH... _VALS>
		inline explicit Holder (const DEF<STRW[_VALS]> &...args) noexcept {
			_STATIC_ASSERT_ (ARGCS<1 ,expr_sub (_VALS)...>::value == SIZE::value) ;
			template_write (mString ,_NULL_<const ARGC<0>> () ,args...) ;
		}

		inline const ARR<STR> &to () const noexcept {
			return PTRTOARR[&mString[0]] ;
		}

		inline implicit operator const ARR<STR> & () const noexcept {
			return to () ;
		}

	private:
		inline static void template_write (DEF<STR[SIZE::value]> &array ,const ARGC<SIZE::value - 1> &) noexcept {
			array[SIZE::value - 1] = 0 ;
		}

		template <INDEX _VAL1 ,LENGTH _VAL2 ,LENGTH... _VALS>
		inline static void template_write (DEF<STR[SIZE::value]> &array ,const ARGC<_VAL1> & ,const DEF<STRA[_VAL2]> &arg1 ,const DEF<STRA[_VALS]> &...args) noexcept {
			_STATIC_ASSERT_ (_VAL1 >= 0 && _VAL1 < SIZE::value) ;
			for (INDEX i = 0 ; i < _VAL2 - 1 ; i++) {
				_DEBUG_ASSERT_ (VAR32 (arg1[i]) > 0 && VAR32 (arg1[i]) <= 127) ;
				array[i + _VAL1] = STR (arg1[i]) ;
			}
			template_write (array ,_NULL_<const ARGC<_VAL1 + _VAL2 - 1>> () ,args...) ;
		}

		template <INDEX _VAL1 ,LENGTH _VAL2 ,LENGTH... _VALS>
		inline static void template_write (DEF<STR[SIZE::value]> &array ,const ARGC<_VAL1> & ,const DEF<STRW[_VAL2]> &arg1 ,const DEF<STRW[_VALS]> &...args) noexcept {
			_STATIC_ASSERT_ (_VAL1 >= 0 && _VAL1 < SIZE::value) ;
			for (INDEX i = 0 ; i < _VAL2 - 1 ; i++) {
				_DEBUG_ASSERT_ (VAR32 (arg1[i]) > 0 && VAR32 (arg1[i]) <= 127) ;
				array[i + _VAL1] = STR (arg1[i]) ;
			}
			template_write (_NULL_<const ARGC<_VAL1 + _VAL2 - 1>> () ,args...) ;
		}
	} ;

private:
	const ARR<STR> &mWhat ;

public:
	inline Exception () = delete ;

	template <LENGTH _VAL>
	inline explicit Exception (const DEF<STR[_VAL]> &what) noexcept :mWhat (PTRTOARR[&what[0]]) {}

	template <LENGTH _VAL>
	inline explicit Exception (DEF<STR[_VAL]> &) = delete ;

	template <class _ARG ,LENGTH... _VALS>
	inline explicit Exception (const ARGV<_ARG> & ,const DEF<STRA[_VALS]> &...args) noexcept :mWhat (expr_what (_NULL_<const ARGV<_ARG>> () ,args...)) {}

	template <class _ARG ,LENGTH... _VALS>
	inline explicit Exception (const ARGV<_ARG> & ,const DEF<STRW[_VALS]> &...args) noexcept :mWhat (expr_what (_NULL_<const ARGV<_ARG>> () ,args...)) {}

	inline const ARR<STR> &what () const {
		return mWhat ;
	}

private:
	template <class _ARG ,LENGTH... _VALS>
	inline static const ARR<STR> &expr_what (const ARGV<_ARG> & ,const DEF<STRA[_VALS]> &...args) noexcept {
		const auto r1x = Holder<ARGCS<1 ,expr_sub (_VALS)...>> (args...) ;
		return _CACHE_ ([r1x] () noexcept {
			return r1x ;
		}) ;
	}

	template <class _ARG ,LENGTH... _VALS>
	inline static const ARR<STR> &expr_what (const ARGV<_ARG> & ,const DEF<STRW[_VALS]> &...args) noexcept {
		const auto r1x = Holder<ARGCS<1 ,expr_sub (_VALS)...>> (args...) ;
		return _CACHE_ ([r1x] () noexcept {
			return r1x ;
		}) ;
	}
} ;

template <class _ARG1 ,class _ARG2>
inline void _CALL_TRY_ (const _ARG1 &arg1 ,const _ARG2 &arg2) {
	_STATIC_ASSERT_ (std::is_same<RESULTOF_TYPE<_ARG1 ()> ,void>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULTOF_TYPE<_ARG2 ()> ,void>::value) ;
	try {
		arg1 () ;
		return ;
	} catch (const Exception &e) {
		const auto r1x = e.what () ;
		(void) r1x ;
	}
	arg2 () ;
}

template <class _ARG1 ,class _ARG2>
inline void _CALL_TRY_ (const _ARG1 &arg1 ,const _ARG2 &arg2 ,const decltype (std::nothrow) &) noexcept {
	_STATIC_ASSERT_ (std::is_same<RESULTOF_TYPE<_ARG1 ()> ,void>::value) ;
#ifndef __CSC_COMPILER_CLANG__
	_STATIC_ASSERT_ (noexcept (_NULL_<const _ARG2> () (_NULL_<const Exception> ()))) ;
#endif
	try {
		arg1 () ;
		return ;
	} catch (const Exception &e) {
		arg2 (e) ;
	}
}

class Interface {
public:
	inline Interface () = default ;
	inline virtual ~Interface () noexcept = default ;
	inline Interface (const Interface &) = delete ;
	inline Interface &operator= (const Interface &) = delete ;
	inline Interface (Interface &&) = delete ;
	inline Interface &operator= (Interface &&) = delete ;
} ;

template <class _ARG>
inline FLAG _TYPEID_ (const ARGV<_ARG> &) noexcept {
	_STATIC_ASSERT_ (std::is_same<_ARG ,REMOVE_CVR_TYPE<_ARG>>::value) ;
	//@warn: RTTI might be different across DLL
	class TYPEID final :private Interface {} ret ;
	return std::move (_CAST_<FLAG> (ret)) ;
}

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

#ifdef __CSC_UNITTEST__
class GlobalWatch final :private Wrapped<void> {
private:
	template <class _ARG1 ,class _ARG2>
	class Storage final :private Interface {
	private:
		PACK<FLAG[4] ,PTR<void (_ARG2 &)>> mData ;

	public:
		inline Storage () :mData ({0}) {} ;

		inline implicit operator volatile PACK<FLAG[4] ,PTR<void (_ARG2 &)>> & () volatile & {
			return mData ;
		}

		inline implicit operator volatile PACK<FLAG[4] ,PTR<void (_ARG2 &)>> & () volatile && = delete ;
	} ;

public:
	template <class _ARG1 ,class _ARG2 ,LENGTH _VAL>
	inline static void watch (const ARGV<_ARG1> & ,const DEF<STR[_VAL]> &name ,_ARG2 &data) noexcept {
		static volatile Storage<_ARG1 ,_ARG2> mInstance ;
		auto &r1 = _XVALUE_<volatile PACK<FLAG[4] ,PTR<void (_ARG2 &)>> &> (mInstance) ;
		r1.P1[0] = _ADDRESS_ (&name) ;
		r1.P1[1] = _ADDRESS_ (&data) ;
		r1.P1[2] = _ADDRESS_ (&watch<_ARG1 ,_ARG2 ,_VAL>) ;
		r1.P1[3] = 0 ;
		const auto r2x = _XVALUE_<const volatile PTR<void (_ARG2 &)> &> (r1.P2) ;
		if (r2x == NULL)
			return ;
		r2x (data) ;
		r2x (data) ;
	}
} ;
#endif

class GlobalHeap final :private Wrapped<void> {
private:
	template <class TYPE>
	class OwnerProxy {
	private:
		friend GlobalHeap ;
		PTR<TYPE> mPointer ;

	public:
		inline OwnerProxy () = delete ;

		inline ~OwnerProxy () noexcept {
			if (mPointer == NULL)
				return ;
			GlobalHeap::free (mPointer) ;
			mPointer = NULL ;
		}

		inline OwnerProxy (const OwnerProxy &) = delete ;
		inline OwnerProxy &operator= (const OwnerProxy &) = delete ;

		inline OwnerProxy (OwnerProxy &&) noexcept = default ;
		inline OwnerProxy &operator= (OwnerProxy &&) = delete ;

		inline implicit operator const PTR<TYPE> & () const & noexcept {
			_DEBUG_ASSERT_ (mPointer != NULL) ;
			return mPointer ;
		}

		inline implicit operator const PTR<TYPE> & () && = delete ;

		inline void operator= (decltype (NULL)) & noexcept {
			mPointer = NULL ;
		}

		inline void operator= (decltype (NULL)) && = delete ;

	private:
		inline explicit OwnerProxy (PTR<TYPE> &&pointer) noexcept :mPointer (pointer) {}
	} ;

public:
	template <class _RET>
	inline static OwnerProxy<_RET> alloc () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_RET) <= _ALIGNOF_ (stl::max_align_t)) ;
		const auto r1x = operator new (_SIZEOF_ (_RET) ,std::nothrow) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		const auto r2x = &_NULL_<BYTE> () + _ADDRESS_ (r1x) ;
		return OwnerProxy<_RET> (&_LOAD_<_RET> (r2x)) ;
	}

	template <class _RET>
	inline static OwnerProxy<ARR<_RET>> alloc (LENGTH len) popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		_DEBUG_ASSERT_ (len > 0) ;
		_DEBUG_ASSERT_ (len * _SIZEOF_ (_RET) > 0) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_RET) <= _ALIGNOF_ (stl::max_align_t)) ;
		const auto r1x = operator new (len * _SIZEOF_ (_RET) ,std::nothrow) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		const auto r2x = &_NULL_<BYTE> () + _ADDRESS_ (r1x) ;
		return OwnerProxy<ARR<_RET>> (&_LOAD_<ARR<_RET>> (r2x)) ;
	}

	template <class _ARG>
	inline static void free (const PTR<_ARG> &address) noexcept {
		_DEBUG_ASSERT_ (address != NULL) ;
		const auto r1x = &_NULL_<BYTE> () + _ADDRESS_ (address) ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

template <class TYPE>
class ScopedGuard final {
private:
	TYPE &mLock ;

public:
	inline ScopedGuard () = delete ;

	inline explicit ScopedGuard (TYPE &lock) popping :mLock (lock) {
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
		} ,[&] () {
			(void) mLock ;
		}) ;
#endif
	}

	inline ScopedGuard (const ScopedGuard &) = delete ;
	inline ScopedGuard &operator= (const ScopedGuard &) = delete ;
	inline ScopedGuard (ScopedGuard &&) = delete ;
	inline ScopedGuard &operator= (ScopedGuard &&) = delete ;
		} ;

template <class TYPE>
class ScopedHolder final {
private:
	const volatile PTR<TEMP<TYPE>> &mAddress ;

public:
	inline ScopedHolder () = delete ;

	template <class... _ARGS>
	inline explicit ScopedHolder (const volatile PTR<TEMP<TYPE>> &address ,_ARGS &&...args) popping :mAddress (address) {
		const auto r1x = _XVALUE_<const volatile PTR<TEMP<TYPE>> &> (mAddress) ;
		_CREATE_ (r1x ,std::forward<_ARGS> (args)...) ;
	}

	inline ~ScopedHolder () noexcept {
		const auto r1x = _XVALUE_<const volatile PTR<TEMP<TYPE>> &> (mAddress) ;
		if (r1x == NULL)
			return ;
		_DESTROY_ (r1x) ;
	}

	inline ScopedHolder (const ScopedHolder &) = delete ;
	inline ScopedHolder &operator= (const ScopedHolder &) = delete ;
	inline ScopedHolder (ScopedHolder &&) = delete ;
	inline ScopedHolder &operator= (ScopedHolder &&) = delete ;
} ;

template <class TYPE>
class ScopedHolder<ARR<TYPE>> final {
private:
	const volatile PTR<ARR<TEMP<TYPE>>> &mAddress ;
	INDEX mWrite ;

public:
	inline ScopedHolder () = delete ;

	inline explicit ScopedHolder (const volatile PTR<ARR<TEMP<TYPE>>> &address ,LENGTH len) popping :mAddress (address) ,mWrite (0) {
		const auto r1x = _XVALUE_<const volatile PTR<ARR<TEMP<TYPE>>> &> (mAddress) ;
		if (r1x == NULL)
			return ;
		while (mWrite < len) {
			_CREATE_ (&(*r1x)[mWrite]) ;
			mWrite++ ;
		}
	}

	inline explicit ScopedHolder (const volatile PTR<ARR<TEMP<TYPE>>> &address ,const ARR<TYPE> &src ,LENGTH len) popping :mAddress (address) ,mWrite (0) {
		_DEBUG_ASSERT_ (src != NULL) ;
		const auto r1x = _XVALUE_<const volatile PTR<ARR<TEMP<TYPE>>> &> (mAddress) ;
		if (r1x == NULL)
			return ;
		while (mWrite < len) {
			_CREATE_ (&(*r1x)[mWrite] ,src[mWrite]) ;
			mWrite++ ;
		}
	}

	inline ~ScopedHolder () noexcept {
		const auto r1x = _XVALUE_<const volatile PTR<ARR<TEMP<TYPE>>> &> (mAddress) ;
		if (r1x == NULL)
			return ;
		while (mWrite >= 0) {
			_DESTROY_ (&(*r1x)[mWrite]) ;
			mWrite-- ;
		}
	}

	inline ScopedHolder (const ScopedHolder &) = delete ;
	inline ScopedHolder &operator= (const ScopedHolder &) = delete ;
	inline ScopedHolder (ScopedHolder &&) = delete ;
	inline ScopedHolder &operator= (ScopedHolder &&) = delete ;
} ;

template <class TYPE>
class Singleton final {
private:
	class Pack {
	private:
		friend Singleton ;
		TYPE mData ;
	} ;

private:
	_STATIC_ASSERT_ (std::is_class<TYPE>::value) ;
	_STATIC_ASSERT_ (!std::is_default_constructible<TYPE>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_destructible<TYPE>::value) ;
	friend TYPE ;
	template <class>
	friend class GlobalStatic ;
	PTR<Pack> mPointer ;

private:
	inline Singleton () {
		auto sgd = GlobalHeap::alloc<TEMP<Pack>> () ;
		ScopedHolder<Pack> ANONYMOUS (sgd) ;
		mPointer = &_LOAD_<Pack> (_XVALUE_<const PTR<TEMP<Pack>> &> (sgd)) ;
		sgd = NULL ;
	}

	inline ~Singleton () noexcept {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		mPointer->~Pack () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline Singleton (const Singleton &) = delete ;
	inline Singleton &operator= (const Singleton &) = delete ;
	inline Singleton (Singleton &&) = delete ;
	inline Singleton &operator= (Singleton &&) = delete ;

	inline TYPE &to () const {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		return mPointer->mData ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

public:
	//@warn: static instance across DLL ruins Singleton
	inline static DEF<TYPE & ()> instance ;
} ;

template <class>
class AutoRef ;

template <class TYPE>
class AutoRef<SPECIALIZATION<TYPE ,FALSE>> {
private:
	using SPECIALIZATION_TYPE = AutoRef<TYPE> ;

	class Holder {
	private:
		friend AutoRef ;
		friend SPECIALIZATION_TYPE ;
		TYPE mData ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend SPECIALIZATION_TYPE ;
	PTR<Holder> mPointer ;

public:
	inline AutoRef () noexcept {
		mPointer = NULL ;
	}

	inline ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AutoRef (const AutoRef &) = delete ;
	inline AutoRef &operator= (const AutoRef &) = delete ;

	inline AutoRef (AutoRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&right) noexcept {
		if (this != &right) {
			this->~AutoRef () ;
			new (this) AutoRef (std::move (right)) ;
		}
		return _XVALUE_<AutoRef &> (*this) ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :mPointer (pointer) {}
} ;

template <class TYPE>
class AutoRef<SPECIALIZATION<TYPE ,TRUE>> {
private:
	using SPECIALIZATION_TYPE = AutoRef<TYPE> ;

	class Holder {
	private:
		friend AutoRef ;
		friend SPECIALIZATION_TYPE ;
		TYPE mData ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend SPECIALIZATION_TYPE ;
	PTR<Holder> mPointer ;

public:
	inline AutoRef () noexcept {
		mPointer = NULL ;
	}

	inline ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AutoRef (const AutoRef &right) {
		auto sgd = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedHolder<Holder> ANONYMOUS (sgd ,_XVALUE_<const TYPE &> (right.mPointer->mData)) ;
		mPointer = &_LOAD_<Holder> (_XVALUE_<const PTR<TEMP<Holder>> &> (sgd)) ;
		sgd = NULL ;
	}

	inline AutoRef &operator= (const AutoRef &right) {
		if (this != &right) {
			this->~AutoRef () ;
			new (this) AutoRef (std::move (right)) ;
		}
		return _XVALUE_<AutoRef &> (*this) ;
	}

	inline AutoRef (AutoRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&right) noexcept {
		if (this != &right) {
			this->~AutoRef () ;
			new (this) AutoRef (std::move (right)) ;
		}
		return _XVALUE_<AutoRef &> (*this) ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :mPointer (pointer) {}
} ;

template <class TYPE>
class AutoRef :private AutoRef<SPECIALIZATION<TYPE ,(std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value)>> {
private:
	using SPECIALIZATION_BASE = AutoRef<SPECIALIZATION<TYPE ,(std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value)>> ;
	using Holder = typename SPECIALIZATION_BASE::Holder ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mPointer ;

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

private:
	inline explicit AutoRef (PTR<Holder> pointer) :SPECIALIZATION_BASE (pointer) {}

public:
	template <class... _ARGS>
	inline static AutoRef make (_ARGS &&...args) {
		auto sgd = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedHolder<Holder> ANONYMOUS (sgd ,std::forward<_ARGS> (args)...) ;
		AutoRef ret = AutoRef (&_LOAD_<Holder> (_XVALUE_<const PTR<TEMP<Holder>> &> (sgd))) ;
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
		if (r1x) {
			mPointer->~Holder () ;
			GlobalHeap::free (mPointer) ;
		}
		mPointer = NULL ;
	}

	inline SharedRef (const SharedRef &right) :SharedRef (right.mPointer) {}

	inline SharedRef &operator= (const SharedRef &right) {
		if (this != &right) {
			this->~SharedRef () ;
			new (this) SharedRef (std::move (right)) ;
		}
		return _XVALUE_<SharedRef &> (*this) ;
	}

	inline SharedRef (SharedRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline SharedRef &operator= (SharedRef &&right) noexcept {
		if (this != &right) {
			this->~SharedRef () ;
			new (this) SharedRef (std::move (right)) ;
		}
		return _XVALUE_<SharedRef &> (*this) ;
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

private:
	inline explicit SharedRef (PTR<Holder> pointer) popping {
		mPointer = _COPY_ (pointer) ;
		if (mPointer == NULL)
			return ;
		const auto r1x = ++mPointer->mCounter ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		(void) r1x ;
	}

public:
	template <class... _ARGS>
	inline static SharedRef make (_ARGS &&...args) {
		auto sgd = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedHolder<Holder> ANONYMOUS (sgd ,std::forward<_ARGS> (args)...) ;
		SharedRef ret = SharedRef (&_LOAD_<Holder> (_XVALUE_<const PTR<TEMP<Holder>> &> (sgd))) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <class TYPE>
class AnyRef {
private:
	struct Holder :public Interface {
		virtual FLAG type () const = 0 ;
	} ;

	template <class _TYPE>
	class ImplHolder :public Holder {
	private:
		friend AnyRef ;
		_TYPE mData ;

	public:
		template <class... _ARGS>
		inline explicit ImplHolder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}

		inline FLAG type () const override {
			return _TYPEID_<_TYPE> () ;
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
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&right) noexcept {
		if (this != &right) {
			this->~AnyRef () ;
			new (this) AnyRef (std::move (right)) ;
		}
		return _XVALUE_<AnyRef &> (*this) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline FLAG type () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->type () ;
	}

	inline TYPE &to () {
		_DEBUG_ASSERT_ (type () == _TYPEID_<TYPE> ()) ;
		return static_cast<PTR<ImplHolder<TYPE>>> (mPointer)->mData ;
	}

	inline implicit operator TYPE & () {
		return to () ;
	}

	inline PTR<TYPE> operator-> () {
		return &to () ;
	}

	inline const TYPE &to () const {
		_DEBUG_ASSERT_ (type () == _TYPEID_<TYPE> ()) ;
		return static_cast<PTR<ImplHolder<TYPE>>> (mPointer)->mData ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

private:
	inline explicit AnyRef (PTR<Holder> pointer) :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static AnyRef make (_ARGS &&...args) {
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<TYPE>>> () ;
		ScopedHolder<ImplHolder<TYPE>> ANONYMOUS (sgd ,std::forward<_ARGS> (args)...) ;
		AnyRef ret = AnyRef (&_LOAD_<ImplHolder<TYPE>> (_XVALUE_<const PTR<TEMP<ImplHolder<TYPE>>> &> (sgd))) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <>
class AnyRef<void> {
private:
	struct Holder :public Interface {
		virtual FLAG type () const = 0 ;
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
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&right) noexcept {
		if (this != &right) {
			this->~AnyRef () ;
			new (this) AnyRef (std::move (right)) ;
		}
		return _XVALUE_<AnyRef &> (*this) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}

	inline FLAG type () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->type () ;
	}
} ;

template <class TYPE>
class UniqueRef {
private:
	struct Holder :public Interface {
		virtual void release () = 0 ;
	} ;

	template <class _TYPE>
	class ImplHolder :public Holder {
	private:
		friend UniqueRef ;
		REMOVE_CVR_TYPE<TYPE> mData ;
		_TYPE mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (_TYPE &&functor) :mFunctor (std::move (functor)) {}

		inline void release () override {
			mFunctor (mData) ;
		}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline UniqueRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (const _ARG1 &constructor ,_ARG2 &&destructor) popping {
		_STATIC_ASSERT_ (std::is_same<RESULTOF_TYPE<_ARG1 (TYPE &)> ,void>::value && std::is_same<RESULTOF_TYPE<_ARG2 (TYPE &)> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG2 ,PTR<void (TYPE &)>>::value) ;
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<_ARG2>>> () ;
		ScopedHolder<ImplHolder<_ARG2>> ANONYMOUS (sgd ,std::forward<_ARG2> (destructor)) ;
		const auto r1x = &_LOAD_<ImplHolder<_ARG2>> (_XVALUE_<const PTR<TEMP<ImplHolder<_ARG2>>> &> (sgd)) ;
		constructor (r1x->mData) ;
		mPointer = r1x ;
		sgd = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
#ifdef __CSC_DEBUG__
		_CALL_TRY_ ([&] () {
			mPointer->release () ;
		} ,std::nothrow) ;
#else
		_CALL_TRY_ ([&] () {
			mPointer->release () ;
		} ,[&] () {
			(void) mPointer ;
		}) ;
#endif
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
		}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&right) noexcept {
		if (this != &right) {
			this->~UniqueRef () ;
			new (this) UniqueRef (std::move (right)) ;
		}
		return _XVALUE_<UniqueRef &> (*this) ;
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
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<PTR<void (TYPE &)>>>> () ;
		const auto r1x = _XVALUE_<const PTR<void (TYPE &)> &> ([] (TYPE &) {}) ;
		ScopedHolder<ImplHolder<PTR<void (TYPE &)>>> ANONYMOUS (sgd ,r1x) ;
		const auto r2x = &_LOAD_<ImplHolder<PTR<void (TYPE &)>>> (_XVALUE_<const PTR<TEMP<ImplHolder<PTR<void (TYPE &)>>>> &> (sgd)) ;
		r2x->mData = TYPE (std::forward<_ARGS> (args)...) ;
		UniqueRef ret = UniqueRef (_XVALUE_<const PTR<Holder> &> (r2x)) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
	} ;

template <>
class UniqueRef<void> {
private:
	struct Holder :public Interface {
		virtual void release () = 0 ;
	} ;

	template <class _TYPE>
	class ImplHolder :public Holder {
	private:
		_TYPE mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (_TYPE &&functor) :mFunctor (std::move (functor)) {}

		inline void release () override {
			mFunctor () ;
		}
	} ;

private:
	PTR<Holder> mPointer ;

public:
	inline UniqueRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (const _ARG1 &constructor ,_ARG2 &&destructor) popping {
		_STATIC_ASSERT_ (std::is_same<RESULTOF_TYPE<_ARG1 ()> ,void>::value && std::is_same<RESULTOF_TYPE<_ARG2 ()> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<_ARG2 ,PTR<void ()>>::value) ;
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<_ARG2>>> () ;
		ScopedHolder<ImplHolder<_ARG2>> ANONYMOUS (sgd ,std::forward<_ARG2> (destructor)) ;
		const auto r1x = &_LOAD_<ImplHolder<_ARG2>> (_XVALUE_<const PTR<TEMP<ImplHolder<_ARG2>>> &> (sgd)) ;
		constructor () ;
		mPointer = r1x ;
		sgd = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
#ifdef __CSC_DEBUG__
		_CALL_TRY_ ([&] () {
			mPointer->release () ;
		} ,std::nothrow) ;
#else
		_CALL_TRY_ ([&] () {
			mPointer->release () ;
		} ,[&] () {
			(void) mPointer ;
		}) ;
#endif
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
		}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&right) noexcept {
		if (this != &right) {
			this->~UniqueRef () ;
			new (this) UniqueRef (std::move (right)) ;
		}
		return _XVALUE_<UniqueRef &> (*this) ;
	}

	inline BOOL exist () const {
		return mPointer != NULL ;
	}
	} ;

template <class TYPE>
class PhanRef {
private:
	PTR<TYPE> mPointer ;

public:
	inline PhanRef () noexcept {
		mPointer = NULL ;
	}

	inline ~PhanRef () noexcept {
		mPointer = NULL ;
	}

	inline PhanRef (const PhanRef &) = delete ;
	inline PhanRef &operator= (const PhanRef &) = delete ;

	inline PhanRef (PhanRef &&right) noexcept {
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline PhanRef &operator= (PhanRef &&right) noexcept {
		if (this != &right) {
			this->~PhanRef () ;
			new (this) PhanRef (std::move (right)) ;
		}
		return _XVALUE_<PhanRef &> (*this) ;
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

private:
	inline explicit PhanRef (PTR<TYPE> pointer) :mPointer (pointer) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static PhanRef make (TYPE &arg) {
		return PhanRef (&arg) ;
	}

	inline static PhanRef make (const PhanRef<TYPE> &arg) {
		if (!arg.exist ())
			return PhanRef () ;
		return make (arg.self) ;
	}
} ;

template <class>
class Function ;

template <class TYPE1 ,class... TYPES>
class Function<TYPE1 (TYPES...)> {
private:
	struct Holder :public Interface {
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
		_STATIC_ASSERT_ (std::is_same<RESULTOF_TYPE<REMOVE_CVR_TYPE<_ARG> (TYPES...)> ,TYPE1>::value) ;
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_CVR_TYPE<_ARG>>>> () ;
		ScopedHolder<ImplHolder<REMOVE_CVR_TYPE<_ARG>>> ANONYMOUS (sgd ,std::forward<_ARG> (right)) ;
		mFunction1 = &_LOAD_<ImplHolder<REMOVE_CVR_TYPE<_ARG>>> (_XVALUE_<const PTR<TEMP<ImplHolder<REMOVE_CVR_TYPE<_ARG>>>> &> (sgd)) ;
		mFunction2 = NULL ;
		sgd = NULL ;
	}

	inline ~Function () noexcept {
		if (mFunction1 == NULL && mFunction2 == NULL)
			return ;
		if (mFunction1 != NULL) {
			mFunction1->~Holder () ;
			GlobalHeap::free (mFunction1) ;
		}
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
		if (this != &right) {
			this->~Function () ;
			new (this) Function (std::move (right)) ;
		}
		return _XVALUE_<Function &> (*this) ;
	}

	inline BOOL exist () const {
		return mFunction1 != NULL || mFunction2 != NULL ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mFunction2 != NULL)
			return mFunction2 (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
		return mFunction1->invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

	inline TYPE1 operator() (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

private:
	inline explicit Function (PTR<Holder> pointer) :mFunction1 (pointer) ,mFunction2 (NULL) {}

public:
	//@info: the function is incompleted without 'csc_ext.hpp'
	template <class... _ARGS>
	inline static Function make (PTR<TYPE1 (TYPES... ,_ARGS...)> functor ,const REMOVE_CVR_TYPE<_ARGS> &...args) ;
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

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping override {
		return mFunctor (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
using FIX_MSVC_DEDUCTION_2 = DEF<DEF<TYPE1 (TYPES...)> NONE::*> ;

template <class TYPE1 ,class... TYPES>
class Function<FIX_MSVC_DEDUCTION_2<TYPE1 ,TYPES...>> {
private:
	class VariantHolder ;

	struct Holder :public Interface {
		virtual void address_copy (PTR<TEMP<VariantHolder>> address) const noexcept = 0 ;
		virtual TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping = 0 ;
	} ;

	template <class>
	class ImplHolder ;

	class VariantHolder :public Holder {
	private:
		PTR<NONE> mContext ;
		DEF<DEF<TYPE1 (TYPES...)> NONE::*> mFunction ;

	public:
		inline VariantHolder () = delete ;

		inline void address_copy (PTR<TEMP<VariantHolder>> address) const noexcept override ;
		inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping override ;
	} ;

private:
	TEMP<VariantHolder> mHolder ;

public:
	inline Function () = delete ;

	template <class _ARG>
	inline explicit Function (const PhanRef<_ARG> &context ,DEF<DEF<TYPE1 (TYPES...)> _ARG::*> function) noexcept {
		_DEBUG_ASSERT_ (function != NULL) ;
		ImplHolder<_ARG>::address_create (&mHolder ,&context.self ,function) ;
	}

	template <class _ARG>
	inline explicit Function (const PhanRef<const _ARG> &context ,DEF<DEF<TYPE1 (TYPES...) const> _ARG::*> function) noexcept {
		_DEBUG_ASSERT_ (function != NULL) ;
		ImplHolder<const _ARG>::address_create (&mHolder ,&context.self ,function) ;
	}

	inline ~Function () noexcept {
		if (!exist ())
			return ;
		_XVALUE_<const Holder &> (_CAST_<VariantHolder> (mHolder)).~Holder () ;
		_ZERO_ (mHolder) ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&right) noexcept {
		_ZERO_ (mHolder) ;
		if (!right.exist ())
			return ;
		_XVALUE_<const Holder &> (_CAST_<VariantHolder> (right.mHolder)).address_copy (&mHolder) ;
	}

	inline Function &operator= (Function &&right) noexcept {
		if (this != &right) {
			this->~Function () ;
			new (this) Function (std::move (right)) ;
		}
		return _XVALUE_<Function &> (*this) ;
	}

	inline BOOL exist () const {
		const auto r1x = _XVALUE_<const PACK<BYTE[_SIZEOF_ (TEMP<VariantHolder>)]> &> ({0}) ;
		return !_MEMEQUAL_ (_CAST_<BYTE[_SIZEOF_ (TEMP<VariantHolder>)]> (mHolder) ,PTRTOARR[&r1x.P1[0]]) ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		return _XVALUE_<const Holder &> (_CAST_<VariantHolder> (mHolder)).invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

	inline TYPE1 operator() (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class _TYPE>
class Function<FIX_MSVC_DEDUCTION_2<TYPE1 ,TYPES...>>::ImplHolder :public Holder {
private:
	PTR<_TYPE> mContext ;
	DEF<DEF<TYPE1 (TYPES...)> _TYPE::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<_TYPE> context ,DEF<DEF<TYPE1 (TYPES...)> _TYPE::*> function) noexcept :mContext (context) ,mFunction (function) {}

	inline void address_copy (PTR<TEMP<VariantHolder>> address) const noexcept override {
		address_create (address ,mContext ,mFunction) ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping override {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

public:
	inline static void address_create (PTR<TEMP<VariantHolder>> address ,PTR<_TYPE> context ,DEF<DEF<TYPE1 (TYPES...)> _TYPE::*> function) noexcept {
		_STATIC_ASSERT_ (_SIZEOF_ (TEMP<VariantHolder>) >= _SIZEOF_ (TEMP<ImplHolder>)) ;
		_DEBUG_ASSERT_ (address != NULL) ;
		const auto r1x = &_LOAD_<TEMP<ImplHolder>> (address->unused) ;
		_DEBUG_ASSERT_ (_ADDRESS_ (&_XVALUE_<const Holder &> (_CAST_<VariantHolder> (*address))) == _ADDRESS_ (address)) ;
		_DEBUG_ASSERT_ (_ADDRESS_ (&_XVALUE_<const Holder &> (_CAST_<ImplHolder> (*r1x))) == _ADDRESS_ (r1x)) ;
		_CREATE_ (r1x ,context ,function) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class _TYPE>
class Function<FIX_MSVC_DEDUCTION_2<TYPE1 ,TYPES...>>::ImplHolder<const _TYPE> :public Holder {
private:
	PTR<const _TYPE> mContext ;
	DEF<DEF<TYPE1 (TYPES...) const> _TYPE::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<const _TYPE> context ,DEF<DEF<TYPE1 (TYPES...) const> _TYPE::*> function) noexcept :mContext (context) ,mFunction (function) {}

	inline void address_copy (PTR<TEMP<VariantHolder>> address) const noexcept override {
		address_create (address ,mContext ,mFunction) ;
	}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping override {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)...) ;
	}

public:
	inline static void address_create (PTR<TEMP<VariantHolder>> address ,PTR<const _TYPE> context ,DEF<DEF<TYPE1 (TYPES...) const> _TYPE::*> function) noexcept popping {
		_STATIC_ASSERT_ (_SIZEOF_ (TEMP<VariantHolder>) >= _SIZEOF_ (TEMP<ImplHolder>)) ;
		_DEBUG_ASSERT_ (address != NULL) ;
		const auto r1x = &_LOAD_<TEMP<ImplHolder>> (address->unused) ;
		_DEBUG_ASSERT_ (_ADDRESS_ (&_XVALUE_<const Holder &> (_CAST_<VariantHolder> (*address))) == _ADDRESS_ (address)) ;
		_DEBUG_ASSERT_ (_ADDRESS_ (&_XVALUE_<const Holder &> (_CAST_<ImplHolder> (*r1x))) == _ADDRESS_ (r1x)) ;
		_CREATE_ (r1x ,context ,function) ;
	}
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

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (len >= 0 && len <= SIZE) ;
	}

	inline implicit Buffer (const DEF<TYPE[SIZE]> &right) :Buffer (std::move (Buffer::from (right))) {}

	inline implicit Buffer (DEF<TYPE[SIZE]> &&right) : Buffer (std::move (Buffer::from (right))) {}

	inline ARR<TYPE> &to () {
		return PTRTOARR[&mBuffer[0]] ;
	}

	inline implicit operator ARR<TYPE> & () {
		return to () ;
	}

	inline implicit operator PTR<TYPE> () = delete ;

	inline const ARR<TYPE> &to () const {
		return PTRTOARR[&mBuffer[0]] ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline implicit operator PTR<const TYPE> () = delete ;

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

	inline INDEX at (const TYPE &item) const {
		INDEX ret = &item - mBuffer ;
		if (ret < 0 || ret >= size ())
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &right) const {
		return _MEMEQUAL_ (mBuffer ,PTRTOARR[&right.mBuffer[0]]) ;
	}

	inline BOOL operator== (const Buffer &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Buffer &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Buffer &right) const {
		return _MEMLESS_ (mBuffer ,PTRTOARR[&right.mBuffer[0]]) ;
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
		return Buffer () ;
	}

	inline void expand (Buffer &&arg) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

public:
	inline static Buffer &from (DEF<TYPE[SIZE]> &_self) {
		return _CAST_<Buffer> (_self) ;
	}

	inline static const Buffer &from (const DEF<TYPE[SIZE]> &_self) {
		return _CAST_<Buffer> (_self) ;
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
		mBuffer = NULL ;
		mSize = 0 ;
		if (len == 0)
			return ;
		auto sgd = GlobalHeap::alloc<TEMP<TYPE>> (len) ;
		ScopedHolder<ARR<TYPE>> ANONYMOUS (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd) ,len) ;
		mBuffer = &_LOAD_<ARR<TYPE>> (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd)) ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (INDEX i = 0 ; i < mSize ; i++)
			(*mBuffer)[i].~TYPE () ;
		GlobalHeap::free (mBuffer) ;
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

	inline implicit operator PTR<TYPE> () = delete ;

	inline const ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline implicit operator PTR<const TYPE> () = delete ;

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

	inline INDEX at (const TYPE &item) const {
		INDEX ret = &item - *mBuffer ;
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
class Buffer<SPECIALIZATION<TYPE ,FALSE> ,SAUTO> {
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
		mBuffer = NULL ;
		mSize = 0 ;
		if (len == 0)
			return ;
		auto sgd = GlobalHeap::alloc<TEMP<TYPE>> (len) ;
		ScopedHolder<ARR<TYPE>> ANONYMOUS (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd) ,len) ;
		mBuffer = &_LOAD_<ARR<TYPE>> (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd)) ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (INDEX i = 0 ; i < mSize ; i++)
			(*mBuffer)[i].~TYPE () ;
		GlobalHeap::free (mBuffer) ;
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
		if (this != &right) {
			this->~Buffer () ;
			new (this) Buffer (std::move (right)) ;
		}
		return _XVALUE_<Buffer &> (*this) ;
	}
} ;

template <class TYPE>
class Buffer<SPECIALIZATION<TYPE ,TRUE> ,SAUTO> {
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
		mBuffer = NULL ;
		mSize = 0 ;
		if (len == 0)
			return ;
		auto sgd = GlobalHeap::alloc<TEMP<TYPE>> (len) ;
		ScopedHolder<ARR<TYPE>> ANONYMOUS (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd) ,len) ;
		mBuffer = &_LOAD_<ARR<TYPE>> (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd)) ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (INDEX i = 0 ; i < mSize ; i++)
			(*mBuffer)[i].~TYPE () ;
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &right) {
		mBuffer = NULL ;
		mSize = 0 ;
		if (right.mSize == 0)
			return ;
		auto sgd = GlobalHeap::alloc<TEMP<TYPE>> (right.mSize) ;
		ScopedHolder<ARR<TYPE>> ANONYMOUS (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd) ,*right.mBuffer ,right.mSize) ;
		mBuffer = &_LOAD_<ARR<TYPE>> (_XVALUE_<const PTR<ARR<TEMP<TYPE>>> &> (sgd)) ;
		mSize = right.mSize ;
		sgd = NULL ;
	}

	inline Buffer &operator= (const Buffer &right) {
		if (this != &right) {
			this->~Buffer () ;
			new (this) Buffer (std::move (right)) ;
		}
		return _XVALUE_<Buffer &> (*this) ;
	}

	inline Buffer (Buffer &&right) noexcept {
		mBuffer = _EXCHANGE_ (right.mBuffer) ;
		mSize = _EXCHANGE_ (right.mSize) ;
	}

	inline Buffer &operator= (Buffer &&right) noexcept {
		if (this != &right) {
			this->~Buffer () ;
			new (this) Buffer (std::move (right)) ;
		}
		return _XVALUE_<Buffer &> (*this) ;
	}
} ;

template <class TYPE>
class Buffer<TYPE ,SAUTO> :private Buffer<SPECIALIZATION<TYPE ,(std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value)> ,SAUTO> {
private:
	using SPECIALIZATION_BASE = Buffer<SPECIALIZATION<TYPE ,(std::is_copy_constructible<TYPE>::value && std::is_nothrow_move_constructible<TYPE>::value)> ,SAUTO> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mBuffer ;
	using SPECIALIZATION_BASE::mSize ;

public:
	inline Buffer () = default ;

	inline explicit Buffer (LENGTH len) :SPECIALIZATION_BASE (len) {}

	inline ARR<TYPE> &to () {
		return *mBuffer ;
	}

	inline implicit operator ARR<TYPE> & () {
		return to () ;
	}

	inline implicit operator PTR<TYPE> () = delete ;

	inline const ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline implicit operator PTR<const TYPE> () = delete ;

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

	inline INDEX at (const TYPE &item) const {
		INDEX ret = &item - *mBuffer ;
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
		const auto r1x = _MAX_ (LENGTH (VALX_SQRT2 * mSize) ,(mSize + DEFAULT_RECURSIVE_SIZE::value)) ;
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
		if (this != &right) {
			this->~Buffer () ;
			new (this) Buffer (std::move (right)) ;
		}
		return _XVALUE_<Buffer &> (*this) ;
	}

	inline const ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<TYPE> & () const {
		return to () ;
	}

	inline implicit operator PTR<const TYPE> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline const TYPE &get (INDEX index) const {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
#pragma GCC diagnostic pop
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &item) const {
		INDEX ret = &item - *mBuffer ;
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
		return Buffer () ;
	}

	inline void expand (Buffer &&arg) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

private:
	inline explicit Buffer (PTR<const ARR<TYPE>> src ,LENGTH len) :mBuffer (src) ,mSize (len) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static Buffer make (const ARR<TYPE> &src ,LENGTH len) {
		_DEBUG_ASSERT_ (src != NULL || len == 0) ;
		_DEBUG_ASSERT_ (len >= 0) ;
		const auto r1x = (len > 0) ? (&src) : NULL ;
		return Buffer (r1x ,len) ;
	}

	template <LENGTH _VAL>
	inline static Buffer make (const DEF<TYPE[_VAL]> &src) {
		return make (PTRTOARR[&src[0]] ,_VAL) ;
	}

	template <class _ARG>
	inline static Buffer make (const Buffer<TYPE ,_ARG> &src) {
		return make (src ,src.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<std::is_same<TYPE ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && LOAD_TRATIS_TYPE<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (const Buffer<_ARG1 ,_ARG2> &src) {
		if (src.size () == 0)
			return Buffer () ;
		return make (_LOAD_<ARR<TYPE>> (&src.self) ,src.size () * _SIZEOF_ (_ARG1)) ;
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
		if (this != &right) {
			this->~Buffer () ;
			new (this) Buffer (std::move (right)) ;
		}
		return _XVALUE_<Buffer &> (*this) ;
	}

	inline ARR<TYPE> &to () const {
		return *mBuffer ;
	}

	inline implicit operator ARR<TYPE> & () const {
		return to () ;
	}

	inline implicit operator PTR<TYPE> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline TYPE &get (INDEX index) const {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
#pragma GCC diagnostic pop
	}

	inline TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &item) const {
		INDEX ret = &item - *mBuffer ;
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
		return Buffer () ;
	}

	inline void expand (Buffer &&arg) {
		_DEBUG_ASSERT_ (FALSE) ;
	}

private:
	inline explicit Buffer (PTR<ARR<TYPE>> src ,LENGTH len) :mBuffer (src) ,mSize (len) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static Buffer make (ARR<TYPE> &src ,LENGTH len) {
		_DEBUG_ASSERT_ (src != NULL || len == 0) ;
		_DEBUG_ASSERT_ (len >= 0) ;
		const auto r1x = (len > 0) ? (&src) : NULL ;
		return Buffer (r1x ,len) ;
	}

	template <LENGTH _VAL>
	inline static Buffer make (DEF<TYPE[_VAL]> &src) {
		return make (PTRTOARR[&src[0]] ,_VAL) ;
	}

	template <class _ARG>
	inline static Buffer make (Buffer<TYPE ,_ARG> &src) {
		return make (src.self ,src.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<std::is_same<TYPE ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && LOAD_TRATIS_TYPE<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (Buffer<_ARG1 ,_ARG2> &src) {
		if (src.size () == 0)
			return Buffer () ;
		return make (_LOAD_<ARR<TYPE>> (&src.self) ,src.size () * _SIZEOF_ (_ARG1)) ;
	}

	inline static Buffer make (const Buffer<TYPE ,SMPHAN> &src) {
		return make (src.self ,src.size ()) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<TYPE ,BYTE>::value && !std::is_same<_ARG ,BYTE>::value && LOAD_TRATIS_TYPE<ARR<BYTE> ,ARR<_ARG>>::value>>
	inline static Buffer make (const Buffer<_ARG ,SMPHAN> &src) {
		if (src.size () == 0)
			return Buffer () ;
		return make (_LOAD_<ARR<TYPE>> (&src.self) ,src.size () * _SIZEOF_ (_ARG)) ;
	}
} ;

template <class TYPE>
using PhanBuffer = Buffer<REMOVE_CONST_TYPE<TYPE> ,CONDITIONAL_TYPE<std::is_const<TYPE>::value ,SCPHAN ,SMPHAN>> ;

template <class ,class>
class Allocator ;

template <class TYPE ,class SIZE>
class Allocator<SPECIALIZATION<TYPE ,FALSE ,FALSE> ,SIZE> {
private:
	using SPECIALIZATION_TYPE = Allocator<TYPE ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Pack {
	private:
		friend Allocator ;
		friend SPECIALIZATION_TYPE ;
		TEMP<TYPE> mData ;
		INDEX mNext ;
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Pack ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_free (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_free (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;
	inline Allocator (Allocator &&) = delete ;
	inline Allocator &operator= (Allocator &&) = delete ;
} ;

template <class TYPE ,class SIZE>
class Allocator<SPECIALIZATION<TYPE ,FALSE ,TRUE> ,SIZE> {
private:
	using SPECIALIZATION_TYPE = Allocator<TYPE ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Pack {
	private:
		friend Allocator ;
		friend SPECIALIZATION_TYPE ;
		TEMP<TYPE> mData ;
		INDEX mNext ;
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Pack ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_free (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_free (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;

	inline Allocator (Allocator &&right) noexcept :mAllocator (std::move (right.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		INDEX iw = (std::is_pod<TYPE>::value) ? (right.mAllocator.size ()) : VAR_ZERO ;
		for (INDEX i = iw ; i < right.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<TYPE> (right.mAllocator[i].mData))) ;
			iw++ ;
		}
		mLength = right.mLength ;
		mFree = right.mFree ;
		if (right.mAllocator.size () == 0)
			return ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (&right)->clear () ;
	}

	inline Allocator &operator= (Allocator &&right) noexcept {
		if (this != &right) {
			this->~Allocator () ;
			new (this) Allocator (std::move (right)) ;
		}
		return _XVALUE_<Allocator &> (*this) ;
	}
} ;

template <class TYPE ,class SIZE>
class Allocator<SPECIALIZATION<TYPE ,TRUE ,TRUE> ,SIZE> {
private:
	using SPECIALIZATION_TYPE = Allocator<TYPE ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Pack {
	private:
		friend Allocator ;
		friend SPECIALIZATION_TYPE ;
		TEMP<TYPE> mData ;
		INDEX mNext ;
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Pack ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_free (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->update_free (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		static_cast<PTR<SPECIALIZATION_TYPE>> (this)->clear () ;
	}

	inline Allocator (const Allocator &right) :mAllocator (std::move (right.mAllocator)) {
		INDEX iw = (std::is_pod<TYPE>::value) ? (right.mAllocator.size ()) : 0 ;
		const auto r1x = [&] () noexcept popping {
			for (INDEX i = 0 ; i < iw ; i++) {
				if (mAllocator[i].mNext != VAR_USED)
					continue ;
				_DESTROY_ (&mAllocator[i].mData) ;
			}
		iw = VAR_NONE ;
		} ;
		class Finally :private Wrapped<decltype (r1x)> {
		public:
			inline void lock () const noexcept {
				_STATIC_WARNING_ ("noop") ;
			}

			inline void unlock () const noexcept {
				Finally::mData () ;
			}
		} ;
		ScopedGuard<const Finally> ANONYMOUS (_CAST_<Finally> (r1x)) ;
		for (INDEX i = iw ; i < right.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<TYPE> (right.mAllocator[i].mData))) ;
			iw++ ;
		}
		mLength = right.mLength ;
		mFree = right.mFree ;
		iw = VAR_NONE ;
	}

	inline Allocator &operator= (const Allocator &right) {
		if (this != &right) {
			this->~Allocator () ;
			new (this) Allocator (std::move (right)) ;
		}
		return _XVALUE_<Allocator &> (*this) ;
	}

	inline Allocator (Allocator &&right) noexcept :mAllocator (std::move (right.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		INDEX iw = (std::is_pod<TYPE>::value) ? (right.mAllocator.size ()) : VAR_ZERO ;
		for (INDEX i = iw ; i < right.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<TYPE> (right.mAllocator[i].mData))) ;
			iw++ ;
		}
		mLength = right.mLength ;
		mFree = right.mFree ;
		if (right.mAllocator.size () == 0)
			return ;
		static_cast<PTR<SPECIALIZATION_TYPE>> (&right)->clear () ;
	}

	inline Allocator &operator= (Allocator &&right) noexcept {
		if (this != &right) {
			this->~Allocator () ;
			new (this) Allocator (std::move (right)) ;
		}
		return _XVALUE_<Allocator &> (*this) ;
	}
} ;

template <class TYPE ,class SIZE>
class Allocator :private Allocator<SPECIALIZATION<TYPE ,(std::is_copy_constructible<Buffer<TYPE ,SIZE>>::value && std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value) ,std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Allocator<SPECIALIZATION<TYPE ,(std::is_copy_constructible<Buffer<TYPE ,SIZE>>::value && std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value) ,std::is_nothrow_move_constructible<Buffer<TYPE ,SIZE>>::value> ,SIZE> ;
	using Pack = typename SPECIALIZATION_BASE::Pack ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mAllocator ;
	using SPECIALIZATION_BASE::mLength ;
	using SPECIALIZATION_BASE::mFree ;

public:
	inline Allocator () = default ;

	inline explicit Allocator (LENGTH len) :SPECIALIZATION_BASE (len) {}

	inline LENGTH size () const {
		return mAllocator.size () ;
	}

	inline LENGTH length () const {
		if (mAllocator.size () == 0)
			return 0 ;
		return mLength ;
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

	inline BOOL used (INDEX index) const {
		return mAllocator[index].mNext == VAR_USED ;
	}

	inline TYPE &get (INDEX index) {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<TYPE> (mAllocator[index].mData) ;
	}

	inline TYPE &operator[] (INDEX index) {
		return get (index) ;
	}

	inline const TYPE &get (INDEX index) const {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<TYPE> (mAllocator[index].mData) ;
	}

	inline const TYPE &operator[] (INDEX index) const {
		return get (index) ;
	}

	inline INDEX at (const TYPE &item) const {
		INDEX ret = mAllocator.at (_OFFSET_ (&Pack::mData ,_CAST_<TEMP<TYPE>> (item))) ;
		if (!used (ret))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	template <class... _ARGS>
	inline INDEX alloc (_ARGS &&...args) popping {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		if (mFree == VAR_NONE) {
			auto rax = mAllocator.expand () ;
			_CREATE_ (&rax[mLength].mData ,std::forward<_ARGS> (args)...) ;
			for (INDEX i = 0 ; i < mAllocator.size () ; i++) {
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<TYPE> (mAllocator[i].mData))) ;
				rax[i].mNext = VAR_USED ;
			}
			mAllocator.expand (std::move (rax)) ;
			update_free (mLength ,mFree) ;
		} else if (mFree != VAR_NONE) {
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
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<TYPE> (mAllocator[i].mData))) ;
			rax[i].mNext = mAllocator[i].mNext ;
		}
		mAllocator.expand (std::move (rax)) ;
		update_free (r1x ,mFree) ;
	}

	inline void clean () {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<TYPE>::value && std::is_nothrow_move_assignable<TYPE>::value) ;
		const auto r1x = shrink_size () ;
		if (r1x == mAllocator.size ())
			return ;
		auto rax = mAllocator.expand (r1x) ;
		mFree = VAR_NONE ;
		for (INDEX i = r1x - 1 ; i >= 0 ; i--) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<TYPE> (mAllocator[i].mData))) ;
			rax[i].mNext = mAllocator[i].mNext ;
			if (rax[i].mNext == VAR_USED)
				continue ;
			rax[i].mNext = mFree ;
			mFree = i ;
		}
		mAllocator.expand (std::move (rax)) ;
	}

private:
	inline void update_free (INDEX _length ,INDEX _free) {
		mLength = _length ;
		mFree = _free ;
		for (INDEX i = mAllocator.size () - 1 ; i >= _length ; i--) {
			mAllocator[i].mNext = mFree ;
			mFree = i ;
		}
	}

	inline LENGTH shrink_size () const {
		LENGTH ret = mAllocator.size () ;
		while (ret - 1 >= 0 && mAllocator[ret - 1].mNext != VAR_USED)
			ret-- ;
		return std::move (ret) ;
	}
} ;
} ;