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

exports VFat<PathHolder> PathHolder::hold (VREF<PathLayout> that) {
	return VFat<PathHolder> (External<PathHolder ,PathLayout>::declare () ,that) ;
}

exports CFat<PathHolder> PathHolder::hold (CREF<PathLayout> that) {
	return CFat<PathHolder> (External<PathHolder ,PathLayout>::declare () ,that) ;
}

template class External<FileProcHolder ,FileProcLayout> ;

struct FileProcLayout {
	Mutex mMutex ;
	Pin<List<UniqueRef<String<STR>>>> mLockDirectory ;
} ;

exports CREF<OfThis<UniqueRef<FileProcLayout>>> FileProcHolder::instance () {
	return memorize ([&] () {
		OfThis<UniqueRef<FileProcLayout>> ret ;
		ret.mThis = UniqueRef<FileProcLayout>::make () ;
		FileProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FileProcHolder> FileProcHolder::hold (VREF<FileProcLayout> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::declare () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::hold (CREF<FileProcLayout> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,FileProcLayout>::declare () ,that) ;
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

exports OfThis<AutoRef<StreamFileLayout>> StreamFileHolder::create () {
	OfThis<AutoRef<StreamFileLayout>> ret ;
	ret.mThis = AutoRef<StreamFileLayout>::make () ;
	return move (ret) ;
}

exports VFat<StreamFileHolder> StreamFileHolder::hold (VREF<StreamFileLayout> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::declare () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::hold (CREF<StreamFileLayout> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,StreamFileLayout>::declare () ,that) ;
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
	void initialize (CREF<String<STR>> file) override {
		self.mStreamFile = StreamFile (file) ;
		self.mStreamFile.open_w (0) ;
		self.mFileBuffer = RefBuffer<BYTE> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer () ;
	}

	void set_writer () {
		self.mFileWriter = ByteWriter (Ref<RefBuffer<BYTE>>::reference (self.mFileBuffer)) ;
		auto &&rax = self ;
		self.mFileWriter.use_overflow ([&] (VREF<ByteWriter> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter.reset () ;
		}) ;
	}

	VREF<ByteWriter> deref_m () leftvalue override {
		return self.mFileWriter ;
	}

	void flush () override {
		const auto r1x = self.mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (self.mFileBuffer.deref) ;
		self.mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		self.mFileWriter.reset () ;
		self.mStreamFile.flush () ;
	}
} ;

exports OfThis<AutoRef<StreamFileByteWriterLayout>> StreamFileByteWriterHolder::create () {
	OfThis<AutoRef<StreamFileByteWriterLayout>> ret ;
	ret.mThis = AutoRef<StreamFileByteWriterLayout>::make () ;
	return move (ret) ;
}

exports VFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (VREF<StreamFileByteWriterLayout> that) {
	return VFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

exports CFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (CREF<StreamFileByteWriterLayout> that) {
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
	void initialize (CREF<String<STR>> file) override {
		self.mStreamFile = StreamFile (file) ;
		self.mStreamFile.open_w (0) ;
		self.mFileBuffer = RefBuffer<BYTE> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer () ;
	}

	void set_writer () {
		self.mFileWriter = TextWriter (Ref<RefBuffer<BYTE>>::reference (self.mFileBuffer)) ;
		auto &&rax = self ;
		self.mFileWriter.use_overflow ([&] (VREF<TextWriter> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter.reset () ;
		}) ;
	}

	VREF<TextWriter> deref_m () leftvalue override {
		return self.mFileWriter ;
	}

	void flush () override {
		const auto r1x = self.mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (self.mFileBuffer.deref) ;
		self.mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		self.mFileWriter.reset () ;
		self.mStreamFile.flush () ;
	}
} ;

exports OfThis<AutoRef<StreamFileTextWriterLayout>> StreamFileTextWriterHolder::create () {
	OfThis<AutoRef<StreamFileTextWriterLayout>> ret ;
	ret.mThis = AutoRef<StreamFileTextWriterLayout>::make () ;
	return move (ret) ;
}

exports VFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (VREF<StreamFileTextWriterLayout> that) {
	return VFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

exports CFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (CREF<StreamFileTextWriterLayout> that) {
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

exports OfThis<AutoRef<BufferFileLayout>> BufferFileHolder::create () {
	OfThis<AutoRef<BufferFileLayout>> ret ;
	ret.mThis = AutoRef<BufferFileLayout>::make () ;
	return move (ret) ;
}

exports VFat<BufferFileHolder> BufferFileHolder::hold (VREF<BufferFileLayout> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::declare () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::hold (CREF<BufferFileLayout> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,BufferFileLayout>::declare () ,that) ;
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

exports OfThis<AutoRef<UartFileLayout>> UartFileHolder::create () {
	OfThis<AutoRef<UartFileLayout>> ret ;
	ret.mThis = AutoRef<UartFileLayout>::make () ;
	return move (ret) ;
}

exports VFat<UartFileHolder> UartFileHolder::hold (VREF<UartFileLayout> that) {
	return VFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::declare () ,that) ;
}

exports CFat<UartFileHolder> UartFileHolder::hold (CREF<UartFileLayout> that) {
	return CFat<UartFileHolder> (External<UartFileHolder ,UartFileLayout>::declare () ,that) ;
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
	System mCommand ;
} ;

exports CREF<OfThis<SharedRef<ConsoleLayout>>> ConsoleHolder::instance () {
	return memorize ([&] () {
		OfThis<SharedRef<ConsoleLayout>> ret ;
		ret.mThis = SharedRef<ConsoleLayout>::make () ;
		ConsoleHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConsoleHolder> ConsoleHolder::hold (VREF<ConsoleLayout> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::declare () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::hold (CREF<ConsoleLayout> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,ConsoleLayout>::declare () ,that) ;
}
} ;