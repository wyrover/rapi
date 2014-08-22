#ifndef RAPI_DEFS_INC__
#define RAPI_DEFS_INC__

#define LC_IPC_NAME _T("rapisrvr")

//------------------------------------------------------------------------------------
typedef enum
{
  RASC_SUCCESS = 0,
  RASC_CANNOTLISTEN,
  RASC_CANNOTCREATECHILDSERVER,
  RASC_CANNOT
} eRApiStatusCodes;

//------------------------------------------------------------------------------------
typedef enum
{
  LCCC_DUPHANDLE = 0,
  LCCC_CLOSEHANDLE,
  LCCC_X
} eListenChildCommCodes;

//------------------------------------------------------------------------------------
// This struct is used to let both the listener and the child servers communincate
//
//
typedef struct
{
  DWORD Command;
  DWORD Win32LastError;
  union
  {
    DWORD dd;
    BOOL  b;
  } rc;
  union
  {
    struct
    {
      DWORD   ChildPID;
      DWORD   ListenerPID;
      HANDLE  ListenerHandle;
      HANDLE  ChildHandle;
    } DupHandle;

    struct
    {
      HANDLE handle;
    } CloseHandle;

  } Request;
} tListenChildComm;

#endif