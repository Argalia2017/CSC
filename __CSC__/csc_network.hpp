#pragma once

#ifndef __CSC_NETWORK__
#define __CSC_NETWORK__
#endif

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_extend.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"
#include "csc_stream.hpp"
#include "csc_string.hpp"
#include "csc_runtime.hpp"
#include "csc_thread.hpp"

namespace CSC {
class TCPListener ;

class TCPSocket {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract
		:public Interface {
	public:
		virtual String<STRU8> sock_name () const = 0 ;
		virtual String<STRU8> peer_sock_name () const = 0 ;
		virtual void link (const String<STRU8> &ip_addr) = 0 ;
		virtual void modify_buffer (const LENGTH &rcv_len ,const LENGTH &snd_len) = 0 ;
		virtual void modify_timeout (const LENGTH &timeout) = 0 ;
		virtual void read (const PhanBuffer<BYTE> &data) = 0 ;
		virtual void read (const PhanBuffer<BYTE> &data ,INDEX &out_iw ,const LENGTH &timeout) = 0 ;
		virtual void write (const PhanBuffer<const BYTE> &data) = 0 ;
	} ;

private:
	friend TCPListener ;
	StrongRef<Abstract> mThis ;

public:
	implicit TCPSocket () = delete ;

	explicit TCPSocket (const String<STRU8> &ip_addr) ;

	String<STRU8> sock_name () const {
		return mThis->sock_name () ;
	}

	String<STRU8> peer_sock_name () const {
		return mThis->peer_sock_name () ;
	}

	template <class _RET = REMOVE_CVR_TYPE<TCPListener>>
	_RET listener () {
		struct Dependent ;
		using R1X = DEPENDENT_TYPE<TCPListener ,Dependent> ;
		_DEBUG_ASSERT_ (mThis.exist ()) ;
		return R1X (mThis) ;
	}

	void link (const String<STRU8> &ip_addr) {
		return mThis->link (ip_addr) ;
	}

	void modify_buffer (const LENGTH &rcv_len ,const LENGTH &snd_len) {
		return mThis->modify_buffer (rcv_len ,snd_len) ;
	}

	void modify_timeout (const LENGTH &timeout) {
		return mThis->modify_timeout (timeout) ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		return mThis->read (data) ;
	}

	template <class _ARG1>
	void read (Buffer<BYTE ,_ARG1> &data) {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline TCPSocket &operator>> (Buffer<BYTE ,_ARG1> &data) {
		read (data) ;
		return DEREF[this] ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &out_iw ,const LENGTH &timeout) {
		return mThis->read (data ,out_iw ,timeout) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		return mThis->write (data) ;
	}

	template <class _ARG1>
	void write (const Buffer<BYTE ,_ARG1> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline TCPSocket &operator<< (const Buffer<BYTE ,_ARG1> &data) {
		write (data) ;
		return DEREF[this] ;
	}

public:
	imports String<STRU8> http_get (const String<STRU8> &ip_addr ,const String<STRU8> &site ,const String<STRU8> &msg ,const LENGTH &buffer_len ,const LENGTH &timeout) ;

	imports String<STRU8> http_post (const String<STRU8> &ip_addr ,const String<STRU8> &site ,const String<STRU8> &msg ,const LENGTH &buffer_len ,const LENGTH &timeout) ;
} ;

class TCPListener {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract
		:public Interface {
	public:
		virtual void wait_linker () = 0 ;
		virtual void accept () = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit TCPListener () = delete ;

	explicit TCPListener (const StrongRef<TCPSocket::Private::Implement> &socket_) ;

	void wait_linker () {
		return mThis->wait_linker () ;
	}

	void accept () {
		return mThis->accept () ;
	}
} ;

class UDPSocket {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract
		:public Interface {
	public:
		virtual String<STRU8> sock_name () const = 0 ;
		virtual String<STRU8> peer_sock_name () const = 0 ;
		virtual void link (const String<STRU8> &ip_addr) = 0 ;
		virtual void modify_timeout (const LENGTH &timeout) = 0 ;
		virtual void read (const PhanBuffer<BYTE> &data) = 0 ;
		virtual void read (const PhanBuffer<BYTE> &data ,INDEX &out_iw ,const LENGTH &timeout) = 0 ;
		virtual void write (const PhanBuffer<const BYTE> &data) = 0 ;
	} ;

private:
	StrongRef<Abstract> mThis ;

public:
	implicit UDPSocket () = delete ;

	explicit UDPSocket (const String<STRU8> &ip_addr) ;

	String<STRU8> sock_name () const {
		return mThis->sock_name () ;
	}

	String<STRU8> peer_sock_name () const {
		return mThis->peer_sock_name () ;
	}

	void link (const String<STRU8> &ip_addr) {
		return mThis->link (ip_addr) ;
	}

	void modify_timeout (const LENGTH &timeout) {
		return mThis->modify_timeout (timeout) ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		return mThis->read (data) ;
	}

	template <class _ARG1>
	void read (Buffer<BYTE ,_ARG1> &data) {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline UDPSocket &operator>> (Buffer<BYTE ,_ARG1> &data) {
		read (data) ;
		return DEREF[this] ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &out_iw ,const LENGTH &timeout) {
		return mThis->read (data ,out_iw ,timeout) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		return mThis->write (data) ;
	}

	template <class _ARG1>
	void write (const Buffer<BYTE ,_ARG1> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline UDPSocket &operator<< (const Buffer<BYTE ,_ARG1> &data) {
		write (data) ;
		return DEREF[this] ;
	}
} ;

class NetworkService
	:private Proxy {
private:
	struct Private {
		class Implement ;
	} ;

	class Abstract
		:public Interface {
	public:
		virtual void startup () = 0 ;
		virtual void shutdown () = 0 ;
		virtual String<STRU8> localhost_name () const = 0 ;
		virtual String<STRU8> localhost_addr () const = 0 ;
		virtual String<STRU8> broadcast_addr () const = 0 ;
		virtual LENGTH pref_timeout () const = 0 ;
	} ;

private:
	friend Singleton<NetworkService> ;
	Monostate<RecursiveMutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	implicit NetworkService () = delete ;

	void startup () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->startup () ;
	}

	void shutdown () {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->shutdown () ;
	}

	String<STRU8> localhost_name () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->localhost_name () ;
	}

	String<STRU8> localhost_addr () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->localhost_addr () ;
	}

	String<STRU8> broadcast_addr () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->broadcast_addr () ;
	}

	LENGTH pref_timeout () const {
		ScopedGuard<RecursiveMutex> ANONYMOUS (mMutex) ;
		return mThis->pref_timeout () ;
	}

private:
	explicit NetworkService (const ARGVF<Singleton<NetworkService>> &) ;
} ;
} ;