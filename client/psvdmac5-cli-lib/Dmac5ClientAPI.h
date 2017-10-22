#pragma once

#include <stdint.h>

namespace dmac5
{

int sceSblSsMgrAESECBEncryptWithKeygen(SOCKET socket, const unsigned char *src, unsigned char *dst, int size, const unsigned char *key, int key_size, uint16_t key_id, int mask_enable);
int sceSblSsMgrAESECBDecryptWithKeygen(SOCKET socket, const unsigned char *src, unsigned char *dst, int size, const unsigned char *key, int key_size, uint16_t key_id, int mask_enable);

int sceSblSsMgrAESCBCEncryptWithKeygen(SOCKET socket, const unsigned char *src, unsigned char *dst, int size, const unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);
int sceSblSsMgrAESCBCDecryptWithKeygen(SOCKET socket, const unsigned char *src, unsigned char *dst, int size, const unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable);

int sceSblSsMgrHMACSHA1WithKeygen(SOCKET socket, const unsigned char *src, unsigned char *dst, int size, const unsigned char *key, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);
int sceSblSsMgrAESCMACWithKeygen(SOCKET socket, const unsigned char *src, unsigned char *dst, int size, const unsigned char *key, int key_size, unsigned char *iv, uint16_t key_id, int mask_enable, int command_bit);

}