#ifndef BSTREE_H
#define BSTREE_H

typedef struct bsnode_t bsnode_t;

struct bsnode_t
{
  const bsnode_t* left;
  const bsnode_t* right;
  int             token;
};

static const bsnode_t _019A3868 = { NULL, NULL, 0 }; /* " " 0 221208 */
static const bsnode_t _019A72D8 = { NULL, NULL, 1 }; /* "." 100 62755 */
static const bsnode_t _019A74E0 = { NULL, NULL, 2 }; /* "=" 1010 29673 */
static const bsnode_t _019998B8 = { NULL, NULL, 3 }; /* "end" 1011000 3844 */
static const bsnode_t _0199A8D0 = { NULL, NULL, 4 }; /* "if" 10110010 1967 */
static const bsnode_t _019A38E0 = { NULL, NULL, 5 }; /* "==" 10110011 2003 */
static const bsnode_t _019A2D50 = { &_0199A8D0, &_019A38E0, -1 };
static const bsnode_t _019A2E18 = { &_019998B8, &_019A2D50, -1 };
static const bsnode_t _0199A6A0 = { NULL, NULL, 6 }; /* "function" 101101000 1006 */
static const bsnode_t _019A74B8 = { NULL, NULL, 7 }; /* "local" 101101001 1096 */
static const bsnode_t _019A2CB0 = { &_0199A6A0, &_019A74B8, -1 };
static const bsnode_t _019A73A0 = { NULL, NULL, 8 }; /* "<=" 1011010100 541 */
static const bsnode_t _019A3930 = { NULL, NULL, 9 }; /* "while" 1011010101 557 */
static const bsnode_t _019A2BC0 = { &_019A73A0, &_019A3930, -1 };
static const bsnode_t _019A7530 = { NULL, NULL, 10 }; /* ":" 10110101100 289 */
static const bsnode_t _019A73C8 = { NULL, NULL, 11 }; /* "<" 10110101101 290 */
static const bsnode_t _019A2AD0 = { &_019A7530, &_019A73C8, -1 };
static const bsnode_t _019A3A48 = { NULL, NULL, 12 }; /* "-" 10110101110 304 */
static const bsnode_t _0199CFB8 = { NULL, NULL, 13 }; /* "for" 10110101111 314 */
static const bsnode_t _019A2AF8 = { &_019A3A48, &_0199CFB8, -1 };
static const bsnode_t _019A2BE8 = { &_019A2AD0, &_019A2AF8, -1 };
static const bsnode_t _019A2CD8 = { &_019A2BC0, &_019A2BE8, -1 };
static const bsnode_t _019A2D78 = { &_019A2CB0, &_019A2CD8, -1 };
static const bsnode_t _01999020 = { NULL, NULL, 14 }; /* "then" 10110110 2637 */
static const bsnode_t _0199A8F8 = { NULL, NULL, 15 }; /* "and" 1011011100 644 */
static const bsnode_t _019A7350 = { NULL, NULL, 16 }; /* ">=" 10110111010 319 */
static const bsnode_t _0199A6C8 = { NULL, NULL, 17 }; /* ">" 10110111011 326 */
static const bsnode_t _019A2B20 = { &_019A7350, &_0199A6C8, -1 };
static const bsnode_t _019A2C10 = { &_0199A8F8, &_019A2B20, -1 };
static const bsnode_t _0199A880 = { NULL, NULL, 18 }; /* "elseif" 1011011110 670 */
static const bsnode_t _01999840 = { NULL, NULL, 19 }; /* "else" 10110111110 381 */
static const bsnode_t _019A3908 = { NULL, NULL, 20 }; /* "%" 101101111110 198 */
static const bsnode_t _019A39D0 = { NULL, NULL, 21 }; /* "nil" 101101111111 201 */
static const bsnode_t _019A7850 = { &_019A3908, &_019A39D0, -1 };
static const bsnode_t _019A2B48 = { &_01999840, &_019A7850, -1 };
static const bsnode_t _019A2C38 = { &_0199A880, &_019A2B48, -1 };
static const bsnode_t _019A2D00 = { &_019A2C10, &_019A2C38, -1 };
static const bsnode_t _019A2DA0 = { &_01999020, &_019A2D00, -1 };
static const bsnode_t _019A2E40 = { &_019A2D78, &_019A2DA0, -1 };
static const bsnode_t _0199E4A8 = { &_019A2E18, &_019A2E40, -1 };
static const bsnode_t _0199A7E0 = { NULL, NULL, 22 }; /* "self" 10111 24196 */
static const bsnode_t _0199E520 = { &_0199E4A8, &_0199A7E0, -1 };
static const bsnode_t _0199E570 = { &_019A74E0, &_0199E520, -1 };
static const bsnode_t _0199D0F8 = { &_019A72D8, &_0199E570, -1 };
static const bsnode_t _019A73F0 = { NULL, NULL, 23 }; /* "\x0a" 1100 45578 */
static const bsnode_t _019A7300 = { NULL, NULL, 24 }; /* "true" 11010000 2799 */
static const bsnode_t _019A3A70 = { NULL, NULL, 25 }; /* "," 11010001 2815 */
static const bsnode_t _019A2DC8 = { &_019A7300, &_019A3A70, -1 };
static const bsnode_t _0199CF90 = { NULL, NULL, 26 }; /* "[" 1101001 6744 */
static const bsnode_t _0199E458 = { &_019A2DC8, &_0199CF90, -1 };
static const bsnode_t _019A39A8 = { NULL, NULL, 27 }; /* ")" 110101 12925 */
static const bsnode_t _0199E4D0 = { &_0199E458, &_019A39A8, -1 };
static const bsnode_t _0199A678 = { NULL, NULL, 28 }; /* "(" 110110 12925 */
static const bsnode_t _0199CFE0 = { NULL, NULL, 29 }; /* "]" 1101110 6744 */
static const bsnode_t _0199A8A8 = { NULL, NULL, 30 }; /* "false" 11011110 3512 */
static const bsnode_t _019A7490 = { NULL, NULL, 31 }; /* "{" 11011111000 429 */
static const bsnode_t _01999908 = { NULL, NULL, 32 }; /* "}" 11011111001 429 */
static const bsnode_t _019A2B70 = { &_019A7490, &_01999908, -1 };
static const bsnode_t _019A7328 = { NULL, NULL, 33 }; /* "do" 1101111101 871 */
static const bsnode_t _019A2C60 = { &_019A2B70, &_019A7328, -1 };
static const bsnode_t _019A39F8 = { NULL, NULL, 34 }; /* "+" 1101111110 900 */
static const bsnode_t _0199A790 = { NULL, NULL, 35 }; /* "~=" 1101111111000 99 */
static const bsnode_t _019A7378 = { NULL, NULL, 36 }; /* "return" 11011111110010 55 */
static const bsnode_t _019A72B0 = { NULL, NULL, 37 }; /* "/" 1101111111001100 13 */
static const bsnode_t _0199D058 = { NULL, NULL, 38 }; /* "^" 1101111111001101 14 */
static const bsnode_t _019A76E8 = { &_019A72B0, &_0199D058, -1 };
static const bsnode_t _019A3890 = { NULL, NULL, 39 }; /* "#" 110111111100111 28 */
static const bsnode_t _019A7738 = { &_019A76E8, &_019A3890, -1 };
static const bsnode_t _019A77B0 = { &_019A7378, &_019A7738, -1 };
static const bsnode_t _019A7800 = { &_0199A790, &_019A77B0, -1 };
static const bsnode_t _0199A718 = { NULL, NULL, 40 }; /* "or" 110111111101 220 */
static const bsnode_t _019A2A80 = { &_019A7800, &_0199A718, -1 };
static const bsnode_t _0199A808 = { NULL, NULL, 41 }; /* "not" 110111111110 244 */
static const bsnode_t _0199A7B8 = { NULL, NULL, 42 }; /* "//" 1101111111110 121 */
static const bsnode_t _0199A920 = { NULL, NULL, 43 }; /* "in" 110111111111100 34 */
static const bsnode_t _019A3A20 = { NULL, NULL, 44 }; /* "*" 110111111111101 35 */
static const bsnode_t _019A7760 = { &_0199A920, &_019A3A20, -1 };
static const bsnode_t _0199F628 = { NULL, NULL, 45 }; /* "until" 11011111111111000 7 */
static const bsnode_t _0199A830 = { NULL, NULL, 46 }; /* "|" 1101111111111100100000 0 */
static const bsnode_t _0199D008 = { NULL, NULL, 47 }; /* "goto" 1101111111111100100001 0 */
static const bsnode_t _019A6D60 = { &_0199A830, &_0199D008, -1 };
static const bsnode_t _0199D030 = { NULL, NULL, 48 }; /* "break" 1101111111111100100010 0 */
static const bsnode_t _019A38B8 = { NULL, NULL, 49 }; /* ">>" 11011111111111001000110 0 */
static const bsnode_t _019A7418 = { NULL, NULL, 50 }; /* "\x09" 11011111111111001000111 0 */
static const bsnode_t _019A6C98 = { &_019A38B8, &_019A7418, -1 };
static const bsnode_t _019A6D88 = { &_0199D030, &_019A6C98, -1 };
static const bsnode_t _019A6E00 = { &_019A6D60, &_019A6D88, -1 };
static const bsnode_t _019A7558 = { NULL, NULL, 51 }; /* "~" 11011111111111001001000 0 */
static const bsnode_t _019A7580 = { NULL, NULL, 52 }; /* "::" 11011111111111001001001 0 */
static const bsnode_t _019A6CC0 = { &_019A7558, &_019A7580, -1 };
static const bsnode_t _01999868 = { NULL, NULL, 53 }; /* ";" 11011111111111001001010 0 */
static const bsnode_t _01999890 = { NULL, NULL, 54 }; /* "eof" 11011111111111001001011 0 */
static const bsnode_t _019A6CE8 = { &_01999868, &_01999890, -1 };
static const bsnode_t _019A6DB0 = { &_019A6CC0, &_019A6CE8, -1 };
static const bsnode_t _019998E0 = { NULL, NULL, 55 }; /* "<<" 11011111111111001001100 0 */
static const bsnode_t _0199A6F0 = { NULL, NULL, 56 }; /* "\x0b" 11011111111111001001101 0 */
static const bsnode_t _019A6D10 = { &_019998E0, &_0199A6F0, -1 };
static const bsnode_t _0199A740 = { NULL, NULL, 57 }; /* "\x0d" 11011111111111001001110 0 */
static const bsnode_t _0199A768 = { NULL, NULL, 58 }; /* "\x0c" 11011111111111001001111 0 */
static const bsnode_t _019A6D38 = { &_0199A740, &_0199A768, -1 };
static const bsnode_t _019A6DD8 = { &_019A6D10, &_019A6D38, -1 };
static const bsnode_t _019A6E28 = { &_019A6DB0, &_019A6DD8, -1 };
static const bsnode_t _019A7648 = { &_019A6E00, &_019A6E28, -1 };
static const bsnode_t _019A7440 = { NULL, NULL, 59 }; /* "..." 1101111111111100101 4 */
static const bsnode_t _019A7670 = { &_019A7648, &_019A7440, -1 };
static const bsnode_t _019A7508 = { NULL, NULL, 60 }; /* "repeat" 110111111111110011 7 */
static const bsnode_t _019A7698 = { &_019A7670, &_019A7508, -1 };
static const bsnode_t _019A76C0 = { &_0199F628, &_019A7698, -1 };
static const bsnode_t _019A3980 = { NULL, NULL, 61 }; /* "&" 1101111111111101 20 */
static const bsnode_t _019A7710 = { &_019A76C0, &_019A3980, -1 };
static const bsnode_t _019A7468 = { NULL, NULL, 62 }; /* ".." 110111111111111 39 */
static const bsnode_t _019A7788 = { &_019A7710, &_019A7468, -1 };
static const bsnode_t _019A77D8 = { &_019A7760, &_019A7788, -1 };
static const bsnode_t _019A7828 = { &_0199A7B8, &_019A77D8, -1 };
static const bsnode_t _019A2AA8 = { &_0199A808, &_019A7828, -1 };
static const bsnode_t _019A2B98 = { &_019A2A80, &_019A2AA8, -1 };
static const bsnode_t _019A2C88 = { &_019A39F8, &_019A2B98, -1 };
static const bsnode_t _019A2D28 = { &_019A2C60, &_019A2C88, -1 };
static const bsnode_t _019A2DF0 = { &_0199A8A8, &_019A2D28, -1 };
static const bsnode_t _0199E480 = { &_0199CFE0, &_019A2DF0, -1 };
static const bsnode_t _0199E4F8 = { &_0199A678, &_0199E480, -1 };
static const bsnode_t _0199E548 = { &_0199E4D0, &_0199E4F8, -1 };
static const bsnode_t _0199D0D0 = { &_019A73F0, &_0199E548, -1 };
static const bsnode_t _0199A858 = { NULL, NULL, 63 }; /* "literal" 111 116390 */
static const bsnode_t _0199D120 = { &_0199D0D0, &_0199A858, -1 };
static const bsnode_t _0199D148 = { &_0199D0F8, &_0199D120, -1 };
static const bsnode_t _0199D170 = { &_019A3868, &_0199D148, -1 };

static struct{ const char* literal; size_t len; } tokens[] =
{
  { " ", 1 },
  { ".", 1 },
  { "=", 1 },
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
  { "then", 4 },
  { "and", 3 },
  { ">=", 2 },
  { ">", 1 },
  { "elseif", 6 },
  { "else", 4 },
  { "%", 1 },
  { "nil", 3 },
  { "self", 4 },
  { "\x0a", 1 },
  { "true", 4 },
  { ",", 1 },
  { "[", 1 },
  { ")", 1 },
  { "(", 1 },
  { "]", 1 },
  { "false", 5 },
  { "{", 1 },
  { "}", 1 },
  { "do", 2 },
  { "+", 1 },
  { "~=", 2 },
  { "return", 6 },
  { "/", 1 },
  { "^", 1 },
  { "#", 1 },
  { "or", 2 },
  { "not", 3 },
  { "//", 2 },
  { "in", 2 },
  { "*", 1 },
  { "until", 5 },
  { "|", 1 },
  { "goto", 4 },
  { "break", 5 },
  { ">>", 2 },
  { "\x09", 1 },
  { "~", 1 },
  { "::", 2 },
  { ";", 1 },
  { NULL, 0 },
  { "<<", 2 },
  { "\x0b", 1 },
  { "\x0d", 1 },
  { "\x0c", 1 },
  { "...", 3 },
  { "repeat", 6 },
  { "&", 1 },
  { "..", 2 },
  { NULL, 0 },
};

#define BS_ROOT    ( &_0199D170 )
#define BS_LITERAL 63
#define BS_EOF     54

#endif /* BSTREE_H */
