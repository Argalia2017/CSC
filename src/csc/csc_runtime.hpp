#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"

namespace CSC {
struct TimeCalendar {
	LENGTH mYear ;
	LENGTH mMonth ;
	LENGTH mDay ;
	LENGTH mWDay ;
	LENGTH mYDay ;
	LENGTH mHour ;
	LENGTH mMinute ;
	LENGTH mSecond ;
} ;

struct TimeImplLayout ;

struct TimeLayout {
	Box<TimeImplLayout ,Storage<ENUM<8> ,ENUM<8>>> mThis ;
} ;

struct TimeHolder implement Interface {
	imports VFat<TimeHolder> create (VREF<TimeLayout> that) ;
	imports CFat<TimeHolder> create (CREF<TimeLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<LENGTH> milliseconds_) = 0 ;
	virtual void initialize (CREF<TimeCalendar> calendar_) = 0 ;
	virtual void initialize (CREF<TimeLayout> that) = 0 ;
	virtual LENGTH megaseconds () const = 0 ;
	virtual LENGTH kiloseconds () const = 0 ;
	virtual LENGTH seconds () const = 0 ;
	virtual LENGTH milliseconds () const = 0 ;
	virtual LENGTH microseconds () const = 0 ;
	virtual LENGTH nanoseconds () const = 0 ;
	virtual TimeCalendar calendar () const = 0 ;
	virtual TimeLayout add (CREF<TimeLayout> that) const = 0 ;
	virtual TimeLayout sub (CREF<TimeLayout> that) const = 0 ;
} ;

class Time implement TimeLayout {
protected:
	using TimeLayout::mThis ;

public:
	implicit Time () = default ;

	explicit Time (CREF<LENGTH> milliseconds_) {
		TimeHolder::create (thiz)->initialize (milliseconds_) ;
	}

	explicit Time (CREF<TimeCalendar> calendar_) {
		TimeHolder::create (thiz)->initialize (calendar_) ;
	}

	implicit Time (CREF<Time> that) {
		TimeHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Time> operator= (CREF<Time> that) {
		return assign (thiz ,that) ;
	}

	implicit Time (RREF<Time> that) = default ;

	forceinline VREF<Time> operator= (RREF<Time> that) = default ;

	LENGTH megaseconds () const {
		return TimeHolder::create (thiz)->megaseconds () ;
	}

	LENGTH kiloseconds () const {
		return TimeHolder::create (thiz)->kiloseconds () ;
	}

	LENGTH seconds () const {
		return TimeHolder::create (thiz)->seconds () ;
	}

	LENGTH milliseconds () const {
		return TimeHolder::create (thiz)->milliseconds () ;
	}

	LENGTH microseconds () const {
		return TimeHolder::create (thiz)->microseconds () ;
	}

	LENGTH nanoseconds () const {
		return TimeHolder::create (thiz)->nanoseconds () ;
	}

	TimeCalendar calendar () const {
		return TimeHolder::create (thiz)->calendar () ;
	}

	Time add (CREF<Time> that) const {
		TimeLayout ret = TimeHolder::create (thiz)->add (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator+ (CREF<Time> that) const {
		return add (that) ;
	}

	forceinline void operator+= (CREF<Time> that) {
		thiz = add (that) ;
	}

	Time sub (CREF<Time> that) const {
		TimeLayout ret = TimeHolder::create (thiz)->sub (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator- (CREF<Time> that) const {
		return sub (that) ;
	}

	forceinline void operator-= (CREF<Time> that) {
		thiz = sub (that) ;
	}
} ;

inline Time CurrentTime () {
	Time ret ;
	TimeHolder::create (ret)->initialize () ;
	return move (ret) ;
}

struct RuntimeProcLayout {
	RefLayout mThis ;
} ;

struct RuntimeProcHolder implement Interface {
	imports VFat<RuntimeProcHolder> create (VREF<RuntimeProcLayout> that) ;
	imports CFat<RuntimeProcHolder> create (CREF<RuntimeProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH thread_concurrency () const = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void thread_sleep (CREF<Time> time) const = 0 ;
	virtual void thread_yield () const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual void process_exit () const = 0 ;
	virtual FLAG random_seed () const = 0 ;
	virtual String<STR> working_path () const = 0 ;
	virtual String<STR> module_path () const = 0 ;
	virtual String<STR> module_name () const = 0 ;
} ;

class RuntimeProc implement RuntimeProcLayout {
protected:
	using RuntimeProcLayout::mThis ;

public:
	imports CREF<RuntimeProc> instance () {
		return memorize ([&] () {
			RuntimeProc ret ;
			RuntimeProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports LENGTH thread_concurrency () {
		return RuntimeProcHolder::create (instance ())->thread_concurrency () ;
	}

	imports FLAG thread_uid () {
		return RuntimeProcHolder::create (instance ())->thread_uid () ;
	}

	imports void thread_sleep (CREF<Time> time) {
		return RuntimeProcHolder::create (instance ())->thread_sleep (time) ;
	}

	imports void thread_yield () {
		return RuntimeProcHolder::create (instance ())->thread_yield () ;
	}

	imports FLAG process_uid () {
		return RuntimeProcHolder::create (instance ())->process_uid () ;
	}

	imports void process_exit () {
		return RuntimeProcHolder::create (instance ())->process_exit () ;
	}

	imports FLAG random_seed () {
		return RuntimeProcHolder::create (instance ())->random_seed () ;
	}

	imports String<STR> working_path () {
		return RuntimeProcHolder::create (instance ())->working_path () ;
	}

	imports String<STR> module_path () {
		return RuntimeProcHolder::create (instance ())->module_path () ;
	}

	imports String<STR> module_name () {
		return RuntimeProcHolder::create (instance ())->module_name () ;
	}
} ;

struct AtomicImplLayout ;

struct AtomicLayout {
	Box<AtomicImplLayout ,Storage<ENUM<8> ,ENUM<8>>> mThis ;
} ;

struct AtomicHolder implement Interface {
	imports VFat<AtomicHolder> create (VREF<AtomicLayout> that) ;
	imports CFat<AtomicHolder> create (CREF<AtomicLayout> that) ;

	virtual void initialize () = 0 ;
	virtual VAL fetch () const = 0 ;
	virtual void store (CREF<VAL> item) const = 0 ;
	virtual VAL exchange (CREF<VAL> item) const = 0 ;
	virtual BOOL change (VREF<VAL> expect ,CREF<VAL> item) const = 0 ;
	virtual void replace (CREF<VAL> expect ,CREF<VAL> item) const = 0 ;
	virtual void increase () const = 0 ;
	virtual void decrease () const = 0 ;
} ;

class Atomic implement AtomicLayout {
protected:
	using AtomicLayout::mThis ;

public:
	implicit Atomic () = default ;

	implicit Atomic (CREF<typeof (NULL)>) {
		AtomicHolder::create (thiz)->initialize () ;
	}

	VAL fetch () const {
		return AtomicHolder::create (thiz)->fetch () ;
	}

	void store (CREF<VAL> item) const {
		return AtomicHolder::create (thiz)->store (item) ;
	}

	VAL exchange (CREF<VAL> item) const {
		return AtomicHolder::create (thiz)->exchange (item) ;
	}

	BOOL change (VREF<VAL> expect ,CREF<VAL> item) const {
		return AtomicHolder::create (thiz)->change (expect ,item) ;
	}

	void replace (CREF<VAL> expect ,CREF<VAL> item) const {
		return AtomicHolder::create (thiz)->replace (expect ,item) ;
	}

	void increase () const {
		return AtomicHolder::create (thiz)->increase () ;
	}

	void operator++ (int) const {
		increase () ;
	}

	void decrease () const {
		return AtomicHolder::create (thiz)->decrease () ;
	}

	void operator-- (int) const {
		decrease () ;
	}
} ;

struct MutexImplLayout ;

struct MutexLayout {
	Ref<MutexImplLayout> mThis ;
} ;

struct MutexHolder implement Interface {
	imports VFat<MutexHolder> create (VREF<MutexLayout> that) ;
	imports CFat<MutexHolder> create (CREF<MutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Ref<MutexImplLayout> borrow () const = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class Mutex implement MutexLayout {
protected:
	using MutexLayout::mThis ;

public:
	implicit Mutex () = default ;

	Ref<MutexImplLayout> borrow () const {
		return MutexHolder::create (thiz)->borrow () ;
	}

	void enter () {
		return MutexHolder::create (thiz)->enter () ;
	}

	void leave () {
		return MutexHolder::create (thiz)->leave () ;
	}
} ;

struct MakeMutexHolder implement Interface {
	imports VFat<MakeMutexHolder> create (VREF<MutexLayout> that) ;
	imports CFat<MakeMutexHolder> create (CREF<MutexLayout> that) ;

	virtual void MakeMutex_initialize () = 0 ;
	virtual void RecursiveMutex_initialize () = 0 ;
	virtual void SharedMutex_initialize () = 0 ;
	virtual void UniqueMutex_initialize () = 0 ;
} ;

inline Mutex MakeMutex () {
	Mutex ret ;
	MakeMutexHolder::create (ret)->MakeMutex_initialize () ;
	return move (ret) ;
}

inline Mutex RecursiveMutex () {
	Mutex ret ;
	MakeMutexHolder::create (ret)->RecursiveMutex_initialize () ;
	return move (ret) ;
}

inline Mutex SharedMutex () {
	Mutex ret ;
	MakeMutexHolder::create (ret)->SharedMutex_initialize () ;
	return move (ret) ;
}

inline Mutex UniqueMutex () {
	Mutex ret ;
	MakeMutexHolder::create (ret)->UniqueMutex_initialize () ;
	return move (ret) ;
}

struct SharedLockImplLayout ;

struct SharedLockLayout {
	Box<SharedLockImplLayout ,Storage<ENUM<32> ,ENUM<8>>> mThis ;
} ;

struct SharedLockHolder implement Interface {
	imports VFat<SharedLockHolder> create (VREF<SharedLockLayout> that) ;
	imports CFat<SharedLockHolder> create (CREF<SharedLockLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual BOOL busy () const = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class SharedLock implement SharedLockLayout {
protected:
	using SharedLockLayout::mThis ;

public:
	implicit SharedLock () = default ;

	explicit SharedLock (CREF<Mutex> mutex) {
		SharedLockHolder::create (thiz)->initialize (mutex) ;
	}

	BOOL busy () const {
		return SharedLockHolder::create (thiz)->busy () ;
	}

	void enter () {
		return SharedLockHolder::create (thiz)->enter () ;
	}

	void leave () {
		return SharedLockHolder::create (thiz)->leave () ;
	}
} ;

struct UniqueLockImplLayout ;

struct UniqueLockLayout {
	Box<UniqueLockImplLayout ,Storage<ENUM<32> ,ENUM<8>>> mThis ;
} ;

struct UniqueLockHolder implement Interface {
	imports VFat<UniqueLockHolder> create (VREF<UniqueLockLayout> that) ;
	imports CFat<UniqueLockHolder> create (CREF<UniqueLockLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual void wait () = 0 ;
	virtual void wait (CREF<Time> time) = 0 ;
	virtual void notify () = 0 ;
	virtual void yield () = 0 ;
} ;

class UniqueLock implement UniqueLockLayout {
protected:
	using UniqueLockLayout::mThis ;

public:
	implicit UniqueLock () = default ;

	explicit UniqueLock (CREF<Mutex> mutex) {
		UniqueLockHolder::create (thiz)->initialize (mutex) ;
	}

	void wait () {
		return UniqueLockHolder::create (thiz)->wait () ;
	}

	void wait (CREF<Time> time) {
		return UniqueLockHolder::create (thiz)->wait (time) ;
	}

	void notify () {
		return UniqueLockHolder::create (thiz)->notify () ;
	}

	void yield () {
		return UniqueLockHolder::create (thiz)->yield () ;
	}
} ;

struct ThreadFriend implement Interface {
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
} ;

template <class A>
class ThreadFriendBinder implement Fat<ThreadFriend ,A> {
public:
	imports VFat<ThreadFriend> create (VREF<A> that) {
		return VFat<ThreadFriend> (ThreadFriendBinder () ,that) ;
	}

	void friend_execute (CREF<INDEX> slot) override {
		thiz.fake.friend_execute (slot) ;
	}
} ;

struct ThreadImplLayout ;

struct ThreadLayout {
	Ref<ThreadImplLayout> mThis ;
} ;

struct ThreadHolder implement Interface {
	imports VFat<ThreadHolder> create (VREF<ThreadLayout> that) ;
	imports CFat<ThreadHolder> create (CREF<ThreadLayout> that) ;

	virtual void initialize (RREF<Ref<ThreadFriend>> executor ,CREF<INDEX> slot) = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement ThreadLayout {
protected:
	using ThreadLayout::mThis ;

public:
	implicit Thread () = default ;

	explicit Thread (RREF<Ref<ThreadFriend>> executor ,CREF<INDEX> slot) {
		ThreadHolder::create (thiz)->initialize (move (executor) ,slot) ;
	}

	FLAG thread_uid () const {
		return ThreadHolder::create (thiz)->thread_uid () ;
	}

	void start () {
		return ThreadHolder::create (thiz)->start () ;
	}

	void stop () {
		return ThreadHolder::create (thiz)->stop () ;
	}
} ;

struct ProcessImplLayout ;

struct ProcessLayout {
	Ref<ProcessImplLayout> mThis ;
} ;

struct ProcessHolder implement Interface {
	imports VFat<ProcessHolder> create (VREF<ProcessLayout> that) ;
	imports CFat<ProcessHolder> create (CREF<ProcessLayout> that) ;

	virtual void initialize (CREF<FLAG> uid) = 0 ;
	virtual void initialize (CREF<RefBuffer<BYTE>> snapshot_) = 0 ;
	virtual BOOL equal (CREF<ProcessLayout> that) const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual RefBuffer<BYTE> snapshot () const = 0 ;
} ;

class Process implement ProcessLayout {
protected:
	using ProcessLayout::mThis ;

public:
	implicit Process () = default ;

	explicit Process (CREF<FLAG> uid) {
		ProcessHolder::create (thiz)->initialize (uid) ;
	}

	explicit Process (CREF<RefBuffer<BYTE>> snapshot_) {
		ProcessHolder::create (thiz)->initialize (snapshot_) ;
	}

	BOOL equal (CREF<Process> that) const {
		return ProcessHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Process> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Process> that) const {
		return (!equal (that)) ;
	}

	FLAG process_uid () const {
		return ProcessHolder::create (thiz)->process_uid () ;
	}

	RefBuffer<BYTE> snapshot () const {
		return ProcessHolder::create (thiz)->snapshot () ;
	}
} ;

struct ModuleImplLayout ;

struct ModuleLayout {
	Ref<ModuleImplLayout> mThis ;
} ;

struct ModuleHolder implement Interface {
	imports VFat<ModuleHolder> create (VREF<ModuleLayout> that) ;
	imports CFat<ModuleHolder> create (CREF<ModuleLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual FLAG load (CREF<String<STR>> name) = 0 ;
	virtual String<STR> error () const = 0 ;
} ;

class Module implement ModuleLayout {
protected:
	using ModuleLayout::mThis ;

public:
	implicit Module () = default ;

	explicit Module (CREF<String<STR>> file) {
		ModuleHolder::create (thiz)->initialize (file) ;
	}

	FLAG load (CREF<String<STR>> name) {
		return ModuleHolder::create (thiz)->load (name) ;
	}

	String<STR> error () const {
		return ModuleHolder::create (thiz)->error () ;
	}
} ;

struct SystemImplLayout ;

struct SystemLayout {
	Ref<SystemImplLayout> mThis ;
} ;

struct SystemHolder implement Interface {
	imports VFat<SystemHolder> create (VREF<SystemLayout> that) ;
	imports CFat<SystemHolder> create (CREF<SystemLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_locale (CREF<String<STR>> name) const = 0 ;
	virtual FLAG execute (CREF<String<STR>> command) const = 0 ;
} ;

class System implement SystemLayout {
protected:
	using SystemLayout::mThis ;

public:
	implicit System () = default ;

	void set_locale (CREF<String<STR>> name) const {
		return SystemHolder::create (thiz)->set_locale (name) ;
	}

	FLAG execute (CREF<String<STR>> command) const {
		return SystemHolder::create (thiz)->execute (command) ;
	}
} ;

struct RandomImplLayout ;

struct RandomLayout {
	Ref<RandomImplLayout> mThis ;
} ;

struct RandomHolder implement Interface {
	imports VFat<RandomHolder> create (VREF<RandomLayout> that) ;
	imports CFat<RandomHolder> create (CREF<RandomLayout> that) ;

	virtual void initialize (CREF<FLAG> seed) = 0 ;
	virtual FLAG seed () const = 0 ;
	virtual INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const = 0 ;
	virtual Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const = 0 ;
	virtual void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) const = 0 ;
	virtual BitSet random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const = 0 ;
	virtual void random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<BitSet> result) const = 0 ;
	virtual BOOL random_draw (CREF<FLT64> possibility) const = 0 ;
	virtual FLT64 random_normal () const = 0 ;
} ;

class Random implement RandomLayout {
protected:
	using RandomLayout::mThis ;

public:
	implicit Random () = default ;

	explicit Random (CREF<FLAG> seed) {
		RandomHolder::create (thiz)->initialize (seed) ;
	}

	FLAG seed () const {
		return RandomHolder::create (thiz)->seed () ;
	}

	INDEX random_value (CREF<INDEX> lb ,CREF<INDEX> rb) const {
		return RandomHolder::create (thiz)->random_value (lb ,rb) ;
	}

	Array<INDEX> random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
		return RandomHolder::create (thiz)->random_shuffle (count ,size_) ;
	}

	void random_shuffle (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<Array<INDEX>> result) const {
		return RandomHolder::create (thiz)->random_shuffle (count ,size_ ,result) ;
	}

	BitSet random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_) const {
		return RandomHolder::create (thiz)->random_pick (count ,size_) ;
	}

	void random_pick (CREF<LENGTH> count ,CREF<LENGTH> size_ ,VREF<BitSet> result) const {
		return RandomHolder::create (thiz)->random_pick (count ,size_ ,result) ;
	}

	BOOL random_draw (CREF<FLT64> possibility) const {
		return RandomHolder::create (thiz)->random_draw (possibility) ;
	}

	FLT64 random_normal () const {
		return RandomHolder::create (thiz)->random_normal () ;
	}
} ;

struct SingletonProcImplLayout ;

struct SingletonProcLayout {
	SharedRef<SingletonProcImplLayout> mThis ;
} ;

struct SingletonProcHolder implement Interface {
	imports VFat<SingletonProcHolder> create (VREF<SingletonProcLayout> that) ;
	imports CFat<SingletonProcHolder> create (CREF<SingletonProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual FLAG load (CREF<Clazz> clazz) const = 0 ;
	virtual void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const = 0 ;
} ;

class SingletonProc implement SingletonProcLayout {
protected:
	using SingletonProcLayout::mThis ;

public:
	imports CREF<SingletonProc> instance () {
		return memorize ([&] () {
			SingletonProc ret ;
			SingletonProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports FLAG load (CREF<Clazz> clazz) {
		return SingletonProcHolder::create (instance ())->load (clazz) ;
	}

	imports void save (CREF<Clazz> clazz ,CREF<FLAG> addr) {
		return SingletonProcHolder::create (instance ())->save (clazz ,addr) ;
	}
} ;

template <class A>
class Singleton implement Proxy {
public:
	imports CREF<A> instance () {
		return memorize ([&] () {
			const auto r1x = Clazz (TYPE<A>::expr) ;
			auto rax = SingletonProc::load (r1x) ;
			if ifdo (TRUE) {
				if (rax != ZERO)
					discard ;
				rax = address (A::instance ()) ;
				SingletonProc::save (r1x ,rax) ;
				rax = SingletonProc::load (r1x) ;
			}
			return Ref<A>::reference (Pointer::make (rax)) ;
		}).self ;
	}
} ;

struct GlobalImplLayout ;

struct GlobalLayout {
	Mutex mMutex ;
	SharedRef<GlobalImplLayout> mThis ;
} ;

struct GlobalHolder implement Interface {
	imports VFat<GlobalHolder> create (VREF<GlobalLayout> that) ;
	imports CFat<GlobalHolder> create (CREF<GlobalLayout> that) ;

	virtual void initialize () = 0 ;
	virtual CREF<AutoRef<Pointer>> fetch (CREF<Slice> name) const = 0 ;
	virtual void store (CREF<Slice> name ,RREF<AutoRef<Pointer>> item) const = 0 ;
	virtual void abuse (CREF<Slice> name) const = 0 ;
} ;

class GlobalRoot implement Proxy {
public:
	imports CREF<GlobalLayout> instance () {
		return memorize ([&] () {
			GlobalLayout ret ;
			GlobalHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}
} ;

template <class A>
class Global implement GlobalLayout {
protected:
	using GlobalLayout::mMutex ;
	using GlobalLayout::mThis ;

public:
	imports CREF<Global> instance () {
		return keep[TYPE<Global>::expr] (GlobalRoot::instance ()) ;
	}

	A fetch (CREF<Slice> name) const {
		auto &&rax = GlobalHolder::create (thiz)->fetch (name) ;
		return rax.rebind (TYPE<A>::expr).self ;
	}

	void store (CREF<Slice> name ,RREF<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return GlobalHolder::create (thiz)->store (name ,move (rax)) ;
	}

	void abuse (CREF<Slice> name) const {
		return GlobalHolder::create (thiz)->abuse (name) ;
	}
} ;

struct PathImplLayout ;

struct PathLayout {
	Ref<PathImplLayout> mThis ;
} ;

struct PathHolder implement Interface {
	imports VFat<PathHolder> create (VREF<PathLayout> that) ;
	imports CFat<PathHolder> create (CREF<PathLayout> that) ;

	virtual void initialize (RREF<String<STR>> pathname) = 0 ;
	virtual void initialize (CREF<PathLayout> that) = 0 ;
	virtual String<STR> poll () const = 0 ;
	virtual PathLayout root () const = 0 ;
	virtual PathLayout child (CREF<Slice> name) const = 0 ;
	virtual PathLayout child (CREF<Format> name) const = 0 ;
	virtual PathLayout child (CREF<String<STR>> name) const = 0 ;
	virtual Array<PathLayout> list () const = 0 ;
	virtual Array<PathLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<PathLayout> that) const = 0 ;
	virtual BOOL is_file () const = 0 ;
	virtual BOOL is_dire () const = 0 ;
	virtual BOOL is_link () const = 0 ;
	virtual Deque<String<STR>> decouple () const = 0 ;
	virtual String<STR> path () const = 0 ;
	virtual String<STR> name () const = 0 ;
	virtual String<STR> stem () const = 0 ;
	virtual String<STR> extension () const = 0 ;
} ;

class Path implement PathLayout {
protected:
	using PathLayout::mThis ;

public:
	implicit Path () = default ;

	explicit Path (CREF<String<STR>> pathname) {
		PathHolder::create (thiz)->initialize (move (pathname)) ;
	}

	explicit Path (RREF<String<STR>> pathname) {
		PathHolder::create (thiz)->initialize (move (pathname)) ;
	}

	implicit Path (CREF<Path> that) {
		PathHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Path> operator= (CREF<Path> that) {
		return assign (thiz ,that) ;
	}

	implicit Path (RREF<Path> that) = default ;

	forceinline VREF<Path> operator= (RREF<Path> that) = default ;

	String<STR> poll () const {
		return PathHolder::create (thiz)->poll () ;
	}

	forceinline operator String<STR> () const {
		return poll () ;
	}

	PathLayout root () const {
		PathLayout ret = PathHolder::create (thiz)->root () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<Slice> name) const {
		PathLayout ret = PathHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<Format> name) const {
		PathLayout ret = PathHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<String<STR>> name) const {
		PathLayout ret = PathHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Array<Path> list () const {
		ArrayLayout ret = PathHolder::create (thiz)->list () ;
		return move (keep[TYPE<Array<Path>>::expr] (ret)) ;
	}

	Array<Path> list (CREF<LENGTH> size_) const {
		ArrayLayout ret = PathHolder::create (thiz)->list (size_) ;
		return move (keep[TYPE<Array<Path>>::expr] (ret)) ;
	}

	BOOL equal (CREF<Path> that) const {
		return PathHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Path> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Path> that) const {
		return (!equal (that)) ;
	}

	BOOL is_file () const {
		return PathHolder::create (thiz)->is_file () ;
	}

	BOOL is_dire () const {
		return PathHolder::create (thiz)->is_dire () ;
	}

	BOOL is_link () const {
		return PathHolder::create (thiz)->is_link () ;
	}

	Deque<String<STR>> decouple () const {
		return PathHolder::create (thiz)->decouple () ;
	}

	String<STR> path () const {
		return PathHolder::create (thiz)->path () ;
	}

	String<STR> name () const {
		return PathHolder::create (thiz)->name () ;
	}

	String<STR> stem () const {
		return PathHolder::create (thiz)->stem () ;
	}

	String<STR> extension () const {
		return PathHolder::create (thiz)->extension () ;
	}
} ;

struct FileProcImplLayout ;

struct FileProcLayout {
	Ref<FileProcImplLayout> mThis ;
} ;

struct FileProcHolder implement Interface {
	imports VFat<FileProcHolder> create (VREF<FileProcLayout> that) ;
	imports CFat<FileProcHolder> create (CREF<FileProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual RefBuffer<BYTE> load_file (CREF<String<STR>> file) const = 0 ;
	virtual void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> data) const = 0 ;
	virtual RefBuffer<BYTE> load_asset (CREF<String<STR>> file) const = 0 ;
	virtual void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const = 0 ;
	virtual void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const = 0 ;
	virtual void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const = 0 ;
	virtual void erase_file (CREF<String<STR>> file) const = 0 ;
	virtual void build_dire (CREF<String<STR>> dire) const = 0 ;
	virtual void clear_dire (CREF<String<STR>> dire) const = 0 ;
	virtual void erase_dire (CREF<String<STR>> dire) const = 0 ;
	virtual BOOL lock_dire (CREF<String<STR>> dire) const = 0 ;
} ;

class FileProc implement FileProcLayout {
protected:
	using FileProcLayout::mThis ;

public:
	imports CREF<FileProc> instance () {
		return memorize ([&] () {
			FileProc ret ;
			FileProcHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports RefBuffer<BYTE> load_file (CREF<String<STR>> file) {
		return FileProcHolder::create (instance ())->load_file (file) ;
	}

	imports void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> data) {
		return FileProcHolder::create (instance ())->save_file (file ,data) ;
	}

	imports RefBuffer<BYTE> load_asset (CREF<String<STR>> file) {
		return FileProcHolder::create (instance ())->load_asset (file) ;
	}

	imports void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::create (instance ())->copy_file (dst ,src) ;
	}

	imports void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::create (instance ())->move_file (dst ,src) ;
	}

	imports void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) {
		return FileProcHolder::create (instance ())->link_file (dst ,src) ;
	}

	imports void erase_file (CREF<String<STR>> file) {
		return FileProcHolder::create (instance ())->erase_file (file) ;
	}

	imports void build_dire (CREF<String<STR>> dire) {
		return FileProcHolder::create (instance ())->build_dire (dire) ;
	}

	imports void clear_dire (CREF<String<STR>> dire) {
		return FileProcHolder::create (instance ())->clear_dire (dire) ;
	}

	imports void erase_dire (CREF<String<STR>> dire) {
		return FileProcHolder::create (instance ())->erase_dire (dire) ;
	}

	imports BOOL lock_dire (CREF<String<STR>> dire) {
		return FileProcHolder::create (instance ())->lock_dire (dire) ;
	}
} ;

struct StreamFileImplLayout ;

struct StreamFileLayout {
	Ref<StreamFileImplLayout> mThis ;
} ;

struct StreamFileHolder implement Interface {
	imports VFat<StreamFileHolder> create (VREF<StreamFileLayout> that) ;
	imports CFat<StreamFileHolder> create (CREF<StreamFileLayout> that) ;

	virtual void initialize (RREF<String<STR>> file) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CREF<LENGTH> count) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual void read (VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFile implement StreamFileLayout {
protected:
	using StreamFileLayout::mThis ;

public:
	implicit StreamFile () = default ;

	explicit StreamFile (CREF<String<STR>> file) {
		StreamFileHolder::create (thiz)->initialize (move (file)) ;
	}

	explicit StreamFile (RREF<String<STR>> file) {
		StreamFileHolder::create (thiz)->initialize (move (file)) ;
	}

	void open_r () {
		return StreamFileHolder::create (thiz)->open_r () ;
	}

	void open_w (CREF<LENGTH> count) {
		return StreamFileHolder::create (thiz)->open_w (count) ;
	}

	void open_a () {
		return StreamFileHolder::create (thiz)->open_a () ;
	}

	LENGTH file_size () const {
		return StreamFileHolder::create (thiz)->file_size () ;
	}

	void read (VREF<RefBuffer<BYTE>> item) {
		return StreamFileHolder::create (thiz)->read (item) ;
	}

	void write (CREF<RefBuffer<BYTE>> item) {
		return StreamFileHolder::create (thiz)->write (item) ;
	}

	void flush () {
		return StreamFileHolder::create (thiz)->flush () ;
	}
} ;

struct StreamFileByteWriterLayout implement ByteWriter {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mStreamFileBuffer ;
} ;

struct StreamFileByteWriterHolder implement Interface {
	imports VFat<StreamFileByteWriterHolder> create (VREF<StreamFileByteWriterLayout> that) ;
	imports CFat<StreamFileByteWriterHolder> create (CREF<StreamFileByteWriterLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual void close () = 0 ;
} ;

class StreamFileByteWriter implement StreamFileByteWriterLayout {
protected:
	using StreamFileByteWriterLayout::mStreamFile ;
	using StreamFileByteWriterLayout::mStreamFileBuffer ;

public:
	implicit StreamFileByteWriter () = delete ;

	explicit StreamFileByteWriter (CREF<String<STR>> file) {
		StreamFileByteWriterHolder::create (thiz)->initialize (file) ;
	}

	implicit ~StreamFileByteWriter () noexcept {
		close () ;
	}

	implicit StreamFileByteWriter (CREF<StreamFileByteWriter> that) = default ;

	forceinline VREF<StreamFileByteWriter> operator= (CREF<StreamFileByteWriter> that) = default ;

	implicit StreamFileByteWriter (RREF<StreamFileByteWriter> that) = default ;

	forceinline VREF<StreamFileByteWriter> operator= (RREF<StreamFileByteWriter> that) = default ;

	void close () {
		StreamFileByteWriterHolder::create (thiz)->close () ;
	}
} ;

struct StreamFileTextWriterLayout implement TextWriter {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mStreamFileBuffer ;
} ;

struct StreamFileTextWriterHolder implement Interface {
	imports VFat<StreamFileTextWriterHolder> create (VREF<StreamFileTextWriterLayout> that) ;
	imports CFat<StreamFileTextWriterHolder> create (CREF<StreamFileTextWriterLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
	virtual void close () = 0 ;
} ;

class StreamFileTextWriter implement StreamFileTextWriterLayout {
protected:
	using StreamFileTextWriterLayout::mStreamFile ;
	using StreamFileTextWriterLayout::mStreamFileBuffer ;

public:
	implicit StreamFileTextWriter () = delete ;

	explicit StreamFileTextWriter (CREF<String<STR>> file) {
		StreamFileTextWriterHolder::create (thiz)->initialize (file) ;
	}

	implicit ~StreamFileTextWriter () noexcept {
		close () ;
	}

	implicit StreamFileTextWriter (CREF<StreamFileTextWriter> that) = default ;

	forceinline VREF<StreamFileTextWriter> operator= (CREF<StreamFileTextWriter> that) = default ;

	implicit StreamFileTextWriter (RREF<StreamFileTextWriter> that) = default ;

	forceinline VREF<StreamFileTextWriter> operator= (RREF<StreamFileTextWriter> that) = default ;

	void close () {
		StreamFileTextWriterHolder::create (thiz)->close () ;
	}
} ;

struct BufferFileImplLayout ;

struct BufferFileLayout {
	Ref<BufferFileImplLayout> mThis ;
} ;

struct BufferFileHolder implement Interface {
	imports VFat<BufferFileHolder> create (VREF<BufferFileLayout> that) ;
	imports CFat<BufferFileHolder> create (CREF<BufferFileLayout> that) ;

	virtual void initialize (RREF<String<STR>> file) = 0 ;
	virtual void set_block_step (CREF<LENGTH> size_) = 0 ;
	virtual void set_cache_size (CREF<LENGTH> size_) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CREF<LENGTH> count) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class BufferFile implement BufferFileLayout {
protected:
	using BufferFileLayout::mThis ;

public:
	implicit BufferFile () = default ;

	explicit BufferFile (CREF<String<STR>> file) {
		BufferFileHolder::create (thiz)->initialize (move (file)) ;
	}

	explicit BufferFile (RREF<String<STR>> file) {
		BufferFileHolder::create (thiz)->initialize (move (file)) ;
	}

	void set_block_step (CREF<LENGTH> size_) {
		return BufferFileHolder::create (thiz)->set_block_step (size_) ;
	}

	void set_cache_size (CREF<LENGTH> size_) {
		return BufferFileHolder::create (thiz)->set_cache_size (size_) ;
	}

	void open_r () {
		return BufferFileHolder::create (thiz)->open_r () ;
	}

	void open_w (CREF<LENGTH> count) {
		return BufferFileHolder::create (thiz)->open_w (count) ;
	}

	void open_a () {
		return BufferFileHolder::create (thiz)->open_a () ;
	}

	LENGTH file_size () const {
		return BufferFileHolder::create (thiz)->file_size () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) {
		return BufferFileHolder::create (thiz)->read (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) {
		return BufferFileHolder::create (thiz)->write (index ,item) ;
	}

	void flush () {
		return BufferFileHolder::create (thiz)->flush () ;
	}
} ;

struct ConsoleOption {
	enum {
		All ,
		NoPrint ,
		NoFatal ,
		NoError ,
		NoWarn ,
		NoInfo ,
		NoDebug ,
		NoTrace ,
		ETC
	} ;
} ;

struct ConsoleImplLayout ;

struct ConsoleLayout {
	Mutex mMutex ;
	SharedRef<ConsoleImplLayout> mThis ;
} ;

struct ConsoleHolder implement Interface {
	imports VFat<ConsoleHolder> create (VREF<ConsoleLayout> that) ;
	imports CFat<ConsoleHolder> create (CREF<ConsoleLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_option (CREF<Just<ConsoleOption>> option) const = 0 ;
	virtual void print (CREF<Format> msg) const = 0 ;
	virtual void fatal (CREF<Format> msg) const = 0 ;
	virtual void error (CREF<Format> msg) const = 0 ;
	virtual void warn (CREF<Format> msg) const = 0 ;
	virtual void info (CREF<Format> msg) const = 0 ;
	virtual void debug (CREF<Format> msg) const = 0 ;
	virtual void trace (CREF<Format> msg) const = 0 ;
	virtual void open (CREF<String<STR>> dire) const = 0 ;
	virtual void start () const = 0 ;
	virtual void stop () const = 0 ;
	virtual void pause () const = 0 ;
	virtual void clear () const = 0 ;
} ;

class Console implement ConsoleLayout {
protected:
	using ConsoleLayout::mMutex ;
	using ConsoleLayout::mThis ;

public:
	imports CREF<Console> instance () {
		return memorize ([&] () {
			Console ret ;
			ConsoleHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	void set_option (CREF<Just<ConsoleOption>> option) const {
		return ConsoleHolder::create (thiz)->set_option (option) ;
	}

	template <class...ARG1>
	void print (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->print (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void fatal (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->fatal (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void error (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->error (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void warn (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->warn (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void info (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->info (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void debug (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->debug (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void trace (CREF<ARG1>...params) const {
		return ConsoleHolder::create (thiz)->trace (PrintFormat (params...)) ;
	}

	void open (CREF<String<STR>> dire) const {
		return ConsoleHolder::create (thiz)->open (dire) ;
	}

	void start () const {
		return ConsoleHolder::create (thiz)->start () ;
	}

	void stop () const {
		return ConsoleHolder::create (thiz)->stop () ;
	}

	void pause () const {
		return ConsoleHolder::create (thiz)->pause () ;
	}

	void clear () const {
		return ConsoleHolder::create (thiz)->clear () ;
	}
} ;
} ;