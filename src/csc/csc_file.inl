#pragma once

#ifndef __CSC_FILE__
#error "∑(っ°Д° ;)っ : require module"
#endif

#include "csc_file.hpp"

#include "csc_end.h"
#include <cstdio>
#include "csc_begin.h"

namespace CSC {
template class External<PathHolder ,Ref<PathLayout>> ;

exports VFat<PathHolder> PathHolder::hold (VREF<Ref<PathLayout>> that) {
	return VFat<PathHolder> (External<PathHolder ,Ref<PathLayout>>::declare () ,that) ;
}

exports CFat<PathHolder> PathHolder::hold (CREF<Ref<PathLayout>> that) {
	return CFat<PathHolder> (External<PathHolder ,Ref<PathLayout>>::declare () ,that) ;
}

template class External<FileProcHolder ,Ref<FileProcLayout>> ;

exports CREF<Ref<FileProcLayout>> FileProcHolder::instance () {
	return memorize ([&] () {
		Ref<FileProcLayout> ret ;
		FileProcHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<FileProcHolder> FileProcHolder::hold (VREF<Ref<FileProcLayout>> that) {
	return VFat<FileProcHolder> (External<FileProcHolder ,Ref<FileProcLayout>>::declare () ,that) ;
}

exports CFat<FileProcHolder> FileProcHolder::hold (CREF<Ref<FileProcLayout>> that) {
	return CFat<FileProcHolder> (External<FileProcHolder ,Ref<FileProcLayout>>::declare () ,that) ;
}

template class External<StreamFileHolder ,AutoRef<StreamFileLayout>> ;

exports VFat<StreamFileHolder> StreamFileHolder::hold (VREF<AutoRef<StreamFileLayout>> that) {
	return VFat<StreamFileHolder> (External<StreamFileHolder ,AutoRef<StreamFileLayout>>::declare () ,that) ;
}

exports CFat<StreamFileHolder> StreamFileHolder::hold (CREF<AutoRef<StreamFileLayout>> that) {
	return CFat<StreamFileHolder> (External<StreamFileHolder ,AutoRef<StreamFileLayout>>::declare () ,that) ;
}

struct StreamFileByteWriterLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	ByteWriter mFileWriter ;
} ;

class StreamFileByteWriterImpl implement StreamFileByteWriterLayout {
protected:
	using StreamFileByteWriterLayout::mStreamFile ;
	using StreamFileByteWriterLayout::mFileBuffer ;
	using StreamFileByteWriterLayout::mFileWriter ;

public:
	implicit StreamFileByteWriterImpl () = default ;

	implicit ~StreamFileByteWriterImpl () noexcept {
		flush () ;
	}

	void initialize (CREF<String<STR>> file) {
		mStreamFile = StreamFile (file) ;
		mStreamFile.open_w (0) ;
		mFileBuffer = RefBuffer<BYTE> (STREAMFILE_BUF_SIZE::expr) ;
		set_writer () ;
	}

	void set_writer () {
		mFileWriter = ByteWriter (Ref<RefBuffer<BYTE>>::reference (mFileBuffer)) ;
		mFileWriter.use_overflow ([&] (VREF<ByteWriter> writer) {
			mStreamFile.write (mFileBuffer) ;
			mFileWriter.reset () ;
		}) ;
	}

	void flush () {
		const auto r1x = mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (mFileBuffer.self) ;
		mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		mFileWriter.reset () ;
		mStreamFile.flush () ;
	}
} ;

class StreamFileByteWriterImplHolder final implement Fat<StreamFileByteWriterHolder ,AutoRef<StreamFileByteWriterLayout>> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake = AutoRef<StreamFileByteWriterImpl>::make () ;
		ptr (fake).initialize (file) ;
	}

	static VREF<StreamFileByteWriterImpl> ptr (VREF<AutoRef<StreamFileByteWriterLayout>> that) {
		return keep[TYPE<StreamFileByteWriterImpl>::expr] (that.self) ;
	}

	VREF<ByteWriter> self_m () leftvalue override {
		return fake->mFileWriter ;
	}

	void flush () override {
		return ptr (fake).flush () ;
	}
} ;

exports VFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (VREF<AutoRef<StreamFileByteWriterLayout>> that) {
	return VFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

exports CFat<StreamFileByteWriterHolder> StreamFileByteWriterHolder::hold (CREF<AutoRef<StreamFileByteWriterLayout>> that) {
	return CFat<StreamFileByteWriterHolder> (StreamFileByteWriterImplHolder () ,that) ;
}

struct StreamFileTextWriterLayout {
	StreamFile mStreamFile ;
	RefBuffer<BYTE> mFileBuffer ;
	TextWriter mFileWriter ;
} ;

class StreamFileTextWriterImpl implement StreamFileTextWriterLayout {
protected:
	using StreamFileTextWriterLayout::mStreamFile ;
	using StreamFileTextWriterLayout::mFileBuffer ;
	using StreamFileTextWriterLayout::mFileWriter ;

public:
	implicit StreamFileTextWriterImpl () = default ;

	implicit ~StreamFileTextWriterImpl () noexcept {
		flush () ;
	}

	void initialize (CREF<String<STR>> file) {
		mStreamFile = StreamFile (file) ;
		mStreamFile.open_w (0) ;
		mFileBuffer = RefBuffer<BYTE> (STREAMFILE_BUF_SIZE::expr) ;
		set_writer () ;
	}

	void set_writer () {
		mFileWriter = TextWriter (Ref<RefBuffer<BYTE>>::reference (mFileBuffer)) ;
		mFileWriter.use_overflow ([&] (VREF<TextWriter> writer) {
			mStreamFile.write (mFileBuffer) ;
			mFileWriter.reset () ;
		}) ;
	}

	void flush () {
		const auto r1x = mFileWriter.length () ;
		if (r1x == 0)
			return ;
		const auto r2x = FLAG (mFileBuffer.self) ;
		mStreamFile.write (RefBuffer<BYTE>::reference (r2x ,r1x)) ;
		mFileWriter.reset () ;
		mStreamFile.flush () ;
	}
} ;

class StreamFileTextWriterImplHolder final implement Fat<StreamFileTextWriterHolder ,AutoRef<StreamFileTextWriterLayout>> {
public:
	void initialize (CREF<String<STR>> file) override {
		fake = AutoRef<StreamFileTextWriterImpl>::make () ;
		ptr (fake).initialize (file) ;
	}

	static VREF<StreamFileTextWriterImpl> ptr (VREF<AutoRef<StreamFileTextWriterLayout>> that) {
		return keep[TYPE<StreamFileTextWriterImpl>::expr] (that.self) ;
	}

	VREF<TextWriter> self_m () leftvalue override {
		return fake->mFileWriter ;
	}

	void flush () override {
		return ptr (fake).flush () ;
	}
} ;

exports VFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (VREF<AutoRef<StreamFileTextWriterLayout>> that) {
	return VFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

exports CFat<StreamFileTextWriterHolder> StreamFileTextWriterHolder::hold (CREF<AutoRef<StreamFileTextWriterLayout>> that) {
	return CFat<StreamFileTextWriterHolder> (StreamFileTextWriterImplHolder () ,that) ;
}

template class External<BufferFileHolder ,AutoRef<BufferFileLayout>> ;

exports VFat<BufferFileHolder> BufferFileHolder::hold (VREF<AutoRef<BufferFileLayout>> that) {
	return VFat<BufferFileHolder> (External<BufferFileHolder ,AutoRef<BufferFileLayout>>::declare () ,that) ;
}

exports CFat<BufferFileHolder> BufferFileHolder::hold (CREF<AutoRef<BufferFileLayout>> that) {
	return CFat<BufferFileHolder> (External<BufferFileHolder ,AutoRef<BufferFileLayout>>::declare () ,that) ;
}

template class External<UartFileHolder ,AutoRef<UartFileLayout>> ;

exports VFat<UartFileHolder> UartFileHolder::hold (VREF<AutoRef<UartFileLayout>> that) {
	return VFat<UartFileHolder> (External<UartFileHolder ,AutoRef<UartFileLayout>>::declare () ,that) ;
}

exports CFat<UartFileHolder> UartFileHolder::hold (CREF<AutoRef<UartFileLayout>> that) {
	return CFat<UartFileHolder> (External<UartFileHolder ,AutoRef<UartFileLayout>>::declare () ,that) ;
}

template class External<ConsoleHolder ,SharedRef<ConsoleLayout>> ;

exports CREF<SharedRef<ConsoleLayout>> ConsoleHolder::instance () {
	return memorize ([&] () {
		SharedRef<ConsoleLayout> ret ;
		ConsoleHolder::hold (ret)->initialize () ;
		return move (ret) ;
	}) ;
}

exports VFat<ConsoleHolder> ConsoleHolder::hold (VREF<SharedRef<ConsoleLayout>> that) {
	return VFat<ConsoleHolder> (External<ConsoleHolder ,SharedRef<ConsoleLayout>>::declare () ,that) ;
}

exports CFat<ConsoleHolder> ConsoleHolder::hold (CREF<SharedRef<ConsoleLayout>> that) {
	return CFat<ConsoleHolder> (External<ConsoleHolder ,SharedRef<ConsoleLayout>>::declare () ,that) ;
}
} ;