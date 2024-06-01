#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <exception>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include "csc_begin.h"

namespace CSC {
#ifdef __CSC_COMPILER_MSVC__
struct FUNCTION_calendar_from_timepoint {
	inline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		inline_memset (ret) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_CLANG__
struct FUNCTION_calendar_from_timepoint {
	inline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		inline_memset (ret) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_COMPILER_GNUC__
struct FUNCTION_calendar_from_timepoint {
	inline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		const auto r1x = FLAG (std::localtime ((&time))) ;
		inline_memcpy (Pointer::from (ret) ,Pointer::make (r1x) ,SIZE_OF<std::tm>::expr) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto calendar_from_timepoint = FUNCTION_calendar_from_timepoint () ;

struct TimePureLayout {
	std::chrono::system_clock::duration mTime ;
} ;

class TimeImplHolder implement Fat<TimeHolder ,TimeLayout> {
public:
	void initialize () override {
		const auto r1x = std::chrono::system_clock::now () ;
		fake.mThis = Ref<TimePureLayout>::make () ;
		fake.mThis->mTime = r1x.time_since_epoch () ;
	}

	void initialize (CREF<LENGTH> milliseconds_) override {
		const auto r1x = std::chrono::milliseconds (milliseconds_) ;
		fake.mThis = Ref<TimePureLayout>::make () ;
		fake.mThis->mTime = std::chrono::duration_cast<std::chrono::system_clock::duration> (r1x) ;
	}

	void initialize (CREF<TimeCalendar> calendar_) override {
		auto rax = std::tm () ;
		const auto r1x = calendar_.mYear - 1900 ;
		rax.tm_year = VAL32 (r1x * LENGTH (r1x > 0)) ;
		const auto r2x = calendar_.mMonth ;
		rax.tm_mon = VAL32 (r2x * LENGTH (r2x > 0)) ;
		rax.tm_mday = VAL32 (calendar_.mDay) ;
		const auto r3x = calendar_.mWDay - 1 ;
		rax.tm_wday = VAL32 (r3x * LENGTH (r3x > 0)) ;
		const auto r4x = calendar_.mYDay - 1 ;
		rax.tm_yday = VAL32 (r4x * LENGTH (r4x > 0)) ;
		rax.tm_hour = VAL32 (calendar_.mHour) ;
		rax.tm_min = VAL32 (calendar_.mMinute) ;
		rax.tm_sec = VAL32 (calendar_.mSecond) ;
		rax.tm_isdst = 0 ;
		const auto r5x = std::mktime ((&rax)) ;
		const auto r6x = std::chrono::system_clock::from_time_t (r5x) ;
		fake.mThis = Ref<TimePureLayout>::make () ;
		fake.mThis->mTime = r6x.time_since_epoch () ;
	}

	void initialize (CREF<TimeLayout> that) override {
		fake.mThis = Ref<TimePureLayout>::make () ;
		fake.mThis->mTime = that.mThis->mTime ;
	}

	LENGTH hours () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::hours> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH minutes () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::minutes> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH seconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH milliseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH microseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH nanoseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (fake.mThis->mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	TimeCalendar calendar () const override {
		TimeCalendar ret ;
		const auto r1x = std::chrono::system_clock::time_point (fake.mThis->mTime) ;
		const auto r2x = std::time_t (std::chrono::system_clock::to_time_t (r1x)) ;
		const auto r3x = calendar_from_timepoint (r2x) ;
		ret.mYear = r3x.tm_year + 1900 ;
		ret.mMonth = r3x.tm_mon + 1 ;
		ret.mDay = r3x.tm_mday ;
		ret.mWDay = r3x.tm_wday + 1 ;
		ret.mYDay = r3x.tm_yday + 1 ;
		ret.mHour = r3x.tm_hour ;
		ret.mMinute = r3x.tm_min ;
		ret.mSecond = r3x.tm_sec ;
		return move (ret) ;
	}

	TimeLayout add (CREF<TimeLayout> that) const override {
		TimeLayout ret ;
		ret.mThis = Ref<TimePureLayout>::make () ;
		ret.mThis->mTime = fake.mThis->mTime + that.mThis->mTime ;
		return move (ret) ;
	}

	TimeLayout sub (CREF<TimeLayout> that) const override {
		TimeLayout ret ;
		ret.mThis = Ref<TimePureLayout>::make () ;
		ret.mThis->mTime = fake.mThis->mTime - that.mThis->mTime ;
		return move (ret) ;
	}
} ;

exports VFat<TimeHolder> TimeHolder::create (VREF<TimeLayout> that) {
	return VFat<TimeHolder> (TimeImplHolder () ,that) ;
}

exports CFat<TimeHolder> TimeHolder::create (CREF<TimeLayout> that) {
	return CFat<TimeHolder> (TimeImplHolder () ,that) ;
}

template class External<RuntimeProcHolder ,RuntimeProcLayout> ;

exports VFat<RuntimeProcHolder> RuntimeProcHolder::create (VREF<RuntimeProcLayout> that) {
	return VFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::create () ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::create (CREF<RuntimeProcLayout> that) {
	return CFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::create () ,that) ;
}

struct AtomicPureLayout {
	Pin<std::atomic<VAL>> mAtomic ;
} ;

class AtomicImplHolder implement Fat<AtomicHolder ,AtomicLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<AtomicPureLayout>::make () ;
		fake.mThis->mAtomic = NULL ;
		fake.mThis->mAtomic->store (0) ;
	}

	VAL fetch () const override {
		if (fake.mThis == NULL)
			return 0 ;
		return fake.mThis->mAtomic->load (std::memory_order::memory_order_relaxed) ;
	}

	void store (CREF<VAL> item) const override {
		return fake.mThis->mAtomic->store (item ,std::memory_order::memory_order_relaxed) ;
	}

	VAL exchange (CREF<VAL> item) const override {
		return fake.mThis->mAtomic->exchange (item ,std::memory_order::memory_order_relaxed) ;
	}

	BOOL change (VREF<VAL> expect ,CREF<VAL> item) const override {
		return fake.mThis->mAtomic->compare_exchange_weak (expect ,item ,std::memory_order::memory_order_relaxed) ;
	}

	void replace (CREF<VAL> expect ,CREF<VAL> item) const override {
		auto rax = expect ;
		fake.mThis->mAtomic->compare_exchange_strong (rax ,item ,std::memory_order::memory_order_relaxed) ;
	}

	void increase () const override {
		fake.mThis->mAtomic->fetch_add (1 ,std::memory_order::memory_order_relaxed) ;
	}

	void decrease () const override {
		fake.mThis->mAtomic->fetch_sub (1 ,std::memory_order::memory_order_relaxed) ;
	}
} ;

exports VFat<AtomicHolder> AtomicHolder::create (VREF<AtomicLayout> that) {
	return VFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

exports CFat<AtomicHolder> AtomicHolder::create (CREF<AtomicLayout> that) {
	return CFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

struct MutexType {
	enum {
		Make ,
		Recursive ,
		Shared ,
		Unique ,
		ETC
	} ;
} ;

struct MutexPureLayout {
	FLAG mMutexType ;
	Pin<std::mutex> mMutex ;
	Pin<std::recursive_mutex> mRecursive ;
	Atomic mShared ;
	Pin<std::condition_variable> mUnique ;
} ;

class MutexImplHolder implement Fat<MutexHolder ,MutexLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<MutexPureLayout>::make () ;
		fake.mThis->mMutexType = MutexType::Make ;
		fake.mThis->mMutex = NULL ;
		fake.mThis->mRecursive = NULL ;
		fake.mThis->mUnique = NULL ;
	}

	Ref<MutexPureLayout> borrow () const override {
		assert (fake.mThis != NULL) ;
		return Ref<MutexPureLayout>::reference (Pointer::make (address (fake.mThis.self))) ;
	}

	void enter () const override {
		if (fake.mThis == NULL)
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mMutexType != MutexType::Recursive)
				discard ;
			fake.mThis->mRecursive->lock () ;
		}
		if ifdo (act) {
			fake.mThis->mMutex->lock () ;
		}
	}

	void leave () const override {
		if (fake.mThis == NULL)
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mMutexType != MutexType::Recursive)
				discard ;
			fake.mThis->mRecursive->unlock () ;
		}
		if ifdo (act) {
			fake.mThis->mMutex->unlock () ;
		}
	}
} ;

exports VFat<MutexHolder> MutexHolder::create (VREF<MutexLayout> that) {
	return VFat<MutexHolder> (MutexImplHolder () ,that) ;
}

exports CFat<MutexHolder> MutexHolder::create (CREF<MutexLayout> that) {
	return CFat<MutexHolder> (MutexImplHolder () ,that) ;
}

class MakeMutexImplHolder implement Fat<MakeMutexHolder ,MutexLayout> {
public:
	void MakeMutex_initialize () override {
		MutexHolder::create (fake)->initialize () ;
	}

	void RecursiveMutex_initialize () override {
		MutexHolder::create (fake)->initialize () ;
		fake.mThis->mMutexType = MutexType::Recursive ;
	}

	void SharedMutex_initialize () override {
		MutexHolder::create (fake)->initialize () ;
		fake.mThis->mMutexType = MutexType::Shared ;
		fake.mThis->mShared = NULL ;
	}

	void UniqueMutex_initialize () override {
		MutexHolder::create (fake)->initialize () ;
		fake.mThis->mMutexType = MutexType::Unique ;
	}
} ;

exports VFat<MakeMutexHolder> MakeMutexHolder::create (VREF<MutexLayout> that) {
	return VFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

exports CFat<MakeMutexHolder> MakeMutexHolder::create (CREF<MutexLayout> that) {
	return CFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

class SharedAtomicMutex implement Proxy {
protected:
	Atomic mThat ;

public:
	imports VREF<SharedAtomicMutex> from (VREF<Atomic> that) {
		return Pointer::from (that) ;
	}

	void lock () {
		noop () ;
	}

	void unlock () {
		mThat.decrease () ;
	}
} ;

struct SharedLockPureLayout {
	Ref<MutexPureLayout> mMutex ;
	std::unique_lock<SharedAtomicMutex> mLock ;
} ;

class SharedLockImplHolder implement Fat<SharedLockHolder ,SharedLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Ref<SharedLockPureLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;	
		assert (fake.mThis->mMutex->mMutexType == MutexType::Shared) ;
		shared_enter () ;
		fake.mThis->mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (fake.mThis->mMutex->mShared)) ;
	}

	void shared_enter () {
		if ifdo (TRUE) {
			auto rax = fake.mThis->mMutex->mShared.fetch () ;
			while (TRUE) {
				rax = MathProc::abs (rax) ;
				const auto r1x = fake.mThis->mMutex->mShared.change (rax ,rax + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
		std::atomic_thread_fence (std::memory_order::memory_order_acquire) ;
	}

	BOOL busy () const override {
		return fake.mThis->mMutex->mShared.fetch () != IDEN ;
	}

	void enter () const override {
		fake.mThis->mMutex->mShared.decrease () ;
		fake.mThis->mMutex->mMutex->lock () ;
		if ifdo (TRUE) {
			auto rax = ZERO ;
			while (TRUE) {
				rax = ZERO ;
				const auto r1x = fake.mThis->mMutex->mShared.change (rax ,NONE) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
	}

	void leave () const override {
		std::atomic_thread_fence (std::memory_order::memory_order_release) ;
		fake.mThis->mMutex->mShared.replace (NONE ,IDEN) ;
		fake.mThis->mMutex->mMutex->unlock () ;
	}
} ;

exports VFat<SharedLockHolder> SharedLockHolder::create (VREF<SharedLockLayout> that) {
	return VFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

exports CFat<SharedLockHolder> SharedLockHolder::create (CREF<SharedLockLayout> that) {
	return CFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

struct UniqueLockPureLayout {
	Ref<MutexPureLayout> mMutex ;
	std::unique_lock<std::mutex> mLock ;
} ;

class UniqueLockImplHolder implement Fat<UniqueLockHolder ,UniqueLockLayout> {
public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Ref<UniqueLockPureLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		assert (fake.mThis->mMutex->mMutexType == MutexType::Unique) ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (fake.mThis->mMutex->mMutex.self) ;
	}

	void wait () override {
		fake.mThis->mMutex->mUnique->wait (fake.mThis->mLock) ;
	}

	void wait (CREF<Time> time) override {
		auto &&tmp = keep[TYPE<TimeLayout>::expr] (time) ;
		fake.mThis->mMutex->mUnique->wait_for (fake.mThis->mLock ,tmp.mThis->mTime) ;
	}

	void notify () override {
		fake.mThis->mMutex->mUnique->notify_all () ;
	}

	void yield () override {
		fake.mThis->mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (fake.mThis->mMutex->mMutex.self) ;
	}
} ;

exports VFat<UniqueLockHolder> UniqueLockHolder::create (VREF<UniqueLockLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::create (CREF<UniqueLockLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

struct ThreadPureLayout {
	Ref<ThreadFriend> mExecutor ;
	FLAG mUid ;
	INDEX mSlot ;
	Atomic mEnable ;
	std::thread mThread ;
} ;

class ThreadImplHolder implement Fat<ThreadHolder ,ThreadLayout> {
public:
	void initialize (RREF<Ref<ThreadFriend>> executor ,CREF<INDEX> slot) override {
		fake.mThis = Ref<ThreadPureLayout>::make () ;
		fake.mThis->mExecutor = move (executor) ;
		fake.mThis->mUid = ZERO ;
		fake.mThis->mSlot = slot ;
		fake.mThis->mEnable = NULL ;
	}

	FLAG thread_uid () const override {
		return fake.mThis->mUid ;
	}

	void start () override {
		const auto r1x = Wrapper<VREF<ThreadPureLayout>> (address (fake.mThis.self)) ;
		fake.mThis->mThread = std::thread ([=] () {
			r1x ([&] (VREF<ThreadPureLayout> me) {
				me.mUid = RuntimeProc::thread_uid () ;
				try {
					while (TRUE) {
						if (me.mEnable.fetch ())
							break ;
						me.mExecutor->friend_execute (me.mSlot) ;
					}
				} catch (CREF<Exception> e) {
					noop (e) ;
					me.mEnable.store (TRUE) ;
				}
			}) ;
		}) ;
	}

	void stop () override {
		fake.mThis->mEnable.store (TRUE) ;
		fake.mThis->mThread.join () ;
		fake.mThis->mThread = std::thread () ;
	}
} ;

exports VFat<ThreadHolder> ThreadHolder::create (VREF<ThreadLayout> that) {
	return VFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

exports CFat<ThreadHolder> ThreadHolder::create (CREF<ThreadLayout> that) {
	return CFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

template class External<ProcessHolder ,ProcessLayout> ;

exports VFat<ProcessHolder> ProcessHolder::create (VREF<ProcessLayout> that) {
	return VFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::create () ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::create (CREF<ProcessLayout> that) {
	return CFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::create () ,that) ;
}

template class External<ModuleHolder ,ModuleLayout> ;

exports VFat<ModuleHolder> ModuleHolder::create (VREF<ModuleLayout> that) {
	return VFat<ModuleHolder> (External<ModuleHolder ,ModuleLayout>::create () ,that) ;
}

exports CFat<ModuleHolder> ModuleHolder::create (CREF<ModuleLayout> that) {
	return CFat<ModuleHolder> (External<ModuleHolder ,ModuleLayout>::create () ,that) ;
}

class SystemImplHolder implement Fat<SystemHolder ,SystemLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void set_locale (CREF<String<STR>> name) const override {
		const auto r1x = StringProc::stra_from_strs (name) ;
		std::setlocale (LC_ALL ,r1x) ;
	}

	FLAG execute (CREF<String<STR>> command) const override {
		const auto r1x = StringProc::stra_from_strs (command) ;
		return FLAG (std::system (r1x)) ;
	}
} ;

exports VFat<SystemHolder> SystemHolder::create (VREF<SystemLayout> that) {
	return VFat<SystemHolder> (SystemImplHolder () ,that) ;
}

exports CFat<SystemHolder> SystemHolder::create (CREF<SystemLayout> that) {
	return CFat<SystemHolder> (SystemImplHolder () ,that) ;
}

struct RandomPureLayout {
	FLAG mSeed ;
	Pin<std::mt19937_64> mRandom ;
} ;

class RandomImplHolder implement Fat<RandomHolder ,RandomLayout> {
public:
	void initialize (CREF<FLAG> seed) override {
		fake.mThis = Ref<RandomPureLayout>::make () ;
		fake.mThis->mSeed = seed ;
		fake.mThis->mRandom = NULL ;
		fake.mThis->mRandom.self = std::mt19937_64 (seed) ;
	}

	FLAG seed () const override {
		return fake.mThis->mSeed ;
	}

	QUAD random_byte () const {
		return QUAD (fake.mThis->mRandom.self ()) ;
	}

	INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const override {
		assert (lb <= rb) ;
		const auto r1x = VAL64 (rb) - VAL64 (lb) + 1 ;
		const auto r2x = VAL64 (random_byte () & QUAD (VAL64_MAX)) ;
		return INDEX (r2x % r1x + lb) ;
	}

	Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const override {
		Array<INDEX> ret = Array<INDEX>::make (iter (0 ,size_)) ;
		random_shuffle (count ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) const override {
		assert (count >= 0) ;
		assert (count <= size_) ;
		assert (result.size () == size_) ;
		const auto r1x = result.size () - 1 ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= count)
				break ;
			INDEX iy = random_value (ix ,r1x) ;
			swap (result[ix] ,result[iy]) ;
			ix++ ;
		}
	}

	BitSet random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const override {
		BitSet ret = BitSet (size_) ;
		random_pick (count ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<BitSet> result) const override {
		assert (count >= 0) ;
		assert (count <= size_) ;
		assert (result.size () == size_) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (count >= size_ / 2)
				discard ;
			const auto r1x = random_shuffle (count ,size_) ;
			for (auto &&i : iter (0 ,count))
				result.add (r1x[i]) ;
		}
		if ifdo (act) {
			const auto r2x = random_shuffle (size_ - count ,size_) ;
			for (auto &&i : iter (size_ - count ,size_))
				result.add (r2x[i]) ;
		}
	}

	BOOL random_draw (CREF<FLT64> possibility) const override {
		const auto r1x = random_value (0 ,10000) ;
		const auto r2x = FLT64 (r1x) * MathProc::inverse (FLT64 (10000)) ;
		if (r2x < possibility)
			return TRUE ;
		return FALSE ;
	}
} ;

exports VFat<RandomHolder> RandomHolder::create (VREF<RandomLayout> that) {
	return VFat<RandomHolder> (RandomImplHolder () ,that) ;
}

exports CFat<RandomHolder> RandomHolder::create (CREF<RandomLayout> that) {
	return CFat<RandomHolder> (RandomImplHolder () ,that) ;
}

template class External<SingletonProcHolder ,SingletonProcLayout> ;

exports VFat<SingletonProcHolder> SingletonProcHolder::create (VREF<SingletonProcLayout> that) {
	return VFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::create () ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::create (CREF<SingletonProcLayout> that) {
	return CFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::create () ,that) ;
}

#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_dump_memory_leaks {
	forceinline void operator() () const {
		noop () ;
	}
} ;
#endif

static constexpr auto dump_memory_leaks = FUNCTION_dump_memory_leaks () ;

struct GlobalPureLayout {
	Set<Slice> mNameSet ;
	List<AutoRef<Pointer>> mGlobalList ;
} ;

class GlobalImplHolder implement Fat<GlobalHolder ,GlobalLayout> {
public:
	void initialize () override {
		fake.mMutex = RecursiveMutex () ;
		fake.mThis = SharedRef<GlobalPureLayout>::make () ;
	}

	void startup () const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		dump_memory_leaks () ;
	}

	RefLayout borrow (CREF<Slice> name ,RREF<UnknownHolder> value) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		INDEX ix = fake.mThis->mNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = fake.mThis->mGlobalList.insert () ;
			fake.mThis->mNameSet.add (name ,ix) ;
			fake.mThis->mGlobalList[ix] = AutoRef<Pointer> (move (value)) ;
		}
		return Ref<Pointer>::reference (fake.mThis->mGlobalList[ix]) ;
	}

	void shutdown () const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		fake.mThis->mNameSet.clear () ;
		fake.mThis->mGlobalList.clear () ;
	}
} ;

exports VFat<GlobalHolder> GlobalHolder::create (VREF<GlobalLayout> that) {
	return VFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}

exports CFat<GlobalHolder> GlobalHolder::create (CREF<GlobalLayout> that) {
	return CFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}

template class External<PathHolder ,PathLayout> ;

exports VFat<PathHolder> PathHolder::create (VREF<PathLayout> that) {
	return VFat<PathHolder> (External<PathHolder ,PathLayout>::create () ,that) ;
}

exports CFat<PathHolder> PathHolder::create (CREF<PathLayout> that) {
	return CFat<PathHolder> (External<PathHolder ,PathLayout>::create () ,that) ;
}

template class External<FileProcHolder ,FileProcLayout> ;

exports VFat<FileProcHolder> FileProcHolder::create (VREF<FileProcLayout> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::create () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::create (CREF<FileProcLayout> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::create () ,that) ;
}

template class External<StreamFileHolder ,StreamFileLayout> ;

exports VFat<StreamFileHolder> StreamFileHolder::create (VREF<StreamFileLayout> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::create () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::create (CREF<StreamFileLayout> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::create () ,that) ;
}

template class External<BufferFileHolder ,BufferFileLayout> ;

exports VFat<BufferFileHolder> BufferFileHolder::create (VREF<BufferFileLayout> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::create () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::create (CREF<BufferFileLayout> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::create () ,that) ;
}

template class External<ConsoleHolder ,ConsoleLayout> ;

exports VFat<ConsoleHolder> ConsoleHolder::create (VREF<ConsoleLayout> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::create () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::create (CREF<ConsoleLayout> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::create () ,that) ;
}
} ;