#pragma once

#include <csc.hpp>
#include <csc_core.hpp>
#include <csc_basic.hpp>
#include <csc_extend.hpp>
#include <csc_array.hpp>
#include <csc_math.hpp>
#include <csc_functional.hpp>
#include <csc_images.hpp>
#include <csc_geometry.hpp>
#include <csc_algorithm.hpp>
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
#pragma region
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
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
#pragma warning (pop)
#pragma endregion
#endif

#ifndef TEST_CLASS
#define TEST_CLASS(arg) class arg
#endif

#ifndef TEST_METHOD
#define TEST_METHOD(arg) void arg () const
#endif

#if defined (__CSC_TARGET_EXE__) || defined (__CSC_TARGET_DLL__)
namespace CSC {
inline exports PTR<NONE> GlobalStatic<void>::Public::unique_atomic_address (const PTR<NONE> &expect ,const PTR<NONE> &data) side_effects {
	PTR<NONE> ret = NULL ;
	_CALL_TRY_ ([&] () {
		auto &r1x = _CACHE_ ([&] () {
			return SharedRef<Atomic>::make () ;
		}) ;
		const auto r2x = r1x->compare_exchange (_ADDRESS_ (expect) ,_ADDRESS_ (data)) ;
		auto &r3x = _LOAD_UNSAFE_ (ARGV<NONE>::null ,r2x) ;
		ret = DEPTR[r3x] ;
	} ,[&] () {
		ret = NULL ;
	}) ;
	return _MOVE_ (ret) ;
}
} ;
#endif

#ifdef __CSC_UNITTEST__
#ifdef __CSC_COMPILER_MSVC__
namespace CSC {
inline exports void GlobalWatch::Public::done (const Exception &e) {
	const auto r1x = String<STR> (e.what ()) ;
	Singleton<ConsoleService>::instance ().fatal (r1x) ;
#ifdef MS_CPP_UNITTESTFRAMEWORK
	const auto r2x = StringProc::build_strs (ARGV<STRW>::null ,r1x) ;
	Assert::Fail (r2x.raw ().self) ;
#endif
}
} ;
#endif
#endif

namespace UNITTEST {
using namespace CSC ;
} ;

#ifdef __CSC_COMPILER_GNUC__
#pragma region
#ifdef __CSC_MATH__
#include <csc_math.hpp.default.inl>
#endif

#ifdef __CSC_FUNCTIONAL__
#include <csc_functional.hpp.op.inl>
#endif

#ifdef __CSC_STRING__
#include <csc_string.hpp.default.inl>
#include <csc_string.hpp.gbk.inl>
#endif

#ifdef __CSC_RUNTIME__
#include <csc_runtime.hpp.default.inl>
#endif

#ifdef __CSC_FILESYSTEM__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_filesystem.hpp.windows.inl>
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <csc_filesystem.hpp.linux.inl>
#endif
#endif

#ifdef __CSC_NETWORK__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_network.hpp.windows.inl>
#endif
#endif

#ifdef __CSC_DEBUGGER__
#ifdef __CSC_SYSTEM_WINDOWS__
#include <csc_debugger.hpp.windows.inl>
#endif

#ifdef __CSC_SYSTEM_LINUX__
#include <csc_debugger.hpp.linux.inl>
#endif
#endif
#pragma endregion
#endif