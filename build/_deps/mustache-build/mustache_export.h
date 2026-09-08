
#ifndef MUSTACHE_EXPORT_H
#define MUSTACHE_EXPORT_H

#ifdef MUSTACHE_STATIC_DEFINE
#  define MUSTACHE_EXPORT
#  define MUSTACHE_NO_EXPORT
#else
#  ifndef MUSTACHE_EXPORT
#    ifdef mustache_EXPORTS
        /* We are building this library */
#      define MUSTACHE_EXPORT 
#    else
        /* We are using this library */
#      define MUSTACHE_EXPORT 
#    endif
#  endif

#  ifndef MUSTACHE_NO_EXPORT
#    define MUSTACHE_NO_EXPORT 
#  endif
#endif

#ifndef MUSTACHE_DEPRECATED
#  define MUSTACHE_DEPRECATED __declspec(deprecated)
#endif

#ifndef MUSTACHE_DEPRECATED_EXPORT
#  define MUSTACHE_DEPRECATED_EXPORT MUSTACHE_EXPORT MUSTACHE_DEPRECATED
#endif

#ifndef MUSTACHE_DEPRECATED_NO_EXPORT
#  define MUSTACHE_DEPRECATED_NO_EXPORT MUSTACHE_NO_EXPORT MUSTACHE_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MUSTACHE_NO_DEPRECATED
#    define MUSTACHE_NO_DEPRECATED
#  endif
#endif

#endif /* MUSTACHE_EXPORT_H */
