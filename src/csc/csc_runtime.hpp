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
	Length mYear ;
	Length mMonth ;
	Length mDay ;
	Length mWDay ;
	Length mYDay ;
	Length mHour ;
	Length mMinute ;
	Length mSecond ;
} ;

struct TimeLayout ;
struct TimeStorage {
	Val64 mTime ;
} ;

struct TimeHolder implement Interface {
	imports Box<TimeLayout ,TimeStorage> create () ;
	imports VFat<TimeHolder> hold (VR<TimeLayout> that) ;
	imports CFat<TimeHolder> hold (CR<TimeLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CR<Length> milliseconds_) = 0 ;
	virtual void initialize (CR<TimeCalendar> calendar_) = 0 ;
	virtual void initialize (CR<TimeLayout> that) = 0 ;
	virtual Ref<TimeLayout> borrow () const leftvalue = 0 ;
	virtual Length megaseconds () const = 0 ;
	virtual Length kiloseconds () const = 0 ;
	virtual Length seconds () const = 0 ;
	virtual Length milliseconds () const = 0 ;
	virtual Length microseconds () const = 0 ;
	virtual Length nanoseconds () const = 0 ;
	virtual TimeCalendar calendar () const = 0 ;
	virtual Super<Box<TimeLayout ,TimeStorage>> sadd (CR<TimeLayout> that) const = 0 ;
	virtual Super<Box<TimeLayout ,TimeStorage>> ssub (CR<TimeLayout> that) const = 0 ;
} ;

class Time implement Super<Box<TimeLayout ,TimeStorage>> {
public:
	implicit Time () = default ;

	explicit Time (CR<Length> milliseconds_) {
		mThis = TimeHolder::create () ;
		TimeHolder::hold (thiz)->initialize (milliseconds_) ;
	}

	explicit Time (CR<TimeCalendar> calendar_) {
		mThis = TimeHolder::create () ;
		TimeHolder::hold (thiz)->initialize (calendar_) ;
	}

	implicit Time (CR<Time> that) {
		if (!that.mThis.exist ())
			return ;
		mThis = TimeHolder::create () ;
		TimeHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Time> operator= (CR<Time> that) {
		return assign (thiz ,that) ;
	}

	implicit Time (RR<Time> that) = default ;

	forceinline VR<Time> operator= (RR<Time> that) = default ;

	Ref<TimeLayout> borrow () const leftvalue {
		return TimeHolder::hold (thiz)->borrow () ;
	}

	Length megaseconds () const {
		return TimeHolder::hold (thiz)->megaseconds () ;
	}

	Length kiloseconds () const {
		return TimeHolder::hold (thiz)->kiloseconds () ;
	}

	Length seconds () const {
		return TimeHolder::hold (thiz)->seconds () ;
	}

	Length milliseconds () const {
		return TimeHolder::hold (thiz)->milliseconds () ;
	}

	Length microseconds () const {
		return TimeHolder::hold (thiz)->microseconds () ;
	}

	Length nanoseconds () const {
		return TimeHolder::hold (thiz)->nanoseconds () ;
	}

	TimeCalendar calendar () const {
		return TimeHolder::hold (thiz)->calendar () ;
	}

	Time sadd (CR<Time> that) const {
		Super<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator+ (CR<Time> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<Time> that) {
		thiz = sadd (that) ;
	}

	Time ssub (CR<Time> that) const {
		Super<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::hold (thiz)->ssub (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator- (CR<Time> that) const {
		return ssub (that) ;
	}

	forceinline void operator-= (CR<Time> that) {
		thiz = ssub (that) ;
	}
} ;

inline Time CurrentTime () {
	Time ret ;
	ret.mThis = TimeHolder::create () ;
	TimeHolder::hold (ret)->initialize () ;
	return move (ret) ;
}

struct RuntimeProcLayout ;

struct RuntimeProcHolder implement Interface {
	imports CR<Super<Ref<RuntimeProcLayout>>> expr_m () ;
	imports VFat<RuntimeProcHolder> hold (VR<RuntimeProcLayout> that) ;
	imports CFat<RuntimeProcHolder> hold (CR<RuntimeProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Length thread_concurrency () const = 0 ;
	virtual Flag thread_uid () const = 0 ;
	virtual void thread_sleep (CR<Time> time) const = 0 ;
	virtual void thread_yield () const = 0 ;
	virtual Flag process_uid () const = 0 ;
	virtual void process_exit () const = 0 ;
	virtual String<Str> library_file (CR<csc_handle_t> addr) const = 0 ;
	virtual String<Str> library_main () const = 0 ;
} ;

class RuntimeProc implement Super<Ref<RuntimeProcLayout>> {
public:
	static CR<RuntimeProc> expr_m () {
		return keep[TYPE<RuntimeProc>::expr] (RuntimeProcHolder::expr) ;
	}

	static Length thread_concurrency () {
		return RuntimeProcHolder::hold (expr)->thread_concurrency () ;
	}

	static Flag thread_uid () {
		return RuntimeProcHolder::hold (expr)->thread_uid () ;
	}

	static void thread_sleep (CR<Time> time) {
		return RuntimeProcHolder::hold (expr)->thread_sleep (time) ;
	}

	static void thread_yield () {
		return RuntimeProcHolder::hold (expr)->thread_yield () ;
	}

	static Flag process_uid () {
		return RuntimeProcHolder::hold (expr)->process_uid () ;
	}

	static void process_exit () {
		return RuntimeProcHolder::hold (expr)->process_exit () ;
	}

	static String<Str> library_file (CR<csc_handle_t> addr) {
		return RuntimeProcHolder::hold (expr)->library_file (addr) ;
	}

	static String<Str> library_main () {
		return RuntimeProcHolder::hold (expr)->library_main () ;
	}
} ;

struct AtomicLayout ;
struct AtomicStorage {
	Val64 mAtomic ;
} ;

struct AtomicHolder implement Interface {
	imports Box<AtomicLayout ,AtomicStorage> create () ;
	imports VFat<AtomicHolder> hold (VR<AtomicLayout> that) ;
	imports CFat<AtomicHolder> hold (CR<AtomicLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Val fetch () = 0 ;
	virtual void store (CR<Val> item) = 0 ;
	virtual Val exchange (CR<Val> item) = 0 ;
	virtual Bool change (VR<Val> expect ,CR<Val> item) = 0 ;
	virtual void replace (CR<Val> expect ,CR<Val> item) = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class Atomic implement Super<Box<AtomicLayout ,AtomicStorage>> {
public:
	implicit Atomic () = default ;

	implicit Atomic (CR<typeof (NULL)>) {
		mThis = AtomicHolder::create () ;
		AtomicHolder::hold (thiz)->initialize () ;
	}

	Val fetch () const {
		return AtomicHolder::hold (thiz)->fetch () ;
	}

	forceinline operator Val () const {
		return fetch () ;
	}

	void store (CR<Val> item) const {
		return AtomicHolder::hold (thiz)->store (item) ;
	}

	Val exchange (CR<Val> item) const {
		return AtomicHolder::hold (thiz)->exchange (item) ;
	}

	Bool change (VR<Val> expect ,CR<Val> item) const {
		return AtomicHolder::hold (thiz)->change (expect ,item) ;
	}

	void replace (CR<Val> expect ,CR<Val> item) const {
		return AtomicHolder::hold (thiz)->replace (expect ,item) ;
	}

	void increase () const {
		return AtomicHolder::hold (thiz)->increase () ;
	}

	forceinline void operator++ (int) const {
		return increase () ;
	}

	void decrease () const {
		return AtomicHolder::hold (thiz)->decrease () ;
	}

	forceinline void operator-- (int) const {
		return decrease () ;
	}
} ;

struct MutexLayout ;

struct MutexHolder implement Interface {
	imports Ref<MutexLayout> create () ;
	imports VFat<MutexHolder> hold (VR<MutexLayout> that) ;
	imports CFat<MutexHolder> hold (CR<MutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Ref<MutexLayout> borrow () leftvalue = 0 ;
	virtual Bool done () = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class Mutex implement Super<Ref<MutexLayout>> {
public:
	implicit Mutex () = default ;

	implicit Mutex (CR<typeof (NULL)>) {
		mThis = MutexHolder::create () ;
		MutexHolder::hold (thiz)->initialize () ;
	}

	Ref<MutexLayout> borrow () const leftvalue {
		return MutexHolder::hold (thiz)->borrow () ;
	}

	Bool done () const {
		return MutexHolder::hold (thiz)->done () ;
	}

	void enter () {
		return MutexHolder::hold (thiz)->enter () ;
	}

	void leave () {
		return MutexHolder::hold (thiz)->leave () ;
	}
} ;

struct MakeMutexHolder implement Interface {
	imports VFat<MakeMutexHolder> hold (VR<MutexLayout> that) ;
	imports CFat<MakeMutexHolder> hold (CR<MutexLayout> that) ;

	virtual void make_OnceMutex () = 0 ;
	virtual void make_SharedMutex () = 0 ;
	virtual void make_UniqueMutex () = 0 ;
} ;

inline Mutex OnceMutex () {
	Mutex ret ;
	ret.mThis = MutexHolder::create () ;
	MakeMutexHolder::hold (ret)->make_OnceMutex () ;
	return move (ret) ;
}

inline Mutex SharedMutex () {
	Mutex ret ;
	ret.mThis = MutexHolder::create () ;
	MakeMutexHolder::hold (ret)->make_SharedMutex () ;
	return move (ret) ;
}

inline Mutex UniqueMutex () {
	Mutex ret ;
	ret.mThis = MutexHolder::create () ;
	MakeMutexHolder::hold (ret)->make_UniqueMutex () ;
	return move (ret) ;
}

struct SharedLockLayout ;
struct SharedLockStorage {
	RefLayout mMutex ;
	RefLayout mLock ;
} ;

struct SharedLockHolder implement Interface {
	imports Box<SharedLockLayout ,SharedLockStorage> create () ;
	imports VFat<SharedLockHolder> hold (VR<SharedLockLayout> that) ;
	imports CFat<SharedLockHolder> hold (CR<SharedLockLayout> that) ;

	virtual void initialize (CR<Mutex> mutex) = 0 ;
	virtual Bool busy () const = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class SharedLock implement Super<Box<SharedLockLayout ,SharedLockStorage>> {
public:
	implicit SharedLock () = default ;

	explicit SharedLock (CR<Mutex> mutex) {
		mThis = SharedLockHolder::create () ;
		SharedLockHolder::hold (thiz)->initialize (mutex) ;
	}

	Bool busy () const {
		return SharedLockHolder::hold (thiz)->busy () ;
	}

	void enter () {
		return SharedLockHolder::hold (thiz)->enter () ;
	}

	void leave () {
		return SharedLockHolder::hold (thiz)->leave () ;
	}
} ;

struct UniqueLockLayout ;
struct UniqueLockStorage {
	RefLayout mMutex ;
	RefLayout mLock ;
} ;

struct UniqueLockHolder implement Interface {
	imports Box<UniqueLockLayout ,UniqueLockStorage> create () ;
	imports VFat<UniqueLockHolder> hold (VR<UniqueLockLayout> that) ;
	imports CFat<UniqueLockHolder> hold (CR<UniqueLockLayout> that) ;

	virtual void initialize (CR<Mutex> mutex) = 0 ;
	virtual void wait () = 0 ;
	virtual void wait (CR<Time> time) = 0 ;
	virtual void notify () = 0 ;
	virtual void yield () = 0 ;
} ;

class UniqueLock implement Super<Box<UniqueLockLayout ,UniqueLockStorage>> {
public:
	implicit UniqueLock () = default ;

	explicit UniqueLock (CR<Mutex> mutex) {
		mThis = UniqueLockHolder::create () ;
		UniqueLockHolder::hold (thiz)->initialize (mutex) ;
	}

	void wait () {
		return UniqueLockHolder::hold (thiz)->wait () ;
	}

	void wait (CR<Time> time) {
		return UniqueLockHolder::hold (thiz)->wait (time) ;
	}

	void notify () {
		return UniqueLockHolder::hold (thiz)->notify () ;
	}

	void yield () {
		return UniqueLockHolder::hold (thiz)->yield () ;
	}
} ;

struct ExecutingHolder implement Interface {
	virtual void friend_execute (CR<Index> slot) = 0 ;
} ;

class Executing implement Proxy {
protected:
	VFat<ExecutingHolder> mThat ;

public:
	implicit Executing () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<ExecutingHolder ,ARG1>>>
	implicit Executing (CR<VFat<ARG1>> that) :mThat (that) {}

	void friend_execute (CR<Index> slot) const {
		return mThat->friend_execute (slot) ;
	}
} ;

struct ThreadLayout ;

struct ThreadHolder implement Interface {
	imports Ref<ThreadLayout> create () ;
	imports VFat<ThreadHolder> hold (VR<ThreadLayout> that) ;
	imports CFat<ThreadHolder> hold (CR<ThreadLayout> that) ;

	virtual void initialize (RR<Ref<Executing>> executing ,CR<Index> slot) = 0 ;
	virtual Flag thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement Super<Ref<ThreadLayout>> {
public:
	implicit Thread () = default ;

	explicit Thread (RR<Ref<Executing>> executing ,CR<Index> slot) {
		mThis = ThreadHolder::create () ;
		ThreadHolder::hold (thiz)->initialize (move (executing) ,slot) ;
	}

	Flag thread_uid () const {
		return ThreadHolder::hold (thiz)->thread_uid () ;
	}

	void start () {
		return ThreadHolder::hold (thiz)->start () ;
	}

	void stop () {
		if (!mThis.exist ())
			return ;
		return ThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct ProcessLayout ;

struct ProcessHolder implement Interface {
	imports Ref<ProcessLayout> create () ;
	imports VFat<ProcessHolder> hold (VR<ProcessLayout> that) ;
	imports CFat<ProcessHolder> hold (CR<ProcessLayout> that) ;

	virtual void initialize (CR<Flag> uid) = 0 ;
	virtual void initialize (CR<RefBuffer<Byte>> snapshot_) = 0 ;
	virtual Bool equal (CR<ProcessLayout> that) const = 0 ;
	virtual Flag process_uid () const = 0 ;
	virtual RefBuffer<Byte> snapshot () const = 0 ;
} ;

class Process implement Super<Ref<ProcessLayout>> {
public:
	implicit Process () = default ;

	explicit Process (CR<Flag> uid) {
		mThis = ProcessHolder::create () ;
		ProcessHolder::hold (thiz)->initialize (uid) ;
	}

	explicit Process (CR<RefBuffer<Byte>> snapshot_) {
		mThis = ProcessHolder::create () ;
		ProcessHolder::hold (thiz)->initialize (snapshot_) ;
	}

	Bool equal (CR<Process> that) const {
		return ProcessHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<Process> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<Process> that) const {
		return (!equal (that)) ;
	}

	Flag process_uid () const {
		return ProcessHolder::hold (thiz)->process_uid () ;
	}

	RefBuffer<Byte> snapshot () const {
		return ProcessHolder::hold (thiz)->snapshot () ;
	}
} ;

struct LibraryLayout ;

struct LibraryHolder implement Interface {
	imports Ref<LibraryLayout> create () ;
	imports VFat<LibraryHolder> hold (VR<LibraryLayout> that) ;
	imports CFat<LibraryHolder> hold (CR<LibraryLayout> that) ;

	virtual void initialize (CR<String<Str>> file) = 0 ;
	virtual String<Str> library_file () const = 0 ;
	virtual Flag load (CR<String<Str>> name) = 0 ;
	virtual String<Str> error () const = 0 ;
} ;

class Library implement Super<Ref<LibraryLayout>> {
public:
	implicit Library () = default ;

	explicit Library (CR<String<Str>> file) {
		mThis = LibraryHolder::create () ;
		LibraryHolder::hold (thiz)->initialize (file) ;
	}

	String<Str> library_file () const {
		return LibraryHolder::hold (thiz)->library_file () ;
	}

	Flag load (CR<String<Str>> name) {
		return LibraryHolder::hold (thiz)->load (name) ;
	}

	String<Str> error () const {
		return LibraryHolder::hold (thiz)->error () ;
	}
} ;

struct SystemLayout ;

struct SystemHolder implement Interface {
	imports Ref<SystemLayout> create () ;
	imports VFat<SystemHolder> hold (VR<SystemLayout> that) ;
	imports CFat<SystemHolder> hold (CR<SystemLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_locale (CR<String<Str>> name) = 0 ;
	virtual void execute (CR<String<Str>> command) const = 0 ;
} ;

class System implement Super<Ref<SystemLayout>> {
public:
	implicit System () = default ;

	implicit System (CR<typeof (NULL)>) {
		mThis = SystemHolder::create () ;
		SystemHolder::hold (thiz)->initialize () ;
	}

	void set_locale (CR<String<Str>> name) {
		return SystemHolder::hold (thiz)->set_locale (name) ;
	}

	void execute (CR<String<Str>> command) const {
		return SystemHolder::hold (thiz)->execute (command) ;
	}
} ;

struct RandomLayout ;

struct RandomHolder implement Interface {
	imports Ref<RandomLayout> create () ;
	imports VFat<RandomHolder> hold (VR<RandomLayout> that) ;
	imports CFat<RandomHolder> hold (CR<RandomLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CR<Flag> seed) = 0 ;
	virtual Flag seed () const = 0 ;
	virtual Val32 random_value (CR<Val32> min_ ,CR<Val32> max_) = 0 ;
	virtual Val64 random_value (CR<Val64> min_ ,CR<Val64> max_) = 0 ;
	virtual Array<Index> random_shuffle (CR<Length> length_ ,CR<Length> size_) = 0 ;
	virtual void random_shuffle (CR<Length> length_ ,CR<Length> size_ ,VR<Array<Index>> result) = 0 ;
	virtual BitSet random_pick (CR<Length> length_ ,CR<Length> size_) = 0 ;
	virtual void random_pick (CR<Length> length_ ,CR<Length> size_ ,VR<BitSet> result) = 0 ;
	virtual Bool random_draw (CR<Flt64> possibility) = 0 ;
	virtual Array<Flt64> random_uniform (CR<Length> count) = 0 ;
	virtual Array<Flt64> random_normal (CR<Length> count) = 0 ;
} ;

class Random implement Super<Ref<RandomLayout>> {
public:
	implicit Random () = default ;

	explicit Random (CR<Flag> seed) {
		mThis = RandomHolder::create () ;
		RandomHolder::hold (thiz)->initialize (seed) ;
	}

	Flag seed () const {
		return RandomHolder::hold (thiz)->seed () ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	ARG1 random_value (CR<ARG1> min_ ,CR<ARG1> max_) const {
		return RandomHolder::hold (thiz)->random_value (min_ ,max_) ;
	}

	Array<Index> random_shuffle (CR<Length> length_ ,CR<Length> size_) const {
		return RandomHolder::hold (thiz)->random_shuffle (length_ ,size_) ;
	}

	void random_shuffle (CR<Length> length_ ,CR<Length> size_ ,VR<Array<Index>> result) const {
		return RandomHolder::hold (thiz)->random_shuffle (length_ ,size_ ,result) ;
	}

	BitSet random_pick (CR<Length> length_ ,CR<Length> size_) const {
		return RandomHolder::hold (thiz)->random_pick (length_ ,size_) ;
	}

	void random_pick (CR<Length> length_ ,CR<Length> size_ ,VR<BitSet> result) const {
		return RandomHolder::hold (thiz)->random_pick (length_ ,size_ ,result) ;
	}

	Bool random_draw (CR<Flt64> possibility) const {
		return RandomHolder::hold (thiz)->random_draw (possibility) ;
	}

	Array<Flt64> random_uniform (CR<Length> count) const {
		return RandomHolder::hold (thiz)->random_uniform (count) ;
	}

	Array<Flt64> random_normal (CR<Length> count) const {
		return RandomHolder::hold (thiz)->random_normal (count) ;
	}
} ;

inline Random CurrentRandom () {
	Random ret ;
	ret.mThis = RandomHolder::create () ;
	RandomHolder::hold (ret)->initialize () ;
	return move (ret) ;
}

struct SingletonProcLayout ;

struct SingletonProcHolder implement Interface {
	imports CR<Super<Ref<SingletonProcLayout>>> expr_m () ;
	imports VFat<SingletonProcHolder> hold (VR<SingletonProcLayout> that) ;
	imports CFat<SingletonProcHolder> hold (CR<SingletonProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Quad abi_reserve () const = 0 ;
	virtual Quad ctx_reserve () const = 0 ;
	virtual Flag load (CR<Clazz> clazz) const = 0 ;
	virtual void save (CR<Clazz> clazz ,CR<Flag> layout) const = 0 ;
} ;

class SingletonProc implement Super<Ref<SingletonProcLayout>> {
public:
	static CR<SingletonProc> expr_m () {
		return keep[TYPE<SingletonProc>::expr] (SingletonProcHolder::expr) ;
	}

	static Quad abi_reserve () {
		return SingletonProcHolder::hold (expr)->abi_reserve () ;
	}

	static Quad ctx_reserve () {
		return SingletonProcHolder::hold (expr)->ctx_reserve () ;
	}

	static Flag load (CR<Clazz> clazz) {
		return SingletonProcHolder::hold (expr)->load (clazz) ;
	}

	static void save (CR<Clazz> clazz ,CR<Flag> layout) {
		return SingletonProcHolder::hold (expr)->save (clazz ,layout) ;
	}
} ;

template <class A>
class Singleton implement Proxy {
public:
	static CR<A> expr_m () {
		return memorize ([&] () {
			const auto r1x = Clazz (TYPE<A>::expr) ;
			auto rax = SingletonProc::load (r1x) ;
			if ifdo (TRUE) {
				if (rax != ZERO)
					discard ;
				rax = address (A::expr) ;
				SingletonProc::save (r1x ,rax) ;
				rax = SingletonProc::load (r1x) ;
			}
			auto &&rbx = keep[TYPE<A>::expr] (Pointer::make (rax)) ;
			return Ref<A>::reference (rbx) ;
		}).ref ;
	}
} ;

struct GlobalTree ;

struct GlobalLayout {
	SharedRef<GlobalTree> mThis ;
	Index mIndex ;
	Clazz mClazz ;
} ;

struct GlobalHolder implement Interface {
	imports CR<GlobalLayout> expr_m () ;
	imports VFat<GlobalHolder> hold (VR<GlobalLayout> that) ;
	imports CFat<GlobalHolder> hold (CR<GlobalLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CR<Slice> name ,CR<Unknown> holder) = 0 ;
	virtual void startup () const = 0 ;
	virtual void shutdown () const = 0 ;
	virtual Bool exist () const = 0 ;
	virtual AutoRef<Pointer> fetch () const = 0 ;
	virtual void store (RR<AutoRef<Pointer>> item) const = 0 ;
} ;

class GlobalProc implement GlobalLayout {
public:
	static CR<GlobalProc> expr_m () {
		return keep[TYPE<GlobalProc>::expr] (GlobalHolder::expr) ;
	}

	static void startup () {
		return GlobalHolder::hold (expr)->startup () ;
	}

	static void shutdown () {
		return GlobalHolder::hold (expr)->shutdown () ;
	}
} ;

template <class A>
class GlobalUnknownBinder final implement Fat<UnknownHolder ,void> {
public:
	Flag reflect (CR<Flag> uuid) const override {
		if (uuid == ReflectSizeBinder<A>::expr)
			return inline_vptr (ReflectSizeBinder<A> ()) ;
		if (uuid == ReflectCreateBinder<A>::expr)
			return inline_vptr (ReflectCreateBinder<A> ()) ;
		if (uuid == ReflectDestroyBinder<A>::expr)
			return inline_vptr (ReflectDestroyBinder<A> ()) ;
		if (uuid == ReflectGuidBinder<A>::expr)
			return inline_vptr (ReflectGuidBinder<A> ()) ;
		if (uuid == ReflectNameBinder<A>::expr)
			return inline_vptr (ReflectNameBinder<A> ()) ;
		if (uuid == ReflectCompileBinder<A>::expr)
			return inline_vptr (ReflectCompileBinder<A> ()) ;
		if (uuid == ReflectCloneBinder<A>::expr)
			return inline_vptr (ReflectCloneBinder<A> ()) ;
		return ZERO ;
	}
} ;

template <class A>
class Global implement GlobalLayout {
protected:
	using GlobalLayout::mThis ;
	using GlobalLayout::mIndex ;
	using GlobalLayout::mClazz ;

public:
	implicit Global () = default ;

	explicit Global (CR<Slice> name) {
		GlobalHolder::hold (thiz)->initialize (name ,GlobalUnknownBinder<A> ()) ;
	}

	Bool exist () const {
		return GlobalHolder::hold (thiz)->exist () ;
	}

	A fetch () const {
		auto rax = GlobalHolder::hold (thiz)->fetch () ;
		return move (rax.rebind (TYPE<A>::expr).ref) ;
	}

	forceinline operator A () const {
		return fetch () ;
	}

	void store (CR<A> item) const {
		store (move (item)) ;
	}

	void store (RR<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return GlobalHolder::hold (thiz)->store (move (rax)) ;
	}
} ;
} ;