
#ifdef DEBUG
#include <assert.h>
#else
#define assert(x)
#endif


#define pre_condition(x) assert(x != 0)
#define post_condition(x) assert(x != 0)
