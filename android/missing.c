// https://github.com/awong-dev/ndk/blob/master/sources/android/support/src/locale/localeconv.c

#include <limits.h>
#include <locale.h>

struct lconv* localeconv(void) {
  static struct lconv C_LCONV[1] =  { {
    .decimal_point = ".",
    .thousands_sep = "",
    .grouping = "",
    .int_curr_symbol = "",
    .currency_symbol = "",
    .mon_decimal_point = "",
    .mon_thousands_sep = "",
    .mon_grouping = "",
    .positive_sign = "",
    .negative_sign = "",
    .int_frac_digits = CHAR_MAX,
    .frac_digits = CHAR_MAX,
    .p_cs_precedes = CHAR_MAX,
    .p_sep_by_space = CHAR_MAX,
    .n_cs_precedes = CHAR_MAX,
    .n_sep_by_space = CHAR_MAX,
    .p_sign_posn = CHAR_MAX,
    .n_sign_posn = CHAR_MAX,
    .int_p_cs_precedes = CHAR_MAX,
    .int_p_sep_by_space = CHAR_MAX,
    .int_n_cs_precedes = CHAR_MAX,
    .int_n_sep_by_space = CHAR_MAX,
    .int_p_sign_posn = CHAR_MAX,
    .int_n_sign_posn = CHAR_MAX,
  } };

  return C_LCONV;
}

// http://en.wikipedia.org/wiki/Linear_congruential_generator
static unsigned int s_seed;

int rand(void) {
  return s_seed = s_seed * 1103515245 + 12345;
}

void srand(unsigned int seed) {
  s_seed = seed;
}
