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
	virtual void before () = 0 ;
	virtual Bool tick (CR<Flt64> deltatime) = 0 ;
	virtual Bool idle () = 0 ;
	virtual void after () = 0 ;
	virtual void execute () = 0 ;
} ;

class Coroutine implement Proxy {
protected:
	VFat<CoroutineHolder> mThat ;

public:
	implicit Coroutine () = delete ;

	template <class ARG1 ,class = REQUIRE<IS_EXTEND<CoroutineHolder ,ARG1>>>
	implicit Coroutine (CR<VFat<ARG1>> that) :mThat (that) {}

	void before () const {
		return mThat->before () ;
	}

	Bool tick (CR<Flt64> deltatime) const {
		return mThat->tick (deltatime) ;
	}

	Bool idle () const {
		return mThat->idle () ;
	}

	void after () const {
		return mThat->after () ;
	}

	void execute () const {
		return mThat->execute () ;
	}
} ;

struct WorkThreadLayout ;

struct WorkThreadHolder implement Interface {
	imports SharedRef<WorkThreadLayout> create () ;
	imports VFat<WorkThreadHolder> hold (VR<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> hold (CR<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CR<Length> size_) = 0 ;
	virtual void set_queue_size (CR<Length> size_) = 0 ;
	virtual void start (CR<Function<CR<Index>>> func) = 0 ;
	virtual void friend_execute (CR<Index> slot) = 0 ;
	virtual void post (CR<Index> begin_ ,CR<Index> end_) = 0 ;
	virtual void join () = 0 ;
	virtual Bool join (CR<Time> interval) = 0 ;
	virtual void stop () = 0 ;
} ;

class WorkThread implement Like<SharedRef<WorkThreadLayout>> {
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
	Flt64 mAvgError ;
	Flt64 mStdError ;
	BitSet mInput ;
} ;

struct CalcThreadLayout ;

struct CalcThreadHolder implement Interface {
	imports SharedRef<CalcThreadLayout> create () ;
	imports VFat<CalcThreadHolder> hold (VR<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> hold (CR<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CR<Length> size_) = 0 ;
	virtual void set_start_input (CR<BitSet> input ,CR<Flt64> factor) = 0 ;
	virtual void start (CR<Function<CR<CalcSolution> ,VR<CalcSolution>>> func) = 0 ;
	virtual void friend_execute (CR<Index> slot) = 0 ;
	virtual Bool ready () const = 0 ;
	virtual CalcSolution poll () = 0 ;
	virtual void suspend () = 0 ;
	virtual void resume () = 0 ;
	virtual void stop () = 0 ;
} ;

class CalcThread implement Like<SharedRef<CalcThreadLayout>> {
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

	CalcSolution poll () const {
		return CalcThreadHolder::hold (thiz)->poll () ;
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
	imports SharedRef<PromiseLayout> create () ;
	imports VFat<PromiseHolder> hold (VR<PromiseLayout> that) ;
	imports CFat<PromiseHolder> hold (CR<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CR<Bool> flag) = 0 ;
	virtual void start () = 0 ;
	virtual void start (CR<Function<>> func) = 0 ;
	virtual void friend_execute (CR<Index> slot) = 0 ;
	virtual void post (RR<AutoRef<Pointer>> item) = 0 ;
	virtual void rethrow (CR<Exception> e) = 0 ;
	virtual Bool ready () const = 0 ;
	virtual Bool running () const = 0 ;
	virtual AutoRef<Pointer> poll () = 0 ;
	virtual void future () = 0 ;
	virtual void stop () = 0 ;
} ;

template <class A>
class Promise implement Like<SharedRef<PromiseLayout>> {
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

	Optional<A> poll () const {
		auto rax = PromiseHolder::hold (thiz)->poll () ;
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

struct SyntaxLayout ;

struct SyntaxHolder implement Interface {
	imports SharedRef<SyntaxLayout> create () ;
	imports VFat<SyntaxHolder> hold (VR<SyntaxLayout> that) ;
	imports CFat<SyntaxHolder> hold (CR<SyntaxLayout> that) ;

	virtual void initialize () = 0 ;
	virtual CR<Pointer> stack (CR<Clazz> name) const leftvalue = 0 ;
	virtual CR<Pointer> maybe (CR<Clazz> name) const leftvalue = 0 ;
	virtual void once (CR<Function<>> func) = 0 ;
	virtual void then (CR<Function<>> func) = 0 ;
	virtual void monad (CR<Clazz> name) = 0 ;
	virtual void until (RR<Ref<Bool>> flag) = 0 ;
	virtual void execute () = 0 ;
} ;

class Syntax implement Like<SharedRef<SyntaxLayout>> {
public:
	implicit Syntax () = default ;

	implicit Syntax (CR<typeof (NULL)>) {
		mThis = SyntaxHolder::create () ;
		SyntaxHolder::hold (thiz)->initialize () ;
	}

	template <class ARG1>
	CR<ARG1> stack (TYPE<ARG1>) const leftvalue {
		return SyntaxHolder::hold (thiz)->stack (Clazz (TYPE<ARG1>::expr)) ;
	}

	template <class ARG1>
	CR<ARG1> maybe (TYPE<ARG1>) const leftvalue {
		return SyntaxHolder::hold (thiz)->maybe (Clazz (TYPE<ARG1>::expr)) ;
	}

	void once (CR<Function<>> func) {
		return SyntaxHolder::hold (thiz)->once (func) ;
	}

	void then (CR<Function<>> func) {
		return SyntaxHolder::hold (thiz)->then (func) ;
	}

	template <class ARG1>
	void monad (TYPE<ARG1>) {
		return SyntaxHolder::hold (thiz)->monad (Clazz (TYPE<ARG1>::expr)) ;
	}

	void until (RR<Ref<Bool>> flag) {
		return SyntaxHolder::hold (thiz)->until (move (flag)) ;
	}

	void execute () {
		return SyntaxHolder::hold (thiz)->execute () ;
	}
} ;
} ;