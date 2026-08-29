#include "util.h"

#include <csc_end.h>
#define _CRT_SECURE_NO_WARNINGS
#ifdef __CSC_SYSTEM_WINDOWS__
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#endif
#include <csc_begin.h>

#include <csc_core.inl>
#include <csc_basic.inl>
#include <csc_math.inl>
#include <csc_array.inl>
#include <csc_image.inl>
#include <csc_matrix.inl>
#include <csc_numeric.inl>
#include <csc_stream.inl>
#include <csc_string.inl>
#include <csc_runtime.inl>
#include <csc_file.inl>
#include <csc_thread.inl>
#include <csc_property.inl>
#include <csc_algorithm.inl>
#include <csc_spatial.inl>

#ifdef __CSC_COMPILER_MSVC__
#ifdef __CSC_PLATFORM_X64__
#define LIB_WITH_EIGEN
#define LIB_WITH_CERES
#define LIB_WITH_NANOFLANN

#ifdef __CSC_VER_DEBUG__
#define LIB_WITH_FREEIMAGE
#endif

#ifndef __CSC_VER_DEBUG__
#define LIB_WITH_OPENCV
#endif
#endif
#endif

#include <csc_math.cache.inl>

#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_runtime.windows.inl>
#include <csc_file.windows.inl>
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <csc_runtime.linux.inl>
#include <csc_file.linux.inl>
#endif

#ifdef __CSC_SYSTEM_EMBEDDED__
#include <csc_runtime.embedded.inl>
#include <csc_file.embedded.inl>
#endif

#ifdef LIB_WITH_EIGEN
#include <csc_matrix.eigen.inl>
#endif

#ifdef LIB_WITH_CERES
#include <csc_numeric.ceres.inl>
#ifdef __CSC_COMPILER_MSVC__
#pragma comment (lib ,"ceres.lib")
#endif
#endif

#ifdef LIB_WITH_NANOFLANN
#include <csc_spatial.nanoflann.inl>
#endif

#ifdef LIB_WITH_OPENCV
#include <csc_image.opencv.inl>
#ifdef __CSC_COMPILER_MSVC__
#pragma comment (lib ,"opencv_world4120.lib")
#endif
#endif

#ifdef LIB_WITH_FREEIMAGE
#include <csc_image.freeimage.inl>
#ifdef __CSC_COMPILER_MSVC__
#pragma comment (lib ,"FreeImage.lib")
#endif
#endif