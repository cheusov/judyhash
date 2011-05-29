PROG =		bench_tool
SRCS =		bench_tool.cc slow_compare.cc

#CXXFLAGS +=	-DNO_BOOST

.include "../libhashfuncs/linkme.mk"
.include "../libjudyhash/linkme.mk"
.include "../libmemused/linkme.mk"

LDADD +=	-lJudy

.include <mkc.prog.mk>
