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
#pragma warning (disable :4068) //@info: warning C4068: unknown pragma
#pragma warning (disable :4619) //@info: warning C4619: #pragma warning: there is no warning number 'xxx'
#pragma warning (disable :4100) //@info: warning C4100: 'xxx': unreferenced formal parameter
#pragma warning (disable :4180) //@info: warning C4180: qualifier applied to function type has no meaning; ignored
#pragma warning (disable :4365) //@info: warning C4365: 'xxx': conversion from 'xxx' to 'xxx', signed/unsigned mismatch
#pragma warning (disable :4371) //@info: warning C4371: 'xxx': layout of class may have changed from a previous version of the compiler due to better packing of member 'xxx'
#pragma warning (disable :4464) //@info: warning C4464: relative include path contains '..'
#pragma warning (disable :4505) //@info: warning C4505: 'xxx': unreferenced local function has been removed
#pragma warning (disable :4514) //@info: warning C4514: 'xxx': unreferenced inline function has been removed
#pragma warning (disable :4571) //@info: warning C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
#pragma warning (disable :4574) //@info: warning C4574: 'xxx' is defined to be '0': did you mean to use '#if xxx'?
#pragma warning (disable :4668) //@info: warning C4668: 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning (disable :4710) //@info: warning C4710: 'xxx': function not inlined
#pragma warning (disable :4711) //@info: warning C4711: function 'xxx' selected for automatic inline expansion
#pragma warning (disable :4774) //@info: warning C4774: 'xxx' : format string expected in argument ? is not a string literal
#pragma warning (disable :4324) //@info: warning C4324: 'xxx': structure was padded due to alignment specifier
#pragma warning (disable :4820) //@info: warning C4820: 'xxx': 'xxx' bytes padding added after data member 'xxx'
#pragma warning (disable :4623) //@info: warning C4623: 'xxx': default constructor was implicitly defined as deleted
#pragma warning (disable :4624) //@info: warning C4624: 'xxx': destructor was implicitly defined as deleted
#pragma warning (disable :4625) //@info: warning C4625: 'xxx': copy constructor was implicitly defined as deleted
#pragma warning (disable :4626) //@info: warning C4626: 'xxx': assignment operator was implicitly defined as deleted
#pragma warning (disable :5026) //@info: warning C5026: 'xxx': move constructor was implicitly defined as deleted
#pragma warning (disable :5027) //@info: warning C5027: 'xxx': move assignment operator was implicitly defined as deleted
#pragma warning (disable :5045) //@info: warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
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
} ;

#define _UNWIND_X_(...) __VA_ARGS__
#define _UNW_(...) _UNWIND_X_(__VA_ARGS__)
#define _STRINGIZE_X_(...) #__VA_ARGS__
#define _STR_(...) _STRINGIZE_X_(__VA_ARGS__)
#define _CONCAT_X_(lhs ,rhs) lhs ## rhs
#define _CAT_(lhs ,rhs) _CONCAT_X_(lhs ,rhs)

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

#define SWITCH_CASE(var) (TRUE) for ( ; !var ; var = TRUE)
#define SWITCH_ONCE(var) (var) for (auto ANONYMOUS = FALSE ; !ANONYMOUS ; ANONYMOUS = TRUE)

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
//@error: g++4.8 is too useless to use references to array of unknown bound as parameters
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
		return (*PTR<ARR<_ARG1>> (that)) ;
	}

	template <class _ARG1 ,LENGTH _VAL1>
	inline constexpr ARR<_ARG1> &operator[] (DEF<_ARG1[_VAL1]> &that) const {
		return (*PTR<ARR<_ARG1>> (&that)) ;
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

#define _PCSTRU8_(text) CSC::Plain<CSC::STRU8> (_CAT_ (u8 ,text))
#define _PCSTRU16_(text) CSC::Plain<CSC::STRU16> (_CAT_ (u ,text))
#define _PCSTRU32_(text) CSC::Plain<CSC::STRU32> (_CAT_ (U ,text))

using STRA = char ;
using STRW = wchar_t ;

#define _PCSTRA_(text) CSC::Plain<CSC::STRA> (_UNW_ (text))
#define _PCSTRW_(text) CSC::Plain<CSC::STRW> (_CAT_ (L ,text))

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

template <class>
struct ARGV {} ;

template <class...>
struct ARGVS ;

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
#ifdef __CSC__
namespace U {
template <class>
struct ENABLE ;

template <>
struct ENABLE<ARGC<TRUE>> {
	using TYPE = VOID ;
} ;

template <BOOL _VAL1>
using ENABLE_TYPE = typename ENABLE<ARGC<_VAL1>>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct CONDITIONAL ;

template <class _ARG1 ,class _ARG2>
struct CONDITIONAL<ARGC<TRUE> ,_ARG1 ,_ARG2> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2>
struct CONDITIONAL<ARGC<FALSE> ,_ARG1 ,_ARG2> {
	using TYPE = _ARG2 ;
} ;

template <BOOL _VAL1 ,class _ARG1 ,class _ARG2>
using CONDITIONAL_TYPE = typename CONDITIONAL<ARGC<_VAL1> ,_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_REFERENCE {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_REFERENCE<_ARG1 &> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_REFERENCE<_ARG1 &&> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_REFERENCE_TYPE = typename REMOVE_REFERENCE<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_CONST {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_CONST<const _ARG1> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_CONST_TYPE = typename REMOVE_CONST<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_VOLATILE {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_VOLATILE<volatile _ARG1> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_VOLATILE_TYPE = typename REMOVE_VOLATILE<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
using REMOVE_CVR_TYPE = REMOVE_CONST_TYPE<REMOVE_VOLATILE_TYPE<REMOVE_REFERENCE_TYPE<_ARG1>>> ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_POINTER {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_POINTER<PTR<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_POINTER_TYPE = typename REMOVE_POINTER<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_ARRAY {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_ARRAY<ARR<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,LENGTH _VAL1>
struct REMOVE_ARRAY<_ARG1[_VAL1]> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_ARRAY_TYPE = typename REMOVE_ARRAY<_ARG1>::TYPE ;
} ;

namespace U {
template <class _ARG1>
struct REMOVE_TEMP {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
struct REMOVE_TEMP<TEMP<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_TEMP_TYPE = typename REMOVE_TEMP<REMOVE_CVR_TYPE<_ARG1>>::TYPE ;
} ;

namespace U {
template <class>
struct REMOVE_MEMFUNC ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...)> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) const> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) volatile> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) const volatile> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

//@warn: behaviors of 'noexcept' changed in C++17
#if _HAS_CXX17
template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) const noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) volatile noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct REMOVE_MEMFUNC<_ARG1 (_ARGS...) const volatile noexcept> {
	using TYPE = DEF<_ARG1 (_ARGS...)> ;
} ;
#endif

template <class _ARG1>
using REMOVE_MEMFUNC_TYPE = typename REMOVE_MEMFUNC<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct REMOVE_MEMBER ;

template <class _ARG1 ,class _ARG2>
struct REMOVE_MEMBER<_ARG1 _ARG2::*> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_MEMBER_TYPE = typename REMOVE_MEMBER<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct MEMBER_OF ;

template <class _ARG1 ,class _ARG2>
struct MEMBER_OF<_ARG1 _ARG2::*> {
	using TYPE = _ARG2 ;
} ;

template <class _ARG1>
using MEMBER_OF_TYPE = typename MEMBER_OF<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct INVOKE_RESULT ;

template <class _ARG1 ,class... _ARGS>
struct INVOKE_RESULT<_ARG1 (_ARGS...)> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using INVOKE_RESULT_TYPE = typename INVOKE_RESULT<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct INVOKE_PARAMS ;

template <class _ARG1 ,class... _ARGS>
struct INVOKE_PARAMS<_ARG1 (_ARGS...)> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using INVOKE_PARAMS_TYPE = typename INVOKE_PARAMS<_ARG1>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct RESULT_OF ;

#ifdef __CSC_COMPILER_CLANG__
template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct RESULT_OF {
	using TYPE = NONE ;
} ;
#endif

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct RESULT_OF<_ARG1 (_ARGS...) ,ARGVS<_ARGS...> ,_ARG2> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2>
struct RESULT_OF<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (decltype (&_ARG1::operator())) > 0)>> {
	using TYPE = typename RESULT_OF<REMOVE_MEMFUNC_TYPE<REMOVE_MEMBER_TYPE<decltype (&_ARG1::operator())>> ,_ARG2 ,VOID>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
using RESULT_OF_TYPE = typename RESULT_OF<REMOVE_CVR_TYPE<REMOVE_POINTER_TYPE<_ARG1>> ,_ARG2 ,VOID>::TYPE ;
} ;

namespace U {
template <class>
struct COUNTOF_TRAITS ;

template <class _ARG1>
struct COUNTOF_TRAITS<ARR<_ARG1>> {
	using TYPE = ARGC<0> ;
} ;

template <class _ARG1 ,LENGTH _VAL1>
struct COUNTOF_TRAITS<_ARG1[_VAL1]> {
	using TYPE = ARGC<_VAL1> ;
} ;

template <class _ARG1>
using COUNTOF_TRAITS_TYPE = typename COUNTOF_TRAITS<_ARG1>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct is_full_array_of_help :public stl::false_type {} ;

template <class _ARG1 ,LENGTH _VAL1>
struct is_full_array_of_help<_ARG1 ,DEF<_ARG1[_VAL1]> ,ENABLE_TYPE<(_VAL1 > 0)>> :public stl::true_type {} ;

template <class _ARG1 ,class _ARG2>
using is_full_array_of = is_full_array_of_help<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<_ARG2> ,VOID> ;
} ;

namespace U {
template <class _ARG1>
struct FORWARD_TRAITS {
	using TYPE = const _ARG1 & ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &> {
	using TYPE = _ARG1 & ;
} ;

template <class _ARG1>
struct FORWARD_TRAITS<_ARG1 &&> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using FORWARD_TRAITS_TYPE = typename FORWARD_TRAITS<_ARG1>::TYPE ;
} ;

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

template <class _ARG1 ,class _ARG2>
using CAST_TRAITS_TYPE = typename CAST_TRAITS<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class>
struct is_var_xyz_help :public stl::false_type {} ;

template <>
struct is_var_xyz_help<VAR32 ,VOID> :public stl::true_type {} ;

template <>
struct is_var_xyz_help<VAR64 ,VOID> :public stl::true_type {} ;

template <class _ARG1>
struct is_var_xyz_help<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,VARX>::value && !stl::is_same<_ARG1 ,VAR32>::value && !stl::is_same<_ARG1 ,VAR64>::value>> :public stl::true_type {} ;

template <class _ARG1>
struct is_var_xyz_help<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,VARY>::value && !stl::is_same<_ARG1 ,CHAR>::value && !stl::is_same<_ARG1 ,DATA>::value>> :public stl::true_type {} ;

template <class _ARG1>
using is_var_xyz = is_var_xyz_help<_ARG1 ,VOID> ;
} ;

namespace U {
template <class ,class>
struct is_val_xyz_help :public stl::false_type {} ;

template <>
struct is_val_xyz_help<VAL32 ,VOID> :public stl::true_type {} ;

template <>
struct is_val_xyz_help<VAL64 ,VOID> :public stl::true_type {} ;

template <class _ARG1>
struct is_val_xyz_help<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,VALX>::value && !stl::is_same<_ARG1 ,VAL32>::value && !stl::is_same<_ARG1 ,VAL64>::value>> :public stl::true_type {} ;

template <class _ARG1>
using is_val_xyz = is_val_xyz_help<_ARG1 ,VOID> ;
} ;

namespace U {
template <class>
struct is_byte_xyz_help :public stl::false_type {} ;

template <>
struct is_byte_xyz_help<BYTE> :public stl::true_type {} ;

template <>
struct is_byte_xyz_help<WORD> :public stl::true_type {} ;

template <>
struct is_byte_xyz_help<CHAR> :public stl::true_type {} ;

template <>
struct is_byte_xyz_help<DATA> :public stl::true_type {} ;

template <>
struct is_byte_xyz_help<MEGA> :public stl::true_type {} ;

template <class _ARG1>
using is_byte_xyz = U::is_byte_xyz_help<_ARG1> ;
} ;

namespace U {
template <class ,class>
struct is_str_xyz_help :public stl::false_type {} ;

template <>
struct is_str_xyz_help<STRU8 ,VOID> :public stl::true_type {} ;

template <>
struct is_str_xyz_help<STRU16 ,VOID> :public stl::true_type {} ;

template <>
struct is_str_xyz_help<STRU32 ,VOID> :public stl::true_type {} ;

template <>
struct is_str_xyz_help<STRA ,VOID> :public stl::true_type {} ;

template <>
struct is_str_xyz_help<STRW ,VOID> :public stl::true_type {} ;

template <class _ARG1>
struct is_str_xyz_help<_ARG1 ,ENABLE_TYPE<stl::is_same<_ARG1 ,STRX>::value && !stl::is_same<_ARG1 ,STRA>::value && !stl::is_same<_ARG1 ,STRW>::value>> :public stl::true_type {} ;

template <class _ARG1>
using is_str_xyz = is_str_xyz_help<_ARG1 ,VOID> ;
} ;

namespace U {
template <class ,class ,class ,class>
struct LOAD_CHECK {
	using TYPE = ARGC<FALSE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<_ARG1 ,TEMP<_ARG1> ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<TEMP<_ARG1> ,_ARG1 ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<ARR<_ARG1> ,ARR<TEMP<_ARG1>> ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<ARR<TEMP<_ARG1>> ,ARR<_ARG1> ,VOID ,ARGC<1>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<1>> {
	using TYPE = typename LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>>::TYPE ;
} ;

template <class _ARG1>
struct LOAD_CHECK<_ARG1 ,NONE ,ENABLE_TYPE<stl::is_class<_ARG1>::value> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<NONE ,_ARG1 ,ENABLE_TYPE<stl::is_class<_ARG1>::value> ,ARGC<2>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<2>> {
	using TYPE = typename LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>>::TYPE ;
} ;

template <>
struct LOAD_CHECK<ARR<BYTE> ,ARR<BOOL> ,VOID ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<is_var_xyz<_ARG1>::value> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<is_val_xyz<_ARG1>::value> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(is_byte_xyz<_ARG1>::value && !is_str_xyz<_ARG1>::value)> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1>
struct LOAD_CHECK<ARR<BYTE> ,ARR<_ARG1> ,ENABLE_TYPE<(is_str_xyz<_ARG1>::value && !is_byte_xyz<_ARG1>::value)> ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

//@info: compatible for old c api
template <>
struct LOAD_CHECK<ARR<STRA> ,ARR<BYTE> ,VOID ,ARGC<3>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<3>> {
	using TYPE = typename LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
struct LOAD_CHECK<TEMP<_ARG1> ,TEMP<_ARG2> ,ENABLE_TYPE<(_SIZEOF_ (TEMP<_ARG2>) >= _SIZEOF_ (TEMP<_ARG1>) && _ALIGNOF_ (TEMP<_ARG2>) % _ALIGNOF_ (TEMP<_ARG1>) == 0)> ,ARGC<4>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3>
struct LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<4>> {
	using TYPE = typename LOAD_CHECK<_ARG1 ,_ARG2 ,_ARG3 ,ARGC<5>>::TYPE ;
} ;

template <class _ARG1>
struct LOAD_CHECK<_ARG1 ,VOID ,ENABLE_TYPE<!stl::is_pointer<_ARG1>::value> ,ARGC<5>> {
	using TYPE = ARGC<TRUE> ;
} ;

template <class _ARG1 ,class _ARG2>
using LOAD_CHECK_TYPE = typename LOAD_CHECK<_ARG1 ,_ARG2 ,VOID ,ARGC<1>>::TYPE ;
} ;

namespace U {
template <class ,class ,class>
struct INDEX_OF {
	using TYPE = ARGC<VAR_NONE> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEX_OF<_ARG1 ,REMOVE_CVR_TYPE<_ARG2> ,ARGVS<_ARG2 ,_ARGS...>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
struct INDEX_OF<_ARG1 ,_ARG2 ,ARGVS<_ARG3 ,_ARGS...>> {
	using TYPE = typename INDEX_OF<ARGC<_ARG1::value + 1> ,_ARG2 ,ARGVS<_ARGS...>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
using INDEX_OF_TYPE = typename INDEX_OF<ARGC<0> ,REMOVE_CVR_TYPE<_ARG1> ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class>
struct INDEX_TO ;

template <class _ARG1 ,class... _ARGS>
struct INDEX_TO<ARGC<0> ,ARGVS<_ARG1 ,_ARGS...>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEX_TO<_ARG1 ,ARGVS<_ARG2 ,_ARGS...>> {
	_STATIC_ASSERT_ (LENGTH (_ARG1::value) > 0) ;
	using TYPE = typename INDEX_TO<ARGC<_ARG1::value - 1> ,ARGVS<_ARGS...>>::TYPE ;
} ;

template <class _ARG1 ,class _ARG2>
using INDEX_TO_TYPE = typename INDEX_TO<_ARG1 ,_ARG2>::TYPE ;
} ;

namespace U {
template <class ,class>
struct is_complete_type_help :public stl::false_type {} ;

template <class _ARG1>
struct is_complete_type_help<_ARG1 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0)>> :public stl::true_type {} ;

template <class _ARG1>
using is_complete_type = is_complete_type_help<_ARG1 ,VOID> ;
} ;

namespace U {
template <class ,class ,class>
struct is_interface_type_help :public stl::false_type {} ;

template <class _ARG1 ,class _ARG2>
struct is_interface_type_help<_ARG1 ,_ARG2 ,ENABLE_TYPE<_SIZEOF_ (_ARG1) == _SIZEOF_ (_ARG2) && _ALIGNOF_ (_ARG1) == _ALIGNOF_ (_ARG2)>> :public stl::is_base_of<_ARG2 ,_ARG1> {} ;

template <class _ARG1 ,class _ARG2>
using is_interface_type = is_interface_type_help<_ARG1 ,_ARG2 ,VOID> ;
} ;

namespace U {
template <class ,class ,class>
struct is_always_base_of_help :public stl::false_type {} ;

template <class _ARG1 ,class _ARG2>
struct is_always_base_of_help<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0 && _SIZEOF_ (_ARG2) > 0)>> :public stl::is_base_of<_ARG1 ,_ARG2> {} ;

template <class _ARG1 ,class _ARG2>
using is_always_base_of = is_always_base_of_help<_ARG1 ,_ARG2 ,VOID> ;
} ;

namespace U {
template <class...>
struct is_all_same_help :public stl::false_type {} ;

template <class _ARG1>
struct is_all_same_help<_ARG1> :public stl::true_type {} ;

template <class _ARG1>
struct is_all_same_help<_ARG1 ,_ARG1> :public stl::true_type {} ;

template <class _ARG1 ,class... _ARGS>
struct is_all_same_help<_ARG1 ,_ARG1 ,_ARGS...> :public is_all_same_help<_ARG1 ,_ARGS...> {} ;

template <class... _ARGS>
using is_all_same = is_all_same_help<_ARGS...> ;
} ;

namespace U {
template <class...>
struct is_any_same_help :public stl::false_type {} ;

template <class _ARG1>
struct is_any_same_help<_ARG1> :public stl::false_type {} ;

template <class _ARG1 ,class... _ARGS>
struct is_any_same_help<_ARG1 ,_ARG1 ,_ARGS...> :public stl::true_type {} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct is_any_same_help<_ARG1 ,_ARG2 ,_ARGS...> :public CONDITIONAL_TYPE<(is_any_same_help<_ARG1 ,_ARGS...>::value && is_any_same_help<_ARG2 ,_ARGS...>::value) ,stl::true_type ,stl::false_type> {} ;

template <class... _ARGS>
using is_any_same = is_any_same_help<_ARGS...> ;
} ;

namespace U {
template <class>
struct is_template_type_help :public stl::false_type {} ;

template <template <class...> class _ARGT ,class... _ARGS>
struct is_template_type_help<_ARGT<_ARGS...>> :public stl::true_type {} ;

template <class _ARG1>
using is_template_type = is_template_type_help<_ARG1> ;
} ;

namespace U {
template <class>
struct REMOVE_TEMPLATE ;

template <template <class> class _ARGT ,class _ARG1>
struct REMOVE_TEMPLATE<_ARGT<_ARG1>> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1>
using REMOVE_TEMPLATE_TYPE = typename REMOVE_TEMPLATE<_ARG1>::TYPE ;
} ;

namespace U {
template <class>
struct TEMPLATE_PARAMS ;

template <template <class...> class _ARGT ,class... _ARGS>
struct TEMPLATE_PARAMS<_ARGT<_ARGS...>> {
	using TYPE = ARGVS<_ARGS...> ;
} ;

template <class _ARG1>
using TEMPLATE_PARAMS_TYPE = typename TEMPLATE_PARAMS<_ARG1>::TYPE ;
} ;
#endif
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

template <class _ARG1 ,class _ARG2>
using INDEX_OF_TYPE = U::INDEX_OF_TYPE<_ARG1 ,_ARG2> ;

template <class _ARG1 ,class _ARG2>
using INDEX_TO_TYPE = U::INDEX_TO_TYPE<_ARG1 ,_ARG2> ;

template <class _ARG1>
using REMOVE_TEMPLATE_TYPE = U::REMOVE_TEMPLATE_TYPE<_ARG1> ;

template <class _ARG1>
using TEMPLATE_PARAMS_TYPE = U::TEMPLATE_PARAMS_TYPE<_ARG1> ;

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

namespace stl {
template <class... _ARGS>
using is_all_same = U::is_all_same<_ARGS...> ;

template <class... _ARGS>
using is_any_same = U::is_any_same<_ARGS...> ;
} ;

template <class UNIT>
struct TEMP {
	_STATIC_ASSERT_ (!std::is_reference<UNIT>::value) ;
	alignas (UNIT) DEF<BYTE[_SIZEOF_ (UNIT)]> unused ;
} ;

template <class _RET>
inline constexpr _RET &_NULL_ () {
	return (*PTR<REMOVE_REFERENCE_TYPE<_RET>> (NULL)) ;
}

template <class _RET>
inline REMOVE_REFERENCE_TYPE<_RET> &_XVALUE_ (REMOVE_CVR_TYPE<_RET> &object) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return object ;
}

template <class _RET>
inline const REMOVE_REFERENCE_TYPE<_RET> &_XVALUE_ (const REMOVE_CVR_TYPE<_RET> &object) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return object ;
}

template <class _ARG1>
inline LENGTH _ADDRESS_ (PTR<_ARG1> address) popping {
	//@info: as 'asm volatile("" :: "rm" (address) : "memory") ;'
	static volatile auto mInstance = _XVALUE_<PTR<void (PTR<_ARG1>)>> ([] (PTR<_ARG1>) {}) ;
	mInstance (address) ;
	return LENGTH (address) ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_CAST_ (_ARG1 &object) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_RET>::value) ;
	_STATIC_ASSERT_ (!(std::is_pointer<_ARG1>::value && !std::is_same<_RET ,TEMP<_ARG1>>::value)) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG1)) ;
	_STATIC_ASSERT_ (_ALIGNOF_ (_ARG1) % _ALIGNOF_ (_RET) == 0) ;
	const auto r1x = reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (_ADDRESS_ (&object)) ;
#ifdef __CSC_COMPILER_GNUC__
	_ADDRESS_ (&object) ;
#endif
	return (*r1x) ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_LOAD_ (PTR<_ARG1> address) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (LOAD_CHECK_TYPE<REMOVE_CVR_TYPE<_RET> ,REMOVE_CVR_TYPE<_ARG1>>::value) ;
	_DEBUG_ASSERT_ (address != NULL) ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<(std::is_same<REMOVE_CVR_TYPE<_RET> ,VOID>::value || std::is_same<REMOVE_CVR_TYPE<_RET> ,NONE>::value || std::is_same<REMOVE_CVR_TYPE<_RET> ,REMOVE_CVR_TYPE<_ARG1>>::value) ,BYTE ,REMOVE_ARRAY_TYPE<_RET>>) ;
	_DEBUG_ASSERT_ (_ADDRESS_ (address) % r1x == 0) ;
	(void) r1x ;
	const auto r2x = reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (_ADDRESS_ (address)) ;
#ifdef __CSC_COMPILER_GNUC__
	_ADDRESS_ (address) ;
#endif
	return (*r2x) ;
}

//@warn: not type-safe; be careful about strict-aliasing
template <class _RET ,class _ARG1>
inline CAST_TRAITS_TYPE<_RET ,_ARG1> &_LOAD_ (PTR<_ARG1> owner ,LENGTH address) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (LOAD_CHECK_TYPE<REMOVE_CVR_TYPE<_RET> ,VOID>::value) ;
	_DEBUG_ASSERT_ (address != 0) ;
	const auto r1x = _ALIGNOF_ (CONDITIONAL_TYPE<(std::is_same<REMOVE_CVR_TYPE<_RET> ,VOID>::value || std::is_same<REMOVE_CVR_TYPE<_RET> ,NONE>::value) ,BYTE ,REMOVE_ARRAY_TYPE<_RET>>) ;
	_DEBUG_ASSERT_ (address % r1x == 0) ;
	(void) r1x ;
	const auto r2x = reinterpret_cast<PTR<CAST_TRAITS_TYPE<_RET ,_ARG1>>> (address) ;
#ifdef __CSC_COMPILER_GNUC__
	_ADDRESS_ (owner) ;
#endif
	return (*r2x) ;
}

template <class _ARG1 ,class _ARG2 ,class _ARG3>
inline CAST_TRAITS_TYPE<_ARG2 ,_ARG3> &_OFFSET_ (const DEF<_ARG1 _ARG2::*> &mptr ,_ARG3 &mref) noexcept {
	_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG3> ,_ARG1>::value) ;
	_DEBUG_ASSERT_ (mptr != NULL) ;
	const auto r1x = _ADDRESS_ (&mref) - _ADDRESS_ (&(_NULL_<_ARG2> ().*mptr)) ;
	return _LOAD_<_ARG2> (&mref ,r1x) ;
}

template <class _ARG1>
inline void _ZERO_ (_ARG1 &object) noexcept {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_CAST_<TEMP<_ARG1>> (object) = {0} ;
}

template <class _ARG1>
inline _ARG1 _COPY_ (const _ARG1 &object) {
	return object ;
}

template <class _ARG1>
inline _ARG1 _EXCHANGE_ (_ARG1 &handle) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_ARG1 ret = handle ;
	_ZERO_ (handle) ;
	return std::move (ret) ;
}

template <class _ARG1>
inline _ARG1 _EXCHANGE_ (_ARG1 &handle ,const REMOVE_CVR_TYPE<_ARG1> &nil_) noexcept popping {
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_ARG1 ret = handle ;
	handle = nil_ ;
	return std::move (ret) ;
}

template <class _ARG1>
inline void _SWAP_ (_ARG1 &lhs ,_ARG1 &rhs) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_nothrow_move_assignable<_ARG1>::value) ;
	auto rax = std::move (lhs) ;
	lhs = std::move (rhs) ;
	rhs = std::move (rax) ;
}

template <class _ARG1 ,class... _ARGS>
inline void _CREATE_ (PTR<TEMP<_ARG1>> address ,_ARGS &&...initval) {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG1>::value) ;
	auto &r1y = _LOAD_<_ARG1> (address) ;
	const auto r2x = new (&r1y) _ARG1 (std::forward<_ARGS> (initval)...) ;
	_DEBUG_ASSERT_ (r2x == &r1y) ;
	(void) r2x ;
}

template <class _ARG1>
inline void _DESTROY_ (PTR<TEMP<_ARG1>> address) noexcept {
	_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_array<_ARG1>::value) ;
	auto &r1y = _LOAD_<_ARG1> (address) ;
	r1y.~_ARG1 () ;
	(void) r1y ;
}

template <class _ARG1>
inline FLAG _TYPEUID_ (const ARGV<_ARG1> &) noexcept ;

template <class _RET>
inline FLAG _TYPEUID_ () noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	return _TYPEUID_ (_NULL_<ARGV<REMOVE_CVR_TYPE<_RET>>> ()) ;
}

template <class _ARG1>
inline constexpr _ARG1 &&_SWITCH_ (_ARG1 &&expr) {
	return std::forward<_ARG1> (expr) ;
}

inline namespace S {
template <class _ARG1>
inline constexpr _ARG1 _ABS_ (const _ARG1 &val) {
	return _SWITCH_ (
		(val < 0) ? (-val) :
		(+val)) ;
}

template <class _ARG1>
inline constexpr _ARG1 _SQE_ (const _ARG1 &val) {
	return val * val ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_MIN_ (const _ARG1 &lhs ,const _ARG1 &rhs) {
	return _SWITCH_ (
		!(rhs < lhs) ? lhs :
		rhs) ;
}

template <class _ARG1>
inline constexpr const _ARG1 &_MAX_ (const _ARG1 &lhs ,const _ARG1 &rhs) {
	return _SWITCH_ (
		!(lhs < rhs) ? lhs :
		rhs) ;
}
} ;

template <class _ARG1>
inline RESULT_OF_TYPE<_ARG1 ,ARGVS<>> _CALL_ (_ARG1 &&func) popping {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	return func () ;
}

//@warn: assure ruined object when an exception was thrown
template <class _ARG1>
inline void _CALL_TRY_ (_ARG1 &&proc) {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	proc () ;
}

//@warn: assure ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (_ARG1 &&proc_one ,_ARGS &&...proc_rest) ;

template <class _ARG1>
inline void _CATCH_ (_ARG1 &&proc) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	proc () ;
}

//@info: this function is incompleted without 'csc_extend.hpp'
template <class _ARG1 ,class _ARG2>
inline void _CATCH_ (_ARG1 &&try_proc ,_ARG2 &&catch_proc) noexcept ;

template <class _ARG1>
inline const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> &_CACHE_ (_ARG1 &&func) popping {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_reference<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
	static const RESULT_OF_TYPE<_ARG1 ,ARGVS<>> mInstance = func () ;
	return mInstance ;
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

template <class _ARG1>
inline FLAG _TYPEUID_ (const ARGV<_ARG1> &) noexcept {
	_STATIC_ASSERT_ (std::is_same<_ARG1 ,REMOVE_CVR_TYPE<_ARG1>>::value) ;
	//@warn: RTTI might be different across DLL
	class Storage final :private Interface {} ;
	Storage ret ;
	return std::move (_CAST_<FLAG> (ret)) ;
}

namespace stl {
template <class _ARG1>
using is_template_type = U::is_template_type<_ARG1> ;

template <class _ARG1>
using is_complete_type = U::is_complete_type<_ARG1> ;

template <class _ARG1>
using is_interface_type = U::is_interface_type<_ARG1 ,Interface> ;

template <class _ARG1 ,class _ARG2>
using is_always_base_of = U::is_always_base_of<_ARG1 ,_ARG2> ;
} ;

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
	inline constexpr implicit Plain (DEF<REAL[_VAL1]> &) = delete ;

	template <LENGTH _VAL1>
	inline constexpr implicit Plain (const DEF<REAL[_VAL1]> &that) :mPlain (&that[0]) ,mSize (_VAL1 - 1) {}

	template <class _ARG1>
	inline explicit Plain (_ARG1 &) = delete ;

	template <class _ARG1 ,class = ENABLE_TYPE<!stl::is_full_array_of<REAL ,_ARG1>::value>>
	inline explicit Plain (const _ARG1 &that) noexcept :Plain (_CAST_<REAL[_COUNTOF_ (_ARG1)]> (that)) {}

	template <class _ARG1 ,class... _ARGS>
	inline explicit Plain (const ARGV<_ARG1> & ,const _ARGS &...text) noexcept :Plain (Detail::cache_string (_NULL_<ARGV<_ARG1>> () ,text...)) {}

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
			inline explicit PlainString (const _ARGS &...text) noexcept {
				template_write (mString ,_NULL_<ARGV<ARGC<0>>> () ,text...) ;
			}
		} ;

		inline static constexpr LENGTH constexpr_cache_string_size (const ARGV<ARGVS<>> &) {
			return 1 ;
		}

		template <class _ARG1 ,class... _ARGS>
		inline static constexpr LENGTH constexpr_cache_string_size (const ARGV<ARGVS<_ARG1 ,_ARGS...>> &) {
			return _COUNTOF_ (_ARG1) - 1 + constexpr_cache_string_size (_NULL_<ARGV<ARGVS<_ARGS...>>> ()) ;
		}

		template <class... _ARGS>
		using PLAIN_STRING_SIZE = ARGC<constexpr_cache_string_size (_NULL_<ARGV<ARGVS<_ARGS...>>> ())> ;

		template <class _ARG1 ,class... _ARGS>
		inline static const DEF<REAL[PLAIN_STRING_SIZE<_ARGS...>::value]> &cache_string (const ARGV<_ARG1> & ,const _ARGS &...text) noexcept {
			const auto r1x = PlainString<PLAIN_STRING_SIZE<_ARGS...>> (text...) ;
			auto &r2y = _CACHE_ ([r1x] () noexcept {
				return r1x ;
			}) ;
			return r2y.mString ;
		}

		template <class _ARG1 ,class _ARG2>
		inline static void template_write (_ARG1 &array ,const ARGV<_ARG2> &) noexcept {
			_STATIC_ASSERT_ (stl::is_full_array_of<REAL ,_ARG1>::value) ;
			_STATIC_ASSERT_ (LENGTH (_ARG2::value) == _COUNTOF_ (_ARG1) - 1) ;
			array[_ARG2::value] = 0 ;
		}

		template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
		inline static void template_write (_ARG1 &array ,const ARGV<_ARG2> & ,const _ARG3 &text_one ,const _ARGS &...text_rest) noexcept {
			_STATIC_ASSERT_ (stl::is_full_array_of<REAL ,_ARG1>::value) ;
			_STATIC_ASSERT_ (LENGTH (_ARG2::value) >= 0 && LENGTH (_ARG2::value) < _COUNTOF_ (_ARG1)) ;
			_STATIC_ASSERT_ (stl::is_full_array_of<STRX ,_ARG3>::value || stl::is_full_array_of<STRA ,_ARG3>::value || stl::is_full_array_of<STRW ,_ARG3>::value) ;
			for (INDEX i = 0 ,ie = _COUNTOF_ (_ARG3) - 1 ; i < ie ; i++)
				array[i + _ARG2::value] = REAL (text_one[i]) ;
			template_write (array ,_NULL_<ARGV<ARGC<_ARG2::value + _COUNTOF_ (_ARG3) - 1>>> () ,text_rest...) ;
		}
	} ;
} ;

class Exception final {
private:
	const ARR<STR> &mWhat ;

public:
	inline Exception () = delete ;

	inline explicit Exception (const Plain<STR> &what_) noexcept :mWhat (what_.self) {}

	inline Exception (const Exception &) = default ;
	inline Exception &operator= (const Exception &) = delete ;

	inline Exception (Exception &&) noexcept = default ;
	inline Exception &operator= (Exception &&) = delete ;

	inline const ARR<STR> &what () const noexcept {
		return mWhat ;
	}

	inline void raise () const {
		throw (*this) ;
	}
} ;

//@warn: assure ruined object when an exception was thrown
template <class _ARG1 ,class... _ARGS>
inline void _CALL_TRY_ (_ARG1 &&proc_one ,_ARGS &&...proc_rest) {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	try {
		proc_one () ;
		return ;
	} catch (const Exception &) {
		_STATIC_WARNING_ ("noop") ;
	}
	_CALL_TRY_ (std::forward<_ARGS> (proc_rest)...) ;
}
} ;