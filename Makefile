############################################################
include Makefile.config

CPPFLAGS=$(CPPFLAGS_STLPORT) $(CPPFLAGS_BOOST) \
    $(CPPFLAGS_SPARSEHASH) $(CPPFLAGS_COMMON)

CFLAGS_O=-I. $(CPPFLAGS) $(CFLAGS_COMMON) $(CFLAGS_OPT)
CFLAGS_T=-I. $(CPPFLAGS) $(CFLAGS_COMMON) $(CFLAGS_TEST)

LDFLAGS_C=$(LDFLAGS_COMMON) $(LDFLAGS_STLPORT) \
    $(LDFLAGS_JUDY)

LDFLAGS_O=$(LDFLAGS_C)
LDFLAGS_T=$(LDFLAGS_TEST) $(LDFLAGS_C)

############################################################

.PHONY : all
all : judyhash_test

judyhash_test.o : main_test.cpp *.h judyarray/*.h
	$(CXX) -o $@ $(CFLAGS_T) -c main_test.cpp
judyhash_test : judyhash_test.o
	$(CXX) -o $@ $(LDFLAGS_T) judyhash_test.o

.PHONY : clean
clean:
	rm -f *.o judyhash_test expected.txt
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
	./judyhash_test "0" >expected.txt && \
	./judyhash_test "61" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 61 done" && \
	./judyhash_test "62" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 62 done" && \
	./judyhash_test "63" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 63 done" && \
	./judyhash_test "64" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 64 done" && \
	./judyhash_test "65" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 65 done" && \
	./judyhash_test "66" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 66 done" && \
	\
	printf "\n" && \
	./judyhash_test "10" >expected.txt && \
	./judyhash_test "71" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 71 done" && \
	./judyhash_test "72" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 72 done" && \
	./judyhash_test "73" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 73 done" && \
	./judyhash_test "74" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 74 done" && \
	./judyhash_test "75" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 75 done" && \
	./judyhash_test "76" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 76 done" && \
	\
	printf "\n" && \
	./judyhash_test "20" >expected.txt && \
	./judyhash_test "81" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 81 done" && \
	./judyhash_test "82" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 82 done" && \
	./judyhash_test "83" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 83 done" && \
	./judyhash_test "84" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 84 done" && \
	./judyhash_test "85" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 85 done" && \
	./judyhash_test "86" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 86 done" && \
	\
	printf "\n" && \
	./judyhash_test "30" >expected.txt && \
	./judyhash_test "91" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 91 done" && \
	./judyhash_test "92" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 92 done" && \
	./judyhash_test "93" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 93 done" && \
	./judyhash_test "94" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 94 done" && \
	./judyhash_test "95" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 95 done" && \
	./judyhash_test "96" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 96 done" && \
	\
	printf "\n" && \
	./judyhash_test "40" >expected.txt && \
	./judyhash_test "41" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_cell <const char *> 41 done" && \
	\
	printf "\n" && \
	./judyhash_test "50" >expected.txt && \
	./judyhash_test "51" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_kdcell <const char *, int> 51 done" && \
	\
	printf "\n" && \
	./judyhash_test "101" >/dev/null && \
	echo "judy_set_cell <int> 101 done" && \
	printf "\n" && \
	./judyhash_test "102" >/dev/null && \
	echo "judy_set_l <int> 102 done" && \
	printf "\n" && \
	./judyhash_test "103" >/dev/null && \
	echo "judy_map_kdcell <int, int> 103 done" && \
	printf "\n" && \
	./judyhash_test "104" >/dev/null && \
	echo "judy_map_l <int, int> 104 done" && \
	printf "\n" && \
	./judyhash_test "105" >/dev/null && \
	echo "judy_map_m <int, int> 105 done" && \
	printf "\n" && \
	./judyhash_test "106" >/dev/null && \
	echo "judy_set_m <int, int> 106 done" && \
	\
	true
