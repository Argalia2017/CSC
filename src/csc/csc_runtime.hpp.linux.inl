﻿#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "begin.h"
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
#include "end.h"

#include "begin.h"
#include <unistd.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include "end.h"

namespace CSC {
template <class DEPEND>
trait RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		implicit ImplHolder () = default ;

		LENGTH thread_concurrency () const override {
			return LENGTH (std::thread::hardware_concurrency ()) ;
		}

		FLAG thread_uid () const override {
			return FLAG (syscall (SYS_gettid)) ;
		}

		void thread_sleep (CREF<TimeDuration> time_) const override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::id) ;
			std::this_thread::sleep_for (r1x->get_mTimeDuration ()) ;
		}

		void thread_sleep (CREF<TimePoint> time_) const override {
			using R1X = typename TIMEPOINT_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = time_.native ().poll (TYPEAS<CRef<R1X>>::id) ;
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
			auto rax = String<STRA>::make () ;
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
				rax[ix] = STRA ('/') ;
				ix++ ;
				rax[ix] = 0 ;
			}
			return string_cvt[TYPEAS<TYPEAS<STR ,STRA>>::id] (rax) ;
		}

		String<STR> module_path () const override {
			return memorize ([&] () {
				auto rax = String<STRA>::make () ;
				const auto r1x = readlink ("/proc/self/exe" ,(&rax[0]) ,VAL32 (rax.size ())) ;
				if ifswitch (TRUE) {
					if (vbetween (r1x ,0 ,rax.size ()))
						discard ;
					rax.clear () ;
				}
				String<STR> ret = string_cvt[TYPEAS<TYPEAS<STR ,STRA>>::id] (rax) ;
				ret = Directory (ret).path () ;
				return move (ret) ;
			}) ;
		}

		String<STR> module_name () const override {
			return memorize ([&] () {
				auto rax = String<STRA>::make () ;
				const auto r1x = readlink ("/proc/self/exe" ,(&rax[0]) ,VAL32 (rax.size ())) ;
				if ifswitch (TRUE) {
					if (vbetween (r1x ,0 ,rax.size ()))
						discard ;
					rax.clear () ;
				}
				String<STR> ret = string_cvt[TYPEAS<TYPEAS<STR ,STRA>>::id] (rax) ;
				ret = Directory (ret).name () ;
				return move (ret) ;
			}) ;
		}
	} ;
} ;

template <>
exports auto RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
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
		implicit ImplHolder () = default ;

		void initialize (CREF<FLAG> uid) override {
			mUID = uid ;
			auto rax = VarBuffer<BYTE> (128) ;
			auto rbx = ByteWriter (RegBuffer<BYTE>::from (rax ,0 ,rax.size ())) ;
			if ifswitch (TRUE) {
				const auto r1x = String<STR>::make (slice ("/proc/") ,mUID ,slice ("/stat")) ;
				const auto r2x = load_proc_file (r1x) ;
				if (r2x.empty ())
					discard ;
				rbx << ByteWriter::GAP ;
				rbx << VAL64 (mUID) ;
				rbx << ByteWriter::GAP ;
				rbx << slice ("linux") ;
				rbx << ByteWriter::GAP ;
				const auto r3x = process_code (r2x ,mUID) ;
				rbx << r3x ;
				rbx << ByteWriter::GAP ;
				const auto r4x = process_time (r2x ,mUID) ;
				rbx << r4x ;
			}
			rbx << ByteWriter::GAP ;
			rbx << ByteWriter::EOS ;
			mSnapshot = SNAPSHOT (move (rax)) ;
		}

		String<STRU8> load_proc_file (CREF<String<STR>> file_) const {
			String<STRU8> ret = String<STRU8>::make () ;
			auto rax = StreamFile (file_) ;
			const auto r1x = rax.link (TRUE ,FALSE) ;
			assume (r1x) ;
			auto rbx = RegBuffer<BYTE>::from (unsafe_array (ret[0]) ,0 ,ret.size ()) ;
			const auto r2x = rax.read (rbx) ;
			ret[r2x] = 0 ;
			return move (ret) ;
		}

		DATA process_code (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
			return DATA (getpgid (pid_t (uid))) ;
		}

		DATA process_time (CREF<String<STRU8>> info ,CREF<FLAG> uid) const {
			auto rax = TextReader<STRU8> (info.raw ()) ;
			auto rbx = String<STRU8> () ;
			rax >> TextReader<STRU8>::GAP ;
			rax >> rbx ;
			const auto r1x = string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id] (rbx) ;
			assume (r1x == uid) ;
			rax >> TextReader<STRU8>::GAP ;
			rax >> slice ("(") ;
			while (TRUE) {
				const auto r2x = rax.poll (TYPEAS<STRU8>::id) ;
				if (r2x == STRU8 ('\0'))
					break ;
				if (r2x == STRU8 (')'))
					break ;
				assume (r2x != STRU8 ('(')) ;
			}
			rax >> TextReader<STRU8>::GAP ;
			rax >> rbx ;
			assume (rbx.length () == 1) ;
			for (auto &&i : iter (0 ,18)) {
				noop (i) ;
				rax >> TextReader<STRU8>::GAP ;
				rax >> rbx ;
			}
			rax >> TextReader<STRU8>::GAP ;
			rax >> rbx ;
			const auto r3x = string_parse[TYPEAS<TYPEAS<VAL64 ,STRU8>>::id] (rbx) ;
			for (auto &&i : iter (19 ,49)) {
				noop (i) ;
				rax >> TextReader<STRU8>::GAP ;
				rax >> rbx ;
			}
			rax >> TextReader<STRU8>::GAP ;
			rax >> TextReader<STRU8>::EOS ;
			return DATA (r3x) ;
		}

		void initialize (CREF<SNAPSHOT> snapshot_) override {
			mSnapshot = snapshot_ ;
			auto &&tmp = keep[TYPEAS<CREF<SNAPSHOT>>::id] (mSnapshot) ;
			auto rax = ByteReader (RegBuffer<BYTE>::from (tmp ,0 ,tmp.size ())) ;
			rax >> ByteReader::GAP ;
			const auto r1x = rax.poll (TYPEAS<VAL64>::id) ;
			assume (r1x > 0) ;
			assume (r1x <= VAL32_MAX) ;
			mUID = FLAG (r1x) ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		FLAG process_uid () const override {
			return mUID ;
		}

		CREF<SNAPSHOT> snapshot () const override {
			return mSnapshot ;
		}
	} ;
} ;

template <>
exports auto PROCESS_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
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
		String<STR> mError ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file_) override {
			const auto r1x = Directory (file_).name () ;
			const auto r2x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (r1x) ;
			assert (ifnot (r2x.empty ())) ;
			mErrorBuffer = String<STR>::make () ;
			mModule = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r3x = VAL32 (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
				const auto r4x = VAL32 (r3x | RTLD_NOLOAD) ;
				me = dlopen ((&r2x[0]) ,r4x) ;
				if (me != NULL)
					return ;
				me = dlopen ((&r2x[0]) ,r3x) ;
				if (me != NULL)
					return ;
				const auto r5x = FLAG (errno) ;
				format_dllerror () ;
				mError = String<STR>::make (slice ("Error = ") ,r5x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HANDLE> me) {
				noop () ;
			}) ;
		}

		CREF<String<STR>> error () const leftvalue override {
			return mError ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (mModule.exist ()) ;
			assert (ifnot (name.empty ())) ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (name) ;
			FLAG ret = FLAG (dlsym (mModule ,(&r1x[0]))) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (errno) ;
				format_dllerror () ;
				mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

		void format_dllerror () {
			const auto r1x = FLAG (dlerror ()) ;
			assume (r1x != ZERO) ;
			auto &&tmp = keep[TYPEAS<CREF<STRA>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<STRA>>::id] (unsafe_pointer (r1x)))) ;
			BufferProc::buf_slice (mErrorBuffer ,unsafe_array (tmp) ,mErrorBuffer.size ()) ;
		}
	} ;
} ;

template <>
exports auto MODULE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait SINGLETON_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

	using HFILE = csc_int32_t ;
	using HANDLE = csc_pointer_t ;

	struct HEAP {
		RecursiveMutex mMutex ;
		Set<Slice<STR>> mAddressSet ;
	} ;

	struct PIPE {
		DATA mReserve1 ;
		DATA mAddress1 ;
		DATA mReserve2 ;
		DATA mAddress2 ;
		DATA mReserve3 ;
	} ;

	class ImplHolder implement Holder {
	protected:
		String<STRA> mName ;
		UniqueRef<String<STRA>> mPipe ;
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mName = String<STRA>::make (slice ("CSC_Singleton_") ,RuntimeProc::process_uid ()) ;
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
			const auto r1x = FLAG (rax.mAddress1) ;
			assume (r1x != ZERO) ;
			mHeap = unsafe_deref (unsafe_cast[TYPEAS<TEMP<SharedRef<HEAP>>>::id] (unsafe_pointer (r1x))) ;
			assume (mHeap.available ()) ;
		}

		void init_pipe () {
			if (mPipe.exist ())
				return ;
			mPipe = UniqueRef<String<STRA>> ([&] (VREF<String<STRA>> me) {
				me = mName ;
				const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
					const auto r2x = VAL32 (O_CREAT | O_RDWR | O_EXCL) ;
					const auto r3x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
					me = shm_open ((&mName[0]) ,r2x ,r3x) ;
					assume (me != NONE) ;
				} ,[] (VREF<HFILE> me) {
					noop () ;
				}) ;
				const auto r4x = ftruncate (r1x ,SIZE_OF<PIPE>::value) ;
				assume (r4x == 0) ;
			} ,[] (VREF<String<STRA>> me) {
				shm_unlink ((&me[0])) ;
			}) ;
			mHeap = SharedRef<HEAP>::make () ;
		}

		PIPE load_pipe () const {
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				me = shm_open ((&mName[0]) ,O_RDONLY ,0) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				noop () ;
			}) ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = mmap (NULL ,SIZE_OF<PIPE>::value ,PROT_READ ,MAP_SHARED ,r1x ,0) ;
				replace (me ,MAP_FAILED ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				munmap (me ,SIZE_OF<PIPE>::value) ;
			}) ;
			PIPE ret ;
			zeroize (ret) ;
			std::memcpy ((&ret) ,r2x ,SIZE_OF<PIPE>::value) ;
			assume (ret.mReserve1 == DATA (0X1122334455667788)) ;
			assume (ret.mReserve3 == DATA (0X1122334455667788)) ;
			assume (ret.mReserve2 == DATA (0XAAAABBBBCCCCDDDD)) ;
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
				me = mmap (NULL ,SIZE_OF<PIPE>::value ,PROT_WRITE ,MAP_SHARED ,r1x ,0) ;
				replace (me ,MAP_FAILED ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				munmap (me ,SIZE_OF<PIPE>::value) ;
			}) ;
			auto rax = PIPE () ;
			rax.mReserve1 = DATA (0X1122334455667788) ;
			rax.mAddress1 = DATA (address (mHeap)) ;
			rax.mReserve2 = DATA (0XAAAABBBBCCCCDDDD) ;
			rax.mAddress2 = DATA (address (mHeap)) ;
			rax.mReserve3 = DATA (0X1122334455667788) ;
			std::memcpy (r2x ,(&rax) ,SIZE_OF<PIPE>::value) ;
		}

		void add (CREF<Slice<STR>> name ,CREF<FLAG> addr_) const override {
			Scope<Mutex> anonymous (mHeap->mMutex) ;
			assert (addr_ != ZERO) ;
			assert (addr_ != NONE) ;
			mHeap->mAddressSet.add (name ,addr_) ;
		}

		FLAG map (CREF<Slice<STR>> name) const override {
			Scope<Mutex> anonymous (mHeap->mMutex) ;
			FLAG ret = mHeap->mAddressSet.map (name) ;
			replace (ret ,NONE ,ZERO) ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename SINGLETON_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;