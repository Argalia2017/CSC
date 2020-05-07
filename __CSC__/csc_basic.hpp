#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
class BasicProc
	:private Wrapped<void> {
public:
	template <class _ARG1>
	inline imports_static BOOL mem_equal (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static FLAG mem_compr (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static FLAG mem_hash (const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static FLAG mem_crc32 (const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static INDEX mem_chr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) ;

	template <class _ARG1>
	inline imports_static INDEX mem_rchr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) ;

	template <class _ARG1>
	inline imports_static void mem_copy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static void mem_rcopy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static void mem_move (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static void mem_swap (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) ;

	template <class _ARG1>
	inline imports_static void mem_fill (ARR<_ARG1> &dst ,const LENGTH &len ,const _ARG1 &val) ;
} ;

template <class _ARG1>
inline BOOL BasicProc::mem_equal (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (src1 == src2)
		return TRUE ;
	for (auto &&i : _RANGE_ (0 ,len))
		if (!(src1[i] == src2[i]))
			return FALSE ;
	return TRUE ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

namespace U {
struct OPERATOR_COMPR {
	template <class _ARG1>
	inline static FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const ARGV<ENABLE_TYPE<stl::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().compr (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ()))> ,FLAG>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		return lhs.compr (rhs) ;
	}

	template <class _ARG1>
	inline static FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const ARGV<ENABLE_TYPE<stl::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> () < _NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ())> ,BOOL>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		if (lhs < rhs)
			return FLAG (-1) ;
		if (rhs < lhs)
			return FLAG (+1) ;
		return FLAG (0) ;
	}

	template <class _ARG1>
	inline static FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const ARGV<ENABLE_TYPE<stl::is_pod<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		return BasicProc::mem_compr (PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (lhs)] ,PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (rhs)] ,_SIZEOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline static FLAG invoke (const _ARG1 &lhs ,const _ARG1 &rhs) {
		return template_compr (lhs ,rhs ,ARGVPX ,ARGVP9) ;
	}
} ;
} ;

template <class _ARG1>
inline FLAG BasicProc::mem_compr (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (src1 == src2)
		return FLAG (0) ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		const auto r1x = U::OPERATOR_COMPR::invoke (src1[i] ,src2[i]) ;
		if (r1x != 0)
			return r1x ;
	}
	return FLAG (0) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

namespace U {
struct OPERATOR_HASH {
	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<stl::is_same<DEF<decltype (_NULL_<const REMOVE_REFERENCE_TYPE<_ARG1>> ().hash ())> ,FLAG>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		return self_.hash () ;
	}

	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<stl::is_integral<_ARG1>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		return FLAG (self_) ;
	}

	template <class _ARG1>
	inline static FLAG template_hash (const _ARG1 &self_ ,const ARGV<ENABLE_TYPE<stl::is_pod<_ARG1>::value>> & ,const DEF<decltype (ARGVP1)> &) {
		return BasicProc::mem_hash (PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (self_)] ,_SIZEOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline static FLAG invoke (const _ARG1 &self_) {
		FLAG ret = template_hash (self_ ,ARGVPX ,ARGVP9) ;
		ret &= VAR_MAX ;
		return _MOVE_ (ret) ;
	}
} ;
} ;

#ifdef __CSC_CONFIG_VAR32__
template <class _ARG1>
inline FLAG BasicProc::mem_hash (const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	static constexpr auto M_MAGIC_N1 = VAR (-2128831035) ;
	static constexpr auto M_MAGIC_N2 = VAR (16777619) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	FLAG ret = M_MAGIC_N1 ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		ret ^= U::OPERATOR_HASH::invoke (src[i]) ;
		ret *= M_MAGIC_N2 ;
	}
	ret &= VAR_MAX ;
	return _MOVE_ (ret) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}
#endif

#ifdef __CSC_CONFIG_VAR64__
template <class _ARG1>
inline FLAG BasicProc::mem_hash (const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	static constexpr auto M_MAGIC_N1 = VAR (-3750763034362895579) ;
	static constexpr auto M_MAGIC_N2 = VAR (1099511628211) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	FLAG ret = M_MAGIC_N1 ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		ret ^= U::OPERATOR_HASH::invoke (src[i]) ;
		ret *= M_MAGIC_N2 ;
	}
	ret &= VAR_MAX ;
	return _MOVE_ (ret) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}
#endif

namespace U {
inline CHAR static_mem_crc32_table_each (const CHAR &val) {
	CHAR ret = val ;
	for (auto &&i : _RANGE_ (0 ,8)) {
		const auto r1x = CHAR (ret & CHAR (0X00000001)) ;
		ret >>= 1 ;
		if (r1x == 0)
			continue ;
		ret ^= CHAR (0XEDB88320) ;
		_STATIC_UNUSED_ (i) ;
	}
	return _MOVE_ (ret) ;
}

inline const PACK<CHAR[256]> &static_mem_crc32_table () {
	return _CACHE_ ([&] () {
		PACK<CHAR[256]> ret ;
		for (auto &&i : _RANGE_ (0 ,_COUNTOF_ (decltype (ret.P1))))
			ret.P1[i] = static_mem_crc32_table_each (CHAR (i)) ;
		return _MOVE_ (ret) ;
	}) ;
}
} ;

template <class _ARG1>
inline FLAG BasicProc::mem_crc32 (const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_STATIC_ASSERT_ (stl::is_same<_ARG1 ,BYTE>::value) ;
	FLAG ret = FLAG (0XFFFFFFFF) ;
	auto &r1x = U::static_mem_crc32_table () ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		const auto r2x = CHAR ((CHAR (ret) ^ CHAR (src[i])) & CHAR (0X000000FF)) ;
		ret = FLAG (r1x.P1[INDEX (r2x)] ^ (CHAR (ret) >> 8)) ;
	}
	ret &= VAR32_MAX ;
	return _MOVE_ (ret) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline INDEX BasicProc::mem_chr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	for (auto &&i : _RANGE_ (0 ,len))
		if (src[i] == val)
			return i ;
	return VAR_NONE ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline INDEX BasicProc::mem_rchr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	for (auto &&i : _RANGE_ (0 ,len))
		if (src[len + ~i] == val)
			return (len + ~i) ;
	return VAR_NONE ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline void BasicProc::mem_copy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst == src)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
	for (auto &&i : _RANGE_ (0 ,len))
		dst[i] = src[i] ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline void BasicProc::mem_rcopy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	auto fax = TRUE ;
	if switch_case (fax) {
		if (!(dst != src))
			discard ;
		_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst[i] = src[len + ~i] ;
	}
	if switch_case (fax) {
		if (!(dst == src))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len / 2)) {
			const auto r1x = dst[i] ;
			dst[i] = dst[len + ~i] ;
			dst[len + ~i] = r1x ;
		}
	}
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline void BasicProc::mem_move (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst1 == dst2)
		return ;
	auto fax = TRUE ;
	if switch_case (fax) {
		if (!(dst1 < dst2))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst1[i] = _MOVE_ (dst2[i]) ;
	}
	if switch_case (fax) {
		if (!(dst1 > dst2))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst1[len + ~i] = _MOVE_ (dst2[len + ~i]) ;
	}
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline void BasicProc::mem_swap (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst1 == dst2)
		return ;
	_DEBUG_ASSERT_ (_ABS_ (dst1 - dst2) >= len) ;
	for (auto &&i : _RANGE_ (0 ,len))
		_SWAP_ (dst1[i] ,dst2[i]) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline void BasicProc::mem_fill (ARR<_ARG1> &dst ,const LENGTH &len ,const _ARG1 &val) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	for (auto &&i : _RANGE_ (0 ,len))
		dst[i] = val ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class UNIT ,class CONT>
class ScopedPtr final
	:private Proxy {
private:
	PTR<UNIT> mPointer ;

public:
	inline ScopedPtr () = delete ;

	inline explicit ScopedPtr (const PTR<UNIT> &pointer)
		:ScopedPtr (ARGVP0) {
		mPointer = pointer ;
	}

	inline ~ScopedPtr () noexcept {
		if (mPointer == NULL)
			return ;
		CONT::free (mPointer) ;
		mPointer = NULL ;
	}

	inline ScopedPtr (const ScopedPtr &) = default ;
	inline ScopedPtr &operator= (const ScopedPtr &) = default ;

	inline ScopedPtr (ScopedPtr &&) = default ;
	inline ScopedPtr &operator= (ScopedPtr &&) = default ;

	inline implicit operator const PTR<UNIT> & () const leftvalue {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		return mPointer ;
	}

	inline void operator= (const DEF<decltype (NULL)> &) leftvalue noexcept {
		mPointer = NULL ;
	}

private:
	inline explicit ScopedPtr (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class ScopedGuard final
	:private Proxy {
private:
	PTR<UNIT> mPointer ;

public:
	inline ScopedGuard () = delete ;

	template <class _ARG1>
	inline explicit ScopedGuard (_ARG1 &address) popping
		:ScopedGuard (ARGVP0) {
		_STATIC_ASSERT_ (stl::is_convertible<_ARG1 & ,UNIT &>::value) ;
		auto &r1x = _FORWARD_<UNIT &> (address) ;
		r1x.lock () ;
		mPointer = DEPTR[r1x] ;
	}

	inline ~ScopedGuard () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			mPointer->unlock () ;
		} ,[&] () {
			_DEBUG_ASSERT_ (FALSE) ;
		}) ;
		mPointer = NULL ;
	}

	inline ScopedGuard (const ScopedGuard &) = default ;
	inline ScopedGuard &operator= (const ScopedGuard &) = default ;

	inline ScopedGuard (ScopedGuard &&) = default ;
	inline ScopedGuard &operator= (ScopedGuard &&) = default ;

private:
	inline explicit ScopedGuard (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class ScopedBuild final
	:private Proxy {
private:
	PTR<const volatile PTR<TEMP<UNIT>>> mPointer ;
	LENGTH mSize ;

public:
	inline ScopedBuild () = delete ;

	template <class _ARG1 ,class... _ARGS>
	inline explicit ScopedBuild (_ARG1 &address ,_ARGS &&...initval) popping
		:ScopedBuild (ARGVP0) {
		_STATIC_ASSERT_ (stl::is_convertible<_ARG1 & ,const PTR<TEMP<UNIT>> &>::value) ;
		auto &r1x = _FORWARD_<const PTR<TEMP<UNIT>> &> (address) ;
		if (r1x == NULL)
			return ;
		mPointer = DEPTR[r1x] ;
		_CREATE_ (r1x ,_FORWARD_<_ARGS> (initval)...) ;
		mSize++ ;
	}

	inline ~ScopedBuild () noexcept {
		if (mPointer == NULL)
			return ;
		const auto r1x = _COPY_ (DEREF[mPointer]) ;
		if (r1x == NULL)
			return ;
		if switch_case (TRUE) {
			if (mSize <= 0)
				discard ;
			_DESTROY_ (r1x) ;
			mSize-- ;
		}
		mPointer = NULL ;
	}

	inline ScopedBuild (const ScopedBuild &) = default ;
	inline ScopedBuild &operator= (const ScopedBuild &) = default ;

	inline ScopedBuild (ScopedBuild &&) = default ;
	inline ScopedBuild &operator= (ScopedBuild &&) = default ;

private:
	inline explicit ScopedBuild (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class ScopedBuild<ARR<UNIT>> final
	:private Proxy {
private:
	PTR<const volatile PTR<ARR<TEMP<UNIT>>>> mPointer ;
	LENGTH mSize ;

public:
	inline ScopedBuild () = delete ;

	template <class _ARG1 ,class... _ARGS>
	inline explicit ScopedBuild (_ARG1 &address ,const LENGTH &len) popping
		:ScopedBuild (ARGVP0) {
		_STATIC_ASSERT_ (stl::is_convertible<_ARG1 & ,const PTR<ARR<TEMP<UNIT>>> &>::value) ;
		auto &r1x = _FORWARD_<const PTR<ARR<TEMP<UNIT>>> &> (address) ;
		if (r1x == NULL)
			return ;
		mPointer = DEPTR[r1x] ;
		while (TRUE) {
			if (mSize >= len)
				break ;
			_CREATE_ (DEPTR[DEREF[r1x][mSize]]) ;
			mSize++ ;
		}
	}

	template <class _ARG1 ,class... _ARGS>
	inline explicit ScopedBuild (_ARG1 &address ,const ARR<UNIT> &src ,const LENGTH &len) popping
		:ScopedBuild (ARGVP0) {
		_STATIC_ASSERT_ (stl::is_convertible<_ARG1 & ,const PTR<ARR<TEMP<UNIT>>> &>::value) ;
		auto &r1x = _FORWARD_<const PTR<ARR<TEMP<UNIT>>> &> (address) ;
		if (r1x == NULL)
			return ;
		mPointer = DEPTR[r1x] ;
		while (TRUE) {
			if (mSize >= len)
				break ;
			_CREATE_ (DEPTR[DEREF[r1x][mSize]] ,src[mSize]) ;
			mSize++ ;
		}
	}

	inline ~ScopedBuild () noexcept {
		if (mPointer == NULL)
			return ;
		const auto r1x = _COPY_ (DEREF[mPointer]) ;
		if (r1x == NULL)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			_DESTROY_ (DEPTR[DEREF[r1x][mSize - 1]]) ;
			mSize-- ;
		}
		mPointer = NULL ;
	}

	inline ScopedBuild (const ScopedBuild &) = default ;
	inline ScopedBuild &operator= (const ScopedBuild &) = default ;

	inline ScopedBuild (ScopedBuild &&) = default ;
	inline ScopedBuild &operator= (ScopedBuild &&) = default ;

private:
	inline explicit ScopedBuild (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) ,mSize (0) {}
} ;

class GlobalHeap final
	:private Wrapped<void> {
private:
	struct Detail ;

public:
	template <class _RET>
	inline imports_static ScopedPtr<_RET ,GlobalHeap> alloc () popping {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (stl::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_RET) <= _ALIGNOF_ (stl::max_align_t)) ;
		const auto r1x = operator new (_SIZEOF_ (_RET) ,stl::nothrow) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		auto &r2x = _LOAD_<_RET> (r1x) ;
		return ScopedPtr<_RET ,GlobalHeap> (DEPTR[r2x]) ;
	}

	template <class _RET>
	inline imports_static ScopedPtr<ARR<_RET> ,GlobalHeap> alloc (const LENGTH &len) popping {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (stl::is_pod<_RET>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_RET) <= _ALIGNOF_ (stl::max_align_t)) ;
		_DEBUG_ASSERT_ (len > 0) ;
		const auto r1x = len * _SIZEOF_ (_RET) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		const auto r2x = operator new (r1x ,stl::nothrow) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		auto &r3x = _LOAD_<ARR<_RET>> (r2x) ;
		return ScopedPtr<ARR<_RET> ,GlobalHeap> (DEPTR[r3x]) ;
	}

	template <class _ARG1>
	inline imports_static void free (const PTR<_ARG1> &address) noexcept {
		auto &r1x = _LOAD_UNSAFE_<NONE> (_ADDRESS_ (address)) ;
		operator delete (DEPTR[r1x] ,stl::nothrow) ;
	}
} ;

template <class>
class AutoRef ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,FALSE>> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	class Holder {
	private:
		friend AutoRef ;
		friend AutoRef<UNIT> ;
		UNIT mValue ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...initval)
			:mValue (_FORWARD_<_ARGS> (initval)...) {}
	} ;

private:
	friend AutoRef<UNIT> ;
	PTR<Holder> mPointer ;

public:
	inline AutoRef ()
		:AutoRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
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

	inline AutoRef (AutoRef &&that) noexcept
		:AutoRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~AutoRef () ;
			new (this) AutoRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	inline explicit AutoRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,TRUE>> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	class Holder {
	private:
		friend AutoRef ;
		friend AutoRef<UNIT> ;
		UNIT mValue ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...initval)
			:mValue (_FORWARD_<_ARGS> (initval)...) {}
	} ;

private:
	friend AutoRef<UNIT> ;
	PTR<Holder> mPointer ;

public:
	inline AutoRef ()
		:AutoRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AutoRef (const AutoRef &that)
		:AutoRef (ARGVP0) {
		if (that.mPointer == NULL)
			return ;
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax ,_FORWARD_<const UNIT &> (that.mPointer->mValue)) ;
		auto &r1x = _LOAD_<Holder> (_FORWARD_<const PTR<TEMP<Holder>> &> (rax)) ;
		mPointer = DEPTR[r1x] ;
		rax = NULL ;
	}

	inline AutoRef &operator= (const AutoRef &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~AutoRef () ;
			new (this) AutoRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline AutoRef (AutoRef &&that) noexcept
		:AutoRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~AutoRef () ;
			new (this) AutoRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	inline explicit AutoRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class AutoRef
	:private AutoRef<SPECIALIZATION<UNIT ,(stl::is_copy_constructible<UNIT>::value && stl::is_nothrow_move_constructible<UNIT>::value)>> {
private:
	using SPECIALIZATION_BASE = AutoRef<SPECIALIZATION<UNIT ,(stl::is_copy_constructible<UNIT>::value && stl::is_nothrow_move_constructible<UNIT>::value)>> ;
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

	inline UNIT &to () leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mValue ;
	}

	inline implicit operator UNIT & () leftvalue {
		return to () ;
	}

	inline PTR<UNIT> operator-> () leftvalue {
		return DEPTR[to ()] ;
	}

	inline const UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->mValue ;
	}

	inline implicit operator const UNIT & () const leftvalue {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const leftvalue {
		return DEPTR[to ()] ;
	}

public:
	template <class... _ARGS>
	inline imports_static AutoRef make (_ARGS &&...initval) {
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax ,_FORWARD_<_ARGS> (initval)...) ;
		auto &r1x = _LOAD_<Holder> (_FORWARD_<const PTR<TEMP<Holder>> &> (rax)) ;
		AutoRef ret = AutoRef (DEPTR[r1x]) ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	inline explicit AutoRef (const PTR<Holder> &pointer)
		:SPECIALIZATION_BASE (ARGVP0) {
		mPointer = pointer ;
	}
} ;

template <class UNIT>
class SharedRef {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	class Holder {
	private:
		friend SharedRef ;
		UNIT mValue ;
		LENGTH mCounter ;

	public:
		template <class... _ARGS>
		inline explicit Holder (_ARGS &&...initval)
			:mValue (_FORWARD_<_ARGS> (initval)...) ,mCounter (0) {}
	} ;

private:
	struct Detail ;
	PTR<Holder> mPointer ;

public:
	inline SharedRef ()
		:SharedRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ~SharedRef () noexcept {
		if (mPointer == NULL)
			return ;
		if switch_case (TRUE) {
			const auto r1x = --mPointer->mCounter ;
			if (r1x != 0)
				discard ;
			mPointer->~Holder () ;
			GlobalHeap::free (mPointer) ;
		}
		mPointer = NULL ;
	}

	inline SharedRef (const SharedRef &that)
		:SharedRef (that.mPointer) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline SharedRef &operator= (const SharedRef &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~SharedRef () ;
			new (this) SharedRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline SharedRef (SharedRef &&that) noexcept
		:SharedRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline SharedRef &operator= (SharedRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~SharedRef () ;
			new (this) SharedRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = static_cast<PTR<Holder>> (mPointer) ;
		return r1x->mValue ;
	}

	inline implicit operator UNIT & () const leftvalue {
		return to () ;
	}

	inline PTR<UNIT> operator-> () const leftvalue {
		return DEPTR[to ()] ;
	}

public:
	template <class... _ARGS>
	inline imports_static SharedRef make (_ARGS &&...initval) {
		auto rax = GlobalHeap::alloc<TEMP<Holder>> () ;
		ScopedBuild<Holder> ANONYMOUS (rax ,_FORWARD_<_ARGS> (initval)...) ;
		auto &r1x = _LOAD_<Holder> (_FORWARD_<const PTR<TEMP<Holder>> &> (rax)) ;
		SharedRef ret = SharedRef (DEPTR[r1x]) ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	inline explicit SharedRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}

	inline explicit SharedRef (const PTR<Holder> &pointer) noexcept
		:SharedRef (ARGVP0) {
		if (pointer == NULL)
			return ;
		const auto r1x = ++pointer->mCounter ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		_STATIC_UNUSED_ (r1x) ;
		mPointer = pointer ;
	}
} ;

template <class>
class AnyRef ;

template <>
class AnyRef<void> {
private:
	exports class Holder
		:public Interface {
	public:
		virtual FLAG typemid () const = 0 ;
	} ;

private:
	template <class>
	friend class AnyRef ;
	PTR<Holder> mPointer ;

public:
	inline AnyRef ()
		:AnyRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline implicit AnyRef (AnyRef<_ARG1> &&that)
		: AnyRef (_MOVE_ (that.template rebind<void> ())) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&that) noexcept
		:AnyRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~AnyRef () ;
			new (this) AnyRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () leftvalue {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (DEREF[this]) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const leftvalue {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (DEREF[this]) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG typemid () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->typemid () ;
	}

private:
	inline explicit AnyRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class AnyRef {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	using Holder = typename AnyRef<void>::Holder ;

private:
	struct Detail ;
	PTR<Holder> mPointer ;

public:
	inline AnyRef ()
		:AnyRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline implicit AnyRef (AnyRef<_ARG1> &&that)
		: AnyRef (_MOVE_ (that.template rebind<UNIT> ())) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline AnyRef (const AnyRef &) = delete ;
	inline AnyRef &operator= (const AnyRef &) = delete ;

	inline AnyRef (AnyRef &&that) noexcept
		:AnyRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~AnyRef () ;
			new (this) AnyRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	template <class _RET>
	inline AnyRef<_RET> &rebind () leftvalue {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (DEREF[this]) ;
	}

	template <class _RET>
	inline const AnyRef<_RET> &rebind () const leftvalue {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		return _CAST_<AnyRef<_RET>> (DEREF[this]) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG typemid () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return mPointer->typemid () ;
	}

	inline UNIT &to () leftvalue {
		using ImplHolder = typename Detail::template ImplHolder<UNIT> ;
		_DEBUG_ASSERT_ (typemid () == _TYPEMID_<UNIT> ()) ;
		const auto r1x = static_cast<PTR<ImplHolder>> (mPointer) ;
		return r1x->mValue ;
	}

	inline implicit operator UNIT & () leftvalue {
		return to () ;
	}

	inline PTR<UNIT> operator-> () leftvalue {
		return DEPTR[to ()] ;
	}

	inline const UNIT &to () const leftvalue {
		using ImplHolder = typename Detail::template ImplHolder<UNIT> ;
		_DEBUG_ASSERT_ (typemid () == _TYPEMID_<UNIT> ()) ;
		const auto r1x = static_cast<PTR<ImplHolder>> (mPointer) ;
		return r1x->mValue ;
	}

	inline implicit operator const UNIT & () const leftvalue {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const leftvalue {
		return DEPTR[to ()] ;
	}

public:
	template <class... _ARGS>
	inline imports_static AnyRef make (_ARGS &&...initval) {
		using ImplHolder = typename Detail::template ImplHolder<UNIT> ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,_FORWARD_<_ARGS> (initval)...) ;
		auto &r1x = _LOAD_<ImplHolder> (_FORWARD_<const PTR<TEMP<ImplHolder>> &> (rax)) ;
		AnyRef ret = AnyRef (DEPTR[r1x]) ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	inline explicit AnyRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}

	inline explicit AnyRef (const PTR<Holder> &pointer)
		:AnyRef (ARGVP0) {
		mPointer = pointer ;
	}
} ;

template <class UNIT>
struct AnyRef<UNIT>::Detail {
	template <class UNIT_>
	class ImplHolder
		:public Holder {
	private:
		friend AnyRef ;
		UNIT_ mValue ;

	public:
		template <class... _ARGS>
		inline explicit ImplHolder (_ARGS &&...initval)
			:mValue (_FORWARD_<_ARGS> (initval)...) {}

		inline FLAG typemid () const override {
			return _TYPEMID_<UNIT_> () ;
		}
	} ;
} ;

template <class>
class UniqueRef ;

template <>
class UniqueRef<void> {
private:
	exports class Holder
		:public Interface {
	public:
		virtual void release () = 0 ;
	} ;

private:
	struct Detail ;
	template <class>
	friend class UniqueRef ;
	PTR<Holder> mPointer ;

public:
	inline UniqueRef ()
		:UniqueRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (_ARG1 &&constructor ,_ARG2 &&destructor) popping
		: UniqueRef (ARGVP0) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Detail ,Dependent>::template ImplHolder<PTR<void ()>> ;
		_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
		_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
		_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<>> ,void>::value) ;
		_STATIC_ASSERT_ (stl::is_convertible<REMOVE_REFERENCE_TYPE<_ARG2> ,PTR<void ()>>::value) ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
		const auto r1x = _FORWARD_<PTR<void ()>> (destructor) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,r1x) ;
		auto &r2x = _LOAD_<ImplHolder> (_FORWARD_<const PTR<TEMP<ImplHolder>> &> (rax)) ;
		constructor () ;
		mPointer = DEPTR[r2x] ;
		rax = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			mPointer->release () ;
		} ,[&] () {
			_DEBUG_ASSERT_ (FALSE) ;
		}) ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&that) noexcept
		:UniqueRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~UniqueRef () ;
			new (this) UniqueRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

private:
	inline explicit UniqueRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

struct UniqueRef<void>::Detail {
	template <class UNIT_>
	class ImplHolder
		:public Holder {
	private:
		UNIT_ mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (const REMOVE_CVR_TYPE<UNIT_> &func)
			:mFunctor (_MOVE_ (func)) {}

		inline explicit ImplHolder (REMOVE_CVR_TYPE<UNIT_> &&func)
			:mFunctor (_MOVE_ (func)) {}

		inline void release () override {
			mFunctor () ;
		}
	} ;
} ;

template <class UNIT>
class UniqueRef {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	using Holder = typename UniqueRef<void>::Holder ;

private:
	struct Detail ;
	PTR<Holder> mPointer ;

public:
	inline UniqueRef ()
		:UniqueRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class _ARG2>
	inline explicit UniqueRef (_ARG1 &&constructor ,_ARG2 &&destructor) popping
		: UniqueRef (ARGVP0) {
		using ImplHolder = typename Detail::template ImplHolder<PTR<void (UNIT &)>> ;
		_STATIC_ASSERT_ (!stl::is_reference<_ARG1>::value) ;
		_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNIT &>> ,void>::value) ;
		_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<UNIT &>> ,void>::value) ;
		_STATIC_ASSERT_ (stl::is_convertible<REMOVE_REFERENCE_TYPE<_ARG2> ,PTR<void (UNIT &)>>::value) ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
		const auto r1x = _FORWARD_<PTR<void (UNIT &)>> (destructor) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,r1x) ;
		auto &r2x = _LOAD_<ImplHolder> (_FORWARD_<const PTR<TEMP<ImplHolder>> &> (rax)) ;
		constructor (r2x.mValue) ;
		mPointer = DEPTR[r2x] ;
		rax = NULL ;
	}

	inline ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			mPointer->release () ;
		} ,[&] () {
			_DEBUG_ASSERT_ (FALSE) ;
		}) ;
		mPointer->~Holder () ;
		GlobalHeap::free (mPointer) ;
		mPointer = NULL ;
	}

	inline UniqueRef (const UniqueRef &) = delete ;
	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	inline UniqueRef (UniqueRef &&that) noexcept
		: UniqueRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~UniqueRef () ;
			new (this) UniqueRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline const UNIT &to () const leftvalue {
		using ImplHolder = typename Detail::template ImplHolder<PTR<void (UNIT &)>> ;
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = static_cast<PTR<ImplHolder>> (mPointer) ;
		return r1x->mValue ;
	}

	inline implicit operator const UNIT & () const leftvalue {
		return to () ;
	}

	inline PTR<const UNIT> operator-> () const leftvalue {
		return DEPTR[to ()] ;
	}

public:
	template <class... _ARGS>
	inline imports_static UniqueRef make (_ARGS &&...initval) {
		using ImplHolder = typename Detail::template ImplHolder<PTR<void (UNIT &)>> ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
		const auto r1x = _FORWARD_<PTR<void (UNIT &)>> ([] (UNIT &) {}) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,r1x) ;
		auto &r2x = _LOAD_<ImplHolder> (_FORWARD_<const PTR<TEMP<ImplHolder>> &> (rax)) ;
		r2x.mValue = UNIT (_FORWARD_<_ARGS> (initval)...) ;
		const auto r3x = _FORWARD_<PTR<Holder>> (DEPTR[r2x]) ;
		UniqueRef ret = UniqueRef (r3x) ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	inline explicit UniqueRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}

	inline explicit UniqueRef (const PTR<Holder> &pointer)
		:UniqueRef (ARGVP0) {
		mPointer = pointer ;
	}
} ;

template <class UNIT>
struct UniqueRef<UNIT>::Detail {
	template <class UNIT_>
	class ImplHolder
		:public Holder {
	private:
		friend UniqueRef ;
		REMOVE_CVR_TYPE<UNIT> mValue ;
		UNIT_ mFunctor ;

	public:
		inline ImplHolder () = delete ;

		inline explicit ImplHolder (const REMOVE_CVR_TYPE<UNIT_> &func)
			:mFunctor (_MOVE_ (func)) {}

		inline explicit ImplHolder (REMOVE_CVR_TYPE<UNIT_> &&func)
			:mFunctor (_MOVE_ (func)) {}

		inline void release () override {
			mFunctor (mValue) ;
		}
	} ;
} ;

template <class UNIT>
class PhanRef {
	_STATIC_ASSERT_ (_SIZEOF_ (UNIT) > 0) ;

private:
	PTR<UNIT> mPointer ;

public:
	inline PhanRef ()
		:PhanRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ~PhanRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer = NULL ;
	}

	inline PhanRef (const PhanRef &) = delete ;
	inline PhanRef &operator= (const PhanRef &) = delete ;

	inline PhanRef (PhanRef &&that) noexcept
		:PhanRef (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline PhanRef &operator= (PhanRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~PhanRef () ;
			new (this) PhanRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = static_cast<PTR<UNIT>> (mPointer) ;
		return DEREF[r1x] ;
	}

	inline implicit operator UNIT & () const leftvalue {
		return to () ;
	}

	inline PTR<UNIT> operator-> () const leftvalue {
		return DEPTR[to ()] ;
	}

private:
	inline explicit PhanRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}

	inline explicit PhanRef (const PTR<UNIT> &pointer)
		:PhanRef (ARGVP0) {
		mPointer = pointer ;
	}

public:
	//@warn: phantom means deliver pointer without holder
	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<UNIT>>::value>>
	inline imports_static PhanRef make (_ARG1 &val) popping {
		return PhanRef (DEPTR[val]) ;
	}

	template <class _ARG1>
	inline imports_static PhanRef make (const PhanRef<_ARG1> &val) {
		_STATIC_ASSERT_ (stl::is_convertible<_ARG1 & ,UNIT &>::value) ;
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
	exports class Holder
		:public Interface {
	public:
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const = 0 ;
	} ;

private:
	struct Detail ;
	PTR<Holder> mPointer ;
	PTR<UNIT1 (UNITS...)> mFunction ;

public:
	inline Function ()
		:Function (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline implicit Function (const PTR<UNIT1 (UNITS...)> &that)
		: Function (ARGVP0) {
		_DEBUG_ASSERT_ (that != NULL) ;
		mFunction = that ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<!stl::is_same<REMOVE_CVR_TYPE<_ARG1> ,Function>::value>>
	inline implicit Function (_ARG1 &&that)
		: Function (ARGVP0) {
		using ImplHolder = typename Detail::template ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>> ;
		_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNITS...>> ,UNIT1>::value) ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,_FORWARD_<_ARG1> (that)) ;
		auto &r1x = _LOAD_<ImplHolder> (_FORWARD_<const PTR<TEMP<ImplHolder>> &> (rax)) ;
		mPointer = DEPTR[r1x] ;
		rax = NULL ;
	}

	inline ~Function () noexcept {
		if (mPointer == NULL)
			if (mFunction == NULL)
				return ;
		if switch_case (TRUE) {
			if (mPointer == NULL)
				discard ;
			mPointer->~Holder () ;
			GlobalHeap::free (mPointer) ;
		}
		mFunction = NULL ;
		mPointer = NULL ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&that) noexcept
		:Function (ARGVP0) {
		mPointer = _EXCHANGE_ (that.mPointer) ;
		mFunction = _EXCHANGE_ (that.mFunction) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Function () ;
			new (this) Function (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			if (mFunction == NULL)
				return FALSE ;
		return TRUE ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mFunction != NULL)
			return mFunction (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
		return mPointer->invoke (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		return invoke (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

public:
	template <class... _ARGS>
	inline imports_static Function make (const PTR<UNIT1 (UNITS... ,_ARGS...)> &func ,const REMOVE_CVR_TYPE<_ARGS> &...parameter) {
		using ImplHolder = typename Detail::template ImplHolder<PTR<UNIT1 (UNITS... ,_ARGS...)>> ;
		auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,func ,parameter...) ;
		auto &r1x = _LOAD_<ImplHolder> (_FORWARD_<const PTR<TEMP<ImplHolder>> &> (rax)) ;
		const auto r2x = _FORWARD_<PTR<Holder>> (DEPTR[r1x]) ;
		Function ret = Function (r2x) ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	inline explicit Function (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) ,mFunction (NULL) {}

	inline explicit Function (const PTR<Holder> &pointer)
		:Function (ARGVP0) {
		mPointer = pointer ;
	}
} ;

template <class UNIT1 ,class... UNITS>
struct Function<UNIT1 (UNITS...)>::Detail {
	template <class>
	class ImplHolder ;
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<UNIT1 (UNITS...)>::Detail::ImplHolder
	:public Holder {
	_STATIC_ASSERT_ (stl::is_same<RESULT_OF_TYPE<UNIT_ ,ARGVS<UNITS...>> ,UNIT1>::value) ;

private:
	UNIT_ mFunctor ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const REMOVE_CVR_TYPE<UNIT_> &func)
		:mFunctor (_MOVE_ (func)) {}

	inline explicit ImplHolder (REMOVE_CVR_TYPE<UNIT_> &&func)
		:mFunctor (_MOVE_ (func)) {}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return mFunctor (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}
} ;

namespace U {
//@error: fuck vs2017
template <class _ARG1 ,class... _ARGS>
using MEMBER_FUNCTION_HINT = DEF<DEF<_ARG1 (_ARGS...)> NONE::*> ;
} ;

template <class UNIT1 ,class... UNITS>
class Function<U::MEMBER_FUNCTION_HINT<UNIT1 ,UNITS...>> {
#pragma push_macro ("fake")
#undef fake
#define fake m_fake ()

private:
	class FakeHolder ;

	exports class Holder
		:public Interface {
	public:
		virtual void friend_copy (const PTR<TEMP<FakeHolder>> &address) const = 0 ;
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const = 0 ;
	} ;

	class FakeHolder
		:public Holder {
	private:
		PTR<NONE> mContext ;
		DEF<DEF<UNIT1 (UNITS...)> NONE::*> mFunction ;

	public:
		inline FakeHolder () = delete ;

		inline void friend_copy (const PTR<TEMP<FakeHolder>> &address) const override ;

		inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override ;
	} ;

private:
	struct Detail ;
	TEMP<FakeHolder> mVariant ;

public:
	inline Function ()
		:Function (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline implicit Function (const PTR<UNIT1 (UNITS...)> &that)
		: Function (ARGVP0) {
		using PureHolder = typename Detail::PureHolder ;
		_DEBUG_ASSERT_ (that != NULL) ;
		static_create<PureHolder> (DEPTR[mVariant] ,that) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context_ ,const DEF<DEF<UNIT1 (UNITS...)> _ARG1::*> &func)
		:Function (ARGVP0) {
		using ImplHolder = typename Detail::template ImplHolder<_ARG1> ;
		static_create<ImplHolder> (DEPTR[mVariant] ,DEPTR[context_.self] ,func) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<const _ARG1> &context_ ,const DEF<DEF<UNIT1 (UNITS...) const> _ARG1::*> &func)
		:Function (ARGVP0) {
		using ImplHolder = typename Detail::template ImplHolder<const _ARG1> ;
		static_create<ImplHolder> (DEPTR[mVariant] ,DEPTR[context_.self] ,func) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<_ARG1> &context_ ,const PTR<UNIT1 (PTR<_ARG1> ,UNITS...)> &func)
		:Function (ARGVP0) {
		using ImplHolder = typename Detail::template ImplHolder<PTR<_ARG1>> ;
		_DEBUG_ASSERT_ (func != NULL) ;
		static_create<ImplHolder> (DEPTR[mVariant] ,DEPTR[context_.self] ,func) ;
	}

	template <class _ARG1>
	inline explicit Function (const PhanRef<const _ARG1> &context_ ,const PTR<UNIT1 (PTR<const _ARG1> ,UNITS...)> &func)
		:Function (ARGVP0) {
		using ImplHolder = typename Detail::template ImplHolder<PTR<const _ARG1>> ;
		_DEBUG_ASSERT_ (func != NULL) ;
		static_create<ImplHolder> (DEPTR[mVariant] ,DEPTR[context_.self] ,func) ;
	}

	inline ~Function () noexcept {
		if (!exist ())
			return ;
		fake.~Holder () ;
		_ZERO_ (mVariant) ;
	}

	inline Function (const Function &) = delete ;
	inline Function &operator= (const Function &) = delete ;

	inline Function (Function &&that) noexcept
		:Function (ARGVP0) {
		if (!that.exist ())
			return ;
		that.fake.friend_copy (DEPTR[mVariant]) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Function () ;
			new (this) Function (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline BOOL exist () const {
		const auto r1x = _CAST_<FLAG> (_FORWARD_<const Interface &> (fake)) ;
		if (r1x == VAR_ZERO)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		_DEBUG_ASSERT_ (exist ()) ;
		return fake.invoke (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		return invoke (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

private:
	inline explicit Function (const DEF<decltype (ARGVP0)> &) noexcept {
		_ZERO_ (mVariant) ;
	}

private:
	inline Holder &m_fake () leftvalue {
		return _CAST_<FakeHolder> (mVariant) ;
	}

	inline const Holder &m_fake () const leftvalue {
		return _CAST_<FakeHolder> (mVariant) ;
	}

private:
	inline static TEMP<FakeHolder> zero_variant () {
		TEMP<FakeHolder> ret ;
		_ZERO_ (ret) ;
		return _MOVE_ (ret) ;
	}

	template <class _RET ,class... _ARGS>
	inline static void static_create (const PTR<TEMP<FakeHolder>> &address ,_ARGS &&...funcval) {
		_STATIC_ASSERT_ (!stl::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (stl::is_constructible<_RET ,_ARGS &&...>::value) ;
		auto &r1x = _LOAD_<TEMP<_RET>> (address) ;
		auto &r2x = _FORWARD_<Holder &> (_CAST_<_RET> (r1x)) ;
		auto &r3x = _FORWARD_<Holder &> (_CAST_<FakeHolder> (DEREF[address])) ;
		_DYNAMIC_ASSERT_ (DEPTR[r2x] == DEPTR[r3x]) ;
		_CREATE_ (DEPTR[r1x] ,_FORWARD_<_ARGS> (funcval)...) ;
	}

#pragma pop_macro ("fake")
} ;

template <class UNIT1 ,class... UNITS>
struct Function<U::MEMBER_FUNCTION_HINT<UNIT1 ,UNITS...>>::Detail {
	class PureHolder
		:public Holder {
	private:
		PTR<UNIT1 (UNITS...)> mFunction ;

	public:
		inline PureHolder () = delete ;

		inline explicit PureHolder (const PTR<UNIT1 (UNITS...)> &func)
			:mFunction (func) {}

		inline void friend_copy (const PTR<TEMP<FakeHolder>> &address) const override {
			static_create<PureHolder> (address ,mFunction) ;
		}

		inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
			return mFunction (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
		}
	} ;

	template <class>
	class ImplHolder ;
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<U::MEMBER_FUNCTION_HINT<UNIT1 ,UNITS...>>::Detail::ImplHolder
	:public Holder {
private:
	PTR<UNIT_> mContext ;
	DEF<DEF<UNIT1 (UNITS...)> UNIT_::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const PTR<UNIT_> &context_ ,const DEF<DEF<UNIT1 (UNITS...)> UNIT_::*> &func)
		:mContext (context_) ,mFunction (func) {}

	inline void friend_copy (const PTR<TEMP<FakeHolder>> &address) const override {
		static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return (DEREF[mContext].*mFunction) (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<U::MEMBER_FUNCTION_HINT<UNIT1 ,UNITS...>>::Detail::ImplHolder<const UNIT_>
	:public Holder {
private:
	PTR<const UNIT_> mContext ;
	DEF<DEF<UNIT1 (UNITS...) const> UNIT_::*> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const PTR<const UNIT_> &context_ ,const DEF<DEF<UNIT1 (UNITS...) const> UNIT_::*> &func)
		:mContext (context_) ,mFunction (func) {}

	inline void friend_copy (const PTR<TEMP<FakeHolder>> &address) const override {
		static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return (DEREF[mContext].*mFunction) (_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<U::MEMBER_FUNCTION_HINT<UNIT1 ,UNITS...>>::Detail::ImplHolder<PTR<UNIT_>>
	:public Holder {
private:
	PTR<UNIT_> mContext ;
	PTR<UNIT1 (PTR<UNIT_> ,UNITS...)> mFunction ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const PTR<UNIT_> &context_ ,const PTR<UNIT1 (PTR<UNIT_> ,UNITS...)> &func)
		:mContext (context_) ,mFunction (func) {}

	inline void friend_copy (const PTR<TEMP<FakeHolder>> &address) const override {
		static_create<ImplHolder> (address ,mContext ,mFunction) ;
	}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return mFunction (mContext ,_FORWARD_<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}
} ;

template <class ,class>
class Buffer ;

using SFLEX = ZERO ;
using SFIXED = ARGC<-1> ;
using SAUTO = ARGC<-2> ;
using SCPHAN = ARGC<-4> ;
using SMPHAN = ARGC<-5> ;

template <class UNIT ,class SIZE>
class Buffer {
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
#ifdef __CSC_COMPILER_MSVC__
	//@error: fuck vs2015
	friend Buffer<UNIT ,SAUTO> ;
#endif
	DEF<UNIT[SIZE::value]> mBuffer ;

public:
	inline Buffer () = default ;

	inline explicit Buffer (const LENGTH &len) {
		_DEBUG_ASSERT_ (len >= 0 && len <= SIZE::value) ;
	}

	inline implicit Buffer (const DEF<UNIT[SIZE::value]> &that)
		:Buffer (_MOVE_ (_CAST_<Buffer> (that))) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline implicit Buffer (DEF<UNIT[SIZE::value]> &&that)
		: Buffer (_MOVE_ (_CAST_<Buffer> (that))) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ARR<UNIT> &to () leftvalue {
		return PTRTOARR[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () leftvalue {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const leftvalue {
		return PTRTOARR[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return SIZE::value ;
	}

	inline UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	inline const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	inline INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - PTRTOARR[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		return BasicProc::mem_equal (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE::value) ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Buffer &that) const {
		return BasicProc::mem_compr (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE::value) ;
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

	inline Buffer expand (const LENGTH &len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) {
		BasicProc::mem_swap (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE::value) ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SFIXED> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,len) ;
		auto &r1x = _LOAD_<ARR<UNIT>> (_FORWARD_<const PTR<ARR<TEMP<UNIT>>> &> (rax)) ;
		mBuffer = DEPTR[r1x] ;
		mSize = len ;
		rax = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			DEREF[mBuffer][i].~UNIT () ;
			_STATIC_UNUSED_ (i) ;
		}
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;
	inline Buffer (Buffer &&) = delete ;
	inline Buffer &operator= (Buffer &&) = delete ;

	inline ARR<UNIT> &to () leftvalue {
		if (mBuffer == NULL)
			return _NULL_<ARR<UNIT>> () ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () leftvalue {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_<ARR<UNIT>> () ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	inline const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	inline INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!BasicProc::mem_equal (DEREF[mBuffer] ,DEREF[that.mBuffer] ,that.mSize))
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
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return U::OPERATOR_COMPR::invoke (mSize ,that.mSize) ;
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

	inline Buffer<UNIT ,SAUTO> expand (const LENGTH &len) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Buffer<UNIT ,SAUTO> () ;
	}

	inline void swap (Buffer<UNIT ,SAUTO> &that) {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	inline void swap (Buffer &that) {
		_DYNAMIC_ASSERT_ (mSize == that.mSize) ;
		BasicProc::mem_swap (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,mSize) ;
	}

private:
	inline explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		:mBuffer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
using FixedBuffer = CAST_TRAITS_TYPE<Buffer<REMOVE_CVR_TYPE<UNIT> ,SFIXED> ,UNIT> ;

template <class UNIT>
class Buffer<UNIT ,SAUTO> ;

template <class UNIT>
class Buffer<SPECIALIZATION<UNIT ,FALSE> ,SAUTO> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	friend Buffer<UNIT ,SAUTO> ;
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,len) ;
		auto &r1x = _LOAD_<ARR<UNIT>> (_FORWARD_<const PTR<ARR<TEMP<UNIT>>> &> (rax)) ;
		mBuffer = DEPTR[r1x] ;
		mSize = len ;
		rax = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			DEREF[mBuffer][i].~UNIT () ;
			_STATIC_UNUSED_ (i) ;
		}
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &) = delete ;
	inline Buffer &operator= (const Buffer &) = delete ;

	inline Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	inline explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		: mBuffer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class Buffer<SPECIALIZATION<UNIT ,TRUE> ,SAUTO> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	friend Buffer<UNIT ,SAUTO> ;
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,len) ;
		auto &r1x = _LOAD_<ARR<UNIT>> (_FORWARD_<const PTR<ARR<TEMP<UNIT>>> &> (rax)) ;
		mBuffer = DEPTR[r1x] ;
		mSize = len ;
		rax = NULL ;
	}

	inline ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			DEREF[mBuffer][i].~UNIT () ;
			_STATIC_UNUSED_ (i) ;
		}
		GlobalHeap::free (mBuffer) ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	inline Buffer (const Buffer &that)
		:Buffer (ARGVP0) {
		if (that.mBuffer == NULL)
			return ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> (that.mSize) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,DEREF[that.mBuffer] ,that.mSize) ;
		auto &r1x = _LOAD_<ARR<UNIT>> (_FORWARD_<const PTR<ARR<TEMP<UNIT>>> &> (rax)) ;
		mBuffer = DEPTR[r1x] ;
		mSize = that.mSize ;
		rax = NULL ;
	}

	inline Buffer &operator= (const Buffer &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	inline explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		: mBuffer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class Buffer<UNIT ,SAUTO>
	:private Buffer<SPECIALIZATION<UNIT ,(stl::is_copy_constructible<UNIT>::value && stl::is_nothrow_move_constructible<UNIT>::value)> ,SAUTO> {
private:
	using SPECIALIZATION_BASE = Buffer<SPECIALIZATION<UNIT ,(stl::is_copy_constructible<UNIT>::value && stl::is_nothrow_move_constructible<UNIT>::value)> ,SAUTO> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mBuffer ;
	using SPECIALIZATION_BASE::mSize ;

public:
	inline Buffer () = default ;

	inline explicit Buffer (const LENGTH &len)
		:SPECIALIZATION_BASE (len) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline ARR<UNIT> &to () leftvalue {
		if (mBuffer == NULL)
			return _NULL_<ARR<UNIT>> () ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () leftvalue {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline const ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_<ARR<UNIT>> () ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	inline const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	inline INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!BasicProc::mem_equal (DEREF[mBuffer] ,DEREF[that.mBuffer] ,that.mSize))
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
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return U::OPERATOR_COMPR::invoke (mSize ,that.mSize) ;
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
		const auto r1x = LENGTH (mSize * MATH_SQRT2) ;
		const auto r2x = mSize + DEFAULT_RECURSIVE_SIZE::value ;
		return _MAX_ (r1x ,r2x) ;
	}

	inline Buffer expand (const LENGTH &len) const {
		return Buffer (len) ;
	}

	inline void swap (Buffer &that) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}
} ;

template <class UNIT>
using AutoBuffer = CAST_TRAITS_TYPE<Buffer<REMOVE_CVR_TYPE<UNIT> ,SAUTO> ,UNIT> ;

template <class UNIT>
class Buffer<UNIT ,SCPHAN> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	PTR<const ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
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

	inline Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline const ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_<ARR<UNIT>> () ;
		const auto r1x = static_cast<PTR<const ARR<UNIT>>> (mBuffer) ;
		return DEREF[r1x] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return to () ;
	}

	inline implicit operator PTR<const UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline const UNIT &get (const INDEX &index) const leftvalue {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		const auto r1x = static_cast<PTR<const ARR<UNIT>>> (mBuffer) ;
		return DEREF[r1x][index] ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	inline INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!BasicProc::mem_equal (DEREF[mBuffer] ,DEREF[that.mBuffer] ,that.mSize))
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
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return U::OPERATOR_COMPR::invoke (mSize ,that.mSize) ;
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

	inline Buffer expand (const LENGTH &len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

private:
	inline explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		: mBuffer (NULL) ,mSize (0) {}

	inline explicit Buffer (const PTR<const ARR<UNIT>> &buffer ,const LENGTH &size_)
		: Buffer (ARGVP0) {
		if (size_ == 0)
			return ;
		_DEBUG_ASSERT_ (size_ > 0) ;
		mBuffer = buffer ;
		mSize = size_ ;
	}

public:
	//@warn: phantom means deliver pointer without holder
	inline imports_static Buffer make (const ARR<UNIT> &src ,const LENGTH &len) popping {
		return Buffer (DEPTR[src] ,len) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_bounded_array_of<UNIT ,_ARG1>::value>>
	inline imports_static Buffer make (_ARG1 &val) popping {
		return make (PTRTOARR[val] ,_COUNTOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline imports_static Buffer make (const Buffer<UNIT ,_ARG1> &val) {
		return make (val ,val.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<stl::is_same<UNIT ,BYTE>::value && !stl::is_same<_ARG1 ,BYTE>::value>>
	inline imports_static Buffer make (const Buffer<_ARG1 ,_ARG2> &val) {
		_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value) ;
		if (val.size () == 0)
			return Buffer () ;
		auto &r1x = _LOAD_<ARR<BYTE>> (DEPTR[val.self]) ;
		return make (r1x ,(val.size () * _SIZEOF_ (_ARG1))) ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SMPHAN> {
	_STATIC_ASSERT_ (stl::is_complete<UNIT>::value) ;

private:
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	inline Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
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

	inline Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		mBuffer = _EXCHANGE_ (that.mBuffer) ;
		mSize = _EXCHANGE_ (that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_<ARR<UNIT>> () ;
		const auto r1x = static_cast<PTR<ARR<UNIT>>> (mBuffer) ;
		return DEREF[r1x] ;
	}

	inline implicit operator ARR<UNIT> & () const leftvalue {
		return to () ;
	}

	inline implicit operator PTR<UNIT> () = delete ;

	inline LENGTH size () const {
		return mSize ;
	}

	inline UNIT &get (const INDEX &index) const leftvalue {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		const auto r1x = static_cast<PTR<ARR<UNIT>>> (mBuffer) ;
		return DEREF[r1x][index] ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

	inline UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	inline INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	inline BOOL equal (const Buffer &that) const {
		if (mSize != that.mSize)
			return FALSE ;
		if (!BasicProc::mem_equal (DEREF[mBuffer] ,DEREF[that.mBuffer] ,that.mSize))
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
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return U::OPERATOR_COMPR::invoke (mSize ,that.mSize) ;
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

	inline Buffer expand (const LENGTH &len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	inline void swap (Buffer &that) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

private:
	inline explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		: mBuffer (NULL) ,mSize (0) {}

	inline explicit Buffer (const PTR<ARR<UNIT>> &buffer ,const LENGTH &size_)
		: Buffer (ARGVP0) {
		if (size_ == 0)
			return ;
		_DEBUG_ASSERT_ (size_ > 0) ;
		mBuffer = buffer ;
		mSize = size_ ;
	}

public:
	//@warn: phantom means deliver pointer without holder
	inline imports_static Buffer make (ARR<UNIT> &src ,const LENGTH &len) popping {
		return Buffer (DEPTR[src] ,len) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_bounded_array_of<UNIT ,_ARG1>::value>>
	inline imports_static Buffer make (_ARG1 &val) popping {
		return make (PTRTOARR[val] ,_COUNTOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	inline imports_static Buffer make (Buffer<UNIT ,_ARG1> &val) popping {
		return make (val.self ,val.size ()) ;
	}

	inline imports_static Buffer make (const Buffer<UNIT ,SMPHAN> &val) {
		return make (val.self ,val.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<stl::is_same<UNIT ,BYTE>::value && !stl::is_same<_ARG1 ,BYTE>::value>>
	inline imports_static Buffer make (Buffer<_ARG1 ,_ARG2> &val) popping {
		_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value) ;
		if (val.size () == 0)
			return Buffer () ;
		auto &r1x = _LOAD_<ARR<BYTE>> (DEPTR[val.self]) ;
		return make (r1x ,(val.size () * _SIZEOF_ (_ARG1))) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_same<UNIT ,BYTE>::value && !stl::is_same<_ARG1 ,BYTE>::value>>
	inline imports_static Buffer make (const Buffer<_ARG1 ,SMPHAN> &val) {
		_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value) ;
		if (val.size () == 0)
			return Buffer () ;
		auto &r1x = _LOAD_<ARR<BYTE>> (DEPTR[val.self]) ;
		return make (r1x ,(val.size () * _SIZEOF_ (_ARG1))) ;
	}
} ;

template <class UNIT>
using PhanBuffer = Buffer<REMOVE_CONST_TYPE<UNIT> ,CONDITIONAL_TYPE<stl::is_const<UNIT>::value ,SCPHAN ,SMPHAN>> ;

template <class ,class>
class Allocator ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,FALSE ,FALSE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_THIS = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Node {
	private:
		friend Allocator ;
		friend SPECIALIZATION_THIS ;
		TEMP<UNIT> mValue ;
		INDEX mNext ;

	public:
		inline Node () = default ;
	} ;

private:
	friend SPECIALIZATION_THIS ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator ()
		:Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline explicit Allocator (const LENGTH &len)
		:Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline ~Allocator () noexcept {
		if (mAllocator.size () == 0)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			if switch_case (TRUE) {
				INDEX ix = mSize - 1 ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_DESTROY_ (&mAllocator[ix].mValue) ;
			}
			mSize-- ;
		}
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;
	inline Allocator (Allocator &&) = delete ;
	inline Allocator &operator= (Allocator &&) = delete ;

private:
	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

private:
	inline SPECIALIZATION_THIS &m_spec () leftvalue {
		return DEREF[static_cast<PTR<SPECIALIZATION_THIS>> (this)] ;
	}

	inline const SPECIALIZATION_THIS &m_spec () const leftvalue {
		return DEREF[static_cast<PTR<const SPECIALIZATION_THIS>> (this)] ;
	}

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,FALSE ,TRUE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_THIS = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Node {
	private:
		friend Allocator ;
		friend SPECIALIZATION_THIS ;
		TEMP<UNIT> mValue ;
		INDEX mNext ;

	public:
		inline Node () = default ;
	} ;

private:
	friend SPECIALIZATION_THIS ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator ()
		:Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline explicit Allocator (const LENGTH &len)
		:Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline ~Allocator () noexcept {
		if (mAllocator.size () == 0)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			if switch_case (TRUE) {
				INDEX ix = mSize - 1 ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_DESTROY_ (&mAllocator[ix].mValue) ;
			}
			mSize-- ;
		}
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	inline Allocator (const Allocator &) = delete ;
	inline Allocator &operator= (const Allocator &) = delete ;

	inline Allocator (Allocator &&that) noexcept
		:Allocator (ARGVP0 ,_MOVE_ (that.mAllocator)) {
		_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<UNIT>::value) ;
		if (mAllocator.size () != that.mAllocator.size ())
			mSize = _EXCHANGE_ (that.mSize) ;
		mSize += _EBOOL_ (stl::is_pod<UNIT>::value) * _MAX_ (VAR_ZERO ,(that.mSize - mSize)) ;
		while (TRUE) {
			if (mSize >= that.mSize)
				break ;
			if switch_case (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (DEPTR[mAllocator[ix].mValue] ,_MOVE_ (_CAST_<UNIT> (that.mAllocator[ix].mValue))) ;
			}
			mSize++ ;
		}
		mLength = _EXCHANGE_ (that.mLength) ;
		mFree = _EXCHANGE_ (that.mFree ,VAR_NONE) ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Allocator () ;
			new (this) Allocator (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,Buffer<Node ,SIZE> &&allocator_)
		:mAllocator (_MOVE_ (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

private:
	inline SPECIALIZATION_THIS &m_spec () leftvalue {
		return DEREF[static_cast<PTR<SPECIALIZATION_THIS>> (this)] ;
	}

	inline const SPECIALIZATION_THIS &m_spec () const leftvalue {
		return DEREF[static_cast<PTR<const SPECIALIZATION_THIS>> (this)] ;
	}

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,TRUE ,TRUE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

private:
	using SPECIALIZATION_THIS = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	class Node {
	private:
		friend Allocator ;
		friend SPECIALIZATION_THIS ;
		TEMP<UNIT> mValue ;
		INDEX mNext ;

	public:
		inline Node () = default ;
	} ;

private:
	friend SPECIALIZATION_THIS ;
	Buffer<Node ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

public:
	inline Allocator ()
		:Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline explicit Allocator (const LENGTH &len)
		:Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	inline ~Allocator () noexcept {
		if (mAllocator.size () == 0)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			if switch_case (TRUE) {
				INDEX ix = mSize - 1 ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_DESTROY_ (&mAllocator[ix].mValue) ;
			}
			mSize-- ;
		}
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	inline Allocator (const Allocator &that)
		:Allocator (ARGVP0 ,_MOVE_ (that.mAllocator)) {
		_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<UNIT>::value) ;
		if (mAllocator.size () != that.mAllocator.size ())
			mSize = that.mSize ;
		mSize += _EBOOL_ (stl::is_pod<UNIT>::value) * _MAX_ (VAR_ZERO ,(that.mSize - mSize)) ;
		while (TRUE) {
			if (mSize >= that.mSize)
				break ;
			if switch_case (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (DEPTR[mAllocator[ix].mValue] ,_MOVE_ (_CAST_<UNIT> (that.mAllocator[ix].mValue))) ;
			}
			mSize++ ;
		}
		mLength = that.mLength ;
		mFree = that.mFree ;
	}

	inline Allocator &operator= (const Allocator &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Allocator () ;
			new (this) Allocator (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	inline Allocator (Allocator &&that) noexcept
		:Allocator (ARGVP0 ,_MOVE_ (that.mAllocator)) {
		_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<UNIT>::value) ;
		if (mAllocator.size () != that.mAllocator.size ())
			mSize = _EXCHANGE_ (that.mSize) ;
		mSize += _EBOOL_ (stl::is_pod<UNIT>::value) * _MAX_ (VAR_ZERO ,(that.mSize - mSize)) ;
		while (TRUE) {
			if (mSize >= that.mSize)
				break ;
			if switch_case (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (DEPTR[mAllocator[ix].mValue] ,_MOVE_ (_CAST_<UNIT> (that.mAllocator[ix].mValue))) ;
			}
			mSize++ ;
		}
		mLength = _EXCHANGE_ (that.mLength) ;
		mFree = _EXCHANGE_ (that.mFree ,VAR_NONE) ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			DEREF[this].~Allocator () ;
			new (this) Allocator (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,const Buffer<Node ,SIZE> &allocator_)
		:mAllocator (_MOVE_ (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline explicit Allocator (const DEF<decltype (ARGVP0)> & ,Buffer<Node ,SIZE> &&allocator_)
		:mAllocator (_MOVE_ (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

private:
	inline SPECIALIZATION_THIS &m_spec () leftvalue {
		return DEREF[static_cast<PTR<SPECIALIZATION_THIS>> (this)] ;
	}

	inline const SPECIALIZATION_THIS &m_spec () const leftvalue {
		return DEREF[static_cast<PTR<const SPECIALIZATION_THIS>> (this)] ;
	}

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator
	:private Allocator<SPECIALIZATION<UNIT ,(stl::is_copy_constructible<Buffer<UNIT ,SIZE>>::value && stl::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value) ,stl::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value> ,SIZE> {
private:
	using SPECIALIZATION_BASE = Allocator<SPECIALIZATION<UNIT ,(stl::is_copy_constructible<Buffer<UNIT ,SIZE>>::value && stl::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value) ,stl::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value> ,SIZE> ;
	using Node = typename SPECIALIZATION_BASE::Node ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mAllocator ;
	using SPECIALIZATION_BASE::mSize ;
	using SPECIALIZATION_BASE::mLength ;
	using SPECIALIZATION_BASE::mFree ;

public:
	inline Allocator () = default ;

	inline explicit Allocator (const LENGTH &len)
		:SPECIALIZATION_BASE (len) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline LENGTH size () const {
		return mSize ;
	}

	inline LENGTH length () const {
		if (mAllocator.size () == 0)
			return 0 ;
		return mLength ;
	}

	inline void clear () {
		INDEX ix = VAR_NONE ;
		INDEX iy = VAR_NONE ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			iy = ix ;
			ix = mSize + ~i ;
			if (mAllocator[ix].mNext == VAR_USED)
				_DESTROY_ (DEPTR[mAllocator[ix].mValue]) ;
			mAllocator[ix].mNext = iy ;
		}
		mLength = 0 ;
		mFree = ix ;
	}

	inline BOOL used (const INDEX &index) const {
		if (mAllocator[index].mNext != VAR_USED)
			return FALSE ;
		return TRUE ;
	}

	inline UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<UNIT> (mAllocator[index].mValue) ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	inline const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_<UNIT> (mAllocator[index].mValue) ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	inline INDEX at (const UNIT &item) const {
		auto &r1x = _OFFSET_ (&Node::mValue ,_CAST_<TEMP<UNIT>> (item)) ;
		INDEX ret = mAllocator.at (r1x) ;
		if (!used (ret))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	template <class... _ARGS>
	inline INDEX alloc (_ARGS &&...initval) popping {
		_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<UNIT>::value) ;
		INDEX ret = VAR_NONE ;
		auto fax = TRUE ;
		if switch_case (fax) {
			if (mFree != VAR_NONE)
				discard ;
			auto tmp = mAllocator.expand (mAllocator.expand_size ()) ;
			ret = mSize ;
			_CREATE_ (DEPTR[tmp[ret].mValue] ,_FORWARD_<_ARGS> (initval)...) ;
			for (auto &&i : _RANGE_ (0 ,mSize)) {
				_CREATE_ (DEPTR[tmp[i].mValue] ,_MOVE_ (_CAST_<UNIT> (mAllocator[i].mValue))) ;
				tmp[i].mNext = VAR_USED ;
			}
			mAllocator.swap (tmp) ;
			update_reserve (mSize ,mFree) ;
		}
		if switch_case (fax) {
			ret = mFree ;
			_CREATE_ (DEPTR[mAllocator[ret].mValue] ,_FORWARD_<_ARGS> (initval)...) ;
		}
		mFree = mAllocator[ret].mNext ;
		mAllocator[ret].mNext = VAR_USED ;
		mLength++ ;
		return _MOVE_ (ret) ;
	}

	inline void free (const INDEX &index) noexcept {
		_DEBUG_ASSERT_ (used (index)) ;
		_DESTROY_ (DEPTR[mAllocator[index].mValue]) ;
		mAllocator[index].mNext = mFree ;
		mFree = index ;
		mLength-- ;
	}

	inline void reserve (const LENGTH &len) {
		_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<UNIT>::value) ;
		_DEBUG_ASSERT_ (len >= 0) ;
		const auto r1x = _MAX_ (len - (mSize - mLength) ,VAR_ZERO) ;
		if (r1x == 0)
			return ;
		_DEBUG_ASSERT_ (mSize + r1x > mSize) ;
		auto tmp = mAllocator.expand (mSize + r1x) ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (DEPTR[tmp[i].mValue] ,_MOVE_ (_CAST_<UNIT> (mAllocator[i].mValue))) ;
			tmp[i].mNext = mAllocator[i].mNext ;
		}
		mAllocator.swap (tmp) ;
		update_reserve (mSize ,mFree) ;
	}

	inline void clean () {
		_STATIC_ASSERT_ (stl::is_nothrow_move_constructible<UNIT>::value) ;
		_STATIC_ASSERT_ (stl::is_nothrow_move_assignable<UNIT>::value) ;
		const auto r1x = shrink_size () ;
		if (r1x == mSize)
			return ;
		_DYNAMIC_ASSERT_ (r1x == mLength) ;
		auto tmp = mAllocator.expand (r1x) ;
		for (auto &&i : _RANGE_ (0 ,tmp.size ())) {
			_DEBUG_ASSERT_ (mAllocator[i].mNext == VAR_USED) ;
			_CREATE_ (DEPTR[tmp[i].mValue] ,_MOVE_ (_CAST_<UNIT> (mAllocator[i].mValue))) ;
			tmp[i].mNext = VAR_USED ;
		}
		mAllocator.swap (tmp) ;
		update_reserve (r1x ,VAR_NONE) ;
	}

private:
	inline void update_reserve (const INDEX &size_ ,const INDEX &free_) noexcept {
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
		return _MOVE_ (ret) ;
	}
} ;
} ;