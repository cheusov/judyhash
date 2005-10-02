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

judyhash_test.o : main_test.cpp judy_map.h judy_set.h
	$(CXX) -o $@ -I. $(CPPFLAGS) $(CFLAGS_TEST) -c main_test.cpp
judyhash_test : judyhash_test.o
	$(CXX) $(LDFLAGS_TEST) -o $@ judyhash_test.o -L. -lJudy

.PHONY : clean
clean:
	rm -f *.o judyhash_bench judyhash_test expected.txt
	rm -f *.tmp core* *~ semantic.cache judyhash log*

.PHONY : test
test : judyhash_test
	printf "\n\n################# TESTS !!! ######################\n\n"; \
	./judyhash_test "0" >expected.txt && \
	./judyhash_test "1" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (const char *) 1 done" && \
	./judyhash_test "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (const char *) 2 done" && \
	./judyhash_test "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (const char *) 3 done" && \
	./judyhash_test "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (const char *) 4 done" && \
	./judyhash_test "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (const char *) 5 done" && \
	./judyhash_test "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (const char *) 6 done" && \
	\
	printf "\n"; \
	./judyhash_test "10" >expected.txt && \
	./judyhash_test "11" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (std::string) 11 done" && \
	./judyhash_test "12" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (std::string) 12 done" && \
	./judyhash_test "13" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (std::string) 13 done" && \
	./judyhash_test "14" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (std::string) 14 done" && \
	./judyhash_test "15" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (std::string) 15 done" && \
	./judyhash_test "16" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_map (std::string) 16 done" && \
	\
	printf "\n"; \
	./judyhash_test "20" >expected.txt && \
	./judyhash_test "21" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (const char *) 21 done" && \
	./judyhash_test "22" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (const char *) 22 done" && \
	./judyhash_test "23" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (const char *) 23 done" && \
	./judyhash_test "24" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (const char *) 24 done" && \
	./judyhash_test "25" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (const char *) 25 done" && \
	./judyhash_test "26" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (const char *) 26 done" && \
	\
	printf "\n"; \
	./judyhash_test "30" >expected.txt && \
	./judyhash_test "31" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (std::string) 31 done" && \
	./judyhash_test "32" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (std::string) 32 done" && \
	./judyhash_test "33" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (std::string) 33 done" && \
	./judyhash_test "34" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (std::string) 34 done" && \
	./judyhash_test "35" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (std::string) 35 done" && \
	./judyhash_test "36" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judyhash_set (std::string) 36 done" && \
	\
	true
