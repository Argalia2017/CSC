#pragma once

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"

namespace CSC {
class BoxImplHolder implement BoxHolder {
protected:
	XPTR<BoxLayout> mThis ;

public:
	void initialize () override {
		
	}

	void destroy () override {
		mThis->mExist ;
	}
} ;

exports VFat<BoxHolder> BoxHolder::create (VREF<BoxLayout> that) {
	return Pointer::make (0) ;
}

exports CFat<BoxHolder> BoxHolder::create (CREF<BoxLayout> that) {
	return Pointer::make (0) ;
}
} ;