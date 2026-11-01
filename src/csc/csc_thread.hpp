#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
struct CoroutineHolder implement Interface {
	virtual void awake () = 0 ;
	virtual Bool tick (CR<Flt64> delta_sec) = 0 ;
	virtual void idle_tick (CR<Flt64> delta_sec) = 0 ;
	virtual Bool alive () = 0 ;
} ;

class Coroutine implement Proxy {
protected:
	VFat<CoroutineHolder> mThat ;

public:
	implicit Coroutine () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<CoroutineHolder ,ARG1>>>
	implicit Coroutine (CR<VFat<ARG1>> that) :mThat (that) {}

	void awake () const {
		return mThat->awake () ;
	}

	Bool tick (CR<Flt64> delta_sec) const {
		return mThat->tick (delta_sec) ;
	}

	void idle_tick (CR<Flt64> delta_sec) const {
		return mThat->idle_tick (delta_sec) ;
	}

	Bool alive () const {
		return mThat->alive () ;
	}
} ;

struct WorkThreadLayout ;

struct WorkThreadHolder implement Interface {
	imports Ref<WorkThreadLayout> create () ;
	imports VFat<WorkThreadHolder> hold (VR<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> hold (CR<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CR<Length> size_) = 0 ;
	virtual void set_queue_size (CR<Length> size_) = 0 ;
	virtual void start (CR<Function<CR<Index>>> func) = 0 ;
	virtual void friend_proc (CR<Index> slot) = 0 ;
	virtual void post (CR<Index> begin_ ,CR<Index> end_) = 0 ;
	virtual void join () = 0 ;
	virtual Bool join (CR<Time> interval) = 0 ;
	virtual void stop () = 0 ;
} ;

class WorkThread implement Super<Ref<WorkThreadLayout>> {
public:
	implicit WorkThread () = default ;

	implicit WorkThread (CR<typeof (NULL)>) {
		mThis = WorkThreadHolder::create () ;
		WorkThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CR<Length> size_) const {
		return WorkThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_queue_size (CR<Length> size_) const {
		return WorkThreadHolder::hold (thiz)->set_queue_size (size_) ;
	}

	void start (CR<Function<CR<Index>>> func) const {
		return WorkThreadHolder::hold (thiz)->start (func) ;
	}

	void post (CR<Index> begin_ ,CR<Index> end_) const {
		return WorkThreadHolder::hold (thiz)->post (begin_ ,end_) ;
	}

	void join () const {
		return WorkThreadHolder::hold (thiz)->join () ;
	}

	Bool join (CR<Time> interval) const {
		return WorkThreadHolder::hold (thiz)->join (interval) ;
	}

	void stop () const {
		if (!mThis.exist ())
			return ;
		return WorkThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct CalcSolution {
	Index mIteration ;
	NormalError mCost ;
	BitSet mInput ;
} ;

struct CalcThreadLayout ;

struct CalcThreadHolder implement Interface {
	imports Ref<CalcThreadLayout> create () ;
	imports VFat<CalcThreadHolder> hold (VR<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> hold (CR<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CR<Length> size_) = 0 ;
	virtual void set_start_input (CR<BitSet> input ,CR<Flt64> factor) = 0 ;
	virtual void start (CR<Function<CR<CalcSolution> ,VR<CalcSolution>>> func) = 0 ;
	virtual void friend_proc (CR<Index> slot) = 0 ;
	virtual Bool ready () const = 0 ;
	virtual CalcSolution pull () = 0 ;
	virtual void suspend () = 0 ;
	virtual void resume () = 0 ;
	virtual void stop () = 0 ;
} ;

class CalcThread implement Super<Ref<CalcThreadLayout>> {
public:
	implicit CalcThread () = default ;

	implicit CalcThread (CR<typeof (NULL)>) {
		mThis = CalcThreadHolder::create () ;
		CalcThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CR<Length> size_) const {
		return CalcThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_start_input (CR<BitSet> input ,CR<Flt64> factor) const {
		return CalcThreadHolder::hold (thiz)->set_start_input (input ,factor) ;
	}

	void start (CR<Function<CR<CalcSolution> ,VR<CalcSolution>>> func) const {
		return CalcThreadHolder::hold (thiz)->start (func) ;
	}

	Bool ready () const {
		return CalcThreadHolder::hold (thiz)->ready () ;
	}

	CalcSolution pull () const {
		return CalcThreadHolder::hold (thiz)->pull () ;
	}

	void suspend () const {
		return CalcThreadHolder::hold (thiz)->suspend () ;
	}

	void resume () const {
		return CalcThreadHolder::hold (thiz)->resume () ;
	}

	void stop () const {
		if (!mThis.exist ())
			return ;
		return CalcThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct PromiseLayout ;

struct PromiseHolder implement Interface {
	imports Ref<PromiseLayout> create () ;
	imports VFat<PromiseHolder> hold (VR<PromiseLayout> that) ;
	imports CFat<PromiseHolder> hold (CR<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CR<Bool> flag) = 0 ;
	virtual void start () = 0 ;
	virtual void start (CR<Function<>> func) = 0 ;
	virtual void friend_proc (CR<Index> slot) = 0 ;
	virtual void post (RR<AutoRef<Pointer>> item) = 0 ;
	virtual void rethrow (CR<Exception> e) = 0 ;
	virtual Bool ready () const = 0 ;
	virtual Bool running () const = 0 ;
	virtual AutoRef<Pointer> pull () = 0 ;
	virtual void future () = 0 ;
	virtual void stop () = 0 ;
} ;

template <class A>
class Promise implement Super<Ref<PromiseLayout>> {
public:
	implicit Promise () = default ;

	implicit Promise (CR<typeof (NULL)>) {
		mThis = PromiseHolder::create () ;
		PromiseHolder::hold (thiz)->initialize () ;
	}

	void set_retry (CR<Bool> flag) const {
		return PromiseHolder::hold (thiz)->set_retry (flag) ;
	}

	void start () const {
		return PromiseHolder::hold (thiz)->start () ;
	}

	void start (CR<Function<>> func) const {
		return PromiseHolder::hold (thiz)->start (func) ;
	}

	void post (CR<A> item) const {
		return post (move (item)) ;
	}

	void post (RR<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return PromiseHolder::hold (thiz)->post (move (rax)) ;
	}

	void rethrow (CR<Exception> e) const {
		return PromiseHolder::hold (thiz)->rethrow (e) ;
	}

	Bool ready () const {
		return PromiseHolder::hold (thiz)->ready () ;
	}

	Bool running () const {
		return PromiseHolder::hold (thiz)->running () ;
	}

	Optional<A> pull () const {
		auto rax = PromiseHolder::hold (thiz)->pull () ;
		if (!rax.exist ())
			return Optional<A>::error (1) ;
		return move (rax.rebind (TYPE<A>::expr).ref) ;
	}

	void future () const {
		return PromiseHolder::hold (thiz)->future () ;
	}

	void stop () const {
		if (!mThis.exist ())
			return ;
		return PromiseHolder::hold (thiz)->stop () ;
	}
} ;

class Execution ;

struct ReflectCompile implement Interface {
	virtual void compile (VR<Pointer> obj ,CR<Execution> exec) const = 0 ;

	forceinline static consteval Flag expr_m () noexcept {
		return 300 ;
	}
} ;

template <class A>
class ReflectCompileBinder final implement Fat<ReflectCompile ,void> {
public:
	void compile (VR<Pointer> obj ,CR<Execution> exec) const override {
		auto &&rax = keep[TYPE<A>::expr] (obj) ;
		rax.compile (exec) ;
	}
} ;

struct ExecutionLayout ;

struct ExecutionHolder implement Interface {
	imports Ref<ExecutionLayout> create () ;
	imports VFat<ExecutionHolder> hold (VR<ExecutionLayout> that) ;
	imports CFat<ExecutionHolder> hold (CR<ExecutionLayout> that) ;

	virtual void initialize (CR<String<Str>> dire) = 0 ;
	virtual Path local_path () const = 0 ;
	virtual String<Str> local_flow () const = 0 ;
	virtual String<Str> local_node () const = 0 ;
	virtual CR<Pointer> stack (CR<String<Str>> flow ,CR<String<Str>> node) leftvalue = 0 ;
	virtual void once (CR<Function<>> actor) = 0 ;
	virtual void then (CR<Function<>> actor) = 0 ;
	virtual void monad (CR<Unknown> holder ,CR<String<Str>> node) = 0 ;
	virtual void undo (CR<String<Str>> flow ,CR<String<Str>> node) = 0 ;
	virtual void execute () = 0 ;
} ;

template <class A>
class ExecutionUnknownBinder final implement Fat<UnknownHolder ,void> {
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
		return ZERO ;
	}
} ;

class Execution implement Super<Ref<ExecutionLayout>> {
public:
	implicit Execution () = default ;

	implicit Execution (CR<String<Str>> dire) {
		mThis = ExecutionHolder::create () ;
		ExecutionHolder::hold (thiz)->initialize (dire) ;
	}

	Path local_path () const {
		return ExecutionHolder::hold (thiz)->local_path () ;
	}

	String<Str> local_flow () const {
		return ExecutionHolder::hold (thiz)->local_flow () ;
	}

	String<Str> local_node () const {
		return ExecutionHolder::hold (thiz)->local_node () ;
	}

	CR<Pointer> stack (CR<String<Str>> flow ,CR<String<Str>> node) const leftvalue {
		return ExecutionHolder::hold (thiz)->stack (flow ,node) ;
	}

	void once (CR<Function<>> actor) const {
		return ExecutionHolder::hold (thiz)->once (actor) ;
	}

	void then (CR<Function<>> actor) const {
		return ExecutionHolder::hold (thiz)->then (actor) ;
	}

	template <class ARG1>
	void monad (TYPE<ARG1> ,CR<String<Str>> node) const {
		return ExecutionHolder::hold (thiz)->monad (ExecutionUnknownBinder<ARG1> () ,node) ;
	}

	void undo (CR<String<Str>> flow ,CR<String<Str>> node) const {
		return ExecutionHolder::hold (thiz)->undo (flow ,node) ;
	}

	void execute () const {
		return ExecutionHolder::hold (thiz)->execute () ;
	}
} ;
} ;