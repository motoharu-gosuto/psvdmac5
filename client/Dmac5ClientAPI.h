#pragma once

#include <stdint.h>

namespace dmac5
{

int sceSblSsMgrAESECBEncryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable);
int sceSblSsMgrAESECBDecryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, uint16_t key_id, int mask_enable);

int sceSblSsMgrAESCBCEncryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);
int sceSblSsMgrAESCBCDecryptWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);

int sceSblSsMgrHMACSHA1WithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);
int sceSblSsMgrAESCMACWithKeygen(SOCKET socket, unsigned char *src, unsigned char *dst, int size, unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);

}