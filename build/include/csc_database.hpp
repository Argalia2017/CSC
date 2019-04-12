#pragma once

#ifndef __CSC_DATABASE__
#define __CSC_DATABASE__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
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
public:
	export struct Abstract :public Interface {
		virtual void load_data (AnyRef<void> &_this) = 0 ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	AnyRef<void> mHolder ;

public:
	AbstractDatabase () = default ;

	explicit AbstractDatabase (const PhanRef<const Abstract> &engine) :mAbstract (PhanRef<const Abstract>::make (engine)) {}

	BOOL exist () const {
		return mAbstract.exist () && mHolder.exist () ;
	}
} ;
} ;