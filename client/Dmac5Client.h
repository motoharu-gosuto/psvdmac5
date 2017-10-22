#pragma once

#include <stdint.h>

//TODO: need to merge server and client definitions into single file
//these are much better than original on server

#define PSVDMAC5_COMMAND_PING 0
#define PSVDMAC5_COMMAND_TERM 1

#define DMAC5_COMMAND_AESECB 2
#define DMAC5_COMMAND_AESCBC 3
#define DMAC5_COMMAND_HMACSHA1 4
#define DMAC5_COMMAND_AESCMAC 5

#pragma pack(push, 1)

struct command_response_base
{
   uint32_t command;
   uint32_t vita_err;
   uint32_t proxy_err;
};

//connection test command
struct command_0_request
{
   uint32_t command;
};

struct command_0_response
{
   command_response_base base;
   uint8_t data[10];
};

//connection terminate command
struct command_1_request
{
   uint32_t command;
};

struct command_1_response
{
   command_response_base base;
   uint8_t data[10];
};

//aes-ecb command
typedef struct command_2_request
{
  int command;

  int encrypt;

  int size;
  int key_size;
  uint16_t key_id;
  int mask_enable;

  unsigned char data[]; // src, key
  
} command_2_request;

typedef struct command_2_response
{
    command_response_base base;
    unsigned char data[]; // dst

} command_2_response;

//aes-cbc command
typedef struct command_3_request
{
  int command;

  int encrypt;

  int size;
  int key_size;
  uint16_t key_id;
  int mask_enable;

  unsigned char data[]; // src, key, iv
  
} command_3_request;

typedef struct command_3_response
{
    command_response_base base;
    unsigned char data[]; // dst

} command_3_response;

//hmac-sha1 command
typedef struct command_4_request
{
  int command;

  int size;
  uint16_t key_id;
  int mask_enable;
  int command_bit;

  unsigned char data[]; // src, key

} command_4_request;

typedef struct command_4_response
{
    command_response_base base;
    unsigned char data[]; // dst

} command_4_response;

//aes-cmac command
typedef struct command_5_request
{
  int command;

  int size;
  int key_size;
  uint16_t key_id;
  int mask_enable;
  int command_bit;

  unsigned char data[]; // src, key

} command_5_request;

typedef struct command_5_response
{
    command_response_base base;
    unsigned char data[]; // dst

} command_5_response;

//--------------------------------

#pragma pack(pop)

int initialize_dmac5_proxy_connection(SOCKET& ConnectSocket);

int deinitialize_dmac5_proxy_connection(SOCKET ConnectSocket);