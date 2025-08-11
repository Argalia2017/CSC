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
struct FriendCoroutine implement Interface {
	virtual void before () = 0 ;
	virtual BOOL tick (CREF<FLT64> deltatime) = 0 ;
	virtual BOOL idle () = 0 ;
	virtual void after () = 0 ;

	void execute () {
		thiz.before () ;
		while (TRUE) {
			while (thiz.tick (0))
				noop () ;
			if (!thiz.idle ())
				break ;
		}
		thiz.after () ;
	}
} ;

template <class A>
class FriendCoroutineBinder final implement Fat<FriendCoroutine ,A> {
public:
	static VFat<FriendCoroutine> hold (VREF<A> that) {
		return VFat<FriendCoroutine> (FriendCoroutineBinder () ,that) ;
	}

	void before () override {
		return thiz.self.before () ;
	}

	BOOL tick (CREF<FLT64> deltatime) override {
		return thiz.self.tick (deltatime) ;
	}

	BOOL idle () override {
		return thiz.self.idle () ;
	}

	void after () override {
		return thiz.self.after () ;
	}
} ;

struct WorkThreadLayout ;

struct WorkThreadHolder implement Interface {
	imports SharedRef<WorkThreadLayout> create () ;
	imports VFat<WorkThreadHolder> hold (VREF<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> hold (CREF<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
	virtual void set_queue_size (CREF<LENGTH> size_) = 0 ;
	virtual void start (CREF<Function<CREF<INDEX>>> func) = 0 ;
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
	virtual void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) = 0 ;
	virtual void join () = 0 ;
	virtual BOOL join (CREF<Time> interval) = 0 ;
	virtual void stop () = 0 ;
} ;

class WorkThread implement OfThis<SharedRef<WorkThreadLayout>> {
public:
	implicit WorkThread () = default ;

	implicit WorkThread (CREF<typeof (NULL)>) {
		mThis = WorkThreadHolder::create () ;
		WorkThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_queue_size (CREF<LENGTH> size_) const {
		return WorkThreadHolder::hold (thiz)->set_queue_size (size_) ;
	}

	void start (CREF<Function<CREF<INDEX>>> func) const {
		return WorkThreadHolder::hold (thiz)->start (func) ;
	}

	void post (CREF<INDEX> begin_ ,CREF<INDEX> end_) const {
		return WorkThreadHolder::hold (thiz)->post (begin_ ,end_) ;
	}

	void join () const {
		return WorkThreadHolder::hold (thiz)->join () ;
	}

	BOOL join (CREF<Time> interval) const {
		return WorkThreadHolder::hold (thiz)->join (interval) ;
	}

	void stop () const {
		if (!mThis.exist ())
			return ;
		return WorkThreadHolder::hold (thiz)->stop () ;
	}
} ;

struct CalcSolution {
	INDEX mIteration ;
	FLT64 mAvgError ;
	FLT64 mStdError ;
	BitSet mInput ;
} ;

struct CalcThreadLayout ;

struct CalcThreadHolder implement Interface {
	imports SharedRef<CalcThreadLayout> create () ;
	imports VFat<CalcThreadHolder> hold (VREF<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> hold (CREF<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CREF<LENGTH> size_) = 0 ;
	virtual void set_start_input (CREF<BitSet> input ,CREF<FLT64> factor) = 0 ;
	virtual void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) = 0 ;
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual CalcSolution poll () = 0 ;
	virtual void suspend () = 0 ;
	virtual void resume () = 0 ;
	virtual void stop () = 0 ;
} ;

class CalcThread implement OfThis<SharedRef<CalcThreadLayout>> {
public:
	implicit CalcThread () = default ;

	implicit CalcThread (CREF<typeof (NULL)>) {
		mThis = CalcThreadHolder::create () ;
		CalcThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CREF<LENGTH> size_) const {
		return CalcThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_start_input (CREF<BitSet> input ,CREF<FLT64> factor) const {
		return CalcThreadHolder::hold (thiz)->set_start_input (input ,factor) ;
	}

	void start (CREF<Function<CREF<CalcSolution> ,VREF<CalcSolution>>> func) const {
		return CalcThreadHolder::hold (thiz)->start (func) ;
	}

	BOOL ready () const {
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
	imports VFat<PromiseHolder> hold (VREF<PromiseLayout> that) ;
	imports CFat<PromiseHolder> hold (CREF<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CREF<BOOL> flag) = 0 ;
	virtual void start () = 0 ;
	virtual void start (CREF<Function<>> func) = 0 ;
	virtual void friend_execute (CREF<INDEX> slot) = 0 ;
	virtual void post (RREF<AutoRef<Pointer>> item) = 0 ;
	virtual void rethrow (CREF<Exception> e) = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual BOOL running () const = 0 ;
	virtual AutoRef<Pointer> poll () = 0 ;
	virtual void future () = 0 ;
	virtual void stop () = 0 ;
} ;

template <class A>
class Promise implement OfThis<SharedRef<PromiseLayout>> {
public:
	implicit Promise () = default ;

	implicit Promise (CREF<typeof (NULL)>) {
		mThis = PromiseHolder::create () ;
		PromiseHolder::hold (thiz)->initialize () ;
	}

	void set_retry (CREF<BOOL> flag) const {
		return PromiseHolder::hold (thiz)->set_retry (flag) ;
	}

	void start () const {
		return PromiseHolder::hold (thiz)->start () ;
	}

	void start (CREF<Function<>> func) const {
		return PromiseHolder::hold (thiz)->start (func) ;
	}

	void post (CREF<A> item) const {
		return post (move (item)) ;
	}

	void post (RREF<A> item) const {
		auto rax = AutoRef<A>::make (move (item)) ;
		return PromiseHolder::hold (thiz)->post (move (rax)) ;
	}

	void rethrow (CREF<Exception> e) const {
		return PromiseHolder::hold (thiz)->rethrow (e) ;
	}

	BOOL ready () const {
		return PromiseHolder::hold (thiz)->ready () ;
	}

	BOOL running () const {
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

struct EntityLayout ;
struct ComponentLayout ;
struct ServiceLayout ;

struct EntityHolder implement Interface {
	imports SharedRef<EntityLayout> create () ;
	imports VFat<EntityHolder> hold (VREF<EntityLayout> that) ;
	imports CFat<EntityHolder> hold (CREF<EntityLayout> that) ;

	virtual void initialize (CREF<Clazz> clazz_) = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual INDEX keyid () const = 0 ;
	virtual void add_component (CREF<OfThis<SharedRef<ComponentLayout>>> component) = 0 ;
	virtual void register_service (CREF<OfThis<SharedRef<ServiceLayout>>> service) = 0 ;
} ;

class Entity implement OfThis<SharedRef<EntityLayout>> {
public:
	implicit Entity () = default ;

	implicit Entity (CREF<Clazz> clazz_) {
		mThis = EntityHolder::create () ;
		EntityHolder::hold (thiz)->initialize (clazz_) ;
	}

	Clazz clazz () const {
		return EntityHolder::hold (thiz)->clazz () ;
	}

	INDEX keyid () const {
		return EntityHolder::hold (thiz)->keyid () ;
	}

	void add_component (CREF<OfThis<SharedRef<ComponentLayout>>> component) const {
		return EntityHolder::hold (thiz)->add_component (component) ;
	}
	
	void register_service (CREF<OfThis<SharedRef<ServiceLayout>>> service) const {
		return EntityHolder::hold (thiz)->register_service (service) ;
	}
} ;

struct ComponentHolder implement Interface {
	imports SharedRef<ComponentLayout> create () ;
	imports VFat<ComponentHolder> hold (VREF<ComponentLayout> that) ;
	imports CFat<ComponentHolder> hold (CREF<ComponentLayout> that) ;

	virtual void initialize (CREF<Clazz> clazz_) = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual BOOL contain (CREF<Clazz> clazz_) const = 0 ;
	virtual OfThis<SharedRef<ComponentLayout>> get (CREF<Clazz> clazz_) const = 0 ;
} ;

class Component implement OfThis<SharedRef<ComponentLayout>> {
public:
	implicit Component () = default ;

	implicit Component (CREF<Clazz> clazz_) {
		mThis = ComponentHolder::create () ;
		ComponentHolder::hold (thiz)->initialize (clazz_) ;
	}

	Clazz clazz () const {
		return ComponentHolder::hold (thiz)->clazz () ;
	}

	BOOL contain (CREF<Clazz> clazz_) const {
		return ComponentHolder::hold (thiz)->contain (clazz_) ;
	}

	Component get (CREF<Clazz> clazz_) const {
		OfThis<SharedRef<ComponentLayout>> ret = ComponentHolder::hold (thiz)->get (clazz_) ;
		return move (keep[TYPE<Component>::expr] (ret)) ;
	}
} ;

struct ServiceHolder implement Interface {
	imports SharedRef<ServiceLayout> create () ;
	imports VFat<ServiceHolder> hold (VREF<ServiceLayout> that) ;
	imports CFat<ServiceHolder> hold (CREF<ServiceLayout> that) ;

	virtual void initialize (CREF<Clazz> clazz_) = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual INDEX spwan_entity () = 0 ;
} ;

class Service implement OfThis<SharedRef<ServiceLayout>> {
public:
	implicit Service () = default ;

	implicit Service (CREF<Clazz> clazz_) {
		mThis = ServiceHolder::create () ;
		ServiceHolder::hold (thiz)->initialize (clazz_) ;
	}

	Clazz clazz () const {
		return ServiceHolder::hold (thiz)->clazz () ;
	}

	INDEX spwan_entity () const {
		return ServiceHolder::hold (thiz)->spwan_entity () ;
	}
} ;

struct ECSManagerLayout ;

struct ECSManagerHolder implement Interface {
	imports CREF<OfThis<SharedRef<ECSManagerLayout>>> expr_m () ;
	imports VFat<ECSManagerHolder> hold (VREF<ECSManagerLayout> that) ;
	imports CFat<ECSManagerHolder> hold (CREF<ECSManagerLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Entity entity (CREF<INDEX> index) const = 0 ;
	virtual INDEX entity (CREF<Entity> item) = 0 ;
	virtual Component component (CREF<INDEX> index) const = 0 ;
	virtual INDEX component (CREF<Component> item) = 0 ;
	virtual Service service (CREF<INDEX> index) const = 0 ;
	virtual INDEX service (CREF<Service> item) = 0 ;
} ;

class ECSManager implement OfThis<SharedRef<ECSManagerLayout>> {
public:
	imports CREF<ECSManager> expr_m () {
		return keep[TYPE<ECSManager>::expr] (ECSManagerHolder::expr) ;
	}

	Entity entity (CREF<INDEX> index) const {
		return ECSManagerHolder::hold (thiz)->entity (index) ;
	}

	INDEX entity (CREF<Entity> item) const {
		return ECSManagerHolder::hold (thiz)->entity (item) ;
	}

	Component component (CREF<INDEX> index) const {
		return ECSManagerHolder::hold (thiz)->component (index) ;
	}

	INDEX component (CREF<Component> item) const {
		return ECSManagerHolder::hold (thiz)->component (item) ;
	}

	Service service (CREF<INDEX> index) const {
		return ECSManagerHolder::hold (thiz)->service (index) ;
	}

	INDEX service (CREF<Service> item) const {
		return ECSManagerHolder::hold (thiz)->service (item) ;
	}
} ;
} ;