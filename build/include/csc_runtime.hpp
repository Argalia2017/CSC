#pragma once

#ifndef __CSC_RUNTIME__
#define __CSC_RUNTIME__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"

namespace CSC {
namespace stl {
using std::exit ;
using std::atexit ;
using std::abort ;
using std::terminate ;
using std::set_terminate ;
} ;

#if defined (_CSIGNAL_) || defined (_GLIBCXX_CSIGNAL)
namespace stl {
using std::signal ;
using std::raise ;
} ;
#endif
} ;