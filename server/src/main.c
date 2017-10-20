#include <stdio.h>
#include <malloc.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>

#include <psvdmac5.h>

#include "debugScreen.h"

int main(int argc, char *argv[])
{
   psvDebugScreenInit();

   psvDebugScreenPrintf("psvdmac5: server started\n");

   return 0;
}
