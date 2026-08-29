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
struct PropertyTree {
	Index mSid ;
	String<Str> mName ;
	Length mSize ;
	Length mAlign ;
	Length mChannel ;
	FarBuffer<Pointer> mBuffer ;
} ;

struct PropertyLayout {
	SharedRef<PropertyTree> mThis ;
	Index mIndex ;
} ;

struct PropertyHolder implement Interface {
	imports VFat<PropertyHolder> hold (VR<PropertyLayout> that) ;
	imports CFat<PropertyHolder> hold (CR<PropertyLayout> that) ;

	virtual void initialize () = 0 ;
	virtual Index sid () const = 0 ;
	virtual CR<String<Str>> name () const leftvalue = 0 ;
	virtual Length size () const = 0 ;
	virtual Length align () const = 0 ;
	virtual Length channel () const = 0 ;
	virtual void load (VR<BoxLayout> item) const = 0 ;
	virtual void save (CR<BoxLayout> item) const = 0 ;
	virtual void target (CR<Index> index) = 0 ;
} ;

class Property implement PropertyLayout {
public:
	implicit Property () = default ;

	Index sid () const {
		return PropertyHolder::hold (thiz)->sid () ;
	}

	CR<String<Str>> name () const leftvalue {
		return PropertyHolder::hold (thiz)->name () ;
	}

	Length size () const {
		return PropertyHolder::hold (thiz)->size () ;
	}

	Length align () const {
		return PropertyHolder::hold (thiz)->align () ;
	}

	Length channel () const {
		return PropertyHolder::hold (thiz)->channel () ;
	}

	template <class ARG1>
	ARG1 parse (CR<ARG1> def) const {
		require (IS_TRIVIAL<ARG1>) ;
		auto rax = Box<ARG1>::make (def) ;
		PropertyHolder::hold (thiz)->load (rax) ;
		return move (rax.ref) ;
	}

	template <class ARG1>
	void build (CR<ARG1> item) const {
		require (IS_TRIVIAL<ARG1>) ;
		auto rax = Box<ARG1>::make (item) ;
		PropertyHolder::hold (thiz)->save (rax) ;
	}

	void target (CR<Index> index) {
		return PropertyHolder::hold (thiz)->target (index) ;
	}
} ;

struct BehaviorNode {
	Index mSid ;
	String<Str> mCommand ;
	Array<Index> mParams ;
} ;

struct BehaviorTree {
	Length mBehaviorSid ;
	List<BehaviorNode> mTree ;
} ;

struct BehaviorLayout {
	SharedRef<BehaviorTree> mThis ;
	Index mCheck ;
} ;

struct BehaviorHolder implement Interface {
	imports VFat<BehaviorHolder> hold (VR<BehaviorLayout> that) ;
	imports CFat<BehaviorHolder> hold (CR<BehaviorLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void send (CR<String<Str>> command ,CR<Wrapper<Index>> params) = 0 ;
} ;

class Behavior implement BehaviorLayout {
public:
	implicit Behavior () = default ;

	template <class...ARG1 ,class = REQUIRE<ENUM_ALL<IS_VALUE<ARG1>...>>>
	void send (CR<String<Str>> command ,CR<ARG1>...params) {
		return BehaviorHolder::hold (thiz)->send (command ,MakeWrapper (Index (params))...) ;
	}
} ;

struct DataFrameLayout ;

struct DataFrameHolder implement Interface {
	imports SharedRef<DataFrameLayout> create () ;
	imports VFat<DataFrameHolder> hold (VR<DataFrameLayout> that) ;
	imports CFat<DataFrameHolder> hold (CR<DataFrameLayout> that) ;

	virtual void initialize (CR<Length> size_) = 0 ;
	virtual Length size () const = 0 ;
	virtual Length length () const = 0 ;
	virtual Index ibegin () const = 0 ;
	virtual Index iend () const = 0 ;
	virtual Index inext (CR<Index> index) const = 0 ;
	virtual void claim (CR<String<Str>> name ,CR<Clazz> clazz ,RR<FarBuffer<Pointer>> buffer) = 0 ;
	virtual Property map (CR<Index> sid ,CR<Index> index) const = 0 ;
	virtual Property map (CR<String<Str>> name ,CR<Index> index) const = 0 ;
	virtual Bool contain (CR<Index> sid) const = 0 ;
	virtual Bool contain (CR<String<Str>> name) const = 0 ;
	virtual Behavior record () = 0 ;
	virtual void submit () = 0 ;
	virtual void spawn (CR<Index> begin_ ,CR<Index> end_) = 0 ;
	virtual void despawn (CR<Index> begin_ ,CR<Index> end_) = 0 ;
} ;

class DataFrame implement Super<SharedRef<DataFrameLayout>> {
public:
	implicit DataFrame () = default ;

	implicit DataFrame (CR<Length> size_) {
		mThis = DataFrameHolder::create () ;
		DataFrameHolder::hold (thiz)->initialize (size_) ;
	}

	Length size () const {
		return DataFrameHolder::hold (thiz)->size () ;
	}

	Length length () const {
		return DataFrameHolder::hold (thiz)->length () ;
	}

	Index ibegin () const {
		return DataFrameHolder::hold (thiz)->ibegin () ;
	}

	Index iend () const {
		return DataFrameHolder::hold (thiz)->iend () ;
	}

	Index inext (CR<Index> index) const {
		return DataFrameHolder::hold (thiz)->inext (index) ;
	}

	ArrayRange<CR<DataFrame>> iter () const leftvalue {
		return ArrayRange<CR<DataFrame>> (thiz) ;
	}

	template <class ARG1>
	void claim (CR<String<Str>> name ,RR<FarBuffer<ARG1>> buffer) {
		const auto r1x = Clazz (TYPE<ARG1>::expr) ;
		auto &&rax = keep[TYPE<FarBuffer<Pointer>>::expr] (Pointer::from (buffer)) ;
		return DataFrameHolder::hold (thiz)->claim (name ,r1x ,move (rax)) ;
	}

	Property map (CR<Index> sid ,CR<Index> index) const {
		return DataFrameHolder::hold (thiz)->map (sid ,index) ;
	}

	Property map (CR<String<Str>> name ,CR<Index> index) const {
		return DataFrameHolder::hold (thiz)->map (name ,index) ;
	}

	Bool contain (CR<Index> sid) const {
		return DataFrameHolder::hold (thiz)->contain (sid) ;
	}

	Bool contain (CR<String<Str>> name) const {
		return DataFrameHolder::hold (thiz)->contain (name) ;
	}

	Behavior record () {
		return DataFrameHolder::hold (thiz)->record () ;
	}

	void submit () {
		return DataFrameHolder::hold (thiz)->submit () ;
	}

	void spawn (CR<Index> begin ,CR<Index> end_) {
		return DataFrameHolder::hold (thiz)->spawn (begin ,end_) ;
	}

	void despawn (CR<Index> begin ,CR<Index> end_) {
		return DataFrameHolder::hold (thiz)->despawn (begin ,end_) ;
	}
} ;
} ;