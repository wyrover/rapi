#include "remoteapi_client.h"
#include "remotedebug_util.h"

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

////////////////////////////////////////////////////////////////////////////////////////////
void UnicodeToAnsi(LPWSTR WideStr, LPSTR AnsiStr)
{
  CHAR buf[MAX_PATH];
  LPSTR p = (LPSTR) &buf;
  
  if ((LPVOID)WideStr != (LPVOID)AnsiStr)
    p = AnsiStr;

  int n = WideCharToMultiByte(0, 0, WideStr, -1, p, MAX_PATH, 0, 0);
  
  if (p != AnsiStr)
    memcpy(AnsiStr, p, n);
}


////////////////////////////////////////////////////////////////////////////////////////////
BOOL GetRemoteDebugeeModuleName(
  tRemoteApiSession &s,
  const HANDLE hProcess, 
  const LPVOID lpImageName, 
  BOOL Unicode,
  LPVOID Buffer,
  int nSize)
{
  DWORD nbRW;
  LPVOID lpRemoteImageName;

  // no image name ?
  if (lpImageName == NULL)
    return FALSE;

  // see where image name points to
  if (!RApiClientReadProcessMemory(s,
                                  hProcess, 
                                  (LPCVOID)lpImageName, 
                                  &lpRemoteImageName, 
                                  4, 
                                  &nbRW))
    return FALSE;

  // No image name pointer ?
  if (lpRemoteImageName == NULL)
    return FALSE;

  // now read image name
  if (!RApiClientReadProcessMemory(s, hProcess, lpRemoteImageName, Buffer, nSize, &nbRW))
    return FALSE;
  
  if (Unicode)
    UnicodeToAnsi((LPWSTR)Buffer, (LPSTR)Buffer);

  return TRUE;
}


//--------------------------------------------------------------------------
//
// This function, given a process name (can be full path) and a module name that is used
// by the process it will then return both the process' ID and the module handle.
//
// The modulename can be the procName too, and that will return module of processname
// As if you were in the same processes and issued a GetModuleHandle(NULL)
//
//
//
BOOL FindRemoteModule(
  tRemoteApiSession &session,
  LPCSTR procName,
  LPCSTR modName,
  PDWORD _uProcessId,
  PDWORD _hModule,
  BOOL BaseNames)
{

  DWORD uProcessId, nModules, nProcess, nCount = 1024;

  HANDLE hProcess;

  DWORD *processes = new DWORD[nCount];

  // buffer for process names
  CHAR buf[MAX_PATH];

  // enum all processes
  if (!RApiClientEnumProcesses(session, processes, nCount * sizeof(DWORD), &nProcess))
  {
    delete processes;
    return FALSE;
  }

  // convert fron bytes count to items count
  nProcess /= 4;

  // allocate memory for modules
  HMODULE *modules   = new HMODULE[nCount], hModule;

  BOOL bStopLoop = FALSE;

  // walk in process list
  for (DWORD i=0; (i < nProcess) && !bStopLoop; i++)
  {
    uProcessId = processes[i];

    // open process for querying only
    hProcess = RApiClientOpenProcess(session, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, uProcessId);
    
    if (!hProcess)
      continue;

    // try to get first module
    if (!RApiClientEnumProcessModules(session, hProcess, &hModule, sizeof(hModule), &nModules))
    {
      RApiClientCloseHandle(session, hProcess);
      continue;
    }

    if (BaseNames)
    {
      // get module name
      RApiClientGetModuleBaseName(session, hProcess, hModule, buf, sizeof(buf));    
    }
    else
    {
      // get module name (base)
      RApiClientGetModuleFileNameEx(session, hProcess, hModule, buf, sizeof(buf));
    }

    // that is not our process
    if (stricmp(buf, procName) != 0)
    {
      RApiClientCloseHandle(session, hProcess);
      continue;
    }

    // now try to enum all modules
    if (!RApiClientEnumProcessModules(session, hProcess, modules, nCount * sizeof(DWORD), &nModules))    
    {
      RApiClientCloseHandle(session, hProcess);
      continue;
    }

    nModules /= 4;

    for (DWORD j=0;j<nModules;j++)
    {
      if (BaseNames)
      {
        // get module name (base)
        RApiClientGetModuleBaseName(session, hProcess, modules[j], buf, sizeof(buf));    
      }
      else
      {
        // get module name (full)
        RApiClientGetModuleFileNameEx(session, hProcess, modules[j], buf, sizeof(buf));
      }

      // is that our desired module ?
      if (stricmp(buf, modName) == 0)
      {
        // return this process id
        *_uProcessId = uProcessId;    

        // return hModule
        *_hModule    = (DWORD)modules[j];

        bStopLoop = TRUE;

        RApiClientCloseHandle(session, hProcess);

        break;
      }
    }
  }

  delete modules;
  delete processes;
  return bStopLoop;
}
