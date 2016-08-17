#include "proto.h"

void convert_buffer_to_text (const void * buffer, unsigned string_length, char * result) {
  const unsigned char * buf;
  unsigned short char_code;
  unsigned char table[] = CHARACTER_TABLE;
  unsigned char * ptr;
  for (buf = buffer; string_length; result ++, buf += 2, string_length --) {
    char_code = read_number(buf, 2);
    if ((!char_code) || (char_code == 0xffff)) break;
    if ((char_code < 0x0101) || (char_code > 0x01fe)) {
      *result = '^';
      continue;
    }
    ptr = strchr(table, char_code & 0xff);
    *result = ptr ? (0x20 + (ptr - table)) : '^';
  }
  *result = 0;
}

int convert_text_to_buffer (const char * text, unsigned length_limit, void * result) {
  unsigned char * buf;
  unsigned char converted;
  unsigned length;
  unsigned char table[] = CHARACTER_TABLE;
  for (buf = result, length = 0; *text; text ++, buf += 2, length ++) {
    if (length >= length_limit) return EPSS_STRING_TOO_LONG;
    if ((*text < 0x20) || (*text > 0x7e)) return EPSS_INVALID_CHARACTERS;
    converted = table[*text - 0x20];
    if (converted == 0xff) return EPSS_INVALID_CHARACTERS;
    write_number(buf, 2, 0x0100 | converted);
  }
  write_number(buf, 2, 0xffff);
  return EPSS_OK;
}
