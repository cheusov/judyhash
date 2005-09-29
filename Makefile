############################################################

# Examples
#CXX=icpc -no-gcc
CXX=g++

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
all : judyhash_test #judyhash_bench

judyhash_test.o : main_test.cpp judyhash.h
	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS_TEST) -c main_test.cpp
judyhash_test : judyhash_test.o
	$(CXX) $(LDFLAGS_TEST) -o $@ judyhash_test.o -L. -lJudy

#judyhash_bench.o : main.cpp judyhash.h
#	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS) -c main.cpp
#judyhash_bench : judyhash_bench.o
#	$(CXX) $(LDFLAGS) -o $@ judyhash.o -L. -lJudy

.PHONY : clean
clean:
	rm -f *.o judyhash_bench judyhash_test expected.txt
	rm -f *.tmp core* *~ semantic.cache judyhash

.PHONY : test
test : judyhash_test
	./judyhash_test "0" >expected.txt && \
	./judyhash_test "1" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test1 done" && \
	./judyhash_test "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test2 done" && \
	./judyhash_test "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test3 done" && \
	./judyhash_test "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test4 done" && \
	./judyhash_test "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test5 done" && \
	./judyhash_test "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test6 done" && \
	\
	./judyhash_test "10" >expected.txt && \
	./judyhash_test "11" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test11 done" && \
	./judyhash_test "12" >res.tmp && diff -u expected.txt res.tmp && \
	echo "test12 done" && \
	true
