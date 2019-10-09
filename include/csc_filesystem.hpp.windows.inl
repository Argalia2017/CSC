#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#undef discard
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

#ifdef __CSC__
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("discard")
#endif

namespace CSC {
inline namespace S {
inline exports AutoBuffer<BYTE> _LOADFILE_ (const String<STR> &file) popping {
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		CloseHandle (me) ;
	}) ;
	const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
	_DYNAMIC_ASSERT_ (r2x >= 0 && r2x < VAR32_MAX) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r2x) ;
	auto rax = VARY () ;
	rax = VARY (0) ;
	const auto r3x = ReadFile (r1x.self ,ret.self ,VARY (r2x) ,&rax ,NULL) ;
	_DYNAMIC_ASSERT_ (r3x) ;
	return std::move (ret) ;
}

inline exports void _LOADFILE_ (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping {
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		CloseHandle (me) ;
	}) ;
	const auto r2x = LENGTH (GetFileSize (r1x.self ,NULL)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= data.size ()) ;
	auto rax = VARY () ;
	rax = VARY (0) ;
	const auto r3x = ReadFile (r1x.self ,data ,VARY (r2x) ,&rax ,NULL) ;
	_DYNAMIC_ASSERT_ (r3x) ;
	_MEMFILL_ (PTRTOARR[&data.self[r2x]] ,(data.size () - r2x) ,BYTE (0X00)) ;
}

inline exports void _SAVEFILE_ (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		CloseHandle (me) ;
	}) ;
	auto rax = VARY () ;
	rax = VARY (0) ;
	const auto r2x = WriteFile (r1x.self ,data.self ,VARY (data.size ()) ,&rax ,NULL) ;
	_DYNAMIC_ASSERT_ (r2x) ;
}

inline exports PhanBuffer<const BYTE> _LOADASSETFILE_ (FLAG resource) popping {
	const auto r1x = FindResource (NULL ,MAKEINTRESOURCE (resource) ,_PCSTR_ ("BIN")) ;
	_DYNAMIC_ASSERT_ (r1x != NULL) ;
	const auto r2x = LoadResource (NULL ,r1x) ;
	_DYNAMIC_ASSERT_ (r2x != NULL) ;
	const auto r3x = LockResource (r2x) ;
	_DYNAMIC_ASSERT_ (r3x != NULL) ;
	const auto r4x = LENGTH (SizeofResource (NULL ,r1x)) ;
	_DYNAMIC_ASSERT_ (r4x >= 0) ;
	auto &r5y = _LOAD_<ARR<BYTE>> (r3x) ;
	return PhanBuffer<const BYTE>::make (r5y ,r4x) ;
}

inline exports BOOL _FINDFILE_ (const String<STR> &file) popping {
	const auto r1x = GetFileAttributes (file.raw ().self) ;
	if (r1x == INVALID_FILE_ATTRIBUTES)
		return FALSE ;
	if ((r1x & FILE_ATTRIBUTE_DIRECTORY) != 0)
		return FALSE ;
	return TRUE ;
}

inline exports void _ERASEFILE_ (const String<STR> &file) {
	DeleteFile (file.raw ().self) ;
}

inline exports void _COPYFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _FINDFILE_ (dst_file) ;
	_DYNAMIC_ASSERT_ (!r1x) ;
	CopyFile (src_file.raw ().self ,dst_file.raw ().self ,TRUE) ;
}

inline exports void _MOVEFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _FINDFILE_ (dst_file) ;
	_DYNAMIC_ASSERT_ (!r1x) ;
	MoveFile (src_file.raw ().self ,dst_file.raw ().self) ;
}

inline exports void _LINKFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _FINDFILE_ (dst_file) ;
	_DYNAMIC_ASSERT_ (!r1x) ;
	CreateHardLink (dst_file.raw ().self ,src_file.raw ().self ,NULL) ;
}

inline exports BOOL _GUARDFILE_ (const String<STR> &file) popping {
	if (_FINDFILE_ (file))
		return FALSE ;
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,0 ,0 ,NULL ,CREATE_NEW ,(FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN) ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
	}) ;
	if (r1x.self == NULL)
		return FALSE ;
	return TRUE ;
}

inline exports BOOL _IDENTICALFILE_ (const String<STR> &file1 ,const String<STR> &file2) popping {
	auto rax = ARRAY2<BY_HANDLE_FILE_INFORMATION> () ;
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file1.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
	}) ;
	_ZERO_ (rax[0]) ;
	const auto r2x = GetFileInformationByHandle (r1x.self ,&rax[0]) ;
	if (r2x == 0)
		return FALSE ;
	if (rax[0].nNumberOfLinks == 0)
		return FALSE ;
	const auto r3x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file2.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
	}) ;
	_ZERO_ (rax[1]) ;
	const auto r4x = GetFileInformationByHandle (r3x.self ,&rax[1]) ;
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

inline exports String<STR> _PARSEFILEPATH_ (const String<STR> &file) {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = _MEMRCHR_ (r2x.self ,r1x ,STR ('\\')) ;
	const auto r4x = _MEMRCHR_ (r2x.self ,r1x ,STR ('/')) ;
	const auto r5x = _MAXOF_ (r3x ,r4x ,VAR_ZERO) ;
	_MEMCOPY_ (ret.raw ().self ,r2x.self ,r5x) ;
	return std::move (ret) ;
}

inline exports String<STR> _PARSEFILENAME_ (const String<STR> &file) {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = _MEMRCHR_ (r2x.self ,r1x ,STR ('\\')) ;
	const auto r4x = _MEMRCHR_ (r2x.self ,r1x ,STR ('/')) ;
	const auto r5x = _MAX_ (r3x ,r4x) + 1 ;
	_MEMCOPY_ (ret.raw ().self ,PTRTOARR[&r2x.self[r5x]] ,(r1x - r5x)) ;
	return std::move (ret) ;
}

inline exports Deque<String<STR>> _DECOUPLEPATHNAME_ (const String<STR> &file) {
	const auto r1x = _SWITCH_ (
		(file.empty ()) ? (PhanBuffer<const STR> ()) :
		(file.raw ())) ;
	auto ris = TextReader<STR> (r1x) ;
	ris.attr ().modify_space (STR ('\\') ,0) ;
	ris.attr ().modify_space (STR ('/') ,0) ;
	auto rax = STR () ;
	Deque<String<STR>> ret ;
	INDEX ix = ret.insert () ;
	ris.copy () >> rax ;
	if (ris.attr ().varify_space (rax))
		ris >> rax ;
	while (TRUE) {
		ris >> ret[ix] ;
		if (ret[ix].empty ())
			break ;
		ix = ret.insert () ;
		ris >> rax ;
		if (rax == ris.attr ().varify_ending_item ())
			break ;
		_DYNAMIC_ASSERT_ (ris.attr ().varify_space (rax)) ;
	}
	ret.pop () ;
	ris >> _EOS_ ;
	return std::move (ret) ;
}

inline exports String<STR> _WORKINGPATH_ () {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	GetCurrentDirectory (VARY (ret.size ()) ,ret.raw ().self) ;
	ret += _PCSTR_ ("\\") ;
	return std::move (ret) ;
}

inline Deque<INDEX> _inline_RELATIVEPATHNAME_ (const Deque<String<STR>> &path_name) {
	Deque<INDEX> ret = Deque<INDEX> (path_name.length ()) ;
	for (INDEX i = 0 ,ie = path_name.length () ; i < ie ; i++) {
		INDEX ix = path_name.access (i) ;
		if (path_name[ix] == _PCSTR_ ("."))
			continue ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (ret.empty ())
				discard ;
			if (!(path_name[ix] == _PCSTR_ ("..")))
				discard ;
			if (!(path_name[ret[ret.tail ()]] != _PCSTR_ ("..")))
				discard ;
			ret.pop () ;
		}
		if SWITCH_CASE (fax) {
			ret.add (ix) ;
		}
	}
	return std::move (ret) ;
}

inline exports String<STR> _ABSOLUTEPATH_ (const String<STR> &path) {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _DECOUPLEPATHNAME_ (path) ;
	const auto r2x = _inline_RELATIVEPATHNAME_ (r1x) ;
	auto fax = FALSE ;
	if SWITCH_CASE (fax) {
		const auto r3x = BOOL (path.size () >= 1 && path[0] == STR ('\\')) ;
		const auto r4x = BOOL (path.size () >= 1 && path[0] == STR ('/')) ;
		if (!r3x && !r4x)
			discard ;
		ret += _PCSTR_ ("\\") ;
	}
	if SWITCH_CASE (fax) {
		const auto r5x = BOOL (r1x.length () >= 1 && r1x[r1x.access (0)] == _PCSTR_ (".")) ;
		const auto r6x = BOOL (r1x.length () >= 1 && r1x[r1x.access (0)] == _PCSTR_ ("..")) ;
		if (!r5x && !r6x)
			discard ;
		//@debug: not absolute path really
		ret += _WORKINGPATH_ () ;
	}
	for (INDEX i = 0 ,ie = r2x.length () ; i < ie ; i++) {
		if (i > 0)
			ret += _PCSTR_ ("\\") ;
		INDEX ix = r2x[r2x.access (i)] ;
		ret += r1x[ix] ;
	}
	if SWITCH_ONCE (TRUE) {
		const auto r7x = ret.length () ;
		if (r7x < 1)
			discard ;
		if (ret[r7x - 1] == STR ('\\'))
			discard ;
		if (ret[r7x - 1] == STR ('/'))
			discard ;
		ret += _PCSTR_ ("\\") ;
	}
	return std::move (ret) ;
}

inline exports const String<STR> &_MODULEFILEPATH_ () popping {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	return _CACHE_ ([] () {
		String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
		GetModuleFileName (NULL ,ret.raw ().self ,VARY (ret.size ())) ;
		ret = _PARSEFILEPATH_ (ret) ;
		ret += _PCSTR_ ("\\") ;
		return std::move (ret) ;
	}) ;
}

inline exports const String<STR> &_MODULEFILENAME_ () popping {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	return _CACHE_ ([] () {
		String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
		GetModuleFileName (NULL ,ret.raw ().self ,VARY (ret.size ())) ;
		ret = _PARSEFILENAME_ (ret) ;
		return std::move (ret) ;
	}) ;
}

inline exports BOOL _FINDDIRECTORY_ (const String<STR> &dire) popping {
	const auto r1x = GetFileAttributes (dire.raw ().self) ;
	if (r1x == INVALID_FILE_ATTRIBUTES)
		return FALSE ;
	if ((r1x & FILE_ATTRIBUTE_DIRECTORY) == 0)
		return FALSE ;
	return TRUE ;
}

inline exports void _BUILDDIRECTORY_ (const String<STR> &dire) {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	if (_FINDDIRECTORY_ (dire))
		return ;
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _DECOUPLEPATHNAME_ (_ABSOLUTEPATH_ (dire)) ;
	_DEBUG_ASSERT_ (r1x.length () >= 1) ;
	if SWITCH_ONCE (TRUE) {
		const auto r2x = BOOL (dire.size () >= 1 && dire[0] == STR ('\\')) ;
		const auto r3x = BOOL (dire.size () >= 1 && dire[0] == STR ('/')) ;
		if (!r2x && !r3x)
			discard ;
		rax += _PCSTR_ ("\\") ;
	}
	for (INDEX i = 0 ,ie = r1x.length () ; i < ie ; i++) {
		if (i > 0)
			rax += _PCSTR_ ("\\") ;
		INDEX ix = r1x.access (i) ;
		rax += r1x[ix] ;
		const auto r4x = r1x[ix].length () ;
		if (r4x > 1 && r1x[ix][r4x - 1] == STR (':'))
			continue ;
		CreateDirectory (rax.raw ().self ,NULL) ;
	}
}

inline exports void _ERASEDIRECTORY_ (const String<STR> &dire) {
	RemoveDirectory (dire.raw ().self) ;
}

//@warn: recursive call with junction(symbolic link) may cause endless loop
inline exports void _ENUMDIRECTORY_ (const String<STR> &dire ,Deque<String<STR>> &file_list ,Deque<String<STR>> &dire_list) popping {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	rax += dire ;
	rax += _PCSTR_ ("\\") ;
	const auto r1x = rax.length () ;
	auto rbx = WIN32_FIND_DATA () ;
	const auto r2x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		rax += _PCSTR_ ("*.*") ;
		_ZERO_ (rbx) ;
		me = FindFirstFile (rax.raw ().self ,&rbx) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		rax[r1x] = 0 ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		FindClose (me) ;
	}) ;
	if (r2x.self == NULL)
		return ;
	while (TRUE) {
		if (rbx.cFileName[0] == 0)
			break ;
		if SWITCH_ONCE (TRUE) {
			const auto r3x = String<STR> (PTRTOARR[rbx.cFileName]) ;
			if (r3x == _PCSTR_ ("."))
				discard ;
			if (r3x == _PCSTR_ (".."))
				discard ;
			auto &r4y = _SWITCH_ (
				((rbx.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ? dire_list :
				file_list) ;
			rax += r3x ;
			r4y.add (rax) ;
		}
		rax[r1x] = 0 ;
		rbx.cFileName[0] = 0 ;
		FindNextFile (r2x.self ,&rbx) ;
	}
}

inline exports void _CLEARDIRECTORY_ (const String<STR> &dire) {
	using DEFAULT_EXPANDLIMIT_SIZE = ARGC<65536> ;
	auto rax = Deque<PACK<String<STR> ,BOOL>> () ;
	auto rbx = ARRAY2<Deque<String<STR>>> () ;
	rbx[0].clear () ;
	rbx[1].clear () ;
	_ENUMDIRECTORY_ (dire ,rbx[0] ,rbx[1]) ;
	for (auto &&i : rbx[0])
		_ERASEFILE_ (i) ;
	_DYNAMIC_ASSERT_ (rax.length () + rbx[1].length () < DEFAULT_EXPANDLIMIT_SIZE::value) ;
	for (auto &&i : rbx[1])
		rax.add (PACK<String<STR> ,BOOL> {i ,FALSE}) ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		INDEX ix = rax.tail () ;
		_ERASEDIRECTORY_ (rax[ix].P1) ;
		auto fax = FALSE ;
		if SWITCH_CASE (fax) {
			if (!(rax[ix].P2))
				discard ;
			rax.pop () ;
		}
		if SWITCH_CASE (fax) {
			rbx[0].clear () ;
			rbx[1].clear () ;
			_ENUMDIRECTORY_ (rax[ix].P1 ,rbx[0] ,rbx[1]) ;
			for (auto &&i : rbx[0])
				_ERASEFILE_ (i) ;
			_DYNAMIC_ASSERT_ (rax.length () + rbx[1].length () < DEFAULT_EXPANDLIMIT_SIZE::value) ;
			for (auto &&i : rbx[1])
				rax.add (PACK<String<STR> ,BOOL> {i ,FALSE}) ;
			rax[ix].P2 = TRUE ;
		}
	}
}
} ;

class StreamLoader::Implement final :private Interface {
private:
	UniqueRef<HANDLE> mReadFile ;
	UniqueRef<HANDLE> mWriteFile ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		mWriteFile = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,GENERIC_WRITE ,(FILE_SHARE_READ | FILE_SHARE_WRITE) ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
			SetFilePointer (me ,0 ,NULL ,FILE_END) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		mReadFile = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,GENERIC_READ ,(FILE_SHARE_READ | FILE_SHARE_WRITE) ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
	}

	void read (const PhanBuffer<BYTE> &data) popping {
		_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		const auto r1x = ReadFile (mReadFile ,data.self ,VARY (data.size ()) ,&rax ,NULL) ;
		const auto r2x = _SWITCH_ (
			(r1x != 0) ? (LENGTH (rax)) :
			0) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x >= 0 && r2x < VAR32_MAX) ;
		if (data.size () - r2x == 0)
			return ;
		_MEMFILL_ (PTRTOARR[&data[r2x]] ,(data.size () - r2x) ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
		auto rax = VARY () ;
		rax = VARY (0) ;
		const auto r1x = WriteFile (mWriteFile ,data.self ,VARY (data.size ()) ,&rax ,NULL) ;
		const auto r2x = _SWITCH_ (
			(r1x != 0) ? (LENGTH (rax)) :
			0) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
	}

	void flush () {
		FlushFileBuffers (mWriteFile) ;
	}
} ;

inline exports StreamLoader::StreamLoader (const String<STR> &file) {
	mThis = AnyRef<Implement>::make (file) ;
}

inline exports void StreamLoader::read (const PhanBuffer<BYTE> &data) popping {
	mThis.rebind<Implement> ()->read (data) ;
}

inline exports void StreamLoader::write (const PhanBuffer<const BYTE> &data) {
	mThis.rebind<Implement> ()->write (data) ;
}

inline exports void StreamLoader::flush () {
	mThis.rebind<Implement> ()->flush () ;
}

class BufferLoader::Implement final :private Interface {
private:
	class Holder {
	private:
		friend Implement ;
		Monostate<UniqueRef<HANDLE>> mFile ;
		Monostate<UniqueRef<HANDLE>> mMapping ;
		Monostate<UniqueRef<PhanBuffer<BYTE>>> mBuffer ;
	} ;

private:
	UniqueRef<Holder> mThis ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		update_reset () ;
		auto &r1y = mThis->mFile.self ;
		auto &r2y = mThis->mMapping.self ;
		auto &r3y = mThis->mBuffer.self ;
		r1y = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		const auto r4x = LENGTH (GetFileSize (r1y.self ,NULL)) ;
		_DYNAMIC_ASSERT_ (r4x >= 0 && r4x < VAR32_MAX) ;
		r2y = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFileMapping (r1y.self ,NULL ,PAGE_READONLY ,0 ,VARY (r4x) ,NULL) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		r3y = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r5x = MapViewOfFile (r2y.self ,FILE_MAP_READ ,0 ,0 ,r4x) ;
			_DYNAMIC_ASSERT_ (r5x != NULL) ;
			auto &r6y = _LOAD_<ARR<BYTE>> (r5x) ;
			me = PhanBuffer<BYTE>::make (r6y ,r4x) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	explicit Implement (const String<STR> &file ,LENGTH file_len) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		update_reset () ;
		auto &r1y = mThis->mFile.self ;
		auto &r2y = mThis->mMapping.self ;
		auto &r3y = mThis->mBuffer.self ;
		r1y = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,(GENERIC_READ | GENERIC_WRITE) ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		r2y = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFileMapping (r1y.self ,NULL ,PAGE_READWRITE ,0 ,VARY (file_len) ,NULL) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		r3y = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r4x = MapViewOfFile (r2y.self ,(FILE_MAP_READ | FILE_MAP_WRITE) ,0 ,0 ,file_len) ;
			_DYNAMIC_ASSERT_ (r4x != NULL) ;
			auto &r5y = _LOAD_<ARR<BYTE>> (r4x) ;
			me = PhanBuffer<BYTE>::make (r5y ,file_len) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	explicit Implement (const String<STR> &file ,BOOL cache) {
		_DEBUG_ASSERT_ (cache) ;
		update_reset () ;
		auto &r1y = mThis->mMapping.self ;
		auto &r2y = mThis->mBuffer.self ;
		r1y = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = OpenFileMapping (FILE_MAP_READ ,FALSE ,file.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		r2y = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r3x = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
				const auto r4x = MapViewOfFile (r1y.self ,FILE_MAP_READ ,0 ,0 ,0) ;
				_DYNAMIC_ASSERT_ (r4x != NULL) ;
				auto &r5y = _LOAD_<ARR<BYTE>> (r4x) ;
				me = PhanBuffer<BYTE>::make (r5y ,1) ;
			} ,[] (PhanBuffer<BYTE> &me) {
				UnmapViewOfFile (me.self) ;
			}) ;
			auto rax = MEMORY_BASIC_INFORMATION () ;
			_ZERO_ (rax) ;
			const auto r6x = VirtualQuery (r3x->self ,&rax ,_SIZEOF_ (MEMORY_BASIC_INFORMATION)) ;
			_DYNAMIC_ASSERT_ (r6x == _SIZEOF_ (MEMORY_BASIC_INFORMATION)) ;
			const auto r7x = MapViewOfFile (r1y.self ,FILE_MAP_READ ,0 ,0 ,rax.RegionSize) ;
			_DYNAMIC_ASSERT_ (r7x != NULL) ;
			auto &r8y = _LOAD_<ARR<BYTE>> (r7x) ;
			me = PhanBuffer<BYTE>::make (r8y ,LENGTH (rax.RegionSize)) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	explicit Implement (const String<STR> &file ,LENGTH file_len ,BOOL cache) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cache) ;
		update_reset () ;
		auto &r1y = mThis->mMapping.self ;
		auto &r2y = mThis->mBuffer.self ;
		r1y = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,VARY (file_len) ,file.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			CloseHandle (me) ;
		}) ;
		r2y = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r3x = MapViewOfFile (r1y.self ,(FILE_MAP_READ | FILE_MAP_WRITE) ,0 ,0 ,file_len) ;
			_DYNAMIC_ASSERT_ (r3x != NULL) ;
			auto &r4y = _LOAD_<ARR<BYTE>> (r3x) ;
			me = PhanBuffer<BYTE>::make (r4y ,file_len) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	PhanBuffer<BYTE> watch () & {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		return PhanBuffer<BYTE>::make (mThis->mBuffer.self.self) ;
	}

	PhanBuffer<const BYTE> watch () const & {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		return PhanBuffer<const BYTE>::make (mThis->mBuffer.self.self) ;
	}

	PhanBuffer<BYTE> watch () && = delete ;

	void flush () {
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		FlushViewOfFile (mThis->mBuffer.self->self ,mThis->mBuffer.self->size ()) ;
	}

private:
	void update_reset () {
		mThis = UniqueRef<Holder> ([&] (Holder &me) {
			me.mFile.self = UniqueRef<HANDLE> () ;
			me.mMapping.self = UniqueRef<HANDLE> () ;
			me.mBuffer.self = UniqueRef<PhanBuffer<BYTE>> () ;
		} ,[] (Holder &me) {
			me.mBuffer.self = UniqueRef<PhanBuffer<BYTE>> () ;
			me.mMapping.self = UniqueRef<HANDLE> () ;
			me.mFile.self = UniqueRef<HANDLE> () ;
		}) ;
	}
} ;

inline exports BufferLoader::BufferLoader (const String<STR> &file) {
	mThis = AnyRef<Implement>::make (file) ;
}

inline exports BufferLoader::BufferLoader (const String<STR> &file ,LENGTH file_len) {
	mThis = AnyRef<Implement>::make (file ,file_len) ;
}

inline exports BufferLoader::BufferLoader (const String<STR> &file ,BOOL cache) {
	mThis = AnyRef<Implement>::make (file ,cache) ;
}

inline exports BufferLoader::BufferLoader (const String<STR> &file ,LENGTH file_len ,BOOL cache) {
	mThis = AnyRef<Implement>::make (file ,file_len ,cache) ;
}

inline exports PhanBuffer<BYTE> BufferLoader::watch () & {
	return mThis.rebind<Implement> ()->watch () ;
}

inline exports PhanBuffer<const BYTE> BufferLoader::watch () const & {
	return mThis.rebind<Implement> ()->watch () ;
}

inline exports void BufferLoader::flush () {
	mThis.rebind<Implement> ()->flush () ;
}

class FileSystemService::Implement :public FileSystemService::Abstract {
public:
	void startup () override {
		_STATIC_WARNING_ ("noop") ;
	}

	void shutdown () override {
		_STATIC_WARNING_ ("noop") ;
	}
} ;

inline exports FileSystemService::FileSystemService () {
	mThis = StrongRef<Implement>::make () ;
}
} ;