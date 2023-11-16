/*---------------------------------------------------------------------------*/
/*                                                                           */
/* PassiveSocket.cpp - Passive Socket Implementation                         */
/*                                                                           */
/* Author : Mark Carrier (mark@carrierlabs.com)                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Copyright (c) 2007-2009 CarrierLabs, LLC.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * 4. The name "CarrierLabs" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    mark@carrierlabs.com.
 *
 * THIS SOFTWARE IS PROVIDED BY MARK CARRIER ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL MARK CARRIER OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *----------------------------------------------------------------------------*/

#include "PassiveSocket.h"

#ifdef _WIN32
#include <Ws2tcpip.h>
#elif defined( _LINUX ) || defined( _DARWIN )
#include <netinet/ip.h>
#endif

CPassiveSocket::CPassiveSocket( CSocketType nType ) : CSimpleSocket( nType ) {}

bool CPassiveSocket::Listen( const char* pAddr, uint16_t nPort, int32_t nConnectionBacklog )
{
#ifdef _LINUX
   //--------------------------------------------------------------------------
   // Set the following socket option SO_REUSEADDR. This will allow the file
   // descriptor to be reused immediately after the socket is closed instead
   // of setting in a TIMED_WAIT state.
   //--------------------------------------------------------------------------
   if ( !SetOptionReuseAddr() )
   {
      return false;
   }
#endif

   if ( ( pAddr == nullptr ) || ( strlen( pAddr ) == 0 ) )
   {
      // bind to all interfaces
      m_stServerSockaddr.sin_addr.s_addr = htonl( INADDR_ANY );
   }
   else
   {
      // lookup specified address
      switch ( inet_pton( m_nSocketDomain, pAddr, &m_stServerSockaddr.sin_addr ) )
      {
      // Only possible with bad socket domain
      // case SocketError:
      //   TranslateSocketError();
      //   return false;
      case 0:
         SetSocketError( SocketInvalidAddress );
         return false;
      default:
         // Otherwise Success
         break;
      }
   }

   m_stServerSockaddr.sin_family = static_cast<decltype( m_stServerSockaddr.sin_family )>( m_nSocketDomain );
   m_stServerSockaddr.sin_port = htons( nPort );

   m_timer.SetStartTime();

   // Bind to the specified addr and port
   bool bRetVal = ( BIND( m_socket, &m_stServerSockaddr, SOCKET_ADDR_IN_SIZE ) == CSimpleSocket::SocketSuccess );

   if ( bRetVal && m_nSocketType == CSimpleSocket::SocketTypeTcp )
   {
      bRetVal = ( listen( m_socket, nConnectionBacklog ) != CSimpleSocket::SocketError );
   }

   m_timer.SetEndTime();

   TranslateSocketError();

   // If there was a socket error then close the socket to clean out the connection in the backlog.
   if ( !bRetVal )
   {
      m_stServerSockaddr.sin_port = htons( 0 );
      m_stServerSockaddr.sin_addr.s_addr = htonl( INADDR_ANY );
      const CSocketError err = GetSocketError();
      Close();
      SetSocketError( err );
   }
   else
   {
      socklen_t nSockAddrLen( SOCKET_ADDR_IN_SIZE );
      memset( &m_stServerSockaddr, 0, SOCKET_ADDR_IN_SIZE );
      GETSOCKNAME( m_socket, &m_stServerSockaddr, &nSockAddrLen );
   }

   return bRetVal;
}

auto CPassiveSocket::Accept() -> std::unique_ptr<CActiveSocket>
{
   if ( m_nSocketType != CSimpleSocket::SocketTypeTcp )
   {
      SetSocketError( CSimpleSocket::SocketProtocolError );
      return nullptr;
   }

   auto pClientSocket = std::make_unique<CActiveSocket>();
   CSocketError socketErrno;

   m_timer.SetStartTime();

   // do
   //{
   socklen_t nSockAddrLen( SOCKET_ADDR_IN_SIZE );
   const SOCKET socket = ACCEPT( m_socket, &m_stClientSockaddr, &nSockAddrLen );   // Wait for incoming connection.

   if ( socket != INVALID_SOCKET )
   {
      pClientSocket->SetSocketHandle( socket );
      pClientSocket->TranslateSocketError();
      socketErrno = pClientSocket->GetSocketError();

      // Store client and server IP and port information for this connection.
      GETPEERNAME( m_socket, &pClientSocket->m_stClientSockaddr, &nSockAddrLen );
      memcpy( &pClientSocket->m_stClientSockaddr, &m_stClientSockaddr, SOCKET_ADDR_IN_SIZE );

      GETSOCKNAME( m_socket, &pClientSocket->m_stServerSockaddr, &nSockAddrLen );
   }
   else
   {
      TranslateSocketError();
      socketErrno = GetSocketError();
   }
   //} while ( socketErrno == CSimpleSocket::SocketInterrupted );

   m_timer.SetEndTime();

   if ( socketErrno != CSimpleSocket::SocketSuccess )
   {
      pClientSocket = nullptr;
   }

   return pClientSocket;
}
