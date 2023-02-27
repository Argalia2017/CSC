#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#include "csc_filesystem.hpp"

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "csc_end.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include "csc_begin.h"

namespace api {
using ::open ;
using ::close ;
using ::read ;
using ::write ;
} ;

namespace CSC {
template <class DEPEND>
trait FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;
	using RETRY_TIMES = typename FILE_HELP<DEPEND ,ALWAYS>::RETRY_TIMES ;

	using HFILE = csc_int32_t ;
	using STAT_INFO = DEF<struct stat> ;

	class ImplHolder implement Holder {
	protected:
		String<STRA> mFile ;

	public:
		void initialize (CREF<String<STR>> file) override {
			mFile = string_cvt[TYPEAS<STRA ,STR>::expr] (file) ;
			if ifswitch (TRUE) {
				const auto r1x = mFile.length () ;
				assume (r1x > 0) ;
				INDEX ix = r1x - 1 ;
				if (mFile[ix] != STRA ('\\'))
					if (mFile[ix] != STRA ('/'))
						discard ;
				assume (mFile[ix] != STRA ('\\')) ;
				assume (mFile[ix] != STR ('/')) ;
			}
		}

		VarBuffer<BYTE> load () const override {
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				me = api::open ((&mFile[0]) ,O_RDONLY) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			const auto r2x = file_size (r1x) ;
			assume (r2x <= VAL32_MAX) ;
			const auto r3x = LENGTH (r2x) ;
			VarBuffer<BYTE> ret = VarBuffer<BYTE> (r3x) ;
			auto rax = r3x ;
			for (auto &&i : iter (0 ,RETRY_TIMES::expr)) {
				noop (i) ;
				if (rax == 0)
					break ;
				const auto r4x = LENGTH (api::read (r1x ,(&ret[r3x - rax]) ,VAL32 (rax))) ;
				assume (r4x > 0) ;
				rax -= r4x ;
			}
			assume (rax == 0) ;
			return move (ret) ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const override {
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				me = api::open ((&mFile[0]) ,O_RDONLY) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			const auto r2x = file_size (r1x) ;
			assume (r2x <= VAL32_MAX) ;
			const auto r3x = MathProc::min_of (LENGTH (r2x) ,item.size ()) ;
			auto rax = r3x ;
			for (auto &&i : iter (0 ,RETRY_TIMES::expr)) {
				noop (i) ;
				if (rax == 0)
					break ;
				const auto r4x = LENGTH (api::read (r1x ,(&item[r3x - rax]) ,VAL32 (rax))) ;
				assume (r4x > 0) ;
				rax -= r4x ;
			}
			assume (rax == 0) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const override {
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
				const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r2x ,r3x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			const auto r4x = item.size () ;
			auto rax = r4x ;
			for (auto &&i : iter (0 ,RETRY_TIMES::expr)) {
				noop (i) ;
				if (rax == 0)
					break ;
				const auto r5x = LENGTH (api::write (r1x ,(&item[r4x - rax]) ,VAL32 (rax))) ;
				assume (r5x > 0) ;
				rax -= r5x ;
			}
			assume (rax == 0) ;
		}

		CRef<RegBuffer<BYTE>> load_asset () const override {
			const auto r1x = dlopen (NULL ,RTLD_LOCAL) ;
			assume (r1x != NULL) ;
			const auto r2x = PrintString<STRA>::make (mFile ,slice ("input_txt_start")) ;
			const auto r3x = PrintString<STRA>::make (mFile ,slice ("input_txt_end")) ;
			const auto r4x = FLAG (dlsym (r1x ,(&r2x[0]))) ;
			assume (r4x != ZERO) ;
			const auto r5x = FLAG (dlsym (r1x ,(&r3x[0]))) ;
			assume (r5x != ZERO) ;
			const auto r6x = r5x - r4x ;
			assume (r6x >= 0) ;
			return RegBuffer<BYTE>::from (r4x ,0 ,r6x).borrow ().as_cref () ;
		}

		BOOL available () const override {
			auto rax = STAT_INFO () ;
			zeroize (rax) ;
			const auto r1x = stat ((&mFile[0]) ,(&rax)) ;
			if (r1x != 0)
				return FALSE ;
			if (S_ISDIR (rax.st_mode))
				return FALSE ;
			return TRUE ;
		}

		VAL64 length () const override {
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				me = api::open ((&mFile[0]) ,O_RDONLY) ;
			} ,[] (VREF<HFILE> me) {
				if (me == NONE)
					return ;
				api::close (me) ;
			}) ;
			if (r1x.self == NONE)
				return 0 ;
			return file_size (r1x) ;
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

		void erase () const override {
			if ifnot (available ())
				return ;
			unlink ((&mFile[0])) ;
		}

		void copy_from (CREF<Holder> that) const override {
			return copy_from (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		void copy_from (CREF<ImplHolder> that) const {
			assume (ifnot (that.available ())) ;
			const auto r1x = that.load () ;
			save (RegBuffer<BYTE>::from (r1x)) ;
		}

		void move_from (CREF<Holder> that) const override {
			return move_from (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		void move_from (CREF<ImplHolder> that) const {
			const auto r1x = available () ;
			assume (ifnot (r1x)) ;
			const auto r2x = rename ((&that.mFile[0]) ,(&mFile[0])) ;
			noop (r2x) ;
		}

		void link_from (CREF<Holder> that) const override {
			return link_from (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		void link_from (CREF<ImplHolder> that) const {
			const auto r1x = available () ;
			assume (ifnot (r1x)) ;
			const auto r2x = link ((&that.mFile[0]) ,(&mFile[0])) ;
			noop (r2x) ;
		}

		BOOL identical (CREF<Holder> that) const override {
			return identical (keep[TYPEAS<CREF<ImplHolder>>::expr] (that)) ;
		}

		BOOL identical (CREF<ImplHolder> that) const {
			auto rax = ARRAY2<STAT_INFO> () ;
			zeroize (rax[0]) ;
			const auto r1x = stat ((&mFile[0]) ,(&rax[0])) ;
			if (r1x != 0)
				return FALSE ;
			if (rax[0].st_nlink == 0)
				return FALSE ;
			zeroize (rax[1]) ;
			const auto r2x = stat ((&that.mFile[0]) ,(&rax[1])) ;
			if (r2x != 0)
				return FALSE ;
			if (rax[1].st_nlink == 0)
				return FALSE ;
			if (rax[0].st_dev != rax[1].st_dev)
				return FALSE ;
			if (rax[0].st_ino != rax[1].st_ino)
				return FALSE ;
			return TRUE ;
		}
	} ;
} ;

template <>
exports auto FILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class...>
trait FUNCTION_decouple_path_HELP ;

template <class DEPEND>
trait FUNCTION_decouple_path_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TEXTATTRIBUTE_PUREHOLDER_HELP<STR ,ALWAYS>::PureHolder ;

	class Wrapper implement Holder {
	public:
		BOOL is_gap (CREF<STR> str) const override {
			return is_gap_space (str) ;
		}

		BOOL is_gap_space (CREF<STR> str) const override {
			if (str == STR ('\\'))
				return TRUE ;
			if (str == STR ('/'))
				return TRUE ;
			return FALSE ;
		}

		BOOL is_gap_endline (CREF<STR> str) const override {
			return FALSE ;
		}
	} ;

	struct FUNCTION_decouple_path {
		inline ArrayList<String<STR>> operator() (CREF<String<STR>> dire) const {
			ArrayList<String<STR>> ret ;
			auto rax = TextReader<STR> (dire.raw ().borrow ()) ;
			rax.attribute ().derive (TYPEAS<Wrapper>::expr) ;
			const auto r1x = rax.attribute () ;
			INDEX ix = ret.insert () ;
			auto rbx = STR () ;
			if ifswitch (TRUE) {
				rax.backup () ;
				rax >> rbx ;
				if (r1x.is_gap (rbx))
					discard ;
				rax.recover () ;
			}
			while (TRUE) {
				rax >> ret[ix] ;
				if (ret[ix].empty ())
					break ;
				ix = ret.insert () ;
				rax >> rbx ;
				if (rbx == r1x.ending_item ())
					break ;
				assume (r1x.is_gap (rbx)) ;
			}
			rax >> EOS ;
			ret.remove (ix) ;
			ret.remap () ;
			return move (ret) ;
		}
	} ;
} ;

template <class DEPEND>
trait DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Holder ;
	using CHILD = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::CHILD ;
	using CHILD_MAX_SIZE = ENUMAS<VAL ,65536> ;

	using HDIRE = DEF<DIR *> ;
	using STAT_INFO = DEF<struct stat> ;

	struct HEAP {
		String<STR> mDire ;
		String<STRA> mDireA ;
		ArrayList<String<STR>> mPath ;
		BOOL mPathCached ;
		ArrayList<CHILD> mChild ;
		BOOL mChildCached ;
	} ;

	class ImplHolder implement Holder {
	protected:
		SharedRef<HEAP> mHeap ;

	public:
		void initialize () {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mPathCached = FALSE ;
			mHeap->mChildCached = FALSE ;
		}

		void initialize (CREF<String<STR>> dire) override {
			mHeap = SharedRef<HEAP>::make () ;
			mHeap->mPathCached = FALSE ;
			mHeap->mChildCached = FALSE ;
			mHeap->mDire = move (dire) ;
			if ifswitch (TRUE) {
				const auto r1x = mHeap->mDire.length () ;
				assume (r1x > 0) ;
				INDEX ix = r1x - 1 ;
				if (mHeap->mDire[ix] != STR ('\\'))
					if (mHeap->mDire[ix] != STR ('/'))
						discard ;
				mHeap->mDire += slice (".") ;
			}
			mHeap->mDireA = string_cvt[TYPEAS<STRA ,STR>::expr] (mHeap->mDire) ;
		}

		String<STR> path () const override {
			update_path () ;
			const auto r1x = mHeap->mDire.length () ;
			const auto r2x = BufferProc<STR>::buf_find_r (mHeap->mDire.raw () ,STR ('\\') ,0 ,r1x) ;
			const auto r3x = BufferProc<STR>::buf_find_r (mHeap->mDire.raw () ,STR ('/') ,0 ,r1x) ;
			const auto r4x = MathProc::max_of (r2x ,r3x) + 1 ;
			if (r4x == 0)
				return String<STR> (slice ("/")) ;
			return mHeap->mDire.segment (0 ,r4x) ;
		}

		String<STR> name () const override {
			update_path () ;
			const auto r1x = mHeap->mDire.length () ;
			const auto r2x = BufferProc<STR>::buf_find_r (mHeap->mDire.raw () ,STR ('\\') ,0 ,r1x) ;
			const auto r3x = BufferProc<STR>::buf_find_r (mHeap->mDire.raw () ,STR ('/') ,0 ,r1x) ;
			const auto r4x = MathProc::max_of (r2x ,r3x) + 1 ;
			return mHeap->mDire.segment (r4x ,r1x) ;
		}

		LENGTH depth () const override {
			update_path () ;
			return mHeap->mPath.length () ;
		}

		String<STR> absolute () const override {
			update_path () ;
			String<STR> ret = PrintString<STR>::make () ;
			if ifswitch (TRUE) {
				if (mHeap->mDire.length () < 1)
					discard ;
				if (mHeap->mDire[0] != STR ('\\'))
					if (mHeap->mDire[0] != STR ('/'))
						discard ;
				ret += slice ("/") ;
			}
			for (auto &&i : mHeap->mPath.iter ()) {
				ret += mHeap->mPath[i] ;
				ret += slice ("/") ;
			}
			if ifswitch (TRUE) {
				const auto r1x = ret.length () ;
				if (r1x < 1)
					discard ;
				ret[r1x - 1] = 0 ;
			}
			return move (ret) ;
		}

		Array<CHILD> load () const override {
			update_child () ;
			Array<CHILD> ret = Array<CHILD> (mHeap->mChild.length ()) ;
			INDEX ix = 0 ;
			for (auto &&i : mHeap->mChild) {
				ret[ix] = i ;
				ix++ ;
			}
			return move (ret) ;
		}

		void fresh () const override {
			mHeap->mPath = ArrayList<String<STR>> () ;
			mHeap->mPathCached = FALSE ;
			mHeap->mChild = ArrayList<CHILD> () ;
			mHeap->mChildCached = FALSE ;
		}

		BOOL available () const override {
			auto rax = STAT_INFO () ;
			zeroize (rax) ;
			const auto r1x = stat ((&mHeap->mDireA[0]) ,(&rax)) ;
			if (r1x != 0)
				return FALSE ;
			if ifnot (S_ISDIR (rax.st_mode))
				return FALSE ;
			return TRUE ;
		}

		BOOL lock () const override {
			const auto r1x = PrintString<STR>::make (mHeap->mDire ,STR ('\\') ,slice (".lockdirectory")) ;
			const auto r2x = CurrentProcess::make () ;
			if ifswitch (TRUE) {
				const auto r3x = File (r1x) ;
				if ifnot (r3x.available ())
					discard ;
				const auto r4x = r3x.load ().as_cref () ;
				const auto r5x = Process (r4x) ;
				const auto r6x = Process (r5x.process_uid ()) ;
				if (r5x.snapshot () != r6x.snapshot ())
					discard ;
				return r5x.snapshot () == r2x.snapshot () ;
			}
			const auto r7x = r2x.snapshot () ;
			const auto r8x = lock_handle (r1x ,r7x) ;
			return r8x->self.available () ;
		}

		CRef<UniqueRef<File>> lock_handle (CREF<String<STR>> file ,CREF<ConBuffer<BYTE>> snapshot_) const {
			return CRef<UniqueRef<File>>::reference (memorize ([&] () {
				return UniqueRef<File> ([&] (VREF<File> me) {
					me = File (file) ;
					me.save (snapshot_) ;
				} ,[] (VREF<File> me) {
					me.erase () ;
				}) ;
			})) ;
		}

		void build () const override {
			update_path () ;
			auto rax = PrintString<STR>::make () ;
			if ifswitch (TRUE) {
				if (mHeap->mDire.length () < 1)
					discard ;
				if (mHeap->mDire[0] != STR ('\\'))
					if (mHeap->mDire[0] != STR ('/'))
						discard ;
				rax += slice ("/") ;
			}
			for (auto &&i : mHeap->mPath.iter ()) {
				rax += mHeap->mPath[i] ;
				if ifswitch (TRUE) {
					const auto r1x = mHeap->mPath[i].length () ;
					if (r1x == 0)
						discard ;
					if (mHeap->mPath[i][r1x - 1] == STR (':'))
						discard ;
					const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
					mkdir ((&rax[0]) ,r2x) ;
				}
				rax += slice ("/") ;
			}
		}

		void erase () const override {
			if ifswitch (TRUE) {
				rmdir ((&mHeap->mDireA[0])) ;
				if ifnot (available ())
					discard ;
				unlink ((&mHeap->mDireA[0])) ;
			}
			fresh () ;
		}

		void clear () const override {
			auto rax = Deque<String<STR>> (CHILD_MAX_SIZE::expr) ;
			auto rbx = ImplHolder () ;
			rbx.initialize () ;
			rax.add (mHeap->mDire) ;
			while (TRUE) {
				if (rax.empty ())
					break ;
				rax.take (rbx.mHeap->mDire) ;
				rbx.fresh () ;
				rbx.update_child () ;
				for (auto &&i : rbx.mHeap->mChild) {
					if ifnot (i.mIsFile)
						continue ;
					const auto r1x = File (i.mFile) ;
					r1x.erase () ;
				}
				for (auto &&i : rbx.mHeap->mChild) {
					if ifnot (i.mIsDire)
						continue ;
					if ifnot (i.mIsLink)
						continue ;
					const auto r2x = Directory (i.mFile) ;
					r2x.erase () ;
				}
				for (auto &&i : rbx.mHeap->mChild) {
					if ifnot (i.mIsDire)
						continue ;
					if (i.mIsLink)
						continue ;
					rax.add (i.mFile) ;
				}
			}
			fresh () ;
		}

		void update_path () const {
			using R1X = typename FUNCTION_decouple_path_HELP<DEPEND ,ALWAYS>::FUNCTION_decouple_path ;
			if (mHeap->mPathCached)
				return ;
			const auto r1x = R1X () ;
			auto rax = r1x (mHeap->mDire) ;
			fix_working_path (rax) ;
			fix_relative_path (rax) ;
			mHeap->mPath = move (rax) ;
			mHeap->mPathCached = TRUE ;
		}

		void fix_working_path (VREF<ArrayList<String<STR>>> path_) const {
			using R1X = typename FUNCTION_decouple_path_HELP<DEPEND ,ALWAYS>::FUNCTION_decouple_path ;
			const auto r1x = R1X () ;
			if (path_.length () < 0)
				return ;
			if (path_[0] != slice ("."))
				if (path_[0] != slice (".."))
					return ;
			const auto r2x = RuntimeProc::working_path () ;
			auto rax = r1x (r2x) ;
			for (auto &&i : path_)
				rax.add (move (i)) ;
			path_ = move (rax) ;
			path_.remap () ;
		}

		void fix_relative_path (VREF<ArrayList<String<STR>>> path_) const {
			const auto r1x = invoke ([&] () {
				Deque<INDEX> ret = Deque<INDEX> (path_.size ()) ;
				for (auto &&i : path_.iter ()) {
					if (path_[i] == slice ("."))
						continue ;
					auto act = TRUE ;
					if ifswitch (act) {
						if (ret.empty ())
							discard ;
						if ifnot (path_[i] == slice (".."))
							discard ;
						ret.pop () ;
					}
					if ifswitch (act) {
						ret.add (i) ;
					}
				}
				return move (ret) ;
			}) ;
			if (r1x.length () >= path_.length ())
				return ;
			const auto r2x = invoke ([&] () {
				BitSet<> ret = BitSet<> (path_.length ()) ;
				ret.fill (BYTE (0XFF)) ;
				for (auto &&i : r1x)
					ret[i] = FALSE ;
				return move (ret) ;
			}) ;
			for (auto &&i : r2x)
				path_.remove (i) ;
			path_.remap () ;
		}

		void update_child () const {
			if (mHeap->mChildCached)
				return ;
			auto rax = ArrayList<CHILD> (CHILD_MAX_SIZE::expr) ;
			auto rbx = PrintString<STR>::make () ;
			rbx += mHeap->mDire ;
			rbx += slice ("\\") ;
			const auto r1x = rbx.length () ;
			const auto r2x = UniqueRef<HDIRE> ([&] (VREF<HDIRE> me) {
				me = opendir ((&mHeap->mDireA[0])) ;
				assume (me != NULL) ;
			} ,[] (VREF<HDIRE> me) {
				closedir (me) ;
			}) ;
			rbx[r1x] = 0 ;
			auto rdx = PrintString<STR>::make () ;
			while (TRUE) {
				const auto r3x = readdir (r2x.self) ;
				if (r3x == NULL)
					break ;
				if ifswitch (TRUE) {
					rdx -= BufferProc<STR>::buf_slice (unsafe_array (r3x->d_name[0]) ,rdx.size ()) ;
					if (rdx == slice ("."))
						discard ;
					if (rdx == slice (".."))
						discard ;
					rbx += rdx ;
					const auto r4x = BitProc::bit_any (CHAR (r3x->d_type) ,DT_DIR) ;
					const auto r5x = BitProc::bit_any (CHAR (r3x->d_type) ,DT_LNK) ;
					assume (rax.length () < rax.size ()) ;
					INDEX ix = rax.insert () ;
					rax[ix].mFile = rbx ;
					rax[ix].mIsFile = ifnot (r4x) ;
					rax[ix].mIsDire = r4x ;
					rax[ix].mIsLink = r5x ;
				}
				rbx[r1x] = 0 ;
			}
			rax.remap () ;
			mHeap->mChild = move (rax) ;
			mHeap->mChildCached = TRUE ;
		}
	} ;
} ;

template <>
exports auto DIRECTORY_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait STREAMFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename STREAMFILE_HELP<DEPEND ,ALWAYS>::Holder ;

	using HFILE = csc_int32_t ;

	class ImplHolder implement Holder {
	protected:
		String<STRA> mFile ;
		UniqueRef<HFILE> mReadPipe ;
		UniqueRef<HFILE> mWritePipe ;
		VAL64 mRead ;
		VAL64 mWrite ;

	public:
		void initialize (CREF<String<STR>> file) override {
			mFile = string_cvt[TYPEAS<STRA ,STR>::expr] (file) ;
			mRead = 0 ;
			mWrite = 0 ;
		}

		void open_r () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,O_RDONLY ,r1x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mRead = 0 ;
		}

		void open_w () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
				const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r1x ,r2x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mWrite = 0 ;
		}

		void open_a () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
				const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r1x ,r2x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
				const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r3x ,r4x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mRead = 0 ;
			mWrite = 0 ;
			if ifswitch (TRUE) {
				const auto r5x = VAL64 (lseek64 (mWritePipe ,0 ,SEEK_END)) ;
				if (r5x <= 0)
					discard ;
				mWrite += r5x ;
			}
		}

		void close () override {
			mReadPipe = UniqueRef<HFILE> () ;
			mWritePipe = UniqueRef<HFILE> () ;
		}

		void open (CREF<BOOL> readable ,CREF<BOOL> writable) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (readable)
					discard ;
				if (writable)
					discard ;
				open_r () ;
			}
			if ifswitch (act) {
				if (readable)
					discard ;
				if ifnot (writable)
					discard ;
				open_w () ;
			}
			if ifswitch (act) {
				if ifnot (readable)
					discard ;
				if ifnot (writable)
					discard ;
				try_invoke ([&] () {
					open_r () ;
				} ,[&] () {
					open_w () ;
				} ,[&] () {
					noop () ;
				}) ;
				close () ;
				open_a () ;
			}
		}

		LENGTH read (VREF<RegBuffer<BYTE>> item) override {
			assert (mReadPipe.exist ()) ;
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = item.size () ;
			auto rax = r1x ;
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				const auto r2x = LENGTH (api::read (mReadPipe ,(&item[r1x - rax]) ,VAL32 (rax))) ;
				assume (r2x > 0) ;
				rax -= r2x ;
			}
			mRead += r1x ;
			return r1x - rax ;
		}

		LENGTH read (VREF<RegBuffer<WORD>> item) override {
			using R1X = SIZE_OF<WORD> ;
			if (item.size () == 0)
				return 0 ;
			const auto r1x = address (item[0]) ;
			LENGTH ret = read (RegBuffer<BYTE>::from (r1x ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_launder (item) ;
			return move (ret) ;
		}

		LENGTH read (VREF<RegBuffer<CHAR>> item) override {
			using R1X = SIZE_OF<CHAR> ;
			if (item.size () == 0)
				return 0 ;
			const auto r1x = address (item[0]) ;
			LENGTH ret = read (RegBuffer<BYTE>::from (r1x ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_launder (item) ;
			return move (ret) ;
		}

		LENGTH read (VREF<RegBuffer<DATA>> item) override {
			using R1X = SIZE_OF<DATA> ;
			if (item.size () == 0)
				return 0 ;
			const auto r1x = address (item[0]) ;
			LENGTH ret = read (RegBuffer<BYTE>::from (r1x ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_launder (item) ;
			return move (ret) ;
		}

		LENGTH write (CREF<RegBuffer<BYTE>> item) override {
			assert (mWritePipe.exist ()) ;
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = item.size () ;
			auto rax = r1x ;
			if ifswitch (TRUE) {
				if (rax == 0)
					break ;
				const auto r2x = LENGTH (api::write (mWritePipe ,(&item[r1x - rax]) ,VAL32 (rax))) ;
				assume (r2x > 0) ;
				rax -= r2x ;
			}
			mWrite += r1x ;
			return r1x - rax ;
		}

		LENGTH write (CREF<RegBuffer<WORD>> item) override {
			using R1X = SIZE_OF<WORD> ;
			if (item.size () == 0)
				return 0 ;
			const auto r1x = address (item[0]) ;
			LENGTH ret = write (RegBuffer<BYTE>::from (r1x ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			return move (ret) ;
		}

		LENGTH write (CREF<RegBuffer<CHAR>> item) override {
			using R1X = SIZE_OF<CHAR> ;
			if (item.size () == 0)
				return 0 ;
			const auto r1x = address (item[0]) ;
			LENGTH ret = write (RegBuffer<BYTE>::from (r1x ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			return move (ret) ;
		}

		LENGTH write (CREF<RegBuffer<DATA>> item) override {
			using R1X = SIZE_OF<DATA> ;
			if (item.size () == 0)
				return 0 ;
			const auto r1x = address (item[0]) ;
			LENGTH ret = write (RegBuffer<BYTE>::from (r1x ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			return move (ret) ;
		}

		void flush () override {
			if ifnot (mWritePipe.exist ())
				return ;
			syncfs (mWritePipe) ;
		}
	} ;
} ;

template <>
exports auto STREAMFILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename STREAMFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BUFFERFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using CHUNK_SIZE = ENUMAS<VAL ,4194304> ;
	using HEADER_SIZE = ENUMAS<VAL ,65536> ;

	using HFILE = csc_int32_t ;
	using HANDLE = csc_pointer_t ;

	struct CHUNK {
		VAL64 mOffset ;
		VAL64 mCacheTime ;
		UniqueRef<Tuple<HANDLE ,LENGTH>> mBuffer ;
	} ;

	struct HEADER {
		VAL64 mFileSize ;
		VAL64 mFileEndian ;
		VAL64 mItemSize ;
		VAL64 mItemAlign ;
		VAL64 mItemCount ;
		VAL64 mChunkSize ;
		VAL64 mChunkCapacity ;
		VAL64 mChunkCount ;
	} ;

	class ImplHolder implement Holder {
	protected:
		String<STRA> mFile ;
		Clazz mItemClazz ;
		UniqueRef<HFILE> mPipe ;
		UniqueRef<> mMapping ;
		VAL64 mFileSize ;
		csc_enum_t mFileMapFlag ;
		VRef<HEADER> mHeader ;
		Set<VAL64> mCacheSet ;
		List<CHUNK> mCacheList ;
		VAL64 mCacheTimer ;

	public:
		void initialize (CREF<String<STR>> file ,CREF<Clazz> clazz) override {
			mFile = string_cvt[TYPEAS<STRA ,STR>::expr] (file) ;
			mItemClazz = clazz ;
			mFileSize = 0 ;
			mFileMapFlag = 0 ;
			set_cache_size (1) ;
		}

		void set_cache_size (CREF<LENGTH> size_) override {
			mCacheSet = Set<VAL64> (size_) ;
			mCacheList = List<CHUNK> (size_) ;
			mCacheTimer = 0 ;
		}

		void open_r () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,O_RDONLY ,r1x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mFileSize = file_size (mPipe) ;
			mFileMapFlag = csc_enum_t (PROT_READ) ;
			read_header () ;
		}

		void open_w () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
				const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r1x ,r2x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mFileSize = HEADER_SIZE::expr ;
			mMapping = UniqueRef<> ([&] () {
				const auto r1x = ftruncate64 (mPipe ,mFileSize) ;
				assume (r1x == 0) ;
			} ,[] () {
				noop () ;
			}) ;
			mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
			write_header () ;
		}

		void open_a () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			assume (mHeader != NULL) ;
			mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
				const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r1x ,r2x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mFileSize = mHeader->mFileSize ;
			mMapping = UniqueRef<> ([&] () {
				const auto r1x = ftruncate64 (mPipe ,mFileSize) ;
				assume (r1x == 0) ;
			} ,[] () {
				noop () ;
			}) ;
			mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
			write_header () ;
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
			mHeader = VRef<HEADER>::make () ;
			INDEX ix = load (0 ,HEADER_SIZE::expr) ;
			const auto r1x = address (mCacheList[ix].mBuffer->m1st) ;
			auto rax = ByteReader (RegBuffer<BYTE>::from (r1x ,0 ,HEADER_SIZE::expr).borrow ()) ;
			rax >> GAP ;
			rax >> GAP ;
			rax >> slice ("CSC_BufferFile") ;
			rax >> GAP ;
			rax >> mHeader->mFileSize ;
			assume (mHeader->mFileSize == mFileSize) ;
			rax >> GAP ;
			rax >> mHeader->mFileEndian ;
			const auto r2x = file_endian () ;
			assume (mHeader->mFileEndian == r2x) ;
			rax >> GAP ;
			rax >> mHeader->mItemSize ;
			assume (mHeader->mItemSize == mItemClazz.type_size ()) ;
			rax >> GAP ;
			rax >> mHeader->mItemAlign ;
			assume (mHeader->mItemAlign == mItemClazz.type_align ()) ;
			rax >> GAP ;
			rax >> mHeader->mItemCount ;
			assume (mHeader->mItemCount >= 0) ;
			rax >> GAP ;
			rax >> mHeader->mChunkSize ;
			assume (mHeader->mChunkSize == CHUNK_SIZE::expr) ;
			rax >> GAP ;
			rax >> mHeader->mChunkCapacity ;
			const auto r3x = mHeader->mChunkSize / mHeader->mItemSize ;
			assume (mHeader->mChunkCapacity == r3x) ;
			rax >> GAP ;
			rax >> mHeader->mChunkCount ;
			assume (mHeader->mChunkCount >= 0) ;
			rax >> GAP ;
			rax >> GAP ;
		}

		void write_header () {
			if ifswitch (TRUE) {
				if (mHeader != NULL)
					discard ;
				mHeader = VRef<HEADER>::make () ;
				mHeader->mFileSize = mFileSize ;
				const auto r1x = file_endian () ;
				mHeader->mFileEndian = r1x ;
				mHeader->mItemSize = mItemClazz.type_size () ;
				mHeader->mItemAlign = mItemClazz.type_align () ;
				mHeader->mItemCount = 0 ;
				mHeader->mChunkSize = CHUNK_SIZE::expr ;
				assume (mHeader->mChunkSize >= mHeader->mItemSize) ;
				mHeader->mChunkCapacity = mHeader->mChunkSize / mHeader->mItemSize ;
				mHeader->mChunkCount = 0 ;
			}
			INDEX ix = load (0 ,HEADER_SIZE::expr) ;
			const auto r2x = address (mCacheList[ix].mBuffer->m1st) ;
			auto rax = ByteWriter (RegBuffer<BYTE>::from (r2x ,0 ,HEADER_SIZE::expr).borrow ()) ;
			rax << GAP ;
			rax << GAP ;
			rax << slice ("CSC_BufferFile") ;
			rax << GAP ;
			rax << mHeader->mFileSize ;
			rax << GAP ;
			rax << mHeader->mFileEndian ;
			rax << GAP ;
			rax << mHeader->mItemSize ;
			rax << GAP ;
			rax << mHeader->mItemAlign ;
			rax << GAP ;
			rax << mHeader->mItemCount ;
			rax << GAP ;
			rax << mHeader->mChunkSize ;
			rax << GAP ;
			rax << mHeader->mChunkCapacity ;
			rax << GAP ;
			rax << mHeader->mChunkCount ;
			rax << GAP ;
			rax << GAP ;
			flush () ;
		}

		VAL64 file_endian () const {
			const auto r1x = invoke ([&] () {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> (0) ;
				rax[0] = BYTE (0X00) ;
				rax[1] = BYTE (0X01) ;
				rax[2] = BYTE (0X02) ;
				rax[3] = BYTE (0X03) ;
				return bitwise[TYPEAS<CHAR>::expr] (rax) ;
			}) ;
			return VAL64 (r1x) ;
		}

		void close () override {
			flush () ;
			mMapping = UniqueRef<> () ;
			mPipe = UniqueRef<HFILE> () ;
		}

		void open (CREF<BOOL> readable ,CREF<BOOL> writable) override {
			auto act = TRUE ;
			if ifswitch (act) {
				if ifnot (readable)
					discard ;
				if (writable)
					discard ;
				open_r () ;
			}
			if ifswitch (act) {
				if (readable)
					discard ;
				if ifnot (writable)
					discard ;
				open_w () ;
			}
			if ifswitch (act) {
				if ifnot (readable)
					discard ;
				if ifnot (writable)
					discard ;
				try_invoke ([&] () {
					open_r () ;
				} ,[&] () {
					open_w () ;
				} ,[&] () {
					noop () ;
				}) ;
				close () ;
				open_a () ;
			}
		}

		VAL64 length () const override {
			if (mHeader == NULL)
				return 0 ;
			return mHeader->mItemCount ;
		}

		void resize (CREF<VAL64> size_) override {
			assert (mPipe.exist ()) ;
			const auto r1x = mHeader->mItemCount ;
			const auto r2x = valign (r1x + size_ ,mHeader->mChunkCapacity) / mHeader->mChunkCapacity ;
			const auto r3x = MathProc::max_of (r2x - mHeader->mChunkCount ,VAL64 (0)) ;
			mHeader->mItemCount += size_ ;
			mHeader->mChunkCount += r3x ;
			mHeader->mFileSize = HEADER_SIZE::expr + mHeader->mChunkCount * mHeader->mChunkSize ;
			if ifswitch (TRUE) {
				if (r3x == 0)
					discard ;
				close () ;
				open_a () ;
			}
		}

		void get (CREF<VAL64> index ,VREF<RegBuffer<BYTE>> item) override {
			assert (mPipe.exist ()) ;
			assume (vbetween (index ,0 ,mHeader->mItemCount)) ;
			assume (item.size () == mHeader->mItemSize) ;
			const auto r1x = index / mHeader->mChunkCapacity ;
			const auto r2x = index % mHeader->mChunkCapacity * mHeader->mItemSize ;
			const auto r3x = r2x + mHeader->mItemSize ;
			const auto r4x = HEADER_SIZE::expr + r1x * mHeader->mChunkSize ;
			INDEX ix = load (r4x ,mHeader->mChunkSize) ;
			const auto r5x = address (mCacheList[ix].mBuffer->m1st) ;
			BufferProc<BYTE>::buf_copy (item ,RegBuffer<BYTE>::from (r5x ,r2x ,r3x) ,0 ,mHeader->mItemSize) ;
		}

		void set (CREF<VAL64> index ,CREF<RegBuffer<BYTE>> item) override {
			assert (mPipe.exist ()) ;
			assume (vbetween (index ,0 ,mHeader->mItemCount)) ;
			assume (item.size () == mHeader->mItemSize) ;
			const auto r1x = index / mHeader->mChunkCapacity ;
			const auto r2x = index % mHeader->mChunkCapacity * mHeader->mItemSize ;
			const auto r3x = r2x + mHeader->mItemSize ;
			const auto r4x = HEADER_SIZE::expr + r1x * mHeader->mChunkSize ;
			INDEX ix = load (r4x ,mHeader->mChunkSize) ;
			const auto r5x = address (mCacheList[ix].mBuffer->m1st) ;
			BufferProc<BYTE>::buf_copy (RegBuffer<BYTE>::from (r5x ,r2x ,r3x) ,item ,0 ,mHeader->mItemSize) ;
		}

		INDEX load (CREF<VAL64> offset ,CREF<LENGTH> size_) {
			INDEX ret = mCacheSet.map (offset) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				update_overflow () ;
				ret = mCacheList.insert () ;
				mCacheSet.add (offset ,ret) ;
				mCacheList[ret].mOffset = offset ;
				mCacheList[ret].mBuffer = UniqueRef<Tuple<HANDLE ,LENGTH>> ([&] (VREF<Tuple<HANDLE ,LENGTH>> me) {
					const auto r2x = mmap64 (NULL ,size_ ,mFileMapFlag ,MAP_SHARED ,mPipe ,offset) ;
					assume (r2x != MAP_FAILED) ;
					me.m1st = r2x ;
					me.m2nd = size_ ;
				} ,[] (VREF<Tuple<HANDLE ,LENGTH>> me) {
					msync (me.m1st ,me.m2nd ,MS_SYNC) ;
					munmap (me.m1st ,me.m2nd) ;
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
				for (auto &&i : mCacheList.iter ()) {
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

		void flush () override {
			if ifnot (mPipe.exist ())
				return ;
			mCacheSet.clear () ;
			mCacheList.clear () ;
			mCacheTimer = 0 ;
			fsync (mPipe) ;
		}
	} ;
} ;

template <>
exports auto BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BUFFERFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;