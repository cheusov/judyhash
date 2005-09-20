############################################################

# Examples
CXX=g++
#CXX=icc

# Examples
CPPFLAGS=-I/usr/include/boost
#CPPFLAGS=-I/usr/include/boost -I/usr/include/stlport

# Examples
CFLAGS=-O3 -march=i586
CFLAGS_TEST=-O0 -g
#CFLAGS=-O

# Examples
LDFLAGS=
LDFLAGS_TEST=
#LDFLAGS=-L. -lstlport -lm


############################################################

.PHONY : all
all : judyhash #judyhash_bench

judyhash.o : main_test.cpp judyhash.h
	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS_TEST) -c main_test.cpp
judyhash : judyhash.o
	$(CXX) $(LDFLAGS_TEST) -o $@ judyhash.o -L. -lJudy

#judyhash_bench.o : main.cpp judyhash.h
#	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS) -c main.cpp
#judyhash_bench : judyhash_bench.o
#	$(CXX) $(LDFLAGS) -o $@ judyhash.o -L. -lJudy

.PHONY : clean
clean:
	rm -f *.o judyhash_bench judyhash *.tmp core* *~ semantic.cache

.PHONY : test
test : judyhash
	./judyhash "1" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test1 done" && \
	./judyhash "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test2 done" && \
	./judyhash "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test3 done" && \
	./judyhash "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test4 done" && \
	./judyhash "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test5 done" && \
	./judyhash "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test6 done" && \
	true
