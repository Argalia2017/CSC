#pragma once

#ifndef __CSC_DATABASE__
#define __CSC_DATABASE__
#endif

#include "csc.hpp"
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
	exports class Abstract
		:public Interface {
	public:
		virtual void compute_load_data (AnyRef<void> &this_) const = 0 ;
	} ;

	class Pack {
	private:
		friend AbstractDatabase ;
		AnyRef<void> mHolder ;
	} ;

private:
	PhanRef<const Abstract> mAbstract ;
	SharedRef<Pack> mThis ;

public:
	AbstractDatabase () = default ;

	explicit AbstractDatabase (const PhanRef<const Abstract> &abstract_)
		:AbstractDatabase (PhanRef<const Abstract>::make (abstract_) ,SharedRef<Pack>::make ()) {}

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
	explicit AbstractDatabase (PhanRef<const Abstract> &&abstract_ ,SharedRef<Pack> &&this_)
		:mAbstract (std::move (abstract_)) ,mThis (std::move (this_)) {}
} ;
} ;