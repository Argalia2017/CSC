

#undef implicit
#undef exports
#undef imports
#undef leftvalue
#undef rightvalue
#undef self
#undef trait
#undef implement
#undef extend
#undef require
#undef anonymous
#undef slice
#undef assert
#undef dynamic_assert
#undef dynamic_watch
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