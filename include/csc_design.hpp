#pragma once

#ifndef __CSC_DESIGN__
#define __CSC_DESIGN__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_serialize.hpp"
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"
#include "csc_network.hpp"
#include "csc_databases.hpp"
#include "csc_debugger.hpp"

namespace CSC {
/*
*class SingletonPattern ;
Singleton<T>::instance

#class FactoryPattern ;
FactoryClass->(Factory{ObjectClass})->Object

%class BuilderPattern ;
A=create(B)->(B=create()=>>B.set1()=>>B.set2())

*class PrototypePattern ;
copy-on-write

*class ProxyPattern ;
BitSet<>::Bit|Matrix<T>::Row

*class DocoratorPattern ;
ForwardIterator::IterableArray

*class AdapterPattern ;
Function<BOOL ()>::make (&_COPY_<BOOL> ,FALSE)

*class CompositePattern ;
XmlParser::make|JsonParser::make

*class FacadePattern ;
class DEF<void BGFSAlgorithm::initialize ()>::Lambda

*class FlyweightPattern ;
TextReader<T>::Attribute

*class BridgePattern ;
PimplImage<-(PIMPLIMAGE_ENGINE_OPENCV|PIMPLIMAGE_ENGINE_FREEIMAGE)

#class CommandPattern ;
std::system|std::async

*class HandlerPattern ;
F(A)->F1(A)->F2(A)->F3(A)-x>F4(A)

*class MediatorPattern ;
Broadcaster<T>::(std::condition_variable)

*class StrategyPattern ;
OPERATOR_CONVERT<T1 ,T2>::invoke

*class IteratorPattern ;
ForwardIterator|XmlParser::brother

*class TemplateMethodPattern ;
active->(onBeforeActive=>>onActive=>>onAfterActive)

#class ObserverPattern ;
(Signal->Connection->Slot)

*class MementoPattern ;
B=copy(A)=>>B.operator()=>>A=move(B)

*class VisitorPattern ;
operator>>(TextReader ,A)|operator<<(TextWriter ,A)

#class StatePattern ;
(State(0)->Transition(0 ,1)->State(1))

*class InterpreterPattern ;
XmlParser::initialize::update_shift_e0
*/
} ;

namespace CSC {
#ifdef __CSC_DEPRECATED__
class FactoryPattern :public Object {
public:
	FactoryPattern () :Object (this) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;

class CommandPattern :public Object {
public:
	CommandPattern () :Object (this) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;

class ObserverPattern :public Object {
public:
	class Signal :public Interface {
	public:
		virtual void active () = 0 ;

		virtual StrongRef<ObserverPattern> getContext () = 0 ;
		virtual void setParameter (AnyRef<void> &&param) = 0 ;
		virtual AnyRef<void> getResult () const = 0 ;
	} ;

	class Connection :public Interface {
	public:
		virtual void delay () = 0 ;
		virtual void inactive () = 0 ;

		virtual StrongRef<ObserverPattern> getContext () = 0 ;
		virtual const AnyRef<void> &getParameter () const = 0 ;
		virtual void setResult (AnyRef<void> &&result) = 0 ;

	protected:
		virtual void onAttach () = 0 ;
		virtual void onDetach () = 0 ;
	} ;

	class Slot :public Interface {
	public:
		virtual StrongRef<ObserverPattern> getContext () = 0 ;

	protected:
		virtual void onActive (const StrongRef<Connection> &connection) = 0 ;
	} ;

private:
	QList<WeakRef<Signal>> mSignalList ;
	QList<WeakRef<Slot>> mSlotList ;
	QList<WeakRef<Connection>> mConnectionList ;

public:
	ObserverPattern () :Object (this) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;

class StatePattern :public Object {
public:
	StatePattern () :Object (this) {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
	}
} ;
#endif
} ;