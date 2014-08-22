#include "remoteapi_client.h"

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

/////////////////////////////////////////////////////////////////////////////////////////
// K32.FindNextFile()
BOOL REMOTEAPICALL RApiClientK32FindNextFile(
  tRemoteApiSession &s,
  HANDLE hFindFile,
  LPWIN32_FIND_DATA lpFindFileData
)
{
  s.rarp.Cmd = RAC_K32FINDNEXTFILE;
  s.rarp.Parameters.K32FindNextFile.hFindFile = hFindFile;

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  memcpy(lpFindFileData, s.rarp.CacheBuffer , sizeof(WIN32_FIND_DATA));
  return s.rarp.ReturnValues.b;

}

/////////////////////////////////////////////////////////////////////////////////////////
// K32.FindFirstFile()
//
HANDLE REMOTEAPICALL RApiClientK32FindFirstFile(
  tRemoteApiSession &s,
  LPCTSTR lpFileName,
  LPWIN32_FIND_DATA lpFindFileData
)
{
  LPTSTR p = (LPTSTR)s.rarp.CacheBuffer;
  s.rarp.Cmd = RAC_K32FINDFIRSTFILE;

  multisz_store(&p, lpFileName);

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  memcpy(lpFindFileData, p, sizeof(WIN32_FIND_DATA));
  return s.rarp.ReturnValues.h; 
}

/////////////////////////////////////////////////////////////////////////////////////////
// U32.MessageBox()
//
int REMOTEAPICALL RApiClientU32MessageBox(
    tRemoteApiSession &s,
    HWND hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT uType
)
{
  LPTSTR p = (LPTSTR)s.rarp.CacheBuffer;
  s.rarp.Cmd = RAC_U32MESSAGEBOX;
  s.rarp.Parameters.U32MessageBox.hWnd = hWnd;
  s.rarp.Parameters.U32MessageBox.uType = uType;

  multisz_store(&p, lpText);
  multisz_store(&p, lpCaption);

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.int_; 
}

/////////////////////////////////////////////////////////////////////////////////////////
// FindWindow()
//
HWND REMOTEAPICALL RApiClientU32FindWindow(
    tRemoteApiSession &s,
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName
)
{
  LPTSTR p = (LPTSTR)s.rarp.CacheBuffer;
  s.rarp.Cmd = RAC_U32FINDWINDOW;
  multisz_store(&p, lpClassName);
  multisz_store(&p, lpWindowName);

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.hwnd;    
}

/////////////////////////////////////////////////////////////////////////////////////////
// WinExec()
//
BOOL REMOTEAPICALL RApiClientK32WinExec(
  tRemoteApiSession &s,
  LPCSTR lpCmdLine,
  UINT uCmdShow
)
{
  s.rarp.Cmd = RAC_K32WINEXEC;
  s.rarp.Parameters.K32WinExec.uCmdShow = uCmdShow;
  _tcscpy(s.rarp.CacheBuffer, lpCmdLine);

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;    
}

/////////////////////////////////////////////////////////////////////////////////////////
// DebugActiveProcess()
//
DWORD REMOTEAPICALL RApiClientK32DebugActiveProcess(
  tRemoteApiSession &s,
  DWORD dwProcessId
)
{
  s.rarp.Cmd = RAC_K32DEBUGACTIVEPROCESS;
  s.rarp.Parameters.K32DebugActiveProcess.dwProcessId = dwProcessId;

  rarpSend(s);
  rarpReceive(s);

  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;    
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL REMOTEAPICALL RApiClientEnumProcessModules(
  tRemoteApiSession &s,
  HANDLE hProcess,
  HMODULE* lphModule,
  DWORD cb,
  LPDWORD lpcbNeeded
)
{
  s.rarp.Cmd = RAC_PSAPIENUMPROCESSMODULES;
  s.rarp.Parameters.PSAPI_EnumProcessModules.cb = cb;
  s.rarp.Parameters.PSAPI_EnumProcessModules.hProcess = hProcess;

  rarpSend(s);
  rarpReceive(s);

  if (cb > RAPI_CACHE_BUFSIZE)
  {
    ragdReceive(s, lphModule, cb);
  }
  else
  {
    memcpy(lphModule, &s.rarp.CacheBuffer, cb);
  }
  *lpcbNeeded = s.rarp.Parameters.PSAPI_EnumProcessModules.cbNeeded;
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}

HANDLE REMOTEAPICALL RApiClientOpenProcess(
  tRemoteApiSession &s,
  DWORD dwDesiredAccess,
  BOOL bInheritHandle,
  DWORD dwProcessId
)
{
  s.rarp.Cmd = RAC_K32OPENPROCESS;
  s.rarp.Parameters.OpenProcess.dwDesiredAccess = dwDesiredAccess;
  s.rarp.Parameters.OpenProcess.bInheritHandle  = bInheritHandle;
  s.rarp.Parameters.OpenProcess.dwProcessId     = dwProcessId;

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.h;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL REMOTEAPICALL RApiClientEnumProcesses(
  tRemoteApiSession &s,
  DWORD* lpidProcess,
  DWORD cb,
  DWORD* cbNeeded
)
{
  s.rarp.Cmd = RAC_PSAPIENUMPROCESSES;
  s.rarp.Parameters.PSAPI_EnumProcesses.cb = cb;

  rarpSend(s);
  rarpReceive(s);

  // Buffer supplied bigger than cache size ?
  if (cb > RAPI_CACHE_BUFSIZE)
  {
    // receive the buffer
    ragdReceive(s, lpidProcess, cb);
  }
  else
  {
    // copy from cache buffer
    memcpy(lpidProcess, &s.rarp.CacheBuffer, cb);
  }

  *cbNeeded = s.rarp.Parameters.PSAPI_EnumProcesses.cbNeeded;

  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
DWORD REMOTEAPICALL RApiClientGetModuleBaseName(
  tRemoteApiSession &s,
  HANDLE hProcess,
  HMODULE hModule,
  LPTSTR lpBaseName,
  DWORD nSize
)
{
  //
  // Form basic packet
  //
  s.rarp.Cmd = RAC_PSAPIGETMODULEBASENAME;
  s.rarp.Parameters.PSAPI_GetModuleBaseName.hProcess = hProcess;
  s.rarp.Parameters.PSAPI_GetModuleBaseName.hModule  = hModule;
  s.rarp.Parameters.PSAPI_GetModuleBaseName.nSize    = nSize;
  rarpSend(s);

  //
  // Recevie basic response
  //
  rarpReceive(s);

  //
  // Receive module file name
  //
  memcpy(lpBaseName, s.rarp.CacheBuffer, nSize);

  rarpSetLastError(s);
  return s.rarp.ReturnValues.dw;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//
//
DWORD REMOTEAPICALL RApiClientGetModuleFileNameEx(
  tRemoteApiSession &s,
  HANDLE hProcess,    // handle to process
  HMODULE hModule,    // handle to module
  LPTSTR lpFilename,  // path buffer
  DWORD nSize)        // maximum characters to retrieve
{
  //
  // Form basic packet
  //
  s.rarp.Cmd = RAC_PSAPIGETMODULEFILENAMEEX;
  s.rarp.Parameters.PSAPI_GetModuleFileNameEx.hProcess = hProcess;
  s.rarp.Parameters.PSAPI_GetModuleFileNameEx.hModule  = hModule;
  s.rarp.Parameters.PSAPI_GetModuleFileNameEx.nSize    = nSize;
  rarpSend(s);

  //
  // Recevie basic response
  //
  rarpReceive(s);

  //
  // Receive module file name
  //
  memcpy(lpFilename, s.rarp.CacheBuffer, nSize);

  rarpSetLastError(s);
  return s.rarp.ReturnValues.dw;
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientContinueDebugEvent(
  tRemoteApiSession &s,
  DWORD dwProcessId,       // process to continue
  DWORD dwThreadId,        // thread to continue
  DWORD dwContinueStatus   // continuation status
)
{
  s.rarp.Cmd = RAC_K32CONTINUEDEBUGEVENT;
  s.rarp.Parameters.K32ContinueDebugEvent.dwProcessId = dwProcessId;
  s.rarp.Parameters.K32ContinueDebugEvent.dwThreadId  = dwThreadId;
  s.rarp.Parameters.K32ContinueDebugEvent.dwContinueStatus = dwContinueStatus;
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientVirtualQueryEx(
  tRemoteApiSession &s,
  HANDLE  hProcess,                    // handle to process
  LPCVOID lpAddress,                   // address of region
  PMEMORY_BASIC_INFORMATION lpBuffer,  // information buffer
  DWORD dwLength                       // size of buffer
)
{
  s.rarp.Cmd = RAC_K32VIRTUALQUERYEX;
  s.rarp.Parameters.VirtualQueryEx.hProcess = hProcess;
  s.rarp.Parameters.VirtualQueryEx.dwLength = dwLength;
  s.rarp.Parameters.VirtualQueryEx.lpAddress = lpAddress;
  rarpSend(s);

  rarpReceive(s);

  // Only if lpBuffer is of different type then request to get that buffer
  //
  if (dwLength != sizeof(MEMORY_BASIC_INFORMATION))
  {
    // receibe memory information buffer
    ragdReceive(s, lpBuffer, dwLength);
  }
  else
  {
    memcpy(lpBuffer, (LPVOID)&s.rarp.Parameters.VirtualQueryEx.Buffer, dwLength);
  }
  rarpSetLastError(s);
  return s.rarp.ReturnValues.dw;
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientFlushInstructionCache(
  tRemoteApiSession &s,
  HANDLE hProcess,        // handle to process 
  LPCVOID lpBaseAddress,  // base of region to flush
  DWORD dwSize            // length of region to flush
)
{
  s.rarp.Cmd = RAC_K32FLUSHINSTRUCTIONCACHE;
  s.rarp.Parameters.FlushInstructionCache.hProcess = hProcess;
  s.rarp.Parameters.FlushInstructionCache.lpBaseAddress = lpBaseAddress;
  s.rarp.Parameters.FlushInstructionCache.dwSize = dwSize;
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}

HANDLE REMOTEAPICALL RApiClientCreateFile(
  tRemoteApiSession &s,
  LPCTSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  HANDLE hTemplateFile)
{
  // if buffer too small then use cache
  if (
      (lpFileName  == 0) || 
      (lpSecurityAttributes != 0) ||
      (_tcslen(lpFileName) > RAPI_CACHE_BUFSIZE)
     )
  {
    ::SetLastError(ERROR_INVALID_PARAMETER);
    return INVALID_HANDLE_VALUE;
  }

  _tcscpy((TCHAR *)s.rarp.CacheBuffer, lpFileName);

  // Send request
  s.rarp.Cmd = RAC_K32CREATEFILE;
  s.rarp.Parameters.K32CreateFile.dwDesiredAccess = dwDesiredAccess;
  s.rarp.Parameters.K32CreateFile.dwShareMode = dwShareMode;
  s.rarp.Parameters.K32CreateFile.dwCreationDisposition = dwCreationDisposition;
  s.rarp.Parameters.K32CreateFile.dwFlagsAndAttributes = dwFlagsAndAttributes;
  s.rarp.Parameters.K32CreateFile.hTemplateFile = hTemplateFile;

  rarpSend(s);

  // receive response
  rarpReceive(s);

  rarpSetLastError(s);
  return s.rarp.ReturnValues.h;
}

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientGetFileSize(
  tRemoteApiSession &s,
  HANDLE hFile,
  LPDWORD lpFileSizeHigh)
{
  s.rarp.Cmd = RAC_K32GETFILESIZE;
  s.rarp.Parameters.K32GetFileSize.hFile = hFile;
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);

  if (lpFileSizeHigh != NULL)
    *lpFileSizeHigh = s.rarp.Parameters.K32GetFileSize.lpFileSizeHigh;

  return s.rarp.ReturnValues.b;  
}

BOOL REMOTEAPICALL RApiClientDeviceIoControl(
  tRemoteApiSession &s,
  HANDLE hDevice,
  DWORD dwIoControlCode,
  LPVOID lpInBuffer,
  DWORD nInBufferSize,
  LPVOID lpOutBuffer,
  DWORD nOutBufferSize,
  LPDWORD lpBytesReturned,
  LPOVERLAPPED lpOverlapped)
{
  if (lpOverlapped != NULL)
  {
    ::SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
  }

  s.rarp.Cmd = RAC_K32DEVICEIOCONTROL;
  s.rarp.Parameters.K32DeviceIoControl.hDevice= hDevice;
  s.rarp.Parameters.K32DeviceIoControl.dwIoControlCode = dwIoControlCode;
  s.rarp.Parameters.K32DeviceIoControl.nInBufferSize = nInBufferSize;
  s.rarp.Parameters.K32DeviceIoControl.nOutBufferSize = nOutBufferSize;

  // Send request
  rarpSend(s);

  // Conditionally send input buffer
  if (nInBufferSize != 0)
    ragdSend(s, lpInBuffer, nInBufferSize);

  // Get response
  rarpReceive(s);

  // Conditionally receive output buffer size
  if (nOutBufferSize != 0)
    ragdReceive(s, lpOutBuffer, nOutBufferSize);

  rarpSetLastError(s);

  return s.rarp.ReturnValues.b;
}

BOOL REMOTEAPICALL RApiClientSetFilePointerEx(
  tRemoteApiSession &s,
  HANDLE hFile,
  LARGE_INTEGER liDistanceToMove,
  PLARGE_INTEGER lpNewFilePointer,
  DWORD dwMoveMethod)
{
  s.rarp.Cmd = RAC_K32SETFILEPOINTEREX;
  s.rarp.Parameters.K32SetFilePointerEx.hFile = hFile;
  s.rarp.Parameters.K32SetFilePointerEx.liDistanceToMove = liDistanceToMove;
  s.rarp.Parameters.K32SetFilePointerEx.dwMoveMethod = dwMoveMethod;

  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);

  if (lpNewFilePointer != NULL)
    s.rarp.Parameters.K32SetFilePointerEx.lpNewFilePointer = *lpNewFilePointer;

  return s.rarp.ReturnValues.b;
}

DWORD REMOTEAPICALL RApiClientSetFilePointer(
  tRemoteApiSession &s,
  HANDLE hFile,
  LONG lDistanceToMove,
  PLONG lpDistanceToMoveHigh,
  DWORD dwMoveMethod)
{
  s.rarp.Cmd = RAC_K32SETFILEPOINTER;
  s.rarp.Parameters.K32SetFilePointer.hFile = hFile;
  s.rarp.Parameters.K32SetFilePointer.lDistanceToMove = lDistanceToMove;
  s.rarp.Parameters.K32SetFilePointer.dwMoveMethod = dwMoveMethod;

  if (lpDistanceToMoveHigh == NULL)
    s.rarp.Parameters.K32SetFilePointer.lpDistanceToMoveHigh = 0;
  else
    s.rarp.Parameters.K32SetFilePointer.lpDistanceToMoveHigh = *lpDistanceToMoveHigh;

  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);

  return s.rarp.ReturnValues.dw;  
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientWriteFile(
  tRemoteApiSession &s,
  HANDLE hFile,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  LPOVERLAPPED lpOverlapped)
{

  // Overlapped operations are not allowed
  if (lpOverlapped != NULL)
  {
    ::SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
  }

  // if buffer too small then use cache
  if (nNumberOfBytesToWrite < RAPI_CACHE_BUFSIZE)
    memcpy((LPVOID)&s.rarp.CacheBuffer, lpBuffer, nNumberOfBytesToWrite);

  // send request
  s.rarp.Cmd = RAC_K32WRITEFILE;
  s.rarp.Parameters.K32WriteFile.hFile = hFile;
  s.rarp.Parameters.K32WriteFile.nNumberOfBytesToWrite = nNumberOfBytesToWrite;

  rarpSend(s);

  if (nNumberOfBytesToWrite > RAPI_CACHE_BUFSIZE)
  {
    // Send data buffer
    ragdSend(s, (LPVOID) lpBuffer, nNumberOfBytesToWrite);
  }

  // receive response
  rarpReceive(s);
  *lpNumberOfBytesWritten = s.rarp.Parameters.K32WriteFile.lpNumberOfBytesWritten;

  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientWriteProcessMemory(
  tRemoteApiSession &s,
  HANDLE hProcess,               // handle to process
  LPVOID lpBaseAddress,          // base of memory area
  LPVOID lpBuffer,               // data buffer
  DWORD nSize,                   // number of bytes to write
  LPDWORD lpNumberOfBytesWritten // number of bytes written
)
{
  // if buffer too small then use cache
  if (nSize < RAPI_CACHE_BUFSIZE)
  {
    memcpy((LPVOID)&s.rarp.CacheBuffer, lpBuffer, nSize);
  }

  // send request
  s.rarp.Cmd = RAC_K32WRITEPROCESSMEMORY;
  s.rarp.Parameters.WriteProcessMemory.hProcess = hProcess;
  s.rarp.Parameters.WriteProcessMemory.lpBaseAddress = lpBaseAddress;
  s.rarp.Parameters.WriteProcessMemory.nSize = nSize;
  rarpSend(s);

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // send data buffer
    ragdSend(s, lpBuffer, nSize);
  }

  // receive response
  rarpReceive(s);
  *lpNumberOfBytesWritten = s.rarp.Parameters.WriteProcessMemory.NumberOfBytesWritten;

  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientReadFile(
  tRemoteApiSession &s,
  HANDLE hFile,
  LPVOID lpBuffer,
  DWORD nNumberOfBytesToRead,
  LPDWORD lpNumberOfBytesRead,
  LPOVERLAPPED lpOverlapped)
{
  // Overlapped operations are not allowed
  if (lpOverlapped != NULL)
  {
    ::SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
  }

  s.rarp.Cmd = RAC_K32READFILE;

  s.rarp.Parameters.K32ReadFile.hFile = hFile;
  s.rarp.Parameters.K32ReadFile.nNumberOfBytesToRead = nNumberOfBytesToRead;

  rarpSend(s);

  // receive the response first
  rarpReceive(s);

  // Transfer read buffer and other variables to the user

  *lpNumberOfBytesRead = s.rarp.Parameters.K32ReadFile.lpNumberOfBytesRead;

  if (nNumberOfBytesToRead > RAPI_CACHE_BUFSIZE)
  {
    // now receive the read buffer 
    ragdReceive(s, lpBuffer, *lpNumberOfBytesRead);
  }
  else
  {
    memcpy(lpBuffer, (LPVOID)&s.rarp.CacheBuffer, nNumberOfBytesToRead);
  }
  rarpSetLastError(s);

  return s.rarp.ReturnValues.b;
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientReadProcessMemory(
  tRemoteApiSession &s,
  HANDLE hProcess,             // handle to the process
  LPCVOID lpBaseAddress,       // base of memory area
  LPVOID lpBuffer,             // data buffer
  DWORD nSize,                 // number of bytes to read
  LPDWORD lpNumberOfBytesRead  // number of bytes read
)
{
  s.rarp.Cmd = RAC_K32READPROCESSMEMORY;
  s.rarp.Parameters.ReadProcessMemory.hProcess = hProcess;
  s.rarp.Parameters.ReadProcessMemory.lpBaseAddress = lpBaseAddress;
  s.rarp.Parameters.ReadProcessMemory.nSize = nSize;
  rarpSend(s);

  // receive the response first
  rarpReceive(s);
  
  // transfer read buffer and other variables to the user
  *lpNumberOfBytesRead = s.rarp.Parameters.ReadProcessMemory.NumberOfBytesRead;

  if (nSize > RAPI_CACHE_BUFSIZE)
  {
    // now receive the read buffer 
    ragdReceive(s, lpBuffer, nSize);
  }
  else
  {
    memcpy(lpBuffer, (LPVOID)&s.rarp.CacheBuffer, nSize);
  }
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientSetThreadContext(
  tRemoteApiSession &s,
  HANDLE hThread,            // handle to thread
  CONST CONTEXT *lpContext   // context structure
)
{
  s.rarp.Cmd = RAC_K32SETTHREADCONTEXT;
  s.rarp.Parameters.SetThreadContext.hThread = hThread;
  memcpy((LPVOID)&s.rarp.Parameters.SetThreadContext.Context, (LPVOID)lpContext, sizeof(CONTEXT));
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientGetThreadContext(
  tRemoteApiSession &s,
  HANDLE hThread,       // handle to thread with context
  LPCONTEXT lpContext   // context structure
)
{
  s.rarp.Cmd = RAC_K32GETTHREADCONTEXT;
  s.rarp.Parameters.GetThreadContext.hThread = hThread;
  memcpy((LPVOID)&s.rarp.Parameters.GetThreadContext.Context, (LPVOID)lpContext, sizeof(CONTEXT));
  rarpSend(s);


  rarpReceive(s);
  memcpy((LPVOID)lpContext,(LPVOID)&s.rarp.Parameters.GetThreadContext.Context, sizeof(CONTEXT));
  rarpSetLastError(s);

  return s.rarp.ReturnValues.b;
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientK32FindClose(
  tRemoteApiSession &s,
  HANDLE hFindFile   // handle to object
)
{
  s.rarp.Cmd = RAC_K32FINDCLOSE;
  s.rarp.Parameters.K32FindClose.hFindFile = hFindFile;
  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}


/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientCloseHandle(
  tRemoteApiSession &s,
  HANDLE hObject)   // handle to object
{
  s.rarp.Cmd = RAC_K32CLOSEHANDLE;
  s.rarp.Parameters.K32CloseHandle.hObject = hObject;
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//
//
BOOL REMOTEAPICALL RApiClientTerminateThread(
  tRemoteApiSession &s,
  HANDLE hThread, // handle to thread
  UINT uExitCode   // exit
)
{
  s.rarp.Cmd = RAC_K32TERMINATETHREAD;
  s.rarp.Parameters.TerminateThread.hThread   = hThread;
  s.rarp.Parameters.TerminateThread.uExitCode = uExitCode;
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientTerminateProcess(
  tRemoteApiSession &s,
  HANDLE hProcess, // handle to the process
  UINT uExitCode   // exit code for the process
)
{
  s.rarp.Cmd = RAC_K32TERMINATEPROCESS;
  s.rarp.Parameters.K32TerminateProcess.hProcess = hProcess;
  s.rarp.Parameters.K32TerminateProcess.uExitCode = uExitCode;
  rarpSend(s);

  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;  
}

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientResumeThread(
  tRemoteApiSession &s,
  HANDLE hThread   // handle to thread
)
{
  s.rarp.Cmd = RAC_K32RESUMETHREAD;
  s.rarp.Parameters.ResumeThread.hThread = hThread;
  rarpSend(s);
  
  rarpReceive(s);

  rarpSetLastError(s);
  return s.rarp.ReturnValues.dw;    
}

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientSuspendThread(
  tRemoteApiSession &s,
  HANDLE hThread   // handle to thread
)
{
  s.rarp.Cmd = RAC_K32SUSPENDTHREAD;
  s.rarp.Parameters.SuspendThread.hThread = hThread;
  rarpSend(s);
  
  rarpReceive(s);

  rarpSetLastError(s);
  return s.rarp.ReturnValues.dw;    
}


/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientCreateProcess(
  tRemoteApiSession &s,
  LPCTSTR lpApplicationName,                 // name of executable module
  LPTSTR lpCommandLine,                      // command line string
  LPSECURITY_ATTRIBUTES lpProcessAttributes, // SD
  LPSECURITY_ATTRIBUTES lpThreadAttributes,  // SD
  BOOL bInheritHandles,                      // handle inheritance option
  DWORD dwCreationFlags,                     // creation flags
  LPVOID lpEnvironment,                      // new environment block
  LPCTSTR lpCurrentDirectory,                // current directory name
  LPSTARTUPINFO lpStartupInfo,               // startup information
  LPPROCESS_INFORMATION lpProcessInformation // process information
)
{
  /*
    Security Attributes are not supported yet.
    They have variable size! therefore I must code them correctly thereafter
  */

  DWORD nLen;

  // security attributes are ignored at the moment

  //
  // prepare/send request block  
  //
  s.rarp.Cmd = RAC_K32CREATEPROCESS;
  s.rarp.Parameters.CreateProcess.bInheritHandles = bInheritHandles;
  s.rarp.Parameters.CreateProcess.dwCreationFlags = dwCreationFlags;

  // ----------------------------------------------
  // Startup INFO is not portable between machines/sessions!!!! make sure you clear it
  // and re-initialize it!!!!!!!!
  //
  LPSTARTUPINFOA psi = (LPSTARTUPINFOA)&s.rarp.Parameters.CreateProcess.StartupInfoA;
  memset(psi, 0, sizeof(STARTUPINFOA));
  psi->cb = sizeof(STARTUPINFOA);

  memcpy((LPVOID)&s.rarp.Parameters.CreateProcess.ProcessInformation, 
         lpProcessInformation, 
         sizeof(PROCESS_INFORMATION));

  rarpSend(s);

  //
  // Send application name
  //
  if (lpApplicationName)
  {
    nLen = strlen(lpApplicationName) + 1;
    ragdSend(s, (LPVOID)lpApplicationName, nLen);
  }
  else
  {
    ragdSend(s, NULL, 0);
  }
  //
  // send command line
  //
  if (lpCommandLine)
  {
    nLen = strlen(lpCommandLine) + 1;
    ragdSend(s, (LPVOID)lpCommandLine, nLen);
  }
  else
  {
    ragdSend(s, NULL, 0);
  }

  // 
  // send current directory
  //


  // directory specified is NULL means current directory => pass it to server
  if (!lpCurrentDirectory)
  {
    //CHAR szCurrentDirectory[MAX_PATH];
    //GetCurrentDirectory(sizeof(szCurrentDirectory), szCurrentDirectory);
    ragdSend(s, NULL, 4);
  }
  else
  {
    nLen = strlen(lpCurrentDirectory) + 1; 
    ragdSend(s, (LPVOID)lpCurrentDirectory, nLen);
  }

  //
  // send environment string
  //
  LPSTR pEnvironment;
  // NULL means to use caller's environment string...therefore get it and pass it to server
  if (!lpEnvironment)
  {
    //pEnvironment = GetEnvironmentStrings();
    // total length of environment string
    //nLen = (wcslen((wchar_t *)pEnvironment)+1) * 2;
    pEnvironment = NULL;
    nLen = 4;
  }    
  else
  {
    // send env string as supplied by user
    nLen = (wcslen((wchar_t *)lpEnvironment)+1) * 2;
    pEnvironment = (LPSTR)lpEnvironment;
  }
  ragdSend(s, (LPVOID)pEnvironment, nLen);

  //
  // Receive basic response
  //
  rarpReceive(s);
  
  //
  // copy back vital data to caller
  //

  // copy STARTUPINFO
  memcpy(lpStartupInfo, 
         (LPVOID)&s.rarp.Parameters.CreateProcess.StartupInfoA,
         sizeof(STARTUPINFOA));

  // copy PROCESS_INFORMATION
  memcpy(lpProcessInformation, 
         (LPVOID)&s.rarp.Parameters.CreateProcess.ProcessInformation, 
         sizeof(PROCESS_INFORMATION));

  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;    
}


/////////////////////////////////////////////////////////////////////////////////////////
int RApiClient_Init(tRemoteApiSession &s, const char *pcHost, int nPort)
{
  u_long nRemoteAddress = LookupAddress(pcHost);
  if (nRemoteAddress == INADDR_NONE)
    return 1;

  memset((LPVOID)&s, 0, sizeof(tRemoteApiSession));

  s.sd = EstablishConnection(nRemoteAddress, htons(nPort));

  if (s.sd == INVALID_SOCKET)
    return 2;

  s.rarp.Signature = REMOTEAPI_SIG;
  return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
void RApiClient_Close(tRemoteApiSession &s)
{
  ShutdownConnection(s.sd);
  // invalidate socket handle
  s.sd = INVALID_SOCKET;
}


/////////////////////////////////////////////////////////////////////////////////////////
void RApiClient_CloseServer(tRemoteApiSession &s)
{
  s.rarp.Cmd = RAC_SERVER_STOP;
  rarpSend(s);
}

/////////////////////////////////////////////////////////////////////////////////////////
void RApiClient_RestartServer(tRemoteApiSession &s)
{
  s.rarp.Cmd = RAC_SERVER_RESTART;
  rarpSend(s);
}



/////////////////////////////////////////////////////////////////////////////////////////
void processCreateProcessDebugEvent(const LPDEBUG_EVENT pde)
{

  LPCREATE_PROCESS_DEBUG_INFO cpdi = (LPCREATE_PROCESS_DEBUG_INFO)&pde->u.CreateProcessInfo;

  cpdi->hFile = NULL;

  /*
  CHAR buf[1024];
  if (!GetRemoteDebugeeModuleName(session,
                                  cpdi->hProcess, 
                                  (LPVOID)cpdi->lpImageName, 
                                  cpdi->fUnicode, 
                                  buf, sizeof(buf)))
    return;
  
  DEBUG_PRINT(
    printf("WFDE->CreateProcess(hFile=%ld, hProcess=%ld, \n\tImageName=%s)\n", cpdi->hFile, cpdi->hProcess, buf);
  );

  // Create the file on the client side too
  // this is to save me the trouble of also remoting File I/O API!
  // All files operations must be done on the debugger side
  CreateFile(buf,
             GENERIC_READ,
             FILE_SHARE_READ,
             NULL,
             OPEN_EXISTING,
             NULL,
             NULL);
  */
}


/////////////////////////////////////////////////////////////////////////////////////////
void processLoadDllDebugEvent(const LPDEBUG_EVENT pde)
{
  LPLOAD_DLL_DEBUG_INFO lddi = (LPLOAD_DLL_DEBUG_INFO)&pde->u.LoadDll;

  // return a NULL to the file handle!
  lddi->hFile = NULL;

/*
  CHAR buf[1024];
  if (!GetRemoteDebugeeModuleName(g_hProcess, 
                                  (LPVOID)lddi->lpImageName, 
                                  lddi->fUnicode, 
                                  buf, sizeof(buf)))
    return;
  
  // Create the file on the client side too
  // this is to save me the trouble of also remoting File I/O API!
  // All files operations must be done on the debugger side
  CreateFile(buf,
             GENERIC_READ,
             FILE_SHARE_READ,
             NULL,
             OPEN_EXISTING,
             NULL,
             NULL);
  */
}


/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientWaitForDebugEvent(
  tRemoteApiSession &s,
  LPDEBUG_EVENT lpDebugEvent,  // debug event information
  DWORD dwMilliseconds         // time-out value
)
{
  LPDEBUG_EVENT pde = (LPDEBUG_EVENT)&s.rarp.Parameters.WaitForDebugEvent.DebugEvent;

  s.rarp.Cmd = RAC_K32WAITFORDEBUGEVENT;
  s.rarp.Parameters.WaitForDebugEvent.dwMilliseconds = dwMilliseconds;
  rarpSend(s);

  rarpReceive(s);

  memcpy(lpDebugEvent, (LPVOID)&s.rarp.Parameters.WaitForDebugEvent.DebugEvent, sizeof(DEBUG_EVENT));

  switch (pde->dwDebugEventCode)
  {
    case LOAD_DLL_DEBUG_EVENT:
      processLoadDllDebugEvent(lpDebugEvent);
      break;
    case CREATE_PROCESS_DEBUG_EVENT:
      processCreateProcessDebugEvent(lpDebugEvent);
      break;
    case EXIT_PROCESS_DEBUG_EVENT:
      break;
  }
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}


#ifdef RAPI_XP_FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL REMOTEAPICALL RApiClientK32DebugBreakProcess(
  tRemoteApiSession &s,
  HANDLE Process
)
{
  s.rarp.Cmd = RAC_K32DEBUGBREAKPROCESS;
  s.rarp.Parameters.K32DebugBreakProcess.Process = Process;

  rarpSend(s);
  rarpReceive(s);
  rarpSetLastError(s);
  return s.rarp.ReturnValues.b;
}
#endif
