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
class SQLDatabase {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract :
		delegate public Interface {
	public:
		virtual void load_data () = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit SQLDatabase () ;
} ;
} ;