

#include "begin.h"

#define implicit
#define exports
#define imports static
#define leftvalue &
#define rightvalue &&
#define self to ()
#define trait struct
#define extend :public
#define implement :public
#define require __macro_requires
#define anonymous __macro_anonymous
#define slice __macro_slice
#define assert __macro_assert
#define dynamic_assert __macro_dynamic_assert
#define dynamic_watch __macro_dynamic_watch
#define ifnot __macro_ifnot
#define ifswitch __macro_ifswitch
#define discard break
#define typeof __macro_typeof

#pragma push_macro ("TRUE")
#undef TRUE
#pragma push_macro ("FALSE")
#undef FALSE
#pragma push_macro ("NULL")
#undef NULL