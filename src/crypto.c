#include "crypto.h"

void xtea_encrypt_block(uint32_t v[2], const uint32_t key[4]) {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < 64; i++) {
    v[0] += (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + key[sum & 3]);
    sum += 0x9E3779B9; // golden ratio (delta)
    v[1] += (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + key[(sum >> 11) & 3]);
  }
}

void xtea_decrypt_block(uint32_t v[2], const uint32_t key[4]) {
  uint32_t sum = 0x9E3779B9 * 64;
  for (uint8_t i = 0; i < 64; i++) {
    v[1] -= (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + key[(sum >> 11) & 3]);
    sum -= 0x9E3779B9;
    v[0] -= (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + key[sum & 3]);
  }
}

void encrypt_v0(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]) {

  uint32_t original_iv[2] = {iv[0], iv[1]};
  uint32_t tmp[2]; // nonce (iv[0]) and counter (iv[1])

  for (uint32_t i = 0; i < data->size / 4; i += 2) {
    tmp[0] = iv[0];
    tmp[1] = iv[1]++;

    // encrypt tmp with key using xtea
    xtea_encrypt_block(tmp, key);

    // xor v with tmp
    data->txt[i] ^= tmp[0];
    data->txt[i + 1] ^= tmp[1];
  }

  // restore original iv (for time measurement and tests etc.)
  iv[0] = original_iv[0];
  iv[1] = original_iv[1];
}

void decrypt_v0(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]) {
  encrypt_v0(data, key, iv);
}

void encrypt_v1(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]) {

  uint32_t original_iv[2] = {iv[0], iv[1]};

  for (uint32_t i = 0; i < data->size / 4; i += 2) {

    // xor plain with iv
    data->txt[i] ^= iv[0];
    data->txt[i + 1] ^= iv[1];

    // encrypt plain with key using xtea
    xtea_encrypt_block(data->txt + i, key);

    // cipher into iv for next round
    iv[0] = data->txt[i];
    iv[1] = data->txt[i + 1];
  }

  iv[0] = original_iv[0];
  iv[1] = original_iv[1];
}

void decrypt_v1(struct data_struct *data, const uint32_t key[4],
                uint32_t iv[2]) {
  uint32_t original_iv[2] = {iv[0], iv[1]};
  uint32_t tmp[2]; // save cipher for next rounds iv
  for (uint32_t i = 0; i < data->size / 4; i += 2) {

    // use cipher as iv for next round
    tmp[0] = data->txt[i];
    tmp[1] = data->txt[i + 1];

    // decrypt cipher with key using xtea
    xtea_decrypt_block(data->txt + i, key);

    // xor cipher with iv
    data->txt[i] ^= iv[0];
    data->txt[i + 1] ^= iv[1];

    // cipher into iv for next round
    iv[0] = tmp[0];
    iv[1] = tmp[1];
  }

  iv[0] = original_iv[0];
  iv[1] = original_iv[1];
}

void add_padding(struct data_struct *data) {

  if (data->size % 8 == 0) {
    data->txt[data->size / 4] = 0x08080808;
    data->txt[data->size / 4 + 1] = 0x08080808;
    data->size += 8;
  } else {

    uint32_t padding = 8 - (data->size % 8);

    // calculate end pointer as char pointer
    char *end_ptr = (char *)&data->txt[data->size / 4] + data->size % 4;

    // fill with padding
    for (uint32_t i = 0; i < padding; i++) {
      *end_ptr = padding;
      end_ptr++;
    }

    data->size += padding;
  }
}

void remove_padding(struct data_struct *data) {

  uint32_t padding = (data->txt[data->size / 4 - 1] & 0xff000000) >> 24;

  // this could happen if the file was e.g. not encrypted using our
  // program and the unpadding will fail to prevent segfaults

  if (padding > 8) {
    fprintf(stderr,
            "Aborting Execution...\n"
            "ERROR: Unpadding\n"
            "Padding is not Valid: %d\n"
            "Last Block: %08x %08x\n",
            padding, data->txt[data->size / 4 - 2],
            data->txt[data->size / 4 - 1]);
    exit(1);
  }
  data->size -= padding; // just moving end pointer
}
