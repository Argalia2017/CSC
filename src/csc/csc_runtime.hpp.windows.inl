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

namespace CSC {
template <class DEPEND>
trait RUNTIMEPROC_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename RUNTIMEPROC_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		LENGTH thread_concurrency () const override {
			return LENGTH (std::thread::hardware_concurrency ()) ;
		}

		FLAG thread_uid () const override {
			return FLAG (GetCurrentThreadId ()) ;
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
				const auto r1x = ret.length () ;
				if (r1x < 1)
					discard ;
				if (ret[r1x - 1] == STR ('\\'))
					discard ;
				if (ret[r1x - 1] == STR ('/'))
					discard ;
				ret[r1x] = STR ('\\') ;
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
		implicit ImplHolder () {
			mUID = ZERO ;
		}

		void init_current () override {
			mUID = RuntimeProc::process_uid () ;
			auto rax = ByteWriter (RegBuffer<BYTE>::from (mSnapshot ,0 ,mSnapshot.size ())) ;
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
				rax << ByteWriter::GAP ;
				rax << VAL64 (mUID) ;
				rax << ByteWriter::GAP ;
				rax << slice ("windows") ;
				rax << ByteWriter::GAP ;
				const auto r2x = process_code (r1x ,mUID) ;
				rax << r2x ;
				rax << ByteWriter::GAP ;
				const auto r3x = process_time (r1x ,mUID) ;
				rax << r3x ;
			}
			rax << ByteWriter::GAP ;
			rax << ByteWriter::EOS ;
		}

		DATA process_code (CREF<HANDLE> handle ,CREF<FLAG> uid) {
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

		DATA process_time (CREF<HANDLE> handle ,CREF<FLAG> uid) {
			const auto r1x = invoke ([&] () {
				ARRAY4<FILETIME> ret ;
				zeroize (ret[0]) ;
				zeroize (ret[1]) ;
				zeroize (ret[2]) ;
				zeroize (ret[3]) ;
				GetProcessTimes (handle ,(&ret[0]) ,(&ret[1]) ,(&ret[2]) ,(&ret[3])) ;
				return move (ret) ;
			}) ;
			const auto r2x = (DATA (r1x[0].dwHighDateTime) << 32) | DATA (r1x[0].dwLowDateTime) ;
			return DATA (r2x) ;
		}

		void init_snapshot (CREF<SNAPSHOT> snapshot_) override {
			mSnapshot = snapshot_ ;
			auto &&tmp = keep[TYPEAS<CREF<SNAPSHOT>>::id] (snapshot_) ;
			auto rax = ByteReader (RegBuffer<BYTE>::from (tmp ,0 ,tmp.size ())) ;
			rax >> ByteReader::GAP ;
			const auto r1x = rax.poll (TYPEAS<VAL64>::id) ;
			assume (r1x != ZERO) ;
			assume (r1x >= VAL32_MIN) ;
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

		BOOL equal (CREF<Holder> a) const override {
			const auto r1x = a.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			if (mUID != r1x->mUID)
				return FALSE ;
			return mSnapshot == r1x->mSnapshot ;
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

		void init_new () override {
			noop () ;
		}

		CREF<String<STR>> error () const leftvalue override {
			return mError ;
		}

		void open (CREF<String<STR>> file_) override {
			const auto r1x = Directory (file_).name () ;
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

		void close () override {
			mModule = UniqueRef<HMODULE> () ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assert (mModule.exist ()) ;
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (name) ;
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
			if ifswitch (TRUE) {
				if (mErrorBuffer.size () > 0)
					discard ;
				mErrorBuffer = String<STR>::make () ;
			}
			const auto r1x = DWORD (MAKELANGID (LANG_NEUTRAL ,SUBLANG_DEFAULT)) ;
			const auto r2x = mErrorBuffer.size () ;
			FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM ,NULL ,DWORD (code) ,r1x ,(&mErrorBuffer[0]) ,DWORD (r2x) ,NULL) ;
		}
	} ;
} ;

template <>
exports auto MODULE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;