#ifndef __REMOTEDEBUG_UTIL_INC
#define __REMOTEDEBUG_UTIL_INC

#include <windows.h>

void UnicodeToAnsi(LPWSTR WideStr, LPSTR AnsiStr);

BOOL GetRemoteDebugeeModuleName(
  tRemoteApiSession &s,
  const HANDLE hProcess, 
  const LPVOID lpImageName, 
  BOOL Unicode,
  LPVOID Buffer,
  int nSize);

BOOL FindRemoteModule(
  tRemoteApiSession &session,
  LPCSTR procName,
  LPCSTR modName,
  PDWORD _uProcessId,
  PDWORD _hModule,
  BOOL BaseNames);

#endif