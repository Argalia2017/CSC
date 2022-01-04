#pragma once

#ifndef __CSC_FILESYSTEM__
#error "б╞(д├бузебу ;)д├ : require 'csc_filesystem.hpp'"
#endif

#ifndef __CSC_SYSTEM_WINDOWS__
#error "б╞(д├бузебу ;)д├
#endif

namespace CSC {
namespace DEBUGGER {
template <class...>
trait FUNCTION_file_max_size_HELP ;

template <class DEPEND>
trait FUNCTION_file_max_size_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL32<DEPEND>>> {
	struct FUNCTION_file_max_size {
		inline LENGTH operator() () const {
			return LENGTH (VAL32_MAX) ;
		}
	} ;
} ;

template <class DEPEND>
trait FUNCTION_file_max_size_HELP<DEPEND ,REQUIRE<MACRO_CONFIG_VAL64<DEPEND>>> {
	struct FUNCTION_file_max_size {
		inline LENGTH operator() () const {
			return LENGTH (VAL32_MAX) * 2 ;
		}
	} ;
} ;

struct FUNCTION_file_max_size {
	inline LENGTH operator() () const {
		using R1X = typename FUNCTION_file_max_size_HELP<DEPEND ,ALWAYS>::FUNCTION_file_max_size ;
		static constexpr auto M_INVOKE = R1X () ;
		return M_INVOKE () ;
	} ;
} ;

static constexpr auto file_max_size = FUNCTION_file_max_size () ;

template <class...>
trait FILE_IMPLHOLDER_HELP ;

template <class DEPEND>
trait FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SIZE ;
	using FILE_RETRY_TIMES = RANK2 ;
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		String<STR> mFile ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		void init_file (CREF<String<STR>> file) {
			mHeap.mFile = move (file) ;
		}

		Auto native () const {
			return CRef<HEAP>::reference (mHeap) ;
		}

		String<STR> path () const {
			String<STR> ret = String<STR> (FILE_NAME_SIZE::value) ;
			const auto r1x = mHeap.mFile.length () ;
			const auto r2x = mHeap.mFile.find (STR ('\\') ,0 ,r1x ,FALSE) ;
			const auto r3x = mHeap.mFile.find (STR ('/') ,0 ,r1x ,FALSE) ;
			const auto r4x = max_of (r2x ,r3x ,ZERO) ;
			for (auto &&i : iter (0 ,r4x))
				ret[i] = mHeap.mFile[i] ;
			return move (ret) ;
		}

		String<STR> name () const {
			String<STR> ret = String<STR> (FILE_NAME_SIZE::value) ;
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

		String<STR> full_name () const {
			return mHeap.mFile ;
		}

		VarBuffer<BYTE> load () const {
			const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
				me = CreateFile (mHeap.mFile.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
				assume (me != NULL) ;
			} ,[] (HANDLE &me) {
				CloseHandle (me) ;
			}) ;
			const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
			assume (r2x >= 0) ;
			assume (r2x <= file_max_size ()) ;
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

		void load (VREF<RegBuffer<BYTE>> item) const {
			const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
				me = CreateFile (mHeap.mFile.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
				if (me == INVALID_HANDLE_VALUE)
					me = NULL ;
				assume (me != NULL) ;
			} ,[] (HANDLE &me) {
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

		void save (CREF<RegBuffer<BYTE>> item) const {
			assume (item.size () <= file_max_size ()) ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile (mHeap.mFile.raw ().self ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
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

		CREF<RegBuffer<BYTE>> load_assert (CREF<FLAG> uuid) const {
			const auto r1x = FindResource (NULL ,MAKEINTRESOURCE (uuid) ,TEXT ("BIN")) ;
			assume (r1x != NULL) ;
			const auto r2x = LoadResource (NULL ,r1x) ;
			assume (r2x != NULL) ;
			const auto r3x = FLAG (LockResource (r2x)) ;
			assume (r3x != ZERO) ;
			const auto r4x = LENGTH (SizeofResource (NULL ,r1x)) ;
			assume (r4x >= 0) ;
			unimplemented () ;
			return bad (TYPEAS<CREF<RegBuffer<BYTE>>>::id) ;
		}

		BOOL find () const {
			const auto r1x = GetFileAttributes (mHeap.mFile.raw ().self) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (CSC::CHAR (r1x & FILE_ATTRIBUTE_DIRECTORY) != CSC::CHAR (0X00))
				return FALSE ;
			return TRUE ;
		}

		void erase () const {
			DeleteFile (mHeap.mFile.raw ().self) ;
		}

		void copy_from (CREF<Holder> that) const {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			const auto r1x = find () ;
			assume (ifnot (r1x)) ;
			CopyFile (tmp.mFile.raw ().self ,mHeap.mFile.raw ().self ,TRUE) ;
		}

		void move_from (CREF<Holder> that) const {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			const auto r1x = find () ;
			assume (ifnot (r1x)) ;
			MoveFile (tmp.mFile.raw ().self ,mHeap.mFile.raw ().self) ;
		}

		void link_from (CREF<Holder> that) const {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			const auto r1x = find () ;
			assume (ifnot (r1x)) ;
			CreateHardLink (mHeap.mFile.raw ().self ,tmp.mFile.raw ().self ,NULL) ;
		}

		BOOL identical (CREF<Holder> that) const {
			using R1X = typename FILE_IMPLHOLDER_HELP<DEPEND ,ALWAYS>::HEAP ;
			auto &&tmp = that.native ().as (TYPEAS<CRef<R1X>>::id).self ;
			auto rax = ARRAY2<BY_HANDLE_FILE_INFORMATION> () ;
			const auto r1x = UniqueRef<HANDLE> ([&] (VREF<HANDLE> me) {
				me = CreateFile (mHeap.mFile.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
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
				me = CreateFile (tmp.mFile.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
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

template <class...>
trait DIRECTORY_IMPLHOLDER_HELP ;

template <class DEPEND>
trait DIRECTORY_IMPLHOLDER_HELP<DEPEND ,ALWAYS> {
	using FILE_NAME_SIZE = typename FILE_HELP<DEPEND ,ALWAYS>::FILE_NAME_SIZE ;
	using Holder = typename FILE_HELP<DEPEND ,ALWAYS>::Holder ;

	struct HEAP {
		String<STR> mFile ;
		Cell<CRef<Array<INDEX>>> mQuote ;
	} ;

	class ImplHolder implement Holder {
	private:
		HEAP mHeap ;

	public:
		void init_file (CREF<String<STR>> file) {
			mHeap.mFile = move (file) ;
			mHeap.mQuote.initialize () ;
		}

		Auto native () const {
			return CRef<HEAP>::reference (mHeap) ;
		}

		String<STR> path () const {
			String<STR> ret = String<STR> (FILE_NAME_SIZE::value) ;
			update_quote () ;
			const auto r1x = mHeap.mQuote.fetch () ;
			const auto r2x = invoke ([&] () {
				if (r1x->length () == 0)
					return ZERO ;
				return r1x.self[r1x->length () - 1] ;
			}) ;
			for (auto &&i : iter (0 ,r2x))
				ret[i] = mHeap.mFile[i] ;
			return move (ret) ;
		}

		String<STR> name () const {
			String<STR> ret = String<STR> (FILE_NAME_SIZE::value) ;
			update_quote () ;
			const auto r1x = mHeap.mQuote.fetch () ;
			const auto r2x = invoke ([&] () {
				if (r1x->length () < 2)
					return ZERO ;
				return r1x.self[r1x->length () - 2] + 1 ;
			}) ;
			const auto r3x = r1x.self[r1x->length () - 1] ;
			for (auto &&i : iter (r2x ,r3x)) {
				INDEX ix = i - r2x ;
				ret[ix] = mHeap.mFile[i] ;
			}
			return move (ret) ;
		}

		String<STR> full_name () const {
			unimplemented () ;
			return bad (TYPEAS<String<STR>>::id) ;
		}

		LENGTH depth () const {
			unimplemented () ;
			return bad (TYPEAS<LENGTH>::id) ;
		}

		void parent_from (CREF<Holder> a) {
			unimplemented () ;
		}

		void brother_from (CREF<Holder> a) {
			unimplemented () ;
		}

		void child_from (CREF<Holder> a ,CREF<String<STR>> file) {
			unimplemented () ;
		}

		LENGTH child_size () const {
			unimplemented () ;
			return ZERO ;
		}

		void child_from (CREF<Holder> a ,CREF<INDEX> index) {
			unimplemented () ;
		}

		BOOL find () const {
			const auto r1x = GetFileAttributes (mHeap.mFile.raw ().self) ;
			if (r1x == INVALID_FILE_ATTRIBUTES)
				return FALSE ;
			if (CSC::CHAR (r1x & FILE_ATTRIBUTE_DIRECTORY) == CSC::CHAR (0X00))
				return FALSE ;
			return TRUE ;
		}

		BOOL lock () const {
			unimplemented () ;
			return bad (TYPEAS<BOOL>::id) ;
		}

		void build () const {
			unimplemented () ;
		}

		void erase () const {
			RemoveDirectory (mHeap.mFile.raw ().self) ;
		}

		void clear () const {
			unimplemented () ;
		}

	private:
		void update_quote () const {
			if (mHeap.mQuote.exist ())
				return ;
			const auto r1x = mHeap.mFile.length () ;
			const auto r2x = invoke ([&] () {
				LENGTH ret = 0 ;
				INDEX ix = 0 ;
				while (TRUE) {
					if (ix >= r1x)
						break ;
					if ifswitch (TRUE) {
						if (mHeap.mFile[ix] != STR ('\\'))
							if (mHeap.mFile[ix] != STR ('/'))
								discard ;
						ret++ ;
					}
					ix++ ;
				}
				return move (ret) ;
			}) ;
			auto rax = Array<INDEX> (r2x + 1) ;
			INDEX ix = 0 ;
			INDEX jx = 0 ;
			while (TRUE) {
				if (ix >= r1x)
					break ;
				if (jx >= r2x)
					break ;
				if ifswitch (TRUE) {
					if (mHeap.mFile[ix] != STR ('\\'))
						if (mHeap.mFile[ix] != STR ('/'))
							discard ;
					rax[jx] = ix ;
					jx++ ;
				}
				ix++ ;
			}
			assert (ix == r2x) ;
			rax[r2x] = r1x ;
			mHeap.mQuote.store (CRef<Array<INDEX>>::make (move (rax))) ;
		}
	} ;
} ;
} ;
} ;