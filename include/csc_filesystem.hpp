#pragma once

#ifndef __CSC_FILESYSTEM__
#define __CSC_FILESYSTEM__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_thread.hpp"

namespace CSC {
inline namespace FILESYSTEM {
inline import DEF<AutoBuffer<BYTE> (const String<STR> &file)> _LOADFILE_ ;

inline import DEF<void (const String<STR> &file ,const PhanBuffer<BYTE> &data)> _LOADFILE_ ;

template <class _ARG>
inline void _LOADFILE_ (const String<STR> &file ,Buffer<BYTE ,_ARG> &data) {
	_LOADFILE_ (file ,PhanBuffer<BYTE>::make (data)) ;
}

inline import DEF<void (const String<STR> &file ,const PhanBuffer<const BYTE> &data)> _SAVEFILE_ ;

template <class _ARG>
inline void _SAVEFILE_ (const String<STR> &file ,const Buffer<BYTE ,_ARG> &data) {
	_SAVEFILE_ (file ,PhanBuffer<const BYTE>::make (data)) ;
}

inline import DEF<BOOL (const String<STR> &file)> _EXISTFILE_ ;

inline import DEF<void (const String<STR> &file)> _ERASEFILE_ ;

inline import DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _COPYFILE_ ;

inline import DEF<void (const String<STR> &dst_file ,const String<STR> &src_file)> _MOVEFILE_ ;

inline String<STR> _PARSEFILENAME_ (const String<STR> &file) {
	const auto r1x = file.length () ;
	INDEX ix = _MAX_ (_MEMRCHR_ (file.self ,r1x ,STR ('\\')) ,_MEMRCHR_ (file.self ,r1x ,STR ('/'))) + 1 ;
	String<STR> ret = String<STR> (r1x - ix) ;
	_MEMCOPY_ (ret.raw ().self ,&file[ix] ,ret.size ()) ;
	return std::move (ret) ;
}

inline String<STR> _PARSEFILEPATH_ (const String<STR> &file) {
	const auto r1x = file.length () ;
	INDEX ix = _MAX_ (_MEMRCHR_ (file.self ,r1x ,STR ('\\')) ,_MEMRCHR_ (file.self ,r1x ,STR ('/'))) + 1 ;
	String<STR> ret = String<STR> (ix) ;
	_MEMCOPY_ (ret.raw ().self ,&file[0] ,ret.size ()) ;
	return std::move (ret) ;
}

inline import DEF<BOOL (const String<STR> &file)> _EXISTDIRECTORY_ ;

inline import DEF<void (const String<STR> &path ,const Function<void (String<STR> &&)> &file_proc ,const Function<void (String<STR> &&)> &dire_proc)> _ENUMPATH_ ;

inline void _ENUMPATHALL_ (const String<STR> &path ,const Function<void (String<STR> &&)> &file_proc ,const Function<void (String<STR> &&)> &dire_proc) {
	auto rax = Stack<String<STR>> () ;
	rax.add (path) ;
	const auto r1x = Function<void (String<STR> &&)> ([&] (String<STR> &&file) {
		file_proc (std::move (file)) ;
	}) ;
	const auto r2x = Function<void (String<STR> &&)> ([&] (String<STR> &&file) {
		rax.add (file + _PCSTR_ ("\\")) ;
		dire_proc (std::move (file)) ;
	}) ;
	while (!rax.empty ())
		_ENUMPATH_ (rax.poll () ,r1x ,r2x) ;
}

inline import DEF<void (const String<STR> &file)> _ERASEDIRECTORY_ ;

inline import DEF<void (const String<STR> &file)> _CLEARDIRECTORY_ ;

inline import DEF<void (const String<STR> &path)> _ENSUREPATH_ ;

inline import DEF<PhanBuffer<const BYTE> (FLAG resource ,const String<STR> &type)> _LOADASSET_ ;

inline import DEF<String<STR> ()> _WORKINGPATH_ ;

inline import DEF<String<STR> ()> _EXECUTINGPATH_ ;
} ;

class StreamLoader {
private:
	class Engine ;

private:
	AnyRef<void> mHolder ;

public:
	StreamLoader () = default ;

	explicit StreamLoader (const String<STR> &file) ;

	BOOL exist () const ;

	void read (const PhanBuffer<BYTE> &data) ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data) {
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
	class Engine ;

private:
	AnyRef<void> mHolder ;

public:
	BufferLoader () = default ;

	explicit BufferLoader (const String<STR> &file) ;

	explicit BufferLoader (const String<STR> &file ,LENGTH len) ;

	explicit BufferLoader (const String<STR> &file ,const ARGV<ARGC<1>> &) ;

	explicit BufferLoader (const String<STR> &file ,LENGTH len ,const ARGV<ARGC<1>> &) ;

	BOOL exist () const ;

	PhanBuffer<BYTE> watch () ;

	PhanBuffer<const BYTE> watch () const ;

	void flush () ;
} ;
} ;