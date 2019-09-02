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
public:
	class Listener ;

private:
	class Implement ;
	AnyRef<void> mThis ;

public:
	TCPSocket () = delete ;

	explicit TCPSocket (const String<STRU8> &addr) ;

	String<STRU8> sock_name () const ;

	String<STRU8> peer_sock_name () const ;

	Listener listener () popping ;

	void link (const String<STRU8> &addr) ;

	void modify_buffer (LENGTH rcv_len ,LENGTH snd_len) ;

	void read (const PhanBuffer<BYTE> &data) popping ;

	template <class _ARG1>
	void read (Buffer<BYTE ,_ARG1> &data) popping {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline TCPSocket &operator>> (Buffer<BYTE ,_ARG1> &data) popping {
		read (data) ;
		return (*this) ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &out_i ,LENGTH timeout) popping ;

	void write (const PhanBuffer<const BYTE> &data) ;

	template <class _ARG1>
	void write (const Buffer<BYTE ,_ARG1> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline TCPSocket &operator<< (const Buffer<BYTE ,_ARG1> &data) {
		write (data) ;
		return (*this) ;
	}
} ;

class TCPSocket::Listener {
private:
	friend TCPSocket ;
	class Implement ;
	AnyRef<void> mThis ;

public:
	Listener () = delete ;

	void wait_linker () ;

	void accept () ;

private:
	explicit Listener (const AnyRef<void> &_socket) ;
} ;

inline TCPSocket::Listener TCPSocket::listener () popping {
	_DEBUG_ASSERT_ (mThis.exist ()) ;
	return TCPSocket::Listener (mThis) ;
}

class UDPSocket {
private:
	class Implement ;
	AnyRef<void> mThis ;

public:
	UDPSocket () = delete ;

	explicit UDPSocket (const String<STRU8> &addr) ;

	String<STRU8> sock_name () const ;

	String<STRU8> peer_sock_name () const ;

	void link (const String<STRU8> &addr) ;

	void read (const PhanBuffer<BYTE> &data) popping ;

	template <class _ARG1>
	void read (Buffer<BYTE ,_ARG1> &data) popping {
		read (PhanBuffer<BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline UDPSocket &operator>> (Buffer<BYTE ,_ARG1> &data) popping {
		read (data) ;
		return (*this) ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &out_i ,LENGTH timeout) popping ;

	void write (const PhanBuffer<const BYTE> &data) ;

	template <class _ARG1>
	void write (const Buffer<BYTE ,_ARG1> &data) {
		write (PhanBuffer<const BYTE>::make (data)) ;
	}

	template <class _ARG1>
	inline UDPSocket &operator<< (const Buffer<BYTE ,_ARG1> &data) {
		write (data) ;
		return (*this) ;
	}
} ;

class NetworkService final :private Interface {
private:
	exports struct Abstract :public Interface {
		virtual void startup () = 0 ;
		virtual void shutdown () = 0 ;
		virtual String<STRU8> localhost_name () const = 0 ;
		virtual String<STRU8> localhost_addr () const = 0 ;
		virtual String<STRU8> broadcast_addr () const = 0 ;
		virtual LENGTH get_timeout () const = 0 ;
		virtual void set_timeout (LENGTH timeout) = 0 ;
	} ;

private:
	class Implement ;
	friend Singleton<NetworkService> ;
	Monostate<std::recursive_mutex> mMutex ;
	StrongRef<Abstract> mThis ;

public:
	void startup () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->startup () ;
	}

	void shutdown () {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		mThis->shutdown () ;
	}

	String<STRU8> localhost_name () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->localhost_name () ;
	}

	String<STRU8> localhost_addr () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->localhost_addr () ;
	}

	String<STRU8> broadcast_addr () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->broadcast_addr () ;
	}

	LENGTH get_timeout () const {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->get_timeout () ;
	}

	void set_timeout (LENGTH timeout) {
		ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
		return mThis->set_timeout (timeout) ;
	}

private:
	NetworkService () ;
} ;
} ;