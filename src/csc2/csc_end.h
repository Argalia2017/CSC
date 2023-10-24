
#undef implicit
#undef exports
#undef imports
#undef leftvalue
#undef rightvalue
#undef thiz
#undef self
#undef fake
#undef expr
#undef m1st
#undef m2nd
#undef m3rd
#undef trait
#undef implement
#undef require
#undef anonymous
#undef slice
#undef assert
#undef assume
#undef unittest
#undef forceinline
#undef ifnot
#undef ifswitch
#undef discard
#undef typeof

#ifdef csc_push_macro_null
#pragma pop_macro ("TRUE")
#pragma pop_macro ("FALSE")
#pragma pop_macro ("NULL")
#pragma pop_macro ("ZERO")
#pragma pop_macro ("IDEN")
#pragma pop_macro ("NONE")
#pragma pop_macro ("USED")
#undef csc_push_macro_null
#endif