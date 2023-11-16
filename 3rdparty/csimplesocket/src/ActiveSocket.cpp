/*---------------------------------------------------------------------------*/
/*                                                                           */
/* CActiveSocket.cpp - Active Socket Implementation                          */
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

#include "ActiveSocket.h"

#ifdef _WIN32
#include <Ws2tcpip.h>
#elif defined( _LINUX ) || defined( _DARWIN )
#include <netdb.h>
#endif

//------------------------------------------------------------------------------
CActiveSocket::CActiveSocket( CSocketType nType ) : CSimpleSocket( nType ) {}

//------------------------------------------------------------------------------
bool CActiveSocket::Validate( const char* pAddr, uint16_t nPort )
{
   if ( !IsSocketValid() )
   {
      SetSocketError( CSimpleSocket::SocketInvalidSocket );
      return false;
   }

   if ( pAddr == nullptr )
   {
      SetSocketError( CSimpleSocket::SocketInvalidAddress );
      return false;
   }

   if ( nPort == 0 )
   {
      SetSocketError( CSimpleSocket::SocketInvalidPort );
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------
bool CActiveSocket::PreConnect( const char* pAddr, uint16_t nPort )
{
   addrinfo hints{ AI_ALL, m_nSocketDomain, 0, 0, 0, nullptr, nullptr, nullptr };
   addrinfo* pResult = nullptr;

   /// https://codereview.stackexchange.com/a/17866
   if ( getaddrinfo( pAddr, nullptr, &hints, &pResult ) != SocketSuccess )
   {
#ifdef _WIN32
      TranslateSocketError();
#else
      SetSocketError( SocketInvalidAddress );
#endif
   }
   else
   {
      m_stServerSockaddr.sin_family = static_cast<decltype( m_stServerSockaddr.sin_family )>( m_nSocketDomain ),
      m_stServerSockaddr.sin_addr = reinterpret_cast<sockaddr_in*>( pResult->ai_addr )->sin_addr;   // NOLINT
      m_stServerSockaddr.sin_port = htons( nPort ),
      freeaddrinfo( pResult );
      return true;
   }

   return false;
}

//------------------------------------------------------------------------------
bool CActiveSocket::ConnectStreamSocket()
{
   bool bRetVal = false;

   m_timer.SetStartTime();

   // Connect to address "xxx.xxx.xxx.xxx"    (IPv4) address only.
   if ( CONNECT( m_socket, &m_stServerSockaddr, SOCKET_ADDR_IN_SIZE ) == CSimpleSocket::SocketError )
   {
      // Get error value this might be a non-blocking socket so we  must first check.
      TranslateSocketError();

      //--------------------------------------------------------------
      // If the socket is non-blocking and the current socket error
      // is SocketEinprogress or SocketEwouldblock then poll connection
      // with select for designated timeout period.
      // Linux returns EINPROGRESS and Windows returns WSAEWOULDBLOCK.
      //--------------------------------------------------------------
      if ( ( IsNonblocking() ) && ( ( GetSocketError() == CSimpleSocket::SocketEwouldblock ) ||
                                    ( GetSocketError() == CSimpleSocket::SocketEinprogress ) ) )
      {
         bRetVal = Select( GetConnectTimeoutSec(), GetConnectTimeoutUSec() );
      }
   }
   else
   {
      TranslateSocketError();
      bRetVal = true;
   }

   m_timer.SetEndTime();

   return bRetVal;
}

//------------------------------------------------------------------------------
bool CActiveSocket::ConnectDatagramSocket()
{
   m_timer.SetStartTime();
   const bool bRetVal = ( CONNECT( m_socket, &m_stServerSockaddr, SOCKET_ADDR_IN_SIZE ) == SocketSuccess );
   m_timer.SetEndTime();

   TranslateSocketError();

   return bRetVal;
}

//------------------------------------------------------------------------------
bool CActiveSocket::Open( const char* pAddr, uint16_t nPort )
{
   const auto stCurrentServerInfo = m_stServerSockaddr;
   bool bRetVal = Validate( pAddr, nPort );

   // Preconnection setup that must be preformed
   if ( bRetVal )
   {
      bRetVal = PreConnect( pAddr, nPort );
   }

   if ( bRetVal )
   {
      bRetVal = ( m_nSocketType == SocketTypeTcp ) ? ConnectStreamSocket () :
                    ( m_nSocketType == SocketTypeUdp ) ? ConnectDatagramSocket() : false;
   }

   // If successful then get a local copy of the address and port
   if ( bRetVal )
   {
      socklen_t nSockLen = SOCKET_ADDR_IN_SIZE;

      memset( &m_stServerSockaddr, 0, SOCKET_ADDR_IN_SIZE );
      GETPEERNAME( m_socket, &m_stServerSockaddr, &nSockLen );

      memset( &m_stClientSockaddr, 0, SOCKET_ADDR_IN_SIZE );
      GETSOCKNAME( m_socket, &m_stClientSockaddr, &nSockLen );
   }
   else
   {
      m_stServerSockaddr = stCurrentServerInfo;
   }

   return bRetVal;
}

//------------------------------------------------------------------------------
sockaddr_in* CActiveSocket::GetUdpRxAddrBuffer()
{
   return m_bIsMulticast ? &m_stClientSockaddr : &m_stServerSockaddr;
}

//------------------------------------------------------------------------------
sockaddr_in* CActiveSocket::GetUdpTxAddrBuffer()
{
   return m_bIsMulticast ? &m_stMulticastGroup : &m_stServerSockaddr;
}
