PATH.hashfuncs :=	${.PARSEDIR}

CPPFLAGS +=		-I${PATH.hashfuncs}
LDADD +=		-lhashfuncs
DPLIBDIRS +=		${PATH.hashfuncs}
