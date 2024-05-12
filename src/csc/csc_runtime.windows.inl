#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_runtime.hpp"

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require windows.h"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

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
struct RuntimeProcPureLayout {} ;

class RuntimeProcImplement implement Fat<RuntimeProcHolder ,RuntimeProcLayout> {
public:
	void initialize () override {
		fake.mThis = Ref<RuntimeProcPureLayout>::make () ;
	}

	LENGTH thread_concurrency () const override {
		return std::thread::hardware_concurrency () ;
	}

	FLAG thread_uid () const override {
		const auto r1x = std::this_thread::get_id () ;
		return FLAG (bitwise[TYPE<CHAR>::expr] (r1x)) ;
	}

	void thread_sleep (CREF<Time> time) const override {
		auto &&tmp = keep[TYPE<TimeLayout>::expr] (time) ;
		std::this_thread::sleep_for (tmp.mThis->mTime) ;
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

	FLAG random_seed () const override {
		return invoke (std::random_device ()) ;
	}

	String<STR> working_path () const override {
		String<STR> ret = String<STR>::make () ;
		GetCurrentDirectory (csc_enum_t (ret.size ()) ,ret.raw ()) ;
		ret = Path (ret).child ().path () ;
		return move (ret) ;
	}

	String<STR> module_path () const override {
		String<STR> ret = String<STR>::make () ;
		GetModuleFileName (NULL ,ret.raw () ,csc_enum_t (ret.size ())) ;
		ret = Path (ret).path () ;
		return move (ret) ;
	}

	String<STR> module_name () const override {
		String<STR> ret = String<STR>::make () ;
		GetModuleFileName (NULL ,ret.raw () ,csc_enum_t (ret.size ())) ;
		ret = Path (ret).name () ;
		return move (ret) ;
	}
} ;

exports VFat<RuntimeProcHolder> RuntimeProcHolder::create (VREF<RuntimeProcLayout> that) {
	return VFat<RuntimeProcHolder> (RuntimeProcImplement () ,that) ;
}

exports CFat<RuntimeProcHolder> RuntimeProcHolder::create (CREF<RuntimeProcLayout> that) {
	return CFat<RuntimeProcHolder> (RuntimeProcImplement () ,that) ;
}

struct ProcessPureLayout {
	FLAG mUid ;
	QUAD mProcessCode ;
	QUAD mProcessTime ;
} ;

class ProcessImplement implement Fat<ProcessHolder ,ProcessLayout> {
private:
	using PROCESS_SNAPSHOT_SIZE = ENUM<128> ;

public:
	void initialize (CREF<FLAG> uid) override {
		fake.mThis = Ref<ProcessPureLayout>::make () ;
		fake.mThis->mUid = uid ;
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,csc_enum_t (uid)) ;
		} ,[&] (VREF<HANDLE> me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
		}) ;
		fake.mThis->mProcessCode = process_code (r1x) ;
		fake.mThis->mProcessTime = process_time (r1x) ;
	}

	QUAD process_code (CREF<HANDLE> handle) const {
		const auto r1x = invoke ([&] () {
			csc_enum_t ret ;
			if ifdo (TRUE) {
				const auto r2x = GetExitCodeProcess (handle ,(&ret)) ;
				if (r2x != ZERO)
					discard ;
				ret = 0 ;
			}
			return move (ret) ;
		}) ;
		return QUAD (r1x) ;
	}

	QUAD process_time (CREF<HANDLE> handle) const {
		const auto r1x = invoke ([&] () {
			BoxBuffer<FILETIME ,RANK4> ret ;
			CoreProc::inline_memset (Pointer::from (ret) ,SIZE_OF<BoxBuffer<FILETIME ,RANK4>>::expr) ;
			GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
			return move (ret) ;
		}) ;
		return ByteProc::bit_merge (CHAR (r1x[0].dwHighDateTime) ,CHAR (r1x[0].dwLowDateTime)) ;
	}

	void initialize (CREF<RefBuffer<BYTE>> snapshot_) override {
		fake.mThis = Ref<ProcessPureLayout>::make () ;
		fake.mThis->mUid = 0 ;
		try {
			assume (snapshot_.size () == PROCESS_SNAPSHOT_SIZE::expr) ;
			auto rax = ByteReader (Ref<RefBuffer<BYTE>>::reference (Pointer::make (address (snapshot_)))) ;
			rax >> slice ("CSC_Process") ;
			rax >> GAP ;
			if ifdo (TRUE) {
				const auto r1x = rax.poll (TYPE<VAL64>::expr) ;
				if (r1x <= 0)
					discard ;
				if (r1x > VAL32_MAX)
					discard ;
				fake.mThis->mUid = FLAG (r1x) ;
			}
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
		if ifnot (r1x)
			return r1x ;
		const auto r2x = inline_equal (fake.mThis->mProcessCode ,that.mThis->mProcessCode) ;
		if ifnot (r2x)
			return r2x ;
		const auto r3x = inline_equal (fake.mThis->mProcessTime ,that.mThis->mProcessTime) ;
		if ifnot (r3x)
			return r3x ;
		return TRUE ;
	}

	FLAG process_uid () const override {
		return fake.mThis->mUid ;
	}

	RefBuffer<BYTE> snapshot () const override {
		auto ret = RefBuffer<BYTE> (PROCESS_SNAPSHOT_SIZE::expr) ;
		auto rax = ByteWriter (Ref<RefBuffer<BYTE>>::reference (ret)) ;
		if ifdo (TRUE) {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,csc_enum_t (fake.mThis->mUid)) ;
			} ,[&] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			if (r1x.self == NULL)
				discard ;
			rax << slice ("CSC_Process") ;
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

exports VFat<ProcessHolder> ProcessHolder::create (VREF<ProcessLayout> that) {
	return VFat<ProcessHolder> (ProcessImplement () ,that) ;
}

exports CFat<ProcessHolder> ProcessHolder::create (CREF<ProcessLayout> that) {
	return CFat<ProcessHolder> (ProcessImplement () ,that) ;
}

struct ModulePureLayout {
	UniqueRef<HMODULE> mModule ;
	FLAG mLastError ;
} ;

class ModuleImplement implement Fat<ModuleHolder ,ModuleLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		const auto r1x = Path (file).name () ;
		assert (r1x.length () > 0) ;
		fake.mThis->mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
			me = GetModuleHandle (r1x.raw ()) ;
			if (me != NULL)
				return ;
			me = LoadLibrary (file.raw ()) ;
			if (me != NULL)
				return ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		} ,[&] (VREF<HMODULE> me) {
			noop () ;
		}) ;
	}

	FLAG load (CREF<String<STR>> name) override {
		assert (name.length () > 0) ;
		const auto r1x = StringProc::stra_from_strs (name) ;
		FLAG ret = FLAG (GetProcAddress (fake.mThis->mModule ,r1x.raw ())) ;
		if ifdo (TRUE) {
			if (ret != ZERO)
				discard ;
			fake.mThis->mLastError = FLAG (GetLastError ()) ;
			assume (FALSE) ;
		}
		return move (ret) ;
	}

	String<STR> error () const override {
		String<STR> ret = String<STR>::make () ;
		const auto r1x = csc_enum_t (fake.mThis->mLastError) ;
		const auto r2x = csc_enum_t (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
		const auto r3x = csc_enum_t (ret.size ()) ;
		FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,r1x ,r2x ,ret.raw () ,r3x ,NULL) ;
		ret = String<STR>::make (slice ("LastError = ") ,FLAG (r1x) ,slice (" : ") ,ret) ;
		return move (ret) ;
	}
} ;

exports VFat<ModuleHolder> ModuleHolder::create (VREF<ModuleLayout> that) {
	return VFat<ModuleHolder> (ModuleImplement () ,that) ;
}

exports CFat<ModuleHolder> ModuleHolder::create (CREF<ModuleLayout> that) {
	return CFat<ModuleHolder> (ModuleImplement () ,that) ;
}

struct SingletonHeap {
	Mutex mMutex ;
	Set<Clazz> mClazzSet ;
} ;

struct SingletonHeapPureLayout {
	RefPureLayout mValue ;
	SingletonHeap mData ;
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
	UniqueRef<HANDLE> mPipe ;
	Ref<SingletonHeap> mHeap ;
} ;

class SingletonProcImplement implement Fat<SingletonProcHolder ,SingletonProcLayout> {
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
		fake.mThis->mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,csc_enum_t (SIZE_OF<SingletonPipe>::expr) ,fake.mThis->mName.raw ()) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		fake.mThis->mHeap = Ref<SingletonHeap>::make () ;
		fake.mThis->mHeap->mMutex = RecursiveMutex::make () ;
		fake.mThis->mHeap->mClazzSet = Set<Clazz> () ;
	}

	SingletonPipe load_pipe () const {
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = OpenFileMapping (FILE_MAP_READ ,FALSE ,fake.mThis->mName.raw ()) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_READ ,0 ,0 ,SIZE_OF<SingletonPipe>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			UnmapViewOfFile (me) ;
		}) ;
		const auto r3x = FLAG (r2x.self) ;
		SingletonPipe ret ;
		CoreProc::inline_memcpy (Pointer::from (ret) ,Pointer::make (r3x) ,SIZE_OF<SingletonPipe>::expr) ;
		assume (ret.mReserve1 == QUAD (0X1122334455667788)) ;
		assume (ret.mReserve3 == QUAD (0XAAAABBBBCCCCDDDD)) ;
		assume (ret.mReserve2 == QUAD (fake.mThis->mUid)) ;
		assume (ret.mAddress1 == ret.mAddress2) ;
		return move (ret) ;
	}

	void save_pipe () const {
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = OpenFileMapping (FILE_MAP_WRITE ,FALSE ,fake.mThis->mName.raw ()) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = MapViewOfFile (r1x ,FILE_MAP_WRITE ,0 ,0 ,SIZE_OF<SingletonPipe>::expr) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			UnmapViewOfFile (me) ;
		}) ;
		const auto r3x = FLAG (r2x.self) ;
		auto rax = SingletonPipe () ;
		rax.mReserve1 = QUAD (0X1122334455667788) ;
		rax.mAddress1 = QUAD (address (fake.mThis->mHeap.self)) ;
		rax.mReserve2 = QUAD (fake.mThis->mUid) ;
		rax.mAddress2 = QUAD (address (fake.mThis->mHeap.self)) ;
		rax.mReserve3 = QUAD (0XAAAABBBBCCCCDDDD) ;
		CoreProc::inline_memcpy (Pointer::make (r3x) ,Pointer::from (rax) ,SIZE_OF<SingletonPipe>::expr) ;
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

exports VFat<SingletonProcHolder> SingletonProcHolder::create (VREF<SingletonProcLayout> that) {
	return VFat<SingletonProcHolder> (SingletonProcImplement () ,that) ;
}

exports CFat<SingletonProcHolder> SingletonProcHolder::create (CREF<SingletonProcLayout> that) {
	return CFat<SingletonProcHolder> (SingletonProcImplement () ,that) ;
}

struct PathPureLayout {
	String<STR> mPathName ;
	LENGTH mLength ;
} ;

class PathImplement implement Fat<PathHolder ,PathLayout> {
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
			if ifnot (is_separator (fake.mThis->mPathName[ix]))
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
			if ifnot (StreamProc::is_alpha (fake.mThis->mPathName[0]))
				discard ;
			if (fake.mThis->mPathName[1] != STRU32 (':'))
				discard ;
			ret = Path (Slice (FLAG (fake.mThis->mPathName.raw ()) ,2 ,SIZE_OF<STR>::expr)) ;
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
		auto rax = WIN32_FIND_DATA () ;
		const auto r1x = String<STR>::make (fake.mThis->mPathName ,slice ("/*.*")) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = FindFirstFile (r1x.raw () ,(&rax)) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			if (me == NULL)
				return ;
			FindClose (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			assume (Slice (FLAG (rax.cFileName) ,1 ,SIZE_OF<STR>::expr) == slice (".")) ;
			FindNextFile (r2x ,(&rax)) ;
			assume (Slice (FLAG (rax.cFileName) ,2 ,SIZE_OF<STR>::expr) == slice ("..")) ;
			while (TRUE) {
				const auto r3x = FindNextFile (r2x ,(&rax)) ;
				if ifnot (r3x)
					break ;
				rbx.add (Slice (rax.cFileName)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (rbx.length ()) ;
		for (auto &&i : iter (0 ,rbx.length ()))
			ret[i] = Path (move (rbx[i])) ;
		return move (ret) ;
	}

	Array<PathLayout> list (CREF<LENGTH> size_) const override {
		auto rax = WIN32_FIND_DATA () ;
		const auto r1x = String<STR>::make (fake.mThis->mPathName ,slice ("/*.*")) ;
		const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = FindFirstFile (r1x.raw () ,(&rax)) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			if (me == NULL)
				return ;
			FindClose (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			assume (Slice (FLAG (rax.cFileName) ,1 ,SIZE_OF<STR>::expr) == slice (".")) ;
			FindNextFile (r2x ,(&rax)) ;
			assume (Slice (FLAG (rax.cFileName) ,2 ,SIZE_OF<STR>::expr) == slice ("..")) ;
			while (TRUE) {
				const auto r3x = FindNextFile (r2x ,(&rax)) ;
				if ifnot (r3x)
					break ;
				if (rbx.length () >= size_)
					break ;
				rbx.add (Slice (rax.cFileName)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (size_) ;
		const auto r4x = inline_min (rbx.length () ,size_) ;
		for (auto &&i : iter (0 ,r4x))
			ret[i] = Path (move (rbx[i])) ;
		return move (ret) ;
	}

	BOOL equal (CREF<PathLayout> that) const override {
		return fake.mThis->mPathName == that.mThis->mPathName ;
	}

	BOOL is_file () const override {
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName.raw ())) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ifnot (ByteProc::bit_any (r1x ,FILE_ATTRIBUTE_DIRECTORY)) ;
	}

	BOOL is_dire () const override {
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName.raw ())) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::bit_any (r1x ,FILE_ATTRIBUTE_DIRECTORY) ;
	}

	BOOL is_link () const override {
		const auto r1x = CHAR (GetFileAttributes (fake.mThis->mPathName.raw ())) ;
		if (r1x == CHAR (INVALID_FILE_ATTRIBUTES))
			return FALSE ;
		return ByteProc::bit_any (r1x ,FILE_ATTRIBUTE_REPARSE_POINT) ;
	}

	Deque<String<STR>> decouple () const override {
		Deque<String<STR>> ret ;
		INDEX ix = 0 ;
		for (auto &&i : fake.mThis->mPathName.range ()) {
			if ifnot (is_separator (fake.mThis->mPathName[i]))
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
		const auto r2x = invoke ([&] () {
			LENGTH ret = 0 ;
			for (auto &&i : iter (0 ,rbx.size ()))
				ret += rbx[i].length () ;
			return move (ret) ;
		}) ;
		String<STR> ret = String<STR> (r2x) ;
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
			if ifnot (StreamProc::is_word (fake.mThis->mPathName[ix]))
				break ;
			if (fake.mThis->mPathName[ix] == STRU32 ('.'))
				return ix ;
			ix-- ;
		}
		return fake.mThis->mLength ;
	}
} ;

exports VFat<PathHolder> PathHolder::create (VREF<PathLayout> that) {
	return VFat<PathHolder> (PathImplement () ,that) ;
}

exports CFat<PathHolder> PathHolder::create (CREF<PathLayout> that) {
	return CFat<PathHolder> (PathImplement () ,that) ;
}

class FileProcImplement implement Fat<FileProcHolder ,FileProcLayout> {
public:
	void initialize () override {
		noop () ;
	}

	RefBuffer<BYTE> load_file (CREF<String<STR>> file) const override {
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (file.raw () ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = file_size (r1x) ;
		assume (r2x <= VAL32_MAX) ;
		const auto r3x = LENGTH (r2x) ;
		RefBuffer<BYTE> ret = RefBuffer<BYTE> (r3x) ;
		auto rax = r3x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r4x = ReadFile (r1x ,(&ret[r3x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r4x) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
		return move (ret) ;
	}

	LENGTH file_size (CREF<HANDLE> handle) const {
		auto rax = BoxBuffer<csc_enum_t ,RANK2> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = LENGTH (ByteProc::bit_merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
		return r1x ;
	}

	void save_file (CREF<String<STR>> file ,CREF<RefBuffer<BYTE>> data) const override {
		assert (data.size () <= VAL32_MAX) ;
		const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (file.raw () ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = data.size () ;
		auto rax = r2x ;
		while (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r3x = WriteFile (r1x ,(&data[r2x - rax]) ,rbx ,(&rbx) ,NULL) ;
			assume (r3x) ;
			rax -= LENGTH (rbx) ;
		}
		assume (rax == 0) ;
	}

	RefBuffer<BYTE> load_asset (CREF<String<STR>> file) const override {
		const auto r1x = FindResource (NULL ,file.raw () ,TEXT (".BIN")) ;
		assume (r1x != NULL) ;
		const auto r2x = LENGTH (SizeofResource (NULL ,r1x)) ;
		assume (r2x > 0) ;
		const auto r3x = LoadResource (NULL ,r1x) ;
		assume (r3x != NULL) ;
		const auto r4x = LockResource (r3x) ;
		RefBufferLayout ret ;
		RefBufferHolder::create (ret)->initialize (RefUnknownBinder<BYTE> ()) ;
		ret.mBuffer = FLAG (r4x) ;
		ret.mSize = r2x ;
		ret.mStep = 1 ;
		return move (keep[TYPE<RefBuffer<BYTE>>::expr] (ret)) ;
	}

	void copy_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = CopyFile (dst.raw () ,src.raw () ,TRUE) ;
		assume (r1x) ;
	}

	void move_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = MoveFile (dst.raw () ,src.raw ()) ;
		assume (r1x) ;
	}

	void link_file (CREF<String<STR>> dst ,CREF<String<STR>> src) const override {
		const auto r1x = CreateHardLink (dst.raw () ,src.raw () ,NULL) ;
		assume (r1x) ;
	}

	void erase_file (CREF<String<STR>> file) const override {
		const auto r1x = DeleteFile (file.raw ()) ;
		assume (r1x) ;
	}

	void build_dire (CREF<String<STR>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		auto rax = String<STR>::make () ;
		for (auto &&i : r1x) {
			rax += i ;
			CreateDirectory (rax.raw () ,NULL) ;
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
			//@mark
			if (rax[ix].m2nd) {
				rax[ix].m2nd = FALSE ;
				const auto r1x = rax[ix].m1st.list () ;
				for (auto &&i : r1x) {
					auto act = TRUE ;
					if ifdo (act) {
						if ifnot (i.is_link ())
							discard ;
						erase_file (i.absolute ()) ;
					}
					if ifdo (act) {
						if ifnot (i.is_file ())
							discard ;
						erase_file (i.absolute ()) ;
					}
					if ifdo (act) {
						if ifnot (i.is_dire ())
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
		const auto r1x = RemoveDirectory (dire.raw ()) ;
		assume (r1x) ;
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

exports VFat<FileProcHolder> FileProcHolder::create (VREF<FileProcLayout> that) {
	return VFat<FileProcHolder> (FileProcImplement () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::create (CREF<FileProcLayout> that) {
	return CFat<FileProcHolder> (FileProcImplement () ,that) ;
}

class StreamFileImplLayout {
protected:
	String<STR> mFile ;
	UniqueRef<HANDLE> mReadPipe ;
	UniqueRef<HANDLE> mWritePipe ;
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
		assert (ifnot (mReadPipe.exist ())) ;
		assert (ifnot (mWritePipe.exist ())) ;
		mReadPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (mFile.raw () ,GENERIC_READ ,r1x ,NULL ,OPEN_EXISTING ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileSize = file_size (mReadPipe) ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void open_w (CREF<LENGTH> count) {
		assert (ifnot (mReadPipe.exist ())) ;
		assert (ifnot (mWritePipe.exist ())) ;
		mWritePipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (mFile.raw () ,GENERIC_WRITE ,r1x ,NULL ,CREATE_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileSize = count ;
		mRead = 0 ;
		mWrite = 0 ;
	}

	void open_a () {
		assert (ifnot (mReadPipe.exist ())) ;
		assert (ifnot (mWritePipe.exist ())) ;
		mReadPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (mFile.raw () ,GENERIC_READ ,r1x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mWritePipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r2x = csc_enum_t (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
			me = CreateFile (mFile.raw () ,GENERIC_WRITE ,r2x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileSize = file_size (mReadPipe) ;
		mRead = 0 ;
		mWrite = 0 ;
		if ifdo (TRUE) {
			auto rax = LONG (0) ;
			const auto r3x = SetFilePointer (mWritePipe ,0 ,(&rax) ,FILE_END) ;
			const auto r4x = VAL64 (ByteProc::bit_merge (CHAR (rax) ,CHAR (r3x))) ;
			if (r4x <= 0)
				return ;
			mWrite += r4x ;
		}
	}

	LENGTH file_size () const {
		return LENGTH (mFileSize) ;
	}

	LENGTH file_size (CREF<HANDLE> handle) const {
		auto rax = BoxBuffer<csc_enum_t ,RANK2> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = LENGTH (ByteProc::bit_merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
		return r1x ;
	}

	void read (VREF<RefBuffer<BYTE>> item) {
		assert (mReadPipe.exist ()) ;
		assert (item.size () <= VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = ReadFile (mReadPipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			if ifnot (r2x)
				continue ;
			rax -= LENGTH (rbx) ;
		}
		mRead += r1x ;
	}

	void write (CREF<RefBuffer<BYTE>> item) {
		assert (mWritePipe.exist ()) ;
		assert (item.size () <= VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			if (rax == 0)
				break ;
			auto rbx = csc_enum_t (rax) ;
			const auto r2x = WriteFile (mWritePipe ,(&item[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
			if ifnot (r2x)
				continue ;
			rax -= LENGTH (rbx) ;
		}
		mWrite += r1x ;
	}

	void flush () {
		if ifnot (mWritePipe.exist ())
			return ;
		FlushFileBuffers (mWritePipe) ;
	}
} ;

class StreamFileImplement implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = Ref<StreamFileImplLayout>::make () ;
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

exports VFat<StreamFileHolder> StreamFileHolder::create (VREF<StreamFileLayout> that) {
	return VFat<StreamFileHolder> (StreamFileImplement () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::create (CREF<StreamFileLayout> that) {
	return CFat<StreamFileHolder> (StreamFileImplement () ,that) ;
}

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
	UniqueRef<Tuple<HANDLE ,LENGTH>> mBuffer ;
} ;

class BufferFileImplLayout {
private:
	using CHUNK_STEP_SIZE = ENUM<4194304> ;
	using HEADER_SIZE = ENUM<65536> ;

protected:
	String<STR> mFile ;
	UniqueRef<HANDLE> mPipe ;
	UniqueRef<HANDLE> mMapping ;
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
		set_block_step (4096) ;
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
		assert (ifnot (mPipe.exist ())) ;
		assert (ifnot (mMapping.exist ())) ;
		mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFile (mFile.raw () ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileSize = file_size (mPipe) ;
		const auto r1x = csc_enum_t (ByteProc::bit_high (QUAD (mFileSize))) ;
		const auto r2x = csc_enum_t (ByteProc::bit_low (QUAD (mFileSize))) ;
		mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFileMapping (mPipe ,NULL ,PAGE_READONLY ,r1x ,r2x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileMapFlag = csc_enum_t (FILE_MAP_READ) ;
		read_header () ;
	}

	void open_w (CREF<LENGTH> count) {
		assert (ifnot (mPipe.exist ())) ;
		assert (ifnot (mMapping.exist ())) ;
		mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (GENERIC_READ | GENERIC_WRITE) ;
			me = CreateFile (mFile.raw () ,r1x ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		const auto r2x = count * mBlockStepSize ;
		const auto r3x = CHUNK_STEP_SIZE::expr / mBlockStepSize ;
		const auto r4x = inline_alignas (r2x ,r3x) / r3x ;
		mFileSize = HEADER_SIZE::expr + r4x * CHUNK_STEP_SIZE::expr ;
		const auto r5x = csc_enum_t (ByteProc::bit_high (QUAD (mFileSize))) ;
		const auto r6x = csc_enum_t (ByteProc::bit_low (QUAD (mFileSize))) ;
		mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFileMapping (mPipe ,NULL ,PAGE_READWRITE ,r5x ,r6x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileMapFlag = csc_enum_t (FILE_MAP_READ | FILE_MAP_WRITE) ;
		write_header () ;
	}

	void open_a () {
		assert (ifnot (mPipe.exist ())) ;
		assert (ifnot (mMapping.exist ())) ;
		assume (mHeader != NULL) ;
		mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			const auto r1x = csc_enum_t (GENERIC_READ | GENERIC_WRITE) ;
			me = CreateFile (mFile.raw () ,r1x ,0 ,NULL ,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileSize = mHeader->mFileSize ;
		const auto r2x = csc_enum_t (ByteProc::bit_high (QUAD (mFileSize))) ;
		const auto r3x = csc_enum_t (ByteProc::bit_low (QUAD (mFileSize))) ;
		mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			me = CreateFileMapping (mPipe ,NULL ,PAGE_READWRITE ,r2x ,r3x ,NULL) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			CloseHandle (me) ;
		}) ;
		mFileMapFlag = csc_enum_t (FILE_MAP_READ | FILE_MAP_WRITE) ;
		write_header () ;
	}

	LENGTH file_size () const {
		return LENGTH (mFileSize) ;
	}

	LENGTH file_size (CREF<HANDLE> handle) const {
		auto rax = BoxBuffer<csc_enum_t ,RANK2> () ;
		rax[0] = GetFileSize (handle ,(&rax[1])) ;
		const auto r1x = LENGTH (ByteProc::bit_merge (CHAR (rax[1]) ,CHAR (rax[0]))) ;
		assume (r1x >= 0) ;
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
		auto &&tmp = keep[TYPE<RefBufferLayout>::expr] (ret.self) ;
		RefBufferHolder::create (tmp)->initialize (RefUnknownBinder<BYTE> ()) ;
		tmp.mBuffer = FLAG (mCacheList[index].mBuffer->m1st) ;
		tmp.mSize = HEADER_SIZE::expr ;
		tmp.mStep = 1 ;
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
		mMapping = UniqueRef<HANDLE> () ;
		mPipe = UniqueRef<HANDLE> () ;
	}

	void get (CREF<VAL64> index ,VREF<RefBuffer<BYTE>> item) {
		assert (mPipe.exist ()) ;
		assume (inline_between (index ,0 ,mHeader->mBlockCount)) ;
		assume (item.size () == mHeader->mBlockStepSize) ;
		const auto r1x = index / mHeader->mChunkCapacity ;
		const auto r2x = index % mHeader->mChunkCapacity * mHeader->mBlockStepSize ;
		const auto r3x = HEADER_SIZE::expr + r1x * mHeader->mChunkStepSize ;
		INDEX ix = load (r3x ,mHeader->mChunkStepSize) ;
		const auto r4x = address (mCacheList[ix].mBuffer->m1st) + r2x ;
		CoreProc::inline_memcpy (Pointer::from (item.self) ,Pointer::make (r4x) ,mHeader->mBlockStepSize) ;
	}

	void set (CREF<VAL64> index ,CREF<RefBuffer<BYTE>> item) {
		assert (mPipe.exist ()) ;
		assume (inline_between (index ,0 ,mHeader->mBlockCount)) ;
		assume (item.size () == mHeader->mBlockStepSize) ;
		const auto r1x = index / mHeader->mChunkCapacity ;
		const auto r2x = index % mHeader->mChunkCapacity * mHeader->mBlockStepSize ;
		const auto r3x = HEADER_SIZE::expr + r1x * mHeader->mChunkStepSize ;
		INDEX ix = load (r3x ,mHeader->mChunkStepSize) ;
		const auto r4x = address (mCacheList[ix].mBuffer->m1st) ;
		CoreProc::inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.self) ,mHeader->mBlockStepSize) ;
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
			const auto r1x = csc_enum_t (ByteProc::bit_high (QUAD (offset))) ;
			const auto r2x = csc_enum_t (ByteProc::bit_low (QUAD (offset))) ;
			mCacheList[ret].mBuffer = UniqueRef<Tuple<HANDLE ,LENGTH>> ([&] (VREF<Tuple<HANDLE ,LENGTH>> me) {
				const auto r3x = MapViewOfFile (mMapping ,mFileMapFlag ,r1x ,r2x ,size_) ;
				assume (r3x != NULL) ;
				me.m1st = r3x ;
				me.m2nd = size_ ;
			} ,[&] (VREF<Tuple<HANDLE ,LENGTH>> me) {
				FlushViewOfFile (me.m1st ,me.m2nd) ;
				UnmapViewOfFile (me.m1st) ;
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
		if ifnot (mPipe.exist ())
			return ;
		mCacheSet.clear () ;
		mCacheList.clear () ;
		mCacheTimer = 0 ;
		FlushFileBuffers (mPipe) ;
	}
} ;

class BufferFileImplement implement Fat<BufferFileHolder ,BufferFileLayout> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake.mThis = Ref<BufferFileImplLayout>::make () ;
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

exports VFat<BufferFileHolder> BufferFileHolder::create (VREF<BufferFileLayout> that) {
	return VFat<BufferFileHolder> (BufferFileImplement () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::create (CREF<BufferFileLayout> that) {
	return CFat<BufferFileHolder> (BufferFileImplement () ,that) ;
}

struct ConsolePureLayout {
	BitSet mOption ;
	UniqueRef<HANDLE> mConsole ;
	RefBuffer<BYTE> mWriterBuffer ;
	TextWriter mWriter ;
	String<STR> mLogFile ;
	String<STR> mOldLogFile ;
} ;

class ConsoleImplement implement Fat<ConsoleHolder ,ConsoleLayout> {
public:
	void initialize () override {
		fake.mMutex = RecursiveMutex::make () ;
		fake.mThis = SharedRef<ConsolePureLayout>::make () ;
		fake.mThis->mOption = BitSet (ConsoleOption::ETC) ;
	}

	void set_option (CREF<FLAG> option) const override {
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

	void log (CREF<String<STR>> tag ,CREF<String<STR>> msg) const {
		fake.mThis->mWriter << CLS ;
		fake.mThis->mWriter << slice ("[") ;
		const auto r1x = Time::current () ;
		const auto r2x = r1x.calendar () ;
		fake.mThis->mWriter << AlignText (r2x.mHour ,2) ;
		fake.mThis->mWriter << slice (":") ;
		fake.mThis->mWriter << AlignText (r2x.mMinute ,2) ;
		fake.mThis->mWriter << slice (":") ;
		fake.mThis->mWriter << AlignText (r2x.mSecond ,2) ;
		fake.mThis->mWriter << slice ("][") ;
		fake.mThis->mWriter << tag ;
		fake.mThis->mWriter << slice ("] : ") ;
		fake.mThis->mWriter << msg ;
		fake.mThis->mWriter << GAP ;
		fake.mThis->mWriter << EOS ;
	}

	void print (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoPrint])
			return ;
		log (slice ("Print") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void fatal (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void error (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void warn (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void info (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_GREEN | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void debug (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void verbose (CREF<String<STR>> msg) const override {
		if (fake.mThis->mOption[ConsoleOption::NoVarbose])
			return ;
		log (slice ("Varbose") ,msg) ;
		if ifdo (TRUE) {
			const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY) ;
			SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
			auto rax = csc_enum_t (fake.mThis->mWriter.length () - 1) ;
			WriteConsole (fake.mThis->mConsole ,(&fake.mThis->mWriterBuffer[0]) ,rax ,(&rax) ,NULL) ;
		}
	}

	void open (CREF<String<STR>> dire) const override {
		fake.mThis->mLogFile = Path (dire).child (slice ("console.log")).absolute () ;
		fake.mThis->mOldLogFile = Path (dire).child (slice ("console.old.log")).absolute () ;
	}

	void start () const override {
		if (fake.mThis->mConsole.exist ())
			if (fake.mThis->mConsole.self != INVALID_HANDLE_VALUE)
				return ;
		fake.mThis->mConsole = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
			AllocConsole () ;
			me = GetStdHandle (STD_OUTPUT_HANDLE) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HANDLE> me) {
			FreeConsole () ;
		}) ;
	}

	void stop () const override {
		fake.mThis->mConsole = UniqueRef<HANDLE>::make (INVALID_HANDLE_VALUE) ;
	}

	void pause () const override {
		if ifdo (TRUE) {
			const auto r1x = GetConsoleWindow () ;
			if (r1x == NULL)
				discard ;
			FlashWindow (r1x ,TRUE) ;
		}
		const auto r2x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (fake.mThis->mConsole ,r2x) ;
		const auto r3x = std::system ("pause") ;
		noop (r3x) ;
	}

	void clear () const override {
		const auto r1x = csc_uint16_t (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) ;
		SetConsoleTextAttribute (fake.mThis->mConsole ,r1x) ;
		const auto r2x = std::system ("cls") ;
		noop (r2x) ;
	}
} ;

exports VFat<ConsoleHolder> ConsoleHolder::create (VREF<ConsoleLayout> that) {
	return VFat<ConsoleHolder> (ConsoleImplement () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::create (CREF<ConsoleLayout> that) {
	return CFat<ConsoleHolder> (ConsoleImplement () ,that) ;
}
} ;