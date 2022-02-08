#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
#error "∑(っ°Д° ;)っ : bad include"
#endif

namespace CSC {
namespace FILESYSTEM {
template <class...>
trait FILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SSIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SSIZE ;
	using FILE_RETRY_TIMES = RANK2 ;
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		String<STR> mFile ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		void init_file (CREF<String<STR>> file_) override {
			mHeap.mFile = move (file_) ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		String<STR> path () const override {
			String<STR> ret = String<STR> (FILE_NAME_SSIZE::value) ;
			const auto r1x = mHeap.mFile.length () ;
			const auto r2x = mHeap.mFile.find (STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = mHeap.mFile.find (STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = max_of (r2x ,r3x ,ZERO) ;
			for (auto &&i : iter (0 ,r4x))
				ret[i] = mHeap.mFile[i] ;
			return move (ret) ;
		}

		String<STR> name () const override {
			String<STR> ret = String<STR> (FILE_NAME_SSIZE::value) ;
			const auto r1x = mHeap.mFile.length () ;
			const auto r2x = mHeap.mFile.find (STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = mHeap.mFile.find (STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = max_of (r2x ,r3x) + 1 ;
			for (auto &&i : iter (r4x ,r1x)) {
				INDEX ix = i - r4x ;
				ret[ix] = mHeap.mFile[i] ;
			}
			return move (ret) ;
		}

		String<STR> full_name () const override {
			return mHeap.mFile ;
		}

		VarBuffer<BYTE> load () const override {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mHeap.mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
			assume (r2x >= 0) ;
			assume (r2x <= VAL32_MAX) ;
			VarBuffer<BYTE> ret = VarBuffer<BYTE> (r2x) ;
			auto rax = DWORD () ;
			auto rbx = r2x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rbx == 0)
					break ;
				rax = DWORD (0) ;
				const auto r3x = ReadFile (r1x.self ,(&ret.self[r2x - rbx]) ,DWORD (rbx) ,(&rax) ,NULL) ;
				assume (r3x) ;
				rbx -= LENGTH (rax) ;
			}
			assume (rbx == 0) ;
			return move (ret) ;
		}

		void load (VREF<RegBuffer<BYTE>> item) const override {
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mHeap.mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
			assume (r2x >= 0) ;
			assume (r2x <= item.size ()) ;
			auto rax = DWORD () ;
			auto rbx = r2x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rbx == 0)
					break ;
				rax = DWORD (0) ;
				const auto r3x = ReadFile (r1x.self ,(&item.self[r2x - rbx]) ,DWORD (rbx) ,(&rax) ,NULL) ;
				assume (r3x) ;
				rbx -= LENGTH (rax) ;
			}
			for (auto &&i : iter (rbx ,item.size ()))
				item[i] = BYTE (0X00) ;
		}

		void save (CREF<RegBuffer<BYTE>> item) const override {
			assume (item.size () <= VAL32_MAX) ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mHeap.mFile[0]) ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
				assume (me != NULL) ;
			} ,[] (VREF<HANDLE> me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = item.size () ;
			auto rax = DWORD () ;
			auto rbx = r2x ;
			for (auto &&i : iter (0 ,FILE_RETRY_TIMES::value)) {
				noop (i) ;
				if (rbx == 0)
					break ;
				rax = DWORD (0) ;
				const auto r3x = WriteFile (r1x.self ,(&item.self[r2x - rbx]) ,DWORD (rbx) ,(&rax) ,NULL) ;
				assume (r3x) ;
				rbx -= LENGTH (rax) ;
			}
			assume (rbx == 0) ;
		}

		CRef<RegBuffer<BYTE>> load_asset (CREF<FLAG> uuid) const override {
			const auto r1x = FindResource (NULL ,MAKEINTRESOURCE (uuid) ,TEXT ("BIN")) ;
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
			const auto r1x = GetFileAttributes ((&mHeap.mFile[0])) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (CSC::CHAR (r1x & FILE_ATTRIBUTE_DIRECTORY) != CSC::CHAR (0X00))
				return FALSE ;
			return TRUE ;
		}

		void erase () const override {
			DeleteFile ((&mHeap.mFile[0])) ;
		}

		void copy_from (CREF<Holder> that) const override {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			const auto r1x = find () ;
			assume (ifnot (r1x)) ;
			CopyFile ((&tmp.mFile[0]) ,(&mHeap.mFile[0]) ,TRUE) ;
		}

		void move_from (CREF<Holder> that) const override {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			const auto r1x = find () ;
			assume (ifnot (r1x)) ;
			MoveFile ((&tmp.mFile[0]) ,(&mHeap.mFile[0])) ;
		}

		void link_from (CREF<Holder> that) const override {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			const auto r1x = find () ;
			assume (ifnot (r1x)) ;
			CreateHardLink ((&mHeap.mFile[0]) ,(&tmp.mFile[0]) ,NULL) ;
		}

		BOOL identical (CREF<Holder> that) const override {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			auto rax = ARRAY2<BY_HANDLE_FILE_INFORMATION> () ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&mHeap.mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			zeroize (rax[0]) ;
			const auto r2x = GetFileInformationByHandle (r1x.self ,(&rax[0])) ;
			if (r2x == 0)
				return FALSE ;
			if (rax[0].nNumberOfLinks == 0)
				return FALSE ;
			const auto r3x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile ((&tmp.mFile[0]) ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
			} ,[] (VREF<HANDLE> me) {
				if (me == NULL)
					return ;
				CloseHandle (me) ;
			}) ;
			zeroize (rax[1]) ;
			const auto r4x = GetFileInformationByHandle (r3x.self ,(&rax[1])) ;
			if (r4x == 0)
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
exports auto FILE_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait DIRECTORY_IMPLHOLDER_HELP ;

template <class DEPEND>
trait DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SSIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SSIZE ;
	using Holder = typename DIRECTORY_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		String<STR> mFile ;
		ArrayList<INDEX> mQuote ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		void init_file (CREF<String<STR>> file_) override {
			mHeap.mFile = move (file_) ;
			update_quote () ;
		}

		Auto native () const override {
			return CRef<HEAP>::reference (mHeap) ;
		}

		String<STR> path () const override {
			String<STR> ret = String<STR> (FILE_NAME_SSIZE::value) ;
			const auto r2x = invoke ([&] () {
				if (mHeap.mQuote.length () == 0)
					return ZERO ;
				return mHeap.mQuote[mHeap.mQuote.length () - 1] ;
			}) ;
			for (auto &&i : iter (0 ,r2x))
				ret[i] = mHeap.mFile[i] ;
			return move (ret) ;
		}

		String<STR> name () const override {
			String<STR> ret = String<STR> (FILE_NAME_SSIZE::value) ;
			const auto r2x = invoke ([&] () {
				if (mHeap.mQuote.length () < 2)
					return ZERO ;
				return mHeap.mQuote[mHeap.mQuote.length () - 2] + 1 ;
			}) ;
			const auto r3x = mHeap.mQuote[mHeap.mQuote.length () - 1] ;
			for (auto &&i : iter (r2x ,r3x)) {
				INDEX ix = i - r2x ;
				ret[ix] = mHeap.mFile[i] ;
			}
			return move (ret) ;
		}

		String<STR> full_name () const override {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}

		LENGTH depth () const override {
			unimplemented () ;
			return bad (TYPEAS<LENGTH>::id) ;
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

		void child_from (CREF<Holder> a ,CREF<String<STR>> file_) override {
			unimplemented () ;
		}

		BOOL find () const override {
			if (mHeap.mFile.empty ())
				return FALSE ;
			const auto r1x = GetFileAttributes ((&mHeap.mFile[0])) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (CSC::CHAR (r1x & FILE_ATTRIBUTE_DIRECTORY) == CSC::CHAR (0X00))
				return FALSE ;
			return TRUE ;
		}

		BOOL lock () const override {
			unimplemented () ;
			return bad (TYPEAS<BOOL>::id) ;
		}

		void build () const override {
			unimplemented () ;
		}

		void erase () const override {
			RemoveDirectory ((&mHeap.mFile[0])) ;
		}

		void clear () const override {
			unimplemented () ;
		}

	private:
		void update_quote () {
			const auto r1x = mHeap.mFile.length () ;
			mHeap.mQuote = ArrayList<INDEX> (mHeap.mFile.length ()) ;
			INDEX ix = 0 ;
			while (TRUE) {
				if (ix >= r1x)
					break ;
				if ifswitch (TRUE) {
					if (mHeap.mFile[ix] != STR ('\\'))
						if (mHeap.mFile[ix] != STR ('/'))
							discard ;
					mHeap.mQuote.add (ix) ;
				}
				ix++ ;
			}
			mHeap.mQuote.add (r1x) ;
			mHeap.mQuote.remap () ;
		}
	} ;
} ;

template <>
exports auto DIRECTORY_HELP<DEPEND ,ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;