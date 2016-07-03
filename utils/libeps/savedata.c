#include "proto.h"

int read_data_from_savefile (SaveFile * savefile, void * data, unsigned length, unsigned offset) {
  char * slot = (char *) (savefile -> slots + ((savefile -> current_mode & 1) ^ (savefile -> current_slot)));
  memcpy(data, slot + offset, length);
  return EPSS_OK;
}

int write_data_to_savefile (SaveFile * savefile, const void * data, unsigned length, unsigned offset) {
  char * slot;
  if (savefile -> current_mode != 1) {
    slot = (char *) (savefile -> slots + savefile -> current_slot);
    memcpy(slot + offset, data, length);
  }
  if (savefile -> current_mode) {
    slot = (char *) (savefile -> slots + (savefile -> current_slot ^ 1));
    memcpy(slot + offset, data, length);
  }
  return EPSS_OK;
}

int read_data_from_saveslot (SaveFile * savefile, void * data, unsigned length, unsigned offset) {
  if (!(savefile -> current_subslot)) return EPSS_NO_SLOT_SELECTED;
  return read_data_from_savefile(savefile, data, length, offset + 0x6ff00 * savefile -> current_subslot - 0x6fb80);
}

int write_data_to_saveslot (SaveFile * savefile, const void * data, unsigned length, unsigned offset) {
  if (!(savefile -> current_subslot)) return EPSS_NO_SLOT_SELECTED;
  return write_data_to_savefile(savefile, data, length, offset + 0x6ff00 * savefile -> current_subslot - 0x6fb80);
}

int is_slot_empty (SaveFile * save, unsigned char slot) {
  SaveSubslot * subslot = save -> slots[(save -> current_mode & 1) ^ (save -> current_slot)].subslots + (slot - 1);
  unsigned p;
  for (p = 0; p < 0x6ff00; p ++) if (p[*subslot]) return 0;
  return 1;
}

int erase_save_slot (SaveFile * save, unsigned char slot) {
  void * data = calloc(1, 0x6ff00);
  if (!data) return EPSS_OUT_OF_MEMORY;
  int rv = write_data_to_savefile(save, data, 0x6ff00, 0x6ff00 * slot - 0x6fb80);
  free(data);
  return rv;
}

int copy_save_slot (SaveFile * save, unsigned char from_slot, unsigned char to_slot) {
  void * data = malloc(0x6ff00);
  if (!data) return EPSS_OUT_OF_MEMORY;
  int rv = read_data_from_savefile(save, data, 0x6ff00, 0x6ff00 * from_slot - 0x6fb80);
  if (rv) {
    free(data);
    return rv;
  }
  rv = write_data_to_savefile(save, data, 0x6ff00, 0x6ff00 * to_slot - 0x6fb80);
  free(data);
  return rv;
}
