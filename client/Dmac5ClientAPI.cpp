#include <windows.h>

#include <iostream>
#include <sstream>
#include <string>

#include "Dmac5ClientAPI.h"
#include "Dmac5Client.h"

int send_data(SOCKET socket, unsigned char* src, int size)
{
   int bytesToSend = size;
   int bytesWereSend = 0;

   while(bytesToSend != bytesWereSend)
   {
      int iResult = send(socket, (char*)(src + bytesWereSend), bytesToSend - bytesWereSend, 0);
      if (iResult == SOCKET_ERROR) 
      {
         std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
         closesocket(socket);
         WSACleanup();
         return -1;
      }

      bytesWereSend = bytesWereSend + iResult;
   }
   return 0;
}

int recv_data(SOCKET socket, unsigned char* dest, int size)
{
   int bytesToReceive = size;
   int bytesWereReceived = 0;
   
   while(bytesToReceive != bytesWereReceived)
   {
      int iResult = recv(socket, (char*)(dest + bytesWereReceived), bytesToReceive - bytesWereReceived, 0);
      if (iResult == SOCKET_ERROR) 
      {
         std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
         closesocket(socket);
         WSACleanup();
         return -1;
      }

      bytesWereReceived = bytesWereReceived + iResult;
   }
   return 0;
}

int Dmac5ExecuteCommand0(SOCKET socket)
{
   command_0_request cmd0;
   cmd0.command = PSVDMAC5_COMMAND_PING;

   if(send_data(socket, (unsigned char*)&cmd0, sizeof(command_0_request)) < 0)
      return -1;

   command_0_response resp0;

   if(recv_data(socket, (unsigned char*)&resp0, sizeof(command_0_response)) < 0)
      return -1;

   if(resp0.base.command != PSVDMAC5_COMMAND_PING || resp0.base.vita_err < 0 || resp0.base.proxy_err != 0)
   {
      closesocket(socket);
      WSACleanup();
      return -1;
   }

   std::cout << "Welcome message: " << std::string((char*)resp0.data, 10) << std::endl;

   return 0;
}

int Dmac5ExecuteCommand1(SOCKET socket)
{
   command_1_request cmd1;
   cmd1.command = PSVDMAC5_COMMAND_TERM;

   if(send_data(socket, (unsigned char*)&cmd1, sizeof(command_1_request)) < 0)
      return -1;

   command_1_response resp1;

   if(recv_data(socket, (unsigned char*)&resp1, sizeof(command_1_response)) < 0)
      return -1;

   if(resp1.base.command != PSVDMAC5_COMMAND_TERM || resp1.base.vita_err < 0 || resp1.base.proxy_err != 0)
   {
      closesocket(socket);
      WSACleanup();
      return -1;
   }

   std::cout << "Terminate message: " << std::string((char*)resp1.data, 10) << std::endl;

   return 0;
}

int Dmac5ExecuteCommand2(SOCKET socket, int encrypt, int size, int key_size, uint16_t key_id, int mask_enable, unsigned char* src, unsigned char* key, unsigned char* dest)
{
   //check args
   if(src == 0 || dest == 0 || size == 0 || key == 0 || key_size == 0 || mask_enable != 1)
      return -1;

   //initialize header
   command_2_request cmd2;
   cmd2.command = DMAC5_COMMAND_AESECB;

   cmd2.encrypt = encrypt;
   cmd2.size = size;
   cmd2.key_size = key_size;
   cmd2.key_id = key_id;
   cmd2.mask_enable = mask_enable;

   //send header
   if(send_data(socket, (unsigned char*)&cmd2, sizeof(command_2_request)) < 0)
      return -1;

   //send data buffer - no reason to copy to separate buffer
   if(send_data(socket, src, size) < 0)
      return -1;

   if(send_data(socket, key, (key_size / 8)) < 0)
      return -1;

   //receive header
   command_2_response resp2;

   if(recv_data(socket, (unsigned char*)&resp2, sizeof(command_2_response)) < 0)
      return -1;

   //check result
   if(resp2.base.command != DMAC5_COMMAND_AESECB || resp2.base.vita_err < 0 || resp2.base.proxy_err != 0)
   {
      closesocket(socket);
      WSACleanup();
      return -1;
   }

   //receive additional data - no reason to copy to separate buffer
   if(recv_data(socket, dest, size) < 0)
      return -1;

   return 0;
}

int Dmac5ExecuteCommand3(SOCKET socket, int encrypt, int size, int key_size, uint16_t key_id, int mask_enable, unsigned char* src, unsigned char* key, unsigned char* iv, unsigned char* dest)
{
   //check args
   if(src == 0 || dest == 0 || size == 0 || key == 0 || key_size == 0 || iv == 0 || mask_enable != 1)
      return -1;

   //initialize header
   command_3_request cmd3;
   cmd3.command = DMAC5_COMMAND_AESCBC;

   cmd3.encrypt = encrypt;
   cmd3.size = size;
   cmd3.key_size = key_size;
   cmd3.key_id = key_id;
   cmd3.mask_enable = mask_enable;

   //send header
   if(send_data(socket, (unsigned char*)&cmd3, sizeof(command_3_request)) < 0)
      return -1;

   //send data buffer - no reason to copy to separate buffer
   if(send_data(socket, src, size) < 0)
      return -1;

   if(send_data(socket, key, (key_size / 8)) < 0)
      return -1;

   if(send_data(socket, iv, 0x10) < 0)
      return -1;

   //receive header
   command_3_response resp3;

   if(recv_data(socket, (unsigned char*)&resp3, sizeof(command_3_response)) < 0)
      return -1;

   //check result
   if(resp3.base.command != DMAC5_COMMAND_AESCBC || resp3.base.vita_err < 0 || resp3.base.proxy_err != 0)
   {
      closesocket(socket);
      WSACleanup();
      return -1;
   }

   //receive additional data - no reason to copy to separate buffer
   if(recv_data(socket, dest, size) < 0)
      return -1;

   if(recv_data(socket, iv, 0x10) < 0)
      return -1;

   return 0;
}

int Dmac5ExecuteCommand4(SOCKET socket, int size, uint16_t key_id, int mask_enable, int command_bit, unsigned char* src, unsigned char* key, unsigned char* dest)
{
   //check args
   if(src == 0 || dest == 0 || size == 0 || key == 0 || mask_enable != 1 || command_bit != 0)
      return -1;

   //initialize header
   command_4_request cmd4;
   cmd4.command = DMAC5_COMMAND_HMACSHA1;

   cmd4.size = size;
   cmd4.key_id = key_id;
   cmd4.mask_enable = mask_enable;
   cmd4.command_bit = command_bit;

   //send header
   if(send_data(socket, (unsigned char*)&cmd4, sizeof(command_4_request)) < 0)
      return -1;

   //send data buffer - no reason to copy to separate buffer
   if(send_data(socket, src, size) < 0)
      return -1;

   if(send_data(socket, key, 0x20) < 0)
      return -1;

   //receive header
   command_4_response resp4;

   if(recv_data(socket, (unsigned char*)&resp4, sizeof(command_4_response)) < 0)
      return -1;

   //check result
   if(resp4.base.command != DMAC5_COMMAND_HMACSHA1 || resp4.base.vita_err < 0 || resp4.base.proxy_err != 0)
   {
      closesocket(socket);
      WSACleanup();
      return -1;
   }

   //receive additional data - no reason to copy to separate buffer
   if(recv_data(socket, dest, size) < 0)
      return -1;

   return 0;
}

int Dmac5ExecuteCommand5(SOCKET socket, int size, int key_size, uint16_t key_id, int mask_enable, int command_bit, unsigned char* src, unsigned char* key, unsigned char* dest)
{
   //check args
   if(src == 0 || dest == 0 || size == 0 || key == 0 || key_size == 0 || mask_enable != 1)
      return -1;

   //initialize header
   command_5_request cmd5;
   cmd5.command = DMAC5_COMMAND_AESCMAC;

   cmd5.size = size;
   cmd5.key_size = key_size;
   cmd5.key_id = key_id;
   cmd5.mask_enable = mask_enable;
   cmd5.command_bit = command_bit;

   //send header
   if(send_data(socket, (unsigned char*)&cmd5, sizeof(command_5_request)) < 0)
      return -1;

   //send data buffer - no reason to copy to separate buffer
   if(send_data(socket, src, size) < 0)
      return -1;

   if(send_data(socket, key, (key_size / 8)) < 0)
      return -1;

   //receive header
   command_5_response resp5;

   if(recv_data(socket, (unsigned char*)&resp5, sizeof(command_5_response)) < 0)
      return -1;

   //check result
   if(resp5.base.command != DMAC5_COMMAND_AESCMAC || resp5.base.vita_err < 0 || resp5.base.proxy_err != 0)
   {
      closesocket(socket);
      WSACleanup();
      return -1;
   }

   //receive additional data - no reason to copy to separate buffer
   if(recv_data(socket, dest, size) < 0)
      return -1;

   return 0;
}

//---

int dmac5::sceSblSsMgrAESECBEncryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable)
{
   return Dmac5ExecuteCommand2(socket, 1, size, key_size, key_id, mask_enable, src,key, dst);
}

int dmac5::sceSblSsMgrAESECBDecryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable)
{
   return Dmac5ExecuteCommand2(socket, 0, size, key_size, key_id, mask_enable, src,key, dst);
}

//---

int dmac5::sceSblSsMgrAESCBCEncryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable)
{
   return Dmac5ExecuteCommand3(socket, 1, size,key_size, key_id, mask_enable, src, key, iv, dst);
}

int dmac5::sceSblSsMgrAESCBCDecryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable)
{
   return Dmac5ExecuteCommand3(socket, 0, size, key_size, key_id, mask_enable, src, key, iv, dst);
}

//---

int dmac5::sceSblSsMgrHMACSHA1WithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit)
{
   return Dmac5ExecuteCommand4(socket, size, key_id, mask_enable, command_bit, src, key, dst);
}

int dmac5::sceSblSsMgrAESCMACWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit)
{
   return Dmac5ExecuteCommand5(socket, size, key_size, key_id, mask_enable, command_bit, src, key, dst);
}
