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
	List<UniqueRef<String<Str>>> mLockDirectory ;
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
	String<Str> mFile ;
	UniqueRef<csc_pipe_t> mReadPipe ;
	UniqueRef<csc_pipe_t> mWritePipe ;
	Val64 mFileSize ;
	Val64 mRead ;
	Val64 mWrite ;
	Bool mShortRead ;
	Length mShortSize ;
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

struct StreamFileWriterLayout {
	StreamFile mStreamFile ;
	RefBuffer<Byte> mFileBuffer ;
	AutoRef<Writer> mFileWriter ;

public:
	implicit StreamFileWriterLayout () = default ;

	implicit ~StreamFileWriterLayout () noexcept {
		StreamFileWriterHolder::hold (thiz)->flush () ;
	}
} ;

class StreamFileWriterImplHolder final implement Fat<StreamFileWriterHolder ,StreamFileWriterLayout> {
public:
	void initialize (CR<String<Str>> file ,CR<Just<StreamFileEncode>> encode) override {
		self.mStreamFile = StreamFile (file) ;
		self.mStreamFile.open_w (0) ;
		self.mFileBuffer = RefBuffer<Byte> (STREAMFILE_CHUNK_STEP::expr) ;
		set_writer (encode) ;
	}

	void set_writer (CR<Just<StreamFileEncode>> encode) {
		auto act = TRUE ;
		if ifdo (act) {
			if (encode != StreamFileEncode::ByteWriter)
				discard ;
			self.mFileWriter = AutoRef<ByteWriter>::make (Ref<RefBuffer<Byte>>::reference (self.mFileBuffer)) ;
		}
		if ifdo (act) {
			if (encode != StreamFileEncode::TextWriter)
				discard ;
			self.mFileWriter = AutoRef<TextWriter>::make (Ref<RefBuffer<Byte>>::reference (self.mFileBuffer)) ;
		}
		auto &&rax = self ;
		self.mFileWriter->use_overflow ([&] (VR<Writer> writer) {
			rax.mStreamFile.write (rax.mFileBuffer) ;
			rax.mFileWriter->reset () ;
		}) ;
	}

	VR<Writer> ref_m () leftvalue override {
		return self.mFileWriter.ref ;
	}

	void flush () override {
		const auto r1x = self.mFileWriter->length () ;
		if (r1x == 0)
			return ;
		const auto r2x = Flag (self.mFileBuffer.ref) ;
		self.mStreamFile.write (RefBuffer<Byte>::reference (r2x ,r1x)) ;
		self.mFileWriter->reset () ;
		self.mStreamFile.flush () ;
	}
} ;

exports AutoRef<StreamFileWriterLayout> StreamFileWriterHolder::create () {
	return AutoRef<StreamFileWriterLayout>::make () ;
}

exports VFat<StreamFileWriterHolder> StreamFileWriterHolder::hold (VR<StreamFileWriterLayout> that) {
	return VFat<StreamFileWriterHolder> (StreamFileWriterImplHolder () ,that) ;
}

exports CFat<StreamFileWriterHolder> StreamFileWriterHolder::hold (CR<StreamFileWriterLayout> that) {
	return CFat<StreamFileWriterHolder> (StreamFileWriterImplHolder () ,that) ;
}

template class External<BufferFileHolder ,BufferFileLayout> ;

struct BufferFileHeader {
	Quad mFileEndian ;
	Val64 mFileSize ;
	Val64 mBlockSize ;
	Val64 mBlockStep ;
	Val64 mBlockLength ;
	Val64 mChunkSize ;
	Val64 mChunkStep ;
	Val64 mChunkLength ;
} ;

struct BufferFileChunk {
	Val64 mIndex ;
	Val64 mCacheTime ;
	UniqueRef<Tuple<Flag ,Flag>> mBlock ;
} ;

struct BufferFileLayout {
	String<Str> mFile ;
	UniqueRef<csc_pipe_t> mPipe ;
	UniqueRef<csc_handle_t> mMapping ;
	Val64 mFileSize ;
	Val64 mBlockStep ;
	Val64 mChunkStep ;
	csc_enum_t mFileMapFlag ;
	Box<BufferFileHeader> mHeader ;
	Set<Val64> mCacheSet ;
	List<BufferFileChunk> mCacheList ;
	Val64 mCacheTimer ;
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
	String<Str> mPortName ;
	Length mPortRate ;
	UniqueRef<csc_pipe_t> mPipe ;
	Ref<UartFileCOMParams> mCOMParams ;
	Ref<UartFileCOMStatus> mCOMStatus ;
	csc_enum_t mCOMError ;
	RefBuffer<Byte> mRingBuffer ;
	Index mRingRead ;
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
	String<Str> mLogBuffer ;
	TextWriter mLogWriter ;
	String<Str> mLogFile ;
	String<Str> mOldLogFile ;
	StreamFile mLogStreamFile ;
	Bool mDebugMode ;
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