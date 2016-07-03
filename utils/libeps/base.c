#include "proto.h"

uintmax_t read_number (const unsigned char * buffer, unsigned char size) {
  // big endian!
  uintmax_t result = 0;
  while (size --) result |= *(buffer ++) << (size << 3);
  return result;
}

void write_number (unsigned char * buffer, unsigned char size, uintmax_t number) {
  // big endian!
  while (size --) *(buffer ++) = number >> (size << 3);
}

void convert_data_to_savefile (unsigned char * data, SaveFile * savefile) {
  memset(((char *) savefile) + 0x380000, 0, sizeof(SaveFile) - 0x380000);
  memcpy(savefile -> slots, data, 0x380000);
  uint32_t sc0 = read_number(savefile -> slots -> config + 0x4c, 4);
  uint32_t sc1 = read_number(savefile -> slots[1].config + 0x4c, 4);
  savefile -> current_slot = sc1 > sc0;
}

void convert_savefile_to_data (SaveFile * savefile, unsigned char * data) {
  if (!(savefile -> swap_saveslots)) {
    memcpy(data, savefile -> slots, 0x380000);
    return;
  }
  memcpy(data, savefile -> slots + 1, 0x1c0000);
  memcpy(data + 0x1c0000, savefile -> slots, 0x1c0000);
}

int validate_offset_and_width (unsigned offset, int width, unsigned limit) {
  if ((width < 0) || (width > 4)) return EPSS_INVALID_ARGUMENT;
  if ((offset + width) > limit) return EPSS_INDEX_OUT_OF_RANGE;
  return EPSS_OK;
}
