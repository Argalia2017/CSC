#pragma once

#ifndef __CSC_EXT__
#define __CSC_EXT__
#endif

#include "csc.hpp"

#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039)
#endif
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#pragma warning (pop)
#pragma endregion

namespace CSC {
namespace stl {
using std::mutex ;
using std::recursive_mutex ;
using std::atomic ;
using std::unique_lock ;
using std::condition_variable ;
using std::thread ;

namespace chrono {
using std::chrono::milliseconds ;
using std::chrono::system_clock ;
} ;

namespace this_thread {
using std::this_thread::get_id ;
using std::this_thread::sleep_for ;
using std::this_thread::sleep_until ;
using std::this_thread::yield ;
} ;
} ;

class VAR128 {
#pragma region
#pragma push_macro ("v2i0")
#pragma push_macro ("v2i1")
#pragma push_macro ("v4i0")
#pragma push_macro ("v4i1")
#pragma push_macro ("v4i2")
#pragma push_macro ("v4i3")
#undef v2i0
#undef v2i1
#undef v4i0
#undef v4i1
#undef v4i2
#undef v4i3
#define v2i0 m_v2i0 ()
#define v2i1 m_v2i1 ()
#define v4i0 m_v4i0 ()
#define v4i1 m_v4i1 ()
#define v4i2 m_v4i2 ()
#define v4i3 m_v4i3 ()

private:
	MEGA mData ;

public:
	inline VAR128 () = default ;

	inline implicit VAR128 (VAR64 right) {
		v2i1 = DATA (right) ;
		v2i0 = (right >= 0) ? (DATA (0)) : (DATA (-1)) ;
	}

	inline explicit operator VAR32 () const {
		return VAR32 (v2i1) ;
	}

	inline explicit operator VAR64 () const {
		return VAR64 (v2i1) ;
	}

	inline BOOL operator== (const VAR128 &right) const {
		return v2i1 == right.v2i1 && v2i0 == right.v2i0 ;
	}

	inline BOOL operator!= (const VAR128 &right) const {
		return v2i1 != right.v2i1 || v2i0 != right.v2i0 ;
	}

	inline BOOL operator< (const VAR128 &right) const {
		const auto r1x = _CAST_<VAR64> (v2i0) ;
		const auto r2x = _CAST_<VAR64> (right.v2i0) ;
		if (r1x != r2x)
			return r1x < r2x ;
		return v2i1 < right.v2i1 ;
	}

	inline BOOL operator>= (const VAR128 &right) const {
		const auto r1x = _CAST_<VAR64> (v2i0) ;
		const auto r2x = _CAST_<VAR64> (right.v2i0) ;
		if (r1x != r2x)
			return r1x >= r2x ;
		return v2i1 >= right.v2i1 ;
	}

	inline BOOL operator> (const VAR128 &right) const {
		const auto r1x = _CAST_<VAR64> (right.v2i0) ;
		const auto r2x = _CAST_<VAR64> (v2i0) ;
		if (r1x != r2x)
			return r1x > r2x ;
		return v2i1 > right.v2i1 ;
	}

	inline BOOL operator<= (const VAR128 &right) const {
		const auto r1x = _CAST_<VAR64> (right.v2i0) ;
		const auto r2x = _CAST_<VAR64> (v2i0) ;
		if (r1x != r2x)
			return r1x <= r2x ;
		return v2i1 <= right.v2i1 ;
	}

	inline VAR128 operator& (const VAR128 &right) const {
		VAR128 ret = 0 ;
		ret.v2i1 = v2i1 & right.v2i1 ;
		ret.v2i0 = v2i0 & right.v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator&= (const VAR128 &right) {
		v2i1 &= right.v2i1 ;
		v2i0 &= right.v2i0 ;
		return *this ;
	}

	inline VAR128 operator| (const VAR128 &right) const {
		VAR128 ret = 0 ;
		ret.v2i1 = v2i1 | right.v2i1 ;
		ret.v2i0 = v2i0 | right.v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator|= (const VAR128 &right) {
		v2i1 |= right.v2i1 ;
		v2i0 |= right.v2i0 ;
		return *this ;
	}

	inline VAR128 operator^ (const VAR128 &right) const {
		VAR128 ret = 0 ;
		ret.v2i1 = v2i1 ^ right.v2i1 ;
		ret.v2i0 = v2i0 ^ right.v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator^= (const VAR128 &right) {
		v2i1 ^= right.v2i1 ;
		v2i0 ^= right.v2i0 ;
		return *this ;
	}

	inline VAR128 operator~ () const {
		VAR128 ret = 0 ;
		ret.v2i1 = ~v2i1 ;
		ret.v2i0 = ~v2i0 ;
		return std::move (ret) ;
	}

	inline VAR128 operator>> (VAR scale) const {
		_DEBUG_ASSERT_ (scale >= 0 && scale < 128) ;
		VAR128 ret = 0 ;
		ret.v2i1 = (scale < 64) ? ((v2i1 >> scale) | (v2i0 << (64 - scale))) : (v2i0 >> (scale - 64)) ;
		ret.v2i0 = (scale < 64) ? (v2i0 >> scale) : 0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator>>= (VAR scale) {
		_DEBUG_ASSERT_ (scale >= 0 && scale < 128) ;
		v2i1 = (scale < 64) ? ((v2i1 >> scale) | (v2i0 << (64 - scale))) : (v2i0 >> (scale - 64)) ;
		v2i0 = (scale < 64) ? (v2i0 >> scale) : 0 ;
		return *this ;
	}

	inline VAR128 operator<< (VAR scale) const {
		_DEBUG_ASSERT_ (scale >= 0 && scale < 128) ;
		VAR128 ret = 0 ;
		ret.v2i0 = (scale < 64) ? ((v2i0 << scale) | (v2i1 >> (64 - scale))) : (v2i1 << (scale - 64)) ;
		ret.v2i1 = (scale < 64) ? (v2i1 << scale) : 0 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator<<= (VAR scale) {
		_DEBUG_ASSERT_ (scale >= 0 && scale < 128) ;
		v2i0 = (scale < 64) ? ((v2i0 << scale) | (v2i1 >> (64 - scale))) : (v2i1 << (scale - 64)) ;
		v2i1 = (scale < 64) ? (v2i1 << scale) : 0 ;
		return *this ;
	}

	inline VAR128 operator+ (const VAR128 &right) const {
		VAR128 ret = 0 ;
		ret.v2i0 = v2i0 + right.v2i0 + EFLAG (v2i1 > ~right.v2i1) ;
		ret.v2i1 = v2i1 + right.v2i1 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator+= (const VAR128 &right) {
		v2i0 += right.v2i0 + EFLAG (v2i1 > ~right.v2i1) ;
		v2i1 += right.v2i1 ;
		return *this ;
	}

	inline VAR128 operator- (const VAR128 &right) const {
		VAR128 ret = 0 ;
		ret.v2i0 = v2i0 - right.v2i0 - EFLAG (v2i1 < right.v2i1) ;
		ret.v2i1 = v2i1 - right.v2i1 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator-= (const VAR128 &right) {
		v2i0 -= right.v2i0 + EFLAG (v2i1 < right.v2i1) ;
		v2i1 -= right.v2i1 ;
		return *this ;
	}

	inline VAR128 operator* (const VAR128 &right) const {
		VAR128 ret = 0 ;
		auto rax = DATA () ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i3) ;
		ret.v4i3 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i2) ;
		rax += DATA (v4i2) * DATA (right.v4i3) ;
		ret.v4i2 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i1) ;
		rax += DATA (v4i2) * DATA (right.v4i2) ;
		rax += DATA (v4i1) * DATA (right.v4i3) ;
		ret.v4i1 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (right.v4i0) ;
		rax += DATA (v4i2) * DATA (right.v4i1) ;
		rax += DATA (v4i1) * DATA (right.v4i2) ;
		rax += DATA (v4i0) * DATA (right.v4i3) ;
		ret.v4i0 = CHAR (rax) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator*= (const VAR128 &right) {
		*this = *this * right ;
		return *this ;
	}

	inline VAR128 operator/ (const VAR128 &right) const {
		_DEBUG_ASSERT_ (right.v2i1 != 0 || right.v2i0 != 0) ;
		VAR128 ret = 0 ;
		const auto r1x = _CAST_<VAR64> (v2i0) >= 0 ;
		const auto r2x = _CAST_<VAR64> (right.v2i0) >= 0 ;
		if (r1x && right.v4i0 == 0 && right.v4i1 == 0 && right.v4i2 == 0) {
			auto rax = DATA () ;
			const auto r3x = DATA (right.v4i3) ;
			_DEBUG_ASSERT_ (r3x != 0) ;
			rax = (DATA (0) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i0) ;
			ret.v4i0 = CHAR (rax / r3x) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i1) ;
			ret.v4i1 = CHAR (rax / r3x) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i2) ;
			ret.v4i2 = CHAR (rax / r3x) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i3) ;
			ret.v4i3 = CHAR (rax / r3x) ;
		} else if (!r1x) {
			const auto r3x = v2i0 == DATA (VAR64_MIN) && v2i1 == 0 ;
			ret = r3x ? (-(-(*this + right) / right + 1)) : (-(-*this / right)) ;
		} else if (!r2x) {
			const auto r3x = right.v2i0 == DATA (VAR64_MIN) && right.v2i1 == 0 ;
			ret = r3x ? (VAR128 (0)) : (*this / -right) ;
		} else {
			ret = slow_divide (*this ,right) ;
		}
		return std::move (ret) ;
	}

	inline VAR128 &operator/= (const VAR128 &right) {
		*this = *this / right ;
		return *this ;
	}

	inline VAR128 operator% (const VAR128 &right) const {
		_DEBUG_ASSERT_ (right.v2i1 != 0 || right.v2i0 != 0) ;
		VAR128 ret = 0 ;
		const auto r1x = _CAST_<VAR64> (v2i0) >= 0 ;
		const auto r2x = _CAST_<VAR64> (right.v2i0) >= 0 ;
		if (r1x && right.v4i0 == 0 && right.v4i1 == 0 && right.v4i2 == 0) {
			auto rax = DATA () ;
			const auto r3x = DATA (right.v4i3) ;
			_DEBUG_ASSERT_ (r3x != 0) ;
			rax = (DATA (0) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i0) ;
			ret.v4i0 = 0 ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i1) ;
			ret.v4i1 = 0 ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i2) ;
			ret.v4i2 = 0 ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i3) ;
			ret.v4i3 = CHAR (rax % r3x) ;
		} else if (!r1x) {
			const auto r3x = v2i0 == DATA (VAR64_MIN) && v2i1 == 0 ;
			ret = r3x ? (-(-(*this + right) % right)) : (-(-*this % right)) ;
		} else if (!r2x) {
			const auto r3x = right.v2i0 == DATA (VAR64_MIN) && right.v2i1 == 0 ;
			ret = r3x ? (*this) : (*this % -right) ;
		} else {
			ret = right - slow_divide (*this ,right) * right ;
		}
		return std::move (ret) ;
	}

	inline VAR128 &operator%= (const VAR128 &right) {
		*this = *this % right ;
		return *this ;
	}

	inline VAR128 operator+ () const {
		return *this ;
	}

	inline VAR128 operator- () const {
		VAR128 ret = 0 ;
		ret.v2i1 = ~v2i1 + 1 ;
		ret.v2i0 = ~v2i0 + EFLAG (ret.v2i1 == DATA (0)) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator++ () {
		v2i1++ ;
		v2i0 += EFLAG (v2i1 == DATA (0)) ;
		return *this ;
	}

	inline VAR128 operator++ (int) popping {
		VAR128 ret = *this ;
		++*this ;
		return std::move (ret) ;
	}

	inline VAR128 &operator-- () {
		v2i1-- ;
		v2i0 -= EFLAG (v2i1 == DATA (-1)) ;
		return *this ;
	}

	inline VAR128 operator-- (int) popping {
		VAR128 ret = *this ;
		--*this ;
		return std::move (ret) ;
	}

private:
	inline DATA &m_v2i0 () {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[0]] ;
	}

	inline const DATA &m_v2i0 () const {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[0]] ;
	}

	inline DATA &m_v2i1 () {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[1]] ;
	}

	inline const DATA &m_v2i1 () const {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[1]] ;
	}

	inline CHAR &m_v4i0 () {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[0]] ;
	}

	inline const CHAR &m_v4i0 () const {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[0]] ;
	}

	inline CHAR &m_v4i1 () {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[1]] ;
	}

	inline const CHAR &m_v4i1 () const {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[1]] ;
	}

	inline CHAR &m_v4i2 () {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[2]] ;
	}

	inline const CHAR &m_v4i2 () const {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[2]] ;
	}

	inline CHAR &m_v4i3 () {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[3]] ;
	}

	inline const CHAR &m_v4i3 () const {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[3]] ;
	}

private:
	inline static VAR128 slow_divide (const VAR128 &y ,const VAR128 &x) {
		_DEBUG_ASSERT_ (y >= 0) ;
		_DEBUG_ASSERT_ (x > 0) ;
		VAR128 ret = 0 ;
		auto rax = Buffer<VAR128 ,ARGC<2>> ({0 ,y}) ;
		while (TRUE) {
			if (rax[0] > rax[1])
				break ;
			ret = rax[0] + (rax[1] - rax[0]) / 2 ;
			const auto r2x = x * ret ;
			if (r2x == y)
				break ;
			const auto r3x = (r2x < y) ? (ret + 1) : (ret - 1) ;
			INDEX ix = EFLAG (r2x >= y) ;
			rax[ix] = r3x ;
		}
		ret -= EFLAG (ret * x > y) ;
		return std::move (ret) ;
	}

#undef v2i0
#undef v2i1
#undef v4i0
#undef v4i1
#undef v4i2
#undef v4i3
#pragma pop_macro ("v2i1")
#pragma pop_macro ("v2i0")
#pragma pop_macro ("v4i3")
#pragma pop_macro ("v4i2")
#pragma pop_macro ("v4i1")
#pragma pop_macro ("v4i0")
#pragma endregion
} ;

namespace U {
struct OPERATOR_CRC32 {
	inline static constexpr BOOL ensure_index (INDEX index ,INDEX ib ,INDEX ie) {
		return index >= ib && index < ie ;
	}

	inline static constexpr CHAR expr_crc32_table (CHAR arg ,const ARGC<8> &) {
		return arg ;
	}

	template <INDEX _VAL ,class = ENABLE_TYPE<ensure_index (_VAL ,0 ,8)>>
	inline static constexpr CHAR expr_crc32_table (CHAR arg ,const ARGC<_VAL> &) {
		return expr_crc32_table (((((arg & CHAR (0X00000001)) != 0) ? (CHAR (0)) : (CHAR (0XEDB88320)) ^ (arg >> 1))) ,_NULL_<const ARGC<_VAL + 1>> ()) ;
	}

	inline static constexpr CHAR expr_crc32_table (INDEX arg) {
		return expr_crc32_table (CHAR (arg) ,_NULL_<const ARGC<0>> ()) ;
	}

	template <LENGTH _VAL>
	inline static constexpr CHAR expr_crc32_hash (const DEF<STR[_VAL]> &arg1 ,CHAR arg2 ,const ARGC<_VAL - 1> &) {
		return arg2 ;
	}

	template <LENGTH _VAL1 ,INDEX _VAL2 ,class = ENABLE_TYPE<ensure_index (_VAL2 ,0 ,_VAL1 - 1)>>
	inline static constexpr CHAR expr_crc32_hash (const DEF<STR[_VAL1]> &arg1 ,CHAR arg2 ,const ARGC<_VAL2> &) {
		return expr_crc32_hash (arg1 ,(expr_crc32_table (INDEX ((CHAR (arg2) ^ CHAR (arg1[_VAL2])) & CHAR (0X000000FF))) ^ (arg2 >> 8)) ,_NULL_<const ARGC<_VAL2 + 1>> ()) ;
	}

	template <LENGTH _VAL>
	inline static constexpr FLAG invoke (const DEF<STR[_VAL]> &arg) {
		return FLAG (VAR32 (expr_crc32_hash (arg ,CHAR (0XFFFFFFFF) ,_NULL_<const ARGC<0>> ())) & VAR32_MAX) ;
	}
} ;
} ;

namespace stl {
template <class>
struct is_template_type :public false_type {} ;

template <template <class...> class _ARGT ,class... _ARGS>
struct is_template_type<_ARGT<_ARGS...>> :public true_type {} ;

template <template <class...> class ,class>
struct is_template_of :public false_type {} ;

template <template <class...> class _ARGT ,class... _ARGS>
struct is_template_of<_ARGT ,_ARGT<_ARGS...>> :public true_type {} ;
} ;

template <class TYPE>
class Mutable {
private:
	static constexpr auto STATUS_CACHED = FLAG (1) ;
	static constexpr auto STATUS_SIGNALED = FLAG (2) ;
	static constexpr auto STATUS_FINISHED = FLAG (3) ;

private:
	mutable TYPE mData ;
	mutable FLAG mStatus ;

public:
	inline Mutable () :mStatus (STATUS_SIGNALED) {}

	inline implicit Mutable (const TYPE &right) : mData (std::move (right)) ,mStatus (STATUS_CACHED) {}

	inline implicit Mutable (TYPE &&right) : mData (std::move (right)) ,mStatus (STATUS_CACHED) {}

	inline const TYPE &to () const {
		return mData ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline void apply (const Function<void (TYPE &)> &proc) const {
		if (mStatus != STATUS_SIGNALED)
			return ;
		proc (mData) ;
		mStatus = STATUS_CACHED ;
	}

	inline void apply (const Function<void (const TYPE &)> &proc) const {
		if (mStatus != STATUS_SIGNALED)
			return ;
		proc (mData) ;
		mStatus = STATUS_CACHED ;
	}

	inline void apply (const Function<DEF<void (TYPE &)> NONE::*> &proc) const {
		if (mStatus != STATUS_SIGNALED)
			return ;
		proc (mData) ;
		mStatus = STATUS_CACHED ;
	}

	inline void apply (const Function<DEF<void (const TYPE &)> NONE::*> &proc) const {
		if (mStatus != STATUS_SIGNALED)
			return ;
		proc (mData) ;
		mStatus = STATUS_CACHED ;
	}

#ifdef __CSC_DEPRECATED__
	template <class _ARG ,class = ENABLE_TYPE<std::is_same<RESULTOF_TYPE<_ARG (TYPE &)> ,void>::value>>
	inline void apply_proc (const _ARG &proc) const {
		apply (Function<DEF<void (TYPE &)> NONE::*> (PhanRef<const _ARG>::make (proc) ,&_ARG::operator())) ;
	}
#endif

	inline void signal () const {
		if (mStatus != STATUS_CACHED)
			return ;
		mStatus = STATUS_SIGNALED ;
	}

	inline void finish () const {
		mStatus = STATUS_FINISHED ;
	}
} ;

namespace U {
template <class ,class...>
struct INDEXOF_TRAITS {
	using TYPE = ARGC<VAR_NONE> ;
} ;

template <class _ARG1 ,class... _ARGS>
struct INDEXOF_TRAITS<_ARG1 ,_ARG1 ,_ARGS...> {
	using TYPE = ARGC<VAR_ZERO> ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEXOF_TRAITS<_ARG1 ,_ARG2 ,_ARGS...> {
	using TYPE = ARGC<(INDEXOF_TRAITS<_ARG1 ,_ARGS...>::TYPE::value != VAR_NONE) ? (INDEXOF_TRAITS<_ARG1 ,_ARGS...>::TYPE::value + 1) : VAR_NONE> ;
} ;
} ;

template <class _ARG1 ,class... _ARGS>
using INDEXOF_TRAITS_TYPE = typename U::INDEXOF_TRAITS<_ARG1 ,_ARGS...>::TYPE ;

namespace U {
template <class ,class...>
struct VISITOF_TRATIS ;

template <class _ARG1 ,class... _ARGS>
struct VISITOF_TRATIS<ARGC<0> ,_ARG1 ,_ARGS...> {
	using TYPE = _ARG1 ;
} ;

template <class _ARG1 ,class _ARG2 ,class... _ARGS>
struct VISITOF_TRATIS<_ARG1 ,_ARG2 ,_ARGS...> {
	_STATIC_ASSERT_ (_ARG1::value > 0) ;
	using TYPE = typename VISITOF_TRATIS<ARGC<_ARG1::value - 1> ,_ARGS...>::TYPE ;
} ;
} ;

template <INDEX _VAL ,class... _ARGS>
using VISITOF_TRATIS_TYPE = typename U::VISITOF_TRATIS<ARGC<_VAL> ,_ARGS...>::TYPE ;

template <class... TYPES>
class Variant {
private:
	inline static constexpr LENGTH expr_max_sizeof (const ARGVS<> &) {
		return 1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static constexpr LENGTH expr_max_sizeof (const ARGVS<_ARG1 ,_ARGS...> &) {
		return _MAX_ (_SIZEOF_ (_ARG1) ,expr_max_sizeof (_NULL_<const ARGVS<_ARGS...>> ())) ;
	}

	inline static constexpr LENGTH expr_max_alignof (const ARGVS<> &) {
		return 1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static constexpr LENGTH expr_max_alignof (const ARGVS<_ARG1 ,_ARGS...> &) {
		return _MAX_ (_ALIGNOF_ (_ARG1) ,expr_max_alignof (_NULL_<const ARGVS<_ARGS...>> ())) ;
	}

	using VARIANT_SIZE = ARGC<expr_max_sizeof (_NULL_<const ARGVS<TYPES...>> ())> ;
	using VARIANT_ALIGN = ARGC<expr_max_alignof (_NULL_<const ARGVS<TYPES...>> ())> ;

	template <LENGTH _VAL1 ,LENGTH _VAL2>
	struct ALIGNED_UNION {
		alignas (_VAL1) DEF<BYTE[_VAL2]> unused ;
	} ;

	using VARIANT = ALIGNED_UNION<VARIANT_ALIGN::value ,VARIANT_SIZE::value> ;

	struct NULLOPT ;

private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) > 0) ;
	TEMP<VARIANT> mVariant ;
	INDEX mIndex ;

public:
	inline Variant () {
		mIndex = template_construct (&mVariant ,_NULL_<const ARGVS<TYPES...>> ()) ;
	}

	template <class _ARG ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG> ,Variant>::value>>
	inline implicit Variant (_ARG &&right) {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_ARG> ,TYPES...>::value != VAR_NONE) ;
		mIndex = template_init_construct (&mVariant ,std::forward<_ARG> (right)) ;
		_DYNAMIC_ASSERT_ (mIndex != VAR_NONE) ;
	}

	inline ~Variant () noexcept {
		if (mIndex == VAR_NONE)
			return ;
		template_destruct (&mVariant ,mIndex ,_NULL_<const ARGVS<TYPES...>> ()) ;
		mIndex = VAR_NONE ;
	}

	inline Variant (const Variant &right) {
		mIndex = right.mIndex ;
		if (mIndex == VAR_NONE)
			return ;
		template_copy_construct (&mVariant ,&right.mVariant ,mIndex ,_NULL_<const ARGVS<TYPES...>> ()) ;
	}

	inline Variant &operator= (const Variant &right) {
		if (this != &right) {
			this->~Variant () ;
			new (this) Variant (std::move (right)) ;
		}
		return _XVALUE_<Variant &> (*this) ;
	}

	inline Variant (Variant &&right) noexcept {
		mIndex = right.mIndex ;
		if (mIndex == VAR_NONE)
			return ;
		template_move_construct (&mVariant ,&right.mVariant ,mIndex ,_NULL_<const ARGVS<TYPES...>> ()) ;
	}

	inline Variant &operator= (Variant &&right) noexcept {
		if (this != &right) {
			this->~Variant () ;
			new (this) Variant (std::move (right)) ;
		}
		return _XVALUE_<Variant &> (*this) ;
	}

	inline BOOL exist () const {
		return mIndex != VAR_NONE ;
	}

	template <class _RET>
	inline BOOL available () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value != VAR_NONE) ;
		return mIndex == INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value ;
	}

	inline VISITOF_TRATIS_TYPE<0 ,TYPES...> &to () {
		_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) == 1) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		return _LOAD_<VISITOF_TRATIS_TYPE<0 ,TYPES...>> (mVariant.unused) ;
	}

	inline implicit operator VISITOF_TRATIS_TYPE<0 ,TYPES...> & () {
		return to () ;
	}

	inline const VISITOF_TRATIS_TYPE<0 ,TYPES...> &to () const {
		_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) == 1) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		return _LOAD_<VISITOF_TRATIS_TYPE<0 ,TYPES...>> (mVariant.unused) ;
	}

	inline implicit operator const VISITOF_TRATIS_TYPE<0 ,TYPES...> & () const {
		return to () ;
	}

	template <class _ARG>
	inline _ARG &value (_ARG &def) {
		if (!available<_ARG> ())
			return def ;
		return _LOAD_<_ARG> (mVariant.unused) ;
	}

	template <class _ARG>
	inline const _ARG &value (const _ARG &def) const {
		if (!available<_ARG> ())
			return def ;
		return _LOAD_<_ARG> (mVariant.unused) ;
	}

	template <class _ARG>
	inline void apply (const Function<void (_ARG &)> &proc) {
		if (!available<_ARG> ())
			return ;
		proc (_LOAD_<_ARG> (mVariant.unused)) ;
	}

	template <class _ARG>
	inline void apply (const Function<void (const _ARG &)> &proc) const {
		if (!available<_ARG> ())
			return ;
		proc (_LOAD_<_ARG> (mVariant.unused)) ;
	}

	template <class _ARG>
	inline void apply (const Function<DEF<void (_ARG &)> NONE::*> &proc) {
		if (!available<_ARG> ())
			return ;
		proc (_LOAD_<_ARG> (mVariant.unused)) ;
	}

	template <class _ARG>
	inline void apply (const Function<DEF<void (const _ARG &)> NONE::*> &proc) const {
		if (!available<_ARG> ())
			return ;
		proc (_LOAD_<_ARG> (mVariant.unused)) ;
	}

	template <class _RET ,class... _ARGS>
	inline void recreate (_ARGS &&...args) {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value != VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_constructible<_RET ,_ARGS &&...>::value) ;
		if (mIndex != VAR_NONE)
			template_destruct (&mVariant ,mIndex ,_NULL_<const ARGVS<TYPES...>> ()) ;
		mIndex = VAR_NONE ;
		const auto r1x = &_LOAD_<TEMP<_RET>> (mVariant.unused) ;
		mIndex = template_create (_NULL_<const ARGC<TRUE>> () ,r1x ,std::forward<_ARGS> (args)...) ;
	}

private:
	inline explicit Variant (const ARGV<NULLOPT> &) noexcept :mIndex (VAR_NONE) {}

private:
	inline static INDEX template_construct (PTR<TEMP<VARIANT>> address ,const ARGVS<> &) popping {
		return VAR_NONE ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static INDEX template_construct (PTR<TEMP<VARIANT>> address ,const ARGVS<_ARG1 ,_ARGS...> &) popping {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		const auto r1x = &_LOAD_<TEMP<_ARG1>> (address->unused) ;
		INDEX ret = template_create (_NULL_<const ARGC<std::is_default_constructible<_ARG1>::value>> () ,r1x) ;
		if (ret == VAR_NONE)
			ret = template_construct (address ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		return std::move (ret) ;
	}

	template <class _ARG>
	inline static INDEX template_init_construct (PTR<TEMP<VARIANT>> address ,_ARG &&arg) popping {
		const auto r1x = &_LOAD_<TEMP<REMOVE_CVR_TYPE<_ARG>>> (address->unused) ;
		return template_create (_NULL_<const ARGC<std::is_constructible<REMOVE_CVR_TYPE<_ARG> ,_ARG &&>::value>> () ,r1x ,std::forward<_ARG> (arg)) ;
	}

	inline static void template_destruct (PTR<TEMP<VARIANT>> address ,INDEX index ,const ARGVS<> &) noexcept {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_destruct (PTR<TEMP<VARIANT>> address ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) noexcept {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
		if (index == 0) {
			_DESTROY_ (&_LOAD_<TEMP<_ARG1>> (address->unused)) ;
		} else {
			template_destruct (address ,(index - 1) ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		}
	}

	inline static void template_copy_construct (PTR<TEMP<VARIANT>> address ,PTR<const TEMP<VARIANT>> src ,INDEX index ,const ARGVS<> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_copy_construct (PTR<TEMP<VARIANT>> address ,PTR<const TEMP<VARIANT>> src ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		if (index == 0) {
			const auto r1x = &_LOAD_<TEMP<_ARG1>> (address->unused) ;
			const auto r2x = template_create (_NULL_<const ARGC<std::is_copy_constructible<_ARG1>::value && std::is_nothrow_move_constructible<_ARG1>::value>> () ,r1x ,_LOAD_<_ARG1> (src->unused)) ;
			_DYNAMIC_ASSERT_ (r2x != VAR_NONE) ;
		} else {
			template_copy_construct (address ,src ,(index - 1) ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		}
	}

	inline static void template_move_construct (PTR<TEMP<VARIANT>> address ,PTR<TEMP<VARIANT>> src ,INDEX index ,const ARGVS<> &) noexcept {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_move_construct (PTR<TEMP<VARIANT>> address ,PTR<TEMP<VARIANT>> src ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) noexcept {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG1>::value && std::is_nothrow_move_assignable<_ARG1>::value) ;
		if (index == 0) {
			_CREATE_ (&_LOAD_<TEMP<_ARG1>> (address->unused) ,std::move (_LOAD_<_ARG1> (src->unused))) ;
		} else {
			template_move_construct (address ,src ,(index - 1) ,_NULL_<const ARGVS<_ARGS...>> ()) ;
		}
	}

	template <class _ARG1 ,class... _ARGS>
	inline static INDEX template_create (const ARGC<TRUE> & ,PTR<TEMP<_ARG1>> address ,_ARGS &&...args) popping {
		_CREATE_ (address ,std::forward<_ARGS> (args)...) ;
		return INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_ARG1> ,TYPES...>::value ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static INDEX template_create (const ARGC<FALSE> & ,PTR<TEMP<_ARG1>> address ,_ARGS &&...args) popping {
		return VAR_NONE ;
	}

public:
	inline static Variant nullopt () noexcept {
		return Variant (_NULL_<const ARGV<NULLOPT>> ()) ;
	}

	//@warn: none class shall be base on its address
	inline static void address_swap (Variant &left ,Variant &right) noexcept {
		_SWAP_ (left.mVariant ,right.mVariant) ;
		_SWAP_ (left.mIndex ,right.mIndex) ;
	}
} ;

template <class TYPE>
using Optional = Variant<TYPE> ;

template <class...>
class Tuple ;

template <>
class Tuple<> {
public:
	inline constexpr Tuple () = default ;

	inline constexpr LENGTH capacity () const {
		return 0 ;
	}

	inline constexpr BOOL equal (const Tuple &right) const {
		return TRUE ;
	}

	inline constexpr BOOL operator== (const Tuple &right) const {
		return equal (right) ;
	}

	inline constexpr BOOL operator!= (const Tuple &right) const {
		return !equal (right) ;
	}

	inline constexpr BOOL less (const Tuple &right) const {
		return FALSE ;
	}

	inline constexpr BOOL operator< (const Tuple &right) const {
		return less (right) ;
	}

	inline constexpr BOOL operator>= (const Tuple &right) const {
		return !less (right) ;
	}

	inline constexpr BOOL operator> (const Tuple &right) const {
		return right.less (*this) ;
	}

	inline constexpr BOOL operator<= (const Tuple &right) const {
		return !right.less (*this) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
class Tuple<TYPE1 ,TYPES...> :private Tuple<TYPES...> {
private:
	template <class...>
	friend class Tuple ;
	TYPE1 mData ;

public:
	inline constexpr Tuple () = default ;

	inline constexpr implicit Tuple (const REMOVE_REFERENCE_TYPE<TYPE1> &arg1 ,const REMOVE_REFERENCE_TYPE<TYPES> &...args) :Tuple<TYPES...> (std::move (args)...) ,mData (std::move (arg1)) {}

	inline constexpr implicit Tuple (REMOVE_REFERENCE_TYPE<TYPE1> &&arg1 ,REMOVE_REFERENCE_TYPE<TYPES> &&...args) : Tuple<TYPES...> (std::move (args)...) ,mData (std::move (arg1)) {}

	inline constexpr LENGTH capacity () const {
		return 1 + rest ().capacity () ;
	}

	inline TYPE1 &one () {
		return mData ;
	}

	inline constexpr const TYPE1 &one () const {
		return mData ;
	}

	inline Tuple<TYPES...> &rest () {
		return *this ;
	}

	inline constexpr const Tuple<TYPES...> &rest () const {
		return *this ;
	}

	template <INDEX _VAL>
	inline VISITOF_TRATIS_TYPE<_VAL ,TYPE1 ,TYPES...> &visit () {
		return template_visit (*this ,_NULL_<const ARGC<_VAL>> ()) ;
	}

	template <INDEX _VAL>
	inline constexpr const VISITOF_TRATIS_TYPE<_VAL ,TYPE1 ,TYPES...> &visit () const {
		return template_visit (*this ,_NULL_<const ARGC<_VAL>> ()) ;
	}

	inline constexpr BOOL equal (const Tuple &right) const {
		return one () == right.one () && rest () == right.rest () ;
	}

	inline constexpr BOOL operator== (const Tuple &right) const {
		return equal (right) ;
	}

	inline constexpr BOOL operator!= (const Tuple &right) const {
		return !equal (right) ;
	}

	inline constexpr BOOL less (const Tuple &right) const {
		if (one () != right.one ())
			return one () < right.one () ;
		return rest () < right.rest () ;
	}

	inline constexpr BOOL operator< (const Tuple &right) const {
		return less (right) ;
	}

	inline constexpr BOOL operator>= (const Tuple &right) const {
		return !less (right) ;
	}

	inline constexpr BOOL operator> (const Tuple &right) const {
		return right.less (*this) ;
	}

	inline constexpr BOOL operator<= (const Tuple &right) const {
		return !right.less (*this) ;
	}

private:
	inline static VISITOF_TRATIS_TYPE<0 ,TYPE1 ,TYPES...> &template_visit (Tuple &_self ,const ARGC<0> &) {
		return _self.one () ;
	}

	template <INDEX _VAL>
	inline static VISITOF_TRATIS_TYPE<_VAL ,TYPE1 ,TYPES...> &template_visit (Tuple &_self ,const ARGC<_VAL> &) {
		_STATIC_ASSERT_ (_VAL > 0 && _VAL < 1 + _CAPACITYOF_ (TYPES)) ;
		return Tuple<TYPES...>::template_visit (_self.rest () ,_NULL_<const ARGC<_VAL - 1>> ()) ;
	}

	inline static constexpr const VISITOF_TRATIS_TYPE<0 ,TYPE1 ,TYPES...> &template_visit (const Tuple &_self ,const ARGC<0> &) {
		return _self.one () ;
	}

	template <INDEX _VAL>
	inline static constexpr const VISITOF_TRATIS_TYPE<_VAL ,TYPE1 ,TYPES...> &template_visit (const Tuple &_self ,const ARGC<_VAL> &) {
		_STATIC_ASSERT_ (_VAL > 0 && _VAL < 1 + _CAPACITYOF_ (TYPES)) ;
		return Tuple<TYPES...>::template_visit (_self.rest () ,_NULL_<const ARGC<_VAL - 1>> ()) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class... _TYPES>
class Function<TYPE1 (TYPES...)>::ImplHolder<PTR<TYPE1 (TYPES... ,_TYPES...)>> :public Function<TYPE1 (TYPES...)>::Holder {
private:
	PTR<TYPE1 (TYPES... ,_TYPES...)> mFunctor ;
	Tuple<REMOVE_CVR_TYPE<_TYPES>...> mParameters ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (PTR<TYPE1 (TYPES... ,_TYPES...)> functor ,const REMOVE_CVR_TYPE<_TYPES> &...args) :mFunctor (functor) ,mParameters (args...) {}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping override {
		return template_apply (mFunctor ,std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)... ,mParameters) ;
	}

private:
	inline static TYPE1 template_apply (PTR<TYPE1 (TYPES... ,_TYPES...)> functor ,FORWARD_TRAITS_TYPE<TYPES> &&...args1 ,const Tuple<> &parameters ,const REMOVE_CVR_TYPE<_TYPES> &...args2) popping {
		_DEBUG_ASSERT_ (functor != NULL) ;
		return functor (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args1)... ,args2...) ;
	}

	template <class... _ARGS1 ,class... _ARGS2>
	inline static TYPE1 template_apply (PTR<TYPE1 (TYPES... ,_TYPES...)> functor ,FORWARD_TRAITS_TYPE<TYPES> &&...args1 ,const Tuple<_ARGS2...> &parameters ,const _ARGS1 &...args2) popping {
		return template_apply (functor ,std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args1)... ,parameters.rest () ,args2... ,parameters.one ()) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class... _ARGS>
inline Function<TYPE1 (TYPES...)> Function<TYPE1 (TYPES...)>::make (PTR<TYPE1 (TYPES... ,_ARGS...)> functor ,const REMOVE_CVR_TYPE<_ARGS> &...args) {
	auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>>> () ;
	ScopedHolder<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>> ANONYMOUS (sgd ,functor ,args...) ;
	const auto r1x = &_LOAD_<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>> (_XVALUE_<const PTR<TEMP<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>>> &> (sgd)) ;
	Function ret = Function (_XVALUE_<const PTR<Holder> &> (r1x)) ;
	sgd = NULL ;
	return std::move (ret) ;
}

template <class...>
class AllOfTuple ;

template <class...>
class AnyOfTuple ;

template <class... TYPES>
class AllOfTuple :private Tuple<const TYPES &...> {
private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) > 0) ;

public:
	inline AllOfTuple () = delete ;

	template <class _ARG>
	inline BOOL operator== (const _ARG &right) const {
		return template_equal (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator!= (const _ARG &right) const {
		return template_not_equal (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator< (const _ARG &right) const {
		return template_less (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator>= (const _ARG &right) const {
		return template_not_less (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator> (const _ARG &right) const {
		return template_bigger (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator<= (const _ARG &right) const {
		return template_not_bigger (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

private:
	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return arg1 == arg2 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return arg2 == arg1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return arg2 == arg1 ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return arg1 < arg2 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return arg2 > arg1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return arg2 > arg1 ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_equal (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return operator_equal (left.one () ,right) && template_equal (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_not_equal (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return !operator_equal (left.one () ,right) && template_not_equal (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_less (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return operator_less (left.one () ,right) && template_less (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_not_less (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return !operator_less (left.one () ,right) && template_not_less (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_bigger (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return operator_less (right ,left.one ()) && template_bigger (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_not_bigger (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return !operator_less (right ,left.one ()) && template_not_bigger (left.rest () ,right) ;
	}
} ;

template <class... TYPES>
class AnyOfTuple :private Tuple<const TYPES &...> {
private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) > 0) ;

public:
	inline AnyOfTuple () = delete ;

	template <class _ARG>
	inline BOOL operator== (const _ARG &right) const {
		return template_equal (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator!= (const _ARG &right) const {
		return template_not_equal (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator< (const _ARG &right) const {
		return template_less (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator>= (const _ARG &right) const {
		return template_not_less (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator> (const _ARG &right) const {
		return template_bigger (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

	template <class _ARG>
	inline BOOL operator<= (const _ARG &right) const {
		return template_not_bigger (_XVALUE_<const Tuple<const TYPES &...> &> (*this) ,right) ;
	}

private:
	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return arg1 == arg2 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return arg2 == arg1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return arg2 == arg1 ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return arg1 < arg2 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return arg2 > arg1 ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return arg2 > arg1 ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_equal (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return operator_equal (left.one () ,right) || template_equal (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_not_equal (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return !operator_equal (left.one () ,right) || template_not_equal (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_less (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return operator_less (left.one () ,right) || template_less (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_not_less (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return !operator_less (left.one () ,right) || template_not_less (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_bigger (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return operator_less (right ,left.one ()) || template_bigger (left.rest () ,right) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class _ARG3 ,class... _ARGS>
	inline static BOOL template_not_bigger (const Tuple<_ARG1 ,_ARG2 ,_ARGS...> &left ,const _ARG3 &right) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,_ARG2>::value) ;
		return !operator_less (right ,left.one ()) || template_not_bigger (left.rest () ,right) ;
	}
} ;

namespace S {
template <class _ARG1 ,class... _ARGS>
inline static AllOfTuple<_ARG1 ,_ARGS...> _ALLOF_ (const _ARG1 &arg1 ,const _ARGS &...args) {
	const auto r1x = Tuple<const _ARG1 & ,const _ARGS &...> (arg1 ,args...) ;
	return _CAST_<AllOfTuple<_ARG1 ,_ARGS...>> (r1x) ;
}

template <class _ARG1 ,class... _ARGS>
inline static AnyOfTuple<_ARG1 ,_ARGS...> _ANYOF_ (const _ARG1 &arg1 ,const _ARGS &...args) {
	const auto r1x = Tuple<const _ARG1 & ,const _ARGS &...> (arg1 ,args...) ;
	return _CAST_<AnyOfTuple<_ARG1 ,_ARGS...>> (r1x) ;
}
} ;

template <class>
class StrongRef ;

template <class>
class WeakRef ;

template <class>
class SoftRef ;

namespace stl {
template <class ,class ,class = VOID>
struct is_virtual_base_of :public false_type {} ;

template <class _ARG>
struct is_virtual_base_of_helper1 :public virtual _ARG {} ;

template <class _ARG1 ,class _ARG2>
struct is_virtual_base_of_helper2 :public _ARG2 ,public is_virtual_base_of_helper1<_ARG1> {} ;

template <class _ARG1 ,class _ARG2>
struct is_virtual_base_of<_ARG1 ,_ARG2 ,ENABLE_TYPE<_SIZEOF_ (is_virtual_base_of_helper2<_ARG1 ,_ARG2>) == (_SIZEOF_ (is_virtual_base_of_helper1<_ARG1>) - _SIZEOF_ (_ARG1) + _SIZEOF_ (_ARG2)) && _ALIGNOF_ (is_virtual_base_of_helper2<_ARG1 ,_ARG2>) == _ALIGNOF_ (_ARG2)>> :public is_base_of<_ARG1 ,_ARG2> {} ;
} ;

template <>
class WeakRef<void> {
private:
	class Holder {
	private:
		template <class>
		friend class StrongRef ;
		template <class>
		friend class WeakRef ;
		AnyRef<void> mData ;
		LENGTH mCounter ;
	} ;

private:
	template <class>
	friend class StrongRef ;
	template <class>
	friend class WeakRef ;
	SharedRef<Holder> mHolder ;

public:
	template <class _ARG>
	inline static StrongRef<_ARG> strong_from_this (PTR<_ARG> _this) ;
} ;

template <class TYPE>
class StrongRef {
private:
	using Holder = typename WeakRef<void>::Holder ;

private:
	template <class>
	friend class StrongRef ;
	friend WeakRef<void> ;
	friend WeakRef<TYPE> ;
	friend SoftRef<TYPE> ;
	SharedRef<Holder> mHolder ;
	PTR<TYPE> mPointer ;

public:
	inline StrongRef () noexcept {
		mPointer = NULL ;
	}

	//@warn: circular reference ruins StrongRef
	template <class _ARG ,class = ENABLE_TYPE<std::is_base_of<TYPE ,_ARG>::value>>
	inline implicit StrongRef (const StrongRef<_ARG> &right) :StrongRef (right.template recast<TYPE> ()) {}

	inline implicit StrongRef (const WeakRef<TYPE> &right) ;

	inline implicit StrongRef (const SoftRef<TYPE> &right) ;

	inline ~StrongRef () noexcept {
		if (mPointer == NULL)
			return ;
		if (!mHolder.exist ())
			return ;
		if (!mHolder->mData.exist ())
			return ;
		const auto r1x = --mHolder->mCounter == 0 ;
		if (r1x)
			mHolder->mData = AnyRef<void> () ;
		mPointer = NULL ;
	}

	inline StrongRef (const StrongRef &right) :StrongRef (right.mHolder ,right.mPointer) {}

	inline StrongRef &operator= (const StrongRef &right) {
		if (this != &right) {
			this->~StrongRef () ;
			new (this) StrongRef (std::move (right)) ;
		}
		return _XVALUE_<StrongRef &> (*this) ;
	}

	inline StrongRef (StrongRef &&right) noexcept {
		mHolder = std::move (right.mHolder) ;
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline StrongRef &operator= (StrongRef &&right) noexcept {
		if (this != &right) {
			this->~StrongRef () ;
			new (this) StrongRef (std::move (right)) ;
		}
		return _XVALUE_<StrongRef &> (*this) ;
	}

	inline BOOL exist () const {
		return mPointer != NULL && mHolder.exist () && mHolder->mData.exist () ;
	}

	template <class _RET>
	inline StrongRef<CAST_TRAITS_TYPE<_RET ,TYPE>> recast () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		const auto r1x = template_recast (mPointer ,_NULL_<const ARGV<CAST_TRAITS_TYPE<_RET ,TYPE>>> () ,ARGV_VOID ,ARGVP9) ;
		_DYNAMIC_ASSERT_ (EFLAG (r1x != NULL) == EFLAG (mPointer != NULL)) ;
		return StrongRef<CAST_TRAITS_TYPE<_RET ,TYPE>> (mHolder ,r1x) ;
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

	inline BOOL equal (const StrongRef &right) const {
		if (!mHolder.exist () || !right.mHolder.exist ())
			return !mHolder.exist () && !right.mHolder.exist () ;
		return &mHolder.self == &right.mHolder.self ;
	}

	inline BOOL operator== (const StrongRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const WeakRef<TYPE> &right) const ;

	inline BOOL operator== (const WeakRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const WeakRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const SoftRef<TYPE> &right) const ;

	inline BOOL operator== (const SoftRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftRef<TYPE> &right) const {
		return !equal (right) ;
	}

private:
	inline explicit StrongRef (const SharedRef<Holder> &holder ,PTR<TYPE> pointer) {
		mHolder = _COPY_ (holder) ;
		mPointer = _COPY_ (pointer) ;
		if (!exist ())
			return ;
		const auto r1x = ++mHolder->mCounter ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		(void) r1x ;
	}

private:
	inline static void template_enable_shared (const SharedRef<Holder> &holder ,PTR<WeakRef<void>> pointer) {
		pointer->mHolder = holder ;
	}

	inline static void template_enable_shared (const SharedRef<Holder> &holder ,PTR<VOID> pointer) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG>
	inline static PTR<_ARG> template_recast (PTR<TYPE> pointer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_base_of<_ARG ,TYPE>::value>> & ,const decltype (ARGVP2) &) {
		return _XVALUE_<const PTR<_ARG> &> (pointer) ;
	}

	template <class _ARG>
	inline static PTR<_ARG> template_recast (PTR<TYPE> pointer ,const ARGV<_ARG> & ,const ARGV<ENABLE_TYPE<std::is_base_of<Interface ,TYPE>::value && std::is_base_of<Interface ,_ARG>::value>> & ,const decltype (ARGVP1) &) {
		//@warn: RTTI might be different across DLL
		return dynamic_cast<PTR<_ARG>> (pointer) ;
	}

public:
	template <class... _ARGS>
	inline static StrongRef make (_ARGS &&...args) {
		auto rax = SharedRef<Holder>::make () ;
		rax->mData = AnyRef<REMOVE_CVR_TYPE<TYPE>>::make (std::forward<_ARGS> (args)...) ;
		rax->mCounter = 0 ;
		const auto r1x = &rax->mData.rebind<REMOVE_CVR_TYPE<TYPE>> ().self ;
		template_enable_shared (rax ,r1x) ;
		return StrongRef (rax ,r1x) ;
	}
} ;

template <class _ARG>
inline StrongRef<_ARG> WeakRef<void>::strong_from_this (PTR<_ARG> _this) {
	_STATIC_ASSERT_ (stl::is_virtual_base_of<WeakRef<void> ,_ARG>::value) ;
	_DEBUG_ASSERT_ (_this != NULL) ;
	const auto r1x = _XVALUE_<const PTR<CAST_TRAITS_TYPE<WeakRef<void> ,_ARG>> &> (_this) ;
	return StrongRef<_ARG> (r1x->mHolder ,_this) ;
}

template <class TYPE>
class WeakRef {
private:
	using Holder = typename WeakRef<void>::Holder ;

private:
	SharedRef<Holder> mHolder ;
	PTR<TYPE> mPointer ;

public:
	inline WeakRef () noexcept {
		mPointer = NULL ;
	}

	inline BOOL exist () const {
		return mPointer != NULL && mHolder.exist () && mHolder->mData.exist () ;
	}

	inline BOOL equal (const WeakRef &right) const {
		if (!mHolder.exist () || !right.mHolder.exist ())
			return !mHolder.exist () && !right.mHolder.exist () ;
		return &mHolder.self == &right.mHolder.self ;
	}

	inline BOOL operator== (const WeakRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const WeakRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const StrongRef<TYPE> &right) const {
		if (!mHolder.exist () || !right.mHolder.exist ())
			return !mHolder.exist () && !right.mHolder.exist () ;
		return &mHolder.self == &right.mHolder.self ;
	}

	inline BOOL operator== (const StrongRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline StrongRef<TYPE> strong () const {
		return StrongRef<TYPE> (mHolder ,mPointer) ;
	}

	inline void assign (const StrongRef<TYPE> &right) {
		mHolder = right.mHolder ;
		mPointer = right.mPointer ;
	}

	inline void operator= (const StrongRef<TYPE> &right) {
		assign (right) ;
	}
} ;

template <class TYPE>
inline StrongRef<TYPE>::StrongRef (const WeakRef<TYPE> &right) :StrongRef (right.strong ()) {}

template <class TYPE>
inline BOOL StrongRef<TYPE>::equal (const WeakRef<TYPE> &right) const {
	return right.equal (*this) ;
}

template <class TYPE>
class SoftRef {
private:
	class Pack {
	private:
		friend SoftRef ;
		StrongRef<TYPE> mData ;
		LENGTH mWeight ;
	} ;

	inline static constexpr VAR expr_log2 (VAR arg) {
		return (arg <= 0) ? VAR_NONE : (arg == 1) ? 0 : (1 + expr_log2 (arg >> 1)) ;
	}

private:
	WeakRef<TYPE> mWeakRef ;
	SharedRef<Allocator<Pack ,SFIXED>> mHeap ;
	INDEX mIndex ;

public:
	inline SoftRef () = default ;

	inline explicit SoftRef (LENGTH len) {
		mHeap = SharedRef<Allocator<Pack ,SFIXED>>::make (len) ;
	}

	inline ~SoftRef () noexcept {
		if (!linked ())
			return ;
		_DEBUG_ASSERT_ (mHeap.self[mIndex].mWeight >= 0) ;
		mHeap->free (mIndex) ;
	}

	inline SoftRef (const SoftRef &) = delete ;
	inline SoftRef &operator= (const SoftRef &) = delete ;

	inline SoftRef (SoftRef &&right) noexcept {
		mHeap = std::move (right.mHeap) ;
		mWeakRef = std::move (right.mWeakRef) ;
		mIndex = right.mIndex ;
	}

	inline SoftRef &operator= (SoftRef &&right) noexcept {
		if (this != &right) {
			this->~SoftRef () ;
			new (this) SoftRef (std::move (right)) ;
		}
		return _XVALUE_<SoftRef &> (*this) ;
	}

	LENGTH capacity () const {
		if (!mHeap.exist ())
			return 0 ;
		return mHeap->size () ;
	}

	inline BOOL exist () const {
		return mWeakRef.exist () ;
	}

	inline SoftRef copy () popping {
		SoftRef ret ;
		ret.mHeap = mHeap ;
		ret.mWeakRef = mWeakRef ;
		ret.mIndex = mIndex ;
		return std::move (ret) ;
	}

	inline BOOL equal (const SoftRef &right) const {
		return mWeakRef == right.mWeakRef ;
	}

	inline BOOL operator== (const SoftRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const StrongRef<TYPE> &right) const {
		return mWeakRef == right ;
	}

	inline BOOL operator== (const StrongRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline StrongRef<TYPE> strong () const {
		return mWeakRef.strong () ;
	}

	inline void assign (const StrongRef<TYPE> &right) {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!linked ())
				continue ;
			_DEBUG_ASSERT_ (mHeap.self[mIndex].mWeight >= 0) ;
			mHeap->free (mIndex) ;
			mIndex = VAR_NONE ;
			if (!right.exist ())
				continue ;
			mIndex = alloc (right) ;
		}
		mWeakRef = mHeap.self[mIndex].mData ;
	}

	inline void operator= (const StrongRef<TYPE> &right) {
		assign (right) ;
	}

	inline void lock () const {
		if (!linked ())
			return ;
		if (mHeap.self[mIndex].mWeight < 0)
			return ;
		if (mHeap.self[mIndex].mWeight >= 0 && mHeap.self[mIndex].mWeight < VAR32_MAX)
			mHeap.self[mIndex].mWeight++ ;
		mHeap.self[mIndex].mWeight = ~mHeap.self[mIndex].mWeight ;
	}

	inline void unlock () const {
		if (!linked ())
			return ;
		if (mHeap.self[mIndex].mWeight >= 0)
			return ;
		mHeap.self[mIndex].mWeight = ~mHeap.self[mIndex].mWeight ;
	}

	inline void clean () const {
		if (!mHeap.exist ())
			return ;
		for (INDEX i = 0 ; i < mHeap->size () ; i++) {
			if (mHeap.self[i].mWeight < 0)
				continue ;
			mHeap.self[i].mData = StrongRef<TYPE> () ;
			mHeap.self[i].mWeight = 0 ;
		}
	}

private:
	inline BOOL linked () const {
		if (!mHeap.exist ())
			return FALSE ;
		if (!mHeap->used (mIndex))
			return FALSE ;
		if (mWeakRef != mHeap.self[mIndex].mData)
			return FALSE ;
		return TRUE ;
	}

	inline INDEX alloc (const StrongRef<TYPE> &data) const popping {
		INDEX ret = VAR_NONE ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (mHeap->length () < mHeap->size ())
				continue ;
			_DYNAMIC_ASSERT_ (ret != VAR_NONE) ;
			const auto r1x = expr_log2 (mHeap.self[ret].mWeight) ;
			if (r1x <= 0)
				continue ;
			for (INDEX i = 0 ; i < mHeap->size () ; i++)
				mHeap.self[i].mWeight >>= r1x ;
		}
		if (ret == VAR_NONE)
			ret = mHeap->alloc () ;
		mHeap.self[ret].mData = data ;
		mHeap.self[ret].mWeight = 3 ;
		return std::move (ret) ;
	}

	inline INDEX min_weight_one () const {
		INDEX ret = VAR_NONE ;
		auto rax = LENGTH () ;
		for (INDEX i = 0 ; i < mHeap->size () ; i++) {
			const auto r1x = mHeap.self[i].mWeight ;
			if (r1x < 0)
				continue ;
			if (ret != VAR_NONE && rax <= r1x)
				continue ;
			ret = i ;
			rax = r1x ;
		}
		return std::move (ret) ;
	}
} ;

template <class TYPE>
inline StrongRef<TYPE>::StrongRef (const SoftRef<TYPE> &right) :StrongRef (right.strong ()) {}

template <class TYPE>
inline BOOL StrongRef<TYPE>::equal (const SoftRef<TYPE> &right) const {
	return right.equal (*this) ;
}

template <class TYPE>
class IntrusiveRef {
private:
	using RETRY_TIMES_SIZE = ARGC<128> ;

	using INTRUSIVE_TYPE = typename TYPE::INTRUSIVE_TYPE ;

	class WatcherProxy {
	private:
		friend IntrusiveRef ;
		PTR<TYPE> mPointer ;

	public:
		inline WatcherProxy () = delete ;

		inline ~WatcherProxy () noexcept {
			if (mPointer == NULL)
				return ;
			_CALL_TRY_ ([&] () {
				release (mPointer) ;
			} ,std::nothrow) ;
			mPointer = NULL ;
		}

		inline WatcherProxy (const WatcherProxy &) = delete ;
		inline WatcherProxy &operator= (const WatcherProxy &) = delete ;

		inline WatcherProxy (WatcherProxy &&) noexcept = default ;
		inline WatcherProxy &operator= (WatcherProxy &&) = delete ;

		inline implicit operator TYPE & () const & noexcept {
			_DEBUG_ASSERT_ (mPointer != NULL) ;
			return *mPointer ;
		}

		inline implicit operator TYPE & () && = delete ;

		inline implicit operator PTR<TYPE> () const & noexcept {
			return mPointer ;
		}

		inline implicit operator PTR<TYPE> () && = delete ;

	private:
		inline explicit WatcherProxy (PTR<TYPE> pointer) noexcept :mPointer (pointer) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend ScopedGuard<IntrusiveRef> ;
	std::atomic<PTR<TYPE>> mPointer ;
	std::atomic<BOOL> mSemaphore ;

public:
	inline IntrusiveRef () = delete ;

	template <class _ARG ,class = ENABLE_TYPE<std::is_same<_ARG ,PTR<TYPE>>::value>>
	inline explicit IntrusiveRef (const _ARG &address) popping :IntrusiveRef (ARGVP0) {
		//@warn: object must be from 'IntrusiveRef::make'
		acquire (address ,FALSE) ;
		update_pointer (address) ;
	}

	inline ~IntrusiveRef () noexcept {
		ScopedGuard<IntrusiveRef> ANONYMOUS (*this) ;
		const auto r1x = mPointer.exchange (NULL) ;
#ifdef __CSC_DEBUG__
		_CALL_TRY_ ([&] () {
			release (r1x) ;
		} ,std::nothrow) ;
#else
		_CALL_TRY_ ([&] () {
			release (r1x) ;
		} ,[&] () {
			(void) r1x ;
		}) ;
#endif
	}

	inline IntrusiveRef (const IntrusiveRef &) = delete ;
	inline IntrusiveRef &operator= (const IntrusiveRef &) = delete ;

	inline IntrusiveRef (IntrusiveRef &&right) noexcept :IntrusiveRef (ARGVP0) {
		ScopedGuard<IntrusiveRef> ANONYMOUS (right) ;
		const auto r1x = right.mPointer.exchange (NULL) ;
		update_pointer (r1x) ;
	}

	inline IntrusiveRef &operator= (IntrusiveRef &&) = delete ;

	inline BOOL exist () const {
		const auto r1x = mPointer.load () ;
		return r1x != NULL ;
	}

	inline IntrusiveRef copy () popping {
		ScopedGuard<IntrusiveRef> ANONYMOUS (*this) ;
		IntrusiveRef ret = IntrusiveRef (ARGVP0) ;
		const auto r1x = mPointer.load () ;
		acquire (r1x ,FALSE) ;
		ret.update_pointer (r1x) ;
		return std::move (ret) ;
	}

	inline WatcherProxy watch () popping {
		ScopedGuard<IntrusiveRef> ANONYMOUS (*this) ;
		const auto r1x = mPointer.load () ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		acquire (r1x ,FALSE) ;
		return WatcherProxy (r1x) ;
	}

private:
	inline explicit IntrusiveRef (const decltype (ARGVP0) &) noexcept :mPointer (NULL) ,mSemaphore (FALSE) {}

	inline void lock () noexcept {
		auto rax = FALSE ;
		INDEX ir = 0 ;
		while (!mSemaphore.compare_exchange_weak (rax ,TRUE)) {
			ir++ ;
			_DEBUG_ASSERT_ (ir < RETRY_TIMES_SIZE::value) ;
			std::this_thread::yield () ;
			rax = FALSE ;
		}
	}

	inline void unlock () noexcept {
		const auto r1x = mSemaphore.exchange (FALSE) ;
		_DEBUG_ASSERT_ (r1x) ;
		(void) r1x ;
	}

	inline void update_pointer (PTR<TYPE> address) noexcept {
		const auto r1x = mPointer.exchange (address) ;
		_DEBUG_ASSERT_ (r1x == NULL) ;
		(void) r1x ;
	}

private:
	inline static void acquire (PTR<TYPE> address ,BOOL init) {
		if (address == NULL)
			return ;
		if (init)
			INTRUSIVE_TYPE::intrusive_create (*address) ;
		const auto r1x = INTRUSIVE_TYPE::intrusive_attach (*address) ;
		_DEBUG_ASSERT_ (r1x >= 1 + EFLAG (!init)) ;
		(void) r1x ;
	}

	inline static void release (PTR<TYPE> address) {
		if (address == NULL)
			return ;
		const auto r1x = INTRUSIVE_TYPE::intrusive_detach (*address) ;
		_DEBUG_ASSERT_ (r1x >= 0) ;
		if (r1x > 0)
			return ;
		INTRUSIVE_TYPE::intrusive_destroy (*address) ;
		address->~TYPE () ;
		GlobalHeap::free (address) ;
	}

public:
	template <class... _ARGS>
	inline static IntrusiveRef make (_ARGS &&...args) {
		IntrusiveRef ret = IntrusiveRef (ARGVP0) ;
		auto sgd = GlobalHeap::alloc<TEMP<TYPE>> () ;
		ScopedHolder<TYPE> ANONYMOUS (sgd ,std::forward<_ARGS> (args)...) ;
		const auto r1x = &_LOAD_<TYPE> (_XVALUE_<const PTR<TEMP<TYPE>> &> (sgd)) ;
		acquire (r1x ,TRUE) ;
		ret.update_pointer (r1x) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

namespace stl {
template <class TYPE ,class = VOID>
struct is_complete_type :public false_type {} ;

template <class TYPE>
struct is_complete_type<TYPE ,ENABLE_TYPE<(_SIZEOF_ (TYPE) > 0)>> :public true_type {} ;

template <class TYPE ,class = VOID>
struct is_interface_type :public false_type {} ;

template <class TYPE>
struct is_interface_type<TYPE ,ENABLE_TYPE<_SIZEOF_ (TYPE) == _SIZEOF_ (Interface) && _ALIGNOF_ (TYPE) == _ALIGNOF_ (Interface)>> :public is_base_of<Interface ,TYPE> {} ;
} ;

template <class>
class HolderRef ;

template <class TYPE>
class HolderRef<SPECIALIZATION<TYPE ,TRUE ,FALSE>> {
private:
	_STATIC_ASSERT_ (!stl::is_complete_type<TYPE>::value) ;
	friend HolderRef<TYPE> ;
	AnyRef<void> mHolder ;
	PTR<TYPE> mPointer ;

public:
	inline HolderRef () {
		mHolder = AnyRef<TYPE>::make () ;
		mPointer = &mHolder.rebind<TYPE> ().self ;
	}
} ;

template <class TYPE>
class HolderRef<SPECIALIZATION<TYPE ,FALSE ,TRUE>> {
private:
	using SPECIALIZATION_TYPE = HolderRef<TYPE> ;

private:
	_STATIC_ASSERT_ (stl::is_interface_type<TYPE>::value) ;
	friend SPECIALIZATION_TYPE ;
	AnyRef<void> mHolder ;
	PTR<TYPE> mPointer ;

public:
	inline HolderRef () = default ;

	template <class _ARG>
	inline explicit HolderRef (const ARGV<_ARG> &) {
		mHolder = AnyRef<_ARG>::make () ;
		mPointer = SPECIALIZATION_TYPE::template_recast (&mHolder.rebind<_ARG> ().self) ;
	}
} ;

template <class TYPE>
class HolderRef :private HolderRef<SPECIALIZATION<TYPE ,!stl::is_complete_type<TYPE>::value ,stl::is_interface_type<TYPE>::value>> {
private:
	using SPECIALIZATION_BASE = HolderRef<SPECIALIZATION<TYPE ,!stl::is_complete_type<TYPE>::value ,stl::is_interface_type<TYPE>::value>> ;

private:
	friend SPECIALIZATION_BASE ;
	using SPECIALIZATION_BASE::mHolder ;
	using SPECIALIZATION_BASE::mPointer ;

public:
	inline HolderRef () = default ;

	template <class _ARG ,class = ENABLE_TYPE<std::is_base_of<TYPE ,_ARG>::value>>
	inline explicit HolderRef (const ARGV<_ARG> &) :SPECIALIZATION_BASE (_NULL_<const ARGV<_ARG>> ()) {}

	inline TYPE &to () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		return *mPointer ;
	}

	inline implicit operator TYPE & () {
		return to () ;
	}

	inline PTR<TYPE> operator-> () {
		return &to () ;
	}

	inline const TYPE &to () const {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		return *mPointer ;
	}

	inline implicit operator const TYPE & () const {
		return to () ;
	}

	inline PTR<const TYPE> operator-> () const {
		return &to () ;
	}

private:
	template <class _ARG ,class = ENABLE_TYPE<std::is_base_of<TYPE ,_ARG>::value>>
	inline static PTR<TYPE> template_recast (PTR<_ARG> arg) {
		return arg ;
	}
} ;

template <class TYPE ,class SUBJECT = TYPE>
class Monostate ;

template <class TYPE>
class Monostate<TYPE ,TYPE> {
private:
	template <class ,class>
	friend class Monostate ;
	SharedRef<TYPE> mThis ;

public:
	inline Monostate () :mThis (SharedRef<TYPE>::make ()) {}

	inline implicit Monostate (const TYPE &right) : mThis (SharedRef<TYPE>::make (std::move (right))) {}

	inline implicit Monostate (TYPE &&right) : mThis (SharedRef<TYPE>::make (std::move (right))) {}

	inline Monostate (const Monostate &) = delete ;
	inline Monostate &operator= (const Monostate &) = delete ;
	inline Monostate (Monostate &&) = delete ;
	inline Monostate &operator= (Monostate &&) = delete ;

	inline TYPE &to () const {
		return mThis.self ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

	template <class _ARG>
	inline void swap (Monostate<TYPE ,_ARG> &right) popping {
		_SWAP_ (mThis ,right.mThis) ;
	}
} ;

template <class TYPE>
class Monostate<TYPE ,void> {
private:
	template <class ,class>
	friend class Monostate ;
	SharedRef<TYPE> mThis ;

public:
	inline Monostate () = default ;

	template <class... _ARGS>
	inline explicit Monostate (_ARGS &&...args) :mThis (SharedRef<TYPE>::make (std::forward<_ARGS> (args)...)) {}

	inline Monostate (const Monostate &) = delete ;
	inline Monostate &operator= (const Monostate &) = delete ;
	inline Monostate (Monostate &&) = delete ;
	inline Monostate &operator= (Monostate &&) = delete ;

	inline TYPE &to () const {
		return mThis.self ;
	}

	inline implicit operator TYPE & () const {
		return to () ;
	}

	template <class _ARG>
	inline void swap (Monostate<TYPE ,_ARG> &right) popping {
		_SWAP_ (mThis ,right.mThis) ;
	}
} ;

class MemoryPool {
private:
	struct HEADER ;

	struct Pool :public Interface {
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual PTR<HEADER> alloc (LENGTH) popping = 0 ;
		virtual void free (PTR<HEADER> address) noexcept = 0 ;
		virtual void clean () = 0 ;
	} ;

	struct HEADER {
		alignas (8) PTR<Pool> P1 ;
		alignas (8) PTR<struct HEADER> P2 ;
	} ;

	inline static constexpr VAR expr_ceil (VAR base ,VAR align) {
		return (base / align + EFLAG (base % align != 0)) * align ;
	}

	template <class SIZE>
	class ImplPool :public Pool {
	private:
		struct BLOCK {
			PTR<struct BLOCK> next ;
			HEADER flex_data ;
		} ;

		inline static constexpr VAL64 expr_pow (VAL64 base ,LENGTH power) {
			return (power % 2 != 0) ? (base * expr_pow (base ,(power - 1))) : (power != 0) ? (_SQE_ (expr_pow (base ,(power / 2)))) : 1 ;
		}

		struct CHUNK {
			PTR<ARR<BYTE>> origin ;
			PTR<struct CHUNK> prev ;
			PTR<struct CHUNK> next ;
			LENGTH count ;
		} ;

	private:
		PTR<CHUNK> mFirst ;
		PTR<BLOCK> mFree ;
		LENGTH mSize ;
		LENGTH mLength ;

	public:
		inline ImplPool () {
			mFirst = NULL ;
			mFree = NULL ;
			mSize = 0 ;
			mLength = 0 ;
		}

		inline ~ImplPool () noexcept override {
			clear () ;
		}

		inline LENGTH size () const override {
			return mSize ;
		}

		inline LENGTH length () const override {
			return mLength ;
		}

		inline void clear () noexcept {
			for (PTR<CHUNK> i = mFirst ,ir = NULL ; i != NULL ; i = ir) {
				ir = i->next ;
				GlobalHeap::free (i->origin) ;
			}
			mFirst = NULL ;
			mFree = NULL ;
			mSize = 0 ;
			mLength = 0 ;
		}

		inline void reserve () {
			_STATIC_ASSERT_ (_ALIGNOF_ (BLOCK) == 8) ;
			_STATIC_ASSERT_ (SIZE::value % 8 == 0) ;
			if (mFree != NULL)
				return ;
#ifdef __CSC_COMPILER_GNUC__
			_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<decltype (16 - SIZE::value / 8)> ,VAR>::value) ;
#endif
			const auto r1x = LENGTH (expr_pow (VAL64 (1.25) ,_MAX_ (VAR (1) ,(16 - SIZE::value / 8))) + VAL64 (1)) ;
			const auto r2x = _ALIGNOF_ (CHUNK) + _SIZEOF_ (CHUNK) + _ALIGNOF_ (BLOCK) + r1x * (_SIZEOF_ (BLOCK) + SIZE::value) ;
			auto sgd = GlobalHeap::alloc<BYTE> (r2x) ;
			const auto r3x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (_XVALUE_<const PTR<ARR<BYTE>> &> (sgd)) ,_ALIGNOF_ (CHUNK)) ;
			const auto r4x = &_LOAD_<CHUNK> (r3x) ;
			r4x->origin = _XVALUE_<const PTR<ARR<BYTE>> &> (sgd) ;
			r4x->prev = NULL ;
			r4x->next = mFirst ;
			r4x->count = r1x ;
			if (mFirst != NULL)
				mFirst->prev = r4x ;
			mFirst = r4x ;
			mSize += r1x * SIZE::value ;
			const auto r5x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (r3x) + _SIZEOF_ (CHUNK) ,_ALIGNOF_ (BLOCK)) ;
			for (INDEX i = 0 ; i < mFirst->count ; i++) {
				const auto r6x = &_LOAD_<BLOCK> (r5x + i * (_SIZEOF_ (BLOCK) + SIZE::value)) ;
				r6x->next = mFree ;
				mFree = r6x ;
			}
			sgd = NULL ;
		}

		inline PTR<HEADER> alloc (LENGTH len) popping override {
			_DEBUG_ASSERT_ (len <= SIZE::value) ;
			reserve () ;
			const auto r1x = _COPY_ (mFree) ;
			mFree = r1x->next ;
			mLength += SIZE::value ;
			const auto r2x = _CAST_<TEMP<PTR<BLOCK>>> (VAR_USED) ;
			r1x->next = _LOAD_<PTR<BLOCK>> (&r2x) ;
			return &r1x->flex_data ;
		}

		inline void free (PTR<HEADER> address) noexcept override {
			_DEBUG_ASSERT_ (address != NULL) ;
			const auto r1x = &_OFFSET_ (&BLOCK::flex_data ,*address) ;
			_DEBUG_ASSERT_ (_ADDRESS_ (r1x->next) == VAR_USED) ;
			r1x->next = mFree ;
			mFree = r1x ;
			mLength -= SIZE::value ;
		}

		inline void clean () override {
			if (mSize == mLength)
				return ;
			for (PTR<CHUNK> i = mFirst ,ir = NULL ; i != NULL ; i = ir) {
				ir = i->next ;
				if (!empty_node (i))
					continue ;
				auto &r1 = (i->prev != NULL) ? (i->prev->next) : mFirst ;
				r1 = i->next ;
				if (i->next != NULL)
					i->next->prev = i->prev ;
				mSize -= i->count * SIZE::value ;
				GlobalHeap::free (i) ;
			}
		}

	private:
		inline BOOL empty_node (PTR<const CHUNK> node) const {
			const auto r1x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (node) + _SIZEOF_ (CHUNK) ,_ALIGNOF_ (BLOCK)) ;
			for (INDEX i = 0 ; i < node->count ; i++)
				if (_ADDRESS_ (_LOAD_<BLOCK> (r1x + i * (_SIZEOF_ (BLOCK) + SIZE::value)).next) == VAR_USED)
					return FALSE ;
			return TRUE ;
		}
	} ;

	class HugePool :public Pool {
	private:
		struct BLOCK {
			PTR<ARR<BYTE>> origin ;
			PTR<struct BLOCK> prev ;
			PTR<struct BLOCK> next ;
			LENGTH count ;
			HEADER flex_data ;
		} ;

	private:
		PTR<BLOCK> mFirst ;
		LENGTH mSize ;

	public:
		inline HugePool () {
			mFirst = NULL ;
			mSize = 0 ;
		}

		inline ~HugePool () noexcept override {
			clear () ;
		}

		inline LENGTH size () const override {
			return mSize ;
		}

		inline LENGTH length () const override {
			return mSize ;
		}

		inline void clear () noexcept {
			for (PTR<BLOCK> i = mFirst ,ir = NULL ; i != NULL ; i = ir) {
				ir = i->next ;
				GlobalHeap::free (i->origin) ;
			}
			mFirst = NULL ;
			mSize = 0 ;
		}

		inline PTR<HEADER> alloc (LENGTH len) popping override {
			const auto r1x = _ALIGNOF_ (BLOCK) + _SIZEOF_ (BLOCK) + len ;
			auto sgd = GlobalHeap::alloc<BYTE> (r1x) ;
			const auto r2x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (_XVALUE_<const PTR<ARR<BYTE>> &> (sgd)) ,_ALIGNOF_ (BLOCK)) ;
			const auto r3x = &_LOAD_<BLOCK> (r2x) ;
			r3x->origin = _XVALUE_<const PTR<ARR<BYTE>> &> (sgd) ;
			r3x->prev = NULL ;
			r3x->next = mFirst ;
			r3x->count = len ;
			if (mFirst != NULL)
				mFirst->prev = r3x ;
			mFirst = r3x ;
			mSize += len ;
			sgd = NULL ;
			return &r3x->flex_data ;
		}

		inline void free (PTR<HEADER> address) noexcept override {
			_DEBUG_ASSERT_ (address != NULL) ;
			const auto r1x = &_OFFSET_ (&BLOCK::flex_data ,*address) ;
			auto &r1 = (r1x->prev != NULL) ? (r1x->prev->next) : mFirst ;
			r1 = r1x->next ;
			if (r1x->next != NULL)
				r1x->next->prev = r1x->prev ;
			mSize -= r1x->count ;
			GlobalHeap::free (r1x->origin) ;
		}

		inline void clean () override {
			_STATIC_WARNING_ ("noop") ;
		}
	} ;

private:
	ImplPool<ARGC<8>> mPool1 ;
	ImplPool<ARGC<16>> mPool2 ;
	ImplPool<ARGC<24>> mPool3 ;
	ImplPool<ARGC<32>> mPool4 ;
	ImplPool<ARGC<40>> mPool5 ;
	ImplPool<ARGC<48>> mPool6 ;
	ImplPool<ARGC<56>> mPool7 ;
	ImplPool<ARGC<64>> mPool8 ;
	ImplPool<ARGC<72>> mPool9 ;
	ImplPool<ARGC<80>> mPool10 ;
	ImplPool<ARGC<88>> mPool11 ;
	ImplPool<ARGC<96>> mPool12 ;
	ImplPool<ARGC<104>> mPool13 ;
	ImplPool<ARGC<112>> mPool14 ;
	ImplPool<ARGC<120>> mPool15 ;
	ImplPool<ARGC<128>> mPool16 ;
	HugePool mPool17 ;
	Buffer<PTR<Pool> ,ARGC<17>> mPool ;

public:
	inline MemoryPool () {
		mPool = Buffer<PTR<Pool> ,ARGC<17>> ({
			&mPool1 ,&mPool2 ,&mPool3 ,&mPool4 ,&mPool5 ,&mPool6 ,&mPool7 ,&mPool8 ,
			&mPool9 ,&mPool10 ,&mPool11 ,&mPool12 ,&mPool13 ,&mPool14 ,&mPool15 ,&mPool16 ,
			&mPool17}) ;
	}

	inline LENGTH size () const {
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mPool.size () ; i++)
			ret += mPool[i]->size () ;
		return std::move (ret) ;
	}

	inline LENGTH length () const {
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mPool.size () ; i++)
			ret += mPool[i]->length () ;
		return std::move (ret) ;
	}

	//@warn: held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<_RET> alloc () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		const auto r1x = _SIZEOF_ (_RET) + _MAX_ (_ALIGNOF_ (_RET) - 8 ,VAR_ZERO) ;
		INDEX ix = _MIN_ ((r1x - 1) / 8 ,_SIZEOF_ (HEADER)) ;
		const auto r2x = mPool[ix]->alloc (r1x) ;
		const auto r3x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (r2x) + _SIZEOF_ (HEADER) ,_ALIGNOF_ (_RET)) - _SIZEOF_ (HEADER) ;
		const auto r4x = &_LOAD_<HEADER> (r3x) ;
		r4x->P1 = mPool[ix] ;
		r4x->P2 = r2x ;
		return &_LOAD_<_RET> (r3x + _SIZEOF_ (HEADER)) ;
	}

	//@warn: held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<ARR<_RET>> alloc (LENGTH len) popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		const auto r1x = len * _SIZEOF_ (_RET) + _MAX_ (_ALIGNOF_ (_RET) - 8 ,VAR_ZERO) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		INDEX ix = _MIN_ ((r1x - 1) / 8 ,_SIZEOF_ (HEADER)) ;
		const auto r2x = mPool[ix]->alloc (r1x) ;
		const auto r3x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (r2x) + _SIZEOF_ (HEADER) ,_ALIGNOF_ (_RET)) - _SIZEOF_ (HEADER) ;
		const auto r4x = &_LOAD_<HEADER> (r3x) ;
		r4x->P1 = mPool[ix] ;
		r4x->P2 = r2x ;
		return &_LOAD_<ARR<_RET>> (r3x + _SIZEOF_ (HEADER)) ;
	}

	template <class _ARG>
	inline void free (const PTR<_ARG> &address) noexcept {
		_STATIC_ASSERT_ (std::is_pod<REMOVE_ARRAY_TYPE<_ARG>>::value) ;
		const auto r1x = &_NULL_<BYTE> () + _ADDRESS_ (address) - _SIZEOF_ (HEADER) ;
		const auto r2x = &_LOAD_<HEADER> (r1x) ;
		INDEX ix = _MEMCHR_ (mPool.self ,mPool.size () ,r2x->P1) ;
		_DEBUG_ASSERT_ (ix != VAR_NONE) ;
		mPool[ix]->free (r2x->P2) ;
	}

	inline void clean () {
		for (INDEX i = 0 ; i < mPool.size () ; i++)
			mPool[i]->clean () ;
	}
} ;

template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void> final :private Wrapped<void> {
private:
	class NodeA {
	private:
		template <class>
		friend class GlobalStatic ;
		FLAG mUUID ;
		PTR<class NodeA> mRight ;
		BOOL mReadOnly ;
		VAR mData ;

	public:
		inline NodeA () = delete ;

		inline explicit NodeA (FLAG uuid) :mUUID (uuid) {}
	} ;

	using GUID_TYPE = TEMP<BYTE[DEFAULT_RECURSIVE_SIZE::value]> ;

	class NodeB {
	private:
		template <class>
		friend class GlobalStatic ;
		GUID_TYPE mGUID ;
		PTR<class NodeB> mRight ;
		BOOL mReadOnly ;
		PTR<NONE> mData ;

	public:
		inline NodeB () = delete ;

		inline explicit NodeB (const GUID_TYPE &guid) :mGUID (guid) {}
	} ;

	class Pack {
	public:
		using INTRUSIVE_TYPE = GlobalStatic ;

	private:
		template <class>
		friend class GlobalStatic ;
		friend IntrusiveRef<Pack> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		Monostate<std::mutex> mNodeMutex ;
		LENGTH mALength ;
		PTR<NodeA> mAFirst ;
		LENGTH mBLength ;
		PTR<NodeB> mBFirst ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Pack> ;

private:
	inline static DEF<PTR<NONE> (PTR<NONE> ,PTR<NONE>) popping> unique_atomic_address ;

	template <class _ARG>
	inline static Pack &unique_heap (const ARGV<_ARG> &) popping {
		return _CACHE_ ([] () {
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = AutoRef<IntrusiveRef<Pack>> () ;
			if (rax == NULL) {
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = AutoRef<IntrusiveRef<Pack>>::make (IntrusiveRef<Pack>::make ()) ;
				const auto r1x = rbx->watch () ;
				const auto r2x = &_LOAD_<NONE> (_XVALUE_<const PTR<Pack> &> (r1x)) ;
				rax = unique_atomic_address (NULL ,r2x) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			const auto r3x = &_LOAD_<Pack> (rax) ;
			return IntrusiveRef<Pack> (r3x).watch () ;
		}) ;
	}

	inline static PTR<NodeA> static_new_node (Pack &_self ,FLAG uuid) popping {
		//@warn: sure 'GlobalHeap' can be used across DLL
		auto sgd = GlobalHeap::alloc<TEMP<NodeA>> () ;
		ScopedHolder<NodeA> ANONYMOUS (sgd ,uuid) ;
		PTR<NodeA> ret = &_LOAD_<NodeA> (_XVALUE_<const PTR<TEMP<NodeA>> &> (sgd)) ;
		_DEBUG_ASSERT_ (ret != NULL) ;
		ret->mRight = _self.mAFirst ;
		_self.mAFirst = ret ;
		_self.mALength++ ;
		sgd = NULL ;
		return std::move (ret) ;
	}

	inline static PTR<NodeB> static_new_node (Pack &_self ,const GUID_TYPE &guid) popping {
		//@warn: sure 'GlobalHeap' can be used across DLL
		auto sgd = GlobalHeap::alloc<TEMP<NodeB>> () ;
		ScopedHolder<NodeB> ANONYMOUS (sgd ,guid) ;
		PTR<NodeB> ret = &_LOAD_<NodeB> (_XVALUE_<const PTR<TEMP<NodeB>> &> (sgd)) ;
		_DEBUG_ASSERT_ (ret != NULL) ;
		ret->mRight = _self.mBFirst ;
		_self.mBFirst = ret ;
		_self.mBLength++ ;
		sgd = NULL ;
		return std::move (ret) ;
	}

	inline static PTR<NodeA> static_find_node (Pack &_self ,FLAG uuid) {
		for (PTR<NodeA> i = _self.mAFirst ; i != NULL ; i = i->mRight)
			if (i->mUUID == uuid)
				return i ;
		return NULL ;
	}

	inline static PTR<NodeB> static_find_node (Pack &_self ,const GUID_TYPE &guid) {
		for (PTR<NodeB> i = _self.mBFirst ; i != NULL ; i = i->mRight)
			if (_MEMEQUAL_ (i->mGUID.unused ,PTRTOARR[&guid.unused[0]]))
				return i ;
		return NULL ;
	}

	inline static void intrusive_create (Pack &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
		_self.mCounter.self = 0 ;
		_self.mALength = 0 ;
		_self.mAFirst = NULL ;
		_self.mBLength = 0 ;
		_self.mBFirst = NULL ;
	}

	inline static void intrusive_destroy (Pack &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
		for (PTR<NodeA> i = _self.mAFirst ,ir = NULL ; i != NULL ; i = ir) {
			ir = i->mRight ;
			i->~NodeA () ;
			GlobalHeap::free (i) ;
		}
		_self.mALength = 0 ;
		_self.mAFirst = NULL ;
		for (PTR<NodeB> i = _self.mBFirst ,ir = NULL ; i != NULL ; i = ir) {
			ir = i->mRight ;
			i->~NodeB () ;
			GlobalHeap::free (i) ;
		}
		_self.mBLength = 0 ;
		_self.mBFirst = NULL ;
	}

	inline static LENGTH intrusive_attach (Pack &_self) popping {
		return ++_self.mCounter.self ;
	}

	inline static LENGTH intrusive_detach (Pack &_self) popping {
		return --_self.mCounter.self ;
	}
} ;

template <FLAG UUID>
class GlobalStatic<ARGC<UUID>> final :private Wrapped<void> {
public:
	inline static void init (VAR data ,BOOL read_only) {
		auto &r1 = GlobalStatic<void>::unique_heap (ARGVP0) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,UUID) ;
		if (r1x != NULL)
			return ;
		const auto r2x = GlobalStatic<void>::static_new_node (r1 ,UUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		r2x->mReadOnly = read_only ;
		r2x->mData = data ;
	}

	inline static VAR load () popping {
		auto &r1 = GlobalStatic<void>::unique_heap (ARGVP0) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,UUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		return r1x->mData ;
	}

	inline static VAR compare_and_swap (VAR expect ,VAR data) popping {
		auto &r1 = GlobalStatic<void>::unique_heap (ARGVP0) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,UUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		_DEBUG_ASSERT_ (!r1x->mReadOnly) ;
		if (r1x->mData == expect)
			r1x->mData = data ;
		return r1x->mData ;
	}

	inline static void save (VAR data) {
		auto &r1 = GlobalStatic<void>::unique_heap (ARGVP0) ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1 ,UUID) ;
		if (rax == NULL) {
			rax = GlobalStatic<void>::static_new_node (r1 ,UUID) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DEBUG_ASSERT_ (!rax->mReadOnly) ;
		rax->mData = data ;
	}
} ;

template <class TYPE>
class GlobalStatic<ARGV<TYPE>> final :private Wrapped<void> {
private:
	class Pack {
	public:
		using INTRUSIVE_TYPE = GlobalStatic ;

	public:
		friend IntrusiveRef<Pack> ;
		Monostate<std::atomic<LENGTH>> mCounter ;
		TYPE mHolder ;
	} ;

	using GUID_TYPE = typename GlobalStatic<void>::GUID_TYPE ;

private:
	_STATIC_ASSERT_ (std::is_class<TYPE>::value) ;
	friend IntrusiveRef<Pack> ;

private:
	inline static void intrusive_create (Pack &_self) {
		_self.mCounter.self = 0 ;
	}

	inline static void intrusive_destroy (Pack &_self) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline static LENGTH intrusive_attach (Pack &_self) popping {
		return ++_self.mCounter.self ;
	}

	inline static LENGTH intrusive_detach (Pack &_self) popping {
		return --_self.mCounter.self ;
	}

	inline static GUID_TYPE guid_from_typeid_name () {
		GUID_TYPE ret ;
		const auto r1x = &_NULL_<BYTE> () + _ADDRESS_ (typeid (TYPE).name ()) ;
		const auto r2x = _MEMCHR_ (PTRTOARR[&r1x[0]] ,DEFAULT_HUGEBUFFER_SIZE::value ,BYTE (0X00)) ;
		_DEBUG_ASSERT_ (r2x > 0 && r2x < _SIZEOF_ (decltype (ret))) ;
		const auto r3x = _MIN_ (r2x ,_SIZEOF_ (decltype (ret))) ;
		_MEMCOPY_ (PTRTOARR[&_ZERO_ (ret).unused[0]] ,PTRTOARR[&r1x[0]] ,r3x) ;
		return std::move (ret) ;
	}

public:
	inline static TYPE &echo (const PTR<void (TYPE &)> &init_proc) popping {
		auto &r1 = _CACHE_ ([&] () {
			auto &r2 = GlobalStatic<void>::unique_heap (ARGVP0) ;
			ScopedGuard<std::mutex> ANONYMOUS (r2.mNodeMutex) ;
			const auto r1x = guid_from_typeid_name () ;
			auto rax = GlobalStatic<void>::static_find_node (r2 ,r1x) ;
			auto rbx = AutoRef<IntrusiveRef<Pack>> () ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				if (rax != NULL)
					continue ;
				rax = GlobalStatic<void>::static_new_node (r2 ,r1x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = AutoRef<IntrusiveRef<Pack>>::make (IntrusiveRef<Pack>::make ()) ;
				const auto r2x = rbx->watch () ;
				const auto r3x = _XVALUE_<const PTR<Pack> &> (r2x) ;
				rax->mData = &_LOAD_<NONE> (r3x) ;
				if (init_proc == NULL)
					continue ;
				init_proc (r3x->mHolder) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			const auto r4x = &_LOAD_<Pack> (rax->mData) ;
			return IntrusiveRef<Pack> (r4x).watch () ;
		}) ;
		return _XVALUE_<Pack &> (r1).mHolder ;
	}
} ;

class Object ;

struct Objective :public Interface {
	virtual StrongRef<Object> clone () const = 0 ;
} ;

class Object :public Objective ,public virtual WeakRef<void> {
private:
	class Metadata {
	private:
		friend Object ;
		LENGTH mObjectSize ;
		LENGTH mObjectAlign ;
		FLAG mTypeID ;
		Function<void (PTR<NONE>)> mConstrutor ;
		Function<void (PTR<NONE>)> mDestructor ;

	private:
		inline Metadata () = delete ;

		template <class _ARG>
		inline explicit Metadata (const ARGV<_ARG> &) {
			_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG> ,_ARG>::value) ;
			mObjectSize = _SIZEOF_ (_ARG) ;
			mObjectAlign = _ALIGNOF_ (_ARG) ;
			mTypeID = _TYPEID_<_ARG> () ;
			mConstrutor = _XVALUE_<const PTR<void (PTR<NONE>)> &> ([] (PTR<NONE> address) {
				_CREATE_ (&_LOAD_<TEMP<_ARG>> (address)) ;
			}) ;
			mDestructor = _XVALUE_<const PTR<void (PTR<NONE>)> &> ([] (PTR<NONE> address) {
				_DESTROY_ (&_LOAD_<TEMP<_ARG>> (address)) ;
			}) ;
		}
	} ;

public:
	inline Object () = delete ;

	template <class _ARG>
	inline explicit Object (PTR<_ARG> deriver) {
		_STATIC_ASSERT_ (std::is_base_of<Object ,_ARG>::value) ;
		_STATIC_ASSERT_ (!std::is_same<REMOVE_CVR_TYPE<_ARG> ,Object>::value) ;
		_DEBUG_ASSERT_ (_ADDRESS_ (this) == _ADDRESS_ (deriver)) ;
	}

	inline StrongRef<Object> clone () const override {
		return StrongRef<Object> () ;
	}
} ;

template <class TYPE1 ,class TYPE2>
class Serializer {
private:
	struct Binder :public Interface {
		virtual void friend_visit (TYPE1 &visitor ,TYPE2 &context) const = 0 ;
	} ;

	template <class...>
	class ImplBinder ;

	class Member {
	private:
		friend Serializer ;
		const Serializer &mBase ;
		TYPE2 &mContext ;

	public:
		inline Member () = delete ;

		inline Member (const Member &) = delete ;

		inline Member (Member &&) noexcept = default ;

		inline void friend_visit (TYPE1 &visitor) const & = delete ;

		inline void friend_visit (TYPE1 &visitor) && popping {
			mBase.mBinder->friend_visit (visitor ,mContext) ;
		}

	private:
		inline explicit Member (const Serializer &base ,TYPE2 &context) :mBase (base) ,mContext (context) {}
	} ;

private:
	StrongRef<const Binder> mBinder ;

public:
	inline Serializer () = delete ;

	template <class... _ARGS>
	inline explicit Serializer (const DEF<_ARGS TYPE2::*> &...args) :mBinder (StrongRef<const ImplBinder<_ARGS...>>::make (args...)) {}

	inline Member operator() (TYPE2 &context) const {
		_DEBUG_ASSERT_ (mBinder.exist ()) ;
		return Member (*this ,context) ;
	}
} ;

template <class TYPE1 ,class TYPE2>
template <class... _TYPES>
class Serializer<TYPE1 ,TYPE2>::ImplBinder :public Binder {
private:
	Tuple<DEF<_TYPES TYPE2::*>...> mMember ;

public:
	inline ImplBinder () = delete ;

	inline explicit ImplBinder (const DEF<_TYPES TYPE2::*> &...args) :mMember (args...) {}

	inline void friend_visit (TYPE1 &visitor ,TYPE2 &context) const popping override {
		template_visit (visitor ,context ,mMember) ;
	}

private:
	inline static void template_visit (TYPE1 &visitor ,TYPE2 &context ,const Tuple<> &members) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class... _ARGS>
	inline static void template_visit (TYPE1 &visitor ,TYPE2 &context ,const Tuple<DEF<_ARGS TYPE2::*>...> &members) {
		visitor.visit (context.*members.one ()) ;
		template_visit (visitor ,context ,members.rest ()) ;
	}
} ;

#ifdef __CSC_COMPILER_GNUC__
template <class TYPE1 ,class TYPE2>
inline void Serializer<TYPE1 ,TYPE2>::Binder::friend_visit (TYPE1 &visitor ,TYPE2 &context) const popping {
	//@error: g++4.8 is too useless to compile without hint when 'TYPE1' becomes a function-local-type
	_DEBUG_ASSERT_ (FALSE) ;
}
#endif
} ;