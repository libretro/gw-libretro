/* C code produced by gperf version 3.0.1 */
/* Command-line: 'C:\\MinGW\\bin\\gperf.exe' -c -C -l -L C -t -7 -m 100 -I --output-file=gwlua/entries.c gwlua/entries.gperf  */
/* Computed positions: -k'1-2,10-11' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "gwlua/entries.gperf"
struct binary_t { const char* name; const unsigned char* data; size_t size; };
//;
#include <string.h>

#define TOTAL_KEYWORDS 86
#define MIN_WORD_LENGTH 8
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 9
#define MAX_HASH_VALUE 118
/* maximum key range = 110, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119,   5, 119, 119, 119,
        3,   2,   1,   0,   8,   4,  65,  61,  57,  53,
       49,  45,  41,  37, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119, 119, 119, 119,   9,  10,   4,
        1,   0,   0,  12,  32,  27,  22,  17,   0,   2,
        0,   0,  65, 119,   0,   3,   3,  67, 119,   0,
        3,   2, 119,   0, 119, 119, 119, 119, 119, 119,
      119, 119, 119, 119
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[10]+6];
      /*FALLTHROUGH*/
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
const struct binary_t *
in_word_set (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  8,  8, 12,  9,
      12, 12, 12, 12, 12,  8, 13,  0,  0,  0, 12, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
      15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 11,  8,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0, 11, 11
    };
  static const struct binary_t wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 80 "gwlua/entries.gperf"
      {"forms.lua", gwlua_lua_forms_lua, sizeof( gwlua_lua_forms_lua )},
#line 78 "gwlua/entries.gperf"
      {"fmod.lua", gwlua_lua_fmod_lua, sizeof( gwlua_lua_fmod_lua )},
#line 86 "gwlua/entries.gperf"
      {"snes.rle", gwlua_png_snes_rle, sizeof( gwlua_png_snes_rle )},
#line 85 "gwlua/entries.gperf"
      {"registry.lua", gwlua_lua_registry_lua, sizeof( gwlua_lua_registry_lua )},
#line 73 "gwlua/entries.gperf"
      {"class.lua", gwlua_lua_class_lua, sizeof( gwlua_lua_class_lua )},
#line 84 "gwlua/entries.gperf"
      {"messages.lua", gwlua_lua_messages_lua, sizeof( gwlua_lua_messages_lua )},
#line 77 "gwlua/entries.gperf"
      {"extctrls.lua", gwlua_lua_extctrls_lua, sizeof( gwlua_lua_extctrls_lua )},
#line 75 "gwlua/entries.gperf"
      {"controls.lua", gwlua_lua_controls_lua, sizeof( gwlua_lua_controls_lua )},
#line 88 "gwlua/entries.gperf"
      {"sysutils.lua", gwlua_lua_sysutils_lua, sizeof( gwlua_lua_sysutils_lua )},
#line 87 "gwlua/entries.gperf"
      {"stdctrls.lua", gwlua_lua_stdctrls_lua, sizeof( gwlua_lua_stdctrls_lua )},
#line 83 "gwlua/entries.gperf"
      {"math.lua", gwlua_lua_math_lua, sizeof( gwlua_lua_math_lua )},
#line 79 "gwlua/entries.gperf"
      {"fmodtypes.lua", gwlua_lua_fmodtypes_lua, sizeof( gwlua_lua_fmodtypes_lua )},
      {""}, {""}, {""},
#line 81 "gwlua/entries.gperf"
      {"graphics.lua", gwlua_lua_graphics_lua, sizeof( gwlua_lua_graphics_lua )},
#line 67 "gwlua/entries.gperf"
      {"boxybold_5f.rle", gwlua_png_boxybold_5f_rle, sizeof( gwlua_png_boxybold_5f_rle )},
#line 51 "gwlua/entries.gperf"
      {"boxybold_4f.rle", gwlua_png_boxybold_4f_rle, sizeof( gwlua_png_boxybold_4f_rle )},
#line 35 "gwlua/entries.gperf"
      {"boxybold_3f.rle", gwlua_png_boxybold_3f_rle, sizeof( gwlua_png_boxybold_3f_rle )},
#line 19 "gwlua/entries.gperf"
      {"boxybold_2f.rle", gwlua_png_boxybold_2f_rle, sizeof( gwlua_png_boxybold_2f_rle )},
#line 53 "gwlua/entries.gperf"
      {"boxybold_51.rle", gwlua_png_boxybold_51_rle, sizeof( gwlua_png_boxybold_51_rle )},
#line 37 "gwlua/entries.gperf"
      {"boxybold_41.rle", gwlua_png_boxybold_41_rle, sizeof( gwlua_png_boxybold_41_rle )},
#line 21 "gwlua/entries.gperf"
      {"boxybold_31.rle", gwlua_png_boxybold_31_rle, sizeof( gwlua_png_boxybold_31_rle )},
#line 5 "gwlua/entries.gperf"
      {"boxybold_21.rle", gwlua_png_boxybold_21_rle, sizeof( gwlua_png_boxybold_21_rle )},
#line 52 "gwlua/entries.gperf"
      {"boxybold_50.rle", gwlua_png_boxybold_50_rle, sizeof( gwlua_png_boxybold_50_rle )},
#line 36 "gwlua/entries.gperf"
      {"boxybold_40.rle", gwlua_png_boxybold_40_rle, sizeof( gwlua_png_boxybold_40_rle )},
#line 20 "gwlua/entries.gperf"
      {"boxybold_30.rle", gwlua_png_boxybold_30_rle, sizeof( gwlua_png_boxybold_30_rle )},
#line 4 "gwlua/entries.gperf"
      {"boxybold_20.rle", gwlua_png_boxybold_20_rle, sizeof( gwlua_png_boxybold_20_rle )},
#line 62 "gwlua/entries.gperf"
      {"boxybold_5a.rle", gwlua_png_boxybold_5a_rle, sizeof( gwlua_png_boxybold_5a_rle )},
#line 46 "gwlua/entries.gperf"
      {"boxybold_4a.rle", gwlua_png_boxybold_4a_rle, sizeof( gwlua_png_boxybold_4a_rle )},
#line 30 "gwlua/entries.gperf"
      {"boxybold_3a.rle", gwlua_png_boxybold_3a_rle, sizeof( gwlua_png_boxybold_3a_rle )},
#line 14 "gwlua/entries.gperf"
      {"boxybold_2a.rle", gwlua_png_boxybold_2a_rle, sizeof( gwlua_png_boxybold_2a_rle )},
#line 68 "gwlua/entries.gperf"
      {"boxybold_60.rle", gwlua_png_boxybold_60_rle, sizeof( gwlua_png_boxybold_60_rle )},
#line 66 "gwlua/entries.gperf"
      {"boxybold_5e.rle", gwlua_png_boxybold_5e_rle, sizeof( gwlua_png_boxybold_5e_rle )},
#line 50 "gwlua/entries.gperf"
      {"boxybold_4e.rle", gwlua_png_boxybold_4e_rle, sizeof( gwlua_png_boxybold_4e_rle )},
#line 34 "gwlua/entries.gperf"
      {"boxybold_3e.rle", gwlua_png_boxybold_3e_rle, sizeof( gwlua_png_boxybold_3e_rle )},
#line 18 "gwlua/entries.gperf"
      {"boxybold_2e.rle", gwlua_png_boxybold_2e_rle, sizeof( gwlua_png_boxybold_2e_rle )},
#line 72 "gwlua/entries.gperf"
      {"boxybold_7e.rle", gwlua_png_boxybold_7e_rle, sizeof( gwlua_png_boxybold_7e_rle )},
#line 65 "gwlua/entries.gperf"
      {"boxybold_5d.rle", gwlua_png_boxybold_5d_rle, sizeof( gwlua_png_boxybold_5d_rle )},
#line 49 "gwlua/entries.gperf"
      {"boxybold_4d.rle", gwlua_png_boxybold_4d_rle, sizeof( gwlua_png_boxybold_4d_rle )},
#line 33 "gwlua/entries.gperf"
      {"boxybold_3d.rle", gwlua_png_boxybold_3d_rle, sizeof( gwlua_png_boxybold_3d_rle )},
#line 17 "gwlua/entries.gperf"
      {"boxybold_2d.rle", gwlua_png_boxybold_2d_rle, sizeof( gwlua_png_boxybold_2d_rle )},
#line 71 "gwlua/entries.gperf"
      {"boxybold_7d.rle", gwlua_png_boxybold_7d_rle, sizeof( gwlua_png_boxybold_7d_rle )},
#line 64 "gwlua/entries.gperf"
      {"boxybold_5c.rle", gwlua_png_boxybold_5c_rle, sizeof( gwlua_png_boxybold_5c_rle )},
#line 48 "gwlua/entries.gperf"
      {"boxybold_4c.rle", gwlua_png_boxybold_4c_rle, sizeof( gwlua_png_boxybold_4c_rle )},
#line 32 "gwlua/entries.gperf"
      {"boxybold_3c.rle", gwlua_png_boxybold_3c_rle, sizeof( gwlua_png_boxybold_3c_rle )},
#line 16 "gwlua/entries.gperf"
      {"boxybold_2c.rle", gwlua_png_boxybold_2c_rle, sizeof( gwlua_png_boxybold_2c_rle )},
#line 70 "gwlua/entries.gperf"
      {"boxybold_7c.rle", gwlua_png_boxybold_7c_rle, sizeof( gwlua_png_boxybold_7c_rle )},
#line 63 "gwlua/entries.gperf"
      {"boxybold_5b.rle", gwlua_png_boxybold_5b_rle, sizeof( gwlua_png_boxybold_5b_rle )},
#line 47 "gwlua/entries.gperf"
      {"boxybold_4b.rle", gwlua_png_boxybold_4b_rle, sizeof( gwlua_png_boxybold_4b_rle )},
#line 31 "gwlua/entries.gperf"
      {"boxybold_3b.rle", gwlua_png_boxybold_3b_rle, sizeof( gwlua_png_boxybold_3b_rle )},
#line 15 "gwlua/entries.gperf"
      {"boxybold_2b.rle", gwlua_png_boxybold_2b_rle, sizeof( gwlua_png_boxybold_2b_rle )},
#line 69 "gwlua/entries.gperf"
      {"boxybold_7b.rle", gwlua_png_boxybold_7b_rle, sizeof( gwlua_png_boxybold_7b_rle )},
#line 61 "gwlua/entries.gperf"
      {"boxybold_59.rle", gwlua_png_boxybold_59_rle, sizeof( gwlua_png_boxybold_59_rle )},
#line 45 "gwlua/entries.gperf"
      {"boxybold_49.rle", gwlua_png_boxybold_49_rle, sizeof( gwlua_png_boxybold_49_rle )},
#line 29 "gwlua/entries.gperf"
      {"boxybold_39.rle", gwlua_png_boxybold_39_rle, sizeof( gwlua_png_boxybold_39_rle )},
#line 13 "gwlua/entries.gperf"
      {"boxybold_29.rle", gwlua_png_boxybold_29_rle, sizeof( gwlua_png_boxybold_29_rle )},
#line 60 "gwlua/entries.gperf"
      {"boxybold_58.rle", gwlua_png_boxybold_58_rle, sizeof( gwlua_png_boxybold_58_rle )},
#line 44 "gwlua/entries.gperf"
      {"boxybold_48.rle", gwlua_png_boxybold_48_rle, sizeof( gwlua_png_boxybold_48_rle )},
#line 28 "gwlua/entries.gperf"
      {"boxybold_38.rle", gwlua_png_boxybold_38_rle, sizeof( gwlua_png_boxybold_38_rle )},
#line 12 "gwlua/entries.gperf"
      {"boxybold_28.rle", gwlua_png_boxybold_28_rle, sizeof( gwlua_png_boxybold_28_rle )},
#line 59 "gwlua/entries.gperf"
      {"boxybold_57.rle", gwlua_png_boxybold_57_rle, sizeof( gwlua_png_boxybold_57_rle )},
#line 43 "gwlua/entries.gperf"
      {"boxybold_47.rle", gwlua_png_boxybold_47_rle, sizeof( gwlua_png_boxybold_47_rle )},
#line 27 "gwlua/entries.gperf"
      {"boxybold_37.rle", gwlua_png_boxybold_37_rle, sizeof( gwlua_png_boxybold_37_rle )},
#line 11 "gwlua/entries.gperf"
      {"boxybold_27.rle", gwlua_png_boxybold_27_rle, sizeof( gwlua_png_boxybold_27_rle )},
#line 58 "gwlua/entries.gperf"
      {"boxybold_56.rle", gwlua_png_boxybold_56_rle, sizeof( gwlua_png_boxybold_56_rle )},
#line 42 "gwlua/entries.gperf"
      {"boxybold_46.rle", gwlua_png_boxybold_46_rle, sizeof( gwlua_png_boxybold_46_rle )},
#line 26 "gwlua/entries.gperf"
      {"boxybold_36.rle", gwlua_png_boxybold_36_rle, sizeof( gwlua_png_boxybold_36_rle )},
#line 10 "gwlua/entries.gperf"
      {"boxybold_26.rle", gwlua_png_boxybold_26_rle, sizeof( gwlua_png_boxybold_26_rle )},
#line 57 "gwlua/entries.gperf"
      {"boxybold_55.rle", gwlua_png_boxybold_55_rle, sizeof( gwlua_png_boxybold_55_rle )},
#line 41 "gwlua/entries.gperf"
      {"boxybold_45.rle", gwlua_png_boxybold_45_rle, sizeof( gwlua_png_boxybold_45_rle )},
#line 25 "gwlua/entries.gperf"
      {"boxybold_35.rle", gwlua_png_boxybold_35_rle, sizeof( gwlua_png_boxybold_35_rle )},
#line 9 "gwlua/entries.gperf"
      {"boxybold_25.rle", gwlua_png_boxybold_25_rle, sizeof( gwlua_png_boxybold_25_rle )},
#line 56 "gwlua/entries.gperf"
      {"boxybold_54.rle", gwlua_png_boxybold_54_rle, sizeof( gwlua_png_boxybold_54_rle )},
#line 40 "gwlua/entries.gperf"
      {"boxybold_44.rle", gwlua_png_boxybold_44_rle, sizeof( gwlua_png_boxybold_44_rle )},
#line 24 "gwlua/entries.gperf"
      {"boxybold_34.rle", gwlua_png_boxybold_34_rle, sizeof( gwlua_png_boxybold_34_rle )},
#line 8 "gwlua/entries.gperf"
      {"boxybold_24.rle", gwlua_png_boxybold_24_rle, sizeof( gwlua_png_boxybold_24_rle )},
#line 55 "gwlua/entries.gperf"
      {"boxybold_53.rle", gwlua_png_boxybold_53_rle, sizeof( gwlua_png_boxybold_53_rle )},
#line 39 "gwlua/entries.gperf"
      {"boxybold_43.rle", gwlua_png_boxybold_43_rle, sizeof( gwlua_png_boxybold_43_rle )},
#line 23 "gwlua/entries.gperf"
      {"boxybold_33.rle", gwlua_png_boxybold_33_rle, sizeof( gwlua_png_boxybold_33_rle )},
#line 7 "gwlua/entries.gperf"
      {"boxybold_23.rle", gwlua_png_boxybold_23_rle, sizeof( gwlua_png_boxybold_23_rle )},
#line 54 "gwlua/entries.gperf"
      {"boxybold_52.rle", gwlua_png_boxybold_52_rle, sizeof( gwlua_png_boxybold_52_rle )},
#line 38 "gwlua/entries.gperf"
      {"boxybold_42.rle", gwlua_png_boxybold_42_rle, sizeof( gwlua_png_boxybold_42_rle )},
#line 22 "gwlua/entries.gperf"
      {"boxybold_32.rle", gwlua_png_boxybold_32_rle, sizeof( gwlua_png_boxybold_32_rle )},
#line 6 "gwlua/entries.gperf"
      {"boxybold_22.rle", gwlua_png_boxybold_22_rle, sizeof( gwlua_png_boxybold_22_rle )},
#line 74 "gwlua/entries.gperf"
      {"classes.lua", gwlua_lua_classes_lua, sizeof( gwlua_lua_classes_lua )},
#line 82 "gwlua/entries.gperf"
      {"jpeg.lua", gwlua_lua_jpeg_lua, sizeof( gwlua_lua_jpeg_lua )},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 89 "gwlua/entries.gperf"
      {"windows.lua", gwlua_lua_windows_lua, sizeof( gwlua_lua_windows_lua )},
#line 76 "gwlua/entries.gperf"
      {"dialogs.lua", gwlua_lua_dialogs_lua, sizeof( gwlua_lua_dialogs_lua )}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
