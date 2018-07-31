#pragma once

#ifndef __CSC_FILESYSTEM__
#error "∑(っ°Д° ;)っ : required csc_filesystem.hpp"
#endif

#ifndef __CSC_STRICTHPPINL__
#include <cstdlib>
#include <cstdio>
#endif

namespace CSC {
inline namespace FILESYSTEM {
inline export AutoBuffer<BYTE> _LOADFILE_ (const String<STR> &file) {
	const auto r1x = UniqueRef<PTR<FILE>> ([&] (PTR<FILE> &me) {
		me = NULL ;
		me = std::fopen (_BUILDSTRS_<STRA> (file) ,_PCSTRA_ ("rb")) ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (PTR<FILE> &me) {
		if (me == NULL)
			return ;
		std::fclose (me) ;
		me = NULL ;
	}) ;
	std::fseek (r1x ,0 ,SEEK_END) ;
	const auto r2x = LENGTH (std::ftell (r1x)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= VAR32_MAX) ;
	std::fseek (r1x ,0 ,SEEK_SET) ;
	AutoBuffer<BYTE> ret = AutoBuffer<BYTE> (r2x) ;
	const auto r3x = LENGTH (std::fread (ret ,1 ,r2x ,r1x)) ;
	_DYNAMIC_ASSERT_ (r3x == r2x) ;
	return std::move (ret) ;
}

inline export void _LOADFILE_ (const String<STR> &file ,const PhanBuffer<BYTE> &data) {
	const auto r1x = UniqueRef<PTR<FILE>> ([&] (PTR<FILE> &me) {
		me = NULL ;
		me = std::fopen (_BUILDSTRS_<STRA> (file) ,_PCSTRA_ ("rb")) ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (PTR<FILE> &me) {
		if (me == NULL)
			return ;
		std::fclose (me) ;
		me = NULL ;
	}) ;
	std::fseek (r1x ,0 ,SEEK_END) ;
	const auto r2x = LENGTH (std::ftell (r1x)) ;
	_DYNAMIC_ASSERT_ (r2x > 0 && r2x <= data.size ()) ;
	std::fseek (r1x ,0 ,SEEK_SET) ;
	const auto r3x = LENGTH (std::fread (data ,1 ,r2x ,r1x)) ;
	_DYNAMIC_ASSERT_ (r3x == r2x) ;
}

inline export void _SAVEFILE_ (const String<STR> &file ,const PhanBuffer<const BYTE> &data) {
	_DEBUG_ASSERT_ (data.size () >= 0 && data.size () <= VAR32_MAX) ;
	const auto r1x = UniqueRef<PTR<FILE>> ([&] (PTR<FILE> &me) {
		me = NULL ;
		me = std::fopen (_BUILDSTRS_<STRA> (file) ,_PCSTRA_ ("wb")) ;
		_DYNAMIC_ASSERT_ (me != NULL) ;
	} ,[] (PTR<FILE> &me) {
		if (me == NULL)
			return ;
		std::fclose (me) ;
		me = NULL ;
	}) ;
	const auto r2x = VAR (std::fwrite (data ,1 ,data.size () ,r1x)) ;
	_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
}

inline export BOOL _EXISTFILE_ (const String<STR> &file) {
	const auto r1x = UniqueRef<PTR<FILE>> ([&] (PTR<FILE> &me) {
		me = NULL ;
		me = std::fopen (_BUILDSTRS_<STRA> (file) ,_PCSTRA_ ("r")) ;
	} ,[] (PTR<FILE> &me) {
		if (me == NULL)
			return ;
		std::fclose (me) ;
		me = NULL ;
	}) ;
	return r1x != NULL ;
}

inline export void _ERASEFILE_ (const String<STR> &file) {
	std::remove (_BUILDSTRS_<STRA> (file)) ;
}

inline export void _COPYFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	std::system (_BUILDARGSS_<STRA> (_PCSTRA_ ("cp -f ") ,_BUILDSTRS_<STRA> (src_file) ,_PCSTRA_ (" ") ,_BUILDSTRS_<STRA> (dst_file))) ;
}

inline export void _MOVEFILE_ (const String<STR> &dst_file ,const String<STR> &src_file) {
	std::rename (_BUILDSTRS_<STRA> (src_file) ,_BUILDSTRS_<STRA> (dst_file)) ;
}

inline export BOOL _EXISTDIRECTORY_ (const String<STR> &file) {
	//@warn:unimplemented
	//_DEBUG_ASSERT_ (FALSE) ;
	return FALSE ;
}

inline export void _ENUMPATH_ (const String<STR> &path ,const Function<void (String<STR> &&)> &file_proc ,const Function<void (String<STR> &&)> &dire_proc) {
	//@warn:unimplemented
	//_DEBUG_ASSERT_ (FALSE) ;
}

inline export void _ERASEDIRECTORY_ (const String<STR> &file) {
	std::system (_BUILDARGSS_<STRA> (_PCSTRA_ ("rm -rf ") ,_BUILDSTRS_<STRA> (file)).self) ;
}

inline export void _CLEARDIRECTORY_ (const String<STR> &file) {
	std::system (_BUILDARGSS_<STRA> (_PCSTRA_ ("rm -rf ") ,_BUILDSTRS_<STRA> (file) ,_PCSTRA_ ("/*")).self) ;
}

inline export void _ENSUREPATH_ (const String<STR> &path) {
	std::system (_BUILDARGSS_<STRA> (_PCSTRA_ ("mkdir -p ") ,_BUILDSTRS_<STRA> (path)).self) ;
}

inline export PhanBuffer<const BYTE> _LOADASSET_ (FLAG resource ,const String<STR> &type) {
	_DYNAMIC_ASSERT_ (FALSE) ;
	return PhanBuffer<const BYTE> () ;
}

inline export String<STR> _WORKINGPATH_ () {
	//@warn:unimplemented
	//_DEBUG_ASSERT_ (FALSE) ;
	return String<STR> () ;
}

inline export String<STR> _EXECUTINGPATH_ () {
	//@warn:unimplemented
	//_DEBUG_ASSERT_ (FALSE) ;
	return String<STR> () ;
}
} ;

class StreamLoader::Engine :public Interface {
private:
	UniqueRef<PTR<FILE>> mFile ;

public:
	Engine () = delete ;

	explicit Engine (const String<STR> &file) {
		mFile = UniqueRef<PTR<FILE>> ([&] (PTR<FILE> &me) {
			me = NULL ;
			me = std::fopen (_BUILDSTRS_<STRA> (file) ,_PCSTRA_ ("wb+")) ;
			_DYNAMIC_ASSERT_ (me != NULL) ;
		} ,[] (PTR<FILE> &me) {
			if (me == NULL)
				return ;
			std::fclose (me) ;
			me = NULL ;
		}) ;
	}

	BOOL exist () const {
		return mFile.exist () ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (data.size () > 0 && data.size () <= VAR32_MAX) ;
		const auto r1x = VAR (std::fread (data.self ,1 ,data.size () ,mFile)) ;
		_DYNAMIC_ASSERT_ (r1x >= 0 && r1x <= VAR32_MAX) ;
		_MEMFILL_ (&data[r1x] ,data.size () - r1x ,BYTE (0X00)) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (data.size () >= 0 && data.size () <= VAR32_MAX) ;
		const auto r1x = VAR (std::fwrite (data.self ,1 ,data.size () ,mFile)) ;
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void flush () {
		_DEBUG_ASSERT_ (exist ()) ;
		std::fflush (mFile) ;
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

#ifdef __CSC_DEPRECATED__
class BufferLoader::Engine :public Interface {
public:
	Engine () = delete ;

	explicit Engine (const String<STR> &file) {}

	explicit Engine (const String<STR> &file ,LENGTH len) {
		_DYNAMIC_ASSERT_ (len > 0 && len <= VAR32_MAX) ;
	}

	explicit Engine (const String<STR> &file ,const ARGV<ARGC<1>> &) {}

	explicit Engine (const String<STR> &file ,LENGTH len ,const ARGV<ARGC<1>> &) {
		_DYNAMIC_ASSERT_ (len > 0 && len <= VAR32_MAX) ;
	}

	BOOL exist () const {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return FALSE ;
	}

	PhanBuffer<BYTE> watch () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return PhanBuffer<BYTE> () ;
	}

	PhanBuffer<const BYTE> watch () const {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
		return PhanBuffer<const BYTE> () ;
	}

	void flush () {
		//@warn:unimplemented
		_DEBUG_ASSERT_ (FALSE) ;
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
#endif
} ;