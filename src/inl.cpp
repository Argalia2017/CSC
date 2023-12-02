#include "util.h"

#include "csc/csc_end.h"
#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4091)
#pragma warning (disable :4996)
#pragma warning (disable :5039)
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif

#include <iostream>
#include "csc/csc_begin.h"

#include "csc/csc_core.inl"
#include "csc/csc_basic.inl"
#include "csc/csc_array.inl"
#include "csc/csc_math.inl"
#include "csc/csc_matrix.inl"