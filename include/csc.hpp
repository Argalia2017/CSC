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
#pragma warning (default :26409) //@info: warning C26409: Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
#pragma warning (default :26410) //@info: warning C26410: The parameter 'xxx' is a reference to const unique pointer, use const T* or const T& instead (r.32).
#pragma warning (default :26411) //@info: warning C26411: The parameter 'xxx' is a reference to unique pointer and it is never reassigned or reset, use T* or T& instead (r.33).
#pragma warning (default :26415) //@info: warning C26415: Smart pointer parameter 'xxx' is used only to access contained pointer. Use T* or T& instead (r.30).
#pragma warning (default :26416) //@info: warning C26416: Shared pointer parameter 'xxx' is passed by rvalue reference. Pass by value instead (r.34).
#pragma warning (default :26426) //@info: warning C26426: Global initializer calls a non-constexpr function 'xxx' (i.22).
#pragma warning (default :26429) //@info: warning C26429: Symbol 'xxx' is never tested for nullness, it can be marked as not_null (f.23).
#pragma warning (default :26432) //@info: warning C26432: If you define or delete any default operation in the type 'xxx', define or delete them all (c.21).
#pragma warning (default :26434) //@info: warning C26434: Function 'xxx' hides a non-virtual function 'xxx' (c.128).
#pragma warning (default :26435) //@info: warning C26435: Function 'xxx' should specify exactly one of 'virtual', 'override', or 'final' (c.128).
#pragma warning (default :26439) //@info: warning C26439: This kind of function may not throw. Declare it 'noexcept' (f.6).
#pragma warning (default :26440) //@info: warning C26440: Function 'xxx' can be declared 'noexcept' (f.6).
#pragma warning (default :26443) //@info: warning C26443: Overriding destructor should not use explicit 'override' or 'virtual' specifiers (c.128).
#pragma warning (default :26446) //@info: warning C26446: Prefer to use gsl::at() instead of unchecked subscript operator (bounds.4).
#pragma warning (default :26447) //@info: warning C26447: The function is declared 'noexcept' but calls function 'xxx' which may throw exceptions (f.6).
#pragma warning (default :26455) //@info: warning C26455: Default constructor may not throw. Declare it 'noexcept' (f.6).
#pragma warning (default :26460) //@info: warning C26460: The reference argument 'xxx' for function 'xxx' can be marked as const (con.3).
#pragma warning (default :26466) //@info: warning C26466: Don't use static_cast downcasts. A cast from a polymorphic type should use dynamic_cast (type.2).
#pragma warning (default :26471) //@info: warning C26471: Don't use reinterpret_cast. A cast from void* can use static_cast (type.1).
#pragma warning (default :26473) //@info: warning C26473: Don't cast between pointer types where the source type and the target type are the same (type.1).
#pragma warning (default :26475) //@info: warning C26475: Do not use function style C-casts (es.49).
#pragma warning (default :26481) //@info: warning C26481: Don't use pointer arithmetic. Use span instead (bounds.1).
#pragma warning (default :26482) //@info: warning C26482: Only index into arrays using constant expressions (bounds.2).
#pragma warning (default :26485) //@info: warning C26485: Expression 'xxx': No array to pointer decay (bounds.3).
#pragma warning (default :26486) //@info: warning C26486: Don't pass a pointer that may be invalid to a function. Parameter ? 'xxx' in call to 'xxx' may be invalid (lifetime.1).
#pragma warning (default :26487) //@info: warning C26487: Don't return a pointer that may be invalid (lifetime.1).
#pragma warning (default :26489) //@info: warning C26489: Don't dereference a pointer that may be invalid: 'xxx'. 'xxx' may have been invalidated at line ? (lifetime.1).
#pragma warning (default :26490) //@info: warning C26490: Don't use reinterpret_cast (type.1).
#pragma warning (default :26491) //@info: warning C26491: Don't use static_cast downcasts (type.2).
#pragma warning (default :26493) //@info: warning C26493: Don't use C-style casts (type.4).
#pragma warning (default :26495) //@info: warning C26495: Variable 'xxx' is uninitialized. Always initialize a member variable (type.6).
#pragma warning (default :26496) //@info: warning C26496: The variable 'xxx' is assigned only once, mark it as const (con.4).
#endif
#endif

#ifdef __CSC__
#ifdef self
#error "∑(っ°Д° ;)っ : defined 'self'"
#endif
#define self to ()

#ifdef implicit
#error "∑(っ°Д° ;)っ : defined 'implicit'"
#endif
#define implicit

#ifdef popping
#error "∑(っ°Д° ;)っ : defined 'popping'"
#endif
#define popping

#ifdef imports
#error "∑(っ°Д° ;)っ : defined 'imports'"
#endif
#define imports extern

#ifdef exports
#error "∑(っ°Д° ;)っ : defined 'exports'"
#endif
#define exports

#ifdef discard
#error "∑(っ°Д° ;)っ : defined 'discard'"
#endif
#define discard break
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

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <limits>
#include <type_traits>
#include <initializer_list>
#include <new>
#include <typeinfo>
#include <utility>

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("discard")
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

#ifndef __CSC_COMPILER_GNUC__
//@error: 'std::is_trivial' is not avaliable in g++4.8
using std::is_trivial ;
#endif

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

using std::enable_if ;
using std::conditional ;
using std::remove_reference ;
using std::remove_const ;
using std::remove_volatile ;
using std::remove_pointer ;
using std::remove_extent ;
} ;

#define _UNWIND_X_(...) __VA_ARGS__
#define _UNW_(...) _UNWIND_X_(__VA_ARGS__)
#define _STRINGIZE_X_(...) #__VA_ARGS__
#define _STR_(...) _STRINGIZE_X_(__VA_ARGS__)
#define _CONCAT_X_(arg1 ,arg2) arg1 ## arg2
#define _CAT_(arg1 ,arg2) _CONCAT_X_(arg1 ,arg2)

#define M_DATE __DATE__
#define M_HOUR __TIME__
#define M_FILE __FILE__
#define M_LINE _STR_ (__LINE__)

#ifdef __CSC_COMPILER_MSVC__
#define M_FUNC __FUNCSIG__
#elif defined __CSC_COMPILER_GNUC__
#define M_FUNC __PRETTY_FUNCTION__
#elif defined __CSC_COMPILER_CLANG__
#define M_FUNC __PRETTY_FUNCTION__
#else
#define M_FUNC __func__
#endif

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

#ifdef __CSC_COMPILER_MSVC__
#define _DYNAMIC_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception (_PCSTR_ ("dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," M_LINE)) ; } while (FALSE)
#else
#define _DYNAMIC_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) throw CSC::Exception (CSC::Plain<CSC::STR> (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,"dynamic_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " ,M_FILE ," ," ,M_LINE)) ; } while (FALSE)
#endif

#ifdef __CSC_UNITTEST__
#define _UNITTEST_WATCH_(...) do { struct ARGVPL ; CSC::GlobalWatch::done (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,_PCSTR_ (_STR_ (__VA_ARGS__)) ,(_UNW_ (__VA_ARGS__))) ; } while (FALSE)
#else
#define _UNITTEST_WATCH_(...) do {} while (FALSE)
#endif

#define ANONYMOUS _CAT_ (_anonymous_ ,__LINE__)

#ifdef __CSC_COMPILER_MSVC__
#define FOR_ONCE_DO_WHILE auto ANONYMOUS : CSC::DEF<CSC::ARGVS<>[1]> {}
#elif defined __CSC_COMPILER_GNUC__
#define FOR_ONCE_DO_WHILE __attribute__ ((unused)) auto &ANONYMOUS : CSC::_NULL_<CSC::DEF<int[1]>> ()
#elif defined __CSC_COMPILER_CLANG__
#define FOR_ONCE_DO_WHILE __attribute__ ((unused)) auto &ANONYMOUS : CSC::_NULL_<CSC::DEF<int[1]>> ()
#else
#define FOR_ONCE_DO_WHILE auto &ANONYMOUS : CSC::_NULL_<CSC::DEF<int[1]>> ()
#endif

#define _CASE_REQUIRE_(...) do {if (!(_UNW_ (__VA_ARGS__))) return (void) (_case_req = FALSE) ; } while (FALSE)

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

#define _SIZEOF_(...) (CSC::LENGTH (sizeof (CSC::U::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>)))
#define _ALIGNOF_(...) (CSC::LENGTH (alignof (CSC::U::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>)))
#define _CAPACITYOF_(...) (CSC::LENGTH (sizeof... (_UNW_ (__VA_ARGS__))))
#define _COUNTOF_(...) (CSC::LENGTH (CSC::U::COUNTOF_TRAITS_TYPE<CSC::U::REMOVE_CVR_TYPE<_UNW_ (__VA_ARGS__)>>::value))

using VAL32 = float ;
using VAL64 = double ;

static constexpr auto VAL32_MAX = VAL32 (3.402823466E+38) ;
static constexpr auto VAL32_MIN = VAL32 (1.175494351E-38) ;
static constexpr auto VAL32_EPS = VAL32 (1.192092896E-07) ;
static constexpr auto VAL32_INF = std::numeric_limits<VAL32>::infinity () ;
static constexpr auto VAL32_NAN = std::numeric_limits<VAL32>::quiet_NaN () ;
static constexpr auto VAL64_MAX = VAL64 (1.7976931348623158E+308) ;
static constexpr auto VAL64_MIN = VAL64 (2.2250738585072014E-308) ;
static constexpr auto VAL64_EPS = VAL64 (2.2204460492503131E-016) ;
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

static constexpr auto MATH_E = VALX (2.71828182845904523536) ;
static constexpr auto MATH_PI = VALX (3.14159265358979323846) ;
static constexpr auto MATH_SQRT2 = VALX (1.41421356237309504880) ;
static constexpr auto MATH_LN2 = VALX (0.693147180559945309417) ;
static constexpr auto MATH_LN10 = VALX (2.30258509299404568402) ;
static constexpr auto MATH_GR = VALX (1.61803398874989484820) ;

inline namespace S {
template <class _ARG1>
inline constexpr _ARG1 _ABS_ (const _ARG1 &arg1) {
	return (arg1 < 0) ? (-arg1) : (+arg1) ;
}

template <class _ARG1>
inline constexpr _ARG1 _SQE_ (const _ARG1 &arg1) {
	return arg1 * arg1 ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_MIN_ (const _ARG1 &arg1 ,const _ARG1 &arg2) {
	return (!BOOL (arg2 < arg1)) ? arg1 : arg2 ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_MAX_ (const _ARG1 &arg1 ,const _ARG1 &arg2) {
	return (!BOOL (arg1 < arg2)) ? arg1 : arg2 ;
}
} ;

#ifdef VOID
#undef VOID
#endif
using VOID = void ;

#ifdef NONE
#undef NONE
#endif
struct NONE ;

#ifdef NULL
#undef NULL
#endif
#define NULL nullptr

template <class TYPE>
using DEF = TYPE ;

template <class TYPE>
using PTR = DEF<TYPE *> ;

#ifdef __CSC_COMPILER_GNUC__
template <class TYPE>
using ARR = DEF<TYPE[0]> ;
#else
template <class TYPE>
using ARR = DEF<TYPE[]> ;
#endif

namespace U {
struct OPERATOR_PTRTOARR {
	template <class _ARG1>
	inline constexpr ARR<_ARG1> &operator[] (const PTR<_ARG1> &that) const {
		return *PTR<ARR<_ARG1>> (that) ;
	}

	template <class _ARG1 ,LENGTH _VAL1>
	inline constexpr ARR<_ARG1> &operator[] (DEF<_ARG1[_VAL1]> &that) const {
		return *PTR<ARR<_ARG1>> (&that) ;
	}
} ;
} ;

static constexpr auto PTRTOARR = U::OPERATOR_PTRTOARR {} ;

using BYTE = std::uint8_t ;
using WORD = std::uint16_t ;
using CHAR = std::uint32_t ;
using DATA = std::uint64_t ;

struct __uint128_t {
	alignas (16) DEF<BYTE[16]> unused ;
} ;

using MEGA = __uint128_t ;

using STRU8 = unsigned char ;
using STRU16 = char16_t ;
using STRU32 = char32_t ;

#define _PCSTRU8_(arg1) CSC::Plain<CSC::STRU8> (_CAT_ (u8 ,arg1))
#define _PCSTRU16_(arg1) CSC::Plain<CSC::STRU16> (_CAT_ (u ,arg1))
#define _PCSTRU32_(arg1) CSC::Plain<CSC::STRU32> (_CAT_ (U ,arg1))

using STRA = char ;
using STRW = wchar_t ;

#define _PCSTRA_(arg1) CSC::Plain<CSC::STRA> (_UNW_ (arg1))
#define _PCSTRW_(arg1) CSC::Plain<CSC::STRW> (_CAT_ (L ,arg1))

#ifdef __CSC_CONFIG_STRA__
using STR = STRA ;

#define _PCSTR_ _PCSTRA_
#elif defined __CSC_CONFIG_STRW__
using STR = STRW ;

#define _PCSTR_ _PCSTRW_
#endif

using STRX = signed char ;

template <VAR _VAL1>
struct ARGC {
	enum :VAR {
		value = _VAL1
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

template <VAR _VAL1>
struct ARGV<ARGVP<_VAL1>> :public ARGV<ARGVP<_VAL1 - 1>> {
	_STATIC_ASSERT_ (_VAL1 > 0) ;
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

static constexpr auto ARGVPX = ARGV<VOID> {} ;
static constexpr auto ARGVPY = ARGV<NONE> {} ;

using DEFAULT_RECURSIVE_SIZE = ARGC<256> ;
using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
using DEFAULT_LONGSTRING_SIZE = ARGC<8195> ;
using DEFAULT_HUGEBUFFER_SIZE = ARGC<8388608> ;
using DEFAULT_TIMEOUT_SIZE = ARGC<30000> ;
using DEFAULT_RETRYTIMES_SIZE = ARGC<64> ;
using DEFAULT_EXPANDFIRST_SIZE = ARGC<256> ;
using DEFAULT_EXPANDGUARD_SIZE = ARGC<65536> ;

template <class ,BOOL...>
struct SPECIALIZATION ;

template <class>
struct TEMP ;

template <class...>
struct PACK ;

template <>
struct PACK<> {} ;

template <class UNIT1>
struct PACK<UNIT1> {
	UNIT1 P1 ;
} ;

template <class UNIT1 ,class UNIT2>
struct PACK<UNIT1 ,UNIT2> {
	UNIT1 P1 ;
	UNIT2 P2 ;
} ;

template <class UNIT1 ,class UNIT2 ,class UNIT3>
struct PACK<UNIT1 ,UNIT2 ,UNIT3> {
	UNIT1 P1 ;
	UNIT2 P2 ;
	UNIT3 P3 ;
} ;

#pragma region
#define __CSC_USE_TRAITS__
#include "csc.hpp.traits.inl"
#undef __CSC_USE_TRAITS__
#pragma endregion

template <BOOL _VAL1>
using ENABLE_TYPE = U::ENABLE_TYPE<_VAL1> ;

template <BOOL _VAL1 ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = U::CONDITIONAL_TYPE<_VAL1 ,_ARG1 ,_ARG2> ;

template <class _ARG1>
using REMOVE_REFERENCE_TYPE = U::REMOVE_REFERENCE_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_CONST_TYPE = U::REMOVE_CONST_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_VOLATILE_TYPE = U::REMOVE_VOLATILE_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_CVR_TYPE = U::REMOVE_CVR_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_POINTER_TYPE = U::REMOVE_POINTER_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_ARRAY_TYPE = U::REMOVE_ARRAY_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_MEMFUNC_TYPE = U::REMOVE_MEMFUNC_TYPE<_ARG1> ;

template <class _ARG1>
using REMOVE_MEMBER_TYPE = U::REMOVE_MEMBER_TYPE<_ARG1> ;

template <class _ARG1>
using MEMBER_OF_TYPE = U::MEMBER_OF_TYPE<_ARG1> ;

template <class _ARG1>
using INVOKE_RESULT_TYPE = U::INVOKE_RESULT_TYPE<_ARG1> ;

template <class _ARG1>
using INVOKE_PARAMS_TYPE = U::INVOKE_PARAMS_TYPE<_ARG1> ;

template <class _ARG1 ,class _ARG2>
using RESULT_OF_TYPE = U::RESULT_OF_TYPE<_ARG1 ,_ARG2> ;

template <class _ARG1>
using FORWARD_TRAITS_TYPE = U::FORWARD_TRAITS_TYPE<_ARG1> ;

template <class _ARG1 ,class _ARG2>
using CAST_TRAITS_TYPE = U::CAST_TRAITS_TYPE<_ARG1 ,_ARG2> ;

template <class _ARG1 ,class _ARG2>
using LOAD_CHECK_TYPE = U::LOAD_CHECK_TYPE<_ARG1 ,_ARG2> ;

namespace stl {
template <class _ARG1>
using is_var_xyz = U::is_var_xyz<_ARG1> ;

template <class _ARG1>
using is_val_xyz = U::is_val_xyz<_ARG1> ;

template <class _ARG1>
using is_byte_xyz = U::is_byte_xyz<_ARG1> ;

template <class _ARG1>
using is_str_xyz = U::is_str_xyz<_ARG1> ;

template <class _ARG1 ,class _ARG2>
using is_full_array_of = U::is_full_array_of<_ARG1 ,_ARG2> ;
} ;

template <class UNIT>
struct TEMP {
	_STATIC_ASSERT_ (!std::is_reference<UNIT>::value) ;
	alignas (UNIT) DEF<BYTE[_SIZEOF_ (UNIT)]> unused ;
} ;

template <class _RET>
inline constexpr _RET &_NULL_ () {
	return *PTR<REMOVE_REFERENCE_TYPE<_RET>> (NULL) ;
}

template <class _ARG1>
inline constexpr LENGTH _ADDRESS_ (const PTR<_ARG1> &address) {
	return LENGTH (address) ;
}

//@warn: not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_CAST_ (_ARG1 &arg1) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_RET>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_ARG1>::value || std::is_same<_RET ,TEMP<_ARG1>>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG1)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG1) % _ALIGNOF_ (_RET) == 0) ;
	return *reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (_ADDRESS_ (&arg1)) ;
}

//@warn: not type-safe ,be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_LOAD_ (PTR<_ARG1> address) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (LOAD_CHECK_TYPE<REMOVE_CVR_TYPE<_RET> ,REMOVE_CVR_TYPE<_ARG1>>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<(std::is_same<REMOVE_CVR_TYPE<_RET> ,VOID>::value || std::is_same<REMOVE_CVR_TYPE<_RET> ,NONE>::value) ,BYTE ,REMOVE_ARRAY_TYPE<_RET>>) ;
	_DEBUG_ASSERT_ (_ADDRESS_ (address) % r1x == 0) ;
	(void) r1x ;
	return *reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (address) ;
}

//@warn: not type-safe ,be careful about strict-aliasing
template <class _RET>
inline _RET &_LOAD_ (const DEF<decltype (NULL)> & ,LENGTH address) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	const auto r1x = PTR<VOID> (address) ;
	return _LOAD_<_RET> (r1x) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (const DEF<_ARG1 _ARG2::*> &arg1 ,_ARG3 &arg2) noexcept {
	_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG3> ,_ARG1>::value) ;
	_DEBUG_ASSERT_ (arg1 != NULL) ;
	const auto r1x = _ADDRESS_ (&arg2) - _ADDRESS_ (&(_NULL_<_ARG2> ().*arg1)) ;
	return _LOAD_<CAST_TRAITS_TYPE<_ARG2 ,_ARG3>> (NULL ,r1x) ;
}

template <class _RET>
inline REMOVE_REFERENCE_TYPE<_RET> &_XVALUE_ (REMOVE_CVR_TYPE<_RET> &arg1) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return arg1 ;
}

template <class _RET>
inline const REMOVE_REFERENCE_TYPE<_RET> &_XVALUE_ (const REMOVE_CVR_TYPE<_RET> &arg1) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return arg1 ;
}

template <class _ARG1>
inline void _ZERO_ (_ARG1 &arg1) noexcept {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_CAST_<TEMP<_ARG1>> (arg1) = TEMP<_ARG1> {0} ;
}

template <class _ARG1>
inline _ARG1 _EXCHANGE_ (_ARG1 &arg1) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_ARG1 ret = arg1 ;
	_ZERO_ (arg1) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline _ARG1 _EXCHANGE_ (_ARG1 &arg1 ,const REMOVE_CVR_TYPE<_ARG1> &arg2) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_ARG1 ret = arg1 ;
	arg1 = arg2 ;
	return std::move (ret) ;
}

template <class _ARG1>
inline _ARG1 _COPY_ (const _ARG1 &arg1) {
	return arg1 ;
}

template <class _ARG1>
inline void _SWAP_ (_ARG1 &arg1 ,_ARG1 &arg2) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_move_assignable<_ARG1>::value) ;
	auto rax = std::move (arg1) ;
	arg1 = std::move (arg2) ;
	arg2 = std::move (rax) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _SWAP_ (DEF<_ARG1[_VAL1]> &arg1 ,DEF<_ARG1[_VAL1]> &arg2) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_move_assignable<_ARG1>::value) ;
	_SWAP_ (_CAST_<PACK<_ARG1[_VAL1]>> (arg1) ,_CAST_<PACK<_ARG1[_VAL1]>> (arg2)) ;
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

template <class _ARG1>
inline void _DESTROY_ (PTR<TEMP<_ARG1>> address) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG1>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = &_LOAD_<_ARG1> (address) ;
	r1x->~_ARG1 () ;
	(void) r1x ;
}

template <class _ARG1>
inline FLAG _TYPEID_ (const ARGV<_ARG1> &) noexcept ;

template <class _RET>
inline FLAG _TYPEID_ () noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return _TYPEID_ (_NULL_<ARGV<REMOVE_CVR_TYPE<_RET>>> ()) ;
}

template <class _ARG1>
inline RESULT_OF_TYPE<_ARG1 ,ARGVS<>> _CALL_ (_ARG1 &&arg1) popping {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	return arg1 () ;
}

template <class _ARG1>
inline void _CALL_IF_ (_ARG1 &&arg1) {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<BOOL &>> ,void>::value) ;
	auto rax = TRUE ;
	arg1 (rax) ;
}

template <class _ARG1 ,class... _ARGS>
inline void _CALL_IF_ (_ARG1 &&arg1 ,_ARGS &&...args) {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<BOOL &>> ,void>::value) ;
	auto rax = TRUE ;
	arg1 (rax) ;
	if (rax)
		return ;
	_CALL_IF_ (std::forward<_ARGS> (args)...) ;
}

//@warn: assure ruined object when an exception was thrown
template <class _ARG1>
inline void _CALL_TRY_ (_ARG1 &&arg1) {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	arg1 () ;
}

//@warn: assure ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (_ARG1 &&arg1 ,_ARGS &&...args) ;

template <class _ARG1>
inline void _CALL_EH_ (_ARG1 &&arg1) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	arg1 () ;
}

//@info: the function is incompleted without 'csc_ext.hpp'
template <class _ARG1 ,class _ARG2>
inline void _CALL_EH_ (_ARG1 &&arg1 ,_ARG2 &&arg2) noexcept ;

template <class _ARG1>
inline const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> &_CACHE_ (_ARG1 &&arg1) popping {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	static const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> mInstance = arg1 () ;
	return mInstance ;
}

inline namespace S {
template <class _ARG1>
inline BOOL _MEMEQUAL_ (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src1 != NULL) ;
		_DEBUG_ASSERT_ (src2 != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	if (src1 == src2)
		return TRUE ;
	for (INDEX i = 0 ; i < len ; i++)
		if (!BOOL (src1[i] == src2[i]))
			return FALSE ;
	return TRUE ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline BOOL _MEMEQUAL_ (const ARR<_ARG1> &src1 ,const DEF<_ARG1[_VAL1]> &src2) {
	return _MEMEQUAL_ (src1 ,PTRTOARR[src2] ,_VAL1) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline BOOL _MEMEQUAL_ (const DEF<_ARG1[_VAL1]> &src1 ,const ARR<_ARG1> &src2) {
	return _MEMEQUAL_ (PTRTOARR[src1] ,src2 ,_VAL1) ;
}

template <class _ARG1>
inline FLAG _MEMCOMPR_ (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src1 != NULL) ;
		_DEBUG_ASSERT_ (src2 != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	FLAG ret = 0 ;
	INDEX ix = 0 ;
	if (src1 == src2)
		ix = len ;
	while (TRUE) {
		if (ix >= len)
			break ;
		for (FOR_ONCE_DO_WHILE) {
			if (ret != 0)
				discard ;
			if (!BOOL (src1[ix] < src2[ix]))
				discard ;
			ret = -1 ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (ret != 0)
				discard ;
			if (!BOOL (src2[ix] < src1[ix]))
				discard ;
			ret = +1 ;
		}
		if (ret != 0)
			break ;
		ix++ ;
	}
	return std::move (ret) ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline FLAG _MEMCOMPR_ (const ARR<_ARG1> &src1 ,const DEF<_ARG1[_VAL1]> &src2) {
	return _MEMCOMPR_ (src1 ,PTRTOARR[src2] ,_VAL1) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline FLAG _MEMCOMPR_ (const DEF<_ARG1[_VAL1]> &src1 ,const ARR<_ARG1> &src2) {
	return _MEMCOMPR_ (PTRTOARR[src1] ,src2 ,_VAL1) ;
}

template <class _ARG1>
inline FLAG _MEMHASH_ (const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_STATIC_ASSERT_ (std::is_same<_ARG1 ,BYTE>::value) ;
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
#ifdef __CSC_CONFIG_VAR32__
	static constexpr auto M_MAGIC_N1 = VAR (-2128831035) ;
	static constexpr auto M_MAGIC_N2 = VAR (16777619) ;
#elif defined __CSC_CONFIG_VAR64__
	static constexpr auto M_MAGIC_N1 = VAR (-3750763034362895579) ;
	static constexpr auto M_MAGIC_N2 = VAR (1099511628211) ;
#endif
	FLAG ret = M_MAGIC_N1 ;
	for (INDEX i = 0 ; i < len ; i++) {
		ret ^= FLAG (src[i]) ;
		ret *= M_MAGIC_N2 ;
	}
	ret &= VAR_MAX ;
	return std::move (ret) ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline FLAG _MEMHASH_ (const DEF<_ARG1[_VAL1]> &src) {
	return _MEMHASH_ (PTRTOARR[src] ,_VAL1) ;
}

template <class _ARG1>
inline INDEX _MEMCHR_ (const ARR<_ARG1> &src ,LENGTH len ,const _ARG1 &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	for (INDEX i = 0 ; i < len ; i++)
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline INDEX _MEMCHR_ (const DEF<_ARG1[_VAL1]> &src ,const _ARG1 &val) {
	return _MEMCHR_ (PTRTOARR[src] ,_VAL1 ,val) ;
}

template <class _ARG1>
inline INDEX _MEMRCHR_ (const ARR<_ARG1> &src ,LENGTH len ,const _ARG1 &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	for (INDEX i = 0 ; i < len ; i++)
		if (src[len + ~i] == val)
			return (len + ~i) ;
	return VAR_NONE ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline INDEX _MEMRCHR_ (const DEF<_ARG1[_VAL1]> &src ,const _ARG1 &val) {
	return _MEMRCHR_ (PTRTOARR[src] ,_VAL1 ,val) ;
}

template <class _ARG1>
inline void _MEMCOPY_ (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (dst != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	if (dst == src)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
	for (INDEX i = 0 ; i < len ; i++)
		dst[i] = src[i] ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMCOPY_ (ARR<_ARG1> &dst ,const DEF<_ARG1[_VAL1]> &src) {
	_MEMCOPY_ (dst ,PTRTOARR[src] ,_VAL1) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMCOPY_ (DEF<_ARG1[_VAL1]> &dst ,const ARR<_ARG1> &src) {
	_MEMCOPY_ (PTRTOARR[dst] ,src ,_VAL1) ;
}

template <class _ARG1>
inline void _MEMRCOPY_ (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (dst != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;
	_CALL_IF_ ([&] (BOOL &_case_req) {
		_CASE_REQUIRE_ (dst != src) ;
		_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
		for (INDEX i = 0 ; i < len ; i++)
			dst[i] = src[len + ~i] ;
	} ,[&] (BOOL &_case_req) {
		_CASE_REQUIRE_ (dst == src) ;
		for (INDEX i = 0 ,ie = len / 2 ; i < ie ; i++) {
			const auto r1x = dst[i] ;
			dst[i] = dst[len + ~i] ;
			dst[len + ~i] = r1x ;
		}
	}) ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMRCOPY_ (ARR<_ARG1> &dst ,const DEF<_ARG1[_VAL1]> &src) {
	_MEMRCOPY_ (dst ,PTRTOARR[src] ,_VAL1) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMRCOPY_ (DEF<_ARG1[_VAL1]> &dst ,const ARR<_ARG1> &src) {
	_MEMRCOPY_ (PTRTOARR[dst] ,src ,_VAL1) ;
}

template <class _ARG1>
inline void _MEMMOVE_ (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (dst1 != NULL) ;
		_DEBUG_ASSERT_ (dst2 != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	if (dst1 == dst2)
		return ;
	_CALL_IF_ ([&] (BOOL &_case_req) {
		_CASE_REQUIRE_ (dst1 < dst2) ;
		for (INDEX i = 0 ; i < len ; i++)
			dst1[i] = std::move (dst2[i]) ;
	} ,[&] (BOOL &_case_req) {
		_CASE_REQUIRE_ (dst1 > dst2) ;
		for (INDEX i = 0 ; i < len ; i++)
			dst1[len + ~i] = std::move (dst2[len + ~i]) ;
	}) ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMMOVE_ (ARR<_ARG1> &dst1 ,DEF<_ARG1[_VAL1]> &dst2) {
	_MEMMOVE_ (dst1 ,PTRTOARR[dst2] ,_VAL1) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMMOVE_ (DEF<_ARG1[_VAL1]> &dst1 ,ARR<_ARG1> &dst2) {
	_MEMMOVE_ (PTRTOARR[dst1] ,dst2 ,_VAL1) ;
}

template <class _ARG1>
inline void _MEMSWAP_ (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (dst1 != NULL) ;
		_DEBUG_ASSERT_ (dst2 != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	if (dst1 == dst2)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst1 - dst2) >= len) ;
	for (INDEX i = 0 ; i < len ; i++)
		_SWAP_ (dst1[i] ,dst2[i]) ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMSWAP_ (ARR<_ARG1> &dst1 ,DEF<_ARG1[_VAL1]> &dst2) {
	_MEMSWAP_ (dst1 ,PTRTOARR[dst2] ,_VAL1) ;
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMSWAP_ (DEF<_ARG1[_VAL1]> &dst1 ,ARR<_ARG1> &dst2) {
	_MEMSWAP_ (PTRTOARR[dst1] ,dst2 ,_VAL1) ;
}

template <class _ARG1>
inline void _MEMFILL_ (ARR<_ARG1> &dst ,LENGTH len ,const _ARG1 &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	for (FOR_ONCE_DO_WHILE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (dst != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
	}
	for (INDEX i = 0 ; i < len ; i++)
		dst[i] = val ;
#pragma GCC diagnostic pop
}

template <class _ARG1 ,LENGTH _VAL1>
inline void _MEMFILL_ (DEF<_ARG1[_VAL1]> &dst ,const _ARG1 &val) {
	_MEMFILL_ (PTRTOARR[dst] ,_VAL1 ,val) ;
}
} ;

class Interface {
public:
	inline Interface () = default ;
	inline virtual ~Interface () noexcept = default ;
	inline Interface (const Interface &) = delete ;
	inline Interface &operator= (const Interface &) = delete ;
	inline Interface (Interface &&) = delete ;
	inline Interface &operator= (Interface &&) = delete ;
} ;

template <class _ARG1>
inline FLAG _TYPEID_ (const ARGV<_ARG1> &) noexcept {
	_STATIC_ASSERT_ (std::is_same<_ARG1 ,REMOVE_CVR_TYPE<_ARG1>>::value) ;
	//@warn: RTTI might be different across DLL
	class Storage final :private Interface {} ret ;
	return std::move (_CAST_<FLAG> (ret)) ;
}

template <class UNIT>
class Wrapped {
protected:
	UNIT mSelf ;

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

template <class REAL>
class Plain final {
private:
	_STATIC_ASSERT_ (stl::is_str_xyz<REAL>::value) ;
	class Detail ;
	PTR<const REAL> mPlain ;
	LENGTH mSize ;

public:
	inline Plain () = delete ;

	template <LENGTH _VAL1>
	inline constexpr implicit Plain (const DEF<REAL[_VAL1]> &that) :mPlain (&that[0]) ,mSize (_VAL1 - 1) {}

	template <LENGTH _VAL1>
	inline constexpr implicit Plain (DEF<REAL[_VAL1]> &) = delete ;

	template <class _ARG1 ,class = ENABLE_TYPE<!stl::is_full_array_of<REAL ,_ARG1>::value>>
	inline explicit Plain (const _ARG1 &that) noexcept :Plain (_CAST_<REAL[_COUNTOF_ (_ARG1)]> (that)) {}

	template <class _ARG1>
	inline explicit Plain (_ARG1 &) = delete ;

	template <class _ARG1 ,class... _ARGS>
	inline explicit Plain (const ARGV<_ARG1> & ,const _ARGS &...args) noexcept :Plain (Detail::cache_string (_NULL_<ARGV<_ARG1>> () ,args...)) {}

	inline constexpr LENGTH size () const {
		return mSize ;
	}

	inline constexpr const ARR<REAL> &to () const {
		_STATIC_WARNING_ ("mark") ;
		return PTRTOARR[mPlain] ;
	}

	inline constexpr implicit operator const ARR<REAL> & () const {
		return to () ;
	}

private:
	class Detail :private Wrapped<void> {
	public:
		template <class SIZE>
		class PlainString {
		private:
			friend Plain ;
			DEF<REAL[SIZE::value]> mString ;

		public:
			inline PlainString () = delete ;

			template <class... _ARGS>
			inline explicit PlainString (const _ARGS &...args) noexcept {
				template_write (mString ,_NULL_<ARGV<ARGC<0>>> () ,args...) ;
			}
		} ;

		inline static constexpr LENGTH constexpr_cache_string_size (const ARGVS<> &) {
			return 1 ;
		}

		template <class _ARG1 ,class... _ARGS>
		inline static constexpr LENGTH constexpr_cache_string_size (const ARGVS<_ARG1 ,_ARGS...> &) {
			return _COUNTOF_ (_ARG1) - 1 + constexpr_cache_string_size (_NULL_<ARGVS<_ARGS...>> ()) ;
		}

		template <class... _ARGS>
		using PLAIN_STRING_SIZE = ARGC<constexpr_cache_string_size (_NULL_<ARGVS<_ARGS...>> ())> ;

		template <class _ARG1 ,class... _ARGS>
		inline static const DEF<REAL[PLAIN_STRING_SIZE<_ARGS...>::value]> &cache_string (const ARGV<_ARG1> & ,const _ARGS &...args) noexcept {
			const auto r1x = PlainString<PLAIN_STRING_SIZE<_ARGS...>> (args...) ;
			auto &r1 = _CACHE_ ([r1x] () noexcept {
				return _COPY_ (r1x) ;
			}) ;
			return r1.mString ;
		}

		template <class _ARG1 ,class _ARG2>
		inline static void template_write (_ARG1 &array ,const ARGV<_ARG2> &) noexcept {
			_STATIC_ASSERT_ (stl::is_full_array_of<REAL ,_ARG1>::value) ;
			_STATIC_ASSERT_ (LENGTH (_ARG2::value) == _COUNTOF_ (_ARG1) - 1) ;
			array[_ARG2::value] = 0 ;
		}

		template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
		inline static void template_write (_ARG1 &array ,const ARGV<_ARG2> & ,const _ARG3 &arg1 ,const _ARGS &...args) noexcept {
			_STATIC_ASSERT_ (stl::is_full_array_of<REAL ,_ARG1>::value) ;
			_STATIC_ASSERT_ (BOOL (LENGTH (_ARG2::value) >= 0 && LENGTH (_ARG2::value) < _COUNTOF_ (_ARG1))) ;
			_STATIC_ASSERT_ (BOOL (stl::is_full_array_of<STRX ,_ARG3>::value || stl::is_full_array_of<STRA ,_ARG3>::value || stl::is_full_array_of<STRW ,_ARG3>::value)) ;
			for (INDEX i = 0 ; i < _COUNTOF_ (_ARG3) - 1 ; i++)
				array[i + _ARG2::value] = raw_to_plain_str (arg1[i]) ;
			template_write (array ,_NULL_<ARGV<ARGC<_ARG2::value + _COUNTOF_ (_ARG3) - 1>>> () ,args...) ;
		}

		template <class _ARG1>
		inline static REAL raw_to_plain_str (const _ARG1 &arg1) noexcept {
			if (arg1 >= _ARG1 (32) && arg1 <= _ARG1 (126))
				return REAL (arg1) ;
			if (BOOL (arg1 == _ARG1 ('\t') || arg1 == _ARG1 ('\v')))
				return REAL (arg1) ;
			if (BOOL (arg1 == _ARG1 ('\r') || arg1 == _ARG1 ('\n') || arg1 == _ARG1 ('\f')))
				return REAL (arg1) ;
			return REAL ('?') ;
		}
	} ;
} ;

class Exception final {
private:
	const ARR<STR> &mWhat ;

public:
	inline Exception () = delete ;

	inline explicit Exception (const Plain<STR> &_what) noexcept :mWhat (_what.self) {}

	inline const ARR<STR> &what () const noexcept {
		return mWhat ;
	}

	inline void rethrow () const {
		throw *this ;
	}
} ;

//@warn: assure ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (_ARG1 &&arg1 ,_ARGS &&...args) {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	try {
		arg1 () ;
		return ;
	} catch (const Exception &) {
		_STATIC_WARNING_ ("noop") ;
	}
	_CALL_TRY_ (std::forward<_ARGS> (args)...) ;
}

class GlobalHeap final :private Wrapped<void> {
private:
	template <class UNIT>
	class OwnerProxy {
	private:
		friend GlobalHeap ;
		PTR<UNIT> mPointer ;

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

		inline implicit operator const PTR<UNIT> & () const & noexcept {
			_DEBUG_ASSERT_ (mPointer != NULL) ;
			return mPointer ;
		}

		inline implicit operator const PTR<UNIT> & () && = delete ;

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<const PTR<UNIT> & ,_RET>::value>>
		inline implicit operator _RET () const & {
			_DEBUG_ASSERT_ (mPointer != NULL) ;
			return _RET (_XVALUE_<PTR<UNIT>> (mPointer)) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

		inline void operator= (DEF<decltype (NULL)>) & noexcept {
			mPointer = NULL ;
		}

		inline void operator= (DEF<decltype (NULL)>) && = delete ;

	private:
		inline explicit OwnerProxy (PTR<UNIT> pointer) noexcept :mPointer (pointer) {}
	} ;

public:
	template <class _RET>
	inline static OwnerProxy<_RET> alloc () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_RET) <= _ALIGNOF_ (stl::max_align_t)) ;
		const auto r1x = operator new (_SIZEOF_ (_RET) ,std::nothrow) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		const auto r2x = &_LOAD_<_RET> (r1x) ;
		return OwnerProxy<_RET> (r2x) ;
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
		const auto r2x = &_LOAD_<ARR<_RET>> (r1x) ;
		return OwnerProxy<ARR<_RET>> (r2x) ;
	}

	template <class _ARG1>
	inline static void free (const PTR<_ARG1> &address) noexcept {
		_DEBUG_ASSERT_ (address != NULL) ;
		const auto r1x = &_LOAD_<ARR<BYTE>> (NULL ,_ADDRESS_ (address)) ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

template <class UNIT>
class ScopedGuard final {
private:
	UNIT &mLock ;

public:
	inline ScopedGuard () = delete ;

	inline explicit ScopedGuard (UNIT &lock) popping :mLock (lock) {
		mLock.lock () ;
	}

	inline ~ScopedGuard () noexcept {
		_CALL_EH_ ([&] () {
			mLock.unlock () ;
		}) ;
	}

	inline ScopedGuard (const ScopedGuard &) = delete ;
	inline ScopedGuard &operator= (const ScopedGuard &) = delete ;
	inline ScopedGuard (ScopedGuard &&) = delete ;
	inline ScopedGuard &operator= (ScopedGuard &&) = delete ;
} ;

template <class UNIT>
class ScopedHolder final {
private:
	const volatile PTR<TEMP<UNIT>> &mAddress ;

public:
	inline ScopedHolder () = delete ;

	template <class... _ARGS>
	inline explicit ScopedHolder (const volatile PTR<TEMP<UNIT>> &address ,_ARGS &&...args) popping :mAddress (address) {
		const auto r1x = _COPY_ (mAddress) ;
		_CREATE_ (r1x ,std::forward<_ARGS> (args)...) ;
	}

	inline ~ScopedHolder () noexcept {
		const auto r1x = _COPY_ (mAddress) ;
		if (r1x == NULL)
			return ;
		_DESTROY_ (r1x) ;
	}

	inline ScopedHolder (const ScopedHolder &) = delete ;
	inline ScopedHolder &operator= (const ScopedHolder &) = delete ;
	inline ScopedHolder (ScopedHolder &&) = delete ;
	inline ScopedHolder &operator= (ScopedHolder &&) = delete ;
} ;

template <class UNIT>
class ScopedHolder<ARR<UNIT>> final {
private:
	const volatile PTR<ARR<TEMP<UNIT>>> &mAddress ;
	INDEX mWrite ;

public:
	inline ScopedHolder () = delete ;

	inline explicit ScopedHolder (const volatile PTR<ARR<TEMP<UNIT>>> &address ,LENGTH len) popping :mAddress (address) ,mWrite (0) {
		const auto r1x = _COPY_ (mAddress) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mWrite >= len)
				break ;
			_CREATE_ (&(*r1x)[mWrite]) ;
			mWrite++ ;
		}
	}

	inline explicit ScopedHolder (const volatile PTR<ARR<TEMP<UNIT>>> &address ,const ARR<UNIT> &src ,LENGTH len) popping :mAddress (address) ,mWrite (0) {
		_DEBUG_ASSERT_ (src != NULL) ;
		const auto r1x = _COPY_ (mAddress) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mWrite >= len)
				break ;
			_CREATE_ (&(*r1x)[mWrite] ,src[mWrite]) ;
			mWrite++ ;
		}
	}

	inline ~ScopedHolder () noexcept {
		const auto r1x = _COPY_ (mAddress) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mWrite <= 0)
				break ;
			_DESTROY_ (&(*r1x)[mWrite]) ;
			mWrite-- ;
		}
	}

	inline ScopedHolder (const ScopedHolder &) = delete ;
	inline ScopedHolder &operator= (const ScopedHolder &) = delete ;
	inline ScopedHolder (ScopedHolder &&) = delete ;
	inline ScopedHolder &operator= (ScopedHolder &&) = delete ;
} ;

template <class UNIT>
class Singleton final {
private:
	class Holder {
	private:
		friend Singleton ;
		UNIT mData ;
	} ;

private:
	_STATIC_ASSERT_ (std::is_class<UNIT>::value) ;
	_STATIC_ASSERT_ (!std::is_default_constructible<UNIT>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_destructible<UNIT>::value) ;
	friend UNIT ;
	template <class>
	friend class GlobalStatic ;
	PTR<Holder> mPointer ;

private:
	inline Singleton () {
		auto sgd = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedHolder<Holder> ANONYMOUS (sgd) ;
		mPointer = &_LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (sgd)) ;
		sgd = NULL ;
	}

	inline ~Singleton () noexcept {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline Singleton (const Singleton &) = delete ;
	inline Singleton &operator= (const Singleton &) = delete ;
	inline Singleton (Singleton &&) = delete ;
	inline Singleton &operator= (Singleton &&) = delete ;

	inline UNIT &to () const {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		return mPointer->mData ;
	}

	inline implicit operator UNIT & () const {
		return to () ;
	}

public:
	//@warn: static instance across DLL ruins Singleton
	inline static DEF<UNIT & ()> instance ;
} ;

template <class>
class AutoRef ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,FALSE>> {
private:
	using SPECIALIZATION_TYPE = AutoRef<UNIT> ;

	class Holder {
	private:
		friend AutoRef ;
		friend SPECIALIZATION_TYPE ;
		UNIT mData ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
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

	inline AutoRef (AutoRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~AutoRef () ;
			new (this) AutoRef (std::move (that)) ;
		}
		return *this ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :mPointer (pointer) {}
} ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,TRUE>> {
private:
	using SPECIALIZATION_TYPE = AutoRef<UNIT> ;

	class Holder {
	private:
		friend AutoRef ;
		friend SPECIALIZATION_TYPE ;
		UNIT mData ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
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

	inline AutoRef (const AutoRef &that) {
		auto sgd = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedHolder<Holder> ANONYMOUS (sgd ,_XVALUE_<const UNIT> (that.mPointer->mData)) ;
		mPointer = &_LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (sgd)) ;
		sgd = NULL ;
	}

	inline AutoRef &operator= (const AutoRef &that) {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~AutoRef () ;
			new (this) AutoRef (std::move (that)) ;
		}
		return *this ;
	}

	inline AutoRef (AutoRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~AutoRef () ;
			new (this) AutoRef (std::move (that)) ;
		}
		return *this ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :mPointer (pointer) {}
} ;

template <class UNIT>
class AutoRef :private AutoRef<SPECIALIZATION<UNIT ,(std::is_copy_constructible<UNIT>::value && std::is_nothrow_move_constructible<UNIT>::value)>> {
private:
	using SPECIALIZATION_BASE = AutoRef<SPECIALIZATION<UNIT ,(std::is_copy_constructible<UNIT>::value && std::is_nothrow_move_constructible<UNIT>::value)>> ;
	using Holder = typename SPECIALIZATION_BASE::Holder ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mPointer ;

public:
	inline AutoRef () = default ;

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mData ;
	}

	inline implicit operator UNIT & () {
		return to () ;
	}

	inline PTR<UNIT> operator-> () {
		return &to () ;
	}

	inline const UNIT &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mData ;
	}

	inline implicit operator const UNIT & () const {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const {
		return &to () ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) :SPECIALIZATION_BASE (pointer) {}

public:
	template <class... _ARGS>
	inline static AutoRef make (_ARGS &&...args) {
		auto sgd = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedHolder<Holder> ANONYMOUS (sgd ,std::forward<_ARGS> (args)...) ;
		AutoRef ret = AutoRef (&_LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (sgd))) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <class UNIT>
class SharedRef {
private:
	class Holder {
	private:
		friend SharedRef ;
		UNIT mData ;
		LENGTH mCounter ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) ,mCounter (0) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline SharedRef () noexcept {
		mPointer = NULL ;
	}

	inline ~SharedRef () noexcept {
		if (mPointer == NULL)
			return ;
		for (FOR_ONCE_DO_WHILE) {
			const auto r1x = --mPointer->mCounter ;
			if (r1x != 0)
				discard ;
			mPointer->~Holder () ;
			GlobalHeap::free (mPointer) ;
		}
		mPointer = NULL ;
	}

	inline SharedRef (const SharedRef &that) :SharedRef (that.mPointer) {}

	inline SharedRef &operator= (const SharedRef &that) {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~SharedRef () ;
			new (this) SharedRef (std::move (that)) ;
		}
		return *this ;
	}

	inline SharedRef (SharedRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline SharedRef &operator= (SharedRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~SharedRef () ;
			new (this) SharedRef (std::move (that)) ;
		}
		return *this ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mData ;
	}

	inline implicit operator UNIT & () const {
		return to () ;
	}

	inline PTR<UNIT> operator-> () const {
		return &to () ;
	}

private:
	inline explicit SharedRef (PTR<Holder> pointer) {
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
		SharedRef ret = SharedRef (&_LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (sgd))) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <class UNIT>
class AnyRef {
private:
	struct Holder :public Interface {
		virtual FLAG type () const = 0 ;
	} ;

	template <class _UNIT>
	class ImplHolder :public Holder {
	private:
		friend AnyRef ;
		_UNIT mData ;

	public:
		template <class... _ARGS>
		inline explicit ImplHolder (_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) {}

		inline FLAG type () const override {
			return _TYPEID_<_UNIT> () ;
		}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline AnyRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG1>
	inline implicit AnyRef (AnyRef<_ARG1> &&that) : AnyRef (std::move (that.template rebind<UNIT> ())) {}

	inline ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~AnyRef () ;
			new (this) AnyRef (std::move (that)) ;
		}
		return *this ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () && = delete ;

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG type () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->type () ;
	}

	inline UNIT &to () {
		_DEBUG_ASSERT_ (type () == _TYPEID_<UNIT> ()) ;
		const auto r1x = static_cast<PTR<ImplHolder<UNIT>>> (mPointer) ;
		return r1x->mData ;
	}

	inline implicit operator UNIT & () {
		return to () ;
	}

	inline PTR<UNIT> operator-> () {
		return &to () ;
	}

	inline const UNIT &to () const {
		_DEBUG_ASSERT_ (type () == _TYPEID_<UNIT> ()) ;
		const auto r1x = static_cast<PTR<ImplHolder<UNIT>>> (mPointer) ;
		return r1x->mData ;
	}

	inline implicit operator const UNIT & () const {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const {
		return &to () ;
	}

private:
	inline explicit AnyRef (PTR<Holder> pointer) :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static AnyRef make (_ARGS &&...args) {
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<UNIT>>> () ;
		ScopedHolder<ImplHolder<UNIT>> ANONYMOUS (sgd ,std::forward<_ARGS> (args)...) ;
		AnyRef ret = AnyRef (&_LOAD_<ImplHolder<UNIT>> (_XVALUE_<PTR<TEMP<ImplHolder<UNIT>>>> (sgd))) ;
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

	template <class _ARG1>
	inline implicit AnyRef (AnyRef<_ARG1> &&that) :AnyRef (std::move (that.template rebind<void> ())) {}

	inline ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~AnyRef () ;
			new (this) AnyRef (std::move (that)) ;
		}
		return *this ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (*this) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () && = delete ;

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG type () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->type () ;
	}
} ;

template <class UNIT>
class UniqueRef {
private:
	struct Holder :public Interface {
		virtual void release () = 0 ;
	} ;

	template <class _UNIT>
	class ImplHolder :public Holder {
	private:
		friend UniqueRef ;
		REMOVE_CVR_TYPE<UNIT> mData ;
		_UNIT mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (_UNIT &&functor) :mFunctor (std::move (functor)) {}

		inline void release () override {
			mFunctor (mData) ;
		}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	PTR<Holder> mPointer ;

public:
	inline UniqueRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (_ARG1 &&constructor ,_ARG2 &&destructor) popping {
		_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNIT &>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<UNIT &>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<REMOVE_REFERENCE_TYPE<_ARG2> ,PTR<void (UNIT &)>>::value) ;
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>> () ;
		ScopedHolder<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> ANONYMOUS (sgd ,std::forward<_ARG2> (destructor)) ;
		const auto r1x = &_LOAD_<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> (_XVALUE_<PTR<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>>> (sgd)) ;
		constructor (r1x->mData) ;
		mPointer = r1x ;
		sgd = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_EH_ ([&] () {
			mPointer->release () ;
		}) ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~UniqueRef () ;
			new (this) UniqueRef (std::move (that)) ;
		}
		return *this ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline const UNIT &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = static_cast<PTR<ImplHolder<PTR<void (UNIT &)>>>> (mPointer) ;
		return r1x->mData ;
	}

	inline implicit operator const UNIT & () const {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const {
		return &to () ;
	}

private:
	inline explicit UniqueRef (PTR<Holder> pointer) :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static UniqueRef make (_ARGS &&...args) {
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<PTR<void (UNIT &)>>>> () ;
		const auto r1x = _XVALUE_<PTR<void (UNIT &)>> ([] (UNIT &) {}) ;
		ScopedHolder<ImplHolder<PTR<void (UNIT &)>>> ANONYMOUS (sgd ,r1x) ;
		const auto r2x = &_LOAD_<ImplHolder<PTR<void (UNIT &)>>> (_XVALUE_<PTR<TEMP<ImplHolder<PTR<void (UNIT &)>>>>> (sgd)) ;
		r2x->mData = UNIT (std::forward<_ARGS> (args)...) ;
		UniqueRef ret = UniqueRef (_XVALUE_<PTR<Holder>> (r2x)) ;
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

	template <class _UNIT>
	class ImplHolder :public Holder {
	private:
		_UNIT mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (_UNIT &&functor) :mFunctor (std::move (functor)) {}

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
	inline explicit UniqueRef (_ARG1 &&constructor ,_ARG2 &&destructor) popping {
		_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<REMOVE_REFERENCE_TYPE<_ARG2> ,PTR<void ()>>::value) ;
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>> () ;
		ScopedHolder<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> ANONYMOUS (sgd ,std::forward<_ARG2> (destructor)) ;
		const auto r1x = &_LOAD_<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> (_XVALUE_<PTR<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>>> (sgd)) ;
		constructor () ;
		mPointer = r1x ;
		sgd = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_EH_ ([&] () {
			mPointer->release () ;
		}) ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~UniqueRef () ;
			new (this) UniqueRef (std::move (that)) ;
		}
		return *this ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}
} ;

template <class UNIT>
class PhanRef {
private:
	PTR<UNIT> mPointer ;

public:
	inline PhanRef () noexcept {
		mPointer = NULL ;
	}

	inline ~PhanRef () noexcept {
		mPointer = NULL ;
	}

	inline PhanRef (const PhanRef &) = delete ;
	inline PhanRef &operator= (const PhanRef &) = delete ;

	inline PhanRef (PhanRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline PhanRef &operator= (PhanRef &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~PhanRef () ;
			new (this) PhanRef (std::move (that)) ;
		}
		return *this ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return *mPointer ;
	}

	inline implicit operator UNIT & () const {
		return to () ;
	}

	inline PTR<UNIT> operator-> () const {
		return &to () ;
	}

private:
	inline explicit PhanRef (PTR<UNIT> pointer) :mPointer (pointer) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static PhanRef make (UNIT &val) {
		return PhanRef (&val) ;
	}

	inline static PhanRef make (const PhanRef<UNIT> &val) {
		if (!val.exist ())
			return PhanRef () ;
		return make (val.self) ;
	}
} ;

template <class>
class Function ;

template <class UNIT1 ,class... UNITS>
class Function<UNIT1 (UNITS...)> {
private:
	struct Holder :public Interface {
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping = 0 ;
	} ;

	template <class _UNIT>
	class ImplHolder :public Function<UNIT1 (UNITS...)>::Holder {
	private:
		_UNIT mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (const _UNIT &functor) :mFunctor (std::move (functor)) {}

		inline explicit ImplHolder (_UNIT &&functor) :mFunctor (std::move (functor)) {}

		inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping override {
			return mFunctor (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
		}
	} ;

private:
	PTR<Holder> mFunction_a ;
	PTR<UNIT1 (UNITS...)> mFunction_b ;

public:
	inline Function () noexcept {
		mFunction_a = NULL ;
		mFunction_b = NULL ;
	}

	inline implicit Function (const PTR<UNIT1 (UNITS...)> &that) {
		mFunction_a = NULL ;
		mFunction_b = that ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Function>::value && std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNITS...>> ,UNIT1>::value>>
	inline implicit Function (_ARG1 &&that) {
		auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>>> () ;
		ScopedHolder<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>> ANONYMOUS (sgd ,std::forward<_ARG1> (that)) ;
		mFunction_a = &_LOAD_<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>> (_XVALUE_<PTR<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>>>> (sgd)) ;
		mFunction_b = NULL ;
		sgd = NULL ;
	}

	inline ~Function () noexcept {
		if (mFunction_a == NULL && mFunction_b == NULL)
			return ;
		for (FOR_ONCE_DO_WHILE) {
			if (mFunction_a == NULL)
				discard ;
			mFunction_a->~Holder () ;
			GlobalHeap::free (mFunction_a) ;
		}
		mFunction_a = NULL ;
		mFunction_b = NULL ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&that) noexcept {
		mFunction_a = _EXCHANGE_ (that.mFunction_a) ;
		mFunction_b = _EXCHANGE_ (that.mFunction_b) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Function () ;
			new (this) Function (std::move (that)) ;
		}
		return *this ;
	}

	inline BOOL exist () const {
		if (mFunction_a != NULL)
			return TRUE ;
		if (mFunction_b != NULL)
			return TRUE ;
		return FALSE ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mFunction_b != NULL)
			return mFunction_b (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
		return mFunction_a->invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}

private:
	inline explicit Function (const DEF<decltype (ARGVP0)> & ,PTR<Holder> pointer) :mFunction_a (pointer) ,mFunction_b (NULL) {}

public:
	//@info: the function is incompleted without 'csc_ext.hpp'
	template <class... _ARGS>
	inline static Function make (const PTR<UNIT1 (UNITS... ,_ARGS...)> &function ,const REMOVE_CVR_TYPE<_ARGS> &...args) ;
} ;

//@error: vs2017 is too useless to compile without hint
template <class UNIT1 ,class... UNITS>
using FIX_MSVC_DEDUCTION_2 = DEF<DEF<UNIT1 (UNITS...)> NONE::*> ;

template <class UNIT1 ,class... UNITS>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>> {
private:
	class FakeHolder ;

	struct Holder :public Interface {
		virtual void compute_copy (PTR<TEMP<FakeHolder>> address) const noexcept = 0 ;
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping = 0 ;
	} ;

	class FakeHolder :public Holder {
	private:
		PTR<NONE> mContext ;
		DEF<DEF<UNIT1 (UNITS...)> NONE::*> mFunction ;

	public:
		inline FakeHolder () = delete ;

		inline void compute_copy (PTR<TEMP<FakeHolder>> address) const noexcept override ;
		inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping override ;
	} ;

	template <class>
	class ImplHolder ;

private:
	class Detail ;
	TEMP<FakeHolder> mVariant ;

public:
	inline Function () noexcept {
		_ZERO_ (mVariant) ;
	}

	inline implicit Function (const PTR<UNIT1 (UNITS...)> &that) noexcept {
		_DEBUG_ASSERT_ (that != NULL) ;
		Detail::template static_create<typename Detail::PureHolder> (&mVariant ,that) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context ,const DEF<DEF<UNIT1 (UNITS...)> _ARG1::*> &function) noexcept {
		_DEBUG_ASSERT_ (function != NULL) ;
		Detail::template static_create<ImplHolder<_ARG1>> (&mVariant ,&context.self ,function) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<const _ARG1> &context ,const DEF<DEF<UNIT1 (UNITS...) const> _ARG1::*> &function) noexcept {
		_DEBUG_ASSERT_ (function != NULL) ;
		Detail::template static_create<ImplHolder<const _ARG1>> (&mVariant ,&context.self ,function) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context ,const PTR<UNIT1 (PTR<_ARG1> ,UNITS...)> &function) noexcept {
		_DEBUG_ASSERT_ (function != NULL) ;
		Detail::template static_create<ImplHolder<PTR<_ARG1>>> (&mVariant ,&context.self ,function) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context ,const PTR<UNIT1 (PTR<const _ARG1> ,UNITS...)> &function) noexcept {
		_DEBUG_ASSERT_ (function != NULL) ;
		Detail::template static_create<ImplHolder<PTR<const _ARG1>>> (&mVariant ,&context.self ,function) ;
	}

	inline ~Function () noexcept {
		if (!exist ())
			return ;
		_XVALUE_<Holder> (_CAST_<FakeHolder> (mVariant)).~Holder () ;
		_ZERO_ (mVariant) ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&that) noexcept {
		_ZERO_ (mVariant) ;
		if (!that.exist ())
			return ;
		_XVALUE_<Holder> (_CAST_<FakeHolder> (that.mVariant)).compute_copy (&mVariant) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Function () ;
			new (this) Function (std::move (that)) ;
		}
		return *this ;
	}

	inline BOOL exist () const {
		auto &r1 = _CAST_<BYTE[_SIZEOF_ (TEMP<FakeHolder>)]> (mVariant) ;
		for (INDEX i = 0 ; i < _COUNTOF_ (decltype (r1)) ; i++)
			if (r1[i] != 0)
				return TRUE ;
		return FALSE ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		return _XVALUE_<Holder> (_CAST_<FakeHolder> (mVariant)).invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}

private:
	class Detail :private Wrapped<void> {
	public:
		class PureHolder :public Holder {
		private:
			PTR<UNIT1 (UNITS...)> mFunction ;

		public:
			inline PureHolder () = delete ;

			inline explicit PureHolder (const PTR<UNIT1 (UNITS...)> &function) noexcept :mFunction (function) {}

			inline void compute_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
				static_create<PureHolder> (address ,mFunction) ;
			}

			inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping override {
				return mFunction (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
			}
		} ;

		template <class _RET ,class... _ARGS>
		inline static void static_create (PTR<TEMP<FakeHolder>> address ,_ARGS &&...args) noexcept {
			_STATIC_ASSERT_ (_ALIGNOF_ (TEMP<FakeHolder>) >= _ALIGNOF_ (TEMP<_RET>)) ;
			_STATIC_ASSERT_ (_SIZEOF_ (TEMP<FakeHolder>) >= _SIZEOF_ (TEMP<_RET>)) ;
			_DEBUG_ASSERT_ (address != NULL) ;
			const auto r1x = &_LOAD_<TEMP<_RET>> (NULL ,_ADDRESS_ (address)) ;
			const auto r2x = &_XVALUE_<Holder> (_CAST_<_RET> (*r1x)) ;
			_DEBUG_ASSERT_ (_ADDRESS_ (r2x) == _ADDRESS_ (static_cast<PTR<FakeHolder>> (r2x))) ;
			_DEBUG_ASSERT_ (_ADDRESS_ (r2x) == _ADDRESS_ (static_cast<PTR<_RET>> (r2x))) ;
			(void) r2x ;
			_CREATE_ (r1x ,std::forward<_ARGS> (args)...) ;
		}
	} ;
} ;

template <class UNIT1 ,class... UNITS>
template <class _UNIT>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>>::ImplHolder :public Holder {
private:
	PTR<_UNIT> mContext ;
	DEF<DEF<UNIT1 (UNITS...)> _UNIT::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<_UNIT> context ,const DEF<DEF<UNIT1 (UNITS...)> _UNIT::*> &function) noexcept :mContext (context) ,mFunction (function) {}

	inline void compute_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
		Detail::template static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping override {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class _UNIT>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>>::ImplHolder<const _UNIT> :public Holder {
private:
	PTR<const _UNIT> mContext ;
	DEF<DEF<UNIT1 (UNITS...) const> _UNIT::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<const _UNIT> context ,const DEF<DEF<UNIT1 (UNITS...) const> _UNIT::*> &function) noexcept :mContext (context) ,mFunction (function) {}

	inline void compute_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
		Detail::template static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping override {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class _UNIT>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>>::ImplHolder<PTR<_UNIT>> :public Holder {
private:
	PTR<_UNIT> mContext ;
	PTR<UNIT1 (PTR<_UNIT> ,UNITS...)> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<_UNIT> context ,const PTR<UNIT1 (PTR<_UNIT> ,UNITS...)> &function) noexcept :mContext (context) ,mFunction (function) {}

	inline void compute_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
		Detail::template static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...args) const popping override {
		return mFunction (mContext ,std::forward<FORWARD_TRAITS_TYPE<UNITS>> (args)...) ;
	}
} ;

template <class ,class>
class Buffer ;

using SFLEX = ARGC<0> ;
using SFIXED = ARGC<-1> ;
using SAUTO = ARGC<-2> ;
using SCPHAN = ARGC<-4> ;
using SMPHAN = ARGC<-5> ;

template <class UNIT ,LENGTH SIZE>
class Buffer<UNIT ,ARGC<SIZE>> {
private:
	_STATIC_ASSERT_ (SIZE > 0) ;
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	DEF<UNIT[SIZE]> mBuffer ;

public:
	inline Buffer () = default ;

	inline explicit Buffer (LENGTH len) {
		_DEBUG_ASSERT_ (BOOL (len >= 0 && len <= SIZE)) ;
	}

	inline implicit Buffer (const DEF<UNIT[SIZE]> &that) :Buffer (std::move (Buffer::from (that))) {}

	inline implicit Buffer (DEF<UNIT[SIZE]> &&that) : Buffer (std::move (Buffer::from (that))) {}

	inline ARR<UNIT> &to () {
		return PTRTOARR[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const {
		return PTRTOARR[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return SIZE ;
	}

	inline UNIT &get (INDEX index) & {
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return mBuffer[index] ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return mBuffer[index] ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - mBuffer ;
		if (!BOOL (ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		return _MEMEQUAL_ (mBuffer ,PTRTOARR[that.mBuffer]) ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		return _MEMCOMPR_ (mBuffer ,PTRTOARR[that.mBuffer]) ;
	}

	inline BOOL operator< (const Buffer &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Buffer &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Buffer &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Buffer &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	inline Buffer expand () const {
		return expand (0) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) popping {
		_SWAP_ (mBuffer ,that.mBuffer) ;
	}

public:
	inline static Buffer &from (DEF<UNIT[SIZE]> &_self) {
		return _CAST_<Buffer> (_self) ;
	}

	inline static const Buffer &from (const DEF<UNIT[SIZE]> &_self) {
		return _CAST_<Buffer> (_self) ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SFIXED> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	PTR<ARR<UNIT>> mBuffer ;
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
		auto sgd = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedHolder<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd) ,len) ;
		mBuffer = &_LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd)) ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (INDEX i = 0 ; i < mSize ; i++)
			(*mBuffer)[i].~UNIT () ;
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;
	inline Buffer (Buffer &&) = delete ;
	inline Buffer &operator= (Buffer &&) = delete ;

	inline ARR<UNIT> &to () {
		return *mBuffer ;
	}

	inline implicit operator ARR<UNIT> & () {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (INDEX index) & {
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return (*mBuffer)[index] ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return (*mBuffer)[index] ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - *mBuffer ;
		if (!BOOL (ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ (*mBuffer ,*that.mBuffer ,that.mSize))
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = _MEMCOMPR_ (*mBuffer ,*that.mBuffer ,r1x) ;
		if (r2x != 0)
			return _COPY_ (r2x) ;
		return _MEMCOMPR_ (PTRTOARR[&mSize] ,PTRTOARR[&that.mSize] ,1) ;
	}

	inline BOOL operator< (const Buffer &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Buffer &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Buffer &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Buffer &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	inline Buffer<UNIT ,SAUTO> expand () const {
		return expand (0) ;
	}

	inline Buffer<UNIT ,SAUTO> expand (LENGTH len) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Buffer<UNIT ,SAUTO> () ;
	}

	inline void swap (Buffer<UNIT ,SAUTO> &that) popping {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;

template <class UNIT>
using FixedBuffer = Buffer<UNIT ,SFIXED> ;

template <class UNIT>
class Buffer<UNIT ,SAUTO> ;

template <class UNIT>
class Buffer<SPECIALIZATION<UNIT ,FALSE> ,SAUTO> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	friend Buffer<UNIT ,SAUTO> ;
	PTR<ARR<UNIT>> mBuffer ;
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
		auto sgd = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedHolder<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd) ,len) ;
		mBuffer = &_LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd)) ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (INDEX i = 0 ; i < mSize ; i++)
			(*mBuffer)[i].~UNIT () ;
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;

	inline Buffer (Buffer &&that) noexcept {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return *this ;
	}
} ;

template <class UNIT>
class Buffer<SPECIALIZATION<UNIT ,TRUE> ,SAUTO> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	friend Buffer<UNIT ,SAUTO> ;
	PTR<ARR<UNIT>> mBuffer ;
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
		auto sgd = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedHolder<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd) ,len) ;
		mBuffer = &_LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd)) ;
		mSize = len ;
		sgd = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (INDEX i = 0 ; i < mSize ; i++)
			(*mBuffer)[i].~UNIT () ;
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &that) {
		mBuffer = NULL ;
		mSize = 0 ;
		if (that.mSize == 0)
			return ;
		auto sgd = GlobalHeap::alloc<TEMP<UNIT>> (that.mSize) ;
		ScopedHolder<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd) ,*that.mBuffer ,that.mSize) ;
		mBuffer = &_LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (sgd)) ;
		mSize = that.mSize ;
		sgd = NULL ;
	}

	inline Buffer &operator= (const Buffer &that) {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return *this ;
	}

	inline Buffer (Buffer &&that) noexcept {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return *this ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SAUTO> :private Buffer<SPECIALIZATION<UNIT ,(std::is_copy_constructible<UNIT>::value && std::is_nothrow_move_constructible<UNIT>::value)> ,SAUTO> {
private:
	using SPECIALIZATION_BASE = Buffer<SPECIALIZATION<UNIT ,(std::is_copy_constructible<UNIT>::value && std::is_nothrow_move_constructible<UNIT>::value)> ,SAUTO> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mBuffer ;
	using SPECIALIZATION_BASE::mSize ;

public:
	inline Buffer () = default ;

	inline explicit Buffer (LENGTH len) :SPECIALIZATION_BASE (len) {}

	inline ARR<UNIT> &to () {
		return *mBuffer ;
	}

	inline implicit operator ARR<UNIT> & () {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (INDEX index) & {
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return (*mBuffer)[index] ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return (*mBuffer)[index] ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - *mBuffer ;
		if (!BOOL (ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ (*mBuffer ,*that.mBuffer ,that.mSize))
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = _MEMCOMPR_ (*mBuffer ,*that.mBuffer ,r1x) ;
		if (r2x != 0)
			return _COPY_ (r2x) ;
		return _MEMCOMPR_ (PTRTOARR[&mSize] ,PTRTOARR[&that.mSize] ,1) ;
	}

	inline BOOL operator< (const Buffer &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Buffer &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Buffer &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Buffer &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	inline Buffer expand () const {
		const auto r1x = _MAX_ (LENGTH (mSize * MATH_SQRT2) ,(mSize + DEFAULT_EXPANDFIRST_SIZE::value)) ;
		return expand (r1x) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (len >= 0) ;
		return Buffer (len) ;
	}

	inline void swap (Buffer &that) popping {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}
} ;

template <class UNIT>
using AutoBuffer = Buffer<UNIT ,SAUTO> ;

template <class UNIT>
class Buffer<UNIT ,SCPHAN> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	PTR<const ARR<UNIT>> mBuffer ;
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

	inline Buffer (Buffer &&that) noexcept {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return *this ;
	}

	inline const ARR<UNIT> &to () const {
		return *mBuffer ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline const UNIT &get (INDEX index) const & {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return (*mBuffer)[index] ;
#pragma GCC diagnostic pop
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX) && = delete ;

	inline const UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - *mBuffer ;
		if (!BOOL (ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ (*mBuffer ,*that.mBuffer ,that.mSize))
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = _MEMCOMPR_ (*mBuffer ,*that.mBuffer ,r1x) ;
		if (r2x != 0)
			return _COPY_ (r2x) ;
		return _MEMCOMPR_ (PTRTOARR[&mSize] ,PTRTOARR[&that.mSize] ,1) ;
	}

	inline BOOL operator< (const Buffer &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Buffer &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Buffer &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Buffer &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	inline Buffer expand () const {
		return expand (0) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) popping {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

private:
	inline explicit Buffer (PTR<const ARR<UNIT>> src ,LENGTH len) :mBuffer (src) ,mSize (len) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static Buffer make (const ARR<UNIT> &src ,LENGTH len) {
		if (len == 0)
			return Buffer (NULL ,0) ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
		return Buffer (&src ,len) ;
	}

	template <LENGTH _VAL1>
	inline static Buffer make (const DEF<UNIT[_VAL1]> &src) {
		return make (PTRTOARR[src] ,_VAL1) ;
	}

	template <class _ARG1>
	inline static Buffer make (const Buffer<UNIT ,_ARG1> &src) {
		return make (src ,src.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<std::is_same<UNIT ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && LOAD_CHECK_TYPE<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (const Buffer<_ARG1 ,_ARG2> &src) {
		if (src.size () == 0)
			return Buffer () ;
		return make (_LOAD_<ARR<UNIT>> (&src.self) ,(src.size () * _SIZEOF_ (_ARG1))) ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SMPHAN> {
private:
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;
	PTR<ARR<UNIT>> mBuffer ;
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

	inline Buffer (Buffer &&that) noexcept {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return *this ;
	}

	inline ARR<UNIT> &to () const {
		return *mBuffer ;
	}

	inline implicit operator ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (INDEX index) const & {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (BOOL (index >= 0 && index < size ())) ;
		return (*mBuffer)[index] ;
#pragma GCC diagnostic pop
	}

	inline UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - *mBuffer ;
		if (!BOOL (ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ (*mBuffer ,*that.mBuffer ,that.mSize))
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = _MEMCOMPR_ (*mBuffer ,*that.mBuffer ,r1x) ;
		if (r2x != 0)
			return _COPY_ (r2x) ;
		return _MEMCOMPR_ (PTRTOARR[&mSize] ,PTRTOARR[&that.mSize] ,1) ;
	}

	inline BOOL operator< (const Buffer &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Buffer &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Buffer &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Buffer &that) const {
		return BOOL (compr (that) <= 0) ;
	}

	inline Buffer expand () const {
		return expand (0) ;
	}

	inline Buffer expand (LENGTH len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) popping {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

private:
	inline explicit Buffer (PTR<ARR<UNIT>> src ,LENGTH len) :mBuffer (src) ,mSize (len) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static Buffer make (ARR<UNIT> &src ,LENGTH len) {
		if (len == 0)
			return Buffer (NULL ,0) ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len >= 0) ;
		return Buffer (&src ,len) ;
	}

	template <LENGTH _VAL1>
	inline static Buffer make (DEF<UNIT[_VAL1]> &src) {
		return make (PTRTOARR[src] ,_VAL1) ;
	}

	template <class _ARG1>
	inline static Buffer make (Buffer<UNIT ,_ARG1> &src) {
		return make (src.self ,src.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<std::is_same<UNIT ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && LOAD_CHECK_TYPE<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (Buffer<_ARG1 ,_ARG2> &src) {
		if (src.size () == 0)
			return Buffer () ;
		return make (_LOAD_<ARR<UNIT>> (&src.self) ,(src.size () * _SIZEOF_ (_ARG1))) ;
	}

	inline static Buffer make (const Buffer<UNIT ,SMPHAN> &src) {
		return make (src.self ,src.size ()) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<UNIT ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && LOAD_CHECK_TYPE<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (const Buffer<_ARG1 ,SMPHAN> &src) {
		if (src.size () == 0)
			return Buffer () ;
		return make (_LOAD_<ARR<UNIT>> (&src.self) ,(src.size () * _SIZEOF_ (_ARG1))) ;
	}
} ;

template <class UNIT>
using PhanBuffer = Buffer<REMOVE_CONST_TYPE<UNIT> ,CONDITIONAL_TYPE<std::is_const<UNIT>::value ,SCPHAN ,SMPHAN>> ;

template <class ,class>
class Allocator ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,FALSE ,FALSE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Node {
	private:
		friend Allocator ;
		friend SPECIALIZATION_TYPE ;
		TEMP<UNIT> mData ;
		INDEX mNext ;

	public:
		inline Node () = default ;
	} ;

private:
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		spec.update_reset (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		spec.update_reset (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		spec.clear () ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;
	inline Allocator (Allocator &&) = delete ;
	inline Allocator &operator= (Allocator &&) = delete ;

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return *static_cast<PTR<SPECIALIZATION_TYPE>> (this) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return *static_cast<PTR<const SPECIALIZATION_TYPE>> (this) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,FALSE ,TRUE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Node {
	private:
		friend Allocator ;
		friend SPECIALIZATION_TYPE ;
		TEMP<UNIT> mData ;
		INDEX mNext ;

	public:
		inline Node () = default ;
	} ;

private:
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		spec.update_reset (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		spec.update_reset (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		spec.clear () ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;

	inline Allocator (Allocator &&that) noexcept :mAllocator (std::move (that.mAllocator)) {
		const auto r1x = (std::is_pod<UNIT>::value) ? (mAllocator.size ()) : 0 ;
		for (INDEX i = r1x ; i < that.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext != VAR_USED)
				continue ;
			_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<UNIT> (that.mAllocator[i].mData))) ;
		}
		mLength = that.mLength ;
		mFree = that.mFree ;
		that.spec.clear () ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Allocator () ;
			new (this) Allocator (std::move (that)) ;
		}
		return *this ;
	}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return *static_cast<PTR<SPECIALIZATION_TYPE>> (this) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return *static_cast<PTR<const SPECIALIZATION_TYPE>> (this) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,TRUE ,TRUE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_TYPE = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Node {
	private:
		friend Allocator ;
		friend SPECIALIZATION_TYPE ;
		TEMP<UNIT> mData ;
		INDEX mNext ;

	public:
		inline Node () = default ;
	} ;

private:
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () {
		spec.update_reset (0 ,VAR_NONE) ;
	}

	inline explicit Allocator (LENGTH len) :mAllocator (len) {
		spec.update_reset (0 ,VAR_NONE) ;
	}

	inline ~Allocator () noexcept {
		spec.clear () ;
	}

	inline Allocator (const Allocator &that) :mAllocator (std::move (that.mAllocator)) {
		class Finally :private Wrapped<Allocator> {
		public:
			inline void lock () noexcept {
				Finally::mSelf.mLength = 0 ;
				Finally::mSelf.mFree = VAR_NONE ;
			}

			inline void unlock () noexcept {
				if (Finally::mSelf.mLength > 0)
					return ;
				const auto r1x = (std::is_pod<UNIT>::value) ? (Finally::mSelf.mAllocator.size ()) : 0 ;
				for (INDEX i = r1x ; i < Finally::mSelf.mAllocator.size () ; i++) {
					if (Finally::mSelf.mAllocator[i].mNext != VAR_USED)
						continue ;
					_DESTROY_ (&Finally::mSelf.mAllocator[i].mData) ;
				}
				Finally::mSelf.mLength = 0 ;
				Finally::mSelf.mFree = VAR_NONE ;
			}
		} ;
		ScopedGuard<Finally> ANONYMOUS (_CAST_<Finally> (*this)) ;
		const auto r2x = (std::is_pod<UNIT>::value) ? (mAllocator.size ()) : 0 ;
		for (INDEX i = r2x ; i < mAllocator.size () ; i++) {
			if (mAllocator[i].mNext != VAR_USED)
				continue ;
			_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<UNIT> (that.mAllocator[i].mData))) ;
		}
		mLength = that.mLength ;
		mFree = that.mFree ;
	}

	inline Allocator &operator= (const Allocator &that) {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Allocator () ;
			new (this) Allocator (std::move (that)) ;
		}
		return *this ;
	}

	inline Allocator (Allocator &&that) noexcept :mAllocator (std::move (that.mAllocator)) {
		const auto r1x = (std::is_pod<UNIT>::value) ? (mAllocator.size ()) : 0 ;
		for (INDEX i = r1x ; i < that.mAllocator.size () ; i++) {
			if (mAllocator[i].mNext != VAR_USED)
				continue ;
			_CREATE_ (&mAllocator[i].mData ,std::move (_CAST_<UNIT> (that.mAllocator[i].mData))) ;
		}
		mLength = that.mLength ;
		mFree = that.mFree ;
		that.spec.clear () ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		for (FOR_ONCE_DO_WHILE) {
			if (this == &that)
				discard ;
			this->~Allocator () ;
			new (this) Allocator (std::move (that)) ;
		}
		return *this ;
	}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return *static_cast<PTR<SPECIALIZATION_TYPE>> (this) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return *static_cast<PTR<const SPECIALIZATION_TYPE>> (this) ;
	}

	inline SPECIALIZATION_TYPE &m_spec () && = delete ;

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator :private Allocator<SPECIALIZATION<UNIT ,(std::is_copy_constructible<Buffer<UNIT ,SIZE>>::value && std::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value) ,std::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Allocator<SPECIALIZATION<UNIT ,(std::is_copy_constructible<Buffer<UNIT ,SIZE>>::value && std::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value) ,std::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;

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
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		for (INDEX i = 0 ; i < mAllocator.size () ; i++) {
			iy = ix ;
			ix = mAllocator.size () + ~i ;
			if (mAllocator[ix].mNext == VAR_USED)
				_DESTROY_ (&mAllocator[ix].mData) ;
			mAllocator[ix].mNext = iy ;
		}
		mLength = 0 ;
		mFree = ix ;
	}

	inline BOOL used (INDEX index) const {
		if (mAllocator[index].mNext != VAR_USED)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &get (INDEX index) & {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<UNIT> (mAllocator[index].mData) ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<UNIT> (mAllocator[index].mData) ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = mAllocator.at (_OFFSET_ (&Node::mData ,_CAST_<TEMP<UNIT>> (item))) ;
		if (!used (ret))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline void swap (Allocator &that) popping {
		mAllocator.swap (that.mAllocator) ;
		_SWAP_ (mLength ,that.mLength) ;
		_SWAP_ (mFree ,that.mFree) ;
	}

	template <class... _ARGS>
	inline INDEX alloc (_ARGS &&...args) popping {
		INDEX ret = VAR_NONE ;
		for (FOR_ONCE_DO_WHILE) {
			if (ret != VAR_NONE)
				discard ;
			if (mFree != VAR_NONE)
				discard ;
			auto rax = mAllocator.expand () ;
			_CREATE_ (&rax[mLength].mData ,std::forward<_ARGS> (args)...) ;
			for (INDEX i = 0 ; i < mAllocator.size () ; i++) {
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<UNIT> (mAllocator[i].mData))) ;
				rax[i].mNext = VAR_USED ;
			}
			mAllocator.swap (rax) ;
			update_reset (mLength ,mFree) ;
			ret = mFree ;
		}
		for (FOR_ONCE_DO_WHILE) {
			if (ret != VAR_NONE)
				discard ;
			if (mFree == VAR_NONE)
				discard ;
			_CREATE_ (&mAllocator[mFree].mData ,std::forward<_ARGS> (args)...) ;
			ret = mFree ;
		}
		mFree = mAllocator[ret].mNext ;
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
		const auto r1x = mAllocator.size () ;
		_DEBUG_ASSERT_ (len >= 0) ;
		const auto r2x = _MAX_ (len - (r1x - mLength) ,VAR_ZERO) ;
		if (r2x == 0)
			return ;
		_DEBUG_ASSERT_ (r1x + r2x > r1x) ;
		auto rax = mAllocator.expand (r1x + r2x) ;
		for (INDEX i = 0 ; i < r1x ; i++) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&rax[i].mData ,std::move (_CAST_<UNIT> (mAllocator[i].mData))) ;
			rax[i].mNext = mAllocator[i].mNext ;
		}
		mAllocator.swap (rax) ;
		update_reset (r1x ,mFree) ;
	}

	inline void clean () {
		const auto r1x = shrink_size () ;
		if (r1x == mAllocator.size ())
			return ;
		_DYNAMIC_ASSERT_ (r1x == mLength) ;
		auto rax = mAllocator.expand (r1x) ;
		for (INDEX i = 0 ; i < rax.size () ; i++) {
			_DEBUG_ASSERT_ (mAllocator[i].mNext == VAR_USED) ;
			_CREATE_ (&rax[i].mData ,std::move (_CAST_<UNIT> (mAllocator[i].mData))) ;
			rax[i].mNext = VAR_USED ;
		}
		mAllocator.swap (rax) ;
		update_reset (mLength ,VAR_NONE) ;
	}

private:
	inline void update_reset (INDEX _length ,INDEX _free) {
		INDEX ix = _free ;
		INDEX iy = VAR_NONE ;
		for (INDEX i = 0 ,ie = mAllocator.size () - _length ; i < ie ; i++) {
			iy = ix ;
			ix = mAllocator.size () + ~i ;
			mAllocator[ix].mNext = iy ;
		}
		mLength = _length ;
		mFree = ix ;
	}

	inline LENGTH shrink_size () const {
		LENGTH ret = mAllocator.size () ;
		while (ret - 1 >= 0 && mAllocator[ret - 1].mNext != VAR_USED)
			ret-- ;
		return std::move (ret) ;
	}
} ;
} ;