﻿#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_runtime.hpp"

#include "csc_end.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/sendfile.h>
#include "csc_begin.h"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include "csc_begin.h"

#ifndef USE_LINUX_HANDLE
#define USE_LINUX_HANDLE

using HMODULE = CSC::csc_pointer_t ;
using HANDLE = CSC::csc_pointer_t ;
using HFILE = int ;
#endif

namespace std {
inline namespace extand {
using ::open ;
using ::close ;
using ::read ;
using ::write ;
} ;
} ;

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
		const auto r1x = std::this_thread::get_id () ;
		return FLAG (bitwise[TYPE<QUAD>::expr] (r1x)) ;
	}

	void thread_sleep (CREF<Time> time) const override {
		const auto r1x = time.borrow () ;
		std::this_thread::sleep_for (r1x->mTime) ;
	}

	void thread_yield () const override {
		std::this_thread::yield () ;
	}

	FLAG process_uid () const override {
		return FLAG (syscall (SYS_gettid)) ;
	}

	void process_exit () const override {
		std::quick_exit (0) ;
	}

	String<STR> working_path () const override {
		String<STR> ret = String<STR>::make () ;
		if ifdo (TRUE) {
			const auto r1x = getcwd (ret ,csc_size_t (ret.size ())) ;
			if (r1x != NULL)
				discard ;
			ret.clear () ;
		}
		ret = Path (ret).fetch () ;
		return move (ret) ;
	}

	String<STR> library_path () const override {
		String<STR> ret = String<STR>::make () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("/proc/self/exe")) ;
			const auto r2x = readlink (r1x ,ret ,csc_size_t (ret.size ())) ;
			if (r2x >= 0)
				discard ;
			ret.clear () ;
		}
		ret = Path (ret).path () ;
		return move (ret) ;
	}

	String<STR> library_name () const override {
		String<STR> ret = String<STR>::make () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("/proc/self/exe")) ;
			const auto r2x = readlink (r1x ,ret ,csc_size_t (ret.size ())) ;
			if (r2x >= 0)
				discard ;
			ret.clear () ;
		}
		ret = Path (ret).name () ;
		return move (ret) ;
	}
} ;

static const auto mRuntimeProcExternal = External<RuntimeProcHolder ,RuntimeProcLayout>::declare (RuntimeProcImplHolder ()) ;

struct ProcessImplLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

class ProcessImplHolder final implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_SIZE = ENUM<128> ;

public:
	void initialize (CREF<FLAG> uid) override {
		fake.mThis = AutoRef<ProcessImplLayout>::make () ;
		fake.mThis->mUid = uid ;
		const auto r1x = load_proc_file (uid) ;
		fake.mThis->mProcessCode = process_code (r1x ,uid) ;
		fake.mThis->mProcessTime = process_time (r1x ,uid) ;
	}

	String<STRU8> load_proc_file (CREF<FLAG> uid) const {
		String<STRU8> ret = String<STRU8>::make () ;
		const auto r1x = String<STR>::make (Format (slice ("/proc/$1/stat")) (uid)) ;
		auto rax = StreamFile (r1x) ;
		rax.open_r () ;
		auto rbx = ret.borrow () ;
		rax.read (rbx.leak ()) ;
		return move (ret) ;
	}

	QUAD process_code (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
		return QUAD (getpgid (pid_t (uid))) ;
	}

	QUAD process_time (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
		auto rax = TextReader (info.borrow ()) ;
		auto rbx = String<STRU8> () ;
		rax >> GAP ;
		rax >> rbx ;
		const auto r1x = StringParse<VAL64>::make (rbx) ;
		assume (r1x == uid) ;
		rax >> GAP ;
		rax >> slice ("(") ;
		while (TRUE) {
			const auto r2x = rax.poll (TYPE<STRU32>::expr) ;
			if (r2x == STRU32 (0X00))
				break ;
			if (r2x == STRU32 (')'))
				break ;
			assume (r2x != STRU32 ('(')) ;
		}
		rax >> GAP ;
		rax >> rbx ;
		assume (rbx.length () == 1) ;
		for (auto &&i : iter (0 ,18)) {
			noop (i) ;
			rax >> GAP ;
			rax >> rbx ;
		}
		rax >> GAP ;
		rax >> rbx ;
		const auto r3x = StringParse<VAL64>::make (rbx) ;
		for (auto &&i : iter (19 ,49)) {
			noop (i) ;
			rax >> GAP ;
			rax >> rbx ;
		}
		rax >> GAP ;
		rax >> EOS ;
		return QUAD (r3x) ;
	}

	void initialize (CREF<RefBuffer<BYTE>> snapshot_) override {
		fake.mThis = AutoRef<ProcessImplLayout>::make () ;
		fake.mThis->mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_SIZE::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (snapshot_)) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			const auto r1x = rax.poll (TYPE<VAL32>::expr) ;
			fake.mThis->mUid = FLAG (r1x) ;
			rax >> GAP ;
			rax >> fake.mThis->mProcessCode ;
			rax >> GAP ;
			rax >> fake.mThis->mProcessTime ;
			rax >> GAP ;
			rax >> EOS ;
		} catch (CREF<Exception> e) {
			noop (e) ;
		}
	}

	BOOL equal (CREF<ProcessLayout> that) const override {
		const auto r1x = inline_equal (fake.mThis->mUid ,that.mThis->mUid) ;
		if (!r1x)
			return r1x ;
		const auto r2x = inline_equal (fake.mThis->mProcessCode ,that.mThis->mProcessCode) ;
		if (!r2x)
			return r2x ;
		const auto r3x = inline_equal (fake.mThis->mProcessTime ,that.mThis->mProcessTime) ;
		if (!r3x)
			return r3x ;
		return TRUE ;
	}

	FLAG process_uid () const override {
		return fake.mThis->mUid ;
	}

	RefBuffer<BYTE> snapshot () const override {
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (PROCESS_SNAPSHOT_SIZE::expr) ;
		auto rax = ByteWriter (Ref<RefBuffer<BYTE>>::reference (ret)) ;
		rax << slice ("CSC_Process") ;
		if ifdo (TRUE) {
			rax << GAP ;
			rax << VAL64 (fake.mThis->mUid) ;
			rax << GAP ;
			rax << fake.mThis->mProcessCode ;
			rax << GAP ;
			rax << fake.mThis->mProcessTime ;
		}
		rax << GAP ;
		rax << EOS ;
		return move (ret) ;
	}
} ;

static const auto mProcessExternal = External<ProcessHolder ,ProcessLayout>::declare (ProcessImplHolder ()) ;

struct LibraryImplLayout {
	UniqueRef<HMODULE> mLibrary ;
	FLAG mLastError ;
} ;

class LibraryImplHolder final implement Fat<LibraryHolder ,LibraryLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		const auto r1x = Path (file).name () ;
		assert (r1x.length () > 0) ;
		const auto r2x = StringProc::stra_from_strs (r1x) ;
		fake.mThis->mLibrary = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
			const auto r3x = csc_enum_t (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
			const auto r4x = csc_enum_t (r3x | RTLD_NOLOAD) ;
			me = dlopen (r2x ,r4x) ;
			if (me != NULL)
				return ;
			me = dlopen (r2x ,r3x) ;
			if (me != NULL)
				return ;
			fake.mThis->mLastError = FLAG (errno) ;
			assume (FALSE) ;
		} ,[&] (VREF<HMODULE> me) {
			noop () ;
		}) ;
	}

	FLAG load (CREF<String<STR>> name) override {
		assert (name.length () > 0) ;
		const auto r1x = StringProc::stra_from_strs (name) ;
		FLAG ret = FLAG (dlsym (fake.mThis->mLibrary ,r1x)) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			fake.mThis->mLastError = FLAG (errno) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<STR> error () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = FLAG (dlerror ()) ;
		assume (r1x != ZERO) ;
		const auto r2x = Slice (r1x ,SLICE_MAX_SIZE::expr ,1).eos () ;
		ret = String<STR>::make (Format (slice ("LastError = $1 : $2")) (r1x ,r2x)) ;
		return move (ret) ;
	}
} ;

static const auto mLibraryExternal = External<LibraryHolder ,LibraryLayout>::declare (LibraryImplHolder ()) ;

struct SingletonLayout {
	Mutex mMutex ;
	Pin<Set<Clazz>> mClazzSet ;
} ;

class SingletonRoot implement Pin<SingletonLayout> {
public:
	static CREF<SingletonRoot> instance () {
		return memorize ([&] () {
			return SingletonRoot () ;
		}) ;
	}
} ;

struct SingletonPipe {
	QUAD mReserve1 ;
	QUAD mAddress1 ;
	QUAD mReserve2 ;
	QUAD mAddress2 ;
	QUAD mReserve3 ;
} ;

struct SingletonProcImplLayout {
	FLAG mUid ;
	String<STR> mName ;
	UniqueRef<Tuple<HFILE ,String<STR>>> mPipe ;
	Ref<SingletonLayout> mRoot ;
	FLAG mAddress ;
} ;

class SingletonProcImplHolder final implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<SingletonProcImplLayout>::make () ;
		fake.mThis->mUid = RuntimeProc::process_uid () ;
		fake.mThis->mName = String<STR>::make (slice ("/CSC_Singleton_") ,fake.mThis->mUid) ;
		fake.mThis->mAddress = ZERO ;
		link_pipe () ;
	}

	void link_pipe () {
		auto act = TRUE ;
		if ifdo (act) {
			try {
				load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				init_pipe () ;
				save_pipe () ;
				load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
	}

	void init_pipe () {
		if (fake.mThis->mPipe.exist ())
			return ;
		fake.mThis->mPipe = UniqueRef<Tuple<HFILE ,String<STR>>> ([&] (VREF<Tuple<HFILE ,String<STR>>> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_EXCL) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me.m1st = shm_open (fake.mThis->mName ,r1x ,r2x) ;
			assume (me.m1st != NONE) ;
			me.m2nd = fake.mThis->mName ;
		} ,[&] (VREF<Tuple<HFILE ,String<STR>>> me) {
			shm_unlink (me.m2nd) ;
		}) ;
		const auto r3x = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate (fake.mThis->mPipe->m1st ,SIZE_OF<SingletonPipe>::expr) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mAddress = address (SingletonRoot::instance ().self) ;
		auto &&rax = keep[TYPE<SingletonRoot>::expr] (Pointer::make (fake.mThis->mAddress)) ;
		rax->mMutex = NULL ;
	}

	void load_pipe () {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = shm_open (fake.mThis->mName ,O_RDONLY ,0) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = mmap (NULL ,SIZE_OF<SingletonPipe>::expr ,PROT_READ ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			munmap (me ,SIZE_OF<SingletonPipe>::expr) ;
		}) ;
		const auto r3x = FLAG (r2x.self) ;
		auto rax = SingletonPipe () ;
		inline_memcpy (Pointer::from (rax) ,Pointer::make (r3x) ,SIZE_OF<SingletonPipe>::expr) ;
		assume (rax.mReserve1 == QUAD (fake.mThis->mUid)) ;
		assume (rax.mReserve2 == abi_reserve ()) ;
		assume (rax.mReserve3 == ctx_reserve ()) ;
		assume (rax.mAddress1 == rax.mAddress2) ;
		const auto r4x = FLAG (rax.mAddress1) ;
		assume (r4x != ZERO) ;
		auto &&rbx = keep[TYPE<SingletonLayout>::expr] (Pointer::make (r4x)) ;
		fake.mThis->mRoot = Ref<SingletonLayout>::reference (rbx) ;
	}

	void save_pipe () {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = shm_open (fake.mThis->mName ,O_RDWR ,0) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			noop () ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = mmap (NULL ,SIZE_OF<SingletonPipe>::expr ,PROT_WRITE ,MAP_SHARED ,r1x ,0) ;
			replace (me ,MAP_FAILED ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			munmap (me ,SIZE_OF<SingletonPipe>::expr) ;
		}) ;
		const auto r3x = FLAG (r2x.self) ;
		auto rax = SingletonPipe () ;
		rax.mReserve1 = QUAD (fake.mThis->mUid) ;
		rax.mAddress1 = QUAD (fake.mThis->mAddress) ;
		rax.mReserve2 = abi_reserve () ;
		rax.mAddress2 = QUAD (fake.mThis->mAddress) ;
		rax.mReserve3 = ctx_reserve () ;
		inline_memcpy (Pointer::make (r3x) ,Pointer::from (rax) ,SIZE_OF<SingletonPipe>::expr) ;
	}

	QUAD abi_reserve () const override {
		QUAD ret = QUAD (0X00) ;
#ifdef __CSC_VER_DEBUG__
		ret |= QUAD (0X00000001) ;
#elif defined __CSC_VER_UNITTEST__
		ret |= QUAD (0X00000001) ;
#elif defined __CSC_VER_RELEASE__
		ret |= QUAD (0X00000001) ;
#endif
#ifdef __CSC_COMPILER_MSVC__
		ret |= QUAD (0X00000010) ;
#elif defined __CSC_COMPILER_GNUC__
		ret |= QUAD (0X00000020) ;
#elif defined __CSC_COMPILER_CLANG__
		ret |= QUAD (0X00000040) ;
#endif
#ifdef __CSC_SYSTEM_WINDOWS__
		ret |= QUAD (0X00000100) ;
#elif defined __CSC_SYSTEM_LINUX__
		ret |= QUAD (0X00000200) ;
#endif
#ifdef __CSC_TARGET_EXE__
		ret |= QUAD (0X00001000) ;
#elif defined __CSC_TARGET_DLL__
		ret |= QUAD (0X00001000) ;
#elif defined __CSC_TARGET_LIB__
		ret |= QUAD (0X00001000) ;
#endif
#ifdef __CSC_PLATFORM_X86__
		ret |= QUAD (0X00010000) ;
#elif defined __CSC_PLATFORM_X64__
		ret |= QUAD (0X00020000) ;
#elif defined __CSC_PLATFORM_ARM__
		ret |= QUAD (0X00030000) ;
#elif defined __CSC_PLATFORM_ARM64__
		ret |= QUAD (0X00040000) ;
#endif
#ifdef __CSC_CONFIG_VAL32__
		ret |= QUAD (0X00100000) ;
#elif defined __CSC_CONFIG_VAL64__
		ret |= QUAD (0X00200000) ;
#endif
#ifdef __CSC_CONFIG_STRA__
		ret |= QUAD (0X01000000) ;
#elif defined __CSC_CONFIG_STRW__
		ret |= QUAD (0X02000000) ;
#endif
#ifdef __CSC_CXX_LATEST__
		ret |= QUAD (0X10000000) ;
#else
		ret |= QUAD (0X10000000) ;
#endif
		return move (ret) ;
	}

	QUAD ctx_reserve () const override {
		return QUAD (0XAAAABBBBCCCCDDDD) ;
	}

	FLAG load (CREF<Clazz> clazz) const override {
		Scope<Mutex> anonymous (fake.mThis->mRoot->mMutex) ;
		FLAG ret = fake.mThis->mRoot->mClazzSet->map (clazz) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> pointer) const override {
		assert (pointer != ZERO) ;
		assert (pointer != NONE) ;
		Scope<Mutex> anonymous (fake.mThis->mRoot->mMutex) ;
		fake.mThis->mRoot->mClazzSet->add (clazz ,pointer) ;
	}
} ;

static const auto mSingletonProcExternal = External<SingletonProcHolder ,SingletonProcLayout>::declare (SingletonProcImplHolder ()) ;

struct PathImplLayout {
	String<STR> mPathName ;
	Deque<INDEX> mSeparator ;
} ;

class PathImplHolder final implement Fat<PathHolder ,PathLayout> {
private:
	using HDIR = DEF<DIR *> ;
	using HDIRENT = DEF<dirent * > ;
	using STAT_INFO = DEF<struct stat> ;

public:
	void initialize (RREF<String<STR>> pathname) override {
		auto rax = PathImplLayout () ;
		rax.mPathName = move (pathname) ;
		rax.mSeparator.add (NONE) ;
		const auto r1x = rax.mPathName.length () ;
		assume (r1x > 0) ;
		for (auto &&i : iter (0 ,r1x)) {
			if (!is_separator (rax.mPathName[i]))
				continue ;
			rax.mSeparator.add (i) ;
			rax.mPathName[i] = STR ('/') ;
		}
		rax.mSeparator.add (r1x) ;
		if ifdo (TRUE) {
			if (rax.mSeparator[1] != 0)
				discard ;
			rax.mSeparator.take () ;
		}
		if ifdo (TRUE) {
			INDEX ix = rax.mSeparator[rax.mSeparator.length () - 1] ;
			if (ix != r1x - 1)
				discard ;
			rax.mPathName.trunc (ix) ;
			rax.mSeparator.pop () ;
		}
		fake.mThis = Ref<PathImplLayout>::make (move (rax)) ;
	}

	BOOL is_separator (CREF<STRU32> str) const {
		if (str == STRU32 ('\\'))
			return TRUE ;
		if (str == STRU32 ('/'))
			return TRUE ;
		return FALSE ;
	}

	void initialize (CREF<PathLayout> that) override {
		fake.mThis = that.mThis.share () ;
	}

	String<STR> fetch () const override {
		return fake.mThis->mPathName ;
	}

	PathLayout root () const override {
		PathLayout ret ;
		if ifdo (TRUE) {
			if (!StreamProc::is_alpha (fake.mThis->mPathName[0]))
				discard ;
			if (fake.mThis->mPathName[1] != STRU32 (':'))
				discard ;
			ret = Path (Slice (FLAG (fake.mThis->mPathName.self) ,2 ,SIZE_OF<STR>::expr)) ;
		}
		return move (ret) ;
	}

	PathLayout child (CREF<Slice> name) const override {
		PathLayout ret ;
		ret = Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
		return move (ret) ;
	}

	PathLayout child (CREF<Format> name) const override {
		PathLayout ret ;
		ret = Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
		return move (ret) ;
	}

	PathLayout child (CREF<String<STR>> name) const override {
		PathLayout ret ;
		ret = Path (String<STR>::make (fake.mThis->mPathName ,slice ("/") ,name)) ;
		return move (ret) ;
	}

	Array<PathLayout> list () const override {
		auto rax = HDIRENT (NULL) ;
		const auto r1x = StringProc::stra_from_strs (fake.mThis->mPathName) ;
		const auto r2x = UniqueRef<HDIR> ([&] (VREF<HDIR> me) {
			me = opendir (r1x) ;
		} ,[&] (VREF<HDIR> me) {
			if (me == NULL)
				return ;
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			rax = readdir (r2x) ;
			assert (Slice (rax->d_name).eos () == slice (".")) ;
			rax = readdir (r2x) ;
			assert (Slice (rax->d_name).eos () == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r2x) ;
				if (rax == NULL)
					break ;
				rbx.add (Slice (rax->d_name)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (rbx.length ()) ;
		for (auto &&i : ret.range ())
			ret[i] = child (rbx[i]) ;
		return move (ret) ;
	}

	Array<PathLayout> list (CREF<LENGTH> size_) const override {
		auto rax = HDIRENT (NULL) ;
		const auto r1x = StringProc::stra_from_strs (fake.mThis->mPathName) ;
		const auto r2x = UniqueRef<HDIR> ([&] (VREF<HDIR> me) {
			me = opendir (r1x) ;
		} ,[&] (VREF<HDIR> me) {
			if (me == NULL)
				return ;
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			rax = readdir (r2x) ;
			assert (Slice (rax->d_name).eos () == slice (".")) ;
			rax = readdir (r2x) ;
			assert (Slice (rax->d_name).eos () == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r2x) ;
				if (rax == NULL)
					break ;
				if (rbx.length () >= size_)
					break ;
				rbx.add (Slice (rax->d_name)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (size_) ;
		const auto r3x = inline_min (rbx.length () ,size_) ;
		for (auto &&i : iter (0 ,r3x))
			ret[i] = child (rbx[i]) ;
		for (auto &&i : iter (r3x ,size_))
			PathHolder::hold (ret[i])->initialize (fake) ;
		return move (ret) ;
	}

	BOOL equal (CREF<PathLayout> that) const override {
		return fake.mThis->mPathName == that.mThis->mPathName ;
	}

	BOOL is_file () const override {
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = stat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_dire () const override {
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = stat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_link () const override {
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = stat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISLNK (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	Deque<String<STR>> decouple () const override {
		Deque<String<STR>> ret = Deque<String<STR>> (fake.mThis->mSeparator.length ()) ;
		for (auto &&i : iter (1 ,fake.mThis->mSeparator.length ())) {
			const auto r1x = fake.mThis->mSeparator[i - 1] + 1 ;
			const auto r2x = fake.mThis->mSeparator[i] ;
			const auto r3x = address (fake.mThis->mPathName.at (r1x)) ;
			const auto r4x = Slice (r3x ,r2x - r1x ,SIZE_OF<STR>::expr) ;
			ret.add (r4x) ;
		}
		return move (ret) ;
	}

	String<STR> path () const override {
		const auto r1x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] + 1 ;
		String<STR> ret = String<STR> (r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (0 ,r1x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> name () const override {
		const auto r1x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] + 1 ;
		const auto r2x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] ;
		String<STR> ret = String<STR> (r2x - r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r2x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> stem () const override {
		const auto r1x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 2] + 1 ;
		const auto r2x = find_last_dot_word () ;
		String<STR> ret = String<STR> (r2x - r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r2x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> extension () const override {
		const auto r1x = find_last_dot_word () ;
		const auto r2x = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] ;
		String<STR> ret = String<STR> (r2x - r1x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r2x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	INDEX find_last_dot_word () const {
		INDEX ix = fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] - 1 ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (fake.mThis->mPathName[ix] == STRU32 ('.'))
				return ix ;
			if (!StreamProc::is_word (fake.mThis->mPathName[ix]))
				break ;
			ix-- ;
		}
		return fake.mThis->mSeparator[fake.mThis->mSeparator.length () - 1] ;
	}
} ;

static const auto mPathExternal = External<PathHolder ,PathLayout>::declare (PathImplHolder ()) ;

struct FileProcImplLayout {
	Mutex mMutex ;
	Pin<List<UniqueRef<String<STR>>>> mLockDirectory ;
} ;

class FileProcImplHolder final implement Fat<FileProcHolder ,FileProcLayout> {
public:
	void initialize () override {
		fake.mThis = AutoRef<FileProcImplLayout>::make () ;
		fake.mThis->mMutex = NULL ;
	}

	RefBuffer<BYTE> load_file (CREF<String<STR>> file) const override {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = std::open (file ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = file_size (r1x) ;
		assume (r2x < VAL32_MAX) ;
		const auto r3x = LENGTH (r2x) ;
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (r3x) ;
		auto rax = r3x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (r1x ,(&ret[r3x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
		return move (ret) ;
	}

	VAL64 file_size (CREF<HFILE> handle) const {
		const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> item) const override {
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (file ,r2x ,r3x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r4x = item.size () ;
		auto rax = r4x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (r1x ,(&item[r4x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
	}

	Ref<RefBuffer<BYTE>> load_asset (CREF<String<STR>> file) const override {
		const auto r1x = dlopen (NULL ,RTLD_LOCAL) ;
		assume (r1x != NULL) ;
		const auto r2x = String<STRA>::make (file ,slice ("input_txt_start")) ;
		const auto r3x = String<STRA>::make (file ,slice ("input_txt_end")) ;
		const auto r4x = FLAG (dlsym (r1x ,r2x)) ;
		assume (r4x != ZERO) ;
		const auto r5x = FLAG (dlsym (r1x ,r3x)) ;
		assume (r5x != ZERO) ;
		const auto r6x = r5x - r4x ;
		assume (r6x >= 0) ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r4x ,r6x)) ;
	}

	void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = std::open (src ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (dst ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r5x = file_size (r1x) ;
		assume (r5x < VAL32_MAX) ;
		sendfile (r1x ,r2x ,NULL ,r5x) ;
	}

	void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = rename (src ,dst) ;
		noop (r1x) ;
	}

	void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = link (src ,dst) ;
		noop (r1x) ;
	}

	void erase_file (CREF<String<STR>> file) const override {
		const auto r1x = unlink (file) ;
		noop (r1x) ;
	}

	void build_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
		auto rax = String<STR>::make () ;
		INDEX ix = 0 ;
		for (auto &&i : r1x) {
			rax.splice (ix ,i) ;
			ix = rax.length () ;
			mkdir (rax ,r2x) ;
			rax.splice (ix ,slice ("/")) ;
			ix++ ;
		}
	}

	void clear_dire (CREF<String<STR>> dire) const override {
		auto rax = Deque<Tuple<Path ,BOOL>> () ;
		rax.add ({Path (dire) ,TRUE}) ;
		clear_dire_push (rax ,0) ;
		rax.take () ;
		while (TRUE) {
			if (rax.empty ())
				break ;
			INDEX ix = rax.tail () ;
			auto act = TRUE ;
			if ifdo (act) {
				if (!rax[ix].m2nd)
					discard ;
				rax[ix].m2nd = FALSE ;
				clear_dire_push (rax ,ix) ;
			}
			if ifdo (act) {
				erase_dire (rax[ix].m1st) ;
				rax.take () ;
			}
		}
	}

	void clear_dire_push (VREF<Deque<Tuple<Path ,BOOL>>> queue ,CREF<INDEX> curr) const {
		const auto r1x = queue[curr].m1st.list () ;
		for (auto &&i : r1x) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!i.is_link ())
					discard ;
				erase_file (i) ;
			}
			if ifdo (act) {
				if (!i.is_file ())
					discard ;
				erase_file (i) ;
			}
			if ifdo (act) {
				if (!i.is_dire ())
					discard ;
				queue.add ({i ,TRUE}) ;
			}
		}
	}

	void erase_dire (CREF<String<STR>> dire) const override {
		unlink (dire) ;
	}

	BOOL lock_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).child (slice (".lockdirectory")) ;
		const auto r2x = Process (RuntimeProc::process_uid ()) ;
		if ifdo (TRUE) {
			if (r1x.is_file ())
				discard ;
			const auto r3x = FileProc::load_file (r1x) ;
			const auto r4x = Process (r3x) ;
			const auto r5x = Process (r4x.process_uid ()) ;
			if (r4x != r5x)
				discard ;
			return r4x == r2x ;
		}
		lock_dire_push (r1x ,r2x.snapshot ()) ;
		return TRUE ;
	}

	void lock_dire_push (CREF<Path> file ,CREF<RefBuffer<BYTE>> snapshot_) const {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		auto rax = UniqueRef<String<STR>> ([&] (VREF<String<STR>> me) {
			me = file ;
			FileProc::save_file (me ,snapshot_) ;
		} ,[&] (VREF<String<STR>> me) {
			FileProc::erase_dire (me) ;
		}) ;
		fake.mThis->mLockDirectory->add (move (rax)) ;
	}
} ;

static const auto mFileProcExternal = External<FileProcHolder ,FileProcLayout>::declare (FileProcImplHolder ()) ;

struct StreamFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HFILE> mReadPipe ;
	UniqueRef<HFILE> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;
} ;

class StreamFileImplHolder final implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void initialize (RREF<String<STR>> file) override {
		fake.mThis = AutoRef<StreamFileImplLayout>::make () ;
		fake.mThis->mFile = move (file) ;
		fake.mThis->mFileSize = 0 ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_r () override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mReadPipe) ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = size_ ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
	}

	void open_a () override {
		assert (!fake.mThis->mReadPipe.exist ()) ;
		assert (!fake.mThis->mWritePipe.exist ()) ;
		fake.mThis->mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mReadPipe) ;
		fake.mThis->mRead = 0 ;
		fake.mThis->mWrite = 0 ;
		if ifdo (TRUE) {
			const auto r5x = VAL64 (lseek64 (fake.mThis->mWritePipe ,0 ,SEEK_END)) ;
			if (r5x <= 0)
				discard ;
			fake.mThis->mWrite += r5x ;
		}
	}

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HFILE> handle) const {
		const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mReadPipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (fake.mThis->mReadPipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		fake.mThis->mRead += r1x ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mWritePipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (fake.mThis->mWritePipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		fake.mThis->mWrite += r1x ;
	}

	void flush () override {
		if (!fake.mThis->mWritePipe.exist ())
			return ;
		fsync (fake.mThis->mWritePipe) ;
	}
} ;

static const auto mStreamFileExternal = External<StreamFileHolder ,StreamFileLayout>::declare (StreamFileImplHolder ()) ;

struct BufferFileHeader {
	VAL64 mFileEndian ;
	VAL64 mFileSize ;
	VAL64 mBlockSize ;
	VAL64 mBlockStep ;
	VAL64 mBlockLength ;
	VAL64 mChunkStep ;
	VAL64 mChunkSize ;
	VAL64 mChunkLength ;
} ;

struct BufferFileChunk {
	VAL64 mIndex ;
	VAL64 mCacheTime ;
	UniqueRef<Tuple<FLAG ,FLAG>> mBlock ;
} ;

struct BufferFileImplLayout {
	String<STR> mFile ;
	UniqueRef<HFILE> mPipe ;
	UniqueRef<LENGTH> mMapping ;
	VAL64 mFileSize ;
	VAL64 mBlockStep ;
	VAL64 mChunkStep ;
	csc_enum_t mFileMapFlag ;
	Box<BufferFileHeader> mHeader ;
	Set<VAL64> mCacheSet ;
	List<BufferFileChunk> mCacheList ;
	VAL64 mCacheTimer ;
} ;

class BufferFileImplHolder final implement Fat<BufferFileHolder ,BufferFileLayout> {
private:
	using BLOCK_STEP_SIZE = ENUM<1024> ;
	using CHUNK_STEP_SIZE = ENUM<4194304> ;
	using HEADER_SIZE = ENUM<65536> ;

public:
	void initialize (RREF<String<STR>> file) override {
		fake.mThis = AutoRef<BufferFileImplLayout>::make () ;
		fake.mThis->mFile = move (file) ;
		fake.mThis->mFileSize = 0 ;
		fake.mThis->mFileMapFlag = 0 ;
		set_block_step (BLOCK_STEP_SIZE::expr) ;
		set_cache_size (1) ;
	}

	void set_block_step (CREF<LENGTH> step_) override {
		fake.mThis->mBlockStep = step_ ;
		fake.mThis->mChunkStep = CHUNK_STEP_SIZE::expr ;
	}

	void set_cache_size (CREF<LENGTH> size_) override {
		fake.mThis->mCacheSet = Set<VAL64> (size_) ;
		fake.mThis->mCacheList = List<BufferFileChunk> (size_) ;
		fake.mThis->mCacheTimer = 0 ;
	}

	void open_r () override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = file_size (fake.mThis->mPipe) ;
		fake.mThis->mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = 0 ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mFileMapFlag = csc_enum_t (PROT_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> size_) override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r3x = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
		const auto r4x = (size_ + r3x - 1) / r3x ;
		fake.mThis->mFileSize = HEADER_SIZE::expr + r4x * fake.mThis->mChunkStep ;
		fake.mThis->mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate64 (fake.mThis->mPipe ,fake.mThis->mFileSize) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!fake.mThis->mPipe.exist ()) ;
		assert (!fake.mThis->mMapping.exist ()) ;
		assume (fake.mThis->mHeader != NULL) ;
		fake.mThis->mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (fake.mThis->mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		fake.mThis->mFileSize = fake.mThis->mHeader->mFileSize ;
		fake.mThis->mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate64 (fake.mThis->mPipe ,fake.mThis->mFileSize) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		fake.mThis->mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		read_header () ;
	}

	void read_header () {
		assert (fake.mThis->mHeader == NULL) ;
		fake.mThis->mHeader = Box<BufferFileHeader>::make () ;
		auto rax = ByteReader (borrow_header ()) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mFileEndian ;
		assume (fake.mThis->mHeader->mFileEndian == file_endian ()) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mFileSize ;
		assume (fake.mThis->mHeader->mFileSize == fake.mThis->mFileSize) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mBlockSize ;
		const auto r1x = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
		assume (fake.mThis->mHeader->mBlockSize == r1x) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mBlockStep ;
		assume (fake.mThis->mHeader->mBlockStep == fake.mThis->mBlockStep) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mBlockLength ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mChunkSize ;
		assume (fake.mThis->mHeader->mChunkSize >= 0) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mChunkStep ;
		assume (fake.mThis->mHeader->mChunkStep == fake.mThis->mChunkStep) ;
		rax >> GAP ;
		rax >> fake.mThis->mHeader->mChunkLength ;
		rax >> GAP ;
	}

	void write_header () {
		if ifdo (TRUE) {
			if (fake.mThis->mHeader != NULL)
				discard ;
			fake.mThis->mHeader = Box<BufferFileHeader>::make () ;
			fake.mThis->mHeader->mFileEndian = file_endian () ;
			fake.mThis->mHeader->mFileSize = fake.mThis->mFileSize ;
			fake.mThis->mHeader->mBlockSize = fake.mThis->mChunkStep / fake.mThis->mBlockStep ;
			fake.mThis->mHeader->mBlockStep = fake.mThis->mBlockStep ;
			fake.mThis->mHeader->mBlockLength = 0 ;
			fake.mThis->mHeader->mChunkSize = (fake.mThis->mFileSize - HEADER_SIZE::expr) / fake.mThis->mChunkStep ;
			fake.mThis->mHeader->mChunkStep = fake.mThis->mChunkStep ;
			fake.mThis->mHeader->mChunkLength = 0 ;
		}
		auto rax = ByteWriter (borrow_header ()) ;
		rax << slice ("CSC_BufferFile") ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mFileEndian ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mFileSize ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mBlockSize ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mBlockStep ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mBlockLength ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mChunkSize ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mChunkStep ;
		rax << GAP ;
		rax << fake.mThis->mHeader->mChunkLength ;
		rax << GAP ;
		flush () ;
	}

	Ref<RefBuffer<BYTE>> borrow_header () {
		INDEX ix = mmap_cache (0 ,HEADER_SIZE::expr) ;
		const auto r1x = fake.mThis->mCacheList[ix].mBlock->m1st ;
		const auto r2x = HEADER_SIZE::expr ;
		return Ref<RefBuffer<BYTE>>::make (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	VAL64 file_endian () const {
		const auto r1x = invoke ([&] () {
			auto rax = Buffer<BYTE ,SIZE_OF<CHAR>> () ;
			rax[0] = BYTE (0X00) ;
			rax[1] = BYTE (0X01) ;
			rax[2] = BYTE (0X02) ;
			rax[3] = BYTE (0X03) ;
			return bitwise[TYPE<CHAR>::expr] (rax) ;
		}) ;
		return VAL64 (r1x) ;
	}

	LENGTH file_size () const override {
		assume (fake.mThis->mFileSize < VAL32_MAX) ;
		return LENGTH (fake.mThis->mFileSize) ;
	}

	VAL64 file_size (CREF<HFILE> handle) const {
		const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (fake.mThis->mHeader->mBlockSize))) ;
		assert (item.size () == fake.mThis->mHeader->mBlockStep) ;
		const auto r1x = index / fake.mThis->mHeader->mBlockSize ;
		const auto r2x = index % fake.mThis->mHeader->mBlockSize * fake.mThis->mHeader->mBlockStep ;
		const auto r3x = HEADER_SIZE::expr + r1x * fake.mThis->mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (fake.mThis->mHeader->mChunkStep)) ;
		const auto r4x = fake.mThis->mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::from (item.self) ,Pointer::make (r4x) ,LENGTH (fake.mThis->mHeader->mBlockStep)) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) override {
		assert (fake.mThis->mPipe.exist ()) ;
		assert (inline_between (index ,0 ,LENGTH (fake.mThis->mHeader->mBlockSize))) ;
		assert (item.size () == fake.mThis->mHeader->mBlockStep) ;
		const auto r1x = index / fake.mThis->mHeader->mBlockSize ;
		const auto r2x = index % fake.mThis->mHeader->mBlockSize * fake.mThis->mHeader->mBlockStep ;
		const auto r3x = HEADER_SIZE::expr + r1x * fake.mThis->mHeader->mChunkStep ;
		INDEX ix = mmap_cache (r3x ,LENGTH (fake.mThis->mHeader->mChunkStep)) ;
		const auto r4x = fake.mThis->mCacheList[ix].mBlock->m1st + LENGTH (r2x) ;
		inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.self) ,LENGTH (fake.mThis->mHeader->mBlockStep)) ;
	}

	INDEX mmap_cache (CREF<VAL64> index ,CREF<LENGTH> size_) {
		INDEX ret = fake.mThis->mCacheSet.map (index) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			update_overflow () ;
			ret = fake.mThis->mCacheList.insert () ;
			fake.mThis->mCacheSet.add (index ,ret) ;
			fake.mThis->mCacheList[ret].mIndex = index ;
			fake.mThis->mCacheList[ret].mBlock = UniqueRef<Tuple<FLAG ,FLAG>> ([&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r1x = mmap64 (NULL ,size_ ,fake.mThis->mFileMapFlag ,MAP_SHARED ,fake.mThis->mPipe ,index) ;
				assume (r1x != MAP_FAILED) ;
				me.m1st = FLAG (r1x) ;
				me.m2nd = me.m1st + size_ ;
			} ,[&] (VREF<Tuple<FLAG ,FLAG>> me) {
				const auto r2x = csc_pointer_t (me.m1st) ;
				const auto r3x = me.m2nd - me.m1st ;
				msync (r2x ,r3x ,MS_SYNC) ;
				munmap (r2x ,r3x) ;
			}) ;
		}
		fake.mThis->mCacheList[ret].mCacheTime = fake.mThis->mCacheTimer ;
		fake.mThis->mCacheTimer++ ;
		if ifdo (TRUE) {
			if (fake.mThis->mCacheTimer < VAL32_MAX)
				discard ;
			for (auto &&i : fake.mThis->mCacheList.range ())
				fake.mThis->mCacheList[i].mCacheTime = 0 ;
			fake.mThis->mCacheList[ret].mCacheTime = 1 ;
			fake.mThis->mCacheTimer = 2 ;
		}
		return move (ret) ;
	}

	void update_overflow () {
		if (fake.mThis->mCacheList.length () < fake.mThis->mCacheList.size ())
			return ;
		const auto r1x = invoke ([&] () {
			INDEX ret = NONE ;
			auto rax = VAL64 () ;
			for (auto &&i : fake.mThis->mCacheList.range ()) {
				if (ret != NONE)
					if (rax >= fake.mThis->mCacheList[i].mCacheTime)
						continue ;
				ret = i ;
				rax = fake.mThis->mCacheList[i].mCacheTime ;
			}
			return move (ret) ;
		}) ;
		assert (r1x != NONE) ;
		fake.mThis->mCacheSet.erase (fake.mThis->mCacheList[r1x].mIndex) ;
		fake.mThis->mCacheList.remove (r1x) ;
	}

	void flush () override {
		if (!fake.mThis->mPipe.exist ())
			return ;
		fake.mThis->mCacheSet.clear () ;
		fake.mThis->mCacheList.clear () ;
		fake.mThis->mCacheTimer = 0 ;
		fsync (fake.mThis->mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout>::declare (BufferFileImplHolder ()) ;

struct ConsoleImplLayout {
	Mutex mMutex ;
	BitSet mOption ;
	UniqueRef<HANDLE> mConsole ;
	String<STR> mLogBuffer ;
	TextWriter mLogWriter ;
	String<STR> mLogFile ;
	String<STR> mOldLogFile ;
	StreamFile mLogStreamFile ;
} ;

class ConsoleImplHolder final implement Fat<ConsoleHolder ,ConsoleLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<ConsoleImplLayout>::make () ;
		fake.mThis->mMutex = NULL ;
		fake.mThis->mOption = BitSet (ConsoleOption::ETC) ;
		fake.mThis->mLogBuffer = String<STR> (STREAMFILE_BUF_SIZE::expr) ;
		fake.mThis->mLogWriter = TextWriter (fake.mThis->mLogBuffer.borrow ()) ;
	}

	void set_option (CREF<Just<ConsoleOption>> option) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ConsoleOption::All)
				discard ;
			fake.mThis->mOption.clear () ;
		}
		if ifdo (act) {
			fake.mThis->mOption.add (option) ;
		}
	}

	void log (CREF<String<STR>> tag ,CREF<Format> msg) const {
		fake.mThis->mLogWriter << CLS ;
		fake.mThis->mLogWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		fake.mThis->mLogWriter << AlignedText (r2x.mHour ,2) ;
		fake.mThis->mLogWriter << slice (":") ;
		fake.mThis->mLogWriter << AlignedText (r2x.mMinute ,2) ;
		fake.mThis->mLogWriter << slice (":") ;
		fake.mThis->mLogWriter << AlignedText (r2x.mSecond ,2) ;
		fake.mThis->mLogWriter << slice ("][") ;
		fake.mThis->mLogWriter << tag ;
		fake.mThis->mLogWriter << slice ("] : ") ;
		fake.mThis->mLogWriter << msg ;
		fake.mThis->mLogWriter << GAP ;
		fake.mThis->mLogWriter << EOS ;
	}

	void print (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoPrint])
			return ;
		fake.mThis->mLogWriter << CLS ;
		fake.mThis->mLogWriter << msg ;
		fake.mThis->mLogWriter << EOS ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("%s")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void fatal (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("\033[1;34m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void error (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("\033[1;31m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void warn (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("\033[1;33m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void info (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("\033[1;32m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void debug (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("\033[1;36m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void trace (CREF<Format> msg) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			const auto r1x = String<STRA> (slice ("\033[1;37m%s\033[0m")) ;
			std::printf (r1x ,fake.mThis->mLogBuffer.self) ;
		}
	}

	void open (CREF<String<STR>> dire) const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mLogFile = Path (dire).child (slice ("console.log")) ;
		fake.mThis->mOldLogFile = Path (dire).child (slice ("console.old.log")) ;
		FileProc::erase_file (fake.mThis->mOldLogFile) ;
		FileProc::move_file (fake.mThis->mOldLogFile ,fake.mThis->mLogFile) ;
		fake.mThis->mLogStreamFile = StreamFile (fake.mThis->mLogFile) ;
		fake.mThis->mLogStreamFile.open_w (0) ;
		fake.mThis->mLogWriter << CLS ;
		fake.mThis->mLogWriter << BOM ;
		fake.mThis->mLogWriter << EOS ;
		log_file () ;
	}

	void log_file () const {
		if (fake.mThis->mLogFile.length () == 0)
			return ;
		const auto r1x = FLAG (fake.mThis->mLogBuffer.self) ;
		const auto r2x = (fake.mThis->mLogWriter.length () - 1) * SIZE_OF<STR>::expr ;
		fake.mThis->mLogStreamFile.write (RefBuffer<BYTE>::reference (r1x ,r2x)) ;
	}

	void start () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		if (fake.mThis->mConsole.exist ())
			return ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make (stderr) ;
	}

	void stop () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make () ;
	}

	void pause () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = String<STRA> (slice ("press any key to continue...")) ;
		std::printf ("%s\n" ,r1x.self) ;
		const auto r2x = std::getchar () ;
		noop (r2x) ;
	}

	void clear () const override {
		Scope<Mutex> anonymous (fake.mThis->mMutex) ;
		const auto r1x = String<STRA> (slice ("clear")) ;
		const auto r2x = std::system (r1x) ;
		noop (r2x) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout>::declare (ConsoleImplHolder ()) ;
} ;