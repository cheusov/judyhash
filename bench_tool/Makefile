PROG =		bench_tool
SRCS =		bench_tool.cc slow_compare.cc

MKC_CHECK_HEADERS = sys/time.h sys/resource.h sys/utsname.h

#CXXFLAGS +=	-DNO_BOOST

.include "../libhashfuncs/linkme.mk"
.include "../libjudyhash/linkme.mk"
.include "../libmemused/linkme.mk"

LDADD +=	-lJudy

.include <mkc.prog.mk>
