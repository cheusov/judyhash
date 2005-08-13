#CXX=icc
#CC=icc
LD=g++
CXX=g++
CC=gcc
#CXX=/usr/local/gcc-3/bin/g++
#CC=/usr/local/gcc-3/bin/gcc
CFLAGS=-O3 -march=i586
#CFLAGS=-O0 -g

LDFLAGS=-L.
#CPPFLAGS=-I/usr/include/stlport
#LDFLAGS=-L. -lstlport -lm

all : libjudyhash.so main main2 main-test main-plusplus

main.o : main.c
	$(CC) -o $@ -D_GNU_SOURCE -c $(CPPFLAGS) $(CFLAGS) $^
main : main.o
	$(CC) $(LDFLAGS) -o $@ $^

main2.o : main.c
	$(CC) -o $@ -I. -DUSE_JUDY_HASH -c $(CPPFLAGS) $(CFLAGS) $<
main2 : main2.o libjudyhash.so
	$(CC) $(LDFLAGS) -o $@ -L. -ljudyhash -lJudy main2.o

main-plusplus.o : main.cpp judyhash_plusplus.h
	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS) -c main.cpp
main-plusplus : main-plusplus.o
	$(CXX) $(LDFLAGS) -o $@ main-plusplus.o -L. -lJudy

main-test2.o : main-test.c libjudyhash.so
	$(CC) -c -o $@ -I. $(CPPFLAGS) $(CFLAGS) main-test.c
main-test2 : main-test.o libjudyhash.so
	$(CC) -o $@ $(LDFLAGS) -L. -ljudyhash -lJudy main-test.o

main-test.o : main-test.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<
main-test : main-test.o
	$(CC) $(LDFLAGS) -o $@ $^

judyhash.o : judyhash.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<
libjudyhash.so : judyhash.o
	$(LD) $(LDFLAGS) -shared -o $@ $^

.PHONY : clean
clean:
	rm -f libjudyhash.so judyhash.o main main2 main-test main-test2 \
		main-plusplus* *.o
