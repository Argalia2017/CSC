
#include "csc_end.h"

#define implicit
#define exports __macro_exports
#define imports static
#define leftvalue &
#define rightvalue &&
#define thiz (*this)
#define self self_m ()
#define fake fake_m ()
#define expr expr_m ()
#define m1st m1st_m ()
#define m2nd m2nd_m ()
#define m3rd m3rd_m ()
#define trait struct
#define implement :public
#define extend :private
#define require __macro_requires
#define anonymous __macro_anonymous
#define as
#define where __macro_where
#define slice __macro_slice
#define assert __macro_assert
#define assume __macro_assume
#define unittest __macro_unittest
#define avoidinline __macro_avoidinline
#define forceinline __macro_forceinline
#define ifnot __macro_ifnot
#define ifswitch __macro_ifswitch
#define discard break
#define typeof __macro_typeof

#ifdef NULL
#define csc_push_macro_null
#pragma push_macro ("TRUE")
#pragma push_macro ("FALSE")
#pragma push_macro ("NULL")
#pragma push_macro ("ZERO")
#pragma push_macro ("IDEN")
#pragma push_macro ("NONE")
#pragma push_macro ("USED")
#undef TRUE
#undef FALSE
#undef NULL
#undef ZERO
#undef IDEN
#undef NONE
#undef USED
#endif