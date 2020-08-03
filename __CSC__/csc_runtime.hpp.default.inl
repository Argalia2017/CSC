#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_once")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_once
#undef discard
#endif

#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif
#endif

#ifdef __CSC_SYSTEM_LINUX__
#ifdef __CSC_DEPRECATED__
#include <unistd.h>
#include <sys/syscall.h>
#endif
#endif

#ifdef __CSC_DEPRECATED__
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :5039)
#endif
#include <cstdlib>
#include <locale>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cmath>
#include <random>

#include <setjmp.h>
#pragma warning (pop)
#pragma endregion
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_once")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using std::mutex ;
using std::recursive_mutex ;
using std::memory_order ;
using std::unique_lock ;
using std::condition_variable ;
using std::thread ;
using std::random_device ;
using std::mt19937 ;
using std::chrono::duration ;
using std::chrono::time_point ;
using std::chrono::hours ;
using std::chrono::minutes ;
using std::chrono::seconds ;
using std::chrono::milliseconds ;
using std::chrono::microseconds ;
using std::chrono::nanoseconds ;
using std::chrono::system_clock ;

using std::chrono::duration_cast ;
using std::chrono::time_point_cast ;
using std::this_thread::sleep_for ;
using std::this_thread::sleep_until ;
using std::this_thread::yield ;
using std::atomic_thread_fence ;

#ifndef __CSC_COMPILER_GNUC__
//@error: fuck g++4.8
using std::quick_exit ;
#endif

using std::exit ;
using std::terminate ;
using std::system ;

using ::setlocale ;
using ::longjmp ;

#ifdef __CSC_SYSTEM_WINDOWS__
using ::HANDLE ;

using ::GetCurrentThreadId ;
using ::GetCurrentProcessId ;
using ::OpenProcess ;
using ::CloseHandle ;
using ::GetProcessTimes ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
using ::pid_t ;
using ::getpgid ;
using ::getsid ;
#endif
} ;

class Duration::Private::Implement
	:public Abstract {
private:
	api::system_clock::duration mDuration ;

public:
	implicit Implement () = delete ;

	explicit Implement (const LENGTH &milliseconds_) {
		const auto r1x = api::milliseconds (milliseconds_) ;
		mDuration = api::duration_cast<api::system_clock::duration> (r1x) ;
	}

	explicit Implement (const ARRAY6<LENGTH> &time_) {
		const auto r1x = api::hours (time_[0]) ;
		const auto r2x = api::minutes (time_[1]) ;
		const auto r3x = api::seconds (time_[2]) ;
		const auto r4x = api::milliseconds (time_[3]) ;
		const auto r5x = api::microseconds (time_[4]) ;
		const auto r6x = api::nanoseconds (time_[5]) ;
		const auto r7x = r1x + r2x + r3x + r4x + r5x + r6x ;
		mDuration = api::duration_cast<api::system_clock::duration> (r7x) ;
	}

	explicit Implement (const api::system_clock::duration &time_) {
		mDuration = api::duration_cast<api::system_clock::duration> (time_) ;
	}

	const api::system_clock::duration &get_mDuration () const leftvalue {
		return mDuration ;
	}

	const Implement &native () const override {
		return DEREF[this] ;
	}

	LENGTH hours () const override {
		const auto r1x = api::duration_cast<api::hours> (mDuration) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH minutes () const override {
		const auto r1x = api::duration_cast<api::minutes> (mDuration) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH seconds () const override {
		const auto r1x = api::duration_cast<api::seconds> (mDuration) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH miliseconds () const override {
		const auto r1x = api::duration_cast<api::seconds> (mDuration) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH microseconds () const override {
		const auto r1x = api::duration_cast<api::microseconds> (mDuration) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH nanoseconds () const override {
		const auto r1x = api::duration_cast<api::nanoseconds> (mDuration) ;
		return LENGTH (r1x.count ()) ;
	}

	Duration add (const Duration &that) const override {
		const auto r1x = mDuration + that.native ().get_mDuration () ;
		const auto r2x = api::duration_cast<api::system_clock::duration> (r1x) ;
		return Duration (r2x) ;
	}

	Duration sub (const Duration &that) const override {
		const auto r1x = mDuration - that.native ().get_mDuration () ;
		const auto r2x = api::duration_cast<api::system_clock::duration> (r1x) ;
		return Duration (r2x) ;
	}
} ;

inline exports Duration::Duration (const LENGTH &milliseconds_) {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make (milliseconds_) ;
}

template <class _ARG1 ,class>
inline exports Duration::Duration (_ARG1 &&time_) {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make (_FORWARD_ (ARGV<_ARG1 &&>::null ,time_)) ;
}

class TimePoint::Private::Implement
	:public Abstract {
private:
	api::system_clock::time_point mTimePoint ;

public:
	implicit Implement () = delete ;

	explicit Implement (const ARRAY8<LENGTH> &time_) {
		auto rax = api::tm () ;
		_ZERO_ (rax) ;
		const auto r1x = (time_[0] - 1900) * _EBOOL_ (time_[0] > 0) ;
		rax.tm_year = VAR32 (r1x) ;
		const auto r2x = (time_[1] - 1) * _EBOOL_ (time_[1] > 0) ;
		rax.tm_mon = VAR32 (r2x) ;
		rax.tm_mday = VAR32 (time_[2]) ;
		const auto r3x = (time_[3] - 1) * _EBOOL_ (time_[3] > 0) ;
		rax.tm_wday = VAR32 (r3x) ;
		const auto r4x = (time_[4] - 1) * _EBOOL_ (time_[4] > 0) ;
		rax.tm_yday = VAR32 (r4x) ;
		rax.tm_hour = VAR32 (time_[5]) ;
		rax.tm_min = VAR32 (time_[6]) ;
		rax.tm_sec = VAR32 (time_[7]) ;
		const auto r5x = api::mktime (DEPTR[rax]) ;
		mTimePoint = api::system_clock::from_time_t (r5x) ;
	}

	explicit Implement (const api::system_clock::time_point &time_) {
		mTimePoint = api::time_point_cast<api::system_clock::duration> (time_) ;
	}

	const Implement &native () const override {
		return DEREF[this] ;
	}

	const api::system_clock::time_point &get_mTimePoint () const leftvalue {
		return mTimePoint ;
	}

#ifdef __CSC_COMPILER_MSVC__
	ARRAY8<LENGTH> calendar () const override {
		ARRAY8<LENGTH> ret ;
		ret.fill (0) ;
		const auto r1x = api::time_t (api::system_clock::to_time_t (mTimePoint)) ;
		auto rax = api::tm () ;
		_ZERO_ (rax) ;
		api::localtime_s (DEPTR[rax] ,DEPTR[r1x]) ;
		ret[0] = rax.tm_year + 1900 ;
		ret[1] = rax.tm_mon + 1 ;
		ret[2] = rax.tm_mday ;
		ret[3] = rax.tm_wday + 1 ;
		ret[4] = rax.tm_yday + 1 ;
		ret[5] = rax.tm_hour ;
		ret[6] = rax.tm_min ;
		ret[7] = rax.tm_sec ;
		return _MOVE_ (ret) ;
	}
#endif

#ifndef __CSC_COMPILER_MSVC__
	ARRAY8<LENGTH> calendar () const override {
		ARRAY8<LENGTH> ret ;
		ret.fill (0) ;
		const auto r1x = api::time_t (api::system_clock::to_time_t (mTimePoint)) ;
		auto rax = api::tm () ;
		_ZERO_ (rax) ;
		//@warn: not thread-safe due to internel storage
		const auto r2x = api::localtime (DEPTR[r1x]) ;
		_DEBUG_ASSERT_ (r2x != NULL) ;
		rax = DEREF[r2x] ;
		ret[0] = rax.tm_year + 1900 ;
		ret[1] = rax.tm_mon + 1 ;
		ret[2] = rax.tm_mday ;
		ret[3] = rax.tm_wday + 1 ;
		ret[4] = rax.tm_yday + 1 ;
		ret[5] = rax.tm_hour ;
		ret[6] = rax.tm_min ;
		ret[7] = rax.tm_sec ;
		return _MOVE_ (ret) ;
}
#endif

	TimePoint add (const Duration &that) const override {
		const auto r1x = mTimePoint + that.native ().get_mDuration () ;
		const auto r2x = api::time_point_cast<api::system_clock::duration> (r1x) ;
		return TimePoint (r2x) ;
	}

	Duration sub (const TimePoint &that) const override {
		const auto r1x = mTimePoint - that.native ().get_mTimePoint () ;
		const auto r2x = api::duration_cast<api::system_clock::duration> (r1x) ;
		return Duration (r2x) ;
	}
} ;

template <class _ARG1 ,class>
inline exports TimePoint::TimePoint (_ARG1 &&time_) {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make (_FORWARD_ (ARGV<_ARG1 &&>::null ,time_)) ;
}

class Mutex::Private::Implement
	:public Abstract {
private:
	api::mutex mMutex ;

public:
	implicit Implement () = default ;

	Implement &native () override {
		return DEREF[this] ;
	}

	const Implement &native () const override {
		return DEREF[this] ;
	}

	api::mutex &get_mMutex () leftvalue {
		return mMutex ;
	}

	void lock () override {
		mMutex.lock () ;
	}

	BOOL try_lock () override {
		return mMutex.try_lock () ;
	}

	void unlock () override {
		mMutex.unlock () ;
	}
} ;

inline exports Mutex::Mutex () {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make () ;
}

class RecursiveMutex::Private::Implement
	:public Abstract {
private:
	api::recursive_mutex mMutex ;

public:
	implicit Implement () = default ;

	Implement &native () override {
		return DEREF[this] ;
	}

	const Implement &native () const override {
		return DEREF[this] ;
	}

	api::recursive_mutex &get_mMutex () leftvalue {
		return mMutex ;
	}

	void lock () override {
		mMutex.lock () ;
	}

	BOOL try_lock () override {
		return mMutex.try_lock () ;
	}

	void unlock () override {
		mMutex.unlock () ;
	}
} ;

inline exports RecursiveMutex::RecursiveMutex () {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make () ;
}

class ConditionLock::Private::Implement
	:public Abstract {
private:
	api::condition_variable mConditionLock ;

public:
	implicit Implement () = default ;

	Implement &native () override {
		return DEREF[this] ;
	}

	const Implement &native () const override {
		return DEREF[this] ;
	}

	api::condition_variable &get_mConditionLock () leftvalue {
		return mConditionLock ;
	}
} ;

inline exports ConditionLock::ConditionLock () {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make () ;
}

class UniqueLock::Private::Implement
	:public Abstract {
private:
	PhanRef<Mutex> mMutex ;
	PhanRef<ConditionLock> mConditionLock ;
	api::unique_lock<api::mutex> mUniqueLock ;

public:
	implicit Implement () = delete ;

	explicit Implement (PhanRef<Mutex> &&mutex_ ,PhanRef<ConditionLock> &&condition_lock) {
		mMutex = _MOVE_ (mutex_) ;
		mConditionLock = _MOVE_ (condition_lock) ;
		mUniqueLock = api::unique_lock<api::mutex> (mMutex->native ().get_mMutex ()) ;
	}

	void wait () override {
		mConditionLock->native ().get_mConditionLock ().wait (mUniqueLock) ;
	}

	void wait (const TimePoint &time_) override {
		auto &r1x = time_.native ().get_mTimePoint () ;
		mConditionLock->native ().get_mConditionLock ().wait_until (mUniqueLock ,r1x) ;
	}

	void wait (const Duration &time_) override {
		auto &r1x = time_.native ().get_mDuration () ;
		mConditionLock->native ().get_mConditionLock ().wait_for (mUniqueLock ,r1x) ;
	}

	void yield () override {
		const auto r1x = api::milliseconds (0) ;
		mConditionLock->native ().get_mConditionLock ().wait_for (mUniqueLock ,r1x) ;
	}

	void notify () override {
		mConditionLock->native ().get_mConditionLock ().notify_all () ;
	}
} ;

inline exports UniqueLock::UniqueLock (PhanRef<Mutex> &&mutex_ ,PhanRef<ConditionLock> &&condition_lock) {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make (_MOVE_ (mutex_) ,_MOVE_ (condition_lock)) ;
}

class Thread::Private::Implement
	:public Abstract {
private:
	StrongRef<Binder> mRunnable ;
	api::thread mThread ;

public:
	implicit Implement () = delete ;

	explicit Implement (const StrongRef<Binder> &runnable) {
		mRunnable = runnable.share () ;
		mThread = api::thread (Private::Runnable (PhanRef<Binder>::make (mRunnable.self))) ;
	}

	void join () override {
		mThread.join () ;
	}
} ;

inline exports Thread::Thread (const StrongRef<Binder> &runnable) {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make (runnable) ;
}

inline exports TimePoint GlobalRuntime::clock_now () {
	const auto r1x = api::system_clock::now () ;
	return TimePoint (r1x) ;
}

inline exports TimePoint GlobalRuntime::clock_epoch () {
	const auto r1x = api::system_clock::duration::zero () ;
	const auto r2x = api::system_clock::time_point (r1x) ;
	return TimePoint (r2x) ;
}

#ifdef __CSC_SYSTEM_WINDOWS__
inline exports FLAG GlobalRuntime::thread_tid () {
	return FLAG (api::GetCurrentThreadId ()) ;
}
#endif

#ifdef __CSC_SYSTEM_LINUX__
inline exports FLAG GlobalRuntime::thread_tid () {
	return FLAG (syscall (SYS_gettid)) ;
}
#endif

inline exports void GlobalRuntime::thread_sleep (const Duration &time_) {
	auto &r1x = time_.native ().get_mDuration () ;
	api::sleep_for (r1x) ;
}

inline exports void GlobalRuntime::thread_sleep (const TimePoint &time_) {
	auto &r1x = time_.native ().get_mTimePoint () ;
	api::sleep_until (r1x) ;
}

inline exports void GlobalRuntime::thread_yield () {
	api::yield () ;
}

inline exports LENGTH GlobalRuntime::thread_concurrency () {
	return LENGTH (api::thread::hardware_concurrency ()) ;
}

inline exports void GlobalRuntime::thread_fence () {
	api::atomic_thread_fence (api::memory_order::memory_order_seq_cst) ;
}

inline exports void GlobalRuntime::locale_init (const Plain<STRA> &locale_) {
	api::setlocale (LC_ALL ,locale_.self) ;
}

#ifdef __CSC_SYSTEM_WINDOWS__
inline exports FLAG GlobalRuntime::process_pid () {
	return FLAG (api::GetCurrentProcessId ()) ;
}

inline exports Buffer<BYTE ,ARGC<128>> GlobalRuntime::process_info (const FLAG &pid) {
	Buffer<BYTE ,ARGC<128>> ret ;
	auto rax = ByteWriter<BYTE> (PhanBuffer<BYTE>::make (ret)) ;
	if switch_once (TRUE) {
		const auto r1x = UniqueRef<api::HANDLE> ([&] (api::HANDLE &me) {
			me = api::OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,VARY (pid)) ;
		} ,[] (api::HANDLE &me) {
			if (me == NULL)
				return ;
			api::CloseHandle (me) ;
		}) ;
		if (r1x.self == NULL)
			discard ;
		rax << VAR64 (pid) ;
		rax << ByteWriter<BYTE>::GAP ;
		rax << _PCSTRU8_ ("windows") ;
		rax << ByteWriter<BYTE>::GAP ;
		auto rbx = ARRAY4<FILETIME> () ;
		_ZERO_ (rbx[0]) ;
		_ZERO_ (rbx[1]) ;
		_ZERO_ (rbx[2]) ;
		_ZERO_ (rbx[3]) ;
		api::GetProcessTimes (r1x ,DEPTR[rbx[0]] ,DEPTR[rbx[1]] ,DEPTR[rbx[2]] ,DEPTR[rbx[3]]) ;
		const auto r2x = DATA ((DATA (rbx[0].dwHighDateTime) << 32) | DATA (rbx[0].dwLowDateTime)) ;
		rax << VAR64 (r2x) ;
		rax << ByteWriter<BYTE>::GAP ;
	}
	rax << ByteWriter<BYTE>::EOS ;
	return _MOVE_ (ret) ;
}

inline exports FLAG GlobalRuntime::process_info_pid (const PhanBuffer<const STRU8> &info) {
	_DEBUG_ASSERT_ (info.size () == 128) ;
	auto rax = ByteReader<BYTE> (PhanBuffer<const BYTE>::make (info)) ;
	const auto r1x = rax.read (ARGV<VAR64>::null) ;
	_DYNAMIC_ASSERT_ (r1x >= VAR32_MIN && r1x <= VAR32_MAX) ;
	return FLAG (r1x) ;
}
#endif

#ifdef __CSC_SYSTEM_LINUX__
inline exports FLAG GlobalRuntime::process_pid () {
	return FLAG (syscall (SYS_getpid)) ;
}

inline exports Buffer<BYTE ,ARGC<128>> GlobalRuntime::process_info (const FLAG &pid) {
	Buffer<BYTE ,ARGC<128>> ret ;
	auto rax = ByteWriter<BYTE> (PhanBuffer<BYTE>::make (ret)) ;
	if switch_once (TRUE) {
		const auto r1x = api::getpgid (api::pid_t (pid)) ;
		if (r1x < 0)
			discard ;
		rax << VAR64 (pid) ;
		rax << ByteWriter<BYTE>::GAP ;
		rax << _PCSTRU8_ ("linux") ;
		rax << ByteWriter<BYTE>::GAP ;
		rax << VAR64 (r1x) ;
		rax << ByteWriter<BYTE>::GAP ;
		const auto r2x = api::getsid (api::pid_t (pid)) ;
		rax << VAR64 (r2x) ;
		rax << ByteWriter<BYTE>::GAP ;
	}
	rax << ByteWriter<BYTE>::EOS ;
	return _MOVE_ (ret) ;
}

inline exports FLAG GlobalRuntime::process_info_pid (const PhanBuffer<const STRU8> &info) {
	_DEBUG_ASSERT_ (info.size () == 128) ;
	auto rax = ByteReader<BYTE> (PhanBuffer<const BYTE>::make (info)) ;
	const auto r1x = rax.read (ARGV<VAR64>::null) ;
	_DYNAMIC_ASSERT_ (r1x >= VAR32_MIN && r1x <= VAR32_MAX) ;
	return FLAG (r1x) ;
}
#endif

#ifndef __CSC_COMPILER_GNUC__
inline exports void GlobalRuntime::process_exit[[noreturn]] () {
	api::quick_exit (EXIT_FAILURE) ;
}
#endif

#ifdef __CSC_COMPILER_GNUC__
inline exports void GlobalRuntime::process_exit[[noreturn]] () {
	//@error: fuck g++4.8
	api::exit (EXIT_FAILURE) ;
}
#endif

inline exports void GlobalRuntime::process_abort[[noreturn]] () {
	api::terminate () ;
}


inline exports FLAG GlobalRuntime::system_exec (const String<STR> &cmd) {
	const auto r1x = StringProc::build_strs (ARGV<STRA>::null ,cmd) ;
	const auto r2x = api::system (r1x.raw ().self) ;
	return FLAG (r2x) ;
}

class RandomService::Private::Implement
	:public RandomService::Abstract {
private:
	AutoRef<api::random_device> mRandomSeed ;
	AutoRef<api::mt19937> mRandomDevice ;

public:
	implicit Implement () {
		mRandomSeed = AutoRef<api::random_device>::make () ;
		mRandomDevice = AutoRef<api::mt19937>::make (CHAR (mRandomSeed.self ())) ;
	}

	VAR entropy () const override {
		return VAR (mRandomSeed->entropy ()) ;
	}

	void reset_seed (const VAR &seed_) override {
		mRandomDevice = AutoRef<api::mt19937>::make (CHAR (seed_)) ;
	}

	VAR random_value () override {
		return VAR (mRandomDevice.self ()) ;
	}

	void random_skip (const LENGTH &len) override {
#pragma push_macro ("discard")
#undef discard
		mRandomDevice->discard (len) ;
#pragma pop_macro ("discard")
	}
} ;

inline exports RandomService::RandomService (const ARGVF<Singleton<RandomService>> &) {
	using Implement = typename Private::Implement ;
	mThis = StrongRef<Implement>::make () ;
}
} ;