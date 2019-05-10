#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#endif

#ifdef __CSC_DEPRECATED__
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#define self to ()
#define implicit
#define popping
#define imports extern
#define exports
#endif

namespace CSC {
inline namespace S {
inline exports AutoBuffer<BYTE> _LOADFILE_ (const String<STR> &file) popping {
	const auto r1x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = open (_BUILDSTRS_<STRA> (file).raw ().self ,O_RDONLY) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		_DEBUG_ASSERT_ (me >= 0) ;
		close (me) ;
	}) ;
	const auto r2x = LENGTH (lseek (r1x ,0 ,SEEK_END)) ;
	_DYNAMIC_ASSERT_ (r2x >= 0 && r2x < VAR32_MAX) ;
	lseek (r1x ,0 ,SEEK_SET) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r2x) ;
	const auto r3x = LENGTH (read (r1x ,ret.self ,r2x)) ;
	_DYNAMIC_ASSERT_ (r3x == r2x) ;
	return std::move (ret) ;
}

inline exports void _LOADFILE_ (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping {
	const auto r1x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = open (_BUILDSTRS_<STRA> (file).raw ().self ,O_RDONLY) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		_DEBUG_ASSERT_ (me >= 0) ;
		close (me) ;
	}) ;
	const auto r2x = LENGTH (lseek (r1x ,0 ,SEEK_END)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= data.size ()) ;
	lseek (r1x ,0 ,SEEK_SET) ;
	const auto r3x = LENGTH (read (r1x ,data.self ,r2x)) ;
	_DYNAMIC_ASSERT_ (r3x == r2x) ;
}

inline exports void _SAVEFILE_ (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	_DEBUG_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
	const auto r1x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = open (_BUILDSTRS_<STRA> (file).raw ().self ,mode_t (O_CREAT | O_WRONLY | O_TRUNC) ,mode_t (S_IRWXU | S_IRWXG | S_IRWXO)) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		_DEBUG_ASSERT_ (me >= 0) ;
		close (me) ;
	}) ;
	const auto r2x = LENGTH (write (r1x ,data.self ,data.size ())) ;
	_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
}

inline exports PhanBuffer<const BYTE> _LOADASSETFILE_ (FLAG resource) popping {
	_STATIC_WARNING_ ("unimplemented") ;
	_DYNAMIC_ASSERT_ (FALSE) ;
	return PhanBuffer<const BYTE> () ;
}

inline exports BOOL _FINDFILE_ (const String<STR> &file) popping {
	const auto r1x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = open (_BUILDSTRS_<STRA> (file).raw ().self ,O_RDONLY) ;
	} ,[] (VAR32 &me) {
		if (me < 0)
			return ;
		close (me) ;
	}) ;
	return r1x >= 0 ;
}

inline exports void _ERASEFILE_ (const String<STR> &file) {
	remove (_BUILDSTRS_<STRA> (file).raw ().self) ;
}

inline exports void _COPYFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = _LOADFILE_ (src_file) ;
	_SAVEFILE_ (dst_file ,PhanBuffer<const BYTE>::make (r1x)) ;
}

inline exports void _MOVEFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	rename (_BUILDSTRS_<STRA> (src_file).raw ().self ,_BUILDSTRS_<STRA> (dst_file).raw ().self) ;
}

inline exports void _LINKFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = link (_BUILDSTRS_<STRA> (src_file).raw ().self ,_BUILDSTRS_<STRA> (dst_file).raw ().self) ;
	(void) r1x ;
}

inline exports BOOL _IDENTICALFILE_ (const String<STR> &file1 ,const String<STR> &file2) popping {
	auto rax = ARRAY2<struct stat> () ;
	const auto r1x = stat (_BUILDSTRS_<STRA> (file1).raw ().self ,&_ZERO_ (rax[0])) ;
	if (r1x != 0)
		return FALSE ;
	if (rax[0].st_nlink == 0)
		return FALSE ;
	const auto r2x = stat (_BUILDSTRS_<STRA> (file2).raw ().self ,&_ZERO_ (rax[1])) ;
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

inline exports String<STR> _PARSEFILEPATH_ (const String<STR> &file) {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = _MAX_ (_MEMRCHR_ (r2x.self ,r1x ,STR ('\\')) ,VAR_ZERO) ;
	const auto r4x = _MAX_ (_MEMRCHR_ (r2x.self ,r1x ,STR ('/')) ,r3x) ;
	_MEMCOPY_ (ret.raw ().self ,r2x.self ,r4x) ;
	return std::move (ret) ;
}

inline exports String<STR> _PARSEFILENAME_ (const String<STR> &file) {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = _MEMRCHR_ (r2x.self ,r1x ,STR ('\\')) ;
	const auto r4x = _MEMRCHR_ (r2x.self ,r1x ,STR ('/')) ;
	const auto r5x = _MAX_ (r3x ,r4x) + 1 ;
	_MEMCOPY_ (ret.raw ().self ,PTRTOARR[&r2x.self[r5x]] ,(r1x - r5x)) ;
	return std::move (ret) ;
}

inline exports Queue<String<STR>> _DECOUPLEPATHNAME_ (const String<STR> &file) {
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

inline exports String<STR> _WORKINGPATH_ () {
	auto rax = String<STRA> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = getcwd (rax.raw ().self ,VAR32 (rax.size ())) ;
	if (r1x == NULL)
		rax.clear () ;
	const auto r2x = rax.length () ;
	if (r2x >= 1 && rax[r2x - 1] != STRA ('\\') && rax[r2x - 1] != STRA ('/'))
		rax += _PCSTRA_ ("/") ;
	return _PARSESTRS_ (rax) ;
}

inline exports String<STR> _ABSOLUTEPATH_ (const String<STR> &path) {
	String<STR> ret = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _DECOUPLEPATHNAME_ (path) ;
	const auto r2x = _CALL_ ([&] () {
		Stack<INDEX> ret = Stack<INDEX> (r1x.length ()) ;
		for (INDEX i = 0 ; i < r1x.length () ; i++) {
			INDEX ix = r1x.access (i) ;
			if (r1x[ix] == _PCSTR_ ("."))
				continue ;
			_CALL_IF_ ([&] (BOOL &if_cond) {
				if (ret.empty ())
					return (void) (if_cond = FALSE) ;
				if (r1x[ix] != _PCSTR_ (".."))
					return (void) (if_cond = FALSE) ;
				if (r1x[ret[ret.peek ()]] == _PCSTR_ (".."))
					return ;
				ret.take () ;
			} ,[&] (BOOL &if_cond) {
				ret.add (ix) ;
			}) ;
		}
		return std::move (ret) ;
	}) ;
	const auto r4x = BOOL (path.size () >= 1 && path[0] == STR ('\\')) ;
	const auto r5x = BOOL (path.size () >= 1 && path[0] == STR ('/')) ;
	if (r4x || r5x)
		ret += _PCSTR_ ("/") ;
	const auto r6x = BOOL (r1x.length () >= 1 && r1x[r1x.access (0)] == _PCSTR_ (".")) ;
	const auto r7x = BOOL (r1x.length () >= 1 && r1x[r1x.access (0)] == _PCSTR_ ("..")) ;
	const auto r8x = BOOL (!r4x && !r5x && (r6x || r7x)) ;
	if (r8x)
		ret += _WORKINGPATH_ () ;
	for (INDEX i = 0 ; i < r2x.length () ; i++) {
		if (i != 0)
			ret += _PCSTR_ ("/") ;
		INDEX ix = r2x[r2x.access (i)] ;
		ret += r1x[ix] ;
	}
	const auto r10x = BOOL (ret.size () >= 1 && ret[0] == STR ('\\')) ;
	const auto r11x = BOOL (ret.size () >= 1 && ret[0] == STR ('/')) ;
	if (!r10x && !r11x)
		ret += _PCSTR_ ("/") ;
	return std::move (ret) ;
}

inline exports String<STR> _MODULEFILEPATH_ () {
	auto rax = String<STRA> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = readlink (_PCSTRA_ ("/proc/self/exe") ,rax.raw ().self ,VAR32 (rax.size ())) ;
	if (r1x < 0 || r1x >= rax.size ())
		rax.clear () ;
	String<STR> ret = _PARSESTRS_ (rax) ;
	ret = _PARSEFILEPATH_ (ret) ;
	ret += _PCSTR_ ("/") ;
	return std::move (ret) ;
}

inline exports String<STR> _MODULEFILENAME_ () {
	auto rax = String<STRA> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = readlink (_PCSTRA_ ("/proc/self/exe") ,rax.raw ().self ,VAR32 (rax.size ())) ;
	if (r1x < 0 || r1x >= rax.size ())
		rax.clear () ;
	const auto r2x = _PARSESTRS_ (rax) ;
	return _PARSEFILENAME_ (r2x) ;
}

inline exports BOOL _FINDDIRECTORY_ (const String<STR> &dire) popping {
	const auto r1x = UniqueRef<PTR<DIR>> ([&] (PTR<DIR> &me) {
		me = opendir (_BUILDSTRS_<STRA> (dire).raw ().self) ;
	} ,[] (PTR<DIR> &me) {
		if (me == NULL)
			return ;
		closedir (me) ;
	}) ;
	return r1x.self != NULL ;
}

inline exports void _BUILDDIRECTORY_ (const String<STR> &dire) {
	if (_FINDDIRECTORY_ (dire))
		return ;
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	const auto r1x = _DECOUPLEPATHNAME_ (_ABSOLUTEPATH_ (dire)) ;
	_DEBUG_ASSERT_ (r1x.length () >= 1) ;
	const auto r2x = BOOL (dire.size () >= 1 && dire[0] == STR ('\\')) ;
	const auto r3x = BOOL (dire.size () >= 1 && dire[0] == STR ('/')) ;
	if (r2x || r3x)
		rax += _PCSTR_ ("/") ;
	for (INDEX i = 0 ; i < r1x.length () ; i++) {
		if (i != 0)
			rax += _PCSTR_ ("/") ;
		INDEX ix = r1x.access (i) ;
		rax += r1x[ix] ;
		const auto r4x = r1x[ix].length () ;
		if (r4x > 1 && r1x[ix][r4x - 1] == STR (':'))
			continue ;
		mkdir (_BUILDSTRS_<STRA> (rax).raw ().self ,mode_t (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) ;
	}
}

inline exports void _ERASEDIRECTORY_ (const String<STR> &dire) {
	unlink (_BUILDSTRS_<STRA> (dire).raw ().self) ;
}

inline exports void _CLEARDIRECTORY_ (const String<STR> &dire) {
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
		_CALL_IF_ ([&] (BOOL &if_cond) {
			if (!rax[ix].P2)
				return (void) (if_cond = FALSE) ;
			_ERASEDIRECTORY_ (rax[ix].P1) ;
			rax.take () ;
		} ,[&] (BOOL &if_cond) {
			_ENUMDIRECTORY_ (rax[ix].P1 ,r1x ,r2x) ;
			rax[ix].P2 = TRUE ;
		}) ;
	}
}

inline exports void _ENUMDIRECTORY_ (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping {
	auto rax = String<STR> (DEFAULT_SHORTSTRING_SIZE::value) ;
	rax += dire ;
	rax += _PCSTR_ ("/") ;
	const auto r1x = rax.length () ;
	const auto r2x = UniqueRef<PTR<DIR>> ([&] (PTR<DIR> &me) {
		me = opendir (_BUILDSTRS_<STRA> (rax).raw ().self) ;
	} ,[] (PTR<DIR> &me) {
		if (me == NULL)
			return ;
		closedir (me) ;
	}) ;
	if (r2x == NULL)
		return ;
	while (TRUE) {
		const auto r3x = readdir (r2x) ;
		if (r3x == NULL)
			break ;
		const auto r4x = _PARSESTRS_ (String<STRA> (r3x->d_name)) ;
		if (r4x == _PCSTR_ ("."))
			continue ;
		if (r4x == _PCSTR_ (".."))
			continue ;
		rax += r4x ;
		auto &r1 = (_FINDDIRECTORY_ (rax)) ? dire_proc : file_proc ;
		if (!r1.exist ())
			continue ;
		r1 (rax) ;
		rax[r1x] = 0 ;
	}
}
} ;

class StreamLoader::Implement final :private Interface {
private:
	UniqueRef<PTR<FILE>> mFile ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		mFile = UniqueRef<PTR<FILE>> ([&] (PTR<FILE> &me) {
			me = std::fopen (_BUILDSTRS_<STRA> (file).raw ().self ,_PCSTRA_ ("wb+")) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FILE> &me) {
			_DEBUG_ASSERT_ (me != NULL) ;
			std::fclose (me) ;
		}) ;
	}

	void read (const PhanBuffer<BYTE> &data) popping {
		_DEBUG_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		const auto r1x = LENGTH (std::fread (data.self ,1 ,data.size () ,mFile)) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		_MEMFILL_ (PTRTOARR[&data.self[r1x]] ,(data.size () - r1x) ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (data.size () >= 0 && data.size () < VAR32_MAX) ;
		const auto r1x = LENGTH (std::fwrite (data.self ,1 ,data.size () ,mFile)) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void flush () {
		std::fflush (mFile) ;
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

	PhanBuffer<BYTE> watch () {
		_STATIC_WARNING_ ("unimplemented") ;
		return PhanBuffer<BYTE> () ;
	}

	PhanBuffer<const BYTE> watch () const {
		_STATIC_WARNING_ ("unimplemented") ;
		return PhanBuffer<const BYTE> () ;
	}

	void flush () {
		_STATIC_WARNING_ ("unimplemented") ;
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

inline exports PhanBuffer<BYTE> BufferLoader::watch () {
	return mThis.rebind<Implement> ()->watch () ;
}

inline exports PhanBuffer<const BYTE> BufferLoader::watch () const {
	return mThis.rebind<Implement> ()->watch () ;
}

inline exports void BufferLoader::flush () {
	mThis.rebind<Implement> ()->flush () ;
}

class FileSystemService::Implement final :private FileSystemService::Abstract {
private:
	friend FileSystemService ;
	friend StrongRef<Implement> ;

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

	void build_directory (const String<STR> &dire) override {
		_BUILDDIRECTORY_ (dire) ;
	}

	void erase_directory (const String<STR> &dire) override {
		_ERASEDIRECTORY_ (dire) ;
	}

	void clear_directory (const String<STR> &dire) override {
		_CLEARDIRECTORY_ (dire) ;
	}

	void enum_directory (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping override {
		_ENUMDIRECTORY_ (dire ,file_proc ,dire_proc) ;
	}
} ;

inline exports FileSystemService::FileSystemService () {
	mThis = StrongRef<Implement>::make () ;
}
} ;