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
		virtual void compute_load_data (AnyRef<void> &holder) const = 0 ;
	} ;

	struct SELF_PACK {
		AnyRef<void> mHolder ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	SharedRef<SELF_PACK> mThis ;

public:
	implicit AbstractDatabase () = default ;

	explicit AbstractDatabase (PhanRef<const Abstract> &&abstract_) {
		mAbstract = _MOVE_ (abstract_) ;
		mThis = SharedRef<SELF_PACK>::make () ;
	}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}
} ;
} ;