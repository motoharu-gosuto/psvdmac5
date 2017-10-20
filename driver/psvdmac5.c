#include "psvdmac5.h"
 
#include <psp2kern/types.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/io/fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <taihen.h>
#include <module.h>

#include "global_log.h"

#define SceSblSsMgrForDriver_NID 0x61E9428D

typedef int (sceSblSsMgrAESECBEncryptWithKeygenForDriver_t)(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable);
typedef int (sceSblSsMgrAESECBDecryptWithKeygenForDriver_t)(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable);

typedef int (sceSblSsMgrAESCBCEncryptWithKeygenForDriver_t)(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);
typedef int (sceSblSsMgrAESCBCDecryptWithKeygenForDriver_t)(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);

typedef int (sceSblSsMgrHMACSHA1WithKeygenForDriver_t)(unsigned char *src, unsigned char *dst, int size, unsigned char *key, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);
typedef int (sceSblSsMgrAESCMACWithKeygenForDriver_t)(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);

sceSblSsMgrAESECBEncryptWithKeygenForDriver_t* sceSblSsMgrAESECBEncryptWithKeygenForDriver = 0;
sceSblSsMgrAESECBDecryptWithKeygenForDriver_t* sceSblSsMgrAESECBDecryptWithKeygenForDriver = 0;
sceSblSsMgrAESCBCEncryptWithKeygenForDriver_t* sceSblSsMgrAESCBCEncryptWithKeygenForDriver = 0;
sceSblSsMgrAESCBCDecryptWithKeygenForDriver_t* sceSblSsMgrAESCBCDecryptWithKeygenForDriver = 0;
sceSblSsMgrHMACSHA1WithKeygenForDriver_t* sceSblSsMgrHMACSHA1WithKeygenForDriver = 0;
sceSblSsMgrAESCMACWithKeygenForDriver_t* sceSblSsMgrAESCMACWithKeygenForDriver = 0;

int initialize_functions()
{
  int res = module_get_export_func(KERNEL_PID, "SceSblSsMgr", SceSblSsMgrForDriver_NID, 0x0F7D28AF, (uintptr_t*)&sceSblSsMgrAESECBEncryptWithKeygenForDriver);
  if(res < 0)
  {
    snprintf(sprintfBuffer, 256, "failed to set sceSblSsMgrAESECBEncryptWithKeygenForDriver : %x\n", res);
    FILE_GLOBAL_WRITE_LEN(sprintfBuffer);
    return -1;
  }

  res = module_get_export_func(KERNEL_PID, "SceSblSsMgr", SceSblSsMgrForDriver_NID, 0x197ACF6F, (uintptr_t*)&sceSblSsMgrAESECBDecryptWithKeygenForDriver);
  if(res < 0)
  {
    snprintf(sprintfBuffer, 256, "failed to set sceSblSsMgrAESECBDecryptWithKeygenForDriver : %x\n", res);
    FILE_GLOBAL_WRITE_LEN(sprintfBuffer);
    return -1;
  }

  res = module_get_export_func(KERNEL_PID, "SceSblSsMgr", SceSblSsMgrForDriver_NID, 0x711C057A, (uintptr_t*)&sceSblSsMgrAESCBCEncryptWithKeygenForDriver);
  if(res < 0)
  {
    snprintf(sprintfBuffer, 256, "failed to set sceSblSsMgrAESCBCEncryptWithKeygenForDriver : %x\n", res);
    FILE_GLOBAL_WRITE_LEN(sprintfBuffer);
    return -1;
  }

  res = module_get_export_func(KERNEL_PID, "SceSblSsMgr", SceSblSsMgrForDriver_NID, 0x1901CB5E, (uintptr_t*)&sceSblSsMgrAESCBCDecryptWithKeygenForDriver);
  if(res < 0)
  {
    snprintf(sprintfBuffer, 256, "failed to set sceSblSsMgrAESCBCDecryptWithKeygenForDriver : %x\n", res);
    FILE_GLOBAL_WRITE_LEN(sprintfBuffer);
    return -1;
  }

  res = module_get_export_func(KERNEL_PID, "SceSblSsMgr", SceSblSsMgrForDriver_NID, 0x92E37656, (uintptr_t*)&sceSblSsMgrHMACSHA1WithKeygenForDriver);
  if(res < 0)
  {
    snprintf(sprintfBuffer, 256, "failed to set sceSblSsMgrHMACSHA1WithKeygenForDriver : %x\n", res);
    FILE_GLOBAL_WRITE_LEN(sprintfBuffer);
    return -1;
  }

  res = module_get_export_func(KERNEL_PID, "SceSblSsMgr", SceSblSsMgrForDriver_NID, 0x83B058F5, (uintptr_t*)&sceSblSsMgrAESCMACWithKeygenForDriver);
  if(res < 0)
  {
    snprintf(sprintfBuffer, 256, "failed to set sceSblSsMgrAESCMACWithKeygenForDriver : %x\n", res);
    FILE_GLOBAL_WRITE_LEN(sprintfBuffer);
    return -1;
  }

  return 0;
}

// ====== kernel level functions ======

int sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable)
{
  return sceSblSsMgrAESECBEncryptWithKeygenForDriver(src, dst, size, key, key_size, key_id, mask_enable);
}

int sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable)
{
  return sceSblSsMgrAESECBDecryptWithKeygenForDriver(src, dst, size, key, key_size, key_id, mask_enable);
}

int sceSblSsMgrAESCBCEncryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable)
{
  return sceSblSsMgrAESCBCEncryptWithKeygenForDriver(src, dst, size, key, key_size, iv, key_id, mask_enable);
}

int sceSblSsMgrAESCBCDecryptWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable)
{
  return sceSblSsMgrAESCBCDecryptWithKeygenForDriver(src, dst, size, key, key_size, iv, key_id, mask_enable);
}

int sceSblSsMgrHMACSHA1WithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit)
{
  return sceSblSsMgrHMACSHA1WithKeygenForDriver(src, dst, size, key, iv, key_id, mask_enable, command_bit);
}

int sceSblSsMgrAESCMACWithKeygenForDriverProxy(unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit)
{
  return sceSblSsMgrAESCMACWithKeygenForDriver(src, dst, size, key, key_size, iv, key_id, mask_enable, command_bit);
}

// ====== user level functions ======

int _sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy(sceSblSsMgrAESECBWithKeygenForDriverProxy_args* args)
{
  //unwrap args and call kernel function
  return 0;
}

int _sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(sceSblSsMgrAESECBWithKeygenForDriverProxy_args* args)
{
  //unwrap args and call kernel function
  return 0;
}

int _sceSblSsMgrAESCBCEncryptWithKeygenForDriverProxy(sceSblSsMgrAESCBCWithKeygenForDriverProxy_args* args)
{
  //unwrap args and call kernel function
  return 0;
}

int _sceSblSsMgrAESCBCDecryptWithKeygenForDriverProxy(sceSblSsMgrAESCBCWithKeygenForDriverProxy_args* args)
{
  //unwrap args and call kernel function
  return 0;
}

int _sceSblSsMgrHMACSHA1WithKeygenForDriverProxy(sceSblSsMgrHMACSHA1WithKeygenForDriverProxy_args* args)
{
  //unwrap args and call kernel function
  return 0;  
}

int _sceSblSsMgrAESCMACWithKeygenForDriverProxy(sceSblSsMgrAESCMACWithKeygenForDriverProxy_args* args)
{
  //unwrap args and call kernel function
  return 0;
}

// ====== main ======

int module_start(SceSize argc, const void *args)
{  
   FILE_GLOBAL_WRITE_LEN("Loaded dmac5 kernel driver");

   initialize_functions();

   return SCE_KERNEL_START_SUCCESS;
}
 
//Alias to inhibit compiler warning
void _start() __attribute__ ((weak, alias ("module_start")));
 
int module_stop(SceSize argc, const void *args) 
{
   return SCE_KERNEL_STOP_SUCCESS;
}
