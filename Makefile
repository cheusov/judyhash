CXX=icc
#CXX=g++
#CXX=/usr/local/gcc-3/bin/g++
CFLAGS=-O3 -march=i686
#CFLAGS=-O0 -g

all : libjudyhash.so main main2 main-test
main : main.c
	$(CXX) -o $@ $(CFLAGS) $^
main2 : main.c libjudyhash.so
	$(CXX) -o $@ -I. -L. -ljudyhash -lJudy $(CFLAGS) main.c

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
	rm -f libjudyhash.so judyhash.o main main2 main-test main-test2
