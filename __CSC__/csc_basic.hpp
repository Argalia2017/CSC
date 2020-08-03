#pragma once

#ifndef __CSC_BASIC__
#define __CSC_BASIC__
#endif

#include "csc.hpp"
#include "csc_core.hpp"

namespace CSC {
class BasicProc
	:private Wrapped<> {
public:
	template <class _ARG1>
	imports BOOL mem_equal (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) ;

	template <class _ARG1>
	imports FLAG mem_compr (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) ;

	template <class _ARG1>
	imports FLAG mem_hash (const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	imports FLAG mem_crc32 (const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	imports INDEX mem_chr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) ;

	template <class _ARG1>
	imports INDEX mem_rchr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) ;

	template <class _ARG1>
	imports void mem_copy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	imports void mem_rcopy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) ;

	template <class _ARG1>
	imports void mem_move (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) ;

	template <class _ARG1>
	imports void mem_swap (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) ;

	template <class _ARG1>
	imports void mem_fill (ARR<_ARG1> &dst ,const LENGTH &len ,const _ARG1 &val) ;
} ;

class EqualInvokeProc
	:private Wrapped<> {
public:
	template <class _ARG1>
	imports FLAG invoke (const _ARG1 &lhs ,const _ARG1 &rhs) {
		return template_equal (lhs ,rhs ,ARGVPX) ;
	}

private:
	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<DEF<decltype (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null).equal (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null)))> ,BOOL>::value)>>
	imports FLAG template_equal (const _ARG1 &lhs ,const _ARG1 &rhs ,const DEF<decltype (ARGVP3)> &) {
		return lhs.equal (rhs) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<DEF<decltype (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null) == _NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null))> ,BOOL>::value)>>
	imports FLAG template_equal (const _ARG1 &lhs ,const _ARG1 &rhs ,const DEF<decltype (ARGVP2)> &) {
		return BOOL (lhs == rhs) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_TRIVIAL_HELP<_ARG1>::value)>>
	imports FLAG template_equal (const _ARG1 &lhs ,const _ARG1 &rhs ,const DEF<decltype (ARGVP1)> &) {
		//@error: not strict good because of alignment
		auto &r1x = _CAST_ (ARGV<BYTE[_SIZEOF_ (_ARG1)]>::null ,lhs) ;
		auto &r2x = _CAST_ (ARGV<BYTE[_SIZEOF_ (_ARG1)]>::null ,rhs) ;
		return BasicProc::mem_equal (PTRTOARR[r1x] ,PTRTOARR[r2x] ,_SIZEOF_ (_ARG1)) ;
	}
} ;

template <class _ARG1>
inline exports BOOL BasicProc::mem_equal (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (src1 == src2)
		return TRUE ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		if (!EqualInvokeProc::invoke (src1[i] ,src2[i]))
			return FALSE ;
	}
	return TRUE ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

class ComprInvokeProc
	:private Wrapped<> {
public:
	template <class _ARG1>
	imports FLAG invoke (const _ARG1 &lhs ,const _ARG1 &rhs) {
		return template_compr (lhs ,rhs ,ARGVPX) ;
	}

private:
	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<DEF<decltype (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null).compr (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null)))> ,FLAG>::value)>>
	imports FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const DEF<decltype (ARGVP3)> &) {
		return lhs.compr (rhs) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<DEF<decltype (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null) < _NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null))> ,BOOL>::value)>>
		imports FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const DEF<decltype (ARGVP2)> &) {
			if (lhs < rhs)
				return FLAG (-1) ;
			if (rhs < lhs)
				return FLAG (+1) ;
			return FLAG (0) ;
		}

		template <class _ARG1 ,class = ENABLE_TYPE<(IS_TRIVIAL_HELP<_ARG1>::value)>>
		imports FLAG template_compr (const _ARG1 &lhs ,const _ARG1 &rhs ,const DEF<decltype (ARGVP1)> &) {
			//@error: not strict good because of alignment
			auto &r1x = _CAST_ (ARGV<BYTE[_SIZEOF_ (_ARG1)]>::null ,lhs) ;
			auto &r2x = _CAST_ (ARGV<BYTE[_SIZEOF_ (_ARG1)]>::null ,rhs) ;
			return BasicProc::mem_compr (PTRTOARR[r1x] ,PTRTOARR[r2x] ,_SIZEOF_ (_ARG1)) ;
		}

} ;

template <class _ARG1>
inline exports FLAG BasicProc::mem_compr (const ARR<_ARG1> &src1 ,const ARR<_ARG1> &src2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (src1 == src2)
		return FLAG (0) ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		const auto r1x = ComprInvokeProc::invoke (src1[i] ,src2[i]) ;
		if (r1x != 0)
			return r1x ;
	}
	return FLAG (0) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

class HashInvokeProc
	:private Wrapped<> {
public:
	template <class _ARG1>
	imports FLAG invoke (const _ARG1 &self_) {
		FLAG ret = template_hash (self_ ,ARGVPX) ;
		ret &= VAR_MAX ;
		return _MOVE_ (ret) ;
	}

private:
	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<DEF<decltype (_NULL_ (ARGV<const REMOVE_REFERENCE_TYPE<_ARG1>>::null).hash ())> ,FLAG>::value)>>
	imports FLAG template_hash (const _ARG1 &self_ ,const DEF<decltype (ARGVP3)> &) {
		return self_.hash () ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,BYTE>::value)>>
	imports FLAG template_hash (const _ARG1 &self_ ,const DEF<decltype (ARGVP2)> &) {
		return FLAG (self_) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_TRIVIAL_HELP<_ARG1>::value)>>
	imports FLAG template_hash (const _ARG1 &self_ ,const DEF<decltype (ARGVP1)> &) {
		//@error: not strict good because of alignment
		auto &r1x = _CAST_ (ARGV<BYTE[_SIZEOF_ (_ARG1)]>::null ,self_) ;
		return BasicProc::mem_hash (PTRTOARR[r1x] ,_SIZEOF_ (_ARG1)) ;
	}
} ;

#ifdef __CSC_CONFIG_VAR32__
template <class _ARG1>
inline exports FLAG BasicProc::mem_hash (const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	static constexpr auto M_MAGIC_N1 = VAR (-2128831035) ;
	static constexpr auto M_MAGIC_N2 = VAR (16777619) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	FLAG ret = M_MAGIC_N1 ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		ret ^= HashInvokeProc::invoke (src[i]) ;
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
inline exports FLAG BasicProc::mem_hash (const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	static constexpr auto M_MAGIC_N1 = VAR (-3750763034362895579) ;
	static constexpr auto M_MAGIC_N2 = VAR (1099511628211) ;
	_DEBUG_ASSERT_ (len >= 0) ;
	FLAG ret = M_MAGIC_N1 ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		ret ^= HashInvokeProc::invoke (src[i]) ;
		ret *= M_MAGIC_N2 ;
	}
	ret &= VAR_MAX ;
	return _MOVE_ (ret) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}
#endif

class CRC32StaticProc
	:private Wrapped<> {
public:
	imports CHAR static_mem_crc32_table_each (const CHAR &val) ;

	imports const PACK<CHAR[256]> &static_mem_crc32_table () ;
} ;

inline exports CHAR CRC32StaticProc::static_mem_crc32_table_each (const CHAR &val) {
	CHAR ret = val ;
	for (auto &&i : _RANGE_ (0 ,8)) {
		_STATIC_UNUSED_ (i) ;
		const auto r1x = CHAR (ret & CHAR (0X00000001)) ;
		ret = CHAR (ret >> 1) ;
		if (r1x == 0)
			continue ;
		ret ^= CHAR (0XEDB88320) ;
	}
	return _MOVE_ (ret) ;
}

inline exports const PACK<CHAR[256]> &CRC32StaticProc::static_mem_crc32_table () {
	return _CACHE_ ([&] () {
		PACK<CHAR[256]> ret ;
		for (auto &&i : _RANGE_ (0 ,_COUNTOF_ (DEF<decltype (ret.mP1)>)))
			ret.mP1[i] = CRC32StaticProc::static_mem_crc32_table_each (CHAR (i)) ;
		return _MOVE_ (ret) ;
	}) ;
}

template <class _ARG1>
inline exports FLAG BasicProc::mem_crc32 (const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_STATIC_ASSERT_ (IS_SAME_HELP<_ARG1 ,BYTE>::value) ;
	FLAG ret = FLAG (0XFFFFFFFF) ;
	auto &r1x = CRC32StaticProc::static_mem_crc32_table () ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		const auto r2x = CHAR ((CHAR (ret) ^ CHAR (src[i])) & CHAR (0X000000FF)) ;
		ret = FLAG (r1x.mP1[INDEX (r2x)] ^ (CHAR (ret) >> 8)) ;
	}
	ret &= VAR32_MAX ;
	return _MOVE_ (ret) ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline exports INDEX BasicProc::mem_chr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		if (EqualInvokeProc::invoke (src[i] ,val))
			return i ;
	}
	return VAR_NONE ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline exports INDEX BasicProc::mem_rchr (const ARR<_ARG1> &src ,const LENGTH &len ,const _ARG1 &val) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	for (auto &&i : _RANGE_ (0 ,len)) {
		INDEX ix = len + ~i ;
		if (EqualInvokeProc::invoke (src[ix] ,val))
			return ix ;
	}
	return VAR_NONE ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline exports void BasicProc::mem_copy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) {
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
inline exports void BasicProc::mem_rcopy (ARR<_ARG1> &dst ,const ARR<_ARG1> &src ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	auto fax = TRUE ;
	if switch_once (fax) {
		if (dst == src)
			discard ;
		_DEBUG_ASSERT_ (_ABS_ (dst - src) >= len) ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst[i] = src[len + ~i] ;
	}
	if switch_once (fax) {
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
inline exports void BasicProc::mem_move (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
	_DEBUG_ASSERT_ (len >= 0) ;
	if (dst1 == dst2)
		return ;
	auto fax = TRUE ;
	if switch_once (fax) {
		if (!(dst1 <= dst2))
			discard ;
		for (auto &&i : _RANGE_ (0 ,len))
			dst1[i] = _MOVE_ (dst2[i]) ;
	}
	if switch_once (fax) {
		for (auto &&i : _RANGE_ (0 ,len))
			dst1[len + ~i] = _MOVE_ (dst2[len + ~i]) ;
	}
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

template <class _ARG1>
inline exports void BasicProc::mem_swap (ARR<_ARG1> &dst1 ,ARR<_ARG1> &dst2 ,const LENGTH &len) {
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
inline exports void BasicProc::mem_fill (ARR<_ARG1> &dst ,const LENGTH &len ,const _ARG1 &val) {
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
	implicit ScopedPtr () = delete ;

	explicit ScopedPtr (const PTR<UNIT> &pointer)
		:ScopedPtr (ARGVP0) {
		mPointer = pointer ;
	}

	implicit ScopedPtr (const DEF<decltype (NULL)> &) = delete ;

	implicit ~ScopedPtr () noexcept {
		if (mPointer == NULL)
			return ;
		CONT::free (mPointer) ;
		mPointer = NULL ;
	}

	implicit ScopedPtr (const ScopedPtr &) = default ;

	inline ScopedPtr &operator= (const ScopedPtr &) = default ;

	implicit ScopedPtr (ScopedPtr &&) = default ;

	inline ScopedPtr &operator= (ScopedPtr &&) = default ;

	const PTR<UNIT> &to () const leftvalue {
		_DEBUG_ASSERT_ (mPointer != NULL) ;
		return mPointer ;
	}

	inline implicit operator const PTR<UNIT> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<UNIT> () const leftvalue = delete ;

	inline void operator= (const DEF<decltype (NULL)> &) leftvalue noexcept {
		mPointer = NULL ;
	}

private:
	explicit ScopedPtr (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class ScopedGuard final
	:private Proxy {
private:
	PTR<UNIT> mPointer ;

public:
	implicit ScopedGuard () = delete ;

	template <class _ARG1>
	explicit ScopedGuard (_ARG1 &address)
		:ScopedGuard (ARGVP0) {
		auto &r1x = _FORWARD_ (ARGV<UNIT>::null ,address) ;
		r1x.lock () ;
		mPointer = DEPTR[r1x] ;
	}

	implicit ~ScopedGuard () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			DEREF[mPointer].unlock () ;
		} ,[&] () {
			_DEBUG_ASSERT_ (FALSE) ;
		}) ;
		mPointer = NULL ;
	}

	implicit ScopedGuard (const ScopedGuard &) = default ;

	inline ScopedGuard &operator= (const ScopedGuard &) = default ;

	implicit ScopedGuard (ScopedGuard &&) = default ;

	inline ScopedGuard &operator= (ScopedGuard &&) = default ;

private:
	explicit ScopedGuard (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class UNIT>
class ScopedBuild final
	:private Proxy {
private:
	PTR<const PTR<TEMP<UNIT>>> mPointer ;
	LENGTH mSize ;

public:
	implicit ScopedBuild () = delete ;

	template <class _ARG1 ,class... _ARGS>
	explicit ScopedBuild (_ARG1 &address ,_ARGS &&...initval)
		:ScopedBuild (ARGVP0) {
		auto &r1x = _FORWARD_ (ARGV<PTR<TEMP<UNIT>>>::null ,address) ;
		if (r1x == NULL)
			return ;
		mPointer = DEPTR[r1x] ;
		_CREATE_ (r1x ,_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
		mSize++ ;
	}

	implicit ~ScopedBuild () noexcept {
		if (mPointer == NULL)
			return ;
		const auto r1x = _COPY_ (DEREF[mPointer]) ;
		if (r1x == NULL)
			return ;
		if switch_once (TRUE) {
			if (mSize <= 0)
				discard ;
			_DESTROY_ (r1x) ;
			mSize-- ;
		}
		mPointer = NULL ;
	}

	implicit ScopedBuild (const ScopedBuild &) = default ;

	inline ScopedBuild &operator= (const ScopedBuild &) = default ;

	implicit ScopedBuild (ScopedBuild &&) = default ;

	inline ScopedBuild &operator= (ScopedBuild &&) = default ;

private:
	explicit ScopedBuild (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class ScopedBuild<ARR<UNIT>> final
	:private Proxy {
private:
	PTR<const PTR<ARR<TEMP<UNIT>>>> mPointer ;
	LENGTH mSize ;

public:
	implicit ScopedBuild () = delete ;

	template <class _ARG1>
	explicit ScopedBuild (_ARG1 &address ,const LENGTH &len)
		:ScopedBuild (ARGVP0) {
		auto &r1x = _FORWARD_ (ARGV<PTR<ARR<TEMP<UNIT>>>>::null ,address) ;
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

	template <class _ARG1>
	explicit ScopedBuild (_ARG1 &address ,const ARR<UNIT> &src ,const LENGTH &len)
		:ScopedBuild (ARGVP0) {
		auto &r1x = _FORWARD_ (ARGV<PTR<ARR<TEMP<UNIT>>>>::null ,address) ;
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

	implicit ~ScopedBuild () noexcept {
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

	implicit ScopedBuild (const ScopedBuild &) = default ;

	inline ScopedBuild &operator= (const ScopedBuild &) = default ;

	implicit ScopedBuild (ScopedBuild &&) = default ;

	inline ScopedBuild &operator= (ScopedBuild &&) = default ;

private:
	explicit ScopedBuild (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) ,mSize (0) {}
} ;

class GlobalHeap
	:private Wrapped<> {
public:
	template <class _ARG1>
	imports ScopedPtr<_ARG1 ,GlobalHeap> alloc (const ARGVF<_ARG1> &) {
		_STATIC_ASSERT_ (IS_TRIVIAL_HELP<_ARG1>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_ARG1) <= _ALIGNOF_ (api::max_align_t)) ;
		const auto r1x = operator new (_SIZEOF_ (_ARG1) ,api::nothrow) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		const auto r2x = _POINTER_CAST_ (ARGV<_ARG1>::null ,r1x) ;
		_DYNAMIC_ASSERT_ (_ADDRESS_ (r2x) % _ALIGNOF_ (_ARG1) == 0) ;
		return ScopedPtr<_ARG1 ,GlobalHeap> (r2x) ;
	}

	template <class _ARG1>
	imports ScopedPtr<ARR<_ARG1> ,GlobalHeap> alloc (const ARGVF<_ARG1> & ,const LENGTH &len) {
		_STATIC_ASSERT_ (IS_TRIVIAL_HELP<_ARG1>::value) ;
		_STATIC_ASSERT_ (_ALIGNOF_ (_ARG1) <= _ALIGNOF_ (api::max_align_t)) ;
		_DEBUG_ASSERT_ (len > 0) ;
		const auto r1x = len * _SIZEOF_ (_ARG1) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		const auto r2x = operator new (r1x ,api::nothrow) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		const auto r3x = _POINTER_CAST_ (ARGV<ARR<_ARG1>>::null ,r2x) ;
		_DYNAMIC_ASSERT_ (_ADDRESS_ (r3x) % _ALIGNOF_ (_ARG1) == 0) ;
		return ScopedPtr<ARR<_ARG1> ,GlobalHeap> (r3x) ;
	}

	template <class _ARG1>
	imports void free (const PTR<_ARG1> &address) noexcept {
		if (address == NULL)
			return ;
		//@error: address may be different from what alloc returned
		const auto r1x = _FORWARD_ (ARGV<PTR<NONE>>::null ,address) ;
		operator delete (r1x ,api::nothrow) ;
	}
} ;

template <class>
class AutoRef ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,FALSE>> {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

protected:
	class Holder
		:public Interface {
	public:
		virtual UNIT &deref () leftvalue = 0 ;
	} ;

	struct Private {
		class PureHolder ;
	} ;

protected:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

protected:
	implicit AutoRef ()
		:AutoRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		DEREF[mPointer].~Holder () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit AutoRef (const AutoRef &) = delete ;

	inline AutoRef &operator= (const AutoRef &) = delete ;

	implicit AutoRef (AutoRef &&that) noexcept
		:AutoRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~AutoRef () ;
			new (this) AutoRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

protected:
	explicit AutoRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}
} ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,FALSE>>::Private::PureHolder
	:public Holder {
private:
	REMOVE_CVR_TYPE<UNIT> mValue ;

public:
	template <class... _ARGS>
	explicit PureHolder (_ARGS &&...initval)
		:mValue (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	UNIT &deref () leftvalue override {
		return mValue ;
	}
} ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,TRUE>> {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

protected:
	class Holder
		:public Interface {
	public:
		virtual UNIT &deref () leftvalue = 0 ;
	} ;

	struct Private {
		class PureHolder ;
	} ;

protected:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

protected:
	implicit AutoRef ()
		:AutoRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit ~AutoRef () noexcept {
		if (mPointer == NULL)
			return ;
		DEREF[mPointer].~Holder () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit AutoRef (const AutoRef &that)
		:AutoRef (ARGVP0) {
		struct Dependent ;
		using PureHolder = typename DEPENDENT_TYPE<Private ,Dependent>::PureHolder ;
		if (that.mPointer == NULL)
			return ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<PureHolder>>::null) ;
		auto &r1x = _FORWARD_ (ARGV<const UNIT>::null ,DEREF[that.mPointer].deref ()) ;
		ScopedBuild<PureHolder> ANONYMOUS (rax ,r1x) ;
		const auto r2x = _POINTER_CAST_ (ARGV<PureHolder>::null ,rax.self) ;
		mOrigin = rax.self ;
		mPointer = r2x ;
		rax = NULL ;
	}

	inline AutoRef &operator= (const AutoRef &that) {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~AutoRef () ;
			new (this) AutoRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	implicit AutoRef (AutoRef &&that) noexcept
		:AutoRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline AutoRef &operator= (AutoRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~AutoRef () ;
			new (this) AutoRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

protected:
	explicit AutoRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}
} ;

template <class UNIT>
class AutoRef<SPECIALIZATION<UNIT ,TRUE>>::Private::PureHolder
	:public Holder {
private:
	REMOVE_CVR_TYPE<UNIT> mValue ;

public:
	template <class... _ARGS>
	explicit PureHolder (_ARGS &&...initval)
		:mValue (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	UNIT &deref () leftvalue override {
		return mValue ;
	}
} ;

template <class UNIT>
class AutoRef final
	:private AutoRef<SPECIALIZATION<UNIT ,(IS_COPY_CONSTRUCTIBLE_HELP<UNIT>::value && api::is_nothrow_move_constructible<UNIT>::value)>> {
private:
	using SPECIALIZATION_BASE = AutoRef<SPECIALIZATION<UNIT ,(IS_COPY_CONSTRUCTIBLE_HELP<UNIT>::value && api::is_nothrow_move_constructible<UNIT>::value)>> ;
	using Holder = typename SPECIALIZATION_BASE::Holder ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mOrigin ;
	using SPECIALIZATION_BASE::mPointer ;

public:
	implicit AutoRef () = default ;

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	UNIT &to () leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].deref () ;
	}

	inline PTR<UNIT> operator-> () leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator UNIT & () leftvalue {
		return self ;
	}

	const UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].deref () ;
	}

	inline PTR<const UNIT> operator-> () const leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator const UNIT & () const leftvalue {
		return self ;
	}

	template <class... _ARGS>
	imports AutoRef make (_ARGS &&...initval) {
		using PureHolder = typename SPECIALIZATION_BASE::Private::PureHolder ;
		AutoRef ret ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<PureHolder>>::null) ;
		ScopedBuild<PureHolder> ANONYMOUS (rax ,_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
		const auto r1x = _POINTER_CAST_ (ARGV<PureHolder>::null ,rax.self) ;
		ret.mOrigin = rax.self ;
		ret.mPointer = r1x ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}
} ;

template <class UNIT>
class SharedRef final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	class Holder
		:public Interface {
	public:
		virtual UNIT &deref () leftvalue = 0 ;
		virtual LENGTH increase () = 0 ;
		virtual LENGTH decrease () = 0 ;
	} ;

	struct Private {
		class PureHolder ;
	} ;

private:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

public:
	implicit SharedRef ()
		:SharedRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit ~SharedRef () noexcept {
		if (mPointer == NULL)
			return ;
		if switch_once (TRUE) {
			const auto r1x = DEREF[mPointer].decrease () ;
			if (r1x > 0)
				discard ;
			DEREF[mPointer].~Holder () ;
			GlobalHeap::free (mOrigin) ;
		}
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit SharedRef (const SharedRef &that)
		:SharedRef (ARGVP0) {
		aquire (that.mPointer) ;
		mOrigin = that.mOrigin ;
		mPointer = that.mPointer ;
	}

	inline SharedRef &operator= (const SharedRef &that) {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~SharedRef () ;
			new (this) SharedRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	implicit SharedRef (SharedRef &&that) noexcept
		:SharedRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline SharedRef &operator= (SharedRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~SharedRef () ;
			new (this) SharedRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].deref () ;
	}

	inline PTR<UNIT> operator-> () const leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator UNIT & () const leftvalue {
		return self ;
	}

	template <class... _ARGS>
	imports SharedRef make (_ARGS &&...initval) {
		struct Dependent ;
		using PureHolder = typename DEPENDENT_TYPE<Private ,Dependent>::PureHolder ;
		SharedRef ret ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<PureHolder>>::null) ;
		ScopedBuild<PureHolder> ANONYMOUS (rax ,_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
		const auto r1x = _POINTER_CAST_ (ARGV<PureHolder>::null ,rax.self) ;
		aquire (r1x) ;
		ret.mOrigin = rax.self ;
		ret.mPointer = r1x ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	explicit SharedRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}

	imports void aquire (const PTR<Holder> &pointer) {
		if (pointer == NULL)
			return ;
		const auto r1x = DEREF[pointer].increase () ;
		_STATIC_UNUSED_ (r1x) ;
		_DEBUG_ASSERT_ (r1x >= 1) ;
	}
} ;

template <class UNIT>
class SharedRef<UNIT>::Private::PureHolder
	:public Holder {
private:
	REMOVE_CVR_TYPE<UNIT> mValue ;
	LENGTH mCounter ;

public:
	template <class... _ARGS>
	explicit PureHolder (_ARGS &&...initval)
		:mValue (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ,mCounter (0) {}

	UNIT &deref () leftvalue override {
		return mValue ;
	}

	LENGTH increase () override {
		return ++mCounter ;
	}

	LENGTH decrease () override {
		return --mCounter ;
	}
} ;

template <class UNIT = NONE>
class AnyRef ;

template <>
class AnyRef<NONE> final {
private:
	class Holder
		:public Interface {
	public:
		virtual TYPEABI type_abi () const = 0 ;
		virtual FLAG type_mid () const = 0 ;
		virtual PTR<NONE> type_address () = 0 ;
		virtual PTR<const NONE> type_address () const = 0 ;
	} ;

private:
	template <class>
	friend class AnyRef ;
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

public:
	implicit AnyRef ()
		:AnyRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	implicit AnyRef (AnyRef<_ARG1> &&that)
		: AnyRef (_MOVE_ (that.rebind (ARGV<NONE>::null))) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		DEREF[mPointer].~Holder () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit AnyRef (const AnyRef &) = delete ;

	inline AnyRef &operator= (const AnyRef &) = delete ;

	implicit AnyRef (AnyRef &&that) noexcept
		:AnyRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~AnyRef () ;
			new (this) AnyRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	template <class _ARG1>
	AnyRef<_ARG1> &rebind (const ARGVF<_ARG1> &) leftvalue {
		return _CAST_ (ARGV<AnyRef<_ARG1>>::null ,DEREF[this]) ;
	}

	template <class _ARG1>
	const AnyRef<_ARG1> &rebind (const ARGVF<_ARG1> &) const leftvalue {
		return _CAST_ (ARGV<AnyRef<_ARG1>>::null ,DEREF[this]) ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	TYPEABI type_abi () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_abi () ;
	}

	FLAG type_mid () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_mid () ;
	}

	PTR<NONE> type_address () {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_address () ;
	}

	PTR<const NONE> type_address () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_address () ;
	}

private:
	explicit AnyRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}
} ;

template <class UNIT>
class AnyRef final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	using Holder = typename AnyRef<NONE>::Holder ;

	struct Private {
		template <class>
		class ImplHolder ;
	} ;

private:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

public:
	implicit AnyRef ()
		:AnyRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	implicit AnyRef (AnyRef<_ARG1> &&that)
		: AnyRef (_MOVE_ (that.rebind (ARGV<UNIT>::null))) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit ~AnyRef () noexcept {
		if (mPointer == NULL)
			return ;
		DEREF[mPointer].~Holder () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit AnyRef (const AnyRef &) = delete ;

	inline AnyRef &operator= (const AnyRef &) = delete ;

	implicit AnyRef (AnyRef &&that) noexcept
		:AnyRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline AnyRef &operator= (AnyRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~AnyRef () ;
			new (this) AnyRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	template <class _ARG1>
	AnyRef<_ARG1> &rebind (const ARGVF<_ARG1> &) leftvalue {
		return _CAST_ (ARGV<AnyRef<_ARG1>>::null ,DEREF[this]) ;
	}

	template <class _ARG1>
	const AnyRef<_ARG1> &rebind (const ARGVF<_ARG1> &) const leftvalue {
		return _CAST_ (ARGV<AnyRef<_ARG1>>::null ,DEREF[this]) ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	LENGTH type_abi () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_abi () ;
	}

	FLAG type_mid () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_mid () ;
	}

	PTR<NONE> type_address () {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_address () ;
	}

	PTR<const NONE> type_address () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].type_address () ;
	}

	UNIT &to () leftvalue {
		struct Dependent ;
		_DEBUG_ASSERT_ (type_mid () == _TYPEMID_ (ARGV<UNIT>::null)) ;
		const auto r1x = DEREF[mPointer].type_address () ;
		const auto r2x = _POINTER_CAST_ (ARGV<UNIT>::null ,r1x) ;
		return DEREF[r2x] ;
	}

	inline PTR<UNIT> operator-> () leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator UNIT & () leftvalue {
		return self ;
	}

	const UNIT &to () const leftvalue {
		struct Dependent ;
		_DEBUG_ASSERT_ (type_mid () == _TYPEMID_ (ARGV<UNIT>::null)) ;
		const auto r1x = DEREF[mPointer].type_address () ;
		const auto r2x = _POINTER_CAST_ (ARGV<UNIT>::null ,r1x) ;
		return DEREF[r2x] ;
	}

	inline PTR<const UNIT> operator-> () const leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator const UNIT & () const leftvalue {
		return self ;
	}

	template <class... _ARGS>
	imports AnyRef make (_ARGS &&...initval) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<UNIT> ;
		AnyRef ret ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<ImplHolder>>::null) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
		const auto r1x = _POINTER_CAST_ (ARGV<ImplHolder>::null ,rax.self) ;
		ret.mOrigin = rax.self ;
		ret.mPointer = r1x ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	explicit AnyRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}
} ;

template <class UNIT>
template <class UNIT_>
class AnyRef<UNIT>::Private::ImplHolder
	:public Holder {
private:
	UNIT_ mValue ;

public:
	template <class... _ARGS>
	explicit ImplHolder (_ARGS &&...initval)
		:mValue (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	TYPEABI type_abi () const override {
		return _TYPEABI_ (ARGV<UNIT_>::null) ;
	}

	FLAG type_mid () const override {
		return _TYPEMID_ (ARGV<UNIT_>::null) ;
	}

	PTR<NONE> type_address () override {
		return DEPTR[mValue] ;
	}

	PTR<const NONE> type_address () const override {
		return DEPTR[mValue] ;
	}
} ;

template <class>
class Function ;

template <class UNIT = NONE>
class UniqueRef ;

template <>
class UniqueRef<NONE> final {
private:
	class Holder
		:public Interface {
	public:
		virtual void release () = 0 ;
	} ;

	struct Private {
		template <class>
		class ImplHolder ;
	} ;

private:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

public:
	implicit UniqueRef ()
		:UniqueRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<(!IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<decltype (ARGVP0)>>::value)>>
	explicit UniqueRef (const _ARG1 &constructor ,_ARG2 &&destructor)
		: UniqueRef (ARGVP0) {
		struct Dependent ;
		using HINT_T1 = DEF<void ()> ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<PTR<HINT_T1>> ;
		using Function = DEPENDENT_TYPE<Function<HINT_T1> ,Dependent> ;
		_STATIC_ASSERT_ (IS_VOID_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<>>>::value) ;
		_STATIC_ASSERT_ (!IS_REFERENCE_HELP<_ARG2>::value) ;
		_STATIC_ASSERT_ (IS_VOID_HELP<RESULT_OF_TYPE<_ARG2 ,ARGVS<>>>::value) ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<ImplHolder>>::null) ;
		const auto r1x = Function (_FORWARD_ (ARGV<_ARG2 &&>::null ,destructor)) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,DEPTR[r1x.self]) ;
		const auto r2x = _POINTER_CAST_ (ARGV<ImplHolder>::null ,rax.self) ;
		constructor () ;
		mOrigin = rax.self ;
		mPointer = r2x ;
		rax = NULL ;
	}

	implicit ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			DEREF[mPointer].release () ;
		} ,[&] () {
			_DEBUG_ASSERT_ (FALSE) ;
		}) ;
		DEREF[mPointer].~Holder () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit UniqueRef (const UniqueRef &) = delete ;

	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	implicit UniqueRef (UniqueRef &&that) noexcept
		:UniqueRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~UniqueRef () ;
			new (this) UniqueRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

private:
	explicit UniqueRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}
} ;

template <class UNIT_>
class UniqueRef<>::Private::ImplHolder
	:public Holder {
private:
	UNIT_ mFunctor ;

public:
	template <class... _ARGS>
	explicit ImplHolder (_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	void release () override {
		mFunctor () ;
	}
} ;

template <class UNIT>
class UniqueRef final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	class Holder
		:public Interface {
	public:
		virtual UNIT &deref () leftvalue = 0 ;
		virtual void release () = 0 ;
	} ;

	struct Private {
		template <class>
		class ImplHolder ;
	} ;

private:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;

public:
	implicit UniqueRef ()
		:UniqueRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<(!IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<decltype (ARGVP0)>>::value)>>
	explicit UniqueRef (const _ARG1 &constructor ,_ARG2 &&destructor)
		: UniqueRef (ARGVP0) {
		struct Dependent ;
		using HINT_T1 = DEF<void (UNIT &)> ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<PTR<HINT_T1>> ;
		using Function = DEPENDENT_TYPE<Function<HINT_T1> ,Dependent> ;
		_STATIC_ASSERT_ (IS_VOID_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNIT &>>>::value) ;
		_STATIC_ASSERT_ (!IS_REFERENCE_HELP<_ARG2>::value) ;
		_STATIC_ASSERT_ (IS_VOID_HELP<RESULT_OF_TYPE<_ARG2 ,ARGVS<UNIT &>>>::value) ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<ImplHolder>>::null) ;
		const auto r1x = Function (_FORWARD_ (ARGV<_ARG2 &&>::null ,destructor)) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,DEPTR[r1x.self]) ;
		const auto r2x = _POINTER_CAST_ (ARGV<ImplHolder>::null ,rax.self) ;
		constructor (DEREF[r2x].deref ()) ;
		mOrigin = rax.self ;
		mPointer = r2x ;
		rax = NULL ;
	}

	implicit ~UniqueRef () noexcept {
		if (mPointer == NULL)
			return ;
		_CALL_TRY_ ([&] () {
			DEREF[mPointer].release () ;
		} ,[&] () {
			_DEBUG_ASSERT_ (FALSE) ;
		}) ;
		DEREF[mPointer].~Holder () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mPointer = NULL ;
	}

	implicit UniqueRef (const UniqueRef &) = delete ;

	inline UniqueRef &operator= (const UniqueRef &) = delete ;

	implicit UniqueRef (UniqueRef &&that) noexcept
		: UniqueRef (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline UniqueRef &operator= (UniqueRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~UniqueRef () ;
			new (this) UniqueRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	const UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer].deref () ;
	}

	inline PTR<const UNIT> operator-> () const leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator const UNIT & () const leftvalue {
		return self ;
	}

	template <class... _ARGS>
	imports UniqueRef make (_ARGS &&...initval) {
		struct Dependent ;
		using HINT_T1 = DEF<void (UNIT &)> ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<PTR<HINT_T1>> ;
		using Function = DEPENDENT_TYPE<Function<HINT_T1> ,Dependent> ;
		UniqueRef ret ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<ImplHolder>>::null) ;
		const auto r1x = Function ([] (UNIT &) {}) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,DEPTR[r1x.self]) ;
		const auto r2x = _POINTER_CAST_ (ARGV<ImplHolder>::null ,rax.self) ;
		auto &r3x = DEREF[r2x].deref () ;
		r3x = UNIT (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
		ret.mOrigin = rax.self ;
		ret.mPointer = r2x ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	explicit UniqueRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) {}
} ;

template <class UNIT>
template <class UNIT_>
class UniqueRef<UNIT>::Private::ImplHolder
	:public Holder {
private:
	REMOVE_CVR_TYPE<UNIT> mValue ;
	UNIT_ mFunctor ;

public:
	template <class... _ARGS>
	explicit ImplHolder (_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	UNIT &deref () leftvalue override {
		return mValue ;
	}

	void release () override {
		mFunctor (mValue) ;
	}
} ;

template <class UNIT>
class PhanRef final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	PTR<UNIT> mPointer ;

public:
	implicit PhanRef ()
		:PhanRef (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit ~PhanRef () noexcept {
		if (mPointer == NULL)
			return ;
		mPointer = NULL ;
	}

	implicit PhanRef (const PhanRef &) = delete ;

	inline PhanRef &operator= (const PhanRef &) = delete ;

	implicit PhanRef (PhanRef &&that) noexcept
		:PhanRef (ARGVP0) {
		_SWAP_ (mPointer ,that.mPointer) ;
	}

	inline PhanRef &operator= (PhanRef &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~PhanRef () ;
			new (this) PhanRef (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	UNIT &to () const leftvalue {
		_DEBUG_ASSERT_ (exist ()) ;
		return DEREF[mPointer] ;
	}

	inline PTR<UNIT> operator-> () const leftvalue {
		return DEPTR[self] ;
	}

	inline implicit operator UNIT & () const leftvalue {
		return self ;
	}

	//@warn: phantom means deliver pointer without holder
	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<UNIT>>::value)>>
	imports PhanRef make (_ARG1 &val) {
		PhanRef ret ;
		ret.mPointer = DEPTR[val] ;
		return _MOVE_ (ret) ;
	}

	template <class _ARG1>
	imports PhanRef make (const PhanRef<_ARG1> &val) {
		if (!val.exist ())
			return PhanRef () ;
		auto &r1x = _FORWARD_ (ARGV<UNIT>::null ,val.self) ;
		return make (r1x) ;
	}

private:
	explicit PhanRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) {}
} ;

template <class>
class Function ;

class FunctorInvokeProc
	:public Wrapped<> {
public:
	template <class _ARG1 ,class _ARG2>
	imports PTR<_ARG1> invoke (const ARGVF<_ARG1> & ,_ARG2 &functor) {
		return template_functor (ARGV<_ARG1>::null ,functor ,ARGVPX) ;
	}

private:
	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<(IS_IMPLICIT_CONVERTIBLE_HELP<_ARG2 ,PTR<_ARG1>>::value)>>
	imports PTR<_ARG1> template_functor (const ARGVF<_ARG1> & ,_ARG2 &functor ,const DEF<decltype (ARGVP2)> &) {
		return _FORWARD_ (ARGV<PTR<_ARG1>>::null ,functor) ;
	}

	template <class _ARG1 ,class _ARG2>
	imports PTR<_ARG1> template_functor (const ARGVF<_ARG1> & ,_ARG2 &functor ,const DEF<decltype (ARGVP1)> &) {
		return NULL ;
	}
} ;

template <class UNIT1 ,class... UNITS>
class Function<UNIT1 (UNITS...)> final {
private:
	class Holder
		:public Interface {
	public:
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const = 0 ;
	} ;

	struct Private {
		template <class>
		class ImplHolder ;
	} ;

private:
	PTR<NONE> mOrigin ;
	PTR<Holder> mPointer ;
	PTR<UNIT1 (UNITS...)> mFunctor ;

public:
	implicit Function ()
		:Function (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit Function (const DEF<UNIT1 (UNITS...)> &that)
		: Function (ARGVP0) {
		mFunctor = DEPTR[that] ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(!IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,Function>::value && !IS_SAME_HELP<REMOVE_CVR_TYPE<_ARG1> ,REMOVE_CVR_TYPE<decltype (ARGVP0)>>::value)>>
	implicit Function (_ARG1 &&that)
		: Function (ARGVP0) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<REMOVE_REFERENCE_TYPE<_ARG1>> ;
		_STATIC_ASSERT_ (IS_SAME_HELP<RESULT_OF_TYPE<_ARG1 ,ARGVS<UNITS...>> ,UNIT1>::value) ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<ImplHolder>>::null) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,_FORWARD_ (ARGV<_ARG1 &&>::null ,that)) ;
		const auto r1x = _POINTER_CAST_ (ARGV<ImplHolder>::null ,rax.self) ;
		mOrigin = rax.self ;
		mPointer = r1x ;
		mFunctor = FunctorInvokeProc::invoke (ARGV<UNIT1 (UNITS...)>::null ,DEREF[r1x].deref ()) ;
		rax = NULL ;
	}

	implicit ~Function () noexcept {
		if (mPointer == NULL)
			if (mFunctor == NULL)
				return ;
		if switch_once (TRUE) {
			if (mPointer == NULL)
				discard ;
			DEREF[mPointer].~Holder () ;
			GlobalHeap::free (mOrigin) ;
		}
		mOrigin = NULL ;
		mPointer = NULL ;
		mFunctor = NULL ;
	}

	implicit Function (const Function &) = delete ;

	inline Function &operator= (const Function &) = delete ;

	implicit Function (Function &&that) noexcept
		:Function (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mPointer ,that.mPointer) ;
		_SWAP_ (mFunctor ,that.mFunctor) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Function () ;
			new (this) Function (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	BOOL exist () const {
		if (mPointer == NULL)
			if (mFunctor == NULL)
				return FALSE ;
		return TRUE ;
	}

	const DEF<UNIT1 (UNITS...)> &to () const leftvalue {
		_DEBUG_ASSERT_ (mFunctor != NULL) ;
		return DEREF[mFunctor] ;
	}

	inline implicit operator const DEF<UNIT1 (UNITS...)> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<UNIT1 (UNITS...)> () const leftvalue = delete ;

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		_DEBUG_ASSERT_ (exist ()) ;
		if (mFunctor != NULL)
			return mFunctor (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
		return DEREF[mPointer].invoke (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		return invoke (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}

	template <class... _ARGS>
	imports Function make (const DEF<UNIT1 (UNITS... ,_ARGS...)> &functor ,const REMOVE_CVR_TYPE<_ARGS> &...parameter) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<UNIT1 (UNITS... ,_ARGS...)> ;
		Function ret ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<ImplHolder>>::null) ;
		ScopedBuild<ImplHolder> ANONYMOUS (rax ,functor ,parameter...) ;
		const auto r1x = _POINTER_CAST_ (ARGV<ImplHolder>::null ,rax.self) ;
		ret.mOrigin = rax.self ;
		ret.mPointer = r1x ;
		ret.mFunctor = NULL ;
		rax = NULL ;
		return _MOVE_ (ret) ;
	}

private:
	explicit Function (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mPointer (NULL) ,mFunctor (NULL) {}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<UNIT1 (UNITS...)>::Private::ImplHolder
	:public Holder {
	_STATIC_ASSERT_ (IS_SAME_HELP<RESULT_OF_TYPE<UNIT_ ,ARGVS<UNITS...>> ,UNIT1>::value) ;

private:
	UNIT_ mFunctor ;

public:
	template <class... _ARGS>
	explicit ImplHolder (_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	UNIT_ &deref () leftvalue {
		return mFunctor ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return mFunctor (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
class Function<MEMPTR<UNIT1 (UNITS...)>> final {
#pragma push_macro ("fake")
#undef fake
#define fake m_fake ()

private:
	class FakeHolder ;

	class Holder
		:public Interface {
	public:
		virtual void friend_move (const PTR<TEMP<FakeHolder>> &address) = 0 ;
		virtual UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const = 0 ;
	} ;

	class FakeHolder
		:public Holder {
	private:
		PTR<NONE> mContext ;
		MEMPTR<UNIT1 (UNITS...)> mFunctor ;
	} ;

	struct Private {
		class PureHolder ;

		template <class ,class>
		class ImplHolder ;
	} ;

private:
	TEMP<FakeHolder> mVariant ;

public:
	implicit Function ()
		:Function (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit Function (const DEF<UNIT1 (UNITS...)> &that)
		: Function (ARGVP0) {
		struct Dependent ;
		using PureHolder = typename DEPENDENT_TYPE<Private ,Dependent>::PureHolder ;
		static_create (ARGV<PureHolder>::null ,DEPTR[mVariant] ,that) ;
	}

	template <class _ARG1>
	explicit Function (PhanRef<_ARG1> &&context_ ,const MEMPTR<DEF<UNIT1 (UNITS...)> ,_ARG1> &functor)
		:Function (ARGVP0) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<_ARG1 ,ARGC<1>> ;
		static_create (ARGV<ImplHolder>::null ,DEPTR[mVariant] ,_MOVE_ (context_) ,functor) ;
	}

	template <class _ARG1>
	explicit Function (PhanRef<const _ARG1> &&context_ ,const MEMPTR<DEF<UNIT1 (UNITS...) const> ,_ARG1> &functor)
		:Function (ARGVP0) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<_ARG1 ,ARGC<2>> ;
		static_create (ARGV<ImplHolder>::null ,DEPTR[mVariant] ,_MOVE_ (context_) ,functor) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<(!IS_FUNCTION_HELP<_ARG2>::value)>>
	explicit Function (PhanRef<_ARG1> &&context_ ,const MEMPTR<_ARG2 ,_ARG1> &functor)
		:Function (ARGVP0) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<_ARG1 ,ARGC<3>> ;
		static_create (ARGV<ImplHolder>::null ,DEPTR[mVariant] ,_MOVE_ (context_) ,functor) ;
	}

	template <class _ARG1>
	explicit Function (PhanRef<_ARG1> &&context_ ,const DEF<UNIT1 (_ARG1 & ,UNITS...)> &functor)
		:Function (ARGVP0) {
		struct Dependent ;
		using ImplHolder = typename DEPENDENT_TYPE<Private ,Dependent>::template ImplHolder<_ARG1 ,ARGC<4>> ;
		static_create (ARGV<ImplHolder>::null ,DEPTR[mVariant] ,_MOVE_ (context_) ,functor) ;
	}

	implicit ~Function () noexcept {
		if (!exist ())
			return ;
		fake.~Holder () ;
		_ZERO_ (mVariant) ;
	}

	implicit Function (const Function &) = delete ;

	inline Function &operator= (const Function &) = delete ;

	implicit Function (Function &&that) noexcept
		:Function (ARGVP0) {
		if (!that.exist ())
			return ;
		that.fake.friend_move (DEPTR[mVariant]) ;
	}

	inline Function &operator= (Function &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Function () ;
			new (this) Function (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	BOOL exist () const {
		auto &r1x = _FORWARD_ (ARGV<Interface>::null ,fake) ;
		const auto r2x = _CAST_ (ARGV<FLAG>::null ,r1x) ;
		if (r2x == VAR_ZERO)
			return FALSE ;
		return TRUE ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		_DEBUG_ASSERT_ (exist ()) ;
		return fake.invoke (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}

	inline UNIT1 operator() (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const {
		return invoke (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}

private:
	explicit Function (const DEF<decltype (ARGVP0)> &) noexcept {
		_ZERO_ (mVariant) ;
	}

	inline Holder &m_fake () leftvalue {
		return _CAST_ (ARGV<FakeHolder>::null ,mVariant) ;
	}

	inline const Holder &m_fake () const leftvalue {
		return _CAST_ (ARGV<FakeHolder>::null ,mVariant) ;
	}

	template <class _ARG1 ,class... _ARGS>
	imports void static_create (const ARGVF<_ARG1> & ,const PTR<TEMP<FakeHolder>> &address ,_ARGS &&...funcval) {
		_STATIC_ASSERT_ (IS_CONSTRUCTIBLE_HELP<_ARG1 ,_ARGS &&...>::value) ;
		const auto r1x = _POINTER_CAST_ (ARGV<TEMP<_ARG1>>::null ,address) ;
		auto &r2x = _FORWARD_ (ARGV<Holder>::null ,_CAST_ (ARGV<_ARG1>::null ,DEREF[r1x])) ;
		auto &r3x = _FORWARD_ (ARGV<Holder>::null ,_CAST_ (ARGV<FakeHolder>::null ,DEREF[address])) ;
		_DYNAMIC_ASSERT_ (DEPTR[r2x] == DEPTR[r3x]) ;
		_CREATE_ (r1x ,_FORWARD_ (ARGV<_ARGS &&>::null ,funcval)...) ;
	}

#pragma pop_macro ("fake")
} ;

template <class UNIT1 ,class... UNITS>
class Function<MEMPTR<UNIT1 (UNITS...)>>::Private::PureHolder
	:public Holder {
private:
	Function<UNIT1 (UNITS...)> mFunctor ;

public:
	template <class... _ARGS>
	explicit PureHolder (_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) {}

	void friend_move (const PTR<TEMP<FakeHolder>> &address) override {
		static_create (ARGV<PureHolder>::null ,address ,_MOVE_ (mFunctor)) ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return mFunctor (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<MEMPTR<UNIT1 (UNITS...)>>::Private::ImplHolder<UNIT_ ,ARGC<1>>
	:public Holder {
private:
	MEMPTR<DEF<UNIT1 (UNITS...)> ,UNIT_> mFunctor ;
	PhanRef<UNIT_> mContext ;

public:
	template <class... _ARGS>
	explicit ImplHolder (PhanRef<UNIT_> &&context_ ,_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ,mContext (_MOVE_ (context_)) {}

	void friend_move (const PTR<TEMP<FakeHolder>> &address) override {
		static_create (ARGV<ImplHolder>::null ,address ,_MOVE_ (mContext) ,_MOVE_ (mFunctor)) ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		auto &r1x = mContext.self ;
		return (r1x.*mFunctor) (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<MEMPTR<UNIT1 (UNITS...)>>::Private::ImplHolder<UNIT_ ,ARGC<2>>
	:public Holder {
private:
	MEMPTR<DEF<UNIT1 (UNITS...) const> ,UNIT_> mFunctor ;
	PhanRef<const UNIT_> mContext ;

public:
	template <class... _ARGS>
	explicit ImplHolder (PhanRef<const UNIT_> &&context_ ,_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ,mContext (_MOVE_ (context_)) {}

	void friend_move (const PTR<TEMP<FakeHolder>> &address) override {
		static_create (ARGV<ImplHolder>::null ,address ,_MOVE_ (mContext) ,_MOVE_ (mFunctor)) ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		auto &r1x = mContext.self ;
		return (r1x.*mFunctor) (_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<MEMPTR<UNIT1 (UNITS...)>>::Private::ImplHolder<UNIT_ ,ARGC<3>>
	:public Holder {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) == 0) ;

private:
	MEMPTR<REMOVE_REFERENCE_TYPE<UNIT1> ,UNIT_> mFunctor ;
	PhanRef<UNIT_> mContext ;

public:
	template <class... _ARGS>
	explicit ImplHolder (PhanRef<UNIT_> &&context_ ,_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ,mContext (_MOVE_ (context_)) {}

	void friend_move (const PTR<TEMP<FakeHolder>> &address) override {
		static_create (ARGV<ImplHolder>::null ,address ,_MOVE_ (mContext) ,_MOVE_ (mFunctor)) ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		auto &r1x = mContext.self ;
		return (r1x.*mFunctor) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class UNIT_>
class Function<MEMPTR<UNIT1 (UNITS...)>>::Private::ImplHolder<UNIT_ ,ARGC<4>>
	:public Holder {
private:
	Function<UNIT1 (UNIT_ & ,UNITS...)> mFunctor ;
	PhanRef<UNIT_> mContext ;

public:
	template <class... _ARGS>
	explicit ImplHolder (PhanRef<UNIT_> &&context_ ,_ARGS &&...initval)
		:mFunctor (_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ,mContext (_MOVE_ (context_)) {}

	void friend_move (const PTR<TEMP<FakeHolder>> &address) override {
		static_create (ARGV<ImplHolder>::null ,address ,_MOVE_ (mContext) ,_MOVE_ (mFunctor)) ;
	}

	UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return mFunctor (mContext.self ,_FORWARD_ (ARGV<FORWARD_TRAITS_TYPE<UNITS> &&>::null ,funcval)...) ;
	}
} ;

template <class ,class>
class Buffer ;

using SFLEX = ZERO ;
using SFIXED = ARGC<(-1)> ;
using SAUTO = ARGC<(-2)> ;
using SCPHAN = ARGC<(-4)> ;
using SMPHAN = ARGC<(-5)> ;

template <class UNIT ,class SIZE>
class Buffer {
	_STATIC_ASSERT_ (SIZE::value > 0) ;
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
#ifdef __CSC_COMPILER_MSVC__
	//@error: fuck vs2017
	template <class ,class>
	friend class Buffer ;
#endif
	DEF<UNIT[SIZE::value]> mBuffer ;

public:
	implicit Buffer () = default ;

	explicit Buffer (const LENGTH &len) {
		_DEBUG_ASSERT_ (len >= 0 && len <= SIZE::value) ;
	}

	implicit Buffer (const DEF<UNIT[SIZE::value]> &that)
		:Buffer (_MOVE_ (_CAST_ (ARGV<Buffer>::null ,that))) {
		_STATIC_WARNING_ ("noop") ;
	}

	implicit Buffer (DEF<UNIT[SIZE::value]> &&that)
		: Buffer (_MOVE_ (_CAST_ (ARGV<Buffer>::null ,that))) {
		_STATIC_WARNING_ ("noop") ;
	}

	ARR<UNIT> &to () leftvalue {
		return PTRTOARR[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () leftvalue {
		return self ;
	}

	inline implicit operator PTR<UNIT> () leftvalue = delete ;

	const ARR<UNIT> &to () const leftvalue {
		return PTRTOARR[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<const UNIT> () leftvalue = delete ;

	LENGTH size () const {
		return SIZE::value ;
	}

	UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return mBuffer[index] ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - PTRTOARR[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const Buffer &that) const {
		return BasicProc::mem_equal (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE::value) ;
	}

	inline BOOL operator== (const Buffer &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Buffer &that) const {
		return !equal (that) ;
	}

	FLAG compr (const Buffer &that) const {
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

	LENGTH expand_size () const {
		return 0 ;
	}

	Buffer expand (const LENGTH &len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	void swap (Buffer &that) {
		BasicProc::mem_swap (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,SIZE::value) ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SFIXED> final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	PTR<NONE> mOrigin ;
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	implicit Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<UNIT>>::null ,len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,len) ;
		mOrigin = rax.self ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<UNIT>>::null ,rax.self) ;
		mBuffer = r1x ;
		mSize = len ;
		rax = NULL ;
	}

	implicit ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize))
			DEREF[mBuffer][i].~UNIT () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	implicit Buffer (const Buffer &) = delete ;

	inline Buffer &operator= (const Buffer &) = delete ;

	implicit Buffer (Buffer &&) = delete ;

	inline Buffer &operator= (Buffer &&) = delete ;

	ARR<UNIT> &to () leftvalue {
		if (mBuffer == NULL)
			return _NULL_ (ARGV<ARR<UNIT>>::null) ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () leftvalue {
		return self ;
	}

	inline implicit operator PTR<UNIT> () leftvalue = delete ;

	const ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_ (ARGV<ARR<UNIT>>::null) ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<const UNIT> () const leftvalue = delete ;

	LENGTH size () const {
		return mSize ;
	}

	UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const Buffer &that) const {
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

	FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return ComprInvokeProc::invoke (mSize ,that.mSize) ;
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

	LENGTH expand_size () const {
		return 0 ;
	}

	Buffer<UNIT ,SAUTO> expand (const LENGTH &len) const {
		_DYNAMIC_ASSERT_ (FALSE) ;
		return Buffer<UNIT ,SAUTO> () ;
	}

	void swap (Buffer<UNIT ,SAUTO> &that) {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	void swap (Buffer &that) {
		_DYNAMIC_ASSERT_ (mSize == that.mSize) ;
		BasicProc::mem_swap (PTRTOARR[mBuffer] ,PTRTOARR[that.mBuffer] ,mSize) ;
	}

private:
	explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mBuffer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
using FixedBuffer = CAST_TRAITS_TYPE<Buffer<REMOVE_CVR_TYPE<UNIT> ,SFIXED> ,UNIT> ;

template <class UNIT>
class Buffer<UNIT ,SAUTO> ;

template <class UNIT>
class Buffer<SPECIALIZATION<UNIT ,FALSE> ,SAUTO> {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

protected:
	PTR<NONE> mOrigin ;
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

protected:
	implicit Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<UNIT>>::null ,len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,len) ;
		mOrigin = rax.self ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<UNIT>>::null ,rax.self) ;
		mBuffer = r1x ;
		mSize = len ;
		rax = NULL ;
	}

	implicit ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize))
			DEREF[mBuffer][i].~UNIT () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	implicit Buffer (const Buffer &) = delete ;

	inline Buffer &operator= (const Buffer &) = delete ;

	implicit Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

protected:
	explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mBuffer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class Buffer<SPECIALIZATION<UNIT ,TRUE> ,SAUTO> {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

protected:
	PTR<NONE> mOrigin ;
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

protected:
	implicit Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		if (len == 0)
			return ;
		_DEBUG_ASSERT_ (len > 0) ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<UNIT>>::null ,len) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,len) ;
		mOrigin = rax.self ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<UNIT>>::null ,rax.self) ;
		mBuffer = r1x ;
		mSize = len ;
		rax = NULL ;
	}

	implicit ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		for (auto &&i : _RANGE_ (0 ,mSize))
			DEREF[mBuffer][i].~UNIT () ;
		GlobalHeap::free (mOrigin) ;
		mOrigin = NULL ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	implicit Buffer (const Buffer &that)
		:Buffer (ARGVP0) {
		if (that.mBuffer == NULL)
			return ;
		auto rax = GlobalHeap::alloc (ARGV<TEMP<UNIT>>::null ,that.mSize) ;
		ScopedBuild<ARR<UNIT>> ANONYMOUS (rax ,DEREF[that.mBuffer] ,that.mSize) ;
		mOrigin = rax.self ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<UNIT>>::null ,rax.self) ;
		mBuffer = r1x ;
		mSize = that.mSize ;
		rax = NULL ;
	}

	inline Buffer &operator= (const Buffer &that) {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	implicit Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

protected:
	explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		:mOrigin (NULL) ,mBuffer (NULL) ,mSize (0) {}
} ;

template <class UNIT>
class Buffer<UNIT ,SAUTO> final
	:private Buffer<SPECIALIZATION<UNIT ,(IS_COPY_CONSTRUCTIBLE_HELP<UNIT>::value && api::is_nothrow_move_constructible<UNIT>::value)> ,SAUTO> {
private:
	using SPECIALIZATION_BASE = Buffer<SPECIALIZATION<UNIT ,(IS_COPY_CONSTRUCTIBLE_HELP<UNIT>::value && api::is_nothrow_move_constructible<UNIT>::value)> ,SAUTO> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mOrigin ;
	using SPECIALIZATION_BASE::mBuffer ;
	using SPECIALIZATION_BASE::mSize ;

public:
	implicit Buffer () = default ;

	explicit Buffer (const LENGTH &len)
		:SPECIALIZATION_BASE (len) {
		_STATIC_WARNING_ ("noop") ;
	}

	ARR<UNIT> &to () leftvalue {
		if (mBuffer == NULL)
			return _NULL_ (ARGV<ARR<UNIT>>::null) ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () leftvalue {
		return self ;
	}

	inline implicit operator PTR<UNIT> () leftvalue = delete ;

	const ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_ (ARGV<ARR<UNIT>>::null) ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<const UNIT> () const leftvalue = delete ;

	LENGTH size () const {
		return mSize ;
	}

	UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const Buffer &that) const {
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

	FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return ComprInvokeProc::invoke (mSize ,that.mSize) ;
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

	LENGTH expand_size () const {
		const auto r1x = LENGTH (mSize * MATH_SQRT2) ;
		const auto r2x = mSize + DEFAULT_RECURSIVE_SIZE::value ;
		return _MAX_ (r1x ,r2x) ;
	}

	Buffer expand (const LENGTH &len) const {
		return Buffer (len) ;
	}

	void swap (Buffer &that) {
		_SWAP_ (mOrigin ,that.mOrigin) ;
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}
} ;

template <class UNIT>
using AutoBuffer = CAST_TRAITS_TYPE<Buffer<REMOVE_CVR_TYPE<UNIT> ,SAUTO> ,UNIT> ;

template <class UNIT>
class Buffer<UNIT ,SCPHAN> final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	PTR<const ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	implicit Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		_DEBUG_ASSERT_ (len == 0) ;
	}

	implicit ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	implicit Buffer (const Buffer &) = delete ;

	inline Buffer &operator= (const Buffer &) = delete ;

	implicit Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	const ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_ (ARGV<ARR<UNIT>>::null) ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator const ARR<UNIT> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<const UNIT> () const leftvalue = delete ;

	LENGTH size () const {
		return mSize ;
	}

	const UNIT &get (const INDEX &index) const leftvalue {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const Buffer &that) const {
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

	FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return ComprInvokeProc::invoke (mSize ,that.mSize) ;
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

	LENGTH expand_size () const {
		return 0 ;
	}

	Buffer expand (const LENGTH &len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	void swap (Buffer &that) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

	//@warn: phantom means deliver pointer without holder
	imports Buffer make (const ARR<UNIT> &src ,const LENGTH &len) {
		return Buffer (ARGVP0 ,DEPTR[src] ,len) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_ARRAY_OF_HELP<UNIT ,_ARG1>::value)>>
	imports Buffer make (_ARG1 &val) {
		return make (PTRTOARR[val] ,_COUNTOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	imports Buffer make (const Buffer<UNIT ,_ARG1> &val) {
		return make (val.self ,val.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<(IS_SAME_HELP<UNIT ,BYTE>::value && !IS_SAME_HELP<_ARG1 ,BYTE>::value)>>
	imports Buffer make (const Buffer<_ARG1 ,_ARG2> &val) {
		_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value) ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<BYTE>>::null ,DEPTR[val.self]) ;
		const auto r2x = val.size () * _SIZEOF_ (_ARG1) ;
		return make (DEREF[r1x] ,r2x) ;
	}

private:
	explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		:mBuffer (NULL) ,mSize (0) {}

	explicit Buffer (const DEF<decltype (ARGVP0)> & ,const PTR<const ARR<UNIT>> &buffer ,const LENGTH &size_)
		: Buffer (ARGVP0) {
		if (size_ == 0)
			return ;
		_DEBUG_ASSERT_ (size_ > 0) ;
		mBuffer = buffer ;
		mSize = size_ ;
	}
} ;

template <class UNIT>
class Buffer<UNIT ,SMPHAN> final {
	_STATIC_ASSERT_ (IS_COMPLETE_HELP<UNIT>::value) ;

private:
	PTR<ARR<UNIT>> mBuffer ;
	LENGTH mSize ;

public:
	implicit Buffer ()
		:Buffer (ARGVP0) {
		_STATIC_WARNING_ ("noop") ;
	}

	explicit Buffer (const LENGTH &len)
		:Buffer (ARGVP0) {
		_DEBUG_ASSERT_ (len == 0) ;
	}

	implicit ~Buffer () noexcept {
		if (mBuffer == NULL)
			return ;
		mBuffer = NULL ;
		mSize = 0 ;
	}

	implicit Buffer (const Buffer &) = delete ;

	inline Buffer &operator= (const Buffer &) = delete ;

	implicit Buffer (Buffer &&that) noexcept
		:Buffer (ARGVP0) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

	inline Buffer &operator= (Buffer &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Buffer () ;
			new (this) Buffer (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	ARR<UNIT> &to () const leftvalue {
		if (mBuffer == NULL)
			return _NULL_ (ARGV<ARR<UNIT>>::null) ;
		return DEREF[mBuffer] ;
	}

	inline implicit operator ARR<UNIT> & () const leftvalue {
		return self ;
	}

	inline implicit operator PTR<UNIT> () const leftvalue = delete ;

	LENGTH size () const {
		return mSize ;
	}

	UNIT &get (const INDEX &index) const leftvalue {
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
		_DEBUG_ASSERT_ (index >= 0 && index < size ()) ;
		return DEREF[mBuffer][index] ;
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif
}

	inline UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const UNIT &item) const {
		INDEX ret = DEPTR[item] - DEREF[mBuffer] ;
		if (!(ret >= 0 && ret < size ()))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	BOOL equal (const Buffer &that) const {
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

	FLAG compr (const Buffer &that) const {
		const auto r1x = _MIN_ (mSize ,that.mSize) ;
		const auto r2x = BasicProc::mem_compr (DEREF[mBuffer] ,DEREF[that.mBuffer] ,r1x) ;
		if (r2x != 0)
			return r2x ;
		return ComprInvokeProc::invoke (mSize ,that.mSize) ;
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

	LENGTH expand_size () const {
		return 0 ;
	}

	Buffer expand (const LENGTH &len) const {
		_DEBUG_ASSERT_ (FALSE) ;
		return Buffer () ;
	}

	void swap (Buffer &that) {
		_SWAP_ (mBuffer ,that.mBuffer) ;
		_SWAP_ (mSize ,that.mSize) ;
	}

	//@warn: phantom means deliver pointer without holder
	imports Buffer make (ARR<UNIT> &src ,const LENGTH &len) {
		return Buffer (ARGVP0 ,DEPTR[src] ,len) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_ARRAY_OF_HELP<UNIT ,_ARG1>::value)>>
	imports Buffer make (_ARG1 &val) {
		return make (PTRTOARR[val] ,_COUNTOF_ (_ARG1)) ;
	}

	template <class _ARG1>
	imports Buffer make (Buffer<UNIT ,_ARG1> &val) {
		return make (val.self ,val.size ()) ;
	}

	imports Buffer make (const Buffer<UNIT ,SMPHAN> &val) {
		return make (val.self ,val.size ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class = ENABLE_TYPE<(IS_SAME_HELP<UNIT ,BYTE>::value && !IS_SAME_HELP<_ARG1 ,BYTE>::value)>>
	imports Buffer make (Buffer<_ARG1 ,_ARG2> &val) {
		_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value) ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<BYTE>>::null ,DEPTR[val.self]) ;
		const auto r2x = val.size () * _SIZEOF_ (_ARG1) ;
		return make (DEREF[r1x] ,r2x) ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<(IS_SAME_HELP<UNIT ,BYTE>::value && !IS_SAME_HELP<_ARG1 ,BYTE>::value)>>
	imports Buffer make (const Buffer<_ARG1 ,SMPHAN> &val) {
		_STATIC_ASSERT_ (U::IS_SAFE_ALIASING_HELP<ARR<BYTE> ,ARR<_ARG1>>::value) ;
		const auto r1x = _POINTER_CAST_ (ARGV<ARR<BYTE>>::null ,DEPTR[val.self]) ;
		const auto r2x = val.size () * _SIZEOF_ (_ARG1) ;
		return make (DEREF[r1x] ,r2x) ;
	}

private:
	explicit Buffer (const DEF<decltype (ARGVP0)> &) noexcept
		:mBuffer (NULL) ,mSize (0) {}

	explicit Buffer (const DEF<decltype (ARGVP0)> & ,const PTR<ARR<UNIT>> &buffer ,const LENGTH &size_)
		: Buffer (ARGVP0) {
		if (size_ == 0)
			return ;
		_DEBUG_ASSERT_ (size_ > 0) ;
		mBuffer = buffer ;
		mSize = size_ ;
	}
} ;

template <class UNIT>
using PhanBuffer = Buffer<REMOVE_CONST_TYPE<UNIT> ,CONDITIONAL_TYPE<(IS_CONST_HELP<UNIT>::value) ,SCPHAN ,SMPHAN>> ;

template <class ,class>
class Allocator ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,FALSE ,FALSE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

protected:
	using SPECIALIZATION_THIS = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	struct NODE_PACK {
		TEMP<UNIT> mValue ;
		INDEX mNext ;
	} ;

protected:
	Buffer<NODE_PACK ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

protected:
	implicit Allocator ()
		:Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	explicit Allocator (const LENGTH &len)
		:Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	implicit ~Allocator () noexcept {
		if (mAllocator.size () == 0)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			if switch_once (TRUE) {
				INDEX ix = mSize - 1 ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_DESTROY_ (DEPTR[mAllocator[ix].mValue]) ;
			}
			mSize-- ;
		}
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	implicit Allocator (const Allocator &) = delete ;

	inline Allocator &operator= (const Allocator &) = delete ;

	implicit Allocator (Allocator &&) = delete ;

	inline Allocator &operator= (Allocator &&) = delete ;

protected:
	explicit Allocator (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline SPECIALIZATION_THIS &m_spec () leftvalue {
		return static_cast<SPECIALIZATION_THIS &> (DEREF[this]) ;
	}

	inline const SPECIALIZATION_THIS &m_spec () const leftvalue {
		return static_cast<const SPECIALIZATION_THIS &> (DEREF[this]) ;
	}

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,FALSE ,TRUE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

protected:
	using SPECIALIZATION_THIS = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	struct NODE_PACK {
		TEMP<UNIT> mValue ;
		INDEX mNext ;
	} ;

protected:
	Buffer<NODE_PACK ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

protected:
	implicit Allocator ()
		:Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	explicit Allocator (const LENGTH &len)
		:Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	implicit ~Allocator () noexcept {
		if (mAllocator.size () == 0)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			if switch_once (TRUE) {
				INDEX ix = mSize - 1 ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_DESTROY_ (DEPTR[mAllocator[ix].mValue]) ;
			}
			mSize-- ;
		}
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	implicit Allocator (const Allocator &) = delete ;

	inline Allocator &operator= (const Allocator &) = delete ;

	implicit Allocator (Allocator &&that) noexcept
		:Allocator (ARGVP0 ,_MOVE_ (that.mAllocator)) {
		if (mAllocator.size () != that.mAllocator.size ())
			_SWAP_ (mSize ,that.mSize) ;
		const auto r1x = that.mSize - mSize ;
		const auto r2x = VAR_ZERO ;
		mSize += _MAX_ (r1x ,r2x) * _EBOOL_ (IS_TRIVIAL_HELP<UNIT>::value) ;
		while (TRUE) {
			if (mSize >= that.mSize)
				break ;
			if switch_once (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (DEPTR[mAllocator[ix].mValue] ,_MOVE_ (_CAST_ (ARGV<UNIT>::null ,that.mAllocator[ix].mValue))) ;
			}
			mSize++ ;
		}
		_SWAP_ (mLength ,that.mLength) ;
		_SWAP_ (mFree ,that.mFree) ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Allocator () ;
			new (this) Allocator (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

private:
	explicit Allocator (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	explicit Allocator (const DEF<decltype (ARGVP0)> & ,Buffer<NODE_PACK ,SIZE> &&allocator_)
		:mAllocator (_MOVE_ (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline SPECIALIZATION_THIS &m_spec () leftvalue {
		return static_cast<SPECIALIZATION_THIS &> (DEREF[this]) ;
	}

	inline const SPECIALIZATION_THIS &m_spec () const leftvalue {
		return static_cast<const SPECIALIZATION_THIS &> (DEREF[this]) ;
	}

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator<SPECIALIZATION<UNIT ,TRUE ,TRUE> ,SIZE> {
#pragma push_macro ("spec")
#undef spec
#define spec m_spec ()

protected:
	using SPECIALIZATION_THIS = Allocator<UNIT ,SIZE> ;

	//@warn: memory alignment reduce utilization ratio of memory
	struct NODE_PACK {
		TEMP<UNIT> mValue ;
		INDEX mNext ;
	} ;

protected:
	Buffer<NODE_PACK ,SIZE> mAllocator ;
	LENGTH mSize ;
	LENGTH mLength ;
	INDEX mFree ;

protected:
	implicit Allocator ()
		:Allocator (ARGVP0 ,0) {
		spec.update_reserve (mSize ,mFree) ;
	}

	explicit Allocator (const LENGTH &len)
		:Allocator (ARGVP0 ,len) {
		spec.update_reserve (mSize ,mFree) ;
	}

	implicit ~Allocator () noexcept {
		if (mAllocator.size () == 0)
			return ;
		while (TRUE) {
			if (mSize <= 0)
				break ;
			if switch_once (TRUE) {
				INDEX ix = mSize - 1 ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_DESTROY_ (DEPTR[mAllocator[ix].mValue]) ;
			}
			mSize-- ;
		}
		mLength = 0 ;
		mFree = VAR_NONE ;
	}

	implicit Allocator (const Allocator &that)
		:Allocator (ARGVP0 ,_MOVE_ (that.mAllocator)) {
		if (mAllocator.size () != that.mAllocator.size ())
			mSize = that.mSize ;
		const auto r1x = that.mSize - mSize ;
		const auto r2x = VAR_ZERO ;
		mSize += _MAX_ (r1x ,r2x) * _EBOOL_ (IS_TRIVIAL_HELP<UNIT>::value) ;
		while (TRUE) {
			if (mSize >= that.mSize)
				break ;
			if switch_once (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (DEPTR[mAllocator[ix].mValue] ,_MOVE_ (_CAST_ (ARGV<UNIT>::null ,that.mAllocator[ix].mValue))) ;
			}
			mSize++ ;
		}
		mLength = that.mLength ;
		mFree = that.mFree ;
	}

	inline Allocator &operator= (const Allocator &that) {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Allocator () ;
			new (this) Allocator (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

	implicit Allocator (Allocator &&that) noexcept
		:Allocator (ARGVP0 ,_MOVE_ (that.mAllocator)) {
		if (mAllocator.size () != that.mAllocator.size ())
			_SWAP_ (mSize ,that.mSize) ;
		const auto r1x = that.mSize - mSize ;
		const auto r2x = VAR_ZERO ;
		mSize += _MAX_ (r1x ,r2x) * _EBOOL_ (IS_TRIVIAL_HELP<UNIT>::value) ;
		while (TRUE) {
			if (mSize >= that.mSize)
				break ;
			if switch_once (TRUE) {
				INDEX ix = mSize ;
				if (mAllocator[ix].mNext != VAR_USED)
					discard ;
				_CREATE_ (DEPTR[mAllocator[ix].mValue] ,_MOVE_ (_CAST_ (ARGV<UNIT>::null ,that.mAllocator[ix].mValue))) ;
			}
			mSize++ ;
		}
		_SWAP_ (mLength ,that.mLength) ;
		_SWAP_ (mFree ,that.mFree) ;
	}

	inline Allocator &operator= (Allocator &&that) noexcept {
		if switch_once (TRUE) {
			if (this == DEPTR[that])
				discard ;
			DEREF[this].~Allocator () ;
			new (this) Allocator (_MOVE_ (that)) ;
		}
		return DEREF[this] ;
	}

protected:
	explicit Allocator (const DEF<decltype (ARGVP0)> & ,const LENGTH &len)
		:mAllocator (len) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	explicit Allocator (const DEF<decltype (ARGVP0)> & ,const Buffer<NODE_PACK ,SIZE> &allocator_)
		:mAllocator (_MOVE_ (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	explicit Allocator (const DEF<decltype (ARGVP0)> & ,Buffer<NODE_PACK ,SIZE> &&allocator_)
		:mAllocator (_MOVE_ (allocator_)) ,mSize (0) ,mLength (0) ,mFree (VAR_NONE) {}

	inline SPECIALIZATION_THIS &m_spec () leftvalue {
		return static_cast<SPECIALIZATION_THIS &> (DEREF[this]) ;
	}

	inline const SPECIALIZATION_THIS &m_spec () const leftvalue {
		return static_cast<const SPECIALIZATION_THIS &> (DEREF[this]) ;
	}

#pragma pop_macro ("spec")
} ;

template <class UNIT ,class SIZE>
class Allocator final
	:private Allocator<SPECIALIZATION<UNIT ,(IS_COPY_CONSTRUCTIBLE_HELP<Buffer<UNIT ,SIZE>>::value && api::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value) ,api::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value> ,SIZE> {
	_STATIC_ASSERT_ (api::is_nothrow_move_constructible<UNIT>::value) ;
	_STATIC_ASSERT_ (api::is_nothrow_move_assignable<UNIT>::value) ;

private:
	using SPECIALIZATION_BASE = Allocator<SPECIALIZATION<UNIT ,(IS_COPY_CONSTRUCTIBLE_HELP<Buffer<UNIT ,SIZE>>::value && api::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value) ,api::is_nothrow_move_constructible<Buffer<UNIT ,SIZE>>::value> ,SIZE> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mAllocator ;
	using SPECIALIZATION_BASE::mSize ;
	using SPECIALIZATION_BASE::mLength ;
	using SPECIALIZATION_BASE::mFree ;

public:
	implicit Allocator () = default ;

	explicit Allocator (const LENGTH &len)
		:SPECIALIZATION_BASE (len) {
		_STATIC_WARNING_ ("noop") ;
	}

	LENGTH size () const {
		return mSize ;
	}

	LENGTH length () const {
		if (mAllocator.size () == 0)
			return 0 ;
		return mLength ;
	}

	void clear () {
		INDEX ix = VAR_NONE ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			INDEX iy = ix ;
			ix = mSize + ~i ;
			if (mAllocator[ix].mNext == VAR_USED)
				_DESTROY_ (DEPTR[mAllocator[ix].mValue]) ;
			mAllocator[ix].mNext = iy ;
		}
		mLength = 0 ;
		mFree = ix ;
	}

	BOOL used (const INDEX &index) const {
		if (mAllocator[index].mNext != VAR_USED)
			return FALSE ;
		return TRUE ;
	}

	UNIT &get (const INDEX &index) leftvalue {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_ (ARGV<UNIT>::null ,mAllocator[index].mValue) ;
	}

	inline UNIT &operator[] (const INDEX &index) leftvalue {
		return get (index) ;
	}

	const UNIT &get (const INDEX &index) const leftvalue {
		_DEBUG_ASSERT_ (used (index)) ;
		return _CAST_ (ARGV<UNIT>::null ,mAllocator[index].mValue) ;
	}

	inline const UNIT &operator[] (const INDEX &index) const leftvalue {
		return get (index) ;
	}

	INDEX at (const UNIT &item) const {
		using NODE_PACK = typename SPECIALIZATION_BASE::NODE_PACK ;
		auto &r1x = _OFFSET_ (&NODE_PACK::mValue ,_CAST_ (ARGV<TEMP<UNIT>>::null ,item)) ;
		INDEX ret = mAllocator.at (r1x) ;
		if (!used (ret))
			ret = VAR_NONE ;
		return _MOVE_ (ret) ;
	}

	template <class... _ARGS>
	INDEX alloc (_ARGS &&...initval) {
		INDEX ret = VAR_NONE ;
		auto fax = TRUE ;
		if switch_once (fax) {
			if (mFree != VAR_NONE)
				discard ;
			auto rax = mAllocator.expand (mAllocator.expand_size ()) ;
			ret = mSize ;
			_CREATE_ (DEPTR[rax[ret].mValue] ,_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
			for (auto &&i : _RANGE_ (0 ,mSize)) {
				_CREATE_ (DEPTR[rax[i].mValue] ,_MOVE_ (_CAST_ (ARGV<UNIT>::null ,mAllocator[i].mValue))) ;
				rax[i].mNext = VAR_USED ;
			}
			mAllocator.swap (rax) ;
			update_reserve (mSize ,mFree) ;
		}
		if switch_once (fax) {
			ret = mFree ;
			_CREATE_ (DEPTR[mAllocator[ret].mValue] ,_FORWARD_ (ARGV<_ARGS &&>::null ,initval)...) ;
		}
		mFree = mAllocator[ret].mNext ;
		mAllocator[ret].mNext = VAR_USED ;
		mLength++ ;
		return _MOVE_ (ret) ;
	}

	void free (const INDEX &index) noexcept {
		_DEBUG_ASSERT_ (used (index)) ;
		_DESTROY_ (DEPTR[mAllocator[index].mValue]) ;
		mAllocator[index].mNext = mFree ;
		mFree = index ;
		mLength-- ;
	}

	void reserve (const LENGTH &len) {
		_DEBUG_ASSERT_ (len >= 0) ;
		const auto r1x = len - (mSize - mLength) ;
		const auto r2x = VAR_ZERO ;
		const auto r3x = _MAX_ (r1x ,r2x) ;
		if (r3x == 0)
			return ;
		_DEBUG_ASSERT_ (mSize + r3x > mSize) ;
		auto rax = mAllocator.expand (mSize + r3x) ;
		for (auto &&i : _RANGE_ (0 ,mSize)) {
			if (mAllocator[i].mNext == VAR_USED)
				_CREATE_ (DEPTR[rax[i].mValue] ,_MOVE_ (_CAST_ (ARGV<UNIT>::null ,mAllocator[i].mValue))) ;
			rax[i].mNext = mAllocator[i].mNext ;
		}
		mAllocator.swap (rax) ;
		update_reserve (mSize ,mFree) ;
	}

	void clean () {
		const auto r1x = shrink_size () ;
		if (r1x == mSize)
			return ;
		_DYNAMIC_ASSERT_ (r1x == mLength) ;
		auto rax = mAllocator.expand (r1x) ;
		for (auto &&i : _RANGE_ (0 ,rax.size ())) {
			_DEBUG_ASSERT_ (mAllocator[i].mNext == VAR_USED) ;
			_CREATE_ (DEPTR[rax[i].mValue] ,_MOVE_ (_CAST_ (ARGV<UNIT>::null ,mAllocator[i].mValue))) ;
			rax[i].mNext = VAR_USED ;
		}
		mAllocator.swap (rax) ;
		update_reserve (r1x ,VAR_NONE) ;
	}

private:
	void update_reserve (const INDEX &size_ ,const INDEX &free_) noexcept {
		INDEX ix = free_ ;
		for (auto &&i : _RANGE_ (size_ ,mAllocator.size ())) {
			INDEX iy = ix ;
			ix = mAllocator.size () + ~(i - size_) ;
			mAllocator[ix].mNext = iy ;
		}
		mSize = mAllocator.size () ;
		mFree = ix ;
	}

	LENGTH shrink_size () const {
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