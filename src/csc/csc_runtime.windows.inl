#pragma once

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
#ifdef __CSC_SYSTEM_WINDOWS__
#include <debugapi.h>
#include <DbgHelp.h>
#pragma comment (lib ,"DbgHelp.lib")
#endif

#include <cstdlib>
#include <thread>
#include "csc_begin.h"

namespace CSC {
class RuntimeProcImplHolder final implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Tuple<Flag ,Flag> stack_limit () const override {
		Tuple<Flag ,Flag> ret ;
		const auto r1x = invoke ([&] () {
			Buffer2<ULONG_PTR> ret ;
			inline_memset (ret) ;
			GetCurrentThreadStackLimits ((&ret[0]) ,(&ret[1])) ;
			return move (ret) ;
		}) ;
		ret.m1st = Flag (r1x[0]) ;
		ret.m2nd = Flag (r1x[1]) ;
		return move (ret) ;
	}

	String<Str> stack_trace (CR<Length> skip) const override {
		assert (skip >= 0) ;
		String<Str> ret = String<Str> (15 * 1024) ;
		auto mWriter = TextWriter (ret.borrow ()) ;
		const auto r1x = GetCurrentProcess () ;
		const auto r2x = SymInitialize (r1x ,NULL ,TRUE) ;
		const auto r3x = 128 * SIZE_OF<csc_handle_t>::expr ;
		const auto r4x = r3x + SIZE_OF<SYMBOL_INFO>::expr + 1024 ;
		const auto r5x = r4x + SIZE_OF<IMAGEHLP_MODULE64>::expr ;
		const auto r6x = address (ret[ret.size () - r5x]) ;
		const auto r7x = Length (CaptureStackBackTrace (csc_enum_t (skip) ,csc_enum_t (128) ,PTR<VR<csc_handle_t>> (r6x) ,NULL)) ;
		auto &&rax = keep[TYPE<SYMBOL_INFO>::expr] (Pointer::make (r6x + r3x)) ;
		auto &&rbx = keep[TYPE<IMAGEHLP_MODULE64>::expr] (Pointer::make (r6x + r4x)) ;
		rax.SizeOfStruct = csc_enum_t (SIZE_OF<SYMBOL_INFO>::expr) ;
		rax.MaxNameLen = csc_enum_t (1024) ;
		rbx.SizeOfStruct = csc_enum_t (SIZE_OF<IMAGEHLP_MODULE64>::expr) ;
		for (auto &&i : range (0 ,r7x)) {
			const auto r8x = r6x + i * SIZE_OF<csc_handle_t>::expr ;
			const auto r9x = csc_handle_t (bitwise (Pointer::make (r8x))) ;
			mWriter << slice ("#") << WriteAligned (i ,2) ;
			mWriter << slice (" [0X") ;
			mWriter << Quad (Flag (r9x)) ;
			mWriter << slice ("] : ") ;
			auto act = TRUE ;
			if ifdo (act) {
				if (!r2x)
					discard ;
				const auto r10x = SymGetModuleInfo64 (r1x ,csc_size_t (r9x) ,(&rbx)) ;
				if (!r10x)
					discard ;
				const auto r11x = Slice (Flag (rbx.ImageName) ,SLICE_MAX_SIZE::expr ,1).eos () ;
				if (r11x.size () == 0)
					discard ;
				mWriter << slice_filename (r11x) ;
				mWriter << slice ("!") ;
				const auto r12x = SymFromAddr (r1x ,csc_size_t (r9x) ,NULL ,(&rax)) ;
				if (!r12x)
					discard ;
				const auto r13x = Slice (Flag (rax.Name) ,rax.NameLen ,1).eos () ;
				if (r13x.size () == 0)
					discard ;
				mWriter << r13x ;
			}
			if ifdo (act) {
				mWriter << slice ("???") ;
			}
			mWriter << GAP ;
		}
		mWriter << EOS ;
		return move (ret) ;
	}

	Slice slice_filename (CR<Slice> s) const {
		const auto r1x = s.size () ;
		Index ix = r1x - 1 ;
		while (TRUE) {
			if (ix < 0)
				break ;
			const auto r2x = s[ix] ;
			if (r2x == Stru32 ('\\'))
				break ;
			if (r2x == Stru32 ('/'))
				break ;
			ix-- ;
		}
		ix++ ;
		return Slice (s.offset (ix) ,r1x - ix ,s.step ()) ;
	}

	Length thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	Flag thread_uid () const override {
		return Flag (GetCurrentThreadId ()) ;
	}

	void thread_sleep (CR<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	Flag process_uid () const override {
		return Flag (GetCurrentProcessId ()) ;
	}

	void process_exit () const override {
		std::exit (0) ;
	}

	String<Str> library_file (CR<csc_handle_t> addr) const override {
		String<Str> ret = String<Str>::make () ;
		const auto r1x = invoke ([&] () {
			const auto r2x = csc_enum_t (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT) ;
			const auto r3x = LPTSTR (addr) ;
			csc_device_t ret = NULL ;
			const auto r4x = GetModuleHandleEx (r2x ,r3x ,(&ret)) ;
			assume (r4x) ;
			return move (ret) ;
		}) ;
		GetModuleFileName (r1x ,ret ,csc_enum_t (ret.size ())) ;
		return move (ret) ;
	}

	String<Str> library_main () const override {
		String<Str> ret = String<Str>::make () ;
		GetModuleFileName (NULL ,ret ,csc_enum_t (ret.size ())) ;
		return move (ret) ;
	}
} ;

static const auto mRuntimeProcExternal = External<RuntimeProcHolder ,RuntimeProcLayout> (RuntimeProcImplHolder ()) ;

class ProcessImplHolder final implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_STEP = ENUM<128> ;

public:
	void initialize (CR<Flag> uid) override {
		self.mUid = uid ;
		const auto r1x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,csc_enum_t (uid)) ;
		} ,[&] (VR<csc_handle_t> me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
		}) ;
		self.mProcessCode = process_code (r1x ,uid) ;
		self.mProcessTime = process_time (r1x ,uid) ;
	}

	Quad process_code (CR<csc_handle_t> handle ,CR<Flag> uid) const {
		return Quad (GetProcessVersion (csc_enum_t (uid))) ;
	}

	Quad process_time (CR<csc_handle_t> handle ,CR<Flag> uid) const {
		const auto r1x = invoke ([&] () {
			Buffer4<FILETIME> ret ;
			inline_memset (ret) ;
			GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
			return move (ret) ;
		}) ;
		return ByteProc::merge (Char (r1x[0].dwHighDateTime) ,Char (r1x[0].dwLowDateTime)) ;
	}

	void initialize (CR<RefBuffer<Byte>> snapshot_) override {
		self.mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_STEP::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<Byte>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.pull (TYPE<Val64>::expr) ;
			self.mUid = Flag (r1x) ;
			rax >> GAP ;
			rax >> self.mProcessCode ;
			rax >> GAP ;
			rax >> self.mProcessTime ;
			rax >> GAP ;
			rax >> EOS ;
		} catch (CR<Exception> e) {
			noop (e) ;
		}
	}

	Bool equal (CR<ProcessLayout> that) const override {
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

	Flag process_uid () const override {
		return self.mUid ;
	}

	RefBuffer<Byte> snapshot () const override {
		RefBuffer<Byte> ret = RefBuffer<Byte> (PROCESS_SNAPSHOT_STEP::expr) ;
		auto rax = ByteWriter (Ref<RefBuffer<Byte>>::reference (ret)) ;
		if ifdo (TRUE) {
			rax << slice ("CSC_Process") ;
			rax << GAP ;
			rax << Val64 (self.mUid) ;
			rax << GAP ;
			rax << self.mProcessCode ;
			rax << GAP ;
			rax << self.mProcessTime ;
			rax << GAP ;
		}
		rax << EOS ;
		return move (ret) ;
	}
} ;

static const auto mProcessExternal = External<ProcessHolder ,ProcessLayout> (ProcessImplHolder ()) ;

class LibraryImplHolder final implement Fat<LibraryHolder ,LibraryLayout> {
public:
	void initialize (CR<String<Str>> file) override {
		self.mFile = move (file) ;
		assert (self.mFile.length () > 0) ;
		self.mLibrary = UniqueRef<csc_device_t> ([&] (VR<csc_device_t> me) {
			me = GetModuleHandle (self.mFile) ;
			if (me != NULL)
				return ;
			me = LoadLibrary (self.mFile) ;
			if (me != NULL)
				return ;
			self.mLastError = Flag (GetLastError ()) ;
			assume (FALSE) ;
		} ,[&] (VR<csc_device_t> me) {
			noop () ;
		}) ;
	}

	String<Str> library_file () const override {
		return self.mFile ;
	}

	Flag load (CR<String<Str>> name) override {
		assert (name.length () > 0) ;
		const auto r1x = StringProc::stra_from (name) ;
		Flag ret = Flag (GetProcAddress (self.mLibrary ,r1x)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			self.mLastError = Flag (GetLastError ()) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<Str> error () const override {
		String<Str> ret = String<Str>::make () ;
		const auto r1x = csc_enum_t (self.mLastError) ;
		const auto r2x = csc_enum_t (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
		const auto r3x = csc_enum_t (ret.size ()) ;
		FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,r1x ,r2x ,ret ,r3x ,NULL) ;
		ret = String<Str>::make (Format (slice ("LastError = $1 : $2")) (Flag (r1x) ,ret)) ;
		return move (ret) ;
	}
} ;

static const auto mLibraryExternal = External<LibraryHolder ,LibraryLayout> (LibraryImplHolder ()) ;

class SingletonProcImplHolder final implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		self.mUid = RuntimeProc::process_uid () ;
		self.mName = String<Str>::make (slice ("CSC_Singleton_") ,self.mUid) ;
		inline_memset (self.mLocal) ;
		sync_local () ;
	}

	void sync_local () {
		auto act = TRUE ;
		if ifdo (act) {
			try {
				load_local () ;
			} catch (CR<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				init_local () ;
				save_local () ;
			} catch (CR<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (TRUE) {
			if (self.mRoot.exist ())
				discard ;
			const auto r1x = Flag (self.mLocal.mAddress1) ;
			assume (r1x != ZERO) ;
			auto &&rax = keep[TYPE<SingletonImplLayout>::expr] (Pointer::make (r1x)) ;
			self.mRoot = Ref<SingletonImplLayout>::reference (rax) ;
		}
	}

	void init_local () {
		if (self.mMapping.exist ())
			return ;
		self.mMapping = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			const auto r1x = csc_enum_t (SIZE_OF<SingletonLocal>::expr) ;
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,r1x ,self.mName) ;
			assume (me != NULL) ;
			const auto r2x = GetLastError () ;
			assume (r2x != ERROR_ALREADY_EXISTS) ;
		} ,[&] (VR<csc_handle_t> me) {
			CloseHandle (me) ;
		}) ;
		self.mRoot = Ref<SingletonImplLayout>::reference (SingletonImplLayout::expr) ;
		self.mRoot->mMutex = NULL ;
		self.mLocal.mReserve1 = Quad (self.mUid) ;
		self.mLocal.mAddress1 = Quad (address (self.mRoot.ref)) ;
		self.mLocal.mReserve2 = abi_reserve () ;
		self.mLocal.mAddress2 = Quad (address (self.mRoot.ref)) ;
		self.mLocal.mReserve3 = ctx_reserve () ;
	}

	void load_local () {
		const auto r1x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = OpenFileMapping (FILE_MAP_READ ,FALSE ,self.mName) ;
			assume (me != NULL) ;
		} ,[&] (VR<csc_handle_t> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_READ ,0 ,0 ,SIZE_OF<SingletonLocal>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VR<csc_handle_t> me) {
			UnmapViewOfFile (me) ;
		}) ;
		const auto r3x = Flag (r2x.ref) ;
		auto rax = SingletonLocal () ;
		rax = bitwise (Pointer::make (r3x)) ;
		assume (rax.mReserve1 == Quad (self.mUid)) ;
		assume (rax.mAddress1 != Quad (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		self.mLocal = rax ;
	}

	void save_local () {
		const auto r1x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = OpenFileMapping (FILE_MAP_WRITE ,FALSE ,self.mName) ;
			assume (me != NULL) ;
		} ,[&] (VR<csc_handle_t> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_WRITE ,0 ,0 ,SIZE_OF<SingletonLocal>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VR<csc_handle_t> me) {
			UnmapViewOfFile (me) ;
		}) ;
		const auto r3x = Flag (r2x.ref) ;
		auto rax = self.mLocal ;
		assume (rax.mReserve1 == Quad (self.mUid)) ;
		assume (rax.mAddress1 != Quad (0X00)) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		bitwise (Pointer::make (r3x)) = rax ;
	}

	Quad abi_reserve () const override {
		Quad ret = Quad (0X00) ;
#ifdef __CSC_VER_DEBUG__
		ret |= Quad (0X00000001) ;
#elif defined __CSC_VER_UNITTEST__
		ret |= Quad (0X00000002) ;
#elif defined __CSC_VER_RELEASE__
		ret |= Quad (0X00000003) ;
#endif
#ifdef __CSC_COMPILER_MSVC__
		ret |= Quad (0X00000010) ;
#elif defined __CSC_COMPILER_GNUC__
		ret |= Quad (0X00000020) ;
#elif defined __CSC_COMPILER_CLANG__
		ret |= Quad (0X00000030) ;
#endif
#ifdef __CSC_SYSTEM_WINDOWS__
		ret |= Quad (0X00000100) ;
#elif defined __CSC_SYSTEM_LINUX__
		ret |= Quad (0X00000200) ;
#endif
#ifdef __CSC_PLATFORM_X86__
		ret |= Quad (0X00001000) ;
#elif defined __CSC_PLATFORM_X64__
		ret |= Quad (0X00002000) ;
#elif defined __CSC_PLATFORM_ARM__
		ret |= Quad (0X00003000) ;
#elif defined __CSC_PLATFORM_ARM64__
		ret |= Quad (0X00004000) ;
#endif
#ifdef __CSC_CONFIG_VAL32__
		ret |= Quad (0X00010000) ;
#elif defined __CSC_CONFIG_VAL64__
		ret |= Quad (0X00020000) ;
#endif
#ifdef __CSC_CONFIG_STRA__
		ret |= Quad (0X00100000) ;
#elif defined __CSC_CONFIG_STRW__
		ret |= Quad (0X00200000) ;
#endif
		return move (ret) ;
	}

	Quad ctx_reserve () const override {
		const auto r1x = Process (self.mUid) ;
		const auto r2x = r1x.snapshot () ;
		const auto r3x = HashProc::fnvhash64 (Pointer::from (r2x) ,r2x.size ()) ;
		return Quad (r3x) ;
	}

	Flag regi (CR<Unknown> holder) const override {
		const auto r1x = Clazz (holder) ;
		Flag ret = SingletonProc::load (r1x) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			ret = r1x.type_expr () ;
			SingletonProc::save (r1x ,ret) ;
			ret = SingletonProc::load (r1x) ;
		}
		assert (ret != ZERO) ;
		return move (ret) ;
	}

	Flag load (CR<Clazz> clazz) const override {
		assume (self.mRoot.exist ()) ;
		Scope anonymous (self.mRoot->mMutex) ;
		Flag ret = self.mRoot->mClazzSet.map (clazz) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CR<Clazz> clazz ,CR<Flag> layout) const override {
		assert (layout != ZERO) ;
		assert (layout != NONE) ;
		assume (self.mRoot.exist ()) ;
		Scope anonymous (self.mRoot->mMutex) ;
		self.mRoot->mPin->mClazzSet.add (clazz ,layout) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout> (SingletonProcImplHolder ()) ;
} ;