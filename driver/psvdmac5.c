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
  //copy arguments to kernel
  sceSblSsMgrAESECBWithKeygenForDriverProxy_args kargs;
  int res0 = ksceKernelMemcpyUserToKernel(&kargs, (uintptr_t)args, sizeof(sceSblSsMgrAESECBWithKeygenForDriverProxy_args));
  if(res0 < 0)
    return -1;

  //allocate source buffer
  SceUID aes_src_uid = ksceKernelAllocMemBlock("aes_src", SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW, kargs.size, NULL);
  if(aes_src_uid < 0)
  {
    return -1;
  }

  void* aes_src = 0;

  int res1 = ksceKernelGetMemBlockBase(aes_src_uid, &aes_src);
  if(res1 < 0)
  {
    return -1;
  }

  //allocate dest buffer
  SceUID aes_dst_uid = ksceKernelAllocMemBlock("aes_dst", SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW, kargs.size, NULL);
  if(aes_dst_uid < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid);
    return -1;
  }

  void* aes_dst = 0;

  int res2 = ksceKernelGetMemBlockBase(aes_dst_uid, &aes_dst);
  if(res2 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid);
    return -1;
  }

  //allocate key buffer
  SceUID aes_key_uid = ksceKernelAllocMemBlock("aes_key", SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW, kargs.key_size / 8, NULL);
  if(aes_key_uid < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid);
    return -1;
  }

  void* aes_key = 0;
  
  int res3 = ksceKernelGetMemBlockBase(aes_key_uid, &aes_key);
  if(res3 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid);
    return -1;
  }

  //copy source to kernel
  int res4 = ksceKernelMemcpyUserToKernel(aes_src, (uintptr_t)kargs.src, kargs.size);
  if(res4 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //copy key to kernel
  int res5 = ksceKernelMemcpyUserToKernel(aes_key, (uintptr_t)kargs.key, kargs.key_size / 8);
  if(res5 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //call function
  int result = sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy(aes_src, aes_dst, kargs.size, aes_key, kargs.key_size, kargs.key_id, kargs.mask_enable);
  if(result < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //copy result to dest
  int res6 = ksceKernelMemcpyKernelToUser((uintptr_t)kargs.dst, aes_dst, kargs.size);
  if(res6 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //deallocate memblocks
  ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);

  return result;
}

int _sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(sceSblSsMgrAESECBWithKeygenForDriverProxy_args* args)
{
  //copy arguments to kernel
  sceSblSsMgrAESECBWithKeygenForDriverProxy_args kargs;
  int res0 = ksceKernelMemcpyUserToKernel(&kargs, (uintptr_t)args, sizeof(sceSblSsMgrAESECBWithKeygenForDriverProxy_args));
  if(res0 < 0)
    return -1;

  //allocate source buffer
  SceUID aes_src_uid = ksceKernelAllocMemBlock("aes_src", SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW, kargs.size, NULL);
  if(aes_src_uid < 0)
  {
    return -1;
  }

  void* aes_src = 0;

  int res1 = ksceKernelGetMemBlockBase(aes_src_uid, &aes_src);
  if(res1 < 0)
  {
    return -1;
  }

  //allocate dest buffer
  SceUID aes_dst_uid = ksceKernelAllocMemBlock("aes_dst", SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW, kargs.size, NULL);
  if(aes_dst_uid < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid);
    return -1;
  }

  void* aes_dst = 0;

  int res2 = ksceKernelGetMemBlockBase(aes_dst_uid, &aes_dst);
  if(res2 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid);
    return -1;
  }

  //allocate key buffer
  SceUID aes_key_uid = ksceKernelAllocMemBlock("aes_key", SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW, kargs.key_size / 8, NULL);
  if(aes_key_uid < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid);
    return -1;
  }

  void* aes_key = 0;
  
  int res3 = ksceKernelGetMemBlockBase(aes_key_uid, &aes_key);
  if(res3 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid);
    return -1;
  }

  //copy source to kernel
  int res4 = ksceKernelMemcpyUserToKernel(aes_src, (uintptr_t)kargs.src, kargs.size);
  if(res4 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //copy key to kernel
  int res5 = ksceKernelMemcpyUserToKernel(aes_key, (uintptr_t)kargs.key, kargs.key_size / 8);
  if(res5 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //call function
  int result = sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy(aes_src, aes_dst, kargs.size, aes_key, kargs.key_size, kargs.key_id, kargs.mask_enable);
  if(result < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //copy result to dest
  int res6 = ksceKernelMemcpyKernelToUser((uintptr_t)kargs.dst, aes_dst, kargs.size);
  if(res6 < 0)
  {
    ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);
    return -1;
  }

  //deallocate memblocks
  ksceKernelFreeMemBlock(aes_src_uid); ksceKernelFreeMemBlock(aes_dst_uid); ksceKernelFreeMemBlock(aes_key_uid);

  return result;
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
