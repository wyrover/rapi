#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>

#include "rapi_lib/remoteapi_client.h"
#include "rapi_lib/remotedebug_util.h"

tRemoteApiSession session;

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


void test2(char *program)
{
  PROCESS_INFORMATION pi;
  STARTUPINFO si;

  DWORD nbRW;
  DEBUG_EVENT de;
  CHAR buf[1024];

  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);

  BOOL rc;
  rc = RApiClientCreateProcess(session,
                        program,
                        "c:\\", // command line
                        NULL,
                        NULL,
                        FALSE,
                        DEBUG_PROCESS | NORMAL_PRIORITY_CLASS | CREATE_DEFAULT_ERROR_MODE,
                        NULL, // env
                        "C:\\", // cur dir
                        &si,
                        &pi);
  GetLastError();   
  if (!rc)
  {
    printf("failed to create process! (gle=%ld)", GetLastError());
    return;
  }

  printf("Process ID:%08lX ThreadID:%08lX\n", pi.dwProcessId, pi.dwThreadId);

  BOOL b=TRUE;
  DWORD gle;
  while (b)
  {
    b = RApiClientWaitForDebugEvent(session, &de, INFINITE);
    if (gle=GetLastError())
    {
      if (gle==0x12B)
        b = TRUE;
      printf("WaitForDebugEvent failed: (GLE=%d)\n", GetLastError());
    }

    // skip all debug events that are not ours!
    if (de.dwProcessId != pi.dwProcessId)
      RApiClientContinueDebugEvent(session, de.dwProcessId, de.dwThreadId, DBG_CONTINUE);

    switch (de.dwDebugEventCode)
    {
      //
      //
      //
      case EXCEPTION_DEBUG_EVENT:
        printf("Exception: (address=%08lX)\n", de.u.Exception.ExceptionRecord.ExceptionAddress);
        break;
      //
      //
      //
      case CREATE_THREAD_DEBUG_EVENT:
        printf("CreateThread: (hThread:%08lX LocalBase:%08lX StartAddress:%08lX)\n", 
                de.u.CreateThread.hThread,
                de.u.CreateThread.lpThreadLocalBase,
                de.u.CreateThread.lpStartAddress);
        break;
      //
      //
      //
      case CREATE_PROCESS_DEBUG_EVENT:
        strcpy(buf, "?");
        if (!GetRemoteDebugeeModuleName(session,
                                        de.u.CreateProcessInfo.hProcess, 
                                        de.u.CreateProcessInfo.lpImageName, 
                                        de.u.CreateProcessInfo.fUnicode,
                                        &buf, 
                                        sizeof(buf)))
        {
          // ...  
        }
        printf("CreateProcess: (\n\tModule:%s\n\thFile:%ld)\n", 
                buf, 
                de.u.CreateProcessInfo.hFile);

        break;
      //
      //
      //
      case OUTPUT_DEBUG_STRING_EVENT:
        if (!RApiClientReadProcessMemory(session, pi.hProcess, de.u.DebugString.lpDebugStringData, buf, 
                                    (de.u.DebugString.fUnicode ? 2 : 1) * de.u.DebugString.nDebugStringLength,
                                    &nbRW))
          strcpy(buf, "NULL");
        printf("OutDebugString(%s)\n", buf);
        break;
      case LOAD_DLL_DEBUG_EVENT:
        strcpy(buf, "?");
        if (!GetRemoteDebugeeModuleName(session, pi.hProcess, 
                                        de.u.LoadDll.lpImageName, 
                                        de.u.LoadDll.fUnicode,
                                        &buf, sizeof(buf)))
        {
          //RApiClientReadProcessMemory(pi.hProcess, (LPVOID)((DWORD)de.u.LoadDll.lpBaseOfDll+0x1000), buf, sizeof(buf), &nbRW);
          //RApiClientGetModuleFileNameEx(pi.hProcess, (HMODULE)de.u.LoadDll.lpBaseOfDll, buf, sizeof(buf));
        }
        printf("LoadDll: (Module: %s Base:%08lX)\n", buf, de.u.LoadDll.lpBaseOfDll);         
        {
          MEMORY_BASIC_INFORMATION mbi;
          RApiClientVirtualQueryEx(session, pi.hProcess, (LPVOID)0x401000, &mbi, sizeof(mbi));
        }
        break;
      case EXIT_PROCESS_DEBUG_EVENT:
        b = FALSE;
        printf("Process has exited (code=%d)!\n", de.u.ExitProcess.dwExitCode);
        break;
      case UNLOAD_DLL_DEBUG_EVENT:
        printf("Unload dll: (Base: %08lX)\n", de.u.UnloadDll.lpBaseOfDll);
        break;
    }
    RApiClientContinueDebugEvent(session, de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
  }

  RApiClientTerminateProcess(session, pi.hProcess, 0);
}


void test3(int argc, char *argv[])
{
  CHAR program[MAX_PATH];

  if (argc > 3)
    strcpy(program, argv[3]);
  else
    strcpy(program, "c:\\temp\\notepad1.exe");

  //RApiClientK32WinExec(session, "calc.exe", SW_SHOW);
  //  HWND h = RApiClientU32FindWindow(session, "Notepad", NULL);
  //  ShowWindow(h, SW_SHOW);
  //HWND h(NULL);
  //RApiClientU32MessageBox(session, h, "Hello world", "Info", MB_YESNO);

  WIN32_FIND_DATA fd;
  HANDLE fh;

  fh = RApiClientK32FindFirstFile(session, "c:\\*.*", &fd);
  ::GetLastError();

  do
  {
    printf("%s\n", fd.cFileName);
  } while (RApiClientK32FindNextFile(session, fh, &fd));


  RApiClientK32FindClose(session, fh);
}

void test4()
{
  HANDLE h = RApiClientCreateFile(session, "c:\\trash1.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
  if (h == INVALID_HANDLE_VALUE)
    return;

  DWORD t, sz = RApiClientGetFileSize(session, h, NULL);

  printf("file's size is:%d\n", sz);

  char buf[123];

  t = 0;

  while (t < sz)
  {
    DWORD nbread;

    if (!RApiClientReadFile(session, h, buf, sizeof(buf), &nbread, NULL) || (nbread == 0))
      break;
    printf("read: %d\n", nbread);
    t += nbread;
  }
  
  RApiClientCloseHandle(session, h);
}

int main(int argc, char *argv[])
{
  WSADATA ws;

  printf("sig(%d) rdrp(%d)\n", REMOTEAPI_SIG, sizeof(tRemoteApiRequestPacket));

  if (argc < 2)
  {
    printf("Usage: client [host] [port]");
    return 0;
  }

  WSAStartup(MAKEWORD(1,1), &ws);   

  int nPort = atoi(argv[2]);
  if (RApiClient_Init(session, argv[1], nPort))
    return 0;

  test4();

  RApiClient_Close(session);

  WSACleanup();
  return 0;
}