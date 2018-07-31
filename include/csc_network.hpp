#pragma once

#ifndef __CSC_NETWORK__
#define __CSC_NETWORK__
#endif

#include "csc.hpp"
#include "csc_ext.hpp"
#include "csc_array.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_thread.hpp"

namespace CSC {
class TCPSocket {
private:
	class Engine ;

public:
	class Listener ;

private:
	AnyRef<void> mHolder ;

public:
	TCPSocket () = default ;

	explicit TCPSocket (const String<STRU8> &addr) ;

	BOOL exist () const ;

	String<STRU8> host_name () const ;

	String<STRU8> peer_host_name () const ;

	void link (const String<STRU8> &addr) ;

	void link (const String<STRU8> &addr ,VAR timeout) ;

	void modify (LENGTH rcvlen ,LENGTH sndlen) ;

	void read (const PhanBuffer<BYTE> &data) ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data) {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG>
	inline TCPSocket &operator>> (Buffer<BYTE ,_ARG> &data) {
		read (data) ;
		return *this ;
	}

	void read (const PhanBuffer<BYTE> &data ,VAR timeout) ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data ,VAR timeout) {
		read (PhanBuffer<BYTE>::make (data) ,timeout) ;
	}

	void write (const PhanBuffer<const BYTE> &data) ;

	template <class _ARG>
	void write (const Buffer<BYTE ,_ARG> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	template <class _ARG>
	inline TCPSocket &operator<< (const Buffer<BYTE ,_ARG> &data) {
		write (data) ;
		return *this ;
	}

	void write (const PhanBuffer<const BYTE> &data ,VAR timeout) ;

	template <class _ARG>
	void write (const Buffer<BYTE ,_ARG> &data ,VAR timeout) {
		write (PhanBuffer<const BYTE>::make (data) ,timeout) ;
	}
} ;

class TCPSocket::Listener {
private:
	class Engine ;

private:
	AnyRef<void> mHolder ;

public:
	Listener () = default ;

	explicit Listener (const String<STRU8> &addr) ;

	BOOL exist () const ;

	void listen () ;

	void listen (VAR timeout) ;

	void accept (TCPSocket &linker) ;
} ;

class UDPSocket {
private:
	class Engine ;

private:
	AnyRef<void> mHolder ;

public:
	UDPSocket () = default ;

	explicit UDPSocket (const String<STRU8> &addr) ;

	BOOL exist () const ;

	String<STRU8> host_name () const ;

	String<STRU8> peer_host_name () const ;

	void link (const String<STRU8> &addr) ;

	void link (const String<STRU8> &addr ,VAR timeout) ;

	void read (const PhanBuffer<BYTE> &data) ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data) {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG>
	inline UDPSocket &operator>> (Buffer<BYTE ,_ARG> &data) {
		read (data) ;
		return *this ;
	}

	void read (const PhanBuffer<BYTE> &data ,VAR timeout) ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data ,VAR timeout) {
		read (PhanBuffer<BYTE>::make (data) ,timeout) ;
	}

	void write (const PhanBuffer<const BYTE> &data) ;

	template <class _ARG>
	void write (const Buffer<BYTE ,_ARG> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	template <class _ARG>
	inline UDPSocket &operator<< (const Buffer<BYTE ,_ARG> &data) {
		write (data) ;
		return *this ;
	}

	void write (const PhanBuffer<const BYTE> &data ,VAR timeout) ;

	template <class _ARG>
	void write (const Buffer<BYTE ,_ARG> &data ,VAR timeout) {
		write (PhanBuffer<const BYTE>::make (data) ,timeout) ;
	}
} ;

class NetworkService {
private:
	class Engine ;

private:
	friend Singleton<NetworkService> ;
	SharedRef<std::recursive_mutex> mMutex ;
	AnyRef<void> mHolder ;

public:
	void start () ;

	void stop () ;

	FLAG last_error () const ;

	String<STRA> local_host_name () const ;

	String<STRU8> http_get (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) ;

	String<STRU8> http_post (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) ;

private:
	NetworkService () ;

	~NetworkService () noexcept {}
} ;

inline String<STRU8> NetworkService::http_get (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) {
	String<STRU8> ret = String<STRU8> (HUGESTRING_SIZE::value) ;
	auto rax = TCPSocket (_PCSTRU8_ ("")) ;
	rax.link (addr) ;
	const auto r1x = _BUILDARGSS_<STRU8> (_PCSTRU8_ ("GET ") ,site ,_PCSTRU8_ ("?") ,msg ,_PCSTRU8_ (" HTTP/1.1\r\nHOST: ") ,addr ,_PCSTRU8_ ("\r\n\r\n")) ;
	rax.write (r1x.raw ()) ;
	_CALL_TRY_ ([&] () {
		rax.read (ret.raw ()) ;
	} ,[&] () {
		ret = String<STRU8> () ;
	}) ;
	return std::move (ret) ;
}

inline String<STRU8> NetworkService::http_post (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) {
	String<STRU8> ret = String<STRU8> (HUGESTRING_SIZE::value) ;
	auto rax = TCPSocket (_PCSTRU8_ ("")) ;
	rax.link (addr) ;
	const auto r1x = _BUILDARGSS_<STRU8> (_PCSTRU8_ ("POST ") ,site ,_PCSTRU8_ (" HTTP/1.1\r\nHOST: ") ,addr ,_PCSTRU8_ ("\r\nContent-Length: ") ,msg.length () ,_PCSTRU8_ ("\r\n\r\n") ,msg) ;
	rax.write (r1x.raw ()) ;
	_CALL_TRY_ ([&] () {
		rax.read (ret.raw ()) ;
	} ,[&] () {
		ret = String<STRU8> () ;
	}) ;
	return std::move (ret) ;
}
} ;