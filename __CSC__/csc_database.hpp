#pragma once

#ifndef __CSC_DATABASE__
#define __CSC_DATABASE__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"
#include "csc_network.hpp"

namespace CSC {
class AbstractDatabase {
private:
	class Abstract
		:public Interface {
	public:
		virtual void compute_load_data (AnyRef<> &holder) const = 0 ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	AnyRef<> mHolder ;

public:
	implicit AbstractDatabase () = default ;

	explicit AbstractDatabase (PhanRef<const Abstract> &&abstract_) {
		mAbstract = _MOVE_ (abstract_) ;
	}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}
} ;
} ;