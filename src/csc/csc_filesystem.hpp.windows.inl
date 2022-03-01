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
	using FILE_NAME_SSIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SSIZE ;
	using FILE_RETRY_TIMES = RANK2 ;
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		String<STR> mFile ;

	public:
		void init_file (CREF<String<STR>> file_) override {
			mFile = move (file_) ;
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
			const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
			assume (r2x >= 0) ;
			assume (r2x <= VAL32_MAX) ;
			VarBuffer<BYTE> ret = VarBuffer<BYTE> (r2x) ;
			auto rax = r2x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r3x = ReadFile (r1x.self ,(&ret.self[r2x - rax]) ,rbx ,(&rbx) ,NULL) ;
				assume (r3x) ;
				rax -= LENGTH (rbx) ;
			}
			assume (rax == 0) ;
			return move (ret) ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const override {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
			assume (r2x >= 0) ;
			assume (r2x <= item.size ()) ;
			const auto r3x = min_of (r2x ,item.size ()) ;
			auto rax = r3x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r4x = ReadFile (r1x.self ,(&item.self[r3x - rax]) ,rbx ,(&rbx) ,NULL) ;
				assume (r4x) ;
				rax -= LENGTH (rbx) ;
			}
		}

		void save (CREF<RegBuffer<BYTE>> item) const override {
			assume (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = item.size () ;
			auto rax = r2x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rax == 0)
					break ;
				auto rbx = DWORD (rax) ;
				const auto r3x = WriteFile (r1x.self ,(&item.self[r2x - rax]) ,rbx ,(&rbx) ,NULL) ;
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
			auto &&tmp_a = unsafe_deref (unsafe_cast[TYPEAS<TEMP<BYTE>>::id] (unsafe_pointer (r3x))) ;
			auto &&tmp_b = keep[TYPEAS<CREF<ARR<BYTE>>>::id] (unsafe_array (tmp_a)) ;
			return RegBuffer<BYTE>::from (tmp_b ,0 ,r4x) ;
		}

		BOOL find () const override {
			const auto r1x = GetFileAttributes ((&mFile[0])) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (CSC::CHAR (r1x & FILE_ATTRIBUTE_DIRECTORY) != CSC::CHAR (0X00))
				return FALSE ;
			return TRUE ;
		}

		LENGTH file_size () const override {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			if (r1x == NULL)
				return ZERO ;
			return LENGTH (GetFileSize (r1x.self ,NULL)) ;
		}

		void erase () const override {
			DeleteFile ((&mFile[0])) ;
		}

		void copy_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = find () ;
			assume (ifnot (r2x)) ;
			CopyFile ((&r1x->mFile[0]) ,(&mFile[0]) ,TRUE) ;
		}

		void move_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = find () ;
			assume (ifnot (r2x)) ;
			MoveFile ((&r1x->mFile[0]) ,(&mFile[0])) ;
		}

		void link_from (CREF<Holder> that) const override {
			const auto r1x = that.native ().poll (TYPEAS<CRef<ImplHolder>>::id) ;
			const auto r2x = find () ;
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
			const auto r3x = GetFileInformationByHandle (r2x.self ,(&rax[0])) ;
			if (r3x == 0)
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
			const auto r5x = GetFileInformationByHandle (r4x.self ,(&rax[1])) ;
			if (r5x == 0)
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
trait DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SSIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SSIZE ;
	using Holder = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	protected:
		String<STR> mDirectory ;
		Cell<CRef<ArrayList<String<STR>>>> mPath ;
		Cell<CRef<ArrayList<String<STR>>>> mLoadFile ;
		Cell<CRef<ArrayList<String<STR>>>> mLoadDire ;

	public:
		void init_dire (CREF<String<STR>> dire_) override {
			mDirectory = move (dire_) ;
			mPath = NULL ;
			mLoadFile = NULL ;
			mLoadDire = NULL ;
		}

		Auto native () const leftvalue override {
			return CRef<ImplHolder>::reference (thiz) ;
		}

		String<STR> path () const override {
			const auto r1x = mDirectory.length () ;
			const auto r2x = mDirectory.find (STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = mDirectory.find (STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = max_of (r2x ,r3x ,ZERO) ;
			return mDirectory.segment (0 ,r4x) ;
		}

		String<STR> name () const override {
			const auto r1x = mDirectory.length () ;
			const auto r2x = mDirectory.find (STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = mDirectory.find (STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = max_of (r2x ,r3x) + 1 ;
			return mDirectory.segment (r4x ,r1x) ;
		}

		String<STR> absolute () const override {
			return mDirectory ;
		}

		LENGTH depth () const override {
			unimplemented () ;
			return ZERO ;
		}

		void parent_from (CREF<Holder> a) override {
			unimplemented () ;
		}

		void brother_from (CREF<Holder> a) override {
			unimplemented () ;
		}

		void child_from (CREF<Holder> a) override {
			unimplemented () ;
		}

		BOOL find () const override {
			if (mDirectory.empty ())
				return FALSE ;
			const auto r1x = GetFileAttributes ((&mDirectory[0])) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (CSC::CHAR (r1x & FILE_ATTRIBUTE_DIRECTORY) == CSC::CHAR (0X00))
				return FALSE ;
			return TRUE ;
		}

		BOOL lock () const override {
			unimplemented () ;
			return FALSE ;
		}

		void build () const override {
			unimplemented () ;
		}

		void erase () const override {
			RemoveDirectory ((&mDirectory[0])) ;
		}

		void clear () const override {
			const auto r1x = mLoadFile.fetch () ;
			const auto r2x = mLoadDire.fetch () ;
			for (auto &&i : r1x->iter ()) {
				const auto r3x = File (r1x.self[i]) ;
				r3x.erase () ;
			}
			for (auto &&i : r2x->iter ()) {
				const auto r4x = Directory (r2x.self[i]) ;
				r4x.clear () ;
				r4x.erase () ;
			}
		}

	private:
		void update_decouple () const {
			using R1X = typename DIRECTORY_DECOUPLE_HELP<DEPEND ,ALWAYS>::ImplAttribute ;
			if (mPath != NULL)
				return ;
			auto rax = ArrayList<String<STR>> () ;
			auto rbx = TextReader<STR> (mDirectory.raw ()) ;
			rbx.set_attr (TYPEAS<R1X>::id) ;
			const auto r1x = rbx.get_attr () ;
			INDEX ix = rax.insert () ;
			auto rcx = STR () ;
			if ifswitch (TRUE) {
				rbx.backup () ;
				rbx >> rcx ;
				if (r1x->is_space (rcx))
					discard ;
				rbx.recover () ;
			}
			while (TRUE) {
				rbx >> rax[ix] ;
				if (rax[ix].empty ())
					break ;
				ix = rax.insert () ;
				rbx >> rcx ;
				if (rcx == r1x->ending_item ())
					break ;
				assume (r1x->is_space (rcx)) ;
			}
			rbx >> TextReader<STR>::EOS ;
			rax.remove (ix) ;
			rax.remap () ;
			mPath = Cell<CRef<ArrayList<String<STR>>>>::make (CRef<ArrayList<String<STR>>>::make (move (rax))) ;
		}

		void update_child () const {
			if (mLoadFile != NULL)
				if (mLoadDire != NULL)
					return ;
			auto rax = ARRAY2<ArrayList<String<STR>>> () ;
			auto rbx = String<STR>::make () ;
			rbx += mDirectory ;
			rbx += slice ("\\") ;
			const auto r1x = rbx.length () ;
			rbx += slice ("*.*") ;
			auto rcx = WIN32_FIND_DATA () ;
			const auto r2x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				zeroize (rcx) ;
				me = FindFirstFile ((&rbx[0]) ,(&rcx)) ;
				replace (me ,INVALID_HANDLE_VALUE ,NULL) ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				FindClose (me) ;
			}) ;
			rbx[r1x] = 0 ;
			if (r2x.self == NULL)
				return ;
			while (TRUE) {
				if (rcx.cFileName[0] == 0)
					break ;
				if ifswitch (TRUE) {
					const auto r3x = invoke ([&] () {
						String<STR> ret = String<STR> (MAX_PATH) ;
						for (auto &&i : iter (0 ,ret.size ())) {
							if (rcx.cFileName[i] == 0)
								break ;
							ret[i] = rcx.cFileName[i] ;
						}
						return move (ret) ;
					}) ;
					if (r3x == slice ("."))
						discard ;
					if (r3x == slice (".."))
						discard ;
					rbx += r3x ;
					const auto r4x = invoke ([&] () {
						if ((rcx.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
							return INDEX (0) ;
						return INDEX (1) ;
					}) ;
					rax[r4x].add (rbx) ;
				}
				rbx[r1x] = 0 ;
				rcx.cFileName[0] = 0 ;
				FindNextFile (r2x.self ,(&rcx)) ;
			}
			mLoadFile = Cell<CRef<ArrayList<String<STR>>>>::make (CRef<ArrayList<String<STR>>>::make (move (rax[0]))) ;
			mLoadDire = Cell<CRef<ArrayList<String<STR>>>>::make (CRef<ArrayList<String<STR>>>::make (move (rax[1]))) ;
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
	public:
		void init_read (CREF<String<STR>> file_) const override {

		}

		void init_write (CREF<String<STR>> file_) const override {

		}

		LENGTH size () const override {
			return 0 ;
		}

		LENGTH length () const override {
			return 0 ;
		}

		void reset () override {

		}

		void reset (CREF<INDEX> read_ ,CREF<INDEX> write_) override {

		}

		void backup () override {

		}

		void recover () override {

		}

		void read (VREF<RegBuffer<BYTE>> item) override {

		}

		void write (CREF<RegBuffer<BYTE>> item) override {

		}

		void flush () override {

		}
	} ;
} ;

template <>
exports auto STREAMFILE_HELP<DEPEND ,ALWAYS>::FUNCTION_extern::invoke () -> VRef<Holder> {
	using R1X = typename STREAMFILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
}
} ;