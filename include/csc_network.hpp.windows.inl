#pragma once

#ifndef __CSC_NETWORK__
#error "∑(っ°Д° ;)っ : required csc_network.hpp"
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : required Windows.h"
#endif

#if !defined (_WINSOCK2API_) && defined (_WINSOCKAPI_)
#error "∑(っ°Д° ;)っ : required WIN32_LEAN_AND_MEAN before Windows.h"
#endif

#ifndef __CSC_STRICTHPPINL__
#include <winsock2.h>
#endif

#ifndef _WINSOCKAPI_
#error "∑(っ°Д° ;)っ : required winsock.h"
#endif

namespace CSC {
inline namespace NETWORK {
inline TIMEVAL _MAKETIMEVAL_ (VAR arg) {
	return {VAR32 (arg / 1000) ,VAR32 (arg % 1000 * 1000)} ;
}

inline IN_ADDR _MAKEINADDR_ (CHAR arg) {
	IN_ADDR ret = {0} ;
	auto &r1 = _CAST_<BYTE[4]> (ret.S_un.S_addr) ;
	auto &r2 = _CAST_<EndianBytes<CHAR>> (arg) ;
	r1[0] = r2[0] ;
	r1[1] = r2[1] ;
	r1[2] = r2[2] ;
	r1[3] = r2[3] ;
	return std::move (ret) ;
}

inline SOCKADDR_IN _MAKESOCKADDR_ (const String<STRU8> &arg) {
	const auto r1x = _PARSEIPV4S_ (arg) ;
	SOCKADDR_IN ret = {0} ;
	ret.sin_family = AF_INET ;
	auto &r2 = _CAST_<BYTE[2]> (ret.sin_port) ;
	auto &r3 = _CAST_<EndianBytes<WORD>> (r1x.P1) ;
	r2[0] = r3[0] ;
	r2[1] = r3[1] ;
	ret.sin_addr = _MAKEINADDR_ (r1x.P2) ;
	return std::move (ret) ;
}

inline String<STRU8> _MAKEIPV4S_ (const SOCKADDR_IN &arg) {
	const auto r1x = _CALL_ ([&] () {
		PACK<WORD ,CHAR> ret = {0} ;
		auto &r2 = _CAST_<EndianBytes<WORD>> (ret.P1) ;
		auto &r3 = _CAST_<BYTE[2]> (arg.sin_port) ;
		r2[0] = r3[0] ;
		r2[1] = r3[1] ;
		auto &r4 = _CAST_<EndianBytes<CHAR>> (ret.P2) ;
		auto &r5 = _CAST_<BYTE[4]> (arg.sin_addr.S_un.S_addr) ;
		r4[0] = r5[0] ;
		r4[1] = r5[1] ;
		r4[2] = r5[2] ;
		r4[3] = r5[3] ;
		return std::move (ret) ;
	}) ;
	return _BUILDIPV4S_<STRU8> (r1x) ;
}
} ;

class TCPSocket::Engine :public Interface {
private:
	friend TCPSocket::Listener ;
	UniqueRef<SOCKET> mSocket ;
	SOCKADDR_IN mPeer ;

public:
	Engine () = delete ;

	explicit Engine (const String<STRU8> &addr) {
		mSocket = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = INVALID_SOCKET ;
			me = ::socket (AF_INET ,SOCK_STREAM ,IPPROTO_TCP) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
		} ,[] (SOCKET &me) {
			if (me == INVALID_SOCKET)
				return ;
			::closesocket (me) ;
			me = INVALID_SOCKET ;
		}) ;
		if (!addr.empty ())
			socket_bind (mSocket ,addr) ;
		_ZERO_ (mPeer) ;
	}

	BOOL exist () const {
		return mSocket.exist () ;
	}

	String<STRU8> host_name () const {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = _CALL_ ([&] () {
			PACK<SOCKADDR_IN ,VAR32> ret ;
			ret.P1 = {0} ;
			ret.P2 = VAR32 (_SIZEOF_ (SOCKADDR_IN)) ;
			::getsockname (mSocket ,&_CAST_<SOCKADDR> (ret.P1) ,&ret.P2) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r1x.P2 == _SIZEOF_ (SOCKADDR_IN)) ;
		return _MAKEIPV4S_ (r1x.P1) ;
	}

	String<STRU8> peer_host_name () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return _MAKEIPV4S_ (mPeer) ;
	}

	void link (const String<STRU8> &addr) {
		_DEBUG_ASSERT_ (exist ()) ;
		mPeer = _MAKESOCKADDR_ (addr) ;
		const auto r1x = ::connect (mSocket ,&_CAST_<SOCKADDR> (mPeer) ,VAR32 (_SIZEOF_ (mPeer))) ;
		_DYNAMIC_ASSERT_ (r1x != SOCKET_ERROR) ;
	}

	void link (const String<STRU8> &addr ,VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		mPeer = _MAKESOCKADDR_ (addr) ;
		::ioctlsocket (mSocket ,FIONBIO ,&_XVALUE_ (ULONG (1))) ;
		const auto r1x = ::connect (mSocket ,&_CAST_<SOCKADDR> (mPeer) ,VAR32 (_SIZEOF_ (mPeer))) ;
		::ioctlsocket (mSocket ,FIONBIO ,&_XVALUE_ (ULONG (0))) ;
		if (r1x == 0)
			return ;
		_DYNAMIC_ASSERT_ (r1x >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
		const auto r2x = socket_select (mSocket ,timeout) ;
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r2x[1]) != 0) ;
		const auto r3x = _CALL_ ([&] () {
			VAR32 ret = 0 ;
			::getsockopt (mSocket ,SOL_SOCKET ,SO_ERROR ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (ret) ,&_XVALUE_ (VAR32 (_SIZEOF_ (VAR32)))) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r3x == 0) ;
	}

	void modify (LENGTH rcvlen ,LENGTH sndlen) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (rcvlen >= 0 && rcvlen <= VAR32_MAX && sndlen >= 0 && sndlen <= VAR32_MAX) ;
		const auto r1x = VAR32 (rcvlen) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVBUF ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r1x) ,VAR32 (_SIZEOF_ (VAR32))) ;
		const auto r2x = VAR32 (sndlen) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_SNDBUF ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r2x) ,VAR32 (_SIZEOF_ (VAR32))) ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = ::recv (mSocket ,_LOAD_<ARR<STRA>> (&data.self) ,VAR32 (data.size ()) ,0) ;
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void read (const PhanBuffer<BYTE> &data ,VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		const auto r1x = _MAKETIMEVAL_ (timeout) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r1x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		const auto r2x = ::recv (mSocket ,_LOAD_<ARR<STRA>> (&data.self) ,VAR32 (data.size ()) ,0) ;
		const auto r3x = TIMEVAL {0} ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r3x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		_DYNAMIC_ASSERT_ (r2x >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
		_DYNAMIC_ASSERT_ (r2x > 0 && r2x == data.size ()) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = ::send (mSocket ,_LOAD_<ARR<STRA>> (&data.self) ,VAR32 (data.size ()) ,0) ;
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void write (const PhanBuffer<const BYTE> &data ,VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		const auto r1x = _MAKETIMEVAL_ (timeout) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_SNDTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r1x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		const auto r2x = ::send (mSocket ,_LOAD_<ARR<STRA>> (&data.self) ,VAR32 (data.size ()) ,0) ;
		const auto r3x = TIMEVAL {0} ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_SNDTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r3x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		_DYNAMIC_ASSERT_ (r2x >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
		_DYNAMIC_ASSERT_ (r2x > 0 && r2x == data.size ()) ;
	}

private:
	void accept (UniqueRef<SOCKET> &&socket) {
		mSocket = std::move (socket) ;
		const auto r1x = _CALL_ ([&] () {
			PACK<SOCKADDR_IN ,VAR32> ret ;
			ret.P1 = {0} ;
			ret.P2 = VAR32 (_SIZEOF_ (SOCKADDR_IN)) ;
			::getpeername (mSocket ,&_CAST_<SOCKADDR> (ret.P1) ,&ret.P2) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r1x.P2 == _SIZEOF_ (SOCKADDR_IN)) ;
		mPeer = r1x.P1 ;
	}

private:
	static SOCKADDR_IN socket_bind (const SOCKET &socket ,const String<STRU8> &addr) {
		SOCKADDR_IN ret = _MAKESOCKADDR_ (addr) ;
		const auto r1x = ::bind (socket ,&_CAST_<SOCKADDR> (ret) ,VAR32 (_SIZEOF_ (ret))) ;
		_DYNAMIC_ASSERT_ (r1x != SOCKET_ERROR) ;
		return std::move (ret) ;
	}

	static ARRAY2<fd_set> socket_select (const SOCKET &socket ,VAR timeout) {
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4548) //@info:warning C4548: 逗号前的表达式不起任何作用；应输入带副作用的表达式
#endif
		ARRAY2<fd_set> ret ;
		FD_ZERO (&ret[0]) ;
		FD_SET (socket ,&ret[0]) ;
		ret[1] = ret[0] ;
		auto rax = _MAKETIMEVAL_ (timeout) ;
		while (::select (FD_SETSIZE ,&ret[0] ,&ret[1] ,NULL ,&rax) < 0)
			(void) rax ;
		return std::move (ret) ;
#pragma warning (pop)
	}
} ;

inline export TCPSocket::TCPSocket (const String<STRU8> &addr) {
	mHolder = AnyRef<Engine>::make (addr) ;
}

inline export BOOL TCPSocket::exist () const {
	return mHolder.exist () && mHolder.rebind<Engine> ()->exist () ;
}

inline export String<STRU8> TCPSocket::host_name () const {
	return mHolder.rebind<Engine> ()->host_name () ;
}

inline export String<STRU8> TCPSocket::peer_host_name () const {
	return mHolder.rebind<Engine> ()->peer_host_name () ;
}

inline export void TCPSocket::link (const String<STRU8> &addr) {
	mHolder.rebind<Engine> ()->link (addr) ;
}

inline export void TCPSocket::link (const String<STRU8> &addr ,VAR timeout) {
	mHolder.rebind<Engine> ()->link (addr ,timeout) ;
}

inline export void TCPSocket::modify (LENGTH rcvlen ,LENGTH sndlen) {
	mHolder.rebind<Engine> ()->modify (rcvlen ,sndlen) ;
}

inline export void TCPSocket::read (const PhanBuffer<BYTE> &data) {
	mHolder.rebind<Engine> ()->read (data) ;
}

inline export void TCPSocket::read (const PhanBuffer<BYTE> &data ,VAR timeout) {
	mHolder.rebind<Engine> ()->read (data ,timeout) ;
}

inline export void TCPSocket::write (const PhanBuffer<const BYTE> &data) {
	mHolder.rebind<Engine> ()->write (data) ;
}

inline export void TCPSocket::write (const PhanBuffer<const BYTE> &data ,VAR timeout) {
	mHolder.rebind<Engine> ()->write (data ,timeout) ;
}

class TCPSocket::Listener::Engine :public Interface {
private:
	UniqueRef<SOCKET> mListener ;
	UniqueRef<SOCKET> mLinker ;

public:
	Engine () = delete ;

	explicit Engine (const String<STRU8> &addr) {
		mListener = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = INVALID_SOCKET ;
			me = ::socket (AF_INET ,SOCK_STREAM ,IPPROTO_TCP) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
		} ,[] (SOCKET &me) {
			if (me == INVALID_SOCKET)
				return ;
			::closesocket (me) ;
			me = INVALID_SOCKET ;
		}) ;
		_DYNAMIC_ASSERT_ (!addr.empty ()) ;
		TCPSocket::Engine::socket_bind (mListener ,addr) ;
		const auto r1x = ::listen (mListener ,5) ;
		_DYNAMIC_ASSERT_ (r1x != SOCKET_ERROR) ;
	}

	BOOL exist () const {
		return mListener.exist () ;
	}

	void listen () {
		_DEBUG_ASSERT_ (exist ()) ;
		mLinker = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = INVALID_SOCKET ;
			me = ::accept (mListener ,NULL ,NULL) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
		} ,[] (SOCKET &me) {
			if (me == INVALID_SOCKET)
				return ;
			::closesocket (me) ;
			me = INVALID_SOCKET ;
		}) ;
	}

	void listen (VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		const auto r1x = TCPSocket::Engine::socket_select (mListener ,timeout) ;
		_DYNAMIC_ASSERT_ (FD_ISSET (mListener ,&r1x[0]) != 0) ;
		listen () ;
	}

	void accept (TCPSocket &linker) {
		_DEBUG_ASSERT_ (exist ()) ;
		linker.mHolder.rebind<TCPSocket::Engine> ()->accept (std::move (mLinker)) ;
	}
} ;

inline export TCPSocket::Listener::Listener (const String<STRU8> &addr) {
	mHolder = AnyRef<Engine>::make (addr) ;
}

inline export BOOL TCPSocket::Listener::exist () const {
	return mHolder.exist () && mHolder.rebind<Engine> ()->exist () ;
}

inline export void TCPSocket::Listener::listen () {
	mHolder.rebind<Engine> ()->listen () ;
}

inline export void TCPSocket::Listener::listen (VAR timeout) {
	mHolder.rebind<Engine> ()->listen (timeout) ;
}

inline export void TCPSocket::Listener::accept (TCPSocket &linker) {
	mHolder.rebind<Engine> ()->accept (linker) ;
}

class UDPSocket::Engine :public Interface {
private:
	UniqueRef<SOCKET> mSocket ;
	SOCKADDR_IN mPeer ;

public:
	Engine () = delete ;

	explicit Engine (const String<STRU8> &addr) {
		mSocket = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = INVALID_SOCKET ;
			me = ::socket (AF_INET ,SOCK_DGRAM ,IPPROTO_UDP) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
			const auto r1x = VAR32 (1) ;
			::setsockopt (me ,SOL_SOCKET ,SO_REUSEADDR ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r1x) ,VAR32 (_SIZEOF_ (VAR32))) ;
			::setsockopt (me ,SOL_SOCKET ,SO_BROADCAST ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r1x) ,VAR32 (_SIZEOF_ (VAR32))) ;
		} ,[] (SOCKET &me) {
			if (me == INVALID_SOCKET)
				return ;
			::closesocket (me) ;
			me = INVALID_SOCKET ;
		}) ;
		if (!addr.empty ())
			socket_bind (mSocket ,addr) ;
		_ZERO_ (mPeer) ;
	}

	BOOL exist () const {
		return mSocket.exist () ;
	}

	String<STRU8> host_name () const {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = _CALL_ ([&] () {
			PACK<SOCKADDR_IN ,VAR32> ret ;
			ret.P1 = {0} ;
			ret.P2 = VAR32 (_SIZEOF_ (SOCKADDR_IN)) ;
			::getsockname (mSocket ,&_CAST_<SOCKADDR> (ret.P1) ,&ret.P2) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r1x.P2 == _SIZEOF_ (SOCKADDR_IN)) ;
		return _MAKEIPV4S_ (r1x.P1) ;
	}

	String<STRU8> peer_host_name () const {
		_DEBUG_ASSERT_ (exist ()) ;
		return _MAKEIPV4S_ (mPeer) ;
	}

	void link (const String<STRU8> &addr) {
		_DEBUG_ASSERT_ (exist ()) ;
		mPeer = _MAKESOCKADDR_ (addr) ;
	}

	void link (const String<STRU8> &addr ,VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		link (addr) ;
	}

	void read (const PhanBuffer<BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = _CALL_ ([&] () {
			PACK<SOCKADDR_IN ,VAR32 ,VAR32> ret ;
			ret.P1 = {0} ;
			ret.P2 = VAR32 (_SIZEOF_ (SOCKADDR_IN)) ;
			ret.P3 = ::recvfrom (mSocket ,_LOAD_<ARR<STRA>> (&data.self) ,VAR32 (data.size ()) ,0 ,&_CAST_<SOCKADDR> (ret.P1) ,&ret.P2) ;
			return std::move (ret) ;
		}) ;
		_DYNAMIC_ASSERT_ (r1x.P3 == data.size ()) ;
		_DYNAMIC_ASSERT_ (r1x.P2 == _SIZEOF_ (SOCKADDR_IN)) ;
		mPeer = r1x.P1 ;
	}

	void read (const PhanBuffer<BYTE> &data ,VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		const auto r1x = socket_select (mSocket ,timeout) ;
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r1x[0]) != 0) ;
		read (data) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		_DEBUG_ASSERT_ (exist ()) ;
		const auto r1x = ::sendto (mSocket ,_LOAD_<ARR<STRA>> (&data.self) ,VAR32 (data.size ()) ,0 ,&_CAST_<SOCKADDR> (mPeer) ,VAR32 (_SIZEOF_ (mPeer))) ;
		_DYNAMIC_ASSERT_ (r1x == data.size ()) ;
	}

	void write (const PhanBuffer<const BYTE> &data ,VAR timeout) {
		_DEBUG_ASSERT_ (exist ()) ;
		_DEBUG_ASSERT_ (timeout >= 0 && timeout <= VAR32_MAX) ;
		const auto r1x = socket_select (mSocket ,timeout) ;
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r1x[1]) != 0) ;
		write (data) ;
	}

private:
	static SOCKADDR_IN socket_bind (const SOCKET &socket ,const String<STRU8> &addr) {
		SOCKADDR_IN ret = _MAKESOCKADDR_ (addr) ;
		const auto r1x = ::bind (socket ,&_CAST_<SOCKADDR> (ret) ,VAR32 (_SIZEOF_ (ret))) ;
		_DYNAMIC_ASSERT_ (r1x != SOCKET_ERROR) ;
		return std::move (ret) ;
	}

	static ARRAY2<fd_set> socket_select (const SOCKET &socket ,VAR timeout) {
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4548) //@info:warning C4548: 逗号前的表达式不起任何作用；应输入带副作用的表达式
#endif
		ARRAY2<fd_set> ret ;
		FD_ZERO (&ret[0]) ;
		FD_SET (socket ,&ret[0]) ;
		ret[1] = ret[0] ;
		auto rax = _MAKETIMEVAL_ (timeout) ;
		while (::select (FD_SETSIZE ,&ret[0] ,&ret[1] ,NULL ,&rax) < 0)
			(void) rax ;
		return std::move (ret) ;
#pragma warning (pop)
	}
} ;

inline export UDPSocket::UDPSocket (const String<STRU8> &addr) {
	mHolder = AnyRef<Engine>::make (addr) ;
}

inline export BOOL UDPSocket::exist () const {
	return mHolder.exist () && mHolder.rebind<Engine> ()->exist () ;
}

inline export String<STRU8> UDPSocket::host_name () const {
	return mHolder.rebind<Engine> ()->host_name () ;
}

inline export String<STRU8> UDPSocket::peer_host_name () const {
	return mHolder.rebind<Engine> ()->peer_host_name () ;
}

inline export void UDPSocket::link (const String<STRU8> &addr) {
	mHolder.rebind<Engine> ()->link (addr) ;
}

inline export void UDPSocket::link (const String<STRU8> &addr ,VAR timeout) {
	mHolder.rebind<Engine> ()->link (addr ,timeout) ;
}

inline export void UDPSocket::read (const PhanBuffer<BYTE> &data) {
	mHolder.rebind<Engine> ()->read (data) ;
}

inline export void UDPSocket::read (const PhanBuffer<BYTE> &data ,VAR timeout) {
	mHolder.rebind<Engine> ()->read (data ,timeout) ;
}

inline export void UDPSocket::write (const PhanBuffer<const BYTE> &data) {
	mHolder.rebind<Engine> ()->write (data) ;
}

inline export void UDPSocket::write (const PhanBuffer<const BYTE> &data ,VAR timeout) {
	mHolder.rebind<Engine> ()->write (data ,timeout) ;
}

class NetworkService::Engine :public Interface {
public:
	static constexpr CHAR M_ADDR_ANY = INADDR_ANY ;
	static constexpr CHAR M_ADDR_LOCALHOST = INADDR_LOOPBACK ;
	static constexpr CHAR M_ADDR_BROADCAST = INADDR_BROADCAST ;
	static constexpr CHAR M_ADDR_NONE = INADDR_NONE ;

private:
	UniqueRef<void> mService ;

public:
	Engine () = default ;

	void start () {
		if (mService.exist ())
			return ;
		mService = UniqueRef<void> ([&] () {
			const auto r1x = (WORD (2) << (_SIZEOF_ (BYTE) * 8)) | WORD (2) ;
			const auto r2x = WSAStartup (r1x ,&_XVALUE_ (WSADATA {0})) ;
			_DYNAMIC_ASSERT_ (r2x == 0) ;
		} ,[] () {
			WSACleanup () ;
		}) ;
	}

	void stop () {
		mService = UniqueRef<void> () ;
	}

	FLAG last_error () const {
		return WSAGetLastError () ;
	}

	String<STRA> local_host_name () const {
		String<STRA> ret = String<STRA> (127) ;
		::gethostname (ret.raw () ,VAR32 (ret.size ())) ;
		return std::move (ret) ;
	}
} ;

inline export void NetworkService::start () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->start () ;
}

inline export void NetworkService::stop () {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	mHolder.rebind<Engine> ()->stop () ;
}

inline export FLAG NetworkService::last_error () const {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	return mHolder.rebind<Engine> ()->last_error () ;
}

inline export String<STRA> NetworkService::local_host_name () const {
	ScopedGuard<std::recursive_mutex> ANONYMOUS (mMutex) ;
	return mHolder.rebind<Engine> ()->local_host_name () ;
}

inline export NetworkService::NetworkService () {
	mMutex = SharedRef<std::recursive_mutex>::make () ;
	mHolder = AnyRef<Engine>::make () ;
}
} ;
