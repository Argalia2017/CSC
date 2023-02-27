
#ifndef __CSC_CXX_LITE__
#error
#endif

#ifdef __CSC_COMPILER_GNUC__
//@fatal: fuck gnuc
#include <type_traits>
#define __is_constructible(...) std::is_constructible<__VA_ARGS__>::value
#define __is_nothrow_constructible(...) std::is_nothrow_constructible<__VA_ARGS__>::value
#define __is_nothrow_destructible(...) std::is_nothrow_destructible<__VA_ARGS__>::value
#define __is_assignable(...) std::is_assignable<__VA_ARGS__>::value
#define __is_convertible_to(...) std::is_convertible<__VA_ARGS__>::value
#define __is_trivially_constructible __has_trivial_constructor
#define __is_trivially_destructible __has_trivial_destructor
#endif

#ifndef _INITIALIZER_LIST_
#ifndef _INITIALIZER_LIST
//@fatal: fuck initializer_list
#define _INITIALIZER_LIST_
#define _INITIALIZER_LIST

#ifdef __CSC_COMPILER_MSVC__
namespace std {
template <class UNIT>
class initializer_list {
private:
	using iterator_t = const UNIT * ;

protected:
	iterator_t mBegin ;
	iterator_t mEnd ;

public:
	constexpr initializer_list () noexcept
		:mBegin (0) ,mEnd (0) {}

	constexpr initializer_list (iterator_t begin_ ,iterator_t end_) noexcept
		:mBegin (begin_) ,mEnd (end_) {}

	constexpr iterator_t begin () const noexcept {
		return mBegin ;
	}

	constexpr iterator_t end () const noexcept {
		return mEnd ;
	}

	constexpr size_t size () const noexcept {
		return size_t (mEnd - mBegin) ;
	}
} ;
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
namespace std {
template <class UNIT>
class initializer_list {
private:
	using iterator_t = const UNIT * ;

protected:
	iterator_t mBegin ;
	size_t mSize ;

public:
	constexpr initializer_list () noexcept
		:mBegin (0) ,mSize (0) {}

	constexpr initializer_list (iterator_t begin_ ,size_t size_) noexcept
		:mBegin (begin_) ,mSize (size_) {}

	constexpr iterator_t begin () const noexcept {
		return mBegin ;
	}

	constexpr iterator_t end () const noexcept {
		return mBegin + mSize ;
	}

	constexpr size_t size () const noexcept {
		return mSize ;
	}
} ;
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
namespace std {
template <class UNIT>
class initializer_list {
private:
	using iterator_t = const UNIT * ;

protected:
	iterator_t mBegin ;
	iterator_t mEnd ;

public:
	constexpr initializer_list () noexcept
		:mBegin (0) ,mEnd (0) {}

	constexpr initializer_list (iterator_t begin_ ,iterator_t end_) noexcept
		:mBegin (begin_) ,mEnd (end_) {}

	constexpr iterator_t begin () const noexcept {
		return mBegin ;
	}

	constexpr iterator_t end () const noexcept {
		return mEnd ;
	}

	constexpr size_t size () const noexcept {
		return size_t (mEnd - mBegin) ;
	}
} ;
} ;
#endif
#endif
#endif