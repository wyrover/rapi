#include "remoteapi.h"

/* ----------------------------------------------------------------------------- 
 * Copyright (c) 2003 Elias Bachaalany <lallousz-x86@yahoo.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ----------------------------------------------------------------------------- 
 */

////////////////////////////////////////////////////////////////////////////////
//   Sends a generic buffer
//   You may send a null buffer
int ragdSend(tRemoteApiSession &s, LPVOID p, UINT n)
{
  if (!p)
  {
    s.ragd.nLen   = 4;
    s.ragd.lpData = NULL;
    SendBuffer(s.sd, (LPVOID)&s.ragd, sizeof(tRemoteApiGenericData));
    return SendBuffer(s.sd, (LPVOID)&s.ragd, 4);
  }
  else
  {
    s.ragd.nLen = n;
    s.ragd.lpData = p;
    SendBuffer(s.sd, (LPVOID)&s.ragd, sizeof(tRemoteApiGenericData));
    return SendBuffer(s.sd, p, n);
  }
}

////////////////////////////////////////////////////////////////////////////////
//    Receives a generic remote debug data struct
//    It receives NULL buffers that were sent by ragdSend(s, NULL, x)
int ragdReceive(tRemoteApiSession &s, LPVOID p, UINT n)
{
  ReceiveBuffer(s.sd, (LPVOID)&s.ragd, sizeof(tRemoteApiGenericData));
  if (s.ragd.lpData)
  {
    return ReceiveBuffer(s.sd, p, s.ragd.nLen);
  }
  else
  {
    unsigned long t;
    return ReceiveBuffer(s.sd, (LPVOID)&t, 4);
  }
}


////////////////////////////////////////////////////////////////////////////////
//    Receives a generic remote api data struct
//    then allocates memory needed
//    and then receives the actual data
//    it can receive buffer sent by ragdSendNULL while leaving 'p' unchanged
int ragdReceiveAlloc(tRemoteApiSession &s, LPVOID *p)
{
  ReceiveBuffer(s.sd, (LPVOID)&s.ragd, sizeof(tRemoteApiGenericData));
  if (s.ragd.lpData)
  {
    *p = new CHAR[s.ragd.nLen];
    return ReceiveBuffer(s.sd, *p, s.ragd.nLen);  
  }
  else
  {
    unsigned long t;
    return ReceiveBuffer(s.sd, (LPVOID)&t, 4);
  }
}
