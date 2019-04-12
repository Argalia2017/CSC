#pragma once

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"

namespace CSC {
#ifdef __CSC_DEPRECATED__
inline namespace S {
inline import DEF<AutoBuffer<BYTE> (const String<STR> &file) popping> _LOADFILE_ ;

inline import DEF<void (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping> _LOADFILE_ ;

inline import DEF<void (const String<STR> &file ,const PhanBuffer<const BYTE> &data)> _SAVEFILE_ ;

inline import DEF<PhanBuffer<const BYTE> (FLAG resource) popping> _LOADASSETFILE_ ;

inline import DEF<BOOL (const String<STR> &file) popping> _FINDFILE_ ;

inline import DEF<void (const String<STR> &file)> _ERASEFILE_ ;

inline import DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _COPYFILE_ ;

inline import DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _MOVEFILE_ ;

inline import DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _LINKFILE_ ;

inline import DEF<BOOL (const String<STR> &file1 ,const String<STR> &file2) popping> _IDENTICALFILE_ ;

inline import DEF<String<STR> (const String<STR> &file)> _PARSEFILEPATH_ ;

inline import DEF<String<STR> (const String<STR> &file)> _PARSEFILENAME_ ;

inline import DEF<Queue<String<STR>> (const String<STR> &file)> _DECOUPLEPATHNAME_ ;

inline import DEF<String<STR> ()> _WORKINGPATH_ ;

inline import DEF<String<STR> (const String<STR> &path)> _ABSOLUTEPATH_ ;

inline import DEF<String<STR> (const String<STR> &path ,const String<STR> &file)> _RELATIVEPATHNAME_ ;

inline import DEF<String<STR> ()> _MODULEFILEPATH_ ;

inline import DEF<String<STR> ()> _MODULEFILENAME_ ;

inline import DEF<BOOL (const String<STR> &dire) popping> _FINDDIRECTORY_ ;

inline import DEF<void (const String<STR> &dire)> _ERASEDIRECTORY_ ;

inline import DEF<void (const String<STR> &dire)> _CLEARDIRECTORY_ ;

inline import DEF<void (const String<STR> &dire)> _BUILDDIRECTORY_ ;

inline import DEF<void (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping> _ENUMDIRECTORY_ ;
} ;
#endif

class StreamLoader {
private:
	class Implement ;

private:
	AnyRef<void> mThis ;

public:
	StreamLoader () = delete ;

	explicit StreamLoader (const String<STR> &file) ;

	void read (const PhanBuffer<BYTE> &data) popping ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data) popping {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) ;

	template <class _ARG>
	void write (const Buffer<BYTE ,_ARG> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	void flush () ;
} ;

class BufferLoader {
private:
	class Implement ;

private:
	AnyRef<void> mThis ;

public:
	BufferLoader () = delete ;

	explicit BufferLoader (const String<STR> &file) ;

	explicit BufferLoader (const String<STR> &file ,LENGTH file_len) ;

	explicit BufferLoader (const String<STR> &file ,BOOL cache) ;

	explicit BufferLoader (const String<STR> &file ,LENGTH file_len ,BOOL cache) ;

	PhanBuffer<BYTE> watch () ;

	PhanBuffer<const BYTE> watch () const ;

	void flush () ;
} ;

class FileSystemService final :private Interface {
private:
	class Implement ;

	export struct Abstract :public Interface {
		virtual AutoBuffer<BYTE> load_file (const String<STR> &file) popping = 0 ;
		virtual void load_file (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping = 0 ;
		virtual void save_file (const String<STR> &file ,const PhanBuffer<const BYTE> &data) = 0 ;
		virtual PhanBuffer<const BYTE> load_asset_file (FLAG resource) popping = 0 ;
		virtual BOOL find_file (const String<STR> &file) popping = 0 ;
		virtual void erase_file (const String<STR> &file) = 0 ;
		virtual void copy_file (const String<STR> &dst_file ,const String<STR> &src_file) = 0 ;
		virtual void move_file (const String<STR> &dst_file ,const String<STR> &src_file) = 0 ;
		virtual void link_file (const String<STR> &dst_file ,const String<STR> &src_file) = 0 ;
		virtual BOOL identical_file (const String<STR> &file1 ,const String<STR> &file2) popping = 0 ;
		virtual String<STR> parse_file_path (const String<STR> &file) const = 0 ;
		virtual String<STR> parse_file_name (const String<STR> &file) const = 0 ;
		virtual Queue<String<STR>> decouple_path_name (const String<STR> &file) const = 0 ;
		virtual String<STR> working_path () const = 0 ;
		virtual String<STR> absolute_path (const String<STR> &path) const = 0 ;
		virtual String<STR> module_file_path () const = 0 ;
		virtual String<STR> module_file_name () const = 0 ;
		virtual BOOL find_directory (const String<STR> &dire) popping = 0 ;
		virtual void erase_directory (const String<STR> &dire) = 0 ;
		virtual void clear_directory (const String<STR> &dire) = 0 ;
		virtual void build_directory (const String<STR> &dire) = 0 ;
		virtual void enum_directory (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping = 0 ;
	} ;

private:
	friend Singleton<FileSystemService> ;
	Monostate<std::recursive_mutex> mMutex ;
	HolderRef<Abstract> mThis ;

public:
	AutoBuffer<BYTE> load_file (const String<STR> &file) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->load_file (file) ;
	}

	void load_file (const String<STR> &file ,const PhanBuffer<BYTE> &data) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->load_file (file ,data) ;
	}

	template <class _ARG>
	void load_file (const String<STR> &file ,Buffer<BYTE ,_ARG> &data) {
		load_file (file ,PhanBuffer<BYTE>::make (data)) ;
	}

	void save_file (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->save_file (file ,data) ;
	}

	template <class _ARG>
	void save_file (const String<STR> &file ,const Buffer<BYTE ,_ARG> &data) {
		save_file (file ,PhanBuffer<const BYTE>::make (data)) ;
	}

	PhanBuffer<const BYTE> load_asset_file (FLAG resource) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->load_asset_file (resource) ;
	}

	BOOL find_file (const String<STR> &file) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->find_file (file) ;
	}

	void erase_file (const String<STR> &file) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->erase_file (file) ;
	}

	void copy_file (const String<STR> &dst_file ,const String<STR> &src_file) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->copy_file (dst_file ,src_file) ;
	}

	void move_file (const String<STR> &dst_file ,const String<STR> &src_file) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->move_file (dst_file ,src_file) ;
	}

	void link_file (const String<STR> &dst_file ,const String<STR> &src_file) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->link_file (dst_file ,src_file) ;
	}

	BOOL identical_file (const String<STR> &file1 ,const String<STR> &file2) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->identical_file (file1 ,file2) ;
	}

	String<STR> parse_file_path (const String<STR> &file) const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->parse_file_path (file) ;
	}

	String<STR> parse_file_name (const String<STR> &file) const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->parse_file_name (file) ;
	}

	Queue<String<STR>> decouple_path_name (const String<STR> &file) const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->decouple_path_name (file) ;
	}

	String<STR> working_path () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->working_path () ;
	}

	String<STR> absolute_path (const String<STR> &path) const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->absolute_path (path) ;
	}

	String<STR> module_file_path () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->module_file_path () ;
	}

	String<STR> module_file_name () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->module_file_name () ;
	}
	
	BOOL find_directory (const String<STR> &dire) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->find_directory (dire) ;
	}

	void erase_directory (const String<STR> &dire) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->erase_directory (dire) ;
	}

	void clear_directory (const String<STR> &dire) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->clear_directory (dire) ;
	}

	void build_directory (const String<STR> &dire) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->build_directory (dire) ;
	}

	void enum_directory (const String<STR> &dire ,const Function<void (const String<STR> &)> &file_proc ,const Function<void (const String<STR> &)> &dire_proc) popping {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->enum_directory (dire ,file_proc ,dire_proc) ;
	}

private:
	FileSystemService () ;
} ;
} ;