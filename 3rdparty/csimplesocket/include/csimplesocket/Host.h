// Copyright (c) 2007-2009 CarrierLabs, LLC.  All rights reserved.1
// BSD-3-Clause

#ifndef __HOST_H__
#define __HOST_H__

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Type Definition Macros                                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef __WORDSIZE
   static_assert( sizeof( void* ) == 8 || sizeof( void* ) == 4 );
   constexpr auto __WORDSIZE = ( sizeof( void* ) == 8 ) ? 64 : ( sizeof( void* ) == 4 ) ? 32 : 0;
#endif

#if defined(_LINUX) || defined(_DARWIN)
   using SOCKET = int;
#elif defined( _WIN32 )
   struct iovec
   {
      void*  iov_base;
      size_t iov_len;
   };

   using socklen_t = int;
#endif

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Socket Macros                                                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifdef _WIN32

   #define SHUT_RD                0
   #define SHUT_WR                1
   #define SHUT_RDWR              2
   #define ACCEPT(a,b,c)          accept(a,(sockaddr *)b,c)
   #define BIND(a,b,c)            bind(a,(sockaddr *)b,c)
   #define CONNECT(a,b,c)         connect(a,(sockaddr *)b,c)
   #define CLOSE(a)               closesocket(a)
   #define READ(a,b,c)            _read(a,b,c)
   #define SEEK(a,b,c)            _lseek(a,b,c)
   #define RECV(a,b,c,d)          recv(a, (char *)b, c, d)
   #define RECVFROM(a,b,c,d,e,f)  recvfrom(a, (char *)b, c, d, (sockaddr *)e, (int *)f)
   #define RECV_FLAGS             MSG_WAITALL
   #define SELECT(a,b,c,d,e)      select((int32_t)a,b,c,d,e)
   #define SEND(a,b,c,d)          send(a, (const char *)b, (int)c, d)
   #define SENDTO(a,b,c,d,e,f)    sendto(a, (const char *)b, (int)c, d, e, f)
   #define SEND_FLAGS             0
   #define SENDFILE(a,b,c,d)      sendfile(a, b, c, d)
   #define SET_SOCKET_ERROR(x,y)  errno=y
   #define SOCKET_ERROR_INTERUPT  EINTR
   #define SOCKET_ERROR_TIMEDOUT  EAGAIN
   #define WRITE(a,b,c)           write(a,b,c)
   #define WRITEV(a,b,c)          Writev(b, c)
   #define GETSOCKOPT(a,b,c,d,e)  getsockopt(a,b,c,(char *)d, (int *)e)
   #define SETSOCKOPT(a,b,c,d,e)  setsockopt(a,b,c,(char *)d, (int)e)
   #define GETPEERNAME(a, b, c)   getpeername( a, (sockaddr*)b, c)
   #define GETSOCKNAME(a, b, c)   getsockname( a, (sockaddr*)b, c)

#elif defined(_LINUX) || defined(_DARWIN)

   #define ACCEPT(a,b,c)          accept(a,(sockaddr *)b,c)
   #define BIND(a,b,c)            bind(a,(sockaddr *)b,c)
   #define CONNECT(a,b,c)         connect(a,(sockaddr *)b,c)
   #define CLOSE(a)               close(a)
   #define READ(a,b,c)            pread(a,b,c,0)
   #define SEEK(a,b,c)            lseek(a,b,c)
   #define RECV(a,b,c,d)          recv(a, (void *)b, c, d)
   #define RECVFROM(a,b,c,d,e,f)  recvfrom(a, (char *)b, c, d, (sockaddr *)e, f)
   #define RECV_FLAGS             MSG_WAITALL
   #define SELECT(a,b,c,d,e)      select(a,b,c,d,e)
   #define SEND(a,b,c,d)          send(a, (const char *)b, c, d)
   #define SENDTO(a,b,c,d,e,f)    sendto(a, (const char *)b, c, d, e, f)
   #define SEND_FLAGS             0
   #define SENDFILE(a,b,c,d)      sendfile(a, b, c, d)
   #define SET_SOCKET_ERROR(x,y)  errno=y
   #define SOCKET_ERROR_INTERUPT  EINTR
   #define SOCKET_ERROR_TIMEDOUT  EAGAIN
   #define WRITE(a,b,c)           write(a,b,c)
   #define WRITEV(a,b,c)          writev(a, b, c)
   #define GETSOCKOPT(a,b,c,d,e)  getsockopt((int)a,(int)b,(int)c,(void *)d,(socklen_t *)e)
   #define SETSOCKOPT(a,b,c,d,e)  setsockopt((int)a,(int)b,(int)c,(const void *)d,(int)e)
   #define GETPEERNAME(a, b, c)   getpeername( a, (sockaddr*)b, c)
   #define GETSOCKNAME(a, b, c)   getsockname( a, (sockaddr*)b, c)

#endif

#endif   // __HOST_H__
