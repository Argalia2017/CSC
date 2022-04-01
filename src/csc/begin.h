

#undef implicit
#undef exports
#undef imports
#undef leftvalue
#undef rightvalue
#undef thiz
#undef self
#undef fake
#undef trait
#undef implement
#undef extend
#undef require
#undef anonymous
#undef where
#undef slice
#undef assert
#undef assume
#undef unittest
#undef noinline
#undef forceinline
#undef ifnot
#undef ifswitch
#undef discard
#undef typeof

#ifdef use_define_has_null
#pragma pop_macro ("TRUE")
#pragma pop_macro ("FALSE")
#pragma pop_macro ("NULL")
#undef use_define_has_null
#endif