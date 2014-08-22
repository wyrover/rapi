#ifndef __REMOTEAPI_SERVER__INC
#define __REMOTEAPI_SERVER__INC

#ifdef _MYDEBUGPRINTMEDIUM
#undef _MYDEBUGPRINTMEDIUM
#endif
#define _MYDEBUGPRINTMEDIUM LogwindowSendFmt


#include "../libraries/socks_common.h"
#include "../libraries/string_util.h"

#include "remoteapi.h"

#ifdef __RAPI_USE_PSAPI
#include <psapi.h>
#pragma comment(lib, "psapi")
#endif

#include <stdio.h>

#ifdef __RAPI_USE_PSAPI
void RApiServerGetModuleFileNameEx(tRemoteApiSession &);
void RApiServerEnumProcesses(tRemoteApiSession &);
void RApiServerEnumProcessModules(tRemoteApiSession &);
void RApiServerGetModuleBaseName(tRemoteApiSession &);
#endif

void RApiServerK32CloseHandle(tRemoteApiSession &);
void RApiServerK32DebugActiveProcess(tRemoteApiSession &);
void RApiServerK32DebugBreakProcess(tRemoteApiSession &);
void RApiServerK32WinExec(tRemoteApiSession &);
void RApiServerK32FindFirstFile(tRemoteApiSession &);
void RApiServerK32FindNextFile(tRemoteApiSession &);
void RApiServerK32FindClose(tRemoteApiSession &);
void RApiServerK32ResumeThread(tRemoteApiSession &);
void RApiServerK32SuspendThread(tRemoteApiSession &);
void RApiServerK32CreateProcess(tRemoteApiSession &);
void RApiServerK32TerminateProcess(tRemoteApiSession &);
void RApiServerK32TerminateThread(tRemoteApiSession &);
void RApiServerK32GetThreadContext(tRemoteApiSession &);
void RApiServerK32SetThreadContext(tRemoteApiSession &);
void RApiServerK32ReadProcessMemory(tRemoteApiSession &);
void RApiServerK32WriteProcessMemory(tRemoteApiSession &);
void RApiServerK32WriteProcessMemory(tRemoteApiSession &);
void RApiServerK32FlushInstructionCache(tRemoteApiSession &);
void RApiServerK32VirtualQueryEx(tRemoteApiSession &);
void RApiServerK32ContinueDebugEvent(tRemoteApiSession &);
void RApiServerK32WaitForDebugEvent(tRemoteApiSession &);
void RApiServerK32OpenProcess(tRemoteApiSession &);
void RApiServerK32CreateFile(tRemoteApiSession &);
void RApiServerK32ReadFile(tRemoteApiSession &);
void RApiServerK32WriteFile(tRemoteApiSession &);
void RApiServerK32GetFileSize(tRemoteApiSession &);
void RApiServerK32SetFilePointer(tRemoteApiSession &);
void RApiServerK32SetFilePointerEx(tRemoteApiSession &);
void RApiServerK32DeviceIoControl(tRemoteApiSession &);

void RApiServerU32FindWindow(tRemoteApiSession &);
void RApiServerU32MessageBox(tRemoteApiSession &);

#endif