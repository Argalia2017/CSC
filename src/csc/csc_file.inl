#pragma once

#ifndef __CSC_FILE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_file.hpp"

#include "csc_end.h"
#include <cstdio>
#include "csc_begin.h"

namespace CSC {
template class External<PathHolder ,PathLayout> ;

exports VFat<PathHolder> PathHolder::hold (VR<PathLayout> that) {
	return VFat<PathHolder> (External<PathHolder ,PathLayout>::expr ,that) ;
}

exports CFat<PathHolder> PathHolder::hold (CR<PathLayout> that) {
	return CFat<PathHolder> (External<PathHolder ,PathLayout>::expr ,that) ;
}

template class External<FileProcHolder ,FileProcLayout> ;

struct FileProcLayout {
	Mutex mMutex ;
	List<UniqueRef<String<STR>>> mLockDirectory ;
} ;

exports CR<Like<UniqueRef<FileProcLayout>>> FileProcHolder::expr_m () {
	return memorize ([&] () {
		Like<UniqueRef<FileProcLayout>> ret ;
		ret.mThis = UniqueRef<FileProcLayout>::make () ;
		FileProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FileProcHolder> FileProcHolder::hold (VR<FileProcLayout> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::expr ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::hold (CR<FileProcLayout> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::expr ,that) ;
}

template class External<StreamFileHolder ,StreamFileLayout> ;

struct StreamFileLayout {
	String<STR> mFile ;
	UniqueRef<csc_pipe_t> mReadPipe ;
	UniqueRef<csc_pipe_t> mWritePipe ;
	VAL64 mFileSize ;
	VAL64 mRead ;
	VAL64 mWrite ;
	BOOL mShortRead ;
	LENGTH mShortSize ;
} ;

exports AutoRef<StreamFileLayout> StreamFileHolder::create () {
	return AutoRef<StreamFileLayout>::make () ;
}

exports VFat<StreamFileHolder> StreamFileHolder::hold (VR<StreamFileLayout> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::expr ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::hold (CR<StreamFileLayout> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::expr ,that) ;
}

struct StreamFileByteWriterLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	ByteWriter mFileWriter ;

public:
	implicit StreamFileByteWriterLayout () = default ;

	implicit ~StreamFileByteWriterLayout () noexcept {
		StreamFileByteWriterHolder::hold (thiz)->flush () ;
	}
} ;

class StreamFileByteWriterImplHolder final implement Fat<StreamFileByteWriterHolder ,StreamFileByteWriterLayout> {
public:
	void initialize (CR<String<STR>> file) override {
		self.mStreamFile = StreamFile (file) ;
		self.mStreamFile.open_w (0) ;
		self.mFileBuffer = RefBuffer<BYTE> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer () ;
	}

	void set_writer () {
		self.mFileWriter = ByteWriter (Ref<RefBuffer<BYTE>>::reference (self.mFileBuffer)) ;
		auto &&rax = self ;
		self.mFileWriter.use_overflow ([&] (VR<ByteWriter> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter.reset () ;
		}) ;
	}

	VR<ByteWriter> ref_m () leftvalue override {
		return self.mFileWriter ;
	}

	void flush () override {
		const auto r1x = self.mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (self.mFileBuffer.ref) ;
		self.mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		self.mFileWriter.reset () ;
		self.mStreamFile.flush () ;
	}
} ;

exports AutoRef<StreamFileByteWriterLayout> StreamFileByteWriterHolder::create () {
	return AutoRef<StreamFileByteWriterLayout>::make () ;
}

exports VFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (VR<StreamFileByteWriterLayout> that) {
	return VFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

exports CFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (CR<StreamFileByteWriterLayout> that) {
	return CFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

struct StreamFileTextWriterLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	TextWriter mFileWriter ;

public:
	implicit StreamFileTextWriterLayout () = default ;

	implicit ~StreamFileTextWriterLayout () noexcept {
		StreamFileTextWriterHolder::hold (thiz)->flush () ;
	}
} ;

class StreamFileTextWriterImplHolder final implement Fat<StreamFileTextWriterHolder ,StreamFileTextWriterLayout> {
public:
	void initialize (CR<String<STR>> file) override {
		self.mStreamFile = StreamFile (file) ;
		self.mStreamFile.open_w (0) ;
		self.mFileBuffer = RefBuffer<BYTE> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer () ;
	}

	void set_writer () {
		self.mFileWriter = TextWriter (Ref<RefBuffer<BYTE>>::reference (self.mFileBuffer)) ;
		auto &&rax = self ;
		self.mFileWriter.use_overflow ([&] (VR<TextWriter> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter.reset () ;
		}) ;
	}

	VR<TextWriter> ref_m () leftvalue override {
		return self.mFileWriter ;
	}

	void flush () override {
		const auto r1x = self.mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (self.mFileBuffer.ref) ;
		self.mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		self.mFileWriter.reset () ;
		self.mStreamFile.flush () ;
	}
} ;

exports AutoRef<StreamFileTextWriterLayout> StreamFileTextWriterHolder::create () {
	return AutoRef<StreamFileTextWriterLayout>::make () ;
}

exports VFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (VR<StreamFileTextWriterLayout> that) {
	return VFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

exports CFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (CR<StreamFileTextWriterLayout> that) {
	return CFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

template class External<BufferFileHolder ,BufferFileLayout> ;

struct BufferFileHeader {
	QUAD mFileEndian ;
	VAL64 mFileSize ;
	VAL64 mBlockSize ;
	VAL64 mBlockStep ;
	VAL64 mBlockLength ;
	VAL64 mChunkSize ;
	VAL64 mChunkStep ;
	VAL64 mChunkLength ;
} ;

struct BufferFileChunk {
	VAL64 mIndex ;
	VAL64 mCacheTime ;
	UniqueRef<Tuple<FLAG ,FLAG>> mBlock ;
} ;

struct BufferFileLayout {
	String<STR> mFile ;
	UniqueRef<csc_pipe_t> mPipe ;
	UniqueRef<csc_handle_t> mMapping ;
	VAL64 mFileSize ;
	VAL64 mBlockStep ;
	VAL64 mChunkStep ;
	csc_enum_t mFileMapFlag ;
	Box<BufferFileHeader> mHeader ;
	Set<VAL64> mCacheSet ;
	List<BufferFileChunk> mCacheList ;
	VAL64 mCacheTimer ;
} ;

exports AutoRef<BufferFileLayout> BufferFileHolder::create () {
	return AutoRef<BufferFileLayout>::make () ;
}

exports VFat<BufferFileHolder> BufferFileHolder::hold (VR<BufferFileLayout> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::expr ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::hold (CR<BufferFileLayout> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::expr ,that) ;
}

template class External<UartFileHolder ,UartFileLayout> ;

struct UartFileCOMParams ;
struct UartFileCOMStatus ;

struct UartFileLayout {
	String<STR> mPortName ;
	LENGTH mPortRate ;
	UniqueRef<csc_pipe_t> mPipe ;
	Ref<UartFileCOMParams> mCOMParams ;
	Ref<UartFileCOMStatus> mCOMStatus ;
	csc_enum_t mCOMError ;
	RefBuffer<BYTE> mRingBuffer ;
	INDEX mRingRead ;
} ;

exports AutoRef<UartFileLayout> UartFileHolder::create () {
	return AutoRef<UartFileLayout>::make () ;
}

exports VFat<UartFileHolder> UartFileHolder::hold (VR<UartFileLayout> that) {
	return VFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::expr ,that) ;
}

exports CFat<UartFileHolder> UartFileHolder::hold (CR<UartFileLayout> that) {
	return CFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::expr ,that) ;
}

template class External<ConsoleHolder ,ConsoleLayout> ;

struct ConsoleLayout {
	Mutex mMutex ;
	BitSet mOption ;
	UniqueRef<csc_handle_t> mConsole ;
	String<STR> mLogBuffer ;
	TextWriter mLogWriter ;
	String<STR> mLogFile ;
	String<STR> mOldLogFile ;
	StreamFile mLogStreamFile ;
	BOOL mDebugMode ;
	System mCommand ;
} ;

exports CR<Like<SharedRef<ConsoleLayout>>> ConsoleHolder::expr_m () {
	return memorize ([&] () {
		Like<SharedRef<ConsoleLayout>> ret ;
		ret.mThis = SharedRef<ConsoleLayout>::make () ;
		ConsoleHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConsoleHolder> ConsoleHolder::hold (VR<ConsoleLayout> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::expr ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::hold (CR<ConsoleLayout> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::expr ,that) ;
}
} ;