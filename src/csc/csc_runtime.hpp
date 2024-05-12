#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
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

struct TimeImplLayout ;

struct TimeLayout {
	BoxRef<TimeImplLayout> mThis ;
	TEMP<Storage<ENUM<16>>> mTime ;
} ;

struct TimeHolder implement Interface {
	imports VFat<TimeHolder> create (VREF<TimeLayout> that) ;
	imports CFat<TimeHolder> create (CREF<TimeLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) = 0 ;
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
	using TimeLayout::mTime ;

public:
	implicit Time () = default ;

	implicit Time (CREF<Time> that) {
		TimeHolder::create (thiz)->initialize (that) ;
	}

	forceinline VREF<Time> operator= (CREF<Time> that) {
		return assign (thiz ,that) ;
	}

	implicit Time (RREF<Time> that) = default ;

	forceinline VREF<Time> operator= (RREF<Time> that) = default ;

	explicit Time (CREF<LENGTH> milliseconds_ ,CREF<LENGTH> nanoseconds_) {
		TimeHolder::create (thiz)->initialize (milliseconds_ ,nanoseconds_) ;
	}

	explicit Time (CREF<TimeCalendar> calendar_) {
		TimeHolder::create (thiz)->initialize (calendar_) ;
	}

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

	Time sub (CREF<Time> that) const {
		TimeLayout ret = TimeHolder::create (thiz)->sub (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}
} ;

struct RuntimeToolPureLayout ;

struct RuntimeToolLayout {
	Ref<RuntimeToolPureLayout> mThis ;
} ;

struct RuntimeToolHolder implement Interface {
	imports VFat<RuntimeToolHolder> create (VREF<RuntimeToolLayout> that) ;
	imports CFat<RuntimeToolHolder> create (CREF<RuntimeToolLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH thread_concurrency () const = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void thread_sleep (CREF<Time> time) const = 0 ;
	virtual void thread_yield () const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual void process_exit () const = 0 ;
	virtual FLAG random_seed () const = 0 ;
	virtual String working_path () const = 0 ;
	virtual String module_path () const = 0 ;
	virtual String module_name () const = 0 ;
} ;

class RuntimeTool implement RuntimeToolLayout {
protected:
	using RuntimeToolLayout::mThis ;

public:
	imports CREF<RuntimeTool> instance () {
		return memorize ([&] () {
			RuntimeTool ret ;
			RuntimeToolHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports LENGTH thread_concurrency () {
		return RuntimeToolHolder::create (instance ())->thread_concurrency () ;
	}

	imports FLAG thread_uid () {
		return RuntimeToolHolder::create (instance ())->thread_uid () ;
	}

	imports void thread_sleep (CREF<Time> time) {
		return RuntimeToolHolder::create (instance ())->thread_sleep (time) ;
	}

	imports void thread_yield () {
		return RuntimeToolHolder::create (instance ())->thread_yield () ;
	}

	imports FLAG process_uid () {
		return RuntimeToolHolder::create (instance ())->process_uid () ;
	}

	imports void process_exit () {
		return RuntimeToolHolder::create (instance ())->process_exit () ;
	}

	imports FLAG random_seed () {
		return RuntimeToolHolder::create (instance ())->random_seed () ;
	}

	imports String working_path () {
		return RuntimeToolHolder::create (instance ())->working_path () ;
	}

	imports String module_path () {
		return RuntimeToolHolder::create (instance ())->module_path () ;
	}

	imports String module_name () {
		return RuntimeToolHolder::create (instance ())->module_name () ;
	}
} ;

struct AtomicImplLayout ;

struct AtomicLayout {
	BoxRef<AtomicImplLayout> mThis ;
	TEMP<Storage<ENUM<8>>> mAtomic ;
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
	using AtomicLayout::mAtomic ;

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

	void decrease () const {
		return AtomicHolder::create (thiz)->decrease () ;
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

struct SharedMutex_initialize implement Proxy {
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

struct SharedLockImplLayout ;

struct SharedLockLayout {
	BoxRef<SharedLockImplLayout> mThis ;
	Ref<MutexPureLayout> mMutex ;
	TEMP<Storage<ENUM<16>>> mLock ;
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
	using SharedLockLayout::mMutex ;
	using SharedLockLayout::mLock ;

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

struct UniqueLockImplLayout ;

struct UniqueLockLayout {
	BoxRef<UniqueLockImplLayout> mThis ;
	Ref<MutexPureLayout> mMutex ;
	TEMP<Storage<ENUM<16>>> mLock ;
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
	using UniqueLockLayout::mMutex ;
	using UniqueLockLayout::mLock ;

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

struct ThreadExecutable implement Interface {
	virtual void execute (CREF<INDEX> slot) = 0 ;
} ;

template <class A>
class ThreadExecutableBinder implement ThreadExecutable {
protected:
	Ref<A> mThat ;

public:
	implicit ThreadExecutableBinder () = delete ;

	explicit ThreadExecutableBinder (CREF<Ref<A>> that) {
		mThat = that.share () ;
	}

	void execute (CREF<INDEX> slot) override {
		mThat->execute (slot) ;
	}
} ;

struct ThreadPureLayout ;

struct ThreadLayout {
	Ref<ThreadPureLayout> mThis ;
} ;

struct ThreadHolder implement Interface {
	imports VFat<ThreadHolder> create (VREF<ThreadLayout> that) ;
	imports CFat<ThreadHolder> create (CREF<ThreadLayout> that) ;

	virtual void initialize (CREF<SharedRef<ThreadExecutable>> binder ,CREF<INDEX> slot) = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement ThreadLayout {
public:
	implicit Thread () = default ;

	explicit Thread (CREF<SharedRef<ThreadExecutable>> binder ,CREF<INDEX> slot) {
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

struct ProcessSnapshot {
	FLAG mUid ;
	RefBuffer<BYTE> mFeature ;
} ;

struct ProcessPureLayout ;

struct ProcessLayout {
	Ref<ProcessPureLayout> mThis ;
} ;

struct ProcessHolder implement Interface {
	imports VFat<ProcessHolder> create (VREF<ProcessLayout> that) ;
	imports CFat<ProcessHolder> create (CREF<ProcessLayout> that) ;

	virtual void initialize (CREF<FLAG> uid) = 0 ;
	virtual void initialize (CREF<ProcessSnapshot> snapshot_) = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual ProcessSnapshot snapshot () const = 0 ;
} ;

class Process implement ProcessLayout {
protected:
	using ProcessLayout::mThis ;

public:
	implicit Process () = default ;

	explicit Process (CREF<FLAG> uid) {
		ProcessHolder::create (thiz)->initialize (uid) ;
	}

	explicit Process (CREF<ProcessSnapshot> snapshot_) {
		ProcessHolder::create (thiz)->initialize (snapshot_) ;
	}

	FLAG process_uid () const {
		return ProcessHolder::create (thiz)->process_uid () ;
	}

	ProcessSnapshot snapshot () const {
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

	virtual void initialize (CREF<String> file) = 0 ;
	virtual FLAG load (CREF<String> name) = 0 ;
	virtual String error () const = 0 ;
} ;

class Module implement ModuleLayout {
protected:
	using ModuleLayout::mThis ;

public:
	implicit Module () = default ;

	explicit Module (CREF<String> file) {
		ModuleHolder::create (thiz)->initialize (file) ;
	}

	FLAG load (CREF<String> name) {
		return ModuleHolder::create (thiz)->load (name) ;
	}

	String error () const {
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
	virtual void set_locale (CREF<String> name) const = 0 ;
	virtual FLAG execute (CREF<String> command) const = 0 ;
} ;

class System implement SystemLayout {
protected:
	using SystemLayout::mThis ;

public:
	implicit System () = default ;

	void set_locale (CREF<String> name) const {
		return SystemHolder::create (thiz)->set_locale (name) ;
	}

	FLAG execute (CREF<String> command) const {
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

struct SingletonToolPureLayout ;

struct SingletonToolLayout {
	Ref<SingletonToolPureLayout> mThis ;
} ;

struct SingletonToolHolder implement Interface {
	imports VFat<SingletonToolHolder> create (VREF<SingletonToolLayout> that) ;
	imports CFat<SingletonToolHolder> create (CREF<SingletonToolLayout> that) ;

	virtual void initialize () = 0 ;
	virtual FLAG load (CREF<Clazz> clazz) const = 0 ;
	virtual void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const = 0 ;
} ;

struct SingletonTool implement SingletonToolLayout {
protected:
	using SingletonToolLayout::mThis ;

public:
	imports CREF<SingletonTool> instance () {
		return memorize ([&] () {
			SingletonTool ret ;
			SingletonToolHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports FLAG load (CREF<Clazz> clazz) {
		return SingletonToolHolder::create (instance ())->load (clazz) ;
	}

	imports void save (CREF<Clazz> clazz ,CREF<FLAG> addr) {
		return SingletonToolHolder::create (instance ())->save (clazz ,addr) ;
	}
} ;

template <class A>
struct Singleton implement Proxy {
	imports CREF<A> instance () {
		return memorize ([&] () {
			const auto r1x = Clazz (TYPE<A>::expr) ;
			auto rax = SingletonTool::load (r1x) ;
			if ifdo (TRUE) {
				if (rax == ZERO)
					discard ;
				rax = address (A::instance ()) ;
				SingletonTool::save (r1x ,rax) ;
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
	imports VFat<GlobalHolder> create (VREF<GlobalPureLayout> that) ;
	imports CFat<GlobalHolder> create (CREF<GlobalPureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void startup () = 0 ;
	virtual RefLayout borrow (CREF<Slice<STR>> name ,CREF<Unknown> holder) = 0 ;
	virtual void shutdown () = 0 ;
} ;

template <class A>
struct GlobalUnknownBinder implement Unknown {
	FLAG reflect (CREF<FLAG> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_code (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_code (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_code (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectCodeBinder<A>::expr)
			return inline_code (ReflectCodeBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_code (ReflectNameBinder<A> ()) ;
		return ZERO ;
	}
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
		Scope<Mutex> anonymous (mMutex) ;
		return GlobalHolder::create (mThis)->startup () ;
	}

	Ref<A> borrow (CREF<Slice<STR>> name) const {
		Scope<Mutex> anonymous (mMutex) ;
		RefLayout ret = GlobalHolder::create (mThis)->borrow (name ,GlobalUnknownBinder<A> ()) ;
		return move (keep[TYPE<Ref<A>>::expr] (ret)) ;
	}

	void shutdown () const {
		Scope<Mutex> anonymous (mMutex) ;
		return GlobalHolder::create (mThis)->shutdown () ;
	}
} ;

struct PathChild {
	String mFile ;
	BOOL mIsFile ;
	BOOL mIsDire ;
	BOOL mIsLink ;
} ;

struct PathLayout {
	String mPathName ;
} ;

struct PathHolder implement Interface {
	imports VFat<PathHolder> create (VREF<PathLayout> that) ;
	imports CFat<PathHolder> create (CREF<PathLayout> that) ;

	virtual void initialize (CREF<String> pathname) = 0 ;
	virtual PathLayout root () const = 0 ;
	virtual PathLayout parent () const = 0 ;
	virtual PathLayout brother () const = 0 ;
	virtual PathLayout child () const = 0 ;
	virtual Array<PathLayout> list () const = 0 ;
	virtual Array<PathLayout> list (CREF<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CREF<PathLayout> that) const = 0 ;
	virtual String pathname () const = 0 ;
	virtual String absolute () const = 0 ;
	virtual String path () const = 0 ;
	virtual String name () const = 0 ;
	virtual String extend () const = 0 ;
} ;

class Path implement PathLayout {
protected:
	using PathLayout::mPathName ;

public:
	implicit Path () = default ;

	explicit Path (CREF<String> pathname) {
		PathHolder::create (thiz)->initialize (pathname) ;
	}

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

	String pathname () const {
		return PathHolder::create (thiz)->pathname () ;
	}

	String absolute () const {
		return PathHolder::create (thiz)->absolute () ;
	}

	String path () const {
		return PathHolder::create (thiz)->path () ;
	}

	String name () const {
		return PathHolder::create (thiz)->name () ;
	}

	String extend () const {
		return PathHolder::create (thiz)->extend () ;
	}
} ;

struct FileToolPureLayout ;

struct FileToolLayout {
	Ref<FileToolPureLayout> mThis ;
} ;

struct FileToolHolder implement Interface {
	imports VFat<FileToolHolder> create (VREF<FileToolLayout> that) ;
	imports CFat<FileToolHolder> create (CREF<FileToolLayout> that) ;

	virtual void initialize () = 0 ;
	virtual RefBuffer<BYTE> load_file (CREF<String> file) const = 0 ;
	virtual void save_file (CREF<String> file ,CREF<RefBuffer<BYTE>> data) const = 0 ;
	virtual void copy_file (CREF<String> dst ,CREF<String> src) const = 0 ;
	virtual void move_file (CREF<String> dst ,CREF<String> src) const = 0 ;
	virtual void link_file (CREF<String> dst ,CREF<String> src) const = 0 ;
	virtual void erase_file (CREF<String> file) const = 0 ;
	virtual void build_dire (CREF<String> dire) const = 0 ;
	virtual void clear_dire (CREF<String> dire) const = 0 ;
	virtual void erase_dire (CREF<String> dire) const = 0 ;
} ;

class FileTool implement FileToolLayout {
protected:
	using FileToolLayout::mThis ;

public:
	imports CREF<FileTool> instance () {
		return memorize ([&] () {
			FileTool ret ;
			FileToolHolder::create (ret)->initialize () ;
			return move (ret) ;
		}) ;
	}

	imports RefBuffer<BYTE> load_file (CREF<String> file) {
		return FileToolHolder::create (instance ())->load_file (file) ;
	}

	imports void save_file (CREF<String> file ,CREF<RefBuffer<BYTE>> data) {
		return FileToolHolder::create (instance ())->save_file (file ,data) ;
	}

	imports void copy_file (CREF<String> dst ,CREF<String> src) {
		return FileToolHolder::create (instance ())->copy_file (dst ,src) ;
	}

	imports void move_file (CREF<String> dst ,CREF<String> src) {
		return FileToolHolder::create (instance ())->move_file (dst ,src) ;
	}

	imports void link_file (CREF<String> dst ,CREF<String> src) {
		return FileToolHolder::create (instance ())->link_file (dst ,src) ;
	}

	imports void erase_file (CREF<String> file) {
		return FileToolHolder::create (instance ())->erase_file (file) ;
	}

	imports void build_dire (CREF<String> dire) {
		return FileToolHolder::create (instance ())->build_dire (dire) ;
	}

	imports void clear_dire (CREF<String> dire) {
		return FileToolHolder::create (instance ())->clear_dire (dire) ;
	}

	imports void erase_dire (CREF<String> dire) {
		return FileToolHolder::create (instance ())->erase_dire (dire) ;
	}
} ;

struct StreamLoaderPureLayout ;

struct StreamLoaderLayout {
	Ref<StreamLoaderPureLayout> mThis ;
} ;

struct StreamLoaderHolder implement Interface {
	imports VFat<StreamLoaderHolder> create (VREF<StreamLoaderLayout> that) ;
	imports CFat<StreamLoaderHolder> create (CREF<StreamLoaderLayout> that) ;

	virtual void initialize (CREF<String> file) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w () = 0 ;
	virtual void open_a () = 0 ;
	virtual void close () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
	virtual void read (VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void read (VREF<RefBuffer<WORD>> item) = 0 ;
	virtual void read (VREF<RefBuffer<CHAR>> item) = 0 ;
	virtual void read (VREF<RefBuffer<QUAD>> item) = 0 ;
	virtual void write (CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<RefBuffer<WORD>> item) = 0 ;
	virtual void write (CREF<RefBuffer<CHAR>> item) = 0 ;
	virtual void write (CREF<RefBuffer<QUAD>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamLoader implement StreamLoaderLayout {
protected:
	using StreamLoaderLayout::mThis ;

public:
	implicit StreamLoader () = default ;

	explicit StreamLoader (CREF<String> file) {
		StreamLoaderHolder::create (thiz)->initialize (file) ;
	}

	void open_r () {
		return StreamLoaderHolder::create (thiz)->open_r () ;
	}

	void open_w () {
		return StreamLoaderHolder::create (thiz)->open_w () ;
	}

	void open_a () {
		return StreamLoaderHolder::create (thiz)->open_a () ;
	}

	void close () {
		return StreamLoaderHolder::create (thiz)->close () ;
	}

	LENGTH size () const {
		return StreamLoaderHolder::create (thiz)->size () ;
	}

	void resize (CREF<LENGTH> size_) {
		return StreamLoaderHolder::create (thiz)->resize (size_) ;
	}

	void read (VREF<RefBuffer<BYTE>> item) {
		return StreamLoaderHolder::create (thiz)->read (item) ;
	}

	void read (VREF<RefBuffer<WORD>> item) {
		return StreamLoaderHolder::create (thiz)->read (item) ;
	}

	void read (VREF<RefBuffer<CHAR>> item) {
		return StreamLoaderHolder::create (thiz)->read (item) ;
	}

	void read (VREF<RefBuffer<QUAD>> item) {
		return StreamLoaderHolder::create (thiz)->read (item) ;
	}

	void write (CREF<RefBuffer<BYTE>> item) {
		return StreamLoaderHolder::create (thiz)->write (item) ;
	}

	void write (CREF<RefBuffer<WORD>> item) {
		return StreamLoaderHolder::create (thiz)->write (item) ;
	}

	void write (CREF<RefBuffer<CHAR>> item) {
		return StreamLoaderHolder::create (thiz)->write (item) ;
	}

	void write (CREF<RefBuffer<QUAD>> item) {
		return StreamLoaderHolder::create (thiz)->write (item) ;
	}

	void flush () {
		return StreamLoaderHolder::create (thiz)->flush () ;
	}
} ;

struct BufferLoaderPureLayout ;

struct BufferLoaderLayout {
	Ref<BufferLoaderPureLayout> mThis ;
} ;

struct BufferLoaderHolder implement Interface {
	imports VFat<BufferLoaderHolder> create (VREF<BufferLoaderLayout> that) ;
	imports CFat<BufferLoaderHolder> create (CREF<BufferLoaderLayout> that) ;

	virtual void initialize (CREF<String> file) = 0 ;
	virtual void set_cache_size (CREF<LENGTH> size_) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w () = 0 ;
	virtual void open_a () = 0 ;
	virtual void close () = 0 ;
	virtual LENGTH size () const = 0 ;
	virtual void resize (CREF<LENGTH> size_) = 0 ;
	virtual void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void read (CREF<INDEX> index ,VREF<RefBuffer<WORD>> item) = 0 ;
	virtual void read (CREF<INDEX> index ,VREF<RefBuffer<CHAR>> item) = 0 ;
	virtual void read (CREF<INDEX> index ,VREF<RefBuffer<QUAD>> item) = 0 ;
	virtual void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CREF<INDEX> index ,CREF<RefBuffer<WORD>> item) = 0 ;
	virtual void write (CREF<INDEX> index ,CREF<RefBuffer<CHAR>> item) = 0 ;
	virtual void write (CREF<INDEX> index ,CREF<RefBuffer<QUAD>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class BufferLoader implement BufferLoaderLayout {
protected:
	using BufferLoaderLayout::mThis ;

public:
	implicit BufferLoader () = default ;

	explicit BufferLoader (CREF<String> file) {
		BufferLoaderHolder::create (thiz)->initialize (file) ;
	}

	void set_cache_size (CREF<LENGTH> size_) {
		return BufferLoaderHolder::create (thiz)->set_cache_size (size_) ;
	}

	void open_r () {
		return BufferLoaderHolder::create (thiz)->open_r () ;
	}

	void open_w () {
		return BufferLoaderHolder::create (thiz)->open_w () ;
	}

	void open_a () {
		return BufferLoaderHolder::create (thiz)->open_a () ;
	}

	void close () {
		return BufferLoaderHolder::create (thiz)->close () ;
	}

	LENGTH size () const {
		return BufferLoaderHolder::create (thiz)->size () ;
	}

	void resize (CREF<LENGTH> size_) {
		return BufferLoaderHolder::create (thiz)->resize (size_) ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) {
		return BufferLoaderHolder::create (thiz)->read (index ,item) ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<WORD>> item) {
		return BufferLoaderHolder::create (thiz)->read (index ,item) ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<CHAR>> item) {
		return BufferLoaderHolder::create (thiz)->read (index ,item) ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<QUAD>> item) {
		return BufferLoaderHolder::create (thiz)->read (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) {
		return BufferLoaderHolder::create (thiz)->write (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<WORD>> item) {
		return BufferLoaderHolder::create (thiz)->write (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<CHAR>> item) {
		return BufferLoaderHolder::create (thiz)->write (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<QUAD>> item) {
		return BufferLoaderHolder::create (thiz)->write (index ,item) ;
	}

	void flush () {
		return BufferLoaderHolder::create (thiz)->flush () ;
	}
} ;

struct ConsolePureLayout ;

struct ConsoleLayout {
	Mutex mMutex ;
	SharedRef<ConsolePureLayout> mThis ;
} ;

struct ConsoleHolder implement Interface {
	imports SharedRef<ConsolePureLayout> create () ;
	imports VFat<ConsoleHolder> create (VREF<ConsolePureLayout> that) ;
	imports CFat<ConsoleHolder> create (CREF<ConsolePureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_option (CREF<FLAG> option) = 0 ;
	virtual void print (CREF<String> msg) = 0 ;
	virtual void fatal (CREF<String> msg) = 0 ;
	virtual void error (CREF<String> msg) = 0 ;
	virtual void warn (CREF<String> msg) = 0 ;
	virtual void info (CREF<String> msg) = 0 ;
	virtual void debug (CREF<String> msg) = 0 ;
	virtual void verbose (CREF<String> msg) = 0 ;
	virtual void open (CREF<String> dire) = 0 ;
	virtual void show () = 0 ;
	virtual void hide () = 0 ;
	virtual void pause () = 0 ;
	virtual void clear () = 0 ;
} ;

class Console implement ConsoleLayout {
protected:
	using ConsoleLayout::mThis ;

public:
	imports CREF<Console> instance () {
		return memorize ([&] () {
			Console ret ;
			ret.mMutex = RecursiveMutex::make () ;
			ret.mThis = ConsoleHolder::create () ;
			return move (ret) ;
		}) ;
	}

	void set_option (CREF<FLAG> option) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->set_option (option) ;
	}

	void print (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->print (msg) ;
	}

	void fatal (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->fatal (msg) ;
	}

	void error (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->error (msg) ;
	}

	void warn (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->warn (msg) ;
	}

	void info (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->info (msg) ;
	}

	void debug (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->debug (msg) ;
	}

	void verbose (CREF<String> msg) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->verbose (msg) ;
	}

	void open (CREF<String> dire) const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->open (dire) ;
	}

	void show () const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->show () ;
	}

	void hide () const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->hide () ;
	}

	void pause () const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->pause () ;
	}

	void clear () const {
		Scope<Mutex> anonymous (mMutex) ;
		return ConsoleHolder::create (mThis)->clear () ;
	}
} ;
} ;