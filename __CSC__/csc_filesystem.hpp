#pragma once

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"

namespace CSC {
inline namespace FILESYSTEM {
inline imports DEF<AutoBuffer<BYTE> (const String<STR> &file) popping> _LOADFILE_ ;

inline imports DEF<void (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping> _LOADFILE_ ;

inline imports DEF<void (const String<STR> &file ,const PhanBuffer<const BYTE> &data)> _SAVEFILE_ ;

inline imports DEF<PhanBuffer<const BYTE> (FLAG resource) popping> _LOADASSETFILE_ ;

inline imports DEF<BOOL (const String<STR> &file) popping> _FINDFILE_ ;

inline imports DEF<void (const String<STR> &file)> _ERASEFILE_ ;

inline imports DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _COPYFILE_ ;

inline imports DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _MOVEFILE_ ;

inline imports DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _LINKFILE_ ;

inline imports DEF<BOOL (const String<STR> &file1 ,const String<STR> &file2) popping> _IDENTICALFILE_ ;

inline imports DEF<String<STR> (const String<STR> &file)> _PARSEDIRENAME_ ;

inline imports DEF<String<STR> (const String<STR> &file)> _PARSEFILENAME_ ;

inline imports DEF<Deque<String<STR>> (const String<STR> &file)> _DECOUPLEPATHNAME_ ;

inline imports DEF<String<STR> ()> _WORKINGPATH_ ;

inline imports DEF<String<STR> (const String<STR> &path)> _ABSOLUTEPATH_ ;

inline imports DEF<const String<STR> & () popping> _MODULEFILEPATH_ ;

inline imports DEF<const String<STR> & () popping> _MODULEFILENAME_ ;

inline imports DEF<BOOL (const String<STR> &dire) popping> _FINDDIRECTORY_ ;

inline imports DEF<BOOL (const String<STR> &dire) popping> _LOCKDIRECTORY_ ;

inline imports DEF<void (const String<STR> &dire)> _BUILDDIRECTORY_ ;

inline imports DEF<void (const String<STR> &dire)> _ERASEDIRECTORY_ ;

inline imports DEF<void (const String<STR> &dire ,Deque<String<STR>> &file_list ,Deque<String<STR>> &dire_list) popping> _ENUMDIRECTORY_ ;

inline imports DEF<void (const String<STR> &dire)> _CLEARDIRECTORY_ ;
} ;

class StreamLoader {
private:
	class Implement ;
	AnyRef<void> mThis ;

public:
	StreamLoader () = delete ;

	explicit StreamLoader (const String<STR> &file) ;

	void read (const PhanBuffer<BYTE> &data) popping ;

	template <class _ARG1>
	void read (Buffer<BYTE ,_ARG1> &data) popping {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) ;

	template <class _ARG1>
	void write (const Buffer<BYTE ,_ARG1> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	void flush () ;
} ;

class BufferLoader {
private:
	class Implement ;
	AnyRef<void> mThis ;

public:
	BufferLoader () = delete ;

	explicit BufferLoader (const String<STR> &file) ;

	explicit BufferLoader (const String<STR> &file ,LENGTH file_len) ;

	explicit BufferLoader (const String<STR> &file ,BOOL cache) ;

	explicit BufferLoader (const String<STR> &file ,LENGTH file_len ,BOOL cache) ;

	PhanBuffer<BYTE> watch () & ;

	PhanBuffer<const BYTE> watch () const & ;

	PhanBuffer<BYTE> watch () && = delete ;

	void flush () ;
} ;

class FileSystemService final :private Interface {
private:
	exports struct Abstract :public Interface {
		virtual void startup () = 0 ;
		virtual void shutdown () = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<FileSystemService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	void startup () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->startup () ;
	}

	void shutdown () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->shutdown () ;
	}

private:
	FileSystemService () ;
} ;
} ;