#ifdef ANDROID
/* http://en.wikipedia.org/wiki/Linear_congruential_generator */
static unsigned int s_seed;

int rand(void) { return s_seed = s_seed * 1103515245 + 12345; }
void srand(unsigned int seed) { s_seed = seed; }
#endif
