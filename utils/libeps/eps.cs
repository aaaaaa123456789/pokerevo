using System;
using System.Text;
using System.Runtime.InteropServices;

public static class eps {
  public const int EPSS_OK                                       =                          0;
  public const int EPSS_OUT_OF_MEMORY                            = unchecked((int) 0x80000001);
  public const int EPSS_INVALID_ARGUMENT                         = unchecked((int) 0x80000101);
  public const int EPSS_INDEX_OUT_OF_RANGE                       = unchecked((int) 0x80000102);
  public const int EPSS_NOT_IMPLEMENTED                          = unchecked((int) 0x80000103);
  public const int EPSS_NULL_POINTER                             = unchecked((int) 0x80000104);
  public const int EPSS_STRING_TOO_LONG                          = unchecked((int) 0x80000105);
  public const int EPSS_INVALID_CHARACTERS                       = unchecked((int) 0x80000106);
  public const int EPSS_INVALID_CHECKSUM                         = unchecked((int) 0x80000201);
  public const int EPSS_INVALID_HEADER_CHECKSUM                  = unchecked((int) 0x80000202);
  public const int EPSS_FILE_NOT_FOUND                           = unchecked((int) 0x80000301);
  public const int EPSS_WRONG_FILE_SIZE                          = unchecked((int) 0x80000302);
  public const int EPSS_READ_ERROR                               = unchecked((int) 0x80000303);
  public const int EPSS_ERROR_OPENING_FILE                       = unchecked((int) 0x80000304);
  public const int EPSS_WRITE_ERROR                              = unchecked((int) 0x80000305);
  public const int EPSS_SLOT_IS_EMPTY                            = unchecked((int) 0x80000401);
  public const int EPSS_NO_SLOT_SELECTED                         = unchecked((int) 0x80000402);

  public const int EPSM_ACTIVE_SAVEFILE        = 0;
  public const int EPSM_BACKUP_SAVEFILE        = 1;
  public const int EPSM_WRITE_BOTH_READ_ACTIVE = 2;
  public const int EPSM_WRITE_BOTH_READ_BACKUP = 3;

  public const int EPSK_BYTE                       =  1;
  public const int EPSK_TWO_BYTES                  =  2;
  public const int EPSK_THREE_BYTES                =  3;
  public const int EPSK_FOUR_BYTES                 =  4;
  public const int EPSK_PERSONALITY_VALUE          =  5;
  public const int EPSK_CHECKSUM                   =  6;
  public const int EPSK_SPECIES_NUMBER             =  7;
  public const int EPSK_HELD_ITEM                  =  8;
  public const int EPSK_OT_ID                      =  9;
  public const int EPSK_EXPERIENCE_POINTS          = 10;
  public const int EPSK_FRIENDSHIP                 = 11;
  public const int EPSK_ABILITY                    = 12;
  public const int EPSK_BOX_MARKS                  = 13;
  public const int EPSK_COUNTRY_OF_ORIGIN          = 14;
  public const int EPSK_EFFORT_VALUE               = 15;
  public const int EPSK_CONTEST_STAT               = 16;
  public const int EPSK_RIBBON                     = 17;
  public const int EPSK_MOVE                       = 18;
  public const int EPSK_MOVE_PP                    = 19;
  public const int EPSK_MOVE_PP_UPS                = 20;
  public const int EPSK_INDIVIDUAL_VALUE           = 21;
  public const int EPSK_IS_EGG                     = 22;
  public const int EPSK_IS_NICKNAMED               = 23;
  public const int EPSK_FATEFUL_ENCOUNTER          = 24;
  public const int EPSK_GENDER                     = 25;
  public const int EPSK_FORM                       = 26;
  public const int EPSK_EGG_LOCATION_PLATINUM      = 27;
  public const int EPSK_MET_LOCATION_PLATINUM      = 28;
  public const int EPSK_HOMETOWN                   = 29;
  public const int EPSK_CONTEST_DATA               = 30;
  public const int EPSK_EGG_RECEIVED_DATE          = 31;
  public const int EPSK_MET_DATE                   = 32;
  public const int EPSK_EGG_LOCATION_DIAMOND_PEARL = 33;
  public const int EPSK_MET_LOCATION_DIAMOND_PEARL = 34;
  public const int EPSK_POKERUS                    = 35;
  public const int EPSK_POKE_BALL                  = 36;
  public const int EPSK_MET_LEVEL                  = 37;
  public const int EPSK_OT_GENDER                  = 38;
  public const int EPSK_ENCOUNTER_TYPE             = 39;
  public const int EPSK_POKE_BALL_HG_SS            = 40;
  public const int EPSK_PERFORMANCE                = 41;
  public const int EPSK_HEADER_UNKNOWN             = 42;
  public const int EPSK_SMALL_UNKNOWN              = 43;
  public const int EPSK_BIG_UNKNOWN                = 44;
  public const int EPSK_CONTEST_RIBBON             = 45;
  public const int EPSK_WORD_UNKNOWN               = 46;
  public const int EPSK_SHINY_LEAF                 = 47;
  public const int EPSK_LEAF_CROWN                 = 48;
  public const int EPSK_LEAF_UNUSED                = 49;

  // EVs; IVs
  public const int EPSN_HP                         = 0;
  public const int EPSN_ATTACK                     = 1;
  public const int EPSN_DEFENSE                    = 2;
  public const int EPSN_SPEED                      = 3;
  public const int EPSN_SPECIAL_ATTACK             = 4;
  public const int EPSN_SPECIAL_DEFENSE            = 5;
  // contest stats
  public const int EPSN_COOL                       = 0;
  public const int EPSN_BEAUTY                     = 1;
  public const int EPSN_CUTE                       = 2;
  public const int EPSN_SMART                      = 3;
  public const int EPSN_TOUGH                      = 4;
  public const int EPSN_FEEL                       = 5;
  // OT ID
  public const int EPSN_VISIBLE_ID                 = 0;
  public const int EPSN_SECRET_ID                  = 1;

   [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_read_save_from_file(string filename, out IntPtr save);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_destroy_save(IntPtr save);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_write_save_to_file(IntPtr save, string filename);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_current_operation_mode(IntPtr save);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_select_operation_mode(IntPtr save, int mode);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_current_save_slot(IntPtr save);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_select_save_slot(IntPtr save, int slot);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_copy_save_slot(IntPtr save, int from, int to);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_erase_save_slot(IntPtr save, int slot);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_is_save_slot_empty(IntPtr save, int slot);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_get_active_savefile(IntPtr save);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_swap_savefiles(IntPtr save);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_get_encryption_key(IntPtr save, out ulong key);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_set_encryption_key(IntPtr save, ulong key);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_read_savefile_raw(IntPtr save, uint offset, int width, out uint value);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_write_savefile_raw(IntPtr save, uint offset, int width, uint value);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_read_saveslot_raw(IntPtr save, uint offset, int width, out uint value);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_write_saveslot_raw(IntPtr save, uint offset, int width, uint value);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_new_pokemon(out IntPtr pokemon);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_destroy_pokemon(IntPtr pokemon);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_read_pokemon_from_file(string filename, out IntPtr pokemon);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_write_pokemon_to_file(IntPtr pokemon, string filename);
  
  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_read_pokemon_from_buffer([In] byte[] buffer, out IntPtr pokemon);
  
  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_write_pokemon_to_buffer(IntPtr pokemon, [Out] byte[] buffer);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_read_pokemon_from_save(IntPtr save, int box, int position, out IntPtr pokemon);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_write_pokemon_to_save(IntPtr save, int box, int position, IntPtr pokemon);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_erase_pokemon_from_save(IntPtr save, int box, int position);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_get_pokemon_name(IntPtr pokemon, int which, StringBuilder name);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_set_pokemon_name(IntPtr pokemon, int which, string name);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_get_pokemon_value(IntPtr pokemon, int kind, int index, out uint value);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_set_pokemon_value(IntPtr pokemon, int kind, int index, uint value);

  [DllImport("libeps.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
  public static extern int epsf_fix_pokemon_checksum(IntPtr pokemon);
}
