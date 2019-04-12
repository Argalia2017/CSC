#pragma once

#ifndef __CSC_NETWORK__
#define __CSC_NETWORK__
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
class TCPSocket {
private:
	class Implement ;

public:
	class Listener ;

private:
	AnyRef<void> mThis ;

public:
	TCPSocket () = delete ;

	explicit TCPSocket (const String<STRU8> &addr) ;

	String<STRU8> sock_name () const ;

	String<STRU8> peer_sock_name () const ;

	void link (const String<STRU8> &addr) ;

	void modify_buffer (LENGTH rcv_len ,LENGTH snd_len) ;

	void read (const PhanBuffer<BYTE> &data) popping ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data) popping {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG>
	inline TCPSocket &operator>> (Buffer<BYTE ,_ARG> &data) popping {
		read (data) ;
		return *this ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &it ,LENGTH timeout) popping ;

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
} ;

class TCPSocket::Listener {
private:
	class Implement ;

private:
	AnyRef<void> mThis ;

public:
	Listener () = delete ;

	explicit Listener (const String<STRU8> &addr) ;

	void listen () ;

	void accept (TCPSocket &linker) ;
} ;

class UDPSocket {
private:
	class Implement ;

private:
	AnyRef<void> mThis ;

public:
	UDPSocket () = delete ;

	explicit UDPSocket (const String<STRU8> &addr) ;

	String<STRU8> sock_name () const ;

	String<STRU8> peer_sock_name () const ;

	void link (const String<STRU8> &addr) ;

	void read (const PhanBuffer<BYTE> &data) popping ;

	template <class _ARG>
	void read (Buffer<BYTE ,_ARG> &data) popping {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG>
	inline UDPSocket &operator>> (Buffer<BYTE ,_ARG> &data) popping {
		read (data) ;
		return *this ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &it ,LENGTH timeout) popping ;

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
} ;

class NetworkService final :private Interface {
public:
	static constexpr auto ADDR_NULL = CHAR (0X00000000) ;
	static constexpr auto ADDR_NONE = CHAR (0XFFFFFFFF) ;
	static constexpr auto ADDR_LOCALHOST = CHAR (0X7F000001) ;
	static constexpr auto ADDR_BROADCAST = CHAR (0XFFFFFFFF) ;

private:
	class Implement ;

	export struct Abstract :public Interface {
		virtual void startup () = 0 ;
		virtual void shutdown () = 0 ;
		virtual String<STRU8> host_name () const = 0 ;
		virtual String<STRU8> host_addr () const = 0 ;
	} ;

private:
	friend Singleton<NetworkService> ;
	Monostate<std::recursive_mutex> mMutex ;
	HolderRef<Abstract> mThis ;

public:
	void startup () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->startup () ;
	}

	void shutdown () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->shutdown () ;
	}

	String<STRU8> host_name () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->host_name () ;
	}

	String<STRU8> host_addr () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->host_addr () ;
	}

	String<STRU8> http_get (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) popping ;

	String<STRU8> http_post (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) popping ;

private:
	NetworkService () ;
} ;

inline String<STRU8> NetworkService::http_get (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) popping {
	startup () ;
	String<STRU8> ret = String<STRU8> (DEFAULT_HUGEBUFFER_SIZE::value) ;
	INDEX iw = 0 ;
	auto rax = TCPSocket (_PCSTRU8_ ("")) ;
	rax.link (addr) ;
	const auto r1x = _XVALUE_<const PTR<void (TextWriter<STRU8> &)> &> (_GAP_) ;
	const auto r2x = _PRINTS_<STRU8> (_PCSTRU8_ ("GET ") ,site ,_PCSTRU8_ ("?") ,msg ,_PCSTRU8_ (" HTTP/1.1") ,r1x ,_PCSTRU8_ ("HOST: ") ,addr ,r1x ,r1x) ;
	rax.write (PhanBuffer<const BYTE>::make (r2x.raw ())) ;
	rax.read (PhanBuffer<BYTE>::make (ret.raw ()) ,iw ,DEFAULT_TIMEOUT_SIZE::value) ;
	_DYNAMIC_ASSERT_ (iw >= 0 && iw < ret.size ()) ;
	ret[iw] = 0 ;
	return std::move (ret) ;
}

inline String<STRU8> NetworkService::http_post (const String<STRU8> &addr ,const String<STRU8> &site ,const String<STRU8> &msg) popping {
	startup () ;
	String<STRU8> ret = String<STRU8> (DEFAULT_HUGEBUFFER_SIZE::value) ;
	INDEX iw = 0 ;
	auto rax = TCPSocket (_PCSTRU8_ ("")) ;
	rax.link (addr) ;
	const auto r1x = _XVALUE_<const PTR<void (TextWriter<STRU8> &)> &> (_GAP_) ;
	const auto r2x = _PRINTS_<STRU8> (_PCSTRU8_ ("POST ") ,site ,_PCSTRU8_ (" HTTP/1.1") ,r1x ,_PCSTRU8_ ("HOST: ") ,addr ,r1x ,_PCSTRU8_ ("Content-Length: ") ,msg.length () ,r1x ,r1x ,msg) ;
	rax.write (PhanBuffer<const BYTE>::make (r2x.raw ())) ;
	rax.read (PhanBuffer<BYTE>::make (ret.raw ()) ,iw ,DEFAULT_TIMEOUT_SIZE::value) ;
	_DYNAMIC_ASSERT_ (iw >= 0 && iw < ret.size ()) ;
	ret[iw] = 0 ;
	return std::move (ret) ;
}
} ;