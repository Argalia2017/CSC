#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : require 'csc_filesystem.hpp'"
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("leftvalue")
#pragma push_macro ("rightvalue")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#pragma push_macro ("switch_case")
#pragma push_macro ("discard")
#undef self
#undef implicit
#undef popping
#undef leftvalue
#undef rightvalue
#undef imports
#undef exports
#undef switch_case
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
#pragma pop_macro ("leftvalue")
#pragma pop_macro ("rightvalue")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#pragma pop_macro ("switch_case")
#pragma pop_macro ("discard")
#endif

namespace CSC {
namespace api {
using ::DIR ;

using ::open ;
using ::close ;
using ::lseek ;
using ::read ;
using ::write ;
using ::opendir ;
using ::readdir ;
using ::closedir ;
using ::link ;
using ::unlink ;
using ::rename ;
using ::stat ;
using ::getcwd ;
using ::readlink ;
using ::mkdir ;
using ::rmdir ;
} ;

inline exports AutoBuffer<BYTE> FileSystemProc::load_file (const String<STR> &file) popping {
	const auto r1x = StringProc::build_strs<STRA> (file) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = api::open (r1x.raw ().self ,O_RDONLY) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		api::close (me) ;
	}) ;
	const auto r3x = LENGTH (api::lseek (r2x.self ,0 ,SEEK_END)) ;
	_DYNAMIC_ASSERT_ (r3x >= 0 && r3x < VAR32_MAX) ;
	api::lseek (r2x.self ,0 ,SEEK_SET) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r3x) ;
	const auto r4x = LENGTH (api::read (r2x.self ,ret.self ,VAR32 (r3x))) ;
	_DYNAMIC_ASSERT_ (r4x == r3x) ;
	return _MOVE_ (ret) ;
}

inline exports void FileSystemProc::load_file (const String<STR> &file ,const PhanBuffer<BYTE> &data) {
	const auto r1x = StringProc::build_strs<STRA> (file) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = api::open (r1x.raw ().self ,O_RDONLY) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		api::close (me) ;
	}) ;
	const auto r3x = LENGTH (api::lseek (r2x.self ,0 ,SEEK_END)) ;
	_DYNAMIC_ASSERT_ (r3x > 0 && r3x <= data.size ()) ;
	api::lseek (r2x.self ,0 ,SEEK_SET) ;
	const auto r4x = LENGTH (api::read (r2x.self ,data.self ,VAR32 (r3x))) ;
	_DYNAMIC_ASSERT_ (r4x == r3x) ;
	BasicProc::mem_fill (PTRTOARR[&data.self[r3x]] ,(data.size () - r3x) ,BYTE (0X00)) ;
}

inline exports void FileSystemProc::save_file (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	const auto r1x = StringProc::build_strs<STRA> (file) ;
	_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		const auto r3x = VAR32 (O_CREAT | O_WRONLY | O_TRUNC) ;
		const auto r4x = VAR32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
		me = api::open (r1x.raw ().self ,r3x ,r4x) ;
		_DYNAMIC_ASSERT_ (me >= 0) ;
	} ,[] (VAR32 &me) {
		api::close (me) ;
	}) ;
	const auto r5x = LENGTH (api::write (r2x.self ,data.self ,VAR32 (data.size ()))) ;
	_DYNAMIC_ASSERT_ (r5x == data.size ()) ;
}

inline exports PhanBuffer<const BYTE> FileSystemProc::load_assert_file (const FLAG &resource) popping {
	_STATIC_WARNING_ ("unimplemented") ;
	_DYNAMIC_ASSERT_ (FALSE) ;
	return PhanBuffer<const BYTE> () ;
}

inline exports BOOL FileSystemProc::find_file (const String<STR> &file) popping {
	const auto r1x = StringProc::build_strs<STRA> (file) ;
	const auto r2x = UniqueRef<VAR32> ([&] (VAR32 &me) {
		me = api::open (r1x.raw ().self ,O_RDONLY) ;
	} ,[] (VAR32 &me) {
		if (me < 0)
			return ;
		api::close (me) ;
	}) ;
	if (r2x.self < 0)
		return FALSE ;
	return TRUE ;
}

namespace U {
inline BOOL static_find_juntion (const String<STRA> &dire) popping {
	using HDIR = PTR<api::DIR> ;
	const auto r1x = UniqueRef<HDIR> ([&] (HDIR &me) {
		me = api::opendir (dire.raw ().self) ;
	} ,[] (HDIR &me) {
		if (me == NULL)
			return ;
		api::closedir (me) ;
	}) ;
	if (r1x.self == NULL)
		return FALSE ;
	return TRUE ;
}
} ;

inline exports void FileSystemProc::erase_file (const String<STR> &file) {
	const auto r1x = StringProc::build_strs<STRA> (file) ;
	const auto r2x = U::static_find_juntion (r1x) ;
	if (r2x)
		return ;
	const auto r3x = api::unlink (r1x.raw ().self) ;
	_STATIC_UNUSED_ (r3x) ;
}

inline exports void FileSystemProc::copy_file (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = FileSystemProc::find_file (dst_file) ;
	_DYNAMIC_ASSERT_ (!r1x) ;
	const auto r2x = FileSystemProc::load_file (src_file) ;
	FileSystemProc::save_file (dst_file ,PhanBuffer<const BYTE>::make (r2x)) ;
}

inline exports void FileSystemProc::move_file (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = StringProc::build_strs<STRA> (src_file) ;
	const auto r2x = StringProc::build_strs<STRA> (dst_file) ;
	const auto r3x = FileSystemProc::find_file (dst_file) ;
	_DYNAMIC_ASSERT_ (!r3x) ;
	const auto r4x = api::rename (r1x.raw ().self ,r2x.raw ().self) ;
	_STATIC_UNUSED_ (r4x) ;
}

inline exports void FileSystemProc::link_file (const String<STR> &dst_file ,const String<STR> &src_file) {
	const auto r1x = StringProc::build_strs<STRA> (src_file) ;
	const auto r2x = StringProc::build_strs<STRA> (dst_file) ;
	const auto r3x = FileSystemProc::find_file (dst_file) ;
	_DYNAMIC_ASSERT_ (!r3x) ;
	const auto r4x = api::link (r1x.raw ().self ,r2x.raw ().self) ;
	_STATIC_UNUSED_ (r4x) ;
}

inline exports BOOL FileSystemProc::identical_file (const String<STR> &file1 ,const String<STR> &file2) popping {
	const auto r1x = StringProc::build_strs<STRA> (file1) ;
	const auto r2x = StringProc::build_strs<STRA> (file2) ;
	auto rax = ARRAY2<DEF<struct stat>> () ;
	_ZERO_ (rax[0]) ;
	const auto r3x = api::stat (r1x.raw ().self ,DEPTR[rax[0]]) ;
	if (r3x != 0)
		return FALSE ;
	if (rax[0].st_nlink == 0)
		return FALSE ;
	_ZERO_ (rax[1]) ;
	const auto r4x = api::stat (r2x.raw ().self ,DEPTR[rax[1]]) ;
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

inline exports String<STR> FileSystemProc::parse_path_name (const String<STR> &file) {
	String<STR> ret = String<STR> (DEFAULT_FILEPATH_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = BasicProc::mem_rchr (r2x.self ,r1x ,STR ('\\')) ;
	const auto r4x = BasicProc::mem_rchr (r2x.self ,r1x ,STR ('/')) ;
	const auto r5x = MathProc::maxof (r3x ,r4x ,VAR_ZERO) ;
	BasicProc::mem_copy (ret.raw ().self ,r2x.self ,r5x) ;
	return _MOVE_ (ret) ;
}

inline exports String<STR> FileSystemProc::parse_file_name (const String<STR> &file) {
	String<STR> ret = String<STR> (DEFAULT_FILEPATH_SIZE::value) ;
	const auto r1x = file.length () ;
	const auto r2x = file.raw () ;
	const auto r3x = BasicProc::mem_rchr (r2x.self ,r1x ,STR ('\\')) ;
	const auto r4x = BasicProc::mem_rchr (r2x.self ,r1x ,STR ('/')) ;
	const auto r5x = MathProc::maxof (r3x ,r4x) + 1 ;
	BasicProc::mem_copy (ret.raw ().self ,PTRTOARR[&r2x.self[r5x]] ,(r1x - r5x)) ;
	return _MOVE_ (ret) ;
}

inline exports Deque<String<STR>> FileSystemProc::decouple_path_name (const String<STR> &file) {
	const auto r1x = _CALL_ ([&] () {
		if (!file.empty ())
			return file.raw () ;
		return PhanBuffer<const STR> () ;
	}) ;
	auto rax = TextReader<STR> (r1x) ;
	const auto r2x = rax.attr () ;
	r2x.modify_space (STR ('\\') ,0) ;
	r2x.modify_space (STR ('/') ,0) ;
	auto rbx = STR () ;
	Deque<String<STR>> ret ;
	INDEX ix = ret.insert () ;
	rax.share () >> rbx ;
	if (r2x.varify_space (rbx))
		rax >> rbx ;
	while (TRUE) {
		rax >> ret[ix] ;
		if (ret[ix].empty ())
			break ;
		ix = ret.insert () ;
		rax >> rbx ;
		if (rbx == r2x.varify_ending_item ())
			break ;
		_DYNAMIC_ASSERT_ (r2x.varify_space (rbx)) ;
	}
	ret.pop () ;
	rax >> TextReader<STR>::EOS ;
	return _MOVE_ (ret) ;
}

inline exports String<STR> FileSystemProc::working_path () {
	auto rax = String<STRA> (DEFAULT_FILEPATH_SIZE::value) ;
	if switch_case (TRUE) {
		const auto r1x = api::getcwd (rax.raw ().self ,VAR32 (rax.size ())) ;
		if (r1x != NULL)
			discard ;
		rax.clear () ;
	}
	if switch_case (TRUE) {
		const auto r2x = rax.length () ;
		if (r2x < 1)
			discard ;
		if (rax[r2x - 1] == STRA ('\\'))
			discard ;
		if (rax[r2x - 1] == STRA ('/'))
			discard ;
		rax += _PCSTRA_ ("/") ;
	}
	return StringProc::parse_strs (rax) ;
}

namespace U {
inline Deque<INDEX> static_relative_path_name (const Deque<String<STR>> &path_name) {
	Deque<INDEX> ret = Deque<INDEX> (path_name.length ()) ;
	for (auto &&i : _RANGE_ (0 ,path_name.length ())) {
		INDEX ix = path_name.access (i) ;
		if (path_name[ix] == _PCSTR_ ("."))
			continue ;
		auto fax = TRUE ;
		if switch_case (fax) {
			if (ret.empty ())
				discard ;
			if (!(path_name[ix] == _PCSTR_ ("..")))
				discard ;
			if (!(path_name[ret[ret.tail ()]] != _PCSTR_ ("..")))
				discard ;
			ret.pop () ;
		}
		if switch_case (fax) {
			ret.add (ix) ;
		}
	}
	return _MOVE_ (ret) ;
}
} ;

inline exports String<STR> FileSystemProc::absolute_path (const String<STR> &path) {
	String<STR> ret = String<STR> (DEFAULT_FILEPATH_SIZE::value) ;
	auto rax = FileSystemProc::decouple_path_name (path) ;
	auto fax = TRUE ;
	if switch_case (fax) {
		if (path.size () < 1)
			discard ;
		if (path[0] != STR ('\\'))
			if (path[0] != STR ('/'))
				discard ;
		ret += _PCSTR_ ("/") ;
	}
	if switch_case (fax) {
		if (rax.length () < 1)
			discard ;
		if (rax[rax.access (0)] != _PCSTR_ ("."))
			if (rax[rax.access (0)] != _PCSTR_ (".."))
				discard ;
		const auto r1x = FileSystemProc::working_path () ;
		if switch_case (TRUE) {
			auto tmp = FileSystemProc::decouple_path_name (r1x) ;
			tmp.appand (_MOVE_ (rax)) ;
			rax = _MOVE_ (tmp) ;
		}
		if (r1x.size () < 1)
			discard ;
		if (r1x[0] != STR ('\\'))
			if (r1x[0] != STR ('/'))
				discard ;
		ret += _PCSTR_ ("/") ;
	}
	const auto r2x = U::static_relative_path_name (rax) ;
	for (auto &&i : _RANGE_ (0 ,r2x.length ())) {
		if (i > 0)
			ret += _PCSTR_ ("/") ;
		INDEX ix = r2x[r2x.access (i)] ;
		ret += rax[ix] ;
	}
	if switch_case (TRUE) {
		const auto r3x = ret.length () ;
		if (r3x < 1)
			discard ;
		if (ret[r3x - 1] == STR ('\\'))
			discard ;
		if (ret[r3x - 1] == STR ('/'))
			discard ;
		ret += _PCSTR_ ("/") ;
	}
	return _MOVE_ (ret) ;
}

inline exports const String<STR> &FileSystemProc::module_file_path () popping {
	return _CACHE_ ([&] () {
		auto rax = String<STRA> (DEFAULT_FILEPATH_SIZE::value) ;
		const auto r1x = api::readlink (_PCSTRA_ ("/proc/self/exe") ,rax.raw ().self ,VAR32 (rax.size ())) ;
		if (!(r1x >= 0 && r1x < rax.size ()))
			rax.clear () ;
		String<STR> ret = StringProc::parse_strs (rax) ;
		ret = FileSystemProc::parse_path_name (ret) ;
		ret += _PCSTR_ ("/") ;
		return _MOVE_ (ret) ;
	}) ;
}

inline exports const String<STR> &FileSystemProc::module_file_name () popping {
	return _CACHE_ ([&] () {
		auto rax = String<STRA> (DEFAULT_FILEPATH_SIZE::value) ;
		const auto r1x = api::readlink (_PCSTRA_ ("/proc/self/exe") ,rax.raw ().self ,VAR32 (rax.size ())) ;
		if (!(r1x >= 0 && r1x < rax.size ()))
			rax.clear () ;
		const auto r2x = StringProc::parse_strs (rax) ;
		return FileSystemProc::parse_file_name (r2x) ;
	}) ;
}

inline exports BOOL FileSystemProc::find_directory (const String<STR> &dire) popping {
	using HDIR = PTR<api::DIR> ;
	const auto r1x = StringProc::build_strs<STRA> (dire) ;
	const auto r2x = UniqueRef<HDIR> ([&] (HDIR &me) {
		me = api::opendir (r1x.raw ().self) ;
	} ,[] (HDIR &me) {
		if (me == NULL)
			return ;
		api::closedir (me) ;
	}) ;
	if (r2x.self == NULL)
		return FALSE ;
	return TRUE ;
}

inline exports BOOL FileSystemProc::lock_directory (const String<STR> &dire) popping {
	BOOL ret = FALSE ;
	const auto r1x = String<STR>::make (dire ,_PCSTR_ ("/") ,_PCSTR_ (".lockdirectory")) ;
	const auto r2x = GlobalRuntime::process_pid () ;
	const auto r3x = GlobalRuntime::process_info (r2x) ;
	auto fax = TRUE ;
	if switch_case (fax) {
		const auto r4x = FileSystemProc::find_file (r1x) ;
		if (!r4x)
			discard ;
		const auto r5x = FileSystemProc::load_file (r1x) ;
		if (r5x.size () != r3x.size ())
			discard ;
		const auto r6x = GlobalRuntime::process_info_pid (PhanBuffer<const STRU8>::make (r5x)) ;
		const auto r7x = GlobalRuntime::process_info (r6x) ;
		_DEBUG_ASSERT_ (r7x.size () == r5x.size ()) ;
		const auto r8x = BasicProc::mem_compr (r7x.self ,r5x.self ,r5x.size ()) ;
		if (r8x != 0)
			discard ;
		ret = CSC::BOOL (r2x == r6x) ;
	}
	if switch_case (fax) {
		auto &r9x = _CACHE_ ([&] () {
			return UniqueRef<String<STR>> ([&] (String<STR> &me) {
				me = r1x ;
				FileSystemProc::save_file (r1x ,PhanBuffer<const CSC::BYTE>::make (r3x)) ;
			} ,[] (String<STR> &me) {
				FileSystemProc::erase_file (me) ;
			}) ;
		}) ;
		_STATIC_UNUSED_ (r9x) ;
		ret = TRUE ;
	}
	return _MOVE_ (ret) ;
}

inline exports void FileSystemProc::build_directory (const String<STR> &dire) {
	if (FileSystemProc::find_directory (dire))
		return ;
	auto rax = String<STR> (DEFAULT_FILEPATH_SIZE::value) ;
	const auto r1x = FileSystemProc::absolute_path (dire) ;
	const auto r2x = FileSystemProc::decouple_path_name (r1x) ;
	_DEBUG_ASSERT_ (r2x.length () >= 1) ;
	if switch_case (TRUE) {
		if (dire.size () < 1)
			discard ;
		if (dire[0] != STR ('\\'))
			if (dire[0] != STR ('/'))
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
		const auto r4x = StringProc::build_strs<STRA> (rax) ;
		const auto r5x = VAR32 (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) ;
		api::mkdir (r4x.raw ().self ,r5x) ;
	}
}

inline exports void FileSystemProc::erase_directory (const String<STR> &dire) {
	const auto r1x = StringProc::build_strs<STRA> (dire) ;
	const auto r2x = api::rmdir (r1x.raw ().self) ;
	_STATIC_UNUSED_ (r2x) ;
	const auto r3x = U::static_find_juntion (r1x) ;
	if (!r3x)
		return ;
	const auto r4x = api::unlink (r1x.raw ().self) ;
	_STATIC_UNUSED_ (r4x) ;
}

//@warn: recursive call with junction(const symbolic &link) may cause endless loop
inline exports void FileSystemProc::enum_directory (const String<STR> &dire ,Deque<String<STR>> &file_list ,Deque<String<STR>> &dire_list) {
	using HDIR = PTR<api::DIR> ;
	auto rax = String<STR> (DEFAULT_FILEPATH_SIZE::value) ;
	rax += dire ;
	rax += _PCSTR_ ("/") ;
	const auto r1x = rax.length () ;
	const auto r2x = StringProc::build_strs<STRA> (rax) ;
	const auto r3x = UniqueRef<HDIR> ([&] (HDIR &me) {
		me = api::opendir (r2x.raw ().self) ;
	} ,[] (HDIR &me) {
		if (me == NULL)
			return ;
		api::closedir (me) ;
	}) ;
	if (r3x.self == NULL)
		return ;
	while (TRUE) {
		const auto r4x = api::readdir (r3x.self) ;
		if (r4x == NULL)
			break ;
		const auto r5x = StringProc::parse_strs (String<STRA> (PTRTOARR[r4x->d_name])) ;
		if switch_case (TRUE) {
			if (r5x == _PCSTR_ ("."))
				discard ;
			if (r5x == _PCSTR_ (".."))
				discard ;
			rax += r5x ;
			auto &r6x = _SWITCH_ (
				(FileSystemProc::find_directory (rax)) ? dire_list :
				file_list) ;
			r6x.add (rax) ;
		}
		rax[r1x] = 0 ;
	}
}

inline exports void FileSystemProc::clear_directory (const String<STR> &dire) {
	auto rax = Deque<PACK<String<STR> ,BOOL>> () ;
	auto rbx = ARRAY2<Deque<String<STR>>> () ;
	rbx[0].clear () ;
	rbx[1].clear () ;
	FileSystemProc::enum_directory (dire ,rbx[0] ,rbx[1]) ;
	for (auto &&i : rbx[0])
		FileSystemProc::erase_file (i) ;
	for (auto &&i : rbx[1])
		rax.add (PACK<String<STR> ,BOOL> {i ,FALSE}) ;
	_DYNAMIC_ASSERT_ (rax.length () <= DEFAULT_DIRECTORY_SIZE::value) ;
	while (TRUE) {
		if (rax.empty ())
			break ;
		INDEX ix = rax.tail () ;
		FileSystemProc::erase_directory (rax[ix].P1) ;
		auto fax = TRUE ;
		if switch_case (fax) {
			if (!rax[ix].P2)
				discard ;
			rax.pop () ;
		}
		if switch_case (fax) {
			rbx[0].clear () ;
			rbx[1].clear () ;
			FileSystemProc::enum_directory (rax[ix].P1 ,rbx[0] ,rbx[1]) ;
			for (auto &&i : rbx[0])
				FileSystemProc::erase_file (i) ;
			for (auto &&i : rbx[1])
				rax.add (PACK<String<STR> ,BOOL> {i ,FALSE}) ;
			_DYNAMIC_ASSERT_ (rax.length () <= DEFAULT_DIRECTORY_SIZE::value) ;
			rax[ix].P2 = TRUE ;
		}
	}
}

class StreamLoader::Implement {
private:
	UniqueRef<VAR32> mReadFile ;
	UniqueRef<VAR32> mWriteFile ;

public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		const auto r1x = StringProc::build_strs<STRA> (file) ;
		mWriteFile = UniqueRef<VAR32> ([&] (VAR32 &me) {
			const auto r2x = VAR32 (O_CREAT | O_WRONLY | O_APPEND) ;
			const auto r3x = VAR32 (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = api::open (r1x.raw ().self ,r2x ,r3x) ;
			_DYNAMIC_ASSERT_ (me >= 0) ;
		} ,[] (VAR32 &me) {
			api::close (me) ;
		}) ;
		mReadFile = UniqueRef<VAR32> ([&] (VAR32 &me) {
			me = api::open (r1x.raw ().self ,O_RDONLY) ;
			_DYNAMIC_ASSERT_ (me >= 0) ;
		} ,[] (VAR32 &me) {
			api::close (me) ;
		}) ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
		const auto r1x = LENGTH (api::read (mReadFile ,data.self ,VAR32 (data.size ()))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x < VAR32_MAX) ;
		BasicProc::mem_fill (PTRTOARR[&data.self[r1x]] ,(data.size () - r1x) ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (data.size () < VAR32_MAX) ;
		const auto r1x = LENGTH (api::write (mWriteFile ,data.self ,VAR32 (data.size ()))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void flush () {
		_STATIC_WARNING_ ("noop") ;
	}
} ;

inline exports StreamLoader::StreamLoader (const String<STR> &file) {
	mThis = StrongRef<Implement>::make (file) ;
}

inline exports void StreamLoader::read (const PhanBuffer<BYTE> &data) {
	mThis->read (data) ;
}

inline exports void StreamLoader::write (const PhanBuffer<const BYTE> &data) {
	mThis->write (data) ;
}

inline exports void StreamLoader::flush () {
	mThis->flush () ;
}

class BufferLoader::Implement {
public:
	Implement () = delete ;

	explicit Implement (const String<STR> &file) {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	explicit Implement (const String<STR> &file ,const LENGTH &file_len) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	explicit Implement (const String<STR> &file ,const BOOL &cache) {
		_DEBUG_ASSERT_ (cache) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	explicit Implement (const String<STR> &file ,const LENGTH &file_len ,const BOOL &cache) {
		_DEBUG_ASSERT_ (file_len >= 0 && file_len < VAR32_MAX) ;
		_DEBUG_ASSERT_ (cache) ;
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}

	PhanBuffer<BYTE> watch () leftvalue {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
		return PhanBuffer<BYTE> () ;
	}

	PhanBuffer<const BYTE> watch () const leftvalue {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
		return PhanBuffer<const BYTE> () ;
	}

	void flush () {
		_STATIC_WARNING_ ("unimplemented") ;
		_DYNAMIC_ASSERT_ (FALSE) ;
	}
} ;

inline exports BufferLoader::BufferLoader (const String<STR> &file) {
	mThis = StrongRef<Implement>::make (file) ;
}

inline exports BufferLoader::BufferLoader (const String<STR> &file ,const LENGTH &file_len) {
	mThis = StrongRef<Implement>::make (file ,file_len) ;
}

inline exports BufferLoader::BufferLoader (const String<STR> &file ,const BOOL &cache) {
	mThis = StrongRef<Implement>::make (file ,cache) ;
}

inline exports BufferLoader::BufferLoader (const String<STR> &file ,const LENGTH &file_len ,const BOOL &cache) {
	mThis = StrongRef<Implement>::make (file ,file_len ,cache) ;
}

inline exports PhanBuffer<BYTE> BufferLoader::watch () leftvalue {
	return _FORWARD_<Implement &> (mThis).watch () ;
}

inline exports PhanBuffer<const BYTE> BufferLoader::watch () const leftvalue {
	return _FORWARD_<const Implement &> (mThis).watch () ;
}

inline exports void BufferLoader::flush () {
	mThis->flush () ;
}

class FileSystemService::Implement
	:public FileSystemService::Abstract {
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