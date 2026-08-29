#pragma once

#ifndef __CSC_NUMERIC__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_numeric.hpp"

#ifdef ERROR
#pragma push_macro ("ERROR")
#undef ERROR
#endif

#include "csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#include <corecrt_math_defines.h>
#endif

#include <ceres/ceres.h>
#include "csc_begin.h"

#ifndef ERROR
#pragma pop_macro ("ERROR")
#endif

namespace CSC {

} ;