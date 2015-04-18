#ifndef BSTREE_H
#define BSTREE_H

typedef struct node_t node_t;

struct node_t
{
  const node_t* left;
  const node_t* right;
  char          token;
};

static const node_t _00AC9490 = { NULL, NULL, 0 }; /* "literal" 0 119952 */
static const node_t _00AC7AC8 = { NULL, NULL, 1 }; /* "=" 100 31470 */
static const node_t _00ACC780 = { NULL, NULL, 2 }; /* "]" 10100 8538 */
static const node_t _00AC0660 = { NULL, NULL, 3 }; /* "," 101010 4599 */
static const node_t _00ACC7D0 = { NULL, NULL, 4 }; /* "then" 1010110 2635 */
static const node_t _00AC49E0 = { NULL, NULL, 5 }; /* "and" 101011100 644 */
static const node_t _00AC93A0 = { NULL, NULL, 6 }; /* ">=" 1010111010 319 */
static const node_t _00ACC5C8 = { NULL, NULL, 7 }; /* ">" 1010111011 326 */
static const node_t _00AC63F8 = { &_00AC93A0, &_00ACC5C8, -1 };
static const node_t _00ABFD28 = { &_00AC49E0, &_00AC63F8, -1 };
static const node_t _00AC94B8 = { NULL, NULL, 8 }; /* "elseif" 101011110 667 */
static const node_t _00ACC618 = { NULL, NULL, 9 }; /* "else" 1010111110 381 */
static const node_t _00AC7BE0 = { NULL, NULL, 10 }; /* "%" 10101111110 198 */
static const node_t _00AC7A78 = { NULL, NULL, 11 }; /* "nil" 10101111111 201 */
static const node_t _00AC6330 = { &_00AC7BE0, &_00AC7A78, -1 };
static const node_t _00AC6420 = { &_00ACC618, &_00AC6330, -1 };
static const node_t _00ABFD50 = { &_00AC94B8, &_00AC6420, -1 };
static const node_t _00ABFE18 = { &_00ABFD28, &_00ABFD50, -1 };
static const node_t _00ABFEB8 = { &_00ACC7D0, &_00ABFE18, -1 };
static const node_t _00AC0340 = { &_00AC0660, &_00ABFEB8, -1 };
static const node_t _00AC0408 = { &_00ACC780, &_00AC0340, -1 };
static const node_t _00AC93F0 = { NULL, NULL, 12 }; /* "self" 1011 24196 */
static const node_t _00AC0480 = { &_00AC0408, &_00AC93F0, -1 };
static const node_t _00AC0CF0 = { &_00AC7AC8, &_00AC0480, -1 };
static const node_t _00ACA228 = { NULL, NULL, 13 }; /* ")" 11000 12916 */
static const node_t _00AC05C0 = { NULL, NULL, 14 }; /* "(" 11001 12916 */
static const node_t _00AC0430 = { &_00ACA228, &_00AC05C0, -1 };
static const node_t _00AC06D8 = { NULL, NULL, 15 }; /* "true" 1101000 2799 */
static const node_t _00AC94E0 = { NULL, NULL, 16 }; /* "false" 1101001 3512 */
static const node_t _00ABFEE0 = { &_00AC06D8, &_00AC94E0, -1 };
static const node_t _00ACC500 = { NULL, NULL, 17 }; /* "{" 1101010000 428 */
static const node_t _00ACC550 = { NULL, NULL, 18 }; /* "}" 1101010001 428 */
static const node_t _00AC6448 = { &_00ACC500, &_00ACC550, -1 };
static const node_t _00AC7BB8 = { NULL, NULL, 19 }; /* "do" 110101001 870 */
static const node_t _00ABFD78 = { &_00AC6448, &_00AC7BB8, -1 };
static const node_t _00AC05E8 = { NULL, NULL, 20 }; /* "+" 110101010 900 */
static const node_t _00AC4648 = { NULL, NULL, 21 }; /* "~=" 110101011000 99 */
static const node_t _00ACC758 = { NULL, NULL, 22 }; /* "#" 11010101100100 27 */
static const node_t _00AC0688 = { NULL, NULL, 23 }; /* "/" 110101011001010 13 */
static const node_t _00ACC7F8 = { NULL, NULL, 24 }; /* "^" 110101011001011 14 */
static const node_t _00AC5D90 = { &_00AC0688, &_00ACC7F8, -1 };
static const node_t _00AC5DE0 = { &_00ACC758, &_00AC5D90, -1 };
static const node_t _00AC0700 = { NULL, NULL, 25 }; /* "return" 1101010110011 55 */
static const node_t _00AC5E80 = { &_00AC5DE0, &_00AC0700, -1 };
static const node_t _00AC62B8 = { &_00AC4648, &_00AC5E80, -1 };
static const node_t _00AC93C8 = { NULL, NULL, 26 }; /* "or" 11010101101 222 */
static const node_t _00AC6358 = { &_00AC62B8, &_00AC93C8, -1 };
static const node_t _00AC9440 = { NULL, NULL, 27 }; /* "not" 11010101110 244 */
static const node_t _00AC0598 = { NULL, NULL, 28 }; /* "//" 110101011110 121 */
static const node_t _00AC9468 = { NULL, NULL, 29 }; /* "in" 11010101111100 34 */
static const node_t _00AC0610 = { NULL, NULL, 30 }; /* "*" 11010101111101 35 */
static const node_t _00AC5E08 = { &_00AC9468, &_00AC0610, -1 };
static const node_t _00ACC708 = { NULL, NULL, 31 }; /* "until" 1101010111111000 7 */
static const node_t _00ACC078 = { NULL, NULL, 32 }; /* "~" 110101011111100100000 0 */
static const node_t _00ACC0C8 = { NULL, NULL, 33 }; /* ">>" 110101011111100100001 0 */
static const node_t _00ACC848 = { &_00ACC078, &_00ACC0C8, -1 };
static const node_t _00ACC4D8 = { NULL, NULL, 34 }; /* "<<" 110101011111100100010 0 */
static const node_t _00ACC5A0 = { NULL, NULL, 35 }; /* "|" 110101011111100100011 0 */
static const node_t _00ACC870 = { &_00ACC4D8, &_00ACC5A0, -1 };
static const node_t _00ACC8E8 = { &_00ACC848, &_00ACC870, -1 };
static const node_t _00ACC5F0 = { NULL, NULL, 36 }; /* "::" 110101011111100100100 0 */
static const node_t _00AC7A50 = { NULL, NULL, 37 }; /* "eof" 110101011111100100101 0 */
static const node_t _00ACC898 = { &_00ACC5F0, &_00AC7A50, -1 };
static const node_t _00ACC7A8 = { NULL, NULL, 38 }; /* "goto" 110101011111100100110 0 */
static const node_t _00ACBC40 = { NULL, NULL, 39 }; /* "break" 1101010111111001001110 0 */
static const node_t _00ACBC68 = { NULL, NULL, 40 }; /* ";" 1101010111111001001111 0 */
static const node_t _00ACC820 = { &_00ACBC40, &_00ACBC68, -1 };
static const node_t _00ACC8C0 = { &_00ACC7A8, &_00ACC820, -1 };
static const node_t _00ACC910 = { &_00ACC898, &_00ACC8C0, -1 };
static const node_t _00AC5CF0 = { &_00ACC8E8, &_00ACC910, -1 };
static const node_t _00AC7B68 = { NULL, NULL, 41 }; /* "..." 110101011111100101 4 */
static const node_t _00AC5D18 = { &_00AC5CF0, &_00AC7B68, -1 };
static const node_t _00AC7AF0 = { NULL, NULL, 42 }; /* "repeat" 11010101111110011 7 */
static const node_t _00AC5D40 = { &_00AC5D18, &_00AC7AF0, -1 };
static const node_t _00AC5D68 = { &_00ACC708, &_00AC5D40, -1 };
static const node_t _00AC7B40 = { NULL, NULL, 43 }; /* "&" 110101011111101 20 */
static const node_t _00AC5DB8 = { &_00AC5D68, &_00AC7B40, -1 };
static const node_t _00AC7AA0 = { NULL, NULL, 44 }; /* ".." 11010101111111 39 */
static const node_t _00AC5E58 = { &_00AC5DB8, &_00AC7AA0, -1 };
static const node_t _00AC5EA8 = { &_00AC5E08, &_00AC5E58, -1 };
static const node_t _00AC62E0 = { &_00AC0598, &_00AC5EA8, -1 };
static const node_t _00AC6380 = { &_00AC9440, &_00AC62E0, -1 };
static const node_t _00AC6470 = { &_00AC6358, &_00AC6380, -1 };
static const node_t _00ABFDA0 = { &_00AC05E8, &_00AC6470, -1 };
static const node_t _00ABFE40 = { &_00ABFD78, &_00ABFDA0, -1 };
static const node_t _00AC7B90 = { NULL, NULL, 45 }; /* "end" 1101011 3844 */
static const node_t _00AC02F0 = { &_00ABFE40, &_00AC7B90, -1 };
static const node_t _00AC0368 = { &_00ABFEE0, &_00AC02F0, -1 };
static const node_t _00AC9508 = { NULL, NULL, 46 }; /* "if" 11011000 1968 */
static const node_t _00AC4670 = { NULL, NULL, 47 }; /* "==" 11011001 2003 */
static const node_t _00ABFE68 = { &_00AC9508, &_00AC4670, -1 };
static const node_t _00AC7B18 = { NULL, NULL, 48 }; /* "function" 110110100 1006 */
static const node_t _00ACC528 = { NULL, NULL, 49 }; /* "local" 110110101 1096 */
static const node_t _00ABFDC8 = { &_00AC7B18, &_00ACC528, -1 };
static const node_t _00AC4698 = { NULL, NULL, 50 }; /* "<=" 1101101100 541 */
static const node_t _00ACA250 = { NULL, NULL, 51 }; /* "while" 1101101101 557 */
static const node_t _00AC6498 = { &_00AC4698, &_00ACA250, -1 };
static const node_t _00AC7A28 = { NULL, NULL, 52 }; /* ":" 11011011100 288 */
static const node_t _00ACC578 = { NULL, NULL, 53 }; /* "<" 11011011101 290 */
static const node_t _00AC63A8 = { &_00AC7A28, &_00ACC578, -1 };
static const node_t _00AC0638 = { NULL, NULL, 54 }; /* "-" 11011011110 304 */
static const node_t _00AC4A08 = { NULL, NULL, 55 }; /* "for" 11011011111 313 */
static const node_t _00AC63D0 = { &_00AC0638, &_00AC4A08, -1 };
static const node_t _00AC64C0 = { &_00AC63A8, &_00AC63D0, -1 };
static const node_t _00ABFDF0 = { &_00AC6498, &_00AC64C0, -1 };
static const node_t _00ABFE90 = { &_00ABFDC8, &_00ABFDF0, -1 };
static const node_t _00AC0318 = { &_00ABFE68, &_00ABFE90, -1 };
static const node_t _00ACC730 = { NULL, NULL, 56 }; /* "[" 110111 8538 */
static const node_t _00AC0390 = { &_00AC0318, &_00ACC730, -1 };
static const node_t _00AC0458 = { &_00AC0368, &_00AC0390, -1 };
static const node_t _00AC08B8 = { &_00AC0430, &_00AC0458, -1 };
static const node_t _00AC06B0 = { NULL, NULL, 57 }; /* "." 111 62752 */
static const node_t _00AC0D18 = { &_00AC08B8, &_00AC06B0, -1 };
static const node_t _00AC0D40 = { &_00AC0CF0, &_00AC0D18, -1 };
static const node_t _00AC0228 = { &_00AC9490, &_00AC0D40, -1 };

static struct{ const char* literal; size_t len; } tokens[] =
{
  { NULL, 0 },
  { "=", 1 },
  { "]", 1 },
  { ",", 1 },
  { "then", 4 },
  { "and", 3 },
  { ">=", 2 },
  { ">", 1 },
  { "elseif", 6 },
  { "else", 4 },
  { "%", 1 },
  { "nil", 3 },
  { "self", 4 },
  { ")", 1 },
  { "(", 1 },
  { "true", 4 },
  { "false", 5 },
  { "{", 1 },
  { "}", 1 },
  { "do", 2 },
  { "+", 1 },
  { "~=", 2 },
  { "#", 1 },
  { "/", 1 },
  { "^", 1 },
  { "return", 6 },
  { "or", 2 },
  { "not", 3 },
  { "//", 2 },
  { "in", 2 },
  { "*", 1 },
  { "until", 5 },
  { "~", 1 },
  { ">>", 2 },
  { "<<", 2 },
  { "|", 1 },
  { "::", 2 },
  { NULL, 0 },
  { "goto", 4 },
  { "break", 5 },
  { ";", 1 },
  { "...", 3 },
  { "repeat", 6 },
  { "&", 1 },
  { "..", 2 },
  { "end", 3 },
  { "if", 2 },
  { "==", 2 },
  { "function", 8 },
  { "local", 5 },
  { "<=", 2 },
  { "while", 5 },
  { ":", 1 },
  { "<", 1 },
  { "-", 1 },
  { "for", 3 },
  { "[", 1 },
  { ".", 1 },
};

#define BS_ROOT    ( &_00AC0228 )
#define BS_LITERAL 0
#define BS_EOF     37

#endif /* BSTREE_H */
