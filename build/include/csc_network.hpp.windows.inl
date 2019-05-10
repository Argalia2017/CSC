#pragma once

#ifndef __CSC_NETWORK__
#error "∑(っ°Д° ;)っ : require 'csc_network.hpp'"
#endif

#ifdef __CSC__
#undef self
#undef implicit
#undef popping
#undef imports
#undef exports
#pragma pop_macro ("self")
#pragma pop_macro ("implicit")
#pragma pop_macro ("popping")
#pragma pop_macro ("imports")
#pragma pop_macro ("exports")
#endif

#ifndef _INC_WINDOWS
#error "∑(っ°Д° ;)っ : require 'Windows.h'"
#endif

#ifdef _WINSOCKAPI_
#ifndef _WINSOCK2API_
#error "∑(っ°Д° ;)っ : require 'WIN32_LEAN_AND_MEAN' before 'Windows.h'"
#endif
#endif

#ifdef __CSC_DEPRECATED__
#include <winsock2.h>
#endif

#ifndef _WINSOCKAPI_
#error "∑(っ°Д° ;)っ : require 'winsock.h'"
#endif

#ifdef __CSC_DEPRECATED__
#ifdef __CSC_COMPILER_MSVC__
#ifndef use_comment_lib_ws2_32
#define use_comment_lib_ws2_32 "ws2_32.lib"
#endif
#pragma comment (lib ,use_comment_lib_ws2_32)
#undef use_comment_lib_ws2_32
#endif
#endif

#ifdef __CSC__
#pragma push_macro ("self")
#pragma push_macro ("implicit")
#pragma push_macro ("popping")
#pragma push_macro ("imports")
#pragma push_macro ("exports")
#define self to ()
#define implicit
#define popping
#define imports extern
#define exports
#endif

namespace CSC {
inline namespace NETWORK {
inline TIMEVAL _inline_SOCKET_CVTTO_TIMEVAL_ (LENGTH src) {
	_DEBUG_ASSERT_ (src >= 0) ;
	return TIMEVAL {VAR32 (src / 1000) ,VAR32 ((src % 1000) * 1000)} ;
}

inline String<STRU8> _inline_SOCKET_CVTTO_IPV4S_ (const SOCKADDR &src) {
	_STATIC_ASSERT_ (_SIZEOF_ (SOCKADDR) == _SIZEOF_ (SOCKADDR_IN)) ;
	const auto r1x = _CALL_ ([&] () {
		TEMP<SOCKADDR_IN> ret ;
		_MEMCOPY_ (PTRTOARR[&_ZERO_ (ret).unused[0]] ,_CAST_<BYTE[_SIZEOF_ (SOCKADDR)]> (src)) ;
		return std::move (_CAST_<SOCKADDR_IN> (ret)) ;
	}) ;
	const auto r2x = _CALL_ ([&] () {
		PACK<WORD ,CHAR> ret ;
		ret.P1 = _CAST_<EndianBytes<WORD>> (r1x.sin_port).merge () ;
		ret.P2 = _CAST_<EndianBytes<CHAR>> (r1x.sin_addr.S_un.S_addr).merge () ;
		return std::move (ret) ;
	}) ;
	return _BUILDIPV4S_<STRU8> (r2x) ;
}

inline SOCKADDR _inline_SOCKET_CVTTO_SOCKETADDR_ (const String<STRU8> &src) {
	_STATIC_ASSERT_ (_SIZEOF_ (SOCKADDR) == _SIZEOF_ (SOCKADDR_IN)) ;
	TEMP<SOCKADDR> ret ;
	const auto r1x = _CALL_ ([&] () {
		SOCKADDR_IN ret ;
		_ZERO_ (ret) ;
		ret.sin_family = AF_INET ;
		const auto r2x = _PARSEIPV4S_ (src) ;
		ret.sin_port = _CAST_<EndianBytes<WORD>> (r2x.P1).merge () ;
		ret.sin_addr.S_un.S_addr = _CAST_<EndianBytes<CHAR>> (r2x.P2).merge () ;
		return std::move (ret) ;
	}) ;
	_MEMCOPY_ (PTRTOARR[&_ZERO_ (ret).unused[0]] ,_CAST_<BYTE[_SIZEOF_ (SOCKADDR)]> (r1x)) ;
	return std::move (_CAST_<SOCKADDR> (ret)) ;
}

inline void _inline_SOCKET_BIND_ (const SOCKET &socket ,const String<STRU8> &addr) {
	const auto r1x = _inline_SOCKET_CVTTO_SOCKETADDR_ (addr) ;
	const auto r2x = ::bind (socket ,&r1x ,VAR32 (_SIZEOF_ (SOCKADDR))) ;
	_DYNAMIC_ASSERT_ (r2x != SOCKET_ERROR) ;
}

inline ARRAY2<fd_set> _inline_SOCKET_SELECT_ (const SOCKET &socket ,LENGTH timeout) {
#pragma warning (push)
#ifdef __CSC_COMPILER_MSVC__
#pragma warning (disable :4548) //@info: warning C4548: 逗号前的表达式不起任何作用；应输入带副作用的表达式
#endif
	ARRAY2<fd_set> ret ;
	FD_ZERO (&ret[0]) ;
	FD_ZERO (&ret[1]) ;
	FD_SET (socket ,&ret[0]) ;
	FD_SET (socket ,&ret[1]) ;
	auto rax = _inline_SOCKET_CVTTO_TIMEVAL_ (timeout) ;
	while (::select (FD_SETSIZE ,&ret[0] ,&ret[1] ,NULL ,&rax) < 0)
		(void) rax ;
	_ZERO_ (rax) ;
	return std::move (ret) ;
#pragma warning (pop)
}
} ;

class TCPSocket::Implement final :private Interface {
private:
	friend TCPSocket::Listener ;
	UniqueRef<SOCKET> mSocket ;
	SOCKADDR mPeer ;

public:
	Implement () = delete ;

	explicit Implement (const String<STRU8> &addr) {
		mSocket = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = ::socket (AF_INET ,SOCK_STREAM ,IPPROTO_TCP) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
		} ,[] (SOCKET &me) {
			_DEBUG_ASSERT_ (me != INVALID_SOCKET) ;
			::closesocket (me) ;
		}) ;
		if (!addr.empty ())
			_inline_SOCKET_BIND_ (mSocket ,addr) ;
		_ZERO_ (mPeer) ;
	}

	String<STRU8> sock_name () const {
		auto rax = PACK<SOCKADDR ,VAR32> () ;
		::getsockname (mSocket ,&_ZERO_ (rax.P1) ,&(rax.P2 = VAR32 (_SIZEOF_ (SOCKADDR)))) ;
		//@info: ipv6 is not supported
		_DYNAMIC_ASSERT_ (rax.P2 == _SIZEOF_ (SOCKADDR)) ;
		return _inline_SOCKET_CVTTO_IPV4S_ (rax.P1) ;
	}

	String<STRU8> peer_sock_name () const {
		return _inline_SOCKET_CVTTO_IPV4S_ (mPeer) ;
	}

	void link (const String<STRU8> &addr) {
		mPeer = _inline_SOCKET_CVTTO_SOCKETADDR_ (addr) ;
		auto rax = ULONG () ;
		::ioctlsocket (mSocket ,FIONBIO ,&(rax = ULONG (1))) ;
		const auto r1x = ::connect (mSocket ,&mPeer ,VAR32 (_SIZEOF_ (SOCKADDR))) ;
		::ioctlsocket (mSocket ,FIONBIO ,&(rax = ULONG (0))) ;
		if (r1x == 0)
			return ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r1x >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
		link_confirm () ;
	}

	void modify_buffer (LENGTH rcv_len ,LENGTH snd_len) {
		_DEBUG_ASSERT_ (rcv_len >= 0 && rcv_len < VAR32_MAX) ;
		_DEBUG_ASSERT_ (snd_len >= 0 && snd_len < VAR32_MAX) ;
		const auto r1x = VAR32 (rcv_len) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVBUF ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r1x) ,VAR32 (_SIZEOF_ (VAR32))) ;
		const auto r2x = VAR32 (snd_len) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_SNDBUF ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r2x) ,VAR32 (_SIZEOF_ (VAR32))) ;
	}

	void read (const PhanBuffer<BYTE> &data) popping {
		const auto r1x = _inline_SOCKET_CVTTO_TIMEVAL_ (DEFAULT_TIMEOUT_SIZE::value) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r1x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		const auto r2x = ::recv (mSocket ,_LOAD_<ARR<STRA>> (data.self) ,VAR32 (data.size ()) ,0) ;
		const auto r3x = _inline_SOCKET_CVTTO_TIMEVAL_ (0) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r3x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
		_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &it ,LENGTH timeout) popping {
		const auto r1x = _inline_SOCKET_CVTTO_TIMEVAL_ (timeout) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r1x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		it = ::recv (mSocket ,_LOAD_<ARR<STRA>> (data.self) ,VAR32 (data.size ()) ,0) ;
		const auto r2x = _inline_SOCKET_CVTTO_TIMEVAL_ (0) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_RCVTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r2x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (it >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		const auto r1x = _inline_SOCKET_CVTTO_TIMEVAL_ (DEFAULT_TIMEOUT_SIZE::value) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_SNDTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r1x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		const auto r2x = ::send (mSocket ,_LOAD_<ARR<STRA>> (data.self) ,VAR32 (data.size ()) ,0) ;
		const auto r3x = _inline_SOCKET_CVTTO_TIMEVAL_ (0) ;
		::setsockopt (mSocket ,SOL_SOCKET ,SO_SNDTIMEO ,_CAST_<STRA[_SIZEOF_ (TIMEVAL)]> (r3x) ,VAR32 (_SIZEOF_ (TIMEVAL))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x >= 0 || errno == 0 || errno == EINPROGRESS || errno == EWOULDBLOCK) ;
		_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
	}

private:
	void link_confirm () {
		const auto r1x = _inline_SOCKET_SELECT_ (mSocket ,DEFAULT_TIMEOUT_SIZE::value) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r1x[1]) != 0) ;
		auto rax = PACK<STRA[_SIZEOF_ (VAR32)] ,VAR32> () ;
		::getsockopt (mSocket ,SOL_SOCKET ,SO_ERROR ,_ZERO_ (rax.P1) ,&(rax.P2 = VAR32 (_SIZEOF_ (VAR32)))) ;
		const auto r2x = _CALL_ ([&] () {
			TEMP<VAR32> ret ;
			_MEMCOPY_ (PTRTOARR[&_ZERO_ (ret).unused[0]] ,_CAST_<BYTE[_SIZEOF_ (VAR32)]> (rax.P1)) ;
			return std::move (_CAST_<VAR32> (ret)) ;
		}) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x == 0) ;
	}

	void friend_accept (UniqueRef<SOCKET> &&socket) popping {
		mSocket = std::move (socket) ;
		auto rax = PACK<SOCKADDR ,VAR32> () ;
		::getpeername (mSocket ,&_ZERO_ (rax.P1) ,&(rax.P2 = VAR32 (_SIZEOF_ (SOCKADDR)))) ;
		_DYNAMIC_ASSERT_ (rax.P2 == _SIZEOF_ (SOCKADDR)) ;
		mPeer = rax.P1 ;
	}
} ;

inline exports TCPSocket::TCPSocket (const String<STRU8> &addr) {
	mThis = AnyRef<Implement>::make (addr) ;
}

inline exports String<STRU8> TCPSocket::sock_name () const {
	return mThis.rebind<Implement> ()->sock_name () ;
}

inline exports String<STRU8> TCPSocket::peer_sock_name () const {
	return mThis.rebind<Implement> ()->peer_sock_name () ;
}

inline exports void TCPSocket::link (const String<STRU8> &addr) {
	mThis.rebind<Implement> ()->link (addr) ;
}

inline exports void TCPSocket::modify_buffer (LENGTH rcv_len ,LENGTH snd_len) {
	mThis.rebind<Implement> ()->modify_buffer (rcv_len ,snd_len) ;
}

inline exports void TCPSocket::read (const PhanBuffer<BYTE> &data) popping {
	mThis.rebind<Implement> ()->read (data) ;
}

inline exports void TCPSocket::read (const PhanBuffer<BYTE> &data ,INDEX &it ,LENGTH timeout) popping {
	mThis.rebind<Implement> ()->read (data ,it ,timeout) ;
}

inline exports void TCPSocket::write (const PhanBuffer<const BYTE> &data) {
	mThis.rebind<Implement> ()->write (data) ;
}

class TCPSocket::Listener::Implement final :private Interface {
private:
	UniqueRef<SOCKET> mListener ;
	UniqueRef<SOCKET> mLinker ;

public:
	Implement () = delete ;

	explicit Implement (const String<STRU8> &addr) {
		mListener = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = ::socket (AF_INET ,SOCK_STREAM ,IPPROTO_TCP) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
		} ,[] (SOCKET &me) {
			_DEBUG_ASSERT_ (me != INVALID_SOCKET) ;
			::closesocket (me) ;
		}) ;
		_DYNAMIC_ASSERT_ (!addr.empty ()) ;
		_inline_SOCKET_BIND_ (mListener ,addr) ;
		const auto r1x = ::listen (mListener ,5) ;
		_DYNAMIC_ASSERT_ (r1x != SOCKET_ERROR) ;
	}

	void listen () {
		const auto r1x = _inline_SOCKET_SELECT_ (mListener ,DEFAULT_TIMEOUT_SIZE::value) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (FD_ISSET (mListener ,&r1x[0]) != 0) ;
		mLinker = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = ::accept (mListener ,NULL ,NULL) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
		} ,[] (SOCKET &me) {
			_DEBUG_ASSERT_ (me != INVALID_SOCKET) ;
			::closesocket (me) ;
		}) ;
	}

	void accept (TCPSocket &linker) popping {
		linker.mThis.rebind<TCPSocket::Implement> ()->friend_accept (std::move (mLinker)) ;
	}
} ;

inline exports TCPSocket::Listener::Listener (const String<STRU8> &addr) {
	mThis = AnyRef<Implement>::make (addr) ;
}

inline exports void TCPSocket::Listener::listen () {
	mThis.rebind<Implement> ()->listen () ;
}

inline exports void TCPSocket::Listener::accept (TCPSocket &linker) {
	mThis.rebind<Implement> ()->accept (linker) ;
}

class UDPSocket::Implement final :private Interface {
private:
	UniqueRef<SOCKET> mSocket ;
	SOCKADDR mPeer ;

public:
	Implement () = delete ;

	explicit Implement (const String<STRU8> &addr) {
		mSocket = UniqueRef<SOCKET> ([&] (SOCKET &me) {
			me = ::socket (AF_INET ,SOCK_DGRAM ,IPPROTO_UDP) ;
			_DYNAMIC_ASSERT_ (me != INVALID_SOCKET) ;
			const auto r1x = VAR32 (1) ;
			::setsockopt (me ,SOL_SOCKET ,SO_REUSEADDR ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r1x) ,VAR32 (_SIZEOF_ (VAR32))) ;
			::setsockopt (me ,SOL_SOCKET ,SO_BROADCAST ,_CAST_<STRA[_SIZEOF_ (VAR32)]> (r1x) ,VAR32 (_SIZEOF_ (VAR32))) ;
		} ,[] (SOCKET &me) {
			_DEBUG_ASSERT_ (me != INVALID_SOCKET) ;
			::closesocket (me) ;
		}) ;
		if (!addr.empty ())
			_inline_SOCKET_BIND_ (mSocket ,addr) ;
		_ZERO_ (mPeer) ;
	}

	String<STRU8> sock_name () const {
		auto rax = PACK<SOCKADDR ,VAR32> () ;
		::getsockname (mSocket ,&_ZERO_ (rax.P1) ,&(rax.P2 = VAR32 (_SIZEOF_ (SOCKADDR)))) ;
		//@info: ipv6 is not supported
		_DYNAMIC_ASSERT_ (rax.P2 == _SIZEOF_ (SOCKADDR)) ;
		return _inline_SOCKET_CVTTO_IPV4S_ (rax.P1) ;
	}

	String<STRU8> peer_sock_name () const {
		return _inline_SOCKET_CVTTO_IPV4S_ (mPeer) ;
	}

	void link (const String<STRU8> &addr) {
		mPeer = _inline_SOCKET_CVTTO_SOCKETADDR_ (addr) ;
	}

	void read (const PhanBuffer<BYTE> &data) popping {
		const auto r1x = _inline_SOCKET_SELECT_ (mSocket ,DEFAULT_TIMEOUT_SIZE::value) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r1x[0]) != 0) ;
		auto rax = PACK<SOCKADDR ,VAR32> () ;
		const auto r2x = ::recvfrom (mSocket ,_LOAD_<ARR<STRA>> (data.self) ,VAR32 (data.size ()) ,0 ,&_ZERO_ (rax.P1) ,&(rax.P2 = VAR32 (_SIZEOF_ (SOCKADDR)))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (rax.P2 == _SIZEOF_ (SOCKADDR)) ;
		_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
		mPeer = rax.P1 ;
	}

	void read (const PhanBuffer<BYTE> &data ,INDEX &it ,LENGTH timeout) popping {
		const auto r1x = _inline_SOCKET_SELECT_ (mSocket ,timeout) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r1x[0]) != 0) ;
		auto rax = PACK<SOCKADDR ,VAR32> () ;
		const auto r2x = ::recvfrom (mSocket ,_LOAD_<ARR<STRA>> (data.self) ,VAR32 (data.size ()) ,0 ,&_ZERO_ (rax.P1) ,&(rax.P2 = VAR32 (_SIZEOF_ (SOCKADDR)))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (rax.P2 == _SIZEOF_ (SOCKADDR)) ;
		mPeer = rax.P1 ;
		it = r2x ;
	}

	void write (const PhanBuffer<const BYTE> &data) {
		const auto r1x = _inline_SOCKET_SELECT_ (mSocket ,DEFAULT_TIMEOUT_SIZE::value) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (FD_ISSET (mSocket ,&r1x[1]) != 0) ;
		const auto r2x = ::sendto (mSocket ,_LOAD_<ARR<STRA>> (data.self) ,VAR32 (data.size ()) ,0 ,&mPeer ,VAR32 (_SIZEOF_ (SOCKADDR))) ;
		//@info: state of 'this' has been changed
		_DYNAMIC_ASSERT_ (r2x == data.size ()) ;
	}
} ;

inline exports UDPSocket::UDPSocket (const String<STRU8> &addr) {
	mThis = AnyRef<Implement>::make (addr) ;
}

inline exports String<STRU8> UDPSocket::sock_name () const {
	return mThis.rebind<Implement> ()->sock_name () ;
}

inline exports String<STRU8> UDPSocket::peer_sock_name () const {
	return mThis.rebind<Implement> ()->peer_sock_name () ;
}

inline exports void UDPSocket::link (const String<STRU8> &addr) {
	mThis.rebind<Implement> ()->link (addr) ;
}

inline exports void UDPSocket::read (const PhanBuffer<BYTE> &data) popping {
	mThis.rebind<Implement> ()->read (data) ;
}

inline exports void UDPSocket::read (const PhanBuffer<BYTE> &data ,INDEX &it ,LENGTH timeout) popping {
	mThis.rebind<Implement> ()->read (data ,it ,timeout) ;
}

inline exports void UDPSocket::write (const PhanBuffer<const BYTE> &data) {
	mThis.rebind<Implement> ()->write (data) ;
}

class NetworkService::Implement final :private NetworkService::Abstract {
private:
	friend NetworkService ;
	friend StrongRef<Implement> ;
	UniqueRef<void> mService ;

public:
	void startup () override {
		if (mService.exist ())
			return ;
		mService = UniqueRef<void> ([&] () {
			const auto r1x = (WORD (2) << (_SIZEOF_ (BYTE) * 8)) | WORD (2) ;
			auto rax = WSADATA () ;
			const auto r2x = WSAStartup (r1x ,&_ZERO_ (rax)) ;
			_DYNAMIC_ASSERT_ (r2x == 0) ;
		} ,[] () {
			WSACleanup () ;
		}) ;
	}

	void shutdown () override {
		mService = UniqueRef<void> () ;
	}

	String<STRU8> host_name () const override {
		auto rax = String<STRA> (127) ;
		::gethostname (rax.raw ().self ,VAR32 (rax.size ())) ;
		return _ASTOU8S_ (rax) ;
	}

	String<STRU8> host_addr () const override {
		return String<STRU8> (_PCSTRU8_ ("127.0.0.1")) ;
	}
} ;

inline exports NetworkService::NetworkService () {
	mThis = StrongRef<Implement>::make () ;
}
} ;