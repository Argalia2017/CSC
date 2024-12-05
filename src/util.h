#pragma once

#include "csc/csc.hpp"
#include "csc/csc_type.hpp"
#include "csc/csc_core.hpp"
#include "csc/csc_basic.hpp"
#include "csc/csc_math.hpp"
#include "csc/csc_array.hpp"
#include "csc/csc_image.hpp"
#include "csc/csc_matrix.hpp"
#include "csc/csc_stream.hpp"
#include "csc/csc_string.hpp"
#include "csc/csc_runtime.hpp"
#include "csc/csc_thread.hpp"

#ifndef DLLEXTERN
#ifdef __CSC_COMPILER_MSVC__
#define DLLEXTERN __declspec (dllexport)
#define DLLAPI __stdcall
#endif

#ifdef __CSC_COMPILER_GNUC__
#define DLLEXTERN __attribute__ ((visibility("default")))
#define DLLAPI
#endif
#endif

namespace SOLUTION {
using namespace CSC ;

using CSC::BOOL ;
using CSC::BYTE ;
using CSC::WORD ;
using CSC::CHAR ;

using CSC::move ;
using CSC::swap ;
} ;