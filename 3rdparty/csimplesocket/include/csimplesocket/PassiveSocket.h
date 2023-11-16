// Copyright (c) 2007-2009 CarrierLabs, LLC.  All rights reserved.1
// BSD-3-Clause

#ifndef __PASSIVESOCKET_H__
#define __PASSIVESOCKET_H__

#include "ActiveSocket.h"

#include <memory>

class CPassiveSocket : public CSimpleSocket
{
public:
   explicit CPassiveSocket( CSocketType type = SocketTypeTcp );

   auto Accept() -> std::unique_ptr<CActiveSocket>;
   bool Listen( const char* pAddr, uint16_t nPort, int32_t nConnectionBacklog = 30000 );
};

#endif   // __PASSIVESOCKET_H__
