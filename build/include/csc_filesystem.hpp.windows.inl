#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef import
#undef export
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("import")
#pragma pop_macro ("export")
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("import")
#pragma push_macro ("export")
#define self to ()
#define implicit
#define popping
#define import extern
#define export
#endif

namespace CSC {
inline namespace S {
inline export AutoBuffer<BYTE> _LOADFILE_ (const String<STR> &file) popping {
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		_DEBUG_ASSERT_ (me != NULL) ;
		CloseHandle (me) ;
	}) ;
	const auto r2x = LENGTH (GetFileSize (r1x ,NULL)) ;
	_DYNAMIC_ASSERT_ (r2x >= 0 && r2x < VAR32_MAX) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r2x) ;
	auto rax = VARY () ;
	const auto r3x = ReadFile (r1x ,ret.self ,VARY (r2x) ,&(rax = VARY (0)) ,NULL) ;
	_DYNAMIC_ASSERT_ (r3x) ;
	return std::move (ret) ;
}

inline export void _LOADFILE_ (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping {
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		_DEBUG_ASSERT_ (me != NULL) ;
		CloseHandle (me) ;
	}) ;
	const auto r2x = LENGTH (GetFileSize (r1x ,NULL)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= data.size ()) ;
	auto rax = VARY () ;
	const auto r3x = ReadFile (r1x ,data ,VARY (r2x) ,&(rax = VARY (0)) ,NULL) ;
	_DYNAMIC_ASSERT_ (r3x) ;
}

inline export void _SAVEFILE_ (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	_DEBUG_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file.raw ().self ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		_DEBUG_ASSERT_ (me != NULL) ;
		CloseHandle (me) ;
	}) ;
	auto rax = VARY () ;
	const auto r2x = WriteFile (r1x ,data ,VARY (data.size ()) ,&(rax = VARY (0)) ,NULL) ;
	_DYNAMIC_ASSERT_ (r2x) ;
}

inline export PhanBuffer<const BYTE> _LOADASSETFILE_ (FLAG resource) popping {
	const auto r1x = FindResource (NULL ,MAKEINTRESOURCE (resource) ,_PCSTR_ ("BIN")) ;
	_DYNAMIC_ASSERT_ (r1x != NULL) ;
	const auto r2x = LoadResource (NULL ,r1x) ;
	_DYNAMIC_ASSERT_ (r2x != NULL) ;
	const auto r3x = LockResource (r2x) ;
	_DYNAMIC_ASSERT_ (r3x != NULL) ;
	const auto r4x = LENGTH (SizeofResource (NULL ,r1x)) ;
	_DYNAMIC_ASSERT_ (r4x >= 0) ;
	return PhanBuffer<const BYTE>::make (_LOAD_<ARR<BYTE>> (r3x) ,r4x) ;
}

inline export BOOL _FINDFILE_ (const String<STR> &file) popping {
	const auto r1x = GetFileAttributes (file.raw ().self) ;
	return r1x != INVALID_FILE_ATTRIBUTES && (r1x & FILE_ATTRIBUTE_DIRECTORY) == 0 ;
}

inline export void _ERASEFILE_ (const String<STR> &file) {
	DeleteFile (file.raw ().self) ;
}

inline export void _COPYFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	CopyFile (src_file.raw ().self ,dst_file.raw ().self ,TRUE) ;
}

inline export void _MOVEFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	MoveFile (src_file.raw ().self ,dst_file.raw ().self) ;
}

inline export void _LINKFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	CreateHardLink (dst_file.raw ().self ,src_file.raw ().self ,NULL) ;
}

inline export BOOL _IDENTICALFILE_ (const String<STR> &file1 ,const String<STR> &file2) popping {
	auto rax = ARRAY2<BY_HANDLE_FILE_INFORMATION> () ;
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file1.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_STATIC_WARNING_ ("unqualified") ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
	}) ;
	const auto r2x = GetFileInformationByHandle (r1x ,&_ZERO_ (rax[0])) ;
	if (r2x == 0 || rax[0].nNumberOfLinks == 0)
		return FALSE ;
	const auto r3x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = CreateFile (file2.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_STATIC_WARNING_ ("unqualified") ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
	}) ;
	const auto r4x = GetFileInformationByHandle (r3x ,&_ZERO_ (rax[1])) ;
	if (r4x == 0 || rax[1].nNumberOfLinks == 0)
		return FALSE ;
	if (rax[0].dwVolumeSerialNumber != rax[1].dwVolumeSerialNumber)
		return FALSE ;
	if (rax[0].nFileIndexHigh != rax[1].nFileIndexHigh)
		return FALSE ;
	if (rax[0].nFileIndexLow != rax[1].nFileIndexLow)
		return FALSE ;
	return TRUE ;
}

inline export String<STR> _PARSEFILEPATH_ (const String<STR> &file) {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = _MAX_ (_MEMRCHR_ (r2x.self ,r1x ,STR ('\\')) ,_MEMRCHR_ (r2x.self ,r1x ,STR ('/'))) + 1 ;
	_MEMCOPY_ (ret.raw ().self ,r2x.self ,r3x) ;
	return std::move (ret) ;
}

inline export String<STR> _PARSEFILENAME_ (const String<STR> &file) {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = _MAX_ (_MEMRCHR_ (r2x.self ,r1x ,STR ('\\')) ,_MEMRCHR_ (r2x.self ,r1x ,STR ('/'))) + 1 ;
	_MEMCOPY_ (ret.raw ().self ,PTRTOARR[&r2x.self[r3x]] ,(r1x - r3x)) ;
	return std::move (ret) ;
}

inline export Queue<String<STR>> _DECOUPLEPATHNAME_ (const String<STR> &file) {
	const auto r1x = (file.empty ()) ? (PhanBuffer<const STR> ()) : (file.raw ()) ;
	auto ris = TextReader<STR> (r1x) ;
	ris.attr ().modify_space (STR ('\\')) ;
	ris.attr ().modify_space (STR ('/')) ;
	auto rax = STR () ;
	Queue<String<STR>> ret = Queue<String<STR>> (DEFAULT_RECURSIVE_SIZE::value) ;
	INDEX ix = ret.insert () ;
	ris.copy () >> rax ;
	if (ris.attr ().varify_space (rax))
		ris >> rax ;
	while (TRUE) {
		ret[ix] = ris.template read<String<STR>> () ;
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

inline export String<STR> _WORKINGPATH_ () {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	GetCurrentDirectory (VARY (ret.size ()) ,ret.raw ().self) ;
	ret += _PCSTR_ ("\\") ;
	return std::move (ret) ;
}

inline export String<STR> _ABSOLUTEPATH_ (const String<STR> &path) {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _DECOUPLEPATHNAME_ (path) ;
	const auto r2x = _CALL_ ([&] () {
		Stack<INDEX> ret = Stack<INDEX> (r1x.length ()) ;
		for (INDEX i = 0 ; i < r1x.length () ; i++) {
			INDEX ix = r1x.access (i) ;
			if (_MEMEQUAL_ (r1x[ix].raw ().self ,_PCSTR_ (".")))
				continue ;
			const auto r3x = !ret.empty () && _MEMEQUAL_ (r1x[ix].raw ().self ,_PCSTR_ ("..")) ;
			for (FOR_ONCE_DO_WHILE_FALSE) {
				if (!r3x)
					continue ;
				if (_MEMEQUAL_ (r1x[ret[ret.peek ()]].raw ().self ,_PCSTR_ ("..")))
					continue ;
				ret.take () ;
			}
			if (r3x)
				continue ;
			ret.add (ix) ;
		}
		return std::move (ret) ;
	}) ;
	const auto r4x = path.length () >= 1 && (path[0] == STR ('\\') || path[0] == STR ('/')) ;
	if (r4x)
		ret += _PCSTR_ ("\\") ;
	const auto r5x = r1x.length () >= 1 && _MEMEQUAL_ (r1x[r1x.access (0)].raw ().self ,_PCSTR_ (".")) ;
	const auto r6x = r1x.length () >= 1 && _MEMEQUAL_ (r1x[r1x.access (0)].raw ().self ,_PCSTR_ ("..")) ;
	const auto r7x = !r4x && (r5x || r6x) ;
	if (r7x)
		ret += _WORKINGPATH_ () ;
	for (INDEX i = 0 ; i < r2x.length () ; i++) {
		if (i != 0)
			ret += _PCSTR_ ("\\") ;
		INDEX ix = r2x[r2x.access (i)] ;
		ret += r1x[ix] ;
	}
	const auto r8x = ret.length () ;
	if (r8x >= 1 && ret[r8x - 1] != STR ('\\') && ret[r8x - 1] != STR ('/'))
		ret += _PCSTR_ ("\\") ;
	return std::move (ret) ;
}

inline export String<STR> _MODULEFILEPATH_ () {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	GetModuleFileName (NULL ,ret.raw ().self ,VARY (ret.size ())) ;
	ret = _PARSEFILEPATH_ (ret) ;
	return std::move (ret) ;
}

inline export String<STR> _MODULEFILENAME_ () {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	GetModuleFileName (NULL ,ret.raw ().self ,VARY (ret.size ())) ;
	ret = _PARSEFILENAME_ (ret) ;
	return std::move (ret) ;
}

inline export BOOL _FINDDIRECTORY_ (const String<STR> &dire) popping {
	const auto r1x = GetFileAttributes (dire.raw ().self) ;
	return r1x != INVALID_FILE_ATTRIBUTES && (r1x & FILE_ATTRIBUTE_DIRECTORY) != 0 ;
}

inline export void _ERASEDIRECTORY_ (const String<STR> &dire) {
	RemoveDirectory (dire.raw ().self) ;
}

inline export void _CLEARDIRECTORY_ (const String<STR> &dire) {
	auto rax = Stack<PACK<String<STR> ,BOOL>> () ;
	const auto r1x = Function<void (const String<STR> &)> ([&] (const String<STR> &_file) {
		_ERASEFILE_ (_file) ;
	}) ;
	const auto r2x = Function<void (const String<STR> &)> ([&] (const String<STR> &_dire) {
		_DYNAMIC_ASSERT_ (!rax.full () || rax.size () < DEFAULT_EXPANDGUARD_SIZE::value) ;
		rax.add ({_dire ,FALSE}) ;
	}) ;
	_ENUMDIRECTORY_ (dire ,r1x ,r2x) ;
	while (!rax.empty ()) {
		INDEX ix = rax.peek () ;
		if (rax[ix].P2) {
			_ERASEDIRECTORY_ (rax[ix].P1) ;
			rax.take () ;
		} else {
			_ENUMDIRECTORY_ (rax[ix].P1 ,r1x ,r2x) ;
			rax[ix].P2 = TRUE ;
		}
	}
}

inline export void _BUILDDIRECTORY_ (const String<STR> &dire) {
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _DECOUPLEPATHNAME_ (_ABSOLUTEPATH_ (dire)) ;
	_DEBUG_ASSERT_ (r1x.length () >= 1) ;
	const auto r2x = dire.length () >= 1 && (dire[0] == STR ('\\') || dire[0] == STR ('/')) ;
	if (r2x)
		rax += _PCSTR_ ("\\") ;
	for (INDEX i = 0 ; i < r1x.length () ; i++) {
		if (i != 0)
			rax += _PCSTR_ ("\\") ;
		INDEX ix = r1x.access (i) ;
		rax += r1x[ix] ;
		const auto r3x = r1x[ix].length () ;
		if (r3x > 1 && r1x[ix][r3x - 1] == STR (':'))
			continue ;
		CreateDirectory (rax.raw ().self ,NULL) ;
	}
}

inline export void _ENUMDIRECTORY_ (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping {
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	rax += dire ;
	rax += _PCSTR_ ("\\") ;
	const auto r1x = rax.length () ;
	auto rbx = WIN32_FIND_DATA () ;
	const auto r2x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		rax += _PCSTR_ ("*.*") ;
		me = FindFirstFile (rax.raw ().self ,&_ZERO_ (rbx)) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		rax[r1x] = 0 ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		FindClose (me) ;
	}) ;
	if (r2x == NULL)
		return ;
	while (rbx.cFileName[0] != 0) {
		for (FOR_ONCE_DO_WHILE_FALSE) {
			if (_MEMEQUAL_ (PTRTOARR[&rbx.cFileName[0]] ,_PCSTR_ (".")))
				continue ;
			if (_MEMEQUAL_ (PTRTOARR[&rbx.cFileName[0]] ,_PCSTR_ ("..")))
				continue ;
			auto &r1 = ((rbx.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) ? file_proc : dire_proc ;
			if (!r1.exist ())
				continue ;
			rax += String<STR> (rbx.cFileName) ;
			r1 (rax) ;
			rax[r1x] = 0 ;
		}
		rbx.cFileName[0] = 0 ;
		FindNextFile (r2x ,&rbx) ;
	}
}
} ;

class StreamLoader::Implement final :private Interface {
private:
	UniqueRef<HANDLE> mFile ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		mFile = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,(GENERIC_READ | GENERIC_WRITE) ,(FILE_SHARE_READ | FILE_SHARE_WRITE) ,NULL ,OPEN_ALWAYS ,FILE_FLAG_SEQUENTIAL_SCAN ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
	}

	void read (const PhanBuffer<BYTE> &data) popping {
		_DEBUG_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = VARY () ;
		const auto r1x = ReadFile (mFile ,data ,VARY (data.size ()) ,&(rax = 0) ,NULL) ;
		const auto r2x = (r1x != 0) ? (LENGTH (rax)) : 0 ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x >= 0 && r2x < VAR32_MAX) ;
		if (data.size () - r2x == 0)
			return ;
		_MEMFILL_ (PTRTOARR[&data[r2x]] ,(data.size () - r2x) ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		auto rax = VARY () ;
		const auto r1x = WriteFile (mFile ,data ,VARY (data.size ()) ,&(rax = 0) ,NULL) ;
		const auto r2x = (r1x != 0) ? (LENGTH (rax)) : 0 ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
	}

	void flush () {
		FlushFileBuffers (mFile) ;
	}
} ;

inline export StreamLoader::StreamLoader (const String<STR> &file) {
	mThis = AnyRef<Implement>::make (file) ;
}

inline export void StreamLoader::read (const PhanBuffer<BYTE> &data) popping {
	mThis.rebind<Implement> ()->read (data) ;
}

inline export void StreamLoader::write (const PhanBuffer<const BYTE> &data) {
	mThis.rebind<Implement> ()->write (data) ;
}

inline export void StreamLoader::flush () {
	mThis.rebind<Implement> ()->flush () ;
}

class BufferLoader::Implement final :private Interface {
private:
	class Pack {
	private:
		friend Implement ;
		Monostate<UniqueRef<HANDLE>> mFile ;
		Monostate<UniqueRef<HANDLE>> mMapping ;
		Monostate<UniqueRef<PhanBuffer<BYTE>>> mBuffer ;
	} ;

private:
	UniqueRef<Pack> mHolder ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) :Implement (ARGVP0) {
		auto &r1 = mHolder->mFile.self ;
		auto &r2 = mHolder->mMapping.self ;
		auto &r3 = mHolder->mBuffer.self ;
		r1 = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
		const auto r1x = LENGTH (GetFileSize (r1 ,NULL)) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		r2 = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFileMapping (r1 ,NULL ,PAGE_READONLY ,0 ,VARY (r1x) ,NULL) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
		r3 = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r2x = MapViewOfFile (r2 ,FILE_MAP_READ ,0 ,0 ,r1x) ;
			_DYNAMIC_ASSERT_ (r2x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r2x) ,r1x) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			_DEBUG_ASSERT_ (me.self != NULL) ;
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	explicit Implement (const String<STR> &file ,LENGTH file_len) :Implement (ARGVP0) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		auto &r1 = mHolder->mFile.self ;
		auto &r2 = mHolder->mMapping.self ;
		auto &r3 = mHolder->mBuffer.self ;
		r1 = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFile (file.raw ().self ,(GENERIC_READ | GENERIC_WRITE) ,0 ,NULL ,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
		r2 = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFileMapping (r1 ,NULL ,PAGE_READWRITE ,0 ,VARY (file_len) ,NULL) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
		r3 = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r1x = MapViewOfFile (r2 ,(FILE_MAP_READ | FILE_MAP_WRITE) ,0 ,0 ,file_len) ;
			_DYNAMIC_ASSERT_ (r1x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r1x) ,file_len) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			if (me.self == NULL)
				return ;
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	explicit Implement (const String<STR> &file ,BOOL cache) :Implement (ARGVP0) {
		_DEBUG_ASSERT_ (cache) ;
		auto &r2 = mHolder->mMapping.self ;
		auto &r3 = mHolder->mBuffer.self ;
		r2 = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = OpenFileMapping (FILE_MAP_READ ,FALSE ,file.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
		r3 = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r1x = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
				const auto r2x = MapViewOfFile (r2 ,FILE_MAP_READ ,0 ,0 ,0) ;
				_DYNAMIC_ASSERT_ (r2x != NULL) ;
				me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r2x) ,1) ;
			} ,[] (PhanBuffer<BYTE> &me) {
				_DEBUG_ASSERT_ (me.self != NULL) ;
				UnmapViewOfFile (me.self) ;
			}) ;
			auto rax = MEMORY_BASIC_INFORMATION () ;
			const auto r3x = VirtualQuery (r1x->self ,&_ZERO_ (rax) ,_SIZEOF_ (MEMORY_BASIC_INFORMATION)) ;
			_DYNAMIC_ASSERT_ (r3x == _SIZEOF_ (MEMORY_BASIC_INFORMATION)) ;
			const auto r4x = MapViewOfFile (r2 ,FILE_MAP_READ ,0 ,0 ,rax.RegionSize) ;
			_DYNAMIC_ASSERT_ (r4x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r4x) ,LENGTH (rax.RegionSize)) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			_DEBUG_ASSERT_ (me.self != NULL) ;
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	explicit Implement (const String<STR> &file ,LENGTH file_len ,BOOL cache) :Implement (ARGVP0) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cache) ;
		auto &r2 = mHolder->mMapping.self ;
		auto &r3 = mHolder->mBuffer.self ;
		r2 = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,VARY (file_len) ,file.raw ().self) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			CloseHandle (me) ;
		}) ;
		r3 = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r1x = MapViewOfFile (r2 ,(FILE_MAP_READ | FILE_MAP_WRITE) ,0 ,0 ,file_len) ;
			_DYNAMIC_ASSERT_ (r1x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r1x) ,file_len) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			_DEBUG_ASSERT_ (me.self != NULL) ;
			UnmapViewOfFile (me.self) ;
		}) ;
	}

	PhanBuffer<BYTE> watch () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		return PhanBuffer<BYTE>::make (mHolder->mBuffer.self.self) ;
	}

	PhanBuffer<const BYTE> watch () const {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		return PhanBuffer<const BYTE>::make (mHolder->mBuffer.self.self) ;
	}

	void flush () {
		_DEBUG_ASSERT_ (mHolder.exist ()) ;
		FlushViewOfFile (mHolder->mBuffer.self->self ,mHolder->mBuffer.self->size ()) ;
	}

private:
	explicit Implement (const decltype (ARGVP0) &) {
		mHolder = UniqueRef<Pack> ([] (Pack &me) {
			me.mFile.self = UniqueRef<HANDLE> () ;
			me.mMapping.self = UniqueRef<HANDLE> () ;
			me.mBuffer.self = UniqueRef<PhanBuffer<BYTE>> () ;
		} ,[] (Pack &me) {
			me.mBuffer.self = UniqueRef<PhanBuffer<BYTE>> () ;
			me.mMapping.self = UniqueRef<HANDLE> () ;
			me.mFile.self = UniqueRef<HANDLE> () ;
		}) ;
	}
} ;

inline export BufferLoader::BufferLoader (const String<STR> &file) {
	mThis = AnyRef<Implement>::make (file) ;
}

inline export BufferLoader::BufferLoader (const String<STR> &file ,LENGTH file_len) {
	mThis = AnyRef<Implement>::make (file ,file_len) ;
}

inline export BufferLoader::BufferLoader (const String<STR> &file ,BOOL cache) {
	mThis = AnyRef<Implement>::make (file ,cache) ;
}

inline export BufferLoader::BufferLoader (const String<STR> &file ,LENGTH file_len ,BOOL cache) {
	mThis = AnyRef<Implement>::make (file ,file_len ,cache) ;
}

inline export PhanBuffer<BYTE> BufferLoader::watch () {
	return mThis.rebind<Implement> ()->watch () ;
}

inline export PhanBuffer<const BYTE> BufferLoader::watch () const {
	return mThis.rebind<Implement> ()->watch () ;
}

inline export void BufferLoader::flush () {
	mThis.rebind<Implement> ()->flush () ;
}

class FileSystemService::Implement final :private FileSystemService::Abstract {
private:
	friend FileSystemService ;
	friend HolderRef<Abstract> ;

public:
	AutoBuffer<BYTE> load_file (const String<STR> &file) popping override {
		return _LOADFILE_ (file) ;
	}

	void load_file (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping override {
		_LOADFILE_ (file ,data) ;
	}

	void save_file (const String<STR> &file ,const PhanBuffer<const BYTE> &data) override {
		_SAVEFILE_ (file ,data) ;
	}

	PhanBuffer<const BYTE> load_asset_file (FLAG resource) popping override {
		return _LOADASSETFILE_ (resource) ;
	}

	BOOL find_file (const String<STR> &file) popping override {
		return _FINDFILE_ (file) ;
	}

	void erase_file (const String<STR> &file) override {
		_ERASEFILE_ (file) ;
	}

	void copy_file (const String<STR> &dst_file ,const String<STR> &src_file) override {
		_COPYFILE_ (dst_file ,src_file) ;
	}

	void move_file (const String<STR> &dst_file ,const String<STR> &src_file) override {
		_MOVEFILE_ (dst_file ,src_file) ;
	}

	void link_file (const String<STR> &dst_file ,const String<STR> &src_file) override {
		_LINKFILE_ (dst_file ,src_file) ;
	}

	BOOL identical_file (const String<STR> &file1 ,const String<STR> &file2) popping override {
		return _IDENTICALFILE_ (file1 ,file2) ;
	}

	String<STR> parse_file_path (const String<STR> &file) const override {
		return _PARSEFILEPATH_ (file) ;
	}

	String<STR> parse_file_name (const String<STR> &file) const override {
		return _PARSEFILENAME_ (file) ;
	}

	Queue<String<STR>> decouple_path_name (const String<STR> &file) const override {
		return _DECOUPLEPATHNAME_ (file) ;
	}

	String<STR> working_path () const override {
		return _WORKINGPATH_ () ;
	}

	String<STR> absolute_path (const String<STR> &path) const override {
		return _ABSOLUTEPATH_ (path) ;
	}

	String<STR> module_file_path () const override {
		return _MODULEFILEPATH_ () ;
	}

	String<STR> module_file_name () const override {
		return _MODULEFILEPATH_ () ;
	}

	BOOL find_directory (const String<STR> &dire) popping override {
		return _FINDDIRECTORY_ (dire) ;
	}

	void erase_directory (const String<STR> &dire) override {
		_ERASEDIRECTORY_ (dire) ;
	}

	void clear_directory (const String<STR> &dire) override {
		_CLEARDIRECTORY_ (dire) ;
	}

	void build_directory (const String<STR> &dire) override {
		_BUILDDIRECTORY_ (dire) ;
	}

	void enum_directory (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping override {
		_ENUMDIRECTORY_ (dire ,file_proc ,dire_proc) ;
	}
} ;

inline export FileSystemService::FileSystemService () {
	mThis = HolderRef<Abstract> (_NULL_<const ARGV<Implement>> ()) ;
}
} ;