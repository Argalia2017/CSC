

#ifdef implicit
#undef implicit
#endif
#define implicit

#ifdef exports
#undef exports
#endif
#define exports

#ifdef imports
#undef imports
#endif
#define imports static

#ifdef leftvalue
#undef leftvalue
#endif
#define leftvalue &

#ifdef rightvalue
#undef rightvalue
#endif
#define rightvalue &&

#ifdef self
#undef self
#endif
#define self to ()

#ifdef trait
#undef trait
#endif
#define trait struct

#ifdef require
#undef require
#endif
#define require __macro_requires

#ifdef assert
#undef assert
#endif
#define assert __macro_assert

#ifdef anonymous
#undef anonymous
#endif
#define anonymous __macro_anonymous

#ifdef ifnot
#undef ifnot
#endif
#define ifnot __macro_ifnot

#ifdef ifswitch
#undef ifswitch
#endif
#define ifswitch __macro_ifswitch

#ifdef discard
#undef discard
#endif
#define discard break

#ifdef typeof
#undef typeof
#endif
#define typeof __macro_typeof

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef NULL
#undef NULL
#endif
