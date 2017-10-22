#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>

#include <Dmac5Client.h>
#include <Dmac5ClientAPI.h>

int test_aes_ecb(SOCKET socket)
{
   char* input = "The gray fox jumped over the dog";
   unsigned char key[0x20] = {0};
   unsigned char output[0x20] = {0};

   if(dmac5::sceSblSsMgrAESECBEncryptWithKeygen(socket, (unsigned char *)input, output, 0x20, key, 0x100, 0, 1) < 0)
      return -1;

   unsigned char result[0x20] = {0};
   if(dmac5::sceSblSsMgrAESECBDecryptWithKeygen(socket, output, result, 0x20, key, 0x100, 0, 1) < 0)
      return -1;

   if(memcmp(input, result, 0x20) != 0)
   {
      std::cout << "Results are not equal" << std::endl;
      return -1;
   }

   return 0;
}

int test_aes_cbc(SOCKET socket)
{
   char* input = "The gray fox jumped over the dog";
   unsigned char key[0x20] = {0};
   unsigned char output[0x20] = {0};
   unsigned char iv[0x10] = {0};

   if(dmac5::sceSblSsMgrAESCBCEncryptWithKeygen(socket, (unsigned char *)input, output, 0x20, key, 0x100, iv, 0, 1) < 0)
      return -1;

   memset(iv, 0, 0x10);

   unsigned char result[0x20] = {0};
   if(dmac5::sceSblSsMgrAESCBCDecryptWithKeygen(socket, output, result, 0x20, key, 0x100, iv, 0, 1) < 0)
      return -1;

   if(memcmp(input, result, 0x20) != 0)
   {
      std::cout << "Results are not equal" << std::endl;
      return -1;
   }

   return 0;
}

int test_hmac_sha1(SOCKET socket)
{
   char* input = "The gray fox jumped over the dog";
   unsigned char key[0x20] = {0};
   unsigned char output[0x14] = {0};
   unsigned char iv[0x10] = {0};

   if(dmac5::sceSblSsMgrHMACSHA1WithKeygen(socket, (unsigned char *)input, output, 0x20, key, 0, 0, 1, 0) < 0)
      return -1;

   return 0;
}

int test_aes_cmac(SOCKET socket)
{
   char* input = "The gray fox jumped over the dog";
   unsigned char key[0x20] = {0};
   unsigned char output[0x10] = {0};

   if(dmac5::sceSblSsMgrAESCMACWithKeygen(socket, (unsigned char *)input, output, 0x20, key, 0x100, 0, 0, 1, 0) < 0)
      return -1;

   return 0;
}

int main(int argc, char* argv[])
{
   SOCKET dmac5_socket = 0;
   if(dmac5::initialize_dmac5_proxy_connection(dmac5_socket) < 0)
      return - 1;

   //Dmac5ExecuteCommand0(dmac5_socket);
   //Dmac5ExecuteCommand1(dmac5_socket);

   test_aes_ecb(dmac5_socket);

   test_aes_cbc(dmac5_socket);

   test_hmac_sha1(dmac5_socket);

   test_aes_cmac(dmac5_socket);

   if(dmac5::deinitialize_dmac5_proxy_connection(dmac5_socket) < 0)
      return - 1;

   return 0;
}

