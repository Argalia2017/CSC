#pragma once

#include <csc.hpp>
#include <csc_type.hpp>
#include <csc_core.hpp>
#include <csc_basic.hpp>
#include <csc_math.hpp>
#include <csc_array.hpp>
#include <csc_image.hpp>
#include <csc_matrix.hpp>
#include <csc_stream.hpp>
#include <csc_string.hpp>
#include <csc_runtime.hpp>
#include <csc_file.hpp>
#include <csc_thread.hpp>

namespace ROUTINE {
using namespace CSC ;

using CSC::move ;
using CSC::swap ;

#ifndef CLOG
#define CLOG Singleton<Console>::expr
#endif
} ;