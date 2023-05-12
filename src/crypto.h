#pragma once
#include <emmintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELTA 0x9E3779B9 // 2^32 / golden ratio used by xtea
#define BLOCK_SIZE 8     // 64 bit blocks used by xtea

#include "util.h"

/**
 * @brief encrypts a block with xtea
 *
 * @param v v[0] and v[1] are the two 32 bit blocks to encrypt
 * @param key key to use for encryption
 */
void xtea_encrypt_block(uint32_t v[2], const uint32_t key[4]);

/**
 * @brief decrypts a block with xtea
 *
 * @param v v[0] and v[1] are the two 32 bit blocks to decrypt
 * @param key key to use for decryption
 */
void xtea_decrypt_block(uint32_t v[2], const uint32_t key[4]);

/**
 * @brief encrypts data with counter mode
 *
 * @param data data struct with txt and size to encrypt
 * @param key key to use for encryption
 * @param iv iv to use for encryption
 */
void encrypt_v0(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]);

/**
 * @brief decrypts data with counter mode
 *
 * @param data data struct with txt and size to encrypt
 * @param key key to use for encryption
 * @param iv iv to use for encryption
 */
void decrypt_v0(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]);

/**
 * @brief encrypts data with cipher block chaining mode
 *
 * @param data data struct with txt and size to encrypt
 * @param key key to use for encryption
 * @param iv iv to use for encryption
 */
void encrypt_v1(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]);

/**
 * @brief decrypts data with cipher block chaining mode
 *
 * @param data data struct with txt and size to encrypt
 * @param key key to use for encryption
 * @param iv iv to use for encryption
 */
void decrypt_v1(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]);

/**
 * @brief adds padding to a string
 *
 * @param data data struct with txt and size to encrypt
 */
void add_padding(struct data_struct *data);

/**
 * @brief removes padding from a string
 *
 * @param data data struct with txt and size to encrypt
 */
void remove_padding(struct data_struct *data);