#pragma once

#ifndef __CSC_EXT__
#define __CSC_EXT__
#endif

#include "csc.hpp"

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
		const auto r1x = (right >= 0) ? (DATA (0)) : (DATA (-1)) ;
		v2i0 = r1x ;
	}

	inline explicit operator VAR32 () const {
		return VAR32 (v2i1) ;
	}

	inline explicit operator VAR64 () const {
		return VAR64 (v2i1) ;
	}

	inline BOOL operator== (const VAR128 &right) const {
		if (v2i1 != right.v2i1)
			return FALSE ;
		if (v2i0 != right.v2i0)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator!= (const VAR128 &right) const {
		return !operator== (right) ;
	}

	inline BOOL operator< (const VAR128 &right) const {
		const auto r1x = _CAST_<VAR64> (v2i0) ;
		const auto r2x = _CAST_<VAR64> (right.v2i0) ;
		if (r1x < r2x)
			return TRUE ;
		if (r1x > r2x)
			return FALSE ;
		if (v2i1 >= right.v2i1)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator>= (const VAR128 &right) const {
		return !operator< (right) ;
	}

	inline BOOL operator> (const VAR128 &right) const {
		return right.operator< (*this) ;
	}

	inline BOOL operator<= (const VAR128 &right) const {
		return !operator> (right) ;
	}

	inline VAR128 operator& (const VAR128 &) const = delete ;

	inline VAR128 &operator&= (const VAR128 &) = delete ;

	inline VAR128 operator| (const VAR128 &) const = delete ;

	inline VAR128 &operator|= (const VAR128 &) = delete ;

	inline VAR128 operator^ (const VAR128 &) const = delete ;

	inline VAR128 &operator^= (const VAR128 &) = delete ;

	inline VAR128 operator~ () const = delete ;

	inline VAR128 operator>> (VAR) const = delete ;

	inline VAR128 &operator>>= (VAR) = delete ;

	inline VAR128 operator<< (VAR) const = delete ;

	inline VAR128 &operator<<= (VAR) = delete ;

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
		_DYNAMIC_ASSERT_ (right.v2i1 != 0 || right.v2i0 != 0) ;
		VAR128 ret = 0 ;
		const auto r1x = BOOL (_CAST_<VAR64> (v2i0) >= 0) ;
		const auto r2x = BOOL (_CAST_<VAR64> (right.v2i0) >= 0) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (!r1x)
				discard ;
			if (right.v4i0 != 0)
				discard ;
			if (right.v4i1 != 0)
				discard ;
			if (right.v4i2 != 0)
				discard ;
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
		} ,[&] (BOOL &if_flag) {
			if (r1x)
				discard ;
			if (v2i0 == DATA (VAR64_MIN) && v2i1 == 0)
				discard ;
			ret = -(-*this / right) ;
		} ,[&] (BOOL &if_flag) {
			if (r1x)
				discard ;
			ret = -(-(*this + right) / right + 1) ;
		} ,[&] (BOOL &if_flag) {
			if (r2x)
				discard ;
			if (right.v2i0 == DATA (VAR64_MIN) && right.v2i1 == 0)
				discard ;
			ret = *this / -right ;
		} ,[&] (BOOL &if_flag) {
			if (r2x)
				discard ;
			ret = VAR128 (0) ;
		} ,[&] (BOOL &if_flag) {
			ret = slow_divide (*this ,right) ;
		}) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator/= (const VAR128 &right) {
		*this = *this / right ;
		return *this ;
	}

	inline VAR128 operator% (const VAR128 &right) const {
		_DYNAMIC_ASSERT_ (right.v2i1 != 0 || right.v2i0 != 0) ;
		VAR128 ret = 0 ;
		const auto r1x = BOOL (_CAST_<VAR64> (v2i0) >= 0) ;
		const auto r2x = BOOL (_CAST_<VAR64> (right.v2i0) >= 0) ;
		_CALL_IF_ ([&] (BOOL &if_flag) {
			if (!r1x)
				discard ;
			if (right.v4i0 != 0)
				discard ;
			if (right.v4i1 != 0)
				discard ;
			if (right.v4i2 != 0)
				discard ;
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
		} ,[&] (BOOL &if_flag) {
			if (r1x)
				discard ;
			if (v2i0 == DATA (VAR64_MIN) && v2i1 == 0)
				discard ;
			ret = -(-*this % right) ;
		} ,[&] (BOOL &if_flag) {
			if (r1x)
				discard ;
			ret = -(-(*this + right) % right) ;
		} ,[&] (BOOL &if_flag) {
			if (r2x)
				discard ;
			if (right.v2i0 == DATA (VAR64_MIN) && right.v2i1 == 0)
				discard ;
			ret = *this % -right ;
		} ,[&] (BOOL &if_flag) {
			if (r2x)
				discard ;
			ret = *this ;
		} ,[&] (BOOL &if_flag) {
			ret = right - slow_divide (*this ,right) * right ;
		}) ;
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
			auto &r1 = (r2x < y) ? (rax[0]) : (rax[1]) ;
			const auto r3x = (r2x < y) ? (ret + 1) : (ret - 1) ;
			r1 = r3x ;
		}
		ret -= EFLAG (ret * x > y) ;
		return std::move (ret) ;
	}

private:
	inline DATA &m_v2i0 () & {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[0]] ;
	}

	inline const DATA &m_v2i0 () const & {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[0]] ;
	}

	inline DATA &m_v2i0 () && = delete ;

	inline DATA &m_v2i1 () & {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[1]] ;
	}

	inline const DATA &m_v2i1 () const & {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mData)[_CAST_<BYTE[2]> (r1x)[1]] ;
	}

	inline DATA &m_v2i1 () && = delete ;

	inline CHAR &m_v4i0 () & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[0]] ;
	}

	inline const CHAR &m_v4i0 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[0]] ;
	}

	inline CHAR &m_v4i0 () && = delete ;

	inline CHAR &m_v4i1 () & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[1]] ;
	}

	inline const CHAR &m_v4i1 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[1]] ;
	}

	inline CHAR &m_v4i1 () && = delete ;

	inline CHAR &m_v4i2 () & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[2]] ;
	}

	inline const CHAR &m_v4i2 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[2]] ;
	}

	inline CHAR &m_v4i2 () && = delete ;

	inline CHAR &m_v4i3 () & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[3]] ;
	}

	inline const CHAR &m_v4i3 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mData)[_CAST_<BYTE[4]> (r1x)[3]] ;
	}

	inline CHAR &m_v4i3 () && = delete ;

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

class VAL128 {
#pragma region
#pragma push_macro ("v2f0")
#pragma push_macro ("v2f1")
#undef v2f0
#undef v2f1
#define v2f0 m_v2f0 ()
#define v2f1 m_v2f1 ()

private:
	MEGA mData ;

public:
	inline VAL128 () = default ;

	inline implicit VAL128 (VAL64 right) {
		v2f1 = right ;
		v2f0 = 1 ;
	}

	inline explicit operator VAL32 () const {
		return VAL32 (v2f1) ;
	}

	inline explicit operator VAL64 () const {
		return VAL64 (v2f1) ;
	}

	inline BOOL operator== (const VAL128 &) const = delete ;

	inline BOOL operator!= (const VAL128 &) const = delete ;

	inline BOOL operator< (const VAL128 &right) const {
		if (v2f1 >= right.v2f1)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator>= (const VAL128 &) const = delete ;

	inline BOOL operator> (const VAL128 &right) const {
		return right.operator< (*this) ;
	}

	inline BOOL operator<= (const VAL128 &) const = delete ;

	inline VAL128 operator+ (const VAL128 &right) const {
		VAL128 ret = 0 ;
		ret.v2f1 = v2f1 + right.v2f1 ;
		return std::move (ret) ;
	}

	inline VAL128 &operator+= (const VAL128 &right) {
		v2f1 += right.v2f1 ;
		return *this ;
	}

	inline VAL128 operator- (const VAL128 &right) const {
		VAL128 ret = 0 ;
		ret.v2f1 = v2f1 - right.v2f1 ;
		return std::move (ret) ;
	}

	inline VAL128 &operator-= (const VAL128 &right) {
		v2f1 -= right.v2f1 ;
		return *this ;
	}

	inline VAL128 operator* (const VAL128 &right) const {
		VAL128 ret = 0 ;
		ret.v2f1 = v2f1 * right.v2f1 ;
		return std::move (ret) ;
	}

	inline VAL128 &operator*= (const VAL128 &right) {
		v2f1 *= right.v2f1 ;
		return *this ;
	}

	inline VAL128 operator/ (const VAL128 &right) const {
		VAL128 ret = 0 ;
		ret.v2f1 = v2f1 / right.v2f1 ;
		return std::move (ret) ;
	}

	inline VAL128 &operator/= (const VAL128 &right) {
		v2f1 *= right.v2f1 ;
		return *this ;
	}

	inline VAL128 operator+ () const {
		return *this ;
	}

	inline VAL128 operator- () const {
		VAL128 ret = 0 ;
		ret.v2f1 = -v2f1 ;
		return std::move (ret) ;
	}

private:
	inline VAL64 &m_v2f0 () & {
		return _CAST_<VAL64[2]> (mData)[0] ;
	}

	inline const VAL64 &m_v2f0 () const & {
		return _CAST_<VAL64[2]> (mData)[0] ;
	}

	inline VAL64 &m_v2f0 () && = delete ;

	inline VAL64 &m_v2f1 () & {
		return _CAST_<VAL64[2]> (mData)[1] ;
	}

	inline const VAL64 &m_v2f1 () const & {
		return _CAST_<VAL64[2]> (mData)[1] ;
	}

	inline VAL64 &m_v2f1 () && = delete ;

#undef v2f0
#undef v2f1
#pragma pop_macro ("v2f0")
#pragma pop_macro ("v2f1")
#pragma endregion
} ;

namespace U {
struct OPERATOR_CRC32 {
	inline static constexpr BOOL ensure_index (INDEX index ,INDEX ib ,INDEX ie) {
		return BOOL (index >= ib && index < ie) ;
	}

	inline static constexpr CHAR expr_crc32_next (CHAR each) {
		return ((each & CHAR (0X00000001)) != 0) ? (CHAR (0)) : (CHAR (0XEDB88320) ^ (each >> 1)) ;
	}

	inline static constexpr CHAR expr_crc32_table (CHAR each ,INDEX it) {
		return (it < 0 || it >= 8) ? each : expr_crc32_table (expr_crc32_next (each) ,(it + 1)) ;
	}

	template <LENGTH _VAL1>
	inline static constexpr CHAR expr_crc32_hash (const DEF<STR[_VAL1]> &stri ,CHAR each ,INDEX it) {
		return (it < 0 || it >= _VAL1) ? each : expr_crc32_hash (stri ,(expr_crc32_table (INDEX ((CHAR (each) ^ CHAR (stri[it])) & CHAR (0X000000FF)) ,0) ^ (each >> 8)) ,(it + 1)) ;
	}

	template <LENGTH _VAL1>
	inline static constexpr FLAG invoke (const DEF<STR[_VAL1]> &stri) {
		return FLAG (VAR32 (expr_crc32_hash (stri ,CHAR (0XFFFFFFFF) ,0)) & VAR32_MAX) ;
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
	inline Mutable () :Mutable (ARGVP0) {}

	inline implicit Mutable (const TYPE &right) : Mutable (ARGVP0 ,std::move (right)) {
		mStatus = STATUS_CACHED ;
	}

	inline implicit Mutable (TYPE &&right) : Mutable (ARGVP0 ,std::move (right)) {
		mStatus = STATUS_CACHED ;
	}

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

	inline void signal () const {
		if (mStatus != STATUS_CACHED)
			return ;
		mStatus = STATUS_SIGNALED ;
	}

	inline void finish () const {
		mStatus = STATUS_FINISHED ;
	}

private:
	template <class... _ARGS>
	inline explicit Mutable (const DEF<decltype (ARGVP0)> & ,_ARGS &&...args) :mData (std::forward<_ARGS> (args)...) ,mStatus (STATUS_SIGNALED) {}
} ;

namespace U {
template <class ,class ,class...>
struct INDEXOF_TRAITS {
	using TYPE = ARGC<VAR_NONE> ;
} ;

template <LENGTH _VAL1 ,class _ARG1 ,class... _ARGS>
struct INDEXOF_TRAITS<ARGC<_VAL1> ,_ARG1 ,_ARG1 ,_ARGS...> {
	using TYPE = ARGC<_VAL1> ;
} ;

template <LENGTH _VAL1 ,class _ARG1 ,class _ARG2 ,class... _ARGS>
struct INDEXOF_TRAITS<ARGC<_VAL1> ,_ARG1 ,_ARG2 ,_ARGS...> {
	using TYPE = typename INDEXOF_TRAITS<ARGC<_VAL1 + 1> ,_ARG1 ,_ARGS...>::TYPE ;
} ;
} ;

template <class _ARG1 ,class... _ARGS>
using INDEXOF_TRAITS_TYPE = typename U::INDEXOF_TRAITS<ARGC<0> ,_ARG1 ,_ARGS...>::TYPE ;

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

template <INDEX _VAL1 ,class... _ARGS>
using VISITOF_TRATIS_TYPE = typename U::VISITOF_TRATIS<ARGC<_VAL1> ,_ARGS...>::TYPE ;

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

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Variant>::value && INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_ARG1> ,TYPES...>::value != VAR_NONE>>
	inline implicit Variant (_ARG1 &&right) {
		mIndex = template_init_construct (&mVariant ,std::forward<_ARG1> (right)) ;
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
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (this == &right)
				continue ;
			this->~Variant () ;
			new (this) Variant (std::move (right)) ;
		}
		return _XVALUE_<Variant> (*this) ;
	}

	inline Variant (Variant &&right) noexcept {
		mIndex = right.mIndex ;
		if (mIndex == VAR_NONE)
			return ;
		template_move_construct (&mVariant ,&right.mVariant ,mIndex ,_NULL_<const ARGVS<TYPES...>> ()) ;
	}

	inline Variant &operator= (Variant &&right) noexcept {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (this == &right)
				continue ;
			this->~Variant () ;
			new (this) Variant (std::move (right)) ;
		}
		return _XVALUE_<Variant> (*this) ;
	}

	inline BOOL exist () const {
		if (mIndex == VAR_NONE)
			return FALSE ;
		return TRUE ;
	}

	template <class _RET>
	inline BOOL available () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value != VAR_NONE) ;
		if (mIndex != INDEXOF_TRAITS_TYPE<REMOVE_CVR_TYPE<_RET> ,TYPES...>::value)
			return FALSE ;
		return TRUE ;
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

	template <class _ARG1>
	inline void apply (const Function<void (_ARG1 &)> &proc) {
		if (!available<_ARG1> ())
			return ;
		proc (_LOAD_<_ARG1> (mVariant.unused)) ;
	}

	template <class _ARG1>
	inline void apply (const Function<void (const _ARG1 &)> &proc) const {
		if (!available<_ARG1> ())
			return ;
		proc (_LOAD_<_ARG1> (mVariant.unused)) ;
	}

	template <class _ARG1>
	inline void apply (const Function<DEF<void (_ARG1 &)> NONE::*> &proc) {
		if (!available<_ARG1> ())
			return ;
		proc (_LOAD_<_ARG1> (mVariant.unused)) ;
	}

	template <class _ARG1>
	inline void apply (const Function<DEF<void (const _ARG1 &)> NONE::*> &proc) const {
		if (!available<_ARG1> ())
			return ;
		proc (_LOAD_<_ARG1> (mVariant.unused)) ;
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

	//@warn: none class shall be base on its address
	inline void address_swap (Variant &right) noexcept popping {
		_SWAP_ (mVariant ,right.mVariant) ;
		_SWAP_ (mIndex ,right.mIndex) ;
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

	template <class _ARG1>
	inline static INDEX template_init_construct (PTR<TEMP<VARIANT>> address ,_ARG1 &&arg1) popping {
		const auto r1x = &_LOAD_<TEMP<REMOVE_CVR_TYPE<_ARG1>>> (address->unused) ;
		return template_create (_NULL_<const ARGC<std::is_constructible<REMOVE_CVR_TYPE<_ARG1> ,_ARG1 &&>::value>> () ,r1x ,std::forward<_ARG1> (arg1)) ;
	}

	inline static void template_destruct (PTR<TEMP<VARIANT>> address ,INDEX index ,const ARGVS<> &) noexcept {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_destruct (PTR<TEMP<VARIANT>> address ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) noexcept {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_nothrow_destructible<_ARG1>::value) ;
		const auto r1x = BOOL (index == 0) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r1x)
				continue ;
			_DESTROY_ (&_LOAD_<TEMP<_ARG1>> (address->unused)) ;
		}
		if (r1x)
			return ;
		template_destruct (address ,(index - 1) ,_NULL_<const ARGVS<_ARGS...>> ()) ;
	}

	inline static void template_copy_construct (PTR<TEMP<VARIANT>> address ,PTR<const TEMP<VARIANT>> src ,INDEX index ,const ARGVS<> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_copy_construct (PTR<TEMP<VARIANT>> address ,PTR<const TEMP<VARIANT>> src ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		const auto r1x = BOOL (index == 0) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r1x)
				continue ;
			const auto r2x = &_LOAD_<TEMP<_ARG1>> (address->unused) ;
			const auto r3x = template_create (_NULL_<const ARGC<std::is_copy_constructible<_ARG1>::value && std::is_nothrow_move_constructible<_ARG1>::value>> () ,r2x ,_LOAD_<_ARG1> (src->unused)) ;
			_DYNAMIC_ASSERT_ (r3x != VAR_NONE) ;
		}
		if (r1x)
			return ;
		template_copy_construct (address ,src ,(index - 1) ,_NULL_<const ARGVS<_ARGS...>> ()) ;
	}

	inline static void template_move_construct (PTR<TEMP<VARIANT>> address ,PTR<TEMP<VARIANT>> src ,INDEX index ,const ARGVS<> &) noexcept {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_move_construct (PTR<TEMP<VARIANT>> address ,PTR<TEMP<VARIANT>> src ,INDEX index ,const ARGVS<_ARG1 ,_ARGS...> &) noexcept {
		_STATIC_ASSERT_ (INDEXOF_TRAITS_TYPE<_ARG1 ,_ARGS...>::value == VAR_NONE) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<_ARG1>::value && std::is_nothrow_move_assignable<_ARG1>::value) ;
		const auto r1x = BOOL (index == 0) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (!r1x)
				continue ;
			_CREATE_ (&_LOAD_<TEMP<_ARG1>> (address->unused) ,std::move (_LOAD_<_ARG1> (src->unused))) ;
		}
		if (r1x)
			return ;
		template_move_construct (address ,src ,(index - 1) ,_NULL_<const ARGVS<_ARGS...>> ()) ;
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

	inline TYPE1 &one () & {
		return mData ;
	}

	inline constexpr const TYPE1 &one () const & {
		return mData ;
	}

	inline TYPE1 &one () && = delete ;

	inline Tuple<TYPES...> &rest () & {
		return *this ;
	}

	inline constexpr const Tuple<TYPES...> &rest () const & {
		return *this ;
	}

	inline Tuple<TYPES...> &rest () && = delete ;

	template <INDEX _VAL1>
	inline VISITOF_TRATIS_TYPE<_VAL1 ,TYPE1 ,TYPES...> &visit () & {
		return template_visit (*this ,_NULL_<const ARGC<_VAL1>> ()) ;
	}

	template <INDEX _VAL1>
	inline constexpr const VISITOF_TRATIS_TYPE<_VAL1 ,TYPE1 ,TYPES...> &visit () const & {
		return template_visit (*this ,_NULL_<const ARGC<_VAL1>> ()) ;
	}

	template <INDEX _VAL1>
	inline VISITOF_TRATIS_TYPE<_VAL1 ,TYPE1 ,TYPES...> &visit () && = delete ;

	inline BOOL equal (const Tuple &right) const {
		if (one () != right.one ())
			return FALSE ;
		if (rest () != right.rest ())
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Tuple &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const Tuple &right) const {
		return !equal (right) ;
	}

	inline BOOL less (const Tuple &right) const {
		if (one () < right.one ())
			return TRUE ;
		if (one () > right.one ())
			return FALSE ;
		if (rest () >= right.rest ())
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator< (const Tuple &right) const {
		return less (right) ;
	}

	inline BOOL operator>= (const Tuple &right) const {
		return !less (right) ;
	}

	inline BOOL operator> (const Tuple &right) const {
		return right.less (*this) ;
	}

	inline BOOL operator<= (const Tuple &right) const {
		return !right.less (*this) ;
	}

private:
	inline static VISITOF_TRATIS_TYPE<0 ,TYPE1 ,TYPES...> &template_visit (Tuple &_self ,const ARGC<0> &) {
		return _self.one () ;
	}

	template <INDEX _VAL1>
	inline static VISITOF_TRATIS_TYPE<_VAL1 ,TYPE1 ,TYPES...> &template_visit (Tuple &_self ,const ARGC<_VAL1> &) {
		_STATIC_ASSERT_ (_VAL1 > 0 && _VAL1 < 1 + _CAPACITYOF_ (TYPES)) ;
		return Tuple<TYPES...>::template_visit (_self.rest () ,_NULL_<const ARGC<_VAL1 - 1>> ()) ;
	}

	inline static constexpr const VISITOF_TRATIS_TYPE<0 ,TYPE1 ,TYPES...> &template_visit (const Tuple &_self ,const ARGC<0> &) {
		return _self.one () ;
	}

	template <INDEX _VAL1>
	inline static constexpr const VISITOF_TRATIS_TYPE<_VAL1 ,TYPE1 ,TYPES...> &template_visit (const Tuple &_self ,const ARGC<_VAL1> &) {
		_STATIC_ASSERT_ (_VAL1 > 0 && _VAL1 < 1 + _CAPACITYOF_ (TYPES)) ;
		return Tuple<TYPES...>::template_visit (_self.rest () ,_NULL_<const ARGC<_VAL1 - 1>> ()) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class... _TYPES>
class Function<TYPE1 (TYPES...)>::ImplHolder<PTR<TYPE1 (TYPES... ,_TYPES...)>> :public Function<TYPE1 (TYPES...)>::Holder {
private:
	PTR<TYPE1 (TYPES... ,_TYPES...)> mFunction ;
	Tuple<REMOVE_CVR_TYPE<_TYPES>...> mParameters ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const PTR<TYPE1 (TYPES... ,_TYPES...)> &function ,const REMOVE_CVR_TYPE<_TYPES> &...args) :mFunction (function) ,mParameters (args...) {}

	inline TYPE1 invoke (FORWARD_TRAITS_TYPE<TYPES> &&...args) const popping override {
		return template_apply (mFunction ,std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args)... ,mParameters) ;
	}

private:
	inline static TYPE1 template_apply (const PTR<TYPE1 (TYPES... ,_TYPES...)> &function ,FORWARD_TRAITS_TYPE<TYPES> &&...args1 ,const Tuple<> &parameters ,const REMOVE_CVR_TYPE<_TYPES> &...args2) popping {
		_DEBUG_ASSERT_ (function != NULL) ;
		return function (std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args1)... ,args2...) ;
	}

	template <class... _ARGS1 ,class... _ARGS2>
	inline static TYPE1 template_apply (const PTR<TYPE1 (TYPES... ,_TYPES...)> &function ,FORWARD_TRAITS_TYPE<TYPES> &&...args1 ,const Tuple<_ARGS2...> &parameters ,const _ARGS1 &...args2) popping {
		return template_apply (function ,std::forward<FORWARD_TRAITS_TYPE<TYPES>> (args1)... ,parameters.rest () ,args2... ,parameters.one ()) ;
	}
} ;

template <class TYPE1 ,class... TYPES>
template <class... _ARGS>
inline Function<TYPE1 (TYPES...)> Function<TYPE1 (TYPES...)>::make (const PTR<TYPE1 (TYPES... ,_ARGS...)> &function ,const REMOVE_CVR_TYPE<_ARGS> &...args) {
	auto sgd = GlobalHeap::alloc<TEMP<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>>> () ;
	ScopedHolder<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>> ANONYMOUS (sgd ,function ,args...) ;
	const auto r1x = &_LOAD_<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>> (_XVALUE_<PTR<TEMP<ImplHolder<PTR<TYPE1 (TYPES... ,_ARGS...)>>>>> (sgd)) ;
	Function ret = Function (_XVALUE_<PTR<Holder>> (r1x)) ;
	sgd = NULL ;
	return std::move (ret) ;
}

template <class...>
class AllOfTuple ;

template <class...>
class AnyOfTuple ;

namespace stl {
template <class...>
struct is_all_same :public false_type {} ;

template <class _ARG1>
struct is_all_same<_ARG1> :public true_type {} ;

template <class _ARG1>
struct is_all_same<_ARG1 ,_ARG1> :public true_type {} ;

template <class _ARG1 ,class... _ARGS>
struct is_all_same<_ARG1 ,_ARG1 ,_ARGS...> :public is_all_same<_ARG1 ,_ARGS...> {} ;
} ;

template <class... TYPES>
class AllOfTuple :private Tuple<const TYPES &...> {
private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) > 0) ;
	_STATIC_ASSERT_ (stl::is_all_same<TYPES...>::value) ;

public:
	inline AllOfTuple () = delete ;

	template <class _ARG1>
	inline implicit operator BOOL () const {
		return template_cast (_XVALUE_<Tuple<const TYPES &...>> (*this)) ;
	}

	template <class _ARG1>
	inline BOOL operator== (const _ARG1 &right) const {
		return template_equal (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator!= (const _ARG1 &right) const {
		return template_not_equal (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator< (const _ARG1 &right) const {
		return template_less (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator>= (const _ARG1 &right) const {
		return template_not_less (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator> (const _ARG1 &right) const {
		return template_bigger (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator<= (const _ARG1 &right) const {
		return template_not_bigger (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

private:
	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return BOOL (arg1 == arg2) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 == arg1) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 == arg1) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return BOOL (arg1 < arg2) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 > arg1) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 > arg1) ;
	}

	template <class _ARG1>
	inline static BOOL template_cast (const Tuple<_ARG1> &left) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,BOOL>::value) ;
		return BOOL (left.one ()) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL template_cast (const Tuple<_ARG1 ,_ARGS...> &left) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,BOOL>::value) ;
		if (!BOOL (left.one ()))
			return FALSE ;
		if (!template_cast (left.rest ()))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_equal (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (!operator_equal (left.one () ,right))
			return FALSE ;
		if (!template_equal (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_not_equal (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (operator_equal (left.one () ,right))
			return FALSE ;
		if (!template_not_equal (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_less (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (!operator_less (left.one () ,right))
			return FALSE ;
		if (!template_less (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_not_less (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (operator_less (left.one () ,right))
			return FALSE ;
		if (!template_not_less (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_bigger (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (!operator_less (right ,left.one ()))
			return FALSE ;
		if (!template_bigger (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_not_bigger (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (operator_less (right ,left.one ()))
			return FALSE ;
		if (!template_not_bigger (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}
} ;

template <class... TYPES>
class AnyOfTuple :private Tuple<const TYPES &...> {
private:
	_STATIC_ASSERT_ (_CAPACITYOF_ (TYPES) > 0) ;
	_STATIC_ASSERT_ (stl::is_all_same<TYPES...>::value) ;

public:
	inline AnyOfTuple () = delete ;

	template <class _ARG1>
	inline implicit operator BOOL () const {
		return template_cast (_XVALUE_<Tuple<const TYPES &...>> (*this)) ;
	}

	template <class _ARG1>
	inline BOOL operator== (const _ARG1 &right) const {
		return template_equal (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator!= (const _ARG1 &right) const {
		return template_not_equal (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator< (const _ARG1 &right) const {
		return template_less (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator>= (const _ARG1 &right) const {
		return template_not_less (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator> (const _ARG1 &right) const {
		return template_bigger (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

	template <class _ARG1>
	inline BOOL operator<= (const _ARG1 &right) const {
		return template_not_bigger (_XVALUE_<Tuple<const TYPES &...>> (*this) ,right) ;
	}

private:
	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return BOOL (arg1 == arg2) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 == arg1) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_equal (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 == arg1) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const _ARG2 &arg2) {
		return BOOL (arg1 < arg2) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AllOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 > arg1) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL operator_less (const _ARG1 &arg1 ,const AnyOfTuple<_ARGS...> &arg2) {
		return BOOL (arg2 > arg1) ;
	}

	template <class _ARG1>
	inline static BOOL template_cast (const Tuple<_ARG1> &left) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,BOOL>::value) ;
		return BOOL (left.one ()) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static BOOL template_cast (const Tuple<_ARG1 ,_ARGS...> &left) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,BOOL>::value) ;
		if (BOOL (left.one ()))
			return TRUE ;
		if (!template_cast (left.rest ()))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_equal (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (operator_equal (left.one () ,right))
			return TRUE ;
		if (!template_equal (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_equal (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_equal (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_not_equal (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (!operator_equal (left.one () ,right))
			return TRUE ;
		if (!template_not_equal (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_less (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (operator_less (left.one () ,right))
			return TRUE ;
		if (!template_less (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_less (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (left.one () ,right) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_not_less (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (!operator_less (left.one () ,right))
			return TRUE ;
		if (!template_not_less (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_bigger (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (operator_less (right ,left.one ()))
			return TRUE ;
		if (!template_bigger (left.rest () ,right))
			return FALSE ;
		return TRUE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static BOOL template_not_bigger (const Tuple<_ARG1> &left ,const _ARG2 &right) {
		return !operator_less (right ,left.one ()) ;
	}

	template <class _ARG1 ,class _ARG2 ,class... _ARGS>
	inline static BOOL template_not_bigger (const Tuple<_ARG1 ,_ARG1 ,_ARGS...> &left ,const _ARG2 &right) {
		if (!operator_less (right ,left.one ()))
			return TRUE ;
		if (!template_not_bigger (left.rest () ,right))
			return FALSE ;
		return TRUE ;
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
template <class TYPE ,class = VOID>
struct is_complete_type :public false_type {} ;

template <class TYPE>
struct is_complete_type<TYPE ,ENABLE_TYPE<(_SIZEOF_ (TYPE) > 0)>> :public true_type {} ;

template <class TYPE ,class = VOID>
struct is_interface_type :public false_type {} ;

template <class TYPE>
struct is_interface_type<TYPE ,ENABLE_TYPE<_SIZEOF_ (TYPE) == _SIZEOF_ (Interface) && _ALIGNOF_ (TYPE) == _ALIGNOF_ (Interface)>> :public is_base_of<Interface ,TYPE> {} ;
} ;

namespace stl {
template <class _ARG1 ,class _ARG2 ,class = VOID>
struct is_always_base_of :public false_type {} ;

template <class _ARG1 ,class _ARG2>
struct is_always_base_of<_ARG1 ,_ARG2 ,ENABLE_TYPE<(_SIZEOF_ (_ARG1) > 0 && _SIZEOF_ (_ARG2) > 0)>> :public is_base_of<_ARG1 ,_ARG2> {} ;

template <class ,class ,class = VOID>
struct is_virtual_base_of :public false_type {} ;

template <class _ARG1>
struct is_virtual_base_of_helper1 :public virtual _ARG1 {} ;

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
	template <class _ARG1>
	inline static StrongRef<_ARG1> strong_from_this (PTR<_ARG1> _this) ;
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
	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_always_base_of<TYPE ,_ARG1>::value>>
	inline implicit StrongRef (const StrongRef<_ARG1> &right) :StrongRef (right.template recast<TYPE> ()) {}

	inline implicit StrongRef (const WeakRef<TYPE> &right) ;

	inline implicit StrongRef (const SoftRef<TYPE> &right) ;

	inline ~StrongRef () noexcept {
		if (mPointer == NULL)
			return ;
		if (!mHolder.exist ())
			return ;
		if (!mHolder->mData.exist ())
			return ;
		const auto r1x = BOOL (--mHolder->mCounter == 0) ;
		if (r1x)
			mHolder->mData = AnyRef<void> () ;
		mPointer = NULL ;
	}

	inline StrongRef (const StrongRef &right) :StrongRef (right.mHolder ,right.mPointer) {}

	inline StrongRef &operator= (const StrongRef &right) {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (this == &right)
				continue ;
			this->~StrongRef () ;
			new (this) StrongRef (std::move (right)) ;
		}
		return _XVALUE_<StrongRef> (*this) ;
	}

	inline StrongRef (StrongRef &&right) noexcept {
		mHolder = std::move (right.mHolder) ;
		mPointer = _EXCHANGE_ (right.mPointer) ;
	}

	inline StrongRef &operator= (StrongRef &&right) noexcept {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (this == &right)
				continue ;
			this->~StrongRef () ;
			new (this) StrongRef (std::move (right)) ;
		}
		return _XVALUE_<StrongRef> (*this) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		if (!mHolder.exist ())
			return FALSE ;
		if (!mHolder->mData.exist ())
			return FALSE ;
		return TRUE ;
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
		if (!mHolder.exist () && !right.mHolder.exist ())
			return TRUE ;
		if (!mHolder.exist () || !right.mHolder.exist ())
			return FALSE ;
		if (&mHolder.self != &right.mHolder.self)
			return FALSE ;
		return TRUE ;
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

	template <class _ARG1>
	inline static PTR<_ARG1> template_recast (PTR<TYPE> pointer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_always_base_of<_ARG1 ,TYPE>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		return _XVALUE_<PTR<_ARG1>> (pointer) ;
	}

	template <class _ARG1>
	inline static PTR<_ARG1> template_recast (PTR<TYPE> pointer ,const ARGV<_ARG1> & ,const ARGV<ENABLE_TYPE<stl::is_always_base_of<Interface ,TYPE>::value && stl::is_always_base_of<Interface ,_ARG1>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		//@warn: RTTI might be different across DLL
		return dynamic_cast<PTR<_ARG1>> (pointer) ;
	}

	template <class _ARG1>
	inline static PTR<_ARG1> template_recast (PTR<TYPE> pointer ,const ARGV<_ARG1> & ,const ARGV<VOID> & ,const DEF<decltype (ARGVP1)> &) {
		return NULL ;
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

template <class _ARG1>
inline StrongRef<_ARG1> WeakRef<void>::strong_from_this (PTR<_ARG1> _this) {
	_STATIC_ASSERT_ (stl::is_virtual_base_of<WeakRef<void> ,_ARG1>::value) ;
	_DEBUG_ASSERT_ (_this != NULL) ;
	const auto r1x = _XVALUE_<PTR<CAST_TRAITS_TYPE<WeakRef<void> ,_ARG1>>> (_this) ;
	return StrongRef<_ARG1> (r1x->mHolder ,_this) ;
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
		if (!mHolder.exist ())
			return FALSE ;
		if (!mHolder->mData.exist ())
			return FALSE ;
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL equal (const WeakRef &right) const {
		if (!mHolder.exist () && !right.mHolder.exist ())
			return TRUE ;
		if (!mHolder.exist () || !right.mHolder.exist ())
			return FALSE ;
		if (&mHolder.self != &right.mHolder.self)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const WeakRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const WeakRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const StrongRef<TYPE> &right) const {
		if (!mHolder.exist () && !right.mHolder.exist ())
			return TRUE ;
		if (!mHolder.exist () || !right.mHolder.exist ())
			return FALSE ;
		if (&mHolder.self != &right.mHolder.self)
			return FALSE ;
		return TRUE ;
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
class SoftRef :public WeakRef<TYPE> {
#pragma region
#pragma push_macro ("super")
#undef super
#define super m_super ()

private:
	class Pack {
	private:
		friend SoftRef ;
		StrongRef<TYPE> mData ;
		LENGTH mWeight ;
	} ;

	inline static constexpr VAR expr_log2 (VAR arg1) {
		return (arg1 <= 0) ? VAR_NONE : (arg1 == 1) ? 0 : (1 + expr_log2 (arg1 >> 1)) ;
	}

private:
	SharedRef<Allocator<Pack ,SFIXED>> mHeap ;
	INDEX mIndex ;

public:
	inline SoftRef () = default ;

	inline explicit SoftRef (LENGTH len) {
		mHeap = SharedRef<Allocator<Pack ,SFIXED>>::make (len) ;
	}

	inline ~SoftRef () noexcept {
		_CALL_EH_ ([&] () {
			detach () ;
		}) ;
	}

	inline SoftRef (const SoftRef &) = delete ;
	inline SoftRef &operator= (const SoftRef &) = delete ;

	inline SoftRef (SoftRef &&right) noexcept {
		super = std::move (right.super) ;
		mHeap = std::move (right.mHeap) ;
		mIndex = right.mIndex ;
	}

	inline SoftRef &operator= (SoftRef &&right) noexcept {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (this == &right)
				continue ;
			this->~SoftRef () ;
			new (this) SoftRef (std::move (right)) ;
		}
		return _XVALUE_<SoftRef> (*this) ;
	}

	LENGTH capacity () const {
		if (!mHeap.exist ())
			return 0 ;
		return mHeap->size () ;
	}

	inline BOOL exist () const {
		return super.exist () ;
	}

	inline SoftRef copy () popping {
		SoftRef ret ;
		ret.super = super ;
		ret.mHeap = mHeap ;
		ret.mIndex = mIndex ;
		return std::move (ret) ;
	}

	inline BOOL equal (const SoftRef &right) const {
		if (super != right.super)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const SoftRef &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const SoftRef &right) const {
		return !equal (right) ;
	}

	inline BOOL equal (const StrongRef<TYPE> &right) const {
		if (super != right)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const StrongRef<TYPE> &right) const {
		return equal (right) ;
	}

	inline BOOL operator!= (const StrongRef<TYPE> &right) const {
		return !equal (right) ;
	}

	inline StrongRef<TYPE> strong () const {
		return super.strong () ;
	}

	inline void assign (const StrongRef<TYPE> &right) {
		detach () ;
		super = right ;
		attach () ;
	}

	inline void operator= (const StrongRef<TYPE> &right) {
		assign (right) ;
	}

	inline void lock () const {
		if (!linked ())
			return ;
		if (mHeap.self[mIndex].mWeight < 0)
			return ;
		const auto r1x = BOOL (mHeap.self[mIndex].mWeight >= 0 && mHeap.self[mIndex].mWeight < VAR32_MAX) ;
		mHeap.self[mIndex].mWeight += EFLAG (r1x) ;
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
		if (super != mHeap.self[mIndex].mData)
			return FALSE ;
		return TRUE ;
	}

	inline void attach () {
		if (linked ())
			return ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (mHeap->length () < mHeap->size ())
				continue ;
			mIndex = min_weight_one () ;
			_DYNAMIC_ASSERT_ (mIndex != VAR_NONE) ;
			const auto r1x = expr_log2 (mHeap.self[mIndex].mWeight) ;
			if (r1x <= 0)
				continue ;
			for (INDEX i = 0 ; i < mHeap->size () ; i++)
				mHeap.self[i].mWeight = mHeap.self[i].mWeight >> r1x ;
		}
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (mIndex != VAR_NONE)
				continue ;
			mIndex = mHeap->alloc () ;
		}
		mHeap.self[mIndex].mData = super ;
		mHeap.self[mIndex].mWeight = 3 ;
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

	inline void detach () {
		if (!linked ())
			return ;
		_DEBUG_ASSERT_ (mHeap.self[mIndex].mWeight >= 0) ;
		mHeap->free (mIndex) ;
		mIndex = VAR_NONE ;
	}

private:
	inline WeakRef<TYPE> &m_super () & {
		return *this ;
	}

	inline const WeakRef<TYPE> &m_super () const & {
		return *this ;
	}

	inline WeakRef<TYPE> &m_super () && = delete ;

#undef super
#pragma pop_macro ("super")
#pragma endregion
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
			_CALL_EH_ ([&] () {
				release (mPointer) ;
			}) ;
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

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<TYPE & ,_RET>::value>>
		inline implicit operator _RET () const & {
			_DEBUG_ASSERT_ (mPointer != NULL) ;
			return _RET (*mPointer) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

	private:
		inline explicit WatcherProxy (PTR<TYPE> pointer) noexcept :mPointer (pointer) {}
	} ;

private:
	_STATIC_ASSERT_ (_SIZEOF_ (TYPE) > 0) ;
	friend ScopedGuard<IntrusiveRef> ;
	std::atomic<PTR<TYPE>> mPointer ;
	std::atomic<BOOL> mSemaphore ;

public:
	inline IntrusiveRef () :IntrusiveRef (ARGVP0) {}

	template <class _ARG1>
	inline explicit IntrusiveRef (const _ARG1 &address) : IntrusiveRef (ARGVP0) {
		_STATIC_ASSERT_ (std::is_same<_ARG1 ,PTR<TYPE>>::value) ;
		//@warn: address must be from 'IntrusiveRef::make'
		acquire (address ,FALSE) ;
		update_pointer (address) ;
	}

	inline ~IntrusiveRef () noexcept {
		ScopedGuard<IntrusiveRef> ANONYMOUS (*this) ;
		const auto r1x = mPointer.exchange (NULL) ;
		_CALL_EH_ ([&] () {
			release (r1x) ;
		}) ;
	}

	inline IntrusiveRef (const IntrusiveRef &) = delete ;
	inline IntrusiveRef &operator= (const IntrusiveRef &) = delete ;

	inline IntrusiveRef (IntrusiveRef &&right) noexcept :IntrusiveRef (ARGVP0) {
		ScopedGuard<IntrusiveRef> ANONYMOUS (right) ;
		const auto r1x = right.mPointer.exchange (NULL) ;
		update_pointer (r1x) ;
	}

	inline IntrusiveRef &operator= (IntrusiveRef &&right) noexcept {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (this == &right)
				continue ;
			this->~IntrusiveRef () ;
			new (this) IntrusiveRef (std::move (right)) ;
		}
		return _XVALUE_<IntrusiveRef> (*this) ;
	}

	inline BOOL exist () const {
		const auto r1x = mPointer.load () ;
		if (r1x == NULL)
			return FALSE ;
		return TRUE ;
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
	inline explicit IntrusiveRef (const DEF<decltype (ARGVP0)> &) noexcept :mPointer (NULL) ,mSemaphore (FALSE) {}

private:
	inline void lock () noexcept {
		auto rax = FALSE ;
		INDEX ir = 0 ;
		while (TRUE) {
			if (mSemaphore.compare_exchange_weak (rax ,TRUE))
				break ;
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
		const auto r1x = &_LOAD_<TYPE> (_XVALUE_<PTR<TEMP<TYPE>>> (sgd)) ;
		acquire (r1x ,TRUE) ;
		ret.update_pointer (r1x) ;
		sgd = NULL ;
		return std::move (ret) ;
	}
} ;

template <class TYPE>
class Monostate {
private:
	SharedRef<TYPE> mThis ;

public:
	inline Monostate () {
		mThis = SharedRef<TYPE>::make () ;
	}

	inline implicit Monostate (const TYPE &right) {
		mThis = SharedRef<TYPE>::make (std::move (right)) ;
	}

	inline implicit Monostate (TYPE &&right) {
		mThis = SharedRef<TYPE>::make (std::move (right)) ;
	}

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

	inline void swap (Monostate &right) popping {
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
		return (align <= 0) ? 0 : ((base / align + EFLAG (base % align != 0)) * align) ;
	}

	inline static constexpr VAL64 expr_pow (VAL64 base ,LENGTH power) {
		return (power <= 0) ? 1 : (power % 2 != 0) ? (base * expr_pow (base ,(power - 1))) : (_SQE_ (expr_pow (base ,(power / 2)))) ;
	}

	template <class SIZE>
	class ImplPool :public Pool {
	private:
		struct BLOCK {
			PTR<struct BLOCK> mNext ;
			HEADER mFlexData ;
		} ;

		struct CHUNK {
			PTR<ARR<BYTE>> mOrigin ;
			PTR<struct CHUNK> mPrev ;
			PTR<struct CHUNK> mNext ;
			LENGTH mCount ;
		} ;

	private:
		PTR<CHUNK> mRoot ;
		PTR<BLOCK> mFree ;
		LENGTH mSize ;
		LENGTH mLength ;

	public:
		inline ImplPool () {
			mRoot = NULL ;
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
			for (PTR<CHUNK> i = mRoot ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				GlobalHeap::free (i->mOrigin) ;
			}
			mRoot = NULL ;
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
			const auto r3x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (_XVALUE_<PTR<ARR<BYTE>>> (sgd)) ,_ALIGNOF_ (CHUNK)) ;
			const auto r4x = &_LOAD_<CHUNK> (r3x) ;
			r4x->mOrigin = _XVALUE_<PTR<ARR<BYTE>>> (sgd) ;
			r4x->mPrev = NULL ;
			r4x->mNext = mRoot ;
			r4x->mCount = r1x ;
			if (mRoot != NULL)
				mRoot->mPrev = r4x ;
			mRoot = r4x ;
			mSize += r1x * SIZE::value ;
			const auto r5x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (r3x) + _SIZEOF_ (CHUNK) ,_ALIGNOF_ (BLOCK)) ;
			for (INDEX i = 0 ,ie = mRoot->mCount ; i < ie ; i++) {
				const auto r6x = &_LOAD_<BLOCK> (r5x + i * (_SIZEOF_ (BLOCK) + SIZE::value)) ;
				r6x->mNext = mFree ;
				mFree = r6x ;
			}
			sgd = NULL ;
		}

		inline PTR<HEADER> alloc (LENGTH len) popping override {
			_DEBUG_ASSERT_ (len <= SIZE::value) ;
			reserve () ;
			const auto r1x = mFree ;
			mFree = r1x->mNext ;
			mLength += SIZE::value ;
			const auto r2x = _CAST_<TEMP<PTR<BLOCK>>> (VAR_USED) ;
			r1x->mNext = _LOAD_<PTR<BLOCK>> (&r2x) ;
			return &r1x->mFlexData ;
		}

		inline void free (PTR<HEADER> address) noexcept override {
			_DEBUG_ASSERT_ (address != NULL) ;
			const auto r1x = &_OFFSET_ (&BLOCK::mFlexData ,*address) ;
			_DEBUG_ASSERT_ (_ADDRESS_ (r1x->mNext) == VAR_USED) ;
			r1x->mNext = mFree ;
			mFree = r1x ;
			mLength -= SIZE::value ;
		}

		inline void clean () override {
			if (mSize == mLength)
				return ;
			for (PTR<CHUNK> i = mRoot ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				if (!empty_node (i))
					continue ;
				auto &r1 = (i->mPrev != NULL) ? (i->mPrev->mNext) : mRoot ;
				r1 = i->mNext ;
				if (i->mNext != NULL)
					i->mNext->mPrev = i->mPrev ;
				mSize -= i->mCount * SIZE::value ;
				GlobalHeap::free (i->mOrigin) ;
			}
		}

	private:
		inline BOOL empty_node (PTR<const CHUNK> node) const {
			const auto r1x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (node) + _SIZEOF_ (CHUNK) ,_ALIGNOF_ (BLOCK)) ;
			for (INDEX i = 0 ,ie = node->mCount ; i < ie ; i++)
				if (_ADDRESS_ (_LOAD_<BLOCK> (r1x + i * (_SIZEOF_ (BLOCK) + SIZE::value)).mNext) == VAR_USED)
					return FALSE ;
			return TRUE ;
		}
	} ;

	class HugePool :public Pool {
	private:
		struct BLOCK {
			PTR<ARR<BYTE>> mOrigin ;
			PTR<struct BLOCK> mPrev ;
			PTR<struct BLOCK> mNext ;
			LENGTH mCount ;
			HEADER mFlexData ;
		} ;

	private:
		PTR<BLOCK> mRoot ;
		LENGTH mSize ;

	public:
		inline HugePool () {
			mRoot = NULL ;
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
			for (PTR<BLOCK> i = mRoot ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				GlobalHeap::free (i->mOrigin) ;
			}
			mRoot = NULL ;
			mSize = 0 ;
		}

		inline PTR<HEADER> alloc (LENGTH len) popping override {
			const auto r1x = _ALIGNOF_ (BLOCK) + _SIZEOF_ (BLOCK) + len ;
			auto sgd = GlobalHeap::alloc<BYTE> (r1x) ;
			const auto r2x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (_XVALUE_<PTR<ARR<BYTE>>> (sgd)) ,_ALIGNOF_ (BLOCK)) ;
			const auto r3x = &_LOAD_<BLOCK> (r2x) ;
			r3x->mOrigin = _XVALUE_<PTR<ARR<BYTE>>> (sgd) ;
			r3x->mPrev = NULL ;
			r3x->mNext = mRoot ;
			r3x->mCount = len ;
			if (mRoot != NULL)
				mRoot->mPrev = r3x ;
			mRoot = r3x ;
			mSize += len ;
			sgd = NULL ;
			return &r3x->mFlexData ;
		}

		inline void free (PTR<HEADER> address) noexcept override {
			_DEBUG_ASSERT_ (address != NULL) ;
			const auto r1x = &_OFFSET_ (&BLOCK::mFlexData ,*address) ;
			auto &r1 = (r1x->mPrev != NULL) ? (r1x->mPrev->mNext) : mRoot ;
			r1 = r1x->mNext ;
			if (r1x->mNext != NULL)
				r1x->mNext->mPrev = r1x->mPrev ;
			mSize -= r1x->mCount ;
			GlobalHeap::free (r1x->mOrigin) ;
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
	Monostate<Buffer<PTR<Pool> ,ARGC<17>>> mPool ;

public:
	inline MemoryPool () {
		mPool.self = Buffer<PTR<Pool> ,ARGC<17>> ({
			&mPool1 ,&mPool2 ,&mPool3 ,&mPool4 ,&mPool5 ,&mPool6 ,&mPool7 ,&mPool8 ,
			&mPool9 ,&mPool10 ,&mPool11 ,&mPool12 ,&mPool13 ,&mPool14 ,&mPool15 ,&mPool16 ,
			&mPool17}) ;
	}

	inline LENGTH size () const {
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mPool.self.size () ; i++)
			ret += mPool.self[i]->size () ;
		return std::move (ret) ;
	}

	inline LENGTH length () const {
		LENGTH ret = 0 ;
		for (INDEX i = 0 ; i < mPool.self.size () ; i++)
			ret += mPool.self[i]->length () ;
		return std::move (ret) ;
	}

	//@warn: held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<_RET> alloc () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		const auto r1x = _SIZEOF_ (_RET) + _MAX_ ((_ALIGNOF_ (_RET) - 8) ,VAR_ZERO) ;
		INDEX ix = _MIN_ (((r1x - 1) / 8) ,_SIZEOF_ (HEADER)) ;
		const auto r2x = mPool.self[ix]->alloc (r1x) ;
		const auto r3x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (r2x) + _SIZEOF_ (HEADER) ,_ALIGNOF_ (_RET)) - _SIZEOF_ (HEADER) ;
		const auto r4x = &_LOAD_<HEADER> (r3x) ;
		r4x->P1 = mPool.self[ix] ;
		r4x->P2 = r2x ;
		return &_LOAD_<_RET> (r3x + _SIZEOF_ (HEADER)) ;
	}

	//@warn: held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<ARR<_RET>> alloc (LENGTH len) popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		const auto r1x = len * _SIZEOF_ (_RET) + _MAX_ ((_ALIGNOF_ (_RET) - 8) ,VAR_ZERO) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		INDEX ix = _MIN_ (((r1x - 1) / 8) ,_SIZEOF_ (HEADER)) ;
		const auto r2x = mPool.self[ix]->alloc (r1x) ;
		const auto r3x = &_NULL_<BYTE> () + expr_ceil (_ADDRESS_ (r2x) + _SIZEOF_ (HEADER) ,_ALIGNOF_ (_RET)) - _SIZEOF_ (HEADER) ;
		const auto r4x = &_LOAD_<HEADER> (r3x) ;
		r4x->P1 = mPool.self[ix] ;
		r4x->P2 = r2x ;
		return &_LOAD_<ARR<_RET>> (r3x + _SIZEOF_ (HEADER)) ;
	}

	template <class _ARG1>
	inline void free (const PTR<_ARG1> &address) noexcept {
		_STATIC_ASSERT_ (std::is_pod<REMOVE_ARRAY_TYPE<_ARG1>>::value) ;
		const auto r1x = &_NULL_<BYTE> () + _ADDRESS_ (address) - _SIZEOF_ (HEADER) ;
		const auto r2x = &_LOAD_<HEADER> (r1x) ;
		INDEX ix = _MEMCHR_ (mPool.self.self ,mPool.self.size () ,r2x->P1) ;
		_DEBUG_ASSERT_ (ix != VAR_NONE) ;
		mPool.self[ix]->free (r2x->P2) ;
	}

	inline void clean () {
		for (INDEX i = 0 ; i < mPool.self.size () ; i++)
			mPool.self[i]->clean () ;
	}
} ;

template <class>
class GlobalStatic ;

template <>
class GlobalStatic<void> final :private Wrapped<void> {
private:
	struct NODE_A {
		FLAG mGUID ;
		PTR<struct NODE_A> mNext ;
		BOOL mReadOnly ;
		VAR mData ;
	} ;

	using GUID_TYPE = TEMP<BYTE[DEFAULT_RECURSIVE_SIZE::value]> ;

	struct NODE_B {
		GUID_TYPE mGUID ;
		PTR<struct NODE_B> mNext ;
		BOOL mReadOnly ;
		PTR<NONE> mData ;
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
		LENGTH mLength_a ;
		PTR<NODE_A> mRoot_a ;
		LENGTH mLength_b ;
		PTR<NODE_B> mRoot_b ;
	} ;

private:
	template <class>
	friend class GlobalStatic ;
	friend IntrusiveRef<Pack> ;

private:
	inline static DEF<PTR<NONE> (PTR<NONE> ,PTR<NONE>) popping> unique_atomic_address ;

	inline static Pack &unique () popping {
		return _CACHE_ ([] () {
			auto rax = unique_atomic_address (NULL ,NULL) ;
			auto rbx = IntrusiveRef<Pack> () ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				if (rax != NULL)
					continue ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack>::make () ;
				const auto r2x = rbx.watch () ;
				const auto r3x = &_XVALUE_<Pack> (r2x) ;
				const auto r4x = &_LOAD_<NONE> (r3x) ;
				rax = unique_atomic_address (NULL ,r4x) ;
			}
			_DYNAMIC_ASSERT_ (rax != NULL) ;
			const auto r5x = &_LOAD_<Pack> (rax) ;
			return IntrusiveRef<Pack> (r5x).watch () ;
		}) ;
	}

	inline static PTR<NODE_A> static_new_node (Pack &_self ,FLAG guid) popping {
		//@warn: sure 'GlobalHeap' can be used across DLL
		auto sgd = GlobalHeap::alloc<TEMP<NODE_A>> () ;
		ScopedHolder<NODE_A> ANONYMOUS (sgd) ;
		PTR<NODE_A> ret = &_LOAD_<NODE_A> (_XVALUE_<PTR<TEMP<NODE_A>>> (sgd)) ;
		_DEBUG_ASSERT_ (ret != NULL) ;
		ret->mGUID = guid ;
		ret->mNext = _self.mRoot_a ;
		_self.mRoot_a = ret ;
		_self.mLength_a++ ;
		sgd = NULL ;
		return std::move (ret) ;
	}

	inline static PTR<NODE_B> static_new_node (Pack &_self ,const GUID_TYPE &guid) popping {
		//@warn: sure 'GlobalHeap' can be used across DLL
		auto sgd = GlobalHeap::alloc<TEMP<NODE_B>> () ;
		ScopedHolder<NODE_B> ANONYMOUS (sgd) ;
		PTR<NODE_B> ret = &_LOAD_<NODE_B> (_XVALUE_<PTR<TEMP<NODE_B>>> (sgd)) ;
		_DEBUG_ASSERT_ (ret != NULL) ;
		ret->mGUID = guid ;
		ret->mNext = _self.mRoot_b ;
		_self.mRoot_b = ret ;
		_self.mLength_b++ ;
		sgd = NULL ;
		return std::move (ret) ;
	}

	inline static PTR<NODE_A> static_find_node (const Pack &_self ,FLAG guid) {
		for (PTR<NODE_A> i = _self.mRoot_a ; i != NULL ; i = i->mNext)
			if (i->mGUID == guid)
				return i ;
		return NULL ;
	}

	inline static PTR<NODE_B> static_find_node (const Pack &_self ,const GUID_TYPE &guid) {
		for (PTR<NODE_B> i = _self.mRoot_b ; i != NULL ; i = i->mNext)
			if (_MEMEQUAL_ (i->mGUID.unused ,PTRTOARR[&guid.unused[0]]))
				return i ;
		return NULL ;
	}

	inline static void intrusive_create (Pack &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
		_self.mCounter.self = 0 ;
		_self.mLength_a = 0 ;
		_self.mRoot_a = NULL ;
		_self.mLength_b = 0 ;
		_self.mRoot_b = NULL ;
	}

	inline static void intrusive_destroy (Pack &_self) {
		ScopedGuard<std::mutex> ANONYMOUS (_self.mNodeMutex) ;
		_CALL_EH_ ([&] () {
			for (PTR<NODE_A> i = _self.mRoot_a ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				i->~NODE_A () ;
				GlobalHeap::free (i) ;
			}
		}) ;
		_self.mLength_a = 0 ;
		_self.mRoot_a = NULL ;
		_CALL_EH_ ([&] () {
			for (PTR<NODE_B> i = _self.mRoot_b ,ir ; i != NULL ; i = ir) {
				ir = i->mNext ;
				i->~NODE_B () ;
				GlobalHeap::free (i) ;
			}
		}) ;
		_self.mLength_b = 0 ;
		_self.mRoot_b = NULL ;
	}

	inline static LENGTH intrusive_attach (Pack &_self) popping {
		return ++_self.mCounter.self ;
	}

	inline static LENGTH intrusive_detach (Pack &_self) popping {
		return --_self.mCounter.self ;
	}
} ;

template <FLAG GUID>
class GlobalStatic<ARGC<GUID>> final :private Wrapped<void> {
public:
	inline static void init (VAR data ,BOOL read_only) {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		if (r1x != NULL)
			return ;
		const auto r2x = GlobalStatic<void>::static_new_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r2x != NULL) ;
		r2x->mReadOnly = read_only ;
		r2x->mData = data ;
	}

	inline static VAR load () popping {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		return r1x->mData ;
	}

	inline static VAR compare_and_swap (VAR expect ,VAR data) popping {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		const auto r1x = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		_DYNAMIC_ASSERT_ (!r1x->mReadOnly) ;
		if (r1x->mData == expect)
			r1x->mData = data ;
		return r1x->mData ;
	}

	inline static void save (VAR data) {
		auto &r1 = GlobalStatic<void>::unique () ;
		ScopedGuard<std::mutex> ANONYMOUS (r1.mNodeMutex) ;
		auto rax = GlobalStatic<void>::static_find_node (r1 ,GUID) ;
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (rax != NULL)
				continue ;
			rax = GlobalStatic<void>::static_new_node (r1 ,GUID) ;
			rax->mReadOnly = FALSE ;
		}
		_DYNAMIC_ASSERT_ (rax != NULL) ;
		_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
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
		TYPE mData ;
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
		_DEBUG_ASSERT_ (r2x > 0 && r2x < _SIZEOF_ (GUID_TYPE)) ;
		const auto r3x = _MIN_ (r2x ,_SIZEOF_ (GUID_TYPE)) ;
		_MEMCOPY_ (PTRTOARR[&_ZERO_ (ret).unused[0]] ,PTRTOARR[&r1x[0]] ,r3x) ;
		return std::move (ret) ;
	}

public:
	inline static TYPE &unique () popping {
		auto &r1 = _CACHE_ ([] () {
			auto &r2 = GlobalStatic<void>::unique () ;
			ScopedGuard<std::mutex> ANONYMOUS (r2.mNodeMutex) ;
			const auto r2x = guid_from_typeid_name () ;
			auto rax = GlobalStatic<void>::static_find_node (r2 ,r2x) ;
			auto rbx = IntrusiveRef<Pack> () ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				if (rax != NULL)
					continue ;
				rax = GlobalStatic<void>::static_new_node (r2 ,r2x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack>::make () ;
				const auto r3x = rbx.watch () ;
				const auto r4x = &_XVALUE_<Pack> (r3x) ;
				rax->mReadOnly = FALSE ;
				rax->mData = &_LOAD_<NONE> (r4x) ;
			}
			_DYNAMIC_ASSERT_ (!rax->mReadOnly) ;
			const auto r5x = &_LOAD_<Pack> (rax->mData) ;
			return IntrusiveRef<Pack> (r5x).watch () ;
		}) ;
		return _XVALUE_<Pack> (r1).mData ;
	}

	inline static const TYPE &const_unique () popping {
		auto &r1 = _CACHE_ ([] () {
			auto &r2 = GlobalStatic<void>::unique () ;
			ScopedGuard<std::mutex> ANONYMOUS (r2.mNodeMutex) ;
			const auto r2x = guid_from_typeid_name () ;
			auto rax = GlobalStatic<void>::static_find_node (r2 ,r2x) ;
			auto rbx = IntrusiveRef<Pack> () ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				if (rax != NULL)
					continue ;
				rax = GlobalStatic<void>::static_new_node (r2 ,r2x) ;
				_DYNAMIC_ASSERT_ (rax != NULL) ;
				//@warn: sure 'GlobalHeap' can be used across DLL
				rbx = IntrusiveRef<Pack>::make () ;
				const auto r3x = rbx.watch () ;
				const auto r4x = &_XVALUE_<Pack> (r3x) ;
				rax->mReadOnly = TRUE ;
				rax->mData = &_LOAD_<NONE> (r4x) ;
			}
			const auto r5x = &_LOAD_<Pack> (rax->mData) ;
			return IntrusiveRef<Pack> (r5x).watch () ;
		}) ;
		return _XVALUE_<Pack> (r1).mData ;
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

		template <class _ARG1>
		inline explicit Metadata (const ARGV<_ARG1> &) {
			_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
			mObjectSize = _SIZEOF_ (_ARG1) ;
			mObjectAlign = _ALIGNOF_ (_ARG1) ;
			mTypeID = _TYPEID_<_ARG1> () ;
			mConstrutor = _XVALUE_<PTR<void (PTR<NONE>)>> ([] (PTR<NONE> address) {
				_CREATE_ (&_LOAD_<TEMP<_ARG1>> (address)) ;
			}) ;
			mDestructor = _XVALUE_<PTR<void (PTR<NONE>)>> ([] (PTR<NONE> address) {
				_DESTROY_ (&_LOAD_<TEMP<_ARG1>> (address)) ;
			}) ;
		}
	} ;

public:
	inline Object () = delete ;

	template <class _ARG1>
	inline explicit Object (PTR<_ARG1> deriver) {
		_STATIC_ASSERT_ (stl::is_always_base_of<Object ,_ARG1>::value) ;
		_STATIC_ASSERT_ (!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Object>::value) ;
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

	template <class... _TYPES>
	class ImplBinder :public Binder {
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
		inline explicit Member (const Serializer &base ,TYPE2 &context) popping : mBase (base) ,mContext (context) {}
	} ;

private:
	StrongRef<const Binder> mBinder ;

public:
	inline Serializer () = delete ;

	template <class... _ARGS>
	inline explicit Serializer (const DEF<_ARGS TYPE2::*> &...args) :mBinder (StrongRef<const ImplBinder<_ARGS...>>::make (args...)) {}

	inline Member operator() (TYPE2 &context) const popping {
		_DEBUG_ASSERT_ (mBinder.exist ()) ;
		return Member (*this ,context) ;
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