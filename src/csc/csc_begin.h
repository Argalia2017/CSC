#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic pop
#endif

#include "csc_end.h"

#define implicit
#define exports __macro_exports
#define imports static
#define forceinline __macro_forceinline
#define leftvalue &
#define rightvalue &&
#define thiz (*this)
#define self self_m ()
#define fake thiz.fake_m ()
#define expr expr_m ()
#define trait struct
#define implement :public
#define require __macro_requires
#define as __macro_as
#define anonymous __macro_anonymous
#define slice __macro_slice
#define assert __macro_assert
#define assume __macro_assume
#define barrier __macro_barrier
#define ifnot __macro_ifnot
#define ifdo __macro_ifdo
#define discard break
#define typeof __macro_typeof

#ifdef NULL
#define csc_push_macro
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

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (pop)
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic pop
#endif