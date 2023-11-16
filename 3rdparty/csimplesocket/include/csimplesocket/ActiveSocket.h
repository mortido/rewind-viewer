// Copyright (c) 2007-2009 CarrierLabs, LLC.  All rights reserved.1
// BSD-3-Clause

#ifndef __ACTIVESOCKET_H__
#define __ACTIVESOCKET_H__

#include "SimpleSocket.h"

class CActiveSocket : public CSimpleSocket
{
public:
   friend class CPassiveSocket;

   explicit CActiveSocket( CSocketType type = SocketTypeTcp );

   bool Open( const char* pAddr, uint16_t nPort );

protected:
   sockaddr_in* GetUdpRxAddrBuffer() override;
   sockaddr_in* GetUdpTxAddrBuffer() override;

private:
   bool Validate( const char* pAddr, uint16_t nPort );
   bool PreConnect( const char* pAddr, uint16_t nPort );   // Convert and Save params for OS layer
   bool ConnectStreamSocket();
   bool ConnectDatagramSocket();
};

#endif   //  __ACTIVESOCKET_H__
