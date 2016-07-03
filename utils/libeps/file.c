#include "proto.h"

int read_save_from_file (const char * filename, SaveFile ** save) {
  FILE * fp;
  int rv = open_file_check_size(filename, 0x380000, &fp);
  if (rv) return rv;
  void * data = malloc(0x380000);
  SaveFile * savefile = malloc(sizeof(SaveFile));
  if (!(data && savefile)) {
    fclose(fp);
    free(data);
    free(savefile);
    return EPSS_OUT_OF_MEMORY;
  }
  rv = fread(data, 1, 0x380000, fp);
  fclose(fp);
  if (rv != 0x380000) {
    free(data);
    free(savefile);
    return EPSS_READ_ERROR;
  }
  rv = decrypt_save_file(data);
  if (rv) {
    free(data);
    free(savefile);
    return rv;
  }
  convert_data_to_savefile(data, savefile);
  free(data);
  *save = savefile;
  return EPSS_OK;
}

int read_pokemon_from_file (const char * filename, Pokemon ** pokemon) {
  FILE * fp;
  int rv = open_file_check_size(filename, 0x88, &fp);
  if (rv) return rv;
  void * data = malloc(0x88);
  Pokemon * pkmn = malloc(sizeof(Pokemon));
  if (!(data && pkmn)) {
    fclose(fp);
    free(data);
    free(pkmn);
    return EPSS_OUT_OF_MEMORY;
  }
  rv = fread(data, 1, 0x88, fp);
  fclose(fp);
  if (rv != 0x88) {
    free(data);
    free(pkmn);
    return EPSS_READ_ERROR;
  }
  convert_data_to_pokemon(data, pkmn);
  free(data);
  *pokemon = pkmn;
  return EPSS_OK;
}

int open_file_check_size (const char * filename, unsigned size, FILE ** result) {
  FILE * fp = fopen(filename, "rb");
  if (!fp) return EPSS_FILE_NOT_FOUND;
  long filesize;
  if (fseek(fp, 0, 2) || ((filesize = ftell(fp)) == -1)) {
    fclose(fp);
    return EPSS_READ_ERROR;
  }
  (void) fseek(fp, 0, 0);
  if (filesize != size) {
    fclose(fp);
    return EPSS_WRONG_FILE_SIZE;
  }
  *result = fp;
  return EPSS_OK;
}

int write_save_to_file (SaveFile * save, const char * filename) {
  void * data = malloc(0x380000);
  if (!data) return EPSS_OUT_OF_MEMORY;
  convert_savefile_to_data(save, data);
  encrypt_save_file(data);
  FILE * fp = fopen(filename, "wb");
  if (!fp) {
    free(data);
    return EPSS_ERROR_OPENING_FILE;
  }
  int rv = fwrite(data, 1, 0x380000, fp);
  free(data);
  fclose(fp);
  if (rv != 0x380000) return EPSS_WRITE_ERROR;
  return EPSS_OK;
}
