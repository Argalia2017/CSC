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

#ifdef __CSC_DEPRECATED__
#include <cstdio>

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
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
inline namespace FILESYSTEM {
inline exports AutoBuffer<BYTE> _LOADFILE_ (const String<STR> &file) popping {
	const auto r1x = _BUILDSTRS_<STRA> (file) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = ::open (r1x.raw ().self ,O_RDONLY) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		::close (me) ;
	}) ;
	const auto r3x = LENGTH (::lseek (r2x.self ,0 ,SEEK_END)) ;
	_DYNAMIC_ASSERT_ (r3x >= 0 && r3x < VAR32_MAX) ;
	::lseek (r2x.self ,0 ,SEEK_SET) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r3x) ;
	const auto r4x = LENGTH (::read (r2x.self ,ret.self ,VAR32 (r3x))) ;
	_DYNAMIC_ASSERT_ (r4x == r3x) ;
	return std::move (ret) ;
}

inline exports void _LOADFILE_ (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping {
	const auto r1x = _BUILDSTRS_<STRA> (file) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = ::open (r1x.raw ().self ,O_RDONLY) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		::close (me) ;
	}) ;
	const auto r3x = LENGTH (::lseek (r2x.self ,0 ,SEEK_END)) ;
	_DYNAMIC_ASSERT_ (r3x > 0 && r3x <= data.size ()) ;
	::lseek (r2x.self ,0 ,SEEK_SET) ;
	const auto r4x = LENGTH (::read (r2x.self ,data.self ,VAR32 (r3x))) ;
	_DYNAMIC_ASSERT_ (r4x == r3x) ;
	_MEMFILL_ (PTRTOARR[&data.self[r3x]] ,(data.size () - r3x) ,BYTE (0X00)) ;
}

inline exports void _SAVEFILE_ (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	const auto r1x = _BUILDSTRS_<STRA> (file) ;
	_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = ::open (r1x.raw ().self ,(O_CREAT | O_WRONLY | O_TRUNC) ,(S_IRWXU | S_IRWXG | S_IRWXO)) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		::close (me) ;
	}) ;
	const auto r3x = LENGTH (::write (r2x.self ,data.self ,VAR32 (data.size ()))) ;
	_DYNAMIC_ASSERT_ (r3x == data.size ()) ;
}

inline exports PhanBuffer<const BYTE> _LOADASSETFILE_ (FLAG resource) popping {
	_STATIC_WARNING_ ("unimplemented") ;
	_DYNAMIC_ASSERT_ (FALSE) ;
	return PhanBuffer<const BYTE> () ;
}

inline exports BOOL _FINDFILE_ (const String<STR> &file) popping {
	const auto r1x = _BUILDSTRS_<STRA> (file) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = ::open (r1x.raw ().self ,O_RDONLY) ;
	} ,[] (VAR32 &me) {
		if (me < 0)
			return ;
		::close (me) ;
	}) ;
	if (r2x.self < 0)
		return FALSE ;
	return TRUE ;
}

inline BOOL _inline_FINDJUNTION_ (const String<STRA> &dire) popping {
	const auto r1x = UniqueRef<PTR<DIR>> ([&] (PTR<DIR> &me) {
		me = ::opendir (dire.raw ().self) ;
	} ,[] (PTR<DIR> &me) {
		if (me == NULL)
			return ;
		::closedir (me) ;
	}) ;
	if (r1x.self == NULL)
		return FALSE ;
	return TRUE ;
}

inline exports void _ERASEFILE_ (const String<STR> &file) {
	const auto r1x = _BUILDSTRS_<STRA> (file) ;
	const auto r2x = _inline_FINDJUNTION_ (r1x) ;
	if (r2x)
		return ;
	const auto r3x = ::unlink (r1x.raw ().self) ;
	(void) r3x ;
}

inline exports void _COPYFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _FINDFILE_ (dst_file) ;
	_DYNAMIC_ASSERT_ (!r1x) ;
	const auto r2x = _LOADFILE_ (src_file) ;
	_SAVEFILE_ (dst_file ,PhanBuffer<const BYTE>::make (r2x)) ;
}

inline exports void _MOVEFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _BUILDSTRS_<STRA> (src_file) ;
	const auto r2x = _BUILDSTRS_<STRA> (dst_file) ;
	const auto r3x = _FINDFILE_ (dst_file) ;
	_DYNAMIC_ASSERT_ (!r3x) ;
	const auto r4x = ::rename (r1x.raw ().self ,r2x.raw ().self) ;
	(void) r4x ;
}

inline exports void _LINKFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _BUILDSTRS_<STRA> (src_file) ;
	const auto r2x = _BUILDSTRS_<STRA> (dst_file) ;
	const auto r3x = _FINDFILE_ (dst_file) ;
	_DYNAMIC_ASSERT_ (!r3x) ;
	const auto r4x = ::link (r1x.raw ().self ,r2x.raw ().self) ;
	(void) r4x ;
}

inline exports BOOL _IDENTICALFILE_ (const String<STR> &file1 ,const String<STR> &file2) popping {
	const auto r1x = _BUILDSTRS_<STRA> (file1) ;
	const auto r2x = _BUILDSTRS_<STRA> (file2) ;
	auto rax = ARRAY2<DEF<struct stat>> () ;
	_ZERO_ (rax[0]) ;
	const auto r3x = ::stat (r1x.raw ().self ,&rax[0]) ;
	if (r3x != 0)
		return FALSE ;
	if (rax[0].st_nlink == 0)
		return FALSE ;
	_ZERO_ (rax[1]) ;
	const auto r4x = ::stat (r2x.raw ().self ,&rax[1]) ;
	if (r4x != 0)
		return FALSE ;
	if (rax[1].st_nlink == 0)
		return FALSE ;
	if (rax[0].st_dev != rax[1].st_dev)
		return FALSE ;
	if (rax[0].st_ino != rax[1].st_ino)
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
		(file.empty ()) ? PhanBuffer<const STR> () :
		file.raw ()) ;
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
	auto rax = String<STRA> (DEFAULT_SHORTSTRING_SIZE::value) ;
	if SWITCH_CASE (TRUE) {
		const auto r1x = ::getcwd (rax.raw ().self ,VAR32 (rax.size ())) ;
		if (r1x != NULL)
			discard ;
		rax.clear () ;
	}
	if SWITCH_CASE (TRUE) {
		const auto r2x = rax.length () ;
		if (r2x < 1)
			discard ;
		if (rax[r2x - 1] == STRA ('\\'))
			discard ;
		if (rax[r2x - 1] == STRA ('/'))
			discard ;
		rax += _PCSTRA_ ("/") ;
	}
	return _PARSESTRS_ (rax) ;
}

inline Deque<INDEX> _inline_RELATIVEPATHNAME_ (const Deque<String<STR>> &path_name) {
	Deque<INDEX> ret = Deque<INDEX> (path_name.length ()) ;
	for (auto &&i : _RANGE_ (0 ,path_name.length ())) {
		INDEX ix = path_name.access (i) ;
		if (path_name[ix] == _PCSTR_ ("."))
			continue ;
		auto fax = TRUE ;
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
	auto rax = _DECOUPLEPATHNAME_ (path) ;
	auto fax = TRUE ;
	if SWITCH_CASE (fax) {
		if (!(path.size () >= 1 && path[0] == STR ('\\')))
			if (!(path.size () >= 1 && path[0] == STR ('/')))
				discard ;
		ret += _PCSTR_ ("/") ;
	}
	if SWITCH_CASE (fax) {
		if (!(rax.length () >= 1 && rax[rax.access (0)] == _PCSTR_ (".")))
			if (!(rax.length () >= 1 && rax[rax.access (0)] == _PCSTR_ ("..")))
				discard ;
		const auto r1x = _WORKINGPATH_ () ;
		auto tmp = _DECOUPLEPATHNAME_ (r1x) ;
		tmp.appand (std::move (rax)) ;
		rax = std::move (tmp) ;
		if (!(r1x.size () >= 1 && r1x[0] == STR ('\\')))
			if (!(r1x.size () >= 1 && r1x[0] == STR ('/')))
				discard ;
		ret += _PCSTR_ ("/") ;
	}
	const auto r2x = _inline_RELATIVEPATHNAME_ (rax) ;
	for (auto &&i : _RANGE_ (0 ,r2x.length ())) {
		if (i > 0)
			ret += _PCSTR_ ("/") ;
		INDEX ix = r2x[r2x.access (i)] ;
		ret += rax[ix] ;
	}
	if SWITCH_CASE (TRUE) {
		const auto r3x = ret.length () ;
		if (r3x < 1)
			discard ;
		if (ret[r3x - 1] == STR ('\\'))
			discard ;
		if (ret[r3x - 1] == STR ('/'))
			discard ;
		ret += _PCSTR_ ("/") ;
	}
	return std::move (ret) ;
}

inline exports const String<STR> &_MODULEFILEPATH_ () popping {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	return _CACHE_ ([] () {
		auto rax = String<STRA> (DEFAULT_SHORTSTRING_SIZE::value) ;
		const auto r1x = ::readlink (_PCSTRA_ ("/proc/self/exe") ,rax.raw ().self ,VAR32 (rax.size ())) ;
		if (!(r1x >= 0 && r1x < rax.size ()))
			rax.clear () ;
		String<STR> ret = _PARSESTRS_ (rax) ;
		ret = _PARSEFILEPATH_ (ret) ;
		ret += _PCSTR_ ("/") ;
		return std::move (ret) ;
	}) ;
}

inline exports const String<STR> &_MODULEFILENAME_ () popping {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	return _CACHE_ ([] () {
		auto rax = String<STRA> (DEFAULT_SHORTSTRING_SIZE::value) ;
		const auto r1x = ::readlink (_PCSTRA_ ("/proc/self/exe") ,rax.raw ().self ,VAR32 (rax.size ())) ;
		if (!(r1x >= 0 && r1x < rax.size ()))
			rax.clear () ;
		const auto r2x = _PARSESTRS_ (rax) ;
		return _PARSEFILENAME_ (r2x) ;
	}) ;
}

inline exports BOOL _FINDDIRECTORY_ (const String<STR> &dire) popping {
	const auto r1x = _BUILDSTRS_<STRA> (dire) ;
	const auto r2x = UniqueRef<PTR<DIR>> ([&] (PTR<DIR> &me) {
		me = ::opendir (r1x.raw ().self) ;
	} ,[] (PTR<DIR> &me) {
		if (me == NULL)
			return ;
		::closedir (me) ;
	}) ;
	if (r2x.self == NULL)
		return FALSE ;
	return TRUE ;
}

inline exports BOOL _LOCKDIRECTORY_ (const String<STR> &dire) popping {
	_STATIC_WARNING_ ("unimplemented") ;
	_DYNAMIC_ASSERT_ (FALSE) ;
	return TRUE ;
}

inline exports void _BUILDDIRECTORY_ (const String<STR> &dire) {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	if (_FINDDIRECTORY_ (dire))
		return ;
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _ABSOLUTEPATH_ (dire) ;
	const auto r2x = _DECOUPLEPATHNAME_ (r1x) ;
	_DEBUG_ASSERT_ (r2x.length () >= 1) ;
	if SWITCH_CASE (TRUE) {
		if (!(dire.size () >= 1 && dire[0] == STR ('\\')))
			if (!(dire.size () >= 1 && dire[0] == STR ('/')))
				discard ;
		rax += _PCSTR_ ("/") ;
	}
	for (auto &&i : _RANGE_ (0 ,r2x.length ())) {
		if (i > 0)
			rax += _PCSTR_ ("/") ;
		INDEX ix = r2x.access (i) ;
		rax += r2x[ix] ;
		const auto r3x = r2x[ix].length () ;
		if (r3x > 1)
			if (r2x[ix][r3x - 1] == STR (':'))
				continue ;
		const auto r4x = _BUILDSTRS_<STRA> (rax) ;
		::mkdir (r4x.raw ().self ,(S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) ;
	}
}

inline exports void _ERASEDIRECTORY_ (const String<STR> &dire) {
	const auto r1x = _BUILDSTRS_<STRA> (dire) ;
	const auto r2x = ::rmdir (r1x.raw ().self) ;
	(void) r2x ;
	const auto r3x = _inline_FINDJUNTION_ (r1x) ;
	if (!r3x)
		return ;
	const auto r4x = ::unlink (r1x.raw ().self) ;
	(void) r4x ;
}

//@warn: recursive call with junction(symbolic link) may cause endless loop
inline exports void _ENUMDIRECTORY_ (const String<STR> &dire ,Deque<String<STR>> &file_list ,Deque<String<STR>> &dire_list) popping {
	using DEFAULT_SHORTSTRING_SIZE = ARGC<1023> ;
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	rax += dire ;
	rax += _PCSTR_ ("/") ;
	const auto r1x = rax.length () ;
	const auto r2x = _BUILDSTRS_<STRA> (rax) ;
	const auto r3x = UniqueRef<PTR<DIR>> ([&] (PTR<DIR> &me) {
		me = ::opendir (r2x.raw ().self) ;
	} ,[] (PTR<DIR> &me) {
		if (me == NULL)
			return ;
		::closedir (me) ;
	}) ;
	if (r3x.self == NULL)
		return ;
	while (TRUE) {
		const auto r4x = ::readdir (r3x.self) ;
		if (r4x == NULL)
			break ;
		const auto r5x = _PARSESTRS_ (String<STRA> (PTRTOARR[r4x->d_name])) ;
		if SWITCH_CASE (TRUE) {
			if (r5x == _PCSTR_ ("."))
				discard ;
			if (r5x == _PCSTR_ (".."))
				discard ;
			rax += r5x ;
			auto &r6y = _SWITCH_ (
				(_FINDDIRECTORY_ (rax)) ? dire_list :
				file_list) ;
			r6y.add (rax) ;
		}
		rax[r1x] = 0 ;
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
		auto fax = TRUE ;
		if SWITCH_CASE (fax) {
			if (!rax[ix].P2)
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
	UniqueRef<VAR32> mReadFile ;
	UniqueRef<VAR32> mWriteFile ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		const auto r1x = _BUILDSTRS_<STRA> (file) ;
		mWriteFile = UniqueRef<VAR32> ([&] (VAR32 &me) {
			me = ::open (r1x.raw ().self ,(O_CREAT | O_WRONLY | O_APPEND) ,(S_IRWXU | S_IRWXG | S_IRWXO)) ;
			_DYNAMIC_ASSERT_ (me >= 0) ;
		} ,[] (VAR32 &me) {
			::close (me) ;
		}) ;
		mReadFile = UniqueRef<VAR32> ([&] (VAR32 &me) {
			me = ::open (r1x.raw ().self ,O_RDONLY) ;
			_DYNAMIC_ASSERT_ (me >= 0) ;
		} ,[] (VAR32 &me) {
			::close (me) ;
		}) ;
	}

	void read (const PhanBuffer<BYTE> &data) popping {
		_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
		const auto r1x = LENGTH (::read (mReadFile ,data.self ,VAR32 (data.size ()))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		_MEMFILL_ (PTRTOARR[&data.self[r1x]] ,(data.size () - r1x) ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
		const auto r1x = LENGTH (::write (mWriteFile ,data.self ,VAR32 (data.size ()))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void flush () {
		_STATIC_WARNING_ ("noop") ;
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
public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	explicit Implement (const String<STR> &file ,LENGTH file_len) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	explicit Implement (const String<STR> &file ,BOOL cache) {
		_DEBUG_ASSERT_ (cache) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	explicit Implement (const String<STR> &file ,LENGTH file_len ,BOOL cache) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cache) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	PhanBuffer<BYTE> watch () & {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
		return PhanBuffer<BYTE> () ;
	}

	PhanBuffer<const BYTE> watch () const & {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
		return PhanBuffer<const BYTE> () ;
	}

	PhanBuffer<BYTE> watch () && = delete ;

	void flush () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
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