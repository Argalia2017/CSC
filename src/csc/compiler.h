

#ifdef __CSC_CXX_LITE__
namespace std {
template <class>
class initializer_list ;
} ;

#ifdef __CSC_COMPILER_GNUC__
#include <type_traits>

#define __is_constructible(...) std::is_constructible<__VA_ARGS__>::value
#define __is_assignable(...) std::is_assignable<__VA_ARGS__>::value
#define __is_nothrow_constructible(...) std::is_nothrow_constructible<__VA_ARGS__>::value
#define __is_nothrow_destructible(...) std::is_nothrow_destructible<__VA_ARGS__>::value
#define __is_nothrow_assignable(...) std::is_nothrow_assignable<__VA_ARGS__>::value
#define __is_convertible_to(...) std::is_convertible<__VA_ARGS__>::value
#define __is_trivially_constructible __has_trivial_constructor
#define __is_trivially_destructible __has_trivial_destructor
#endif
#endif

#ifdef __CSC_COMPILER_GNUC__
#if __GLIBCXX__ <= 20140522L
namespace std {
template <class T>
struct is_trivially_constructible :integral_constant<bool ,__has_trivial_constructor (T)> {} ;
} ;
#endif
#endif

#ifdef __CSC_COMPILER_GNUC__
using size_t = __SIZE_TYPE__ ;
using ptrdiff_t = __PTRDIFF_TYPE__ ;
#endif

namespace CSC {
template <class ,class>
struct TEMPID ;
} ;

template <class ARG1 ,class ARG2>
inline void * operator new (size_t ,CSC::TEMPID<ARG1 ,ARG2> * where_) noexcept {
	return where_ ;
}

template <class ARG1 ,class ARG2>
inline void operator delete (void * ,CSC::TEMPID<ARG1 ,ARG2> *) noexcept {
	return ;
}