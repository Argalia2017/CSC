#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_LINUX__
#error "∑(っ°Д° ;)っ : bad include"
#endif

#include "begin.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include "end.h"

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
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file) override {
			mFile = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (file) ;
			if ifswitch (TRUE) {
				const auto r1x = mFile.length () ;
				assume (r1x > 0) ;
				INDEX ix = r1x - 1 ;
				if (mFile[ix] != STRA ('\\'))
					if (mFile[ix] != STRA ('/'))
						discard ;
				assume (mFile[ix] != STRA ('\\')) ;
				assume (mFile[ix] != STRA ('\\')) ;
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
				const auto r4x = LENGTH (api::read (r1x ,(&ret.self[r3x - rax]) ,VAL32 (rax))) ;
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
				const auto r4x = LENGTH (api::read (r1x ,(&item.self[r3x - rax]) ,VAL32 (rax))) ;
				assume (r4x > 0) ;
				rax -= r4x ;
			}
			assume (rax == 0) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const override {
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r2x = VAL32 (O_CREAT | O_WRONLY | O_TRUNC) ;
				const auto r3x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
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
				const auto r5x = LENGTH (api::write (r1x ,(&item.self[r4x - rax]) ,VAL32 (rax))) ;
				assume (r5x > 0) ;
				rax -= r5x ;
			}
			assume (rax == 0) ;
		}

		CRef<RegBuffer<BYTE>> load_asset () const override {
			unimplemented () ;
			return bad (TYPEAS<CRef<RegBuffer<BYTE>>>::expr) ;
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
				return ZERO ;
			return file_size (r1x) ;
		}

		VAL64 file_size (CREF<HFILE> handle) const {
			const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
			const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
			//@warn: file in '/proc' is zero size
			if (r1x == NONE)
				return ZERO ;
			if (r2x == NONE)
				return ZERO ;
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
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			assume (ifnot (tmp.available ())) ;
			const auto r1x = tmp.load () ;
			save (RegBuffer<BYTE>::from (r1x)) ;
		}

		void move_from (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			const auto r1x = available () ;
			assume (ifnot (r1x)) ;
			const auto r2x = rename ((&tmp.mFile[0]) ,(&mFile[0])) ;
			noop (r2x) ;
		}

		void link_from (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			const auto r1x = available () ;
			assume (ifnot (r1x)) ;
			const auto r2x = link ((&tmp.mFile[0]) ,(&mFile[0])) ;
			noop (r2x) ;
		}

		BOOL identical (CREF<Holder> that) const override {
			auto &&tmp = keep[TYPEAS<CREF<ImplHolder>>::expr] (that) ;
			auto rax = ARRAY2<STAT_INFO> () ;
			zeroize (rax[0]) ;
			const auto r1x = stat ((&mFile[0]) ,(&rax[0])) ;
			if (r1x != 0)
				return FALSE ;
			if (rax[0].st_nlink == 0)
				return FALSE ;
			zeroize (rax[1]) ;
			const auto r2x = stat ((&tmp.mFile[0]) ,(&rax[1])) ;
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

exports auto FILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait DIRECTORY_DECOUPLE_HELP<DEPEND ,ALWAYS> {
	using Holder = typename TEXTATTRIBUTE_HELP<STR ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		CRef<Holder> mThis ;

	public:
		implicit ImplHolder () = delete ;

		explicit ImplHolder (RREF<CRef<Holder>> that) {
			mThis = move (that) ;
		}

		void initialize () override {
			noop () ;
		}

		STR ending_item () const override {
			return mThis->ending_item () ;
		}

		BOOL is_space (CREF<STR> str) const override {
			if (str == STR ('\\'))
				return TRUE ;
			if (str == STR ('/'))
				return TRUE ;
			return FALSE ;
		}

		BOOL is_endline_space (CREF<STR> str) const override {
			return FALSE ;
		}

		BOOL is_word (CREF<STR> str) const override {
			return mThis->is_word (str) ;
		}

		BOOL is_number (CREF<STR> str) const override {
			return mThis->is_number (str) ;
		}

		BOOL is_hex_number (CREF<STR> str) const override {
			return mThis->is_hex_number (str) ;
		}

		INDEX hex_from_str (CREF<STR> str) const override {
			return mThis->hex_from_str (str) ;
		}

		STR str_from_hex (CREF<INDEX> hex) const override {
			return mThis->str_from_hex (hex) ;
		}

		BOOL is_control (CREF<STR> str) const override {
			return mThis->is_control (str) ;
		}

		Optional<STR> escape_cast (CREF<STR> str) const override {
			return mThis->escape_cast (str) ;
		}

		LENGTH value_precision () const override {
			return mThis->value_precision () ;
		}

		LENGTH float_precision () const override {
			return mThis->float_precision () ;
		}

		LENGTH number_precision () const override {
			return mThis->number_precision () ;
		}
	} ;

	struct FUNCTION_decouple_path {
		inline ArrayList<String<STR>> operator() (CREF<String<STR>> dire) const {
			ArrayList<String<STR>> ret ;
			auto rax = TextReader<STR> (dire.raw ()) ;
			rax.attribute ().derive (TYPEAS<ImplHolder>::expr) ;
			const auto r1x = rax.attribute () ;
			INDEX ix = ret.insert () ;
			auto rbx = STR () ;
			if ifswitch (TRUE) {
				rax.backup () ;
				rax >> rbx ;
				if (r1x.is_space (rbx))
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
				assume (r1x.is_space (rbx)) ;
			}
			rax >> TextReader<STR>::EOS ;
			ret.remove (ix) ;
			ret.remap () ;
			return move (ret) ;
		}

		imports CRef<ImplHolder> text_attr () {
			return memorize ([&] () {
				auto rax = VRef<ImplHolder>::make () ;
				rax->initialize () ;
				return rax.as_cref () ;
			}) ;
		}
	} ;
} ;

template <class DEPEND>
trait DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Holder ;
	using CHILD = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::CHILD ;
	using CHILD_MAX_SIZE = ENUMAS<VAL ,ENUMID<65536>> ;

	using HDIRE = DEF<DIR *> ;
	using STAT_INFO = DEF<struct stat> ;

	class ImplHolder implement Holder {
	protected:
		String<STR> mDire ;
		String<STRA> mDireA ;
		Cell<CRef<ArrayList<String<STR>>>> mPath ;
		Cell<CRef<ArrayList<CHILD>>> mChild ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> dire) override {
			mDire = move (dire) ;
			mPath = Cell<CRef<ArrayList<String<STR>>>>::make () ;
			mChild = Cell<CRef<ArrayList<CHILD>>>::make () ;
			if ifswitch (TRUE) {
				const auto r1x = mDire.length () ;
				assume (r1x > 0) ;
				INDEX ix = r1x - 1 ;
				if (mDire[ix] != STR ('\\'))
					if (mDire[ix] != STR ('/'))
						discard ;
				mDire.trunc (ix) ;
			}
			mDireA = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (mDire) ;
		}

		String<STR> path () const override {
			const auto r1x = mDire.length () ;
			const auto r2x = BufferProc::buf_find (mDire ,STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = BufferProc::buf_find (mDire ,STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = MathProc::max_of (r2x ,r3x ,ZERO) ;
			return mDire.segment (0 ,r4x) ;
		}

		String<STR> name () const override {
			const auto r1x = mDire.length () ;
			const auto r2x = BufferProc::buf_find (mDire ,STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = BufferProc::buf_find (mDire ,STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = MathProc::max_of (r2x ,r3x) + 1 ;
			return mDire.segment (r4x ,r1x) ;
		}

		LENGTH depth () const override {
			update_path () ;
			const auto r1x = mPath.fetch () ;
			return r1x->length () ;
		}

		String<STR> absolute () const override {
			update_path () ;
			const auto r1x = mPath.fetch () ;
			String<STR> ret = String<STR>::make () ;
			if ifswitch (TRUE) {
				if (mDire.length () < 1)
					discard ;
				if (mDire[0] != STR ('\\'))
					if (mDire[0] != STR ('/'))
						discard ;
				ret += slice ("/") ;
			}
			for (auto &&i : r1x->iter ()) {
				ret += r1x.self[i] ;
				ret += slice ("/") ;
			}
			if ifswitch (TRUE) {
				const auto r2x = ret.length () ;
				if (r2x < 1)
					discard ;
				ret[r2x - 1] = 0 ;
			}
			return move (ret) ;
		}

		ArrayList<CHILD> load () const override {
			update_child () ;
			const auto r1x = mChild.fetch () ;
			ArrayList<CHILD> ret = ArrayList<CHILD> (r1x->length ()) ;
			for (auto &&i : r1x.self)
				ret.add (i) ;
			return move (ret) ;
		}

		void fresh () const override {
			mPath.store (NULL) ;
			mChild.store (NULL) ;
		}

		BOOL available () const override {
			auto rax = STAT_INFO () ;
			zeroize (rax) ;
			const auto r1x = stat ((&mDireA[0]) ,(&rax)) ;
			if (r1x != 0)
				return FALSE ;
			if ifnot (S_ISDIR (rax.st_mode))
				return FALSE ;
			return TRUE ;
		}

		BOOL lock () const override {
			const auto r1x = String<STR>::make (mDire ,STR ('\\') ,slice (".lockdirectory")) ;
			const auto r2x = CurrentProcess () ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				const auto r3x = File (r1x) ;
				if ifnot (r3x.available ())
					discard ;
				const auto r4x = ConBuffer<BYTE> (r3x.load ()) ;
				const auto r5x = Process (r4x) ;
				const auto r6x = Process (r5x.process_uid ()) ;
				if (r5x.snapshot () != r6x.snapshot ())
					discard ;
				return r5x.snapshot () == r2x.snapshot () ;
			}
			if ifswitch (rxx) {
				const auto r7x = r2x.snapshot () ;
				const auto r8x = lock_handle (r1x ,r7x) ;
				return r8x->self.available () ;
			}
			return FALSE ;
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
			const auto r1x = mPath.fetch () ;
			auto rax = String<STR>::make () ;
			if ifswitch (TRUE) {
				if (mDire.length () < 1)
					discard ;
				if (mDire[0] != STR ('\\'))
					if (mDire[0] != STR ('/'))
						discard ;
				rax += slice ("/") ;
			}
			for (auto &&i : r1x->iter ()) {
				rax += r1x.self[i] ;
				if ifswitch (TRUE) {
					const auto r2x = r1x.self[i].length () ;
					if (r2x == 0)
						discard ;
					if (r1x.self[i][r2x - 1] == STR (':'))
						discard ;
					const auto r3x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
					mkdir ((&rax[0]) ,r3x) ;
				}
				rax += slice ("/") ;
			}
		}

		void erase () const override {
			if ifswitch (TRUE) {
				rmdir ((&mDireA[0])) ;
				if ifnot (available ())
					discard ;
				unlink ((&mDireA[0])) ;
			}
			fresh () ;
		}

		void clear () const override {
			auto rax = Deque<String<STR>> (CHILD_MAX_SIZE::expr) ;
			auto rbx = ImplHolder () ;
			rbx.mPath = Cell<CRef<ArrayList<String<STR>>>>::make () ;
			rbx.mChild = Cell<CRef<ArrayList<CHILD>>>::make () ;
			rax.add (mDire) ;
			while (TRUE) {
				if (rax.empty ())
					break ;
				rax.take (rbx.mDire) ;
				rbx.fresh () ;
				rbx.update_child () ;
				const auto r1x = rbx.mChild.fetch () ;
				for (auto &&i : r1x.self) {
					if ifnot (i.mIsFile)
						continue ;
					const auto r2x = File (i.mFile) ;
					r2x.erase () ;
				}
				for (auto &&i : r1x.self) {
					if ifnot (i.mIsDire)
						continue ;
					if ifnot (i.mIsLink)
						continue ;
					const auto r3x = Directory (i.mFile) ;
					r3x.erase () ;
				}
				for (auto &&i : r1x.self) {
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
			using R1X = typename DIRECTORY_DECOUPLE_HELP<DEPEND ,ALWAYS>::FUNCTION_decouple_path ;
			if (mPath.fetch () != NULL)
				return ;
			const auto r1x = R1X () ;
			auto rax = r1x (mDire) ;
			if ifswitch (TRUE) {
				if (rax.length () < 0)
					discard ;
				if (rax[0] != slice ("."))
					if (rax[0] != slice (".."))
						discard ;
				const auto r2x = RuntimeProc::working_path () ;
				auto rbx = r1x (r2x) ;
				for (auto &&i : rax)
					rbx.add (move (i)) ;
				rax = move (rbx) ;
				rax.remap () ;
			}
			if ifswitch (TRUE) {
				const auto r3x = invoke ([&] () {
					Deque<INDEX> ret = Deque<INDEX> (rax.size ()) ;
					for (auto &&i : rax.iter ()) {
						if (rax[i] == slice ("."))
							continue ;
						auto rxx = TRUE ;
						if ifswitch (rxx) {
							if (ret.empty ())
								discard ;
							if ifnot (rax[i] == slice (".."))
								discard ;
							ret.pop () ;
						}
						if ifswitch (rxx) {
							ret.add (i) ;
						}
					}
					return move (ret) ;
				}) ;
				if (r3x.length () >= rax.length ())
					discard ;
				const auto r4x = invoke ([&] () {
					BitSet<> ret = BitSet<> (rax.length ()) ;
					ret.fill (BYTE (0XFF)) ;
					for (auto &&i : r3x)
						ret[i] = FALSE ;
					return move (ret) ;
				}) ;
				for (auto &&i : r4x)
					rax.remove (i) ;
				rax.remap () ;
			}
			mPath.store (CRef<ArrayList<String<STR>>>::make (move (rax))) ;
		}

		void update_child () const {
			if (mChild.fetch () != NULL)
				return ;
			auto rax = ArrayList<CHILD> (CHILD_MAX_SIZE::expr) ;
			auto rbx = String<STR>::make () ;
			rbx += mDire ;
			rbx += slice ("\\") ;
			const auto r1x = rbx.length () ;
			const auto r2x = UniqueRef<HDIRE> ([&] (VREF<HDIRE> me) {
				me = opendir ((&mDireA[0])) ;
				assume (me != NULL) ;
			} ,[] (VREF<HDIRE> me) {
				closedir (me) ;
			}) ;
			rbx[r1x] = 0 ;
			auto rdx = String<STR>::make () ;
			while (TRUE) {
				const auto r3x = readdir (r2x.self) ;
				if (r3x == NULL)
					break ;
				if ifswitch (TRUE) {
					BufferProc::buf_slice (rdx ,r3x->d_name ,rdx.size ()) ;
					if (rdx == slice ("."))
						discard ;
					if (rdx == slice (".."))
						discard ;
					rbx += rdx ;
					const auto r4x = BitProc::get_bit (CHAR (r3x->d_type) ,DT_DIR) ;
					const auto r5x = BitProc::get_bit (CHAR (r3x->d_type) ,DT_LNK) ;
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
			mChild.store (CRef<ArrayList<CHILD>>::make (move (rax))) ;
		}
	} ;
} ;

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
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file) override {
			mFile = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (file) ;
			mRead = 0 ;
			mWrite = 0 ;
		}

		void open () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,O_RDONLY ,r1x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mRead = 0 ;
		}

		void create () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = VAL32 (O_CREAT | O_WRONLY | O_TRUNC) ;
				const auto r2x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r1x ,r2x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mWrite = 0 ;
		}

		void append () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mReadPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = VAL32 (O_CREAT | O_RDONLY) ;
				const auto r2x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,r1x ,r2x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mWritePipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r3x = VAL32 (O_CREAT | O_WRONLY) ;
				const auto r4x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
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

		BOOL link (CREF<BOOL> readable ,CREF<BOOL> writable) override {
			BOOL ret = FALSE ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (readable)
					discard ;
				if (writable)
					discard ;
				open () ;
				ret = TRUE ;
			}
			if ifswitch (rxx) {
				if (readable)
					discard ;
				if ifnot (writable)
					discard ;
				create () ;
				ret = TRUE ;
			}
			if ifswitch (rxx) {
				if ifnot (readable)
					discard ;
				if ifnot (writable)
					discard ;
				try_invoke ([&] () {
					open () ;
				} ,[&] () {
					create () ;
				} ,[&] () {
					noop () ;
				}) ;
				close () ;
				append () ;
				ret = TRUE ;
			}
			return move (ret) ;
		}

		LENGTH read (VREF<RegBuffer<BYTE>> item) override {
			assert (mReadPipe.exist ()) ;
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = item.size () ;
			auto rax = r1x ;
			if ifswitch (TRUE) {
				if (rax == 0)
					discard ;
				const auto r2x = LENGTH (api::read (mReadPipe ,(&item.self[r1x - rax]) ,VAL32 (rax))) ;
				assume (r2x > 0) ;
				rax -= r2x ;
			}
			mRead += r1x ;
			return r1x - rax ;
		}

		LENGTH read (VREF<RegBuffer<WORD>> item) override {
			using R1X = SIZE_OF<WORD> ;
			if (item.size () == 0)
				return ZERO ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item[0])) ;
			LENGTH ret = read (RegBuffer<BYTE>::from (tmp ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		LENGTH read (VREF<RegBuffer<CHAR>> item) override {
			using R1X = SIZE_OF<CHAR> ;
			if (item.size () == 0)
				return ZERO ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item[0])) ;
			LENGTH ret = read (RegBuffer<BYTE>::from (tmp ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		LENGTH read (VREF<RegBuffer<DATA>> item) override {
			using R1X = SIZE_OF<DATA> ;
			if (item.size () == 0)
				return ZERO ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item[0])) ;
			LENGTH ret = read (RegBuffer<BYTE>::from (tmp ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_barrier () ;
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
				const auto r2x = LENGTH (api::write (mWritePipe ,(&item.self[r1x - rax]) ,VAL32 (rax))) ;
				assume (r2x > 0) ;
				rax -= r2x ;
			}
			mWrite += r1x ;
			return r1x - rax ;
		}

		LENGTH write (CREF<RegBuffer<WORD>> item) override {
			using R1X = SIZE_OF<WORD> ;
			if (item.size () == 0)
				return ZERO ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item[0])) ;
			LENGTH ret = write (RegBuffer<BYTE>::from (tmp ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		LENGTH write (CREF<RegBuffer<CHAR>> item) override {
			using R1X = SIZE_OF<CHAR> ;
			if (item.size () == 0)
				return ZERO ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item[0])) ;
			LENGTH ret = write (RegBuffer<BYTE>::from (tmp ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		LENGTH write (CREF<RegBuffer<DATA>> item) override {
			using R1X = SIZE_OF<DATA> ;
			if (item.size () == 0)
				return ZERO ;
			auto &&tmp = unsafe_cast[TYPEAS<TEMP<void>>::expr] (unsafe_deptr (item[0])) ;
			LENGTH ret = write (RegBuffer<BYTE>::from (tmp ,0 ,item.size () * R1X::expr)) ;
			ret /= R1X::expr ;
			unsafe_barrier () ;
			return move (ret) ;
		}

		void flush () override {
			if ifnot (mWritePipe.exist ())
				return ;
			syncfs (mWritePipe) ;
		}
	} ;
} ;

exports auto STREAMFILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename STREAMFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BUFFERFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using PAGE_SIZE = ENUMAS<VAL ,ENUMID<4194304>> ;
	using HEADER_SIZE = ENUMAS<VAL ,ENUMID<65536>> ;

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
		VAL64 mChunkPageSize ;
		VAL64 mChunkItemSize ;
		VAL64 mChunkSize ;
	} ;

	class ImplHolder implement Holder {
	protected:
		String<STRA> mFile ;
		Clazz mItemClazz ;
		UniqueRef<HFILE> mPipe ;
		UniqueRef<> mMapping ;
		VAL64 mFileSize ;
		VAL32 mFileMapFlag ;
		VRef<HEADER> mHeader ;
		Set<VAL64> mCacheSet ;
		List<CHUNK> mCacheList ;
		VAL64 mCacheTimer ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file ,CREF<Clazz> clazz) override {
			mFile = string_cvt[TYPEAS<TYPEAS<STRA ,STR>>::expr] (file) ;
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

		void open () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
				me = api::open ((&mFile[0]) ,O_RDONLY ,r1x) ;
				assume (me != NONE) ;
			} ,[] (VREF<HFILE> me) {
				api::close (me) ;
			}) ;
			mFileSize = file_size (mPipe) ;
			mFileMapFlag = VAL32 (PROT_READ) ;
			read_header () ;
		}

		void create () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = VAL32 (O_CREAT | O_RDWR | O_TRUNC) ;
				const auto r2x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
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
			mFileMapFlag = VAL32 (PROT_READ | PROT_WRITE) ;
			write_header () ;
		}

		void append () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			assume (mHeader != NULL) ;
			mPipe = UniqueRef<HFILE> ([&] (VREF<HFILE> me) {
				const auto r1x = VAL32 (O_CREAT | O_RDWR) ;
				const auto r2x = VAL32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
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
			mFileMapFlag = VAL32 (PROT_READ | PROT_WRITE) ;
			write_header () ;
		}

		VAL64 file_size (CREF<HFILE> handle) const {
			const auto r1x = VAL64 (lseek64 (handle ,0 ,SEEK_END)) ;
			const auto r2x = VAL64 (lseek64 (handle ,0 ,SEEK_SET)) ;
			//@warn: file in '/proc' is zero size
			if (r1x == NONE)
				return ZERO ;
			if (r2x == NONE)
				return ZERO ;
			assume (r1x >= 0) ;
			assume (r2x == 0) ;
			return r1x ;
		}

		void read_header () {
			assert (mHeader == NULL) ;
			mHeader = VRef<HEADER>::make () ;
			INDEX ix = load (0 ,HEADER_SIZE::expr) ;
			const auto r1x = FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::expr)) ;
			auto &&tmp = keep[TYPEAS<CREF<TEMP<void>>>::expr] (unsafe_pointer (r1x)) ;
			auto rax = ByteReader (RegBuffer<BYTE>::from (tmp ,0 ,HEADER_SIZE::expr).lift ()) ;
			rax >> ByteReader::GAP ;
			rax >> ByteReader::GAP ;
			rax >> slice ("CSC_BufferFile") ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mFileSize ;
			assume (mHeader->mFileSize == mFileSize) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mFileEndian ;
			const auto r2x = file_endian () ;
			assume (mHeader->mFileEndian == r2x) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mItemSize ;
			assume (mHeader->mItemSize == mItemClazz.type_size ()) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mItemAlign ;
			assume (mHeader->mItemAlign == mItemClazz.type_align ()) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mItemCount ;
			assume (mHeader->mItemCount >= 0) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mChunkPageSize ;
			assume (mHeader->mChunkPageSize == PAGE_SIZE::expr) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mChunkItemSize ;
			const auto r3x = mHeader->mChunkPageSize / mHeader->mItemSize ;
			assume (mHeader->mChunkItemSize == r3x) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mChunkSize ;
			assume (mHeader->mChunkSize >= 0) ;
			rax >> ByteReader::GAP ;
			rax >> ByteReader::GAP ;
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
				mHeader->mChunkPageSize = PAGE_SIZE::expr ;
				assume (mHeader->mChunkPageSize >= mHeader->mItemSize) ;
				mHeader->mChunkItemSize = mHeader->mChunkPageSize / mHeader->mItemSize ;
				mHeader->mChunkSize = 0 ;
			}
			INDEX ix = load (0 ,HEADER_SIZE::expr) ;
			const auto r2x = FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::expr)) ;
			auto &&tmp = unsafe_pointer (r2x) ;
			auto rax = ByteWriter (RegBuffer<BYTE>::from (tmp ,0 ,HEADER_SIZE::expr).lift ()) ;
			rax << ByteWriter::GAP ;
			rax << ByteWriter::GAP ;
			rax << slice ("CSC_BufferFile") ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mFileSize ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mFileEndian ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mItemSize ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mItemAlign ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mItemCount ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mChunkPageSize ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mChunkItemSize ;
			rax << ByteWriter::GAP ;
			rax << mHeader->mChunkSize ;
			rax << ByteWriter::GAP ;
			rax << ByteWriter::GAP ;
			flush () ;
		}

		VAL64 file_endian () const {
			const auto r1x = invoke ([&] () {
				auto rax = BoxBuffer<BYTE ,SIZE_OF<CHAR>> () ;
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

		BOOL link (CREF<BOOL> readable ,CREF<BOOL> writable) override {
			BOOL ret = FALSE ;
			auto rxx = TRUE ;
			if ifswitch (rxx) {
				if ifnot (readable)
					discard ;
				if (writable)
					discard ;
				open () ;
				ret = TRUE ;
			}
			if ifswitch (rxx) {
				if (readable)
					discard ;
				if ifnot (writable)
					discard ;
				create () ;
				ret = TRUE ;
			}
			if ifswitch (rxx) {
				if ifnot (readable)
					discard ;
				if ifnot (writable)
					discard ;
				try_invoke ([&] () {
					open () ;
				} ,[&] () {
					create () ;
				} ,[&] () {
					noop () ;
				}) ;
				close () ;
				append () ;
				ret = TRUE ;
			}
			return move (ret) ;
		}

		VAL64 length () const override {
			if (mHeader == NULL)
				return 0 ;
			return mHeader->mItemCount ;
		}

		void resize (CREF<VAL64> size_) override {
			assert (mPipe.exist ()) ;
			const auto r1x = mHeader->mItemCount ;
			const auto r2x = valign (r1x + size_ ,mHeader->mChunkItemSize) / mHeader->mChunkItemSize ;
			const auto r3x = MathProc::max_of (r2x - mHeader->mChunkSize ,VAL64 (ZERO)) ;
			mHeader->mItemCount += size_ ;
			mHeader->mChunkSize += r3x ;
			mHeader->mFileSize = HEADER_SIZE::expr + mHeader->mChunkSize * mHeader->mChunkPageSize ;
			if ifswitch (TRUE) {
				if (r3x == 0)
					discard ;
				close () ;
				append () ;
			}
		}

		void get (CREF<VAL64> index ,VREF<RegBuffer<BYTE>> item) override {
			assert (mPipe.exist ()) ;
			assume (vbetween (index ,0 ,mHeader->mItemCount)) ;
			assume (item.size () == mHeader->mItemSize) ;
			const auto r1x = index / mHeader->mChunkItemSize ;
			const auto r2x = index % mHeader->mChunkItemSize * mHeader->mItemSize ;
			const auto r3x = HEADER_SIZE::expr + r1x * mHeader->mChunkPageSize ;
			INDEX ix = load (r3x ,mHeader->mChunkPageSize) ;
			const auto r4x = r2x + FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::expr)) ;
			auto &&tmp = keep[TYPEAS<CREF<TEMP<void>>>::expr] (unsafe_pointer (r4x)) ;
			BufferProc::buf_copy (item ,RegBuffer<BYTE>::from (tmp ,0 ,VAL32_MAX) ,0 ,mHeader->mItemSize) ;
		}

		void set (CREF<VAL64> index ,CREF<RegBuffer<BYTE>> item) override {
			assert (mPipe.exist ()) ;
			assume (vbetween (index ,0 ,mHeader->mItemCount)) ;
			assume (item.size () == mHeader->mItemSize) ;
			const auto r1x = index / mHeader->mChunkItemSize ;
			const auto r2x = index % mHeader->mChunkItemSize * mHeader->mItemSize ;
			const auto r3x = HEADER_SIZE::expr + r1x * mHeader->mChunkPageSize ;
			INDEX ix = load (r3x ,mHeader->mChunkPageSize) ;
			const auto r4x = r2x + FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::expr)) ;
			auto &&tmp = unsafe_pointer (r4x) ;
			BufferProc::buf_copy (RegBuffer<BYTE>::from (tmp ,0 ,VAL32_MAX) ,item ,0 ,mHeader->mItemSize) ;
		}

		INDEX load (CREF<VAL64> offset ,CREF<LENGTH> size_) {
			INDEX ret = mCacheSet.map (offset) ;
			if ifswitch (TRUE) {
				if (ret != NONE)
					discard ;
				if ifswitch (TRUE) {
					if (mCacheList.length () < mCacheList.size ())
						discard ;
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
				ret = mCacheList.insert () ;
				mCacheSet.add (offset ,ret) ;
				mCacheList[ret].mOffset = offset ;
				mCacheList[ret].mBuffer = UniqueRef<Tuple<HANDLE ,LENGTH>> ([&] (VREF<Tuple<HANDLE ,LENGTH>> me) {
					const auto r2x = mmap64 (NULL ,size_ ,mFileMapFlag ,MAP_SHARED ,mPipe ,offset) ;
					assume (r2x != MAP_FAILED) ;
					me.mP1st = r2x ;
					me.mP2nd = size_ ;
				} ,[] (VREF<Tuple<HANDLE ,LENGTH>> me) {
					msync (me.mP1st ,me.mP2nd ,MS_SYNC) ;
					munmap (me.mP1st ,me.mP2nd) ;
				}) ;
			}
			mCacheList[ret].mCacheTime = mCacheTimer ;
			mCacheTimer++ ;
			return move (ret) ;
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

exports auto BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () ->VRef<Holder> {
	using R1X = typename BUFFERFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;