#pragma once

#ifdef __clang__
#if _MSC_VER >= 1900
#ifndef __llvm__
#define __is_trivially_destructible __is_pod
#endif
#endif
#endif

#define _UNITTEST
#ifdef _MSC_VER
#define _STRICTHPPINL
#endif
#include <csc.hpp>
#include <csc_ext.hpp>
#include <csc_array.hpp>
#include <csc_math.hpp>
#include <csc_images.hpp>
#include <csc_graphics.hpp>
#include <csc_algorithm.hpp>
#include <csc_analysis.hpp>
#include <csc_stream.hpp>
#include <csc_string.hpp>
#include <csc_serialize.hpp>
#include <csc_thread.hpp>
#include <csc_filesystem.hpp>
#include <csc_network.hpp>
#include <csc_databases.hpp>
#include <csc_debugger.hpp>
#include <csc_design.hpp>

#ifdef __CSC_SYSTEM_WINDOWS__
#pragma region
#pragma warning (push)
#pragma warning (disable :4996)
#pragma warning (disable :5039)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef __CSC_COMPILER_MSVC__
#ifdef __CSC_TARGET_DLL__
#include <CppUnitTest.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework ;
#endif
#endif
#pragma warning (pop)
#pragma endregion
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4628)
#endif

#ifdef __CSC_PLATFORM_X86__
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4738)
#endif
#endif

#ifndef MS_CPP_UNITTESTFRAMEWORK
#define TEST_CLASS(arg) class arg
#define TEST_METHOD(arg) void arg () const
#endif

namespace CSC {
#ifdef __CSC_UNITTEST__
inline export void _UNITTEST_ASSERT_HANDLER_ (PTR<const STRA> what) {
#ifdef MS_CPP_UNITTESTFRAMEWORK
#ifdef __CSC_STRING__
	Assert::Fail (OPERATOR_CONVERT_TYPE<String<STRW>>::invoke (_ASTOUAS_ (what))) ;
#else
	Assert::Fail () ;
#endif
#else
	(void) what ;
	std::terminate () ;
#endif
#endif
}

#if defined (__CSC_TARGET_EXE__) || defined (__CSC_TARGET_DLL__)
template <>
inline export Console &Singleton<Console>::instance () {
	static Singleton mInstance ;
	return mInstance ;
}

template <>
inline export Logger &Singleton<Logger>::instance () {
	static Singleton mInstance ;
	return mInstance ;
}

template <>
inline export DebuggerService &Singleton<DebuggerService>::instance () {
	static Singleton mInstance ;
	return mInstance ;
}
#endif

template <class... _ARGS>
inline void _UNITTEST_PRINTS_ (const _ARGS &...args) {
	Singleton<Logger>::instance ().write (_PCSTR_ ("UNITTEST") ,args...) ;
}
} ;

namespace UNITTEST {
#ifdef __CSC__
using namespace CSC ;
using namespace CSC::STD ;
#endif

#ifdef __CSC_ARRAY__
using namespace CSC::ARRAY ;
#endif

#ifdef __CSC_MATH__
using namespace CSC::MATH ;
#endif

#ifdef __CSC_STREAM__
using namespace CSC::STREAM ;
#endif

#ifdef __CSC_STRING__
using namespace CSC::STRING ;
#endif

#ifdef __CSC_FILESYSTEM__
using namespace CSC::FILESYSTEM ;
#endif
} ;

#ifdef __CSC_COMPILER_GNUC__
#pragma region
#pragma push_macro ("_STRICTHPPINL")
#pragma push_macro ("__CSC_STRICTHPPINL__")
#undef _STRICTHPPINL
#undef __CSC_STRICTHPPINL__

#ifdef __CSC_MATH__
#include "csc_math.hpp.inl"
#endif

#ifdef __CSC_IMAGES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include "csc_images.hpp.opencv.inl"
#include "csc_images.hpp.freeimage.inl"
#endif
#endif
#endif

#ifdef __CSC_GRAPHICS__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include "csc_graphics.hpp.opengl.inl"
#endif
#endif
#endif

#ifdef __CSC_STREAM__
#include "csc_stream.hpp.inl"
#endif

#ifdef __CSC_STRING__
#include "csc_string.hpp.inl"
#endif

#ifdef __CSC_FILESYSTEM__
#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc_filesystem.hpp.windows.inl"
#elif defined __CSC_SYSTEM_LINUX__
#include "csc_filesystem.hpp.linux.inl"
#endif
#endif

#ifdef __CSC_NETWORK__
#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc_network.hpp.windows.inl"
#endif
#endif

#ifdef __CSC_DATABASES__
#ifdef __CSC_PLATFORM_X64__
#ifdef __CSC_COMPILER_MSVC__
#include "csc_databases.hpp.mysql.inl"
#endif
#endif
#endif

#ifdef __CSC_DEBUGGER__
#ifdef __CSC_SYSTEM_WINDOWS__
#include "csc_debugger.hpp.windows.inl"
#elif defined __CSC_SYSTEM_LINUX__
#include "csc_debugger.hpp.linux.inl"
#endif
#endif

#pragma pop_macro ("_STRICTHPPINL")
#pragma pop_macro ("__CSC_STRICTHPPINL__")
#pragma endregion
#endif