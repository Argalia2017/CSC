

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
#undef slice
#undef assert
#undef assume
#undef unittest_watch
#undef noinline
#undef forceinline
#undef ifnot
#undef ifswitch
#undef discard
#undef typeof

#ifdef __CSC_HAS_NULL
#pragma pop_macro ("TRUE")
#pragma pop_macro ("FALSE")
#pragma pop_macro ("NULL")

#undef __CSC_HAS_NULL
#endif