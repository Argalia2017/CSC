﻿#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_runtime.hpp"

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require windows.h"
#endif

#include "csc_end.h"
#include <cstdlib>
#include <thread>
#include "csc_begin.h"

namespace CSC {
class RuntimeProcImplHolder final implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	LENGTH thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	FLAG thread_uid () const override {
		return FLAG (GetCurrentThreadId ()) ;
	}

	void thread_sleep (CREF<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	FLAG process_uid () const override {
		return FLAG (GetCurrentProcessId ()) ;
	}

	void process_exit () const override {
		std::quick_exit (0) ;
	}

	String<STR> library_file () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = invoke ([&] () {
			const auto r2x = csc_enum_t (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT) ;
			auto &&rax = keep[TYPE<HeapLayout>::expr] (Heap::expr) ;
			const auto r3x = LPTSTR (rax.mHolder) ;
			csc_device_t ret = NULL ;
			const auto r4x = GetModuleHandleEx (r2x ,r3x ,(&ret)) ;
			assume (r4x) ;
			return move (ret) ;
		}) ;
		GetModuleFileName (r1x ,ret ,csc_enum_t (ret.size ())) ;
		return move (ret) ;
	}

	String<STR> library_main () const override {
		String<STR> ret = String<STR>::make () ;
		GetModuleFileName (NULL ,ret ,csc_enum_t (ret.size ())) ;
		return move (ret) ;
	}
} ;

static const auto mRuntimeProcExternal = External<RuntimeProcHolder ,RuntimeProcLayout> (RuntimeProcImplHolder ()) ;

class ProcessImplHolder final implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_STEP = ENUM<128> ;

public:
	void initialize (CREF<FLAG> uid) override {
		self.mUid = uid ;
		const auto r1x = UniqueRef<csc_handle_t> ([&] (VREF<csc_handle_t> me) {
			me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,csc_enum_t (uid)) ;
		} ,[&] (VREF<csc_handle_t> me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
		}) ;
		self.mProcessCode = process_code (r1x ,uid) ;
		self.mProcessTime = process_time (r1x ,uid) ;
	}

	QUAD process_code (CREF<csc_handle_t> handle ,CREF<FLAG> uid) const {
		return QUAD (GetProcessVersion (csc_enum_t (uid))) ;
	}

	QUAD process_time (CREF<csc_handle_t> handle ,CREF<FLAG> uid) const {
		const auto r1x = invoke ([&] () {
			Buffer4<FILETIME> ret ;
			inline_memset (ret) ;
			GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
			return move (ret) ;
		}) ;
		return ByteProc::merge (CHAR (r1x[0].dwHighDateTime) ,CHAR (r1x[0].dwLowDateTime)) ;
	}

	void initialize (CREF<RefBuffer<BYTE>> snapshot_) override {
		self.mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_STEP::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.poll (TYPE<VAL64>::expr) ;
			self.mUid = FLAG (r1x) ;
			rax >> GAP ;
			rax >> self.mProcessCode ;
			rax >> GAP ;
			rax >> self.mProcessTime ;
			rax >> GAP ;
			rax >> EOS ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
	}

	BOOL equal (CREF<ProcessLayout> that) const override {
		const auto r1x = inline_equal (self.mUid ,that.mUid) ;
		if (!r1x)
			return r1x ;
		const auto r2x = inline_equal (self.mProcessCode ,that.mProcessCode) ;
		if (!r2x)
			return r2x ;
		const auto r3x = inline_equal (self.mProcessTime ,that.mProcessTime) ;
		if (!r3x)
			return r3x ;
		return TRUE ;
	}

	FLAG process_uid () const override {
		return self.mUid ;
	}

	RefBuffer<BYTE> snapshot () const override {
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (PROCESS_SNAPSHOT_STEP::expr) ;
		auto rax = ByteWriter (Ref<RefBuffer<BYTE>>::reference (ret)) ;
		if ifdo (TRUE) {
			rax << slice ("CSC_Process") ;
			rax << GAP ;
			rax << VAL64 (self.mUid) ;
			rax << GAP ;
			rax << self.mProcessCode ;
			rax << GAP ;
			rax << self.mProcessTime ;
			rax << GAP ;
			rax << EOS ;
		}
		return move (ret) ;
	}
} ;

static const auto mProcessExternal = External<ProcessHolder ,ProcessLayout> (ProcessImplHolder ()) ;

class LibraryImplHolder final implement Fat<LibraryHolder ,LibraryLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		self.mFile = move (file) ;
		assert (self.mFile.length () > 0) ;
		self.mLibrary = UniqueRef<csc_device_t> ([&] (VREF<csc_device_t> me) {
			me = GetModuleHandle (self.mFile) ;
			if (me != NULL)
				return ;
			me = LoadLibrary (self.mFile) ;
			if (me != NULL)
				return ;
			self.mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		} ,[&] (VREF<csc_device_t> me) {
			noop () ;
		}) ;
	}

	String<STR> library_file () const override {
		return self.mFile ;
	}

	FLAG load (CREF<String<STR>> name) override {
		assert (name.length () > 0) ;
		const auto r1x = StringProc::stra_from_strs (name) ;
		FLAG ret = FLAG (GetProcAddress (self.mLibrary ,r1x)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			self.mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<STR> error () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = csc_enum_t (self.mLastError) ;
		const auto r2x = csc_enum_t (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
		const auto r3x = csc_enum_t (ret.size ()) ;
		FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,r1x ,r2x ,ret ,r3x ,NULL) ;
		ret = String<STR>::make (Format (slice ("LastError = $1 : $2")) (FLAG (r1x) ,ret)) ;
		return move (ret) ;
	}
} ;

static const auto mLibraryExternal = External<LibraryHolder ,LibraryLayout> (LibraryImplHolder ()) ;

class SingletonProcImplHolder final implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		self.mUid = RuntimeProc::process_uid () ;
		self.mName = String<STR>::make (slice ("CSC_Singleton_") ,self.mUid) ;
		inline_memset (self.mLocal) ;
		sync_local () ;
	}

	static VREF<SingletonRoot> root_ptr () {
		return memorize ([&] () {
			SingletonRoot ret ;
			ret.mPin.pin (ret) ;
			return move (ret) ;
		}).mPin.ref ;
	}

	void sync_local () {
		auto act = TRUE ;
		if ifdo (act) {
			try {
				load_local () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				init_local () ;
				save_local () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = FLAG (self.mLocal.mAddress1) ;
			assume (r1x != ZERO) ;
			auto &&rax = keep[TYPE<SingletonRoot>::expr] (Pointer::make (r1x)) ;
			self.mRoot = Ref<SingletonRoot>::reference (rax) ;
		}
	}

	void init_local () {
		if (self.mMapping.exist ())
			return ;
		self.mMapping = UniqueRef<csc_handle_t> ([&] (VREF<csc_handle_t> me) {
			const auto r1x = csc_enum_t (SIZE_OF<SingletonLocal>::expr) ;
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,r1x ,self.mName) ;
			assume (me != NULL) ;
			const auto r2x = GetLastError () ;
			assume (r2x != ERROR_ALREADY_EXISTS) ;
		} ,[&] (VREF<csc_handle_t> me) {
			CloseHandle (me) ;
		}) ;
		root_ptr ().mMutex = NULL ;
		self.mLocal.mReserve1 = QUAD (self.mUid) ;
		self.mLocal.mAddress1 = QUAD (address (root_ptr ())) ;
		self.mLocal.mReserve2 = abi_reserve () ;
		self.mLocal.mAddress2 = QUAD (address (root_ptr ())) ;
		self.mLocal.mReserve3 = ctx_reserve () ;
	}

	void load_local () {
		const auto r1x = UniqueRef<csc_handle_t> ([&] (VREF<csc_handle_t> me) {
			me = OpenFileMapping (FILE_MAP_READ ,FALSE ,self.mName) ;
			assume (me != NULL) ;
		} ,[&] (VREF<csc_handle_t> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<csc_handle_t> ([&] (VREF<csc_handle_t> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_READ ,0 ,0 ,SIZE_OF<SingletonLocal>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VREF<csc_handle_t> me) {
			UnmapViewOfFile (me) ;
		}) ;
		const auto r3x = FLAG (r2x.ref) ;
		auto rax = SingletonLocal () ;
		inline_memcpy (Pointer::from (rax) ,Pointer::make (r3x) ,SIZE_OF<SingletonLocal>::expr) ;
		assume (rax.mReserve1 == QUAD (self.mUid)) ;
		assume (rax.mAddress1 != QUAD (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		self.mLocal = rax ;
	}

	void save_local () {
		const auto r1x = UniqueRef<csc_handle_t> ([&] (VREF<csc_handle_t> me) {
			me = OpenFileMapping (FILE_MAP_WRITE ,FALSE ,self.mName) ;
			assume (me != NULL) ;
		} ,[&] (VREF<csc_handle_t> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<csc_handle_t> ([&] (VREF<csc_handle_t> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_WRITE ,0 ,0 ,SIZE_OF<SingletonLocal>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VREF<csc_handle_t> me) {
			UnmapViewOfFile (me) ;
		}) ;
		const auto r3x = FLAG (r2x.ref) ;
		auto rax = self.mLocal ;
		assume (rax.mReserve1 == QUAD (self.mUid)) ;
		assume (rax.mAddress1 != QUAD (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		inline_memcpy (Pointer::make (r3x) ,Pointer::from (rax) ,SIZE_OF<SingletonLocal>::expr) ;
	}

	QUAD abi_reserve () const override {
		QUAD ret = QUAD (0X00) ;
#ifdef __CSC_VER_DEBUG__
		ret |= QUAD (0X00000001) ;
#elif defined __CSC_VER_UNITTEST__
		ret |= QUAD (0X00000002) ;
#elif defined __CSC_VER_RELEASE__
		ret |= QUAD (0X00000003) ;
#endif
#ifndef __CSC_COMPILER_NVCC__
#ifdef __CSC_COMPILER_MSVC__
		ret |= QUAD (0X00000010) ;
#elif defined __CSC_COMPILER_GNUC__
		ret |= QUAD (0X00000020) ;
#elif defined __CSC_COMPILER_CLANG__
		ret |= QUAD (0X00000030) ;
#endif
#else
		ret |= QUAD (0X00000040) ;
#endif
#ifdef __CSC_SYSTEM_WINDOWS__
		ret |= QUAD (0X00000100) ;
#elif defined __CSC_SYSTEM_LINUX__
		ret |= QUAD (0X00000200) ;
#endif
#ifdef __CSC_PLATFORM_X86__
		ret |= QUAD (0X00001000) ;
#elif defined __CSC_PLATFORM_X64__
		ret |= QUAD (0X00002000) ;
#elif defined __CSC_PLATFORM_ARM__
		ret |= QUAD (0X00003000) ;
#elif defined __CSC_PLATFORM_ARM64__
		ret |= QUAD (0X00004000) ;
#endif
#ifdef __CSC_CONFIG_VAL32__
		ret |= QUAD (0X00010000) ;
#elif defined __CSC_CONFIG_VAL64__
		ret |= QUAD (0X00020000) ;
#endif
#ifdef __CSC_CONFIG_STRA__
		ret |= QUAD (0X00100000) ;
#elif defined __CSC_CONFIG_STRW__
		ret |= QUAD (0X00200000) ;
#endif
		return move (ret) ;
	}

	QUAD ctx_reserve () const override {
		return QUAD (0X0FEDCBA987654321) ;
	}

	FLAG load (CREF<Clazz> clazz) const override {
		assume (self.mRoot.exist ()) ;
		Scope<Mutex> anonymous (self.mRoot->mMutex) ;
		FLAG ret = self.mRoot->mClazzSet.map (clazz) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> layout) const override {
		assert (layout != ZERO) ;
		assert (layout != NONE) ;
		assume (self.mRoot.exist ()) ;
		Scope<Mutex> anonymous (self.mRoot->mMutex) ;
		self.mRoot->mPin.ref.mClazzSet.add (clazz ,layout) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout> (SingletonProcImplHolder ()) ;
} ;