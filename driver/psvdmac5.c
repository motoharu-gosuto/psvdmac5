#include "psvdmac5.h"
 
#include <psp2kern/types.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/io/fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "global_log.h"

int sceSblSsMgrAESECBEncryptWithKeygenForDriverProxy()
{
   return 0;
}

int sceSblSsMgrAESECBDecryptWithKeygenForDriverProxy()
{
   return 0;
}

int sceSblSsMgrAESCBCEncryptWithKeygenForDriverProxy()
{
   return 0;
}

int sceSblSsMgrAESCBCDecryptWithKeygenForDriverProxy()
{
   return 0;
}

int sceSblSsMgrHMACSHA1WithKeygenForDriverProxy()
{
   return 0;
}

int sceSblSsMgrAESCMACWithKeygenForDriverProxy()
{
   return 0;
}

int module_start(SceSize argc, const void *args)
{  
   FILE_GLOBAL_WRITE_LEN("Loaded dmac5 kernel driver");

   return SCE_KERNEL_START_SUCCESS;
}
 
//Alias to inhibit compiler warning
void _start() __attribute__ ((weak, alias ("module_start")));
 
int module_stop(SceSize argc, const void *args) 
{
   return SCE_KERNEL_STOP_SUCCESS;
}
