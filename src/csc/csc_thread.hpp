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
#include "csc_runtime.hpp"

namespace CSC {
class WorkThreadImplLayout ;

struct WorkThreadLayout {
	SharedRef<WorkThreadImplLayout> mThis ;
} ;

struct WorkThreadHolder implement Interface {
	imports VFat<WorkThreadHolder> create (VREF<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> create (CREF<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) const = 0 ;
	virtual void set_queue_size (CREF<LENGTH> size_) const = 0 ;
	virtual void start (RREF<Function<CREF<INDEX>>> func) const = 0 ;
	virtual void post (CREF<INDEX> item) const = 0 ;
	virtual void post (CREF<Array<INDEX>> item) const = 0 ;
	virtual void join () const = 0 ;
	virtual BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) const = 0 ;
	virtual void stop () const = 0 ;
} ;

class WorkThread implement WorkThreadLayout {
protected:
	using WorkThreadLayout::mThis ;

public:
	implicit WorkThread () = default ;

	implicit WorkThread (CREF<typeof (FULL)>) {
		WorkThreadHolder::create (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::create (thiz)->set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::create (thiz)->set_queue_size (size_) ;
	}

	void start (RREF<Function<CREF<INDEX>>> func) const {
		return WorkThreadHolder::create (thiz)->start (move (func)) ;
	}

	void post (CREF<INDEX> item) const {
		return WorkThreadHolder::create (thiz)->post (item) ;
	}

	void post (CREF<Array<INDEX>> item) const {
		return WorkThreadHolder::create (thiz)->post (item) ;
	}

	void join () const {
		return WorkThreadHolder::create (thiz)->join () ;
	}

	BOOL join (CREF<Time> interval ,CREF<Function<VREF<BOOL>>> predicate) const {
		return WorkThreadHolder::create (thiz)->join (interval ,predicate) ;
	}

	void stop () const {
		return WorkThreadHolder::create (thiz)->stop () ;
	}
} ;

struct CalcSolution {
	INDEX mIndex ;
	FLT64 mError ;
	BitSet mValue ;
} ;

class CalcThreadImplLayout ;

struct CalcThreadLayout {
	SharedRef<CalcThreadImplLayout> mThis ;
} ;

struct CalcThreadHolder implement Interface {
	imports VFat<CalcThreadHolder> create (VREF<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> create (CREF<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) const = 0 ;
	virtual void start (RREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const = 0 ;
	virtual CalcSolution best () const = 0 ;
	virtual void suspend () const = 0 ;
	virtual void resume () const = 0 ;
	virtual void stop () const = 0 ;
} ;

class CalcThread implement CalcThreadLayout {
protected:
	using CalcThreadLayout::mThis ;

public:
	implicit CalcThread () = default ;

	implicit CalcThread (CREF<typeof (FULL)>) {
		CalcThreadHolder::create (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return CalcThreadHolder::create (thiz)->set_thread_size (size_) ;
	}

	void start (RREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const {
		return CalcThreadHolder::create (thiz)->start (move (func)) ;
	}

	CalcSolution best () const {
		return CalcThreadHolder::create (thiz)->best () ;
	}

	void suspend () const {
		return CalcThreadHolder::create (thiz)->suspend () ;
	}

	void resume () const {
		return CalcThreadHolder::create (thiz)->resume () ;
	}

	void stop () const {
		return CalcThreadHolder::create (thiz)->stop () ;
	}
} ;

class PromiseImplLayout ;

struct PromiseLayout {
	SharedRef<PromiseImplLayout> mThis ;
} ;

struct PromiseHolder implement Interface {
	imports VFat<PromiseHolder> create (VREF<PromiseLayout> that) ;
	imports CFat<PromiseHolder> create (CREF<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void start () const = 0 ;
	virtual void start (RREF<FunctionLayout> func) const = 0 ;
	virtual void post (RREF<AutoRef<Pointer>> item) const = 0 ;
	virtual void rethrow (CREF<Exception> e) const = 0 ;
	virtual void signal () const = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual AutoRef<Pointer> poll () const = 0 ;
	virtual void stop () const = 0 ;
} ;

template <class A>
class Promise implement PromiseLayout {
protected:
	using PromiseLayout::mThis ;

public:
	implicit Promise () = default ;

	implicit Promise (CREF<typeof (FULL)>) {
		PromiseHolder::create (thiz)->initialize () ;
	}

	imports Promise make (RREF<Function<VREF<AutoRef<A>>>> func) {
		Promise ret = FULL ;
		ret.start (move (func)) ;
		return move (ret) ;
	}

	void start () const {
		return PromiseHolder::create (thiz)->start () ;
	}

	void start (RREF<Function<VREF<AutoRef<A>>>> func) const {
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

	AutoRef<A> poll () const {
		return PromiseHolder::create (thiz)->poll () ;
	}

	void stop () const {
		return PromiseHolder::create (thiz)->stop () ;
	}
} ;
} ;