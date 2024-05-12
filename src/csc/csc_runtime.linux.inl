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
			const auto r1x = getcwd (ret.raw () ,csc_size_t (ret.size ())) ;
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
			const auto r1x = readlink ("/proc/self/exe" ,ret.raw () ,csc_size_t (ret.size ())) ;
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
			const auto r1x = readlink ("/proc/self/exe" ,ret.raw () ,csc_size_t (ret.size ())) ;
			if (r1x >= 0)
				discard ;
			ret.clear () ;
		}
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

struct PathPureLayout {
	String<STR> mPathName ;
	LENGTH mLength ;
} ;

class PathImplement implement Fat<PathHolder ,PathLayout> {
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
		auto rax = HDIRENT (NULL) ;
		const auto r1x = StringProc::stra_from_strs (fake.mThis->mPathName) ;
		const auto r2x = UniqueRef<HDIR> ([&] (VREF<HDIR> me) {
			me = opendir (r1x.raw ()) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HDIR> me) {
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			rax = readdir (r2x.self) ;
			assume (Slice (FLAG (rax->d_name) ,1 ,SIZE_OF<STR>::expr) == slice (".")) ;
			rax = readdir (r2x.self) ;
			assume (Slice (FLAG (rax->d_name) ,2 ,SIZE_OF<STR>::expr) == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r2x.self) ;
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
			me = opendir (r1x.raw ()) ;
			assume (me != NULL) ;
		} ,[&] (VREF<HDIR> me) {
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<STR>> () ;
		if ifdo (TRUE) {
			if (r2x == NULL)
				discard ;
			rax = readdir (r2x.self) ;
			assume (Slice (FLAG (rax->d_name) ,1 ,SIZE_OF<STR>::expr) == slice (".")) ;
			rax = readdir (r2x.self) ;
			assume (Slice (FLAG (rax->d_name) ,2 ,SIZE_OF<STR>::expr) == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r2x.self) ;
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
		CoreProc::inline_memset (Pointer::from (rax) ,SIZE_OF<typeof (rax)>::expr) ;
		const auto r1x = stat (fake.mThis->mPathName.raw () ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_dire () const override {
		auto rax = STAT_INFO () ;
		CoreProc::inline_memset (Pointer::from (rax) ,SIZE_OF<typeof (rax)>::expr) ;
		const auto r1x = stat (fake.mThis->mPathName.raw () ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if ifnot (S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	BOOL is_link () const override {
		auto rax = STAT_INFO () ;
		CoreProc::inline_memset (Pointer::from (rax) ,SIZE_OF<typeof (rax)>::expr) ;
		const auto r1x = stat (fake.mThis->mPathName.raw () ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if ifnot (S_ISLNK (rax.st_mode))
			return FALSE ;
		return TRUE ;
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
} ;