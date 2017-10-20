#pragma once

#include <stdint.h>

typedef struct sceSblSsMgrAESECBWithKeygenForDriverProxy_args
{
  unsigned char *src;
  unsigned char *dst;
  int size;
  unsigned char *key;
  int key_size;
  uint16_t key_id;
  int mask_enable;
}sceSblSsMgrAESECBWithKeygenForDriverProxy_args;

typedef struct sceSblSsMgrAESCBCWithKeygenForDriverProxy_args
{
  unsigned char *src;
  unsigned char *dst;
  int size;
  unsigned char *key;
  int key_size;
  unsigned char *iv;
  uint16_t key_id;
  int mask_enable;
}sceSblSsMgrAESCBCWithKeygenForDriverProxy_args;

typedef struct sceSblSsMgrHMACSHA1WithKeygenForDriverProxy_args
{
  unsigned char *src;
  unsigned char *dst;
  int size;
  unsigned char *key;
  unsigned char *iv;
  uint16_t key_id;
  int mask_enable;
  int command_bit;
} sceSblSsMgrHMACSHA1WithKeygenForDriverProxy_args;

typedef struct sceSblSsMgrAESCMACWithKeygenForDriverProxy_args
{
  unsigned char *src;
  unsigned char *dst;
  int size;
  unsigned char *key;
  int key_size;
  unsigned char *iv;
  uint16_t key_id;
  int mask_enable;
  int command_bit;
} sceSblSsMgrAESCMACWithKeygenForDriverProxy_args;

int _sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy(sceSblSsMgrAESECBWithKeygenForDriverProxy_args* args);
int _sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(sceSblSsMgrAESECBWithKeygenForDriverProxy_args* args);

int _sceSblSsMgrAESCBCEncryptWithKeygenForDriverProxy(sceSblSsMgrAESCBCWithKeygenForDriverProxy_args* args);
int _sceSblSsMgrAESCBCDecryptWithKeygenForDriverProxy(sceSblSsMgrAESCBCWithKeygenForDriverProxy_args* args);

int _sceSblSsMgrHMACSHA1WithKeygenForDriverProxy(sceSblSsMgrHMACSHA1WithKeygenForDriverProxy_args* args);
int _sceSblSsMgrAESCMACWithKeygenForDriverProxy(sceSblSsMgrAESCMACWithKeygenForDriverProxy_args* args);

//---

int sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable);
int sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable);

int sceSblSsMgrAESCBCEncryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);
int sceSblSsMgrAESCBCDecryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);

int sceSblSsMgrHMACSHA1WithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);
int sceSblSsMgrAESCMACWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);