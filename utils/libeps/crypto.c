#include "proto.h"

void advance_encryption_keys (uint16_t * keys) {
  uint16_t k[4];
  unsigned char p, n;
  for (p = 0; p < 4; p ++) k[p] = keys[p] + p[(uint16_t []) {67, 41, 23, 19}];
  for (p = 0; p < 4; p ++) {
    keys[p] = 0;
    for (n = 0; n < 4; n ++) keys[p] |= ((n > p) ? (k[n] << ((n - p) << 2)) : (k[n] >> ((p - n) << 2))) & (15 << (n << 2));
  }
}

void data_structure_crypto (unsigned char * data, unsigned size, int encrypt) {
  uint16_t keys[4];
  unsigned pos;
  encrypt = encrypt ? 1 : -1;
  for (pos = 0; pos < 4; pos ++) keys[pos] = read_number(data + (pos << 1), 2);
  for (pos = 8; pos < size; pos += 2) {
    write_number(data + pos, 2, read_number(data + pos, 2) + encrypt * keys[(pos >> 1) & 3]);
    if ((pos & 6) == 6) advance_encryption_keys(keys);
  }
}

int validate_data_checksum (unsigned char * data, unsigned size, unsigned offset, int fix) {
  uint32_t current_checksum[16];
  uint32_t checksum[16];
  unsigned pos;
  unsigned char n;
  uint16_t value;
  for (n = 0; n < 16; n ++) current_checksum[n] = read_number(data + offset + (n << 2), 4);
  memset(data + offset, 0, 64);
  memset(checksum, 0, 16 * sizeof(uint32_t));
  for (pos = 0; pos < size; pos += 2) {
    value = read_number(data + pos, 2);
    for (n = 0; n < 16; n ++) if (value & (1 << n)) checksum[n] ++;
  }
  for (n = 0; n < 16; n ++) write_number(data + offset + (n << 2), 4, n[fix ? checksum : current_checksum]);
  if (fix) return 0;
  for (n = 0; n < 16; n ++) if (checksum[n] != current_checksum[n]) return EPSS_INVALID_CHECKSUM;
  return 0;
}

int decrypt_save_file (unsigned char * data) {
  unsigned char slot;
  int rv = 0;
  for (slot = 0; slot < 2; slot ++) {
    data_structure_crypto(data + 0x1c0000 * slot, 0x1c0000, 0);
    if (!rv && validate_data_checksum(data + 0x1c0000 * slot, 0x100, 8, 0)) rv = EPSS_INVALID_HEADER_CHECKSUM;
    if (!rv) rv = validate_data_checksum(data + 0x1c0000 * slot, 0x1c0000, 0x1bff80, 0);
  }
  return rv;
}

void encrypt_save_file (unsigned char * data) {
  unsigned char slot;
  for (slot = 0; slot < 2; slot ++) {
    validate_data_checksum(data + 0x1c0000 * slot, 0x100, 8, 1);
    validate_data_checksum(data + 0x1c0000 * slot, 0x1c0000, 0x1bff80, 1);
    data_structure_crypto(data + 0x1c0000 * slot, 0x1c0000, 1);
  }
}
