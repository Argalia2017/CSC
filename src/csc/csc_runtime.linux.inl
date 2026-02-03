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
#ifdef __CSC_SYSTEM_LINUX__
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/mman.h>
#include <sys/syscall.h>
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

	Length thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	Flag thread_uid () const override {
		return Flag (syscall (SYS_gettid)) ;
	}

	void thread_sleep (CR<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	Flag process_uid () const override {
		return Flag (syscall (SYS_getpid)) ;
	}

	void process_exit () const override {
		std::quick_exit (0) ;
	}

	String<Str> library_file (CR<csc_handle_t> addr) const override {
		const auto r1x = addr ;
		auto rax = Dl_info () ;
		const auto r2x = dladdr (r1x ,(&rax)) ;
		assume (r2x != ZERO) ;
		const auto r3x = Slice (Flag (rax.dli_fname) ,SLICE_MAX_SIZE::expr ,1).eos () ;
		return String<Str> (r3x) ;
	}

	String<Str> library_main () const override {
		String<Str> ret = String<Str>::make () ;
		const auto r1x = String<Str> (slice ("/proc/self/exe")) ;
		const auto r2x = Index (readlink (r1x ,ret ,csc_size_t (ret.size ()))) ;
		ret.trunc (r2x) ;
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
		auto rax = TextReader (info.borrow ()) ;
		auto rbx = String<Stru> () ;
		rax >> GAP ;
		rax >> ReadBlank (rbx) ;
		const auto r1x = StringParse<Val64>::make (rbx) ;
		assume (r1x == uid) ;
		rax >> GAP ;
		rax >> slice ("(") ;
		while (TRUE) {
			const auto r2x = rax.poll (TYPE<Stru32>::expr) ;
			if (r2x == Stru32 (0X00))
				break ;
			if (r2x == Stru32 (')'))
				break ;
			assume (r2x != Stru32 ('(')) ;
		}
		rax >> GAP ;
		rax >> ReadBlank (rbx) ;
		assume (rbx.length () == 1) ;
		for (auto &&i : range (0 ,18)) {
			noop (i) ;
			rax >> GAP ;
			rax >> ReadBlank (rbx) ;
		}
		rax >> GAP ;
		rax >> ReadBlank (rbx) ;
		const auto r3x = StringParse<Val64>::make (rbx) ;
		return Quad (r3x) ;
	}

	void initialize (CR<RefBuffer<Byte>> snapshot_) override {
		self.mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_STEP::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<Byte>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.poll (TYPE<Val64>::expr) ;
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
			rax << EOS ;
		}
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
			const auto r1x = csc_enum_t (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
			const auto r2x = csc_enum_t (r1x | RTLD_NOLOAD) ;
			me = csc_device_t (dlopen (self.mFile ,r2x)) ;
			if (me != NULL)
				return ;
			me = csc_device_t (dlopen (self.mFile ,r1x)) ;
			if (me != NULL)
				return ;
			self.mLastError = Flag (errno) ;
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
		Flag ret = Flag (dlsym (self.mLibrary ,name)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			self.mLastError = Flag (errno) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<Str> error () const override {
		String<Str> ret = String<Str>::make () ;
		const auto r1x = Flag (dlerror ()) ;
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

	static VR<SingletonRoot> root_ptr () {
		static auto mInstance = SingletonRoot () ;
		return mInstance ;
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
			const auto r1x = Flag (self.mLocal.mAddress1) ;
			assume (r1x != ZERO) ;
			auto &&rax = keep[TYPE<SingletonRoot>::expr] (Pointer::make (r1x)) ;
			self.mRoot = Ref<SingletonRoot>::reference (rax) ;
		}
	}

	void init_local () {
		if (self.mMapping.exist ())
			return ;
		self.mMapping = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_EXCL) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			const auto r3x = shm_open (self.mName ,r1x ,r2x) ;
			assume (r3x != NONE) ;
			const auto r4x = ftruncate (r3x ,SIZE_OF<SingletonLocal>::expr) ;
			assume (r4x == 0) ;
			me = csc_handle_t (self.mName.ref) ;
		} ,[&] (VR<csc_handle_t> me) {
			shm_unlink (csc_string_t (me)) ;
		}) ;
		self.mLocal.mReserve1 = Quad (self.mUid) ;
		self.mLocal.mAddress1 = Quad (address (root_ptr ())) ;
		self.mLocal.mReserve2 = abi_reserve () ;
		self.mLocal.mAddress2 = Quad (address (root_ptr ())) ;
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
		return QUAD_ENDIAN ;
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
		const auto r1x = Pin<Set<Clazz>> (self.mRoot->mClazzSet) ;
		r1x->add (clazz ,layout) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout> (SingletonProcImplHolder ()) ;
} ;