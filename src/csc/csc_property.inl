#pragma once

#ifndef __CSC_PROPERTY__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_property.hpp"

namespace CSC {
struct PropertyTree {
	String<Str> mName ;
	Clazz mClazz ;
	FarBuffer<Pointer> mBuffer ;
} ;

class PropertyImplHolder final implement Fat<PropertyHolder ,PropertyLayout> {
public:
	void initialize (CR<Clazz> clazz ,RR<PropertyLayout> that) override {
		self.mThis = move (that.mThis) ;
		self.mChild = move (that.mChild) ;
		assume (self.mThis.exist ()) ;
		assume (self.mThis->mClazz == clazz) ;
	}

	Length size () const override {
		return self.mChild.size () ;
	}

	VR<Index> at (CR<Index> index) leftvalue override {
		return self.mChild[index] ;
	}

	CR<Index> at (CR<Index> index) const leftvalue override {
		return self.mChild[index] ;
	}

	void fetch (VR<Pointer> item) const override {
		unimplemented () ;
	}

	void store (CR<Pointer> item) const override {
		unimplemented () ;
	}
} ;

exports VFat<PropertyHolder> PropertyHolder::hold (VR<PropertyLayout> that) {
	return VFat<PropertyHolder> (PropertyImplHolder () ,that) ;
}

exports CFat<PropertyHolder> PropertyHolder::hold (CR<PropertyLayout> that) {
	return CFat<PropertyHolder> (PropertyImplHolder () ,that) ;
}

struct BehaviorNode {
	Index mOrder ;
	Flag mMessage ;
	String<Str> mCommand ;
} ;

struct BehaviorTree {
	Index mBehaviorCheck ;
	Index mBehaviorOrder ;
	List<BehaviorNode> mTree ;
} ;

class BehaviorImplHolder final implement Fat<BehaviorHolder ,BehaviorLayout> {
public:
	void initialize (RR<BehaviorLayout> that) override {
		self.mThis = move (that.mThis) ;
	}

	void send (CR<Flag> message ,CR<Format> command) override {
		assert (self.mThis.exist ()) ;
		Index ix = self.mThis->mTree.insert () ;
		self.mThis->mTree[ix].mOrder = self.mThis->mBehaviorOrder ;
		self.mThis->mBehaviorOrder++ ;
		self.mThis->mBehaviorOrder = inline_max (self.mThis->mBehaviorOrder ,0) ;
		self.mThis->mTree[ix].mMessage = message ;
		self.mThis->mTree[ix].mCommand = String<Str>::make (command) ;
	}

	void awake () override {
		unimplemented () ;
	}

	Bool tick (CR<Flt64> delta_sec) override {
		unimplemented () ;
		return FALSE ;
	}

	void idle_tick (CR<Flt64> delta_sec) override {
		unimplemented () ;
	}

	Bool alive () override {
		unimplemented () ;
		return FALSE ;
	}
} ;

exports VFat<BehaviorHolder> BehaviorHolder::hold (VR<BehaviorLayout> that) {
	return VFat<BehaviorHolder> (BehaviorImplHolder () ,that) ;
}

exports CFat<BehaviorHolder> BehaviorHolder::hold (CR<BehaviorLayout> that) {
	return CFat<BehaviorHolder> (BehaviorImplHolder () ,that) ;
}

struct DataFrameLayout {
	Array<Slice> mShape ;
	ArrayList<SharedRef<PropertyTree>> mProperty ;
	Set<String<Str>> mPropertyName ;
	SharedRef<BehaviorTree> mBehavior ;
} ;

class DataFrameImplHolder final implement Fat<DataFrameHolder ,DataFrameLayout> {
public:
	void initialize (CR<Length> rank_) override {
		self.mShape = Array<Slice> (rank_) ;
		self.mBehavior = SharedRef<BehaviorTree>::make () ;
	}

	Length rank () const override {
		return self.mShape.size () ;
	}

	Array<Slice> shape () const override {
		return self.mShape ;
	}

	Length shape (CR<Index> index) const override {
		return self.mShape[index].size () ;
	}

	void claim (CR<String<Str>> name ,CR<Clazz> clazz) override {
		Index ix = self.mPropertyName.map (name) ;
		if ifdo (TRUE) {
			if (ix != NONE)
				discard ;
			ix = self.mProperty.insert () ;
			self.mProperty[ix] = SharedRef<PropertyTree>::make () ;
			self.mProperty[ix]->mName = name ;
			self.mPropertyName.add (self.mProperty[ix]->mName ,ix) ;
			self.mProperty[ix]->mClazz = clazz ;
		}
	}

	void spawn (CR<Index> rank_ ,CR<Length> size_) override {
		self.mShape[rank_] = Slice (0 ,size_ ,1) ;
	}

	PropertyLayout map (CR<String<Str>> filter) override {
		PropertyLayout ret ;
		if ifdo (TRUE) {
			Index ix = self.mPropertyName.map (filter) ;
			if (ix == NONE)
				discard ;
			ret.mThis = self.mProperty[ix] ;
			ret.mChild = Array<Index> (rank ()) ;
		}
		return move (ret) ;
	}

	BehaviorLayout record () override {
		BehaviorLayout ret ;
		ret.mThis = self.mBehavior ;
		return move (ret) ;
	}

	void submit () override {
		auto rax = self.mBehavior ;
		rax->mBehaviorOrder = 0 ;
		rax->mBehaviorCheck++ ;
		rax->mBehaviorCheck = inline_max (rax->mBehaviorCheck ,1) ;
		rax->mTree.clear () ;
	}

	RefBuffer<Byte> serialize () override {
		unimplemented () ;
		return RefBuffer<Byte> () ;
	}
} ;

exports SharedRef<DataFrameLayout> DataFrameHolder::create () {
	return SharedRef<DataFrameLayout>::make () ;
}

exports VFat<DataFrameHolder> DataFrameHolder::hold (VR<DataFrameLayout> that) {
	return VFat<DataFrameHolder> (DataFrameImplHolder () ,that) ;
}

exports CFat<DataFrameHolder> DataFrameHolder::hold (CR<DataFrameLayout> that) {
	return CFat<DataFrameHolder> (DataFrameImplHolder () ,that) ;
}
} ;