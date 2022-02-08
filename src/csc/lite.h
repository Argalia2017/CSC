﻿
#ifdef __CSC_COMPILER_MSVC__
#include <yvals_core.h>
#endif

#include <initializer_list>
#include <new>

#ifdef __CSC_COMPILER_GNUC__
#include <type_traits>

#define __is_constructible(...) std::is_constructible<__VA_ARGS__>::value
#define __is_assignable(...) std::is_assignable<__VA_ARGS__>::value
#define __is_nothrow_constructible(...) std::is_nothrow_constructible<__VA_ARGS__>::value
#define __is_nothrow_destructible(...) std::is_nothrow_destructible<__VA_ARGS__>::value
#define __is_nothrow_assignable(...) std::is_nothrow_assignable<__VA_ARGS__>::value
#define __is_convertible_to(...) std::is_convertible<__VA_ARGS__>::value
#define __is_trivially_constructible(...) std::is_trivially_constructible<__VA_ARGS__>::value
#define __is_trivially_destructible(...) std::is_trivially_destructible<__VA_ARGS__>::value
#endif