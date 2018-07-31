#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : required csc_filesystem.hpp"
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : required Windows.h"
#endif

namespace CSC {
inline namespace FILESYSTEM {
inline export AutoBuffer<BYTE> _LOADFILE_ (const String<STR> &file) {
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = NULL ;
		me = CreateFile (file ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
		me = NULL ;
	}) ;
	const auto r2x = LENGTH (GetFileSize (r1x ,NULL)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= VAR32_MAX) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r2x) ;
	const auto r3x = ReadFile (r1x ,ret ,VARY (r2x) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	_DYNAMIC_ASSERT_ (r3x) ;
	return std::move (ret) ;
}

inline export void _LOADFILE_ (const String<STR> &file ,const PhanBuffer<BYTE> &data) {
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = NULL ;
		me = CreateFile (file ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
		me = NULL ;
	}) ;
	const auto r2x = LENGTH (GetFileSize (r1x ,NULL)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= data.size ()) ;
	const auto r3x = ReadFile (r1x ,data ,VARY (r2x) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	_DYNAMIC_ASSERT_ (r3x) ;
}

inline export void _SAVEFILE_ (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	_DEBUG_ASSERT_ (data.size () >= 0 && data.size () <= VAR32_MAX) ;
	const auto r1x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = NULL ;
		me = CreateFile (file ,GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,
			FILE_ATTRIBUTE_NORMAL ,NULL) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		CloseHandle (me) ;
		me = NULL ;
	}) ;
	const auto r2x = WriteFile (r1x ,data ,VARY (data.size ()) ,&_XVALUE_ (VARY (0)) ,NULL) ;
	_DYNAMIC_ASSERT_ (r2x) ;
}

inline export BOOL _EXISTFILE_ (const String<STR> &file) {
	const auto r1x = GetFileAttributes (file) ;
	return r1x != INVALID_FILE_ATTRIBUTES && (r1x & FILE_ATTRIBUTE_DIRECTORY) == 0 ;
}

inline export void _ERASEFILE_ (const String<STR> &file) {
	DeleteFile (file) ;
}

inline export void _COPYFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	CopyFile (src_file ,dst_file ,TRUE) ;
}

inline export void _MOVEFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	MoveFile (src_file ,dst_file) ;
}

inline export BOOL _EXISTDIRECTORY_ (const String<STR> &file) {
	const auto r1x = GetFileAttributes (file) ;
	return r1x != INVALID_FILE_ATTRIBUTES && (r1x & FILE_ATTRIBUTE_DIRECTORY) != 0 ;
}

inline export void _ENUMPATH_ (const String<STR> &path ,const Function<void (String<STR> &&)> &file_proc ,const Function<void (String<STR> &&)> &dire_proc) {
	const auto r1x = _CALL_ ([&] () {
		String<STR> ret = String<STR> (SHORTSTRING_SIZE::value) ;
		ret += path ;
		const auto r2x = ret.length () - 1 ;
		if (r2x >= 0 && path[r2x] != STR ('\\') && path[r2x] != STR ('/'))
			ret += STR ('\\') ;
		return std::move (ret) ;
	}) ;
	auto rax = WIN32_FIND_DATA {0} ;
	const auto r3x = UniqueRef<HANDLE> ([&] (HANDLE &me) {
		me = FindFirstFile (r1x + _PCSTR_ ("*.*") ,&rax) ;
		if (me == INVALID_HANDLE_VALUE)
			me = NULL ;
	} ,[] (HANDLE &me) {
		if (me == NULL)
			return ;
		FindClose (me) ;
		me = NULL ;
	}) ;
	if (r3x == NULL)
		return ;
	while (rax.cFileName[0] != STR ('\0')) {
		if (!_MEMEQUAL_ (&rax.cFileName[0] ,_PCSTR_ (".")) && !_MEMEQUAL_ (&rax.cFileName[0] ,_PCSTR_ (".."))) {
			auto &r1 = (rax.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 ? file_proc : dire_proc ;
			r1 (r1x + String<STR> (rax.cFileName)) ;
		}
		rax.cFileName[0] = STR ('\0') ;
		FindNextFile (r3x ,&rax) ;
	}
}

inline export void _ERASEDIRECTORY_ (const String<STR> &file) {
	RemoveDirectory (file) ;
}

inline export void _CLEARDIRECTORY_ (const String<STR> &file) {
	auto rax = ARRAY2<Stack<String<STR>>> () ;
	const auto r1x = Function<void (String<STR> &&)> ([&] (String<STR> &&file) {
		rax[0].add (file) ;
	}) ;
	const auto r2x = Function<void (String<STR> &&)> ([&] (String<STR> &&file) {
		rax[1].add (file) ;
	}) ;
	_ENUMPATHALL_ (file ,r1x ,r2x) ;
	for (auto &i : rax[0])
		_ERASEFILE_ (i) ;
	for (auto &i : rax[1])
		_ERASEDIRECTORY_ (i) ;
}

inline export void _ENSUREPATH_ (const String<STR> &path) {
	auto ris = TextReader<STR> (path.raw ()) ;
	ris.attr ().modify_space (STR ('\\')) ;
	ris.attr ().modify_space (STR ('/')) ;
	auto rax = String<STR> (SHORTSTRING_SIZE::value) ;
	while (TRUE) {
		const auto r1x = ris.read<String<STR>> () ;
		if (r1x.empty ())
			break ;
		rax += r1x ;
		CreateDirectory (rax ,NULL) ;
		rax += STR ('\\') ;
		ris.read<STR> () ;
	}
	ris >> _EOS_ ;
}

inline export PhanBuffer<const BYTE> _LOADASSET_ (FLAG resource ,const String<STR> &type) {
	const auto r1x = FindResource (NULL ,MAKEINTRESOURCE (resource) ,type.self) ;
	_DYNAMIC_ASSERT_ (r1x != NULL) ;
	const auto r2x = LoadResource (NULL ,r1x) ;
	_DYNAMIC_ASSERT_ (r2x != NULL) ;
	const auto r3x = LockResource (r2x) ;
	_DYNAMIC_ASSERT_ (r3x != NULL) ;
	const auto r4x = LENGTH (SizeofResource (NULL ,r1x)) ;
	_DYNAMIC_ASSERT_ (r4x >= 0) ;
	return PhanBuffer<const BYTE>::make (_LOAD_<ARR<BYTE>> (r3x) ,r4x) ;
}

inline export String<STR> _WORKINGPATH_ () {
	String<STR> ret = String<STR> (SHORTSTRING_SIZE::value) ;
	GetCurrentDirectory (VARY (ret.size ()) ,&ret[0]) ;
	ret += STR ('\\') ;
	return std::move (ret) ;
}

inline export String<STR> _EXECUTINGPATH_ () {
	String<STR> ret = String<STR> (SHORTSTRING_SIZE::value) ;
	GetModuleFileName (NULL ,&ret[0] ,VARY (ret.size ())) ;
	ret = _PARSEFILEPATH_ (ret) ;
	return std::move (ret) ;
}
} ;

class StreamLoader::Engine :public Interface {
private:
	UniqueRef<HANDLE> mFile ;

public:
	Engine () = delete ;

	explicit Engine (const String<STR> &file) {
		mFile = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = CreateFile (file ,GENERIC_READ | GENERIC_WRITE ,FILE_SHARE_READ | FILE_SHARE_WRITE ,NULL ,OPEN_ALWAYS ,
				FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
	}

	BOOL exist () const {
		return mFile.exist () ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (data.size () > 0 && data.size () <= VAR32_MAX) ;
		const auto r1x = _CALL_ ([&] () {
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :6031) //@info:warning C6031: 返回值被忽略: “xxx”。
#endif
			VARY ret = VARY (data.size ()) ;
			ReadFile (mFile ,data ,ret ,&ret ,NULL) ;
			return std::move (ret) ;
#pragma warning (pop)
		}) ;
		_DYNAMIC_ASSERT_ (VAR (r1x) >= 0 && VAR (r1x) <= VAR32_MAX) ;
		_MEMFILL_ (&data[VAR (r1x)] ,data.size () - VAR (r1x) ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (data.size () >= 0 && data.size () <= VAR32_MAX) ;
		const auto r1x = _CALL_ ([&] () {
			VARY ret = VARY (data.size ()) ;
			WriteFile (mFile ,data ,ret ,&ret ,NULL) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (VAR (r1x) == data.size ()) ;
	}

	void flush () {
		_DEBUG_ASSERT_ (exist ()) ;
		FlushFileBuffers (mFile) ;
	}
} ;

inline export StreamLoader::StreamLoader (const String<STR> &file) {
	mHolder = AnyRef<Engine>::make (file) ;
}

inline export BOOL StreamLoader::exist () const {
	return mHolder.exist () && mHolder.rebind<Engine> ()->exist () ;
}

inline export void StreamLoader::read (const PhanBuffer<BYTE> &data) {
	mHolder.rebind<Engine> ()->read (data) ;
}

inline export void StreamLoader::write (const PhanBuffer<const BYTE> &data) {
	mHolder.rebind<Engine> ()->write (data) ;
}

inline export void StreamLoader::flush () {
	mHolder.rebind<Engine> ()->flush () ;
}

class BufferLoader::Engine :public Interface {
private:
	UniqueRef<HANDLE> mFile ;
	UniqueRef<HANDLE> mMapping ;
	UniqueRef<PhanBuffer<BYTE>> mBuffer ;

public:
	Engine () = delete ;

	explicit Engine (const String<STR> &file) {
		mFile = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = CreateFile (file ,GENERIC_READ ,FILE_SHARE_READ ,NULL ,OPEN_EXISTING ,
				FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
		const auto r1x = LENGTH (GetFileSize (mFile ,NULL)) ;
		_DYNAMIC_ASSERT_ (r1x > 0 && r1x <= VAR32_MAX) ;
		mMapping = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = CreateFileMapping (mFile ,NULL ,PAGE_READONLY ,0 ,VARY (r1x) ,NULL) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
		mBuffer = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r2x = MapViewOfFile (mMapping ,FILE_MAP_READ ,0 ,0 ,r1x) ;
			_DYNAMIC_ASSERT_ (r2x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r2x) ,r1x) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			if (me.self == NULL)
				return ;
			UnmapViewOfFile (me) ;
			me = PhanBuffer<BYTE> () ;
		}) ;
	}

	explicit Engine (const String<STR> &file ,LENGTH len) {
		_DYNAMIC_ASSERT_ (len > 0 && len <= VAR32_MAX) ;
		mFile = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = CreateFile (file ,GENERIC_READ | GENERIC_WRITE ,0 ,NULL ,CREATE_ALWAYS ,
				FILE_ATTRIBUTE_NORMAL ,NULL) ;
			if (me == INVALID_HANDLE_VALUE)
				me = NULL ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
		mMapping = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = CreateFileMapping (mFile ,NULL ,PAGE_READWRITE ,0 ,VARY (len) ,NULL) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
		mBuffer = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r1x = MapViewOfFile (mMapping ,FILE_MAP_READ | FILE_MAP_WRITE ,0 ,0 ,len) ;
			_DYNAMIC_ASSERT_ (r1x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r1x) ,len) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			if (me.self == NULL)
				return ;
			UnmapViewOfFile (me) ;
			me = PhanBuffer<BYTE> () ;
		}) ;
	}

	explicit Engine (const String<STR> &file ,const ARGV<ARGC<1>> &) {
		mMapping = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = OpenFileMapping (FILE_MAP_READ | FILE_MAP_WRITE ,FALSE ,file) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
		mBuffer = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r1x = adress_info (mMapping) ;
			const auto r2x = MapViewOfFile (mMapping ,FILE_MAP_READ | FILE_MAP_WRITE ,0 ,0 ,r1x.RegionSize) ;
			_DYNAMIC_ASSERT_ (r2x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r2x) ,r1x.RegionSize) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			if (me.self == NULL)
				return ;
			UnmapViewOfFile (me) ;
			me = PhanBuffer<BYTE> () ;
		}) ;
	}

	explicit Engine (const String<STR> &file ,LENGTH len ,const ARGV<ARGC<1>> &) {
		_DYNAMIC_ASSERT_ (len > 0 && len <= VAR32_MAX) ;
		mMapping = UniqueRef<HANDLE> ([&] (HANDLE &me) {
			me = NULL ;
			me = CreateFileMapping (INVALID_HANDLE_VALUE ,NULL ,PAGE_READWRITE ,0 ,VARY (len) ,file) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (HANDLE &me) {
			if (me == NULL)
				return ;
			CloseHandle (me) ;
			me = NULL ;
		}) ;
		mBuffer = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r1x = MapViewOfFile (mMapping ,FILE_MAP_READ | FILE_MAP_WRITE ,0 ,0 ,len) ;
			_DYNAMIC_ASSERT_ (r1x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r1x) ,len) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			if (me.self == NULL)
				return ;
			UnmapViewOfFile (me) ;
			me = PhanBuffer<BYTE> () ;
		}) ;
	}

	~Engine () noexcept override {
		mBuffer = UniqueRef<PhanBuffer<BYTE>> () ;
		mMapping = UniqueRef<HANDLE> () ;
		mFile = UniqueRef<HANDLE> () ;
	}

	BOOL exist () const {
		return mFile.exist () && mMapping.exist () && mBuffer.exist () ;
	}

	PhanBuffer<BYTE> watch () {
		_DEBUG_ASSERT_ (exist ()) ;
		return PhanBuffer<BYTE>::make (mBuffer.self) ;
	}

	PhanBuffer<const BYTE> watch () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return PhanBuffer<const BYTE>::make (mBuffer.self) ;
	}

	void flush () {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (mBuffer->self != NULL) ;
		FlushViewOfFile (mBuffer->self ,mBuffer->size ()) ;
	}

private:
	static MEMORY_BASIC_INFORMATION adress_info (HANDLE mapping) {
		MEMORY_BASIC_INFORMATION ret = {0} ;
		const auto r1x = UniqueRef<PhanBuffer<BYTE>> ([&] (PhanBuffer<BYTE> &me) {
			const auto r2x = MapViewOfFile (mapping ,FILE_MAP_READ ,0 ,0 ,0) ;
			_DYNAMIC_ASSERT_ (r2x != NULL) ;
			me = PhanBuffer<BYTE>::make (_LOAD_<ARR<BYTE>> (r2x) ,1) ;
		} ,[] (PhanBuffer<BYTE> &me) {
			if (me.self == NULL)
				return ;
			UnmapViewOfFile (me) ;
			me = PhanBuffer<BYTE> () ;
		}) ;
		const auto r3x = VirtualQuery (r1x->self ,&ret ,_SIZEOF_ (ret)) ;
		_DYNAMIC_ASSERT_ (r3x == _SIZEOF_ (ret)) ;
		return std::move (ret) ;
	}
} ;

inline export BufferLoader::BufferLoader (const String<STR> &file) {
	mHolder = AnyRef<Engine>::make (file) ;
}

inline export BufferLoader::BufferLoader (const String<STR> &file ,LENGTH len) {
	mHolder = AnyRef<Engine>::make (file ,len) ;
}

inline export BufferLoader::BufferLoader (const String<STR> &file ,const ARGV<ARGC<1>> &) {
	mHolder = AnyRef<Engine>::make (file ,ARGV1) ;
}

inline export BufferLoader::BufferLoader (const String<STR> &file ,LENGTH len ,const ARGV<ARGC<1>> &) {
	mHolder = AnyRef<Engine>::make (file ,len ,ARGV1) ;
}

inline export BOOL BufferLoader::exist () const {
	return mHolder.exist () && mHolder.rebind<Engine> ()->exist () ;
}

inline export PhanBuffer<BYTE> BufferLoader::watch () {
	return mHolder.rebind<Engine> ()->watch () ;
}

inline export PhanBuffer<const BYTE> BufferLoader::watch () const {
	return mHolder.rebind<Engine> ()->watch () ;
}

inline export void BufferLoader::flush () {
	mHolder.rebind<Engine> ()->flush () ;
}
} ;