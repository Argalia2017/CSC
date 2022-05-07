#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

namespace CSC {
template <class DEPEND>
trait FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;
	using RETRY_TIMES = typename FILE_HELP<DEPEND ,ALWAYS>::RETRY_TIMES ;

	class ImplHolder implement Holder {
	protected:
		String<STR> mFile ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file_) override {
			mFile = move (file_) ;
			if ifswitch (TRUE) {
				const auto r1x = mFile.length () ;
				assume (r1x > 0) ;
				INDEX ix = r1x - 1 ;
				if (mFile[ix] != STR ('\\'))
					if (mFile[ix] != STR ('/'))
						discard ;
				assume (mFile[ix] != STR ('\\')) ;
				assume (mFile[ix] != STR ('\\')) ;
			}
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		VarBuffer<BYTE> load () const override {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = file_size (r1x) ;
			assume (r2x <= VAL32_MAX) ;
			const auto r3x = LENGTH (r2x) ;
			VarBuffer<BYTE> ret = VarBuffer<BYTE> (r3x) ;
			auto rax = r3x ;
			for (auto &&i : iter (0 ,RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r4x = ReadFile (r1x ,(&ret.self[r3x - rax]) ,rbx ,(&rbx) ,NULL) ;
				assume (r4x) ;
				rax -= LENGTH (rbx) ;
			}
			assume (rax == 0) ;
			return move (ret) ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const override {
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = file_size (r1x) ;
			assume (r2x <= VAL32_MAX) ;
			const auto r3x = MathProc::min_of (LENGTH (r2x) ,item.size ()) ;
			auto rax = r3x ;
			for (auto &&i : iter (0 ,RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r4x = ReadFile (r1x ,(&item.self[r3x - rax]) ,rbx ,(&rbx) ,NULL) ;
				assume (r4x) ;
				rax -= LENGTH (rbx) ;
			}
			assume (rax == 0) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const override {
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = item.size () ;
			auto rax = r2x ;
			for (auto &&i : iter (0 ,RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r3x = WriteFile (r1x ,(&item.self[r2x - rax]) ,rbx ,(&rbx) ,NULL) ;
				assume (r3x) ;
				rax -= LENGTH (rbx) ;
			}
			assume (rax == 0) ;
		}

		CRef<RegBuffer<BYTE>> load_asset () const override {
			const auto r1x = FindResource (NULL ,(&mFile[0]) ,TEXT ("BIN")) ;
			assume (r1x != NULL) ;
			const auto r2x = LoadResource (NULL ,r1x) ;
			assume (r2x != NULL) ;
			const auto r3x = FLAG (LockResource (r2x)) ;
			assume (r3x != ZERO) ;
			const auto r4x = LENGTH (SizeofResource (NULL ,r1x)) ;
			assume (r4x >= 0) ;
			auto &&tmp = keep[TYPEAS<CREF<BYTE>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<BYTE>>::id] (unsafe_pointer (r3x)))) ;
			return RegBuffer<BYTE>::from (unsafe_array (tmp) ,0 ,r4x) ;
		}

		BOOL available () const override {
			const auto r1x = GetFileAttributes ((&mFile[0])) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (BitProc::get_bit (CHAR (r1x) ,FILE_ATTRIBUTE_DIRECTORY))
				return FALSE ;
			return TRUE ;
		}

		VAL64 length () const override {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			if (r1x.self == NULL)
				return ZERO ;
			return file_size (r1x) ;
		}

		VAL64 file_size (CREF<HANDLE> handle) const {
			auto rax = ARRAY2<DWORD> () ;
			rax[0] = GetFileSize (handle ,(&rax[1])) ;
			const auto r1x = VAL64 (BitProc::up_bit (CHAR (rax[1]) ,CHAR (rax[0]))) ;
			assume (r1x >= 0) ;
			return r1x ;
		}

		void erase () const override {
			DeleteFile ((&mFile[0])) ;
		}

		void copy_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = available () ;
			assume (ifnot (r2x)) ;
			CopyFile ((&r1x->mFile[0]) ,(&mFile[0]) ,TRUE) ;
		}

		void move_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = available () ;
			assume (ifnot (r2x)) ;
			MoveFile ((&r1x->mFile[0]) ,(&mFile[0])) ;
		}

		void link_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = available () ;
			assume (ifnot (r2x)) ;
			CreateHardLink ((&mFile[0]) ,(&r1x->mFile[0]) ,NULL) ;
		}

		BOOL identical (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			auto rax = ARRAY2<BY_HANDLE_FILE_INFORMATION> () ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			zeroize (rax[0]) ;
			const auto r3x = GetFileInformationByHandle (r2x ,(&rax[0])) ;
			if ifnot (r3x)
				return FALSE ;
			if (rax[0].nNumberOfLinks == 0)
				return FALSE ;
			const auto r4x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&r1x->mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			zeroize (rax[1]) ;
			const auto r5x = GetFileInformationByHandle (r4x ,(&rax[1])) ;
			if ifnot (r5x)
				return FALSE ;
			if (rax[1].nNumberOfLinks == 0)
				return FALSE ;
			if (rax[0].dwVolumeSerialNumber != rax[1].dwVolumeSerialNumber)
				return FALSE ;
			if (rax[0].nFileIndexHigh != rax[1].nFileIndexHigh)
				return FALSE ;
			if (rax[0].nFileIndexLow != rax[1].nFileIndexLow)
				return FALSE ;
			return TRUE ;
		}
	} ;
} ;

template <>
exports auto FILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait DIRECTORY_DECOUPLE_HELP<DEPEND ,ALWAYS> {
	using SUPER = typename TEXTREADER_IMPLHOLDER_HELP<STR ,ALWAYS>::ImplHolder ;

	class ImplHolder implement SUPER {
	public:
		implicit ImplHolder () = default ;

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
	} ;

	struct FUNCTION_decouple_path {
		inline ArrayList<String<STR>> operator() (CREF<String<STR>> dire) const {
			ArrayList<String<STR>> ret ;
			auto rax = TextReader<STR> (dire.raw ()) ;
			rax.set_attr (TYPEAS<ImplHolder>::id) ;
			const auto r1x = rax.get_attr () ;
			INDEX ix = ret.insert () ;
			auto rbx = STR () ;
			if ifswitch (TRUE) {
				rax.backup () ;
				rax >> rbx ;
				if (r1x->is_space (rbx))
					discard ;
				rax.recover () ;
			}
			while (TRUE) {
				rax >> ret[ix] ;
				if (ret[ix].empty ())
					break ;
				ix = ret.insert () ;
				rax >> rbx ;
				if (rbx == r1x->ending_item ())
					break ;
				assume (r1x->is_space (rbx)) ;
			}
			rax >> TextReader<STR>::EOS ;
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
	using CHILD_MAX_SIZE = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::CHILD_MAX_SIZE ;

	class ImplHolder implement Holder {
	protected:
		String<STR> mDire ;
		Cell<CRef<ArrayList<String<STR>>>> mPath ;
		Cell<CRef<ArrayList<CHILD>>> mChild ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> dire_) override {
			mDire = move (dire_) ;
			mPath = Cell<CRef<ArrayList<String<STR>>>>::make () ;
			mChild = Cell<CRef<ArrayList<CHILD>>>::make () ;
			if ifswitch (TRUE) {
				const auto r1x = mDire.length () ;
				assume (r1x > 0) ;
				INDEX ix = r1x - 1 ;
				if (mDire[ix] != STR ('\\'))
					if (mDire[ix] != STR ('/'))
						discard ;
				mDire[ix] = 0 ;
			}
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
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
				ret += slice ("\\") ;
			}
			for (auto &&i : r1x->iter ()) {
				ret += r1x.self[i] ;
				ret += slice ("\\") ;
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
			const auto r1x = GetFileAttributes ((&mDire[0])) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if ifnot (BitProc::get_bit (CHAR (r1x) ,FILE_ATTRIBUTE_DIRECTORY))
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
				auto &&tmp = lock_handle (r1x ,r7x) ;
				return tmp->available () ;
			}
			return FALSE ;
		}

		CREF<UniqueRef<File>> lock_handle (CREF<String<STR>> file_ ,CREF<ConBuffer<BYTE>> snapshot_) const {
			return memorize ([&] () {
				return UniqueRef<File> ([&] (VREF<File> me) {
					me = File (file_) ;
					me.save (snapshot_) ;
				} ,[] (VREF<File> me) {
					me.erase () ;
				}) ;
			}) ;
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
				rax += slice ("\\") ;
			}
			for (auto &&i : r1x->iter ()) {
				rax += r1x.self[i] ;
				if ifswitch (TRUE) {
					const auto r2x = r1x.self[i].length () ;
					if (r2x == 0)
						discard ;
					if (r1x.self[i][r2x - 1] == STR (':'))
						discard ;
					CreateDirectory ((&rax[0]) ,NULL) ;
				}
				rax += slice ("\\") ;
			}
		}

		void erase () const override {
			RemoveDirectory ((&mDire[0])) ;
			fresh () ;
		}

		void clear () const override {
			auto rax = Deque<String<STR>> (CHILD_MAX_SIZE::value) ;
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

	private:
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
			auto rax = ArrayList<CHILD> (CHILD_MAX_SIZE::value) ;
			auto rbx = String<STR>::make () ;
			rbx += mDire ;
			rbx += slice ("\\") ;
			const auto r1x = rbx.length () ;
			rbx += slice ("*.*") ;
			auto rcx = WIN32_FIND_DATA () ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				zeroize (rcx) ;
				me = FindFirstFile ((&rbx[0]) ,(&rcx)) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				FindClose (me) ;
			}) ;
			rbx[r1x] = 0 ;
			auto rdx = String<STR>::make () ;
			while (TRUE) {
				if (rcx.cFileName[0] == 0)
					break ;
				if ifswitch (TRUE) {
					BufferProc::buf_slice (rdx ,rcx.cFileName ,rdx.size ()) ;
					if (rdx == slice ("."))
						discard ;
					if (rdx == slice (".."))
						discard ;
					rbx += rdx ;
					const auto r3x = BitProc::get_bit (CHAR (rcx.dwFileAttributes) ,FILE_ATTRIBUTE_DIRECTORY) ;
					const auto r4x = BitProc::get_bit (CHAR (rcx.dwFileAttributes) ,FILE_ATTRIBUTE_REPARSE_POINT) ;
					assume (rax.length () < rax.size ()) ;
					INDEX ix = rax.insert () ;
					rax[ix].mFile = rbx ;
					rax[ix].mIsFile = ifnot (r3x) ;
					rax[ix].mIsDire = r3x ;
					rax[ix].mIsLink = r4x ;
				}
				rbx[r1x] = 0 ;
				rcx.cFileName[0] = 0 ;
				FindNextFile (r2x ,(&rcx)) ;
			}
			rax.remap () ;
			mChild.store (CRef<ArrayList<CHILD>>::make (move (rax))) ;
		}
	} ;
} ;

template <>
exports auto DIRECTORY_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait STREAMFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename STREAMFILE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		String<STR> mFile ;
		UniqueRef<HANDLE> mReadPipe ;
		UniqueRef<HANDLE> mWritePipe ;
		VAL64 mRead ;
		VAL64 mWrite ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file_) override {
			mFile = move (file_) ;
			mRead = 0 ;
			mWrite = 0 ;
		}

		void open () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mReadPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r1x = DWORD (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,r1x ,NULL ,OPEN_EXISTING ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mRead = 0 ;
		}

		void create () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mWritePipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r1x = DWORD (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
				me = CreateFile ((&mFile[0]) ,GENERIC_WRITE ,r1x ,NULL ,CREATE_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mWrite = 0 ;
		}

		void open_create () override {
			assert (ifnot (mReadPipe.exist ())) ;
			assert (ifnot (mWritePipe.exist ())) ;
			mReadPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r1x = DWORD (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,r1x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mWritePipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r2x = DWORD (FILE_SHARE_READ | FILE_SHARE_WRITE) ;
				me = CreateFile ((&mFile[0]) ,GENERIC_WRITE ,r2x ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mRead = 0 ;
			mWrite = 0 ;
			if ifswitch (TRUE) {
				auto rax = LONG (0) ;
				const auto r3x = SetFilePointer (mWritePipe ,0 ,(&rax) ,FILE_END) ;
				const auto r4x = VAL64 (BitProc::up_bit (CHAR (rax) ,CHAR (r3x))) ;
				if (r4x <= 0)
					return ;
				mWrite += r4x ;
			}
		}

		void close () override {
			mReadPipe = UniqueRef<HANDLE> () ;
			mWritePipe = UniqueRef<HANDLE> () ;
		}

		LENGTH read (VREF<RegBuffer<BYTE>> item) override {
			assert (mReadPipe.exist ()) ;
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = item.size () ;
			auto rax = r1x ;
			if ifswitch (TRUE) {
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r2x = ReadFile (mReadPipe ,(&item.self[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
				if ifnot (r2x)
					continue ;
				rax -= LENGTH (rbx) ;
			}
			mRead += r1x ;
			return r1x - rax ;
		}

		LENGTH write (CREF<RegBuffer<BYTE>> item) override {
			assert (mWritePipe.exist ()) ;
			assert (item.size () <= VAL32_MAX) ;
			const auto r1x = item.size () ;
			auto rax = r1x ;
			if ifswitch (TRUE) {
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r2x = WriteFile (mWritePipe ,(&item.self[r1x - rax]) ,rbx ,(&rbx) ,NULL) ;
				if ifnot (r2x)
					continue ;
				rax -= LENGTH (rbx) ;
			}
			mWrite += r1x ;
			return r1x - rax ;
		}

		void flush () override {
			if ifnot (mWritePipe.exist ())
				return ;
			FlushFileBuffers (mWritePipe) ;
		}
	} ;
} ;

template <>
exports auto STREAMFILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename STREAMFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}

template <class DEPEND>
trait BUFFERFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using Holder = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::Holder ;
	using PAGE_SIZE = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::PAGE_SIZE ;
	using HEADER_SIZE = typename BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::HEADER_SIZE ;

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
		String<STR> mFile ;
		Clazz mItemClazz ;
		UniqueRef<HANDLE> mPipe ;
		UniqueRef<HANDLE> mMapping ;
		VAL64 mFileSize ;
		DWORD mFileMapFlag ;
		VRef<HEADER> mHeader ;
		Set<VAL64> mCacheSet ;
		List<CHUNK> mCacheList ;
		VAL64 mCacheTimer ;

	public:
		implicit ImplHolder () = default ;

		void initialize (CREF<String<STR>> file_ ,CREF<Clazz> clazz) override {
			mFile = move (file_) ;
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
			mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mFileSize = file_size (mPipe) ;
			const auto r1x = DWORD (BitProc::high_bit (DATA (mFileSize))) ;
			const auto r2x = DWORD (BitProc::low_bit (DATA (mFileSize))) ;
			mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFileMapping (mPipe ,NULL ,PAGE_READONLY ,r1x ,r2x ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mFileMapFlag = DWORD (FILE_MAP_READ) ;
			read_header () ;
		}

		void create () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r1x = DWORD (GENERIC_READ | GENERIC_WRITE) ;
				me = CreateFile ((&mFile[0]) ,r1x ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mFileSize = HEADER_SIZE::value ;
			const auto r2x = DWORD (BitProc::high_bit (DATA (mFileSize))) ;
			const auto r3x = DWORD (BitProc::low_bit (DATA (mFileSize))) ;
			mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFileMapping (mPipe ,NULL ,PAGE_READWRITE ,r2x ,r3x ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mFileMapFlag = DWORD (FILE_MAP_READ | FILE_MAP_WRITE) ;
			write_header () ;
		}

		void open_create () override {
			assert (ifnot (mPipe.exist ())) ;
			assert (ifnot (mMapping.exist ())) ;
			assume (mHeader != NULL) ;
			mPipe = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				const auto r1x = DWORD (GENERIC_READ | GENERIC_WRITE) ;
				me = CreateFile ((&mFile[0]) ,r1x ,0 ,NULL ,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mFileSize = mHeader->mFileSize ;
			const auto r2x = DWORD (BitProc::high_bit (DATA (mFileSize))) ;
			const auto r3x = DWORD (BitProc::low_bit (DATA (mFileSize))) ;
			mMapping = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFileMapping (mPipe ,NULL ,PAGE_READWRITE ,r2x ,r3x ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			mFileMapFlag = DWORD (FILE_MAP_READ | FILE_MAP_WRITE) ;
			write_header () ;
		}

		VAL64 file_size (CREF<HANDLE> handle) const {
			auto rax = ARRAY2<DWORD> () ;
			rax[0] = GetFileSize (handle ,(&rax[1])) ;
			const auto r1x = VAL64 (BitProc::up_bit (CHAR (rax[1]) ,CHAR (rax[0]))) ;
			assume (r1x >= 0) ;
			return r1x ;
		}

		void read_header () {
			assert (mHeader == NULL) ;
			mHeader = VRef<HEADER>::make () ;
			INDEX ix = load (0 ,HEADER_SIZE::value) ;
			const auto r1x = FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::id)) ;
			auto &&tmp = keep[TYPEAS<CREF<BYTE>>::id] (unsafe_deref (unsafe_cast[TYPEAS<TEMP<BYTE>>::id] (unsafe_pointer (r1x)))) ;
			auto rax = ByteReader (RegBuffer<BYTE>::from (unsafe_array (tmp) ,0 ,HEADER_SIZE::value)) ;
			rax >> ByteReader::GAP ;
			rax >> ByteReader::GAP ;
			rax >> slice ("CSC_BufferFile") ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mFileSize ;
			assume (mHeader->mFileSize == mFileSize) ;
			rax >> ByteReader::GAP ;
			rax >> mHeader->mFileEndian ;
			const auto r2x = invoke ([&] () {
				auto rax = Box<ARR<BYTE ,RANK4>>::make () ;
				rax.self[0] = BYTE (0X00) ;
				rax.self[1] = BYTE (0X01) ;
				rax.self[2] = BYTE (0X02) ;
				rax.self[3] = BYTE (0X03) ;
				return VAL64 (bitwise[TYPEAS<CHAR>::id] (rax.self)) ;
			}) ;
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
			assume (mHeader->mChunkPageSize == PAGE_SIZE::value) ;
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
				const auto r1x = invoke ([&] () {
					auto rax = Box<ARR<BYTE ,RANK4>>::make () ;
					rax.self[0] = BYTE (0X00) ;
					rax.self[1] = BYTE (0X01) ;
					rax.self[2] = BYTE (0X02) ;
					rax.self[3] = BYTE (0X03) ;
					return VAL64 (bitwise[TYPEAS<CHAR>::id] (rax.self)) ;
				}) ;
				mHeader->mFileEndian = r1x ;
				mHeader->mItemSize = mItemClazz.type_size () ;
				mHeader->mItemAlign = mItemClazz.type_align () ;
				mHeader->mItemCount = 0 ;
				mHeader->mChunkPageSize = PAGE_SIZE::value ;
				mHeader->mChunkItemSize = mHeader->mChunkPageSize / mHeader->mItemSize ;
				mHeader->mChunkSize = 0 ;
			}
			INDEX ix = load (0 ,HEADER_SIZE::value) ;
			const auto r2x = FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::id)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BYTE>>::id] (unsafe_pointer (r2x))) ;
			auto rax = ByteWriter (RegBuffer<BYTE>::from (unsafe_array (tmp) ,0 ,HEADER_SIZE::value)) ;
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

		void close () override {
			flush () ;
			mMapping = UniqueRef<HANDLE> () ;
			mPipe = UniqueRef<HANDLE> () ;
		}

		VAL64 length () const override {
			if (mHeader == NULL)
				return ZERO ;
			return mHeader->mItemCount ;
		}

		VAL64 insert (CREF<VAL64> size_) override {
			assert (mPipe.exist ()) ;
			const auto r1x = mHeader->mItemCount ;
			const auto r2x = valign (mHeader->mItemCount + size_ ,mHeader->mChunkItemSize) / mHeader->mChunkItemSize ;
			const auto r3x = MathProc::max_of (r2x - mHeader->mChunkSize ,VAL64 (ZERO)) ;
			mHeader->mItemCount += size_ ;
			mHeader->mChunkSize += r3x ;
			mHeader->mFileSize = HEADER_SIZE::value + mHeader->mChunkSize * mHeader->mChunkPageSize ;
			if ifswitch (TRUE) {
				if (r3x == 0)
					discard ;
				close () ;
				open_create () ;
			}
			return r1x ;
		}

		void get (CREF<VAL64> index ,VREF<RegBuffer<BYTE>> item) override {
			assert (mPipe.exist ()) ;
			assume (vbetween (index ,0 ,mHeader->mItemCount)) ;
			assume (item.size () == mHeader->mItemSize) ;
			const auto r1x = index / mHeader->mChunkItemSize ;
			const auto r2x = index % mHeader->mChunkItemSize * mHeader->mItemSize ;
			const auto r3x = HEADER_SIZE::value + r1x * mHeader->mChunkPageSize ;
			INDEX ix = load (r3x ,mHeader->mChunkPageSize) ;
			const auto r4x = r2x + FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::id)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BYTE>>::id] (unsafe_pointer (r4x))) ;
			auto rax = RegBuffer<BYTE>::from (unsafe_array (tmp) ,0 ,mHeader->mItemSize) ;
			BufferProc::buf_copy (item ,rax.self ,0 ,mHeader->mItemSize) ;
		}

		void set (CREF<VAL64> index ,CREF<RegBuffer<BYTE>> item) override {
			assert (mPipe.exist ()) ;
			assume (vbetween (index ,0 ,mHeader->mItemCount)) ;
			assume (item.size () == mHeader->mItemSize) ;
			const auto r1x = index / mHeader->mChunkItemSize ;
			const auto r2x = index % mHeader->mChunkItemSize * mHeader->mItemSize ;
			const auto r3x = HEADER_SIZE::value + r1x * mHeader->mChunkPageSize ;
			INDEX ix = load (r3x ,mHeader->mChunkPageSize) ;
			const auto r4x = r2x + FLAG (mCacheList[ix].mBuffer->pick (TYPEAS<RANK0>::id)) ;
			auto &&tmp = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BYTE>>::id] (unsafe_pointer (r4x))) ;
			auto rax = RegBuffer<BYTE>::from (unsafe_array (tmp) ,0 ,mHeader->mItemSize) ;
			BufferProc::buf_copy (rax.self ,item ,0 ,mHeader->mItemSize) ;
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
				const auto r2x = DWORD (BitProc::high_bit (DATA (offset))) ;
				const auto r3x = DWORD (BitProc::low_bit (DATA (offset))) ;
				mCacheList[ret].mBuffer = UniqueRef<Tuple<HANDLE ,LENGTH>> ([&] (VREF<Tuple<HANDLE ,LENGTH>> me) {
					const auto r4x = MapViewOfFile (mMapping ,mFileMapFlag ,r2x ,r3x ,size_) ;
					assume (r4x != NULL) ;
					me.mP1st = r4x ;
					me.mP2nd = size_ ;
				} ,[] (VREF<Tuple<HANDLE ,LENGTH>> me) {
					FlushViewOfFile (me.mP1st ,me.mP2nd) ;
					UnmapViewOfFile (me.mP1st) ;
				}) ; ;
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
			FlushFileBuffers (mPipe) ;
		}
	} ;
} ;

template <>
exports auto BUFFERFILE_HOLDER_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename BUFFERFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;