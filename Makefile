SUBPRJ  = libhashfuncs:memused \
           libjudyhash:memused \
            libmemused:memused

SUBPRJ += libhashfuncs:selftest \
           libjudyhash:selftest

SUBPRJ += libhashfuncs:bench_tool \
           libjudyhash:bench_tool

SUBPRJ += selftest:test

SUBPRJ += bench_tool:bench

.include <mkc.subprj.mk>
