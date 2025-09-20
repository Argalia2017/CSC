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
	virtual BOOL tick (CR<FLT64> deltatime) = 0 ;
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
	static VFat<FriendCoroutine> hold (VR<A> that) {
		return VFat<FriendCoroutine> (FriendCoroutineBinder () ,that) ;
	}

	void before () override {
		return thiz.self.before () ;
	}

	BOOL tick (CR<FLT64> deltatime) override {
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
	imports VFat<WorkThreadHolder> hold (VR<WorkThreadLayout> that) ;
	imports CFat<WorkThreadHolder> hold (CR<WorkThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CR<LENGTH> size_) = 0 ;
	virtual void set_queue_size (CR<LENGTH> size_) = 0 ;
	virtual void start (CR<Function<CR<INDEX>>> func) = 0 ;
	virtual void friend_execute (CR<INDEX> slot) = 0 ;
	virtual void post (CR<INDEX> begin_ ,CR<INDEX> end_) = 0 ;
	virtual void join () = 0 ;
	virtual BOOL join (CR<Time> interval) = 0 ;
	virtual void stop () = 0 ;
} ;

class WorkThread implement OfThis<SharedRef<WorkThreadLayout>> {
public:
	implicit WorkThread () = default ;

	implicit WorkThread (CR<typeof (NULL)>) {
		mThis = WorkThreadHolder::create () ;
		WorkThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CR<LENGTH> size_) const {
		return WorkThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_queue_size (CR<LENGTH> size_) const {
		return WorkThreadHolder::hold (thiz)->set_queue_size (size_) ;
	}

	void start (CR<Function<CR<INDEX>>> func) const {
		return WorkThreadHolder::hold (thiz)->start (func) ;
	}

	void post (CR<INDEX> begin_ ,CR<INDEX> end_) const {
		return WorkThreadHolder::hold (thiz)->post (begin_ ,end_) ;
	}

	void join () const {
		return WorkThreadHolder::hold (thiz)->join () ;
	}

	BOOL join (CR<Time> interval) const {
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
	imports VFat<CalcThreadHolder> hold (VR<CalcThreadLayout> that) ;
	imports CFat<CalcThreadHolder> hold (CR<CalcThreadLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_thread_size (CR<LENGTH> size_) = 0 ;
	virtual void set_start_input (CR<BitSet> input ,CR<FLT64> factor) = 0 ;
	virtual void start (CR<Function<CR<CalcSolution> ,VR<CalcSolution>>> func) = 0 ;
	virtual void friend_execute (CR<INDEX> slot) = 0 ;
	virtual BOOL ready () const = 0 ;
	virtual CalcSolution poll () = 0 ;
	virtual void suspend () = 0 ;
	virtual void resume () = 0 ;
	virtual void stop () = 0 ;
} ;

class CalcThread implement OfThis<SharedRef<CalcThreadLayout>> {
public:
	implicit CalcThread () = default ;

	implicit CalcThread (CR<typeof (NULL)>) {
		mThis = CalcThreadHolder::create () ;
		CalcThreadHolder::hold (thiz)->initialize () ;
	}

	void set_thread_size (CR<LENGTH> size_) const {
		return CalcThreadHolder::hold (thiz)->set_thread_size (size_) ;
	}

	void set_start_input (CR<BitSet> input ,CR<FLT64> factor) const {
		return CalcThreadHolder::hold (thiz)->set_start_input (input ,factor) ;
	}

	void start (CR<Function<CR<CalcSolution> ,VR<CalcSolution>>> func) const {
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
	imports VFat<PromiseHolder> hold (VR<PromiseLayout> that) ;
	imports CFat<PromiseHolder> hold (CR<PromiseLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_retry (CR<BOOL> flag) = 0 ;
	virtual void start () = 0 ;
	virtual void start (CR<Function<>> func) = 0 ;
	virtual void friend_execute (CR<INDEX> slot) = 0 ;
	virtual void post (RR<AutoRef<Pointer>> item) = 0 ;
	virtual void rethrow (CR<Exception> e) = 0 ;
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

	implicit Promise (CR<typeof (NULL)>) {
		mThis = PromiseHolder::create () ;
		PromiseHolder::hold (thiz)->initialize () ;
	}

	void set_retry (CR<BOOL> flag) const {
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
	imports VFat<EntityHolder> hold (VR<EntityLayout> that) ;
	imports CFat<EntityHolder> hold (CR<EntityLayout> that) ;

	virtual void initialize (CR<Clazz> clazz_) = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual INDEX keyid () const = 0 ;
	virtual void add_component (CR<OfThis<SharedRef<ComponentLayout>>> component) = 0 ;
	virtual void register_service (CR<OfThis<SharedRef<ServiceLayout>>> service) = 0 ;
} ;

class Entity implement OfThis<SharedRef<EntityLayout>> {
public:
	implicit Entity () = default ;

	implicit Entity (CR<Clazz> clazz_) {
		mThis = EntityHolder::create () ;
		EntityHolder::hold (thiz)->initialize (clazz_) ;
	}

	Clazz clazz () const {
		return EntityHolder::hold (thiz)->clazz () ;
	}

	INDEX keyid () const {
		return EntityHolder::hold (thiz)->keyid () ;
	}

	void add_component (CR<OfThis<SharedRef<ComponentLayout>>> component) const {
		return EntityHolder::hold (thiz)->add_component (component) ;
	}

	void register_service (CR<OfThis<SharedRef<ServiceLayout>>> service) const {
		return EntityHolder::hold (thiz)->register_service (service) ;
	}
} ;

struct ComponentHolder implement Interface {
	imports SharedRef<ComponentLayout> create () ;
	imports VFat<ComponentHolder> hold (VR<ComponentLayout> that) ;
	imports CFat<ComponentHolder> hold (CR<ComponentLayout> that) ;

	virtual void initialize (CR<Clazz> clazz_) = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual BOOL contain (CR<Clazz> clazz_) const = 0 ;
	virtual OfThis<SharedRef<ComponentLayout>> get (CR<Clazz> clazz_) const = 0 ;
} ;

class Component implement OfThis<SharedRef<ComponentLayout>> {
public:
	implicit Component () = default ;

	implicit Component (CR<Clazz> clazz_) {
		mThis = ComponentHolder::create () ;
		ComponentHolder::hold (thiz)->initialize (clazz_) ;
	}

	Clazz clazz () const {
		return ComponentHolder::hold (thiz)->clazz () ;
	}

	BOOL contain (CR<Clazz> clazz_) const {
		return ComponentHolder::hold (thiz)->contain (clazz_) ;
	}

	Component get (CR<Clazz> clazz_) const {
		OfThis<SharedRef<ComponentLayout>> ret = ComponentHolder::hold (thiz)->get (clazz_) ;
		return move (keep[TYPE<Component>::expr] (ret)) ;
	}
} ;

struct ServiceHolder implement Interface {
	imports SharedRef<ServiceLayout> create () ;
	imports VFat<ServiceHolder> hold (VR<ServiceLayout> that) ;
	imports CFat<ServiceHolder> hold (CR<ServiceLayout> that) ;

	virtual void initialize (CR<Clazz> clazz_) = 0 ;
	virtual Clazz clazz () const = 0 ;
	virtual INDEX spwan_entity () = 0 ;
} ;

class Service implement OfThis<SharedRef<ServiceLayout>> {
public:
	implicit Service () = default ;

	implicit Service (CR<Clazz> clazz_) {
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

struct ManagerLayout ;

struct ManagerHolder implement Interface {
	imports CR<OfThis<SharedRef<ManagerLayout>>> expr_m () ;
	imports VFat<ManagerHolder> hold (VR<ManagerLayout> that) ;
	imports CFat<ManagerHolder> hold (CR<ManagerLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Entity entity (CR<INDEX> index) const = 0 ;
	virtual INDEX entity (CR<Entity> item) = 0 ;
	virtual Component component (CR<INDEX> index) const = 0 ;
	virtual INDEX component (CR<Component> item) = 0 ;
	virtual Service service (CR<INDEX> index) const = 0 ;
	virtual INDEX service (CR<Service> item) = 0 ;
} ;

class Manager implement OfThis<SharedRef<ManagerLayout>> {
public:
	imports CR<Manager> expr_m () {
		return keep[TYPE<Manager>::expr] (ManagerHolder::expr) ;
	}

	Entity entity (CR<INDEX> index) const {
		return ManagerHolder::hold (thiz)->entity (index) ;
	}

	INDEX entity (CR<Entity> item) const {
		return ManagerHolder::hold (thiz)->entity (item) ;
	}

	Component component (CR<INDEX> index) const {
		return ManagerHolder::hold (thiz)->component (index) ;
	}

	INDEX component (CR<Component> item) const {
		return ManagerHolder::hold (thiz)->component (item) ;
	}

	Service service (CR<INDEX> index) const {
		return ManagerHolder::hold (thiz)->service (index) ;
	}

	INDEX service (CR<Service> item) const {
		return ManagerHolder::hold (thiz)->service (item) ;
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
	virtual void until (RR<Ref<BOOL>> flag) = 0 ;
	virtual void execute () = 0 ;
} ;

class Syntax implement OfThis<SharedRef<SyntaxLayout>> {
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

	void until (RR<Ref<BOOL>> flag) {
		return SyntaxHolder::hold (thiz)->until (move (flag)) ;
	}

	void execute () {
		return SyntaxHolder::hold (thiz)->execute () ;
	}
} ;
} ;