#include "proto.h"

void convert_data_to_pokemon (unsigned char * data, Pokemon * pokemon) {
  pokemon -> personality_value = read_number(data, 4);
  pokemon -> header_unknown = read_number(data + 4, 2);
  pokemon -> checksum = read_number(data + 6, 2);
  unsigned char ordering = (pokemon -> personality_value >> 13) & 31;
  ordering = ordering[(unsigned char []) BLOCK_ORDERINGS];
  unsigned char p;
  for (p = 0; p < 4; p ++, ordering >>= 2) memcpy(pokemon -> data + ((ordering & 3) << 5), data + 8 + (p << 5), 0x20);
}

void convert_pokemon_to_data (Pokemon * pokemon, unsigned char * data) {
  write_number(data, 4, pokemon -> personality_value);
  write_number(data + 4, 2, pokemon -> header_unknown);
  write_number(data + 6, 2, pokemon -> checksum);
  unsigned char ordering = (pokemon -> personality_value >> 13) & 31;
  ordering = ordering[(unsigned char []) BLOCK_ORDERINGS];
  unsigned char p;
  for (p = 0; p < 4; p ++, ordering >>= 2) memcpy(data + 8 + (p << 5), pokemon -> data + ((ordering & 3) << 5), 0x20);
}

unsigned short calculate_pokemon_checksum (Pokemon * pokemon) {
  uint16_t checksum = 0;
  unsigned char p;
  for (p = 0; p < 0x40; p ++) checksum += read_number(pokemon -> data + (p << 1), 2);
  return checksum;
}

int access_pokemon_data_by_position (Pokemon * pokemon, unsigned position, unsigned width, unsigned bit_offset, unsigned bit_length, int write, unsigned * value) {
  unsigned char data[0x88];
  unsigned char * ptr;
  if (position < 8) {
    write_number(data, 4, pokemon -> personality_value);
    write_number(data + 4, 2, pokemon -> header_unknown);
    write_number(data + 6, 2, pokemon -> checksum);
    memcpy(data + 8, pokemon -> data, 0x80);
    ptr = data + position;
  } else
    ptr = pokemon -> data + (position - 8);
  unsigned current = read_number(ptr, width);
  if (!write) {
    if (!bit_offset && (bit_length == (width << 3)))  
      *value = current;
    else
      *value = (current >> bit_offset) & ((1 << bit_length) - 1);
    return EPSS_OK;
  }
  if (!bit_offset && (bit_length == (width << 3)))
    write_number(ptr, width, *value);
  else {
    current &= ~(((1 << bit_length) - 1) << bit_offset);
    current |= (*value & ((1 << bit_length) - 1)) << bit_offset;
    write_number(ptr, width, current);
  }
  if (position < 8) {
    pokemon -> personality_value = read_number(data, 4);
    pokemon -> header_unknown = read_number(data + 4, 2);
    pokemon -> checksum = read_number(data + 6, 2);
    memcpy(pokemon -> data, data + 8, 0x80);
  }
  return EPSS_OK;
}

int access_pokemon_data (Pokemon * pokemon, int kind, int index, int write, unsigned * value) {
  switch (kind) {
    case EPSK_BYTE: case EPSK_TWO_BYTES: case EPSK_THREE_BYTES: case EPSK_FOUR_BYTES:
      if ((index < 0) || ((index + kind) > 0x88)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, index, kind, 0, kind << 3, write, value);
    case EPSK_PERSONALITY_VALUE:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      if (write)
        pokemon -> personality_value = *value;
      else
        *value = pokemon -> personality_value;
      return EPSS_OK;
    case EPSK_CHECKSUM:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      if (write)
        pokemon -> checksum = *value;
      else
        *value = pokemon -> checksum;
      return EPSS_OK;
    case EPSK_SPECIES_NUMBER:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 8, 2, 0, 16, write, value);
    case EPSK_HELD_ITEM:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 10, 2, 0, 16, write, value);
    case EPSK_OT_ID:
      if ((index < 0) || (index > 1)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 12 + (index << 1), 2, 0, 16, write, value);
    case EPSK_EXPERIENCE_POINTS:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 16, 4, 0, 32, write, value);
    case EPSK_FRIENDSHIP:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 20, 1, 0, 8, write, value);
    case EPSK_ABILITY:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 21, 1, 0, 8, write, value);
    case EPSK_BOX_MARKS:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 22, 1, 0, 8, write, value);
    case EPSK_COUNTRY_OF_ORIGIN:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 23, 1, 0, 8, write, value);
    case EPSK_EFFORT_VALUE:
      if ((index < 0) || (index > 5)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 24 + index, 1, 0, 8, write, value);
    case EPSK_CONTEST_STAT:
      if ((index < 0) || (index > 5)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 30 + index, 1, 0, 8, write, value);
    case EPSK_RIBBON:
      if ((index < 0) || (index > 63)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, (index >> 4)[(unsigned []) {36, 38, 60, 62}], 4, index & 15, 1, write, value);
    case EPSK_MOVE:
      if ((index < 1) || (index > 4)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 38 + (index << 1), 2, 0, 16, write, value);
    case EPSK_MOVE_PP:
      if ((index < 1) || (index > 4)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 47 + index, 1, 0, 8, write, value);
    case EPSK_MOVE_PP_UPS:
      if ((index < 1) || (index > 4)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 51 + index, 1, 0, 8, write, value);
    case EPSK_INDIVIDUAL_VALUE:
      if ((index < 0) || (index > 5)) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 56, 4, 27 - 5 * index, 5, write, value);
    case EPSK_IS_EGG:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 56, 4, 0, 1, write, value);
    case EPSK_IS_NICKNAMED:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 56, 4, 1, 1, write, value);
    case EPSK_FATEFUL_ENCOUNTER:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 64, 1, 7, 1, write, value);
    case EPSK_GENDER:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 64, 1, 5, 2, write, value);
    case EPSK_FORM:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 64, 1, 0, 5, write, value);
    case EPSK_EGG_LOCATION_PLATINUM:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 68, 2, 0, 16, write, value);
    case EPSK_MET_LOCATION_PLATINUM:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 70, 2, 0, 16, write, value);
    case EPSK_HOMETOWN:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 94, 2, 0, 16, write, value);
    case EPSK_CONTEST_DATA:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 96, 4, 0, 32, write, value);
    case EPSK_EGG_RECEIVED_DATE:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 120, 3, 0, 24, write, value);
    case EPSK_MET_DATE:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 123, 3, 0, 24, write, value);
    case EPSK_EGG_LOCATION_DIAMOND_PEARL:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 126, 2, 0, 16, write, value);
    case EPSK_MET_LOCATION_DIAMOND_PEARL:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 128, 2, 0, 16, write, value);
    case EPSK_POKERUS:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 130, 1, 0, 8, write, value);
    case EPSK_POKE_BALL:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 131, 1, 0, 8, write, value);
    case EPSK_MET_LEVEL:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 132, 1, 1, 7, write, value);
    case EPSK_OT_GENDER:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 132, 1, 0, 1, write, value);
    case EPSK_ENCOUNTER_TYPE:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 133, 1, 0, 8, write, value);
    case EPSK_POKE_BALL_HG_SS:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 134, 1, 0, 8, write, value);
    case EPSK_PERFORMANCE:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 135, 1, 0, 8, write, value);
    case EPSK_HEADER_UNKNOWN:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      if (write)
        pokemon -> header_unknown = *value;
      else
        *value = pokemon -> header_unknown;
      return EPSS_OK;
    case EPSK_SMALL_UNKNOWN:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 65, 3, 0, 24, write, value);
    case EPSK_BIG_UNKNOWN:
      if (index) return EPSS_INDEX_OUT_OF_RANGE;
      return access_pokemon_data_by_position(pokemon, 100, 4, 0, 32, write, value);
    default:
      return EPSS_INVALID_ARGUMENT;
  }
}
