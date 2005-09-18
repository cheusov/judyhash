############################################################

# Examples
CXX=g++
#CXX=icc

# Examples
CPPFLAGS=-I/usr/include/boost
#CPPFLAGS=-I/usr/include/boost -I/usr/include/stlport

# Examples
CFLAGS=-O3 -march=i586
#CFLAGS=-O

# Examples
LDFLAGS=
#LDFLAGS=-L. -lstlport -lm


############################################################

all : judyhash

judyhash.o : main_test.cpp judyhash.h
	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS) -c main_test.cpp
judyhash : judyhash.o
	$(CXX) $(LDFLAGS) -o $@ judyhash.o -L. -lJudy

.PHONY : clean
clean:
	rm -f *.o judyhash
