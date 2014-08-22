#ifndef REMOTEAPI_CLIENT__INC
#define REMOTEAPI_CLIENT__INC

#include "../libraries/socks_common.h"
#include "../libraries/string_util.h"
#include "remoteapi.h"


/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientK32FindNextFile(
  tRemoteApiSession &s,
  HANDLE hFindFile,
  LPWIN32_FIND_DATA lpFindFileData
);

/////////////////////////////////////////////////////////////////////////////////////////
HANDLE REMOTEAPICALL RApiClientK32FindFirstFile(
  tRemoteApiSession &s,
  LPCTSTR lpFileName,
  LPWIN32_FIND_DATA lpFindFileData
);

/////////////////////////////////////////////////////////////////////////////////////////
int REMOTEAPICALL RApiClientU32MessageBox(
    tRemoteApiSession &s,
    HWND hWnd,
    LPCTSTR lpText,
    LPCTSTR lpCaption,
    UINT uType
);

/////////////////////////////////////////////////////////////////////////////////////////
void REMOTEAPICALL RApiClientK32DebugBreak();

/////////////////////////////////////////////////////////////////////////////////////////
HWND REMOTEAPICALL RApiClientU32FindWindow(
    tRemoteApiSession &s,
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName
);


/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientK32WinExec(
  tRemoteApiSession &s,
  LPCSTR lpCmdLine,
  UINT uCmdShow
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientK32DebugBreakProcess(
  tRemoteApiSession &s,
  HANDLE Process
);

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientK32DebugActiveProcess(
  tRemoteApiSession &s,
  DWORD dwProcessId
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientEnumProcessModules(
  tRemoteApiSession &s,
  HANDLE hProcess,
  HMODULE* lphModule,
  DWORD cb,
  LPDWORD lpcbNeeded
);

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientGetModuleBaseName(
  tRemoteApiSession &s,
  HANDLE hProcess,
  HMODULE hModule,
  LPTSTR lpBaseName,
  DWORD nSize
);

/////////////////////////////////////////////////////////////////////////////////////////
HANDLE REMOTEAPICALL RApiClientOpenProcess(
  tRemoteApiSession &s,
  DWORD dwDesiredAccess,
  BOOL bInheritHandle,
  DWORD dwProcessId
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientEnumProcesses(
  tRemoteApiSession &s,
  DWORD* lpidProcess,
  DWORD cb,
  DWORD* cbNeeded
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientWaitForDebugEvent(
  tRemoteApiSession &s,
  LPDEBUG_EVENT lpDebugEvent,  // debug event information
  DWORD dwMilliseconds         // time-out value
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientK32FindClose(
  tRemoteApiSession &s,
  HANDLE hFindFile   // handle to object
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientCloseHandle(
  tRemoteApiSession &s,
  HANDLE hObject   // handle to object
);

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientGetModuleFileNameEx(
  tRemoteApiSession &s,
  HANDLE hProcess,    // handle to process
  HMODULE hModule,    // handle to module
  LPTSTR lpFilename,  // path buffer
  DWORD nSize         // maximum characters to retrieve
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientContinueDebugEvent(
  tRemoteApiSession &s,
  DWORD dwProcessId,       // process to continue
  DWORD dwThreadId,        // thread to continue
  DWORD dwContinueStatus   // continuation status
);

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientVirtualQueryEx(
  tRemoteApiSession &s,
  HANDLE hProcess,                     // handle to process
  LPCVOID lpAddress,                   // address of region
  PMEMORY_BASIC_INFORMATION lpBuffer,  // information buffer
  DWORD dwLength                       // size of buffer
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientFlushInstructionCache(
  tRemoteApiSession &s,
  HANDLE hProcess,        // handle to process 
  LPCVOID lpBaseAddress,  // base of region to flush
  DWORD dwSize            // length of region to flush
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientWriteProcessMemory(
  tRemoteApiSession &s,
  HANDLE hProcess,               // handle to process
  LPVOID lpBaseAddress,          // base of memory area
  LPVOID lpBuffer,               // data buffer
  DWORD nSize,                   // number of bytes to write
  LPDWORD lpNumberOfBytesWritten // number of bytes written
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientReadProcessMemory(
  tRemoteApiSession &s,
  HANDLE hProcess,             // handle to the process
  LPCVOID lpBaseAddress,       // base of memory area
  LPVOID lpBuffer,             // data buffer
  DWORD nSize,                 // number of bytes to read
  LPDWORD lpNumberOfBytesRead  // number of bytes read
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientSetThreadContext(
  tRemoteApiSession &s,
  HANDLE hThread,            // handle to thread
  CONST CONTEXT *lpContext   // context structure
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientGetThreadContext(
  tRemoteApiSession &s,
  HANDLE hThread,       // handle to thread with context
  LPCONTEXT lpContext   // context structure
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientTerminateThread(
  tRemoteApiSession &s,
  HANDLE hThread, // handle to thread
  UINT uExitCode   // exit
);

/////////////////////////////////////////////////////////////////////////////////////////
BOOL REMOTEAPICALL RApiClientTerminateProcess(
  tRemoteApiSession &s,
  HANDLE hProcess, // handle to the process
  UINT uExitCode   // exit code for the process
);

HANDLE REMOTEAPICALL RApiClientCreateFile(
  tRemoteApiSession &s,
  LPCTSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  HANDLE hTemplateFile
);

DWORD REMOTEAPICALL RApiClientGetFileSize(
  tRemoteApiSession &s,
  HANDLE hFile,
  LPDWORD lpFileSizeHigh
);

BOOL REMOTEAPICALL RApiClientSetFilePointerEx(
  tRemoteApiSession &s,
  HANDLE hFile,
  LARGE_INTEGER liDistanceToMove,
  PLARGE_INTEGER lpNewFilePointer,
  DWORD dwMoveMethod
);

BOOL REMOTEAPICALL RApiClientDeviceIoControl(
  tRemoteApiSession &s,
  HANDLE hDevice,
  DWORD dwIoControlCode,
  LPVOID lpInBuffer,
  DWORD nInBufferSize,
  LPVOID lpOutBuffer,
  DWORD nOutBufferSize,
  LPDWORD lpBytesReturned,
  LPOVERLAPPED lpOverlapped
);

DWORD REMOTEAPICALL RApiClientSetFilePointer(
  tRemoteApiSession &s,
  HANDLE hFile,
  LONG lDistanceToMove,
  PLONG lpDistanceToMoveHigh,
  DWORD dwMoveMethod);

BOOL REMOTEAPICALL RApiClientWriteFile(
  tRemoteApiSession &s,
  HANDLE hFile,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  LPOVERLAPPED lpOverlapped
);

BOOL REMOTEAPICALL RApiClientReadFile(
  tRemoteApiSession &s,
  HANDLE hFile,
  LPVOID lpBuffer,
  DWORD nNumberOfBytesToRead,
  LPDWORD lpNumberOfBytesRead,
  LPOVERLAPPED lpOverlapped
);

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientResumeThread(
  tRemoteApiSession &s,
  HANDLE hThread   // handle to thread
);

/////////////////////////////////////////////////////////////////////////////////////////
DWORD REMOTEAPICALL RApiClientSuspendThread(
  tRemoteApiSession &s,
  HANDLE hThread   // handle to thread
);

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
);


/////////////////////////////////////////////////////////////////////////////////////////
int  RApiClient_Init(tRemoteApiSession &s, LPCTSTR pcHost, int nPort);
void RApiClient_Close(tRemoteApiSession &s);
void RApiClient_CloseServer(tRemoteApiSession &s);
void RApiClient_RestartServer(tRemoteApiSession &s);

#endif