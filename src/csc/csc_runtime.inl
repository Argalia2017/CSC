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
#ifdef __CSC_SYSTEM_WINDOWS__
struct FUNCTION_calendar_from_timepoint {
	inline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		CoreTool::inline_memset (Pointer::from (ret) ,SIZE_OF<std::tm>::expr) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_calendar_from_timepoint {
	inline std::tm operator() (CREF<std::time_t> time) const {
		std::tm ret ;
		const auto r1x = FLAG (std::localtime ((&time))) ;
		CoreTool::inline_memcpy (Pointer::from (ret) ,Pointer::make (r1x) ,SIZE_OF<std::tm>::expr) ;
		return move (ret) ;
	}
} ;
#endif

static constexpr auto calendar_from_timepoint = FUNCTION_calendar_from_timepoint () ;

struct TimeImplLayout {
	std::chrono::system_clock::duration mTime ;
} ;

class TimeImplement implement Fat<TimeHolder ,TimeLayout> {
private:
	require (ENUM_COMPR_LTEQ<SIZE_OF<Box<TimeImplLayout>> ,SIZE_OF<TimeLayout>>) ;
	require (ENUM_COMPR_LTEQ<ALIGN_OF<Box<TimeImplLayout>> ,ALIGN_OF<TimeLayout>>) ;

public:
	void initialize () override {
		const auto r1x = std::chrono::system_clock::now () ;
		fake.mThis = Box<TimeImplLayout>::make () ;
		fake.mThis->mTime = r1x.time_since_epoch () ;
	}

	void initialize (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) override {
		const auto r1x = std::chrono::milliseconds (milliseconds_) ;
		const auto r2x = std::chrono::nanoseconds (milliseconds_) ;
		const auto r3x = r1x + r2x ;
		fake.mThis = Box<TimeImplLayout>::make () ;
		fake.mThis->mTime = std::chrono::duration_cast<std::chrono::system_clock::duration> (r3x) ;
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
		fake.mThis = Box<TimeImplLayout>::make () ;
		fake.mThis->mTime = r6x.time_since_epoch () ;
	}

	void initialize (CREF<TimeLayout> that) override {
		fake.mThis = Box<TimeImplLayout>::make () ;
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
		ret.mThis = Box<TimeImplLayout>::make () ;
		ret.mThis->mTime = fake.mThis->mTime + that.mThis->mTime ;
		return move (ret) ;
	}

	TimeLayout sub (CREF<TimeLayout> that) const override {
		TimeLayout ret ;
		ret.mThis = Box<TimeImplLayout>::make () ;
		ret.mThis->mTime = fake.mThis->mTime - that.mThis->mTime ;
		return move (ret) ;
	}
} ;

exports VFat<TimeHolder> TimeHolder::create (VREF<TimeLayout> that) {
	return VFat<TimeHolder> (TimeImplement () ,Pointer::from (that)) ;
}

exports CFat<TimeHolder> TimeHolder::create (CREF<TimeLayout> that) {
	return CFat<TimeHolder> (TimeImplement () ,Pointer::from (that)) ;
}

struct AtomicImplLayout {
	Pin<std::atomic<VAL>> mAtomic ;
} ;

class AtomicImplement implement Fat<AtomicHolder ,AtomicLayout> {
public:
	void initialize () override {
		fake.mThis = Box<AtomicImplLayout>::make () ;
		fake.mThis->mAtomic.self = 0 ;
	}

	VAL fetch () const override {
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
	return VFat<AtomicHolder> (AtomicImplement () ,Pointer::from (that)) ;
}

exports CFat<AtomicHolder> AtomicHolder::create (CREF<AtomicLayout> that) {
	return CFat<AtomicHolder> (AtomicImplement () ,Pointer::from (that)) ;
}

struct MutexType {
	enum {
		Empty ,
		Recursive ,
		Shared ,
		Unique ,
		ETC
	} ;
} ;

struct MutexPureLayout {
	FLAG mMutexType ;
	Pin<std::mutex> mMutex ;
	Atomic mShared ;
	Pin<std::recursive_mutex> mRecursive ;
	Pin<std::condition_variable> mConditional ;
} ;

class MutexImplement implement Fat<MutexHolder ,MutexLayout> {
public:
	Ref<MutexPureLayout> borrow () const override {
		return Ref<MutexPureLayout>::reference (Pointer::make (address (fake.mThis.self))) ;
	}

	void enter () const override {
		if ifnot (fake.mThis.exist ())
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mMutexType == MutexType::Recursive)
				discard ;
			fake.mThis->mRecursive->lock () ;
		}
		if ifdo (act) {
			fake.mThis->mMutex->lock () ;
		}
	}

	void leave () const override {
		if ifnot (fake.mThis.exist ())
			return ;
		auto act = TRUE ;
		if ifdo (act) {
			if (fake.mThis->mMutexType == MutexType::Recursive)
				discard ;
			fake.mThis->mRecursive->unlock () ;
		}
		if ifdo (act) {
			fake.mThis->mMutex->unlock () ;
		}
	}
} ;

exports VFat<MutexHolder> MutexHolder::create (VREF<MutexLayout> that) {
	return VFat<MutexHolder> (MutexImplement () ,Pointer::from (that)) ;
}

exports CFat<MutexHolder> MutexHolder::create (CREF<MutexLayout> that) {
	return CFat<MutexHolder> (MutexImplement () ,Pointer::from (that)) ;
}

class MakeMutexImplement implement Fat<MakeMutexHolder ,MutexLayout> {
public:
	void RecursiveMutex_initialize () override {
		fake.mThis = Ref<MutexPureLayout>::make () ;
		fake.mThis->mMutexType = MutexType::Recursive ;
	}

	void SharedMutex_initialize () override {
		fake.mThis = Ref<MutexPureLayout>::make () ;
		fake.mThis->mMutexType = MutexType::Shared ;
		fake.mThis->mShared = NULL ;
	}

	void UniqueMutex_initialize () override {
		fake.mThis = Ref<MutexPureLayout>::make () ;
		fake.mThis->mMutexType = MutexType::Unique ;
	}
} ;

exports VFat<MakeMutexHolder> MakeMutexHolder::create (VREF<MutexLayout> that) {
	return VFat<MakeMutexHolder> (MakeMutexImplement () ,Pointer::from (that)) ;
}

exports CFat<MakeMutexHolder> MakeMutexHolder::create (CREF<MutexLayout> that) {
	return CFat<MakeMutexHolder> (MakeMutexImplement () ,Pointer::from (that)) ;
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

struct SharedLockImplLayout {
	Ref<MutexPureLayout> mMutex ;
	std::unique_lock<SharedAtomicMutex> mLock ;
} ;

class SharedLockImplement implement Fat<SharedLockHolder ,SharedLockLayout> {
private:
	require (ENUM_COMPR_LTEQ<SIZE_OF<Box<SharedLockImplLayout>> ,SIZE_OF<SharedLockLayout>>) ;
	require (ENUM_COMPR_LTEQ<ALIGN_OF<Box<SharedLockImplLayout>> ,ALIGN_OF<SharedLockLayout>>) ;

public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Box<SharedLockImplLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		shared_enter () ;
		fake.mThis->mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (fake.mThis->mMutex->mShared)) ;
	}

	void shared_enter () {
		if ifdo (TRUE) {
			auto rax = fake.mThis->mMutex->mShared.fetch () ;
			while (TRUE) {
				rax = MathTool::abs (rax) ;
				const auto r2x = fake.mThis->mMutex->mShared.change (rax ,rax + 1) ;
				if (r2x)
					break ;
				RuntimeTool::thread_yield () ;
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
				const auto r2x = fake.mThis->mMutex->mShared.change (rax ,NONE) ;
				if (r2x)
					break ;
				RuntimeTool::thread_yield () ;
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
	return VFat<SharedLockHolder> (SharedLockImplement () ,Pointer::from (that)) ;
}

exports CFat<SharedLockHolder> SharedLockHolder::create (CREF<SharedLockLayout> that) {
	return CFat<SharedLockHolder> (SharedLockImplement () ,Pointer::from (that)) ;
}

struct UniqueLockImplLayout {
	Ref<MutexPureLayout> mMutex ;
	std::unique_lock<std::mutex> mLock ;
} ;

class UniqueLockImplement implement Fat<UniqueLockHolder ,UniqueLockLayout> {
private:
	require (ENUM_COMPR_LTEQ<SIZE_OF<Box<UniqueLockImplLayout>> ,SIZE_OF<UniqueLockLayout>>) ;
	require (ENUM_COMPR_LTEQ<ALIGN_OF<Box<UniqueLockImplLayout>> ,ALIGN_OF<UniqueLockLayout>>) ;

public:
	void initialize (CREF<Mutex> mutex) override {
		fake.mThis = Box<UniqueLockImplLayout>::make () ;
		fake.mThis->mMutex = mutex.borrow () ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (fake.mThis->mMutex->mMutex.self) ;
	}

	void wait () override {
		fake.mThis->mMutex->mConditional->wait (fake.mThis->mLock) ;
	}

	void wait (CREF<Time> time) override {
		auto &&tmp = keep[TYPE<TimeLayout>::expr] (time) ;
		fake.mThis->mMutex->mConditional->wait_for (fake.mThis->mLock ,tmp.mThis->mTime) ;
	}

	void notify () override {
		fake.mThis->mMutex->mConditional->notify_all () ;
	}

	void yield () override {
		fake.mThis->mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		fake.mThis->mLock = std::unique_lock<std::mutex> (fake.mThis->mMutex->mMutex.self) ;
	}
} ;

exports VFat<UniqueLockHolder> UniqueLockHolder::create (VREF<UniqueLockLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplement () ,Pointer::from (that)) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::create (CREF<UniqueLockLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplement () ,Pointer::from (that)) ;
}

struct ThreadPureLayout {
	SharedRef<ThreadExecutable> mBinder ;
	INDEX mSlot ;
	Atomic mUid ;
	std::thread mThread ;
} ;

class ThreadImplement implement Fat<ThreadHolder ,ThreadLayout> {
public:
	void initialize (CREF<SharedRef<ThreadExecutable>> binder ,CREF<INDEX> slot) override {
		fake.mThis = Ref<ThreadPureLayout>::make () ;
		fake.mThis->mBinder = binder ;
		fake.mThis->mSlot = slot ;
		fake.mThis->mUid = NULL ;
	}

	FLAG thread_uid () const override {
		return fake.mThis->mUid.fetch () ;
	}

	void start () override {
		const auto r1x = Capture<VREF<ThreadPureLayout>> (fake.mThis.self) ;
		fake.mThis->mThread = std::thread ([r1x] () noexcept {
			r1x ([&] (VREF<ThreadPureLayout> me) {
				me.mUid.store (RuntimeTool::thread_uid ()) ;
				me.mBinder->execute (me.mSlot) ;
			}) ;
		}) ;
	}

	void stop () override {
		fake.mThis->mThread.join () ;
		fake.mThis->mThread = std::thread () ;
	}
} ;

exports VFat<ThreadHolder> ThreadHolder::create (VREF<ThreadLayout> that) {
	return VFat<ThreadHolder> (ThreadImplement () ,Pointer::from (that)) ;
}

exports CFat<ThreadHolder> ThreadHolder::create (CREF<ThreadLayout> that) {
	return CFat<ThreadHolder> (ThreadImplement () ,Pointer::from (that)) ;
}

class ProcessImplement implement Fat<ProcessHolder ,ProcessLayout> {
public:
	void initialize (CREF<FLAG> uid) override {
		unimplemented () ;
	}

	void initialize (CREF<ProcessSnapshot> snapshot_) override {
		unimplemented () ;
	}

	FLAG process_uid () const override {
		unimplemented () ;
		return FLAG () ;
	}

	ProcessSnapshot snapshot () const override {
		unimplemented () ;
		return ProcessSnapshot () ;
	}
} ;

exports VFat<ProcessHolder> ProcessHolder::create (VREF<ProcessLayout> that) {
	return VFat<ProcessHolder> (ProcessImplement () ,Pointer::from (that)) ;
}

exports CFat<ProcessHolder> ProcessHolder::create (CREF<ProcessLayout> that) {
	return CFat<ProcessHolder> (ProcessImplement () ,Pointer::from (that)) ;
}

class SystemImplement implement Fat<SystemHolder ,SystemLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	void set_locale (CREF<String> name) const override {
		unimplemented () ;
	}

	FLAG execute (CREF<String> command) const override {
		unimplemented () ;
		return FLAG () ;
	}
} ;

exports VFat<SystemHolder> SystemHolder::create (VREF<SystemLayout> that) {
	return VFat<SystemHolder> (SystemImplement () ,Pointer::from (that)) ;
}

exports CFat<SystemHolder> SystemHolder::create (CREF<SystemLayout> that) {
	return CFat<SystemHolder> (SystemImplement () ,Pointer::from (that)) ;
}

struct RandomPureLayout {
	FLAG mSeed ;
	Pin<std::mt19937_64> mRandom ;
} ;

class RandomImplement implement Fat<RandomHolder ,RandomLayout> {
public:
	void initialize (CREF<FLAG> seed) override {
		fake.mThis = Ref<RandomPureLayout>::make () ;
		fake.mThis->mSeed = seed ;
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
		const auto r2x = FLT64 (r1x) * MathTool::inverse (FLT64 (10000)) ;
		if (r2x < possibility)
			return TRUE ;
		return FALSE ;
	}
} ;

exports VFat<RandomHolder> RandomHolder::create (VREF<RandomLayout> that) {
	return VFat<RandomHolder> (RandomImplement () ,Pointer::from (that)) ;
}

exports CFat<RandomHolder> RandomHolder::create (CREF<RandomLayout> that) {
	return CFat<RandomHolder> (RandomImplement () ,Pointer::from (that)) ;
}

class SingletonProcImplement implement Fat<SingletonToolHolder ,SingletonToolLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	FLAG load (CREF<Clazz> clazz) const override {
		unimplemented () ;
		return FLAG () ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const override {
		unimplemented () ;
	}
} ;

exports VFat<SingletonToolHolder> SingletonToolHolder::create (VREF<SingletonToolLayout> that) {
	return VFat<SingletonToolHolder> (SingletonProcImplement () ,Pointer::from (that)) ;
}

exports CFat<SingletonToolHolder> SingletonToolHolder::create (CREF<SingletonToolLayout> that) {
	return CFat<SingletonToolHolder> (SingletonProcImplement () ,Pointer::from (that)) ;
}

struct GlobalPureLayout {
	Set<Slice<STR>> mNameSet ;
	List<AutoRef<Pointer>> mGlobalList ;
} ;

class GlobalImplement implement Fat<GlobalHolder ,GlobalPureLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	void startup () override {
		unimplemented () ;
	}

	RefLayout borrow (CREF<Slice<STR>> name ,CREF<Unknown> holder) override {
		RefLayout ret ;
		INDEX ix = fake.mNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = fake.mGlobalList.insert () ;
			fake.mNameSet.add (name ,ix) ;
			fake.mGlobalList[ix] = AutoRef<Pointer>::generate (holder) ;
		}
		ret.mPointer = address (fake.mGlobalList[ix].self) ;
		return move (ret) ;
	}

	void shutdown () override {
		fake.mNameSet.clear () ;
		fake.mGlobalList.clear () ;
	}
} ;

exports VFat<GlobalHolder> GlobalHolder::create (VREF<GlobalPureLayout> that) {
	return VFat<GlobalHolder> (SingletonProcImplement () ,Pointer::from (that)) ;
}

exports CFat<GlobalHolder> GlobalHolder::create (CREF<GlobalPureLayout> that) {
	return CFat<GlobalHolder> (SingletonProcImplement () ,Pointer::from (that)) ;
}

class PathImplement implement Fat<PathHolder ,PathLayout> {
public:
	void initialize (CREF<String> dire) override {
		fake.mPathName = dire ;
	}

	PathLayout root () const override {
		unimplemented () ;
		return PathLayout () ;
	}

	PathLayout parent () const override {
		unimplemented () ;
		return PathLayout () ;
	}

	PathLayout brother () const override {
		unimplemented () ;
		return PathLayout () ;
	}

	PathLayout child () const override {
		unimplemented () ;
		return PathLayout () ;
	}

	Array<PathLayout> list () const override {
		unimplemented () ;
		return Array<PathLayout> () ;
	}

	Array<PathLayout> list (CREF<LENGTH> size_) const override {
		unimplemented () ;
		return Array<PathLayout> () ;
	}

	BOOL equal (CREF<PathLayout> that) const override {
		return fake.mPathName == that.mPathName ;
	}

	String pathname () const override {
		return fake.mPathName ;
	}

	String absolute () const override {
		unimplemented () ;
		return String () ;
	}

	String path () const override {
		unimplemented () ;
		return String () ;
	}

	String name () const override {
		unimplemented () ;
		return String () ;
	}

	String extend () const override {
		unimplemented () ;
		return String () ;
	}
} ;

exports VFat<PathHolder> PathHolder::create (VREF<PathLayout> that) {
	return VFat<PathHolder> (PathImplement () ,Pointer::from (that)) ;
}

exports CFat<PathHolder> PathHolder::create (CREF<PathLayout> that) {
	return CFat<PathHolder> (PathImplement () ,Pointer::from (that)) ;
}

class FileProcImplement implement Fat<FileToolHolder ,FileToolLayout> {
public:
	void initialize () override {
		unimplemented () ;
	}

	RefBuffer<BYTE> load_file (CREF<String> file) const override {
		unimplemented () ;
		return RefBuffer<BYTE> () ;
	}

	void save_file (CREF<String> file ,CREF<RefBuffer<BYTE>> data) const override {
		unimplemented () ;
	}

	void copy_file (CREF<String> dst ,CREF<String> src) const override {
		unimplemented () ;
	}

	void move_file (CREF<String> dst ,CREF<String> src) const override {
		unimplemented () ;
	}

	void link_file (CREF<String> dst ,CREF<String> src) const override {
		unimplemented () ;
	}

	void erase_file (CREF<String> file) const override {
		unimplemented () ;
	}

	void build_dire (CREF<String> dire) const override {
		unimplemented () ;
	}

	void clear_dire (CREF<String> dire) const override {
		unimplemented () ;
	}

	void erase_dire (CREF<String> dire) const override {
		unimplemented () ;
	}
} ;

exports VFat<FileToolHolder> FileToolHolder::create (VREF<FileToolLayout> that) {
	return VFat<FileToolHolder> (FileProcImplement () ,Pointer::from (that)) ;
}

exports CFat<FileToolHolder> FileToolHolder::create (CREF<FileToolLayout> that) {
	return CFat<FileToolHolder> (FileProcImplement () ,Pointer::from (that)) ;
}

class StreamLoaderImplement implement Fat<StreamLoaderHolder ,StreamLoaderLayout> {
public:
	void initialize (CREF<String> file) override {
		unimplemented () ;
	}

	void open_r () override {
		unimplemented () ;
	}

	void open_w () override {
		unimplemented () ;
	}

	void open_a () override {
		unimplemented () ;
	}

	void close () override {
		unimplemented () ;
	}

	LENGTH size () const override {
		unimplemented () ;
		return FLAG () ;
	}

	void resize (CREF<LENGTH> size_) override {
		unimplemented () ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		unimplemented () ;
	}

	void read (VREF<RefBuffer<WORD>> item) override {
		unimplemented () ;
	}

	void read (VREF<RefBuffer<CHAR>> item) override {
		unimplemented () ;
	}

	void read (VREF<RefBuffer<QUAD>> item) override {
		unimplemented () ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		unimplemented () ;
	}

	void write (CREF<RefBuffer<WORD>> item) override {
		unimplemented () ;
	}

	void write (CREF<RefBuffer<CHAR>> item) override {
		unimplemented () ;
	}

	void write (CREF<RefBuffer<QUAD>> item) override {
		unimplemented () ;
	}

	void flush () override {
		unimplemented () ;
	}
} ;

exports VFat<StreamLoaderHolder> StreamLoaderHolder::create (VREF<StreamLoaderLayout> that) {
	return VFat<StreamLoaderHolder> (StreamLoaderImplement () ,Pointer::from (that)) ;
}

exports CFat<StreamLoaderHolder> StreamLoaderHolder::create (CREF<StreamLoaderLayout> that) {
	return CFat<StreamLoaderHolder> (StreamLoaderImplement () ,Pointer::from (that)) ;
}

class BufferLoaderImplement implement Fat<BufferLoaderHolder ,BufferLoaderLayout> {
public:
	void initialize (CREF<String> file) override {
		unimplemented () ;
	}

	void set_cache_size (CREF<LENGTH> size_) override {
		unimplemented () ;
	}

	void open_r () override {
		unimplemented () ;
	}

	void open_w () override {
		unimplemented () ;
	}

	void open_a () override {
		unimplemented () ;
	}

	void close () override {
		unimplemented () ;
	}

	LENGTH size () const override {
		unimplemented () ;
		return FLAG () ;
	}

	void resize (CREF<LENGTH> size_) override {
		unimplemented () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) override {
		unimplemented () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<WORD>> item) override {
		unimplemented () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<CHAR>> item) override {
		unimplemented () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<QUAD>> item) override {
		unimplemented () ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) override {
		unimplemented () ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<WORD>> item) override {
		unimplemented () ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<CHAR>> item) override {
		unimplemented () ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<QUAD>> item) override {
		unimplemented () ;
	}

	void flush () override {
		unimplemented () ;
	}
} ;

exports VFat<BufferLoaderHolder> BufferLoaderHolder::create (VREF<BufferLoaderLayout> that) {
	return VFat<BufferLoaderHolder> (BufferLoaderImplement () ,Pointer::from (that)) ;
}

exports CFat<BufferLoaderHolder> BufferLoaderHolder::create (CREF<BufferLoaderLayout> that) {
	return CFat<BufferLoaderHolder> (BufferLoaderImplement () ,Pointer::from (that)) ;
}
} ;