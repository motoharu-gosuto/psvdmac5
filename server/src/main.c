#include <stdio.h>
#include <malloc.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>

#include <psvdmac5.h>

#include "types.h"

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

char sprintfBuffer[256];

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

int send_data(unsigned char* src, int size)
{
  int bytesToSend = size;
  int bytesWereSend = 0;
  while(bytesWereSend != bytesToSend)
  {
     int sendLen = sceNetSend(_cli_sock, (char*)(src + bytesWereSend), bytesToSend - bytesWereSend, 0);
     if(sendLen <= 0)
     {
        psvDebugScreenPrintf("psvdmac5: failed to send data\n");
        return - 1;
     }
     
     bytesWereSend = bytesWereSend + sendLen;
  }
  return 0;
}

int recv_data(unsigned char* dest, int size)
{
  int bytesToReceive = size;
  int bytesWereReceived = 0;
  while(bytesWereReceived != bytesToReceive)
  {
     int recvLen = sceNetRecv(_cli_sock, (char*)(dest + bytesWereReceived), bytesToReceive - bytesWereReceived, 0);
     if(recvLen <= 0)
     {
       psvDebugScreenPrintf("psvdmac5: failed to receive data\n");
       return - 1;
     }
     bytesWereReceived = bytesWereReceived + recvLen;
  }
  return 0;
}

#define ROUNDUP(n, width) (((n) + (width) - 1) & (~(unsigned int)((width) - 1)))

#define ONE_MB_SIZE (1024 * 1024)

int allocate_buffer(char* name, int size, SceUID* uid, unsigned char** dest)
{
   *uid = sceKernelAllocMemBlock(name, SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, ROUNDUP(size, ONE_MB_SIZE), 0);
   if(*uid < 0)
   {
     snprintf(sprintfBuffer, 256, "psvdmac5: failed to allocate buffer: %x. size: %x\n", *uid, size);
     psvDebugScreenPrintf(sprintfBuffer);
     return -1;
   }

   int mem_block_res = sceKernelGetMemBlockBase(*uid, (void**)dest);
   if(mem_block_res < 0)
   {
     psvDebugScreenPrintf("psvdmac5: failed to get buffer\n");
     return - 1;
   }

   return 0;
}

int deallocate_buffer(SceUID uid)
{
  if(sceKernelFreeMemBlock(uid) < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to deallocate buffer\n");
    return - 1;
  }
  return 0;
}

int handle_command_0()
{
  command_0_response resp;
  memset(&resp, 0, sizeof(command_0_response));
  resp.command = PSVDMAC5_COMMAND_PING;
  memcpy(resp.data, "dmac5proxy", 10);
  psvDebugScreenPrintf("psvdmac5: execute command 0\n");

  return sceNetSend(_cli_sock, &resp, sizeof(command_0_response), 0);
}

int handle_command_1()
{
  command_1_response resp;
  memset(&resp, 0, sizeof(command_1_response));
  resp.command = PSVDMAC5_COMMAND_TERM;
  memcpy(resp.data, "dmac5proxy", 10);
  
  psvDebugScreenPrintf("psvdmac5: execute command 1\n");

  return sceNetSend(_cli_sock, &resp, sizeof(command_1_response), 0);
}

int handle_command_2(command_2_request* req, unsigned char* data)
{
  //allocate buffer
  SceUID dest_uid;
  unsigned char* dest_buffer = 0;
  if(allocate_buffer("dest", req->size, &dest_uid, &dest_buffer) < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to allocate dest buffer\n");
    return -1;
  }

  //initialize args
  sceSblSsMgrAESECBWithKeygenForDriverProxy_args args;
  args.src = data;
  args.dst = dest_buffer;
  args.size = req->size;
  args.key = data + req->size;
  args.key_size = req->key_size;
  args.key_id = req->key_id;
  args.mask_enable = req->mask_enable;

  //execute kernel proxy function
  command_2_response resp;
  resp.command = PSVDMAC5_COMMAND_AESECB;
  resp.proxy_err = 0;

  if(req->encrypt)
    resp.vita_err = _sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy(&args);
  else
    resp.vita_err = _sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(&args);

  //exit if proxy call failed
  if(resp.vita_err < 0)
  {
    snprintf(sprintfBuffer, 256, "psvdmac5: failed to call proxy function: %x\n", resp.vita_err);
    psvDebugScreenPrintf(sprintfBuffer);

    deallocate_buffer(dest_uid);
    return -1;
  }

  //send response to client - base data
  if(send_data(((char*)&resp), sizeof(command_2_response)) < 0)
  {
    deallocate_buffer(dest_uid);
    return -1;
  }

  //send additional data
  if(send_data(dest_buffer, req->size) < 0)
  {
    deallocate_buffer(dest_uid);
    return -1;  
  }

  //deallocate buffer
  if(deallocate_buffer(dest_uid) < 0)
    return -1;

  return 0;
}

int handle_command_3(command_3_request* req, unsigned char* data)
{
  //allocate buffer
  SceUID dest_uid;
  unsigned char* dest_buffer = 0;
  if(allocate_buffer("dest", req->size, &dest_uid, &dest_buffer) < 0)
  {
    psvDebugScreenPrintf("psvdmac5: failed to allocate dest buffer\n");
    return -1;
  }

  //initialize args
  sceSblSsMgrAESCBCWithKeygenForDriverProxy_args args;
  args.src = data;
  args.dst = dest_buffer;
  args.size = req->size;
  args.key = data + req->size;
  args.key_size = req->key_size;
  args.iv = data + req->size + (req->key_size / 8);
  args.key_id = req->key_id;
  args.mask_enable = req->mask_enable;

  //execute kernel proxy function
  command_3_response resp;
  resp.command = PSVDMAC5_COMMAND_AESCBC;
  resp.proxy_err = 0;

  if(req->encrypt)
    resp.vita_err = _sceSblSsMgrAESCBCEncryptWithKeygenForDriverProxy(&args);
  else
    resp.vita_err = _sceSblSsMgrAESCBCDecryptWithKeygenForDriverProxy(&args);

  //exit if proxy call failed
  if(resp.vita_err < 0)
  {
    snprintf(sprintfBuffer, 256, "psvdmac5: failed to call proxy function: %x\n", resp.vita_err);
    psvDebugScreenPrintf(sprintfBuffer);

    deallocate_buffer(dest_uid);
    return -1;
  }

  //send response to client - base data
  if(send_data(((char*)&resp), sizeof(command_3_response)) < 0)
  {
    deallocate_buffer(dest_uid);
    return -1;
  }

  //send additional data - send dest buffer
  if(send_data(dest_buffer, req->size) < 0)
  {
    deallocate_buffer(dest_uid);
    return -1;  
  }

  //send additional data - send iv
  if(send_data(data + req->size + (req->key_size / 8), 0x10) < 0)
  {
    deallocate_buffer(dest_uid);
    return -1;  
  }

  //deallocate buffer
  if(deallocate_buffer(dest_uid) < 0)
    return -1;

  return 0;
}

int handle_command_4(command_4_request* req, unsigned char* data)
{
  sceSblSsMgrHMACSHA1WithKeygenForDriverProxy_args args;
  args.src = 0;
  args.dst = 0;
  args.size = req->size;
  args.key = 0;
  args.iv = 0;
  args.key_id = req->key_id;
  args.mask_enable = req->mask_enable;
  args.command_bit = req->command_bit;

  //TODO: src, key, iv should be pointed to req.data
  //TODO: dst should be allocated from heap and then sent in response

  command_4_response resp;
  
  resp.vita_err = _sceSblSsMgrHMACSHA1WithKeygenForDriverProxy(&args);

  return 0;
}

int handle_command_5(command_5_request* req, unsigned char* data)
{
  sceSblSsMgrAESCMACWithKeygenForDriverProxy_args args;
  args.src = 0;
  args.dst = 0;
  args.size = req->size;
  args.key = 0;
  args.key_size = req->key_size;
  args.iv = 0;
  args.key_id = req->key_id;
  args.mask_enable = req->mask_enable;
  args.command_bit = req->command_bit;

  //TODO: src, key, iv should be pointed to req.data
  //TODO: dst should be allocated from heap and then sent in response

  command_5_response resp;

  resp.vita_err = _sceSblSsMgrAESCMACWithKeygenForDriverProxy(&args);

  return 0;
}

//---

int handle_command_2_base(int command)
{
  command_2_request recvBuffer;
  recvBuffer.command = command;

  //receive main data
  int header_size = sizeof(command_2_request) - sizeof(int);
  char* header_dest = ((char*)&recvBuffer) + sizeof(int);
  if(recv_data(header_dest, header_size) < 0)
    return -1;

  //check arguments
  if(recvBuffer.size == 0 || recvBuffer.key_size == 0 || recvBuffer.mask_enable != 1)
  {
    psvDebugScreenPrintf("psvdmac5: invalid args in command 2\n");
    return -1;
  }

  //allocate data buffer
  int data_size = recvBuffer.size + (recvBuffer.key_size / 8);
  unsigned char* data_dest = 0;
  SceUID data_uid = -1;
  
  if(allocate_buffer("data", data_size, &data_uid, &data_dest) < 0)
    return -1;
  
  //receive additional data
  if(recv_data(data_dest, data_size) < 0)
  {
    deallocate_buffer(data_uid);
    return -1;
  }

  //handle command
  if(handle_command_2(&recvBuffer, data_dest) < 0)
  {
    deallocate_buffer(data_uid);
    psvDebugScreenPrintf("psvdmac5: failed to handle command 2\n");
    return -1;
  }

  //deallocate buffer
  if(deallocate_buffer(data_uid) < 0)
    return -1;

  return 0;
}

int handle_command_3_base(int command)
{
  command_3_request recvBuffer;
  recvBuffer.command = command;

  //receive main data
  int header_size = sizeof(command_3_request) - sizeof(int);
  char* header_dest = ((char*)&recvBuffer) + sizeof(int);
  if(recv_data(header_dest, header_size) < 0)
    return -1;

  //check arguments
  if(recvBuffer.size == 0 || recvBuffer.key_size == 0 || recvBuffer.mask_enable != 1)
  {
    psvDebugScreenPrintf("psvdmac5: invalid args in command 3\n");
    return -1;
  }

  //allocate data buffer
  int data_size = recvBuffer.size + (recvBuffer.key_size / 8) + 0x10;
  unsigned char* data_dest = 0;
  SceUID data_uid = -1;
  
  if(allocate_buffer("data", data_size, &data_uid, &data_dest) < 0)
    return -1;
  
  //receive additional data
  if(recv_data(data_dest, data_size) < 0)
  {
    deallocate_buffer(data_uid);
    return -1;
  }

  //handle command
  if(handle_command_3(&recvBuffer, data_dest) < 0)
  {
    deallocate_buffer(data_uid);
    psvDebugScreenPrintf("psvdmac5: failed to handle command 3\n");
    return -1;
  }

  //deallocate buffer
  if(deallocate_buffer(data_uid) < 0)
    return -1;

  return 0;
}

int handle_command_4_base(int command)
{
  command_4_request recvBuffer;
  recvBuffer.command = command;

  //TODO: additional data should be read before handling command

  handle_command_4(&recvBuffer, 0);

  return 0;
}

int handle_command_5_base(int command)
{
  command_5_request recvBuffer;
  recvBuffer.command = command;

  //TODO: additional data should be read before handling command

  handle_command_5(&recvBuffer, 0);

  return 0;
}

void receive_commands()
{
  while(1)
  {
    int command = -1;
    int recvLen = sceNetRecv(_cli_sock, &command, sizeof(int), 0);
    if(recvLen <= 0)
    {
      psvDebugScreenPrintf("psvdmac5: failed to receive data. client could have disconnected.\n");
      return;
    }
	  
    switch(command)
    {
    case PSVDMAC5_COMMAND_PING:
      if(handle_command_0() < 0)
      {
        psvDebugScreenPrintf("psvdmac5: failed to handle command 0\n");
        return;
      }
      break;
    case PSVDMAC5_COMMAND_TERM:
      if(handle_command_1() < 0)
      {
        psvDebugScreenPrintf("psvdmac5: failed to handle command 1\n");
        return;
      }
      return;
    case PSVDMAC5_COMMAND_AESECB:
      {
        if(handle_command_2_base(command) < 0)
          return;
      }
      break;
    case PSVDMAC5_COMMAND_AESCBC:
      {
        if(handle_command_3_base(command) < 0)
          return;
      }
      break;
    case PSVDMAC5_COMMAND_HMACSHA1:
      {
        if(handle_command_4_base(command) < 0)
          return;
      }
      break;
    case PSVDMAC5_COMMAND_AESCMAC:
      {
        if(handle_command_5_base(command) < 0)
          return;
      }
      break;
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
