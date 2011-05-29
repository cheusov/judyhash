selftest_cmd =	../selftest/selftest

.PHONY : all
all:
	printf "\n\n################# TESTS !!! ######################\n\n"; \
	${selftest_cmd} "0" >expected.txt && \
	${selftest_cmd} "1" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 1 done" && \
	${selftest_cmd} "2" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 2 done" && \
	${selftest_cmd} "3" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 3 done" && \
	${selftest_cmd} "4" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 4 done" && \
	${selftest_cmd} "5" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 5 done" && \
	${selftest_cmd} "6" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <const char *, int, ...> 6 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "10" >expected.txt && \
	${selftest_cmd} "11" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 11 done" && \
	${selftest_cmd} "12" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 12 done" && \
	${selftest_cmd} "13" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 13 done" && \
	${selftest_cmd} "14" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 14 done" && \
	${selftest_cmd} "15" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 15 done" && \
	${selftest_cmd} "16" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_l <std::string, int, ...> 16 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "20" >expected.txt && \
	${selftest_cmd} "21" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 21 done" && \
	${selftest_cmd} "22" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 22 done" && \
	${selftest_cmd} "23" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 23 done" && \
	${selftest_cmd} "24" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 24 done" && \
	${selftest_cmd} "25" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 25 done" && \
	${selftest_cmd} "26" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <const char *, ...> 26 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "30" >expected.txt && \
	${selftest_cmd} "31" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 31 done" && \
	${selftest_cmd} "32" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 32 done" && \
	${selftest_cmd} "33" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 33 done" && \
	${selftest_cmd} "34" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 34 done" && \
	${selftest_cmd} "35" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 35 done" && \
	${selftest_cmd} "36" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_l <std::string, ...> 36 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "0" >expected.txt && \
	${selftest_cmd} "61" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 61 done" && \
	${selftest_cmd} "62" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 62 done" && \
	${selftest_cmd} "63" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 63 done" && \
	${selftest_cmd} "64" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 64 done" && \
	${selftest_cmd} "65" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 65 done" && \
	${selftest_cmd} "66" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <const char *, int, ...> 66 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "10" >expected.txt && \
	${selftest_cmd} "71" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 71 done" && \
	${selftest_cmd} "72" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 72 done" && \
	${selftest_cmd} "73" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 73 done" && \
	${selftest_cmd} "74" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 74 done" && \
	${selftest_cmd} "75" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 75 done" && \
	${selftest_cmd} "76" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_m <std::string, int, ...> 76 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "20" >expected.txt && \
	${selftest_cmd} "81" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 81 done" && \
	${selftest_cmd} "82" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 82 done" && \
	${selftest_cmd} "83" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 83 done" && \
	${selftest_cmd} "84" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 84 done" && \
	${selftest_cmd} "85" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 85 done" && \
	${selftest_cmd} "86" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <const char *, ...> 86 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "30" >expected.txt && \
	${selftest_cmd} "91" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 91 done" && \
	${selftest_cmd} "92" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 92 done" && \
	${selftest_cmd} "93" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 93 done" && \
	${selftest_cmd} "94" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 94 done" && \
	${selftest_cmd} "95" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 95 done" && \
	${selftest_cmd} "96" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_m <std::string, ...> 96 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "40" >expected.txt && \
	${selftest_cmd} "41" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_set_cell <const char *> 41 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "50" >expected.txt && \
	${selftest_cmd} "51" >res.tmp && diff -u expected.txt res.tmp && \
	echo "judy_map_kdcell <const char *, int> 51 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "101" >/dev/null && \
	echo "judy_set_cell <int> 101 done" && \
	printf "\n" && \
	${selftest_cmd} "102" >/dev/null && \
	echo "judy_set_l <int> 102 done" && \
	printf "\n" && \
	${selftest_cmd} "103" >/dev/null && \
	echo "judy_map_kdcell <int, int> 103 done" && \
	printf "\n" && \
	${selftest_cmd} "104" >/dev/null && \
	echo "judy_map_l <int, int> 104 done" && \
	printf "\n" && \
	${selftest_cmd} "105" >/dev/null && \
	echo "judy_map_m <int, int> 105 done" && \
	printf "\n" && \
	${selftest_cmd} "106" >/dev/null && \
	echo "judy_set_m <int, int> 106 done" && \
	\
	printf "\n" && \
	${selftest_cmd} "112" >/dev/null && \
	echo "judy_set_l <int> 102 done" && \
	printf "\n" && \
	${selftest_cmd} "114" >/dev/null && \
	echo "judy_map_l <int, int> 104 done" && \
	printf "\n" && \
	${selftest_cmd} "115" >/dev/null && \
	echo "judy_map_m <int, int> 105 done" && \
	printf "\n" && \
	${selftest_cmd} "116" >/dev/null && \
	echo "judy_set_m <int, int> 106 done" && \
	\
	true

.include <mkc.files.mk>
