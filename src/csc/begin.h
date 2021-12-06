

#undef implicit
#undef exports
#undef imports
#undef leftvalue
#undef rightvalue
#undef thiz
#undef self
#undef fake
#undef attr
#undef trait
#undef implement
#undef extend
#undef require
#undef anonymous
#undef slice
#undef assert
#undef dynamic_assert
#undef forceinline
#undef unittest_watch
#undef ifnot
#undef ifswitch
#undef discard
#undef typeof

#ifdef __HAS_NULL
#pragma pop_macro ("TRUE")
#pragma pop_macro ("FALSE")
#pragma pop_macro ("NULL")

#undef __HAS_NULL
#endif