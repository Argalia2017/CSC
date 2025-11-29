#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <clocale>
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
	forceinline std::tm operator() (CR<std::time_t> time) const {
		std::tm ret ;
		inline_memset (ret) ;
		localtime_s ((&ret) ,(&time)) ;
		return move (ret) ;
	}
} ;
#endif

#ifdef __CSC_SYSTEM_LINUX__
struct FUNCTION_calendar_from_timepoint {
	forceinline std::tm operator() (CR<std::time_t> time) const {
		const auto r1x = FLAG (std::localtime (&time)) ;
		return bitwise[TYPE<std::tm>::expr] (Pointer::make (r1x)) ;
	}
} ;
#endif

static constexpr auto calendar_from_timepoint = FUNCTION_calendar_from_timepoint () ;

struct TimeLayout {
	std::chrono::system_clock::duration mTime ;
} ;

class TimeImplHolder final implement Fat<TimeHolder ,TimeLayout> {
public:
	void initialize () override {
		const auto r1x = std::chrono::system_clock::now () ;
		self.mTime = r1x.time_since_epoch () ;
	}

	void initialize (CR<LENGTH> milliseconds_) override {
		const auto r1x = std::chrono::milliseconds (milliseconds_) ;
		self.mTime = std::chrono::duration_cast<std::chrono::system_clock::duration> (r1x) ;
	}

	void initialize (CR<TimeCalendar> calendar_) override {
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
		const auto r5x = std::mktime (&rax) ;
		const auto r6x = std::chrono::system_clock::from_time_t (r5x) ;
		self.mTime = r6x.time_since_epoch () ;
	}

	void initialize (CR<TimeLayout> that) override {
		self.mTime = that.mTime ;
	}

	Ref<TimeLayout> borrow () const leftvalue override {
		return Ref<TimeLayout>::reference (self) ;
	}

	LENGTH megaseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<10000000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH kiloseconds () const override {
		using R1X = std::chrono::duration<csc_int64_t ,std::ratio<1000>> ;
		const auto r1x = std::chrono::duration_cast<R1X> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH seconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::seconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH milliseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::milliseconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH microseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::microseconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	LENGTH nanoseconds () const override {
		const auto r1x = std::chrono::duration_cast<std::chrono::nanoseconds> (self.mTime) ;
		return LENGTH (r1x.count ()) ;
	}

	TimeCalendar calendar () const override {
		TimeCalendar ret ;
		const auto r1x = std::chrono::system_clock::time_point (self.mTime) ;
		const auto r2x = std::time_t (std::chrono::system_clock::to_time_t (r1x)) ;
		const auto r3x = calendar_from_timepoint (r2x) ;
		ret.mYear = LENGTH (r3x.tm_year) + 1900 ;
		ret.mMonth = LENGTH (r3x.tm_mon) + 1 ;
		ret.mDay = LENGTH (r3x.tm_mday) ;
		ret.mWDay = LENGTH (r3x.tm_wday) + 1 ;
		ret.mYDay = LENGTH (r3x.tm_yday) + 1 ;
		ret.mHour = LENGTH (r3x.tm_hour) ;
		ret.mMinute = LENGTH (r3x.tm_min) ;
		ret.mSecond = LENGTH (r3x.tm_sec) ;
		return move (ret) ;
	}

	OfThis<Box<TimeLayout ,TimeStorage>> sadd (CR<TimeLayout> that) const override {
		OfThis<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::create () ;
		ret.mThis->mTime = self.mTime + that.mTime ;
		return move (ret) ;
	}

	OfThis<Box<TimeLayout ,TimeStorage>> ssub (CR<TimeLayout> that) const override {
		OfThis<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::create () ;
		ret.mThis->mTime = self.mTime - that.mTime ;
		return move (ret) ;
	}
} ;

exports Box<TimeLayout ,TimeStorage> TimeHolder::create () {
	return Box<TimeLayout>::make () ;
}

exports VFat<TimeHolder> TimeHolder::hold (VR<TimeLayout> that) {
	return VFat<TimeHolder> (TimeImplHolder () ,that) ;
}

exports CFat<TimeHolder> TimeHolder::hold (CR<TimeLayout> that) {
	return CFat<TimeHolder> (TimeImplHolder () ,that) ;
}

template class External<RuntimeProcHolder ,RuntimeProcLayout> ;

struct RuntimeProcLayout {} ;

exports CR<OfThis<UniqueRef<RuntimeProcLayout>>> RuntimeProcHolder::expr_m () {
	return memorize ([&] () {
		OfThis<UniqueRef<RuntimeProcLayout>> ret ;
		ret.mThis = UniqueRef<RuntimeProcLayout>::make () ;
		RuntimeProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<RuntimeProcHolder> RuntimeProcHolder::hold (VR<RuntimeProcLayout> that) {
	return VFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::expr ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::hold (CR<RuntimeProcLayout> that) {
	return CFat<RuntimeProcHolder> (External<RuntimeProcHolder ,RuntimeProcLayout>::expr ,that) ;
}

struct AtomicLayout {
	std::atomic<VAL> mAtomic ;
} ;

class AtomicImplHolder final implement Fat<AtomicHolder ,AtomicLayout> {
public:
	void initialize () override {
		noop () ;
	}

	VAL fetch () override {
		return self.mAtomic.load (std::memory_order_relaxed) ;
	}

	void store (CR<VAL> item) override {
		return self.mAtomic.store (item ,std::memory_order_relaxed) ;
	}

	VAL exchange (CR<VAL> item) override {
		return self.mAtomic.exchange (item ,std::memory_order_relaxed) ;
	}

	BOOL change (VR<VAL> expect ,CR<VAL> item) override {
		return self.mAtomic.compare_exchange_weak (expect ,item ,std::memory_order_relaxed) ;
	}

	void replace (CR<VAL> expect ,CR<VAL> item) override {
		auto rax = expect ;
		self.mAtomic.compare_exchange_strong (rax ,item ,std::memory_order_relaxed) ;
	}

	void increase () override {
		self.mAtomic.fetch_add (1 ,std::memory_order_relaxed) ;
	}

	void decrease () override {
		self.mAtomic.fetch_sub (1 ,std::memory_order_relaxed) ;
	}
} ;

exports Box<AtomicLayout ,AtomicStorage> AtomicHolder::create () {
	return Box<AtomicLayout>::make () ;
}

exports VFat<AtomicHolder> AtomicHolder::hold (VR<AtomicLayout> that) {
	return VFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

exports CFat<AtomicHolder> AtomicHolder::hold (CR<AtomicLayout> that) {
	return CFat<AtomicHolder> (AtomicImplHolder () ,that) ;
}

struct MutexType {
	enum {
		Basic ,
		Once ,
		OnceDone ,
		Shared ,
		Unique ,
		ETC
	} ;
} ;

struct MutexLayout {
	Just<MutexType> mType ;
	Box<std::mutex> mBasic ;
	Atomic mShared ;
	Box<std::condition_variable> mUnique ;
} ;

class MutexImplHolder final implement Fat<MutexHolder ,MutexLayout> {
public:
	void initialize () override {
		self.mType = MutexType::Basic ;
		self.mBasic.remake () ;
	}

	Ref<MutexLayout> borrow () leftvalue override {
		return Ref<MutexLayout>::reference (self) ;
	}

	BOOL done () override {
		if (self.mType == MutexType::OnceDone)
			return TRUE ;
		return FALSE ;
	}

	void enter () override {
		if (done ())
			return ;
		self.mBasic->lock () ;
	}

	void leave () override {
		if (done ())
			return ;
		replace (self.mType ,FLAG (MutexType::Once) ,MutexType::OnceDone) ;
		self.mBasic->unlock () ;
	}
} ;

exports SharedRef<MutexLayout> MutexHolder::create () {
	return SharedRef<MutexLayout>::make () ;
}

exports VFat<MutexHolder> MutexHolder::hold (VR<MutexLayout> that) {
	return VFat<MutexHolder> (MutexImplHolder () ,that) ;
}

exports CFat<MutexHolder> MutexHolder::hold (CR<MutexLayout> that) {
	return CFat<MutexHolder> (MutexImplHolder () ,that) ;
}

class MakeMutexImplHolder final implement Fat<MakeMutexHolder ,MutexLayout> {
public:
	void make_OnceMutex () override {
		self.mType = MutexType::Once ;
		self.mBasic.remake () ;
	}

	void make_SharedMutex () override {
		self.mType = MutexType::Shared ;
		self.mBasic.remake () ;
		self.mShared = NULL ;
	}

	void make_UniqueMutex () override {
		self.mType = MutexType::Unique ;
		self.mBasic.remake () ;
		self.mUnique.remake () ;
	}
} ;

exports VFat<MakeMutexHolder> MakeMutexHolder::hold (VR<MutexLayout> that) {
	return VFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

exports CFat<MakeMutexHolder> MakeMutexHolder::hold (CR<MutexLayout> that) {
	return CFat<MakeMutexHolder> (MakeMutexImplHolder () ,that) ;
}

class SharedAtomicMutex implement Proxy {
protected:
	Atomic mThat ;

public:
	static VR<SharedAtomicMutex> from (VR<Atomic> that) {
		return Pointer::from (that) ;
	}

	void lock () {
		noop () ;
	}

	void unlock () {
		mThat.decrease () ;
	}
} ;

struct SharedLockLayout {
	Ref<MutexLayout> mMutex ;
	std::unique_lock<SharedAtomicMutex> mLock ;
} ;

class SharedLockImplHolder final implement Fat<SharedLockHolder ,SharedLockLayout> {
public:
	void initialize (CR<Mutex> mutex) override {
		self.mMutex = mutex.borrow () ;
		assert (self.mMutex.exclusive ()) ;
		assert (self.mMutex->mType == MutexType::Shared) ;
		shared_enter () ;
		self.mLock = std::unique_lock<SharedAtomicMutex> (SharedAtomicMutex::from (self.mMutex->mShared)) ;
	}

	void shared_enter () {
		if ifdo (TRUE) {
			auto rax = self.mMutex->mShared.fetch () ;
			while (TRUE) {
				rax = MathProc::abs (rax) ;
				const auto r1x = self.mMutex->mShared.change (rax ,rax + 1) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
		std::atomic_thread_fence (std::memory_order_acquire) ;
	}

	BOOL busy () const override {
		return self.mMutex->mShared.fetch () != IDEN ;
	}

	void enter () override {
		self.mMutex->mShared.decrease () ;
		self.mMutex->mBasic->lock () ;
		if ifdo (TRUE) {
			auto rax = ZERO ;
			while (TRUE) {
				rax = ZERO ;
				const auto r1x = self.mMutex->mShared.change (rax ,NONE) ;
				if (r1x)
					break ;
				RuntimeProc::thread_yield () ;
			}
		}
	}

	void leave () override {
		std::atomic_thread_fence (std::memory_order_release) ;
		self.mMutex->mShared.replace (NONE ,IDEN) ;
		self.mMutex->mBasic->unlock () ;
	}
} ;

exports Box<SharedLockLayout ,SharedLockStorage> SharedLockHolder::create () {
	return Box<SharedLockLayout>::make () ;
}

exports VFat<SharedLockHolder> SharedLockHolder::hold (VR<SharedLockLayout> that) {
	return VFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

exports CFat<SharedLockHolder> SharedLockHolder::hold (CR<SharedLockLayout> that) {
	return CFat<SharedLockHolder> (SharedLockImplHolder () ,that) ;
}

struct UniqueLockLayout {
	Ref<MutexLayout> mMutex ;
	std::unique_lock<std::mutex> mLock ;
} ;

class UniqueLockImplHolder final implement Fat<UniqueLockHolder ,UniqueLockLayout> {
public:
	void initialize (CR<Mutex> mutex) override {
		self.mMutex = mutex.borrow () ;
		assert (self.mMutex.exclusive ()) ;
		assert (self.mMutex->mType == MutexType::Unique) ;
		self.mLock = std::unique_lock<std::mutex> (self.mMutex->mBasic.ref) ;
	}

	void wait () override {
		self.mMutex->mUnique->wait (self.mLock) ;
	}

	void wait (CR<Time> time) override {
		const auto r1x = time.borrow () ;
		self.mMutex->mUnique->wait_for (self.mLock ,r1x->mTime) ;
	}

	void notify () override {
		self.mMutex->mUnique->notify_all () ;
	}

	void yield () override {
		self.mLock = std::unique_lock<std::mutex> () ;
		std::this_thread::yield () ;
		self.mLock = std::unique_lock<std::mutex> (self.mMutex->mBasic.ref) ;
	}
} ;

exports Box<UniqueLockLayout ,UniqueLockStorage> UniqueLockHolder::create () {
	return Box<UniqueLockLayout>::make () ;
}

exports VFat<UniqueLockHolder> UniqueLockHolder::hold (VR<UniqueLockLayout> that) {
	return VFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

exports CFat<UniqueLockHolder> UniqueLockHolder::hold (CR<UniqueLockLayout> that) {
	return CFat<UniqueLockHolder> (UniqueLockImplHolder () ,that) ;
}

struct ThreadLayout {
	Box<VFat<FriendExecuting>> mExecuting ;
	FLAG mUid ;
	INDEX mSlot ;
	Box<std::thread> mThread ;

public:
	implicit ThreadLayout () = default ;

	implicit ~ThreadLayout () noexcept {
		assert (mThread == NULL) ;
	}
} ;

class ThreadImplHolder final implement Fat<ThreadHolder ,ThreadLayout> {
public:
	void initialize (RR<VFat<FriendExecuting>> executing ,CR<INDEX> slot) override {
		self.mExecuting = Box<VFat<FriendExecuting>>::make (move (executing)) ;
		self.mUid = ZERO ;
		self.mSlot = slot ;
	}

	FLAG thread_uid () const override {
		return self.mUid ;
	}

	void start () override {
		auto &&rax = self ;
		self.mThread = Box<std::thread>::make ([&] () {
			rax.mUid = RuntimeProc::thread_uid () ;
			rax.mExecuting.ref->friend_execute (rax.mSlot) ;
		}) ;
	}

	void stop () override {
		if (self.mThread == NULL)
			return ;
		self.mThread->join () ;
		self.mThread = NULL ;
	}
} ;

exports AutoRef<ThreadLayout> ThreadHolder::create () {
	return AutoRef<ThreadLayout>::make () ;
}

exports VFat<ThreadHolder> ThreadHolder::hold (VR<ThreadLayout> that) {
	return VFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

exports CFat<ThreadHolder> ThreadHolder::hold (CR<ThreadLayout> that) {
	return CFat<ThreadHolder> (ThreadImplHolder () ,that) ;
}

template class External<ProcessHolder ,ProcessLayout> ;

struct ProcessLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

exports AutoRef<ProcessLayout> ProcessHolder::create () {
	return AutoRef<ProcessLayout>::make () ;
}

exports VFat<ProcessHolder> ProcessHolder::hold (VR<ProcessLayout> that) {
	return VFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::expr ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::hold (CR<ProcessLayout> that) {
	return CFat<ProcessHolder> (External<ProcessHolder ,ProcessLayout>::expr ,that) ;
}

template class External<LibraryHolder ,LibraryLayout> ;

struct LibraryLayout {
	String<STR> mFile ;
	UniqueRef<csc_device_t> mLibrary ;
	FLAG mLastError ;
} ;

exports AutoRef<LibraryLayout> LibraryHolder::create () {
	return AutoRef<LibraryLayout>::make () ;
}

exports VFat<LibraryHolder> LibraryHolder::hold (VR<LibraryLayout> that) {
	return VFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::expr ,that) ;
}

exports CFat<LibraryHolder> LibraryHolder::hold (CR<LibraryLayout> that) {
	return CFat<LibraryHolder> (External<LibraryHolder ,LibraryLayout>::expr ,that) ;
}

struct SystemLayout {
	std::locale mLocale ;
} ;

class SystemImplHolder final implement Fat<SystemHolder ,SystemLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void set_locale (CR<String<STR>> name) override {
		const auto r1x = StringProc::stra_from_strs (name) ;
		self.mLocale = std::locale (r1x) ;
	}

	void execute (CR<String<STR>> command) const override {
		const auto r1x = StringProc::stra_from_strs (command) ;
		const auto r2x = FLAG (std::system (r1x)) ;
		noop (r2x) ;
	}
} ;

exports AutoRef<SystemLayout> SystemHolder::create () {
	return AutoRef<SystemLayout>::make () ;
}

exports VFat<SystemHolder> SystemHolder::hold (VR<SystemLayout> that) {
	return VFat<SystemHolder> (SystemImplHolder () ,that) ;
}

exports CFat<SystemHolder> SystemHolder::hold (CR<SystemLayout> that) {
	return CFat<SystemHolder> (SystemImplHolder () ,that) ;
}

struct RandomNormal {
	BOOL mOdd ;
	FLT64 mNX ;
	FLT64 mNY ;
} ;

struct RandomLayout {
	FLAG mSeed ;
	Box<std::mt19937_64> mRandom ;
	RandomNormal mNormal ;
} ;

class RandomImplHolder final implement Fat<RandomHolder ,RandomLayout> {
public:
	void initialize () override {
		const auto r1x = invoke (std::random_device ()) ;
		initialize (r1x) ;
	}

	void initialize (CR<FLAG> seed) override {
		self.mSeed = seed ;
		self.mRandom.remake () ;
		self.mRandom.ref = std::mt19937_64 (seed) ;
		self.mNormal.mOdd = FALSE ;
	}

	FLAG seed () const override {
		return self.mSeed ;
	}

	QUAD random_byte () {
		return QUAD (self.mRandom.ref ()) ;
	}

	VAL32 random_value (CR<VAL32> min_ ,CR<VAL32> max_) override {
		assert (min_ <= max_) ;
		const auto r1x = VAL32 (max_) - VAL32 (min_) + 1 ;
		assert (r1x > 0) ;
		const auto r2x = VAL32 (random_byte ()) & VAL32_MAX ;
		const auto r3x = r2x % r1x + min_ ;
		return r3x ;
	}

	VAL64 random_value (CR<VAL64> min_ ,CR<VAL64> max_) override {
		assert (min_ <= max_) ;
		const auto r1x = VAL64 (max_) - VAL64 (min_) + 1 ;
		assert (r1x > 0) ;
		const auto r2x = VAL64 (random_byte ()) & VAL64_MAX ;
		const auto r3x = r2x % r1x + min_ ;
		return r3x ;
	}

	Array<INDEX> random_shuffle (CR<LENGTH> length_ ,CR<LENGTH> size_) override {
		Array<INDEX> ret = Array<INDEX>::make (range (0 ,size_)) ;
		random_shuffle (length_ ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_shuffle (CR<LENGTH> length_ ,CR<LENGTH> size_ ,VR<Array<INDEX>> result) override {
		assert (length_ >= 0) ;
		assert (length_ <= size_) ;
		assert (result.size () == size_) ;
		const auto r1x = result.size () - 1 ;
		INDEX ix = 0 ;
		while (TRUE) {
			if (ix >= length_)
				break ;
			INDEX iy = random_value (ix ,r1x) ;
			swap (result[ix] ,result[iy]) ;
			ix++ ;
		}
	}

	BitSet random_pick (CR<LENGTH> length_ ,CR<LENGTH> size_) override {
		BitSet ret = BitSet (size_) ;
		random_pick (length_ ,size_ ,ret) ;
		return move (ret) ;
	}

	void random_pick (CR<LENGTH> length_ ,CR<LENGTH> size_ ,VR<BitSet> result) override {
		assert (length_ >= 0) ;
		assert (length_ <= size_) ;
		assert (result.size () == size_) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (length_ >= size_ / 2)
				discard ;
			result.clear () ;
			const auto r1x = random_shuffle (length_ ,size_) ;
			for (auto &&i : range (0 ,length_))
				result.add (r1x[i]) ;
		}
		if ifdo (act) {
			result.fill (BYTE (0XFF)) ;
			const auto r2x = random_shuffle (size_ - length_ ,size_) ;
			for (auto &&i : range (size_ - length_ ,size_))
				result.erase (r2x[i]) ;
		}
	}

	FLT64 random_float (CR<LENGTH> precision) override {
		static const VAL64 M_EXP10[] = {1 ,10 ,100 ,1000 ,10000 ,100000 ,1000000 ,10000000 ,100000000} ;
		const auto r1x = MathProc::clamp (precision ,ZERO ,LENGTH (8)) ;
		const auto r2x = FLT64 (random_value (VAL64 (0) ,M_EXP10[r1x])) ;
		const auto r3x = r2x * MathProc::inverse (FLT64 (M_EXP10[r1x])) ;
		return r3x ;
	}

	BOOL random_draw (CR<FLT64> possibility) override {
		if (random_float (4) < possibility)
			return TRUE ;
		return FALSE ;
	}

	FLT64 random_normal () override {
		if ifdo (TRUE) {
			if (self.mNormal.mOdd)
				discard ;
			const auto r1x = random_float (4) ;
			const auto r2x = random_float (4) ;
			const auto r3x = MathProc::clamp (r1x ,FLT64_EPS ,FLT64 (1)) ;
			const auto r4x = MathProc::sqrt (FLT64 (-2) * MathProc::log (r3x)) ;
			const auto r5x = MATH_PI * 2 * r2x ;
			self.mNormal.mNX = r4x * MathProc::cos (r5x) ;
			self.mNormal.mNY = r4x * MathProc::sin (r5x) ;
			self.mNormal.mOdd = TRUE ;
			return self.mNormal.mNX ;
		}
		self.mNormal.mOdd = FALSE ;
		return self.mNormal.mNY ;
	}
} ;

exports SharedRef<RandomLayout> RandomHolder::create () {
	return SharedRef<RandomLayout>::make () ;
}

exports VFat<RandomHolder> RandomHolder::hold (VR<RandomLayout> that) {
	return VFat<RandomHolder> (RandomImplHolder () ,that) ;
}

exports CFat<RandomHolder> RandomHolder::hold (CR<RandomLayout> that) {
	return CFat<RandomHolder> (RandomImplHolder () ,that) ;
}

template class External<SingletonProcHolder ,SingletonProcLayout> ;

struct SingletonRoot {
	Mutex mMutex ;
	Set<Clazz> mClazzSet ;
} ;

struct SingletonLocal {
	QUAD mReserve1 ;
	QUAD mAddress1 ;
	QUAD mReserve2 ;
	QUAD mAddress2 ;
	QUAD mReserve3 ;
} ;

struct SingletonProcLayout {
	FLAG mUid ;
	String<STR> mName ;
	UniqueRef<csc_handle_t> mMapping ;
	SingletonLocal mLocal ;
	Ref<SingletonRoot> mRoot ;

public:
	implicit SingletonProcLayout () = default ;

	implicit ~SingletonProcLayout () noexcept {
		if (mRoot == NULL)
			return ;
		mRoot->~SingletonRoot () ;
	}
} ;

exports CR<OfThis<UniqueRef<SingletonProcLayout>>> SingletonProcHolder::expr_m () {
	return memorize ([&] () {
		OfThis<UniqueRef<SingletonProcLayout>> ret ;
		ret.mThis = UniqueRef<SingletonProcLayout>::make () ;
		SingletonProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<SingletonProcHolder> SingletonProcHolder::hold (VR<SingletonProcLayout> that) {
	return VFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::expr ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::hold (CR<SingletonProcLayout> that) {
	return CFat<SingletonProcHolder> (External<SingletonProcHolder ,SingletonProcLayout>::expr ,that) ;
}

struct GlobalNode {
	FLAG mHolder ;
	AutoRef<Pointer> mValue ;
} ;

struct GlobalTree {
	Mutex mMutex ;
	Set<Slice> mGlobalNameSet ;
	List<GlobalNode> mGlobalList ;
	BOOL mFinalize ;
} ;

class GlobalImplHolder final implement Fat<GlobalHolder ,GlobalLayout> {
public:
	void initialize () override {
		self.mThis = Ref<GlobalTree>::make () ;
		self.mThis->mMutex = NULL ;
		self.mThis->mFinalize = FALSE ;
		self.mIndex = NONE ;
	}

	void initialize (CR<Slice> name ,CR<Unknown> holder) override {
		self.mThis = Singleton<GlobalProc>::expr.mThis.share () ;
		assert (!self.mThis->mFinalize) ;
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mThis->mGlobalNameSet.map (name) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mThis->mGlobalList.insert () ;
			self.mThis->mGlobalNameSet.add (name ,ix) ;
			self.mThis->mGlobalList[ix].mHolder = inline_vptr (holder) ;
		}
		self.mIndex = ix ;
		ClazzHolder::hold (self.mClazz)->initialize (holder) ;
	}

	void startup () const override {
		auto rax = Singleton<GlobalProc>::expr.mThis.share () ;
		assume (!rax->mFinalize) ;
	}

	void shutdown () const override {
		auto rax = Singleton<GlobalProc>::expr.mThis.share () ;
		if (rax->mFinalize)
			return ;
		rax->mFinalize = TRUE ;
		rax->mGlobalNameSet.clear () ;
		rax->mGlobalList.clear () ;
	}

	BOOL exist () const override {
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mIndex ;
		auto &&rax = self.mThis->mGlobalList[ix].mValue ;
		return self.mClazz == rax.clazz () ;
	}

	AutoRef<Pointer> fetch () const override {
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mIndex ;
		auto &&rax = self.mThis->mGlobalList[ix].mValue ;
		assume (rax.exist ()) ;
		const auto r1x = Unknown (self.mThis->mGlobalList[ix].mHolder) ;
		AutoRef<Pointer> ret = AutoRef<Pointer> (r1x) ;
		const auto r2x = RFat<ReflectClone> (r1x) ;
		r2x->clone (ret.ref ,rax.ref) ;
		return move (ret) ;
	}

	void store (RR<AutoRef<Pointer>> item) const override {
		Scope<Mutex> anonymous (self.mThis->mMutex) ;
		INDEX ix = self.mIndex ;
		const auto r1x = Pin<AutoRef<Pointer>> (self.mThis->mGlobalList[ix].mValue) ;
		assume (!r1x->exist ()) ;
		r1x.ref = move (item) ;
	}
} ;

exports CR<GlobalLayout> GlobalHolder::expr_m () {
	return memorize ([&] () {
		GlobalLayout ret ;
		GlobalHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<GlobalHolder> GlobalHolder::hold (VR<GlobalLayout> that) {
	return VFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}

exports CFat<GlobalHolder> GlobalHolder::hold (CR<GlobalLayout> that) {
	return CFat<GlobalHolder> (GlobalImplHolder () ,that) ;
}
} ;