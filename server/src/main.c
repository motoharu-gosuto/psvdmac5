#include <stdio.h>
#include <malloc.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>

#include <psvdmac5.h>

#include "debugScreen.h"

//net initialization part is taken from xerpi:
//https://gist.github.com/xerpi/e426284df19c217a8128

//also some usefull info here:
//https://github.com/xerpi/libftpvita/blob/master/libftpvita/ftpvita.c
//https://github.com/psxdev/debugnet/blob/master/libdebugnet/source/debugnet.c
 
//some refresher info about sockets here
//http://stackoverflow.com/questions/16486361/creating-a-basic-c-c-tcp-socket-writer
//http://matrixsust.blogspot.ru/2011/10/basic-tcp-server-client.html
//http://www.linuxhowtos.org/C_C++/socket.htm

#define NET_INIT_SIZE 1*1024*1024

int _dmac5_sock = 0;
void *net_memory = NULL;

int dmac5_port = 1330;

char* dmac5_sock_name = "dmac5proxy";

int _cli_sock = 0;

int init_net()
{
  if (sceNetShowNetstat() == SCE_NET_ERROR_ENOTINIT) 
  {
      net_memory = malloc(NET_INIT_SIZE);

      SceNetInitParam initparam;
      initparam.memory = net_memory;
      initparam.size = NET_INIT_SIZE;
      initparam.flags = 0;

      sceNetInit(&initparam);
      sceKernelDelayThread(100 * 1000);
      
      psvDebugScreenPrintf("psvdmac5: net initialized\n");
  }
 
  if (sceNetCtlInit() < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to initialize netctl\n");
    return -1;
  }
 
  psvDebugScreenPrintf("psvdmac5: netctl initialized\n");
    
  SceNetSockaddrIn server;
    
  server.sin_len = sizeof(server);
  server.sin_family = SCE_NET_AF_INET;
  server.sin_addr.s_addr = SCE_NET_INADDR_ANY;
  server.sin_port = sceNetHtons(dmac5_port);
 
  memset(server.sin_zero, 0, sizeof(server.sin_zero));

  _dmac5_sock = sceNetSocket(dmac5_sock_name, SCE_NET_AF_INET, SCE_NET_SOCK_STREAM, 0);
  if(_dmac5_sock < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to create socket\n");
    return -1;
  }
 
  psvDebugScreenPrintf("psvdmac5: server socket created\n");
   
  int bind_res = sceNetBind(_dmac5_sock, (SceNetSockaddr*)&server, sizeof(server));
  if(bind_res < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to bind socket %x\n", bind_res);
    return -1;
  }
 
  SceNetCtlInfo info;
  if (sceNetCtlInetGetInfo(SCE_NETCTL_INFO_GET_IP_ADDRESS, &info) < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to get network info\n");
    return -1;
  }
 
  psvDebugScreenPrintf("psvdmac5: server socket binded %s:%d\n", info.ip_address, dmac5_port);
  
  if(sceNetListen(_dmac5_sock, 128) < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to listen socket\n");
    return -1;
  }
 
  psvDebugScreenPrintf("psvdmac5: listening for connection\n");
 
  return 0;
}

void deinit_net()
{
  if(_cli_sock)
  {
    if(sceNetSocketClose(_cli_sock) < 0)
      psvDebugScreenPrintf("psvdmac5: failed to close client socket\n");
    _cli_sock = 0;
  }

  if (_dmac5_sock) 
  {
    if(sceNetSocketClose(_dmac5_sock) < 0)
      psvDebugScreenPrintf("psvdmac5: failed to close server socket\n");
    _dmac5_sock = 0;
  }
 
  sceNetCtlTerm();

  sceNetTerm();
 
  if (net_memory) 
  {
    free(net_memory);
    net_memory = NULL;
  }
}

#define DMAC5_COMMAND_PING 0
#define DMAC5_COMMAND_TERM 1

#pragma pack(push, 1)

typedef struct command_0_request
{
  int command;
} command_0_request;
 
typedef struct command_0_response
{
    int command;
    int vita_err;
    int proxy_err;
    char data[10];
} command_0_response;

typedef struct command_1_request
{
  int command;
} command_1_request;

typedef struct command_1_response
{
    int command;
    int vita_err;
    int proxy_err;
    char data[10];
} command_1_response;

int handle_command_0()
{
  command_0_response resp;
  memset(&resp, 0, sizeof(command_0_response));
  resp.command = DMAC5_COMMAND_PING;
  memcpy(resp.data, "dmac5proxy", 10);
  psvDebugScreenPrintf("psvdmac5: execute command 0\n");

  return sceNetSend(_cli_sock, &resp, sizeof(command_0_response), 0);
}

int handle_command_1()
{
  command_1_response resp;
  memset(&resp, 0, sizeof(command_1_response));
  resp.command = DMAC5_COMMAND_TERM;
  memcpy(resp.data, "dmac5proxy", 10);
  
  psvDebugScreenPrintf("psvdmac5: execute command 1\n");

  return sceNetSend(_cli_sock, &resp, sizeof(command_1_response), 0);
}

void receive_commands()
{
  while(1)
  {
    int command = -1;
    int recvLen = sceNetRecv(_cli_sock, &command, sizeof(int), 0);
    if(recvLen <= 0)
    {
      psvDebugScreenPrintf("psvdmac5: failed to receive data\n");
      return;
    }
	  
    switch(command)
    {
    case DMAC5_COMMAND_PING:
      if(handle_command_0() < 0)
      {
        psvDebugScreenPrintf("psvdmac5: failed to handle command 0\n");
        return;
      }
      break;
    case DMAC5_COMMAND_TERM:
      if(handle_command_1() < 0)
      {
        psvDebugScreenPrintf("psvdmac5: failed to handle command 1\n");
        return;
      }
      return;
    default:
      psvDebugScreenPrintf("psvdmac5: unknown command\n");
      return;
    }
  }
}

void accept_single_connection()
{
  while(1)
  {
    SceNetSockaddrIn client;
    memset(&client, 0, sizeof(client));
    client.sin_len = sizeof(client);

    unsigned int sin_size = sizeof(client);
    _cli_sock = sceNetAccept(_dmac5_sock, (SceNetSockaddr*)&client, &sin_size);
    if(_cli_sock < 0)
    {
      psvDebugScreenPrintf("psvdmac5: failed to accept socket'\n");
      return;
    }

    char ipstr[16];
    psvDebugScreenPrintf("psvdmac5: Accepted connection from %s:%d\n", sceNetInetNtop(SCE_NET_AF_INET, &client.sin_addr, ipstr, 16), sceNetNtohs(client.sin_port));

    receive_commands();

    if(_cli_sock)
    {
      if(sceNetSocketClose(_cli_sock) < 0)
        psvDebugScreenPrintf("psvdmac5: failed to close client socket\n");
      _cli_sock = 0;
    }

    psvDebugScreenPrintf("psvdmac5: closed client socket\n");
  }
}


int main(int argc, char *argv[])
{
  psvDebugScreenInit();

  psvDebugScreenPrintf("psvdmac5: server started\n");

  if (sceSysmoduleIsLoaded(SCE_SYSMODULE_NET) != SCE_SYSMODULE_LOADED)
  {
    if(sceSysmoduleLoadModule(SCE_SYSMODULE_NET) < 0)
    {
      psvDebugScreenPrintf("psvdmac5: failed to load net module\n");
      return 1;
    }
  }

  if(init_net() >= 0)
  {
    accept_single_connection();
  }

  deinit_net();

  psvDebugScreenPrintf("psvdmac5: server stopped\n");

  sceKernelDelayThread(10*1000*1000);

  sceKernelExitProcess(0);

  return 0;
}
