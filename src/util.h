#pragma once

#include <csc.hpp>
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
#endif
#ifdef __CSC_COMPILER_MSVC__
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
#define TEST_CLASS(var1) class var1
#endif

#ifndef TEST_METHOD
#define TEST_METHOD(var1) void var1 () const
#endif

#if defined (__CSC_TARGET_EXE__) || defined (__CSC_TARGET_DLL__)
namespace CSC {
inline exports PTR<NONE> GlobalStatic<void>::unique_atomic_address (PTR<NONE> expect ,PTR<NONE> data) popping {
	PTR<NONE> ret = NULL ;
	_CALL_TRY_ ([&] () {
		auto &r1y = _CACHE_ ([] () {
			return SharedRef<std::atomic<PTR<NONE>>>::make (_XVALUE_<PTR<NONE>> (NULL)) ;
		}) ;
		r1y->compare_exchange_strong (expect ,data) ;
		ret = r1y->load () ;
	} ,[&] () {
		ret = NULL ;
	}) ;
	return std::move (ret) ;
}
} ;
#endif

namespace CSC {
template <>
inline exports ConsoleService &Singleton<ConsoleService>::instance () {
	return GlobalStatic<Singleton<ConsoleService>>::unique () ;
}
} ;

#ifdef __CSC_COMPILER_MSVC__
namespace CSC {
template <>
inline exports NetworkService &Singleton<NetworkService>::instance () {
	return GlobalStatic<Singleton<NetworkService>>::unique () ;
}
} ;

namespace CSC {
template <>
inline exports DebuggerService &Singleton<DebuggerService>::instance () {
	return GlobalStatic<Singleton<DebuggerService>>::unique () ;
}
} ;
#endif

namespace UNITTEST {
using namespace CSC ;
} ;

namespace UNITTEST {
#ifdef __CSC_UNITTEST__
class GlobalUnittest final
	:private Wrapped<void> {
public:
	inline static void done (const ARR<STR> &what) {
#ifdef MS_CPP_UNITTESTFRAMEWORK
#ifdef __CSC_STRING__
		const auto r1x = _BUILDSTRS_<STRW> (what) ;
		Assert::Fail (r1x.raw ().self) ;
#else
		Assert::Fail () ;
#endif
#else
		Singleton<ConsoleService>::instance ().fatal (String<STR> (what)) ;
#endif
	}
} ;
#endif
} ;

#ifdef __CSC_UNITTEST__
#ifdef __CSC_COMPILER_MSVC__
#define _UNITTEST_ASSERT_(...) do { if (!(_UNW_ (__VA_ARGS__))) GlobalUnittest::done (CSC::Plain<CSC::STR> (_PCSTR_ ("unittest_assert failed : " _STR_ (__VA_ARGS__) " : at " M_FUNC " in " M_FILE " ," M_LINE))) ; } while (FALSE)
#else
#define _UNITTEST_ASSERT_(...) do { struct ARGVPL ; if (!(_UNW_ (__VA_ARGS__))) GlobalUnittest::done (CSC::Plain<CSC::STR> (CSC::_NULL_<CSC::ARGV<ARGVPL>> () ,"unittest_assert failed : " _STR_ (__VA_ARGS__) " : at " ,M_FUNC ," in " ,M_FILE ," ," ,M_LINE)) ; } while (FALSE)
#endif
#else
#define _UNITTEST_ASSERT_(...) do {} while (FALSE)
#endif

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
#elif defined __CSC_SYSTEM_LINUX__
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
#elif defined __CSC_SYSTEM_LINUX__
#include <csc_debugger.hpp.linux.inl>
#endif
#endif
#pragma endregion
#endif