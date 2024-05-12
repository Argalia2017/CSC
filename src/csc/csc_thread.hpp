#pragma once

#ifndef __CSC_THREAD__
#define __CSC_THREAD__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_runtime.hpp"

namespace CSC {
struct WorkThreadPureLayout ;

struct WorkThreadLayout {
	SharedRef<WorkThreadPureLayout> mThis ;
} ;

struct WorkThreadHolder implement Interface {
	imports VFat<WorkThreadHolder> create (VREF<WorkThreadPureLayout> that) ;
	imports CFat<WorkThreadHolder> create (CREF<WorkThreadPureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
	virtual void set_queue_size (CREF<LENGTH> size_) = 0 ;
	virtual void start (RREF<Function<CREF<INDEX>>> func) = 0 ;
	virtual void post (CREF<INDEX> item) = 0 ;
	virtual void post (CREF<Array<INDEX>> item) = 0 ;
	virtual void join () = 0 ;
	virtual BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) = 0 ;
	virtual void stop () = 0 ;
} ;

class WorkThread implement WorkThreadLayout {
protected:
	using WorkThreadLayout::mThis ;

public:
	implicit WorkThread () = default ;

	void set_thread_size (CREF<LENGTH> size_) {
		return WorkThreadHolder::create (mThis)->set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) {
		return WorkThreadHolder::create (mThis)->set_queue_size (size_) ;
	}

	void start (RREF<Function<CREF<INDEX>>> func) {
		return WorkThreadHolder::create (mThis)->start (move (func)) ;
	}

	void post (CREF<INDEX> item) {
		return WorkThreadHolder::create (mThis)->post (item) ;
	}

	void post (CREF<Array<INDEX>> item) {
		return WorkThreadHolder::create (mThis)->post (item) ;
	}

	void join () {
		return WorkThreadHolder::create (mThis)->join () ;
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) {
		return WorkThreadHolder::create (mThis)->join (interval ,predicate) ;
	}

	void stop () {
		return WorkThreadHolder::create (mThis)->stop () ;
	}
} ;

struct CalcSolution {
	INDEX mIteration ;
	NORMALERROR mCost ;
	Array<FLT64> mValue ;
} ;

struct CalcThreadPureLayout ;

struct CalcThreadLayout {
	SharedRef<CalcThreadPureLayout> mThis ;
} ;

struct CalcThreadHolder implement Interface {
	imports VFat<CalcThreadHolder> create (VREF<CalcThreadPureLayout> that) ;
	imports CFat<CalcThreadHolder> create (CREF<CalcThreadPureLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
	virtual void start (RREF<Function<VREF<CalcSolution>>> func) = 0 ;
	virtual CalcSolution best () = 0 ;
	virtual void suspend () = 0 ;
	virtual void resume () = 0 ;
	virtual void stop () = 0 ;
} ;

class CalcThread implement CalcThreadLayout {
protected:
	using CalcThreadLayout::mThis ;

public:
	implicit CalcThread () = default ;

	void set_thread_size (CREF<LENGTH> size_) {
		return CalcThreadHolder::create (mThis)->set_thread_size (size_) ;
	}

	void start (RREF<Function<VREF<CalcSolution>>> func) {
		return CalcThreadHolder::create (mThis)->start (move (func)) ;
	}

	CalcSolution best () {
		return CalcThreadHolder::create (mThis)->best () ;
	}

	void suspend () {
		return CalcThreadHolder::create (mThis)->suspend () ;
	}

	void resume () {
		return CalcThreadHolder::create (mThis)->resume () ;
	}

	void stop () {
		return CalcThreadHolder::create (mThis)->stop () ;
	}
} ;

struct PromiseImplLayout ;

struct PromiseLayout {
	SharedRef<PromiseImplLayout> mThis ;
} ;

struct PromiseHolder implement Interface {
	imports VFat<PromiseHolder> create (VREF<PromiseLayout> that) ;
	imports CFat<PromiseHolder> create (CREF<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void start () const = 0 ;
	virtual void start (RREF<Function<VREF<AutoRef<Pointer>>>> func) const = 0 ;
	virtual void post (RREF<AutoRef<Pointer>> item) const = 0 ;
	virtual void rethrow (CREF<Exception> e) const = 0 ;
	virtual void signal () const = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual AutoRef<Pointer> poll () const = 0 ;
	virtual PromiseLayout then (RREF<Function<VREF<AutoRef<Pointer>>>> func) const = 0 ;
	virtual void stop () const = 0 ;
} ;

class Promise implement PromiseLayout {
protected:
	using PromiseLayout::mThis ;

public:
	implicit Promise () = default ;

	imports Promise make (RREF<Function<VREF<AutoRef<Pointer>>>> func) {
		Promise ret ;
		ret.start (move (func)) ;
		return move (ret) ;
	}

	void start () const {
		return PromiseHolder::create (thiz)->start () ;
	}

	void start (RREF<Function<VREF<AutoRef<Pointer>>>> func) const {
		return PromiseHolder::create (thiz)->start (move (func)) ;
	}

	void post (RREF<AutoRef<Pointer>> item) const {
		return PromiseHolder::create (thiz)->post (move (item)) ;
	}

	void rethrow (CREF<Exception> e) const {
		return PromiseHolder::create (thiz)->rethrow (e) ;
	}

	void signal () const {
		return PromiseHolder::create (thiz)->signal () ;
	}

	BOOL ready () const {
		return PromiseHolder::create (thiz)->ready () ;
	}

	AutoRef<Pointer> poll () const {
		return PromiseHolder::create (thiz)->poll () ;
	}

	Promise then (RREF<Function<VREF<AutoRef<Pointer>>>> func) const {
		PromiseLayout ret = PromiseHolder::create (thiz)->then (move (func)) ;
		return move (keep[TYPE<Promise>::expr] (ret)) ;
	}

	void stop () const {
		return PromiseHolder::create (thiz)->stop () ;
	}
} ;
} ;