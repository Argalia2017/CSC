#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

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
#include <sys/types.h>
#include <sys/sendfile.h>
#include "csc_begin.h"

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <exception>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <random>
#include "csc_begin.h"

namespace CSC {
namespace std {
using namespace ::std ;

using ::open ;
using ::close ;
using ::read ;
using ::write ;
} ;

using HMODULE = csc_pointer_t ;
using HANDLE = csc_pointer_t ;
using HFILE = int ;
} ;

namespace CSC {
class RuntimeProcImplHolder implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
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
		auto &&tmp = keep[TYPE<TimeLayout>::expr] (time) ;
		std::this_thread::sleep_for (tmp.mThis->mTime) ;
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

	FLAG random_seed () const override {
		return invoke (std::random_device ()) ;
	}

	String<STR> working_path () const override {
		String<STR> ret = String<STR>::make () ;
		if ifdo (TRUE) {
			const auto r1x = getcwd (ret ,csc_size_t (ret.size ())) ;
			if (r1x != NULL)
				discard ;
			ret.clear () ;
		}
		ret = Path (ret).child ().path () ;
		return move (ret) ;
	}

	String<STR> module_path () const override {
		String<STR> ret = String<STR>::make () ;
		if ifdo (TRUE) {
			const auto r1x = readlink ("/proc/self/exe" ,ret ,csc_size_t (ret.size ())) ;
			if (r1x >= 0)
				discard ;
			ret.clear () ;
		}
		ret = Path (ret).path () ;
		return move (ret) ;
	}

	String<STR> module_name () const override {
		String<STR> ret = String<STR>::make () ;
		if ifdo (TRUE) {
			const auto r1x = readlink ("/proc/self/exe" ,ret ,csc_size_t (ret.size ())) ;
			if (r1x >= 0)
				discard ;
			ret.clear () ;
		}
		ret = Path (ret).name () ;
		return move (ret) ;
	}
} ;

static const auto anonymous = External<RuntimeProcHolder ,RuntimeProcLayout>::declare ([] () {
	return inline_hold (RuntimeProcImplHolder ()) ;
}) ;

struct ProcessPureLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

class ProcessImplHolder implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_SIZE = ENUM<128> ;

public:
	void initialize (CREF<FLAG> uid) override {
		fake.mThis = Ref<ProcessPureLayout>::make () ;
		fake.mThis->mUid = uid ;
		const auto r1x = load_proc_file (uid) ;
		fake.mThis->mProcessCode = process_code (r1x ,uid) ;
		fake.mThis->mProcessTime = process_time (r1x ,uid) ;
	}

	String<STRU8> load_proc_file (CREF<FLAG> uid) const {
		String<STRU8> ret = String<STRU8>::make () ;
		const auto r1x = String<STR>::make (slice ("/proc/") ,uid ,slice ("/stat")) ;
		auto rax = StreamFile (r1x) ;
		rax.open_r () ;
		auto rbx = ret.borrow () ;
		auto &&tmp = keep[TYPE<RefBuffer<BYTE>>::expr] (keep[TYPE<RefBufferLayout>::expr] (rbx.self)) ;
		rax.read (tmp) ;
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
		fake.mThis = Ref<ProcessPureLayout>::make () ;
		fake.mThis->mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_SIZE::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (Pointer::make (address (snapshot_)))) ;
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
		if (!(r1x))
			return r1x ;
		const auto r2x = inline_equal (fake.mThis->mProcessCode ,that.mThis->mProcessCode) ;
		if (!(r2x))
			return r2x ;
		const auto r3x = inline_equal (fake.mThis->mProcessTime ,that.mThis->mProcessTime) ;
		if (!(r3x))
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

static const auto anonymous = External<ProcessHolder ,ProcessLayout>::declare ([] () {
	return inline_hold (ProcessImplHolder ()) ;
}) ;

struct ModulePureLayout {
	UniqueRef<HMODULE> mModule ;
	FLAG mLastError ;
} ;

class ModuleImplHolder implement Fat<ModuleHolder ,ModuleLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		const auto r1x = Path (file).name () ;
		assert (r1x.length () > 0) ;
		const auto r2x = StringProc::stra_from_strs (r1x) ;
		fake.mThis->mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
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
		FLAG ret = FLAG (dlsym (fake.mThis->mModule ,r1x)) ;
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
		const auto r2x = Slice (r1x ,DEFAULT_SLICE_SIZE::expr ,1).eos () ;
		ret = String<STR>::make (slice ("LastError = ") ,r2x ,slice (" : ") ,ret) ;
		return move (ret) ;
	}
} ;

static const auto anonymous = External<ModuleHolder ,ModuleLayout>::declare ([] () {
	return inline_hold (ModuleImplHolder ()) ;
}) ;

struct SingletonHeap {
	Mutex mMutex ;
	Set<Clazz> mClazzSet ;
} ;

struct SingletonHeapPureLayout {
	RefPureLayout mValue ;
	SingletonHeap mStorage ;
} ;

struct SingletonPipe {
	QUAD mReserve1 ;
	QUAD mAddress1 ;
	QUAD mReserve2 ;
	QUAD mAddress2 ;
	QUAD mReserve3 ;
} ;

struct SingletonProcPureLayout {
	FLAG mUid ;
	String<STR> mName ;
	UniqueRef<Tuple<HFILE ,String<STR>>> mPipe ;
	Ref<SingletonHeap> mHeap ;
} ;

class SingletonProcImplHolder implement Fat<SingletonProcHolder ,SingletonProcLayout> {
public:
	void initialize () override {
		fake.mThis = SharedRef<SingletonProcPureLayout>::make () ;
		fake.mThis->mUid = RuntimeProc::process_uid () ;
		fake.mThis->mName = String<STR>::make (slice ("CSC_Singleton_") ,fake.mThis->mUid) ;
		link_pipe () ;
	}

	void link_pipe () {
		auto rax = SingletonPipe () ;
		auto act = TRUE ;
		if ifdo (act) {
			try {
				init_pipe () ;
				rax = load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (act) {
			try {
				save_pipe () ;
				rax = load_pipe () ;
			} catch (CREF<Exception> e) {
				noop (e) ;
				discard ;
			}
		}
		if ifdo (TRUE) {
			const auto r1x = FLAG (rax.mAddress1) ;
			assume (r1x != ZERO) ;
			if (address (fake.mThis->mHeap.self) == r1x)
				return ;
			fake.mThis->mHeap = Ref<SingletonHeap>::reference (Pointer::make (r1x)) ;
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
		fake.mThis->mHeap = Ref<SingletonHeap>::make () ;
		fake.mThis->mHeap->mMutex = RecursiveMutex () ;
		fake.mThis->mHeap->mClazzSet = Set<Clazz> () ;
	}

	SingletonPipe load_pipe () const {
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
		SingletonPipe ret ;
		inline_memcpy (Pointer::from (ret) ,Pointer::make (r3x) ,SIZE_OF<SingletonPipe>::expr) ;
		assume (ret.mReserve1 == QUAD (0X1122334455667788)) ;
		assume (ret.mReserve3 == QUAD (0XAAAABBBBCCCCDDDD)) ;
		assume (ret.mReserve2 == QUAD (fake.mThis->mUid)) ;
		assume (ret.mAddress1 == ret.mAddress2) ;
		return move (ret) ;
	}

	void save_pipe () const {
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
		rax.mReserve1 = QUAD (0X1122334455667788) ;
		rax.mAddress1 = QUAD (address (fake.mThis->mHeap.self)) ;
		rax.mReserve2 = QUAD (fake.mThis->mUid) ;
		rax.mAddress2 = QUAD (address (fake.mThis->mHeap.self)) ;
		rax.mReserve3 = QUAD (0XAAAABBBBCCCCDDDD) ;
		inline_memcpy (Pointer::make (r3x) ,Pointer::from (rax) ,SIZE_OF<SingletonPipe>::expr) ;
	}

	FLAG load (CREF<Clazz> clazz) const override {
		FLAG ret = fake.mThis->mHeap->mClazzSet.map (clazz) ;
		replace (ret ,NONE ,ZERO) ;
		return move (ret) ;
	}

	void save (CREF<Clazz> clazz ,CREF<FLAG> addr) const override {
		assert (addr != ZERO) ;
		assert (addr != NONE) ;
		fake.mThis->mHeap->mClazzSet.add (clazz ,addr) ;
	}
} ;

static const auto anonymous = External<SingletonProcHolder ,SingletonProcLayout>::declare ([] () {
	return inline_hold (SingletonProcImplHolder ()) ;
}) ;

struct PathPureLayout {
	String<STR> mPathName ;
	LENGTH mLength ;
} ;

class PathImplHolder implement Fat<PathHolder ,PathLayout> {
private:
	using HDIR = DEF<DIR *> ;
	using HDIRENT = DEF<dirent * > ;
	using STAT_INFO = DEF<struct stat> ;

public:
	void initialize (RREF<String<STR>> pathname_) override {
		assert (pathname_.step () == SIZE_OF<STR>::expr) ;
		fake.mThis = Ref<PathPureLayout>::make () ;
		fake.mThis->mPathName = move (pathname_) ;
		fake.mThis->mLength = fake.mThis->mPathName.length () ;
		if ifdo (TRUE) {
			if (fake.mThis->mLength > 0)
				discard ;
			fake.mThis->mPathName = slice (".") ;
		}
		if ifdo (TRUE) {
			INDEX ix = fake.mThis->mLength - 1 ;
			if (!(is_separator (fake.mThis->mPathName[ix])))
				discard ;
			fake.mThis->mPathName.trunc (ix) ;
			fake.mThis->mLength = ix ;
		}
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

	PathLayout root () const override {
		PathLayout ret ;
		if ifdo (TRUE) {
			if (!(StreamProc::is_alpha (fake.mThis->mPathName[0])))
				discard ;
			if (fake.mThis->mPathName[1] != STRU32 (':'))
				discard ;
			ret = Path (Slice (FLAG (fake.mThis->mPathName.self) ,2 ,SIZE_OF<STR>::expr)) ;
		}
		return move (ret) ;
	}

	PathLayout parent () const override {
		PathLayout ret ;
		unimplemented () ;
		return move (ret) ;
	}

	PathLayout brother () const override {
		PathLayout ret ;
		unimplemented () ;
		return move (ret) ;
	}

	PathLayout child () const override {
		PathLayout ret ;
		unimplemented () ;
		return move (ret) ;
	}

	PathLayout child (CREF<Slice> name) const override {
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
		for (auto &&i : iter (0 ,rbx.length ()))
			ret[i] = Path (move (rbx[i])) ;
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
			ret[i] = Path (move (rbx[i])) ;
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
		if (!(S_ISDIR (rax.st_mode)))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_link () const override {
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = stat (fake.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!(S_ISLNK (rax.st_mode)))
			return FALSE ;
		return TRUE ;
	}

	Deque<String<STR>> decouple () const override {
		Deque<String<STR>> ret ;
		INDEX ix = 0 ;
		for (auto &&i : fake.mThis->mPathName.range ()) {
			if (!(is_separator (fake.mThis->mPathName[i])))
				continue ;
			const auto r1x = i - ix ;
			if (r1x <= 0)
				continue ;
			const auto r2x = address (fake.mThis->mPathName.at (ix)) ;
			const auto r3x = Slice (r2x ,r1x ,SIZE_OF<STR>::expr) ;
			ret.add (r3x) ;
		}
		return move (ret) ;
	}

	String<STR> absolute () const override {
		auto rax = decouple () ;
		auto rbx = Path (RuntimeProc::working_path ()).decouple () ;
		for (auto &&i : iter (0 ,rax.size ())) {
			auto act = TRUE ;
			if ifdo (act) {
				if (rax[i] != slice ("."))
					discard ;
				noop () ;
			}
			if ifdo (act) {
				if (rax[i] != slice (".."))
					discard ;
				assume (rbx.length () > 0) ;
				rbx.pop () ;
			}
			if ifdo (act) {
				rbx.add (move (rax[i])) ;
			}
		}
		const auto r1x = invoke ([&] () {
			LENGTH ret = 0 ;
			for (auto &&i : iter (0 ,rbx.size ()))
				ret += rbx[i].length () ;
			return move (ret) ;
		}) ;
		String<STR> ret = String<STR> (r1x) ;
		for (auto &&i : iter (0 ,rbx.size ()))
			ret += rbx[i] ;
		return move (ret) ;
	}

	String<STR> path () const override {
		const auto r1x = find_last_quat () + 1 ;
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
		const auto r1x = find_last_quat () + 1 ;
		const auto r2x = fake.mThis->mLength - r1x ;
		String<STR> ret = String<STR> (r2x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,fake.mThis->mLength)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	INDEX find_last_quat () const {
		INDEX ix = fake.mThis->mLength ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (is_separator (fake.mThis->mPathName[ix]))
				return ix ;
			ix-- ;
		}
		return 0 ;
	}

	String<STR> stem () const override {
		const auto r1x = find_last_quat () + 1 ;
		const auto r2x = find_last_dot_word () ;
		const auto r3x = inline_max (r2x - r1x ,0) ;
		String<STR> ret = String<STR> (r3x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,r3x)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	String<STR> extension () const override {
		const auto r1x = find_last_dot_word () ;
		const auto r2x = fake.mThis->mLength - r1x ;
		String<STR> ret = String<STR> (r2x) ;
		INDEX ix = 0 ;
		for (auto &&i : iter (r1x ,fake.mThis->mLength)) {
			ret[ix] = fake.mThis->mPathName[i] ;
			ix++ ;
		}
		ret.trunc (ix) ;
		return move (ret) ;
	}

	INDEX find_last_dot_word () const {
		INDEX ix = fake.mThis->mLength - 1 ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (!(StreamProc::is_word (fake.mThis->mPathName[ix])))
				break ;
			if (fake.mThis->mPathName[ix] == STRU32 ('.'))
				return ix ;
			ix-- ;
		}
		return fake.mThis->mLength ;
	}
} ;

static const auto anonymous = External<PathHolder ,PathLayout>::declare ([] () {
	return inline_hold (PathImplHolder ()) ;
}) ;

class FileProcImplHolder implement Fat<FileProcHolder ,FileProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	RefBuffer<BYTE> load_file (CREF<String<STR>> file) const override {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = std::open (file ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = file_size (r1x) ;
		assume (r2x <= VAL32_MAX) ;
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

	void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> data) const override {
		assert (data.size () <= VAL32_MAX) ;
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (file ,r2x ,r3x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r4x = data.size () ;
		auto rax = r4x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (r1x ,(&data[r4x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
	}

	RefBuffer<BYTE> load_asset (CREF<String<STR>> file) const override {
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
		return RefBuffer<BYTE>::reference (r4x ,r6x) ;
	}

	void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			me = std::open (src ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r2x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (dst ,r2x ,r3x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r3x = file_size (r1x) ;
		assume (r3x <= VAL32_MAX) ;
		sendfile (r1x ,r2x ,NULL ,r3x) ;
	}

	void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		rename (src ,dst) ;
	}

	void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = link (src ,dst) ;
		assume (r1x) ;
	}

	void erase_file (CREF<String<STR>> file) const override {
		unlink (file) ;
	}

	void build_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
		auto rax = String<STR>::make () ;
		for (auto &&i : r1x) {
			rax += i ;
			mkdir (rax ,r2x) ;
			rax += slice ("/") ;
		}
	}

	void clear_dire (CREF<String<STR>> dire) const override {
		auto rax = Deque<Tuple<Path ,BOOL>> () ;
		rax.add ({Path (dire) ,TRUE}) ;
		while (TRUE) {
			if (rax.empty ())
				break ;
			INDEX ix = rax.tail () ;
			if (rax[ix].m2nd) {
				rax[ix].m2nd = FALSE ;
				const auto r1x = rax[ix].m1st.list () ;
				for (auto &&i : r1x) {
					auto act = TRUE ;
					if ifdo (act) {
						if (!(i.is_link ()))
							discard ;
						erase_file (i.absolute ()) ;
					}
					if ifdo (act) {
						if (!(i.is_file ()))
							discard ;
						erase_file (i.absolute ()) ;
					}
					if ifdo (act) {
						if (!(i.is_dire ()))
							discard ;
						rax.add ({i ,TRUE}) ;
					}
				}
			} else {
				erase_dire (rax[ix].m1st.absolute ()) ;
				rax.take () ;
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
			const auto r3x = FileProc::load_file (r1x.absolute ()) ;
			const auto r4x = Process (r3x) ;
			const auto r5x = Process (r4x.process_uid ()) ;
			if (r4x != r5x)
				discard ;
			return r4x == r2x ;
		}
		return lock_handle (r1x ,r2x.snapshot ()).exist () ;
	}

	CREF<UniqueRef<String<STR>>> lock_handle (CREF<Path> file ,CREF<RefBuffer<BYTE>> snapshot_) const {
		return memorize ([&] () {
			return UniqueRef<String<STR>> ([&] (VREF<String<STR>> me) {
				me = file.absolute () ;
				FileProc::save_file (me ,snapshot_) ;
			} ,[&] (VREF<String<STR>> me) {
				FileProc::erase_dire (me) ;
			}) ;
		}) ;
	}
} ;

static const auto anonymous = External<FileProcHolder ,FileProcLayout>::declare ([] () {
	return inline_hold (FileProcImplHolder ()) ;
}) ;

class StreamFileImplement {
protected:
	String<STR> mFile ;
	UniqueRef<HFILE> mReadPipe ;
	UniqueRef<HFILE> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;

public:
	void initialize (CREF<String<STR>> file) {
		mFile = move (file) ;
		mFileSize = 0 ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void open_r () {
		assert (!(mReadPipe.exist ())) ;
		assert (!(mWritePipe.exist ())) ;
		mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		mFileSize = file_size (mReadPipe) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> count) {
		assert (!(mReadPipe.exist ())) ;
		assert (!(mWritePipe.exist ())) ;
		mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		mFileSize = count ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void open_a () {
		assert (!(mReadPipe.exist ())) ;
		assert (!(mWritePipe.exist ())) ;
		mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		mFileSize = file_size (mReadPipe) ;
		mRead = 0 ;
		mWrite = 0 ;
		if ifdo (TRUE) {
			const auto r5x = VAL64 (lseek64 (mWritePipe ,0 ,SEEK_END)) ;
			if (r5x <= 0)
				discard ;
			mWrite += r5x ;
		}
	}

	LENGTH file_size () const {
		return LENGTH (mFileSize) ;
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

	void read (VREF<RefBuffer<BYTE>> item) {
		assert (mReadPipe.exist ()) ;
		assert (item.size () <= VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (mReadPipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		mRead += r1x ;
	}

	void write (CREF<RefBuffer<BYTE>> item) {
		assert (mWritePipe.exist ()) ;
		assert (item.size () <= VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (mWritePipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= LENGTH (rbx) ;
		}
		mWrite += r1x ;
	}

	void flush () {
		if (!(mWritePipe.exist ()))
			return ;
		fsync (mWritePipe) ;
	}
} ;

class StreamFileImplHolder implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = Ref<StreamFileImplement>::make () ;
		fake.mThis->initialize (file) ;
	}

	void open_r () override {
		return fake.mThis->open_r () ;
	}

	void open_w (CREF<LENGTH> count) override {
		return fake.mThis->open_w (count) ;
	}

	void open_a () override {
		return fake.mThis->open_a () ;
	}

	LENGTH file_size () const override {
		return fake.mThis->file_size () ;
	}

	void read (VREF<RefBuffer<BYTE>> item) override {
		return fake.mThis->read (item) ;
	}

	void write (CREF<RefBuffer<BYTE>> item) override {
		return fake.mThis->write (item) ;
	}

	void flush () override {
		return fake.mThis->flush () ;
	}
} ;

static const auto anonymous = External<StreamFileHolder ,StreamFileLayout>::declare ([] () {
	return inline_hold (StreamFileImplHolder ()) ;
}) ;

struct BufferFileHeader {
	VAL64 mFileEndian ;
	VAL64 mFileSize ;
	VAL64 mBlockStepSize ;
	VAL64 mBlockStepAlign ;
	VAL64 mBlockCount ;
	VAL64 mChunkStepSize ;
	VAL64 mChunkCapacity ;
	VAL64 mChunkCount ;
} ;

struct BufferFileChunk {
	VAL64 mOffset ;
	VAL64 mCacheTime ;
	UniqueRef<RefBuffer<BYTE>> mBuffer ;
} ;

class BufferFileImplement {
private:
	using CHUNK_STEP_SIZE = ENUM<4194304> ;
	using HEADER_SIZE = ENUM<65536> ;

protected:
	String<STR> mFile ;
	UniqueRef<HFILE> mPipe ;
	UniqueRef<LENGTH> mMapping ;
	VAL64 mFileSize ;
	VAL64 mBlockStepSize ;
	VAL64 mBlockStepAlign ;
	csc_enum_t mFileMapFlag ;
	Box<BufferFileHeader> mHeader ;
	Set<VAL64> mCacheSet ;
	List<BufferFileChunk> mCacheList ;
	VAL64 mCacheTimer ;

public:
	void initialize (CREF<String<STR>> file) {
		mFile = move (file) ;
		mFileSize = 0 ;
		mFileMapFlag = 0 ;
		set_block_step (SIZE_OF<Auto>::expr) ;
		set_cache_size (1) ;
	}

	void set_block_step (CREF<LENGTH> size_) {
		mBlockStepSize = size_ ;
		mBlockStepAlign = 8 ;
	}

	void set_cache_size (CREF<LENGTH> size_) {
		mCacheSet = Set<VAL64> (size_) ;
		mCacheList = List<BufferFileChunk> (size_) ;
		mCacheTimer = 0 ;
	}

	void open_r () {
		assert (!(mPipe.exist ())) ;
		assert (!(mMapping.exist ())) ;
		mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		mFileSize = file_size (mPipe) ;
		mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = 0 ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		mFileMapFlag = csc_enum_t (PROT_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> count) {
		assert (!(mPipe.exist ())) ;
		assert (!(mMapping.exist ())) ;
		mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		const auto r3x = count * mBlockStepSize ;
		const auto r4x = CHUNK_STEP_SIZE::expr / mBlockStepSize ;
		const auto r5x = inline_alignas (r3x ,r4x) / r4x ;
		mFileSize = HEADER_SIZE::expr + r5x * CHUNK_STEP_SIZE::expr ;
		mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate64 (mPipe ,mFileSize) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	void open_a () {
		assert (!(mPipe.exist ())) ;
		assert (!(mMapping.exist ())) ;
		assume (mHeader != NULL) ;
		mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VREF<HFILE> me) {
			std::close (me) ;
		}) ;
		mFileSize = mHeader->mFileSize ;
		mMapping = UniqueRef<LENGTH> ([&] (VREF<LENGTH> me) {
			me = ftruncate64 (mPipe ,mFileSize) ;
			assume (me == 0) ;
		} ,[&] (VREF<LENGTH> me) {
			noop () ;
		}) ;
		mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	LENGTH file_size () const {
		return LENGTH (mFileSize) ;
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

	void read_header () {
		assert (mHeader == NULL) ;
		mHeader = Box<BufferFileHeader>::make () ;
		INDEX ix = load (0 ,HEADER_SIZE::expr) ;
		auto rax = ByteReader (borrow (ix)) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> mHeader->mFileEndian ;
		assume (mHeader->mFileEndian == file_endian ()) ;
		rax >> GAP ;
		rax >> mHeader->mFileSize ;
		assume (mHeader->mFileSize == mFileSize) ;
		rax >> GAP ;
		rax >> mHeader->mBlockStepSize ;
		assume (mHeader->mBlockStepSize == mBlockStepSize) ;
		rax >> GAP ;
		rax >> mHeader->mBlockStepAlign ;
		assume (mHeader->mBlockStepAlign == mBlockStepAlign) ;
		rax >> GAP ;
		rax >> mHeader->mBlockCount ;
		assume (mHeader->mBlockCount >= 0) ;
		rax >> GAP ;
		rax >> mHeader->mChunkStepSize ;
		assume (mHeader->mChunkStepSize == CHUNK_STEP_SIZE::expr) ;
		rax >> GAP ;
		rax >> mHeader->mChunkCapacity ;
		const auto r1x = mHeader->mChunkStepSize / mHeader->mBlockStepSize ;
		assume (mHeader->mChunkCapacity == r1x) ;
		rax >> GAP ;
		rax >> mHeader->mChunkCount ;
		assume (mHeader->mChunkCount >= 0) ;
		rax >> GAP ;
	}

	void write_header () {
		if ifdo (TRUE) {
			if (mHeader != NULL)
				discard ;
			mHeader = Box<BufferFileHeader>::make () ;
			mHeader->mFileEndian = file_endian () ;
			mHeader->mFileSize = mFileSize ;
			mHeader->mBlockStepSize = mBlockStepSize ;
			mHeader->mBlockStepAlign = mBlockStepAlign ;
			mHeader->mBlockCount = 0 ;
			mHeader->mChunkStepSize = CHUNK_STEP_SIZE::expr ;
			assume (mHeader->mChunkStepSize >= mHeader->mBlockStepSize) ;
			mHeader->mChunkCapacity = mHeader->mChunkStepSize / mHeader->mBlockStepSize ;
			mHeader->mChunkCount = 0 ;
		}
		INDEX ix = load (0 ,HEADER_SIZE::expr) ;
		auto rax = ByteWriter (borrow (ix)) ;
		rax << slice ("CSC_BufferFile") ;
		rax << GAP ;
		rax << mHeader->mFileEndian ;
		rax << GAP ;
		rax << mHeader->mFileSize ;
		rax << GAP ;
		rax << mHeader->mBlockStepSize ;
		rax << GAP ;
		rax << mHeader->mBlockStepAlign ;
		rax << GAP ;
		rax << mHeader->mBlockCount ;
		rax << GAP ;
		rax << mHeader->mChunkStepSize ;
		rax << GAP ;
		rax << mHeader->mChunkCapacity ;
		rax << GAP ;
		rax << mHeader->mChunkCount ;
		rax << GAP ;
		flush () ;
	}

	Ref<RefBuffer<BYTE>> borrow (CREF<INDEX> index) const {
		Ref<RefBuffer<BYTE>> ret = Ref<RefBuffer<BYTE>>::make () ;
		ret.self = RefBuffer<BYTE>::reference (FLAG (mCacheList[index].mBuffer->self) ,HEADER_SIZE::expr) ;
		return move (ret) ;
	}

	VAL64 file_endian () const {
		const auto r1x = invoke ([&] () {
			auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> () ;
			rax[0] = BYTE (0X00) ;
			rax[1] = BYTE (0X01) ;
			rax[2] = BYTE (0X02) ;
			rax[3] = BYTE (0X03) ;
			return bitwise[TYPE<CHAR>::expr] (rax) ;
		}) ;
		return VAL64 (r1x) ;
	}

	void close () {
		flush () ;
		mMapping = UniqueRef<LENGTH> () ;
		mPipe = UniqueRef<HFILE> () ;
	}

	void get (CREF<VAL64> index ,VREF<RefBuffer<BYTE>> item) {
		assert (mPipe.exist ()) ;
		assert (inline_between (index ,0 ,mHeader->mBlockCount)) ;
		assert (item.size () == mHeader->mBlockStepSize) ;
		const auto r1x = index / mHeader->mChunkCapacity ;
		const auto r2x = index % mHeader->mChunkCapacity * mHeader->mBlockStepSize ;
		const auto r3x = HEADER_SIZE::expr + r1x * mHeader->mChunkStepSize ;
		INDEX ix = load (r3x ,mHeader->mChunkStepSize) ;
		const auto r4x = FLAG (mCacheList[ix].mBuffer->self) + r2x ;
		inline_memcpy (Pointer::from (item.self) ,Pointer::make (r4x) ,mHeader->mBlockStepSize) ;
	}

	void set (CREF<VAL64> index ,CREF<RefBuffer<BYTE>> item) {
		assert (mPipe.exist ()) ;
		assert (inline_between (index ,0 ,mHeader->mBlockCount)) ;
		assert (item.size () == mHeader->mBlockStepSize) ;
		const auto r1x = index / mHeader->mChunkCapacity ;
		const auto r2x = index % mHeader->mChunkCapacity * mHeader->mBlockStepSize ;
		const auto r3x = HEADER_SIZE::expr + r1x * mHeader->mChunkStepSize ;
		INDEX ix = load (r3x ,mHeader->mChunkStepSize) ;
		const auto r4x = FLAG (mCacheList[ix].mBuffer->self) + r2x ;
		inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.self) ,mHeader->mBlockStepSize) ;
	}

	INDEX load (CREF<VAL64> offset ,CREF<LENGTH> size_) {
		INDEX ret = mCacheSet.map (offset) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			update_overflow () ;
			ret = mCacheList.insert () ;
			mCacheSet.add (offset ,ret) ;
			mCacheList[ret].mOffset = offset ;
			mCacheList[ret].mBuffer = UniqueRef<RefBuffer<BYTE>> ([&] (VREF<RefBuffer<BYTE>> me) {
				const auto r3x = mmap64 (NULL ,size_ ,mFileMapFlag ,MAP_SHARED ,mPipe ,offset) ;
				assume (r3x != MAP_FAILED) ;
				me = RefBuffer<BYTE>::reference (FLAG (r3x) ,size_) ;
			} ,[&] (VREF<RefBuffer<BYTE>> me) {
				msync (me.self ,me.size () ,MS_SYNC) ;
				munmap (me.self ,me.size ()) ;
			}) ;
		}
		mCacheList[ret].mCacheTime = mCacheTimer ;
		mCacheTimer++ ;
		return move (ret) ;
	}

	void update_overflow () {
		if (mCacheList.length () < mCacheList.size ())
			return ;
		const auto r1x = invoke ([&] () {
			INDEX ret = NONE ;
			auto rax = VAL64 () ;
			for (auto &&i : mCacheList.range ()) {
				if (ret != NONE)
					if (rax >= mCacheList[i].mCacheTime)
						continue ;
				ret = i ;
				rax = mCacheList[i].mCacheTime ;
			}
			return move (ret) ;
		}) ;
		assert (r1x != NONE) ;
		mCacheSet.erase (mCacheList[r1x].mOffset) ;
		mCacheList.remove (r1x) ;
	}

	void flush () {
		if (!(mPipe.exist ()))
			return ;
		mCacheSet.clear () ;
		mCacheList.clear () ;
		mCacheTimer = 0 ;
		fsync (mPipe) ;
	}
} ;

class BufferFileImplHolder implement Fat<BufferFileHolder ,BufferFileLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = Ref<BufferFileImplement>::make () ;
		fake.mThis->initialize (file) ;
	}

	void set_block_step (CREF<LENGTH> size_) override {
		return fake.mThis->set_block_step (size_) ;
	}

	void set_cache_size (CREF<LENGTH> size_) override {
		return fake.mThis->set_cache_size (size_) ;
	}

	void open_r () override {
		return fake.mThis->open_r () ;
	}

	void open_w (CREF<LENGTH> count) override {
		return fake.mThis->open_w (count) ;
	}

	void open_a () override {
		return fake.mThis->open_a () ;
	}

	LENGTH file_size () const override {
		return fake.mThis->file_size () ;
	}

	void read (CREF<INDEX> index ,VREF<RefBuffer<BYTE>> item) override {
		return fake.mThis->get (index ,item) ;
	}

	void write (CREF<INDEX> index ,CREF<RefBuffer<BYTE>> item) override {
		return fake.mThis->set (index ,item) ;
	}

	void flush () override {
		return fake.mThis->flush () ;
	}
} ;

static const auto anonymous = External<BufferFileHolder ,BufferFileLayout>::declare ([] () {
	return inline_hold (BufferFileImplHolder ()) ;
}) ;

struct ConsolePureLayout {
	BitSet mOption ;
	UniqueRef<HANDLE> mConsole ;
	String<STR> mWriterBuffer ;
	TextWriter mWriter ;
	String<STR> mLogFile ;
	String<STR> mOldLogFile ;
} ;

class ConsoleImplHolder implement Fat<ConsoleHolder ,ConsoleLayout> {
private:
	using CONSOLE_BUFFER_SIZE = ENUM<4194304> ;

public:
	void initialize () override {
		fake.mMutex = RecursiveMutex () ;
		fake.mThis = SharedRef<ConsolePureLayout>::make () ;
		fake.mThis->mOption = BitSet (ConsoleOption::ETC) ;
		fake.mThis->mWriterBuffer = String<STR> (CONSOLE_BUFFER_SIZE::expr) ;
		fake.mThis->mWriter = TextWriter (fake.mThis->mWriterBuffer.borrow ()) ;
	}

	void set_option (CREF<FLAG> option) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
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
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		fake.mThis->mWriter << CLS ;
		fake.mThis->mWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		fake.mThis->mWriter << AlignedText (r2x.mHour ,2) ;
		fake.mThis->mWriter << slice (":") ;
		fake.mThis->mWriter << AlignedText (r2x.mMinute ,2) ;
		fake.mThis->mWriter << slice (":") ;
		fake.mThis->mWriter << AlignedText (r2x.mSecond ,2) ;
		fake.mThis->mWriter << slice ("][") ;
		fake.mThis->mWriter << tag ;
		fake.mThis->mWriter << slice ("] : ") ;
		fake.mThis->mWriter << msg ;
		fake.mThis->mWriter << GAP ;
		fake.mThis->mWriter << EOS ;
	}

	void print (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoPrint])
			return ;
		log (slice ("Print") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "%s\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void fatal (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "\033[1;34m%s\033[0m\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void error (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "\033[1;31m%s\033[0m\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void warn (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "\033[1;33m%s\033[0m\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void info (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "\033[1;32m%s\033[0m\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void debug (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "\033[1;36m%s\033[0m\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void verbose (CREF<Format> msg) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mOption[ConsoleOption::NoVarbose])
			return ;
		log (slice ("Varbose") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = "\033[1;37m%s\033[0m\n" ;
			auto rax = PTR<CREF<char>> (&fake.mThis->mWriterBuffer[0]) ;
			printf (r1x ,rax) ;
		}
	}

	void open (CREF<String<STR>> dire) const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		fake.mThis->mLogFile = Path (dire).child (slice ("console.log")).absolute () ;
		fake.mThis->mOldLogFile = Path (dire).child (slice ("console.old.log")).absolute () ;
	}

	void start () const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		if (fake.mThis->mConsole.exist ())
			return ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make (stderr) ;
	}

	void stop () const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		fake.mThis->mConsole = UniqueRef<HANDLE>::make () ;
	}

	void pause () const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		printf ("press any key to continue...\n") ;
		const auto r1x = getchar () ;
		noop (r1x) ;
	}

	void clear () const override {
		Scope<CREF<Mutex>> anonymous (fake.mMutex) ;
		const auto r1x = system ("clear") ;
		noop (r1x) ;
	}
} ;

static const auto anonymous = External<ConsoleHolder ,ConsoleLayout>::declare ([] () {
	return inline_hold (ConsoleImplHolder ()) ;
}) ;
} ;