#pragma once

#include <csc.hpp>
#include <csc_core.hpp>
#include <csc_basic.hpp>
#include <csc_extend.hpp>
#include <csc_array.hpp>
#include <csc_math.hpp>
#include <csc_images.hpp>
#include <csc_geometry.hpp>
#include <csc_algorithm.hpp>
#include <csc_functional.hpp>
#include <csc_stream.hpp>
#include <csc_string.hpp>
#include <csc_serialization.hpp>
#include <csc_runtime.hpp>
#include <csc_thread.hpp>
#include <csc_graphics.hpp>
#include <csc_filesystem.hpp>
#include <csc_network.hpp>
#include <csc_database.hpp>
#include <csc_debugger.hpp>

#ifdef __CSC_SYSTEM_WINDOWS__
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#pragma warning (disable :4996)
#pragma warning (disable :5039)
#pragma warning (disable :26429)
#pragma warning (disable :26432)
#pragma warning (disable :26433)
#pragma warning (disable :26439)
#pragma warning (disable :26440)
#pragma warning (disable :26443)
#pragma warning (disable :26455)
#pragma warning (disable :26461)
#pragma warning (disable :26466)
#pragma warning (disable :26485)
#pragma warning (disable :26490)
#pragma warning (disable :26495)
#pragma warning (disable :26496)
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#ifdef __CSC_COMPILER_MSVC__
#ifdef __CSC_TARGET_DLL__
#include <CppUnitTest.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework ;
#endif
#endif
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif
#endif

#ifndef TEST_CLASS
#define TEST_CLASS(arg) class arg
#endif

#ifndef TEST_METHOD
#define TEST_METHOD(arg) void arg () const
#endif

#if defined (__CSC_TARGET_EXE__) || defined (__CSC_TARGET_DLL__)
namespace CSC {
inline exports PTR<NONE> GlobalStaticEngine::unique_compare_exchange (const PTR<NONE> &expect ,const PTR<NONE> &data) {
	PTR<NONE> ret = NULL ;
	_CALL_TRY_ ([&] () {
		auto &r1x = _CACHE_ ([&] () {
			return SharedRef<AtomicPtr>::make () ;
		}) ;
		ret = r1x->compare_exchange (expect ,data) ;
	} ,[&] () {
		ret = NULL ;
	}) ;
	return _MOVE_ (ret) ;
}
} ;
#endif

namespace UNITTEST {
using namespace CSC ;
} ;