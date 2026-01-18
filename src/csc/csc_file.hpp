#pragma once

#ifndef __CSC_FILE__
#define __CSC_FILE__
#endif

#include "csc.hpp"
#include "csc_type.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_math.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"

namespace CSC {
struct PathTree ;

struct PathLayout {
	Ref<PathTree> mThis ;
} ;

struct PathHolder implement Interface {
	imports VFat<PathHolder> hold (VR<PathLayout> that) ;
	imports CFat<PathHolder> hold (CR<PathLayout> that) ;

	virtual void initialize (RR<String<Str>> pathname) = 0 ;
	virtual void initialize (CR<Deque<String<Str>>> pathname) = 0 ;
	virtual void initialize (CR<PathLayout> that) = 0 ;
	virtual String<Str> fetch () const = 0 ;
	virtual PathLayout child (CR<Slice> name) const = 0 ;
	virtual PathLayout child (CR<Format> name) const = 0 ;
	virtual PathLayout child (CR<String<Str>> name) const = 0 ;
	virtual Array<PathLayout> list () const = 0 ;
	virtual Array<PathLayout> list (CR<Length> size_) const = 0 ;
	virtual Bool equal (CR<PathLayout> that) const = 0 ;
	virtual Bool is_file () const = 0 ;
	virtual Bool is_dire () const = 0 ;
	virtual Bool is_link () const = 0 ;
	virtual PathLayout symbolic () const = 0 ;
	virtual PathLayout absolute () const = 0 ;
	virtual Deque<String<Str>> decouple () const = 0 ;
	virtual String<Str> path () const = 0 ;
	virtual String<Str> name () const = 0 ;
	virtual String<Str> stem () const = 0 ;
	virtual String<Str> extension () const = 0 ;
} ;

class Path implement PathLayout {
protected:
	using PathLayout::mThis ;

public:
	implicit Path () = default ;

	explicit Path (CR<String<Str>> pathname) {
		PathHolder::hold (thiz)->initialize (move (pathname)) ;
	}

	explicit Path (RR<String<Str>> pathname) {
		PathHolder::hold (thiz)->initialize (move (pathname)) ;
	}

	explicit Path (CR<Deque<String<Str>>> pathname) {
		PathHolder::hold (thiz)->initialize (pathname) ;
	}

	implicit Path (CR<Path> that) {
		PathHolder::hold (thiz)->initialize (that) ;
	}

	forceinline VR<Path> operator= (CR<Path> that) {
		return assign (thiz ,that) ;
	}

	implicit Path (RR<Path> that) = default ;

	forceinline VR<Path> operator= (RR<Path> that) = default ;

	String<Str> fetch () const {
		return PathHolder::hold (thiz)->fetch () ;
	}

	forceinline operator String<Str> () const {
		return fetch () ;
	}

	Path child (CR<Slice> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CR<Format> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path child (CR<String<Str>> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Array<Path> list () const {
		ArrayLayout ret = PathHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<Path>>::expr] (Pointer::from (ret))) ;
	}

	Array<Path> list (CR<Length> size_) const {
		ArrayLayout ret = PathHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<Path>>::expr] (Pointer::from (ret))) ;
	}

	Bool equal (CR<Path> that) const {
		return PathHolder::hold (thiz)->equal (that) ;
	}

	forceinline Bool operator== (CR<Path> that) const {
		return equal (that) ;
	}

	forceinline Bool operator!= (CR<Path> that) const {
		return (!equal (that)) ;
	}

	Bool is_file () const {
		return PathHolder::hold (thiz)->is_file () ;
	}

	Bool is_dire () const {
		return PathHolder::hold (thiz)->is_dire () ;
	}

	Bool is_link () const {
		return PathHolder::hold (thiz)->is_link () ;
	}

	Path symbolic () const {
		PathLayout ret = PathHolder::hold (thiz)->symbolic () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Path absolute () const {
		PathLayout ret = PathHolder::hold (thiz)->absolute () ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Deque<String<Str>> decouple () const {
		return PathHolder::hold (thiz)->decouple () ;
	}

	String<Str> path () const {
		return PathHolder::hold (thiz)->path () ;
	}

	String<Str> name () const {
		return PathHolder::hold (thiz)->name () ;
	}

	String<Str> stem () const {
		return PathHolder::hold (thiz)->stem () ;
	}

	String<Str> extension () const {
		return PathHolder::hold (thiz)->extension () ;
	}
} ;

struct FileProcLayout ;

struct FileProcHolder implement Interface {
	imports CR<Like<UniqueRef<FileProcLayout>>> expr_m () ;
	imports VFat<FileProcHolder> hold (VR<FileProcLayout> that) ;
	imports CFat<FileProcHolder> hold (CR<FileProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual RefBuffer<Byte> load_file (CR<String<Str>> file) const = 0 ;
	virtual void save_file (CR<String<Str>> file ,CR<RefBuffer<Byte>> item) const = 0 ;
	virtual Ref<RefBuffer<Byte>> load_asset (CR<String<Str>> file) const = 0 ;
	virtual void copy_file (CR<String<Str>> dst ,CR<String<Str>> src) const = 0 ;
	virtual void move_file (CR<String<Str>> dst ,CR<String<Str>> src) const = 0 ;
	virtual void link_file (CR<String<Str>> dst ,CR<String<Str>> src) const = 0 ;
	virtual void erase_file (CR<String<Str>> file) const = 0 ;
	virtual void build_dire (CR<String<Str>> dire) const = 0 ;
	virtual void link_dire (CR<String<Str>> dst ,CR<String<Str>> src) const = 0 ;
	virtual void clear_dire (CR<String<Str>> dire) const = 0 ;
	virtual void erase_dire (CR<String<Str>> dire) const = 0 ;
	virtual Bool lock_dire (CR<String<Str>> dire) const = 0 ;
} ;

class FileProc implement Like<UniqueRef<FileProcLayout>> {
public:
	static CR<FileProc> expr_m () {
		return keep[TYPE<FileProc>::expr] (FileProcHolder::expr) ;
	}

	static RefBuffer<Byte> load_file (CR<String<Str>> file) {
		return FileProcHolder::hold (expr)->load_file (file) ;
	}

	static void save_file (CR<String<Str>> file ,CR<RefBuffer<Byte>> item) {
		return FileProcHolder::hold (expr)->save_file (file ,item) ;
	}

	static Ref<RefBuffer<Byte>> load_asset (CR<String<Str>> file) {
		return FileProcHolder::hold (expr)->load_asset (file) ;
	}

	static void copy_file (CR<String<Str>> dst ,CR<String<Str>> src) {
		return FileProcHolder::hold (expr)->copy_file (dst ,src) ;
	}

	static void move_file (CR<String<Str>> dst ,CR<String<Str>> src) {
		return FileProcHolder::hold (expr)->move_file (dst ,src) ;
	}

	static void link_file (CR<String<Str>> dst ,CR<String<Str>> src) {
		return FileProcHolder::hold (expr)->link_file (dst ,src) ;
	}

	static void erase_file (CR<String<Str>> file) {
		return FileProcHolder::hold (expr)->erase_file (file) ;
	}

	static void build_dire (CR<String<Str>> dire) {
		return FileProcHolder::hold (expr)->build_dire (dire) ;
	}

	static void link_dire (CR<String<Str>> dst ,CR<String<Str>> src) {
		return FileProcHolder::hold (expr)->link_dire (dst ,src) ;
	}

	static void clear_dire (CR<String<Str>> dire) {
		return FileProcHolder::hold (expr)->clear_dire (dire) ;
	}

	static void erase_dire (CR<String<Str>> dire) {
		return FileProcHolder::hold (expr)->erase_dire (dire) ;
	}

	static Bool lock_dire (CR<String<Str>> dire) {
		return FileProcHolder::hold (expr)->lock_dire (dire) ;
	}
} ;

struct StreamFileLayout ;

struct StreamFileHolder implement Interface {
	imports AutoRef<StreamFileLayout> create () ;
	imports VFat<StreamFileHolder> hold (VR<StreamFileLayout> that) ;
	imports CFat<StreamFileHolder> hold (CR<StreamFileLayout> that) ;

	virtual void initialize (CR<String<Str>> file) = 0 ;
	virtual void set_short_read (CR<Bool> flag) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CR<Length> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual Length file_size () const = 0 ;
	virtual Length short_size () const = 0 ;
	virtual void read (VR<RefBuffer<Byte>> item) = 0 ;
	virtual void write (CR<RefBuffer<Byte>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

using STREAMFILE_CHUNK_STEP = ENUM<65536> ;

class StreamFile implement Like<AutoRef<StreamFileLayout>> {
public:
	implicit StreamFile () = default ;

	explicit StreamFile (CR<String<Str>> file) {
		mThis = StreamFileHolder::create () ;
		StreamFileHolder::hold (thiz)->initialize (file) ;
	}

	void set_short_read (CR<Bool> flag) {
		return StreamFileHolder::hold (thiz)->set_short_read (flag) ;
	}

	void open_r () {
		return StreamFileHolder::hold (thiz)->open_r () ;
	}

	void open_w (CR<Length> size_) {
		return StreamFileHolder::hold (thiz)->open_w (size_) ;
	}

	void open_a () {
		return StreamFileHolder::hold (thiz)->open_a () ;
	}

	Length file_size () const {
		return StreamFileHolder::hold (thiz)->file_size () ;
	}

	Length short_size () const {
		return StreamFileHolder::hold (thiz)->short_size () ;
	}

	void read (VR<RefBuffer<Byte>> item) {
		return StreamFileHolder::hold (thiz)->read (item) ;
	}

	void write (CR<RefBuffer<Byte>> item) {
		return StreamFileHolder::hold (thiz)->write (item) ;
	}

	void flush () {
		return StreamFileHolder::hold (thiz)->flush () ;
	}
} ;

struct StreamFileEncode {
	enum {
		ByteWriter ,
		TextWriter ,
		ECT
	};
} ;

struct StreamFileWriterLayout ;

struct StreamFileWriterHolder implement Interface {
	imports AutoRef<StreamFileWriterLayout> create () ;
	imports VFat<StreamFileWriterHolder> hold (VR<StreamFileWriterLayout> that) ;
	imports CFat<StreamFileWriterHolder> hold (CR<StreamFileWriterLayout> that) ;

	virtual void initialize (CR<String<Str>> file ,CR<Just<StreamFileEncode>> encode) = 0 ;
	virtual CR<Writer> ref_m () leftvalue = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFileWriter implement Like<AutoRef<StreamFileWriterLayout>> {
public:
	implicit StreamFileWriter () = default ;

	explicit StreamFileWriter (CR<String<Str>> file ,CR<Just<StreamFileEncode>> encode) {
		mThis = StreamFileWriterHolder::create () ;
		StreamFileWriterHolder::hold (thiz)->initialize (file ,encode) ;
	}

	CR<Writer> ref_m () leftvalue {
		return StreamFileWriterHolder::hold (thiz)->ref ;
	}

	forceinline operator CR<Writer> () leftvalue {
		return ref ;
	}

	void flush () {
		StreamFileWriterHolder::hold (thiz)->flush () ;
	}
} ;

struct BufferFileLayout ;

struct BufferFileHolder implement Interface {
	imports AutoRef<BufferFileLayout> create () ;
	imports VFat<BufferFileHolder> hold (VR<BufferFileLayout> that) ;
	imports CFat<BufferFileHolder> hold (CR<BufferFileLayout> that) ;

	virtual void initialize (CR<String<Str>> file) = 0 ;
	virtual void set_block_step (CR<Length> step_) = 0 ;
	virtual void set_cache_size (CR<Length> size_) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CR<Length> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual Length file_size () const = 0 ;
	virtual void read (CR<Index> index ,VR<RefBuffer<Byte>> item) = 0 ;
	virtual void write (CR<Index> index ,CR<RefBuffer<Byte>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class BufferFile implement Like<AutoRef<BufferFileLayout>> {
public:
	implicit BufferFile () = default ;

	explicit BufferFile (CR<String<Str>> file) {
		mThis = BufferFileHolder::create () ;
		BufferFileHolder::hold (thiz)->initialize (file) ;
	}

	void set_block_step (CR<Length> step_) {
		return BufferFileHolder::hold (thiz)->set_block_step (step_) ;
	}

	void set_cache_size (CR<Length> size_) {
		return BufferFileHolder::hold (thiz)->set_cache_size (size_) ;
	}

	void open_r () {
		return BufferFileHolder::hold (thiz)->open_r () ;
	}

	void open_w (CR<Length> size_) {
		return BufferFileHolder::hold (thiz)->open_w (size_) ;
	}

	void open_a () {
		return BufferFileHolder::hold (thiz)->open_a () ;
	}

	Length file_size () const {
		return BufferFileHolder::hold (thiz)->file_size () ;
	}

	void read (CR<Index> index ,VR<RefBuffer<Byte>> item) {
		return BufferFileHolder::hold (thiz)->read (index ,item) ;
	}

	void write (CR<Index> index ,CR<RefBuffer<Byte>> item) {
		return BufferFileHolder::hold (thiz)->write (index ,item) ;
	}

	void flush () {
		return BufferFileHolder::hold (thiz)->flush () ;
	}
} ;

struct UartFileLayout ;

struct UartFileHolder implement Interface {
	imports AutoRef<UartFileLayout> create () ;
	imports VFat<UartFileHolder> hold (VR<UartFileLayout> that) ;
	imports CFat<UartFileHolder> hold (CR<UartFileLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_port_name (CR<String<Str>> name) = 0 ;
	virtual void set_port_rate (CR<Length> rate) = 0 ;
	virtual void set_ring_size (CR<Length> size_) = 0 ;
	virtual void open () = 0 ;
	virtual void read (VR<RefBuffer<Byte>> buffer ,CR<Index> offset ,CR<Length> size_) = 0 ;
} ;

class UartFile implement Like<AutoRef<UartFileLayout>> {
public:
	implicit UartFile () = default ;

	implicit UartFile (CR<typeof (NULL)>) {
		mThis = UartFileHolder::create () ;
		UartFileHolder::hold (thiz)->initialize () ;
	}

	void set_port_name (CR<String<Str>> name) {
		return UartFileHolder::hold (thiz)->set_port_name (name) ;
	}

	void set_port_rate (CR<Length> rate) {
		return UartFileHolder::hold (thiz)->set_port_rate (rate) ;
	}

	void set_ring_size (CR<Length> size_) {
		return UartFileHolder::hold (thiz)->set_ring_size (size_) ;
	}

	void open () {
		return UartFileHolder::hold (thiz)->open () ;
	}

	void read (VR<RefBuffer<Byte>> buffer ,CR<Index> offset ,CR<Length> size_) {
		return UartFileHolder::hold (thiz)->read (buffer ,offset ,size_) ;
	}
} ;

struct ConsoleOption {
	enum {
		All ,
		NoPrint ,
		NoFatal ,
		NoError ,
		NoWarn ,
		NoInfo ,
		NoDebug ,
		NoTrace ,
		ETC
	} ;
} ;

struct ConsoleLayout ;

struct ConsoleHolder implement Interface {
	imports CR<Like<SharedRef<ConsoleLayout>>> expr_m () ;
	imports VFat<ConsoleHolder> hold (VR<ConsoleLayout> that) ;
	imports CFat<ConsoleHolder> hold (CR<ConsoleLayout> that) ;

	virtual void initialize () = 0 ;
	virtual void set_option (CR<Just<ConsoleOption>> option) = 0 ;
	virtual void print (CR<Format> msg) = 0 ;
	virtual void fatal (CR<Format> msg) = 0 ;
	virtual void error (CR<Format> msg) = 0 ;
	virtual void warn (CR<Format> msg) = 0 ;
	virtual void info (CR<Format> msg) = 0 ;
	virtual void debug (CR<Format> msg) = 0 ;
	virtual void trace (CR<Format> msg) = 0 ;
	virtual void open (CR<String<Str>> dire) = 0 ;
	virtual void show () = 0 ;
	virtual void hide () = 0 ;
	virtual void pause () = 0 ;
	virtual void clear () = 0 ;
} ;

class Console implement Like<SharedRef<ConsoleLayout>> {
public:
	static CR<Console> expr_m () {
		return keep[TYPE<Console>::expr] (ConsoleHolder::expr) ;
	}

	void set_option (CR<Just<ConsoleOption>> option) const {
		return ConsoleHolder::hold (thiz)->set_option (option) ;
	}

	template <class...ARG1>
	void print (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->print (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void fatal (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->fatal (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void error (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->error (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void warn (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->warn (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void info (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->info (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void debug (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->debug (PrintFormat (params...)) ;
	}

	template <class...ARG1>
	void trace (CR<ARG1>...params) const {
		return ConsoleHolder::hold (thiz)->trace (PrintFormat (params...)) ;
	}

	void open (CR<String<Str>> dire) const {
		return ConsoleHolder::hold (thiz)->open (dire) ;
	}

	void show () const {
		return ConsoleHolder::hold (thiz)->show () ;
	}

	void hide () const {
		return ConsoleHolder::hold (thiz)->hide () ;
	}

	void pause () const {
		return ConsoleHolder::hold (thiz)->pause () ;
	}

	void clear () const {
		return ConsoleHolder::hold (thiz)->clear () ;
	}
} ;
} ;