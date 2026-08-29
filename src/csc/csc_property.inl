#pragma once

#ifndef __CSC_PROPERTY__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_property.hpp"

namespace CSC {
class PropertyImplHolder final implement Fat<PropertyHolder ,PropertyLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Index sid () const override {
		if (!self.mThis.exist ())
			return NONE ;
		return self.mThis->mSid ;
	}

	CR<String<Str>> name () const leftvalue override {
		if (!self.mThis.exist ())
			return String<Str>::zero () ;
		return self.mThis->mName ;
	}

	Length size () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mBuffer.size () ;
	}

	Length align () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mAlign ;
	}

	Length channel () const override {
		if (!self.mThis.exist ())
			return 0 ;
		return self.mThis->mChannel ;
	}

	void load (VR<BoxLayout> item) const override {
		const auto r1x = align () ;
		const auto r2x = channel () * r1x ;
		const auto r3x = RFat<ReflectSize> (BoxHolder::hold (item)->unknown ()) ;
		if (r2x != r3x->type_size ())
			return ;
		if (r1x != r3x->type_align ())
			return ;
		inline_memcpy (BoxHolder::hold (item)->ref ,self.mThis->mBuffer[self.mIndex] ,r2x) ;
		self.mThis->mBuffer.refresh () ;
	}

	void save (CR<BoxLayout> item) const override {
		const auto r1x = align () ;
		const auto r2x = channel () * r1x ;
		const auto r3x = RFat<ReflectSize> (BoxHolder::hold (item)->unknown ()) ;
		if (r2x != r3x->type_size ())
			return ;
		if (r1x != r3x->type_align ())
			return ;
		inline_memcpy (self.mThis->mBuffer[self.mIndex] ,BoxHolder::hold (item)->ref ,r2x) ;
		self.mThis->mBuffer.refresh () ;
	}

	void target (CR<Index> index) override {
		assume (inline_mid (index ,0 ,size ())) ;
		self.mIndex = index ;
	}
} ;

exports VFat<PropertyHolder> PropertyHolder::hold (VR<PropertyLayout> that) {
	return VFat<PropertyHolder> (PropertyImplHolder () ,that) ;
}

exports CFat<PropertyHolder> PropertyHolder::hold (CR<PropertyLayout> that) {
	return CFat<PropertyHolder> (PropertyImplHolder () ,that) ;
}

class BehaviorImplHolder final implement Fat<BehaviorHolder ,BehaviorLayout> {
public:
	void initialize () override {
		noop () ;
	}

	void send (CR<String<Str>> command ,CR<Wrapper<Index>> params) override {
		if (!self.mThis.exist ())
			return ;
		Index ix = self.mThis->mTree.insert () ;
		self.mThis->mTree[ix].mSid = self.mThis->mBehaviorSid ;
		self.mThis->mBehaviorSid++ ;
		self.mThis->mBehaviorSid = inline_max (self.mThis->mBehaviorSid ,0) ;
		self.mThis->mTree[ix].mCommand = command ;
		self.mThis->mTree[ix].mParams = Array<Index> (params.rank ()) ;
		for (auto &&i : range (0 ,params.rank ())) {
			self.mThis->mTree[ix].mParams[i] = params[i] ;
		}
	}
} ;

exports VFat<BehaviorHolder> BehaviorHolder::hold (VR<BehaviorLayout> that) {
	return VFat<BehaviorHolder> (BehaviorImplHolder () ,that) ;
}

exports CFat<BehaviorHolder> BehaviorHolder::hold (CR<BehaviorLayout> that) {
	return CFat<BehaviorHolder> (BehaviorImplHolder () ,that) ;
}

struct DataFrameLayout {
	BitSet mDataFrame ;
	Bool mConstructed ;
	ArrayList<SharedRef<PropertyTree>> mProperty ;
	Set<String<Str>> mPropertyName ;
	SharedRef<BehaviorTree> mBehavior ;
	Index mBehaviorCheck ;
} ;

class DataFrameImplHolder final implement Fat<DataFrameHolder ,DataFrameLayout> {
public:
	void initialize (CR<Length> size_) override {
		self.mDataFrame = BitSet (size_) ;
		self.mConstructed = FALSE ;
		self.mBehavior = SharedRef<BehaviorTree>::make () ;
		self.mBehavior->mBehaviorSid = 0 ;
	}

	Length size () const override {
		return self.mDataFrame.size () ;
	}

	Length length () const override {
		return self.mDataFrame.length () ;
	}

	Index ibegin () const override {
		return self.mDataFrame.ibegin () ;
	}

	Index iend () const override {
		return self.mDataFrame.iend () ;
	}

	Index inext (CR<Index> index) const override {
		return self.mDataFrame.inext (index) ;
	}

	void claim (CR<String<Str>> name ,CR<Clazz> clazz ,RR<FarBuffer<Pointer>> buffer) override {
		assert (!self.mConstructed) ;
		Index ix = self.mPropertyName.map (name) ;
		assume (ix == NONE) ;
		ix = self.mProperty.insert () ;
		self.mProperty[ix] = SharedRef<PropertyTree>::make () ;
		self.mProperty[ix]->mSid = ix ;
		self.mProperty[ix]->mName = name ;
		self.mPropertyName.add (self.mProperty[ix]->mName ,ix) ;
		const auto r1x = clazz.type_size () ;
		const auto r2x = clazz.type_align () ;
		self.mProperty[ix]->mSize = size () ;
		self.mProperty[ix]->mAlign = r2x ;
		self.mProperty[ix]->mChannel = r1x / r2x ;
		self.mProperty[ix]->mBuffer = move (buffer) ;
	}

	Property map (CR<Index> sid ,CR<Index> index) const override {
		Index ix = sid ;
		assume (contain (ix)) ;
		assume (self.mDataFrame.contain (index)) ;
		Property ret ;
		ret.mThis = self.mProperty[ix] ;
		ret.mIndex = index ;
		return move (ret) ;
	}

	Property map (CR<String<Str>> name ,CR<Index> index) const override {
		Index ix = self.mPropertyName.map (name) ;
		assume (ix != NONE) ;
		assume (self.mDataFrame.contain (index)) ;
		Property ret ;
		ret.mThis = self.mProperty[ix] ;
		ret.mIndex = index ;
		return move (ret) ;
	}

	Bool contain (CR<Index> sid) const override {
		if (!inline_mid (sid ,0 ,self.mProperty.size ()))
			return FALSE ;
		if (self.mProperty.is_slot (sid))
			return FALSE ;
		return TRUE ;
	}

	Bool contain (CR<String<Str>> name) const override {
		return self.mPropertyName.contain (name) ;
	}

	Behavior record () override {
		Behavior ret ;
		ret.mThis = self.mBehavior ;
		ret.mCheck = self.mBehaviorCheck ;
		return move (ret) ;
	}

	void submit () override {
		unimplemented () ;
	}

	void spawn (CR<Index> begin_ ,CR<Index> end_) override {
		assert (begin_ >= 0) ;
		assert (end_ >= 0) ;
		assert (begin_ <= end_) ;
		const auto r1x = inline_max (end_ ,self.mDataFrame.size ()) ;
		self.mDataFrame.resize (r1x) ;
		for (auto &&i : range (begin_ ,end_))
			self.mDataFrame.add (i) ;
	}

	void despawn (CR<Index> begin_ ,CR<Index> end_) override {
		assert (begin_ >= 0) ;
		assert (end_ >= 0) ;
		assert (begin_ <= end_) ;
		const auto r1x = inline_min (end_ ,self.mDataFrame.size ()) ;
		for (auto &&i : range (begin_ ,r1x))
			self.mDataFrame.erase (i) ;
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