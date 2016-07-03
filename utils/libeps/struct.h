typedef unsigned char SaveSubslot[0x6ff00];

typedef struct {
  unsigned char config[0x100];
  unsigned char header[0x280];
  SaveSubslot subslots[4];
  unsigned char checksum[0x40];
  unsigned char footer[0x40];
} SaveSlot;

typedef struct {
  SaveSlot slots[2];
  unsigned current_slot : 1;
  unsigned swap_saveslots : 1;
  unsigned current_subslot : 3; // 0: none, 1-4: slot selected
  unsigned current_mode : 2; // 0: current, 1: backup, 2: both/read current, 3: both/read backup
} SaveFile;

typedef struct {
  unsigned personality_value;
  unsigned short header_unknown;
  unsigned short checksum;
  unsigned char data[0x80];
} Pokemon;

#define CHARACTER_TABLE { /* All ASCII characters map to the 0x01xx range. This table omits the 0x01. */       \
  /* 20 */ 0xde, 0xab, 0xb5, 0xc0, 0xa8, 0xd2, 0xc2, 0xb3, 0xb9, 0xba, 0xbf, 0xbd, 0xad, 0xbe, 0xae, 0xb1,     \
  /* 30 */ 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0xc4, 0xc5, 0xbb, 0xc1, 0xbc, 0xac,     \
  /* 40 */ 0xd0, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,     \
  /* 50 */ 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0xff, 0xff, 0xff, 0xff, 0xe9,     \
  /* 60 */ 0xff, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,     \
  /* 70 */ 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0xb7, 0xff, 0xb8, 0xc3, 0x00}
         /*  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F  */

#define BLOCK_ORDER(a, b, c, d) ((a) | ((b) << 2) | ((c) << 4) | ((d) << 6))

#define BLOCK_ORDERINGS {                                                                                 \
  BLOCK_ORDER(0, 1, 2, 3), BLOCK_ORDER(0, 1, 3, 2), BLOCK_ORDER(0, 2, 1, 3), BLOCK_ORDER(0, 2, 3, 1),     \
  BLOCK_ORDER(0, 3, 1, 2), BLOCK_ORDER(0, 3, 2, 1), BLOCK_ORDER(1, 0, 2, 3), BLOCK_ORDER(1, 0, 3, 2),     \
  BLOCK_ORDER(1, 2, 0, 3), BLOCK_ORDER(1, 2, 3, 0), BLOCK_ORDER(1, 3, 0, 2), BLOCK_ORDER(1, 3, 2, 0),     \
  BLOCK_ORDER(2, 0, 1, 3), BLOCK_ORDER(2, 0, 3, 1), BLOCK_ORDER(2, 1, 0, 3), BLOCK_ORDER(2, 1, 3, 0),     \
  BLOCK_ORDER(2, 3, 0, 1), BLOCK_ORDER(2, 3, 1, 0), BLOCK_ORDER(3, 0, 1, 2), BLOCK_ORDER(3, 0, 2, 1),     \
  BLOCK_ORDER(3, 1, 0, 2), BLOCK_ORDER(3, 1, 2, 0), BLOCK_ORDER(3, 2, 0, 1), BLOCK_ORDER(3, 2, 1, 0),     \
  BLOCK_ORDER(0, 1, 2, 3), BLOCK_ORDER(0, 1, 3, 2), BLOCK_ORDER(0, 2, 1, 3), BLOCK_ORDER(0, 2, 3, 1),     \
  BLOCK_ORDER(0, 3, 1, 2), BLOCK_ORDER(0, 3, 2, 1), BLOCK_ORDER(1, 0, 2, 3), BLOCK_ORDER(1, 0, 3, 2)      \
}
