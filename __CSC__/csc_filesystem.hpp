#pragma once

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"

namespace CSC {
using DEFAULT_FILEPATH_SIZE = ARGC<1023> ;
using DEFAULT_DIRECTORY_SIZE = ARGC<65536> ;

class FileSystemProc
	:private Wrapped<void> {
public:
	imports AutoBuffer<BYTE> load_file (const String<STR> &file) side_effects ;

	imports void load_file (const String<STR> &file ,const PhanBuffer<BYTE> &data) ;

	imports void save_file (const String<STR> &file ,const PhanBuffer<const BYTE> &data) ;

	imports PhanBuffer<const BYTE> load_assert_file (const FLAG &resource) side_effects ;

	imports BOOL find_file (const String<STR> &file) side_effects ;

	imports void erase_file (const String<STR> &file) ;

	imports void copy_file (const String<STR> &dst_file ,const String<STR> &src_file) ;

	imports void move_file (const String<STR> &dst_file ,const String<STR> &src_file) ;

	imports void link_file (const String<STR> &dst_file ,const String<STR> &src_file) ;

	imports BOOL identical_file (const String<STR> &file1 ,const String<STR> &file2) side_effects ;

	imports String<STR> parse_path_name (const String<STR> &file) ;

	imports String<STR> parse_file_name (const String<STR> &file) ;

	imports Deque<String<STR>> decouple_path_name (const String<STR> &file) ;

	imports String<STR> working_path () ;

	imports String<STR> absolute_path (const String<STR> &path) ;

	imports const String<STR> &module_file_path () side_effects ;

	imports const String<STR> &module_file_name () side_effects ;

	imports BOOL find_directory (const String<STR> &dire) side_effects ;

	imports BOOL lock_directory (const String<STR> &dire) side_effects ;

	imports void build_directory (const String<STR> &dire) ;

	imports void erase_directory (const String<STR> &dire) ;

	imports void enum_directory (const String<STR> &dire ,Deque<String<STR>> &file_list ,Deque<String<STR>> &dire_list) ;

	imports void clear_directory (const String<STR> &dire) ;
} ;

class StreamLoader {
private:
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	implicit StreamLoader () = delete ;

	explicit StreamLoader (const String<STR> &file) ;

	void read (const PhanBuffer<BYTE> &data) ;

	template <class _ARG1>
	void read (Buffer<BYTE ,_ARG1> &data) {
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
	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	StrongRef<Implement> mThis ;

public:
	implicit BufferLoader () = delete ;

	explicit BufferLoader (const String<STR> &file) ;

	explicit BufferLoader (const String<STR> &file ,const LENGTH &file_len) ;

	explicit BufferLoader (const String<STR> &file ,const BOOL &cache) ;

	explicit BufferLoader (const String<STR> &file ,const LENGTH &file_len ,const BOOL &cache) ;

	PhanBuffer<BYTE> watch () leftvalue ;

	PhanBuffer<const BYTE> watch () const leftvalue ;

	void flush () ;
} ;

class FileSystemService
	:private Proxy {
private:
	class Abstract
		:public Interface {
	public:
		virtual void startup () = 0 ;
		virtual void shutdown () = 0 ;
	} ;

	struct Private {
		class Implement ;
	} ;

	using Implement = typename Private::Implement ;

private:
	friend Singleton<FileSystemService> ;
	Monostate<RecursiveMutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	void startup () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		mThis->startup () ;
	}

	void shutdown () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		mThis->shutdown () ;
	}

private:
	FileSystemService () ;
} ;
} ;