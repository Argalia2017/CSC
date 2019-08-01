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
	exports struct Abstract :public Interface {
		virtual void compute_load_data (AnyRef<void> &_this) const = 0 ;
	} ;

	class Holder {
	private:
		friend AbstractDatabase ;
		AnyRef<void> mHolder ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	SharedRef<Holder> mThis ;

public:
	AbstractDatabase () = default ;

	explicit AbstractDatabase (const PhanRef<const Abstract> &_abstract) :AbstractDatabase (PhanRef<const Abstract>::make (_abstract) ,SharedRef<Holder>::make ()) {}

	BOOL exist () const {
		if (!mAbstract.exist ())
			return FALSE ;
		if (!mThis.exist ())
			return FALSE ;
		if (!mThis->mHolder.exist ())
			return FALSE ;
		return TRUE ;
	}

private:
	explicit AbstractDatabase (PhanRef<const Abstract> &&_abstract ,SharedRef<Holder> &&_this) :mAbstract (std::move (_abstract)) ,mThis (std::move (_this)) {}
} ;
} ;