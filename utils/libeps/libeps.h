#ifndef LIBEPS

#define LIBEPS

#ifdef _WIN32
  #ifdef ___EXPORT_INTERFACE
    #define interface __stdcall __declspec(dllexport)
  #else
    #define interface __stdcall __declspec(dllimport)
  #endif
#elif defined(__GNUC__)
  #define interface __attribute__ ((visibility("default")))
#endif

enum epsx_error_codes {
  EPSS_OK                                       =          0,
  EPSS_OUT_OF_MEMORY                            = 0x80000001,
  EPSS_INVALID_ARGUMENT                         = 0x80000101,
  EPSS_INDEX_OUT_OF_RANGE                       = 0x80000102,
  EPSS_NOT_IMPLEMENTED                          = 0x80000103,
  EPSS_NULL_POINTER                             = 0x80000104,
  EPSS_STRING_TOO_LONG                          = 0x80000105,
  EPSS_INVALID_CHARACTERS                       = 0x80000106,
  EPSS_INVALID_CHECKSUM                         = 0x80000201,
  EPSS_INVALID_HEADER_CHECKSUM                  = 0x80000202,
  EPSS_FILE_NOT_FOUND                           = 0x80000301,
  EPSS_WRONG_FILE_SIZE                          = 0x80000302,
  EPSS_READ_ERROR                               = 0x80000303,
  EPSS_ERROR_OPENING_FILE                       = 0x80000304,
  EPSS_WRITE_ERROR                              = 0x80000305,
  EPSS_SLOT_IS_EMPTY                            = 0x80000401,
  EPSS_NO_SLOT_SELECTED                         = 0x80000402
};

enum epsx_operation_modes {
  EPSM_ACTIVE_SAVEFILE        = 0,
  EPSM_BACKUP_SAVEFILE        = 1,
  EPSM_WRITE_BOTH_READ_ACTIVE = 2,
  EPSM_WRITE_BOTH_READ_BACKUP = 3
};

enum epsx_value_kinds {
  EPSK_BYTE                       =  1,
  EPSK_TWO_BYTES                  =  2,
  EPSK_THREE_BYTES                =  3,
  EPSK_FOUR_BYTES                 =  4,
  EPSK_PERSONALITY_VALUE          =  5,
  EPSK_CHECKSUM                   =  6,
  EPSK_SPECIES_NUMBER             =  7,
  EPSK_HELD_ITEM                  =  8,
  EPSK_OT_ID                      =  9,
  EPSK_EXPERIENCE_POINTS          = 10,
  EPSK_FRIENDSHIP                 = 11,
  EPSK_ABILITY                    = 12,
  EPSK_BOX_MARKS                  = 13,
  EPSK_COUNTRY_OF_ORIGIN          = 14,
  EPSK_EFFORT_VALUE               = 15,
  EPSK_CONTEST_STAT               = 16,
  EPSK_RIBBON                     = 17,
  EPSK_MOVE                       = 18,
  EPSK_MOVE_PP                    = 19,
  EPSK_MOVE_PP_UPS                = 20,
  EPSK_INDIVIDUAL_VALUE           = 21,
  EPSK_IS_EGG                     = 22,
  EPSK_IS_NICKNAMED               = 23,
  EPSK_FATEFUL_ENCOUNTER          = 24,
  EPSK_GENDER                     = 25,
  EPSK_FORM                       = 26,
  EPSK_EGG_LOCATION_PLATINUM      = 27,
  EPSK_MET_LOCATION_PLATINUM      = 28,
  EPSK_HOMETOWN                   = 29,
  EPSK_CONTEST_DATA               = 30,
  EPSK_EGG_RECEIVED_DATE          = 31,
  EPSK_MET_DATE                   = 32,
  EPSK_EGG_LOCATION_DIAMOND_PEARL = 33,
  EPSK_MET_LOCATION_DIAMOND_PEARL = 34,
  EPSK_POKERUS                    = 35,
  EPSK_POKE_BALL                  = 36,
  EPSK_MET_LEVEL                  = 37,
  EPSK_OT_GENDER                  = 38,
  EPSK_ENCOUNTER_TYPE             = 39,
  EPSK_POKE_BALL_HG_SS            = 40,
  EPSK_PERFORMANCE                = 41,
  EPSK_HEADER_UNKNOWN             = 42,
  EPSK_SMALL_UNKNOWN              = 43,
  EPSK_BIG_UNKNOWN                = 44
};

enum epsx_value_indices {
  // EVs, IVs
  EPSN_HP                         = 0,
  EPSN_ATTACK                     = 1,
  EPSN_DEFENSE                    = 2,
  EPSN_SPEED                      = 3,
  EPSN_SPECIAL_ATTACK             = 4,
  EPSN_SPECIAL_DEFENSE            = 5,
  // contest stats
  EPSN_COOL                       = 0,
  EPSN_BEAUTY                     = 1,
  EPSN_CUTE                       = 2,
  EPSN_SMART                      = 3,
  EPSN_TOUGH                      = 4,
  EPSN_FEEL                       = 5,
  // OT ID
  EPSN_VISIBLE_ID                 = 0,
  EPSN_SECRET_ID                  = 1
};

#ifdef __cplusplus
  extern "C" {
#endif

interface int epsf_read_save_from_file(const char * filename, void ** save);
interface int epsf_destroy_save(void * save);
interface int epsf_write_save_to_file(void * save, const char * filename);
interface int epsf_current_operation_mode(void * save);
interface int epsf_select_operation_mode(void * save, int mode);
interface int epsf_current_save_slot(void * save);
interface int epsf_select_save_slot(void * save, int slot);
interface int epsf_copy_save_slot(void * save, int from, int to);
interface int epsf_erase_save_slot(void * save, int slot);
interface int epsf_is_save_slot_empty(void * save, int slot);
interface int epsf_get_active_savefile(void * save);
interface int epsf_swap_savefiles(void * save);
interface int epsf_get_encryption_key(void * save, unsigned long long * key);
interface int epsf_set_encryption_key(void * save, unsigned long long key);
interface int epsf_read_savefile_raw(void * save, unsigned offset, int width, unsigned * value);
interface int epsf_write_savefile_raw(void * save, unsigned offset, int width, unsigned value);
interface int epsf_read_saveslot_raw(void * save, unsigned offset, int width, unsigned * value);
interface int epsf_write_saveslot_raw(void * save, unsigned offset, int width, unsigned value);
interface int epsf_new_pokemon(void ** pokemon);
interface int epsf_destroy_pokemon(void * pokemon);
interface int epsf_read_pokemon_from_file(const char * filename, void ** pokemon);
interface int epsf_write_pokemon_to_file(void * pokemon, const char * filename);
interface int epsf_read_pokemon_from_save(void * save, int box, int position, void ** pokemon);
interface int epsf_write_pokemon_to_save(void * save, int box, int position, void * pokemon);
interface int epsf_erase_pokemon_from_save(void * save, int box, int position);
interface int epsf_get_pokemon_name(void * pokemon, int which, char * name);
interface int epsf_set_pokemon_name(void * pokemon, int which, const char * name);
interface int epsf_get_pokemon_value(void * pokemon, int kind, int index, unsigned * value);
interface int epsf_set_pokemon_value(void * pokemon, int kind, int index, unsigned value);
interface int epsf_fix_pokemon_checksum(void * pokemon);

#ifdef __cplusplus
  }
#endif

#endif
