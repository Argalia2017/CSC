
#undef implicit
#undef exports
#undef imports
#undef forceinline
#undef leftvalue
#undef rightvalue
#undef thiz
#undef ref
#undef self
#undef expr
#undef trait
#undef implement
#undef require
#undef anonymous
#undef slice
#undef assert
#undef assume
#undef notice
#undef ifdo
#undef discard
#undef typeof
#undef nullof

#ifdef csc_push_macro
#pragma pop_macro ("TRUE")
#pragma pop_macro ("FALSE")
#pragma pop_macro ("NULL")
#pragma pop_macro ("ZERO")
#pragma pop_macro ("IDEN")
#pragma pop_macro ("NONE")
#pragma pop_macro ("USED")
#undef csc_push_macro
#endif

#ifdef __CSC_COMPILER_MSVC__
#pragma warning (push)
#define NOISY_WARNINGS 4263 4264 4355 4661 4819 4946 5204 5214 5054 5219 5220 5266 5267 6201 6255 6269 6294 26439 26450 26495 26813 26820
#pragma warning (disable : NOISY_WARNINGS)
#endif

#ifdef __CSC_COMPILER_GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wdeprecated"
#pragma GCC diagnostic ignored "-Wsuggest-override"
#endif

#ifdef __CSC_COMPILER_CLANG__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wdeprecated"
#pragma clang diagnostic ignored "-Wsuggest-override"
#endif