#pragma once

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

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
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
			return FLAG (GetCurrentThreadId ()) ;
		}

		void thread_sleep (CREF<TimeDuration> time_) const override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = CRef<R1X> (time_.native ()) ;
			std::this_thread::sleep_for (r1x->get_mTimeDuration ()) ;
		}

		void thread_sleep (CREF<TimePoint> time_) const override {
			using R1X = typename TIMEDURATION_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
			const auto r1x = CRef<R1X> (time_.native ()) ;
			std::this_thread::sleep_until (r1x->get_mTimePoint ()) ;
		}

		void thread_yield () const override {
			std::this_thread::yield () ;
		}

		FLAG process_uid () const override {
			return FLAG (GetCurrentProcessId ()) ;
		}

		void process_exit () const override {
			std::exit (EXIT_FAILURE) ;
		}

		void process_abort () const override {
			unsafe_abort () ;
		}

		String<STR> working_path () const override {
			String<STR> ret = PrintString<STR>::make () ;
			GetCurrentDirectory (VAL32 (ret.size ()) ,(&ret[0])) ;
			if ifswitch (TRUE) {
				INDEX ix = ret.length () - 1 ;
				if (ix < 0)
					discard ;
				if (ret[ix] == STRA ('\\'))
					discard ;
				if (ret[ix] == STRA ('/'))
					discard ;
				ix++ ;
				ret[ix] = STRA ('\\') ;
				ix++ ;
				ret.trunc (ix) ;
			}
			return move (ret) ;
		}

		CREF<String<STR>> module_path () const override {
			return memorize ([&] () {
				String<STR> ret = PrintString<STR>::make () ;
				GetModuleFileName (NULL ,(&ret[0]) ,VAL32 (ret.size ())) ;
				ret = Directory (ret).path () ;
				return move (ret) ;
			}) ;
		}

		CREF<String<STR>> module_name () const override {
			return memorize ([&] () {
				String<STR> ret = PrintString<STR>::make () ;
				GetModuleFileName (NULL ,(&ret[0]) ,VAL32 (ret.size ())) ;
				ret = Directory (ret).name () ;
				return move (ret) ;
			}) ;
		}
	} ;
} ;

template <>
exports auto RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait THREADLOCAL_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename THREADLOCAL_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		UniqueRef<csc_enum_t> mTLSHandle ;
		Atomic mCounter ;

	public:
		void initialize () override {
			mTLSHandle = UniqueRef<csc_enum_t> ([&] (VREF<csc_enum_t> me) {
				me = TlsAlloc () ;
				assume (me != TLS_OUT_OF_INDEXES) ;
			} ,[] (VREF<csc_enum_t> me) {
				TlsFree (me) ;
			}) ;
			mCounter = Atomic (0) ;
		}

		INDEX local () const override {
			INDEX ret = INDEX (TlsGetValue (mTLSHandle)) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				ret = mCounter.increase () ;
				TlsSetValue (mTLSHandle ,csc_pointer_t (ret)) ;
			}
			ret-- ;
			return move (ret) ;
		}
	} ;
} ;

template <>
exports auto THREADLOCAL_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename THREADLOCAL_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
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
				const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
					me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,csc_enum_t (mUID)) ;
				} ,[] (VREF<HANDLE> me) {
					if (me == NULL)
						return ;
					CloseHandle (me) ;
				}) ;
				if (r1x.self == NULL)
					discard ;
				rbx << GAP ;
				rbx << VAL64 (mUID) ;
				rbx << GAP ;
				rbx << slice ("windows") ;
				rbx << GAP ;
				const auto r2x = process_code (r1x ,mUID) ;
				rbx << r2x ;
				rbx << GAP ;
				const auto r3x = process_time (r1x ,mUID) ;
				rbx << r3x ;
			}
			rbx << GAP ;
			rbx << EOS ;
			mSnapshot = move (rax) ;
		}

		DATA process_code (CREF<HANDLE> handle ,CREF<FLAG> uid) const {
			const auto r1x = invoke ([&] () {
				csc_enum_t ret ;
				if ifswitch (TRUE) {
					const auto r2x = GetExitCodeProcess (handle ,(&ret)) ;
					if (r2x != ZERO)
						discard ;
					ret = 0 ;
				}
				return move (ret) ;
			}) ;
			return DATA (r1x) ;
		}

		DATA process_time (CREF<HANDLE> handle ,CREF<FLAG> uid) const {
			const auto r1x = invoke ([&] () {
				ARRAY4<FILETIME> ret ;
				zeroize (ret[0]) ;
				zeroize (ret[1]) ;
				zeroize (ret[2]) ;
				zeroize (ret[3]) ;
				GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
				return move (ret) ;
			}) ;
			return BitProc::bit_merge (CHAR (r1x[0].dwHighDateTime) ,CHAR (r1x[0].dwLowDateTime)) ;
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
exports auto PROCESS_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename PROCESS_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename MODULE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		UniqueRef<HMODULE> mModule ;
		String<STR> mErrorBuffer ;
		String<STR> mErrorLog ;

	public:
		void initialize () override {
			const auto r1x = GetModuleHandle (NULL) ;
			mModule = UniqueRef<HMODULE>::make (r1x) ;
		}

		void initialize (CREF<String<STR>> file) override {
			const auto r1x = Directory (file).name () ;
			assert (ifnot (r1x.empty ())) ;
			mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				me = GetModuleHandle ((&r1x[0])) ;
				if (me != NULL)
					return ;
				me = LoadLibrary ((&file[0])) ;
				if (me != NULL)
					return ;
				const auto r2x = csc_enum_t (GetLastError ()) ;
				format_dllerror (r2x) ;
				mErrorLog = PrintString<STR>::make (slice ("Error = ") ,FLAG (r2x) ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
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
			FLAG ret = FLAG (GetProcAddress (mModule ,(&r1x[0]))) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = csc_enum_t (GetLastError ()) ;
				format_dllerror (r2x) ;
				mErrorLog = PrintString<STR>::make (slice ("Error = ") ,FLAG (r2x) ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

		void format_dllerror (CREF<csc_enum_t> code) {
			if ifswitch (TRUE) {
				if (mErrorBuffer.size () > 0)
					discard ;
				mErrorBuffer = PrintString<STR>::make () ;
			}
			const auto r1x = csc_enum_t (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,code ,r1x ,(&mErrorBuffer[0]) ,VAL32 (mErrorBuffer.size ()) ,NULL) ;
		}
	} ;
} ;

template <>
exports auto MODULE_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait SINGLETON_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;

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
		String<STR> mName ;
		UniqueRef<HANDLE> mPipe ;
		SharedRef<HEAP> mHeap ;
		BOOL mWeakHeap ;

	public:
		void initialize () override {
			mUID = RuntimeProc::process_uid () ;
			mName = PrintString<STR>::make (slice ("CSC_Singleton_") ,mUID) ;
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
				auto &&tmp = unsafe_cast[TYPEAS<SharedRef<HEAP>>::expr] (unsafe_deref (r1x)) ;
				mHeap = tmp.weak () ;
				mWeakHeap = TRUE ;
				assume (mHeap.good ()) ;
			}
		}

		void init_pipe () {
			if (mPipe.exist ())
				return ;
			mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,VAL32 (SIZE_OF<PIPE>::expr) ,(&mName[0])) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
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
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = OpenFileMapping (FILE_MAP_READ ,FALSE ,(&mName[0])) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = MapViewOfFile (r1x ,FILE_MAP_READ ,0 ,0 ,SIZE_OF<PIPE>::expr) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				UnmapViewOfFile (me) ;
			}) ;
			const auto r3x = FLAG (r2x.self) ;
			PIPE ret ;
			unsafe_sync (unsafe_cast[TYPEAS<TEMP<PIPE>>::expr] (ret) ,unsafe_deref (r3x)) ;
			unsafe_launder (ret) ;
			assume (ret.mReserve1 == DATA (0X1122334455667788)) ;
			assume (ret.mReserve3 == DATA (0XAAAABBBBCCCCDDDD)) ;
			assume (ret.mReserve2 == DATA (mUID)) ;
			assume (ret.mAddress1 == ret.mAddress2) ;
			return move (ret) ;
		}

		void save_pipe () const {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = OpenFileMapping (FILE_MAP_WRITE ,FALSE ,(&mName[0])) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = MapViewOfFile (r1x ,FILE_MAP_WRITE ,0 ,0 ,SIZE_OF<PIPE>::expr) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				UnmapViewOfFile (me) ;
			}) ;
			const auto r3x = FLAG (r2x.self) ;
			auto rax = PIPE () ;
			rax.mReserve1 = DATA (0X1122334455667788) ;
			rax.mAddress1 = DATA (address (mHeap)) ;
			rax.mReserve2 = DATA (mUID) ;
			rax.mAddress2 = DATA (address (mHeap)) ;
			rax.mReserve3 = DATA (0XAAAABBBBCCCCDDDD) ;
			unsafe_sync (unsafe_deref (r3x) ,unsafe_cast[TYPEAS<TEMP<PIPE>>::expr] (rax)) ;
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
exports auto SINGLETON_HOLDER_HELP<DEPEND ,ALWAYS>::Holder::create () ->VRef<Holder> {
	using R1X = typename SINGLETON_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;