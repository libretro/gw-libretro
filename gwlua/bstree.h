#ifndef BSTREE_H
#define BSTREE_H

typedef struct node_t node_t;

struct node_t
{
  const node_t* left;
  const node_t* right;
  char          token;
};

static const node_t _00d1ce98 = { NULL, NULL, 0 }; /* "literal" 0 114491 */
static const node_t _00d26e98 = { NULL, NULL, 1 }; /* "=" 100 29375 */
static const node_t _00d1d078 = { NULL, NULL, 2 }; /* "end" 101000 3769 */
static const node_t _00d1d780 = { NULL, NULL, 3 }; /* "if" 1010010 1916 */
static const node_t _00d1cf60 = { NULL, NULL, 4 }; /* "==" 1010011 1987 */
static const node_t _00d23270 = { &_00d1d780, &_00d1cf60, -1 };
static const node_t _00d23338 = { &_00d1d078, &_00d23270, -1 };
static const node_t _00d1d730 = { NULL, NULL, 5 }; /* "function" 10101000 1025 */
static const node_t _00d1db40 = { NULL, NULL, 6 }; /* "not" 1010100100 225 */
static const node_t _00d26f10 = { NULL, NULL, 7 }; /* "for" 1010100101 273 */
static const node_t _00d1d8c0 = { &_00d1db40, &_00d26f10, -1 };
static const node_t _00d1d5a0 = { NULL, NULL, 8 }; /* "<=" 101010011 541 */
static const node_t _00d1b9d0 = { &_00d1d8c0, &_00d1d5a0, -1 };
static const node_t _00d231d0 = { &_00d1d730, &_00d1b9d0, -1 };
static const node_t _00d1cf88 = { NULL, NULL, 9 }; /* "local" 10101010 1045 */
static const node_t _00d1d6e0 = { NULL, NULL, 10 }; /* "while" 101010110 555 */
static const node_t _00d1d848 = { NULL, NULL, 11 }; /* "//" 10101011100 121 */
static const node_t _00d1d708 = { NULL, NULL, 12 }; /* "or" 10101011101 155 */
static const node_t _00d186e0 = { &_00d1d848, &_00d1d708, -1 };
static const node_t _00d26ec0 = { NULL, NULL, 13 }; /* "<" 1010101111 286 */
static const node_t _00d1b908 = { &_00d186e0, &_00d26ec0, -1 };
static const node_t _00d1b9f8 = { &_00d1d6e0, &_00d1b908, -1 };
static const node_t _00d231f8 = { &_00d1cf88, &_00d1b9f8, -1 };
static const node_t _00d23298 = { &_00d231d0, &_00d231f8, -1 };
static const node_t _00d1d758 = { NULL, NULL, 14 }; /* "," 1010110 2548 */
static const node_t _00d1d0c8 = { NULL, NULL, 15 }; /* "-" 1010111000 300 */
static const node_t _00d1db18 = { NULL, NULL, 16 }; /* ">=" 1010111001 319 */
static const node_t _00d1b930 = { &_00d1d0c8, &_00d1db18, -1 };
static const node_t _00d1d118 = { NULL, NULL, 17 }; /* "and" 101011101 626 */
static const node_t _00d1ba20 = { &_00d1b930, &_00d1d118, -1 };
static const node_t _00d26ee8 = { NULL, NULL, 18 }; /* ">" 1010111100 326 */
static const node_t _00d1cf10 = { NULL, NULL, 19 }; /* "~=" 101011110100 68 */
static const node_t _00d24580 = { NULL, NULL, 20 }; /* "&" 10101111010100 20 */
static const node_t _00d1cee8 = { NULL, NULL, 21 }; /* "return" 10101111010101 21 */
static const node_t _00d1d410 = { &_00d24580, &_00d1cee8, -1 };
static const node_t _00d24670 = { NULL, NULL, 22 }; /* "break" 10101111010110000000 0 */
static const node_t _00d246e8 = { NULL, NULL, 23 }; /* "<<" 10101111010110000001 0 */
static const node_t _00d1d258 = { &_00d24670, &_00d246e8, -1 };
static const node_t _00d1d140 = { NULL, NULL, 24 }; /* "..." 10101111010110000010 0 */
static const node_t _00d1d5c8 = { NULL, NULL, 25 }; /* "eof" 101011110101100000110 0 */
static const node_t _00d1d640 = { NULL, NULL, 26 }; /* ";" 101011110101100000111 0 */
static const node_t _00d1d190 = { &_00d1d5c8, &_00d1d640, -1 };
static const node_t _00d1d280 = { &_00d1d140, &_00d1d190, -1 };
static const node_t _00d1d2f8 = { &_00d1d258, &_00d1d280, -1 };
static const node_t _00d1d668 = { NULL, NULL, 27 }; /* "::" 101011110101100001000 0 */
static const node_t _00d1d690 = { NULL, NULL, 28 }; /* "in" 101011110101100001001 0 */
static const node_t _00d1d1b8 = { &_00d1d668, &_00d1d690, -1 };
static const node_t _00d1d6b8 = { NULL, NULL, 29 }; /* ">>" 101011110101100001010 0 */
static const node_t _00d1d7f8 = { NULL, NULL, 30 }; /* "goto" 101011110101100001011 0 */
static const node_t _00d1d1e0 = { &_00d1d6b8, &_00d1d7f8, -1 };
static const node_t _00d1d2a8 = { &_00d1d1b8, &_00d1d1e0, -1 };
static const node_t _00d1d820 = { NULL, NULL, 31 }; /* "#" 101011110101100001100 0 */
static const node_t _00d1cec0 = { NULL, NULL, 32 }; /* "|" 101011110101100001101 0 */
static const node_t _00d1d208 = { &_00d1d820, &_00d1cec0, -1 };
static const node_t _00d24558 = { NULL, NULL, 33 }; /* "~" 101011110101100001110 0 */
static const node_t _00d24620 = { NULL, NULL, 34 }; /* "/" 101011110101100001111 0 */
static const node_t _00d1d230 = { &_00d24558, &_00d24620, -1 };
static const node_t _00d1d2d0 = { &_00d1d208, &_00d1d230, -1 };
static const node_t _00d1d320 = { &_00d1d2a8, &_00d1d2d0, -1 };
static const node_t _00d1d348 = { &_00d1d2f8, &_00d1d320, -1 };
static const node_t _00d246c0 = { NULL, NULL, 35 }; /* "^" 10101111010110001 3 */
static const node_t _00d1d370 = { &_00d1d348, &_00d246c0, -1 };
static const node_t _00d1daf0 = { NULL, NULL, 36 }; /* "until" 1010111101011001 7 */
static const node_t _00d1d398 = { &_00d1d370, &_00d1daf0, -1 };
static const node_t _00d1d7d0 = { NULL, NULL, 37 }; /* "repeat" 1010111101011010 7 */
static const node_t _00d1db68 = { NULL, NULL, 38 }; /* ".." 1010111101011011 8 */
static const node_t _00d1d3c0 = { &_00d1d7d0, &_00d1db68, -1 };
static const node_t _00d1d3e8 = { &_00d1d398, &_00d1d3c0, -1 };
static const node_t _00d1d028 = { NULL, NULL, 39 }; /* "*" 10101111010111 29 */
static const node_t _00d1d438 = { &_00d1d3e8, &_00d1d028, -1 };
static const node_t _00d18690 = { &_00d1d410, &_00d1d438, -1 };
static const node_t _00d186b8 = { &_00d1cf10, &_00d18690, -1 };
static const node_t _00d26e70 = { NULL, NULL, 40 }; /* ":" 10101111011 172 */
static const node_t _00d1d870 = { &_00d186b8, &_00d26e70, -1 };
static const node_t _00d1b958 = { &_00d26ee8, &_00d1d870, -1 };
static const node_t _00d1cf38 = { NULL, NULL, 41 }; /* "elseif" 101011111 662 */
static const node_t _00d23158 = { &_00d1b958, &_00d1cf38, -1 };
static const node_t _00d23220 = { &_00d1ba20, &_00d23158, -1 };
static const node_t _00d232c0 = { &_00d1d758, &_00d23220, -1 };
static const node_t _00d23360 = { &_00d23298, &_00d232c0, -1 };
static const node_t _00d233d8 = { &_00d23338, &_00d23360, -1 };
static const node_t _00d1d618 = { NULL, NULL, 42 }; /* "self" 1011 24192 */
static const node_t _00d17d08 = { &_00d233d8, &_00d1d618, -1 };
static const node_t _00d17d58 = { &_00d26e98, &_00d17d08, -1 };
static const node_t _00d1d050 = { NULL, NULL, 43 }; /* "then" 1100000 2578 */
static const node_t _00d1d5f0 = { NULL, NULL, 44 }; /* "true" 1100001 2799 */
static const node_t _00d232e8 = { &_00d1d050, &_00d1d5f0, -1 };
static const node_t _00d245f8 = { NULL, NULL, 45 }; /* "[" 110001 6496 */
static const node_t _00d23388 = { &_00d232e8, &_00d245f8, -1 };
static const node_t _00d245a8 = { NULL, NULL, 46 }; /* ")" 11001 12659 */
static const node_t _00d17cb8 = { &_00d23388, &_00d245a8, -1 };
static const node_t _00d245d0 = { NULL, NULL, 47 }; /* "(" 11010 12659 */
static const node_t _00d24648 = { NULL, NULL, 48 }; /* "]" 110110 6496 */
static const node_t _00d1cfd8 = { NULL, NULL, 49 }; /* "else" 1101110000 341 */
static const node_t _00d26e48 = { NULL, NULL, 50 }; /* "{" 1101110001 363 */
static const node_t _00d1b980 = { &_00d1cfd8, &_00d26e48, -1 };
static const node_t _00d1d0a0 = { NULL, NULL, 51 }; /* "}" 1101110010 363 */
static const node_t _00d1cfb0 = { NULL, NULL, 52 }; /* "%" 11011100110 190 */
static const node_t _00d1d168 = { NULL, NULL, 53 }; /* "nil" 11011100111 198 */
static const node_t _00d1d898 = { &_00d1cfb0, &_00d1d168, -1 };
static const node_t _00d1b9a8 = { &_00d1d0a0, &_00d1d898, -1 };
static const node_t _00d23180 = { &_00d1b980, &_00d1b9a8, -1 };
static const node_t _00d1d0f0 = { NULL, NULL, 54 }; /* "do" 110111010 828 */
static const node_t _00d1d000 = { NULL, NULL, 55 }; /* "+" 110111011 872 */
static const node_t _00d231a8 = { &_00d1d0f0, &_00d1d000, -1 };
static const node_t _00d23248 = { &_00d23180, &_00d231a8, -1 };
static const node_t _00d24698 = { NULL, NULL, 56 }; /* "false" 1101111 3512 */
static const node_t _00d23310 = { &_00d23248, &_00d24698, -1 };
static const node_t _00d233b0 = { &_00d24648, &_00d23310, -1 };
static const node_t _00d17ce0 = { &_00d245d0, &_00d233b0, -1 };
static const node_t _00d17d30 = { &_00d17cb8, &_00d17ce0, -1 };
static const node_t _00d1d7a8 = { NULL, NULL, 57 }; /* "." 111 62611 */
static const node_t _00d17d80 = { &_00d17d30, &_00d1d7a8, -1 };
static const node_t _00d17da8 = { &_00d17d58, &_00d17d80, -1 };
static const node_t _00d17dd0 = { &_00d1ce98, &_00d17da8, -1 };

static struct{ const char* literal; size_t len; } tokens[] =
{
  { NULL, 0 },
  { "=", 1 },
  { "end", 3 },
  { "if", 2 },
  { "==", 2 },
  { "function", 8 },
  { "not", 3 },
  { "for", 3 },
  { "<=", 2 },
  { "local", 5 },
  { "while", 5 },
  { "//", 2 },
  { "or", 2 },
  { "<", 1 },
  { ",", 1 },
  { "-", 1 },
  { ">=", 2 },
  { "and", 3 },
  { ">", 1 },
  { "~=", 2 },
  { "&", 1 },
  { "return", 6 },
  { "break", 5 },
  { "<<", 2 },
  { "...", 3 },
  { NULL, 0 },
  { ";", 1 },
  { "::", 2 },
  { "in", 2 },
  { ">>", 2 },
  { "goto", 4 },
  { "#", 1 },
  { "|", 1 },
  { "~", 1 },
  { "/", 1 },
  { "^", 1 },
  { "until", 5 },
  { "repeat", 6 },
  { "..", 2 },
  { "*", 1 },
  { ":", 1 },
  { "elseif", 6 },
  { "self", 4 },
  { "then", 4 },
  { "true", 4 },
  { "[", 1 },
  { ")", 1 },
  { "(", 1 },
  { "]", 1 },
  { "else", 4 },
  { "{", 1 },
  { "}", 1 },
  { "%", 1 },
  { "nil", 3 },
  { "do", 2 },
  { "+", 1 },
  { "false", 5 },
  { ".", 1 },
};

#define ROOT    ( &_00d17dd0 )
#define LITERAL 0
#define EOF     25

#endif /* BSTREE_H */
