PROG =		memused
SRCS =		memused.cc

MKC_REQUIRE_HEADERS=	Judy.h
MKC_REQUIRE_FUNCLIBS=	JudyLIns:Judy

.include "../libhashfuncs/linkme.mk"
.include "../libmemused/linkme.mk"
.include "../libjudyhash/linkme.mk"

.include <mkc.prog.mk>
