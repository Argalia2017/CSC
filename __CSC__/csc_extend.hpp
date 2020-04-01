#pragma once

#ifndef __CSC_EXTEND__
#define __CSC_EXTEND__
#endif

#include "csc.hpp"

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef switch_case
#undef discard
#endif

#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039)
#endif
#include <cstdlib>
#include <locale>
#include <exception>
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
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace stl {
using std::exception ;

using std::mutex ;
using std::recursive_mutex ;
using std::atomic ;
using std::unique_lock ;
using std::condition_variable ;
using std::thread ;

namespace chrono {
using std::chrono::duration ;
using std::chrono::time_point ;
using std::chrono::milliseconds ;
using std::chrono::system_clock ;
using std::chrono::steady_clock ;
} ;

namespace this_thread {
using std::this_thread::sleep_for ;
using std::this_thread::yield ;
} ;

using std::atomic_thread_fence ;

using ::setlocale ;

#ifndef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
using std::quick_exit ;
#endif

using std::exit ;
using std::terminate ;
} ;

class GlobalRuntime final
	:private Wrapped<void> {
public:
	inline static std::chrono::system_clock::time_point clock_now () {
		return std::chrono::system_clock::now () ;
	}

	inline static std::chrono::steady_clock::time_point clock_tick () {
		return std::chrono::steady_clock::now () ;
	}

	inline static FLAG thread_tid () ;

	template <class _ARG1 ,class _ARG2>
	inline static void thread_sleep (const std::chrono::duration<_ARG1 ,_ARG2> &time_) {
		std::this_thread::sleep_for (time_) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static void thread_sleep (const std::chrono::time_point<_ARG1 ,_ARG2> &time_) {
		std::this_thread::sleep_for (time_) ;
	}

	inline static void thread_sleep () {
		std::this_thread::yield () ;
	}

	inline static LENGTH thread_concurrency () {
		return LENGTH (std::thread::hardware_concurrency ()) ;
	}

	inline static void thread_fence () {
		std::atomic_thread_fence (std::memory_order::memory_order_seq_cst) ;
	}

	inline static void locale_init (const Plain<STRA> &locale_) {
		stl::setlocale (LC_ALL ,locale_.self) ;
	}

	inline static FLAG process_pid () ;

	inline static Buffer<BYTE ,ARGC<128>> process_info (FLAG pid) ;

	inline static FLAG process_info_pid (const PhanBuffer<const STRU8> &info) ;

	inline static FLAG module_mid () {
		return _TYPEMID_<Interface> () ;
	}

	inline static void process_exit[[noreturn]] () {
#ifdef __CSC_COMPILER_GNUC__
		//@error: fuck g++4.8
		std::exit (EXIT_FAILURE) ;
#else
		std::quick_exit (EXIT_FAILURE) ;
#endif
	}

	inline static void process_abort[[noreturn]] () {
		std::terminate () ;
	}
} ;

template <class _ARG1 ,class _ARG2>
inline void _CATCH_ (_ARG1 &&try_proc ,_ARG2 &&catch_proc) noexcept {
	_STATIC_ASSERT_ (!std::is_reference<_ARG1>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG1 ,ARGVS<>> ,void>::value) ;
	_STATIC_ASSERT_ (!std::is_reference<_ARG2>::value) ;
	_STATIC_ASSERT_ (std::is_same<RESULT_OF_TYPE<_ARG2 ,ARGVS<const Exception &>> ,void>::value) ;
	try {
		try_proc () ;
		return ;
	} catch (const Exception &e) {
		catch_proc (e) ;
	} catch (const std::exception &) {
		catch_proc (Exception (_PCSTR_ ("std::exception : unknown"))) ;
	} catch (...) {
		catch_proc (Exception (_PCSTR_ ("unknown C++ exception"))) ;
	}
}

#ifdef __CSC_UNITTEST__
class GlobalWatch final
	:private Wrapped<void> {
private:
	struct Detail ;

public:
	template <class _ARG1 ,class _ARG2>
	inline static void done (const ARGV<_ARG1> & ,const Plain<STR> &name ,_ARG2 &data) noexcept {
		struct Dependent ;
		using Storage = typename DEPENDENT_TYPE<Detail ,Dependent>::template Storage<_ARG2> ;
		static volatile Storage mInstance ;
		mInstance.mName = name.self ;
		mInstance.mAddress = &data ;
		mInstance.mTypeMID = _TYPEMID_<_ARG2> () ;
		mInstance.mWatch (data) ;
	}
} ;

struct GlobalWatch::Detail {
	template <class UNIT>
	class Storage final
		:private Interface {
	private:
		friend GlobalWatch ;
		PTR<const STR> mName ;
		PTR<UNIT> mAddress ;
		FLAG mTypeMID ;
		PTR<void (UNIT &)> mWatch ;

	public:
		inline Storage () {
			mName = NULL ;
			mAddress = NULL ;
			mTypeMID = 0 ;
			const auto r1x = _XVALUE_<PTR<void (UNIT &)>> ([] (UNIT &) {}) ;
			mWatch = r1x ;
		} ;
	} ;
} ;
#endif

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
	MEGA mValue ;

public:
	inline VAR128 () = default ;

	inline implicit VAR128 (VAR64 that) {
		const auto r1x = _EBOOL_ (that < 0) * DATA (-1) ;
		v2i1 = DATA (that) ;
		v2i0 = r1x ;
	}

	inline explicit operator VAR32 () const {
		return VAR32 (v2i1) ;
	}

	inline explicit operator VAR64 () const {
		return VAR64 (v2i1) ;
	}

	inline BOOL operator== (const VAR128 &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const VAR128 &that) const {
		return !equal (that) ;
	}

	inline BOOL operator< (const VAR128 &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const VAR128 &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const VAR128 &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const VAR128 &that) const {
		return BOOL (compr (that) <= 0) ;
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

	inline VAR128 operator+ (const VAR128 &that) const {
		VAR128 ret = 0 ;
		ret.v2i0 = v2i0 + that.v2i0 + _EBOOL_ (v2i1 > ~that.v2i1) ;
		ret.v2i1 = v2i1 + that.v2i1 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator+= (const VAR128 &that) {
		v2i0 += that.v2i0 + _EBOOL_ (v2i1 > ~that.v2i1) ;
		v2i1 += that.v2i1 ;
		return (*this) ;
	}

	inline VAR128 operator- (const VAR128 &that) const {
		VAR128 ret = 0 ;
		ret.v2i0 = v2i0 - that.v2i0 - _EBOOL_ (v2i1 < that.v2i1) ;
		ret.v2i1 = v2i1 - that.v2i1 ;
		return std::move (ret) ;
	}

	inline VAR128 &operator-= (const VAR128 &that) {
		v2i0 -= that.v2i0 + _EBOOL_ (v2i1 < that.v2i1) ;
		v2i1 -= that.v2i1 ;
		return (*this) ;
	}

	inline VAR128 operator* (const VAR128 &that) const {
		VAR128 ret = 0 ;
		auto rax = DATA (0) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (that.v4i3) ;
		ret.v4i3 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (that.v4i2) ;
		rax += DATA (v4i2) * DATA (that.v4i3) ;
		ret.v4i2 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (that.v4i1) ;
		rax += DATA (v4i2) * DATA (that.v4i2) ;
		rax += DATA (v4i1) * DATA (that.v4i3) ;
		ret.v4i1 = CHAR (rax) ;
		rax = CHAR (rax >> (_SIZEOF_ (CHAR) * 8)) ;
		rax += DATA (v4i3) * DATA (that.v4i0) ;
		rax += DATA (v4i2) * DATA (that.v4i1) ;
		rax += DATA (v4i1) * DATA (that.v4i2) ;
		rax += DATA (v4i0) * DATA (that.v4i3) ;
		ret.v4i0 = CHAR (rax) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator*= (const VAR128 &that) {
		(*this) = (*this) * that ;
		return (*this) ;
	}

	inline VAR128 operator/ (const VAR128 &that) const {
		VAR128 ret = 0 ;
		const auto r1x = _CAST_<VAR64> (v2i0) ;
		const auto r2x = _CAST_<VAR64> (that.v2i0) ;
		auto fax = TRUE ;
		if switch_case (fax) {
			if (!(r1x >= 0))
				discard ;
			if (!(that.v4i0 == 0))
				discard ;
			if (!(that.v4i1 == 0))
				discard ;
			if (!(that.v4i2 == 0))
				discard ;
			auto rax = DATA (0) ;
			const auto r3x = DATA (that.v4i3) ;
			_DEBUG_ASSERT_ (r3x != 0) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i0) ;
			ret.v4i0 = CHAR (rax / r3x) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i1) ;
			ret.v4i1 = CHAR (rax / r3x) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i2) ;
			ret.v4i2 = CHAR (rax / r3x) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i3) ;
			ret.v4i3 = CHAR (rax / r3x) ;
		}
		if switch_case (fax) {
			if (!(v2i0 == DATA (VAR64_MIN)))
				discard ;
			if (!(v2i1 == 0))
				discard ;
			if (!(r2x >= 0))
				discard ;
			ret = -(-((*this) + that) / that + 1) ;
		}
		if switch_case (fax) {
			if (!(v2i0 == DATA (VAR64_MIN)))
				discard ;
			if (!(v2i1 == 0))
				discard ;
			if (!(r2x < 0))
				discard ;
			ret = -(-((*this) - that) / that - 1) ;
		}
		if switch_case (fax) {
			if (!(r1x < 0))
				discard ;
			ret = -(-(*this) / that) ;
		}
		if switch_case (fax) {
			if (!(r1x >= 0))
				discard ;
			if (!(that.v2i0 == DATA (VAR64_MIN)))
				discard ;
			if (!(that.v2i1 == 0))
				discard ;
			ret = VAR128 (0) ;
		}
		if switch_case (fax) {
			if (!(r1x >= 0))
				discard ;
			if (!(r2x < 0))
				discard ;
			ret = (*this) / (-that) ;
		}
		if switch_case (fax) {
			ret = slow_divide ((*this) ,that) ;
		}
		return std::move (ret) ;
	}

	inline VAR128 &operator/= (const VAR128 &that) {
		(*this) = (*this) / that ;
		return (*this) ;
	}

	inline VAR128 operator% (const VAR128 &that) const {
		VAR128 ret = 0 ;
		const auto r1x = _CAST_<VAR64> (v2i0) ;
		const auto r2x = _CAST_<VAR64> (that.v2i0) ;
		auto fax = TRUE ;
		if switch_case (fax) {
			if (!(r1x >= 0))
				discard ;
			if (!(that.v4i0 == 0))
				discard ;
			if (!(that.v4i1 == 0))
				discard ;
			if (!(that.v4i2 == 0))
				discard ;
			auto rax = DATA (0) ;
			const auto r3x = DATA (that.v4i3) ;
			_DEBUG_ASSERT_ (r3x != 0) ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i0) ;
			ret.v4i0 = 0 ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i1) ;
			ret.v4i1 = 0 ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i2) ;
			ret.v4i2 = 0 ;
			rax = (DATA (rax % r3x) << (_SIZEOF_ (CHAR) * 8)) | DATA (v4i3) ;
			ret.v4i3 = CHAR (rax % r3x) ;
		}
		if switch_case (fax) {
			if (!(v2i0 == DATA (VAR64_MIN)))
				discard ;
			if (!(v2i1 == 0))
				discard ;
			if (!(r2x >= 0))
				discard ;
			ret = -(-((*this) + that) % that) ;
		}
		if switch_case (fax) {
			if (!(v2i0 == DATA (VAR64_MIN)))
				discard ;
			if (!(v2i1 == 0))
				discard ;
			if (!(r2x < 0))
				discard ;
			ret = -(-((*this) - that) % that) ;
		}
		if switch_case (fax) {
			if (!(r1x < 0))
				discard ;
			ret = -(-(*this) % that) ;
		}
		if switch_case (fax) {
			if (!(r1x >= 0))
				discard ;
			if (!(that.v2i0 == DATA (VAR64_MIN)))
				discard ;
			if (!(that.v2i1 == 0))
				discard ;
			ret = (*this) ;
		}
		if switch_case (fax) {
			if (!(r1x >= 0))
				discard ;
			if (!(r2x < 0))
				discard ;
			ret = (*this) % (-that) ;
		}
		if switch_case (fax) {
			ret = that - slow_divide ((*this) ,that) * that ;
		}
		return std::move (ret) ;
	}

	inline VAR128 &operator%= (const VAR128 &that) {
		(*this) = (*this) % that ;
		return (*this) ;
	}

	inline VAR128 operator+ () const {
		return (*this) ;
	}

	inline VAR128 operator- () const {
		VAR128 ret = 0 ;
		ret.v2i1 = ~v2i1 + 1 ;
		ret.v2i0 = ~v2i0 + _EBOOL_ (ret.v2i1 == DATA (0)) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator++ () {
		v2i1++ ;
		v2i0 += _EBOOL_ (v2i1 == DATA (0)) ;
		return (*this) ;
	}

	inline VAR128 operator++ (VAR32) popping {
		VAR128 ret = (*this) ;
		++(*this) ;
		return std::move (ret) ;
	}

	inline VAR128 &operator-- () {
		v2i1-- ;
		v2i0 -= _EBOOL_ (v2i1 == DATA (-1)) ;
		return (*this) ;
	}

	inline VAR128 operator-- (VAR32) popping {
		VAR128 ret = (*this) ;
		--(*this) ;
		return std::move (ret) ;
	}

private:
	inline BOOL equal (const VAR128 &that) const {
		if (v2i1 != that.v2i1)
			return FALSE ;
		if (v2i0 != that.v2i0)
			return FALSE ;
		return TRUE ;
	}

	inline FLAG compr (const VAR128 &that) const {
		const auto r1x = _CAST_<VAR64> (v2i0) ;
		const auto r2x = _CAST_<VAR64> (that.v2i0) ;
		const auto r3x = _MEMCOMPR_ (PTRTOARR[&r1x] ,PTRTOARR[&r2x] ,1) ;
		if (r3x != 0)
			return r3x ;
		return _MEMCOMPR_ (PTRTOARR[&v2i1] ,PTRTOARR[&that.v2i1] ,1) ;
	}

	inline DATA &m_v2i0 () & {
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mValue)[_CAST_<BYTE[2]> (r1x)[0]] ;
	}

	inline const DATA &m_v2i0 () const & {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mValue)[_CAST_<BYTE[2]> (r1x)[0]] ;
	}

	inline auto m_v2i0 () && ->void = delete ;

	inline DATA &m_v2i1 () & {
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mValue)[_CAST_<BYTE[2]> (r1x)[1]] ;
	}

	inline const DATA &m_v2i1 () const & {
		const auto r1x = WORD (0X0001) ;
		return _CAST_<DATA[2]> (mValue)[_CAST_<BYTE[2]> (r1x)[1]] ;
	}

	inline auto m_v2i1 () && ->void = delete ;

	inline CHAR &m_v4i0 () & {
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[0]] ;
	}

	inline const CHAR &m_v4i0 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[0]] ;
	}

	inline auto m_v4i0 () && ->void = delete ;

	inline CHAR &m_v4i1 () & {
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[1]] ;
	}

	inline const CHAR &m_v4i1 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[1]] ;
	}

	inline auto m_v4i1 () && ->void = delete ;

	inline CHAR &m_v4i2 () & {
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[2]] ;
	}

	inline const CHAR &m_v4i2 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[2]] ;
	}

	inline auto m_v4i2 () && ->void = delete ;

	inline CHAR &m_v4i3 () & {
		_STATIC_WARNING_ ("mark") ;
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[3]] ;
	}

	inline const CHAR &m_v4i3 () const & {
		const auto r1x = CHAR (0X00010203) ;
		return _CAST_<CHAR[4]> (mValue)[_CAST_<BYTE[4]> (r1x)[3]] ;
	}

	inline auto m_v4i3 () && ->void = delete ;

private:
	inline static VAR128 slow_divide (const VAR128 &y ,const VAR128 &x) {
		_DEBUG_ASSERT_ (y >= 0) ;
		_DEBUG_ASSERT_ (x > 0) ;
		VAR128 ret = 0 ;
		auto rax = Buffer<VAR128 ,ARGC<2>> () ;
		rax[0] = 0 ;
		rax[1] = y ;
		while (TRUE) {
			if (rax[0] > rax[1])
				break ;
			ret = rax[0] + (rax[1] - rax[0]) / 2 ;
			const auto r1x = x * ret ;
			if (r1x == y)
				break ;
			auto fax = TRUE ;
			if switch_case (fax) {
				if (!(r1x < y))
					discard ;
				rax[0] = ret + 1 ;
			}
			if switch_case (fax) {
				rax[1] = ret - 1 ;
			}
		}
		ret -= _EBOOL_ (x * ret > y) ;
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

template <class UNIT>
class Mutable {
private:
	static constexpr auto STATE_CACHED = EFLAG (1) ;
	static constexpr auto STATE_SIGNALED = EFLAG (2) ;
	static constexpr auto STATE_FINISHED = EFLAG (3) ;

private:
	mutable UNIT mValue ;
	mutable EFLAG mState ;

public:
	inline Mutable () {
		mState = STATE_SIGNALED ;
	}

	inline implicit Mutable (const UNIT &that)
		:Mutable (ARGVP0 ,std::move (that)) {}

	inline implicit Mutable (UNIT &&that)
		: Mutable (ARGVP0 ,std::move (that)) {}

	inline const UNIT &to () const {
		return mValue ;
	}

	inline implicit operator const UNIT & () const {
		return to () ;
	}

	template <class _ARG1>
	inline void apply (const Function<void (_ARG1 &)> &proc) const {
		_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG1> ,UNIT>::value) ;
		if (mState != STATE_SIGNALED)
			return ;
		proc (mValue) ;
		mState = STATE_CACHED ;
	}

	template <class _ARG1>
	inline void apply (const Function<U::MEMBER_FUNCTION_HINT<void ,_ARG1 &>> &proc) const {
		_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG1> ,UNIT>::value) ;
		if (mState != STATE_SIGNALED)
			return ;
		proc (mValue) ;
		mState = STATE_CACHED ;
	}

	inline void signal () const {
		if (mState != STATE_CACHED)
			return ;
		mState = STATE_SIGNALED ;
	}

	inline void finish () const {
		mState = STATE_FINISHED ;
	}

private:
	template <class... _ARGS>
	inline explicit Mutable (const DEF<decltype (ARGVP0)> & ,_ARGS &&...initval)
		:mValue (std::forward<_ARGS> (initval)...) ,mState (STATE_CACHED) {}
} ;

namespace U {
inline constexpr LENGTH constexpr_max_sizeof (const ARGV<ARGVS<>> &) {
	return 1 ;
}

template <class _ARG1>
inline constexpr LENGTH constexpr_max_sizeof (const ARGV<_ARG1> &) {
	using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
	using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
	return _MAX_ (_SIZEOF_ (ONE_HINT) ,constexpr_max_sizeof (_NULL_<ARGV<REST_HINT>> ())) ;
}

inline constexpr LENGTH constexpr_max_alignof (const ARGV<ARGVS<>> &) {
	return 1 ;
}

template <class _ARG1>
inline constexpr LENGTH constexpr_max_alignof (const ARGV<_ARG1> &) {
	using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
	using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
	return _MAX_ (_ALIGNOF_ (ONE_HINT) ,constexpr_max_alignof (_NULL_<ARGV<REST_HINT>> ())) ;
}
} ;

template <class... UNITS>
class Variant {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) > 0) ;
	_STATIC_ASSERT_ (!stl::is_any_same<REMOVE_CVR_TYPE<UNITS>...>::value) ;

private:
	static constexpr auto VARIANT_ALIGN = U::constexpr_max_alignof (_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;
	static constexpr auto VARIANT_SIZE = U::constexpr_max_sizeof (_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;

	//@error: fuck g++4.8
	template <LENGTH ALIGN = VARIANT_ALIGN>
	struct ALIGNED_UNION {
		alignas (ALIGN) DEF<BYTE[VARIANT_SIZE]> unused ;
	} ;

	using VARIANT = ALIGNED_UNION<> ;

	using OPTIONAL = INDEX_TO_TYPE<ZERO ,ARGVS<UNITS...>> ;

private:
	TEMP<VARIANT> mVariant ;
	INDEX mIndex ;

public:
	inline Variant ()
		:Variant (ARGVP0) {
		const auto r1x = default_constructible_index (_NULL_<ARGV<ZERO>> () ,_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;
		template_construct (r1x ,_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;
		mIndex = r1x ;
	}

	template <class _ARG1 ,class = ENABLE_TYPE<!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Variant>::value && INDEX_OF_TYPE<REMOVE_CVR_TYPE<_ARG1> ,ARGVS<UNITS...>>::value != VAR_NONE>>
	inline implicit Variant (_ARG1 &&that)
		:Variant (ARGVP0) {
		_STATIC_ASSERT_ (!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,DEF<decltype (ARGVP0)>>::value) ;
		auto &r1x = _NULL_<ARGV<ARGC<std::is_constructible<REMOVE_CVR_TYPE<_ARG1> ,_ARG1 &&>::value>>> () ;
		auto &r2x = _LOAD_<TEMP<REMOVE_CVR_TYPE<_ARG1>>> (&mVariant) ;
		template_create (r1x ,&r2x ,std::forward<_ARG1> (that)) ;
		mIndex = INDEX_OF_TYPE<REMOVE_CVR_TYPE<_ARG1> ,ARGVS<UNITS...>>::value ;
	}

	inline ~Variant () noexcept {
		if (mIndex == VAR_NONE)
			return ;
		template_destruct (mIndex ,_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;
		mIndex = VAR_NONE ;
	}

	inline Variant (const Variant &that)
		:Variant (ARGVP0) {
		if (that.mIndex == VAR_NONE)
			return ;
		template_copy_construct (that ,that.mIndex ,_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;
		mIndex = that.mIndex ;
	}

	inline Variant &operator= (const Variant &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Variant () ;
			new (this) Variant (std::move (that)) ;
		}
		return (*this) ;
	}

	inline Variant (Variant &&that) noexcept
		:Variant (ARGVP0) {
		if (that.mIndex == VAR_NONE)
			return ;
		template_move_construct (that ,that.mIndex ,_NULL_<ARGV<ARGVS<UNITS...>>> ()) ;
		mIndex = that.mIndex ;
	}

	inline Variant &operator= (Variant &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~Variant () ;
			new (this) Variant (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		if (mIndex == VAR_NONE)
			return FALSE ;
		return TRUE ;
	}

	template <class _RET>
	inline BOOL available () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (INDEX_OF_TYPE<REMOVE_CVR_TYPE<_RET> ,ARGVS<UNITS...>>::value != VAR_NONE) ;
		if (mIndex != INDEX_OF_TYPE<REMOVE_CVR_TYPE<_RET> ,ARGVS<UNITS...>>::value)
			return FALSE ;
		return TRUE ;
	}

	inline OPTIONAL &to () {
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) == 1) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		auto &r1x = _LOAD_<TEMP<OPTIONAL>> (&mVariant) ;
		return _CAST_<OPTIONAL> (r1x) ;
	}

	inline implicit operator OPTIONAL & () {
		return to () ;
	}

	inline const OPTIONAL &to () const {
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) == 1) ;
		_DYNAMIC_ASSERT_ (exist ()) ;
		auto &r1x = _LOAD_<TEMP<OPTIONAL>> (&mVariant) ;
		return _CAST_<OPTIONAL> (r1x) ;
	}

	inline implicit operator const OPTIONAL & () const {
		return to () ;
	}

	template <class _ARG1>
	inline void apply (const Function<void (_ARG1 &)> &proc) {
		if (!available<_ARG1> ())
			return ;
		auto &r1x = _LOAD_<TEMP<_ARG1>> (&mVariant) ;
		proc (_CAST_<_ARG1> (r1x)) ;
	}

	template <class _ARG1>
	inline void apply (const Function<U::MEMBER_FUNCTION_HINT<void ,_ARG1 &>> &proc) {
		if (!available<_ARG1> ())
			return ;
		auto &r1x = _LOAD_<TEMP<_ARG1>> (&mVariant) ;
		proc (_CAST_<_ARG1> (r1x)) ;
	}

	//@warn: none class shall be base on its address
	inline void address_swap (Variant &that) noexcept {
		_SWAP_ (mVariant ,that.mVariant) ;
		_SWAP_ (mIndex ,that.mIndex) ;
	}

public:
	inline static Variant nullopt () noexcept {
		return Variant (ARGVP0) ;
	}

private:
	inline explicit Variant (const DEF<decltype (ARGVP0)> &) noexcept
		:mIndex (VAR_NONE) {}

private:
	inline void template_construct (INDEX index ,const ARGV<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline void template_construct (INDEX index ,const ARGV<_ARG1> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<ONE_HINT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<ONE_HINT>::value) ;
		const auto r1x = BOOL (index == 0) ;
		if switch_case (TRUE) {
			if (!r1x)
				discard ;
			auto &r2x = _NULL_<ARGV<ARGC<std::is_default_constructible<ONE_HINT>::value>>> () ;
			auto &r3x = _LOAD_<TEMP<ONE_HINT>> (&mVariant) ;
			template_create (r2x ,&r3x) ;
		}
		if (r1x)
			return ;
		template_construct ((index - 1) ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

	inline void template_destruct (INDEX index ,const ARGV<ARGVS<>> &) noexcept {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline void template_destruct (INDEX index ,const ARGV<_ARG1> &) noexcept {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		_STATIC_ASSERT_ (std::is_nothrow_destructible<ONE_HINT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<ONE_HINT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<ONE_HINT>::value) ;
		const auto r1x = BOOL (index == 0) ;
		if switch_case (TRUE) {
			if (!r1x)
				discard ;
			auto &r2x = _LOAD_<TEMP<ONE_HINT>> (&mVariant) ;
			_DESTROY_ (&r2x) ;
		}
		if (r1x)
			return ;
		template_destruct ((index - 1) ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

	inline void template_copy_construct (const Variant &that ,INDEX index ,const ARGV<ARGVS<>> &) {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline void template_copy_construct (const Variant &that ,INDEX index ,const ARGV<_ARG1> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		const auto r1x = BOOL (index == 0) ;
		if switch_case (TRUE) {
			if (!r1x)
				discard ;
			auto &r2x = _NULL_<ARGV<ARGC<std::is_copy_constructible<ONE_HINT>::value && std::is_nothrow_move_constructible<ONE_HINT>::value>>> () ;
			auto &r3x = _LOAD_<TEMP<ONE_HINT>> (&mVariant) ;
			auto &r4x = _LOAD_<TEMP<ONE_HINT>> (&that.mVariant) ;
			template_create (r2x ,&r3x ,std::move (_CAST_<ONE_HINT> (r4x))) ;
		}
		if (r1x)
			return ;
		template_copy_construct (that ,(index - 1) ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

	inline void template_move_construct (Variant &that ,INDEX index ,const ARGV<ARGVS<>> &) noexcept {
		_STATIC_WARNING_ ("noop") ;
	}

	template <class _ARG1>
	inline void template_move_construct (Variant &that ,INDEX index ,const ARGV<_ARG1> &) noexcept {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG1> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG1> ;
		_STATIC_ASSERT_ (std::is_nothrow_move_constructible<ONE_HINT>::value) ;
		_STATIC_ASSERT_ (std::is_nothrow_move_assignable<ONE_HINT>::value) ;
		const auto r1x = BOOL (index == 0) ;
		if switch_case (TRUE) {
			if (!r1x)
				discard ;
			auto &r2x = _LOAD_<TEMP<ONE_HINT>> (&mVariant) ;
			auto &r3x = _LOAD_<TEMP<ONE_HINT>> (&that.mVariant) ;
			template_create (_NULL_<ARGV<ARGC<TRUE>>> () ,&r2x ,std::move (_CAST_<ONE_HINT> (r3x))) ;
		}
		if (r1x)
			return ;
		template_move_construct (that ,(index - 1) ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

private:
	template <class _ARG1>
	inline static INDEX default_constructible_index (const ARGV<_ARG1> & ,const ARGV<ARGVS<>> &) {
		return VAR_NONE ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static INDEX default_constructible_index (const ARGV<_ARG1> & ,const ARGV<_ARG2> &) {
		using ONE_HINT = ARGVS_ONE_TYPE<_ARG2> ;
		using REST_HINT = ARGVS_REST_TYPE<_ARG2> ;
		if (std::is_default_constructible<ONE_HINT>::value)
			return _ARG1::value ;
		return default_constructible_index (_NULL_<ARGV<INCREASE<_ARG1>>> () ,_NULL_<ARGV<REST_HINT>> ()) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_create (const ARGV<ARGC<TRUE>> & ,PTR<TEMP<_ARG1>> address ,_ARGS &&...initval) {
		_CREATE_ (address ,std::forward<_ARGS> (initval)...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline static void template_create (const ARGV<ARGC<FALSE>> & ,PTR<TEMP<_ARG1>> address ,_ARGS &&...initval) {
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;

template <class UNIT>
using Optional = Variant<UNIT> ;

template <class UNIT>
class Monostate {
private:
	SharedRef<UNIT> mValue ;

public:
	inline Monostate () {
		mValue = SharedRef<UNIT>::make () ;
	}

	inline Monostate (const Monostate &) = delete ;
	inline Monostate &operator= (const Monostate &) = delete ;
	inline Monostate (Monostate &&) = delete ;
	inline Monostate &operator= (Monostate &&) = delete ;

	inline UNIT &to () const {
		return mValue.self ;
	}

	inline implicit operator UNIT & () const {
		return to () ;
	}

	inline void swap (Monostate &that) {
		_SWAP_ (mValue ,that.mValue) ;
	}
} ;

template <class...>
class Tuple ;

template <>
class Tuple<> {
public:
	inline Tuple () = default ;

	inline LENGTH capacity () const {
		return 0 ;
	}

	inline BOOL equal (const Tuple &that) const {
		return TRUE ;
	}

	inline BOOL operator== (const Tuple &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Tuple &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Tuple &that) const {
		return 0 ;
	}

	inline BOOL operator< (const Tuple &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Tuple &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Tuple &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Tuple &that) const {
		return BOOL (compr (that) <= 0) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
class Tuple<UNIT1 ,UNITS...>
	:private Tuple<UNITS...> {
	_STATIC_ASSERT_ (!std::is_rvalue_reference<UNIT1>::value) ;

private:
	template <class...>
	friend class Tuple ;
	UNIT1 mValue ;

public:
	inline Tuple () = default ;

	inline implicit Tuple (FORWARD_TRAITS_TYPE<UNIT1> &&one_ ,FORWARD_TRAITS_TYPE<UNITS> &&...rest_) :Tuple<UNITS...> (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (rest_)...) ,mValue (std::forward<FORWARD_TRAITS_TYPE<UNIT1>> (one_)) {}

	inline LENGTH capacity () const {
		return 1 + rest ().capacity () ;
	}

	inline UNIT1 &one () & {
		return mValue ;
	}

	inline const UNIT1 &one () const & {
		return mValue ;
	}

	inline auto one () && ->void = delete ;

	inline Tuple<UNITS...> &rest () & {
		return (*this) ;
	}

	inline const Tuple<UNITS...> &rest () const & {
		return (*this) ;
	}

	inline auto rest () && ->void = delete ;

	template <class _ARG1>
	inline INDEX_TO_TYPE<DECREASE<_ARG1> ,ARGVS<UNIT1 ,UNITS...>> &pick (const ARGV<ARGVP<_ARG1>> &) & {
		return template_pick ((*this) ,_NULL_<ARGV<DECREASE<_ARG1>>> ()) ;
	}

	template <class _ARG1>
	inline const INDEX_TO_TYPE<DECREASE<_ARG1> ,ARGVS<UNIT1 ,UNITS...>> &pick (const ARGV<ARGVP<_ARG1>> &) const & {
		return template_pick ((*this) ,_NULL_<ARGV<DECREASE<_ARG1>>> ()) ;
	}

	template <class _ARG1>
	inline auto pick (const ARGV<ARGVP<_ARG1>> &) && ->void = delete ;

	inline BOOL equal (const Tuple &that) const {
		if (one () != that.one ())
			return FALSE ;
		if (rest () != that.rest ())
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const Tuple &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const Tuple &that) const {
		return !equal (that) ;
	}

	inline FLAG compr (const Tuple &that) const {
		const auto r1x = _MEMCOMPR_ (PTRTOARR[&one ()] ,PTRTOARR[&that.one ()] ,1) ;
		if (r1x != 0)
			return r1x ;
		return rest ().compr (that.rest ()) ;
	}

	inline BOOL operator< (const Tuple &that) const {
		return BOOL (compr (that) < 0) ;
	}

	inline BOOL operator>= (const Tuple &that) const {
		return BOOL (compr (that) >= 0) ;
	}

	inline BOOL operator> (const Tuple &that) const {
		return BOOL (compr (that) > 0) ;
	}

	inline BOOL operator<= (const Tuple &that) const {
		return BOOL (compr (that) <= 0) ;
	}

private:
	inline static UNIT1 &template_pick (Tuple &self_ ,const ARGV<ZERO> &) {
		return self_.one () ;
	}

	template <class _ARG1>
	inline static INDEX_TO_TYPE<_ARG1 ,ARGVS<UNIT1 ,UNITS...>> &template_pick (Tuple &self_ ,const ARGV<_ARG1> &) {
		_STATIC_ASSERT_ (_ARG1::value > 0 && _ARG1::value <= _CAPACITYOF_ (ARGVS<UNITS...>)) ;
		return Tuple<UNITS...>::template_pick (self_.rest () ,_NULL_<ARGV<DECREASE<_ARG1>>> ()) ;
	}

	inline static constexpr const UNIT1 &template_pick (const Tuple &self_ ,const ARGV<ZERO> &) {
		return self_.one () ;
	}

	template <class _ARG1>
	inline static constexpr const INDEX_TO_TYPE<_ARG1 ,ARGVS<UNIT1 ,UNITS...>> &template_pick (const Tuple &self_ ,const ARGV<_ARG1> &) {
		_STATIC_ASSERT_ (_ARG1::value > 0 && _ARG1::value <= _CAPACITYOF_ (ARGVS<UNITS...>)) ;
		return Tuple<UNITS...>::template_pick (self_.rest () ,_NULL_<ARGV<DECREASE<_ARG1>>> ()) ;
	}
} ;

template <class... UNITS>
using TupleBinder = Tuple<UNITS &...> ;

template <class UNIT1 ,class... UNITS>
template <class... UNITS_>
class Function<UNIT1 (UNITS...)>::Detail::ImplHolder<PTR<UNIT1 (UNITS... ,UNITS_...)>>
	:public Function<UNIT1 (UNITS...)>::Holder {
private:
	Function<UNIT1 (UNITS... ,UNITS_...)> mFunction ;
	Tuple<REMOVE_CVR_TYPE<UNITS_>...> mParameter ;

public:
	inline ImplHolder () = delete ;

	inline explicit ImplHolder (const PTR<UNIT1 (UNITS... ,UNITS_...)> &func ,const REMOVE_CVR_TYPE<UNITS_> &...parameter)
		:mFunction (func) ,mParameter (parameter...) {}

	inline UNIT1 invoke (FORWARD_TRAITS_TYPE<UNITS> &&...funcval) const override {
		return template_invoke (mParameter ,std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval)...) ;
	}

private:
	inline UNIT1 template_invoke (const Tuple<> &parameter ,FORWARD_TRAITS_TYPE<UNITS> &&...funcval1 ,const REMOVE_CVR_TYPE<UNITS_> &...funcval2) const {
		return mFunction (std::forward<FORWARD_TRAITS_TYPE<UNITS>> (funcval1)... ,funcval2...) ;
	}

	template <class _ARG1 ,class... _ARGS>
	inline UNIT1 template_invoke (const _ARG1 &parameter ,_ARGS &&...funcval) const {
		return template_invoke (parameter.rest () ,std::forward<_ARGS> (funcval)... ,parameter.one ()) ;
	}
} ;

template <class UNIT1 ,class... UNITS>
template <class... _ARGS>
inline exports Function<UNIT1 (UNITS...)> Function<UNIT1 (UNITS...)>::make (const PTR<UNIT1 (UNITS... ,_ARGS...)> &func ,const REMOVE_CVR_TYPE<_ARGS> &...parameter) {
	using ImplHolder = typename Detail::template ImplHolder<PTR<UNIT1 (UNITS... ,_ARGS...)>> ;
	auto rax = GlobalHeap::alloc<TEMP<ImplHolder>> () ;
	ScopedBuild<ImplHolder> ANONYMOUS (rax ,func ,parameter...) ;
	auto &r1x = _LOAD_<ImplHolder> (_XVALUE_<PTR<TEMP<ImplHolder>>> (rax)) ;
	Function ret = Function (ARGVP0 ,_XVALUE_<PTR<Holder>> (&r1x)) ;
	rax = NULL ;
	return std::move (ret) ;
}

template <class...>
class AllOfTuple ;

template <class...>
class AnyOfTuple ;

template <class... UNITS>
class AllOfTuple final
	:private Proxy {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) > 0) ;
	_STATIC_ASSERT_ (stl::is_all_same<UNITS...>::value) ;

private:
	using WRAPPED = INDEX_TO_TYPE<ZERO ,ARGVS<UNITS...>> ;

private:
	TupleBinder<const UNITS...> mTuple ;

public:
	inline AllOfTuple () = delete ;

	inline implicit operator BOOL () && {
		return template_boolean (mTuple) ;
	}

	inline BOOL operator== (const WRAPPED &that) && {
		return template_equal (mTuple ,that) ;
	}

	inline friend BOOL operator== (const WRAPPED &that ,AllOfTuple &&self_) {
		return std::move (self_) == that ;
	}

	inline BOOL operator!= (const WRAPPED &that) && {
		return template_not_equal (mTuple ,that) ;
	}

	inline friend BOOL operator!= (const WRAPPED &that ,AllOfTuple &&self_) {
		return std::move (self_) != that ;
	}

	inline BOOL operator< (const WRAPPED &that) && {
		return template_less (mTuple ,that) ;
	}

	inline friend BOOL operator< (const WRAPPED &that ,AllOfTuple &&self_) {
		return std::move (self_) > that ;
	}

	inline BOOL operator>= (const WRAPPED &that) && {
		return template_not_less (mTuple ,that) ;
	}

	inline friend BOOL operator>= (const WRAPPED &that ,AllOfTuple &&self_) {
		return std::move (self_) <= that ;
	}

	inline BOOL operator> (const WRAPPED &that) && {
		return template_less (that ,mTuple) ;
	}

	inline friend BOOL operator> (const WRAPPED &that ,AllOfTuple &&self_) {
		return std::move (self_) < that ;
	}

	inline BOOL operator<= (const WRAPPED &that) && {
		return template_not_less (that ,mTuple) ;
	}

	inline friend BOOL operator<= (const WRAPPED &that ,AllOfTuple &&self_) {
		return std::move (self_) >= that ;
	}

private:
	inline static BOOL operator_equal (const WRAPPED &lhs ,const WRAPPED &rhs) {
		return BOOL (lhs == rhs) ;
	}

	inline static BOOL operator_less (const WRAPPED &lhs ,const WRAPPED &rhs) {
		return BOOL (lhs < rhs) ;
	}

	inline static BOOL template_boolean (const Tuple<> &self_) {
		return TRUE ;
	}

	template <class _ARG1>
	inline static BOOL template_boolean (const _ARG1 &self_) {
		if (!BOOL (self_.one ()))
			return FALSE ;
		return template_boolean (self_.rest ()) ;
	}

	inline static BOOL template_equal (const Tuple<> &self_ ,const WRAPPED &that) {
		return TRUE ;
	}

	template <class _ARG1>
	inline static BOOL template_equal (const _ARG1 &self_ ,const WRAPPED &that) {
		if (!operator_equal (self_.one () ,that))
			return FALSE ;
		return template_equal (self_.rest () ,that) ;
	}

	inline static BOOL template_not_equal (const Tuple<> &self_ ,const WRAPPED &that) {
		return TRUE ;
	}

	template <class _ARG1>
	inline static BOOL template_not_equal (const _ARG1 &self_ ,const WRAPPED &that) {
		if (operator_equal (self_.one () ,that))
			return FALSE ;
		return template_not_equal (self_.rest () ,that) ;
	}

	inline static BOOL template_less (const Tuple<> &self_ ,const WRAPPED &that) {
		return TRUE ;
	}

	template <class _ARG1>
	inline static BOOL template_less (const _ARG1 &self_ ,const WRAPPED &that) {
		if (!operator_less (self_.one () ,that))
			return FALSE ;
		return template_less (self_.rest () ,that) ;
	}

	inline static BOOL template_not_less (const Tuple<> &self_ ,const WRAPPED &that) {
		return TRUE ;
	}

	template <class _ARG1>
	inline static BOOL template_not_less (const _ARG1 &self_ ,const WRAPPED &that) {
		if (operator_less (self_.one () ,that))
			return FALSE ;
		return template_not_less (self_.rest () ,that) ;
	}
} ;

template <class... UNITS>
class AnyOfTuple final
	:private Proxy {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<UNITS...>) > 0) ;
	_STATIC_ASSERT_ (stl::is_all_same<UNITS...>::value) ;

private:
	using WRAPPED = INDEX_TO_TYPE<ZERO ,ARGVS<UNITS...>> ;

private:
	TupleBinder<const UNITS...> mTuple ;

public:
	inline AnyOfTuple () = delete ;

	inline implicit operator BOOL () && {
		return template_boolean (mTuple) ;
	}

	inline BOOL operator== (const WRAPPED &that) && {
		return template_equal (mTuple ,that) ;
	}

	inline friend BOOL operator== (const WRAPPED &that ,AnyOfTuple &&self_) {
		return std::move (self_) == that ;
	}

	inline BOOL operator!= (const WRAPPED &that) && {
		return template_not_equal (mTuple ,that) ;
	}

	inline friend BOOL operator!= (const WRAPPED &that ,AnyOfTuple &&self_) {
		return std::move (self_) != that ;
	}

	inline BOOL operator< (const WRAPPED &that) && {
		return template_less (mTuple ,that) ;
	}

	inline friend BOOL operator< (const WRAPPED &that ,AnyOfTuple &&self_) {
		return std::move (self_) > that ;
	}

	inline BOOL operator>= (const WRAPPED &that) && {
		return template_not_less (mTuple ,that) ;
	}

	inline friend BOOL operator>= (const WRAPPED &that ,AnyOfTuple &&self_) {
		return std::move (self_) <= that ;
	}

	inline BOOL operator> (const WRAPPED &that) && {
		return template_less (that ,mTuple) ;
	}

	inline friend BOOL operator> (const WRAPPED &that ,AnyOfTuple &&self_) {
		return std::move (self_) < that ;
	}

	inline BOOL operator<= (const WRAPPED &that) && {
		return template_not_less (that ,mTuple) ;
	}

	inline friend BOOL operator<= (const WRAPPED &that ,AnyOfTuple &&self_) {
		return std::move (self_) >= that ;
	}

private:
	inline static BOOL operator_equal (const WRAPPED &lhs ,const WRAPPED &rhs) {
		return BOOL (lhs == rhs) ;
	}

	inline static BOOL operator_less (const WRAPPED &lhs ,const WRAPPED &rhs) {
		return BOOL (lhs < rhs) ;
	}

	inline static BOOL template_boolean (const Tuple<> &self_) {
		return FALSE ;
	}

	template <class _ARG1>
	inline static BOOL template_boolean (const _ARG1 &self_) {
		if (BOOL (self_.one ()))
			return TRUE ;
		return template_boolean (self_.rest ()) ;
	}

	inline static BOOL template_equal (const Tuple<> &self_ ,const WRAPPED &that) {
		return FALSE ;
	}

	template <class _ARG1>
	inline static BOOL template_equal (const _ARG1 &self_ ,const WRAPPED &that) {
		if (operator_equal (self_.one () ,that))
			return TRUE ;
		return template_equal (self_.rest () ,that) ;
	}

	inline static BOOL template_not_equal (const Tuple<> &self_ ,const WRAPPED &that) {
		return FALSE ;
	}

	template <class _ARG1>
	inline static BOOL template_not_equal (const _ARG1 &self_ ,const WRAPPED &that) {
		if (!operator_equal (self_.one () ,that))
			return TRUE ;
		return template_not_equal (self_.rest () ,that) ;
	}

	inline static BOOL template_less (const Tuple<> &self_ ,const WRAPPED &that) {
		return FALSE ;
	}

	template <class _ARG1>
	inline static BOOL template_less (const _ARG1 &self_ ,const WRAPPED &that) {
		if (operator_less (self_.one () ,that))
			return TRUE ;
		return template_less (self_.rest () ,that) ;
	}

	inline static BOOL template_not_less (const Tuple<> &self_ ,const WRAPPED &that) {
		return FALSE ;
	}

	template <class _ARG1>
	inline static BOOL template_not_less (const _ARG1 &self_ ,const WRAPPED &that) {
		if (!operator_less (self_.one () ,that))
			return TRUE ;
		return template_not_less (self_.rest () ,that) ;
	}
} ;

inline namespace EXTEND {
template <class... _ARGS>
inline static AllOfTuple<_ARGS...> _ALLOF_ (const _ARGS &...list) {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) > 0) ;
	TupleBinder<const _ARGS...> ret = TupleBinder<const _ARGS...> (list...) ;
	return std::move (_CAST_<AllOfTuple<_ARGS...>> (ret)) ;
}

template <class... _ARGS>
inline static AnyOfTuple<_ARGS...> _ANYOF_ (const _ARGS &...list) {
	_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) > 0) ;
	TupleBinder<const _ARGS...> ret = TupleBinder<const _ARGS...> (list...) ;
	return std::move (_CAST_<AnyOfTuple<_ARGS...>> (ret)) ;
}
} ;

template <class>
class StrongRef ;

template <class>
class WeakRef ;

template <class>
class SoftRef ;

template <>
class WeakRef<void> {
private:
	class Pack {
	private:
		template <class>
		friend class StrongRef ;
		template <class>
		friend class WeakRef ;
		AnyRef<void> mHolder ;
		LENGTH mCounter ;
	} ;

private:
	template <class>
	friend class StrongRef ;
	template <class>
	friend class WeakRef ;
	SharedRef<Pack> mThis ;
} ;

namespace U {
struct OPERATOR_RECAST {
	template <class _ARG1 ,class _ARG2>
	inline static PTR<_ARG2> template_recast (PTR<_ARG1> address ,const ARGV<_ARG2> & ,const ARGV<ENABLE_TYPE<stl::is_always_base_of<_ARG2 ,_ARG1>::value>> & ,const DEF<decltype (ARGVP3)> &) {
		return _XVALUE_<PTR<_ARG2>> (address) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static PTR<_ARG2> template_recast (PTR<_ARG1> address ,const ARGV<_ARG2> & ,const ARGV<ENABLE_TYPE<stl::is_always_base_of<Interface ,_ARG1>::value && stl::is_always_base_of<Interface ,_ARG2>::value>> & ,const DEF<decltype (ARGVP2)> &) {
		//@warn: RTTI might be different across DLL
		return dynamic_cast<PTR<_ARG2>> (address) ;
	}

	template <class _ARG1 ,class _ARG2>
	inline static PTR<_ARG2> template_recast (PTR<_ARG1> address ,const ARGV<_ARG2> & ,const DEF<decltype (ARGVPX)> & ,const DEF<decltype (ARGVP1)> &) {
		return NULL ;
	}

	template <class _RET ,class _ARG1>
	inline static PTR<_RET> invoke (PTR<_ARG1> address) {
		return template_recast (address ,_NULL_<ARGV<CAST_TRAITS_TYPE<_RET ,_ARG1>>> () ,ARGVPX ,ARGVP9) ;
	}
} ;
} ;

template <class UNIT>
class StrongRef {
private:
	using Pack = typename WeakRef<void>::Pack ;

	using Dependent = StrongRef ;

private:
	template <class>
	friend class StrongRef ;
	friend WeakRef<UNIT> ;
	friend SoftRef<UNIT> ;
	SharedRef<Pack> mThis ;
	PTR<UNIT> mPointer ;

public:
	inline StrongRef () noexcept {
		mPointer = NULL ;
	}

	//@warn: circular reference ruins StrongRef
	template <class _ARG1 ,class = ENABLE_TYPE<stl::is_always_base_of<UNIT ,_ARG1>::value>>
	inline implicit StrongRef (const StrongRef<_ARG1> &that)
		: StrongRef (that.template recast<UNIT> ()) {}

	inline implicit StrongRef (const DEPENDENT_TYPE<WeakRef<UNIT> ,Dependent> &that)
		: StrongRef (that.watch ()) {}

	inline implicit StrongRef (const DEPENDENT_TYPE<SoftRef<UNIT> ,Dependent> &that)
		: StrongRef (that.watch ()) {}

	inline ~StrongRef () noexcept {
		if (mPointer == NULL)
			return ;
		if (!mThis.exist ())
			return ;
		if (!mThis->mHolder.exist ())
			return ;
		if switch_case (TRUE) {
			const auto r1x = --mThis->mCounter ;
			if (r1x != 0)
				discard ;
			mThis->mHolder = AnyRef<void> () ;
		}
		mPointer = NULL ;
	}

	inline StrongRef (const StrongRef &that)
		:StrongRef (that.mThis ,that.mPointer) {
		_STATIC_WARNING_ ("noop") ;
	}

	inline StrongRef &operator= (const StrongRef &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~StrongRef () ;
			new (this) StrongRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline StrongRef (StrongRef &&that) noexcept {
		mThis = std::move (that.mThis) ;
		mPointer = _EXCHANGE_ (that.mPointer) ;
	}

	inline StrongRef &operator= (StrongRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~StrongRef () ;
			new (this) StrongRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		if (mPointer == NULL)
			return FALSE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

	template <class _RET>
	inline StrongRef<CAST_TRAITS_TYPE<_RET ,UNIT>> recast () const {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		const auto r1x = U::OPERATOR_RECAST::template invoke<_RET> (mPointer) ;
		_DYNAMIC_ASSERT_ (_EBOOL_ (r1x != NULL) == _EBOOL_ (mPointer != NULL)) ;
		return StrongRef<CAST_TRAITS_TYPE<_RET ,UNIT>> (mThis ,r1x) ;
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

	inline BOOL equal (const StrongRef &that) const {
		if (!mThis.exist ())
			if (!that.mThis.exist ())
				return TRUE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!that.mThis.exist ())
			return FALSE ;
		if (&mThis.self != &that.mThis.self)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const StrongRef &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const StrongRef &that) const {
		return !equal (that) ;
	}

	inline BOOL equal (const DEPENDENT_TYPE<WeakRef<UNIT> ,Dependent> &that) const {
		return that.equal ((*this)) ;
	}

	inline BOOL operator== (const WeakRef<UNIT> &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const WeakRef<UNIT> &that) const {
		return !equal (that) ;
	}

	inline BOOL equal (const DEPENDENT_TYPE<SoftRef<UNIT> ,Dependent> &that) const {
		return that.equal ((*this)) ;
	}

	inline BOOL operator== (const SoftRef<UNIT> &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const SoftRef<UNIT> &that) const {
		return !equal (that) ;
	}

public:
	template <class... _ARGS>
	inline static StrongRef make (_ARGS &&...initval) {
		auto rax = SharedRef<Pack>::make () ;
		rax->mHolder = AnyRef<REMOVE_CVR_TYPE<UNIT>>::make (std::forward<_ARGS> (initval)...) ;
		rax->mCounter = 0 ;
		auto &r1x = rax->mHolder.rebind<UNIT> ().self ;
		return StrongRef (rax ,&r1x) ;
	}

private:
	inline explicit StrongRef (const SharedRef<Pack> &holder ,PTR<UNIT> pointer)
		:StrongRef () {
		if (pointer == NULL)
			return ;
		if (!holder.exist ())
			return ;
		if (!holder->mHolder.exist ())
			return ;
		const auto r1x = ++holder->mCounter ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		(void) r1x ;
		mThis = holder ;
		mPointer = pointer ;
	}
} ;

template <class UNIT>
class WeakRef {
private:
	using Pack = typename WeakRef<void>::Pack ;

private:
	SharedRef<Pack> mThis ;
	PTR<UNIT> mPointer ;

public:
	inline WeakRef () noexcept {
		mPointer = NULL ;
	}

	inline BOOL exist () const {
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		if (mPointer == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL equal (const WeakRef &that) const {
		if (!mThis.exist ())
			if (!that.mThis.exist ())
				return TRUE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!that.mThis.exist ())
			return FALSE ;
		if (&mThis.self != &that.mThis.self)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const WeakRef &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const WeakRef &that) const {
		return !equal (that) ;
	}

	inline BOOL equal (const StrongRef<UNIT> &that) const {
		if (!mThis.exist ())
			if (!that.mThis.exist ())
				return TRUE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!that.mThis.exist ())
			return FALSE ;
		if (&mThis.self != &that.mThis.self)
			return FALSE ;
		return TRUE ;
	}

	inline BOOL operator== (const StrongRef<UNIT> &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const StrongRef<UNIT> &that) const {
		return !equal (that) ;
	}

	inline StrongRef<UNIT> watch () const {
		return StrongRef<UNIT> (mThis ,mPointer) ;
	}

	inline void assign (const StrongRef<UNIT> &that) {
		mThis = that.mThis ;
		mPointer = that.mPointer ;
	}

	inline void assign (StrongRef<UNIT> &&that) {
		mThis = std::move (that.mThis) ;
		mPointer = std::move (that.mPointer) ;
	}
} ;

template <class UNIT>
class SoftRef {
private:
	class Node {
	private:
		friend SoftRef ;
		StrongRef<UNIT> mHolder ;
		LENGTH mWeight ;

	public:
		inline Node ()
			:mWeight (0) {}
	} ;

private:
	SharedRef<FixedBuffer<Node>> mHeap ;
	WeakRef<UNIT> mWeakRef ;
	INDEX mIndex ;

public:
	inline SoftRef () {
		mIndex = VAR_NONE ;
	}

	inline explicit SoftRef (LENGTH len) {
		mHeap = SharedRef<FixedBuffer<Node>>::make (len) ;
		mIndex = VAR_NONE ;
	}

	inline SoftRef (const SoftRef &that) {
		mHeap = that.mHeap ;
		mWeakRef = that.mWeakRef ;
		mIndex = that.mIndex ;
		acquire () ;
	}

	inline SoftRef &operator= (const SoftRef &that) {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~SoftRef () ;
			new (this) SoftRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline SoftRef (SoftRef &&that) noexcept {
		mHeap = std::move (that.mHeap) ;
		mWeakRef = std::move (that.mWeakRef) ;
		mIndex = that.mIndex ;
	}

	inline SoftRef &operator= (SoftRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~SoftRef () ;
			new (this) SoftRef (std::move (that)) ;
		}
		return (*this) ;
	}

	LENGTH capacity () const {
		if (!mHeap.exist ())
			return 0 ;
		return mHeap->size () ;
	}

	inline BOOL exist () const {
		return mWeakRef.exist () ;
	}

	inline SoftRef share () popping {
		SoftRef ret ;
		ret.mHeap = mHeap ;
		ret.mIndex = VAR_NONE ;
		return std::move (ret) ;
	}

	inline BOOL equal (const SoftRef &that) const {
		return BOOL (mWeakRef == that.mWeakRef) ;
	}

	inline BOOL operator== (const SoftRef &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const SoftRef &that) const {
		return !equal (that) ;
	}

	inline BOOL equal (const StrongRef<UNIT> &that) const {
		return BOOL (mWeakRef == that) ;
	}

	inline BOOL operator== (const StrongRef<UNIT> &that) const {
		return equal (that) ;
	}

	inline BOOL operator!= (const StrongRef<UNIT> &that) const {
		return !equal (that) ;
	}

	inline StrongRef<UNIT> watch () const {
		if switch_case (TRUE) {
			if (!linked ())
				discard ;
			auto &r1x = mHeap.self[mIndex].mWeight ;
			const auto r2x = _EBOOL_ (r1x >= 0 && r1x < VAR32_MAX) ;
			const auto r3x = _EBOOL_ (r1x < 0 && r1x > VAR32_MIN) ;
			r1x += r2x - r3x ;
		}
		return mWeakRef.watch () ;
	}

	inline void assign (const StrongRef<UNIT> &that) {
		mWeakRef.assign (std::move (that)) ;
		mIndex = find_has_linked (mWeakRef) ;
		acquire () ;
	}

	inline void assign (StrongRef<UNIT> &&that) {
		mWeakRef.assign (std::move (that)) ;
		mIndex = find_has_linked (mWeakRef) ;
		acquire () ;
	}

	inline void as_strong () const {
		if (!linked ())
			return ;
		if (mHeap.self[mIndex].mWeight < 0)
			return ;
		mHeap.self[mIndex].mWeight = ~mHeap.self[mIndex].mWeight ;
	}

	inline void as_weak () const {
		if (!linked ())
			return ;
		if (mHeap.self[mIndex].mWeight >= 0)
			return ;
		mHeap.self[mIndex].mWeight = ~mHeap.self[mIndex].mWeight ;
	}

	inline void clean () const {
		if (!mHeap.exist ())
			return ;
		for (auto &&i : _RANGE_ (0 ,mHeap->size ())) {
			if (mHeap.self[i].mWeight < 0)
				continue ;
			mHeap.self[i].mHolder = StrongRef<UNIT> () ;
			mHeap.self[i].mWeight = 0 ;
		}
	}

private:
	inline BOOL linked () const {
		if (!mHeap.exist ())
			return FALSE ;
		if (!mWeakRef.exist ())
			return FALSE ;
		if (!(mIndex >= 0 && mIndex < mHeap->size ()))
			return FALSE ;
		if (mWeakRef != mHeap.self[mIndex].mHolder)
			return FALSE ;
		return TRUE ;
	}

	inline INDEX find_has_linked (const WeakRef<UNIT> &that) const {
		for (auto &&i : _RANGE_ (0 ,mHeap->size ()))
			if (mHeap.self[i].mHolder == that)
				return i ;
		return VAR_NONE ;
	}

	inline void acquire () {
		if (linked ())
			return ;
		if (!mHeap.exist ())
			return ;
		if (!mWeakRef.exist ())
			return ;
		if switch_case (TRUE) {
			mIndex = find_min_weight () ;
			if (mIndex == VAR_NONE)
				discard ;
			const auto r1x = easy_log2x (mHeap.self[mIndex].mWeight) ;
			if (r1x <= 0)
				discard ;
			for (auto &&i : _RANGE_ (0 ,mHeap->size ()))
				mHeap.self[i].mWeight = mHeap.self[i].mWeight >> r1x ;
		}
		_DYNAMIC_ASSERT_ (mIndex != VAR_NONE) ;
		mHeap.self[mIndex].mHolder = mWeakRef ;
		mHeap.self[mIndex].mWeight = 3 ;
	}

	inline VAR easy_log2x (VAR val) const {
		if (val <= 0)
			return VAR_NONE ;
		if (val == 1)
			return 0 ;
		return 1 + easy_log2x (val / 2) ;
	}

	inline INDEX find_min_weight () const {
		INDEX ret = VAR_NONE ;
		auto rax = LENGTH () ;
		for (auto &&i : _RANGE_ (0 ,mHeap->size ())) {
			const auto r1x = mHeap.self[i].mWeight ;
			if (r1x < 0)
				continue ;
			if (ret != VAR_NONE)
				if (rax <= r1x)
					continue ;
			ret = i ;
			rax = r1x ;
		}
		return std::move (ret) ;
	}
} ;

template <class UNIT>
class IntrusiveRef {
private:
	using INTRUSIVE_THIS = typename UNIT::INTRUSIVE_THIS ;

private:
	struct Detail ;
	friend ScopedGuard<IntrusiveRef> ;
	std::atomic<PTR<UNIT>> mPointer ;
	std::atomic<LENGTH> mLatch ;

public:
	inline IntrusiveRef () noexcept {
		mPointer = NULL ;
		mLatch = 0 ;
	}

	//@warn: address must be from 'IntrusiveRef::make'
	template <class _ARG1 ,class = ENABLE_TYPE<std::is_same<_ARG1 ,PTR<UNIT>>::value>>
	inline explicit IntrusiveRef (const _ARG1 &address)
		: IntrusiveRef (ARGVP0) {
		acquire (address ,FALSE) ;
		const auto r1x = safe_exchange (address) ;
		_DEBUG_ASSERT_ (r1x == NULL) ;
		(void) r1x ;
	}

	inline ~IntrusiveRef () noexcept {
		const auto r1x = safe_exchange (NULL) ;
		_CALL_TRY_ ([&] () {
			release (r1x) ;
		}) ;
	}

	inline IntrusiveRef (const IntrusiveRef &) = delete ;
	inline IntrusiveRef &operator= (const IntrusiveRef &) = delete ;

	inline IntrusiveRef (IntrusiveRef &&that) noexcept
		:IntrusiveRef (ARGVP0) {
		const auto r1x = that.safe_exchange (NULL) ;
		const auto r2x = safe_exchange (r1x) ;
		_DEBUG_ASSERT_ (r2x == NULL) ;
		(void) r2x ;
	}

	inline IntrusiveRef &operator= (IntrusiveRef &&that) noexcept {
		if switch_case (TRUE) {
			if (this == &that)
				discard ;
			(*this).~IntrusiveRef () ;
			new (this) IntrusiveRef (std::move (that)) ;
		}
		return (*this) ;
	}

	inline BOOL exist () const {
		const auto r1x = mPointer.load () ;
		if (r1x == NULL)
			return FALSE ;
		return TRUE ;
	}

	inline IntrusiveRef copy () popping {
		using LatchCounter = typename Detail::LatchCounter ;
		ScopedGuard<LatchCounter> ANONYMOUS (_CAST_<LatchCounter> (mLatch)) ;
		IntrusiveRef ret = IntrusiveRef (ARGVP0) ;
		const auto r1x = mPointer.load () ;
		acquire (r1x ,FALSE) ;
		const auto r2x = ret.safe_exchange (r1x) ;
		_DEBUG_ASSERT_ (r2x == NULL) ;
		(void) r2x ;
		return std::move (ret) ;
	}

	inline DEF<typename Detail::WatchProxy> watch () popping {
		using WatchProxy = typename Detail::WatchProxy ;
		using LatchCounter = typename Detail::LatchCounter ;
		ScopedGuard<LatchCounter> ANONYMOUS (_CAST_<LatchCounter> (mLatch)) ;
		const auto r1x = mPointer.load () ;
		_DYNAMIC_ASSERT_ (r1x != NULL) ;
		acquire (r1x ,FALSE) ;
		return WatchProxy (r1x) ;
	}

public:
	template <class... _ARGS>
	inline static IntrusiveRef make (_ARGS &&...initval) {
		IntrusiveRef ret = IntrusiveRef (ARGVP0) ;
		auto rax = GlobalHeap::alloc<TEMP<UNIT>> () ;
		ScopedBuild<UNIT> ANONYMOUS (rax ,std::forward<_ARGS> (initval)...) ;
		auto &r1x = _LOAD_<UNIT> (_XVALUE_<PTR<TEMP<UNIT>>> (rax)) ;
		acquire (&r1x ,TRUE) ;
		const auto r2x = ret.safe_exchange (&r1x) ;
		_DEBUG_ASSERT_ (r2x == NULL) ;
		(void) r2x ;
		rax = NULL ;
		return std::move (ret) ;
	}

private:
	inline explicit IntrusiveRef (const DEF<decltype (ARGVP0)> &) noexcept
		:mPointer (NULL) ,mLatch (0) {}

private:
	inline PTR<UNIT> safe_exchange (PTR<UNIT> address) noexcept popping {
#pragma GCC diagnostic push
#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
		const auto r1x = mPointer.exchange (address) ;
		if (r1x == NULL)
			return r1x ;
		INDEX ir = 0 ;
		while (TRUE) {
			const auto r2x = ir++ ;
			_DEBUG_ASSERT_ (r2x <= DEFAULT_RECURSIVE_SIZE::value) ;
			(void) r2x ;
			const auto r3x = mLatch.load () ;
			if (r3x == 0)
				break ;
			INTRUSIVE_THIS::friend_latch ((*r1x)) ;
		}
		return r1x ;
#pragma GCC diagnostic pop
	}

private:
	inline static void acquire (PTR<UNIT> address ,BOOL init) {
		if (address == NULL)
			return ;
		if (init)
			INTRUSIVE_THIS::friend_create ((*address)) ;
		const auto r1x = INTRUSIVE_THIS::friend_attach ((*address)) ;
		_DEBUG_ASSERT_ (r1x >= 1 + _EBOOL_ (!init)) ;
		(void) r1x ;
	}

	inline static void release (PTR<UNIT> address) {
		if (address == NULL)
			return ;
		const auto r1x = INTRUSIVE_THIS::friend_detach ((*address)) ;
		_DEBUG_ASSERT_ (r1x >= 0) ;
		if (r1x > 0)
			return ;
		INTRUSIVE_THIS::friend_destroy ((*address)) ;
		address->~UNIT () ;
		GlobalHeap::free (address) ;
	}
} ;

template <class UNIT>
struct IntrusiveRef<UNIT>::Detail {
	class WatchProxy final
		:private Proxy {
	private:
		friend IntrusiveRef ;
		PTR<UNIT> mPointer ;

	public:
		inline WatchProxy () = delete ;

		inline ~WatchProxy () noexcept {
			if (mPointer == NULL)
				return ;
			_CALL_TRY_ ([&] () {
				release (mPointer) ;
			}) ;
			mPointer = NULL ;
		}

		inline WatchProxy (const WatchProxy &) = default ;
		inline WatchProxy &operator= (const WatchProxy &) = default ;

		inline WatchProxy (WatchProxy &&) = default ;
		inline WatchProxy &operator= (WatchProxy &&) = default ;

		inline implicit operator UNIT & () const & noexcept {
			const auto r1x = static_cast<PTR<UNIT>> (mPointer) ;
			return (*r1x) ;
		}

		inline implicit operator UNIT & () && = delete ;

		template <class _RET ,class = ENABLE_TYPE<std::is_convertible<UNIT & ,_RET>::value>>
		inline implicit operator _RET () const & {
			const auto r1x = static_cast<PTR<UNIT>> (mPointer) ;
			return _RET ((*r1x)) ;
		}

		template <class _RET>
		inline implicit operator _RET () && = delete ;

	private:
		inline explicit WatchProxy (PTR<UNIT> pointer) noexcept
			:mPointer (pointer) {}
	} ;

	class LatchCounter
		:private Wrapped<std::atomic<LENGTH>> {
	public:
		inline void lock () {
			const auto r1x = ++LatchCounter::mSelf ;
			_DEBUG_ASSERT_ (r1x >= 1) ;
			(void) r1x ;
		}

		inline void unlock () {
			const auto r1x = --LatchCounter::mSelf ;
			_DEBUG_ASSERT_ (r1x >= 0) ;
			(void) r1x ;
		}
	} ;
} ;

inline namespace EXTEND {
inline constexpr INDEX _ALIGNAS_ (INDEX base ,LENGTH align_) {
	return base + (align_ - base % align_) % align_ ;
}
} ;

inline namespace EXTEND {
template <class _RET ,class _ARG1>
inline _RET _BITWISE_CAST_ (const _ARG1 &object) {
	_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
	_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_RET>::value) ;
	_STATIC_ASSERT_ (std::is_pod<_ARG1>::value) ;
	_STATIC_ASSERT_ (!std::is_pointer<_ARG1>::value) ;
	_STATIC_ASSERT_ (_SIZEOF_ (_RET) == _SIZEOF_ (_ARG1)) ;
	TEMP<_RET> ret ;
	_ZERO_ (ret) ;
	_MEMCOPY_ (PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_RET)]> (ret)] ,PTRTOARR[_CAST_<BYTE[_SIZEOF_ (_ARG1)]> (object)] ,_SIZEOF_ (_ARG1)) ;
	return std::move (_CAST_<_RET> (ret)) ;
}
} ;

class MemoryPool {
private:
	struct HEADER ;

	exports class Pool
		:public Interface {
	public:
		virtual LENGTH size () const = 0 ;
		virtual LENGTH length () const = 0 ;
		virtual PTR<HEADER> alloc (LENGTH) popping = 0 ;
		virtual void free (PTR<HEADER> address) noexcept = 0 ;
		virtual void clean () = 0 ;
	} ;

	struct HEADER {
		alignas (8) PTR<Pool> mFrom ;
		alignas (8) PTR<struct HEADER> mCurr ;
	} ;

private:
	struct Detail ;
	Buffer<StrongRef<Pool> ,ARGC<17>> mPool ;

public:
	inline MemoryPool () ;

	inline LENGTH size () const {
		LENGTH ret = 0 ;
		for (auto &&i : _RANGE_ (0 ,mPool.size ()))
			ret += mPool[i]->size () ;
		return std::move (ret) ;
	}

	inline LENGTH length () const {
		LENGTH ret = 0 ;
		for (auto &&i : _RANGE_ (0 ,mPool.size ()))
			ret += mPool[i]->length () ;
		return std::move (ret) ;
	}

	//@warn: held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<_RET> alloc () popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		const auto r1x = _MAX_ (_ALIGNOF_ (_RET) - _ALIGNOF_ (HEADER) ,VAR_ZERO) + _SIZEOF_ (_RET) ;
		INDEX ix = _MIN_ (((r1x - 1) / 8) ,_SIZEOF_ (HEADER)) ;
		const auto r2x = mPool[ix]->alloc (r1x) ;
		const auto r3x = _ALIGNAS_ (_ADDRESS_ (r2x) + _SIZEOF_ (HEADER) ,_ALIGNOF_ (_RET)) ;
		const auto r4x = r3x - _SIZEOF_ (HEADER) ;
		auto &r5x = _LOAD_<HEADER> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r4x)) ;
		r5x.mFrom = mPool[ix] ;
		r5x.mCurr = r2x ;
		auto &r6x = _LOAD_<_RET> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r3x)) ;
		return &r6x ;
	}

	//@warn: held by RAII to avoid static-memory-leaks
	template <class _RET>
	inline PTR<ARR<_RET>> alloc (LENGTH len) popping {
		_STATIC_ASSERT_ (!std::is_reference<_RET>::value) ;
		_STATIC_ASSERT_ (std::is_pod<_RET>::value) ;
		const auto r1x = _MAX_ (_ALIGNOF_ (_RET) - _ALIGNOF_ (HEADER) ,VAR_ZERO) + len * _SIZEOF_ (_RET) ;
		_DEBUG_ASSERT_ (r1x > 0) ;
		INDEX ix = _MIN_ (((r1x - 1) / 8) ,_SIZEOF_ (HEADER)) ;
		const auto r2x = mPool[ix]->alloc (r1x) ;
		const auto r3x = _ALIGNAS_ (_ADDRESS_ (r2x) + _SIZEOF_ (HEADER) ,_ALIGNOF_ (_RET)) ;
		const auto r4x = r3x - _SIZEOF_ (HEADER) ;
		auto &r5x = _LOAD_<HEADER> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r4x)) ;
		r5x.mFrom = mPool[ix] ;
		r5x.mCurr = r2x ;
		auto &r6x = _LOAD_<ARR<_RET>> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r3x)) ;
		return &r6x ;
	}

	template <class _ARG1>
	inline void free (const PTR<_ARG1> &address) noexcept {
		_STATIC_ASSERT_ (std::is_pod<REMOVE_ARRAY_TYPE<_ARG1>>::value) ;
		const auto r1x = _ADDRESS_ (address) - _SIZEOF_ (HEADER) ;
		auto &r2x = _LOAD_<HEADER> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r1x)) ;
		INDEX ix = _MEMCHR_ (mPool ,mPool.size () ,r2x.mFrom) ;
		mPool[ix]->free (r2x.mCurr) ;
	}

	inline void clean () {
		for (auto &&i : _RANGE_ (0 ,mPool.size ()))
			mPool[i]->clean () ;
	}
} ;

struct MemoryPool::Detail {
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

	template <class SIZE ,class RESE>
	class ImplPool
		:public Pool {
		_STATIC_ASSERT_ (SIZE::value > 0) ;
		_STATIC_ASSERT_ (RESE::value > 0) ;

	private:
		PTR<CHUNK> mRoot ;
		PTR<BLOCK> mFree ;
		LENGTH mSize ;
		LENGTH mLength ;

	public:
		inline ImplPool () noexcept {
			mRoot = NULL ;
			mFree = NULL ;
			mSize = 0 ;
			mLength = 0 ;
		}

		inline ~ImplPool () noexcept override {
			if (mRoot == NULL)
				return ;
			for (PTR<CHUNK> i = mRoot ,it ; i != NULL ; i = it) {
				it = i->mNext ;
				GlobalHeap::free (i->mOrigin) ;
			}
			mRoot = NULL ;
		}

		inline LENGTH size () const override {
			return mSize ;
		}

		inline LENGTH length () const override {
			return mLength ;
		}

		inline void reserve () {
			if (mFree != NULL)
				return ;
			const auto r1x = _ALIGNAS_ (_SIZEOF_ (BLOCK) + SIZE::value ,_ALIGNOF_ (BLOCK)) ;
			const auto r2x = _ALIGNOF_ (CHUNK) - 1 + _SIZEOF_ (CHUNK) + _ALIGNOF_ (BLOCK) - 1 + RESE::value * r1x ;
			auto rax = GlobalHeap::alloc<BYTE> (r2x) ;
			const auto r3x = _ADDRESS_ (_XVALUE_<PTR<ARR<BYTE>>> (rax)) ;
			const auto r4x = _ALIGNAS_ (r3x ,_ALIGNOF_ (CHUNK)) ;
			auto &r5x = _LOAD_<CHUNK> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r4x)) ;
			r5x.mOrigin = _XVALUE_<PTR<ARR<BYTE>>> (rax) ;
			r5x.mPrev = NULL ;
			r5x.mNext = mRoot ;
			r5x.mCount = RESE::value ;
			if (mRoot != NULL)
				mRoot->mPrev = &r5x ;
			mRoot = &r5x ;
			mSize += RESE::value * SIZE::value ;
			const auto r6x = _ALIGNAS_ (r4x + _SIZEOF_ (CHUNK) ,_ALIGNOF_ (BLOCK)) ;
			for (auto &&i : _RANGE_ (0 ,mRoot->mCount)) {
				const auto r7x = r6x + i * r1x ;
				auto &r8x = _LOAD_<BLOCK> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r7x)) ;
				r8x.mNext = mFree ;
				mFree = &r8x ;
			}
			rax = NULL ;
		}

		inline PTR<HEADER> alloc (LENGTH len) popping override {
			_DEBUG_ASSERT_ (len <= SIZE::value) ;
			reserve () ;
			const auto r1x = mFree ;
			mFree = r1x->mNext ;
			mLength += SIZE::value ;
			const auto r2x = _CAST_<TEMP<PTR<BLOCK>>> (_XVALUE_<LENGTH> (VAR_USED)) ;
			_CAST_<TEMP<PTR<BLOCK>>> (r1x->mNext) = r2x ;
			return &r1x->mFlexData ;
		}

		inline void free (PTR<HEADER> address) noexcept override {
			_DEBUG_ASSERT_ (address != NULL) ;
			auto &r1x = _OFFSET_ (&BLOCK::mFlexData ,(*address)) ;
			_DEBUG_ASSERT_ (_ADDRESS_ (r1x.mNext) == VAR_USED) ;
			r1x.mNext = mFree ;
			mFree = &r1x ;
			mLength -= SIZE::value ;
		}

		inline void clean () override {
			if (mSize == mLength)
				return ;
			for (PTR<CHUNK> i = mRoot ,it ; i != NULL ; i = it) {
				it = i->mNext ;
				if (!empty_node (i))
					continue ;
				auto &r1x = _SWITCH_ (
					(i->mPrev != NULL) ? i->mPrev->mNext :
					mRoot) ;
				r1x = i->mNext ;
				if (i->mNext != NULL)
					i->mNext->mPrev = i->mPrev ;
				mSize -= i->mCount * SIZE::value ;
				GlobalHeap::free (i->mOrigin) ;
			}
		}

	private:
		inline BOOL empty_node (PTR<const CHUNK> node) const {
			const auto r1x = _ALIGNAS_ (_SIZEOF_ (BLOCK) + SIZE::value ,_ALIGNOF_ (BLOCK)) ;
			const auto r2x = _ALIGNAS_ (_ADDRESS_ (node) + _SIZEOF_ (CHUNK) ,_ALIGNOF_ (BLOCK)) ;
			for (auto &&i : _RANGE_ (0 ,node->mCount)) {
				const auto r3x = r2x + i * r1x ;
				auto &r4x = _LOAD_<BLOCK> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r3x)) ;
				if (_ADDRESS_ (r4x.mNext) == VAR_USED)
					return FALSE ;
			}
			return TRUE ;
		}
	} ;

	struct FBLOCK {
		PTR<ARR<BYTE>> mOrigin ;
		PTR<struct FBLOCK> mPrev ;
		PTR<struct FBLOCK> mNext ;
		LENGTH mCount ;
		HEADER mFlexData ;
	} ;

	class HugePool
		:public Pool {
	private:
		PTR<FBLOCK> mRoot ;
		LENGTH mSize ;
		LENGTH mLength ;

	public:
		inline HugePool () noexcept {
			mRoot = NULL ;
			mLength = 0 ;
			mSize = 0 ;
		}

		inline ~HugePool () noexcept override {
			if (mRoot == NULL)
				return ;
			for (PTR<FBLOCK> i = mRoot ,it ; i != NULL ; i = it) {
				it = i->mNext ;
				GlobalHeap::free (i->mOrigin) ;
			}
			mRoot = NULL ;
		}

		inline LENGTH size () const override {
			return mSize ;
		}

		inline LENGTH length () const override {
			return mLength ;
		}

		inline PTR<HEADER> alloc (LENGTH len) popping override {
			const auto r1x = _ALIGNAS_ (len ,_ALIGNOF_ (FBLOCK)) ;
			const auto r2x = _ALIGNOF_ (FBLOCK) - 1 + _SIZEOF_ (FBLOCK) + r1x ;
			auto rax = GlobalHeap::alloc<BYTE> (r2x) ;
			const auto r3x = _ADDRESS_ (_XVALUE_<PTR<ARR<BYTE>>> (rax)) ;
			const auto r4x = _ALIGNAS_ (r3x ,_ALIGNOF_ (FBLOCK)) ;
			auto &r5x = _LOAD_<FBLOCK> (_XVALUE_<PTR<VOID>> (&_NULL_<BYTE> () + r4x)) ;
			r5x.mOrigin = _XVALUE_<PTR<ARR<BYTE>>> (rax) ;
			r5x.mPrev = NULL ;
			r5x.mNext = mRoot ;
			r5x.mCount = r1x ;
			if (mRoot != NULL)
				mRoot->mPrev = &r5x ;
			mRoot = &r5x ;
			mSize += r5x.mCount ;
			mLength += r5x.mCount ;
			rax = NULL ;
			return &r5x.mFlexData ;
		}

		inline void free (PTR<HEADER> address) noexcept override {
			_DEBUG_ASSERT_ (address != NULL) ;
			auto &r1x = _OFFSET_ (&FBLOCK::mFlexData ,(*address)) ;
			auto &r2x = _SWITCH_ (
				(r1x.mPrev != NULL) ? r1x.mPrev->mNext :
				mRoot) ;
			r2x = r1x.mNext ;
			if (r1x.mNext != NULL)
				r1x.mNext->mPrev = r1x.mPrev ;
			mSize -= r1x.mCount ;
			mLength -= r1x.mCount ;
			GlobalHeap::free (r1x.mOrigin) ;
		}

		inline void clean () override {
			_STATIC_WARNING_ ("noop") ;
		}
	} ;
} ;

inline MemoryPool::MemoryPool () {
	using ImplPool8 = typename Detail::template ImplPool<ARGC<8> ,ARGC<32>> ;
	using ImplPool16 = typename Detail::template ImplPool<ARGC<16> ,ARGC<32>> ;
	using ImplPool24 = typename Detail::template ImplPool<ARGC<24> ,ARGC<32>> ;
	using ImplPool32 = typename Detail::template ImplPool<ARGC<32> ,ARGC<32>> ;
	using ImplPool40 = typename Detail::template ImplPool<ARGC<40> ,ARGC<16>> ;
	using ImplPool48 = typename Detail::template ImplPool<ARGC<48> ,ARGC<16>> ;
	using ImplPool56 = typename Detail::template ImplPool<ARGC<56> ,ARGC<16>> ;
	using ImplPool64 = typename Detail::template ImplPool<ARGC<64> ,ARGC<16>> ;
	using ImplPool72 = typename Detail::template ImplPool<ARGC<72> ,ARGC<8>> ;
	using ImplPool80 = typename Detail::template ImplPool<ARGC<80> ,ARGC<8>> ;
	using ImplPool88 = typename Detail::template ImplPool<ARGC<88> ,ARGC<8>> ;
	using ImplPool96 = typename Detail::template ImplPool<ARGC<96> ,ARGC<8>> ;
	using ImplPool104 = typename Detail::template ImplPool<ARGC<104> ,ARGC<4>> ;
	using ImplPool112 = typename Detail::template ImplPool<ARGC<112> ,ARGC<4>> ;
	using ImplPool120 = typename Detail::template ImplPool<ARGC<120> ,ARGC<4>> ;
	using ImplPool128 = typename Detail::template ImplPool<ARGC<128> ,ARGC<4>> ;
	using HugePool = typename Detail::HugePool ;
	mPool[0] = StrongRef<ImplPool8>::make () ;
	mPool[1] = StrongRef<ImplPool16>::make () ;
	mPool[2] = StrongRef<ImplPool24>::make () ;
	mPool[3] = StrongRef<ImplPool32>::make () ;
	mPool[4] = StrongRef<ImplPool40>::make () ;
	mPool[5] = StrongRef<ImplPool48>::make () ;
	mPool[6] = StrongRef<ImplPool56>::make () ;
	mPool[7] = StrongRef<ImplPool64>::make () ;
	mPool[8] = StrongRef<ImplPool72>::make () ;
	mPool[9] = StrongRef<ImplPool80>::make () ;
	mPool[10] = StrongRef<ImplPool88>::make () ;
	mPool[11] = StrongRef<ImplPool96>::make () ;
	mPool[12] = StrongRef<ImplPool104>::make () ;
	mPool[13] = StrongRef<ImplPool112>::make () ;
	mPool[14] = StrongRef<ImplPool120>::make () ;
	mPool[15] = StrongRef<ImplPool128>::make () ;
	mPool[16] = StrongRef<HugePool>::make () ;
}

class Object ;

exports class Objective
	:public Interface {
public:
	virtual WeakRef<Object> &weak_of_this () = 0 ;
	virtual const WeakRef<Object> &weak_of_this () const = 0 ;
	virtual StrongRef<Object> clone () const = 0 ;
} ;

class Object
	:public Objective {
public:
	class Virtual ;

private:
	struct Detail ;
	WeakRef<Object> mWeakOfThis ;

public:
	inline Object () = delete ;

	template <class _ARG1>
	inline explicit Object (const ARGV<_ARG1> &) {
		_STATIC_ASSERT_ (stl::is_always_base_of<Object ,_ARG1>::value) ;
		_STATIC_ASSERT_ (!std::is_same<REMOVE_CVR_TYPE<_ARG1> ,Object>::value) ;
	}

	inline WeakRef<Object> &weak_of_this () override {
		return mWeakOfThis ;
	}

	inline const WeakRef<Object> &weak_of_this () const override {
		return mWeakOfThis ;
	}

	inline StrongRef<Object> clone () const override {
		return StrongRef<Object> () ;
	}
} ;

struct Object::Detail {
	class Metadata {
	private:
		friend Object ;
		LENGTH mObjectSize ;
		LENGTH mObjectAlign ;
		FLAG mObjectTypeMID ;
		Function<void (PTR<NONE>)> mConstrutor ;
		Function<void (PTR<NONE>)> mDestructor ;

	private:
		inline Metadata () = delete ;

		template <class _ARG1>
		inline explicit Metadata (const ARGV<_ARG1> &) {
			_STATIC_ASSERT_ (std::is_same<REMOVE_CVR_TYPE<_ARG1> ,_ARG1>::value) ;
			mObjectSize = _SIZEOF_ (_ARG1) ;
			mObjectAlign = _ALIGNOF_ (_ARG1) ;
			mObjectTypeMID = _TYPEMID_<_ARG1> () ;
			const auto r1x = _XVALUE_<PTR<void (PTR<NONE>)>> ([] (PTR<NONE> address) {
				auto &r2x = _LOAD_<TEMP<_ARG1>> (address) ;
				_CREATE_ (&r2x) ;
			}) ;
			mConstrutor = r1x ;
			const auto r3x = _XVALUE_<PTR<void (PTR<NONE>)>> ([] (PTR<NONE> address) {
				auto &r4x = _LOAD_<TEMP<_ARG1>> (address) ;
				_DESTROY_ (&r4x) ;
			}) ;
			mDestructor = r1x ;
		}
	} ;
} ;

class Object::Virtual
	:public virtual Object {
public:
	inline Virtual ()
		:Object (_NULL_<ARGV<decltype ((*this))>> ()) {}
} ;

template <class UNIT ,class CONT>
class Serializer {
private:
	exports class Binder
		:public Interface {
	public:
		virtual void compute_visit (UNIT &visitor ,CONT &context_) const = 0 ;
	} ;

private:
	struct Detail ;
	StrongRef<const Binder> mBinder ;

public:
	inline Serializer () = delete ;

	template <class... _ARGS>
	inline explicit Serializer (const DEF<_ARGS CONT::*> &...memptr) {
		using ImplBinder = typename Detail::template ImplBinder<_ARGS...> ;
		_STATIC_ASSERT_ (_CAPACITYOF_ (ARGVS<_ARGS...>) > 0) ;
		mBinder = StrongRef<const ImplBinder>::make (memptr...) ;
	}

	inline DEF<typename Detail::Member> operator() (CONT &context_) const popping {
		using Member = typename Detail::Member ;
		_DEBUG_ASSERT_ (mBinder.exist ()) ;
		return Member ((*this) ,context_) ;
	}
} ;

template <class UNIT ,class CONT>
struct Serializer<UNIT ,CONT>::Detail {
	class Member final
		:private Proxy {
	private:
		friend Serializer ;
		const Serializer &mBase ;
		CONT &mContext ;

	public:
		inline Member () = delete ;

		inline void friend_visit (UNIT &) const & = delete ;

		inline void friend_visit (UNIT &visitor) && {
			mBase.mBinder->compute_visit (visitor ,mContext) ;
		}

	private:
		inline explicit Member (const Serializer &base ,CONT &context_)
			: mBase (base) ,mContext (context_) {}
	} ;

	template <class... UNITS_>
	class ImplBinder
		:public Binder {
	private:
		Tuple<UNITS_...> mMemPtr ;

	public:
		inline ImplBinder () = delete ;

		inline explicit ImplBinder (const UNITS_ &...memptr)
			:mMemPtr (memptr...) {}

		inline void compute_visit (UNIT &visitor ,CONT &context_) const override {
			template_visit (visitor ,context_ ,mMemPtr) ;
		}

	private:
		inline void template_visit (UNIT &visitor ,CONT &context_ ,const Tuple<> &memptr) const {
			_STATIC_WARNING_ ("noop") ;
		}

		template <class _ARG1>
		inline void template_visit (UNIT &visitor ,CONT &context_ ,const _ARG1 &memptr) const {
			auto &r1x = (context_.*memptr.one ()) ;
			visitor.visit (r1x) ;
			template_visit (visitor ,context_ ,memptr.rest ()) ;
		}
	} ;
} ;
} ;