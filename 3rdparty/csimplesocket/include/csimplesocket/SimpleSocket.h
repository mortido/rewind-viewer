// Copyright (c) 2007-2009 CarrierLabs, LLC.  All rights reserved.1
// BSD-3-Clause

#ifndef __SOCKET_H__
#define __SOCKET_H__

#if defined( _LINUX ) || defined( _DARWIN )
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/tcp.h>
#include <sys/uio.h>
#include <unistd.h>
#endif

#ifdef _DARWIN
#include <net/if.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "Host.h"
#include "StatTimer.h"

#include <string>
#include <cstdint>

#ifdef STRING_VIEW
#include <string_view>
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET ~( 0 )
#endif

auto constexpr length( const char* str ) -> long { return *str ? 1 + length( str + 1 ) : 0; }

class CSimpleSocket
{
public:
   /// Defines the three possible states for shutting down a socket.
   enum CShutdownMode
   {
      Receives = SHUT_RD,   ///< Shutdown passive socket.
      Sends = SHUT_WR,      ///< Shutdown active socket.
      Both = SHUT_RDWR      ///< Shutdown both active and passive sockets.
   };

   /// Defines the socket types defined by CSimpleSocket class.
   enum CSocketType
   {
      SocketTypeInvalid,             ///< Invalid socket type.
      SocketTypeTcp = SOCK_STREAM,   ///< Defines socket as TCP socket.
      SocketTypeUdp = SOCK_DGRAM,    ///< Defines socket as UDP socket.
      // SocketTypeTcp6,      ///< Defines socket as IPv6 TCP socket.
      // SocketTypeUdp6,      ///< Defines socket as IPv6 UDP socket.
      // SocketTypeRaw = SOCK_RAW   ///< Provides raw network protocol access.
      // SocketTypeRaw6   ///< Provides raw network protocol access.
   };

   /// Defines all error codes handled by the CSimpleSocket class.
   enum CSocketError
   {
      SocketError = -1,          ///< Generic socket error translates to error below.
      SocketSuccess = 0,         ///< No socket error.
      SocketInvalidSocket,       ///< Invalid socket handle.
      SocketInvalidAddress,      ///< Invalid destination address specified.
      SocketInvalidPort,         ///< Invalid destination port specified.
      SocketConnectionRefused,   ///< No server is listening at remote address.
      SocketTimedout,            ///< Timed out while attempting operation.
      SocketEwouldblock,         ///< Operation would block if socket were blocking.
      SocketNotconnected,        ///< Currently not connected.
      SocketEinprogress,         ///< Socket is non-blocking and the connection cannot be completed immediately
      SocketInterrupted,   ///< Call was interrupted by a signal that was caught before a valid connection arrived.
      SocketConnectionAborted,     ///< The connection has been aborted.
      SocketProtocolError,         ///< Invalid protocol for operation.
      SocketFirewallError,         ///< Firewall rules forbid connection.
      SocketInvalidSocketBuffer,   ///< The receive buffer point outside the process's address space.
      SocketConnectionReset,       ///< Connection was forcibly closed by the remote host.
      SocketAddressInUse,          ///< Address already in use.
      SocketInvalidPointer,        ///< Pointer type supplied as argument is invalid.
      SocketInvalidOperation,      ///< An invalid argument was provide for the requested action.
      SocketAlreadyConnected,      ///< A requested action was not possible because of socket state.
      SocketRoutingError,          ///< OS could not resolve route for requested operation
      SocketEunknown               ///< Unknown error please report to mark@carrierlabs.com
   };

public:
   explicit CSimpleSocket( CSocketType type = SocketTypeTcp );
   CSimpleSocket( const CSimpleSocket& ) = delete;
   CSimpleSocket( CSimpleSocket&& socket ) noexcept;
   virtual ~CSimpleSocket();

   CSimpleSocket& operator=( const CSimpleSocket& ) = delete;
   CSimpleSocket& operator=( CSimpleSocket&& other ) noexcept;

   friend void swap( CSimpleSocket& lhs, CSimpleSocket& rhs ) noexcept;

   bool Shutdown( CShutdownMode nShutdown );
   bool Close();

   bool Select();
   bool Select( int32_t nTimeoutSec, int32_t nTimeoutUSec );

   [[nodiscard]] bool IsSocketValid() const { return ( m_socket != INVALID_SOCKET ); }

   static std::string DescribeError( CSocketError err );
   [[nodiscard]] std::string DescribeError() const { return DescribeError( m_error ); }

   int32_t Receive( uint32_t nMaxBytes = 1, uint8_t* pBuffer = nullptr );

   /// Attempts to send a block of data on an established connection.
   /// @param pBuf block of data to be sent.
   /// @param bytesToSend size of data block to be sent.
   /// @return number of bytes actually sent.
   /// @return of zero means the connection has been shutdown on the other side.
   /// @return of -1 means that an error has occurred.
   virtual int32_t Send( const uint8_t* pBuf, size_t bytesToSend );

#ifdef STRING_VIEW
   int32_t Send( std::string_view bytes )
   {
      return Send( reinterpret_cast<const uint8_t*>( bytes.data() ), bytes.length() );
   }
#endif

   [[nodiscard]] bool IsNonblocking() const { return !m_bIsBlocking; }
   bool SetBlocking();
   bool SetNonblocking();

   [[nodiscard]] const std::string& GetData() const { return m_sBuffer; }
   [[nodiscard]] int32_t GetBytesReceived() const { return m_nBytesReceived; }
   [[nodiscard]] int32_t GetBytesSent() const { return m_nBytesSent; }

   /// IGMPv2 Join for a multicast group.This options is only valid for
   /// socket descriptors of type CSimpleSocket::SocketTypeUdp and
   /// GetMulticast() is true
   /// @return true if the operation completes successfully or else an
   /// error will be set.
   bool JoinMulticast( const char* pGroup, uint16_t nPort );

   /// Bind socket to a specific interface when using unicast or multicast.
   /// @return true if successfully bound to interface
   bool BindInterface( const char* pInterface );

   /// Controls the actions taken when CSimpleSocket::Close is executed on a
   /// socket object that has unsent data. The default value for this option
   /// is \b off.
   /// - Following are the three possible scenarios.
   ///  -# \b bEnable is false, CSimpleSocket::Close returns immediately, but
   ///  any unset data is transmitted (after CSimpleSocket::Close returns)
   ///  -# \b bEnable is true and \b nTime is zero, CSimpleSocket::Close return
   /// immediately and any unsent data is discarded.
   ///  -# \b bEnable is true and \b nTime is nonzero, CSimpleSocket::Close does
   ///  not return until all unsent data is transmitted (or the connection is
   ///  Closed by the remote system).
   /// <br/><p>
   /// @param bEnable true to enable option false to disable option.
   /// @param nTime time in seconds to linger.
   /// @return true if option successfully set
   bool SetOptionLinger( bool bEnable, uint16_t nTime );

   /// Tells the kernel that even if this port is busy (in the TIME_WAIT state),
   /// go ahead and reuse it anyway.  If it is busy, but with another state,
   /// you will still get an address already in use error.
   /// @return true if option successfully set
   bool SetOptionReuseAddr();

   [[nodiscard]] int32_t GetConnectTimeoutSec() const { return m_stConnectTimeout.tv_sec; }
   [[nodiscard]] int32_t GetConnectTimeoutUSec() const { return m_stConnectTimeout.tv_usec; }
   void SetConnectTimeout( int32_t nConnectTimeoutSec, int32_t nConnectTimeoutUsec );

   [[nodiscard]] int32_t GetReceiveTimeoutSec() const { return m_stRecvTimeout.tv_sec; }
   [[nodiscard]] int32_t GetReceiveTimeoutUSec() const { return m_stRecvTimeout.tv_usec; }
   bool SetReceiveTimeout( int32_t nRecvTimeoutSec, int32_t nRecvTimeoutUsec = 0 );

   [[nodiscard]] int32_t GetSendTimeoutSec() const { return m_stSendTimeout.tv_sec; }
   [[nodiscard]] int32_t GetSendTimeoutUSec() const { return m_stSendTimeout.tv_usec; }
   bool SetSendTimeout( int32_t nSendTimeoutSec, int32_t nSendTimeoutUsec = 0 );

   bool SetMulticast( bool bEnable, uint8_t multicastTTL = 1 );
   [[nodiscard]] bool GetMulticast() const { return m_bIsMulticast; }

   /// Get the total time the of the last operation in milliseconds.
   ///  @return number of milliseconds of last operation.
   [[nodiscard]] auto GetTotalTimeMs() const { return m_timer.GetMilliSeconds(); }

   /// Get the total time the of the last operation in microseconds.
   ///  @return number of microseconds or last operation.
   [[nodiscard]] auto GetTotalTimeUsec() const { return m_timer.GetMicroSeconds(); }

   /// Return Differentiated Services Code Point (DSCP) value currently set on the socket object.
   /// @return DSCP for current socket object.
   /// <br/><br/> \b NOTE: Windows special notes http://support.microsoft.com/kb/248611.
   int GetSocketDscp();

   /// Set Differentiated Services Code Point (DSCP) for socket object.
   ///  @param nDscp value of TOS setting which will be converted to DSCP
   ///  @return true if DSCP value was properly set
   /// <br/><br/> \b NOTE: Windows special notes http://support.microsoft.com/kb/248611.
   bool SetSocketDscp( int nDscp );

   [[nodiscard]] CSocketError GetSocketError() const { return m_error; }
   [[nodiscard]] CSocketType GetSocketType() const { return m_nSocketType; }

   std::string GetClientAddr();
   [[nodiscard]] uint16_t GetClientPort() const { return ntohs( m_stClientSockaddr.sin_port ); }

   std::string GetServerAddr();
   [[nodiscard]] uint16_t GetServerPort() const { return ntohs( m_stServerSockaddr.sin_port ); }

   std::string GetJoinedGroup();

   /// Get the TCP receive buffer window size for the current socket object.
   /// <br/><br/>\b NOTE: Linux will set the receive buffer to twice the value passed.
   ///  @return zero on failure else the number of bytes of the TCP receive buffer window size if successful.
   uint32_t GetReceiveWindowSize() { return GetWindowSize( SO_RCVBUF ); }

   /// Get the TCP send buffer window size for the current socket object.
   /// <br/><br/>\b NOTE: Linux will set the send buffer to twice the value passed.
   ///  @return zero on failure else the number of bytes of the TCP receive buffer window size if successful.
   uint32_t GetSendWindowSize() { return GetWindowSize( SO_SNDBUF ); }

   /// Set the TCP receive buffer window size for the current socket object.
   /// <br/><br/>\b NOTE: Linux will set the receive buffer to twice the value passed.
   ///  @return zero on failure else the number of bytes of the TCP send buffer window size if successful.
   uint32_t SetReceiveWindowSize( uint32_t nWindowSize ) { return SetWindowSize( SO_RCVBUF, nWindowSize ); }

   /// Set the TCP send buffer window size for the current socket object.
   /// <br/><br/>\b NOTE: Linux will set the send buffer to twice the value passed.
   ///  @return zero on failure else the number of bytes of the TCP send buffer window size if successful.
   uint32_t SetSendWindowSize( uint32_t nWindowSize ) { return SetWindowSize( SO_SNDBUF, nWindowSize ); }

   /// Disable the Nagle algorithm (Set TCP_NODELAY to true)
   /// @return false if failed to set socket option otherwise return true;
   bool DisableNagleAlgoritm();

   /// Enable the Nagle algorithm (Set TCP_NODELAY to false)
   /// @return false if failed to set socket option otherwise return true;
   bool EnableNagleAlgoritm();

   /// Flush the socket descriptor owned by the object.
   /// @return true data was successfully sent, else return false;
   bool Flush();
   
protected:
   /// Errors : CSocket::SocketProtocolError, CSocket::SocketInvalidSocket,
   /// @return true if properly initialized.
   bool ObtainNewHandle();

   /// Set internal socket error to that specified error
   ///  @param error type of error
   void SetSocketError( CSimpleSocket::CSocketError error ) { m_error = error; }

   /// Provides a standard error code for cross platform development by mapping the
   /// operating system error to an error defined by the CSimpleSocket class.
   void TranslateSocketError();

   /// Set object socket handle to that specified as parameter
   ///  @param socket value of socket descriptor
   void SetSocketHandle( SOCKET socket ) { m_socket = socket; }

   virtual sockaddr_in* GetUdpRxAddrBuffer() { return &m_stClientSockaddr; }
   virtual sockaddr_in* GetUdpTxAddrBuffer() { return m_bIsMulticast ? &m_stMulticastGroup : &m_stClientSockaddr; }

   static constexpr int SOCKET_ADDR_IN_SIZE = sizeof( sockaddr_in );

private:
   /// Generic function used to get the send/receive window size
   ///  @return zero on failure else the number of bytes of the TCP window size if successful.
   uint32_t GetWindowSize( uint32_t nOptionName );

   /// Generic function used to set the send/receive window size
   ///  @return zero on failure else the number of bytes of the TCP window size if successful.
   uint32_t SetWindowSize( uint32_t nOptionName, uint32_t nWindowSize );

   bool BindUnicastInterface( const char* pInterface );
   bool BindMulticastInterface( const char* pInterface );

protected:
   SOCKET m_socket = INVALID_SOCKET;                /// socket handle
   CSocketError m_error = SocketInvalidSocket;      /// number of last error
   std::string m_sBuffer;                           /// internal send/receive buffer
   int32_t m_nSocketDomain = AF_UNSPEC;             /// socket domain IPv4 (AF_INET) or IPv6 (AF_INET6)
   CSocketType m_nSocketType = SocketTypeInvalid;   /// socket type - UDP, TCP or RAW
   int32_t m_nBytesReceived = -1;                   /// number of bytes received
   int32_t m_nBytesSent = -1;                       /// number of bytes sent
   uint32_t m_nFlags = 0;                           /// socket flags
   bool m_bIsBlocking = true;                       /// is socket blocking
   bool m_bIsMulticast = false;                     /// is the UDP socket multi-cast;
   timeval m_stConnectTimeout = { 0, 0 };           /// connection timeout
   timeval m_stRecvTimeout = { 0, 0 };              /// receive timeout
   timeval m_stSendTimeout = { 0, 0 };              /// send timeout
   sockaddr_in m_stServerSockaddr = {};             /// server address
   sockaddr_in m_stClientSockaddr = {};             /// client address
   sockaddr_in m_stMulticastGroup = {};             /// multi-cast group to bind to
   linger m_stLinger = { 0, 0 };                    /// linger flag
   CStatTimer m_timer;                              /// internal statistics.

#ifdef WIN32
   WSADATA m_hWSAData = {};   /// Windows
#endif

   fd_set m_writeFds = { 0, 0 };   /// write file descriptor set
   fd_set m_readFds = { 0, 0 };    /// read file descriptor set
   fd_set m_errorFds = { 0, 0 };   /// error file descriptor set
};

#endif   //  __SOCKET_H__
