#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_runtime.hpp"

#ifdef __CSC_CONFIG_STRW__
#error "∑(っ°Д° ;)っ : unsupported"
#endif

#include "csc_end.h"
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <cxxabi.h>

#ifdef __CSC_SYSTEM_EMBEDDED__
#include <process.h>
#include <devctl.h>
#include <unwind.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/procfs.h>
#endif

#include <cstdlib>
#include <thread>
#include "csc_begin.h"

namespace posix {
inline namespace {
using ::open ;
using ::close ;
using ::read ;
using ::write ;
} ;
} ;

namespace CSC {
struct Dl_info_symbol {
	int mStatus ;
	csc_size_t mLength ;
	DEF<char[1]> mSymbol ;
} ;

struct UnwindBacktraceContext {
	Flag mBuffer ;
	Length mSize ;
	Length mCount ;
} ;

class RuntimeProcImplHolder final implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	Tuple<Flag ,Flag> stack_limit () const override {
		Tuple<Flag ,Flag> ret ;
		const auto r1x = String<Str> (slice ("/proc/self/ctl")) ;
		const auto r2x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			me = posix::open (r1x ,O_RDWR) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			posix::close (me) ;
		}) ;
		auto rax = procfs_status () ;
		inline_memset (rax) ;
		rax.tid = pthread_t (gettid ()) ;
		const auto r3x = devctl (r2x ,DCMD_PROC_TIDSTATUS ,(&rax) ,SIZE_OF<procfs_status>::expr ,NULL) ;
		assume (r3x == EOK) ;
		ret.m1st = Flag (rax.stkbase) ;
		ret.m2nd = ret.m1st + Flag (rax.stksize) ;
		return move (ret) ;
	}

	String<Str> stack_trace (CR<Length> skip) const override {
		assert (skip >= 0) ;
		String<Str> ret = String<Str> (15 * 1024) ;
		auto mWriter = TextWriter (ret.borrow ()) ;
		const auto r1x = 128 * SIZE_OF<csc_handle_t>::expr ;
		const auto r2x = r1x + SIZE_OF<Dl_info_symbol>::expr + 1024 ;
		const auto r3x = r2x + SIZE_OF<Dl_info>::expr ;
		const auto r4x = address (ret[ret.size () - r3x]) ;
		const auto r5x = unwind_backtrace (r4x ,128) - skip ;
		auto &&rax = keep[TYPE<Dl_info_symbol>::expr] (Pointer::make (r4x + r1x)) ;
		auto &&rbx = keep[TYPE<Dl_info>::expr] (Pointer::make (r4x + r2x)) ;
		for (auto &&i : range (0 ,r5x)) {
			const auto r6x = r4x + (skip + i) * SIZE_OF<csc_handle_t>::expr ;
			const auto r7x = csc_handle_t (bitwise (Pointer::make (r6x))) ;
			mWriter << slice ("#") << WriteAligned (i ,2) ;
			mWriter << slice (" [0X") ;
			mWriter << Quad (Flag (r7x)) ;
			mWriter << slice ("] : ") ;
			auto act = TRUE ;
			if ifdo (act) {
				const auto r8x = dladdr (r7x ,(&rbx)) ;
				if (r8x == ZERO)
					discard ;
				const auto r9x = Slice (Flag (rbx.dli_fname) ,SLICE_MAX_SIZE::expr ,1).eos () ;
				if (r9x.size () == 0)
					discard ;
				mWriter << slice_filename (r9x) ;
				mWriter << slice ("!") ;
				rax.mLength = 1024 ;
				abi::__cxa_demangle (rbx.dli_sname ,rax.mSymbol ,(&rax.mLength) ,(&rax.mStatus)) ;
				const auto r10x = rax.mStatus == 0 ? Flag (rax.mSymbol) : Flag (rbx.dli_sname) ;
				const auto r11x = Slice (r10x ,SLICE_MAX_SIZE::expr ,1).eos () ;
				if (r11x.size () == 0)
					discard ;
				mWriter << r11x ;
			}
			if ifdo (act) {
				mWriter << slice ("???") ;
			}
			mWriter << GAP ;
		}
		mWriter << EOS ;
		return move (ret) ;
	}

	Length unwind_backtrace (CR<Flag> buffer ,CR<Length> size_) const {
		auto rax = UnwindBacktraceContext () ;
		rax.mBuffer = buffer ;
		rax.mSize = size_ ;
		rax.mCount = 0 ;
		_Unwind_Backtrace (unwind_backtrace_step ,(&rax)) ;
		return rax.mCount ;
	}

	static _Unwind_Reason_Code unwind_backtrace_step (struct _Unwind_Context *context ,void *arg) {
		auto &&rax = keep[TYPE<UnwindBacktraceContext>::expr] (Pointer::make (Flag (arg))) ;
		if (rax.mCount >= rax.mSize)
			return _URC_END_OF_STACK ;
		const auto r1x = rax.mBuffer + rax.mCount * SIZE_OF<csc_handle_t>::expr ;
		bitwise (Pointer::make (r1x)) = csc_handle_t (_Unwind_GetIP (context)) ;
		rax.mCount++ ;
		return _URC_NO_REASON ;
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
		return Flag (gettid ()) ;
	}

	void thread_sleep (CR<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	Flag process_uid () const override {
		return Flag (getpid ()) ;
	}

	void process_exit () const override {
		std::exit (0) ;
	}

	String<Str> library_file (CR<Flag> addr) const override {
		const auto r1x = csc_handle_t (addr) ;
		auto rax = Dl_info () ;
		const auto r2x = dladdr (r1x ,(&rax)) ;
		assume (r2x != ZERO) ;
		const auto r3x = Slice (Flag (rax.dli_fname) ,SLICE_MAX_SIZE::expr ,1).eos () ;
		return String<Str> (r3x) ;
	}

	String<Str> library_main () const override {
		String<Str> ret = String<Str>::make () ;
		const auto r1x = String<Str> (slice ("/proc/self/exefile")) ;
		const auto r2x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			me = posix::open (r1x ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			posix::close (me) ;
		}) ;
		auto rbx = ret.size () ;
		rbx = posix::read (r2x ,ret.ref ,rbx) ;
		assume (rbx >= 0) ;
		ret.trunc (rbx) ;
		rbx = strcspn (ret.ref ,"\n") ;
		ret.trunc (rbx) ;
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
		const auto r1x = load_proc_file (uid) ;
		self.mProcessCode = process_code (r1x ,uid) ;
		self.mProcessTime = process_time (r1x ,uid) ;
	}

	String<Stru> load_proc_file (CR<Flag> uid) const {
		String<Stru> ret = String<Stru>::make () ;
		ret.fill (Stru32 (0X00)) ;
		const auto r1x = String<Str>::make (Format (slice ("/proc/$1/stat")) (uid)) ;
		try {
			auto rax = StreamFile (r1x) ;
			rax.open_r () ;
			auto rbx = ret.borrow () ;
			rax.set_short_read (TRUE) ;
			rax.read (rbx.ref) ;
		} catch (CR<Exception> e) {
			noop (e) ;
			ret.clear () ;
		}
		return move (ret) ;
	}

	Quad process_code (CR<String<Stru>> info ,CR<Flag> uid) const {
		return Quad (getpgid (pid_t (uid))) ;
	}

	Quad process_time (CR<String<Stru>> info ,CR<Flag> uid) const {
		if (info.length () == 0)
			return Quad (0X00) ;
		const auto r1x = String<Str>::make (Format (slice ("/proc/$1/ctl")) (uid)) ;
		const auto r2x = posix::open (r1x ,O_RDWR) ;
		if (r2x == NONE)
			return Quad (0X00) ;
		auto rax = procfs_info () ;
		inline_memset (rax) ;
		const auto r3x = devctl (r2x ,DCMD_PROC_INFO ,(&rax) ,SIZE_OF<procfs_info>::expr ,NULL) ;
		posix::close (r2x) ;
		if (r3x != EOK)
			return Quad (0X00) ;
		return Quad (rax.start_time) ;
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
			const auto r1x = supported_Bsymbolic () ;
			const auto r2x = csc_enum_t (RTLD_NOLOAD | r1x) ;
			me = csc_device_t (dlopen (self.mFile ,r2x)) ;
			if (me != NULL)
				return ;
			const auto r3x = csc_enum_t (RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE | r1x) ;
			me = csc_device_t (dlopen (self.mFile ,r3x)) ;
			if (me != NULL)
				return ;
			self.mLastError = Flag (dlerror ()) ;
			assume (FALSE) ;
		} ,[&] (VR<csc_device_t> me) {
			noop () ;
		}) ;
	}

	csc_enum_t supported_Bsymbolic () const {
		csc_enum_t ret = 0 ;
#ifdef RTLD_DEEPBIND
		ret |= RTLD_DEEPBIND ;
#endif
#ifdef RTLD_GROUP
		ret |= RTLD_GROUP ;
#endif
		return move (ret) ;
	}

	String<Str> library_file () const override {
		return self.mFile ;
	}

	Flag load (CR<String<Str>> name) override {
		assert (name.length () > 0) ;
		Flag ret = Flag (dlsym (self.mLibrary ,name)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			self.mLastError = Flag (dlerror ()) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<Str> error () const override {
		String<Str> ret = String<Str>::make () ;
		const auto r1x = self.mLastError ;
		assume (r1x != ZERO) ;
		const auto r2x = Slice (r1x ,SLICE_MAX_SIZE::expr ,1).eos () ;
		ret = String<Str>::make (Format (slice ("LastError = $1 : $2")) (r1x ,r2x)) ;
		return move (ret) ;
	}
} ;

static const auto mLibraryExternal = External<LibraryHolder ,LibraryLayout> (LibraryImplHolder ()) ;

class SingletonProcImplHolder final implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		self.mUid = RuntimeProc::process_uid () ;
		self.mName = String<Str>::make (slice ("/CSC_Singleton_") ,self.mUid) ;
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
				load_local () ;
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
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			const auto r3x = shm_open (self.mName ,r1x ,r2x) ;
			assume (r3x != NONE) ;
			const auto r4x = ftruncate (r3x ,SIZE_OF<SingletonLocal>::expr) ;
			assume (r4x == 0) ;
			me = csc_handle_t (self.mName.ref) ;
		} ,[&] (VR<csc_handle_t> me) {
			shm_unlink (csc_string_t (me)) ;
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
		const auto r1x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			me = shm_open (self.mName ,O_RDONLY ,0) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = mmap (NULL ,SIZE_OF<SingletonLocal>::expr ,PROT_READ ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VR<csc_handle_t> me) {
			munmap (me ,SIZE_OF<SingletonLocal>::expr) ;
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
		const auto r1x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			me = shm_open (self.mName ,O_RDWR ,0) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = mmap (NULL ,SIZE_OF<SingletonLocal>::expr ,PROT_WRITE ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VR<csc_handle_t> me) {
			munmap (me ,SIZE_OF<SingletonLocal>::expr) ;
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
#elif defined __CSC_SYSTEM_EMBEDDED__
		ret |= Quad (0X00000300) ;
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