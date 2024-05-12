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

struct TimePureLayout ;

struct TimeLayout {
	Ref<TimePureLayout> mThis ;
} ;

struct TimeHolder implement Interface {
	imports VFat<TimeHolder> create (VREF<TimeLayout> that) ;
	imports CFat<TimeHolder> create (CREF<TimeLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<LENGTH> milliseconds_) = 0 ;
	virtual void initialize (CREF<TimeCalendar> calendar_) = 0 ;
	virtual void initialize (CREF<TimeLayout> that) = 0 ;
	virtual LENGTH hours () const = 0 ;
	virtual LENGTH minutes () const = 0 ;
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

	imports Time current () {
		Time ret ;
		TimeHolder::create (ret)->initialize () ;
		return move (ret) ;
	}

	implicit Time (CREF<Time> that) {
		TimeHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Time> operator= (CREF<Time> that) {
		return assign (thiz ,that) ;
	}

	implicit Time (RREF<Time> that) = default ;

	forceinline VREF<Time> operator= (RREF<Time> that) = default ;

	LENGTH hours () const {
		return TimeHolder::create (thiz)->hours () ;
	}

	LENGTH minutes () const {
		return TimeHolder::create (thiz)->minutes () ;
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

struct RuntimeProcPureLayout ;

struct RuntimeProcLayout {
	Ref<RuntimeProcPureLayout> mThis ;
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

struct AtomicPureLayout ;

struct AtomicLayout {
	Ref<AtomicPureLayout> mThis ;
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

	implicit Atomic (CREF<typeof (FULL)>) {
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

struct MutexPureLayout ;

struct MutexLayout {
	Ref<MutexPureLayout> mThis ;
} ;

struct MutexHolder implement Interface {
	imports VFat<MutexHolder> create (VREF<MutexLayout> that) ;
	imports CFat<MutexHolder> create (CREF<MutexLayout> that) ;

	virtual Ref<MutexPureLayout> borrow () const = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
} ;

class Mutex implement MutexLayout {
protected:
	using MutexLayout::mThis ;

public:
	implicit Mutex () = default ;

	Ref<MutexPureLayout> borrow () const {
		return MutexHolder::create (thiz)->borrow () ;
	}

	void enter () const {
		return MutexHolder::create (thiz)->enter () ;
	}

	void leave () const {
		return MutexHolder::create (thiz)->leave () ;
	}
} ;

struct MakeMutexHolder implement Interface {
	imports VFat<MakeMutexHolder> create (VREF<MutexLayout> that) ;
	imports CFat<MakeMutexHolder> create (CREF<MutexLayout> that) ;

	virtual void RecursiveMutex_initialize () = 0 ;
	virtual void SharedMutex_initialize () = 0 ;
	virtual void UniqueMutex_initialize () = 0 ;
} ;

struct RecursiveMutex implement Proxy {
	imports Mutex make () {
		Mutex ret ;
		MakeMutexHolder::create (ret)->RecursiveMutex_initialize () ;
		return move (ret) ;
	}
} ;

struct SharedMutex implement Proxy {
	imports Mutex make () {
		Mutex ret ;
		MakeMutexHolder::create (ret)->SharedMutex_initialize () ;
		return move (ret) ;
	}
} ;

struct UniqueMutex implement Proxy {
	imports Mutex make () {
		Mutex ret ;
		MakeMutexHolder::create (ret)->UniqueMutex_initialize () ;
		return move (ret) ;
	}
} ;

struct SharedLockPureLayout ;

struct SharedLockLayout {
	Ref<SharedLockPureLayout> mThis ;
} ;

struct SharedLockHolder implement Interface {
	imports VFat<SharedLockHolder> create (VREF<SharedLockLayout> that) ;
	imports CFat<SharedLockHolder> create (CREF<SharedLockLayout> that) ;

	virtual void initialize (CREF<Mutex> mutex) = 0 ;
	virtual BOOL busy () const = 0 ;
	virtual void enter () const = 0 ;
	virtual void leave () const = 0 ;
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

	void enter () const {
		return SharedLockHolder::create (thiz)->enter () ;
	}

	void leave () const {
		return SharedLockHolder::create (thiz)->leave () ;
	}
} ;

struct UniqueLockPureLayout ;

struct UniqueLockLayout {
	Ref<UniqueLockPureLayout> mThis ;
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
	void friend_execute (CREF<INDEX> slot) override {
		return fake.friend_execute (slot) ;
	}
} ;

struct ThreadPureLayout ;

struct ThreadLayout {
	Ref<ThreadPureLayout> mThis ;
} ;

struct ThreadHolder implement Interface {
	imports VFat<ThreadHolder> create (VREF<ThreadLayout> that) ;
	imports CFat<ThreadHolder> create (CREF<ThreadLayout> that) ;

	virtual void initialize (RREF<VFat<ThreadFriend>> binder ,CREF<INDEX> slot) = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement ThreadLayout {
public:
	implicit Thread () = default ;

	explicit Thread (RREF<VFat<ThreadFriend>> binder ,CREF<INDEX> slot) {
		ThreadHolder::create (thiz)->initialize (move (binder) ,slot) ;
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

struct ProcessPureLayout ;

struct ProcessLayout {
	Ref<ProcessPureLayout> mThis ;
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
		return ifnot (equal (that)) ;
	}

	FLAG process_uid () const {
		return ProcessHolder::create (thiz)->process_uid () ;
	}

	RefBuffer<BYTE> snapshot () const {
		return ProcessHolder::create (thiz)->snapshot () ;
	}
} ;

struct ModulePureLayout ;

struct ModuleLayout {
	Ref<ModulePureLayout> mThis ;
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

struct SystemPureLayout ;

struct SystemLayout {
	Ref<SystemPureLayout> mThis ;
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

struct RandomPureLayout ;

struct RandomLayout {
	Ref<RandomPureLayout> mThis ;
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
} ;

struct SingletonProcPureLayout ;

struct SingletonProcLayout {
	SharedRef<SingletonProcPureLayout> mThis ;
} ;

struct SingletonProcHolder implement Interface {
	imports VFat<SingletonProcHolder> create (VREF<SingletonProcLayout> that) ;
	imports CFat<SingletonProcHolder> create (CREF<SingletonProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual FLAG load (CREF<Clazz> clazz) const = 0 ;
	virtual void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const = 0 ;
} ;

struct SingletonProc implement SingletonProcLayout {
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
struct Singleton implement Proxy {
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

struct GlobalPureLayout ;

struct GlobalLayout {
	Mutex mMutex ;
	SharedRef<GlobalPureLayout> mThis ;
} ;

struct GlobalHolder implement Interface {
	imports VFat<GlobalHolder> create (VREF<GlobalLayout> that) ;
	imports CFat<GlobalHolder> create (CREF<GlobalLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void startup () const = 0 ;
	virtual RefLayout borrow (CREF<Slice> name ,RREF<UnknownHolder> value) const = 0 ;
	virtual void shutdown () const = 0 ;
} ;

template <class A>
class Global implement GlobalLayout {
protected:
	using GlobalLayout::mMutex ;
	using GlobalLayout::mThis ;

public:
	imports CREF<Global> instance () {
		return memorize ([&] () {
			Global ret ;
			GlobalHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	void startup () const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return GlobalHolder::create (thiz)->startup () ;
	}

	Ref<A> borrow (CREF<Slice> name) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		RefLayout ret = GlobalHolder::create (thiz)->borrow (name ,AutoRef<A>::expression ()) ;
		return move (keep[TYPE<Ref<A>>::expr] (ret)) ;
	}

	void shutdown () const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return GlobalHolder::create (thiz)->shutdown () ;
	}
} ;

struct PathPureLayout ;

struct PathLayout {
	Ref<PathPureLayout> mThis ;
} ;

struct PathHolder implement Interface {
	imports VFat<PathHolder> create (VREF<PathLayout> that) ;
	imports CFat<PathHolder> create (CREF<PathLayout> that) ;

	virtual void initialize (RREF<String<STR>> pathname_) = 0 ;
	virtual void initialize (CREF<PathLayout> that) = 0 ;
	virtual PathLayout root () const = 0 ;
	virtual PathLayout parent () const = 0 ;
	virtual PathLayout brother () const = 0 ;
	virtual PathLayout child () const = 0 ;
	virtual PathLayout child (CREF<Slice> name) const = 0 ;
	virtual PathLayout child (CREF<String<STR>> name) const = 0 ;
	virtual Array<PathLayout> list () const = 0 ;
	virtual Array<PathLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<PathLayout> that) const = 0 ;
	virtual BOOL is_file () const = 0 ;
	virtual BOOL is_dire () const = 0 ;
	virtual BOOL is_link () const = 0 ;
	virtual Deque<String<STR>> decouple () const = 0 ;
	virtual String<STR> absolute () const = 0 ;
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

	explicit Path (CREF<String<STR>> pathname_) {
		PathHolder::create (thiz)->initialize (move (pathname_)) ;
	}

	explicit Path (RREF<String<STR>> pathname_) {
		PathHolder::create (thiz)->initialize (move (pathname_)) ;
	}

	implicit Path (CREF<Path> that) {
		PathHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Path> operator= (CREF<Path> that) {
		return assign (thiz ,that) ;
	}

	implicit Path (RREF<Path> that) = default ;

	forceinline VREF<Path> operator= (RREF<Path> that) = default ;

	PathLayout root () const {
		PathLayout ret = PathHolder::create (thiz)->root () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path parent () const {
		PathLayout ret = PathHolder::create (thiz)->parent () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path brother () const {
		PathLayout ret = PathHolder::create (thiz)->brother () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child () const {
		PathLayout ret = PathHolder::create (thiz)->child () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<Slice> name) const {
		PathLayout ret = PathHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CREF<String<STR>> name) const {
		PathLayout ret = PathHolder::create (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Array<Path> list () const {
		Array<PathLayout> ret = PathHolder::create (thiz)->list () ;
		return move (keep[TYPE<Array<Path>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	Array<Path> list (CREF<LENGTH> size_) const {
		Array<PathLayout> ret = PathHolder::create (thiz)->list (size_) ;
		return move (keep[TYPE<Array<Path>>::expr] (keep[TYPE<ArrayLayout>::expr] (ret))) ;
	}

	BOOL equal (CREF<Path> that) const {
		return PathHolder::create (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CREF<Path> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CREF<Path> that) const {
		return ifnot (equal (that)) ;
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

	String<STR> absolute () const {
		return PathHolder::create (thiz)->absolute () ;
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

struct FileProcPureLayout ;

struct FileProcLayout {
	Ref<FileProcPureLayout> mThis ;
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

class StreamFileImplLayout ;

struct StreamFileLayout {
	Ref<StreamFileImplLayout> mThis ;
} ;

struct StreamFileHolder implement Interface {
	imports VFat<StreamFileHolder> create (VREF<StreamFileLayout> that) ;
	imports CFat<StreamFileHolder> create (CREF<StreamFileLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
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
		StreamFileHolder::create (thiz)->initialize (file) ;
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

class BufferFileImplLayout ;

struct BufferFileLayout {
	Ref<BufferFileImplLayout> mThis ;
} ;

struct BufferFileHolder implement Interface {
	imports VFat<BufferFileHolder> create (VREF<BufferFileLayout> that) ;
	imports CFat<BufferFileHolder> create (CREF<BufferFileLayout> that) ;

	virtual void initialize (CREF<String<STR>> file) = 0 ;
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
		BufferFileHolder::create (thiz)->initialize (file) ;
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
		NoVarbose ,
		ETC
	} ;
} ;

struct ConsolePureLayout ;

struct ConsoleLayout {
	Mutex mMutex ;
	SharedRef<ConsolePureLayout> mThis ;
} ;

struct ConsoleHolder implement Interface {
	imports VFat<ConsoleHolder> create (VREF<ConsoleLayout> that) ;
	imports CFat<ConsoleHolder> create (CREF<ConsoleLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_option (CREF<FLAG> option) const = 0 ;
	virtual void print (CREF<String<STR>> msg) const = 0 ;
	virtual void fatal (CREF<String<STR>> msg) const = 0 ;
	virtual void error (CREF<String<STR>> msg) const = 0 ;
	virtual void warn (CREF<String<STR>> msg) const = 0 ;
	virtual void info (CREF<String<STR>> msg) const = 0 ;
	virtual void debug (CREF<String<STR>> msg) const = 0 ;
	virtual void verbose (CREF<String<STR>> msg) const = 0 ;
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

	void set_option (CREF<FLAG> option) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->set_option (option) ;
	}

	void print (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->print (msg) ;
	}

	void fatal (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->fatal (msg) ;
	}

	void error (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->error (msg) ;
	}

	void warn (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->warn (msg) ;
	}

	void info (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->info (msg) ;
	}

	void debug (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->debug (msg) ;
	}

	void verbose (CREF<String<STR>> msg) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->verbose (msg) ;
	}

	void open (CREF<String<STR>> dire) const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->open (dire) ;
	}

	void start () const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->start () ;
	}

	void stop () const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->stop () ;
	}

	void pause () const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->pause () ;
	}

	void clear () const {
		Scope<CPTR<Mutex>> anonymous (mMutex) ;
		return ConsoleHolder::create (thiz)->clear () ;
	}
} ;

struct StackTracePureLayout ;

struct StackTraceLayout {
	Mutex mMutex ;
	SharedRef<StackTracePureLayout> mThis ;
} ;

struct StackTraceHolder implement Interface {
	imports VFat<StackTraceHolder> create (VREF<StackTraceLayout> that) ;
	imports CFat<StackTraceHolder> create (CREF<StackTraceLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Array<FLAG> stack_trace () const = 0 ;
	virtual String<STR> function_name (CREF<FLAG> addr) const = 0 ;
} ;

class StackTrace implement StackTraceLayout {
protected:
	using StackTraceLayout::mMutex ;
	using StackTraceLayout::mThis ;

public:
	imports CREF<StackTrace> instance () {
		return memorize ([&] () {
			StackTrace ret ;
			StackTraceHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	Array<FLAG> stack_trace () const {
		return StackTraceHolder::create (thiz)->stack_trace () ;
	}

	String<STR> function_name (CREF<FLAG> addr) const {
		return StackTraceHolder::create (thiz)->function_name (addr) ;
	}
} ;
} ;