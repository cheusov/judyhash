#CXX=icc
#CXX=g++
CXX=/usr/local/gcc-3/bin/g++
CFLAGS=-O3 -march=i686
#CFLAGS=-O0 -g

all : libjudyhash.so main main2 main-test main-plusplus

main.o : main.c
	$(CC) -o $@ -D_GNU_SOURCE -c $(CFLAGS) $^
main : main.o
	$(CC) -o $@ $^

main2.o : main.c
	$(CC) -o $@ -I. -DUSE_JUDY_HASH -c $(CFLAGS) $<
main2 : main2.o libjudyhash.so
	$(CC) -o $@ -L. -ljudyhash -lJudy main2.o

main-plusplus : main-plusplus.o
	$(CXX) -o $@ main-plusplus.o -L. -lJudy
main-plusplus.o : main.cpp judyhash_plusplus.h
	$(CXX) -o $@ -I. $(CFLAGS) -c main.cpp

main-test : main-test.c
	$(CXX) -o $@ $(CFLAGS) $^
main-test2 : main-test.c libjudyhash.so
	$(CXX) -o $@ -I. -L. -ljudyhash -lJudy $(CFLAGS) main-test.c

libjudyhash.so : judyhash.o
	$(CXX) -shared -static -o $@ $^
judyhash.o : judyhash.cpp
	$(CXX) -c -o $@ $(CFLAGS) $<

.PHONY : clean
clean:
	rm -f libjudyhash.so judyhash.o main main2 main-test main-test2 \
		main-plusplus* *.o
