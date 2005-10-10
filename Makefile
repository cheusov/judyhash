############################################################

# Examples
#CXX=icpc
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

judyhash_test.o : main_test.cpp *.h judyarray/*.h
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
	echo "judy_map_l <const char *, int, ...> 1 done" && \
	./judyhash_test "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 2 done" && \
	./judyhash_test "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 3 done" && \
	./judyhash_test "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 4 done" && \
	./judyhash_test "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 5 done" && \
	./judyhash_test "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 6 done" && \
	\
	printf "\n" && \
	./judyhash_test "10" >expected.txt && \
	./judyhash_test "11" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 11 done" && \
	./judyhash_test "12" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 12 done" && \
	./judyhash_test "13" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 13 done" && \
	./judyhash_test "14" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 14 done" && \
	./judyhash_test "15" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 15 done" && \
	./judyhash_test "16" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 16 done" && \
	\
	printf "\n" && \
	./judyhash_test "20" >expected.txt && \
	./judyhash_test "21" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 21 done" && \
	./judyhash_test "22" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 22 done" && \
	./judyhash_test "23" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 23 done" && \
	./judyhash_test "24" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 24 done" && \
	./judyhash_test "25" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 25 done" && \
	./judyhash_test "26" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 26 done" && \
	\
	printf "\n" && \
	./judyhash_test "30" >expected.txt && \
	./judyhash_test "31" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 31 done" && \
	./judyhash_test "32" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 32 done" && \
	./judyhash_test "33" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 33 done" && \
	./judyhash_test "34" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 34 done" && \
	./judyhash_test "35" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 35 done" && \
	./judyhash_test "36" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 36 done" && \
	\
	printf "\n" && \
	./judyhash_test "40" >expected.txt && \
	./judyhash_test "41" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_cell <const char *> 41 done" && \
	\
	printf "\n" && \
	./judyhash_test "50" >expected.txt && \
	./judyhash_test "51" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_kcell_dcell <const char *, int> 51 done" && \
	\
	printf "\n" && \
	./judyhash_test "101" >/dev/null && \
	echo "judy_set_cell <int> 101 done" && \
	printf "\n" && \
	./judyhash_test "102" >/dev/null && \
	echo "judy_set_l <int> 102 done" && \
	printf "\n" && \
	./judyhash_test "103" >/dev/null && \
	echo "judy_map_kcell_dcell <int, int> 103 done" && \
	printf "\n" && \
	./judyhash_test "104" >/dev/null && \
	echo "judy_map_l <int, int> 104 done" && \
	\
	true
