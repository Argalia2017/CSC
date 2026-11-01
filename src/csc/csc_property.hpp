#pragma once

#ifndef __CSC_PROPERTY__
#define __CSC_PROPERTY__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_image.hpp"
#include "csc_matrix.hpp"
#include "csc_numeric.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_file.hpp"
#include "csc_thread.hpp"

namespace CSC {
struct PropertyTree ;

struct PropertyLayout {
	SharedRef<PropertyTree> mThis ;
	Array<Index> mChild ;
} ;

struct PropertyHolder implement Interface {
	imports VFat<PropertyHolder> hold (VR<PropertyLayout> that) ;
	imports CFat<PropertyHolder> hold (CR<PropertyLayout> that) ;

	virtual void initialize (CR<Clazz> clazz ,RR<PropertyLayout> that) = 0 ;
	virtual Length size () const = 0 ;
	virtual VR<Index> at (CR<Index> index) leftvalue = 0 ;
	virtual CR<Index> at (CR<Index> index) const leftvalue = 0 ;
	virtual void fetch (VR<Pointer> item) const = 0 ;
	virtual void store (CR<Pointer> item) const = 0 ;
} ;

template <class A>
class Property implement PropertyLayout {
private:
	require (IS_TRIVIAL<A>) ;

public:
	using PropertyLayout::mThis ;
	using PropertyLayout::mChild ;

public:
	implicit Property () = default ;

	implicit Property (RR<PropertyLayout> that) {
		PropertyHolder::hold (thiz)->initialize (Clazz (TYPE<A>::expr) ,move (that)) ;
	}

	Length size () const {
		return PropertyHolder::hold (thiz)->size () ;
	}

	VR<Index> at (CR<Index> index) leftvalue {
		return PropertyHolder::hold (thiz)->at (index) ;
	}

	forceinline VR<Index> operator[] (CR<Index> index) leftvalue {
		return at (index) ;
	}

	CR<Index> at (CR<Index> index) const leftvalue {
		return PropertyHolder::hold (thiz)->at (index) ;
	}

	forceinline CR<Index> operator[] (CR<Index> index) const leftvalue {
		return at (index) ;
	}

	A fetch () const {
		A ret ;
		PropertyHolder::hold (thiz)->fetch (Pointer::from (ret)) ;
		return move (ret) ;
	}

	void store (CR<A> item) const {
		PropertyHolder::hold (thiz)->store (Pointer::from (item)) ;
	}
} ;

struct BehaviorTree ;

struct BehaviorLayout {
	SharedRef<BehaviorTree> mThis ;
} ;

struct BehaviorHolder implement Interface {
	imports VFat<BehaviorHolder> hold (VR<BehaviorLayout> that) ;
	imports CFat<BehaviorHolder> hold (CR<BehaviorLayout> that) ;

	virtual void initialize (RR<BehaviorLayout> that) = 0 ;
	virtual void send (CR<Flag> message ,CR<Format> command) = 0 ;
	virtual void awake () = 0 ;
	virtual Bool tick (CR<Flt64> delta_sec) = 0 ;
	virtual void idle_tick (CR<Flt64> delta_sec) = 0 ;
	virtual Bool alive () = 0 ;
} ;

class BehaviorCoroutineBinder final implement Fat<CoroutineHolder ,BehaviorLayout> {
public:
	void awake () override {
		return BehaviorHolder::hold (self)->awake () ;
	}

	Bool tick (CR<Flt64> delta_sec) override {
		return BehaviorHolder::hold (self)->tick (delta_sec) ;
	}

	void idle_tick (CR<Flt64> delta_sec) override {
		return BehaviorHolder::hold (self)->idle_tick (delta_sec) ;
	}

	Bool alive () override {
		return BehaviorHolder::hold (self)->alive () ;
	}
} ;

class Behavior implement BehaviorLayout {
protected:
	using BehaviorLayout::mThis ;

public:
	implicit Behavior () = default ;

	implicit Behavior (RR<BehaviorLayout> that) {
		BehaviorHolder::hold (thiz)->initialize (move (that)) ;
	}

	void send (CR<Flag> message ,CR<Format> command) {
		return BehaviorHolder::hold (thiz)->send (message ,command) ;
	}

	forceinline operator Coroutine () leftvalue {
		return VFat<CoroutineHolder> (BehaviorCoroutineBinder () ,thiz) ;
	}
} ;

struct DataFrameLayout ;

struct DataFrameHolder implement Interface {
	imports SharedRef<DataFrameLayout> create () ;
	imports VFat<DataFrameHolder> hold (VR<DataFrameLayout> that) ;
	imports CFat<DataFrameHolder> hold (CR<DataFrameLayout> that) ;

	virtual void initialize (CR<Length> rank_) = 0 ;
	virtual Length rank () const = 0 ;
	virtual Array<Slice> shape () const = 0 ;
	virtual Length shape (CR<Index> index) const = 0 ;
	virtual void claim (CR<String<Str>> name ,CR<Clazz> clazz) = 0 ;
	virtual void spawn (CR<Index> rank_ ,CR<Length> size_) = 0 ;
	virtual PropertyLayout map (CR<String<Str>> filter) = 0 ;
	virtual BehaviorLayout record () = 0 ;
	virtual void submit () = 0 ;
	virtual RefBuffer<Byte> serialize () = 0 ;
} ;

class DataFrame implement Super<SharedRef<DataFrameLayout>> {
public:
	implicit DataFrame () = default ;

	implicit DataFrame (CR<Length> rank_) {
		mThis = DataFrameHolder::create () ;
		DataFrameHolder::hold (thiz)->initialize (rank_) ;
	}

	Length rank () const {
		return DataFrameHolder::hold (thiz)->rank () ;
	}

	Array<Slice> shape () const {
		return DataFrameHolder::hold (thiz)->shape () ;
	}

	Length shape (CR<Index> index) const {
		return DataFrameHolder::hold (thiz)->shape (index) ;
	}

	void claim (CR<String<Str>> name ,CR<Clazz> clazz) const {
		return DataFrameHolder::hold (thiz)->claim (name ,clazz) ;
	}

	void spawn (CR<Index> rank_ ,CR<Length> size_) const {
		return DataFrameHolder::hold (thiz)->spawn (rank_ ,size_) ;
	}

	PropertyLayout map (CR<String<Str>> filter) const {
		return DataFrameHolder::hold (thiz)->map (filter) ;
	}

	BehaviorLayout record () const {
		return DataFrameHolder::hold (thiz)->record () ;
	}

	void submit () const {
		return DataFrameHolder::hold (thiz)->submit () ;
	}

	RefBuffer<Byte> serialize () {
		return DataFrameHolder::hold (thiz)->serialize () ;
	}
} ;
} ;