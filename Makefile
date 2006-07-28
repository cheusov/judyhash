############################################################
include Makefile.config

############################################################

.PHONY : all
all : selftest

selftest.o : selftest.cc *.h judyarray/*.h
	$(CXX) -o $@ $(CFLAGS_T) -c selftest.cc
selftest : selftest.o
	$(CXX) -o $@ $(LDFLAGS_T) selftest.o

time_hash_map.o : time_hash_map.cc *.h judyarray/*.h
	$(CXX) -o $@ $(CFLAGS_O) -c time_hash_map.cc
time_hash_map : time_hash_map.o slow_compare.o
	$(CXX) -o $@ $(LDFLAGS_O) $^

slow_compare.o : slow_compare.cc
	$(CXX) -o $@ $(CFLAGS_T) -DSLOW_LEVEL=$(SLOW_LEVEL) -c slow_compare.cc

.PHONY : clean
clean:
	rm -f *.o selftest expected.txt *.tmp
	rm -f *.tmp core* *~ semantic.cache judyhash log*

.PHONY : test
test : selftest
	printf "\n\n################# TESTS !!! ######################\n\n"; \
	./selftest "0" >expected.txt && \
	./selftest "1" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 1 done" && \
	./selftest "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 2 done" && \
	./selftest "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 3 done" && \
	./selftest "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 4 done" && \
	./selftest "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 5 done" && \
	./selftest "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 6 done" && \
	\
	printf "\n" && \
	./selftest "10" >expected.txt && \
	./selftest "11" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 11 done" && \
	./selftest "12" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 12 done" && \
	./selftest "13" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 13 done" && \
	./selftest "14" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 14 done" && \
	./selftest "15" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 15 done" && \
	./selftest "16" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 16 done" && \
	\
	printf "\n" && \
	./selftest "20" >expected.txt && \
	./selftest "21" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 21 done" && \
	./selftest "22" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 22 done" && \
	./selftest "23" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 23 done" && \
	./selftest "24" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 24 done" && \
	./selftest "25" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 25 done" && \
	./selftest "26" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 26 done" && \
	\
	printf "\n" && \
	./selftest "30" >expected.txt && \
	./selftest "31" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 31 done" && \
	./selftest "32" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 32 done" && \
	./selftest "33" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 33 done" && \
	./selftest "34" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 34 done" && \
	./selftest "35" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 35 done" && \
	./selftest "36" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 36 done" && \
	\
	printf "\n" && \
	./selftest "0" >expected.txt && \
	./selftest "61" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 61 done" && \
	./selftest "62" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 62 done" && \
	./selftest "63" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 63 done" && \
	./selftest "64" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 64 done" && \
	./selftest "65" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 65 done" && \
	./selftest "66" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 66 done" && \
	\
	printf "\n" && \
	./selftest "10" >expected.txt && \
	./selftest "71" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 71 done" && \
	./selftest "72" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 72 done" && \
	./selftest "73" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 73 done" && \
	./selftest "74" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 74 done" && \
	./selftest "75" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 75 done" && \
	./selftest "76" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 76 done" && \
	\
	printf "\n" && \
	./selftest "20" >expected.txt && \
	./selftest "81" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 81 done" && \
	./selftest "82" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 82 done" && \
	./selftest "83" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 83 done" && \
	./selftest "84" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 84 done" && \
	./selftest "85" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 85 done" && \
	./selftest "86" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 86 done" && \
	\
	printf "\n" && \
	./selftest "30" >expected.txt && \
	./selftest "91" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 91 done" && \
	./selftest "92" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 92 done" && \
	./selftest "93" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 93 done" && \
	./selftest "94" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 94 done" && \
	./selftest "95" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 95 done" && \
	./selftest "96" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 96 done" && \
	\
	printf "\n" && \
	./selftest "40" >expected.txt && \
	./selftest "41" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_cell <const char *> 41 done" && \
	\
	printf "\n" && \
	./selftest "50" >expected.txt && \
	./selftest "51" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_kdcell <const char *, int> 51 done" && \
	\
	printf "\n" && \
	./selftest "101" >/dev/null && \
	echo "judy_set_cell <int> 101 done" && \
	printf "\n" && \
	./selftest "102" >/dev/null && \
	echo "judy_set_l <int> 102 done" && \
	printf "\n" && \
	./selftest "103" >/dev/null && \
	echo "judy_map_kdcell <int, int> 103 done" && \
	printf "\n" && \
	./selftest "104" >/dev/null && \
	echo "judy_map_l <int, int> 104 done" && \
	printf "\n" && \
	./selftest "105" >/dev/null && \
	echo "judy_map_m <int, int> 105 done" && \
	printf "\n" && \
	./selftest "106" >/dev/null && \
	echo "judy_set_m <int, int> 106 done" && \
	\
	true

.PHONY : bench
bench: bench_count bench_slowness

.PHONY : bench_count
bench_count : bench_count.tmp
bench_count.tmp : time_hash_map
	./run_bench count | tee bench_count.tmp

.PHONY : bench_slowness
bench_slowness : bench_slowness.tmp
bench_slowness.tmp : time_hash_map
	./run_bench slowness | tee bench_slowness.tmp
