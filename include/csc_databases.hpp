#pragma once

#ifndef __CSC_DATABASES__
#define __CSC_DATABASES__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_thread.hpp"
#include "csc_filesystem.hpp"
#include "csc_network.hpp"

namespace CSC {
class AbstractDatabase {
public:
	class Engine :public Interface {} ;

private:
	PhanRef<const Engine> mEngine ;
	AnyRef<void> mHolder ;

public:
	AbstractDatabase () = default ;

	explicit AbstractDatabase (const Engine &engine) {
		mEngine = PhanRef<const Engine>::make (&engine) ;
	}
} ;
} ;