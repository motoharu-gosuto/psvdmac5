#pragma once

#define PSVDMAC5_COMMAND_PING 0
#define PSVDMAC5_COMMAND_TERM 1
#define PSVDMAC5_COMMAND_AESECB 2
#define PSVDMAC5_COMMAND_AESCBC 3
#define PSVDMAC5_COMMAND_HMACSHA1 4
#define PSVDMAC5_COMMAND_AESCMAC 5

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
    int command;
    int vita_err;
    int proxy_err;

    unsigned char data[]; // dst

} command_2_response;

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
    int command;
    int vita_err;
    int proxy_err;

    unsigned char data[]; // dst

} command_3_response;

typedef struct command_4_request
{
  int command;

  int size;
  uint16_t key_id;
  int mask_enable;
  int command_bit;

  unsigned char data[]; // src, key, iv

} command_4_request;

typedef struct command_4_response
{
    int command;
    int vita_err;
    int proxy_err;

    unsigned char data[]; // dst

} command_4_response;

typedef struct command_5_request
{
  int command;

  int size;
  int key_size;
  uint16_t key_id;
  int mask_enable;
  int command_bit;

  unsigned char data[]; // src, key, iv

} command_5_request;

typedef struct command_5_response
{
    int command;
    int vita_err;
    int proxy_err;

    unsigned char data[]; // dst

} command_5_response;

#pragma pack(pop)