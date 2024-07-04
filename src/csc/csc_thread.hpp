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
struct CoroutineFriend implement Interface {
	virtual void before () const = 0 ;
	virtual BOOL tick () const = 0 ;
	virtual void after () const = 0 ;
} ;

template <class A>
class CoroutineFriendBinder implement Fat<CoroutineFriend ,A> {
public:
	imports VFat<CoroutineFriend> create (VREF<A> that) {
		return VFat<CoroutineFriend> (CoroutineFriendBinder () ,that) ;
	}

	void before () override {
		return thiz.fake.before () ;
	}

	BOOL tick () override {
		return thiz.fake.tick () ;
	}

	void after () override {
		return thiz.fake.after () ;
	}
} ;

struct WorkThreadImplLayout ;

struct WorkThreadLayout {
	SharedRef<WorkThreadImplLayout> mThis ;
} ;

struct WorkThreadHolder implement Interface {
	imports VFat<WorkThreadHolder> create (VREF<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> create (CREF<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) const = 0 ;
	virtual void set_queue_size (CREF<LENGTH> size_) const = 0 ;
	virtual void start (CREF<Function<CREF<INDEX>>> func) const = 0 ;
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
	explicit WorkThread () {
		WorkThreadHolder::create (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::create (thiz)->set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::create (thiz)->set_queue_size (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) const {
		return WorkThreadHolder::create (thiz)->start (func) ;
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
	BitSet mInput ;

public:
	CalcSolution clone () const {
		CalcSolution ret ;
		ret.mIndex = mIndex ;
		ret.mError = mError ;
		ret.mInput = mInput.clone () ;
		return move (ret) ;
	}
} ;

struct CalcThreadImplLayout ;

struct CalcThreadLayout {
	SharedRef<CalcThreadImplLayout> mThis ;
} ;

struct CalcThreadHolder implement Interface {
	imports VFat<CalcThreadHolder> create (VREF<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> create (CREF<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) const = 0 ;
	virtual void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const = 0 ;
	virtual CalcSolution best () const = 0 ;
	virtual void join () const = 0 ;
	virtual void suspend () const = 0 ;
	virtual void resume () const = 0 ;
	virtual void stop () const = 0 ;
} ;

class CalcThread implement CalcThreadLayout {
protected:
	using CalcThreadLayout::mThis ;

public:
	explicit CalcThread () {
		CalcThreadHolder::create (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return CalcThreadHolder::create (thiz)->set_thread_size (size_) ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const {
		return CalcThreadHolder::create (thiz)->start (func) ;
	}

	CalcSolution best () const {
		return CalcThreadHolder::create (thiz)->best () ;
	}

	void join () const {
		return CalcThreadHolder::create (thiz)->join () ;
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

struct PromiseImplLayout ;

struct PromiseLayout {
	SharedRef<PromiseImplLayout> mThis ;
} ;

struct PromiseHolder implement Interface {
	imports VFat<PromiseHolder> create (VREF<PromiseLayout> that) ;
	imports CFat<PromiseHolder> create (CREF<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CREF<BOOL> flag) const = 0 ;
	virtual void start () const = 0 ;
	virtual void start (CREF<Function<>> func) const = 0 ;
	virtual void post (RREF<AutoRef<Pointer>> item) const = 0 ;
	virtual void rethrow (CREF<Exception> e) const = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual BOOL running () const = 0 ;
	virtual AutoRef<Pointer> poll () const = 0 ;
	virtual void signal () const = 0 ;
	virtual void stop () const = 0 ;
} ;

template <class A>
class Promise implement PromiseLayout {
protected:
	using PromiseLayout::mThis ;

public:
	explicit Promise () {
		PromiseHolder::create (thiz)->initialize () ;
	}

	void set_retry (CREF<BOOL> flag) const {
		return PromiseHolder::create (thiz)->set_retry (flag) ;
	}

	void start () const {
		return PromiseHolder::create (thiz)->start () ;
	}

	void start (CREF<Function<>> func) const {
		return PromiseHolder::create (thiz)->start (func) ;
	}

	void post (CREF<A> item) const {
		return post (move (item)) ;
	}

	void post (RREF<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return PromiseHolder::create (thiz)->post (move (rax)) ;
	}

	void rethrow (CREF<Exception> e) const {
		return PromiseHolder::create (thiz)->rethrow (e) ;
	}

	BOOL ready () const {
		return PromiseHolder::create (thiz)->ready () ;
	}

	BOOL running () const {
		return PromiseHolder::create (thiz)->running () ;
	}

	Optional<A> poll () const {
		auto rax = PromiseHolder::create (thiz)->poll () ;
		if (!rax.exist ())
			return Optional<A>::error (1) ;
		return move (rax.rebind (TYPE<A>::expr).self) ;
	}

	void signal () const {
		return PromiseHolder::create (thiz)->signal () ;
	}

	void stop () const {
		return PromiseHolder::create (thiz)->stop () ;
	}
} ;

struct ExpressionImplLayout ;

struct ExpressionLayout {
	SharedRef<ExpressionImplLayout> mThis ;
	INDEX mIndex ;
} ;

struct ExpressionHolder implement Interface {
	imports VFat<ExpressionHolder> create (VREF<ExpressionLayout> that) ;
	imports CFat<ExpressionHolder> create (CREF<ExpressionLayout> that) ;

	virtual void initialize (RREF<AutoRef<Pointer>> item) = 0 ;
	virtual LENGTH rank () const = 0 ;
	virtual ExpressionLayout add (CREF<ExpressionLayout> that) const = 0 ;
	virtual ExpressionLayout sub (CREF<ExpressionLayout> that) const = 0 ;
	virtual CREF<AutoRef<Pointer>> eval () const leftvalue = 0 ;
} ;

class Expression implement ExpressionLayout {
protected:
	using ExpressionLayout::mThis ;
	using ExpressionLayout::mIndex ;

public:
	implicit Expression () = default ;

	template <class ARG1>
	explicit Expression (RREF<AutoRef<ARG1>> item) {
		ExpressionHolder::create (thiz)->initialize (move (item)) ;
	}

	LENGTH rank () const {
		return ExpressionHolder::create (thiz)->rank () ;
	}

	Expression add (CREF<Expression> that) const {
		ExpressionLayout ret = ExpressionHolder::create (thiz)->add (that) ;
		return move (keep[TYPE<Expression>::expr] (ret)) ;
	}

	forceinline Expression operator+ (CREF<Expression> that) const {
		return add (that) ;
	}

	forceinline void operator+= (CREF<Expression> that) {
		thiz = add (that) ;
	}

	Expression sub (CREF<Expression> that) const {
		ExpressionLayout ret = ExpressionHolder::create (thiz)->sub (that) ;
		return move (keep[TYPE<Expression>::expr] (ret)) ;
	}

	forceinline Expression operator- (CREF<Expression> that) const {
		return sub (that) ;
	}

	forceinline void operator-= (CREF<Expression> that) {
		thiz = sub (that) ;
	}

	template <class ARG1>
	CREF<AutoRef<ARG1>> eval (TYPE<ARG1>) const leftvalue {
		auto &&rax = ExpressionHolder::create (thiz)->eval () ;
		return rax.rebind (TYPE<ARG1>::expr).self ;
	}
} ;
} ;