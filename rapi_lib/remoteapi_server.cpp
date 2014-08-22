#include "remoteapi_server.h"

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

#ifdef __RAPI_USE_PSAPI
//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerEnumProcesses(tRemoteApiSession &s)
{
  DWORD &cb = s.rarp.Parameters.PSAPI_EnumProcesses.cb;
  BOOL big = cb > RAPI_CACHE_BUFSIZE;
  BYTE *lpidProcess;
  
  if (big)
    lpidProcess = new BYTE[cb];
  else
    lpidProcess = (BYTE *)&s.rarp.CacheBuffer;

  s.rarp.ReturnValues.b = ::EnumProcesses((DWORD *)lpidProcess, 
                                        cb, 
                                        &s.rarp.Parameters.PSAPI_EnumProcesses.cbNeeded);

  rarpGetLastError(s);
  rarpSend(s);

  if (big)
  {
    ragdSend(s, lpidProcess, cb);
    delete [] lpidProcess;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerGetModuleFileNameEx(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.dw = ::GetModuleFileNameEx(
                            s.rarp.Parameters.PSAPI_GetModuleFileNameEx.hProcess,
                            s.rarp.Parameters.PSAPI_GetModuleFileNameEx.hModule,
                            (LPTSTR)&s.rarp.CacheBuffer,
                            s.rarp.Parameters.PSAPI_GetModuleFileNameEx.nSize);
  rarpGetLastError(s);
  rarpSend(s);

  DEBUG_PRINT(("GetModuleFileNameEx:(hProcess=%ld, hModule=%08lX)=%s;GLE=%ld", 
                  s.rarp.Parameters.GetModuleFileNameEx.hProcess,
                  s.rarp.Parameters.GetModuleFileNameEx.hModule,
                  s.rarp.ReturnValues.dw ? buf : "?",
                  s.rarp.Win32LastError))
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerGetModuleBaseName(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.dw = ::GetModuleBaseName(
                            s.rarp.Parameters.PSAPI_GetModuleBaseName.hProcess,
                            s.rarp.Parameters.PSAPI_GetModuleBaseName.hModule,
                            (LPTSTR)&s.rarp.CacheBuffer,
                            s.rarp.Parameters.PSAPI_GetModuleBaseName.nSize);
  rarpGetLastError(s);
  rarpSend(s);

  DEBUG_PRINT(("GetModuleBaseName:(hProcess=%ld, hModule=%08lX)=%s;GLE=%ld", 
                  s.rarp.Parameters.PSAPI_GetModuleFileNameEx.hProcess,
                  s.rarp.Parameters.PSAPI_GetModuleFileNameEx.hModule,
                  s.rarp.ReturnValues.dw ? buf : "?",
                  s.rarp.Win32LastError))
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerEnumProcessModules(tRemoteApiSession &s)
{
  DWORD &cb = s.rarp.Parameters.PSAPI_EnumProcessModules.cb;
  BOOL big = cb > RAPI_CACHE_BUFSIZE;
  BYTE *lphModule;
  
  // big? allocate buffer so we can store into it
  if (big)
    lphModule = new BYTE [cb];
  else
    // store in our cache buffer
    lphModule = (BYTE *)&s.rarp.CacheBuffer;

  s.rarp.ReturnValues.b = ::EnumProcessModules(s.rarp.Parameters.PSAPI_EnumProcessModules.hProcess,
                                        (HMODULE *)lphModule, 
                                        cb, 
                                        &s.rarp.Parameters.PSAPI_EnumProcessModules.cbNeeded);

  rarpGetLastError(s);
  rarpSend(s);

  if (big)
  {
    ragdSend(s, lphModule, cb);
    delete [] lphModule;
  }
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32OpenProcess(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.h = ::OpenProcess(s.rarp.Parameters.OpenProcess.dwDesiredAccess,
                                      s.rarp.Parameters.OpenProcess.bInheritHandle,
                                      s.rarp.Parameters.OpenProcess.dwProcessId);
  rarpGetLastError(s);
  rarpSend(s);
}


//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32ContinueDebugEvent(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::ContinueDebugEvent(s.rarp.Parameters.K32ContinueDebugEvent.dwProcessId,
                                           s.rarp.Parameters.K32ContinueDebugEvent.dwThreadId,
                                           s.rarp.Parameters.K32ContinueDebugEvent.dwContinueStatus);
  rarpGetLastError(s);
  rarpSend(s);

  DEBUG_PRINT(("ContinueDebugEvent(ProcessID=%ld, ThreadID=%ld, ContinueStatus=%ld);GLE=%ld\n", 
                  s.rarp.Parameters.ContinueDebugEvent.dwProcessId,
                  s.rarp.Parameters.ContinueDebugEvent.dwThreadId,
                  s.rarp.Parameters.ContinueDebugEvent.dwContinueStatus,
                  s.rarp.Win32LastError));
}


//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32VirtualQueryEx(tRemoteApiSession &s)
{
  DWORD nSize = s.rarp.Parameters.VirtualQueryEx.dwLength;

  // Different type of buffers? 
  //
  if (nSize != sizeof(MEMORY_BASIC_INFORMATION))
  {
    CHAR *p;
    p = new CHAR[nSize];

    s.rarp.ReturnValues.dw = ::VirtualQueryEx(
           s.rarp.Parameters.VirtualQueryEx.hProcess,
           s.rarp.Parameters.VirtualQueryEx.lpAddress,
           (PMEMORY_BASIC_INFORMATION) p,
           nSize);

    // send basic information
    rarpGetLastError(s);
    rarpSend(s);

    // send read buffer
    ragdSend(s, p, nSize);
    delete p;
  }
  else
  {
    s.rarp.ReturnValues.dw = ::VirtualQueryEx(
           s.rarp.Parameters.VirtualQueryEx.hProcess,
           s.rarp.Parameters.VirtualQueryEx.lpAddress,
           (PMEMORY_BASIC_INFORMATION) &s.rarp.Parameters.VirtualQueryEx.Buffer,
           nSize);
    // send basic information
    rarpGetLastError(s);
    rarpSend(s);
  }

  DEBUG_PRINT(("VirtualQueryEx(hProcess=%ld, lpAddress=%08lX, nSize=%08lX);GLE=%ld\n",
       s.rarp.Parameters.VirtualQueryEx.hProcess,
       s.rarp.Parameters.VirtualQueryEx.lpAddress,
       nSize,
       s.rarp.Win32LastError));
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32FlushInstructionCache(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::FlushInstructionCache(s.rarp.Parameters.FlushInstructionCache.hProcess,
                                              s.rarp.Parameters.FlushInstructionCache.lpBaseAddress,
                                              s.rarp.Parameters.FlushInstructionCache.dwSize);
  rarpGetLastError(s);
  rarpSend(s);

  DEBUG_PRINT(("FlushInstructionCache(hProcess=%ld, lpAddress=%08lX, dwSize=%ld);GLE=%ld",
       s.rarp.Parameters.FlushInstructionCache.hProcess,
       s.rarp.Parameters.FlushInstructionCache.lpBaseAddress,
       s.rarp.Parameters.FlushInstructionCache.dwSize,
       s.rarp.Win32LastError
  ));
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32WriteProcessMemory(tRemoteApiSession &s)
{
  DWORD nSize = s.rarp.Parameters.WriteProcessMemory.nSize;
  CHAR *p = NULL;
  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // receive buffer to write
    ragdReceiveAlloc(s, (LPVOID *)&p);
  }
  else
  {
    // use cache
    p = (CHAR *)&s.rarp.CacheBuffer;  
  }

  s.rarp.ReturnValues.b = ::WriteProcessMemory(
                         s.rarp.Parameters.WriteProcessMemory.hProcess,
                         s.rarp.Parameters.WriteProcessMemory.lpBaseAddress,
                         (LPVOID) p,
                         nSize,
                         &s.rarp.Parameters.WriteProcessMemory.NumberOfBytesWritten);
  // send response
  rarpGetLastError(s);
  rarpSend(s);

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // delete allocate block
    delete [] p;
  }
 
  DEBUG_PRINT(("WriteProcessMemory(hProcess=%ld, lpBaseAddress=%08lX, nSize=%ld, nbWritten=%ld);GLE=%ld",
             s.rarp.Parameters.WriteProcessMemory.hProcess,
             s.rarp.Parameters.WriteProcessMemory.lpBaseAddress,
             nSize,
             s.rarp.Parameters.WriteProcessMemory.NumberOfBytesWritten,
             s.rarp.Win32LastError

   ));   
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32WriteFile(tRemoteApiSession &s)
{
  DWORD nSize = s.rarp.Parameters.K32WriteFile.nNumberOfBytesToWrite;

  CHAR *p = NULL;

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // receive buffer to write
    ragdReceiveAlloc(s, (LPVOID *)&p);
  }
  else
  {
    // use cache
    p = (CHAR *)&s.rarp.CacheBuffer;  
  }

  s.rarp.ReturnValues.b = ::WriteFile(
    s.rarp.Parameters.K32WriteFile.hFile,
    (LPVOID) p,
    nSize,
    &s.rarp.Parameters.K32WriteFile.lpNumberOfBytesWritten,
    NULL);

  // send response
  rarpGetLastError(s);
  rarpSend(s);

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // delete allocate block
    delete [] p;
  }

  DEBUG_PRINT(("WriteFile(hFile=%ld, nSize=%ld, nbWritten=%ld);GLE=%ld",
    s.rarp.Parameters.K32WriteFile.hFile,
    nSize,
    s.rarp.Parameters.K32WriteFile.lpNumberOfBytesWritten,
    s.rarp.Win32LastError
    ));   
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32ReadFile(tRemoteApiSession &s)
{
  DWORD nSize = s.rarp.Parameters.K32ReadFile.nNumberOfBytesToRead;

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // allocate memory (+1 to assure there might not be overflow)
    CHAR *p = new CHAR[nSize+1];

    s.rarp.ReturnValues.b = ::ReadFile(s.rarp.Parameters.K32ReadFile.hFile,
      (LPVOID)p,
      nSize,
      &s.rarp.Parameters.K32ReadFile.lpNumberOfBytesRead,
      NULL);

    // Send first information
    rarpGetLastError(s);
    rarpSend(s);

    // Send Read buffer
    ragdSend(s, p, s.rarp.Parameters.K32ReadFile.lpNumberOfBytesRead);

    // delete allocate block
    delete p;
  }
  // buffer small? use the cache buffer instead!
  else
  {
    s.rarp.ReturnValues.b = ::ReadFile(s.rarp.Parameters.K32ReadFile.hFile,
      (LPVOID) &s.rarp.CacheBuffer,
      nSize,
      &s.rarp.Parameters.K32ReadFile.lpNumberOfBytesRead,
      NULL);

    // Send the header back
    rarpGetLastError(s);
    rarpSend(s);
  }

  //
  DEBUG_PRINT(("ReadFile(hFile=%ld, nSize=%ld, nbRead=%ld);GLE=%ld\n",
    s.rarp.Parameters.K32ReadFile.hFile,
    nSize,
    s.rarp.Parameters.K32ReadFile.lpNumberOfBytesRead,
    s.rarp.Win32LastError));   

}

void RApiServerK32DeviceIoControl(tRemoteApiSession &s)
{
  DWORD nSizeIn  = s.rarp.Parameters.K32DeviceIoControl.nInBufferSize;
  DWORD nSizeOut = s.rarp.Parameters.K32DeviceIoControl.nOutBufferSize;

  // Allocate memory for output buffer
  CHAR *pOut = new CHAR[nSizeOut+1];

  // Get the input buffer
  CHAR *pIn  = 0;

  if (nSizeIn != 0)
    ragdReceiveAlloc(s, (LPVOID *)&pIn);

  s.rarp.ReturnValues.b = ::DeviceIoControl(s.rarp.Parameters.K32DeviceIoControl.hDevice, 
    s.rarp.Parameters.K32DeviceIoControl.dwIoControlCode,
    nSizeIn == 0 ? NULL : pIn, nSizeIn,
    nSizeOut == 0 ? NULL : pOut, nSizeOut, 
    &s.rarp.Parameters.K32DeviceIoControl.lpBytesReturned,
    NULL);

  // Send first information
  rarpGetLastError(s);
  rarpSend(s);

  // Send the out buffer
  if (nSizeOut != 0)
    ragdSend(s, pOut, nSizeOut);

  if (nSizeIn != 0)
    delete [] pIn;

  delete [] pOut;
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32ReadProcessMemory(tRemoteApiSession &s)
{
  DWORD nSize = s.rarp.Parameters.ReadProcessMemory.nSize;

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // allocate memory (+1 to assure there might not be overflow)
    CHAR *p = new CHAR[nSize+1];

    s.rarp.ReturnValues.b = ::ReadProcessMemory(
                         s.rarp.Parameters.ReadProcessMemory.hProcess,
                         s.rarp.Parameters.ReadProcessMemory.lpBaseAddress,
                         (LPVOID) p,
                         nSize,
                         &s.rarp.Parameters.ReadProcessMemory.NumberOfBytesRead);

    // Send first information
    rarpGetLastError(s);
    rarpSend(s);

    // send read buffer
    ragdSend(s, p, nSize);

    // delete allocate block
    delete p;
  }
  // buffer small? use the cache buffer instead!
  else
  {
    s.rarp.ReturnValues.b = ::ReadProcessMemory(
                         s.rarp.Parameters.ReadProcessMemory.hProcess,
                         s.rarp.Parameters.ReadProcessMemory.lpBaseAddress,
                         (LPVOID) &s.rarp.CacheBuffer,
                         nSize,
                         &s.rarp.Parameters.ReadProcessMemory.NumberOfBytesRead);
    // send first information
    rarpGetLastError(s);
    rarpSend(s);
  }
  //
  DEBUG_PRINT(("ReadProcessMemory(hProcess=%ld, lpBaseAddress=%08lX, nSize=%ld, nbRead=%ld);GLE=%ld\n",
             s.rarp.Parameters.ReadProcessMemory.hProcess,
             s.rarp.Parameters.ReadProcessMemory.lpBaseAddress,
             nSize,
             s.rarp.Parameters.ReadProcessMemory.NumberOfBytesRead,
             s.rarp.Win32LastError

   ));   
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32SetThreadContext(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::SetThreadContext(s.rarp.Parameters.SetThreadContext.hThread,
                                         &s.rarp.Parameters.SetThreadContext.Context);
  rarpGetLastError(s);
  rarpSend(s);

  DEBUG_PRINT(("SetThreadContext(hThread=%ld);GLE=%ld\n", 
            s.rarp.Parameters.SetThreadContext.hThread,
            s.rarp.Win32LastError));
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32GetThreadContext(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::GetThreadContext(s.rarp.Parameters.GetThreadContext.hThread,
                                         &s.rarp.Parameters.GetThreadContext.Context);
  rarpGetLastError(s);
  rarpSend(s);

  //
  DEBUG_PRINT(("GetThreadContext(hThread=%ld);GLE=%ld\n", 
            s.rarp.Parameters.GetThreadContext.hThread,
            s.rarp.Win32LastError));

}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32TerminateThread(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::TerminateThread(s.rarp.Parameters.TerminateThread.hThread,
                                         s.rarp.Parameters.TerminateThread.uExitCode);
  rarpGetLastError(s);
  rarpSend(s);

  //
  DEBUG_PRINT(("TerminateThread(hThread=%ld, ExitCode=%ld);GLE=%ld\n", 
            s.rarp.Parameters.TerminateThread.hThread,
            s.rarp.Parameters.TerminateThread.uExitCode,
            s.rarp.Win32LastError));

}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32FindClose(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::FindClose(s.rarp.Parameters.K32FindClose.hFindFile);
  rarpGetLastError(s);
  rarpSend(s);

}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32CloseHandle(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::CloseHandle(s.rarp.Parameters.K32CloseHandle.hObject);
  rarpGetLastError(s);
  rarpSend(s);

  //
  DEBUG_PRINT(("CloseHandle(hObject=%ld);GLE=%ld\n", 
            s.rarp.Parameters.CloseHandle.hObject,
            s.rarp.Win32LastError));

}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32TerminateProcess(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::TerminateProcess(s.rarp.Parameters.K32TerminateProcess.hProcess,
                                         s.rarp.Parameters.K32TerminateProcess.uExitCode);
  rarpGetLastError(s);
  rarpSend(s);
  //
  DEBUG_PRINT(("TerminateProcess(hProcess=%ld, ExitCode=%ld);GLE=%ld\n", 
            s.rarp.Parameters.TerminateProcess.hProcess,
            s.rarp.Parameters.TerminateProcess.uExitCode,
            s.rarp.Win32LastError));
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32ResumeThread(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.dw = ::ResumeThread(s.rarp.Parameters.ResumeThread.hThread);
  rarpGetLastError(s);
  rarpSend(s);

  //
  DEBUG_PRINT(("ResumeThread(hThread=%ld);GLE=%ld\n", 
            s.rarp.Parameters.ResumeThread.hThread,
            s.rarp.Win32LastError));
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32DebugActiveProcess(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::DebugActiveProcess(s.rarp.Parameters.K32DebugActiveProcess.dwProcessId);
  rarpGetLastError(s);
  rarpSend(s);
}

//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32SuspendThread(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.dw = ::SuspendThread(s.rarp.Parameters.SuspendThread.hThread);
  rarpGetLastError(s);
  rarpSend(s);

  //
  DEBUG_PRINT(("SuspendThread(hThread=%ld);GLE=%ld\n", 
            s.rarp.Parameters.SuspendThread.hThread,
            s.rarp.Win32LastError));
}


//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32CreateProcess(tRemoteApiSession &s)
{
  /*
  static HLOCAL pSD = NULL;
  SECURITY_ATTRIBUTES sa;

  // left unallocated?
  if (pSD)
    LocalFree(pSD);

  
  // set return value = FALSE
  s.rarp.ReturnValues.b = FALSE;

  // Allocate memory for security descriptor
  pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
  if (pSD == NULL)
  {
    rarpSend(s);
    return;
  }

  // initialize descriptor
  if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
  {
    LocalFree((HLOCAL)pSD);
    pSD = NULL;
    rarpSend(s);
    return;
  }

  if (!SetSecurityDescriptorDacl(pSD, TRUE, (PACL)NULL, FALSE))  // all access
  {
    LocalFree((HLOCAL)pSD);
    pSD = NULL;
    rarpSend(s);
    return;
  }

  sa.nLength = sizeof(sa);
  sa.lpSecurityDescriptor = pSD;
  sa.bInheritHandle = FALSE;
*/


  CHAR *lpApplicationName = NULL;
  CHAR *lpCommandLine = NULL;
  CHAR *lpCurrentDirectory = NULL;
  CHAR *lpEnviron = NULL;

  // receive application name
  ragdReceiveAlloc(s, (LPVOID *)&lpApplicationName);

  // receive command line
  ragdReceiveAlloc(s, (LPVOID *)&lpCommandLine);

  // receive current directory
  ragdReceiveAlloc(s, (LPVOID *)&lpCurrentDirectory);

  // receive environment
  ragdReceiveAlloc(s, (LPVOID *)&lpEnviron);

  // Issue command
  s.rarp.ReturnValues.b = ::CreateProcessA(
        lpApplicationName,
        lpCommandLine,
        NULL,
        NULL,
        s.rarp.Parameters.CreateProcess.bInheritHandles,
        s.rarp.Parameters.CreateProcess.dwCreationFlags,
        NULL, // env
        lpCurrentDirectory,
        &s.rarp.Parameters.CreateProcess.StartupInfoA,
        &s.rarp.Parameters.CreateProcess.ProcessInformation);  

  rarpGetLastError(s);
  
  // send response
  rarpSend(s);

  //
  DEBUG_PRINT(("CreateProcess(\n\t"
   "lpApplication=%s\n\t"
   "Param=%s\n\t"
   "CurDir=%s\n\t"
   "hProcess=%ld, PID=%ld);GLE=%d\n",
             lpApplicationName,
             lpCommandLine,
             lpCurrentDirectory,
             s.rarp.Parameters.CreateProcess.ProcessInformation.hProcess,
             s.rarp.Parameters.CreateProcess.ProcessInformation.dwProcessId,
             s.rarp.Win32LastError));

  if (lpApplicationName)
    delete [] lpApplicationName;
  if (lpCommandLine)
    delete [] lpCommandLine;
  if (lpCurrentDirectory)
    delete [] lpCurrentDirectory;
  if (lpEnviron)
    delete [] lpEnviron;
}


//////////////////////////////////////////////////////////////////////////////////////////
void processCreateProcessDebugEvent(const LPDEBUG_EVENT pde)
{
  LPCREATE_PROCESS_DEBUG_INFO cpdi = (LPCREATE_PROCESS_DEBUG_INFO)&pde->u.CreateProcessInfo;

  DEBUG_PRINT(("DebugEvent:CreateProcess(hFile=%ld, hProcess=%ld)\n", cpdi->hFile, cpdi->hProcess));

}


//////////////////////////////////////////////////////////////////////////////////////////
void processLoadDllDebugEvent(const LPDEBUG_EVENT pde)
{
  LPLOAD_DLL_DEBUG_INFO lddi = (LPLOAD_DLL_DEBUG_INFO)&pde->u.LoadDll;

  DEBUG_PRINT(("DebugEvent:load_dll: base=%08lX hFile=%08lX\n", pde->u.LoadDll.lpBaseOfDll, pde->u.LoadDll.hFile));

  if (lddi->hFile == NULL)
    return;

  // Should the server get filename from file handle
  // and pass it to client ?
  if (::GetFileType(lddi->hFile) != FILE_TYPE_DISK)
  {
    // should clear hFile so that client don't get confused?
    // when it tries to get its caller a file handle ?
  }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// WaitForDebugEvent.
//
// It can post/pre process the specific debug event codes to it make sure that client/server
// transportation is sane!
//
void RApiServerK32WaitForDebugEvent(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::WaitForDebugEvent((LPDEBUG_EVENT) &s.rarp.Parameters.WaitForDebugEvent, 
                                          s.rarp.Parameters.WaitForDebugEvent.dwMilliseconds);
  rarpGetLastError(s);

  LPDEBUG_EVENT pde = (LPDEBUG_EVENT)&s.rarp.Parameters.WaitForDebugEvent.DebugEvent;
  switch (pde->dwDebugEventCode)
  {
    case LOAD_DLL_DEBUG_EVENT:
      processLoadDllDebugEvent(pde);
      break;
    case CREATE_PROCESS_DEBUG_EVENT:
      processCreateProcessDebugEvent(pde);
      break;
  }
  rarpSend(s);

  //DEBUG_PRINT(("WaitForDebugEvent.code=%d;GLE=%d", pde->dwDebugEventCode, s.rarp.Win32LastError));
}


//----------------------------------------------------------------------------------------
// user32!MessageBox()
void RApiServerU32MessageBox(tRemoteApiSession &s)
{
  LPTSTR p = (LPTSTR) s.rarp.CacheBuffer;
  LPTSTR lpText;
  LPTSTR lpCaption;

  multisz_retrieve(&p, &lpText);
  multisz_retrieve(&p, &lpCaption);

  s.rarp.ReturnValues.int_ = ::MessageBox(s.rarp.Parameters.U32MessageBox.hWnd, 
                                          lpText, lpCaption,
                                          s.rarp.Parameters.U32MessageBox.uType);
  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// k32.FindNextFile()
void RApiServerK32FindNextFile(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::FindNextFile(s.rarp.Parameters.K32FindNextFile.hFindFile, (LPWIN32_FIND_DATA)s.rarp.CacheBuffer);
  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// k32.FindFirstFile()
void RApiServerK32FindFirstFile(tRemoteApiSession &s)
{
  
  LPTSTR p = (LPTSTR) s.rarp.CacheBuffer;
  LPTSTR lpFileName;
  LPWIN32_FIND_DATA lpFindData;

  multisz_retrieve(&p, &lpFileName);
  lpFindData = (LPWIN32_FIND_DATA) p;

  s.rarp.ReturnValues.h = ::FindFirstFile(lpFileName, lpFindData);
  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// user32!FindWindow()
void RApiServerU32FindWindow(tRemoteApiSession &s)
{
  LPTSTR p = (LPTSTR) s.rarp.CacheBuffer;
  LPTSTR lpClassName, lpWindowName;

  multisz_retrieve(&p, &lpClassName);
  multisz_retrieve(&p, &lpWindowName);

  s.rarp.ReturnValues.hwnd = ::FindWindow(lpClassName, lpWindowName);
  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// kernel32!CreateFile
void RApiServerK32CreateFile(tRemoteApiSession &s)
{
 s.rarp.ReturnValues.h = ::CreateFileA(
     s.rarp.CacheBuffer,
     s.rarp.Parameters.K32CreateFile.dwDesiredAccess,
     s.rarp.Parameters.K32CreateFile.dwShareMode,
     NULL,
     s.rarp.Parameters.K32CreateFile.dwCreationDisposition,
     s.rarp.Parameters.K32CreateFile.dwFlagsAndAttributes,
     s.rarp.Parameters.K32CreateFile.hTemplateFile);

 rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// kernel32!GetFileSize
void RApiServerK32GetFileSize(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.dw = ::GetFileSize(s.rarp.Parameters.K32GetFileSize.hFile,
    &s.rarp.Parameters.K32GetFileSize.lpFileSizeHigh);
  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// kernel32!SetFilePointer
void RApiServerK32SetFilePointer(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.dw = ::SetFilePointer(s.rarp.Parameters.K32SetFilePointer.hFile, 
    s.rarp.Parameters.K32SetFilePointer.lDistanceToMove,
    &s.rarp.Parameters.K32SetFilePointer.lpDistanceToMoveHigh,
    s.rarp.Parameters.K32SetFilePointer.dwMoveMethod);

  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// kernel32!SetFilePointerEx
void RApiServerK32SetFilePointerEx(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::SetFilePointerEx(s.rarp.Parameters.K32SetFilePointerEx.hFile, 
    s.rarp.Parameters.K32SetFilePointerEx.liDistanceToMove,
    &s.rarp.Parameters.K32SetFilePointerEx.lpNewFilePointer,
    s.rarp.Parameters.K32SetFilePointerEx.dwMoveMethod);

  rarpGetLastError(s);
  rarpSend(s);
}

//----------------------------------------------------------------------------------------
// kernel32!WinExec()
void RApiServerK32WinExec(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::WinExec(s.rarp.CacheBuffer, s.rarp.Parameters.K32WinExec.uCmdShow);
  rarpGetLastError(s);
  rarpSend(s);
}

#ifdef __RAPI_USE_XP_FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////
void RApiServerK32DebugBreakProcess(tRemoteApiSession &s)
{
  s.rarp.ReturnValues.b = ::DebugBreakProcess(s.rarp.Parameters.K32DebugBreakProcess.Process);
  rarpGetLastError(s);
  rarpSend(s);
}

#endif