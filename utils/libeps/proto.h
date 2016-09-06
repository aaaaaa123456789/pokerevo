#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef __GNUC__
  #pragma GCC visibility push(hidden)
#endif

#include "struct.h"

// public.c
#define ___EXPORT_INTERFACE
#include "libeps.h"

// base.c
uintmax_t read_number(const unsigned char *, unsigned char);
void write_number(unsigned char *, unsigned char, uintmax_t);
void convert_data_to_savefile(unsigned char *, SaveFile *);
void convert_savefile_to_data(SaveFile *, unsigned char *);
int validate_offset_and_width(unsigned, int, unsigned);

// crypto.c
void advance_encryption_keys(uint16_t *);
void data_structure_crypto(unsigned char *, unsigned, int);
int validate_data_checksum(unsigned char *, unsigned, unsigned, int);
int decrypt_save_file(unsigned char *);
void encrypt_save_file(unsigned char *);

// file.c
int read_save_from_file(const char *, SaveFile **);
int read_pokemon_from_file(const char *, Pokemon **);
int open_file_check_size(const char *, unsigned, FILE **);
int write_save_to_file(SaveFile *, const char *);

// pokemon.c
void convert_data_to_pokemon(const unsigned char *, Pokemon *);
void convert_pokemon_to_data(Pokemon *, unsigned char *);
unsigned short calculate_pokemon_checksum(Pokemon *);
int access_pokemon_data_by_position(Pokemon *, unsigned, unsigned, unsigned, unsigned, int, unsigned *);
int access_pokemon_data(Pokemon *, int, int, int, unsigned *);

// savedata.c
int read_data_from_savefile(SaveFile *, void *, unsigned, unsigned);
int write_data_to_savefile(SaveFile *, const void *, unsigned, unsigned);
int read_data_from_saveslot(SaveFile *, void *, unsigned, unsigned);
int write_data_to_saveslot(SaveFile *, const void *, unsigned, unsigned);
int is_slot_empty(SaveFile *, unsigned char);
int erase_save_slot(SaveFile *, unsigned char);
int copy_save_slot(SaveFile *, unsigned char, unsigned char);

// text.c
void convert_buffer_to_text(const void *, unsigned, char *);
int convert_text_to_buffer(const char *, unsigned, void *);
