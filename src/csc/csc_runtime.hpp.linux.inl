#pragma once

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
#include <unistd.h>
#include <dlfcn.h>
#include <sys/syscall.h>
#include "end.h"

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
				const auto r2x = rax.length () ;
				if (r2x < 1)
					discard ;
				if (rax[r2x - 1] == STRA ('\\'))
					discard ;
				if (rax[r2x - 1] == STRA ('/'))
					discard ;
				rax[r2x] = STRA ('/') ;
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
		implicit ImplHolder () {
			mUID = ZERO ;
		}

		void init_current () override {
			mUID = RuntimeProc::process_uid () ;
			auto rax = ByteWriter (RegBuffer<BYTE>::from (mSnapshot ,0 ,mSnapshot.size ())) ;
			if ifswitch (TRUE) {
				const auto r1x = File (String<STR>::make (slice ("/proc/") ,mUID ,slice ("/stat"))) ;
				if ifnot (r1x.find ())
					discard ;
				rax << ByteWriter::GAP ;
				rax << VAL64 (mUID) ;
				rax << ByteWriter::GAP ;
				rax << slice ("linux") ;
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

		DATA process_code (CREF<File> handle ,CREF<FLAG> uid) {
			return DATA (getpgid (pid_t (uid))) ;
		}

		DATA process_time (CREF<File> handle ,CREF<FLAG> uid) {
			const auto r1x = handle.load () ;
			auto rax = TextReader<STRU8> (RegBuffer<BYTE>::from (r1x ,0 ,r1x.size ())) ;
			rax >> TextReader<STRU8>::GAP ;
			const auto r2x = rax.poll (TYPEAS<VAL64>::id) ;
			assume (r2x == uid) ;
			rax >> TextReader<STRU8>::GAP ;
			rax >> slice ("(") ;
			while (TRUE) {
				if (rax.length () >= r1x.size ())
					break ;
				const auto r3x = rax.poll (TYPEAS<STRU8>::id) ;
				if (r3x == STRU8 (')'))
					break ;
				assume (r3x != STRU8 ('(')) ;
			}
			rax >> TextReader<STRU8>::GAP ;
			const auto r4x = rax.poll (TYPEAS<STRU8>::id) ;
			noop (r4x) ;
			for (auto &&i : iter (0 ,18)) {
				noop (i) ;
				rax >> TextReader<STRU8>::GAP ;
				const auto r5x = rax.poll (TYPEAS<VAL64>::id) ;
				noop (r5x) ;
			}
			rax >> TextReader<STRU8>::GAP ;
			const auto r6x = rax.poll (TYPEAS<VAL64>::id) ;
			for (auto &&i : iter (19 ,49)) {
				noop (i) ;
				rax >> TextReader<STRU8>::GAP ;
				const auto r7x = rax.poll (TYPEAS<VAL64>::id) ;
				noop (r7x) ;
			}
			rax >> TextReader<STRU8>::EOS ;
			return DATA (r6x) ;
		}

		void init_snapshot (CREF<SNAPSHOT> snapshot_) override {
			mSnapshot = snapshot_ ;
			auto &&tmp = keep[TYPEAS<CREF<SNAPSHOT>>::id] (mSnapshot) ;
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
	using HMODULE = csc_pointer_t ;

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
			const auto r1x = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::id] (file_) ;
			mModule = UniqueRef<HMODULE> ([&] (VREF<HMODULE> me) {
				const auto r2x = VAL32 (RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND | RTLD_NODELETE) ;
				const auto r3x = VAL32 (RTLD_NOLOAD | r2x) ;
				me = dlopen ((&r1x[0]) ,r3x) ;
				if (me != NULL)
					return ;
				me = dlopen ((&r1x[0]) ,r2x) ;
				if (me != NULL)
					return ;
				const auto r4x = FLAG (errno) ;
				format_dllerror () ;
				mError = String<STR>::make (slice ("Error = ") ,r4x ,slice (" : ") ,mErrorBuffer) ;
				assume (FALSE) ;
			} ,[] (VREF<HMODULE> me) {
				noop () ;
			}) ;
		}

		void close () override {
			mModule = UniqueRef<HMODULE> () ;
		}

		FLAG link (CREF<String<STR>> name) override {
			assume (mModule.exist ()) ;
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
			if ifswitch (TRUE) {
				if (mErrorBuffer.size () > 0)
					discard ;
				mErrorBuffer = String<STR>::make () ;
			}
			const auto r1x = dlerror () ;
			assume (r1x != NULL) ;
			auto &&tmp = unsafe_array ((*r1x)) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= mErrorBuffer.size ())
					break ;
				if (tmp[ix] == 0)
					break ;
				mErrorBuffer[ix] = STR (tmp[ix]) ;
				ix++ ;
			}
			mErrorBuffer[ix] = 0 ;
		}
	} ;
} ;

template <>
exports auto MODULE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename MODULE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;