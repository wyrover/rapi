#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "rapi_lib/remoteapi.h"
#include "rapi_lib/remoteapi_server.h"
#include "processservercommunication.h"

tRemoteApiSession session;

bool g_close_server = false, g_restart_server = false;

int server(const char *pcHost, int nPort)
{
  SOCKET listenSocket = SetUpListener(pcHost, htons(nPort));  
  
  if (listenSocket == INVALID_SOCKET)
  {
    printf("Error setting listening socket!\n");
    return 1;
  }

  g_close_server = false;

  while (!g_close_server)
  {
    printf("Waiting for one connection on port %d...\n", nPort);

    // address of address that will connect to us
    sockaddr_in sinRemote;

    // data socket
    session.sd = AcceptConnection(listenSocket, sinRemote);

    if (session.sd != INVALID_SOCKET)
    {
      printf("Accepted connection from %s:%d\n", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));
    }
    else
    {
      printf("Error accepting connection!\n");
      return 2;
    }
    
    while (true)
    {
      if (processServerCommunication(session) == false)
        break;
    }
    ShutdownConnection(session.sd);
    printf("Connection to client closed\n");
  }
  return 0;
}


int main(int argc, char *argv[])
{
  WSADATA ws;
  int rc;
  if (argc < 3)
  {
    printf("Usage: server [host] [port]");
    return 0;
  }

  printf("RApiServer build (%s %s)\n", __DATE__, __TIME__);

  WSAStartup(MAKEWORD(1,1), &ws);   

  int nPort = atoi(argv[2]);

  rc = server(argv[1], nPort);  

  printf("Server shutdown!\n");

  WSACleanup();

  if (g_restart_server)
  {
    rc = 3;
    printf("Server restarting...\n");  
    WinExec(GetCommandLine(), SW_SHOW);
  }
  return rc;
}