#ifndef __REMOTEAPI_INC
#define __REMOTEAPI_INC

#include <winsock2.h>
#include <windows.h>
#include <tchar.h>

#include "../libraries/socks_common.h"

#define RAPI_CACHE_BUFSIZE 7168

// generic transport data type used with tRemoteApiGenericData
typedef enum
{
  RDDT_GENERIC
} eRemoteApiDataTypes;


// defines base of command codes for different libraries
#define RACBASE_APISERVER          0x10000
#define RACBASE_KERNEL32           0x20000
#define RACBASE_APISERVERUTIL      0x31000
#define RACBASE_PSAPI              0x32000
#define RACBASE_USER32             0x33000

// Different command code values
typedef enum
{
  //
  // Server commands
  //
  RAC_APISERVER_BASE = RACBASE_APISERVER,
  RAC_SERVER_STOP,                      // server: Stop server
  RAC_SERVER_RESTART,                   // server: Restart server
  RAC_SERVER_MODULESCOUT,               // server: Scout module names and their handles
  RAC_SERVER_GETVERSION,                // server: Returns version of server and Operating system
  RAC_SERVER_REMOTINGCAPABILITIES,      // server: returns information about what can be remoted and what not

  //
  // User32.dll
  //
  RAC_USER32_BASE = RACBASE_USER32,
  RAC_U32MESSAGEBOX,
  RAC_U32FINDWINDOW,
  RAC_U32SENDMESSAGE,


  //
  // Kernel32.dll
  //
  RAC_KERNEL32_BASE = RACBASE_KERNEL32,
  RAC_K32CREATEPROCESS,               // CreateProcess()
  RAC_K32SUSPENDTHREAD,               // SuspendThread()
  RAC_K32RESUMETHREAD,                // ResumeThread()
  RAC_K32TERMINATEPROCESS,            // TerminateProcess()
  RAC_K32TERMINATETHREAD,             // TerminateThread()
  RAC_K32GETTHREADCONTEXT,            // GetThreadContext()
  RAC_K32SETTHREADCONTEXT,            // SetThreadContext()
  RAC_K32READPROCESSMEMORY,           // ReadProcessMemory()
  RAC_K32WRITEPROCESSMEMORY,          // WriteProcessMemory()
  RAC_K32FLUSHINSTRUCTIONCACHE,       // FlushInstructionCache()
  RAC_K32VIRTUALQUERYEX,              // VirtualQueryEx()
  RAC_K32CONTINUEDEBUGEVENT,          // ContinueDebugEvent()
  RAC_K32WAITFORDEBUGEVENT,           // WaitForDebugEvent()
  RAC_K32OUTPUTDEBUGSTRING,           // OutputDebugString()
  RAC_K32CLOSEHANDLE,                 // CloseHandle()
  RAC_K32OPENPROCESS,                 // OpenProcess()
  RAC_K32VIRTUALPROTECTEX,            // VirtualProtectEx()
  RAC_K32DEBUGBREAKPROCESS,           // DebugBreakProcess()
  RAC_K32DEBUGACTIVEPROCESS,          // DebugActiveProcess
  RAC_K32GETVERSIONEX,                // 
  RAC_K32GETVERSION,                  //
  RAC_K32WINEXEC,                     // WinExec()
  RAC_K32FINDFIRSTFILE,               // FindFirstFile()
  RAC_K32FINDCLOSE,                   // FindClose()
  RAC_K32FINDNEXTFILE,                // FindNextFile()
  RAC_K32CREATEFILE,            
  RAC_K32READFILE,
  RAC_K32WRITEFILE,
  RAC_K32GETFILESIZE,
  RAC_K32SETFILEPOINTER,
  RAC_K32SETFILEPOINTEREX,
  RAC_K32DEVICEIOCONTROL,

  //
  // PsApi.dll
  //
  RAC_PSAPI_BASE = RACBASE_PSAPI,
  RAC_PSAPIGETMODULEFILENAMEEX,          // PSAPI!GetModuleFileNameEx
  RAC_PSAPIENUMPROCESSES,                // PSAPI!EnumProcesses()
  RAC_PSAPIENUMPROCESSMODULES,           // PSAPI!EnumProcessModules()
  RAC_PSAPIGETMODULEBASENAME,            // PSAPI!GetModuleBaseName()
  RAC_PSAPIDEBUGACTIVEPROCESS,

  RAC_LAST_COMMAND
} eRemoteApiCommands;


// This is a generic buffer used to send/receive buffers of variable length
typedef struct
{
  DWORD  nLen;
  LPVOID lpData;
} tRemoteApiGenericData;

// This buffer is used to transport all the API functions parameters and their return values
typedef struct
{
  // signature and version
  DWORD Signature; 

  // Command code
  eRemoteApiCommands Cmd;

  // The return value of GetLastError()
  DWORD Win32LastError;

  // This is computed and checked against
  DWORD Integrity;


  // ---------------------------------------------------------------------


  // Different possible return values for the transported functions
  union
  {
    BOOL    b;
    DWORD   dw;
    HANDLE  h;
    PVOID   pv;
    PDWORD  pd;
    HMODULE hmodule;
    HWND    hwnd;
    int     int_;
  } ReturnValues;

  // All parameters used by these functions
  // these are optimized for size, meanning that big buffers that needs to be transported
  // should be used with the NextBlock and NextBlock.Type
  union
  {
    // CreateThread()
    struct
    {
      BOOL bInheritHandles;                      // handle inheritance option
      DWORD dwCreationFlags;                     // creation flags
      STARTUPINFOA StartupInfoA;                 // startup information
      PROCESS_INFORMATION ProcessInformation;    // process information
    } CreateProcess;

    
    // k32.DebugActiveProcess()
    struct
    {
      DWORD dwProcessId;
    } K32DebugActiveProcess;

    // SuspendThread()
    struct
    {
      HANDLE hThread; // handle to thread
    } SuspendThread;

    // ResumeThread()
    struct
    {
      HANDLE hThread;   // handle to thread
    } ResumeThread;

    
    // TerminateThread()
    struct
    {
      HANDLE hThread; // handle to thread
      UINT uExitCode;  // exit code
    } TerminateThread;    

    // k32.CloseHandle()
    struct
    {
      HANDLE hObject;   // handle to object
    } K32CloseHandle;

    // k32.FindClose()
    struct
    {
      HANDLE hFindFile;   // handle to object
    } K32FindClose;

    // TerminateProcess()
    struct
    {
      HANDLE hProcess; // handle to the process
      UINT uExitCode;  // exit code for the process
    } K32TerminateProcess;    

    // ContinueDebugEvent()
    struct
    {
      DWORD dwProcessId;       // process to continue
      DWORD dwThreadId;        // thread to continue
      DWORD dwContinueStatus;  // continuation status
    } K32ContinueDebugEvent;

    // VirtualQueryEx()
    struct
    {
      HANDLE hProcess;                     // handle to process
      LPCVOID lpAddress;                   // address of region
      MEMORY_BASIC_INFORMATION Buffer;     // information buffer
      DWORD dwLength;                      // size of buffer
    } VirtualQueryEx;

    // k32.DebugActiveProcess()
    struct
    {
      DWORD dwProcessId;   // process to be debugged
    } DebugActiveProcess;
    

    // k32.DebugBreakProcess()
    struct
    {
      HANDLE Process;
    } K32DebugBreakProcess;

    // k32.OpenProcess()
    struct
    {
      DWORD dwDesiredAccess; // [in] Access to the process object. This parameter can be one or more of the process access rights. 
      BOOL bInheritHandle; // [in] If this parameter is TRUE, the handle is inheritable. If the parameter is FALSE, the handle cannot be inherited. 
      DWORD dwProcessId; // [in] Identifier of the process to open. 
    } OpenProcess;

    // psapi.EnumProcessModules
    struct
    {
      HANDLE hProcess; // [in] Handle to the process. 
      DWORD cb; // [in] Size of the lphModule array, in bytes. 
      DWORD cbNeeded; // [out] Number of bytes required to store all module handles in the lphModule array. 
    } PSAPI_EnumProcessModules;

    // psapi.EnumProcesses
    struct
    {
      DWORD cb;       // [in] Size of the lpidProcess array, in bytes. 
      DWORD cbNeeded; // [out] Number of bytes returned in the lpidProcess array. 
    } PSAPI_EnumProcesses;

    // psapi.GetModuleBaseName()
    struct
    {
      HANDLE hProcess;    // handle to process
      HMODULE hModule;    // handle to module
      DWORD nSize;        // maximum characters to retrieve
    } PSAPI_GetModuleBaseName;

    // psapi.GetModuleFileNameEx()
    struct
    {
      HANDLE hProcess;    // handle to process
      HMODULE hModule;    // handle to module
      DWORD nSize;        // maximum characters to retrieve
    } PSAPI_GetModuleFileNameEx;

    struct
    {
      HANDLE hProcess;        // handle to process 
      LPCVOID lpBaseAddress;  // base of region to flush
      DWORD dwSize;           // length of region to flush
    } FlushInstructionCache;

    // GetThreadContext
    struct
    {
      HANDLE hThread;   // handle to thread with context
      CONTEXT Context;  // context structure
    } GetThreadContext;


    // SetThreadContext()
    struct
    {
      HANDLE hThread;   // handle to thread with context
      CONTEXT Context;  // context structure
    } SetThreadContext;


    // GetThreadSelectorEntry()
    struct
    {
      HANDLE hThread;              // handle to thread
      DWORD dwSelector;            // number of selector value
      LDT_ENTRY SelectorEntry;     // descriptor table entry
    } GetThreadSelectorEntry;

    // k32.IsDebuggerPresent()
    struct
    {
    } IsDebuggerPresent;

    // u32.MessageBox()
    struct
    {
      HWND hWnd;
      UINT uType;
    } U32MessageBox;

    // k32.OutputDebugString()
    struct
    {
      HANDLE hFindFile;
    } K32FindNextFile;

    // k32.ReadProcessMemory()
    struct
    {
      HANDLE  hProcess;              // handle to the process
      LPCVOID lpBaseAddress;         // base of memory area
      DWORD   nSize;                 // number of bytes to read
      DWORD  NumberOfBytesRead;      // number of bytes read
    } ReadProcessMemory;

    // k32.WriteProcessMemory
    struct
    {
      HANDLE hProcess;                // handle to process
      LPVOID lpBaseAddress;           // base of memory area
      LPVOID lpBuffer;                // data buffer
      DWORD  nSize;                   // number of bytes to write
      DWORD  NumberOfBytesWritten;    // number of bytes written
    } WriteProcessMemory;

    // WaitForDebugEvent()
    struct
    {
      DEBUG_EVENT DebugEvent;  // debug event information
      DWORD dwMilliseconds;    // time-out value
    } WaitForDebugEvent;
    
    struct
    {
      UINT uCmdShow;
    } K32WinExec;

    // miscellaneous usage parameters
    struct
    {
      LPVOID Param1;
      LPVOID Param2;
      LPVOID Param3;
      LPVOID Param4;
    } Misc;

    struct
    {
      DWORD dwDesiredAccess;
      DWORD dwShareMode;
      DWORD dwCreationDisposition;
      DWORD dwFlagsAndAttributes;
      HANDLE hTemplateFile;
    } K32CreateFile;

    struct 
    {
      HANDLE hFile;
      DWORD nNumberOfBytesToRead;
      DWORD lpNumberOfBytesRead;
    } K32ReadFile;

    struct
    {
      HANDLE hFile;
      DWORD nNumberOfBytesToWrite;
      DWORD lpNumberOfBytesWritten;
    } K32WriteFile;

    struct
    {
      HANDLE hFile;
      DWORD lpFileSizeHigh;
    } K32GetFileSize;

    struct 
    {
      HANDLE hFile;
      LONG lDistanceToMove;
      LONG lpDistanceToMoveHigh;
      DWORD dwMoveMethod;
    } K32SetFilePointer;

    struct 
    {
      HANDLE hFile;
      LARGE_INTEGER  liDistanceToMove;
      LARGE_INTEGER  lpNewFilePointer;
      DWORD dwMoveMethod;
    } K32SetFilePointerEx;

    struct 
    {
      HANDLE hDevice;
      DWORD dwIoControlCode;
      DWORD nInBufferSize;
      DWORD nOutBufferSize;
      DWORD lpBytesReturned;
    } K32DeviceIoControl;
  } Parameters;

  CHAR CacheBuffer[RAPI_CACHE_BUFSIZE];
} tRemoteApiRequestPacket;


//---------------------------------------------------------------------------
// This structure is used to define a remote API session.
// It got a socket handle, request packet and a generic data buffer struct.
// Additional variables are used here just for convenience and extendibility
typedef struct 
{
  SOCKET sd;
  tRemoteApiRequestPacket   rarp;
  tRemoteApiGenericData     ragd;
  LPVOID                    Param;
  LPVOID                    Param2;

  union
  {
    DWORD                     dd[4];
    LPVOID                    lp[4];
  } Dummy;
} tRemoteApiSession, *pRemoteApiSession;

// Macros to send/receive Remote Api Request Packets.
#define rarpSend(session)    SendBuffer(session.sd, (LPVOID)&s.rarp, sizeof(tRemoteApiRequestPacket))
#define rarpReceive(session) ReceiveBuffer(session.sd, (LPVOID)&s.rarp, sizeof(tRemoteApiRequestPacket))

#define rarpSetLastError(session) ::SetLastError(session.rarp.Win32LastError)
#define rarpGetLastError(session) session.rarp.Win32LastError = ::GetLastError()
#define rarpLastErrorSet(session, error) session.rarp.Win32LastError = error

#define REMOTEAPI_SIG MAKEWORD('E', 'B')

#ifndef _MYDEBUGPRINTMEDIUM
#define _MYDEBUGPRINTMEDIUM printf
#endif

#ifdef _MYDEBUG
#define DEBUG_PRINT(expr) _MYDEBUGPRINTMEDIUM expr;
#else
#define DEBUG_PRINT(expr)
#endif

#define REMOTEAPICALL _stdcall

//
//
//
//
int ragdSend(tRemoteApiSession &, LPVOID p, UINT n);
int ragdReceiveAlloc(tRemoteApiSession &, LPVOID *p);
int ragdReceive(tRemoteApiSession &, LPVOID p, UINT n);

#endif