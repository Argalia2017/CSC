#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"

namespace CSC {
inline namespace BASIC {
template <class _ARG1>
inline BOOL _MEMEQUAL_ (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src1 != NULL) ;
		_DEBUG_ASSERT_ (src2 != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	if (src1 == src2)
		return TRUE ;
	for (auto &&i : _RANGE_ (0 ,len))
		if (!(src1[i] == src2[i]))
			return FALSE ;
	return TRUE ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline FLAG _MEMCOMPR_ (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src1 != NULL) ;
		_DEBUG_ASSERT_ (src2 != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	if (src1 == src2)
		return FLAG (0) ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		if (src1[i] < src2[i])
			return FLAG (-1) ;
		if (src2[i] < src1[i])
			return FLAG (+1) ;
	}
	return FLAG (0) ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline FLAG _MEMHASH_ (const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
#ifdef __CSC_CONFIG_VAR32__
	static constexpr auto M_MAGIC_N1 = VAR (-2128831035) ;
	static constexpr auto M_MAGIC_N2 = VAR (16777619) ;
#elif defined __CSC_CONFIG_VAR64__
	static constexpr auto M_MAGIC_N1 = VAR (-3750763034362895579) ;
	static constexpr auto M_MAGIC_N2 = VAR (1099511628211) ;
#endif
	FLAG ret = M_MAGIC_N1 ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		ret ^= FLAG (src[i]) ;
		ret *= M_MAGIC_N2 ;
	}
	ret &= VAR_MAX ;
	return std::move (ret) ;
#pragma GCC diagnostic pop
}

inline CHAR _inline_MEMCRC32_TABLE_EACH_ (CHAR val) {
	CHAR ret = val ;
	for (auto &&i : _RANGE_ (0 ,8)) {
		const auto r1x = ret & CHAR (0X00000001) ;
		ret >>= 1 ;
		if (r1x == 0)
			continue ;
		ret ^= CHAR (0XEDB88320) ;
		(void) i ;
	}
	return std::move (ret) ;
}

inline const PACK<CHAR[256]> &_inline_MEMCRC32_TABLE_ () {
	return _CACHE_ ([] () {
		PACK<CHAR[256]> ret ;
		for (auto &&i : _RANGE_ (0 ,_COUNTOF_ (decltype (ret.P1))))
			ret.P1[i] = _inline_MEMCRC32_TABLE_EACH_ (CHAR (i)) ;
		return std::move (ret) ;
	}) ;
}

template <class _ARG1>
inline FLAG _MEMCRC32_ (const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_STATIC_ASSERT_ (std::is_same<_ARG1 ,BYTE>::value) ;
	FLAG ret = FLAG (0XFFFFFFFF) ;
	auto &r1y = _inline_MEMCRC32_TABLE_ () ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		const auto r2x = INDEX ((CHAR (ret) ^ CHAR (src[i])) & CHAR (0X000000FF)) ;
		ret = FLAG (r1y.P1[r2x] ^ (CHAR (ret) >> 8)) ;
	}
	ret &= VAR32_MAX ;
	return std::move (ret) ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline INDEX _MEMCHR_ (const ARR<_ARG1> &src ,LENGTH len ,const _ARG1 &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	for (auto &&i : _RANGE_ (0 ,len))
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline INDEX _MEMRCHR_ (const ARR<_ARG1> &src ,LENGTH len ,const _ARG1 &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	for (auto &&i : _RANGE_ (0 ,len))
		if (src[len + ~i] == val)
			return (len + ~i) ;
	return VAR_NONE ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline void _MEMCOPY_ (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (dst != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	if (dst == src)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
	for (auto &&i : _RANGE_ (0 ,len))
		dst[i] = src[i] ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline void _MEMRCOPY_ (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (dst != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;
	auto fax = TRUE ;
	if SWITCH_CASE (fax) {
		if (!(dst != src))
			discard ;
		_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst[i] = src[len + ~i] ;
	}
	if SWITCH_CASE (fax) {
		if (!(dst == src))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len / 2)) {
			const auto r1x = dst[i] ;
			dst[i] = dst[len + ~i] ;
			dst[len + ~i] = r1x ;
		}
	}
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline void _MEMMOVE_ (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (dst1 != NULL) ;
		_DEBUG_ASSERT_ (dst2 != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	if (dst1 == dst2)
		return ;
	auto fax = TRUE ;
	if SWITCH_CASE (fax) {
		if (!(dst1 < dst2))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst1[i] = std::move (dst2[i]) ;
	}
	if SWITCH_CASE (fax) {
		if (!(dst1 > dst2))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst1[len + ~i] = std::move (dst2[len + ~i]) ;
	}
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline void _MEMSWAP_ (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,LENGTH len) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (dst1 != NULL) ;
		_DEBUG_ASSERT_ (dst2 != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	if (dst1 == dst2)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst1 - dst2) >= len) ;
	for (auto &&i : _RANGE_ (0 ,len))
		_SWAP_ (dst1[i] ,dst2[i]) ;
#pragma GCC diagnostic pop
}

template <class _ARG1>
inline void _MEMFILL_ (ARR<_ARG1> &dst ,LENGTH len ,const _ARG1 &val) {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	if SWITCH_CASE (TRUE) {
		if (len == 0)
			discard ;
		_DEBUG_ASSERT_ (dst != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
	}
	for (auto &&i : _RANGE_ (0 ,len))
		dst[i] = val ;
#pragma GCC diagnostic pop
}
} ;

namespace U {
struct OPERATOR_COMPR {
	template <class _ARG1>
	inline static FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const ARGV<ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().compr (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ()))> ,FLAG>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		return lhs.compr (rhs) ;
	}

	template <class _ARG1>
	inline static FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const ARGV<ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().operator< (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ()))> ,FLAG>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		if (lhs < rhs)
			return FLAG (-1) ;
		if (rhs < lhs)
			return FLAG (+1) ;
		return FLAG (0) ;
	}

	template <class _ARG1>
	inline static FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const ARGV<ENABLE_TYPE<std::is_pod<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		return _MEMCOMPR_ (PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (lhs)] ,PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (rhs)] ,_SIZEOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline static FLAG invoke (const _ARG1 &lhs ,const _ARG1 &rhs) {
		return template_compr (lhs ,rhs ,ARGVPX ,ARGVP9) ;
	}
} ;
} ;

namespace U {
struct OPERATOR_HASH {
	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<std::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().hash ())> ,FLAG>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		return self_.hash () ;
	}

	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<std::is_integral<_ARG1>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		return FLAG (self_) ;
	}

	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<std::is_pod<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		return _MEMHASH_ (PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (self_)] ,_SIZEOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline static FLAG invoke (const _ARG1 &self_) {
		FLAG ret = template_hash (self_ ,ARGVPX ,ARGVP9) ;
		ret &= VAR_MAX ;
		return std::move (ret) ;
	}
} ;
} ;

class GlobalHeap final :private Wrapped<void> {
private:
	template <class UNIT>
	class OwnerProxy final {
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
			return _RET (_XVALUE_<const PTR<UNIT>> (mPointer)) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

		inline void operator= (const DEF<decltype (NULL)> &) & noexcept {
			mPointer = NULL ;
		}

		inline void operator= (const DEF<decltype (NULL)> &) && = delete ;

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
		auto &r2y = _LOAD_<_RET> (r1x) ;
		return OwnerProxy<_RET> (&r2y) ;
	}

	template <class _RET>
	inline static OwnerProxy<ARR<_RET>> alloc (LENGTH len) popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_RET) <= _ALIGNOF_ (stl::max_align_t)) ;
		_DEBUG_ASSERT_ (len > 0) ;
		const auto r1x = len * _SIZEOF_ (_RET) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		const auto r2x = operator new (r1x ,std::nothrow) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		auto &r3y = _LOAD_<ARR<_RET>> (r2x) ;
		return OwnerProxy<ARR<_RET>> (&r3y) ;
	}

	template <class _ARG1>
	inline static void free (const PTR<_ARG1> &address) noexcept {
		const auto r1x = _XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + _ADDRESS_ (address)) ;
		operator delete (r1x ,std::nothrow) ;
	}
} ;

template <class UNIT>
class ScopedGuard final {
private:
	PTR<UNIT> mAddress ;

public:
	inline ScopedGuard () = delete ;

	inline explicit ScopedGuard (UNIT &address) popping :ScopedGuard (ARGVP0) {
		address.lock () ;
		mAddress = &address ;
	}

	inline explicit ScopedGuard (UNIT &&) = delete ;

	inline ~ScopedGuard () noexcept {
		if (mAddress == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			mAddress->unlock () ;
		}) ;
		mAddress = NULL ;
	}

	inline ScopedGuard (const ScopedGuard &) = delete ;
	inline ScopedGuard &operator= (const ScopedGuard &) = delete ;
	inline ScopedGuard (ScopedGuard &&) = delete ;
	inline ScopedGuard &operator= (ScopedGuard &&) = delete ;

private:
	inline explicit ScopedGuard (const DEF<decltype (ARGVP0)> &) noexcept :mAddress (NULL) {}
} ;

template <class UNIT>
class ScopedBuild final {
private:
	PTR<const volatile PTR<TEMP<UNIT>>> mAddress ;
	LENGTH mSize ;

public:
	inline ScopedBuild () = delete ;

	template <class... _ARGS>
	inline explicit ScopedBuild (const volatile PTR<TEMP<UNIT>> &address ,_ARGS &&...initval) popping :ScopedBuild (ARGVP0) {
		mAddress = &address ;
		const auto r1x = _COPY_ (*mAddress) ;
		_CREATE_ (r1x ,std::forward<_ARGS> (initval)...) ;
		mSize++ ;
	}

	inline ~ScopedBuild () noexcept {
		if (mAddress == NULL)
			return ;
		const auto r1x = _COPY_ (*mAddress) ;
		if (r1x == NULL)
			return ;
		if SWITCH_CASE (TRUE) {
			if (mSize <= 0)
				discard ;
			_DESTROY_ (r1x) ;
			mSize-- ;
		}
		mAddress = NULL ;
	}

	inline ScopedBuild (const ScopedBuild &) = delete ;
	inline ScopedBuild &operator= (const ScopedBuild &) = delete ;
	inline ScopedBuild (ScopedBuild &&) = delete ;
	inline ScopedBuild &operator= (ScopedBuild &&) = delete ;

private:
	inline explicit ScopedBuild (const DEF<decltype (ARGVP0)> &) noexcept :mAddress (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class ScopedBuild<ARR<UNIT>> final {
private:
	PTR<const volatile PTR<ARR<TEMP<UNIT>>>> mAddress ;
	LENGTH mSize ;

public:
	inline ScopedBuild () = delete ;

	inline explicit ScopedBuild (const volatile PTR<ARR<TEMP<UNIT>>> &address ,LENGTH len) popping :ScopedBuild (ARGVP0) {
		mAddress = &address ;
		const auto r1x = _COPY_ (*mAddress) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mSize >= len)
				break ;
			_CREATE_ (&(*r1x)[mSize]) ;
			mSize++ ;
		}
	}

	inline explicit ScopedBuild (const volatile PTR<ARR<TEMP<UNIT>>> &address ,const ARR<UNIT> &src ,LENGTH len) popping :ScopedBuild (ARGVP0) {
		_DEBUG_ASSERT_ (src != NULL) ;
		mAddress = &address ;
		const auto r1x = _COPY_ (*mAddress) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mSize >= len)
				break ;
			_CREATE_ (&(*r1x)[mSize] ,src[mSize]) ;
			mSize++ ;
		}
	}

	inline ~ScopedBuild () noexcept {
		if (mAddress == NULL)
			return ;
		const auto r1x = _COPY_ (*mAddress) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			_DESTROY_ (&(*r1x)[mSize - 1]) ;
			mSize-- ;
		}
		mAddress = NULL ;
	}

	inline ScopedBuild (const ScopedBuild &) = delete ;
	inline ScopedBuild &operator= (const ScopedBuild &) = delete ;
	inline ScopedBuild (ScopedBuild &&) = delete ;
	inline ScopedBuild &operator= (ScopedBuild &&) = delete ;

private:
	inline explicit ScopedBuild (const DEF<decltype (ARGVP0)> &) noexcept :mAddress (NULL) ,mSize (0) {}
} ;

template <class>
class GlobalStatic ;

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
	inline Singleton () :Singleton (ARGVP0) {
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax) ;
		auto &r1y = _LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (rax)) ;
		mPointer = &r1y ;
		rax = NULL ;
	}

	inline ~Singleton () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline Singleton (const Singleton &) = delete ;
	inline Singleton &operator= (const Singleton &) = delete ;
	inline Singleton (Singleton &&) = delete ;
	inline Singleton &operator= (Singleton &&) = delete ;

	inline UNIT &to () {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		return mPointer->mData ;
	}

	inline implicit operator UNIT & () {
		return to () ;
	}

private:
	inline explicit Singleton (const DEF<decltype (ARGVP0)> &) noexcept :mPointer (NULL) {}

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
		inline explicit Holder (_ARGS &&...initval) :mData (std::forward<_ARGS> (initval)...) {}
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~AutoRef () ;
			new (this) AutoRef (std::move (that)) ;
		}
		return (*this) ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) noexcept :mPointer (pointer) {}
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
		inline explicit Holder (_ARGS &&...initval) :mData (std::forward<_ARGS> (initval)...) {}
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

	inline AutoRef (const AutoRef &that) :AutoRef () {
		if (that.mPointer == NULL)
			return ;
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax ,_XVALUE_<const UNIT> (that.mPointer->mData)) ;
		auto &r1y = _LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (rax)) ;
		mPointer = &r1y ;
		rax = NULL ;
	}

	inline AutoRef &operator= (const AutoRef &that) {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~AutoRef () ;
			new (this) AutoRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline AutoRef (AutoRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~AutoRef () ;
			new (this) AutoRef (std::move (that)) ;
		}
		return (*this) ;
	}

private:
	inline explicit AutoRef (PTR<Holder> pointer) noexcept :mPointer (pointer) {}
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
	inline explicit AutoRef (PTR<Holder> pointer) noexcept :SPECIALIZATION_BASE (pointer) {}

public:
	template <class... _ARGS>
	inline static AutoRef make (_ARGS &&...initval) {
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax ,std::forward<_ARGS> (initval)...) ;
		auto &r1y = _LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (rax)) ;
		AutoRef ret = AutoRef (&r1y) ;
		rax = NULL ;
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
		inline explicit Holder (_ARGS &&...initval) :mData (std::forward<_ARGS> (initval)...) ,mCounter (0) {}
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
		if SWITCH_CASE (TRUE) {
			const auto r1x = --mPointer->mCounter ;
			if (r1x != 0)
				discard ;
			mPointer->~Holder () ;
			GlobalHeap::free (mPointer) ;
		}
		mPointer = NULL ;
	}

	inline SharedRef (const SharedRef &that) :SharedRef (that.mPointer) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline SharedRef &operator= (const SharedRef &that) {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~SharedRef () ;
			new (this) SharedRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline SharedRef (SharedRef &&that) noexcept {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline SharedRef &operator= (SharedRef &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~SharedRef () ;
			new (this) SharedRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = static_cast<PTR<Holder>> (mPointer) ;
		return r1x->mData ;
	}

	inline implicit operator UNIT & () const {
		return to () ;
	}

	inline PTR<UNIT> operator-> () const {
		return &to () ;
	}

private:
	inline explicit SharedRef (PTR<Holder> pointer) :SharedRef () {
		if (pointer == NULL)
			return ;
		const auto r1x = ++pointer->mCounter ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		(void) r1x ;
		mPointer = pointer ;
	}

public:
	template <class... _ARGS>
	inline static SharedRef make (_ARGS &&...initval) {
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax ,std::forward<_ARGS> (initval)...) ;
		auto &r1y = _LOAD_<Holder> (_XVALUE_<PTR<TEMP<Holder>>> (rax)) ;
		SharedRef ret = SharedRef (&r1y) ;
		rax = NULL ;
		return std::move (ret) ;
	}
} ;

template <class>
class AnyRef ;

template <>
class AnyRef<void> {
private:
	exports struct Holder :public Interface {
		virtual FLAG typeuid () const = 0 ;
	} ;

private:
	template <class>
	friend class AnyRef ;
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~AnyRef () ;
			new (this) AnyRef (std::move (that)) ;
		}
		return (*this) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> ((*this)) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> ((*this)) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () && = delete ;

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG typeuid () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->typeuid () ;
	}
} ;

template <class UNIT>
class AnyRef {
private:
	using Holder = typename AnyRef<void>::Holder ;

	template <class UNIT_>
	class ImplHolder :public Holder {
	private:
		friend AnyRef ;
		UNIT_ mData ;

	public:
		template <class... _ARGS>
		inline explicit ImplHolder (_ARGS &&...initval) :mData (std::forward<_ARGS> (initval)...) {}

		inline FLAG typeuid () const override {
			return _TYPEUID_<UNIT_> () ;
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~AnyRef () ;
			new (this) AnyRef (std::move (that)) ;
		}
		return (*this) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> ((*this)) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const & {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> ((*this)) ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () && = delete ;

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG typeuid () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->typeuid () ;
	}

	inline UNIT &to () {
		_DEBUG_ASSERT_ (typeuid () == _TYPEUID_<UNIT> ()) ;
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
		_DEBUG_ASSERT_ (typeuid () == _TYPEUID_<UNIT> ()) ;
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
	inline explicit AnyRef (PTR<Holder> pointer) noexcept :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static AnyRef make (_ARGS &&...initval) {
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder<UNIT>>> () ;
		ScopedBuild<ImplHolder<UNIT>> ANONYMOUS (rax ,std::forward<_ARGS> (initval)...) ;
		auto &r1y = _LOAD_<ImplHolder<UNIT>> (_XVALUE_<PTR<TEMP<ImplHolder<UNIT>>>> (rax)) ;
		AnyRef ret = AnyRef (&r1y) ;
		rax = NULL ;
		return std::move (ret) ;
	}
} ;

template <class>
class UniqueRef ;

template <>
class UniqueRef<void> {
private:
	exports struct Holder :public Interface {
		virtual void release () = 0 ;
	} ;

	template <class UNIT_>
	class ImplHolder :public Holder {
	private:
		UNIT_ mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (const UNIT_ &functor) :mFunctor (std::move (functor)) {}

		inline explicit ImplHolder (UNIT_ &&functor) :mFunctor (std::move (functor)) {}

		inline void release () override {
			mFunctor () ;
		}
	} ;

private:
	template <class>
	friend class UniqueRef ;
	PTR<Holder> mPointer ;

public:
	inline UniqueRef () noexcept {
		mPointer = NULL ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (_ARG1 &&constructor ,_ARG2 &&destructor) popping :UniqueRef () {
		_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<REMOVE_REFERENCE_TYPE<_ARG2> ,PTR<void ()>>::value) ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>> () ;
		ScopedBuild<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> ANONYMOUS (rax ,std::forward<_ARG2> (destructor)) ;
		auto &r1y = _LOAD_<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> (_XVALUE_<PTR<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>>> (rax)) ;
		constructor () ;
		mPointer = &r1y ;
		rax = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~UniqueRef () ;
			new (this) UniqueRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}
} ;

template <class UNIT>
class UniqueRef {
private:
	using Holder = typename UniqueRef<void>::Holder ;

	template <class UNIT_>
	class ImplHolder :public Holder {
	private:
		friend UniqueRef ;
		REMOVE_CVR_TYPE<UNIT> mData ;
		UNIT_ mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (const UNIT_ &functor) :mFunctor (std::move (functor)) {}

		inline explicit ImplHolder (UNIT_ &&functor) :mFunctor (std::move (functor)) {}

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
	inline explicit UniqueRef (_ARG1 &&constructor ,_ARG2 &&destructor) popping :UniqueRef () {
		_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNIT &>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<UNIT &>> ,void>::value) ;
		_STATIC_ASSERT_ (std::is_convertible<REMOVE_REFERENCE_TYPE<_ARG2> ,PTR<void (UNIT &)>>::value) ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>> () ;
		ScopedBuild<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> ANONYMOUS (rax ,std::forward<_ARG2> (destructor)) ;
		auto &r1y = _LOAD_<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>> (_XVALUE_<PTR<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG2>>>>> (rax)) ;
		constructor (r1y.mData) ;
		mPointer = &r1y ;
		rax = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~UniqueRef () ;
			new (this) UniqueRef (std::move (that)) ;
		}
		return (*this) ;
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
	inline explicit UniqueRef (PTR<Holder> pointer) noexcept :mPointer (pointer) {}

public:
	template <class... _ARGS>
	inline static UniqueRef make (_ARGS &&...initval) {
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder<PTR<void (UNIT &)>>>> () ;
		const auto r1x = _XVALUE_<PTR<void (UNIT &)>> ([] (UNIT &) {}) ;
		ScopedBuild<ImplHolder<PTR<void (UNIT &)>>> ANONYMOUS (rax ,r1x) ;
		auto &r2y = _LOAD_<ImplHolder<PTR<void (UNIT &)>>> (_XVALUE_<PTR<TEMP<ImplHolder<PTR<void (UNIT &)>>>>> (rax)) ;
		r2y.mData = UNIT (std::forward<_ARGS> (initval)...) ;
		UniqueRef ret = UniqueRef (_XVALUE_<PTR<Holder>> (&r2y)) ;
		rax = NULL ;
		return std::move (ret) ;
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~PhanRef () ;
			new (this) PhanRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () const {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = static_cast<PTR<UNIT>> (mPointer) ;
		return (*r1x) ;
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
	inline static PhanRef make (UNIT &val) popping {
		return PhanRef (&val) ;
	}

	template <class _ARG1>
	inline static PhanRef make (const PhanRef<_ARG1> &val) {
		_STATIC_ASSERT_ (std::is_convertible<_ARG1 & ,UNIT &>::value) ;
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
	exports struct Holder :public Interface {
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping = 0 ;
	} ;

	template <class UNIT_>
	class ImplHolder :public Function<UNIT1 (UNITS...)>::Holder {
	private:
		UNIT_ mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (const UNIT_ &functor) :mFunctor (std::move (functor)) {}

		inline explicit ImplHolder (UNIT_ &&functor) :mFunctor (std::move (functor)) {}

		inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping override {
			return mFunctor (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
		}
	} ;

private:
	PTR<Holder> mFunction ;
	PTR<UNIT1 (UNITS...)> mCPPFunction ;

public:
	inline Function () noexcept {
		mFunction = NULL ;
		mCPPFunction = NULL ;
	}

	inline implicit Function (const PTR<UNIT1 (UNITS...)> &that) noexcept {
		mFunction = NULL ;
		mCPPFunction = that ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Function>::value && std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNITS...>> ,UNIT1>::value>>
	inline implicit Function (_ARG1 &&that) :Function () {
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>>> () ;
		ScopedBuild<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>> ANONYMOUS (rax ,std::forward<_ARG1> (that)) ;
		auto &r1y = _LOAD_<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>> (_XVALUE_<PTR<TEMP<ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>>>>> (rax)) ;
		mFunction = &r1y ;
		mCPPFunction = NULL ;
		rax = NULL ;
	}

	inline ~Function () noexcept {
		if (mFunction == NULL)
			if (mCPPFunction == NULL)
				return ;
		if SWITCH_CASE (TRUE) {
			if (mFunction == NULL)
				discard ;
			mFunction->~Holder () ;
			GlobalHeap::free (mFunction) ;
		}
		mFunction = NULL ;
		mCPPFunction = NULL ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&that) noexcept {
		mFunction = _EXCHANGE_ (that.mFunction) ;
		mCPPFunction = _EXCHANGE_ (that.mCPPFunction) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Function () ;
			new (this) Function (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		if (mFunction == NULL)
			if (mCPPFunction == NULL)
				return FALSE ;
		return TRUE ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mCPPFunction != NULL)
			return mCPPFunction (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
		return mFunction->invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

private:
	inline explicit Function (const DEF<decltype (ARGVP0)> & ,PTR<Holder> _function) :mFunction (_function) ,mCPPFunction (NULL) {}

public:
	//@info: this function is incompleted without 'csc_extend.hpp'
	template <class... _ARGS>
	inline static Function make (const PTR<UNIT1 (UNITS... ,_ARGS...)> &func ,const REMOVE_CVR_TYPE<_ARGS> &...parameter) ;
} ;

//@error: vs2017 is too useless to compile without hint
template <class UNIT1 ,class... UNITS>
using FIX_MSVC_DEDUCTION_2 = DEF<DEF<UNIT1 (UNITS...)> NONE::*> ;

template <class UNIT1 ,class... UNITS>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>> {
#pragma push_macro ("fake")
#undef fake
#define fake m_fake ()

private:
	class FakeHolder ;

	exports struct Holder :public Interface {
		virtual void friend_copy (PTR<TEMP<FakeHolder>> address) const noexcept = 0 ;
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping = 0 ;
	} ;

	class FakeHolder :public Holder {
	private:
		PTR<NONE> mContext ;
		DEF<DEF<UNIT1 (UNITS...)> NONE::*> mFunction ;

	public:
		inline FakeHolder () = delete ;

		inline void friend_copy (PTR<TEMP<FakeHolder>> address) const noexcept override ;
		inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping override ;
	} ;

	template <class>
	class ImplHolder ;

private:
	struct Detail ;
	TEMP<FakeHolder> mVariant ;

public:
	inline Function () noexcept {
		_ZERO_ (mVariant) ;
	}

	inline implicit Function (const PTR<UNIT1 (UNITS...)> &that) noexcept {
		using PureHolder = typename Detail::PureHolder ;
		_DEBUG_ASSERT_ (that != NULL) ;
		Detail::template static_create<PureHolder> (&mVariant ,that) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context_ ,const DEF<DEF<UNIT1 (UNITS...)> _ARG1::*> &func) noexcept {
		Detail::template static_create<ImplHolder<_ARG1>> (&mVariant ,&context_.self ,func) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<const _ARG1> &context_ ,const DEF<DEF<UNIT1 (UNITS...) const> _ARG1::*> &func) noexcept {
		Detail::template static_create<ImplHolder<const _ARG1>> (&mVariant ,&context_.self ,func) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context_ ,const PTR<UNIT1 (PTR<_ARG1> ,UNITS...)> &func) noexcept {
		_DEBUG_ASSERT_ (func != NULL) ;
		Detail::template static_create<ImplHolder<PTR<_ARG1>>> (&mVariant ,&context_.self ,func) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context_ ,const PTR<UNIT1 (PTR<const _ARG1> ,UNITS...)> &func) noexcept {
		_DEBUG_ASSERT_ (func != NULL) ;
		Detail::template static_create<ImplHolder<PTR<const _ARG1>>> (&mVariant ,&context_.self ,func) ;
	}

	inline ~Function () noexcept {
		if (!exist ())
			return ;
		fake.~Holder () ;
		_ZERO_ (mVariant) ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&that) noexcept :Function () {
		if (!that.exist ())
			return ;
		that.fake.friend_copy (&mVariant) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Function () ;
			new (this) Function (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		const auto r1x = _CAST_<FLAG> (_XVALUE_<Interface> (fake)) ;
		if (r1x == VAR_ZERO)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping {
		_DEBUG_ASSERT_ (exist ()) ;
		return fake.invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping {
		return invoke (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

private:
	inline Holder &m_fake () & {
		return _CAST_<FakeHolder> (mVariant) ;
	}

	inline const Holder &m_fake () const & {
		return _CAST_<FakeHolder> (mVariant) ;
	}

	inline Holder &m_fake () && = delete ;

private:
	struct Detail {
		class PureHolder :public Holder {
		private:
			PTR<UNIT1 (UNITS...)> mFunction ;

		public:
			inline PureHolder () = delete ;

			inline explicit PureHolder (const PTR<UNIT1 (UNITS...)> &func) noexcept :mFunction (func) {}

			inline void friend_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
				static_create<PureHolder> (address ,mFunction) ;
			}

			inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping override {
				return mFunction (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
			}
		} ;

		template <class _RET ,class... _ARGS>
		inline static void static_create (PTR<TEMP<FakeHolder>> address ,_ARGS &&...funcval) noexcept {
			_STATIC_ASSERT_ (std::is_nothrow_constructible<_RET ,_ARGS &&...>::value) ;
			auto &r1y = _LOAD_<TEMP<_RET>> (address) ;
			auto &r2y = _XVALUE_<Holder> (_CAST_<_RET> (r1y)) ;
			auto &r3y = _XVALUE_<Holder> (_CAST_<FakeHolder> ((*address))) ;
			_DYNAMIC_ASSERT_ (&r2y == &r3y) ;
			_CREATE_ (&r1y ,std::forward<_ARGS> (funcval)...) ;
		}
	} ;

#pragma pop_macro ("fake")
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>>::ImplHolder :public Holder {
private:
	PTR<UNIT_> mContext ;
	DEF<DEF<UNIT1 (UNITS...)> UNIT_::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<UNIT_> context_ ,const DEF<DEF<UNIT1 (UNITS...)> UNIT_::*> &func) noexcept :mContext (context_) ,mFunction (func) {}

	inline void friend_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
		Detail::template static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping override {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>>::ImplHolder<const UNIT_> :public Holder {
private:
	PTR<const UNIT_> mContext ;
	DEF<DEF<UNIT1 (UNITS...) const> UNIT_::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<const UNIT_> context_ ,const DEF<DEF<UNIT1 (UNITS...) const> UNIT_::*> &func) noexcept :mContext (context_) ,mFunction (func) {}

	inline void friend_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
		Detail::template static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping override {
		return (mContext->*mFunction) (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<FIX_MSVC_DEDUCTION_2<UNIT1 ,UNITS...>>::ImplHolder<PTR<UNIT_>> :public Holder {
private:
	PTR<UNIT_> mContext ;
	PTR<UNIT1 (PTR<UNIT_> ,UNITS...)> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<UNIT_> context_ ,const PTR<UNIT1 (PTR<UNIT_> ,UNITS...)> &func) noexcept :mContext (context_) ,mFunction (func) {}

	inline void friend_copy (PTR<TEMP<FakeHolder>> address) const noexcept override {
		Detail::template static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const popping override {
		return mFunction (mContext ,std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
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
		_DEBUG_ASSERT_ (len >= 0 && len <= SIZE) ;
	}

	inline implicit Buffer (const DEF<UNIT[SIZE]> &that) :Buffer (std::move (_CAST_<Buffer> (that))) {}

	inline implicit Buffer (DEF<UNIT[SIZE]> &&that) : Buffer (std::move (_CAST_<Buffer> (that))) {}

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
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - mBuffer ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		return _MEMEQUAL_ (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE) ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		return _MEMCOMPR_ (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE) ;
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

	inline LENGTH expand_size () const {
		return 0 ;
	}

	inline Buffer expand (LENGTH len) const {
		_STATIC_WARNING_ ("unexpected") ;
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) popping {
		_MEMSWAP_ (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE) ;
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

	inline explicit Buffer (LENGTH len) :Buffer () {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax) ,len) ;
		auto &r1y = _LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax)) ;
		mBuffer = &r1y ;
		mSize = len ;
		rax = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			(*mBuffer)[i].~UNIT () ;
			(void) i ;
		}
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;
	inline Buffer (Buffer &&) = delete ;
	inline Buffer &operator= (Buffer &&) = delete ;

	inline ARR<UNIT> &to () {
		return (*mBuffer) ;
	}

	inline implicit operator ARR<UNIT> & () {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const {
		return (*mBuffer) ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (INDEX index) & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - (*mBuffer) ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ ((*mBuffer) ,(*that.mBuffer) ,that.mSize))
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
		const auto r2x = _MEMCOMPR_ ((*mBuffer) ,(*that.mBuffer) ,r1x) ;
		if (r2x != 0)
			return r2x ;
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

	inline LENGTH expand_size () const {
		return 0 ;
	}

	inline Buffer<UNIT ,SAUTO> expand (LENGTH len) const {
		_STATIC_WARNING_ ("unexpected") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Buffer<UNIT ,SAUTO> () ;
	}

	inline void swap (Buffer<UNIT ,SAUTO> &that) popping {
		_STATIC_WARNING_ ("unexpected") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	inline void swap (Buffer &that) popping {
		_DYNAMIC_ASSERT_ (mSize == that.mSize) ;
		_MEMSWAP_ ((*mBuffer) ,(*that.mBuffer) ,mSize) ;
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

	inline explicit Buffer (LENGTH len) :Buffer () {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax) ,len) ;
		auto &r1y = _LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax)) ;
		mBuffer = &r1y ;
		mSize = len ;
		rax = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			(*mBuffer)[i].~UNIT () ;
			(void) i ;
		}
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return (*this) ;
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

	inline explicit Buffer (LENGTH len) :Buffer () {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax) ,len) ;
		auto &r1y = _LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax)) ;
		mBuffer = &r1y ;
		mSize = len ;
		rax = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			(*mBuffer)[i].~UNIT () ;
			(void) i ;
		}
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &that) :Buffer () {
		if (that.mBuffer == NULL)
			return ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (that.mSize) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax) ,(*that.mBuffer) ,that.mSize) ;
		auto &r1y = _LOAD_<ARR<UNIT>> (_XVALUE_<PTR<ARR<TEMP<UNIT>>>> (rax)) ;
		mBuffer = &r1y ;
		mSize = that.mSize ;
		rax = NULL ;
	}

	inline Buffer &operator= (const Buffer &that) {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return (*this) ;
	}

	inline Buffer (Buffer &&that) noexcept {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return (*this) ;
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
		return (*mBuffer) ;
	}

	inline implicit operator ARR<UNIT> & () {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const {
		return (*mBuffer) ;
	}

	inline implicit operator const ARR<UNIT> & () const {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (INDEX index) & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline UNIT &operator[] (INDEX index) & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX index) const & {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return (*mBuffer)[index] ;
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - (*mBuffer) ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ ((*mBuffer) ,(*that.mBuffer) ,that.mSize))
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
		const auto r2x = _MEMCOMPR_ ((*mBuffer) ,(*that.mBuffer) ,r1x) ;
		if (r2x != 0)
			return r2x ;
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

	inline LENGTH expand_size () const {
		using DEFAULT_EXPANDFIRST_SIZE = ARGC<256> ;
		const auto r1x = LENGTH (mSize * MATH_SQRT2) ;
		const auto r2x = mSize + DEFAULT_EXPANDFIRST_SIZE::value ;
		return _MAX_ (r1x ,r2x) ;
	}

	inline Buffer expand (LENGTH len) const {
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

	inline explicit Buffer (LENGTH len) :Buffer () {
		_DEBUG_ASSERT_ (len == 0) ;
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return (*this) ;
	}

	inline const ARR<UNIT> &to () const {
		if (mBuffer == NULL)
			return (*mBuffer) ;
		const auto r1x = static_cast<PTR<const ARR<UNIT>>> (mBuffer) ;
		return (*r1x) ;
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
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		const auto r1x = static_cast<PTR<const ARR<UNIT>>> (mBuffer) ;
		return (*r1x)[index] ;
#pragma GCC diagnostic pop
	}

	inline const UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline const UNIT &get (INDEX) && = delete ;

	inline const UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - (*mBuffer) ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ ((*mBuffer) ,(*that.mBuffer) ,that.mSize))
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
		const auto r2x = _MEMCOMPR_ ((*mBuffer) ,(*that.mBuffer) ,r1x) ;
		if (r2x != 0)
			return r2x ;
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

	inline LENGTH expand_size () const {
		return 0 ;
	}

	inline Buffer expand (LENGTH len) const {
		_STATIC_WARNING_ ("unexpected") ;
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) popping {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

private:
	inline explicit Buffer (PTR<const ARR<UNIT>> src ,LENGTH len) noexcept :mBuffer (src) ,mSize (len) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static Buffer make (const ARR<UNIT> &src ,LENGTH len) popping {
		if (len == 0)
			return Buffer () ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
		return Buffer (&src ,len) ;
	}

	template <LENGTH _VAL1>
	inline static Buffer make (const DEF<UNIT[_VAL1]> &val) popping {
		return make (PTRTOARR[val] ,_VAL1) ;
	}

	template <class _ARG1>
	inline static Buffer make (const Buffer<UNIT ,_ARG1> &val) {
		return make (val ,val.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<std::is_same<UNIT ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (const Buffer<_ARG1 ,_ARG2> &val) {
		if (val.size () == 0)
			return Buffer () ;
		auto &r1y = _LOAD_<ARR<BYTE>> (&val.self) ;
		return make (r1y ,(val.size () * _SIZEOF_ (_ARG1))) ;
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

	inline explicit Buffer (LENGTH len) :Buffer () {
		_DEBUG_ASSERT_ (len == 0) ;
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
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Buffer () ;
			new (this) Buffer (std::move (that)) ;
		}
		return (*this) ;
	}

	inline ARR<UNIT> &to () const {
		if (mBuffer == NULL)
			return (*mBuffer) ;
		const auto r1x = static_cast<PTR<ARR<UNIT>>> (mBuffer) ;
		return (*r1x) ;
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
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		const auto r1x = static_cast<PTR<ARR<UNIT>>> (mBuffer) ;
		return (*r1x)[index] ;
#pragma GCC diagnostic pop
	}

	inline UNIT &operator[] (INDEX index) const & {
		return get (index) ;
	}

	inline UNIT &get (INDEX) && = delete ;

	inline UNIT &operator[] (INDEX) && = delete ;

	inline INDEX at (const UNIT &item) const {
		INDEX ret = &item - (*mBuffer) ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!_MEMEQUAL_ ((*mBuffer) ,(*that.mBuffer) ,that.mSize))
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
		const auto r2x = _MEMCOMPR_ ((*mBuffer) ,(*that.mBuffer) ,r1x) ;
		if (r2x != 0)
			return r2x ;
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

	inline LENGTH expand_size () const {
		return 0 ;
	}

	inline Buffer expand (LENGTH len) const {
		_STATIC_WARNING_ ("unexpected") ;
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) popping {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

private:
	inline explicit Buffer (PTR<ARR<UNIT>> src ,LENGTH len) noexcept :mBuffer (src) ,mSize (len) {}

public:
	//@warn: phantom means deliver pointer without holder
	inline static Buffer make (ARR<UNIT> &src ,LENGTH len) popping {
		if (len == 0)
			return Buffer () ;
		_DEBUG_ASSERT_ (src != NULL) ;
		_DEBUG_ASSERT_ (len > 0) ;
		return Buffer (&src ,len) ;
	}

	template <LENGTH _VAL1>
	inline static Buffer make (DEF<UNIT[_VAL1]> &val) popping {
		return make (PTRTOARR[val] ,_VAL1) ;
	}

	template <class _ARG1>
	inline static Buffer make (Buffer<UNIT ,_ARG1> &val) popping {
		return make (val.self ,val.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<std::is_same<UNIT ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (Buffer<_ARG1 ,_ARG2> &val) popping {
		if (val.size () == 0)
			return Buffer () ;
		auto &r1y = _LOAD_<ARR<BYTE>> (&val.self) ;
		return make (r1y ,(val.size () * _SIZEOF_ (_ARG1))) ;
	}

	inline static Buffer make (const Buffer<UNIT ,SMPHAN> &val) {
		return make (val.self ,val.size ()) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<UNIT ,BYTE>::value && !std::is_same<_ARG1 ,BYTE>::value && U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value>>
	inline static Buffer make (const Buffer<_ARG1 ,SMPHAN> &val) {
		if (val.size () == 0)
			return Buffer () ;
		auto &r1y = _LOAD_<ARR<BYTE>> (&val.self) ;
		return make (r1y ,(val.size () * _SIZEOF_ (_ARG1))) ;
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
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () :Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline explicit Allocator (LENGTH len) :Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline ~Allocator () noexcept {
		if (mSize == 0)
			return ;
		spec.clear () ;
		mSize = 0 ;
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;
	inline Allocator (Allocator &&) = delete ;
	inline Allocator &operator= (Allocator &&) = delete ;

private:
	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
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
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () :Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline explicit Allocator (LENGTH len) :Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline ~Allocator () noexcept {
		if (mSize == 0)
			return ;
		spec.clear () ;
		mSize = 0 ;
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;

	inline Allocator (Allocator &&that) noexcept :Allocator (ARGVP0 ,std::move (that.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
		const auto r1x = _SWITCH_ (
			(std::is_pod<UNIT>::value) ? mAllocator.size () :
			0) ;
		mSize = r1x ;
		while (TRUE) {
			if (mSize >= that.mAllocator.size ())
				break ;
			if SWITCH_CASE (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (&mAllocator[ix].mData ,std::move (_CAST_<UNIT> (that.mAllocator[ix].mData))) ;
			}
			mSize++ ;
		}
		mSize = mAllocator.size () ;
		mLength = that.mLength ;
		mFree = that.mFree ;
		that.spec.clear () ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Allocator () ;
			new (this) Allocator (std::move (that)) ;
		}
		return (*this) ;
	}

private:
	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,Buffer<Node ,SIZE> &&allocator_) :mAllocator (std::move (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
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

	class Finally :private Wrapped<Allocator> {
	public:
		inline void lock () {
			const auto r1x = _SWITCH_ (
				(std::is_pod<UNIT>::value) ? Finally::mSelf.mAllocator.size () :
				0) ;
			Finally::mSelf.mSize = r1x ;
		}

		inline void unlock () {
			if (Finally::mSelf.mSize == Finally::mSelf.mAllocator.size ())
				return ;
			while (TRUE) {
				if (Finally::mSelf.mSize <= 0)
					break ;
				if SWITCH_CASE (TRUE) {
					INDEX ix = Finally::mSelf.mSize - 1 ;
					if (Finally::mSelf.mAllocator[ix].mNext != VAR_USED)
						discard ;
					_DESTROY_ (&Finally::mSelf.mAllocator[ix].mData) ;
					Finally::mSelf.mSize-- ;
				}
			}
		}
	} ;

private:
	friend SPECIALIZATION_TYPE ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator () :Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline explicit Allocator (LENGTH len) :Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline ~Allocator () noexcept {
		if (mSize == 0)
			return ;
		spec.clear () ;
		mSize = 0 ;
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	inline Allocator (const Allocator &that) :Allocator (ARGVP0 ,std::move (that.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
		ScopedGuard<Finally> ANONYMOUS (_CAST_<Finally> ((*this))) ;
		while (TRUE) {
			if (mSize >= that.mAllocator.size ())
				break ;
			if SWITCH_CASE (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (&mAllocator[ix].mData ,std::move (_CAST_<UNIT> (that.mAllocator[ix].mData))) ;
			}
			mSize++ ;
		}
		mSize = mAllocator.size () ;
		mLength = that.mLength ;
		mFree = that.mFree ;
	}

	inline Allocator &operator= (const Allocator &that) {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Allocator () ;
			new (this) Allocator (std::move (that)) ;
		}
		return (*this) ;
	}

	inline Allocator (Allocator &&that) noexcept :Allocator (ARGVP0 ,std::move (that.mAllocator)) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
		const auto r1x = _SWITCH_ (
			(std::is_pod<UNIT>::value) ? mAllocator.size () :
			0) ;
		mSize = r1x ;
		while (TRUE) {
			if (mSize >= that.mAllocator.size ())
				break ;
			if SWITCH_CASE (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (&mAllocator[ix].mData ,std::move (_CAST_<UNIT> (that.mAllocator[ix].mData))) ;
			}
			mSize++ ;
		}
		mSize = mAllocator.size () ;
		mLength = that.mLength ;
		mFree = that.mFree ;
		that.spec.clear () ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		if SWITCH_CASE (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Allocator () ;
			new (this) Allocator (std::move (that)) ;
		}
		return (*this) ;
	}

private:
	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,LENGTH len) :mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,const Buffer<Node ,SIZE> &allocator_) :mAllocator (std::move (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,Buffer<Node ,SIZE> &&allocator_) :mAllocator (std::move (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

private:
	inline SPECIALIZATION_TYPE &m_spec () & {
		return (*static_cast<PTR<SPECIALIZATION_TYPE>> (this)) ;
	}

	inline const SPECIALIZATION_TYPE &m_spec () const & {
		return (*static_cast<PTR<const SPECIALIZATION_TYPE>> (this)) ;
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
	using SPECIALIZATION_BASE::mSize ;
	using SPECIALIZATION_BASE::mLength ;
	using SPECIALIZATION_BASE::mFree ;

public:
	inline Allocator () = default ;

	inline explicit Allocator (LENGTH len) :SPECIALIZATION_BASE (len) {}

	inline LENGTH size () const {
		return mSize ;
	}

	inline LENGTH length () const {
		if (mAllocator.size () == 0)
			return 0 ;
		return mLength ;
	}

	inline void clear () noexcept {
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		for (auto &&i : _RANGE_ (0 ,mAllocator.size ())) {
			iy = ix ;
			ix = mAllocator.size () + ~i ;
			if (mAllocator[ix].mNext == VAR_USED)
				_DESTROY_ (&mAllocator[ix].mData) ;
			mAllocator[ix].mNext = iy ;
		}
		mSize = mAllocator.size () ;
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
		auto &r1y = _OFFSET_ (&Node::mData ,_CAST_<TEMP<UNIT>> (item)) ;
		INDEX ret = mAllocator.at (r1y) ;
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
	inline INDEX alloc (_ARGS &&...initval) popping {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
		_DEBUG_ASSERT_ (mSize == mAllocator.size ()) ;
		if SWITCH_CASE (TRUE) {
			if (mFree != VAR_NONE)
				discard ;
			auto tmp = mAllocator.expand (mAllocator.expand_size ()) ;
			const auto r1x = mSize ;
			_CREATE_ (&tmp[r1x].mData ,std::forward<_ARGS> (initval)...) ;
			for (auto &&i : _RANGE_ (0 ,mSize)) {
				_CREATE_ (&tmp[i].mData ,std::move (_CAST_<UNIT> (mAllocator[i].mData))) ;
				tmp[i].mNext = VAR_USED ;
			}
			mAllocator.swap (tmp) ;
			update_reserve (mSize ,mFree) ;
			mFree = mAllocator[r1x].mNext ;
			mAllocator[r1x].mNext = VAR_USED ;
			mLength++ ;
			return r1x ;
		}
		if SWITCH_CASE (TRUE) {
			if (mFree == VAR_NONE)
				discard ;
			const auto r2x = mFree ;
			_CREATE_ (&mAllocator[r2x].mData ,std::forward<_ARGS> (initval)...) ;
			mFree = mAllocator[r2x].mNext ;
			mAllocator[r2x].mNext = VAR_USED ;
			mLength++ ;
			return r2x ;
		}
		_STATIC_WARNING_ ("unexpected") ;
		_DEBUG_ASSERT_ (FALSE) ;
		return VAR_NONE ;
	}

	inline void free (INDEX index) noexcept {
		_DEBUG_ASSERT_ (mSize == mAllocator.size ()) ;
		_DEBUG_ASSERT_ (used (index)) ;
		_DESTROY_ (&mAllocator[index].mData) ;
		mAllocator[index].mNext = mFree ;
		mFree = index ;
		mLength-- ;
	}

	inline void reserve (LENGTH len) {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
		_DEBUG_ASSERT_ (mSize == mAllocator.size ()) ;
		_DEBUG_ASSERT_ (len >= 0) ;
		const auto r1x = _MAX_ (len - (mSize - mLength) ,VAR_ZERO) ;
		if (r1x == 0)
			return ;
		_DEBUG_ASSERT_ (mSize + r1x > mSize) ;
		auto tmp = mAllocator.expand (mSize + r1x) ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (&tmp[i].mData ,std::move (_CAST_<UNIT> (mAllocator[i].mData))) ;
			tmp[i].mNext = mAllocator[i].mNext ;
		}
		mAllocator.swap (tmp) ;
		update_reserve (mSize ,mFree) ;
	}

	inline void clean () {
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<UNIT>::value) ;
		_DEBUG_ASSERT_ (mSize == mAllocator.size ()) ;
		const auto r1x = shrink_size () ;
		if (r1x == mSize)
			return ;
		_DYNAMIC_ASSERT_ (r1x == mLength) ;
		auto tmp = mAllocator.expand (r1x) ;
		for (auto &&i : _RANGE_ (0 ,tmp.size ())) {
			_DEBUG_ASSERT_ (mAllocator[i].mNext == VAR_USED) ;
			_CREATE_ (&tmp[i].mData ,std::move (_CAST_<UNIT> (mAllocator[i].mData))) ;
			tmp[i].mNext = VAR_USED ;
		}
		mAllocator.swap (tmp) ;
		update_reserve (r1x ,VAR_NONE) ;
	}

private:
	inline void update_reserve (INDEX size_ ,INDEX free_) {
		INDEX ix = free_ ;
		INDEX iy = VAR_NONE ;
		for (auto &&i : _RANGE_ (size_ ,mAllocator.size ())) {
			iy = ix ;
			ix = mAllocator.size () + ~(i - size_) ;
			mAllocator[ix].mNext = iy ;
		}
		mSize = mAllocator.size () ;
		mFree = ix ;
	}

	inline LENGTH shrink_size () const {
		LENGTH ret = mSize ;
		while (TRUE) {
			if (ret < 1)
				break ;
			if (mAllocator[ret - 1].mNext == VAR_USED)
				break ;
			ret-- ;
		}
		return std::move (ret) ;
	}
} ;
} ;