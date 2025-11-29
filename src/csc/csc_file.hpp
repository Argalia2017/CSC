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

	virtual void initialize (RR<String<STR>> pathname) = 0 ;
	virtual void initialize (CR<Deque<String<STR>>> pathname) = 0 ;
	virtual void initialize (CR<PathLayout> that) = 0 ;
	virtual String<STR> fetch () const = 0 ;
	virtual PathLayout child (CR<Slice> name) const = 0 ;
	virtual PathLayout child (CR<Format> name) const = 0 ;
	virtual PathLayout child (CR<String<STR>> name) const = 0 ;
	virtual Array<PathLayout> list () const = 0 ;
	virtual Array<PathLayout> list (CR<LENGTH> size_) const = 0 ;
	virtual BOOL equal (CR<PathLayout> that) const = 0 ;
	virtual BOOL is_file () const = 0 ;
	virtual BOOL is_dire () const = 0 ;
	virtual BOOL is_link () const = 0 ;
	virtual PathLayout symbolic () const = 0 ;
	virtual PathLayout absolute () const = 0 ;
	virtual Deque<String<STR>> decouple () const = 0 ;
	virtual String<STR> path () const = 0 ;
	virtual String<STR> name () const = 0 ;
	virtual String<STR> stem () const = 0 ;
	virtual String<STR> extension () const = 0 ;
} ;

class Path implement PathLayout {
protected:
	using PathLayout::mThis ;

public:
	implicit Path () = default ;

	explicit Path (CR<String<STR>> pathname) {
		PathHolder::hold (thiz)->initialize (move (pathname)) ;
	}

	explicit Path (RR<String<STR>> pathname) {
		PathHolder::hold (thiz)->initialize (move (pathname)) ;
	}

	explicit Path (CR<Deque<String<STR>>> pathname) {
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

	String<STR> fetch () const {
		return PathHolder::hold (thiz)->fetch () ;
	}

	forceinline operator String<STR> () const {
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

	Path child (CR<String<STR>> name) const {
		PathLayout ret = PathHolder::hold (thiz)->child (name) ;
		return move (keep[TYPE<Path>::expr] (ret)) ;
	}

	Array<Path> list () const {
		ArrayLayout ret = PathHolder::hold (thiz)->list () ;
		return move (keep[TYPE<Array<Path>>::expr] (Pointer::from (ret))) ;
	}

	Array<Path> list (CR<LENGTH> size_) const {
		ArrayLayout ret = PathHolder::hold (thiz)->list (size_) ;
		return move (keep[TYPE<Array<Path>>::expr] (Pointer::from (ret))) ;
	}

	BOOL equal (CR<Path> that) const {
		return PathHolder::hold (thiz)->equal (that) ;
	}

	forceinline BOOL operator== (CR<Path> that) const {
		return equal (that) ;
	}

	forceinline BOOL operator!= (CR<Path> that) const {
		return (!equal (that)) ;
	}

	BOOL is_file () const {
		return PathHolder::hold (thiz)->is_file () ;
	}

	BOOL is_dire () const {
		return PathHolder::hold (thiz)->is_dire () ;
	}

	BOOL is_link () const {
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

	Deque<String<STR>> decouple () const {
		return PathHolder::hold (thiz)->decouple () ;
	}

	String<STR> path () const {
		return PathHolder::hold (thiz)->path () ;
	}

	String<STR> name () const {
		return PathHolder::hold (thiz)->name () ;
	}

	String<STR> stem () const {
		return PathHolder::hold (thiz)->stem () ;
	}

	String<STR> extension () const {
		return PathHolder::hold (thiz)->extension () ;
	}
} ;

struct FileProcLayout ;

struct FileProcHolder implement Interface {
	imports CR<OfThis<UniqueRef<FileProcLayout>>> expr_m () ;
	imports VFat<FileProcHolder> hold (VR<FileProcLayout> that) ;
	imports CFat<FileProcHolder> hold (CR<FileProcLayout> that) ;

	virtual void initialize () = 0 ;
	virtual RefBuffer<BYTE> load_file (CR<String<STR>> file) const = 0 ;
	virtual void save_file (CR<String<STR>> file ,CR<RefBuffer<BYTE>> item) const = 0 ;
	virtual Ref<RefBuffer<BYTE>> load_asset (CR<String<STR>> file) const = 0 ;
	virtual void copy_file (CR<String<STR>> dst ,CR<String<STR>> src) const = 0 ;
	virtual void move_file (CR<String<STR>> dst ,CR<String<STR>> src) const = 0 ;
	virtual void link_file (CR<String<STR>> dst ,CR<String<STR>> src) const = 0 ;
	virtual void erase_file (CR<String<STR>> file) const = 0 ;
	virtual void build_dire (CR<String<STR>> dire) const = 0 ;
	virtual void link_dire (CR<String<STR>> dst ,CR<String<STR>> src) const = 0 ;
	virtual void clear_dire (CR<String<STR>> dire) const = 0 ;
	virtual void erase_dire (CR<String<STR>> dire) const = 0 ;
	virtual BOOL lock_dire (CR<String<STR>> dire) const = 0 ;
} ;

class FileProc implement OfThis<UniqueRef<FileProcLayout>> {
public:
	static CR<FileProc> expr_m () {
		return keep[TYPE<FileProc>::expr] (FileProcHolder::expr) ;
	}

	static RefBuffer<BYTE> load_file (CR<String<STR>> file) {
		return FileProcHolder::hold (expr)->load_file (file) ;
	}

	static void save_file (CR<String<STR>> file ,CR<RefBuffer<BYTE>> item) {
		return FileProcHolder::hold (expr)->save_file (file ,item) ;
	}

	static Ref<RefBuffer<BYTE>> load_asset (CR<String<STR>> file) {
		return FileProcHolder::hold (expr)->load_asset (file) ;
	}

	static void copy_file (CR<String<STR>> dst ,CR<String<STR>> src) {
		return FileProcHolder::hold (expr)->copy_file (dst ,src) ;
	}

	static void move_file (CR<String<STR>> dst ,CR<String<STR>> src) {
		return FileProcHolder::hold (expr)->move_file (dst ,src) ;
	}

	static void link_file (CR<String<STR>> dst ,CR<String<STR>> src) {
		return FileProcHolder::hold (expr)->link_file (dst ,src) ;
	}

	static void erase_file (CR<String<STR>> file) {
		return FileProcHolder::hold (expr)->erase_file (file) ;
	}

	static void build_dire (CR<String<STR>> dire) {
		return FileProcHolder::hold (expr)->build_dire (dire) ;
	}

	static void link_dire (CR<String<STR>> dst ,CR<String<STR>> src) {
		return FileProcHolder::hold (expr)->link_dire (dst ,src) ;
	}

	static void clear_dire (CR<String<STR>> dire) {
		return FileProcHolder::hold (expr)->clear_dire (dire) ;
	}

	static void erase_dire (CR<String<STR>> dire) {
		return FileProcHolder::hold (expr)->erase_dire (dire) ;
	}

	static BOOL lock_dire (CR<String<STR>> dire) {
		return FileProcHolder::hold (expr)->lock_dire (dire) ;
	}
} ;

struct StreamFileLayout ;

struct StreamFileHolder implement Interface {
	imports AutoRef<StreamFileLayout> create () ;
	imports VFat<StreamFileHolder> hold (VR<StreamFileLayout> that) ;
	imports CFat<StreamFileHolder> hold (CR<StreamFileLayout> that) ;

	virtual void initialize (CR<String<STR>> file) = 0 ;
	virtual void set_short_read (CR<BOOL> flag) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CR<LENGTH> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual LENGTH short_size () const = 0 ;
	virtual void read (VR<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CR<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

using STREAMFILE_CHUNK_STEP = ENUM<65536> ;

class StreamFile implement OfThis<AutoRef<StreamFileLayout>> {
public:
	implicit StreamFile () = default ;

	explicit StreamFile (CR<String<STR>> file) {
		mThis = StreamFileHolder::create () ;
		StreamFileHolder::hold (thiz)->initialize (file) ;
	}

	void set_short_read (CR<BOOL> flag) {
		return StreamFileHolder::hold (thiz)->set_short_read (flag) ;
	}

	void open_r () {
		return StreamFileHolder::hold (thiz)->open_r () ;
	}

	void open_w (CR<LENGTH> size_) {
		return StreamFileHolder::hold (thiz)->open_w (size_) ;
	}

	void open_a () {
		return StreamFileHolder::hold (thiz)->open_a () ;
	}

	LENGTH file_size () const {
		return StreamFileHolder::hold (thiz)->file_size () ;
	}

	LENGTH short_size () const {
		return StreamFileHolder::hold (thiz)->short_size () ;
	}

	void read (VR<RefBuffer<BYTE>> item) {
		return StreamFileHolder::hold (thiz)->read (item) ;
	}

	void write (CR<RefBuffer<BYTE>> item) {
		return StreamFileHolder::hold (thiz)->write (item) ;
	}

	void flush () {
		return StreamFileHolder::hold (thiz)->flush () ;
	}
} ;

struct StreamFileByteWriterLayout ;

struct StreamFileByteWriterHolder implement Interface {
	imports AutoRef<StreamFileByteWriterLayout> create () ;
	imports VFat<StreamFileByteWriterHolder> hold (VR<StreamFileByteWriterLayout> that) ;
	imports CFat<StreamFileByteWriterHolder> hold (CR<StreamFileByteWriterLayout> that) ;

	virtual void initialize (CR<String<STR>> file) = 0 ;
	virtual VR<ByteWriter> ref_m () leftvalue = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFileByteWriter implement OfThis<AutoRef<StreamFileByteWriterLayout>> {
public:
	implicit StreamFileByteWriter () = default ;

	explicit StreamFileByteWriter (CR<String<STR>> file) {
		mThis = StreamFileByteWriterHolder::create () ;
		StreamFileByteWriterHolder::hold (thiz)->initialize (file) ;
	}

	VR<ByteWriter> ref_m () leftvalue {
		return StreamFileByteWriterHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<ByteWriter> () leftvalue {
		return ref ;
	}

	void flush () {
		StreamFileByteWriterHolder::hold (thiz)->flush () ;
	}
} ;

struct StreamFileTextWriterLayout ;

struct StreamFileTextWriterHolder implement Interface {
	imports AutoRef<StreamFileTextWriterLayout> create () ;
	imports VFat<StreamFileTextWriterHolder> hold (VR<StreamFileTextWriterLayout> that) ;
	imports CFat<StreamFileTextWriterHolder> hold (CR<StreamFileTextWriterLayout> that) ;

	virtual void initialize (CR<String<STR>> file) = 0 ;
	virtual VR<TextWriter> ref_m () leftvalue = 0 ;
	virtual void flush () = 0 ;
} ;

class StreamFileTextWriter implement OfThis<AutoRef<StreamFileTextWriterLayout>> {
public:
	implicit StreamFileTextWriter () = default ;

	explicit StreamFileTextWriter (CR<String<STR>> file) {
		mThis = StreamFileTextWriterHolder::create () ;
		StreamFileTextWriterHolder::hold (thiz)->initialize (file) ;
	}

	VR<TextWriter> ref_m () leftvalue {
		return StreamFileTextWriterHolder::hold (thiz)->ref ;
	}

	forceinline operator VR<TextWriter> () leftvalue {
		return ref ;
	}

	void flush () {
		StreamFileTextWriterHolder::hold (thiz)->flush () ;
	}
} ;

struct BufferFileLayout ;

struct BufferFileHolder implement Interface {
	imports AutoRef<BufferFileLayout> create () ;
	imports VFat<BufferFileHolder> hold (VR<BufferFileLayout> that) ;
	imports CFat<BufferFileHolder> hold (CR<BufferFileLayout> that) ;

	virtual void initialize (CR<String<STR>> file) = 0 ;
	virtual void set_block_step (CR<LENGTH> step_) = 0 ;
	virtual void set_cache_size (CR<LENGTH> size_) = 0 ;
	virtual void open_r () = 0 ;
	virtual void open_w (CR<LENGTH> size_) = 0 ;
	virtual void open_a () = 0 ;
	virtual LENGTH file_size () const = 0 ;
	virtual void read (CR<INDEX> index ,VR<RefBuffer<BYTE>> item) = 0 ;
	virtual void write (CR<INDEX> index ,CR<RefBuffer<BYTE>> item) = 0 ;
	virtual void flush () = 0 ;
} ;

class BufferFile implement OfThis<AutoRef<BufferFileLayout>> {
public:
	implicit BufferFile () = default ;

	explicit BufferFile (CR<String<STR>> file) {
		mThis = BufferFileHolder::create () ;
		BufferFileHolder::hold (thiz)->initialize (file) ;
	}

	void set_block_step (CR<LENGTH> step_) {
		return BufferFileHolder::hold (thiz)->set_block_step (step_) ;
	}

	void set_cache_size (CR<LENGTH> size_) {
		return BufferFileHolder::hold (thiz)->set_cache_size (size_) ;
	}

	void open_r () {
		return BufferFileHolder::hold (thiz)->open_r () ;
	}

	void open_w (CR<LENGTH> size_) {
		return BufferFileHolder::hold (thiz)->open_w (size_) ;
	}

	void open_a () {
		return BufferFileHolder::hold (thiz)->open_a () ;
	}

	LENGTH file_size () const {
		return BufferFileHolder::hold (thiz)->file_size () ;
	}

	void read (CR<INDEX> index ,VR<RefBuffer<BYTE>> item) {
		return BufferFileHolder::hold (thiz)->read (index ,item) ;
	}

	void write (CR<INDEX> index ,CR<RefBuffer<BYTE>> item) {
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
	virtual void set_port_name (CR<String<STR>> name) = 0 ;
	virtual void set_port_rate (CR<LENGTH> rate) = 0 ;
	virtual void set_ring_size (CR<LENGTH> size_) = 0 ;
	virtual void open () = 0 ;
	virtual void read (VR<RefBuffer<BYTE>> buffer ,CR<INDEX> offset ,CR<LENGTH> size_) = 0 ;
} ;

class UartFile implement OfThis<AutoRef<UartFileLayout>> {
public:
	implicit UartFile () = default ;

	implicit UartFile (CR<typeof (NULL)>) {
		mThis = UartFileHolder::create () ;
		UartFileHolder::hold (thiz)->initialize () ;
	}

	void set_port_name (CR<String<STR>> name) {
		return UartFileHolder::hold (thiz)->set_port_name (name) ;
	}

	void set_port_rate (CR<LENGTH> rate) {
		return UartFileHolder::hold (thiz)->set_port_rate (rate) ;
	}

	void set_ring_size (CR<LENGTH> size_) {
		return UartFileHolder::hold (thiz)->set_ring_size (size_) ;
	}

	void open () {
		return UartFileHolder::hold (thiz)->open () ;
	}

	void read (VR<RefBuffer<BYTE>> buffer ,CR<INDEX> offset ,CR<LENGTH> size_) {
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
	imports CR<OfThis<SharedRef<ConsoleLayout>>> expr_m () ;
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
	virtual void open (CR<String<STR>> dire) = 0 ;
	virtual void show () = 0 ;
	virtual void hide () = 0 ;
	virtual void pause () = 0 ;
	virtual void clear () = 0 ;
} ;

class Console implement OfThis<SharedRef<ConsoleLayout>> {
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

	void open (CR<String<STR>> dire) const {
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