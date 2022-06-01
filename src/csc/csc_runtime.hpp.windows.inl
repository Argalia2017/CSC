#pragma once

#ifndef __CSC_RUNTIME__
#error "∑(っ°Д° ;)っ : require 'csc_runtime.hpp'"
#endif

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
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
			return FLAG (GetCurrentThreadId ()) ;
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
			return FLAG (GetCurrentProcessId ()) ;
		}

		void process_exit () const override {
			std::exit (EXIT_FAILURE) ;
		}

		void process_abort () const override {
			unsafe_abort () ;
		}

		String<STR> working_path () const override {
			String<STR> ret = String<STR>::make () ;
			GetCurrentDirectory (DWORD (ret.size ()) ,(&ret[0])) ;
			if ifswitch (TRUE) {
				INDEX ix = ret.length () - 1 ;
				if (ix < 0)
					discard ;
				if (ret[ix] == STRA ('\\'))
					discard ;
				if (ret[ix] == STRA ('/'))
					discard ;
				ret[ix] = STRA ('\\') ;
				ix++ ;
				ret[ix] = 0 ;
			}
			return move (ret) ;
		}

		String<STR> module_path () const override {
			return memorize ([&] () {
				String<STR> ret = String<STR>::make () ;
				GetModuleFileName (NULL ,(&ret[0]) ,DWORD (ret.size ())) ;
				ret = Directory (ret).path () ;
				return move (ret) ;
			}) ;
		}

		String<STR> module_name () const override {
			return memorize ([&] () {
				String<STR> ret = String<STR>::make () ;
				GetModuleFileName (NULL ,(&ret[0]) ,DWORD (ret.size ())) ;
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
			auto rbx = ByteWriter (RegBuffer<BYTE>::from (rax).lift ()) ;
			if ifswitch (TRUE) {
				const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
					me = OpenProcess (PROCESS_QUERY_INFORMATION ,FALSE ,DWORD (mUID)) ;
				} ,[] (VREF<HANDLE> me) {
					if (me == NULL)
						return ;
					CloseHandle (me) ;
				}) ;
				if (r1x.self == NULL)
					discard ;
				rbx << ByteWriter::GAP ;
				rbx << VAL64 (mUID) ;
				rbx << ByteWriter::GAP ;
				rbx << slice ("windows") ;
				rbx << ByteWriter::GAP ;
				const auto r2x = process_code (r1x ,mUID) ;
				rbx << r2x ;
				rbx << ByteWriter::GAP ;
				const auto r3x = process_time (r1x ,mUID) ;
				rbx << r3x ;
			}
			rbx << ByteWriter::GAP ;
			rbx << ByteWriter::EOS ;
			mSnapshot = SNAPSHOT (move (rax)) ;
		}

		DATA process_code (CREF<HANDLE> handle ,CREF<FLAG> uid) const {
			const auto r1x = invoke ([&] () {
				DWORD ret ;
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
			return BitProc::up_bit (CHAR (r1x[0].dwHighDateTime) ,CHAR (r1x[0].dwLowDateTime)) ;
		}

		void initialize (CREF<SNAPSHOT> snapshot_) override {
			mSnapshot = snapshot_ ;
			auto rax = ByteReader (RegBuffer<BYTE>::from (mSnapshot).lift ()) ;
			rax >> ByteReader::GAP ;
			const auto r1x = rax.poll (TYPEAS<VAL64>::expr) ;
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

	class ImplHolder implement Holder {
	protected:
		UniqueRef<HMODULE> mModule ;
		String<STR> mErrorBuffer ;
		String<STR> mError ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file_) override {
			const auto r1x = Directory (file_).name () ;
			assert (ifnot (r1x.empty ())) ;
			mErrorBuffer = String<STR>::make () ;
			mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				me = GetModuleHandle ((&r1x[0])) ;
				if (me != NULL)
					return ;
				me = LoadLibrary ((&r1x[0])) ;
				if (me != NULL)
					return ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
				noop () ;
			}) ;
		}

		CREF<String<STR>> error () const leftvalue override {
			return mError ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (mModule.exist ()) ;
			assert (ifnot (name.empty ())) ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (name) ;
			FLAG ret = FLAG (GetProcAddress (mModule ,(&r1x[0]))) ;
			if ifswitch (TRUE) {
				if (ret != ZERO)
					discard ;
				const auto r2x = FLAG (GetLastError ()) ;
				format_dllerror (r2x) ;
				mError = String<STR>::make (slice ("Error = ") ,r2x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			}
			return move (ret) ;
		}

		void format_dllerror (CREF<FLAG> code) {
			const auto r1x = DWORD (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,DWORD (code) ,r1x ,(&mErrorBuffer[0]) ,DWORD (mErrorBuffer.size ()) ,NULL) ;
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
		String<STR> mName ;
		UniqueRef<HANDLE> mPipe ;
		SharedRef<HEAP> mHeap ;

	public:
		implicit ImplHolder () = default ;

		void initialize () override {
			mName = String<STR>::make (slice ("CSC_Singleton_") ,RuntimeProc::process_uid ()) ;
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
			mHeap = unsafe_deref (unsafe_cast[TYPEAS<TEMP<SharedRef<HEAP>>>::expr] (unsafe_pointer (r1x))) ;
			assume (mHeap.available ()) ;
		}

		void init_pipe () {
			if (mPipe.exist ())
				return ;
			mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,DWORD (SIZE_OF<PIPE>::expr) ,(&mName[0])) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mHeap = SharedRef<HEAP>::make () ;
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
			PIPE ret ;
			zeroize (ret) ;
			std::memcpy ((&ret) ,r2x.self ,SIZE_OF<PIPE>::expr) ;
			assume (ret.mReserve1 == DATA (0X1122334455667788)) ;
			assume (ret.mReserve3 == DATA (0X1122334455667788)) ;
			assume (ret.mReserve2 == DATA (0XAAAABBBBCCCCDDDD)) ;
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
			auto rax = PIPE () ;
			rax.mReserve1 = DATA (0X1122334455667788) ;
			rax.mAddress1 = DATA (address (mHeap)) ;
			rax.mReserve2 = DATA (0XAAAABBBBCCCCDDDD) ;
			rax.mAddress2 = DATA (address (mHeap)) ;
			rax.mReserve3 = DATA (0X1122334455667788) ;
			std::memcpy (r2x.self ,(&rax) ,SIZE_OF<PIPE>::expr) ;
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