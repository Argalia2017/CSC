#pragma once

#ifndef __CSC_FILE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma system_header
#endif

#include "csc_file.hpp"

#ifdef __CSC_CONFIG_STRW__
#error "∑(っ°Д° ;)っ : unsupported"
#endif

#include "csc_end.h"
#ifdef __CSC_SYSTEM_LINUX__
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <termios.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#endif

#include <cstdio>
#include "csc_begin.h"

inline namespace {
using HDIR = CSC::DEF<DIR *> ;
using HDIRENT = CSC::DEF<dirent *> ;
using STAT_INFO = CSC::DEF<struct stat> ;
using TERMIOS_INFO = CSC::DEF<struct termios> ;
using HIOF = CSC::DEF<FILE *> ;
} ;

namespace std {
inline namespace {
using ::open ;
using ::close ;
using ::read ;
using ::write ;
using ::stat ;
using ::lstat ;
} ;
} ;

namespace CSC {
struct PathTree {
	String<Str> mPathName ;
	Deque<Index> mSeparator ;
} ;

class PathImplHolder final implement Fat<PathHolder ,PathLayout> {
public:
	void initialize (RR<String<Str>> pathname) override {
		self.mThis = Ref<PathTree>::make () ;
		self.mThis->mPathName = move (pathname) ;
		self.mThis->mSeparator.add (NONE) ;
		const auto r1x = self.mThis->mPathName.length () ;
		for (auto &&i : range (0 ,r1x)) {
			if (!is_separator (self.mThis->mPathName[i]))
				continue ;
			self.mThis->mSeparator.add (i) ;
			self.mThis->mPathName[i] = Str ('/') ;
		}
		self.mThis->mSeparator.add (r1x) ;
		if ifdo (TRUE) {
			if (r1x == 0)
				discard ;
			Index ix = self.mThis->mSeparator[self.mThis->mSeparator.length () - 2] ;
			if (ix != r1x - 1)
				discard ;
			self.mThis->mPathName.trunc (ix) ;
			self.mThis->mSeparator.pop () ;
		}
		if ifdo (TRUE) {
			if (self.mThis->mSeparator.length () != 2)
				discard ;
			Index ix = self.mThis->mSeparator[0] + 1 ;
			Index iy = self.mThis->mSeparator[1] ;
			if (!is_root (self.mThis->mPathName.segment (ix ,iy)))
				discard ;
			self.mThis->mPathName = String<Str>::make (self.mThis->mPathName ,slice ("/") ,slice (".")) ;
			self.mThis->mSeparator.add (iy + 2) ;
		}
		assume (self.mThis->mSeparator.length () >= 2) ;
	}

	void initialize (CR<Deque<String<Str>>> pathname) override {
		auto rax = String<Str>::make () ;
		Index ix = 0 ;
		for (auto &&i : pathname.iter ()) {
			if ifdo (TRUE) {
				if (i == 0)
					discard ;
				rax.splice (ix ,slice ("/")) ;
				ix++ ;
			}
			rax.splice (ix ,pathname[i]) ;
			ix += pathname[i].length () ;
		}
		initialize (move (rax)) ;
	}

	void initialize (CR<PathLayout> that) override {
		self.mThis = that.mThis.share () ;
	}

	Bool is_separator (CR<Stru32> str) const {
		if (str == Stru32 ('\\'))
			return TRUE ;
		if (str == Stru32 ('/'))
			return TRUE ;
		return FALSE ;
	}

	String<Str> fetch () const override {
		if (self.mThis == NULL)
			return String<Str>::zero () ;
		return self.mThis->mPathName ;
	}

	PathLayout child (CR<Slice> name) const override {
		return Path (String<Str>::make (fetch () ,slice ("/") ,name)) ;
	}

	PathLayout child (CR<Format> name) const override {
		return Path (String<Str>::make (fetch () ,slice ("/") ,name)) ;
	}

	PathLayout child (CR<String<Str>> name) const override {
		return Path (String<Str>::make (fetch () ,slice ("/") ,name)) ;
	}

	Array<PathLayout> list () const override {
		auto rax = HDIRENT (NULL) ;
		const auto r1x = UniqueRef<HDIR> ([&] (VR<HDIR> me) {
			me = opendir (self.mThis->mPathName) ;
		} ,[&] (VR<HDIR> me) {
			if (me == NULL)
				return ;
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<Str>> () ;
		if ifdo (TRUE) {
			if (r1x == NULL)
				discard ;
			const auto r2x = Slice (address (rax->d_name) ,3 ,SIZE_OF<Str>::expr) ;
			noop (r2x) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice (".")) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r1x) ;
				if (rax == NULL)
					break ;
				rbx.add (Slice (rax->d_name)) ;
			}
		}
		Array<PathLayout> ret = Array<PathLayout> (rbx.length ()) ;
		for (auto &&i : ret.iter ())
			ret[i] = child (rbx[i]) ;
		return move (ret) ;
	}

	Array<PathLayout> list (CR<Length> size_) const override {
		auto rax = HDIRENT (NULL) ;
		const auto r1x = UniqueRef<HDIR> ([&] (VR<HDIR> me) {
			me = opendir (self.mThis->mPathName) ;
		} ,[&] (VR<HDIR> me) {
			if (me == NULL)
				return ;
			closedir (me) ;
		}) ;
		auto rbx = Deque<String<Str>> () ;
		if ifdo (TRUE) {
			if (r1x == NULL)
				discard ;
			const auto r2x = Slice (address (rax->d_name) ,3 ,SIZE_OF<Str>::expr) ;
			noop (r2x) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice (".")) ;
			rax = readdir (r1x) ;
			assert (r2x.eos () == slice ("..")) ;
			while (TRUE) {
				rax = readdir (r1x) ;
				if (rax == NULL)
					break ;
				if (rbx.length () >= size_)
					break ;
				rbx.add (Slice (rax->d_name)) ;
			}
		}
		assume (rbx.length () == size_) ;
		Array<PathLayout> ret = Array<PathLayout> (size_) ;
		for (auto &&i : range (0 ,size_))
			ret[i] = child (rbx[i]) ;
		return move (ret) ;
	}

	Bool equal (CR<PathLayout> that) const override {
		const auto r1x = inline_compr (self.mThis.exist () ,that.mThis.exist ()) ;
		if (r1x != ZERO)
			return FALSE ;
		if (!self.mThis.exist ())
			return FALSE ;
		return self.mThis->mPathName == that.mThis->mPathName ;
	}

	Bool is_file () const override {
		if (self.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::stat (self.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	Bool is_dire () const override {
		if (self.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::stat (self.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISDIR (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	Bool is_link () const override {
		if (self.mThis == NULL)
			return FALSE ;
		auto rax = STAT_INFO () ;
		inline_memset (rax) ;
		const auto r1x = std::lstat (self.mThis->mPathName ,(&rax)) ;
		if (r1x != 0)
			return FALSE ;
		if (!S_ISLNK (rax.st_mode))
			return FALSE ;
		return TRUE ;
	}

	PathLayout symbolic () const override {
		PathLayout ret ;
		auto act = TRUE ;
		if ifdo (act) {
			if (!is_link ())
				discard ;
			auto rax = String<Str>::make () ;
			const auto r1x = Index (readlink (self.mThis->mPathName ,rax ,csc_size_t (rax.size ()))) ;
			rax.trunc (r1x) ;
			ret = Path (rax) ;
		}
		if ifdo (act) {
			ret.mThis = self.mThis.share () ;
		}
		return move (ret) ;
	}

	PathLayout absolute () const override {
		auto rax = Deque<String<Str>> () ;
		auto rbx = decouple () ;
		while (TRUE) {
			if (rbx.empty ())
				break ;
			auto act = TRUE ;
			if ifdo (act) {
				if (rbx[0] != slice ("."))
					discard ;
				if ifdo (TRUE) {
					if (!rax.empty ())
						discard ;
					auto rcx = String<Str>::make () ;
					if ifdo (TRUE) {
						const auto r1x = getcwd (rcx ,csc_size_t (rcx.size ())) ;
						if (r1x != NULL)
							discard ;
						rcx.clear () ;
					}
					rax = Path (move (rcx)).decouple () ;
				}
				rbx.take () ;
			}
			if ifdo (act) {
				if (rbx[0] != slice (".."))
					discard ;
				if ifdo (TRUE) {
					if (!rax.empty ())
						discard ;
					auto rcx = String<Str>::make () ;
					if ifdo (TRUE) {
						const auto r1x = getcwd (rcx ,csc_size_t (rcx.size ())) ;
						if (r1x != NULL)
							discard ;
						rcx.clear () ;
					}
					rax = Path (move (rcx)).decouple () ;
				}
				if ifdo (TRUE) {
					if (rax.length () > 1)
						discard ;
					if (!is_root (rax[0].segment (0 ,rax[0].length ())))
						discard ;
					rax.add (String<Str>::zero ()) ;
				}
				rax.pop () ;
				rbx.take () ;
			}
			if ifdo (act) {
				rax.add (move (rbx[0])) ;
				rbx.take () ;
			}
		}
		return Path (rax) ;
	}

	Bool is_root (CR<Slice> str) const {
		if (str.size () == 0)
			return TRUE ;
		if (str.size () != 2)
			return FALSE ;
		if (StreamProc::is_alpha (str[0]))
			if (str[1] == Stru32 (':'))
				return TRUE ;
		return FALSE ;
	}

	Deque<String<Str>> decouple () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		Deque<String<Str>> ret = Deque<String<Str>> (r1x) ;
		for (auto &&i : range (0 ,r1x - 1)) {
			Index ix = self.mThis->mSeparator[i] + 1 ;
			Index iy = self.mThis->mSeparator[i + 1] ;
			const auto r2x = self.mThis->mPathName.segment (ix ,iy) ;
			ret.add (r2x) ;
		}
		return move (ret) ;
	}

	String<Str> path () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		Index ix = self.mThis->mSeparator[0] + 1 ;
		Index iy = self.mThis->mSeparator[r1x - 2] + 1 ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	String<Str> name () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		Index ix = self.mThis->mSeparator[r1x - 2] + 1 ;
		Index iy = self.mThis->mSeparator[r1x - 1] ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	String<Str> stem () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		Index ix = self.mThis->mSeparator[r1x - 2] + 1 ;
		Index iy = find_last_dot_word () ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	String<Str> extension () const override {
		const auto r1x = self.mThis->mSeparator.length () ;
		Index ix = find_last_dot_word () ;
		Index iy = self.mThis->mSeparator[r1x - 1] ;
		return self.mThis->mPathName.segment (ix ,iy) ;
	}

	Index find_last_dot_word () const {
		const auto r1x = self.mThis->mSeparator.length () ;
		Index ix = self.mThis->mSeparator[r1x - 1] - 1 ;
		while (TRUE) {
			if (ix <= 0)
				break ;
			if (self.mThis->mPathName[ix] == Stru32 ('.'))
				return ix ;
			if (!StreamProc::is_word (self.mThis->mPathName[ix]))
				break ;
			ix-- ;
		}
		return self.mThis->mSeparator[r1x - 1] ;
	}
} ;

static const auto mPathExternal = External<PathHolder ,PathLayout> (PathImplHolder ()) ;

class FileProcImplHolder final implement Fat<FileProcHolder ,FileProcLayout> {
private:
	using FILEPROC_RETRY_TIME = RANK3 ;

public:
	void initialize () override {
		self.mMutex = NULL ;
	}

	RefBuffer<Byte> load_file (CR<String<Str>> file) const override {
		const auto r1x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			me = std::open (file ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		const auto r2x = file_size (r1x) ;
		assume (r2x < VAL32_MAX) ;
		const auto r3x = Length (r2x) ;
		RefBuffer<Byte> ret = RefBuffer<Byte> (r3x) ;
		auto rax = r3x ;
		for (auto &&i : range (0 ,FILEPROC_RETRY_TIME::expr)) {
			noop (i) ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (r1x ,(&ret[r3x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= Length (rbx) ;
			if (rax == 0)
				break ;
		}
		assume (rax == 0) ;
		return move (ret) ;
	}

	Val64 file_size (CR<csc_pipe_t> handle) const {
		const auto r1x = Val64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = Val64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void save_file (CR<String<Str>> file ,CR<RefBuffer<Byte>> item) const override {
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r2x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r3x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (file ,r2x ,r3x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		const auto r4x = item.size () ;
		auto rax = r4x ;
		for (auto &&i : range (0 ,FILEPROC_RETRY_TIME::expr)) {
			noop (i) ;
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (r1x ,(&item[r4x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= Length (rbx) ;
			if (rax == 0)
				break ;
		}
		assume (rax == 0) ;
	}

	Ref<RefBuffer<Byte>> load_asset (CR<String<Str>> file) const override {
		const auto r1x = dlopen (NULL ,RTLD_LOCAL) ;
		assume (r1x != NULL) ;
		const auto r2x = String<Str>::make (file ,slice ("input_txt_start")) ;
		const auto r3x = String<Str>::make (file ,slice ("input_txt_end")) ;
		const auto r4x = Flag (dlsym (r1x ,r2x)) ;
		assume (r4x != ZERO) ;
		const auto r5x = Flag (dlsym (r1x ,r3x)) ;
		assume (r5x != ZERO) ;
		const auto r6x = r5x - r4x ;
		assume (r6x >= 0) ;
		return Ref<RefBuffer<Byte>>::make (RefBuffer<Byte>::reference (r4x ,r6x)) ;
	}

	void copy_file (CR<String<Str>> dst ,CR<String<Str>> src) const override {
		const auto r1x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			me = std::open (src ,O_RDONLY) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		const auto r2x = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (dst ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		const auto r5x = file_size (r1x) ;
		assume (r5x < VAL32_MAX) ;
		sendfile (r1x ,r2x ,NULL ,r5x) ;
	}

	void move_file (CR<String<Str>> dst ,CR<String<Str>> src) const override {
		const auto r1x = rename (src ,dst) ;
		noop (r1x) ;
	}

	void link_file (CR<String<Str>> dst ,CR<String<Str>> src) const override {
		if (!Path (src).is_file ())
			return ;
		const auto r1x = symlink (src ,dst) ;
		noop (r1x) ;
	}

	void erase_file (CR<String<Str>> file) const override {
		if (!Path (file).is_file ())
			return ;
		const auto r1x = unlink (file) ;
		noop (r1x) ;
	}

	void build_dire (CR<String<Str>> dire) const override {
		const auto r1x = Path (dire).decouple () ;
		if (r1x.length () == 0)
			return ;
		auto rax = Path (r1x[0]) ;
		const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
		for (auto &&i : r1x.iter ()) {
			if ifdo (TRUE) {
				if (i == 0)
					discard ;
				rax = rax.child (r1x[i]) ;
			}
			const auto r3x = rax.fetch () ;
			mkdir (r3x ,r2x) ;
		}
	}

	void link_dire (CR<String<Str>> dst ,CR<String<Str>> src) const override {
		if (!Path (src).is_dire ())
			return ;
		const auto r1x = symlink (src ,dst) ;
		noop (r1x) ;
	}

	void clear_dire (CR<String<Str>> dire) const override {
		auto rax = Deque<Tuple<Path ,Bool>> () ;
		clear_dire_push (rax ,Path (dire)) ;
		while (TRUE) {
			if (rax.empty ())
				break ;
			Index ix = rax.tail () ;
			auto act = TRUE ;
			if ifdo (act) {
				if (!rax[ix].m2nd)
					discard ;
				rax[ix].m2nd = FALSE ;
				clear_dire_push (rax ,rax[ix].m1st) ;
			}
			if ifdo (act) {
				erase_dire (rax[ix].m1st) ;
				rax.pop () ;
			}
		}
	}

	void clear_dire_push (VR<Deque<Tuple<Path ,Bool>>> queue ,CR<Path> dire) const {
		const auto r1x = dire.list () ;
		for (auto &&i : r1x) {
			auto act = TRUE ;
			if ifdo (act) {
				if (!i.is_file ())
					discard ;
				erase_file (i) ;
			}
			if ifdo (act) {
				if (!i.is_link ())
					discard ;
				erase_dire (i) ;
			}
			if ifdo (act) {
				if (!i.is_dire ())
					discard ;
				queue.add ({i ,TRUE}) ;
			}
		}
	}

	void erase_dire (CR<String<Str>> dire) const override {
		if (!Path (dire).is_dire ())
			return ;
		const auto r1x = unlink (dire) ;
		noop (r1x) ;
		const auto r2x = rmdir (dire) ;
		noop (r2x) ;
	}

	Bool lock_dire (CR<String<Str>> dire) const override {
		const auto r1x = Path (dire).child (slice (".lockdirectory")) ;
		const auto r2x = Process (RuntimeProc::process_uid ()) ;
		if ifdo (TRUE) {
			if (!r1x.is_file ())
				discard ;
			const auto r3x = FileProc::load_file (r1x) ;
			const auto r4x = Process (r3x) ;
			const auto r5x = Process (r4x.process_uid ()) ;
			if (r4x != r5x)
				discard ;
			return r4x == r2x ;
		}
		lock_dire_push (r1x ,r2x.snapshot ()) ;
		return TRUE ;
	}

	void lock_dire_push (CR<Path> file ,CR<RefBuffer<Byte>> snapshot_) const {
		Scope<Mutex> anonymous (self.mMutex) ;
		auto rax = UniqueRef<String<Str>> ([&] (VR<String<Str>> me) {
			me = file ;
			FileProc::save_file (me ,snapshot_) ;
		} ,[&] (VR<String<Str>> me) {
			FileProc::erase_file (me) ;
		}) ;
		const auto r1x = Pin<List<UniqueRef<String<Str>>>> (self.mLockDirectory) ;
		r1x->add (move (rax)) ;
	}
} ;

static const auto mFileProcExternal = External<FileProcHolder ,FileProcLayout> (FileProcImplHolder ()) ;

class StreamFileImplHolder final implement Fat<StreamFileHolder ,StreamFileLayout> {
public:
	void initialize (CR<String<Str>> file) override {
		self.mFile = move (file) ;
		self.mFileSize = 0 ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
		self.mShortRead = FALSE ;
		self.mShortSize = 0 ;
	}

	void set_short_read (CR<Bool> flag) override {
		self.mShortRead = flag ;
	}

	void open_r () override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mReadPipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = file_size (self.mReadPipe) ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	void open_w (CR<Length> size_) override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mWritePipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_WRONLY | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = size_ ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
	}

	void open_a () override {
		assert (!self.mReadPipe.exist ()) ;
		assert (!self.mWritePipe.exist ()) ;
		self.mReadPipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDONLY) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		self.mWritePipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r3x = csc_enum_t (O_CREAT | O_WRONLY) ;
			const auto r4x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r3x ,r4x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = file_size (self.mReadPipe) ;
		self.mRead = 0 ;
		self.mWrite = 0 ;
		if ifdo (TRUE) {
			const auto r5x = Val64 (lseek64 (self.mWritePipe ,0 ,SEEK_END)) ;
			if (r5x <= 0)
				discard ;
			self.mWrite += r5x ;
		}
	}

	Length file_size () const override {
		assume (self.mFileSize < VAL32_MAX) ;
		return Length (self.mFileSize) ;
	}

	Val64 file_size (CR<csc_pipe_t> handle) const {
		const auto r1x = Val64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = Val64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	Length short_size () const override {
		return self.mShortSize ;
	}

	void read (VR<RefBuffer<Byte>> item) override {
		assert (self.mReadPipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::read (self.mReadPipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= Length (rbx) ;
			if (rax == 0)
				discard ;
			assume (self.mShortRead) ;
		}
		self.mShortSize = r1x - rax ;
		self.mRead += self.mShortSize ;
	}

	void write (CR<RefBuffer<Byte>> item) override {
		assert (self.mWritePipe.exist ()) ;
		assert (item.size () < VAL32_MAX) ;
		const auto r1x = item.size () ;
		auto rax = r1x ;
		if ifdo (TRUE) {
			auto rbx = csc_size_t (rax) ;
			rbx = std::write (self.mWritePipe ,(&item[r1x - rax]) ,rbx) ;
			assume (rbx >= 0) ;
			rax -= Length (rbx) ;
			if (rax == 0)
				discard ;
			assume (self.mShortRead) ;
		}
		self.mShortSize = r1x - rax ;
		self.mWrite += self.mShortSize ;
	}

	void flush () override {
		if (!self.mWritePipe.exist ())
			return ;
		fsync (self.mWritePipe) ;
	}
} ;

static const auto mStreamFileExternal = External<StreamFileHolder ,StreamFileLayout> (StreamFileImplHolder ()) ;

class BufferFileImplHolder final implement Fat<BufferFileHolder ,BufferFileLayout> {
private:
	using BUFFERFILE_BLOCK_STEP = ENUM<1024> ;
	using BUFFERFILE_CHUNK_STEP = ENUM<4194304> ;
	using BUFFERFILE_HEADER_STEP = ENUM<65536> ;

public:
	void initialize (CR<String<Str>> file) override {
		self.mFile = move (file) ;
		self.mFileSize = 0 ;
		self.mFileMapFlag = 0 ;
		set_block_step (BUFFERFILE_BLOCK_STEP::expr) ;
		set_cache_size (1) ;
	}

	void set_block_step (CR<Length> step_) override {
		self.mBlockStep = step_ ;
		self.mChunkStep = BUFFERFILE_CHUNK_STEP::expr ;
	}

	void set_cache_size (CR<Length> size_) override {
		assert (size_ > 0) ;
		self.mCacheSet = Set<Val64> (size_) ;
		self.mCacheList = List<BufferFileChunk> (size_) ;
		self.mCacheTimer = 0 ;
	}

	void open_r () override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		self.mPipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,O_RDONLY ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = file_size (self.mPipe) ;
		self.mMapping = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			me = csc_handle_t (self.mFile.ref) ;
		} ,[&] (VR<csc_handle_t> me) {
			noop () ;
		}) ;
		self.mFileMapFlag = csc_enum_t (PROT_READ) ;
		read_header () ;
	}

	void open_w (CR<Length> size_) override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		self.mPipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR | O_TRUNC) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		const auto r3x = self.mChunkStep / self.mBlockStep ;
		const auto r4x = (size_ + r3x - 1) / r3x ;
		self.mFileSize = BUFFERFILE_HEADER_STEP::expr + r4x * self.mChunkStep ;
		self.mMapping = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			const auto r5x = ftruncate64 (self.mPipe ,self.mFileSize) ;
			assume (r5x == 0) ;
			me = csc_handle_t (self.mFile.ref) ;
		} ,[&] (VR<csc_handle_t> me) {
			noop () ;
		}) ;
		self.mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		write_header () ;
	}

	void open_a () override {
		assert (!self.mPipe.exist ()) ;
		assert (!self.mMapping.exist ()) ;
		assume (self.mHeader != NULL) ;
		self.mPipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (O_CREAT | O_RDWR) ;
			const auto r2x = csc_enum_t (S_IRWXU | S_IRWXG | S_IRWXO) ;
			me = std::open (self.mFile ,r1x ,r2x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		self.mFileSize = self.mHeader->mFileSize ;
		self.mMapping = UniqueRef<csc_handle_t> ([&] (VR<csc_handle_t> me) {
			const auto r3x = ftruncate64 (self.mPipe ,self.mFileSize) ;
			assume (r3x == 0) ;
			me = csc_handle_t (self.mFile.ref) ;
		} ,[&] (VR<csc_handle_t> me) {
			noop () ;
		}) ;
		self.mFileMapFlag = csc_enum_t (PROT_READ | PROT_WRITE) ;
		read_header () ;
	}

	void read_header () {
		assert (self.mHeader == NULL) ;
		self.mHeader = Box<BufferFileHeader>::make () ;
		auto rax = ByteReader (borrow_header ()) ;
		rax >> slice ("CSC_BufferFile") ;
		rax >> GAP ;
		rax >> self.mHeader->mFileEndian ;
		assume (self.mHeader->mFileEndian == QUAD_ENDIAN) ;
		rax >> GAP ;
		rax >> self.mHeader->mFileSize ;
		assume (self.mHeader->mFileSize == self.mFileSize) ;
		rax >> GAP ;
		rax >> self.mHeader->mBlockSize ;
		const auto r1x = self.mChunkStep / self.mBlockStep ;
		assume (self.mHeader->mBlockSize == r1x) ;
		rax >> GAP ;
		rax >> self.mHeader->mBlockStep ;
		assume (self.mHeader->mBlockStep == self.mBlockStep) ;
		rax >> GAP ;
		rax >> self.mHeader->mBlockLength ;
		rax >> GAP ;
		rax >> self.mHeader->mChunkSize ;
		assume (self.mHeader->mChunkSize >= 0) ;
		rax >> GAP ;
		rax >> self.mHeader->mChunkStep ;
		assume (self.mHeader->mChunkStep == self.mChunkStep) ;
		rax >> GAP ;
		rax >> self.mHeader->mChunkLength ;
		rax >> GAP ;
	}

	void write_header () {
		if ifdo (TRUE) {
			if (self.mHeader != NULL)
				discard ;
			self.mHeader = Box<BufferFileHeader>::make () ;
			self.mHeader->mFileEndian = QUAD_ENDIAN ;
			self.mHeader->mFileSize = self.mFileSize ;
			self.mHeader->mBlockSize = self.mChunkStep / self.mBlockStep ;
			self.mHeader->mBlockStep = self.mBlockStep ;
			self.mHeader->mBlockLength = 0 ;
			self.mHeader->mChunkSize = (self.mFileSize - BUFFERFILE_HEADER_STEP::expr) / self.mChunkStep ;
			self.mHeader->mChunkStep = self.mChunkStep ;
			self.mHeader->mChunkLength = 0 ;
		}
		auto rax = ByteWriter (borrow_header ()) ;
		rax << slice ("CSC_BufferFile") ;
		rax << GAP ;
		rax << self.mHeader->mFileEndian ;
		rax << GAP ;
		rax << self.mHeader->mFileSize ;
		rax << GAP ;
		rax << self.mHeader->mBlockSize ;
		rax << GAP ;
		rax << self.mHeader->mBlockStep ;
		rax << GAP ;
		rax << self.mHeader->mBlockLength ;
		rax << GAP ;
		rax << self.mHeader->mChunkSize ;
		rax << GAP ;
		rax << self.mHeader->mChunkStep ;
		rax << GAP ;
		rax << self.mHeader->mChunkLength ;
		rax << GAP ;
		flush () ;
	}

	Ref<RefBuffer<Byte>> borrow_header () {
		Index ix = mmap_cache (0 ,BUFFERFILE_HEADER_STEP::expr) ;
		const auto r1x = self.mCacheList[ix].mBlock->m1st ;
		const auto r2x = BUFFERFILE_HEADER_STEP::expr ;
		return Ref<RefBuffer<Byte>>::make (RefBuffer<Byte>::reference (r1x ,r2x)) ;
	}

	Length file_size () const override {
		assume (self.mFileSize < VAL32_MAX) ;
		return Length (self.mFileSize) ;
	}

	Val64 file_size (CR<csc_pipe_t> handle) const {
		const auto r1x = Val64 (lseek64 (handle ,0 ,SEEK_END)) ;
		const auto r2x = Val64 (lseek64 (handle ,0 ,SEEK_SET)) ;
		//@warn: file in '/proc' is zero size
		if (r1x == NONE)
			return 0 ;
		if (r2x == NONE)
			return 0 ;
		assume (r1x >= 0) ;
		assume (r2x == 0) ;
		return r1x ;
	}

	void read (CR<Index> index ,VR<RefBuffer<Byte>> item) override {
		assert (self.mPipe.exist ()) ;
		assert (inline_between (index ,0 ,Length (self.mHeader->mBlockSize))) ;
		assert (item.size () == self.mHeader->mBlockStep) ;
		const auto r1x = index / self.mHeader->mBlockSize ;
		const auto r2x = index % self.mHeader->mBlockSize * self.mHeader->mBlockStep ;
		const auto r3x = BUFFERFILE_HEADER_STEP::expr + r1x * self.mHeader->mChunkStep ;
		Index ix = mmap_cache (r3x ,Length (self.mHeader->mChunkStep)) ;
		const auto r4x = self.mCacheList[ix].mBlock->m1st + Length (r2x) ;
		inline_memcpy (Pointer::from (item.ref) ,Pointer::make (r4x) ,Length (self.mHeader->mBlockStep)) ;
	}

	void write (CR<Index> index ,CR<RefBuffer<Byte>> item) override {
		assert (self.mPipe.exist ()) ;
		assert (inline_between (index ,0 ,Length (self.mHeader->mBlockSize))) ;
		assert (item.size () == self.mHeader->mBlockStep) ;
		const auto r1x = index / self.mHeader->mBlockSize ;
		const auto r2x = index % self.mHeader->mBlockSize * self.mHeader->mBlockStep ;
		const auto r3x = BUFFERFILE_HEADER_STEP::expr + r1x * self.mHeader->mChunkStep ;
		Index ix = mmap_cache (r3x ,Length (self.mHeader->mChunkStep)) ;
		const auto r4x = self.mCacheList[ix].mBlock->m1st + Length (r2x) ;
		inline_memcpy (Pointer::make (r4x) ,Pointer::from (item.ref) ,Length (self.mHeader->mBlockStep)) ;
	}

	Index mmap_cache (CR<Val64> index ,CR<Length> size_) {
		Index ret = self.mCacheSet.map (index) ;
		if ifdo (TRUE) {
			if (ret != NONE)
				discard ;
			update_overflow () ;
			ret = self.mCacheList.insert () ;
			self.mCacheSet.add (index ,ret) ;
			self.mCacheList[ret].mIndex = index ;
			self.mCacheList[ret].mBlock = UniqueRef<Tuple<Flag ,Flag>> ([&] (VR<Tuple<Flag ,Flag>> me) {
				const auto r1x = mmap64 (NULL ,size_ ,self.mFileMapFlag ,MAP_SHARED ,self.mPipe ,index) ;
				assume (r1x != MAP_FAILED) ;
				me.m1st = Flag (r1x) ;
				me.m2nd = me.m1st + size_ ;
			} ,[&] (VR<Tuple<Flag ,Flag>> me) {
				const auto r2x = csc_handle_t (me.m1st) ;
				const auto r3x = me.m2nd - me.m1st ;
				msync (r2x ,r3x ,MS_SYNC) ;
				munmap (r2x ,r3x) ;
			}) ;
		}
		self.mCacheList[ret].mCacheTime = self.mCacheTimer ;
		self.mCacheTimer++ ;
		if ifdo (TRUE) {
			if (self.mCacheTimer < VAL32_MAX)
				discard ;
			for (auto &&i : self.mCacheList.iter ())
				self.mCacheList[i].mCacheTime = 0 ;
			self.mCacheList[ret].mCacheTime = 1 ;
			self.mCacheTimer = 2 ;
		}
		return move (ret) ;
	}

	void update_overflow () {
		if (!self.mCacheList.full ())
			return ;
		const auto r1x = invoke ([&] () {
			Index ret = NONE ;
			auto rax = Val64 () ;
			for (auto &&i : self.mCacheList.iter ()) {
				if (ret != NONE)
					if (rax >= self.mCacheList[i].mCacheTime)
						continue ;
				ret = i ;
				rax = self.mCacheList[i].mCacheTime ;
			}
			return move (ret) ;
		}) ;
		assert (r1x != NONE) ;
		self.mCacheSet.erase (self.mCacheList[r1x].mIndex) ;
		self.mCacheList.remove (r1x) ;
	}

	void flush () override {
		if (!self.mPipe.exist ())
			return ;
		self.mCacheSet.clear () ;
		self.mCacheList.clear () ;
		self.mCacheTimer = 0 ;
		fsync (self.mPipe) ;
	}
} ;

static const auto mBufferFileExternal = External<BufferFileHolder ,BufferFileLayout> (BufferFileImplHolder ()) ;

struct UartFileCOMParams implement TERMIOS_INFO {} ;

struct UartFileCOMStatus {} ;

class UartFileImplHolder final implement Fat<UartFileHolder ,UartFileLayout> {
private:
	void initialize () override {
		self.mCOMParams = Ref<UartFileCOMParams>::make () ;
		self.mCOMStatus = Ref<UartFileCOMStatus>::make () ;
		self.mCOMError = 0 ;
	}

	void set_port_name (CR<String<Str>> name) override {
		self.mPortName = name ;
	}

	void set_port_rate (CR<Length> rate) override {
		self.mPortRate = rate ;
	}

	void set_ring_size (CR<Length> size_) override {
		self.mRingBuffer = RefBuffer<Byte> (size_) ;
		self.mRingRead = 0 ;
	}

	void open () override {
		assert (self.mPortName.length () > 0) ;
		assert (self.mRingBuffer.size () > 0) ;
		self.mPipe = UniqueRef<csc_pipe_t> ([&] (VR<csc_pipe_t> me) {
			const auto r1x = csc_enum_t (O_RDWR | O_NOCTTY | O_SYNC) ;
			me = std::open (self.mPortName ,r1x) ;
			assume (me != NONE) ;
		} ,[&] (VR<csc_pipe_t> me) {
			std::close (me) ;
		}) ;
		const auto r2x = tcgetattr (self.mPipe ,(&self.mCOMParams.ref)) ;
		assume (r2x != 0) ;
		cfsetospeed ((&self.mCOMParams.ref) ,Val32 (self.mPortRate)) ;
		cfsetispeed ((&self.mCOMParams.ref) ,Val32 (self.mPortRate)) ;
		self.mCOMParams->c_cflag = (self.mCOMParams->c_cflag & ~CSIZE) | CS8 ;
		self.mCOMParams->c_iflag &= ~IGNBRK ;
		self.mCOMParams->c_lflag = 0 ;
		self.mCOMParams->c_oflag = 0 ;
		self.mCOMParams->c_cc[VMIN] = 0 ;
		self.mCOMParams->c_cc[VTIME] = 5 ;
		self.mCOMParams->c_iflag &= ~(IXON | IXOFF | IXANY) ;
		self.mCOMParams->c_cflag |= (CLOCAL | CREAD) ;
		self.mCOMParams->c_cflag &= ~(PARENB | PARODD) ;
		self.mCOMParams->c_cflag &= ~CSTOPB ;
		self.mCOMParams->c_cflag &= ~CRTSCTS ;
		const auto r3x = tcsetattr (self.mPipe ,TCSANOW ,(&self.mCOMParams.ref)) ;
		assume (r3x != 0) ;
	}

	void read (VR<RefBuffer<Byte>> buffer ,CR<Index> offset ,CR<Length> size_) override {
		for (auto &&i : range (0 ,size_)) {
			buffer[offset + i] = self.mRingBuffer[self.mRingRead] ;
			self.mRingRead++ ;
			if ifdo (TRUE) {
				if (self.mRingRead < self.mRingBuffer.size ())
					discard ;
				auto rax = self.mRingBuffer.size () ;
				while (TRUE) {
					auto rbx = csc_size_t (rax) ;
					rbx = std::read (self.mPipe ,self.mRingBuffer ,rbx) ;
					assume (rbx >= 0) ;
					rax -= rbx ;
					if (rax == 0)
						break ;
					RuntimeProc::thread_yield () ;
				}
				self.mRingRead = 0 ;
			}
		}
	}
} ;

static const auto mUartFileExternal = External<UartFileHolder ,UartFileLayout> (UartFileImplHolder ()) ;

class ConsoleImplHolder final implement Fat<ConsoleHolder ,ConsoleLayout> {
public:
	void initialize () override {
		self.mMutex = NULL ;
		self.mOption = BitSet (ConsoleOption::ETC) ;
		self.mLogBuffer = String<Str> (STREAMFILE_CHUNK_STEP::expr) ;
		self.mLogWriter = TextWriter (self.mLogBuffer.borrow ()) ;
		self.mDebugMode = inline_debug () ;
		self.mCommand = NULL ;
	}

	void set_option (CR<Just<ConsoleOption>> option) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		auto act = TRUE ;
		if ifdo (act) {
			if (option != ConsoleOption::All)
				discard ;
			self.mOption.clear () ;
		}
		if ifdo (act) {
			self.mOption.add (option) ;
		}
	}

	void log (CR<String<Str>> tag ,CR<Format> msg) {
		self.mLogWriter.reset () ;
		self.mLogWriter << slice ("[") ;
		const auto r1x = CurrentTime () ;
		const auto r2x = r1x.calendar () ;
		self.mLogWriter << WriteAligned (r2x.mHour ,2) ;
		self.mLogWriter << slice (":") ;
		self.mLogWriter << WriteAligned (r2x.mMinute ,2) ;
		self.mLogWriter << slice (":") ;
		self.mLogWriter << WriteAligned (r2x.mSecond ,2) ;
		self.mLogWriter << slice ("][") ;
		self.mLogWriter << tag ;
		self.mLogWriter << slice ("] : ") ;
		self.mLogWriter << msg ;
		self.mLogWriter << GAP ;
		self.mLogWriter << EOS ;
	}

	void print (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoPrint])
			return ;
		self.mLogWriter.reset () ;
		self.mLogWriter << msg ;
		self.mLogWriter << EOS ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("%s")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void fatal (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoFatal])
			return ;
		log (slice ("Fatal") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("\033[1;34m%s\033[0m")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void error (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoError])
			return ;
		log (slice ("Error") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("\033[1;31m%s\033[0m")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void warn (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoWarn])
			return ;
		log (slice ("Warn") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("\033[1;33m%s\033[0m")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void info (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoInfo])
			return ;
		log (slice ("Info") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("\033[1;32m%s\033[0m")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void debug (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoDebug])
			return ;
		log (slice ("Debug") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("\033[1;36m%s\033[0m")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void trace (CR<Format> msg) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mOption[ConsoleOption::NoTrace])
			return ;
		log (slice ("Trace") ,msg) ;
		log_file () ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("\033[1;37m%s\033[0m")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,self.mLogBuffer.ref) ;
		}
	}

	void open (CR<String<Str>> dire) override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mLogFile = Path (dire).child (slice ("console.log")) ;
		self.mOldLogFile = Path (dire).child (slice ("console.old.log")) ;
		FileProc::erase_file (self.mOldLogFile) ;
		FileProc::move_file (self.mOldLogFile ,self.mLogFile) ;
		self.mLogStreamFile = StreamFile (self.mLogFile) ;
		self.mLogStreamFile.open_w (0) ;
		self.mLogWriter.reset () ;
		self.mLogWriter << BOM ;
		self.mLogWriter << EOS ;
		log_file () ;
	}

	void log_file () {
		if ifdo (TRUE) {
			if (self.mLogFile.length () == 0)
				discard ;
			const auto r1x = Flag (self.mLogBuffer.ref) ;
			const auto r2x = (self.mLogWriter.length () - 1) * SIZE_OF<Str>::expr ;
			self.mLogStreamFile.write (RefBuffer<Byte>::reference (r1x ,r2x)) ;
		}
	}

	void show () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if (self.mConsole.exist ())
			return ;
		self.mConsole = UniqueRef<csc_handle_t>::make (stderr) ;
	}

	void hide () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mConsole = UniqueRef<csc_handle_t> () ;
	}

	void pause () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		if ifdo (TRUE) {
			if (!self.mConsole.exist ())
				discard ;
			const auto r1x = String<Str> (slice ("%s\n")) ;
			const auto r2x = String<Str> (slice ("press any key to continue...")) ;
			std::fprintf (HIOF (self.mConsole.ref) ,r1x.ref ,r2x.ref) ;
		}
		const auto r3x = std::getchar () ;
		noop (r3x) ;
	}

	void clear () override {
		Scope<Mutex> anonymous (self.mMutex) ;
		self.mCommand.execute (slice ("clear")) ;
	}
} ;

static const auto mConsoleExternal = External<ConsoleHolder ,ConsoleLayout> (ConsoleImplHolder ()) ;
} ;