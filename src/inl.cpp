#include "util.h"

#include "csc/csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif
#include "csc/csc_begin.h"

#include "csc/csc_core.inl"
#include "csc/csc_basic.inl"
#include "csc/csc_array.inl"
#include "csc/csc_math.inl"
#include "csc/csc_image.inl"
#include "csc/csc_matrix.inl"
#include "csc/csc_stream.inl"
#include "csc/csc_runtime.inl"
#include "csc/csc_thread.inl"

#ifdef __CSC_SYSTEM_WINDOWS__
#include  "csc/csc_runtime.windows.inl"
#endif

#ifdef __CSC_COMPILER_MSVC__
#include "csc/csc_image.opencv.inl"
#endif

#include "csc/csc_matrix.eigen.inl"