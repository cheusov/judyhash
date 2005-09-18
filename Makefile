include Makefile.init

all : judyhash

judyhash.o : main_test.cpp judyhash_plusplus.h
	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS) -c main_test.cpp
judyhash : judyhash.o
	$(CXX) $(LDFLAGS) -o $@ judyhash.o -L. -lJudy

.PHONY : clean
clean:
	rm -f *.o judyhash
