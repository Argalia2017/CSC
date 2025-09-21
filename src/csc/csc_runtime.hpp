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

struct TimeLayout ;
struct TimeStorage implement Storage<ENUM<8> ,ENUM<8>> {} ;

struct TimeHolder implement Interface {
	imports Box<TimeLayout ,TimeStorage> create () ;
	imports VFat<TimeHolder> hold (VR<TimeLayout> that) ;
	imports CFat<TimeHolder> hold (CR<TimeLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CR<LENGTH> milliseconds_) = 0 ;
	virtual void initialize (CR<TimeCalendar> calendar_) = 0 ;
	virtual void initialize (CR<TimeLayout> that) = 0 ;
	virtual Ref<TimeLayout> borrow () const leftvalue = 0 ;
	virtual LENGTH megaseconds () const = 0 ;
	virtual LENGTH kiloseconds () const = 0 ;
	virtual LENGTH seconds () const = 0 ;
	virtual LENGTH milliseconds () const = 0 ;
	virtual LENGTH microseconds () const = 0 ;
	virtual LENGTH nanoseconds () const = 0 ;
	virtual TimeCalendar calendar () const = 0 ;
	virtual OfThis<Box<TimeLayout ,TimeStorage>> sadd (CR<TimeLayout> that) const = 0 ;
	virtual OfThis<Box<TimeLayout ,TimeStorage>> ssub (CR<TimeLayout> that) const = 0 ;
} ;

class Time implement OfThis<Box<TimeLayout ,TimeStorage>> {
public:
	implicit Time () = default ;

	explicit Time (CR<LENGTH> milliseconds_) {
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

	LENGTH megaseconds () const {
		return TimeHolder::hold (thiz)->megaseconds () ;
	}

	LENGTH kiloseconds () const {
		return TimeHolder::hold (thiz)->kiloseconds () ;
	}

	LENGTH seconds () const {
		return TimeHolder::hold (thiz)->seconds () ;
	}

	LENGTH milliseconds () const {
		return TimeHolder::hold (thiz)->milliseconds () ;
	}

	LENGTH microseconds () const {
		return TimeHolder::hold (thiz)->microseconds () ;
	}

	LENGTH nanoseconds () const {
		return TimeHolder::hold (thiz)->nanoseconds () ;
	}

	TimeCalendar calendar () const {
		return TimeHolder::hold (thiz)->calendar () ;
	}

	Time sadd (CR<Time> that) const {
		OfThis<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::hold (thiz)->sadd (that) ;
		return move (keep[TYPE<Time>::expr] (ret)) ;
	}

	Time operator+ (CR<Time> that) const {
		return sadd (that) ;
	}

	forceinline void operator+= (CR<Time> that) {
		thiz = sadd (that) ;
	}

	Time ssub (CR<Time> that) const {
		OfThis<Box<TimeLayout ,TimeStorage>> ret = TimeHolder::hold (thiz)->ssub (that) ;
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
	imports CR<OfThis<UniqueRef<RuntimeProcLayout>>> expr_m () ;
	imports VFat<RuntimeProcHolder> hold (VR<RuntimeProcLayout> that) ;
	imports CFat<RuntimeProcHolder> hold (CR<RuntimeProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual LENGTH thread_concurrency () const = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void thread_sleep (CR<Time> time) const = 0 ;
	virtual void thread_yield () const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual void process_exit () const = 0 ;
	virtual String<STR> library_file () const = 0 ;
	virtual String<STR> library_main () const = 0 ;
} ;

class RuntimeProc implement OfThis<UniqueRef<RuntimeProcLayout>> {
public:
	static CR<RuntimeProc> expr_m () {
		return keep[TYPE<RuntimeProc>::expr] (RuntimeProcHolder::expr) ;
	}

	static LENGTH thread_concurrency () {
		return RuntimeProcHolder::hold (expr)->thread_concurrency () ;
	}

	static FLAG thread_uid () {
		return RuntimeProcHolder::hold (expr)->thread_uid () ;
	}

	static void thread_sleep (CR<Time> time) {
		return RuntimeProcHolder::hold (expr)->thread_sleep (time) ;
	}

	static void thread_yield () {
		return RuntimeProcHolder::hold (expr)->thread_yield () ;
	}

	static FLAG process_uid () {
		return RuntimeProcHolder::hold (expr)->process_uid () ;
	}

	static void process_exit () {
		return RuntimeProcHolder::hold (expr)->process_exit () ;
	}

	static String<STR> library_file () {
		return RuntimeProcHolder::hold (expr)->library_file () ;
	}

	static String<STR> library_main () {
		return RuntimeProcHolder::hold (expr)->library_main () ;
	}
} ;

struct AtomicLayout ;
struct AtomicStorage implement Storage<ENUM_MUL<SIZE_OF<VAL> ,RANK1> ,ENUM<8>> {} ;

struct AtomicHolder implement Interface {
	imports Box<AtomicLayout ,AtomicStorage> create () ;
	imports VFat<AtomicHolder> hold (VR<AtomicLayout> that) ;
	imports CFat<AtomicHolder> hold (CR<AtomicLayout> that) ;

	virtual void initialize () = 0 ;
	virtual VAL fetch () = 0 ;
	virtual void store (CR<VAL> item) = 0 ;
	virtual VAL exchange (CR<VAL> item) = 0 ;
	virtual BOOL change (VR<VAL> expect ,CR<VAL> item) = 0 ;
	virtual void replace (CR<VAL> expect ,CR<VAL> item) = 0 ;
	virtual void increase () = 0 ;
	virtual void decrease () = 0 ;
} ;

class Atomic implement OfThis<Box<AtomicLayout ,AtomicStorage>> {
public:
	implicit Atomic () = default ;

	implicit Atomic (CR<typeof (NULL)>) {
		mThis = AtomicHolder::create () ;
		AtomicHolder::hold (thiz)->initialize () ;
	}

	VAL fetch () const {
		return AtomicHolder::hold (thiz)->fetch () ;
	}

	forceinline operator VAL () const {
		return fetch () ;
	}

	void store (CR<VAL> item) const {
		return AtomicHolder::hold (thiz)->store (item) ;
	}

	VAL exchange (CR<VAL> item) const {
		return AtomicHolder::hold (thiz)->exchange (item) ;
	}

	BOOL change (VR<VAL> expect ,CR<VAL> item) const {
		return AtomicHolder::hold (thiz)->change (expect ,item) ;
	}

	void replace (CR<VAL> expect ,CR<VAL> item) const {
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
	imports SharedRef<MutexLayout> create () ;
	imports VFat<MutexHolder> hold (VR<MutexLayout> that) ;
	imports CFat<MutexHolder> hold (CR<MutexLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Ref<MutexLayout> borrow () leftvalue = 0 ;
	virtual BOOL done () = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class Mutex implement OfThis<SharedRef<MutexLayout>> {
public:
	implicit Mutex () = default ;

	implicit Mutex (CR<typeof (NULL)>) {
		mThis = MutexHolder::create () ;
		MutexHolder::hold (thiz)->initialize () ;
	}

	Ref<MutexLayout> borrow () const leftvalue {
		return MutexHolder::hold (thiz)->borrow () ;
	}

	BOOL done () const {
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
struct SharedLockStorage implement Storage<ENUM_MUL<SIZE_OF<VAL> ,RANK4> ,ENUM<8>> {} ;

struct SharedLockHolder implement Interface {
	imports Box<SharedLockLayout ,SharedLockStorage> create () ;
	imports VFat<SharedLockHolder> hold (VR<SharedLockLayout> that) ;
	imports CFat<SharedLockHolder> hold (CR<SharedLockLayout> that) ;

	virtual void initialize (CR<Mutex> mutex) = 0 ;
	virtual BOOL busy () const = 0 ;
	virtual void enter () = 0 ;
	virtual void leave () = 0 ;
} ;

class SharedLock implement OfThis<Box<SharedLockLayout ,SharedLockStorage>> {
public:
	implicit SharedLock () = default ;

	explicit SharedLock (CR<Mutex> mutex) {
		mThis = SharedLockHolder::create () ;
		SharedLockHolder::hold (thiz)->initialize (mutex) ;
	}

	BOOL busy () const {
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
struct UniqueLockStorage implement Storage<ENUM_MUL<SIZE_OF<VAL> ,RANK4> ,ENUM<8>> {} ;

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

class UniqueLock implement OfThis<Box<UniqueLockLayout ,UniqueLockStorage>> {
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

struct FriendExecuting implement Interface {
	virtual void friend_execute (CR<INDEX> slot) = 0 ;
} ;

template <class A>
class FriendExecutingBinder final implement Fat<FriendExecuting ,A> {
public:
	static VFat<FriendExecuting> hold (VR<A> that) {
		return VFat<FriendExecuting> (FriendExecutingBinder () ,that) ;
	}

	void friend_execute (CR<INDEX> slot) override {
		thiz.self.friend_execute (slot) ;
	}
} ;

struct ThreadLayout ;

struct ThreadHolder implement Interface {
	imports AutoRef<ThreadLayout> create () ;
	imports VFat<ThreadHolder> hold (VR<ThreadLayout> that) ;
	imports CFat<ThreadHolder> hold (CR<ThreadLayout> that) ;

	virtual void initialize (RR<VFat<FriendExecuting>> executing ,CR<INDEX> slot) = 0 ;
	virtual FLAG thread_uid () const = 0 ;
	virtual void start () = 0 ;
	virtual void stop () = 0 ;
} ;

class Thread implement OfThis<AutoRef<ThreadLayout>> {
public:
	implicit Thread () = default ;

	explicit Thread (RR<VFat<FriendExecuting>> executing ,CR<INDEX> slot) {
		mThis = ThreadHolder::create () ;
		ThreadHolder::hold (thiz)->initialize (move (executing) ,slot) ;
	}

	FLAG thread_uid () const {
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
	imports AutoRef<ProcessLayout> create () ;
	imports VFat<ProcessHolder> hold (VR<ProcessLayout> that) ;
	imports CFat<ProcessHolder> hold (CR<ProcessLayout> that) ;

	virtual void initialize (CR<FLAG> uid) = 0 ;
	virtual void initialize (CR<RefBuffer<BYTE>> snapshot_) = 0 ;
	virtual BOOL equal (CR<ProcessLayout> that) const = 0 ;
	virtual FLAG process_uid () const = 0 ;
	virtual RefBuffer<BYTE> snapshot () const = 0 ;
} ;

class Process implement OfThis<AutoRef<ProcessLayout>> {
public:
	implicit Process () = default ;

	explicit Process (CR<FLAG> uid) {
		mThis = ProcessHolder::create () ;
		ProcessHolder::hold (thiz)->initialize (uid) ;
	}

	explicit Process (CR<RefBuffer<BYTE>> snapshot_) {
		mThis = ProcessHolder::create () ;
		ProcessHolder::hold (thiz)->initialize (snapshot_) ;
	}

	BOOL equal (CR<Process> that) const {
		return ProcessHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Process> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Process> that) const {
		return (!equal (that)) ;
	}

	FLAG process_uid () const {
		return ProcessHolder::hold (thiz)->process_uid () ;
	}

	RefBuffer<BYTE> snapshot () const {
		return ProcessHolder::hold (thiz)->snapshot () ;
	}
} ;

struct LibraryLayout ;

struct LibraryHolder implement Interface {
	imports AutoRef<LibraryLayout> create () ;
	imports VFat<LibraryHolder> hold (VR<LibraryLayout> that) ;
	imports CFat<LibraryHolder> hold (CR<LibraryLayout> that) ;

	virtual void initialize (CR<String<STR>> file) = 0 ;
	virtual String<STR> library_file () const = 0 ;
	virtual FLAG load (CR<String<STR>> name) = 0 ;
	virtual String<STR> error () const = 0 ;
} ;

class Library implement OfThis<AutoRef<LibraryLayout>> {
public:
	implicit Library () = default ;

	explicit Library (CR<String<STR>> file) {
		mThis = LibraryHolder::create () ;
		LibraryHolder::hold (thiz)->initialize (file) ;
	}

	String<STR> library_file () const {
		return LibraryHolder::hold (thiz)->library_file () ;
	}

	FLAG load (CR<String<STR>> name) {
		return LibraryHolder::hold (thiz)->load (name) ;
	}

	String<STR> error () const {
		return LibraryHolder::hold (thiz)->error () ;
	}
} ;

struct SystemLayout ;

struct SystemHolder implement Interface {
	imports AutoRef<SystemLayout> create () ;
	imports VFat<SystemHolder> hold (VR<SystemLayout> that) ;
	imports CFat<SystemHolder> hold (CR<SystemLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_locale (CR<String<STR>> name) = 0 ;
	virtual void execute (CR<String<STR>> command) const = 0 ;
} ;

class System implement OfThis<AutoRef<SystemLayout>> {
public:
	implicit System () = default ;

	implicit System (CR<typeof (NULL)>) {
		mThis = SystemHolder::create () ;
		SystemHolder::hold (thiz)->initialize () ;
	}

	void set_locale (CR<String<STR>> name) {
		return SystemHolder::hold (thiz)->set_locale (name) ;
	}

	void execute (CR<String<STR>> command) const {
		return SystemHolder::hold (thiz)->execute (command) ;
	}
} ;

struct RandomLayout ;

struct RandomHolder implement Interface {
	imports SharedRef<RandomLayout> create () ;
	imports VFat<RandomHolder> hold (VR<RandomLayout> that) ;
	imports CFat<RandomHolder> hold (CR<RandomLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void initialize (CR<FLAG> seed) = 0 ;
	virtual FLAG seed () const = 0 ;
	virtual VAL32 random_value (CR<VAL32> min_ ,CR<VAL32> max_) = 0 ;
	virtual VAL64 random_value (CR<VAL64> min_ ,CR<VAL64> max_) = 0 ;
	virtual Array<INDEX> random_shuffle (CR<LENGTH> length_ ,CR<LENGTH> size_) = 0 ;
	virtual void random_shuffle (CR<LENGTH> length_ ,CR<LENGTH> size_ ,VR<Array<INDEX>> result) = 0 ;
	virtual BitSet random_pick (CR<LENGTH> length_ ,CR<LENGTH> size_) = 0 ;
	virtual void random_pick (CR<LENGTH> length_ ,CR<LENGTH> size_ ,VR<BitSet> result) = 0 ;
	virtual FLT64 random_float (CR<LENGTH> precision) = 0 ;
	virtual BOOL random_draw (CR<FLT64> possibility) = 0 ;
	virtual FLT64 random_normal () = 0 ;
} ;

class Random implement OfThis<SharedRef<RandomLayout>> {
public:
	implicit Random () = default ;

	explicit Random (CR<FLAG> seed) {
		mThis = RandomHolder::create () ;
		RandomHolder::hold (thiz)->initialize (seed) ;
	}

	FLAG seed () const {
		return RandomHolder::hold (thiz)->seed () ;
	}

	template <class ARG1 ,class = REQUIRE<IS_VALUE<ARG1>>>
	ARG1 random_value (CR<ARG1> min_ ,CR<ARG1> max_) const {
		return RandomHolder::hold (thiz)->random_value (min_ ,max_) ;
	}

	Array<INDEX> random_shuffle (CR<LENGTH> length_ ,CR<LENGTH> size_) const {
		return RandomHolder::hold (thiz)->random_shuffle (length_ ,size_) ;
	}

	void random_shuffle (CR<LENGTH> length_ ,CR<LENGTH> size_ ,VR<Array<INDEX>> result) const {
		return RandomHolder::hold (thiz)->random_shuffle (length_ ,size_ ,result) ;
	}

	BitSet random_pick (CR<LENGTH> length_ ,CR<LENGTH> size_) const {
		return RandomHolder::hold (thiz)->random_pick (length_ ,size_) ;
	}

	void random_pick (CR<LENGTH> length_ ,CR<LENGTH> size_ ,VR<BitSet> result) const {
		return RandomHolder::hold (thiz)->random_pick (length_ ,size_ ,result) ;
	}

	FLT64 random_float (CR<LENGTH> precision) const {
		return RandomHolder::hold (thiz)->random_float (precision) ;
	}

	BOOL random_draw (CR<FLT64> possibility) const {
		return RandomHolder::hold (thiz)->random_draw (possibility) ;
	}

	FLT64 random_normal () const {
		return RandomHolder::hold (thiz)->random_normal () ;
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
	imports CR<OfThis<UniqueRef<SingletonProcLayout>>> expr_m () ;
	imports VFat<SingletonProcHolder> hold (VR<SingletonProcLayout> that) ;
	imports CFat<SingletonProcHolder> hold (CR<SingletonProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual QUAD abi_reserve () const = 0 ;
	virtual QUAD ctx_reserve () const = 0 ;
	virtual FLAG load (CR<Clazz> clazz) const = 0 ;
	virtual void save (CR<Clazz> clazz ,CR<FLAG> layout) const = 0 ;
} ;

class SingletonProc implement OfThis<UniqueRef<SingletonProcLayout>> {
public:
	static CR<SingletonProc> expr_m () {
		return keep[TYPE<SingletonProc>::expr] (SingletonProcHolder::expr) ;
	}

	static QUAD abi_reserve () {
		return SingletonProcHolder::hold (expr)->abi_reserve () ;
	}

	static QUAD ctx_reserve () {
		return SingletonProcHolder::hold (expr)->ctx_reserve () ;
	}

	static FLAG load (CR<Clazz> clazz) {
		return SingletonProcHolder::hold (expr)->load (clazz) ;
	}

	static void save (CR<Clazz> clazz ,CR<FLAG> layout) {
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
	Ref<GlobalTree> mThis ;
	INDEX mIndex ;
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
	virtual BOOL exist () const = 0 ;
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
class GlobalUnknownBinder final implement Fat<FriendUnknown ,Proxy> {
public:
	FLAG reflect (CR<FLAG> uuid) const override {
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

	BOOL exist () const {
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