﻿#pragma once

/*
MIT License

Copyright (c) 2017 Argalia2017

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#include "csc_runtime.hpp"
#include "csc_filesystem.hpp"

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "csc_end.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <locale>
#include <random>
#include "csc_begin.h"

#include "csc_end.h"
#include <unistd.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include "csc_begin.h"

namespace CSC {
template <class DEPEND>
trait RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		void initialize () override {
			noop () ;
		}

		LENGTH thread_concurrency () const override {
			return LENGTH (std::thread::hardware_concurrency ()) ;
		}

		FLAG thread_uid () const override {
			return FLAG (syscall (SYS_gettid)) ;
		}

		void thread_sleep (CREF<TimeDuration> time_) const override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			std::this_thread::sleep_for (r1x->get_mTimeDuration ()) ;
		}

		void thread_sleep (CREF<TimePoint> time_) const override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::expr) ;
			std::this_thread::sleep_until (r1x->get_mTimePoint ()) ;
		}

		void thread_yield () const override {
			std::this_thread::yield () ;
		}

		FLAG process_uid () const override {
			return FLAG (syscall (SYS_getpid)) ;
		}

		void process_exit () const override {
			std::exit (EXIT_FAILURE) ;
		}

		void process_abort () const override {
			unsafe_abort () ;
		}

		String<STR> working_path () const override {
			auto rax = PrintString<STRA>::make () ;
			if ifswitch (TRUE) {
				const auto r1x = getcwd ((&rax[0]) ,VAL32 (rax.size ())) ;
				if (r1x != NULL)
					discard ;
				rax.clear () ;
			}
			if ifswitch (TRUE) {
				INDEX ix = rax.length () - 1 ;
				if (ix < 0)
					discard ;
				if (rax[ix] == STRA ('\\'))
					discard ;
				if (rax[ix] == STRA ('/'))
					discard ;
				ix++ ;
				rax[ix] = STRA ('/') ;
				ix++ ;
				rax.trunc (ix) ;
			}
			return string_cvt[TYPEAS<STR ,STRA>::expr] (rax) ;
		}

		String<STR> module_path () const override {
			return memorize ([&] () {
				auto rax = PrintString<STRA>::make () ;
				const auto r1x = readlink ("/proc/self/exe" ,(&rax[0]) ,VAL32 (rax.size ())) ;
				if ifswitch (TRUE) {
					if (vbetween (r1x ,0 ,rax.size ()))
						discard ;
					rax.clear () ;
				}
				String<STR> ret = string_cvt[TYPEAS<STR ,STRA>::expr] (rax) ;
				ret = Directory (ret).path () ;
				return move (ret) ;
			}) ;
		}

		String<STR> module_name () const override {
			return memorize ([&] () {
				auto rax = PrintString<STRA>::make () ;
				const auto r1x = readlink ("/proc/self/exe" ,(&rax[0]) ,VAL32 (rax.size ())) ;
				if ifswitch (TRUE) {
					if (vbetween (r1x ,0 ,rax.size ()))
						discard ;
					rax.clear () ;
				}
				String<STR> ret = string_cvt[TYPEAS<STR ,STRA>::expr] (rax) ;
				ret = Directory (ret).name () ;
				return move (ret) ;
			}) ;
		}
	} ;
} ;

template <>
exports auto RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait PROCESS_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename PROCESS_HELP<DEPEND ,ALWAYS>::Holder ;
	using SNAPSHOT = typename PROCESS_HELP<DEPEND ,ALWAYS>::SNAPSHOT ;

	class ImplHolder implement Holder {
	protected:
		FLAG mUID ;
		SNAPSHOT mSnapshot ;

	public:
		void initialize (CREF<FLAG> uid) override {
			mUID = uid ;
			auto rax = VarBuffer<BYTE> (128) ;
			auto rbx = ByteWriter (RegBuffer<BYTE>::from (rax).borrow ()) ;
			if ifswitch (TRUE) {
				const auto r1x = PrintString<STR>::make (slice ("/proc/") ,mUID ,slice ("/stat")) ;
				const auto r2x = load_proc_file (r1x) ;
				if (r2x.empty ())
					discard ;
				rbx << GAP ;
				rbx << VAL64 (mUID) ;
				rbx << GAP ;
				rbx << slice ("linux") ;
				rbx << GAP ;
				const auto r3x = process_code (r2x ,mUID) ;
				rbx << r3x ;
				rbx << GAP ;
				const auto r4x = process_time (r2x ,mUID) ;
				rbx << r4x ;
			}
			rbx << GAP ;
			rbx << EOS ;
			mSnapshot = rax.as_cref () ;
		}

		String<STRU8> load_proc_file (CREF<String<STR>> file) const {
			String<STRU8> ret = PrintString<STRU8>::make () ;
			auto rax = StreamFile (file) ;
			rax.open (TRUE ,FALSE) ;
			auto rbx = ret.raw () ;
			const auto r2x = rax.read (RegBuffer<BYTE>::from (rbx)) ;
			ret[r2x] = 0 ;
			return move (ret) ;
		}

		DATA process_code (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
			return DATA (getpgid (pid_t (uid))) ;
		}

		DATA process_time (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
			auto rax = TextReader<STRU8> (info.raw ().borrow ()) ;
			auto rbx = String<STRU8> () ;
			rax >> GAP ;
			rax >> rbx ;
			const auto r1x = string_parse[TYPEAS<VAL64 ,STRU8>::expr] (rbx) ;
			assume (r1x == uid) ;
			rax >> GAP ;
			rax >> slice ("(") ;
			while (TRUE) {
				const auto r2x = rax.poll (TYPEAS<STRU8>::expr) ;
				if (r2x == STRU8 (0))
					break ;
				if (r2x == STRU8 (')'))
					break ;
				assume (r2x != STRU8 ('(')) ;
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
			const auto r3x = string_parse[TYPEAS<VAL64 ,STRU8>::expr] (rbx) ;
			for (auto &&i : iter (19 ,49)) {
				noop (i) ;
				rax >> GAP ;
				rax >> rbx ;
			}
			rax >> GAP ;
			rax >> EOS ;
			return DATA (r3x) ;
		}

		void initialize (CREF<SNAPSHOT> snapshot_) override {
			mUID = 0 ;
			mSnapshot = snapshot_ ;
			try_invoke ([&] () {
				auto rax = ByteReader (RegBuffer<BYTE>::from (mSnapshot).borrow ()) ;
				rax >> GAP ;
				if ifswitch (TRUE) {
					const auto r1x = rax.poll (TYPEAS<VAL64>::expr) ;
					if (r1x <= 0)
						discard ;
					if (r1x > VAL32_MAX)
						discard ;
					mUID = FLAG (r1x) ;
				}
			} ,[&] () {
				noop () ;
			}) ;
		}

		FLAG process_uid () const override {
			return mUID ;
		}

		SNAPSHOT snapshot () const override {
			return mSnapshot ;
		}
	} ;
} ;

template <>
exports auto PROCESS_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename PROCESS_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MODULE_HELP<DEPEND ,ALWAYS>::Holder ;

	using HANDLE = csc_pointer_t ;

	class ImplHolder implement Holder {
	protected:
		UniqueRef<HANDLE> mModule ;
		String<STR> mErrorBuffer ;
		String<STR> mErrorLog ;

	public:
		void initialize () override {
			const auto r1x = dlopen (NULL ,csc_enum_t (RTLD_GLOBAL)) ;
			mModule = UniqueRef<HANDLE>::make (r1x) ;
		}

		void initialize (CREF<String<STR>> file) override {
			const auto r1x = Directory (file).name () ;
			const auto r2x = string_cvt[TYPEAS<STRA ,STR>::expr] (r1x) ;
			assert (ifnot (r2x.empty ())) ;
			mModule = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r3x = csc_enum_t (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
				const auto r4x = csc_enum_t (r3x | RTLD_NOLOAD) ;
				me = dlopen ((&r2x[0]) ,r4x) ;
				if (me != NULL)
					return ;
				me = dlopen ((&r2x[0]) ,r3x) ;
				if (me != NULL)
					return ;
				const auto r5x = FLAG (errno) ;
				format_dllerror () ;
				mErrorLog = PrintString<STR>::make (slice ("Error = ") ,r5x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HANDLE> me) {
				noop () ;
			}) ;
		}

		CREF<String<STR>> error () const leftvalue override {
			return mErrorLog ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (ifnot (name.empty ())) ;
			const auto r1x = string_cvt[TYPEAS<STRA ,STR>::expr] (name) ;
			assume (mModule.exist ()) ;
			FLAG ret = FLAG (dlsym (mModule ,(&r1x[0]))) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (errno) ;
				format_dllerror () ;
				mErrorLog = PrintString<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

		void format_dllerror () {
			if ifswitch (TRUE) {
				if (mErrorBuffer.size () > 0)
					discard ;
				mErrorBuffer = PrintString<STR>::make () ;
			}
			const auto r1x = FLAG (dlerror ()) ;
			assume (r1x != ZERO) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<ARR<STRA>>>::expr] (unsafe_pointer (r1x))) ;
			mErrorBuffer -= BufferProc<STR>::buf_slice (tmp ,mErrorBuffer.size ()) ;
		}
	} ;
} ;

template <>
exports auto MODULE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait SINGLETON_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	using HFILE = csc_int32_t ;
	using HANDLE = csc_pointer_t ;

	struct HEAP {
		Mutex mMutex ;
		Set<Slice<STR>> mAddressSet ;
	} ;

	struct PIPE {
		DATA mReserve1 ;
		DATA mAddress1 ;
		DATA mReserve2 ;
		DATA mAddress2 ;
		DATA mReserve3 ;
	} ;

	using HEAPROOT = typename SHAREDREF_PUREHOLDER_HELP<HEAP ,ALWAYS>::PureHolder ;

	class ImplHolder implement Holder {
	protected:
		FLAG mUID ;
		String<STRA> mName ;
		UniqueRef<String<STRA>> mPipe ;
		SharedRef<HEAP> mHeap ;
		BOOL mWeakHeap ;

	public:
		void initialize () override {
			mUID = RuntimeProc::process_uid () ;
			mName = PrintString<STRA>::make (slice ("CSC_Singleton_") ,mUID) ;
			auto rax = PIPE () ;
			try_invoke ([&] () {
				init_pipe () ;
				rax = load_pipe () ;
			} ,[&] () {
				rax = load_pipe () ;
			} ,[&] () {
				save_pipe () ;
				rax = load_pipe () ;
			} ,[&] () {
				zeroize (rax) ;
			}) ;
			if ifswitch (TRUE) {
				const auto r1x = FLAG (rax.mAddress1) ;
				assume (r1x != ZERO) ;
				if (address (mHeap) == r1x)
					discard ;
				auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<SharedRef<HEAP>>>::expr] (unsafe_pointer (r1x))) ;
				mHeap = tmp.weak () ;
				mWeakHeap = TRUE ;
				assume (mHeap.available ()) ;
			}
		}

		void init_pipe () {
			if (mPipe.exist ())
				return ;
			mPipe = UniqueRef<String<STRA>> ([&] (VREF<String<STRA>> me) {
				me = mName ;
				const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
					const auto r2x = csc_enum_t (O_CREAT | O_RDWR | O_EXCL) ;
					const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
					me = shm_open ((&mName[0]) ,r2x ,r3x) ;
					assume (me != NONE) ;
				} ,[] (VREF<HFILE> me) {
					noop () ;
				}) ;
				const auto r4x = ftruncate (r1x ,SIZE_OF<PIPE>::expr) ;
				assume (r4x == 0) ;
			} ,[] (VREF<String<STRA>> me) {
				shm_unlink ((&me[0])) ;
			}) ;
			mHeap = SharedRef<HEAP>::intrusive (heap_root ()) ;
			mWeakHeap = FALSE ;
			mHeap->mMutex = RecursiveMutex::make () ;
		}

		CRef<HEAPROOT> heap_root () const {
			return CRef<HEAPROOT>::reference (memorize ([&] () {
				Box<HEAPROOT> ret = Box<HEAPROOT>::make () ;
				ret->initialize () ;
				return move (ret) ;
			}).self) ;
		}

		PIPE load_pipe () const {
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				me = shm_open ((&mName[0]) ,O_RDONLY ,0) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				noop () ;
			}) ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = mmap (NULL ,SIZE_OF<PIPE>::expr ,PROT_READ ,MAP_SHARED ,r1x ,0) ;
				replace (me ,MAP_FAILED ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				munmap (me ,SIZE_OF<PIPE>::expr) ;
			}) ;
			const auto r3x = FLAG (r2x.self) ;
			PIPE ret ;
			unsafe_sync (unsafe_deptr (ret) ,unsafe_pointer (r3x)) ;
			unsafe_launder (ret) ;
			assume (ret.mReserve1 == DATA (0X1122334455667788)) ;
			assume (ret.mReserve3 == DATA (0XAAAABBBBCCCCDDDD)) ;
			assume (ret.mReserve2 == DATA (mUID)) ;
			assume (ret.mAddress1 == ret.mAddress2) ;
			return move (ret) ;
		}

		void save_pipe () const {
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				me = shm_open ((&mName[0]) ,O_RDWR ,0) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				noop () ;
			}) ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = mmap (NULL ,SIZE_OF<PIPE>::expr ,PROT_WRITE ,MAP_SHARED ,r1x ,0) ;
				replace (me ,MAP_FAILED ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				munmap (me ,SIZE_OF<PIPE>::expr) ;
			}) ;
			const auto r3x = FLAG (r2x.self) ;
			auto rax = PIPE () ;
			rax.mReserve1 = DATA (0X1122334455667788) ;
			rax.mAddress1 = DATA (address (mHeap)) ;
			rax.mReserve2 = DATA (mUID) ;
			rax.mAddress2 = DATA (address (mHeap)) ;
			rax.mReserve3 = DATA (0XAAAABBBBCCCCDDDD) ;
			unsafe_sync (unsafe_pointer (r3x) ,unsafe_deptr (ret)) ;
		}

		void regi (CREF<Slice<STR>> name ,CREF<FLAG> addr) const override {
			Scope<Mutex> anonymous (mHeap->mMutex) ;
			assert (addr != ZERO) ;
			assert (addr != NONE) ;
			mHeap->mAddressSet.add (name ,addr) ;
		}

		FLAG link (CREF<Slice<STR>> name) const override {
			Scope<Mutex> anonymous (mHeap->mMutex) ;
			FLAG ret = mHeap->mAddressSet.map (name) ;
			replace (ret ,NONE ,ZERO) ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename SINGLETON_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;