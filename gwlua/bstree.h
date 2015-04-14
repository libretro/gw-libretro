#ifndef BSTREE_H
#define BSTREE_H

typedef struct node_t node_t;

struct node_t
{
  const node_t* left;
  const node_t* right;
  char          token;
};

static const node_t _0177CF18 = { NULL, NULL, 0 }; /* "literal" 0 114491 */
static const node_t _0177F2B8 = { NULL, NULL, 1 }; /* "=" 100 29375 */
static const node_t _0177EED0 = { NULL, NULL, 2 }; /* "end" 101000 3769 */
static const node_t _0177CF90 = { NULL, NULL, 3 }; /* "if" 1010010 1916 */
static const node_t _01786DD8 = { NULL, NULL, 4 }; /* "==" 1010011 1987 */
static const node_t _01781540 = { &_0177CF90, &_01786DD8, -1 };
static const node_t _01780438 = { &_0177EED0, &_01781540, -1 };
static const node_t _0177F290 = { NULL, NULL, 5 }; /* "function" 10101000 1025 */
static const node_t _01782CB0 = { NULL, NULL, 6 }; /* "not" 1010100100 225 */
static const node_t _0177CFE0 = { NULL, NULL, 7 }; /* "for" 1010100101 273 */
static const node_t _0177BB68 = { &_01782CB0, &_0177CFE0, -1 };
static const node_t _01786E00 = { NULL, NULL, 8 }; /* "<=" 101010011 541 */
static const node_t _01784EC0 = { &_0177BB68, &_01786E00, -1 };
static const node_t _01787A30 = { &_0177F290, &_01784EC0, -1 };
static const node_t _0177E8E0 = { NULL, NULL, 9 }; /* "local" 10101010 1045 */
static const node_t _01786E50 = { NULL, NULL, 10 }; /* "while" 101010110 555 */
static const node_t _01786E78 = { NULL, NULL, 11 }; /* "//" 10101011100 121 */
static const node_t _01788930 = { NULL, NULL, 12 }; /* "or" 10101011101 155 */
static const node_t _0177BAF0 = { &_01786E78, &_01788930, -1 };
static const node_t _0177F0D8 = { NULL, NULL, 13 }; /* "<" 1010101111 286 */
static const node_t _0177BB90 = { &_0177BAF0, &_0177F0D8, -1 };
static const node_t _01784EE8 = { &_01786E50, &_0177BB90, -1 };
static const node_t _01787A58 = { &_0177E8E0, &_01784EE8, -1 };
static const node_t _01781568 = { &_01787A30, &_01787A58, -1 };
static const node_t _01786F40 = { NULL, NULL, 14 }; /* "," 1010110 2548 */
static const node_t _01786F18 = { NULL, NULL, 15 }; /* "-" 1010111000 300 */
static const node_t _01786DB0 = { NULL, NULL, 16 }; /* ">=" 1010111001 319 */
static const node_t _0177BBB8 = { &_01786F18, &_01786DB0, -1 };
static const node_t _0177CFB8 = { NULL, NULL, 17 }; /* "and" 101011101 626 */
static const node_t _01781450 = { &_0177BBB8, &_0177CFB8, -1 };
static const node_t _0177F128 = { NULL, NULL, 18 }; /* ">" 1010111100 326 */
static const node_t _01788908 = { NULL, NULL, 19 }; /* "~=" 101011110100 68 */
static const node_t _017888E0 = { NULL, NULL, 20 }; /* "&" 10101111010100 20 */
static const node_t _0177E7C8 = { NULL, NULL, 21 }; /* "return" 10101111010101 21 */
static const node_t _0177BA50 = { &_017888E0, &_0177E7C8, -1 };
static const node_t _0177F1F0 = { NULL, NULL, 22 }; /* "in" 10101111010110000000 0 */
static const node_t _0177F268 = { NULL, NULL, 23 }; /* "break" 10101111010110000001 0 */
static const node_t _01787788 = { &_0177F1F0, &_0177F268, -1 };
static const node_t _0177D0A8 = { NULL, NULL, 24 }; /* "goto" 10101111010110000010 0 */
static const node_t _01786F68 = { NULL, NULL, 25 }; /* "/" 101011110101100000110 0 */
static const node_t _0177E7F0 = { NULL, NULL, 26 }; /* "#" 101011110101100000111 0 */
static const node_t _0177D120 = { &_01786F68, &_0177E7F0, -1 };
static const node_t _017877B0 = { &_0177D0A8, &_0177D120, -1 };
static const node_t _01787828 = { &_01787788, &_017877B0, -1 };
static const node_t _0177E818 = { NULL, NULL, 27 }; /* "..." 101011110101100001000 0 */
static const node_t _0177E840 = { NULL, NULL, 28 }; /* ";" 101011110101100001001 0 */
static const node_t _0177AE70 = { &_0177E818, &_0177E840, -1 };
static const node_t _0177E868 = { NULL, NULL, 29 }; /* "|" 101011110101100001010 0 */
static const node_t _0177E890 = { NULL, NULL, 30 }; /* ">>" 101011110101100001011 0 */
static const node_t _01786298 = { &_0177E868, &_0177E890, -1 };
static const node_t _017877D8 = { &_0177AE70, &_01786298, -1 };
static const node_t _0177F100 = { NULL, NULL, 31 }; /* "<<" 101011110101100001100 0 */
static const node_t _0177F150 = { NULL, NULL, 32 }; /* "::" 101011110101100001101 0 */
static const node_t _01787738 = { &_0177F100, &_0177F150, -1 };
static const node_t _0177F1A0 = { NULL, NULL, 33 }; /* "~" 101011110101100001110 0 */
static const node_t _0177F1C8 = { NULL, NULL, 34 }; /* "eof" 101011110101100001111 0 */
static const node_t _01787760 = { &_0177F1A0, &_0177F1C8, -1 };
static const node_t _01787800 = { &_01787738, &_01787760, -1 };
static const node_t _01787850 = { &_017877D8, &_01787800, -1 };
static const node_t _01787878 = { &_01787828, &_01787850, -1 };
static const node_t _0177D0F8 = { NULL, NULL, 35 }; /* "^" 10101111010110001 3 */
static const node_t _017878A0 = { &_01787878, &_0177D0F8, -1 };
static const node_t _0177EEF8 = { NULL, NULL, 36 }; /* "repeat" 1010111101011001 7 */
static const node_t _017878C8 = { &_017878A0, &_0177EEF8, -1 };
static const node_t _0177D008 = { NULL, NULL, 37 }; /* "until" 1010111101011010 7 */
static const node_t _0177F218 = { NULL, NULL, 38 }; /* ".." 1010111101011011 8 */
static const node_t _017878F0 = { &_0177D008, &_0177F218, -1 };
static const node_t _01787918 = { &_017878C8, &_017878F0, -1 };
static const node_t _01786EF0 = { NULL, NULL, 39 }; /* "*" 10101111010111 29 */
static const node_t _0177BA78 = { &_01787918, &_01786EF0, -1 };
static const node_t _0177BAA0 = { &_0177BA50, &_0177BA78, -1 };
static const node_t _0177BAC8 = { &_01788908, &_0177BAA0, -1 };
static const node_t _0177D0D0 = { NULL, NULL, 40 }; /* ":" 10101111011 172 */
static const node_t _0177BB18 = { &_0177BAC8, &_0177D0D0, -1 };
static const node_t _0177BBE0 = { &_0177F128, &_0177BB18, -1 };
static const node_t _0177CF40 = { NULL, NULL, 41 }; /* "elseif" 101011111 662 */
static const node_t _017879B8 = { &_0177BBE0, &_0177CF40, -1 };
static const node_t _01787A80 = { &_01781450, &_017879B8, -1 };
static const node_t _017832C8 = { &_01786F40, &_01787A80, -1 };
static const node_t _01780460 = { &_01781568, &_017832C8, -1 };
static const node_t _017804D8 = { &_01780438, &_01780460, -1 };
static const node_t _01788958 = { NULL, NULL, 42 }; /* "self" 1011 24192 */
static const node_t _01780550 = { &_017804D8, &_01788958, -1 };
static const node_t _01783020 = { &_0177F2B8, &_01780550, -1 };
static const node_t _0177FAB0 = { NULL, NULL, 43 }; /* "then" 1100000 2578 */
static const node_t _0177E7A0 = { NULL, NULL, 44 }; /* "true" 1100001 2799 */
static const node_t _017803E8 = { &_0177FAB0, &_0177E7A0, -1 };
static const node_t _0177D030 = { NULL, NULL, 45 }; /* "[" 110001 6496 */
static const node_t _01780488 = { &_017803E8, &_0177D030, -1 };
static const node_t _01786E28 = { NULL, NULL, 46 }; /* ")" 11001 12659 */
static const node_t _01780500 = { &_01780488, &_01786E28, -1 };
static const node_t _0177F240 = { NULL, NULL, 47 }; /* "(" 11010 12659 */
static const node_t _0177D080 = { NULL, NULL, 48 }; /* "]" 110110 6496 */
static const node_t _0177F178 = { NULL, NULL, 49 }; /* "else" 1101110000 341 */
static const node_t _0177E8B8 = { NULL, NULL, 50 }; /* "{" 1101110001 363 */
static const node_t _017869C8 = { &_0177F178, &_0177E8B8, -1 };
static const node_t _0177F0B0 = { NULL, NULL, 51 }; /* "}" 1101110010 363 */
static const node_t _0177CEF0 = { NULL, NULL, 52 }; /* "%" 11011100110 190 */
static const node_t _01786EA0 = { NULL, NULL, 53 }; /* "nil" 11011100111 198 */
static const node_t _0177BB40 = { &_0177CEF0, &_01786EA0, -1 };
static const node_t _01784E98 = { &_0177F0B0, &_0177BB40, -1 };
static const node_t _017879E0 = { &_017869C8, &_01784E98, -1 };
static const node_t _0177D058 = { NULL, NULL, 54 }; /* "do" 110111010 828 */
static const node_t _01786EC8 = { NULL, NULL, 55 }; /* "+" 110111011 872 */
static const node_t _01787A08 = { &_0177D058, &_01786EC8, -1 };
static const node_t _01781518 = { &_017879E0, &_01787A08, -1 };
static const node_t _0177CF68 = { NULL, NULL, 56 }; /* "false" 1101111 3512 */
static const node_t _01780410 = { &_01781518, &_0177CF68, -1 };
static const node_t _017804B0 = { &_0177D080, &_01780410, -1 };
static const node_t _01780528 = { &_0177F240, &_017804B0, -1 };
static const node_t _01780578 = { &_01780500, &_01780528, -1 };
static const node_t _0177E778 = { NULL, NULL, 57 }; /* "." 111 62611 */
static const node_t _01783048 = { &_01780578, &_0177E778, -1 };
static const node_t _01783070 = { &_01783020, &_01783048, -1 };
static const node_t _01783098 = { &_0177CF18, &_01783070, -1 };

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
  { "in", 2 },
  { "break", 5 },
  { "goto", 4 },
  { "/", 1 },
  { "#", 1 },
  { "...", 3 },
  { ";", 1 },
  { "|", 1 },
  { ">>", 2 },
  { "<<", 2 },
  { "::", 2 },
  { "~", 1 },
  { NULL, 0 },
  { "^", 1 },
  { "repeat", 6 },
  { "until", 5 },
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

#define ROOT    ( &_01783098 )
#define LITERAL 0
#define EOF     34

#endif /* BSTREE_H */
