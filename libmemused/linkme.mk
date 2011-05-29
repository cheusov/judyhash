PATH.memused :=	${.PARSEDIR}

CPPFLAGS +=		-I${PATH.memused}
LDADD +=		-lmemused
DPLIBDIRS +=		${PATH.memused}
