LIBDEPS  = libhashfuncs:memused \
           libjudyhash:memused \
            libmemused:memused

LIBDEPS += libhashfuncs:selftest \
           libjudyhash:selftest

LIBDEPS += libhashfuncs:bench_tool \
           libjudyhash:bench_tool \
            libmemused:bench_tool

SUBPRJ += selftest:test

SUBPRJ += bench_tool:bench

MKC_REQD = 0.29.1

.include <mkc.subprj.mk>
