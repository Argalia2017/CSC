

#include "begin.h"

#define implicit
#define exports __macro_exports
#define imports static
#define leftvalue &
#define rightvalue &&
#define thiz (*this)
#define self m_self ()
#define fake m_fake ()
#define attr m_attr ()
#define trait struct
#define implement :public
#define extend :private
#define require __macro_requires
#define anonymous __macro_anonymous
#define slice __macro_slice
#define assert __macro_assert
#define dynamic_assert __macro_dynamic_assert
#define unittest_watch __macro_unittest_watch
#define forceinline __macro_forceinline
#define ifnot __macro_ifnot
#define ifswitch __macro_ifswitch
#define discard break
#define typeof __macro_typeof

#ifdef NULL
#define __HAS_NULL

#pragma push_macro ("TRUE")
#undef TRUE
#pragma push_macro ("FALSE")
#undef FALSE
#pragma push_macro ("NULL")
#undef NULL
#endif