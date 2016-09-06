#include "proto.h"

interface int epsf_read_save_from_file (const char * filename, void ** save) {
  if (save) *save = NULL;
  if (!(filename && save)) return EPSS_NULL_POINTER;
  return read_save_from_file(filename, (SaveFile **) save);
}

interface int epsf_destroy_save (void * save) {
  if (!save) return EPSS_NULL_POINTER;
  free(save);
  return EPSS_OK;
}

interface int epsf_write_save_to_file (void * save, const char * filename) {
  if (!(save && filename)) return EPSS_NULL_POINTER;
  return write_save_to_file(save, filename);
}

interface int epsf_current_operation_mode (void * save) {
  if (!save) return EPSS_NULL_POINTER;
  return ((SaveFile *) save) -> current_mode;
}

interface int epsf_select_operation_mode (void * save, int mode) {
  if (!save) return EPSS_NULL_POINTER;
  if (mode & ~3) return EPSS_INVALID_ARGUMENT;
  ((SaveFile *) save) -> current_mode = mode;
  return EPSS_OK;
}

interface int epsf_current_save_slot (void * save) {
  if (!save) return EPSS_NULL_POINTER;
  return ((SaveFile *) save) -> current_subslot;
}

interface int epsf_select_save_slot (void * save, int slot) {
  if (!save) return EPSS_NULL_POINTER;
  if ((slot < 0) || (slot > 4)) return EPSS_INDEX_OUT_OF_RANGE;
  if (slot && is_slot_empty(save, slot)) return EPSS_SLOT_IS_EMPTY;
  ((SaveFile *) save) -> current_subslot = slot;
  return EPSS_OK;
}

interface int epsf_copy_save_slot (void * save, int from, int to) {
  if (!save) return EPSS_NULL_POINTER;
  if ((from < 1) || (from > 4) || (to < 1) || (to > 4)) return EPSS_INDEX_OUT_OF_RANGE;
  if (from == to) return EPSS_OK;
  return copy_save_slot(save, from, to);
}

interface int epsf_erase_save_slot (void * save, int slot) {
  if (!save) return EPSS_NULL_POINTER;
  if ((slot < 1) || (slot > 4)) return EPSS_INDEX_OUT_OF_RANGE;
  SaveFile * sf = save;
  int rv = erase_save_slot(sf, slot);
  if (rv) return rv;
  if (sf -> current_subslot == slot) sf -> current_subslot = 0;
  return EPSS_OK;
}

interface int epsf_is_save_slot_empty (void * save, int slot) {
  if (!save) return EPSS_NULL_POINTER;
  if ((slot < 1) || (slot > 4)) return EPSS_INDEX_OUT_OF_RANGE;
  return is_slot_empty(save, slot);
}

interface int epsf_get_active_savefile (void * save) {
  if (!save) return EPSS_NULL_POINTER;
  SaveFile * sf = save;
  return sf -> current_slot ^ sf -> swap_saveslots;
}

interface int epsf_swap_savefiles (void * save) {
  if (!save) return EPSS_NULL_POINTER;
  ((SaveFile *) save) -> swap_saveslots ^= 1;
  return EPSS_OK;
}

interface int epsf_get_encryption_key (void * save, unsigned long long * key) {
  if (!(save && key)) return EPSS_NULL_POINTER;
  char data[8];
  int rv = read_data_from_savefile(save, data, 8, 0);
  if (rv) return rv;
  *key = read_number(data, 8);
  return EPSS_OK;
}

interface int epsf_set_encryption_key (void * save, unsigned long long key) {
  if (!save) return EPSS_NULL_POINTER;
  char data[8];
  write_number(data, 8, key);
  return write_data_to_savefile(save, data, 8, 0);
}

interface int epsf_read_savefile_raw (void * save, unsigned offset, int width, unsigned * value) {
  if (!(save && value)) return EPSS_NULL_POINTER;
  int rv;
  if (rv = validate_offset_and_width(offset, width, 0x1c0000)) return rv;
  char data[4];
  rv = read_data_from_savefile(save, data, width, offset);
  if (rv) return rv;
  *value = read_number(data, width);
  return EPSS_OK;
}

interface int epsf_write_savefile_raw (void * save, unsigned offset, int width, unsigned value) {
  if (!save) return EPSS_NULL_POINTER;
  int rv;
  if (rv = validate_offset_and_width(offset, width, 0x1c0000)) return rv;
  char data[4];
  write_number(data, width, value);
  return write_data_to_savefile(save, data, width, offset);
}

interface int epsf_read_saveslot_raw (void * save, unsigned offset, int width, unsigned * value) {
  if (!(save && value)) return EPSS_NULL_POINTER;
  int rv;
  if (rv = validate_offset_and_width(offset, width, 0x6ff00)) return rv;
  char data[4];
  rv = read_data_from_saveslot(save, data, width, offset);
  if (rv) return rv;
  *value = read_number(data, width);
  return EPSS_OK;
}

interface int epsf_write_saveslot_raw (void * save, unsigned offset, int width, unsigned value) {
  if (!save) return EPSS_NULL_POINTER;
  int rv;
  if (rv = validate_offset_and_width(offset, width, 0x6ff00)) return rv;
  char data[4];
  write_number(data, width, value);
  return write_data_to_saveslot(save, data, width, offset);
}

interface int epsf_new_pokemon (void ** pokemon) {
  if (!pokemon) return EPSS_NULL_POINTER;
  *pokemon = calloc(1, sizeof(Pokemon));
  if (!*pokemon) return EPSS_OUT_OF_MEMORY;
  return EPSS_OK;
}

interface int epsf_destroy_pokemon (void * pokemon) {
  if (!pokemon) return EPSS_NULL_POINTER;
  free(pokemon);
  return EPSS_OK;
}

interface int epsf_read_pokemon_from_file (const char * filename, void ** pokemon) {
  if (pokemon) *pokemon = NULL;
  if (!(filename && pokemon)) return EPSS_NULL_POINTER;
  return read_pokemon_from_file(filename, (Pokemon **) pokemon);
}

interface int epsf_write_pokemon_to_file (void * pokemon, const char * filename) {
  if (!(filename && pokemon)) return EPSS_NULL_POINTER;
  FILE * fp = fopen(filename, "wb");
  if (!fp) return EPSS_ERROR_OPENING_FILE;
  char data[0x88];
  convert_pokemon_to_data(pokemon, data);
  int rv = fwrite(data, 1, 0x88, fp);
  fclose(fp);
  if (rv != 0x88) return EPSS_WRITE_ERROR;
  return EPSS_OK;
}

interface int epsf_read_pokemon_from_buffer (const void * buffer, void ** pokemon) {
  if (pokemon) *pokemon = NULL;
  if (!(buffer && pokemon)) return EPSS_NULL_POINTER;
  *pokemon = malloc(0x88);
  if (!*pokemon) return EPSS_OUT_OF_MEMORY;
  convert_data_to_pokemon(buffer, *pokemon);
  return EPSS_OK;
}

interface int epsf_write_pokemon_to_buffer (void * pokemon, void * buffer) {
  if (!(buffer && pokemon)) return EPSS_NULL_POINTER;
  convert_pokemon_to_data(pokemon, buffer);
  return EPSS_OK;
}

interface int epsf_read_pokemon_from_save (void * save, int box, int position, void ** pokemon) {
  if (pokemon) *pokemon = NULL;
  if (!(save && pokemon)) return EPSS_NULL_POINTER;
  if ((box < 0) || (box > 18) || (position < 1) || (position > 30) || (!box && (position > 6))) return EPSS_INDEX_OUT_OF_RANGE;
  char data[0x88];
  int rv = read_data_from_saveslot(save, data, 0x88, box ? ((box * 30 + position) * 0x88 - 0xa80) : (position * 0xdc - 0x10));
  if (rv) return rv;
  Pokemon * pkmn = malloc(sizeof(Pokemon));
  if (!pkmn) return EPSS_OUT_OF_MEMORY;
  convert_data_to_pokemon(data, pkmn);
  *pokemon = pkmn;
  return EPSS_OK;
}

interface int epsf_write_pokemon_to_save (void * save, int box, int position, void * pokemon) {
  if (!(save && pokemon)) return EPSS_NULL_POINTER;
  if ((box < 0) || (box > 18) || (position < 1) || (position > 30) || (!box && (position > 6))) return EPSS_INDEX_OUT_OF_RANGE;
  if (!box) return EPSS_NOT_IMPLEMENTED;
  char data[0x88];
  convert_pokemon_to_data(pokemon, data);
  return write_data_to_saveslot(save, data, 0x88, (box * 30 + position) * 0x88 - 0xa80);
}

interface int epsf_erase_pokemon_from_save (void * save, int box, int position) {
  if (!save) return EPSS_NULL_POINTER;
  if ((box < 0) || (box > 18) || (position < 1) || (position > 30) || (!box && (position > 6))) return EPSS_INDEX_OUT_OF_RANGE;
  if (!box) return EPSS_NOT_IMPLEMENTED;
  char data[0x88];
  memset(data, 0, 0x88);
  return write_data_to_saveslot(save, data, 0x88, (box * 30 + position) * 0x88 - 0xa80);
}

interface int epsf_get_pokemon_name (void * pokemon, int which, char * name) {
  if (!(pokemon && name)) return EPSS_NULL_POINTER;
  if (which & ~1) return EPSS_INVALID_ARGUMENT;
  convert_buffer_to_text(((Pokemon *) pokemon) -> data + (which ? 0x60 : 0x40), which ? 8 : 11, name);
  return EPSS_OK;
}

interface int epsf_set_pokemon_name (void * pokemon, int which, const char * name) {
  if (!(pokemon && name)) return EPSS_NULL_POINTER;
  if (which & ~1) return EPSS_INVALID_ARGUMENT;
  char data[22] = {0};
  int rv = convert_text_to_buffer(name, which ? 7 : 10, data);
  if (rv) return rv;
  memcpy(((Pokemon *) pokemon) -> data + (which ? 0x60 : 0x40), data, which ? 16 : 22);
  return EPSS_OK;
}

interface int epsf_get_pokemon_value (void * pokemon, int kind, int index, unsigned * value) {
  if (!(pokemon && value)) return EPSS_NULL_POINTER;
  return access_pokemon_data(pokemon, kind, index, 0, value);
}

interface int epsf_set_pokemon_value (void * pokemon, int kind, int index, unsigned value) {
  if (!pokemon) return EPSS_NULL_POINTER;
  return access_pokemon_data(pokemon, kind, index, 1, &value);
}

interface int epsf_fix_pokemon_checksum (void * pokemon) {
  if (!pokemon) return EPSS_NULL_POINTER;
  unsigned checksum = calculate_pokemon_checksum(pokemon);
  return access_pokemon_data(pokemon, EPSK_CHECKSUM, 0, 1, &checksum);
}
