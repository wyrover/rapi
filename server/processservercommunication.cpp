#include "processservercommunication.h"
#include <map>

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

typedef void (*RAPISERVERCALLHANDLER_CB)(tRemoteApiSession &s);
typedef std::map<DWORD, RAPISERVERCALLHANDLER_CB> RAPISERVERCALLHANDLER_MAP;

void fillFunctionTable(RAPISERVERCALLHANDLER_MAP &callmap);

//-------------------------------------------------------------------------------------------
//
// Processes all the server's communication
//
//
bool processServerCommunication(tRemoteApiSession &s)
{
  bool rc;
  
  while (true)
  {
    rc = rarpReceive(s) > 0;
    if (WSAGetLastError() != WSAEWOULDBLOCK)
      break;
    else
    {
      Sleep(100);
    }
  }
  
  if (!rc)
    return false;

  static RAPISERVERCALLHANDLER_MAP callmap;

  if (callmap.size() == 0)
    fillFunctionTable(callmap);

  switch (s.rarp.Cmd)
  {
    //
    case RAC_SERVER_RESTART:
      //restart_server = true;
      //g_close_server = true;
      return false;

    default:
    {
      RAPISERVERCALLHANDLER_MAP::iterator it = callmap.find(s.rarp.Cmd);
      if (it == callmap.end())
        return false;
      it->second(s);
    }
  }
  return true;
}

void fillFunctionTable(RAPISERVERCALLHANDLER_MAP &callmap)
{
  callmap[RAC_K32FINDNEXTFILE]          = RApiServerK32FindNextFile;
  callmap[RAC_K32FINDCLOSE]             = RApiServerK32FindClose;
  callmap[RAC_K32FINDFIRSTFILE]         = RApiServerK32FindFirstFile;
  callmap[RAC_K32WINEXEC]               = RApiServerK32WinExec;
  callmap[RAC_K32CREATEPROCESS]         = RApiServerK32CreateProcess;
  callmap[RAC_K32SUSPENDTHREAD]         = RApiServerK32SuspendThread;
  callmap[RAC_K32RESUMETHREAD]          = RApiServerK32ResumeThread;
  callmap[RAC_K32TERMINATEPROCESS]      = RApiServerK32TerminateProcess;
  callmap[RAC_K32GETTHREADCONTEXT]      = RApiServerK32GetThreadContext;
  callmap[RAC_K32SETTHREADCONTEXT]      = RApiServerK32SetThreadContext;
  callmap[RAC_K32TERMINATETHREAD]       = RApiServerK32TerminateThread;
  callmap[RAC_K32READPROCESSMEMORY]     = RApiServerK32ReadProcessMemory;
  callmap[RAC_K32WRITEPROCESSMEMORY]    = RApiServerK32WriteProcessMemory;
  callmap[RAC_K32FLUSHINSTRUCTIONCACHE] = RApiServerK32FlushInstructionCache;
  callmap[RAC_K32VIRTUALQUERYEX]        = RApiServerK32VirtualQueryEx;
  callmap[RAC_K32WAITFORDEBUGEVENT]     = RApiServerK32WaitForDebugEvent;
  callmap[RAC_K32CREATEFILE]            = RApiServerK32CreateFile;
  callmap[RAC_K32GETFILESIZE]           = RApiServerK32GetFileSize;
  callmap[RAC_K32READFILE]              = RApiServerK32ReadFile;
  callmap[RAC_K32WRITEFILE]             = RApiServerK32WriteFile;
  callmap[RAC_K32SETFILEPOINTER]        = RApiServerK32SetFilePointer;
  callmap[RAC_K32SETFILEPOINTEREX]      = RApiServerK32SetFilePointerEx;
  callmap[RAC_K32CONTINUEDEBUGEVENT]    = RApiServerK32ContinueDebugEvent;
  callmap[RAC_K32CLOSEHANDLE]           = RApiServerK32CloseHandle;
  callmap[RAC_K32OPENPROCESS]           = RApiServerK32OpenProcess;
  callmap[RAC_K32DEBUGACTIVEPROCESS]    = RApiServerK32DebugActiveProcess;
  callmap[RAC_K32DEVICEIOCONTROL]       = RApiServerK32DeviceIoControl;

#ifdef __RAPI_USE_PSAPI
  callmap[RAC_PSAPIGETMODULEFILENAMEEX] = RApiServerGetModuleFileNameEx;
  callmap[RAC_PSAPIGETMODULEBASENAME]   = RApiServerGetModuleBaseName;
  callmap[RAC_PSAPIENUMPROCESSES]       = RApiServerEnumProcesses;
  callmap[RAC_PSAPIENUMPROCESSMODULES]  = RApiServerEnumProcessModules;
#endif

#ifdef __RAPI_USE_XP_FUNCTIONS
  callmap[RAC_K32DEBUGBREAKPROCESS]     = RApiServerK32DebugBreakProcess;
#endif

  callmap[RAC_U32MESSAGEBOX]            = RApiServerU32MessageBox;
  callmap[RAC_U32FINDWINDOW]            = RApiServerU32FindWindow;
}
